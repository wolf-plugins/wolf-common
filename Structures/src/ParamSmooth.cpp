#include "ParamSmooth.hpp"

START_NAMESPACE_DISTRHO

ParamSmooth::ParamSmooth() : fHistory(0.0f)
{
}

float ParamSmooth::process(float input, float frequency, double sampleRate)
{
    float coeff = std::exp(-2.0 * M_PI * frequency / sampleRate);

    float result = input + coeff * (fHistory - input);
    fHistory = result;

    return result;
}

END_NAMESPACE_DISTRHO