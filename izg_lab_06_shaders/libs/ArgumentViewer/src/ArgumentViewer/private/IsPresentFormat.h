#pragma once

#include <ArgumentViewer/private/Format.h>

class IsPresentFormat : public Format {
 public:
  IsPresentFormat(string const &name, string const &com);
  virtual string      toStr(size_t indent,
                            size_t maxNameSize,
                            size_t maxDefaultsSize,
                            size_t maxTypeSize) const override final;
  virtual MatchStatus match(vector<string> const &args,
                            size_t &              index) const override;
  string              argumentName;
};
