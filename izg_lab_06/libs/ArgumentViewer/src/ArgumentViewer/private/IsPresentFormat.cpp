#include <ArgumentViewer/private/CommonFunctions.h>
#include <ArgumentViewer/private/Globals.h>
#include <ArgumentViewer/private/IsPresentFormat.h>
#include <sstream>

IsPresentFormat::IsPresentFormat(string const &name, string const &com)
    : Format(com), argumentName(name)
{
}

void writeDefaultsSeparatorAsSpaces(stringstream &ss)
{
  writeSpaces(ss, defaultsSeparator.length());
}

void writeTypePreDecoratorAsSpaces(stringstream &ss)
{
  writeSpaces(ss, typePreDecorator.length());
}

void writeTypePostDecoratorAsSpaces(stringstream &ss)
{
  writeSpaces(ss, typePostDecorator.length());
}

string IsPresentFormat::toStr(size_t indent,
                              size_t maxNameSize,
                              size_t maxDefaultsSize,
                              size_t maxTypeSize) const
{
  stringstream ss;
  writeIndentation(ss, indent);
  writeAlignedString(ss, argumentName, maxNameSize);
  writeDefaultsSeparatorAsSpaces(ss);
  writeSpaces(ss, maxDefaultsSize);
  writeTypePreDecoratorAsSpaces(ss);
  writeSpaces(ss, maxTypeSize);
  writeTypePostDecoratorAsSpaces(ss);
  writeComment(ss);
  writeLineEnd(ss);
  return ss.str();
}

Format::MatchStatus IsPresentFormat::match(vector<string> const &args,
                                           size_t &              index) const
{
  if (index >= args.size()) return MATCH_FAILURE;
  if (args.at(index) != argumentName) return MATCH_FAILURE;
  index++;
  return MATCH_SUCCESS;
}
