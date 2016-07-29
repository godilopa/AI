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
	void SetPath(const Path * path){ mPath = path; }
private:
	SeekSteering * mSeekSteering;
	const Path * mPath;
};

#endif
