#include <ArgumentViewer/Exception.h>
#include <ArgumentViewer/private/Globals.h>
#include <ArgumentViewer/private/LineSplitter.h>
#include <ArgumentViewer/private/StringVectorFormat.h>
#include <algorithm>

using namespace argumentViewer::ex;

StringVectorFormat::StringVectorFormat(string const &        argument,
                                       vector<string> const &defs,
                                       string const &        com)
    : ValueFormat(argument, com), defaults(defs) {}

string StringVectorFormat::getDefaults() const {
  LineSplitter splitter;
  splitter.addString(contextBegin);
  bool first = true;
  for (auto const &x : defaults) {
    if (first)
      first = false;
    else
      splitter.addString(" ");
    splitter.addString(x);
  }
  splitter.addString(contextEnd);
  return splitter.get();
}

size_t StringVectorFormat::getDefaultsLength() const {
  auto   defaults      = getDefaults();
  size_t maxLineLength = 0;
  size_t lineStart     = 0;
  size_t lineEnd;
  while ((lineEnd = defaults.find("\n", lineStart)) != string::npos) {
    maxLineLength = max(maxLineLength, lineEnd - lineStart + 1);
    lineStart     = lineEnd + 1;
  }
  maxLineLength = max(maxLineLength, defaults.length() - lineStart + 1);
  return maxLineLength;
}

string StringVectorFormat::getType() const { return "string*"; }

void throwIfArgumentsDoesNotContainContextBegin(vector<string> const &args,size_t &index,string const&argumentName){
  if (args.at(index) == contextBegin) return;
  stringstream ss;
  ss << "Argument error:" << endl;
  ss << "expected " << contextBegin << " after argument: " << argumentName;
  ss << " not argument: " << args.at(index);
  throw MatchError(ss.str());
}

void throwIfArgumentsUbrupltlyEnds(vector<string> const &args,size_t &index,string const&argumentName){
  if (index < args.size()) return;
  stringstream ss;
  ss << "Argument error:" << endl;
  ss << "expected " << contextEnd
     << " at the end of context argument: " << argumentName;
  ss << " not end of arguments";
  throw MatchError(ss.str());
}

Format::MatchStatus StringVectorFormat::match(vector<string> const &args,
                                              size_t &index) const {
  if (index >= args.size()) return MATCH_FAILURE;
  if (args.at(index) != argumentName) return MATCH_FAILURE;
  ++index;
  throwIfArgumentsDoesNotContainContextBegin(args,index,argumentName);
  ++index;
  while (index < args.size() && args.at(index) != contextEnd) ++index;
  throwIfArgumentsUbrupltlyEnds(args,index,argumentName);
  ++index;
  return MATCH_SUCCESS;
}
