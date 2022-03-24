format PE64 GUI DLL

include 'win64ax.inc'
include './luac.inc'

entry EntryPoint

macro mov r,n
{
      if r eqtype rax
         if n eq 0
            xor r,r
         else if n eq 1
            xor r,r
            inc r
         else if n eq -1
            xor r,r
            dec r
         else
            mov r,n
         end if
      ;else if r eqtype qword[rsp+0] | r eqtype [rsp+0]
      ;   if n eq 0
      ;      mov r,r15
      ;   else
      ;      mov r,n
      ;   end if
      else
         mov r,n
      end if
}

section '.bbs' data readable writeable

        include './declare.inc'

        lua_state dd 0
        str_empty db 0,0,0,0,0,0,0
        lua_file  db 'E:\Prueba.lua',0

        include './variables.inc'

        ;tst du 'hola mundo',0
section '.data' code readable executable

include './utils.inc'
include './functions.inc'
include './lib/utils.inc'
include './lib/process.inc'
include './lib/speaker.inc'
include './lib/window.inc'
include './lib/clip.inc'
include './lib/table.inc'
include './lib/mouse.inc'

proc EntryPoint hinstDLL:QWORD,fdwReason:QWORD,lpvReserved:QWORD
     mov QWORD[hinstDLL],rcx
     mov QWORD[fdwReason],rdx
     mov QWORD[lpvReserved],r8

     .if rcx = DLL_PROCESS_ATTACH | rcx = DLL_THREAD_ATTACH
         stdcall initialize
     .elseif rcx = DLL_PROCESS_DETACH
         stdcall finalize
     .endif

     ret
endp

proc initialize
     invoke GetSystemMetrics,SM_CXSCREEN
     mov DWORD[desktop_width],eax
     invoke GetSystemMetrics,SM_CYSCREEN
     mov DWORD[desktop_height],eax
     invoke GetProcessHeap
     mov QWORD[pHead],rax
     invoke GetCurrentThreadId
     mov QWORD[tid],rax
     inc QWORD[GSI.GdiplusVersion]
     invoke GdiplusStartup,GdiPlusHandle,GSI,0
     .if rax > 0
         mov QWORD[GdiPlusEnabled],1
     .endif

     invoke CoInitialize,0
     .if rax = 0
         mov QWORD[com_inicialized],1
         invoke CoCreateInstance,CLSID_SpVoice,0,CLSCTX_ALL,IID_ISpVoice,oVoice
         .if rax = 0
             mov QWORD[SpVoice_initialized],1
         .endif
     .endif

     ret
endp

proc loadcfunctions state:QWORD
     mov QWORD[state],rcx
     stdcall lua_pushcfunction,rcx,lixus_success_function
     invoke lua_setglobal,QWORD[state],lixus_success_function_title
     stdcall lua_pushcfunction,QWORD[state],lixus_failure_function
     invoke lua_setglobal,QWORD[state],lixus_failure_function_title
     ;stdcall lua_pushcfunction,QWORD[state],lixus_checktype_function
     ;invoke lua_setglobal,QWORD[state],lixus_checktype_function_title
     ret
endp

proc finalize
     .if QWORD[com_inicialized] = 1
         .if QWORD[SpVoice_initialized] = 1
             cominvk oVoice,Release
         .endif
         invoke CoUninitialize
     .endif

     .if QWORD[GdiPlusEnabled] = 1
         invoke GdiplusShutdown,QWORD[GdiPlusHandle]
     .endif
     ret
endp

proc Lixus_EnableUtils
     mov BYTE[lixus_req_utils],cl
     ret
endp

proc Lixus_EnableProcess
     mov BYTE[lixus_req_process],cl
     ret
endp

proc Lixus_EnableSpeaker
     mov BYTE[lixus_req_speaker],cl
     ret
endp

proc Lixus_EnableWindow
     mov BYTE[lixus_req_window],cl
     ret
endp

proc Lixus_EnableTable
     mov BYTE[lixus_req_table],cl
     ret
endp

proc Lixus_EnableClip
     mov BYTE[lixus_req_clip],cl
     ret
endp

