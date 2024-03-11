//=============================================================================
//
// objモデル処理 [0bj.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "model.h"

#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "house.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX		(1)									// テクスチャの数

#define	MODEL_HOUSE		"data/MODEL/HOUSE/Snow.obj"			// 読み込むモデル名

#define MAX_HOUSE		(6)

#define HOUSE_OFFSET_Y		(1.0f)						// エネミーの足元をあわせる
#define HOUSE_OFFSET_Z		(300.0f)						// エネミーの足元をあわせる


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点情報
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static HOUSE						g_House;				// ポリゴンデータ
static int							g_TexNo;				// テクスチャ番号



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitHouse(void)
{

	LoadModel(MODEL_HOUSE, &g_House.model);


	// 位置・回転・スケールの初期設定
	g_House.pos = XMFLOAT3(0.0f, HOUSE_OFFSET_Y, HOUSE_OFFSET_Z);
	g_House.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_House.scl = XMFLOAT3(10.0f, 10.0f, 10.0f);

	g_TexNo = 0;
	g_House.load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHouse(void)
{

	// モデルの解放処理
	if (g_House.load)
	{
		UnloadModel(&g_House.model);
		g_House.load = FALSE;

	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateHouse(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHouse(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;


	for (int i = 0; i < MAX_HOUSE / 2; i++)
	{
		//柱の描画
		//ワールドマトリックスの初期化y
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(1.0, 1.0, 1.0);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_House.rot.x, g_House.rot.y, g_House.rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		 mtxTranslate = XMMatrixTranslation(g_House.pos.x - 300.0f + (i * 300.0f), g_House.pos.y, g_House.pos.z);
		 mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_House.mtxWorld, mtxWorld);

		//モデル描画
		DrawModel(&g_House.model);
	}



	for (int i = 0; i < MAX_HOUSE; i++)
	{
		//柱の描画
		//ワールドマトリックスの初期化y
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(1.0, 1.0, 1.0);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_House.rot.x, g_House.rot.y + 160.0f, g_House.rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_House.pos.x - 400.0f + (i * 300.0f), g_House.pos.y, g_House.pos.z - 620.0f);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_House.mtxWorld, mtxWorld);

		//モデル描画
		DrawModel(&g_House.model);
	}



}