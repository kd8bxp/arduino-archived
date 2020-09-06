# LoRaKissTNC

This is a sketch that realizes the following functions with the 430MHz LoRa module. *1

1. KISS mode TNC
1. Text chat

In "KISS mode TNC" mode, the LoRa module operates as a terminal node controller in KISS mode. Packets sent in AX25 format from the serial line are sent with LoRa modulation at the specified frequency. *2

In "text chat" mode, the plain text of the text message sent from the terminal serially connected to the LoRa module is transmitted by LoRa modulation at the specified frequency.

*1 This is based on [Arduino LoRa](https://github.com/sandeepmistry/arduino-LoRa/blob/master/README.md) with LoRa's Channel Activity Detection function added.

*2 Divert KISS mode TNC from [APRS on LoRa](https://github.com/josefmtd/lora-aprs).

## Communication method
Please refer to [Protocol](https://github.com/w-ockham/LoRaKissTNC/blob/master/Protocol.md) for detailed communication method.

## hardware

 * Works with boards using [Semtech SX1276/77/78/79](http://www.semtech.com/apps/product.php?pn=SX1276). :
   * [BSFrance LoRa32u4](https://bsfrance.fr/lora-long-range/1311-BSFrance-LoRa32u4-1KM-Long-Range-Board-Based-Atmega32u4-433MHz-LoRA-RA02-Module.html)

### Semtech SX1276/77/78/79

| Semtech SX1276/77/78/79 | Arduino |
| :---------------------: | :------:|
| VCC | 3.3V |
| GND | GND |
| SCK | SCK |
| MISO | MISO |
| MOSI | MOSI |
| NSS | 8 |
| NRESET | 4 |
| DIO0 | 7 |


`NSS`,`NRESET`,`DIO0` can be connected with `LoRa.setPins(csPin, resetPin, irqPin)`. Change the pin connection depending on the board.

## How to install
### Add LoRa32u4 board to Arduino IDE
1. In File → Preferences, add the following to the URL of the additional board manager
   `https://adafruit.github.io/arduino-board-index/package_adafruit_index.json`
2. Select "Ada Fruit Feather32u4" from Tools → Board.
3. Select "Arduino ISP" from Tools → Writing device

### Compile and write
1. Open LoRaKissTNC.ino
2. Compile with Sketch → Verify/Compile
3. Write to Arduino by sketching → writing to the microcomputer board

### Connection with smartphone
1. Install [USB serial terminal](https://play.google.com/store/apps/details?id=jp.sugnakys.usbserialconsole&hl=ja) on your smartphone.
2. Connect the LoRa32u4 to your smartphone using [OTG cable] (https://www.amazon.co.jp/dp/B012V56C8K) and microUSB cable
3. In the serial port settings
  Baud rate 9600 Data bits 8 Parity none Stop bit 1 Flow control off
  choose
4. Display the send form and select the line feed code CR+LF in the connection settings

## KISS mode TNC mode
### APRS client settings
Use [APRSDroid](https://aprsdroid.org/) on the Android device as the APRS client. First, set the call sign on the APR SDroid setting screen. Then connect the LoRa transceiver with an OTG cable and set the following on the setting screen.

1. Select connection method TNC(KISS)
2. TNC initialization setting In the initialization setting character string, set the URL encoding character of the TNC initialization character string.
```
%0DKISS%2043851%2C3%2C8%2C8%2C10000%0D
```
This is the following TNC initialization string URL-encoded (`%0D=newline %20=space %2C=,`).
```
 (Line feed) KISS 43851,3,8,8,10000 (Line feed)
```
The details of the initialization string will be described later.

3. Set the connection type USB serial.
4. Set the device communication speed to 9600bps.

that's all.

When ARPSDroid is started, AX.25 format packet is sent to TNC. It will be digipete from the LoRa compatible i-gate station.

### About TNC initialization string
The KISS command puts the LoRa module into KISS TNC mode.
You can specify the parameters of the module as follows using the initialization string.
```
  KISS <carrier frequency (10 kHz unit)>, <band>, <spreading rate>, <coding rate>, <maximum backoff time (ms)>
  ```
For details on each parameter, refer to the command in "text chat" mode.
#### Carrier frequency
 Specify in units of 10kHz from 438.0MHz to 439MHz. If the frequency out of range is specified, it cannot be transmitted.
#### Band
Specify the following BW value. The default value is 15.6kHz.

LoRa's radio wave model is F1D. In the 430MHz band, the occupied bandwidth will be 30kHz or less based on the Ministry of Internal Affairs and Communications Notification No. 125`'Allowed value of occupied frequency bandwidth of radio equipment of amateur station based on the provision of Annex 2 of Radio Equipment Regulations No. 54'. be careful.

| BW value | Bandwidth |
|:-----|:-------|
|0 | 7.8kHz |
|1 | 10.4kHz |
|2 | 15.6kHz (default value) |
|3 | 20.8kHz |

#### Spread rate
Spread rate can be specified in the range of `7(2^7=128) to 12(2^12=4096)`.
#### Coding rate
You can specify the coding rate in the range of `5(4/5) to 8(4/8)`.
#### Maximum backoff time
This program monitors the activity of the channel for a predetermined time (3 seconds) before transmission, and when the other station is not transmitting, it transmits from its own station.
When a collision occurs, the waiting time is randomly entered within the range of 3 seconds to the time specified here. If the spreading factor is high, collisions are likely to occur, so set a backoff time as long as the frame transmission time (BW=62.5, SF=111, CR=8, or about 10000ms).

### Patch to APRS gateway
LoRaKissTNC returns RSSI/SNR of received packet by KISS protocol. Prepared a patch for [APRX](https://github.com/PhirePhly/aprx/blob/master/README) so that it can be displayed on the APRS gateway side.
After cloning from git (the patch is based on v2.9.0), apply the patch with the patch command.

```
2020-01-14 16:41:53.564 JL1NIE-6 R(-55,11.50) JL1NIE-5>APDR15,WIDE1-1::JL1NIE-10:dx{23
```

RSSI/SNR is entered in the R flag indicating the received packet of aprf-rf.log in the format of `R(RSSI,SNR)`.
We also prepared a script that creates a message packet for beacon from the log.
Since the position of your own station is embedded in the script, please correct it appropriately.

```
#!/bin/sh
tac /var/log/aprx/aprx-rf.log | grep "R(" | head -n 1 | gawk'{match($2,/(.+)\..+/,t);match($4, /R\((.+)\,(.+)\)/,a);match($5,/([A-Z0-9\-]+)>/,c);printf("!3536.15N /13931.24E-LoRa station:%s %s RSSI=%s SNR=%s\n",t[1],c[1],a[1],a[2]);}'
```

### APRS gateway settings
For reference, here is an example of aprx.conf.
Please set it after considering the influence on other stations such as load on APRS network.
```
mycall JS1YFC-6 # Your station call sign
myloc lat 3536.05N lon 13931.22E # Own station position

<aprsis>
        passcode 12345 #APRS-IS password
        server rotate.aprs2.net
</aprsis>

<logging>
        pidfile /var/run/aprx.pid
        rflog /var/log/aprx/aprx-rf.log
        aprxlog /var/log/aprx/aprx.log
</logging>

<interface>
        serial-device /dev/ttyACM0 9600 8n1 KISS
        # TNC initial string
        # Freq=438.51MHz BW=15.6kHz SF=8 CR=8 Backofftime=10000ms
        # <FEND><RET><FEND><CR>SET KISS 43851,3,8,8,10000<CR>
        initstring "\xc0\xff\xc0\x0dSET KISS 43851,3,8,8,10000\x0d"
        callsign $mycall
        tx-ok true
</interface>

<beacon>
        beaconmode aprsis
        cycle-size 5m
        #Display beacon of last receiving station
        beacon exec /usr/local/bin/aprxLastseen.sh
</beacon>

<digipeater>
        transmitter $mycall
        <source>
                source APRSIS
                relay-type third-party
                ratelimit 240 480
                via-
