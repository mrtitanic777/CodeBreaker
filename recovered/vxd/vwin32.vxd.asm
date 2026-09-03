; VWIN32 -- reconstructed 32-bit disassembly (VxD / LE format)
; 8 objects, 8295 instructions recovered by recursive descent

Declare_Virtual_Device VWIN32, 1, 3, VWIN32_Control, 0002Ah, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h
;   service table: obj1:0x1010, 29 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1210  R-X
VWIN32_Control:
        cmp    eax, 0
        je     loc_3_0000
        cmp    eax, 1
        je     loc_3_0044
        cmp    eax, 0x1d
        je     loc_4_2E41
        cmp    eax, 0x1e
        je     loc_4_2E9F
        cmp    eax, 0x1f
        je     loc_4_2EDC
        cmp    eax, 0x21
        je     loc_4_2F2C
        cmp    eax, 2
        je     loc_3_020A
        cmp    eax, 0x15
        je     loc_4_044E
        cmp    eax, 0x16
        je     loc_4_0507
        cmp    eax, 0x23
        je     loc_4_0950
        clc    
        ret    
VWIN32_Get_Version:
        mov    eax, 0x103
        clc    
        ret    
VWIN32_GetCurrentDirectory:
        push   ebx
        push   edi
        push   esi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_00A6
        mov    ecx, dword ptr [0x10dc]       ; -> obj1:0x10DC
        jecxz  loc_1_00A6
        test   dword ptr [ecx + 0x20], 8
        jne    loc_1_00A6
        mov    ecx, dword ptr [ecx + 0x40]
        jecxz  loc_1_00A6
        mov    ecx, dword ptr [ecx + 0xc]
        jecxz  loc_1_00A6
        mov    edi, ecx
        push   edi
        xor    eax, eax
        mov    ecx, 0xffffffff
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edi, esi
        pop    esi
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, ecx
        jmp    loc_1_00AB
loc_1_00A6:
        mov    eax, 0xffffffff
loc_1_00AB:
        pop    esi
        pop    edi
        pop    ebx
        ret    
VWIN32_IFSRIPWhenLev2Taken:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, ebx
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        pop    ebx
        jne    loc_1_00D1
        mov    eax, dword ptr [0x10d8]       ; -> obj1:0x10D8
        mov    edx, dword ptr [0x2a4]        ; -> obj5:0x02A4
        mov    edx, dword ptr [edx + 8]
        sub    eax, edx
loc_1_00D1:
        ret    
VWIN32_GetCurrentProcessHandle:
        mov    eax, dword ptr [0x10dc]       ; -> obj1:0x10DC
        clc    
        ret    
loc_1_00D9:
        mov    ebx, edx
VWIN32_DIOCCompletionRoutine:
        test   ebx, ebx
        je     loc_1_010B
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    edx, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        cmp    edx, ebx
        pop    ebx
        je     loc_1_0104
        push   esi
        mov    esi, 0xd9                     ; -> loc_1_00D9
        xchg   ebx, edx
        VMMCall  VMM.Schedule_VM_Event           ; cd200f000100
        pop    esi
        jmp    loc_1_010B
loc_1_0104:
        mov    eax, ebx
        call   _VWIN32_SetWin32Event
loc_1_010B:
        clc    
        ret    
VWIN32_ActiveTimeBiasSet:
        clc    
        ret    
_VWIN32_IsClientWin32:
        push   ebx
        push   edi
        push   ebp
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    ebx, dword ptr [edi + 0x14]
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_0145
        mov    ebp, dword ptr [edi + 0x10]
        movzx  eax, word ptr [ebp + 0x28]
        cmp    ax, word ptr [0x360]          ; -> loc_5_0360
        jne    loc_1_0145
        movzx  eax, word ptr [ebp + 0x34]
        sub    ax, word ptr [0x362]          ; -> loc_5_0362
        jne    loc_1_0145
        inc    eax
loc_1_0141:
        pop    ebp
        pop    edi
        pop    ebx
        ret    
loc_1_0145:
        sub    eax, eax
        jmp    loc_1_0141
        db  000h, 000h, 000h
VWIN32_BlueScreenPopup:
        xor    ecx, ecx
        test   dl, 1
        je     loc_1_0162
        VxDCall  DEBUG.Service_0000h             ; cd2000000200
        or     eax, eax
        je     loc_1_0162
        mov    ecx, 1
loc_1_0162:
        VxDCall  REBOOT.Service_0001h            ; cd2001000900
        push   edx
        sub    esp, 0x50
        mov    eax, 1
        test   edx, 2
        je     loc_1_019F
        mov    edx, cs
        cmp    bx, dx
        jne    loc_1_019F
        mov    edx, esp
        push   ecx
        push   edx
        push   edi
        VMMCall  VMM._GetVxDName                 ; cd207e010100
        add    esp, 8
        pop    ecx
        or     eax, eax
        mov    eax, 1
        je     loc_1_019F
        mov    eax, 2
        mov    edx, esp
loc_1_019F:
        or     ecx, 0x40001000
        add    eax, 0x180000
        push   edx
        push   edi
        push   ebx
        push   esi
        push   eax
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        xchg   ecx, eax
        xor    edi, edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        add    esp, dword ptr [esp]
        add    esp, 0x50
        pop    edx
        VxDCall  REBOOT.Service_0002h            ; cd2002000900
        cmp    eax, 2
        je     loc_1_01F0
        test   dl, 4
        jne    loc_1_01EC
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        VxDCall  NDIS.Service_0018h              ; cd2018002a00
loc_1_01EC:
        xor    eax, eax
        jmp    loc_1_01F3
loc_1_01F0:
        xor    eax, eax
        inc    eax
loc_1_01F3:
        ret    
sub_1_01F4:
        push   edi
        mov    edi, dword ptr [0x10e0]       ; -> obj1:0x10E0
loc_1_01FB:
        mov    eax, dword ptr [esi]
        cmp    eax, -1
        jne    loc_1_021A
        mov    dword ptr [edi + 0x44], esi
        push   1
        push   esi
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        add    esp, 8
        xor    eax, eax
        xchg   dword ptr [edi + 0x44], eax
        or     eax, eax
        jne    loc_1_01FB
loc_1_021A:
        pop    edi
        ret    
loc_1_021C:
        xor    ecx, ecx
        xchg   dword ptr [edx + 0x18], ecx
        jecxz  loc_1_0241
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xor    eax, eax
        mov    ecx, dword ptr [0x111c]       ; -> obj1:0x111C
        mov    eax, 0x1000
        mov    esi, 0x198f                   ; -> loc_4_198F
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
loc_1_0241:
        ret    
loc_1_0242:
        xor    ecx, ecx
        xchg   dword ptr [edx + 0x18], ecx
        jecxz  loc_1_0267
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xor    eax, eax
        mov    ecx, dword ptr [0x111c]       ; -> obj1:0x111C
        mov    eax, 0x1000
        mov    esi, 0x19ce                   ; -> loc_4_19CE
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
loc_1_0267:
        ret    
loc_1_0268:
        enter  8, 0
        push   ebx
        push   edi
        push   esi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    esi, edi
        xor    ecx, ecx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        mov    dword ptr [ebp - 8], eax
        movzx  eax, word ptr [esi + 0x2a]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0x11a2]       ; -> obj1:0x11A2
        mov    ecx, 1
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        mov    eax, dword ptr [0x119e]       ; -> obj1:0x119E
        mov    word ptr [esi + 0x2a], ax
        push   0x31
        push   dword ptr [0x11b6]            ; -> obj1:0x11B6
        VMMCall  VMM._EnterMutex                 ; cd202e010100
        add    esp, 8
        mov    edi, dword ptr [ebp + 8]
        mov    edi, dword ptr [edi]
        rol    edi, 3
        mov    ebx, edi
        shr    ebx, 0x16
        movzx  ebx, word ptr [ebx*2 + 0x119c] ; -> obj1:0x119C
        mov    edx, edi
        and    edx, 0x7ffff
        shl    edx, 9
        shld   ecx, edx, 0x10
        mov    ax, 0x4200
        push   0x21
        VMMCall  VMM._ExecVxDIntMustComplete     ; cd203b010100
        jb     loc_1_0316
        mov    edx, edi
        mov    ecx, 0x1000
        and    edx, 0x380000
        je     loc_1_0307
        mov    ecx, edx
        shr    ecx, 0x13
        neg    ecx
        and    ecx, 7
        shl    ecx, 9
loc_1_0307:
        mov    edx, dword ptr [ebp + 0xc]
        mov    ax, 0x3f00
        push   0x21
        VMMCall  VMM._ExecVxDIntMustComplete     ; cd203b010100
loc_1_0316:
        sbb    ebx, ebx
        inc    ebx
        push   dword ptr [0x11b6]            ; -> obj1:0x11B6
        VMMCall  VMM._LeaveMutex                 ; cd202f010100
        add    esp, 4
        or     ebx, ebx
        je     loc_1_0348
        mov    ecx, edi
        and    ecx, 0x380000
        je     loc_1_0348
        shr    ecx, 0xc
        mov    edi, dword ptr [ebp + 0xc]
        add    edi, 0xffc
        sub    eax, eax
        std    
        rep stosd dword ptr es:[edi], eax
        cld    
loc_1_0348:
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 0x2a], ax
        pop    eax
        pop    esi
        pop    edi
        pop    ebx
        leave  
        ret    
sub_1_036A:
        enter  8, 0
        push   ebx
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        cmp    ebx, 0xffff
        jb     loc_1_039F
        mov    eax, 0x8000d600
        mov    al, byte ptr [ebp + 0x20]
        sub    al, 0x3f
        xor    ecx, ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [ebp + 0x10]
        VxDCall  IFSMgr.IFSMgr_Ring0_FileIO      ; cd2032004000
        sbb    eax, eax
        inc    eax
        jmp    loc_1_0460
loc_1_039F:
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    esi, edi
        xor    ecx, ecx
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        mov    dword ptr [ebp - 8], eax
        movzx  eax, word ptr [esi + 0x2a]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, 1
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        pop    ebx
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [esi + 0x2a], ax
        push   0x31
        push   dword ptr [0x11b2]            ; -> obj1:0x11B2
        VMMCall  VMM._EnterMutex                 ; cd202e010100
        add    esp, 8
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        shld   ecx, edx, 0x10
        mov    ax, 0x4200
        push   0x21
        VMMCall  VMM._ExecVxDIntMustComplete     ; cd203b010100
        jb     loc_1_0410
        mov    edx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    ah, byte ptr [ebp + 0x20]
        push   0x21
        VMMCall  VMM._ExecVxDIntMustComplete     ; cd203b010100
loc_1_0410:
        sbb    ebx, ebx
        inc    ebx
        push   dword ptr [0x11b2]            ; -> obj1:0x11B2
        VMMCall  VMM._LeaveMutex                 ; cd202f010100
        add    esp, 4
        or     ebx, ebx
        je     loc_1_0443
        cmp    byte ptr [ebp + 0x20], 0x40
        je     loc_1_0443
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edi, dword ptr [ebp + 0x10]
        add    edi, ecx
        neg    ecx
        add    ecx, 0x1000
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
loc_1_0443:
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 0x2a], ax
        pop    eax
loc_1_0460:
        pop    esi
        pop    edi
        pop    ebx
        leave  
        ret    
loc_1_0465:
        enter  8, 0
        push   ebx
        push   edi
        push   esi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    esi, edi
        xor    ecx, ecx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        mov    dword ptr [ebp - 8], eax
        movzx  eax, word ptr [esi + 0x2a]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, 1
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        mov    eax, dword ptr [ebp + 0x10]
        mov    word ptr [esi + 0x2a], ax
        push   0x31
        push   dword ptr [0x11b2]            ; -> obj1:0x11B2
        VMMCall  VMM._EnterMutex                 ; cd202e010100
        add    esp, 8
        push   esi
        mov    ax, 0x716c
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dx, 1
        xor    cx, cx
        push   0x21
        VMMCall  VMM._ExecVxDIntMustComplete     ; cd203b010100
        pushfd 
        xor    ebx, ebx
        popfd  
        jae    loc_1_04D8
        or     ebx, 0xffff0000
loc_1_04D8:
        mov    bx, ax
        pop    esi
        push   ebx
        push   dword ptr [0x11b2]            ; -> obj1:0x11B2
        VMMCall  VMM._LeaveMutex                 ; cd202f010100
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 0x2a], ax
        pop    eax
        pop    esi
        pop    edi
        pop    ebx
        leave  
        ret    
loc_1_050D:
        enter  8, 0
        push   ebx
        push   edi
        push   esi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    esi, edi
        xor    ecx, ecx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        mov    dword ptr [ebp - 8], eax
        movzx  eax, word ptr [esi + 0x2a]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, 1
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [esi + 0x2a], ax
        push   0x31
        push   dword ptr [0x11b2]            ; -> obj1:0x11B2
        VMMCall  VMM._EnterMutex                 ; cd202e010100
        add    esp, 8
        mov    ebx, dword ptr [ebp + 8]
        mov    ax, 0x3e00
        push   0x21
        VMMCall  VMM._ExecVxDIntMustComplete     ; cd203b010100
        pushfd 
        xor    eax, eax
        popfd  
        jb     loc_1_0570
        inc    eax
loc_1_0570:
        push   eax
        push   dword ptr [0x11b2]            ; -> obj1:0x11B2
        VMMCall  VMM._LeaveMutex                 ; cd202f010100
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 0x2a], ax
        pop    eax
        pop    esi
        pop    edi
        pop    ebx
        leave  
        ret    
sub_1_05A1:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], 0x3e4        ; -> loc_4_03E4
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebx], 0x3f9        ; -> loc_4_03F9
        mov    ebx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx], 0x417        ; -> loc_4_0417
        mov    ebx, dword ptr [ebp + 0x14]
        mov    dword ptr [ebx], 0x408        ; -> loc_4_0408
        mov    ebx, dword ptr [ebp + 0x18]
        mov    dword ptr [ebx], 0x426        ; -> loc_4_0426
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebx], 0x43b        ; -> loc_4_043B
        mov    ebx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebx], 0x465        ; -> loc_1_0465
        mov    ebx, dword ptr [ebp + 0x24]
        mov    dword ptr [ebx], 0x50d        ; -> loc_1_050D
        mov    ebx, dword ptr [ebp + 0x28]
        mov    dword ptr [ebx], 0xfb6        ; -> loc_1_0FB6
        mov    ebx, dword ptr [ebp + 0x2c]
        mov    dword ptr [ebx], 0x120c       ; -> loc_1_120C
        mov    eax, 0x36a                    ; -> sub_1_036A
        pop    ebx
        leave  
        ret    
        db  000h
sub_1_0608:
        mov    edx, dword ptr [0x10e0]       ; -> obj1:0x10E0
        or     byte ptr [edx + 0x2c], 0x10
loc_1_0612:
        lea    eax, [edx + 0x289816]
        push   0x11
        push   eax
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        add    esp, 8
        test   byte ptr [edx + 0x2c], 0x10
        jne    loc_1_0612
        ret    
sub_1_062B:
        mov    edx, dword ptr [0x10e0]       ; -> obj1:0x10E0
loc_1_0631:
        test   byte ptr [edx + 0x2e], 4
        je     loc_1_064B
        lea    eax, [edx + 0x569595]
        push   0x11
        push   eax
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        add    esp, 8
        jmp    loc_1_0631
loc_1_064B:
        ret    
sub_1_064C:
        push   ebx
        mov    ebx, dword ptr [esp + 8]
        mov    eax, dword ptr [ebx + 0x5c]
        cmp    eax, dword ptr [0x1114]       ; -> obj1:0x1114
        je     loc_1_0674
        mov    fs, word ptr [ebx + 0x1c]
        mov    word ptr fs:[6], 1
        push   -1
        call   sub_1_0813
        add    esp, 4
loc_1_0674:
        or     al, 0xff
        xchg   byte ptr [0x11c0], al         ; -> obj1:0x11C0
        or     al, al
        je     loc_1_06A1
        push   esi
        push   edi
        mov    fs, word ptr [ebx + 0x1c]
        mov    word ptr fs:[6], 1
        VMMCall  VMM.Get_Sys_Thread_Handle       ; cd200a010100
        xor    ecx, ecx
        call   sub_4_05BF
        pop    edi
        pop    esi
        jmp    loc_1_0674
loc_1_06A1:
        pop    ebx
        ret    4
sub_1_06A5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    edx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    edx, dword ptr [edx + edi]
        btr    dword ptr [edx + 0x2c], 2
        jae    loc_1_06DD
        cmp    dword ptr [edx + 0x20], 0
        je     loc_1_06DD
loc_1_06C7:
        lea    eax, [edx + 0x20]
        push   0x11
        push   eax
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        add    esp, 8
        btr    dword ptr [edx + 0x2c], 3
        jae    loc_1_06C7
loc_1_06DD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_06E4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    edi, dword ptr [edi + esi]
        mov    ebx, dword ptr [edi + 0x20]
        cmp    ebx, 0x7f
        jge    loc_1_0737
        inc    dword ptr [edi + 0x20]
        or     ebx, ebx
        jg     loc_1_072C
        btr    dword ptr [edi + 0x2c], 3
        jb     loc_1_072C
        bts    dword ptr [edi + 0x2c], 2
        jb     loc_1_072C
        push   2
        push   esi
        push   0
        push   0x6a5                         ; -> sub_1_06A5
        call   _VWIN32_QueueKernelAPC
        add    esp, 0x10
        or     eax, eax
        je     loc_1_0733
        mov    dword ptr [edi + 0x24], eax
loc_1_072C:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0733:
        and    byte ptr [edi + 0x2c], 0xfb
loc_1_0737:
        mov    ebx, 0xffffffff
        jmp    loc_1_072C
sub_1_073E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    edx, dword ptr [edx + edi]
        mov    ebx, dword ptr [edx + 0x20]
        or     ebx, ebx
        je     loc_1_0773
        dec    dword ptr [edx + 0x20]
        jne    loc_1_0773
        call   sub_1_0779
        jb     loc_1_0773
        or     byte ptr [edx + 0x2c], 8
        lea    eax, [edx + 0x20]
        push   eax
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
loc_1_0773:
        mov    eax, ebx
        pop    edi
        pop    ebx
        leave  
        ret    
sub_1_0779:
        btr    dword ptr [edx + 0x2c], 2
        jae    loc_1_078F
        xor    eax, eax
        xchg   dword ptr [edx + 0x24], eax
        push   edx
        push   esi
        call   sub_1_0A44
        pop    esi
        pop    edx
        stc    
loc_1_078F:
        ret    
sub_1_0790:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    edx, dword ptr [0x10e0]       ; -> obj1:0x10E0
        mov    dword ptr [edx + 0x10], 0
        cmp    dword ptr [edx + 0x1c], 0
        jg     loc_1_07F4
        mov    eax, dword ptr [ebp + 8]
        inc    eax
        je     loc_1_07F4
        dec    eax
        jne    loc_1_07D1
        VMMCall  VMM.Time_Slice_Sleep            ; cd2061010100
        or     byte ptr [edx + 0x2c], 2
        cmp    dword ptr [edx + 0x1c], 0
        jg     loc_1_07F4
        mov    dword ptr [edx + 0x14], 0x102
        jmp    loc_1_0807
loc_1_07D1:
        push   edx
        mov    edx, edi
        mov    esi, 0x87d                    ; -> loc_1_087D
        mov    ecx, eax
        sub    ecx, 0x7fffffff
        jbe    loc_1_07EA
        mov    eax, ecx
        mov    esi, 0x89b                    ; -> loc_1_089B
loc_1_07EA:
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        pop    edx
        mov    dword ptr [edx + 0x10], esi
loc_1_07F4:
        dec    dword ptr [edx + 0x1c]
        jge    loc_1_0807
        mov    eax, dword ptr [edx + 0x18]
        mov    ecx, 0x35
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
loc_1_0807:
        and    byte ptr [edx + 0x2c], 0xfe
        mov    eax, dword ptr [edx + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0813:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_0790
        add    esp, 4
        mov    edx, dword ptr [0x10e0]       ; -> obj1:0x10E0
        and    byte ptr [edx + 0x2c], 0xfd
        mov    eax, dword ptr [edx + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0836:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    edx, dword ptr [edx + esi]
        and    dword ptr [edx + 0x2c], 0xfffffffe
        xor    esi, esi
        xchg   dword ptr [edx + 0x10], esi
        or     esi, esi
        je     loc_1_085B
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
loc_1_085B:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x14], eax
        bts    dword ptr [edx + 0x2c], 1
        jb     loc_1_0876
        inc    dword ptr [edx + 0x1c]
        jg     loc_1_0876
        mov    eax, dword ptr [edx + 0x18]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_1_0876:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_087D:
        mov    eax, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    eax, dword ptr [eax + edx]
        mov    dword ptr [eax + 0x10], 0
        push   0x102
        push   edx
        call   sub_1_0836
        add    esp, 8
        ret    
loc_1_089B:
        mov    edi, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    edi, dword ptr [edi + edx]
        mov    esi, 0x87d                    ; -> loc_1_087D
        mov    eax, 0x7fffffff
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [edi + 0x10], esi
        ret    
sub_1_08B8:
        push   ebx
        push   esi
        push   edi
        push   1
        push   0xc
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_1_08F3
        mov    esi, eax
        sub    ecx, ecx
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jb     loc_1_08E7
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi], 0x78747665
        mov    eax, esi
loc_1_08E3:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_08E7:
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_1_08F3:
        sub    eax, eax
        jmp    loc_1_08E3
sub_1_08F7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0x78747665
        jne    loc_1_091F
        mov    eax, dword ptr [eax + 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        push   0
        push   dword ptr [ebp + 8]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_1_091F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0924:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0x78747665
        jne    loc_1_0952
        dec    dword ptr [eax + 8]
        je     loc_1_0948
        mov    eax, dword ptr [eax + 4]
        mov    ecx, 0x35
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
loc_1_0948:
        mov    eax, 1
loc_1_094D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0952:
        sub    eax, eax
        jmp    loc_1_094D
sub_1_0956:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 0x78747665
        jne    loc_1_097E
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 8], 1
        je     loc_1_097E
        inc    dword ptr [eax + 8]
        jg     loc_1_097E
        mov    eax, dword ptr [eax + 4]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_1_097E:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0985:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    ebx, dword ptr [ebp + 0xc]
        VMMCall  VMM.Get_Initial_Thread_Handle   ; cd200d010100
        mov    eax, edi
        mov    edi, dword ptr [ebp + 8]
        VMMCall  VMM.Attach_Thread_To_Group      ; cd206b010100
        VMMCall  VMM.Boost_Thread_With_VM        ; cd2078010100
        pop    ebx
        pop    edi
        leave  
        ret    
loc_1_09A8:
        mov    edx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    edx, dword ptr [edi + edx]
        or     edx, edx
        je     loc_1_0A2A
        mov    eax, dword ptr [edx]
        mov    dword ptr [0x10d8], eax       ; -> obj1:0x10D8
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [0x10dc], eax       ; -> obj1:0x10DC
        mov    dword ptr [0x10e0], edx       ; -> obj1:0x10E0
        mov    ax, word ptr [edx + 0x72]
        mov    ebx, dword ptr [0x10e4]       ; -> obj1:0x10E4
        mov    word ptr [ebx], ax
        mov    esi, dword ptr [0x10f4]       ; -> obj1:0x10F4
        cmp    edx, esi
        je     loc_1_0A14
        push   ebx
        push   edx
        xor    ecx, ecx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        pop    edx
        mov    word ptr [esi + 0x76], ax
        mov    ax, word ptr [edx + 0x76]
        mov    ecx, 1
        push   edx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        pop    edx
        pop    ebx
        mov    dword ptr [0x10f4], edx       ; -> obj1:0x10F4
loc_1_0A14:
        mov    eax, dword ptr [edx + 8]
        test   eax, eax
        je     loc_1_0A2A
        mov    dword ptr [0x10e8], eax       ; -> obj1:0x10E8
        push   eax
        VMMCall  VMM._ContextSwitch              ; cd2032010100
        add    esp, 4
loc_1_0A2A:
        ret    
VWIN32_GetContextHandle:
        cmp    dword ptr [edi + 0x2c], 0x2a
        jne    loc_1_0A41
        mov    ebx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    ebx, dword ptr [edi + ebx]
        or     ebx, ebx
        je     loc_1_0A41
        mov    ebx, dword ptr [ebx + 8]
loc_1_0A41:
        ret    
        db  000h, 000h
sub_1_0A44:
        call   sub_1_0A9F
        jb     loc_1_0A74
        xor    esi, esi
        xchg   dword ptr [eax + 0xc], esi
        or     esi, esi
        je     sub_1_0A7C
        cmp    esi, -1
        je     loc_1_0A61
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        jmp    sub_1_0A7C
loc_1_0A61:
        or     byte ptr [eax + 0x1a], 4
        and    byte ptr [eax + 0x1a], 0xfe
        xchg   dword ptr [eax + 0xc], esi
        mov    dword ptr [eax + 8], 0xa75    ; -> loc_1_0A75
        clc    
loc_1_0A74:
        ret    
loc_1_0A75:
        push   ebp
        mov    ebp, esp
        leave  
        ret    4
sub_1_0A7C:
        call   sub_1_0A9F
        jb     loc_1_0A9E
        mov    edx, dword ptr [eax]
        mov    dword ptr [esi], edx
        and    byte ptr [eax + 0x1a], 0xfe
        test   byte ptr [eax + 0x1a], 2
        jne    loc_1_0A9D
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_1_0A9D:
        clc    
loc_1_0A9E:
        ret    
sub_1_0A9F:
        mov    edx, edi
        add    edx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    edx, dword ptr [edx]
        add    edx, 0x3c
loc_1_0AAC:
        mov    ecx, dword ptr [edx]
        stc    
        jecxz  loc_1_0ABA
        cmp    ecx, eax
        mov    esi, edx
        mov    edx, ecx
        jne    loc_1_0AAC
        clc    
loc_1_0ABA:
        ret    
sub_1_0ABB:
        push   esi
        mov    esi, dword ptr [0x11c8]       ; -> obj1:0x11C8
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_1_0AD7
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 0xc], 0
loc_1_0AD7:
        pop    esi
        ret    
sub_1_0AD9:
        push   esi
        mov    esi, dword ptr [0x11c8]       ; -> obj1:0x11C8
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        pop    esi
        ret    
sub_1_0AE8:
        mov    esi, edx
        add    esi, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    esi, dword ptr [esi]
        or     esi, esi
        je     loc_1_0B1F
        mov    ecx, dword ptr [esi + 0x38]
        jecxz  loc_1_0B07
loc_1_0AFB:
        mov    ebx, ecx
        mov    ecx, dword ptr [ecx]
        jecxz  loc_1_0B03
        jmp    loc_1_0AFB
loc_1_0B03:
        mov    dword ptr [ebx], eax
        jmp    loc_1_0B1F
loc_1_0B07:
        mov    dword ptr [esi + 0x38], eax
        btr    dword ptr [esi + 0x2c], 0
        jae    loc_1_0B1F
        push   0xc0
        push   edx
        call   sub_1_0836
        add    esp, 8
loc_1_0B1F:
        ret    
_VWIN32_QueueUserApc:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0x10]
        cmp    edx, -1
        jne    loc_1_0B40
        mov    edx, dword ptr [0x1110]       ; -> obj1:0x1110
        or     edx, edx
        jne    loc_1_0B40
        VMMCall  VMM.Get_Sys_Thread_Handle       ; cd200a010100
        mov    edx, edi
loc_1_0B40:
        call   sub_1_0ABB
        jae    loc_1_0B4B
        xor    eax, eax
        jmp    loc_1_0B67
loc_1_0B4B:
        mov    dword ptr [eax + 0xc], 0
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], ebx
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 4], ebx
        call   sub_1_0AE8
        mov    eax, esi
        or     eax, eax
loc_1_0B67:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0B6C:
        push   ebx
        push   esi
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    ebx, edi
        add    ebx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    ebx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 0x38]
        or     eax, eax
        je     loc_1_0B9F
loc_1_0B86:
        test   dword ptr [eax + 0xc], 1
        je     loc_1_0B9F
        mov    edx, dword ptr [eax]
        mov    dword ptr [ebx + 0x38], edx
        call   sub_1_0AD9
        mov    eax, edx
        or     eax, eax
        jne    loc_1_0B86
loc_1_0B9F:
        pop    edi
        pop    esi
        pop    ebx
        ret    
_VWIN32_QueueKernelAPC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0x10]
        cmp    ebx, -1
        jne    loc_1_0BC3
        mov    ebx, dword ptr [0x1110]       ; -> obj1:0x1110
        or     ebx, ebx
        jne    loc_1_0BC3
        VMMCall  VMM.Get_Sys_Thread_Handle       ; cd200a010100
        mov    ebx, edi
loc_1_0BC3:
        mov    edi, ebx
        add    edi, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    edi, dword ptr [edi]
        lea    edx, [edi + 0x48]
        test   byte ptr [ebp + 0x14], 2
        jne    loc_1_0BE9
        push   1
        push   0x1c
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_1_0C4C
        mov    edx, eax
loc_1_0BE9:
        xor    eax, eax
        bts    word ptr [edx + 0x1a], 0
        jb     loc_1_0C4C
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 4], ecx
        btr    word ptr [edx + 0x1a], 2
        jb     loc_1_0C33
        mov    dword ptr [edx + 0xc], eax
        mov    eax, dword ptr [edi + 0x3c]
        mov    dword ptr [edx], eax
        mov    dword ptr [edi + 0x3c], edx
        test   byte ptr [ebp + 0x14], 1
        jne    loc_1_0C1E
        cmp    dword ptr [edi + 0x28], 0
        jne    loc_1_0C33
loc_1_0C1E:
        mov    esi, 0xc53                    ; -> loc_1_0C53
        xor    eax, eax
        mov    ecx, 0x1c8
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [edx + 0xc], esi
loc_1_0C33:
        test   byte ptr [ebp + 0x14], 4
        je     loc_1_0C4A
        push   edx
        push   dword ptr [ebp + 0x10]
        push   0
        push   0
        call   _VWIN32_QueueUserApc
        add    esp, 0xc
        pop    edx
loc_1_0C4A:
        mov    eax, edx
loc_1_0C4C:
        or     eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0C53:
        or     eax, 0xffffffff
        xchg   dword ptr [edx + 0xc], eax
        or     eax, eax
        je     loc_1_0C88
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [edx + 0x10], eax
        mov    eax, dword ptr [ebp + 0x24]
        mov    dword ptr [edx + 0x14], eax
        mov    ax, word ptr [ebp + 0x28]
        mov    word ptr [edx + 0x18], ax
        mov    dword ptr [ebp + 0x1c], edx
        mov    cx, word ptr [0x11c6]         ; -> obj1:0x11C6
        movzx  edx, word ptr [0x11c4]        ; -> obj1:0x11C4
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049000100
loc_1_0C88:
        ret    
