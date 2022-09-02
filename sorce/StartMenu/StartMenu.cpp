#include "StartMenu.h"

StartMenu::StartMenu() {
};

StartMenu::~StartMenu() {

};

void StartMenu::StartMenuFirstTime(MasterDataBase *pmdb){


}
//メイン実行関数
void StartMenu::StartMenuVoid(MasterDataBase *pmdb){

    if (StartMenuFirstTimeFlag == true) {
        StartMenuFirstTime(pmdb);
        StartMenuFirstTimeFlag = false;
    }

    //直前の選択項目を記憶
    BeforeSelectNum = SelectNum;

    //キー入力チェック
    StartMenuKeyCheck(pmdb);
    //マウス確認
    MouseCheck(pmdb);
    
    if (BeforeSelectNum != SelectNum) {
        PlaySoundMem(pmdb->mSound.Select, DX_PLAYTYPE_BACK);
    }

    //画面表示
    StartMenuDraw(pmdb);

    // チェックウィンドウ処理
    if (CheckWindowFlag == true) {
        CheckWindow(pmdb);
    }

}

//キー入力のチェック
void StartMenu::StartMenuKeyCheck(MasterDataBase* pmdb) {

    //現在のモードによって行う処理を変える
    if (pmdb->TimeFlag == true) {
        switch (StartMenuMode) {

            ////////////////////////
            ///　TOP画面の処理　////
            ////////////////////////

        case StartMenuMode_Top:
            if (CheckWindowFlag == false) {

                //ENTERキー
                if (((bool)CheckHitKey(KEY_INPUT_SPACE) == true || (bool)CheckHitKey(KEY_INPUT_RETURN) == true) && pmdb->PushKeyFlag.Space == false) {
                    pmdb->PushKeyFlag.Space = true;
                    PlaySoundMem(pmdb->mSound.Enter, DX_PLAYTYPE_BACK);

                    //　選択中の項目にスタートメニューのモードを変える。
                    if (CheckWindowFlag == false) {
                        switch (SelectNum) {
                        case StartMenuElement_GameStart:
                            StartMenuMode = StartMenuMode_StageSelectCheck;
                            CheckWindowFlag = true;
                            break;
                        case StartMenuElement_Howto:
                            StartMenuMode = StartMenuMode_HowTo;
                            break;
                        case StartMenuElement_Exit:
                            StartMenuMode = StartMenuMode_ExitCheck;
                            CheckWindowFlag = true;
                            break;
                        default: //ERROR
                             //エラーメッセージを出して終了
                            MessageBox(NULL, "存在しない項目を参照しています", "ERROR : StartMenu", MB_OK);
                        }
                    }
                }

                //キーを離した状態にする
                else if (((bool)CheckHitKey(KEY_INPUT_SPACE) != true && pmdb->PushKeyFlag.Space == true) && ((bool)CheckHitKey(KEY_INPUT_RETURN) != true && pmdb->PushKeyFlag.Space == true)) {
                    pmdb->PushKeyFlag.Space = false;
                }

                //下キー処理
                if (((bool)CheckHitKey(KEY_INPUT_DOWN) == true || (bool)CheckHitKey(KEY_INPUT_S) == true) && pmdb->PushKeyFlag.Down == false) {
                    // 現在の選択項目を一つ↓にずらす(プラスする)
                    SelectNum = (SelectNum + 1) % StartMenuElementsNum;
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
                    SelectNum = (SelectNum - 1) % StartMenuElementsNum;
                    pmdb->PushKeyFlag.Up = true;
                    PlaySoundMem(pmdb->mSound.Select, DX_PLAYTYPE_BACK);
                }
                //キーを離した状態にする
                else if (((bool)CheckHitKey(KEY_INPUT_UP) != true && pmdb->PushKeyFlag.Up == true) && ((bool)CheckHitKey(KEY_INPUT_W) != true && pmdb->PushKeyFlag.Up == true)) {
                    pmdb->PushKeyFlag.Up = false;//ディレイ
                }

                // 保険
                if (SelectNum < 0) {
                    SelectNum = StartMenuElementsNum - 1;//  下から上にループさせる
                }
                else if (SelectNum >= StartMenuElementsNum) {
                    SelectNum = 0;//  下から上にループさせる
                }

            }

            break;

            ////////////////////////////////
            ///　ステージ選択画面処理　  ///
            ////////////////////////////////
        case StartMenuMode_HowTo:
            if (((bool)CheckHitKey(KEY_INPUT_SPACE) == true || (bool)CheckHitKey(KEY_INPUT_RETURN) == true) && pmdb->PushKeyFlag.Space == false) {
                pmdb->PushKeyFlag.Space = true;
                PlaySoundMem(pmdb->mSound.Enter, DX_PLAYTYPE_BACK);
                StartMenuMode = StartMenuMode_Top;
            }
            //キーを離した状態にする
            else if (((bool)CheckHitKey(KEY_INPUT_SPACE) != true && pmdb->PushKeyFlag.Space == true) && ((bool)CheckHitKey(KEY_INPUT_RETURN) != true && pmdb->PushKeyFlag.Space == true)) {
                pmdb->PushKeyFlag.Space = false;
            }
            break;

        default: //ERROR
             //エラーメッセージを出して終了
            //MessageBox(NULL, "存在しないモードを参照しています", "ERROR : StartMenu", MB_OK);
            ;
        }
    }
}

