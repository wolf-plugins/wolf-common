#ifndef WOLF_OVERSAMPLER_INCLUDED
#define WOLF_OVERSAMPLER_INCLUDED

#include "DspFilters/Dsp.h"
#include "src/DistrhoDefines.h"
#include "extra/LeakDetector.hpp"
#include "resampler.hpp"

START_NAMESPACE_DISTRHO

class Oversampler
{
  public:
    Oversampler();
    ~Oversampler();

    float **upsample(int ratio, uint32_t numSamples, double sampleRate, const float **audio);
    void downsample(float **targetBuffer);

  protected:
    void lowPass1();
    void lowPass2();

    void reverse();
    void gainBoost();

  private:
    int fRatio;
    double fSampleRate;
    uint32_t fNumSamples;

    Resampler fUpsamplers[2];
    Resampler fDownsamplers[2];

    uint32_t fCurrentCapacity;
    uint32_t fRequiredCapacity;

    float **fBuffer;

    float **fInput;
    uint32_t fNumInputSamples;
    
    DISTRHO_LEAK_DETECTOR(Oversampler)
};

END_NAMESPACE_DISTRHO

#endif