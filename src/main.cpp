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
	MessageBox(NULL, s.c_str(), "エラー", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);
	return;
}



void filecheck(const char* path)
{
	FILE* fp = fopen(path, "r");
	if (fp == NULL) {
		errput("ファイルがありません");
	}
	fclose(fp);
	return;
}




static void PngSave_(void) 
{
	//		PngSave 命令の処理
	//		(PngSave "ファイル名",p1,p2)
	//		"ファイル名"で指定したファイルにp1のgselで指定された画面のrgb内容を、p2の画面のrをαとしてpngでsaveする。
	//		p1省略時カレントウィンドウになる
	//		p2省略時αは255になる

	char* p;
	char pathname[_MAX_PATH];
	char out[64];
	int chk;

	p = code_gets();								// 文字列を取得
	strncpy(pathname, p, _MAX_PATH - 1);			// 取得した文字列をコピー

	p1 = code_getdi(-1);								// 整数値を取得
	if (p1 <= -1)p1 = active_window;

	p2 = code_getdi(-1);								// 整数値を取得
	if (p2 <= -1) //省略時
	{
		BMSCR* bm;
		bm = (BMSCR*)getbmscr(p1);	// p1のBMSCRを取得
		unsigned char* lpDest;		// 描画画面のバッファ
		int width;		// 描画座標幅
		int height;	// 描画座標高さ
		int wByte;

		lpDest = (unsigned char*)bm->pBit;
		width = bm->sx;
		wByte = bm->sx * 3;
		height = bm->sy;
		wByte = (wByte + 3) / 4 * 4;

		vector<unsigned char> vecData(width * height * 4);

		//イメージデータ格納
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
		bm1 = (BMSCR*)getbmscr(p1);	// p1のBMSCRを取得
		unsigned char* lpDest1;		// 描画画面のバッファ
		int width1;		// 描画座標幅
		int height1;	// 描画座標高さ
		int wByte1;

		lpDest1 = (unsigned char*)bm1->pBit;
		width1 = bm1->sx;
		wByte1 = bm1->sx * 3;
		height1 = bm1->sy;
		wByte1 = (wByte1 + 3) / 4 * 4;

		vector<unsigned char> vecData(width1 * height1 * 4);

		//イメージデータ格納
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
		bm2 = (BMSCR*)getbmscr(p2);	// p1のBMSCRを取得
		unsigned char* lpDest2;		// 描画画面のバッファ
		int width2;		// 描画座標幅
		int height2;	// 描画座標高さ
		int wByte2;

		lpDest2 = (unsigned char*)bm2->pBit;
		width2 = bm2->sx;
		wByte2 = bm2->sx * 3;
		height2 = bm2->sy;
		wByte2 = (wByte2 + 3) / 4 * 4;

		//イメージデータ格納
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
	//		PngLoad 命令の処理
	//		(PngLoad "ファイル名",p1,p2)
	//		"ファイル名"で指定したファイルを読み込みp1にrgb,p2にaを書き込みます
	char* p;
	char pathname[_MAX_PATH];
	char out[64];
	int chk;

	p = code_gets();								// 文字列を取得
	strncpy(pathname, p, _MAX_PATH - 1);			// 取得した文字列をコピー
	filecheck(pathname);

	p1 = code_geti();								// 整数値を取得
	p2 = code_geti();								// 整数値を取得
	
	int x, y, n;
	//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
	//    // ... process data if not NULL ...
	//    // ... x = width, y = height, n = # 8-bit components per pixel ...
	//    // ... replace '0' with '1'..'4' to force that many components per pixel
	//    // ... but 'n' will always be the number that it would have been if you said 0
	//    stbi_image_free(data);
	unsigned char* data = stbi_load(pathname, &x, &y, &n, 0);



	BMSCR* bm1;
	bm1 = (BMSCR*)getbmscr(p1);	// p1のBMSCRを取得
	unsigned char* lpDest1;		// 描画画面のバッファ
	int width1;		// 描画座標幅
	int height1;	// 描画座標高さ
	int wByte1;
	lpDest1 = (unsigned char*)bm1->pBit;
	width1 = bm1->sx;
	wByte1 = bm1->sx * 3;
	height1 = bm1->sy;
	wByte1 = (wByte1 + 3) / 4 * 4;

	BMSCR* bm2;
	bm2 = (BMSCR*)getbmscr(p2);	// p1のBMSCRを取得
	unsigned char* lpDest2;		// 描画画面のバッファ
	int width2;		// 描画座標幅
	int height2;	// 描画座標高さ
	int wByte2;
	lpDest2 = (unsigned char*)bm2->pBit;
	width2 = bm2->sx;
	wByte2 = bm2->sx * 3;
	height2 = bm2->sy;
	wByte2 = (wByte2 + 3) / 4 * 4;



	//イメージデータ格納
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

	//イメージデータ格納
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
	//bms_send(bm1, 0, 0, bm1->sx, bm1->sy);		// 全画面を更新
	//bms_send(bm2, 0, 0, bm2->sx, bm2->sy);		// 全画面を更新
	return;
}


static void PngLoadXYN_( void )
{
	//		PngLoadXYN 命令の処理
	//		(PngLoadXYN "ファイル名",p2,p3,p4)
	//		"ファイル名"で指定したファイルのx,y,n情報をp2,p3,p4に書き込む
	char* p;
	char pathname[_MAX_PATH];
	char out[64];
	int chk;

	p = code_gets();								// 文字列を取得
	strncpy(pathname, p, _MAX_PATH - 1);			// 取得した文字列をコピー
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
	//		p2,p3,p4で指定した変数に代入
	PVal* pval;
	APTR aptr;
	aptr = code_getva(&pval);						// 変数の取得(PValとAPTRポインタ)
	code_setva(pval, aptr, HSPVAR_FLAG_INT, &x);		// 変数に値を代入(整数値)

	aptr = code_getva(&pval);						// 変数の取得(PValとAPTRポインタ)
	code_setva(pval, aptr, HSPVAR_FLAG_INT, &y);		// 変数に値を代入(整数値)

	aptr = code_getva(&pval);						// 変数の取得(PValとAPTRポインタ)
	code_setva(pval, aptr, HSPVAR_FLAG_INT, &n);		// 変数に値を代入(整数値)

	stbi_image_free(data);
	return;
}


static void AlphaBlur( void )
{
	//		AlphaBlur 命令の処理
	//		(AlphaBlur p1)
	//		p1の画面で白(のr成分)を不透明、黒を透明として、不透明部分の輪郭がぼやっとなるようにぼやかす処理
	//		p2でブラーの強度
	//		p3ウェイトの指定

	p1 = code_geti();								// 整数値を取得
	p2 = code_getdi(2);								// 整数値を取得、ブラー半径、ピクセル
	p3 = code_getdi(256);								// 整数値を取得、ウェイト、256で一番不透明に近くなる
	if (p2 > 8192)p2 = 8192;
	if (p2 < 0)p2 = 0;
	if (p3 > 256)p3 = 256;
	if (p3 < 0)p3 = 0;

	BMSCR* bm1;
	bm1 = (BMSCR*)getbmscr(p1);	// p1のBMSCRを取得
	unsigned char* lpDest1;		// 描画画面のバッファ
	int width1;		// 描画座標幅
	int height1;	// 描画座標高さ
	int wByte1;
	lpDest1 = (unsigned char*)bm1->pBit;
	width1 = bm1->sx;
	wByte1 = bm1->sx * 3;
	height1 = bm1->sy;
	wByte1 = (wByte1 + 3) / 4 * 4;

	//範囲外は黒＝透明として処理
	int p22 = p2 * p2;
	vector<int> leng(width1 * height1, 99999999);//一番近い不透明までどのくらいか、長さの二乗がはいる
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
	//		p1の画面でcolor(p2,p3,p4)で指定した色をp5にする
	p1 = code_geti();								// 整数値を取得
	p2 = code_geti();								// r
	p3 = code_geti();								// g
	p4 = code_geti();								// b
	p5 = code_geti();								// val
	if (p5 > 255)p5 = 255;
	if (p5 < 0)p5 = 0;
	BMSCR* bm1;
	bm1 = (BMSCR*)getbmscr(p1);	// p1のBMSCRを取得
	unsigned char* lpDest1;		// 描画画面のバッファ
	int width1;		// 描画座標幅
	int height1;	// 描画座標高さ
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
	//		p1の画面でcolor(p2,p3,p4)で指定した色以外をp5にする
	p1 = code_geti();								// 整数値を取得
	p2 = code_geti();								// r
	p3 = code_geti();								// g
	p4 = code_geti();								// b
	p5 = code_geti();								// val
	if (p5 > 255)p5 = 255;
	if (p5 < 0)p5 = 0;
	BMSCR* bm1;
	bm1 = (BMSCR*)getbmscr(p1);	// p1のBMSCRを取得
	unsigned char* lpDest1;		// 描画画面のバッファ
	int width1;		// 描画座標幅
	int height1;	// 描画座標高さ
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
	//		p1の画面で全部の色をp2倍する
	p1 = code_geti();								// 整数値を取得
	double dp2 = code_getd();
	BMSCR* bm1;
	bm1 = (BMSCR*)getbmscr(p1);	// p1のBMSCRを取得
	unsigned char* lpDest1;		// 描画画面のバッファ
	int width1;		// 描画座標幅
	int height1;	// 描画座標高さ
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
	//		p1の画面で黒を高さ0として、黒以外のピクセルの高さを求める。黒からの距離が遠いほど高い
	//		出力は同じp1の画面にピクセルとして出力
	p1 = code_geti();								// 整数値を取得
	BMSCR* bm1;
	bm1 = (BMSCR*)getbmscr(p1);	// p1のBMSCRを取得
	unsigned char* lpDest1;		// 描画画面のバッファ
	int width1;		// 描画座標幅
	int height1;	// 描画座標高さ
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
	//		p1の画面で不透明の部分の縁を認識して
	//		出力は同じp1の画面にピクセルとして出力
	p1 = code_geti();								// 整数値を取得
	p2 = code_geti();								// 勾配計算の半径
	double dp3 = code_getd();								// cos 円 高さ
	double dp4 = code_getd();								// 光源x
	double dp5 = code_getd();								// 光源y
	double dp6 = code_getd();								// 光源z
	double dp7 = code_getd();								// 光源col R
	double dp8 = code_getd();								// 光源col G
	double dp9 = code_getd();								// 光源col B
	double dp10 = code_getdd(39);								// スペキュラpow数
	double dp11 = code_getdd(5.5);								// スペキュラpow強さ

	BMSCR* bm1;
	bm1 = (BMSCR*)getbmscr(p1);	// p1のBMSCRを取得
	unsigned char* lpDest1;		// 描画画面のバッファ
	int width1;		// 描画座標幅
	int height1;	// 描画座標高さ
	int wByte1;
	lpDest1 = (unsigned char*)bm1->pBit;
	width1 = bm1->sx;
	wByte1 = bm1->sx * 3;
	height1 = bm1->sy;
	wByte1 = (wByte1 + 3) / 4 * 4;

	if (width1 <= 2)return;
	if (height1 <= 2)return;

	//ベクトル計算
	//不透明ならtrue、透明ならfalse
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
			//これで法線の方向と透明ピクセルまでの距離がわかった
			double dlen = sqrt(totalvecx * totalvecx + totalvecy * totalvecy);
			if (dlen != 0.0) 
			{
				//ここで法線を計算。
				//まずは方向だけ
				totalvecx /= dlen;
				totalvecy /= dlen;

				//min0lenは0～p2の二乗
				//円関数のdy/dx
				auto dy = sqrt((double)p22 - min0len) / sqrt(min0len);
				//カーブがきついほうが法線がきつい
				totalvecx *= dy;
				totalvecy *= dy;

				double vecz = 1.0 / dp3;//高さを反映
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




	//ベクトル計算して光
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

			//ピクセルから光源までを計算
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
			
			//内積計算
			double dot = rvecx * vecx + rvecy * vecy + rvecz * vecz;
			if (dot < 0)continue;
			//スペキュラは適当に
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
	//		実行処理 (命令実行時に呼ばれます)
	//
	code_next();							// 次のコードを取得(最初に必ず必要です)

	switch( cmd ) {							// サブコマンドごとの分岐

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

static int ref_ival;						// 返値のための変数

static void *reffunc( int *type_res, int cmd )
{
	//		関数・システム変数の実行処理 (値の参照時に呼ばれます)
	//			'('で始まるかを調べる
	if ( *type != TYPE_MARK ) puterror( HSPERR_INVALID_FUNCPARAM );
	if ( *val != '(' ) puterror( HSPERR_INVALID_FUNCPARAM );
	code_next();

	switch( cmd ) {							// サブコマンドごとの分岐
		/*
	case 0x00:								// newcmd

		p1 = code_geti();				// 整数値を取得(デフォルトなし)
		ref_ival = p1 * 2;				// 返値をivalに設定
		break;
		*/
	default:
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
	}

	//			'('で終わるかを調べる
	if ( *type != TYPE_MARK ) puterror( HSPERR_INVALID_FUNCPARAM );
	if ( *val != ')' ) puterror( HSPERR_INVALID_FUNCPARAM );
	code_next();
	*type_res = HSPVAR_FLAG_INT;			// 返値のタイプを整数に指定する
	return (void *)&ref_ival;
}


/*------------------------------------------------------------*/

static int termfunc( int option )
{
	//		終了処理 (アプリケーション終了時に呼ばれます)
	return 0;
}

/*------------------------------------------------------------*/

static int eventfunc( int event, int prm1, int prm2, void *prm3 )
{
	//		イベント処理 (HSPイベント発生時に呼ばれます)
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
	//		DLLエントリー (何もする必要はありません)
	return TRUE;
}


EXPORT void WINAPI hsp3cmdinit( HSP3TYPEINFO *info )
{
	//		プラグイン初期化 (実行・終了処理を登録します)
	//
	hsp3sdk_init( info );			// SDKの初期化(最初に行なって下さい)
	info->cmdfunc = cmdfunc;		// 実行関数(cmdfunc)の登録
	info->reffunc = reffunc;		// 参照関数(reffunc)の登録
	info->termfunc = termfunc;		// 終了関数(termfunc)の登録
}


