CP_ACP = 0
CP_UTF8 = 65001

HEAP_ZERO_MEMORY = 0x00000008

proc malloc
     invoke HeapAlloc,QWORD[pHead],HEAP_ZERO_MEMORY,rcx
     ret
endp

proc mfree
     mov r8,rcx
     invoke HeapFree,QWORD[pHead],1,r8
     ret
endp

proc fixXY
     local x:DWORD
     local y:DWORD
     local rx:DWORD
     local ry:DWORD
     mov DWORD[x],ecx
     mov DWORD[y],edx
     fld QWORD[limit_size]
     fld st0
     fidiv DWORD[desktop_width]
     fimul DWORD[x]
     fisttp DWORD[rx]
     fidiv DWORD[desktop_height]
     fimul DWORD[y]
     fisttp DWORD[ry]
     mov ebx,DWORD[rx]
     mov ecx,DWORD[ry]
     ret
endp

proc makelparam
     xor rax,rax
     shl ecx,16
     and edx,0xFFFF
     or ecx,edx
     mov eax,ecx
     ret
endp

proc atou str:QWORD
     local l:QWORD
     local l2:QWORD
     local b:QWORD
     mov QWORD[str],rcx
     invoke strlen,rcx
     .if rax = 0
	 lea rax,QWORD[str_empty]
	 jmp .END
     .endif
     mov QWORD[l],rax
     inc rax
     imul rax,2
     mov QWORD[l2],rax
     stdcall malloc,rax
     mov QWORD[b],rax
     invoke MultiByteToWideChar,CP_UTF8,0,QWORD[str],QWORD[l],rax,QWORD[l2]
     mov rax,QWORD[b]
.END:
     ret
endp

proc utoa str:QWORD
     local l:QWORD
     local b:QWORD
     mov QWORD[str],rcx
     invoke wcslen,rcx
     .if rax = 0
	 lea rax,QWORD[str_empty]
	 jmp .END
     .endif
     mov QWORD[l],rax
     inc rax
     stdcall malloc,rax
     mov QWORD[b],rax
     invoke WideCharToMultiByte,CP_UTF8,0,QWORD[str],-1,rax,QWORD[l],0,0
     mov rax,QWORD[b]
.END:
     ret
endp

proc fastGetFileSize
     local WFAD:WIN32_FILE_ATTRIBUTE_DATA
     invoke GetFileAttributesEx,rcx,0,addr WFAD
     .if rax = 0
	 dec rax
	 jmp .END
     .endif
     mov eax,DWORD[WFAD.nFileSizeLow]
.END:
     ret
endp

ImageLockModeRead = 0
ImageLockModeWrite = 1
ImageLockModeUserInputBuf = 2

searchSize = 4

