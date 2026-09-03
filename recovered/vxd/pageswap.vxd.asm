; PAGESWAP -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 50 instructions recovered by recursive descent

Declare_Virtual_Device PAGESWAP, 2, 10, PAGESWAP_Control, 00007h, Undefined_Init_Order
;   DDK version 0400h   init_order 01C000000h   flags 00000h
;   service table: obj1:0x58, 10 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0xd8  R-X
PAGESWAP_Control:
        cmp    eax, 5
        je     loc_3_0000
        clc    
        ret    
PageSwap_Get_Version:
        mov    bl, byte ptr [0xd4]           ; -> obj1:0x00D4
        mov    eax, 0x400
        mov    ecx, dword ptr [0xd0]         ; -> obj1:0x00D0
        clc    
        ret    
PageSwap_Test_IO_Valid:
        cmp    byte ptr [0xd4], 1            ; -> obj1:0x00D4
        jbe    loc_1_002D
        VxDCall  PAGEFILE.PageFile_Test_IO_Valid ; cd2006802100
loc_1_002D:
        clc    
        ret    
PageSwap_Read_Or_Write:
        enter  0x10, 0
        push   ebx
        lea    ebx, [ebp - 0x10]
        mov    byte ptr [ebx + 6], 0xff
        mov    byte ptr [ebx + 4], ch
        mov    dword ptr [ebx + 8], esi
        mov    dword ptr [ebx + 0xc], edi
        mov    byte ptr [ebx + 7], al
        VxDCall  PAGEFILE.PageFile_Read_Or_Write ; cd2004002100
        movzx  eax, byte ptr [ebx + 6]
        dec    eax
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h
        dd  PageSwap_Get_Version     ; fixup
        dd  PageSwap_Invalid_Service1 ; fixup
        dd  PageSwap_Invalid_Service1 ; fixup
        dd  PageSwap_Invalid_Service1 ; fixup
        dd  PageSwap_Invalid_Service1 ; fixup
        dd  PageSwap_Invalid_Service1 ; fixup
        dd  PageSwap_Test_IO_Valid   ; fixup
        dd  PageSwap_Read_Or_Write   ; fixup
        dd  PageSwap_Grow_File       ; fixup
        dd  PageSwap_Init_File       ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00007h                      ; DDB_Req_Device_Number
        db  2, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'PAGESWAP'                  ; DDB_Name
        dd  01C000000h                  ; DDB_Init_Order
        dd  PAGESWAP_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x0058              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x28  R-X
PageSwap_Init_File:
        VxDCall  PAGEFILE.PageFile_Get_Version   ; cd2000002100
        jb     loc_2_0027
        VxDCall  PAGEFILE.PageFile_Init_File     ; cd2001002100
        test   ebx, ebx
        je     loc_2_0027
        mov    dword ptr [0xd0], ebx         ; -> obj1:0x00D0
        push   eax
        VxDCall  PAGEFILE.PageFile_Get_Version   ; cd2000002100
        pop    eax
        mov    byte ptr [0xd4], bl           ; -> obj1:0x00D4
        clc    
loc_2_0027:
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x20  R-X
loc_3_0000:
        cmp    dword ptr [0xd0], 0           ; -> obj1:0x00D0
        je     loc_3_000F
        VxDCall  PAGEFILE.PageFile_Clean_Up      ; cd2002002100
loc_3_000F:
        clc    
        ret    
PageSwap_Invalid_Service1:
        stc    
        ret    
PageSwap_Grow_File:
        VxDCall  PAGEFILE.PageFile_Grow_File     ; cd2003002100
        mov    dword ptr [0xd0], ebx         ; -> obj1:0x00D0
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x1a  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 00Bh, 066h, 033h, 0D2h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h
        db  0C3h, 033h, 0DBh, 033h, 0F6h, 0B8h, 001h, 080h, 0C3h, 025h
VxD__LTEXT ENDS
