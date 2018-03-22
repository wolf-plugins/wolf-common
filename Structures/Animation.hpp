#ifndef WOLF_ANIMATION_DEFINED_H
#define WOLF_ANIMATION_DEFINED_H

#include "src/DistrhoDefines.h"
#include "Color.hpp"
#include "Geometry.hpp"
#include "Widget.hpp"

START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::Color;

enum EasingFunction
{
    noEasing,
    easeOutPower2,
    easeOutPower4,
    easeInPower2,
    easeInPower4
};

class Animation
{
  public:
    Animation(float duration, EasingFunction easingFunction = noEasing);
    void play();
    void pause();
    void rewind();
    bool isRunning();

  protected:
    virtual void run() = 0;
    virtual void applyEasing();

    float fDuration; //in seconds
    float fCurrentTime;

    EasingFunction fEasingFunction;
    bool fIsRunning;
  private:
};

class SizeChangeAnimation : Animation
{
  public:
    SizeChangeAnimation(float duration, Widget *widget, Size<uint> targetSize, EasingFunction easingFunction = noEasing);

  protected:
    void run() override;

    Widget *fWidget;
    Size<uint> fSourceSize;
    Size<uint> fTargetSize;
};

END_NAMESPACE_DISTRHO

#endif