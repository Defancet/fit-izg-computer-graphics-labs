#pragma once

#include <ArgumentViewer/Fwd.h>
#include <stdexcept>
#include <string>

class argumentViewer::ex::Exception : public std::runtime_error {
 public:
  Exception(std::string const& msg) : std::runtime_error(msg) {
    message = std::string("ArgumentViewer - ") + std::string(msg);
  }
  virtual ~Exception() throw() {}
  virtual char const* what() const throw() override {
    return message.c_str();
  }
  std::string message;
};

class argumentViewer::ex::MatchError : public Exception {
 public:
  MatchError(std::string const& msg) : Exception(msg) {
    message = std::string("ArgumentViewer::match - ") + std::string(msg);
  }
};
