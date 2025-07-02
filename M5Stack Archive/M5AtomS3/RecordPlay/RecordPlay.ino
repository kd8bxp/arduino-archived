/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#include <Arduino.h>
#include "M5EchoBase.h"

#define RECORD_SIZE (1024 * 96)  // Define the size of the record buffer to 96KB.

M5EchoBase echobase(I2S_NUM_0);    // Create an instance of the M5EchoBase class.
static uint8_t *buffer = nullptr;  // Pointer to hold the audio buffer.

void setup()
{
    Serial.begin(115200);

    // Initialize the EchoBase with ATOMS3 pinmap.
    echobase.init(16000 /*Sample Rate*/, 38 /*I2C SDA*/, 39 /*I2C SCL*/, 7 /*I2S DIN*/, 6 /*I2S WS*/, 5 /*I2S DOUT*/,
                  8 /*I2S BCK*/, Wire);

    // Initialize the EchoBase with ATOM pinmap.
    // echobase.init(16000 /*Sample Rate*/, 25 /*I2C SDA*/, 21 /*I2C SCL*/, 23 /*I2S DIN*/, 19 /*I2S WS*/, 22 /*I2S
    // DOUT*/, 33 /*I2S BCK*/, Wire);

    echobase.setSpeakerVolume(80);             // Set speaker volume to 50%.
    echobase.setMicGain(ES8311_MIC_GAIN_6DB);  // Set microphone gain to 6dB.

    buffer = (uint8_t *)malloc(RECORD_SIZE);  // Allocate memory for the record buffer.
    // Check if memory allocation was successful.
    if (buffer == nullptr) {
        // If memory allocation fails, enter an infinite loop.
        while (true) {
            Serial.println("Failed to allocate memory :(");
            delay(1000);
        }
    }

    Serial.println("EchoBase ready, start recording and playing!");
}

void loop()
{
    // Recording
    echobase.setMute(true);
    delay(10);
    echobase.record(buffer, RECORD_SIZE);  // Record audio into buffer.
    delay(100);

    // Playing
    echobase.setMute(false);
    delay(10);
    echobase.play(buffer, RECORD_SIZE);  // Play audio from buffer.
    delay(100);
}
