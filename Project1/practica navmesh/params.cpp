#include <stdafx.h>
#include <tinyxml.h>
#include "params.h"

bool ReadParams(const char* filename, Params& params)
{
    TiXmlDocument doc(filename);
    if (!doc.LoadFile())
    {
        fprintf(stderr, "Couldn't read params from %s", filename);
        return false;
    }

    TiXmlHandle hDoc(&doc);

    TiXmlElement* pElem;
    pElem = hDoc.FirstChildElement().Element();
    if (!pElem)
    {
        fprintf(stderr, "Invalid format for %s", filename);
        return false;
    }

    TiXmlHandle hRoot(pElem);
    TiXmlHandle hParams = hRoot.FirstChildElement("velparams");

    TiXmlElement* paramElem = hParams.FirstChildElement("max_velocity").Element();
    if (paramElem)
        paramElem->Attribute("value", &params.max_velocity);

    paramElem = hParams.FirstChildElement("max_acceleration").Element();
    if (paramElem)
        paramElem->Attribute("value", &params.max_acceleration);

    paramElem = hParams.FirstChildElement("dest_radius").Element();
    if (paramElem)
        paramElem->Attribute("value", &params.dest_radius);

	paramElem = hParams.FirstChildElement("arrive_radius").Element();
	if (paramElem)
		paramElem->Attribute("value", &params.arrive_radius);

    paramElem = hParams.FirstChildElement("targetPosition").Element();
    if (paramElem)
    {
        paramElem->Attribute("x", &params.targetPosition.mX);
        paramElem->Attribute("y", &params.targetPosition.mY);
    }

	hParams = hRoot.FirstChildElement("rotparams");

	paramElem = hParams.FirstChildElement("max_angular_velocity").Element();
	if (paramElem)
		paramElem->Attribute("value", &params.max_angular_velocity);

	paramElem = hParams.FirstChildElement("max_angular_acceleration").Element();
	if (paramElem)
		paramElem->Attribute("value", &params.max_angular_acceleration);

	paramElem = hParams.FirstChildElement("angular_dest_radius").Element();
	if (paramElem)
		paramElem->Attribute("value", &params.angular_dest_radius);

	paramElem = hParams.FirstChildElement("angular_arrive_radius").Element();
	if (paramElem)
		paramElem->Attribute("value", &params.angular_arrive_radius);

	paramElem = hParams.FirstChildElement("targetRotation").Element();
	if (paramElem)
		paramElem->Attribute("value", &params.targetRotation);

	//Rellenamos el path
	hParams = hRoot.FirstChildElement("path");
	paramElem = hParams.FirstChildElement("look_ahead").Element();

	if (paramElem)
		paramElem->Attribute("value", &params.lookAhead);

	TiXmlHandle pointsElem = hParams.FirstChildElement("points");
	paramElem = pointsElem.FirstChildElement("point").Element();
	while (paramElem){
		USVec2D point;
		paramElem->Attribute("x", &point.mX);
		paramElem->Attribute("y", &point.mY);
		params.path.AddPoint(point);
		paramElem = paramElem->NextSiblingElement();
	}

    return true;
}

bool ReadObstacles(const char* filename, Params& params)
{
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
	{
		fprintf(stderr, "Couldn't read params from %s", filename);
		return false;
	}

	TiXmlHandle hDoc(&doc);

	TiXmlElement* pElem;
	pElem = hDoc.FirstChildElement().Element();
	if (!pElem)
	{
		fprintf(stderr, "Invalid format for %s", filename);
		return false;
	}

	TiXmlHandle hRoot(pElem);
	TiXmlHandle hParams = hRoot.FirstChildElement("obstacles");
	TiXmlElement* paramElem = hParams.FirstChildElement("obstacle").Element();

	while (paramElem) {
		Obstacle obstacle;
		paramElem->Attribute("x", &obstacle.center.mX);
		paramElem->Attribute("y", &obstacle.center.mY);
		paramElem->Attribute("r", &obstacle.radius);
		params.obstacles.push_back(obstacle);
		paramElem = paramElem->NextSiblingElement();
	}



	return true;
}