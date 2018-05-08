#include "ParamSmooth.hpp"

START_NAMESPACE_DISTRHO

ParamSmooth::ParamSmooth() : fHistory(0.0f),
                             fValue(0.0f)
{
}

ParamSmooth::ParamSmooth(float value) : fHistory(0.0f),
                                        fValue(value)
{
}

void ParamSmooth::setValue(float value)
{
    fValue = value;
}

float ParamSmooth::getRawValue() const
{
    return fValue;
}

float ParamSmooth::getSmoothedValue(float frequency, double sampleRate)
{
    float coeff = std::exp(-2.0 * M_PI * frequency / sampleRate);

    float result = fValue + coeff * (fHistory - fValue);
    fHistory = result;

    return result;
}

END_NAMESPACE_DISTRHO