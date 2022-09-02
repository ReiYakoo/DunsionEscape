#pragma once
#include "../MAIN.h"

class MapControll {

public:
	void CleateMap(MasterDataBase *pmdb);
	void DrawMap(MasterDataBase* pmdb);
	void MapReset(MasterDataBase *pmdb);

	float KeyAngleY = 0.0f;	//ƒJƒM‚ÌŒX‚«

};
