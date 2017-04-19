// �R�c�A�N�V������{

#include "DxLib.h"
#include "DxGraphics.h"
#include <math.h>

// �Œ�l��` -------------------------------------------------------------------------------------

// �L�����N�^�[���ʂ̏��
#define CHARA_PLAY_ANIM_SPEED			250.0f		// �A�j���[�V�������x
#define CHARA_MOVE_SPEED			30.0f		// �ړ����x
#define CHARA_ANIM_BLEND_SPEED			0.1f		// �A�j���[�V�����̃u�����h���ω����x
#define CHARA_ANGLE_SPEED			0.2f		// �p�x�ω����x
#define CHARA_JUMP_POWER			100.0f		// �W�����v��
#define CHARA_FALL_UP_POWER			20.0f		// ���𓥂݊O�������̃W�����v��
#define CHARA_GRAVITY				3.0f		// �d��
#define CHARA_MAX_HITCOLL			2048		// ��������R���W�����|���S���̍ő吔
#define CHARA_ENUM_DEFAULT_SIZE			800.0f		// ���͂̃|���S�����o�Ɏg�p���鋅�̏����T�C�Y
#define CHARA_HIT_WIDTH				200.0f		// �����蔻��J�v�Z���̔��a
#define CHARA_HIT_HEIGHT			700.0f		// �����蔻��J�v�Z���̍���
#define CHARA_HIT_TRYNUM			16		// �ǉ����o�������̍ő厎�s��
#define CHARA_HIT_SLIDE_LENGTH			5.0f		// ��x�̕ǉ����o�������ŃX���C�h�����鋗��
#define CHARA_HIT_PUSH_POWER			40.0f		// �L�����N�^�[���m�œ��������Ƃ��̉����o������
#define CHARA_SHADOW_SIZE			200.0f		// �e�̑傫��
#define CHARA_SHADOW_HEIGHT			700.0f		// �e�������鍂��

// �J�����֌W�̒�`
#define CAMERA_ANGLE_SPEED			0.05f		// ���񑬓x
#define CAMERA_PLAYER_TARGET_HEIGHT		400.0f		// �v���C���[���W����ǂꂾ�������ʒu�𒍎��_�Ƃ��邩
#define CAMERA_PLAYER_LENGTH			1600.0f		// �v���C���[�Ƃ̋���
#define CAMERA_COLLISION_SIZE			50.0f		// �J�����̓����蔻��T�C�Y

// �v���C���[�L�����ȊO�L�����̏��
#define NOTPLAYER_NUM				4		// �v���C���[�ȊO�̃L�����̐�
#define NOTPLAYER_MOVETIME			120		// ��̕����Ɉړ����鎞��
#define NOTPLAYER_JUMPRATIO			250		// �v���C���[�ȊO�̃L�������W�����v����m��

// ���C�g�֌W
#define LIGHT_ANGLE_SPEED			0.01f		// ���C�g�̈ʒu�����肷��p�x�̕ω����x
#define LIGHT_CENTER_DISTANCE			1500.0f		// ���C�g���ړ�����͈�

// �\���̒�` -------------------------------------------------------------------------------------

// ���͏��\����
struct PADINPUT
{
	int		NowInput ;				// ���݂̓���
	int		EdgeInput ;				// ���݂̃t���[���ŉ����ꂽ�{�^���̂݃r�b�g�������Ă�����͒l
} ;

// �L�����N�^�[���
struct CHARA
{
	VECTOR		Position ;				// ���W
	VECTOR		TargetMoveDirection ;			// ���f���������ׂ������̃x�N�g��
	float		Angle ;					// ���f���������Ă�������̊p�x
	float		JumpPower ;				// �x�������̑��x
	int		ModelHandle ;				// ���f���n���h��
	int		State ;					// ���( 0:�����~�܂�  1:����  2:�W�����v )

	int		PlayAnim1 ;				// �Đ����Ă���A�j���[�V�����P�̃A�^�b�`�ԍ�( -1:�����A�j���[�V�������A�^�b�`����Ă��Ȃ� )
	float		AnimPlayCount1 ;			// �Đ����Ă���A�j���[�V�����P�̍Đ�����
	int		PlayAnim2 ;				// �Đ����Ă���A�j���[�V�����Q�̃A�^�b�`�ԍ�( -1:�����A�j���[�V�������A�^�b�`����Ă��Ȃ� )
	float		AnimPlayCount2 ;			// �Đ����Ă���A�j���[�V�����Q�̍Đ�����
	float		AnimBlendRate ;				// �Đ����Ă���A�j���[�V�����P�ƂQ�̃u�����h��
} ;

// �v���C���[���\����
struct PLAYER
{
	CHARA		CharaInfo ;				// �L�����N�^�[���
} ;

// �v���C���[�ȊO�L�������\����
struct NOTPLAYER
{
	CHARA		CharaInfo ;				// �L�����N�^�[���
	int		MoveTime ;				// �ړ�����
	float		MoveAngle ;				// �ړ�����
} ;

// �L�����N�^�[���ʏ��
struct CHARA_COMMON
{
	int		BaseModelHandle ;			// ���ʂ̔h�������f���n���h��
	int		ShadowHandle ;				// �e�`��p�̃O���t�B�b�N�n���h��
} ;

// �X�e�[�W���\����
struct STAGE
{
	int		ModelHandle ;				// ���f���n���h��

} ;

// �J�������\����
struct CAMERA
{
	float		AngleH ;				// �����p�x
	float		AngleV ;				// �����p�x
	VECTOR		Eye ;					// �J�������W
	VECTOR		Target ;				// �����_���W
} ;


// �֐��v���g�^�C�v�錾 ---------------------------------------------------------------------------

void Input_Process( void ) ;			// ���͏���

void Chara_Initialize( CHARA *ch, VECTOR Position ) ;			// �L�����N�^�[�̏�����
void Chara_Terminate( CHARA *ch ) ;					// �L�����N�^�[�̌�n��
void Chara_Process( CHARA *ch, VECTOR MoveVec, int JumpFlag ) ;		// �L�����N�^�[�̏���
void Chara_Move( CHARA *ch, VECTOR MoveVector ) ;			// �L�����N�^�[�̈ړ�����
void Chara_Collision( CHARA *ch, VECTOR *ch_MoveVec, CHARA *chk_ch ) ;	// �L�����N�^�[�ɓ������Ă����牟���o���������s��( chk_ch �� ch ���������Ă����� ch ������� )
void Chara_AngleProcess( CHARA *ch ) ;					// �L�����N�^�[�̌�����ς��鏈��
void Chara_PlayAnim( CHARA *ch, int PlayAnim ) ;			// �L�����N�^�[�ɐV���ȃA�j���[�V�������Đ�����
void Chara_AnimProcess( CHARA *ch ) ;					// �L�����N�^�[�̃A�j���[�V��������

void Player_Initialize( void ) ;		// �v���C���[�̏�����
void Player_Terminate( void ) ;			// �v���C���[�̌�n��
void Player_Process( void ) ;			// �v���C���[�̏���

void Stage_Initialize( void ) ;			// �X�e�[�W�̏���������
void Stage_Terminate( void ) ;			// �X�e�[�W�̌�n������

void Camera_Initialize( void ) ;		// �J�����̏���������
void Camera_Process( void ) ;			// �J�����̏���

void SetupDepthImage( void ) ;			// �e�`��p�̐[�x�L�^�摜����������
void DrawModelWithDepthShadow( void ) ;	// �e�p�̐[�x�L�^�摜���g�����e�𗎂Ƃ��������܂߂����f���̕`��

void Render_Process( void ) ;			// �`�揈��


// ���̐錾 ---------------------------------------------------------------------------------------

PADINPUT inp ;		// ���͏��̎��̐錾
PLAYER pl ;		// �v���C���[���̎��̐錾
NOTPLAYER npl[ NOTPLAYER_NUM ] ;	// �v���C���[�ł͂Ȃ��L�����̎��̐錾
CHARA_COMMON chcmn ;	// �L�����N�^�[�̋��ʏ��̎��̐錾
STAGE stg ;		// �X�e�[�W���̎��̐錾
CAMERA cam ;		// �J�������̎��̐錾

// �S���ʃV���h�E�}�b�v�������_�����O����ۂ̃J�����̕���
VECTOR depth_lookAt[ 6 ] =
{
	{  1.0f,  0.0f,  0.0f },
	{ -1.0f,  0.0f,  0.0f },
	{  0.0f,  1.0f,  0.0f },
	{  0.0f, -1.0f,  0.0f },
	{  0.0f,  0.0f,  1.0f },
	{  0.0f,  0.0f, -1.0f },
};

