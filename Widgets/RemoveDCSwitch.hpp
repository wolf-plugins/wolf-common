#ifndef SPOONIE_REMOVE_DC_SWITCH_HPP_INCLUDED
#define SPOONIE_REMOVE_DC_SWITCH_HPP_INCLUDED

#include "NanoSwitch.hpp"
#include "Animation.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

class RemoveDCSwitch : public NanoSwitch,
                       public IdleCallback
{
  public:
    explicit RemoveDCSwitch(NanoWidget *widget, Size<uint> size) noexcept;

  protected:
    void draw() override;
    void idleCallback() override;
    void onClick() override;

  private:
    Color fSocketColor;
    Color fGlowIcol;
    Color fMainRectColor;

    ColorTransition fSocketColorTransition;
    ColorTransition fGlowIcolTransition;
    ColorTransition fMainRectColorTransition;

    DISTRHO_LEAK_DETECTOR(RemoveDCSwitch)
};

END_NAMESPACE_DISTRHO

#endif