// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include "../Namespace.hpp"

#include <stdlib.h>  // for size_t

namespace ARDUINOJSON_NAMESPACE {

// The default reader is a simple wrapper for Readers that are not copiable
template <typename TSource, typename Enable = void>
struct Reader {
 public:
  Reader(TSource& source) : _source(&source) {}

  int read() {
    return _source->read();
  }

  size_t readBytes(char* buffer, size_t length) {
    return _source->readBytes(buffer, length);
  }

 private:
  TSource* _source;
};

template <typename TSource, typename Enable = void>
struct BoundedReader {
  // no default implementation because we need to pass the size to the
  // constructor
};
}  // namespace ARDUINOJSON_NAMESPACE

#include "Readers/IteratorReader.hpp"
#include "Readers/RamReader.hpp"
#include "Readers/VariantReader.hpp"

#if ARDUINOJSON_ENABLE_ARDUINO_STREAM
#include "Readers/ArduinoStreamReader.hpp"
#endif

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
#include "Readers/ArduinoStringReader.hpp"
#endif

#if ARDUINOJSON_ENABLE_PROGMEM
#include "Readers/FlashReader.hpp"
#endif

#if ARDUINOJSON_ENABLE_STD_STREAM
#include "Readers/StdStreamReader.hpp"
#endif
