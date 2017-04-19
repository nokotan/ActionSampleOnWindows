#include "DxLib.h"
#include <math.h>

#define MAX_L	20					// ���[�U�[�̍ő吔��`
#define PI		3.1415926535897932		// ��

#define LINE_MAXNUM	1000				// �`�悷����̍ő吔

// �f�[�^�^�錾

// ���C���`��p�\���̐錾
typedef struct tugLINE
{
	int x , y ;					// �`�����C���̍��W
	double Angle ;					// ���C���̌���
	int Counter ;					// �`�����C���̐F����p�l
} LINE , *LPLINE ;

// ���[�U�[�\���̌^�錾
typedef struct tugLASER
{
	int x , y ;					// ���݂̍��W
	int sx , sy ;					// ���݂̑��x

	int LogNum ;					// �L�^�����O�Ղ̐�
	double Angle ;					// �i��ł���p�x
	int Counter ;					// �ǔ����͂��߂Ă���o�߂�������

	LINE Line[LINE_MAXNUM] ;			// ���[�U�[�̃��C���f�[�^
	int LineNum ;					// �\������Ă��郉�C���̐�

	int ValidFlag ;					// ���̃f�[�^���g�p�����t���O
} LASER , *LPLASER ;

// �f�[�^��`

int Hx , Hy ;						// �C��̍��W
int Hm ;						// �C��̈ړ�����
int Hsc ;						// �C��̃V���b�g�Ԋu�J�E���^
int Px , Py ;						// ���@�̍��W

LASER Ls[MAX_L] ;					// �z�[�~���O���[�U�[�̃f�[�^
int Lg, Lg2 ;						// ���[�U�[�̃O���t�B�b�N�n���h��


