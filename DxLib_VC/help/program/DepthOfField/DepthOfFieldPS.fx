// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float2 TexCoords0      : TEXCOORD0 ;
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;
} ;

// C++ ���Őݒ肷��e�N�X�`���̒�`
float2   cfDOF_Focus : register( c0 ) ;		// ��ʊE�[�x�̃t�H�[�J�X���   x:�t�H�[�J�X�J�n�ʒu   y:�t�H�[�J�X�I���ʒu

sampler  ColorMapTexture0            : register( s0 ) ;		// �J���[�}�b�v�e�N�X�`��( ��������e�N�X�`�� )
sampler  ColorMapTexture1            : register( s1 ) ;		// �J���[�}�b�v�e�N�X�`��( �ア�ڂ����̃e�N�X�`�� )
sampler  ColorMapTexture2            : register( s2 ) ;		// �J���[�}�b�v�e�N�X�`��( �����ڂ����̃e�N�X�`�� )
sampler  DepthMapTexture             : register( s3 ) ;		// �[�x�}�b�v�e�N�X�`��

// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 Color1, Color2 ;
	float BlendRate ;
	float Fade ;
	float Depth ;

	// �[�x�l���擾����
	Depth = tex2D( DepthMapTexture, PSInput.TexCoords0 ) ;

	// �t�H�[�J�X��񂩂�ڂ₯�����Z�o
	if( Depth < cfDOF_Focus.x )
	{
		Fade = 1.0f - Depth / cfDOF_Focus.x ;
	}
	else
	if( Depth < cfDOF_Focus.y )
	{
		Fade = 0.0f;
	}
	else
	{
		Fade = ( Depth - cfDOF_Focus.y ) / ( 1.0f - cfDOF_Focus.y ) ;
	}

	// �ڂ₯������F���Z�o
	if( Fade < 0.5f )
	{
		// �ڂ₯���� 0.5f �ȉ��̏ꍇ�͂ڂ��������摜�Ǝア�ڂ����摜����������
		Color1 = tex2D( ColorMapTexture0, PSInput.TexCoords0 ) ;
		Color2 = tex2D( ColorMapTexture1, PSInput.TexCoords0 ) ;
		BlendRate = Fade / 0.5f ;
	}
	else
	{
		// �ڂ₯���� 0.5f �ȏ�̏ꍇ�͎ア�ڂ����摜�Ƌ����ڂ����摜����������
		Color1 = tex2D( ColorMapTexture1, PSInput.TexCoords0 ) ;
		Color2 = tex2D( ColorMapTexture2, PSInput.TexCoords0 ) ;
		BlendRate = ( Fade - 0.5f ) / 0.5f ;
	}

	// ���������F�̒l���Z�o
	PSOutput.Color0 = lerp( Color1, Color2, BlendRate ) ;
   
	return PSOutput ;    
}