void StartMenu::MouseCheck(MasterDataBase* pmdb) {
    if (StartMenuMode == StartMenuMode_Top) {
        //Start
        if (/*左*/ pmdb->MouseCursor_X > StartMenuElement_Den_X - 20 &&
            /*右*/ pmdb->MouseCursor_X < StartMenuElement_Den_X + StartMenu_Width &&
            /*上*/ pmdb->MouseCursor_Y > StartMenuElement_Den_Y + StartMenu_Padding * 0 &&
            /*下*/ pmdb->MouseCursor_Y < StartMenuElement_Den_Y + StartMenu_Padding * 1) {
            SelectNum = StartMenuElement_GameStart;
            if (GetMouseInput() & MOUSE_INPUT_LEFT) {
                PlaySoundMem(pmdb->mSound.Enter, DX_PLAYTYPE_BACK);
                StartMenuMode = StartMenuMode_StageSelectCheck;
                CheckWindowFlag = true;
            }
        }
        //Howto
        if (/*左*/ pmdb->MouseCursor_X > StartMenuElement_Den_X - 20 &&
            /*右*/ pmdb->MouseCursor_X < StartMenuElement_Den_X + StartMenu_Width &&
            /*上*/ pmdb->MouseCursor_Y > StartMenuElement_Den_Y + StartMenu_Padding * 1 &&
            /*下*/ pmdb->MouseCursor_Y < StartMenuElement_Den_Y + StartMenu_Padding * 2) {
            SelectNum = StartMenuElement_Howto;
            if (GetMouseInput() & MOUSE_INPUT_LEFT) {
                PlaySoundMem(pmdb->mSound.Enter, DX_PLAYTYPE_BACK);
                StartMenuMode = StartMenuMode_HowTo;
            }
        }
        //Exit
        if (/*左*/ pmdb->MouseCursor_X > StartMenuElement_Den_X - 20 &&
            /*右*/ pmdb->MouseCursor_X < StartMenuElement_Den_X + StartMenu_Width &&
            /*上*/ pmdb->MouseCursor_Y > StartMenuElement_Den_Y + StartMenu_Padding * 2 &&
            /*下*/ pmdb->MouseCursor_Y < StartMenuElement_Den_Y + StartMenu_Padding * 3) {
            SelectNum = StartMenuElement_Exit;
            if (GetMouseInput() & MOUSE_INPUT_LEFT) {
                PlaySoundMem(pmdb->mSound.Enter, DX_PLAYTYPE_BACK);
                StartMenuMode = StartMenuMode_ExitCheck;
                CheckWindowFlag = true;
            }
        }
    }
    /*
    if (StartMenuMode == StartMenuMode_HowTo) {
        if (GetMouseInput() & MOUSE_INPUT_LEFT) {
            PlaySoundMem(pmdb->mSound.Enter, DX_PLAYTYPE_BACK);
            StartMenuMode = StartMenuMode_Top;
        }
    }
    */
    else if (CheckWindowFlag == true) {
        //はい
        if (/*左*/ pmdb->MouseCursor_X > WINDOW_MAX_X * 2 / 7 &&
            /*右*/ pmdb->MouseCursor_X < WINDOW_MAX_X * 3 / 7 &&
            /*上*/ pmdb->MouseCursor_Y > WINDOW_MAX_Y * 4 / 7 &&
            /*下*/ pmdb->MouseCursor_Y < WINDOW_MAX_Y * 4 / 7 + 30) {
            CheckWindowSelect = true;
            if (GetMouseInput() & MOUSE_INPUT_LEFT) {
                PlaySoundMem(pmdb->mSound.Enter, DX_PLAYTYPE_BACK);
                if (StartMenuMode == StartMenuMode_StageSelectCheck) {
                    StartMenuMode = StartMenuMode_Top;
                    CheckWindowFlag = false;
                    pmdb->ExecutionMode = Mode_StageSetting;
                }
                else if (StartMenuMode == StartMenuMode_ExitCheck) {
                    pmdb->ExecutionMode = Mode_GameExit;
                }
            }
        }

        //いいえ
        if (/*左*/ pmdb->MouseCursor_X > WINDOW_MAX_X * 4 / 7 &&
            /*右*/ pmdb->MouseCursor_X < WINDOW_MAX_X * 5 / 7 &&
            /*上*/ pmdb->MouseCursor_Y > WINDOW_MAX_Y * 4 / 7 &&
            /*下*/ pmdb->MouseCursor_Y < WINDOW_MAX_Y * 4 / 7 + 30) {
            CheckWindowSelect = false;
            if (GetMouseInput() & MOUSE_INPUT_LEFT) {
                PlaySoundMem(pmdb->mSound.Enter, DX_PLAYTYPE_BACK);
                if (StartMenuMode == StartMenuMode_StageSelectCheck) {
                    StartMenuMode = StartMenuMode_Top;
                    CheckWindowFlag = false;
                }
                else if (StartMenuMode == StartMenuMode_ExitCheck) {
                    StartMenuMode = StartMenuMode_Top;
                    CheckWindowFlag = false;
                }
            }
        }
    }
}


