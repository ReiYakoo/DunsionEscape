#pragma once
#include <vector>

#include "../DxLib/DxLib.h"
#include "../Component.h"

namespace Model {
	class MMD;
}

namespace Fatman {

	//�G���쐬
	Enemy* CreateFatman();

	class FatmanParameter {
	public:
		constexpr static float FATMAN_SPEED					= 20.0f;	//Fatman�@�X�s�[�h
		constexpr static float FATMAN_WALK_ANIM_LOOP_START	= 87.0f;	//Fatman�@�������[�V�����A���[�v�X�^�[�g����
		constexpr static float FATMAN_WALK_ANIM_LOOP_END	= 127.0f;	//Fatman�@�������[�V�����A���[�v�G���h����

		//Fatman�@�X�P�[���l
		//../DxLib/Dxlib.h �� VECTOR(tagVECTOR) ���� 
		//constexpr tagVECTOR(x,y,z)�@�C���X�g���N�^��ǉ����Ă�������
		constexpr static VECTOR FATMAN_SCALE = VECTOR(30.0f, 30.0f, 40.0f);
	};

	//�m�[�h�̏��
	enum class NODE_STATE
	{
		EMPTY,		//���T��
		CLOSED,		//�T����
		OPEN,		//�T����(�{�����j
		OBSTACLE,	//��Q��(��ʍs)
		GOAL,		//�ړI�n
	};

	//Aster �Ŏg���}�b�v�i1�}�X�P�ʁj
	class Node {
	public:
		Node();

		int x;				//�����̃}�Xx���W
		int z;				//�����̃}�X�̃}�Xx���W
		float score;		//�����̓_��(�Ⴏ��΃S�[���ɋ߂�)
		NODE_STATE state;	//�����̃}�X�̏��
		Node* parent;		//�����̐e(�������痈����)

		bool operator<(const Node& right);		//std::sort() �p�֐�
	};

	typedef unsigned long long ull;
	typedef std::vector<ull> v_ull;
	typedef std::vector<char> v_char;
	typedef std::vector<std::vector<char>> vv_char;
	typedef std::vector<Node> v_node;
	typedef std::vector<std::vector<Node>> vv_node;
	typedef std::vector<VECTOR> v_vector;

	//�v�l����
	class FatmanAI : public AI_Component {	
	public:
		FatmanAI();
		~FatmanAI();

		void Init(const MasterDataBase* pmdb, Enemy* pEnemy);		//������
		void Update(MasterDataBase* pmdb, Enemy* pEnemy);			//�X�V

		int Astar_Recursive(int x, int z, int nowCost);				//Aster �A���S���Y��
		void ResetNode();											//�m�[�h�̃��Z�b�g

		void CreateRouteFromNode_Recursive(Node* pNode);			//pNode �̐e�����ǂ��ă��[�g�\�z

		//���E���ɂ���v���C���[�܂ł̃��[�g�\�z
		//false �̏ꍇ�A���[�g�͉����ς��Ȃ�
		//���E�� : true / ���E�O : false
		bool CreateRouteFromLookPlayer(MasterDataBase* pmdb, Enemy* pEnemy);	

	private:
		int EnemyMass_X;		//�G���ݒn(�}�X)
		int EnemyMass_Z;		//�G���ݒn(�}�X)
		int DestinationMass_X;	//�ړ��ړI�n(�v���C���[�̍��W/�}�X)
		int DestinationMass_Z;	//�ړ��ړI�n(�v���C���[�̍��W/�}�X)

		bool IsPreLookPlayer;	//���O�̃t���[���Ńv���C���[�������Ă��邩
		bool IsNowLookPlayer;	//���݂̃t���[���v���C���[�������Ă��邩
		bool IsDoAstar;			//A*���s����

		VECTOR EnemyPos;		//�G���ݒn(���[���h���W)

		v_vector RouteUpToDestination;		//�ړI�n���猈�肳�ꂽ�i�s���[�g
		vv_node MapNode;					//�}�b�v��񂩂猈�肳�ꂽ�m�[�h�Q
	};

	//�����n
	class FatmanPhysics : public Physics_Component {
	public:
		FatmanPhysics();
		~FatmanPhysics();

		void Init(const MasterDataBase* pmdb, Enemy* pEnemy);	//������
		void Update(MasterDataBase* pmdb, Enemy* pEnemy);		//�X�V
	};

	//�`��/���f���֘A
	class FatmanMMD : public Model_Component {
	public:
		FatmanMMD();
		~FatmanMMD();

		void Init(const MasterDataBase* pmdb, Enemy* pEnemy);	//������
		void Update(MasterDataBase* pmdb, Enemy* pEnemy);		//�X�V
		void Draw(Enemy* pEnemy, bool isAnimation);				//�`�悷�邾���̊֐�

	private:
		Model::MMD* pModel;			//���f���̃C���^�[�t�F�[�X
	};
}