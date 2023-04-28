#pragma once

#include <ArgumentViewer/private/ArgumentListFormat.h>
#include <ArgumentViewer/private/Globals.h>

class ContextFormat : public ArgumentListFormat {
 public:
  string argumentName;
  ContextFormat(string const &argument, string const &com);

  virtual string toStr(size_t indent,
                            size_t = 0,
                            size_t = 0,
                            size_t = 0) const override;
  virtual MatchStatus match(vector<string> const &args,
                            size_t &index) const override;
};
