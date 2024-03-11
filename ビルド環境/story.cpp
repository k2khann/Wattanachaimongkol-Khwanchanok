//=============================================================================
//
// �^�C�g����ʏ��� [story.cpp]
// Author : ���b�^�i�`���C�����R���@�J���`���m
//
//=============================================================================
#include "story.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "light.h"
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(8)				// �e�N�X�`���̐�

#define TEXTURE_WIDTH_NEXT			(120)			// �I�����T�C�Y
#define TEXTURE_HEIGHT_NEXT			(60)			// �I�����T�C�Y

#define TEXTURE_WIDTH_SUB			(750)
#define TEXTURE_HEIGHT_SUB			(100)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/story01.jpg",
	"data/TEXTURE/story02.jpg",
	"data/TEXTURE/story03.jpg",

	"data/TEXTURE/sub01.png",
	"data/TEXTURE/sub02.png",
	"data/TEXTURE/sub03.png",

	"data/TEXTURE/enterkey.png",



};



static BOOL		g_Load = FALSE;		// �f�[�^�ǂݍ��݃t���O��OFF
static STORY	g_Story[TEXTURE_MAX];

static float	alpha;
static BOOL		flag_alpha;

 // �X�g�[���[�̃y�[�W
static int		g_TexNo;
static XMFLOAT3 g_Pos;
static int		g_subNo;

// �����p
float			sub_alpha;


static INTERPOLATION_DATA g_ScrollTbl0[] = {

	//���W								��]��							�g�嗦						����
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	500 },
	{ XMFLOAT3(0.0f, -100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(2.0f, 2.0f, 2.0f),	300 },

};


static INTERPOLATION_DATA g_ScrollTbl1[] = {

	//���W								��]��							�g�嗦						����
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	500 },
	{ XMFLOAT3(-100.0f, -100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(2.0f, 2.0f, 2.0f),	300 },

};

static INTERPOLATION_DATA g_ScrollTbl2[] = {

	//���W								��]��							�g�嗦						����
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
// ����������
//=============================================================================
HRESULT InitStory(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);

		// �ϐ��̏�����
		g_Story[i].w = TEXTURE_WIDTH;
		g_Story[i].h = TEXTURE_HEIGHT;
		g_Story[i].pos = XMFLOAT3(g_Story[i].w / 2, g_Story[i].h / 2, 0.0f);
		g_Story[i].texNo = 0;


		// //0�Ԃ������`��Ԃœ������Ă݂�
		//g_Story[i].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		//g_Story[i].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		//g_Story[i].tblMax = sizeof(g_ScrollTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


	}


	//0�Ԃ������`��Ԃœ������Ă݂�
	g_Story[0].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Story[0].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Story[0].tblMax = sizeof(g_ScrollTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	//1�Ԃ������`��Ԃœ������Ă݂�
	g_Story[1].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Story[1].tblNo = 1;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Story[1].tblMax = sizeof(g_ScrollTbl1) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


	//2�Ԃ������`��Ԃœ������Ă݂�
	g_Story[2].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Story[2].tblNo = 2;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Story[2].tblMax = sizeof(g_ScrollTbl2) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


	// ���`��ԗp
	g_TexNo = 0;
	g_subNo = (0 + SUBTITLE_MAX) % SUBTITLE_MAX + STORY_MAX;


	// ���_�o�b�t�@�����i�v���~�e�B�u�j
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// �{�^�� �p
	alpha = 1.0f;
	flag_alpha = TRUE;

	// �����p
	sub_alpha = 0.0f;

	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM_STORY);


	g_Load = TRUE;
	return S_OK;

}

