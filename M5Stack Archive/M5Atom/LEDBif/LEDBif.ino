#include "src/M5Atom/M5Atom.h"

uint8_t DisBuff[2 + 5 * 5 * 3];

void setBuff(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata)
{
    DisBuff[0] = 0x05;
    DisBuff[1] = 0x05;
    for (int i = 0; i < 25; i++)
    {
        DisBuff[2 + i * 3 + 0] = Rdata;
        DisBuff[2 + i * 3 + 1] = Gdata;
        DisBuff[2 + i * 3 + 2] = Bdata;
    }
}

// "Double Scroll" circuit which shows chaotic behavior

class DoubleScroll
{
public:
  DoubleScroll(float c1);
  virtual ~DoubleScroll();
  void update(float &vc1, float &iL, float &vc2);
private:
  float g(float vc);
  float m_vc1, m_vc2, m_iL;
  float m_c1;
  const float m_c2 = 1.0f;
  const float m_L = 1.0f/7;
  const float m_G = 0.7f;
  const float m_dt = 0.001f;
};

DoubleScroll::DoubleScroll(float c1) : m_c1(c1)
{
  m_vc1 = 0.0f;
  m_vc2 = 1.0f;
  m_iL = 0.0f;
}

DoubleScroll::~DoubleScroll() {}

// non-linear condactance
float
DoubleScroll::g(float vc)
{
  if (vc < 1.0f && -1.0f < vc)
    return vc * -0.8f;
  else if (vc > 0)
    return -0.8f -0.5f*(vc - 1.0f);
  else
    return 0.8f + 0.5f*(-vc - 1.0f);
}

void
DoubleScroll::update(float &vc1, float &iL, float &vc2)
{
  float dvc1 = m_dt*(1/m_c1)*(m_G*(m_vc2 - m_vc1) - g(m_vc1));
  float dvc2 = m_dt*(1/m_c2)*(m_G*(m_vc1 - m_vc2) + m_iL);
  float diL = m_dt*(1/m_L)* (-m_vc2);
  m_vc1 += dvc1;
  m_vc2 += dvc2;
  m_iL += diL;
  vc1 = m_vc1;
  iL = m_iL;
  vc2 = m_vc2;
}
 
DoubleScroll ds(1/9.0);
int count = 0;

void setup() {
  M5.begin(true, false, true);
  delay(10);
  setBuff(0xff, 0x00, 0x00);
  M5.dis.displaybuff(DisBuff);
}

void loop() {
  float x, y, z;
  ds.update(y, x, z);
  if (++count > 10*1000) {
    // Convert point on orbit to RGB values
    
    uint8_t ix = 127 + (127/2.5)*x;
    uint8_t iy = 127 + (127/2.5)*y;
    uint8_t iz = 127 + (127/2.5)*z;
    //Serial.printf("%3d %3d %3d\r\n", ix, iy, iz);
    setBuff(ix, iy, iz);
    M5.dis.displaybuff(DisBuff);
  }
  M5.update();
  delay(1);
}