loc_1_0C89:
        mov    esi, dword ptr [ebp + 0x1c]
        mov    edx, dword ptr [esi + 0x10]
        mov    dword ptr [ebp + 0x1c], edx
        mov    cx, word ptr [esi + 0x18]
        mov    edx, dword ptr [esi + 0x14]
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049000100
        and    byte ptr [esi + 0x1a], 0xfb
        push   dword ptr [esi + 4]
        mov    ebx, dword ptr [esi + 8]
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    eax, esi
        call   sub_1_0A7C
        call   ebx
        ret    
sub_1_0CB9:
        push   ebx
        push   esi
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
loc_1_0CC2:
        mov    esi, edi
        add    esi, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    esi, dword ptr [esi]
        add    esi, 0x3c
loc_1_0CCF:
        mov    ecx, dword ptr [esi]
        jecxz  loc_1_0CEC
        lea    esi, [ecx]
        cmp    dword ptr [ecx + 0xc], 0
        jne    loc_1_0CCF
        push   dword ptr [ecx + 4]
        mov    ebx, dword ptr [ecx + 8]
        mov    eax, ecx
        call   sub_1_0A7C
        call   ebx
        jmp    loc_1_0CC2
loc_1_0CEC:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0CF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_0D12
        mov    eax, dword ptr [0x10e0]       ; -> obj1:0x10E0
        cmp    dword ptr [eax + 0x38], 0
        je     loc_1_0D0E
        mov    dword ptr [eax + 0x14], 0xc0
        jmp    loc_1_0D27
loc_1_0D0E:
        or     byte ptr [eax + 0x2c], 1
loc_1_0D12:
        call   sub_4_3A46
        push   dword ptr [ebp + 8]
        call   sub_1_0790
        add    esp, 4
        call   sub_4_3A2D
loc_1_0D27:
        mov    eax, dword ptr [0x10d8]       ; -> obj1:0x10D8
        mov    eax, dword ptr [eax + 0x54]
        or     eax, eax
        je     loc_1_0D38
        call   sub_4_3B81
loc_1_0D38:
        mov    eax, dword ptr [0x10e0]       ; -> obj1:0x10E0
        mov    ebx, dword ptr [eax + 0x14]
        and    byte ptr [eax + 0x2c], 0xfd
        call   sub_4_3A46
        mov    eax, ebx
        pop    ebx
        leave  
        ret    
_VWIN32_CloseVxDHandle:
        cmp    dword ptr [eax], 2
        je     loc_1_0D56
        xor    eax, eax
        ret    
loc_1_0D56:
        dec    dword ptr [eax + 4]
        jne    loc_1_0D6F
        inc    dword ptr [eax + 4]
        push   -1
        push   eax
        push   dword ptr [0x1100]            ; -> obj1:0x1100
        call   _VWIN32_QueueUserApc
        add    esp, 0xc
loc_1_0D6F:
        ret    
_VWIN32_CheckDelayedNpxTrap:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x80
        push   ebx
        push   esi
        push   edi
        xor    edi, edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [ebp - 0x10], edi
        mov    byte ptr [ebp - 4], 0
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], 1
        mov    ebx, dword ptr [ecx + 0x24]
loc_1_0D95:
        push   0
        lea    eax, [ebp - 1]
        push   1
        push   eax
        push   ebx
        call   _VWIN32_CopyMem
loc_1_0DA3:
        add    esp, 0x10
        test   eax, eax
        je     loc_1_0E5E
        movzx  eax, byte ptr [ebp - 1]
        inc    ebx
        cmp    eax, 0x26
        je     loc_1_0DD5
        cmp    eax, 0x2e
        je     loc_1_0DD5
        cmp    eax, 0x36
        je     loc_1_0DD5
        cmp    eax, 0x3e
        je     loc_1_0DD5
        cmp    eax, 0x64
        jl     loc_1_0DD1
        cmp    eax, 0x65
        jle    loc_1_0DD5
loc_1_0DD1:
        mov    byte ptr [ebp - 2], 0
loc_1_0DD5:
        cmp    byte ptr [ebp - 2], 0
        jne    loc_1_0D95
        push   0
        lea    eax, [ebp - 3]
        push   1
        push   eax
        push   ebx
        call   _VWIN32_CopyMem
loc_1_0DE9:
        add    esp, 0x10
        test   eax, eax
        je     loc_1_0E5E
        mov    cl, byte ptr [ebp - 3]
        inc    ebx
        mov    dword ptr [ebp - 0xc], 0
        and    cl, 0x38
        shr    cl, 3
        cmp    byte ptr [0], 0               ; -> obj1:0x11EC
        je     loc_1_0E5E
loc_1_0E0A:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dl, byte ptr [ebp - 1]
        shl    eax, 2
        cmp    byte ptr [eax], dl            ; -> obj1:0x11EC
        jne    loc_1_0E3F
        cmp    byte ptr [eax + 2], 0         ; -> obj1:0x11EE
        je     loc_1_0E34
        cmp    byte ptr [eax + 1], cl        ; -> loc_1_11ED
        jne    loc_1_0E3F
        mov    al, byte ptr [eax + 3]        ; -> loc_1_11EF
        jmp    loc_1_0E5B
loc_1_0E34:
        mov    dl, byte ptr [ebp - 3]
        cmp    byte ptr [eax + 1], dl        ; -> loc_1_11ED
        je     loc_1_0E51
loc_1_0E3F:
        inc    dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 0xc]
        cmp    byte ptr [eax*4], 0           ; -> obj1:0x11EC
        jne    loc_1_0E0A
        jmp    loc_1_0E5E
loc_1_0E51:
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax*4 + 3]      ; -> loc_1_11EF
loc_1_0E5B:
        mov    byte ptr [ebp - 4], al
loc_1_0E5E:
        cmp    byte ptr [ebp - 4], 0
        jne    loc_1_0E6B
        xor    eax, eax
        jmp    loc_1_0FAF
loc_1_0E6B:
        cmp    byte ptr [ebp - 4], 1
        jne    loc_1_0E7B
        mov    eax, 2
        jmp    loc_1_0FAF
loc_1_0E7B:
        mov    edi, dword ptr [ebp - 0x10]
        lea    esi, [ebp - 0x80]
        VxDCall  VMCPD.VMCPD_Get_Thread_State    ; cd2005001100
        cmp    byte ptr [ebp - 4], 4
        jne    loc_1_0E9F
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 0x7c]
        mov    word ptr [ecx + 0x1c], ax
        mov    dword ptr [ecx + 0x24], ebx
        jmp    loc_1_0FAA
loc_1_0E9F:
        cmp    byte ptr [ebp - 4], 2
        jne    loc_1_0EAA
        mov    eax, dword ptr [ebp - 0x80]
        jmp    loc_1_0EAD
loc_1_0EAA:
        mov    eax, dword ptr [ebp - 0x7c]
loc_1_0EAD:
        mov    word ptr [ebp - 8], ax
        mov    al, byte ptr [ebp - 3]
        shr    al, 4
        xor    esi, esi
        movzx  eax, al
        and    al, 0xfc
        lea    edi, [eax]                    ; -> loc_1_11DC
        mov    al, byte ptr [ebp - 3]
        and    al, 7
        mov    byte ptr [ebp - 5], al
        cmp    byte ptr [edi], al
        je     loc_1_0F3F
        cmp    byte ptr [edi + 1], al
        jne    loc_1_0F2E
        push   esi
        lea    eax, [ebp - 1]
        push   1
        push   eax
        push   ebx
        call   _VWIN32_CopyMem
loc_1_0EE2:
        add    esp, 0x10
        test   eax, eax
        je     loc_1_0FAA
        inc    ebx
        mov    al, byte ptr [ebp - 1]
        and    al, 0x38
        shr    al, 3
        movzx  ecx, al
        mov    dword ptr [ebp - 0xc], ecx
        cmp    ecx, 4
        je     loc_1_0F16
        movzx  edx, byte ptr [ecx]           ; -> loc_1_11CC
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx + ecx]
        mov    cl, byte ptr [ebp - 1]
        shr    cl, 6
        shl    esi, cl
loc_1_0F16:
        mov    al, byte ptr [ebp - 1]
        mov    ecx, dword ptr [ebp + 8]
        and    eax, 7
        movzx  edx, byte ptr [eax]           ; -> loc_1_11CC
        mov    dword ptr [ebp - 0xc], eax
        add    esi, dword ptr [edx + ecx]
        jmp    loc_1_0F3F
loc_1_0F2E:
        movzx  eax, byte ptr [ebp - 5]
        mov    ecx, dword ptr [ebp + 8]
        movzx  edx, byte ptr [eax]           ; -> loc_1_11CC
        mov    esi, dword ptr [edx + ecx]
loc_1_0F3F:
        mov    edx, 1
        mov    cl, byte ptr [ebp - 5]
        shl    edx, cl
        movzx  eax, byte ptr [edi + 2]
        test   edx, eax
        je     loc_1_0F8F
        cmp    byte ptr [edi + 3], 4
        push   0
        jne    loc_1_0F71
        push   4
        lea    eax, [ebp - 0xc]
        push   eax
        push   ebx
        call   _VWIN32_CopyMem
loc_1_0F65:
        add    esp, 0x10
        test   eax, eax
        je     loc_1_0FAA
        add    ebx, 4
        jmp    loc_1_0F8C
loc_1_0F71:
        push   1
        lea    eax, [ebp - 1]
        push   eax
        push   ebx
        call   _VWIN32_CopyMem
loc_1_0F7D:
        add    esp, 0x10
        test   eax, eax
        je     loc_1_0FAA
        movsx  eax, byte ptr [ebp - 1]
        inc    ebx
        mov    dword ptr [ebp - 0xc], eax
loc_1_0F8C:
        add    esi, dword ptr [ebp - 0xc]
loc_1_0F8F:
        push   0
        lea    eax, [ebp - 8]
        push   2
        push   esi
        push   eax
        call   _VWIN32_CopyMem
loc_1_0F9D:
        add    esp, 0x10
        test   eax, eax
        je     loc_1_0FAA
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x24], ebx
loc_1_0FAA:
        mov    eax, 1
loc_1_0FAF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0FB6:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [0]            ; -> loc_1_120C
        mov    ecx, edx
        push   0x40
        and    ecx, 0xfffcffff
        and    edx, 0x3ffff
        shr    ecx, 0x10
        shl    edx, 0xc
        mov    ecx, dword ptr [ecx + eax]
        movzx  eax, word ptr [0]             ; -> obj1:0x10D4
        push   eax
        movzx  eax, word ptr [0]             ; -> obj1:0x10D6
        push   eax
        mov    eax, dword ptr [ecx + 0x10]
        sub    eax, edx
        cmp    eax, 0x1000
        jb     loc_1_0FFB
        mov    eax, 0x1000
loc_1_0FFB:
        push   eax
        push   dword ptr [esp + 0x18]
        push   edx
        push   dword ptr [ecx + 0x24]
        call   sub_1_036A
        add    esp, 0x1c
        ret    
        db  000h, 000h, 000h
        dd  VWIN32_Get_Version       ; fixup
        dd  VWIN32_DIOCCompletionRoutine ; fixup
        dd  _VWIN32_QueueUserApc     ; fixup
        dd  _VWIN32_Get_Thread_Context ; fixup
        dd  _VWIN32_Set_Thread_Context ; fixup
        dd  _VWIN32_CopyMem          ; fixup
        dd  _VWIN32_Npx_Exception    ; fixup
        dd  _VWIN32_Emulate_Npx      ; fixup
        dd  _VWIN32_CheckDelayedNpxTrap ; fixup
        dd  VWIN32_EnterCrstR0       ; fixup
        dd  VWIN32_LeaveCrstR0       ; fixup
        dd  _VWIN32_FaultPopup       ; fixup
        dd  VWIN32_GetContextHandle  ; fixup
        dd  VWIN32_GetCurrentProcessHandle ; fixup
        dd  _VWIN32_SetWin32Event    ; fixup
        dd  _VWIN32_PulseWin32Event  ; fixup
        dd  _VWIN32_ResetWin32Event  ; fixup
        dd  _VWIN32_WaitSingleObject ; fixup
        dd  _VWIN32_WaitMultipleObjects ; fixup
        dd  _VWIN32_CreateRing0Thread ; fixup
        dd  _VWIN32_CloseVxDHandle   ; fixup
        dd  VWIN32_ActiveTimeBiasSet ; fixup
        dd  VWIN32_GetCurrentDirectory ; fixup
        dd  VWIN32_BlueScreenPopup   ; fixup
        dd  VWIN32_TerminateApp      ; fixup
        dd  _VWIN32_QueueKernelAPC   ; fixup
        dd  VWIN32_SysErrorBox       ; fixup
        dd  _VWIN32_IsClientWin32    ; fixup
        dd  VWIN32_IFSRIPWhenLev2Taken ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0002Ah                      ; DDB_Req_Device_Number
        db  1, 3                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VWIN32  '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  VWIN32_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  VWIN32_PM_API            ; DDB_PM_API_Proc
        dd  obj1:0x1010              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_1120:
        and    al, 0x11
        add    byte ptr [eax], al
loc_1_1124:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1150:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_118C:
        js     loc_1_1192
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1192:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        idiv   dword ptr [edi]
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_11CC:
        sbb    al, 0x18
        adc    al, 0x10
        xor    byte ptr [eax], cl
        add    al, 0
loc_1_11D4:
        sbb    al, 0x18
        adc    al, 0x10
        db  01Dh, 019h, 015h, 011h
loc_1_11DC:
        add    eax, 0xff042004
        add    al, 0xff
        add    edi, edi
        add    al, 0xff
        add    al, 0xff
        inc    dword ptr [eax]
        add    bl, bl
loc_1_11ED:
        jecxz  loc_1_11EF
loc_1_11EF:
        add    ebx, ebx
        loop   loc_1_11F3
loc_1_11F3:
        add    ecx, ebx
        push   es
        add    dword ptr [ecx], eax
        fld    qword ptr [esi]
        add    dword ptr [ecx], eax
        fld    dword ptr [edi]
        add    dword ptr [edx], eax
        fld    qword ptr [edi]
        add    dword ptr [ebx], eax
        fnstsw ax
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        db  001h
loc_1_120C:
        lodsd  eax, dword ptr [esi]
        sti    
        db  0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x23e  R-X
loc_2_0000:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [ecx], al
        sub    al, byte ptr [edx]
        add    byte ptr [eax], al
loc_2_0014:
        inc    ecx
        and    byte ptr [esi + 0x61], ah
        je     0x7b
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + 0x78], ah
        arpl   word ptr [ebp + 0x70], sp
        je     loc_2_008C
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [0x20783231], ah
        push   0x6f207361
        arpl   word ptr [ebx + 0x75], sp
        jb     0xa7
        and    byte ptr fs:[ecx + 0x74], ah
        and    byte ptr [0x3a783432], ah
        and    eax, 0x2e783833
        and    byte ptr [eax], ah
        push   esp
        push   0x75632065
        jb     0xc1
        outsb  dx, byte ptr gs:[esi]
        je     0x73
        popal  
        jo     loc_2_00C6
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr [edi + 0x69], dh
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [edx + 0x65], ah
        and    byte ptr [ebp + 0x72], dh
        insd   dword ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x61], 0x2e646574
        or     eax, 0x202a0a0a
        and    byte ptr [eax + 0x72], dl
        jae    loc_2_00F0
        and    byte ptr [ecx + 0x6e], ah
        jns    loc_2_00A2
        imul   esp, dword ptr [ebp + 0x79], 0x20
        je     0xf7
        and    byte ptr [ebp + 0x72], dh
loc_2_008C:
        insd   dword ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x61], 0x74206574
        push   0x75632065
        jb     loc_2_010D
        outsb  dx, byte ptr gs:[esi]
        je     0xbf
        popal  
        jo     0x112
loc_2_00A2:
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        or     eax, 0x20202a0a
        push   eax
        jb     0x118
        jae    loc_2_0128
        and    byte ptr [ebx + 0x54], al
        push   edx
        dec    esp
        sub    eax, dword ptr [ecx + 0x4c]
        push   esp
        sub    eax, dword ptr [ebp + eax*2 + 0x4c]
        and    byte ptr [ecx + 0x67], ah
        popal  
loc_2_00C6:
        imul   ebp, dword ptr [esi + 0x20], 0x72206f74
        jae    0x144
        popal  
        jb     0x147
        and    byte ptr [ecx + 0x6f], bh
        jne    loc_2_014A
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        jo     0x153
        je     loc_2_0145
        jb     loc_2_0110
        and    byte ptr [ecx + 0x6f], bl
        jne    0x107
        ja     0x152
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        or     eax, 0x2020080a
loc_2_00F0:
        and    byte ptr [edi + ebp*2 + 0x73], ch
        and    byte ptr gs:[ecx + 0x6e], ah
        jns    0x11a
        jne    0x16a
        jae    0x15f
        jbe    0x165
        and    byte ptr fs:[ecx + 0x6e], ch
        outsw  dx, word ptr [esi]
        jb     0x175
        popal  
        je     0x174
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_2_010D:
        and    byte ptr [ecx + 0x6e], ch
loc_2_0110:
        and    byte ptr [ecx + 0x6c], ah
        insb   byte ptr es:[edi], dx
        and    byte ptr [ecx + 0x70], ah
        jo     loc_2_0185
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        jae    loc_2_0150
        add    byte ptr [eax], al
        inc    ecx
        and    byte ptr [esi + 0x61], ah
loc_2_0128:
        je     0x18b
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + 0x78], ah
        arpl   word ptr [ebp + 0x70], sp
        je     loc_2_019C
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [0x20783231], ah
        push   0x6f207361
        arpl   word ptr [ebx + 0x75], sp
        jb     loc_2_01B7
loc_2_0145:
        and    byte ptr fs:[ecx + 0x74], ah
loc_2_014A:
        and    byte ptr [0x3a783432], ah
loc_2_0150:
        and    eax, 0x20783833
        imul   ebp, dword ptr [esi + 0x20], 0x20445856
        and    eax, 0x202e7334
        push   esp
        push   0x75632065
        jb     0x1db
        outsb  dx, byte ptr gs:[esi]
        je     0x18d
        popal  
        jo     loc_2_01E0
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr [edi + 0x69], dh
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [edx + 0x65], ah
        and    byte ptr [ebp + 0x72], dh
        insd   dword ptr es:[edi], dx
loc_2_0185:
        imul   ebp, dword ptr [esi + 0x61], 0x2e646574
        or     eax, 0x202a0a0a
        and    byte ptr [eax + 0x72], dl
        jae    loc_2_020A
        and    byte ptr [ecx + 0x6e], ah
        jns    loc_2_01BC
loc_2_019C:
        imul   esp, dword ptr [ebp + 0x79], 0x20
        je     0x211
        and    byte ptr [ebp + 0x72], dh
        insd   dword ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x61], 0x74206574
        push   0x75632065
        jb     loc_2_0227
        outsb  dx, byte ptr gs:[esi]
loc_2_01B7:
        je     0x1d9
        popal  
        jo     0x22c
loc_2_01BC:
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        or     eax, 0x20202a0a
        push   eax
        jb     0x232
        jae    0x242
        and    byte ptr [ebx + 0x54], al
        push   edx
        dec    esp
        sub    eax, dword ptr [ecx + 0x4c]
        push   esp
        sub    eax, dword ptr [ebp + eax*2 + 0x4c]
        and    byte ptr [ecx + 0x67], ah
        popal  
loc_2_01E0:
        imul   ebp, dword ptr [esi + 0x20], 0x72206f74
        jae    0x25e
        popal  
        jb     0x261
        and    byte ptr [ecx + 0x6f], bh
        jne    0x264
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        jo     0x26d
        je     0x25f
        jb     loc_2_022A
        and    byte ptr [ecx + 0x6f], bl
        jne    0x221
        ja     0x26c
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        or     eax, 0x2020080a
loc_2_020A:
        and    byte ptr [edi + ebp*2 + 0x73], ch
        and    byte ptr gs:[ecx + 0x6e], ah
        jns    0x234
        jne    0x284
        jae    0x279
        jbe    0x27f
        and    byte ptr fs:[ecx + 0x6e], ch
        outsw  dx, word ptr [esi]
        jb     0x28f
        popal  
        je     0x28e
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_2_0227:
        and    byte ptr [ecx + 0x6e], ch
loc_2_022A:
        and    byte ptr [ecx + 0x6c], ah
        insb   byte ptr es:[edi], dx
        and    byte ptr [ecx + 0x70], ah
        jo     0x29f
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        jae    0x26a
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x354  R-X
loc_3_0000:
        mov    esi, 0x284                    ; -> obj5:0x0284
        VMMCall  VMM.Install_Exception_Handler   ; cd20ef000100
        mov    ecx, 0x11
        sub    eax, eax
loc_3_0012:
        mov    esi, dword ptr [eax*4 + 0x368] ; -> sub_5_0368
        or     esi, esi
        je     loc_3_0035
        VMMCall  VMM.Hook_PM_Fault               ; cd2080000100
        jb     loc_3_0043
        or     esi, esi
        jne    loc_3_002E
        mov    esi, 0xf0f                    ; -> loc_4_0F0F
loc_3_002E:
        mov    dword ptr [eax*4 + 0x368], esi ; -> sub_5_0368
loc_3_0035:
        inc    eax
        loop   loc_3_0012
        mov    dword ptr [0x384], 0xf0f      ; -> loc_5_0384
        clc    
loc_3_0043:
        ret    
loc_3_0044:
        VMMCall  VMM.Get_VMM_Version             ; cd2000000100
        cmp    ax, 0x400
        jb     loc_3_0208
        mov    esi, 0x11c8                   ; -> loc_4_11C8
        VMMCall  VMM.Hook_Invalid_Page_Fault     ; cd20e9000100
        VMMCall  VMM._AllocateThreadDataSlot     ; cd20ff000100
        test   eax, eax
        jne    loc_3_006E
        jmp    loc_3_0208
loc_3_006E:
        mov    dword ptr [0x10ec], eax       ; -> obj1:0x10EC
        push   0
        push   0
        VMMCall  VMM._CreateMutex                ; cd2001010100
        add    esp, 8
        or     eax, eax
        jne    loc_3_0089
        jmp    loc_3_0208
loc_3_0089:
        mov    dword ptr [0x10f8], eax       ; -> obj1:0x10F8
        push   2
        push   0
        VMMCall  VMM._CreateMutex                ; cd2001010100
        add    esp, 8
        or     eax, eax
        jne    loc_3_00A4
        jmp    loc_3_0208
loc_3_00A4:
        mov    dword ptr [0x10fc], eax       ; -> obj1:0x10FC
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp + 0x1c], 0x5200
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  eax, word ptr [ebp + 0x38]
        push   eax
        mov    word ptr [ebp + 0x1c], 0xea00
        and    word ptr [ebp + 0x10], 0
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  eax, word ptr [ebp + 0x10]
        or     eax, eax
        je     loc_3_0155
        mov    al, 1
        mov    byte ptr [0x2c8], al          ; -> obj5:0x02C8
        mov    word ptr [ebp + 0x1c], 0x58d4
        and    word ptr [ebp + 0x18], 0
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    word ptr [ebp + 0x18], 0x3fcf
        jne    loc_3_0131
        cmp    word ptr [ebp + 0x14], 2
        jne    loc_3_0131
        mov    ax, word ptr [ebp + 0x1c]
        cmp    al, 1
        jne    loc_3_0131
        sub    al, ah
        jne    loc_3_0131
        mov    byte ptr [0x2c8], al          ; -> obj5:0x02C8
        jmp    loc_3_0155
loc_3_0131:
        mov    word ptr [ebp + 0x1c], 0x7a20
        and    word ptr [ebp + 0x10], 0
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  eax, word ptr [ebp + 0x1c]
        or     eax, eax
        jne    loc_3_0155
        inc    dword ptr [0x2c8]             ; -> obj5:0x02C8
loc_3_0155:
        pop    eax
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        shl    eax, 4
        mov    ecx, dword ptr [ebx + 4]
        lea    eax, [ecx + eax + 0x330]
        mov    dword ptr [0x10f0], eax       ; -> obj1:0x10F0
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        add    edi, dword ptr [0x10ec]       ; -> obj1:0x10EC
        push   0
        push   0
        push   0
        push   0
        call   sub_4_255A
        or     eax, eax
        je     loc_3_0208
        mov    dword ptr [edi], eax
        mov    dword ptr [0x10f4], eax       ; -> obj1:0x10F4
        mov    dword ptr [0x10e0], eax       ; -> obj1:0x10E0
        push   0                             ; -> obj5:0x0000
        push   0x1084                        ; -> obj1:0x1084
        VMMCall  VMM._Register_Win32_Services    ; cd2029010100
        add    esp, 8
        mov    esi, 0x10f7                   ; -> loc_4_10F7
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        jb     loc_3_0208
        mov    dword ptr [0x344], eax        ; -> loc_5_0344
        mov    edx, 8
        mov    esi, 0x1186                   ; -> sub_4_1186
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        jb     loc_3_0208
        mov    dword ptr [0x348], eax        ; -> loc_5_0348
        mov    esi, 0x2676                   ; -> loc_4_2676
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        jb     loc_3_0208
        mov    dword ptr [0x2bc], eax        ; -> obj5:0x02BC
        mov    esi, 0x13ca                   ; -> loc_4_13CA
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        jb     loc_3_0208
        mov    dword ptr [0x2c0], eax        ; -> obj5:0x02C0
        clc    
        ret    
loc_3_0208:
        stc    
        ret    
loc_3_020A:
        VxDCall  PAGEFILE.PageFile_Get_Version   ; cd2000002100
        or     eax, eax
        je     loc_3_0219
        cmp    bl, 3
        je     loc_3_0220
loc_3_0219:
        or     dword ptr [0x111c], 2         ; -> obj1:0x111C
loc_3_0220:
        call   sub_3_022C
        mov    dword ptr [0x280], eax        ; -> obj5:0x0280
        clc    
        ret    
sub_3_022C:
        mov    esi, 0x118c                   ; -> loc_1_118C
        mov    ecx, 1
loc_3_0236:
        push   ecx
        mov    edx, dword ptr [esi]
        mov    edi, 0x2d4                    ; -> obj3:0x02D4
        VMMCall  VMM.OpenFile                    ; cd20b8000100
        jb     loc_3_02A3
        mov    ebx, eax
        mov    ah, 0x3e
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    edi, 0x2d4                    ; -> obj3:0x02D4
        mov    ecx, 0x80
        sub    eax, eax
        cld    
        repne scasb al, byte ptr es:[edi]
        sub    ecx, 0x80
        neg    ecx
        mov    ebx, ecx
        push   0
        push   ecx
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_3_02A3
        mov    edx, esi
        mov    ecx, ebx
        mov    esi, 0x2d4                    ; -> obj3:0x02D4
        mov    edi, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, bl
        and    cl, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, edx
        mov    dword ptr [esi + 4], eax
        pop    ecx
        add    esi, 0x10
        loop   loc_3_0236
        mov    eax, 1
loc_3_02A2:
        ret    
loc_3_02A3:
        add    esp, 4
        mov    esi, 0x118c                   ; -> loc_1_118C
        mov    ebx, 1
loc_3_02B0:
        cmp    dword ptr [esi + 4], 0
        je     loc_3_02CE
        push   0
        push   dword ptr [esi + 4]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    dword ptr [esi + 4], 0
        dec    ebx
        jne    loc_3_02B0
loc_3_02CE:
        sub    eax, eax
        jmp    loc_3_02A2
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x49af  R-X
loc_4_0000:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_0CC9
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_4_0014:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_1_05A1
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x30
loc_4_0028:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_0CCF
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_4_003C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2488
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_4_0050:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_24D0
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_4_0064:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_24DC
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x14
loc_4_0078:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_254A
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_4_008C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_25FC
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x2c
loc_4_00A0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_1_0813
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_4_00B4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_1_0836
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_4_00C8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2782
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_4_00DC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_0D3B
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_4_00F0:
        pop    edi
        pop    esi
        pop    ebx
        call   _VWIN32_QueueUserApc
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x14
loc_4_0104:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_1_0B6C
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_4_0118:
        pop    edi
        pop    esi
        pop    ebx
        call   _VWIN32_QueueKernelAPC
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x18
loc_4_012C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_0B33
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x20
loc_4_0140:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_1_0790
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_4_0154:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2C75
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_4_0168:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_1_06E4
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_4_017C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_1_073E
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_4_0190:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_1_0CB9
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_4_01A4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2C8C
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_4_01B8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2C9F
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x14
loc_4_01CC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2CDB
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x18
loc_4_01E0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2CF6
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_4_01F4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2D09
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_4_0208:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2D17
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_4_021C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2CB5
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_4_0230:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2CC8
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_4_0244:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_1_0608
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_4_0258:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_26AD
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_4_026C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_1_0CF0
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_4_0280:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_3C17
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x14
loc_4_0294:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_3CC0
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x1c
loc_4_02A8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_3DA0
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_4_02BC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_3A08
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_4_02D0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2947
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_4_02E4:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2F66
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_4_02F8:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_2D2A
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_4_030C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_1_0956
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_4_0320:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_251A
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_4_0334:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_18BF
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_4_0348:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_1A5C
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_4_035C:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_0B50
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0xc
loc_4_0370:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_1_0985
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_4_0384:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_0B65
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    8
loc_4_0398:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_449B
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x10
loc_4_03AC:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_44ED
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x14
loc_4_03C0:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_4_4513
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x14], edx
        push   ebx
        push   esi
        push   edi
        ret    0x14
        db  000h, 000h, 000h, 000h
        dd  loc_4_090F               ; fixup
        dd  loc_4_0927               ; fixup
        dd  loc_4_0908               ; fixup
loc_4_03E4:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0x10]
        call   _VWIN32_WaitSingleObject
        pop    ebx
        leave  
        ret    
loc_4_03F9:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        call   _VWIN32_SetWin32Event
        pop    ebx
        leave  
        ret    
loc_4_0408:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        call   sub_4_2C1E
        pop    ebx
        leave  
        ret    
loc_4_0417:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        call   sub_4_2BAF
        pop    ebx
        leave  
        ret    
loc_4_0426:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   1
        push   dword ptr [ebp + 8]
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        add    esp, 8
        pop    ebx
        leave  
        ret    
loc_4_043B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   dword ptr [ebp + 8]
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
        pop    ebx
        leave  
        ret    
loc_4_044E:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_4_0505
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [edi + 0x2c], 0x2a
        push   1
        push   0
        VMMCall  VMM._CreateMutex                ; cd2001010100
        add    esp, 8
        mov    dword ptr [0x11b6], eax       ; -> obj1:0x11B6
        push   1
        push   0
        VMMCall  VMM._CreateMutex                ; cd2001010100
        add    esp, 8
        mov    dword ptr [0x11b2], eax       ; -> obj1:0x11B2
        push   ebx
        mov    ah, 0x51
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        movzx  eax, bx
        mov    dword ptr [0x119e], eax       ; -> obj1:0x119E
        pop    ebx
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        shr    eax, 4
        mov    dword ptr [0x11a2], eax       ; -> obj1:0x11A2
        mov    eax, dword ptr [0x280]        ; -> obj5:0x0280
        or     eax, eax
        je     loc_4_04C8
        call   sub_4_20B4
