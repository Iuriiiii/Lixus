proc GetWindowText hwnd:QWORD
     local len:QWORD
     local buff:QWORD
     mov QWORD[hwnd],rcx
     stdcall GetWindowTextLength,rcx
     .if rax = 0
         stdcall malloc,1
         xor rbx,rbx
         jmp .END
     .endif
     inc rax
     mov QWORD[len],rax
     push rbx
     .if ebx = 1
         imul eax,2
     .endif
     stdcall malloc,rax
     mov QWORD[buff],rax
     pop rbx
     .if ebx = 1 ; ebx = 1 if unicode
         invoke GetWindowTextW,QWORD[hwnd],QWORD[buff],QWORD[len]
         stdcall utoa,QWORD[buff]
         xchg QWORD[buff],rax
         stdcall mfree,rax
     .else
         invoke GetWindowTextA,QWORD[hwnd],QWORD[buff],QWORD[len]
     .endif
     mov rax,QWORD[buff]
     mov rbx,QWORD[len]
.END:
     ret
endp

proc SetWindowText hwnd:QWORD,text:QWORD
     mov QWORD[hwnd],rcx
     mov QWORD[text],rdx
     invoke IsWindowUnicode,QWORD[hwnd]
     .if rax = 1
         stdcall atou,QWORD[text]
         xchg QWORD[text],rax
         invoke SetWindowTextW,QWORD[hwnd],QWORD[text]
         push rax
         stdcall mfree,QWORD[text]
         pop rax
     .else
         invoke SetWindowTextA,QWORD[hwnd],QWORD[text]
     .endif
     ret
endp

proc GetWindowTextLength hwnd:QWORD
     mov QWORD[hwnd],rcx
     invoke IsWindowUnicode,rcx
     .if rax = 0
         invoke GetWindowTextLengthA,QWORD[hwnd]
         xor ebx,ebx
     .else
         invoke GetWindowTextLengthW,QWORD[hwnd]
         xor ebx,ebx
         inc ebx
     .endif
     ;inc rax
     ret
endp

proc EnumWindowsProc hwnd:QWORD,array:QWORD
     local title:QWORD
     local c:QWORD
     local state:QWORD
     local processid:DWORD

     .if rcx = 0
         jmp .END
     .endif

     mov rax,QWORD[rdx+8]
     mov QWORD[state],rax
     inc QWORD[rdx]
     mov rax,QWORD[rdx]
     mov QWORD[c],rax
     mov QWORD[hwnd],rcx
     invoke lua_pushinteger,QWORD[state],QWORD[c]
     ;stdcall lua_newtable,QWORD[state]
     invoke lua_createtable,QWORD[state],0,3
     stdcall lua_pushsi,QWORD[state],lixus_lib_window_list_hwnd_title,QWORD[hwnd]
     stdcall GetWindowText,QWORD[hwnd]
     mov QWORD[title],rax
     stdcall lua_pushss,QWORD[state],lixus_lib_window_list_text_title,rax
     stdcall mfree,QWORD[title]
     invoke GetWindowThreadProcessId,QWORD[hwnd],addr processid
     stdcall lua_pushsi,QWORD[state],lixus_lib_window_list_pid_title,DWORD[processid]
     invoke lua_settable,QWORD[state],-3
.END:
     xor rax,rax
     inc rax
     ret
endp

proc lixus_lib_window_list_function state:QWORD
     local array:QWORD

     mov QWORD[state],rcx
     mov r15,rcx

     stdcall malloc,2*8

     mov QWORD[rax+8],r15
     mov QWORD[array],rax

     stdcall lua_isnoneornil,QWORD[state],1
     .if rax = 1
         invoke lua_createtable,rcx,0,1
         invoke EnumWindows,EnumWindowsProc,QWORD[array]
     .else
         stdcall lua_istable,QWORD[state],1
         .if rax = 1
             invoke lua_getfield,QWORD[state],1,lixus_lib_window_list_hwnd_title
             invoke luaL_checkinteger,QWORD[state],-1,0
         .else
             invoke luaL_checkinteger,QWORD[state],1,0
         .endif
         invoke lua_createtable,rcx,0,1
         invoke EnumChildWindows,rax,EnumWindowsProc,QWORD[array]
     .endif
     stdcall mfree,QWORD[array]
     ret
