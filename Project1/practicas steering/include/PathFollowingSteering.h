#ifndef PATH_FOLLOWING_STEERING_H
#define PATH_FOLLOWING_STEERING_H

#include "ISteering.h"
class SeekSteering;
struct Acceleration;
class Path;

class PathFollowingSteering : public ISteering {
public:
	PathFollowingSteering(Path * path);
	~PathFollowingSteering();
	virtual Acceleration GetSteering(Character * character);
private:
	SeekSteering * mSeekSteering;
	Path * mPath;
};

#endif
