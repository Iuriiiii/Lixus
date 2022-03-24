GMEM_MOVEABLE = 0x0002
GHND = 0x0042
GMEM_FIXED = 0x0000
GMEM_ZEROINIT = 0x0040
GPTR = 0x0040

CF_TEXT = 1

proc lixus_lib_clip_put_function state:QWORD
     local buff:QWORD
     local len:QWORD
     local hmem:QWORD
     local s:QWORD
     local lock:QWORD

     mov QWORD[state],rcx

     invoke luaL_checklstring,QWORD[state],1,addr len

     mov QWORD[s],rax
     mov rax,QWORD[len]
     imul rax,2
     mov QWORD[len],rax

     invoke OpenClipboard,0
     .if rax = 0
	 jmp .RETURN
     .endif

     invoke GlobalAlloc,GMEM_MOVEABLE,QWORD[len]
     mov QWORD[hmem],rax

     invoke GlobalLock,rax
     mov QWORD[lock],rax

     invoke lstrcpy,rax,QWORD[s]
     invoke SetClipboardData,CF_TEXT,QWORD[hmem]
     invoke GlobalUnlock,QWORD[lock]
     invoke CloseClipboard
.RETURN:
     invoke lua_pushboolean,QWORD[state],rax
.END:
     mov rax,1
     ret
endp

proc lixus_lib_clip_clear_function state:QWORD
     invoke OpenClipboard,0
     .if rax = 0
	 jmp .RETURN
     .endif
     invoke EmptyClipboard
     invoke CloseClipboard
.RETURN:
     invoke lua_pushboolean,QWORD[state],rax
     ret
endp

proc lixus_lib_clip_get_function state:QWORD
     local buff:QWORD
     local hmem:QWORD
     local lock:QWORD
     local buff:QWORD

     mov QWORD[state],rcx
     ;int3
     invoke OpenClipboard,0
     .if rax = 0
	 jmp .RETURN_EMPTY
     .endif

     invoke GetClipboardData,CF_TEXT
     .if rax = 0
	 jmp .RETURN_EMPTY
     .endif

     mov QWORD[hmem],rax
     stdcall malloc,<invoke lstrlen,rax>
     mov QWORD[buff],rax
     invoke lstrcpy,rax,QWORD[hmem]
     ;invoke GlobalLock,rax
     ;mov QWORD[lock],rax

     ;invoke calloc,1,<invoke lstrlen,rax>
     ;mov QWORD[buff],rax
     ;invoke lstrcpy,rax,QWORD[lock]
     ;invoke GlobalUnlock,QWORD[lock]
     invoke CloseClipboard
     jmp .RETURN
.RETURN_EMPTY:
     stdcall malloc,1
     mov QWORD[buff],rax
.RETURN:
     invoke lua_pushstring,QWORD[state],QWORD[buff]
     stdcall mfree,QWORD[buff]
     mov rax,1
     ret
endp

proc luaopen_clip
     stdcall luaL_newlib,rcx,lixus_lib_clip,QWORD[lixus_lib_clip_size]
     xor rax,rax
     inc rax
     ret
endp