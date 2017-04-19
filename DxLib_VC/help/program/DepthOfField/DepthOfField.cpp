#include "DxLib.h"
#include <math.h>


// キャラクターのＸ座標
#define CHARAMODEL_X		(-3500.0f)

// キャラクターの１人目のＺ座標
#define CHARAMODEL_Z		(-3500.0f)

// 描画するキャラクターの数
#define CHARAMODEL_NUM		(8)

// キャラクター１人あたりのスペース
#define CHARAMODEL_SPACE	(800.0f)


// カメラの速度
#define CAMERA_SPEED		(32.0f)


// 画面の横解像度
#define SCREEN_W			(640)

// 画面の縦解像度
#define SCREEN_H			(480)

// 描画用テクスチャの横解像度
#define SCREEN_TEX_W		(1024)

// 描画用テクスチャの縦解像度
#define SCREEN_TEX_H		(512)


// ステージモデルとキャラクターモデルのハンドル
int StageModelHandle ;
int CharacterModelHandle ;


// 剛体メッシュ用の深度値描画用頂点シェーダーハンドル
int Depth_NormalMeshVS ;

// スキニングメッシュ用の深度値描画用頂点シェーダーハンドル
int Depth_SkinMeshVS ;

// 深度値描画用ピクセルシェーダーハンドル
int DepthPS ;

// 被写界深度処理用のピクセルシェーダーハンドル
int DepthOfFieldPS ;


// 最もぼかしが強いところとぼかし無しのところの補間距離
float DepthOfField_InterpSize ;

// ぼかし無しの範囲の中心位置
float DepthOfField_Focus ;

// ぼかし無しの範囲
float DepthOfField_FocusSize ;

// 補間範囲とぼかし無しの範囲の合計距離
float DepthOfField_TotalSize ;


// 深度値描画用テクスチャ
int DepthMapTexture ;

// 0:通常描画用テクスチャ
// 1:通常描画に弱いぼかしを加えた画像保存用のテクスチャ
// 2:通常描画に強いぼかしを加えた画像保存用のテクスチャ
int ColorMapTexture[ 3 ] ;


int CameraAngle ;		// カメラの水平角度
VECTOR CameraEyePosition ;	// カメラの座標
VECTOR CameraDirection ;	// カメラの向いている方向
VECTOR CameraTargetPosition ;	// カメラの注視点


// 指定の画面に３Ｄモデルを描画
void DrawScreen( int ScreenGraphHandle, int UseDepthShader )
{
	int i ;
	FLOAT4 ParamF ;

	// 描画対象を変更
	SetDrawScreen( ScreenGraphHandle ) ;

	// 描画範囲を設定
	SetDrawArea( 0, 0, SCREEN_W, SCREEN_H ) ;

	// 消失点座標を設定
	SetCameraScreenCenter( SCREEN_W / 2.0f, SCREEN_H / 2.0f ) ;

	// 表示画面に合うようにスケーリング
	SetDraw3DScale( ( float )SCREEN_H / SCREEN_TEX_H ) ;

	// 画面を初期化
	ClearDrawScreen() ;

	// カメラの設定を行う
	SetCameraPositionAndTarget_UpVecY( CameraEyePosition, CameraTargetPosition ) ;

	// 深度値を描画するかどうかで処理を分岐
	if( UseDepthShader != 0 )
	{
		// 深度値を描画する場合はオリジナルのシェーダーを使用してモデルを描画する設定にする
		MV1SetUseOrigShader( TRUE ) ;

		// 深度値を描画するためのピクセルシェーダーをセット
		SetUsePixelShader( DepthPS ) ;

		// 被写界深度の開始位置を計算
		ParamF.x = DepthOfField_Focus - DepthOfField_FocusSize / 2.0f - DepthOfField_InterpSize ;

		// 被写界深度の終了位置を計算
		ParamF.y = DepthOfField_Focus + DepthOfField_FocusSize / 2.0f + DepthOfField_InterpSize ;

		// 被写界深度の範囲の逆数を計算
		ParamF.z = 1.0f / ( ParamF.y - ParamF.x ) ;

		// ピクセルシェーダーの定数にセット
		SetPSConstF( 0, ParamF ) ;
	}
	else
	{
		// 深度値を描画するわけではない場合はオリジナルのシェーダーは使用しない
		MV1SetUseOrigShader( FALSE ) ;
	}

	// 深度値を描画する場合は頂点シェーダーを剛体メッシュ用の深度値描画用頂点シェーダーにする
	if( UseDepthShader != 0 )
	{
		SetUseVertexShader( Depth_NormalMeshVS ) ;
	}

	// ステージモデル( 剛体メッシュ )を描画
	MV1DrawModel( StageModelHandle ) ;


	// 深度値を描画する場合は頂点シェーダーをスキニングメッシュ用の深度値描画用頂点シェーダーにする
	if( UseDepthShader != 0 )
	{
		SetUseVertexShader( Depth_SkinMeshVS ) ;
	}

	// キャラクターモデル( スキニングメッシュ )を描画
	for( i = 0 ; i < CHARAMODEL_NUM ; i ++ )
	{
		MV1SetPosition( CharacterModelHandle, VGet( CHARAMODEL_X, 0.0f, CHARAMODEL_Z + i * CHARAMODEL_SPACE ) ) ;
		MV1DrawModel( CharacterModelHandle ) ;
	}
}

