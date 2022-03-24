proc luaL_dostring l:QWORD
     mov QWORD[l],rcx
     invoke luaL_loadstring,rcx,rdx
     invoke lua_pcallk,QWORD[l],0,LUA_MULTRET,0,0,0
     ret
endp

proc lua_pushcfunction
     invoke lua_pushcclosure,rcx,rdx,0
     ret
endp

proc lua_register l:QWORD,n:QWORD,f:QWORD
     mov QWORD[l],rcx
     mov QWORD[n],rdx
     invoke lua_pushcclosure,rcx,r8,0
     invoke lua_setglobal,QWORD[l],QWORD[n]
     ret
endp

proc lua_pop
     imul rdx,-1
     dec rdx
     invoke lua_settop,rcx,rdx
     ret
endp

proc luaL_openlib
     invoke luaL_setfuncs,rcx,r8,r9
     ret
endp

proc luaL_register
     stdcall luaL_openlib,rcx,rdx,r8,0
     ret
endp

proc luaL_newlibtable
     mov r8,rdx
     dec r8
     xor rdx,rdx
     call [lua_createtable]
     ret
endp

proc luaL_newlib l1:QWORD,l2:QWORD
     mov QWORD[l1],rcx
     mov QWORD[l2],rdx
     stdcall luaL_newlibtable,rcx,r8
     invoke luaL_setfuncs,QWORD[l1],QWORD[l2],0
     ret
endp

proc luaL_loadfile
     xor r8,r8
     invoke luaL_loadfilex,rcx,rdx,r8
     ret
endp

proc lua_pcall
     invoke lua_pcallk,rcx,rdx,r8,r9,0,0
     ret
endp

proc luaL_dofile l:QWORD
     mov QWORD[l],rcx
     stdcall luaL_loadfile,rcx,rdx
     stdcall lua_pcall,QWORD[l],0,LUA_MULTRET,0
     ret
endp

proc lua_tostring
     invoke lua_tolstring,rcx,rdx,0
     ret
endp

proc lua_newtable
     invoke lua_createtable,rcx,0,0
     ret
endp

proc lua_tointeger
     invoke lua_tointegerx,rcx,rdx,0
     ret
endp

; Propias

proc lua_getindexorfield state:QWORD,index:QWORD,field:QWORD
     mov QWORD[state],rcx
     mov QWORD[index],rdx
     mov QWORD[field],r8
     invoke lua_getfield,QWORD[state],-1,QWORD[field]
     .if eax = LUA_TNIL | eax = LUA_TNONE
	 stdcall lua_pop,QWORD[state],1
	 invoke lua_pushinteger,QWORD[state],QWORD[index]
	 invoke lua_gettable,QWORD[state],-2
     .endif
     ret
endp

proc lua_pushss state:QWORD,name:QWORD,s:QWORD
     mov QWORD[state],rcx
     mov QWORD[name],rdx
     mov QWORD[s],r8
     invoke lua_pushstring,rcx,rdx
     invoke lua_pushstring,QWORD[state],QWORD[s]
     invoke lua_settable,QWORD[state],-3
     ret
endp

proc lua_pushsi state:QWORD,name:QWORD,i:QWORD
     mov QWORD[state],rcx
     mov QWORD[name],rdx
     mov QWORD[i],r8
     invoke lua_pushstring,rcx,rdx
     invoke lua_pushinteger,QWORD[state],QWORD[i]
     invoke lua_settable,QWORD[state],-3
     ret
endp

proc lua_pushsf state:QWORD,name:QWORD,f:QWORD
     mov QWORD[state],rcx
     mov QWORD[name],rdx
     mov QWORD[f],r8
     invoke lua_pushstring,rcx,rdx
     stdcall lua_pushcfunction,QWORD[state],QWORD[f]
     invoke lua_settable,QWORD[state],-3
     ret
endp

proc lua_isfunction
     invoke lua_type,rcx,rdx
     .if rax = LUA_TFUNCTION
	 mov rax,1
     .else
	 xor rax,rax
     .endif
     ret
endp

proc lua_istable
     invoke lua_type,rcx,rdx
     .if rax = LUA_TTABLE
	 mov rax,1
     .else
	 xor rax,rax
     .endif
     ret
endp

proc lua_islightuserdata
     invoke lua_type,rcx,rdx
     .if rax = LUA_TLIGHTUSERDATA
	 mov rax,1
     .else
	 xor rax,rax
     .endif
     ret
endp

proc lua_isnil
     invoke lua_type,rcx,rdx
     .if eax = LUA_TNIL
	 mov rax,1
     .else
	 xor rax,rax
     .endif
     ret
endp

proc lua_isboolean
     invoke lua_type,rcx,rdx
     .if rax = LUA_TBOOLEAN
	 mov rax,1
     .else
	 xor rax,rax
     .endif
     ret
endp

proc lua_isthread
     invoke lua_type,rcx,rdx
     .if rax = LUA_TTHREAD
	 mov rax,1
     .else
	 xor rax,rax
     .endif
     ret
endp

proc lua_isnone
     invoke lua_type,rcx,rdx
     .if eax = LUA_TNONE
	 mov rax,1
     .else
	 xor rax,rax
     .endif
     ret
endp

proc lua_isnoneornil
     invoke lua_type,rcx,rdx
     .if eax = LUA_TNONE | eax = LUA_TNIL
	 xor rax,rax
	 inc eax
     .else
	 xor eax,eax
     .endif
     ret
endp