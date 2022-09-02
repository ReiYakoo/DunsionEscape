#pragma once
#include"../DxLib/DxLib.h"

class MasterDataBase;
class AI_Component;
class Physics_Component;
class Model_Component;

class Enemy {
public:
	Enemy(AI_Component* ai, Physics_Component* physics, Model_Component* model);
	~Enemy();

	void Init(const MasterDataBase* pmdb);
	void Update(MasterDataBase* pmdb);

	void Draw(bool isAnimation);

	void SetPos(VECTOR pos);
	void SetPos(int x, int z);
	void SetSpeed(float speed);
	void SetRotateRadian(VECTOR rotateRadian);

	void GetPos(int* x, int* z);
	float GetSpeed();
	VECTOR GetPos();
	VECTOR GetRotateRadian();

private:
	int Mass_X;
	int Mass_Z;

	float Speed;

	VECTOR Pos;
	VECTOR RotateRadian;
	
	AI_Component* pAI;
	Physics_Component* pPhysics;
	Model_Component* pModel;
};