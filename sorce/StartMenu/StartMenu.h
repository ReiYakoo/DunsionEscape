#pragma once
#include "../MAIN.h"

#define StartMenuElementsNum 3			//�X�^�[�g���j���[���ڐ�
#define StartMenuElementsStrings 50		//�X�^�[�g���j���[�ŕ\�����镶����

#define StartMenuElement_Den_X	50		//�X�^�[�g���j���[���ڂ̏���X���W
#define StartMenuElement_Den_Y	WINDOW_MAX_Y * 2 / 3		//�X�^�[�g���j���[���ڂ̏���Y���W

#define StartMenu_Padding 30			//�X�^�[�g���j���[�̍s�ԍ���
#define StartMenu_Hight	StartMenu_Padding * (StartMenuElementsNum + 1 )		//�X�^�[�g���j���[�S�̂̍��� 
#define StartMenu_Width	200				//�X�^�[�g���j���[�S�̂̕�

//�X�^�[�g���j���[�̎��s���[�h
enum StartMenuMode {
	StartMenuMode_Top,				//�g�b�v���
	StartMenuMode_HowTo,			//����������
	StartMenuMode_StageSelect,		//�X�e�[�W�I��
	StartMenuMode_StageSelectCheck,	//�v���C���܂����H
	StartMenuMode_ExitCheck,		//�I�����܂����H
};

//�X�^�[�g���j���[�̑I������
enum StartMenuElements {
	StartMenuElement_GameStart,
	StartMenuElement_Howto,
	StartMenuElement_Exit,
};

class StartMenu {
public:
	StartMenu();
	~StartMenu();
	void StartMenuVoid(MasterDataBase * pmdb);		//�X�^�[�g���j���[�̎��s�Ƃ�܂Ƃ�
	void StartMenuFirstTime(MasterDataBase *pmdb);
	void StartMenuKeyCheck(MasterDataBase* pmdb);	//�L�[���̓`�F�b�N
	void StartMenuDraw(MasterDataBase* pmdb);		//��ʏ�ɕ�����摜��\��
	void MouseCheck(MasterDataBase* pmdb);
	void CheckWindow(MasterDataBase *pmdb);

	bool StartMenuFirstTimeFlag = true;			//���񃍁[�h�t���O
	int StartMenuMode = StartMenuMode_Top;		//�X�^�[�g���j���[���s���Ă��鏈��
	int SelectNum = StartMenuElement_GameStart;	//�I�𒆂̍���
	int BeforeSelectNum = SelectNum;				//���O�̑I������
	bool CheckWindowFlag = false;				//�`�F�b�N�E�B���h�E�\���t���O(true�ŕ\��)
	bool CheckWindowSelect = true;				//�`�F�b�N��ʑI�𒆍���(true:�͂� / false:������)

	//�X�^�[�g���j���[�\���̃p�[�c
	struct StartMenuStruct {
		int x, y;								//�ʒu
		char title[StartMenuElementsStrings];	//�\�����镶��
		int color;								//�����F
	};
	
	StartMenuStruct StartMenuStruct[StartMenuElementsNum] = {
		StartMenuElement_Den_X, StartMenuElement_Den_Y,								"GAME START",	(int)GetColor(255,255,255),
		StartMenuElement_Den_X, StartMenuElement_Den_Y + StartMenu_Padding,			"HOW TO PLAY",	(int)GetColor(255,255,255),
		StartMenuElement_Den_X, StartMenuElement_Den_Y + StartMenu_Padding * 2,		"EXIT",			(int)GetColor(255,255,255),
	};

};