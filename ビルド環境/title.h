//=============================================================================
//
// タイトル画面処理 [title.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct TITLE
{
	XMFLOAT3	pos;		// ポリゴンの座標
	float		w, h;		// 幅と高さ
	int			texNo;		// 使用しているテクスチャ番号
};

struct TITLE_LOGO
{
	XMFLOAT3	pos;		// ポリゴンの座標
	BOOL		use;		//
	float		w, h;		// 幅と高さ
	int			texNo;		// 使用しているテクスチャ番号

	float	countAnim;
	int		patternAnim;
};

// Choice
enum
{
	START,
	TUTORIAL,
	EXIT,
	TITLE_MAX
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);


