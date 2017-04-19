// ���̐[���ɂ���ē����x�𒲐�����V�F�[�_�[

#include "DxLib.h"
#include <math.h>


// ��ʂ̉𑜓x
#define SCREEN_W			(640)
#define SCREEN_H			(480)

// �[�x�̎擾�Ɏg�p����X�N���[���̉𑜓x
#define DEPTH_SCREEN_W		(1024)
#define DEPTH_SCREEN_H		(512)

// �J�����̑��x
#define CAMERA_SPEED		(32.0f)


// �L�����N�^�[���f���ƃX�e�[�W���f��
int CharaModel ;		
int StageModel ;		

// �[�x�̕`��Ɏg�p����V�F�[�_�[
int Depth_NormalMeshVS ;
int Depth_SkinMeshVS ;
int DepthPS ;

// �[�x�����Đ��ʂ�`�悷��V�F�[�_�[
int VolumeWaterVS ;
int VolumeWaterPS ;

// �[�x���擾����X�N���[��
int DepthScreen ;

// ���ʂ̃��[���h���W
VECTOR WaterWorldPos[ 4 ] =
{
	{ -8000.0f, 400.0f,  8000.0f },
	{  8000.0f, 400.0f,  8000.0f },
	{ -8000.0f, 400.0f, -8000.0f },
	{  8000.0f, 400.0f, -8000.0f },
} ;

// ���ʂ� Diffuse Color
COLOR_U8 MirrorDiffuseColor =
{
	255, 255,   0, 255,
} ;

