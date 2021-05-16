#include "WidgetBar.hpp"
#include "Mathf.hpp"

START_NAMESPACE_DISTRHO

WidgetBar::WidgetBar(Widget  *widget, Size<uint> size) noexcept : WolfWidget(widget),
                                                                     fFillPaint(linearGradient(0, 0, 0, 0, Color(0, 0, 0, 0), Color(0, 0, 0, 0))),
                                                                     fStrokePaint(linearGradient(0, 0, 0, 0, Color(0, 0, 0, 0), Color(0, 0, 0, 0))),
                                                                     fStrokeWidth(2.0f)
{
    setSize(size);
}

void WidgetBar::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();

    beginPath();

    fillPaint(fFillPaint);
    strokePaint(fStrokePaint);
    strokeWidth(fStrokeWidth);

    rect(0, 0, width, height);
    fill();
    stroke();

    closePath();
}

void WidgetBar::setFillPaint(Paint fillPaint)
{
    fFillPaint = fillPaint;
}

void WidgetBar::setStrokePaint(Paint strokePaint)
{
    fStrokePaint = strokePaint;
}

void WidgetBar::setStrokeWidth(const float strokeWidth)
{
    fStrokeWidth = strokeWidth;
}  

END_NAMESPACE_DISTRHO