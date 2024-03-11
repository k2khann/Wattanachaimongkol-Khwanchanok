//=============================================================================
//
// エネミーモデル処理 [boss.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "boss.h"
#include "player.h"
#include "bullet.h"
#include "shadow.h"
#include "fade.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_BOSS			"data/MODEL/YetiGold/YetiGold.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(5.0f)						// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量

#define BOSS_SHADOW_SIZE	(0.4f)						// 影の大きさ
#define BOSS_OFFSET_Y		(3.0f)						// エネミーの足元をあわせる

#define BOSS_SPAWN_RANGE_X	(800)
#define BOSS_SPAWN_RANGE_Z	(800)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static BOSS			g_Boss[MAX_BOSS];				// エネミー

int g_Boss_load = 0;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBoss(void)
{
	for (int i = 0; i < MAX_BOSS; i++)
	{
		LoadModel(MODEL_BOSS, &g_Boss[i].model);
		g_Boss[i].load = TRUE;

		g_Boss[i].pos = XMFLOAT3(-60.0f, BOSS_OFFSET_Y, 0.0f);
		g_Boss[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Boss[i].scl = XMFLOAT3(10.0f, 10.0f, 10.0f);

		g_Boss[i].spd  = 0.0f;			// 移動スピードクリア
		g_Boss[i].size = BOSS_SIZE;	// 当たり判定の大きさ

		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Boss[i].model, &g_Boss[i].diffuse[0]);

		XMFLOAT3 pos = g_Boss[i].pos;
		pos.y -= (BOSS_OFFSET_Y - 0.1f);
		g_Boss[i].shadowIdx = CreateShadow(pos, BOSS_SHADOW_SIZE, BOSS_SHADOW_SIZE);
		
		g_Boss[i].time = 0.0f;			// 線形補間用のタイマーをクリア
		g_Boss[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Boss[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

		g_Boss[i].use = TRUE;			// TRUE:生きてる

		g_Boss[i].hp = BOSS_LIFE;

	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBoss(void)
{

	for (int i = 0; i < MAX_BOSS; i++)
	{
		if (g_Boss[i].load)
		{
			UnloadModel(&g_Boss[i].model);
			g_Boss[i].load = FALSE;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBoss(void)
{

	// エネミーを動かく場合は、影も合わせて動かす事を忘れないようにね！
	for (int i = 0; i < MAX_BOSS; i++)
	{
		if (g_Boss[i].use == TRUE)		// このエネミーが使われている？
		{								// Yes


			// BossのHPが０になったら、ゲームが終わる
			if (g_Boss[i].hp == 0)
			{
				SetFade(FADE_OUT, MODE_RESULT);
			}



			// 影もプレイヤーの位置に合わせる
			XMFLOAT3 pos = g_Boss[i].pos;
			pos.y -= (BOSS_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Boss[i].shadowIdx, pos);
		}
	}






#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_P))
	{
		// モデルの色を変更できるよ！半透明にもできるよ。
		for (int j = 0; j < g_Boss[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Boss[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
		}
	}

	if (GetKeyboardTrigger(DIK_L))
	{
		// モデルの色を元に戻している
		for (int j = 0; j < g_Boss[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Boss[0].model, j, g_Boss[0].diffuse[j]);
		}
	}
#endif


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBoss(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_BOSS; i++)
	{
		if (g_Boss[i].use == FALSE) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Boss[i].scl.x, g_Boss[i].scl.y, g_Boss[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Boss[i].rot.x, g_Boss[i].rot.y + XM_PI, g_Boss[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Boss[i].pos.x, g_Boss[i].pos.y, g_Boss[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Boss[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_Boss[i].model);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
BOSS *GetBoss()
{
	return &g_Boss[0];
}

