; REBOOT -- reconstructed 32-bit disassembly (VxD / LE format)
; 6 objects, 630 instructions recovered by recursive descent

Declare_Virtual_Device REBOOT, 4, 0, REBOOT_Control, 00009h, Undefined_Init_Order
;   DDK version 0400h   init_order 024000000h   flags 00000h
;   service table: obj1:0x258, 4 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x2f0  R-X
REBOOT_Control:
        cmp    eax, 1
        je     loc_3_0000
        cmp    eax, 2
        je     loc_3_00A4
        cmp    eax, 7
        je     loc_5_0000
        cmp    eax, 0xc
        je     loc_6_0000
        cmp    eax, 0x23
        je     loc_4_0097
        cmp    eax, 4
        jne    loc_1_0039
        or     byte ptr [0x2e4], 0x20        ; -> obj1:0x02E4
loc_1_0039:
        clc    
        ret    
loc_1_003B:
        test   byte ptr [0x2e4], 0x80        ; -> obj1:0x02E4
        jne    loc_1_0101
        test   byte ptr [0x2e4], 0x31        ; -> obj1:0x02E4
        jne    REBOOT_Service_03
        mov    al, byte ptr [0x2e6]          ; -> obj1:0x02E6
        cmp    al, 3
        je     REBOOT_Service_03
        cmp    al, 1
        je     loc_1_0102
        cmp    al, 2
        je     loc_1_0102
        cmp    al, 0
        jne    loc_1_0101
        cmp    dword ptr [0x2d8], 0          ; -> obj1:0x02D8
        je     REBOOT_Service_03
        movzx  eax, byte ptr [0x2e7]         ; -> loc_1_02E7
        push   0
        push   0
        push   0x2e8                         ; -> obj1:0x02E8
        push   eax
        VxDCall  IFSMgr.IFSMgr_GetLockState      ; cd206f004000
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_00B2
        cmp    dword ptr [0x2e8], 2          ; -> obj1:0x02E8
        jge    loc_1_0152
loc_1_00B2:
        mov    byte ptr [0x2e6], 1           ; -> obj1:0x02E6
        mov    eax, 0x3a98
        mov    esi, 0x13f                    ; -> loc_1_013F
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x2e0], esi        ; -> obj1:0x02E0
        or     byte ptr [0x2e5], 1           ; -> obj1:0x02E5
        mov    edi, dword ptr [0x2dc]        ; -> obj1:0x02DC
        mov    eax, 0x400000
        VMMCall  VMM.Adjust_Thread_Exec_Priority ; cd2014010100
        push   edi
        push   0
        push   dword ptr [0x2d8]             ; -> obj1:0x02D8
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
        push   edi
        VMMCall  VMM._SignalID                   ; cd2027010100
        pop    ecx
loc_1_0101:
        ret    
loc_1_0102:
        mov    byte ptr [0x2e6], 8           ; -> obj1:0x02E6
        mov    eax, 0xc0001010
        mov    ecx, 5
        movzx  edi, word ptr [0x12]          ; -> loc_2_0012
        add    edi, dword ptr [4]            ; -> loc_2_0004
        call   sub_1_01D8
        cmp    dword ptr [0x2e0], 0          ; -> obj1:0x02E0
        je     loc_1_0152
        cmp    byte ptr [0x2e6], 8           ; -> obj1:0x02E6
        jne    loc_1_0101
        mov    byte ptr [0x2e6], 1           ; -> obj1:0x02E6
        ret    
loc_1_013F:
        mov    dword ptr [0x2e0], 0          ; -> obj1:0x02E0
        cmp    byte ptr [0x2e6], 8           ; -> obj1:0x02E6
        je     loc_1_016E
loc_1_0152:
        mov    eax, 0xc0001010
        mov    ecx, 3
        movzx  edi, word ptr [0xe]           ; -> loc_2_000E
        add    edi, dword ptr [4]            ; -> loc_2_0004
        call   sub_1_01D8
loc_1_016E:
        ret    
REBOOT_Service_03:
        push   5
        pop    ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        bts    word ptr [0x2e4], 1           ; -> obj1:0x02E4
        jb     loc_1_018E
        mov    eax, 0x17
        VMMCall  VMM.System_Control              ; cd2093000100
loc_1_018E:
        cli    
        bts    word ptr [0x2e4], 2           ; -> obj1:0x02E4
        jb     loc_1_01A5
        mov    eax, 0x18
        VMMCall  VMM.System_Control              ; cd2093000100
