// �L�[�R���t�B�O���

#include "DxLib.h"
#include "KeyConfig.h"

#define KEYCONFIG_FILE_NAME     "KeyConfig.txt"     // �L�[�R���t�B�O�f�[�^�̃t�@�C����
#define INFO_X                  (10)                // ���̕`��J�n���W
#define INFO_Y                  (10)
#define INFO_SPACE              (24)                // ��s������ɋ󂯂�X�y�[�X
#define INFO_NAME_X             (140)               // ���͖���`�悷��w���W

// �L�[�R���t�B�O�Ώۂ̍���
int TargetIndex ;

// �L�[�R���t�B�O�e���ڂ̖��O
static const char *g_KeyConfigMenuTable[ KEYCONFIG_INPUT_NUM ] =
{
    "��",
    "�E",
    "��",
    "��",
    "�J����������",
    "�J�����E����",
    "�J���������",
    "�J����������",
    "�U��",
    "�W�����v",
} ;

// WinMain �֐�
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    int i ;
    int DrawY ;
    int InputWait ;

    // �E�C���h�E���[�h�ŋN��
    ChangeWindowMode( TRUE ) ;

    // �c�w���C�u�����̏�����
    if( DxLib_Init() < 0 ) return -1 ;

    // �`���𗠉�ʂɂ���
    SetDrawScreen( DX_SCREEN_BACK ) ;

    // �L�[�R���t�B�O�����̏��������s��
    KeyConfig_Initialize() ;

    // �L�[�R���t�B�O�t�@�C����ǂݍ���
    if( KeyConfig_Load( KEYCONFIG_FILE_NAME ) == FALSE )
    {
        // �R���t�B�O�t�@�C�����ǂݍ��߂Ȃ�������f�t�H���g�ݒ�ɂ���
        KeyConfig_SetDefault() ;
    }

    // �u�������͂��������牽�����Ȃ��v�t���O��|��
    InputWait = FALSE ;

    // ���C�����[�v
    while( ProcessMessage() == 0 )
    {
        // ��ʂ̃N���A
        ClearDrawScreen() ;

        // �L�[�R���t�B�O�̓��͏������s��
        KeyConfig_InputProcess() ;

        // �u�������͂��������牽�����Ȃ��v�t���O�������Ă���ꍇ�͏����𕪊�
        if( InputWait == TRUE )
        {
            // �������͂�����������u�������͂��������牽�����Ȃ��v�t���O��|��
            if( KeyConfig_CheckInput() == FALSE )
            {
                InputWait = FALSE ;
            }
        }
        else
        {
            // �L�[��p�b�h�̉�����Ԃ��`�F�b�N���āA��������Ă�����L�[�R���t�B�O�ݒ�ɔ��f������
            if( KeyConfig_UpdateInputTypeInfo( TargetIndex ) )
            {
                // �����{�^���������ꂽ�玟�̍��ڂɈړ�����
                TargetIndex ++ ;

                // �u�������͂��������牽�����Ȃ��v�t���O�𗧂Ă�
                InputWait = TRUE ;
            }
        }

        // ���ڂ̐������J��Ԃ�
        DrawY = INFO_Y ;
        for( i = 0; i < KEYCONFIG_INPUT_NUM; i++ )
        {
            int DrawColor ;
            char InputString[ 256 ] ;

            // �ύX�Ώۂ̍��ڂ��ǂ����ŕ`��F��ς���
            DrawColor = TargetIndex == i ? GetColor( 255,0,0 ) : GetColor( 255,255,255 ) ;

            // ���ږ��̕`��
            DrawString( INFO_X, DrawY, g_KeyConfigMenuTable[ i ], DrawColor ) ;

            // ���͂Ɋ��蓖�Ă��Ă�����͖����擾����
            KeyConfig_GetInputTypeString( i, InputString ) ;

            // ���蓖�Ă��Ă�����͖���`�悷��
            DrawString( INFO_NAME_X, DrawY, InputString, DrawColor ) ;

            // ���ڂ̕`��Y���W���P���ڕ����Ɉړ�����
            DrawY += INFO_SPACE ;
        }

        // ����ʂ̓��e��\��ʂɔ��f
        ScreenFlip() ;

        // �S�Ă̓��͂̐ݒ肪�I������烋�[�v�𔲂���
        if( TargetIndex == KEYCONFIG_INPUT_NUM )
        {
            break ;
        }
    }

    // �L�[�R���t�B�O�ݒ��ۑ�����
    KeyConfig_Save( KEYCONFIG_FILE_NAME ) ;

    // �c�w���C�u�����̌�n��
    DxLib_End() ;

    // �\�t�g�̏I��
    return 0 ;
}








