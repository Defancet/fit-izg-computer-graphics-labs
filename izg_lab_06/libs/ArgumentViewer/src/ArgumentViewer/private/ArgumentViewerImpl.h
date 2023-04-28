#pragma once

#include <ArgumentViewer/Exception.h>
#include <ArgumentViewer/Fwd.h>
#include <ArgumentViewer/private/ArgumentListFormat.h>
#include <ArgumentViewer/private/CommonFunctions.h>
#include <ArgumentViewer/private/Format.h>
#include <ArgumentViewer/private/SingleValueFormat.h>
#include <ArgumentViewer/private/VectorFormat.h>
#include <TxtUtils/TxtUtils.h>
#include <cassert>
#include <memory>
#include <set>
#include <sstream>

using namespace argumentViewer;

class argumentViewer::ArgumentViewerImpl {
 public:
  string                applicationName = "";
  vector<string>        arguments;
  ArgumentViewer const *parent = nullptr;
  shared_ptr<Format>    format = nullptr;
  bool validate()const;
  string toStr()const;
  string getApplicationName()const;
  size_t getNofArguments()const;
  bool isPresent(string const&argument,string const&com)const;
  shared_ptr<ArgumentViewer>getContext(string const&name,string const&com,ArgumentViewer const*_this)const;
  size_t                getArgumentPosition(string const &argument) const;
  bool                  getContext(vector<string> &contextArguments,
                                   string const &  argument) const;
  vector<string>        getsv(string const&argument,vector<string>const&def,string const&com)const;
  bool                  isInRange(size_t index) const;
  string                getArgument(size_t index) const;
  void loadArgumentFiles(vector<string> &args, set<string> &alreadyLoaded);
  void splitFileToArguments(vector<string> &args, string const &fileContent);

  template <typename TYPE>
  TYPE getArgument(string const &argument, TYPE const &def) const;
  template <typename TYPE>
  vector<TYPE> getArguments(string const &      argument,
                            vector<TYPE> const &def) const;
  template <typename TYPE>
  TYPE getArgumentWithFormat(string const &argument,
                             TYPE const &  def,
                             string const &com) const;
  template <typename TYPE>
  vector<TYPE> getArgumentsWithFormat(string const &      argument,
                                      vector<TYPE> const &def,
                                      string const &      com) const;
};

template <typename TYPE>
TYPE ArgumentViewerImpl::getArgument(string const &argument,
                                     TYPE const &  def) const
{
  size_t i = getArgumentPosition(argument);
  if (!isInRange(i++)) return def;
  if (!isInRange(i)) return def;
  auto value = getArgument(i);
  if (!isValueConvertibleTo<TYPE>(value)) return def;
  return str2val<TYPE>(value);
}

template <typename TYPE>
vector<TYPE> ArgumentViewerImpl::getArguments(string const &      argument,
                                              vector<TYPE> const &def) const
{
  size_t argumentIndex = getArgumentPosition(argument);
  if (!isInRange(argumentIndex++)) return def;
  if (!isInRange(argumentIndex)) return def;
  vector<TYPE> result;
  while (isInRange(argumentIndex) &&
         isValueConvertibleTo<TYPE>(getArgument(argumentIndex)))
    result.push_back(txtUtils::str2Value<TYPE>(getArgument(argumentIndex++)));
  while (result.size() < def.size()) result.push_back(def.at(result.size()));
  return result;
}

template <typename TYPE>
TYPE ArgumentViewerImpl::getArgumentWithFormat(string const &argument,
                                               TYPE const &  def,
                                               string const &com) const
{
  auto alf = dynamic_pointer_cast<ArgumentListFormat>(format);
  assert(alf != nullptr);

  auto subFormatIt = alf->formats.find(argument);
  if (subFormatIt != alf->formats.end()) {
    auto subFormat = subFormatIt->second;
    auto singleValueFormat =
        dynamic_pointer_cast<SingleValueFormat<TYPE>>(subFormat);
    if (!singleValueFormat)
      throw ex::Exception(string("argument: ") + argument +
                          " is already defined as something else than single " +
                          typeName<TYPE>() + " value");
    if (singleValueFormat->defaults != def)
      throw ex::Exception(
          string("argument: ") + argument +
          " has already been defined with different default value: " +
          txtUtils::valueToString(singleValueFormat->defaults));
  }
  else
    alf->formats[argument] =
        make_shared<SingleValueFormat<TYPE>>(argument, def, com);
  if (alf->formats[argument]->comment == "")
    alf->formats[argument]->comment = com;

  return getArgument<TYPE>(argument, def);
}

template <typename TYPE>
vector<TYPE> ArgumentViewerImpl::getArgumentsWithFormat(string const &argument,
                                                        vector<TYPE> const &def,
                                                        string const &com) const
{
  auto alf = dynamic_pointer_cast<ArgumentListFormat>(format);
  assert(alf != nullptr);

  auto subFormatIt = alf->formats.find(argument);
  if (subFormatIt != alf->formats.end()) {
    auto subFormat    = subFormatIt->second;
    auto vectorFormat = dynamic_pointer_cast<VectorFormat<TYPE>>(subFormat);
    if (!vectorFormat)
      throw ex::Exception(
          string("argument: ") + argument +
          " is already defined as something else than vector of " +
          typeName<TYPE>() + " values");
    if (vectorFormat->defaults != def)
      throw ex::Exception(
          string("argument: ") + argument +
          " has already been defined with different default values: " +
          txtUtils::valueToString(vectorFormat->defaults));
  }
  else
    alf->formats[argument] =
        make_shared<VectorFormat<TYPE>>(argument, def, com);
  if (alf->formats[argument]->comment == "")
    alf->formats[argument]->comment = com;

  return getArguments<TYPE>(argument, def);
}
