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
	vector<USVec2D>& GetPath() { return mPoints; }
	USVec2D GetNearestPathPosition(USVec2D position) const;
	USVec2D GetNextPathPosition(USVec2D position, float progress) const;
private:
	vector<USVec2D> mPoints;
	mutable int activeSegment;
};

#endif
