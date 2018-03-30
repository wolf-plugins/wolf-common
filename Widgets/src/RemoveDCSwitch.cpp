#include "RemoveDCSwitch.hpp"

START_NAMESPACE_DISTRHO

RemoveDCSwitch::RemoveDCSwitch(NanoWidget *widget, Size<uint> size) noexcept : NanoSwitch(widget, size),
                                                                               fSocketColor(27, 27, 27, 255),
                                                                               fSocketColorTransition(0.500f, &fSocketColor, Color(59, 36, 27, 255)),
                                                                               fGlowIcol(Color(210, 123, 30, 0)),
                                                                               fGlowIcolTransition(0.500f, &fGlowIcol, Color(210, 123, 30, 125)),
                                                                               fMainRectColor(Color(82, 82, 82, 255)),
                                                                               fMainRectColorTransition(0.500f, &fMainRectColor, Color(234, 151, 39, 255))

{
    widget->getParentWindow().addIdleCallback(this);
}

void RemoveDCSwitch::idleCallback()
{
    bool mustRepaint = false;

    if (fSocketColorTransition.isPlaying())
    {
        fSocketColorTransition.run();
        mustRepaint = true;
    }

    if (fGlowIcolTransition.isPlaying())
    {
        fGlowIcolTransition.run();
        mustRepaint = true;
    }

    if (fMainRectColorTransition.isPlaying())
    {
        fMainRectColorTransition.run();
        mustRepaint = true;
    }

    if (mustRepaint)
        repaint();
}

void RemoveDCSwitch::onClick()
{
    fSocketColorTransition.play(isDown() ? Animation::Forward : Animation::Backward);
    fGlowIcolTransition.play(isDown() ? Animation::Forward : Animation::Backward);
    fMainRectColorTransition.play(isDown() ? Animation::Forward : Animation::Backward);
}

void RemoveDCSwitch::draw()
{
    const float socketMargin = 2.0f; //how much we can see the socket behind the main square
    const float doubleSocketMargin = socketMargin * 2.0f;

    const float glowMargin = 3.0f; //the space around the square we're keeping for glow
    const float doubleGlowMargin = glowMargin * 2.0f;

    const float mainRectTopLeft = glowMargin + socketMargin;
    const float mainRectWidth = getWidth() - doubleGlowMargin - doubleSocketMargin;
    const float mainRectHeight = getHeight() - doubleGlowMargin - doubleSocketMargin;
    const float mainRectHalfWidth = mainRectWidth / 2.0f;
    const float mainRectCenter = mainRectTopLeft + mainRectHalfWidth;

    //glow
    beginPath();

    fillPaint(boxGradient(mainRectTopLeft, mainRectTopLeft, mainRectWidth, mainRectHeight, 4.0f, 12.6f, fGlowIcol, Color(210, 123, 30, 0)));

    roundedRect(0, 0, getWidth(), getHeight(), 6.0f);
    fill();

    closePath();

    //socket
    beginPath();

    fillColor(fSocketColor);

    roundedRect(glowMargin, glowMargin, getWidth() - doubleGlowMargin, getHeight() - doubleGlowMargin, 4.0f);

    fill();

    closePath();

    //main rectangle
    beginPath();

    fillColor(fMainRectColor);

    roundedRect(mainRectTopLeft, mainRectTopLeft, mainRectWidth, mainRectHeight, 2.0f);
    fill();

    closePath();

    //radial gradient at middle of main rectangle
    beginPath();

    if (isDown())
        fillPaint(radialGradient(mainRectCenter, mainRectCenter, 0.5f, mainRectHalfWidth, Color(254, 224, 191, 255), Color(240, 199, 154, 0)));
    else
        fillPaint(radialGradient(mainRectCenter, mainRectCenter, 0.5f, mainRectHalfWidth, Color(113, 113, 113, 255), Color(73, 73, 73, 255)));

    rect(mainRectTopLeft, mainRectTopLeft, mainRectWidth, mainRectHeight);

    fill();

    closePath();

    if (isDown())
    {
        //line at top
        beginPath();

        strokeColor(Color(255, 182, 82, 255));
        strokeWidth(1.0f);

        moveTo(mainRectTopLeft, mainRectTopLeft + 1.0f);
        lineTo(mainRectTopLeft + mainRectWidth, mainRectTopLeft + 1.0f);

        stroke();

        closePath();
    }
}

END_NAMESPACE_DISTRHO