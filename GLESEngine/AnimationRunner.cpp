#include "AnimationRunner.h"

AnimationRunner::AnimationRunner(BaseObject* object, BaseAnimation* animation) {
	this->object = object;
	this->animation = animation;
	run = true;
	totalSElapsed = 0;
}

void AnimationRunner::runAnimation(double sElapsed) {
	if (run) {
		totalSElapsed += sElapsed;
		if (totalSElapsed > animation->getsDuration()) {
			if (animation->isRepeat()) {
				totalSElapsed = 0;
			} else {
				run = false;
				totalSElapsed = animation->getsDuration();
				if (animation->isRevert()) {
					animation->revert(object);
					return;
				}
			}
		}
		animation->doAnimation(object, totalSElapsed);
	}
}