#include <ArgumentViewer/private/CommonFunctions.h>
#include <ArgumentViewer/private/Globals.h>
#include <ArgumentViewer/private/ValueFormat.h>

ValueFormat::ValueFormat(string const &argument, string const &com)
    : Format(com), argumentName(argument)
{
}

string ValueFormat::getName() const { return argumentName; }

size_t ValueFormat::getDefaultsLength() const { return getDefaults().length(); }

void writeTypePreDecorator(stringstream &ss) { ss << typePreDecorator; }

void writeTypePostDecorator(stringstream &ss) { ss << typePostDecorator; }

void writeDefaultsSeparator(stringstream &ss) { ss << defaultsSeparator; }

void writeAlignedType(stringstream &ss, string const &type, size_t maxTypeSize)
{
  writeTypePreDecorator(ss);
  writeAlignedString(ss, type, maxTypeSize);
  writeTypePostDecorator(ss);
}

void ifNotFirstLineWriteIndentation(stringstream&ss,size_t i,size_t firstLine,size_t indentation){
  if(i == firstLine)return;
  writeIndentation(ss,indentation);
}
void ifNotLastLineWriteLineEnd(stringstream&ss,size_t i,size_t lastLine){
  if(i == lastLine )return;
  writeLineEnd(ss);
}

void ValueFormat::writeDefaults(stringstream&ss,size_t maxDefaultsSize)const{
  size_t const indentation = ss.str().length();
  auto const lines = splitString(getDefaults(),"\n");
  size_t const firstLine = 0;
  size_t const lastLine = lines.size()-1;
  for(size_t i=0;i<lines.size();++i){
    ifNotFirstLineWriteIndentation(ss,i,firstLine,indentation);
    writeAlignedString(ss,lines[i],maxDefaultsSize);
    ifNotLastLineWriteLineEnd(ss,i,lastLine);
  }
  if(!lines.empty())return;
  writeSpaces(ss,maxDefaultsSize);
}

string ValueFormat::toStr(size_t indent,
                          size_t maxNameSize,
                          size_t maxDefaultsSize,
                          size_t maxTypeSize) const
{
  stringstream ss;
  writeIndentation(ss, indent);
  writeAlignedString(ss, getName(), maxNameSize);
  writeDefaultsSeparator(ss);
  writeDefaults(ss,maxDefaultsSize);
  writeAlignedType(ss, getType(), maxTypeSize);
  writeComment(ss);
  writeLineEnd(ss);
  return ss.str();
}