loc_4_04C8:
        mov    dword ptr [0x294], eax        ; -> obj5:0x0294
        or     eax, eax
        je     loc_4_0505
        call   sub_4_2F8B
        push   0x10
        push   1
        push   0x80080000
        VMMCall  VMM._PageReserve                ; cd201d010100
        add    esp, 0xc
        shr    eax, 0xc
        mov    dword ptr [0x2c4], eax        ; -> obj5:0x02C4
        push   0x20060000
        push   0
        push   2
        push   1
        push   eax
        VMMCall  VMM._PageCommit                 ; cd201e010100
        add    esp, 0x14
loc_4_0505:
        clc    
        ret    
loc_4_0507:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_4_0578
        cmp    dword ptr [0x294], 0          ; -> obj5:0x0294
        je     loc_4_0578
        mov    esi, 0x11c8                   ; -> loc_4_11C8
        VMMCall  VMM.Unhook_Invalid_Page_Fault   ; cd20ea000100
        push   0x10
        push   0xbff70000
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        call   sub_4_245A
        mov    eax, dword ptr [0x11b2]       ; -> obj1:0x11B2
        or     eax, eax
        je     loc_4_054B
        push   eax
        VMMCall  VMM._DestroyMutex               ; cd2002010100
        add    esp, 4
loc_4_054B:
        mov    eax, dword ptr [0x11b6]       ; -> obj1:0x11B6
        or     eax, eax
        je     loc_4_055E
        push   eax
        VMMCall  VMM._DestroyMutex               ; cd2002010100
        add    esp, 4
loc_4_055E:
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        add    edi, dword ptr [0x10ec]       ; -> obj1:0x10EC
        sub    eax, eax
        xchg   dword ptr [edi], eax
        or     eax, eax
        je     loc_4_0578
        push   eax
        call   sub_4_25E4
loc_4_0578:
        mov    eax, 1
        clc    
        ret    
VWIN32_PM_API:
        movzx  eax, byte ptr [ebp + 0x1d]
        cmp    eax, 0x15
        ja     loc_4_058F
        jmp    dword ptr [eax*4 + 0x2d0]     ; -> obj5:0x02D0
loc_4_058F:
        jmp    loc_4_0597
loc_4_0591:
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_4_0597:
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_4_059D:
        mov    eax, 0x103
        mov    dword ptr [ebp + 0x1c], eax
        mov    eax, dword ptr [0x294]        ; -> obj5:0x0294
        mov    dword ptr [ebp + 0x14], eax
        jmp    loc_4_0591
loc_4_05AF:
        mov    edi, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x18]
        call   sub_4_05BF
        mov    dword ptr [ebp + 0x1c], eax
        jmp    loc_4_0591
sub_4_05BF:
        mov    dword ptr [0x1114], edi       ; -> obj1:0x1114
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
        mov    edx, edi
        add    edx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    edx, dword ptr [edx]
        cmp    dword ptr [edx + 0x1c], 0
        jl     loc_4_05E4
        cmp    dword ptr [edx + 0x14], 0x101
        je     loc_4_05F0
loc_4_05E4:
        push   ecx
        push   edi
        call   sub_1_0836
        add    esp, 8
        jmp    loc_4_05F3
loc_4_05F0:
        mov    dword ptr [edx + 0x14], ecx
loc_4_05F3:
        push   -1
        call   sub_1_0813
        add    esp, 4
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    edx, edi
        add    edx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    edx, dword ptr [edx]
        xor    eax, eax
        xchg   dword ptr [edx + 0x14], eax
        cmp    eax, 0x101
        jne    loc_4_062D
        mov    cx, word ptr [0x110a]         ; -> obj1:0x110A
        movzx  edx, word ptr [0x1108]        ; -> obj1:0x1108
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
loc_4_062D:
        ret    
loc_4_062E:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        jmp    loc_4_05F3
loc_4_0636:
        push   ds
        mov    ds, word ptr [ebp + 0x34]
        mov    esi, dword ptr [ebp + 0x30]
        mov    ecx, dword ptr [ebp + 0x18]
        sub    esp, ecx
        shr    ecx, 2
        mov    edi, esp
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        push   ss
        pop    ds
        mov    al, byte ptr [ebp + 0x1c]
        cmp    al, 1
        ja     loc_4_066E
        jne    loc_4_065E
        VMMCall  VMM._PageCommit                 ; cd201e010100
        jmp    loc_4_0678
loc_4_065E:
        VMMCall  VMM._PageReserve                ; cd201d010100
        jmp    loc_4_0678
loc_4_0666:
        VMMCall  VMM._PageFree                   ; cd2055000100
        jmp    loc_4_0678
loc_4_066E:
        cmp    al, 2
        ja     loc_4_0666
        VMMCall  VMM._PageDecommit               ; cd201f010100
loc_4_0678:
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ebp + 0x30], esi
        add    esp, ecx
        pop    ds
        mov    dword ptr [ebp + 0x1c], eax
        ret    
loc_4_0685:
        mov    eax, dword ptr [0x2c8]        ; -> obj5:0x02C8
        mov    word ptr [ebp + 0x1c], ax
        jmp    loc_4_0591
loc_4_0693:
        push   dword ptr [ebp + 0x10]
        call   sub_1_0956
        add    esp, 4
        mov    dword ptr [ebp + 0x1c], eax
        jmp    loc_4_0591
loc_4_06A6:
        push   dword ptr [ebp + 0x10]
        call   sub_1_0924
        add    esp, 4
        mov    dword ptr [ebp + 0x1c], eax
        jmp    loc_4_0591
loc_4_06B9:
        call   sub_1_08B8
        mov    dword ptr [ebp + 0x1c], eax
        jmp    loc_4_0591
loc_4_06C6:
        push   dword ptr [ebp + 0x10]
        call   sub_1_08F7
        add    esp, 4
        mov    dword ptr [ebp + 0x1c], eax
        jmp    loc_4_0591
loc_4_06D9:
        mov    ebx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x18]
        push   ecx
        push   ebx
        call   sub_4_2C75
        add    esp, 8
        jmp    loc_4_0591
loc_4_06EE:
        push   dword ptr [0x10d8]            ; -> obj1:0x10D8
        push   dword ptr [ebp + 0x10]
        call   sub_4_2991
        add    esp, 8
        jmp    loc_4_0591
loc_4_0704:
        push   dword ptr [0x10d8]            ; -> obj1:0x10D8
        push   dword ptr [ebp + 0x10]
        call   sub_4_2ADF
        add    esp, 8
        jmp    loc_4_0591
loc_4_071A:
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x10]
        call   sub_4_2CC8
        add    esp, 8
        jmp    loc_4_0591
loc_4_072D:
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x10]
        call   sub_4_2CB5
        add    esp, 8
        jmp    loc_4_0591
loc_4_0740:
        mov    edi, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [ebp + 0x14]
        VMMCall  VMM.Boost_With_Decay            ; cd2062010100
        jmp    loc_4_0591
loc_4_0754:
        VMMCall  VMM.Time_Slice_Sys_VM_Idle      ; cd2060010100
        jmp    loc_4_0591
loc_4_075F:
        VMMCall  VMM.Time_Slice_Wake_Sys_VM      ; cd2084010100
        jmp    loc_4_0591
loc_4_076A:
        mov    ax, word ptr [ebp + 0x38]
        mov    word ptr [0x364], ax          ; -> loc_5_0364
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x14]
        mov    dword ptr [0x340], eax        ; -> loc_5_0340
        mov    ax, word ptr [ebp + 0x10]
        mov    dword ptr [0x2b8], eax        ; -> obj5:0x02B8
        mov    ax, word ptr [ebp]
        mov    dword ptr [0x350], eax        ; -> loc_5_0350
        mov    ax, word ptr [ebp + 8]
        mov    dword ptr [0x354], eax        ; -> loc_5_0354
        mov    ax, word ptr [ebp + 0x18]
        mov    dword ptr [0x358], eax        ; -> loc_5_0358
        movzx  eax, word ptr [ebp + 0x3c]
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_4_0597
        mov    edx, dword ptr [ebp + 4]
        movzx  ecx, dx
        add    ecx, eax
        mov    dword ptr [0x2b0], ecx        ; -> obj5:0x02B0
        shr    edx, 0x10
        add    edx, eax
        mov    dword ptr [0x2b4], edx        ; -> obj5:0x02B4
        mov    eax, dword ptr [0x2c0]        ; -> obj5:0x02C0
        mov    dword ptr [ebp + 0x1c], eax
        jmp    loc_4_0591
loc_4_07E4:
        VxDCall  IFSMgr.IFSMgr_GetConversionTablePtrs ; cd2051004000
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        jmp    loc_4_0591
loc_4_07FA:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x18]
        test   ebx, ebx
        jne    loc_4_090F
        or     ebx, dword ptr [0x110c]       ; -> obj1:0x110C
        jne    loc_4_0830
        xchg   dword ptr [esp + 0x1c], ebx
        xchg   dword ptr [0x110c], ebx       ; -> obj1:0x110C
        xchg   dword ptr [esp + 0x34], ebx
        xchg   dword ptr [0x1110], ebx       ; -> obj1:0x1110
        mov    esi, 0x3d4                    ; -> obj4:0x03D4
        VMMCall  VMM.Install_Exception_Handler   ; cd20ef000100
loc_4_0830:
        mov    eax, dword ptr [esp + 0x28]
        inc    eax
        je     loc_4_08F2
        mov    ecx, dword ptr [esp + 0x24]
        jecxz  loc_4_088F
        VMMCall  VMM.VMM_GetDDBList              ; cd203f010100
        mov    ebx, eax
loc_4_0849:
        test   ebx, ebx
        je     loc_4_0904
        mov    esi, dword ptr [esp + 0x20]
        lea    edi, [ebx + 0xc]
        mov    ecx, 8
loc_4_085D:
        lodsb  al, byte ptr [esi]
        or     al, al
        jne    loc_4_0869
        cmp    byte ptr [edi], 0x20
        je     loc_4_08BF
        jmp    loc_4_088B
loc_4_0869:
        cmp    al, 0x61
        jb     loc_4_0873
        cmp    al, 0x7a
        ja     loc_4_0873
        and    al, 0xdf
loc_4_0873:
        mov    dl, byte ptr [edi]
        cmp    dl, 0x61
        jb     loc_4_0882
        cmp    dl, 0x7a
        ja     loc_4_0882
        and    dl, 0xdf
loc_4_0882:
        cmp    al, dl
        jne    loc_4_088B
        inc    edi
        loop   loc_4_085D
        jmp    loc_4_08BF
loc_4_088B:
        mov    ebx, dword ptr [ebx]
        jmp    loc_4_0849
loc_4_088F:
        mov    al, 1
        mov    edx, dword ptr [esp + 0x20]
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_4_0904
        mov    ebx, eax
        lea    esi, [eax + 0xc]
        mov    ecx, 8
        mov    edi, dword ptr [esp + 0x28]
        mov    dword ptr [edi], 0x5c2e5c5c
        add    edi, 4
loc_4_08B4:
        lodsb  al, byte ptr [esi]
        cmp    al, 0x20
        je     loc_4_08BC
        stosb  byte ptr es:[edi], al
        loop   loc_4_08B4
loc_4_08BC:
        xor    al, al
        stosb  byte ptr es:[edi], al
loc_4_08BF:
        mov    edi, dword ptr [esp + 0x40]
        lea    esi, [ebx + 0xc]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [esp + 0x18], ebx
        mov    eax, 0x23
        mov    ecx, 0
        mov    dword ptr [esp + 0x1c], ecx
        xor    edx, edx
        lea    esi, [esp + 0x10]
        push   ebx
        call   dword ptr [ebx + 0x18]
        pop    ebx
        test   eax, eax
        mov    eax, ebx
        je     loc_4_093B
        mov    eax, 0xffffffff
        jmp    loc_4_093B
loc_4_08F2:
        mov    edx, dword ptr [esp + 0x20]
        xor    ebx, ebx
        VxDCall  APM.Service_0002h               ; cd2002002700
        jb     loc_4_093B
        xor    eax, eax
        jmp    loc_4_093B
loc_4_0904:
        xor    eax, eax
        jmp    loc_4_093B
loc_4_0908:
        mov    eax, 0x37
        jmp    loc_4_093B
loc_4_090F:
        or     dl, byte ptr [ebx + 0x18]
        mov    eax, dword ptr [ebx + 0x18]
        or     dl, byte ptr [eax]
        mov    esi, dword ptr [esp + 0x40]
        lea    edi, [ebx + 0xc]
        mov    ecx, 8
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_0908
loc_4_0927:
        mov    eax, 0x23
        mov    ecx, dword ptr [esp + 0x1c]
        mov    edx, dword ptr [esp + 0x38]
        lea    esi, [esp + 0x10]
        call   dword ptr [ebx + 0x18]
loc_4_093B:
        mov    edi, dword ptr [esp + 0x10]
        mov    dword ptr [edi + 0x1c], eax
        pop    edi
        pop    esi
        pop    ebx
        ret    0x34
loc_4_0948:
        xor    eax, eax
        mov    ecx, 0x103
        ret    
loc_4_0950:
        mov    ecx, dword ptr [esi + 0xc]
        jecxz  loc_4_0948
        inc    ecx
        jecxz  loc_4_09A2
        dec    ecx
        mov    edx, 0x21
        dec    ecx
        jecxz  loc_4_09DE
        mov    edx, 0x25
        dec    ecx
        jecxz  loc_4_09DE
        mov    edx, 0x26
        dec    ecx
        jecxz  loc_4_09DE
        mov    edx, 0x13
        dec    ecx
        jecxz  loc_4_09DE
        dec    ecx
        jecxz  loc_4_09A9
        mov    edx, 0x21
        dec    ecx
        jecxz  loc_4_09DE
        mov    eax, 0x32
        jmp    loc_4_0B32
loc_4_098E:
        mov    eax, 0x7a
        jmp    loc_4_0B32
loc_4_0998:
        mov    eax, 0x57
        jmp    loc_4_0B32
loc_4_09A2:
        xor    eax, eax
        jmp    loc_4_0B32
loc_4_09A9:
        mov    eax, dword ptr [esi + 0x1c]
        or     eax, dword ptr [esi + 0x18]
        or     eax, dword ptr [esi + 0x20]
        or     eax, dword ptr [esi + 0x24]
        jne    loc_4_0998
        cmp    dword ptr [esi + 0x14], 4
        jne    loc_4_0998
        mov    eax, dword ptr [esi + 0x10]
        test   eax, eax
        je     loc_4_0998
        push   -1
        push   dword ptr [eax]
        push   dword ptr [0x1118]            ; -> obj1:0x1118
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
        xor    eax, eax
        jmp    loc_4_0B32
loc_4_09DE:
        cmp    dword ptr [esi + 0x14], 0x1c
        jb     loc_4_098E
        cmp    dword ptr [esi + 0x1c], 0x1c
        jb     loc_4_098E
        mov    ecx, dword ptr [esi + 0x18]
        jecxz  loc_4_0998
        mov    ecx, dword ptr [esi + 0x10]
        jecxz  loc_4_0998
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        push   esi
        mov    edi, dword ptr [esi]
        push   edi
        lea    edi, [edi + 0x10]
        mov    esi, dword ptr [esi + 0x10]
        cld    
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        pop    edi
        push   edi
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        pop    edi
        cmp    edx, 0x21
        jne    loc_4_0AF4
        pop    esi
        push   esi
        cmp    dword ptr [esi + 0xc], 6
        je     loc_4_0AF0
        mov    byte ptr [edi + 0x1d], 0x44
        cmp    byte ptr [edi + 0x1c], 0xd
        jne    loc_4_0AF4
        mov    ecx, dword ptr [edi + 0x18]
        and    ch, 0xbf
        cmp    ch, 8
        jne    loc_4_0AF4
        cmp    byte ptr [edi + 0x11], 2
        jne    loc_4_0A59
        cmp    cl, 0x4a
        je     loc_4_0A68
        cmp    cl, 0x4b
        je     loc_4_0A68
loc_4_0A59:
        cmp    cl, 0x6a
        je     loc_4_0AA0
        cmp    cl, 0x6b
        je     loc_4_0AA0
        jmp    loc_4_0AF4
loc_4_0A68:
        push   edx
        mov    ebx, dword ptr [0x10d8]       ; -> obj1:0x10D8
        push   ebx
        push   dword ptr [0x29c]             ; -> obj5:0x029C
        call   sub_4_45A9
        push   ebx
        push   dword ptr [0x2a4]             ; -> obj5:0x02A4
        call   sub_4_45A9
        pop    edx
        inc    byte ptr [0x2cc]              ; -> obj5:0x02CC
        mov    eax, edx
        VMMCall  VMM.Exec_PM_Int                 ; cd2054010100
        test   word ptr [ebp + 0x2c], 1
        jne    loc_4_0AEE
        jmp    loc_4_0AFC
loc_4_0AA0:
        mov    eax, edx
        VMMCall  VMM.Exec_PM_Int                 ; cd2054010100
        test   word ptr [ebp + 0x2c], 1
        jne    loc_4_0AFC
        cmp    word ptr [ebp + 0x1c], 0x440d
        jne    loc_4_0AC1
        cmp    byte ptr [0x2cc], 0           ; -> obj5:0x02CC
        jne    loc_4_0AC8
loc_4_0AC1:
        cmp    word ptr [ebp + 0x1c], 1
        jne    loc_4_0AFC
loc_4_0AC8:
        mov    ebx, dword ptr [0x10d8]       ; -> obj1:0x10D8
        push   ebx
        push   dword ptr [0x2a4]             ; -> obj5:0x02A4
        call   sub_4_45C3
        push   ebx
        push   dword ptr [0x29c]             ; -> obj5:0x029C
        call   sub_4_45C3
        dec    byte ptr [0x2cc]              ; -> obj5:0x02CC
        jmp    loc_4_0AFC
loc_4_0AEE:
        jmp    loc_4_0AC8
loc_4_0AF0:
        mov    byte ptr [edi + 0x1d], 0x73
loc_4_0AF4:
        mov    eax, edx
        VMMCall  VMM.Exec_PM_Int                 ; cd2054010100
loc_4_0AFC:
        pop    esi
        push   esi
        mov    edi, dword ptr [esi + 0x18]
        mov    esi, dword ptr [esi]
        push   esi
        lea    esi, [esi + 0x10]
        cld    
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        pop    esi
        push   esi
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        pop    esi
        mov    eax, dword ptr [esi + 0x2c]
        stosd  dword ptr es:[edi], eax
        pop    esi
        mov    ecx, dword ptr [esi + 0x20]
        jecxz  loc_4_0B21
        mov    dword ptr [ecx], 0x1c
loc_4_0B21:
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        xor    eax, eax
loc_4_0B32:
        ret    
sub_4_0B33:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        VxDCall  IFSMgr.IFSMgr_Win32DupHandle    ; cd2031004000
        leave  
        ret    
sub_4_0B50:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, 0xd700
        VxDCall  IFSMgr.IFSMgr_Ring0_FileIO      ; cd2032004000
        pop    ebx
        leave  
        ret    
sub_4_0B65:
        VxDCall  NDIS.Service_0015h              ; cd2015002a00
        ret    
loc_4_0B6C:
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        or     dword ptr [edi], 0x40000
        mov    ebx, dword ptr [0x10d8]       ; -> obj1:0x10D8
        test   dword ptr [ebx + 0x44], 0x20
        je     loc_4_0B8D
        or     dword ptr [edi], 0x80000
loc_4_0B8D:
        mov    esi, dword ptr [edi]
        and    esi, 0x30000
        and    dword ptr [edi], 0xfffcffff
        test   dword ptr [ebx + 0x44], 0x400
        je     loc_4_0BAA
        or     dword ptr [edi], 0x10000
loc_4_0BAA:
        mov    eax, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [edx + 0x1c], eax
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [edx + 0x18], ecx
        push   dword ptr [edx + 0x40]
        cmp    ah, 0x3f
        je     loc_4_0BEC
        cmp    ah, 0x40
        je     loc_4_0BEC
loc_4_0BC9:
        mov    eax, 0x21
        VMMCall  VMM.Exec_PM_Int                 ; cd2054010100
loc_4_0BD4:
        pop    eax
        mov    edx, dword ptr [esp + 4]
        mov    word ptr [edx + 0x40], ax
        and    dword ptr [edi], 0xfff3ffff
        or     esi, esi
        je     loc_4_0BE9
        or     dword ptr [edi], esi
loc_4_0BE9:
        ret    0x10
loc_4_0BEC:
        push   esi
        push   ebx
        push   edx
        push   eax
        push   ecx
        mov    ebx, dword ptr [edx + 0x10]
        mov    esi, dword ptr [edx + 0x14]
        VxDCall  IFSMgr.IFSMgr_Win32_Get_Ring0_Handle ; cd2033004000
        pop    ecx
        pop    eax
        jb     loc_4_0C38
        sub    ah, 0x3f
        movzx  eax, ah
        add    eax, 0xd602
        VxDCall  IFSMgr.IFSMgr_Ring0_FileIO      ; cd2032004000
        pop    edx
        push   edx
        push   eax
        mov    dword ptr [edx + 0x1c], eax
        sbb    eax, eax
        and    eax, 1
        and    word ptr [edx + 0x2c], 0xfffe
        or     word ptr [edx + 0x2c], ax
        test   eax, eax
        pop    eax
        je     loc_4_0C37
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    word ptr [edi + 0x34], ax
        pop    edi
loc_4_0C37:
        clc    
loc_4_0C38:
        pop    edx
        pop    ebx
        pop    esi
        jb     loc_4_0BC9
        jmp    loc_4_0BD4
loc_4_0C3F:
        mov    eax, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [edx + 0x1c], eax
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [edx + 0x18], ecx
        mov    eax, 0x31
        VMMCall  VMM.Exec_PM_Int                 ; cd2054010100
        ret    0x10
loc_4_0C5F:
        mov    eax, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [edx + 0x1c], eax
        mov    eax, 0x41
        VMMCall  VMM.Exec_PM_Int                 ; cd2054010100
        ret    0xc
loc_4_0C78:
        mov    esi, dword ptr [esp + 0xc]
        VMMCall  VMM.VMM_Replace_Global_Environment ; cd2085010100
        ret    0xc
loc_4_0C85:
        ret    
loc_4_0C86:
        mov    ebx, dword ptr [esp + 0xc]
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_4_0CC6
        mov    al, 0x2e
        xor    ah, ah
        mov    ebx, 0x400
        xor    cl, cl
        mov    esi, 0xc85                    ; -> loc_4_0C85
        xor    edx, edx
        xor    edi, edi
        VxDCall  VKD.VKD_Define_Hot_Key          ; cd2001000d00
        jb     loc_4_0CC6
        mov    esi, eax
        mov    ebx, dword ptr [esp + 0xc]
        mov    cx, 0x400
        VxDCall  VKD.VKD_Reflect_Hot_Key         ; cd2005000d00
        mov    eax, esi
        VxDCall  VKD.VKD_Remove_Hot_Key          ; cd2002000d00
loc_4_0CC6:
        ret    0xc
sub_4_0CC9:
        mov    eax, 0x103
        ret    
sub_4_0CCF:
        push   esi
        push   edi
        push   ebx
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    dword ptr [esi + 0x1c], eax
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_4_0CDF:
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        VxDCall  VMCPD.VMCPD_Get_CR0_State       ; cd2003001100
        mov    edi, dword ptr [esp + 8]
        mov    dword ptr [edi + 0x1c], eax
        pop    edi
        ret    0xc
loc_4_0CF5:
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x14]
        VxDCall  VMCPD.VMCPD_Set_CR0_State       ; cd2004001100
        pop    edi
        ret    0x10
loc_4_0D08:
        VxDCall  VMCPD.VMCPD_Get_Version         ; cd2000001100
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [edx + 0x1c], eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], ecx
        ret    0xc
loc_4_0D1E:
        VxDCall  VTD.VTD_Get_Real_Time           ; cd2007000500
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], edx
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x1c], 1
        ret    0xc
sub_4_0D3B:
        enter  4, 0
        push   ebx
        push   esi
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    ebx, edi
        add    ebx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    ebx, dword ptr [ebx]
        mov    dword ptr [ebp - 4], ebx
        or     ebx, ebx
        je     loc_4_0E99
        mov    esi, 0x62e                    ; -> loc_4_062E
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        mov    dword ptr [0x1108], eax       ; -> obj1:0x1108
        jae    loc_4_0D7B
        mov    dword ptr [ebp - 4], 0
        jmp    loc_4_0E99
loc_4_0D7B:
        lea    eax, [edi + 0x2a]
        mov    dword ptr [ebx + 0x40], eax
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x24]
        mov    dword ptr [0x298], eax        ; -> obj5:0x0298
        mov    dword ptr [0x10e0], ebx       ; -> obj1:0x10E0
        mov    eax, dword ptr [edx]
        mov    dword ptr [ebx], eax
        mov    dword ptr [0x10d8], eax       ; -> obj1:0x10D8
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [0x10dc], eax       ; -> obj1:0x10DC
        mov    dword ptr [0x34c], eax        ; -> loc_5_034C
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [edx + 0xc], edi
        mov    dword ptr [edx + 0x10], 0x10d8 ; -> obj1:0x10D8
        mov    eax, dword ptr [edx + 0x14]
        mov    dword ptr [0x10e4], eax       ; -> obj1:0x10E4
        mov    ax, word ptr [eax]
        mov    word ptr [ebx + 0x72], ax
        mov    eax, dword ptr [edx + 0x18]
        mov    dword ptr [0x4a8], eax        ; -> sub_5_04A8
        mov    eax, dword ptr [edx + 0x28]
        mov    dword ptr [0x29c], eax        ; -> obj5:0x029C
        mov    eax, dword ptr [edx + 0x2c]
        mov    dword ptr [0x2a4], eax        ; -> obj5:0x02A4
        mov    eax, dword ptr [edx + 0x30]
        mov    dword ptr [0x2a8], eax        ; -> obj5:0x02A8
        mov    eax, dword ptr [edx + 0x34]
        mov    dword ptr [0x2ac], eax        ; -> obj5:0x02AC
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 4]
        mov    dword ptr [edx + 0x48], ebx
        mov    eax, dword ptr [edx + 0x38]
        add    eax, ebx
        mov    dword ptr [0x2a0], eax        ; -> obj5:0x02A0
        mov    edi, 0x3ac                    ; -> loc_5_03AC
        mov    esi, dword ptr [edx + 0x40]
        mov    ecx, 0x18
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    esi, dword ptr [edx + 0x44]
        mov    ecx, 0x18
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    esi, 0x3ac                    ; -> loc_5_03AC
        mov    edi, esi
        mov    ecx, 0x30
loc_4_0E2D:
        lodsd  eax, dword ptr [esi]
        cmp    eax, 0x3fffff
        ja     loc_4_0E37
        add    eax, ebx
loc_4_0E37:
        stosd  dword ptr es:[edi], eax
        loop   loc_4_0E2D
        VxDCall  IFSMgr.IFSMgr_GetConversionTablePtrs ; cd2051004000
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0x3c], eax
        VMMCall  VMM.Get_PSP_Segment             ; cd20b9000100
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 0x1e], ax
        mov    word ptr [0x10d4], ax         ; -> obj1:0x10D4
        shl    eax, 4
        push   0
        push   0
        push   0x92
        push   0xff
        push   eax
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 0x1c], ax
        mov    word ptr [0x10d6], ax         ; -> obj1:0x10D6
        mov    esi, 0x9a8                    ; -> loc_1_09A8
        VMMCall  VMM.Call_When_Thread_Switched   ; cd2004010100
loc_4_0E99:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
_VWIN32_CopyMem:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
        mov    edx, ecx
        xor    eax, eax
loc_4_0EB3:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_4_0EB5:
        inc    eax
loc_4_0EB6:
        sub    edx, ecx
        mov    esi, dword ptr [esp + 0x18]
        or     esi, esi
        je     loc_4_0EC2
        mov    dword ptr [esi], edx
loc_4_0EC2:
        pop    edi
        pop    esi
        ret    
sub_4_0EC5:
        VMMCall  VMM._PageCommit                 ; cd201e810100
sub_4_0ECB:
        VMMCall  VMM._PageDecommit               ; cd201f810100
sub_4_0ED1:
        VMMCall  VMM._PageAttach                 ; cd2025810100
sub_4_0ED7:
        VMMCall  VMM._PageModifyPermissions      ; cd2033810100
sub_4_0EDD:
        VMMCall  VMM._SelectorMapFlat            ; cd206e800100
        VMMCall  VMM._BlockOnID                  ; cd20fa800100
sub_4_0EE9:
        VMMCall  VMM._SignalID                   ; cd2027810100
sub_4_0EEF:
        VMMCall  VMM._PageQuery                  ; cd2034810100
sub_4_0EF5:
        VMMCall  VMM._EnterMutex                 ; cd202e810100
sub_4_0EFB:
        VMMCall  VMM._LeaveMutex                 ; cd202f810100
sub_4_0F01:
        VxDCall  VMCPD._VMCPD_Get_FP_Instruction_Size ; cd2007801100
        add    byte ptr [esi + 0x40], bh
        jmp    loc_4_0F10
loc_4_0F0F:
        ret    
loc_4_0F10:
        sti    
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        cmp    dword ptr [edi + 0x2c], 0x2a
        jne    loc_4_0F7C
        or     byte ptr [ebp + 0x2d], 2
        cmp    esi, 0x2c
        je     loc_4_0FE6
        cmp    esi, 0x30
        je     loc_4_0FE6
        mov    edi, dword ptr [0x10e0]       ; -> obj1:0x10E0
        call   sub_4_1D79
        jb     loc_4_0F7B
        call   sub_4_1FAC
        jb     loc_4_0F7B
        mov    ax, word ptr [0x360]          ; -> loc_5_0360
        cmp    ax, word ptr [ebp + 0x28]
        jne    loc_4_0F82
        mov    ax, word ptr [0x362]          ; -> loc_5_0362
        cmp    ax, word ptr [ebp + 0x34]
        jne    loc_4_0FBB
        cmp    dword ptr [0x328], 0          ; -> sub_5_0328
        je     loc_4_0FBB
        shr    esi, 2
        push   0xfffebad
        push   ebp
        push   esi
        call   sub_5_0328
        add    esp, 0xc
loc_4_0F7B:
        ret    
loc_4_0F7C:
        jmp    sub_5_0368
