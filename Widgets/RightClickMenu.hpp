#ifndef WOLF_RIGHT_CLICK_MENU_H
#define WOLF_RIGHT_CLICK_MENU_H

#include "Widget.hpp"
#include "Window.hpp"
#include "NanoVG.hpp"
#include "NanoLabel.hpp"

#include <vector>

START_NAMESPACE_DISTRHO

class RightClickMenuEntry
{
  public:
    RightClickMenuEntry(int id, const char *label, bool enabled = true) noexcept;

    int getId();
    bool getEnabled();
    const char *getLabel();

  private:
    int fId;
    bool fEnabled;
    const char *fLabel;

    Rectangle<float> fBounds;
};

class RightClickMenu : private Window,
                       private NanoWidget
{
  public:
    RightClickMenu(NanoWidget *parent) noexcept;
    ~RightClickMenu();

    void show(int posX, int posY);
    void close();
    void setTitle(const char *title);
    void setEntries(std::vector<RightClickMenuEntry> entries);

  protected:
    void onNanoDisplay() override;
    void onFocusOut() override;
    bool onMouse(const MouseEvent &ev) override;

    Rectangle<float> getBoundsOfEntry(const int index);
    
  private:
    std::vector<RightClickMenuEntry> fEntries;
    const char *fTitle;
    NanoWidget *fParent;

    float fFontSize;
    float fLongestWidth;
};

#endif

END_NAMESPACE_DISTRHO
