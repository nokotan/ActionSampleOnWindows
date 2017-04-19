// 鏡面効果基本

#include "DxLib.h"
#include <math.h>


// 画面の解像度
#define SCREEN_W			(640)
#define SCREEN_H			(480)

// 鏡に映る映像の取得に使用するスクリーンの解像度
#define MIRROR_SCREEN_W		(1024)
#define MIRROR_SCREEN_H		(512)

// 鏡の数
#define MIRROR_NUM			(2)

// カメラの速度
#define CAMERA_SPEED		(32.0f)


// キャラクターモデルとステージモデル
int CharaModel ;		
int StageModel ;		

// 鏡処理を行う頂点シェーダーとピクセルシェーダー
int MirrorVS ;
int MirrorPS ;

// 鏡に映る映像の取得に使用するスクリーン
int MirrorHandle[ MIRROR_NUM ] ;

// 鏡に映る映像の取得に使用するクリーンの中の鏡の四隅の座標( 同次座標 )
FLOAT4 MirrorScreenPosW[ MIRROR_NUM ][ 4 ] ;

// 鏡のワールド座標
VECTOR MirrorWorldPos[ MIRROR_NUM ][ 4 ] =
{
	{
		{  2000.0f, 2000.0f, -4498.0f },
		{ -2000.0f, 2000.0f, -4498.0f },
		{  2000.0f,    0.0f, -4498.0f },
		{ -2000.0f,    0.0f, -4498.0f },
	},

	{
		{ -4000.0f, 10.0f,  4500.0f },
		{  4000.0f, 10.0f,  4500.0f },
		{ -4000.0f, 10.0f, -4500.0f },
		{  4000.0f, 10.0f, -4500.0f },
	}
} ;

// 鏡の Diffuse Color
COLOR_U8 MirrorDiffuseColor[ MIRROR_NUM ] =
{
	{ 200, 200, 200, 255 },
	{ 255, 255,   0, 128 },
} ;


// 鏡の初期化
void Mirror_Initialize( void ) ;

// 鏡に映る映像を描画するためのカメラの設定を行う
void Mirror_SetupCamera( int MirrorNo, VECTOR CameraEyePos, VECTOR CameraTargetPos ) ;

// 鏡の描画
void Mirror_Render( int MirrorNo ) ;

// ３Ｄ空間の描画
void World_RenderProcess() ;


// 鏡の初期化
void Mirror_Initialize( void )
{
	int i ;

	// 鏡処理を行うための頂点シェーダーとピクセルシェだーを読み込む
	MirrorVS = LoadVertexShader( "MirrorVS.vso" ) ;
	MirrorPS = LoadPixelShader( "MirrorPS.pso" ) ;

	// 鏡に映る映像の取得に使用するスクリーンの作成
	for( i = 0 ; i < MIRROR_NUM ; i ++ )
	{
		MirrorHandle[ i ] = MakeScreen( MIRROR_SCREEN_W, MIRROR_SCREEN_H, FALSE ) ;
	}
}

// 鏡に映る映像を描画するためのカメラの設定を行う
void Mirror_SetupCamera( int MirrorNo, VECTOR CameraEyePos, VECTOR CameraTargetPos )
{
	int i ;
	float EyeLength, TargetLength ;
	VECTOR MirrorNormal ;
	VECTOR *MirrorWorldPosP ;
	VECTOR MirrorCameraEyePos, MirrorCameraTargetPos ;

	MirrorWorldPosP = MirrorWorldPos[ MirrorNo ] ;

	// 鏡の面の法線を算出
	MirrorNormal = VNorm( VCross( VSub( MirrorWorldPosP[ 1 ], MirrorWorldPosP[ 0 ] ), VSub( MirrorWorldPosP[ 2 ], MirrorWorldPosP[ 0 ] ) ) ) ;

	// 鏡の面からカメラの座標までの最短距離、鏡の面からカメラの注視点までの最短距離を算出
	EyeLength    = Plane_Point_MinLength( MirrorWorldPosP[ 0 ], MirrorNormal, CameraEyePos    ) ;
	TargetLength = Plane_Point_MinLength( MirrorWorldPosP[ 0 ], MirrorNormal, CameraTargetPos ) ;

	// 鏡に映る映像を描画する際に使用するカメラの座標とカメラの注視点を算出
	MirrorCameraEyePos    = VAdd( CameraEyePos,    VScale( MirrorNormal, -EyeLength    * 2.0f ) ) ;
	MirrorCameraTargetPos = VAdd( CameraTargetPos, VScale( MirrorNormal, -TargetLength * 2.0f ) ) ;

	// 計算で得られたカメラの座標とカメラの注視点の座標をカメラの設定する
	SetCameraPositionAndTarget_UpVecY( MirrorCameraEyePos, MirrorCameraTargetPos ) ;

	// 鏡に映る映像の中での鏡の四隅の座標を算出( 同次座標 )
	for( i = 0 ; i < 4 ; i ++ )
	{
		MirrorScreenPosW[ MirrorNo ][ i ] = ConvWorldPosToScreenPosPlusW( MirrorWorldPosP[ i ] ) ;
	}
}

