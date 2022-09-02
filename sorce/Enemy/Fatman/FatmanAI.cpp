#include <algorithm>

#include "Fatman.h"
#include "../Enemy.h"
#include "../../MAIN.h"

#define root2 (1.41421356f)
#define _1RAD (57.2957795f)
#define ANGLE_TO_RADIAN( x ) { x / _1RAD };
#define MIN(x, y) { x < y ? (x) : (y) };

const ull ullx1 = (ull)1;	//ull�^��1

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

		//�ǂݍ��܂ꂽ�}�b�v�f�[�^����m�[�h���쐬
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
		//���W�擾
		EnemyPos = pEnemy->GetPos();
		pEnemy->GetPos(&EnemyMass_X, &EnemyMass_Z);

		//�ڕW�n�_��ݒ�
		DestinationMass_X = pmdb->PlayerMASS_X;
		DestinationMass_Z = pmdb->PlayerMASS_Z;

		//���E���Ƀv���C���[�𔭌����Ă��邩����
		IsPreLookPlayer = IsNowLookPlayer;
		IsNowLookPlayer = CreateRouteFromLookPlayer(pmdb, pEnemy);

		//�v���C���[������������ A*
		//A*������K�v�����鎞�� IsDoAstar = true�@���邱��
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

			//�i�s���������߂�
			float dir_x = RouteUpToDestination[index].x - EnemyPos.x;
			float dir_z = RouteUpToDestination[index].z - EnemyPos.z;

			VECTOR moveDir	= VGet(dir_x, 0.0f, dir_z);
			moveDir			= VNorm(moveDir);
			EnemyPos		= VAdd(EnemyPos, VScale(moveDir, pEnemy->GetSpeed()));
			pEnemy->SetPos(EnemyPos);

			//�i�s���������]�p�x�����߉�]
			VECTOR initFatmanDir = VGet(0.0f, 0.0f, -1.0f);
			float dot = VDot(initFatmanDir, moveDir);
			float rad = acosf(dot);

			if (moveDir.x > 0.0f) {		//180�x���߂�����
				VECTOR dir = VGet(-moveDir.x, 0.0f, -moveDir.z);
				dot = VDot(initFatmanDir, dir);
				rad = DX_PI_F + acosf(dot);
			}

			VECTOR rotate = VGet(0, 0, 0);
			rotate.y = rad;
			pEnemy->SetRotateRadian(rotate);

			//��̖̂ړI�n�ɋ߂Â�����
			//���̖ړI�n�Ɉڍs
			float RangeUpToDestination = VSize(VSub(RouteUpToDestination[index], EnemyPos));
			if (RangeUpToDestination < pEnemy->GetSpeed() + 6.0f) {
				EnemyPos = RouteUpToDestination[index];
				pEnemy->SetPos(EnemyPos);
				RouteUpToDestination.pop_back();
			}
		}
		else {	//�s���悪�����Ȃ����� A* ����
			IsDoAstar = true;
		}
	}
	int FatmanAI::Astar_Recursive(int x, int z, int nowCost)
	{
		MapNode[z][x].state = NODE_STATE::CLOSED;

		int result = 1;
		ull flag = 0;
		v_node openNode;

		//�㉡�̃m�[�h��Open
		//��A�E�A���A���̏��Ɍ���
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

		//�΂߂̃m�[�h��Open
		//�E��A�E���A�����A����̏��Ɍ���
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

		// �~���Ƀ\�[�g
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
		//�m�[�h���Z�b�g
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
		//�ړI�n���珇�Ɏ������ʂ���W���v�b�V�����Ă���
		//�������AAstar() �����O�Ɏ��s���Am_node[m_goalZ][m_goalX] �܂ł̃��[�g���o���Ă��邱��
		//m_route.clear() ���Ăяo����Ă��邱��
		if (p) {
			if (p->parent) {
				RouteUpToDestination.push_back(VGet((float)p->x * MASS_SIZE, 0.0f, (float)p->z * MASS_SIZE));
			}

			CreateRouteFromNode_Recursive(p->parent);
		}
	}
	bool FatmanAI::CreateRouteFromLookPlayer(MasterDataBase* pmdb, Enemy* pEnemy)
	{
		//�}�b�v��̓����擾
		v_ull road = pmdb->MapRoad;

		//���ݒn(���[���h���W)���擾
		auto pos = pEnemy->GetPos();

		//�}�X���擾
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

		if (i == searchRow) {	//�G�̎��E���Ƀv���C���[�������Ƃ��̏���
			float RangeFromPlayerToEnemy = VSize(VSub(VGet(pmdb->PlayerPos_X, 0.0f, pmdb->PlayerPos_Z), pEnemy->GetPos()));
			if (RangeFromPlayerToEnemy > 500.0f) {
				RouteUpToDestination.clear();
				RouteUpToDestination.push_back(VGet(pmdb->PlayerPos_X, 0.0f, pmdb->PlayerPos_Z));
				return true;
			}
			else {	//�v���C���[�ƓG�������������̏���
				ResetNode();
				RouteUpToDestination.clear();
				//pEnemy->SetPos((int)pmdb->EnemyPos[0].x, (int)pmdb->EnemyPos[0].z);
				pmdb->ExecutionMode = Mode_GameOver;
			}
		}

		return false;
	}
}