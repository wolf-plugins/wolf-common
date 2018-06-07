#include "RightClickMenu.hpp"
#include <unistd.h>

START_NAMESPACE_DISTRHO

RightClickMenuEntry::RightClickMenuEntry(int id, const char *label, bool enabled) noexcept : fId(id),
                                                                                             fLabel(label),
                                                                                             fEnabled(enabled),
                                                                                             fIsSection(false)
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

bool RightClickMenuEntry::isSection()
{
    return fIsSection;
}

RightClickMenuSection::RightClickMenuSection(const char *label) noexcept : RightClickMenuEntry(-1, label, false)
{
    fIsSection = true;
}

RightClickMenu::RightClickMenu(NanoWidget *parent) noexcept : Window(parent->getParentApp(), parent->getParentWindow()),
                                                              NanoWidget((Window &)*this),
                                                              fParent(parent),
                                                              fFontSize(17.0f),
                                                              fSectionFontSize(14.0f),
                                                              fBorderColor(Color(161, 67, 198, 255)),
                                                              fMargin(Margin(7, 15, 7, 13))
{
    Window::setResizable(false);
    Window::setBorderless(true);

    loadSharedResources();
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
    adaptSize();

    Window::setAbsolutePos(posX + windowPos.getX(), posY + windowPos.getY());
#endif

    Window::exec(false);

#if defined(DISTRHO_OS_WINDOWS)
    adaptSize();
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

void RightClickMenu::setBorderColor(const Color color)
{
    fBorderColor = color;
}

void RightClickMenu::setRegularFontSize(float fontSize)
{
    fFontSize = fontSize;
}

void RightClickMenu::setSectionFontSize(float fontSize)
{
    fSectionFontSize = fontSize;
}

Rectangle<float> RightClickMenu::getBoundsOfEntry(const int index)
{
    fontSize(fFontSize);
    textAlign(ALIGN_LEFT | ALIGN_TOP);

    Rectangle<float> bounds;

    textBounds(0 + fMargin.left, index * fFontSize + fMargin.top, fEntries[index].getLabel(), NULL, bounds);

    return bounds;
}

void RightClickMenu::adaptSize()
{
    const Size<uint> size = Size<uint>(fLongestWidth + fMargin.left + fMargin.right + 12, fEntries.size() * fFontSize + fMargin.top + fMargin.bottom);

    Window::setSize(size);
    NanoWidget::setSize(size);
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

    adaptSize();
}

void RightClickMenu::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
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

    fillColor(Color(39, 39, 39, 255));

    strokeColor(fBorderColor);
    strokeWidth(3.0f);

    rect(0, 0, width, height);
    fill();
    stroke();

    closePath();

    float verticalOffset = 0;

    translate(fMargin.left, fMargin.top);

    for (size_t i = 0; i < fEntries.size(); ++i)
    {
        beginPath();

        if (fEntries[i].isSection())
        {
            fontSize(fSectionFontSize);
        }
        else
        {
            fontSize(fFontSize);
        }

        if (fEntries[i].getEnabled() == true)
        {
            fillColor(Color(255, 255, 255, 255));
        }
        else
        {
            fillColor(Color(170, 170, 170, 255));
        }

        text(fEntries[i].isSection() ? 0 : 12, verticalOffset, fEntries[i].getLabel(), NULL);
        verticalOffset += bounds.getHeight();

        closePath();
    }
}

bool RightClickMenu::onMouse(const MouseEvent &ev)
{
    if (ev.press == false)
    {
        for (size_t i = 0; i < fEntries.size(); ++i)
        {
            if (fEntries[i].getEnabled() == true && getBoundsOfEntry(i).contains(Point<float>(ev.pos.getX(), ev.pos.getY())))
            {
                fCallback->rightClickMenuEntrySelected(&fEntries[i]);

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