// 鏡の描画
void Mirror_Render( int MirrorNo )
{
	int i ;
	VERTEX3DSHADER Vert[ 6 ] ;

	// 鏡の描画に使用する頂点のセットアップ
	for( i = 0 ; i < 4 ; i ++ )
	{
		// 座標をセット
		Vert[ i ].pos = MirrorWorldPos[ MirrorNo ][ i ] ;

		// Diffuse Color をセット
		Vert[ i ].dif = MirrorDiffuseColor[ MirrorNo ] ;

		// 補助座標に鏡に映る視点での射影座標を代入する
		Vert[ i ].spos.x = MirrorScreenPosW[ MirrorNo ][ i ].x / MIRROR_SCREEN_W ;
		Vert[ i ].spos.y = MirrorScreenPosW[ MirrorNo ][ i ].y / MIRROR_SCREEN_H ;
		Vert[ i ].spos.z = MirrorScreenPosW[ MirrorNo ][ i ].z ;
		Vert[ i ].spos.w = MirrorScreenPosW[ MirrorNo ][ i ].w ;
	}
	Vert[ 4 ] = Vert[ 2 ] ;
	Vert[ 5 ] = Vert[ 1 ] ;

	// 描画モードをバイリニアフィルタリングに設定
	SetDrawMode( DX_DRAWMODE_BILINEAR ) ;

	// 描画ブレンドモードをセット
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 ) ;

	// 描画にＺバッファを使用する
	SetUseZBuffer3D( TRUE ) ;

	// Ｚバッファに書き込みを行う
	SetWriteZBuffer3D( TRUE ) ;

	// 使用する頂点シェーダーをセット
	SetUseVertexShader( MirrorVS ) ;

	// 使用するピクセルシェーダーをセット
	SetUsePixelShader( MirrorPS ) ;

	// 使用するテクスチャをセット
	SetUseTextureToShader( 0, MirrorHandle[ MirrorNo ] ) ;

	// 鏡を描画
	DrawPolygon3DToShader( Vert, 2 ) ;

	// Ｚバッファに書き込みを行う設定を元に戻す
	SetWriteZBuffer3D( FALSE ) ;

	// Ｚバッファを使用する設定を元に戻す
	SetUseZBuffer3D( FALSE ) ;

	// 描画ブレンドモードを元に戻す
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 ) ;

	// 描画モードを元に戻す
	SetDrawMode( DX_DRAWMODE_NEAREST ) ;
}

// ３Ｄ空間の描画
void World_RenderProcess()
{
	// ステージモデルの描画
	MV1DrawModel( StageModel ) ;

	// キャラクターモデルの描画
	MV1DrawModel( CharaModel ) ;
}


