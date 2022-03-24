 lixus_endofexecution db 'End Of Execution',0

        lixus_req_utils db 1
        lixus_req_process db 1
        lixus_req_speaker db 1
        lixus_req_window db 1
        lixus_req_clip db 1
        lixus_req_table db 1
        lixus_req_mouse db 1
        lixus_req_keyboard db 1

        lixus_lib_table_title db 'table',0
        lixus_lib_table_getn_title db 'getn',0

        lixus_invoke_token_title db '.',0

        lixus_success_function_title db 'success',0
        lixus_failure_function_title db 'failure',0
        lixus_checktype_function_title db 'checktype',0

        showMsg_abort_title db 'Abort',0
        showMsg_cancel_title db 'Cancel',0
        showMsg_continue_title db 'Continue',0
        showMsg_ignore_title db 'Ignore',0
        showMsg_no_title db 'No',0
        showMsg_ok_title db 'Ok',0
        showMsg_retry_title db 'Retry',0
        showMsg_tryagain_title db 'TryAgain',0
        showMsg_yes_title db 'Yes',0

        lixus_msgbox_defaultmsgboxtitle_title db 'Lixus',0
        lixus_msgbox_title dq lixus_msgbox_defaultmsgboxtitle_title

        lixus_lib_utils_setmsgtitle_title db 'setMsgTitle',0
        lixus_lib_utils_showmsgtitle_title db 'showMsg',0
        lixus_lib_utils_sleep_title db 'sleep',0
        lixus_lib_utils_processmessages_title db 'processMessages',0
        lixus_lib_utils_rgbtohls_title db 'rgbToHls',0
        lixus_lib_utils_hlstorgb_title db 'hlsToRgb',0

        lixus_lib_utils_rgbtohls_r_title db 'r',0
        lixus_lib_utils_rgbtohls_g_title db 'g',0
        lixus_lib_utils_rgbtohls_b_title db 'b',0
        lixus_lib_utils_rgbtohls_h_title db 'h',0
        lixus_lib_utils_rgbtohls_l_title db 'l',0
        lixus_lib_utils_rgbtohls_s_title db 's',0

        lixus_lib_utils_title db 'utils',0

        lixus_lib_utils:
                dq lixus_lib_utils_setmsgtitle_title
                dq lixus_lib_utils_setmsgtitle_function
                dq lixus_lib_utils_showmsgtitle_title
                dq lixus_lib_utils_showmsg_function
                dq lixus_lib_utils_sleep_title
                dq lixus_lib_utils_sleep_function
                dq lixus_lib_utils_processmessages_title
                dq lixus_lib_utils_processmessages_function
                dq lixus_lib_utils_rgbtohls_title
                dq lixus_lib_utils_rgbtohls_function
                dq 0
                dq 0
        lixus_lib_utils_size dq ($ - lixus_lib_utils) / 16

        lixus_lib_process_title db 'process',0
        lixus_lib_process_list_title db 'list',0
        lixus_lib_process_select_title db 'select',0

        lixus_list_name_title db 'name',0
        lixus_list_pid_title db 'pid',0
        lixus_list_parentpid_title db 'parentpid',0

        lixus_select_table_pid_title db 'pid',0
        lixus_select_table_handle_title db 'handle',0
        lixus_select_table_close_function_title db 'close',0
        lixus_select_table_kill_function_title db 'kill',0
        lixus_select_table_suspend_function_title db 'suspend',0
        lixus_select_table_resume_function_title db 'resume',0
        lixus_select_table_getpath_function_title db 'getPath',0
        lixus_select_table_wait_function_title db 'wait',0
        lixus_select_table_inject_function_title db 'inject',0

        lixus_lib_process:
                dq lixus_lib_process_list_title
                dq lixus_lib_process_list_function
                dq lixus_lib_process_select_title
                dq lixus_lib_process_select_function
                dq 0
                dq 0
        lixus_lib_process_size dq ($ - lixus_lib_process) / 16

        lixus_lib_speaker_title db 'speaker',0
        lixus_lib_speaker_speak_title db 'speak',0
        lixus_lib_speaker_wait_title db 'wait',0
        lixus_lib_speaker_volume_title db 'volume',0
        lixus_lib_speaker_pause_title db 'pause',0
        lixus_lib_speaker_resume_title db 'resume',0

        lixus_lib_speaker_speak_async_title db 'async',0
        lixus_lib_speaker_speak_isfile_title db 'isfile',0

        lixus_lib_speaker:
                dq lixus_lib_speaker_speak_title
                dq lixus_lib_speaker_speak_function
                dq lixus_lib_speaker_wait_title
                dq lixus_lib_speaker_wait_function
                dq lixus_lib_speaker_volume_title
                dq lixus_lib_speaker_volume_function
                dq lixus_lib_speaker_pause_title
                dq lixus_lib_speaker_pause_function
                dq lixus_lib_speaker_resume_title
                dq lixus_lib_speaker_resume_function
                dq 0
                dq 0
        lixus_lib_speaker_size dq ($ - lixus_lib_speaker) / 16

        lixus_lib_window_title db 'window',0
        lixus_lib_window_list_title db 'list',0
        lixus_lib_window_select_title db 'select',0

        lixus_lib_window_list_hwnd_title db 'hwnd',0
        lixus_lib_window_list_text_title db 'text',0
        lixus_lib_window_list_pid_title db 'pid',0

        lixus_lib_window_select_action_title db 'action',0
        lixus_lib_window_select_text_title db 'text',0
        lixus_lib_window_select_is_title db 'is',0
        lixus_lib_window_select_position_title db 'position',0
        lixus_lib_window_select_click_title db 'click',0
        lixus_lib_window_select_sethook_title db 'setHook',0
        lixus_lib_window_select_post_title db 'post',0

        lixus_lib_window_select_post_msg_title db 'msg',0
        lixus_lib_window_select_post_wparam_title db 'wparam',0
        lixus_lib_window_select_post_lparam_title db 'lparam',0

        lixus_lib_window_select_post_h_title db 'h',0
        lixus_lib_window_select_post_l_title db 'l',0

        lixus_lib_window_select_sethook_hookid_title db 'hookid',0
        lixus_lib_window_select_sethook_fptr_title db 'fptr',0

        lixus_lib_window_select_click_x_title db 'x',0
        lixus_lib_window_select_click_y_title db 'y',0
        lixus_lib_window_select_click_ctrl_title db 'ctrl',0
        lixus_lib_window_select_click_shift_title db 'shift',0
        lixus_lib_window_select_click_button_title db 'button',0
        lixus_lib_window_select_click_clicks_title db 'clicks',0


        lixus_lib_window_select_click_button_l_title db 'l',0
        lixus_lib_window_select_click_button_m_title db 'm',0
        lixus_lib_window_select_click_button_r_title db 'r',0
        lixus_lib_window_select_click_button_c_title db 'c',0
        lixus_lib_window_select_click_button_s_title db 's',0

        lixus_lib_window_select_position_x_title db 'x',0
        lixus_lib_window_select_position_y_title db 'y',0
        lixus_lib_window_select_position_width_title db 'width',0
        lixus_lib_window_select_position_height_title db 'heigth',0
        lixus_lib_window_select_position_repaint_title db 'repaint',0

        lixus_lib_window_select_is_enabled_title db 'enabled',0
        lixus_lib_window_select_is_disabled_title db 'disabled',0
        lixus_lib_window_select_is_focused_title db 'focused',0
        lixus_lib_window_select_is_hidden_title db 'hidden',0
        lixus_lib_window_select_is_shown_title db 'shown',0
        lixus_lib_window_select_is_actived_title db 'actived',0
        lixus_lib_window_select_is_minimized_title db 'minimized',0
        lixus_lib_window_select_is_maximized_title db 'maximized',0
        lixus_lib_window_select_is_capturing_title db 'capturing',0
        lixus_lib_window_select_is_tallest_title db 'tallest',0

        lixus_lib_window_select_action_enable_title db 'enable',0
        lixus_lib_window_select_action_disable_title db 'disable',0
        lixus_lib_window_select_action_hide_title db 'hide',0
        lixus_lib_window_select_action_show_title db 'show',0
        lixus_lib_window_select_action_minimize_title db 'minimize',0
        lixus_lib_window_select_action_maximize_title db 'maximize',0
        lixus_lib_window_select_action_close_title db 'close',0
        lixus_lib_window_select_action_restore_title db 'restore',0
        lixus_lib_window_select_action_capturemouse_title db 'capturemouse',0
        lixus_lib_window_select_action_releasemouse_title db 'releasemouse',0
        lixus_lib_window_select_action_focus_title db 'focus',0
        lixus_lib_window_select_action_activate_title db 'activate',0

        lixus_lib_window_hook_function dq 0

        lixus_lib_window:
                dq lixus_lib_window_list_title
                dq lixus_lib_window_list_function
                dq lixus_lib_window_select_title
                dq lixus_lib_window_select_function
                dq 0
                dq 0
        lixus_lib_window_size dq ($ - lixus_lib_window) / 16

        lixus_lib_clip_title db 'clip',0
        lixus_lib_clip_put_title db 'put',0
        lixus_lib_clip_get_title  db 'get',0
        lixus_lib_clip_clear_title db 'clear',0

        lixus_lib_clip:
                dq lixus_lib_clip_put_title
                dq lixus_lib_clip_put_function
                dq lixus_lib_clip_get_title
                dq lixus_lib_clip_get_function
                dq lixus_lib_clip_clear_title
                dq lixus_lib_clip_clear_function
                dq 0
                dq 0
        lixus_lib_clip_size dq ($ - lixus_lib_clip) / 16

        com_inicialized dq 0

        CLSID_SpVoice: dd 0x96749377
                       dw 0x3391
                       dw 0x11d2
                       db 0x9e
                       db 0xe3
                       db 0x00
                       db 0xc0
                       db 0x4f
                       db 0x79
                       db 0x73
                       db 0x96

        IID_ISpVoice:  dd 0x6c44df74
                       dw 0x72b9
                       dw 0x4992
                       db 0xa1
                       db 0xec
                       db 0xef
                       db 0x99
                       db 0x6e
                       db 0x04
                       db 0x22
                       db 0xd4

        SpVoice_initialized dq 0

        oVoice ISpVoice

        lixus_lib_mouse_title db 'mouse',0

        lixus_lib_mouse_move_title db 'move',0
        lixus_lib_mouse_click_title db  'click',0

        lixus_lib_mouse_move_x_title db 'x',0
        lixus_lib_mouse_move_y_title db 'y',0

        lixus_lib_mouse_click_x_title db 'x',0
        lixus_lib_mouse_click_y_title db 'y',0
        lixus_lib_mouse_click_time_title db 'time',0
        lixus_lib_mouse_click_clicks_title db 'clicks',0
        lixus_lib_mouse_click_button_title db 'button',0
        lixus_lib_mouse_click_l_title db 'l',0
        lixus_lib_mouse_click_m_title db 'm',0
        lixus_lib_mouse_click_r_title db 'r',0

        lixus_lib_mouse:
                dq lixus_lib_mouse_move_title
                dq lixus_lib_mouse_move_function
                dq lixus_lib_mouse_click_title
                dq lixus_lib_mouse_click_function
                dq 0
                dq 0
        lixus_lib_mouse_size dq ($ - lixus_lib_mouse) / 16

        lixus_lib_keyboard_title db 'keyboard',0
        lixus_lib_keyboard_sethook_title db 'setButtonHook',0

        lixus_lib_keyboard:

                dq 0
                dq 0
        lixus_lib_keyboard_size dq ($ - lixus_lib_keyboard) / 16

        GdiPlusHandle dd ?
        GdiPlusEnabled dq ?

        bmpextension db ".bmp",0

        ;desk_path db "E:\Desktop.bmp",0
        ;find_path db "E:\Find.bmp",0

        GSI GdiplusStartupInput

        pHead dq 0
        tid dq 0

        desktop_height dd ?
        desktop_width dd ?

        limit_size dq 65540.0f ; CORRECTO!