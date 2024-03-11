//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : ワッタナチャイモンコン　カンチャノ
//
//=============================================================================
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(6)				// テクスチャの数

#define TEXTURE_WIDTH_LOGO			(859 / 2)			// ロゴサイズ
#define TEXTURE_HEIGHT_LOGO			(311 / 2)			// 

#define TEXTURE_WIDTH_CUR			(60)			// 選択肢サイズ
#define TEXTURE_HEIGHT_CUR			(40)			// 選択肢サイズ

// アニメーション用	
#define TEXTURE_WIDTH_ANIM			(500 / 4)			// 選択肢サイズ
#define TEXTURE_HEIGHT_ANIM			(400 / 4)			// 選択肢サイズ

#define ANIM_POS_X1					(150)				// アニメーションの配置
#define ANIM_POS_Y1					(230)

//////////////////////
#define TEXTURE_WIDTH_ANIM2			(500 / 4)			// 選択肢サイズ
#define TEXTURE_HEIGHT_ANIM2		(150 / 4)			// 選択肢サイズ

#define ANIM_POS_X2					(450)				// アニメーションの配置
#define ANIM_POS_Y2					(230)

//////////////////////
#define TEXTURE_WIDTH_ANIM3			(775 / 4)			// 選択肢サイズ
#define TEXTURE_HEIGHT_ANIM3		(98 / 4)			// 選択肢サイズ

#define ANIM_POS_X3					(800)				// アニメーションの配置
#define ANIM_POS_Y3					(230)

//////////////////////
#define TEXTURE_PATTERN_DIVIDE_X	(2)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(30)		// アニメーションの切り替わるWait値


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/Tutorial01.png",
	"data/TEXTURE/effect000.jpg",
	"data/TEXTURE/chioce03.png",
	"data/TEXTURE/Key01.png",
	"data/TEXTURE/Key02.png",
	"data/TEXTURE/Key03.png",
};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

static BOOL						g_Load = FALSE;

static float	effect_dx;
static float	effect_dy;

float	tutorial_alpha;
BOOL	tutorial_flag_alpha;

// Choice
int				cur1;

// アニメーション用
XMFLOAT3	pos;			// ポリゴンの座標
XMFLOAT3	rot;			// ポリゴンの回転量
XMFLOAT3	scl;			// ポリゴンの拡大縮小
float		AnimW, AnimH;			// 幅と高さ
float		countAnim;		// アニメーションカウント
int			patternAnim;	// アニメーションパターンナンバー


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTutorial(void)
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

	tutorial_alpha = 1.0f;
	tutorial_flag_alpha = TRUE;

	effect_dx = 100.0f;
	effect_dy = 100.0f;


	// アニメーション用
	pos = XMFLOAT3(0.0f, 0.0f, 0.0f);			// ポリゴンの座標
	rot = XMFLOAT3(0.0f, 0.0f, 0.0f);						// ポリゴンの回転量
	scl = XMFLOAT3(1.0f, 1.0f, 1.0f);						// ポリゴンの拡大縮小
	AnimW = TEXTURE_WIDTH_ANIM;
	AnimH = TEXTURE_HEIGHT_ANIM;
	countAnim = 0;
	patternAnim = 0;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTutorial(void)
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
void UpdateTutorial(void)
{

	// Choice
	if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, STICK_L_DOWN))
	{
		cur1 = (++cur1 + TUTORIAL_MAX) % TUTORIAL_MAX;
		PlaySound(SOUND_LABEL_SE_CLICK01);

	}
	else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, STICK_L_UP))
	{
		cur1 = (--cur1 + TUTORIAL_MAX) % TUTORIAL_MAX;
		PlaySound(SOUND_LABEL_SE_CLICK01);
	}


	switch (cur1)
	{
	case TUTORIAL_START:
		g_TexNo = 2;
		g_Pos = XMFLOAT3(700.0f, 430.0f, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A))
		{		// Enter押したら、ステージを切り替える
				// SetMode(MODE_GAME);
			SetFade(FADE_OUT, MODE_STORY);
			PlaySound(SOUND_LABEL_SE_PICK);
		}
		break;

	case TUTORIAL_EXIT:
		g_TexNo = 2;
		g_Pos = XMFLOAT3(700.0f, 470.0f, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A))
		{
			SetFade(FADE_OUT, MODE_TITLE);
			PlaySound(SOUND_LABEL_SE_PICK);
		}
		break;
	}



	// アニメーション  
	countAnim += 1.0f;
	if (countAnim > ANIM_WAIT)
	{
		countAnim = 0.0f;
		// パターンの切り替え
		patternAnim = (patternAnim + 1) % ANIM_PATTERN_NUM;
	}

#ifdef _DEBUG	// デバッグ情報を表示する
	//PrintDebugProc("Player:↑ → ↓ ←　Space\n");

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTutorial(void)
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


	//// 加減算のテスト
	//SetBlendState(BLEND_MODE_ADD);			// 加算合成
	//// SetBlendState(BLEND_MODE_SUBTRACT);	// 減算合成
	//	
	//// テクスチャ設定
	//GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);
	//

	//SetBlendState(BLEND_MODE_ALPHABLEND);		// 半透明処理を元に戻す
	




		//Choice
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, g_Pos.x - 20.0f, g_Pos.y, TEXTURE_WIDTH_CUR, TEXTURE_HEIGHT_CUR, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}






}





