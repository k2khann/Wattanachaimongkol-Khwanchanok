//=============================================================================
//
// タイトル画面処理 [story.h]
// Author : ワッタナチャイモンコン　カンチャノ
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct STORY
{
	XMFLOAT3	pos;		// ポリゴンの座標
	XMFLOAT3	rot;				// モデルの向き(回転)
	XMFLOAT3	scl;				// モデルの大きさ(スケール)
	float		w, h;		// 幅と高さ
	int			texNo;		// 使用しているテクスチャ番号


	// 線形補間用

	XMFLOAT3	Direction;	// ライトの方向
	XMFLOAT4	Diffuse;	// 拡散光の色


	float		time;			// 線形補間用
	int			tblNo;			// 行動データのテーブル番号
	int			tblMax;			// そのテーブルのデータ数

};

struct STORY_LOGO
{
	XMFLOAT3	pos;		// ポリゴンの座標
	BOOL		use;		//
	float		w, h;		// 幅と高さ
	int			texNo;		// 使用しているテクスチャ番号

	float	countAnim;
	int		patternAnim;
};

 // ストーリーのページ
enum
{
	STORY_PAGE01,
	STORY_PAGE02,
	STORY_PAGE03,

	STORY_MAX
};

enum
{
	SUBTITLE01,
	SUBTITLE02,
	SUBTITLE03,

	SUBTITLE_MAX,
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitStory(void);
void UninitStory(void);
void UpdateStory(void);
void DrawStory(void);



