// java/runtime_texts.hpp -- the verbatim texts runtime.cpp assembles (data; see runtime_texts.cpp).
#pragma once

#include <string_view>

namespace cb::java::runtime::verbatim {

extern const std::string_view kGl11Header;
extern const std::string_view kBufferUtilsHeader;
extern const std::string_view kSimpleDateFormatHeader;
extern const std::string_view kRuntimeHeader;
extern const std::string_view kRandomAccessFileHeader;
extern const std::string_view kStackHeader;
extern const std::string_view kEntryHeader;
extern const std::string_view kInputStreamHeader;
extern const std::string_view kFileInputStreamHeader;
extern const std::string_view kColorPrefix;
extern const std::string_view kColorSuffix;
extern const std::string_view kGLContextHeader;
extern const std::string_view kClassHeader;
extern const std::string_view kInflaterHeader;
extern const std::string_view kSoftReferenceHeader;
extern const std::string_view kInetAddressHeader;
extern const std::string_view kHostIoCpp;

}  // namespace cb::java::runtime::verbatim
