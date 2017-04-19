// ここにサンプルプログラムをペーストしてください
// 初期状態では LoadGraphScreen のサンプルプログラムが入力されています。
#include "DxLib.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				 LPSTR lpCmdLine, int nCmdShow )
{
	int BlendGraph, GrHandle1, GrHandle2;
	int i;

	ChangeWindowMode(TRUE);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1) return -1;

	// 裏画面を使用
	SetDrawScreen(DX_SCREEN_BACK);

	// ブレンド画像を読み込む
	BlendGraph = LoadBlendGraph("BlendGraph.bmp");

	// 二つの画像の読み込み
	GrHandle1 = LoadGraph("Scene1.jpg");
	GrHandle2 = LoadGraph("Scene2.jpg");

	// 256 フレーム掛けて切り替え
	for (i = 0; i < 256; i++)
	{
		// メッセージ処理
		if (ProcessMessage() != 0) break;

		// Scene2.jpg を描画
		DrawGraph(0, 0, GrHandle2, FALSE);

		// Scene1.jpg と BlendGraph.bmp を合成して描画
		// (境界幅は 64)
		DrawBlendGraph(0, 0, GrHandle1, FALSE, BlendGraph, i, 64);

		// 裏画面の内容を表画面に反映
		ScreenFlip();
	}

	// ＤＸライブラリ使用の終了処理
	DxLib_End();


	// ソフトの終了
	return 0;
}
