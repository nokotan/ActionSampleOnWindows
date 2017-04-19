// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 Diffuse         : COLOR0 ;       // �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1 ;       // �X�y�L�����J���[
	float2 TexCoords0      : TEXCOORD0 ;    // �e�N�X�`�����W
	float4 DPPosition0     : TEXCOORD1 ;    // �[�x�o�b�t�@�O�����_�����O���̍��W( �ˉe��� )
	float4 DPPosition1     : TEXCOORD2 ;    // �[�x�o�b�t�@�P�����_�����O���̍��W( �ˉe��� )
	float4 DPPosition2     : TEXCOORD3 ;    // �[�x�o�b�t�@�Q�����_�����O���̍��W( �ˉe��� )
	float4 DPPosition3     : TEXCOORD4 ;    // �[�x�o�b�t�@�R�����_�����O���̍��W( �ˉe��� )
	float4 DPPosition4     : TEXCOORD5 ;    // �[�x�o�b�t�@�S�����_�����O���̍��W( �ˉe��� )
	float4 DPPosition5     : TEXCOORD6 ;    // �[�x�o�b�t�@�T�����_�����O���̍��W( �ˉe��� )
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;
} ;


// C++ ���Őݒ肷��萔��e�N�X�`���̒�`
sampler DiffuseMapTexture             : register( s0 ) ;		// �f�B�t���[�Y�}�b�v�e�N�X�`��
sampler DepthMapTexture0              : register( s1 ) ;		// �[�x�o�b�t�@�O�e�N�X�`��
sampler DepthMapTexture1              : register( s2 ) ;		// �[�x�o�b�t�@�P�e�N�X�`��
sampler DepthMapTexture2              : register( s3 ) ;		// �[�x�o�b�t�@�Q�e�N�X�`��
sampler DepthMapTexture3              : register( s4 ) ;		// �[�x�o�b�t�@�R�e�N�X�`��
sampler DepthMapTexture4              : register( s5 ) ;		// �[�x�o�b�t�@�S�e�N�X�`��
sampler DepthMapTexture5              : register( s6 ) ;		// �[�x�o�b�t�@�T�e�N�X�`��


// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;
	float TextureDepth ;
	float2 DepthTexCoord ;
	float4 DefaultOutput ;
	float3 DPPosition ;


	// �e�N�X�`���J���[�̓ǂݍ���
	TextureDiffuseColor = tex2D( DiffuseMapTexture, PSInput.TexCoords0.xy ) ;

	// �o�̓J���[ = �f�B�t���[�Y�J���[ * �e�N�X�`���J���[ + �X�y�L�����J���[
	DefaultOutput = PSInput.Diffuse * TextureDiffuseColor + PSInput.Specular ;

	// �o�̓A���t�@ = �f�B�t���[�Y�A���t�@ * �e�N�X�`���A���t�@
	DefaultOutput.a = PSInput.Diffuse.a * TextureDiffuseColor.a ;


	// �[�x�o�b�t�@�O�ɕ`����W���f���Ă��邩�𔻒�
	DPPosition.x = PSInput.DPPosition0.x / PSInput.DPPosition0.w;
	DPPosition.y = PSInput.DPPosition0.y / PSInput.DPPosition0.w;
	DPPosition.z = PSInput.DPPosition0.z / PSInput.DPPosition0.w;
	if( DPPosition.x > -1.0f && DPPosition.x < 1.0f &&
		DPPosition.y > -1.0f && DPPosition.y < 1.0f &&
		DPPosition.z >  0.0f && DPPosition.z < 1.0f )
	{
		// �f���Ă���ꍇ�͐[�x�o�b�t�@�O��K�p����

		// �[�x�o�b�t�@�O�e�N�X�`���̍��W���Z�o
		// PSInput.DPPosition.xy �� -1.0f �` 1.0f �̒l�Ȃ̂ŁA����� 0.0f �` 1.0f �̒l�ɂ���
		DepthTexCoord.x = ( DPPosition.x + 1.0f ) / 2.0f;

		// y�͍X�ɏ㉺���]
		DepthTexCoord.y = 1.0f - ( DPPosition.y + 1.0f ) / 2.0f;

		// �[�x�o�b�t�@�O�e�N�X�`������[�x���擾
		TextureDepth = tex2D( DepthMapTexture0, DepthTexCoord );

		// �e�N�X�`���ɋL�^����Ă���[�x( +�␳�l )���y�l���傫�������牜�ɂ���Ƃ������ƂŋP�x�𔼕��ɂ���
		if( DPPosition.z > TextureDepth + 0.001f )
		{
			DefaultOutput.rgb *= 0.5f;
		}
	}

	// �[�x�o�b�t�@�P�ɕ`����W���f���Ă��邩�𔻒�
	DPPosition.x = PSInput.DPPosition1.x / PSInput.DPPosition1.w;
	DPPosition.y = PSInput.DPPosition1.y / PSInput.DPPosition1.w;
	DPPosition.z = PSInput.DPPosition1.z / PSInput.DPPosition1.w;
	if( DPPosition.x > -1.0f && DPPosition.x < 1.0f &&
		DPPosition.y > -1.0f && DPPosition.y < 1.0f &&
		DPPosition.z >  0.0f && DPPosition.z < 1.0f )
	{
		DepthTexCoord.x = ( DPPosition.x + 1.0f ) / 2.0f;
		DepthTexCoord.y = 1.0f - ( DPPosition.y + 1.0f ) / 2.0f;
		TextureDepth = tex2D( DepthMapTexture1, DepthTexCoord );

		if( DPPosition.z > TextureDepth + 0.001f )
		{
			DefaultOutput.rgb *= 0.5f;
		}
	}

	// �[�x�o�b�t�@�Q�ɕ`����W���f���Ă��邩�𔻒�
	DPPosition.x = PSInput.DPPosition2.x / PSInput.DPPosition2.w;
	DPPosition.y = PSInput.DPPosition2.y / PSInput.DPPosition2.w;
	DPPosition.z = PSInput.DPPosition2.z / PSInput.DPPosition2.w;
	if( DPPosition.x > -1.0f && DPPosition.x < 1.0f &&
		DPPosition.y > -1.0f && DPPosition.y < 1.0f &&
		DPPosition.z >  0.0f && DPPosition.z < 1.0f )
	{
		DepthTexCoord.x = ( DPPosition.x + 1.0f ) / 2.0f;
		DepthTexCoord.y = 1.0f - ( DPPosition.y + 1.0f ) / 2.0f;
		TextureDepth = tex2D( DepthMapTexture2, DepthTexCoord );

		if( DPPosition.z > TextureDepth + 0.001f )
		{
			DefaultOutput.rgb *= 0.5f;
		}
	}

	// �[�x�o�b�t�@�R�ɕ`����W���f���Ă��邩�𔻒�
	DPPosition.x = PSInput.DPPosition3.x / PSInput.DPPosition3.w;
	DPPosition.y = PSInput.DPPosition3.y / PSInput.DPPosition3.w;
	DPPosition.z = PSInput.DPPosition3.z / PSInput.DPPosition3.w;
	if( DPPosition.x > -1.0f && DPPosition.x < 1.0f &&
		DPPosition.y > -1.0f && DPPosition.y < 1.0f &&
		DPPosition.z >  0.0f && DPPosition.z < 1.0f )
	{
		DepthTexCoord.x = ( DPPosition.x + 1.0f ) / 2.0f;
		DepthTexCoord.y = 1.0f - ( DPPosition.y + 1.0f ) / 2.0f;
		TextureDepth = tex2D( DepthMapTexture3, DepthTexCoord );

		if( DPPosition.z > TextureDepth + 0.001f )
		{
			DefaultOutput.rgb *= 0.5f;
		}
	}

	// �[�x�o�b�t�@�S�ɕ`����W���f���Ă��邩�𔻒�
	DPPosition.x = PSInput.DPPosition4.x / PSInput.DPPosition4.w;
	DPPosition.y = PSInput.DPPosition4.y / PSInput.DPPosition4.w;
	DPPosition.z = PSInput.DPPosition4.z / PSInput.DPPosition4.w;
	if( DPPosition.x > -1.0f && DPPosition.x < 1.0f &&
		DPPosition.y > -1.0f && DPPosition.y < 1.0f &&
		DPPosition.z >  0.0f && DPPosition.z < 1.0f )
	{
		DepthTexCoord.x = ( DPPosition.x + 1.0f ) / 2.0f;
		DepthTexCoord.y = 1.0f - ( DPPosition.y + 1.0f ) / 2.0f;
		TextureDepth = tex2D( DepthMapTexture4, DepthTexCoord );

		if( DPPosition.z > TextureDepth + 0.001f )
		{
			DefaultOutput.rgb *= 0.5f;
		}
	}

	// �[�x�o�b�t�@�T�ɕ`����W���f���Ă��邩�𔻒�
	DPPosition.x = PSInput.DPPosition5.x / PSInput.DPPosition5.w;
	DPPosition.y = PSInput.DPPosition5.y / PSInput.DPPosition5.w;
	DPPosition.z = PSInput.DPPosition5.z / PSInput.DPPosition5.w;
	if( DPPosition.x > -1.0f && DPPosition.x < 1.0f &&
		DPPosition.y > -1.0f && DPPosition.y < 1.0f &&
		DPPosition.z >  0.0f && DPPosition.z < 1.0f )
	{
		DepthTexCoord.x = ( DPPosition.x + 1.0f ) / 2.0f;
		DepthTexCoord.y = 1.0f - ( DPPosition.y + 1.0f ) / 2.0f;
		TextureDepth = tex2D( DepthMapTexture5, DepthTexCoord );

		if( DPPosition.z > TextureDepth + 0.001f )
		{
			DefaultOutput.rgb *= 0.5f;
		}
	}


	// �o�̓J���[���Z�b�g
	PSOutput.Color0 = DefaultOutput;


	// �o�̓p�����[�^��Ԃ�
	return PSOutput ;
}


