#pragma once
#include "../DxLib/DxLib.h"

#include <math.h>
#include <vector>

#define Second_flame 60
#define WINDOW_MAX_X 640			// 画面横幅
#define WINDOW_MAX_Y 480			// 画面縦幅

#define MASS_SIZE	1000.0f			// ブロックのサイズ

#define MASS_MAX_X	16				// Ｘ方向のブロック数 :横
#define MASS_MAX_Z	16				// Ｚ方向のブロック数 :縦

#define ROTATE_SPEED (DX_PI_F / 400.0f);	//カギの回転速度
#define MOVE_FRAME 60				//何に使ってたっけ…
#define CLEAR_TIME_MAX 180				//透過可能時間
#define CLEAR_COOLTIME 900			//透過のクールタイム

#define PlayerStaminaMAX 200.0f
#define PlayerMoveSpeed_Defalt 20.0f
#define PlayerMoveSpeed_Dash 40.0f
#define PlayerMoveSpeed_Tired 10.0f

#define KEY_MAX 4					//カギ最大数
#define STAGE_MAX 10				//ステージの最大数

//マスの基本状態
#define MASS_WALL	'0'				// 空地マス(壁)
#define MASS_ROAD	'1'				// 道となるマス
#define MASS_GOAL	'G'				// ゴールマス
#define MASS_KEY	'K'				// カギが置いてあるマス
#define MASS_ENEMY	'E'				// 敵マス

#define CAMERA_Y 600.0f				// カメラの高さ

//プロトタイプ宣言
class GameMain;
class MasterDataBase;
class StartMenu;
class MapControll;
class PlayerOperation;
class Enemy;

typedef unsigned long long ull;
typedef std::vector<ull> v_ull;
typedef std::vector<char> v_char;
typedef std::vector<std::vector<char>> vv_char;
typedef std::vector<VECTOR> v_vector;

//1マスの持つ情報
class MassData {
public:
	int TraficFlag = 1;
	char OriginalData;
	char NowData;
};

//プレイヤーが行っている移動動作
enum MoveMode {
	DontMove,
	GotoFront,
	GotoBack,
	RightSpin,
	LeftSpin,
};

//現在実行中のゲームモード
enum GameMainExecutionMode {
	Mode_GameExit,	//ゲーム終了
	Mode_StartMenu,	//スタートメニュー
	Mode_StageSetting,	//ステージの用意（一回目）
	Mode_Gaming,	//ゲーム中
	Mode_Pause,		//ポーズ画面
	Mode_Clear,		//クリア
	Mode_GameOver,	//ゲームオーバー
	Mode_NextStage,	//ステージ切り替え
	Mode_Tutorial	//チュートリアル画面
};

class GameMain {
public:
	void FirstLoad(MasterDataBase *pmdb);		//初回ロード
	void GameMainVoid(MasterDataBase *pmdb);		//メイン実行部分
	void FadeIn(MasterDataBase* pmdb);
	void FadeOut(MasterDataBase* pmdb);
	void PlayMovieToGraphLoop(int x, int y, float rate, float rota, int MovieGraphHandle, int flag);
	void BGMUpDate(MasterDataBase *pmdb,int NextBGM);

	int GameOverStayTime = 3.5 * Second_flame;
};

//変数やハンドルを各クラスに継承させるクラス
class MasterDataBase{
public:
	//マップのデータ
	MassData MapData[MASS_MAX_Z][MASS_MAX_X];

	//変数等
	int ExecutionMode = Mode_StartMenu;		//現在実行中の処理
	int MouseCursor_X = 0,				//カーソルの現在位置(いずれはマウスで制御)
		MouseCursor_Y = 0;				//カーソルの現在位置(いずれはマウスで制御)
	int NowStageNum = 1;				//現在のステージ番号
	bool TimeFlag = true;				//時間停止/稼働(true:稼働 false:停止)
	int TimeCount = 0;					//時間(1fで1追加)
	int Bright = 0;						//明るさ
	int NowPlayBGM = 0;					//再生中のBGM
	int KeyNum = 0;						//取得カギ個数
	int EnemyNum;						//存在する敵の数

