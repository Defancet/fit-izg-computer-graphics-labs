#include <ArgumentViewer/ArgumentViewer.h>
#include <ArgumentViewer/Exception.h>
#include <ArgumentViewer/private/ArgumentViewerImpl.h>
#include <ArgumentViewer/private/ArgumentListFormat.h>

#include <MealyMachine/MealyMachine.h>
#include <TxtUtils/TxtUtils.h>
#include <algorithm>
#include <cassert>
#include <map>
#include <set>
#include <sstream>

using namespace argumentViewer;

/**
 * @brief Contructor of ArgumentViewer
 * This class can be used for simple argument manipulations
 * int main(int argc,char*argv[]){
 *   auto args = std::make_shared<ArgumentViewer>(argc,argv);
 * }
 *
 * @param argc number of arguments, it should always be one or greater
 * (application name is always first argument)
 * @param argv list of arguments, it should always contain application name as
 * the first argument
 */
ArgumentViewer::ArgumentViewer(int argc, char *argv[]) {
  if (argc <= 0)
    throw ex::Exception("number of arguments has to be greater than 0");
  impl = std::unique_ptr<ArgumentViewerImpl>(new ArgumentViewerImpl);
  assert(impl != nullptr);
  impl->applicationName = std::string(argv[0]);
  std::vector<std::string> args;
  for (int i = 1; i < argc; ++i) args.push_back(std::string(argv[i]));
  std::set<std::string> alreadyLoaded;
  impl->loadArgumentFiles(args, alreadyLoaded);
  impl->arguments = args;
  impl->format    = std::make_shared<ArgumentListFormat>("");
}

/**
 * @brief Destructor of ArgumentViewer instance
 */
ArgumentViewer::~ArgumentViewer() {}

/**
 * @brief Returns application name (zeroes argument)
 *
 * @return application name
 */
std::string ArgumentViewer::getApplicationName() const {
  return impl->getApplicationName();
}

/**
 * @brief Returns number of arguments without application name - it can be zero
 *
 * @return number of arguments without inclusion of application name
 */
size_t ArgumentViewer::getNofArguments() const {
  return impl->getNofArguments();
}

/**
 * @brief Gets argument on certain index
 *
 * @param index index of argument, should always be lesser than
 * getNofArguments()
 *
 * @return argument
 */
std::string ArgumentViewer::getArgument(size_t const &index) const {
  assert(impl != nullptr);
  assert(index < impl->arguments.size());
  return impl->arguments.at(index);
}

/**
 * @brief If argument is present, it returns true
 *
 * @param argument name of argument
 *
 * @return true if argument is present in arguments
 */
bool ArgumentViewer::isPresent(std::string const &argument,
                               std::string const &com) const {
  return impl->isPresent(argument,com);
}

/**
 * @brief gets float value after argument
 *
 * @param argument argument name that has to be followed by float
 * @param def default value
 * @param comment comment
 *
 * @return next value after argument. if argument is not found, it returns def
 */
float ArgumentViewer::getf32(std::string const &argument,
                             float const &      def,
                             std::string const &com) const {
  assert(impl != nullptr);
  return impl->getArgumentWithFormat<float>(argument, def, com);
}

/**
 * @brief gets double value after argument
 *
 * @param argument argument name that has to be followed by double
 * @param def default value
 * @param comment comment
 *
 * @return next value after argument. if argument is not found, it returns def
 */
double ArgumentViewer::getf64(std::string const &argument,
                              double const &     def,
                              std::string const &com) const {
  assert(impl != nullptr);
  return impl->getArgumentWithFormat<double>(argument, def, com);
}

/**
 * @brief gets int32_t value after argument
 *
 * @param argument argument name that has to be followed by int32_t
 * @param def default value
 * @param comment comment
 *
 * @return newxt value after argument. if argument is not found, it returns def
 */
int32_t ArgumentViewer::geti32(std::string const &argument,
                               int32_t const &    def,
                               std::string const &com) const {
  assert(impl != nullptr);
  return impl->getArgumentWithFormat<int32_t>(argument, def, com);
}

/**
 * @brief gets int64_t value after argument
 *
 * @param argument argument name that has to be followed by int64_t
 * @param def default value
 * @param comment comment
 *
 * @return next value after argument. if argument is not found, it returns def
 */
int64_t ArgumentViewer::geti64(std::string const &argument,
                               int64_t const &    def,
                               std::string const &com) const {
  assert(impl != nullptr);
  return impl->getArgumentWithFormat<int64_t>(argument, def, com);
}

/**
 * @brief gets uint32_t value after argument
 *
 * @param argument argument name that has to be followed by uint32_t
 * @param def default value
 * @param comment comment
 *
 * @return next value after argument. if argument is not found, it returns def
 */
uint32_t ArgumentViewer::getu32(std::string const &argument,
                                uint32_t const &   def,
                                std::string const &com) const {
  assert(impl != nullptr);
  return impl->getArgumentWithFormat<uint32_t>(argument, def, com);
}

