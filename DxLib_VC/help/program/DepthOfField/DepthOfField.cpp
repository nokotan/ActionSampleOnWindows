#include "DxLib.h"
#include <math.h>


// �L�����N�^�[�̂w���W
#define CHARAMODEL_X		(-3500.0f)

// �L�����N�^�[�̂P�l�ڂ̂y���W
#define CHARAMODEL_Z		(-3500.0f)

// �`�悷��L�����N�^�[�̐�
#define CHARAMODEL_NUM		(8)

// �L�����N�^�[�P�l������̃X�y�[�X
#define CHARAMODEL_SPACE	(800.0f)


// �J�����̑��x
#define CAMERA_SPEED		(32.0f)


// ��ʂ̉��𑜓x
#define SCREEN_W			(640)

// ��ʂ̏c�𑜓x
#define SCREEN_H			(480)

// �`��p�e�N�X�`���̉��𑜓x
#define SCREEN_TEX_W		(1024)

// �`��p�e�N�X�`���̏c�𑜓x
#define SCREEN_TEX_H		(512)


// �X�e�[�W���f���ƃL�����N�^�[���f���̃n���h��
int StageModelHandle ;
int CharacterModelHandle ;


// ���̃��b�V���p�̐[�x�l�`��p���_�V�F�[�_�[�n���h��
int Depth_NormalMeshVS ;

// �X�L�j���O���b�V���p�̐[�x�l�`��p���_�V�F�[�_�[�n���h��
int Depth_SkinMeshVS ;

// �[�x�l�`��p�s�N�Z���V�F�[�_�[�n���h��
int DepthPS ;

// ��ʊE�[�x�����p�̃s�N�Z���V�F�[�_�[�n���h��
int DepthOfFieldPS ;


// �ł��ڂ����������Ƃ���Ƃڂ��������̂Ƃ���̕�ԋ���
float DepthOfField_InterpSize ;

// �ڂ��������͈̔͂̒��S�ʒu
float DepthOfField_Focus ;

// �ڂ��������͈̔�
float DepthOfField_FocusSize ;

// ��Ԕ͈͂Ƃڂ��������͈̔͂̍��v����
float DepthOfField_TotalSize ;


// �[�x�l�`��p�e�N�X�`��
int DepthMapTexture ;

// 0:�ʏ�`��p�e�N�X�`��
// 1:�ʏ�`��Ɏア�ڂ������������摜�ۑ��p�̃e�N�X�`��
// 2:�ʏ�`��ɋ����ڂ������������摜�ۑ��p�̃e�N�X�`��
int ColorMapTexture[ 3 ] ;


int CameraAngle ;		// �J�����̐����p�x
VECTOR CameraEyePosition ;	// �J�����̍��W
VECTOR CameraDirection ;	// �J�����̌����Ă������
VECTOR CameraTargetPosition ;	// �J�����̒����_


