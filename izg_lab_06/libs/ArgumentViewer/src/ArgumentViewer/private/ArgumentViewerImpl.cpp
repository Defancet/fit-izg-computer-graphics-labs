#include <ArgumentViewer/Exception.h>
#include <ArgumentViewer/private/ArgumentViewerImpl.h>
#include <ArgumentViewer/private/Globals.h>
#include <ArgumentViewer/private/IsPresentFormat.h>
#include <ArgumentViewer/private/ContextFormat.h>
#include <ArgumentViewer/private/StringVectorFormat.h>

#include <MealyMachine/MealyMachine.h>
#include <ArgumentViewer/ArgumentViewer.h>

using namespace argumentViewer;

bool ArgumentViewerImpl::validate() const{
  if (parent != nullptr)
    throw ex::Exception("validation cannot be run on sub ArgumentViewer");
  size_t index = 0;
  return format->match(arguments, index) == Format::MATCH_SUCCESS;
}

string ArgumentViewerImpl::toStr() const {
  return format->toStr();
}

string ArgumentViewerImpl::getApplicationName() const {
  return applicationName;
}

size_t ArgumentViewerImpl::getNofArguments() const {
  return arguments.size();
}

bool ArgumentViewerImpl::isPresent(string const&argument,string const&com)const{
  auto alf = std::dynamic_pointer_cast<ArgumentListFormat>(format);
  assert(alf != nullptr);

  auto subFormatIt = alf->formats.find(argument);
  if (subFormatIt != alf->formats.end()) {
    auto subFormat = subFormatIt->second;
    if (!isTypeOf<IsPresentFormat>(subFormat))
      throw ex::Exception(
          std::string("argument: ") + argument +
          " is already defined as something else than isPresent format");
  } else
    alf->formats[argument] = std::make_shared<IsPresentFormat>(argument, com);
  if (alf->formats[argument]->comment == "")
    alf->formats[argument]->comment = com;

  return getArgumentPosition(argument) < arguments.size();
}

shared_ptr<ArgumentViewer>ArgumentViewerImpl::getContext(string const&name,string const&com,ArgumentViewer const *_this)const{
  auto alf = std::dynamic_pointer_cast<ArgumentListFormat>(format);
  assert(alf != nullptr);

  auto constructEmptyContext = [&]() {
    char const *argv[]    = {applicationName.c_str()};
    auto        result    = std::make_shared<ArgumentViewer>(1, (char **)argv);
    result->impl->parent = _this;
    result->impl->format = alf->formats[name];
    return result;
  };

  auto subFormatIt = alf->formats.find(name);
  if (subFormatIt != alf->formats.end()) {
    auto subFormat = subFormatIt->second;
    if (!isTypeOf<ContextFormat>(subFormat))
      throw ex::Exception(std::string("argument: ") + name +
                          " is already defined as something else than context");
  } else
    alf->formats[name] = std::make_shared<ContextFormat>(name, com);
  if (alf->formats[name]->comment == "") alf->formats[name]->comment = com;

  std::vector<std::string> subArguments;
  if (!getContext(subArguments, name)) return constructEmptyContext();
  char const *appName[] = {applicationName.c_str()};
  auto        result    = std::make_shared<ArgumentViewer>(1, (char **)appName);
  result->impl->parent = _this;
  result->impl->arguments = subArguments;
  result->impl->format    = alf->formats.at(name);
  return result;
}

vector<string>ArgumentViewerImpl::getsv(string const&argument,vector<string>const&def,string const&com)const{
  auto alf = std::dynamic_pointer_cast<ArgumentListFormat>(format);
  assert(alf != nullptr);

  auto subFormatIt = alf->formats.find(argument);
  if (subFormatIt != alf->formats.end()) {
    auto subFormat = subFormatIt->second;
    auto stringVectorFormat =
        std::dynamic_pointer_cast<StringVectorFormat>(subFormat);
    if (!stringVectorFormat)
      throw ex::Exception(
          std::string("argument: ") + argument +
          " is already defined as something else than vector of string values");
    if (stringVectorFormat->defaults != def)
      throw ex::Exception(
          std::string("argument: ") + argument +
          " has already been defined with different default values: "+txtUtils::valueToString(stringVectorFormat->defaults));
  } else
    alf->formats[argument] =
        std::make_shared<StringVectorFormat>(argument, def, com);
  if (alf->formats[argument]->comment == "")
    alf->formats[argument]->comment = com;

  std::vector<std::string> subArguments;
  if (!getContext(subArguments, argument)) return def;
  while (def.size() > subArguments.size())
    subArguments.push_back(def[subArguments.size()]);
  for (auto &x : subArguments) x = parseEscapeSequence(x);
  return subArguments;
}

