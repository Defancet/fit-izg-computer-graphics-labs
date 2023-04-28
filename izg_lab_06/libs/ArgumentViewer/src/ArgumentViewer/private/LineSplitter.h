#pragma once

#include <sstream>
#include <string>

using namespace std;

class LineSplitter {
 public:
  string get() const;
  void   addString(string const &text);

 protected:
  size_t       lineStart = 0;
  stringstream ss;
};
