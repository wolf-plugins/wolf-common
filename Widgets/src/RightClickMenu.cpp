#include "RightClickMenu.hpp"

START_NAMESPACE_DISTRHO

RightClickMenu::RightClickMenu(NanoWidget *parent, Size<uint> size) noexcept : Window(parent->getParentApp()),
                                                                               NanoWidget((Window&)*this),
                                                                               fParent(parent)
{
    Window::setResizable(false);
    Window::setSize(size);
    Window::setBorderless(true);

    NanoWidget::setSize(size);
}

RightClickMenu::~RightClickMenu()
{
    Window::close();
}

void RightClickMenu::show(int posX, int posY)
{
    Window::hideFromTaskbar();    
    Window::show();
    
    Window &parentWindow = fParent->getParentWindow();
    Point<int> windowPos = parentWindow.getAbsolutePos();

    Window::setAbsolutePos(posX + windowPos.getX(), posY + windowPos.getY());
}

void RightClickMenu::close()
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