endp

proc select_action_function state:QWORD
     local hwnd:QWORD
     local action:QWORD

     mov QWORD[state],rcx

     stdcall lua_istable,QWORD[state],1
     .if rax = 0
         jmp .END
     .endif

     stdcall lua_isnoneornil,QWORD[state],2
     .if rax = 1
         dec rax
         jmp .END
     .endif

     invoke luaL_checklstring,QWORD[state],2,0
     mov QWORD[action],rax
     invoke lua_getfield,QWORD[state],1,lixus_lib_window_list_hwnd_title
     invoke luaL_checkinteger,QWORD[state],-1
     mov QWORD[hwnd],rax

     .if rax = 0
         jmp .END
     .endif

     invoke strcmp,QWORD[action],lixus_lib_window_select_action_enable_title
     .if rax = 0
         invoke EnableWindow,QWORD[hwnd],1
         jmp .RETURN
     .endif

     invoke strcmp,QWORD[action],lixus_lib_window_select_action_disable_title
     .if rax = 0
         invoke EnableWindow,QWORD[hwnd],0
         jmp .RETURN
     .endif

     invoke strcmp,QWORD[action],lixus_lib_window_select_action_hide_title
     .if rax = 0
         mov rax,0
         jmp .SHOW_WINDOW
     .endif

     invoke strcmp,QWORD[action],lixus_lib_window_select_action_show_title
     .if rax = 0
         mov rax,5
         jmp .SHOW_WINDOW
     .endif

     invoke strcmp,QWORD[action],lixus_lib_window_select_action_minimize_title
     .if rax = 0
         mov rax,6
         jmp .SHOW_WINDOW
     .endif

     invoke strcmp,QWORD[action],lixus_lib_window_select_action_maximize_title
     .if rax = 0
         mov rax,3
         jmp .SHOW_WINDOW
     .endif

     invoke strcmp,QWORD[action],lixus_lib_window_select_action_close_title
     .if rax = 0
         invoke DestroyWindow,QWORD[hwnd]
         jmp .RETURN
     .endif

     invoke strcmp,QWORD[action],lixus_lib_window_select_action_restore_title
     .if rax = 0
         mov rax,9
         jmp .SHOW_WINDOW
     .endif

     invoke strcmp,QWORD[action],lixus_lib_window_select_action_capturemouse_title
     .if rax = 0
         invoke SetCapture,QWORD[hwnd]
         jmp .RETURN
     .endif

     invoke strcmp,QWORD[action],lixus_lib_window_select_action_releasemouse_title
     .if rax = 0
         invoke ReleaseCapture
         jmp .RETURN
     .endif

     invoke strcmp,QWORD[action],lixus_lib_window_select_action_focus_title
     .if rax = 0
         invoke SetFocus,QWORD[hwnd]
         jmp .RETURN
     .endif

     invoke strcmp,QWORD[action],lixus_lib_window_select_action_activate_title
     .if rax = 0
         invoke SetActiveWindow,QWORD[hwnd]
         jmp .RETURN
     .endif

     jmp .RETURN
.SHOW_WINDOW:
     invoke ShowWindow,QWORD[hwnd],rax
.RETURN:
     invoke lua_pushboolean,QWORD[state],rax
     mov rax,1
.END:
     ret
endp

