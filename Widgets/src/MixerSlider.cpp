#include "MixerSlider.hpp"

START_NAMESPACE_DISTRHO

MixerSlider::MixerSlider(NanoWidget *widget, Size<uint> size) noexcept : NanoSlider(widget, size)
{
}

void MixerSlider::draw()
{
    drawSocket();
}

void MixerSlider::drawSocket()
{
    const float width = getWidth();
    const float height = getHeight();

    const float centerX = getWidth() / 2.0f;
    const float socketWidth = 2.0f;
    const float halfSocketWidth = socketWidth / 2.0f;
    
    //outline
    beginPath();

    strokeWidth(0.5f);
    strokeColor(Color(71, 71, 71, 255));

    roundedRect(centerX - halfSocketWidth - 1.0f, fSocketMarginTop - 1.0f, socketWidth + 2.0f, height - fSocketMarginBottom - fSocketMarginTop + 1.0f, 2.0f);
    stroke();

    closePath();
    
    //empty socket
    beginPath();

    fillColor(Color(29,29,29,255));
    strokeWidth(1.0f);
    strokeColor(Color(19,19,19,255));

    roundedRect(centerX - halfSocketWidth, fSocketMarginTop, socketWidth, height - fSocketMarginBottom - fSocketMarginTop, 2.0f);
    stroke();
    fill();

    closePath();
}

END_NAMESPACE_DISTRHO