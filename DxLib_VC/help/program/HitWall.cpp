// ��]�Ǔ����蔻��v���O����
//
// Create 04/06/04
//
// �@��]����ǂƂ̓����蔻������Ă���v���O�����ł��B
// �@��ɂ��Ă��鎖�͕ǂƂȂ���ƁA�����蔻����������I�u�W�F�N�g�̈ړ��O�ƈړ�������Ԑ��Ƃ̌�������ł��B
// �@�����Ƀx�N�g���̒m�����K�v�ƂȂ�܂��B

#include "DxLib.h"
#include <stdio.h>
#include <math.h>

#define PI			(3.14159265358)		// ��
#define PI_2		(3.14159265358)		// �Q��
#define BOXSIZE		(128)				// ���̑傫��
#define BOXX		(320)				// ���̂w���W
#define BOXY		(240)				// ���̂x���W
#define ROTSPEED	(PI_2 / 180)		// ���̉�]���x
#define SPEED		(8.0F)				// �_�̑��x
#define HITLENGTH	(4.0F)				// ���̕ǂƂ̓����蔻��̕�

float rot ;				// ���̉�]�p�x
float x, y, sx, sy ;	// ���̒��𓮂��_�̍��W�Ƒ��x

// �񎟌��x�N�g���\����
typedef struct tagVECT
{
	float x, y ;
} VECT ;

//  HitCheck �֐����́w�����ɐ����Ɍ����_�����߂鉉�Z�x�ōs���Ă��邱�ƁAx�����߂���
//�@�Ȃ��@�w*�x�F�X�J���[�{�@�@�w�E�x�F���ρ@�@�w�啶���x�F�x�N�g���@�@�w�������x�F�X�J���[�l
//
// P1:�����̎n�_�@V1:�����̌����@P2:��������������������̎n�_(���͏I�_)
// ( P1 + x * V1 ) �FP2 ���� �����ɐ����Ɍ����_�̍��W 
//
// ( ( P1 + x * V1 ) - P2 )�EV1 = 0
// ( x * V1 + P1 - P2 )�EV1 = 0 
// x * V1�EV1 + P1�EV1 - P2�EV1 = 0  (�� V1�EV1 = 1 �Ȃ̂� x * V1�EV1 = x )
// x = P1�EV1 - P2�EV1
// x = V1�E( P1 - P2 )
//
// �Ƃ����킯�ŁA�w��������`�����܂Łx�ł� x = V1�E( P1 - P2 ) ������Ă��܂�

// ���Ɛ��̌�������
// l1x1,l1y1:�����P�̎n�_�@�@l1x2,l1y2:�����P�̏I�_
// l2x1,l2y1:�����Q�̎n�_�@�@l2x2,l2y2:�����Q�̏I�_
bool HitCheck( float l1x1, float l1y1, float l1x2, float l1y2,
				float l2x1, float l2y1, float l2x2, float l2y2 )
{
	VECT p1, v1, p2, v3, t ;
	float x, z1, z2, len ;

	// �����̎n�_�ƌ������Z�o
	p1.x = l1x1 ;				p1.y = l1y1 ;			// �����̎n�_
	v1.x = l1x2 - l1x1 ;		v1.y = l1y2 - l1y1 ;	// ����

	len = sqrt( v1.x * v1.x + v1.y * v1.y ) ;			// ���K��
	v1.x /= len ;				v1.y /= len ;


	// ������������������̎n�_�ƁA�n�_���璼���ɐ����Ɍ���钼����̓_�̍��W�����߂�
	p2.x = l2x1 ;				p2.y = l2y1 ;				// �����蔻������������̎n�_

	// �����ɐ����Ɍ����_�����߂鉉�Z
	t.x = p2.x - p1.x ;			t.y = p2.y - p1.y ;			// ��������
	x = v1.x * t.x + v1.y * t.y ;							// (�����̍s�œ��ς��Ă܂�) 
															// 
	v3.x = p1.x + v1.x * x ;	v3.y = p1.y + v1.y * x ;	// �����܂�

	t.x = v3.x - p2.x ;			t.y = v3.y - p2.y ;					// �n�_���狁�߂�������̓_�ւ̃x�N�g�����Z�o
	if( sqrt( t.x * t.x + t.y * t.y ) < HITLENGTH ) return true ;	// ���߂��_�܂ł̋����� HITLENGTH �ȉ�����������ɓ����������ɂ���

	z1 = v1.x * t.y - v1.y * t.x ;			// �n�_���狁�߂��_�ւ̃x�N�g���ƁA�����̌����Ƃ̊O�ς����߂Ă����E�E�E�@


	// ������������������̏I�_�ƁA�I�_���璼���ɐ����Ɍ���钼����̓_�̍��W�����߂�
	p2.x = l2x2 ;				p2.y = l2y2 ;				// �����蔻������������̏I�_

	// �����ɐ����Ɍ����_�����߂鉉�Z
	t.x = p2.x - p1.x ;			t.y = p2.y - p1.y ;			// ��������
	x = v1.x * t.x + v1.y * t.y ;							// (�����̍s�œ��ς��Ă܂�) 
															// 
	v3.x = p1.x + v1.x * x ;	v3.y = p1.y + v1.y * x ;	// �����܂�

	t.x = v3.x - p2.x ;			t.y = v3.y - p2.y ;					// �I�_���狁�߂�������̓_�ւ̃x�N�g�����Z�o
	if( sqrt( t.x * t.x + t.y * t.y ) < HITLENGTH ) return true ;	// ���߂��_�܂ł̋����� HITLENGTH �ȉ�����������ɓ����������ɂ���

	z2 = v1.x * t.y - v1.y * t.x ;			// �I�_���狁�߂��_�ւ̃x�N�g���ƁA�����̌����Ƃ̊O�ς����߂Ă����E�E�E�A


	// �@�ƇA�ŋ��߂��x�N�g���̌������Ⴄ�ꍇ�͌�����������������ƌ��������؋�
	return ( z1 >= 0.0F && z2 < 0.0F ) || ( z2 >= 0.0F && z1 < 0.0F ) ;
}