// �S���ʃV���h�E�}�b�v�������_�����O����ۂ̃J�����̏����
VECTOR depth_up[ 6 ] =
{
	{  0.0f,  1.0f,  0.0f },
	{  0.0f,  1.0f,  0.0f },
	{  0.0f,  0.0f, -1.0f },
	{  0.0f,  0.0f,  1.0f },
	{  0.0f,  1.0f,  0.0f },
	{  0.0f,  1.0f,  0.0f },
};

// ���C�g�̈ʒu
VECTOR LightPos ;
VECTOR LightBasePos = { 0.0f, 3375.0f, 0.0f } ;
float LightPosAngle ;

// �e�p�̐[�x�l��ۑ�����`��\�O���t�B�b�N
int DepthBufferGraphHandle[ 6 ] ;

// �e�p�̐[�x�L�^�摜���쐬�����ۂ̃J�����̃r���[�s��Ǝˉe�s��
MATRIX LightCamera_ViewMatrix[ 6 ] ;
MATRIX LightCamera_ProjectionMatrix[ 6 ] ;

// �[�x�L�^�摜�ւ̕`��p�̍��̃��b�V���p���_�V�F�[�_�[��
// �X�L�j���O���b�V���p�̒��_�V�F�[�_�[
int Skin4_DepthShadow_Step1_VertexShader ;	
int Normal_DepthShadow_Step1_VertexShader ;

// �[�x�L�^�摜�ւ̕`��p�̃s�N�Z���V�F�[�_�[
int DepthShadow_Step1_PixelShader ;

// �[�x�L�^�摜���g�����|�C���g���C�g��t���̕`��p�̍��̃��b�V���p���_�V�F�[�_�[��
// �X�L�j���O���b�V���p�̒��_�V�F�[�_�[
int Skin4_PointLight_DepthShadow_Step2_VertexShader ;
int Normal_PointLight_DepthShadow_Step2_VertexShader ;

// �[�x�L�^�摜���g�����|�C���g���C�g��t���̕`��p�̃s�N�Z���V�F�[�_�[
int PointLight_DepthShadow_Step2_PixelShader ;



// �֐����� ---------------------------------------------------------------------------------------

// ���͏���
void Input_Process( void )
{
	int Old ;

	// �ЂƂO�̃t���[���̓��͂�ϐ��ɂƂ��Ă���
	Old = inp.NowInput ;

	// ���݂̓��͏�Ԃ��擾
	inp.NowInput = GetJoypadInputState( DX_INPUT_KEY_PAD1 ) ;

	// ���̃t���[���ŐV���ɉ����ꂽ�{�^���̃r�b�g���������Ă���l�� EdgeInput �ɑ������
	inp.EdgeInput = inp.NowInput & ~Old ;
}

// �L�����N�^�[�̋��ʏ���������
void CharaCommon_Initialize( void )
{
	// ���f���̓ǂݍ���
	chcmn.BaseModelHandle = MV1LoadModel( "DxChara.x" ) ;

	// �e�`��p�̉摜�̓ǂݍ���
	chcmn.ShadowHandle = LoadGraph( "Shadow.tga" ) ;
}

// �L�����N�^�[���ʏ��̌�n��
void CharaCommon_Terminate( void )
{
	// ���f���̍폜
	MV1DeleteModel( chcmn.BaseModelHandle ) ;

	// �e�p�摜�̍폜
	DeleteGraph( chcmn.ShadowHandle ) ;
}

// �L�����N�^�[�̏�����
void Chara_Initialize( CHARA *ch, VECTOR Position )
{
	// �������W�͌��_
	ch->Position = Position ;

	// ��]�l�͂O
	ch->Angle = 0.0f ;

	// �W�����v�͂͏�����Ԃł͂O
	ch->JumpPower = 0.0f ;

	// ���f���n���h���̍쐬
	ch->ModelHandle = MV1DuplicateModel( chcmn.BaseModelHandle ) ;

	// ������Ԃł́u�����~��v���
	ch->State = 0 ;

	// ������Ԃ͂w������
	ch->TargetMoveDirection = VGet( 1.0f, 0.0f, 0.0f ) ;

	// �A�j���[�V�����̃u�����h����������
	ch->AnimBlendRate = 1.0f ;

	// ������Ԃł̓A�j���[�V�����̓A�^�b�`����Ă��Ȃ��ɂ���
	ch->PlayAnim1 = -1 ;
	ch->PlayAnim2 = -1 ;

	// ���������Ă���A�j���[�V�������Đ�
	Chara_PlayAnim( ch, 4 ) ;
}

// �L�����N�^�[�̌�n��
void Chara_Terminate( CHARA *ch )
{
	// ���f���̍폜
	MV1DeleteModel( ch->ModelHandle ) ;
}

// �L�����N�^�[�̏���
void Chara_Process( CHARA *ch, VECTOR MoveVec, int JumpFlag )
{
	int MoveFlag ;			// �ړ��������ǂ����̃t���O( 1:�ړ�����  0:�ړ����Ă��Ȃ� )

	// ���[�g�t���[���̂y�������̈ړ��p�����[�^�𖳌��ɂ���
	{
		MATRIX LocalMatrix ;

		// ���[�U�[�s�����������
		MV1ResetFrameUserLocalMatrix( ch->ModelHandle, 2 ) ;

		// ���݂̃��[�g�t���[���̍s����擾����
		LocalMatrix = MV1GetFrameLocalMatrix( ch->ModelHandle, 2 ) ;

		// �y�������̕��s�ړ������𖳌��ɂ���
		LocalMatrix.m[ 3 ][ 2 ] = 0.0f ;

		// ���[�U�[�s��Ƃ��ĕ��s�ړ������𖳌��ɂ����s������[�g�t���[���ɃZ�b�g����
		MV1SetFrameUserLocalMatrix( ch->ModelHandle, 2, LocalMatrix ) ;
	}

	// �ړ��������ǂ����̃t���O���Z�b�g�A�����ł��ړ����Ă�����u�ړ����Ă���v��\���P�ɂ���
	MoveFlag = 0 ;
	if( MoveVec.x < -0.001f || MoveVec.x > 0.001f ||
		MoveVec.y < -0.001f || MoveVec.y > 0.001f ||
		MoveVec.z < -0.001f || MoveVec.z > 0.001f )
	{
		MoveFlag = 1 ;
	}

	// �L�����N�^�[�̏�Ԃ��u�W�����v�v�ł͂Ȃ��A���W�����v�t���O�������Ă�����W�����v����
	if( ch->State != 2 && JumpFlag == 1 )
	{
		// ��Ԃ��u�W�����v�v�ɂ���
		ch->State = 2 ;

		// �x�������̑��x���Z�b�g
		ch->JumpPower = CHARA_JUMP_POWER ;

		// �W�����v�A�j���[�V�����̍Đ�
		Chara_PlayAnim( ch, 2 ) ;
	}

	// �ړ��{�^���������ꂽ���ǂ����ŏ����𕪊�
	if( MoveFlag )
	{
		// �ړ��x�N�g���𐳋K���������̂��L�����N�^�[�������ׂ������Ƃ��ĕۑ�
		ch->TargetMoveDirection = VNorm( MoveVec ) ;

		// �������܂Łu�����~�܂�v��Ԃ�������
		if( ch->State == 0 )
		{
			// ����A�j���[�V�������Đ�����
			Chara_PlayAnim( ch, 1 ) ;

			// ��Ԃ��u����v�ɂ���
			ch->State = 1 ;
		}
	}
	else
	{
		// ���̃t���[���ňړ����Ă��Ȃ��āA����Ԃ��u����v��������
		if( ch->State == 1 )
		{
			// �����~��A�j���[�V�������Đ�����
			Chara_PlayAnim( ch, 4 ) ;

			// ��Ԃ��u�����~��v�ɂ���
			ch->State = 0 ;
		}
	}

	// ��Ԃ��u�W�����v�v�̏ꍇ��
	if( ch->State == 2 )
	{
		// �x�������̑��x���d�͕����Z����
		ch->JumpPower -= CHARA_GRAVITY ;

		// �����������Ă��Ċ��Đ�����Ă���A�j���[�V�������㏸���p�̂��̂������ꍇ��
		if( ch->JumpPower < 0.0f && MV1GetAttachAnim( ch->ModelHandle, ch->PlayAnim1 ) == 2 )
		{
			// �������悤�̃A�j���[�V�������Đ�����
			Chara_PlayAnim( ch, 3 ) ;
		}

		// �ړ��x�N�g���̂x�������x�������̑��x�ɂ���
		MoveVec.y = ch->JumpPower ;
	}

	// �L�����N�^�[�̈ړ������Ƀ��f���̕������߂Â���
	Chara_AngleProcess( ch ) ;

	// �ړ��x�N�g�������ɃR���W�������l�����L�����N�^�[���ړ�
	Chara_Move( ch, MoveVec ) ;

	// �A�j���[�V��������
	Chara_AnimProcess( ch ) ;
}

