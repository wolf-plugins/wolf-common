#ifndef WOLF_PARAM_SMOOTH_INCLUDED
#define WOLF_PARAM_SMOOTH_INCLUDED

#include "src/DistrhoDefines.h"
#include "extra/LeakDetector.hpp"

START_NAMESPACE_DISTRHO

class ParamSmooth
{
public:
  ParamSmooth();
  ParamSmooth(float value);

  float getSmoothedValue(float frequency, double sampleRate);
  float getRawValue() const;

  void setValue(float value);

private:
  float fHistory;
  float fValue;

  DISTRHO_LEAK_DETECTOR(ParamSmooth)
};

END_NAMESPACE_DISTRHO

#endif