// キーコンフィグのテスト

#include "DxLib.h"
#include "KeyConfig.h"

#define KEYCONFIG_FILE_NAME     "KeyConfig.txt"     // キーコンフィグデータのファイル名
#define INFO_X                  (10)                // 描画開始座標
#define INFO_Y                  (10)
#define INFO_SPACE              (24)                // １行あたりに空けるスペース
#define INFO_NAME_X             (140)               // 入力名を描画するＸ座標
#define INFO_INPUT_X            (320)               // 入力状態を描画するＸ座標

// キーコンフィグ各項目の名前
static const char *g_KeyConfigMenuTable[ KEYCONFIG_INPUT_NUM ] =
{
    "左",
    "右",
    "上",
    "下",
    "カメラ左旋回",
    "カメラ右旋回",
    "カメラ上旋回",
    "カメラ下旋回",
    "攻撃",
    "ジャンプ",
} ;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    int i ;
    int Input ;
    int DrawY ;
    VECTOR CameraInput ;

    // ウインドウモードで起動
    ChangeWindowMode( TRUE );

    // ＤＸライブラリの初期化
    if( DxLib_Init() < 0 ) return -1 ;

    // 描画先を裏画面にする
    SetDrawScreen( DX_SCREEN_BACK ) ;

    // キーコンフィグ処理を初期化
    KeyConfig_Initialize() ;

    // キーコンフィグファイルを読み込む
    if( KeyConfig_Load( KEYCONFIG_FILE_NAME ) == FALSE )
    {
        // コンフィグファイルが読み込めなかったらデフォルト設定にする
        KeyConfig_SetDefault() ;
    }

    // メインループ
    while( ProcessMessage() == 0 )
    {
        // 画面のクリア
        ClearDrawScreen() ;

        // キーコンフィグの入力処理を行う
        KeyConfig_InputProcess() ;

        // 入力状態を取得する
        Input = KeyConfig_GetInput() ;

        // 項目の数だけ繰り返し
        DrawY = INFO_Y ;
        for( i = 0; i < KEYCONFIG_INPUT_NUM; i++ )
        {
            char InputString[ 256 ] ;

            // 項目名の描画
            DrawString( INFO_X, DrawY, g_KeyConfigMenuTable[ i ], GetColor( 255,255,255 ) ) ;

            // 入力に割り当てられている入力名を取得する
            KeyConfig_GetInputTypeString( i, InputString ) ;

            // 割り当てられている入力名を描画する
            DrawString( INFO_NAME_X, DrawY, InputString, GetColor( 255,255,255 ) ) ;

            // 入力状態を描画する
            DrawString( INFO_INPUT_X, DrawY, ( Input & ( 1 << i ) ) != 0 ? "○" : "＿", GetColor( 255,255,255 ) ) ;

            // 項目の描画Y座標を１項目分下に移動する
            DrawY += INFO_SPACE ;
        }

        // カメラ用方向入力の状態を描画する
        CameraInput = KeyConfig_GetCameraInput() ;
        DrawFormatString( INFO_X, DrawY, GetColor( 255,255,255 ), "カメラ入力  X:%.3f  Y:%.3f", CameraInput.x, CameraInput.y ) ;

        // 裏画面の内容を表画面に反映
        ScreenFlip() ;
    }

    // ＤＸライブラリの後始末
    DxLib_End();

    // ソフトの終了
    return 0;
}