// �L�����N�^�[�̈ړ�����
void Chara_Move( CHARA *ch, VECTOR MoveVector )
{
	int i, j, k ;						// �ėp�J�E���^�ϐ�
	int MoveFlag ;						// ���������Ɉړ��������ǂ����̃t���O( �O:�ړ����Ă��Ȃ�  �P:�ړ����� )
	int HitFlag ;						// �|���S���ɓ����������ǂ������L�����Ă����̂Ɏg���ϐ�( �O:�������Ă��Ȃ�  �P:�������� )
	MV1_COLL_RESULT_POLY_DIM HitDim ;			// �L�����N�^�[�̎��͂ɂ���|���S�������o�������ʂ��������铖���蔻�茋�ʍ\����
	int KabeNum ;						// �ǃ|���S���Ɣ��f���ꂽ�|���S���̐�
	int YukaNum ;						// ���|���S���Ɣ��f���ꂽ�|���S���̐�
	MV1_COLL_RESULT_POLY *Kabe[ CHARA_MAX_HITCOLL ] ;	// �ǃ|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
	MV1_COLL_RESULT_POLY *Yuka[ CHARA_MAX_HITCOLL ] ;	// ���|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
	MV1_COLL_RESULT_POLY *Poly ;				// �|���S���̍\���̂ɃA�N�Z�X���邽�߂Ɏg�p����|�C���^( �g��Ȃ��Ă��ς܂����܂����v���O�����������Ȃ�̂ŁE�E�E )
	HITRESULT_LINE LineRes ;				// �����ƃ|���S���Ƃ̓����蔻��̌��ʂ�������\����
	VECTOR OldPos ;						// �ړ��O�̍��W	
	VECTOR NowPos ;						// �ړ���̍��W

	// �ړ��O�̍��W��ۑ�
	OldPos = ch->Position ;

	// �ړ���̍��W���Z�o
	NowPos = VAdd( ch->Position, MoveVector ) ;

	// �L�����N�^�[�̎��͂ɂ���X�e�[�W�|���S�����擾����
	// ( ���o����͈͈͂ړ��������l������ )
	HitDim = MV1CollCheck_Sphere( stg.ModelHandle, -1, ch->Position, CHARA_ENUM_DEFAULT_SIZE + VSize( MoveVector ) ) ;

	// x����y�������� 0.01f �ȏ�ړ������ꍇ�́u�ړ������v�t���O���P�ɂ���
	if( fabs( MoveVector.x ) > 0.01f || fabs( MoveVector.z ) > 0.01f )
	{
		MoveFlag = 1 ;
	}
	else
	{
		MoveFlag = 0 ;
	}

	// ���o���ꂽ�|���S�����ǃ|���S��( �w�y���ʂɐ����ȃ|���S�� )�����|���S��( �w�y���ʂɐ����ł͂Ȃ��|���S�� )���𔻒f����
	{
		// �ǃ|���S���Ə��|���S���̐�������������
		KabeNum = 0 ;
		YukaNum = 0 ;

		// ���o���ꂽ�|���S���̐������J��Ԃ�
		for( i = 0 ; i < HitDim.HitNum ; i ++ )
		{
			// �w�y���ʂɐ������ǂ����̓|���S���̖@���̂x�������O�Ɍ���Ȃ��߂����ǂ����Ŕ��f����
			if( HitDim.Dim[ i ].Normal.y < 0.000001f && HitDim.Dim[ i ].Normal.y > -0.000001f )
			{
				// �ǃ|���S���Ɣ��f���ꂽ�ꍇ�ł��A�L�����N�^�[�̂x���W�{�P�D�O����荂���|���S���̂ݓ����蔻����s��
				if( HitDim.Dim[ i ].Position[ 0 ].y > ch->Position.y + 1.0f ||
					HitDim.Dim[ i ].Position[ 1 ].y > ch->Position.y + 1.0f ||
					HitDim.Dim[ i ].Position[ 2 ].y > ch->Position.y + 1.0f )
				{
					// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
					if( KabeNum < CHARA_MAX_HITCOLL )
					{
						// �|���S���̍\���̂̃A�h���X��ǃ|���S���|�C���^�z��ɕۑ�����
						Kabe[ KabeNum ] = &HitDim.Dim[ i ] ;

						// �ǃ|���S���̐������Z����
						KabeNum ++ ;
					}
				}
			}
			else
			{
				// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
				if( YukaNum < CHARA_MAX_HITCOLL )
				{
					// �|���S���̍\���̂̃A�h���X�����|���S���|�C���^�z��ɕۑ�����
					Yuka[ YukaNum ] = &HitDim.Dim[ i ] ;

					// ���|���S���̐������Z����
					YukaNum ++ ;
				}
			}
		}
	}

	// �ǃ|���S���Ƃ̓����蔻�菈��
	if( KabeNum != 0 )
	{
		// �ǂɓ����������ǂ����̃t���O�͏�����Ԃł́u�������Ă��Ȃ��v�ɂ��Ă���
		HitFlag = 0 ;

		// �ړ��������ǂ����ŏ����𕪊�
		if( MoveFlag == 1 )
		{
			// �ǃ|���S���̐������J��Ԃ�
			for( i = 0 ; i < KabeNum ; i ++ )
			{
				// i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
				Poly = Kabe[ i ] ;

				// �|���S���ƃL�����N�^�[���������Ă��Ȃ������玟�̃J�E���g��
				if( HitCheck_Capsule_Triangle( NowPos, VAdd( NowPos, VGet( 0.0f, CHARA_HIT_HEIGHT, 0.0f ) ), CHARA_HIT_WIDTH, Poly->Position[ 0 ], Poly->Position[ 1 ], Poly->Position[ 2 ] ) == FALSE ) continue ;

				// �����ɂ�����|���S���ƃL�����N�^�[���������Ă���Ƃ������ƂȂ̂ŁA�|���S���ɓ��������t���O�𗧂Ă�
				HitFlag = 1 ;

				// �ǂɓ���������ǂɎՂ��Ȃ��ړ������������ړ�����
				{
					VECTOR SlideVec ;	// �L�����N�^�[���X���C�h������x�N�g��

					// �i�s�����x�N�g���ƕǃ|���S���̖@���x�N�g���ɐ����ȃx�N�g�����Z�o
					SlideVec = VCross( MoveVector, Poly->Normal ) ;

					// �Z�o�����x�N�g���ƕǃ|���S���̖@���x�N�g���ɐ����ȃx�N�g�����Z�o�A���ꂪ
					// ���̈ړ���������Ǖ����̈ړ������𔲂����x�N�g��
					SlideVec = VCross( Poly->Normal, SlideVec ) ;

					// ������ړ��O�̍��W�ɑ��������̂�V���ȍ��W�Ƃ���
					NowPos = VAdd( OldPos, SlideVec ) ;
				}

				// �V���Ȉړ����W�ŕǃ|���S���Ɠ������Ă��Ȃ����ǂ����𔻒肷��
				for( j = 0 ; j < KabeNum ; j ++ )
				{
					// j�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
					Poly = Kabe[ j ] ;

					// �������Ă����烋�[�v���甲����
					if( HitCheck_Capsule_Triangle( NowPos, VAdd( NowPos, VGet( 0.0f, CHARA_HIT_HEIGHT, 0.0f ) ), CHARA_HIT_WIDTH, Poly->Position[ 0 ], Poly->Position[ 1 ], Poly->Position[ 2 ] ) == TRUE ) break ;
				}

				// j �� KabeNum �������ꍇ�͂ǂ̃|���S���Ƃ�������Ȃ������Ƃ������ƂȂ̂�
				// �ǂɓ��������t���O��|������Ń��[�v���甲����
				if( j == KabeNum )
				{
					HitFlag = 0 ;
					break ;
				}
			}
		}
		else
		{
			// �ړ����Ă��Ȃ��ꍇ�̏���
			
			// �ǃ|���S���̐������J��Ԃ�
			for( i = 0 ; i < KabeNum ; i ++ )
			{
				// i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
				Poly = Kabe[ i ] ;

				// �|���S���ɓ������Ă����瓖�������t���O�𗧂Ă���Ń��[�v���甲����
				if( HitCheck_Capsule_Triangle( NowPos, VAdd( NowPos, VGet( 0.0f, CHARA_HIT_HEIGHT, 0.0f ) ), CHARA_HIT_WIDTH, Poly->Position[ 0 ], Poly->Position[ 1 ], Poly->Position[ 2 ] ) == TRUE )
				{
					HitFlag = 1 ;
					break ;
				}
			}
		}

		// �ǂɓ������Ă�����ǂ��牟���o���������s��
		if( HitFlag == 1 )
		{
			// �ǂ���̉����o�����������݂�ő吔�����J��Ԃ�
			for( k = 0 ; k < CHARA_HIT_TRYNUM ; k ++ )
			{
				// �ǃ|���S���̐������J��Ԃ�
				for( i = 0 ; i < KabeNum ; i ++ )
				{
					// i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
					Poly = Kabe[ i ] ;

					// �L�����N�^�[�Ɠ������Ă��邩�𔻒�
					if( HitCheck_Capsule_Triangle( NowPos, VAdd( NowPos, VGet( 0.0f, CHARA_HIT_HEIGHT, 0.0f ) ), CHARA_HIT_WIDTH, Poly->Position[ 0 ], Poly->Position[ 1 ], Poly->Position[ 2 ] ) == FALSE ) continue ;

					// �������Ă�����K�苗�����L�����N�^�[��ǂ̖@�������Ɉړ�������
					NowPos = VAdd( NowPos, VScale( Poly->Normal, CHARA_HIT_SLIDE_LENGTH ) ) ;

					// �ړ�������ŕǃ|���S���ƐڐG���Ă��邩�ǂ����𔻒�
					for( j = 0 ; j < KabeNum ; j ++ )
					{
						// �������Ă����烋�[�v�𔲂���
						Poly = Kabe[ j ] ;
						if( HitCheck_Capsule_Triangle( NowPos, VAdd( NowPos, VGet( 0.0f, CHARA_HIT_HEIGHT, 0.0f ) ), CHARA_HIT_WIDTH, Poly->Position[ 0 ], Poly->Position[ 1 ], Poly->Position[ 2 ] ) == TRUE ) break ;
					}

					// �S�Ẵ|���S���Ɠ������Ă��Ȃ������炱���Ń��[�v�I��
					if( j == KabeNum ) break ;
				}

				// i �� KabeNum �ł͂Ȃ��ꍇ�͑S���̃|���S���ŉ����o�������݂�O�ɑS�Ă̕ǃ|���S���ƐڐG���Ȃ��Ȃ����Ƃ������ƂȂ̂Ń��[�v���甲����
				if( i != KabeNum ) break ;
			}
		}
	}

	// ���|���S���Ƃ̓����蔻��
	if( YukaNum != 0 )
	{
		// �W�����v�����㏸���̏ꍇ�͏����𕪊�
		if( ch->State == 2 && ch->JumpPower > 0.0f )
		{
			float MinY ;

			// �V��ɓ����Ԃ��鏈�����s��

			// ��ԒႢ�V��ɂԂ���ׂ̔���p�ϐ���������
			MinY = 0.0f ;

			// �����������ǂ����̃t���O�𓖂����Ă��Ȃ����Ӗ�����O�ɂ��Ă���
			HitFlag = 0 ;

			// ���|���S���̐������J��Ԃ�
			for( i = 0 ; i < YukaNum ; i ++ )
			{
				// i�Ԗڂ̏��|���S���̃A�h���X�����|���S���|�C���^�z�񂩂�擾
				Poly = Yuka[ i ] ;

				// ���悩�瓪�̍����܂ł̊ԂŃ|���S���ƐڐG���Ă��邩�ǂ����𔻒�
				LineRes = HitCheck_Line_Triangle( NowPos, VAdd( NowPos, VGet( 0.0f, CHARA_HIT_HEIGHT, 0.0f ) ), Poly->Position[ 0 ], Poly->Position[ 1 ], Poly->Position[ 2 ] ) ;

				// �ڐG���Ă��Ȃ������牽�����Ȃ�
				if( LineRes.HitFlag == FALSE ) continue ;
				
				// ���Ƀ|���S���ɓ������Ă��āA�����܂Ō��o�����V��|���S����荂���ꍇ�͉������Ȃ�
				if( HitFlag == 1 && MinY < LineRes.Position.y ) continue ;

				// �|���S���ɓ��������t���O�𗧂Ă�
				HitFlag = 1 ;

				// �ڐG�����x���W��ۑ�����
				MinY = LineRes.Position.y ;
			}

			// �ڐG�����|���S�������������ǂ����ŏ����𕪊�
			if( HitFlag == 1 )
			{
				// �ڐG�����ꍇ�̓L�����N�^�[�̂x���W��ڐG���W�����ɍX�V
				NowPos.y = MinY - CHARA_HIT_HEIGHT ;

				// �x�������̑��x�͔��]
				ch->JumpPower = -ch->JumpPower ;
			}
		}
		else
		{
			float MaxY ;

			// ���~�����W�����v���ł͂Ȃ��ꍇ�̏���

			// ���|���S���ɓ����������ǂ����̃t���O��|���Ă���
			HitFlag = 0 ;

			// ��ԍ������|���S���ɂԂ���ׂ̔���p�ϐ���������
			MaxY = 0.0f ;

			// ���|���S���̐������J��Ԃ�
			for( i = 0 ; i < YukaNum ; i ++ )
			{
				// i�Ԗڂ̏��|���S���̃A�h���X�����|���S���|�C���^�z�񂩂�擾
				Poly = Yuka[ i ] ;

				// �W�����v�����ǂ����ŏ����𕪊�
				if( ch->State == 2 )
				{
					// �W�����v���̏ꍇ�͓��̐悩�瑫���菭���Ⴂ�ʒu�̊Ԃœ������Ă��邩�𔻒�
					LineRes = HitCheck_Line_Triangle( VAdd( NowPos, VGet( 0.0f, CHARA_HIT_HEIGHT, 0.0f ) ), VAdd( NowPos, VGet( 0.0f, -1.0f, 0.0f ) ), Poly->Position[ 0 ], Poly->Position[ 1 ], Poly->Position[ 2 ] ) ;
				}
				else
				{
					// �����Ă���ꍇ�͓��̐悩�炻�������Ⴂ�ʒu�̊Ԃœ������Ă��邩�𔻒�( �X�΂ŗ�����ԂɈڍs���Ă��܂�Ȃ��� )
					LineRes = HitCheck_Line_Triangle( VAdd( NowPos, VGet( 0.0f, CHARA_HIT_HEIGHT, 0.0f ) ), VAdd( NowPos, VGet( 0.0f, -40.0f, 0.0f ) ), Poly->Position[ 0 ], Poly->Position[ 1 ], Poly->Position[ 2 ] ) ;
				}

				// �������Ă��Ȃ������牽�����Ȃ�
				if( LineRes.HitFlag == FALSE ) continue ;

				// ���ɓ��������|���S��������A�����܂Ō��o�������|���S�����Ⴂ�ꍇ�͉������Ȃ�
				if( HitFlag == 1 && MaxY > LineRes.Position.y ) continue ;

				// �|���S���ɓ��������t���O�𗧂Ă�
				HitFlag = 1 ;

				// �ڐG�����x���W��ۑ�����
				MaxY = LineRes.Position.y ;
			}

			// ���|���S���ɓ����������ǂ����ŏ����𕪊�
			if( HitFlag == 1 )
			{
				// ���������ꍇ

				// �ڐG�����|���S���ň�ԍ����x���W���L�����N�^�[�̂x���W�ɂ���
				NowPos.y = MaxY ;

				// �x�������̈ړ����x�͂O��
				ch->JumpPower = 0.0f ;

				// �����W�����v���������ꍇ�͒��n��Ԃɂ���
				if( ch->State == 2 )
				{
					// �ړ����Ă������ǂ����Œ��n��̏�ԂƍĐ�����A�j���[�V�����𕪊򂷂�
					if( MoveFlag )
					{
						// �ړ����Ă���ꍇ�͑����Ԃ�
						Chara_PlayAnim( ch, 1 ) ;
						ch->State = 1 ;
					}
					else
					{
						// �ړ����Ă��Ȃ��ꍇ�͗����~���Ԃ�
						Chara_PlayAnim( ch, 4 ) ;
						ch->State = 0 ;
					}

					// ���n���̓A�j���[�V�����̃u�����h�͍s��Ȃ�
					ch->AnimBlendRate = 1.0f ;
				}
			}
			else
			{
				// ���R���W�����ɓ������Ă��Ȃ��Ċ��W�����v��Ԃł͂Ȃ������ꍇ��
				if( ch->State != 2 )
				{
					// �W�����v���ɂ���
					ch->State = 2 ;

					// ������Ƃ����W�����v����
					ch->JumpPower = CHARA_FALL_UP_POWER ;

					// �A�j���[�V�����͗������̂��̂ɂ���
					Chara_PlayAnim( ch, 3 ) ;
				}
			}
		}
	}

	// �V�������W��ۑ�����
	ch->Position = NowPos ;

	// �L�����N�^�[�̃��f���̍��W���X�V����
	MV1SetPosition( ch->ModelHandle, ch->Position ) ;

	// ���o�����L�����N�^�[�̎��͂̃|���S�������J������
	MV1CollResultPolyDimTerminate( HitDim ) ;
}

