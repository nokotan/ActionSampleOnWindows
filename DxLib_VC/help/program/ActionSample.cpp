// �A�N�V�����T���v���v���O����
//
// Create 10/12/03
//
// �@�Q�c�A�N�V�����̊�{�A�}�b�v�Ƃ̓����蔻��ł��B
// �@��ʊO�ɏo���猩���Ȃ��Ƃ���ŉi���ɗ����Ă䂫�܂��̂ł����ӂ��������B(��)
// �@���ۂ̓L�����N�^���^�l�p�Ƃ������Ƃ͂Ȃ��̂ŁA���낢��Ɖ��ǂ�������
// �K�v������܂��B

#include "DxLib.h"

#define SCREEN_WIDTH     (640)                          // ��ʂ̉���
#define SCREEN_HEIGHT    (480)                          // ��ʂ̏c��
#define CHIP_SIZE        (32)                           // ��̃`�b�v�̃T�C�Y
#define MAP_WIDTH        (SCREEN_WIDTH / CHIP_SIZE)     // �}�b�v�̉���
#define MAP_HEIGHT       (SCREEN_HEIGHT / CHIP_SIZE)    // �}�b�v�̏c��

#define G                (0.3F)                         // �L�����Ɋ|����d�͉����x
#define JUMP_POWER       (9.0F)                         // �L�����̃W�����v��
#define SPEED            (5.0F)                         // �L�����̈ړ��X�s�[�h
#define CHAR_SIZE        (30)                           // �v���C���[�̃T�C�Y

// �}�b�v�f�[�^
char MapData[MAP_HEIGHT][MAP_WIDTH] =
{
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
    1,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
    1,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
    1,0,0,1,1, 1,1,0,0,0, 0,0,0,0,0, 0,0,1,0,1,

    1,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,1,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,1,1,0, 0,0,1,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
    1,0,0,0,0, 0,0,1,1,0, 0,0,0,0,0, 1,0,0,0,1,

    1,0,0,0,0, 1,1,1,1,1, 0,0,0,0,1, 1,0,0,0,1,
    1,0,0,0,0, 1,1,1,1,1, 0,0,0,1,1, 1,0,0,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 1,0,0,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
    1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
} ;

float PlX, PlY ;            // �v���C���[�̍��W(���S���W)
float PlDownSp ;            // �v���C���[�̗������x
char PlJumpFlag ;           // �v���C���[���W�����v�����A�̃t���O

int Input, EdgeInput ;      // ���͏��
int FrameStartTime ;        // �U�O�e�o�r�Œ�p�A���ԕۑ��p�ϐ�



// �}�b�v�`�b�v�̒l���擾����֐�
int GetChipParam( float X, float Y ) ;

// �L�����N�^���}�b�v�Ƃ̓����蔻����l�����Ȃ���ړ�����֐�
int CharMove( float *X, float *Y, float *DownSP,
                    float MoveX, float MoveY, float Size, char *JumpFlag ) ;

// �A�N�V�����T���v���v���O�������C���֐�
int ActMain( void ) ;

// �}�b�v�Ƃ̓����蔻��( �߂�l 0:������Ȃ�����  1:���ӂɓ�������  2:�E�ӂɓ�������
//                                                3:��ӂɓ�������  4:���ӂɓ�������
// ���ӁFMoveX �� MoveY �A�ǂ������Е����O����Ȃ��Ƃ܂Ƃ��ɓ��삵�܂���(��)
int MapHitCheck( float X, float Y, float *MoveX, float *MoveY ) ;




// WinMain�֐�
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow )
{
    ChangeWindowMode( TRUE ) ;

    // �c�w���C�u�����̏�����
    if( DxLib_Init() == -1 ) return -1 ;

    // �A�N�V�����Q�[���̃��C���֐����Ă�
    ActMain() ;

    // �c�w���C�u�����̌�n��
    DxLib_End() ;

    // �I��
    return 0 ;
}

