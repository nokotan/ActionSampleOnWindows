// キーコンフィグ画面

#include "DxLib.h"
#include "KeyConfig.h"

#define KEYCONFIG_FILE_NAME     "KeyConfig.txt"     // キーコンフィグデータのファイル名
#define INFO_X                  (10)                // 情報の描画開始座標
#define INFO_Y                  (10)
#define INFO_SPACE              (24)                // 一行あたりに空けるスペース
#define INFO_NAME_X             (140)               // 入力名を描画するＸ座標

// キーコンフィグ対象の項目
int TargetIndex ;

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

// WinMain 関数
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    int i ;
    int DrawY ;
    int InputWait ;

    // ウインドウモードで起動
    ChangeWindowMode( TRUE ) ;

    // ＤＸライブラリの初期化
    if( DxLib_Init() < 0 ) return -1 ;

    // 描画先を裏画面にする
    SetDrawScreen( DX_SCREEN_BACK ) ;

    // キーコンフィグ処理の初期化を行う
    KeyConfig_Initialize() ;

    // キーコンフィグファイルを読み込む
    if( KeyConfig_Load( KEYCONFIG_FILE_NAME ) == FALSE )
    {
        // コンフィグファイルが読み込めなかったらデフォルト設定にする
        KeyConfig_SetDefault() ;
    }

    // 「何か入力があったら何もしない」フラグを倒す
    InputWait = FALSE ;

    // メインループ
    while( ProcessMessage() == 0 )
    {
        // 画面のクリア
        ClearDrawScreen() ;

        // キーコンフィグの入力処理を行う
        KeyConfig_InputProcess() ;

        // 「何か入力があったら何もしない」フラグが立っている場合は処理を分岐
        if( InputWait == TRUE )
        {
            // 何も入力が無かったら「何か入力があったら何もしない」フラグを倒す
            if( KeyConfig_CheckInput() == FALSE )
            {
                InputWait = FALSE ;
            }
        }
        else
        {
            // キーやパッドの押下状態をチェックして、押下されていたらキーコンフィグ設定に反映させる
            if( KeyConfig_UpdateInputTypeInfo( TargetIndex ) )
            {
                // 何かボタンが押されたら次の項目に移動する
                TargetIndex ++ ;

                // 「何か入力があったら何もしない」フラグを立てる
                InputWait = TRUE ;
            }
        }

        // 項目の数だけ繰り返し
        DrawY = INFO_Y ;
        for( i = 0; i < KEYCONFIG_INPUT_NUM; i++ )
        {
            int DrawColor ;
            char InputString[ 256 ] ;

            // 変更対象の項目かどうかで描画色を変える
            DrawColor = TargetIndex == i ? GetColor( 255,0,0 ) : GetColor( 255,255,255 ) ;

            // 項目名の描画
            DrawString( INFO_X, DrawY, g_KeyConfigMenuTable[ i ], DrawColor ) ;

            // 入力に割り当てられている入力名を取得する
            KeyConfig_GetInputTypeString( i, InputString ) ;

            // 割り当てられている入力名を描画する
            DrawString( INFO_NAME_X, DrawY, InputString, DrawColor ) ;

            // 項目の描画Y座標を１項目分下に移動する
            DrawY += INFO_SPACE ;
        }

        // 裏画面の内容を表画面に反映
        ScreenFlip() ;

        // 全ての入力の設定が終わったらループを抜ける
        if( TargetIndex == KEYCONFIG_INPUT_NUM )
        {
            break ;
        }
    }

    // キーコンフィグ設定を保存する
    KeyConfig_Save( KEYCONFIG_FILE_NAME ) ;

    // ＤＸライブラリの後始末
    DxLib_End() ;

    // ソフトの終了
    return 0 ;
}