loc_4_0F82:
        cmp    byte ptr [edi + 0x78], 2
        jae    loc_4_0FBB
        lar    eax, word ptr [ebp + 0x34]
        test   eax, 0x400000
        jne    loc_4_0FAD
        lar    eax, word ptr [ebp + 0x28]
        test   eax, 0x400000
        jne    loc_4_0FAD
loc_4_0F9E:
        inc    byte ptr [edi + 0x78]
        push   edi
        call   sub_5_0368
        pop    edi
        dec    byte ptr [edi + 0x78]
        ret    
loc_4_0FAD:
        cmp    word ptr [ebp + 0x32], 0
        jne    loc_4_0FBB
        cmp    word ptr [ebp + 0x26], 0
        je     loc_4_0F9E
loc_4_0FBB:
        shr    esi, 2
        push   esi
        push   ebp
        VxDCall  NDIS.Service_000Bh              ; cd200b002a00
        add    esp, 8
        or     eax, eax
        jne    loc_4_0FE5
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        cmp    dword ptr [edi + 0x24], 0
        je     loc_4_0FE5
        push   1
        push   edi
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        add    esp, 8
loc_4_0FE5:
        ret    
loc_4_0FE6:
        cmp    dword ptr [edi + 0x24], 0
        je     loc_4_103F
        mov    eax, dword ptr [ebp + 0x20]
        or     al, 3
        lar    ecx, eax
        jne    loc_4_1149
        test   ch, 0x80
        jne    loc_4_10F6
        mov    edi, dword ptr [0x2b4]        ; -> obj5:0x02B4
        movzx  ecx, word ptr [edi]
        jecxz  loc_4_103F
        cmp    ax, cx
        jne    loc_4_103F
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    edi, eax
        push   0
        push   ebx
        push   edi
        VMMCall  VMM._GetDescriptor              ; cd207b000100
        add    esp, 0xc
        or     dh, 0x80
        push   0
        push   eax
        push   edx
        push   ebx
        push   edi
        VMMCall  VMM._SetDescriptor              ; cd207c000100
        add    esp, 0x14
        jmp    loc_4_10F6
loc_4_103F:
        cmp    dword ptr [0x340], 0          ; -> loc_5_0340
        je     loc_4_1149
        lar    eax, word ptr [ebp + 0x34]
        jne    loc_4_1149
        test   eax, 0x400000
        jne    loc_4_1159
        cmp    word ptr [ebp + 0x30], 0x20
        jb     loc_4_1149
        mov    ebx, dword ptr [ebp + 0x20]
        or     bx, bx
        je     loc_4_10AA
        mov    edi, dword ptr [0x10d8]       ; -> obj1:0x10D8
        push   edi
        push   dword ptr [0x2a8]             ; -> obj5:0x02A8
        call   sub_4_45A9
        mov    eax, ebx
        or     al, 3
        lar    eax, eax
        jne    loc_4_1097
        test   ah, 0x80
        je     loc_4_1097
        or     esi, 1
loc_4_1097:
        push   edi
        push   dword ptr [0x2a8]             ; -> obj5:0x02A8
        call   sub_4_45C3
        test   si, 1
        jne    loc_4_10F6
loc_4_10AA:
        sub    word ptr [ebp + 0x30], 0x12
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_4_1149
        mov    eax, dword ptr [0x344]        ; -> loc_5_0344
        stosd  dword ptr es:[edi], eax
        mov    eax, ebx
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp + 0x24]
        stosd  dword ptr es:[edi], eax
        movzx  eax, word ptr [ebp + 0x28]
        stosd  dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp + 0x2c]
        stosd  dword ptr es:[edi], eax
        mov    eax, dword ptr [0x340]        ; -> loc_5_0340
        ror    eax, 0x10
        mov    word ptr [ebp + 0x28], ax
        shr    eax, 0x10
        mov    dword ptr [ebp + 0x24], eax
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
        and    byte ptr [ebp + 0x2d], 0xfe
loc_4_10F6:
        ret    
loc_4_10F7:
        mov    edi, dword ptr [ebp + 0x2c]
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        cmp    esi, -1
        je     loc_4_1139
        add    esi, 2
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [ebp + 0x24], eax
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [ebp + 0x28], eax
        lodsd  eax, dword ptr [esi]
        mov    ecx, eax
        xchg   word ptr [ebp + 0x2c], cx
        xor    ecx, eax
        test   ch, 2
        je     loc_4_1134
        test   ah, 2
        je     loc_4_1134
        and    word ptr [ebp + 0x2c], 0xfdff
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
loc_4_1134:
        add    word ptr [ebp + 0x30], 0xe
loc_4_1139:
        or     esi, 0xffffffff
        test   edi, 1
        jne    loc_4_115C
        mov    esi, 0x2c
loc_4_1149:
        mov    ax, word ptr [0x360]          ; -> loc_5_0360
        cmp    ax, word ptr [ebp + 0x28]
        jne    loc_4_0F7C
loc_4_1159:
        shr    esi, 2
loc_4_115C:
        push   esi
        push   ebp
        VxDCall  NDIS.Service_000Bh              ; cd200b002a00
        add    esp, 8
        ret    
sub_4_1168:
        mov    eax, dword ptr [0x10d8]       ; -> obj1:0x10D8
        or     eax, eax
        je     loc_4_1185
        mov    ebx, dword ptr [eax + 0x80]
loc_4_1177:
        or     ebx, ebx
        je     loc_4_1185
        test   dword ptr [ebx + 4], edx
        jne    loc_4_1184
        mov    ebx, dword ptr [ebx]
        jmp    loc_4_1177
loc_4_1184:
        stc    
loc_4_1185:
        ret    
sub_4_1186:
        push   ebx
        push   edi
        call   sub_4_1168
        jae    loc_4_11C5
        mov    ebx, dword ptr [eax + 0x80]
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    edi, dword ptr [edi + 0x10]
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [edi + 0x24], eax
        mov    eax, dword ptr [ebx + 0xc]
        mov    word ptr [edi + 0x28], ax
        mov    eax, dword ptr [ebx + 0x10]
        mov    dword ptr [edi + 0x30], eax
        mov    eax, dword ptr [ebx + 0x14]
        mov    word ptr [edi + 0x34], ax
        mov    eax, dword ptr [ebx + 0x18]
        mov    dword ptr [edi + 0x1c], eax
        mov    eax, dword ptr [ebx + 0x1c]
        mov    dword ptr [edi + 0x14], eax
        stc    
loc_4_11C5:
        pop    edi
        pop    ebx
        ret    
loc_4_11C8:
        push   edi
        push   ebp
        call   sub_4_46F8
        cmp    eax, 1
        ret    
loc_4_11D3:
        push   1
        push   0xc
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        mov    edx, eax
        je     loc_4_1205
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, 0x122d                   ; -> loc_4_122D
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [edx + 8], esi
loc_4_1205:
        mov    ebx, dword ptr [esp + 4]
        mov    dword ptr [ebx + 0x1c], edx
        ret    0x14
loc_4_120F:
        mov    edx, dword ptr [esp + 0xc]
        xor    esi, esi
        xchg   dword ptr [edx + 8], esi
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        push   0
        push   edx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        ret    0xc
loc_4_122D:
        mov    ebx, edx
        xor    esi, esi
        xchg   dword ptr [ebx + 8], esi
        or     esi, esi
        je     loc_4_125B
        mov    edi, dword ptr [0x33c]        ; -> loc_5_033C
        or     edi, edi
        je     loc_4_125B
        push   edi
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx]
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
        push   edi
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
loc_4_125B:
        ret    
loc_4_125C:
        mov    edx, dword ptr [esp + 0xc]
        call   sub_4_1186
        ret    0xc
loc_4_1268:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [0x328], eax        ; -> sub_5_0328
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [0x360], ax          ; -> loc_5_0360
        mov    eax, dword ptr [esp + 0x14]
        mov    word ptr [0x362], ax          ; -> loc_5_0362
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [0x338], eax        ; -> loc_5_0338
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [0x32c], eax        ; -> loc_5_032C
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [0x330], eax        ; -> loc_5_0330
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [0x334], eax        ; -> loc_5_0334
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [0x33c], eax        ; -> loc_5_033C
        mov    eax, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    ebx, dword ptr [esp + 4]
        mov    dword ptr [ebx + 0x1c], eax
        ret    0x28
loc_4_12C1:
        mov    dword ptr [0x33c], 0          ; -> loc_5_033C
        jmp    loc_4_0591
VWIN32_TerminateApp:
        pushal 
        mov    ebx, dword ptr [edi + 0x14]
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_4_1353
        mov    edx, 0x20
        call   sub_4_1186
        jb     loc_4_134C
        mov    ebx, dword ptr [0x1110]       ; -> obj1:0x1110
        or     ebx, ebx
        je     loc_4_12F6
        cmp    ebx, edi
        jne    loc_4_130A
loc_4_12F6:
        mov    ebx, dword ptr [0x33c]        ; -> loc_5_033C
        or     ebx, ebx
        jne    loc_4_130A
        push   edi
        VMMCall  VMM.Get_Sys_Thread_Handle       ; cd200a010100
        mov    ebx, edi
        pop    edi
loc_4_130A:
        mov    esi, edi
        add    esi, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    esi, dword ptr [esi]
        or     esi, esi
        je     loc_4_1362
        mov    esi, dword ptr [esi]
        push   ebx
        push   esi
        push   dword ptr [0x330]             ; -> loc_5_0330
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
        cmp    ebx, dword ptr [0x33c]        ; -> loc_5_033C
        jne    loc_4_133D
        push   ebx
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
loc_4_133D:
        VMMCall  VMM.Test_Cur_Thread_Handle      ; cd2009010100
        jne    loc_4_134A
        call   sub_1_0608
loc_4_134A:
        popal  
        ret    
loc_4_134C:
        VMMCall  VMM.Cleanup_Thread_State        ; cd2070010100
        ret    
loc_4_1353:
        mov    eax, 0xbb8
        xor    ecx, ecx
        VMMCall  VMM.Close_VM                    ; cd20ec000100
        jmp    loc_4_134A
loc_4_1362:
        pushal 
        xor    esi, esi
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
loc_4_136B:
        mov    ax, 0x3408
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_4_139B
        mov    edx, 8
        call   sub_4_1168
        jae    loc_4_139B
        mov    eax, dword ptr [0x348]        ; -> loc_5_0348
        mov    dword ptr [edi + 0x34], eax
        xor    ax, ax
        mov    word ptr [ebp + 0x1c], ax
        jmp    loc_4_0591
loc_4_139B:
        mov    esi, 0x1735                   ; -> loc_4_1735
        call   sub_4_1609
        mov    word ptr [ebp + 0x1c], ax
        or     eax, eax
        jne    loc_4_13C5
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        cmp    dword ptr [edi + 0x24], 1
        jbe    loc_4_13C5
        push   1
        push   edi
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        add    esp, 8
loc_4_13C5:
        jmp    loc_4_0591
loc_4_13CA:
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    ebx, eax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    ecx, eax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    edx, eax
        mov    si, word ptr [ebp + 0x30]
        mov    di, word ptr [ebp + 0x34]
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    eax, edi
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, esi
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, edx
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, ecx
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, ebx
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        xor    eax, eax
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    cx, word ptr [0x35a]          ; -> loc_5_035A
        movzx  edx, word ptr [0x358]         ; -> loc_5_0358
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        add    word ptr [ebp + 0x30], 2
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    ebx, eax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    ecx, eax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    edx, eax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    esi, eax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    edi, eax
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    word ptr [ebp + 0x34], di
        mov    word ptr [ebp + 0x30], si
        mov    word ptr [ebp + 0x2c], dx
        mov    word ptr [ebp + 0x28], cx
        mov    word ptr [ebp + 0x24], bx
        ret    
loc_4_1482:
        mov    word ptr [ebp + 0x1c], 0
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   ebx, eax
        cmp    ebx, -1
        je     loc_4_14A8
        or     word ptr [ebx + 8], 0x4000
        VxDCall  NDIS.Service_001Ah              ; cd201a002a00
        mov    word ptr [ebp + 0x1c], ax
loc_4_14A8:
        jmp    loc_4_0591
VWIN32_SysErrorBox:
        push   esi
        push   edi
        push   ecx
        push   ebx
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        pop    ebx
        je     loc_4_14C7
        xor    eax, eax
        push   eax
        push   eax
        jmp    loc_4_14ED
loc_4_14C7:
        mov    edi, dword ptr [0x10d8]       ; -> obj1:0x10D8
        push   edi
        push   dword ptr [0x2a4]             ; -> obj5:0x02A4
        call   sub_4_449B
        add    esp, 8
        push   eax
        push   edi
        push   dword ptr [0x29c]             ; -> obj5:0x029C
        call   sub_4_449B
        add    esp, 8
        push   eax
loc_4_14ED:
        push   ebx
        VMMCall  VMM.Get_Crit_Status_Thread      ; cd2045010100
        mov    esi, edi
        pop    ebx
        jecxz  loc_4_1511
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        cmp    edi, esi
        je     loc_4_1507
        xor    ecx, ecx
        jmp    loc_4_1511
loc_4_1507:
        push   ecx
loc_4_1508:
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        loop   loc_4_1508
        pop    ecx
loc_4_1511:
        push   ecx
        mov    dword ptr [ebx], 0xffffffff
        mov    eax, dword ptr [0x33c]        ; -> loc_5_033C
        or     eax, eax
        je     loc_4_154C
        push   eax
        push   ebx
        push   dword ptr [0x334]             ; -> loc_5_0334
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
        push   dword ptr [0x33c]             ; -> loc_5_033C
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
loc_4_1541:
        lea    esi, [ebx]
        call   sub_1_01F4
        jae    loc_4_1541
        mov    esi, eax
loc_4_154C:
        pop    ecx
        jecxz  loc_4_1570
loc_4_154F:
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        loop   loc_4_154F
        push   esi
        push   edx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        xor    edx, edx
        xor    esi, esi
        mov    eax, 0xf
        VMMCall  VMM.System_Control              ; cd2093000100
        pop    edx
        pop    esi
loc_4_1570:
        mov    edi, dword ptr [0x10d8]       ; -> obj1:0x10D8
        pop    eax
        push   eax
        push   edi
        push   dword ptr [0x29c]             ; -> obj5:0x029C
        call   sub_4_44ED
        add    esp, 0xc
        pop    eax
        push   eax
        push   edi
        push   dword ptr [0x2a4]             ; -> obj5:0x02A4
        call   sub_4_44ED
        add    esp, 0xc
        mov    eax, esi
        pop    ebx
        pop    ecx
        pop    edi
        pop    esi
        ret    
loc_4_159F:
        push   ebp
        mov    ebp, esp
        mov    esi, 0x1707                   ; -> loc_4_1707
        call   sub_4_1609
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0x1c], eax
        leave  
        ret    0x14
_VWIN32_FaultPopup:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0xc], -1
        je     loc_4_15E9
        cmp    dword ptr [ebp + 0xc], 0x1f
        jae    loc_4_15E9
        VxDCall  DEBUG.Service_0000h             ; cd2000000200
        or     eax, eax
        je     loc_4_15E9
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    cx, 0x12
        VxDCall  DEBUG.Service_0002h             ; cd2002000200
        mov    eax, 1
        jb     loc_4_1604
loc_4_15E9:
        mov    esi, 0x17f2                   ; -> loc_4_17F2
        call   sub_4_1609
        cmp    eax, 1
        jne    loc_4_1604
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        VxDCall  DEBUG.Service_0001h             ; cd2001000200
loc_4_1604:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_4_1609:
        push   0
        push   0x16b
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_16FD
        mov    ebx, eax
        call   esi
        mov    edx, 8
        mov    ax, word ptr [0x360]          ; -> loc_5_0360
        cmp    ax, word ptr [ebx + 0xc8]
        jne    loc_4_164D
        mov    ax, word ptr [0x362]          ; -> loc_5_0362
        cmp    ax, word ptr [ebx + 0xd4]
        jne    loc_4_164D
        mov    edx, 0x10
loc_4_164D:
        call   sub_4_1186
        mov    eax, 0
        jb     loc_4_16ED
        call   sub_4_181E
        cmp    dword ptr [0x33c], 0          ; -> loc_5_033C
        je     loc_4_1681
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        VMMCall  VMM.Test_Sys_Thread_Handle      ; cd200b010100
        je     loc_4_1681
        cmp    edi, dword ptr [0x33c]        ; -> loc_5_033C
        jne    loc_4_16A9
loc_4_1681:
        push   ebx
        mov    esi, dword ptr [ebx + 4]
        mov    edi, dword ptr [ebx + 0xc4]
        mov    ebx, dword ptr [ebx + 0xc8]
        movzx  edx, byte ptr [ebx + 0x16a]
        xor    edx, 4
        and    edx, 4
        or     edx, 1
        call   VWIN32_BlueScreenPopup
        pop    ebx
        jmp    loc_4_16ED
loc_4_16A9:
        mov    edi, dword ptr [0x10e0]       ; -> obj1:0x10E0
        mov    dword ptr [ebx], edi
        mov    dword ptr [ebx + 0x164], 0xffffffff
        push   dword ptr [0x33c]             ; -> loc_5_033C
        push   ebx
        push   dword ptr [0x32c]             ; -> loc_5_032C
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
        push   dword ptr [0x33c]             ; -> loc_5_033C
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
        lea    esi, [ebx + 0x164]
        call   sub_1_01F4
        jae    loc_4_16FE
loc_4_16ED:
        mov    esi, eax
        push   0
        push   ebx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    eax, esi
loc_4_16FD:
        ret    
loc_4_16FE:
        or     byte ptr [ebx + 0x16a], 8
        jmp    loc_4_16FD
loc_4_1707:
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebx + 4], eax
        xor    eax, eax
        mov    dword ptr [ebx + 8], eax
        cmp    dword ptr [ebp + 0x18], eax
        je     loc_4_1719
        or     al, 2
loc_4_1719:
        mov    byte ptr [ebx + 0x16a], al
        mov    esi, dword ptr [ebp + 0x10]
        or     esi, esi
        je     loc_4_1808
        lea    edi, [ebx + 0xc]
        mov    ecx, 0x33
        rep movsd dword ptr es:[edi], dword ptr [esi]
        ret    
loc_4_1735:
        mov    al, byte ptr [ebp + 0x1c]
        and    al, 1
        or     al, 4
        mov    byte ptr [ebx + 0x16a], al
        cmp    edi, -1
        je     loc_4_17F1
        movzx  eax, word ptr [edi + 0x28]
        mov    dword ptr [ebx + 4], eax
        movzx  eax, word ptr [edi + 0x32]
        mov    dword ptr [ebx + 8], eax
        movzx  eax, word ptr [edi]
        mov    dword ptr [ebx + 0x98], eax
        movzx  eax, word ptr [edi + 2]
        mov    dword ptr [ebx + 0x9c], eax
        movzx  eax, word ptr [edi + 4]
        mov    dword ptr [ebx + 0xa0], eax
        movzx  eax, word ptr [edi + 6]
        mov    dword ptr [ebx + 0xa4], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebx + 0xa8], eax
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0xac], eax
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [ebx + 0xb0], eax
        mov    eax, dword ptr [edi + 0x1c]
        mov    dword ptr [ebx + 0xb4], eax
        mov    eax, dword ptr [edi + 0x20]
        mov    dword ptr [ebx + 0xb8], eax
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [ebx + 0xbc], eax
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [ebx + 0xc0], eax
        movzx  eax, word ptr [edi + 0x34]
        mov    dword ptr [ebx + 0xc4], eax
        movzx  eax, word ptr [edi + 0x36]
        mov    dword ptr [ebx + 0xc8], eax
        movzx  eax, word ptr [edi + 0x38]
        mov    dword ptr [ebx + 0xcc], eax
        movzx  eax, word ptr [edi + 0x3a]
        mov    dword ptr [ebx + 0xd0], eax
        movzx  eax, word ptr [edi + 0x3c]
        mov    dword ptr [ebx + 0xd4], eax
loc_4_17F1:
        ret    
loc_4_17F2:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebx + 4], eax
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x20]
        mov    dword ptr [ebx + 8], eax
        mov    byte ptr [ebx + 0x16a], 4
loc_4_1808:
        lea    eax, [ebx + 0xc]
        mov    dword ptr [eax], 0xffffffff
        push   eax
        push   0
        VxDCall  NDIS.Service_0003h              ; cd2003002a00
        add    esp, 8
        ret    
sub_4_181E:
        mov    esi, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    dword ptr [esi + 0x120], 0
        mov    dword ptr [esi + 0x10c], 0
        push   0
        push   dword ptr [esi + 0xd4]
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_4_1885
        mov    edx, dword ptr [esi + 0xd0]
        lar    ecx, word ptr [esi + 0xd4]
        test   ecx, 0x400000
        jne    loc_4_1869
        movzx  edx, dx
loc_4_1869:
        add    eax, edx
        lea    edi, [esi + 0x124]
        lea    edx, [esi + 0x120]
        push   edx
        push   0x40
        push   edi
        push   eax
        VxDCall  NDIS.Service_0005h              ; cd2005002a00
        add    esp, 0x10
loc_4_1885:
        push   0
        push   dword ptr [esi + 0xc8]
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_4_18BC
        add    eax, dword ptr [esi + 0xc4]
        lea    edi, [esi + 0x110]
        lea    edx, [esi + 0x10c]
        push   edx
        push   0x10
        push   edi
        push   eax
        VxDCall  NDIS.Service_0005h              ; cd2005002a00
        add    esp, 0x10
loc_4_18BC:
        mov    ebx, esi
        ret    
sub_4_18BF:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0x10e0]       ; -> obj1:0x10E0
        or     byte ptr [esi + 0x2d], 0x42
        push   0x35
        push   dword ptr [0x10fc]            ; -> obj1:0x10FC
        VMMCall  VMM._EnterMutex                 ; cd202e010100
        add    esp, 8
        inc    byte ptr [0x475]              ; -> obj5:0x0475
        jne    loc_4_194F
        mov    edi, 0x1150                   ; -> loc_1_1150
        mov    ecx, 0x3c
        xor    al, al
        rep stosb byte ptr es:[edi], al
        mov    ebx, dword ptr [0x1120]       ; -> loc_1_1120
loc_4_18F9:
        lea    edi, [ebx + 8]
        mov    ecx, 0x24
        xor    al, al
        rep stosb byte ptr es:[edi], al
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_4_18F9
        mov    edi, 0x1120                   ; -> loc_1_1120
        lea    ebx, [edi + 0x30]
        mov    eax, dword ptr [0x10f8]       ; -> obj1:0x10F8
        VMMCall  VMM.Force_Mutexes_Free          ; cd207f010100
        mov    eax, dword ptr [0x10dc]       ; -> obj1:0x10DC
        add    eax, 0x60
        mov    dword ptr [edi + 8], eax
        mov    ebx, dword ptr [edi]
loc_4_192B:
        call   sub_4_1958
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_4_192B
        call   sub_4_1B19
        mov    ebx, dword ptr [0x1120]       ; -> loc_1_1120
loc_4_1941:
        mov    edx, dword ptr [ebx + 4]
        call   sub_4_2C1E
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_4_1941
loc_4_194F:
        mov    eax, 0x1120                   ; -> loc_1_1120
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_4_1958:
        push   edi
        mov    eax, 0x7d0
        mov    edx, ebx
        mov    esi, 0x21c                    ; -> loc_1_021C
        VMMCall  VMM.Set_Async_Time_Out          ; cd20fe000100
        mov    dword ptr [ebx + 0x18], esi
        mov    edx, dword ptr [ebx + 4]
        call   sub_4_2BAF
        mov    edi, dword ptr [ebx + 8]
        or     edi, edi
        je     loc_4_1982
        call   sub_4_1D59
loc_4_1982:
        xor    esi, esi
        xchg   dword ptr [ebx + 0x18], esi
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    edi
        ret    
loc_4_198F:
        lea    esi, [edx + 0x20]
        mov    dword ptr [esi + 4], 0x19a3   ; -> loc_4_19A3
        mov    dword ptr [esi + 8], edx
        push   esi
        call   sub_4_1E85
        ret    
loc_4_19A3:
        mov    edi, dword ptr [edx + 4]
        mov    edi, dword ptr [edi + 8]
        or     edi, edi
        je     loc_4_19CD
        mov    eax, 0xbb8
        mov    esi, 0x242                    ; -> loc_1_0242
        VMMCall  VMM.Set_Async_Time_Out          ; cd20fe000100
        mov    dword ptr [edx + 0x18], esi
        mov    dword ptr [edx + 8], edi
        mov    ecx, 0x19dc                   ; -> loc_4_19DC
        call   sub_4_1CBF
loc_4_19CD:
        ret    
loc_4_19CE:
        xor    edi, edi
        xchg   dword ptr [edx + 8], edi
        or     edi, edi
        je     loc_4_19DC
        call   sub_4_1D59
loc_4_19DC:
        lea    esi, [edx + 0x20]
        mov    dword ptr [esi + 4], 0x19f0   ; -> loc_4_19F0
        mov    dword ptr [esi + 8], edx
        push   esi
        call   sub_4_1E85
        ret    
loc_4_19F0:
        xor    esi, esi
        xchg   dword ptr [edx + 0x18], esi
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    ebx, edx
loc_4_19FD:
        mov    edi, dword ptr [ebx + 4]
        mov    eax, dword ptr [edi + 8]
        or     eax, eax
        je     loc_4_1A5B
        mov    dword ptr [ebx + 0xc], eax
        mov    esi, dword ptr [edi + 4]
        mov    dword ptr [ebx + 0x14], esi
        mov    edi, dword ptr [eax + 0x60]
        mov    ecx, dword ptr [edi + 0x28]
        mov    dword ptr [ebx + 0x1c], ecx
        mov    dword ptr [edi + 0x28], 0
        or     byte ptr [edi + 0x2d], 4
        mov    edi, dword ptr [eax + 0x5c]
        or     edi, edi
        je     loc_4_1A38
        push   edi
        call   sub_4_1BC1
        mov    ecx, dword ptr [edi + 0x10]
        or     byte ptr [ecx + 0x2d], 2
loc_4_1A38:
        mov    edx, dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebx + 0xc]
        call   sub_4_2C24
        dec    esi
        jne    loc_4_1A38
        mov    edi, dword ptr [ebx + 0xc]
        jmp    loc_4_1A55
loc_4_1A4B:
        mov    eax, dword ptr [ebx + 4]
        mov    eax, dword ptr [eax + 8]
        cmp    eax, edi
        je     loc_4_19FD
loc_4_1A55:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_4_1A4B
loc_4_1A5B:
        ret    
sub_4_1A5C:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        or     edi, edi
        je     loc_4_1AB0
        dec    byte ptr [0x475]              ; -> obj5:0x0475
        jns    loc_4_1A97
        mov    esi, dword ptr [esp + 0x14]
        mov    ebx, dword ptr [edi]
loc_4_1A75:
        call   sub_4_1AB4
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_4_1A75
        mov    dword ptr [edi + 8], 0
        push   edi
        call   sub_4_1B7A
        pop    edi
        lea    ebx, [edi + 0x30]
        VMMCall  VMM.Restore_Forced_Mutexes      ; cd2080010100
loc_4_1A97:
        mov    esi, dword ptr [0x10e0]       ; -> obj1:0x10E0
        and    byte ptr [esi + 0x2d], 0xbd
        push   dword ptr [0x10fc]            ; -> obj1:0x10FC
        VMMCall  VMM._LeaveMutex                 ; cd202f010100
        add    esp, 4
loc_4_1AB0:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_4_1AB4:
        push   edi
        mov    edi, dword ptr [ebx + 0x14]
        or     edi, edi
        je     loc_4_1AE1
        mov    ecx, dword ptr [ebx + 0xc]
        mov    eax, dword ptr [ecx + 0x40]
        mov    ecx, dword ptr [ecx + 0x60]
        and    byte ptr [ecx + 0x2d], 0xfb
        cmp    eax, esi
        je     loc_4_1AE1
        mov    eax, dword ptr [ebx + 0x1c]
        add    dword ptr [ecx + 0x28], eax
loc_4_1AD3:
        mov    edx, dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebx + 0xc]
        call   sub_4_2BB5
        dec    edi
        jne    loc_4_1AD3
loc_4_1AE1:
        pop    edi
        ret    
loc_4_1AE3:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xor    edx, edx
        xor    esi, esi
        mov    eax, 0xf
        VMMCall  VMM.System_Control              ; cd2093000100
        ret    8
loc_4_1AFB:
        push   dword ptr [esp + 0xc]
        call   sub_4_1C1B
        ret    0xc
loc_4_1B07:
        call   sub_4_1B19
        ret    8
loc_4_1B0F:
        xor    esi, esi
        call   sub_4_1B7A
        ret    8
sub_4_1B19:
        inc    byte ptr [0x366]              ; -> loc_5_0366
        jne    loc_4_1B79
        xor    esi, esi
loc_4_1B23:
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
loc_4_1B29:
        VMMCall  VMM.Validate_Thread_Handle      ; cd200c010100
        jb     loc_4_1B23
        VMMCall  VMM.Get_Next_Thread_Handle      ; cd2013010100
        VMMCall  VMM.Test_Cur_Thread_Handle      ; cd2009010100
        je     loc_4_1B79
        mov    ebx, dword ptr [edi + 0x14]
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_4_1B29
        mov    eax, edi
        add    eax, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, dword ptr [0x34c]        ; -> loc_5_034C
        je     loc_4_1B29
        push   edi
        call   sub_4_1BC1
        inc    esi
        test   esi, 0xf
        jne    loc_4_1B29
        xor    eax, eax
        inc    eax
        VMMCall  VMM.Time_Slice_Sleep            ; cd2061010100
        jmp    loc_4_1B29
loc_4_1B79:
        ret    
sub_4_1B7A:
        dec    byte ptr [0x366]              ; -> loc_5_0366
        jns    loc_4_1BC0
loc_4_1B82:
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
loc_4_1B88:
        VMMCall  VMM.Validate_Thread_Handle      ; cd200c010100
        jb     loc_4_1B82
        VMMCall  VMM.Get_Next_Thread_Handle      ; cd2013010100
        VMMCall  VMM.Test_Cur_Thread_Handle      ; cd2009010100
        je     loc_4_1BC0
        mov    ebx, dword ptr [edi + 0x14]
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_4_1B88
        mov    eax, edi
        add    eax, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    eax, dword ptr [eax]
        cmp    esi, dword ptr [eax + 4]
        je     loc_4_1B88
        push   edi
        call   sub_4_1C1B
        jmp    loc_4_1B88
loc_4_1BC0:
        ret    
sub_4_1BC1:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    ebx, edi
        add    ebx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     loc_4_1C15
        test   byte ptr [ebx + 0x2d], 2
        jne    loc_4_1C15
        bts    dword ptr [ebx + 0x2c], 8
        jb     loc_4_1C15
        push   edi
        call   sub_1_06E4
        add    esp, 4
        test   byte ptr [ebx + 0x2c], 4
        je     loc_4_1C15
        xor    eax, eax
        xchg   dword ptr [ebx + 0x24], eax
        or     eax, eax
        je     loc_4_1C00
        call   sub_1_0A44
