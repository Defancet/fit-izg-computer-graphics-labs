#pragma once

#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Format {
 public:
  string comment;
  Format(string const &com) : comment(com) {}
  enum MatchStatus {
    // match was successful
    MATCH_SUCCESS,
    // match was unsuccessful at the beginning
    MATCH_FAILURE,
  };
  virtual string      toStr(size_t indent      = 0,
                            size_t maxNameSize = 0,
                            size_t maxDefaultsSize = 0,
                            size_t maxTypeSize = 0) const = 0;
  virtual MatchStatus match(vector<string> const &args,
                            size_t &              index) const          = 0;
 protected:
  void writeComment(stringstream&ss)const;
};

void writeIndentation(stringstream&ss,size_t indentation);
void writeSpaces(stringstream&ss,size_t nofSpaces);
void writeAlignedString(stringstream &ss, string const &txt, size_t size);
