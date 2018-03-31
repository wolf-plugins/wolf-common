#include "NanoLabel.hpp"
#include "Mathf.hpp"

#include "Fonts/chivo_bold.hpp"

START_NAMESPACE_DISTRHO

NanoLabel::NanoLabel(NanoWidget *widget, Size<uint> size) noexcept : NanoWidget(widget),
                                                                     fFontSize(12.0f),
                                                                     fAlign(ALIGN_LEFT | ALIGN_TOP),
                                                                     fText(""),
                                                                     fMargin(Margin(0, 0, 0, 0)),
                                                                     fColor(Color(255, 255, 255, 255))
{
    setSize(size);

    using namespace SPOONIE_FONTS;
    fFontId = createFontFromMemory("chivo_bold", (const uchar *)chivo_bold, chivo_bold_size, 0);
}

void NanoLabel::onNanoDisplay()
{
    beginPath();

    fontFaceId(fFontId);
    fontSize(fFontSize);
    fillColor(fColor);
    textAlign(fAlign);

    text(0, 0, fText, NULL);

    closePath();
}

void NanoLabel::setColor(Color color)
{
    fColor = color;
}

void NanoLabel::setFontSize(float fontSize)
{
    fFontSize = fontSize;
}

void NanoLabel::setText(const char *text)
{
    fText = text;
}

void NanoLabel::setAlign(int align)
{
    fAlign = align;
}

void NanoLabel::setMargin(Margin margin)
{
    fMargin = margin;
}

void NanoLabel::setFontId(NanoVG::FontId fontId)
{
    fFontId = fontId;
}

END_NAMESPACE_DISTRHO