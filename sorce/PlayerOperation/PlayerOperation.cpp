#include "PlayerOperation.h"
#include "../MapData/MapData.h"

void PlayerOperation::PlayerOperationMain(MasterDataBase* pmdb) {

	//操作受付
	if (pmdb->TimeFlag == true) {
		KeyCheck(pmdb);
	}

	//ゲームに反映
	HitAndPosCheck(pmdb);

}

////////////////////////////////////////////////////////////////
////				操作を反映した処理						////
////////////////////////////////////////////////////////////////
void PlayerOperation::HitAndPosCheck(MasterDataBase* pmdb) {

	//現在地を参照して立っているマスを更新
	if (pmdb->PlayerPos_X >= pmdb->PlayerMASS_X * (MASS_SIZE)+MASS_SIZE / 2) { pmdb->PlayerMASS_X++; }
	if (pmdb->PlayerPos_X < pmdb->PlayerMASS_X * (MASS_SIZE - 1) - MASS_SIZE / 2) { pmdb->PlayerMASS_X--; }
	if (pmdb->PlayerPos_Z >= pmdb->PlayerMASS_Z * (MASS_SIZE) + MASS_SIZE / 2) { pmdb->PlayerMASS_Z++; }
	if (pmdb->PlayerPos_Z < pmdb->PlayerMASS_Z * (MASS_SIZE - 1) - MASS_SIZE / 2) { pmdb->PlayerMASS_Z--; }

	//隣接するマスが壁か判別する
	//X＋
	if (pmdb->MapData[pmdb->PlayerMASS_Z][pmdb->PlayerMASS_X + 1].NowData == MASS_WALL) {
		//当たり判定距離より1マス先の壁が近かったら修正
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
		//当たり判定距離より1マス前の壁が近かったら修正
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
	//Z＋
	if (pmdb->MapData[pmdb->PlayerMASS_Z + 1][pmdb->PlayerMASS_X].NowData == MASS_WALL) {
		//当たり判定距離より1マス先の壁が近かったら修正
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
		//当たり判定距離より1マス前の壁が近かったら修正
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

	//進めない箇所に対する修正

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

	//光源調整
	ChangeLightTypePoint(pmdb->CamPos, MASS_SIZE * 30 , 0.006f, 0.0004f, 0.0f);

	//現在のマスを更新
	pmdb->PlayerPos_X = pmdb->CamPos.x;
	pmdb->PlayerPos_Z = pmdb->CamPos.z;

	//注視点を調整
	pmdb->CamTarg = VAdd(pmdb->CamPos, pmdb->CamDir);

	//カギを取得
	if (pmdb->MapData[pmdb->PlayerMASS_Z][pmdb->PlayerMASS_X].NowData == MASS_KEY) {
		pmdb->MapData[pmdb->PlayerMASS_Z][pmdb->PlayerMASS_X].NowData = MASS_ROAD;
		pmdb->KeyNum++;
		PlaySoundMem(pmdb->mSound.GetKey, DX_PLAYTYPE_BACK);
	}

	// 鳴き声 //
	//20秒間隔
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
///////                    移動キー入力                        //////
/////////////////////////////////////////////////////////////////////
void PlayerOperation::KeyCheck(MasterDataBase* pmdb) {

	// ひだり回転
	if ((bool)CheckHitKey(KEY_INPUT_LEFT) == true) {
		pmdb->PlayerSpinAngle += DX_PI_F / 50.0f;
		pmdb->CamDir.x = (float)cos(pmdb->PlayerSpinAngle);
		pmdb->CamDir.z = (float)sin(pmdb->PlayerSpinAngle);
		pmdb->CamTarg = VAdd(pmdb->CamPos, pmdb->CamDir);
	}

	// みぎ回転
	else if ((bool)CheckHitKey(KEY_INPUT_RIGHT) == true) {
		pmdb->PlayerSpinAngle -= DX_PI_F / 50.0f;
		pmdb->CamDir.x = (float)cos(pmdb->PlayerSpinAngle);
		pmdb->CamDir.z = (float)sin(pmdb->PlayerSpinAngle);
		pmdb->CamTarg = VAdd(pmdb->CamPos, pmdb->CamDir);
		//足音
		//PlaySoundMem(pmdb->mSound.Walk, DX_PLAYTYPE_BACK);
	}

	// ついでにスタミナ管理もここで //

	//ダッシュ
	if (pmdb->DashPossibleFlag == true) {
		if ((bool)CheckHitKey(KEY_INPUT_LSHIFT) == true) {
			pmdb->PlayerMoveSpeed = PlayerMoveSpeed_Dash;
			Icon = pmdb->mIcon.Dash;
		}
		else {
			pmdb->PlayerMoveSpeed = PlayerMoveSpeed_Defalt;
		}
		//スタミナ回復
		if (pmdb->PlayerStamina < PlayerStaminaMAX) {
			pmdb->PlayerStamina += 0.6f;
		}
	}
	//スタミナ切れ
	if (pmdb->PlayerStamina <= 0) {
		//咳込み
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

	//同時押しされてたら

	if (((bool)CheckHitKey(KEY_INPUT_W) == true || (bool)CheckHitKey(KEY_INPUT_S) == true)
		&& ((bool)CheckHitKey(KEY_INPUT_A) == true || (bool)CheckHitKey(KEY_INPUT_D) == true)) {

		// まえ
		if ((bool)CheckHitKey(KEY_INPUT_W) == true) {
			//足音
			//PlaySoundMem(pmdb->mSound.Walk, DX_PLAYTYPE_BACK);
			pmdb->CamPos = VAdd(pmdb->CamPos, VScale(pmdb->CamDir, pmdb->PlayerMoveSpeed / 1.5));
		}
		//うしろ
		else if ((bool)CheckHitKey(KEY_INPUT_S) == true) {
			pmdb->CamPos = VSub(pmdb->CamPos, VScale(pmdb->CamDir, pmdb->PlayerMoveSpeed / 1.5));

		}
		//ひだり（移動）
		if ((bool)CheckHitKey(KEY_INPUT_A) == true) {
			pmdb->CamPos = VAdd(pmdb->CamPos, VScale(VCross(pmdb->CamDir, pmdb->CamDirHeight), pmdb->PlayerMoveSpeed / 1.5));
		}

		//みぎ（移動）
		else if ((bool)CheckHitKey(KEY_INPUT_D) == true) {
			pmdb->CamPos = VAdd(pmdb->CamPos, VScale(VCross(pmdb->CamDirHeight, pmdb->CamDir), pmdb->PlayerMoveSpeed / 1.5));
		}
		//ダッシュ
		if (pmdb->PlayerMoveSpeed == PlayerMoveSpeed_Dash) {
			pmdb->PlayerStamina -= 1.0f;
			Icon = pmdb->mIcon.Dash;
		}
	}

	//普通に動くとき
	else {
		// まえ
		if ((bool)CheckHitKey(KEY_INPUT_W) == true) {
			//足音
			//PlaySoundMem(pmdb->mSound.Walk, DX_PLAYTYPE_BACK);
			pmdb->CamPos = VAdd(pmdb->CamPos, VScale(pmdb->CamDir, pmdb->PlayerMoveSpeed));
			if (pmdb->PlayerMoveSpeed == PlayerMoveSpeed_Dash) {
				pmdb->PlayerStamina -= 1.0f;
			}


		}
		//うしろ
		else if ((bool)CheckHitKey(KEY_INPUT_S) == true) {
			pmdb->CamPos = VSub(pmdb->CamPos, VScale(pmdb->CamDir, pmdb->PlayerMoveSpeed));
			if (pmdb->PlayerMoveSpeed == PlayerMoveSpeed_Dash) {
				pmdb->PlayerStamina -= 1.0f;
			}

		}
		//ひだり（移動）
		else if ((bool)CheckHitKey(KEY_INPUT_A) == true) {
			pmdb->CamPos = VAdd(pmdb->CamPos, VScale(VCross(pmdb->CamDir, pmdb->CamDirHeight), pmdb->PlayerMoveSpeed));
			if (pmdb->PlayerMoveSpeed == PlayerMoveSpeed_Dash) {
				pmdb->PlayerStamina -= 1.0f;
			}
		}

		//みぎ（移動）
		else if ((bool)CheckHitKey(KEY_INPUT_D) == true) {
			pmdb->CamPos = VAdd(pmdb->CamPos, VScale(VCross(pmdb->CamDirHeight, pmdb->CamDir), pmdb->PlayerMoveSpeed));
			if (pmdb->PlayerMoveSpeed == PlayerMoveSpeed_Dash) {
				pmdb->PlayerStamina -= 1.0f;
			}
		}
	}

	//足音
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

	//Cで透過
	if (CheckHitKey(KEY_INPUT_C) == true && pmdb->ClearModeCoolTime <= 0) {
		pmdb->ClearModeFlag = true;
		pmdb->ClearModeCoolTime = CLEAR_COOLTIME;
		pmdb->RemainingClearTime = CLEAR_TIME_MAX;
	}

	//ESCキー
	if (CheckHitKey(KEY_INPUT_ESCAPE) == true && pmdb->PushKeyFlag.Space == false) {
		PauseFlag = true;
		pmdb->PushKeyFlag.Space = true;
	}
	else if (CheckHitKey(KEY_INPUT_ESCAPE) != true && pmdb->PushKeyFlag.Space == true) {
		pmdb->PushKeyFlag.Space = false;
	}

	//ゴール
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
///////						画面表示							//////
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

	//コメントの更新
	if (pmdb->DashPossibleFlag == false) {
		Comment[0] = "「息が苦しい。」";
		Icon = pmdb->mIcon.DashNot;
	}
	else if (pmdb->MapData[pmdb->PlayerMASS_Z][pmdb->PlayerMASS_X].NowData == MASS_GOAL) {
		Comment[0] = "上の階に行けそうな機構。\n「探せ。足の存在意義。」";
		Icon = pmdb->mIcon.Lock;
		if (pmdb->KeyNum == 1) {
			Comment[0] = "上の階に行けそうな機構。\n「何が必要なのかは、\n分かっているだろう？」";
		}
		else if (pmdb->KeyNum == 2 ) {
			Comment[0] = "上の階に行けそうな機構。\n「考える時間はある。\nお茶は飲めないが。」";
		}
		else if (pmdb->KeyNum == KEY_MAX) {
			Comment[0] = "これで動きそうだ。\n「おめでとう。\nそしてようこそ。」";
			Icon = pmdb->mIcon.Unlock;
		}
	}
	else if (pmdb->KeyNum == KEY_MAX - 1) {
		Comment[0] = "あとひとつ。\n「そして運と一握りの勇気。」";
	}
	else if (pmdb->KeyNum == KEY_MAX) {
		Comment[0] = "準備は万全。\n「望むものは手に入った？」";
	}
	//５分経過
	else if (pmdb->TimeCount / Second_flame / Second_flame >= 5) {
		Comment[0] = "お腹が空いてきた。\n「ねぇ～、まだ～？」";
	}
	else {
		Comment[0] = NULL;
	}
	if (Comment[0] == NULL) {
		Comment[0] = "焦燥と緊張に塗れた静寂が\n肌と背筋にまとわりつく";
	}

	//コメント
	DrawModiGraph(
		/*左上*/ WINDOW_MAX_X / 2 - CommentWindow_Width / 2,		/*Y*/ WINDOW_MAX_Y,
		/*右上*/ WINDOW_MAX_X / 2 + CommentWindow_Width / 2,		/*Y*/ WINDOW_MAX_Y,
		/*右下*/ WINDOW_MAX_X / 2 + CommentWindow_Width / 2,		/*Y*/ WINDOW_MAX_Y - CommentWindow_Height,
		/*左下*/ WINDOW_MAX_X / 2 - CommentWindow_Width / 2,		/*Y*/ WINDOW_MAX_Y - CommentWindow_Height,
		pmdb->mGraph.Window, TRUE);
	//文
	DrawFormatStringToHandle(WINDOW_MAX_X / 2 - CommentWindow_Width / 2 + 30, WINDOW_MAX_Y - CommentWindow_Height / 2 - 40,
		(int)GetColor(200, 200, 200), pmdb->mFont.CommentFont,"[彷徨うあなた]", KEY_MAX - pmdb->KeyNum);
	DrawFormatStringToHandle(WINDOW_MAX_X / 2 - CommentWindow_Width / 2 + 30, WINDOW_MAX_Y - CommentWindow_Height / 2 - 20,
		(int)GetColor(200, 200, 200), pmdb->mFont.CommentFont, *Comment, KEY_MAX - pmdb->KeyNum);
	

	// ピクトグラムアイコン //
	//ゴール地点
	if (pmdb->MapData[pmdb->PlayerMASS_Z][pmdb->PlayerMASS_X].NowData == MASS_GOAL) {
		Icon = pmdb->mIcon.Lock;
		if (pmdb->KeyNum == KEY_MAX) {
			Icon = pmdb->mIcon.Unlock;
		}
	}
	//ダッシュ
	else if ((pmdb->DashPossibleFlag == true) && ((bool)CheckHitKey(KEY_INPUT_LSHIFT) == true)) {
		Icon = pmdb->mIcon.Dash;
	}
	else if (pmdb->DashPossibleFlag == false) {
		Icon = pmdb->mIcon.DashNot;
	}
	//目
	else if (pmdb->ClearModeCoolTime > 0) {
		Icon = pmdb->mIcon.EyeNot;
		
	}
	else {
		Icon = pmdb->mIcon.Eye;
	}

	//ウィンドウ
	DrawModiGraph(
		/*左上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height,
		/*右上*/ WINDOW_MAX_X / 6 + ConfigWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height,
		/*右下*/ WINDOW_MAX_X / 6 + ConfigWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y,
		/*左下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y,
		pmdb->mGraph.Window, TRUE);

	//アイコン
	DrawModiGraph(
		/*左上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 20,	/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height + 30,
		/*右上*/ WINDOW_MAX_X / 6 + ConfigWindow_Width / 2 - 20,	/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height + 30,
		/*右下*/ WINDOW_MAX_X / 6 + ConfigWindow_Width / 2 - 20,	/*Y*/ WINDOW_MAX_Y - 10,
		/*左下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 20,	/*Y*/ WINDOW_MAX_Y - 10,
		Icon, TRUE);
	if ((pmdb->ClearModeCoolTime > 0) && (Icon == pmdb->mIcon.EyeNot)){
		DrawFormatStringToHandle(WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 20,	/*Y*/ WINDOW_MAX_Y - 30, (int)GetColor(200, 200, 200),
			pmdb->mFont.CommentFont, "%02d.%02d",
			pmdb->ClearModeCoolTime / Second_flame % Second_flame,
			(pmdb->ClearModeCoolTime % Second_flame) * 16);
	}
	
	//状態
	DrawStringToHandle(WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 5,	/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height + 10,
		"CONDITION", (int)GetColor(200, 200, 200), pmdb->mFont.CommentFont);
	

	//Time
	DrawFormatStringToHandle(WINDOW_MAX_X - 130, 10, (int)GetColor(200, 200, 200),
		pmdb->mFont.CommentFont, "TIME:%02d.%02d", pmdb->TimeCount / Second_flame / Second_flame, (pmdb->TimeCount / Second_flame) % Second_flame);
	
	//Esc
	DrawFormatStringToHandle(10, 10, (int)GetColor(200, 200, 200), pmdb->mFont.CommentFont, "ESC:Menu");

	//操作アシストウィンドウ(みぎのやつ)
	DrawModiGraph(
		/*左上*/ WINDOW_MAX_X * 7 / 8 - MaterWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height,
		/*右上*/ WINDOW_MAX_X * 7 / 8 + MaterWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height,
		/*右下*/ WINDOW_MAX_X * 7 / 8 + MaterWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y,
		/*左下*/ WINDOW_MAX_X * 7 / 8 - MaterWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y,
		pmdb->mGraph.Window, TRUE);
	
	//文字
	DrawStringToHandle(WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2,/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 3 / 5 - 20,
						"[酸素]", (int)GetColor(200, 200, 200), pmdb->mFont.CommentFont);
	//カギ
	for (int i = 0; i < pmdb->KeyNum; i++) {
		DrawRotaGraph(WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2 + (i * 30), WINDOW_MAX_Y - MaterWindow_Height * 1 / 5 - 5,
			0.3, 0.0, pmdb->mIcon.Key, TRUE);
	}
	/*　移動時に座標がずれる。メニューとかなら使える？*
	//取得済KEY
	GetKey = pmdb->CamPos;
	GetKey = VAdd(GetKey, VScale(pmdb->CamDir, 100.0f));
	GetKey = VSub(GetKey, VScale(VCross(pmdb->CamDir, pmdb->CamDirHeight), 60.0f));
	MV1SetScale(pmdb->mModel.Key, VGet(0.05f, 0.05f, 0.05f));
	MV1SetPosition(pmdb->mModel.Key, GetKey);
	MV1DrawModel(pmdb->mModel.Key);
	MV1SetScale(pmdb->mModel.Key, VGet(1.0f, 1.0f, 1.0f));
	*/

	//スタミナゲージ
	//if (pmdb->PlayerStamina < PlayerStaminaMAX) {

		//バーの枠
	DrawModiGraph(
		/*左上*/ WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2,		/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 4 / 8,
		/*右上*/ WINDOW_MAX_X * 7 / 8 + StaminaGuage_DenWidth / 2 + 4,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 4 / 8,
		/*右下*/ WINDOW_MAX_X * 7 / 8 + StaminaGuage_DenWidth / 2 + 4,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 4 / 8 - StaminaGuage_DenHeight,
		/*左下*/ WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2,		/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 4 / 8 - StaminaGuage_DenHeight,
		pmdb->mGraph.HPFrame, TRUE);

	//バー長さの調整
	StaminaBarWidth = StaminaGuage_DenWidth * (pmdb->PlayerStamina / PlayerStaminaMAX);

	//バー中身
		//-2ずつ縮小させて枠を表示させる
	DrawModiGraph(
		/*左上*/ WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2 + 2,						/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 4 / 8 - 2,
		/*右上*/ WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2 + StaminaBarWidth + 2,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 4 / 8 - 2,
		/*右下*/ WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2 + StaminaBarWidth + 2,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 4 / 8 + 2 - StaminaGuage_DenHeight,
		/*左下*/ WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2 + 2,						/*Y*/ WINDOW_MAX_Y - MaterWindow_Height * 4 / 8 + 2 - StaminaGuage_DenHeight,
		pmdb->mGraph.HPBar, TRUE);

	if (pmdb->MapData[pmdb->PlayerMASS_Z][pmdb->PlayerMASS_X].NowData == MASS_GOAL) {
		if (pmdb->KeyNum == KEY_MAX) {
			//ゴールの文字
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

//ポーズメニュー
void PlayerOperation::PauseMenu(MasterDataBase* pmdb) {

	pmdb->TimeFlag = false;

	//ESCキーで閉じる
	if (CheckHitKey(KEY_INPUT_ESCAPE) == true && pmdb->PushKeyFlag.Space == false) {
		pmdb->PushKeyFlag.Space = true;
		PauseFlag = false;
		pmdb->TimeFlag = true;
	}
	//決定
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
	//決定とESCを同じフラグで管理
	else if (CheckHitKey(KEY_INPUT_ESCAPE) != true && pmdb->PushKeyFlag.Space == true) {
		pmdb->PushKeyFlag.Space = false;
	}

	//下キー処理
	if (((bool)CheckHitKey(KEY_INPUT_DOWN) == true || (bool)CheckHitKey(KEY_INPUT_S) == true) && pmdb->PushKeyFlag.Down == false) {
		// 現在の選択項目を一つ↓にずらす(プラスする)
		PSelectNum = (PSelectNum + 1) % PauseMenuElementMax;
		pmdb->PushKeyFlag.Down = true;
		PlaySoundMem(pmdb->mSound.Select, DX_PLAYTYPE_BACK);
	}
	//キーを離した状態にする
	else if (((bool)CheckHitKey(KEY_INPUT_DOWN) != true && pmdb->PushKeyFlag.Down == true) && ((bool)CheckHitKey(KEY_INPUT_S) != true && pmdb->PushKeyFlag.Down == true)) {
		pmdb->PushKeyFlag.Down = false;
	}

	//上キー処理
	else if (((bool)CheckHitKey(KEY_INPUT_UP) == true || (bool)CheckHitKey(KEY_INPUT_W) == true) && pmdb->PushKeyFlag.Up == false) {
		//現在の選択項目を一つ↑にずらす(マイナスする)
		PSelectNum = (PSelectNum - 1) % PauseMenuElementMax;
		pmdb->PushKeyFlag.Up = true;
		PlaySoundMem(pmdb->mSound.Select, DX_PLAYTYPE_BACK);
	}
	//キーを離した状態にする
	else if (((bool)CheckHitKey(KEY_INPUT_UP) != true && pmdb->PushKeyFlag.Up == true) && ((bool)CheckHitKey(KEY_INPUT_W) != true && pmdb->PushKeyFlag.Up == true)) {
		pmdb->PushKeyFlag.Up = false;//ディレイ
	}

	// 保険
	if (PSelectNum < 0) {
		PSelectNum = PauseMenuElementMax - 1;//  下から上にループさせる
	}
	else if (PSelectNum >= PauseMenuElementMax) {
		PSelectNum = 0;//  下から上にループさせる
	}


	// ウインドウ
	DrawModiGraph(
		/*左上*/ WINDOW_MAX_X * 2 / 7 - 10,	WINDOW_MAX_Y * 2 / 7 - 10,
		/*右上*/ WINDOW_MAX_X * 5 / 7,		WINDOW_MAX_Y * 2 / 7 - 10,
		/*右下*/ WINDOW_MAX_X * 5 / 7,		WINDOW_MAX_Y * 5 / 7,
		/*左下*/ WINDOW_MAX_X * 2 / 7 - 10,	WINDOW_MAX_Y * 5 / 7,
		pmdb->mGraph.Window, TRUE);

	//項目内容を描画
	for (int i = 0; i < PauseMenuElementMax; i++) {
		//非選択項目
		if (i != PSelectNum) {
			DrawStringToHandle(PMenuElements[i].x, PMenuElements[i].y, PMenuElements[i].name, (int)GetColor(255,255,255), pmdb->mFont.ElementFont);
		}

		//選択中の項目
		else if (i == PSelectNum) {
			//選択アイコンの描画
			int x = pmdb->TimeCount % 2* Second_flame;
			if (x <= Second_flame) {
				DrawStringToHandle(PMenuElements[i].x + (x / 4) - 10, PMenuElements[i].y, ">", (int)GetColor(255, 100, 100), pmdb->mFont.ElementFont);
			}
			else if (x > Second_flame) {
				DrawStringToHandle(PMenuElements[i].x + (30 - x / 4) - 10, PMenuElements[i].y, ">", (int)GetColor(255, 100, 100), pmdb->mFont.ElementFont);
			}
			//選択中の項目は横にずらして表示
			DrawStringToHandle(PMenuElements[i].x + 20, PMenuElements[i].y, PMenuElements[i].name, (int)GetColor(255, 100, 100), pmdb->mFont.ElementFont);
		}
	}
}

void PlayerOperation::TutorialWindow(MasterDataBase* pmdb) {

	pmdb->TimeFlag = false;

	DrawModiGraph(
		/*左上*/ WINDOW_MAX_X / 6 - 20,	/*Y*/ WINDOW_MAX_Y / 5 - 20,
		/*右上*/ WINDOW_MAX_X * 5 / 6,	/*Y*/ WINDOW_MAX_Y / 5 - 20,
		/*右下*/ WINDOW_MAX_X * 5 / 6,	/*Y*/ WINDOW_MAX_Y / 5 + 140,
		/*左下*/ WINDOW_MAX_X / 6 - 20,	/*Y*/ WINDOW_MAX_Y / 5 + 140,
		pmdb->mGraph.Window, TRUE);

	if (pmdb->ExecutionMode == Mode_StartMenu) {
		DrawStringToHandle(WINDOW_MAX_X / 6, WINDOW_MAX_Y / 5, "[基本操作説明]　※Fキーで戻る", (int)GetColor(100, 200, 100), pmdb->mFont.CommentFont, TRUE);
	}
	else {
		DrawStringToHandle(WINDOW_MAX_X / 6, WINDOW_MAX_Y / 5, "[基本操作説明]　※Fキーでゲーム開始", (int)GetColor(100, 200, 100), pmdb->mFont.CommentFont, TRUE);
	}

	DrawStringToHandle(WINDOW_MAX_X / 6, WINDOW_MAX_Y / 5 + 20, "  W\nA S D　キーで移動。", (int)GetColor(100, 200, 100), pmdb->mFont.CommentFont, TRUE);
	DrawStringToHandle(WINDOW_MAX_X * 3 / 6, WINDOW_MAX_Y / 5 + 20, "\n←　→　キーで視点移動。", (int)GetColor(100, 200, 100), pmdb->mFont.CommentFont, TRUE);
	DrawStringToHandle(WINDOW_MAX_X / 6, WINDOW_MAX_Y / 5 + 60, "・Eキー・・・ウィンドウ非表示\n・Cキー・・・透過使用", (int)GetColor(100, 200, 100), pmdb->mFont.CommentFont, TRUE);
	DrawStringToHandle(WINDOW_MAX_X / 6, WINDOW_MAX_Y / 5 + 100, "[マウスカーソルで各ウィンドウの詳細表示]", (int)GetColor(100, 200, 100), pmdb->mFont.CommentFont, TRUE);


	//時間
	if (/*左*/ pmdb->MouseCursor_X > WINDOW_MAX_X - 130 &&
		/*右*/ pmdb->MouseCursor_X < WINDOW_MAX_X &&
		/*上*/ pmdb->MouseCursor_Y > 0 &&
		/*下*/ pmdb->MouseCursor_Y < 30) {
		DrawModiGraph(
			/*左上*/ WINDOW_MAX_X - 140,	/*Y*/ 25,
			/*右上*/ WINDOW_MAX_X - 40,		/*Y*/ 25,
			/*右下*/ WINDOW_MAX_X - 40,		/*Y*/ 50,
			/*左下*/ WINDOW_MAX_X - 140,	/*Y*/ 50,
			pmdb->mGraph.Window, TRUE);
		DrawStringToHandle(WINDOW_MAX_X - 130, 30, "[経過時間]", (int)GetColor(100, 200, 100), pmdb->mFont.TutorialFont, TRUE);
	}

	//酸素
	if (/*左*/ pmdb->MouseCursor_X > WINDOW_MAX_X * 6 / 8 &&
		/*右*/ pmdb->MouseCursor_X < WINDOW_MAX_X - 10 &&
		/*上*/ pmdb->MouseCursor_Y > WINDOW_MAX_Y - MaterWindow_Height &&
		/*下*/ pmdb->MouseCursor_Y < WINDOW_MAX_Y - MaterWindow_Height / 2) {
		DrawModiGraph(
			/*左上*/ WINDOW_MAX_X * 4 / 8 - 10,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height - 60,
			/*右上*/ WINDOW_MAX_X - 20,		/*Y*/ WINDOW_MAX_Y - MaterWindow_Height - 60,
			/*右下*/ WINDOW_MAX_X - 20,		/*Y*/ WINDOW_MAX_Y - MaterWindow_Height - 10,
			/*左下*/ WINDOW_MAX_X * 4 / 8 - 10,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height - 10,
			pmdb->mGraph.Window, TRUE);
		DrawStringToHandle(WINDOW_MAX_X * 4 / 8, WINDOW_MAX_Y - MaterWindow_Height - 50,
			"[酸素]走ると減少、走らなければ回復。\n息切れすると歩くのが精一杯になる。", (int)GetColor(100, 200, 100), pmdb->mFont.TutorialFont, TRUE);
	}

	//カギ
	for (int i = 0; i < (CTIconCount / Second_flame / 2) % 5; i++) {
		DrawRotaGraph(WINDOW_MAX_X * 7 / 8 - StaminaGuage_DenWidth / 2 + (i * 30), WINDOW_MAX_Y - MaterWindow_Height * 1 / 5 - 5,
			0.3, 0.0, pmdb->mIcon.Key, TRUE);
	}
	if (/*左*/ pmdb->MouseCursor_X > WINDOW_MAX_X * 6 / 8 &&
		/*右*/ pmdb->MouseCursor_X < WINDOW_MAX_X - 10 &&
		/*上*/ pmdb->MouseCursor_Y > WINDOW_MAX_Y - MaterWindow_Height / 2 &&
		/*下*/ pmdb->MouseCursor_Y < WINDOW_MAX_Y) {
		DrawModiGraph(
			/*左上*/ WINDOW_MAX_X * 4 / 8 - 10,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height - 10,
			/*右上*/ WINDOW_MAX_X - 60,			/*Y*/ WINDOW_MAX_Y - MaterWindow_Height - 10,
			/*右下*/ WINDOW_MAX_X - 60,			/*Y*/ WINDOW_MAX_Y - MaterWindow_Height / 2 + 5,
			/*左下*/ WINDOW_MAX_X * 4 / 8 - 10,	/*Y*/ WINDOW_MAX_Y - MaterWindow_Height / 2 + 5,
			pmdb->mGraph.Window, TRUE);
		DrawStringToHandle(WINDOW_MAX_X * 4 / 8, WINDOW_MAX_Y - MaterWindow_Height,
			"[鍵]集めた鍵。4個全て集めれば\nここからおサラダバー。", (int)GetColor(100, 200, 100), pmdb->mFont.TutorialFont, TRUE);
	}

	//コメントウィンド
	if (/*左*/ pmdb->MouseCursor_X > WINDOW_MAX_X / 2 - CommentWindow_Width / 2 - 20 &&
		/*右*/ pmdb->MouseCursor_X < WINDOW_MAX_X / 2 + CommentWindow_Width / 2 - 20 &&
		/*上*/ pmdb->MouseCursor_Y > WINDOW_MAX_Y - CommentWindow_Height &&
		/*下*/ pmdb->MouseCursor_Y < WINDOW_MAX_Y) {
		//コメント
		DrawModiGraph(
			/*左上*/ WINDOW_MAX_X / 2 - CommentWindow_Width / 2,		/*Y*/ WINDOW_MAX_Y - CommentWindow_Height - 60,
			/*右上*/ WINDOW_MAX_X / 2 + CommentWindow_Width / 2 - 50,	/*Y*/ WINDOW_MAX_Y - CommentWindow_Height - 60,
			/*右下*/ WINDOW_MAX_X / 2 + CommentWindow_Width / 2 - 50,	/*Y*/ WINDOW_MAX_Y - CommentWindow_Height - 10,
			/*左下*/ WINDOW_MAX_X / 2 - CommentWindow_Width / 2,		/*Y*/ WINDOW_MAX_Y - CommentWindow_Height - 10,
			pmdb->mGraph.Window, TRUE);
		DrawStringToHandle(WINDOW_MAX_X / 2 - CommentWindow_Width / 2 + 10, WINDOW_MAX_Y - CommentWindow_Height - 50,
			"お得な情報！一人になると\n漏らしたくなる言葉もある。", (int)GetColor(100, 200, 100), pmdb->mFont.TutorialFont, TRUE);
	}
	//コンディションアイコン
	if (/*左*/ pmdb->MouseCursor_X > WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 &&
		/*右*/ pmdb->MouseCursor_X < WINDOW_MAX_X / 6 + ConfigWindow_Width / 2 &&
		/*上*/ pmdb->MouseCursor_Y > WINDOW_MAX_Y - ConfigWindow_Height &&
		/*下*/ pmdb->MouseCursor_Y < WINDOW_MAX_Y) {
		DrawModiGraph(
			/*左上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
			/*右上*/ WINDOW_MAX_X / 2 + 70,						/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
			/*右下*/ WINDOW_MAX_X / 2 + 70,						/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height,
			/*左下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2,	/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height,
			pmdb->mGraph.Window, TRUE);
		DrawStringToHandle(WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10, WINDOW_MAX_Y - ConfigWindow_Height - 40,
			"[コンディション]客観的で簡易な状態説明。\nバリアフリーに配慮しております。", (int)GetColor(100, 200, 100), pmdb->mFont.TutorialFont, TRUE);

		//めだま
		DrawModiGraph(
			/*左上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 140,
			/*右上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 140,
			/*右下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
			/*左下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
			pmdb->mGraph.Window, TRUE);
		DrawModiGraph(
			/*左上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 110,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 140,
			/*右上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 400,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 140,
			/*右下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 400,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
			/*左下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 110,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
			pmdb->mGraph.Window, TRUE);
		DrawStringToHandle(WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 120, WINDOW_MAX_Y - ConfigWindow_Height - 130,
			"[五感能視覚]\n研ぎ澄ました警戒を視覚に反映する。\n心臓に悪い故連続で使用できない。\n「彼」がどこにいるのか見極めよう。", (int)GetColor(100, 200, 100), pmdb->mFont.TutorialFont, TRUE);
		//
		if (CTIconCount % 2 * Second_flame <= Second_flame) {
			DrawModiGraph(
				/*左上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 140,
				/*右上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 140,
				/*右下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
				/*左下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
				pmdb->mIcon.Eye, TRUE);
		}
		else {
			DrawModiGraph(
				/*左上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 140,
				/*右上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 140,
				/*右下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
				/*左下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 50,
				pmdb->mIcon.EyeNot, TRUE);
		}

		//ダッシュ
		DrawModiGraph(
			/*左上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 240,
			/*右上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 240,
			/*右下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 150,
			/*左下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 150,
			pmdb->mGraph.Window, TRUE);
		DrawModiGraph(
			/*左上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 110,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 240,
			/*右上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 400,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 240,
			/*右下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 400,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 150,
			/*左下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 110,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 150,
			pmdb->mGraph.Window, TRUE);
		DrawStringToHandle(WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 120, WINDOW_MAX_Y - ConfigWindow_Height - 230,
			"[走破]\n運動には酸素が必要不可欠。\n運動の後にはクールダウンも必要。\nこまめに休息を取ろう。", (int)GetColor(100, 200, 100), pmdb->mFont.TutorialFont, TRUE);
		//
		if (CTIconCount % 2 * Second_flame <= Second_flame) {
			DrawModiGraph(
				/*左上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 240,
				/*右上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 240,
				/*右下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 150,
				/*左下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 150,
				pmdb->mIcon.Dash, TRUE);
		}
		else {
			DrawModiGraph(
				/*左上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 240,
				/*右上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 240,
				/*右下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 150,
				/*左下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 150,
				pmdb->mIcon.DashNot, TRUE);
		}

		//かぎ
		DrawModiGraph(
			/*左上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 340,
			/*右上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 340,
			/*右下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 250,
			/*左下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 250,
			pmdb->mGraph.Window, TRUE);
		DrawModiGraph(
			/*左上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 110,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 340,
			/*右上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 400,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 340,
			/*右下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 400,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 250,
			/*左下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 110,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 250,
			pmdb->mGraph.Window, TRUE);
		DrawStringToHandle(WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 120, WINDOW_MAX_Y - ConfigWindow_Height - 330,
			"[階層移動機構の状況]\n開錠しているのはキミか？\nカギを集めた状態なら開錠される。\nここから出るための目標。", (int)GetColor(100, 200, 100), pmdb->mFont.TutorialFont, TRUE);
		//
		if (CTIconCount % 2 * Second_flame <= Second_flame) {
			DrawModiGraph(
				/*左上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 340,
				/*右上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 340,
				/*右下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 250,
				/*左下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 250,
				pmdb->mIcon.Lock, TRUE);
		}
		else {
			DrawModiGraph(
				/*左上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 340,
				/*右上*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 340,
				/*右下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 100,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 250,
				/*左下*/ WINDOW_MAX_X / 6 - ConfigWindow_Width / 2 + 10,		/*Y*/ WINDOW_MAX_Y - ConfigWindow_Height - 250,
				pmdb->mIcon.Unlock, TRUE);
		}
	}

	if ((bool)CheckHitKey(KEY_INPUT_F) == true) {
			pmdb->TutorialFlag = false;
			pmdb->TimeFlag = true;
		CTIconCount++;
	}
}