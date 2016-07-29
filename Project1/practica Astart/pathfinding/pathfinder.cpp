#include <stdafx.h>
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
		ColumsAndRows(m_columns, m_rows);
		m_widthProportion = 1000 / m_columns;
		m_heightProportion = 700 / m_rows;
		ReadGrid();
	}

	//Reiniciamos las variables para construir el path
	m_finalPath.path.clear();
	m_pathFound = false;
	DrawGrid();


	if (m_ejecutarUnaVez) {
		std::vector<PathNode*> openedList;
		std::vector<PathNode*> closedList;
		PathNode * nodeStart = new PathNode();
		nodeStart->x = m_StartPosition.mX;
		nodeStart->y = m_StartPosition.mY;
		nodeStart->father = nullptr;
		nodeStart->cost = 0;

		openedList.push_back(nodeStart);
		while (openedList.size() != 0) {
			PathNode * node = LessCost(openedList);
			auto it = std::find(openedList.begin(), openedList.end(), node);
			if (it != openedList.end()) openedList.erase(it);

			if (node->x == (int)m_EndPosition.mX && node->y == (int)m_EndPosition.mY) {
				BuildPath(node);
				return;
			}
			else {
				int x = node->x;
				int y = node->y;
				SetAction(x - 1, y - 1, node, openedList, closedList);
				SetAction(x, y - 1, node, openedList, closedList);
				SetAction(x + 1, y - 1, node, openedList, closedList);
				SetAction(x - 1, y, node, openedList, closedList);
				SetAction(x + 1, y, node, openedList, closedList);
				SetAction(x - 1, y + 1, node, openedList, closedList);
				SetAction(x, y + 1, node, openedList, closedList);
				SetAction(x + 1, y + 1, node, openedList, closedList);
			}

			closedList.push_back(node);
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

void Pathfinder::SetAction(int x, int y, PathNode* node, std::vector<PathNode*>& openedList, const std::vector<PathNode*>& closedList) {
	if (x >= 0 && y >= 0 && x < m_columns && y < m_rows) {
		//Si la celda vecina no esta cerrada
		if (m_map[x][y]) {
			for (int i = 0; i < closedList.size(); i++) { //Si la celda vecina no esta en la lista cerrada
				if (closedList[i]->x == x && closedList[i]->y == y) return;
			}

			for (int j = 0; j < openedList.size(); j++) { //Si la celda vecina esta en la lista abierta
				if (openedList[j]->x == x && openedList[j]->y == y) {
					if (openedList[j]->cost < node->cost) {
						openedList[j]->cost = node->cost; //Actualizamos coste
						return;
					}
				}
			}

			PathNode * next_node = new PathNode();//En otro caso lo añadimos a la lista abierta
			next_node->x = x;
			next_node->y = y;
			next_node->father = node;
			next_node->cost = node->cost + 1;
			openedList.push_back(next_node);
		}
	}
}


Pathfinder::PathNode * Pathfinder::LessCost(const std::vector<PathNode*>& vector){
	int returnIndexPath = 0;
	int heuristicCost;
	int minorHeuristicCost = abs(vector[0]->x - (int)m_EndPosition.mX) + abs(vector[0]->y - (int)m_EndPosition.mY);

	for (int i = 1; i < vector.size(); i++) {
		heuristicCost = abs(vector[i]->x - (int)m_EndPosition.mX) + abs(vector[i]->y - (int)m_EndPosition.mY);

		if (heuristicCost < minorHeuristicCost) {
			minorHeuristicCost = heuristicCost;
			returnIndexPath = i;
		}
	}

	return vector[returnIndexPath];
}


void Pathfinder::BuildPath(PathNode* pathNode){
	PathNode * currentPathNode = pathNode;

	while (currentPathNode->father) {
		m_finalPath.path.push_back({(float)currentPathNode->x, (float)currentPathNode->y});
		currentPathNode = currentPathNode->father;
	}

	m_finalPath.path.push_back({(float) currentPathNode->x, (float)currentPathNode->y });
	m_pathFound = true;
}

void Pathfinder::DrawDebug(){
	DrawGrid();
}

bool Pathfinder::PathfindStep(){
	// returns true if pathfinding process finished
	return true;
}

void Pathfinder::ColumsAndRows(int& columns, int& rows) {
	ifstream file("grid.txt");
	columns = 0;
	rows = 0;

	if (file.is_open()) {
		char ch;
		file.get(ch);

		while (!file.eof()) {
			while (ch != '\n' && !file.eof()) {
				file.get(ch);
				columns = columns + 1;
			}

			rows = rows + 1;
			file.get(ch);
		}
	}

	columns = columns / rows;
	file.close();
}

void Pathfinder::ReadGrid() {
	ifstream file("grid.txt");
	m_map = new bool*[m_columns];
	for (int i = 0; i < m_columns; i++) {
		m_map[i] = new bool[m_rows];
	}

	if (file.is_open()) {
		char ch;
		int i, j;
		i = j = 0;
		file.get(ch);

		while (!file.eof()) {
			while (ch != '\n' && !file.eof()) {
				if (ch == '.') {
					m_map[i][j] = true;
				}
				else {
					m_map[i][j] = false;
				}
				file.get(ch);
				i++;
			}
			i = 0;
			j++;
			file.get(ch);
		}
	}

	file.close();
}

void Pathfinder::DrawGrid(){
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

	for (int i = 1; i * m_heightProportion <= 1000; i++) {
		for (int j = 1; j * m_widthProportion <= 700; j++) {

			if (m_map[i-1][j-1] == true) {
				gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.5f);
			}else{
				gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
			}
			
			MOAIDraw::DrawRectFill(-500 + (i-1) * m_widthProportion, -350 + (j - 1) * m_heightProportion, -500 + i * m_widthProportion , -350 + j * m_heightProportion );
			gfxDevice.SetPenColor(0.0f, 0.0f, 0.0f, 0.5f);
			MOAIDraw::DrawRectOutline(-500 + (i - 1) * m_widthProportion, -350 + (j - 1) * m_heightProportion, -500 + i * m_widthProportion, -350 + j * m_heightProportion);
		}
	}

	if (m_pathFound) {
		for (int i = 0; i < m_finalPath.path.size(); i++) {
			DrawSquare(m_finalPath.path[i].mX, m_finalPath.path[i].mY);
		}
		
	}
}

void Pathfinder::DrawSquare(int x, int y) {
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);
	MOAIDraw::DrawRectFill(-500 + x * m_widthProportion, -350 + y * m_heightProportion, -500 + (x + 1) * m_widthProportion, -350 + (y + 1) * m_heightProportion);
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