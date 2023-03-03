//
//		HSP3.0 plugin sample
//		onion software/onitama 2004/9
//
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION


#include <windows.h>
#include <tuple>
#include <array>
#include <string>
#include <vector>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include "stb_image_write.h"
#include "stb_image.h"
#include "hsp3plugin.h"

using namespace std;


 /*------------------------------------------------------------*/
/*
		controller
*/
/*------------------------------------------------------------*/

void errput(string s)
{
	MessageBox(NULL, s.c_str(), "�G���[", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);
	return;
}



void filecheck(const char* path)
{
	FILE* fp = fopen(path, "r");
	if (fp == NULL) {
		errput("�t�@�C��������܂���");
	}
	fclose(fp);
	return;
}




static void PngSave_(void) 
{
	//		PngSave ���߂̏���
	//		(PngSave "�t�@�C����",p1,p2)
	//		"�t�@�C����"�Ŏw�肵���t�@�C����p1��gsel�Ŏw�肳�ꂽ��ʂ�rgb���e���Ap2�̉�ʂ�r�����Ƃ���png��save����B
	//		p1�ȗ����J�����g�E�B���h�E�ɂȂ�
	//		p2�ȗ�������255�ɂȂ�

	char* p;
	char pathname[_MAX_PATH];
	char out[64];
	int chk;

	p = code_gets();								// ��������擾
	strncpy(pathname, p, _MAX_PATH - 1);			// �擾������������R�s�[

	p1 = code_getdi(-1);								// �����l���擾
	if (p1 <= -1)p1 = active_window;

	p2 = code_getdi(-1);								// �����l���擾
	if (p2 <= -1) //�ȗ���
	{
		BMSCR* bm;
		bm = (BMSCR*)getbmscr(p1);	// p1��BMSCR���擾
		unsigned char* lpDest;		// �`���ʂ̃o�b�t�@
		int width;		// �`����W��
		int height;	// �`����W����
		int wByte;

		lpDest = (unsigned char*)bm->pBit;
		width = bm->sx;
		wByte = bm->sx * 3;
		height = bm->sy;
		wByte = (wByte + 3) / 4 * 4;

		vector<unsigned char> vecData(width * height * 4);

		//�C���[�W�f�[�^�i�[
		for (int j = 0; j < height; j++)
		{
			int j2 = height - 1 - j;
			for (int i = 0; i < width; i++)
			{
				int iP = (j * width + i) * 4;

				vecData[iP + 0] = lpDest[i * 3 + 2 + j2 * wByte];//R
				vecData[iP + 1] = lpDest[i * 3 + 1 + j2 * wByte];//G
				vecData[iP + 2] = lpDest[i * 3 + 0 + j2 * wByte];//B
				vecData[iP + 3] = 255;//A
			}
		}

		stbi_write_png(pathname, width, height, 4, &vecData.front(), 0);
	}
	else 
	{

		BMSCR* bm1;
		bm1 = (BMSCR*)getbmscr(p1);	// p1��BMSCR���擾
		unsigned char* lpDest1;		// �`���ʂ̃o�b�t�@
		int width1;		// �`����W��
		int height1;	// �`����W����
		int wByte1;

		lpDest1 = (unsigned char*)bm1->pBit;
		width1 = bm1->sx;
		wByte1 = bm1->sx * 3;
		height1 = bm1->sy;
		wByte1 = (wByte1 + 3) / 4 * 4;

		vector<unsigned char> vecData(width1 * height1 * 4);

		//�C���[�W�f�[�^�i�[
		for (int j = 0; j < height1; j++)
		{
			int j2 = height1 - 1 - j;
			for (int i = 0; i < width1; i++)
			{
				int iP = (j * width1 + i) * 4;

				vecData[iP + 0] = lpDest1[i * 3 + 2 + j2 * wByte1];//R
				vecData[iP + 1] = lpDest1[i * 3 + 1 + j2 * wByte1];//G
				vecData[iP + 2] = lpDest1[i * 3 + 0 + j2 * wByte1];//B
				vecData[iP + 3] = 255;//A
			}
		}


		BMSCR* bm2;
		bm2 = (BMSCR*)getbmscr(p2);	// p1��BMSCR���擾
		unsigned char* lpDest2;		// �`���ʂ̃o�b�t�@
		int width2;		// �`����W��
		int height2;	// �`����W����
		int wByte2;

		lpDest2 = (unsigned char*)bm2->pBit;
		width2 = bm2->sx;
		wByte2 = bm2->sx * 3;
		height2 = bm2->sy;
		wByte2 = (wByte2 + 3) / 4 * 4;

		//�C���[�W�f�[�^�i�[
		int mnh = min(height2, height1);
		int mnw = min(width2, width1);
		for (int j = 0; j < mnh; j++)
		{
			int j2 = height2 - 1 - j;
			for (int i = 0; i < mnw; i++)
			{
				int iP = (j * width2 + i) * 4;
				vecData[iP + 3] = lpDest2[i * 3 + 2 + j2 * wByte2];//R
			}
		}



		stbi_write_png(pathname, width1, height1, 4, &vecData.front(), 0);
	}

	return;
}