// �L�����N�^�[�ɓ������Ă����牟���o���������s��( chk_ch �� ch ���������Ă����� ch ������� )
void Chara_Collision( CHARA *ch, VECTOR *ch_MoveVec, CHARA *chk_ch )
{
	VECTOR ChkChToChVec ;
	VECTOR PushVec ;
	VECTOR ChPosition ;
	float Length ;

	// �ړ���� ch �̍��W���Z�o
	ChPosition = VAdd( ch->Position, *ch_MoveVec ) ;

	// �������Ă��Ȃ������牽�����Ȃ�
	if( HitCheck_Capsule_Capsule(
			ChPosition, VAdd( ChPosition, VGet( 0.0f, CHARA_HIT_HEIGHT, 0.0f ) ), CHARA_HIT_WIDTH, 
			chk_ch->Position, VAdd( chk_ch->Position, VGet( 0.0f, CHARA_HIT_HEIGHT, 0.0f ) ), CHARA_HIT_WIDTH ) == TRUE )
	{
		// �������Ă����� ch �� chk ���痣��鏈��������

		// chk_ch ���� ch �ւ̃x�N�g�����Z�o
		ChkChToChVec = VSub( ChPosition, chk_ch->Position ) ;

		// �x���͌��Ȃ�
		ChkChToChVec.y = 0.0f ;

		// ��l�̋������Z�o
		Length = VSize( ChkChToChVec ) ;

		// chk_ch ���� ch �ւ̃x�N�g���𐳋K��( �x�N�g���̒����� 1.0f �ɂ��� )
		PushVec = VScale( ChkChToChVec, 1.0f / Length ) ;

		// �����o���������Z�o�A������l�̋��������l�̑傫�����������l�ɉ����o���͂𑫂��ė���Ă��܂��ꍇ�́A�҂����肭���������Ɉړ�����
		if( Length - CHARA_HIT_WIDTH * 2.0f + CHARA_HIT_PUSH_POWER > 0.0f )
		{
			float TempY ;

			TempY = ChPosition.y ;
			ChPosition = VAdd( chk_ch->Position, VScale( PushVec, CHARA_HIT_WIDTH * 2.0f ) ) ; 

			// �x���W�͕ω������Ȃ�
			ChPosition.y = TempY ;
		}
		else
		{
			// �����o��
			ChPosition = VAdd( ChPosition, VScale( PushVec, CHARA_HIT_PUSH_POWER ) ) ;
		}
	}

	// �����蔻�菈����̈ړ��x�N�g�����Z�b�g
	*ch_MoveVec = VSub( ChPosition, ch->Position ) ;
}

