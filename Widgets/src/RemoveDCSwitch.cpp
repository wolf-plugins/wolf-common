#include "RemoveDCSwitch.hpp"

START_NAMESPACE_DISTRHO

RemoveDCSwitch::RemoveDCSwitch(Window &parent, Size<uint> size) noexcept : NanoSwitch(parent, size)
{
}

RemoveDCSwitch::RemoveDCSwitch(NanoWidget *widget, Size<uint> size) noexcept : NanoSwitch(widget, size)
{
}

void RemoveDCSwitch::drawUp()
{
    beginPath();

    strokeWidth(2.0f);
    fillColor(Color(255, 0, 0, 255));
    strokeColor(Color(0, 0, 0, 255));

    rect(0, 0, getWidth(), getHeight());

    fill();
    stroke();

    closePath();
}

void RemoveDCSwitch::drawDown()
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
    fillPaint(boxGradient(mainRectTopLeft, mainRectTopLeft, mainRectWidth, mainRectHeight, 4.0f, 12.6f, Color(210, 123, 30, 255), Color(210, 123, 30, 0)));

    rect(0, 0, getWidth(), getHeight());

    fill();

    closePath();

    //socket
    beginPath();
    fillColor(Color(89, 56, 1, 255));

    rect(glowMargin, glowMargin, getWidth() - doubleGlowMargin, getHeight() - doubleGlowMargin);

    fill();

    closePath();

    //main rectangle
    beginPath();

    fillColor(Color(234, 151, 39, 255));
    rect(mainRectTopLeft, mainRectTopLeft, mainRectWidth, mainRectHeight);
    fill();

    closePath();

    //radial gradient at middle of main rectangle
    beginPath();
    fillPaint(radialGradient(mainRectCenter, mainRectCenter, 0.5f, mainRectHalfWidth, Color(254, 224, 191, 255), Color(240, 199, 154, 0)));

    rect(mainRectTopLeft, mainRectTopLeft, mainRectWidth, mainRectHeight);

    fill();

    closePath();

    //line at top
    beginPath();

    strokeColor(Color(255, 182, 82, 255));
    strokeWidth(2.0f);

    moveTo(mainRectTopLeft, mainRectTopLeft);
    lineTo(mainRectTopLeft + mainRectWidth, mainRectTopLeft);

    stroke();

    closePath();
}

END_NAMESPACE_DISTRHO