static void PngLoad_(void)
{
	//		PngLoad ���߂̏���
	//		(PngLoad "�t�@�C����",p1,p2)
	//		"�t�@�C����"�Ŏw�肵���t�@�C����ǂݍ���p1��rgb,p2��a���������݂܂�
	char* p;
	char pathname[_MAX_PATH];
	char out[64];
	int chk;

	p = code_gets();								// ��������擾
	strncpy(pathname, p, _MAX_PATH - 1);			// �擾������������R�s�[
	filecheck(pathname);

	p1 = code_geti();								// �����l���擾
	p2 = code_geti();								// �����l���擾
	
	int x, y, n;
	//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
	//    // ... process data if not NULL ...
	//    // ... x = width, y = height, n = # 8-bit components per pixel ...
	//    // ... replace '0' with '1'..'4' to force that many components per pixel
	//    // ... but 'n' will always be the number that it would have been if you said 0
	//    stbi_image_free(data);
	unsigned char* data = stbi_load(pathname, &x, &y, &n, 0);



	BMSCR* bm1;
	bm1 = (BMSCR*)getbmscr(p1);	// p1��BMSCR���擾
	unsigned char* lpDest1;		// �`���ʂ̃o�b�t�@
	int width1;		// �`����W��
	int height1;	// �`����W����
	int wByte1;
	lpDest1 = (unsigned char*)bm1->pBit;
	width1 = bm1->sx;
	wByte1 = bm1->sx * 3;
	height1 = bm1->sy;
	wByte1 = (wByte1 + 3) / 4 * 4;

	BMSCR* bm2;
	bm2 = (BMSCR*)getbmscr(p2);	// p1��BMSCR���擾
	unsigned char* lpDest2;		// �`���ʂ̃o�b�t�@
	int width2;		// �`����W��
	int height2;	// �`����W����
	int wByte2;
	lpDest2 = (unsigned char*)bm2->pBit;
	width2 = bm2->sx;
	wByte2 = bm2->sx * 3;
	height2 = bm2->sy;
	wByte2 = (wByte2 + 3) / 4 * 4;



	//�C���[�W�f�[�^�i�[
	for (int j = 0; j < height1; j++)
	{
		int j2 = height1 - 1 - j;
		for (int i = 0; i < width1; i++)
		{
			int iP = (j * width1 + i) * 4;
			lpDest1[i * 3 + 0 + j2 * wByte1] = data[i * 4 + 2 + j * x * 4];
			lpDest1[i * 3 + 1 + j2 * wByte1] = data[i * 4 + 1 + j * x * 4];
			lpDest1[i * 3 + 2 + j2 * wByte1] = data[i * 4 + 0 + j * x * 4];
		}
	}

	//�C���[�W�f�[�^�i�[
	for (int j = 0; j < height2; j++)
	{
		int j2 = height2 - 1 - j;
		for (int i = 0; i < width2; i++)
		{
			int iP = (j * width2 + i) * 4;
			lpDest2[i * 3 + 0 + j2 * wByte2] = data[i * 4 + 3 + j * x * 4];
			lpDest2[i * 3 + 1 + j2 * wByte2] = data[i * 4 + 3 + j * x * 4];
			lpDest2[i * 3 + 2 + j2 * wByte2] = data[i * 4 + 3 + j * x * 4];
		}
	}


	stbi_image_free(data);
	//bms_send(bm1, 0, 0, bm1->sx, bm1->sy);		// �S��ʂ��X�V
	//bms_send(bm2, 0, 0, bm2->sx, bm2->sy);		// �S��ʂ��X�V
	return;
}


