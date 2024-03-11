//=============================================================================
//
// UI処理 [ui.cpp]
// Author : 
//
//=============================================================================
#include "debugproc.h"
#include "ui.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "score.h"
#include "fade.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(5)				// テクスチャの数

#define TEXTURE_WIDTH_LIFE			(40)			// ハートサイズ
#define TEXTURE_HEIGHT_LIFE			(40)			// 

#define LIFE_POS_X					(170)			// ハートの POS
#define LIFE_POS_Y					(60)

#define TEXTURE_WIDTH_ICON			(130)			// プレイヤーのアイコンサイズ
#define TEXTURE_HEIGHT_ICON			(130)			// 

#define ICON_POS_X					(20)			// ハートの POS
#define ICON_POS_Y					(10)

// アニメーション用
#define TEXTURE_WIDTH_ANIM			(256 / 4)			// 選択肢サイズ
#define TEXTURE_HEIGHT_ANIM			(256 / 4)			// 選択肢サイズ

#define ANIM_POS_X					(90)				// アニメーションの配置
#define ANIM_POS_Y					(78)

#define TEXTURE_PATTERN_DIVIDE_X	(2)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(80)		// アニメーションの切り替わるWait値

// AIM point 
#define AIM_TEXTURE_WIDTH			(429 / 8)	// Scoreのロゴサイズ
#define AIM_TEXTURE_HEIGHT			(446 / 8)

#define AIM_POS_X					(420)	// ScoreのロゴのPOS
#define AIM_POS_Y					(250)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/Life03.png",			// グレー
	"data/TEXTURE/Life02.png",			// 赤

	"data/TEXTURE/IconBG01.png",		// プレイヤーのアイコン
	"data/TEXTURE/PenAnim03.png",		// アニメーション用

	"data/TEXTURE/Aim01.png",			// Aim
};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

static BOOL						g_Load = FALSE;


// アニメーション用
XMFLOAT3	ui_pos;						// ポリゴンの座標
XMFLOAT3	ui_rot;						// ポリゴンの回転量
XMFLOAT3	ui_scl;						// ポリゴンの拡大縮小
float		ui_AnimW, ui_AnimH;			// 幅と高さ
float		ui_countAnim;				// アニメーションカウント
int			ui_patternAnim;				// アニメーションパターンナンバー


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitUi(void)
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


	// アニメーション用
	ui_pos = XMFLOAT3(ANIM_POS_X, ANIM_POS_Y, 0.0f);		// ポリゴンの座標
	ui_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);					// ポリゴンの回転量
	ui_scl = XMFLOAT3(1.0f, 1.0f, 1.0f);					// ポリゴンの拡大縮小
	ui_AnimW = TEXTURE_WIDTH_ANIM;
	ui_AnimH = TEXTURE_HEIGHT_ANIM;
	ui_countAnim = 0;
	ui_patternAnim = 0;


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitUi(void)
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
void UpdateUi(void)
{

	// アニメーション  
	ui_countAnim += 1.0f;
	if (ui_countAnim > ANIM_WAIT)
	{
		ui_countAnim = 0.0f;

		// パターンの切り替え
		ui_patternAnim = (ui_patternAnim + 1) % ANIM_PATTERN_NUM;
	}

#ifdef _DEBUG	// デバッグ情報を表示する
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawUi(void)
{
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

	// ハートらいふ（黒）のUI描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[LIFE_BLACK]);

		for (int i = 0; i < PLAYER_LIFE; i++)
		{
			// スコアの位置やテクスチャー座標を反映
			//表示位置
			float pw = TEXTURE_WIDTH_LIFE;			// スコアの表示幅
			float ph = TEXTURE_HEIGHT_LIFE;			// スコアの表示高さ
			float px = LIFE_POS_X + (i * 50);		// スコアの表示位置X	幅
			float py = LIFE_POS_Y;					// スコアの表示位置Y

			float tw = 1.0f;				// テクスチャの幅
			float th = 1.0f;				// テクスチャの高さ
			float tx = 0.0f;				// テクスチャの左上X座標
			float ty = 0.0f;				// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

		}
	}

	// ハートらいふ（青）のUI描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[LIFE_BLUE]);
		
		PLAYER* player = GetPlayer();

		for (int j = 0; j < player[0].hp ; j++)
		{
			// スコアの位置やテクスチャー座標を反映
			//表示位置
			float pw = TEXTURE_WIDTH_LIFE;			// スコアの表示幅
			float ph = TEXTURE_HEIGHT_LIFE;			// スコアの表示高さ
			float px = LIFE_POS_X + (j * 50);		// スコアの表示位置X	幅
			float py = LIFE_POS_Y;					// スコアの表示位置Y

			float tw = 1.0f;				// テクスチャの幅
			float th = 1.0f;				// テクスチャの高さ
			float tx = 0.0f;				// テクスチャの左上X座標
			float ty = 0.0f;				// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

		}

	}





	//プレイヤーのアイコンのUI描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[PLAYER_ICON]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, ICON_POS_X, ICON_POS_Y, TEXTURE_WIDTH_ICON, TEXTURE_HEIGHT_ICON, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}






	// アニメーション用
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[PLAYER_ICON_ANIM]);

		//プレイヤーの位置やテクスチャー座標を反映
		float uix = ui_pos.x;		// プレイヤーの表示位置X
		float uiy = ui_pos.y;		// プレイヤーの表示位置Y
		float uiw = ui_AnimW;		// プレイヤーの表示幅
		float uih = ui_AnimH;		// プレイヤーの表示高さ

		// アニメーション用
		float uitw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
		float uith = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
		float uitx = (float)(ui_patternAnim % TEXTURE_PATTERN_DIVIDE_X) * uitw;	// テクスチャの左上X座標
		float uity = (float)(ui_patternAnim / TEXTURE_PATTERN_DIVIDE_X) * uith;	// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColorRotation(g_VertexBuffer, uix, uiy, uiw, uih, uitx, uity, uitw, uith,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			ui_rot.z);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}


	// AIMのロゴの描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[AIM_POINT]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, AIM_POS_X, AIM_POS_Y, AIM_TEXTURE_WIDTH, AIM_TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}


}




