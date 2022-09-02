#include "Fatman.h"

#include "../Enemy.h"
#include "../../MMD/MMD.h"

namespace Fatman {

	FatmanMMD::FatmanMMD()
		:pModel(nullptr)
	{
		pModel = new Model::MMD("Models/Fatman/Fatman MMD.pmx");
	}
	FatmanMMD::~FatmanMMD()
	{

	}
	void FatmanMMD::Init(const MasterDataBase* pmdb, Enemy* pEnemy)
	{
		if (pModel) {
			pModel->SetAnimTime(0.0f);
			pModel->SetScale(FatmanParameter::FATMAN_SCALE);
			pModel->SetLoopTime(FatmanParameter::FATMAN_WALK_ANIM_LOOP_START,
								FatmanParameter::FATMAN_WALK_ANIM_LOOP_END);
		}
	}
	void FatmanMMD::Update(MasterDataBase* pmdb, Enemy* pEnemy)
	{
		if (pModel) {
			pModel->SetPos(pEnemy->GetPos());
			pModel->SetRotate(pEnemy->GetRotateRadian());
			pModel->Draw(true);
		}
	}
	void FatmanMMD::Draw(Enemy* pEnemy, bool isAnimation)
	{
		pModel->SetPos(pEnemy->GetPos());
		pModel->SetRotate(pEnemy->GetRotateRadian());
		pModel->Draw(isAnimation);
	}
}