loc_1_01A5:
        bts    word ptr [0x2e4], 3           ; -> obj1:0x02E4
        jb     loc_1_01BB
        mov    eax, 0x12
        VMMCall  VMM.System_Control              ; cd2093000100
loc_1_01BB:
        push   0x80001
        push   0                             ; -> obj2:0x0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        mov    esi, eax
        mov    eax, 1
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
sub_1_01D8:
        or     byte ptr [0x2e4], 0x10        ; -> obj1:0x02E4
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xchg   ecx, eax
        add    eax, 0x80000
        push   eax
        push   0                             ; -> obj2:0x0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        xchg   ecx, eax
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        add    esp, dword ptr [esp]
        and    byte ptr [0x2e4], 0xef        ; -> obj1:0x02E4
        ret    
REBOOT_Service_00:
        xor    eax, eax
        mov    ah, 4
        ret    
REBOOT_Service_01:
        or     byte ptr [0x2e4], 0x10        ; -> obj1:0x02E4
        ret    
REBOOT_Service_02:
        and    byte ptr [0x2e4], 0xef        ; -> obj1:0x02E4
        ret    
        db  000h, 000h
loc_1_0220:
        mov    eax, dword ptr [edi + 8]
        and    eax, 0x47
        cmp    eax, 5
        jne    loc_1_0256
        mov    eax, dword ptr [edi + 4]
        sub    eax, 0xff
loc_1_0233:
        je     loc_4_00C8
        test   eax, 0x3ff
        jne    loc_1_0256
        shl    eax, 0xc
loc_1_0243:
        cmp    eax, dword ptr [ebx + 4]
        je     loc_1_0233
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_1_0243
loc_1_0256:
        stc    
        ret    
        dd  REBOOT_Service_00        ; fixup
        dd  REBOOT_Service_01        ; fixup
        dd  REBOOT_Service_02        ; fixup
        dd  REBOOT_Service_03        ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00009h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'REBOOT  '                  ; DDB_Name
        dd  024000000h                  ; DDB_Init_Order
        dd  REBOOT_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  REBOOT_PM_API            ; DDB_PM_API_Proc
        dd  obj1:0x0258              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_4_0030               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_0045               ; fixup
        db  002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_02E7:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x2e9  R-X
        db  005h, 000h, 000h, 000h
loc_2_0004:
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_000E:
        xor    al, 0
        db  03Eh, 000h
loc_2_0012:
        jns    0x15
        mov    al, byte ptr [ecx]
        rol    dword ptr [edx], 1
loc_2_0018:
        push   eax
        jb     loc_2_0080
        jae    0x90
        and    byte ptr [ebx + 0x54], al
        push   edx
        dec    esp
        sub    eax, dword ptr [ecx + 0x4c]
        push   esp
        sub    eax, dword ptr [ebp + eax*2 + 0x4c]
        and    byte ptr [ecx + 0x67], ah
        popal  
        imul   ebp, dword ptr [esi + 0x20], 0x72206f74
        jae    loc_2_00AC
        popal  
        jb     0xaf
        and    byte ptr [ecx + 0x6f], bh
        jne    0xb2
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        jo     0xbb
        je     0xad
        jb     0x6a
        add    byte ptr [eax], al
        push   edi
        inc    ecx
        push   edx
        dec    esi
        dec    ecx
        dec    esi
        inc    edi
        and    dword ptr [eax], eax
        add    byte ptr [eax + ebp*2 + 0x65], dl
        and    byte ptr [ebx + 0x79], dh
        jae    0xd2
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ebp + 0x69], ah
        je     0xd0
        jb     0x8b
        bound  esi, qword ptr [ebp + 0x73]
        jns    0x90
        outsd  dx, dword ptr [esi]
        jb     0x93
        push   0x62207361
        arpl   word ptr gs:[edi + 0x6d], bp
        and    byte ptr gs:[ebp + 0x6e], dh
loc_2_0080:
        jae    loc_2_00F6
        popal  
        bound  ebp, qword ptr [ebp + 0x2e]
        and    byte ptr [ecx + 0x6f], bl
        jne    loc_2_00AC
        arpl   word ptr [ecx + 0x6e], sp
        and    byte ptr [edi + 0x61], dh
        imul   esi, dword ptr [eax + 0x61], 0x7320646e
        and    byte ptr gs:[ecx + 0x66], ch
        and    byte ptr [ecx + 0x74], ch
        and    byte ptr [edx + 0x65], ah
        arpl   word ptr [edi + 0x6d], bp
        jae    loc_2_00CB
        popal  