proc Lixus_EnableMouse
     mov BYTE[lixus_req_mouse],cl
     ret
endp

proc Lixus_Require state:QWORD
     mov QWORD[state],rcx
     .if BYTE[lixus_req_utils] = 1
         invoke luaL_requiref,QWORD[state],lixus_lib_utils_title,luaopen_utils,1
     .endif
     .if BYTE[lixus_req_process] = 1
         invoke luaL_requiref,QWORD[state],lixus_lib_process_title,luaopen_process,1
     .endif
     .if BYTE[lixus_req_speaker] = 1
         .if QWORD[SpVoice_initialized] = 1
             invoke luaL_requiref,QWORD[state],lixus_lib_speaker_title,luaopen_speaker,1
         .endif
     .endif
     .if BYTE[lixus_req_window] = 1
         invoke luaL_requiref,QWORD[state],lixus_lib_window_title,luaopen_window,1
     .endif
     .if BYTE[lixus_req_clip] = 1
         invoke luaL_requiref,QWORD[state],lixus_lib_clip_title,luaopen_clip,1
     .endif
     .if BYTE[lixus_req_table] = 1
         invoke lua_getglobal,QWORD[state],lixus_lib_table_title
         .if rax = LUA_TTABLE
             stdcall lua_pushsf,QWORD[state],lixus_lib_table_getn_title,lixus_table_getn_function
         .endif
     .endif
     .if BYTE[lixus_req_mouse] = 1
         invoke luaL_requiref,QWORD[state],lixus_lib_mouse_title,luaopen_mouse,1
     .endif
     ret
endp

proc Lixus_DoFile file:QWORD,err:QWORD
     local hstate:QWORD
     mov QWORD[file],rcx
     mov QWORD[err],rdx
     invoke luaL_newstate
     mov QWORD[hstate],rax
     invoke luaL_openlibs,rax
     stdcall loadcfunctions,QWORD[hstate]
     stdcall Lixus_Require,QWORD[hstate]
     stdcall luaL_dofile,QWORD[hstate],QWORD[file]
     .if rax > 0
         stdcall lua_tostring,QWORD[hstate],-1
         invoke printf,rax
     .endif
     invoke lua_close,QWORD[hstate]
     ret
endp

include './luaf.inc'

section '.idata' import data readable writeable

library msvcrt,'msvcrt.dll',\
        lua53,'lua53.dll',\
        user32,'user32.dll',\
        kernel32,'kernel32.dll',\
        ntdll,'ntdll.dll',\
        psapi,'psapi.dll',\
        ole32,'ole32.dll',\
        gdiplus,'gdiplus.dll',\
        gdi32,'gdi32.dll',\
        shlwapi,'shlwapi.dll'

import shlwapi,\
       PathFindExtension,'PathFindExtensionA',\
       ColorHLSToRGB,'ColorHLSToRGB',\
       ColorRGBToHLS,'ColorRGBToHLS'

import gdi32,\
       GetObject,'GetObjectA',\
       DeleteObject,'DeleteObject'

import gdiplus,\
       GdiplusStartup,'GdiplusStartup',\
       GdipBitmapLockBits,'GdipBitmapLockBits',\
       GdipBitmapUnlockBits,'GdipBitmapUnlockBits',\
       GdiplusShutdown,'GdiplusShutdown',\
       GdipDisposeImage,'GdipDisposeImage',\
       GdipCreateBitmapFromFile,'GdipCreateBitmapFromFile',\
       GdipCreateHBITMAPFromBitmap,'GdipCreateHBITMAPFromBitmap'

import ole32,\
       CoInitialize,'CoInitialize',\
       CoCreateInstance,'CoCreateInstance',\
       CoUninitialize,'CoUninitialize'

