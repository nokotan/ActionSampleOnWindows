// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
	float4 Position        : POSITION ;			// ���W( ���[�J����� )
	int4   BlendIndices0   : BLENDINDICES0 ;	// �X�L�j���O�����p Float�^�萔�z��C���f�b�N�X
	float4 BlendWeight0    : BLENDWEIGHT0 ;		// �X�L�j���O�����p�E�G�C�g�l
	float3 Normal          : NORMAL0 ;			// �@��( ���[�J����� )
	float4 Diffuse         : COLOR0 ;			// �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1 ;			// �X�y�L�����J���[
	float4 TexCoords0      : TEXCOORD0 ;		// �e�N�X�`�����W
} ;

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
	float4 ProjectionPosition : POSITION ;		// �ˉe���W
	float4 ViewPosition       : TEXCOORD0 ;		// �r���[���W
} ;



// C++ ���Őݒ肷��萔�̒�`
float4              cfProjectionMatrix[ 4 ] : register( c2  ) ;			// �r���[�@�@���@�ˉe�s��
float4              cfViewMatrix[ 3 ]       : register( c6  ) ;			// ���[���h�@���@�r���[�s��
float4              cfLocalWorldMatrix[ 162 ] : register( c94 ) ;		// ���[�J���@���@���[���h�s��


// main�֐�
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 lLocalWorldMatrix[ 3 ] ;
	float4 lWorldPosition ;


	// �����̃t���[���̃u�����h�s��̍쐬
	lLocalWorldMatrix[ 0 ]  = cfLocalWorldMatrix[ VSInput.BlendIndices0.x + 0 ] * VSInput.BlendWeight0.x;
	lLocalWorldMatrix[ 1 ]  = cfLocalWorldMatrix[ VSInput.BlendIndices0.x + 1 ] * VSInput.BlendWeight0.x;
	lLocalWorldMatrix[ 2 ]  = cfLocalWorldMatrix[ VSInput.BlendIndices0.x + 2 ] * VSInput.BlendWeight0.x;

	lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.y + 0 ] * VSInput.BlendWeight0.y;
	lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.y + 1 ] * VSInput.BlendWeight0.y;
	lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.y + 2 ] * VSInput.BlendWeight0.y;

	lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.z + 0 ] * VSInput.BlendWeight0.z;
	lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.z + 1 ] * VSInput.BlendWeight0.z;
	lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.z + 2 ] * VSInput.BlendWeight0.z;

	lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.w + 0 ] * VSInput.BlendWeight0.w;
	lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.w + 1 ] * VSInput.BlendWeight0.w;
	lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.w + 2 ] * VSInput.BlendWeight0.w;


	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// ���[�J�����W�����[���h���W�ɕϊ�
	lWorldPosition.x = dot( VSInput.Position, lLocalWorldMatrix[ 0 ] ) ;
	lWorldPosition.y = dot( VSInput.Position, lLocalWorldMatrix[ 1 ] ) ;
	lWorldPosition.z = dot( VSInput.Position, lLocalWorldMatrix[ 2 ] ) ;
	lWorldPosition.w = 1.0f ;

	// ���[���h���W���r���[���W�ɕϊ�
	VSOutput.ViewPosition.x = dot( lWorldPosition, cfViewMatrix[ 0 ] ) ;
	VSOutput.ViewPosition.y = dot( lWorldPosition, cfViewMatrix[ 1 ] ) ;
	VSOutput.ViewPosition.z = dot( lWorldPosition, cfViewMatrix[ 2 ] ) ;
	VSOutput.ViewPosition.w = 1.0f ;

	// �r���[���W���ˉe���W�ɕϊ�
	VSOutput.ProjectionPosition.x = dot( VSOutput.ViewPosition, cfProjectionMatrix[ 0 ] ) ;
	VSOutput.ProjectionPosition.y = dot( VSOutput.ViewPosition, cfProjectionMatrix[ 1 ] ) ;
	VSOutput.ProjectionPosition.z = dot( VSOutput.ViewPosition, cfProjectionMatrix[ 2 ] ) ;
	VSOutput.ProjectionPosition.w = dot( VSOutput.ViewPosition, cfProjectionMatrix[ 3 ] ) ;

	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )


	// �o�̓p�����[�^��Ԃ�
	return VSOutput ;
}
