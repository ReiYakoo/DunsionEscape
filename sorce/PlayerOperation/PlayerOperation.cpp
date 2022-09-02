#include "PlayerOperation.h"
#include "../MapData/MapData.h"

void PlayerOperation::PlayerOperationMain(MasterDataBase* pmdb) {

	//�����t
	if (pmdb->TimeFlag == true) {
		KeyCheck(pmdb);
	}

	//�Q�[���ɔ��f
	HitAndPosCheck(pmdb);

}

////////////////////////////////////////////////////////////////
////				����𔽉f��������						////
////////////////////////////////////////////////////////////////
void PlayerOperation::HitAndPosCheck(MasterDataBase* pmdb) {

	//���ݒn���Q�Ƃ��ė����Ă���}�X���X�V
	if (pmdb->PlayerPos_X >= pmdb->PlayerMASS_X * (MASS_SIZE)+MASS_SIZE / 2) { pmdb->PlayerMASS_X++; }
	if (pmdb->PlayerPos_X < pmdb->PlayerMASS_X * (MASS_SIZE - 1) - MASS_SIZE / 2) { pmdb->PlayerMASS_X--; }
	if (pmdb->PlayerPos_Z >= pmdb->PlayerMASS_Z * (MASS_SIZE) + MASS_SIZE / 2) { pmdb->PlayerMASS_Z++; }
	if (pmdb->PlayerPos_Z < pmdb->PlayerMASS_Z * (MASS_SIZE - 1) - MASS_SIZE / 2) { pmdb->PlayerMASS_Z--; }

	//�אڂ���}�X���ǂ����ʂ���
	//X�{
	if (pmdb->MapData[pmdb->PlayerMASS_Z][pmdb->PlayerMASS_X + 1].NowData == MASS_WALL) {
		//�����蔻�苗�����1�}�X��̕ǂ��߂�������C��
		if (pmdb->PlayerPos_X > MASS_SIZE * (pmdb->PlayerMASS_X + 0.5) - pmdb->PlayerHitRad) {
			pmdb->MovePossibleFlag.x_pls = false;
		}
		else {
			pmdb->MovePossibleFlag.x_pls = true;
		}
	}
	else {
		pmdb->MovePossibleFlag.x_pls = true;
	}
	//X-
	if (pmdb->MapData[pmdb->PlayerMASS_Z][pmdb->PlayerMASS_X - 1].NowData == MASS_WALL) {
		//�����蔻�苗�����1�}�X�O�̕ǂ��߂�������C��
		if (pmdb->PlayerPos_X < (MASS_SIZE * (pmdb->PlayerMASS_X - 0.5) + pmdb->PlayerHitRad)) {
			pmdb->MovePossibleFlag.x_min = false;
		}
		else {
			pmdb->MovePossibleFlag.x_min = true;
		}
	}
	else {
		pmdb->MovePossibleFlag.x_min = true;
	}
	//Z�{
	if (pmdb->MapData[pmdb->PlayerMASS_Z + 1][pmdb->PlayerMASS_X].NowData == MASS_WALL) {
		//�����蔻�苗�����1�}�X��̕ǂ��߂�������C��
		if (pmdb->PlayerPos_Z > MASS_SIZE * (pmdb->PlayerMASS_Z + 0.5) - pmdb->PlayerHitRad) {
			pmdb->MovePossibleFlag.z_pls = false;
		}
		else {
			pmdb->MovePossibleFlag.z_pls = true;
		}
	}
	else {
		pmdb->MovePossibleFlag.z_pls = true;
	}
	//Z-
	if (pmdb->MapData[pmdb->PlayerMASS_Z - 1][pmdb->PlayerMASS_X].NowData == MASS_WALL) {
		//�����蔻�苗�����1�}�X�O�̕ǂ��߂�������C��
		if ((pmdb->PlayerPos_Z) < (MASS_SIZE * (pmdb->PlayerMASS_Z - 0.5) + pmdb->PlayerHitRad)) {
			pmdb->MovePossibleFlag.z_min = false;
		}
		else {
			pmdb->MovePossibleFlag.z_min = true;
		}
	}
	else {
		pmdb->MovePossibleFlag.z_min = true;
	}

	//�i�߂Ȃ��ӏ��ɑ΂���C��

	if ((float)tan(pmdb->PlayerSpinAngle - (DX_PI_F / 4.0f)) >= 0) {
		if (pmdb->MovePossibleFlag.x_min == false) {
			pmdb->CamPos.x += pmdb->PlayerMoveSpeed;
		}
		if (pmdb->MovePossibleFlag.x_pls == false) {
			pmdb->CamPos.x -= pmdb->PlayerMoveSpeed;
		}
		if (pmdb->MovePossibleFlag.z_pls == false) {
			pmdb->CamPos.z -= pmdb->PlayerMoveSpeed;
		}
		if (pmdb->MovePossibleFlag.z_min == false) {
			pmdb->CamPos.z += pmdb->PlayerMoveSpeed;
		}
	}

	else if ((float)tan(pmdb->PlayerSpinAngle - (DX_PI_F / 4.0f)) < 0) {
		if (pmdb->MovePossibleFlag.x_min == false) {
			pmdb->CamPos.x += pmdb->PlayerMoveSpeed;
		}
		if (pmdb->MovePossibleFlag.x_pls == false) {
			pmdb->CamPos.x -= pmdb->PlayerMoveSpeed;
		}
		if (pmdb->MovePossibleFlag.z_pls == false) {
			pmdb->CamPos.z -= pmdb->PlayerMoveSpeed;
		}
		if (pmdb->MovePossibleFlag.z_min == false) {
			pmdb->CamPos.z += pmdb->PlayerMoveSpeed;
		}
	}

	//��������
	ChangeLightTypePoint(pmdb->CamPos, MASS_SIZE * 30 , 0.006f, 0.0004f, 0.0f);

	//���݂̃}�X���X�V
	pmdb->PlayerPos_X = pmdb->CamPos.x;
	pmdb->PlayerPos_Z = pmdb->CamPos.z;

	//�����_�𒲐�
	pmdb->CamTarg = VAdd(pmdb->CamPos, pmdb->CamDir);

	//�J�M���擾
	if (pmdb->MapData[pmdb->PlayerMASS_Z][pmdb->PlayerMASS_X].NowData == MASS_KEY) {
		pmdb->MapData[pmdb->PlayerMASS_Z][pmdb->PlayerMASS_X].NowData = MASS_ROAD;
		pmdb->KeyNum++;
		PlaySoundMem(pmdb->mSound.GetKey, DX_PLAYTYPE_BACK);
	}

	// ���� //
	//20�b�Ԋu
	if (pmdb->TimeCount % (20 * Second_flame) == 0) {

		if (FearSoundCount % 3 == 0) {
			PlaySoundMem(pmdb->mSound.Fear01, DX_PLAYTYPE_BACK);
		}
		else if (FearSoundCount % 3 == 1) {
			PlaySoundMem(pmdb->mSound.Fear02, DX_PLAYTYPE_BACK);
		}
		else if (FearSoundCount % 3 == 2) {
			PlaySoundMem(pmdb->mSound.Fear03, DX_PLAYTYPE_BACK);
		}

		FearSoundCount++;
	}
}

