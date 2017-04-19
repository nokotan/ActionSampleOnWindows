#include "DxLib.h"

// ========================================================================================
//
//		マリオ風アクションゲームのサンプル
//
//     ---------------------------------------------------- 制作: のこたん (2014年入部)
//
// 　ブロック、プレイヤーだけのシンプルなアクションゲームです。いろいろいじくってもらいたい
// 　ので、あえて問題点を残しています。まずは、その問題点を改善してみてください
//
// =================================================================== Ver 1.0 (2017/ 4/18)



// プレイヤー構造体
struct Player {
	// プレイヤーの座標
	int x, y;
	// プレイヤーの移動速度
	int dx, dy;
	// プレイヤーの大きさ
	int width, height;
};

// ブロック構造体
struct Block {
	// ブロックの配置する座標
	int x, y;
	// ブロックの大きさ
	int width, height;
};

// Tips! ----------------------------------------------------
//
// 　DXライブラリを使ったアプリケーションのエントリポイントは
//		int main() 
// ではなく、
//		int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
// になります。
//
// ----------------------------------------------------------

// エントリポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// アプリケーションをウィンドウで起動する。
	// (これを省略するとフルスクリーンで起動します。)
	ChangeWindowMode(TRUE);
	// ウィンドウのタイトルバーに表示される文字を "Action Sample" に変更。
	SetMainWindowText("Action Sample");
	// ウィンドウの大きさを 640x480 に変更
	SetGraphMode(640, 480, 32);

	// DXライブラリを初期化。初期化に失敗したとき、すぐにアプリケーションを終了する。
	if (DxLib_Init() == -1) {
		return -1;
	}

	// 描画する先を裏画面にして、画面のちらつきを防止する。
	// (記事 http://dxlib.o.oo7.jp/dxtec.html#M5)
	SetDrawScreen(DX_SCREEN_BACK);

	// ToDo *************************************
	//       ゲーム中に使う変数をここに追加
	// ******************************************
	Player Mario = { 0, 0, 0, 0, 32, 64 };

	// 配置するブロックの数
	const int BlockNum = 2;
	Block Blocks[BlockNum];

	Blocks[0] = { 0, 450, 640, 30 };
	Blocks[1] = { 160, 350, 320, 30 };

	// メインループ
	while (ProcessMessage() != -1) {

		// プレイヤーの位置の更新とあたり判定 ---------------
		//
		// 　１フレーム前の位置と速度からこのフレームでのプ
		// レイヤーの位置を割り出します。
		//
		// 計算式: x(t) = x(t - dt) + v(t - dt)
		//			(テイラー展開の2次以下の項を省略した形)
		//
		// --------------------------------------------------

		// このフレームでのプレイヤーの配置座標
		int MarioNewX, MarioNewY;

		// プレイヤーがブロックにぶつかったかどうか
		bool CollidedX = false, CollidedY = false;

		// すべてのブロックに対してあたり判定を実行する
		for (int i = 0; i < BlockNum; i++) {
			// プレイヤーが下向きに移動しているときのあたり判定
			if (Mario.dy > 0) {
				// x軸方向に重なっているか？
				if ((Blocks[i].x < Mario.x + Mario.width) && (Mario.x < Blocks[i].x + Blocks[i].width)) {
					// y軸方向にまたいだか？
					if ((Mario.y + Mario.height <= Blocks[i].y) && (Blocks[i].y < Mario.y + Mario.dy + Mario.height)) {
						CollidedY = true;
						MarioNewY = Blocks[i].y - Mario.height;
					}
				}
			}
		}

		// ぶつかっていなければ、先ほどの計算式を適用
		if (CollidedX == false) {
			MarioNewX = Mario.x + Mario.dx;
		}

		if (CollidedY == false) {
			MarioNewY = Mario.y + Mario.dy;
		}

		Mario.x = MarioNewX;
		Mario.y = MarioNewY;

		// プレイヤーが画面外に行かないように補正
		if (Mario.x < 0) {
			Mario.x = 0;
		}

		if (Mario.x + Mario.width > 640) {
			Mario.x = 640 - Mario.width;
		}

		// --------------------------------------------------

		// プレイヤーの速度の更新 ---------------------------

		// あたり判定でぶつかっていたら速度を 0 にする
		if (CollidedX == true) {
			Mario.dx = 0;
		}

		if (CheckHitKey(KEY_INPUT_LEFT)) {
			// もし [←] が押されていたら左に加速
			Mario.dx = -2;
		} else if (CheckHitKey(KEY_INPUT_RIGHT)) {
			// もし [→] が押されていたら右に加速
			Mario.dx = 2;
		} else {
			Mario.dx = 0;
		}

		// あたり判定でぶつかっていたら速度を 0 にする
		if (CollidedY == true) {
			Mario.dy = 0;
		}

		// もし [Space] が押されていたらジャンプ
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			Mario.dy = -10;
		} else {
			// 何も押されていなければ、下向きに加速
			Mario.dy += 1;

			// Tips ------------------------------------------------------------
			// 
			// 　本来なら重力加速度は 9.8 m/(s^2) ですが、
			// それだと速すぎるため、あえて 1 pixel/(frame^2) を指定しています。
			//
			// -----------------------------------------------------------------
		}

		// 加速しすぎないように、速度を調整する
		if (Mario.dy > 10) {
			Mario.dy = 10;
		}

		// --------------------------------------------------

		// 描画 ---------------------------------------------

		// 画面に描かれているものを消去
		ClearDrawScreen();

		// プレイヤーを描画
		DrawBox(Mario.x, Mario.y, Mario.x + Mario.width, Mario.y + Mario.height, GetColor(255, 255, 255), TRUE);

		// すべてのブロックを描画
		for (int i = 0; i < BlockNum; i++) {
			DrawBox(Blocks[i].x, Blocks[i].y, Blocks[i].x + Blocks[i].width, Blocks[i].y + Blocks[i].height, GetColor(0, 255, 0), TRUE);
		}

		// --------------------------------------------------

		// 画面に描いたものをウィンドウにコピーする。
		ScreenFlip();
	}

	// DXライブラリの後片付け
	DxLib_End();

	return 0;
}