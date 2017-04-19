// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 ViewPosition             : TEXCOORD0 ;	// 座標( ビュー空間 )
	float4 ProjectionPositionSub    : TEXCOORD1 ;	// 座標( 射影空間 )ピクセルシェーダーで参照する為の物
	float4 Diffuse                  : COLOR0 ;		// ディフューズカラー
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0         : COLOR0 ;
};


// C++ 側で設定する定数の定義
sampler  DepthTexture      : register( s0 ) ;   // 深度テクスチャ

float MaxOpacityDistance   : register( c0 ) ;   // 不透明度が最大になる水中の距離
float MinOpacity           : register( c1 ) ;   // 最低不透明度
float2 DepthTextureUseSize : register( c2 ) ;	// 深度テクスチャで使用しているサイズ


// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float2 TexCoords ;
	float4 Depth ;
	float refractiveRatio ;

	// 深度テクスチャ座標の算出
	TexCoords.x = (  PSInput.ProjectionPositionSub.x / PSInput.ProjectionPositionSub.w + 1.0f ) / 2.0f ;
	TexCoords.y = ( -PSInput.ProjectionPositionSub.y / PSInput.ProjectionPositionSub.w + 1.0f ) / 2.0f ;

	// 深度テクスチャから深度を取得
	Depth = tex2D( DepthTexture, TexCoords * DepthTextureUseSize ) ;

	// 不透明度を計算
	PSOutput.Color0.a = saturate( ( 1.0f - MinOpacity ) * ( Depth.r - PSInput.ViewPosition.z ) / MaxOpacityDistance + MinOpacity ) ;

	// 色のセット
	PSOutput.Color0.rgb = PSInput.Diffuse.rgb ;
   
	return PSOutput ;
}
