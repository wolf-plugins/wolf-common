#ifndef NANOSVG_UTILS
#define NANOSVG_UTILS

#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdint>

#include "nanosvg.h"
#include "nanovg.h"

START_NAMESPACE_DISTRHO

namespace SVGUtils
{

NVGcolor nvgColorU32(uint32_t color);
void nvgSVGLinearGrad(NVGcontext *vg, NSVGgradient *grad);
void nvgDrawSVG(NVGcontext *vg, NSVGimage *image);

};

END_NAMESPACE_DISTRHO

#endif