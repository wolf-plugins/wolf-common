#ifndef SPOONIE_NANO_SWITCH_HPP_INCLUDED
#define SPOONIE_NANO_SWITCH_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class NanoSwitch : public NanoWidget
{
  public:
    class Callback
    {
      public:
        virtual ~Callback() {}
        virtual void nanoSwitchClicked(NanoSwitch *nanoSwitch) = 0;
    };

    explicit NanoSwitch(Window &parent, Size<uint> size) noexcept;
    explicit NanoSwitch(NanoWidget *widget, Size<uint> size) noexcept;

    bool isDown() const noexcept;
    void setDown(bool down, bool sendCallback = false) noexcept;

    void setCallback(Callback *callback) noexcept;

  protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent &) override;
    bool onMotion(const MotionEvent &ev) override;

    virtual void draw() = 0;

    virtual void onClick();

  private:
    bool fIsDown;
    bool fIsHovered;
    
    Callback *fCallback;

    DISTRHO_LEAK_DETECTOR(NanoSwitch)
};

END_NAMESPACE_DISTRHO

#endif