loc_2_00AC:
        jbe    0x10f
        imul   ebp, dword ptr [ecx + 0x62], 0x6120656c
        popal  
        imul   ebp, dword ptr [esi + 0x2c], 0x20726f20
        jns    0x130
        jne    0xe3
        arpl   word ptr [ecx + 0x6e], sp
        and    byte ptr [edx + 0x65], dh
        jae    loc_2_013F
loc_2_00CB:
        popal  
        jb     loc_2_0142
        and    byte ptr [ecx + 0x6f], bh
        jne    0x145
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        jo     loc_2_014E
        je     loc_2_0140
        jb     0x10b
        or     eax, 0x202a0a0a
        and    byte ptr [eax + 0x72], dl
        jae    0x15b
        and    byte ptr [ecx + 0x6e], ah
        jns    0x10d
        imul   esp, dword ptr [ebp + 0x79], 0x20
        je     0x162
        and    byte ptr [edx + 0x65], dh
loc_2_00F6:
        je     loc_2_016D
        jb     loc_2_0168
        and    byte ptr [edi + ebp*2 + 0x20], dh
        push   edi
        imul   ebp, dword ptr [esi + 0x64], 0x2073776f
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[edi + 0x61], dh
        imul   esi, dword ptr [esi + ebp + 0xd], 0x20202a0a
        push   eax
        jb     loc_2_017C
        jae    0x18c
        and    byte ptr [ebx + 0x54], al
        push   edx
        dec    esp
        sub    eax, dword ptr [ecx + 0x4c]
        push   esp
        sub    eax, dword ptr [ebp + eax*2 + 0x4c]
        and    byte ptr [ecx + 0x67], ah
        popal  
        imul   ebp, dword ptr [esi + 0x20], 0x72206f74
        jae    loc_2_01A8
        popal  
        jb     0x1ab
        and    byte ptr [ecx + 0x6f], bh
        jne    0x1ae
        and    byte ptr [ebx + 0x6f], ah
loc_2_013F:
        insd   dword ptr es:[edi], dx
loc_2_0140:
        jo     loc_2_01B7
loc_2_0142:
        je     0x1a9
        jb     0x174
        and    byte ptr [ecx + 0x6f], bl
        jne    loc_2_016B
        ja     0x1b6
        insb   byte ptr es:[edi], dx
loc_2_014E:
        insb   byte ptr es:[edi], dx
        or     eax, 0x2020080a
        and    byte ptr [edi + ebp*2 + 0x73], ch
        and    byte ptr gs:[ebp + 0x6e], dh
        jae    loc_2_01BF
        jbe    0x1c5
        and    byte ptr fs:[ecx + 0x6e], ch
        outsw  dx, word ptr [esi]
        jb     0x1d5
loc_2_0168:
        popal  
        je     0x1d4
loc_2_016B:
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_2_016D:
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [ecx + 0x6e], ah
        jns    0x195
        jo     loc_2_01E9
        outsd  dx, dword ptr [esi]
        jb     loc_2_01DC
        insd   dword ptr es:[edi], dx
loc_2_017C:
        jae    0x19e
        je     0x1e8
        popal  
        je     loc_2_01A3
        popal  
        jb     0x1eb
        and    byte ptr [edx + 0x75], dh
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   ebp, dword ptr [esi + 0x67], 0x5300002e
        jns    loc_2_0207
        je     0x1fb
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [edx + 0x75], ah
        jae    0x218
        add    byte ptr cs:[eax], al
        push   esp
loc_2_01A3:
        push   0x79732065
loc_2_01A8:
        jae    loc_2_021E
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [edx + 0x75], ah
        jae    0x22d
        and    byte ptr [edi + 0x61], dh
loc_2_01B7:
        imul   esi, dword ptr [ecx + ebp*2 + 0x6e], 0x6f662067
loc_2_01BF:
        jb     0x1e1
        je     loc_2_022B
        and    byte ptr gs:[ebx + 0x6c], al
        outsd  dx, dword ptr [esi]
        jae    loc_2_022F
        and    byte ptr [eax + 0x72], dl
        outsd  dx, dword ptr [esi]
        jb     0x232
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + ebp*2 + 0x61], ah
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        and    byte ptr [bp + si + 0x6f], ah
loc_2_01DC:
        js     0x1fe
        je     0x24f
        and    byte ptr [edx + 0x65], ah
        and    byte ptr [ecx + ebp*2 + 0x73], ah
        jo     0x255
