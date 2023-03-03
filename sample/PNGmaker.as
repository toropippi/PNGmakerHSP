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
		dialog ""+fname+"のチャンネル数が4ではありません",1
		end
	}
	buffer gsel1,x,y
	buffer gsel2,x,y
	PngLoad_ fname,gsel1,gsel2
	return
#global




/*
	//		PngSave 命令の処理
	//		(PngSave "ファイル名",p1,p2)
	//		"ファイル名"で指定したファイルにp1のgselで指定された画面のrgb内容を、p2の画面のrをαとしてpngでsaveする。
	//		p1省略時カレントウィンドウになる
	//		p2省略時αは255になる


	//		PngLoad 命令の処理
	//		(PngLoad "ファイル名",p1,p2)
	//		"ファイル名"で指定したファイルを読み込みp1にrgb,p2にaを書き込みます
	

	//		PngLoadXYN 命令の処理
	//		(PngLoadXYN "ファイル名",p2,p3,p4)
	//		"ファイル名"で指定したファイルのx,y,n情報をp2,p3,p4に書き込む


	//		AlphaBlur 命令の処理
	//		(AlphaBlur p1)
	//		p1の画面で白(のr成分)を不透明、黒を透明として、不透明部分の輪郭がぼやっとなるようにぼやかす処理
	//		p2でブラーの強度 整数値を取得、ブラー半径、ピクセル
	//		p3ウェイトの指定 整数値を取得、ウェイト、256で一番不透明に近くなる

	
	//		(ColortoVal p1,p2,p3,p4,p5)
	//		p1の画面でcolor(p2,p3,p4)で指定した色をp5にする
	

	//		(NotColortoVal p1,p2,p3,p4,p5)
	//		p1の画面でcolor(p2,p3,p4)で指定した色以外をp5にする


	//		(MulVal p1,p2)
	//		p1の画面で全部の色をp2倍する


	//		(PixelHeight p1)
	//		p1の画面で黒を高さ0として、黒以外のピクセルの高さを求める。黒からの距離が遠いほど高い
	//		出力は同じp1の画面にピクセルとして出力


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

*/