static void PngLoadXYN_( void )
{
	//		PngLoadXYN ���߂̏���
	//		(PngLoadXYN "�t�@�C����",p2,p3,p4)
	//		"�t�@�C����"�Ŏw�肵���t�@�C����x,y,n����p2,p3,p4�ɏ�������
	char* p;
	char pathname[_MAX_PATH];
	char out[64];
	int chk;

	p = code_gets();								// ��������擾
	strncpy(pathname, p, _MAX_PATH - 1);			// �擾������������R�s�[
	filecheck(pathname);


	int x, y, n;
	//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
	//    // ... process data if not NULL ...
	//    // ... x = width, y = height, n = # 8-bit components per pixel ...
	//    // ... replace '0' with '1'..'4' to force that many components per pixel
	//    // ... but 'n' will always be the number that it would have been if you said 0
	//    stbi_image_free(data);
	unsigned char* data = stbi_load(pathname, &x, &y, &n, 0);

	//		(newcmd2 p1,p2)
	//		p2,p3,p4�Ŏw�肵���ϐ��ɑ��
	PVal* pval;
	APTR aptr;
	aptr = code_getva(&pval);						// �ϐ��̎擾(PVal��APTR�|�C���^)
	code_setva(pval, aptr, HSPVAR_FLAG_INT, &x);		// �ϐ��ɒl����(�����l)

	aptr = code_getva(&pval);						// �ϐ��̎擾(PVal��APTR�|�C���^)
	code_setva(pval, aptr, HSPVAR_FLAG_INT, &y);		// �ϐ��ɒl����(�����l)

	aptr = code_getva(&pval);						// �ϐ��̎擾(PVal��APTR�|�C���^)
	code_setva(pval, aptr, HSPVAR_FLAG_INT, &n);		// �ϐ��ɒl����(�����l)

	stbi_image_free(data);
	return;
}


static void AlphaBlur( void )
{
	//		AlphaBlur ���߂̏���
	//		(AlphaBlur p1)
	//		p1�̉�ʂŔ�(��r����)��s�����A���𓧖��Ƃ��āA�s���������̗֊s���ڂ���ƂȂ�悤�ɂڂ₩������
	//		p2�Ńu���[�̋��x
	//		p3�E�F�C�g�̎w��

	p1 = code_geti();								// �����l���擾
	p2 = code_getdi(2);								// �����l���擾�A�u���[���a�A�s�N�Z��
	p3 = code_getdi(256);								// �����l���擾�A�E�F�C�g�A256�ň�ԕs�����ɋ߂��Ȃ�
	if (p2 > 8192)p2 = 8192;
	if (p2 < 0)p2 = 0;
	if (p3 > 256)p3 = 256;
	if (p3 < 0)p3 = 0;

	BMSCR* bm1;
	bm1 = (BMSCR*)getbmscr(p1);	// p1��BMSCR���擾
	unsigned char* lpDest1;		// �`���ʂ̃o�b�t�@
	int width1;		// �`����W��
	int height1;	// �`����W����
	int wByte1;
	lpDest1 = (unsigned char*)bm1->pBit;
	width1 = bm1->sx;
	wByte1 = bm1->sx * 3;
	height1 = bm1->sy;
	wByte1 = (wByte1 + 3) / 4 * 4;

	//�͈͊O�͍��������Ƃ��ď���
	int p22 = p2 * p2;
	vector<int> leng(width1 * height1, 99999999);//��ԋ߂��s�����܂łǂ̂��炢���A�����̓�悪�͂���
	for (int j = 0; j < height1; j++)
	{
		for (int i = 0; i < width1; i++) 
		{
			if (lpDest1[j * wByte1 + i * 3 + 2] != 0)continue;

			for (int j1 = -p2; j1 <= p2; j1++) 
			{
				for (int i1 = -p2; i1 <= p2; i1++) 
				{
					int l2 = j1 * j1 + i1 * i1;
					if (l2 > p22)
						continue;
					int j2 = j + j1;
					int i2 = i + i1;
					if ((j2 < 0) | (j2 >= height1))
						continue;
					if ((i2 < 0) | (i2 >= width1))
						continue;
					auto r = lpDest1[j2 * wByte1 + i2 * 3 + 2];
					if (r != 0) 
					{
						leng[j * width1 + i] = min(l2, leng[j * width1 + i]);
					}
				}
			}

		}
	}


	for (int j = 0; j < height1; j++)
	{
		for (int i = 0; i < width1; i++)
		{
			int il = leng[j * width1 + i];
			if (il == 99999999)continue;
			double val;
			double len = sqrt((double)il) / p2;
			len = 4.0 - len * 8.0;
			val = 1.0 / (1.0 + exp(-len));
			val = val * (double)p3;
			val = min(max(val, 0), 255);

			lpDest1[j * wByte1 + i * 3 + 0] =
				lpDest1[j * wByte1 + i * 3 + 1] =
				lpDest1[j * wByte1 + i * 3 + 2] = (unsigned char)val;
	
		}
	}

}




