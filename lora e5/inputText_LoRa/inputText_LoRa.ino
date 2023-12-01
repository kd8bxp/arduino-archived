/**
 * @file inputText.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief M5Cardputer input text test
 * @version 0.1
 * @date 2023-10-13
 *
 *
 * @Hardwares: M5Cardputer
 * @Platform Version: Arduino M5Stack Board Manager v2.0.7
 * @Dependent Library:
 * M5GFX: https://github.com/m5stack/M5GFX
 * M5Unified: https://github.com/m5stack/M5Unified
 */

/* Nov 29, 2023 Modified to work with Grove LoRa E5, with parts of the Seeed Studio P2P code */

#include "src/M5Cardputer/M5Cardputer.h"
#include "src/M5GFX/M5GFX.h"

 #define COMSerial Serial1
 #define ShowSerial Serial
 #define RXD2  G1 //33 
 #define TXD2  G2 //32 

M5Canvas canvas(&M5Cardputer.Display);

static char recv_buf[512];
static bool is_exist = false;
String data = "> ";

static int at_send_check_response(char *p_ack, int timeout_ms, char*p_cmd, ...)
{
    int ch = 0;
    int index = 0;
    int startMillis = 0;
    va_list args;
    memset(recv_buf, 0, sizeof(recv_buf));
    va_start(args, p_cmd);
    COMSerial.printf(p_cmd, args);
    ShowSerial.printf(p_cmd, args);
    va_end(args);
    delay(200);
    startMillis = millis();

    if (p_ack == NULL)
    {
        return 0;
    }

    do
    {
        while (COMSerial.available() > 0)
        {
            ch = COMSerial.read();
            recv_buf[index++] = ch;
            ShowSerial.print((char)ch);
            delay(2);
        }

        if (strstr(recv_buf, p_ack) != NULL)
        {
            return 1;
        }

    } while (millis() - startMillis < timeout_ms);
    return 0;
}

static int recv_prase(void)
{
    char ch;
    int index = 0;
    memset(recv_buf, 0, sizeof(recv_buf));
    while (COMSerial.available() > 0)
    {
        ch = COMSerial.read();
        recv_buf[index++] = ch;
        ShowSerial.print((char)ch);
        delay(2);
    }

    if (index)
    {
        char *p_start = NULL;
        char data[32] = {
            0,
        };
        int rssi = 0;
        int snr = 0;

        p_start = strstr(recv_buf, "+TEST: RX \"5345454544");
        if (p_start)
        {
            p_start = strstr(recv_buf, "5345454544");
            if (p_start && (1 == sscanf(p_start, "5345454544%s", data)))
            {
                data[4] = 0;
                //u8x8.setCursor(0, 4);
                //u8x8.print("               ");
                //u8x8.setCursor(2, 4);
                //u8x8.print("RX: 0x");
                //u8x8.print(data);
                ShowSerial.print("RX: 0x");
                ShowSerial.print(data);
                ShowSerial.print("\r\n");
            }

            p_start = strstr(recv_buf, "RSSI:");
            if (p_start && (1 == sscanf(p_start, "RSSI:%d,", &rssi)))
            {
                //u8x8.setCursor(0, 6);
                //u8x8.print("                ");
                //u8x8.setCursor(2, 6);
                //u8x8.print("rssi:");
                //u8x8.print(rssi);
                ShowSerial.print("rssi: "); ShowSerial.println(rssi);
            }
            p_start = strstr(recv_buf, "SNR:");
            if (p_start && (1 == sscanf(p_start, "SNR:%d", &snr)))
            {
                //u8x8.setCursor(0, 7);
                //u8x8.print("                ");
                //u8x8.setCursor(2, 7);
                //u8x8.print("snr :");
                //u8x8.print(snr);
                ShowSerial.print("snr: ");ShowSerial.println(snr);
            }
            return 1;
        }
    }
    return 0;
}

static int node_recv(uint32_t timeout_ms)
{
    at_send_check_response("+TEST: RXLRPKT", 1000, "AT+TEST=RXLRPKT\r\n");
    int startMillis = millis();
    do
    {
        if (recv_prase())
        {
            return 1;
        }
    } while (millis() - startMillis < timeout_ms);
    return 0;
}

