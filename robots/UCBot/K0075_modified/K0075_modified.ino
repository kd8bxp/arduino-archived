// UCBOT DEMO
// Web: http://www.uctronics.com
// This program is a demo of how to use most of the functions of UCBOT.
// There are some differences between this example and the ESP-IDF firmware. 
// In order to be compatible with the factory firmware as much as possible, WIFI name and version information, battery power and other functions have not been added.
// It can view video and command the car to enter different modes through the browser or phone app
// The control command is in the file of README. 

#include "ucbot_camera.h"
#include <WiFi.h>
#include "I2C_slave.h"
#include <Arduino.h>
#include <Wire.h>

const char* ap_ssid = "UCBOT_630C"; //set the ap
const char* ap_password = "12345678"; //set the secret

static uint8_t isr_flag = 0;
int ret;

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void startCameraServer();

void IRAM_ATTR isr() {
  isr_flag = 1;
}

void interrupt_init(){
  pinMode(33, INPUT);
  attachInterrupt(33, isr, CHANGE);
}
uint8_t ultra_dis = 0;

void setup() {
  uint8_t tcs_status = 0;
  Serial.begin(115200);
  Wire.begin();
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    //return;
  }
  else{
    Serial.printf("Camera init success!");
    Serial.println();
    wrSensorReg8_8(STM32_2_SLAVE_ADDR, CAMERA_STATUS, 1);  
  }

  //drop down frame size for higher initial frame rate
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_VGA);

    Serial.println("Configuring access point...");
  
    // You can remove the password parameter if you want the AP to be open.
    WiFi.softAP(ap_ssid, ap_password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    startCameraServer();
    
    Serial.println("Server started");
    //} 
    rdSensorReg8_8(STM32_1_SLAVE_ADDR, TCS_STATUS, &tcs_status);
    Serial.print(tcs_status, DEC);
    if(tcs_status==0){
        wrSensorReg8_8(STM32_2_SLAVE_ADDR, TCS_STATUS, 1);  //color sensor init success
      }
    wrSensorReg8_8(STM32_2_SLAVE_ADDR, ESP32_STATUS, 1);  //esp32 init success
    
    while(1){   //read the distance of ultrasonic detected 
        if(avoid_mode){
          rdSensorReg8_8(STM32_2_SLAVE_ADDR, ULTRA_REG, &ultra_dis);
          if(ultra_dis<15){
            wrSensorReg8_8(STM32_1_SLAVE_ADDR, ULTRA_REG, 1);
            }
          else{
            wrSensorReg8_8(STM32_1_SLAVE_ADDR, ULTRA_REG, 0);
            }
        }
        if(follow_mode){
          rdSensorReg8_8(STM32_2_SLAVE_ADDR, ULTRA_REG, &ultra_dis);
          if(ultra_dis<8){
            wrSensorReg8_8(STM32_1_SLAVE_ADDR, ULTRA_REG, 1);
            }
          else{
            wrSensorReg8_8(STM32_1_SLAVE_ADDR, ULTRA_REG, 0);
            }
        }
          delay(20);
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10);  
}