// �A�N�V�����T���v���v���O�������C��
int ActMain( void )
{
    // �`���𗠉�ʂɃZ�b�g
    SetDrawScreen( DX_SCREEN_BACK ) ;

    // ���������M����҂��Ȃ�
    SetWaitVSyncFlag( FALSE ) ;

    // �v���C���[�̍��W��������
    PlX = 320.0F ;        PlY = 240.0F ;

    // �v���C���[�̗������x��������
    PlDownSp = 0.0F ;

    // �W�����v���t���O��|��
    PlJumpFlag = FALSE ;

    // ���͏�Ԃ̏�����
    Input = 0 ;
    EdgeInput = 0 ;

    // �U�O�e�o�r�Œ�p�A���ԕۑ��p�ϐ������݂̃J�E���g�l�ɃZ�b�g
    FrameStartTime = GetNowCount() ;

    // ���C�����[�v�J�n�A�d�r�b�L�[�ŊO�ɏo��
    while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
    {
        // ��ʂ̃N���A
        ClearDrawScreen() ;

        // �P/�U�O�b���܂ő҂�
        while( GetNowCount() - FrameStartTime < 1000 / 60 ){}

        // ���݂̃J�E���g�l��ۑ�
        FrameStartTime = GetNowCount() ;

        // ���͏�Ԃ��X�V
        {
            int i ;

            // �p�b�h�P�ƃL�[�{�[�h������͂𓾂�
            i = GetJoypadInputState( DX_INPUT_KEY_PAD1 ) ;

            // �G�b�W����������͂��Z�b�g
            EdgeInput = i & ~Input ;

            // ���͏�Ԃ̕ۑ�
            Input = i ;
        }

        // �v���C���[�̈ړ�����
        {
            float MoveX, MoveY ;

            // �ړ��ʂ̏�����
            MoveX = 0.0F ;
            MoveY = 0.0F ;

            // ���E�̈ړ�������
            if( ( Input & PAD_INPUT_LEFT ) != 0 ) MoveX -= SPEED ;
            if( ( Input & PAD_INPUT_RIGHT ) != 0 ) MoveX += SPEED ;

            // �n�ɑ��������Ă���ꍇ�̂݃W�����v�{�^��(�{�^���P or �y�L�[)������
            if( PlJumpFlag == FALSE && ( EdgeInput & PAD_INPUT_A ) != 0 )
            {
                PlDownSp = -JUMP_POWER ;
                PlJumpFlag = TRUE ;
            }

            // ��������
            PlDownSp += G ;

            // �������x���ړ��ʂɉ�����
            MoveY = PlDownSp ;

            // �ړ��ʂɊ�Â��ăL�����N�^�̍��W���ړ�
            CharMove( &PlX, &PlY, &PlDownSp, MoveX, MoveY, CHAR_SIZE, &PlJumpFlag ) ;
        }

        // �}�b�v�̕`��
        {
            int i, j ;

            for( i = 0 ; i < MAP_HEIGHT ; i ++ )
            {
                for( j = 0 ; j < MAP_WIDTH ; j ++ )
                {
                    // �P�͓����蔻��`�b�v��\���Ă���̂łP�̂Ƃ��낾���`��
                    if( MapData[i][j] == 1 )
                    {
                        DrawBox( j * CHIP_SIZE,                i * CHIP_SIZE,
                                 j * CHIP_SIZE + CHIP_SIZE,    i * CHIP_SIZE + CHIP_SIZE,
                                 GetColor( 255, 255, 255 ), TRUE ) ;
                    }
                }
            }
        }

        // �L�����N�^�̕`��
        DrawBox( ( int )( PlX - CHAR_SIZE * 0.5F ),     ( int )( PlY - CHAR_SIZE * 0.5F ),
                 ( int )( PlX + CHAR_SIZE * 0.5F ) + 1, ( int )( PlY + CHAR_SIZE * 0.5F ) + 1,
                    GetColor( 255, 0, 0 ), TRUE ) ;

        // ��ʂ̍X�V
        ScreenFlip() ;
    }

    // �I��
    return 0 ;
}

// �L�����N�^���}�b�v�Ƃ̓����蔻����l�����Ȃ���ړ�����
int CharMove( float *X, float *Y, float *DownSP,
                    float MoveX, float MoveY, float Size, char *JumpFlag )
{
    float Dummy = 0.0F ;
    float hsize ;

    // �L�����N�^�̍���A�E��A�����A�E�������������蔻��̂���
    // �}�b�v�ɏՓ˂��Ă��邩���ׁA�Փ˂��Ă�����␳����

    // �����̃T�C�Y���Z�o
    hsize = Size * 0.5F ;

    // �悸�㉺�ړ����������Ń`�F�b�N
    {
        // �����̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
        if( MapHitCheck( *X - hsize, *Y + hsize, &Dummy, &MoveY ) == 3 ) *DownSP = 0.0F ;

        // �E���̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
        if( MapHitCheck( *X + hsize, *Y + hsize, &Dummy, &MoveY ) == 3 ) *DownSP = 0.0F ;

        // ����̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
        if( MapHitCheck( *X - hsize, *Y - hsize, &Dummy, &MoveY ) == 4 ) *DownSP *= -1.0F ;

        // �E��̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
        if( MapHitCheck( *X + hsize, *Y - hsize, &Dummy, &MoveY ) == 4 ) *DownSP *= -1.0F ;

        // �㉺�ړ����������Z
        *Y += MoveY ;
    }

    // ��ɍ��E�ړ����������Ń`�F�b�N
    {
        // �����̃`�F�b�N
        MapHitCheck( *X - hsize, *Y + hsize, &MoveX, &Dummy ) ;

        // �E���̃`�F�b�N
        MapHitCheck( *X + hsize, *Y + hsize, &MoveX, &Dummy ) ;

        // ����̃`�F�b�N
        MapHitCheck( *X - hsize, *Y - hsize, &MoveX, &Dummy ) ;

        // �E��̃`�F�b�N
        MapHitCheck( *X + hsize, *Y - hsize, &MoveX, &Dummy ) ;

        // ���E�ړ����������Z
        *X += MoveX ;
    }

    // �ڒn����
    {
        // �L�����N�^�̍����ƉE���̉��ɒn�ʂ����邩���ׂ�
        if( GetChipParam( *X - Size * 0.5F, *Y + Size * 0.5F + 1.0F ) == 0 && 
            GetChipParam( *X + Size * 0.5F, *Y + Size * 0.5F + 1.0F ) == 0 )
        {
            // ���ꂪ����������W�����v���ɂ���
            *JumpFlag = TRUE ;
        }
        else
        {
            // ���ꂪ�݂�����ڒn���ɂ���
            *JumpFlag = FALSE ;
        }
    }

    // �I��
    return 0 ;
}


