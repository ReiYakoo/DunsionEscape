#include "MapData.h"

void MapControll::CleateMap(MasterDataBase* pmdb) {
	//読み取るファイルの名前を作る
	char LoadMapName[50] = { "MapData/Map" };
	if (pmdb->NowStageNum < 10) {
		LoadMapName[strlen(LoadMapName)] = pmdb->NowStageNum + '0';
	}
	else if (pmdb->NowStageNum >= 10) {
		LoadMapName[strlen(LoadMapName)] = pmdb->NowStageNum / 10 + '0';
		LoadMapName[strlen(LoadMapName)] = pmdb->NowStageNum % 10 + '0';
	}
	strcat_s(LoadMapName, ".txt");
	//マップデータを呼ぶ
	FILE* LoadFileData = NULL;
	fopen_s(&LoadFileData, LoadMapName, "rt");
	//エラーメッセージ
	if (LoadFileData == NULL) {
		MessageBox(NULL, "ファイル読み込みエラー", "エラー！", MB_OK);
	}
	else {
		//pmdb->Enemy.clear();

		//マップの情報をコピーしていく
		for (int z = 0; z < MASS_MAX_Z; z++) {
			v_char str;
			for (int x = 0; x < MASS_MAX_X; x++) {
				char c[2];

				fscanf_s(LoadFileData, "%c,\n", c, 1);

				pmdb->MapData[z][x].OriginalData = c[0];
				pmdb->MapData[z][x].NowData = c[0];

				//敵用マップにも格納
				str.push_back(c[0]);
			}
			//pmdb->Enemy.push_back(str);
		}

		//敵の道を構築
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
	//カメラをセット
	SetCameraPositionAndTarget_UpVecY(pmdb->CamPos, pmdb->CamTarg);

	//カギ回し
	KeyAngleY += ROTATE_SPEED;

	if (pmdb->ClearModeFlag == true) {
		if (--pmdb->RemainingClearTime <= 0) {
			pmdb->ClearModeFlag = false;
		}
		//半透明にする
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

	//マスの描画
	for (int z = 0; z < MASS_MAX_Z; z++) {
		for (int x = 0; x < MASS_MAX_X; x++) {

			//道ではないところは描画しない
			if (pmdb->MapData[z][x].NowData == MASS_WALL) continue;
			else {
				// フレーム番号を変更し、壁を表示する面を設定
				int FrameNo = 0;
				if (pmdb->MapData[z][x + 1].NowData == MASS_WALL) { FrameNo += 1; }
				if (pmdb->MapData[z][x - 1].NowData == MASS_WALL) { FrameNo += 2; }
				if (pmdb->MapData[z + 1][x].NowData == MASS_WALL) { FrameNo += 4; }
				if (pmdb->MapData[z - 1][x].NowData == MASS_WALL) { FrameNo += 8; }

				//割り出した番号のフレームを描画する

				//普通の道を描画
				if (pmdb->MapData[z][x].NowData == MASS_ROAD ||
					pmdb->MapData[z][x].NowData == MASS_ENEMY) {

					//モデルの座標を変更する
					MV1SetPosition(pmdb->mModel.Wall, VGet(x * MASS_SIZE, 0.0f, z * MASS_SIZE));

					MV1DrawFrame(pmdb->mModel.Wall, FrameNo);
					MV1DrawFrame(pmdb->mModel.Wall, 16);
				}
				//ゴールを描画
				else if (pmdb->MapData[z][x].NowData == MASS_GOAL) {
					//モデルの座標を変更する
					MV1SetPosition(pmdb->mModel.Wall, VGet(x * MASS_SIZE, 0.0f, z * MASS_SIZE));
					MV1DrawFrame(pmdb->mModel.Wall, FrameNo);
					MV1DrawFrame(pmdb->mModel.Wall, 16);

					//ゴールのへんなの
					MV1SetPosition(pmdb->mModel.Ling, VGet(x * MASS_SIZE, 0.0f, z * MASS_SIZE));
					MV1DrawModel(pmdb->mModel.Ling);
				}

				//鍵
				else if (pmdb->MapData[z][x].NowData == MASS_KEY) {
					//モデルの座標を変更する
					MV1SetPosition(pmdb->mModel.Wall, VGet(x * MASS_SIZE, 0.0f, z * MASS_SIZE));
					MV1DrawFrame(pmdb->mModel.Wall, FrameNo);
					MV1DrawFrame(pmdb->mModel.Wall, 16);
					//鍵をマス上に表示
					MV1SetPosition(pmdb->mModel.Key, VGet(x * MASS_SIZE, 100.0f, z * MASS_SIZE));
					MV1SetRotationXYZ(pmdb->mModel.Key, VGet(0.0f, KeyAngleY, 0.0f));
					MV1DrawModel(pmdb->mModel.Key);
				}
			}
		}
	}
}

void MapControll::MapReset(MasterDataBase* pmdb) {

	//カメラ初期化
	pmdb->CamPos = VGet(MASS_SIZE, CAMERA_Y, MASS_SIZE);
	pmdb->CamDir = VGet(1.0f, 0.0f, 0.0f);
	pmdb->CamTarg = VAdd(pmdb->CamPos, pmdb->CamDir);
	SetCameraPositionAndTarget_UpVecY(pmdb->CamPos, pmdb->CamTarg);

	//変数のリセット
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

	//マップのリロード
	CleateMap(pmdb);

	//ゲーム再開
	pmdb->ExecutionMode = Mode_Gaming;

}