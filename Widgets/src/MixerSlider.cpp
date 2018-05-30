#include "MixerSlider.hpp"

START_NAMESPACE_DISTRHO

MixerSlider::MixerSlider(NanoWidget *widget, Size<uint> size) noexcept : NanoSlider(widget, size)
{
}

void MixerSlider::draw()
{
    drawBackground();
    drawSocket();
    drawHandle();
}

void MixerSlider::drawBackground()
{
    const float width = getWidth();
    const float height = getHeight();

    beginPath();

    fillColor(Color(50,52,59, 255));
    rect(0, 0, width, height);
    fill();

    closePath();
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

    strokeWidth(2.0f);
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

    //filled part
    beginPath();

    strokeWidth(1.0f);
    fillColor(Color(51,52,52,255));

    roundedRect(centerX - halfSocketWidth, fSocketMarginTop, socketWidth, 100, 2.0f);
    fill();

    closePath();
}

void MixerSlider::drawHandle()
{
    const float width = getWidth();
    const float height = getHeight();
    const float handleCenterY = fHandle.getY() + fHandle.getHeight() / 2.0f;

    beginPath();

    fillColor(Color(215,215,215, 255));
    strokeColor(Color(35,35,35,255));
    strokeWidth(1.0f);

    rect(fHandle.getX(), fHandle.getY(), fHandle.getWidth(), fHandle.getHeight());

    fill();
    stroke();

    closePath();

    beginPath();

    strokeColor(Color(0,0,0,255));
    strokeWidth(1.0f);

    moveTo(fHandle.getX(), handleCenterY);
    lineTo(fHandle.getX() + fHandle.getWidth(), handleCenterY);

    stroke();

    closePath();    
}

END_NAMESPACE_DISTRHO