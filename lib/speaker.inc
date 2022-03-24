proc lixus_lib_speaker_speak_function state:QWORD
     local text:QWORD
     local utext:QWORD
     local flag:QWORD
     mov QWORD[state],rcx
     mov QWORD[flag],SPF_IS_XML or SPF_ASYNC
     invoke luaL_checklstring,QWORD[state],1,0
     ;mov QWORD[text],rax
     stdcall atou,rax
     mov QWORD[utext],rax
     stdcall lua_istable,QWORD[state],2
     .if rax = 1
	 invoke lua_getfield,QWORD[state],2,lixus_lib_speaker_speak_async_title
	 invoke lua_toboolean,QWORD[state],-1
	 .if rax = 0
	     mov QWORD[flag],SPF_IS_XML
	 .endif
	 invoke lua_getfield,QWORD[state],2,lixus_lib_speaker_speak_isfile_title
	 invoke lua_toboolean,QWORD[state],-1
	 .if rax = 1
	     mov QWORD[flag],SPF_IS_XML or SPF_IS_FILENAME
	 .endif
     .endif
     cominvk oVoice,Speak,QWORD[utext],QWORD[flag],0
     stdcall mfree,QWORD[utext]
     xor rax,rax
     ret
endp

proc lixus_lib_speaker_wait_function state:QWORD
     local time:QWORD
     mov QWORD[state],rcx
     stdcall lua_isnoneornil,QWORD[state],1
     .if rax = 1
	 mov QWORD[time],-1
     .else
	 invoke luaL_checkinteger,QWORD[state],1
	 mov QWORD[time],rax
     .endif
     cominvk oVoice,WaitUntilDone,QWORD[time]
     xor rax,rax
     ret
endp

proc lixus_lib_speaker_volume_function state:QWORD
     local volume:QWORD
     mov QWORD[state],rcx
     stdcall lua_isnoneornil,QWORD[state],1
     .if rax = 1
	 mov QWORD[volume],0
	 lea rdi,QWORD[volume]
	 cominvk oVoice,GetVolume,rdi
	 invoke lua_pushinteger,QWORD[state],QWORD[volume]
	 mov rax,1
     .else
	 invoke luaL_checknumber,QWORD[state],1
	 and rax,100
	 cominvk oVoice,SetVolume,rax
	 xor rax,rax
     .endif
     ret
endp

proc lixus_lib_speaker_pause_function state:QWORD
     ;mov QWORD[state],rcx
     cominvk oVoice,Pause
     xor rax,rax
     ret
endp

proc lixus_lib_speaker_resume_function state:QWORD
     ;mov QWORD[state],rcx
     cominvk oVoice,Resume
     xor rax,rax
     ret
endp

proc luaopen_speaker
     stdcall luaL_newlib,rcx,lixus_lib_speaker,QWORD[lixus_lib_speaker_size]
     xor rax,rax
     inc rax
     ret
endp