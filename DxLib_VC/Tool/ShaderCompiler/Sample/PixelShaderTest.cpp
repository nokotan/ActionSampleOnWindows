#include "DxLib.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	int grhandle, pshandle ;
	VERTEX2DSHADER Vert[ 6 ] ;

	// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode( TRUE );

	// �c�w���C�u�����̏�����
	if( DxLib_Init() < 0 ) return -1;

	// �摜�̓ǂݍ���
	grhandle = LoadGraph( "Test1.bmp" );

	// �s�N�Z���V�F�[�_�[�o�C�i���R�[�h�̓ǂݍ���
	pshandle = LoadPixelShader( "SamplePS.pso" ) ;

	// ���_�f�[�^�̏���
	Vert[ 0 ].pos = VGet(   0.0f,   0.0f, 0.0f ) ;
	Vert[ 1 ].pos = VGet( 128.0f,   0.0f, 0.0f ) ;
	Vert[ 2 ].pos = VGet(   0.0f, 512.0f, 0.0f ) ;
	Vert[ 3 ].pos = VGet( 128.0f, 512.0f, 0.0f ) ;
	Vert[ 0 ].dif = GetColorU8( 255,255,255,255 ) ;
	Vert[ 0 ].spc = GetColorU8( 0,0,0,0 ) ;
	Vert[ 0 ].u = 0.0f ; Vert[ 0 ].v = 0.0f ;
	Vert[ 1 ].u = 1.0f ; Vert[ 1 ].v = 0.0f ;
	Vert[ 2 ].u = 0.0f ; Vert[ 2 ].v = 1.0f ;
	Vert[ 3 ].u = 1.0f ; Vert[ 3 ].v = 1.0f ;
	Vert[ 0 ].su = 0.0f ; Vert[ 0 ].sv = 0.0f ;
	Vert[ 1 ].su = 1.0f ; Vert[ 1 ].sv = 0.0f ;
	Vert[ 2 ].su = 0.0f ; Vert[ 2 ].sv = 1.0f ;
	Vert[ 3 ].su = 1.0f ; Vert[ 3 ].sv = 1.0f ;
	Vert[ 0 ].rhw = 1.0f ;
	Vert[ 1 ].rhw = 1.0f ;
	Vert[ 2 ].rhw = 1.0f ;
	Vert[ 3 ].rhw = 1.0f ;
	Vert[ 4 ] = Vert[ 2 ] ;
	Vert[ 5 ] = Vert[ 1 ] ;

	// �g�p����e�N�X�`�����Z�b�g
	SetUseTextureToShader( 0, grhandle ) ;

	// �g�p����s�N�Z���V�F�[�_�[���Z�b�g
	SetUsePixelShader( pshandle ) ;

	// �`��
	DrawPrimitive2DToShader( Vert, 6, DX_PRIMTYPE_TRIANGLELIST ) ;

	// �L�[���͑҂�
	WaitKey() ;

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}
