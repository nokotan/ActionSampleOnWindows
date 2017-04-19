// �L�[�R���t�B�O�̃e�X�g

#include "DxLib.h"
#include "KeyConfig.h"

#define KEYCONFIG_FILE_NAME     "KeyConfig.txt"     // �L�[�R���t�B�O�f�[�^�̃t�@�C����
#define INFO_X                  (10)                // �`��J�n���W
#define INFO_Y                  (10)
#define INFO_SPACE              (24)                // �P�s������ɋ󂯂�X�y�[�X
#define INFO_NAME_X             (140)               // ���͖���`�悷��w���W
#define INFO_INPUT_X            (320)               // ���͏�Ԃ�`�悷��w���W

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

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    int i ;
    int Input ;
    int DrawY ;
    VECTOR CameraInput ;

    // �E�C���h�E���[�h�ŋN��
    ChangeWindowMode( TRUE );

    // �c�w���C�u�����̏�����
    if( DxLib_Init() < 0 ) return -1 ;

    // �`���𗠉�ʂɂ���
    SetDrawScreen( DX_SCREEN_BACK ) ;

    // �L�[�R���t�B�O������������
    KeyConfig_Initialize() ;

    // �L�[�R���t�B�O�t�@�C����ǂݍ���
    if( KeyConfig_Load( KEYCONFIG_FILE_NAME ) == FALSE )
    {
        // �R���t�B�O�t�@�C�����ǂݍ��߂Ȃ�������f�t�H���g�ݒ�ɂ���
        KeyConfig_SetDefault() ;
    }

    // ���C�����[�v
    while( ProcessMessage() == 0 )
    {
        // ��ʂ̃N���A
        ClearDrawScreen() ;

        // �L�[�R���t�B�O�̓��͏������s��
        KeyConfig_InputProcess() ;

        // ���͏�Ԃ��擾����
        Input = KeyConfig_GetInput() ;

        // ���ڂ̐������J��Ԃ�
        DrawY = INFO_Y ;
        for( i = 0; i < KEYCONFIG_INPUT_NUM; i++ )
        {
            char InputString[ 256 ] ;

            // ���ږ��̕`��
            DrawString( INFO_X, DrawY, g_KeyConfigMenuTable[ i ], GetColor( 255,255,255 ) ) ;

            // ���͂Ɋ��蓖�Ă��Ă�����͖����擾����
            KeyConfig_GetInputTypeString( i, InputString ) ;

            // ���蓖�Ă��Ă�����͖���`�悷��
            DrawString( INFO_NAME_X, DrawY, InputString, GetColor( 255,255,255 ) ) ;

            // ���͏�Ԃ�`�悷��
            DrawString( INFO_INPUT_X, DrawY, ( Input & ( 1 << i ) ) != 0 ? "��" : "�Q", GetColor( 255,255,255 ) ) ;

            // ���ڂ̕`��Y���W���P���ڕ����Ɉړ�����
            DrawY += INFO_SPACE ;
        }

        // �J�����p�������͂̏�Ԃ�`�悷��
        CameraInput = KeyConfig_GetCameraInput() ;
        DrawFormatString( INFO_X, DrawY, GetColor( 255,255,255 ), "�J��������  X:%.3f  Y:%.3f", CameraInput.x, CameraInput.y ) ;

        // ����ʂ̓��e��\��ʂɔ��f
        ScreenFlip() ;
    }

    // �c�w���C�u�����̌�n��
    DxLib_End();

    // �\�t�g�̏I��
    return 0;
}
