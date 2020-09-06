# LoRa transceiver communication method

## LoRa modulation
LoRa modulation uses the following parameters.

| Parameter | Value |
|:---------:|:---:|
| Preamble length | 12 symbols (register value 8) |
| Header | Explicit |
|SYNC word |0x12 (PRIVATE) |
| Bandwidth | 7.8-20.8 kHz (default value 15.6 kHz) |
Spread rate | 6-12 (default value 9) |
| Coding rate | 5/8 |
|Payload CRC| None |

### LoRa packet format
The LoRa packet format is as follows.

Preamble | SYNC | Header | Payload |
|:-----------:|:----:|:-----:|:---------:|
| 00 (12 symbols) | 0x12 | Explicit header | (below) |

The packet is sent in Explicit header mode, and the Explicit header has payload length, payload coding rate,
The presence or absence of CRC of the payload (not used in this method) is entered. The header itself is sent with CRC at a coding rate of 4/8.
See the [Semtech SX1276/77/78/79](http://www.semtech.com/apps/product.php?pn=SX1276) data sheet for more information.

### Payload
There are two types of payloads: "plaintext payload" for sending plaintext and "AX.25 payload" for sending in AX.25 compliant packet format.

#### Plaintext payload
The plaintext payload is used for non-procedural message exchange between LoRa transceiver stations. The plaintext payload sends the plaintext entered by the user in the following format.

| Callsign | Delimiter | Message |
|:---------:|:-----:|:--------:|
| Own station call sign | :> | Message (arbitrary length) |

The own station callsign is the callsign specified by the application. The message contains plain text encoded in UTF-8.
When calling each station, enter the CQ character string in the message, and in the case of communication specifying the partner station, enter the partner station call sign.

#### AX.25 payload
When using LoRa transceiver as TNC (Terminal Node Controller) of [APRS](http://www.aprs.org/doc/APRS101.PDF), AX.25 payload is used.

APRS packet is sent to the payload. The payload has the following format based on [AX.25 Link Access Protocol for Amateur Packet Radio](https://www.tapr.org/pdf/AX25.2.2.pdf).

|Flag| Destination Address | Source Address| Digipeater Addresses|Control Field|Protocol Field| Information Field|FCS|Flag|
|:--:|:-------------------:|:-------------:|:----- --------------:|:-----------:|:------------:|:---- ------------:|:::::--:|
0x7e | Partner station AX.25 call sign | Own station AX.25 call sign | APRS digipeat address | 0x03 | 0xf0 | Arbitrary length data | Frame check sequence (2byte) |0x7e|

For more information on APRS, see [APRS PROTOCOL REFERENCE Protocol Version 1.0](http://www.aprs.org/doc/APRS101.PDF) and [APRS SPEC Addendum 1.1](http://www.aprs.org/aprs11 Please refer to .html).
