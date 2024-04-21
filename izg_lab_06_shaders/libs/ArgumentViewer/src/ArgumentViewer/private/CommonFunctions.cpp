#include <ArgumentViewer/private/CommonFunctions.h>
#include <ArgumentViewer/private/Globals.h>

string parseEscapeSequence(string const &text) {
  if (text == "\\" + contextBegin) return contextBegin;
  if (text == "\\" + contextEnd) return contextEnd;
  if (text == "\\" + fileSymbol) return fileSymbol;
  return text;
}

string alignString(string const &text,
                   size_t        alignment,
                   char          filler,
                   bool          fillerLeft) {
  stringstream ss;
  if (fillerLeft)
    for (size_t i = text.length(); i < alignment; ++i) ss << filler;
  ss << text;
  if (!fillerLeft)
    for (size_t i = text.length(); i < alignment; ++i) ss << filler;
  return ss.str();
}

void writeLineEnd(stringstream &ss) { ss << endl; }

string chopQuotes(string const&x){
  return x.substr(1).substr(0,x.length()-2);
}

string extractSubstring(string const&str,size_t start,size_t end){
  return str.substr(start, end - start);
}

std::vector<string>splitString(string const&txt,string const&splitter){
  std::vector<string>strings;
  size_t       lineEnd     = 0;
  size_t       lineStart   = 0;
  auto const splitterSize = splitter.size();
  while ((lineEnd = txt.find(splitter, lineStart)) != string::npos) {
    auto const part = extractSubstring(txt,lineStart,lineEnd);
    strings.push_back(part);
    lineStart = lineEnd + splitterSize;
  }
  auto const lastPart = extractSubstring(txt,lineStart, txt.length());
  if(lastPart != "")strings.push_back(lastPart);
  return strings;
}


template <>
string typeName<float>() {
  return "f32";
}

template <>
string typeName<double>() {
  return "f64";
}

template <>
string typeName<int32_t>() {
  return "i32";
}

template <>
string typeName<int64_t>() {
  return "i64";
}

template <>
string typeName<uint32_t>() {
  return "u32";
}

template <>
string typeName<uint64_t>() {
  return "u64";
}

template <>
string typeName<string>() {
  return "string";
}

template <>
bool isValueConvertibleTo<float>(string const &text) {
  return txtUtils::isFloatingPoint(text) || txtUtils::isIntegral(text);
}

template <>
bool isValueConvertibleTo<double>(string const &text) {
  return txtUtils::isFloatingPoint(text) || txtUtils::isIntegral(text);
}

template <>
bool isValueConvertibleTo<int32_t>(string const &text) {
  return txtUtils::isIntegral(text);
}

template <>
bool isValueConvertibleTo<int64_t>(string const &text) {
  return txtUtils::isIntegral(text);
}

template <>
bool isValueConvertibleTo<uint32_t>(string const &text) {
  return txtUtils::isUint(text);
}

template <>
bool isValueConvertibleTo<uint64_t>(string const &text) {
  return txtUtils::isUint(text);
}

template <>
bool isValueConvertibleTo<string>(string const &text) {
  return txtUtils::isString(text);
}


