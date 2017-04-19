// �V���[�e�B���O�v���O����

#include "DxLib.h"

#define TARGETNUM	(3)		// �^�[�Q�b�g�̐�
#define TARGETSIZE	(16)	// �^�[�Q�b�g�̑傫��
#define TARGETWAIT	(60)	// ��x������Ă��畜������܂ł̃t���[����

#define SHOTNUM		(9)		// �V���b�g�̐�
#define SHOTSIZE	(8)		// �V���b�g�̃T�C�Y
#define SHOTSPEED	(8)		// �V���b�g�̃X�s�[�h

#define PLAYERSPEED	(5)		// �v���C���[�̃X�s�[�h
#define PLAYERSIZE	(16)	// �v���C���[�̃T�C�Y

#define HIT3TIME	(60)	// �R�����Ƀq�b�g�������́u�R�����ɂЂ��Ƃ����v��\������t���[����


// �^�[�Q�b�g�̏��
int TargetX[TARGETNUM], TargetY[TARGETNUM] ; // ���W
int TargetWait[TARGETNUM] ; // �U�����������Ă���̏o���҂������p( 0:�o����  0�ȏ�:�o���҂� )

// �V���b�g�̏��
int ShotX[SHOTNUM], ShotY[SHOTNUM] ; // ���W
int ShotSX[SHOTNUM], ShotSY[SHOTNUM] ; // ���x
int ShotValidFlag[SHOTNUM] ; // �g�p�����ǂ����̃t���O�ϐ�( 1:�g�p��  0:�g���Ă��Ȃ� )

// ���@�̏��
int PlayerX, PlayerY ; // ���W

// �R�����q�b�g���́u�R�����q�b�g�����v�\���p�J�E���^
// ( 0:��\��  0�ȏ�:�\�� )
int Hit3Counter ;

// ���͏��
int Input, EdgeInput ;

// �V���b�g�𑝂₷( x, y : ���W   sx, sy : ���x )
void AddShot( int x, int y, int sx, int sy )
{
	int i ;
	
	// �g���Ă��Ȃ��V���b�g��T��
	for( i = 0 ; i < SHOTNUM ; i ++ )
		if( ShotValidFlag[i] == 0 ) break ;
		
	// �S���g���Ă�����ǉ����Ȃ�
	if( i == SHOTNUM ) return ;

	// ���Z�b�g
	ShotX[i] = x ;		ShotY[i] = y ;		// ���W�Z�b�g
	ShotSX[i] = sx ;	ShotSY[i] = sy ;	// ���x�Z�b�g
	ShotValidFlag[i] = 1 ;	// �g�p���ɂ���
}

