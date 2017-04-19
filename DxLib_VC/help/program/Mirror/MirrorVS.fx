// 頂点シェーダーの入力
struct VS_INPUT
{
	float4 Position                 : POSITION0 ;		// 座標( ローカル空間 )
	float4 MirrorProjectionPosition : POSITION1 ;		// 鏡に映る映像の視点での射影座標
	float3 Normal                   : NORMAL0 ;			// 法線( ローカル空間 )
	float4 Diffuse                  : COLOR0 ;			// ディフューズカラー
	float4 Specular                 : COLOR1 ;			// スペキュラカラー
	float4 TexCoords0               : TEXCOORD0 ;		// テクスチャ座標
} ;

// 頂点シェーダーの出力
struct VS_OUTPUT
{
	float4 ProjectionPosition       : POSITION ;	// 射影座標
	float4 MirrorProjectionPosition : TEXCOORD0 ;	// 鏡に映る映像の視点での射影座標
	float4 Diffuse                  : COLOR0 ;		// ディフューズカラー
} ;



// C++ 側で設定する定数の定義
float4              cfProjectionMatrix[ 4 ] : register( c2  ) ;		// ビュー　　→　射影行列
float4              cfViewMatrix[ 3 ]       : register( c6  ) ;		// ワールド　→　ビュー行列
float4              cfLocalWorldMatrix[ 3 ] : register( c94 ) ;		// ローカル　→　ワールド行列


// main関数
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 lWorldPosition ;
	float4 lViewPosition ;


	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ローカル座標をワールド座標に変換
	lWorldPosition.x = dot( VSInput.Position, cfLocalWorldMatrix[ 0 ] ) ;
	lWorldPosition.y = dot( VSInput.Position, cfLocalWorldMatrix[ 1 ] ) ;
	lWorldPosition.z = dot( VSInput.Position, cfLocalWorldMatrix[ 2 ] ) ;
	lWorldPosition.w = 1.0f ;

	// ワールド座標をビュー座標に変換
	lViewPosition.x = dot( lWorldPosition, cfViewMatrix[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, cfViewMatrix[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, cfViewMatrix[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	// ビュー座標を射影座標に変換
	VSOutput.ProjectionPosition.x = dot( lViewPosition, cfProjectionMatrix[ 0 ] ) ;
	VSOutput.ProjectionPosition.y = dot( lViewPosition, cfProjectionMatrix[ 1 ] ) ;
	VSOutput.ProjectionPosition.z = dot( lViewPosition, cfProjectionMatrix[ 2 ] ) ;
	VSOutput.ProjectionPosition.w = dot( lViewPosition, cfProjectionMatrix[ 3 ] ) ;
	
	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )

	// ディフューズカラーはそのまま渡す
	VSOutput.Diffuse = VSInput.Diffuse ;

	// 鏡に映る映像の視点での射影座標
	VSOutput.MirrorProjectionPosition = VSInput.MirrorProjectionPosition ;


	// 出力パラメータを返す
	return VSOutput ;
}
