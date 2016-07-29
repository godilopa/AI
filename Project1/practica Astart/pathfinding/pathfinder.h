#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <moaicore/MOAIEntity2D.h>

class Pathfinder : public virtual MOAIEntity2D
{
public:
	Pathfinder();
	~Pathfinder();

	struct PathNode {
		int x;
		int y;
		PathNode * father;
		int cost;
	};

	struct Path {
		vector<USVec2D> path;
	};

	virtual void DrawDebug();

	void SetStartPosition(float x, float y) {
		m_StartPosition = USVec2D(x, y);  
		m_StartPosition.mX = (m_StartPosition.mX + 500) / m_widthProportion;
		m_StartPosition.mY = (m_StartPosition.mY + 350) / m_heightProportion;

		if (m_ejecutarUnaVez && m_map[(int)m_StartPosition.mX][(int)m_StartPosition.mY] == false) {
			m_StartPosition.mX = 0;
			m_StartPosition.mY = 0;
		}

		UpdatePath(); 
	}

	void SetEndPosition(float x, float y) {
		m_EndPosition = USVec2D(x, y); 
		m_EndPosition.mX = (m_EndPosition.mX + 500) / m_widthProportion;
		m_EndPosition.mY = (m_EndPosition.mY + 350) / m_heightProportion;

		if (m_ejecutarUnaVez && m_map[(int)m_EndPosition.mX][(int)m_EndPosition.mY] == false) {
			m_EndPosition.mX = 0;
			m_EndPosition.mY = 0;
		}

		UpdatePath();
	}

	const USVec2D& GetStartPosition() const { return m_StartPosition; }
	const USVec2D& GetEndPosition() const { return m_EndPosition; }
	bool PathfindStep();
private:
	void UpdatePath();
	void DrawSquare(int x, int y);
	PathNode * LessCost(const std::vector<PathNode*>& vector);
	void BuildPath(PathNode* pathNode);
	void SetAction(int x, int y, PathNode* node, std::vector<PathNode*>& openedList, const std::vector<PathNode*>& closedList);
private:
	USVec2D m_StartPosition;
	USVec2D m_EndPosition;
	Path m_finalPath;
	bool m_pathFound;
	bool ** m_map;
	bool m_ejecutarUnaVez;
	int m_columns, m_rows;
	int m_widthProportion, m_heightProportion;
public:
	DECL_LUA_FACTORY(Pathfinder)
public:
	virtual void RegisterLuaFuncs(MOAILuaState& state);
private:
	void ReadGrid();
	void DrawGrid();
	void ColumsAndRows(int& columns, int& rows);
	static int _setStartPosition(lua_State* L);
	static int _setEndPosition(lua_State* L);
	static int _pathfindStep(lua_State* L);
};


#endif