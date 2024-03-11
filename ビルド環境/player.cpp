//=============================================================================
//
// モデル処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "light.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "fade.h"
#include "player.h"
#include "shadow.h"
#include "bullet.h"
#include "debugproc.h"
#include "meshfield.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define MODEL_PLAYER_BODY	"data/MODEL/P_Witch01/WITCH_BODY.obj"				// 読み込むモデル名
#define MODEL_PLAYER_ARM_R	"data/MODEL/P_Witch01/WITCH_R_ARM.obj"			// 読み込むモデル名
#define MODEL_PLAYER_ARM_L	"data/MODEL/P_Witch01/WITCH_L_ARM.obj"			// 読み込むモデル名
#define MODEL_PLAYER_LEG_R	"data/MODEL/P_Witch01/WITCH_R_LEG.obj"			// 読み込むモデル名
#define MODEL_PLAYER_LEG_L	"data/MODEL/P_Witch01/WITCH_L_LEG.obj"			// 読み込むモデル名

#define	VALUE_MOVE			(2.0f)							// 移動量 ↑
#define VALUE_MOVE_BOTH		(1.5f)							// 移動量 ←→
#define	VALUE_MOVE_DOWN		(0.5f)							// 移動量 ↓
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)				// 回転量

#define PLAYER_SHADOW_SIZE	(0.4f)							// 影の大きさ
#define PLAYER_OFFSET_Y		(7.0f)							// プレイヤーの足元をあわせる

#define PLAYER_PARTS_MAX	(5)								// プレイヤーのパーツの数

enum TABLE_LIST
{
	// 待機状態
	WAIT_BODY,
	WAIT_R_ARM,
	WAIT_L_ARM,
	WAIT_R_LEG,
	WAIT_L_LEG,

	// 動く状態
	fMOVE_BODY,
	fMOVE_R_ARM,
	fMOVE_L_ARM,
	fMOVE_R_LEG,
	fMOVE_L_LEG,

	// 動く状態 →右
	rMOVE_BODY,
	rMOVE_R_ARM,
	rMOVE_L_ARM,
	rMOVE_R_LEG,
	rMOVE_L_LEG,

	// 動く状態 →左
	lMOVE_BODY,
	lMOVE_R_ARM,
	lMOVE_L_ARM,
	lMOVE_R_LEG,
	lMOVE_L_LEG,

	// 攻撃状態
	ATTACK_BODY,
	ATTACK_R_ARM,
	ATTACK_L_ARM,
	ATTACK_R_LEG,
	ATTACK_L_LEG,

};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER		g_Player;						// プレイヤー

static PLAYER		g_Parts[PLAYER_PARTS_MAX];		// プレイヤーのパーツ用

static float		roty = 0.0f;

static LIGHT		g_Light;





// プレイヤーの階層アニメーションデータ

