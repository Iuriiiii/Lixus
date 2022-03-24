format PE64 console

include 'win64ax.inc'

entry EntryPoint

section '.bbs' data readable writeable

        lua_state dd 0
        str_empty db 0,0,0,0,0,0,0
        lua_file  db 'E:\Prueba.lua',0

section '.data' code readable executable

proc EntryPoint

     invoke Lixus_DoFile,lua_file,0

     invoke getchar
     invoke exit,0
     ret
endp


section '.idata' import data readable writeable

library msvcrt,'msvcrt.dll',lixus,'lixus.dll'

import lixus,\
       Lixus_DoFile,'lixus_dofile'

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