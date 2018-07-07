#include "MixerStrip.hpp"
#include "nanovg.h"

START_NAMESPACE_DISTRHO

MixerStrip::MixerStrip(NanoWidget *widget, Size<uint> size) noexcept : NanoWidget(widget)
{
    setSize(size);

    fNoiseTexture = generateNoiseTexture();
}

NanoVG::Paint MixerStrip::generateNoiseTexture()
{
    NVGcontext *context = getContext();

    const int width = getWidth();
    const int height = getHeight();

    unsigned char *imageData = (unsigned char *)malloc(width * height * 4);

    int fileId = nvgCreateImageRGBA(context, width, height, 0, imageData);

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            int index = i * (width * 4) + (j * 4);

            int min = 0;
            int max = 8;
            int noiseFloor = 6;

            int random = min + (std::rand() % static_cast<int>(max - min + 1));

            if (random > noiseFloor)
            {
                imageData[index + 0] = 40;
                imageData[index + 1] = 43;
                imageData[index + 2] = 44;
                imageData[index + 3] = 255;
            }
            else
            {
                imageData[index + 0] = 35;
                imageData[index + 1] = 38;
                imageData[index + 2] = 41;
                imageData[index + 3] = 255;
            }
        }
    }

    nvgUpdateImage(context, fileId, imageData);

    return nvgImagePattern(context, 0, 0, width, height, 0, fileId, 1.0f);
}

void MixerStrip::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();

    beginPath();

    fillPaint(fNoiseTexture);
    rect(0, 0, width, height);
    fill();
    closePath();
}

END_NAMESPACE_DISTRHO