loc_4_1C00:
        push   3
        push   edi
        push   0
        push   0x6a5                         ; -> sub_1_06A5
        call   _VWIN32_QueueKernelAPC
        add    esp, 0x10
        mov    dword ptr [ebx + 0x24], eax
loc_4_1C15:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_4_1C1B:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    ebx, edi
        add    ebx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     loc_4_1C6F
        test   byte ptr [ebx + 0x2d], 4
        jne    loc_4_1C6F
        btr    dword ptr [ebx + 0x2c], 8
        jae    loc_4_1C6F
        test   byte ptr [ebx + 0x2c], 4
        je     loc_4_1C66
        xor    eax, eax
        xchg   dword ptr [ebx + 0x24], eax
        or     eax, eax
        je     loc_4_1C51
        call   sub_1_0A44
loc_4_1C51:
        push   2
        push   edi
        push   0
        push   0x6a5                         ; -> sub_1_06A5
        call   _VWIN32_QueueKernelAPC
        add    esp, 0x10
        mov    dword ptr [ebx + 0x24], eax
loc_4_1C66:
        push   edi
        call   sub_1_073E
        add    esp, 4
loc_4_1C6F:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
loc_4_1C75:
        push   0
        push   0x2c
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_1CB5
        mov    edx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [0x1120]       ; -> loc_1_1120
        mov    ebx, 0xffff0bad
loc_4_1C95:
        cmp    edx, dword ptr [esi + 0x10]
        jb     loc_4_1CA2
        mov    ebx, esi
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_4_1C95
loc_4_1CA2:
        mov    dword ptr [eax], esi
        mov    dword ptr [ebx], eax
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 0x10], edx
        mov    eax, 0x1120                   ; -> loc_1_1120
loc_4_1CB5:
        mov    ebx, dword ptr [esp + 4]
        mov    dword ptr [ebx + 0x1c], eax
        ret    0x10
sub_4_1CBF:
        mov    esi, dword ptr [edi + 0x60]
        mov    edi, dword ptr [edi + 0x5c]
        or     edi, edi
        je     loc_4_1D1A
sub_4_1CC9:
        cmp    dword ptr [esi + 0x68], 0
        jne    loc_4_1D1A
        and    byte ptr [esi + 0x2d], 0xef
        cmp    dword ptr [esi + 0x28], 0
        je     loc_4_1D1A
        test   byte ptr [esi + 0x2d], 8
        jne    loc_4_1D1A
        test   byte ptr [esi + 0x2c], 0x10
        jne    loc_4_1D1A
        mov    eax, dword ptr [esi + 4]
        cmp    eax, dword ptr [0x34c]        ; -> loc_5_034C
        je     loc_4_1D1A
        mov    dword ptr [esi + 0x68], ecx
        mov    dword ptr [esi + 0x64], edx
        mov    edx, esi
        mov    ebx, edi
        xor    eax, eax
        mov    edi, 0x1388
        mov    ecx, 0x1d0
        or     ecx, dword ptr [0x111c]       ; -> obj1:0x111C
        mov    esi, 0x1d1b                   ; -> loc_4_1D1B
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [edx + 0x6c], esi
loc_4_1D1A:
        ret    
loc_4_1D1B:
        mov    ecx, 0
        xchg   dword ptr [edx + 0x6c], ecx
        jecxz  loc_4_1D51
        jb     loc_4_1D51
        btr    dword ptr [edx + 0x2c], 0xc
        jb     loc_4_1D51
        mov    ax, word ptr [ebp + 0x28]
        cmp    ax, word ptr [0x360]          ; -> loc_5_0360
        je     loc_4_1D51
        VMMCall  VMM.Test_Debug_Installed        ; cd20c1000100
        je     loc_4_1D4C
        xor    bx, bx
        mov    ax, 0x20
        int    0x41
loc_4_1D4C:
        or     byte ptr [ebp + 0x2d], 1
        ret    
loc_4_1D51:
        mov    dword ptr [edx + 0x68], 0
        ret    
sub_4_1D59:
        mov    eax, dword ptr [edi + 0x60]
        mov    edi, dword ptr [edi + 0x5c]
        or     edi, edi
        je     loc_4_1D78
        xor    esi, esi
        xchg   dword ptr [eax + 0x6c], esi
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        cmp    dword ptr [eax + 0x68], 0
        je     loc_4_1D78
        or     byte ptr [eax + 0x2d], 0x10
loc_4_1D78:
        ret    
sub_4_1D79:
        cmp    esi, 4
        jne    loc_4_1E83
        cmp    dword ptr [edi + 0x68], 0
        je     loc_4_1E83
        btr    dword ptr [edi + 0x2c], 0xc
        jb     loc_4_1E70
        cmp    dword ptr [edi + 0x28], 0
        je     loc_4_1E52
        lar    eax, word ptr [ebp + 0x34]
        test   eax, 0x400000
        jne    loc_4_1E7D
        lar    eax, word ptr [ebp + 0x28]
        test   eax, 0x400000
        jne    loc_4_1E7D
        mov    ax, word ptr [ebp + 0x28]
        mov    cx, ax
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 0x24]
        cmp    ecx, dword ptr [0x354]        ; -> loc_5_0354
        je     loc_4_1E52
        cmp    ax, word ptr [0x364]          ; -> loc_5_0364
        je     loc_4_1E7D
        cmp    ax, word ptr [0x10ae]         ; -> obj1:0x10AE
        je     loc_4_1E7D
        cmp    ax, word ptr [edi + 0x70]
        je     loc_4_1E7D
        mov    word ptr [edi + 0x70], ax
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], ax
        and    byte ptr [ebp + 0x2d], 0xfe
        mov    cx, word ptr [0x352]          ; -> loc_5_0352
        movzx  edx, word ptr [0x350]         ; -> loc_5_0350
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        movzx  eax, word ptr [ebp + 0x1c]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        or     ax, ax
        jne    loc_4_1E7D
loc_4_1E52:
        mov    word ptr [edi + 0x70], 0
        and    byte ptr [ebp + 0x2d], 0xfe
        xor    ecx, ecx
        xchg   dword ptr [edi + 0x68], ecx
        jecxz  loc_4_1E81
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [edi + 0x64]
        call   ecx
        pop    edi
        pop    esi
        pop    ebx
        jmp    loc_4_1E81
loc_4_1E70:
        and    byte ptr [ebp + 0x2d], 0xfe
        mov    dword ptr [edi + 0x68], 0
        jmp    loc_4_1E81
loc_4_1E7D:
        or     byte ptr [ebp + 0x2d], 1
loc_4_1E81:
        stc    
        ret    
loc_4_1E83:
        clc    
        ret    
sub_4_1E85:
        sub    esp, 0xcc
        push   ebx
        push   esi
        push   edi
        push   0x35
        push   dword ptr [0x10f8]            ; -> obj1:0x10F8
        VMMCall  VMM._EnterMutex                 ; cd202e010100
        add    esp, 8
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
loc_4_1EA5:
        VMMCall  VMM.Get_Next_Thread_Handle      ; cd2013010100
        VMMCall  VMM.Test_Cur_Thread_Handle      ; cd2009010100
        je     loc_4_1F6C
        mov    ebx, dword ptr [edi + 0x14]
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_4_1EA5
        mov    edx, edi
        add    edx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    edx, dword ptr [edx]
        or     edx, edx
        je     loc_4_1EA5
        test   byte ptr [edx + 0x2d], 0x40
        jne    loc_4_1EA5
        test   byte ptr [edx + 0x2d], 0x20
        jne    loc_4_1EA5
        lea    eax, [esp + 0xc]
        mov    dword ptr [esp + 0xc], 0x10001
        push   eax
        push   edi
        VxDCall  NDIS.Service_0003h              ; cd2003002a00
        add    esp, 8
        or     eax, eax
        je     loc_4_1EA5
        push   0
        push   dword ptr [esp + 0xcc]
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_4_1EA5
        add    eax, dword ptr [esp + 0xc4]
        mov    esi, 0x3ac                    ; -> loc_5_03AC
loc_4_1F1B:
        cmp    esi, 0x46c                    ; -> loc_5_046C
        je     loc_4_1F61
        cmp    eax, dword ptr [esi]
        jb     loc_4_1F5C
        cmp    eax, dword ptr [esi + 4]
        ja     loc_4_1F5C
        cmp    eax, dword ptr [esi + 8]
        je     loc_4_1F61
        call   sub_4_209B
        mov    ebx, dword ptr [esi + 0xc]
        mov    byte ptr [ebx], 0xcc
        mov    edx, edi
        add    edx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    edx, dword ptr [edx]
        bts    dword ptr [edx + 0x2c], 0xd
        jb     loc_4_1EA5
        inc    dword ptr [0x46c]             ; -> loc_5_046C
        jmp    loc_4_1EA5
loc_4_1F5C:
        add    esi, 0x10
        jmp    loc_4_1F1B
loc_4_1F61:
        push   edi
        call   sub_4_1BC1
        jmp    loc_4_1EA5
loc_4_1F6C:
        mov    esi, dword ptr [esp + 0xdc]
        mov    edx, esi
        xchg   dword ptr [0x35c], esi        ; -> loc_5_035C
        mov    dword ptr [edx], esi
        inc    byte ptr [0x474]              ; -> loc_5_0474
        cmp    dword ptr [0x470], 0          ; -> loc_5_0470
        jne    loc_4_1F91
        call   sub_4_200A
loc_4_1F91:
        push   dword ptr [0x10f8]            ; -> obj1:0x10F8
        VMMCall  VMM._LeaveMutex                 ; cd202f010100
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xcc
        ret    4
sub_4_1FAC:
        cmp    esi, 0xc
        jne    loc_4_1FE2
        movzx  eax, word ptr [ebp + 0x28]
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_4_1FE2
        add    eax, dword ptr [ebp + 0x24]
        dec    eax
        mov    edx, 0x3ac                    ; -> loc_5_03AC
loc_4_1FD0:
        cmp    edx, 0x46c                    ; -> loc_5_046C
        je     loc_4_1FE2
        cmp    eax, dword ptr [edx + 0xc]
        je     sub_4_1FE4
        add    edx, 0x10
        jmp    loc_4_1FD0
loc_4_1FE2:
        clc    
        ret    
sub_4_1FE4:
        mov    edx, dword ptr [0x10e0]       ; -> obj1:0x10E0
        btr    dword ptr [edx + 0x2c], 0xd
        jae    loc_4_2099
        dec    dword ptr [0x46c]             ; -> loc_5_046C
        jne    loc_4_2086
        cmp    dword ptr [0x470], 0          ; -> loc_5_0470
        jne    loc_4_2086
sub_4_200A:
        cmp    dword ptr [0x46c], 0          ; -> loc_5_046C
        jne    loc_4_2099
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        xchg   dword ptr [0x35c], esi        ; -> loc_5_035C
loc_4_2022:
        or     esi, esi
        je     loc_4_2058
        dec    byte ptr [0x474]              ; -> loc_5_0474
        push   0x35
        push   dword ptr [0x10f8]            ; -> obj1:0x10F8
        VMMCall  VMM._EnterMutex                 ; cd202e010100
        add    esp, 8
        mov    edx, dword ptr [esi + 8]
        push   esi
        call   dword ptr [esi + 4]
        pop    esi
        push   dword ptr [0x10f8]            ; -> obj1:0x10F8
        VMMCall  VMM._LeaveMutex                 ; cd202f010100
        add    esp, 4
        mov    esi, dword ptr [esi]
        jmp    loc_4_2022
loc_4_2058:
        inc    byte ptr [0x366]              ; -> loc_5_0366
        call   sub_4_1B7A
        push   dword ptr [0x34c]             ; -> loc_5_034C
        call   sub_4_4581
        mov    ebx, 0x3ac                    ; -> loc_5_03AC
loc_4_2073:
        cmp    ebx, 0x46c                    ; -> loc_5_046C
        je     loc_4_2096
        mov    esi, dword ptr [ebx + 0xc]
        mov    byte ptr [esi], 0x90
        add    ebx, 0x10
        jmp    loc_4_2073
loc_4_2086:
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        push   edi
        call   sub_4_1BC1
        pop    edi
        jmp    loc_4_2099
loc_4_2096:
        pop    edi
        pop    esi
        pop    ebx
loc_4_2099:
        stc    
        ret    
sub_4_209B:
        push   ebx
        push   esi
        push   edi
        VMMCall  VMM.Test_Debug_Installed        ; cd20c1000100
        je     loc_4_20AF
        xor    bx, bx
        mov    ax, 0x20
        int    0x41
loc_4_20AF:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h
sub_4_20B4:
        enter  4, 0
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        or     dword ptr [edi], 0xc0000
        call   sub_4_236D
        or     eax, eax
        je     loc_4_2143
        push   0x10
        push   0x8f
        push   0xbff70
        VMMCall  VMM._PageReserve                ; cd201d010100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_4_2143
        mov    esi, 0x118c                   ; -> loc_1_118C
        mov    ebx, 1
loc_4_20F1:
        mov    ax, 0x3da0
        mov    edx, dword ptr [esi + 4]
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_4_2129
        mov    ecx, dword ptr [esi + 0xc]
        mov    word ptr [ecx*2 + 0x119c], ax ; -> obj1:0x119C
        call   sub_4_2147
        or     eax, eax
        je     loc_4_2129
        add    esi, 0x10
        dec    ebx
        jne    loc_4_20F1
        mov    eax, 1
loc_4_2121:
        and    dword ptr [edi], 0xfff3ffff
        leave  
        ret    
loc_4_2129:
        call   sub_4_2433
        push   0x10
        push   0xbff70000
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        call   sub_4_245A
loc_4_2143:
        sub    eax, eax
        jmp    loc_4_2121
sub_4_2147:
        enter  0xc, 0
        push   ebx
        push   edi
        push   0x60040000
        mov    eax, dword ptr [esi + 0xc]
        shl    eax, 0x16
        ror    eax, 3
        push   eax
        push   dword ptr [0x11a6]            ; -> obj1:0x11A6
        push   1
        mov    eax, dword ptr [esi + 8]
        shr    eax, 0xc
        push   eax
        VMMCall  VMM._PageCommit                 ; cd201e010100
        add    esp, 0x14
        or     eax, eax
        je     loc_4_22DD
        call   sub_4_22E1
        jb     loc_4_22DD
loc_4_2187:
        test   dword ptr [ebx + 0x24], 0x80000000
        je     loc_4_21A8
        mov    dword ptr [ebp - 0xc], 0x60060000
        mov    eax, dword ptr [0x11aa]       ; -> obj1:0x11AA
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], 1
        jmp    loc_4_21BF
loc_4_21A8:
        mov    dword ptr [ebp - 0xc], 0x60040000
        mov    eax, dword ptr [0x11a6]       ; -> obj1:0x11A6
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0x11ae]       ; -> obj1:0x11AE
        mov    dword ptr [ebp - 8], eax
loc_4_21BF:
        test   dword ptr [ebx + 0x24], 0x80
        je     loc_4_21E3
        mov    eax, dword ptr [ebx + 0x10]
        or     eax, eax
        jne    loc_4_21D2
        mov    eax, dword ptr [ebx + 8]
loc_4_21D2:
        add    eax, 0xfff
        and    eax, 0xfffff000
        xor    edx, edx
        jmp    loc_4_22A3
loc_4_21E3:
        cmp    dword ptr [ebx + 0x10], 0x1000
        jb     loc_4_2226
        push   dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebx + 0x14]
        shr    eax, 9
        mov    edx, dword ptr [esi + 0xc]
        shl    edx, 0x16
        or     eax, edx
        ror    eax, 3
        push   eax
        push   dword ptr [ebp - 4]
        mov    eax, dword ptr [ebx + 0x10]
        shr    eax, 0xc
        push   eax
        mov    eax, dword ptr [ebx + 0xc]
        add    eax, dword ptr [esi + 8]
        shr    eax, 0xc
        push   eax
        VMMCall  VMM._PageCommit                 ; cd201e010100
        add    esp, 0x14
        or     eax, eax
        je     loc_4_22DD
loc_4_2226:
        mov    eax, dword ptr [ebx + 0x10]
        test   eax, 0xfff
        je     loc_4_2281
        push   dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebx + 0x10]
        and    eax, 0xfffff000
        add    eax, dword ptr [ebx + 0x14]
        shr    eax, 9
        mov    edx, dword ptr [esi + 0xc]
        shl    edx, 0x16
        or     edx, eax
        mov    eax, dword ptr [ebx + 0x10]
        shr    eax, 9
        neg    eax
        and    eax, 7
        shl    eax, 0x13
        or     eax, edx
        ror    eax, 3
        push   eax
        push   dword ptr [ebp - 4]
        push   1
        mov    eax, dword ptr [ebx + 0x10]
        and    eax, 0xfffff000
        add    eax, dword ptr [ebx + 0xc]
        add    eax, dword ptr [esi + 8]
        shr    eax, 0xc
        push   eax
        VMMCall  VMM._PageCommit                 ; cd201e010100
        add    esp, 0x14
        or     eax, eax
        je     loc_4_22DD
loc_4_2281:
        jmp    loc_4_22CA
        db  08Bh, 043h, 008h, 005h, 0FFh, 00Fh, 000h, 000h, 025h, 000h, 0F0h, 0FFh, 0FFh, 08Bh, 053h, 010h
        db  081h, 0C2h, 0FFh, 00Fh, 000h, 000h, 081h, 0E2h, 000h, 0F0h, 0FFh, 0FFh, 02Bh, 0C2h, 076h, 027h
loc_4_22A3:
        shr    eax, 0xc
        mov    ecx, eax
        push   dword ptr [ebp - 0xc]
        push   0
        push   dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebx + 0xc]
        add    eax, dword ptr [esi + 8]
        add    eax, edx
        shr    eax, 0xc
        push   eax
        VMMCall  VMM._PageCommit                 ; cd201e010100
        add    esp, 0x14
        or     eax, eax
        je     loc_4_22DD
loc_4_22CA:
        add    ebx, 0x28
        dec    edi
        jne    loc_4_2187
        mov    eax, 1
loc_4_22D9:
        pop    edi
        pop    ebx
        leave  
        ret    
loc_4_22DD:
        sub    eax, eax
        jmp    loc_4_22D9
sub_4_22E1:
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [edx + 0x3c]
        cmp    eax, 0xf40
        ja     loc_4_236B
        add    edx, eax
        cmp    dword ptr [edx], 0x4550
        jne    loc_4_236B
        mov    eax, dword ptr [edx + 0x34]
        cmp    eax, dword ptr [esi + 8]
        jne    loc_4_236B
        movzx  eax, word ptr [edx + 6]
        imul   eax, eax, 0x28
        add    eax, 0x18
        movzx  ecx, word ptr [edx + 0x14]
        add    eax, ecx
        add    eax, edx
        sub    eax, dword ptr [esi + 8]
        cmp    eax, 0x1000
        ja     loc_4_236B
        mov    ebx, 0x18
        movzx  ecx, word ptr [edx + 0x14]
        add    ebx, ecx
        add    ebx, edx
        movzx  ecx, word ptr [edx + 6]
loc_4_232D:
        test   dword ptr [ebx + 0x10], 0x1ff
        jne    loc_4_236B
        mov    eax, dword ptr [ebx + 0x14]
        add    eax, dword ptr [ebx + 0x10]
        cmp    eax, 0x8f000
        ja     loc_4_236B
        mov    eax, dword ptr [ebx + 0xc]
        test   eax, 0xfff
        jne    loc_4_236B
        add    eax, dword ptr [ebx + 8]
        cmp    eax, 0x8f000
        ja     loc_4_236B
        add    ebx, 0x28
        loop   loc_4_232D
        lea    ebx, [edx + 0x18]
        movzx  ecx, word ptr [edx + 0x14]
        add    ebx, ecx
        movzx  edi, word ptr [edx + 6]
        clc    
        ret    
loc_4_236B:
        stc    
        ret    
sub_4_236D:
        push   ebx
        push   0x485                         ; -> obj5:0x0485
        push   1
        VMMCall  VMM._PagerQuery                 ; cd2021010100
        add    esp, 8
        or     eax, eax
        je     loc_4_2406
        mov    ebx, dword ptr [0x485]        ; -> obj5:0x0485
        mov    eax, 0x268                    ; -> loc_1_0268
        mov    dword ptr [0x485], eax        ; -> obj5:0x0485
        push   0x485                         ; -> obj5:0x0485
        VMMCall  VMM._PagerRegister              ; cd2020010100
        add    esp, 4
        or     eax, eax
        je     loc_4_2406
        mov    dword ptr [0x11aa], eax       ; -> obj1:0x11AA
        sub    eax, eax
        mov    dword ptr [0x489], 0x268      ; -> obj5:0x0489
        mov    dword ptr [0x491], eax        ; -> obj5:0x0491
        mov    dword ptr [0x499], eax        ; -> obj5:0x0499
        mov    dword ptr [0x49d], eax        ; -> obj5:0x049D
        mov    dword ptr [0x4a1], 1          ; -> obj5:0x04A1
        push   0x485                         ; -> obj5:0x0485
        VMMCall  VMM._PagerRegister              ; cd2020010100
        add    esp, 4
        or     eax, eax
        je     loc_4_2408
        mov    dword ptr [0x11a6], eax       ; -> obj1:0x11A6
        mov    dword ptr [0x485], ebx        ; -> obj5:0x0485
        push   0x485                         ; -> obj5:0x0485
        VMMCall  VMM._PagerRegister              ; cd2020010100
        add    esp, 4
        or     eax, eax
        je     loc_4_2408
        mov    dword ptr [0x11ae], eax       ; -> obj1:0x11AE
loc_4_2406:
        pop    ebx
        ret    
loc_4_2408:
        push   dword ptr [0x11aa]            ; -> obj1:0x11AA
        VMMCall  VMM._PagerDeregister            ; cd2022010100
        add    esp, 4
        cmp    dword ptr [0x11a6], 0         ; -> obj1:0x11A6
        je     loc_4_242F
        push   dword ptr [0x11a6]            ; -> obj1:0x11A6
        VMMCall  VMM._PagerDeregister            ; cd2022010100
        add    esp, 4
loc_4_242F:
        sub    eax, eax
        jmp    loc_4_2406
sub_4_2433:
        mov    ecx, 1
        sub    ebx, ebx
loc_4_243A:
        mov    ax, word ptr [ebx*2 + 0x119c] ; -> obj1:0x119C
        cmp    ax, -1
        je     loc_4_2456
        push   ebx
        mov    ebx, eax
        mov    ah, 0x3e
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebx
loc_4_2456:
        inc    ebx
        loop   loc_4_243A
        ret    
sub_4_245A:
        push   dword ptr [0x11a6]            ; -> obj1:0x11A6
        VMMCall  VMM._PagerDeregister            ; cd2022010100
        add    esp, 4
        push   dword ptr [0x11aa]            ; -> obj1:0x11AA
        VMMCall  VMM._PagerDeregister            ; cd2022010100
        add    esp, 4
        push   dword ptr [0x11ae]            ; -> obj1:0x11AE
        VMMCall  VMM._PagerDeregister            ; cd2022010100
        add    esp, 4
        ret    
sub_4_2488:
        mov    eax, dword ptr [0x11bc]       ; -> obj1:0x11BC
        mov    ecx, 0x35
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    edx, dword ptr [0x10e0]       ; -> obj1:0x10E0
        test   byte ptr [edx + 0x2d], 1
        jne    loc_4_24B1
        cmp    dword ptr [edx + 0x28], 0
        jne    loc_4_24B0
        cmp    dword ptr [edx + 0x20], 0
        jne    loc_4_24B1
loc_4_24B0:
        ret    
loc_4_24B1:
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        call   sub_1_0779
        pop    edi
        or     byte ptr [edx + 0x2c], 4
        push   eax
        call   sub_1_06A5
        jmp    sub_4_2488
sub_4_24D0:
        mov    eax, dword ptr [0x11bc]       ; -> obj1:0x11BC
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        ret    
sub_4_24DC:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     loc_4_24F5
        mov    ecx, dword ptr [eax]
        test   ecx, ecx
        je     loc_4_24F5
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [0x1118], edx       ; -> obj1:0x1118
loc_4_24F5:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [0x1100], eax       ; -> obj1:0x1100
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [0x1104], eax       ; -> obj1:0x1104
        sub    ecx, ecx
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jb     loc_4_2516
        mov    dword ptr [0x11bc], eax       ; -> obj1:0x11BC
loc_4_2514:
        leave  
        ret    
loc_4_2516:
        sub    eax, eax
        jmp    loc_4_2514
sub_4_251A:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0x1110], eax       ; -> obj1:0x1110
        mov    eax, 0x31
        VMMCall  VMM.System_Control              ; cd2093000100
        leave  
        ret    
loc_4_2532:
        mov    dword ptr [0x1110], 0         ; -> obj1:0x1110
        mov    eax, 0x32
        VMMCall  VMM.System_Control              ; cd2093000100
        ret    8
sub_4_254A:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [0x11bc]       ; -> obj1:0x11BC
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        leave  
        ret    
sub_4_255A:
        push   ebp
        mov    ebp, esp
        push   esi
        push   1
        push   0x79
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_25E0
        mov    esi, eax
        sub    ecx, ecx
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jb     loc_4_25D4
        mov    dword ptr [esi + 0x18], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 4], eax
        or     byte ptr [esi + 0x62], 2
        or     eax, eax
        jne    loc_4_25A9
        xor    ecx, ecx
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        push   ebx
        jmp    loc_4_25AF
loc_4_25A9:
        push   eax
        call   sub_4_4312
loc_4_25AF:
        mov    word ptr [esi + 0x76], ax
        test   dword ptr [ebp + 0x14], 2
        je     loc_4_25C0
        or     dword ptr [esi + 0x2c], 0x40
loc_4_25C0:
        test   dword ptr [ebp + 0x14], 1
        je     loc_4_25CD
        or     dword ptr [esi + 0x2c], 0x20
loc_4_25CD:
        mov    eax, esi
loc_4_25CF:
        pop    esi
        leave  
        ret    0x10
loc_4_25D4:
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_25E0:
        sub    eax, eax
        jmp    loc_4_25CF
sub_4_25E4:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x18]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        pop    edi
        leave  
        ret    4
loc_4_25F9:
        push   edi
        pop    edi
        ret    
sub_4_25FC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        add    edi, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    edi, dword ptr [edi]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_255A
        or     eax, eax
        je     loc_4_265D
        mov    dword ptr [edi + 0xc], eax
        mov    ecx, dword ptr [ebp + 0x28]
        mov    dword ptr [ecx], eax
        or     byte ptr [eax + 0x2e], 2
        push   eax
        push   0x25f9                        ; -> loc_4_25F9
        push   0x2a
        push   0
        push   0
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x24]
        push   dword ptr [ebp + 0x20]
        VMMCall  VMM.VMMCreateThread             ; cd2005010100
        add    esp, 0x24
        or     eax, eax
        je     loc_4_2662
        mov    ecx, dword ptr [edi + 0xc]
        and    byte ptr [ecx + 0x2e], 0xfd
loc_4_265D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_4_2662:
        push   dword ptr [edi + 0xc]
        call   sub_4_25E4
        mov    dword ptr [edi + 0xc], 0
        sub    eax, eax
        stc    
        jmp    loc_4_265D
loc_4_2676:
        mov    ebx, dword ptr [ebp + 0x10]
        xor    eax, eax
        mov    dword ptr [0x1114], eax       ; -> obj1:0x1114
        mov    byte ptr [0x11c0], al         ; -> obj1:0x11C0
        push   dword ptr [ebx]
        push   dword ptr [0x29c]             ; -> obj5:0x029C
        call   sub_4_45C3
        VMMCall  VMM.Get_Sys_Thread_Handle       ; cd200a010100
        push   0
        push   edi
        call   sub_1_0836
        add    esp, 8
        push   ebx
        call   sub_4_26AD
        add    esp, 4
        ret    
sub_4_26AD:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx]
        push   dword ptr [esi + 0x40]
        push   dword ptr [ebx + 0x14]
        call   sub_4_1A5C
        add    esp, 8
        mov    edi, dword ptr [esi + 0x64]
        or     edi, edi
        je     loc_4_26E2
        test   dword ptr [esi + 0x44], 0x800
        je     loc_4_26E2
        push   0x10
        push   edi
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_4_26E2:
        xor    edi, edi
        xchg   dword ptr [esi + 0x5c], edi
        mov    edx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    esi, dword ptr [edi + edx]
        mov    dword ptr [esi + 0xc], ebx
        push   edi
        VMMCall  VMM.VMMTerminateThread          ; cd2007010100
        add    esp, 4
loc_4_26FD:
        mov    edx, dword ptr [esp + 4]
        xor    eax, eax
        xchg   dword ptr [edx + 0xc], eax
        or     eax, eax
        je     loc_4_2717
        mov    edi, dword ptr [edx]
        call   sub_4_1D59
        push   edx
        call   sub_4_431D
loc_4_2717:
        ret    4
loc_4_271A:
        xor    eax, eax
        xchg   dword ptr [edx + 0x10], eax
        or     eax, eax
        je     loc_4_273F
        mov    ebx, dword ptr [edx]
        mov    ebx, dword ptr [ebx + 0x5c]
        xor    eax, eax
        mov    edi, 0x2710
        mov    ecx, 0x130
        mov    esi, 0x2740                   ; -> loc_4_2740
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
loc_4_273F:
        ret    
loc_4_2740:
        jb     loc_4_2781
        xor    eax, eax
        xchg   dword ptr [edx + 0xc], eax
        or     eax, eax
        je     loc_4_2765
        mov    ebx, edx
        call   sub_1_0A44
        mov    edi, dword ptr [ebx]
        call   sub_4_1D59
        VMMCall  VMM.Cleanup_Thread_State        ; cd2070010100
        push   ebx
        call   sub_4_431D
loc_4_2765:
        ret    
loc_4_2766:
        xor    eax, eax
        xchg   dword ptr [edx + 0xc], eax
        or     eax, eax
        je     loc_4_2781
        mov    edi, dword ptr [edx]
        mov    edi, dword ptr [edi + 0x5c]
        mov    ebx, edx
        call   sub_1_0A44
        push   ebx
        call   sub_4_431D
loc_4_2781:
        ret    
