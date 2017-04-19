// �L�[�R���t�B�O�����̃w�b�_�t�@�C��

#ifndef KEYCONFIG_H
#define KEYCONFIG_H

#include "DxLib.h"

// ���͏��^�C�v
#define KEYCONFIG_INPUT_LEFT            (0)         // �������͍�
#define KEYCONFIG_INPUT_RIGHT           (1)         // �������͉E
#define KEYCONFIG_INPUT_UP              (2)         // �������͏�
#define KEYCONFIG_INPUT_DOWN            (3)         // �������͉�
#define KEYCONFIG_INPUT_CAMERA_LEFT     (4)         // �J�����p�������͍�
#define KEYCONFIG_INPUT_CAMERA_RIGHT    (5)         // �J�����p�������͉E
#define KEYCONFIG_INPUT_CAMERA_UP       (6)         // �J�����p�������͏�
#define KEYCONFIG_INPUT_CAMERA_DOWN     (7)         // �J�����p�������͉�
#define KEYCONFIG_INPUT_ATTACK          (8)         // �U���{�^��
#define KEYCONFIG_INPUT_JUMP            (9)         // �W�����v�{�^��
#define KEYCONFIG_INPUT_NUM             (10)        // ���̓^�C�v�̐�

extern void   KeyConfig_Initialize( void );             // �L�[�R���t�B�O�̏��������s��
extern void   KeyConfig_InputProcess( void );           // �L�[�R���t�B�O�̓��͏������s��( ���t���[�����s���� )
extern int    KeyConfig_GetInput( void );               // �{�^���̓��͏�Ԃ��擾����( �߂�l : �{�^���̓��͏�� )
extern int    KeyConfig_GetEdgeInput( void );           // �{�^���̉����ꂽ�u�Ԃ݂̂̓��͏�Ԃ��擾����( �߂�l : �{�^���̉����ꂽ�u�Ԃ݂̂̓��͏�� )
extern VECTOR KeyConfig_GetCameraInput( void );         // �J��������p�̓��͏�Ԃ��擾����( �߂�l : �J��������p�̓��͏�� )
extern int    KeyConfig_Save( const char *FilePath );   // �L�[�R���t�B�O�ݒ���t�@�C���ɕۑ�����( �߂�l : �ۑ������FTRUE   �ۑ����s�FFALSE )
extern int    KeyConfig_Load( const char *FilePath );   // �L�[�R���t�B�O�ݒ���t�@�C������ǂݍ���( �߂�l : �ǂݍ��ݐ����FTRUE   �ǂݍ��ݎ��s�FFALSE )
extern void   KeyConfig_SetDefault( void );             // �L�[�R���t�B�O�ݒ���f�t�H���g�ɂ���

// ���݉�������Ă���L�[��p�b�h�̓��͏����`�F�b�N���āA���͂��������ꍇ�͏��X�V����( �L�[�R���t�B�O��ʗp )
//     �߂�l : ���͂����������ǂ���( TRUE:���͂�������  FALSE:���͖͂������� )
extern int    KeyConfig_UpdateInputTypeInfo( int UpdateInputType );

// �p�b�h���L�[�̓��͂����邩�ǂ������擾����( �L�[�R���t�B�O��ʗp )
//     �߂�l : ���͂����������ǂ���( TRUE:���͂�������  FALSE:���͖͂������� )
extern int    KeyConfig_CheckInput( void ) ;

// �w��̓��͏��^�C�v�Ɋ��蓖�Ă��Ă���p�b�h��L�[�̓��̖͂��O���擾����( �L�[�R���t�B�O��ʗp )
extern void   KeyConfig_GetInputTypeString( int InputType, char *InputString );

#endif
