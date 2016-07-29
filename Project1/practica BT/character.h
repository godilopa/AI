#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <moaicore/MOAIEntity2D.h>
#include "pathfinding/pathfinder.h"
#include <params.h>
class ISteering;
class AlignSteering;
class AlignToMovementSteering;
class EnemyBT;

struct Acceleration {
	USVec2D linear;
	float angular;
};

class Character: public MOAIEntity2D
{
public:
    DECL_LUA_FACTORY(Character)
protected:
	virtual void OnStart();
	virtual void OnStop();
	virtual void OnUpdate(float step);
public:
	virtual void DrawDebug();

	Character();
	~Character();
	
	void SetLinearVelocity(float x, float y) { mLinearVelocity.mX = x; mLinearVelocity.mY = y;}
	void SetAngularVelocity(float angle) { mAngularVelocity = angle;}
	void SetMaxVel(float value) { mParams.max_velocity = value; }
	void SetMaxAcc(float value) { mParams.max_acceleration = value; }

	void SetStartPosition(float x, float y) {
		//mPathfinder->SetStartPosition(x, y);
	}

	void SetEndPosition(float x, float y) {
		//mPathfinder->SetEndPosition(x, y);
	}
	
	void ReduceHealth(float value) { mLife -= value; }
	USVec2D GetLinearVelocity() const { return mLinearVelocity;}
	float GetAngularVelocity() const { return mAngularVelocity;}
	float GetLife() const { return mLife; }

	Params GetParams() const { return mParams; }
	Params& GetParams() { return mParams; }
	void SetTargetRotation(float rotation) { mParams.targetRotation = rotation; }
	void SetTargetPosition(USVec2D position) { mParams.targetPosition = position; }
private:
	vector<ISteering *> mSteerings;
	Acceleration mAcceleration;
	USVec2D mLinearVelocity;
	float mAngularVelocity;
	float mLife;
	Params mParams;
	EnemyBT * mEnemyBT;
	// Lua configuration
public:
	virtual void RegisterLuaFuncs(MOAILuaState& state);
private:
	static int _setLinearVel(lua_State* L);
	static int _setAngularVel(lua_State* L);
	static int _setStartPosition(lua_State* L);
	static int _setEndPosition(lua_State* L);
	static int _reduceHealth(lua_State* L);
};

#endif