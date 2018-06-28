#include "MixerStrip.hpp"

START_NAMESPACE_DISTRHO

MixerStrip::MixerStrip(NanoWidget *widget, Size<uint> size) noexcept : NanoWidget(widget)
{
    setSize(size);
}

void MixerStrip::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();

    beginPath();

    fillColor(Color(53, 57, 61, 255));
    rect(0, 0, width, height);
    fill();

    closePath();
}

END_NAMESPACE_DISTRHO