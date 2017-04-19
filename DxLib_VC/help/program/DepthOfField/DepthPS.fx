// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 ViewPosition   : TEXCOORD0;
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0         : COLOR0 ;
};


// C++ ���Őݒ肷��萔�̒�`
float3 cfDOF_Area  : register( c0 ) ;	// ��ʊE�[�x�͈̔͏��   x:��ʊE�[�x�J�n�ʒu   y:��ʊE�[�x�I���ʒu   z:��ʊE�[�x�͈̔͂̋t��


// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float z_param ;

	// ��ʊE�[�x�͈͓̔��� 0.0f �` 1.0f �ɕϊ�
	if( PSInput.ViewPosition.z < cfDOF_Area.x )
	{
		PSOutput.Color0.r = 0.0f ;
	}
	else
	if( PSInput.ViewPosition.z > cfDOF_Area.y )
	{
		PSOutput.Color0.r = 1.0f ;
	}
	else
	{
		PSOutput.Color0.r = ( PSInput.ViewPosition.z - cfDOF_Area.x ) * cfDOF_Area.z ;
	}

	PSOutput.Color0.g = 0.0f ;
	PSOutput.Color0.b = 0.0f ;
	PSOutput.Color0.a = 0.0f ;
   
	return PSOutput ;    
}
