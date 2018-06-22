#ifndef WOLF_WIDGET_HPP_INCLUDED
#define WOLF_WIDGET_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class WolfWidget : public NanoWidget
{
    void setDescription();
    const char* getDescription();

  protected:
    bool onMotion() override;
    bool onMouse() override;

    virtual void onMouseEnter();
    virtual void onMouseLeave();

    void mouseWarp(); //TODO: design this
    
    //fancy way of dealing with double-click ? mouse up, mouse down?
    //WolfWidget::onMotion vs NanoWidget::onMotion ?
  private:
    WolfWidget* fFocusedWidget; //only one focused widget at a time

    DISTRHO_LEAK_DETECTOR(WolfWidget)
};

END_NAMESPACE_DISTRHO

#endif
