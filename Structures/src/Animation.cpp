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

//TODO: Make this more generic if possible
SizeChangeAnimation::SizeChangeAnimation(float duration, Widget *widget, Size<uint> targetSize, EasingFunction easingFunction) : Animation(duration, easingFunction),
																																 fWidget(widget),
																																 fSourceSize(widget->getSize()),
																																 fTargetSize(targetSize)
{
}

SizeChangeAnimation::~SizeChangeAnimation()
{
}

void SizeChangeAnimation::applyEasing()
{
}

void SizeChangeAnimation::run()
{
	using namespace std::chrono;
	steady_clock::time_point now = steady_clock::now();

	float deltaTime = duration_cast<duration<float>>(now - fTimeLastRun).count();

	if (fPlaybackDirection == Forward)
		fCurrentTime = std::min(fDuration, fCurrentTime + deltaTime);
	else
		fCurrentTime = std::max(0.0f, fCurrentTime - deltaTime);

	fTimeLastRun = now;

	//Just some cheap lerp for now
	fWidget->setWidth(spoonie::lerp(fSourceSize.getWidth(), fTargetSize.getWidth(), fCurrentTime / fDuration));
	fWidget->setHeight(spoonie::lerp(fSourceSize.getHeight(), fTargetSize.getHeight(), fCurrentTime / fDuration));

	if ((fPlaybackDirection == Forward && fCurrentTime == fDuration) || (fPlaybackDirection == Backward && fCurrentTime == 0.0f))
	{
		this->pause();
	}
}

END_NAMESPACE_DISTRHO