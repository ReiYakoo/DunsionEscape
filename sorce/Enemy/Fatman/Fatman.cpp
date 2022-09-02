#include "Fatman.h"
#include "../Enemy.h"

namespace Fatman {

	Enemy* Fatman::CreateFatman()
	{
		return new Enemy(new FatmanAI(), new FatmanPhysics(), new FatmanMMD());
	}

}