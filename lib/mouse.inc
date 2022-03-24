proc lixus_lib_mouse_move_function uses rbx,state:QWORD
     local x:QWORD
     local y:QWORD
     local pos:POINT
     mov QWORD[state],rcx
     invoke GetCursorPos,addr pos
     mov rax,QWORD[pos.x]
     mov rbx,QWORD[pos.y]
     mov QWORD[x],rax
     mov QWORD[y],rbx
     stdcall lua_istable,QWORD[state],1
     .if rax = 1
	 invoke lua_getfield,QWORD[state],1,lixus_lib_mouse_move_x_title
	 .if rax = LUA_TNUMBER | rax = LUA_TSTRING
	     invoke lua_tointegerx,QWORD[state],-1,0
	     mov QWORD[x],rax
	 .endif

	 invoke lua_getfield,QWORD[state],1,lixus_lib_mouse_move_y_title
	 .if rax = LUA_TNUMBER | rax = LUA_TSTRING
	     invoke lua_tointegerx,QWORD[state],-1,0
	     mov QWORD[y],rax
	 .endif
     .else
	 invoke luaL_checkinteger,QWORD[state],1
	 mov QWORD[x],rax
	 invoke luaL_checkinteger,QWORD[state],2
	 mov QWORD[y],rax
     .endif
     invoke SetCursorPos,QWORD[x],QWORD[y]
     invoke lua_pushboolean,QWORD[state],rax
     mov rax,1
     ret
endp