// �w��̉�ʂɂR�c���f����`��
void DrawScreen( int ScreenGraphHandle, int UseDepthShader )
{
	int i ;
	FLOAT4 ParamF ;

	// �`��Ώۂ�ύX
	SetDrawScreen( ScreenGraphHandle ) ;

	// �`��͈͂�ݒ�
	SetDrawArea( 0, 0, SCREEN_W, SCREEN_H ) ;

	// �����_���W��ݒ�
	SetCameraScreenCenter( SCREEN_W / 2.0f, SCREEN_H / 2.0f ) ;

	// �\����ʂɍ����悤�ɃX�P�[�����O
	SetDraw3DScale( ( float )SCREEN_H / SCREEN_TEX_H ) ;

	// ��ʂ�������
	ClearDrawScreen() ;

	// �J�����̐ݒ���s��
	SetCameraPositionAndTarget_UpVecY( CameraEyePosition, CameraTargetPosition ) ;

	// �[�x�l��`�悷�邩�ǂ����ŏ����𕪊�
	if( UseDepthShader != 0 )
	{
		// �[�x�l��`�悷��ꍇ�̓I���W�i���̃V�F�[�_�[���g�p���ă��f����`�悷��ݒ�ɂ���
		MV1SetUseOrigShader( TRUE ) ;

		// �[�x�l��`�悷�邽�߂̃s�N�Z���V�F�[�_�[���Z�b�g
		SetUsePixelShader( DepthPS ) ;

		// ��ʊE�[�x�̊J�n�ʒu���v�Z
		ParamF.x = DepthOfField_Focus - DepthOfField_FocusSize / 2.0f - DepthOfField_InterpSize ;

		// ��ʊE�[�x�̏I���ʒu���v�Z
		ParamF.y = DepthOfField_Focus + DepthOfField_FocusSize / 2.0f + DepthOfField_InterpSize ;

		// ��ʊE�[�x�͈̔͂̋t�����v�Z
		ParamF.z = 1.0f / ( ParamF.y - ParamF.x ) ;

		// �s�N�Z���V�F�[�_�[�̒萔�ɃZ�b�g
		SetPSConstF( 0, ParamF ) ;
	}
	else
	{
		// �[�x�l��`�悷��킯�ł͂Ȃ��ꍇ�̓I���W�i���̃V�F�[�_�[�͎g�p���Ȃ�
		MV1SetUseOrigShader( FALSE ) ;
	}

	// �[�x�l��`�悷��ꍇ�͒��_�V�F�[�_�[�����̃��b�V���p�̐[�x�l�`��p���_�V�F�[�_�[�ɂ���
	if( UseDepthShader != 0 )
	{
		SetUseVertexShader( Depth_NormalMeshVS ) ;
	}

	// �X�e�[�W���f��( ���̃��b�V�� )��`��
	MV1DrawModel( StageModelHandle ) ;


	// �[�x�l��`�悷��ꍇ�͒��_�V�F�[�_�[���X�L�j���O���b�V���p�̐[�x�l�`��p���_�V�F�[�_�[�ɂ���
	if( UseDepthShader != 0 )
	{
		SetUseVertexShader( Depth_SkinMeshVS ) ;
	}

	// �L�����N�^�[���f��( �X�L�j���O���b�V�� )��`��
	for( i = 0 ; i < CHARAMODEL_NUM ; i ++ )
	{
		MV1SetPosition( CharacterModelHandle, VGet( CHARAMODEL_X, 0.0f, CHARAMODEL_Z + i * CHARAMODEL_SPACE ) ) ;
		MV1DrawModel( CharacterModelHandle ) ;
	}
}

