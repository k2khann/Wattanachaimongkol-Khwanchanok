//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "title.h"
#include "story.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(3)				// テクスチャの数

#define TEXTURE_WIDTH_LOGO			(859 / 2)		// ロゴサイズ
#define TEXTURE_HEIGHT_LOGO			(311 / 2)		// 

#define TEXTURE_WIDTH_CUR			(60)			// 選択肢サイズ
#define TEXTURE_HEIGHT_CUR			(40)			// 選択肢サイズ

#define ANGLE_MOVE					(360.0f / TEXTURE_HEIGHT_LOGO)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/Title02.png",
	"data/TEXTURE/Title_Logo01.png",
	"data/TEXTURE/chioce03.png",
};

static BOOL		g_Load = FALSE;		// データ読み込みフラグをOFF
static TITLE	g_Title;

static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

// Logo
float	alpha;
BOOL	flag_alpha;

float g_angle = 0.0f;
float amplitude;

// Logo
static	TITLE_LOGO	g_TitleLogo[TEXTURE_HEIGHT_LOGO];
float	startAngle = 0;

// Choice
int				cur;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
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


	// 変数の初期化
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = XMFLOAT3(g_w/2, g_h/2, 0.0f);
	g_TexNo = 0;

	for (int i = 0; i < TEXTURE_HEIGHT_LOGO; i++)
	{
		g_TitleLogo[i].pos = XMFLOAT3(250.0f, 130.0f + i, 0.0f);
		g_TitleLogo[i].use = TRUE;
		g_TitleLogo[i].w = TEXTURE_WIDTH_LOGO;
		g_TitleLogo[i].h = 1;
		g_TitleLogo[i].texNo = 1;

		g_TitleLogo[i].countAnim = 0;
		g_TitleLogo[i].patternAnim = 0;
	}

	alpha = 1.0f;
	flag_alpha = TRUE;

	// BGM再生
	PlaySound(SOUND_LABEL_BGM_TITLE);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
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
void UpdateTitle(void)
{

	// Choice
	if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, STICK_L_DOWN))
	{
		cur = (++cur + TITLE_MAX) % TITLE_MAX;
		PlaySound(SOUND_LABEL_SE_CLICK01);
	}
	else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, STICK_L_UP))
	{
		cur = (--cur + TITLE_MAX) % TITLE_MAX;
		PlaySound(SOUND_LABEL_SE_CLICK01);
	}


	switch (cur)
	{
	case START:
		g_TexNo = 2;
		g_Pos = XMFLOAT3(320.0f, 380.0f, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A))
		{		// Enter押したら、ステージを切り替える
				// SetMode(MODE_GAME);
			SetFade(FADE_OUT, MODE_STORY);
			PlaySound(SOUND_LABEL_SE_PICK);
		}
		break;

	case TUTORIAL:
		g_TexNo = 2;
		g_Pos = XMFLOAT3(320.0f, 420.0f, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A))
		{		// Enter押したら、ステージを切り替える
				// SetMode(MODE_GAME);
			SetFade(FADE_OUT, MODE_TUTORIAL);
			PlaySound(SOUND_LABEL_SE_PICK);
		}
		break;

	case EXIT:
		g_TexNo = 2;
		g_Pos = XMFLOAT3(320.0f, 470.0f, 0.0f);

		if (GetKeyboardPress(DIK_RETURN) || IsButtonPressed(0, BUTTON_A))
		{
			GameLoop();
		}

	}


	// Logo
	{

		for (int i = 0; i < TEXTURE_HEIGHT_LOGO; i++)
		{
			if (g_TitleLogo[i].use = TRUE)
			{
				float DrawAngle = ANGLE_MOVE * i + startAngle;
				// float radian = (XM_PI / 180) * DrawAngle;
				float radian = XMConvertToRadians(DrawAngle);
				g_TitleLogo[i].pos.x = 500.0f + (sinf(radian) * 10);
			}
		}

		startAngle += ANGLE_MOVE;

		if (startAngle > 360.0f)
		{
			startAngle -= 360.0f;
		}

	}

	






#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
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

	// タイトルの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}



	//Choice
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, g_Pos.x + 15.0f, g_Pos.y, TEXTURE_WIDTH_CUR, TEXTURE_HEIGHT_CUR, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}



	for (int i = 0; i < TEXTURE_HEIGHT_LOGO; i++)
	{
		if (g_TitleLogo[i].use == TRUE)
		{
			{
				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TitleLogo[i].texNo]);

				//プレイヤーの位置やテクスチャー座標を反映
				float px = g_TitleLogo[i].pos.x - 200.0f;		// プレイヤーの表示位置X
				float py = g_TitleLogo[i].pos.y + 10.0f;		// プレイヤーの表示位置Y
				float pw = g_TitleLogo[i].w;			// プレイヤーの表示幅
				float ph = g_TitleLogo[i].h;			// プレイヤーの表示高さ
				// float pz = g_TitleLogo[i].rot.z;		// プレイヤーの回転

				float tw = 1.0f;							// テクスチャの幅
				float th = 1.0f / TEXTURE_HEIGHT_LOGO;		// テクスチャの高さ
				float tx = 0.0f;							// テクスチャの左上X座標
				float ty = 1.0f / TEXTURE_HEIGHT_LOGO * i;	// テクスチャの左上Y座標

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);

			}
		}

	}

//	// 加減算のテスト
//	SetBlendState(BLEND_MODE_ADD);		// 加算合成
////	SetBlendState(BLEND_MODE_SUBTRACT);	// 減算合成
//	for(int i=0; i<30; i++)
//	{
//		// テクスチャ設定
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);
//
//		// １枚のポリゴンの頂点とテクスチャ座標を設定
//		float dx = 100.0f;
//		float dy = 100.0f;
//		float sx = (float)(rand() % 100);
//		float sy = (float)(rand() % 100);
//
//
//		SetSpriteColor(g_VertexBuffer, dx+sx, dy+sy, 50, 50, 0.0f, 0.0f, 1.0f, 1.0f,
//			XMFLOAT4(0.3f, 0.3f, 1.0f, 0.5f));
//
//		// ポリゴン描画
//		GetDeviceContext()->Draw(4, 0);
//	}
//	SetBlendState(BLEND_MODE_ALPHABLEND);	// 半透明処理を元に戻す

}





