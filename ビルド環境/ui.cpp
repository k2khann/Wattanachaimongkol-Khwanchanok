//=============================================================================
//
// UI���� [ui.cpp]
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
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(5)				// �e�N�X�`���̐�

#define TEXTURE_WIDTH_LIFE			(40)			// �n�[�g�T�C�Y
#define TEXTURE_HEIGHT_LIFE			(40)			// 

#define LIFE_POS_X					(170)			// �n�[�g�� POS
#define LIFE_POS_Y					(60)

#define TEXTURE_WIDTH_ICON			(130)			// �v���C���[�̃A�C�R���T�C�Y
#define TEXTURE_HEIGHT_ICON			(130)			// 

#define ICON_POS_X					(20)			// �n�[�g�� POS
#define ICON_POS_Y					(10)

// �A�j���[�V�����p
#define TEXTURE_WIDTH_ANIM			(256 / 4)			// �I�����T�C�Y
#define TEXTURE_HEIGHT_ANIM			(256 / 4)			// �I�����T�C�Y

#define ANIM_POS_X					(90)				// �A�j���[�V�����̔z�u
#define ANIM_POS_Y					(78)

#define TEXTURE_PATTERN_DIVIDE_X	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(80)		// �A�j���[�V�����̐؂�ւ��Wait�l

// AIM point 
#define AIM_TEXTURE_WIDTH			(429 / 8)	// Score�̃��S�T�C�Y
#define AIM_TEXTURE_HEIGHT			(446 / 8)

#define AIM_POS_X					(420)	// Score�̃��S��POS
#define AIM_POS_Y					(250)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/Life03.png",			// �O���[
	"data/TEXTURE/Life02.png",			// ��

	"data/TEXTURE/IconBG01.png",		// �v���C���[�̃A�C�R��
	"data/TEXTURE/PenAnim03.png",		// �A�j���[�V�����p

	"data/TEXTURE/Aim01.png",			// Aim
};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

static BOOL						g_Load = FALSE;


// �A�j���[�V�����p
XMFLOAT3	ui_pos;						// �|���S���̍��W
XMFLOAT3	ui_rot;						// �|���S���̉�]��
XMFLOAT3	ui_scl;						// �|���S���̊g��k��
float		ui_AnimW, ui_AnimH;			// ���ƍ���
float		ui_countAnim;				// �A�j���[�V�����J�E���g
int			ui_patternAnim;				// �A�j���[�V�����p�^�[���i���o�[


//=============================================================================
// ����������
//=============================================================================
HRESULT InitUi(void)
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
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �A�j���[�V�����p
	ui_pos = XMFLOAT3(ANIM_POS_X, ANIM_POS_Y, 0.0f);		// �|���S���̍��W
	ui_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);					// �|���S���̉�]��
	ui_scl = XMFLOAT3(1.0f, 1.0f, 1.0f);					// �|���S���̊g��k��
	ui_AnimW = TEXTURE_WIDTH_ANIM;
	ui_AnimH = TEXTURE_HEIGHT_ANIM;
	ui_countAnim = 0;
	ui_patternAnim = 0;


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void UpdateUi(void)
{

	// �A�j���[�V����  
	ui_countAnim += 1.0f;
	if (ui_countAnim > ANIM_WAIT)
	{
		ui_countAnim = 0.0f;

		// �p�^�[���̐؂�ւ�
		ui_patternAnim = (ui_patternAnim + 1) % ANIM_PATTERN_NUM;
	}

#ifdef _DEBUG	// �f�o�b�O����\������
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawUi(void)
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

	// �n�[�g�炢�Ӂi���j��UI�`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[LIFE_BLACK]);

		for (int i = 0; i < PLAYER_LIFE; i++)
		{
			// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
			//�\���ʒu
			float pw = TEXTURE_WIDTH_LIFE;			// �X�R�A�̕\����
			float ph = TEXTURE_HEIGHT_LIFE;			// �X�R�A�̕\������
			float px = LIFE_POS_X + (i * 50);		// �X�R�A�̕\���ʒuX	��
			float py = LIFE_POS_Y;					// �X�R�A�̕\���ʒuY

			float tw = 1.0f;				// �e�N�X�`���̕�
			float th = 1.0f;				// �e�N�X�`���̍���
			float tx = 0.0f;				// �e�N�X�`���̍���X���W
			float ty = 0.0f;				// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

		}
	}

	// �n�[�g�炢�Ӂi�j��UI�`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[LIFE_BLUE]);
		
		PLAYER* player = GetPlayer();

		for (int j = 0; j < player[0].hp ; j++)
		{
			// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
			//�\���ʒu
			float pw = TEXTURE_WIDTH_LIFE;			// �X�R�A�̕\����
			float ph = TEXTURE_HEIGHT_LIFE;			// �X�R�A�̕\������
			float px = LIFE_POS_X + (j * 50);		// �X�R�A�̕\���ʒuX	��
			float py = LIFE_POS_Y;					// �X�R�A�̕\���ʒuY

			float tw = 1.0f;				// �e�N�X�`���̕�
			float th = 1.0f;				// �e�N�X�`���̍���
			float tx = 0.0f;				// �e�N�X�`���̍���X���W
			float ty = 0.0f;				// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

		}

	}





	//�v���C���[�̃A�C�R����UI�`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[PLAYER_ICON]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, ICON_POS_X, ICON_POS_Y, TEXTURE_WIDTH_ICON, TEXTURE_HEIGHT_ICON, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}






	// �A�j���[�V�����p
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[PLAYER_ICON_ANIM]);

		//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
		float uix = ui_pos.x;		// �v���C���[�̕\���ʒuX
		float uiy = ui_pos.y;		// �v���C���[�̕\���ʒuY
		float uiw = ui_AnimW;		// �v���C���[�̕\����
		float uih = ui_AnimH;		// �v���C���[�̕\������

		// �A�j���[�V�����p
		float uitw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
		float uith = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
		float uitx = (float)(ui_patternAnim % TEXTURE_PATTERN_DIVIDE_X) * uitw;	// �e�N�X�`���̍���X���W
		float uity = (float)(ui_patternAnim / TEXTURE_PATTERN_DIVIDE_X) * uith;	// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColorRotation(g_VertexBuffer, uix, uiy, uiw, uih, uitx, uity, uitw, uith,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			ui_rot.z);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


	// AIM�̃��S�̕`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[AIM_POINT]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, AIM_POS_X, AIM_POS_Y, AIM_TEXTURE_WIDTH, AIM_TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


}




