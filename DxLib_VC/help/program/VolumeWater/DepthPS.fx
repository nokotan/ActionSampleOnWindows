// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 ViewPosition   : TEXCOORD0 ;
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0         : COLOR0 ;
};


// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;

	// 奥行き値を書き込み
	PSOutput.Color0.r = PSInput.ViewPosition.z ;
	PSOutput.Color0.g = 0.0f ;
	PSOutput.Color0.b = 0.0f ;
	PSOutput.Color0.a = 1.0 ;
   
	return PSOutput ;    
}
