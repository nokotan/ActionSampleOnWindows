// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
	float4 Position                 : POSITION0 ;		// ���W( ���[�J����� )
	float4 MirrorProjectionPosition : POSITION1 ;		// ���ɉf��f���̎��_�ł̎ˉe���W
	float3 Normal                   : NORMAL0 ;			// �@��( ���[�J����� )
	float4 Diffuse                  : COLOR0 ;			// �f�B�t���[�Y�J���[
	float4 Specular                 : COLOR1 ;			// �X�y�L�����J���[
	float4 TexCoords0               : TEXCOORD0 ;		// �e�N�X�`�����W
} ;

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
	float4 ProjectionPosition       : POSITION ;	// �ˉe���W
	float4 MirrorProjectionPosition : TEXCOORD0 ;	// ���ɉf��f���̎��_�ł̎ˉe���W
	float4 Diffuse                  : COLOR0 ;		// �f�B�t���[�Y�J���[
} ;



// C++ ���Őݒ肷��萔�̒�`
float4              cfProjectionMatrix[ 4 ] : register( c2  ) ;		// �r���[�@�@���@�ˉe�s��
float4              cfViewMatrix[ 3 ]       : register( c6  ) ;		// ���[���h�@���@�r���[�s��
float4              cfLocalWorldMatrix[ 3 ] : register( c94 ) ;		// ���[�J���@���@���[���h�s��


// main�֐�
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 lWorldPosition ;
	float4 lViewPosition ;


	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// ���[�J�����W�����[���h���W�ɕϊ�
	lWorldPosition.x = dot( VSInput.Position, cfLocalWorldMatrix[ 0 ] ) ;
	lWorldPosition.y = dot( VSInput.Position, cfLocalWorldMatrix[ 1 ] ) ;
	lWorldPosition.z = dot( VSInput.Position, cfLocalWorldMatrix[ 2 ] ) ;
	lWorldPosition.w = 1.0f ;

	// ���[���h���W���r���[���W�ɕϊ�
	lViewPosition.x = dot( lWorldPosition, cfViewMatrix[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, cfViewMatrix[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, cfViewMatrix[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	// �r���[���W���ˉe���W�ɕϊ�
	VSOutput.ProjectionPosition.x = dot( lViewPosition, cfProjectionMatrix[ 0 ] ) ;
	VSOutput.ProjectionPosition.y = dot( lViewPosition, cfProjectionMatrix[ 1 ] ) ;
	VSOutput.ProjectionPosition.z = dot( lViewPosition, cfProjectionMatrix[ 2 ] ) ;
	VSOutput.ProjectionPosition.w = dot( lViewPosition, cfProjectionMatrix[ 3 ] ) ;
	
	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )

	// �f�B�t���[�Y�J���[�͂��̂܂ܓn��
	VSOutput.Diffuse = VSInput.Diffuse ;

	// ���ɉf��f���̎��_�ł̎ˉe���W
	VSOutput.MirrorProjectionPosition = VSInput.MirrorProjectionPosition ;


	// �o�̓p�����[�^��Ԃ�
	return VSOutput ;
}