proc select_text_function uses rbx,state:QWORD
     local text:QWORD
     local hwnd:QWORD
     local param2:QWORD
     ;int3
     ;push rbx
     mov QWORD[state],rcx

     stdcall lua_isnoneornil,rcx,2
     mov QWORD[param2],rax

     stdcall lua_istable,QWORD[state],1
     .if rax = 0
         jmp .END
     .endif

     invoke lua_getfield,QWORD[state],1,lixus_lib_window_list_hwnd_title
     .if rax <> LUA_TNUMBER
         jmp .END
     .endif

     invoke lua_tointegerx,QWORD[state],-1,0
     mov QWORD[hwnd],rax
     stdcall lua_pop,QWORD[state],1

     .if QWORD[param2] = 1
         stdcall GetWindowText,QWORD[hwnd]
         mov QWORD[text],rax
         invoke lua_pushstring,QWORD[state],rax
         stdcall mfree,QWORD[text] ; return 1
         jmp .END
     .else
         invoke luaL_checklstring,QWORD[state],2,0
         mov QWORD[text],rax
         stdcall SetWindowText,QWORD[hwnd],rax
         .if rax = 1
             invoke lua_pushstring,QWORD[state],QWORD[text]
         .else
             invoke lua_pushboolean,QWORD[state],rax
         .endif
         mov rax,1
     .endif
.END:
     ;pop rbx
     ret
endp

proc select_is_function state:QWORD
     local hwnd:QWORD
     local type:QWORD
     local wp:WINDOWPLACEMENT

     mov QWORD[state],rcx

     stdcall lua_istable,QWORD[state],1
     .if rax = 0
         jmp .END
     .endif

     invoke luaL_checklstring,QWORD[state],2,0
     mov QWORD[type],rax

     invoke lua_getfield,QWORD[state],1,lixus_lib_window_list_hwnd_title
     .if rax = 0
         jmp .END
     .endif

     invoke lua_tointegerx,QWORD[state],-1,0
     mov QWORD[hwnd],rax

     invoke strcmp,QWORD[type],lixus_lib_window_select_is_enabled_title
     .if rax = 0
         invoke IsWindowEnabled,QWORD[hwnd]
         jmp .RETURN
     .endif

     invoke strcmp,QWORD[type],lixus_lib_window_select_is_disabled_title
     .if rax = 0
         invoke IsWindowEnabled,QWORD[hwnd]
         not eax
         jmp .RETURN
     .endif

     invoke strcmp,QWORD[type],lixus_lib_window_select_is_focused_title
     .if rax = 0
         invoke GetFocus
         .if rax <> QWORD[hwnd]
             jmp .FALSE
         .endif
         jmp .RETURN
     .endif

     invoke strcmp,QWORD[type],lixus_lib_window_select_is_hidden_title
     .if rax = 0
         invoke IsWindowVisible,QWORD[hwnd]
         .if rax <> 0
             jmp .FALSE
         .endif
         inc rax
         jmp .RETURN
     .endif

     invoke strcmp,QWORD[type],lixus_lib_window_select_is_shown_title
     .if rax = 0
         invoke IsWindowVisible,QWORD[hwnd]
         jmp .RETURN
     .endif

     invoke strcmp,QWORD[type],lixus_lib_window_select_is_actived_title
     .if rax = 0
         invoke GetActiveWindow,QWORD[hwnd]
         .if rax = QWORD[hwnd]
             jmp .RETURN
         .endif
         jmp .FALSE
     .endif

     invoke strcmp,QWORD[type],lixus_lib_window_select_is_minimized_title
     .if rax = 0
         invoke IsIconic,QWORD[hwnd]
         jmp .RETURN
     .endif

     invoke strcmp,QWORD[type],lixus_lib_window_select_is_maximized_title
     .if rax = 0
         mov DWORD[wp.length],sizeof.WINDOWPLACEMENT
         invoke GetWindowPlacement,QWORD[hwnd],addr wp
         .if rax = 0
             jmp .END
         .endif
         .if DWORD[wp.showCmd] <> 3
             jmp .FALSE
         .endif
         jmp .RETURN
     .endif

     invoke strcmp,QWORD[type],lixus_lib_window_select_is_capturing_title
     .if rax = 0
         invoke GetCapture
         .if rax <> QWORD[hwnd]
             jmp .FALSE
         .endif
         jmp .RETURN
     .endif

     invoke strcmp,QWORD[type],lixus_lib_window_select_is_tallest_title
     .if rax = 0
         invoke GetTopWindow,0
         .if rax <> QWORD[hwnd]
             jmp .FALSE
         .endif
         jmp .RETURN
     .endif

     xor rax,rax
     jmp .END
