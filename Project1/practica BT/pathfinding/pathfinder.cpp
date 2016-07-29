#include <stdafx.h>
#include <tinyxml.h>
#include <fstream>
#include "pathfinder.h"
#include <vector>

Pathfinder::Pathfinder() : MOAIEntity2D(), m_ejecutarUnaVez(false), m_pathFound(false){
	RTTI_BEGIN
		RTTI_EXTEND(MOAIEntity2D)
	RTTI_END
}

Pathfinder::~Pathfinder(){

}

void Pathfinder::UpdatePath(){
	if (!m_ejecutarUnaVez) {
		ReadNavMesh();
		m_finalPath = new Path();
	}

	//Reiniciamos las variables para construir el path
	m_finalPath->GetPath().clear();
	m_pathFound = false;
	DrawNavMesh();

	if (m_ejecutarUnaVez) {
		std::vector<NavPolygon*> openedList;
		std::vector<NavPolygon*> closedList;
		NavPolygon * polygonStart = new NavPolygon();
		*polygonStart = m_startPolygon;
		polygonStart->father = nullptr;
		polygonStart->cost = 0;

		openedList.push_back(polygonStart);
		while (openedList.size() != 0) {
			NavPolygon * polygon = LessCost(openedList);
			auto it = std::find(openedList.begin(), openedList.end(), polygon);
			if (it != openedList.end()) openedList.erase(it);

			if (polygon->id == m_endPolygon.id) {
				BuildPath(polygon);
				return;
			}
			else {
				//Buscamos los nodos vecinos recorriendo las aristas
				for (int i = 0; i < polygon->m_Edges.size(); i++) {
					if (polygon->m_Edges[i].m_idNeighbour != -1) {
						SetAction(&m_navMesh[polygon->m_Edges[i].m_idNeighbour], polygon, openedList, closedList);
					}
				}
			}

			closedList.push_back(polygon);
		}
		//Borramos la lista de nodos abierta y cerrada
		for (int i = 0; i < closedList.size(); i++) { //Si la celda vecina no esta en la lista cerrada
			delete closedList[i];
		}

		for (int i = 0; i < openedList.size(); i++) { //Si la celda vecina no esta en la lista cerrada
			delete openedList[i];
		}
	}

	m_ejecutarUnaVez = true;
}

void Pathfinder::SetAction(NavPolygon * polygon, NavPolygon * father, std::vector<NavPolygon*>& openedList, const std::vector<NavPolygon*>& closedList) {
	for (int i = 0; i < closedList.size(); i++) { //Si la celda vecina no esta en la lista cerrada
		if (polygon->id == closedList[i]->id) return;
	}

	for (int i = 0; i < openedList.size(); i++) { //Si la celda vecina esta en la lista abierta
		if (polygon->id == openedList[i]->id) {
			if (openedList[i]->cost < polygon->cost) {
				openedList[i]->cost = polygon->cost; //Actualizamos coste
				return;
			}
		}
	}

	NavPolygon * next_polygon= new NavPolygon(); //En otro caso lo añadimos a la lista abierta
	*next_polygon = *polygon;
	next_polygon->father = father;
	next_polygon->cost = father->cost + 1;
	openedList.push_back(next_polygon);
}

NavPolygon Pathfinder::FindPolygon(float x, float y){
	NavPolygon polygon;
	for (int i = 0; i < m_navMesh.size(); i++) {
		int numVert = m_navMesh[i].m_verts.size();
		for (int j = 0; j < numVert; j++){
			USVec2D edge = m_navMesh[i].m_verts[j % numVert] - m_navMesh[i].m_verts[((numVert - 1) + j)%numVert];
			USVec2D point = { x, y };
			USVec2D vertToPoint = point - m_navMesh[i].m_verts[((numVert - 1) + j) % numVert];
			float crossProduct2D = edge.mX * vertToPoint.mY - edge.mY * vertToPoint.mX;

			if (crossProduct2D <= 0) {
				break;
			}
			//Si ninguno se sale esta dentro del poligono, devolver poligono
			if (j >= numVert - 1) {
				return m_navMesh[i];
			}
		}
	}

	return NavPolygon();
}

NavPolygon * Pathfinder::LessCost(const std::vector<NavPolygon*>& vector){
	int returnIndexPath = 0;
	int heuristicCost;
	int minorHeuristicCost = 1000000;
	//Heuristica: distancia al poligono final segun id (lugar que ocupa en el array)
	for (int i = 0; i < vector.size(); i++) {
		heuristicCost = abs(vector[i]->id- m_endPolygon.id);

		if (heuristicCost < minorHeuristicCost) {
			minorHeuristicCost = heuristicCost;
			returnIndexPath = i;
		}
	}

	return vector[returnIndexPath];
}

USVec2D Pathfinder::HalfPoint(USVec2D x, USVec2D y) {
	return (x + y) / 2;
}