static INTERPOLATION_DATA wait_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA wait_tbl_r_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(5.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(5.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA wait_tbl_l_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-6.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-6.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA wait_tbl_r_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(3.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(3.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA wait_tbl_l_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-5.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-5.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};







static INTERPOLATION_DATA move_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI / 10, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 10, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA move_tbl_r_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(5.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(5.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA move_tbl_l_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-6.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-6.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA move_tbl_r_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(3.0f, -5.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(3.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA move_tbl_l_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-5.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-5.0f, -5.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};







static INTERPOLATION_DATA rmove_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f,-5.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -5.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA rmove_tbl_r_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, -5.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.5f, -5.0f), XMFLOAT3(0.0f, -5.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA rmove_tbl_l_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-2.0f, 0.5f, -2.0f), XMFLOAT3(0.0f, -5.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-2.0f, 0.0f, -2.0f), XMFLOAT3(0.0f, -5.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA rmove_tbl_r_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(3.0f, -3.0f, -4.0f), XMFLOAT3(1.0f, -5.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(3.0f, -3.0f, -4.0f), XMFLOAT3(0.0f, -5.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA rmove_tbl_l_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-5.0f, -3.0f, 0.0f), XMFLOAT3(0.0f, -5.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-5.0f, -3.0f, 0.0f), XMFLOAT3(1.0f, -5.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};







static INTERPOLATION_DATA lmove_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f,4.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f,4.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA lmove_tbl_r_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 5.0f), XMFLOAT3(0.0f, 5.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.5f, 5.0f), XMFLOAT3(0.0f, 5.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA lmove_tbl_l_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(4.0f,-0.5f, -5.0f), XMFLOAT3(0.0f, 5.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(4.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 5.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA lmove_tbl_r_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(3.0f, -5.0f, 0.0f), XMFLOAT3(1.0f, 4.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(3.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 4.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA lmove_tbl_l_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(3.0f, -6.0f, 0.0f), XMFLOAT3(0.0f, 4.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(3.0f, -6.0f, 0.0f), XMFLOAT3(1.0f, 4.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};





static INTERPOLATION_DATA attack_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA attack_tbl_r_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(5.0f, 3.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(5.0f, 3.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA attack_tbl_l_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-6.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-6.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA attack_tbl_r_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(3.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(3.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA attack_tbl_l_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-5.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-5.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};


static INTERPOLATION_DATA* g_MoveTblAdr[] =
{

	wait_tbl_body,
	wait_tbl_r_arm,
	wait_tbl_l_arm,
	wait_tbl_r_leg,
	wait_tbl_l_leg,

	move_tbl_body,
	move_tbl_r_arm,
	move_tbl_l_arm,
	move_tbl_r_leg,
	move_tbl_l_leg,

	rmove_tbl_body,
	rmove_tbl_r_arm,
	rmove_tbl_l_arm,
	rmove_tbl_r_leg,
	rmove_tbl_l_leg,

	lmove_tbl_body,
	lmove_tbl_r_arm,
	lmove_tbl_l_arm,
	lmove_tbl_r_leg,
	lmove_tbl_l_leg,

	attack_tbl_body,
	attack_tbl_r_arm,
	attack_tbl_l_arm,
	attack_tbl_r_leg,
	attack_tbl_l_leg,


};






//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	g_Player.load = TRUE;
	LoadModel(MODEL_PLAYER_BODY, &g_Player.model);

	g_Player.pos = XMFLOAT3(-50.0f, PLAYER_OFFSET_Y+50.0f, -250.0f);
	g_Player.rot = XMFLOAT3(0.0f, XM_PI , 0.0f);
	g_Player.scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

	g_Player.spd = 0.0f;			// 移動スピードクリア
	
	g_Player.hp = PLAYER_LIFE;		// プレイヤーのライフ HP

	g_Player.use = TRUE;			// TRUE:生きてる
	g_Player.size = PLAYER_SIZE;	// 当たり判定の大きさ

	g_Player.moving_f = FALSE;		// 動くの階層アニメーション用
	g_Player.attack = FALSE;		// 攻撃の階層アニメーション用



	// ここでプレイヤー用の影を作成している
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ↑
	//        このメンバー変数が生成した影のIndex番号

	// キーを押した時のプレイヤーの向き
	roty = 0.0f;

	g_Player.parent = NULL;			// 本体（親）なのでNULLを入れる


	// 階層アニメーションの初期化
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		g_Parts[i].use = FALSE;

		// 位置・回転・スケールの初期設定
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// 親子関係
		g_Parts[i].parent = &g_Player;		// ← ここに親のアドレスを入れる
		g_Parts[1].parent = &g_Player;		// ← ここに親のアドレスを入れる
		g_Parts[2].parent = &g_Player;		// ← ここに親のアドレスを入れる
		g_Parts[3].parent = &g_Player;		// ← ここに親のアドレスを入れる
		g_Parts[4].parent = &g_Player;		// ← ここに親のアドレスを入れる

	//	g_Parts[腕].parent= &g_Player;		// 腕だったら親は本体（プレイヤー）
	//	g_Parts[手].parent= &g_Paerts[腕];	// 指が腕の子供だった場合の例

		// 階層アニメーション用のメンバー変数の初期化
		g_Parts[i].time = 0.0f;			// 線形補間用のタイマーをクリア
		g_Parts[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Parts[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

		// パーツの読み込みはまだしていない
		g_Parts[i].load = FALSE;
	}

	g_Parts[0].use = TRUE;
	g_Parts[0].parent = &g_Player;												// 親をセット
	g_Parts[0].tblNo = WAIT_BODY;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[0].tblMax = sizeof(move_tbl_body) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット
	g_Parts[0].load = TRUE;
	LoadModel(MODEL_PLAYER_BODY, &g_Parts[0].model);

	g_Parts[1].use = TRUE;
	g_Parts[1].parent = &g_Player;												// 親をセット
	g_Parts[1].tblNo = WAIT_R_ARM;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[1].tblMax = sizeof(move_tbl_r_arm) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[1].load = TRUE;
	LoadModel(MODEL_PLAYER_ARM_R, &g_Parts[1].model);

	g_Parts[2].use = TRUE;
	g_Parts[2].parent = &g_Player;												// 親をセット
	g_Parts[2].tblNo = WAIT_L_ARM;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[2].tblMax = sizeof(move_tbl_l_arm) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[2].load = TRUE;
	LoadModel(MODEL_PLAYER_ARM_L, &g_Parts[2].model);

	g_Parts[3].use = TRUE;
	g_Parts[3].parent = &g_Player;												// 親をセット
	g_Parts[3].tblNo = WAIT_R_LEG;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[3].tblMax = sizeof(move_tbl_r_leg) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[3].load = TRUE;
	LoadModel(MODEL_PLAYER_LEG_R, &g_Parts[3].model);

	g_Parts[4].use = TRUE;
	g_Parts[4].parent = &g_Player;												// 親をセット
	g_Parts[4].tblNo = WAIT_L_LEG;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[4].tblMax = sizeof(move_tbl_l_leg) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[4].load = TRUE;
	LoadModel(MODEL_PLAYER_LEG_L, &g_Parts[4].model);


	// クォータニオンの初期化
	XMStoreFloat4(&g_Player.Quaternion, XMQuaternionIdentity());



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	// モデルの解放処理
	if (g_Player.load == TRUE)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = FALSE;
	}

	// パーツの解放処理
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		if (g_Parts[i].load == TRUE)
		{
			// パーツの解放処理
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = FALSE;
		}
	}



}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();
	
	// カメラワーク（プレイヤーの視点にする）
	XMVECTOR vCameraAt = XMLoadFloat3(&cam->at);
	XMVECTOR vPlayerPos = XMLoadFloat3(&g_Player.pos);
	XMVECTOR vec = vPlayerPos - vCameraAt;
	float anglePlayer = atan2f(vec.m128_f32[0], vec.m128_f32[2]);

	// プレイヤーのスピード
	g_Player.spd *= 0.7f;

	// プレイヤーの動くかどうか
	g_Player.moving_f = FALSE;

	// 移動処理
	if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, STICK_L_LEFT))
	{
		g_Player.spd = VALUE_MOVE_BOTH;
		//g_Player.pos.x -= g_Player.spd;
		roty = anglePlayer - XM_PIDIV2;

		g_Player.state = STATE_MOVE_LEFT;
		g_Player.moving_f = TRUE;
	}
	if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, STICK_L_RIGHT))
	{
		g_Player.spd = VALUE_MOVE_BOTH;
		roty = anglePlayer + XM_PIDIV2;

		g_Player.state = STATE_MOVE_RIGHT;
		g_Player.moving_f = TRUE;
	}
	if (GetKeyboardPress(DIK_UP) || IsButtonPressed(0, STICK_L_UP))
	{
		g_Player.spd = VALUE_MOVE;
		//g_Player.pos.z += g_Player.spd;
		roty = anglePlayer;

		g_Player.state = STATE_MOVE_FORWARD;
		g_Player.moving_f = TRUE;

	}
	if (GetKeyboardPress(DIK_DOWN) || IsButtonPressed(0, STICK_L_DOWN))
	{
		g_Player.spd = VALUE_MOVE_DOWN;
		roty = anglePlayer - XM_PI;

		g_Player.state = STATE_MOVE_FORWARD;
		g_Player.moving_f = TRUE;
	}


	if ((g_Player.moving_f == FALSE) && (g_Player.attack == FALSE))
	{
		g_Player.state = STATE_WAIT;
	}


	//////////////////////////////////////////////////////////////////////
	// 階層アニメーション用
	//////////////////////////////////////////////////////////////////////

	switch (g_Player.state)
	{
	case STATE_WAIT:
		SetWaitTbl();
		break;

	case STATE_MOVE_FORWARD:
		SetMoveTbl();
		break;

	case STATE_MOVE_RIGHT:
		SetMove_RTbl();
		break;

	case STATE_MOVE_LEFT:
		SetMove_LTbl();
		break;


	case STATE_ATTACK:
		SetAttackTbl();
		break;
	}

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_Player.pos.z = g_Player.pos.x = 0.0f;
		g_Player.spd = 0.0f;
		roty = 0.0f;
	}


#endif


	{	// 押した方向にプレイヤーを移動させる
		// 押した方向にプレイヤーを向かせている所
		g_Player.rot.y = anglePlayer;

		g_Player.pos.x -= sinf(roty) * g_Player.spd;
		g_Player.pos.z -= cosf(roty) * g_Player.spd;
	}


	// レイキャストして足元の高さを求める
	XMFLOAT3 HitPosition;		// 交点
	XMFLOAT3 Normal;			// ぶつかったポリゴンの法線ベクトル（向き）
	BOOL ans = RayHitField(g_Player.pos, &HitPosition, &Normal);
	if (ans)
	{
		g_Player.pos.y = HitPosition.y + PLAYER_OFFSET_Y;
	}
	else
	{
		g_Player.pos.y = PLAYER_OFFSET_Y;
		Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}


	// 弾発射処理
	if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, BUTTON_R))
	{
		SetBullet(g_Player.pos, g_Player.rot);

		g_Player.moving_f = FALSE;
		g_Player.state = STATE_ATTACK;
		g_Player.attack = TRUE;
		
	}

	// プレイヤーのHPが０になったら、Bad Endingに遷移する
	if (g_Player.hp == 0)
	{
		SetFade(FADE_OUT, MODE_BADEND);
	}

	// 影もプレイヤーの位置に合わせる
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_Player.shadowIdx, pos);


	// 階層アニメーション
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// 使われているなら処理する
		if ((g_Parts[i].use == TRUE) && (g_Parts[i].tblMax > 0))
		{	// 線形補間の処理
			int nowNo = (int)g_Parts[i].time;			// 整数分であるテーブル番号を取り出している
			int maxNo = g_Parts[i].tblMax;				// 登録テーブル数を数えている
			int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
			INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Parts[i].tblNo];	// 行動テーブルのアドレスを取得

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
			XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
			XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
			XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
			XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

			float nowTime = g_Parts[i].time - nowNo;	// 時間部分である少数を取り出している

			Pos *= nowTime;								// 現在の移動量を計算している
			Rot *= nowTime;								// 現在の回転量を計算している
			Scl *= nowTime;								// 現在の拡大率を計算している

			// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
			XMStoreFloat3(&g_Parts[i].pos, nowPos + Pos);

			// 計算して求めた回転量を現在の移動テーブルに足している
			XMStoreFloat3(&g_Parts[i].rot, nowRot + Rot);

			// 計算して求めた拡大率を現在の移動テーブルに足している
			XMStoreFloat3(&g_Parts[i].scl, nowScl + Scl);

			// frameを使て時間経過処理をする
			g_Parts[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
			if ((int)g_Parts[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
			{
				g_Parts[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
			}

		}

	}


	// ポイントライトのテスト
	{
		LIGHT *light = GetLightData(1);
		XMFLOAT3 pos = g_Player.pos;
		pos.y += 20.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = TRUE;
		SetLightData(1, light);
	}



	//////////////////////////////////////////////////////////////////////
	// 姿勢制御
	//////////////////////////////////////////////////////////////////////

	XMVECTOR vx, nvx, up;
	XMVECTOR quat;
	float len, angle;


	g_Player.UpVector = Normal;
	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	vx = XMVector3Cross(up, XMLoadFloat3(&g_Player.UpVector));

	nvx = XMVector3Length(vx);
	XMStoreFloat(&len, nvx);
	nvx = XMVector3Normalize(vx);
	//nvx = vx / len;
	angle = asinf(len);

	//quat = XMQuaternionIdentity();

	//quat = XMQuaternionRotationAxis(nvx, angle);
	quat = XMQuaternionRotationNormal(nvx, angle);


	quat = XMQuaternionSlerp(XMLoadFloat4(&g_Player.Quaternion), quat, 0.05f);
	XMStoreFloat4(&g_Player.Quaternion, quat);




#ifdef _DEBUG
	// デバッグ表示
	// PrintDebugProc("Player X:%f Y:%f Z:% N:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z, Normal.y);
	// PrintDebugProc("Player RotY:%f\n", g_Player.rot.y);
	// PrintDebugProc("Player anglePlayer:%f\n", anglePlayer);
#endif

}


// 描画処理
//=============================================================================
void DrawPlayer(void)
{

	if (g_Player.use == FALSE) return;

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// クォータニオンを反映
	quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Player.Quaternion));
	mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);


	// 縁取りの設定
	SetFuchi(1);


	// モデル描画
	// DrawModel(&g_Player.model);



	// 階層アニメーション
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[i].parent != NULL)	// 子供だったら親と結合する
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
																			// ↑
																			// g_Player.mtxWorldを指している
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// 使われているなら処理する
		if (g_Parts[i].use == FALSE) continue;

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);


		// モデル描画
		DrawModel(&g_Parts[i].model);

	}

	SetFuchi(0);

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// 階層アニメーション用
//=============================================================================
void SetWaitTbl(void)
{
	g_Parts[0].tblNo = WAIT_BODY;
	g_Parts[0].tblMax = sizeof(wait_tbl_body) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[1].tblNo = WAIT_R_ARM;
	g_Parts[1].tblMax = sizeof(wait_tbl_r_arm) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[2].tblNo = WAIT_L_ARM;
	g_Parts[2].tblMax = sizeof(wait_tbl_l_arm) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[3].tblNo = WAIT_R_LEG;
	g_Parts[3].tblMax = sizeof(wait_tbl_r_leg) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[4].tblNo = WAIT_L_LEG;
	g_Parts[4].tblMax = sizeof(wait_tbl_l_leg) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット


}


void SetMoveTbl(void)
{
	g_Parts[0].tblNo = fMOVE_BODY;
	g_Parts[0].tblMax = sizeof(move_tbl_body) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[1].tblNo = fMOVE_R_ARM;
	g_Parts[1].tblMax = sizeof(move_tbl_r_arm) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[2].tblNo = fMOVE_L_ARM;
	g_Parts[2].tblMax = sizeof(move_tbl_l_arm) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[3].tblNo = fMOVE_R_LEG;
	g_Parts[3].tblMax = sizeof(move_tbl_r_leg) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[4].tblNo = fMOVE_L_LEG;
	g_Parts[4].tblMax = sizeof(move_tbl_l_leg) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット


}


void SetMove_RTbl(void)
{
	g_Parts[0].tblNo = rMOVE_BODY;
	g_Parts[0].tblMax = sizeof(rmove_tbl_body) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[1].tblNo = rMOVE_R_ARM;
	g_Parts[1].tblMax = sizeof(rmove_tbl_r_arm) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[2].tblNo = rMOVE_L_ARM;
	g_Parts[2].tblMax = sizeof(rmove_tbl_l_arm) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[3].tblNo = rMOVE_R_LEG;
	g_Parts[3].tblMax = sizeof(rmove_tbl_r_leg) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[4].tblNo = rMOVE_L_LEG;
	g_Parts[4].tblMax = sizeof(rmove_tbl_l_leg) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット


}



void SetMove_LTbl(void)
{
	g_Parts[0].tblNo = lMOVE_BODY;
	g_Parts[0].tblMax = sizeof(lmove_tbl_body) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[1].tblNo = lMOVE_R_ARM;
	g_Parts[1].tblMax = sizeof(lmove_tbl_r_arm) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[2].tblNo = lMOVE_L_ARM;
	g_Parts[2].tblMax = sizeof(lmove_tbl_l_arm) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[3].tblNo = lMOVE_R_LEG;
	g_Parts[3].tblMax = sizeof(lmove_tbl_r_leg) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[4].tblNo = lMOVE_L_LEG;
	g_Parts[4].tblMax = sizeof(lmove_tbl_l_leg) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット


}

void SetAttackTbl(void)
{
	g_Parts[0].tblNo = ATTACK_BODY;
	g_Parts[0].tblMax = sizeof(lmove_tbl_body) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[1].tblNo = ATTACK_R_ARM;
	g_Parts[1].tblMax = sizeof(lmove_tbl_r_arm) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[2].tblNo = ATTACK_L_ARM;
	g_Parts[2].tblMax = sizeof(lmove_tbl_l_arm) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[3].tblNo = ATTACK_R_LEG;
	g_Parts[3].tblMax = sizeof(lmove_tbl_r_leg) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Parts[4].tblNo = ATTACK_L_LEG;
	g_Parts[4].tblMax = sizeof(lmove_tbl_l_leg) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット


}