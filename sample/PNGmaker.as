#regcmd "_hsp3cmdinit@4","PNGmaker.dll"
#cmd PngSave $000
#cmd PngLoad_ $001
#cmd PngLoadXYN_ $002
#cmd AlphaBlur $003
#cmd ColortoVal $004
#cmd NotColortoVal $005
#cmd MulVal $006


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