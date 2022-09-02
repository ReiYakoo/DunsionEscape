#include <algorithm>

#include "Fatman.h"
#include "../Enemy.h"
#include "../../MAIN.h"

#define root2 (1.41421356f)
#define _1RAD (57.2957795f)
#define ANGLE_TO_RADIAN( x ) { x / _1RAD };
#define MIN(x, y) { x < y ? (x) : (y) };

const ull ullx1 = (ull)1;	//ull型の1

namespace Fatman {

	Node::Node()
		:x(-1),
		z(-1),
		score(0.0f),
		state(NODE_STATE::EMPTY),
		parent(nullptr)
	{

	}
	bool Node::operator<(const Node& right)
	{
		return (this->score < right.score);
	}

	FatmanAI::FatmanAI()
		:EnemyMass_X(1),
		EnemyMass_Z(1),
		DestinationMass_X(1),
		DestinationMass_Z(1),
		EnemyPos(),
		IsPreLookPlayer(true),
		IsNowLookPlayer(true),
		IsDoAstar(true)
	{

	}
	FatmanAI::~FatmanAI()
	{

	}
	void FatmanAI::Init(const MasterDataBase* pmdb, Enemy* pEnemy)
	{
		Node node;
		MapNode.clear();

		//読み込まれたマップデータからノードを作成
		for (int i = 0; i < MASS_MAX_Z; i++) {
			v_node rowNode;
			for (int j = 0; j < MASS_MAX_X; j++) {
				if (pmdb->MapData[i][j].NowData == MASS_WALL) {
					node.state = NODE_STATE::OBSTACLE;
				}
				else {
					node.state = NODE_STATE::EMPTY;
				}
				node.x = j;
				node.z = i;

				rowNode.push_back(node);
			}

			MapNode.push_back(rowNode);
		}
	}
	void FatmanAI::Update(MasterDataBase* pmdb, Enemy* pEnemy)
	{
		//座標取得
		EnemyPos = pEnemy->GetPos();
		pEnemy->GetPos(&EnemyMass_X, &EnemyMass_Z);

		//目標地点を設定
		DestinationMass_X = pmdb->PlayerMASS_X;
		DestinationMass_Z = pmdb->PlayerMASS_Z;

		//視界内にプレイヤーを発見しているか判定
		IsPreLookPlayer = IsNowLookPlayer;
		IsNowLookPlayer = CreateRouteFromLookPlayer(pmdb, pEnemy);

		//プレイヤーを見失ったら A*
		//A*をする必要がある時は IsDoAstar = true　すること
		if ((IsPreLookPlayer && !IsNowLookPlayer) || IsDoAstar) {
			ResetNode();
			if (MapNode[EnemyMass_Z][EnemyMass_X].state != NODE_STATE::GOAL) {
				Astar_Recursive(EnemyMass_X, EnemyMass_Z, 1);
				pEnemy->SetPos(VGet((float)EnemyMass_X * MASS_SIZE, 0.0f, (float)EnemyMass_Z * MASS_SIZE));
				IsDoAstar = false;
			}
		}

		int num = RouteUpToDestination.size();
		if (num > 0) {
			int index = num - 1;

			//進行方向を求める
			float dir_x = RouteUpToDestination[index].x - EnemyPos.x;
			float dir_z = RouteUpToDestination[index].z - EnemyPos.z;

			VECTOR moveDir	= VGet(dir_x, 0.0f, dir_z);
			moveDir			= VNorm(moveDir);
			EnemyPos		= VAdd(EnemyPos, VScale(moveDir, pEnemy->GetSpeed()));
			pEnemy->SetPos(EnemyPos);

			//進行方向から回転角度を求め回転
			VECTOR initFatmanDir = VGet(0.0f, 0.0f, -1.0f);
			float dot = VDot(initFatmanDir, moveDir);
			float rad = acosf(dot);

			if (moveDir.x > 0.0f) {		//180度を過ぎたら
				VECTOR dir = VGet(-moveDir.x, 0.0f, -moveDir.z);
				dot = VDot(initFatmanDir, dir);
				rad = DX_PI_F + acosf(dot);
			}

			VECTOR rotate = VGet(0, 0, 0);
			rotate.y = rad;
			pEnemy->SetRotateRadian(rotate);

			//大体の目的地に近づいたら
			//次の目的地に移行
			float RangeUpToDestination = VSize(VSub(RouteUpToDestination[index], EnemyPos));
			if (RangeUpToDestination < pEnemy->GetSpeed() + 6.0f) {
				EnemyPos = RouteUpToDestination[index];
				pEnemy->SetPos(EnemyPos);
				RouteUpToDestination.pop_back();
			}
		}
		else {	//行き先が無くなったら A* 準備
			IsDoAstar = true;
		}
	}
	int FatmanAI::Astar_Recursive(int x, int z, int nowCost)
	{
		MapNode[z][x].state = NODE_STATE::CLOSED;

		int result = 1;
		ull flag = 0;
		v_node openNode;

		//上横のノードをOpen
		//上、右、下、左の順に検査
		for (int i = 0; i < 4; i++) {
			int next_x = x + (2 - i) % 2;
			int next_z = z + -((1 - i) % 2);
			if (next_x != -1 && next_z != -1) {

				if (MapNode[next_z][next_x].state == NODE_STATE::GOAL) {
					RouteUpToDestination.clear();
					MapNode[EnemyMass_Z][EnemyMass_X].parent = nullptr;
					MapNode[next_z][next_x].parent = &MapNode[z][x];
					CreateRouteFromNode_Recursive(&MapNode[next_z][next_x]);
					return 0;
				}
				else if (MapNode[next_z][next_x].state == NODE_STATE::EMPTY) {
					MapNode[next_z][next_x].state = NODE_STATE::OPEN;

					float heuristic = 0;
					int dif_x = DestinationMass_X - next_x;
					dif_x = dif_x < 0 ? -dif_x : dif_x;

					int dif_z = DestinationMass_Z - next_z;
					dif_z = dif_z < 0 ? -dif_z : dif_z;

					if (dif_x > dif_z) {
						heuristic = (float)dif_z * root2 + (float)(dif_x - dif_z);
					}
					else {
						heuristic = (float)dif_x * root2 + (float)(dif_z - dif_x);
					}

					MapNode[next_z][next_x].score = heuristic + (float)nowCost;

					openNode.push_back(MapNode[next_z][next_x]);

					//
					MapNode[next_z][next_x].parent = &MapNode[z][x];
				}
				else if (MapNode[next_z][next_x].state == NODE_STATE::OBSTACLE) {
					flag += ullx1 << i;
				}
			}
		}

		//斜めのノードをOpen
		//右上、右下、左下、左上の順に検査
		for (int i = 0; i < 4; i++) {
			ull n1 = ullx1 << i;
			ull n2 = ullx1 << (i + ullx1) % 4;
			if (!(flag & n1) && !(flag & n2)) {
				int next_x = x + (1 - ((i / 2) * 2));
				int next_z = z + (-1 + ((i % 3) * (3 - i)));

				if (next_z != -1 && next_x != -1) {
					if (MapNode[next_z][next_x].state == NODE_STATE::GOAL) {
						RouteUpToDestination.clear();
						MapNode[EnemyMass_Z][EnemyMass_X].parent = nullptr;
						MapNode[next_z][next_x].parent = &MapNode[z][x];
						CreateRouteFromNode_Recursive(&MapNode[next_z][next_x]);
						return 0;
					}
					else if (MapNode[next_z][next_x].state == NODE_STATE::EMPTY) {
						MapNode[next_z][next_x].state = NODE_STATE::OPEN;

						float heuristic = 0;
						int dif_x = DestinationMass_X - next_x;
						dif_x = dif_x < 0 ? -dif_x : dif_x;

						int dif_z = DestinationMass_Z - next_z;
						dif_z = dif_z < 0 ? -dif_z : dif_z;

						if (dif_x > dif_z) {
							heuristic = (float)dif_z * root2 + (float)(dif_x - dif_z);
						}
						else {
							heuristic = (float)dif_x * root2 + (float)(dif_z - dif_x);
						}

						MapNode[next_z][next_x].score = heuristic + (float)nowCost;

						openNode.push_back(MapNode[next_z][next_x]);

						//
						MapNode[next_z][next_x].parent = &MapNode[z][x];
					}
				}
			}
		}

		// 降順にソート
		std::sort(openNode.begin(), openNode.end());

		for (const auto& i : openNode) {
			result = Astar_Recursive(i.x, i.z, nowCost + 1);
			if (result == 0) {
				break;
			}
		}

		return result;
	}
	void FatmanAI::ResetNode()
	{
		//ノードリセット
		int mapRowSize = MapNode.size();
		for (int i = 0; i < mapRowSize; i++) {
			int mapColumnSize = MapNode[i].size();
			for (int j = 0; j < mapColumnSize; j++) {
				if (MapNode[i][j].state != NODE_STATE::OBSTACLE) {
					MapNode[i][j].state = NODE_STATE::EMPTY;
					MapNode[i][j].parent = nullptr;
				}
			}
		}

		MapNode[DestinationMass_Z][DestinationMass_X].state = NODE_STATE::GOAL;
	}
	void FatmanAI::CreateRouteFromNode_Recursive(Node* p)
	{
		//目的地から順に自分が通る座標をプッシュしていく
		//ただし、Astar() を事前に実行し、m_node[m_goalZ][m_goalX] までのルートが出来ていること
		//m_route.clear() が呼び出されていること
		if (p) {
			if (p->parent) {
				RouteUpToDestination.push_back(VGet((float)p->x * MASS_SIZE, 0.0f, (float)p->z * MASS_SIZE));
			}

			CreateRouteFromNode_Recursive(p->parent);
		}
	}
	bool FatmanAI::CreateRouteFromLookPlayer(MasterDataBase* pmdb, Enemy* pEnemy)
	{
		//マップ上の道を取得
		v_ull road = pmdb->MapRoad;

		//現在地(ワールド座標)を取得
		auto pos = pEnemy->GetPos();

		//マスを取得
		int enemy_x = EnemyMass_X;
		int enemy_z = EnemyMass_Z;

		int player_x = pmdb->PlayerMASS_X;
		int player_z = pmdb->PlayerMASS_Z;

		//
		int min = MIN(player_z, enemy_z);
		int searchRow = abs(player_z - enemy_z) + 1;

		VECTOR player = VGet((float)player_x, 0.0f, (float)player_z);
		VECTOR enemy = VGet((float)enemy_x, 0.0f, (float)enemy_z);
		VECTOR p_to_e = VSub(player, enemy);
		if (p_to_e.x == p_to_e.z) {
			p_to_e.x /= abs(p_to_e.x);
			p_to_e.z /= abs(p_to_e.z);
		}
		else {
			p_to_e = VNorm(p_to_e);
		}

		v_ull a(searchRow);
		int x = (int)round(enemy.x);
		int z = (int)round(enemy.z);
		int index = (z - min);
		a[index] += ullx1 << x;

		while (x != player_x || z != player_z) {
			enemy = VAdd(enemy, p_to_e);
			x = (int)round(enemy.x);
			z = (int)round(enemy.z);
			index = (z - min);
			a[index] += ullx1 << x;
		}

		int i = 0;
		for (i = 0; i < searchRow; i++) {
			if ((road[min + i] & a[i]) != a[i]) {
				break;
			}
		}

		if (i == searchRow) {	//敵の視界内にプレイヤーがいたときの処理
			float RangeFromPlayerToEnemy = VSize(VSub(VGet(pmdb->PlayerPos_X, 0.0f, pmdb->PlayerPos_Z), pEnemy->GetPos()));
			if (RangeFromPlayerToEnemy > 500.0f) {
				RouteUpToDestination.clear();
				RouteUpToDestination.push_back(VGet(pmdb->PlayerPos_X, 0.0f, pmdb->PlayerPos_Z));
				return true;
			}
			else {	//プレイヤーと敵が当たった時の処理
				ResetNode();
				RouteUpToDestination.clear();
				//pEnemy->SetPos((int)pmdb->EnemyPos[0].x, (int)pmdb->EnemyPos[0].z);
				pmdb->ExecutionMode = Mode_GameOver;
			}
		}

		return false;
	}
}