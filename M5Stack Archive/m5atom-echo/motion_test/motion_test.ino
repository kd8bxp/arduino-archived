#include "src/M5Atom/M5Atom.h"
#include "AtomMotion.h"
#include <driver/i2s.h>

#include "fft.h"

#define CONFIG_I2S_BCK_PIN 19
#define CONFIG_I2S_LRCK_PIN 33
#define CONFIG_I2S_DATA_PIN 22
#define CONFIG_I2S_DATA_IN_PIN 23

//#define SPAKER_I2S_NUMBER I2S_NUM_0
#define SPAKER_I2S_NUMBER I2S_NUM_1

#define MODE_MIC 0
#define MODE_SPK 1

extern const unsigned char audio[364808];

AtomMotion Atom;
int direction = 0; // 0 = stop, 1 = forward, 2 = reverse, 3 = left, 4 = right

//servo angle range 0 ~ 180
//DC motor speed range -127~127

size_t bytes_written;
bool state = true;
bool loop_play = false;
int8_t i2s_readraw_buff[1024];
size_t bytesread;
uint16_t count_n = 0;
double data = 0;
double maxData = 0;
uint16_t ydata;
float adc_data;
int16_t *buffptr;
double redatabuff[512] = {0};
uint16_t pos = 0;
uint16_t count_sys = 0, count_24 = 0;


bool InitI2SSpakerOrMic(int mode)
{
    esp_err_t err = ESP_OK;

    i2s_driver_uninstall(SPAKER_I2S_NUMBER);
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

    //Serial.println("Init i2s_driver_install");

    err += i2s_driver_install(SPAKER_I2S_NUMBER, &i2s_config, 0, NULL);
    i2s_pin_config_t tx_pin_config;

    tx_pin_config.bck_io_num = CONFIG_I2S_BCK_PIN;
    tx_pin_config.ws_io_num = CONFIG_I2S_LRCK_PIN;
    tx_pin_config.data_out_num = CONFIG_I2S_DATA_PIN;
    tx_pin_config.data_in_num = CONFIG_I2S_DATA_IN_PIN;

    //Serial.println("Init i2s_set_pin");
    err += i2s_set_pin(SPAKER_I2S_NUMBER, &tx_pin_config);
    //Serial.println("Init i2s_set_clk");
    err += i2s_set_clk(SPAKER_I2S_NUMBER, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);

    return true;
}



void TaskMotion(void *pvParameters){
  while (1)
  {
    
    
        
    vTaskDelay( 100 / portTICK_RATE_MS );
    if(direction == 0){
      //stop
      Atom.SetMotorSpeed(1, 0);
      Atom.SetMotorSpeed(2, 0);
      M5.dis.drawpix(0, 0x00FF00); //red
    }
     else if (direction == 1) {
      //forward
      Atom.SetMotorSpeed(1,100);
      Atom.SetMotorSpeed(2,100);
      M5.dis.drawpix(0, 0xff0000); //green
    }
    else if (direction == 2) { 
      //reverse
      Atom.SetMotorSpeed(1,-65);
      Atom.SetMotorSpeed(2,-65);
      M5.dis.drawpix(0, 0x0000ff); //blue
    }
    else if (direction == 3) {
      //left
      Atom.SetMotorSpeed(1, -60);
      Atom.SetMotorSpeed(2, 50);
      M5.dis.drawpix(0, 0x00eeee);
    }
    else if (direction == 4) {
      //right
      Atom.SetMotorSpeed(1, 50);
      Atom.SetMotorSpeed(2, -50);
      M5.dis.drawpix(0, 0x00eeee);
    }
  }
  //vTaskDelay(1000/ portTICK_RATE_MS);
}


void setup(){ 
Serial.begin(115200);
  
  //M5.begin(true, true, true);
  M5.begin(true, false, true);
  Atom.Init();     //sda  25     scl  21 

  InitI2SSpakerOrMic(MODE_SPK);
  
  xTaskCreatePinnedToCore(
    TaskMotion
    ,  "TaskMotion"   // A name just for humans
    ,  4096  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  0);

 direction = 0;
 delay(3000);
}

void loop() {
    M5.update();
    direction = 1;
    delay(3000);
    direction = 0;
    delay(3000);
    direction = 2;
    delay(3000);
    direction = 0;
    delay(3000);
    direction = 3;
    delay(3000);
    direction = 0;
    delay(3000);
    direction = 4;
    delay(3000);
    direction = 0;
    delay(1000);
    i2s_write(SPAKER_I2S_NUMBER, audio, 364808, &bytes_written, portMAX_DELAY);

}