// �L�����N�^�[�̌�����ς��鏈��
void Chara_AngleProcess( CHARA *ch )
{
	float TargetAngle ;			// �ڕW�p�x
	float SaAngle ;				// �ڕW�p�x�ƌ��݂̊p�x�Ƃ̍�

	// �ڕW�̕����x�N�g������p�x�l���Z�o����
	TargetAngle = atan2( ch->TargetMoveDirection.x, ch->TargetMoveDirection.z ) ;

	// �ڕW�̊p�x�ƌ��݂̊p�x�Ƃ̍�������o��
	{
		// �ŏ��͒P���Ɉ����Z
		SaAngle = TargetAngle - ch->Angle ;

		// ����������炠������̍����P�W�O�x�ȏ�ɂȂ邱�Ƃ͖����̂�
		// ���̒l���P�W�O�x�ȏ�ɂȂ��Ă�����C������
		if( SaAngle < -DX_PI_F )
		{
			SaAngle += DX_TWO_PI_F ;
		}
		else
		if( SaAngle > DX_PI_F )
		{
			SaAngle -= DX_TWO_PI_F ;
		}
	}

	// �p�x�̍����O�ɋ߂Â���
	if( SaAngle > 0.0f )
	{
		// �����v���X�̏ꍇ�͈���
		SaAngle -= CHARA_ANGLE_SPEED ;
		if( SaAngle < 0.0f )
		{
			SaAngle = 0.0f ;
		}
	}
	else
	{
		// �����}�C�i�X�̏ꍇ�͑���
		SaAngle += CHARA_ANGLE_SPEED ;
		if( SaAngle > 0.0f )
		{
			SaAngle = 0.0f ;
		}
	}

	// ���f���̊p�x���X�V
	ch->Angle = TargetAngle - SaAngle ;
	MV1SetRotationXYZ( ch->ModelHandle, VGet( 0.0f, ch->Angle + DX_PI_F, 0.0f ) ) ;
}

// �L�����N�^�[�ɐV���ȃA�j���[�V�������Đ�����
void Chara_PlayAnim( CHARA *ch, int PlayAnim )
{
	// �Đ����̃��[�V�����Q���L����������f�^�b�`����
	if( ch->PlayAnim2 != -1 )
	{
		MV1DetachAnim( ch->ModelHandle, ch->PlayAnim2 ) ;
		ch->PlayAnim2 = -1 ;
	}

	// ���܂ōĐ����̃��[�V�����P���������̂̏����Q�Ɉړ�����
	ch->PlayAnim2 = ch->PlayAnim1 ;
	ch->AnimPlayCount2 = ch->AnimPlayCount1 ;

	// �V���Ɏw��̃��[�V���������f���ɃA�^�b�`���āA�A�^�b�`�ԍ���ۑ�����
	ch->PlayAnim1 = MV1AttachAnim( ch->ModelHandle, PlayAnim ) ;
	ch->AnimPlayCount1 = 0.0f ;

	// �u�����h���͍Đ����̃��[�V�����Q���L���ł͂Ȃ��ꍇ�͂P�D�O��( �Đ����̃��[�V�����P���P�O�O���̏�� )�ɂ���
	ch->AnimBlendRate = ch->PlayAnim2 == -1 ? 1.0f : 0.0f ;
}

// �L�����N�^�[�̃A�j���[�V��������
void Chara_AnimProcess( CHARA *ch )
{
	float AnimTotalTime ;		// �Đ����Ă���A�j���[�V�����̑�����

	// �u�����h�����P�ȉ��̏ꍇ�͂P�ɋ߂Â���
	if( ch->AnimBlendRate < 1.0f )
	{
		ch->AnimBlendRate += CHARA_ANIM_BLEND_SPEED ;
		if( ch->AnimBlendRate > 1.0f )
		{
			ch->AnimBlendRate = 1.0f ;
		}
	}

	// �Đ����Ă���A�j���[�V�����P�̏���
	if( ch->PlayAnim1 != -1 )
	{
		// �A�j���[�V�����̑����Ԃ��擾
		AnimTotalTime = MV1GetAttachAnimTotalTime( ch->ModelHandle, ch->PlayAnim1 ) ;

		// �Đ����Ԃ�i�߂�
		ch->AnimPlayCount1 += CHARA_PLAY_ANIM_SPEED ;

		// �Đ����Ԃ������Ԃɓ��B���Ă�����Đ����Ԃ����[�v������
		if( ch->AnimPlayCount1 >= AnimTotalTime )
		{
			ch->AnimPlayCount1 = fmod( ch->AnimPlayCount1, AnimTotalTime ) ;
		}

		// �ύX�����Đ����Ԃ����f���ɔ��f������
		MV1SetAttachAnimTime( ch->ModelHandle, ch->PlayAnim1, ch->AnimPlayCount1 ) ;

		// �A�j���[�V�����P�̃��f���ɑ΂��锽�f�����Z�b�g
		MV1SetAttachAnimBlendRate( ch->ModelHandle, ch->PlayAnim1, ch->AnimBlendRate ) ;
	}

	// �Đ����Ă���A�j���[�V�����Q�̏���
	if( ch->PlayAnim2 != -1 )
	{
		// �A�j���[�V�����̑����Ԃ��擾
		AnimTotalTime = MV1GetAttachAnimTotalTime( ch->ModelHandle, ch->PlayAnim2 ) ;

		// �Đ����Ԃ�i�߂�
		ch->AnimPlayCount2 += CHARA_PLAY_ANIM_SPEED ;

		// �Đ����Ԃ������Ԃɓ��B���Ă�����Đ����Ԃ����[�v������
		if( ch->AnimPlayCount2 > AnimTotalTime )
		{
			ch->AnimPlayCount2 = fmod( ch->AnimPlayCount2, AnimTotalTime ) ;
		}

		// �ύX�����Đ����Ԃ����f���ɔ��f������
		MV1SetAttachAnimTime( ch->ModelHandle, ch->PlayAnim2, ch->AnimPlayCount2 ) ;

		// �A�j���[�V�����Q�̃��f���ɑ΂��锽�f�����Z�b�g
		MV1SetAttachAnimBlendRate( ch->ModelHandle, ch->PlayAnim2, 1.0f - ch->AnimBlendRate ) ;
	}
}


// �v���C���[�̏�����
void Player_Initialize( void )
{
	// �L�����N�^�[����������
	Chara_Initialize( &pl.CharaInfo, VGet( 0.0f, 0.0f, 0.0f ) ) ;
}

// �v���C���[�̌�n��
void Player_Terminate( void )
{
	// �L�����N�^�[���̌�n��
	Chara_Terminate( &pl.CharaInfo ) ;
}

