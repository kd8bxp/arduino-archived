void setup()
{
}
void loop()
{
  int m = 32;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < m; j++) {
      if (i == 0) {
        led(15, (m - j), j, 0);
      }
      else if (i == 1) {
        led(15, 0, (m - j), j);
      }
      else {
        led(15, j, 0, (m - j));
      }
      delay(100);
    }
  }
}

void led(int pin, int r, int g, int b) {
  int bufBin[24];
  for (int i = 0; i < 3; i++) {
    int v = (i == 0) ? g : (i == 1) ? r : b;
    v = (v > 255) ? 255 : (v < 0) ? 0 : v;
    for (int j = 0; j < 8; j++) {
      bufBin[i * 8 + j] = (((v >> (7 - j)) & 1) == 1) ? 1 : 0;
    }
  }

  uint8_t          *p   = 4;
//                    pix, count, mask;
//  int32_t         num = numBytes;
  unsigned int bitmask = ( 1 << g_ADigitalPinMap[pin] );
// https://github.com/sandeepmistry/arduino-nRF5/blob/dc53980c8bac27898fca90d8ecb268e11111edc1/variants/BBCmicrobit/variant.cpp

  volatile unsigned int *reg = (unsigned int *) (0x50000000UL + 0x508);

// https://github.com/sandeepmistry/arduino-nRF5/blob/dc53980c8bac27898fca90d8ecb268e11111edc1/cores/nRF5/SDK/components/device/nrf51.h
// http://www.iot-programmer.com/index.php/books/27-micro-bit-iot-in-c/chapters-micro-bit-iot-in-c/47-micro-bit-iot-in-c-fast-memory-mapped-gpio?showall=1
// https://github.com/Microsoft/pxt-neopixel/blob/master/sendbuffer.asm

  asm volatile(
    // "cpsid i" ; disable irq

    //    b .start
    "b  L%=_start"                    "\n\t"
    // .nextbit:               ;            C0
    "L%=_nextbit:"                    "\n\t"          //;            C0
    //    str r1, [r3, #0]    ; pin := hi  C2
    "strb %[bitmask], [%[reg], #0]"   "\n\t"          //; pin := hi  C2
    //    tst r6, r0          ;            C3
    "tst %[mask], %[pix]"             "\n\t"//          ;            C3
    //    bne .islate         ;            C4
    "bne L%=_islate"                  "\n\t"          //;            C4
    //    str r1, [r2, #0]    ; pin := lo  C6
    "strb %[bitmask], [%[reg], #4]"   "\n\t"          //; pin := lo  C6
    // .islate:
    "L%=_islate:"                     "\n\t"
    //    lsrs r6, r6, #1     ; r6 >>= 1   C7
    "lsr %[mask], %[mask], #1"       "\n\t"          //; r6 >>= 1   C7
    //    bne .justbit        ;            C8
    "bne L%=_justbit"                 "\n\t"          //;            C8

    //    ; not just a bit - need new byte
    //    adds r4, #1         ; r4++       C9
    "add %[p], #1"                   "\n\t"          //; r4++       C9
    //    subs r5, #1         ; r5--       C10
    "sub %[num], #1"                 "\n\t"          //; r5--       C10
    //    bcc .stop           ; if (r5<0) goto .stop  C11
    "bcc L%=_stop"                    "\n\t"          //; if (r5<0) goto .stop  C11
    // .start:
    "L%=_start:"
    //    movs r6, #0x80      ; reset mask C12
    "movs %[mask], #0x80"             "\n\t"          //; reset mask C12
    //    nop                 ;            C13
    "nop"                             "\n\t"          //;            C13

    // .common:               ;             C13
    "L%=_common:"                     "\n\t"          //;            C13
    //    str r1, [r2, #0]   ; pin := lo   C15
    "strb %[bitmask], [%[reg], #4]"   "\n\t"          //; pin := lo  C15
    //    ; always re-load byte - it just fits with the cycles better this way
    //    ldrb r0, [r4, #0]  ; r0 := *r4   C17
    "ldrb  %[pix], [%[p], #0]"        "\n\t"          //; r0 := *r4   C17
    //    b .nextbit         ;             C20
    "b L%=_nextbit"                   "\n\t"          //;             C20

    // .justbit: ; C10
    "L%=_justbit:"                    "\n\t"          //; C10
    //    ; no nops, branch taken is already 3 cycles
    //    b .common ; C13
    "b L%=_common"                    "\n\t"          //; C13

    // .stop:
    "L%=_stop:"                       "\n\t"
    //    str r1, [r2, #0]   ; pin := lo
    "strb %[bitmask], [%[reg], #4]"   "\n\t"          //; pin := lo
    //    cpsie i            ; enable irq

    : [p] "+r" (p),
    [pix] "=&r" (pix),
    [count] "=&r" (count),
    [mask] "=&r" (mask),
    [num] "+r" (num)
    : [bitmask] "r" (bitmask),
    [reg] "r" (reg)
    );
}

