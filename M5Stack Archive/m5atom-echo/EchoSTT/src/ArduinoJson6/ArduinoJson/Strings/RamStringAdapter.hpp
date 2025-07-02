// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include "ConstRamStringAdapter.hpp"
#include "IsString.hpp"
#include "StoragePolicy.hpp"

namespace ARDUINOJSON_NAMESPACE {

class RamStringAdapter : public ConstRamStringAdapter {
 public:
  RamStringAdapter(const char* str) : ConstRamStringAdapter(str) {}

  void copyTo(char* p, size_t n) const {
    memcpy(p, _str, n);
  }

  typedef ARDUINOJSON_NAMESPACE::storage_policies::store_by_copy storage_policy;
};

template <typename TChar>
inline RamStringAdapter adaptString(const TChar* str) {
  return RamStringAdapter(reinterpret_cast<const char*>(str));
}

inline RamStringAdapter adaptString(char* str) {
  return RamStringAdapter(str);
}

template <typename TChar>
struct IsString<TChar*> {
  static const bool value = sizeof(TChar) == 1;
};

template <>
struct IsString<void*> {
  static const bool value = false;
};

}  // namespace ARDUINOJSON_NAMESPACE
