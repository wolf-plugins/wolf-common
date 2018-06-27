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

    const float halfWidth = static_cast<float>(getWidth()) / 2;
    const float redYellowHeight = static_cast<float>(getHeight()) * 0.2f;
    const float yellowBaseHeight = static_cast<float>(getHeight()) * 0.4f;
    const float baseBaseHeight = static_cast<float>(getHeight()) * 0.6f;

    // create gradients
    Paint fGradient1 = linearGradient(0.0f, 0.0f, 0.0f, redYellowHeight, kColorRed, kColorYellow);
    Paint fGradient2 = linearGradient(0.0f, redYellowHeight, 0.0f, yellowBaseHeight, kColorYellow, fColor);

    // paint left meter
    beginPath();
    rect(0.0f, 0.0f, halfWidth - 1.0f, redYellowHeight);
    fillPaint(fGradient1);
    fill();
    closePath();

    beginPath();
    rect(0.0f, redYellowHeight - 0.5f, halfWidth - 1.0f, yellowBaseHeight);
    fillPaint(fGradient2);
    fill();
    closePath();

    beginPath();
    rect(0.0f, redYellowHeight + yellowBaseHeight - 1.5f, halfWidth - 1.0f, baseBaseHeight);
    fillColor(fColor);
    fill();
    closePath();

    // paint left black matching output level
    beginPath();
    rect(0.0f, 0.0f, halfWidth - 1.0f, (1.0f - outLeft) * getHeight());
    fillColor(kColorBlack);
    fill();
    closePath();

    // paint right meter
    beginPath();
    rect(halfWidth + 1.0f, 0.0f, halfWidth - 2.0f, redYellowHeight);
    fillPaint(fGradient1);
    fill();
    closePath();

    beginPath();
    rect(halfWidth + 1.0f, redYellowHeight - 0.5f, halfWidth - 2.0f, yellowBaseHeight);
    fillPaint(fGradient2);
    fill();
    closePath();

    beginPath();
    rect(halfWidth + 1.0f, redYellowHeight + yellowBaseHeight - 1.5f, halfWidth - 2.0f, baseBaseHeight);
    fillColor(fColor);
    fill();
    closePath();

    // paint right black matching output level
    beginPath();
    rect(halfWidth + 1.0f, 0.0f, halfWidth - 2.0f, (1.0f - outRight) * getHeight());
    fillColor(kColorBlack);
    fill();
    closePath();
}

END_NAMESPACE_DISTRHO