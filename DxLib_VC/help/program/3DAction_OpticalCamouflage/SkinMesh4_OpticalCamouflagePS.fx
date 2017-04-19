// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 PPosition       : TEXCOORD0 ;        // 座標( 射影空間 )
	float3 VNormal         : TEXCOORD1 ;        // 法線( ビュー空間 )
} ;

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;
} ;


// C++ 側で設定するテクスチャの定義
sampler  TempDrawScreenTexture         : register( s0 ) ;		// ステージモデルを描画した画像


// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureColor ;
	float2 TexCoord ;
	float4 PPositionN ;


	// PPosition の xyz を w で割って非同次座標にする
	PPositionN = PSInput.PPosition / PSInput.PPosition.w ;

	
	// 画面テクスチャの座標を算出
	// PPositionN.xy は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
	TexCoord.x = ( PPositionN.x + 1.0f ) / 2.0f;

	// yは更に上下反転
	TexCoord.y = 1.0f - ( PPositionN.y + 1.0f ) / 2.0f;


	// テクスチャ座標を法線方向に少しずらす
	TexCoord += PSInput.VNormal.xy * 0.01f ;


	// テクスチャカラーの読み込み
	TextureColor = tex2D( TempDrawScreenTexture, TexCoord ) ;


	// 出力カラーはテクスチャカラーを少しだけ暗くしたもの
	PSOutput.Color0.rgb = TextureColor.rgb * 0.9f ;

	// 出力アルファ値は1.0fで固定
	PSOutput.Color0.a = 1.0f ;

	// 出力パラメータを返す
	return PSOutput ;
}


