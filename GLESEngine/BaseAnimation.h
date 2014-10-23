#ifndef BASEANIMATION_H
#define BASEANIMATION_H

#include "BaseObject.h"

class BaseObject;

class BaseAnimation
{
protected:
	float sDuration;
	bool isrevert;
	bool isrepeat;

public:
	BaseAnimation(float duration);

	virtual void doAnimation(BaseObject* object, float sElapsed) = 0;
	virtual void revert(BaseObject* object) = 0;

	inline float getsDuration() {
		return sDuration;
	}
	inline void setsDuration(float sDuration) {
		this->sDuration = sDuration;
	}
	inline  bool isRevert() {
		return isrevert;
	}
	inline void setRevert(bool isRevert) {
		isrevert = isRevert;
	}
	inline bool isRepeat() {
		return isrepeat;
	}
	inline void setRepeat(bool repeat) {
		isrepeat = repeat;
	}
};

#endif // !BASEANIMATION_H
