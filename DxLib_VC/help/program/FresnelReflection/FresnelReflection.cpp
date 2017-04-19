// �t���l�����ˊ�{

#include "DxLib.h"
#include <math.h>


// ��ʂ̉𑜓x
#define SCREEN_W			(640)
#define SCREEN_H			(480)

// ���ɉf��f���̎擾�Ɏg�p����X�N���[���̉𑜓x
#define MIRROR_SCREEN_W		(1024)
#define MIRROR_SCREEN_H		(512)

// ���̐�
#define MIRROR_NUM			(1)

// �J�����̑��x
#define CAMERA_SPEED		(32.0f)


// �L�����N�^�[���f���ƃX�e�[�W���f��
int CharaModel ;		
int StageModel ;		

// ���������s�����_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[
int FresnelReflectionVS ;
int FresnelReflectionPS ;

// ���ɉf��f���̎擾�Ɏg�p����X�N���[��
int MirrorHandle[ MIRROR_NUM ] ;

// ���ɉf��f���̎擾�Ɏg�p����N���[���̒��̋��̎l���̍��W( �������W )
FLOAT4 MirrorScreenPosW[ MIRROR_NUM ][ 4 ] ;

// ���̃��[���h���W
VECTOR MirrorWorldPos[ MIRROR_NUM ][ 4 ] =
{
	{
		{ -4000.0f, 400.0f,  4500.0f },
		{  4000.0f, 400.0f,  4500.0f },
		{ -4000.0f, 400.0f, -4500.0f },
		{  4000.0f, 400.0f, -4500.0f },
	}
} ;

// ���� Diffuse Color
COLOR_U8 MirrorDiffuseColor[ MIRROR_NUM ] =
{
	{ 255, 255,   0, 255 },
} ;


// ���̏�����
void Mirror_Initialize( void ) ;

// ���ɉf��f����`�悷�邽�߂̃J�����̐ݒ���s��
void Mirror_SetupCamera( int MirrorNo, VECTOR CameraEyePos, VECTOR CameraTargetPos ) ;

// ���̕`��
void Mirror_Render( int MirrorNo ) ;

// �R�c��Ԃ̕`��
void World_RenderProcess() ;


// ���̏�����
void Mirror_Initialize( void )
{
	int i ;

	// �t���l�����ˏ������s�����߂̒��_�V�F�[�_�[�ƃs�N�Z���V�F���[��ǂݍ���
	FresnelReflectionVS = LoadVertexShader( "FresnelReflectionVS.vso" ) ;
	FresnelReflectionPS = LoadPixelShader( "FresnelReflectionPS.pso" ) ;

	// ���ɉf��f���̎擾�Ɏg�p����X�N���[���̍쐬
	for( i = 0 ; i < MIRROR_NUM ; i ++ )
	{
		MirrorHandle[ i ] = MakeScreen( MIRROR_SCREEN_W, MIRROR_SCREEN_H, FALSE ) ;
	}
}

// ���ɉf��f����`�悷�邽�߂̃J�����̐ݒ���s��
void Mirror_SetupCamera( int MirrorNo, VECTOR CameraEyePos, VECTOR CameraTargetPos )
{
	int i ;
	float EyeLength, TargetLength ;
	VECTOR MirrorNormal ;
	VECTOR *MirrorWorldPosP ;
	VECTOR MirrorCameraEyePos, MirrorCameraTargetPos ;

	MirrorWorldPosP = MirrorWorldPos[ MirrorNo ] ;

	// ���̖ʂ̖@�����Z�o
	MirrorNormal = VNorm( VCross( VSub( MirrorWorldPosP[ 1 ], MirrorWorldPosP[ 0 ] ), VSub( MirrorWorldPosP[ 2 ], MirrorWorldPosP[ 0 ] ) ) ) ;

	// ���̖ʂ���J�����̍��W�܂ł̍ŒZ�����A���̖ʂ���J�����̒����_�܂ł̍ŒZ�������Z�o
	EyeLength    = Plane_Point_MinLength( MirrorWorldPosP[ 0 ], MirrorNormal, CameraEyePos    ) ;
	TargetLength = Plane_Point_MinLength( MirrorWorldPosP[ 0 ], MirrorNormal, CameraTargetPos ) ;

	// ���ɉf��f����`�悷��ۂɎg�p����J�����̍��W�ƃJ�����̒����_���Z�o
	MirrorCameraEyePos    = VAdd( CameraEyePos,    VScale( MirrorNormal, -EyeLength    * 2.0f ) ) ;
	MirrorCameraTargetPos = VAdd( CameraTargetPos, VScale( MirrorNormal, -TargetLength * 2.0f ) ) ;

	// �v�Z�œ���ꂽ�J�����̍��W�ƃJ�����̒����_�̍��W���J�����̐ݒ肷��
	SetCameraPositionAndTarget_UpVecY( MirrorCameraEyePos, MirrorCameraTargetPos ) ;

	// ���ɉf��f���̒��ł̋��̎l���̍��W���Z�o( �������W )
	for( i = 0 ; i < 4 ; i ++ )
	{
		MirrorScreenPosW[ MirrorNo ][ i ] = ConvWorldPosToScreenPosPlusW( MirrorWorldPosP[ i ] ) ;
	}
}

