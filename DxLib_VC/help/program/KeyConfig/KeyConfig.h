// キーコンフィグ処理のヘッダファイル

#ifndef KEYCONFIG_H
#define KEYCONFIG_H

#include "DxLib.h"

// 入力情報タイプ
#define KEYCONFIG_INPUT_LEFT            (0)         // 方向入力左
#define KEYCONFIG_INPUT_RIGHT           (1)         // 方向入力右
#define KEYCONFIG_INPUT_UP              (2)         // 方向入力上
#define KEYCONFIG_INPUT_DOWN            (3)         // 方向入力下
#define KEYCONFIG_INPUT_CAMERA_LEFT     (4)         // カメラ用方向入力左
#define KEYCONFIG_INPUT_CAMERA_RIGHT    (5)         // カメラ用方向入力右
#define KEYCONFIG_INPUT_CAMERA_UP       (6)         // カメラ用方向入力上
#define KEYCONFIG_INPUT_CAMERA_DOWN     (7)         // カメラ用方向入力下
#define KEYCONFIG_INPUT_ATTACK          (8)         // 攻撃ボタン
#define KEYCONFIG_INPUT_JUMP            (9)         // ジャンプボタン
#define KEYCONFIG_INPUT_NUM             (10)        // 入力タイプの数

extern void   KeyConfig_Initialize( void );             // キーコンフィグの初期化を行う
extern void   KeyConfig_InputProcess( void );           // キーコンフィグの入力処理を行う( 毎フレーム実行する )
extern int    KeyConfig_GetInput( void );               // ボタンの入力状態を取得する( 戻り値 : ボタンの入力状態 )
extern int    KeyConfig_GetEdgeInput( void );           // ボタンの押された瞬間のみの入力状態を取得する( 戻り値 : ボタンの押された瞬間のみの入力状態 )
extern VECTOR KeyConfig_GetCameraInput( void );         // カメラ操作用の入力状態を取得する( 戻り値 : カメラ操作用の入力状態 )
extern int    KeyConfig_Save( const char *FilePath );   // キーコンフィグ設定をファイルに保存する( 戻り値 : 保存成功：TRUE   保存失敗：FALSE )
extern int    KeyConfig_Load( const char *FilePath );   // キーコンフィグ設定をファイルから読み込む( 戻り値 : 読み込み成功：TRUE   読み込み失敗：FALSE )
extern void   KeyConfig_SetDefault( void );             // キーコンフィグ設定をデフォルトにする

// 現在押下されているキーやパッドの入力情報をチェックして、入力があった場合は情報更新する( キーコンフィグ画面用 )
//     戻り値 : 入力があったかどうか( TRUE:入力があった  FALSE:入力は無かった )
extern int    KeyConfig_UpdateInputTypeInfo( int UpdateInputType );

// パッドかキーの入力があるかどうかを取得する( キーコンフィグ画面用 )
//     戻り値 : 入力があったかどうか( TRUE:入力があった  FALSE:入力は無かった )
extern int    KeyConfig_CheckInput( void ) ;

// 指定の入力情報タイプに割り当てられているパッドやキーの入力の名前を取得する( キーコンフィグ画面用 )
extern void   KeyConfig_GetInputTypeString( int InputType, char *InputString );

#endif