.FALSE:
     xor rax,rax
.RETURN:
     invoke lua_pushboolean,QWORD[state],rax
     mov rax,1
.END:
     ret
endp

proc select_position_function state:QWORD
     local rc:RECT
     local hwnd:QWORD
     local x:QWORD
     local y:QWORD
     local height:QWORD
     local width:QWORD
     local brepaint:QWORD

     mov QWORD[state],rcx

     stdcall lua_istable,QWORD[state],1
     .if rax = 0
         jmp .RETURN_NIL
     .endif

     invoke lua_getfield,QWORD[state],1,lixus_lib_window_list_hwnd_title
     .if rax = 0
         jmp .RETURN_NIL
     .endif

     invoke lua_tointegerx,QWORD[state],-1,0
     mov QWORD[hwnd],rax

     invoke GetWindowRect,rax,addr rc

     xor rax,rax
     mov QWORD[brepaint],rax
     mov eax,DWORD[rc.left]
     mov DWORD[x],eax
     mov eax,DWORD[rc.top]
     mov DWORD[y],eax
     mov eax,DWORD[rc.bottom]
     sub eax,DWORD[rc.top]
     mov DWORD[height],eax
     mov eax,DWORD[rc.right]
     sub eax,DWORD[rc.left]
     mov DWORD[width],eax

     stdcall lua_isnoneornil,QWORD[state],2
     .if rax = 1
         invoke lua_pushinteger,QWORD[state],QWORD[x]
         invoke lua_pushinteger,QWORD[state],QWORD[y]
         invoke lua_pushinteger,QWORD[state],QWORD[width]
         invoke lua_pushinteger,QWORD[state],QWORD[height]
         mov rax,4
         jmp .END
     .endif

     invoke luaL_checktype,QWORD[state],2,LUA_TTABLE

     invoke lua_getfield,QWORD[state],2,lixus_lib_window_select_position_x_title
     .if rax = LUA_TNUMBER
         invoke lua_tointegerx,QWORD[state],-1,0
         mov QWORD[x],rax
     .endif

     invoke lua_getfield,QWORD[state],2,lixus_lib_window_select_position_y_title
     .if rax = LUA_TNUMBER
         invoke lua_tointegerx,QWORD[state],-1,0
         mov QWORD[y],rax
     .endif

     invoke lua_getfield,QWORD[state],2,lixus_lib_window_select_position_width_title
     .if rax = LUA_TNUMBER
         invoke lua_tointegerx,QWORD[state],-1,0
         mov QWORD[width],rax
     .endif

     invoke lua_getfield,QWORD[state],2,lixus_lib_window_select_position_height_title
     .if rax = LUA_TNUMBER
         invoke lua_tointegerx,QWORD[state],-1,0
         mov QWORD[height],rax
     .endif

     invoke lua_getfield,QWORD[state],2,lixus_lib_window_select_position_repaint_title
     .if rax = LUA_TBOOLEAN
         invoke lua_toboolean,QWORD[state],-1
         mov QWORD[brepaint],rax
     .endif

     invoke MoveWindow,QWORD[hwnd],QWORD[x],QWORD[y],QWORD[width],QWORD[height],QWORD[brepaint]
     invoke lua_pushboolean,QWORD[state],rax
     mov rax,1
     jmp .END
.RETURN_NIL:
     xor rax,rax
.END:
     ret
endp

