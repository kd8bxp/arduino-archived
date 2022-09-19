/*
 * By GEH00073
 * original tweet:
 * https://twitter.com/GEH00073/status/1219973134121439240
 * website information:
 * https://qiita.com/airpocket/items/0cfb9e3e1a202c2f00df
 * face models here:
 * http://dl.sipeed.com/MAIX/MaixPy/release/maixpy_v0.3.0/
 * 
 *  Jan 21-22, 2020
 */



#include "src/M5StickC/M5StickC.h"

HardwareSerial VSerial(1);

typedef struct
{
    int16_t dx;
    uint32_t area;
}v_response_t;


uint8_t I2CWrite1Byte(uint8_t Addr, uint8_t Data)
{
    Wire.beginTransmission(0x38);
    Wire.write(Addr);
    Wire.write(Data);
    return Wire.endTransmission();
}

uint8_t I2CWritebuff(uint8_t Addr, uint8_t *Data, uint16_t Length)
{
    Wire.beginTransmission(0x38);
    Wire.write(Addr);
    for (int i = 0; i < Length; i++)
    {
        Wire.write(Data[i]);
    }
    return Wire.endTransmission();
}

uint8_t Setspeed(int16_t Vtx, int16_t Vty, int16_t Wt)
{
    int16_t speed_buff[4] = {0};
    int8_t speed_sendbuff[4] = {0};

    Wt = (Wt > 100) ? 100 : Wt;
    Wt = (Wt < -100) ? -100 : Wt;

    Vtx = (Vtx > 100) ? 100 : Vtx;
    Vtx = (Vtx < -100) ? -100 : Vtx;
    Vty = (Vty > 100) ? 100 : Vty;
    Vty = (Vty < -100) ? -100 : Vty;

    Vtx = (Wt != 0) ? Vtx * (100 - abs(Wt)) / 100 : Vtx;
    Vty = (Wt != 0) ? Vty * (100 - abs(Wt)) / 100 : Vty;

    speed_buff[0] = Vty - Vtx - Wt;
    speed_buff[1] = Vty + Vtx + Wt;
    speed_buff[3] = Vty - Vtx + Wt;
    speed_buff[2] = Vty + Vtx - Wt;

    for (int i = 0; i < 4; i++)
    {
        speed_buff[i] = (speed_buff[i] > 100) ? 100 : speed_buff[i];
        speed_buff[i] = (speed_buff[i] < -100) ? -100 : speed_buff[i];
        speed_sendbuff[i] = speed_buff[i];
    }
    return I2CWritebuff(0x00, (uint8_t *)speed_sendbuff, 4);
}

void setup()
{
    M5.begin();
    M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(RED);

    VSerial.begin(115200, SERIAL_8N1, 33, 32);
    Wire.begin(0, 26);
}

enum
{
    kNoTarget = 0,
    kLeft,
    kRight,
    kStraight,
    kTooClose
};

const uint16_t kThreshold = 20; //40; // If target is in range ±kThreshold, the car will go straight defo = 20
v_response_t v_data;    // Data read back from V
uint8_t state = 0;  // Car's movement status
int rotate = 1;     //回転方向指定
int dx_old = 0;     //ターゲットの過去位置

void loop()
{
    VSerial.write(0xAF);
    double vpara = .6; //0.4;              //速度一括変更 Speed Change
    int movetime = 100; //150;              //探索時のローテーション運動の継続時間(msec)。movetime(msec)後waitcam(msec)停止
    int waitcam = 200; //150;               //カメラ安定のための停止時間(msec)


    if(VSerial.available())
    {
        uint8_t buffer[5];

        VSerial.readBytes(buffer, 5);
        v_data.dx = (buffer[0] << 8) | buffer[1];
        v_data.area = (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];

        if(v_data.dx > -160 && v_data.dx < 160)
        {
            M5.Lcd.fillScreen(GREEN);         //ターゲットを認識している間はLCD背景をグリーン表示
            if(v_data.area > 2600 && v_data.area < 2660)           //オブジェクト領域の面積の閾値　面積が閾値以上だと十分近づいたと判断して停止。
            {
                state = kTooClose;  // Stop
                M5.Lcd.fillScreen(BLUE);      //ターゲットに十分接近したらLCD背景をブルー表示
                M5.Lcd.drawString("Too Close", 0, 0);
            }
            else if(v_data.dx > -kThreshold && v_data.dx < kThreshold)
            {
                state = kStraight;  // Go straight
                M5.Lcd.drawString("detect Straight",0,0);  //動作状況確認のためステータス表示を追加
                M5.Lcd.drawNumber(v_data.dx,0,15);
                M5.Lcd.drawNumber(v_data.area,0,30);
            }

            else if(v_data.dx <= -kThreshold)
            {
                state = kRight;  // Go Right
                M5.Lcd.drawString("detect Right",0,0);
                M5.Lcd.drawNumber(v_data.dx,0,15);
                M5.Lcd.drawNumber(v_data.area,0,30);
            }

            else if(v_data.dx >= kThreshold)
            {
                state = kLeft; // Go Left
                M5.Lcd.drawString("detect Left",0,0);
                M5.Lcd.drawNumber(v_data.dx,0,15);
                M5.Lcd.drawNumber(v_data.area,0,30);
            }

            else
            {
                M5.Lcd.drawString("detect Out of range",0,15);
                M5.Lcd.drawNumber(v_data.dx,0,15);
                M5.Lcd.drawNumber(v_data.area,0,30);
                state = kNoTarget;  // Rotate
            }

            if (v_data.dx > 0)                 //回転方向を最後にターゲットを見つけた方向に変更する。
            {
                rotate = 1; 
            }
            else
            {
                rotate = -1;
            }
        }
        else
        {
//            M5.Lcd.fillScreen(RED);　　　　　　　　　　　　　　　　
            M5.Lcd.fillScreen(RED);
            if(dx_old < 160 && dx_old > -160)                   //ターゲットたった今見失ったら1秒停止（その間にブレの無い画像を取得して再判定する。
            {
                M5.Lcd.drawString("Just lost, waiting",0,0);
                delay(1000);
            }
            else                                                //ターゲットを見失っている間はその場で回転して探索。
            {
                state = kNoTarget;  // Rotate
                M5.Lcd.drawString("NO target",0,0);
            }

        }
        //Serial.printf("%d, %d,%d, %d\n", v_data.dx, dx_old,v_data.area, state);
        dx_old = v_data.dx;
    }
    M5.Lcd.setCursor(35, 15);
M5.Lcd.printf("%.2fV, %.2fma", M5.Axp.GetBatVoltage(), M5.Axp.GetBatCurrent());
    switch(state)
    {
        case kNoTarget:
            Setspeed(0, 0, 40 * rotate * vpara);   //vpara batch change speed with
            delay(movetime);                       //movetime(msec)回転を継続
            Setspeed(0, 0, 0);
            delay(waitcam);                        //waitcam(msec)カメラの安定待ち時間
        break;

        case kLeft:
            Setspeed(40 * vpara, 0, 0);            //vparaで速度一括変更
            delay(20);
            Setspeed(0, 0, 0);
        break;

        case kRight:
            Setspeed(-40 * vpara, 0, 0);           //vparaで速度一括変更
            delay(20);
            Setspeed(0, 0, 0);
        break;

        case kStraight:
            Setspeed(0, 40 * vpara, 0);            //vparaで速度一括変更
            delay(20);
            Setspeed(0, 0, 0);
        break;

        case kTooClose:
            Setspeed(0, 0, 0);
        break;

    }
}
