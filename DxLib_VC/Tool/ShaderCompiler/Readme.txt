
  �ȈՃV�F�[�_�[�R���p�C���\�t�g


�@�R�}���h�v�����v�g�Ŏg�p����V�F�[�_�[�v���O�������R���p�C������\�t�g�ł��B
�@�R���p�C���ɂ� DirectX �̋@�\���g�p���܂��̂ŁA�Ȃ�ׂ��V���� DirectX End-User Runtime ���C���X�g�[�����Ă����Ă��������B
�@���ƁA�V�F�[�_�[���g���ɂ͍Œ�ł��O���t�B�b�N�X�f�o�C�X���V�F�[�_�[���f���Q�D�O�ɑΉ����Ă���K�v������܂��B


  ShaderCompiler.exe <option> <shader file>

	�I�v�V����

	/T<profile>	�쐬����V�F�[�_�[�̃^�[�Q�b�g�̎w��
			( vs_1_1 vs_2_0 vs_3_0 ps_2_0 ps_3_0 �� )

	/Fo<file path>	�o�͐�̃t�@�C���p�X�̎w��

	/E<name>	�G���g���[�|�C���g�̎w��
			( �쐬����V�F�[�_�[�̃v���O�����J�n�֐��̎w��
			  �f�t�H���g�ł� main )

	/D<id>=<text>	#define <id> <text> �̃}�N����`



�@�g�p��� Sample �t�H���_�̒����������������B
�@Sample �t�H���_�̒��̃t�@�C���̉����


�@PixelShaderTest.exe
�@�@�T���v���v���O�������Ǘ��l���R���p�C����������

�@PixelShaderTest.cpp   
�@�@�T���v���V�F�[�_�[�v���O���������ۂɎg���c�w���C�u�������g�p�����v���O����

�@SamplePS.fx
�@�@�T���v���s�N�Z���V�F�[�_�[�v���O����
�@�@�e�N�X�`���̐F������ r �� b �����ւ��ĕ`�悷�邽�߂̃s�N�Z���V�F�[�_�[�v���O�����ł�

�@SamplePS.pso
�@�@SamplePS.fx �� ShaderCompiler.exe �Ńo�C�i���R�[�h�ɃR���p�C�������t�@�C��
�@�@�c�w���C�u�����ł͂��̃t�@�C����ǂݍ��݂܂�

�@SamplePSCompile.bat
�@�@SamplePS.fx ���R���p�C������ SamplePS.pso ���쐬���邽�߂̃o�b�`�t�@�C��
�@�@�e�L�X�g�G�f�B�^�ŊJ���ƒ��͂���ȕ��ɂȂ��Ă��܂�

ShaderCompiler.exe /Tps_2_0 SamplePS.fx
pause

�@�@��s�ڂ� SamplePS.fx ���s�N�Z���V�F�[�_�[���f���Q�D�O�̃V�F�[�_�[�v���O�����Ƃ���
�@�@�R���p�C������R�}���h�ł�
�@�@�Q�s�ڂ͉����L�[���������܂Ńo�b�`�������Ƃ߂�R�}���h�ł�

�@Test1.bmp
�@�@�T���v���v���O����( PixelShaderText.cpp )�Ŏg���摜�t�@�C��

�@ShaderCompiler.exe
�@�@���̃t�H���_�ɂ���V�F�[�_�[�R���p�C���\�t�g�Ɠ������̂ł