static void ColortoVal(void)
{
	//		(ColortoVal p1,p2,p3,p4,p5)
	//		p1�̉�ʂ�color(p2,p3,p4)�Ŏw�肵���F��p5�ɂ���
	p1 = code_geti();								// �����l���擾
	p2 = code_geti();								// r
	p3 = code_geti();								// g
	p4 = code_geti();								// b
	p5 = code_geti();								// val
	if (p5 > 255)p5 = 255;
	if (p5 < 0)p5 = 0;
	BMSCR* bm1;
	bm1 = (BMSCR*)getbmscr(p1);	// p1��BMSCR���擾
	unsigned char* lpDest1;		// �`���ʂ̃o�b�t�@
	int width1;		// �`����W��
	int height1;	// �`����W����
	int wByte1;
	lpDest1 = (unsigned char*)bm1->pBit;
	width1 = bm1->sx;
	wByte1 = bm1->sx * 3;
	height1 = bm1->sy;
	wByte1 = (wByte1 + 3) / 4 * 4;


	for (int j = 0; j < height1; j++)
	{
		for (int i = 0; i < width1; i++)
		{
			auto b = lpDest1[j * wByte1 + i * 3 + 0];
			auto g = lpDest1[j * wByte1 + i * 3 + 1];
			auto r = lpDest1[j * wByte1 + i * 3 + 2];
			if ((r == p2) & (g == p3) & (b == p4)) 
			{
				lpDest1[j * wByte1 + i * 3 + 0] =
					lpDest1[j * wByte1 + i * 3 + 1] =
					lpDest1[j * wByte1 + i * 3 + 2] = (unsigned char)p5;
					
			}
		}
	}
	return;
}






static void NotColortoVal(void)
{
	//		(NotColortoVal p1,p2,p3,p4,p5)
	//		p1�̉�ʂ�color(p2,p3,p4)�Ŏw�肵���F�ȊO��p5�ɂ���
	p1 = code_geti();								// �����l���擾
	p2 = code_geti();								// r
	p3 = code_geti();								// g
	p4 = code_geti();								// b
	p5 = code_geti();								// val
	if (p5 > 255)p5 = 255;
	if (p5 < 0)p5 = 0;
	BMSCR* bm1;
	bm1 = (BMSCR*)getbmscr(p1);	// p1��BMSCR���擾
	unsigned char* lpDest1;		// �`���ʂ̃o�b�t�@
	int width1;		// �`����W��
	int height1;	// �`����W����
	int wByte1;
	lpDest1 = (unsigned char*)bm1->pBit;
	width1 = bm1->sx;
	wByte1 = bm1->sx * 3;
	height1 = bm1->sy;
	wByte1 = (wByte1 + 3) / 4 * 4;


	for (int j = 0; j < height1; j++)
	{
		for (int i = 0; i < width1; i++)
		{
			auto b = lpDest1[j * wByte1 + i * 3 + 0];
			auto g = lpDest1[j * wByte1 + i * 3 + 1];
			auto r = lpDest1[j * wByte1 + i * 3 + 2];
			if ((r == p2) & (g == p3) & (b == p4))continue;
			lpDest1[j * wByte1 + i * 3 + 0] =
				lpDest1[j * wByte1 + i * 3 + 1] =
				lpDest1[j * wByte1 + i * 3 + 2] = (unsigned char)p5;
		}
	}
	return;
}




