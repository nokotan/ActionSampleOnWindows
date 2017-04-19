// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 Diffuse         : COLOR0 ;       // ディフューズカラー
	float4 Specular        : COLOR1 ;       // スペキュラカラー
	float2 TexCoords0      : TEXCOORD0 ;    // テクスチャ座標
	float4 LPPosition      : TEXCOORD1 ;    // ライトからみた座標( xとyはライトの射影座標、zはビュー座標 )
} ;

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;
} ;


// C++ 側で設定する定数やテクスチャの定義
sampler DiffuseMapTexture             : register( s0 ) ;		// ディフューズマップテクスチャ
sampler DepthMapTexture               : register( s1 ) ;		// 深度バッファテクスチャ


// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;
	float TextureDepth ;
	float2 DepthTexCoord ;
	float4 DefaultOutput ;


	// テクスチャカラーの読み込み
	TextureDiffuseColor = tex2D( DiffuseMapTexture, PSInput.TexCoords0.xy ) ;

	// 出力カラー = ディフューズカラー * テクスチャカラー + スペキュラカラー
	DefaultOutput = PSInput.Diffuse * TextureDiffuseColor + PSInput.Specular ;

	// 出力アルファ = ディフューズアルファ * テクスチャアルファ
	DefaultOutput.a = PSInput.Diffuse.a * TextureDiffuseColor.a ;


	// 深度テクスチャの座標を算出
	// PSInput.LPPosition.xy は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
	DepthTexCoord.x = ( PSInput.LPPosition.x + 1.0f ) / 2.0f;

	// yは更に上下反転
	DepthTexCoord.y = 1.0f - ( PSInput.LPPosition.y + 1.0f ) / 2.0f;

	// 深度バッファテクスチャから深度を取得
	TextureDepth = tex2D( DepthMapTexture, DepthTexCoord );

	// テクスチャに記録されている深度( +補正値 )よりＺ値が大きかったら奥にあるということで輝度を半分にする
	if( PSInput.LPPosition.z > TextureDepth + 25.0f )
	{
		DefaultOutput.rgb *= 0.5f;
	}


	// 出力カラーをセット
	PSOutput.Color0 = DefaultOutput;


	// 出力パラメータを返す
	return PSOutput ;
}


