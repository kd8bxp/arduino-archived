# include <Arduino.h>
//# include <U8x8lib.h>
#include "src/M5StickC/M5StickC.h"

// https://wiki.seeedstudio.com/Grove_Wio_E5_P2P/

#define NODE_SLAVE

//U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/*reset=*/U8X8_PIN_NONE);
// U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/*clock=*/ SCL, /*data=*/ SDA, /*reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display

   #define COMSerial Serial1
    #define ShowSerial Serial
#define RXD2  33 
#define TXD2  32 


static char recv_buf[512];
static bool is_exist = false;

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

void setup(void)
{
  M5.begin();

    //u8x8.begin();
    //u8x8.setFlipMode(1);
    //u8x8.setFont(u8x8_font_chroma48medium8_r);

    ShowSerial.begin(115200);
    // while (!ShowSerial);

    COMSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
    ShowSerial.print("ping pong communication!\r\n");
    //u8x8.setCursor(0, 0);

    if (at_send_check_response("+AT: OK", 100, "AT\r\n"))
    {
        is_exist = true;
        at_send_check_response("+MODE: TEST", 1000, "AT+MODE=TEST\r\n");
        at_send_check_response("+TEST: RFCFG", 1000, "AT+TEST=RFCFG,866,SF12,125,12,15,14,ON,OFF,OFF\r\n");
        delay(200);
# ifdef NODE_SLAVE
        //u8x8.setCursor(5, 0);
        //u8x8.print("slave");
# else
        //u8x8.setCursor(5, 0);
        //u8x8.print("master");
# endif
    }
    else
    {
        is_exist = false;
        ShowSerial.print("No E5 module found.\r\n");
        //u8x8.setCursor(0, 1);
        //u8x8.print("unfound E5 !");
    }
}

void loop(void)
{
    if (is_exist)
    {
# ifdef NODE_SLAVE
        node_recv_then_send(2000);
# else
        node_send_then_recv(2000);
        delay(3000);
# endif
    }
}
