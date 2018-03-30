#include "Animation.hpp"
#include "Color.hpp"
#include "src/DistrhoDefines.h"
#include "Mathf.hpp"
#include "Geometry.hpp"
#include "Widget.hpp"
#include <chrono>

START_NAMESPACE_DISTRHO

Animation::Animation(float duration, EasingFunction easingFunction) : fDuration(duration),
																	  fEasingFunction(easingFunction),
																	  fIsPlaying(false)
{
}

Animation::~Animation()
{
}

void Animation::play(PlaybackDirection playbackDirection, RepeatMode repeatMode)
{
	fIsPlaying = true;

	fPlaybackDirection = playbackDirection;
	fRepeatMode = repeatMode;

	fTimeLastRun = std::chrono::steady_clock::now();

	onPlay();
}

void Animation::onPlay()
{

}

void Animation::setPlaybackDirection(PlaybackDirection playbackDirection)
{
	fPlaybackDirection = playbackDirection;
}

void Animation::pause()
{
	fIsPlaying = false;
}

void Animation::seek(float time)
{
	fCurrentTime = spoonie::clamp(time, 0.0f, fDuration);
}

void Animation::rewind()
{
	fCurrentTime = 0.0f;
	fTimeLastRun = std::chrono::steady_clock::now();
}

float Animation::getCurrentTime()
{
	return fCurrentTime;
}

float Animation::getDuration()
{
	return fDuration;
}

void Animation::setDuration(float duration)
{
	fDuration = duration;
}

bool Animation::isPlaying()
{
	return fIsPlaying;
}

void Animation::applyEasing()
{
	//TODO
}

FloatTransition::FloatTransition() : Animation(0.0f, noEasing)
{
}

//TODO: Make this more generic if possible
FloatTransition::FloatTransition(float duration, float *initialValue, float targetValue, EasingFunction easingFunction) : Animation(duration, easingFunction),
																														  fCurrentValue(initialValue),
																														  fInitialValue(*initialValue),
																														  fTargetValue(targetValue)
{
}

FloatTransition::~FloatTransition()
{
}

void FloatTransition::applyEasing()
{
}

void FloatTransition::run()
{
	if ((fPlaybackDirection == Forward && fCurrentTime >= fDuration) || (fPlaybackDirection == Backward && fCurrentTime <= 0.0f))
	{
		this->pause();
	}

	using namespace std::chrono;
	steady_clock::time_point now = steady_clock::now();

	float deltaTime = duration_cast<duration<float>>(now - fTimeLastRun).count();

	if (fPlaybackDirection == Forward)
		fCurrentTime = std::min(fDuration, fCurrentTime + deltaTime);
	else
		fCurrentTime = std::max(0.0f, fCurrentTime - deltaTime);

	fTimeLastRun = now;

	//Just some cheap lerp for now
	*fCurrentValue = spoonie::lerp(fInitialValue, fTargetValue, fCurrentTime / fDuration);
}

ColorTransition::ColorTransition(float duration, Color *initialColor, Color targetColor, EasingFunction easingFunction) : Animation(duration, easingFunction)
{
	fRgbaTransitions[0] = FloatTransition(duration, &initialColor->red, targetColor.red, easingFunction);
	fRgbaTransitions[1] = FloatTransition(duration, &initialColor->green, targetColor.green, easingFunction);
	fRgbaTransitions[2] = FloatTransition(duration, &initialColor->blue, targetColor.blue, easingFunction);
	fRgbaTransitions[3] = FloatTransition(duration, &initialColor->alpha, targetColor.alpha, easingFunction);
}

ColorTransition::~ColorTransition()
{
}

void ColorTransition::applyEasing()
{
}

void ColorTransition::onPlay()
{
	for (int i = 0; i < 4; ++i)
	{
		fRgbaTransitions[i].setDuration(fDuration);
		fRgbaTransitions[i].play(fPlaybackDirection);
	}
}

void ColorTransition::run()
{
	for (int i = 0; i < 4; ++i)
	{
		fRgbaTransitions[i].run();
	}

	if (!fRgbaTransitions[0].isPlaying())
		this->pause();
}

END_NAMESPACE_DISTRHO