// WinMain 関数
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	FLOAT4 ParamF ;
	float RightU, BottomV ;
	VERTEX2DSHADER Vertex[ 6 ] ;


	// ウインドウモードで起動
	ChangeWindowMode( TRUE ) ;

	// 画面モードを設定
	SetGraphMode( SCREEN_W, SCREEN_H, 32 ) ;

	// ＤＸライブラリの初期化
	if( DxLib_Init() < 0 )
	{
		// エラーが発生したら直ちに終了
		return -1 ;
	}

	// シェーダーモデル２．０が使用できるかどうかをチェック
	if( GetValidShaderVersion() < 200 )
	{
		DrawString( 0, 0, "シェーダーモデル２．０が使用できません", GetColor( 255,255,255 ) ) ;
		DxLib_End() ;
		return 0 ;
	}

	// 通常描画用のテクスチャを作成
	ColorMapTexture[ 0 ] = MakeScreen( SCREEN_TEX_W, SCREEN_TEX_H, TRUE ) ;

	// 通常描画した映像に弱いぼかしを加えた画像を保存するためのテクスチャを作成
	ColorMapTexture[ 1 ] = MakeScreen( SCREEN_TEX_W, SCREEN_TEX_H, TRUE ) ;

	// 通常描画した映像に強いぼかしを加えた画像を保存するためのテクスチャを作成
	ColorMapTexture[ 2 ] = MakeScreen( SCREEN_TEX_W, SCREEN_TEX_H, TRUE ) ;

	// 深度値を描画する１チャンネル画像を作成
	SetCreateDrawValidGraphChannelNum( 1 ) ;
	DepthMapTexture = MakeScreen( SCREEN_TEX_W, SCREEN_TEX_H, TRUE ) ;
	SetCreateDrawValidGraphChannelNum( 0 ) ;

	// ステージモデルとキャラクターモデルの読み込み
	StageModelHandle     = MV1LoadModel( "ColTestStage.mqo" ) ;
	CharacterModelHandle = MV1LoadModel( "DxChara.x" ) ;

	// 深度値描画用の頂点シェーダーとピクセルシェーダーを読み込み
	Depth_NormalMeshVS = LoadVertexShader( "Depth_NormalMeshVS.vso" ) ;
	Depth_SkinMeshVS   = LoadVertexShader( "Depth_SkinMeshVS.vso" ) ;
	DepthPS            = LoadPixelShader( "DepthPS.pso" ) ;

	// 深度値を使用して被写界深度処理を行うためのピクセルシェーダーを読み込み
	DepthOfFieldPS     = LoadPixelShader( "DepthOfFieldPS.pso" ) ;

	// 被写界深度処理用のパラメータを初期化
	DepthOfField_InterpSize = 3000.0f ;
	DepthOfField_Focus = 3200.0f ;
	DepthOfField_FocusSize = 500.0f ;

	// カメラの角度を初期化
	CameraAngle = 90 ;

	// カメラの座標をセット
	CameraEyePosition = VGet( -2900.0f, 570.0f, -4600.0f ) ;

	// メインループ
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

		// パラメータ操作処理
		{
			if( CheckHitKey( KEY_INPUT_A ) == 1 )
			{
				DepthOfField_Focus += 5.0f ;
			}

			if( CheckHitKey( KEY_INPUT_Z ) == 1 )
			{
				DepthOfField_Focus -= 5.0f ;
			}

			if( CheckHitKey( KEY_INPUT_S ) == 1 )
			{
				DepthOfField_FocusSize += 5.0f ;
			}

			if( CheckHitKey( KEY_INPUT_X ) == 1 )
			{
				DepthOfField_FocusSize -= 5.0f ;
			}

			if( CheckHitKey( KEY_INPUT_D ) == 1 )
			{
				DepthOfField_InterpSize += 5.0f ;
			}

			if( CheckHitKey( KEY_INPUT_C ) == 1 )
			{
				DepthOfField_InterpSize -= 5.0f ;
			}
		}

		// 被写界深度描画の準備
		{
			// 補間範囲とフォーカスが合っている範囲を含めた総距離を算出
			DepthOfField_TotalSize = DepthOfField_InterpSize * 2.0f + DepthOfField_FocusSize ;

			// 通常描画用テクスチャにモデルを通常描画
			DrawScreen( ColorMapTexture[ 0 ], FALSE ) ;

			// 通常描画の結果にぼかしを加えた画像を保存
			GraphFilterRectBlt( ColorMapTexture[ 0 ], ColorMapTexture[ 1 ], 0, 0, SCREEN_W, SCREEN_H, 0, 0, DX_GRAPH_FILTER_GAUSS, 16, 200 ) ;

			// ぼかしを加えた画像に更にぼかしを加えて保存
			GraphFilterRectBlt( ColorMapTexture[ 1 ], ColorMapTexture[ 2 ], 0, 0, SCREEN_W, SCREEN_H, 0, 0, DX_GRAPH_FILTER_GAUSS, 16, 200 ) ;

			// 深度値描画用テクスチャに深度値を描画
			DrawScreen( DepthMapTexture, TRUE ) ;
		}
		

		// 描画先を裏画面に変更
		SetDrawScreen( DX_SCREEN_BACK ) ;

		// 被写界深度を施した画像を裏画面に描画
		{
			// 使用するテクスチャをセット
			SetUseTextureToShader( 0, ColorMapTexture[ 0 ] ) ;
			SetUseTextureToShader( 1, ColorMapTexture[ 1 ] ) ;
			SetUseTextureToShader( 2, ColorMapTexture[ 2 ] ) ;
			SetUseTextureToShader( 3, DepthMapTexture ) ;

			// 被写界深度処理用のピクセルシェーダーをセット
			SetUsePixelShader( DepthOfFieldPS ) ;

			// 被写界深度のぼかし無しの開始位置を計算
			ParamF.x = DepthOfField_InterpSize / DepthOfField_TotalSize ;

			// 被写界深度のぼかし無しの終了位置を計算
			ParamF.y = ( DepthOfField_InterpSize + DepthOfField_FocusSize ) / DepthOfField_TotalSize ;

			// ピクセルシェーダーの定数にセット
			SetPSConstF( 0, ParamF ) ;

			// 画面全体に描画する頂点の準備
			Vertex[ 0 ].pos = VGet(     0.0f,     0.0f, 0.0f ) ;
			Vertex[ 1 ].pos = VGet( SCREEN_W,     0.0f, 0.0f ) ;
			Vertex[ 2 ].pos = VGet(     0.0f, SCREEN_H, 0.0f ) ;
			Vertex[ 3 ].pos = VGet( SCREEN_W, SCREEN_H, 0.0f ) ;

			Vertex[ 0 ].rhw = 1.0f ;
			Vertex[ 1 ].rhw = 1.0f ;
			Vertex[ 2 ].rhw = 1.0f ;
			Vertex[ 3 ].rhw = 1.0f ;

			RightU  = ( float )SCREEN_W / SCREEN_TEX_W ;
			BottomV = ( float )SCREEN_H / SCREEN_TEX_H ;
			Vertex[ 0 ].u = 0.0f ;		Vertex[ 0 ].v = 0.0f ;
			Vertex[ 1 ].u = RightU ;	Vertex[ 1 ].v = 0.0f ;
			Vertex[ 2 ].u = 0.0f ;		Vertex[ 2 ].v = BottomV ;
			Vertex[ 3 ].u = RightU ;	Vertex[ 3 ].v = BottomV ;

			Vertex[ 4 ] = Vertex[ 2 ] ;
			Vertex[ 5 ] = Vertex[ 1 ] ;

			// ポリゴン２枚を使用して画面全体に被写界深度処理を施した３Ｄシーンの描画
			DrawPolygon2DToShader( Vertex, 2 ) ;
		}

		// パラメータを描画
		DrawFormatString( 0, 0, GetColor( 255,255,255 ), "補間距離:%.1f ぼかし無し中心:%.3f ぼかし無し範囲:%.3f",
							DepthOfField_InterpSize, DepthOfField_Focus, DepthOfField_FocusSize ) ;

		// キー操作説明を描画
		DrawString( 0, 480 - 16, "AZキー:ぼかし無し中心変更　SXキー:ぼかし無し範囲変更　DCキー:補間距離変更", GetColor( 255,255,255 ) ) ;
		DrawString( 0, 480 - 32, "方向キー:カメラの操作", GetColor( 255,255,255 ) ) ;

		// 裏画面の内容を表画面に反映する
		ScreenFlip() ;
	}


	// ＤＸライブラリの後始末
	DxLib_End() ;

	// ソフトの終了
	return 0 ;
}
