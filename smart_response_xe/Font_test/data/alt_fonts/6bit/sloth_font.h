//Small and Medium font -  2 sets of 3 pixels - 6 bit font - sloth_font (appears to be working) 
const unsigned char ucSmallFont[] PROGMEM = {
	0x00,0x00,0x00,0x00,0x00,0x00, //  
	0x5c,0x00,0x00,0x00,0x00,0x00, // !
	0x03,0x00,0x03,0x00,0x00,0x00, // "
	0x3e,0x14,0x14,0x3e,0x14,0x00, // #
	0x1c,0x54,0xff,0x54,0x74,0x04, // $
	0x26,0x16,0x08,0x34,0x32,0x00, // %
	0x1d,0x15,0x17,0x17,0x15,0x0c, // &
	0x03,0x00,0x00,0x00,0x00,0x00, // '
	0x7f,0x41,0x00,0x00,0x00,0x00, // (
	0x41,0x7f,0x00,0x00,0x00,0x00, // )
	0x02,0x01,0x00,0x00,0x00,0x00, // *
	0x08,0x1c,0x08,0x00,0x00,0x00, // +
	0x30,0x00,0x00,0x00,0x00,0x00, // ,
	0x08,0x08,0x08,0x00,0x00,0x00, // -
	0x10,0x00,0x00,0x00,0x00,0x00, // .
	0x40,0x20,0x10,0x08,0x04,0x02, // /
	0x1c,0x14,0x14,0x14,0x14,0x1c, // 0
	0x04,0x1c,0x00,0x00,0x00,0x00, // 1
	0x74,0x54,0x54,0x54,0x14,0x1c, // 2
	0x54,0x54,0x54,0x54,0x54,0x7c, // 3
	0x1c,0x10,0x10,0x10,0x38,0x10, // 4
	0x1c,0x14,0x54,0x54,0x54,0x74, // 5
	0x1f,0x15,0x15,0x15,0x14,0x1c, // 6
	0x04,0x04,0x24,0x74,0x2c,0x04, // 7
	0x70,0x5c,0x54,0x54,0x5c,0x70, // 8
	0x1c,0x14,0x54,0x54,0x54,0x7c, // 9
	0x14,0x00,0x00,0x00,0x00,0x00, // :
	0x34,0x00,0x00,0x00,0x00,0x00, // ;
	0x08,0x14,0x22,0x00,0x00,0x00, // <
	0x14,0x14,0x14,0x00,0x00,0x00, // =
	0x22,0x14,0x08,0x00,0x00,0x00, // >
	0x04,0x04,0xb4,0x14,0x14,0x7f, // ?
	0x5d,0x55,0x55,0x55,0x55,0x5f, // @
	0x3c,0x14,0x14,0x14,0x14,0x3c, // A
	0x1f,0x15,0x15,0x17,0x14,0x1c, // B
	0x1c,0x14,0x14,0x14,0x14,0x14, // C
	0x1e,0x1a,0x12,0x12,0x12,0x1e, // D
	0x1f,0x15,0x15,0x15,0x15,0x15, // E
	0x3c,0x14,0x14,0x14,0x14,0x14, // F
	0x1e,0x12,0x12,0x12,0x12,0x3a, // G
	0x1e,0x04,0x04,0x04,0x04,0x1e, // H
	0x1c,0x00,0x00,0x00,0x40,0x40, // I
	0x74,0x00,0x00,0x00,0x00,0x00, // J
	0x1c,0x08,0x08,0x0c,0x08,0x18, // K
	0x1c,0x10,0x10,0x10,0x10,0x10, // L
	0x1c,0x04,0x04,0x1c,0x04,0x1c, // M
	0x1c,0x04,0x08,0x08,0x10,0x1c, // N
	0x1c,0x14,0x14,0x14,0x14,0x1c, // O
	0x3c,0x14,0x14,0x14,0x14,0x1c, // P
	0x1c,0x14,0x14,0x14,0x14,0x3c, // Q
	0x7c,0x14,0x14,0x14,0x54,0x74, // R
	0x5c,0x54,0x54,0x54,0x50,0x70, // S
	0x04,0x04,0x1c,0x04,0x04,0x04, // T
	0x1c,0x10,0x10,0x10,0x10,0x1c, // U
	0x04,0x04,0x08,0x08,0x10,0x1c, // V
	0x1c,0x10,0x10,0x1c,0x10,0x1c, // W
	0x14,0x08,0x08,0x08,0x48,0x54, // X
	0x5c,0x50,0x50,0x50,0x50,0x7c, // Y
	0x44,0x64,0x54,0x54,0x4c,0x44, // Z
	0x7f,0x41,0x00,0x00,0x00,0x00, // [
	0x01,0x02,0x04,0x08,0x10,0x20, // "\"
	0x41,0x7f,0x00,0x00,0x00,0x00, // ]
	0x01,0x00,0x01,0x00,0x00,0x00, // ^
	0x20,0x20,0x20,0x20,0x20,0x20, // _
	0x00,0x01,0x00,0x00,0x00,0x00, // `
	0x1c,0x14,0x15,0x15,0x15,0x1f, // a
	0x1e,0x14,0x14,0x14,0x14,0x1c, // b
	0x1c,0x14,0x14,0x14,0x14,0x14, // c
	0x1c,0x14,0x14,0x14,0x14,0x1e, // d
	0x7c,0x54,0x54,0x54,0x14,0x1c, // e
	0x3c,0x14,0x14,0x14,0x04,0x04, // f
	0x1c,0x14,0x54,0x54,0x54,0x7c, // g
	0x1e,0x04,0x04,0x04,0x04,0x1c, // h
	0x1d,0x00,0x00,0x00,0x40,0x40, // i
	0x74,0x00,0x00,0x00,0x00,0x00, // j
	0x1c,0x08,0x08,0x0c,0x08,0x10, // k
	0x1c,0x10,0x10,0x10,0x10,0x10, // l
	0x1c,0x04,0x04,0x1c,0x04,0x1c, // m
	0x1c,0x04,0x04,0x04,0x04,0x1c, // n
	0x1c,0x14,0x14,0x14,0x14,0x1c, // o
	0x3c,0x14,0x14,0x14,0x14,0x1c, // p
	0x1c,0x14,0x14,0x14,0x14,0x3c, // q
	0x1c,0x04,0x04,0x04,0x04,0x04, // r
	0x1c,0x14,0x54,0x54,0x54,0x74, // s
	0x1e,0x14,0x14,0x10,0x10,0x18, // t
	0x1c,0x10,0x10,0x10,0x10,0x3c, // u
	0x04,0x04,0x08,0x08,0x10,0x1c, // v
	0x1c,0x10,0x10,0x1c,0x10,0x1c, // w
	0x14,0x08,0x08,0x08,0x08,0x14, // x
	0x1c,0x10,0x50,0x50,0x50,0x7c, // y
	0x24,0x24,0x24,0x34,0x2c,0x24, // z
	0x08,0x77,0x41,0x00,0x00,0x00, // {
	0xff,0x00,0x00,0x00,0x00,0x00, // |
	0x41,0x77,0x08,0x00,0x00,0x00, // }
	0x01,0x00,0x01,0x00,0x00,0x00, // ~
	0x00,0x00,0x00,0x00,0x00,0x00
};
