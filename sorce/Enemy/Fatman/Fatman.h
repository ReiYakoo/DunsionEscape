#pragma once
#include <vector>

#include "../DxLib/DxLib.h"
#include "../Component.h"

namespace Model {
	class MMD;
}

namespace Fatman {

	//敵を作成
	Enemy* CreateFatman();

	class FatmanParameter {
	public:
		constexpr static float FATMAN_SPEED					= 20.0f;	//Fatman　スピード
		constexpr static float FATMAN_WALK_ANIM_LOOP_START	= 87.0f;	//Fatman　歩きモーション、ループスタート時間
		constexpr static float FATMAN_WALK_ANIM_LOOP_END	= 127.0f;	//Fatman　歩きモーション、ループエンド時間

		//Fatman　スケール値
		//../DxLib/Dxlib.h の VECTOR(tagVECTOR) 内に 
		//constexpr tagVECTOR(x,y,z)　インストラクタを追加してください
		constexpr static VECTOR FATMAN_SCALE = VECTOR(30.0f, 30.0f, 40.0f);
	};

	//ノードの状態
	enum class NODE_STATE
	{
		EMPTY,		//実探索
		CLOSED,		//探索済
		OPEN,		//探索中(閲覧中）
		OBSTACLE,	//障害物(非通行)
		GOAL,		//目的地
	};

	//Aster で使うマップ（1マス単位）
	class Node {
	public:
		Node();

		int x;				//自分のマスx座標
		int z;				//自分のマスのマスx座標
		float score;		//自分の点数(低ければゴールに近い)
		NODE_STATE state;	//自分のマスの状態
		Node* parent;		//自分の親(何処から来たか)

		bool operator<(const Node& right);		//std::sort() 用関数
	};

	typedef unsigned long long ull;
	typedef std::vector<ull> v_ull;
	typedef std::vector<char> v_char;
	typedef std::vector<std::vector<char>> vv_char;
	typedef std::vector<Node> v_node;
	typedef std::vector<std::vector<Node>> vv_node;
	typedef std::vector<VECTOR> v_vector;

	//思考部分
	class FatmanAI : public AI_Component {	
	public:
		FatmanAI();
		~FatmanAI();

		void Init(const MasterDataBase* pmdb, Enemy* pEnemy);		//初期化
		void Update(MasterDataBase* pmdb, Enemy* pEnemy);			//更新

		int Astar_Recursive(int x, int z, int nowCost);				//Aster アルゴリズム
		void ResetNode();											//ノードのリセット

		void CreateRouteFromNode_Recursive(Node* pNode);			//pNode の親をたどってルート構築

		//視界内にいるプレイヤーまでのルート構築
		//false の場合、ルートは何も変わらない
		//視界内 : true / 視界外 : false
		bool CreateRouteFromLookPlayer(MasterDataBase* pmdb, Enemy* pEnemy);	

	private:
		int EnemyMass_X;		//敵現在地(マス)
		int EnemyMass_Z;		//敵現在地(マス)
		int DestinationMass_X;	//移動目的地(プレイヤーの座標/マス)
		int DestinationMass_Z;	//移動目的地(プレイヤーの座標/マス)

		bool IsPreLookPlayer;	//直前のフレームでプレイヤーが見えているか
		bool IsNowLookPlayer;	//現在のフレームプレイヤーが見えているか
		bool IsDoAstar;			//A*を行うか

		VECTOR EnemyPos;		//敵現在地(ワールド座標)

		v_vector RouteUpToDestination;		//目的地から決定された進行ルート
		vv_node MapNode;					//マップ情報から決定されたノード群
	};

	//物理系
	class FatmanPhysics : public Physics_Component {
	public:
		FatmanPhysics();
		~FatmanPhysics();

		void Init(const MasterDataBase* pmdb, Enemy* pEnemy);	//初期化
		void Update(MasterDataBase* pmdb, Enemy* pEnemy);		//更新
	};

	//描画/モデル関連
	class FatmanMMD : public Model_Component {
	public:
		FatmanMMD();
		~FatmanMMD();

		void Init(const MasterDataBase* pmdb, Enemy* pEnemy);	//初期化
		void Update(MasterDataBase* pmdb, Enemy* pEnemy);		//更新
		void Draw(Enemy* pEnemy, bool isAnimation);				//描画するだけの関数

	private:
		Model::MMD* pModel;			//モデルのインターフェース
	};
}