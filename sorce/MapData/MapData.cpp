#include "MapData.h"

void MapControll::CleateMap(MasterDataBase* pmdb) {
	//�ǂݎ��t�@�C���̖��O�����
	char LoadMapName[50] = { "MapData/Map" };
	if (pmdb->NowStageNum < 10) {
		LoadMapName[strlen(LoadMapName)] = pmdb->NowStageNum + '0';
	}
	else if (pmdb->NowStageNum >= 10) {
		LoadMapName[strlen(LoadMapName)] = pmdb->NowStageNum / 10 + '0';
		LoadMapName[strlen(LoadMapName)] = pmdb->NowStageNum % 10 + '0';
	}
	strcat_s(LoadMapName, ".txt");
	//�}�b�v�f�[�^���Ă�
	FILE* LoadFileData = NULL;
	fopen_s(&LoadFileData, LoadMapName, "rt");
	//�G���[���b�Z�[�W
	if (LoadFileData == NULL) {
		MessageBox(NULL, "�t�@�C���ǂݍ��݃G���[", "�G���[�I", MB_OK);
	}
	else {
		//pmdb->Enemy.clear();

		//�}�b�v�̏����R�s�[���Ă���
		for (int z = 0; z < MASS_MAX_Z; z++) {
			v_char str;
			for (int x = 0; x < MASS_MAX_X; x++) {
				char c[2];

				fscanf_s(LoadFileData, "%c,\n", c, 1);

				pmdb->MapData[z][x].OriginalData = c[0];
				pmdb->MapData[z][x].NowData = c[0];

				//�G�p�}�b�v�ɂ��i�[
				str.push_back(c[0]);
			}
			//pmdb->Enemy.push_back(str);
		}

		//�G�̓����\�z
		pmdb->EnemyNum = 0;
		pmdb->EnemyPos.clear();
		pmdb->MapRoad.clear();
		for (int z = 0; z < MASS_MAX_Z; z++) {
			ull n = 0;
			for (int x = 0; x < MASS_MAX_X; x++) {
				if (pmdb->MapData[z][x].NowData != MASS_WALL) {
					n += ((ull)0x1 << x);
				}
				if (pmdb->MapData[z][x].NowData == MASS_ENEMY) {
					pmdb->EnemyNum++;
					pmdb->EnemyPos.push_back(VGet((float)x, 0, (float)z));
				}
			}
			pmdb->MapRoad.push_back(n);
		}
		fclose(LoadFileData);
	}
}

