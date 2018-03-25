#include "OversampleWheel.hpp"
#include "Fonts/chivo_bold.hpp"

START_NAMESPACE_DISTRHO

//TODO: Put these in the config file
const float trianglesVerticalMargin = 7.0f;
const float trianglesHorizontalMargin = 4.0f;
const float trianglesWidth = 8.0f;
const float trianglesHeight = 6.0f;

OversampleWheel::OversampleWheel(Window &parent, Size<uint> size) noexcept : NanoWheel(parent, size)
{
    using namespace SPOONIE_FONTS;
    createFontFromMemory("chivo_bold", (const uchar *)chivo_bold, chivo_bold_size, 0);
}

OversampleWheel::OversampleWheel(NanoWidget *widget, Size<uint> size) noexcept : NanoWheel(widget, size)
{
    using namespace SPOONIE_FONTS;
    createFontFromMemory("chivo_bold", (const uchar *)chivo_bold, chivo_bold_size, 0);
}

void OversampleWheel::drawTriangles()
{
    const float width = getWidth();
    const float height = getHeight();

    const float rightPointXLocation = width - trianglesHorizontalMargin;
    const float leftPointXLocation = rightPointXLocation - trianglesWidth;
    const float middlePointXLocation = rightPointXLocation - trianglesWidth / 2.0f;

    //top triangle
    const float topTriangleBottomY = trianglesVerticalMargin + trianglesHeight;

    beginPath();

    moveTo(middlePointXLocation, trianglesVerticalMargin);

    lineTo(leftPointXLocation, topTriangleBottomY);
    lineTo(rightPointXLocation, topTriangleBottomY);
    lineTo(middlePointXLocation, trianglesVerticalMargin);

    fillColor(Color(255, 255, 255, 255));
    strokeColor(Color(0, 0, 0, 255));
    strokeWidth(1.0f);

    fill();
    stroke();

    closePath();

    //bottom triangle
    beginPath();

    const float bottomTriangleBottomY = height - trianglesVerticalMargin;
    const float bottomTriangleTopY = bottomTriangleBottomY - trianglesHeight;

    moveTo(middlePointXLocation, bottomTriangleBottomY);

    lineTo(leftPointXLocation, bottomTriangleTopY);
    lineTo(rightPointXLocation, bottomTriangleTopY);
    lineTo(middlePointXLocation, bottomTriangleBottomY);

    fillColor(Color(255, 255, 255, 255));
    strokeColor(Color(0, 0, 0, 255));

    fill();
    stroke();

    closePath();
}

void OversampleWheel::draw()
{
    const float width = getWidth();
    const float height = getHeight();

    const char *oversamplingFactors[] = {"", "2x", "4x", "8x", "16x"};

    //outline
    beginPath();

    const float outlineWidth = 2.0f;

    fillColor(Color(27, 27, 27, 255));
    roundedRect(0, 0, width, height, 4.0f);
    fill();

    closePath();

    //background
    beginPath();

    fillColor(Color(54, 52, 88, 255));
    rect(outlineWidth, outlineWidth, width - outlineWidth * 2.0f, height - outlineWidth * 2.0f);
    fill();

    closePath();

    //line at top of display
    beginPath();

    const float widthLineTopDisplay = 1.0f;

    strokeColor(Color(72, 137, 208, 255));
    strokeWidth(widthLineTopDisplay);

    moveTo(outlineWidth, outlineWidth);
    lineTo(width - outlineWidth, outlineWidth);

    stroke();

    closePath();

    //triangles
    drawTriangles();

    //text
    const int value = getValue();

    if (value > 0)
    {
        beginPath();

        fontFace("chivo_bold");
        fontSize(26.0f);
        fillColor(Color(193, 219, 240, 255));
        textAlign(ALIGN_RIGHT | ALIGN_CENTER);

        text(std::round(width - trianglesWidth - trianglesHorizontalMargin - 5), std::round(height / 1.45f), oversamplingFactors[getValue()], NULL);

        closePath();
    }
}

END_NAMESPACE_DISTRHO