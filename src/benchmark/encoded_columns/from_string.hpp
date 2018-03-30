#pragma once

#include <string>
#include <map>

#include "storage/column_encoding_utils.hpp"

namespace opossum {

template <typename T>
inline T from_string(const std::string& str);

template <>
inline EncodingType from_string<EncodingType>(const std::string& str) {
  static const auto type_for_string = std::map<std::string, EncodingType>{
    { "Unencoded", EncodingType::Unencoded },
    { "Dictionary", EncodingType::Dictionary },
    { "Run Length", EncodingType::RunLength },
    { "FOR", EncodingType::FrameOfReference }};

  return type_for_string.at(str);
}

template <>
inline VectorCompressionType from_string<VectorCompressionType>(const std::string& str) {
  static const auto type_for_string = std::map<std::string, VectorCompressionType>{
    { "FSBA", VectorCompressionType::FixedSizeByteAligned },
    { "SIMD-BP128", VectorCompressionType::SimdBp128 }};

  return type_for_string.at(str);
}

}  // namespace opossum