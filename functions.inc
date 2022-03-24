proc lixus_success_function state:QWORD
     local c:QWORD
     mov QWORD[state],rcx
     invoke lua_gettop,QWORD[state]
     .if rax > 0
	 mov QWORD[c],rax
	 .while QWORD[c] > 0
		stdcall lua_isnoneornil,QWORD[state],rax
		.if rax = 1
		    dec rax
		    jmp .END
		.endif
		dec QWORD[c]
	 .endw
	 invoke lua_pushboolean,QWORD[state],1
	 mov rax,1
     .endif
.END:
     ret
endp

proc lixus_failure_function state:QWORD
     local c:QWORD
     mov QWORD[state],rcx
     invoke lua_gettop,QWORD[state]
     .if rax > 0
	 mov QWORD[c],rax
	 .while QWORD[c] > 0
		stdcall lua_isnoneornil,QWORD[state],rax
		.if rax <> 0
		    jmp .END
		.endif
		dec QWORD[c]
	 .endw
	 invoke lua_pushboolean,QWORD[state],1
	 mov rax,1
     .endif
.END:
     ret
endp

proc lixus_checktype_function state:QWORD
     local type:QWORD
     mov QWORD[state],rcx
     invoke luaL_checkinteger,QWORD[state],2
     invoke luaL_checktype,QWORD[state],1,rax
     ret
endp