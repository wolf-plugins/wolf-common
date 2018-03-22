#include "Animation.hpp"
#include "Color.hpp"
#include "src/DistrhoDefines.h"
#include "Mathf.hpp"
#include "Geometry.hpp"
#include "Widget.hpp"

START_NAMESPACE_DISTRHO

Animation::Animation(float duration, EasingFunction easingFunction) : fDuration(duration),
																	  fEasingFunction(easingFunction)
{
}

void Animation::play()
{
	fIsRunning = true;
}

void Animation::pause()
{
	fIsRunning = false;
}

void Animation::rewind()
{
	fCurrentTime = 0.0f;
}

bool Animation::isRunning()
{
	return fIsRunning;
}

//TODO: Make this more generic if possible
SizeChangeAnimation::SizeChangeAnimation(float duration, Widget *widget, Size<uint> targetSize, EasingFunction easingFunction) : Animation(duration, easingFunction),
																																 fWidget(widget),
																																 fSourceSize(widget->getSize()),
																																 fTargetSize(targetSize)
{
}

void SizeChangeAnimation::run()
{
	fCurrentTime = std::min(fDuration, fCurrentTime + 1);

	//Just some cheap lerp for now
	fWidget->setWidth(spoonie::lerp(fSourceSize.getWidth(), fTargetSize.getWidth(), fCurrentTime / fDuration * 100.0f));
	fWidget->setHeight(spoonie::lerp(fSourceSize.getHeight(), fTargetSize.getHeight(), fCurrentTime / fDuration * 100.0f));

	if (fCurrentTime == fDuration)
		this->pause();
}

END_NAMESPACE_DISTRHO