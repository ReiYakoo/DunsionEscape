#pragma once

class Enemy;
class MasterDataBase;

class AI_Component {
public:
	virtual ~AI_Component() {};

	virtual void Init(const MasterDataBase* pmdb, Enemy* pEnemy) = 0;
	virtual void Update(MasterDataBase* pmdb, Enemy* pEnemy) = 0;
};

class Physics_Component {
public:
	virtual ~Physics_Component() {};

	virtual void Init(const MasterDataBase* pmdb, Enemy* pEnemy) = 0;
	virtual void Update(MasterDataBase* pmdb, Enemy* pEnemy) = 0;
};

class Model_Component {
public:
	virtual ~Model_Component() {};

	virtual void Init(const MasterDataBase* pmdb, Enemy* pEnemy) = 0;
	virtual void Update(MasterDataBase* pmdb, Enemy* pEnemy) = 0;

	virtual void Draw(Enemy* pEnemy, bool isAnimation) = 0;
};