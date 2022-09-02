#include "MAIN.h"
#include "StartMenu/StartMenu.h"
#include "MapData/MapData.h"
#include "PlayerOperation/PlayerOperation.h"

#include "Enemy/Enemy.h"
#include "Enemy/Fatman/Fatman.h"

//std::vector<Enemy*> enemy;
#pragma warning(disable:4996)

//Windows���s���C���֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	GameMain* pGameMain = new GameMain;
	MasterDataBase* pmdb = new MasterDataBase;
	pGameMain->GameMainVoid(pmdb);

	return 0;
};

/////////////////////////////////////////////////////////////////////
///////                      ���񃍁[�h                        //////
/////////////////////////////////////////////////////////////////////
void GameMain::FirstLoad(MasterDataBase *pmdb) {

	pmdb->mGraph.Loading = LoadGraph("Movies/NowLoading.mp4");
	PlayMovieToGraphLoop(WINDOW_MAX_X / 2, WINDOW_MAX_Y / 2 + 20, 0.6, 0, pmdb->mGraph.Loading, TRUE);

	//�t�H���g�ǂݍ���
	pmdb->mFont.ElementFont = CreateFontToHandle("���C���I", 20, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
	pmdb->mFont.TitleFont = CreateFontToHandle("���C���I",65, 5, DX_FONTTYPE_ANTIALIASING_EDGE);
	pmdb->mFont.CommentFont = CreateFontToHandle("MS����", 15, 2, DX_FONTTYPE_ANTIALIASING_EDGE);
	pmdb->mFont.TutorialFont = CreateFontToHandle("MS����", 13, 0, DX_FONTTYPE_ANTIALIASING_EDGE);

	//����ǂݍ���
	pmdb->mGraph.ClearBack = LoadGraph("Movies/nc263414.mp4");
	//mGraph.test = LoadGraph("����/NowLoading.mp4");

	//�J����������
	pmdb->CamPos = VGet(pmdb->PlayerMASS_X * MASS_SIZE, CAMERA_Y, pmdb->PlayerMASS_Z * MASS_SIZE);
	pmdb->CamDir = VGet(1.0f, 0.0f, 0.0f);
	pmdb->CamTarg = VAdd(pmdb->CamPos, pmdb->CamDir);
	SetCameraPositionAndTarget_UpVecY(pmdb->CamPos, pmdb->CamTarg);

	// �ǃ��f���̓ǂ݂���
	pmdb->mModel.Wall = MV1LoadModel("Models/Wall.mqo");
	pmdb->mModel.Goal = MV1LoadModel("Models/Goal.mqo");
	pmdb->mModel.Ling = MV1LoadModel("Models/EL.mqo");
	pmdb->mModel.Key = MV1LoadModel("Models/Key.mqo");

	//�摜�ǂݍ���
	pmdb->mGraph.StartMenuBack = LoadGraph("Images/etc/StartMenu.jpg");
	pmdb->mGraph.Title = LoadGraph("Images/etc/Title.jpg");
	pmdb->mGraph.Window = LoadGraph("Images/InterFace/box_horror_grbl.jpg");
	pmdb->mGraph.HPFrame = LoadGraph("Images/InterFace/HPFrame.jpg");
	pmdb->mGraph.HPBar = LoadGraph("Images/InterFace/HPGauge.jpg");
	pmdb->mGraph.Stage = LoadGraph("Images/etc/S_1.jpg");
	pmdb->mGraph.OverBack = LoadGraph("Images/etc/GameOver.jpg");
	
	//�@�A�C�R��
	//SetTransColor(0, 0, 0);
	pmdb->mIcon.Eye = LoadGraph("Images/InterFace/Eye2.jpg");
	pmdb->mIcon.EyeNot = LoadGraph("Images/InterFace/Eye_Not2.jpg");
	pmdb->mIcon.Dash = LoadGraph("Images/InterFace/Dash2.jpg");
	pmdb->mIcon.DashNot = LoadGraph("Images/InterFace/Dash_Not2.jpg");
	pmdb->mIcon.Unlock = LoadGraph("Images/InterFace/Unlock2.jpg");
	pmdb->mIcon.Lock = LoadGraph("Images/InterFace/Lock2.jpg");
	pmdb->mIcon.Key = LoadGraph("Images/InterFace/Key2.jpg");

	//�T�E���h�ǂݍ���
	pmdb->mSound.StartMenuBGM = LoadSoundMem("SoundFile/BGM/�I���S�[���Ɨ[�Ă�.mp3");
	pmdb->mSound.GamingBGM = LoadSoundMem("SoundFile/BGM/�s���ȒT��.mp3");
	pmdb->mSound.GameOverBGM = LoadSoundMem("SoundFile/BGM/�s���ȋ�C.mp3");
	pmdb->mSound.Select = LoadSoundMem("SoundFile/SE/select.mp3");
	pmdb->mSound.Enter = LoadSoundMem("SoundFile/SE/select02.mp3");
	pmdb->mSound.GetKey = LoadSoundMem("SoundFile/SE/coin04.mp3");
	pmdb->mSound.Walk = LoadSoundMem("SoundFile/SE/walking_on_floor2.mp3");
	pmdb->mSound.TiredCoughing = LoadSoundMem("SoundFile/SE/man_coughing1.mp3");
	pmdb->mSound.Fear01 = LoadSoundMem("SoundFile/SE/devil_groaning1.mp3");
	pmdb->mSound.Fear02 = LoadSoundMem("SoundFile/SE/dragon_roar.mp3");
	pmdb->mSound.Fear03 = LoadSoundMem("SoundFile/SE/monster_heartbeats2.mp3");

}

/////////////////////////////////////////////////////////////////////
///////                      �Q�[�����C��                      //////
/////////////////////////////////////////////////////////////////////
void GameMain::GameMainVoid(MasterDataBase *pmdb){
	
	//Create and Set Window 
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK), SetWindowSize(WINDOW_MAX_X, WINDOW_MAX_Y);
	
	//�ߏ�ȃ��t���b�V�����[�g�̃u���[�L
	double Next_Time = GetNowCount();
	
	//���񃍁[�h
	FirstLoad(pmdb);

	//�N���X���쐬
	StartMenu* gmStartMenu = new StartMenu();
	MapControll* gmMapControll = new MapControll();
	PlayerOperation* gmPlayerOperation = new PlayerOperation();
	
	pmdb->Enemy.push_back(Fatman::CreateFatman());
	/*
	for (auto i : pmdb->Enemy) {
		i->Init(pmdb);
		i->SetPos((int)pmdb->EnemyPos[0].x, (int)pmdb->EnemyPos[0].z);
		pmdb->Enemy.push_back(Fatman::CreateFatman());
	}*/
//test
//gmMapControll->CleateMap(pmdb);
//pmdb->ExecutionMode = Mode_Gaming;

/////////////////////////////////////////////////////////////////////
///////                      ���[�v����                        //////
/////////////////////////////////////////////////////////////////////
while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {

	// �ߏ�ȃ��t���b�V�����[�g�ɑ΂���u���[�L
	// 60fps�ɒ���(1s / 60f �� 16.66count)
	Next_Time += 16.66;
	if (Next_Time > GetNowCount()) {
		WaitTimer((int)Next_Time - GetNowCount());
	}

	////////////////////////////////////
	////// �S���[�h���ʂōs������ //////
	////////////////////////////////////

	//�}�E�X���W�X�V
	GetMousePoint(&pmdb->MouseCursor_X, &pmdb->MouseCursor_Y);

	//���Ԃ�i�߂�
	if (pmdb->TimeFlag == true) {
		pmdb->TimeCount++;
		for (auto i : pmdb->Enemy) {
			i->Draw(true);
		}
	}
	//�~�܂��Ă��鎞�̏���	
	else {
		for (auto i : pmdb->Enemy) {
			i->Draw(false);
		}
	}

	/////////////////////////////////////////////
	////// ���݂̃��[�h�ɉ������������s���B//////
	/////////////////////////////////////////////

	switch (pmdb->ExecutionMode) {
		//�X�^�[�g���j���[����
	case Mode_StartMenu:
		gmStartMenu->StartMenuVoid(pmdb);
		BGMUpDate(pmdb, pmdb->mSound.StartMenuBGM);
		if (gmStartMenu->StartMenuMode == StartMenuMode_HowTo) {
			DrawRotaGraph(WINDOW_MAX_X / 2, WINDOW_MAX_Y / 2, 1.0, 0.0, pmdb->mGraph.Stage, TRUE);
			gmPlayerOperation->InterFace(pmdb);
			gmPlayerOperation->TutorialWindow(pmdb);
			if (CheckHitKey(KEY_INPUT_F) == true) {
				gmStartMenu->StartMenuMode = StartMenuMode_Top;
				pmdb->TutorialFlag = true;
			}
		}
		break;

	case Mode_StageSetting:
		gmMapControll->MapReset(pmdb);

		for (auto i : pmdb->Enemy) {
			i->Init(pmdb);
			i->SetPos((int)pmdb->EnemyPos[0].x, (int)pmdb->EnemyPos[0].z);
		}
		pmdb->ExecutionMode = Mode_Gaming;
		break;

		//�Q�[��������
	case Mode_Gaming:

		BGMUpDate(pmdb, pmdb->mSound.GamingBGM);
		gmPlayerOperation->PlayerOperationMain(pmdb);
		gmMapControll->DrawMap(pmdb);
		if (pmdb->TimeFlag == true) {
			for (auto& i : pmdb->Enemy) {
				i->Update(pmdb);
			}
		}

		if ((bool)CheckHitKey(KEY_INPUT_E) == false) {
			gmPlayerOperation->InterFace(pmdb);
		}
		if (pmdb->TutorialFlag == true) {
			gmPlayerOperation->TutorialWindow(pmdb);
		}
		break;

	case Mode_Clear:
		gmMapControll->MapReset(pmdb);
		//pmdb->Enemy.push_back(Fatman::CreateFatman());

		for (auto i : pmdb->Enemy) {
			i->Init(pmdb);
			i->SetPos((int)pmdb->EnemyPos[0].x, (int)pmdb->EnemyPos[0].z);
		}
		break;
		//�Q�[���I�[�o�[
	case Mode_GameOver:

		if (GameOverStayTime-- > 0) {
			SetDrawBright(pmdb->ShadowLevel--, pmdb->ShadowLevel--, pmdb->ShadowLevel--);
			gmMapControll->DrawMap(pmdb);
			gmPlayerOperation->InterFace(pmdb);
			DrawBox(0, 0, WINDOW_MAX_X, WINDOW_MAX_Y, GetColor(255, 0, 0), TRUE);
			BGMUpDate(pmdb, pmdb->mSound.Fear01);
		}
		else {
			pmdb->ShadowLevel = 255;
			SetDrawBright(pmdb->ShadowLevel, pmdb->ShadowLevel, pmdb->ShadowLevel);

			BGMUpDate(pmdb, pmdb->mSound.GameOverBGM);
			DrawRotaGraph(WINDOW_MAX_X / 2, WINDOW_MAX_Y / 2, 1.0, 0.0, pmdb->mGraph.OverBack, FALSE);

			DrawStringToHandle(WINDOW_MAX_X / 5, WINDOW_MAX_Y * 2 / 3, "-Pless Enter or Space to Continue-", (int)GetColor(200, 60, 60), pmdb->mFont.ElementFont);

			if (CheckHitKey(KEY_INPUT_RETURN) == true || CheckHitKey(KEY_INPUT_SPACE) == true) {
				pmdb->ExecutionMode = Mode_StageSetting;
				pmdb->TutorialFlag = true;
				GameOverStayTime = 3 * Second_flame;
			}
		}
			break;

			//�Q�[���I��������
		case Mode_GameExit:
			DxLib_End();

		default: 
			PlayMovieToGraphLoop(WINDOW_MAX_X / 2, WINDOW_MAX_Y / 2 + 20, 0.6, 0, pmdb->mGraph.Loading, TRUE);
		}
	}
}

//�摜���[�r�[�����[�v�ōĐ�����֐�

void GameMain::PlayMovieToGraphLoop(int x, int y, float rate, float rota, int MovieGraphHandle, int flag) {
	if (GetMovieStateToGraph(MovieGraphHandle) == 0) {
		SeekMovieToGraph(MovieGraphHandle, 0);
		PlayMovieToGraph(MovieGraphHandle);
	}
	DrawRotaGraph(x, y, rate, rota, MovieGraphHandle, flag);
}

void GameMain::BGMUpDate(MasterDataBase* pmdb,int NextSound) {
	if (pmdb->NowPlayBGM != NextSound) {
		StopSoundMem(pmdb->NowPlayBGM);
		pmdb->NowPlayBGM = NextSound;
		PlaySoundMem(pmdb->NowPlayBGM, DX_PLAYTYPE_LOOP);
	}
}