/////////////////////////////////////////////////////////////////////
///////                    �ړ��L�[����                        //////
/////////////////////////////////////////////////////////////////////
void PlayerOperation::KeyCheck(MasterDataBase* pmdb) {

	// �Ђ����]
	if ((bool)CheckHitKey(KEY_INPUT_LEFT) == true) {
		pmdb->PlayerSpinAngle += DX_PI_F / 50.0f;
		pmdb->CamDir.x = (float)cos(pmdb->PlayerSpinAngle);
		pmdb->CamDir.z = (float)sin(pmdb->PlayerSpinAngle);
		pmdb->CamTarg = VAdd(pmdb->CamPos, pmdb->CamDir);
	}

	// �݂���]
	else if ((bool)CheckHitKey(KEY_INPUT_RIGHT) == true) {
		pmdb->PlayerSpinAngle -= DX_PI_F / 50.0f;
		pmdb->CamDir.x = (float)cos(pmdb->PlayerSpinAngle);
		pmdb->CamDir.z = (float)sin(pmdb->PlayerSpinAngle);
		pmdb->CamTarg = VAdd(pmdb->CamPos, pmdb->CamDir);
		//����
		//PlaySoundMem(pmdb->mSound.Walk, DX_PLAYTYPE_BACK);
	}

	// ���łɃX�^�~�i�Ǘ��������� //

	//�_�b�V��
	if (pmdb->DashPossibleFlag == true) {
		if ((bool)CheckHitKey(KEY_INPUT_LSHIFT) == true) {
			pmdb->PlayerMoveSpeed = PlayerMoveSpeed_Dash;
			Icon = pmdb->mIcon.Dash;
		}
		else {
			pmdb->PlayerMoveSpeed = PlayerMoveSpeed_Defalt;
		}
		//�X�^�~�i��
		if (pmdb->PlayerStamina < PlayerStaminaMAX) {
			pmdb->PlayerStamina += 0.6f;
		}
	}
	//�X�^�~�i�؂�
	if (pmdb->PlayerStamina <= 0) {
		//�P����
		PlaySoundMem(pmdb->mSound.TiredCoughing, DX_PLAYTYPE_BACK);

		pmdb->DashPossibleFlag = false;
		pmdb->PlayerMoveSpeed = PlayerMoveSpeed_Tired;
	}
	if (pmdb->DashPossibleFlag == false) {
		pmdb->PlayerStamina += 1.0f;
		if (pmdb->PlayerStamina >= PlayerStaminaMAX) {
			pmdb->DashPossibleFlag = true;
		}
	}

	//������������Ă���

	if (((bool)CheckHitKey(KEY_INPUT_W) == true || (bool)CheckHitKey(KEY_INPUT_S) == true)
		&& ((bool)CheckHitKey(KEY_INPUT_A) == true || (bool)CheckHitKey(KEY_INPUT_D) == true)) {

		// �܂�
		if ((bool)CheckHitKey(KEY_INPUT_W) == true) {
			//����
			//PlaySoundMem(pmdb->mSound.Walk, DX_PLAYTYPE_BACK);
			pmdb->CamPos = VAdd(pmdb->CamPos, VScale(pmdb->CamDir, pmdb->PlayerMoveSpeed / 1.5));
		}
		//������
		else if ((bool)CheckHitKey(KEY_INPUT_S) == true) {
			pmdb->CamPos = VSub(pmdb->CamPos, VScale(pmdb->CamDir, pmdb->PlayerMoveSpeed / 1.5));

		}
		//�Ђ���i�ړ��j
		if ((bool)CheckHitKey(KEY_INPUT_A) == true) {
			pmdb->CamPos = VAdd(pmdb->CamPos, VScale(VCross(pmdb->CamDir, pmdb->CamDirHeight), pmdb->PlayerMoveSpeed / 1.5));
		}

		//�݂��i�ړ��j
		else if ((bool)CheckHitKey(KEY_INPUT_D) == true) {
			pmdb->CamPos = VAdd(pmdb->CamPos, VScale(VCross(pmdb->CamDirHeight, pmdb->CamDir), pmdb->PlayerMoveSpeed / 1.5));
		}
		//�_�b�V��
		if (pmdb->PlayerMoveSpeed == PlayerMoveSpeed_Dash) {
			pmdb->PlayerStamina -= 1.0f;
			Icon = pmdb->mIcon.Dash;
		}
	}

	//���ʂɓ����Ƃ�
	else {
		// �܂�
		if ((bool)CheckHitKey(KEY_INPUT_W) == true) {
			//����
			//PlaySoundMem(pmdb->mSound.Walk, DX_PLAYTYPE_BACK);
			pmdb->CamPos = VAdd(pmdb->CamPos, VScale(pmdb->CamDir, pmdb->PlayerMoveSpeed));
			if (pmdb->PlayerMoveSpeed == PlayerMoveSpeed_Dash) {
				pmdb->PlayerStamina -= 1.0f;
			}


		}
		//������
		else if ((bool)CheckHitKey(KEY_INPUT_S) == true) {
			pmdb->CamPos = VSub(pmdb->CamPos, VScale(pmdb->CamDir, pmdb->PlayerMoveSpeed));
			if (pmdb->PlayerMoveSpeed == PlayerMoveSpeed_Dash) {
				pmdb->PlayerStamina -= 1.0f;
			}

		}
		//�Ђ���i�ړ��j
		else if ((bool)CheckHitKey(KEY_INPUT_A) == true) {
			pmdb->CamPos = VAdd(pmdb->CamPos, VScale(VCross(pmdb->CamDir, pmdb->CamDirHeight), pmdb->PlayerMoveSpeed));
			if (pmdb->PlayerMoveSpeed == PlayerMoveSpeed_Dash) {
				pmdb->PlayerStamina -= 1.0f;
			}
		}

		//�݂��i�ړ��j
		else if ((bool)CheckHitKey(KEY_INPUT_D) == true) {
			pmdb->CamPos = VAdd(pmdb->CamPos, VScale(VCross(pmdb->CamDirHeight, pmdb->CamDir), pmdb->PlayerMoveSpeed));
			if (pmdb->PlayerMoveSpeed == PlayerMoveSpeed_Dash) {
				pmdb->PlayerStamina -= 1.0f;
			}
		}
	}

	//����
	if ((bool)CheckHitKey(KEY_INPUT_W) == true ||
		(bool)CheckHitKey(KEY_INPUT_A) == true ||
		(bool)CheckHitKey(KEY_INPUT_S) == true ||
		(bool)CheckHitKey(KEY_INPUT_D) == true) {
		if (pmdb->PlayerMoveSpeed == PlayerMoveSpeed_Dash) {
			if (FootSoundCount % 20 == 0) {
				PlaySoundMem(pmdb->mSound.Walk, DX_PLAYTYPE_BACK);
			}
		}
		else {
			if (FootSoundCount % 40 == 0) {
				PlaySoundMem(pmdb->mSound.Walk, DX_PLAYTYPE_BACK);
			}
		}
		FootSoundCount++;
	}

	//C�œ���
	if (CheckHitKey(KEY_INPUT_C) == true && pmdb->ClearModeCoolTime <= 0) {
		pmdb->ClearModeFlag = true;
		pmdb->ClearModeCoolTime = CLEAR_COOLTIME;
		pmdb->RemainingClearTime = CLEAR_TIME_MAX;
	}

	//ESC�L�[
	if (CheckHitKey(KEY_INPUT_ESCAPE) == true && pmdb->PushKeyFlag.Space == false) {
		PauseFlag = true;
		pmdb->PushKeyFlag.Space = true;
	}
	else if (CheckHitKey(KEY_INPUT_ESCAPE) != true && pmdb->PushKeyFlag.Space == true) {
		pmdb->PushKeyFlag.Space = false;
	}

	//�S�[��
	if (pmdb->MapData[pmdb->PlayerMASS_Z][pmdb->PlayerMASS_X].NowData == MASS_GOAL &&
		pmdb->KeyNum == KEY_MAX &&
		(bool)CheckHitKey(KEY_INPUT_F) == true) {
		pmdb->NowStageNum += 1;
		if (pmdb->NowStageNum > STAGE_MAX) {
			pmdb->NowStageNum = 1;
		}
		pmdb->ExecutionMode = Mode_Clear;
	}
}

