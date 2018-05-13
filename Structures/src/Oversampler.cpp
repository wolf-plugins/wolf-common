#include "Oversampler.hpp"

START_NAMESPACE_DISTRHO

Oversampler::Oversampler() : fRatio(-1),
                             fSampleRate(44100),
                             fNumSamples(512),
                             fCurrentCapacity(fNumSamples),
                             fRequiredCapacity(fNumSamples),
                             fNumInputSamples(512)
{
    fBuffer = (float **)malloc(sizeof(float *) * 2);

    fBuffer[0] = (float *)malloc(fNumSamples * sizeof(float));
    memset(fBuffer[0], 0, fRequiredCapacity);

    fBuffer[1] = (float *)malloc(fNumSamples * sizeof(float));
    memset(fBuffer[1], 0, fRequiredCapacity);

    fInput = (float **)malloc(sizeof(float *) * 2);
    fInput[0] = (float *)malloc(fNumInputSamples * sizeof(float));
    fInput[1] = (float *)malloc(fNumInputSamples * sizeof(float));
}

Oversampler::~Oversampler()
{
    free(fBuffer[1]);
    free(fBuffer[0]);
    free(fBuffer);

    free(fInput[1]);
    free(fInput[0]);
    free(fInput);
}

float **Oversampler::upsample(int ratio, uint32_t numSamples, double sampleRate, const float **audio)
{
    if (fSampleRate != sampleRate * ratio || fRatio != ratio)
    {
        fSampleRate = sampleRate * ratio;

        fUpsamplers[0].setup(sampleRate, fSampleRate, 1, 16);
        fUpsamplers[1].setup(sampleRate, fSampleRate, 1, 16);

        fDownsamplers[0].setup(fSampleRate, sampleRate, 1, 16);
        fDownsamplers[1].setup(fSampleRate, sampleRate, 1, 16);
    }

    if (fNumInputSamples < numSamples)
    {
        fInput[0] = (float *)realloc(fInput[0], numSamples * sizeof(float));
        fInput[1] = (float *)realloc(fInput[1], numSamples * sizeof(float));

        fNumInputSamples = numSamples;
    }

    fRatio = ratio;
    fNumSamples = numSamples;

    fRequiredCapacity = numSamples * ratio;

    if (fCurrentCapacity < fRequiredCapacity)
    {
        fBuffer[0] = (float *)realloc(fBuffer[0], fRequiredCapacity * sizeof(float));
        fBuffer[1] = (float *)realloc(fBuffer[1], fRequiredCapacity * sizeof(float));

        fCurrentCapacity = fRequiredCapacity;
    }

    memset(fBuffer[0], 0, fRequiredCapacity);
    memset(fBuffer[1], 0, fRequiredCapacity);
    
    /* if(fRatio == 1)
    {
        for (uint32_t i = 0; i < fNumSamples; ++i)
        {
            fBuffer[0][i] = audio[0][i];
            fBuffer[1][i] = audio[1][i];
        }

        return fBuffer;
    } */

    for (uint32_t i = 0; i < 2; ++i)
    {
        for (uint32_t j = 0; j < fNumSamples; ++j)
        {
            fInput[i][j] = audio[i][j];
        }

        fUpsamplers[i].inp_data = fInput[i];
        fUpsamplers[i].out_data = fBuffer[i];
        fUpsamplers[i].inp_count = fNumSamples;
        fUpsamplers[i].out_count = fRequiredCapacity;

        fUpsamplers[i].process();
    }

    return fBuffer;
}

void Oversampler::downsample(float **targetBuffer)
{
    for (int i = 0; i < 2; ++i)
    {
        fDownsamplers[i].inp_data = fBuffer[i];
        fDownsamplers[i].out_data = targetBuffer[i];
        fDownsamplers[i].inp_count = fRequiredCapacity;
        fDownsamplers[i].out_count = fNumSamples;

        fDownsamplers[i].process();
    }
}

END_NAMESPACE_DISTRHO