proc InjectCRT uses rdi,rsi,pid:QWORD,dll:QWORD
     ARRAY_SIZE equ 50
     local hProc:QWORD
     local hLib:QWORD
     local buff[ARRAY_SIZE]:BYTE
     local RemoteStr:QWORD
     local LoadLibAddr:QWORD
     local dlllen:QWORD
     local hThread:QWORD

     mov QWORD[pid],rcx
     mov QWORD[dll],rdx

     lea rcx,QWORD[hProc]
     lea r8,QWORD[hThread]
     sub r8,rcx
     invoke memset,rcx,0,r8

     .if QWORD[pid] = 0
	 jmp .RETURN_FALSE
     .endif

     .if QWORD[dll] = 0
	 jmp .RETURN_FALSE
     .endif

     invoke OpenProcess,PROCESS_ALL_ACCESS,FALSE,QWORD[pid]
     .if eax = 0
	 jmp .RETURN_FALSE
     .endif
     mov DWORD[hProc],eax

     invoke strlen,QWORD[dll]
     .if eax = 0
	 jmp .RETURN_FALSE
     .endif
     mov DWORD[dlllen],eax

     invoke VirtualAllocEx,DWORD[hProc],0,eax,MEM_RESERVE or MEM_COMMIT,PAGE_READWRITE
     .if eax = 0
	 jmp .RETURN_FALSE
     .endif
     mov DWORD[RemoteStr],eax

     invoke WriteProcessMemory,DWORD[hProc],eax,DWORD[dll],DWORD[dlllen],0
     .if eax = 0
	 invoke VirtualFreeEx,DWORD[hProc],DWORD[RemoteStr],0,MEM_RELEASE
	 invoke CloseHandle,DWORD[hProc]
	 jmp .RETURN_FALSE
     .endif

     invoke CreateRemoteThread,DWORD[hProc],0,0,QWORD[LoadLibrary],DWORD[RemoteStr],0,0
     .if eax = 0
	 invoke VirtualFreeEx,DWORD[hProc],DWORD[RemoteStr],0,MEM_RELEASE
	 invoke CloseHandle,DWORD[hProc]
	 jmp .RETURN_FALSE
     .endif
     mov DWORD[hThread],eax

     invoke WaitForSingleObject,eax,INFINITE
     invoke VirtualFreeEx,DWORD[hProc],DWORD[RemoteStr],0,MEM_RELEASE
     .if eax = 0
	 jmp .RETURN_FALSE
     .endif

     invoke CloseHandle,DWORD[hThread]
     invoke CloseHandle,DWORD[hProc]
     mov eax,1
     jmp .END
.RETURN_FALSE:
     mov eax,0
.END:
     ret
     purge ARRAY_SIZE
endp

proc lixus_lib_process_list_function state:QWORD
     local pe32:PROCESSENTRY32
     local initialtable:QWORD
     local snapshot:QWORD
     local c:QWORD
     mov QWORD[state],rcx
     lea rcx,QWORD[pe32]
     mov r8,rbp
     sub r8,rsp
     invoke memset,rcx,0,r8
     mov DWORD[pe32.dwSize],PROCESSENTRY32_SIZE
     stdcall lua_newtable,QWORD[state]
     mov QWORD[initialtable],rax
     invoke CreateToolhelp32Snapshot,TH32CS_SNAPPROCESS,0
     .if rax = 0
	 stdcall lua_pop,QWORD[state],1
	 invoke lua_pushboolean,QWORD[state],0
	 jmp .END
     .endif
     mov QWORD[snapshot],rax
     lea rdx,QWORD[pe32]
     invoke Process32First,rax,rdx
     .while rax > 0
	    inc QWORD[c]
	    invoke lua_pushinteger,QWORD[state],QWORD[c]
	    invoke lua_createtable,QWORD[state],0,3
	    lea r8,QWORD[pe32.szExeFile]
	    stdcall lua_pushss,QWORD[state],lixus_list_name_title,r8 ;
	    stdcall lua_pushsi,QWORD[state],lixus_list_pid_title,DWORD[pe32.th32ProcessID]
	    stdcall lua_pushsi,QWORD[state],lixus_list_parentpid_title,DWORD[pe32.th32ParentProcessID]
	    invoke lua_settable,QWORD[state],-3
	    lea rdx,QWORD[pe32]
	    invoke Process32Next,QWORD[snapshot],rdx
     .endw
     invoke CloseHandle,QWORD[snapshot]
.END:
     xor rax,rax
     inc rax

     ret
endp

