/*
  Sprite.cpp - 2D sprite buffer library for Arduino &amp; Wiring
  Copyright (c) 2006 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include &lt;stdlib.h&gt;
#include &lt;stdarg.h&gt;
//#include &lt;stdio.h&gt;

#include &quot;Sprite.h&quot;

void Sprite::init(uint8_t width, uint8_t height)
{
  _width = width &gt;= 8 ? 8 : width;
  _height = height &gt;= 8 ? 8 : height;

  // for now, do nothing if this allocation fails.  methods that require it
  // should silently fail if _buffer is null.
  _buffer = (uint8_t *) calloc(_height, 1);
}
  
Sprite::Sprite(uint8_t width, uint8_t height)
{
  init(width, height);
}

Sprite::Sprite(uint8_t width, uint8_t height, uint8_t row, ...)
{
  init(width, height);
  
  if (!_buffer) return;
  
  va_list ap;
  va_start(ap, row);

  int y = 0;
  
  for (y = 0; ; y++) {
    for (int x = 0; x &lt; width &amp;&amp; x &lt; 8; x++)
      write(x, y, (row &gt;&gt; (width - x - 1)) &amp; 0x01);
    
    if (y == height - 1)
      break;
      
    row = va_arg(ap, int); // using '...' promotes uint8_t to int
  }
  
  va_end(ap);
}

uint8_t Sprite::width() const
{
  return _width;
}

uint8_t Sprite::height() const
{
  return _height;
}

void Sprite::write(uint8_t x, uint8_t y, uint8_t value)
{
  if (!_buffer) return;
  
  // uint8_t's can't be negative, so don't test for negative x and y.
  if (x &gt;= _width || y &gt;= _height) return;
  
  // we need to bitwise-or the value of the other pixels in the byte with
  // the new value, masked and shifted into the proper bits.
  _buffer[y] = (_buffer[y] &amp; ~(0x01 &lt;&lt; x)) | ((value &amp; 0x01) &lt;&lt; x);
}

uint8_t Sprite::read(uint8_t x, uint8_t y) const
{
  if (!_buffer) return 0;
  
  // uint8_t's can't be negative, so don't test for negative x and y.
  if (x &gt;= _width || y &gt;= _height) return 0;
  
  return (_buffer[y] &gt;&gt; x) &amp; 0x01;
}