static void MulVal(void)
{
	//		(MulVal p1,p2)
	//		p1�̉�ʂőS���̐F��p2�{����
	p1 = code_geti();								// �����l���擾
	double dp2 = code_getd();
	BMSCR* bm1;
	bm1 = (BMSCR*)getbmscr(p1);	// p1��BMSCR���擾
	unsigned char* lpDest1;		// �`���ʂ̃o�b�t�@
	int width1;		// �`����W��
	int height1;	// �`����W����
	int wByte1;
	lpDest1 = (unsigned char*)bm1->pBit;
	width1 = bm1->sx;
	wByte1 = bm1->sx * 3;
	height1 = bm1->sy;
	wByte1 = (wByte1 + 3) / 4 * 4;


	for (int j = 0; j < height1; j++)
	{
		for (int i = 0; i < width1; i++)
		{
			auto b = lpDest1[j * wByte1 + i * 3 + 0];
			auto g = lpDest1[j * wByte1 + i * 3 + 1];
			auto r = lpDest1[j * wByte1 + i * 3 + 2];
			auto db = (double)b * dp2;
			auto dg = (double)g * dp2;
			auto dr = (double)r * dp2;
			db = min(max(db, 0), 255);
			dg = min(max(dg, 0), 255);
			dr = min(max(dr, 0), 255);

			lpDest1[j * wByte1 + i * 3 + 0] = (unsigned char)db;
			lpDest1[j * wByte1 + i * 3 + 1] = (unsigned char)dg;
			lpDest1[j * wByte1 + i * 3 + 2] = (unsigned char)dr;
		}
	}
	return;
}





static void PixelHeight(void)
{
	//		(PixelHeight p1)
	//		p1�̉�ʂō�������0�Ƃ��āA���ȊO�̃s�N�Z���̍��������߂�B������̋����������قǍ���
	//		�o�͓͂���p1�̉�ʂɃs�N�Z���Ƃ��ďo��
	p1 = code_geti();								// �����l���擾
	BMSCR* bm1;
	bm1 = (BMSCR*)getbmscr(p1);	// p1��BMSCR���擾
	unsigned char* lpDest1;		// �`���ʂ̃o�b�t�@
	int width1;		// �`����W��
	int height1;	// �`����W����
	int wByte1;
	lpDest1 = (unsigned char*)bm1->pBit;
	width1 = bm1->sx;
	wByte1 = bm1->sx * 3;
	height1 = bm1->sy;
	wByte1 = (wByte1 + 3) / 4 * 4;

	vector<vector<int>> data(height1, vector<int>(width1, -1));
	queue<tuple<int, int>> que;
	
	for (int j = 0; j < height1; j++)
	{
		for (int i = 0; i < width1; i++)
		{
			auto b = lpDest1[j * wByte1 + i * 3 + 0];
			auto g = lpDest1[j * wByte1 + i * 3 + 1];
			auto r = lpDest1[j * wByte1 + i * 3 + 2];
			if ((r == 0) & (g == 0) & (b == 0)) 
			{
				data[j][i] = 0;
				que.emplace(j, i);
			}
		}
	}

	int dx[] = { 0,0,-1,1 };
	int dy[] = { -1,1,0,0 };

	while (!que.empty()) 
	{
		int y, x;
		tie (y, x) = que.front();
		que.pop();
		auto h = data[y][x];

		for (int ii = 0; ii < 4; ii++) 
		{
			int ny = y + dy[ii];
			int nx = x + dx[ii];
			if (ny >= height1)continue;
			if (ny < 0)continue;
			if (nx >= width1)continue;
			if (nx < 0)continue;
			auto nh = data[ny][nx];
			if (nh == -1)
			{
				data[ny][nx] = h + 1;
				que.emplace(ny, nx);
			}
		}
	}


	for (int j = 0; j < height1; j++)
	{
		for (int i = 0; i < width1; i++)
		{
			int h = data[j][i];
			unsigned char uc = 0;
			if (h > 0) 
			{
				if (h > 255)h = 255;
				uc = (unsigned char)h;
			}

			lpDest1[j * wByte1 + i * 3 + 0] =
				lpDest1[j * wByte1 + i * 3 + 1] =
				lpDest1[j * wByte1 + i * 3 + 2] = uc;
		}
	}

	return;
}