// �v���C���[�̏���
void Player_Process( void )
{
	VECTOR UpMoveVec ;		// �����{�^���u���v����͂������Ƃ��̃v���C���[�̈ړ������x�N�g��
	VECTOR LeftMoveVec ;		// �����{�^���u���v����͂������Ƃ��̃v���C���[�̈ړ������x�N�g��
	VECTOR MoveVec ;		// ���̃t���[���̈ړ��x�N�g��
	int JumpFlag ;			// �W�����v�t���O
	int i ;

	// �v���C���[�̈ړ������̃x�N�g�����Z�o
	{
		// �����{�^���u���v���������Ƃ��̃v���C���[�̈ړ��x�N�g���̓J�����̎�����������x�����𔲂�������
		UpMoveVec = VSub( cam.Target, cam.Eye ) ;
		UpMoveVec.y = 0.0f ;

		// �����{�^���u���v���������Ƃ��̃v���C���[�̈ړ��x�N�g���͏���������Ƃ��̕����x�N�g���Ƃx���̃v���X�����̃x�N�g���ɐ����ȕ���
		LeftMoveVec = VCross( UpMoveVec, VGet( 0.0f, 1.0f, 0.0f ) ) ;

		// ��̃x�N�g���𐳋K��( �x�N�g���̒������P�D�O�ɂ��邱�� )
		UpMoveVec = VNorm( UpMoveVec ) ;
		LeftMoveVec = VNorm( LeftMoveVec ) ;
	}

	// ���̃t���[���ł̈ړ��x�N�g����������
	MoveVec = VGet( 0.0f, 0.0f, 0.0f ) ;

	// �W�����v�t���O��|��
	JumpFlag = 0 ;

	// �p�b�h�̂R�{�^���ƍ��V�t�g���ǂ����������Ă��Ȃ�������v���C���[�̈ړ�����
	if( CheckHitKey( KEY_INPUT_LSHIFT ) == 0 && ( inp.NowInput & PAD_INPUT_C ) == 0 )
	{
		// �����{�^���u���v�����͂��ꂽ��J�����̌��Ă���������猩�č������Ɉړ�����
		if( inp.NowInput & PAD_INPUT_LEFT  )
		{
			// �ړ��x�N�g���Ɂu���v�����͂��ꂽ���̈ړ��x�N�g�������Z����
			MoveVec = VAdd( MoveVec, LeftMoveVec ) ;
		}
		else
		// �����{�^���u���v�����͂��ꂽ��J�����̌��Ă���������猩�ĉE�����Ɉړ�����
		if( inp.NowInput & PAD_INPUT_RIGHT )
		{
			// �ړ��x�N�g���Ɂu���v�����͂��ꂽ���̈ړ��x�N�g���𔽓]�������̂����Z����
			MoveVec = VAdd( MoveVec, VScale( LeftMoveVec, -1.0f ) ) ;
		}

		// �����{�^���u���v�����͂��ꂽ��J�����̌��Ă�������Ɉړ�����
		if( inp.NowInput & PAD_INPUT_UP    )
		{
			// �ړ��x�N�g���Ɂu���v�����͂��ꂽ���̈ړ��x�N�g�������Z����
			MoveVec = VAdd( MoveVec, UpMoveVec ) ;
		}
		else
		// �����{�^���u���v�����͂��ꂽ��J�����̕����Ɉړ�����
		if( inp.NowInput & PAD_INPUT_DOWN  )
		{
			// �ړ��x�N�g���Ɂu���v�����͂��ꂽ���̈ړ��x�N�g���𔽓]�������̂����Z����
			MoveVec = VAdd( MoveVec, VScale( UpMoveVec, -1.0f ) ) ;
		}

		// �{�^���P��������Ă�����W�����v�t���O�𗧂Ă�
		if( inp.EdgeInput & PAD_INPUT_A )
		{
			JumpFlag = 1 ;
		}
	}

	// �ړ��������ړ����x�ŃX�P�[�����O����
	MoveVec = VScale( MoveVec, CHARA_MOVE_SPEED ) ;

	// �v���C���[�L�����ȊO�Ƃ̓����蔻����s��
	for( i = 0 ; i < NOTPLAYER_NUM ; i ++ )
	{
		Chara_Collision( &pl.CharaInfo, &MoveVec, &npl[ i ].CharaInfo ) ;
	}

	// �L�����N�^�[�𓮍삳���鏈�����s��
	Chara_Process( &pl.CharaInfo, MoveVec, JumpFlag ) ;
}

// �v���C���[�ȊO�L�����̏�����
void NotPlayer_Initialize( void )
{
	int i ;
	static VECTOR FirstPosition[ NOTPLAYER_NUM ] =
	{
		{ -3000.0f, 0.0f, 2300.0f },
		{ -2500.0f, 7300.0f, -2500.0f },
		{ -2600.0f, 0.0f, -3100.0f },
		{  2800.0f, 0.0f, 200.0f },
	} ;

	for( i = 0 ; i < NOTPLAYER_NUM ; i ++ )
	{
		// �L�����N�^�[����������
		Chara_Initialize( &npl[ i ].CharaInfo, FirstPosition[ i ] ) ;

		// �ړ����Ԃ�������
		npl[ i ].MoveTime = 0 ;

		// �ړ�������������
		npl[ i ].MoveAngle = GetRand( 1000 ) * DX_PI_F * 2.0f / 1000.0f ;
	}
}

// �v���C���[�ȊO�L�����̌�n��
void NotPlayer_Terminate( void )
{
	int i ;

	// �L�����N�^�̐������J��Ԃ�
	for( i = 0 ; i < NOTPLAYER_NUM ; i ++ )
	{
		// �L�����N�^�[���̌�n��
		Chara_Terminate( &npl[ i ].CharaInfo ) ;
	}
}

// �v���C���[�ȊO�L�����̏���
void NotPlayer_Process( void )
{
	int i ;
	int j ;
	VECTOR MoveVec ;
	int JumpFlag ;

	// �L�����N�^�̐������J��Ԃ�
	for( i = 0 ; i < NOTPLAYER_NUM ; i ++ )
	{
		// �W�����v�t���O��|���Ă���
		JumpFlag = 0 ;

		// ��莞�Ԃ��o�߂�����ړ����������ύX����
		npl[ i ].MoveTime ++ ;
		if( npl[ i ].MoveTime >= NOTPLAYER_MOVETIME )
		{
			npl[ i ].MoveTime = 0 ;

			// �V���������̌���
			npl[ i ].MoveAngle = GetRand( 1000 ) * DX_PI_F * 2.0f / 1000.0f ;

			// ���m���ŃW�����v����
			if( GetRand( 1000 ) < NOTPLAYER_JUMPRATIO )
			{
				JumpFlag = 1 ;
			}
		}

		// �V���������̊p�x����x�N�g�����Z�o
		MoveVec.x = cos( npl[ i ].MoveAngle ) * CHARA_MOVE_SPEED ;
		MoveVec.y = 0.0f ;
		MoveVec.z = sin( npl[ i ].MoveAngle ) * CHARA_MOVE_SPEED ;

		// �v���C���[�Ƃ̓����蔻����s��
		Chara_Collision( &npl[ i ].CharaInfo, &MoveVec, &pl.CharaInfo ) ;

		// �����ȊO�̃v���C���[�L�����Ƃ̓����蔻����s��
		for( j = 0 ; j < NOTPLAYER_NUM ; j ++ )
		{
			// �����Ƃ̓����蔻��͂��Ȃ�
			if( i == j ) continue ;

			Chara_Collision( &npl[ i ].CharaInfo, &MoveVec, &npl[ j ].CharaInfo ) ;
		}

		// �ړ��������s��
		Chara_Process( &npl[ i ].CharaInfo, MoveVec, JumpFlag ) ; 
	}
}

