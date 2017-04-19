// ピクセルシェーダーの入力
struct PS_INPUT
{
	float2 TexCoords0      : TEXCOORD0 ;
} ;

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;
} ;

// C++ 側で設定するテクスチャの定義
float2   cfDOF_Focus : register( c0 ) ;		// 被写界深度のフォーカス情報   x:フォーカス開始位置   y:フォーカス終了位置

sampler  ColorMapTexture0            : register( s0 ) ;		// カラーマップテクスチャ( くっきりテクスチャ )
sampler  ColorMapTexture1            : register( s1 ) ;		// カラーマップテクスチャ( 弱いぼかしのテクスチャ )
sampler  ColorMapTexture2            : register( s2 ) ;		// カラーマップテクスチャ( 強いぼかしのテクスチャ )
sampler  DepthMapTexture             : register( s3 ) ;		// 深度マップテクスチャ

// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 Color1, Color2 ;
	float BlendRate ;
	float Fade ;
	float Depth ;

	// 深度値を取得する
	Depth = tex2D( DepthMapTexture, PSInput.TexCoords0 ) ;

	// フォーカス情報からぼやけ率を算出
	if( Depth < cfDOF_Focus.x )
	{
		Fade = 1.0f - Depth / cfDOF_Focus.x ;
	}
	else
	if( Depth < cfDOF_Focus.y )
	{
		Fade = 0.0f;
	}
	else
	{
		Fade = ( Depth - cfDOF_Focus.y ) / ( 1.0f - cfDOF_Focus.y ) ;
	}

	// ぼやけ率から色を算出
	if( Fade < 0.5f )
	{
		// ぼやけ率が 0.5f 以下の場合はぼかし無し画像と弱いぼかし画像を合成する
		Color1 = tex2D( ColorMapTexture0, PSInput.TexCoords0 ) ;
		Color2 = tex2D( ColorMapTexture1, PSInput.TexCoords0 ) ;
		BlendRate = Fade / 0.5f ;
	}
	else
	{
		// ぼやけ率が 0.5f 以上の場合は弱いぼかし画像と強いぼかし画像を合成する
		Color1 = tex2D( ColorMapTexture1, PSInput.TexCoords0 ) ;
		Color2 = tex2D( ColorMapTexture2, PSInput.TexCoords0 ) ;
		BlendRate = ( Fade - 0.5f ) / 0.5f ;
	}

	// 合成した色の値を算出
	PSOutput.Color0 = lerp( Color1, Color2, BlendRate ) ;
   
	return PSOutput ;    
}