import user32,\
       MessageBox,'MessageBoxA',\
       SetWindowsHookEx,'SetWindowsHookExA',\
       CallNextHookEx,'CallNextHookEx',\
       UnhookWindowsHookEx,'UnhookWindowsHookEx',\
       GetMessage,'GetMessageA',\
       TranslateMessage,'TranslateMessage',\
       DispatchMessage,'DispatchMessageA',\
       EnumWindows,'EnumWindows',\
       IsWindowUnicode,'IsWindowUnicode',\
       GetWindowTextW,'GetWindowTextW',\
       GetWindowTextA,'GetWindowTextA',\
       GetWindowTextLengthA,'GetWindowTextLengthA',\
       GetWindowTextLengthW,'GetWindowTextLengthW',\
       IsWindow,'IsWindow',\
       IsWindowEnabled,'IsWindowEnabled',\
       EnumChildWindows,'EnumChildWindows',\
       SetWindowTextA,'SetWindowTextA',\
       SetWindowTextW,'SetWindowTextW',\
       GetWindowRect,'GetWindowRect',\
       MoveWindow,'MoveWindow',\
       EnableWindow,'EnableWindow',\
       SetFocus,'SetFocus',\
       ShowWindow,'ShowWindow',\
       CloseWindow,'CloseWindow',\
       DestroyWindow,'DestroyWindow',\
       SetCapture,'SetCapture',\
       ReleaseCapture,'ReleaseCapture',\
       GetCapture,'GetCapture',\
       GetActiveWindow,'GetActiveWindow',\
       IsWindowVisible,'IsWindowVisible',\
       IsIconic,'IsIconic',\
       GetWindowPlacement,'GetWindowPlacement',\
       GetTopWindow,'GetTopWindow',\
       SetActiveWindow,'SetActiveWindow',\
       GetWindowThreadProcessId,'GetWindowThreadProcessId',\
       SendMessage,'SendMessageA',\
       FindWindow,'FindWindowA',\
       GetKeyboardLayoutNameA,'GetKeyboardLayoutNameA',\
       LoadKeyboardLayoutA,'LoadKeyboardLayoutA',\
       VkKeyScanExA,'VkKeyScanExA',\
       UnloadKeyboardLayout,'UnloadKeyboardLayout',\
       SendInput,'SendInput',\
       GetSystemMetrics,'GetSystemMetrics',\
       LoadImage,'LoadImageA',\
       GetFocus,'GetFocus',\
       PostMessage,'PostMessageA',\
       GetWindow,'GetWindow',\
       OpenClipboard,'OpenClipboard',\
       GetClipboardData,'GetClipboardData',\
       SetClipboardData,'SetClipboardData',\
       CloseClipboard,'CloseClipboard',\
       EmptyClipboard,'EmptyClipboard',\
       SetProp,'SetPropA',\
       GetProp,'GetPropA',\
       RemoveProp,'RemovePropA',\
       GetCursorPos,'GetCursorPos',\
       SetCursorPos,'SetCursorPos',\
       mouse_event,'mouse_event'

import psapi,\
       GetModuleFileNameEx,'GetModuleFileNameExA'

import ntdll,\
       NtSuspendProcess,'NtSuspendProcess',\
       NtResumeProcess,'NtResumeProcess'

import kernel32,\
       Sleep,'Sleep',\
       Process32First,'Process32First',\
       CreateToolhelp32Snapshot,'CreateToolhelp32Snapshot',\
       Process32Next,'Process32Next',\
       GetShortPathName,'GetShortPathNameA',\
       CloseHandle,'CloseHandle',\
       OpenProcess,'OpenProcess',\
       TerminateProcess,'TerminateProcess',\
       GetCurrentProcessId,'GetCurrentProcessId',\
       GetModuleHandle,'GetModuleHandleA',\
       MultiByteToWideChar,'MultiByteToWideChar',\
       GetFileAttributesEx,'GetFileAttributesExA',\
       WideCharToMultiByte,'WideCharToMultiByte',\
       GlobalLock,'GlobalLock',\
       GlobalUnlock,'GlobalUnlock',\
       GlobalAlloc,'GlobalAlloc',\
       lstrcpy,'lstrcpy',\
       lstrlen,'lstrlen',\
       GetProcessHeap,'GetProcessHeap',\
       HeapAlloc,'HeapAlloc',\
       HeapFree,'HeapFree',\
       WaitForSingleObject,'WaitForSingleObject',\
       GetCurrentThreadId,'GetCurrentThreadId',\
       VirtualAllocEx,'VirtualAllocEx',\
       WriteProcessMemory,'WriteProcessMemory',\
       VirtualFreeEx,'VirtualFreeEx',\
       LoadLibrary,'LoadLibrary',\
       CreateRemoteThread,'CreateRemoteThread'