// ���̕`��
void Mirror_Render( int MirrorNo )
{
	int i ;
	VERTEX3DSHADER Vert[ 6 ] ;
	VECTOR MirrorNormal ;

	// ���̖ʂ̖@�����Z�o
	MirrorNormal = VNorm( VCross( VSub( MirrorWorldPos[ MirrorNo ][ 1 ], MirrorWorldPos[ MirrorNo ][ 0 ] ), VSub( MirrorWorldPos[ MirrorNo ][ 2 ], MirrorWorldPos[ MirrorNo ][ 0 ] ) ) ) ;

	// ���̕`��Ɏg�p���钸�_�̃Z�b�g�A�b�v
	for( i = 0 ; i < 4 ; i ++ )
	{
		// ���W���Z�b�g
		Vert[ i ].pos = MirrorWorldPos[ MirrorNo ][ i ] ;

		// �@����Z�b�g
		Vert[ i ].norm = MirrorNormal ;

		// Diffuse Color ���Z�b�g
		Vert[ i ].dif = MirrorDiffuseColor[ MirrorNo ] ;

		// �⏕���W�ɋ��ɉf�鎋�_�ł̎ˉe���W��������
		Vert[ i ].spos.x = MirrorScreenPosW[ MirrorNo ][ i ].x / MIRROR_SCREEN_W ;
		Vert[ i ].spos.y = MirrorScreenPosW[ MirrorNo ][ i ].y / MIRROR_SCREEN_H ;
		Vert[ i ].spos.z = MirrorScreenPosW[ MirrorNo ][ i ].z ;
		Vert[ i ].spos.w = MirrorScreenPosW[ MirrorNo ][ i ].w ;
	}
	Vert[ 4 ] = Vert[ 2 ] ;
	Vert[ 5 ] = Vert[ 1 ] ;

	// �`�惂�[�h���o�C���j�A�t�B���^�����O�ɐݒ�
	SetDrawMode( DX_DRAWMODE_BILINEAR ) ;

	// �`��u�����h���[�h���Z�b�g
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 ) ;

	// �`��ɂy�o�b�t�@���g�p����
	SetUseZBuffer3D( TRUE ) ;

	// �y�o�b�t�@�ɏ������݂��s��
	SetWriteZBuffer3D( TRUE ) ;

	// �g�p���钸�_�V�F�[�_�[���Z�b�g
	SetUseVertexShader( FresnelReflectionVS ) ;

	// �g�p����s�N�Z���V�F�[�_�[���Z�b�g
	SetUsePixelShader( FresnelReflectionPS ) ;

	// �g�p����e�N�X�`�����Z�b�g
	SetUseTextureToShader( 0, MirrorHandle[ MirrorNo ] ) ;

	// �t���l�����˂̋���`��
	DrawPolygon3DToShader( Vert, 2 ) ;

	// �y�o�b�t�@�ɏ������݂��s���ݒ�����ɖ߂�
	SetWriteZBuffer3D( FALSE ) ;

	// �y�o�b�t�@���g�p����ݒ�����ɖ߂�
	SetUseZBuffer3D( FALSE ) ;

	// �`��u�����h���[�h�����ɖ߂�
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 ) ;

	// �`�惂�[�h�����ɖ߂�
	SetDrawMode( DX_DRAWMODE_NEAREST ) ;
}

// �R�c��Ԃ̕`��
void World_RenderProcess()
{
	// �X�e�[�W���f���̕`��
	MV1DrawModel( StageModel ) ;

	// �L�����N�^�[���f���̕`��
	MV1DrawModel( CharaModel ) ;
}


