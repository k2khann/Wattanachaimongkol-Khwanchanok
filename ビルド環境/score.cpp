//=============================================================================
//
// スコア処理 [score.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "score.h"
#include "npc.h"
#include "boss.h"
#include "debugproc.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(16)	// キャラサイズ
#define TEXTURE_HEIGHT				(32)	// 

#define NUM_POS_X					(930)	// 数字のPOS
#define NUM_POS_Y					(505)

#define SCORE_TEXTURE_WIDTH			(529 / 4)	// Scoreのロゴサイズ
#define SCORE_TEXTURE_HEIGHT		(179 / 4)

#define SCORE_POS_X					(720)	// ScoreのロゴのPOS
#define SCORE_POS_Y					(480)

//////////////////////////////////////////////
#define NPC_MAX						(8)

#define NUM_NPC_POS_X				(95)	// 数字の NPC DOWN
#define NUM_NPC_POS_Y				(510)

#define NPC_LOGO_WIDTH				(233 / 4)
#define NPC_LOGO_HEIGHT				(230 / 4)

#define NPC_LOGO_POS_X				(20)
#define NPC_LOGO_POS_Y				(470)

//////////////////////////////////////////////

#define NUM_BOSS_POS_X				(130)
#define NUM_BOSS_POS_Y				(440)

#define BOSS_LOGO_WIDTH				(261 / 4)
#define BOSS_LOGO_HEIGHT			(337 / 4)
		
#define BOSS_LOGO_POS_X				(20)
#define BOSS_LOGO_POS_Y				(370)

//////////////////////////////////////////////


#define TEXTURE_MAX					(5)		// テクスチャの数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/number01.png",
	"data/TEXTURE/number02.png",
	"data/TEXTURE/Score02.png",

	"data/TEXTURE/NPC_Count01.png",
	"data/TEXTURE/BOSS_Count01.png",
};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

static int						g_Score;					// スコア
static int						g_Down;						// NPCが倒した人数
static int						g_BossHP;					// BOSSのHP

static BOOL						g_Load = FALSE;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitScore(void)
{

	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// プレイヤーの初期化
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = { 500.0f, 20.0f, 0.0f };
	g_TexNo = 0;

	g_Score = 0;			// スコアの初期化
	g_Down = NPC_MAX;		// NPCの倒した数
	g_BossHP = BOSS_LIFE;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{
	BOSS* boss = GetBoss();

#ifdef _DEBUG	// デバッグ情報を表示する
	// PrintDebugProc("BOSS HP%d\n", boss->hp);
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif
	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
	BOSS* boss = GetBoss();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[NUMBER01]);

	// 桁数分処理する
	int number = g_Score;
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		// 今回表示する桁の数字
		float x = (float)(number % 10);

		// スコアの位置やテクスチャー座標を反映
		float px = NUM_POS_X - g_w*i;	// スコアの表示位置X
		float py = NUM_POS_Y;			// スコアの表示位置Y
		float pw = g_w;					// スコアの表示幅
		float ph = g_h;					// スコアの表示高さ

		float tw = 1.0f / 10;			// テクスチャの幅
		float th = 1.0f / 1;			// テクスチャの高さ
		float tx = x * tw;				// テクスチャの左上X座標
		float ty = 0.0f;				// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		// 次の桁へ
		number /= 10;
	}



	// NPCの人数
	// 桁数分処理する
	int numberDown = g_Down;
	for (int i = 0; i < DOWN_DIGIT; i++)
	{
		// 今回表示する桁の数字
		float x = (float)(numberDown % 10);

		// スコアの位置やテクスチャー座標を反映
		float px = NUM_NPC_POS_X - g_w * i;	// スコアの表示位置X
		float py = NUM_NPC_POS_Y;			// スコアの表示位置Y
		float pw = g_w;						// スコアの表示幅
		float ph = g_h;						// スコアの表示高さ

		float tw = 1.0f / 10;				// テクスチャの幅
		float th = 1.0f / 1;				// テクスチャの高さ
		float tx = x * tw;					// テクスチャの左上X座標
		float ty = 0.0f;					// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		// 次の桁へ
		numberDown /= 10;
	}


	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[NUMBER02]);

		// BOSSのHP
		// 桁数分処理する

		int numberBoss = boss->hp;
		// numberBoss = 159;
		for (int i = 0; i < BOSS_DIGIT; i++)
		{
			// 今回表示する桁の数字
			float x = (float)(numberBoss % 10);

			// スコアの位置やテクスチャー座標を反映
			float px = NUM_BOSS_POS_X - g_w * i;		// スコアの表示位置X
			float py = NUM_BOSS_POS_Y;					// スコアの表示位置Y
			float pw = g_w;								// スコアの表示幅
			float ph = g_h;								// スコアの表示高さ

			float tw = 1.0f / 10;						// テクスチャの幅
			float th = 1.0f / 1;						// テクスチャの高さ
			float tx = x * tw;							// テクスチャの左上X座標
			float ty = 0.0f;							// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

			// 次の桁へ
			numberBoss /= 10;
		}

	}


	// Scoreのロゴの描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[SCORE_LOGO]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, SCORE_POS_X, SCORE_POS_Y, SCORE_TEXTURE_WIDTH, SCORE_TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}





	// NPCのロゴの描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[NPC_LOGO]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, NPC_LOGO_POS_X, NPC_LOGO_POS_Y, NPC_LOGO_WIDTH, NPC_LOGO_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
	

	// BOSSのロゴの描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[BOSS_LOGO]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, BOSS_LOGO_POS_X, BOSS_LOGO_POS_Y, BOSS_LOGO_WIDTH, BOSS_LOGO_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

}


//=============================================================================
// スコアを加算する
// 引数:add :追加する点数。マイナスも可能
//=============================================================================
void AddScore(int add)
{
	g_Score += add;
	if (g_Score > SCORE_MAX)
	{
		g_Score = SCORE_MAX;
	}

}

//=============================================================================
// スコアを減算する
// 引数:add :マイナスにする
//=============================================================================
void MinusScore(int add)
{
	g_Score -= add;
	if (g_Score < SCORE_MIN)
	{
		g_Score = SCORE_MIN;
	}
}


int GetScore(void)
{
	return g_Score;
}


void NpcDown(int add)
{
	g_Down -= add;
}





