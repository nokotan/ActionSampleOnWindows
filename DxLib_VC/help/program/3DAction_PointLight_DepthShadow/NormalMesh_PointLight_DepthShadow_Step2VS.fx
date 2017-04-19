// 頂点シェーダーの入力
struct VS_INPUT
{
	float4 Position        : POSITION ;     // 座標( ローカル空間 )
	float3 Normal          : NORMAL0 ;      // 法線( ローカル空間 )
	float4 Diffuse         : COLOR0 ;       // ディフューズカラー
	float4 Specular        : COLOR1 ;       // スペキュラカラー
	float4 TexCoords0      : TEXCOORD0 ;	// テクスチャ座標
} ;

// 頂点シェーダーの出力
struct VS_OUTPUT
{
	float4 Position        : POSITION ;     // 座標( 射影空間 )
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

// マテリアルパラメータ
struct VS_CONST_MATERIAL
{
	float4 Diffuse ;                // マテリアルディフューズカラー
	float4 Specular ;               // マテリアルスペキュラカラー
	float4 Power ;                  // マテリアルスペキュラハイライトの強さ
} ;

// ライトパラメータ
struct VS_CONST_LIGHT
{
	float4 Position ;               // 座標( ビュー空間 )
	float3 Direction ;              // 方向( ビュー空間 )
	float4 Diffuse ;                // ディフューズカラー
	float4 Specular ;               // スペキュラカラー
	float4 Ambient ;                // アンビエントカラーとマテリアルのアンビエントカラーを乗算したもの
	float4 Range_FallOff_AT0_AT1 ;  // x:有効距離  y:スポットライト用FallOff  z:距離による減衰処理用パラメータ０  w:距離による減衰処理用パラメータ１
	float4 AT2_SpotP0_SpotP1 ;      // x:距離による減衰処理用パラメータ２  y:スポットライト用パラメータ０( cos( Phi / 2.0f ) )  z:スポットライト用パラメータ１( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )
} ;



// C++ 側で設定する定数の定義
float4              cfAmbient_Emissive      : register( c1  ) ;		// マテリアルエミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー
float4              cfProjectionMatrix[ 4 ] : register( c2  ) ;		// ビュー　　→　射影行列
float4              cfViewMatrix[ 3 ]       : register( c6  ) ;		// ワールド　→　ビュー行列
float4              cfLocalWorldMatrix[ 3 ] : register( c94 ) ;		// ローカル　→　ワールド行列
VS_CONST_MATERIAL   cfMaterial              : register( c11 ) ;		// マテリアルパラメータ
VS_CONST_LIGHT      cfLight                 : register( c14 ) ;		// 有効ライト０番のパラメータ

matrix              cfDepthViewMatrix0       : register( c43  ) ;	// 深度バッファ０レンダリング時のワールド　→　ビュー行列
matrix              cfDepthProjectionMatrix0 : register( c47  ) ;	// 深度バッファ０レンダリング時のビュー　　→　射影行列
matrix              cfDepthViewMatrix1       : register( c51  ) ;	// 深度バッファ１レンダリング時のワールド　→　ビュー行列
matrix              cfDepthProjectionMatrix1 : register( c55  ) ;	// 深度バッファ１レンダリング時のビュー　　→　射影行列
matrix              cfDepthViewMatrix2       : register( c59  ) ;	// 深度バッファ２レンダリング時のワールド　→　ビュー行列
matrix              cfDepthProjectionMatrix2 : register( c63  ) ;	// 深度バッファ２レンダリング時のビュー　　→　射影行列
matrix              cfDepthViewMatrix3       : register( c67  ) ;	// 深度バッファ３レンダリング時のワールド　→　ビュー行列
matrix              cfDepthProjectionMatrix3 : register( c71  ) ;	// 深度バッファ３レンダリング時のビュー　　→　射影行列
matrix              cfDepthViewMatrix4       : register( c75  ) ;	// 深度バッファ４レンダリング時のワールド　→　ビュー行列
matrix              cfDepthProjectionMatrix4 : register( c79  ) ;	// 深度バッファ４レンダリング時のビュー　　→　射影行列
matrix              cfDepthViewMatrix5       : register( c83  ) ;	// 深度バッファ５レンダリング時のワールド　→　ビュー行列
matrix              cfDepthProjectionMatrix5 : register( c87  ) ;	// 深度バッファ５レンダリング時のビュー　　→　射影行列


// main関数
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 lWorldPosition ;
	float4 lViewPosition ;
	float4 lDViewPosition ;
	float3 lWorldNrm ;
	float3 lViewNrm ;
	float3 lLightHalfVec ;
	float4 lLightLitParam ;
	float4 lLightLitDest ;
	float3 lLightDir ;
	float3 lLightTemp ;
	float lLightDistancePow2 ;
	float lLightGen ;

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
	VSOutput.Position.x = dot( lViewPosition, cfProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( lViewPosition, cfProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( lViewPosition, cfProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( lViewPosition, cfProjectionMatrix[ 3 ] ) ;

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )



	// ライトの処理 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// 法線をビュー空間の角度に変換 =========================================( 開始 )

	// ローカルベクトルをワールドベクトルに変換
	lWorldNrm.x = dot( VSInput.Normal, cfLocalWorldMatrix[ 0 ].xyz ) ;
	lWorldNrm.y = dot( VSInput.Normal, cfLocalWorldMatrix[ 1 ].xyz ) ;
	lWorldNrm.z = dot( VSInput.Normal, cfLocalWorldMatrix[ 2 ].xyz ) ;

	// ワールドベクトルをビューベクトルに変換
	lViewNrm.x = dot( lWorldNrm, cfViewMatrix[ 0 ].xyz ) ;
	lViewNrm.y = dot( lWorldNrm, cfViewMatrix[ 1 ].xyz ) ;
	lViewNrm.z = dot( lWorldNrm, cfViewMatrix[ 2 ].xyz ) ;

	// 法線をビュー空間の角度に変換 =========================================( 終了 )

	// ライト方向ベクトルの計算
	lLightDir = normalize( lViewPosition.xyz - cfLight.Position.xyz ) ;


	// 距離減衰値計算 ===================================================================( 開始 )

	// 頂点とライト位置との距離の二乗を求める
	lLightTemp = lViewPosition.xyz - cfLight.Position.xyz ;
	lLightDistancePow2 = dot( lLightTemp, lLightTemp ) ;

	// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
	lLightGen = 1.0f / ( cfLight.Range_FallOff_AT0_AT1.z + cfLight.Range_FallOff_AT0_AT1.w * sqrt( lLightDistancePow2 ) + cfLight.AT2_SpotP0_SpotP1.x * lLightDistancePow2 ) ;

	// 有効距離外だったら減衰率を最大にする処理
	lLightGen *= step( lLightDistancePow2, cfLight.Range_FallOff_AT0_AT1.x ) ;

	// 距離減衰値計算 ===================================================================( 終了 )


	// ライトディフューズカラーとライトスペキュラカラーの角度減衰計算 ===================( 開始 )

	// 法線とライトの逆方向ベクトルとの内積を lLightLitParam.x にセット
	lLightLitParam.x = dot( lViewNrm, -lLightDir ) ;

	// ハーフベクトルの計算 norm( ( norm( 頂点位置から視点へのベクトル ) + ライトの方向 ) )
	lLightHalfVec = normalize( normalize( -lViewPosition.xyz ) - lLightDir ) ;

	// 法線とハーフベクトルの内積を lLightLitParam.y にセット
	lLightLitParam.y = dot( lLightHalfVec, lViewNrm ) ;

	// スペキュラ反射率を lLightLitParam.w にセット
	lLightLitParam.w = cfMaterial.Power.x ;

	// ライトパラメータ計算
	lLightLitDest = lit( lLightLitParam.x, lLightLitParam.y, lLightLitParam.w ) ;

	// ライトディフューズカラーとライトスペキュラカラーの角度減衰計算 ===================( 終了 )

	// ライトの処理 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )



	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ディフューズカラー =
	//            距離減衰値 *
	//            ( ディフューズ角度減衰計算結果 *
	//              ライトのディフューズカラー *
	//              マテリアルのディフューズカラー +
	//              ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの ) +
	//            マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの
	VSOutput.Diffuse = lLightGen * ( lLightLitDest.y * cfLight.Diffuse * cfMaterial.Diffuse + cfLight.Ambient ) + cfAmbient_Emissive ;

	// ディフューズアルファはマテリアルのディフューズカラーのアルファをそのまま使う
	VSOutput.Diffuse.w = cfMaterial.Diffuse.w ;

	// スペキュラカラー = 距離減衰値 * スペキュラ角度減衰計算結果 * ライトのスペキュラカラー * マテリアルのスペキュラカラー
	VSOutput.Specular = lLightGen * lLightLitDest.z * cfLight.Specular * cfMaterial.Specular ;


	// テクスチャ座標のセット
	VSOutput.TexCoords0 = VSInput.TexCoords0;

	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )



	// 深度バッファレンダリング時のカメラ設定での射影行列を算出 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	lDViewPosition = mul( cfDepthViewMatrix0, lWorldPosition ) ;
	VSOutput.DPPosition0 = mul( cfDepthProjectionMatrix0, lDViewPosition ) ;

	lDViewPosition = mul( cfDepthViewMatrix1, lWorldPosition ) ;
	VSOutput.DPPosition1 = mul( cfDepthProjectionMatrix1, lDViewPosition ) ;

	lDViewPosition = mul( cfDepthViewMatrix2, lWorldPosition ) ;
	VSOutput.DPPosition2 = mul( cfDepthProjectionMatrix2, lDViewPosition ) ;

	lDViewPosition = mul( cfDepthViewMatrix3, lWorldPosition ) ;
	VSOutput.DPPosition3 = mul( cfDepthProjectionMatrix3, lDViewPosition ) ;

	lDViewPosition = mul( cfDepthViewMatrix4, lWorldPosition ) ;
	VSOutput.DPPosition4 = mul( cfDepthProjectionMatrix4, lDViewPosition ) ;

	lDViewPosition = mul( cfDepthViewMatrix5, lWorldPosition ) ;
	VSOutput.DPPosition5 = mul( cfDepthProjectionMatrix5, lDViewPosition ) ;

	// 深度バッファレンダリング時のカメラ設定での射影行列を算出 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )


	// 出力パラメータを返す
	return VSOutput ;
}

