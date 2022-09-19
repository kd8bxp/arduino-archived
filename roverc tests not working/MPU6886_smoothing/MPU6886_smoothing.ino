#include "src/M5StickC/M5StickC.h"
#include "src/M5StickC/utility/MPU6886.h"
#define N_CAL2   100

float pitch, roll, yaw;

const int numReadings = 15;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
float total = 0.0;                  // the running total
float average = 0.0;                // the average

float gyroXoffset, gyroYoffset, gyroZoffset, accXoffset;
float gyroXdata, gyroYdata, gyroZdata, accXdata, accZdata;
float yawAngle = 0.0;
float varAng, varOmg, varSpd, varDst, varIang;
const float clk         = 0.01; 
float cutoff            = 0.1;                     //~=2 * pi * f (Hz)

void taskAvg(void *pvParameters) {
  while(1) {
    vTaskDelay( 100 / portTICK_RATE_MS);
    //M5.IMU.getAttitude(&pitch, &roll);
    M5.IMU.getGyroData(&pitch, &roll, &yaw);
   // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = yaw;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = (total / numReadings) ;
  }
  
}

void setup()
{
    M5.begin();
    M5.IMU.Init();
    Wire1.begin(21,22);
    // M5.Mpu6886.SetGyroFsr(M5.Mpu6886.GFS_250DPS);  // 250DPS 500DPS 1000DPS 2000DPS
    //M5.Mpu6886.SetAccelFsr(M5.Mpu6886.AFS_4G);  // 2G 4G 8G 16G
    //calib2();
    //vSemaphoreCreateBinary( CtlSemaphore );
  xTaskCreatePinnedToCore(
    taskAvg
    ,  "TaskAvg"   // A name just for humans
    ,  4096  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  0);
    
}


void loop()
{
    delay(50);
    
    //Serial.printf("%.2f,%.2f,%.2f,%.2f\n", pitch, roll, yaw, /*arc,*/ val);
    Serial.println(average); //* 180.0/3.14);

   
    M5.update();
;
}

void calib2() {
    
    calDelay(80);
    
    //accXoffset  = 0.0;
    gyroZoffset = 0.0;
    for (int i = 0; i < N_CAL2; i++) {
        readGyro();
        //accXoffset += accXdata;
        gyroZoffset += gyroZdata;
        delay(9);
    }
   // accXoffset /= (float)N_CAL2;
    gyroZoffset /= (float)N_CAL2;
    
}

void calDelay(int n) {
    for (int i = 0; i < n; i++) {
        getGyro();
        delay(9);
    }
}

void readGyro() {
    float gX, gY, gZ, aX, aY, aZ;
    M5.Imu.getGyroData(&gX, &gY, &gZ);
    M5.Imu.getAccelData(&aX, &aY, &aZ);
    gyroYdata = gX;
    gyroZdata = -gY;
    gyroXdata = -gZ;
    accXdata  = aZ;
    accZdata  = aY;
}

void getGyro() {
    readGyro();
    //varOmg = (gyroYdata - gyroYoffset);           // unit:deg/sec
    yawAngle += (gyroZdata - gyroZoffset) * clk;  // unit:g
    //varAng += (varOmg + ((accXdata - accXoffset) * 57.3 - varAng) * cutoff) *
              clk;  // complementary filter
}
