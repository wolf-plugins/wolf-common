#include "VolumeKnob.hpp"

START_NAMESPACE_DISTRHO

VolumeKnob::VolumeKnob(NanoWidget *widget, Size<uint> size) noexcept : NanoKnob(widget, size)

{
    const float radius = size.getHeight() / 2.0f;
    const float gaugeWidth = 3.5f;
    const float diameter = (radius - gaugeWidth) * 2.0f;

    fKnobDiameter = diameter - 2;

    fGrowAnimation = new FloatTransition(0.100f, &fKnobDiameter, fKnobDiameter - 7);

    widget->getParentWindow().addIdleCallback(this);
}

void VolumeKnob::idleCallback()
{
    if (fGrowAnimation->isPlaying())
    {
        fGrowAnimation->run();
        repaint();
    }
}

void VolumeKnob::onMouseHover()
{
    getParentWindow().setCursorStyle(Window::CursorStyle::Grab);
}

void VolumeKnob::onMouseLeave()
{
    getParentWindow().setCursorStyle(Window::CursorStyle::Default);
}

void VolumeKnob::onMouseDown()
{
    fGrowAnimation->pause();
    fGrowAnimation->setDuration(0.100f);
    fGrowAnimation->seek(fGrowAnimation->getCurrentTime() / 2.0f);
    fGrowAnimation->play(Animation::Forward);
}

void VolumeKnob::onMouseUp()
{
    fGrowAnimation->pause();
    fGrowAnimation->setDuration(0.400f);
    fGrowAnimation->seek(fGrowAnimation->getCurrentTime() * 2.0f);
    fGrowAnimation->play(Animation::Backward);
}

void VolumeKnob::draw()
{
    const float width = getWidth();
    const float height = getHeight();
    const Color color = getColor();

    const float value = getValue();
    const float min = getMin();
    const float max = getMax();

    const float percentFilled = (value - min) / (max - min);

    const float radius = height / 2.0f;

    const float indicatorLineHeight = fKnobDiameter / 2.0f - 8;
    const float indicatorLineWidth = 3.0f;
    const float indicatorLineMarginTop = 4.0f;

    const float gaugeWidth = 3.5f;
    Color gaugeColor = Color(0, 0, 40, 255);
    gaugeColor.interpolate(color, 0.4f);

    const float margin = 3.0f;
    
    //Gauge (empty)
    beginPath();

    strokeWidth(gaugeWidth);
    strokeColor(gaugeColor);
    arc(radius, radius, radius - margin, 0.75f * M_PI, 0.25f * M_PI, NanoVG::Winding::CW);
    stroke();

    //Gauge (value)
    beginPath();

    strokeWidth(gaugeWidth);
    strokeColor(color);
    arc(radius, radius, radius - margin, 0.75f * M_PI, (0.75f + 1.5f * percentFilled) * M_PI, NanoVG::Winding::CW);
    stroke();

    //Knob
    beginPath();

    Paint knobPaint = linearGradient(radius, gaugeWidth, radius, fKnobDiameter, Color(86, 92, 95, 255), Color(39, 42, 43, 255));
    //strokeWidth(0.5f);
    //strokeColor(37,37,37,255);

    fillPaint(knobPaint);

    circle(radius, radius, fKnobDiameter / 2.0f);
    fill();
    //stroke();

    //Indicator line
    beginPath();
    save();

    translate(radius, radius);
    rotate((2.0f + ((percentFilled - 0.5f) * 1.5f)) * M_PI);
    translate(-radius, -radius);

    fillColor(color);
    rect(radius - indicatorLineWidth / 2.0f, margin + indicatorLineMarginTop + fKnobDiameter / 2.0f - radius, indicatorLineWidth, indicatorLineHeight);
    fill();

    restore();

    closePath();
}

END_NAMESPACE_DISTRHO