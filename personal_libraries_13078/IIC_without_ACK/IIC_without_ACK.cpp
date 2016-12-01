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

#if ARDUINO &gt;= 100
 #include &quot;Arduino.h&quot;
#else
 #include &quot;WProgram.h&quot;
#endif

#include &quot;IIC_without_ACK.h&quot;
#include &quot;oledfont.c&quot;

//¶¨ÒåÀà
IIC_without_ACK::IIC_without_ACK(int sda, int scl)
{
  _sda = sda;
  _scl = scl;
  pinMode(sda, OUTPUT);
  pinMode(scl, OUTPUT);
}


//¿ªÆôIIC×ÜÏß
void IIC_without_ACK::IIC_Start()
{
  digitalWrite(_scl, HIGH);
  digitalWrite(_sda, HIGH);
  digitalWrite(_sda, LOW);
  digitalWrite(_scl, LOW);
}


//Í£Ö¹IIC×ÜÏß
void IIC_without_ACK::IIC_Stop()
{
  digitalWrite(_scl, LOW);
  digitalWrite(_sda, LOW);
  digitalWrite(_scl, HIGH);
  digitalWrite(_sda, HIGH);
}


//Í¨¹ýIICÐ´Ò»¸ö8Î»µÄÊý¾Ý,±ÈÈç0xff
void IIC_without_ACK::Write_IIC_Byte(unsigned char IIC_Byte)
{
  unsigned char i;
  for(i=0;i&lt;8;i++)
  {
    if((IIC_Byte &lt;&lt; i) &amp; 0x80)digitalWrite(_sda, HIGH);
    else
      digitalWrite(_sda, LOW);
    digitalWrite(_scl, HIGH);
    digitalWrite(_scl, LOW);
//    IIC_Byte&lt;&lt;=1;
   }
  digitalWrite(_sda, HIGH);
  digitalWrite(_scl, HIGH);
  digitalWrite(_scl, LOW);
}


//IICÐ´ÃüÁî -- ÃüÁî²Ù×÷ºÜÉÙ£¬¶ÔËÙ¶ÈÓ°Ïì²»´ó
void IIC_without_ACK::Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);  //Slave address,SA0=0
   Write_IIC_Byte(0x00);	//write command
   Write_IIC_Byte(IIC_Command);
   IIC_Stop();
}


//¿ªÊ¼IICÐ´Êý¾Ý -- ÕâÑù¿ÉÒÔÈÃÒ»×éÊý¾Ý·¢ËÍÍê³ÉºóÔÙ¹Ø±ÕIIC£¬ÄÜºÜ´ó³Ì¶ÈÌáËÙ
void IIC_without_ACK::Begin_IIC_Data()
{
   IIC_Start();
   Write_IIC_Byte(0x78);
   Write_IIC_Byte(0x40);	//write data
}


//ÉèÖÃÆðÊ¼µã×ø±ê
void IIC_without_ACK::IIC_SetPos(unsigned char x, unsigned char y)
{
  IIC_Start();
  Write_IIC_Byte(0x78);  //Slave address,SA0=0
  Write_IIC_Byte(0x00);	//write command
  
  Write_IIC_Byte(0xb0+y);
  Write_IIC_Byte(((x&amp;0xf0)&gt;&gt;4)|0x10);//|0x10
  Write_IIC_Byte((x&amp;0x0f)|0x01);//|0x01
  
  IIC_Stop();//SetPosº¯Êý¾­³£±»Ê¹ÓÃ,ËùÒÔ²ÉÓÃÁËÕâÖÖ·¢ËÍÒ»×éÃüÁîÔÙ¹Ø±ÕIIC×ÜÏßµÄ·½Ê½
}


//È«ÆÁÏÔÊ¾ -- Fill_Screen(0x00)¿ÉÓÃ×÷ÇåÆÁ
void IIC_without_ACK::Fill_Screen(unsigned char fill_Data)
{
  unsigned char m,n;
  for(m=0;m&lt;8;m++)
  {
    Write_IIC_Command(0xb0+m);	//page0-page1
    Write_IIC_Command(0x00);		//low column start address
    Write_IIC_Command(0x10);		//high column start address
    Begin_IIC_Data();
    for(n=0;n&lt;128;n++)
    {
      Write_IIC_Byte(fill_Data);
    }
    IIC_Stop();
  }
}


//ÏÔÊ¾6x8µÄASCII×Ö·û
void IIC_without_ACK::Char_F6x8(unsigned char x, unsigned char y, const char ch[])
{
  unsigned char c,i,j=0;
  while(ch[j] != '\0')
  {
    c = ch[j] - 32;
    if(x&gt;126)
    {
      x=0;
      y++;
    }
    IIC_SetPos(x,y);
    Begin_IIC_Data();//SetPosº¯ÊýÓÐIIC_StopµÄ²Ù×÷
    for(i=0;i&lt;6;i++)
    {
      Write_IIC_Byte(font6x8[c*6+i]);
    }
    IIC_Stop();
    x += 6;
    j++;
  }
}


