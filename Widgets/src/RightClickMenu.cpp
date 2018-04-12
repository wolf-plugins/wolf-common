#include "RightClickMenu.hpp"

START_NAMESPACE_DISTRHO

RightClickMenu::RightClickMenu(NanoWidget *parent, Size<uint> size) noexcept : Window(parent->getParentApp(), parent->getParentWindow()),
                                                                               NanoWidget((Window&)*this)
{
    Window::setResizable(false);
    Window::setSize(size);
    Window::setBorderless(true);

    NanoWidget::setSize(size);

    Window::show();
}

RightClickMenu::~RightClickMenu()
{
    Window::close();
}

void RightClickMenu::setEntries(std::vector<const char *> entries)
{
    fEntries = entries;
}

void RightClickMenu::onNanoDisplay()
{
    const float width = NanoWidget::getWidth();
    const float height = NanoWidget::getHeight();

    beginPath();

    fillColor(Color(255, 0, 0, 255));
    rect(0, 0, width, height);
    fill();
    
    closePath();
}

END_NAMESPACE_DISTRHO