proc processExists type:QWORD,data:QWORD
     local pe32:PROCESSENTRY32
     local snapshot:QWORD
     mov QWORD[type],rcx
     mov QWORD[data],rdx
     lea rcx,QWORD[pe32]
     mov r8,rbp
     sub r8,rsp
     invoke memset,rcx,0,r8
     mov DWORD[pe32.dwSize],PROCESSENTRY32_SIZE
     invoke CreateToolhelp32Snapshot,TH32CS_SNAPPROCESS,0
     mov QWORD[snapshot],rax
     .if rax = 0
	 jmp .END
     .endif
     lea rdx,QWORD[pe32]
     invoke Process32First,rax,rdx
     .while rax > 0
	    .if QWORD[type] = 1
		lea rcx,QWORD[pe32.szExeFile]
		invoke strcmp,rcx,QWORD[data]
		.if rax = 0
		    mov eax,DWORD[pe32.th32ProcessID]
		    jmp .END_WHILE
		.endif
	    .else
		mov eax,DWORD[pe32.th32ProcessID]
		.if DWORD[data] = eax
		    jmp .END_WHILE
		.endif
	    .endif
	    lea rdx,QWORD[pe32]
	    invoke Process32Next,QWORD[snapshot],rdx
     .endw
     xor rax,rax
.END_WHILE:
     push rax
     invoke CloseHandle,QWORD[snapshot]
     pop rax
.END:
     ret
endp

PROCESS_ALL_ACCESS equ 0x1f0fff

proc process_select_close_function state:QWORD
     mov QWORD[state],rcx
     invoke luaL_checktype,rcx,1,LUA_TTABLE
     invoke lua_getfield,QWORD[state],1,lixus_select_table_handle_title
     .if rax = 0
	 jmp .END
     .endif
     invoke lua_tointegerx,QWORD[state],-1,0
     invoke CloseHandle,rax
.END:
     xor rax,rax
     ret
endp

proc process_select_kill_function state:QWORD
     local handle:QWORD
     local reazon:QWORD
     mov QWORD[state],rcx
     invoke luaL_checktype,rcx,1,LUA_TTABLE
     invoke lua_getfield,QWORD[state],1,lixus_select_table_handle_title
     .if rax = 0
	 jmp .END
     .endif
     invoke lua_tointegerx,QWORD[state],-1,0
     mov QWORD[handle],rax
     stdcall lua_isnoneornil,QWORD[state],2
     .if rax = 1
	 xor rax,rax
     .else
	 invoke luaL_checktype,QWORD[state],2,LUA_TNUMBER
	 invoke lua_tointegerx,QWORD[state],2,0
     .endif
     ;mov QWORD[reazon],rax
     invoke TerminateProcess,QWORD[handle],rax
.END:
     xor rax,rax
     ret
endp

proc process_select_suspend_function state:QWORD
     mov QWORD[state],rcx
     invoke luaL_checktype,rcx,1,LUA_TTABLE
     invoke lua_getfield,QWORD[state],1,lixus_select_table_handle_title
     .if rax = 0
	 jmp .END
     .endif
     invoke lua_tointegerx,QWORD[state],-1,0
     invoke NtSuspendProcess,rax
.END:
     xor rax,rax
     ret
endp

proc process_select_resume_function state:QWORD
     mov QWORD[state],rcx
     invoke luaL_checktype,rcx,1,LUA_TTABLE
     invoke lua_getfield,QWORD[state],1,lixus_select_table_handle_title
     .if rax = 0
	 jmp .END
     .endif
     invoke lua_tointegerx,QWORD[state],-1,0
     invoke NtResumeProcess,rax
.END:
     xor rax,rax
     ret
endp

proc process_select_getpath_function state:QWORD
     local pid:QWORD
     local buff:QWORD
     mov QWORD[state],rcx
     invoke luaL_checktype,rcx,1,LUA_TTABLE
     invoke lua_getfield,QWORD[state],1,lixus_select_table_pid_title
     .if rax = 0
	 stdcall lua_pop,QWORD[state],1
	 invoke lua_pushboolean,QWORD[state],0
	 jmp .END
     .endif
     invoke lua_tointegerx,QWORD[state],-1,0
     mov QWORD[pid],rax
     stdcall malloc,MAX_PATH
     mov QWORD[buff],rax
     invoke GetModuleFileNameEx,QWORD[pid],0,rax,MAX_PATH
     invoke lua_pushstring,QWORD[state],QWORD[buff]
     stdcall mfree,QWORD[buff]
.END:
     mov rax,1
     ret
endp

proc process_select_wait_function state:QWORD
     local time:QWORD
     local handle:QWORD
     mov QWORD[state],rcx
     invoke lua_getfield,rcx,1,lixus_select_table_pid_title
     .if rax = 0
	 jmp .END
     .endif
     invoke lua_tointegerx,QWORD[state],-1,0
     mov QWORD[handle],rax
     stdcall lua_isinteger,QWORD[state],2
     .if rax = 1
	 invoke luaL_checkinteger,QWORD[state],1,0
	 mov QWORD[time],rax
     .else
	 mov QWORD[time],-1
     .endif

     invoke WaitForSingleObject,QWORD[handle],QWORD[time]
     xor rax,rax
