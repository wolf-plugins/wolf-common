#ifndef SPOONIE_VOLUME_KNOB_HPP_INCLUDED
#define SPOONIE_VOLUME_KNOB_HPP_INCLUDED

#include "NanoKnob.hpp"
#include "Animation.hpp"

START_NAMESPACE_DISTRHO

class VolumeKnob : public NanoKnob,
                   public IdleCallback
{
public:
  /* explicit VolumeKnob(Window &parent, Size<uint> size) noexcept; */
  explicit VolumeKnob(NanoWidget *widget, Size<uint> size) noexcept;

protected:
  void idleCallback() override;

  void onMouseHover() override;
  void onMouseLeave() override;
  void onMouseUp() override;
  void onMouseDown() override;

  void drawNormal() override;
  void drawHover() override;
  void drawTurning() override;

private:
  ScopedPointer<FloatTransition> fGrowAnimation;
  float fKnobDiameter;

  DISTRHO_LEAK_DETECTOR(VolumeKnob)
};

END_NAMESPACE_DISTRHO

#endif