size_t ArgumentViewerImpl::getArgumentPosition(
    std::string const &argument) const {
  size_t       argumentIndex  = 0;
  size_t       contextCounter = 0;
  size_t const notFound       = arguments.size();
  for (auto x : arguments) {
    if (x == argument && contextCounter == 0) return argumentIndex;
    if (x == contextBegin) ++contextCounter;
    if (x == contextEnd) {
      if (contextCounter == 0) return notFound;
      --contextCounter;
    }
    ++argumentIndex;
  }
  return notFound;
}

bool ArgumentViewerImpl::getContext(std::vector<std::string> &contextArguments,
                                    std::string const &       argument) const {
  size_t argumentIndex = getArgumentPosition(argument);
  if (!isInRange(argumentIndex++)) return false;
  if (!isInRange(argumentIndex)) return false;
  if (getArgument(argumentIndex++) != contextBegin) return false;
  size_t contextCounter = 0;
  while (isInRange(argumentIndex)) {
    if (getArgument(argumentIndex) == contextEnd) {
      if (contextCounter == 0)
        return true;
      else
        contextCounter--;
    }
    if (getArgument(argumentIndex) == contextBegin)
      contextCounter++;
    contextArguments.push_back(getArgument(argumentIndex));
    ++argumentIndex;
  }
  contextArguments.clear();
  return false;
}


bool ArgumentViewerImpl::isInRange(size_t index) const
{
  assert(this != nullptr);
  return index < arguments.size();
}

std::string ArgumentViewerImpl::getArgument(size_t index) const
{
  assert(this != nullptr);
  assert(index < arguments.size());
  return arguments.at(index);
}

void ArgumentViewerImpl::loadArgumentFiles(
    std::vector<std::string> &args, std::set<std::string> &alreadyLoaded) {
  size_t argumentIndex = 0;
  while (argumentIndex < args.size()) {
    if (args.at(argumentIndex) != fileSymbol) {
      ++argumentIndex;
      continue;
    }
    if (argumentIndex + 1 >= args.size())
      throw ex::Exception(std::string("expected filename after ") + fileSymbol +
                          " not end of arguments/file");
    auto fileName = args.at(argumentIndex + 1);
    if (alreadyLoaded.count(fileName))
      throw ex::Exception(std::string("file: ") + fileName +
                          " contains file loading loop");
    std::vector<std::string> newArgs;
    std::string              fileContent = txtUtils::loadTextFile(fileName);
    splitFileToArguments(newArgs, fileContent);
    alreadyLoaded.insert(fileName);
    loadArgumentFiles(newArgs, alreadyLoaded);
    alreadyLoaded.erase(fileName);
    std::vector<std::string> withLoadedFile;
    for (size_t j = 0; j < argumentIndex; ++j)
      withLoadedFile.push_back(args.at(j));
    for (auto const &x : newArgs) withLoadedFile.push_back(x);
    for (size_t j = argumentIndex + 2; j < args.size(); ++j)
      withLoadedFile.push_back(args.at(j));
    args = withLoadedFile;
    argumentIndex--;
    argumentIndex += newArgs.size();
  }
}

void ArgumentViewerImpl::splitFileToArguments(std::vector<std::string> &args,
                                              std::string const &fileContent) {
  mealyMachine::MealyMachine mm;
  auto                       start        = mm.addState();
  auto                       space        = mm.addState();
  auto                       word         = mm.addState();
  auto                       comment      = mm.addState();
  auto                       startNewWord = [&](mealyMachine::MealyMachine *) {
    args.push_back("");
    args.back() += *(char *)mm.getCurrentSymbol();
  };
  auto addCharToWord = [&](mealyMachine::MealyMachine *) {
    args.back() += *(char *)mm.getCurrentSymbol();
  };
  mm.addTransition(start, " \t\n\r", space);
  mm.addTransition(start, "#", comment);
  mm.addElseTransition(start, word, startNewWord);
  mm.addEOFTransition(start);

  mm.addTransition(space, " \t\n\r", space);
  mm.addTransition(space, "#", comment);
  mm.addElseTransition(space, word, startNewWord);
  mm.addEOFTransition(space);

  mm.addTransition(comment, "\n\r", start);
  mm.addElseTransition(comment, comment);
  mm.addEOFTransition(comment);

  mm.addTransition(word, " \t\n\r", space);
  mm.addTransition(word, "#", comment);
  mm.addElseTransition(word, word, addCharToWord);
  mm.addEOFTransition(word);

  mm.match(fileContent.c_str());
}


