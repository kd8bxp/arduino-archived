/*Press button to record,released button to playback*/

#include <driver/i2s.h>
#include "src/M5Atom/M5Atom.h"
#include "src/PubSubClient/PubSubClient.h"
#include <HTTPClient.h>
#include <DNSServer.h>

#define CONFIG_I2S_BCK_PIN 19
#define CONFIG_I2S_LRCK_PIN 33
#define CONFIG_I2S_DATA_PIN 22
#define CONFIG_I2S_DATA_IN_PIN 23

#define SPEAKER_I2S_NUMBER I2S_NUM_0

#define MODE_MIC 0
#define MODE_SPK 1
#define DATA_SIZE 1024
// Replace with your network credentials
const char* ssid     = "homeinternet";
const char* password = "trixie4me";
const char* mqtt_server = "simplesi.cloud";

#define deviceName "kd8bxp"
#define sendTo "kd8bxp2" 

WiFiClient espClient;
PubSubClient client(espClient);

uint8_t microphonedata0[1024 * 80];
int data_offset = 0;

void callback(char* topic, byte* payload, unsigned int length) {
size_t bytes_written;
        InitI2SSpeakerOrMic(MODE_SPK);
        i2s_write(SPEAKER_I2S_NUMBER, microphonedata0, data_offset, &bytes_written, portMAX_DELAY);
i2s_write(SPEAKER_I2S_NUMBER, payload, length, &bytes_written, portMAX_DELAY);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(deviceName);
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void InitI2SSpeakerOrMic(int mode)
{
    esp_err_t err = ESP_OK;

    i2s_driver_uninstall(SPEAKER_I2S_NUMBER);
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER),
        .sample_rate = 16000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
        .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 6,
        .dma_buf_len = 60,
    };
    if (mode == MODE_MIC)
    {
        i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
    }
    else
    {
        i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
        i2s_config.use_apll = false;
        i2s_config.tx_desc_auto_clear = true;
    }

    err += i2s_driver_install(SPEAKER_I2S_NUMBER, &i2s_config, 0, NULL);
    i2s_pin_config_t tx_pin_config;

    tx_pin_config.bck_io_num = CONFIG_I2S_BCK_PIN;
    tx_pin_config.ws_io_num = CONFIG_I2S_LRCK_PIN;
    tx_pin_config.data_out_num = CONFIG_I2S_DATA_PIN;
    tx_pin_config.data_in_num = CONFIG_I2S_DATA_IN_PIN;

    //Serial.println("Init i2s_set_pin");
    err += i2s_set_pin(SPEAKER_I2S_NUMBER, &tx_pin_config);
    //Serial.println("Init i2s_set_clk");
    err += i2s_set_clk(SPEAKER_I2S_NUMBER, 16000, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

void setup() {
    M5.begin(true, false, true);
    M5.dis.drawpix(0, CRGB(128, 128, 0));
    delay(2000);
WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println (".");
  }
  Serial.println(WiFi.localIP());

 client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.subscribe(deviceName);
}

void loop() {
 if (!client.connected()) {
    reconnect();
  }
    if (M5.Btn.isPressed())
    {
        data_offset = 0;
        InitI2SSpeakerOrMic(MODE_MIC);
        M5.dis.drawpix(0, CRGB(128, 128, 0));
        size_t byte_read;
        
        while (1)
        {
            i2s_read(SPEAKER_I2S_NUMBER, (char *)(microphonedata0 + data_offset), DATA_SIZE, &byte_read, (100 / portTICK_RATE_MS));
            data_offset += 1024;
            client.publish(sendTo, microphonedata0);
            M5.update();
            if (M5.Btn.isReleased())
                break;
            //delay(60);
        }
        delay(1);
    }
    M5.update();
client.loop();
}
