#include "Color.h"
#include <cmath>
using namespace std;

CColor::CColor(float fH, float fS, float fV)
  : m_fH(fH)
  , m_fS(fS)
  , m_fV(fV)
{
  HSVtoRGB();
}

void CColor::SetHSV(float fH, float fS, float fV)
{
  m_fH = fH;
  m_fS = fS;
  m_fV = fV;
  HSVtoRGB();
}

void CColor::HSVtoRGB()
{
  int i;
  float f, p, q, t;

  if(m_fS == 0)
  {
    // achromatic (grey)
    m_fR = m_fG = m_fB = m_fV;
    return;
  }

  float fH = m_fH / 60;			// sector 0 to 5
  i = static_cast<int>(floor(fH));
  f = fH - i;			// factorial part of m_fH
  p = m_fV * (1 - m_fS);
  q = m_fV * (1 - m_fS * f);
  t = m_fV * (1 - m_fS * (1 - f));

  switch(i)
  {
  case 0:
    m_fR = m_fV;
    m_fG = t;
    m_fB = p;
    break;
  case 1:
    m_fR = q;
    m_fG = m_fV;
    m_fB = p;
    break;
  case 2:
    m_fR = p;
    m_fG = m_fV;
    m_fB = t;
    break;
  case 3:
    m_fR = p;
    m_fG = q;
    m_fB = m_fV;
    break;
  case 4:
    m_fR = t;
    m_fG = p;
    m_fB = m_fV;
    break;
  default:		// case 5:
    m_fR = m_fV;
    m_fG = p;
    m_fB = q;
    break;
  }
}
