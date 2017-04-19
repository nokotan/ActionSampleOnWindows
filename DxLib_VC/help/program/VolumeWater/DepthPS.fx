// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 ViewPosition   : TEXCOORD0 ;
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0         : COLOR0 ;
};


// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;

	// ���s���l����������
	PSOutput.Color0.r = PSInput.ViewPosition.z ;
	PSOutput.Color0.g = 0.0f ;
	PSOutput.Color0.b = 0.0f ;
	PSOutput.Color0.a = 1.0 ;
   
	return PSOutput ;    
}
