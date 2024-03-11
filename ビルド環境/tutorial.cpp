//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : ���b�^�i�`���C�����R���@�J���`���m
//
//=============================================================================
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(6)				// �e�N�X�`���̐�

#define TEXTURE_WIDTH_LOGO			(859 / 2)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(311 / 2)			// 

#define TEXTURE_WIDTH_CUR			(60)			// �I�����T�C�Y
#define TEXTURE_HEIGHT_CUR			(40)			// �I�����T�C�Y

// �A�j���[�V�����p	
#define TEXTURE_WIDTH_ANIM			(500 / 4)			// �I�����T�C�Y
#define TEXTURE_HEIGHT_ANIM			(400 / 4)			// �I�����T�C�Y

#define ANIM_POS_X1					(150)				// �A�j���[�V�����̔z�u
#define ANIM_POS_Y1					(230)

//////////////////////
#define TEXTURE_WIDTH_ANIM2			(500 / 4)			// �I�����T�C�Y
#define TEXTURE_HEIGHT_ANIM2		(150 / 4)			// �I�����T�C�Y

#define ANIM_POS_X2					(450)				// �A�j���[�V�����̔z�u
#define ANIM_POS_Y2					(230)

//////////////////////
#define TEXTURE_WIDTH_ANIM3			(775 / 4)			// �I�����T�C�Y
#define TEXTURE_HEIGHT_ANIM3		(98 / 4)			// �I�����T�C�Y

#define ANIM_POS_X3					(800)				// �A�j���[�V�����̔z�u
#define ANIM_POS_Y3					(230)

//////////////////////
#define TEXTURE_PATTERN_DIVIDE_X	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(30)		// �A�j���[�V�����̐؂�ւ��Wait�l


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/Tutorial01.png",
	"data/TEXTURE/effect000.jpg",
	"data/TEXTURE/chioce03.png",
	"data/TEXTURE/Key01.png",
	"data/TEXTURE/Key02.png",
	"data/TEXTURE/Key03.png",
};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

static BOOL						g_Load = FALSE;

static float	effect_dx;
static float	effect_dy;

float	tutorial_alpha;
BOOL	tutorial_flag_alpha;

// Choice
int				cur1;

// �A�j���[�V�����p
XMFLOAT3	pos;			// �|���S���̍��W
XMFLOAT3	rot;			// �|���S���̉�]��
XMFLOAT3	scl;			// �|���S���̊g��k��
float		AnimW, AnimH;			// ���ƍ���
float		countAnim;		// �A�j���[�V�����J�E���g
int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[


//=============================================================================
// ����������
//=============================================================================
HRESULT InitTutorial(void)
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

	tutorial_alpha = 1.0f;
	tutorial_flag_alpha = TRUE;

	effect_dx = 100.0f;
	effect_dy = 100.0f;


	// �A�j���[�V�����p
	pos = XMFLOAT3(0.0f, 0.0f, 0.0f);			// �|���S���̍��W
	rot = XMFLOAT3(0.0f, 0.0f, 0.0f);						// �|���S���̉�]��
	scl = XMFLOAT3(1.0f, 1.0f, 1.0f);						// �|���S���̊g��k��
	AnimW = TEXTURE_WIDTH_ANIM;
	AnimH = TEXTURE_HEIGHT_ANIM;
	countAnim = 0;
	patternAnim = 0;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
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
		{		// Enter��������A�X�e�[�W��؂�ւ���
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



	// �A�j���[�V����  
	countAnim += 1.0f;
	if (countAnim > ANIM_WAIT)
	{
		countAnim = 0.0f;
		// �p�^�[���̐؂�ւ�
		patternAnim = (patternAnim + 1) % ANIM_PATTERN_NUM;
	}

#ifdef _DEBUG	// �f�o�b�O����\������
	//PrintDebugProc("Player:�� �� �� ���@Space\n");

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTutorial(void)
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


	//// �����Z�̃e�X�g
	//SetBlendState(BLEND_MODE_ADD);			// ���Z����
	//// SetBlendState(BLEND_MODE_SUBTRACT);	// ���Z����
	//	
	//// �e�N�X�`���ݒ�
	//GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);
	//

	//SetBlendState(BLEND_MODE_ALPHABLEND);		// ���������������ɖ߂�
	




		//Choice
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, g_Pos.x - 20.0f, g_Pos.y, TEXTURE_WIDTH_CUR, TEXTURE_HEIGHT_CUR, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}






}





