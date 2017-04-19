// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 ViewPosition             : TEXCOORD0 ;	// ���W( �r���[��� )
	float4 ProjectionPositionSub    : TEXCOORD1 ;	// ���W( �ˉe��� )�s�N�Z���V�F�[�_�[�ŎQ�Ƃ���ׂ̕�
	float4 Diffuse                  : COLOR0 ;		// �f�B�t���[�Y�J���[
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0         : COLOR0 ;
};


// C++ ���Őݒ肷��萔�̒�`
sampler  DepthTexture      : register( s0 ) ;   // �[�x�e�N�X�`��

float MaxOpacityDistance   : register( c0 ) ;   // �s�����x���ő�ɂȂ鐅���̋���
float MinOpacity           : register( c1 ) ;   // �Œ�s�����x
float2 DepthTextureUseSize : register( c2 ) ;	// �[�x�e�N�X�`���Ŏg�p���Ă���T�C�Y


// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float2 TexCoords ;
	float4 Depth ;
	float refractiveRatio ;

	// �[�x�e�N�X�`�����W�̎Z�o
	TexCoords.x = (  PSInput.ProjectionPositionSub.x / PSInput.ProjectionPositionSub.w + 1.0f ) / 2.0f ;
	TexCoords.y = ( -PSInput.ProjectionPositionSub.y / PSInput.ProjectionPositionSub.w + 1.0f ) / 2.0f ;

	// �[�x�e�N�X�`������[�x���擾
	Depth = tex2D( DepthTexture, TexCoords * DepthTextureUseSize ) ;

	// �s�����x���v�Z
	PSOutput.Color0.a = saturate( ( 1.0f - MinOpacity ) * ( Depth.r - PSInput.ViewPosition.z ) / MaxOpacityDistance + MinOpacity ) ;

	// �F�̃Z�b�g
	PSOutput.Color0.rgb = PSInput.Diffuse.rgb ;
   
	return PSOutput ;
}
