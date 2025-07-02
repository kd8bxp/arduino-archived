// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include "ConstRamStringAdapter.hpp"
#include "RamStringAdapter.hpp"
#include "SizedRamStringAdapter.hpp"

#if ARDUINOJSON_ENABLE_STD_STRING
#include "StdStringAdapter.hpp"
#endif

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
#include "ArduinoStringAdapter.hpp"
#endif

#if ARDUINOJSON_ENABLE_PROGMEM
#include "FlashStringAdapter.hpp"
#include "SizedFlashStringAdapter.hpp"
#endif
