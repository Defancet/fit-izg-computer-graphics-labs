#include <ArgumentViewer/Exception.h>
#include <ArgumentViewer/private/ContextFormat.h>
#include <set>

using namespace argumentViewer::ex;

ContextFormat::ContextFormat(string const &argument,
                             string const &com)
    : ArgumentListFormat(com), argumentName(argument) {}

string ContextFormat::toStr(size_t indent, size_t, size_t, size_t) const {
  stringstream ss;
  for (size_t i = 0; i < indent; ++i) ss << " ";
  ss << argumentName << " ";
  ss << contextBegin << " - " << comment << endl;
  ss << ArgumentListFormat::toStr(indent + 2);
  for (size_t i = 0; i < indent; ++i) ss << " ";
  ss << contextEnd << endl;
  return ss.str();
}

void throwIfIndexIsOutOfRange(vector<string> const &args,size_t &index,string const&argumentName){
  if (index < args.size()) return;
  stringstream ss;
  ss << "Argument error:" << endl;
  ss << "expected " << contextBegin
     << " after context argument: " << argumentName;
  ss << " not end of arguments";
  throw MatchError(ss.str());
}

void throwIfArgumentIsNotContextBegin(vector<string> const &args,size_t &index,string const&argumentName){
  if (args.at(index) == contextBegin) return;
  stringstream ss;
  ss << "Argument error:" << endl;
  ss << "expected " << contextBegin
     << " after context argument: " << argumentName;
  ss << " not: " << args.at(index);
  throw MatchError(ss.str());
}

void throwIfArgumentsUbruptlyEnds(vector<string>const&args,size_t&index,string const&argumentName){
  if (index < args.size()) return;
  stringstream ss;
  ss << "Argument error:" << endl;
  ss << "expected " << contextEnd
     << " at the end of context: " << argumentName;
  ss << " not end of arguments";
  throw MatchError(ss.str());
}

Format::MatchStatus ContextFormat::match(vector<string> const &args,
                                         size_t &index) const {
  if (index >= args.size()) return MATCH_FAILURE;
  if (args.at(index) != argumentName) return MATCH_FAILURE;
  ++index;
  throwIfIndexIsOutOfRange(args,index,argumentName);
  throwIfArgumentIsNotContextBegin(args,index,argumentName);
  ++index;
  auto unusedFormats = getUnusedFormats();
  while (index < args.size() &&
         args.at(index) != contextEnd) {
    checkAndMatchOneUnusedFormat(unusedFormats,args,index);
  }
  throwIfArgumentsUbruptlyEnds(args,index,argumentName);
  ++index;
  return MATCH_SUCCESS;
}