loc_2_01E9:
        popal  
        jns    0x251
        and    byte ptr cs:[eax], ah
        pop    ecx
        outsd  dx, dword ptr [esi]
        jne    loc_2_0214
        arpl   word ptr [ecx + 0x6e], sp
        and    byte ptr [edi + 0x61], dh
        imul   esi, dword ptr [eax + 0x61], 0x7320646e
        and    byte ptr gs:[ecx + 0x66], ch
loc_2_0207:
        and    byte ptr [ecx + 0x74], ch
        and    byte ptr [ecx + 0x70], ah
        jo     loc_2_0274
        popal  
        jb     0x285
        sub    al, 0x20
loc_2_0214:
        outsd  dx, dword ptr [esi]
        jb     0x237
        jns    0x288
        jne    0x23b
        arpl   word ptr [ecx + 0x6e], sp
loc_2_021E:
        and    byte ptr [edx + 0x65], dh
        jae    loc_2_0297
        popal  
        jb     loc_2_029A
        and    byte ptr [ecx + 0x6f], bh
        jne    0x29d
loc_2_022B:
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
loc_2_022F:
        jo     loc_2_02A6
        je     loc_2_0298
        jb     0x263
        or     eax, 0x202a0a0a
        and    byte ptr [eax + 0x72], dl
        jae    0x2b3
        and    byte ptr [ecx + 0x6e], ah
        jns    0x265
        imul   esp, dword ptr [ebp + 0x79], 0x20
        je     0x2ba
        and    byte ptr [edx + 0x65], dh
        je     loc_2_02C5
        jb     loc_2_02C0
        and    byte ptr [edi + ebp*2 + 0x20], dh
        push   edi
        imul   ebp, dword ptr [esi + 0x64], 0x2073776f
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[edi + 0x61], dh
        imul   esi, dword ptr [esi + ebp + 0xd], 0x20202a0a
        push   eax
        jb     loc_2_02D4
        jae    loc_2_02E4
        and    byte ptr [ebx + 0x54], al
loc_2_0274:
        push   edx
        dec    esp
        sub    eax, dword ptr [ecx + 0x4c]
        push   esp
        sub    eax, dword ptr [ebp + eax*2 + 0x4c]
        and    byte ptr [ecx + 0x67], ah
        popal  
        imul   ebp, dword ptr [esi + 0x20], 0x72206f74
        jae    0x300
        popal  
        jb     0x303
        and    byte ptr [ecx + 0x6f], bh
        jne    0x306
        and    byte ptr [ebx + 0x6f], ah
loc_2_0297:
        insd   dword ptr es:[edi], dx
loc_2_0298:
        jo     0x30f
loc_2_029A:
        je     0x301
        jb     loc_2_02CC
        and    byte ptr [ecx + 0x6f], bl
        jne    loc_2_02C3
        ja     0x30e
        insb   byte ptr es:[edi], dx
loc_2_02A6:
        insb   byte ptr es:[edi], dx
        or     eax, 0x2020080a
        and    byte ptr [edi + ebp*2 + 0x73], ch
        and    byte ptr gs:[ebp + 0x6e], dh
        jae    0x317
        jbe    0x31d
        and    byte ptr fs:[ecx + 0x6e], ch
        outsw  dx, word ptr [esi]
        jb     0x32d
loc_2_02C0:
        popal  
        je     0x32c
loc_2_02C3:
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_2_02C5:
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [ecx + 0x6c], ah
        insb   byte ptr es:[edi], dx
loc_2_02CC:
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        jb     0x334
        insd   dword ptr es:[edi], dx
loc_2_02D4:
        jae    0x2f6
        je     0x340
        popal  
        je     0x2fb
        popal  
        jb     0x343
        and    byte ptr [edx + 0x75], dh
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        db  069h
loc_2_02E4:
        outsb  dx, byte ptr [esi]
        add    byte ptr cs:[bx + si], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x10c  R-X