proc select_post_getdword state:QWORD
     local value:QWORD

     mov QWORD[state],rcx
     xor rax,rax
     mov QWORD[value],rax

     stdcall lua_getindexorfield,QWORD[state],2,lixus_lib_window_select_post_h_title

     .if rax = LUA_TNUMBER
         invoke lua_tointegerx,QWORD[state],-1,0
         shl eax,16
         mov DWORD[value],eax
     .elseif rax = LUA_TTABLE

         stdcall lua_getindexorfield,QWORD[state],2,lixus_lib_window_select_post_h_title
         .if rax = LUA_TNUMBER
             shl eax,8
             mov DWORD[value],eax
         .endif

         stdcall lua_getindexorfield,QWORD[state],1,lixus_lib_window_select_post_l_title
         .if rax = LUA_TNUMBER
              mov BYTE[value],al
              shl DWORD[value],8
         .endif
     .endif

     stdcall lua_getindexorfield,QWORD[state],1,lixus_lib_window_select_post_l_title
     .if rax = LUA_TNUMBER
         mov WORD[value],ax
     .elseif rax = LUA_TTABLE

         stdcall lua_getindexorfield,QWORD[state],2,lixus_lib_window_select_post_h_title
         .if rax = LUA_TNUMBER
             shl eax,8
             mov BYTE[value],al
         .endif

         stdcall lua_getindexorfield,QWORD[state],1,lixus_lib_window_select_post_l_title
         .if rax = LUA_TNUMBER
             mov BYTE[value],al
             shl DWORD[value],8
         .endif
     .endif
     mov rax,QWORD[value]
     ret
endp

proc select_post_function state:QWORD
     local hwnd:QWORD
     local msg:QWORD
     local wparam:QWORD
     local lparam:QWORD

     xor rax,rax

     mov QWORD[state],rcx
     mov QWORD[msg],rax
     mov QWORD[wparam],rax
     mov QWORD[lparam],rax

     invoke lua_getfield,rcx,1,lixus_lib_window_list_hwnd_title
     invoke lua_tointegerx,QWORD[state],-1,0
     mov QWORD[hwnd],rax

     stdcall lua_istable,QWORD[state],2
     .if eax = 1
         invoke lua_getfield,QWORD[state],2,lixus_lib_window_select_post_msg_title
         .if rax = LUA_TNUMBER
             invoke lua_tointegerx,QWORD[state],-1,0
             mov QWORD[msg],rax
         .endif

         invoke lua_getfield,QWORD[state],2,lixus_lib_window_select_post_lparam_title
         .if rax = LUA_TNUMBER
             invoke lua_tointegerx,QWORD[state],-1,0
             mov QWORD[lparam],rax
         .elseif rax = LUA_TTABLE
             stdcall select_post_getdword,QWORD[state]
             mov QWORD[lparam],rax
         .endif

         invoke lua_getfield,QWORD[state],2,lixus_lib_window_select_post_wparam_title
         .if rax = LUA_TNUMBER
             invoke lua_tointegerx,QWORD[state],-1,0
             mov QWORD[wparam],rax
         .elseif rax = LUA_TSTRING

         .elseif rax = LUA_TTABLE
             stdcall select_post_getdword,QWORD[state]
             mov QWORD[wparam],rax
         .endif
     .else
        invoke luaL_checkinteger,QWORD[state],2
        mov QWORD[msg],rax

        stdcall lua_tointegerx,QWORD[state],3,0
        mov QWORD[lparam],rax

        stdcall lua_tointegerx,QWORD[state],4,0
        mov QWORD[wparam],rax
     .endif
     invoke PostMessage,QWORD[hwnd],QWORD[msg],QWORD[wparam],QWORD[lparam]
.END:
     ret
endp

WM_LBUTTONDOWN = 0x201
WM_LBUTTONUP   = 0x202
MK_CONTROL = 0x0008
MK_SHIFT = 0x0004
MK_MBUTTON = 0x0010
MK_RBUTTON = 0x0002
MK_LBUTTON = 0x0001
WM_MBUTTONUP = 0x0208
WM_MBUTTONDOWN = 0x0207
WM_RBUTTONDOWN = 0x0204
WM_RBUTTONUP = 0x0205

