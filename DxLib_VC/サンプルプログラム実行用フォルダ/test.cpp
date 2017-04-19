// �����ɃT���v���v���O�������y�[�X�g���Ă�������
// ������Ԃł� LoadGraphScreen �̃T���v���v���O���������͂���Ă��܂��B
#include "DxLib.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				 LPSTR lpCmdLine, int nCmdShow )
{
	int BlendGraph, GrHandle1, GrHandle2;
	int i;

	ChangeWindowMode(TRUE);

	// �c�w���C�u��������������
	if (DxLib_Init() == -1) return -1;

	// ����ʂ��g�p
	SetDrawScreen(DX_SCREEN_BACK);

	// �u�����h�摜��ǂݍ���
	BlendGraph = LoadBlendGraph("BlendGraph.bmp");

	// ��̉摜�̓ǂݍ���
	GrHandle1 = LoadGraph("Scene1.jpg");
	GrHandle2 = LoadGraph("Scene2.jpg");

	// 256 �t���[���|���Đ؂�ւ�
	for (i = 0; i < 256; i++)
	{
		// ���b�Z�[�W����
		if (ProcessMessage() != 0) break;

		// Scene2.jpg ��`��
		DrawGraph(0, 0, GrHandle2, FALSE);

		// Scene1.jpg �� BlendGraph.bmp ���������ĕ`��
		// (���E���� 64)
		DrawBlendGraph(0, 0, GrHandle1, FALSE, BlendGraph, i, 64);

		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();
	}

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();


	// �\�t�g�̏I��
	return 0;
}
