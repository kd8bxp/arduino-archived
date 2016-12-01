/***************************************************************************
* ÒòÎª´Ë°æ±¾µÄIICÍ¨ÐÅÃ»ÓÐACKÐÅºÅ·µ»Ø£¬ËùÒÔÏÖ³ÉµÄ¹ØÓÚIICÍ¨ÐÅµÄ¿â²»ÄÜÓÃÓÚ±¾Æ·¡£
*
* µÚÒ»´ÎÊ¹ÓÃArduino£¬Ò²²»Ì«ÊìÏ¤C++£¬ËùÒÔÕâ¸ö¿âÔÚÊ¹ÓÃÆðÀ´¿ÉÄÜ²»ÈçAdafruit»ò
* U8glibµÄ¿âÄÇÑù·½±ã¡­¡­Àí½âÍòËê¡£±ÈÈçÉèÖÃ×ÖÌå´óÐ¡¡¢ÏÔÊ¾ÈÎÒâ³ß´çBMPÍ¼Æ¬µÈ¹¦ÄÜ£¬
* ºóÐø»áÍêÉÆ¹¦ÄÜµÄ£¡£¡£¡£¡
*
* ×îÐÂ°æ±¾µÄ¡°IIC_wtihout_ACK¡±»á·¢²¼ÔÚ http://www.14blog.com/archives/1358
* 
* ½¨ÒéºÍÎÊÌâ·´À¡£¬Çë·¢ÓÊ¼þÖÁ hello14blog@gmail.com
***************************************************************************/

#ifndef _IIC_without_ACK_H
#define _IIC_without_ACK_H

#if ARDUINO &gt;= 100
 #include &quot;Arduino.h&quot;
#else
 #include &quot;WProgram.h&quot;
#endif

#include &lt;Adafruit_GFX.h&gt;

#define BLACK 0
#define WHITE 1

class IIC_without_ACK
{
 public:
  IIC_without_ACK(int sda, int scl);
  
  void IIC_Start();
  void IIC_Stop();
  void Write_IIC_Byte(unsigned char IIC_Byte);
  void Write_IIC_Command(unsigned char IIC_Command);
  void Begin_IIC_Data();
  void IIC_SetPos(unsigned char x, unsigned char y);
  void Fill_Screen(unsigned char fill_Data);
  void Char_F6x8(unsigned char x, unsigned char y, const char ch[]);
//  void Char_F8x16(unsigned char x, unsigned char y,const char ch[]);
//  void CN_F16x16(unsigned char x, unsigned char y, unsigned char N);
  void Draw_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1,const char BMP[]);
  void Initial();
  
  private:
  int _sda, _scl;
};
#endif