#pragma mark 

#include <M5Stack.h>
#include "FS.h"
#include "SPIFFS.h"
#include <WiFi.h>
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include <google-tts.h>
#include <HTTPClient.h>
#include <DNSServer.h>
#include <WiFiClientSecure.h>

AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

const char* ssid     = "ssid";
const char* password = "pass";

//WiFiClientSecure client;
const char* rootCACertificate = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIJTzCCCDegAwIBAgIQGoaLDa+bxzQIAAAAAD69lzANBgkqhkiG9w0BAQsFADBC\n" \
"MQswCQYDVQQGEwJVUzEeMBwGA1UEChMVR29vZ2xlIFRydXN0IFNlcnZpY2VzMRMw\n" \
"EQYDVQQDEwpHVFMgQ0EgMU8xMB4XDTIwMDUwNTA4MjIzNVoXDTIwMDcyODA4MjIz\n" \
"NVowZjELMAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcT\n" \
"DU1vdW50YWluIFZpZXcxEzARBgNVBAoTCkdvb2dsZSBMTEMxFTATBgNVBAMMDCou\n" \
"Z29vZ2xlLmNvbTBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABMExBCpkvy8VuVVJ\n" \
"gyrY19Q/NvVPzb+yNS371xwzVriYLDhyEyGXNf45D/qj7qIe9EqkpOylAUvF5+4x\n" \
"NYr5INujggbmMIIG4jAOBgNVHQ8BAf8EBAMCB4AwEwYDVR0lBAwwCgYIKwYBBQUH\n" \
"AwEwDAYDVR0TAQH/BAIwADAdBgNVHQ4EFgQUn9s6xwZqJNPn2S7GBvgewUShDNcw\n" \
"HwYDVR0jBBgwFoAUmNH4bhDrz5vsYJ8YkBug630J/SswZAYIKwYBBQUHAQEEWDBW\n" \
"MCcGCCsGAQUFBzABhhtodHRwOi8vb2NzcC5wa2kuZ29vZy9ndHMxbzEwKwYIKwYB\n" \
"BQUHMAKGH2h0dHA6Ly9wa2kuZ29vZy9nc3IyL0dUUzFPMS5jcnQwggSoBgNVHREE\n" \
"ggSfMIIEm4IMKi5nb29nbGUuY29tgg0qLmFuZHJvaWQuY29tghYqLmFwcGVuZ2lu\n" \
"ZS5nb29nbGUuY29tggkqLmJkbi5kZXaCEiouY2xvdWQuZ29vZ2xlLmNvbYIYKi5j\n" \
"cm93ZHNvdXJjZS5nb29nbGUuY29tggYqLmcuY2+CDiouZ2NwLmd2dDIuY29tghEq\n" \
"LmdjcGNkbi5ndnQxLmNvbYIKKi5nZ3BodC5jboIOKi5na2VjbmFwcHMuY26CFiou\n" \
"Z29vZ2xlLWFuYWx5dGljcy5jb22CCyouZ29vZ2xlLmNhggsqLmdvb2dsZS5jbIIO\n" \
"Ki5nb29nbGUuY28uaW6CDiouZ29vZ2xlLmNvLmpwgg4qLmdvb2dsZS5jby51a4IP\n" \
"Ki5nb29nbGUuY29tLmFygg8qLmdvb2dsZS5jb20uYXWCDyouZ29vZ2xlLmNvbS5i\n" \
"coIPKi5nb29nbGUuY29tLmNvgg8qLmdvb2dsZS5jb20ubXiCDyouZ29vZ2xlLmNv\n" \
"bS50coIPKi5nb29nbGUuY29tLnZuggsqLmdvb2dsZS5kZYILKi5nb29nbGUuZXOC\n" \
"CyouZ29vZ2xlLmZyggsqLmdvb2dsZS5odYILKi5nb29nbGUuaXSCCyouZ29vZ2xl\n" \
"Lm5sggsqLmdvb2dsZS5wbIILKi5nb29nbGUucHSCEiouZ29vZ2xlYWRhcGlzLmNv\n" \
"bYIPKi5nb29nbGVhcGlzLmNughEqLmdvb2dsZWNuYXBwcy5jboIUKi5nb29nbGVj\n" \
"b21tZXJjZS5jb22CESouZ29vZ2xldmlkZW8uY29tggwqLmdzdGF0aWMuY26CDSou\n" \
"Z3N0YXRpYy5jb22CEiouZ3N0YXRpY2NuYXBwcy5jboIKKi5ndnQxLmNvbYIKKi5n\n" \
"dnQyLmNvbYIUKi5tZXRyaWMuZ3N0YXRpYy5jb22CDCoudXJjaGluLmNvbYIQKi51\n" \
"cmwuZ29vZ2xlLmNvbYITKi53ZWFyLmdrZWNuYXBwcy5jboIWKi55b3V0dWJlLW5v\n" \
"Y29va2llLmNvbYINKi55b3V0dWJlLmNvbYIWKi55b3V0dWJlZWR1Y2F0aW9uLmNv\n" \
"bYIRKi55b3V0dWJla2lkcy5jb22CByoueXQuYmWCCyoueXRpbWcuY29tghphbmRy\n" \
"b2lkLmNsaWVudHMuZ29vZ2xlLmNvbYILYW5kcm9pZC5jb22CG2RldmVsb3Blci5h\n" \
"bmRyb2lkLmdvb2dsZS5jboIcZGV2ZWxvcGVycy5hbmRyb2lkLmdvb2dsZS5jboIE\n" \
"Zy5jb4IIZ2dwaHQuY26CDGdrZWNuYXBwcy5jboIGZ29vLmdsghRnb29nbGUtYW5h\n" \
"bHl0aWNzLmNvbYIKZ29vZ2xlLmNvbYIPZ29vZ2xlY25hcHBzLmNughJnb29nbGVj\n" \
"b21tZXJjZS5jb22CGHNvdXJjZS5hbmRyb2lkLmdvb2dsZS5jboIKdXJjaGluLmNv\n" \
"bYIKd3d3Lmdvby5nbIIIeW91dHUuYmWCC3lvdXR1YmUuY29tghR5b3V0dWJlZWR1\n" \
"Y2F0aW9uLmNvbYIPeW91dHViZWtpZHMuY29tggV5dC5iZTAhBgNVHSAEGjAYMAgG\n" \
"BmeBDAECAjAMBgorBgEEAdZ5AgUDMC8GA1UdHwQoMCYwJKAioCCGHmh0dHA6Ly9j\n" \
"cmwucGtpLmdvb2cvR1RTMU8xLmNybDCCAQUGCisGAQQB1nkCBAIEgfYEgfMA8QB3\n" \
"ALIeBcyLos2KIE6HZvkruYolIGdr2vpw57JJUy3vi5BeAAABceQk7HEAAAQDAEgw\n" \
"RgIhAMeaPA4eH4PQ6P80cE7pV9+cA+JzHsfEHXybUpJYTUUqAiEAtt6UgO7o9Bo5\n" \
"T83b8KngOaFoAMEitVL5ckcpZHNtOaQAdgBep3P531bA57U2SH3QSeAyepGaDISh\n" \
"EhKEGHWWgXFFWAAAAXHkJOyPAAAEAwBHMEUCIQDLXewTtt5w1Pykw4iJLVEBNWsU\n" \
"8LazX+MIkUmuOpwJQwIgIXySwReiZv9FbIJ/73ytICnrPGNkD43+PVMge9cJ0gQw\n" \
"DQYJKoZIhvcNAQELBQADggEBAM/x3sGLTHpoan0gd9XdVCG6HsQynQEzLL25kTvN\n" \
"q14pRburIrsdFWhtEETqNJo7IG19PozR6IdJkQqEBYjw6D9rRniikWeski5MhcFD\n" \
"fU3ycQ2I2ISlAd3HelYNbVDJ8zJWerEGk7dyi2O/UmRreXHhi+dry8IgTiSssYNA\n" \
"fht9x4GVvFTT6gap4E1Qy5UFMBGMqXmhXNoH9bYX1oY6N+8xr5tkTSQfV0T1kXKJ\n" \
"WUhnDkwMUnvwuMUEP7elcvp7OgOslSQ5zwjnR3zSDozO072YR+L9SGMICCZz1ha1\n" \
"DMCTA95gzVKezFCaUidRU9UyHOFzltfYDt7HRlp7MwWoPLM=\n" \
"-----END CERTIFICATE-----\n";

