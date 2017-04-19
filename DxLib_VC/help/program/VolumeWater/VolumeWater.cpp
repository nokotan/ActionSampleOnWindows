// 水の深さによって透明度を調整するシェーダー

#include "DxLib.h"
#include <math.h>


// 画面の解像度
#define SCREEN_W			(640)
#define SCREEN_H			(480)

// 深度の取得に使用するスクリーンの解像度
#define DEPTH_SCREEN_W		(1024)
#define DEPTH_SCREEN_H		(512)

// カメラの速度
#define CAMERA_SPEED		(32.0f)


// キャラクターモデルとステージモデル
int CharaModel ;		
int StageModel ;		

// 深度の描画に使用するシェーダー
int Depth_NormalMeshVS ;
int Depth_SkinMeshVS ;
int DepthPS ;

// 深度を見て水面を描画するシェーダー
int VolumeWaterVS ;
int VolumeWaterPS ;

// 深度を取得するスクリーン
int DepthScreen ;

// 水面のワールド座標
VECTOR WaterWorldPos[ 4 ] =
{
	{ -8000.0f, 400.0f,  8000.0f },
	{  8000.0f, 400.0f,  8000.0f },
	{ -8000.0f, 400.0f, -8000.0f },
	{  8000.0f, 400.0f, -8000.0f },
} ;

// 水面の Diffuse Color
COLOR_U8 MirrorDiffuseColor =
{
	255, 255,   0, 255,
} ;

// ３Ｄ空間の描画
void World_RenderProcess( int DepthDraw )
{
	// 深度描画の場合は深度描画用の頂点シェーダーをセットする
	if( DepthDraw )
	{
		MV1SetUseOrigShader( TRUE ) ;

		SetUsePixelShader( DepthPS ) ;
		SetUseTextureToShader( 0, -1 ) ;
	}
	else
	{
		MV1SetUseOrigShader( FALSE ) ;
	}

	// ステージモデルの描画
	if( DepthDraw ) 
	{
		SetUseVertexShader( Depth_NormalMeshVS ) ;
	}
	MV1DrawModel( StageModel ) ;

	// キャラクターモデルの描画
	if( DepthDraw ) 
	{
		SetUseVertexShader( Depth_SkinMeshVS ) ;
	}
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

	// 深度を描画するためのシェーダーを読み込む
	Depth_NormalMeshVS = LoadVertexShader( "Depth_NormalMeshVS.vso" ) ;
	Depth_SkinMeshVS = LoadVertexShader( "Depth_SkinMeshVS.vso" ) ;
	DepthPS = LoadPixelShader( "DepthPS.pso" ) ;

	// 深度を描画するテクスチャの作成( １チャンネル浮動小数点１６ビットテクスチャ )
	SetCreateDrawValidGraphChannelNum( 1 ) ;
	SetDrawValidFloatTypeGraphCreateFlag( TRUE ) ;
	SetCreateGraphChannelBitDepth( 16 ) ;
	DepthScreen = MakeScreen( DEPTH_SCREEN_W, DEPTH_SCREEN_H, FALSE ) ;
	SetCreateDrawValidGraphChannelNum( 0 ) ;
	SetDrawValidFloatTypeGraphCreateFlag( FALSE ) ;
	SetCreateGraphChannelBitDepth( 0 ) ;

	// 深度を見て水面を描画するシェーダーの読み込み
	VolumeWaterVS = LoadVertexShader( "VolumeWaterVS.vso" ) ;
	VolumeWaterPS = LoadPixelShader( "VolumeWaterPS.pso" ) ;

	// キャラクターモデルの読み込み
	CharaModel = MV1LoadModel( "DxChara.x" ) ;

	// ステージモデルの読み込み
	StageModel = MV1LoadModel( "Stage.mqo" ) ;

	// ステージモデルの配置
	MV1SetPosition( StageModel, VGet( 0.0f, 800.0f, 0.0f ) ) ;

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

		// 少し下向きにする
		CameraTargetPosition.y -= 0.3f ;
		
		// 深度を描画
		{
			// 描画先を深度スクリーンに変更
			SetDrawScreen( DepthScreen ) ;

			// 画面を初期化する
			ClearDrawScreen() ;

			// 描画範囲を画面サイズに限定
			SetDrawArea( 0, 0, SCREEN_W, SCREEN_H ) ;

			// 消失点を画面サイズの中心に変更
			SetCameraScreenCenter( SCREEN_W / 2.0f, SCREEN_H / 2.0f ) ;

			// 表示画面に合うようにスケーリング
			SetDraw3DScale( ( float )SCREEN_H / DEPTH_SCREEN_H ) ;

			// カメラの設定を行う
			SetCameraPositionAndTarget_UpVecY( CameraEyePosition, CameraTargetPosition ) ;

			// モデルの描画
			World_RenderProcess( TRUE ) ;

			// スケーリング設定を元に戻す
			SetDraw3DScale( 1.0f ) ;
		}
		
		// 通常モデルの描画
		{
			// 描画先を裏画面に変更
			SetDrawScreen( DX_SCREEN_BACK ) ;

			// カメラの設定を行う
			SetCameraPositionAndTarget_UpVecY( CameraEyePosition, CameraTargetPosition ) ;

			// 画面を初期化する
			ClearDrawScreen() ;

			// モデルの描画
			World_RenderProcess( FALSE ) ;
		}
		
		// 水面の描画
		{
			VERTEX3DSHADER Vertex[ 6 ] ;
			FLOAT4 Param ;

			// 頂点データの準備
			for( i = 0 ; i < 4 ; i ++ )
			{
				Vertex[ i ].pos = WaterWorldPos[ i ] ;
				Vertex[ i ].dif = MirrorDiffuseColor ;
			}
			Vertex[ 4 ] = Vertex[ 2 ] ;
			Vertex[ 5 ] = Vertex[ 1 ] ;

			// 使用するシェーダーのセット
			SetUseVertexShader( VolumeWaterVS ) ;
			SetUsePixelShader( VolumeWaterPS ) ;

			// 使用するテクスチャとして深度スクリーンをセット
			SetUseTextureToShader( 0, DepthScreen ) ;

			// 不透明度が最大になる距離をセット
			Param.x = 6000.0f ;
			Param.y = 0.0f ;
			Param.z = 0.0f ;
			Param.w = 0.0f ;
			SetPSConstF( 0, Param ) ;

			// 最低不透明度をセット
			Param.x = 0.1f ;
			SetPSConstF( 1, Param ) ;

			// 深度テクスチャの使用しているサイズをセット
			Param.x = ( float )SCREEN_W / DEPTH_SCREEN_W ;
			Param.y = ( float )SCREEN_H / DEPTH_SCREEN_H ;
			SetPSConstF( 2, Param ) ;

			// 描画ブレンドモードをアルファブレンドにセット
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 ) ;

			// Ｚバッファを使用する
			SetUseZBufferFlag( TRUE ) ;

			// Ｚバッファに書き込みも行う
			SetWriteZBufferFlag( TRUE ) ;

			// ポリゴンの描画
			DrawPolygon3DToShader( Vertex, 2 ) ;

			// 描画ブレンドモードを戻す
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 ) ;
		}
	
		// 裏画面の内容を表画面に反映
		ScreenFlip();
	}

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}
