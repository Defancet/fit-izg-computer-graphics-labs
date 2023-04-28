#pragma once

#include <ArgumentViewer/private/CommonFunctions.h>
#include <ArgumentViewer/private/LineSplitter.h>
#include <ArgumentViewer/private/ValueFormat.h>
#include <TxtUtils/TxtUtils.h>

template <typename TYPE>
class VectorFormat : public ValueFormat {
 public:
  vector<TYPE> defaults;
  VectorFormat(string const &      argument,
               vector<TYPE> const &defs,
               string const &      com);
  virtual string      getDefaults() const override;
  virtual size_t      getDefaultsLength() const override;
  virtual string      getType() const override;
  virtual MatchStatus match(vector<string> const &args,
                            size_t &              index) const override;

 private:
  void writeDefaultsToSplitter(LineSplitter &splitter) const;
};

template <typename TYPE>
VectorFormat<TYPE>::VectorFormat(string const &      argument,
                                 vector<TYPE> const &defs,
                                 string const &      com)
    : ValueFormat(argument, com), defaults(defs)
{
}

template <typename TYPE>
void VectorFormat<TYPE>::writeDefaultsToSplitter(LineSplitter &splitter) const
{
  bool first = true;
  for (auto const &x : defaults) {
    if (first)
      first = false;
    else
      splitter.addString(" ");
    splitter.addString(txtUtils::valueToString(x));
  }
}

template <typename TYPE>
string VectorFormat<TYPE>::getDefaults() const
{
  LineSplitter splitter;
  writeDefaultsToSplitter(splitter);
  return splitter.get();
}

template <typename TYPE>
size_t VectorFormat<TYPE>::getDefaultsLength() const
{
  auto const text      = getDefaults();
  auto const lines     = splitString(text, "\n");
  size_t     maxLength = 0;
  for (auto const &line : lines) maxLength = max(maxLength, line.length());
  return maxLength;
}

template <typename TYPE>
string VectorFormat<TYPE>::getType() const
{
  return typeName<TYPE>() + "*";
}

template <typename TYPE>
void moveIndexToTheAndOfArgumentsWithThisType(vector<string>const&args,size_t&index){
  while (index < args.size() && isValueConvertibleTo<TYPE>(args.at(index)))
    ++index;
}

template <typename TYPE>
Format::MatchStatus VectorFormat<TYPE>::match(vector<string> const &args,
                                              size_t &              index) const
{
  if (index >= args.size()) return MATCH_FAILURE;
  if (args.at(index) != argumentName) return MATCH_FAILURE;
  ++index;
  moveIndexToTheAndOfArgumentsWithThisType<TYPE>(args,index);
  return MATCH_SUCCESS;
}
