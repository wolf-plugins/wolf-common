#include "RightClickMenu.hpp"
#include "Fonts/chivo_bold.hpp"
#include <unistd.h>

START_NAMESPACE_DISTRHO

RightClickMenuEntry::RightClickMenuEntry(int id, const char *label, bool enabled) noexcept : fId(id),
                                                                                             fLabel(label),
                                                                                             fEnabled(enabled)
{
}

int RightClickMenuEntry::getId()
{
    return fId;
}

const char *RightClickMenuEntry::getLabel()
{
    return fLabel;
}

bool RightClickMenuEntry::getEnabled()
{
    return fEnabled;
}

RightClickMenu::RightClickMenu(NanoWidget *parent) noexcept : Window(parent->getParentApp(), parent->getParentWindow()),
                                                              NanoWidget((Window &)*this),
                                                              fParent(parent),
                                                              fFontSize(22.0f)
{
    Window::setResizable(false);
    Window::setBorderless(true);

    using namespace WOLF_FONTS;
    createFontFromMemory("chivo_bold", (const uchar *)chivo_bold, chivo_bold_size, 0);
}

RightClickMenu::~RightClickMenu()
{
    Window::close();
}

void RightClickMenu::show(int posX, int posY)
{
    //we don't want the mouse to intersect with the popup straight away, so we add a bit of margin
    posX += 2;
    posY += 2;

    Window::hideFromTaskbar();

    Window &parentWindow = fParent->getParentWindow();
    Point<int> windowPos = parentWindow.getAbsolutePos();

    //FIXME: this is really a mess... right now, it's necessary to set the size before and after the exec to get the correct window dimensions on win32...
    //it still flickers a bit, so it's not a perfect solution
#if defined(DISTRHO_OS_WINDOWS)
    const Size<uint> size = Size<uint>(fLongestWidth, fEntries.size() * fFontSize);

    Window::setSize(size);
    NanoWidget::setSize(size);

    Window::setAbsolutePos(posX + windowPos.getX(), posY + windowPos.getY());
#endif

    Window::exec(false);

#if defined(DISTRHO_OS_WINDOWS)
    Window::setSize(size);
    NanoWidget::setSize(size);
#endif

    Window::setAbsolutePos(posX + windowPos.getX(), posY + windowPos.getY());
}

void RightClickMenu::close()
{
    Window::close();
}

void RightClickMenu::setTitle(const char *title)
{
    fTitle = title;
}

Rectangle<float> RightClickMenu::getBoundsOfEntry(const int index)
{
    fontSize(fFontSize);
    textAlign(ALIGN_LEFT | ALIGN_TOP);

    Rectangle<float> bounds;

    textBounds(0, index * fFontSize, fEntries[index].getLabel(), NULL, bounds);

    return bounds;
}

void RightClickMenu::setEntries(std::vector<RightClickMenuEntry> entries)
{
    fEntries = entries;

    //find the longest entry

    fLongestWidth = 0.0f;

    for (size_t i = 0; i < fEntries.size(); ++i)
    {
        const float entryWidth = getBoundsOfEntry(i).getWidth();

        if (entryWidth > fLongestWidth)
        {
            fLongestWidth = entryWidth;
        }
    }

    const Size<uint> size = Size<uint>(fLongestWidth, fEntries.size() * fFontSize);

    Window::setSize(size);
    NanoWidget::setSize(size);
}

void RightClickMenu::onNanoDisplay()
{
    const float width = NanoWidget::getWidth();
    const float height = NanoWidget::getHeight();

    beginPath();

    Rectangle<float> bounds;

    fontSize(fFontSize);
    textAlign(ALIGN_LEFT | ALIGN_TOP);
    textBounds(0, 0, fEntries[0].getLabel(), NULL, bounds);

    fillColor(Color(255, 0, 0, 255));
    rect(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
    fill();

    fillColor(Color(255, 255, 255, 255));

    float verticalOffset = 0;

    for (size_t i = 0; i < fEntries.size(); ++i)
    {
        text(0, verticalOffset, fEntries[i].getLabel(), NULL);
        verticalOffset += bounds.getHeight();
    }

    closePath();
}

bool RightClickMenu::onMouse(const MouseEvent &ev)
{
    if (ev.press == false)
    {
        for (size_t i = 0; i < fEntries.size(); ++i)
        {
            if (getBoundsOfEntry(i).contains(Point<float>(ev.pos.getX(), ev.pos.getY())))
            {
                close();

                return true;
            }
        }
    }

    return true;
}

void RightClickMenu::onFocusOut()
{
    close();
}

END_NAMESPACE_DISTRHO
