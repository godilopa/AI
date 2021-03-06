#include <stdafx.h>
#include "character.h"
#include "../include/Steerings.h"
#include <tinyxml.h>
#include <params.h>

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

	mPathfinder = new Pathfinder();
	mPathfinder->SetStartPosition(-400, 110);
	mPathfinder->SetEndPosition(20, 40);

	ISteering * alignToMovementSteering = new AlignToMovementSteering();
	mSteerings.emplace_back(alignToMovementSteering);
	ISteering * pathFollowigSteering = new PathFollowingSteering(&mParams.path);
	mSteerings.emplace_back(pathFollowigSteering);
}

void Character::OnStop()
{
	for (size_t i = 0; i < mSteerings.size(); i++) {
		delete mSteerings[i];
	}
}

void Character::OnUpdate(float step)
{
	((PathFollowingSteering *)mSteerings[1])->SetPath(mPathfinder->GetPath());
	mAcceleration.angular = mSteerings[0]->GetSteering(this).angular;
	mAcceleration.linear = mSteerings[1]->GetSteering(this).linear;

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

	mPathfinder->DrawDebug();

	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	MOAIDraw::DrawLine(GetLoc().mX, GetLoc().mY, GetLoc().mX + mLinearVelocity.mX, GetLoc().mY + mLinearVelocity.mY);

	gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.5f);
	MOAIDraw::DrawLine(GetLoc().mX, GetLoc().mY, GetLoc().mX + mAcceleration.linear.mX, GetLoc().mY + mAcceleration.linear.mY);
}


// Lua configuration

void Character::RegisterLuaFuncs(MOAILuaState& state)
{
	MOAIEntity2D::RegisterLuaFuncs(state);
	
	luaL_Reg regTable [] = {
		{ "setLinearVel",			_setLinearVel},
		{ "setAngularVel",			_setAngularVel},
		{ "setStartPosition",		_setStartPosition },
		{ "setEndPosition",			_setEndPosition },
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
}

int Character::_setStartPosition(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")

		float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetStartPosition(pX, pY);
	return 0;
}

int Character::_setEndPosition(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")

	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetEndPosition(pX, pY);
	return 0;
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
	