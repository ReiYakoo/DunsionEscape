#pragma once
#include "../MAIN.h"

#define StaminaGuage_DenHeight 10	//スタミナゲージの高さ
#define StaminaGuage_DenWidth 100	//スタミナゲージの長さ

#define CommentWindow_Width 300		//真ん中のウィンドウの長さ
#define CommentWindow_Height 100	//真ん中のウィンドウの高さ

#define ConfigWindow_Width 100		//左側のウィンドウの長さ
#define ConfigWindow_Height 100		//左側のウィンドウの高さ

#define MaterWindow_Width 150		//右側のウィンドウの長さ
#define MaterWindow_Height 80		//右側のウィンドウ高さ

#define PauseMenuElementMax 3		//ポーズメニューの項目数

//ポーズメニューの項目
struct Element {
	int x, y;
	char name[20];
};

//ポーズメニュー選択項目
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

		int StaminaBarWidth = StaminaGuage_DenWidth;	//スタミナのバーの現在横幅
		char *Comment[100];							//メッセージ
		int Icon;									//アイコン
		int FootSoundCount = 0;						//足音用カウント
		int FearSoundCount = 0;						//声切り替え　
		int CTIconCount = 0;						//操作画面のアイコン切り替えるやつ
		bool PauseFlag = false;					//ポーズメニューのフラグ
		int PSelectNum = 0;						//選択中項目

		Element PMenuElements[PauseMenuElementMax] = {
			WINDOW_MAX_X * 2 / 7 + 50,	WINDOW_MAX_Y * 2 / 7 + 50,	"ゲームに戻る",
			WINDOW_MAX_X * 2 / 7 + 50,	WINDOW_MAX_Y * 2 / 7 + 80,	"リトライ",
			WINDOW_MAX_X * 2 / 7 + 50,	WINDOW_MAX_Y * 2 / 7 + 110,	"タイトルに戻る",
		};

		VECTOR GetKey;
};