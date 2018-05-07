#ifndef WOLF_PARAM_SMOOTH_INCLUDED
#define WOLF_PARAM_SMOOTH_INCLUDED

#include "src/DistrhoDefines.h"
#include "extra/LeakDetector.hpp"

START_NAMESPACE_DISTRHO

class ParamSmooth
{
  public:
    ParamSmooth(); 

  private:
    float fHistory;

    DISTRHO_LEAK_DETECTOR(ParamSmooth)
};

END_NAMESPACE_DISTRHO

#endif