// �X�e�[�W�̏���������
void Stage_Initialize( void )
{
	int i ;

	// �X�e�[�W���f���̓ǂݍ���
	stg.ModelHandle = MV1LoadModel( "ColTestStage.mqo" ) ;

	// ���f���S�̂̃R���W�������̃Z�b�g�A�b�v
	MV1SetupCollInfo( stg.ModelHandle, -1 ) ;

	// �쐬����摜�̃t�H�[�}�b�g��s�������_�^�łP�`�����l���A�P�U�r�b�g�ɂ���
	SetDrawValidFloatTypeGraphCreateFlag( TRUE ) ;
	SetCreateDrawValidGraphChannelNum( 1 ) ;
	SetCreateGraphColorBitDepth( 16 ) ;

	// �e�p�[�x�L�^�摜�̍쐬
	for( i = 0; i < 6; i++ )
	{
		DepthBufferGraphHandle[ i ] = MakeScreen( 2048, 2048, FALSE ) ;
	}

	// �ݒ�����ɖ߂�
	SetDrawValidFloatTypeGraphCreateFlag( FALSE ) ;
	SetCreateDrawValidGraphChannelNum( 4 ) ;
	SetCreateGraphColorBitDepth( 32 ) ;

	// �[�x�L�^�摜�`��p�̒��_�V�F�[�_�[��ǂݍ���
	Skin4_DepthShadow_Step1_VertexShader  = LoadVertexShader( "SkinMesh4_DepthShadow_Step1VS.vso" ) ;
	Normal_DepthShadow_Step1_VertexShader = LoadVertexShader( "NormalMesh_DepthShadow_Step1VS.vso" ) ;

	// �[�x�L�^�摜�`��p�̃s�N�Z���V�F�[�_�[��ǂݍ���
	DepthShadow_Step1_PixelShader = LoadPixelShader( "DepthShadow_Step1PS.pso" ) ;

	// �[�x�L�^�摜���g�����|�C���g���C�g��̕`��p���_�V�F�[�_�[��ǂݍ���
	Skin4_PointLight_DepthShadow_Step2_VertexShader = LoadVertexShader( "SkinMesh4_PointLight_DepthShadow_Step2VS.vso" ) ;
	Normal_PointLight_DepthShadow_Step2_VertexShader = LoadVertexShader( "NormalMesh_PointLight_DepthShadow_Step2VS.vso" ) ;

	// �[�x�L�^�摜���g�����|�C���g���C�g��̕`��p�s�N�Z���V�F�[�_�[��ǂݍ���
	PointLight_DepthShadow_Step2_PixelShader = LoadPixelShader( "PointLight_DepthShadow_Step2PS.pso" ) ;
}

// �X�e�[�W�̌�n������
void Stage_Terminate( void )
{
	// �X�e�[�W���f���̌�n��
	MV1DeleteModel( stg.ModelHandle ) ;
}

// �J�����̏���������
void Camera_Initialize( void )
{
	// �J�����̏��������p�x�͂P�W�O�x
	cam.AngleH = DX_PI_F ;

	// �����p�x�͂O�x
	cam.AngleV = 0.0f ;
}

// �J�����̏���
void Camera_Process( void )
{
	// �p�b�h�̂R�{�^�����A�V�t�g�L�[��������Ă���ꍇ�̂݊p�x�ύX������s��
	if( CheckHitKey( KEY_INPUT_LSHIFT ) || ( inp.NowInput & PAD_INPUT_C ) )
	{
		// �u���v�{�^����������Ă����琅���p�x���}�C�i�X����
		if( inp.NowInput & PAD_INPUT_LEFT )
		{
			cam.AngleH -= CAMERA_ANGLE_SPEED ;

			// �|�P�W�O�x�ȉ��ɂȂ�����p�x�l���傫���Ȃ肷���Ȃ��悤�ɂR�U�O�x�𑫂�
			if( cam.AngleH < -DX_PI_F )
			{
				cam.AngleH += DX_TWO_PI_F ;
			}
		}

		// �u���v�{�^����������Ă����琅���p�x���v���X����
		if( inp.NowInput & PAD_INPUT_RIGHT )
		{
			cam.AngleH += CAMERA_ANGLE_SPEED ;

			// �P�W�O�x�ȏ�ɂȂ�����p�x�l���傫���Ȃ肷���Ȃ��悤�ɂR�U�O�x������
			if( cam.AngleH > DX_PI_F )
			{
				cam.AngleH -= DX_TWO_PI_F ;
			}
		}

		// �u���v�{�^����������Ă����琂���p�x���}�C�i�X����
		if( inp.NowInput & PAD_INPUT_UP )
		{
			cam.AngleV -= CAMERA_ANGLE_SPEED ;

			// ������p�x�ȉ��ɂ͂Ȃ�Ȃ��悤�ɂ���
			if( cam.AngleV < -DX_PI_F / 2.0f + 0.6f )
			{
				cam.AngleV = -DX_PI_F / 2.0f + 0.6f ;
			}
		}

		// �u���v�{�^����������Ă����琂���p�x���v���X����
		if( inp.NowInput & PAD_INPUT_DOWN )
		{
			cam.AngleV += CAMERA_ANGLE_SPEED ;

			// ������p�x�ȏ�ɂ͂Ȃ�Ȃ��悤�ɂ���
			if( cam.AngleV > DX_PI_F / 2.0f - 0.6f )
			{
				cam.AngleV = DX_PI_F / 2.0f - 0.6f ;
			}
		}
	}

	// �J�����̒����_�̓v���C���[���W����K��l���������W
	cam.Target = VAdd( pl.CharaInfo.Position, VGet( 0.0f, CAMERA_PLAYER_TARGET_HEIGHT, 0.0f ) ) ;

	// �J�����̍��W�����肷��
	{
		MATRIX RotZ, RotY ;
		float Camera_Player_Length ;
		MV1_COLL_RESULT_POLY_DIM HRes ;
		int HitNum ;

		// ���������̉�]�͂x����]
		RotY = MGetRotY( cam.AngleH ) ;

		// ���������̉�]�͂y����] )
		RotZ = MGetRotZ( cam.AngleV ) ;

		// �J��������v���C���[�܂ł̏����������Z�b�g
		Camera_Player_Length = CAMERA_PLAYER_LENGTH ;

		// �J�����̍��W���Z�o
		// �w���ɃJ�����ƃv���C���[�Ƃ̋����������L�т��x�N�g����
		// ����������]( �y����] )���������Ɛ���������]( �x����] )���čX��
		// �����_�̍��W�𑫂������̂��J�����̍��W
		cam.Eye = VAdd( VTransform( VTransform( VGet( -Camera_Player_Length, 0.0f, 0.0f ), RotZ ), RotY ), cam.Target ) ;

		// �����_����J�����̍��W�܂ł̊ԂɃX�e�[�W�̃|���S�������邩���ׂ�
		HRes = MV1CollCheck_Capsule( stg.ModelHandle, -1, cam.Target, cam.Eye, CAMERA_COLLISION_SIZE );
		HitNum = HRes.HitNum ;
		MV1CollResultPolyDimTerminate( HRes ) ;
		if( HitNum != 0 )
		{
			float NotHitLength ;
			float HitLength ;
			float TestLength ;
			VECTOR TestPosition ;

			// �������疳���ʒu�܂Ńv���C���[�ɋ߂Â�

			// �|���S���ɓ�����Ȃ��������Z�b�g
			NotHitLength = 0.0f ;

			// �|���S���ɓ����鋗�����Z�b�g
			HitLength = Camera_Player_Length ;
			do
			{
				// �����邩�ǂ����e�X�g���鋗�����Z�b�g( ������Ȃ������Ɠ����鋗���̒��� )
				TestLength = NotHitLength + ( HitLength - NotHitLength ) / 2.0f ;

				// �e�X�g�p�̃J�������W���Z�o
				TestPosition = VAdd( VTransform( VTransform( VGet( -TestLength, 0.0f, 0.0f ), RotZ ), RotY ), cam.Target ) ;

				// �V�������W�ŕǂɓ����邩�e�X�g
				HRes = MV1CollCheck_Capsule( stg.ModelHandle, -1, cam.Target, TestPosition, CAMERA_COLLISION_SIZE );
				HitNum = HRes.HitNum ;
				MV1CollResultPolyDimTerminate( HRes ) ;
				if( HitNum != 0 )
				{
					// ���������瓖���鋗���� TestLength �ɕύX����
					HitLength = TestLength ;
				}
				else
				{
					// ������Ȃ������瓖����Ȃ������� TestLength �ɕύX����
					NotHitLength = TestLength ;
				}

				// HitLength �� NoHitLength ���\���ɋ߂Â��Ă��Ȃ������烋�[�v
			}while( HitLength - NotHitLength > 0.1f ) ;

			// �J�����̍��W���Z�b�g
			cam.Eye = TestPosition ;
		}
	}
}


