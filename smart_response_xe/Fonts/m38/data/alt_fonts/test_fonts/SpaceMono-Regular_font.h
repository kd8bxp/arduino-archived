//Large & Normal font - are 3 sets of 3 pixels - SpaceMono-Regular_font (8 bit font) 
//Used the convert python script, but it didn't work correctly.
const unsigned char ucFont[] PROGMEM = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //  
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // !
	0x00,0x00,0x00,0x02,0x00,0x00,0x02,0x00, // "
	0x00,0x00,0x48,0xfe,0x48,0x48,0x48,0x48, // #
	0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00, // $
	0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00, // %
	0x00,0x0c,0x00,0x20,0x21,0x21,0x20,0x20, // &
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // '
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // (
	0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00, // )
	0x00,0x20,0x20,0x00,0x20,0x00,0x00,0x20, // *
	0x00,0x00,0x20,0x20,0x20,0x20,0x20,0x20, // +
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ,
	0x00,0x00,0x00,0x40,0x40,0x40,0x40,0x00, // -
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // .
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // /
	0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00, // 0
	0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00, // 1
	0x00,0x00,0x80,0x00,0x40,0x00,0x00,0x00, // 2
	0x00,0x00,0x01,0x01,0x11,0x01,0x01,0x21, // 3
	0x00,0x00,0x20,0x00,0x00,0x01,0xff,0x00, // 4
	0x00,0x00,0x21,0x01,0x01,0x01,0x01,0x01, // 5
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 6
	0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01, // 7
	0x00,0x00,0x00,0x00,0x10,0x10,0x00,0x00, // 8
	0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00, // 9
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // :
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ;
	0x00,0x00,0x10,0x00,0x00,0x08,0x00,0x00, // <
	0x00,0x00,0x50,0x50,0x50,0x50,0x50,0x50, // =
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // >
	0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00, // ?
	0x00,0x00,0x04,0x04,0x04,0x24,0x00,0x00, // @
	0x00,0x00,0x80,0x84,0x81,0x81,0x88,0xc0, // A
	0x00,0x01,0xff,0x21,0x21,0x20,0x00,0x00, // B
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // C
	0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00, // D
	0x00,0x00,0x21,0x21,0x21,0x21,0x21,0x21, // E
	0x00,0x00,0x21,0x21,0x21,0x21,0x21,0x21, // F
	0x00,0x70,0x00,0x00,0x40,0x40,0x40,0x40, // G
	0x00,0x00,0x20,0x20,0x20,0x20,0x20,0x20, // H
	0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01, // I
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xff, // J
	0x00,0x00,0x20,0x20,0x00,0x00,0x00,0x00, // K
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // L
	0x00,0x01,0x01,0x20,0x00,0x00,0x10,0x01, // M
	0x00,0x00,0x01,0x02,0x10,0x80,0x00,0x00, // N
	0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00, // O
	0x00,0x00,0x21,0x21,0x21,0x20,0x00,0x00, // P
	0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00, // Q
	0x00,0x00,0x21,0x21,0x21,0x20,0x20,0x40, // R
	0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00, // S
	0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01, // T
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // U
	0x00,0x02,0x10,0x80,0x00,0x00,0x80,0x08, // V
	0x00,0xc0,0x00,0x80,0x01,0x01,0x00,0x00, // W
	0x00,0x00,0x00,0x00,0x20,0x20,0x08,0x02, // X
	0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00, // Y
	0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01, // Z
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // [
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // "\"
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ]
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ^
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // _
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // `
	0x00,0xc0,0x00,0x00,0x00,0x00,0x00,0x20, // a
	0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00, // b
	0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00, // c
	0x00,0xc0,0x00,0x00,0x00,0x00,0x00,0x10, // d
	0x00,0x00,0x40,0x40,0x40,0x40,0x40,0x40, // e
	0x00,0x00,0x08,0x08,0xfe,0x09,0x09,0x09, // f
	0x00,0xc0,0x00,0x00,0x00,0x00,0x00,0x10, // g
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // h
	0x00,0x00,0x08,0x08,0x08,0x00,0x00,0x00, // i
	0x00,0x00,0x00,0x08,0x08,0x08,0x00,0x00, // j
	0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00, // k
	0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00, // l
	0x00,0xf8,0x00,0x00,0x00,0x00,0x00,0x00, // m
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // n
	0x00,0xc0,0x00,0x00,0x00,0x00,0x00,0x00, // o
	0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00, // p
	0x00,0xc0,0x00,0x00,0x00,0x00,0x00,0x10, // q
	0x00,0x08,0x08,0x00,0x00,0x00,0x00,0x00, // r
	0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00, // s
	0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x00, // t
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // u
	0x00,0x00,0x20,0x00,0x00,0x00,0x80,0x10, // v
	0x00,0x40,0x00,0x00,0x08,0x08,0x00,0x00, // w
	0x00,0x00,0x00,0x00,0x40,0x40,0x00,0x00, // x
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // y
	0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08, // z
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // |
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // }
	0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00, // ~
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

#define FONT_NAME "SpaceMono-Regular_font"