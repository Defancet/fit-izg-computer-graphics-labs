#include <ArgumentViewer/private/Globals.h>
#include <ArgumentViewer/private/LineSplitter.h>
#include <ArgumentViewer/private/CommonFunctions.h>

string LineSplitter::get() const { return ss.str(); }

void LineSplitter::addString(string const &text) {
  auto const lineLength = ss.str().length() - lineStart + 1;
  auto const wouldOverflow = lineLength + text.length() >= maxDefaultsLineLength;
  if (wouldOverflow) {
    writeLineEnd(ss);
    lineStart = ss.str().length();
    if (text == " ") return;
  }
  ss << text;
}
