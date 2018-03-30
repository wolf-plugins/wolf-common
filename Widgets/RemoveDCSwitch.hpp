#ifndef SPOONIE_REMOVE_DC_SWITCH_HPP_INCLUDED
#define SPOONIE_REMOVE_DC_SWITCH_HPP_INCLUDED

#include "NanoSwitch.hpp"

START_NAMESPACE_DISTRHO

class RemoveDCSwitch : public NanoSwitch
{
public:
    explicit RemoveDCSwitch(Window &parent, Size<uint> size) noexcept;
    explicit RemoveDCSwitch(NanoWidget *widget, Size<uint> size) noexcept;

protected:
    void draw() override;
    
private:
    DISTRHO_LEAK_DETECTOR(RemoveDCSwitch)
};

END_NAMESPACE_DISTRHO

#endif