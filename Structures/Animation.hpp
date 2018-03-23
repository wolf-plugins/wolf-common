#ifndef WOLF_ANIMATION_DEFINED_H
#define WOLF_ANIMATION_DEFINED_H

#include "src/DistrhoDefines.h"
#include "Color.hpp"
#include "Geometry.hpp"
#include "Widget.hpp"
#include <chrono>

START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::Color;

enum EasingFunction
{
  noEasing, //linear
  easeOutPower2,
  easeOutPower4,
  easeInPower2,
  easeInPower4
};

class Animation
{
public:
  enum PlaybackDirection
  {
    Forward,
    Backward
  };

  enum RepeatMode
  {
    NoRepeat,
    PingPong //unsupported for now
  };

  Animation(float duration, EasingFunction easingFunction = noEasing);
  ~Animation();

  void play(PlaybackDirection playbackDirection = Forward, RepeatMode repeatMode = NoRepeat);
  void pause();
  void seek(float time);
  void rewind();

  bool isPlaying();

  float getCurrentTime();
  float getDuration();
  void setDuration(float duration);

protected:
  virtual void run() = 0;
  virtual void applyEasing();

  float fDuration; //in seconds
  float fCurrentTime;
  std::chrono::steady_clock::time_point fTimeLastRun;
  PlaybackDirection fPlaybackDirection;
  RepeatMode fRepeatMode;
  EasingFunction fEasingFunction;
  bool fIsPlaying;

private:
};

class SizeChangeAnimation : public Animation
{
public:
  SizeChangeAnimation(float duration, Size<uint> *sourceSize, Size<uint> targetSize, EasingFunction easingFunction = noEasing);
  ~SizeChangeAnimation();

  void run() override;

protected:
  void applyEasing() override;

  Size<uint> fInitialSize;
  Size<uint> *fCurrentSize;  
  Size<uint> fTargetSize;
};

END_NAMESPACE_DISTRHO

#endif