// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 Diffuse         : COLOR0 ;       // ディフューズカラー
	float4 Specular        : COLOR1 ;       // スペキュラカラー
	float2 TexCoords0      : TEXCOORD0 ;    // テクスチャ座標
	float4 DPPosition0     : TEXCOORD1 ;    // 深度バッファ０レンダリング時の座標( 射影空間 )
	float4 DPPosition1     : TEXCOORD2 ;    // 深度バッファ１レンダリング時の座標( 射影空間 )
	float4 DPPosition2     : TEXCOORD3 ;    // 深度バッファ２レンダリング時の座標( 射影空間 )
	float4 DPPosition3     : TEXCOORD4 ;    // 深度バッファ３レンダリング時の座標( 射影空間 )
	float4 DPPosition4     : TEXCOORD5 ;    // 深度バッファ４レンダリング時の座標( 射影空間 )
	float4 DPPosition5     : TEXCOORD6 ;    // 深度バッファ５レンダリング時の座標( 射影空間 )
} ;

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;
} ;


// C++ 側で設定する定数やテクスチャの定義
sampler DiffuseMapTexture             : register( s0 ) ;		// ディフューズマップテクスチャ
sampler DepthMapTexture0              : register( s1 ) ;		// 深度バッファ０テクスチャ
sampler DepthMapTexture1              : register( s2 ) ;		// 深度バッファ１テクスチャ
sampler DepthMapTexture2              : register( s3 ) ;		// 深度バッファ２テクスチャ
sampler DepthMapTexture3              : register( s4 ) ;		// 深度バッファ３テクスチャ
sampler DepthMapTexture4              : register( s5 ) ;		// 深度バッファ４テクスチャ
sampler DepthMapTexture5              : register( s6 ) ;		// 深度バッファ５テクスチャ


// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;
	float TextureDepth ;
	float2 DepthTexCoord ;
	float4 DefaultOutput ;
	float3 DPPosition ;


	// テクスチャカラーの読み込み
	TextureDiffuseColor = tex2D( DiffuseMapTexture, PSInput.TexCoords0.xy ) ;

	// 出力カラー = ディフューズカラー * テクスチャカラー + スペキュラカラー
	DefaultOutput = PSInput.Diffuse * TextureDiffuseColor + PSInput.Specular ;

	// 出力アルファ = ディフューズアルファ * テクスチャアルファ
	DefaultOutput.a = PSInput.Diffuse.a * TextureDiffuseColor.a ;


	// 深度バッファ０に描画座標が映っているかを判定
	DPPosition.x = PSInput.DPPosition0.x / PSInput.DPPosition0.w;
	DPPosition.y = PSInput.DPPosition0.y / PSInput.DPPosition0.w;
	DPPosition.z = PSInput.DPPosition0.z / PSInput.DPPosition0.w;
	if( DPPosition.x > -1.0f && DPPosition.x < 1.0f &&
		DPPosition.y > -1.0f && DPPosition.y < 1.0f &&
		DPPosition.z >  0.0f && DPPosition.z < 1.0f )
	{
		// 映っている場合は深度バッファ０を適用する

		// 深度バッファ０テクスチャの座標を算出
		// PSInput.DPPosition.xy は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
		DepthTexCoord.x = ( DPPosition.x + 1.0f ) / 2.0f;

		// yは更に上下反転
		DepthTexCoord.y = 1.0f - ( DPPosition.y + 1.0f ) / 2.0f;

		// 深度バッファ０テクスチャから深度を取得
		TextureDepth = tex2D( DepthMapTexture0, DepthTexCoord );

		// テクスチャに記録されている深度( +補正値 )よりＺ値が大きかったら奥にあるということで輝度を半分にする
		if( DPPosition.z > TextureDepth + 0.001f )
		{
			DefaultOutput.rgb *= 0.5f;
		}
	}

	// 深度バッファ１に描画座標が映っているかを判定
	DPPosition.x = PSInput.DPPosition1.x / PSInput.DPPosition1.w;
	DPPosition.y = PSInput.DPPosition1.y / PSInput.DPPosition1.w;
	DPPosition.z = PSInput.DPPosition1.z / PSInput.DPPosition1.w;
	if( DPPosition.x > -1.0f && DPPosition.x < 1.0f &&
		DPPosition.y > -1.0f && DPPosition.y < 1.0f &&
		DPPosition.z >  0.0f && DPPosition.z < 1.0f )
	{
		DepthTexCoord.x = ( DPPosition.x + 1.0f ) / 2.0f;
		DepthTexCoord.y = 1.0f - ( DPPosition.y + 1.0f ) / 2.0f;
		TextureDepth = tex2D( DepthMapTexture1, DepthTexCoord );

		if( DPPosition.z > TextureDepth + 0.001f )
		{
			DefaultOutput.rgb *= 0.5f;
		}
	}

	// 深度バッファ２に描画座標が映っているかを判定
	DPPosition.x = PSInput.DPPosition2.x / PSInput.DPPosition2.w;
	DPPosition.y = PSInput.DPPosition2.y / PSInput.DPPosition2.w;
	DPPosition.z = PSInput.DPPosition2.z / PSInput.DPPosition2.w;
	if( DPPosition.x > -1.0f && DPPosition.x < 1.0f &&
		DPPosition.y > -1.0f && DPPosition.y < 1.0f &&
		DPPosition.z >  0.0f && DPPosition.z < 1.0f )
	{
		DepthTexCoord.x = ( DPPosition.x + 1.0f ) / 2.0f;
		DepthTexCoord.y = 1.0f - ( DPPosition.y + 1.0f ) / 2.0f;
		TextureDepth = tex2D( DepthMapTexture2, DepthTexCoord );

		if( DPPosition.z > TextureDepth + 0.001f )
		{
			DefaultOutput.rgb *= 0.5f;
		}
	}

	// 深度バッファ３に描画座標が映っているかを判定
	DPPosition.x = PSInput.DPPosition3.x / PSInput.DPPosition3.w;
	DPPosition.y = PSInput.DPPosition3.y / PSInput.DPPosition3.w;
	DPPosition.z = PSInput.DPPosition3.z / PSInput.DPPosition3.w;
	if( DPPosition.x > -1.0f && DPPosition.x < 1.0f &&
		DPPosition.y > -1.0f && DPPosition.y < 1.0f &&
		DPPosition.z >  0.0f && DPPosition.z < 1.0f )
	{
		DepthTexCoord.x = ( DPPosition.x + 1.0f ) / 2.0f;
		DepthTexCoord.y = 1.0f - ( DPPosition.y + 1.0f ) / 2.0f;
		TextureDepth = tex2D( DepthMapTexture3, DepthTexCoord );

		if( DPPosition.z > TextureDepth + 0.001f )
		{
			DefaultOutput.rgb *= 0.5f;
		}
	}

	// 深度バッファ４に描画座標が映っているかを判定
	DPPosition.x = PSInput.DPPosition4.x / PSInput.DPPosition4.w;
	DPPosition.y = PSInput.DPPosition4.y / PSInput.DPPosition4.w;
	DPPosition.z = PSInput.DPPosition4.z / PSInput.DPPosition4.w;
	if( DPPosition.x > -1.0f && DPPosition.x < 1.0f &&
		DPPosition.y > -1.0f && DPPosition.y < 1.0f &&
		DPPosition.z >  0.0f && DPPosition.z < 1.0f )
	{
		DepthTexCoord.x = ( DPPosition.x + 1.0f ) / 2.0f;
		DepthTexCoord.y = 1.0f - ( DPPosition.y + 1.0f ) / 2.0f;
		TextureDepth = tex2D( DepthMapTexture4, DepthTexCoord );

		if( DPPosition.z > TextureDepth + 0.001f )
		{
			DefaultOutput.rgb *= 0.5f;
		}
	}

	// 深度バッファ５に描画座標が映っているかを判定
	DPPosition.x = PSInput.DPPosition5.x / PSInput.DPPosition5.w;
	DPPosition.y = PSInput.DPPosition5.y / PSInput.DPPosition5.w;
	DPPosition.z = PSInput.DPPosition5.z / PSInput.DPPosition5.w;
	if( DPPosition.x > -1.0f && DPPosition.x < 1.0f &&
		DPPosition.y > -1.0f && DPPosition.y < 1.0f &&
		DPPosition.z >  0.0f && DPPosition.z < 1.0f )
	{
		DepthTexCoord.x = ( DPPosition.x + 1.0f ) / 2.0f;
		DepthTexCoord.y = 1.0f - ( DPPosition.y + 1.0f ) / 2.0f;
		TextureDepth = tex2D( DepthMapTexture5, DepthTexCoord );

		if( DPPosition.z > TextureDepth + 0.001f )
		{
			DefaultOutput.rgb *= 0.5f;
		}
	}


	// 出力カラーをセット
	PSOutput.Color0 = DefaultOutput;


	// 出力パラメータを返す
	return PSOutput ;
}


