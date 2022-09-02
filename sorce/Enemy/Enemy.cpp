#include "Enemy.h"
#include "Component.h"

#include "../MAIN.h"

Enemy::Enemy(AI_Component* ai, Physics_Component* physics, Model_Component* model)
	:Mass_X(-1),
	Mass_Z(-1),
	Speed(0.0f),
	Pos(),
	RotateRadian(),
	pAI(ai),
	pPhysics(physics),
	pModel(model)
{

}
Enemy::~Enemy()
{
	delete (pAI);
	delete (pPhysics);
	delete (pModel);
}
void Enemy::Init(const MasterDataBase* pmdb)
{
	if (pAI) {
		pAI->Init(pmdb, this);
	}
	if (pPhysics) {
		pPhysics->Init(pmdb, this);
	}
	if (pModel) {
		pModel->Init(pmdb, this);
	}
}
void Enemy::Update(MasterDataBase* pmdb)
{
	if (pAI) {
		pAI->Update(pmdb, this);
	}
	if (pPhysics) {
		pPhysics->Update(pmdb, this);
	}
	if (pModel) {
		pModel->Update(pmdb, this);
	}
}
void Enemy::Draw(bool isAnimation)
{
	if (pModel) {
		pModel->Draw(this, isAnimation);
	}
}
void Enemy::SetPos(VECTOR pos)
{
	if (pos.x >= 0.0f && pos.z >= 0.0f) {
		Pos = pos;

		int x = (int)pos.x / (int)MASS_SIZE;
		int z = (int)pos.z / (int)MASS_SIZE;

		if (pos.x > (float)x * MASS_SIZE - 500.0f && pos.x < (float)x * MASS_SIZE + 500.0f) {
			Mass_X = (int)pos.x / (int)MASS_SIZE;
		}
		if (pos.z > (float)z * MASS_SIZE - 500.0f && pos.z < (float)z * MASS_SIZE + 500.0f) {
			Mass_Z = (int)pos.z / (int)MASS_SIZE;
		}
	}
}
void Enemy::SetPos(int x, int z)
{
	if (x >= 0 && z >= 0) {
		Mass_X = x;
		Mass_Z = z;

		Pos.x = (float)Mass_X * MASS_SIZE;
		Pos.z = (float)Mass_Z * MASS_SIZE;
	}
}
void Enemy::SetSpeed(float speed)
{
	Speed = speed;
}
void Enemy::SetRotateRadian(VECTOR rotate)
{
	RotateRadian = rotate;
}
void Enemy::GetPos(int* x, int* z)
{
	if (x) {
		*x = Mass_X;
	}
	if (z) {
		*z = Mass_Z;
	}
}
float Enemy::GetSpeed()
{
	return Speed;
}
VECTOR Enemy::GetPos()
{
	return Pos;
}
VECTOR Enemy::GetRotateRadian()
{
	return RotateRadian;
}