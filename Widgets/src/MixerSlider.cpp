#include "MixerSlider.hpp"

START_NAMESPACE_DISTRHO

MixerSlider::MixerSlider(NanoWidget *widget, Size<uint> size) noexcept : NanoSlider(widget, size)
{
}

void MixerSlider::draw()
{
    const float width = getWidth();
    const float height = getHeight();

    beginPath();

    fillColor(Color(255,0,0,255));
    rect(0,0,width,height);
    fill(); 

    closePath();
}

END_NAMESPACE_DISTRHO