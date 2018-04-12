#ifndef WOLF_RIGHT_CLICK_MENU_H
#define WOLF_RIGHT_CLICK_MENU_H

#include "Widget.hpp"
#include "Window.hpp"
#include "NanoVG.hpp"
#include "NanoLabel.hpp"

#include <vector>

START_NAMESPACE_DISTRHO

class RightClickMenu :  public Window,
                        public NanoWidget
{
public:
    RightClickMenu(NanoWidget *parent, Size<uint> size) noexcept;
    ~RightClickMenu();

protected:
    void onNanoDisplay() override;
    void setEntries(std::vector<const char *> entries);
    
private:
    std::vector<const char *> fEntries;
};

#endif

END_NAMESPACE_DISTRHO