String getMacAddress() {
  byte mac[6];

  WiFi.macAddress(mac);
  String cMac = "";
  for (int i = 0; i < 6; ++i) {
    if (mac[i] < 0x10) {
      cMac += "0";
    }
    cMac += String(mac[i], HEX);
    if (i < 5)
      cMac += ":"; // put : or - if you want byte delimiters
  }
  cMac.toUpperCase();
  return cMac;
}


void setup() {
  //We need to start setting up some variables and other things needed.
  Serial.begin(115200);
  M5.begin();
  
  SPIFFS.begin(true);
     if(!SPIFFS.begin()){
        Serial.println("SPIFFS Mount Failed");
        return;
    }
 WiFi.mode(WIFI_STA);  WiFi.begin(ssid, password);
  Serial.print("MAC: ");
  Serial.println(getMacAddress());
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  TTS tts;
  downloadFile(tts.getSpeechUrl("Hello World"), "tmp.mp3");
  delay(300);
  file = new AudioFileSourceSPIFFS("/tmp.mp3");
  id3 = new AudioFileSourceID3(file);
  out = new AudioOutputI2S(0, 1); // Output to builtInDAC
  out->SetOutputModeMono(true);
  mp3 = new AudioGeneratorMP3();
  mp3->begin(id3, out);    
}

