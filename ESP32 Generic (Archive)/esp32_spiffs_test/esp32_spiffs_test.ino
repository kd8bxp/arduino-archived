/*
 * download ESP32 Spiffs tool from the releases page,
 * https://github.com/me-no-dev/arduino-esp32fs-plugin/releases
 *  Install info from here:
 *  https://github.com/me-no-dev/arduino-esp32fs-plugin
 */


#include "FS.h"
#include "SPIFFS.h"

bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);
Serial.println("OPen");
Serial.println(buf.get());
 /* StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  strcpy(mqtt_server, (const char*)json["mqtt_server"]);
  strcpy(mqttnametemp,(const char*)json["mqttname"]);
  mqttname = String(mqttnametemp);
  strcpy(host,(const char*)json["host"]);
statustop = "tele/" + mqttname + "/STATUS";
*/
  return true;
}

void setup() {
  Serial.begin(115200);

  Serial.println("Mounting FS...");

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
  if (!loadConfig()) {
    Serial.println("Failed to load config");
  } else {
    Serial.println("Config loaded");
  }
}

void loop() {
  
}
