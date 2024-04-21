#pragma once

#include <TxtUtils/TxtUtils.h>
#include <string>

using namespace std;

string parseEscapeSequence(string const &text);

string alignString(string const &text,
                   size_t        alignment  = 0,
                   char          filler     = ' ',
                   bool          fillerLeft = false);

void writeLineEnd(stringstream&ss);

string chopQuotes(string const&x);

string extractSubstring(string const&str,size_t start,size_t end);

std::vector<string>splitString(string const&txt,string const&splitter);

template <typename TO, typename FROM>
bool isTypeOf(shared_ptr<FROM> const &value)
{
  return dynamic_pointer_cast<TO>(value) != nullptr;
}

template <typename TYPE>
string typeName();

template <typename TYPE>
bool isValueConvertibleTo(string const &text);

template <>
string typeName<float>();
template <>
string typeName<double>();
template <>
string typeName<int32_t>();
template <>
string typeName<int64_t>();

template <>
string typeName<uint32_t>();

template <>
string typeName<uint64_t>();

template <>
string typeName<string>();

template <>
bool isValueConvertibleTo<float>(string const &text);

template <>
bool isValueConvertibleTo<double>(string const &text);

template <>
bool isValueConvertibleTo<int32_t>(string const &text);

template <>
bool isValueConvertibleTo<int64_t>(string const &text);

template <>
bool isValueConvertibleTo<uint32_t>(string const &text);
template <>
bool isValueConvertibleTo<uint64_t>(string const &text);
template <>
bool isValueConvertibleTo<string>(string const &text);

template <typename TYPE,
          typename std::enable_if<std::is_same<TYPE, int32_t>::value ||
                                      std::is_same<TYPE, int64_t>::value ||
                                      std::is_same<TYPE, uint32_t>::value ||
                                      std::is_same<TYPE, uint64_t>::value ||
                                      std::is_same<TYPE, float>::value ||
                                      std::is_same<TYPE, double>::value,
                                  unsigned>::type = 0>
TYPE str2val(std::string const &value)
{
  return txtUtils::str2Value<TYPE>(value);
}
template <typename TYPE,
          typename std::enable_if<std::is_same<TYPE, std::string>::value,
                                  unsigned>::type = 0>
TYPE str2val(std::string const &value)
{
  return value;
}
