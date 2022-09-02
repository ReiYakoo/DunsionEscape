#pragma once
#include "../MAIN.h"

#define StartMenuElementsNum 3			//スタートメニュー項目数
#define StartMenuElementsStrings 50		//スタートメニューで表示する文字数

#define StartMenuElement_Den_X	50		//スタートメニュー項目の初期X座標
#define StartMenuElement_Den_Y	WINDOW_MAX_Y * 2 / 3		//スタートメニュー項目の初期Y座標

#define StartMenu_Padding 30			//スタートメニューの行間高さ
#define StartMenu_Hight	StartMenu_Padding * (StartMenuElementsNum + 1 )		//スタートメニュー全体の高さ 
#define StartMenu_Width	200				//スタートメニュー全体の幅

//スタートメニューの実行モード
enum StartMenuMode {
	StartMenuMode_Top,				//トップ画面
	StartMenuMode_HowTo,			//操作説明画面
	StartMenuMode_StageSelect,		//ステージ選択
	StartMenuMode_StageSelectCheck,	//プレイしますか？
	StartMenuMode_ExitCheck,		//終了しますか？
};

//スタートメニューの選択項目
enum StartMenuElements {
	StartMenuElement_GameStart,
	StartMenuElement_Howto,
	StartMenuElement_Exit,
};

class StartMenu {
public:
	StartMenu();
	~StartMenu();
	void StartMenuVoid(MasterDataBase * pmdb);		//スタートメニューの実行とりまとめ
	void StartMenuFirstTime(MasterDataBase *pmdb);
	void StartMenuKeyCheck(MasterDataBase* pmdb);	//キー入力チェック
	void StartMenuDraw(MasterDataBase* pmdb);		//画面上に文字や画像を表示
	void MouseCheck(MasterDataBase* pmdb);
	void CheckWindow(MasterDataBase *pmdb);

	bool StartMenuFirstTimeFlag = true;			//初回ロードフラグ
	int StartMenuMode = StartMenuMode_Top;		//スタートメニューが行っている処理
	int SelectNum = StartMenuElement_GameStart;	//選択中の項目
	int BeforeSelectNum = SelectNum;				//直前の選択項目
	bool CheckWindowFlag = false;				//チェックウィンドウ表示フラグ(trueで表示)
	bool CheckWindowSelect = true;				//チェック画面選択中項目(true:はい / false:いいえ)

	//スタートメニュー構造体パーツ
	struct StartMenuStruct {
		int x, y;								//位置
		char title[StartMenuElementsStrings];	//表示する文字
		int color;								//文字色
	};
	
	StartMenuStruct StartMenuStruct[StartMenuElementsNum] = {
		StartMenuElement_Den_X, StartMenuElement_Den_Y,								"GAME START",	(int)GetColor(255,255,255),
		StartMenuElement_Den_X, StartMenuElement_Den_Y + StartMenu_Padding,			"HOW TO PLAY",	(int)GetColor(255,255,255),
		StartMenuElement_Den_X, StartMenuElement_Den_Y + StartMenu_Padding * 2,		"EXIT",			(int)GetColor(255,255,255),
	};

};