void Pathfinder::BuildPath(NavPolygon* polygon){
	//El primer punto a añadir en el path es el endPoint
	m_finalPath->AddPoint(m_EndPosition);

	NavPolygon * currentPolygon = polygon;

	while (currentPolygon->father) {
		//Buscamos de que lado a llegado comparando el id del lado con el id del padre
		for (int i = 0; i < currentPolygon->m_Edges.size(); i++) {
			if ((currentPolygon->father)->id == currentPolygon->m_Edges[i].m_idNeighbour) {
				//Añadimos al path el punto medio de ese lado
				m_finalPath->AddPoint(HalfPoint(currentPolygon->m_verts[currentPolygon->m_Edges[i].m_verts[0]],
					currentPolygon->m_verts[currentPolygon->m_Edges[i].m_verts[1]]));
			}
		}
		currentPolygon = currentPolygon->father;
	}

	//El primer punto a añadiren el path es el startPoint
	m_finalPath->AddPoint(m_StartPosition);
	m_pathFound = true;
}

void Pathfinder::DrawDebug(){
	DrawNavMesh();
}

bool Pathfinder::PathfindStep(){
	// returns true if pathfinding process finished
	return true;
}


bool Pathfinder::ReadNavMesh() {
	TiXmlDocument doc("navmesh.xml");
	if (!doc.LoadFile())
	{
		fprintf(stderr, "Couldn't read params from %s", "navmesh.xml");
		return false;
	}

	TiXmlHandle hDoc(&doc);

	TiXmlElement* pElem;
	pElem = hDoc.FirstChildElement().Element();
	if (!pElem)
	{
		fprintf(stderr, "Invalid format for %s", "navmesh.xml");
		return false;
	}

	TiXmlHandle hRoot(pElem);
	TiXmlHandle hPolygons = hRoot.FirstChildElement("polygons");

	TiXmlElement* polygonElem = hPolygons.FirstChildElement("polygon").Element();
	int id = 0;

	while (polygonElem) {
		TiXmlElement* pointElem = polygonElem->FirstChildElement("point");
		NavPolygon polygon;
		int v = 0;

		while (pointElem) {
			USVec2D vert;
			NavPolygon::Edge edge;

			pointElem->Attribute("x", &vert.mX);
			pointElem->Attribute("y", &vert.mY);
			edge.m_verts[0] = v;
			edge.m_verts[1] = v+1;
			edge.m_idNeighbour = -1; //-1 no tiene conexion

			polygon.m_Edges.push_back(edge);
			polygon.m_verts.push_back(vert);
			v++;
			pointElem = pointElem->NextSiblingElement();
		}

		//Al salir el ultimo vertice del ultimo edge debe ser 0
		polygon.m_Edges[polygon.m_Edges.size()-1].m_verts[1] = 0;
		polygon.id = id;
		m_navMesh.push_back(polygon);
		id++;
		polygonElem = polygonElem->NextSiblingElement();
	}

	TiXmlHandle hLinks = hRoot.FirstChildElement("links");

	TiXmlElement* linkElem = hLinks.FirstChildElement("link").Element();

	while (linkElem) {
		TiXmlElement* startElem = linkElem->FirstChildElement("start");
		TiXmlElement* endElem = linkElem->FirstChildElement("end");

		int polygon1Number, edge1Number;
		startElem->Attribute("polygon", &polygon1Number);
		startElem->Attribute("edgestart", &edge1Number);

		int polygon2Number, edge2Number;
		endElem->Attribute("polygon", &polygon2Number);
		endElem->Attribute("edgestart", &edge2Number);
		//Se añaden de vecinos a sus edges respectivos
		m_navMesh[polygon1Number].m_Edges[edge1Number].m_idNeighbour = polygon2Number;
		m_navMesh[polygon2Number].m_Edges[edge2Number].m_idNeighbour = polygon1Number;

		linkElem = linkElem->NextSiblingElement();
	}
}

void Pathfinder::DrawNavMesh(){
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);
	//Draw nav mesh
	for (int i = 0; i < m_navMesh.size(); i++) {
		MOAIDraw::DrawPolygonFilled(m_navMesh[i].m_verts);
	}
	//Draw start and end
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	MOAIDraw::DrawPolygonFilled(m_startPolygon.m_verts);
	MOAIDraw::DrawPolygonFilled(m_endPolygon.m_verts);

	if (m_pathFound) {
		m_finalPath->DrawPath();
	}
}




//lua configuration ----------------------------------------------------------------//
void Pathfinder::RegisterLuaFuncs(MOAILuaState& state)
{
	MOAIEntity::RegisterLuaFuncs(state);

	luaL_Reg regTable[] = {
		{ "setStartPosition",		_setStartPosition },
		{ "setEndPosition",			_setEndPosition },
		{ "pathfindStep",           _pathfindStep },
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
}

int Pathfinder::_setStartPosition(lua_State* L)
{
	MOAI_LUA_SETUP(Pathfinder, "U")

		float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetStartPosition(pX, pY);
	return 0;
}

int Pathfinder::_setEndPosition(lua_State* L)
{
	MOAI_LUA_SETUP(Pathfinder, "U")

		float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetEndPosition(pX, pY);
	return 0;
}

int Pathfinder::_pathfindStep(lua_State* L)
{
	MOAI_LUA_SETUP(Pathfinder, "U")

		self->PathfindStep();
	return 0;
}