//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void UpdateStory(void)
{
	// �X�g�[���[�̃y�[�W
	switch (g_TexNo)
	{
	case STORY_PAGE01:

		if ((GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A)))
		{
			g_TexNo = (++g_TexNo + STORY_MAX) % STORY_MAX;
			g_subNo = (++g_subNo + SUBTITLE_MAX) % SUBTITLE_MAX + STORY_MAX;
			sub_alpha = 0.0f;

			g_Story[1].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);				// �|���S���̍��W
			g_Story[1].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);				// ���f���̌���(��])
			g_Story[1].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);				// ���f���̑傫��(�X�P�[��)


		}
		break;


	case STORY_PAGE02:
		if ((GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A)))
		{
			g_TexNo = (++g_TexNo + STORY_MAX) % STORY_MAX;
			g_subNo = (++g_subNo + SUBTITLE_MAX) % SUBTITLE_MAX + STORY_MAX;
			sub_alpha = 0.0f;

			g_Story->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);				// �|���S���̍��W
			g_Story->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);				// ���f���̌���(��])
			g_Story->scl = XMFLOAT3(1.0f, 1.0f, 1.0f);				// ���f���̑傫��(�X�P�[��)

		}
		break;

	case STORY_PAGE03:
		if ((GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A)))
		{
			//if ((GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A)))
			//{
				// Enter�L�[����������A�Q�[������
				SetFade(FADE_OUT, MODE_GAME);
				PlaySound(SOUND_LABEL_SE_PICK);
			//}

		}
		break;




		//case ENTER_GAME:

		//	 �Q�[������
		//	if ((GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A)))
		//	{		// Enter��������A�X�e�[�W��؂�ւ���
		//			 SetMode(MODE_GAME);
		//		SetFade(FADE_OUT, MODE_GAME);
		//		PlaySound(SOUND_LABEL_SE_PICK);
		//	}
		//	break;
	}






	// Enter�L�[�p
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

	// �����p
	sub_alpha += 0.01;
	if (sub_alpha > 1.0f)
	{
		sub_alpha = 1.0f;
	}


	// // �L�[�{�[�h
	// if (GetKeyboardTrigger(DIK_RETURN))
	// {// Enter��������A�X�e�[�W��؂�ւ���
	// 	// SetMode(MODE_GAME);
	// 	SetFade(FADE_OUT, MODE_GAME);
	// }
	// // �Q�[���p�b�h�œ��͏���
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

		// �ړ�����
		if (g_Story[i].tblMax > 0)						// ���`��Ԃ����s����H
		{	// ���`��Ԃ̏���
			int nowNo = (int)g_Story[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
			int maxNo = g_Story[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
			int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
			INTERPOLATION_DATA* tbl = g_ScrollTblAdr[g_Story[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
			XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
			XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
			XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
			XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

			float nowTime = g_Story[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

			Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
			Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
			Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

			// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
			XMStoreFloat3(&g_Story[i].pos, nowPos + Pos);

			// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
			XMStoreFloat3(&g_Story[i].rot, nowRot + Rot);

			// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
			XMStoreFloat3(&g_Story[i].scl, nowScl + Scl);
			//g_Enemy[i].w = ENEMY_SIZE * g_Enemy[i].scl.x;
			//g_Enemy[i].h = TEXTURE_HEIGHT * g_Enemy[i].scl.y;

			// frame���g�Ď��Ԍo�ߏ���������
			g_Story[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���

			//if ((int)g_Story[i].time >= (maxNo - 1))
			//{
			//	g_Story[i].tblMax = -1;					// Loop ���Ȃ��悤�Ɂ@****
			//}

			if ((int)g_Story[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
			{
				g_Story[i].time -= maxNo;					// �O�ԖڂɃ��Z�b�g�������������������p���ł���]

			}


		}

	}






#ifdef _DEBUG	// �f�o�b�O����\������
	//PrintDebugProc("Player:�� �� �� ���@Space\n");

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawStory(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);




	// �X�g�[���[�̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		float pw = TEXTURE_WIDTH * g_Story[g_TexNo].scl.x;
		float ph = TEXTURE_HEIGHT * g_Story[g_TexNo].scl.y;

		float px = SCREEN_CENTER_X + g_Story[g_TexNo].pos.x;
		float py = SCREEN_CENTER_Y + g_Story[g_TexNo].pos.y;

		SetSprite(g_VertexBuffer, px, py, pw, ph, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
	





	// �{�^���̃��S��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[6]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		// SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
		SetSpriteColor(g_VertexBuffer, 870.0f, 50.0f, TEXTURE_WIDTH_NEXT, TEXTURE_HEIGHT_NEXT, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}



	 // �����iStory�j��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_subNo]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		// SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);
		SetSpriteColor(g_VertexBuffer, 500.0f, 450.0f, TEXTURE_WIDTH_SUB, TEXTURE_HEIGHT_SUB, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, sub_alpha));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}



	//}
/*
	// �����Z�̃e�X�g
	SetBlendState(BLEND_MODE_ADD);		// ���Z����
	//SetBlendState(BLEND_MODE_SUBTRACT);	// ���Z����
	for (int i = 0; i < 100; i++)
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		float dx = 150.0f;
		float dy = 150.0f;
		float angle = rand() % 360;
		float speed = rand() % 50;
		float sx = cosf(angle) * speed;
		float sy = sinf(angle) * speed;

		SetSpriteColor(g_VertexBuffer, dx + sx, dy + sy, 50, 50, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 0.1f, 0.0f, 0.5f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
	SetBlendState(BLEND_MODE_ALPHABLEND);	// ���������������ɖ߂�
*/
}