// WinMain関数
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	int CameraAngle ;		// カメラの水平角度
	VECTOR CameraEyePosition ;	// カメラの座標
	VECTOR CameraDirection ;	// カメラの向いている方向
	VECTOR CameraTargetPosition ;	// カメラの注視点
	int i ;

	// ウインドウモードで起動
	ChangeWindowMode( TRUE );

	// 画面の解像度と色ビット深度を設定
	SetGraphMode( SCREEN_W, SCREEN_H, 32 ) ;

	// ＤＸライブラリの初期化
	if( DxLib_Init() < 0 )
		return -1 ;

	// シェーダーモデル２．０が使用できるかどうかをチェック
	if( GetValidShaderVersion() < 200 )
	{
		DrawString( 0, 0, "シェーダーモデル２．０が使用できません", GetColor( 255,255,255 ) ) ;
		DxLib_End() ;
		return 0 ;
	}

	// 鏡処理の初期化
	Mirror_Initialize() ;

	// キャラクターモデルの読み込み
	CharaModel = MV1LoadModel( "DxChara.x" ) ;

	// ステージモデルの読み込み
	StageModel = MV1LoadModel( "ColTestStage.mqo" ) ;

	// キャラクターモデルの設置
	MV1SetPosition( CharaModel, VGet( 0.0f, 0.0f, -3500.0f ) ) ;
	MV1SetRotationXYZ( CharaModel, VGet( 0.0f, 3.14159f / 2.0f, 0.0f ) ) ;
	MV1AttachAnim( CharaModel, 7 ) ;

	// カメラの角度を初期化
	CameraAngle = 325 ;

	// カメラの座標をセット
	CameraEyePosition = VGet( -2605.0f, 670.0f, -2561.0f ) ;

	// ライトの向きをセット
	SetLightDirection( VGet( 1.0f, -1.0f, -1.0f ) ) ;

	// メインループ(何かキーが押されたらループを抜ける)
	while( ProcessMessage() == 0 )
	{
		// 左右キーが押されたらカメラを回転する
		if( CheckHitKey( KEY_INPUT_LEFT  ) )
		{
			CameraAngle += 3 ;
			if( CameraAngle > 360 )
			{
				CameraAngle -= 360 ;
			}
		}
		if( CheckHitKey( KEY_INPUT_RIGHT ) )
		{
			CameraAngle -= 3 ;
			if( CameraAngle < 0 )
			{
				CameraAngle += 360 ;
			}
		}

		// カメラの向いている方向を算出
		CameraDirection.x = cos( CameraAngle * 3.14159f / 180.0f ) ;
		CameraDirection.y = 0.0f ;
		CameraDirection.z = sin( CameraAngle * 3.14159f / 180.0f ) ;

		// 上下キーが押されたらカメラを前進・後進させる
		if( CheckHitKey( KEY_INPUT_UP ) )
		{
			CameraEyePosition = VAdd( CameraEyePosition, VScale( CameraDirection, CAMERA_SPEED ) ) ;
		}
		if( CheckHitKey( KEY_INPUT_DOWN ) )
		{
			CameraEyePosition = VSub( CameraEyePosition, VScale( CameraDirection, CAMERA_SPEED ) ) ;
		}

		// カメラの注視点座標を算出
		CameraTargetPosition = VAdd( CameraEyePosition, CameraDirection ) ;


		// 鏡に映る映像を描画
		for( i = 0 ; i < MIRROR_NUM ; i ++ )
		{
			// 描画先を鏡に映る映像の取得に使用する画像を描画するスクリーンにする
			SetDrawScreen( MirrorHandle[ i ] ) ;

			// 画面を初期化する
			ClearDrawScreen() ;

			// 鏡に映る映像を描画する際に使用するカメラの設定を行う
			Mirror_SetupCamera( i, CameraEyePosition, CameraTargetPosition ) ;

			// ３Ｄ空間の描画
			World_RenderProcess() ;
		}


		// 裏画面を描画対象にする
		SetDrawScreen( DX_SCREEN_BACK ) ;

		// 画面をクリア
		ClearDrawScreen() ;

		// カメラの設定を行う
		SetCameraPositionAndTarget_UpVecY( CameraEyePosition, CameraTargetPosition ) ;

		// ３Ｄ空間の描画
		World_RenderProcess() ;

		// 鏡の描画
		for( i = 0 ; i < MIRROR_NUM ; i ++ )
		{
			Mirror_Render( i ) ;
		}


		// 裏画面の内容を表画面に反映
		ScreenFlip();
	}

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}