static void PixelCosHeightVecLight(void)
{
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

	BMSCR* bm1;
	bm1 = (BMSCR*)getbmscr(p1);	// p1��BMSCR���擾
	unsigned char* lpDest1;		// �`���ʂ̃o�b�t�@
	int width1;		// �`����W��
	int height1;	// �`����W����
	int wByte1;
	lpDest1 = (unsigned char*)bm1->pBit;
	width1 = bm1->sx;
	wByte1 = bm1->sx * 3;
	height1 = bm1->sy;
	wByte1 = (wByte1 + 3) / 4 * 4;

	if (width1 <= 2)return;
	if (height1 <= 2)return;

	//�x�N�g���v�Z
	//�s�����Ȃ�true�A�����Ȃ�false
	vector<vector<bool>> colb(height1, vector<bool>(width1));

	for (int j = 0; j < height1; j++)
	{
		for (int i = 0; i < width1; i++)
		{
			auto b = lpDest1[j * wByte1 + i * 3 + 0];
			auto g = lpDest1[j * wByte1 + i * 3 + 1];
			auto r = lpDest1[j * wByte1 + i * 3 + 2];
			colb[j][i] = !((r == 0) & (g == 0) & (b == 0));
		}
	}

	vector<vector<array<double, 3>>> dh2(height1 - 2, vector<array<double, 3>>(width1 - 2));
	int p22 = p2 * p2;
	for (int j = 1; j < height1 - 1; j++)
	{
		for (int i = 1; i < width1 - 1; i++)
		{
			if (colb[j][i] == false)continue;

			array<double, 3> xy;
			double min0len = 1.0 * p22;
			double totalvecx = 0.0;
			double totalvecy = 0.0;
			for (int j1 = -p2; j1 <= p2; j1++) 
			{
				int j2 = j + j1;
				if (j2 < 0)continue;
				if (j2 >= height1)continue;
				for (int i1 = -p2; i1 <= p2; i1++)
				{
					if (j1 * j1 + i1 * i1 > p22)continue;
					int i2 = i + i1;
					if (i2 < 0)continue;
					if (i2 >= width1)continue;

					if (colb[j2][i2])
					{
						totalvecx += 1.0 * i1;
						totalvecy += 1.0 * j1;
					}
					else 
					{
						min0len = min(min0len, (double)(j1 * j1 + i1 * i1));
					}
				}
			}

			totalvecx = -totalvecx;
			//����Ŗ@���̕����Ɠ����s�N�Z���܂ł̋������킩����
			double dlen = sqrt(totalvecx * totalvecx + totalvecy * totalvecy);
			if (dlen != 0.0) 
			{
				//�����Ŗ@�����v�Z�B
				//�܂��͕�������
				totalvecx /= dlen;
				totalvecy /= dlen;

				//min0len��0�`p2�̓��
				//�~�֐���dy/dx
				auto dy = sqrt((double)p22 - min0len) / sqrt(min0len);
				//�J�[�u�������ق����@��������
				totalvecx *= dy;
				totalvecy *= dy;

				double vecz = 1.0 / dp3;//�����𔽉f
				auto rdlen = 1.0 / sqrt(totalvecx * totalvecx + totalvecy * totalvecy + vecz * vecz);
				totalvecx *= rdlen;
				totalvecy *= rdlen;
				vecz *= rdlen;

				xy[0] = totalvecx;
				xy[1] = totalvecy;
				xy[2] = vecz;
			}
			else 
			{
				xy[0] = 0;//x
				xy[1] = 0;//y
				xy[2] = 1;//z
			}
			dh2[j - 1][i - 1] = xy;

		}
	}




	//�x�N�g���v�Z���Č�
	for (int j = 1; j < height1 - 1; j++)
	{
		for (int i = 1; i < width1 - 1; i++)
		{
			if (colb[j][i] == false) continue;

			auto b = lpDest1[j * wByte1 + i * 3 + 0];
			auto g = lpDest1[j * wByte1 + i * 3 + 1];
			auto r = lpDest1[j * wByte1 + i * 3 + 2];

			auto vecx = dh2[j][i][0];
			auto vecy = dh2[j][i][1];
			auto vecz = dh2[j][i][2];

			//�s�N�Z����������܂ł��v�Z
			double pz = 0.0;
			double py = (double)(height1 - 1 - j);
			double px = (double)i;
			double rvecx = dp4 - px;
			double rvecy = dp5 - py;
			double rvecz = dp6 - pz;
			double rdlen = 1.0 / sqrt(rvecx * rvecx + rvecy * rvecy + rvecz * rvecz);
			rvecx *= rdlen;
			rvecy *= rdlen;
			rvecz *= rdlen;
			
			//���όv�Z
			double dot = rvecx * vecx + rvecy * vecy + rvecz * vecz;
			if (dot < 0)continue;
			//�X�y�L�����͓K����
			double sp = pow(dot, dp10);

			double colr = 0;
			double colg = 0;
			double colb = 0;

			colr += sp * dp11 * dp7;
			colg += sp * dp11 * dp8;
			colb += sp * dp11 * dp9;

			colr += dot * dp7 + (double)r;
			colg += dot * dp8 + (double)g;
			colb += dot * dp9 + (double)b;

			colr = min(max(colr, 0), 255);
			colg = min(max(colg, 0), 255);
			colb = min(max(colb, 0), 255);

			lpDest1[j * wByte1 + i * 3 + 0] = (unsigned char)colb;
			lpDest1[j * wByte1 + i * 3 + 1] = (unsigned char)colg;
			lpDest1[j * wByte1 + i * 3 + 2] = (unsigned char)colr;
		}
	}


}