	int ShadowLevel = 255;
	// bool FadeInFlag = true;
	// bool FadeOutFlag = false;
	// int PlayerNowMove = DontMove;			//プレイヤーが現在行っている動作

	float PlayerHitRad = 150.0f;			//当たり判定大きさ
	int PlayerMASS_X = 2,					//プレイヤーの現在立っているマス
		PlayerMASS_Z = 1;					//プレイヤーの現在立っているマス
	float PlayerPos_X = PlayerMASS_X * MASS_SIZE,		//プレイヤーの現在立っている座標
		  PlayerPos_Z = PlayerMASS_Z * MASS_SIZE;		//プレイヤーの現在立っている座標
	float PlayerSpinAngle = 0.0f;						//プレイヤーの向き（角度の数値）
	float PlayerMoveSpeed = PlayerMoveSpeed_Defalt;		//プレイヤーの移動速度
	bool ClearModeFlag = false;					//透過状態/非透過状態
	int RemainingClearTime = 0;					//残り透過時間
	int ClearModeCoolTime = 0;					//透過使用後のクールタイム(MAXなら使用可能)
	bool TutorialFlag = true;					//操作説明の表示

	float PlayerStamina = PlayerStaminaMAX;		//スタミナ数値
	bool DashPossibleFlag = true;				//ダッシュ可能フラグ(スタミナ切れ)

	VECTOR CamPos;					// カメラの座標
	VECTOR CamTarg;					// カメラの注視点
	VECTOR CamDir;					// カメラの向いている方向
	VECTOR CamDirHeight = VGet(0.0f, 1.0f, 0.0f);	//↑に垂直なベクトル(外積計算用)
	
	v_ull MapRoad;
	std::vector<Enemy*> Enemy;
	std::vector<VECTOR> EnemyPos;

	//キーを押している状態か
	struct KeyFlag {
		bool Down = false;
		bool Up = false;
		bool Right = false;
		bool Left = false;
		bool Space = false;
	}PushKeyFlag;

	//その方向に動けるか(壁があるのか)
	struct MovePossibleFlag{
		bool x_pls = true;
		bool z_min = true;
		bool z_pls = true;
		bool x_min = true;
	}MovePossibleFlag;

	//画像格納体
	struct Graph {
		int StartMenuBack;
		int GameBack;
		int OverBack;
		int Title;
		int Window;
		int SelectArrow;
		int Loading;
		int HPFrame;
		int HPBar;
		int ClearBack;
		int Stage;
	}mGraph;

	//アイコンの画像
	struct Icon {
		int Lock;
		int Unlock;
		int Dash;
		int DashNot;
		int Eye;
		int EyeNot;
		int Key;
	}mIcon;
	
	//サウンド格納体
	struct Sound {
		int StartMenuBGM;
		int GamingBGM;
		int GameOverBGM;
		int Select;
		int Enter;
		int Walk;
		int GetKey;
		int TiredCoughing;
		int Fear01;
		int Fear02;
		int Fear03;
	}mSound;

	//モデル格納体
	struct Model {
		int Wall;
		int Goal;
		int Ling;
		int Key;
	}mModel;

	//フォント格納体
	struct Font{
		int TitleFont;		//タイトルに使うフォント(仮)
		int TextFont;		//テキストのフォント
		int ElementFont;	//メニューなどのフォント
		int CommentFont;	//コメント用
		int TutorialFont;	//説明時の文字	
	}mFont;

	//動画を画像にして回す
	void PlayMovieToGraphLoop(int x, int y, float rate, float rota, int MovieGraphHandle, int flag) {
		if (GetMovieStateToGraph(MovieGraphHandle) == 0) {
			SeekMovieToGraph(MovieGraphHandle, 0);
			PlayMovieToGraph(MovieGraphHandle);
		}
		DrawRotaGraph(x, y, rate, rota, MovieGraphHandle, flag);
	}
};
