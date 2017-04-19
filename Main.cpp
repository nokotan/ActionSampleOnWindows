#include "DxLib.h"

// ========================================================================================
//
//		�}���I���A�N�V�����Q�[���̃T���v��
//
//     ---------------------------------------------------- ����: �̂����� (2014�N����)
//
// �@�u���b�N�A�v���C���[�����̃V���v���ȃA�N�V�����Q�[���ł��B���낢�낢�������Ă��炢����
// �@�̂ŁA�����Ė��_���c���Ă��܂��B�܂��́A���̖��_�����P���Ă݂Ă�������
//
// =================================================================== Ver 1.0 (2017/ 4/18)



// �v���C���[�\����
struct Player {
	// �v���C���[�̍��W
	int x, y;
	// �v���C���[�̈ړ����x
	int dx, dy;
	// �v���C���[�̑傫��
	int width, height;
};

// �u���b�N�\����
struct Block {
	// �u���b�N�̔z�u������W
	int x, y;
	// �u���b�N�̑傫��
	int width, height;
};

// Tips! ----------------------------------------------------
//
// �@DX���C�u�������g�����A�v���P�[�V�����̃G���g���|�C���g��
//		int main() 
// �ł͂Ȃ��A
//		int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
// �ɂȂ�܂��B
//
// ----------------------------------------------------------

// �G���g���|�C���g
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// �A�v���P�[�V�������E�B���h�E�ŋN������B
	// (������ȗ�����ƃt���X�N���[���ŋN�����܂��B)
	ChangeWindowMode(TRUE);
	// �E�B���h�E�̃^�C�g���o�[�ɕ\������镶���� "Action Sample" �ɕύX�B
	SetMainWindowText("Action Sample");
	// �E�B���h�E�̑傫���� 640x480 �ɕύX
	SetGraphMode(640, 480, 32);

	// DX���C�u�������������B�������Ɏ��s�����Ƃ��A�����ɃA�v���P�[�V�������I������B
	if (DxLib_Init() == -1) {
		return -1;
	}

	// �`�悷���𗠉�ʂɂ��āA��ʂ̂������h�~����B
	// (�L�� http://dxlib.o.oo7.jp/dxtec.html#M5)
	SetDrawScreen(DX_SCREEN_BACK);

	// ToDo *************************************
	//       �Q�[�����Ɏg���ϐ��������ɒǉ�
	// ******************************************
	Player Mario = { 0, 0, 0, 0, 32, 64 };

	// �z�u����u���b�N�̐�
	const int BlockNum = 2;
	Block Blocks[BlockNum];

	Blocks[0] = { 0, 450, 640, 30 };
	Blocks[1] = { 160, 350, 320, 30 };

	// ���C�����[�v
	while (ProcessMessage() != -1) {

		// �v���C���[�̈ʒu�̍X�V�Ƃ����蔻�� ---------------
		//
		// �@�P�t���[���O�̈ʒu�Ƒ��x���炱�̃t���[���ł̃v
		// ���C���[�̈ʒu������o���܂��B
		//
		// �v�Z��: x(t) = x(t - dt) + v(t - dt)
		//			(�e�C���[�W�J��2���ȉ��̍����ȗ������`)
		//
		// --------------------------------------------------

		// ���̃t���[���ł̃v���C���[�̔z�u���W
		int MarioNewX, MarioNewY;

		// �v���C���[���u���b�N�ɂԂ��������ǂ���
		bool CollidedX = false, CollidedY = false;

		// ���ׂẴu���b�N�ɑ΂��Ă����蔻������s����
		for (int i = 0; i < BlockNum; i++) {
			// �v���C���[���������Ɉړ����Ă���Ƃ��̂����蔻��
			if (Mario.dy > 0) {
				// x�������ɏd�Ȃ��Ă��邩�H
				if ((Blocks[i].x < Mario.x + Mario.width) && (Mario.x < Blocks[i].x + Blocks[i].width)) {
					// y�������ɂ܂��������H
					if ((Mario.y + Mario.height <= Blocks[i].y) && (Blocks[i].y < Mario.y + Mario.dy + Mario.height)) {
						CollidedY = true;
						MarioNewY = Blocks[i].y - Mario.height;
					}
				}
			}
		}

		// �Ԃ����Ă��Ȃ���΁A��قǂ̌v�Z����K�p
		if (CollidedX == false) {
			MarioNewX = Mario.x + Mario.dx;
		}

		if (CollidedY == false) {
			MarioNewY = Mario.y + Mario.dy;
		}

		Mario.x = MarioNewX;
		Mario.y = MarioNewY;

		// �v���C���[����ʊO�ɍs���Ȃ��悤�ɕ␳
		if (Mario.x < 0) {
			Mario.x = 0;
		}

		if (Mario.x + Mario.width > 640) {
			Mario.x = 640 - Mario.width;
		}

		// --------------------------------------------------

		// �v���C���[�̑��x�̍X�V ---------------------------

		// �����蔻��łԂ����Ă����瑬�x�� 0 �ɂ���
		if (CollidedX == true) {
			Mario.dx = 0;
		}

		if (CheckHitKey(KEY_INPUT_LEFT)) {
			// ���� [��] ��������Ă����獶�ɉ���
			Mario.dx = -2;
		} else if (CheckHitKey(KEY_INPUT_RIGHT)) {
			// ���� [��] ��������Ă�����E�ɉ���
			Mario.dx = 2;
		} else {
			Mario.dx = 0;
		}

		// �����蔻��łԂ����Ă����瑬�x�� 0 �ɂ���
		if (CollidedY == true) {
			Mario.dy = 0;
		}

		// ���� [Space] ��������Ă�����W�����v
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			Mario.dy = -10;
		} else {
			// ����������Ă��Ȃ���΁A�������ɉ���
			Mario.dy += 1;

			// Tips ------------------------------------------------------------
			// 
			// �@�{���Ȃ�d�͉����x�� 9.8 m/(s^2) �ł����A
			// ���ꂾ�Ƒ������邽�߁A������ 1 pixel/(frame^2) ���w�肵�Ă��܂��B
			//
			// -----------------------------------------------------------------
		}

		// �����������Ȃ��悤�ɁA���x�𒲐�����
		if (Mario.dy > 10) {
			Mario.dy = 10;
		}

		// --------------------------------------------------

		// �`�� ---------------------------------------------

		// ��ʂɕ`����Ă�����̂�����
		ClearDrawScreen();

		// �v���C���[��`��
		DrawBox(Mario.x, Mario.y, Mario.x + Mario.width, Mario.y + Mario.height, GetColor(255, 255, 255), TRUE);

		// ���ׂẴu���b�N��`��
		for (int i = 0; i < BlockNum; i++) {
			DrawBox(Blocks[i].x, Blocks[i].y, Blocks[i].x + Blocks[i].width, Blocks[i].y + Blocks[i].height, GetColor(0, 255, 0), TRUE);
		}

		// --------------------------------------------------

		// ��ʂɕ`�������̂��E�B���h�E�ɃR�s�[����B
		ScreenFlip();
	}

	// DX���C�u�����̌�Еt��
	DxLib_End();

	return 0;
}