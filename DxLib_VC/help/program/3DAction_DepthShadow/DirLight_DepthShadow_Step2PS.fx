// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 Diffuse         : COLOR0 ;       // �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1 ;       // �X�y�L�����J���[
	float2 TexCoords0      : TEXCOORD0 ;    // �e�N�X�`�����W
	float4 LPPosition      : TEXCOORD1 ;    // ���C�g����݂����W( x��y�̓��C�g�̎ˉe���W�Az�̓r���[���W )
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;
} ;


// C++ ���Őݒ肷��萔��e�N�X�`���̒�`
sampler DiffuseMapTexture             : register( s0 ) ;		// �f�B�t���[�Y�}�b�v�e�N�X�`��
sampler DepthMapTexture               : register( s1 ) ;		// �[�x�o�b�t�@�e�N�X�`��


// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;
	float TextureDepth ;
	float2 DepthTexCoord ;
	float4 DefaultOutput ;


	// �e�N�X�`���J���[�̓ǂݍ���
	TextureDiffuseColor = tex2D( DiffuseMapTexture, PSInput.TexCoords0.xy ) ;

	// �o�̓J���[ = �f�B�t���[�Y�J���[ * �e�N�X�`���J���[ + �X�y�L�����J���[
	DefaultOutput = PSInput.Diffuse * TextureDiffuseColor + PSInput.Specular ;

	// �o�̓A���t�@ = �f�B�t���[�Y�A���t�@ * �e�N�X�`���A���t�@
	DefaultOutput.a = PSInput.Diffuse.a * TextureDiffuseColor.a ;


	// �[�x�e�N�X�`���̍��W���Z�o
	// PSInput.LPPosition.xy �� -1.0f �` 1.0f �̒l�Ȃ̂ŁA����� 0.0f �` 1.0f �̒l�ɂ���
	DepthTexCoord.x = ( PSInput.LPPosition.x + 1.0f ) / 2.0f;

	// y�͍X�ɏ㉺���]
	DepthTexCoord.y = 1.0f - ( PSInput.LPPosition.y + 1.0f ) / 2.0f;

	// �[�x�o�b�t�@�e�N�X�`������[�x���擾
	TextureDepth = tex2D( DepthMapTexture, DepthTexCoord );

	// �e�N�X�`���ɋL�^����Ă���[�x( +�␳�l )���y�l���傫�������牜�ɂ���Ƃ������ƂŋP�x�𔼕��ɂ���
	if( PSInput.LPPosition.z > TextureDepth + 25.0f )
	{
		DefaultOutput.rgb *= 0.5f;
	}


	// �o�̓J���[���Z�b�g
	PSOutput.Color0 = DefaultOutput;


	// �o�̓p�����[�^��Ԃ�
	return PSOutput ;
}


