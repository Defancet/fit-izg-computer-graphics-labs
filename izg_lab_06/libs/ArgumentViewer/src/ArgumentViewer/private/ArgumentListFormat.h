#pragma once

#include <ArgumentViewer/private/ValueFormat.h>
#include <map>
#include <memory>
#include <set>

class ContextFormat;
class ArgumentListFormat : public Format {
 public:
  ArgumentListFormat(string const &com);
  virtual string                  toStr(size_t indent,
                                        size_t = 0,
                                        size_t = 0,
                                        size_t = 0) const override;
  virtual MatchStatus             match(vector<string> const &args,
                                        size_t &              index) const override;
  map<string, shared_ptr<Format>> formats;

 protected:
  void        getLargestLengths(size_t &nameLength,
                                size_t &defaultsLength,
                                size_t &typeLength) const;
  void        writeIndentedNonContextFormats(stringstream &ss,
                                             size_t        nameLength,
                                             size_t        defaultsLength,
                                             size_t        typeLength,
                                             size_t        indent) const;
  void        writeContextFormats(stringstream &ss, size_t indent) const;
  string      matchOneUnusedFormat(set<string> const &   unusedFormats,
                                   vector<string> const &args,
                                   size_t &              index) const;
  void        checkAndMatchOneUnusedFormat(set<string> &         unusedFormats,
                                           vector<string> const &args,
                                           size_t &              index) const;
  void        matchUnusedFormats(set<string> &         unusedFormats,
                                 vector<string> const &args,
                                 size_t &              index) const;
  set<string> getUnusedFormats() const;
};
