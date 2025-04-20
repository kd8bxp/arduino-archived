// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include "../Configuration.hpp"
#include "../Namespace.hpp"

namespace ARDUINOJSON_NAMESPACE {

#if ARDUINOJSON_USE_DOUBLE
typedef double Float;
#else
typedef float Float;
#endif
}  // namespace ARDUINOJSON_NAMESPACE