proc select_click_function state:QWORD
     local buttondown:QWORD
     local buttonup:QWORD
     local buttons:QWORD
     local ignoreextras:QWORD
     local x:QWORD
     local y:QWORD
     local flag:QWORD
     local r:RECT
     local hwnd:QWORD
     local lpos:QWORD
     local mpos:QWORD
     local rpos:QWORD
     local pos:QWORD
     local clicks:QWORD

     ;int3

     mov QWORD[state],rcx
     xor rax,rax
     mov QWORD[flag],rax
     mov QWORD[ignoreextras],rax
     mov QWORD[buttons],rax
     inc rax
     mov QWORD[clicks],rax

     invoke lua_getfield,rcx,1,lixus_lib_window_list_hwnd_title
     invoke lua_tointegerx,QWORD[state],-1,0

     mov QWORD[hwnd],rax

     invoke GetWindowRect,rax,addr r

     xor rax,rax
     xor rdx,rdx
     mov rcx,2
     mov eax,DWORD[r.right]
     idiv ecx
     xor rdx,rdx
     mov DWORD[x],eax
     mov eax,DWORD[r.bottom]
     idiv ecx
     mov DWORD[y],eax

     mov QWORD[buttondown],WM_LBUTTONDOWN
     mov QWORD[buttonup],WM_LBUTTONUP

     stdcall lua_istable,QWORD[state],2
     .if rax = 1
         ; ¿Qué botones actuarán?
         invoke lua_getfield,QWORD[state],2,lixus_lib_window_select_click_button_title
         .if rax = LUA_TSTRING
             invoke lua_tolstring,QWORD[state],-1,0

             mov QWORD[ignoreextras],rax
             mov QWORD[buttons],rax
         .endif

         invoke lua_getfield,QWORD[state],2,lixus_lib_window_select_click_x_title
         .if rax = LUA_TNUMBER
             invoke lua_tointegerx,QWORD[state],-1,0
             mov QWORD[x],rax
         .endif

         invoke lua_getfield,QWORD[state],2,lixus_lib_window_select_click_y_title
         .if rax = LUA_TNUMBER
             invoke lua_tointegerx,QWORD[state],-1,0
             mov QWORD[y],rax
         .endif

         invoke lua_getfield,QWORD[state],2,lixus_lib_window_select_click_ctrl_title
         .if rax = LUA_TNUMBER
             invoke lua_tointegerx,QWORD[state],-1,0
             .if rax > 0
                 or QWORD[flag],MK_CONTROL
             .endif
         .endif

         invoke lua_getfield,QWORD[state],2,lixus_lib_window_select_click_shift_title
         .if rax = LUA_TNUMBER
             invoke lua_tointegerx,QWORD[state],-1,0
             .if rax > 0
                 or QWORD[flag],MK_SHIFT
             .endif
         .endif

         invoke lua_getfield,QWORD[state],2,lixus_lib_window_select_click_clicks_title
         .if rax = LUA_TNUMBER
             invoke lua_tointegerx,QWORD[state],-1,0
             .if rax > 0
                 mov QWORD[clicks],rax
             .endif
         .endif

     .else
         invoke luaL_checklstring,QWORD[state],2,0
         mov QWORD[buttons],rax

         invoke lua_isinteger,QWORD[state],3
         .if rax > 0
             invoke lua_tointegerx,QWORD[state],4,0
             .if rax > 0
                 mov QWORD[clicks],rax
             .endif
         .endif

         invoke lua_isinteger,QWORD[state],4
         .if rax > 0
             invoke lua_tointegerx,QWORD[state],3,0
             .if rax < 0
                 xor rax,rax
             .endif
             mov QWORD[x],rax
         .endif

         invoke lua_isinteger,QWORD[state],5
         .if rax > 0
             invoke lua_tointegerx,QWORD[state],4,0
             .if rax < 0
                 xor rax,rax
             .endif
             mov QWORD[y],rax
         .endif

         stdcall lua_isboolean,QWORD[state],6
         .if rax > 0
             invoke lua_toboolean,QWORD[state],5
             .if rax > 0
                 or QWORD[flag],MK_CONTROL
             .endif
         .endif

         stdcall lua_isboolean,QWORD[state],7
         .if rax > 0
             invoke lua_toboolean,QWORD[state],6
             .if rax > 0
                 or QWORD[flag],MK_SHIFT
             .endif
         .endif
     .endif

             .if QWORD[buttons] > 0
                 invoke strstr,QWORD[buttons],lixus_lib_window_select_click_button_l_title
                 mov QWORD[lpos],rax

                 invoke strstr,QWORD[buttons],lixus_lib_window_select_click_button_m_title
                 mov QWORD[mpos],rax

                 invoke strstr,QWORD[buttons],lixus_lib_window_select_click_button_r_title
                 mov QWORD[rpos],rax

                 .if QWORD[ignoreextras] = 0
                     invoke strstr,QWORD[buttons],lixus_lib_window_select_click_button_s_title
                     .if rax > 0
                         or QWORD[flag],MK_SHIFT
                     .endif

                     invoke strstr,QWORD[buttons],lixus_lib_window_select_click_button_c_title
                     .if rax > 0
                         or QWORD[flag],MK_CONTROL
                     .endif
                 .endif

                 mov rax,QWORD[lpos]
                 mov rbx,QWORD[mpos]
                 mov rcx,QWORD[rpos]

                 .if (rax > rbx) & (rax > rcx)
                     mov QWORD[buttondown],WM_LBUTTONDOWN
                     mov QWORD[buttonup],WM_LBUTTONUP
                     or QWORD[flag],MK_LBUTTON
                     .if rbx > 0
                         or QWORD[flag],MK_MBUTTON
                     .endif
                     .if rcx > 0
                         or QWORD[flag],MK_RBUTTON
                         .endif
                 .elseif (rbx > rax) & (rbx > rcx)
                     mov QWORD[buttondown],WM_MBUTTONDOWN
                     mov QWORD[buttonup],WM_MBUTTONUP
                     or QWORD[flag],MK_MBUTTON
                     .if rax > 0
                         or QWORD[flag],MK_LBUTTON
                     .endif
                     .if rcx > 0
                         or QWORD[flag],MK_RBUTTON
                     .endif
                 .elseif (rcx > rax) & (rcx > rbx)
                     mov QWORD[buttondown],WM_RBUTTONDOWN
                     mov QWORD[buttonup],WM_RBUTTONUP
                     or QWORD[flag],MK_RBUTTON
                     .if rax > 0
                         or QWORD[flag],MK_LBUTTON
                     .endif
                     .if rbx > 0
                         or QWORD[flag],MK_MBUTTON
                     .endif
                 ;.elseif rax = 0 & rbx = 0 & rcx = 0
                 ;     mov QWORD[buttondown],WM_LBUTTONDOWN
                 ;     mov QWORD[buttonup],WM_LBUTTONUP
                 .endif
             .else
                 or QWORD[flag],MK_LBUTTON
             .endif

     ;invoke lua_getfield,QWORD[state],1,lixus_lib_window_list_hwnd_title
     stdcall makelparam,QWORD[y],QWORD[x]

     mov QWORD[pos],rax

     .while QWORD[clicks] > 0
            invoke PostMessage,QWORD[hwnd],QWORD[buttondown],QWORD[flag],QWORD[pos]
            invoke PostMessage,QWORD[hwnd],QWORD[buttonup]  ,QWORD[flag],QWORD[pos]
            dec QWORD[clicks]
     .endw
     xor rax,rax
     ret