loc_3_0000:
        mov    eax, 0x19
        mov    esi, 0xdf                     ; -> loc_4_00DF
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    ax, 0xff53
        mov    ebx, 0x1800180
        mov    cl, 1
        mov    esi, 0x3b                     ; -> loc_1_003B
        VxDCall  VKD.VKD_Define_Hot_Key          ; cd2001000d00
        xor    esi, esi
        mov    edi, 0xee                     ; -> loc_3_00EE
        mov    eax, 0xffffffff
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        cmp    eax, -1
        je     loc_3_0044
        or     byte ptr [0x2e4], 1           ; -> obj1:0x02E4
loc_3_0044:
        xor    esi, esi
        mov    edi, 0xfa                     ; -> loc_3_00FA
        xor    eax, eax
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        test   eax, eax
        je     loc_3_005E
        or     byte ptr [0x2e4], 0x40        ; -> obj1:0x02E4
loc_3_005E:
        xor    esi, esi
        mov    edi, 0xe4                     ; -> loc_3_00E4
        xor    eax, eax
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        test   eax, eax
        je     loc_3_007C
        mov    esi, 0x16f                    ; -> REBOOT_Service_03
        VMMCall  VMM.Hook_NMI_Event              ; cd2018000100
loc_3_007C:
        push   0x2b8                         ; -> obj1:0x02B8
        push   0x268                         ; -> obj1:0x0268
        VMMCall  VMM._Register_Win32_Services    ; cd2029010100
        pop    ecx
        pop    ecx
        VMMCall  VMM.Get_Config_Directory        ; cd20b7000100
        mov    al, byte ptr [edx]
        and    al, 0xdf
        sub    al, 0x41
        mov    byte ptr [0x2e7], al          ; -> loc_1_02E7
        clc    
        ret    
        db  000h, 000h, 000h
loc_3_00A4:
        push   eax
        mov    eax, esp
        push   0
        push   eax
        push   1
        push   0xff
        VMMCall  VMM._CopyPageTable              ; cd2061000100
        add    esp, 0x10
        pop    eax
        shr    eax, 0xc
        cmp    eax, 0xff
        jne    loc_3_00E0
        mov    esi, 0x220                    ; -> loc_1_0220
        VMMCall  VMM.Hook_Invalid_Page_Fault     ; cd20e9000100
        jb     loc_3_00E0
        mov    byte ptr [1], 0x50            ; -> loc_5_0001
        mov    byte ptr [0], 0x53            ; -> loc_6_0000
loc_3_00E0:
        clc    
        ret    
        db  000h, 000h
loc_3_00E4:
        dec    esi
        dec    ebp
        dec    ecx
        push   edx
        inc    ebp
        inc    edx
        dec    edi
        dec    edi
        push   esp
        db  000h
loc_3_00EE:
        dec    esp
        dec    edi
        inc    ebx
        inc    ecx
        dec    esp
        push   edx
        inc    ebp
        inc    edx
        dec    edi
        dec    edi
        push   esp
        db  000h
loc_3_00FA:
        inc    esp
        bound  esi, qword ptr gs:[ebp + 0x67]
        dec    esp
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ecx + 0x6c], sp
        push   edx
        bound  ebp, qword ptr gs:[edi + 0x6f]
        je     0x10b
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xee  R-X
REBOOT_PM_API:
        movzx  eax, word ptr [ebp + 0x1c]
        dec    ah
        jne    loc_4_0016
        test   eax, eax
        je     REBOOT_Service_03
        dec    eax
        je     loc_4_001B
        dec    eax
        je     loc_4_0024
loc_4_0016:
        or     byte ptr [ebp + 0x2c], 1
        ret    
loc_4_001B:
        or     byte ptr [0x2e4], 0x80        ; -> obj1:0x02E4
        jmp    loc_4_002B
loc_4_0024:
        and    byte ptr [0x2e4], 0x7f        ; -> obj1:0x02E4
loc_4_002B:
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_4_0030:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [0x2d8], eax        ; -> obj1:0x02D8
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [0x2dc], eax        ; -> obj1:0x02DC
        ret    0x10
loc_4_0045:
        mov    al, byte ptr [esp + 0xc]
        mov    byte ptr [0x2e6], al          ; -> obj1:0x02E6
        cmp    al, 0
        je     loc_4_005A
        cmp    al, 8
        je     loc_4_005A
        cmp    al, 3
        jne    loc_4_0068