//////////////////////////////////////////////////////////////////////
///////						��ʕ\��							//////
//////////////////////////////////////////////////////////////////////
void PlayerOperation::InterFace(MasterDataBase* pmdb) {

	//test DEBUG_MODE
	if ((bool)CheckHitKey(KEY_INPUT_RSHIFT) == true) {
		DrawFormatStringToHandle(50, 60, (int)GetColor(100, 100, 100), pmdb->mFont.ElementFont, "X = %d", pmdb->PlayerMASS_X);
		DrawFormatStringToHandle(50, 80, (int)GetColor(100, 100, 100), pmdb->mFont.ElementFont, "Z = %d", pmdb->PlayerMASS_Z);
		DrawFormatStringToHandle(50, 100, (int)GetColor(100, 100, 100), pmdb->mFont.ElementFont, "Xp = %6f", pmdb->PlayerPos_X);
		DrawFormatStringToHandle(50, 120, (int)GetColor(100, 100, 100), pmdb->mFont.ElementFont, "Zp = %6f", pmdb->PlayerPos_Z);
		DrawFormatStringToHandle(50, 140, (int)GetColor(100, 100, 100), pmdb->mFont.ElementFont, "St = %4f", pmdb->PlayerStamina);
		pmdb->ClearModeFlag = true;
		pmdb->PlayerMoveSpeed = PlayerMoveSpeed_Dash;
	}

	//�R�����g�̍X�V
	if (pmdb->DashPossibleFlag == false) {
		Comment[0] = "�u�����ꂵ���B�v";
		Icon = pmdb->mIcon.DashNot;
	}
	else if (pmdb->MapData[pmdb->PlayerMASS_Z][pmdb->PlayerMASS_X].NowData == MASS_GOAL) {
		Comment[0] = "��̊K�ɍs�������ȋ@�\�B\n�u�T���B���̑��݈Ӌ`�B�v";
		Icon = pmdb->mIcon.Lock;
		if (pmdb->KeyNum == 1) {
			Comment[0] = "��̊K�ɍs�������ȋ@�\�B\n�u�����K�v�Ȃ̂��́A\n�������Ă��邾�낤�H�v";
		}
		else if (pmdb->KeyNum == 2 ) {
			Comment[0] = "��̊K�ɍs�������ȋ@�\�B\n�u�l���鎞�Ԃ͂���B\n�����͈��߂Ȃ����B�v";
		}
		else if (pmdb->KeyNum == KEY_MAX) {
			Comment[0] = "����œ����������B\n�u���߂łƂ��B\n�����Ă悤�����B�v";
			Icon = pmdb->mIcon.Unlock;
		}
	}
	else if (pmdb->KeyNum == KEY_MAX - 1) {
		Comment[0] = "���ƂЂƂB\n�u�����ĉ^�ƈꈬ��̗E�C�B�v";
	}
	else if (pmdb->KeyNum == KEY_MAX) {
		Comment[0] = "�����͖��S�B\n�u�]�ނ��͎̂�ɓ������H�v";
	}
	//�T���o��
	else if (pmdb->TimeCount / Second_flame / Second_flame >= 5) {
		Comment[0] = "�������󂢂Ă����B\n�u�˂��`�A�܂��`�H�v";
	}
	else {
		Comment[0] = NULL;
	}
	if (Comment[0] == NULL) {
		Comment[0] = "�ő��Ƌْ��ɓh�ꂽ�Î₪\n���Ɣw�؂ɂ܂Ƃ���";
	}

	//�R�����g
	DrawModiGraph(
		/*����*/ WINDOW_MAX_X / 2 - CommentWindow_Width / 2,		/*Y*/ WINDOW_MAX_Y,
		/*�E��*/ WINDOW_MAX_X / 2 + CommentWindow_Width / 2,		/*Y*/ WINDOW_MAX_Y,
		/*�E��*/ WINDOW_MAX_X / 2 + CommentWindow_Width / 2,		/*Y*/ WINDOW_MAX_Y - CommentWindow_Height,
		/*����*/ WINDOW_MAX_X / 2 - CommentWindow_Width / 2,		/*Y*/ WINDOW_MAX_Y - CommentWindow_Height,
		pmdb->mGraph.Window, TRUE);
	//��
	DrawFormatStringToHandle(WINDOW_MAX_X / 2 - CommentWindow_Width / 2 + 30, WINDOW_MAX_Y - CommentWindow_Height / 2 - 40,
		(int)GetColor(200, 200, 200), pmdb->mFont.CommentFont,"[�f�r�����Ȃ�]", KEY_MAX - pmdb->KeyNum);
	DrawFormatStringToHandle(WINDOW_MAX_X / 2 - CommentWindow_Width / 2 + 30, WINDOW_MAX_Y - CommentWindow_Height / 2 - 20,
		(int)GetColor(200, 200, 200), pmdb->mFont.CommentFont, *Comment, KEY_MAX - pmdb->KeyNum);
	

	// �s�N�g�O�����A�C�R�� //
	//�S�[���n�_
	if (pmdb->MapData[pmdb->PlayerMASS_Z][pmdb->PlayerMASS_X].NowData == MASS_GOAL) {
		Icon = pmdb->mIcon.Lock;
		if (pmdb->KeyNum == KEY_MAX) {
			Icon = pmdb->mIcon.Unlock;
		}
	}
	//�_�b�V��
	else if ((pmdb->DashPossibleFlag == true) && ((bool)CheckHitKey(KEY_INPUT_LSHIFT) == true)) {
		Icon = pmdb->mIcon.Dash;
	}
	else if (pmdb->DashPossibleFlag == false) {
		Icon = pmdb->mIcon.DashNot;
	}
	//��
	else if (pmdb->ClearModeCoolTime > 0) {
		Icon = pmdb->mIcon.EyeNot;
		
	}
	else {
		Icon = pmdb->mIcon.Eye;
	}

	//�E�B���h�E
	DrawModiGraph(
		/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height,
		/*�E��*/ WINDOW_MAX_X / 6 + ConfigWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height,
		/*�E��*/ WINDOW_MAX_X / 6 + ConfigWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y,
		/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y,
		pmdb->mGraph.Window, TRUE);

	//�A�C�R��
	DrawModiGraph(
		/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 20,	/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height + 30,
		/*�E��*/ WINDOW_MAX_X / 6 + ConfigWindow_Width / 2 - 20,	/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height + 30,
		/*�E��*/ WINDOW_MAX_X / 6 + ConfigWindow_Width / 2 - 20,	/*Y*/ WINDOW_MAX_Y - 10,
		/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 20,	/*Y*/ WINDOW_MAX_Y - 10,
		Icon, TRUE);
	if ((pmdb->ClearModeCoolTime > 0) && (Icon == pmdb->mIcon.EyeNot)){
		DrawFormatStringToHandle(WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 20,	/*Y*/ WINDOW_MAX_Y - 30, (int)GetColor(200, 200, 200),
			pmdb->mFont.CommentFont, "%02d.%02d",
			pmdb->ClearModeCoolTime / Second_flame % Second_flame,
			(pmdb->ClearModeCoolTime % Second_flame) * 16);
	}
	
	//���
	DrawStringToHandle(WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 5,	/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height + 10,
		"CONDITION", (int)GetColor(200, 200, 200), pmdb->mFont.CommentFont);
	

	//Time
	DrawFormatStringToHandle(WINDOW_MAX_X - 130, 10, (int)GetColor(200, 200, 200),
		pmdb->mFont.CommentFont, "TIME:%02d.%02d", pmdb->TimeCount / Second_flame / Second_flame, (pmdb->TimeCount / Second_flame) % Second_flame);
	
	//Esc
	DrawFormatStringToHandle(10, 10, (int)GetColor(200, 200, 200), pmdb->mFont.CommentFont, "ESC:Menu");

	//����A�V�X�g�E�B���h�E(�݂��̂��)
	DrawModiGraph(
		/*����*/ WINDOW_MAX_X * 7 / 8 - MaterWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height,
		/*�E��*/ WINDOW_MAX_X * 7 / 8 + MaterWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height,
		/*�E��*/ WINDOW_MAX_X * 7 / 8 + MaterWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y,
		/*����*/ WINDOW_MAX_X * 7 / 8 - MaterWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y,
		pmdb->mGraph.Window, TRUE);
	
	//����
	DrawStringToHandle(WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2,/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 3 / 5 - 20,
						"[�_�f]", (int)GetColor(200, 200, 200), pmdb->mFont.CommentFont);
	//�J�M
	for (int i = 0; i < pmdb->KeyNum; i++) {
		DrawRotaGraph(WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2 + (i * 30), WINDOW_MAX_Y - MaterWindow_Height * 1 / 5 - 5,
			0.3, 0.0, pmdb->mIcon.Key, TRUE);
	}
	/*�@�ړ����ɍ��W�������B���j���[�Ƃ��Ȃ�g����H*
	//�擾��KEY
	GetKey = pmdb->CamPos;
	GetKey = VAdd(GetKey, VScale(pmdb->CamDir, 100.0f));
	GetKey = VSub(GetKey, VScale(VCross(pmdb->CamDir, pmdb->CamDirHeight), 60.0f));
	MV1SetScale(pmdb->mModel.Key, VGet(0.05f, 0.05f, 0.05f));
	MV1SetPosition(pmdb->mModel.Key, GetKey);
	MV1DrawModel(pmdb->mModel.Key);
	MV1SetScale(pmdb->mModel.Key, VGet(1.0f, 1.0f, 1.0f));
	*/

	//�X�^�~�i�Q�[�W
	//if (pmdb->PlayerStamina < PlayerStaminaMAX) {

		//�o�[�̘g
	DrawModiGraph(
		/*����*/ WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2,		/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 4 / 8,
		/*�E��*/ WINDOW_MAX_X * 7 / 8 + StaminaGuage_DenWidth / 2 + 4,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 4 / 8,
		/*�E��*/ WINDOW_MAX_X * 7 / 8 + StaminaGuage_DenWidth / 2 + 4,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 4 / 8 - StaminaGuage_DenHeight,
		/*����*/ WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2,		/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 4 / 8 - StaminaGuage_DenHeight,
		pmdb->mGraph.HPFrame, TRUE);

	//�o�[�����̒���
	StaminaBarWidth = StaminaGuage_DenWidth * (pmdb->PlayerStamina / PlayerStaminaMAX);

	//�o�[���g
		//-2���k�������Ęg��\��������
	DrawModiGraph(
		/*����*/ WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2 + 2,						/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 4 / 8 - 2,
		/*�E��*/ WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2 + StaminaBarWidth + 2,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 4 / 8 - 2,
		/*�E��*/ WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2 + StaminaBarWidth + 2,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 4 / 8 + 2 - StaminaGuage_DenHeight,
		/*����*/ WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2 + 2,						/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 4 / 8 + 2 - StaminaGuage_DenHeight,
		pmdb->mGraph.HPBar, TRUE);

	if (pmdb->MapData[pmdb->PlayerMASS_Z][pmdb->PlayerMASS_X].NowData == MASS_GOAL) {
		if (pmdb->KeyNum == KEY_MAX) {
			//�S�[���̕���
			if (pmdb->TimeCount % 2* Second_flame <= Second_flame) {
				DrawFormatStringToHandle(WINDOW_MAX_X / 2 - 100, WINDOW_MAX_Y / 2,
					(int)GetColor(200, 200, 200), pmdb->mFont.ElementFont, "[PRESS -F- KEY]", KEY_MAX - pmdb->KeyNum);
			}
		}
	}

	if (PauseFlag == true) {
		PauseMenu(pmdb);
	}
}