static int node_send(void)
{
    static uint16_t count = 0;
    int ret = 0;
    char data[32];
    char cmd[128];

    memset(data, 0, sizeof(data));
    sprintf(data, "%04X", count);
    sprintf(cmd, "AT+TEST=TXLRPKT,\"5345454544%s\"\r\n", data);

    //u8x8.setCursor(0, 3);
    //u8x8.print("                ");
    //u8x8.setCursor(2, 3);
    //u8x8.print("TX: 0x");
    //u8x8.print(data);
    ShowSerial.print("TX: 0x");
    ShowSerial.println(data);
    
    ret = at_send_check_response("TX DONE", 2000, cmd);
    if (ret == 1)
    {

        count++;
        ShowSerial.print("Sent successfully!\r\n");
    }
    else
    {
        ShowSerial.print("Send failed!\r\n");
    }
    return ret;
}

static void node_recv_then_send(uint32_t timeout)
{
    int ret = 0;
    ret = node_recv(timeout);
    delay(100);
    if (!ret)
    {
        ShowSerial.print("\r\n");
        return;
    }
    node_send();
    ShowSerial.print("\r\n");
}

static void node_send_then_recv(uint32_t timeout)
{
    int ret = 0;
    ret = node_send();
    if (!ret)
    {
        ShowSerial.print("\r\n");
        return;
    }
    if (!node_recv(timeout))
    {
        ShowSerial.print("recv timeout!\r\n");
    }
    ShowSerial.print("\r\n");
}


void setup() {
    auto cfg = M5.config();
    ShowSerial.begin(115200);
    COMSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
    M5Cardputer.begin(cfg, true);
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.setTextSize(0.5);
    M5Cardputer.Display.drawRect(2, 0, M5Cardputer.Display.width()-2,
                                 M5Cardputer.Display.height() - 28, GREEN);
    M5Cardputer.Display.setTextFont(&fonts::FreeSerifBoldItalic18pt7b);

    M5Cardputer.Display.fillRect(0, M5Cardputer.Display.height() - 4,
                                 M5Cardputer.Display.width(), 4, GREEN);

    canvas.setTextFont(&fonts::FreeSerifBoldItalic18pt7b);
    canvas.setTextSize(0.5);
    canvas.createSprite(M5Cardputer.Display.width() - 8,
                        M5Cardputer.Display.height() - 36);
    canvas.setTextScroll(true);
    canvas.println("Press Key and Enter to Input Text");
    canvas.pushSprite(4, 4);
    M5Cardputer.Display.drawString(data, 4, M5Cardputer.Display.height() - 24);

  if (at_send_check_response("+AT: OK", 100, "AT\r\n"))
    {
        is_exist = true;
        at_send_check_response("+MODE: TEST", 1000, "AT+MODE=TEST\r\n");
        at_send_check_response("+TEST: RFCFG", 1000, "AT+TEST=RFCFG,866,SF12,125,12,15,14,ON,OFF,OFF\r\n");
        delay(200);
          }
    else
    {
        is_exist = false;
        ShowSerial.print("No E5 module found.\r\n");
        //u8x8.setCursor(0, 1);
        //u8x8.print("unfound E5 !");
    }
}

void loop() {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

            for (auto i : status.word) {
                data += i;
            }

            if (status.del) {
                data.remove(data.length() - 1);
            }

            if (status.enter) {
                data.remove(0, 2);
                canvas.println(data);
                canvas.pushSprite(4, 4);
                //char buf[50];
                //data.toCharArray(buf, data.length()+1);
    char cmd[128];
    char dataBuf[128];
    data.toCharArray(dataBuf, data.length()+1);
    memset(dataBuf, 0, sizeof(dataBuf));
    //sprintf(data, "%04X", count);
    sprintf(cmd, "AT+TEST=TXLRPKT,\"5345454544%s\"\r\n", dataBuf); 
    int ret = 0;               
   ret = at_send_check_response("TX DONE", 2000, cmd);
                data = "> ";
            }

            M5Cardputer.Display.fillRect(0, M5Cardputer.Display.height() - 28,
                                         M5Cardputer.Display.width(), 25,
                                         BLACK);

            M5Cardputer.Display.drawString(data, 4,
                                           M5Cardputer.Display.height() - 24);
        }
    }
 /*  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    //char buf1[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    //if (rf95.available()) {
      while (rf95.available()) {
        // Should be a reply message for us now
        if (rf95.recv(buf, &len)) {
            ShowSerial.print("got reply: ");
            ShowSerial.println((char*)buf);
            //M5Cardputer.Display.fillRect(0, M5Cardputer.Display.height() - 28, M5Cardputer.Display.width(), 25, BLACK);
            //M5Cardputer.Display.drawString(String((char*)buf), 4, M5Cardputer.Display.height() - 24);
            canvas.println((char*)buf);
            canvas.pushSprite(4,4);
        } else {
            ShowSerial.println("recv failed");
        }
    }
    */ 
}