void StartMenu::StartMenuDraw(MasterDataBase* pmdb) {

    switch (StartMenuMode) {
        ///////////////////////////
        /// スタート画面：トップ///
        ///////////////////////////
    case StartMenuMode_Top:
    case StartMenuMode_StageSelectCheck:
    case StartMenuMode_ExitCheck:

        //背景
        DrawRotaGraph(WINDOW_MAX_X / 2, WINDOW_MAX_Y / 2, 1.2, 0, pmdb->mGraph.StartMenuBack, TRUE);
        //ウインドウ
        DrawModiGraph(
            /*左上*/ StartMenuElement_Den_X - 40, StartMenuElement_Den_Y - StartMenu_Padding,
            /*右上*/ StartMenuElement_Den_X + StartMenu_Width, StartMenuElement_Den_Y - StartMenu_Padding,
            /*右下*/ StartMenuElement_Den_X + StartMenu_Width, StartMenuElement_Den_Y + StartMenu_Hight,
            /*左下*/ StartMenuElement_Den_X - 40, StartMenuElement_Den_Y + StartMenu_Hight,
            pmdb->mGraph.Window, TRUE);

        // タイトル
        // DrawStringToHandle(70, 100, "Danseon Escape", GetColor(165, 42, 42), pmdb->mFont.TitleFont);
        DrawRotaGraph(WINDOW_MAX_X / 2, WINDOW_MAX_Y * 2 / 7, 1.2, 0, pmdb->mGraph.Title, TRUE);

        DrawStringToHandle(WINDOW_MAX_X / 2, WINDOW_MAX_Y * 2 / 3, "-Pless Enter or Space-", (int)GetColor(255, 255, 255), pmdb->mFont.ElementFont);
        // DrawRotaGraph(MAX_X / 2, MAX_Y / 4 + 20, 0.5, 0, pmdb->graph.window1, TRUE);
        // DrawRotaGraph(MAX_X / 2, MAX_Y / 4 + 20, 0.4, 0, title, TRUE);

        // メニュー項目を描画
        for (int i = 0; i < StartMenuElementsNum; i++) {
            // 非選択項目
            if (i != SelectNum) {
                DrawStringToHandle(StartMenuStruct[i].x, StartMenuStruct[i].y, StartMenuStruct[i].title, StartMenuStruct[i].color, pmdb->mFont.ElementFont);
            }

            // 選択中の項目
            else if (i == SelectNum) {
                // 選択アイコンの描画
                int x = pmdb->TimeCount % (2 * Second_flame);
                if (x <= 60) {
                    DrawStringToHandle(StartMenuStruct[i].x + (x / 4) - 10, StartMenuStruct[i].y, ">", (int)GetColor(255, 100, 100), pmdb->mFont.ElementFont);
                }
                else if (x > Second_flame) {
                    DrawStringToHandle(StartMenuStruct[i].x + (30 - x / 4) - 10, StartMenuStruct[i].y, ">", (int)GetColor(255, 100, 100), pmdb->mFont.ElementFont);
                }
                //選択中の項目は横にずらして表示
                DrawStringToHandle(StartMenuStruct[i].x + 20, StartMenuStruct[i].y, StartMenuStruct[i].title, (int)GetColor(255, 100, 100), pmdb->mFont.ElementFont);
            }
        }
        //if (pmdb->TimeCount % 60 <= 30) {
           // DrawStringToHandle(WINDOW_MAX_X / 4 , WINDOW_MAX_Y / 2, "-PRESS SPACE or ENTER-", (int)GetColor(255, 255, 255), pmdb->mFont.ElementFont);
        //}
        break;

        //////////////////////////////////////
        /// スタート画面:ステージセレクト　///
        //////////////////////////////////////

    case StartMenuMode_StageSelect:

        break;

    case StartMenuMode_HowTo:

        break;

    default: //ERROR
        //エラーメッセージを出して終了
        //MessageBox(NULL, "存在しないモードを描画しています", "ERROR : StartMenuDraw", MB_OK);
        ;
    }
}