sub_4_2782:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [0x10ec]       ; -> obj1:0x10EC
        add    esi, edi
        mov    esi, dword ptr [esi]
        or     byte ptr [esi + 0x2e], 4
        xor    eax, eax
        mov    dword ptr [ebx + 0x10], eax
        mov    dword ptr [ebx + 0xc], eax
        mov    edx, esi
        call   sub_1_0779
        push   2
        push   edi
        push   ebx
        push   0x26fd                        ; -> loc_4_26FD
        call   _VWIN32_QueueKernelAPC
        add    esp, 0x10
        mov    dword ptr [ebx + 0xc], eax
        mov    eax, dword ptr [ebx]
        mov    eax, dword ptr [eax + 0x40]
        test   dword ptr [eax + 0x20], 0x80
        je     loc_4_27FF
        push   esi
        mov    eax, 0x1388
        mov    edx, ebx
        mov    esi, 0x271a                   ; -> loc_4_271A
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [ebx + 0x10], esi
        pop    esi
        push   ebx
        push   esi
        push   edi
        mov    ecx, 0x2766                   ; -> loc_4_2766
        call   sub_4_1CC9
        pop    edi
        pop    esi
        pop    ebx
        mov    eax, 0x800000
        VMMCall  VMM.Adjust_Thread_Exec_Priority ; cd2014010100
loc_4_27FF:
        or     byte ptr [esi + 0x2d], 2
        and    byte ptr [esi + 0x2d], 0xfb
        push   edi
        call   sub_4_1C1B
        cmp    dword ptr [esi + 0x1c], 0
        jg     loc_4_281E
        push   -1
        push   edi
        call   sub_1_0836
        add    esp, 8
loc_4_281E:
        xor    eax, eax
        xchg   dword ptr [esi + 0x44], eax
        or     eax, eax
        je     loc_4_2831
        push   eax
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
loc_4_2831:
        cmp    dword ptr [esi + 0x20], 0
        je     loc_4_2847
        mov    dword ptr [esi + 0x20], 1
        push   edi
        call   sub_1_073E
        add    esp, 4
loc_4_2847:
        btr    dword ptr [esi + 0x2c], 4
        jae    loc_4_285E
        lea    eax, [esi + 0x289816]
        push   eax
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
loc_4_285E:
        and    byte ptr [esi + 0x2e], 0xfb
        lea    eax, [esi + 0x569595]
        push   eax
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_4_2877:
        push   edi
        push   esi
        push   ebx
        push   eax
        lea    edx, [eax + 0xc]
        mov    edi, edx
        mov    esi, dword ptr [ebx + 0x34]
        mov    ah, byte ptr [esi - 4]
        cmp    ah, 0xf
        ja     loc_4_288D
        xor    ah, ah
loc_4_288D:
        mov    ecx, dword ptr [edx]
        jecxz  loc_4_28A0
        mov    esi, dword ptr [ecx + 0x34]
        mov    al, byte ptr [esi - 4]
        cmp    ah, al
        jg     loc_4_28A0
        lea    edx, [ecx + 0x54]
        jmp    loc_4_288D
loc_4_28A0:
        cmp    edi, edx
        jne    loc_4_28E4
        lea    eax, [edi - 0xc]
        cmp    eax, dword ptr [0x29c]        ; -> obj5:0x029C
        je     loc_4_28E4
        push   edx
        mov    edx, eax
        jecxz  loc_4_28C1
        mov    edi, dword ptr [ecx + 0x5c]
        or     edi, edi
        je     loc_4_28C1
        VMMCall  VMM.Release_Inversion_Pri_ID    ; cd206a010100
loc_4_28C1:
        push   ebx
        mov    eax, dword ptr [ebx + 0x34]
        mov    eax, dword ptr [eax]
        mov    ebx, dword ptr [ebx + 0x5c]
        or     ebx, ebx
        je     loc_4_28E2
        mov    edi, dword ptr [edx + 8]
        or     edi, edi
        je     loc_4_28E2
        mov    edi, dword ptr [edi + 0x5c]
        or     edi, edi
        je     loc_4_28E2
        VMMCall  VMM.Set_Inversion_Pri           ; cd2063010100
loc_4_28E2:
        pop    ebx
        pop    edx
loc_4_28E4:
        mov    dword ptr [ebx + 0x54], ecx
        mov    dword ptr [edx], ebx
        pop    eax
        pop    ebx
        pop    esi
        pop    edi
        ret    
sub_4_28EE:
        push   eax
        push   esi
        push   edi
        lea    edx, [eax + 0xc]
        cmp    dword ptr [edx], ebx
        jne    loc_4_292F
        mov    esi, eax
        mov    edi, dword ptr [ebx + 0x5c]
        VMMCall  VMM.Release_Inversion_Pri_ID    ; cd206a010100
        mov    edi, dword ptr [esi + 8]
        or     edi, edi
        je     loc_4_292F
        mov    ecx, dword ptr [ebx + 0x54]
        jecxz  loc_4_292F
        push   ebx
        mov    edx, esi
        mov    edi, dword ptr [edi + 0x5c]
        or     edi, edi
        je     loc_4_292B
        mov    eax, dword ptr [ebx + 0x34]
        mov    eax, dword ptr [eax]
        mov    ebx, dword ptr [ebx + 0x5c]
        or     ebx, ebx
        je     loc_4_292B
        VMMCall  VMM.Set_Inversion_Pri           ; cd2063010100
loc_4_292B:
        pop    ebx
        lea    edx, [esi + 0xc]
loc_4_292F:
        mov    ecx, dword ptr [edx]
        jecxz  loc_4_2943
        cmp    ecx, ebx
        je     loc_4_293C
        lea    edx, [ecx + 0x54]
        jmp    loc_4_292F
loc_4_293C:
        xor    ecx, ecx
        xchg   dword ptr [ebx + 0x54], ecx
        mov    dword ptr [edx], ecx
loc_4_2943:
        pop    edi
        pop    esi
        pop    eax
        ret    
sub_4_2947:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   0x35
        push   dword ptr [0x10f8]            ; -> obj1:0x10F8
        VMMCall  VMM._EnterMutex                 ; cd202e010100
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
loc_4_295F:
        mov    ebx, dword ptr [eax + 0xc]
        or     ebx, ebx
        je     loc_4_297F
        call   sub_4_28EE
        push   eax
        push   0x80
        push   dword ptr [ebx + 0x5c]
        call   sub_1_0836
        add    esp, 8
        pop    eax
        jmp    loc_4_295F
loc_4_297F:
        push   dword ptr [0x10f8]            ; -> obj1:0x10F8
        VMMCall  VMM._LeaveMutex                 ; cd202f010100
        add    esp, 4
        pop    ebx
        leave  
        ret    
sub_4_2991:
        push   ebp
        mov    ebp, esp
        push   ebx
loc_4_2995:
        push   0x35
        push   dword ptr [0x10f8]            ; -> obj1:0x10F8
        VMMCall  VMM._EnterMutex                 ; cd202e010100
        add    esp, 8
        mov    ebx, dword ptr [0x10d8]       ; -> obj1:0x10D8
        mov    eax, dword ptr [ebp + 8]
loc_4_29AF:
        cmp    dword ptr [eax + 4], -1
        je     loc_4_2A91
        mov    dword ptr [ebx + 0x58], 2
        call   sub_4_2877
loc_4_29C5:
        or     byte ptr [eax + 3], 0x80
        push   dword ptr [0x10f8]            ; -> obj1:0x10F8
        VMMCall  VMM._LeaveMutex                 ; cd202f010100
        add    esp, 4
        call   sub_4_1FE4
        push   -1
        call   sub_1_0790
        add    esp, 4
        push   0x35
        push   dword ptr [0x10f8]            ; -> obj1:0x10F8
        VMMCall  VMM._EnterMutex                 ; cd202e010100
        add    esp, 8
        mov    edx, dword ptr [0x10e0]       ; -> obj1:0x10E0
        and    byte ptr [edx + 0x2c], 0xfd
        mov    edx, dword ptr [edx + 0x14]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 8], ebx
        jne    loc_4_2A16
        and    byte ptr [eax + 3], 0x7f
        jmp    loc_4_2AC6
loc_4_2A16:
        cmp    edx, 0x40000001
        je     loc_4_2A36
        cmp    edx, -1
        jne    loc_4_2A7E
        mov    edx, dword ptr [ebx + 0x60]
        cmp    dword ptr [edx + 0x28], 0
        jne    loc_4_2A7E
        call   sub_4_28EE
        jmp    loc_4_2ACC
loc_4_2A36:
        mov    edx, dword ptr [ebx + 0x60]
        test   byte ptr [edx + 0x2d], 1
        jne    loc_4_2A53
        cmp    dword ptr [edx + 0x20], 0
        je     loc_4_29AF
        cmp    dword ptr [edx + 0x28], 0
        jne    loc_4_29AF
loc_4_2A53:
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        call   sub_1_0779
        pop    edi
        or     byte ptr [edx + 0x2c], 4
        push   dword ptr [0x10f8]            ; -> obj1:0x10F8
        VMMCall  VMM._LeaveMutex                 ; cd202f010100
        add    esp, 4
        push   eax
        call   sub_1_06A5
        jmp    loc_4_2995
loc_4_2A7E:
        cmp    dword ptr [eax + 4], -1
        jne    loc_4_29C5
        and    byte ptr [eax + 3], 0x7f
        call   sub_4_28EE
loc_4_2A91:
        push   edi
        push   eax
        mov    ebx, dword ptr [eax + 0xc]
        cmp    eax, dword ptr [0x29c]        ; -> obj5:0x029C
        je     loc_4_2AC0
        or     ebx, ebx
        je     loc_4_2AC0
        mov    edx, eax
        mov    edi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [edi + 0x5c]
        or     edi, edi
        je     loc_4_2AC0
        mov    eax, dword ptr [ebx + 0x34]
        mov    eax, dword ptr [eax]
        mov    ebx, dword ptr [ebx + 0x5c]
        or     ebx, ebx
        je     loc_4_2AC0
        VMMCall  VMM.Set_Inversion_Pri           ; cd2063010100
loc_4_2AC0:
        pop    eax
        pop    edi
        add    dword ptr [eax + 4], 2
loc_4_2AC6:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 8], ebx
loc_4_2ACC:
        push   dword ptr [0x10f8]            ; -> obj1:0x10F8
        VMMCall  VMM._LeaveMutex                 ; cd202f010100
        add    esp, 4
        clc    
        pop    ebx
        leave  
        ret    
sub_4_2ADF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   0x35
        push   dword ptr [0x10f8]            ; -> obj1:0x10F8
        VMMCall  VMM._EnterMutex                 ; cd202e010100
        add    esp, 8
loc_4_2AF4:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0xc]
        or     ecx, ecx
        je     loc_4_2B6C
        mov    ebx, ecx
        mov    edx, dword ptr [ebx + 0x60]
        test   byte ptr [edx + 0x2d], 1
        jne    loc_4_2B15
        cmp    dword ptr [edx + 0x20], 0
        je     loc_4_2B2C
        cmp    dword ptr [edx + 0x28], 0
        jne    loc_4_2B2C
loc_4_2B15:
        call   sub_4_28EE
        push   0x40000001
        push   dword ptr [ebx + 0x5c]
        call   sub_1_0836
        add    esp, 8
        jmp    loc_4_2AF4
loc_4_2B2C:
        push   edi
        push   eax
        mov    edi, dword ptr [ebx + 0x5c]
        VMMCall  VMM.Release_Inversion_Pri_ID    ; cd206a010100
        pop    eax
        pop    edi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ecx + 0x34]
        mov    edx, dword ptr [edx]
        mov    ecx, dword ptr [ebx + 0x34]
        cmp    dword ptr [ecx], edx
        ja     loc_4_2B50
        jb     loc_4_2B5C
        test   byte ptr [eax + 3], 0x80
        je     loc_4_2B5C
loc_4_2B50:
        call   sub_4_28EE
        mov    dword ptr [eax + 8], ebx
        add    dword ptr [eax + 4], 2
loc_4_2B5C:
        push   0
        push   dword ptr [ebx + 0x5c]
        call   sub_1_0836
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
loc_4_2B6C:
        dec    dword ptr [eax + 4]
        push   dword ptr [0x10f8]            ; -> obj1:0x10F8
        VMMCall  VMM._LeaveMutex                 ; cd202f010100
        add    esp, 4
        clc    
        pop    ebx
        leave  
        ret    
loc_4_2B82:
        push   dword ptr [0x10d8]            ; -> obj1:0x10D8
        push   dword ptr [esp + 0x10]
        call   sub_4_2991
        add    esp, 8
        ret    0xc
loc_4_2B97:
        push   dword ptr [0x10d8]            ; -> obj1:0x10D8
        push   dword ptr [esp + 0x10]
        call   sub_4_2ADF
        add    esp, 8
        ret    0xc
VWIN32_EnterCrstR0:
        mov    edx, dword ptr [edx + 0xc]
sub_4_2BAF:
        mov    ecx, dword ptr [0x10d8]       ; -> obj1:0x10D8
sub_4_2BB5:
        inc    dword ptr [0x470]             ; -> loc_5_0470
        dec    dword ptr [edx + 0x10]
        je     loc_4_2BFD
        cmp    dword ptr [edx + 8], ecx
        je     loc_4_2C00
        dec    dword ptr [0x470]             ; -> loc_5_0470
        cmp    edx, dword ptr [0x2ac]        ; -> obj5:0x02AC
        je     loc_4_2BE5
        push   ecx
        push   edx
        call   sub_4_2991
        add    esp, 8
        inc    dword ptr [0x470]             ; -> loc_5_0470
        jmp    loc_4_2C03
loc_4_2BE5:
        push   ecx
        mov    eax, dword ptr [0x11bc]       ; -> obj1:0x11BC
        mov    ecx, 0x35
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        pop    ecx
        inc    dword ptr [0x470]             ; -> loc_5_0470
loc_4_2BFD:
        mov    dword ptr [edx + 8], ecx
loc_4_2C00:
        inc    dword ptr [edx + 4]
loc_4_2C03:
        dec    dword ptr [0x470]             ; -> loc_5_0470
        jne    loc_4_2C19
        cmp    byte ptr [0x474], 0           ; -> loc_5_0474
        je     loc_4_2C19
        call   sub_4_200A
loc_4_2C19:
        clc    
        ret    
VWIN32_LeaveCrstR0:
        mov    edx, dword ptr [edx + 0xc]
sub_4_2C1E:
        mov    ecx, dword ptr [0x10d8]       ; -> obj1:0x10D8
sub_4_2C24:
        inc    dword ptr [0x470]             ; -> loc_5_0470
        dec    dword ptr [edx + 4]
        je     loc_4_2C34
        inc    dword ptr [edx + 0x10]
        jmp    loc_4_2C5D
loc_4_2C34:
        xor    eax, eax
        mov    dword ptr [edx + 8], eax
        inc    dword ptr [edx + 0x10]
        jg     loc_4_2C5D
        cmp    edx, dword ptr [0x2ac]        ; -> obj5:0x02AC
        je     loc_4_2C52
        push   ecx
        push   edx
        call   sub_4_2ADF
        add    esp, 8
        jmp    loc_4_2C5D
loc_4_2C52:
        mov    eax, dword ptr [0x11bc]       ; -> obj1:0x11BC
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
loc_4_2C5D:
        dec    dword ptr [0x470]             ; -> loc_5_0470
        jne    loc_4_2C73
        cmp    byte ptr [0x474], 0           ; -> loc_5_0474
        je     loc_4_2C73
        call   sub_4_200A
loc_4_2C73:
        clc    
        ret    
sub_4_2C75:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        VMMCall  VMM.Adjust_Thread_Exec_Priority ; cd2014010100
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_4_2C8C:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        VMMCall  VMM.Set_Thread_Win32_Pri        ; cd2067010100
        pop    edi
        leave  
        ret    
sub_4_2C9F:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Boost_With_Decay            ; cd2062010100
        pop    edi
        leave  
        ret    
sub_4_2CB5:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        VMMCall  VMM.Set_Thread_Static_Boost     ; cd2068010100
        pop    edi
        leave  
        ret    
sub_4_2CC8:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        VMMCall  VMM.Set_Group_Static_Boost      ; cd206d010100
        pop    edi
        leave  
        ret    
sub_4_2CDB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Set_Inversion_Pri           ; cd2063010100
        pop    edi
        pop    ebx
        leave  
        ret    
sub_4_2CF6:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        VMMCall  VMM.Release_Inversion_Pri_ID    ; cd206a010100
        pop    edi
        leave  
        ret    
sub_4_2D09:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Release_Inversion_Pri       ; cd2065010100
        leave  
        ret    
sub_4_2D17:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        VMMCall  VMM.Attach_Thread_To_Group      ; cd206b010100
        pop    edi
        leave  
        ret    
sub_4_2D2A:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 0x18]
        mov    esi, dword ptr [ebx + 0x14]
        push   dword ptr [ebx + 4]
        or     esi, esi
        je     loc_4_2D57
        or     byte ptr [edi + 3], 0x10
        push   dword ptr [ebx]
        cmp    dword ptr [ebx + 0x10], 0
        jne    loc_4_2D5A
        push   dword ptr [ebx + 0xc]
        call   sub_1_0956
        add    esp, 4
        jmp    loc_4_2D66
loc_4_2D57:
        push   dword ptr [ebx + 0x10]
loc_4_2D5A:
        push   0
        push   ebx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_2D66:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        pop    ecx
        call   ecx
        add    esp, 4
        pop    edi
        leave  
        ret    
_VWIN32_CreateRing0Thread:
        push   ebx
        push   edx
        push   ecx
        push   1
        push   0x1c
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        pop    ecx
        pop    edx
        or     eax, eax
        je     loc_4_2E3F
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 0x10], esi
        mov    ebx, eax
        or     esi, esi
        je     loc_4_2DB7
        push   -1
        push   ebx
        push   dword ptr [0x1104]            ; -> obj1:0x1104
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
        jmp    loc_4_2E3F
loc_4_2DB7:
        call   sub_1_08B8
        or     eax, eax
        je     loc_4_2E2B
        push   eax
        mov    dword ptr [ebx + 0xc], eax
        push   -1
        push   ebx
        push   dword ptr [0x1104]            ; -> obj1:0x1104
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
        mov    edx, dword ptr [0x29c]        ; -> obj5:0x029C
        mov    eax, dword ptr [0x10d8]       ; -> obj1:0x10D8
        cmp    dword ptr [edx + 8], eax
        jne    loc_4_2E1A
        mov    ecx, dword ptr [edx + 4]
        push   ecx
loc_4_2DEA:
        push   edx
        push   ecx
        push   eax
        push   eax
        push   edx
        call   sub_4_45C3
        pop    eax
        pop    ecx
        pop    edx
        loop   loc_4_2DEA
        push   edx
        push   eax
        push   dword ptr [ebx + 0xc]
        call   sub_1_0924
        add    esp, 4
        pop    eax
        pop    edx
        pop    ecx
loc_4_2E09:
        push   edx
        push   ecx
        push   eax
        push   eax
        push   edx
        call   sub_4_45A9
        pop    eax
        pop    ecx
        pop    edx
        loop   loc_4_2E09
        jmp    loc_4_2E25
loc_4_2E1A:
        push   dword ptr [ebx + 0xc]
        call   sub_1_0924
        add    esp, 4
loc_4_2E25:
        call   sub_1_08F7
        pop    eax
loc_4_2E2B:
        push   dword ptr [ebx + 0x14]
        push   dword ptr [ebx + 0x18]
        push   0
        push   ebx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        pop    edx
        pop    eax
loc_4_2E3F:
        pop    ebx
        ret    
loc_4_2E41:
        cmp    dword ptr [edi + 0x2c], 0x2a
        jne    loc_4_2E9D
        mov    eax, dword ptr [0x10ec]       ; -> obj1:0x10EC
        lea    esi, [edi + eax]
        mov    edx, edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        cmp    dword ptr [edi + 0x2c], 0x2a
        jne    loc_4_2E9D
        mov    edi, dword ptr [edi + eax]
        xchg   edi, edx
        mov    ebx, dword ptr [edx + 0xc]
        mov    dword ptr [esi], ebx
        mov    eax, dword ptr [ebx + 4]
        inc    word ptr [eax + 0x32]
        mov    ax, word ptr [ebx + 0x76]
        mov    word ptr [edi + 0x2a], ax
        mov    esi, dword ptr [edi + 0x14]
        push   0
        push   eax
        push   esi
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, 0x100000
        jb     loc_4_2E90
        sub    eax, dword ptr [esi + 4]
loc_4_2E90:
        shr    eax, 4
        mov    word ptr [ebx + 0x76], ax
        lea    eax, [edi + 0x2a]
        mov    dword ptr [ebx + 0x40], eax
loc_4_2E9D:
        clc    
        ret    
loc_4_2E9F:
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        cmp    dword ptr [edi + 0x2c], 0x2a
        jne    loc_4_2EDA
        mov    esi, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    esi, dword ptr [edi + esi]
        mov    eax, 2
        bt     dword ptr [esi + 0x2c], 5
        jae    loc_4_2EC5
        mov    eax, 4
loc_4_2EC5:
        VxDCall  VMCPD.VMCPD_Set_CR0_State       ; cd2004001100
        mov    eax, dword ptr [esi + 0x2c]
        shr    eax, 6
        and    eax, 1
        VxDCall  VMCPD.VMCPD_Set_Thread_Precision ; cd2008001100
loc_4_2EDA:
        clc    
        ret    
loc_4_2EDC:
        cmp    dword ptr [edi + 0x2c], 0x2a
        jne    loc_4_2F2A
        cmp    edi, dword ptr [0x1110]       ; -> obj1:0x1110
        jne    loc_4_2EF4
        mov    dword ptr [0x1110], 0         ; -> obj1:0x1110
loc_4_2EF4:
        cmp    edi, dword ptr [0x33c]        ; -> loc_5_033C
        jne    loc_4_2F06
        mov    dword ptr [0x33c], 0          ; -> loc_5_033C
loc_4_2F06:
        mov    ebx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    ebx, dword ptr [edi + ebx]
        mov    eax, dword ptr [ebx + 4]
        dec    word ptr [eax + 0x32]
        jne    loc_4_2F2A
        or     dword ptr [eax + 0x20], 0x800000
        push   0
        VMMCall  VMM._ContextDestroy             ; cd2024010100
        add    esp, 4
loc_4_2F2A:
        clc    
        ret    
loc_4_2F2C:
        cmp    dword ptr [edi + 0x2c], 0x2a
        jne    loc_4_2F64
        mov    edx, dword ptr [0x10ec]       ; -> obj1:0x10EC
        mov    esi, dword ptr [edi + edx]
        test   byte ptr [esi + 0x2e], 2
        jne    loc_4_2F64
        mov    dword ptr [edi + edx], 0
        push   esi
        call   sub_4_25E4
        mov    ebx, dword ptr [esi + 0xc]
        or     ebx, ebx
        je     loc_4_2F64
        push   -1
        push   ebx
        push   dword ptr [ebx + 8]
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
loc_4_2F64:
        clc    
        ret    
sub_4_2F66:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h
sub_4_2F74:
        mov    eax, 5
        mov    ecx, 0x10
        VMMCall  VMM.List_Create                 ; cd209b000100
        mov    dword ptr [0x11c8], esi       ; -> obj1:0x11C8
        ret    
sub_4_2F8B:
        push   ebx
        push   esi
        push   edi
        mov    esi, 0xc89                    ; -> loc_1_0C89
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        mov    dword ptr [0x11c4], eax       ; -> obj1:0x11C4
        call   sub_4_2F74
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h
loc_4_2FA8:
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        call   _VWIN32_Get_Thread_Context
        mov    ecx, dword ptr [esp + 0xc]
        add    esp, 8
        mov    dword ptr [ecx + 0x1c], eax
        ret    0x10
_VWIN32_Get_Thread_Context:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        jne    loc_4_2FD9
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [ebp + 8], edi
        pop    edi
loc_4_2FD9:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x2c], 0x2a
        je     loc_4_2FE9
        xor    eax, eax
        jmp    loc_4_31EE
loc_4_2FE9:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        test   byte ptr [edx], 0x10
        mov    ecx, dword ptr [eax + 0x10]
        je     loc_4_301E
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dr0
        mov    dword ptr [ebx + 4], eax
        mov    eax, dr1
        mov    dword ptr [ebx + 8], eax
        mov    eax, dr2
        mov    dword ptr [ebx + 0xc], eax
        mov    eax, dr3
        mov    dword ptr [ebx + 0x10], eax
        mov    eax, dr6
        mov    dword ptr [ebx + 0x14], eax
        mov    eax, dr7
        mov    dword ptr [ebx + 0x18], eax
loc_4_301E:
        mov    eax, dword ptr [ebp + 0xc]
        test   byte ptr [eax], 8
        je     loc_4_3040
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        lea    esi, [esi + 0x1c]
        VxDCall  VMCPD.VMCPD_Get_Thread_State    ; cd2005001100
        xor    eax, eax
        VxDCall  VMCPD.VMCPD_Get_CR0_State       ; cd2003001100
        mov    dword ptr [esi + 0x6c], eax
loc_4_3040:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        cmp    eax, -1
        je     loc_4_3081
        test   al, 4
        je     loc_4_30B5
        test   byte ptr [ecx + 0x2e], 2
        je     loc_4_3081
        mov    edx, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [ecx + 0x68]
        mov    dword ptr [edx + 0x8c], eax
        mov    edx, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [ecx + 0x64]
        mov    dword ptr [edx + 0x90], eax
        mov    edx, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [ecx + 0x5c]
        mov    dword ptr [edx + 0x94], eax
        movzx  eax, word ptr [ecx + 0x60]
        jmp    loc_4_30AC
loc_4_3081:
        mov    edx, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [ecx + 0x44]
        mov    dword ptr [edx + 0x8c], eax
        mov    eax, dword ptr [ebp + 0xc]
        movzx  edx, word ptr [ecx + 0x40]
        mov    dword ptr [eax + 0x90], edx
        mov    edx, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [ecx + 0x38]
        mov    dword ptr [edx + 0x94], eax
        movzx  eax, word ptr [ecx + 0x3c]
loc_4_30AC:
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x98], eax
loc_4_30B5:
        mov    eax, dword ptr [ebp + 0xc]
        test   byte ptr [eax], 2
        je     loc_4_3104
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x9c], eax
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xa0], eax
        mov    eax, dword ptr [ecx + 0x14]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xa8], eax
        mov    eax, dword ptr [ecx + 0x18]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xac], eax
        mov    eax, dword ptr [ecx + 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xa4], eax
        mov    eax, dword ptr [ecx + 0x1c]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xb0], eax
loc_4_3104:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        cmp    eax, -1
        jne    loc_4_3145
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax], 0x1001f
loc_4_3117:
        mov    eax, dword ptr [ecx + 0x24]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xb8], eax
        mov    edx, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [ecx + 0x28]
        mov    dword ptr [edx + 0xbc], eax
        mov    eax, dword ptr [ecx + 0x30]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xc4], eax
        movzx  eax, word ptr [ecx + 0x34]
        jmp    loc_4_31C8
loc_4_3145:
        test   al, 1
        je     loc_4_31E9
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x24], 0
        jne    loc_4_3193
        test   byte ptr [ecx + 0x2e], 2
        je     loc_4_3117
        mov    eax, dword ptr [ecx + 0x48]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xb8], eax
        mov    edx, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [ecx + 0x4c]
        mov    dword ptr [edx + 0xbc], eax
        mov    eax, dword ptr [ecx + 0x54]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xc4], eax
        mov    edx, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [ecx + 0x58]
        mov    dword ptr [edx + 0xc8], eax
        mov    eax, dword ptr [ecx + 0x50]
        jmp    loc_4_31D4
loc_4_3193:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x20]
        mov    dword ptr [edx + 0xb8], eax
        mov    eax, dword ptr [ebp + 8]
        movzx  edx, word ptr [eax + 0x28]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0xbc], edx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x1c]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xc4], eax
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 0x1a]
loc_4_31C8:
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xc8], eax
        mov    eax, dword ptr [ecx + 0x2c]
loc_4_31D4:
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xc0], eax
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0xb4], eax
loc_4_31E9:
        mov    eax, 1
loc_4_31EE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_4_31F3:
        push   esi
        push   dword ptr [esp + 0x14]
        mov    esi, dword ptr [esp + 0x14]
        push   esi
        call   _VWIN32_Set_Thread_Context
        add    esp, 8
        test   esi, esi
        je     loc_4_3215
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0x10], ecx
        je     loc_4_3215
        mov    dword ptr [ecx + 0x1c], eax
loc_4_3215:
        pop    esi
        ret    0x10
_VWIN32_Set_Thread_Context:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        jne    loc_4_3230
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [ebp + 8], edi
        pop    edi
loc_4_3230:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x2c], 0x2a
        je     loc_4_3240
        xor    eax, eax
        jmp    loc_4_3469
loc_4_3240:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        test   byte ptr [edx], 0x10
        mov    ecx, dword ptr [eax + 0x10]
        je     loc_4_3275
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebx + 4]
        mov    dr0, eax
        mov    eax, dword ptr [ebx + 8]
        mov    dr1, eax
        mov    eax, dword ptr [ebx + 0xc]
        mov    dr2, eax
        mov    eax, dword ptr [ebx + 0x10]
        mov    dr3, eax
        mov    eax, dword ptr [ebx + 0x14]
        mov    dr6, eax
        mov    eax, dword ptr [ebx + 0x18]
        mov    dr7, eax
loc_4_3275:
        mov    eax, dword ptr [ebp + 0xc]
        test   byte ptr [eax], 8
        je     loc_4_328C
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        lea    esi, [esi + 0x1c]
        VxDCall  VMCPD.VMCPD_Set_Thread_State    ; cd2006001100
loc_4_328C:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        cmp    eax, -1
        je     loc_4_32D6
        test   al, 4
        je     loc_4_330A
        test   byte ptr [ecx + 0x2e], 2
        je     loc_4_32D6
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0x8c]
        mov    word ptr [ecx + 0x68], dx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0x90]
        mov    word ptr [ecx + 0x64], dx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0x94]
        mov    word ptr [ecx + 0x5c], dx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0x98]
        mov    word ptr [ecx + 0x60], dx
        jmp    loc_4_330A
loc_4_32D6:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0x8c]
        mov    word ptr [ecx + 0x44], dx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0x90]
        mov    word ptr [ecx + 0x40], dx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0x94]
        mov    word ptr [ecx + 0x38], dx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0x98]
        mov    word ptr [ecx + 0x3c], dx
loc_4_330A:
        mov    eax, dword ptr [ebp + 0xc]
        test   byte ptr [eax], 2
        je     loc_4_3356
        mov    edx, dword ptr [eax + 0x9c]
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xa0]
        mov    dword ptr [ecx + 4], edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xa8]
        mov    dword ptr [ecx + 0x14], edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xac]
        mov    dword ptr [ecx + 0x18], edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xa4]
        mov    dword ptr [ecx + 0x10], edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xb0]
        mov    dword ptr [ecx + 0x1c], edx