// WinMain �֐�
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{
	float bx, by, px[4], py[4], f ;
	int i ;

	ChangeWindowMode( TRUE ) ;						// �E�C���h�E���[�h�ŋN��
	if( DxLib_Init() < 0 ) return -1 ;				// �c�w���C�u����������

	rot = 0.0F ;									// ���̉�]�p�x��������
	x = 320 ;		y = 240 ;						// �_�̈ʒu��������
	sx = rand() * SPEED / RAND_MAX - SPEED / 2 ;	// �_�̑��x��������
	sy = rand() * SPEED / RAND_MAX - SPEED / 2 ;

	// ���C�����[�v( ���b�Z�[�W���������s���邩�A�d�r�b�L�[���������܂Ń��[�v )
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
	{
		ClearDrawScreen() ;		// ��ʏ�����

		bx = x ;	by = y ;	// �ړ��O�̍��W��ۑ����Ă���
		x += sx ;	y += sy ;	// �_�̍��W�̈ړ�

		rot += ROTSPEED ;								// ���̉�]�p�x��ύX
		if( rot > PI_2 ) rot -= PI_2 ;					// ������Ă�����␳������
		px[0] = - BOXSIZE ;		py[0] = - BOXSIZE ;		// ���̎l���̍��W�̏����l���Z�b�g
		px[1] = + BOXSIZE ;		py[1] = - BOXSIZE ;
		px[2] = + BOXSIZE ;		py[2] = + BOXSIZE ;
		px[3] = - BOXSIZE ;		py[3] = + BOXSIZE ;
		for( i = 0 ; i < 4 ; i ++ )						// ���̎l���̍��W�����݂̉�]�p�x�ɍ��킹�ĉ�]
		{
			f =		px[i] * cos( rot ) - py[i] * sin( rot ) ;
			py[i] =	px[i] * sin( rot ) + py[i] * cos( rot ) ;
			px[i] = f ;

			px[i] += BOXX ;		// ��]������A�\�����������W�Ɉړ�
			py[i] += BOXY ;
		}

		// ���̎l�ӂƓ_�Ƃ̓����蔻��(�_�̈ړ��O�ƈړ�������Ԑ������̎l�ӂƌ����������𔻒�)
		if( HitCheck( px[0], py[0], px[1], py[1], bx, by, x, y ) == true ||
			HitCheck( px[1], py[1], px[2], py[2], bx, by, x, y ) == true ||
			HitCheck( px[2], py[2], px[3], py[3], bx, by, x, y ) == true ||
			HitCheck( px[3], py[3], px[0], py[0], bx, by, x, y ) == true )
		{
			// ��������(��������)������_�̈ʒu�����ɖ߂�
			x = 320 ;		y = 240 ;						// �ʒu������
			sx = rand() * SPEED / RAND_MAX - SPEED / 2 ;	// ���x�ݒ�
			sy = rand() * SPEED / RAND_MAX - SPEED / 2 ;
		}

		// ���̎l�ӂ�`��
		DrawLine( px[0], py[0], px[1], py[1], GetColor( 255, 255, 255 ) ) ;
		DrawLine( px[1], py[1], px[2], py[2], GetColor( 255, 255, 255 ) ) ;
		DrawLine( px[2], py[2], px[3], py[3], GetColor( 255, 255, 255 ) ) ;
		DrawLine( px[3], py[3], px[0], py[0], GetColor( 255, 255, 255 ) ) ;

		// �_��`��
		DrawPixel( x, y, GetColor( 255, 255, 255 ) ) ;

		// ��ʂ��X�V
		ScreenFlip() ;
	}

	DxLib_End() ;	// �c�w���C�u�����̌�n��

	return 0 ;		// �\�t�g�I��
}


