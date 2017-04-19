// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 PPosition       : TEXCOORD0 ;        // ���W( �ˉe��� )
	float3 VNormal         : TEXCOORD1 ;        // �@��( �r���[��� )
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;
} ;


// C++ ���Őݒ肷��e�N�X�`���̒�`
sampler  TempDrawScreenTexture         : register( s0 ) ;		// �X�e�[�W���f����`�悵���摜


// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureColor ;
	float2 TexCoord ;
	float4 PPositionN ;


	// PPosition �� xyz �� w �Ŋ����Ĕ񓯎����W�ɂ���
	PPositionN = PSInput.PPosition / PSInput.PPosition.w ;

	
	// ��ʃe�N�X�`���̍��W���Z�o
	// PPositionN.xy �� -1.0f �` 1.0f �̒l�Ȃ̂ŁA����� 0.0f �` 1.0f �̒l�ɂ���
	TexCoord.x = ( PPositionN.x + 1.0f ) / 2.0f;

	// y�͍X�ɏ㉺���]
	TexCoord.y = 1.0f - ( PPositionN.y + 1.0f ) / 2.0f;


	// �e�N�X�`�����W��@�������ɏ������炷
	TexCoord += PSInput.VNormal.xy * 0.01f ;


	// �e�N�X�`���J���[�̓ǂݍ���
	TextureColor = tex2D( TempDrawScreenTexture, TexCoord ) ;


	// �o�̓J���[�̓e�N�X�`���J���[�����������Â���������
	PSOutput.Color0.rgb = TextureColor.rgb * 0.9f ;

	// �o�̓A���t�@�l��1.0f�ŌŒ�
	PSOutput.Color0.a = 1.0f ;

	// �o�̓p�����[�^��Ԃ�
	return PSOutput ;
}


