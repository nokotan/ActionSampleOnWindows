// ============================================================================
//
//		�t�@�C���֌W�̃��C�u����
//
//		Creator			: �R�c�@�I
//		Creation Data	: 09/17/2004
//
// ============================================================================

// include --------------------------------------
#include <string.h>
#include <stdio.h>
#include <mbstring.h>
#include <windows.h>
#include "FileLib.h"

// define ---------------------------------------

// data type ------------------------------------

// data -----------------------------------------

// function proto type --------------------------

// function code --------------------------------

// �t���p�X�ł͂Ȃ��p�X��������t���p�X�ɕϊ�����
// ( CurrentDir �̓t���p�X�ł���K�v������(����Ɂw\�x�������Ă������Ă��ǂ�) )
// ( CurrentDir �� NULL �̏ꍇ�͎��ۂ̃J�����g�f�B���N�g�����g�p���� )
extern int ConvertFullPath( const char *Src, char *Dest, const char *CurrentDir )
{
	int i, j, k ;
	char iden[256], cur[MAX_PATH] ;

	if( CurrentDir == NULL )
	{
		GetCurrentDirectory( MAX_PATH, cur ) ;
		CurrentDir = cur ;
	}

	if( Src == NULL )
	{
		strcpy( Dest, CurrentDir ) ;
		goto END ;
	}

	i = 0 ;
	j = 0 ;
	k = 0 ;
	
	// �ŏ����w\�x���́w/�x�̏ꍇ�̓J�����g�h���C�u�̃��[�g�f�B���N�g���܂ŗ�����
	if( Src[0] == '\\' )
	{
		Dest[0] = CurrentDir[0] ;
		Dest[1] = CurrentDir[1] ;
		Dest[2] = '\0' ;

		i ++ ;
		j = 2 ;
	}
	else
	// �h���C�u����������Ă����炻�̃h���C�u��
	if( Src[1] == ':' )
	{
		Dest[0] = Src[0] ;
		Dest[1] = Src[1] ;
		Dest[2] = '\0' ;

		i = 2 ;
		j = 2 ;

		if( Src[i] == '\\' ) i ++ ;
	}
	else
	// ����ȊO�̏ꍇ�̓J�����g�f�B���N�g��
	{
		strcpy( Dest, CurrentDir ) ;
		j = strlen( Dest ) ;
		if( Dest[j-1] == '\\' || Dest[j-1] == '/' )
		{
			Dest[j-1] = '\0' ;
			j -- ;
		}
	}

	for(;;)
	{
		switch( Src[i] )
		{
		case '\0' :
			if( k != 0 )
			{
				Dest[j] = '\\' ; j ++ ;
				strcpy( &Dest[j], iden ) ;
				j += k ;
				k = 0 ;
			}
			goto END ;

		case '\\' :
		case '/' :
			if( k == 0 ) return -1 ;		// �����񂪖���������G���[
			if( strcmp( iden, "." ) == 0 )
			{
				// �Ȃɂ����Ȃ�
			}
			else
			if( strcmp( iden, ".." ) == 0 )
			{
				// ����̃f�B���N�g����
				j -- ;
				while( Dest[j] != '\\' && Dest[j] != '/' && Dest[j] != ':' ) j -- ;
				if( Dest[j] != ':' ) Dest[j] = '\0' ;
				else j ++ ;
			}
			else
			{
				Dest[j] = '\\' ; j ++ ;
				strcpy( &Dest[j], iden ) ;
				j += k ;
			}

			k = 0 ;
			i ++ ;
			break ;
		
		default :
			if( _mbsbtype( ( const unsigned char * )&Src[i], 0 ) == 0/*(_MBC_SINGLE)*/ )
			{
				iden[k] = Src[i] ;
				iden[k+1] = 0 ; 
				k ++ ;
				i ++ ;
			}
			else
			{
				*(( unsigned short * )&iden[k] ) = *(( unsigned short * )&Src[i] ) ;
				iden[k+2] = '\0' ;
				k += 2 ;
				i += 2 ;
			}
			break ;
		}
	}
	
END :
	// ����I��
	return 0 ;
}

// �t�@�C�������ꏏ�ɂȂ��Ă���ƕ������Ă���p�X������t�@�C�����ƃf�B���N�g���p�X�𕪊�����
// �t���p�X�ł���K�v�͖����A�t�@�C���������ł��ǂ�
extern int AnalysisFileNameAndDirPath( const char *Src, char *FileName, char *DirPath )
{
	int i, Last ;
	
	// �t�@�C�����𔲂��o��
	i = 0 ;
	Last = -1 ;
	while( Src[i] != '\0' )
	{
		if( _mbsbtype( ( const unsigned char * )&Src[i], 0 ) == 0/*(_MBC_SINGLE)*/ )
		{
			if( Src[i] == '\\' || Src[i] == '/' || Src[i] == '\0' || Src[i] == ':' ) Last = i ;
			i ++ ;
		}
		else
		{
			i += 2 ;
		}
	}
	if( FileName != NULL )
	{
		if( Last != -1 ) strcpy( FileName, &Src[Last+1] ) ;
		else strcpy( FileName, Src ) ;
	}
	
	// �f�B���N�g���p�X�𔲂��o��
	if( DirPath != NULL )
	{
		if( Last != -1 )
		{
			strncpy( DirPath, Src, Last ) ;
			DirPath[Last] = '\0' ;
		}
		else
		{
			DirPath[0] = '\0' ;
		}
	}
	
	// �I��
	return 0 ;
}

// �t�@�C���p�X����t�@�C�����Ɗg���q���擾����
extern int AnalysisFileNameAndExeName( const char *Src, char *Name, char *ExeName )
{
	char FileName[256], *p, ename[128], name[128] ;

	// �t�@�C�����݂̂��擾
	AnalysisFileNameAndDirPath( Src, FileName, 0 ) ;

	// �w.�x�����邩�ǂ����ŏ����𕪊�
	if( ( p = strchr( FileName, '.' ) ) == NULL )
	{
		strcpy( name, FileName ) ;
		ename[0] = '\0' ;
	}
	else
	{
		strncpy( name, FileName, p - FileName ) ;
		name[p - FileName] = '\0' ;
		strcpy( ename, p + 1 ) ;
	}

	if( Name != NULL ) strcpy( Name, name ) ;
	if( ExeName != NULL ) strcpy( ExeName, ename ) ;

	// �I��
	return 0 ;
}




