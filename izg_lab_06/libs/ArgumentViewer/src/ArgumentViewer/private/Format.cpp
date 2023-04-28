#include <ArgumentViewer/private/CommonFunctions.h>
#include <ArgumentViewer/private/Format.h>
#include <ArgumentViewer/private/Globals.h>

void Format::writeComment(stringstream &ss) const
{
  if (comment != "") ss << commentSeparator << comment;
}

void writeSpaces(stringstream &ss, size_t nofSpaces)
{
  for (size_t i = 0; i < nofSpaces; ++i) ss << " ";
}

void writeAlignedString(stringstream &ss, string const &txt, size_t size)
{
  ss << alignString(txt, size);
}

void writeIndentation(stringstream &ss, size_t indentation)
{
  writeSpaces(ss, indentation);
}

