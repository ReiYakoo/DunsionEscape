#include "Fatman.h"
#include "../Enemy.h"

namespace Fatman {

	FatmanPhysics::FatmanPhysics()
	{

	}
	FatmanPhysics::~FatmanPhysics()
	{

	}
	void FatmanPhysics::Init(const MasterDataBase* pmdb, Enemy* pEnemy)
	{
		pEnemy->SetSpeed(FatmanParameter::FATMAN_SPEED);
	}
	void FatmanPhysics::Update(MasterDataBase* pmdb, Enemy* pEnemy)
	{

	}
}