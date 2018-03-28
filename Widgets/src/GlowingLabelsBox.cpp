#include "GlowingLabelsBox.hpp"
#include "Mathf.hpp"

START_NAMESPACE_DISTRHO

GlowingLabelsBox::GlowingLabelsBox(NanoWidget *widget, Size<uint> size) noexcept : NanoWidget(widget),
                                                                                   fSelectedIndex(0)
{
    setSize(size);

    loadSharedResources();
}

void GlowingLabelsBox::onNanoDisplay()
{
    if (getLabelCount() == 0)
        return;

    const float width = getWidth();
    const float height = getHeight();
    const float verticalMargin = 6.0f;
    const float boxOutlineWidth = 2.0f;

    //Box background
    beginPath();

    fillColor(Color(17, 17, 17, 255));

    rect(0, 0, width, height);
    fill();

    closePath();

    const float labelSize = 14.0f;
    const float labelMarginLeft = 4.0f;
    const float labelMarginTop = 4.0f;
    const float labelVerticalSpacing = labelSize + 7.0f;

    //Text
    for (int i = 0; i < getLabelCount(); ++i)
    {
        beginPath();

        fontFace(NANOVG_DEJAVU_SANS_TTF);
        fontSize(labelSize);

        const Color fontColor = (i == fSelectedIndex) ? Color(235, 196, 74, 255) : Color(158, 158, 158, 255);

        fillColor(fontColor);
        textAlign(ALIGN_LEFT | ALIGN_TOP);

        text(labelMarginLeft, labelVerticalSpacing * i + labelMarginTop, fLabels[i], NULL);

        closePath();
    }
}

void GlowingLabelsBox::setFontSize(float fontSize)
{
    fFontSize = fontSize;
}

float GlowingLabelsBox::getFontSize()
{
    return fFontSize;
}

void GlowingLabelsBox::setSelectedIndex(int index)
{
    fSelectedIndex = spoonie::clamp(index, 0, getLabelCount() - 1);
}

int GlowingLabelsBox::getSelectedIndex()
{
    return fSelectedIndex;
}

int GlowingLabelsBox::getLabelCount()
{
    return fLabels.size();
}

void GlowingLabelsBox::setLabels(std::vector<const char *> labels)
{
    fLabels = labels;
}

std::vector<const char *> GlowingLabelsBox::getLabels()
{
    return fLabels;
}

void GlowingLabelsBox::setFontId(NanoVG::FontId fontId)
{
    fFontId = fontId;
}

NanoVG::FontId GlowingLabelsBox::getFontId()
{
    return fFontId;
}

END_NAMESPACE_DISTRHO