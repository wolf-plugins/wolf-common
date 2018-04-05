#include "NanoSwitch.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

NanoSwitch::NanoSwitch(NanoWidget *widget, Size<uint> size) noexcept
    : NanoWidget(widget),
      fIsDown(false),
      fCallback(nullptr),
      fIsHovered(false)
{
    setSize(size);
}

bool NanoSwitch::isDown() const noexcept
{
    return fIsDown;
}

void NanoSwitch::setDown(bool down, bool sendCallback) noexcept
{
    fIsDown = down;

    onClick();

    if (sendCallback && fCallback != nullptr)
        fCallback->nanoSwitchClicked(this);
}

void NanoSwitch::onNanoDisplay()
{
    draw();
}

void NanoSwitch::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void NanoSwitch::onClick()
{
}

bool NanoSwitch::onMouse(const MouseEvent &ev)
{
    if (ev.press && contains(ev.pos))
    {
        setDown(!fIsDown, true);

        return true;
    }

    return false;
}

bool NanoSwitch::onMotion(const MotionEvent &ev)
{
    if (contains(ev.pos))
    {
        if (!fIsHovered)
        {
            fIsHovered = true;
            getParentWindow().setCursorStyle(Window::CursorStyle::Pointer);
        }

        return true;
    }
    else if (fIsHovered)
    {
        fIsHovered = false;
        getParentWindow().setCursorStyle(Window::CursorStyle::Default);
    }

    return false;
}

END_NAMESPACE_DISTRHO