/*------------------------------------------------------------*/

static int cmdfunc( int cmd )
{
	//		���s���� (���ߎ��s���ɌĂ΂�܂�)
	//
	code_next();							// ���̃R�[�h���擾(�ŏ��ɕK���K�v�ł�)

	switch( cmd ) {							// �T�u�R�}���h���Ƃ̕���

	case 0x00:
		PngSave_();
		break;

	case 0x01:
		PngLoad_();
		break;

	case 0x02:
		PngLoadXYN_();
		break;

	case 0x03:
		AlphaBlur();
		break;

	case 0x04:
		ColortoVal();
		break;

	case 0x05:
		NotColortoVal();
		break;

	case 0x06:
		MulVal();
		break;

	case 0x07:
		PixelHeight();
		break;

	case 0x08:
		PixelCosHeightVecLight();
		break;

	default:
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
	}
	return RUNMODE_RUN;
}


/*------------------------------------------------------------*/

static int ref_ival;						// �Ԓl�̂��߂̕ϐ�

static void *reffunc( int *type_res, int cmd )
{
	//		�֐��E�V�X�e���ϐ��̎��s���� (�l�̎Q�Ǝ��ɌĂ΂�܂�)
	//			'('�Ŏn�܂邩�𒲂ׂ�
	if ( *type != TYPE_MARK ) puterror( HSPERR_INVALID_FUNCPARAM );
	if ( *val != '(' ) puterror( HSPERR_INVALID_FUNCPARAM );
	code_next();

	switch( cmd ) {							// �T�u�R�}���h���Ƃ̕���
		/*
	case 0x00:								// newcmd

		p1 = code_geti();				// �����l���擾(�f�t�H���g�Ȃ�)
		ref_ival = p1 * 2;				// �Ԓl��ival�ɐݒ�
		break;
		*/
	default:
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
	}

	//			'('�ŏI��邩�𒲂ׂ�
	if ( *type != TYPE_MARK ) puterror( HSPERR_INVALID_FUNCPARAM );
	if ( *val != ')' ) puterror( HSPERR_INVALID_FUNCPARAM );
	code_next();
	*type_res = HSPVAR_FLAG_INT;			// �Ԓl�̃^�C�v�𐮐��Ɏw�肷��
	return (void *)&ref_ival;
}


/*------------------------------------------------------------*/

static int termfunc( int option )
{
	//		�I������ (�A�v���P�[�V�����I�����ɌĂ΂�܂�)
	return 0;
}

/*------------------------------------------------------------*/

static int eventfunc( int event, int prm1, int prm2, void *prm3 )
{
	//		�C�x���g���� (HSP�C�x���g�������ɌĂ΂�܂�)
	//
	switch( event ) {
	case HSPEVENT_GETKEY:
		{
		int *ival;
		ival = (int *)prm3;
		*ival = 123;
		return 1;
		}
	}
	return 0;
}

/*------------------------------------------------------------*/
/*
		interface
*/
/*------------------------------------------------------------*/

int WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
	//		DLL�G���g���[ (��������K�v�͂���܂���)
	return TRUE;
}


EXPORT void WINAPI hsp3cmdinit( HSP3TYPEINFO *info )
{
	//		�v���O�C�������� (���s�E�I��������o�^���܂�)
	//
	hsp3sdk_init( info );			// SDK�̏�����(�ŏ��ɍs�Ȃ��ĉ�����)
	info->cmdfunc = cmdfunc;		// ���s�֐�(cmdfunc)�̓o�^
	info->reffunc = reffunc;		// �Q�Ɗ֐�(reffunc)�̓o�^
	info->termfunc = termfunc;		// �I���֐�(termfunc)�̓o�^
}


