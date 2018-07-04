#include "src/DistrhoDefines.h"
#include "DistrhoUtils.hpp"

#define NANOSVG_ALL_COLOR_KEYWORDS // Include full list of color keywords.
#define NANOSVG_IMPLEMENTATION     // Expands implementation
#include "SVGUtils.hpp"

START_NAMESPACE_DISTRHO

namespace SVGUtils
{

NVGcolor nvgColorU32(uint32_t color)
{
    return nvgRGB(color & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF);
}

void nvgSVGLinearGrad(NVGcontext *vg, NSVGgradient *grad)
{
    float sx = grad->xform[4];
    float sy = grad->xform[5];
    float ex = sx + grad->xform[2];
    float ey = sy + grad->xform[3];
    NVGcolor cs = nvgColorU32(grad->stops[0].color);
    NVGcolor ce = nvgColorU32(grad->stops[1].color);

    nvgLinearGradient(vg, sx, sy, ex, ey, cs, ce);
}

void nvgDrawSVG(NVGcontext *vg, NSVGimage *image)
{
    DISTRHO_SAFE_ASSERT(image != nullptr)

    for (NSVGshape *shape = image->shapes; shape != NULL; shape = shape->next)
    {
        nvgBeginPath(vg);

        nvgStrokeWidth(vg, shape->strokeWidth);

        uint32_t color = shape->stroke.color;
        NVGcolor ncolor = nvgColorU32(color);
        nvgStrokeColor(vg, ncolor);

        for (NSVGpath *path = shape->paths; path != NULL; path = path->next)
        {
            nvgMoveTo(vg, path->pts[0], path->pts[1]);

            for (int i = 0; i < path->npts - 1; i += 3)
            {
                float *p = &path->pts[i * 2];
                nvgBezierTo(vg, p[2], p[3], p[4], p[5], p[6], p[7]);
            }

            if (path->closed)
            {
                nvgLineTo(vg, path->pts[0], path->pts[1]);

                if (shape->fill.type == NSVG_PAINT_COLOR)
                {
                    nvgFillColor(vg, nvgColorU32(shape->fill.color));
                }
                else if (shape->fill.type == NSVG_PAINT_LINEAR_GRADIENT)
                {
                    nvgSVGLinearGrad(vg, shape->fill.gradient);
                }

                if (shape->fill.type != NSVG_PAINT_NONE)
                {
                    nvgFill(vg);
                }
            }

            if (shape->stroke.type != NSVG_PAINT_NONE)
            {
                nvgStroke(vg);
            }
        }
    }

    //nvgStroke(vg);
}

}; // namespace SVGUtils

END_NAMESPACE_DISTRHO