endp

WH_CALLWNDPROC = 4
WH_GETMESSAGE = 3

proc WindowHook nCode:QWORD,wParam:QWORD,lParam:QWORD
     local hwnd:QWORD
     local hookid:QWORD
     local fptr:QWORD
     local state:QWORD
     ;int3
     nop
     ;invoke printf,"on hook"
     mov QWORD[nCode],rcx
     mov QWORD[wParam],rdx
     mov QWORD[lParam],r8
     .if rcx <> 0
         jmp .NEXT
     .endif
     ;int3
     virtual at r8
             ;.minfo CWPSTRUCT
             .minfo MSG
     end virtual

     mov rax,QWORD[.minfo.hwnd]
     mov QWORD[hwnd],rax

     invoke GetProp,rax,lixus_lib_window_select_sethook_fptr_title
     .if rax = 0
         jmp .NEXT
     .endif
     mov QWORD[fptr],rax

     invoke GetProp,QWORD[hwnd],lixus_lib_window_select_sethook_hookid_title
     .if rax = 0
         jmp .NEXT
     .endif
     mov QWORD[hookid],rax
     ;int3

     ;invoke luaL_newstate
     ;mov QWORD[state],rax
     ;invoke lua_pushinteger,rax,QWORD[fptr]
     ;stdcall lua_pcall,QWORD[state],0,0,0
     ;invoke lua_close,QWORD[state]
     ;invoke printf,"qweqw"