// �e�p�̐[�x�L�^�摜�̏������s��
void SetupDepthImage( void )
{
	int i ;
	int j ;
	VECTOR LookAt_WorldPos ;

	// ���f���̕`��ɃI���W�i���̃V�F�[�_�[���g�p����悤�ɐݒ肷��
	MV1SetUseOrigShader( TRUE ) ;

	// �[�x�L�^�摜�ւ̕`��p�̃s�N�Z���V�F�[�_�[���Z�b�g
	SetUsePixelShader( DepthShadow_Step1_PixelShader ) ;

	// �S���ʂ̕����̐������J��Ԃ�
	for( i = 0; i < 6; i ++ )
	{
		// �`�����e�p�[�x�L�^�摜�ɂ���
		SetDrawScreen( DepthBufferGraphHandle[ i ] ) ;

		// �e�p�[�x�L�^�摜��^�����ɃN���A
		SetBackgroundColor( 255, 255, 255 ) ;
		ClearDrawScreen() ;
		SetBackgroundColor( 0, 0, 0 ) ;

		// �`�悷�鉜�s���͈͂��Z�b�g
		SetCameraNearFar( 100.0f, 15000.0f ) ;

		// �J�����̉�p��90�x�ɐݒ�
		SetupCamera_Perspective( 90.0f / 180.0f * DX_PI_F ) ;

		// �J�����̈ʒu�ƒ����_�A�J�����̏������ݒ�
		LookAt_WorldPos = VAdd( depth_lookAt[ i ], LightPos );
		SetCameraPositionAndTargetAndUpVec( LightPos, LookAt_WorldPos, depth_up[ i ] ) ;

		// �ݒ肵���J�����̃r���[�s��Ǝˉe�s����擾���Ă���
		LightCamera_ViewMatrix[ i ] = GetCameraViewMatrix() ;
		LightCamera_ProjectionMatrix[ i ] = GetCameraProjectionMatrix() ;

		// �[�x�L�^�摜�ւ̍��̃��b�V���`��p�̒��_�V�F�[�_�[���Z�b�g
		SetUseVertexShader( Normal_DepthShadow_Step1_VertexShader ) ;

		// �X�e�[�W���f���̕`��
		{
			// �V��̓V���h�E�}�b�v�ɂ͕`�悵�Ȃ�( �^���ÂɂȂ��Ă��܂����� )
			MV1SetFrameVisible( stg.ModelHandle, 5, FALSE ) ;
			MV1DrawModel( stg.ModelHandle ) ;
			MV1SetFrameVisible( stg.ModelHandle, 5, TRUE ) ;
		}

		// �[�x�L�^�摜�ւ̃X�L�j���O���b�V���`��p�̒��_�V�F�[�_�[���Z�b�g
		SetUseVertexShader( Skin4_DepthShadow_Step1_VertexShader ) ;
	
		// �v���C���[���f���̕`��
		MV1DrawModel( pl.CharaInfo.ModelHandle ) ;

		// �v���C���[�ȊO�L�������f���̕`��
		for( j = 0 ; j < NOTPLAYER_NUM ; j ++ )
		{
			MV1DrawModel( npl[ j ].CharaInfo.ModelHandle ) ;
		}
	}

	// ���f���̕`��ɃI���W�i���̃V�F�[�_�[���g�p����悤�ɂ����ݒ������
	MV1SetUseOrigShader( FALSE ) ;

	// �`���𗠉�ʂɖ߂�
	SetDrawScreen( DX_SCREEN_BACK ) ;
}

// �e�p�̐[�x�L�^�摜���g�����e�𗎂Ƃ��������܂߂����f���̕`��
void DrawModelWithDepthShadow( void )
{
	int i ;

	// �J�����̐ݒ���s��
	SetCameraPositionAndTarget_UpVecY( cam.Eye, cam.Target ) ;

	// ���f���̕`��ɃI���W�i���̃V�F�[�_�[���g�p����悤�ɐݒ肷��
	MV1SetUseOrigShader( TRUE ) ;

	// �[�x�L�^�摜���g�����e�{�|�C���g���C�g��`��p�̃s�N�Z���V�F�[�_�[���Z�b�g
	SetUsePixelShader( PointLight_DepthShadow_Step2_PixelShader ) ;

	// �e�p�[�x�L�^�摜��`�悵���Ƃ��̃J�����̃r���[�s��Ǝˉe�s���萔�ɐݒ肷��
	for( i = 0; i < 6; i++ )
	{
		SetVSConstFMtx( 43 + i * 8, LightCamera_ViewMatrix[ i ] ) ;
		SetVSConstFMtx( 47 + i * 8, LightCamera_ProjectionMatrix[ i ] ) ;
	}

	// �e�p�[�x�L�^�摜���e�N�X�`���P�`�U�ɃZ�b�g
	for( i = 0; i < 6; i++ )
	{
		SetUseTextureToShader( 1 + i, DepthBufferGraphHandle[ i ] ) ;
	}

	// �[�x�L�^�摜���g�����e�{�|�C���g���C�g��̍��̃��b�V���`��p�̒��_�V�F�[�_�[���Z�b�g
	SetUseVertexShader( Normal_PointLight_DepthShadow_Step2_VertexShader ) ;

	// �X�e�[�W��`��
	MV1DrawModel( stg.ModelHandle ) ;


	// �[�x�L�^�摜���g�����e�{�|�C���g���C�g��̃X�L�j���O���b�V���`��p�̒��_�V�F�[�_�[���Z�b�g
	SetUseVertexShader( Skin4_PointLight_DepthShadow_Step2_VertexShader ) ;

	// �v���C���[���f���̕`��
	MV1DrawModel( pl.CharaInfo.ModelHandle ) ;

	// �v���C���[�ȊO�L�������f���̕`��
	for( i = 0 ; i < NOTPLAYER_NUM ; i ++ )
	{
		MV1DrawModel( npl[ i ].CharaInfo.ModelHandle ) ;
	}

	// ���f���̕`��ɃI���W�i���̃V�F�[�_�[���g�p����悤�ɂ����ݒ������
	MV1SetUseOrigShader( FALSE ) ;

	// �g�p�e�N�X�`���̐ݒ������
	SetUseTextureToShader( 1, -1 ) ;

	// �ݒ肵���萔������
	ResetVSConstF( 43, 8 * 6 ) ;

	// �J�����̐ݒ���s��
	SetCameraPositionAndTarget_UpVecY( cam.Eye, cam.Target ) ;
}

// �`�揈��
void Render_Process( void )
{
	// �e�p�̐[�x�L�^�摜�̏������s��
	SetupDepthImage() ;

	// �`���𗠉�ʂɐݒ�
	SetDrawScreen( DX_SCREEN_BACK ) ;
	ClearDrawScreen() ;

	// �`�悷�鉜�s���͈͂��Z�b�g
	SetupCamera_Perspective( 60.0f / 180.0f * DX_PI_F ) ;
	SetCameraNearFar( 100.0f, 15000.0f ) ;

	// �J�����̐ݒ���s��
	SetCameraPositionAndTarget_UpVecY( cam.Eye, cam.Target ) ;

	// �e�p�̐[�x�L�^�摜���g�����e�𗎂Ƃ��������܂߂����f���̕`��
	DrawModelWithDepthShadow() ;

	// ���C�g�̈ʒu�ɋ���`�悷��
	SetUseLighting( FALSE ) ;
	DrawSphere3D( LightPos, 200.0f, 16, GetColor( 255,255,255 ), GetColor( 255,255,255 ), TRUE ) ;
	SetUseLighting( TRUE  ) ;
}

// WinMain
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode( TRUE ) ;

	// ���C�u�����̏�����
	if( DxLib_Init() < 0 )
		return -1 ;

	// �L�����N�^�[�̋��ʏ���������
	CharaCommon_Initialize() ;

	// �v���C���[�̏�����
	Player_Initialize() ;

	// �v���C���[�ȊO�L�����̏�����
	NotPlayer_Initialize() ;

	// �X�e�[�W�̏�����
	Stage_Initialize() ;

	// �J�����̏�����
	Camera_Initialize() ;

	// �`���𗠉�ʂɂ���
	SetDrawScreen( DX_SCREEN_BACK ) ;

	// �d�r�b�L�[��������邩�A�E�C���h�E��������܂Ń��[�v
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
	{
		// ��ʂ��N���A
		ClearDrawScreen() ;

		// ���͏���
		Input_Process() ;

		// �v���C���[�ȊO�L�����̏���
		NotPlayer_Process() ;

		// �v���C���[�̏���
		Player_Process() ;

		// �J�����̏���
		Camera_Process() ;

		// ���C�g�̈ʒu���ړ�����
		LightPosAngle += LIGHT_ANGLE_SPEED ;
		if( LightPosAngle >= DX_TWO_PI_F )
		{
			LightPosAngle -= DX_TWO_PI_F ;
		}
		LightPos.x = LightBasePos.x + sin( LightPosAngle ) * LIGHT_CENTER_DISTANCE ;
		LightPos.y = LightBasePos.y ;
		LightPos.z = LightBasePos.z + cos( LightPosAngle ) * LIGHT_CENTER_DISTANCE ;

		// �|�C���g���C�g�̐ݒ�
		ChangeLightTypePoint( LightPos, 7854.0f, 0.0f, 0.000227f, 0.0f ) ;

		// �`�揈��
		Render_Process() ;

		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip() ;
	}

	// �v���C���[�ȊO�L�����̌�n��
	NotPlayer_Terminate() ;

	// �v���C���[�̌�n��
	Player_Terminate() ;

	// �L�����N�^�[���ʏ��̌�n��
	CharaCommon_Terminate() ;

	// �X�e�[�W�̌�n��
	Stage_Terminate() ;

	// ���C�u�����̌�n��
	DxLib_End() ;

	// �\�t�g�I��
	return 0 ;
}
