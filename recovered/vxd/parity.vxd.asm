; PARITY -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 119 instructions recovered by recursive descent

Declare_Virtual_Device PARITY, 1, 0, PARITY_Control, 00008h, Undefined_Init_Order
;   DDK version 0400h   init_order 020000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x118  R-X
PARITY_Control:
        cmp    eax, 0
        je     loc_3_0000
        clc    
        ret    
loc_1_000B:
        push   eax
        in     al, 0x61
        test   al, 0x80
        je     loc_1_0026
        jmp    loc_1_0014
loc_1_0014:
        or     al, 8
        out    0x61, al
        jmp    loc_1_001A
loc_1_001A:
        and    al, 0xf7
        out    0x61, al
        jmp    loc_1_0020
loc_1_0020:
        in     al, 0x61
        test   al, 0x80
        jne    loc_1_007C
loc_1_0026:
        test   dword ptr [0x114], 0x40000    ; -> obj1:0x0114
        je     loc_1_0053
        jmp    loc_1_0034
loc_1_0034:
        in     al, 0x90
        test   al, 0x20
        je     loc_1_0049
        movzx  ecx, word ptr [0xe]           ; -> loc_2_000E
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        jmp    loc_1_0089
loc_1_0049:
        test   al, 0x40
        je     loc_1_0053
        and    al, 0xbf
        out    0x90, al
        jmp    loc_1_0053
loc_1_0053:
        mov    al, 0x80
        out    0x70, al
        jmp    loc_1_0059
loc_1_0059:
        in     al, 0x71
        jmp    loc_1_005D
loc_1_005D:
        in     al, 0x61
        jmp    loc_1_0061
loc_1_0061:
        or     al, 0xc
        out    0x61, al
        jmp    loc_1_0067
loc_1_0067:
        and    al, 0xf3
        out    0x61, al
        jmp    loc_1_006D
loc_1_006D:
        mov    al, 0xd
        out    0x70, al
        jmp    loc_1_0073
loc_1_0073:
        in     al, 0x71
        pop    eax
        jmp    dword ptr [0x110]             ; -> obj1:0x0110
loc_1_007C:
        movzx  ecx, word ptr [0xc]           ; -> loc_2_000C
        add    ecx, dword ptr [4]            ; -> loc_2_0004
loc_1_0089:
        test   dword ptr [0x114], 0x40000    ; -> obj1:0x0114
        je     loc_1_009D
        in     al, 0x90
        and    al, 0xbf
        jmp    loc_1_009B
loc_1_009B:
        out    0x90, al
loc_1_009D:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 0xe0001010
        movzx  edi, word ptr [0x10]          ; -> loc_2_0010
        add    edi, dword ptr [4]            ; -> loc_2_0004
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        cli    
loc_1_00BC:
        jmp    loc_1_00BC
        db  000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00008h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'PARITY  '                  ; DDB_Name
        dd  020000000h                  ; DDB_Init_Order
        dd  PARITY_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x6b  R-X
        db  003h, 000h, 000h, 000h
loc_2_0004:
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        pop    ds
        db  000h
loc_2_0010:
        dec    ecx
        add    byte ptr [edi], dl
loc_2_0014:
        dec    ebp
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x93
        and    byte ptr [eax + 0x61], dh
        jb     0x88
        je     0x9a
        and    byte ptr [ebp + 0x72], ah
        jb     0x95
        jb     0x48
        je     0x91
        arpl   word ptr [ebp + 0x64], si
        add    byte ptr cs:[eax], al
        dec    ebp
        imul   esp, dword ptr [ebx + 0x72], 0x6168436f
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        insb   byte ptr es:[edi], dx
        and    byte ptr [edx + 0x75], ah
        jae    0x64
        je     0xaf
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr gs:[esi]
        jne    0xbf
        and    byte ptr [ebp + 0x72], ah
        jb     0xbf
        jb     0x72
        je     0xbb
        arpl   word ptr [ebp + 0x64], si
        add    byte ptr cs:[eax], al
        push   ebx
        jns    0xd3
        je     0xc7
        insd   dword ptr es:[edi], dx
        and    byte ptr [ebp + 0x72], al
        jb     0xd7
        jb     0x6a
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x28  R-X
loc_3_0000:
        VMMCall  VMM.Get_Machine_Info            ; cd20bb000100
        mov    dword ptr [0x114], ebx        ; -> obj1:0x0114
        VMMCall  VMM.Get_NMI_Handler_Addr        ; cd2016000100
        mov    dword ptr [0x110], esi        ; -> obj1:0x0110
        mov    esi, 0xb                      ; -> loc_1_000B
        VMMCall  VMM.Set_NMI_Handler_Addr        ; cd2017000100
        clc    
        ret    
        db  023h, 050h, 041h
VxD__LTEXT ENDS
