#include "NanoSlider.hpp"
#include "Mathf.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

SliderHandle::SliderHandle(NanoWidget *parent, Size<uint> size) noexcept : NanoWidget(parent)
{
    setSize(size);
}

void SliderHandle::onNanoDisplay()
{
    const float handleCenterY = getHeight() / 2.0f;

    beginPath();

    fillColor(Color(215, 215, 215, 255));
    strokeColor(Color(35, 35, 35, 255));
    strokeWidth(1.0f);

    rect(0, 0, getWidth(), getHeight());

    fill();
    stroke();

    closePath();

    beginPath();

    strokeColor(Color(0, 0, 0, 255));
    strokeWidth(1.0f);

    moveTo(0, handleCenterY);
    lineTo(getWidth(), handleCenterY);

    stroke();

    closePath();
}

NanoSlider::NanoSlider(NanoWidget *parent, Size<uint> size) noexcept
    : NanoWidget(parent),
      fLeftMouseDown(false),
      fLeftMouseDownLocation(Point<int>(0, 0)),
      fIsHovered(true),
      fValue(0),
      fMin(0),
      fMax(1),
      fSocketMarginBottom(0.0f),
      fSocketMarginTop(0.0f)
{
    setSize(size);

    fHandle = new SliderHandle(this, Size<uint>(16, 26));
}

void NanoSlider::setRange(float min, float max) noexcept
{
    fMin = min;
    fMax = max;

    fValue = wolf::clamp(fValue, min, max);

    positionHandle();
}

void NanoSlider::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void NanoSlider::setValue(float value, bool sendCallback) noexcept
{
    value = wolf::clamp(value, fMin, fMax);

    if (fValue == value)
        return;

    fValue = value;

    positionHandle();

    if (sendCallback && fCallback != nullptr)
        fCallback->nanoSliderValueChanged(this, fValue);

    repaint();
}

void NanoSlider::positionHandle()
{
    const float range = fMax - fMin;

    fHandle->setAbsoluteY((fValue - fMin) / range * getHeight() + getAbsoluteY() + fSocketMarginTop - fSocketMarginBottom - fHandle->getHeight() / 2.0f);
}

float NanoSlider::getValue() noexcept
{
    return fValue;
}

void NanoSlider::onNanoDisplay()
{
    const float posX = getWidth() / 2.0f - fHandle->getWidth() / 2.0f;
    fHandle->setAbsoluteX(getAbsoluteX() + posX);

    draw();
}

void NanoSlider::setHandleSize(const float width, const float height)
{
    fHandle->setSize(width, height);
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

            const float handleCenterX = fHandle->getAbsoluteX() + fHandle->getWidth() / 2.0f;
            const float handleCenterY = fHandle->getAbsoluteY() + fHandle->getHeight() / 2.0f;

            window.setCursorPos(handleCenterX, handleCenterY);
            window.showCursor();
            getParentWindow().setCursorStyle(Window::CursorStyle::UpDown);

            return true;
        }

        return false;
    }

    Point<int> adaptedPos = Point<int>(ev.pos.getX() + getAbsoluteX() - fHandle->getAbsoluteX(),
                                       ev.pos.getY() + getAbsoluteY() - fHandle->getAbsoluteY());

    if (fHandle->contains(adaptedPos))
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
        const float tension = 3.0f; //bigger value means slower handle movement

        const float value = -(fLeftMouseDownLocation.getY() - ev.pos.getY()) / tension;

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

            setValue(fValue + value, true);
        }

        return true;
    }

    Point<int> adaptedPos = Point<int>(ev.pos.getX() + getAbsoluteX() - fHandle->getAbsoluteX(),
                                       ev.pos.getY() + getAbsoluteY() - fHandle->getAbsoluteY());

    if (fHandle->contains(adaptedPos))
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