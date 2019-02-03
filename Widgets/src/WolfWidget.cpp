#include "WolfWidget.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

WolfWidget::WolfWidget(NanoWidget *widget) noexcept : NanoWidget(widget)
{
    fFocusedWidgetId = widget->getFocusedWidgetIdPtr();
}

void WolfWidget::setDescription(const char *description)
{
    fDescription = description;
}

const char *WolfWidget::getDescription()
{
    return fDescription;
}

bool WolfWidget::canBeFocused()
{
    const uint id = *fFocusedWidgetId;

    return id == kNoWidgetFocusedId || id == getId();
}

void WolfWidget::setFocus(bool focus)
{
    if(focus)
        *fFocusedWidgetId = getId();
    else
        *fFocusedWidgetId = kNoWidgetFocusedId;
}

END_NAMESPACE_DISTRHO
