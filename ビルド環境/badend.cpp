//=============================================================================
//
// リザルト画面処理 [badend.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "badend.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(4)				// テクスチャの数

#define TEXTURE_WIDTH_LOGO			(1521 / 2)			// ロゴサイズ
#define TEXTURE_HEIGHT_LOGO			(554 / 2)			//

#define TEXTURE_WIDTH_CUR			(60)			// 選択肢サイズ
#define TEXTURE_HEIGHT_CUR			(40)			// 選択肢サイズ

#define SCROLL_MAX					(300)			// スクロールマックス

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/ending02.png",
	"data/TEXTURE/ending_logo.png",
	"data/TEXTURE/choice04.png",
};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

static BOOL						g_Load = FALSE;

// Choice
int								cur3;

// スクロールアニメーション
float							scrolldown;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBadEnd(void)
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
	g_Pos   = { g_w / 2, 50.0f, 0.0f };
	g_TexNo = 0;

	// BGM再生
	PlaySound(SOUND_LABEL_SE_GAMEOVER);
	
	// スクロールアニメーション
	scrolldown = 0;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBadEnd(void)
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
void UpdateBadEnd(void)
{

	// Choice
	if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, STICK_L_DOWN))
	{
		cur3 = (++cur3 + BADEND_TITLE_MAX) % BADEND_TITLE_MAX;
		PlaySound(SOUND_LABEL_SE_CLICK01);
	}
	else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, STICK_L_UP))
	{
		cur3 = (--cur3 + BADEND_TITLE_MAX) % BADEND_TITLE_MAX;
		PlaySound(SOUND_LABEL_SE_CLICK01);
	}


	switch (cur3)
	{
	case BADEND_RETRY:

		g_Pos = XMFLOAT3(350.0f, 350.0f, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A))
		{		// Enter押したら、ステージを切り替える
				// SetMode(MODE_GAME);
			SetFade(FADE_OUT, MODE_GAME);
			PlaySound(SOUND_LABEL_SE_PICK);
		}
		break;

	case BADEND_START:
		g_TexNo = 3;
		g_Pos = XMFLOAT3(350.0f, 400.0f, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A))
		{		// Enter押したら、ステージを切り替える
		
			SetFade(FADE_OUT, MODE_TITLE);
			PlaySound(SOUND_LABEL_SE_PICK);
		}
		break;

	case BADEND_EXIT:
		g_TexNo = 3;
		g_Pos = XMFLOAT3(350.0f, 450.0f, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A))
		{
			GameLoop();
		}
		break;
	}




	scrolldown += 1;
	if (scrolldown >= SCROLL_MAX)
	{
		scrolldown = SCROLL_MAX;
	}


	

#ifdef _DEBUG	// デバッグ情報を表示する
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBadEnd(void)
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

	// バッドエンドの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, g_w, g_h, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	// バッドエンドのロゴを描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSprite(g_VertexBuffer, 470.0f, scrolldown, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	// Choice
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, g_Pos.x + 420.0f, g_Pos.y + 30.0f, TEXTURE_WIDTH_CUR, TEXTURE_HEIGHT_CUR, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}



}