// WinMain �֐�
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	FLOAT4 ParamF ;
	float RightU, BottomV ;
	VERTEX2DSHADER Vertex[ 6 ] ;


	// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode( TRUE ) ;

	// ��ʃ��[�h��ݒ�
	SetGraphMode( SCREEN_W, SCREEN_H, 32 ) ;

	// �c�w���C�u�����̏�����
	if( DxLib_Init() < 0 )
	{
		// �G���[�����������璼���ɏI��
		return -1 ;
	}

	// �V�F�[�_�[���f���Q�D�O���g�p�ł��邩�ǂ������`�F�b�N
	if( GetValidShaderVersion() < 200 )
	{
		DrawString( 0, 0, "�V�F�[�_�[���f���Q�D�O���g�p�ł��܂���", GetColor( 255,255,255 ) ) ;
		DxLib_End() ;
		return 0 ;
	}

	// �ʏ�`��p�̃e�N�X�`�����쐬
	ColorMapTexture[ 0 ] = MakeScreen( SCREEN_TEX_W, SCREEN_TEX_H, TRUE ) ;

	// �ʏ�`�悵���f���Ɏア�ڂ������������摜��ۑ����邽�߂̃e�N�X�`�����쐬
	ColorMapTexture[ 1 ] = MakeScreen( SCREEN_TEX_W, SCREEN_TEX_H, TRUE ) ;

	// �ʏ�`�悵���f���ɋ����ڂ������������摜��ۑ����邽�߂̃e�N�X�`�����쐬
	ColorMapTexture[ 2 ] = MakeScreen( SCREEN_TEX_W, SCREEN_TEX_H, TRUE ) ;

	// �[�x�l��`�悷��P�`�����l���摜���쐬
	SetCreateDrawValidGraphChannelNum( 1 ) ;
	DepthMapTexture = MakeScreen( SCREEN_TEX_W, SCREEN_TEX_H, TRUE ) ;
	SetCreateDrawValidGraphChannelNum( 0 ) ;

	// �X�e�[�W���f���ƃL�����N�^�[���f���̓ǂݍ���
	StageModelHandle     = MV1LoadModel( "ColTestStage.mqo" ) ;
	CharacterModelHandle = MV1LoadModel( "DxChara.x" ) ;

	// �[�x�l�`��p�̒��_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[��ǂݍ���
	Depth_NormalMeshVS = LoadVertexShader( "Depth_NormalMeshVS.vso" ) ;
	Depth_SkinMeshVS   = LoadVertexShader( "Depth_SkinMeshVS.vso" ) ;
	DepthPS            = LoadPixelShader( "DepthPS.pso" ) ;

	// �[�x�l���g�p���Ĕ�ʊE�[�x�������s�����߂̃s�N�Z���V�F�[�_�[��ǂݍ���
	DepthOfFieldPS     = LoadPixelShader( "DepthOfFieldPS.pso" ) ;

	// ��ʊE�[�x�����p�̃p�����[�^��������
	DepthOfField_InterpSize = 3000.0f ;
	DepthOfField_Focus = 3200.0f ;
	DepthOfField_FocusSize = 500.0f ;

	// �J�����̊p�x��������
	CameraAngle = 90 ;

	// �J�����̍��W���Z�b�g
	CameraEyePosition = VGet( -2900.0f, 570.0f, -4600.0f ) ;

	// ���C�����[�v
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

		// �p�����[�^���쏈��
		{
			if( CheckHitKey( KEY_INPUT_A ) == 1 )
			{
				DepthOfField_Focus += 5.0f ;
			}

			if( CheckHitKey( KEY_INPUT_Z ) == 1 )
			{
				DepthOfField_Focus -= 5.0f ;
			}

			if( CheckHitKey( KEY_INPUT_S ) == 1 )
			{
				DepthOfField_FocusSize += 5.0f ;
			}

			if( CheckHitKey( KEY_INPUT_X ) == 1 )
			{
				DepthOfField_FocusSize -= 5.0f ;
			}

			if( CheckHitKey( KEY_INPUT_D ) == 1 )
			{
				DepthOfField_InterpSize += 5.0f ;
			}

			if( CheckHitKey( KEY_INPUT_C ) == 1 )
			{
				DepthOfField_InterpSize -= 5.0f ;
			}
		}

		// ��ʊE�[�x�`��̏���
		{
			// ��Ԕ͈͂ƃt�H�[�J�X�������Ă���͈͂��܂߂����������Z�o
			DepthOfField_TotalSize = DepthOfField_InterpSize * 2.0f + DepthOfField_FocusSize ;

			// �ʏ�`��p�e�N�X�`���Ƀ��f����ʏ�`��
			DrawScreen( ColorMapTexture[ 0 ], FALSE ) ;

			// �ʏ�`��̌��ʂɂڂ������������摜��ۑ�
			GraphFilterRectBlt( ColorMapTexture[ 0 ], ColorMapTexture[ 1 ], 0, 0, SCREEN_W, SCREEN_H, 0, 0, DX_GRAPH_FILTER_GAUSS, 16, 200 ) ;

			// �ڂ������������摜�ɍX�ɂڂ����������ĕۑ�
			GraphFilterRectBlt( ColorMapTexture[ 1 ], ColorMapTexture[ 2 ], 0, 0, SCREEN_W, SCREEN_H, 0, 0, DX_GRAPH_FILTER_GAUSS, 16, 200 ) ;

			// �[�x�l�`��p�e�N�X�`���ɐ[�x�l��`��
			DrawScreen( DepthMapTexture, TRUE ) ;
		}
		

		// �`���𗠉�ʂɕύX
		SetDrawScreen( DX_SCREEN_BACK ) ;

		// ��ʊE�[�x���{�����摜�𗠉�ʂɕ`��
		{
			// �g�p����e�N�X�`�����Z�b�g
			SetUseTextureToShader( 0, ColorMapTexture[ 0 ] ) ;
			SetUseTextureToShader( 1, ColorMapTexture[ 1 ] ) ;
			SetUseTextureToShader( 2, ColorMapTexture[ 2 ] ) ;
			SetUseTextureToShader( 3, DepthMapTexture ) ;

			// ��ʊE�[�x�����p�̃s�N�Z���V�F�[�_�[���Z�b�g
			SetUsePixelShader( DepthOfFieldPS ) ;

			// ��ʊE�[�x�̂ڂ��������̊J�n�ʒu���v�Z
			ParamF.x = DepthOfField_InterpSize / DepthOfField_TotalSize ;

			// ��ʊE�[�x�̂ڂ��������̏I���ʒu���v�Z
			ParamF.y = ( DepthOfField_InterpSize + DepthOfField_FocusSize ) / DepthOfField_TotalSize ;

			// �s�N�Z���V�F�[�_�[�̒萔�ɃZ�b�g
			SetPSConstF( 0, ParamF ) ;

			// ��ʑS�̂ɕ`�悷�钸�_�̏���
			Vertex[ 0 ].pos = VGet(     0.0f,     0.0f, 0.0f ) ;
			Vertex[ 1 ].pos = VGet( SCREEN_W,     0.0f, 0.0f ) ;
			Vertex[ 2 ].pos = VGet(     0.0f, SCREEN_H, 0.0f ) ;
			Vertex[ 3 ].pos = VGet( SCREEN_W, SCREEN_H, 0.0f ) ;

			Vertex[ 0 ].rhw = 1.0f ;
			Vertex[ 1 ].rhw = 1.0f ;
			Vertex[ 2 ].rhw = 1.0f ;
			Vertex[ 3 ].rhw = 1.0f ;

			RightU  = ( float )SCREEN_W / SCREEN_TEX_W ;
			BottomV = ( float )SCREEN_H / SCREEN_TEX_H ;
			Vertex[ 0 ].u = 0.0f ;		Vertex[ 0 ].v = 0.0f ;
			Vertex[ 1 ].u = RightU ;	Vertex[ 1 ].v = 0.0f ;
			Vertex[ 2 ].u = 0.0f ;		Vertex[ 2 ].v = BottomV ;
			Vertex[ 3 ].u = RightU ;	Vertex[ 3 ].v = BottomV ;

			Vertex[ 4 ] = Vertex[ 2 ] ;
			Vertex[ 5 ] = Vertex[ 1 ] ;

			// �|���S���Q�����g�p���ĉ�ʑS�̂ɔ�ʊE�[�x�������{�����R�c�V�[���̕`��
			DrawPolygon2DToShader( Vertex, 2 ) ;
		}

		// �p�����[�^��`��
		DrawFormatString( 0, 0, GetColor( 255,255,255 ), "��ԋ���:%.1f �ڂ����������S:%.3f �ڂ��������͈�:%.3f",
							DepthOfField_InterpSize, DepthOfField_Focus, DepthOfField_FocusSize ) ;

		// �L�[���������`��
		DrawString( 0, 480 - 16, "AZ�L�[:�ڂ����������S�ύX�@SX�L�[:�ڂ��������͈͕ύX�@DC�L�[:��ԋ����ύX", GetColor( 255,255,255 ) ) ;
		DrawString( 0, 480 - 32, "�����L�[:�J�����̑���", GetColor( 255,255,255 ) ) ;

		// ����ʂ̓��e��\��ʂɔ��f����
		ScreenFlip() ;
	}


	// �c�w���C�u�����̌�n��
	DxLib_End() ;

	// �\�t�g�̏I��
	return 0 ;
}
