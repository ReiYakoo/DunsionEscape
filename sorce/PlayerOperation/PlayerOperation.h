#pragma once
#include "../MAIN.h"

#define StaminaGuage_DenHeight 10	//�X�^�~�i�Q�[�W�̍���
#define StaminaGuage_DenWidth 100	//�X�^�~�i�Q�[�W�̒���

#define CommentWindow_Width 300		//�^�񒆂̃E�B���h�E�̒���
#define CommentWindow_Height 100	//�^�񒆂̃E�B���h�E�̍���

#define ConfigWindow_Width 100		//�����̃E�B���h�E�̒���
#define ConfigWindow_Height 100		//�����̃E�B���h�E�̍���

#define MaterWindow_Width 150		//�E���̃E�B���h�E�̒���
#define MaterWindow_Height 80		//�E���̃E�B���h�E����

#define PauseMenuElementMax 3		//�|�[�Y���j���[�̍��ڐ�

//�|�[�Y���j���[�̍���
struct Element {
	int x, y;
	char name[20];
};

//�|�[�Y���j���[�I������
enum PSelect {
	PSelect_Continue,
	PSelect_Retry,
	PSelect_Exit,
};

class PlayerOperation {
	public:
		void PlayerOperationMain(MasterDataBase *pmdb);
		void KeyCheck(MasterDataBase *pmdb);
		void HitAndPosCheck(MasterDataBase* pmdb);
		void InterFace(MasterDataBase* pmdb);
		void TutorialWindow(MasterDataBase *pmdb);
		void PauseMenu(MasterDataBase *pmdb);

		int StaminaBarWidth = StaminaGuage_DenWidth;	//�X�^�~�i�̃o�[�̌��݉���
		char *Comment[100];							//���b�Z�[�W
		int Icon;									//�A�C�R��
		int FootSoundCount = 0;						//�����p�J�E���g
		int FearSoundCount = 0;						//���؂�ւ��@
		int CTIconCount = 0;						//�����ʂ̃A�C�R���؂�ւ�����
		bool PauseFlag = false;					//�|�[�Y���j���[�̃t���O
		int PSelectNum = 0;						//�I�𒆍���

		Element PMenuElements[PauseMenuElementMax] = {
			WINDOW_MAX_X * 2 / 7 + 50,	WINDOW_MAX_Y * 2 / 7 + 50,	"�Q�[���ɖ߂�",
			WINDOW_MAX_X * 2 / 7 + 50,	WINDOW_MAX_Y * 2 / 7 + 80,	"���g���C",
			WINDOW_MAX_X * 2 / 7 + 50,	WINDOW_MAX_Y * 2 / 7 + 110,	"�^�C�g���ɖ߂�",
		};

		VECTOR GetKey;
};