// �R�c��Ԃ̕`��
void World_RenderProcess( int DepthDraw )
{
	// �[�x�`��̏ꍇ�͐[�x�`��p�̒��_�V�F�[�_�[���Z�b�g����
	if( DepthDraw )
	{
		MV1SetUseOrigShader( TRUE ) ;

		SetUsePixelShader( DepthPS ) ;
		SetUseTextureToShader( 0, -1 ) ;
	}
	else
	{
		MV1SetUseOrigShader( FALSE ) ;
	}

	// �X�e�[�W���f���̕`��
	if( DepthDraw ) 
	{
		SetUseVertexShader( Depth_NormalMeshVS ) ;
	}
	MV1DrawModel( StageModel ) ;

	// �L�����N�^�[���f���̕`��
	if( DepthDraw ) 
	{
		SetUseVertexShader( Depth_SkinMeshVS ) ;
	}
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

	// �[�x��`�悷�邽�߂̃V�F�[�_�[��ǂݍ���
	Depth_NormalMeshVS = LoadVertexShader( "Depth_NormalMeshVS.vso" ) ;
	Depth_SkinMeshVS = LoadVertexShader( "Depth_SkinMeshVS.vso" ) ;
	DepthPS = LoadPixelShader( "DepthPS.pso" ) ;

	// �[�x��`�悷��e�N�X�`���̍쐬( �P�`�����l�����������_�P�U�r�b�g�e�N�X�`�� )
	SetCreateDrawValidGraphChannelNum( 1 ) ;
	SetDrawValidFloatTypeGraphCreateFlag( TRUE ) ;
	SetCreateGraphChannelBitDepth( 16 ) ;
	DepthScreen = MakeScreen( DEPTH_SCREEN_W, DEPTH_SCREEN_H, FALSE ) ;
	SetCreateDrawValidGraphChannelNum( 0 ) ;
	SetDrawValidFloatTypeGraphCreateFlag( FALSE ) ;
	SetCreateGraphChannelBitDepth( 0 ) ;

	// �[�x�����Đ��ʂ�`�悷��V�F�[�_�[�̓ǂݍ���
	VolumeWaterVS = LoadVertexShader( "VolumeWaterVS.vso" ) ;
	VolumeWaterPS = LoadPixelShader( "VolumeWaterPS.pso" ) ;

	// �L�����N�^�[���f���̓ǂݍ���
	CharaModel = MV1LoadModel( "DxChara.x" ) ;

	// �X�e�[�W���f���̓ǂݍ���
	StageModel = MV1LoadModel( "Stage.mqo" ) ;

	// �X�e�[�W���f���̔z�u
	MV1SetPosition( StageModel, VGet( 0.0f, 800.0f, 0.0f ) ) ;

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

		// �����������ɂ���
		CameraTargetPosition.y -= 0.3f ;
		
		// �[�x��`��
		{
			// �`����[�x�X�N���[���ɕύX
			SetDrawScreen( DepthScreen ) ;

			// ��ʂ�����������
			ClearDrawScreen() ;

			// �`��͈͂���ʃT�C�Y�Ɍ���
			SetDrawArea( 0, 0, SCREEN_W, SCREEN_H ) ;

			// �����_����ʃT�C�Y�̒��S�ɕύX
			SetCameraScreenCenter( SCREEN_W / 2.0f, SCREEN_H / 2.0f ) ;

			// �\����ʂɍ����悤�ɃX�P�[�����O
			SetDraw3DScale( ( float )SCREEN_H / DEPTH_SCREEN_H ) ;

			// �J�����̐ݒ���s��
			SetCameraPositionAndTarget_UpVecY( CameraEyePosition, CameraTargetPosition ) ;

			// ���f���̕`��
			World_RenderProcess( TRUE ) ;

			// �X�P�[�����O�ݒ�����ɖ߂�
			SetDraw3DScale( 1.0f ) ;
		}
		
		// �ʏ탂�f���̕`��
		{
			// �`���𗠉�ʂɕύX
			SetDrawScreen( DX_SCREEN_BACK ) ;

			// �J�����̐ݒ���s��
			SetCameraPositionAndTarget_UpVecY( CameraEyePosition, CameraTargetPosition ) ;

			// ��ʂ�����������
			ClearDrawScreen() ;

			// ���f���̕`��
			World_RenderProcess( FALSE ) ;
		}
		
		// ���ʂ̕`��
		{
			VERTEX3DSHADER Vertex[ 6 ] ;
			FLOAT4 Param ;

			// ���_�f�[�^�̏���
			for( i = 0 ; i < 4 ; i ++ )
			{
				Vertex[ i ].pos = WaterWorldPos[ i ] ;
				Vertex[ i ].dif = MirrorDiffuseColor ;
			}
			Vertex[ 4 ] = Vertex[ 2 ] ;
			Vertex[ 5 ] = Vertex[ 1 ] ;

			// �g�p����V�F�[�_�[�̃Z�b�g
			SetUseVertexShader( VolumeWaterVS ) ;
			SetUsePixelShader( VolumeWaterPS ) ;

			// �g�p����e�N�X�`���Ƃ��Đ[�x�X�N���[�����Z�b�g
			SetUseTextureToShader( 0, DepthScreen ) ;

			// �s�����x���ő�ɂȂ鋗�����Z�b�g
			Param.x = 6000.0f ;
			Param.y = 0.0f ;
			Param.z = 0.0f ;
			Param.w = 0.0f ;
			SetPSConstF( 0, Param ) ;

			// �Œ�s�����x���Z�b�g
			Param.x = 0.1f ;
			SetPSConstF( 1, Param ) ;

			// �[�x�e�N�X�`���̎g�p���Ă���T�C�Y���Z�b�g
			Param.x = ( float )SCREEN_W / DEPTH_SCREEN_W ;
			Param.y = ( float )SCREEN_H / DEPTH_SCREEN_H ;
			SetPSConstF( 2, Param ) ;

			// �`��u�����h���[�h���A���t�@�u�����h�ɃZ�b�g
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 ) ;

			// �y�o�b�t�@���g�p����
			SetUseZBufferFlag( TRUE ) ;

			// �y�o�b�t�@�ɏ������݂��s��
			SetWriteZBufferFlag( TRUE ) ;

			// �|���S���̕`��
			DrawPolygon3DToShader( Vertex, 2 ) ;

			// �`��u�����h���[�h��߂�
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 ) ;
		}
	
		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();
	}

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}
