#regcmd "_hsp3cmdinit@4","PNGmaker.dll"
#cmd PngSave $000
#cmd PngLoad_ $001
#cmd PngLoadXYN_ $002
#cmd AlphaBlur $003
#cmd ColortoVal $004
#cmd NotColortoVal $005
#cmd MulVal $006
#cmd PixelHeight $007
#cmd PixelCosHeightVecLight $008

#module PngMaker
#deffunc PngLoad str fname,int gsel1,int gsel2
	PngLoadXYN_ fname,x,y,n
	if n!=4{
		dialog ""+fname+"�̃`�����l������4�ł͂���܂���",1
		end
	}
	buffer gsel1,x,y
	buffer gsel2,x,y
	PngLoad_ fname,gsel1,gsel2
	return
#global




/*
	//		PngSave ���߂̏���
	//		(PngSave "�t�@�C����",p1,p2)
	//		"�t�@�C����"�Ŏw�肵���t�@�C����p1��gsel�Ŏw�肳�ꂽ��ʂ�rgb���e���Ap2�̉�ʂ�r�����Ƃ���png��save����B
	//		p1�ȗ����J�����g�E�B���h�E�ɂȂ�
	//		p2�ȗ�������255�ɂȂ�


	//		PngLoad ���߂̏���
	//		(PngLoad "�t�@�C����",p1,p2)
	//		"�t�@�C����"�Ŏw�肵���t�@�C����ǂݍ���p1��rgb,p2��a���������݂܂�
	

	//		PngLoadXYN ���߂̏���
	//		(PngLoadXYN "�t�@�C����",p2,p3,p4)
	//		"�t�@�C����"�Ŏw�肵���t�@�C����x,y,n����p2,p3,p4�ɏ�������


	//		AlphaBlur ���߂̏���
	//		(AlphaBlur p1)
	//		p1�̉�ʂŔ�(��r����)��s�����A���𓧖��Ƃ��āA�s���������̗֊s���ڂ���ƂȂ�悤�ɂڂ₩������
	//		p2�Ńu���[�̋��x �����l���擾�A�u���[���a�A�s�N�Z��
	//		p3�E�F�C�g�̎w�� �����l���擾�A�E�F�C�g�A256�ň�ԕs�����ɋ߂��Ȃ�

	
	//		(ColortoVal p1,p2,p3,p4,p5)
	//		p1�̉�ʂ�color(p2,p3,p4)�Ŏw�肵���F��p5�ɂ���
	

	//		(NotColortoVal p1,p2,p3,p4,p5)
	//		p1�̉�ʂ�color(p2,p3,p4)�Ŏw�肵���F�ȊO��p5�ɂ���


	//		(MulVal p1,p2)
	//		p1�̉�ʂőS���̐F��p2�{����


	//		(PixelHeight p1)
	//		p1�̉�ʂō�������0�Ƃ��āA���ȊO�̃s�N�Z���̍��������߂�B������̋����������قǍ���
	//		�o�͓͂���p1�̉�ʂɃs�N�Z���Ƃ��ďo��


	//		(PixelCosHeightVecLight p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11)
	//		p1�̉�ʂŕs�����̕����̉���F������
	//		�o�͓͂���p1�̉�ʂɃs�N�Z���Ƃ��ďo��
	p1 = code_geti();								// �����l���擾
	p2 = code_geti();								// ���z�v�Z�̔��a
	double dp3 = code_getd();								// cos �~ ����
	double dp4 = code_getd();								// ����x
	double dp5 = code_getd();								// ����y
	double dp6 = code_getd();								// ����z
	double dp7 = code_getd();								// ����col R
	double dp8 = code_getd();								// ����col G
	double dp9 = code_getd();								// ����col B
	double dp10 = code_getdd(39);								// �X�y�L����pow��
	double dp11 = code_getdd(5.5);								// �X�y�L����pow����

*/