// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
	float4 Position        : POSITION ;     // ���W( ���[�J����� )
	float3 Normal          : NORMAL0 ;      // �@��( ���[�J����� )
	float4 Diffuse         : COLOR0 ;       // �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1 ;       // �X�y�L�����J���[
	float4 TexCoords0      : TEXCOORD0 ;	// �e�N�X�`�����W
} ;

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
	float4 Position        : POSITION ;     // ���W( �ˉe��� )
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

// �}�e���A���p�����[�^
struct VS_CONST_MATERIAL
{
	float4 Diffuse ;                // �}�e���A���f�B�t���[�Y�J���[
	float4 Specular ;               // �}�e���A���X�y�L�����J���[
	float4 Power ;                  // �}�e���A���X�y�L�����n�C���C�g�̋���
} ;

// ���C�g�p�����[�^
struct VS_CONST_LIGHT
{
	float4 Position ;               // ���W( �r���[��� )
	float3 Direction ;              // ����( �r���[��� )
	float4 Diffuse ;                // �f�B�t���[�Y�J���[
	float4 Specular ;               // �X�y�L�����J���[
	float4 Ambient ;                // �A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������
	float4 Range_FallOff_AT0_AT1 ;  // x:�L������  y:�X�|�b�g���C�g�pFallOff  z:�����ɂ�錸�������p�p�����[�^�O  w:�����ɂ�錸�������p�p�����[�^�P
	float4 AT2_SpotP0_SpotP1 ;      // x:�����ɂ�錸�������p�p�����[�^�Q  y:�X�|�b�g���C�g�p�p�����[�^�O( cos( Phi / 2.0f ) )  z:�X�|�b�g���C�g�p�p�����[�^�P( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )
} ;



// C++ ���Őݒ肷��萔�̒�`
float4              cfAmbient_Emissive      : register( c1  ) ;		// �}�e���A���G�~�b�V�u�J���[ + �}�e���A���A���r�G���g�J���[ * �O���[�o���A���r�G���g�J���[
float4              cfProjectionMatrix[ 4 ] : register( c2  ) ;		// �r���[�@�@���@�ˉe�s��
float4              cfViewMatrix[ 3 ]       : register( c6  ) ;		// ���[���h�@���@�r���[�s��
float4              cfLocalWorldMatrix[ 3 ] : register( c94 ) ;		// ���[�J���@���@���[���h�s��
VS_CONST_MATERIAL   cfMaterial              : register( c11 ) ;		// �}�e���A���p�����[�^
VS_CONST_LIGHT      cfLight                 : register( c14 ) ;		// �L�����C�g�O�Ԃ̃p�����[�^

matrix              cfDepthViewMatrix0       : register( c43  ) ;	// �[�x�o�b�t�@�O�����_�����O���̃��[���h�@���@�r���[�s��
matrix              cfDepthProjectionMatrix0 : register( c47  ) ;	// �[�x�o�b�t�@�O�����_�����O���̃r���[�@�@���@�ˉe�s��
matrix              cfDepthViewMatrix1       : register( c51  ) ;	// �[�x�o�b�t�@�P�����_�����O���̃��[���h�@���@�r���[�s��
matrix              cfDepthProjectionMatrix1 : register( c55  ) ;	// �[�x�o�b�t�@�P�����_�����O���̃r���[�@�@���@�ˉe�s��
matrix              cfDepthViewMatrix2       : register( c59  ) ;	// �[�x�o�b�t�@�Q�����_�����O���̃��[���h�@���@�r���[�s��
matrix              cfDepthProjectionMatrix2 : register( c63  ) ;	// �[�x�o�b�t�@�Q�����_�����O���̃r���[�@�@���@�ˉe�s��
matrix              cfDepthViewMatrix3       : register( c67  ) ;	// �[�x�o�b�t�@�R�����_�����O���̃��[���h�@���@�r���[�s��
matrix              cfDepthProjectionMatrix3 : register( c71  ) ;	// �[�x�o�b�t�@�R�����_�����O���̃r���[�@�@���@�ˉe�s��
matrix              cfDepthViewMatrix4       : register( c75  ) ;	// �[�x�o�b�t�@�S�����_�����O���̃��[���h�@���@�r���[�s��
matrix              cfDepthProjectionMatrix4 : register( c79  ) ;	// �[�x�o�b�t�@�S�����_�����O���̃r���[�@�@���@�ˉe�s��
matrix              cfDepthViewMatrix5       : register( c83  ) ;	// �[�x�o�b�t�@�T�����_�����O���̃��[���h�@���@�r���[�s��
matrix              cfDepthProjectionMatrix5 : register( c87  ) ;	// �[�x�o�b�t�@�T�����_�����O���̃r���[�@�@���@�ˉe�s��


