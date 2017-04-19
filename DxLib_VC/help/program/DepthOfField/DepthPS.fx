// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 ViewPosition   : TEXCOORD0;
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0         : COLOR0 ;
};


// C++ 側で設定する定数の定義
float3 cfDOF_Area  : register( c0 ) ;	// 被写界深度の範囲情報   x:被写界深度開始位置   y:被写界深度終了位置   z:被写界深度の範囲の逆数


// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float z_param ;

	// 被写界深度の範囲内を 0.0f ～ 1.0f に変換
	if( PSInput.ViewPosition.z < cfDOF_Area.x )
	{
		PSOutput.Color0.r = 0.0f ;
	}
	else
	if( PSInput.ViewPosition.z > cfDOF_Area.y )
	{
		PSOutput.Color0.r = 1.0f ;
	}
	else
	{
		PSOutput.Color0.r = ( PSInput.ViewPosition.z - cfDOF_Area.x ) * cfDOF_Area.z ;
	}

	PSOutput.Color0.g = 0.0f ;
	PSOutput.Color0.b = 0.0f ;
	PSOutput.Color0.a = 0.0f ;
   
	return PSOutput ;    
}
