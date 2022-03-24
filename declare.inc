SM_CXSCREEN = 0
        SM_CYSCREEN = 1

        struct WIN32_FILE_ATTRIBUTE_DATA
               dwFileAttributes dd ?
               ftCreationTime FILETIME
               ftLastAccessTime FILETIME
               ftLastWriteTime FILETIME
               nFileSizeHigh dd ?
               nFileSizeLow dd ?
        ends

        struct GdiplusStartupInput
               GdiplusVersion dd ?
               DebugEventCallback dq ?
               SuppressBackgroundThread db ?
               SuppressExternalCodecs db ?
               GdiplusStartupInput dq ?
        ends

        Alpha                   = 262144
        Canonical               = 2097152
        DontCare                = 0
        Extended                = 1048576
        Format16bppArgb1555     = 397319
        Format16bppGrayScale    = 1052676
        Format16bppRgb555       = 135173
        Format16bppRgb565       = 135174
        Format1bppIndexed       = 196865
        Format24bppRgb          = 137224
        Format32bppArgb         = 2498570
        Format32bppPArgb        = 925707
        Format32bppRgb          = 139273
        Format48bppRgb          = 1060876
        Format4bppIndexed       = 197634
        Format64bppArgb         = 3424269
        Format64bppPArgb        = 1851406
        Format8bppIndexed       = 198659
        Gdi                     = 131072
        Indexed                 = 65536
        Max                     = 15
        PAlpha                  = 524288
        Undefined               = 0

        struct BitmapData
               Width dd ?
               Height dd ?
               Stride dd ?
               PixelFormat dd ?
               Scan0 dq ?
               Reserved dq ?
        ends

        TH32CS_SNAPPROCESS equ 0x00000002

        SPF_DEFAULT             = 0
        SPF_ASYNC               = ( 1 shl 0 )
        SPF_PURGEBEFORESPEAK    = ( 1 shl 1 )
        SPF_IS_FILENAME         = ( 1 shl 2 )
        SPF_IS_XML              = ( 1 shl 3 )
        SPF_IS_NOT_XML          = ( 1 shl 4 )
        SPF_PERSIST_XML         = ( 1 shl 5 )
        SPF_NLP_SPEAK_PUNC      = ( 1 shl 6 )
        SPF_PARSE_SAPI          = ( 1 shl 7 )
        SPF_PARSE_SSML          = ( 1 shl 8 )
        SPF_PARSE_AUTODETECT    = 0
        SPF_NLP_MASK            = SPF_NLP_SPEAK_PUNC
        SPF_PARSE_MASK          = ( SPF_PARSE_SAPI or SPF_PARSE_SSML )
        SPF_VOICE_MASK          = ( ( ( ( ( ( ( SPF_ASYNC or SPF_PURGEBEFORESPEAK )  or SPF_IS_FILENAME )  or SPF_IS_XML )  or SPF_IS_NOT_XML )  or SPF_NLP_MASK )  or SPF_PERSIST_XML )  or SPF_PARSE_MASK )
        SPF_UNUSED_FLAGS        = -SPF_VOICE_MASK

        struc GUID def
        {
                match d1-d2-d3-d4-d5, def
                \{
                        .Data1 dd 0x\#d1
                        .Data2 dw 0x\#d2
                        .Data3 dw 0x\#d3
                        .Data4 db 0x\#d4 shr 8,0x\#d4 and 0FFh
                        .Data5 db 0x\#d5 shr 40,0x\#d5 shr 32 and 0FFh,0x\#d5 shr 24 and 0FFh,0x\#d5 shr 16 and 0FFh,0x\#d5 shr 8 and 0FFh,0x\#d5 and 0FFh
                \}
         }

        CLSCTX_INPROC_SERVER        = 0x1
        CLSCTX_INPROC_HANDLER       = 0x2
        CLSCTX_LOCAL_SERVER         = 0x4
        CLSCTX_INPROC_SERVER16      = 0x8
        CLSCTX_REMOTE_SERVER        = 0x10
        CLSCTX_INPROC_HANDLER16     = 0x20
        CLSCTX_INPROC_SERVERX86     = 0x40
        CLSCTX_INPROC_HANDLERX86    = 0x80
        CLSCTX_ESERVER_HANDLER      = 0x100
        CLSCTX_NO_CODE_DOWNLOAD     = 0x400
        CLSCTX_NO_CUSTOM_MARSHAL    = 0x1000
        CLSCTX_ENABLE_CODE_DOWNLOAD = 0x2000
        CLSCTX_NO_FAILURE_LOG       = 0x4000
        CLSCTX_DISABLE_AAA          = 0x8000
        CLSCTX_ENABLE_AAA           = 0x10000
        CLSCTX_FROM_DEFAULT_CONTEXT = 0x20000
        CLSCTX_ALL = (CLSCTX_INPROC_SERVER or CLSCTX_INPROC_HANDLER or CLSCTX_LOCAL_SERVER)

     IMAGE_BITMAP = 0
     IMAGE_CURSOR = 2
     IMAGE_ICON   = 1
     LR_CREATEDIBSECTION = 0x00002000
     LR_DEFAULTCOLOR = 0x00000000
     LR_DEFAULTSIZE = 0x00000040
     LR_LOADFROMFILE = 0x00000010
     LR_LOADMAP3DCOLORS = 0x00001000
     LR_LOADTRANSPARENT = 0x00000020
     LR_MONOCHROME = 0x00000001
     LR_SHARED = 0x00008000
     LR_VGACOLOR = 0x00000080

        struct FLOATING_SAVE_AREA
                ControlWord dd ?
                StatusWord dd ?
                TagWord dd ?
                ErrorOffset dd ?
                ErrorSelector dd ?
                DataOffset dd ?
                DataSelector dd ?
                RegisterArea rb 80
                Cr0NpxState dd ?
        ends

        struct CONTEXT
                ContextFlags dd ?
                Dr0 dd ?
                Dr1 dd ?
                Dr2 dd ?
                Dr3 dd ?
                Dr6 dd ?
                Dr7 dd ?
                FloatSave FLOATING_SAVE_AREA
                SegGs dd ?
                SegFs dd ?
                SegEs dd ?
                SegDs dd ?
                Edi? dd ?
                Esi? dd ?
                Ebx? dd ?
                Edx? dd ?
                Ecx? dd ?
                Eax? dd ?
                Ebp? dd ?
                Eip? dd ?
                SegCs dd ?
                EFlags dd ?
                Esp? dd ?
                SegSs dd ?
                ExtendedRegisters rb 512
        ends

        struct THREADENTRY32
                dwSize dd ?
                cntUsage dd ?
                th32ThreadID dd ?
                th32OwnerProcessID dd ?
                tpBasePri dd ?
                tpDeltaPri dd ?
                dwFlags dd ?
        ends

        ;struct PROCESSENTRY32
        ;       dwSize dd ?
        ;       cntUsage dd ?
        ;       th32ProcessID dd ?
        ;       th32DefaultHeapID dd ?
        ;       th32ModuleID dd ?
        ;       cntThreads dd ?
        ;       th32ParentProcessID dd ?
        ;       pcPriClassBase dd ?
        ;       dwFlags dd ?
        ;       szExeFile rb MAX_PATH
        ;ends

        ;TH32CS_SNAPPROCESS = 2
        TH32CS_SNAPTHREAD = 4
        INVALID_HANDLE_VALUE = (-1)
        SEEK_END = 2
        INFINITE = (-1)
        CONTEXT_i386 = 0x10000
        CONTEXT_CONTROL = (CONTEXT_i386 or 0x00000001)
        STANDARD_RIGHTS_REQUIRED = 0xF0000
        SYNCHRONIZE = 0x100000
        THREAD_GET_CONTEXT = 0x0008
        THREAD_SET_CONTEXT = (0x0010)
        THREAD_QUERY_INFORMATION = 64
        THREAD_SUSPEND_RESUME = 0x0002
        THREAD_ALL_ACCESS = STANDARD_RIGHTS_REQUIRED or SYNCHRONIZE or 0x3FF
        SE_KERNEL_OBJECT = 6
        SHELLCODE_SIZE = 22



        struct HARDWAREINPUT
               uMsg dd ?
               wParamL dw ?
               wParamH dw ?
        ends

        struct KEYBDINPUT
               wVk dw ?
               wScan dw ?
               dwFlags dd ?
               time dd ?
               dwExtraInfo dq ?
        ends

        struct MOUSEINPUT
               dx dq ?
               dy dq ?
               mouseData dd ?
               dwFlags dd ?
               time dd ?
               dwExtraInfo dq ?
        ends

        INPUT_MOUSE = 0
        INPUT_KEYBOARD = 1
        INPUT_HARDWARE = 2

        MOUSEEVENTF_ABSOLUTE = 0x8000
        MOUSEEVENTF_HWHEEL = 0x01000
        MOUSEEVENTF_MOVE = 0x0001
        MOUSEEVENTF_MOVE_NOCOALESCE = 0x2000
        MOUSEEVENTF_LEFTDOWN = 0x0002
        MOUSEEVENTF_LEFTUP = 0x0004
        MOUSEEVENTF_RIGHTDOWN = 0x0008
        MOUSEEVENTF_RIGHTUP = 0x0010
        MOUSEEVENTF_MIDDLEDOWN = 0x0020
        MOUSEEVENTF_MIDDLEUP = 0x0040
        MOUSEEVENTF_VIRTUALDESK = 0x4000
        MOUSEEVENTF_WHEEL = 0x0800
        MOUSEEVENTF_XDOWN = 0x0080
        MOUSEEVENTF_XUP = 0x0100

        struct INPUT
               type dd ?
               union
                    mi MOUSEINPUT
                    ki KEYBDINPUT
                    hi HARDWAREINPUT
               ends
        ends

        struct CWPSTRUCT
               lParam dq ?
               wParam dq ?
               message dd ?
               hwnd dq ?
        ends

        struct PROCESSENTRY32
               dwSize dd ?
               cntUsage dd ?
               th32ProcessID dd ?
               th32DefaultHeapID dq ?
               th32ModuleID dd ?
               cntThreads dd ?
               th32ParentProcessID dd ?
               pcPriClassBase dq ?
               dwFlags dd ?
               szExeFile rb MAX_PATH
        ends

        struct MSLLHOOKSTRUCT
               pt POINT ?
               mouseData dd ?
               flags dd ?
               time dd ?
               dwExtraInfo dq ?
        ends

        PROCESSENTRY32_SIZE equ (9 * 8) + MAX_PATH

        ;https://docs.microsoft.com/en-us/previous-versions/office/developer/speech-technologies/jj127823(v%3Dmsdn.10)
        ;https://gist.github.com/Youka/365dc146c14d10ea9659
        interface ISpVoice,\
                  QueryInterface,\
                  AddRef,\
                  Release,\
                  SetNotifySink,\
                  SetNotifyWindowMessage,\
                  SetNotifyCallbackFunction,\
                  SetNotifyCallbackInterface,\
                  SetNotifyWin32Event,\
                  WaitForNotifyEvent,\
                  GetNotifyEventHandle,\
                  SetInterest,\
                  GetEvents,\
                  GetInfo,\
                  SetOutput,\
                  GetOutputObjectToken,\
                  GetOutputStream,\
                  Pause,\
                  Resume,\
                  SetVoice,\
                  GetVoice,\
                  Speak,\
                  SpeakStream,\
                  GetStatus,\
                  Skip,\
                  SetPriority,\
                  GetPriority,\
                  SetAlertBoundary,\
                  GetAlertBoundary,\
                  SetRate,\
                  GetRate,\
                  SetVolume,\
                  GetVolume,\
                  WaitUntilDone,\
                  SetSyncSpeakTimeout,\
                  GetSyncSpeakTimeout,\
                  IsUISupported,\
                  DisplayUI