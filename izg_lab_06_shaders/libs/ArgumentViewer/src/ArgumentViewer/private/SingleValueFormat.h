#pragma once

#include <ArgumentViewer/Exception.h>
#include <ArgumentViewer/private/CommonFunctions.h>
#include <ArgumentViewer/private/ValueFormat.h>

using namespace argumentViewer;

template <typename TYPE>
class SingleValueFormat : public ValueFormat {
 public:
  SingleValueFormat(string const &argument, TYPE const &def, string const &com);
  virtual string      getDefaults() const override;
  virtual string      getType() const override;
  virtual MatchStatus match(vector<string> const &args,
                            size_t &              index) const override;
  TYPE                defaults;
};

template <typename TYPE>
SingleValueFormat<TYPE>::SingleValueFormat(string const &argument,
                                           TYPE const &  def,
                                           string const &com)
    : ValueFormat(argument, com), defaults(def)
{
}

template <typename TYPE>
string SingleValueFormat<TYPE>::getDefaults() const
{
  if (is_same<TYPE, string>::value) {
    auto x = txtUtils::valueToString(defaults);
    return chopQuotes(x);
  }
  return txtUtils::valueToString(defaults);
}

template <typename TYPE>
string SingleValueFormat<TYPE>::getType() const
{
  return typeName<TYPE>();
}

template <typename TYPE>
void throwIfArgumentIsNotFollowedByValueButArgumentsEnds(
    vector<string> const &args,
    size_t &              index,
    string const &        argumentName)
{
  if (index < args.size()) return;
  stringstream ss;
  ss << "Single value argument: " << argumentName
     << " should be followed by value of type: " << typeName<TYPE>()
     << " not by end of arguments";
  throw ex::MatchError(ss.str());
}

template <typename TYPE>
void throwIfArgumentIsNotFollowedByCompatibleValue(vector<string> const &args,
                                                  size_t &              index,
                                                  string const &argumentName)
{
  if (isValueConvertibleTo<TYPE>(args.at(index))) return;
  stringstream ss;
  ss << "Single value argument: " << argumentName
     << " should be followed by value of type: " << typeName<TYPE>()
     << " not by: " << args.at(index);
  throw ex::MatchError(ss.str());
}

template <typename TYPE>
Format::MatchStatus SingleValueFormat<TYPE>::match(vector<string> const &args,
                                                   size_t &index) const
{
  if (index >= args.size()) return MATCH_FAILURE;
  if (args.at(index) != argumentName) return MATCH_FAILURE;
  ++index;
  throwIfArgumentIsNotFollowedByValueButArgumentsEnds<TYPE>(args, index,
                                                            argumentName);
  throwIfArgumentIsNotFollowedByCompatibleValue<TYPE>(args, index, argumentName);
  ++index;
  return MATCH_SUCCESS;
}
