/* USB Host Shield 2.0 board quality control routine */
/* To see the output set your terminal speed to 115200 */
/* for GPIO test to pass you need to connect GPIN0 to GPOUT7, GPIN1 to GPOUT6, etc. */
/* otherwise press any key after getting GPIO error to complete the test */
/**/
#include "cdcacm.h"
#include "usbhub.h"

#include "pgmstrings.h"
// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

class ACMAsyncOper : public CDCAsyncOper
{
public:
    virtual uint8_t OnInit(ACM *pacm);
};

uint8_t ACMAsyncOper::OnInit(ACM *pacm)
{
    uint8_t rcode;
    // Set DTR = 1 RTS=1
    rcode = pacm->SetControlLineState(3);
		Serial.print("\r\n\r\nRCode: ");
		Serial.print(rcode,HEX);
		Serial.print("\r\n\r\n");

    if (rcode)
    {
        ErrorMessage<uint8_t>(PSTR("SetControlLineState"), rcode);
        return rcode;
    }

    LINE_CODING	lc;
    lc.dwDTERate	= 115200;
    lc.bCharFormat	= 0;
    lc.bParityType	= 0;
    lc.bDataBits	= 8;

    rcode = pacm->SetLineCoding(&lc);
		Serial.print("\r\n\r\nRCode: ");
		Serial.print(rcode,HEX);
		Serial.print("\r\n\r\n");

    if (rcode)
        ErrorMessage<uint8_t>(PSTR("SetLineCoding"), rcode);

    return rcode;
}

/* variables */
uint8_t rcode;
uint8_t usbstate;
uint8_t laststate;
//uint8_t buf[sizeof(USB_DEVICE_DESCRIPTOR)];
USB_DEVICE_DESCRIPTOR buf;

/* objects */
USB Usb;
//USBHub hub(&Usb);

ACMAsyncOper  AsyncOper;
ACM           Acm(&Usb, &AsyncOper);

void setup() {
        laststate = 0;
        Serial.begin(115200);
        while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
        E_Notify(PSTR("\r\nCircuits At Home 2011"), 0x80);
        E_Notify(PSTR("\r\nUSB Host Shield Quality Control Routine"), 0x80);
        /* SPI quick test - check revision register */
        E_Notify(PSTR("\r\nReading REVISION register... Die revision "), 0x80);
        Usb.Init(); // Initializes SPI, we don't care about the return value here
        {
                uint8_t tmpbyte = Usb.regRd(rREVISION);
                switch (tmpbyte) {
                        case( 0x01): //rev.01
                                E_Notify(PSTR("01"), 0x80);
                                break;
                        case( 0x12): //rev.02
                                E_Notify(PSTR("02"), 0x80);
                                break;
                        case( 0x13): //rev.03
                                E_Notify(PSTR("03"), 0x80);
                                break;
                        default:
                                E_Notify(PSTR("invalid. Value returned: "), 0x80);
                                print_hex(tmpbyte, 8);
                                halt55();
                                break;
                }//switch( tmpbyte...
        }//check revision register

        /* initializing USB stack */
        if (Usb.Init() == -1) {
                E_Notify(PSTR("\r\nOSCOKIRQ failed to assert"), 0x80);
                halt55();
        }
        E_Notify(PSTR("\r\nChecking USB device communication.\r\n"), 0x80);
        
}

uint8_t Running = 0;

