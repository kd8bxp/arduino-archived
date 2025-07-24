// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Version
#define VERSION "1.3.1"
#define AUTHOR  "F4HWN"
#define NAME    "HAL9000"

#define RANDOM 1  // Set to 1 for random view
#define CORE   1
#define CORE2  2
#define CORES3 3
#define ATOMS3 4

// Others define
#define HAL9000_LOGO "/HAL9000.jpg"
#define HAL9000_EYE  "/HAL9000-eye.jpg"
#define HAL9000_TMP  "/tmp.mjpg"

#define MJPEG_BUFFER_SIZE 128 * 128  // Memory for a single JPEG frame
#define DEST_FS_USES_LITTLEFS

// Dependencies
#include <Preferences.h>
#include <LittleFS.h>
#include <ESP32-targz.h>
#include <M5Unified.h>
#include "MjpegClass.h"
#include "JpegFunc.h"

// Preferences
Preferences preferences;

// Variables
static MjpegClass mjpegClass;

fs::File root;
fs::File mjpegFile;

#define NUMBER_OF_FILENAME 128
#define MAX_FILENAME_SIZE  32

char videoFilename[NUMBER_OF_FILENAME][MAX_FILENAME_SIZE];

boolean load = false;

int8_t indice         = 0;
uint8_t limit         = 0;
uint8_t videoCurrent  = 0;
uint8_t videoLast     = 0;
//uint8_t brightness    = BRIGHTNESS;
uint8_t brightnessOld = 0;
uint8_t showEye       = 10;
