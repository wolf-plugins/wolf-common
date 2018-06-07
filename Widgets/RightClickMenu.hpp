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

    void getSelected();
    void setSelected();

    bool isSection();

  protected:
      bool fIsSection;

  private:
    int fId;
    bool fEnabled;
    const char *fLabel;

    bool fSelected;
    
    Rectangle<float> fBounds;
};

class RightClickMenuSection : public RightClickMenuEntry
{
  public:
    RightClickMenuSection(const char *label) noexcept;
};

class RightClickMenu : private Window,
                       public NanoWidget
{
  public:
    class Callback
    {
      public:
        virtual ~Callback() {}
        virtual void rightClickMenuEntrySelected(RightClickMenuEntry *rightClickMenuEntry) = 0;
    };

    RightClickMenu(NanoWidget *parent) noexcept;
    ~RightClickMenu();

    void show(int posX, int posY);
    void close();
    void setTitle(const char *title);
    void setEntries(std::vector<RightClickMenuEntry> entries);
    void setBorderColor(const Color color);
    void setRegularFontSize(float fontSize);
    void setSectionFontSize(float fontSize);

    void setCallback(Callback *callback) noexcept;

  protected:
    void onNanoDisplay() override;
    void onFocusOut() override;
    bool onMouse(const MouseEvent &ev) override;

    void adaptSize();

    Rectangle<float> getBoundsOfEntry(const int index);
    
  private:
    std::vector<RightClickMenuEntry> fEntries;
    const char *fTitle;
    NanoWidget *fParent;

    float fFontSize;
    float fSectionFontSize;

    float fLongestWidth;
    Color fBorderColor;
    Margin fMargin;

    Callback *fCallback;
};

#endif

END_NAMESPACE_DISTRHO
