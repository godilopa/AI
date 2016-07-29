#include <stdafx.h>
#include "../include/Path.h"
#include <moaicore/MOAIEntity2D.h>

Path::Path(){
}

Path::~Path(){
}

void Path::DrawPath() {
	USVec2D pointA = mPoints[0];

	for (int i = 1; i < mPoints.size(); i++) {
		USVec2D pointB = mPoints[i];
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
		gfxDevice.SetPenColor(1.0f, 0.0f, 1.0f, 0.5f);
		MOAIDraw::DrawLine(pointA.mX, pointA.mY, pointB.mX, pointB.mY);
		pointA = pointB;
	}
}

/*Calculo la distancia al punto mas cercano de cada segmento
 devolviendo el mas cercano entre todos los segmentos */
USVec2D Path::GetNearestPathPosition(USVec2D position) const {
	USVec2D pointA = mPoints[0];
	float minorDistance = 1000000;
	float distance;
	USVec2D nearestPoint;

	for (int i = 1; i < mPoints.size(); i++) {
		USVec2D pointB = mPoints[i];
		float k = ((position.mX - pointA.mX)*(pointB.mX - pointA.mX) + (position.mY - pointA.mY)*(pointB.mY - pointA.mY))
			/ ((pointB.mX - pointA.mX)*(pointB.mX - pointA.mX) + (pointB.mY - pointA.mY) *(pointB.mY - pointA.mY));
		//Point is in Segment
		if (k < 1 && k > 0) {
			USVec2D v = pointA + (pointB - pointA) * k;
			distance = (v - position).LengthSquared();

			if (distance < minorDistance) {
				minorDistance = distance;
				nearestPoint = v;
				activeSegment = i;
			}
		}
		else if (k >= 1) { //point is B
			USVec2D v = pointB - position;
			distance = v.LengthSquared();

			if (distance < minorDistance) {
				minorDistance = distance;
				nearestPoint = pointB;
				activeSegment = i;
			}
		}
		else { //point is A
			USVec2D v = pointA - position;
			distance = v.LengthSquared();

			if (distance < minorDistance) {
				minorDistance = distance;
				nearestPoint = pointA;
				activeSegment = i;
			}
		}

		pointA = pointB;
	}

	return nearestPoint;
}

USVec2D Path::GetNextPathPosition(USVec2D position, float progress) const{
	USVec2D pointA = mPoints[activeSegment - 1];
	USVec2D pointB = mPoints[activeSegment];
	USVec2D dir = (pointB - pointA).NormSafe() * progress;
	float lengthToB = (pointB - position).Length();
	//Si el tamaño a avanzar es mayor de o que queda hasta B pasamos la siguiente segmento
	if (progress > lengthToB) {
		//Si estamos en el ultimo segmento no sumamos mas llegamos al ultimo punto
		if (activeSegment == mPoints.size() - 1) {
			return pointB;
		}
		else {
			float distanceOverlap = progress - lengthToB;
			//Cogemos el siguiente segmento y le sumamos lo que nos pasamos
			pointA = mPoints[activeSegment];
			pointB = mPoints[activeSegment + 1];
			dir = (pointB - pointA).NormSafe() * distanceOverlap;
			return pointA + dir;
		}
	}
	else {
		return position + dir;
	}
}