import msvcrt,\
       calloc,'calloc',\
       free,'free',\
       getchar,'getchar',\
       putchar,'putchar',\
       exit,'exit',\
       system,'system',\
       memset,'memset',\
       strlen,'strlen',\
       printf,'printf',\
       strcmp,'strcmp',\
       strtok,'strtok',\
       itoa,'itoa',\
       atoi,'atoi',\
       wcslen,'wcslen',\
       strstr,'strstr'

import lua53,\
       luaL_openlibs,'luaL_openlibs',\
       luaL_newstate,'luaL_newstate',\
       lua_close,'lua_close',\
       luaL_tolstring,'luaL_tolstring',\
       lua_pushnil,'lua_pushnil',\
       lua_pushlstring,'lua_pushlstring',\
       lua_pushlightuserdata,'lua_pushlightuserdata',\
       lua_pushinteger,'lua_pushinteger',\
       lua_pushstring,'lua_pushstring',\
       lua_pushthread,'lua_pushthread',\
       lua_pushvalue,'lua_pushvalue',\
       lua_pushboolean,'lua_pushboolean',\
       lua_stringtonumber,'lua_stringtonumber',\
       lua_toboolean,'lua_toboolean',\
       lua_tointegerx,'lua_tointegerx',\
       lua_tolstring,'lua_tolstring',\
       lua_topointer,'lua_topointer',\
       lua_tothread,'lua_tothread',\
       lua_touserdata,'lua_touserdata',\
       lua_isinteger,'lua_isinteger',\
       lua_isnumber,'lua_isnumber',\
       lua_isstring,'lua_isstring',\
       lua_isuserdata,'lua_isuserdata',\
       lua_tocfunction,'lua_tocfunction',\
       lua_setglobal,'lua_setglobal',\
       luaL_loadstring,'luaL_loadstring',\
       lua_pcallk,'lua_pcallk',\
       luaL_setfuncs,'luaL_setfuncs',\
       lua_settop,'lua_settop',\
       lua_getglobal,'lua_getglobal',\
       lua_pushcclosure,'lua_pushcclosure',\
       luaL_checkversion,'luaL_checkversion_',\
       lua_createtable,'lua_createtable',\
       lua_getfield,'lua_getfield',\
       luaL_requiref,'luaL_requiref',\
       lua_setlocal,'lua_setlocal',\
       luaL_loadfilex,'luaL_loadfilex',\
       lua_atpanic,'lua_atpanic',\
       luaL_error,'luaL_error',\
       lua_getlocal,'lua_getlocal',\
       lua_error,'lua_error',\
       luaL_checklstring,'luaL_checklstring',\
       luaL_checknumber,'luaL_checknumber',\
       lua_tonumberx,'lua_tonumberx',\
       lua_settable,'lua_settable',\
       lua_setfield,'lua_setfield',\
       lua_pushnumber,'lua_pushnumber',\
       lua_next,'lua_next',\
       lua_type,'lua_type',\
       lua_typename,'lua_typename',\
       luaL_checktype,'luaL_checktype',\
       luaL_checkinteger,'luaL_checkinteger',\
       lua_gettop,'lua_gettop',\
       lua_pushfstring,'lua_pushfstring',\
       lua_gettable,'lua_gettable'

section '.edata' export data readable

  export 'Lixus.dll',\
         Lixus_Require,'lixus_require',\
         Lixus_EnableUtils,'lixus_enableutils',\
         Lixus_EnableSpeaker,'lixus_enablespeaker',\
         Lixus_EnableMouse,'lixus_enablemouse',\
         Lixus_EnableTable,'lixus_enabletable',\
         Lixus_EnableWindow,'lixus_enablewindow',\
         Lixus_EnableProcess,'lixus_enableprocess',\
         Lixus_EnableClip,'lixus_enableclip'

section '.reloc' fixups data readable discardable

  if $=$$
    dd 0,8              ; if there are no fixups, generate dummy entry
  end if