// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 MirrorProjectionPosition : TEXCOORD0 ;	// ���ɉf��f���̎��_�ł̎ˉe���W
	float3 ViewPosition             : TEXCOORD1 ;	// ���W( �r���[��� )
	float3 ViewNormal               : TEXCOORD2 ;	// �@��( �r���[��� )
	float4 Diffuse                  : COLOR0 ;		// �f�B�t���[�Y�J���[
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0         : COLOR0 ;
};


// C++ ���Őݒ肷��萔�̒�`
sampler  MirrorTexture : register( s0 ) ;		// ���ɉf���e�N�X�`��

float4   cfMulColor : register( c0  ) ;			// �e�N�X�`���F�ɏ�Z����F


// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float2 TexCoords ;
	float4 TexColor ;
	float refractiveRatio ;

	// �e�N�X�`�����W�̎Z�o
	TexCoords.x = PSInput.MirrorProjectionPosition.x / PSInput.MirrorProjectionPosition.w ;
	TexCoords.y = PSInput.MirrorProjectionPosition.y / PSInput.MirrorProjectionPosition.w ;

	// ���ɉf���e�N�X�`������F���擾
	TexColor = tex2D( MirrorTexture, TexCoords ) ;

	// �f�B�t���[�Y�J���[�Ə�Z���ďo�͂ɃZ�b�g
	PSOutput.Color0 = TexColor * PSInput.Diffuse ;


	// ���̋��ܗ� = 1.333    ��C�̋��ܗ� = 1.000293
	refractiveRatio = 1.333f / 1.000293f ;

	// �t���l�����˗��v�Z( �}���y�P����[�ǂ��ƃR���l�̃t���l�����ˉ��������p )
	float A = refractiveRatio ;
	float B = dot( -normalize( PSInput.ViewPosition ), normalize( PSInput.ViewNormal ) ) ;
	float C = sqrt( 1.0f - A * A * ( 1 - B * B ) ) ;
	float Rs = ( A * B - C ) * ( A * B - C ) / ( ( A * B + C ) * ( A * B + C ) ) ;
	float Rp = ( A * C - B ) * ( A * C - B ) / ( ( A * C + B ) * ( A * C + B ) ) ;
	float alpha = ( Rs + Rp ) / 2.0f ;

	alpha = min( alpha + 0.20f, 1.0f ) ; 

	// �A���t�@�l���Z�b�g
	PSOutput.Color0.a = alpha ;

   
	return PSOutput ;
}
