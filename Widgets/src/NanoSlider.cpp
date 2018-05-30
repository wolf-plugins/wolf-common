#include "NanoSlider.hpp"
#include "Mathf.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

NanoSlider::NanoSlider(NanoWidget *parent, Size<uint> size) noexcept
    : NanoWidget(parent),
      fLeftMouseDown(false),
      fLeftMouseDownLocation(Point<int>(0, 0)),
      fIsHovered(true),
      fHandle(0,0,18,30),
      fValue(0),
      fMin(0),
      fMax(0),
      fSocketMarginBottom(0.0f),
      fSocketMarginTop(0.0f)
{
    setSize(size);
}

void NanoSlider::setRange(int min, int max) noexcept
{
    fMin = min;
    fMax = max;

    fValue = wolf::clamp(fValue, min, max);
}

void NanoSlider::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void NanoSlider::setValue(int value, bool sendCallback) noexcept
{
    value = wolf::clamp(value, fMin, fMax);

    if (fValue == value)
        return;

    fValue = value;

    if (sendCallback && fCallback != nullptr)
        fCallback->nanoSliderValueChanged(this, fValue);

    repaint();
}

int NanoSlider::getValue() noexcept
{
    return fValue;
}

void NanoSlider::onNanoDisplay()
{
    fHandle.setPos(getWidth() / 2.0f - fHandle.getWidth() / 2.0f, fSocketMarginTop / 2.0f);

    draw();
}

void NanoSlider::setHandleSize(const float width, const float height)
{
    fHandle.setSize(width, height);
}

void NanoSlider::setSocketMargin(const float top, const float bottom)
{
    fSocketMarginBottom = bottom;
    fSocketMarginTop = top;
}

bool NanoSlider::onScroll(const ScrollEvent &ev)
{
    if (!contains(ev.pos))
        return false;

    setValue(getValue() + ev.delta.getY(), true);

    return true;
}

bool NanoSlider::onMouse(const MouseEvent &ev)
{
    if (ev.button != 1)
        return fLeftMouseDown;

    Window &window = getParentWindow();

    if (!ev.press)
    {
        if (fLeftMouseDown == true)
        {
            fLeftMouseDown = false;

            const float handleCenterX = getAbsoluteX() + fHandle.getX() + fHandle.getWidth() / 2.0f;
            const float handleCenterY = getAbsoluteY() + fHandle.getY() + fHandle.getHeight() / 2.0f;

            window.setCursorPos(handleCenterX, handleCenterY);
            window.showCursor();
            getParentWindow().setCursorStyle(Window::CursorStyle::UpDown);

            return true;
        }

        return false;
    }

    if (fHandle.contains(ev.pos))
    {
        fLeftMouseDownLocation = ev.pos;
        fLeftMouseDown = true;

        window.hideCursor();

        return true;
    }

    return false;
}

bool NanoSlider::onMotion(const MotionEvent &ev)
{
    if (fLeftMouseDown)
    {
        const float tension = 40.0f;

        const int value = (fLeftMouseDownLocation.getY() - ev.pos.getY()) / tension;

        if (value != 0)
        {
            fLeftMouseDownLocation.setY(ev.pos.getY());

            Window &window = getParentWindow();
            const uint windowHeight = window.getHeight();

            if (ev.pos.getY() + getAbsoluteY() >= windowHeight - 1)
            {
                window.setCursorPos(getAbsoluteX(), 2);
                fLeftMouseDownLocation.setY(-getAbsoluteY() + 2);
            }
            else if (ev.pos.getY() + getAbsoluteY() == 0)
            {
                window.setCursorPos(getAbsoluteX(), windowHeight - 2);
                fLeftMouseDownLocation.setY(windowHeight - getAbsoluteY() - 2);
            }
            else
            {
                fLeftMouseDownLocation.setY(ev.pos.getY());
            }

            setValue(fValue + wolf::clamp(value, -1, 1), true);
        }

        return true;
    }

    if (fHandle.contains(ev.pos))
    {
        if (!fIsHovered)
        {
            fIsHovered = true;
            getParentWindow().setCursorStyle(Window::CursorStyle::UpDown);
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