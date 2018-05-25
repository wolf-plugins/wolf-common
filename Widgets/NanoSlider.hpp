#ifndef WOLF_NANO_SLIDER_HPP_INCLUDED
#define WOLF_NANO_SLIDER_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class NanoSlider : public NanoWidget
{
  public:
    class Callback
    {
      public:
        virtual ~Callback() {}
        virtual void nanoSliderValueChanged(NanoSlider *nanoSlider, int value) = 0;
    };

    explicit NanoSlider(NanoWidget *widget, Size<uint> size) noexcept;

    void setValue(int value, bool sendCallback = false) noexcept;
    int getValue() noexcept;
    void setRange(int min, int max) noexcept;

    void setCallback(Callback *callback) noexcept;

  protected:
    void onNanoDisplay() override;

    bool onScroll(const ScrollEvent &ev) override;
    bool onMouse(const MouseEvent &) override;
    bool onMotion(const MotionEvent &) override;

    virtual void draw() = 0;

  private:
    Callback *fCallback;

    bool fLeftMouseDown;
    Point<int> fLeftMouseDownLocation;

    bool fIsHovered;

    int fValue;
    int fMin;
    int fMax;

    DISTRHO_LEAK_DETECTOR(NanoSlider)
};

END_NAMESPACE_DISTRHO

#endif