// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 PPosition       : TEXCOORD0 ;        // ���W( �r���[��� )
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;
} ;

// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;

	// �y�l��F�Ƃ��ďo��
	PSOutput.Color0 = PSInput.PPosition.z / PSInput.PPosition.w;

	// �����ɂȂ�Ȃ��悤�ɃA���t�@�͕K���P
	PSOutput.Color0.a = 1.0f;

	// �o�̓p�����[�^��Ԃ�
	return PSOutput ;
}


