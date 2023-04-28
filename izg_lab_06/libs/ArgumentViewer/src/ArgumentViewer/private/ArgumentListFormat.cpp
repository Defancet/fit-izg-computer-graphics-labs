#include <ArgumentViewer/Exception.h>
#include <ArgumentViewer/private/ArgumentListFormat.h>
#include <ArgumentViewer/private/CommonFunctions.h>
#include <ArgumentViewer/private/ContextFormat.h>
#include <set>
#include <algorithm>

using namespace argumentViewer::ex;

ArgumentListFormat::ArgumentListFormat(string const &com) : Format(com) {}

bool isContextFormat(shared_ptr<Format> const &x)
{
  return isTypeOf<ContextFormat>(x);
}

void updateLengthsIfFoundLarger(size_t &                  nameLength,
                                size_t &                  defaultsLength,
                                size_t &                  typeLength,
                                shared_ptr<Format> const &format)
{
  auto vf = dynamic_pointer_cast<ValueFormat>(format);
  if (!vf) return;
  nameLength     = max(nameLength, vf->getName().length());
  defaultsLength = max(defaultsLength, vf->getDefaultsLength());
  typeLength     = max(typeLength, vf->getType().length());
}

void ArgumentListFormat::getLargestLengths(size_t &nameLength,
                                           size_t &defaultsLength,
                                           size_t &typeLength) const
{
  nameLength     = 0;
  defaultsLength = 0;
  typeLength     = 0;
  for (auto const &x : formats)
    updateLengthsIfFoundLarger(nameLength, defaultsLength, typeLength,
                               x.second);
}

void writeNonContextFormat(stringstream &            ss,
                           shared_ptr<Format> const &format,
                           size_t                    nameLength,
                           size_t                    defaultsLength,
                           size_t                    typeLength,
                           size_t                    indent)
{
  if (isContextFormat(format)) return;
  ss << format->toStr(indent, nameLength, defaultsLength, typeLength);
}

void ArgumentListFormat::writeIndentedNonContextFormats(stringstream &ss,
                                                        size_t nameLength,
                                                        size_t defaultsLength,
                                                        size_t typeLength,
                                                        size_t indent) const
{
  for (auto const &x : formats)
    writeNonContextFormat(ss, x.second, nameLength, defaultsLength, typeLength,
                          indent);
}

void writeContextFormat(stringstream &            ss,
                        shared_ptr<Format> const &format,
                        size_t                    indent)
{
  if (!isContextFormat(format)) return;
  ss << format->toStr(indent);
}

void ArgumentListFormat::writeContextFormats(stringstream &ss,
                                             size_t        indent) const
{
  for (auto const &x : formats) writeContextFormat(ss, x.second, indent);
}

string ArgumentListFormat::toStr(size_t indent, size_t, size_t, size_t) const
{
  stringstream ss;
  size_t       nameLength     = 0;
  size_t       defaultsLength = 0;
  size_t       typeLength     = 0;
  getLargestLengths(nameLength, defaultsLength, typeLength);
  writeIndentedNonContextFormats(ss, nameLength, defaultsLength, typeLength,
                                 indent);
  writeContextFormats(ss, indent);
  return ss.str();
}

void throwIfUnusedFormatsIsEmpty(set<string> const &   unusedFormats,
                                 vector<string> const &args,
                                 size_t &              index)
{
  if (!unusedFormats.empty()) return;
  stringstream ss;
  ss << "Argument error:" << endl;
  ss << "following arguments cannot be matched: " << endl;
  for (size_t i = index; i < args.size(); ++i) ss << args.at(i) << " ";
  throw MatchError(ss.str());
}

void throwIfFormatForRemovalIsEmpty(string const &        formatForRemoval,
                                    vector<string> const &args,
                                    size_t &              index)
{
  if (formatForRemoval != "") return;
  stringstream ss;
  ss << "Argument error:" << endl;
  ss << "argument: " << args.at(index) << " at index: " << index
     << " cannot be matched";
  throw MatchError(ss.str());
}

string ArgumentListFormat::matchOneUnusedFormat(
    set<string> const &   unusedFormats,
    vector<string> const &args,
    size_t &              index) const
{
  for (auto const &f : unusedFormats) {
    auto const status = formats.at(f)->match(args, index);
    if (status == MATCH_SUCCESS) return f;
  }
  return "";
}

set<string> ArgumentListFormat::getUnusedFormats() const
{
  set<string> unusedFormats;
  for (auto const &x : formats) unusedFormats.insert(x.first);
  return unusedFormats;
}

void ArgumentListFormat::checkAndMatchOneUnusedFormat(
    set<string> &         unusedFormats,
    vector<string> const &args,
    size_t &              index) const
{
  throwIfUnusedFormatsIsEmpty(unusedFormats, args, index);
  string formatForRemoval = matchOneUnusedFormat(unusedFormats, args, index);
  throwIfFormatForRemovalIsEmpty(formatForRemoval, args, index);
  unusedFormats.erase(formatForRemoval);
}

void ArgumentListFormat::matchUnusedFormats(set<string> &         unusedFormats,
                                            vector<string> const &args,
                                            size_t &              index) const
{
  while (index < args.size())
    checkAndMatchOneUnusedFormat(unusedFormats, args, index);
}

Format::MatchStatus ArgumentListFormat::match(vector<string> const &args,
                                              size_t &              index) const
{
  auto unusedFormats = getUnusedFormats();
  matchUnusedFormats(unusedFormats, args, index);
  return MATCH_SUCCESS;
}