loc_4_3356:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        cmp    eax, -1
        je     loc_4_340E
        test   al, 1
        je     loc_4_3464
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x24], 0
        jne    loc_4_33CE
        test   byte ptr [ecx + 0x2e], 2
        je     loc_4_340E
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xb8]
        mov    dword ptr [ecx + 0x48], edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xbc]
        mov    word ptr [ecx + 0x4c], dx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xc4]
        mov    dword ptr [ecx + 0x54], edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xc8]
        mov    word ptr [ecx + 0x58], dx
        mov    edx, dword ptr [ecx + 0x50]
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc0]
        xor    eax, edx
        and    eax, 0xdd5
        xor    eax, edx
        mov    dword ptr [ecx + 0x50], eax
        jmp    loc_4_3458
loc_4_33CE:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xb8]
        mov    dword ptr [edx + 0x20], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xbc]
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x28], dx
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edx + 0xc4]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0x1c], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [edx + 0x1a], ax
        jmp    loc_4_3440
loc_4_340E:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xb8]
        mov    dword ptr [ecx + 0x24], edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xbc]
        mov    word ptr [ecx + 0x28], dx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xc4]
        mov    dword ptr [ecx + 0x30], edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xc8]
        mov    word ptr [ecx + 0x34], dx
loc_4_3440:
        mov    edx, dword ptr [ecx + 0x2c]
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc0]
        xor    eax, edx
        and    eax, 0xdd5
        xor    eax, edx
        mov    dword ptr [ecx + 0x2c], eax
loc_4_3458:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xb4]
        mov    dword ptr [ecx + 8], edx
loc_4_3464:
        mov    eax, 1
loc_4_3469:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_4_346E:
        sub    esp, 4
        mov    dword ptr [esp], 0xffffffff
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0x10]
        push   edi
        push   ebp
        mov    edi, ebx
        and    edi, 0xfff
        mov    ebp, dword ptr [esp + 0x1c]
        add    edi, ebp
        mov    esi, ebx
        shr    esi, 0xc
        add    edi, 0xfff
        shr    edi, 0xc
        push   0x20000000
        push   -1
        push   edi
        push   esi
        call   sub_4_0ED7
        add    esp, 0x10
        cmp    eax, -1
        je     loc_4_34ED
        test   eax, 0x20000
        jne    loc_4_34D7
        test   byte ptr [esp + 0x26], 2
        je     loc_4_34D7
        lea    eax, [ebx + ebp]
        cmp    eax, 0x80000000
        jae    loc_4_34ED
        push   ebp
        push   ebx
        call   sub_5_04A8
        test   eax, eax
        je     loc_4_34ED
loc_4_34D7:
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   edi
        push   esi
        call   sub_4_0ED7
        mov    dword ptr [esp + 0x20], eax
        add    esp, 0x10
loc_4_34ED:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0x10
loc_4_34FB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [ebp - 0x14], edi
        mov    eax, dword ptr [ebp - 0x14]
        cmp    dword ptr [ebp + 0x10], eax
        jne    loc_4_3530
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        call   sub_4_346E
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1c], eax
        jmp    loc_4_35DD
loc_4_3530:
        mov    dword ptr [ebp - 0x10], 0
        mov    dword ptr [ebp - 0xc], 0x14
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_4_356C
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1c], 0xffffffff
        jmp    loc_4_35DD
loc_4_356C:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], 0xffffffff
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx + 8], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ecx + 0xc], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, 0x10
        mov    ebx, dword ptr [ebp + 0x10]
        mov    ecx, 0x102
        mov    edx, dword ptr [ebp - 4]
        mov    esi, 0                        ; -> loc_4_3996
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    esi, dword ptr [ebp - 4]
        lea    esi, [esi]
        call   sub_1_01F4
loc_4_35BE:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], -2
        jne    loc_4_35D2
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1c], 0xffffffff
        jmp    loc_4_35DD
loc_4_35D2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx + 0x1c], edx
loc_4_35DD:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_35E5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [ebp - 0x1c], edi
        mov    eax, dword ptr [ebp - 0x1c]
        cmp    dword ptr [ebp + 0x10], eax
        jne    loc_4_361A
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        call   sub_4_0EEF
        add    esp, 0xc
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1c], eax
        jmp    loc_4_36F8
loc_4_361A:
        mov    esi, dword ptr [ebp + 0x1c]
        cmp    esi, 0x1c
        lea    eax, [esi + 4]
        jae    loc_4_362A
        mov    eax, 4
loc_4_362A:
        mov    dword ptr [ebp - 0x18], 0
        mov    dword ptr [ebp - 0x14], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_4_3665
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1c], 0
        jmp    loc_4_36F8
loc_4_3665:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [eax], 0xffffffff
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x10], esi
        mov    eax, 0x10
        mov    ebx, dword ptr [ebp + 0x10]
        mov    ecx, 0x102
        mov    edx, dword ptr [ebp - 4]
        mov    esi, 0                        ; -> loc_4_39D4
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    esi, dword ptr [ebp - 4]
        lea    esi, [esi]
        call   sub_1_01F4
loc_4_36A2:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax]
        push   dword ptr [ebp + 0x18]
        add    eax, 4
        push   eax
        call   _VWIN32_CopyMem
        add    esp, 0x10
        test   eax, eax
        je     loc_4_36C9
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx + 0x1c], edx
        jmp    loc_4_36D3
loc_4_36C9:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1c], 0
loc_4_36D3:
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    dword ptr [ebp - 0x20], eax
loc_4_36F8:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_4_3700:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        mov    eax, dword ptr [ebp + 0x1c]
        push   esi
        add    eax, 0xc
        push   edi
        mov    dword ptr [ebp - 0x14], eax
        xor    edi, edi
        mov    dword ptr [ebp - 0x18], edi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        cmp    eax, edi
        jne    loc_4_3743
        mov    edi, 8
        jmp    loc_4_37F6
loc_4_3743:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 8], eax
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx], 0xffffffff
        mov    eax, 0x10
        mov    ebx, dword ptr [ebp + 0x10]
        mov    ecx, 0x102
        mov    edx, dword ptr [ebp - 4]
        mov    esi, 0                        ; -> loc_4_3805
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    esi, dword ptr [ebp - 4]
        lea    esi, [esi]
        call   sub_1_01F4
loc_4_3783:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x1c]
        cmp    dword ptr [eax], ecx
        je     loc_4_3792
        mov    edi, 0x3e6
loc_4_3792:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax]
        push   dword ptr [ebp + 0x18]
        add    eax, 0xc
        push   eax
        call   _VWIN32_CopyMem
        add    esp, 0x10
        test   eax, eax
        jne    loc_4_37B1
        mov    edi, 0x57
loc_4_37B1:
        mov    eax, dword ptr [ebp + 0x20]
        test   eax, eax
        je     loc_4_37D1
        push   0
        push   4
        push   eax
        push   dword ptr [ebp - 4]
        call   _VWIN32_CopyMem
        add    esp, 0x10
        test   eax, eax
        jne    loc_4_37D1
        mov    edi, 0x57
loc_4_37D1:
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    dword ptr [ebp - 0x1c], eax
loc_4_37F6:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1c], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_3805:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        push   dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 4]
        add    eax, 0xc
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 8]
        call   _VWIN32_CopyMem
        add    esp, 0x10
        push   dword ptr [ebp - 4]
        call   sub_4_0EE9
        mov    esp, ebp
        pop    ebp
        ret    
loc_4_3835:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        mov    eax, dword ptr [ebp + 0x1c]
        push   esi
        add    eax, 0xc
        push   edi
        mov    dword ptr [ebp - 0x14], eax
        xor    edi, edi
        mov    dword ptr [ebp - 0x18], edi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        cmp    eax, edi
        jne    loc_4_3878
        mov    edi, 8
        jmp    loc_4_3935
loc_4_3878:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp - 4]
        push   0
        push   ecx
        mov    dword ptr [edx + 8], eax
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx], 0xffffffff
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        push   eax
        push   dword ptr [ebp + 0x18]
        call   _VWIN32_CopyMem
        add    esp, 0x10
        test   eax, eax
        jne    loc_4_38BC
        mov    edi, 0x57
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        jmp    loc_4_38F0
loc_4_38BC:
        mov    eax, 0x10
        mov    ebx, dword ptr [ebp + 0x10]
        mov    ecx, 0x102
        mov    edx, dword ptr [ebp - 4]
        mov    esi, 0                        ; -> loc_4_3944
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    esi, dword ptr [ebp - 4]
        lea    esi, [esi]
        call   sub_1_01F4
loc_4_38E1:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x1c]
        cmp    dword ptr [eax], ecx
        je     loc_4_38F0
        mov    edi, 0x3e6
loc_4_38F0:
        mov    eax, dword ptr [ebp + 0x20]
        test   eax, eax
        je     loc_4_3910
        push   0
        push   4
        push   eax
        push   dword ptr [ebp - 4]
        call   _VWIN32_CopyMem
        add    esp, 0x10
        test   eax, eax
        jne    loc_4_3910
        mov    edi, 0x57
loc_4_3910:
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    dword ptr [ebp - 0x1c], eax
loc_4_3935:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1c], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_3944:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        push   0x20020000
        mov    eax, dword ptr [ebp - 4]
        push   -1
        push   dword ptr [eax + 4]
        push   dword ptr [eax + 8]
        call   sub_4_346E
        cmp    eax, -1
        je     loc_4_3981
        push   dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 4]
        push   dword ptr [eax + 8]
        add    eax, 0xc
        push   eax
        call   _VWIN32_CopyMem
        add    esp, 0x10
        jmp    loc_4_398A
loc_4_3981:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
loc_4_398A:
        push   dword ptr [ebp - 4]
        call   sub_4_0EE9
        mov    esp, ebp
        pop    ebp
        ret    
loc_4_3996:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0x10]
        push   dword ptr [eax + 0xc]
        push   dword ptr [eax + 4]
        push   dword ptr [eax + 8]
        call   sub_4_346E
        cmp    eax, -1
        jne    loc_4_39C3
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0xfffffffe
        jmp    loc_4_39C8
loc_4_39C3:
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
loc_4_39C8:
        push   dword ptr [ebp - 4]
        call   sub_4_0EE9
        mov    esp, ebp
        pop    ebp
        ret    
loc_4_39D4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0x10]
        add    eax, 4
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 8]
        call   sub_4_0EEF
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        push   dword ptr [ebp - 4]
        call   sub_4_0EE9
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h, 000h
sub_4_3A08:
        push   edi
        mov    edi, dword ptr [0x10e0]       ; -> obj1:0x10E0
loc_4_3A0F:
        mov    eax, dword ptr [edi + 0x34]
        or     eax, eax
        je     loc_4_3A2B
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edi + 0x34], ecx
        mov    edx, dword ptr [eax + 8]
        push   dword ptr [eax + 4]
        call   sub_1_0AD9
        pop    eax
        call   edx
        jmp    loc_4_3A0F
loc_4_3A2B:
        pop    edi
        ret    
sub_4_3A2D:
        mov    ecx, dword ptr [0x10e0]       ; -> obj1:0x10E0
        jecxz  loc_4_3A38
        inc    dword ptr [ecx + 0x30]
loc_4_3A38:
        push   edx
        mov    edx, dword ptr [0x2ac]        ; -> obj5:0x02AC
        call   sub_4_2BAF
        pop    edx
        ret    
sub_4_3A46:
        push   edx
        mov    edx, dword ptr [0x2ac]        ; -> obj5:0x02AC
        call   sub_4_2C1E
        pop    edx
        mov    ecx, dword ptr [0x10e0]       ; -> obj1:0x10E0
        jecxz  loc_4_3A6B
        dec    dword ptr [ecx + 0x30]
        jne    loc_4_3A6B
        cmp    dword ptr [ecx + 0x34], 0
        je     loc_4_3A6B
        call   sub_4_3A08
loc_4_3A6B:
        ret    
sub_4_3A6C:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        cmp    eax, ebx
        pop    ebx
        jne    loc_4_3A94
        mov    eax, dword ptr [0x10e0]       ; -> obj1:0x10E0
        cmp    dword ptr [eax + 0x30], 0
        jne    loc_4_3A98
        call   sub_4_3A2D
        xor    eax, eax
        inc    eax
        ret    
loc_4_3A94:
        xor    eax, eax
        stc    
        ret    
loc_4_3A98:
        push   edi
        push   edx
        push   ecx
        lea    edi, [eax + 0x34]
        call   sub_1_0ABB
        jae    loc_4_3AAA
        pop    ecx
        pop    edx
        pop    edi
        jmp    loc_4_3A94
loc_4_3AAA:
        pop    dword ptr [eax + 8]
        pop    dword ptr [eax + 4]
        mov    edx, dword ptr [edi]
loc_4_3AB2:
        or     edx, edx
        je     loc_4_3ABC
        lea    edi, [edx]
        mov    edx, dword ptr [edi]
        jmp    loc_4_3AB2
loc_4_3ABC:
        mov    dword ptr [eax], edx
        mov    dword ptr [edi], eax
        pop    edi
        mov    eax, 1
        stc    
        ret    
sub_4_3AC8:
        cmp    dword ptr [ebx], 3
        jne    loc_4_3AD5
        cmp    dword ptr [ebx + 0x18], edi
        jne    loc_4_3AD5
        test   edi, edi
        ret    
loc_4_3AD5:
        cmp    dword ptr [ebx + 0x10], 0
        ret    
sub_4_3ADA:
        cmp    dword ptr [ebx], 3
        je     loc_4_3AEE
        cmp    dword ptr [ebx], 2
        jne    loc_4_3AF1
        test   dword ptr [ebx + 0x18], 1
        je     loc_4_3AF1
        ret    
loc_4_3AEE:
        mov    dword ptr [ebx + 0x18], edi
loc_4_3AF1:
        dec    dword ptr [ebx + 0x10]
        ret    
sub_4_3AF5:
        push   esi
        mov    esi, eax
        cmp    dword ptr [0x4b0], eax        ; -> loc_5_04B0
        jae    loc_4_3B48
        xor    ecx, ecx
loc_4_3B02:
        add    ecx, 0x40
        mov    edx, dword ptr [0x4b0]        ; -> loc_5_04B0
        add    edx, ecx
        cmp    edx, eax
        jb     loc_4_3B02
        mov    eax, 0x10
        mul    ecx
        push   ecx
        push   0x201
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        pop    ecx
        or     eax, eax
        je     loc_4_3B6E
        mov    edx, dword ptr [0x4ac]        ; -> loc_5_04AC
        add    dword ptr [0x4b0], ecx        ; -> loc_5_04B0
loc_4_3B39:
        mov    dword ptr [eax], edx
        mov    edx, eax
        add    eax, 0x10
        loop   loc_4_3B39
        mov    dword ptr [0x4ac], edx        ; -> loc_5_04AC
loc_4_3B48:
        mov    ecx, esi
        mov    eax, dword ptr [0x4ac]        ; -> loc_5_04AC
loc_4_3B4F:
        mov    edx, eax
        or     eax, eax
        je     loc_4_3B6E
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [eax]
        loop   loc_4_3B4F
        xchg   dword ptr [0x4ac], eax        ; -> loc_5_04AC
        mov    dword ptr [edx], eax
        sub    dword ptr [0x4b0], esi        ; -> loc_5_04B0
loc_4_3B6E:
        pop    esi
        ret    
sub_4_3B70:
        mov    edx, dword ptr [eax]
        xchg   dword ptr [0x4ac], edx        ; -> loc_5_04AC
        mov    dword ptr [eax], edx
        add    dword ptr [0x4b0], ecx        ; -> loc_5_04B0
        ret    
sub_4_3B81:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 8]
        mov    dword ptr [edi + 0x54], 0
        mov    edi, dword ptr [edi + 0x5c]
        mov    esi, eax
        mov    ebx, eax
        xor    ecx, ecx
loc_4_3B97:
        inc    ecx
        mov    eax, dword ptr [ebx + 0xc]
        add    eax, 0xc
loc_4_3B9E:
        mov    edx, dword ptr [eax]
        cmp    edx, ebx
        je     loc_4_3BAD
        or     edx, edx
        je     loc_4_3BAD
        lea    eax, [edx + 4]
        jmp    loc_4_3B9E
loc_4_3BAD:
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [eax], edx
        mov    edx, dword ptr [ebx + 0xc]
        cmp    dword ptr [edx], 3
        jne    loc_4_3BEF
        sub    eax, 0xc
        cmp    edx, eax
        jne    loc_4_3BEF
        cmp    dword ptr [edx + 0x18], 0
        je     loc_4_3BEF
        push   edi
        push   ebx
        push   ecx
        VMMCall  VMM.Release_Inversion_Pri_ID    ; cd206a010100
        mov    ecx, dword ptr [ebx + 4]
        jecxz  loc_4_3BEC
        mov    edi, dword ptr [edx + 0x18]
        mov    ebx, dword ptr [ecx + 8]
        mov    eax, dword ptr [ebx + 0x34]
        mov    eax, dword ptr [eax]
        mov    ebx, dword ptr [ebx + 0x5c]
        mov    edi, dword ptr [edi + 0x5c]
        VMMCall  VMM.Set_Inversion_Pri           ; cd2063010100
loc_4_3BEC:
        pop    ecx
        pop    ebx
        pop    edi
loc_4_3BEF:
        mov    ebx, dword ptr [ebx]
        cmp    esi, ebx
        jne    loc_4_3B97
        mov    eax, ebx
        call   sub_4_3B70
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  0E8h, 028h, 0FEh, 0FFh, 0FFh, 08Bh, 040h, 054h, 00Bh, 0C0h, 074h, 005h, 0E8h, 070h, 0FFh, 0FFh
        db  0FFh, 0E8h, 030h, 0FEh, 0FFh, 0FFh, 0C3h
sub_4_3C17:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
loc_4_3C1D:
        xor    eax, eax
        inc    eax
        call   sub_4_3AF5
        or     eax, eax
        jne    loc_4_3C3E
        call   sub_4_3A46
        xor    eax, eax
        inc    eax
        VMMCall  VMM.Time_Slice_Sleep            ; cd2061010100
        call   sub_4_3A2D
        jmp    loc_4_3C1D
loc_4_3C3E:
        mov    esi, eax
        mov    ebx, dword ptr [0x10d8]       ; -> obj1:0x10D8
        mov    dword ptr [esi + 8], ebx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0xc], edx
        mov    dword ptr [ebx + 0x54], esi
        mov    dword ptr [ebx + 0x58], 0
        lea    edi, [edx + 0xc]
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ebx + 0x34]
        mov    eax, dword ptr [eax]
loc_4_3C63:
        jecxz  loc_4_3C78
        mov    ecx, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ecx + 0x34]
        cmp    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        jb     loc_4_3C78
        lea    edi, [ecx + 4]
        mov    ecx, dword ptr [edi]
        jmp    loc_4_3C63
loc_4_3C78:
        mov    dword ptr [edi], esi
        mov    dword ptr [esi + 4], ecx
        cmp    dword ptr [edx], 3
        jne    loc_4_3CAD
        lea    eax, [edx + 0xc]
        cmp    edi, eax
        jne    loc_4_3CAD
        jecxz  loc_4_3C99
        mov    edi, dword ptr [ecx + 8]
        mov    edi, dword ptr [edi + 0x5c]
        mov    eax, edx
        VMMCall  VMM.Release_Inversion_Pri_ID    ; cd206a010100
loc_4_3C99:
        mov    eax, dword ptr [ebx + 0x34]
        mov    eax, dword ptr [eax]
        mov    ebx, dword ptr [ebx + 0x5c]
        mov    edi, dword ptr [edx + 0x18]
        mov    edi, dword ptr [edi + 0x5c]
        VMMCall  VMM.Set_Inversion_Pri           ; cd2063010100
loc_4_3CAD:
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   sub_1_0CF0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_4_3CC0:
        push   ebp
        mov    ebp, esp
        add    esp, -4
        push   ebx
        push   esi
        push   edi
loc_4_3CC9:
        mov    eax, dword ptr [ebp + 8]
        call   sub_4_3AF5
        or     eax, eax
        jne    loc_4_3CEA
        call   sub_4_3A46
        xor    eax, eax
        inc    eax
        VMMCall  VMM.Time_Slice_Sleep            ; cd2061010100
        call   sub_4_3A2D
        jmp    loc_4_3CC9
loc_4_3CEA:
        mov    esi, eax
        mov    dword ptr [ebp - 4], 0
        mov    ebx, dword ptr [0x10d8]       ; -> obj1:0x10D8
        mov    dword ptr [ebx + 0x54], esi
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebx + 0x58], eax
loc_4_3D02:
        mov    dword ptr [esi + 8], ebx
        mov    edx, dword ptr [ebp - 4]
        shl    edx, 2
        add    edx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [edx]
        cmp    dword ptr [edx], 4
        jbe    loc_4_3D18
        mov    edx, dword ptr [edx + 0xc]
loc_4_3D18:
        mov    dword ptr [esi + 0xc], edx
        lea    edi, [edx + 0xc]
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ebx + 0x34]
        mov    eax, dword ptr [eax]
loc_4_3D25:
        jecxz  loc_4_3D3A
        mov    ecx, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ecx + 0x34]
        cmp    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        jb     loc_4_3D3A
        lea    edi, [ecx + 4]
        mov    ecx, dword ptr [edi]
        jmp    loc_4_3D25
loc_4_3D3A:
        mov    dword ptr [edi], esi
        mov    dword ptr [esi + 4], ecx
        cmp    dword ptr [edx], 3
        jne    loc_4_3D75
        lea    eax, [edx + 0xc]
        cmp    edi, eax
        jne    loc_4_3D75
        cmp    dword ptr [edx + 0x18], 0
        je     loc_4_3D75
        jecxz  loc_4_3D61
        mov    edi, dword ptr [ecx + 8]
        mov    edi, dword ptr [edi + 0x5c]
        mov    eax, edx
        VMMCall  VMM.Release_Inversion_Pri_ID    ; cd206a010100
loc_4_3D61:
        mov    eax, dword ptr [ebx + 0x34]
        mov    eax, dword ptr [eax]
        mov    ebx, dword ptr [ebx + 0x5c]
        mov    edi, dword ptr [edx + 0x18]
        mov    edi, dword ptr [edi + 0x5c]
        VMMCall  VMM.Set_Inversion_Pri           ; cd2063010100
loc_4_3D75:
        mov    esi, dword ptr [esi]
        mov    eax, dword ptr [ebp - 4]
        inc    eax
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 8], eax
        mov    ebx, dword ptr [0x10d8]       ; -> obj1:0x10D8
        ja     loc_4_3D02
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x10]
        call   sub_1_0CF0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_4_3DA0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
loc_4_3DA6:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0xc]
loc_4_3DAC:
        or     eax, eax
        je     loc_4_3E19
        mov    edi, dword ptr [eax + 8]
        call   sub_4_3AC8
        je     loc_4_3E19
        mov    ecx, dword ptr [edi + 0x60]
        cmp    dword ptr [ecx + 0x20], 0
        je     loc_4_3DDA
        call   sub_4_3B81
        push   0x40000001
        push   dword ptr [edi + 0x5c]
        call   sub_1_0836
        add    esp, 8
        jmp    loc_4_3DA6
loc_4_3DDA:
        test   byte ptr [edi + 0x58], 1
        jne    loc_4_3E1E
        xor    esi, esi
        dec    esi
        mov    ecx, dword ptr [edi + 0x54]
loc_4_3DE6:
        inc    esi
        cmp    ecx, eax
        mov    ecx, dword ptr [ecx]
        jne    loc_4_3DE6
        call   sub_4_3ADA
        mov    ebx, dword ptr [eax + 4]
        call   sub_4_3B81
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_4_3E06
        add    esi, 0x80
loc_4_3E06:
        push   esi
        push   dword ptr [edi + 0x5c]
        call   sub_1_0836
        add    esp, 8
        mov    eax, ebx
        mov    ebx, dword ptr [ebp + 8]
        jmp    loc_4_3DAC
loc_4_3E19:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_4_3E1E:
        mov    esi, dword ptr [eax]
loc_4_3E20:
        cmp    esi, eax
        je     loc_4_3E3B
        mov    ebx, dword ptr [esi + 0xc]
        call   sub_4_3AC8
        mov    esi, dword ptr [esi]
        jne    loc_4_3E20
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        jmp    loc_4_3DAC
loc_4_3E3B:
        mov    esi, eax
loc_4_3E3D:
        mov    ebx, dword ptr [esi + 0xc]
        call   sub_4_3ADA
        mov    esi, dword ptr [esi]
        cmp    esi, eax
        jne    loc_4_3E3D
        mov    ebx, dword ptr [eax + 4]
        call   sub_4_3B81
        xor    esi, esi
        cmp    dword ptr [ebp + 0xc], esi
        je     loc_4_3E06
        mov    esi, 0x80
        jmp    loc_4_3E06
_VWIN32_SetWin32Event:
        mov    edx, eax
        cmp    dword ptr [edx], 2
        je     loc_4_3E6C
        xor    eax, eax
        stc    
        ret    
loc_4_3E6C:
        mov    ecx, 0x3e61                   ; -> _VWIN32_SetWin32Event
        call   sub_4_3A6C
        jb     loc_4_3E92
        mov    dword ptr [edx + 0x10], 1
        push   0
        push   edx
        call   sub_4_3DA0
        add    esp, 8
        call   sub_4_3A46
        xor    eax, eax
        inc    eax
loc_4_3E92:
        ret    
_VWIN32_PulseWin32Event:
        mov    edx, eax
        cmp    dword ptr [edx], 2
        je     loc_4_3E9E
        xor    eax, eax
        stc    
        ret    
loc_4_3E9E:
        mov    ecx, 0x3e93                   ; -> _VWIN32_PulseWin32Event
        call   sub_4_3A6C
        jb     loc_4_3ECB
        mov    dword ptr [edx + 0x10], 1
        push   0
        push   edx
        call   sub_4_3DA0
        add    esp, 8
        mov    dword ptr [edx + 0x10], 0
        call   sub_4_3A46
        xor    eax, eax
        inc    eax
loc_4_3ECB:
        ret    
_VWIN32_ResetWin32Event:
        mov    edx, eax
        cmp    dword ptr [edx], 2
        je     loc_4_3ED7
        xor    eax, eax
        stc    
        ret    
loc_4_3ED7:
        mov    ecx, 0x3ecc                   ; -> _VWIN32_ResetWin32Event
        call   sub_4_3A6C
        jb     loc_4_3EF2
        mov    dword ptr [edx + 0x10], 0
        call   sub_4_3A46
        xor    eax, eax
        inc    eax
loc_4_3EF2:
        ret    
_VWIN32_WaitMultipleObjects:
        push   esi
        push   edi
        push   ebx
loc_4_3EF6:
        mov    ebx, 0xffffffff
        jecxz  loc_4_3F6F
        test   al, al
        je     loc_4_3F17
        mov    ebx, dword ptr [0x10e0]       ; -> obj1:0x10E0
        cmp    dword ptr [ebx + 0x38], 0
        je     loc_4_3F17
        mov    ebx, 0xc0
        jmp    loc_4_3FE8
loc_4_3F17:
        mov    edi, dword ptr [0x10d8]       ; -> obj1:0x10D8
        push   eax
        mov    bh, ah
        call   sub_4_3A2D
        xor    eax, eax
        test   bh, bh
        jne    loc_4_3FB6
loc_4_3F2F:
        mov    ebx, dword ptr [esi + eax*4]
        call   sub_4_3AC8
        jne    loc_4_3FAD
        inc    eax
        cmp    eax, ecx
        jb     loc_4_3F2F
        xor    ebx, ebx
loc_4_3F40:
        or     edx, edx
        mov    ebx, 0x102
        je     loc_4_3FE2
        mov    eax, dword ptr [esp]
        push   edx
        push   ecx
        movzx  eax, al
        push   eax
        push   ebx
        push   edx
        push   esi
        push   ecx
        call   sub_4_3CC0
        add    esp, 0x14
        mov    ebx, eax
        pop    ecx
        pop    edx
        pop    eax
        cmp    ebx, 0x40000001
        je     loc_4_3F71
loc_4_3F6F:
        jmp    loc_4_3FE8
loc_4_3F71:
        mov    ebx, dword ptr [0x10e0]       ; -> obj1:0x10E0
        test   byte ptr [ebx + 0x2d], 1
        jne    loc_4_3F91
        cmp    dword ptr [ebx + 0x20], 0
        je     loc_4_3EF6
        cmp    dword ptr [ebx + 0x28], 0
        jne    loc_4_3EF6
loc_4_3F91:
        pushal 
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        call   sub_1_0779
        or     byte ptr [ebx + 0x2c], 4
        push   eax
        call   sub_1_06A5
        popal  
        jmp    loc_4_3EF6
loc_4_3FAD:
        call   sub_4_3ADA
        mov    ebx, eax
        jmp    loc_4_3FE2
loc_4_3FB6:
        mov    ebx, dword ptr [esi + eax*4]
        call   sub_4_3AC8
        je     loc_4_3FD8
        inc    eax
        cmp    eax, ecx
        jb     loc_4_3FB6
        xor    eax, eax
loc_4_3FC7:
        mov    ebx, dword ptr [esi + eax*4]
        call   sub_4_3ADA
        inc    eax
        cmp    eax, ecx
        jb     loc_4_3FC7
        xor    ebx, ebx
        jmp    loc_4_3FE2
loc_4_3FD8:
        mov    ebx, 1
        jmp    loc_4_3F40
loc_4_3FE2:
        pop    eax
        call   sub_4_3A46
loc_4_3FE8:
        mov    eax, ebx
        pop    ebx
        pop    edi
        pop    esi
        ret    
_VWIN32_WaitSingleObject:
        push   edi
        push   esi
        push   ebx
loc_4_3FF1:
        test   eax, eax
        je     loc_4_4008
        mov    esi, dword ptr [0x10e0]       ; -> obj1:0x10E0
        cmp    dword ptr [esi + 0x38], 0
        je     loc_4_4008
        mov    esi, 0xc0
        jmp    loc_4_407A
loc_4_4008:
        mov    edi, dword ptr [0x10d8]       ; -> obj1:0x10D8
        mov    esi, eax
        call   sub_4_3A2D
        call   sub_4_3AC8
        jne    loc_4_406E
        or     edx, edx
        jne    loc_4_4027
        mov    esi, 0x102
        jmp    loc_4_4075
loc_4_4027:
        push   edx
        push   esi
        push   edx
        push   ebx
        call   sub_4_3C17
        add    esp, 0xc
        pop    edx
        xchg   esi, eax
        cmp    esi, 0x40000001
        jne    loc_4_407A
        mov    esi, dword ptr [0x10e0]       ; -> obj1:0x10E0
        test   byte ptr [esi + 0x2d], 1
        jne    loc_4_4055
        cmp    dword ptr [esi + 0x20], 0
        je     loc_4_3FF1
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_4_3FF1
loc_4_4055:
        pushal 
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        call   sub_1_0779
        or     byte ptr [esi + 0x2c], 4
        push   eax
        call   sub_1_06A5
        popal  
        jmp    loc_4_3FF1
loc_4_406E:
        call   sub_4_3ADA
        mov    esi, eax
