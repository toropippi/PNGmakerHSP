//
//		HSP3.0 plugin sample
//		onion software/onitama 2004/9
//
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION


#include <windows.h>
#include <vector>
#include <string>
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
	//		(NotColortoVal p1,p2,p3,p4,p5)
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


