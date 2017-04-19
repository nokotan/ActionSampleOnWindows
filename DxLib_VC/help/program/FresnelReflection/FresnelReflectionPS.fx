// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 MirrorProjectionPosition : TEXCOORD0 ;	// 鏡に映る映像の視点での射影座標
	float3 ViewPosition             : TEXCOORD1 ;	// 座標( ビュー空間 )
	float3 ViewNormal               : TEXCOORD2 ;	// 法線( ビュー空間 )
	float4 Diffuse                  : COLOR0 ;		// ディフューズカラー
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0         : COLOR0 ;
};


// C++ 側で設定する定数の定義
sampler  MirrorTexture : register( s0 ) ;		// 鏡に映すテクスチャ

float4   cfMulColor : register( c0  ) ;			// テクスチャ色に乗算する色


// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float2 TexCoords ;
	float4 TexColor ;
	float refractiveRatio ;

	// テクスチャ座標の算出
	TexCoords.x = PSInput.MirrorProjectionPosition.x / PSInput.MirrorProjectionPosition.w ;
	TexCoords.y = PSInput.MirrorProjectionPosition.y / PSInput.MirrorProjectionPosition.w ;

	// 鏡に映すテクスチャから色を取得
	TexColor = tex2D( MirrorTexture, TexCoords ) ;

	// ディフューズカラーと乗算して出力にセット
	PSOutput.Color0 = TexColor * PSInput.Diffuse ;


	// 水の屈折率 = 1.333    空気の屈折率 = 1.000293
	refractiveRatio = 1.333f / 1.000293f ;

	// フレネル反射率計算( マルペケつくろーどっとコム様のフレネル反射解説から引用 )
	float A = refractiveRatio ;
	float B = dot( -normalize( PSInput.ViewPosition ), normalize( PSInput.ViewNormal ) ) ;
	float C = sqrt( 1.0f - A * A * ( 1 - B * B ) ) ;
	float Rs = ( A * B - C ) * ( A * B - C ) / ( ( A * B + C ) * ( A * B + C ) ) ;
	float Rp = ( A * C - B ) * ( A * C - B ) / ( ( A * C + B ) * ( A * C + B ) ) ;
	float alpha = ( Rs + Rp ) / 2.0f ;

	alpha = min( alpha + 0.20f, 1.0f ) ; 

	// アルファ値をセット
	PSOutput.Color0.a = alpha ;

   
	return PSOutput ;
}