// WinMain �֐�
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{
	int i, j, HitNum ;

	ChangeWindowMode( TRUE ) ; // �ꉞ�E�C���h�E���[�h�ŋN��
	if( DxLib_Init() < 0 ) return -1 ; // �c�w���C�u�����̏�����
	SetDrawScreen( DX_SCREEN_BACK ) ; // �`���𗠉�ʂɂ���
	
	// �u�R�����Ƀq�b�g�����v�\���p�J�E���^��������
	Hit3Counter = 0 ;

	// �^�[�Q�b�g�̏���������
	TargetX[0] = 320 ;	TargetY[0] = 100 ;	TargetWait[0] = 0 ;
	TargetX[1] = 100 ;	TargetY[1] = 100 ;	TargetWait[1] = 0 ;
	TargetX[2] = 540 ;	TargetY[2] = 100 ;	TargetWait[2] = 0 ;
	
	// �V���b�g�̏��̏�����
	for( i = 0 ; i < SHOTNUM ; i ++ ) ShotValidFlag[i] = 0 ; // �S���g���Ă��Ȃ����Ƃɂ���
	
	// �v���C���[�̏�񏉊���
	PlayerX = 320 ;	PlayerY = 400 ;

	// ���C�����[�v
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
	{
		ClearDrawScreen() ; // ��ʏ�����
		
		// ���͏��X�V
		{
			int inp ;
			
			inp = GetJoypadInputState( DX_INPUT_KEY_PAD1 ) ; 
			EdgeInput = inp & ~Input ;
			Input = inp ;
		}
		
		// �^�[�Q�b�g�̏���
		for( i = 0 ; i < TARGETNUM ; i ++ )
		{
			// �����҂����ǂ����ŏ����𕪊�
			if( TargetWait[i] != 0 )
			{
				TargetWait[i] -- ; // �����҂��J�E���^���O�ł͖������͂P���炷
			}
			else
			{
				// �^�[�Q�b�g�̕\��
				DrawBox( TargetX[i] - TARGETSIZE, TargetY[i] - TARGETSIZE,
						 TargetX[i] + TARGETSIZE, TargetY[i] + TARGETSIZE, GetColor( 255, 0, 0 ), TRUE ) ;
			}			
		}

		// �V���b�g�̏���
		HitNum = 0 ; // �����Ƀq�b�g��������������
		for( i = 0 ; i < SHOTNUM ; i ++ )
		{
			if( ShotValidFlag[i] == 0 ) continue ; // �g�p���ł͖�����Ή������Ȃ�
			
			ShotX[i] += ShotSX[i] ; ShotY[i] += ShotSY[i] ; // �ړ�����
			
			// ��ʊO�ɏo�Ă����疢�g�p�ɂ���
			if( ShotY[i] < -60 )
			{
				 ShotValidFlag[i] = 0 ;
				 continue ;
			}
			
			// �V���b�g�̕\��
			DrawBox( ShotX[i] - SHOTSIZE, ShotY[i] - SHOTSIZE,
					 ShotX[i] + SHOTSIZE, ShotY[i] + SHOTSIZE, GetColor( 255, 255, 255 ), TRUE ) ;
			
			// �^�[�Q�b�g�Ƃ̓����蔻��
			for( j = 0 ; j < TARGETNUM ; j ++ )
			{
				if( TargetWait[j] != 0 ) continue ; // ������Ă���܂��������Ă��Ȃ����͓����蔻����s��Ȃ�
				
				// ����
				if( ( ( ShotX[i] > TargetX[j] - TARGETSIZE && ShotX[i] < TargetX[j] + TARGETSIZE ) &&
					  ( ShotY[i] > TargetY[j] - TARGETSIZE && ShotY[i] < TargetY[j] + TARGETSIZE ) ) ||
				  	( ( TargetX[j] > ShotX[i] - SHOTSIZE && TargetX[j] < ShotX[i] + SHOTSIZE ) &&
					  ( TargetY[j] > ShotY[i] - SHOTSIZE && TargetY[j] < ShotY[i] + SHOTSIZE ) ) )
				{
					ShotValidFlag[i] = 0 ; // �q�b�g������V���b�g�����g�p��
					TargetWait[j] = TARGETWAIT ;  // �^�[�Q�b�g�͕����҂���
					HitNum ++ ; // �����Ƀq�b�g���������P���₷
				}
			}
		}

		// �����Ƀq�b�g���������R��������u�R�����Ƀq�b�g�����v�J�E���^���Z�b�g
		if( HitNum == 3 )
		{
			Hit3Counter = HIT3TIME ;
		}
		
		// �u�R�����Ƀq�b�g�����v�\������
		if( Hit3Counter != 0 )
		{
			DrawString( 0, 0, "�R�����Ƀq�b�g����", GetColor( 255, 255, 255 ) ) ; // �R�����Ƀq�b�g�����m�点��\��
			Hit3Counter -- ; // �\�����̓J�E���^����Ɍ��炷
		}

		// �v���C���[�̏���
		if( Input & PAD_INPUT_LEFT ) PlayerX -= PLAYERSPEED ;
		if( Input & PAD_INPUT_RIGHT ) PlayerX += PLAYERSPEED ;
		if( EdgeInput & PAD_INPUT_A )
		{
			// �R�v�`�x�C����
			AddShot( PlayerX, PlayerY, 0, -8 ) ;
			AddShot( PlayerX, PlayerY, 6, -8 ) ;
			AddShot( PlayerX, PlayerY, -6, -8 ) ;
		}
		DrawBox( PlayerX - PLAYERSIZE, PlayerY - PLAYERSIZE,
				 PlayerX + PLAYERSIZE, PlayerY + PLAYERSIZE, GetColor( 0, 255, 0 ), TRUE ) ;
	 
		// ��ʂ̍X�V
		ScreenFlip() ;
	}

	DxLib_End() ; // �c�w���C�u�����̌�n��
	
	return 0 ; // �\�t�g�I��
}




