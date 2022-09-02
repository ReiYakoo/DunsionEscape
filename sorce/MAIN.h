#pragma once
#include "../DxLib/DxLib.h"

#include <math.h>
#include <vector>

#define Second_flame 60
#define WINDOW_MAX_X 640			// ��ʉ���
#define WINDOW_MAX_Y 480			// ��ʏc��

#define MASS_SIZE	1000.0f			// �u���b�N�̃T�C�Y

#define MASS_MAX_X	16				// �w�����̃u���b�N�� :��
#define MASS_MAX_Z	16				// �y�����̃u���b�N�� :�c

#define ROTATE_SPEED (DX_PI_F / 400.0f);	//�J�M�̉�]���x
#define MOVE_FRAME 60				//���Ɏg���Ă������c
#define CLEAR_TIME_MAX 180				//���߉\����
#define CLEAR_COOLTIME 900			//���߂̃N�[���^�C��

#define PlayerStaminaMAX 200.0f
#define PlayerMoveSpeed_Defalt 20.0f
#define PlayerMoveSpeed_Dash 40.0f
#define PlayerMoveSpeed_Tired 10.0f

#define KEY_MAX 4					//�J�M�ő吔
#define STAGE_MAX 10				//�X�e�[�W�̍ő吔

//�}�X�̊�{���
#define MASS_WALL	'0'				// ��n�}�X(��)
#define MASS_ROAD	'1'				// ���ƂȂ�}�X
#define MASS_GOAL	'G'				// �S�[���}�X
#define MASS_KEY	'K'				// �J�M���u���Ă���}�X
#define MASS_ENEMY	'E'				// �G�}�X

#define CAMERA_Y 600.0f				// �J�����̍���

//�v���g�^�C�v�錾
class GameMain;
class MasterDataBase;
class StartMenu;
class MapControll;
class PlayerOperation;
class Enemy;

typedef unsigned long long ull;
typedef std::vector<ull> v_ull;
typedef std::vector<char> v_char;
typedef std::vector<std::vector<char>> vv_char;
typedef std::vector<VECTOR> v_vector;

//1�}�X�̎����
class MassData {
public:
	int TraficFlag = 1;
	char OriginalData;
	char NowData;
};

//�v���C���[���s���Ă���ړ�����
enum MoveMode {
	DontMove,
	GotoFront,
	GotoBack,
	RightSpin,
	LeftSpin,
};

//���ݎ��s���̃Q�[�����[�h
enum GameMainExecutionMode {
	Mode_GameExit,	//�Q�[���I��
	Mode_StartMenu,	//�X�^�[�g���j���[
	Mode_StageSetting,	//�X�e�[�W�̗p�Ӂi���ځj
	Mode_Gaming,	//�Q�[����
	Mode_Pause,		//�|�[�Y���
	Mode_Clear,		//�N���A
	Mode_GameOver,	//�Q�[���I�[�o�[
	Mode_NextStage,	//�X�e�[�W�؂�ւ�
	Mode_Tutorial	//�`���[�g���A�����
};

class GameMain {
public:
	void FirstLoad(MasterDataBase *pmdb);		//���񃍁[�h
	void GameMainVoid(MasterDataBase *pmdb);		//���C�����s����
	void FadeIn(MasterDataBase* pmdb);
	void FadeOut(MasterDataBase* pmdb);
	void PlayMovieToGraphLoop(int x, int y, float rate, float rota, int MovieGraphHandle, int flag);
	void BGMUpDate(MasterDataBase *pmdb,int NextBGM);

	int GameOverStayTime = 3.5 * Second_flame;
};

//�ϐ���n���h�����e�N���X�Ɍp��������N���X
class MasterDataBase{
public:
	//�}�b�v�̃f�[�^
	MassData MapData[MASS_MAX_Z][MASS_MAX_X];

	//�ϐ���
	int ExecutionMode = Mode_StartMenu;		//���ݎ��s���̏���
	int MouseCursor_X = 0,				//�J�[�\���̌��݈ʒu(������̓}�E�X�Ő���)
		MouseCursor_Y = 0;				//�J�[�\���̌��݈ʒu(������̓}�E�X�Ő���)
	int NowStageNum = 1;				//���݂̃X�e�[�W�ԍ�
	bool TimeFlag = true;				//���Ԓ�~/�ғ�(true:�ғ� false:��~)
	int TimeCount = 0;					//����(1f��1�ǉ�)
	int Bright = 0;						//���邳
	int NowPlayBGM = 0;					//�Đ�����BGM
	int KeyNum = 0;						//�擾�J�M��
	int EnemyNum;						//���݂���G�̐�

	int ShadowLevel = 255;
	// bool FadeInFlag = true;
	// bool FadeOutFlag = false;
	// int PlayerNowMove = DontMove;			//�v���C���[�����ݍs���Ă��铮��

	float PlayerHitRad = 150.0f;			//�����蔻��傫��
	int PlayerMASS_X = 2,					//�v���C���[�̌��ݗ����Ă���}�X
		PlayerMASS_Z = 1;					//�v���C���[�̌��ݗ����Ă���}�X
	float PlayerPos_X = PlayerMASS_X * MASS_SIZE,		//�v���C���[�̌��ݗ����Ă�����W
		  PlayerPos_Z = PlayerMASS_Z * MASS_SIZE;		//�v���C���[�̌��ݗ����Ă�����W
	float PlayerSpinAngle = 0.0f;						//�v���C���[�̌����i�p�x�̐��l�j
	float PlayerMoveSpeed = PlayerMoveSpeed_Defalt;		//�v���C���[�̈ړ����x
	bool ClearModeFlag = false;					//���ߏ��/�񓧉ߏ��
	int RemainingClearTime = 0;					//�c�蓧�ߎ���
	int ClearModeCoolTime = 0;					//���ߎg�p��̃N�[���^�C��(MAX�Ȃ�g�p�\)
	bool TutorialFlag = true;					//��������̕\��

	float PlayerStamina = PlayerStaminaMAX;		//�X�^�~�i���l
	bool DashPossibleFlag = true;				//�_�b�V���\�t���O(�X�^�~�i�؂�)

	VECTOR CamPos;					// �J�����̍��W
	VECTOR CamTarg;					// �J�����̒����_
	VECTOR CamDir;					// �J�����̌����Ă������
	VECTOR CamDirHeight = VGet(0.0f, 1.0f, 0.0f);	//���ɐ����ȃx�N�g��(�O�όv�Z�p)
	
	v_ull MapRoad;
	std::vector<Enemy*> Enemy;
	std::vector<VECTOR> EnemyPos;

	//�L�[�������Ă����Ԃ�
	struct KeyFlag {
		bool Down = false;
		bool Up = false;
		bool Right = false;
		bool Left = false;
		bool Space = false;
	}PushKeyFlag;

	//���̕����ɓ����邩(�ǂ�����̂�)
	struct MovePossibleFlag{
		bool x_pls = true;
		bool z_min = true;
		bool z_pls = true;
		bool x_min = true;
	}MovePossibleFlag;

	//�摜�i�[��
	struct Graph {
		int StartMenuBack;
		int GameBack;
		int OverBack;
		int Title;
		int Window;
		int SelectArrow;
		int Loading;
		int HPFrame;
		int HPBar;
		int ClearBack;
		int Stage;
	}mGraph;

	//�A�C�R���̉摜
	struct Icon {
		int Lock;
		int Unlock;
		int Dash;
		int DashNot;
		int Eye;
		int EyeNot;
		int Key;
	}mIcon;
	
	//�T�E���h�i�[��
	struct Sound {
		int StartMenuBGM;
		int GamingBGM;
		int GameOverBGM;
		int Select;
		int Enter;
		int Walk;
		int GetKey;
		int TiredCoughing;
		int Fear01;
		int Fear02;
		int Fear03;
	}mSound;

	//���f���i�[��
	struct Model {
		int Wall;
		int Goal;
		int Ling;
		int Key;
	}mModel;

	//�t�H���g�i�[��
	struct Font{
		int TitleFont;		//�^�C�g���Ɏg���t�H���g(��)
		int TextFont;		//�e�L�X�g�̃t�H���g
		int ElementFont;	//���j���[�Ȃǂ̃t�H���g
		int CommentFont;	//�R�����g�p
		int TutorialFont;	//�������̕���	
	}mFont;

	//������摜�ɂ��ĉ�
	void PlayMovieToGraphLoop(int x, int y, float rate, float rota, int MovieGraphHandle, int flag) {
		if (GetMovieStateToGraph(MovieGraphHandle) == 0) {
			SeekMovieToGraph(MovieGraphHandle, 0);
			PlayMovieToGraph(MovieGraphHandle);
		}
		DrawRotaGraph(x, y, rate, rota, MovieGraphHandle, flag);
	}
};
