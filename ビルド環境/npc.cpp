//=============================================================================
//
// エネミーモデル処理 [npc.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "enemy.h"
#include "player.h"
#include "npc.h"
#include "bullet.h"
#include "shadow.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_NPC			"data/MODEL/P_NPC/P_NPC.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(5.0f)								// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)						// 回転量

#define NPC_SHADOW_SIZE		(0.4f)								// 影の大きさ
#define NPC_OFFSET_Y		(0.0f)								// エネミーの足元をあわせる


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static NPC			g_Npc[MAX_NPC];

int g_Npc_load = 0;



static INTERPOLATION_DATA g_MoveTbl0[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-290.0f,	NPC_OFFSET_Y, 290.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60},
	{ XMFLOAT3(-290.0f,	NPC_OFFSET_Y, 60.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
	{ XMFLOAT3(-290.0f, NPC_OFFSET_Y, 60.0f),  XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 240},
	{ XMFLOAT3(-290.0f,	NPC_OFFSET_Y, 290.0f), XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
};

static INTERPOLATION_DATA g_MoveTbl1[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(5.0f, NPC_OFFSET_Y, 290.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 120},
	{ XMFLOAT3(5.0f, NPC_OFFSET_Y, 60.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
	{ XMFLOAT3(5.0f, NPC_OFFSET_Y, 60.0f),  XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 240},
	{ XMFLOAT3(5.0f, NPC_OFFSET_Y, 290.0f), XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
};

static INTERPOLATION_DATA g_MoveTbl2[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(300.0f, NPC_OFFSET_Y, 290.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
	{ XMFLOAT3(300.0f, NPC_OFFSET_Y, 60.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
	{ XMFLOAT3(300.0f, NPC_OFFSET_Y, 60.0f),  XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 240},
	{ XMFLOAT3(300.0f, NPC_OFFSET_Y, 290.0f), XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
};

static INTERPOLATION_DATA g_MoveTbl3[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-400.0f, NPC_OFFSET_Y, -290.0f), XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60},
	{ XMFLOAT3(-400.0f, NPC_OFFSET_Y, -15.0f),  XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
	{ XMFLOAT3(-400.0f, NPC_OFFSET_Y, -15.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 240},
	{ XMFLOAT3(-400.0f, NPC_OFFSET_Y, -290.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
};

static INTERPOLATION_DATA g_MoveTbl4[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-90.0f, NPC_OFFSET_Y, -290.0f), XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 120},
	{ XMFLOAT3(-90.0f, NPC_OFFSET_Y, -15.0f),  XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
	{ XMFLOAT3(-90.0f, NPC_OFFSET_Y, -15.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 240},
	{ XMFLOAT3(-90.0f, NPC_OFFSET_Y, -290.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
};


static INTERPOLATION_DATA g_MoveTbl5[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(195.0f, NPC_OFFSET_Y, -290.0f), XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
	{ XMFLOAT3(195.0f, NPC_OFFSET_Y, -15.0f),  XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
	{ XMFLOAT3(195.0f, NPC_OFFSET_Y, -15.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 240},
	{ XMFLOAT3(195.0f, NPC_OFFSET_Y, -290.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
};

static INTERPOLATION_DATA g_MoveTbl6[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-400.0f, NPC_OFFSET_Y,-20.0f), XMFLOAT3(0.0f,-2.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 600},
	{ XMFLOAT3(400.0f, NPC_OFFSET_Y, -20.0f), XMFLOAT3(0.0f,2.5f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 600},
};

static INTERPOLATION_DATA g_MoveTbl7[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(400.0f, NPC_OFFSET_Y, -20.0f), XMFLOAT3(0.0f,2.5f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 600},
	{ XMFLOAT3(-400.0f, NPC_OFFSET_Y,-20.0f), XMFLOAT3(0.0f,-2.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 600},

};



static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_MoveTbl0,
	g_MoveTbl1,
	g_MoveTbl2,
	g_MoveTbl3,
	g_MoveTbl4,
	g_MoveTbl5,
	g_MoveTbl6,
	g_MoveTbl7,

};



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitNpc(void)
{
	for (int i = 0; i < MAX_NPC; i++)
	{
		LoadModel(MODEL_NPC, &g_Npc[i].model);
		g_Npc[i].load = TRUE;

		g_Npc[i].pos = XMFLOAT3(0.0f + i * 30.0f, NPC_OFFSET_Y, 0.0f);
		g_Npc[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Npc[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Npc[i].spd  = 0.0f;			// 移動スピードクリア
		g_Npc[i].size = NPC_SIZE;	// 当たり判定の大きさ

		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Npc[i].model, &g_Npc[i].diffuse[0]);

		XMFLOAT3 pos = g_Npc[i].pos;
		pos.y -= (NPC_OFFSET_Y - 0.1f);
		g_Npc[i].shadowIdx = CreateShadow(pos, NPC_SHADOW_SIZE, NPC_SHADOW_SIZE);
		
		g_Npc[i].time = 0.0f;			// 線形補間用のタイマーをクリア
		g_Npc[i].tblNo = 0;				// 再生する行動データテーブルNoをセット
		g_Npc[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

		g_Npc[i].use = TRUE;			// TRUE:生きてる


	}

	// 0番の線形補間で動かす
	g_Npc[0].time = 0.0f;													// 線形補間用のタイマーをクリア
	g_Npc[0].tblNo = 0;														// 再生するアニメデータの先頭アドレスをセット
	g_Npc[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 1番の線形補間で動かす
	g_Npc[1].time = 0.0f;													// 線形補間用のタイマーをクリア
	g_Npc[1].tblNo = 1;														// 再生するアニメデータの先頭アドレスをセット
	g_Npc[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 2番の線形補間で動かす
	g_Npc[2].time = 0.0f;													// 線形補間用のタイマーをクリア
	g_Npc[2].tblNo = 2;														// 再生するアニメデータの先頭アドレスをセット
	g_Npc[2].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 3番の線形補間で動かす
	g_Npc[3].time = 0.0f;													// 線形補間用のタイマーをクリア
	g_Npc[3].tblNo = 3;														// 再生するアニメデータの先頭アドレスをセット
	g_Npc[3].tblMax = sizeof(g_MoveTbl3) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 4番の線形補間で動かす
	g_Npc[4].time = 0.0f;													// 線形補間用のタイマーをクリア
	g_Npc[4].tblNo = 4;														// 再生するアニメデータの先頭アドレスをセット
	g_Npc[4].tblMax = sizeof(g_MoveTbl4) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 5番の線形補間で動かす
	g_Npc[5].time = 0.0f;													// 線形補間用のタイマーをクリア
	g_Npc[5].tblNo = 5;														// 再生するアニメデータの先頭アドレスをセット
	g_Npc[5].tblMax = sizeof(g_MoveTbl5) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 6番の線形補間で動かす
	g_Npc[6].time = 0.0f;													// 線形補間用のタイマーをクリア
	g_Npc[6].tblNo = 6;														// 再生するアニメデータの先頭アドレスをセット
	g_Npc[6].tblMax = sizeof(g_MoveTbl6) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 7番の線形補間で動かす
	g_Npc[7].time = 0.0f;													// 線形補間用のタイマーをクリア
	g_Npc[7].tblNo = 7;														// 再生するアニメデータの先頭アドレスをセット
	g_Npc[7].tblMax = sizeof(g_MoveTbl7) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitNpc(void)
{

	for (int i = 0; i < MAX_NPC; i++)
	{
		if (g_Npc[i].load)
		{
			UnloadModel(&g_Npc[i].model);
			g_Npc[i].load = FALSE;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateNpc(void)
{
	// エネミーを動かく場合は、影も合わせて動かす事を忘れないようにね！
	for (int i = 0; i < MAX_NPC; i++)
	{
		if (g_Npc[i].use == TRUE)		// このエネミーが使われている？
		{								// Yes

			// 移動処理
			if (g_Npc[i].tblMax > 0)	// 線形補間を実行する？
			{	// 線形補間の処理
				int nowNo = (int)g_Npc[i].time;			// 整数分であるテーブル番号を取り出している
				int maxNo = g_Npc[i].tblMax;				// 登録テーブル数を数えている
				int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Npc[i].tblNo];	// 行動テーブルのアドレスを取得

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

				float nowTime = g_Npc[i].time - nowNo;	// 時間部分である少数を取り出している

				Pos *= nowTime;								// 現在の移動量を計算している
				Rot *= nowTime;								// 現在の回転量を計算している
				Scl *= nowTime;								// 現在の拡大率を計算している

				// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
				XMStoreFloat3(&g_Npc[i].pos, nowPos + Pos);

				// 計算して求めた回転量を現在の移動テーブルに足している
				XMStoreFloat3(&g_Npc[i].rot, nowRot + Rot);

				// 計算して求めた拡大率を現在の移動テーブルに足している
				XMStoreFloat3(&g_Npc[i].scl, nowScl + Scl);

				// frameを使て時間経過処理をする
				g_Npc[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
				if ((int)g_Npc[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
				{
					g_Npc[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
				}

			}

			// 影もプレイヤーの位置に合わせる
			XMFLOAT3 pos = g_Npc[i].pos;
			pos.y -= (NPC_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Npc[i].shadowIdx, pos);
		}
	}




#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_P))
	{
		// モデルの色を変更できるよ！半透明にもできるよ。
		for (int j = 0; j < g_Npc[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Npc[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
		}
	}

	if (GetKeyboardTrigger(DIK_L))
	{
		// モデルの色を元に戻している
		for (int j = 0; j < g_Npc[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Npc[0].model, j, g_Npc[0].diffuse[j]);
		}
	}
#endif


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawNpc(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_NPC; i++)
	{
		if (g_Npc[i].use == FALSE) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Npc[i].scl.x, g_Npc[i].scl.y, g_Npc[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Npc[i].rot.x, g_Npc[i].rot.y + XM_PI, g_Npc[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Npc[i].pos.x, g_Npc[i].pos.y, g_Npc[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Npc[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_Npc[i].model);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
NPC *GetNpc()
{
	return &g_Npc[0];
}

