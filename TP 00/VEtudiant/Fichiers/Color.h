#pragma once
class CColor
{
  float m_fH;
  float m_fS;
  float m_fV;
  float m_fR;
  float m_fG;
  float m_fB;

  void HSVtoRGB();

public:
  CColor(float fH = 0.0f, float fS = 0.0f, float fV = 0.0f);
  void SetHSV(float fH, float fS, float fV);
  float GetH() const { return m_fH; }
  float GetS() const { return m_fS; }
  float GetV() const { return m_fV; }
  float GetR() const { return m_fR; }
  float GetG() const { return m_fG; }
  float GetB() const { return m_fB; }
};

