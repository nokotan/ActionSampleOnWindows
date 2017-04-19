// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 MirrorProjectionPosition : TEXCOORD0 ;	// 鏡に映る映像の視点での射影座標
	float4 Diffuse                  : COLOR0 ;		// ディフューズカラー
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0         : COLOR0 ;
};


// C++ 側で設定する定数の定義
sampler  MirrorTexture : register( s0 ) ;		// 鏡に映すテクスチャ


// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float2 TexCoords ;
	float4 TexColor ;

	// テクスチャ座標の算出
	TexCoords.x = PSInput.MirrorProjectionPosition.x / PSInput.MirrorProjectionPosition.w ;
	TexCoords.y = PSInput.MirrorProjectionPosition.y / PSInput.MirrorProjectionPosition.w ;

	// 鏡に映すテクスチャから色を取得
	TexColor = tex2D( MirrorTexture, TexCoords ) ;

	// ディフューズカラーと乗算して出力にセット
	PSOutput.Color0 = TexColor * PSInput.Diffuse ;
   
	return PSOutput ;    
}