// �}�b�v�Ƃ̓����蔻��( �߂�l 0:������Ȃ�����  1:���ӂɓ�������  2:�E�ӂɓ�������
//                                                3:��ӂɓ�������  4:���ӂɓ�������
// ���ӁFMoveX �� MoveY �A�ǂ������Е����O����Ȃ��Ƃ܂Ƃ��ɓ��삵�܂���(��)
int MapHitCheck( float X, float Y,
                    float *MoveX, float *MoveY )
{
    float afX, afY ;

    // �ړ��ʂ𑫂�
    afX = X + *MoveX ;
    afY = Y + *MoveY ;

    // �����蔻��̂���u���b�N�ɓ������Ă��邩�`�F�b�N
    if( GetChipParam( afX, afY ) == 1 )
    {
        float blx, bty, brx, bby ;

        // �������Ă�����ǂ��痣���������s��

        // �u���b�N�̏㉺���E�̍��W���Z�o
        blx = (float)( (int)afX / CHIP_SIZE ) * CHIP_SIZE ;        // ���ӂ� X ���W
        brx = (float)( (int)afX / CHIP_SIZE + 1 ) * CHIP_SIZE ;    // �E�ӂ� X ���W

        bty = (float)( (int)afY / CHIP_SIZE ) * CHIP_SIZE ;        // ��ӂ� Y ���W
        bby = (float)( (int)afY / CHIP_SIZE + 1 ) * CHIP_SIZE ;    // ���ӂ� Y ���W

        // ��ӂɓ������Ă����ꍇ
        if( *MoveY > 0.0F )
        {
            // �ړ��ʂ�␳����
            *MoveY = bty - Y - 1.0F  ;

            // ��ӂɓ��������ƕԂ�
            return 3 ;
        }

        // ���ӂɓ������Ă����ꍇ
        if( *MoveY < 0.0F )
        {
            // �ړ��ʂ�␳����
            *MoveY = bby - Y + 1.0F ;

            // ���ӂɓ��������ƕԂ�
            return 4 ;
        }

        // ���ӂɓ������Ă����ꍇ
        if( *MoveX > 0.0F )
        {
            // �ړ��ʂ�␳����
            *MoveX = blx - X - 1.0F ;

            // ���ӂɓ��������ƕԂ�
            return 1 ;
        }

        // �E�ӂɓ������Ă����ꍇ
        if( *MoveX < 0.0F )
        {
            // �ړ��ʂ�␳����
            *MoveX = brx - X + 1.0F ;

            // �E�ӂɓ��������ƕԂ�
            return 2 ;
        }

        // �����ɗ�����K���Ȓl��Ԃ�
        return 4 ;
    }

    // �ǂ��ɂ�������Ȃ������ƕԂ�
    return 0 ;
}

// �}�b�v�`�b�v�̒l���擾����֐�
int GetChipParam( float X, float Y )
{
    int x, y ;

    // �����l�֕ϊ�
    x = (int)X / CHIP_SIZE ;
    y = (int)Y / CHIP_SIZE ;

    // �}�b�v����͂ݏo�Ă����� 0 ��Ԃ�
    if( x >= MAP_WIDTH || y >= MAP_HEIGHT || x < 0 || y < 0 ) return 0 ;

    // �w��̍��W�ɊY������}�b�v�̏���Ԃ�
    return MapData[ y ][ x ] ;
}



