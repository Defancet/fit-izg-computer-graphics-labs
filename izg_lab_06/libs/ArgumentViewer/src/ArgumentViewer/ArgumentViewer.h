#pragma once

#include <ArgumentViewer/Fwd.h>
#include <ArgumentViewer/argumentviewer_export.h>
#include <iostream>
#include <memory>
#include <vector>

class argumentViewer::ArgumentViewer {
 public:
  ARGUMENTVIEWER_EXPORT ArgumentViewer(int argc, char* argv[]);
  ARGUMENTVIEWER_EXPORT ~ArgumentViewer();
  ARGUMENTVIEWER_EXPORT std::string                     getApplicationName() const;
  ARGUMENTVIEWER_EXPORT size_t                          getNofArguments() const;
  ARGUMENTVIEWER_EXPORT std::string                     getArgument(size_t const& index) const;
  ARGUMENTVIEWER_EXPORT bool                            isPresent(std::string const& argument,
                                                                  std::string const& comment = "") const;
  ARGUMENTVIEWER_EXPORT float                           getf32(std::string const& argument,
                                                               float const&       def     = 0.f,
                                                               std::string const& comment = "") const;
  ARGUMENTVIEWER_EXPORT double                          getf64(std::string const& argument,
                                                               double const&      def     = 0.,
                                                               std::string const& comment = "") const;
  ARGUMENTVIEWER_EXPORT int32_t                         geti32(std::string const& argument,
                                                               int32_t const&     def     = 0,
                                                               std::string const& comment = "") const;
  ARGUMENTVIEWER_EXPORT int64_t                         geti64(std::string const& argument,
                                                               int64_t const&     def     = 0,
                                                               std::string const& comment = "") const;
  ARGUMENTVIEWER_EXPORT uint32_t                        getu32(std::string const& argument,
                                                               uint32_t const&    def     = 0,
                                                               std::string const& comment = "") const;
  ARGUMENTVIEWER_EXPORT uint64_t                        getu64(std::string const& argument,
                                                               uint64_t const&    def     = 0,
                                                               std::string const& comment = "") const;
  ARGUMENTVIEWER_EXPORT std::string                     gets(std::string const& argument,
                                                             std::string const& def     = "",
                                                             std::string const& comment = "") const;
  ARGUMENTVIEWER_EXPORT std::vector<float>              getf32v(std::string const&        argument,
                                                                std::vector<float> const& def = {},
                                                                std::string const&        comment = "") const;
  ARGUMENTVIEWER_EXPORT std::vector<double>             getf64v(std::string const&         argument,
                                                                std::vector<double> const& def = {},
                                                                std::string const&         comment = "") const;
  ARGUMENTVIEWER_EXPORT std::vector<int32_t>            geti32v(std::string const&          argument,
                                                                std::vector<int32_t> const& def = {},
                                                                std::string const&          comment = "") const;
  ARGUMENTVIEWER_EXPORT std::vector<int64_t>            geti64v(std::string const&          argument,
                                                                std::vector<int64_t> const& def = {},
                                                                std::string const&          comment = "") const;
  ARGUMENTVIEWER_EXPORT std::vector<uint32_t>           getu32v(std::string const&           argument,
                                                                std::vector<uint32_t> const& def = {},
                                                                std::string const& comment = "") const;
  ARGUMENTVIEWER_EXPORT std::vector<uint64_t>           getu64v(std::string const&           argument,
                                                                std::vector<uint64_t> const& def = {},
                                                                std::string const& comment = "") const;
  ARGUMENTVIEWER_EXPORT std::vector<std::string>        getsv(std::string const&              contextName,
                                                              std::vector<std::string> const& def = {},
                                                              std::string const& comment = "") const;
  ARGUMENTVIEWER_EXPORT std::shared_ptr<ArgumentViewer> getContext(
      std::string const& name,
      std::string const& comment = "") const;
  ARGUMENTVIEWER_EXPORT bool        validate() const;
  ARGUMENTVIEWER_EXPORT std::string toStr() const;

 protected:
  std::unique_ptr<ArgumentViewerImpl> impl;
  friend class ArgumentViewerImpl;
};
