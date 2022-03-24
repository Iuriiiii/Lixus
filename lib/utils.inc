proc lixus_lib_utils_setmsgtitle_function state:QWORD
     mov QWORD[state],rcx
     invoke luaL_checklstring,QWORD[state],1,0
     mov QWORD[lixus_msgbox_title],rax
     xor rax,rax
     ret
endp

proc lixus_lib_utils_showmsg_function state:QWORD
     local text:QWORD
     local title:QWORD
     local flag:QWORD

     mov QWORD[state],rcx
     invoke luaL_checklstring,QWORD[state],1,0
     mov QWORD[text],rax

     stdcall lua_isnoneornil,QWORD[state],2
     .if rax = 0
	 invoke luaL_checklstring,QWORD[state],2,0
	 mov QWORD[title],rax
     .else
	 mov rax,QWORD[lixus_msgbox_title]
	 mov QWORD[title],rax
     .endif

     stdcall lua_isnoneornil,QWORD[state],3
     .if rax = 0
	 invoke luaL_checkinteger,QWORD[state],3
	 mov QWORD[flag],rax
     .endif

     invoke MessageBox,0,QWORD[text],QWORD[title],QWORD[flag]

     .if rax = 3
	 invoke lua_pushstring,QWORD[state],showMsg_abort_title
     .elseif rax = 2
	     invoke lua_pushstring,QWORD[state],showMsg_cancel_title
     .elseif rax = 11
	     invoke lua_pushstring,QWORD[state],showMsg_continue_title
     .elseif rax = 5
	     invoke lua_pushstring,QWORD[state],showMsg_ignore_title
     .elseif rax = 7
	     invoke lua_pushstring,QWORD[state],showMsg_no_title
     .elseif rax = 1
	     invoke lua_pushstring,QWORD[state],showMsg_ok_title
     .elseif rax = 4
	     invoke lua_pushstring,QWORD[state],showMsg_retry_title
     .elseif rax = 10
	     invoke lua_pushstring,QWORD[state],showMsg_tryagain_title
     .elseif rax = 6
	     invoke lua_pushstring,QWORD[state],showMsg_yes_title
     .endif

     xor rax,rax
     inc rax

     ret
endp

proc lixus_lib_utils_sleep_function state:QWORD
     mov QWORD[state],rcx
     invoke luaL_checkinteger,rcx,1
     invoke Sleep,rax
     xor rax,rax
     ret
endp

proc lixus_lib_utils_processmessages_function state:QWORD
     local msg:MSG
     .repeat
	   invoke GetMessage,addr msg,0,0,0
	   .if rax = 0
	       invoke TranslateMessage,addr msg
	       invoke DispatchMessage,addr msg
	   .endif
     .endr
     ret
endp

proc lixus_lib_utils_rgbtohls_function state:QWORD
     local h:DWORD
     local l:DWORD
     local s:DWORD
     xor rax,rax
     mov r15,rax
     mov QWORD[state],rcx
     mov DWORD[h],eax
     mov DWORD[l],eax
     mov DWORD[s],eax
     stdcall lua_istable,rcx,1
     .if rax = 1
	 invoke lua_getfield,QWORD[state],1,lixus_lib_utils_rgbtohls_r_title
	 .if rax = LUA_TNUMBER
	     invoke lua_tointegerx,QWORD[state],-1,0
	     mov r15b,al
	     shl r15d,1
	 .endif

	 invoke lua_getfield,QWORD[state],1,lixus_lib_utils_rgbtohls_g_title
	 .if rax = LUA_TNUMBER
	     invoke lua_tointegerx,QWORD[state],-1,0
	     mov r15b,al
	     shl r15d,1
	 .endif

	 invoke lua_getfield,QWORD[state],1,lixus_lib_utils_rgbtohls_b_title
	 .if rax = LUA_TNUMBER
	     invoke lua_tointegerx,QWORD[state],-1,0
	     mov r15b,al
	 .endif
	 mov rax,r15
     .else
	 invoke luaL_checkinteger,QWORD[state],1
	 invoke lua_tointegerx,QWORD[state],1,0
     .endif
     invoke ColorRGBToHLS,rax,addr h,addr l,addr s
     invoke lua_pushinteger,QWORD[state],DWORD[h]
     invoke lua_pushinteger,QWORD[state],DWORD[l]
     invoke lua_pushinteger,QWORD[state],DWORD[s]
     mov rax,3
     ret
endp

proc lixus_lib_utils_hlstorgb_function state:QWORD
     local r:DWORD
     local g:DWORD
     local b:DWORD
     local h:DWORD
     local l:DWORD
     local s:DWORD
     xor rax,rax
     mov r15,rax
     mov QWORD[state],rcx
     mov DWORD[r],eax
     mov DWORD[g],eax
     mov DWORD[b],eax
     stdcall lua_istable,rcx,1
     .if rax = 1
	 invoke lua_getfield,QWORD[state],1,lixus_lib_utils_rgbtohls_h_title
	 .if rax = LUA_TNUMBER
	     invoke lua_tointegerx,QWORD[state],-1,0
	     movzx DWORD[h],ax
	 .endif

	 invoke lua_getfield,QWORD[state],1,lixus_lib_utils_rgbtohls_l_title
	 .if rax = LUA_TNUMBER
	     invoke lua_tointegerx,QWORD[state],-1,0
	     movzx DWORD[l],ax
	 .endif

	 invoke lua_getfield,QWORD[state],1,lixus_lib_utils_rgbtohls_s_title
	 .if rax = LUA_TNUMBER
	     invoke lua_tointegerx,QWORD[state],-1,0
	     movzx DWORD[s],ax
	 .endif
     .else
	 invoke luaL_checkinteger,QWORD[state],1
	 invoke lua_tointegerx,QWORD[state],1,0
	 movzx DWORD[s],al
	 shr eax,1
	 movzx DWORD[l],al
	 shr eax,1
	 mov WORD[h],ax
     .endif
     invoke ColorHLSToRGB,DWORD[h],DWORD[l],DWORD[s]
     invoke lua_pushinteger,QWORD[state],eax
     mov rax,1
     ret
endp

proc luaopen_utils
     stdcall luaL_newlib,rcx,lixus_lib_utils,QWORD[lixus_lib_utils_size]
     xor rax,rax
     inc rax
     ret
endp