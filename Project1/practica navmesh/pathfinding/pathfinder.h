#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <moaicore/MOAIEntity2D.h>
#include "../include/Path.h"

struct NavPolygon {
	struct Edge {
		int m_verts[2];
		int m_idNeighbour; //entero con el numero que indica el id del vecino,-1 la arista no tiene vecino
	};
	int id; //entero que indica la posicion en el array del nav mesh que ocupa este poligono, util para calcular heuristic
	std::vector<USVec2D> m_verts;
	std::vector<Edge> m_Edges;
	NavPolygon * father; //vecino del que viene
	int cost;
};

class Pathfinder : public virtual MOAIEntity2D
{
public:
	Pathfinder();
	~Pathfinder();

	typedef vector<NavPolygon> NavMesh;

	virtual void DrawDebug();

	const Path * GetPath() { return m_finalPath; }

	void SetStartPosition(float x, float y) {
		m_StartPosition = USVec2D(x, y);  
		UpdatePath();
		m_startPolygon = FindPolygon(x, y);
		UpdatePath();
	}

	void SetEndPosition(float x, float y) {
		m_EndPosition = USVec2D(x, y); 
		m_endPolygon = FindPolygon(x, y);
		UpdatePath();
	}

	void UpdatePath();
	const USVec2D& GetStartPosition() const { return m_StartPosition; }
	const USVec2D& GetEndPosition() const { return m_EndPosition; }
	bool PathfindStep();
private:
	NavPolygon * LessCost(const std::vector<NavPolygon*>& vector);
	void BuildPath(NavPolygon* pathNode);
	void SetAction(NavPolygon* node, NavPolygon * father, std::vector<NavPolygon*>& openedList, const std::vector<NavPolygon*>& closedList);
	NavPolygon FindPolygon(float x, float y);
private:
	USVec2D m_StartPosition;
	USVec2D m_EndPosition;
	Path * m_finalPath;
	NavMesh m_navMesh;
	NavPolygon m_startPolygon;
	NavPolygon m_endPolygon;
	bool m_pathFound;
	bool m_ejecutarUnaVez;
public:
	DECL_LUA_FACTORY(Pathfinder)
public:
	virtual void RegisterLuaFuncs(MOAILuaState& state);
private:
	bool ReadNavMesh();
	void DrawNavMesh();
	USVec2D HalfPoint(USVec2D x, USVec2D y);
	static int _setStartPosition(lua_State* L);
	static int _setEndPosition(lua_State* L);
	static int _pathfindStep(lua_State* L);
};


#endif