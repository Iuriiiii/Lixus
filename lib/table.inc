proc lixus_table_getn_function state:QWORD
     local c:QWORD

     mov QWORD[state],rcx
     invoke luaL_checktype,rcx,1,LUA_TTABLE
     xor rax,rax
     mov QWORD[c],rax
     invoke lua_pushnil,QWORD[state]
     .repeat
	 invoke lua_next,QWORD[state],-2
	 .if rax = 0
	     jmp .END_REPEAT
	 .endif
	 inc QWORD[c]
	 stdcall lua_pop,QWORD[state],1
     .endr
.END_REPEAT:
     invoke lua_pushinteger,QWORD[state],QWORD[c]
     mov rax,1
     ret
endp