/**
 * @brief gets uint64_t value after argument
 *
 * @param argument argument name that has to be followed by uint64_t
 * @param def default value
 * @param comment comment
 *
 * @return next value after argument. if argument is not found, it returns def
 */
uint64_t ArgumentViewer::getu64(std::string const &argument,
                                uint64_t const &   def,
                                std::string const &com) const {
  assert(impl != nullptr);
  return impl->getArgumentWithFormat<uint64_t>(argument, def, com);
}

/**
 * @brief gets string value after argument
 *
 * @param argument argument name that has to be followed by string
 * @param def default value
 * @param comment comment
 *
 * @return next value after argument. if argument is not found, it returns def
 */
std::string ArgumentViewer::gets(std::string const &argument,
                                 std::string const &def,
                                 std::string const &com) const {
  assert(impl != nullptr);
  return parseEscapeSequence(
      impl->getArgumentWithFormat<std::string>(argument, def, com));
}

/**
 * @brief gets vector of float values after argument
 *
 * @param argument argument name that has to be followed by vector of floats
 * @param def default values
 * @param comment comment
 *
 * @return next values after argument. if number of next values is less that
 * number of def values, returned values are extended by def values
 */
std::vector<float> ArgumentViewer::getf32v(std::string const &       argument,
                                           std::vector<float> const &def,
                                           std::string const &com) const {
  assert(impl != nullptr);
  return impl->getArgumentsWithFormat<float>(argument, def, com);
}

/**
 * @brief gets vector of double values after argument
 *
 * @param argument argument name that has to be followed by vector of doubles
 * @param def default values
 * @param comment comment
 *
 * @return next values after argument. if number of next values is less that
 * number of def values, returned values are extended by def values
 */
std::vector<double> ArgumentViewer::getf64v(std::string const &        argument,
                                            std::vector<double> const &def,
                                            std::string const &com) const {
  assert(impl != nullptr);
  return impl->getArgumentsWithFormat<double>(argument, def, com);
}

/**
 * @brief gets vector of int32_t values after argument
 *
 * @param argument argument name that is followed by vector of int32_t values
 * @param def default values
 * @param comment comment
 *
 * @return next values after argument. if number of next values is less that
 * number of def values, returned values are extended by def values
 */
std::vector<int32_t> ArgumentViewer::geti32v(std::string const &argument,
                                             std::vector<int32_t> const &def,
                                             std::string const &com) const {
  assert(impl != nullptr);
  return impl->getArgumentsWithFormat<int32_t>(argument, def, com);
}

/**
 * @brief gets vector of int64_t values after argument
 *
 * @param argument argument name that is followed by vector of int64_t values
 * @param def default values
 * @param comment comment
 *
 * @return next values after argument. if number of next values is less that
 * number of def values, returned values are extended by def values
 */
std::vector<int64_t> ArgumentViewer::geti64v(std::string const &argument,
                                             std::vector<int64_t> const &def,
                                             std::string const &com) const {
  assert(impl != nullptr);
  return impl->getArgumentsWithFormat<int64_t>(argument, def, com);
}

/**
 * @brief gets vector of uint32_t values after argument
 *
 * @param argument argument name that is followed by vector of uint32_t values
 * @param def default values
 * @param comment comment
 *
 * @return next values after argument. if number of next values is less that
 * number of def values, returned values are extended by def values
 */
std::vector<uint32_t> ArgumentViewer::getu32v(std::string const &argument,
                                              std::vector<uint32_t> const &def,
                                              std::string const &com) const {
  assert(impl != nullptr);
  return impl->getArgumentsWithFormat<uint32_t>(argument, def, com);
}

/**
 * @brief gets vector of uint64_t values after argument
 *
 * @param argument argument name that is followed by vector of uint64_t values
 * @param def default values
 * @param comment comment
 *
 * @return next values after argument. if number of next values is less that
 * number of def values, returned values are extended by def values
 */
std::vector<uint64_t> ArgumentViewer::getu64v(std::string const &argument,
                                              std::vector<uint64_t> const &def,
                                              std::string const &com) const {
  assert(impl != nullptr);
  return impl->getArgumentsWithFormat<uint64_t>(argument, def, com);
}

/**
 * @brief gets context of argument with format: argument { arg0, ... }
 *
 * @param name context name - argument that is followed by { arg0, ... }
 * @param comment comment
 *
 * @return ArgumentViewer that contains arg*
 */
std::shared_ptr<ArgumentViewer> ArgumentViewer::getContext(
    std::string const &name, std::string const &com) const {
  return impl->getContext(name,com,this);
}

/**
 * @brief gets vector of string values
 *
 * @param argument argument that is followed by { arg0, arg1, ... }
 * @param def default values
 * @param comment comment
 *
 * @return return arg*, if size of args is less that def, it is
 * extended by def
 */
std::vector<std::string> ArgumentViewer::getsv(
    std::string const &             argument,
    std::vector<std::string> const &def,
    std::string const &             com) const {
  return impl->getsv(argument,def,com);
}

std::string ArgumentViewer::toStr() const {
  return impl->toStr();
}

bool ArgumentViewer::validate() const {
  return impl->validate();
}