.NEXT:
     invoke CallNextHookEx,QWORD[hookid],QWORD[nCode],QWORD[wParam],QWORD[lParam]
     ret
endp

proc select_sethook_function state:QWORD
     local hwnd:QWORD
     ;int3
     mov QWORD[state],rcx
     invoke lua_getfield,rcx,1,lixus_lib_window_list_hwnd_title
     invoke lua_tointegerx,QWORD[state],-1,0
     mov QWORD[hwnd],rax
     stdcall lua_isfunction,QWORD[state],2
     .if rax = 1
         invoke lua_topointer,QWORD[state],2
     .else
         invoke luaL_checklstring,QWORD[state],2,0
         invoke lua_getglobal,QWORD[state],rax
         invoke lua_tointegerx,QWORD[state],-1,0
     .endif
     mov QWORD[lixus_lib_window_hook_function],rax
     invoke SetProp,QWORD[hwnd],lixus_lib_window_select_sethook_fptr_title,rax
     ;int3
     invoke SetWindowsHookEx,WH_GETMESSAGE,WindowHook,0,QWORD[tid]
     .if rax = 0
         invoke RemoveProp,QWORD[hwnd],lixus_lib_window_select_sethook_fptr_title
         dec rax
         ;invoke printf,"error"
     .else
         invoke SetProp,QWORD[hwnd],lixus_lib_window_select_sethook_hookid_title,rax
         ;invoke printf,"success"
     .endif

     invoke lua_pushboolean,QWORD[state],rax
     mov rax,1
     ret
endp

proc lixus_lib_window_select_function state:QWORD
     local hwnd:QWORD
     local isnumber:QWORD

     mov QWORD[state],rcx

     stdcall lua_istable,rcx,1
     .if rax = 1
         invoke lua_getfield,QWORD[state],1,lixus_lib_window_list_hwnd_title
         invoke luaL_checkinteger,QWORD[state],-1,0
     .else
         mov QWORD[isnumber],rax
         invoke lua_isinteger,QWORD[state],1
         .if rax = 1
             invoke lua_tointegerx,QWORD[state],1,addr isnumber
             .if QWORD[isnumber] = 0
                 jmp .IS_STRING
             .endif
         .else
.IS_STRING:
              invoke lua_tolstring,QWORD[state],1,0
              invoke FindWindow,0,rax
         .endif
     .endif

     mov QWORD[hwnd],rax

     invoke IsWindow,rax
     .if rax = 0
         invoke lua_pushboolean,QWORD[state],rax
         jmp .END
     .endif

     invoke  lua_createtable,QWORD[state],0,5
     stdcall lua_pushsi,QWORD[state],lixus_lib_window_list_hwnd_title,QWORD[hwnd]
     stdcall lua_pushsf,QWORD[state],lixus_lib_window_select_action_title,select_action_function
     stdcall lua_pushsf,QWORD[state],lixus_lib_window_select_text_title,select_text_function
     stdcall lua_pushsf,QWORD[state],lixus_lib_window_select_is_title,select_is_function
     stdcall lua_pushsf,QWORD[state],lixus_lib_window_select_position_title,select_position_function
     stdcall lua_pushsf,QWORD[state],lixus_lib_window_select_click_title,select_click_function
     stdcall lua_pushsf,QWORD[state],lixus_lib_window_select_sethook_title,select_sethook_function
     stdcall lua_pushsf,QWORD[state],lixus_lib_window_select_post_title,select_post_function
.END:
     mov rax,1
     ret
endp

proc luaopen_window
     stdcall luaL_newlib,rcx,lixus_lib_window,QWORD[lixus_lib_window_size]
     xor rax,rax
     inc rax
     ret
endp