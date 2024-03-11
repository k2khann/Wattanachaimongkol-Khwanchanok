//=============================================================================
//
// タイトル画面処理 [story.cpp]
// Author : ワッタナチャイモンコン　カンチャノ
//
//=============================================================================
#include "story.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "light.h"
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(8)				// テクスチャの数

#define TEXTURE_WIDTH_NEXT			(120)			// 選択肢サイズ
#define TEXTURE_HEIGHT_NEXT			(60)			// 選択肢サイズ

#define TEXTURE_WIDTH_SUB			(750)
#define TEXTURE_HEIGHT_SUB			(100)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/story01.jpg",
	"data/TEXTURE/story02.jpg",
	"data/TEXTURE/story03.jpg",

	"data/TEXTURE/sub01.png",
	"data/TEXTURE/sub02.png",
	"data/TEXTURE/sub03.png",

	"data/TEXTURE/enterkey.png",



};



static BOOL		g_Load = FALSE;		// データ読み込みフラグをOFF
static STORY	g_Story[TEXTURE_MAX];

static float	alpha;
static BOOL		flag_alpha;

 // ストーリーのページ
static int		g_TexNo;
static XMFLOAT3 g_Pos;
static int		g_subNo;

// 字幕用
float			sub_alpha;


static INTERPOLATION_DATA g_ScrollTbl0[] = {

	//座標								回転率							拡大率						時間
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	500 },
	{ XMFLOAT3(0.0f, -100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(2.0f, 2.0f, 2.0f),	300 },

};


static INTERPOLATION_DATA g_ScrollTbl1[] = {

	//座標								回転率							拡大率						時間
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	500 },
	{ XMFLOAT3(-100.0f, -100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(2.0f, 2.0f, 2.0f),	300 },

};

static INTERPOLATION_DATA g_ScrollTbl2[] = {

	//座標								回転率							拡大率						時間
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	500 },
	{ XMFLOAT3(400.0f, -100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(2.0f, 2.0f, 2.0f),	300 },

};