loc_4_005A:
        xor    esi, esi
        xchg   dword ptr [0x2e0], esi        ; -> obj1:0x02E0
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
loc_4_0068:
        cmp    al, 0
        jne    loc_4_0088
        btr    word ptr [0x2e4], 8           ; -> obj1:0x02E4
        jae    loc_4_0088
        mov    edi, dword ptr [0x2dc]        ; -> obj1:0x02DC
        mov    eax, 0xffc00000
        VMMCall  VMM.Adjust_Thread_Exec_Priority ; cd2014010100
loc_4_0088:
        ret    0x10
        ; ---- dispatch table (3 entries) ----
        dd  loc_4_00A8
        dd  loc_4_00AF
        dd  loc_4_00B9
loc_4_0097:
        mov    eax, dword ptr [esi + 0xc]
        cmp    eax, 3
        jae    loc_4_00C3
        jmp    dword ptr [eax*4 + 0x8b]      ; -> obj4:0x008B
loc_4_00A8:
        xor    eax, eax
        mov    ecx, eax
        mov    ch, 4
        ret    
loc_4_00AF:
        or     byte ptr [0x2e4], 0x80        ; -> obj1:0x02E4
        xor    eax, eax
        ret    
loc_4_00B9:
        and    byte ptr [0x2e4], 0x7f        ; -> obj1:0x02E4
        xor    eax, eax
        ret    
loc_4_00C3:
        xor    eax, eax
        inc    eax
        ret    
        db  000h
loc_4_00C8:
        call   sub_6_0037
        mov    eax, 0x64
        mov    esi, 0x67                     ; -> loc_5_0067
        VMMCall  VMM.Set_VM_Time_Out             ; cd203d000100
        clc    
        ret    
loc_4_00DF:
        sub    ecx, ecx
        mov    eax, 0x1388
        VMMCall  VMM.Close_VM                    ; cd20ec000100
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x9c  R-X
loc_5_0000:
        clc    
loc_5_0001:
        ret    
        db  08Bh, 0C4h, 06Ah, 000h, 050h, 06Ah, 001h, 068h, 0FFh, 000h, 000h, 000h, 0CDh, 020h, 061h, 000h
        db  001h, 000h, 083h, 0C4h, 010h, 058h, 0C1h, 0E8h, 00Ch, 03Dh, 0FFh, 000h, 000h, 000h, 075h, 043h
        db  0A1h
        dd  obj1:0x02EC              ; fixup
        db  085h, 0C0h, 075h, 035h, 068h, 000h, 001h, 000h, 000h, 050h, 050h, 050h, 050h, 050h, 06Ah, 001h
        db  06Ah, 001h, 0CDh, 020h, 053h, 000h, 001h, 000h, 083h, 0C4h, 020h, 085h, 0C0h, 074h, 01Fh, 0A3h
        dd  obj1:0x02EC              ; fixup
        db  0BEh, 000h, 0F0h, 00Fh, 000h, 08Bh, 0F8h, 0FCh, 0B9h, 000h, 004h, 000h, 000h, 0F3h, 0A5h, 066h
        db  0C7h, 047h, 0F0h, 0CDh, 019h, 0E8h, 002h, 000h, 000h, 000h, 0F8h, 0C3h
loc_5_0067:
        push   0
        push   0
        push   1
        push   0xff
        push   ebx
        push   dword ptr [0x2ec]             ; -> obj1:0x02EC
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        push   0
        push   -1
        push   0
        push   -3
        push   1
        push   0xff
        push   ebx
        VMMCall  VMM._ModifyPageBits             ; cd2060000100
        add    esp, 0x1c
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x4b  R-X
loc_6_0000:
        ret    
        db  0E8h, 031h, 000h, 000h, 000h, 08Bh, 0C3h, 0CDh, 020h, 03Bh, 000h, 001h, 000h, 0CDh, 020h, 004h
        db  000h, 001h, 000h, 075h, 01Fh, 0CDh, 020h, 03Bh, 000h, 001h, 000h, 033h, 0C3h, 075h, 015h, 087h
        db  005h
        dd  obj1:0x02EC              ; fixup
        db  085h, 0C0h, 074h, 00Bh, 06Ah, 000h, 050h, 0CDh, 020h, 055h, 000h, 001h, 000h, 059h, 059h, 05Bh
        db  0C3h
sub_6_0037:
        xor    eax, eax
        push   eax
        push   1
        mov    al, 0xff
        push   eax
        push   ebx
        push   eax
        VMMCall  VMM._PhysIntoV86                ; cd205e000100
        add    esp, 0x14
        ret    
VxD__LTEXT ENDS