void loop() {
if (! Running){
        delay(200);
        Usb.Task();

        usbstate = Usb.getUsbTaskState();
        if (usbstate != laststate) {
                laststate = usbstate;
                /**/
                switch (usbstate) {
                        case( USB_DETACHED_SUBSTATE_WAIT_FOR_DEVICE):
                                E_Notify(PSTR("\r\nWaiting for device..."), 0x80);
                                break;
                        case( USB_ATTACHED_SUBSTATE_RESET_DEVICE):
                                E_Notify(PSTR("\r\nDevice connected. Resetting..."), 0x80);
                                break;
                        case( USB_ATTACHED_SUBSTATE_WAIT_SOF):
                                E_Notify(PSTR("\r\nReset complete. Waiting for the first SOF..."), 0x80);
                                break;
                        case( USB_ATTACHED_SUBSTATE_GET_DEVICE_DESCRIPTOR_SIZE):
                                E_Notify(PSTR("\r\nSOF generation started. Enumerating device..."), 0x80);
                                break;
                        case( USB_STATE_ADDRESSING):
                                E_Notify(PSTR("\r\nSetting device address..."), 0x80);
                                break;
                        case( USB_STATE_RUNNING):
                        		Running=1;
                                E_Notify(PSTR("\r\nGetting device descriptor"), 0x80);
                                rcode = Usb.getDevDescr(1, 0, sizeof (USB_DEVICE_DESCRIPTOR), (uint8_t*) & buf);

                                if (rcode) {
                                        E_Notify(PSTR("\r\nError reading device descriptor. Error code "), 0x80);
                                        print_hex(rcode, 8);
                                } else {
                                        /**/
                                        E_Notify(PSTR("\r\nDescriptor Length:\t"), 0x80);
                                        print_hex(buf.bLength, 8);
                                        E_Notify(PSTR("\r\nDescriptor type:\t"), 0x80);
                                        print_hex(buf.bDescriptorType, 8);
                                        E_Notify(PSTR("\r\nUSB version:\t\t"), 0x80);
                                        print_hex(buf.bcdUSB, 16);
                                        E_Notify(PSTR("\r\nDevice class:\t\t"), 0x80);
                                        print_hex(buf.bDeviceClass, 8);
                                        E_Notify(PSTR("\r\nDevice Subclass:\t"), 0x80);
                                        print_hex(buf.bDeviceSubClass, 8);
                                        E_Notify(PSTR("\r\nDevice Protocol:\t"), 0x80);
                                        print_hex(buf.bDeviceProtocol, 8);
                                        E_Notify(PSTR("\r\nMax.packet size:\t"), 0x80);
                                        print_hex(buf.bMaxPacketSize0, 8);
                                        E_Notify(PSTR("\r\nVendor  ID:\t\t"), 0x80);
                                        print_hex(buf.idVendor, 16);
                                        E_Notify(PSTR("\r\nProduct ID:\t\t"), 0x80);
                                        print_hex(buf.idProduct, 16);
                                        E_Notify(PSTR("\r\nRevision ID:\t\t"), 0x80);
                                        print_hex(buf.bcdDevice, 16);
                                        E_Notify(PSTR("\r\nMfg.string index:\t"), 0x80);
                                        print_hex(buf.iManufacturer, 8);
                                        E_Notify(PSTR("\r\nProd.string index:\t"), 0x80);
                                        print_hex(buf.iProduct, 8);
                                        E_Notify(PSTR("\r\nSerial number index:\t"), 0x80);
                                        print_hex(buf.iSerialNumber, 8);
                                        E_Notify(PSTR("\r\nNumber of conf.:\t"), 0x80);
                                        print_hex(buf.bNumConfigurations, 8);
                                        /**/
                                        E_Notify(PSTR("\r\n\nAll tests passed. there we go !\r\n\r\n"), 0x80);
 //                                       while (1);
                                 }
                                break;
                        case( USB_STATE_ERROR):
                                E_Notify(PSTR("\r\nUSB state machine reached error state"), 0x80);
                                break;

                        default:
                                break;
                }//switch( usbstate...
        }
		Serial.print(" ");
		Serial.print(usbstate,HEX);
		Serial.print(" ");        
	}
else
	{
/*		delay(1000);
		Serial.print(" ");
		Serial.print(usbstate,HEX);
		Serial.print(" ");
		if( Acm.isReady())
			{
				Serial.print("Ready ! ");
			}
		else
			{
				Serial.print("Not ready ! ");
			}
*/
	} 
}//loop()...

/* constantly transmits 0x55 via SPI to aid probing */
void halt55() {

        E_Notify(PSTR("\r\nUnrecoverable error - test halted!!"), 0x80);
        E_Notify(PSTR("\r\n0x55 pattern is transmitted via SPI"), 0x80);
        E_Notify(PSTR("\r\nPress RESET to restart test"), 0x80);

        while (1) {
                Usb.regWr(0x55, 0x55);
        }
}

/* prints hex numbers with leading zeroes */
void print_hex(int v, int num_places) {
        int mask = 0, n, num_nibbles, digit;

        for (n = 1; n <= num_places; n++) {
                mask = (mask << 1) | 0x0001;
        }
        v = v & mask; // truncate v to specified number of places

        num_nibbles = num_places / 4;
        if ((num_places % 4) != 0) {
                ++num_nibbles;
        }
        do {
                digit = ((v >> (num_nibbles - 1) * 4)) & 0x0f;
                Serial.print(digit, HEX);
        } while (--num_nibbles);
}

/* prints "Press any key" and returns when key is pressed */
void press_any_key() {
        E_Notify(PSTR("\r\nPress any key to continue..."), 0x80);
        while (Serial.available() <= 0); //wait for input
        Serial.read(); //empty input buffer
        return;
}
