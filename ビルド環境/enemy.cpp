//=============================================================================
//
// エネミーモデル処理 [enemy.cpp]
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
#include "bullet.h"
#include "shadow.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY			"data/MODEL/YETIS/YETIS.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(5.0f)						// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量

#define ENEMY_SHADOW_SIZE	(0.4f)						// 影の大きさ
#define ENEMY_OFFSET_Y		(12.0f)						// エネミーの足元をあわせる

#define ENEMY_SPAWN_RANGE_X	(800)
#define ENEMY_SPAWN_RANGE_Z	(800)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];				// エネミー

int g_Enemy_load = 0;

// リースポーン用
static int				g_EnemyCnt = MAX_ENEMY;
static int				spawncnt;						//スポーンカウント

//static INTERPOLATION_DATA g_MoveTbl0[] = {	// pos, rot, scl, frame
//	{ XMFLOAT3(0.0f, ENEMY_OFFSET_Y,  20.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
//	{ XMFLOAT3(-200.0f, ENEMY_OFFSET_Y,  20.0f), XMFLOAT3(0.0f, 6.28f, 0.0f), XMFLOAT3(3.0f, 3.0f, 3.0f), 60 * 1 },
//	{ XMFLOAT3(-200.0f, ENEMY_OFFSET_Y, 200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.5f },
//
//};
//
//
//static INTERPOLATION_DATA* g_MoveTblAdr[] =
//{
//	g_MoveTbl0,
//
//};



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		LoadModel(MODEL_ENEMY, &g_Enemy[i].model);
		g_Enemy[i].load = TRUE;

		g_Enemy[i].pos = XMFLOAT3(0.0f, ENEMY_OFFSET_Y, 0.0f);
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Enemy[i].spd  = 0.0f;			// 移動スピードクリア
		g_Enemy[i].size = ENEMY_SIZE;	// 当たり判定の大きさ

		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Enemy[i].model, &g_Enemy[i].diffuse[0]);

		XMFLOAT3 pos = g_Enemy[i].pos;
		pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);
		
		g_Enemy[i].time = 0.0f;			// 線形補間用のタイマーをクリア
		g_Enemy[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Enemy[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

		g_Enemy[i].use = FALSE;			// TRUE:生きてる

	}


	// リースポーン用
	//g_EnemyCnt = 0;
	//spawncnt = 0;

	//// 0番だけ線形補間で動かしてみる
	//g_Enemy[0].time = 0.0f;		// 線形補間用のタイマーをクリア
	//g_Enemy[0].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
	//g_Enemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].load)
		{
			UnloadModel(&g_Enemy[i].model);
			g_Enemy[i].load = FALSE;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	// リースポーン用
	g_EnemyCnt = 0;				// 生きてるエネミーの数
	spawncnt += (10);

	if (spawncnt >= 1000)
	{
		SetEnemy();
		spawncnt = 0;

	}

	// エネミーを動かく場合は、影も合わせて動かす事を忘れないようにね！
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == TRUE)		// このエネミーが使われている？
		{								// Yes

		//// 移動処理
		//if (g_Enemy[i].tblMax > 0)	// 線形補間を実行する？
		//{	// 線形補間の処理
		//	int nowNo = (int)g_Enemy[i].time;			// 整数分であるテーブル番号を取り出している
		//	int maxNo = g_Enemy[i].tblMax;				// 登録テーブル数を数えている
		//	int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
		//	INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Enemy[i].tblNo];	// 行動テーブルのアドレスを取得

		//	XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
		//	XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
		//	XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

		//	XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
		//	XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
		//	XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

		//	float nowTime = g_Enemy[i].time - nowNo;	// 時間部分である少数を取り出している

		//	Pos *= nowTime;								// 現在の移動量を計算している
		//	Rot *= nowTime;								// 現在の回転量を計算している
		//	Scl *= nowTime;								// 現在の拡大率を計算している

		//	// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
		//	XMStoreFloat3(&g_Enemy[i].pos, nowPos + Pos);

		//	// 計算して求めた回転量を現在の移動テーブルに足している
		//	XMStoreFloat3(&g_Enemy[i].rot, nowRot + Rot);

		//	// 計算して求めた拡大率を現在の移動テーブルに足している
		//	XMStoreFloat3(&g_Enemy[i].scl, nowScl + Scl);

		//	// frameを使て時間経過処理をする
		//	g_Enemy[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
		//	if ((int)g_Enemy[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
		//	{
		//		g_Enemy[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
		//	}

		//}




		PLAYER* player = GetPlayer();

		XMVECTOR epos = XMLoadFloat3(&g_Enemy[i].pos);
		XMVECTOR vec = XMLoadFloat3(&player[0].pos) - epos;				//自分とプレイヤーとの差分を求めている
		float angle = atan2f(vec.m128_f32[2], vec.m128_f32[0]);			//その差分を使って角度を求めている
		float speed = 1.0f;												//スピードはちょっと遅くしてみる

		g_Enemy[i].pos.x += cosf(angle) * speed;						//angleの方向へ移動
		g_Enemy[i].pos.z += sinf(angle) * speed;						//angleの方向へ移動

		g_Enemy[i].rot.y = (angle + XM_PI / 2) * -1;







			// 影もプレイヤーの位置に合わせる
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Enemy[i].shadowIdx, pos);
		}
	}






#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_P))
	{
		// モデルの色を変更できるよ！半透明にもできるよ。
		for (int j = 0; j < g_Enemy[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Enemy[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
		}
	}

	if (GetKeyboardTrigger(DIK_L))
	{
		// モデルの色を元に戻している
		for (int j = 0; j < g_Enemy[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Enemy[0].model, j, g_Enemy[0].diffuse[j]);
		}
	}
#endif


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == FALSE) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_Enemy[i].model);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
ENEMY *GetEnemy()
{
	return &g_Enemy[0];
}


//=============================================================================
// エネミーのリースポーン関数
//=============================================================================
void SetEnemy(void)
{
	int start = rand() % MAX_ENEMY;

	for (int n = start; n < MAX_ENEMY + start; n++)
	{
		int i = n % MAX_ENEMY;
		if (g_Enemy[i].use == FALSE)				// 未使用状態のenemyを見つける
		{
			XMFLOAT3 pos;

			pos.x = (float)(rand() % ENEMY_SPAWN_RANGE_X) - (ENEMY_SPAWN_RANGE_X * 0.5f);
			pos.y = ENEMY_OFFSET_Y;							
			pos.z = (float)(rand() % ENEMY_SPAWN_RANGE_Z) - (ENEMY_SPAWN_RANGE_Z * 0.5);
			
			g_Enemy[i].use = TRUE;					// 使用状態へ変更する
			g_Enemy[i].pos = pos;					// 座標をセット
			return;									// 1発セットしたので終了する
		}
	}
}