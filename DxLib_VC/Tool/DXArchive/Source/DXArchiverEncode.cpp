// ============================================================================
//
//			�c�w�`�t�@�C���쐬�\�t�g
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
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <direct.h>

// define -----------------------------
#define VERSION			"1.03"				// �o�[�W����
#define FILETYPE		"dxa"				// �g���q

// code -------------------------------

// ���C���֐�
int main( int argc, char *argv[] )
{
	char SrcPath[256], DestPath[256], *KeyString, *SrcDirectoryPath, *DestFilePath ;

	printf( "�c�w�`�t�@�C���ȈՍ쐬�\�t�g  Ver%s\n", VERSION ) ;

	// ���������������w���v���o��
	if( argc == 1 ) goto ERR ;

	// �������񂪂��邩�ǂ����𒲂ׂ�
	if( strnicmp( argv[1], "-K:", 3 ) == 0 )
	{
		// �������񂪂���ꍇ
		if( argc == 2 ) goto ERR ;

		KeyString        = argv[1] + 3 ;
		SrcDirectoryPath = argv[2] ;
		DestFilePath     = argc == 3 ? NULL : argv[3] ;
	}
	else
	{
		// �������񂪖����ꍇ

		KeyString        = NULL ;
		SrcDirectoryPath = argv[1] ;
		DestFilePath     = argc == 2 ? NULL : argv[2] ;
	}

	// �\�[�X�p�X�̃t���p�X���쐬����
	ConvertFullPath( SrcDirectoryPath, SrcPath ) ;

	// �o�̓t�@�C���p�X�̍쐬
	{
		if( DestFilePath != NULL )
		{
			char temp[256] ;

			strcpy( temp, DestFilePath ) ;
			ConvertFullPath( temp, DestPath ) ;
		}
		else
		{
			// �\�[�X�p�X����o�̓t�@�C�������쐬����
			sprintf( DestPath, "%s.%s", SrcPath, FILETYPE ) ;
		}
	}

	// �A�[�J�C�u�t�@�C�����쐬����
	DXArchive::EncodeArchiveOneDirectory( DestPath, SrcPath, true, KeyString ) ;

	// �I��
	return 0 ;

ERR:

	printf( "DxaEncode.exe [-K:KeyString] SrcDirectoryPath  [DestFilePath]\n\n" ) ;
	printf( "�E�c�w�`�t�@�C���ɂ������t�H���_���h���b�O���h���b�v���邾���ł��쐬�\�ł�\n" ) ; 
	printf( "�E-K �I�v�V�����Ō���ݒ�o���܂�(�ő�P�Q����) �@��  -K:KeyCode \n" ) ; 
	return -1; 
}


