#include "NanoMeter.hpp"

START_NAMESPACE_DISTRHO

NanoMeter::NanoMeter(NanoWidget *widget, Size<uint> size) noexcept : NanoWidget(widget),
                                                                     fOutLeft(0),
                                                                     fOutRight(0)
{
    setSize(size);

    fColor = Color(82, 238, 248);
}

void NanoMeter::setOutLeft(float value) noexcept
{
    value = (fOutLeft * kSmoothMultiplier + value) / (kSmoothMultiplier + 1.0f);

    if (value < 0.001f)
        value = 0.0f;
    else if (value > 0.999f)
        value = 1.0f;

    if (fOutLeft != value)
    {
        fOutLeft = value;
        repaint();
    }
}

void NanoMeter::setOutRight(float value) noexcept
{
    value = (fOutRight * kSmoothMultiplier + value) / (kSmoothMultiplier + 1.0f);

    if (value < 0.001f)
        value = 0.0f;
    else if (value > 0.999f)
        value = 1.0f;

    if (fOutRight != value)
    {
        fOutRight = value;
        repaint();
    }
}

void NanoMeter::onNanoDisplay()
{
    static const Color kColorBlack(0, 0, 0);
    static const Color kColorRed(255, 0, 0);
    static const Color kColorYellow(255, 255, 0);

    const float outLeft(fOutLeft);
    const float outRight(fOutRight);

    const float width = getWidth();
    const float height = getHeight();

    const float halfWidth = static_cast<float>(getWidth()) / 2;
    const float redYellowHeight = static_cast<float>(getHeight()) * 0.2f;
    const float yellowBaseHeight = static_cast<float>(getHeight()) * 0.4f;
    const float baseBaseHeight = static_cast<float>(getHeight()) * 0.6f;

    // outline and socket
    beginPath();

    fillColor(Color(16, 16, 16, 255));
    strokeColor(Color(62, 71, 72, 255));
    strokeWidth(1.0f);

    roundedRect(1, 1, width - 2.0f, height - 2.0f, 0.5f);
    fill();
    stroke();

    closePath();

    // glass
    beginPath();

    const Color glassTopColor = Color(46, 46, 46, 255);
    const Color glassBottomColor = Color(30, 30, 30, 255);
    
    const Color glassTopOutlineColor = Color(74, 74, 74, 255);
    const Color glassBottomOutlineColor = Color(74, 74, 74, 120);

    fillPaint(linearGradient(halfWidth, 0, halfWidth, height, glassTopColor, glassBottomColor));
    strokePaint(linearGradient(halfWidth, 0, halfWidth, height, glassTopOutlineColor, glassBottomOutlineColor));

    strokeWidth(1.0f);

    roundedRect(2.5f, 2.5f, width - 5.0f, height - 5.0f, 0.5f);
    fill();
    stroke();

    closePath();

    // glass reflection
    beginPath();

    fillColor(94, 94, 101, 27);

    const float reflectionRightHeight = 9.f;

    moveTo(2.5f, 2.5f);
    lineTo(width - 5.f, 2.5f);
    lineTo(width - 5.f, 2.5f + reflectionRightHeight);
    lineTo(2.5f, 2.5f + reflectionRightHeight * 2.0f);
    lineTo(2.5f, 2.5f);

    fill();

    closePath();
}

END_NAMESPACE_DISTRHO