void loop() {
 if (mp3->isRunning()) {
    if (!mp3->loop()) mp3->stop();
  } else {
    Serial.printf("MP3 done\n");
    delay(1000);
  }
}


void downloadFile(String url, String filename) {
  WiFiClientSecure *client = new WiFiClientSecure;
  client -> setCACert(rootCACertificate);
  

  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient https;

    Serial.print("[HTTP] begin...\n");

    // configure server and url
   if (https.begin(*client, url)) {
Serial.println("DOWNLOADING " + url + " and saving as " + filename);
    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = https.GET();
    if (httpCode > 0) {
      //SPIFFS.remove(filename);
      File f = SPIFFS.open(filename, "w+");
      if (!f) {
        Serial.println("file open failed");
        return;
      }
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {

        // get lenght of document (is -1 when Server sends no Content-Length header)
        int total = https.getSize();
        int len = total;
        //progressCallback(filename, 0, total);
        // create buffer for read
        uint8_t buff[2048] = { 0 };
        Serial.println("START STREAM");
        // get tcp stream
        WiFiClient * stream = https.getStreamPtr();

        // read all data from server
        while (https.connected() && (len > 0 || len == -1)) {
          // get available data size
          size_t size = stream->available();

          if (size) {
            // read up to 128 byte
            int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

            // write it to Serial
            f.write(buff, c);

            if (len > 0) {
              len -= c;
            }
            //progressCallback(filename, total - len, total);
          }
          delay(1);
        }
        Serial.println("STREAM DONE");
        Serial.println();
        Serial.print("[HTTP] connection closed or file end.\n");

      }
      f.close();
      Serial.print("File Closed");
    }
    else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
  }
}
}

/*
 *The MIT License (MIT)
Copyright (c) 2018 by Giuliano Pisoni
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 
 */
