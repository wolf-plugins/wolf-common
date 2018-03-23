#include "VolumeKnob.hpp"

START_NAMESPACE_DISTRHO

VolumeKnob::VolumeKnob(NanoWidget *widget, Size<uint> size) noexcept : NanoKnob(widget, size)

{
    const float radius = size.getHeight() / 2.0f;
    const float gaugeWidth = 3.5f;
    const float diameter = (radius - gaugeWidth) * 2.0f;

    fKnobSize = Size<uint>(diameter, diameter);
    fGrowAnimation = new SizeChangeAnimation(0.200f, &fKnobSize, Size<uint>(fKnobSize.getWidth() - 8, fKnobSize.getHeight() - 8));

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
    fGrowAnimation->pause();
    fGrowAnimation->setDuration(0.200f);
    fGrowAnimation->seek(fGrowAnimation->getCurrentTime() / 2.0f);
    fGrowAnimation->play(Animation::Forward);
}

void VolumeKnob::onMouseLeave()
{
    fGrowAnimation->pause();
    fGrowAnimation->setDuration(0.400f);
    fGrowAnimation->seek(fGrowAnimation->getCurrentTime() * 2.0f);
    fGrowAnimation->play(Animation::Backward);
}

void VolumeKnob::drawNormal()
{
    const float width = getWidth();
    const float height = getHeight();
    const Color color = getColor();

    const float value = getValue();
    const float min = getMin();
    const float max = getMax();

    const float percentFilled = (value - min) / (max - min);

    const float radius = height / 2.0f;

    const float indicatorLineHeight = fKnobSize.getHeight() / 2.0f - 8;
    const float indicatorLineWidth = 3.0f;
    const float indicatorLineMarginTop = 4.0f;

    const float gaugeWidth = 3.5f;
    Color gaugeColor = Color(0, 0, 40, 255);
    gaugeColor.interpolate(color, 0.4f);

    const float margin = 2.0f;

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

    Paint knobPaint = linearGradient(radius, gaugeWidth, radius, fKnobSize.getHeight(), Color(86, 92, 95, 255), Color(39, 42, 43, 255));
    //strokeWidth(0.5f);
    //strokeColor(37,37,37,255);

    fillPaint(knobPaint);

    circle(radius, radius, fKnobSize.getWidth() / 2.0f);
    fill();
    //stroke();

    //Indicator line
    beginPath();
    save();

    translate(radius, radius);
    rotate((2.0f + ((percentFilled - 0.5f) * 1.5f)) * M_PI);
    translate(-radius, -radius);

    fillColor(color);
    rect(radius - indicatorLineWidth / 2.0f, margin + indicatorLineMarginTop + fKnobSize.getHeight() / 2.0f - radius, indicatorLineWidth, indicatorLineHeight);
    fill();

    restore();

    //Label
    /*fontFace("roboto_light");
    fontSize(32.f);
    fillColor(255, 255, 255, 125);

    textAlign(ALIGN_BOTTOM | ALIGN_RIGHT);
    text(getWidth() - 5, getHeight(), "In", NULL);

    textAlign(ALIGN_TOP | ALIGN_LEFT);
    text(5, 0, "Out", NULL);*/

    closePath();
}

void VolumeKnob::drawHover()
{
}

void VolumeKnob::drawTurning()
{
}

END_NAMESPACE_DISTRHO