proc searchBitmap bmp1:QWORD,bmp2:QWORD,tolerance:QWORD
     local smallData:BitmapData
     local bigData:BitmapData
     local r1:RECT
     local smallBmp:BITMAP
     local bigBmp:BITMAP
     local smallOffset:DWORD
     local bigOffset:DWORD
     local bigWidth:DWORD
     local bigHeight:DWORD
     local smallWidth:DWORD
     local smallHeight:DWORD
     local hbitmapSmall:QWORD
     local hbitmapBig:QWORD
     local temp:QWORD
     local x:DWORD
     local y:DWORD
     local height:DWORD
     local width:DWORD
     local small:QWORD
     local big:QWORD
     local size1:DWORD
     local size2:DWORD
     local bigpath:QWORD
     local smallpath:QWORD

     mov QWORD[bmp1],rcx
     mov QWORD[bmp2],rdx
     imul r8,255
     mov QWORD[tolerance],r8

     invoke PathFindExtension,QWORD[bmp1]
     invoke strcmp,rax,bmpextension

     .if rax <> 0
	 mov rax,(-1)
	 jmp .END
     .endif

     invoke PathFindExtension,QWORD[bmp2]
     invoke strcmp,rax,bmpextension

     .if rax <> 0
	 mov rax,(-1)
	 jmp .END
     .endif

     lea rcx,QWORD[smallData]
     mov r8,rbp
     sub r8,rcx
     invoke memset,rcx,0,r8

     stdcall fastGetFileSize,QWORD[bmp1]

     .if eax = (-1)
	 jmp .END
     .else
	 mov DWORD[size1],eax
     .endif

     stdcall fastGetFileSize,QWORD[bmp2]

     .if eax = (-1)
	 jmp .END
     .else
	 mov DWORD[size2],eax
     .endif


     stdcall atou,QWORD[bmp1]
     mov r15,rax
     stdcall atou,QWORD[bmp2]

     mov ebx,DWORD[size2]
     .if DWORD[size1] > ebx
	 mov QWORD[bigpath],r15
	 mov QWORD[smallpath],rax
     .else
	 mov QWORD[smallpath],r15
	 mov QWORD[bigpath],rax
     .endif

     invoke GdipCreateBitmapFromFile,QWORD[bigpath],addr big
     invoke GdipCreateBitmapFromFile,QWORD[smallpath],addr small

     stdcall mfree,QWORD[bigpath]
     stdcall mfree,QWORD[smallpath]

     invoke GdipCreateHBITMAPFromBitmap,QWORD[small],addr hbitmapSmall,addr temp
     invoke GdipCreateHBITMAPFromBitmap,QWORD[big],addr hbitmapBig,addr temp

     invoke GetObject,QWORD[hbitmapSmall],sizeof.BITMAP,addr smallBmp
     invoke GetObject,QWORD[hbitmapBig],sizeof.BITMAP,addr bigBmp

     mov eax,DWORD[bigBmp.bmWidth]
     mov DWORD[bigWidth],eax

     ;int3
     mov eax,DWORD[bigBmp.bmHeight]
     sub eax,DWORD[smallBmp.bmHeight]
     inc eax
     mov DWORD[bigHeight],eax

     mov eax,DWORD[smallBmp.bmWidth]
     imul eax,searchSize
     mov DWORD[smallWidth],eax

     mov eax,DWORD[smallBmp.bmHeight]
     mov DWORD[smallHeight],eax

     ;mov rax,QWORD[smallBmp.bmWidth]
     ;imul rax,3
     ;sub rax,QWORD[smallData.Stride]
     ;mov QWORD[smallOffset],rax

     mov eax,DWORD[bigBmp.bmWidth]
     imul eax,3
     sub eax,DWORD[bigData.Stride]
     mov DWORD[bigOffset],eax

     mov eax,DWORD[smallBmp.bmWidth]
     mov ebx,DWORD[smallBmp.bmHeight]
     mov DWORD[r1.right],eax
     mov DWORD[r1.bottom],ebx
     invoke GdipBitmapLockBits,QWORD[small],addr r1,ImageLockModeRead,Format32bppRgb,addr smallData

     mov eax,DWORD[bigBmp.bmWidth]
     mov ebx,DWORD[bigBmp.bmHeight]
     mov DWORD[r1.right],eax
     mov DWORD[r1.bottom],ebx
     invoke GdipBitmapLockBits,QWORD[big],addr r1,ImageLockModeRead,Format32bppRgb,addr bigData

     mov rdi,QWORD[bigData.Scan0]
     mov rsi,QWORD[smallData.Scan0]
     ;r8 = y
     ;r9 = x
     ;r10 = i
     ;r11 = j
     ;rdi = bigData.Scan0
     ;rsi = smallData.Scan0

     ;rax = status (1 if true, 0 if false)
     xor rax,rax
     xor r8,r8
     ;xor r12,r12
     ;xor r13,r13

     .while r8d < DWORD[bigHeight]
	    xor r9,r9
	    .while r9d < DWORD[bigWidth]
		   mov ecx,edi
		   mov edx,esi
		   ;ecx = backup bigData.Scan0
		   ;edx = backup smallData.Scan0
		   xor r10,r10
		   .while r10d < DWORD[smallHeight]
			  ; eax deberia volverse verdadero...
			  inc eax
			  xor r11,r11
			  .while r11d < DWORD[smallWidth]
				 movzx r12d,BYTE[rdi]
				 mov r13d,r12d
				 sub r12d,DWORD[tolerance]
				 add r13d,DWORD[tolerance]
				 ;r12 = inf
				 ;r13 = sup
				 .if (r13b < BYTE[rsi]) | (r12b > BYTE[rsi])
				     xor eax,eax
				     jmp .BREAK_J
				 .endif
				 inc esi
				 inc edi
				 inc r11d
			  .endw
.BREAK_J:
			  .if eax = 0
			      jmp .BREAK_I
			  .endif

			  mov edi,ecx
			  mov esi,edx

			  mov r12d,DWORD[smallData.Stride]
			  mov r13d,DWORD[bigData.Stride]
			  mov r14d,r10d
			  inc r14d
			  imul r12d,r14d
			  imul r13d,r14d
			  add esi,r12d
			  add edi,r13d

			  inc r10d
		   .endw
.BREAK_I:
		  .if rax > 0
		      mov DWORD[x],r9d
		      mov DWORD[y],r8d
		      mov ecx,DWORD[smallBmp.bmWidth]
		      mov edx,DWORD[smallBmp.bmHeight]
		      mov DWORD[width],ecx
		      mov DWORD[height],edx
		      jmp .BREAK_X
		  .else
		      mov edi,ecx
		      mov esi,edx
		      add edi,searchSize
		  .endif

		  inc r9

		  ;.if r8 = 0x170 & r9 = 0x36A
		  ;    int3
		  ;.endif
	    .endw
.BREAK_X:
	    .if rax > 0
		jmp .BREAK_Y
	    .endif

	    add edi,DWORD[bigOffset]
	    inc r8
	    ;.if r8 = 0x170
	    ;	 int3
	    ;.endif
     .endw
.BREAK_Y:
     invoke GdipBitmapUnlockBits,QWORD[small],addr smallData
     invoke GdipBitmapUnlockBits,QWORD[big],addr bigData
     invoke DeleteObject,QWORD[hbitmapSmall]
     invoke DeleteObject,QWORD[hbitmapBig]
     invoke GdipDisposeImage,QWORD[small]
     invoke GdipDisposeImage,QWORD[big]
     mov eax,DWORD[x]
     mov ebx,DWORD[y]
     mov ecx,DWORD[width]
     mov edx,DWORD[height]
.END:
     ret
endp

proc testSearchBitmap

     ret
endp