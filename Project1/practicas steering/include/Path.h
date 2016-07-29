#ifndef PATH_H
#define PATH_H

#include <vector>
using namespace std;

class Path {
public:
	Path();
	~Path();
	void AddPoint(USVec2D point) { mPoints.push_back(point); }
	void DrawPath();
	USVec2D GetNearestPathPosition(USVec2D position);
	USVec2D GetNextPathPosition(USVec2D position, float progress);
private:
	vector<USVec2D> mPoints;
	int activeSegment;
};

#endif