void MapControll::DrawMap(MasterDataBase* pmdb) {
	//�J�������Z�b�g
	SetCameraPositionAndTarget_UpVecY(pmdb->CamPos, pmdb->CamTarg);

	//�J�M��
	KeyAngleY += ROTATE_SPEED;

	if (pmdb->ClearModeFlag == true) {
		if (--pmdb->RemainingClearTime <= 0) {
			pmdb->ClearModeFlag = false;
		}
		//�������ɂ���
		MV1SetOpacityRate(pmdb->mModel.Wall, 0.4f);
		MV1SetUseZBuffer(pmdb->mModel.Wall, FALSE);
		MV1SetWriteZBuffer(pmdb->mModel.Wall, FALSE);
		pmdb->PlayMovieToGraphLoop(WINDOW_MAX_X / 2, WINDOW_MAX_Y / 2, 0.5f, 0.0f, pmdb->mGraph.ClearBack, TRUE);
	}
	else {
		MV1SetOpacityRate(pmdb->mModel.Wall, 1.0f);
		MV1SetUseZBuffer(pmdb->mModel.Wall, TRUE);
		MV1SetWriteZBuffer(pmdb->mModel.Wall, TRUE);
		if (pmdb->TimeFlag == true) {
			pmdb->ClearModeCoolTime--;
		}
	}

	//�}�X�̕`��
	for (int z = 0; z < MASS_MAX_Z; z++) {
		for (int x = 0; x < MASS_MAX_X; x++) {

			//���ł͂Ȃ��Ƃ���͕`�悵�Ȃ�
			if (pmdb->MapData[z][x].NowData == MASS_WALL) continue;
			else {
				// �t���[���ԍ���ύX���A�ǂ�\������ʂ�ݒ�
				int FrameNo = 0;
				if (pmdb->MapData[z][x + 1].NowData == MASS_WALL) { FrameNo += 1; }
				if (pmdb->MapData[z][x - 1].NowData == MASS_WALL) { FrameNo += 2; }
				if (pmdb->MapData[z + 1][x].NowData == MASS_WALL) { FrameNo += 4; }
				if (pmdb->MapData[z - 1][x].NowData == MASS_WALL) { FrameNo += 8; }

				//����o�����ԍ��̃t���[����`�悷��

				//���ʂ̓���`��
				if (pmdb->MapData[z][x].NowData == MASS_ROAD ||
					pmdb->MapData[z][x].NowData == MASS_ENEMY) {

					//���f���̍��W��ύX����
					MV1SetPosition(pmdb->mModel.Wall, VGet(x * MASS_SIZE, 0.0f, z * MASS_SIZE));

					MV1DrawFrame(pmdb->mModel.Wall, FrameNo);
					MV1DrawFrame(pmdb->mModel.Wall, 16);
				}
				//�S�[����`��
				else if (pmdb->MapData[z][x].NowData == MASS_GOAL) {
					//���f���̍��W��ύX����
					MV1SetPosition(pmdb->mModel.Wall, VGet(x * MASS_SIZE, 0.0f, z * MASS_SIZE));
					MV1DrawFrame(pmdb->mModel.Wall, FrameNo);
					MV1DrawFrame(pmdb->mModel.Wall, 16);

					//�S�[���̂ւ�Ȃ�
					MV1SetPosition(pmdb->mModel.Ling, VGet(x * MASS_SIZE, 0.0f, z * MASS_SIZE));
					MV1DrawModel(pmdb->mModel.Ling);
				}

				//��
				else if (pmdb->MapData[z][x].NowData == MASS_KEY) {
					//���f���̍��W��ύX����
					MV1SetPosition(pmdb->mModel.Wall, VGet(x * MASS_SIZE, 0.0f, z * MASS_SIZE));
					MV1DrawFrame(pmdb->mModel.Wall, FrameNo);
					MV1DrawFrame(pmdb->mModel.Wall, 16);
					//�����}�X��ɕ\��
					MV1SetPosition(pmdb->mModel.Key, VGet(x * MASS_SIZE, 100.0f, z * MASS_SIZE));
					MV1SetRotationXYZ(pmdb->mModel.Key, VGet(0.0f, KeyAngleY, 0.0f));
					MV1DrawModel(pmdb->mModel.Key);
				}
			}
		}
	}
}

void MapControll::MapReset(MasterDataBase* pmdb) {

	//�J����������
	pmdb->CamPos = VGet(MASS_SIZE, CAMERA_Y, MASS_SIZE);
	pmdb->CamDir = VGet(1.0f, 0.0f, 0.0f);
	pmdb->CamTarg = VAdd(pmdb->CamPos, pmdb->CamDir);
	SetCameraPositionAndTarget_UpVecY(pmdb->CamPos, pmdb->CamTarg);

	//�ϐ��̃��Z�b�g
	pmdb->KeyNum = 0;
	pmdb->TimeCount = 0;
	pmdb->PlayerStamina = PlayerStaminaMAX;
	pmdb->ClearModeCoolTime = 0;
	pmdb->ClearModeFlag = false;

	pmdb->PlayerMASS_X = 1;
	pmdb->PlayerMASS_Z = 1;
	pmdb->PlayerPos_X = pmdb->PlayerMASS_X * MASS_SIZE;
	pmdb->PlayerPos_Z = pmdb->PlayerMASS_Z * MASS_SIZE;

	pmdb->PlayerSpinAngle = 0.0f;

	//�}�b�v�̃����[�h
	CleateMap(pmdb);

	//�Q�[���ĊJ
	pmdb->ExecutionMode = Mode_Gaming;

}