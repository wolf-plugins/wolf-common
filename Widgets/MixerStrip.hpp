#ifndef WOLF_MIXER_STRIP_HPP_INCLUDED
#define WOLF_MIXER_STRIP_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class MixerStrip : public NanoWidget
{
public:
    explicit MixerStrip(NanoWidget* widget, Size<uint> size) noexcept;
    NanoVG::Paint generateNoiseTexture();

protected:
    void onNanoDisplay() override;

private:
    NanoVG::Paint fNoiseTexture;
    
    DISTRHO_LEAK_DETECTOR(MixerStrip)
};

END_NAMESPACE_DISTRHO

#endif