loc_4_4075:
        call   sub_4_3A46
loc_4_407A:
        mov    eax, esi
        pop    ebx
        pop    esi
        pop    edi
        ret    
loc_4_4080:
        sub    esp, 0x18
        mov    dword ptr [esp + 0x10], 0xc0000094
        mov    dword ptr [esp + 0x14], 0     ; -> loc_1_11CC
        mov    dword ptr [esp + 8], 0xffffffff
        mov    ecx, dword ptr [esp + 0x1c]
        push   ebx
        push   esi
        mov    bl, 1
        push   edi
        push   ebp
        mov    edi, 4
        mov    ebp, dword ptr [ecx + 0x24]
loc_4_40AD:
        lea    eax, [esp + 0x13]
        push   0
        push   1
        push   eax
        push   ebp
        call   _VWIN32_CopyMem
        add    esp, 0x10
        test   eax, eax
        je     loc_4_4306
        movzx  eax, byte ptr [esp + 0x13]
        inc    ebp
        cmp    eax, 0x2e
        jg     loc_4_40DB
        je     loc_4_4127
        cmp    eax, 0x26
        je     loc_4_4127
        jmp    loc_4_4116
loc_4_40DB:
        cmp    eax, 0x3e
        jg     loc_4_40E9
        je     loc_4_4127
        cmp    eax, 0x36
        je     loc_4_4127
        jmp    loc_4_4116
loc_4_40E9:
        cmp    eax, 0x64
        jl     loc_4_4116
        cmp    eax, 0x65
        jle    loc_4_4127
        cmp    eax, 0x66
        je     loc_4_411A
        cmp    eax, 0x67
        je     loc_4_4306
        cmp    eax, 0xf0
        je     loc_4_4127
        cmp    eax, 0xf2
        jl     loc_4_4116
        cmp    eax, 0xf3
        jle    loc_4_4127
loc_4_4116:
        xor    bl, bl
        jmp    loc_4_4127
loc_4_411A:
        mov    dword ptr [esp + 0x18], 0xffff
        mov    edi, 2
loc_4_4127:
        test   bl, bl
        jne    loc_4_40AD
        cmp    byte ptr [esp + 0x13], 0xf7
        je     loc_4_413D
        cmp    byte ptr [esp + 0x13], 0xf6
        jne    loc_4_4306
loc_4_413D:
        cmp    byte ptr [esp + 0x13], 0xf6
        jne    loc_4_4151
        mov    dword ptr [esp + 0x18], 0xff
        mov    edi, 1
loc_4_4151:
        lea    eax, [esp + 0x13]
        push   0
        push   1
        push   eax
        push   ebp
        call   _VWIN32_CopyMem
        add    esp, 0x10
        test   eax, eax
        je     loc_4_4306
        inc    ebp
        mov    al, byte ptr [esp + 0x13]
        shr    al, 4
        mov    bl, byte ptr [esp + 0x13]
        movzx  eax, al
        and    al, 0xfc
        and    bl, 7
        cmp    edi, 1
        lea    esi, [eax]                    ; -> loc_1_11DC
        jne    loc_4_419C
        mov    al, byte ptr [esp + 0x13]
        and    al, 0xc0
        cmp    al, 0xc0
        jne    loc_4_419C
        mov    dword ptr [esp + 0x24], 0     ; -> loc_1_11D4
loc_4_419C:
        mov    dword ptr [esp + 0x14], 0
        cmp    byte ptr [esi], bl
        je     loc_4_4233
        cmp    byte ptr [esi + 1], bl
        jne    loc_4_421D
        lea    eax, [esp + 0x13]
        push   0
        push   1
        push   eax
        push   ebp
        call   _VWIN32_CopyMem
        add    esp, 0x10
        test   eax, eax
        je     loc_4_4306
        inc    ebp
        mov    al, byte ptr [esp + 0x13]
        and    al, 0x38
        shr    al, 3
        movzx  ecx, al
        mov    dword ptr [esp + 0x1c], ecx
        cmp    ecx, 4
        je     loc_4_41FE
        movzx  edx, byte ptr [ecx]           ; -> loc_1_11CC
        mov    ecx, dword ptr [esp + 0x2c]
        mov    eax, dword ptr [edx + ecx]
        mov    cl, byte ptr [esp + 0x13]
        shr    cl, 6
        mov    dword ptr [esp + 0x14], eax
        shl    dword ptr [esp + 0x14], cl
loc_4_41FE:
        mov    al, byte ptr [esp + 0x13]
        mov    ecx, dword ptr [esp + 0x2c]
        and    eax, 7
        movzx  edx, byte ptr [eax]           ; -> loc_1_11CC
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [edx + ecx]
        add    dword ptr [esp + 0x14], eax
        jmp    loc_4_4233
loc_4_421D:
        mov    eax, dword ptr [esp + 0x24]
        movzx  ecx, bl
        movzx  edx, byte ptr [ecx + eax]
        mov    ecx, dword ptr [esp + 0x2c]
        mov    eax, dword ptr [edx + ecx]
        mov    dword ptr [esp + 0x14], eax
loc_4_4233:
        mov    edx, 1
        mov    cl, bl
        shl    edx, cl
        movzx  eax, byte ptr [esi + 2]
        test   edx, eax
        je     loc_4_428E
        cmp    byte ptr [esi + 3], 4
        push   0
        jne    loc_4_4265
        lea    eax, [esp + 0x20]
        push   4
        push   eax
        push   ebp
        call   _VWIN32_CopyMem
        add    esp, 0x10
        test   eax, eax
        jne    loc_4_4286
        jmp    loc_4_4306
loc_4_4265:
        lea    eax, [esp + 0x17]
        push   1
        push   eax
        push   ebp
        call   _VWIN32_CopyMem
        add    esp, 0x10
        test   eax, eax
        je     loc_4_4306
        movsx  eax, byte ptr [esp + 0x13]
        mov    dword ptr [esp + 0x1c], eax
loc_4_4286:
        mov    eax, dword ptr [esp + 0x1c]
        add    dword ptr [esp + 0x14], eax
loc_4_428E:
        cmp    byte ptr [esi + 3], 0
        je     loc_4_42F4
        cmp    edi, 1
        je     loc_4_42A5
        cmp    edi, 2
        je     loc_4_42C0
        cmp    edi, 4
        je     loc_4_42DB
        jmp    loc_4_42F4
loc_4_42A5:
        lea    eax, [esp + 0x14]
        push   0
        push   1
        push   eax
        push   dword ptr [esp + 0x20]
        call   _VWIN32_CopyMem
        add    esp, 0x10
        test   eax, eax
        jne    loc_4_42F4
        jmp    loc_4_4306
loc_4_42C0:
        lea    eax, [esp + 0x14]
        push   0
        push   2
        push   eax
        push   dword ptr [esp + 0x20]
        call   _VWIN32_CopyMem
        add    esp, 0x10
        test   eax, eax
        jne    loc_4_42F4
        jmp    loc_4_4306
loc_4_42DB:
        lea    eax, [esp + 0x14]
        push   0
        push   4
        push   eax
        push   dword ptr [esp + 0x20]
        call   _VWIN32_CopyMem
        add    esp, 0x10
        test   eax, eax
        je     loc_4_4306
loc_4_42F4:
        mov    eax, dword ptr [esp + 0x18]
        test   dword ptr [esp + 0x14], eax
        je     loc_4_4306
        mov    dword ptr [esp + 0x20], 0xc0000095
loc_4_4306:
        mov    eax, dword ptr [esp + 0x20]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
sub_4_4312:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 0x28]
        ret    4
sub_4_431D:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ebx, dword ptr [eax]
        push   edi
        mov    ecx, dword ptr [ebx + 0x5c]
        mov    dword ptr [eax + 0x14], 0
        mov    edi, dword ptr [ecx + 0x10]
        call   sub_1_062B
loc_4_433B:
        xor    esi, esi
        mov    eax, dword ptr [ebp + 8]
        xchg   dword ptr [eax + 0x10], esi
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    eax, dword ptr [ebx + 0x60]
        or     byte ptr [eax + 0x2d], 8
        mov    ecx, dword ptr [0]            ; -> loc_1_1120
        mov    esi, dword ptr [ecx]
        cmp    dword ptr [esi], 0
        je     loc_4_4370
loc_4_435D:
        push   ebx
        push   dword ptr [esi + 4]
        call   sub_4_449B
        add    esp, 8
        mov    esi, dword ptr [esi]
        cmp    dword ptr [esi], 0
        jne    loc_4_435D
loc_4_4370:
        mov    eax, dword ptr [ebx + 0x60]
        xor    ecx, ecx
        mov    word ptr [ebx + 0x2e], 0xffff
        mov    dword ptr [eax + 0x30], ecx
        mov    dword ptr [eax + 0x34], ecx
        mov    edx, dword ptr [ebx + 0x40]
        mov    dword ptr [eax + 0x1c], ecx
        and    byte ptr [eax + 0x2c], 0xfc
        mov    dword ptr [ebx + 0x10], 0xffffffff
        test   byte ptr [edx + 0x20], 8
        mov    dword ptr [eax + 0x28], ecx
        je     loc_4_43A1
        push   ebx
        call   sub_1_064C
loc_4_43A1:
        mov    eax, dword ptr [ebx + 0x40]
        mov    esi, dword ptr [eax + 0x20]
        test   esi, 0x80
        je     loc_4_43BA
        call   sub_4_18BF
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x14], eax
loc_4_43BA:
        test   esi, 8
        je     loc_4_4447
        push   ebx
        mov    eax, dword ptr [0]            ; -> obj5:0x029C
        push   eax
        call   sub_4_45A9
        inc    word ptr [ebx + 0x2e]
        mov    eax, dword ptr [0]            ; -> obj5:0x02BC
        movzx  ecx, ax
        shr    eax, 0x10
        mov    dword ptr [edi + 0x24], ecx
        mov    word ptr [edi + 0x28], ax
        mov    ecx, dword ptr [0]            ; -> obj5:0x02B0
        movzx  edx, word ptr [ecx + 8]
        mov    ax, word ptr [ecx + 0xa]
        mov    dword ptr [edi + 0x30], edx
        mov    si, word ptr [0]              ; -> loc_5_0362
        mov    dword ptr [edi + 4], ebx
        mov    word ptr [edi + 0x34], ax
        xor    eax, eax
        mov    word ptr [edi + 0x3c], ax
        sub    edx, 0x16c
        mov    word ptr [edi + 0x38], ax
        mov    dword ptr [edi + 0x30], edx
        mov    word ptr [edi + 0x40], si
        mov    eax, dword ptr [ebx + 0x40]
        mov    esi, dword ptr [ecx]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edi + 8], edx
        mov    dword ptr [ebx + 0x18], esi
        mov    dword ptr [ebx + 0x14], eax
        mov    cx, word ptr [2]              ; -> obj5:0x02BA
        movzx  edx, word ptr [0]             ; -> obj5:0x02B8
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        jmp    loc_4_447A
loc_4_4447:
        mov    cx, word ptr [0]              ; -> loc_5_0360
        mov    eax, dword ptr [0]            ; -> obj5:0x0298
        mov    word ptr [edi + 0x28], cx
        mov    edx, dword ptr [ebx + 0x68]
        mov    cx, word ptr [ebx + 0x4c]
        mov    dword ptr [edi + 0x24], eax
        mov    ax, word ptr [0]              ; -> loc_5_0362
        mov    dword ptr [edi + 0x30], edx
        mov    word ptr [edi + 0x34], ax
        mov    word ptr [edi + 0x3c], ax
        mov    word ptr [edi + 0x38], ax
        mov    word ptr [edi + 0x40], cx
loc_4_447A:
        mov    word ptr [edi + 0x44], 0
        mov    eax, dword ptr [edi + 0x2c]
        and    eax, 0xfffff22a
        or     ah, 2
        mov    dword ptr [edi + 0x2c], eax
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0x10], ecx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_4_449B:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x10F8
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        push   0x35
        push   eax
        call   sub_4_0EF5
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 8], ecx
        jne    loc_4_44D9
        mov    esi, dword ptr [eax + 4]
        test   esi, esi
        lea    edi, [esi - 1]
        jle    loc_4_44D9
loc_4_44C7:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_4_2C24
        mov    eax, edi
        dec    edi
        test   eax, eax
        jg     loc_4_44C7
loc_4_44D9:
        mov    eax, dword ptr [0]            ; -> obj1:0x10F8
        push   eax
        call   sub_4_0EFB
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_4_44ED:
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        test   esi, esi
        je     loc_4_4511
        mov    eax, esi
        dec    esi
        test   eax, eax
        je     loc_4_4511
loc_4_44FD:
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0xc]
        call   sub_4_2BB5
        mov    eax, esi
        dec    esi
        test   eax, eax
        jne    loc_4_44FD
loc_4_4511:
        pop    esi
        ret    
sub_4_4513:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x10F8
        mov    ebp, esp
        push   esi
        push   0x35
        push   eax
        call   sub_4_0EF5
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 0x10]
        cmp    dword ptr [eax + 8], ecx
        jne    loc_4_4538
        sub    esi, dword ptr [eax + 4]
loc_4_4538:
        mov    eax, dword ptr [0]            ; -> obj1:0x10F8
        push   eax
        call   sub_4_0EFB
        add    esp, 4
        test   esi, esi
        je     loc_4_457E
        mov    eax, esi
        jle    loc_4_4567
        dec    esi
        test   eax, eax
        jle    loc_4_457E
loc_4_4553:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_4_2BB5
        mov    eax, esi
        dec    esi
        test   eax, eax
        jg     loc_4_4553
        jmp    loc_4_457E
loc_4_4567:
        inc    esi
        test   eax, eax
        jge    loc_4_457E
loc_4_456C:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_4_2C24
        mov    eax, esi
        inc    esi
        test   eax, eax
        jl     loc_4_456C
loc_4_457E:
        pop    esi
        pop    ebp
        ret    
sub_4_4581:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    ecx, dword ptr [eax + 0x50]
        mov    esi, dword ptr [ecx]
        test   esi, esi
        je     loc_4_45A5
loc_4_458F:
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 0x5c]
        test   eax, eax
        je     loc_4_459F
        push   eax
        call   sub_4_1C1B
loc_4_459F:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_4_458F
loc_4_45A5:
        pop    esi
        ret    4
sub_4_45A9:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 0x60]
        inc    dword ptr [ecx + 0x28]
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        call   sub_4_2BB5
        ret    8
sub_4_45C3:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        call   sub_4_2C24
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 0x60]
        dec    dword ptr [ecx + 0x28]
        ret    8
loc_4_45DD:
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        call   sub_4_0F01
        mov    ecx, dword ptr [esp + 0xc]
        add    esp, 8
        mov    dword ptr [ecx + 0x14], eax
        ret    0x10
sub_4_45F7:
        mov    ax, word ptr [0]              ; -> loc_5_0360
        mov    edx, dword ptr [esp + 8]
        cmp    word ptr [edx + 0x28], ax
        jne    loc_4_4632
        mov    ax, word ptr [0]              ; -> loc_5_0362
        cmp    word ptr [edx + 0x34], ax
        jne    loc_4_4632
        mov    ecx, dword ptr [0]            ; -> sub_5_0328
        test   ecx, ecx
        je     loc_4_4632
        push   dword ptr [esp + 0xc]
        push   edx
        push   dword ptr [esp + 0xc]
        call   ecx
        add    esp, 0xc
        mov    eax, 1
        jmp    loc_4_4634
loc_4_4632:
        xor    eax, eax
loc_4_4634:
        ret    0xc
loc_4_4637:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x64]
        shr    ebx, 0xc
        add    eax, 0x10000
        shr    eax, 0xc
        cmp    eax, ebx
        jae    loc_4_46EF
        push   0x20000000
        lea    eax, [ebx - 3]
        push   3
        push   eax
        call   sub_4_0ECB
        add    esp, 0xc
        test   eax, eax
        je     loc_4_4696
        lea    edi, [ebx - 1]
        push   0x20000000
        push   0
        push   1
        push   1
        push   edi
        call   sub_4_0EC5
        add    esp, 0x14
        test   eax, eax
        je     loc_4_4696
        shl    edi, 0xc
        mov    dword ptr [esi + 0x18], edi
        jmp    loc_4_46EB
loc_4_4696:
        xor    edi, edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [ebp - 4], edi
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    edi, dword ptr [eax + 0x10]
        movzx  ecx, word ptr [edi + 0x34]
        push   ecx
        push   dword ptr [eax + 0x14]
        call   sub_4_0EDD
        add    esp, 0xc
        cmp    eax, -1
        je     loc_4_46EF
        test   eax, eax
        jne    loc_4_46C7
        mov    eax, dword ptr [edi + 0x30]
        jmp    loc_4_46CD
loc_4_46C7:
        movzx  ecx, word ptr [edi + 0x30]
        add    eax, ecx
loc_4_46CD:
        shr    eax, 0xc
        cmp    eax, ebx
        jbe    loc_4_46EF
        push   0x20000000
        push   0
        push   1
        push   ebx
        call   sub_4_0ED7
        add    esp, 0x10
        cmp    eax, -1
        je     loc_4_46EF
loc_4_46EB:
        and    byte ptr [esi + 0x44], 0x7f
loc_4_46EF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_4_46F8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        xor    edi, edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    dword ptr [ebp - 0xc], edi
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [eax + 0x2c], 0x2a
        jne    loc_4_4910
        mov    esi, dword ptr [ebp + 0xc]
        test   byte ptr [esi + 0x10], 0x40
        jne    loc_4_4910
        mov    ebx, dword ptr [0]            ; -> obj1:0x10D8
        mov    eax, dword ptr [esi]
        mov    edi, eax
        mov    dword ptr [ebp - 8], eax
        shr    edi, 0xc
        mov    eax, dword ptr [ebx + 0x64]
        add    eax, 0x10000
        mov    ecx, dword ptr [ebx + 0x14]
        shr    eax, 0xc
        shr    ecx, 0xc
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebx + 0x18]
        shr    eax, 0xc
        test   byte ptr [ebx + 0x44], 8
        je     loc_4_4903
        sub    eax, 8
        cmp    eax, edi
        ja     loc_4_4903
        cmp    ecx, edi
        jbe    loc_4_4903
        cmp    dword ptr [ebp - 4], edi
        ja     loc_4_486E
        mov    eax, dword ptr [esi + 8]
        and    al, 5
        cmp    al, 1
        jne    loc_4_486E
        push   0x20000000
        lea    eax, [edi - 3]
        push   3
        push   eax
        call   sub_4_0ECB
        add    esp, 0xc
        push   0x20060000
        push   0
        push   1
        push   edi
        call   sub_4_0ED7
        add    esp, 0x10
        cmp    eax, -1
        jne    loc_4_47C0
        push   esi
        push   dword ptr [ebp + 8]
        push   0x10e
        call   sub_4_45F7
        jmp    loc_4_4912
loc_4_47C0:
        cmp    dword ptr [ebp - 4], edi
        je     loc_4_480E
        push   0x20000000
        lea    eax, [edi - 1]
        push   0
        push   1
        push   1
        push   eax
        call   sub_4_0EC5
        add    esp, 0x14
        test   eax, eax
        je     loc_4_480E
        lea    eax, [edi - 1]
        shl    eax, 0xc
        mov    dword ptr [ebx + 0x18], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, edi
        cmp    eax, -1
        jne    loc_4_480A
        push   0x20060000
        lea    eax, [edi - 1]
        push   0
        push   1
        push   eax
        call   sub_4_0ED7
        add    esp, 0x10
        jmp    loc_4_480E
loc_4_480A:
        xor    eax, eax
        jmp    loc_4_4813
loc_4_480E:
        mov    eax, 1
loc_4_4813:
        test   eax, eax
        je     loc_4_4864
        mov    eax, dword ptr [8]            ; -> obj1:0x10E0
        cmp    dword ptr [eax + 0x28], 0
        jbe    loc_4_4851
        test   byte ptr [ebx + 0x44], 0x80
        jne    loc_4_4851
        mov    eax, dword ptr [ebx + 0x44]
        push   0
        or     al, 0x80
        mov    dword ptr [ebx + 0x44], eax
        push   dword ptr [ebp - 0xc]
        push   ebx
        push   0                             ; -> loc_4_4637
        call   _VWIN32_QueueKernelAPC
loc_4_4840:
        add    esp, 0x10
        test   eax, eax
        je     loc_4_4851
        mov    eax, 1
        jmp    loc_4_4912
loc_4_4851:
        push   esi
        push   dword ptr [ebp + 8]
        push   0x10e
        call   sub_4_45F7
        jmp    loc_4_4912
loc_4_4864:
        mov    eax, 1
        jmp    loc_4_4912
loc_4_486E:
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 8
        cmp    eax, edi
        ja     loc_4_4903
        push   0
        mov    eax, dword ptr [ebp + 8]
        movzx  ecx, word ptr [eax + 0x34]
        push   ecx
        mov    edx, dword ptr [ebp - 0xc]
        push   dword ptr [edx + 0x14]
        call   sub_4_0EDD
        add    esp, 0xc
        cmp    eax, -1
        jne    loc_4_48A6
        push   esi
        push   dword ptr [ebp + 8]
        push   0xe
        call   sub_4_45F7
        jmp    loc_4_4912
loc_4_48A6:
        test   eax, eax
        je     loc_4_48BB
        mov    ecx, eax
        xor    ecx, dword ptr [ebp - 8]
        test   ecx, 0xfffff000
        je     loc_4_48C8
        test   eax, eax
        jne    loc_4_4903
loc_4_48BB:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xfff
        cmp    eax, 0xa
        jne    loc_4_4903
loc_4_48C8:
        push   1
        mov    eax, dword ptr [0x10]         ; -> obj1:0x10E8
        push   edi
        push   eax
        push   dword ptr [0]                 ; -> obj5:0x02C4
        call   sub_4_0ED1
        add    esp, 0x10
        test   eax, eax
        jne    loc_4_48F3
        push   esi
        push   dword ptr [ebp + 8]
        push   0x10e
        call   sub_4_45F7
        jmp    loc_4_4912
loc_4_48F3:
        shl    edi, 0xc
        mov    eax, 1
        mov    word ptr [edi + 0xa], 0x10
        jmp    loc_4_4912
loc_4_4903:
        push   esi
        push   dword ptr [ebp + 8]
        push   0xe
        call   sub_4_45F7
        jmp    loc_4_4912
loc_4_4910:
        xor    eax, eax
loc_4_4912:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
_VWIN32_Emulate_Npx:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0
        mov    ecx, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [esi + 0x34]
        push   eax
        push   dword ptr [ecx + 0x14]
        call   sub_4_0EDD
        add    esp, 0xc
        cmp    eax, -1
        je     loc_4_49A8
        lea    ebx, [esi + 0x30]
        test   eax, eax
        jne    loc_4_494C
        mov    ecx, dword ptr [ebx]
        jmp    loc_4_4951
loc_4_494C:
        movzx  ecx, word ptr [ebx]
        add    ecx, eax
loc_4_4951:
        mov    edi, dword ptr [esi + 0x2c]
        mov    edx, dword ptr [esi + 0x24]
        push   0
        sub    ecx, 0xc
        push   0xc
        mov    dword ptr [ebp - 4], edi
        push   ecx
        mov    dword ptr [ebp - 0xc], edx
        movzx  eax, word ptr [esi + 0x28]
        lea    edx, [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        push   edx
        call   _VWIN32_CopyMem
        add    esp, 0x10
        test   eax, eax
        jne    loc_4_4989
        push   7
        push   esi
        call   _VWIN32_FaultPopup
        add    esp, 8
        jmp    loc_4_49A8
loc_4_4989:
        mov    ax, word ptr [0]              ; -> loc_5_0360
        and    edi, 0xfffffeff
        mov    word ptr [esi + 0x28], ax
        mov    ecx, dword ptr [0]            ; -> loc_5_0338
        sub    dword ptr [ebx], 0xc
        mov    dword ptr [esi + 0x2c], edi
        mov    dword ptr [esi + 0x24], ecx
loc_4_49A8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x4b4  RW-
        db  04Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_4_0000               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_4_0014               ; fixup
        db  00Ah, 000h, 000h, 000h
        dd  loc_4_0028               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_4_1268               ; fixup
        db  008h, 000h, 000h, 000h
        dd  loc_4_003C               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_0050               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_0064               ; fixup
        db  003h, 000h, 000h, 000h
        dd  loc_4_0078               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_008C               ; fixup
        db  009h, 000h, 000h, 000h
        dd  loc_4_00A0               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_00B4               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_00C8               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_00DC               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_00F0               ; fixup
        db  003h, 000h, 000h, 000h
        dd  loc_4_0104               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_4_0118               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_4_0B6C               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_012C               ; fixup
        db  006h, 000h, 000h, 000h
        dd  loc_4_0140               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_0154               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_2FA8               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_31F3               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_3700               ; fixup
        db  005h, 000h, 000h, 000h
        dd  loc_4_3835               ; fixup
        db  005h, 000h, 000h, 000h
        dd  loc_4_0CDF               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_0CF5               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_0168               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_017C               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_0190               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_4_2B82               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_2B97               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_07FA               ; fixup
        db  00Bh, 000h, 000h, 000h
        dd  loc_4_0D08               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_01A4               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_01B8               ; fixup
        db  003h, 000h, 000h, 000h
        dd  loc_4_01CC               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_4_01E0               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_01F4               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_0208               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_021C               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_0230               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_0C3F               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_0C5F               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_0244               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_4_0258               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_026C               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_0280               ; fixup
        db  003h, 000h, 000h, 000h
        dd  loc_4_0294               ; fixup
        db  005h, 000h, 000h, 000h
        dd  loc_4_02A8               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_02BC               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_4_02D0               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_02E4               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_02F8               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_030C               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_0320               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_159F               ; fixup
        db  003h, 000h, 000h, 000h
        dd  loc_4_0334               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_4_0348               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_1B07               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_4_1B0F               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_4_035C               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_0370               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_0384               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_4_34FB               ; fixup
        db  005h, 000h, 000h, 000h
        dd  loc_4_35E5               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_4_0398               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_03AC               ; fixup
        db  003h, 000h, 000h, 000h
        dd  loc_4_45DD               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_0D1E               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_1AE3               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_4_1AFB               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_0C78               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_2532               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_4_03C0               ; fixup
        db  003h, 000h, 000h, 000h
        dd  loc_4_1C75               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_4_11D3               ; fixup
        db  003h, 000h, 000h, 000h
        dd  loc_4_120F               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_125C               ; fixup
        db  001h, 000h, 000h, 000h
        dd  loc_4_0C86               ; fixup
        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_4_0EB3               ; fixup
        dd  loc_4_0EB5               ; fixup
        dd  loc_4_0EB6               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        ; ---- dispatch table (22 entries) ----
        dd  loc_4_059D
        dd  loc_4_05AF
        dd  loc_4_136B
        dd  loc_4_0636
        dd  loc_4_06B9
        dd  loc_4_06C6
        dd  loc_4_06A6
        dd  loc_4_0693
        dd  loc_4_0685
        dd  loc_4_06D9
        dd  loc_4_06EE
        dd  loc_4_0704
        dd  loc_4_076A
        dd  loc_4_12C1
        dd  loc_4_071A
        dd  loc_4_072D
        dd  loc_4_075F
        dd  loc_4_0754
        dd  sub_1_0CB9
        dd  loc_4_1482
        dd  loc_4_07E4
        dd  loc_4_0740
sub_5_0328:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_032C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0330:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0334:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0338:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_033C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0340:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0344:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0348:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_034C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0350:
        add    byte ptr [eax], al
loc_5_0352:
        add    byte ptr [eax], al
loc_5_0354:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0358:
        add    byte ptr [eax], al
loc_5_035A:
        add    byte ptr [eax], al
loc_5_035C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0360:
        add    byte ptr [eax], al
loc_5_0362:
        add    byte ptr [eax], al
loc_5_0364:
        add    byte ptr [eax], al
loc_5_0366:
        inc    dword ptr [eax]
sub_5_0368:
        adc    byte ptr [edi], cl
        add    byte ptr [eax], al
        adc    byte ptr [edi], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [edi], cl
        add    byte ptr [eax], al
        adc    byte ptr [edi], cl
        add    byte ptr [eax], al
        adc    byte ptr [edi], cl
        add    byte ptr [eax], al
        adc    byte ptr [edi], cl
        add    byte ptr [eax], al
loc_5_0384:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [edi], cl
        add    byte ptr [eax], al
        adc    byte ptr [edi], cl
        add    byte ptr [eax], al
        adc    byte ptr [edi], cl
        add    byte ptr [eax], al
        adc    byte ptr [edi], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_03AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_046C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0470:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0474:
        add    bh, bh
        add    byte ptr [eax], al
loc_5_0478:
        dec    ebx
        inc    ebp
        push   edx
        dec    esi
        inc    ebp
        dec    esp
        xor    esi, dword ptr [edx]
        inc    esp
        dec    esp
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_5_04A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_04AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_04B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0xa  R-X
        db  033h, 0C0h, 033h, 0DBh, 033h, 0F6h, 066h, 033h, 0D2h, 0C3h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 7  vsize=0x4e  RW-
        db  02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h, 06Ch, 069h, 062h, 03Ah, 04Ch, 049h, 042h, 043h
        db  020h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h, 06Ch, 069h, 062h, 03Ah, 04Fh, 04Ch, 044h
        db  04Eh, 041h, 04Dh, 045h, 053h, 020h, 000h, 000h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch, 074h
        db  06Ch, 069h, 062h, 03Ah, 04Ch, 049h, 042h, 043h, 020h, 02Dh, 064h, 065h, 066h, 061h, 075h, 06Ch
        db  074h, 06Ch, 069h, 062h, 03Ah, 04Fh, 04Ch, 044h, 04Eh, 041h, 04Dh, 045h, 053h, 020h
VxD__LDATA ENDS

VxD__LDATA SEGMENT   ; object 8  vsize=0xc0  RW-
        dd  loc_4_2FA8               ; fixup
        db  01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        dd  loc_4_31F3               ; fixup
        db  026h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 001h, 001h
        dd  sub_4_346E               ; fixup
        db  08Dh, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 004h, 000h, 015h, 014h
        dd  loc_4_4080               ; fixup
        db  092h, 002h, 000h, 000h, 006h, 000h, 000h, 000h, 001h, 000h, 025h, 014h
        dd  loc_1_0FB6               ; fixup
        db  057h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        dd  sub_4_4312               ; fixup
        db  00Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  sub_4_44ED               ; fixup
        db  026h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 000h, 001h, 001h
        dd  sub_4_4581               ; fixup
        db  028h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 005h, 001h
        dd  sub_4_45A9               ; fixup
        db  01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  sub_4_45C3               ; fixup
        db  01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  loc_4_45DD               ; fixup
        db  01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        dd  sub_4_45F7               ; fixup
        db  040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 023h, 056h, 057h
VxD__LDATA ENDS