////////////////////////////////
///　チェックウィンドウ処理　///
////////////////////////////////
void StartMenu::CheckWindow(MasterDataBase* pmdb) {

    //ENTERキー
    if (((bool)CheckHitKey(KEY_INPUT_SPACE) == true || (bool)CheckHitKey(KEY_INPUT_RETURN) == true) && pmdb->PushKeyFlag.Space == false) {
        pmdb->PushKeyFlag.Space = true;
        PlaySoundMem(pmdb->mSound.Enter, DX_PLAYTYPE_BACK);
        //はい時の処理
        if (CheckWindowSelect == true) {
            if (StartMenuMode == StartMenuMode_StageSelectCheck) {
                StartMenuMode = StartMenuMode_Top;
                CheckWindowFlag = false;
                pmdb->ExecutionMode = Mode_StageSetting;
            }
            else if (StartMenuMode == StartMenuMode_ExitCheck) {
                pmdb->ExecutionMode = Mode_GameExit;
            }
        }
        //いいえ時の処理
        else if (CheckWindowSelect == false) {
            if (StartMenuMode == StartMenuMode_StageSelectCheck) {
                StartMenuMode = StartMenuMode_Top;
                CheckWindowFlag = false;
            }
            else if (StartMenuMode == StartMenuMode_ExitCheck) {
                StartMenuMode = StartMenuMode_Top;
                CheckWindowFlag = false;
            }
        }
    }
    //キーを離した状態にする
    else if ((bool)CheckHitKey(KEY_INPUT_SPACE) != true && (bool)CheckHitKey(KEY_INPUT_RETURN) != true && pmdb->PushKeyFlag.Space == true) {
        pmdb->PushKeyFlag.Space = false;
    }

    // 右入力時
    if (((bool)CheckHitKey(KEY_INPUT_RIGHT) == true || (bool)CheckHitKey(KEY_INPUT_D) == true) && pmdb->PushKeyFlag.Right == false) {
        //いいえの方向へ
        CheckWindowSelect = false;
        pmdb->PushKeyFlag.Right = true;
        PlaySoundMem(pmdb->mSound.Select, DX_PLAYTYPE_BACK);
    }
    //キーを離した状態にする
    else if (((bool)CheckHitKey(KEY_INPUT_RIGHT) != true && pmdb->PushKeyFlag.Right == true) && ((bool)CheckHitKey(KEY_INPUT_D) != true && pmdb->PushKeyFlag.Right == true)) {
        pmdb->PushKeyFlag.Right = false;//ディレイ
    }

    // 左入力時
    else if (((bool)CheckHitKey(KEY_INPUT_LEFT) == true || (bool)CheckHitKey(KEY_INPUT_A) == true) && pmdb->PushKeyFlag.Left == false) {
        //はいの方向へ
        CheckWindowSelect = true;
        pmdb->PushKeyFlag.Left = true;
        PlaySoundMem(pmdb->mSound.Select, DX_PLAYTYPE_BACK);
    }
    //キーを離した状態にする
    else if (((bool)CheckHitKey(KEY_INPUT_LEFT) != true && pmdb->PushKeyFlag.Left == true) && ((bool)CheckHitKey(KEY_INPUT_A) != true && pmdb->PushKeyFlag.Left == true)) {
        pmdb->PushKeyFlag.Left = false;//ディレイ
    }

    // ウインドウ
    DrawModiGraph(
        /*左上*/ WINDOW_MAX_X / 4, WINDOW_MAX_Y * 2 / 7,
        /*右上*/ WINDOW_MAX_X * 3 / 4, WINDOW_MAX_Y * 2 / 7,
        /*右下*/ WINDOW_MAX_X * 3 / 4, WINDOW_MAX_Y * 5 / 7,
        /*左下*/ WINDOW_MAX_X / 4, WINDOW_MAX_Y * 5 / 7,
        pmdb->mGraph.Window, TRUE);
    
    //Yes or No
    if (CheckWindowSelect == true) {
        DrawStringToHandle(WINDOW_MAX_X * 2 / 7 + 30, WINDOW_MAX_Y * 4 / 7, ">はい", (int)GetColor(255, 100, 100), pmdb->mFont.ElementFont);
        DrawStringToHandle(WINDOW_MAX_X * 4 / 7, WINDOW_MAX_Y * 4 / 7, "いいえ", (int)GetColor(255, 255, 255), pmdb->mFont.ElementFont);
    }
    else if (CheckWindowSelect == false) {
        DrawStringToHandle(WINDOW_MAX_X * 2 / 7 + 30, WINDOW_MAX_Y * 4 / 7, "はい", (int)GetColor(255, 255, 255), pmdb->mFont.ElementFont);
        DrawStringToHandle(WINDOW_MAX_X * 4 / 7, WINDOW_MAX_Y * 4 / 7, ">いいえ", (int)GetColor(255, 100, 100), pmdb->mFont.ElementFont);
    }

    // 内部テキスト
    switch (StartMenuMode) {
    case StartMenuMode_ExitCheck:
        DrawStringToHandle(WINDOW_MAX_X * 2 / 7 + 40, WINDOW_MAX_Y * 3 / 7 - 20, "ゲームを終了します\nよろしいですか？", (int)GetColor(255, 255, 255), pmdb->mFont.ElementFont);
        break;
    case StartMenuMode_StageSelectCheck:
        DrawStringToHandle(WINDOW_MAX_X * 2 / 7 + 40, WINDOW_MAX_Y * 3 / 7 - 20, "ゲームを開始します\nよろしいですか？", (int)GetColor(255, 255, 255), pmdb->mFont.ElementFont);
        break;
    default: //ERROR
        //エラーメッセージを出す
        //MessageBox(NULL, "存在しないモードを参照しています", "ERROR : StartMenu", MB_OK);
        ;
    }
}