// main�֐�
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 lWorldPosition ;
	float4 lViewPosition ;
	float4 lDViewPosition ;
	float3 lWorldNrm ;
	float3 lViewNrm ;
	float3 lLightHalfVec ;
	float4 lLightLitParam ;
	float4 lLightLitDest ;
	float3 lLightDir ;
	float3 lLightTemp ;
	float lLightDistancePow2 ;
	float lLightGen ;

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
	VSOutput.Position.x = dot( lViewPosition, cfProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( lViewPosition, cfProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( lViewPosition, cfProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( lViewPosition, cfProjectionMatrix[ 3 ] ) ;

	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )



	// ���C�g�̏��� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// �@�����r���[��Ԃ̊p�x�ɕϊ� =========================================( �J�n )

	// ���[�J���x�N�g�������[���h�x�N�g���ɕϊ�
	lWorldNrm.x = dot( VSInput.Normal, cfLocalWorldMatrix[ 0 ].xyz ) ;
	lWorldNrm.y = dot( VSInput.Normal, cfLocalWorldMatrix[ 1 ].xyz ) ;
	lWorldNrm.z = dot( VSInput.Normal, cfLocalWorldMatrix[ 2 ].xyz ) ;

	// ���[���h�x�N�g�����r���[�x�N�g���ɕϊ�
	lViewNrm.x = dot( lWorldNrm, cfViewMatrix[ 0 ].xyz ) ;
	lViewNrm.y = dot( lWorldNrm, cfViewMatrix[ 1 ].xyz ) ;
	lViewNrm.z = dot( lWorldNrm, cfViewMatrix[ 2 ].xyz ) ;

	// �@�����r���[��Ԃ̊p�x�ɕϊ� =========================================( �I�� )

	// ���C�g�����x�N�g���̌v�Z
	lLightDir = normalize( lViewPosition.xyz - cfLight.Position.xyz ) ;


	// ���������l�v�Z ===================================================================( �J�n )

	// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
	lLightTemp = lViewPosition.xyz - cfLight.Position.xyz ;
	lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

	// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
	lLightGen = 1.0f / ( cfLight.Range_FallOff_AT0_AT1.z + cfLight.Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight.AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

	// �L�������O�������猸�������ő�ɂ��鏈��
	lLightGen *= step( lLightDistancePow2, cfLight.Range_FallOff_AT0_AT1.x ) ;

	// ���������l�v�Z ===================================================================( �I�� )


	// ���C�g�f�B�t���[�Y�J���[�ƃ��C�g�X�y�L�����J���[�̊p�x�����v�Z ===================( �J�n )

	// �@���ƃ��C�g�̋t�����x�N�g���Ƃ̓��ς� lLightLitParam.x �ɃZ�b�g
	lLightLitParam.x = dot( lViewNrm, -lLightDir ) ;

	// �n�[�t�x�N�g���̌v�Z norm( ( norm( ���_�ʒu���王�_�ւ̃x�N�g�� ) + ���C�g�̕��� ) )
	lLightHalfVec = normalize( normalize( -lViewPosition.xyz ) - lLightDir ) ;

	// �@���ƃn�[�t�x�N�g���̓��ς� lLightLitParam.y �ɃZ�b�g
	lLightLitParam.y = dot( lLightHalfVec, lViewNrm ) ;

	// �X�y�L�������˗��� lLightLitParam.w �ɃZ�b�g
	lLightLitParam.w = cfMaterial.Power.x ;

	// ���C�g�p�����[�^�v�Z
	lLightLitDest = lit( lLightLitParam.x, lLightLitParam.y, lLightLitParam.w ) ;

	// ���C�g�f�B�t���[�Y�J���[�ƃ��C�g�X�y�L�����J���[�̊p�x�����v�Z ===================( �I�� )

	// ���C�g�̏��� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )



	// �o�̓p�����[�^�Z�b�g ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// �f�B�t���[�Y�J���[ =
	//            ���������l *
	//            ( �f�B�t���[�Y�p�x�����v�Z���� *
	//              ���C�g�̃f�B�t���[�Y�J���[ *
	//              �}�e���A���̃f�B�t���[�Y�J���[ +
	//              ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� ) +
	//            �}�e���A���̃A���r�G���g�J���[�ƃO���[�o���A���r�G���g�J���[����Z�������̂ƃ}�e���A���G�~�b�V�u�J���[�����Z��������
	VSOutput.Diffuse = lLightGen * ( lLightLitDest.y * cfLight.Diffuse * cfMaterial.Diffuse + cfLight.Ambient ) + cfAmbient_Emissive ;

	// �f�B�t���[�Y�A���t�@�̓}�e���A���̃f�B�t���[�Y�J���[�̃A���t�@�����̂܂܎g��
	VSOutput.Diffuse.w = cfMaterial.Diffuse.w ;

	// �X�y�L�����J���[ = ���������l * �X�y�L�����p�x�����v�Z���� * ���C�g�̃X�y�L�����J���[ * �}�e���A���̃X�y�L�����J���[
	VSOutput.Specular = lLightGen * lLightLitDest.z * cfLight.Specular * cfMaterial.Specular ;


	// �e�N�X�`�����W�̃Z�b�g
	VSOutput.TexCoords0 = VSInput.TexCoords0;

	// �o�̓p�����[�^�Z�b�g ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )



	// �[�x�o�b�t�@�����_�����O���̃J�����ݒ�ł̎ˉe�s����Z�o ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	lDViewPosition = mul( cfDepthViewMatrix0, lWorldPosition ) ;
	VSOutput.DPPosition0 = mul( cfDepthProjectionMatrix0, lDViewPosition ) ;

	lDViewPosition = mul( cfDepthViewMatrix1, lWorldPosition ) ;
	VSOutput.DPPosition1 = mul( cfDepthProjectionMatrix1, lDViewPosition ) ;

	lDViewPosition = mul( cfDepthViewMatrix2, lWorldPosition ) ;
	VSOutput.DPPosition2 = mul( cfDepthProjectionMatrix2, lDViewPosition ) ;

	lDViewPosition = mul( cfDepthViewMatrix3, lWorldPosition ) ;
	VSOutput.DPPosition3 = mul( cfDepthProjectionMatrix3, lDViewPosition ) ;

	lDViewPosition = mul( cfDepthViewMatrix4, lWorldPosition ) ;
	VSOutput.DPPosition4 = mul( cfDepthProjectionMatrix4, lDViewPosition ) ;

	lDViewPosition = mul( cfDepthViewMatrix5, lWorldPosition ) ;
	VSOutput.DPPosition5 = mul( cfDepthProjectionMatrix5, lDViewPosition ) ;

	// �[�x�o�b�t�@�����_�����O���̃J�����ݒ�ł̎ˉe�s����Z�o ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )


	// �o�̓p�����[�^��Ԃ�
	return VSOutput ;
}