// WinMain�֐�
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine, int nCmdShow )
{
	int Key ;
	LONGLONG Time ;
	int i ;

	// ��ʃ��[�h�̐ݒ�
	SetGraphMode( 640 , 480 , 16 ) ;

	// �c�w���C�u��������������
	if( DxLib_Init() == -1 )
	{
		// �G���[���N�����璼���ɏI��
		return -1;
	}

	// �`���𗠉�ʂɃZ�b�g
	SetDrawScreen( DX_SCREEN_BACK ) ;

	// ����������
	{
		// ���[�U�[�O���t�B�b�N�̃��[�h
		Lg = LoadGraph( "Laser.bmp" ) ;
		Lg2 = LoadGraph( "Laser2.bmp" ) ;

		// ���@�̍��W�Z�b�g
		Px = 320 ; Py = 200 ;

		// �C��̍��W�Z�b�g
		Hx = 320 ; Hy = 30 ;

		// �C��̈ړ������Z�b�g
		Hm = 3 ;

		// �C��̈ړ��Ԋu�J�E���^�Z�b�g
		Hsc = 30 ;

		// ���[�U�[�f�[�^�̏�����
		for( i = 0 ; i < MAX_L ; i ++ )
			Ls[ i ].ValidFlag = 0 ;
	}

	// �Q�[�����[�v
	Time = GetNowHiPerformanceCount() + 1000000 / 60 ;
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
	{
		// �v���C���[�̈ړ�����
		{
			// ���͎擾
			Key = GetJoypadInputState( DX_INPUT_KEY_PAD1 ) ;

			if( Key & PAD_INPUT_RIGHT ) Px += 5 ;	// �E�������Ă�����E�ɐi��
			if( Key & PAD_INPUT_LEFT ) Px -= 5 ;	// ���������Ă����獶�ɐi��

			if( Key & PAD_INPUT_UP ) Py -= 5 ;	// ��������Ă������ɐi��
			if( Key & PAD_INPUT_DOWN ) Py += 5 ;	// ���������Ă����牺�ɐi��

			// ��ʊO�ɏo�Ă�����␳
			if( Px > 640 - 16 ) Px = 640 - 16 ;
			if( Px < 0 ) Px = 0 ;

			if( Py > 480 - 16 ) Py = 480 - 16 ;
			if( Py < 0 ) Py = 0 ;
		}

		// ���[�U�[�̈ړ�����
		for( i = 0 ; i < MAX_L ; i ++ )
		{
			int xb , yb ;

			// ���[�U�[�f�[�^��������������X�L�b�v
			if( Ls[ i ].ValidFlag == 0 ) continue ;

			// �Ə��ɓ������Ă����烌�[�U�[�f�[�^�𖳌��ɂ���
			if( ( Ls[ i ].x > Px && Ls[ i ].x < Px + 32 ) &&
				( Ls[ i ].y > Py && Ls[ i ].y < Py + 32 ) )
			{
				Ls[ i ].ValidFlag = 0 ;
				continue  ;
			}

			// �ǔ��J�E���^���K��l�ɗ��Ă��Ȃ���Βǔ�����
			if( Ls[ i ].Counter < 100 )
			{
				double ax , ay , bx , by ;
				double ar , br ;

				// bx,by �����̐i��ł������ ax,ay �{���i�ނׂ�����  
				bx = cos( Ls[ i ].Angle ) ;
				by = sin( Ls[ i ].Angle ) ;
				ax = Px - Ls[ i ].x + 16 ;
				ay = Py - Ls[ i ].y + 16 ;

				// �x�N�g��b �� a�̐�Βl�����߂�
				br = sqrt( bx * bx + by * by ) ;
				ar = sqrt( ax * ax + ay * ay ) ;

				// �O�ς𗘗p���������Ə����Ɍ�����
				Ls[ i ].Angle += PI / 180 * ( ( bx * ay - by * ax ) / ( br * ar ) ) * 5 ;
			}

			// �ǔ��J�E���^���Z
			Ls[ i ].Counter ++ ;

			// ���x��ύX����
			Ls[ i ].sx = ( int )( cos( Ls[ i ].Angle ) * 1000 );
			Ls[ i ].sy = ( int )( sin( Ls[ i ].Angle ) * 1000 );

			// �ړ��O�̃A�h���X��ۑ�
			xb = Ls[ i ].x ;
			yb = Ls[ i ].y ;

			// �ړ�����
			Ls[ i ].x = ( Ls[ i ].x * 100 + Ls[ i ].sx ) / 100 ;
			Ls[ i ].y = ( Ls[ i ].y * 100 + Ls[ i ].sy ) / 100 ;

			// ���݂̏�Ԃ����C���f�[�^�ɕϊ�
			{
				int j ;

				if( Ls[ i ].LineNum != LINE_MAXNUM )
				{
					// ���C�������Z�b�g����
					j = Ls[ i ].LineNum ;

					// ���W�̃Z�b�g
					Ls[ i ].Line[ j ].x = xb ; Ls[ i ].Line[ j ].y = yb ;

					// �p�x���Z�b�g
					{
						double r ;

						r = sqrt( Ls[ i ].sx * Ls[ i ].sx + Ls[ i ].sy * Ls[ i ].sy ) ;
						Ls[ i ].Line[ j ].Angle = atan2( Ls[ i ].sy , Ls[ i ].sx ) ;
					}

					// �F����J�E���^��������
					Ls[ i ].Line[ j ].Counter = 0 ;

					// ���C���̐��𑝂₷
					Ls[ i ].LineNum ++ ; 
				}
			}

			// ��ʊO�ɏo�Ă����烌�[�U�[�f�[�^�𖳌��ɂ���
			if( Ls[ i ].x < -100 || Ls[ i ].x > 740 ||
				Ls[ i ].y < -100 || Ls[ i ].y > 580 ) Ls[ i ].ValidFlag = 0 ;
		}

		// �C��̈ړ�����
		{
			Hx += Hm ;

			// ��ʒ[�܂ŗ��Ă����甽�]
			if( Hx > 640 - 16 || Hx < 0 ) Hm *= -1 ;
			
			// �V���b�g�J�E���^�����炷
			Hsc -- ;

			// �J�E���^���O�ɂȂ��Ă����烌�[�U�[����
			if( Hsc == 0 )
			{
				// �g���Ă��Ȃ����[�U�[�f�[�^��T��
				for( i = 0 ; i < MAX_L ; i ++ )
				{
					if( Ls[ i ].ValidFlag == 0 && Ls[ i ].LineNum == 0 ) break ;
				}
		
				// �����g���Ă��Ȃ����[�U�[�f�[�^����������V���b�g���o��
				if( i != MAX_L )
				{
					// ���[�U�[�̈ʒu��ݒ�
					Ls[ i ].x = Hx + 16 ;
					Ls[ i ].y = Hy + 16 ;

					// ���[�U�[�̑��x��ݒ�
					Ls[ i ].sx = 0 ;
					Ls[ i ].sy = 0 ;

					// �p�x���Z�b�g
					Ls[ i ].Angle = PI / 2 ;

					// �ǔ��J�E���^���Z�b�g
					Ls[ i ].Counter = 0 ;

					// ���[�U�[�f�[�^���g�p���ɃZ�b�g
					Ls[ i ].ValidFlag = 1 ;
				}

				// ���ˊԊu�J�E���^�l���Z�b�g
				Hsc = 30 ;
			}
		}

		// �`�揈��
		{
			int j , DeleteNum, Bx[2], By[2], Nx[2], Ny[2] ;

			// ��ʂ̏�����
			ClearDrawScreen() ;

			// ���Z�u�����h�ɃZ�b�g
			SetDrawBlendMode( DX_BLENDMODE_ADD, 255 ) ;

			// ���C���̕`��
			for( j = 0 ; j < MAX_L ; j ++ )
			{
				DeleteNum = 0 ;

				// �ŏ��̍��W���Z�b�g
				Bx[0] = (int)( cos( PI / 2 + Ls[ j ].Line[ 0 ].Angle ) * 8 + Ls[ j ].Line[ 0 ].x ) ;
				By[0] = (int)( sin( PI / 2 + Ls[ j ].Line[ 0 ].Angle ) * 8 + Ls[ j ].Line[ 0 ].y ) ;

				Bx[1] = (int)( cos( -PI / 2 + Ls[ j ].Line[ 0 ].Angle ) * 8 + Ls[ j ].Line[ 0 ].x ) ;
				By[1] = (int)( sin( -PI / 2 + Ls[ j ].Line[ 0 ].Angle ) * 8 + Ls[ j ].Line[ 0 ].y ) ;

				for( i = 0 ; i < Ls[ j ].LineNum - DeleteNum ; i ++ )
				{
					// ���W�̎Z�o
					Nx[0] = (int)( cos( PI / 2 + Ls[ j ].Line[ i ].Angle ) * 8 + Ls[ j ].Line[ i ].x ) ;
					Ny[0] = (int)( sin( PI / 2 + Ls[ j ].Line[ i ].Angle ) * 8 + Ls[ j ].Line[ i ].y ) ;

					Nx[1] = (int)( cos( -PI / 2 + Ls[ j ].Line[ i ].Angle ) * 8 + Ls[ j ].Line[ i ].x ) ;
					Ny[1] = (int)( sin( -PI / 2 + Ls[ j ].Line[ i ].Angle ) * 8 + Ls[ j ].Line[ i ].y ) ;

					// ���C���̕`��
					DrawModiGraph( Nx[0], Ny[0], Bx[0], By[0], 
									Bx[1], By[1], Nx[1], Ny[1],  Lg2, TRUE ) ;

					// �����J�E���^���K��l�ɒB���Ă����畁�i�ƂP�W�O�x��]�����O���t�B�b�N��
					// �`�悵�A���̌ド�C���f�[�^�𖳌��ɂ���
					if( Ls[ j ].Line[ i ].Counter == 64 )
					{
						// �폜����f�[�^�̐�������₷
						DeleteNum ++ ;

						// �f�[�^���l�߂�
						MoveMemory( &Ls[ j ].Line[ 0 ] , &Ls[ j ].Line[ 1 ] ,
										sizeof( LINE ) * ( Ls[ j ].LineNum - DeleteNum ) ) ;

						// �l�߂��̂Ŏ��̃f�[�^�����̔z��ԍ� i �Ɠ����Ȃ�
						// �̂ŁA�̏���
						i -- ;

					}
					else
					{
						// �J�E���^�����Z����
						Ls[ j ].Line[ i ].Counter ++  ;
					}

					// ���W�̕ۑ�
					Bx[0] = Nx[0] ;
					By[0] = Ny[0] ;
					Bx[1] = Nx[1] ;
					By[1] = Ny[1] ;
				}
				Ls[ j ].LineNum -= DeleteNum ;
			}

			// �u�����h�����ɃZ�b�g
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 ) ;

			// �`��P�x���Z�b�g
			SetDrawBright( 255 , 255 , 255 ) ; 

			// �v���[���[�̕`��
			DrawBox( Px , Py , Px + 32 , Py + 32 , GetColor( 255 , 255 , 255 ) , TRUE ) ;

			// �C��̕`��
			DrawBox( Hx - 8 , Hy - 8 , Hx + 8 , Hy + 8 , GetColor( 255 , 255 , 0 ) , TRUE ) ;

			// ����ʂ̓��e��\��ʂɔ��f
			ScreenFlip() ;

			// ���ԑ҂�
			while( GetNowHiPerformanceCount() < Time ){}
			Time += 1000000 / 60 ;
		}
	}

	DxLib_End() ;				// �c�w���C�u�����g�p�̏I������

	return 0 ;				// �\�t�g�̏I��
}
