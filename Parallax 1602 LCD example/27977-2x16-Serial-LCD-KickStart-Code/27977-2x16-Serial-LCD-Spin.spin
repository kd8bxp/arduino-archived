OBJ

  LCD : "FullDuplexSerial.spin"

CON
  _clkmode = xtal1 + pll16x
  _xinfreq = 5_000_000
  TxPin = 0

PUB Main

  LCD.start(TxPin, TxPin, %1000, 9_600)
  WaitCnt(ClkFreq / 100 + Cnt)          ' Pause to initialize
  LCD.tx(12)                            ' Clear
  LCD.tx(17)                            ' Turn on backlight
  LCD.str(String("Hello, world..."))    ' First line
  LCD.tx(13)                            ' Line feed
  LCD.str(String("from Parallax!"))     ' Second line
  LCD.tx(212)                           ' Set quarter note
  LCD.tx(220)                           ' A tone
  WaitCnt(ClkFreq * 3 + Cnt)            ' Wait 3 seconds
  LCD.tx(18)                            ' Turn off backlight