//�|�[�Y���j���[
void PlayerOperation::PauseMenu(MasterDataBase* pmdb) {

	pmdb->TimeFlag = false;

	//ESC�L�[�ŕ���
	if (CheckHitKey(KEY_INPUT_ESCAPE) == true && pmdb->PushKeyFlag.Space == false) {
		pmdb->PushKeyFlag.Space = true;
		PauseFlag = false;
		pmdb->TimeFlag = true;
	}
	//����
	else if (((bool)CheckHitKey(KEY_INPUT_SPACE) == true || (bool)CheckHitKey(KEY_INPUT_RETURN) == true) && pmdb->PushKeyFlag.Space == false) {
		pmdb->PushKeyFlag.Space = true;
		PauseFlag = false;
		switch (PSelectNum) {
		case PSelect_Continue:
			PauseFlag = false;
			pmdb->TimeFlag = true;
			break;
		case PSelect_Retry:
			pmdb->ExecutionMode = Mode_Clear;
			pmdb->TimeFlag = true;
			break;
		case PSelect_Exit:
			pmdb->ExecutionMode = Mode_StartMenu;
			pmdb->TimeFlag = true;
			break;
		}
	}
	//�����ESC�𓯂��t���O�ŊǗ�
	else if (CheckHitKey(KEY_INPUT_ESCAPE) != true && pmdb->PushKeyFlag.Space == true) {
		pmdb->PushKeyFlag.Space = false;
	}

	//���L�[����
	if (((bool)CheckHitKey(KEY_INPUT_DOWN) == true || (bool)CheckHitKey(KEY_INPUT_S) == true) && pmdb->PushKeyFlag.Down == false) {
		// ���݂̑I�����ڂ�����ɂ��炷(�v���X����)
		PSelectNum = (PSelectNum + 1) % PauseMenuElementMax;
		pmdb->PushKeyFlag.Down = true;
		PlaySoundMem(pmdb->mSound.Select, DX_PLAYTYPE_BACK);
	}
	//�L�[�𗣂�����Ԃɂ���
	else if (((bool)CheckHitKey(KEY_INPUT_DOWN) != true && pmdb->PushKeyFlag.Down == true) && ((bool)CheckHitKey(KEY_INPUT_S) != true && pmdb->PushKeyFlag.Down == true)) {
		pmdb->PushKeyFlag.Down = false;
	}

	//��L�[����
	else if (((bool)CheckHitKey(KEY_INPUT_UP) == true || (bool)CheckHitKey(KEY_INPUT_W) == true) && pmdb->PushKeyFlag.Up == false) {
		//���݂̑I�����ڂ�����ɂ��炷(�}�C�i�X����)
		PSelectNum = (PSelectNum - 1) % PauseMenuElementMax;
		pmdb->PushKeyFlag.Up = true;
		PlaySoundMem(pmdb->mSound.Select, DX_PLAYTYPE_BACK);
	}
	//�L�[�𗣂�����Ԃɂ���
	else if (((bool)CheckHitKey(KEY_INPUT_UP) != true && pmdb->PushKeyFlag.Up == true) && ((bool)CheckHitKey(KEY_INPUT_W) != true && pmdb->PushKeyFlag.Up == true)) {
		pmdb->PushKeyFlag.Up = false;//�f�B���C
	}

	// �ی�
	if (PSelectNum < 0) {
		PSelectNum = PauseMenuElementMax - 1;//  �������Ƀ��[�v������
	}
	else if (PSelectNum >= PauseMenuElementMax) {
		PSelectNum = 0;//  �������Ƀ��[�v������
	}


	// �E�C���h�E
	DrawModiGraph(
		/*����*/ WINDOW_MAX_X * 2 / 7 - 10,	WINDOW_MAX_Y * 2 / 7 - 10,
		/*�E��*/ WINDOW_MAX_X * 5 / 7,		WINDOW_MAX_Y * 2 / 7 - 10,
		/*�E��*/ WINDOW_MAX_X * 5 / 7,		WINDOW_MAX_Y * 5 / 7,
		/*����*/ WINDOW_MAX_X * 2 / 7 - 10,	WINDOW_MAX_Y * 5 / 7,
		pmdb->mGraph.Window, TRUE);

	//���ړ��e��`��
	for (int i = 0; i < PauseMenuElementMax; i++) {
		//��I������
		if (i != PSelectNum) {
			DrawStringToHandle(PMenuElements[i].x, PMenuElements[i].y, PMenuElements[i].name, (int)GetColor(255,255,255), pmdb->mFont.ElementFont);
		}

		//�I�𒆂̍���
		else if (i == PSelectNum) {
			//�I���A�C�R���̕`��
			int x = pmdb->TimeCount % 2* Second_flame;
			if (x <= Second_flame) {
				DrawStringToHandle(PMenuElements[i].x + (x / 4) - 10, PMenuElements[i].y, ">", (int)GetColor(255, 100, 100), pmdb->mFont.ElementFont);
			}
			else if (x > Second_flame) {
				DrawStringToHandle(PMenuElements[i].x + (30 - x / 4) - 10, PMenuElements[i].y, ">", (int)GetColor(255, 100, 100), pmdb->mFont.ElementFont);
			}
			//�I�𒆂̍��ڂ͉��ɂ��炵�ĕ\��
			DrawStringToHandle(PMenuElements[i].x + 20, PMenuElements[i].y, PMenuElements[i].name, (int)GetColor(255, 100, 100), pmdb->mFont.ElementFont);
		}
	}
}