proc lixus_lib_mouse_click_function uses rbx,state:QWORD
     local down:INPUT
     local up:INPUT
     local flags:QWORD
     local clicks:QWORD
     local pos:POINT
     local x:DWORD
     local y:DWORD
     local time:QWORD
     local button:QWORD
     local setpos:QWORD
     ;int3
     mov QWORD[state],rcx

     lea rcx,QWORD[down]
     lea r8,QWORD[setpos]
     sub r8,rcx
     invoke memset,rcx,0,r8

     ;mov QWORD[down.type],INPUT_MOUSE

     invoke GetCursorPos,addr pos

     mov rax,QWORD[pos.x]
     mov rbx,QWORD[pos.y]
     mov QWORD[x],rax
     mov QWORD[y],rbx
     mov QWORD[clicks],1
     mov QWORD[button],lixus_lib_mouse_click_l_title

     stdcall lua_istable,QWORD[state],1
     .if rax = 1
	 invoke lua_getfield,QWORD[state],1,lixus_lib_mouse_click_x_title
	 .if rax = LUA_TNUMBER
	     invoke lua_tointegerx,QWORD[state],-1,0
	     mov DWORD[x],eax
	     or DWORD[setpos],eax
	 .endif

	 invoke lua_getfield,QWORD[state],1,lixus_lib_mouse_click_y_title
	 .if rax = LUA_TNUMBER
	     invoke lua_tointegerx,QWORD[state],-1,0
	     mov DWORD[y],eax
	     or DWORD[setpos],eax
	 .endif

	 invoke lua_getfield,QWORD[state],1,lixus_lib_mouse_click_time_title
	 .if rax = LUA_TNUMBER
	     invoke lua_tointegerx,QWORD[state],-1,0
	     .if rax > 0
		 mov QWORD[time],rax
	     .endif
	 .endif

	 invoke lua_getfield,QWORD[state],1,lixus_lib_mouse_click_button_title
	 .if rax = LUA_TSTRING
	     invoke lua_tolstring,QWORD[state],-1,0
	     mov QWORD[button],rax
	 .endif

	 invoke lua_getfield,QWORD[state],1,lixus_lib_mouse_click_clicks_title
	 .if rax = LUA_TNUMBER
	     invoke lua_tointegerx,QWORD[state],-1,0
	     mov QWORD[clicks],rax
	 .endif
     .else
	 stdcall lua_isstring,QWORD[state],1
	 .if rax = 1
	     invoke luaL_checklstring,QWORD[state],1,0
	     mov QWORD[button],rax
	 .endif

	 invoke lua_isnumber,QWORD[state],2
	 .if rax = 1
	     invoke lua_tointegerx,QWORD[state],2,0
	     mov QWORD[clicks],rax
	 .endif

	 invoke lua_isnumber,QWORD[state],3
	 .if rax = 1
	     invoke lua_tointegerx,QWORD[state],3,0
	     mov DWORD[x],eax
	     or DWORD[setpos],eax
	 .endif

	 invoke lua_isnumber,QWORD[state],4
	 .if rax = 1
	     invoke lua_tointegerx,QWORD[state],4,0
	     mov DWORD[y],eax
	     or DWORD[setpos],eax
	 .endif

	 invoke lua_isnumber,QWORD[state],5
	 .if rax = 1
	     invoke lua_tointegerx,QWORD[state],5,0
	     .if rax > 0
		 mov QWORD[time],rax
	     .endif
	 .endif
     .endif

     .if QWORD[clicks] > 0
	 invoke strstr,QWORD[button],lixus_lib_mouse_click_l_title
	 .if rax > 0
	     or DWORD[down.mi.dwFlags],MOUSEEVENTF_LEFTDOWN
	     or DWORD[up.mi.dwFlags],MOUSEEVENTF_LEFTUP
	 .endif

	 invoke strstr,QWORD[button],lixus_lib_mouse_click_m_title
	 .if rax > 0
	     or DWORD[down.mi.dwFlags],MOUSEEVENTF_MIDDLEDOWN
	     or DWORD[up.mi.dwFlags],MOUSEEVENTF_MIDDLEUP
	 .endif

	 invoke strstr,QWORD[button],lixus_lib_mouse_click_r_title
	 .if rax > 0
	     or DWORD[down.mi.dwFlags],MOUSEEVENTF_RIGHTDOWN
	     or DWORD[up.mi.dwFlags],MOUSEEVENTF_RIGHTUP
	 .endif
     .endif
     ;int3
     .if QWORD[setpos] > 0
	 or DWORD[down.mi.dwFlags],(MOUSEEVENTF_MOVE or MOUSEEVENTF_ABSOLUTE)
	 or DWORD[up.mi.dwFlags],(MOUSEEVENTF_MOVE or MOUSEEVENTF_ABSOLUTE)
	 stdcall fixXY,DWORD[x],DWORD[y]
	 ;mov ebx,DWORD[x]
	 ;mov ecx,DWORD[y]
	 mov DWORD[down.mi.dx],ebx
	 mov DWORD[down.mi.dy],ecx
	 mov DWORD[up.mi.dx],ebx
	 mov DWORD[up.mi.dy],ecx
     .endif


     mov rax,QWORD[time]
     mov DWORD[down.mi.time],eax
     mov DWORD[up.mi.time],eax

     ;invoke GetMessageExtraInfo
     ;mov QWORD[down.mi.dwExtraInfo],rax
     ;mov QWORD[up.mi.dwExtraInfo],rax

     .while QWORD[clicks] > 0
	    invoke mouse_event,DWORD[down.mi.dwFlags],DWORD[down.mi.dx],DWORD[down.mi.dy],DWORD[down.mi.mouseData],DWORD[down.mi.dwExtraInfo]
	    invoke mouse_event,DWORD[up.mi.dwFlags],DWORD[up.mi.dx],DWORD[up.mi.dy],DWORD[up.mi.mouseData],DWORD[up.mi.dwExtraInfo]
	    ;invoke SendInput,1,addr down,sizeof.INPUT
	    ;invoke SendInput,1,addr up,sizeof.INPUT
	    dec QWORD[clicks]
     .endw
     xor rax,rax
     ret
endp

proc luaopen_mouse
     stdcall luaL_newlib,rcx,lixus_lib_mouse,QWORD[lixus_lib_mouse_size]
     xor rax,rax
     inc rax
     ret
endp