// WinMain�֐�
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	int CameraAngle ;		// �J�����̐����p�x
	VECTOR CameraEyePosition ;	// �J�����̍��W
	VECTOR CameraDirection ;	// �J�����̌����Ă������
	VECTOR CameraTargetPosition ;	// �J�����̒����_
	int i ;

	// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode( TRUE );

	// ��ʂ̉𑜓x�ƐF�r�b�g�[�x��ݒ�
	SetGraphMode( SCREEN_W, SCREEN_H, 32 ) ;

	// �c�w���C�u�����̏�����
	if( DxLib_Init() < 0 )
		return -1 ;

	// �V�F�[�_�[���f���Q�D�O���g�p�ł��邩�ǂ������`�F�b�N
	if( GetValidShaderVersion() < 200 )
	{
		DrawString( 0, 0, "�V�F�[�_�[���f���Q�D�O���g�p�ł��܂���", GetColor( 255,255,255 ) ) ;
		DxLib_End() ;
		return 0 ;
	}

	// �������̏�����
	Mirror_Initialize() ;

	// �L�����N�^�[���f���̓ǂݍ���
	CharaModel = MV1LoadModel( "DxChara.x" ) ;

	// �X�e�[�W���f���̓ǂݍ���
	StageModel = MV1LoadModel( "ColTestStage.mqo" ) ;

	// �L�����N�^�[���f���̐ݒu
	MV1SetPosition( CharaModel, VGet( 0.0f, 0.0f, -3500.0f ) ) ;
	MV1SetRotationXYZ( CharaModel, VGet( 0.0f, 3.14159f / 2.0f, 0.0f ) ) ;
	MV1AttachAnim( CharaModel, 7 ) ;

	// �J�����̊p�x��������
	CameraAngle = 325 ;

	// �J�����̍��W���Z�b�g
	CameraEyePosition = VGet( -2605.0f, 670.0f, -2561.0f ) ;

	// ���C�g�̌������Z�b�g
	SetLightDirection( VGet( 1.0f, -1.0f, -1.0f ) ) ;

	// ���C�����[�v(�����L�[�������ꂽ�烋�[�v�𔲂���)
	while( ProcessMessage() == 0 )
	{
		// ���E�L�[�������ꂽ��J��������]����
		if( CheckHitKey( KEY_INPUT_LEFT  ) )
		{
			CameraAngle += 3 ;
			if( CameraAngle > 360 )
			{
				CameraAngle -= 360 ;
			}
		}
		if( CheckHitKey( KEY_INPUT_RIGHT ) )
		{
			CameraAngle -= 3 ;
			if( CameraAngle < 0 )
			{
				CameraAngle += 360 ;
			}
		}

		// �J�����̌����Ă���������Z�o
		CameraDirection.x = cos( CameraAngle * 3.14159f / 180.0f ) ;
		CameraDirection.y = 0.0f ;
		CameraDirection.z = sin( CameraAngle * 3.14159f / 180.0f ) ;

		// �㉺�L�[�������ꂽ��J������O�i�E��i������
		if( CheckHitKey( KEY_INPUT_UP ) )
		{
			CameraEyePosition = VAdd( CameraEyePosition, VScale( CameraDirection, CAMERA_SPEED ) ) ;
		}
		if( CheckHitKey( KEY_INPUT_DOWN ) )
		{
			CameraEyePosition = VSub( CameraEyePosition, VScale( CameraDirection, CAMERA_SPEED ) ) ;
		}

		// �J�����̒����_���W���Z�o
		CameraTargetPosition = VAdd( CameraEyePosition, CameraDirection ) ;


		// ���ɉf��f����`��
		for( i = 0 ; i < MIRROR_NUM ; i ++ )
		{
			// �`�������ɉf��f���̎擾�Ɏg�p����摜��`�悷��X�N���[���ɂ���
			SetDrawScreen( MirrorHandle[ i ] ) ;

			// ��ʂ�����������
			ClearDrawScreen() ;

			// ���ɉf��f����`�悷��ۂɎg�p����J�����̐ݒ���s��
			Mirror_SetupCamera( i, CameraEyePosition, CameraTargetPosition ) ;

			// �R�c��Ԃ̕`��
			World_RenderProcess() ;
		}


		// ����ʂ�`��Ώۂɂ���
		SetDrawScreen( DX_SCREEN_BACK ) ;

		// ��ʂ��N���A
		ClearDrawScreen() ;

		// �J�����̐ݒ���s��
		SetCameraPositionAndTarget_UpVecY( CameraEyePosition, CameraTargetPosition ) ;

		// �R�c��Ԃ̕`��
		World_RenderProcess() ;

		// ���̕`��
		for( i = 0 ; i < MIRROR_NUM ; i ++ )
		{
			Mirror_Render( i ) ;
		}


		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();
	}

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}
