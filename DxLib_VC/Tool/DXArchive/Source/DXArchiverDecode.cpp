// ============================================================================
//
//			�c�w�`�t�@�C���f�R�[�h�\�t�g
//
//	Creator			: �R�c�@�I
//	Creation Date	: 2003/09/13
//	Version			: 1.02
//
// ============================================================================

// include ----------------------------
#include "DxArchive\\DXArchive.h"
#include "FileLib\\FileLib.h"
#include <windows.h>

// define -----------------------------
#define VERSION			"1.03"				// �o�[�W����
#define FILETYPE		"dxa"				// �g���q

// data type --------------------------

// data -------------------------------

// function proto type ----------------

// function code ----------------------

// ���C���֐�
int main( int argc, char *argv[ ] )
{
	char FullPath[MAX_PATH], FilePath[MAX_PATH], DirectoryPath[MAX_PATH], FileName[MAX_PATH] ;
	char *DxaFilePath, *KeyString ;

	printf( "�c�w�`�t�@�C���ȈՃf�R�[�h�\�t�g  Ver%s\n\n", VERSION ) ;

	// ���������������w���v���o��
	if( argc == 1 ) goto ERR ;

	// �������񂪂��邩�ǂ����ŏ����𕪊�
	if( strnicmp( argv[1], "-K:", 3 ) == 0 )
	{
		// �������񂪂���ꍇ
		if( argc == 2 ) goto ERR ;

		KeyString   = argv[1] + 3 ;
		DxaFilePath = argv[2] ;
	}
	else
	{
		// �������񂪖����ꍇ
		KeyString   = NULL ;
		DxaFilePath = argv[1] ;
	}

	// �c�w�`�t�@�C���̃t���p�X���擾
	ConvertFullPath( DxaFilePath, FullPath ) ;

	// �f�B���N�g���ƃt�@�C�����ɕ���
	AnalysisFileNameAndDirPath( FullPath, FilePath, DirectoryPath ) ;

	// �J�����g�f�B���N�g����ύX
	SetCurrentDirectory( DirectoryPath ) ;

	// �t�@�C��������g���q����������̂��쐬
	AnalysisFileNameAndExeName( FilePath, FileName, NULL ) ;

	// �t�@�C�����Ɠ����f�B���N�g���̍쐬
	CreateDirectory( FileName, NULL ) ;

	// �J�����g�f�B���N�g����ύX
	SetCurrentDirectory( FileName ) ;

	// �A�[�J�C�u�t�@�C����W�J����
	DXArchive::DecodeArchive( FullPath, "", KeyString ) ;

	// �I��
	return 0 ;

ERR:
	printf( "DxaDecode.exe [-K:KeyString] DxArchiveFilePath\n" ) ;
	printf( "�E�c�w�`�t�@�C�����h���b�O���h���b�v���邾���ł��W�J�o���܂�\n" ) ;
	printf( "�E�����ݒ肳��Ă���ꍇ�� -K �I�v�V�����Ō����w�肷��K�v������܂�\n" ) ;
	return -1 ;
}

	

