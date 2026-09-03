; VSD -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 156 instructions recovered by recursive descent

Declare_Virtual_Device VSD, 2, 0, VSD_Control, 0000Bh, Undefined_Init_Order
;   DDK version 0400h   init_order 02C000000h   flags 00000h
;   service table: obj1:0x1b0, 4 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x220  R-X
VSD_Control:
        cmp    eax, 1
        je     loc_2_0000
        cmp    eax, 0xd
        je     loc_1_001A
        cmp    eax, 0xb
        je     sub_1_0032
        cmp    eax, 6
        je     loc_1_003A
        clc    
        ret    
loc_1_001A:
        cmp    ebx, dword ptr [0x214]        ; -> obj1:0x0214
        jne    loc_1_0030
        call   sub_1_0032
        add    ebx, dword ptr [0x210]        ; -> obj1:0x0210
        mov    byte ptr [ebx], 2
loc_1_0030:
        clc    
        ret    
sub_1_0032:
        cmp    dword ptr [0x214], ebx        ; -> obj1:0x0214
        jne    loc_1_0051
loc_1_003A:
        in     al, 0x61
        and    al, 0xfe
        jmp    loc_1_0040
loc_1_0040:
        out    0x61, al
        mov    dword ptr [0x214], 0          ; -> obj1:0x0214
        call   sub_1_00E4
loc_1_0051:
        clc    
        ret    
loc_1_0053:
        mov    esi, dword ptr [0x210]        ; -> obj1:0x0210
        mov    edx, eax
        in     al, 0x61
        and    al, 3
        cmp    al, 3
        je     loc_1_0098
        cmp    dword ptr [0x214], 0          ; -> obj1:0x0214
        jne    loc_1_0073
        cmp    byte ptr [esi + ebx], 2
        je     loc_1_0085
loc_1_0072:
        ret    
loc_1_0073:
        call   sub_1_00E4
        xor    eax, eax
        xchg   dword ptr [0x214], eax        ; -> obj1:0x0214
        mov    byte ptr [esi + eax], 0
        ret    
loc_1_0085:
        in     al, 0x61
        or     al, 3
        jmp    loc_1_008B
loc_1_008B:
        out    0x61, al
        mov    dword ptr [0x214], ebx        ; -> obj1:0x0214
        mov    byte ptr [esi + ebx], 1
        ret    
loc_1_0098:
        cmp    byte ptr [0x21c], 0           ; -> obj1:0x021C
        jne    loc_1_0072
        cmp    dword ptr [0x214], 0          ; -> obj1:0x0214
        jne    loc_1_00CB
        mov    dword ptr [0x214], edx        ; -> obj1:0x0214
        mov    byte ptr [esi + edx], 1
loc_1_00B4:
        mov    esi, 0xf5                     ; -> loc_1_00F5
        mov    eax, 0x9c4
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x218], esi        ; -> obj1:0x0218
        ret    
loc_1_00CB:
        cmp    dword ptr [0x214], ebx        ; -> obj1:0x0214
        jne    loc_1_00D9
        call   sub_1_00E4
        ret    
loc_1_00D9:
        cmp    dword ptr [0x214], edx        ; -> obj1:0x0214
        je     loc_1_00B4
        ret    
        db  08Bh, 0C0h
sub_1_00E4:
        push   esi
        xor    esi, esi
        xchg   dword ptr [0x218], esi        ; -> obj1:0x0218
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        ret    
loc_1_00F5:
        xor    ebx, ebx
        mov    dword ptr [0x218], ebx        ; -> obj1:0x0218
        in     al, 0x61
        and    al, 0xfc
        jmp    loc_1_0103
loc_1_0103:
        out    0x61, al
        xchg   dword ptr [0x214], ebx        ; -> obj1:0x0214
        add    ebx, dword ptr [0x210]        ; -> obj1:0x0210
        mov    byte ptr [ebx], 2
        ret    
VSD_Get_Version:
        mov    eax, 0x300
        clc    
        ret    
VSD_Bell:
        push   eax
        push   esi
        mov    ax, 0x538
        out    0x42, al
        mov    al, ah
        jmp    loc_1_0128
loc_1_0128:
        out    0x42, al
        jmp    loc_1_012C
loc_1_012C:
        in     al, 0x61
        or     al, 3
        jmp    loc_1_0132
loc_1_0132:
        out    0x61, al
        mov    eax, 0x1f4
        mov    esi, 0x14e                    ; -> loc_1_014E
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    byte ptr [0x21c], 0xff        ; -> obj1:0x021C
        pop    esi
        pop    eax
        ret    
loc_1_014E:
        mov    byte ptr [0x21c], 0           ; -> obj1:0x021C
        in     al, 0x61
        and    al, 0xfc
        jmp    loc_1_015B
loc_1_015B:
        out    0x61, al
        ret    
loc_1_015E:
        mov    byte ptr [0x21d], 4           ; -> loc_1_021D
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097800100
VSD_SoundOn:
        ret    
loc_1_016C:
        cmp    ecx, 4
        jbe    loc_1_0177
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0177:
        je     0x183
        in     al, dx
        dec    byte ptr [0x21d]              ; -> loc_1_021D
        jns    loc_1_01A8
        cmp    al, 0xee
        push   eax
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        and    al, 3
        cmp    al, 3
        jne    loc_1_0197
        VxDCall  VSD.VSD_SoundOn                 ; cd2002000b00
loc_1_0197:
        mov    eax, 0x3e8
        mov    esi, 0x15e                    ; -> loc_1_015E
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        pop    eax
loc_1_01A8:
        ret    
VSD_TakeSoundPort:
        mov    byte ptr [0x21c], al          ; -> obj1:0x021C
        clc    
        ret    
        dd  VSD_Get_Version          ; fixup
        dd  VSD_Bell                 ; fixup
        dd  VSD_SoundOn              ; fixup
        dd  VSD_TakeSoundPort        ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0000Bh                      ; DDB_Req_Device_Number
        db  2, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VSD     '                  ; DDB_Name
        dd  02C000000h                  ; DDB_Init_Order
        dd  VSD_Control              ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x01B0              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h
loc_1_021D:
        add    al, 0
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x44  R-X
loc_2_0000:
        mov    esi, 0x53                     ; -> loc_1_0053
        VMMCall  VMM.Call_When_Task_Switched     ; cd202a000100
        push   0
        push   1
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    loc_2_0021
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_2_0021:
        mov    dword ptr [0x210], eax        ; -> obj1:0x0210
        mov    esi, 0x16c                    ; -> loc_1_016C
        mov    edx, 0x61
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0x61
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        clc    
        ret    
        db  020h
VxD__LTEXT ENDS