.END:
     ret
endp

proc lixus_lib_process_inject_function state:QWORD
     local dll:QWORD
     local handle:QWORD

     mov QWORD[state],rcx
     invoke luaL_checktype,rcx,1,LUA_TTABLE
     invoke lua_getfield,QWORD[state],1,lixus_select_table_handle_title
     .if rax = 0
	 jmp .END
     .endif
     invoke lua_tointegerx,QWORD[state],-1,0
     mov QWORD[handle],rax

     invoke luaL_checklstring,QWORD[state],2,0
     mov QWORD[dll],rax

     stdcall InjectCRT,QWORD[handle],QWORD[dll]
.END:
     invoke lua_pushboolean,QWORD[state],eax
     mov rax,1
     ret
endp

proc lixus_lib_process_select_function state:QWORD
     local opflag:QWORD
     local bInheritHandle:QWORD
     local pid:QWORD
     local bsaltar:QWORD

     mov QWORD[state],rcx
     mov QWORD[bsaltar],0

     stdcall lua_isnoneornil,rcx,2

     .if rax = 1
	 mov QWORD[opflag],PROCESS_ALL_ACCESS
     .else
	 invoke lua_tointegerx,QWORD[state],2,0
	 mov QWORD[opflag],rax
     .endif

     stdcall lua_isnoneornil,QWORD[state],3

     .if rax = 1
	 xor rax,rax
     .else
	 invoke luaL_checktype,QWORD[state],3,LUA_TBOOLEAN
	 invoke lua_toboolean,QWORD[state],3
     .endif

     mov QWORD[bInheritHandle],rax
     ;invoke lua_gettop,QWORD[state]
     invoke lua_createtable,QWORD[state],0,6
     stdcall lua_istable,QWORD[state],1
     ;push rax
     ;invoke lua_gettop,QWORD[state]
     ;pop rax
     .if rax = 1
	 invoke lua_getfield,QWORD[state],1,lixus_list_pid_title
	 .if rax > 0
	     invoke lua_tointegerx,QWORD[state],-1,0
	     xor rcx,rcx
	 .endif
	 push rax
	 ;invoke lua_gettop,QWORD[state]
	 stdcall lua_pop,QWORD[state],1
	 ;invoke lua_gettop,QWORD[state]
	 pop rax
     .else
	 invoke lua_isinteger,QWORD[state],1
	 .if rax = 0
	     stdcall lua_isnoneornil,QWORD[state],1
	     .if rax = 1
		 mov QWORD[bsaltar],1
		 invoke GetCurrentProcessId
	     .else
		 invoke luaL_checklstring,QWORD[state],1,0
	     .endif
	     mov rcx,1
	 .else
	     invoke lua_tointegerx,QWORD[state],1,0
	     xor rcx,rcx
	 .endif
     .endif

     .if QWORD[bsaltar] = 0
	 stdcall processExists,rcx,rax
     .endif

     .if rax = 0
.ERROR:
	 stdcall lua_pop,QWORD[state],1
	 invoke lua_pushboolean,QWORD[state],0
     .else
	 mov QWORD[pid],rax
	 invoke OpenProcess,QWORD[opflag],QWORD[bInheritHandle],rax
	 .if rax = 0
	     jmp .ERROR
	 .endif
	 stdcall lua_pushsi,QWORD[state],lixus_select_table_handle_title,rax
	 stdcall lua_pushsi,QWORD[state],lixus_select_table_pid_title,QWORD[pid]
	 stdcall lua_pushsf,QWORD[state],lixus_select_table_close_function_title,process_select_close_function
	 stdcall lua_pushsf,QWORD[state],lixus_select_table_kill_function_title,process_select_kill_function
	 stdcall lua_pushsf,QWORD[state],lixus_select_table_suspend_function_title,process_select_suspend_function
	 stdcall lua_pushsf,QWORD[state],lixus_select_table_resume_function_title,process_select_resume_function
	 stdcall lua_pushsf,QWORD[state],lixus_select_table_getpath_function_title,process_select_getpath_function
	 stdcall lua_pushsf,QWORD[state],lixus_select_table_wait_function_title,process_select_wait_function
	 ;stdcall lua_pushsf,QWORD[state],lixus_select_table_inject_function_title,lixus_lib_process_inject_function
     .endif

     xor rax,rax
     inc rax

     ret
endp

proc luaopen_process
     stdcall luaL_newlib,rcx,lixus_lib_process,QWORD[lixus_lib_process_size]
     xor rax,rax
     inc rax
     ret
endp