static INTERPOLATION_DATA* g_ScrollTblAdr[] =
{
	 g_ScrollTbl0,
	 g_ScrollTbl1,
	 g_ScrollTbl2,

	 
};



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitStory(void)
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

		// 変数の初期化
		g_Story[i].w = TEXTURE_WIDTH;
		g_Story[i].h = TEXTURE_HEIGHT;
		g_Story[i].pos = XMFLOAT3(g_Story[i].w / 2, g_Story[i].h / 2, 0.0f);
		g_Story[i].texNo = 0;


		// //0番だけ線形補間で動かしてみる
		//g_Story[i].time = 0.0f;		// 線形補間用のタイマーをクリア
		//g_Story[i].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
		//g_Story[i].tblMax = sizeof(g_ScrollTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット


	}


	//0番だけ線形補間で動かしてみる
	g_Story[0].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Story[0].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
	g_Story[0].tblMax = sizeof(g_ScrollTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	//1番だけ線形補間で動かしてみる
	g_Story[1].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Story[1].tblNo = 1;		// 再生するアニメデータの先頭アドレスをセット
	g_Story[1].tblMax = sizeof(g_ScrollTbl1) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット


	//2番だけ線形補間で動かしてみる
	g_Story[2].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Story[2].tblNo = 2;		// 再生するアニメデータの先頭アドレスをセット
	g_Story[2].tblMax = sizeof(g_ScrollTbl2) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット


	// 線形補間用
	g_TexNo = 0;
	g_subNo = (0 + SUBTITLE_MAX) % SUBTITLE_MAX + STORY_MAX;


	// 頂点バッファ生成（プリミティブ）
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// ボタン 用
	alpha = 1.0f;
	flag_alpha = TRUE;

	// 字幕用
	sub_alpha = 0.0f;

	// BGM再生
	PlaySound(SOUND_LABEL_BGM_STORY);


	g_Load = TRUE;
	return S_OK;

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStory(void)
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
void UpdateStory(void)
{
	// ストーリーのページ
	switch (g_TexNo)
	{
	case STORY_PAGE01:

		if ((GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A)))
		{
			g_TexNo = (++g_TexNo + STORY_MAX) % STORY_MAX;
			g_subNo = (++g_subNo + SUBTITLE_MAX) % SUBTITLE_MAX + STORY_MAX;
			sub_alpha = 0.0f;

			g_Story[1].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);				// ポリゴンの座標
			g_Story[1].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);				// モデルの向き(回転)
			g_Story[1].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);				// モデルの大きさ(スケール)


		}
		break;


	case STORY_PAGE02:
		if ((GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A)))
		{
			g_TexNo = (++g_TexNo + STORY_MAX) % STORY_MAX;
			g_subNo = (++g_subNo + SUBTITLE_MAX) % SUBTITLE_MAX + STORY_MAX;
			sub_alpha = 0.0f;

			g_Story->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);				// ポリゴンの座標
			g_Story->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);				// モデルの向き(回転)
			g_Story->scl = XMFLOAT3(1.0f, 1.0f, 1.0f);				// モデルの大きさ(スケール)

		}
		break;

	case STORY_PAGE03:
		if ((GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A)))
		{
			//if ((GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A)))
			//{
				// Enterキーを押したら、ゲーム入る
				SetFade(FADE_OUT, MODE_GAME);
				PlaySound(SOUND_LABEL_SE_PICK);
			//}

		}
		break;




		//case ENTER_GAME:

		//	 ゲーム入る
		//	if ((GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A)))
		//	{		// Enter押したら、ステージを切り替える
		//			 SetMode(MODE_GAME);
		//		SetFade(FADE_OUT, MODE_GAME);
		//		PlaySound(SOUND_LABEL_SE_PICK);
		//	}
		//	break;
	}






	// Enterキー用
	if (flag_alpha == TRUE)
	{
		alpha -= 0.02f;
		if (alpha <= 0.0f)
		{
			alpha = 0.0f;
			flag_alpha = FALSE;
		}
	}
	else
	{
		alpha += 0.02f;
		if (alpha >= 1.0f)
		{
			alpha = 1.0f;
			flag_alpha = TRUE;
		}
	}

	// 字幕用
	sub_alpha += 0.01;
	if (sub_alpha > 1.0f)
	{
		sub_alpha = 1.0f;
	}


	// // キーボード
	// if (GetKeyboardTrigger(DIK_RETURN))
	// {// Enter押したら、ステージを切り替える
	// 	// SetMode(MODE_GAME);
	// 	SetFade(FADE_OUT, MODE_GAME);
	// }
	// // ゲームパッドで入力処理
	// else if (IsButtonTriggered(0, BUTTON_START))
	// {
	// 	// SetMode(MODE_GAME);
	// 	SetFade(FADE_OUT, MODE_GAME);
	// }
	// else if (IsButtonTriggered(0, BUTTON_B))
	// {
	// 	// SetMode(MODE_GAME);
	// 	SetFade(FADE_OUT, MODE_GAME);
	// }




	for (int i = 0; i < TEXTURE_MAX; i++)
	{

		// 移動処理
		if (g_Story[i].tblMax > 0)						// 線形補間を実行する？
		{	// 線形補間の処理
			int nowNo = (int)g_Story[i].time;			// 整数分であるテーブル番号を取り出している
			int maxNo = g_Story[i].tblMax;				// 登録テーブル数を数えている
			int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
			INTERPOLATION_DATA* tbl = g_ScrollTblAdr[g_Story[i].tblNo];	// 行動テーブルのアドレスを取得

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
			XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
			XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
			XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
			XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

			float nowTime = g_Story[i].time - nowNo;	// 時間部分である少数を取り出している

			Pos *= nowTime;								// 現在の移動量を計算している
			Rot *= nowTime;								// 現在の回転量を計算している
			Scl *= nowTime;								// 現在の拡大率を計算している

			// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
			XMStoreFloat3(&g_Story[i].pos, nowPos + Pos);

			// 計算して求めた回転量を現在の移動テーブルに足している
			XMStoreFloat3(&g_Story[i].rot, nowRot + Rot);

			// 計算して求めた拡大率を現在の移動テーブルに足している
			XMStoreFloat3(&g_Story[i].scl, nowScl + Scl);
			//g_Enemy[i].w = ENEMY_SIZE * g_Enemy[i].scl.x;
			//g_Enemy[i].h = TEXTURE_HEIGHT * g_Enemy[i].scl.y;

			// frameを使て時間経過処理をする
			g_Story[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている

			//if ((int)g_Story[i].time >= (maxNo - 1))
			//{
			//	g_Story[i].tblMax = -1;					// Loop しないように　****
			//}

			if ((int)g_Story[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
			{
				g_Story[i].time -= maxNo;					// ０番目にリセットしつつも小数部分を引き継いでいる]

			}


		}

	}






#ifdef _DEBUG	// デバッグ情報を表示する
	//PrintDebugProc("Player:↑ → ↓ ←　Space\n");

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawStory(void)
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




	// ストーリーの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		float pw = TEXTURE_WIDTH * g_Story[g_TexNo].scl.x;
		float ph = TEXTURE_HEIGHT * g_Story[g_TexNo].scl.y;

		float px = SCREEN_CENTER_X + g_Story[g_TexNo].pos.x;
		float py = SCREEN_CENTER_Y + g_Story[g_TexNo].pos.y;

		SetSprite(g_VertexBuffer, px, py, pw, ph, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
	





	// ボタンのロゴを描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[6]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		// SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
		SetSpriteColor(g_VertexBuffer, 870.0f, 50.0f, TEXTURE_WIDTH_NEXT, TEXTURE_HEIGHT_NEXT, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}



	 // 字幕（Story）を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_subNo]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		// SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
		SetSpriteColor(g_VertexBuffer, 500.0f, 450.0f, TEXTURE_WIDTH_SUB, TEXTURE_HEIGHT_SUB, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, sub_alpha));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}



	//}
/*
	// 加減算のテスト
	SetBlendState(BLEND_MODE_ADD);		// 加算合成
	//SetBlendState(BLEND_MODE_SUBTRACT);	// 減算合成
	for (int i = 0; i < 100; i++)
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		float dx = 150.0f;
		float dy = 150.0f;
		float angle = rand() % 360;
		float speed = rand() % 50;
		float sx = cosf(angle) * speed;
		float sy = sinf(angle) * speed;

		SetSpriteColor(g_VertexBuffer, dx + sx, dy + sy, 50, 50, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 0.1f, 0.0f, 0.5f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
	SetBlendState(BLEND_MODE_ALPHABLEND);	// 半透明処理を元に戻す
*/
}