//ÏÔÊ¾8x16 ASCII×Ö·û -- Ê¹ÓÃ´Ëº¯ÊýÊ±£¬½«´Ë´¦ÒÔ¼°IIC_without_ACK.hÖÐÏàÓ¦µÄ×¢ÊÍ²¿·ÖÒÆ³ý¡£
/*
void IIC_without_ACK::Char_F8x16(unsigned char x, unsigned char y,const char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x&gt;120)
		{
      x=0;
      y++;
    }
		IIC_SetPos(x,y);
		Begin_IIC_Data();
		for(i=0;i&lt;8;i++)
		{
      Write_IIC_Byte(font8X16[c*16+i]);
    }
    IIC_Stop();
		IIC_SetPos(x,y+1);
		Begin_IIC_Data();
		for(i=0;i&lt;8;i++)
		{
      Write_IIC_Byte(font8X16[c*16+i+8]);
    }
    IIC_Stop();
		x+=8;
		j++;
	}
}*/


//ÏÔÊ¾16x16µÄÖÐÎÄ -- Ê¹ÓÃ´Ëº¯ÊýÊ±£¬½«´Ë´¦ÒÔ¼°IIC_without_ACK.hÖÐÏàÓ¦µÄ×¢ÊÍ²¿·ÖÒÆ³ý¡£
/*
void IIC_without_ACK::CN_F16x16(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	IIC_SetPos(x , y);
	Begin_IIC_Data();
	for(wm = 0;wm &lt; 16;wm++)
	{
		Write_IIC_Byte(CN16x16[adder]);
		adder += 1;
	}
	IIC_Stop();
	IIC_SetPos(x,y + 1);
	Begin_IIC_Data();
	for(wm = 0;wm &lt; 16;wm++)
	{
		Write_IIC_Byte(CN16x16[adder]);
		adder += 1;
	}
	IIC_Stop();
}
*/


//ÏÔÊ¾128x64µÄBMPÍ¼Æ¬ -- Ê¹ÓÃ´Ëº¯ÊýÊ±£¬½«´Ë´¦ÒÔ¼°IIC_without_ACK.hÖÐÏàÓ¦µÄ×¢ÊÍ²¿·ÖÒÆ³ý¡£
void IIC_without_ACK::Draw_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1,const char BMP[])
{
  unsigned int j=0;
  unsigned char x,y;
  
  if(y1%8==0)
    y=y1/8;
  else
    y=y1/8+1;
  for(y=y0;y&lt;y1;y++)
  {
    IIC_SetPos(x0,y);
    Begin_IIC_Data();
    for(x=x0;x&lt;x1;x++)
    {
      Write_IIC_Byte(BMP[j++]);
    }
    IIC_Stop();
  }
}


//SSD1306³õÊ¼»¯
void IIC_without_ACK::Initial()
{
  Write_IIC_Command(0xAE);//display off
  Write_IIC_Command(0x00);//Set Memory Addressing Mode
  Write_IIC_Command(0x10);//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
  Write_IIC_Command(0x40);//Set Page Start Address for Page Addressing Mode,0-7
  Write_IIC_Command(0x81);//Set COM Output Scan Direction
  Write_IIC_Command(0xCF);//---set low column address
  Write_IIC_Command(0xA1);//---set high column address
  Write_IIC_Command(0xC8);//--set start line address
  Write_IIC_Command(0xA6);//--set contrast control register
  Write_IIC_Command(0xA8);
  Write_IIC_Command(0x3F);//--set segment re-map 0 to 127
  Write_IIC_Command(0xD3);//--set normal display
  Write_IIC_Command(0x00);//--set multiplex ratio(1 to 64)
  Write_IIC_Command(0xD5);//
  Write_IIC_Command(0x80);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  Write_IIC_Command(0xD9);//-set display offset
  Write_IIC_Command(0xF1);//-not offset
  Write_IIC_Command(0xDA);//--set display clock divide ratio/oscillator frequency
  Write_IIC_Command(0x12);//--set divide ratio
  Write_IIC_Command(0xDB);//--set pre-charge period
  Write_IIC_Command(0x40);//
  Write_IIC_Command(0x20);//--set com pins hardware configuration
  Write_IIC_Command(0x02);
  Write_IIC_Command(0x8D);//--set vcomh
  Write_IIC_Command(0x14);//0x20,0.77xVcc
  Write_IIC_Command(0xA4);//--set DC-DC enable
  Write_IIC_Command(0xA6);//
  Write_IIC_Command(0xAF);//--turn on oled panel 
}