#include <stdafx.h>
#include "character.h"
#include "../include/Steerings.h"
#include <tinyxml.h>
#include <params.h>

//#define SEEKSTEERING
//#define ARRIVESTEERING
//#define ALIGNSTEERING
//#define ALIGNTOMOVEMENTSTEERING
#define PATHFOLLOWING
//#define OBSTACLEAVOIDANCE
//#define PATHAVOIDANCE

Character::Character() : mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f)
{
	RTTI_BEGIN
		RTTI_EXTEND(MOAIEntity2D)
	RTTI_END
	mAcceleration.angular = 0.0f;
	mAcceleration.linear = {0.0f, 0.0f};
}

Character::~Character()
{

}

void Character::OnStart()
{
    ReadParams("params.xml", mParams);
	ReadObstacles("obstacles.xml", mParams);

	#ifdef SEEKSTEERING
		ISteering * seekSteering = new SeekSteering();
		mSteerings.emplace_back(seekSteering);
	#endif

	#ifdef ARRIVESTEERING
			ISteering * arriveSteering = new ArriveSteering();
			mSteerings.emplace_back(arriveSteering);
	#endif

	#ifdef ALIGNSTEERING
			ISteering * alignSteering = new AlignSteering();
			mSteerings.emplace_back(alignSteering);
	#endif

	#ifdef ALIGNTOMOVEMENTSTEERING
			ISteering * arriveSteering = new ArriveSteering();
			mSteerings.emplace_back(arriveSteering);
			ISteering * alignToMovementSteering = new AlignToMovementSteering();
			mSteerings.emplace_back(alignToMovementSteering);
	#endif

	#ifdef PATHFOLLOWING
			ISteering * pathFollowigSteering = new PathFollowingSteering(&mParams.path);
			mSteerings.emplace_back(pathFollowigSteering);
			ISteering * alignToMovementSteering = new AlignToMovementSteering();
			mSteerings.emplace_back(alignToMovementSteering);
	#endif

	#ifdef OBSTACLEAVOIDANCE
			ISteering * obstacleAvoidanceSteering = new ObstacleAvoidanceSteering();
			mSteerings.emplace_back(obstacleAvoidanceSteering);
	#endif

	#ifdef PATHAVOIDANCE
			ISteering * pathFollowigSteering = new PathFollowingSteering(&mParams.path);
			ISteering * obstacleAvoidanceSteering = new ObstacleAvoidanceSteering();
			ISteering * blendPFOA = new BlendPFOA((PathFollowingSteering *)pathFollowigSteering,
				(ObstacleAvoidanceSteering *)obstacleAvoidanceSteering, 1, 4);
			mSteerings.emplace_back(blendPFOA);
			ISteering * alignToMovementSteering = new AlignToMovementSteering();
			mSteerings.emplace_back(alignToMovementSteering);
	#endif
}

void Character::OnStop()
{
	for (size_t i = 0; i < mSteerings.size(); i++) {
		delete mSteerings[i];
	}
}

void Character::OnUpdate(float step)
{
	#ifdef SEEKSTEERING
		mAcceleration.linear = mSteerings[0]->GetSteering(this).linear;
	#endif

	#ifdef ARRIVESTEERING
			mAcceleration.linear = mSteerings[0]->GetSteering(this).linear;
	#endif

	#ifdef ALIGNSTEERING
			mAcceleration.angular = mSteerings[0]->GetSteering(this).angular;
			mLinearVelocity = { 0 ,0 };
	#endif

	#ifdef ALIGNTOMOVEMENTSTEERING
			mAcceleration.angular = mSteerings[1]->GetSteering(this).angular;
			mAcceleration.linear = mSteerings[0]->GetSteering(this).linear;
	#endif

	#ifdef PATHFOLLOWING
			mAcceleration.angular = mSteerings[1]->GetSteering(this).angular;
			mAcceleration.linear = mSteerings[0]->GetSteering(this).linear;
	#endif

	#ifdef OBSTACLEAVOIDANCE
				mAcceleration.linear = mSteerings[0]->GetSteering(this).linear;
	#endif

	#ifdef PATHAVOIDANCE
				mAcceleration.angular = mSteerings[1]->GetSteering(this).angular;
				mAcceleration.linear = mSteerings[0]->GetSteering(this).linear;
	#endif


	//mAcceleration.linear = mSteerings[1]->GetSteering(this).linear;
	//mAcceleration.angular = mSteerings[2]->GetSteering(this).angular;
	//mAcceleration.angular = mSteerings[3]->GetSteering(this).angular;
	//mAcceleration.linear = mSteerings[4]->GetSteering(this).linear;
	//mAcceleration.angular = mSteerings[3]->GetSteering(this).angular;
	//mAcceleration.linear = mSteerings[5]->GetSteering(this).linear;
	//mAcceleration.linear = mSteerings[6]->GetSteering(this).linear;

	mLinearVelocity += mAcceleration.linear * step;
	mAngularVelocity += mAcceleration.angular * step;

	//Ajustar la velocidad lineal maxima
	if (mLinearVelocity.Length() > mParams.max_velocity) {
		mLinearVelocity = mLinearVelocity.Norm() * mParams.max_velocity;
	}
	//Ajustar la velocidad angular maxima
	if (mAngularVelocity > mParams.max_angular_velocity) {
		mAngularVelocity = mParams.max_angular_velocity;
	}

	SetLoc(GetLoc() + mLinearVelocity * step);
	SetRot(GetRot() + mAngularVelocity * step);
}

void Character::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);
	gfxDevice.SetPointSize(5);

	MOAIDraw::DrawPoint(mParams.targetPosition.mX, mParams.targetPosition.mY);

	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	MOAIDraw::DrawLine(GetLoc().mX, GetLoc().mY, GetLoc().mX + mLinearVelocity.mX, GetLoc().mY + mLinearVelocity.mY);

	gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.5f);
	MOAIDraw::DrawLine(GetLoc().mX, GetLoc().mY, GetLoc().mX + mAcceleration.linear.mX, GetLoc().mY + mAcceleration.linear.mY);

	#ifdef PATHFOLLOWING
		mParams.path.DrawPath();
	#endif

	#ifdef OBSTACLEAVOIDANCE
		for (std::list<Obstacle>::iterator it = mParams.obstacles.begin(); it != mParams.obstacles.end(); ++it) {
			gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 0.5f);
			MOAIDraw::DrawEllipseFill(it->center.mX, it->center.mY, it->radius, it->radius, 100);
		}
	#endif

	#ifdef PATHAVOIDANCE
		mParams.path.DrawPath();
		for (std::list<Obstacle>::iterator it = mParams.obstacles.begin(); it != mParams.obstacles.end(); ++it) {
			gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 0.5f);
			MOAIDraw::DrawEllipseFill(it->center.mX, it->center.mY, it->radius, it->radius, 100);
		}
	#endif
}


// Lua configuration

void Character::RegisterLuaFuncs(MOAILuaState& state)
{
	MOAIEntity2D::RegisterLuaFuncs(state);
	
	luaL_Reg regTable [] = {
		{ "setLinearVel",			_setLinearVel},
		{ "setAngularVel",			_setAngularVel},
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
}

int Character::_setLinearVel(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")
	
	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetLinearVelocity(pX, pY);
	return 0;	
}

int Character::_setAngularVel(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")
	
	float angle = state.GetValue<float>(2, 0.0f);
	self->SetAngularVelocity(angle);

	return 0;
}
	