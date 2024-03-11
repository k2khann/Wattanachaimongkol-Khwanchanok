//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
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
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(3)				// �e�N�X�`���̐�

#define TEXTURE_WIDTH_LOGO			(859 / 2)		// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(311 / 2)		// 

#define TEXTURE_WIDTH_CUR			(60)			// �I�����T�C�Y
#define TEXTURE_HEIGHT_CUR			(40)			// �I�����T�C�Y

#define ANGLE_MOVE					(360.0f / TEXTURE_HEIGHT_LOGO)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/Title02.png",
	"data/TEXTURE/Title_Logo01.png",
	"data/TEXTURE/chioce03.png",
};

static BOOL		g_Load = FALSE;		// �f�[�^�ǂݍ��݃t���O��OFF
static TITLE	g_Title;

static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

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
// ����������
//=============================================================================
HRESULT InitTitle(void)
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


	// �ϐ��̏�����
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

	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM_TITLE);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
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
		{		// Enter��������A�X�e�[�W��؂�ւ���
				// SetMode(MODE_GAME);
			SetFade(FADE_OUT, MODE_STORY);
			PlaySound(SOUND_LABEL_SE_PICK);
		}
		break;

	case TUTORIAL:
		g_TexNo = 2;
		g_Pos = XMFLOAT3(320.0f, 420.0f, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A))
		{		// Enter��������A�X�e�[�W��؂�ւ���
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

	






#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
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

	// �^�C�g���̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}



	//Choice
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, g_Pos.x + 15.0f, g_Pos.y, TEXTURE_WIDTH_CUR, TEXTURE_HEIGHT_CUR, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}



	for (int i = 0; i < TEXTURE_HEIGHT_LOGO; i++)
	{
		if (g_TitleLogo[i].use == TRUE)
		{
			{
				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TitleLogo[i].texNo]);

				//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = g_TitleLogo[i].pos.x - 200.0f;		// �v���C���[�̕\���ʒuX
				float py = g_TitleLogo[i].pos.y + 10.0f;		// �v���C���[�̕\���ʒuY
				float pw = g_TitleLogo[i].w;			// �v���C���[�̕\����
				float ph = g_TitleLogo[i].h;			// �v���C���[�̕\������
				// float pz = g_TitleLogo[i].rot.z;		// �v���C���[�̉�]

				float tw = 1.0f;							// �e�N�X�`���̕�
				float th = 1.0f / TEXTURE_HEIGHT_LOGO;		// �e�N�X�`���̍���
				float tx = 0.0f;							// �e�N�X�`���̍���X���W
				float ty = 1.0f / TEXTURE_HEIGHT_LOGO * i;	// �e�N�X�`���̍���Y���W

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f);

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);

			}
		}

	}

//	// �����Z�̃e�X�g
//	SetBlendState(BLEND_MODE_ADD);		// ���Z����
////	SetBlendState(BLEND_MODE_SUBTRACT);	// ���Z����
//	for(int i=0; i<30; i++)
//	{
//		// �e�N�X�`���ݒ�
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);
//
//		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
//		float dx = 100.0f;
//		float dy = 100.0f;
//		float sx = (float)(rand() % 100);
//		float sy = (float)(rand() % 100);
//
//
//		SetSpriteColor(g_VertexBuffer, dx+sx, dy+sy, 50, 50, 0.0f, 0.0f, 1.0f, 1.0f,
//			XMFLOAT4(0.3f, 0.3f, 1.0f, 0.5f));
//
//		// �|���S���`��
//		GetDeviceContext()->Draw(4, 0);
//	}
//	SetBlendState(BLEND_MODE_ALPHABLEND);	// ���������������ɖ߂�

}