void PlayerOperation::TutorialWindow(MasterDataBase* pmdb) {

	pmdb->TimeFlag = false;

	DrawModiGraph(
		/*����*/ WINDOW_MAX_X / 6 - 20,	/*Y*/ WINDOW_MAX_Y / 5 - 20,
		/*�E��*/ WINDOW_MAX_X * 5 / 6,	/*Y*/ WINDOW_MAX_Y / 5 - 20,
		/*�E��*/ WINDOW_MAX_X * 5 / 6,	/*Y*/ WINDOW_MAX_Y / 5 + 140,
		/*����*/ WINDOW_MAX_X / 6 - 20,	/*Y*/ WINDOW_MAX_Y / 5 + 140,
		pmdb->mGraph.Window, TRUE);

	if (pmdb->ExecutionMode == Mode_StartMenu) {
		DrawStringToHandle(WINDOW_MAX_X / 6, WINDOW_MAX_Y / 5, "[��{�������]�@��F�L�[�Ŗ߂�", (int)GetColor(100, 200, 100), pmdb->mFont.CommentFont, TRUE);
	}
	else {
		DrawStringToHandle(WINDOW_MAX_X / 6, WINDOW_MAX_Y / 5, "[��{�������]�@��F�L�[�ŃQ�[���J�n", (int)GetColor(100, 200, 100), pmdb->mFont.CommentFont, TRUE);
	}

	DrawStringToHandle(WINDOW_MAX_X / 6, WINDOW_MAX_Y / 5 + 20, "  W\nA S D�@�L�[�ňړ��B", (int)GetColor(100, 200, 100), pmdb->mFont.CommentFont, TRUE);
	DrawStringToHandle(WINDOW_MAX_X * 3 / 6, WINDOW_MAX_Y / 5 + 20, "\n���@���@�L�[�Ŏ��_�ړ��B", (int)GetColor(100, 200, 100), pmdb->mFont.CommentFont, TRUE);
	DrawStringToHandle(WINDOW_MAX_X / 6, WINDOW_MAX_Y / 5 + 60, "�EE�L�[�E�E�E�E�B���h�E��\��\n�EC�L�[�E�E�E���ߎg�p", (int)GetColor(100, 200, 100), pmdb->mFont.CommentFont, TRUE);
	DrawStringToHandle(WINDOW_MAX_X / 6, WINDOW_MAX_Y / 5 + 100, "[�}�E�X�J�[�\���Ŋe�E�B���h�E�̏ڍו\��]", (int)GetColor(100, 200, 100), pmdb->mFont.CommentFont, TRUE);


	//����
	if (/*��*/ pmdb->MouseCursor_X > WINDOW_MAX_X - 130 &&
		/*�E*/ pmdb->MouseCursor_X < WINDOW_MAX_X &&
		/*��*/ pmdb->MouseCursor_Y > 0 &&
		/*��*/ pmdb->MouseCursor_Y < 30) {
		DrawModiGraph(
			/*����*/ WINDOW_MAX_X - 140,	/*Y*/ 25,
			/*�E��*/ WINDOW_MAX_X - 40,		/*Y*/ 25,
			/*�E��*/ WINDOW_MAX_X - 40,		/*Y*/ 50,
			/*����*/ WINDOW_MAX_X - 140,	/*Y*/ 50,
			pmdb->mGraph.Window, TRUE);
		DrawStringToHandle(WINDOW_MAX_X - 130, 30, "[�o�ߎ���]", (int)GetColor(100, 200, 100), pmdb->mFont.TutorialFont, TRUE);
	}

	//�_�f
	if (/*��*/ pmdb->MouseCursor_X > WINDOW_MAX_X * 6 / 8 &&
		/*�E*/ pmdb->MouseCursor_X < WINDOW_MAX_X - 10 &&
		/*��*/ pmdb->MouseCursor_Y > WINDOW_MAX_Y - MaterWindow_Height &&
		/*��*/ pmdb->MouseCursor_Y < WINDOW_MAX_Y - MaterWindow_Height / 2) {
		DrawModiGraph(
			/*����*/ WINDOW_MAX_X * 4 / 8 - 10,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height - 60,
			/*�E��*/ WINDOW_MAX_X - 20,		/*Y*/ WINDOW_MAX_Y - MaterWindow_Height - 60,
			/*�E��*/ WINDOW_MAX_X - 20,		/*Y*/ WINDOW_MAX_Y - MaterWindow_Height - 10,
			/*����*/ WINDOW_MAX_X * 4 / 8 - 10,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height - 10,
			pmdb->mGraph.Window, TRUE);
		DrawStringToHandle(WINDOW_MAX_X * 4 / 8, WINDOW_MAX_Y - MaterWindow_Height - 50,
			"[�_�f]����ƌ����A����Ȃ���Ή񕜁B\n���؂ꂷ��ƕ����̂�����t�ɂȂ�B", (int)GetColor(100, 200, 100), pmdb->mFont.TutorialFont, TRUE);
	}

	//�J�M
	for (int i = 0; i < (CTIconCount / Second_flame / 2) % 5; i++) {
		DrawRotaGraph(WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2 + (i * 30), WINDOW_MAX_Y - MaterWindow_Height * 1 / 5 - 5,
			0.3, 0.0, pmdb->mIcon.Key, TRUE);
	}
	if (/*��*/ pmdb->MouseCursor_X > WINDOW_MAX_X * 6 / 8 &&
		/*�E*/ pmdb->MouseCursor_X < WINDOW_MAX_X - 10 &&
		/*��*/ pmdb->MouseCursor_Y > WINDOW_MAX_Y - MaterWindow_Height / 2 &&
		/*��*/ pmdb->MouseCursor_Y < WINDOW_MAX_Y) {
		DrawModiGraph(
			/*����*/ WINDOW_MAX_X * 4 / 8 - 10,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height - 10,
			/*�E��*/ WINDOW_MAX_X - 60,			/*Y*/ WINDOW_MAX_Y - MaterWindow_Height - 10,
			/*�E��*/ WINDOW_MAX_X - 60,			/*Y*/ WINDOW_MAX_Y - MaterWindow_Height / 2 + 5,
			/*����*/ WINDOW_MAX_X * 4 / 8 - 10,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height / 2 + 5,
			pmdb->mGraph.Window, TRUE);
		DrawStringToHandle(WINDOW_MAX_X * 4 / 8, WINDOW_MAX_Y - MaterWindow_Height,
			"[��]�W�߂����B4�S�ďW�߂��\n�������炨�T���_�o�[�B", (int)GetColor(100, 200, 100), pmdb->mFont.TutorialFont, TRUE);
	}

	//�R�����g�E�B���h
	if (/*��*/ pmdb->MouseCursor_X > WINDOW_MAX_X / 2 - CommentWindow_Width / 2 - 20 &&
		/*�E*/ pmdb->MouseCursor_X < WINDOW_MAX_X / 2 + CommentWindow_Width / 2 - 20 &&
		/*��*/ pmdb->MouseCursor_Y > WINDOW_MAX_Y - CommentWindow_Height &&
		/*��*/ pmdb->MouseCursor_Y < WINDOW_MAX_Y) {
		//�R�����g
		DrawModiGraph(
			/*����*/ WINDOW_MAX_X / 2 - CommentWindow_Width / 2,		/*Y*/ WINDOW_MAX_Y - CommentWindow_Height - 60,
			/*�E��*/ WINDOW_MAX_X / 2 + CommentWindow_Width / 2 - 50,	/*Y*/ WINDOW_MAX_Y - CommentWindow_Height - 60,
			/*�E��*/ WINDOW_MAX_X / 2 + CommentWindow_Width / 2 - 50,	/*Y*/ WINDOW_MAX_Y - CommentWindow_Height - 10,
			/*����*/ WINDOW_MAX_X / 2 - CommentWindow_Width / 2,		/*Y*/ WINDOW_MAX_Y - CommentWindow_Height - 10,
			pmdb->mGraph.Window, TRUE);
		DrawStringToHandle(WINDOW_MAX_X / 2 - CommentWindow_Width / 2 + 10, WINDOW_MAX_Y - CommentWindow_Height - 50,
			"�����ȏ��I��l�ɂȂ��\n�R�炵�����Ȃ錾�t������B", (int)GetColor(100, 200, 100), pmdb->mFont.TutorialFont, TRUE);
	}
	//�R���f�B�V�����A�C�R��
	if (/*��*/ pmdb->MouseCursor_X > WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 &&
		/*�E*/ pmdb->MouseCursor_X < WINDOW_MAX_X / 6 + ConfigWindow_Width / 2 &&
		/*��*/ pmdb->MouseCursor_Y > WINDOW_MAX_Y - ConfigWindow_Height &&
		/*��*/ pmdb->MouseCursor_Y < WINDOW_MAX_Y) {
		DrawModiGraph(
			/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
			/*�E��*/ WINDOW_MAX_X / 2 + 70,						/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
			/*�E��*/ WINDOW_MAX_X / 2 + 70,						/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height,
			/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height,
			pmdb->mGraph.Window, TRUE);
		DrawStringToHandle(WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10, WINDOW_MAX_Y - ConfigWindow_Height - 40,
			"[�R���f�B�V����]�q�ϓI�ŊȈՂȏ�Ԑ����B\n�o���A�t���[�ɔz�����Ă���܂��B", (int)GetColor(100, 200, 100), pmdb->mFont.TutorialFont, TRUE);

		//�߂���
		DrawModiGraph(
			/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 140,
			/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 140,
			/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
			/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
			pmdb->mGraph.Window, TRUE);
		DrawModiGraph(
			/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 110,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 140,
			/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 400,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 140,
			/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 400,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
			/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 110,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
			pmdb->mGraph.Window, TRUE);
		DrawStringToHandle(WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 120, WINDOW_MAX_Y - ConfigWindow_Height - 130,
			"[�܊��\���o]\n�������܂����x�������o�ɔ��f����B\n�S���Ɉ����̘A���Ŏg�p�ł��Ȃ��B\n�u�ށv���ǂ��ɂ���̂����ɂ߂悤�B", (int)GetColor(100, 200, 100), pmdb->mFont.TutorialFont, TRUE);
		//
		if (CTIconCount % 2 * Second_flame <= Second_flame) {
			DrawModiGraph(
				/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 140,
				/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 140,
				/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
				/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
				pmdb->mIcon.Eye, TRUE);
		}
		else {
			DrawModiGraph(
				/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 140,
				/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 140,
				/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
				/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
				pmdb->mIcon.EyeNot, TRUE);
		}

		//�_�b�V��
		DrawModiGraph(
			/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 240,
			/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 240,
			/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 150,
			/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 150,
			pmdb->mGraph.Window, TRUE);
		DrawModiGraph(
			/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 110,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 240,
			/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 400,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 240,
			/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 400,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 150,
			/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 110,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 150,
			pmdb->mGraph.Window, TRUE);
		DrawStringToHandle(WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 120, WINDOW_MAX_Y - ConfigWindow_Height - 230,
			"[���j]\n�^���ɂ͎_�f���K�v�s���B\n�^���̌�ɂ̓N�[���_�E�����K�v�B\n���܂߂ɋx������낤�B", (int)GetColor(100, 200, 100), pmdb->mFont.TutorialFont, TRUE);
		//
		if (CTIconCount % 2 * Second_flame <= Second_flame) {
			DrawModiGraph(
				/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 240,
				/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 240,
				/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 150,
				/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 150,
				pmdb->mIcon.Dash, TRUE);
		}
		else {
			DrawModiGraph(
				/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 240,
				/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 240,
				/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 150,
				/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 150,
				pmdb->mIcon.DashNot, TRUE);
		}

		//����
		DrawModiGraph(
			/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 340,
			/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 340,
			/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 250,
			/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 250,
			pmdb->mGraph.Window, TRUE);
		DrawModiGraph(
			/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 110,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 340,
			/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 400,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 340,
			/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 400,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 250,
			/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 110,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 250,
			pmdb->mGraph.Window, TRUE);
		DrawStringToHandle(WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 120, WINDOW_MAX_Y - ConfigWindow_Height - 330,
			"[�K�w�ړ��@�\�̏�]\n�J�����Ă���̂̓L�~���H\n�J�M���W�߂���ԂȂ�J�������B\n��������o�邽�߂̖ڕW�B", (int)GetColor(100, 200, 100), pmdb->mFont.TutorialFont, TRUE);
		//
		if (CTIconCount % 2 * Second_flame <= Second_flame) {
			DrawModiGraph(
				/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 340,
				/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 340,
				/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 250,
				/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 250,
				pmdb->mIcon.Lock, TRUE);
		}
		else {
			DrawModiGraph(
				/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 340,
				/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 340,
				/*�E��*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 250,
				/*����*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 250,
				pmdb->mIcon.Unlock, TRUE);
		}
	}

	if ((bool)CheckHitKey(KEY_INPUT_F) == true) {
			pmdb->TutorialFlag = false;
			pmdb->TimeFlag = true;
		CTIconCount++;
	}
}