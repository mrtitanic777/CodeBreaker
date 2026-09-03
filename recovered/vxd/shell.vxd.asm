; SHELL -- reconstructed 32-bit disassembly (VxD / LE format)
; 17 objects, 6752 instructions recovered by recursive descent

Declare_Virtual_Device SHELL, 4, 0, SHELL_Control, 00017h, Undefined_Init_Order
;   DDK version 0400h   init_order 0B0000000h   flags 00000h
;   service table: obj1:0x940, 28 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0xc24  R-X
sub_1_0000:
        VxDCall  VSD.VSD_Get_Version             ; cd2000000b00
        jb     loc_1_000E
        VxDCall  VSD.VSD_Bell                    ; cd2001000b00
loc_1_000E:
        ret    
sub_1_000F:
        enter  0x20, 0
        push   esi
        push   edi
        push   ebx
        mov    dword ptr [ebp - 4], 1
        and    byte ptr [ebp - 6], 0
        mov    ebx, dword ptr [ebp + 8]
loc_1_0024:
        nop    
        call   sub_5_0000
        jae    loc_1_0035
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    dword ptr [ebp + 8], ebx
loc_1_0035:
        mov    edi, dword ptr [ebp + 0xc]
        and    edi, 0xf
        cmp    edi, 5
        jbe    loc_1_0042
        xor    edi, edi
loc_1_0042:
        mov    dword ptr [ebp - 0xc], edi
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_1_0068
        mov    edx, dword ptr [0xa00]        ; -> obj1:0x0A00
        cmp    word ptr [edx + ebx + 0x36], 0
        je     loc_1_0068
        xor    edx, edx
        xor    esi, esi
        lea    eax, [edx + 0xf]
        VMMCall  VMM.System_Control              ; cd2093000100
loc_1_0068:
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        mov    dword ptr [ebp - 0x20], ebx
        mov    ebx, dword ptr [0xa08]        ; -> obj1:0x0A08
        test   ebx, ebx
        jne    loc_1_0081
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
loc_1_0081:
        inc    byte ptr [0x8e4]              ; -> loc_1_08E4
        jne    loc_1_00A7
        mov    edx, 0xd
        lea    eax, [edx + 2]
        VMMCall  VMM.System_Control              ; cd2093000100
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        mov    eax, 0x10
        VMMCall  VMM.System_Control              ; cd2093000100
        pop    ebp
loc_1_00A7:
        call   sub_1_01B8
        test   byte ptr [ebp + 0xf], 0x20
        je     loc_1_00B7
        int    1
        cli    
loc_1_00B5:
        jmp    loc_1_00B5
loc_1_00B7:
        sub    esp, 0x6c
        mov    edi, esp
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VxDCall  VKD.VKD_Flush_Msg_Key_Queue     ; cd200e000d00
loc_1_00CE:
        call   sub_1_016C
        test   byte ptr [ebp - 6], 1
        je     loc_1_0107
        mov    ah, al
        cmp    ah, byte ptr [0x8ef]          ; -> obj1:0x08EF
        jne    loc_1_00E6
        mov    ah, byte ptr [ebp - 8]
loc_1_00E6:
        mov    edi, dword ptr [ebp - 0xc]
        mov    esi, dword ptr [edi*4 + 0x91c] ; -> loc_1_091C
        xor    ecx, ecx
loc_1_00F2:
        cmp    ah, byte ptr [esi + ecx*2 + 2]
        je     loc_1_010C
        inc    ecx
        cmp    cl, byte ptr [esi + 1]
        je     loc_1_0100
        jmp    loc_1_00F2
loc_1_0100:
        call   0
        jmp    loc_1_00CE
loc_1_0107:
        xor    eax, eax
        inc    eax
        jmp    loc_1_0111
loc_1_010C:
        movzx  eax, byte ptr [esi + ecx*2 + 3]
loc_1_0111:
        mov    dword ptr [ebp - 4], eax
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    esi, esp
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        add    esp, 0x6c
        dec    byte ptr [0x8e4]              ; -> loc_1_08E4
        jns    loc_1_0149
        push   ebx
        mov    ebx, dword ptr [ebp - 0x20]
        mov    eax, 0xf
        lea    edx, [eax - 2]
        VMMCall  VMM.System_Control              ; cd2093000100
        pop    ebx
        lea    eax, [eax + 2]
        VMMCall  VMM.System_Control              ; cd2093000100
loc_1_0149:
        mov    byte ptr [0x8ee], 1           ; -> loc_1_08EE
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_1_0158:
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cmp    byte ptr [0x8ee], 0           ; -> loc_1_08EE
        je     sub_1_016C
        call   sub_1_01B8
sub_1_016C:
        VxDCall  VKD.VKD_Get_Msg_Key             ; cd200c000d00
        je     loc_1_0158
        test   byte ptr [ebp - 6], 1
        je     loc_1_01AA
        test   ah, 0x80
        je     loc_1_019E
        test   ah, 0xc
        jne    loc_1_01A3
        mov    esi, 0x938                    ; -> obj1:0x0938
        mov    ecx, dword ptr [esi]
        lea    esi, [esi + 4]
loc_1_018E:
        cmp    al, byte ptr [esi]
        je     loc_1_0199
        lea    esi, [esi + 2]
        loop   loc_1_018E
        jmp    loc_1_01A3
loc_1_0199:
        mov    al, byte ptr [esi + 1]
        jmp    loc_1_01AA
loc_1_019E:
        test   ah, 0xc
        je     loc_1_01AA
loc_1_01A3:
        call   0
        jmp    sub_1_016C
loc_1_01AA:
        ret    
sub_1_01AB:
        push   ecx
        xor    al, al
        repne scasb al, byte ptr es:[edi]
        pop    eax
        pushfd 
        sub    ecx, eax
        not    ecx
        popfd  
        ret    
sub_1_01B8:
        mov    eax, dword ptr [0x8dc]        ; -> loc_1_08DC
        VxDCall  VDD.VDD_Msg_ClrScrn             ; cd2007000a00
        mov    dword ptr [ebp - 0x10], edx
        mov    dword ptr [ebp - 0x14], eax
        and    dword ptr [ebp - 0x18], 0
        and    byte ptr [0x8ee], 0           ; -> loc_1_08EE
        or     byte ptr [ebp - 6], 2
        call   sub_1_0219
        mov    edx, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x18]
        sub    edx, eax
        jae    loc_1_01E9
        xor    edx, edx
loc_1_01E9:
        shr    edx, 1
        mov    dword ptr [ebp - 0x18], edx
        mov    eax, dword ptr [0x8dc]        ; -> loc_1_08DC
        VxDCall  VDD.VDD_Msg_BakColor            ; cd2009000a00
        mov    eax, dword ptr [0x8e0]        ; -> obj1:0x08E0
        VxDCall  VDD.VDD_Msg_ForColor            ; cd2008000a00
        and    byte ptr [ebp - 6], 0xfd
        call   sub_1_0219
        mov    eax, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [ebp - 0x1c]
        VxDCall  VDD.VDD_Msg_SetCursPos          ; cd200b800a00
sub_1_0219:
        sub    esp, 0x5c
        mov    dword ptr [ebp - 0x1c], 5
        mov    esi, 0x8f0                    ; -> obj1:0x08F0
        call   sub_1_039C
        mov    edi, dword ptr [ebp + 0x14]
        or     edi, edi
        je     loc_1_0295
        cmp    byte ptr [edi], 0
        je     loc_1_0295
        mov    ecx, 0x50
        call   sub_1_01AB
        shr    ecx, 1
        sub    ecx, 0x22
        sub    dword ptr [ebp - 0x1c], ecx
        mov    eax, dword ptr [0x8e0]        ; -> obj1:0x08E0
        VxDCall  VDD.VDD_Msg_BakColor            ; cd2009000a00
        mov    eax, dword ptr [0x8dc]        ; -> loc_1_08DC
        VxDCall  VDD.VDD_Msg_ForColor            ; cd2008000a00
        mov    al, 0x20
        call   sub_1_03B0
        mov    esi, dword ptr [ebp + 0x14]
        call   sub_1_039C
        call   sub_1_03B0
        mov    eax, dword ptr [0x8dc]        ; -> loc_1_08DC
        VxDCall  VDD.VDD_Msg_BakColor            ; cd2009000a00
        mov    eax, dword ptr [0x8e0]        ; -> obj1:0x08E0
        VxDCall  VDD.VDD_Msg_ForColor            ; cd2008000a00
        mov    esi, 0x8f0                    ; -> obj1:0x08F0
        call   sub_1_039C
loc_1_0295:
        mov    esi, dword ptr [ebp + 0x10]
loc_1_0298:
        xor    edx, edx
        lea    edi, [esp + 4]
        lea    ecx, [edi + 0x46]
        mov    dword ptr [esp], esi
        add    dword ptr [esp], 0x46
        cld    
loc_1_02A9:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_1_02FE
        cmp    al, 0xd
        je     loc_1_02D7
        cmp    al, 0xa
        je     loc_1_02D7
        cmp    al, 9
        je     loc_1_02CC
        cmp    al, 0x20
        je     loc_1_02CC
        stosb  byte ptr es:[edi], al
loc_1_02BF:
        inc    edx
        cmp    edx, 0x46
        jb     loc_1_02A9
        mov    edi, ecx
        mov    esi, dword ptr [esp]
        jmp    loc_1_02DC
loc_1_02CC:
        mov    ecx, edi
        stosb  byte ptr es:[edi], al
        mov    dword ptr [esp], esi
        dec    dword ptr [esp]
        jmp    loc_1_02BF
loc_1_02D7:
        movzx  eax, al
        jmp    loc_1_02E1
loc_1_02DC:
        mov    eax, 0xa0d
loc_1_02E1:
        stosd  dword ptr es:[edi], eax
        xchg   esi, eax
        lea    esi, [esp + 4]
        call   sub_1_039C
        xchg   esi, eax
        cld    
loc_1_02EE:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_1_02FE
        cmp    al, 9
        je     loc_1_02EE
        cmp    al, 0x20
        je     loc_1_02EE
        dec    esi
        jmp    loc_1_0298
loc_1_02FE:
        stosb  byte ptr es:[edi], al
        mov    esi, 0x8f2                    ; -> loc_1_08F2
        or     edx, edx
        je     loc_1_0316
        lea    esi, [esp + 4]
        call   sub_1_039C
        mov    esi, 0x8f0                    ; -> obj1:0x08F0
loc_1_0316:
        call   sub_1_039C
        test   byte ptr [ebp + 0xf], 0x20
        je     loc_1_0335
        movzx  esi, word ptr [0xe]           ; -> loc_2_000E
        add    esi, dword ptr [4]            ; -> loc_2_0004
        call   sub_1_039C
        jmp    loc_1_0398
loc_1_0335:
        mov    edi, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [edi*4 + 0x91c] ; -> loc_1_091C
        movzx  esi, byte ptr [edx]
        movzx  esi, word ptr [esi*2 + 0x10]  ; -> loc_2_0010
        add    esi, dword ptr [4]            ; -> loc_2_0004
        call   sub_1_039C
        mov    al, byte ptr [edx + 1]
        or     al, al
        je     loc_1_0398
        or     byte ptr [ebp - 6], 1
        mov    ecx, dword ptr [ebp + 0xd]
        and    ecx, 0xf
        cmp    cl, al
        jae    loc_1_036F
        cmp    edi, 1
        jne    loc_1_0371
loc_1_036F:
        xor    ecx, ecx
loc_1_0371:
        mov    al, byte ptr [edx + ecx*2 + 2]
        mov    byte ptr [ebp - 8], al
        movzx  esi, byte ptr [edx]
        lea    esi, [esi + ecx + 1]
        movzx  esi, word ptr [esi*2 + 0x10]  ; -> loc_2_0010
        add    esi, dword ptr [4]            ; -> loc_2_0004
        push   dword ptr [ebp - 0x1c]
        call   sub_1_039C
        pop    dword ptr [ebp - 0x1c]
loc_1_0398:
        add    esp, 0x5c
        ret    
sub_1_039C:
        pushfd 
        push   esi
        push   eax
loc_1_039F:
        cld    
        lodsb  al, byte ptr [esi]
        test   al, al
        je     loc_1_03AC
        call   sub_1_03B0
        jmp    loc_1_039F
loc_1_03AC:
        pop    eax
        pop    esi
        popfd  
        ret    
sub_1_03B0:
        cmp    al, 0xa
        jne    loc_1_03B9
        inc    dword ptr [ebp - 0x18]
        jmp    loc_1_03BD
loc_1_03B9:
        cmp    al, 0xd
        jne    loc_1_03C6
loc_1_03BD:
        mov    dword ptr [ebp - 0x1c], 5
        jmp    loc_1_03F9
loc_1_03C6:
        cmp    al, 8
        jne    loc_1_03D5
        cmp    dword ptr [ebp - 0x1c], 5
        je     loc_1_03F9
        dec    dword ptr [ebp - 0x1c]
        jmp    loc_1_03F9
loc_1_03D5:
        push   esi
        push   ecx
        push   edx
        push   eax
        mov    esi, esp
        mov    ecx, 1
        mov    eax, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [ebp - 0x1c]
        test   byte ptr [ebp - 6], 2
        jne    loc_1_03F2
        VxDCall  VDD.VDD_Msg_TextOut             ; cd200a000a00
loc_1_03F2:
        inc    dword ptr [ebp - 0x1c]
        pop    eax
        pop    edx
        pop    ecx
        pop    esi
loc_1_03F9:
        ret    
sub_1_03FA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, 0x8e5                    ; -> obj1:0x08E5
        cld    
        lodsd  eax, dword ptr [esi]
        stosd  dword ptr es:[edi], eax
        lodsd  eax, dword ptr [esi]
        stosd  dword ptr es:[edi], eax
        mov    ecx, 8
loc_1_0423:
        cmp    byte ptr [edi - 1], 0x20
        jne    loc_1_042C
        dec    edi
        loop   loc_1_0423
loc_1_042C:
        mov    byte ptr [edi], 0
        push   dword ptr [ebp + 0x18]
        push   0x8e5                         ; -> obj1:0x08E5
        push   dword ptr [ebp + 0x14]
        push   0x140003
        push   0                             ; -> loc_4_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        movzx  edi, word ptr [0xc]           ; -> loc_2_000C
        add    edi, dword ptr [4]            ; -> loc_2_0004
        push   edi
        push   eax
        push   0x34
        push   ebx
        call   sub_1_000F
        add    esp, 0x10
        add    esp, dword ptr [esp]
        cmp    eax, 6
        mov    eax, ebx
        je     loc_1_0471
        mov    eax, dword ptr [ebp + 0xc]
loc_1_0471:
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_1_047C:
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ah, 0x17
        je     loc_11_05A8
        cmp    ax, 0x168e
        je     loc_17_067C
        cmp    ax, 0x168f
        je     loc_17_0791
        stc    
        ret    
SHELL_Get_Version:
        mov    eax, 0x400
        clc    
        ret    
SHELL_SYSMODAL_Message:
        or     ah, 0x10
        pushal 
        mov    ebp, esp
        test   byte ptr [ebp + 0x1f], 0x60
        jne    loc_1_04C2
        push   edi
        push   ecx
        push   eax
        push   ebx
        call   sub_11_02E0
        add    esp, 0x10
        cmp    eax, -1
        jne    loc_1_04C7
loc_1_04C2:
        call   sub_1_04F5
loc_1_04C7:
        mov    dword ptr [ebp + 0x1c], eax
        popal  
        ret    
SHELL_Message:
        pushal 
        mov    ebp, esp
        call   sub_11_0332
        cmp    eax, -1
        jne    loc_1_04EF
        call   sub_1_04F5
        cmp    dword ptr [ebp + 4], 0
        je     loc_1_04EF
        mov    edx, dword ptr [ebp + 0x14]
        call   dword ptr [ebp + 4]
        mov    ebp, esp
        or     eax, 0xffffffff
loc_1_04EF:
        mov    dword ptr [ebp + 0x1c], eax
        clc    
        popal  
        ret    
sub_1_04F5:
        mov    edx, ebp
        mov    esi, 0x549                    ; -> sub_1_0549
        test   byte ptr [ebp + 0x1f], 0x20
        je     loc_1_050A
        mov    word ptr [esi + 2], 0x3d90
        jmp    loc_1_0518
loc_1_050A:
        VMMCall  VMM.Get_Crit_Status_Thread      ; cd2045010100
        VMMCall  VMM.Test_Cur_Thread_Handle      ; cd2009010100
        jne    loc_1_051F
loc_1_0518:
        call   sub_1_0549
        jmp    loc_1_0545
loc_1_051F:
        or     dword ptr [ebp + 0xc], 0xffffffff
        xor    ebx, ebx
        xor    eax, eax
        mov    ecx, 0x800
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        jmp    loc_1_053F
loc_1_0534:
        push   5
        push   ebp
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        pop    ecx
        pop    ecx
loc_1_053F:
        cmp    dword ptr [ebp + 0xc], -1
        je     loc_1_0534
loc_1_0545:
        mov    eax, dword ptr [ebp + 0xc]
        ret    
sub_1_0549:
        xor    ecx, ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        mov    ebp, edx
        mov    ebx, dword ptr [ebp + 0x10]
        xor    esi, esi
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_1_0571
        mov    esi, dword ptr [ebp]
        or     esi, esi
        jne    loc_1_0571
        mov    esi, dword ptr [0xa00]        ; -> obj1:0x0A00
        lea    esi, [esi + ebx + 0x3a]
loc_1_0571:
        push   esi
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x1c]
        push   ebx
        call   sub_1_000F
        add    esp, 0x10
        mov    dword ptr [ebp + 0xc], eax
        push   ebp
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
        VMMCall  VMM.End_Critical_Section        ; cd2020800100
SHELL_Resolve_Contention:
        pushal 
        mov    ebp, esp
        push   esi
        push   ebx
        push   eax
        call   sub_11_0528
        add    esp, 0xc
        cmp    eax, -1
        jne    loc_1_0605
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    dword ptr [ebp + 0xc], eax
        mov    esi, dword ptr [0xa00]        ; -> obj1:0x0A00
        mov    ebx, dword ptr [ebp + 0x1c]
        lea    edi, [esi + ebx + 0x3a]
        mov    ebx, dword ptr [ebp + 0x10]
        lea    esi, [esi + ebx + 0x3a]
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        push   edi
        push   esi
        push   dword ptr [ebp + 4]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x10]
        call   sub_1_03FA
        add    esp, 0x14
        push   eax
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, dword ptr [ebp + 0xc]
        VMMCall  VMM.Adjust_Execution_Time       ; cd2037000100
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        pop    eax
        mov    dword ptr [ebp + 0x10], eax
        push   eax
        call   sub_11_0583
        pop    ecx
        jmp    loc_1_0608
loc_1_0605:
        mov    dword ptr [ebp + 0x10], eax
loc_1_0608:
        clc    
        popal  
        ret    
        db  090h
SHELL_Update_User_Activity:
        mov    byte ptr [0xa19], 1           ; -> obj1:0x0A19
        ret    
sub_1_0614:
        cmp    eax, 0x20
        jae    loc_1_0620
        jmp    dword ptr [eax*4 + 0x622]     ; -> obj1:0x0622
loc_1_0620:
        clc    
        ret    
        ; ---- dispatch table (32 entries) ----
        dd  loc_5_0244
        dd  obj5:0x029F
        dd  loc_5_0012
        dd  loc_11_0129
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_13_0120
        dd  loc_1_0620
        dd  loc_13_025A
        dd  loc_1_0620
        dd  loc_14_00C8
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_1_06B1
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_1_0620
        dd  loc_15_0000
SHELL_Control:
        pushal 
        call   sub_1_0614
        popal  
        jb     loc_1_06B0
        call   sub_1_06C4
loc_1_06B0:
        ret    
loc_1_06B1:
        or     edx, edx
        je     loc_1_06BA
        cmp    edx, 0xa
        jne    loc_1_06C0
loc_1_06BA:
        mov    dword ptr [0xa08], ebx        ; -> obj1:0x0A08
loc_1_06C0:
        clc    
        ret    
        db  000h, 000h
sub_1_06C4:
        cmp    eax, 0
        je     loc_5_0434
        cmp    eax, 1
        je     loc_5_0452
        cmp    eax, 7
        je     loc_13_03A3
        cmp    eax, 0xc
        je     sub_14_02CC
        cmp    eax, 8
        je     loc_13_044D
        cmp    eax, 9
        je     loc_13_03F8
        cmp    eax, 0xf
        je     loc_17_00E9
        cmp    eax, 0x15
        je     loc_11_02C9
        cmp    eax, 0x16
        je     loc_14_0338
        cmp    eax, 0x20
        je     loc_15_002C
        cmp    eax, 0x23
        je     loc_11_0B93
        clc    
        ret    
        db  000h, 000h, 000h
sub_1_072C:
        push   ecx
        push   ebx
        VMMCall  VMM.Get_Crit_Section_Status     ; cd2029000100
        pop    ebx
        pop    ecx
        jb     loc_1_073F
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        clc    
loc_1_073F:
        ret    
loc_1_0740:
        push   5
        pop    ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        mov    edi, edx
        cmp    word ptr [edi + 4], 0
        je     loc_1_07D8
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        xor    edx, edx
        and    byte ptr [ebp + 0x2d], 0xfe
        mov    dword ptr [ebp + 8], edx
        mov    eax, dword ptr [edi]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [edi + 4]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [edi + 8]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [edi + 0xc]
        ror    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        ror    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        movzx  ecx, word ptr [0xc0e]         ; -> loc_1_0C0E
        movzx  edx, word ptr [0xc0c]         ; -> loc_1_0C0C
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Wake_Up_VM                  ; cd2039000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        movzx  edx, word ptr [ebp + 0x1c]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    esi, esp
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        add    esp, 0x6c
loc_1_07D8:
        cmp    dword ptr [edi + 0x10], 0
        je     loc_1_07E9
        push   edi
        push   dword ptr [edi + 0x14]
        push   edx
        call   dword ptr [edi + 0x10]
        pop    ecx
        pop    ecx
        pop    edi
loc_1_07E9:
        mov    eax, 0xfffff000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
        mov    eax, edi
        mov    esi, dword ptr [0xa80]        ; -> loc_1_0A80
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        VMMCall  VMM.End_Critical_Section        ; cd2020800100
        add    byte ptr [eax], al
_SHELL_QueryAppyTimeAvailable:
        push   edi
        VMMCall  VMM.VMM_GetSystemInitState      ; cd2011010100
        cmp    eax, 0xa0000000
        mov    eax, 0
        jae    loc_1_0852
        mov    edi, dword ptr [0xbd8]        ; -> loc_1_0BD8
        test   edi, edi
        je     loc_1_0852
        inc    eax
        VMMCall  VMM.Test_Cur_Thread_Handle      ; cd2009010100
        jne    loc_1_0852
        mov    ecx, dword ptr [edi + 0x10]
        mov    edx, dword ptr [ecx + 0x26]
        mov    dx, word ptr [ecx + 0x24]
        cmp    edx, dword ptr [0xc14]        ; -> loc_1_0C14
        je     loc_1_0850
        cmp    edx, dword ptr [0xc18]        ; -> loc_1_0C18
        jne    loc_1_0852
loc_1_0850:
        or     al, 2
loc_1_0852:
        pop    edi
        ret    
_SHELL_CallAtAppyTime:
        push   ebx
        push   esi
        pushfd 
        cli    
        mov    esi, dword ptr [0xbe0]        ; -> loc_1_0BE0
        or     esi, esi
        jne    loc_1_086D
        call   sub_5_06C0
        mov    esi, dword ptr [0xbe0]        ; -> loc_1_0BE0
loc_1_086D:
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_1_08D5
        mov    ecx, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0x14]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 0xc], ecx
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        xchg   edx, eax
        and    ecx, 2
        je     loc_1_08A6
        mov    eax, dword ptr [esp + 0x1c]
        mov    esi, 0xdf4                    ; -> obj11:0x0DF4
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    ecx, esi
loc_1_08A6:
        mov    dword ptr [edx + 8], ecx
        cmp    dword ptr [0xbd8], 0          ; -> loc_1_0BD8
        je     loc_1_08D4
        bts    dword ptr [0xbe8], 0          ; -> loc_1_0BE8
        jb     loc_1_08D4
        xor    eax, eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    ecx, 0xa
        mov    esi, 0x3a                     ; -> loc_16_003A
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
loc_1_08D4:
        xchg   edx, eax
loc_1_08D5:
        popfd  
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h
loc_1_08DC:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_08E4:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_08EE:
        add    byte ptr [eax], al
        db  00Dh, 00Ah
loc_1_08F2:
        or     eax, 0xa
        add    byte ptr [eax], al
        db  000h
loc_1_08FA:
        add    dword ptr [edx], eax
        add    byte ptr [ecx], al
        add    byte ptr [edx], al
loc_1_0900:
        add    al, 3
        add    byte ptr [ebx], al
        add    byte ptr [eax + eax], al
        add    eax, 0x6000308
        add    byte ptr [edi], al
        add    byte ptr [edx], al
loc_1_0910:
        or     al, 2
        add    byte ptr [esi], al
        add    byte ptr [edi], al
loc_1_0916:
        lar    eax, word ptr [eax]
        add    al, 0
        db  002h
loc_1_091C:
        clc    
        or     byte ptr [eax], al
        add    dl, bh                        ; -> loc_1_08FA
        or     byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0900
        or     dword ptr [eax], eax
        add    byte ptr [eax], cl            ; -> obj1:0x0908
        or     dword ptr [eax], eax
        add    byte ptr [eax], dl            ; -> loc_1_0910
        or     dword ptr [eax], eax
        add    byte ptr [esi], dl            ; -> loc_1_0916
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [esp + ebx], bl
        add    byte ptr [eax], al
        dd  SHELL_Get_Version        ; fixup
        dd  SHELL_Resolve_Contention ; fixup
        dd  SHELL_Event              ; fixup
        dd  SHELL_SYSMODAL_Message   ; fixup
        dd  SHELL_Message            ; fixup
        dd  SHELL_GetVMInfo          ; fixup
        dd  _SHELL_PostMessage       ; fixup
        dd  _SHELL_ShellExecute      ; fixup
        dd  _SHELL_PostShellMessage  ; fixup
        dd  SHELL_DispatchRing0AppyEvents ; fixup
        dd  SHELL_Hook_Properties    ; fixup
        dd  SHELL_Unhook_Properties  ; fixup
        dd  SHELL_Update_User_Activity ; fixup
        dd  _SHELL_QueryAppyTimeAvailable ; fixup
        dd  _SHELL_CallAtAppyTime    ; fixup
        dd  _SHELL_CancelAppyTimeEvent ; fixup
        dd  _SHELL_BroadcastSystemMessage ; fixup
        dd  _SHELL_HookSystemBroadcast ; fixup
        dd  _SHELL_UnhookSystemBroadcast ; fixup
        dd  _SHELL_LocalAllocEx      ; fixup
        dd  _SHELL_LocalFree         ; fixup
        dd  _SHELL_LoadLibrary       ; fixup
        dd  _SHELL_FreeLibrary       ; fixup
        dd  _SHELL_GetProcAddress    ; fixup
        dd  _SHELL_CallDll           ; fixup
        dd  _SHELL_SuggestSingleMSDOSMode ; fixup
        dd  SHELL_CheckHotkeyAllowed ; fixup
        dd  _SHELL_GetDOSAppInfo     ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00017h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'SHELL   '                  ; DDB_Name
        dd  0B0000000h                  ; DDB_Init_Order
        dd  SHELL_Control            ; DDB_Control_Proc
        dd  SHELL_PM_API             ; DDB_V86_API_Proc
        dd  SHELL_PM_API             ; DDB_PM_API_Proc
        dd  obj1:0x0940              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_0A0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A10:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A18:
        add    byte ptr [eax], al
loc_1_0A1A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        push   edi
        push   ebx
        dec    eax
        inc    ebp
        dec    esp
        dec    esp
        and    byte ptr [eax], ah
        add    dword ptr [eax], eax
        add    byte ptr [eax + 0x6c4], dh    ; -> sub_1_06C4
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
        jbe    loc_1_0ABF
        jb     loc_1_0AAC
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], dh
        jbe    loc_1_0AD6
        push   edx
        xor    dh, byte ptr [esi + 0x73]
        push   edx
        xor    esi, dword ptr [esi + 0x73]
        push   edx
loc_1_0A6C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A74:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A78:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A7C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A80:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A84:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_0A8C:
        or     eax, dword ptr [eax]
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A94:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    ecx, dword ptr [ecx]
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], 0xf
        add    byte ptr [ecx + 0x8000], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_0AAC:
        lodsd  eax, dword ptr [esi]
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
        add    dword ptr [eax - 0x6effc6ff], eax
        add    byte ptr [ecx], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0ABF:
        add    byte ptr [ebp + 9], ch        ; -> loc_17_09AD
        add    dword ptr [eax - 0x6effe3ff], eax
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   cs
        db  009h
loc_1_0AD6:
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], 1
        add    byte ptr [edx + 0x80000], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, ah                        ; -> loc_17_08E0
        or     byte ptr [eax], al
        add    byte ptr [eax + 0x1018100], al
        add    byte ptr [ecx + 0x20000], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [9], bh              ; -> loc_11_093D
        add    dword ptr [ecx - 0x6effc8ff], eax
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [9], bh              ; -> loc_11_093D
        add    dword ptr [ecx - 0x6effabff], eax
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [0x1000009], bh      ; -> loc_11_093D
        add    byte ptr [ecx - 0x6effc8ff], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [9], bh              ; -> loc_11_093D
        add    byte ptr [ecx - 0x6efec8ff], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], 0
        add    byte ptr [eax], al
        mov    al, byte ptr [0x80000008]
        add    byte ptr [ecx - 0x6effd1ff], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], 0
        add    byte ptr [eax], al
        mov    al, byte ptr [8]
        add    byte ptr [ecx - 0x6efffeff], al
        add    byte ptr [eax], al
loc_1_0B6D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 6], ch        ; -> loc_11_066B
        db  000h, 000h
        dd  loc_11_066B              ; fixup
        dd  loc_11_066B              ; fixup
        dd  loc_11_066C              ; fixup
        dd  sub_11_068A              ; fixup
        dd  loc_11_0692              ; fixup
        dd  loc_11_066B              ; fixup
        dd  loc_11_066B              ; fixup
        dd  loc_11_066B              ; fixup
        dd  sub_11_06C1              ; fixup
        dd  sub_11_068A              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0BB4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BB8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BBC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BC0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BC4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BC8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BCC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BD0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BD4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BD8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BDC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BE0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BE4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BE8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BEC:
        add    byte ptr [eax], al
loc_1_0BEE:
        add    byte ptr [eax], al
loc_1_0BF0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BFC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C00:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C04:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C08:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C0C:
        add    byte ptr [eax], al
loc_1_0C0E:
        add    byte ptr [eax], al
loc_1_0C10:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C18:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C1C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C20:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x1bd  R-X
        db  014h, 000h, 000h, 000h
loc_2_0004:
        cmp    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        adc    byte ptr [eax], al
loc_2_0010:
        pop    ds
        add    byte ptr [eax], dl
        xchg   byte ptr [eax], al
        mov    dword ptr [eax], eax
        mov    eax, dword ptr [eax]
        mov    byte ptr [eax], 0xc8
        add    dl, cl
        add    ah, cl
        add    byte ptr [esi], al
        add    dword ptr [eax], ecx
        add    dword ptr [edx], ecx
        add    dword ptr [ecx], edx
        add    dword ptr [ebx + 1], eax
        inc    ebp
        add    dword ptr [edi + 1], eax
        jl     0x33
        jle    0x35
        test   dword ptr [ecx], eax
        add    byte ptr [eax], al
loc_2_0038:
        inc    esp
        jbe    0xa5
        arpl   word ptr [ebp + 0x20], sp
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x74], 0x73795300
        je     0xb2
        insd   dword ptr es:[edi], dx
        and    byte ptr [eax + 0x61], ch
        insb   byte ptr es:[edi], dx
        je     0xb9
        add    byte ptr cs:[eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax + 0x72], dl
        jae    0xe5
        and    byte ptr [ecx + 0x6e], ah
        jns    0x97
        imul   esp, dword ptr [ebp + 0x79], 0x20
        je     loc_2_00EC
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        je     loc_2_00EC
        outsb  dx, byte ptr [esi]
        jne    0xeb
        and    byte ptr [eax], al
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   eax
        jb     loc_2_0100
        jae    loc_2_0110
        and    byte ptr [ebp + 0x4e], al
        push   esp
        inc    ebp
        push   edx
        and    byte ptr [esi + 0x6f], ah
        jb     0xc8
        dec    edi
        dec    ebx
        and    byte ptr [edi + 0x72], ch
        and    byte ptr [ebp + 0x53], al
        inc    ebx
        and    byte ptr [edi + ebp*2 + 0x20], dh
        inc    ebx
        popal  
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ebp + 0x6c], sp
        cmp    ah, byte ptr [eax]
        add    byte ptr [edi + 0x4b], cl
        add    byte ptr [eax], bl
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   eax
        jb     loc_2_0137
        jae    0x147
        and    byte ptr [ecx + 0x20], al
        je     loc_2_0148
        and    byte ptr [ecx + 0x62], al
        outsd  dx, dword ptr [esi]
        jb     0x153
        sub    al, 0x20
        push   edx
        and    byte ptr [edi + ebp*2 + 0x20], dh
        push   edx
        je     loc_2_015C
        jns    loc_2_0118
loc_2_00EC:
        and    byte ptr [edi + 0x72], ch
        and    byte ptr [ecx + 0x20], cl
        je     0x163
        and    byte ptr [ecx + 0x67], cl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        jb     0x160
        cmp    ah, byte ptr [eax]
        add    byte ptr [ecx], al
loc_2_0100:
        push   edx
        add    byte ptr [ecx], cl
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_0110:
        push   eax
        jb     loc_2_0178
        jae    loc_2_0188
        and    byte ptr [ecx + 0x20], bl
loc_2_0118:
        outsw  dx, word ptr [esi]
        jb     0x13c
        pop    ecx
        jae    loc_2_014C
        and    byte ptr [esi + 0x20], cl
        outsw  dx, word ptr [esi]
        jb     0x147
        dec    esi
        outsd  dx, dword ptr [esi]
        sub    al, 0x20
        outsd  dx, dword ptr [esi]
        jb     loc_2_014E
        inc    ebp
        push   ebx
        inc    ebx
        and    byte ptr [edi + ebp*2 + 0x20], dh
        inc    ebx
        popal  
loc_2_0137:
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ebp + 0x6c], sp
        cmp    ah, byte ptr [eax]
        add    byte ptr [ecx], bl
        dec    esi
        add    byte ptr [ebx + 0x61], al
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ebp + 0x6c], sp
loc_2_0148:
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_014C:
        and    byte ptr [eax], ah
loc_2_014E:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_015C:
        and    byte ptr [eax + 0x72], dl
        jae    0x1d5
        and    byte ptr [ecx + 0x20], bl
        outsw  dx, word ptr [esi]
        jb     0x189
        pop    ecx
        jae    0x18d
        outsd  dx, dword ptr [esi]
        jb     loc_2_0190
        dec    esi
        and    byte ptr [esi + 0x6f], ah
        jb     0x196
        dec    esi
        outsd  dx, dword ptr [esi]
loc_2_0178:
        cmp    ah, byte ptr [eax]
        add    byte ptr [ecx], bl
        dec    esi
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_0188:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_0190:
        push   eax
        jb     0x1f8
        jae    0x208
        and    byte ptr [edx + 0x20], dl
        je     0x209
        and    byte ptr [edx + 0x65], dl
        je     0x211
        jns    0x1c1
        outsd  dx, dword ptr [esi]
        jb     0x1c4
        inc    ebp
        push   ebx
        inc    ebx
        and    byte ptr [edi + ebp*2 + 0x20], dh
        inc    ebx
        popal  
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ebp + 0x6c], sp
        cmp    ah, byte ptr [eax]
        add    byte ptr [edx], dl
        inc    ebx
        popal  
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ebp + 0x6c], sp
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x6d  R-X
        db  011h, 000h, 000h, 000h
loc_3_0004:
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_3_000C:
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        push   es
        add    byte ptr [edx], cl
        add    byte ptr [esi], cl
        add    byte ptr [edx], dl
        add    byte ptr [esi], dl
        add    byte ptr [edx], bl
        add    byte ptr [0x25002100], bl
        add    byte ptr [ecx], ch
        db  000h
loc_3_0024:
        sub    al, 0
loc_3_0026:
        das    
        db  000h
loc_3_0028:
        xor    dword ptr [eax], eax
loc_3_002A:
        xor    eax, dword ptr [eax]
loc_3_002C:
        xor    eax, 0x1c003d00
        or     eax, 0x1b0100
        push   ds
        inc    ecx
        popal  
        add    byte ptr [ebx], dl
        push   edx
        jb     loc_3_003E
loc_3_003E:
        pop    ss
        dec    ecx
        imul   eax, dword ptr [eax], 0x795915
        xor    dword ptr [esi + 0x6e], ecx
        add    byte ptr [ecx], al
        sbb    eax, dword ptr [eax]
        adc    eax, 0x31007959
        dec    esi
        outsb  dx, byte ptr [esi]
        add    byte ptr [ebx], dl
        push   edx
        jb     loc_3_0059
loc_3_0059:
        add    dword ptr [ebx], ebx
        add    byte ptr [ecx + 0x35000100], bl
        add    byte ptr [eax], dh
        add    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0xdf
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x5a1  R-X
loc_4_0000:
        mov    esi, dword ptr [0xbe0]        ; -> loc_1_0BE0
        test   esi, esi
        je     loc_4_0050
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        jne    loc_4_001D
        push   0x98                          ; -> obj8:0x0098
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_4_001D:
        je     loc_4_0050
        push   0xc3                          ; -> obj8:0x00C3
loc_4_0024:
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        push   0xeb                          ; -> obj8:0x00EB
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_4_0035:
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [eax + 4]
        mov    edi, dword ptr [eax + 0xc]
        push   0x108                         ; -> obj8:0x0108
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_4_0035
loc_4_0050:
        ret    
        db  08Bh, 035h
        dd  loc_1_0BE4               ; fixup
        db  085h, 0F6h, 074h, 03Ch, 068h
        dd  obj8:0x011E              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h
        dd  obj8:0x0147              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 0CDh, 020h, 0A3h, 000h, 001h, 000h, 074h, 01Eh, 08Bh, 008h
        db  08Bh, 050h, 004h, 08Bh, 058h, 008h, 08Bh, 078h, 00Ch, 068h
        dd  obj8:0x016D              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 0CDh, 020h, 0A4h, 000h, 001h, 000h, 075h, 0E2h, 0C3h, 04Eh
        db  06Fh, 020h, 027h, 061h, 070h, 070h, 079h, 020h, 074h, 069h, 06Dh, 065h, 020h, 065h, 076h, 065h
        db  06Eh, 074h, 073h, 020h, 061h, 077h, 061h, 069h, 074h, 069h, 06Eh, 067h, 020h, 070h, 072h, 06Fh
        db  063h, 065h, 073h, 073h, 069h, 06Eh, 067h, 00Dh, 00Ah, 000h, 027h, 041h, 070h, 070h, 079h, 02Dh
        db  074h, 069h, 06Dh, 065h, 020h, 065h, 076h, 065h, 06Eh, 074h, 073h, 020h, 061h, 077h, 061h, 069h
        db  074h, 069h, 06Eh, 067h, 020h, 070h, 072h, 06Fh, 063h, 065h, 073h, 073h, 069h, 06Eh, 067h, 00Dh
        db  00Ah, 000h, 020h, 020h, 020h, 066h, 06Ch, 061h, 067h, 073h, 020h, 020h, 072h, 065h, 066h, 064h
        db  061h, 074h, 061h, 020h, 020h, 020h, 020h, 020h, 070h, 072h, 06Fh, 063h, 00Dh, 00Ah, 000h, 023h
        db  045h, 044h, 049h, 020h, 023h, 045h, 044h, 058h, 020h, 023h, 045h, 043h, 058h, 020h, 03Fh, 045h
        db  043h, 058h, 00Dh, 00Ah, 000h, 049h, 06Eh, 073h, 074h, 061h, 06Ch, 06Ch, 065h, 064h, 020h, 042h
        db  072h, 06Fh, 061h, 064h, 063h, 061h, 073h, 074h, 053h, 079h, 073h, 074h, 065h, 06Dh, 04Dh, 065h
        db  073h, 073h, 061h, 067h, 065h, 020h, 068h, 06Fh, 06Fh, 06Bh, 073h, 00Dh, 00Ah, 000h, 06Eh, 075h
        db  06Dh, 02Dh, 068h, 06Fh, 06Ch, 064h, 020h, 043h, 061h, 06Ch, 06Ch, 04Fh, 072h, 064h, 072h, 020h
        db  020h, 072h, 065h, 066h, 064h, 061h, 074h, 061h, 020h, 020h, 020h, 020h, 020h, 070h, 072h, 06Fh
        db  063h, 00Dh, 00Ah, 000h, 023h, 045h, 044h, 049h, 020h, 023h, 045h, 042h, 058h, 020h, 023h, 045h
        db  044h, 058h, 020h, 023h, 045h, 043h, 058h, 020h, 03Fh, 045h, 043h, 058h, 00Dh, 00Ah, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x7e8  R-X
sub_5_0000:
        nop    
        ret    2
        db  0FBh, 084h, 0E4h, 074h, 014h, 03Dh, 002h, 04Fh
loc_5_000C:
        je     loc_5_0018
        db  080h, 0FCh
loc_5_0010:
        dec    edi
        db  074h
loc_5_0012:
        or     ch, dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0018:
        push   ebx
        push   eax
        mov    eax, ebx
        cmp    eax, 0x7f255050
        jg     0xffffffa6
        clc    
        add    esi, dword ptr [esi + 0xb]
        cmp    eax, 7
        je     loc_5_0032
        pop    eax
        call   0xe1ec0002
loc_5_0032:
        pop    eax
        pushfd 
        push   cs
        call   0xc5e90015
        dec    edx
        add    al, byte ptr [eax]
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
loc_5_0244:
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
loc_5_0434:
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
loc_5_0452:
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
sub_5_06C0:
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
SHELL_DispatchRing0AppyEvents:
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
loc_5_0750:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_0754:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_075A:
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
loc_5_077A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_078C:
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
loc_5_07BA:
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
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x94  R-X
loc_6_0000:
        or     eax, 0x28000000               ; -> obj4:0x0028
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [0x9b021f00], bh
        add    al, byte ptr [ebx + eax - 0x75]
        add    edx, edx
        add    esp, dword ptr [esp + eax]
        jb     0x22
        lds    eax, ptr [ebx + edx]
        add    eax, 0x5710542
        jns    0x2d
        push   eax
        insb   byte ptr es:[edi], dx
        popal  
        jae    0x93
        and    byte ptr [edi + 0x61], dh
        imul   esi, dword ptr [eax + 0x77], 0x656c6968
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0xb4
        and    byte ptr [eax + 0x72], dh
        jo     0xa8
        jb     0xae
        jae    loc_6_006B
        je     0xbc
        and    byte ptr [edx + 0x75], dh
        outsb  dx, byte ptr [esi]
        and    byte ptr [eax + ebp*2 + 0x69], dh
        jae    loc_6_0077
        jo     0xcb
        outsd  dx, dword ptr [esi]
        jb     0xbe
        insd   dword ptr es:[edi], dx
        db  02Eh, 02Eh, 02Eh, 00Dh, 00Ah, 024h, 000h
SHELL_PM_API:
        or     cl, byte ptr [0xcdcdcdc9]
loc_6_006B:
        int    0xcd
        int    0xcd
        int    0xcd
        int    0xcd
        int    0xcd
        int    0xcd
loc_6_0077:
        int    0xcd
        int    0xcd
        int    0xcd
        int    0xcd
        int    0xcd
        int    0xcd
        int    0xcd
        int    0xcd
        int    0xcd
        int    0xcd
        int    0xcd
        int    0xcd
        int    0xcd
        int    0xcd
        db  0CDh
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 7  vsize=0xe8  RW-
        db  060h, 0E8h, 00Ch, 000h, 000h, 000h, 061h, 083h, 02Ch, 024h, 006h, 0C3h, 0CDh, 020h, 005h, 000h
        db  001h, 000h, 080h, 03Dh
        dd  loc_1_0024               ; fixup
        db  090h, 00Fh, 085h, 0F9h, 001h, 000h, 000h, 0A1h
        dd  loc_5_000C               ; fixup
        db  0A3h
        dd  loc_1_0024               ; fixup
        db  066h, 0A1h
        dd  loc_5_0010               ; fixup
        db  066h, 0A3h
        dd  obj1:0x0028              ; fixup
        db  0A1h
        dd  loc_1_08DC               ; fixup
        db  0BFh
        dd  loc_5_077A               ; fixup
        db  033h, 0F6h, 0CDh, 020h, 0AFh, 000h, 001h, 000h, 0A3h
        dd  loc_1_08DC               ; fixup
        db  0A1h
        dd  obj1:0x08E0              ; fixup
        db  0BFh
        dd  obj5:0x0769              ; fixup
        db  033h, 0F6h, 0CDh, 020h, 0AFh, 000h, 001h, 000h, 0A3h
        dd  obj1:0x08E0              ; fixup
        db  00Fh, 0B7h, 005h
        dd  loc_3_002A               ; fixup
        db  003h, 005h
        dd  loc_3_0004               ; fixup
        db  00Fh, 0B6h, 000h, 083h, 0E8h, 030h, 083h, 0F8h, 002h, 00Fh, 084h, 04Eh, 001h, 000h, 000h, 083h
        db  0F8h, 003h, 074h, 015h, 0BFh
        dd  loc_5_075A               ; fixup
        db  033h, 0F6h, 0CDh, 020h, 0B1h, 000h, 001h, 000h, 00Ah, 0C0h, 00Fh, 084h, 034h, 001h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h, 083h, 0ECh, 06Ch, 08Bh, 0FCh, 0CDh, 020h, 08Dh, 000h, 001h
        db  000h, 0CDh, 020h, 083h, 000h, 001h, 000h, 08Bh, 06Bh, 008h, 0FCh, 0BEh
        dd  loc_5_0750               ; fixup
        db  00Fh, 0B7h, 00Dh
        dd  loc_3_0026               ; fixup
        db  003h, 00Dh
        dd  loc_3_0004               ; fixup
        db  08Ah, 009h, 088h, 00Eh, 00Fh, 0B7h, 00Dh
        dd  loc_3_0028               ; fixup
        db  003h, 00Dh
        dd  loc_3_0004               ; fixup
        db  00Fh, 0B6h, 009h, 02Ah, 00Eh, 041h, 051h, 057h, 056h, 0E8h, 035h, 001h, 000h, 000h, 0B9h, 001h
        db  000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 8  vsize=0x188  R-X
        db  033h, 0C9h, 089h, 00Dh
        dd  loc_1_0A14               ; fixup
        db  086h, 00Dh
        dd  obj1:0x0A19              ; fixup
        db  0E3h, 008h, 088h, 02Dh
        dd  loc_1_0A1A               ; fixup
        db  0EBh, 036h, 038h, 00Dh
        dd  loc_1_0A1A               ; fixup
        db  075h, 02Eh, 0FEh, 005h
        dd  loc_1_0A1A               ; fixup
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h, 08Bh, 015h
        dd  obj1:0x0A00              ; fixup
        db  00Fh, 0B7h, 04Ch, 013h, 036h, 0E3h, 015h, 033h, 0C0h, 050h, 050h, 050h, 050h, 068h, 02Ch, 004h
        db  000h, 000h, 051h, 0CDh, 020h, 006h, 000h, 017h, 000h, 083h, 0C4h, 018h, 0A1h
        dd  loc_1_0A10               ; fixup
        db  0BEh
        dd  loc_6_0000               ; fixup
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h, 089h, 035h
        dd  loc_1_0A14               ; fixup
        db  0C3h, 053h, 055h, 0E8h
        dd  loc_17_01DE              ; fixup
        db  05Dh, 05Bh, 040h, 075h, 01Fh, 066h, 08Bh, 045h, 014h, 080h, 0ECh, 001h, 083h, 0F8h, 008h, 077h
        db  00Bh, 080h, 065h, 02Ch, 0FEh, 0FFh, 024h, 085h
        dd  obj7:0x0000              ; fixup
        db  080h, 04Dh, 02Ch, 001h, 083h, 04Dh, 01Ch, 0FFh, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 9  vsize=0x3e  R-X
        dd  loc_11_0000              ; fixup
        dd  obj6:0x0088              ; fixup
        dd  obj6:0x0088              ; fixup
        dd  obj6:0x0088              ; fixup
        dd  loc_13_001C              ; fixup
        dd  loc_13_0085              ; fixup
        dd  loc_11_00CA              ; fixup
        dd  loc_11_010D              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 020h, 008h, 000h, 000h
        dd  loc_13_0494              ; fixup
        dd  loc_12_0022              ; fixup
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 10  vsize=0x151  R-X
        db  0EBh, 006h, 0FFh, 025h
        dd  loc_1_0A0C               ; fixup
loc_10_0008:
        push   ebx
        add    ebx, dword ptr [0xa00]        ; -> obj1:0x0A00
        cmp    ch, 3
        je     loc_10_0020
        btr    word ptr [ebx], 3
        jae    loc_10_0023
        or     byte ptr [ebx], 0x10
        jmp    loc_10_0026
loc_10_0020:
        or     byte ptr [ebx], 0x28
loc_10_0023:
        and    byte ptr [ebx], 0xef
loc_10_0026:
        pop    ebx
        jmp    loc_1_0A0C
        db  000h, 000h, 000h, 0EBh, 006h, 0FFh, 025h
        dd  loc_1_0BB4               ; fixup
loc_10_0038:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_10_007C
        pushal 
        mov    ebp, dword ptr [0xa6c]        ; -> loc_1_0A6C
        and    dword ptr [ebp + ebx + 0x14], 0
        lea    edi, [esi - 8]
        call   sub_17_0608
        jb     loc_10_007B
        mov    word ptr [eax], dx
        mov    edx, eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        cmp    word ptr [eax], 0
        je     loc_10_006F
        cld    
        mov    esi, edi
        lea    edi, [edx + 4]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
loc_10_006F:
        call   sub_17_0740
        xor    edx, edx
        call   sub_17_0661
loc_10_007B:
        popal  
loc_10_007C:
        jmp    loc_1_0BB4
        db  0EBh, 006h, 0FFh, 025h
        dd  loc_1_0BB8               ; fixup
loc_10_008A:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_10_0125
        pushal 
        mov    ecx, dword ptr [0xa6c]        ; -> loc_1_0A6C
        and    byte ptr [ecx + ebx + 0x10], 0xfe
        xor    edx, edx
        xchg   dword ptr [ecx + ebx + 0x14], edx
        test   edx, edx
        je     loc_10_00C7
        VMMCall  VMM.Get_VM_Exec_Time            ; cd2040000100
        sub    eax, edx
        cmp    eax, 0x64
        jae    loc_10_00C7
        pushal 
        push   0
        push   ebx
        VxDCall  SHELL._SHELL_SuggestSingleMSDOSMode ; cd2019001700
        add    esp, 8
        popal  
loc_10_00C7:
        mov    edx, dword ptr [esi + 0x16]
        call   sub_17_05F8
loc_10_00CF:
        cmp    word ptr [eax], dx
        je     loc_10_00DE
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_10_00CF
        jmp    loc_10_0118
loc_10_00DE:
        xchg   edx, eax
        jmp    loc_10_010E
loc_10_00E1:
        test   byte ptr [eax + 3], 1
        je     loc_10_00F8
        pushal 
        mov    ecx, 0x428
        xor    eax, eax
        xor    esi, esi
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        popal  
loc_10_00F8:
        test   byte ptr [eax + 3], 2
        jne    loc_10_0102
        dec    byte ptr [ecx + ebx + 0x18]
loc_10_0102:
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_10_010E:
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        cmp    edx, eax
        jne    loc_10_00E1
loc_10_0118:
        call   sub_17_0740
        xor    edx, edx
        call   sub_17_0661
        popal  
loc_10_0125:
        jmp    loc_1_0BB8
        db  000h
loc_10_012C:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_10_014F
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ax, 0x4b00
        je     loc_17_0F74
        cmp    ah, 0x2b
        je     loc_10_0149
        cmp    ah, 0x2d
loc_10_0149:
        je     0xfff6103e                    ; -> obj11:0x103E
loc_10_014F:
        stc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 11  vsize=0x106c  R-X
loc_11_0000:
        mov    word ptr [ebp + 0x1c], 0x400
        ret    
SHELL_Hook_Properties:
        push   ebx
        push   esi
        push   edi
        cmp    eax, 0xff
        ja     loc_11_001D
        or     eax, eax
        je     loc_11_001A
        cmp    eax, 0xa
        jbe    loc_11_001D
loc_11_001A:
        stc    
        jmp    loc_11_0077
loc_11_001D:
        mov    ebx, esi
        xchg   edx, eax
        xchg   edi, eax
        mov    esi, dword ptr [0x20]         ; -> obj7:0x0020
        or     esi, esi
        jne    loc_11_0043
        mov    eax, 4
        mov    ecx, 0xc
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_11_0077
        mov    dword ptr [0x20], esi         ; -> obj7:0x0020
loc_11_0043:
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_11_0077
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax + 8], ebx
        push   eax
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_11_006B
loc_11_005C:
        call   sub_13_00DD
        je     loc_11_006D
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_11_005C
loc_11_006B:
        sub    eax, eax
loc_11_006D:
        mov    ecx, eax
        pop    eax
        VMMCall  VMM.List_Insert                 ; cd20a0000100
        clc    
loc_11_0077:
        pop    edi
        pop    esi
        pop    ebx
        ret    
SHELL_Unhook_Properties:
        push   ebx
        push   esi
        cmp    eax, 0xff
        ja     loc_11_008C
        cmp    eax, 0xa
        jbe    loc_11_008C
        stc    
        jmp    loc_11_00C7
loc_11_008C:
        mov    ebx, esi
        mov    edx, eax
        mov    esi, dword ptr [0x20]         ; -> obj7:0x0020
        or     esi, esi
        je     loc_11_00C7
loc_11_009A:
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_11_00C7
loc_11_00A2:
        call   sub_13_00DD
        jne    loc_11_00BE
        cmp    dword ptr [eax + 8], ebx
        jne    loc_11_00BE
        push   eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        jmp    loc_11_009A
loc_11_00BE:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_11_00A2
        clc    
loc_11_00C7:
        pop    esi
        pop    ebx
        ret    
loc_11_00CA:
        cmp    byte ptr [0xa18], 1           ; -> loc_1_0A18
        jne    loc_11_0108
        xor    esi, esi
        xchg   dword ptr [0xa14], esi        ; -> loc_1_0A14
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        movzx  ecx, word ptr [ebp + 0x10]
        jecxz  loc_11_0103
        imul   eax, ecx, 0x3e8
        mov    dword ptr [0xa10], eax        ; -> loc_1_0A10
        mov    esi, 0                        ; -> loc_6_0000
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0xa14], esi        ; -> loc_1_0A14
loc_11_0103:
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_11_0108:
        or     byte ptr [ebp + 0x2c], 1
        ret    
loc_11_010D:
        VMMCall  VMM.VMM_GetDDBList              ; cd203f010100
        xchg   ecx, eax
        movzx  eax, word ptr [ebp + 0x1c]
loc_11_0118:
        mov    ecx, dword ptr [ecx]
        jecxz  loc_11_0125
        cmp    word ptr [ecx + 6], ax
        jne    loc_11_0118
        mov    ecx, dword ptr [ecx + 4]
loc_11_0125:
        mov    dword ptr [ebp + 0x1c], ecx
        ret    
loc_11_0129:
        mov    esi, dword ptr [0x24]         ; -> obj7:0x0024
        mov    edi, dword ptr [0x28]         ; -> obj7:0x0028
        cmp    edi, 0x280
        jne    loc_11_0142
        mov    edi, 0xffff
loc_11_0142:
        cmp    esi, 0x280
        jne    loc_11_014F
        mov    esi, 0xffff
loc_11_014F:
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        shl    eax, 2
        cmp    esi, 0xffff
        jne    loc_11_016E
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        shl    eax, 2
        mov    esi, eax
        jmp    loc_11_0170
loc_11_016E:
        add    esi, eax
loc_11_0170:
        cmp    edi, 0xffff
        jne    loc_11_0186
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        shl    eax, 2
        mov    edi, eax
        jmp    loc_11_0188
loc_11_0186:
        add    edi, eax
loc_11_0188:
        cmp    edi, esi
        jbe    loc_11_018E
        mov    edi, esi
loc_11_018E:
        xor    ecx, ecx
        mov    edx, dword ptr [0xa00]        ; -> obj1:0x0A00
        test   byte ptr [edx + ebx + 0xe], 4
        je     loc_11_01A0
        or     ecx, 1
loc_11_01A0:
        VxDCall  V86MMGR.V86MMGR_Allocate_V86_Pages ; cd2001000600
        jb     loc_11_0235
        mov    dword ptr [0x24], eax         ; -> obj7:0x0024
        VMMCall  VMM.Get_Time_Slice_Priority     ; cd2032000100
        mov    esi, dword ptr [0xa00]        ; -> obj1:0x0A00
        and    byte ptr [esi + ebx + 0xc], 0xf2
        or     byte ptr [esi + ebx + 0xc], 2
        mov    word ptr [esi + ebx + 0x2a], cx
        mov    word ptr [esi + ebx + 0x2c], dx
        and    eax, 0xfffffffe
        or     eax, 2
        or     eax, dword ptr [0x30]         ; -> obj7:0x0030
        push   eax
        xor    eax, eax
        VxDCall  VDD.VDD_Set_VMType              ; cd2004000a00
        pop    eax
        VMMCall  VMM.Set_Time_Slice_Priority     ; cd2033000100
        jb     loc_11_0235
        mov    edi, dword ptr [0x2c]         ; -> obj7:0x002C
        lea    eax, [edi]
        lea    ecx, [edi + 0x100]
        lea    edx, [edi + 0x80]
        push   ecx
        push   eax
        push   edx
        push   ebx
        VxDCall  DOSMGR._DOSMGR_Set_Exec_VM_Data ; cd2001001500
        add    esp, 0x10
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        push   0
        push   0x17b                         ; -> loc_14_017B
        push   dword ptr [0x24]              ; -> obj7:0x0024
        push   ebx
        VxDCall  DOSMGR._DOSMGR_Exec_VM          ; cd2003001500
        add    esp, 0x10
        clc    
        ret    
loc_11_0235:
        push   0x80004
        push   0                             ; -> loc_4_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        pushal 
        mov    esi, eax
        xor    eax, eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
loc_11_0250:
        mov    eax, dword ptr [esp + 0xc]
        pushal 
        cmp    byte ptr [eax + 0x11], 3
        jne    loc_11_0281
        mov    edi, dword ptr [eax + 0xc]
        mov    ecx, dword ptr [0xa00]        ; -> obj1:0x0A00
        mov    ebx, dword ptr [edi + 0x14]
        cmp    word ptr [ecx + ebx + 0x36], 0
        je     loc_11_0281
        or     byte ptr [ecx + ebx + 0xf], 8
        xor    esi, esi
        xor    edx, edx
        lea    eax, [edx + 0xf]
        VMMCall  VMM.System_Control              ; cd2093000100
loc_11_0281:
        popal  
        ret    
loc_11_0283:
        push   ebx
        mov    eax, dword ptr [esp + 0x10]
        cmp    byte ptr [eax + 0x11], 3
        jne    loc_11_029E
        mov    eax, dword ptr [eax + 0xc]
        mov    ebx, dword ptr [eax + 0x14]
        mov    eax, dword ptr [0xa00]        ; -> obj1:0x0A00
        and    byte ptr [eax + ebx + 0xf], 0xf7
loc_11_029E:
        pop    ebx
        ret    
loc_11_02A0:
        mov    ebx, edx
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_11_02BD
        call   sub_17_000F
        and    byte ptr [eax + 6], 0xfd
        push   eax
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
loc_11_02BD:
        ret    
loc_11_02BE:
        VMMCall  VMM.Get_Instanced_V86_Int_Vector ; cd2041010100
        cmp    eax, 1
        cmc    
        ret    
loc_11_02C9:
        mov    esi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        and    dword ptr [esi + ebx + 0x14], 0
        ret    
loc_11_02D5:
        btr    word ptr [0xb6d], 2           ; -> loc_1_0B6D
        ret    
        db  000h
sub_11_02E0:
        enter  0x18, 0
        or     dword ptr [ebp - 8], 0xffffffff
        test   eax, 0x10000000
        je     loc_11_02F6
        call   sub_11_0F3E
        jmp    loc_11_0307
loc_11_02F6:
        mov    esi, 0x309                    ; -> loc_11_0309
        call   sub_11_0D00
        cmp    eax, -2
        jb     loc_11_0307
        mov    al, 0xff
loc_11_0307:
        leave  
        ret    
loc_11_0309:
        mov    esi, 0x32b                    ; -> loc_11_032B
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0x1fffffff
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0x14]
        mov    ebx, dword ptr [ebp + 8]
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
        jb     loc_11_0328
        ret    
loc_11_0328:
        or     eax, 0xffffffff
loc_11_032B:
        mov    ebp, edx
        jmp    loc_11_0DBB
sub_11_0332:
        or     dword ptr [ebp + 0xc], 0xffffffff
        test   eax, 0x40000000
        jne    loc_11_03F1
        call   sub_11_041A
        jb     loc_11_03F1
        call   sub_17_000F
        cmp    word ptr [eax + 0x2a], 0
        je     loc_11_03B9
        mov    edi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        bts    dword ptr [edi + ebx + 0x10], 4
        jb     loc_11_03DF
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    dword ptr [edx + 0x1c], eax
        push   ebx
        VMMCall  VMM.Get_Crit_Section_Status     ; cd2029000100
        pop    ebx
        jb     loc_11_039A
        test   dword ptr [ebp + 0x1c], 0x80001000
        jne    loc_11_039A
        mov    esi, 0x495                    ; -> loc_11_0495
        mov    ecx, 0x419
        xor    eax, eax
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        jae    loc_11_03F1
        call   esi
        jmp    loc_11_03F1
loc_11_039A:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        or     byte ptr [edx + 0x18], 1
        push   7
        pop    ecx
        mov    eax, 0x1000
        mov    esi, 0x465                    ; -> loc_11_0465
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        jmp    loc_11_03F1
loc_11_03B9:
        push   edx
        VxDCall  SHELL._SHELL_QueryAppyTimeAvailable ; cd200d001700
        pop    edx
        test   al, 1
        je     loc_11_03DF
        push   0x1388
        push   2
        push   edx
        push   0x3f5                         ; -> loc_11_03F5
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0x10
        test   eax, eax
        jne    loc_11_03F1
loc_11_03DF:
        or     eax, 0xffffffff
        xchg   dword ptr [ebp + 0xc], eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_11_03F1:
        mov    eax, dword ptr [ebp + 0xc]
        ret    
loc_11_03F5:
        pushal 
        mov    edi, dword ptr [esp + 0x24]
        test   byte ptr [esp + 0x28], 2
        jne    loc_11_0413
        mov    ebx, dword ptr [0xa08]        ; -> obj1:0x0A08
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_11_0413
        xor    byte ptr [edi + 0xb], 0x50
loc_11_0413:
        call   sub_11_04AF
        popal  
        ret    
sub_11_041A:
        mov    esi, dword ptr [0xa78]        ; -> loc_1_0A78
        cmp    esi, 1
        jb     loc_11_0464
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_11_0464
        VMMCall  VMM.List_Attach                 ; cd209e000100
        xchg   edx, eax
        xor    eax, eax
        mov    dword ptr [edx + 0x18], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [edx + 0x14], eax
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebp]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [edx + 0xc], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [edx + 0x10], eax
        mov    eax, dword ptr [ebp + 0x1c]
        or     eax, 0x40000000
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [ebp + 0xc], edx
loc_11_0464:
        ret    
loc_11_0465:
        mov    ebx, dword ptr [edx + 0x14]
        mov    esi, 0x495                    ; -> loc_11_0495
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_11_0492
        xor    eax, eax
        test   dword ptr [edx + 8], 0x80001000
        je     loc_11_0485
        or     eax, 0x100001
loc_11_0485:
        mov    ecx, 0x419
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        jae    loc_11_0494
loc_11_0492:
        call   esi
loc_11_0494:
        ret    
loc_11_0495:
        pushfd 
        mov    edi, edx
        mov    ebx, dword ptr [edi + 0x14]
        add    ebx, dword ptr [0xa6c]        ; -> loc_1_0A6C
        and    byte ptr [ebx + 0x10], 0xef
        popfd  
        jae    loc_11_04AA
        jmp    sub_11_04AF
loc_11_04AA:
        mov    eax, dword ptr [ebp + 0x18]
        jmp    loc_11_04CF
sub_11_04AF:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        call   sub_11_04F8
        push   edi
        push   ebx
        mov    ebx, dword ptr [edi + 0x14]
        mov    eax, dword ptr [edi + 8]
        mov    ecx, dword ptr [edi]
        mov    edi, dword ptr [edi + 4]
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        pop    ebx
        pop    edi
loc_11_04CF:
        cmp    dword ptr [edi + 0xc], 0
        je     loc_11_04DF
        push   edi
        push   ebx
        mov    edx, dword ptr [edi + 0x10]
        call   dword ptr [edi + 0xc]
        pop    ebx
        pop    edi
loc_11_04DF:
        call   sub_11_04F8
        mov    eax, edi
        mov    esi, dword ptr [0xa78]        ; -> loc_1_0A78
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        VMMCall  VMM.List_Deallocate             ; cd20a2800100
sub_11_04F8:
        btr    dword ptr [edi + 0x18], 0
        jae    loc_11_0525
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, dword ptr [edi + 0x1c]
        VMMCall  VMM.Adjust_Execution_Time       ; cd2037000100
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, 0xfffff000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
loc_11_0525:
        ret    
        db  000h, 000h
sub_11_0528:
        enter  0x18, 0
        mov    esi, 0x55b                    ; -> loc_11_055B
        call   sub_11_0D00
        jae    loc_11_0544
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        mov    dword ptr [0xb78], ebx        ; -> obj1:0x0B78
loc_11_0544:
        leave  
        ret    
loc_11_0546:
        mov    ebx, ebp
        mov    esi, dword ptr [ebp]
        mov    ebp, dword ptr [esi + 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 0x14], eax
        ret    
loc_11_055B:
        mov    esi, 0x573                    ; -> loc_11_0573
        mov    ebx, dword ptr [ebp + 0xc]
        mov    ecx, 0x41d
        xor    eax, eax
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        jb     loc_11_0573
        ret    
loc_11_0573:
        jb     loc_11_0DD6
        mov    eax, dword ptr [ebp + 0x18]
        mov    ebp, edx
        jmp    loc_11_0DBB
sub_11_0583:
        mov    ebx, dword ptr [0xb78]        ; -> obj1:0x0B78
        call   sub_17_000F
        test   byte ptr [eax], 4
        je     loc_11_05A4
        mov    ecx, 0x420
        xor    eax, eax
        xor    esi, esi
        xor    edx, edx
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
loc_11_05A4:
        ret    
        db  000h, 000h, 000h
loc_11_05A8:
        enter  0x70, 0
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_11_0615
        call   sub_17_0C04
        jb     loc_11_0615
        call   sub_17_05F8
        or     byte ptr [eax + 3], 1
        mov    dword ptr [ebp - 4], ebx
        lea    edi, [ebp - 0x70]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        mov    esi, 0x62c                    ; -> loc_11_062C
        mov    ecx, 0x417
        movzx  eax, byte ptr [ebp - 0x54]
        or     eax, 0x100000
        mov    edx, ebp
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        jb     loc_11_05F4
        call   sub_17_0C23
        jmp    loc_11_0604
loc_11_05F4:
        cmp    byte ptr [ebp - 0x54], 0
        je     loc_11_0604
        and    word ptr [ebp - 0x54], 0
        and    word ptr [ebp - 0x5c], 0
loc_11_0604:
        call   sub_17_0C31
        lea    esi, [ebp - 0x70]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
loc_11_0612:
        leave  
        clc    
        ret    
loc_11_0615:
        mov    edi, dword ptr [ebp]
        cmp    byte ptr [edi + 0x1c], 0
        je     loc_11_0612
        mov    word ptr [edi + 0x1c], 0
        mov    word ptr [edi + 0x14], 0
        jmp    loc_11_0612
loc_11_062C:
        mov    ecx, dword ptr [ebp + 0x18]
        mov    ebp, edx
        jae    loc_11_063B
        cmp    byte ptr [ebp - 0x54], 0
        je     loc_11_0646
        xor    ecx, ecx
loc_11_063B:
        mov    word ptr [ebp - 0x54], cx
        shr    ecx, 0x10
        mov    word ptr [ebp - 0x5c], cx
loc_11_0646:
        jmp    loc_17_0C3F
loc_11_064B:
        mov    esi, dword ptr [ebp]
        xchg   ebp, esi
        mov    ebp, dword ptr [ebp + 4]
        mov    ebx, dword ptr [ebp - 4]
        lea    ebp, [ebp - 0x70]
        movzx  eax, byte ptr [esi + 0x18]
        cmp    eax, 0xb
        jae    loc_11_066B
        jmp    dword ptr [eax*4 + 0xb7c]     ; -> obj1:0x0B7C
loc_11_066B:
        ret    
loc_11_066C:
        call   sub_11_06C1
        call   sub_11_068A
        movzx  eax, word ptr [ebp + 0x38]
        shl    eax, 4
        movzx  ecx, word ptr [ebp + 0x10]
        add    eax, ecx
        add    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 4], eax
        ret    
sub_11_068A:
        movzx  eax, word ptr [ebp + 0x14]
        mov    dword ptr [esi + 0x14], eax
        ret    
loc_11_0692:
        call   sub_11_068A
        movzx  eax, word ptr [ebp + 0x38]
        mov    edi, eax
        shl    eax, 4
        shl    edi, 0x10
        movzx  ecx, word ptr [ebp + 0x10]
        add    eax, ecx
        add    edi, ecx
        add    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 4], edi
        cmp    word ptr [0xe0], 0            ; -> obj7:0x00E0
        jne    loc_11_06BE
        or     eax, 0xffffffff
loc_11_06BE:
        mov    dword ptr [esi], eax
        ret    
sub_11_06C1:
        movzx  eax, word ptr [ebp + 4]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x18]
        mov    dword ptr [esi + 0x18], eax
        ret    
loc_11_06D0:
        push   ebx
        VMMCall  VMM.Get_Execution_Focus         ; cd2030000100
        mov    edi, ebx
        pop    ebx
        xor    edx, edx
        lea    esi, [edx + 2]
        lea    eax, [edx + 0xf]
        VMMCall  VMM.System_Control              ; cd2093000100
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_11_06ED:
        ret    
loc_11_06EE:
        mov    esi, dword ptr [ebp]
        mov    esi, dword ptr [esi + 4]
        mov    ax, 0x3c10
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        push   0x10
        pop    ecx
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        ret    
loc_11_0706:
        or     byte ptr [ebp + 0x2c], 1
        mov    ebx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_11_0721
        call   sub_17_000F
        mov    dword ptr [ebp + 4], eax
        and    byte ptr [ebp + 0x2c], 0xfe
loc_11_0721:
        ret    
loc_11_0722:
        mov    esi, dword ptr [ebp]
        mov    esi, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp + 0x18], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp + 0x1c], eax
        btr    dword ptr [esi + 0x18], 0
        jae    loc_11_074C
        push   0xfffff000
        pop    eax
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
loc_11_074C:
        ret    
loc_11_074D:
        and    byte ptr [ebp + 0x2c], 0xfe
        mov    edx, ebx
        mov    ebx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_11_07D4
        call   sub_17_000F
        bts    dword ptr [eax], 0x1f
        jb     loc_11_07D4
        xchg   edx, ebx
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        inc    eax
        je     loc_11_07C8
        dec    eax
        push   5
        pop    ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        mov    ebx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    esi, 0x8c5                    ; -> loc_11_08C5
        xor    edx, edx
        VxDCall  VKD.VKD_Start_Paste             ; cd200a000d00
        jb     loc_11_07C2
        mov    esi, 0xa8c                    ; -> loc_1_0A8C
        lodsd  eax, dword ptr [esi]
        mov    ecx, eax
        jecxz  loc_11_07BB
loc_11_07A1:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_11_07B6
        cmp    dword ptr [esi], 0x80000000
        jne    loc_11_07B6
        VxDCall  VKD.VKD_Local_Enable_Hot_Key    ; cd2003000d00
loc_11_07B6:
        add    esi, 0x14
        loop   loc_11_07A1
loc_11_07BB:
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
loc_11_07C1:
        ret    
loc_11_07C2:
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
loc_11_07C8:
        mov    ebx, dword ptr [ebp + 0x10]
        call   sub_17_000F
        btr    dword ptr [eax], 0x1f
loc_11_07D4:
        or     byte ptr [ebp + 0x2c], 1
        jmp    loc_11_07C1
loc_11_07DA:
        mov    eax, dword ptr [0xbb0]        ; -> obj1:0x0BB0
        mov    dword ptr [ebp + 0x1c], eax
        ret    
loc_11_07E3:
        mov    ebx, dword ptr [ebp + 0x10]
        VxDCall  SHELL.SHELL_CheckHotkeyAllowed  ; cd201a001700
        jb     loc_11_0808
        movzx  ecx, word ptr [0x22]          ; -> obj4:0x0022
        add    ecx, dword ptr [4]            ; -> obj4:0x0004
        xor    edi, edi
        mov    eax, 0x40001000
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003801700
loc_11_0808:
        ret    
loc_11_0809:
        xor    esi, esi
        mov    eax, 0x100001
        mov    ecx, 0x415
        VxDCall  SHELL.SHELL_Event               ; cd2002801700
        db  000h
loc_11_081C:
        mov    ebx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_11_0868
        mov    edi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        test   byte ptr [edi + ebx + 0x10], 0x80
        je     loc_11_0868
        or     byte ptr [edi + ebx + 0x10], 2
        mov    edx, ebx
loc_11_083B:
        mov    ebx, edx
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_11_0868
        mov    edi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        test   byte ptr [edi + ebx + 0x10], 2
        je     loc_11_0868
        VMMCall  VMM.Wake_Up_VM                  ; cd2039000100
        mov    eax, 0x3e8
        mov    esi, 0x83b                    ; -> loc_11_083B
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
loc_11_0868:
        ret    
loc_11_0869:
        mov    ebx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_11_0868
sub_11_0874:
        mov    edi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_11_089E
        and    byte ptr [edi + ebx + 0x10], 0xfe
        btr    dword ptr [edi + ebx + 0x10], 1
        jae    loc_11_089E
        mov    ecx, 0x424
        xor    eax, eax
        xor    esi, esi
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
loc_11_089E:
        stc    
        ret    
loc_11_08A0:
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_11_090D
        call   sub_17_000F
        test   byte ptr [eax + 3], 0x80
        je     loc_11_08C0
        VxDCall  VKD.VKD_Cancel_Paste            ; cd200b000d00
        ret    
loc_11_08C0:
        mov    eax, 1
loc_11_08C5:
        push   eax
        call   sub_17_000F
        btr    dword ptr [eax], 0x1f
        mov    esi, 0xa8c                    ; -> loc_1_0A8C
        lodsd  eax, dword ptr [esi]
        mov    ecx, eax
        jecxz  loc_11_08F3
loc_11_08D9:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_11_08EE
        cmp    dword ptr [esi], 0x80000000
        jne    loc_11_08EE
        VxDCall  VKD.VKD_Local_Disable_Hot_Key   ; cd2004000d00
loc_11_08EE:
        add    esi, 0x14
        loop   loc_11_08D9
loc_11_08F3:
        mov    ecx, 0x41c
        pop    eax
        xor    esi, esi
        cmp    eax, 1
        jne    loc_11_0905
        mov    esi, 0xa16                    ; -> loc_17_0A16
loc_11_0905:
        xor    edx, edx
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
loc_11_090D:
        ret    
loc_11_090E:
        VxDCall  VKD.VKD_Cancel_Hot_Key_State    ; cd2006000d00
        xor    esi, esi
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        call   sub_17_002A
        jb     loc_11_093C
        mov    eax, 0x100000
        test   ecx, 1
        setne  al
        mov    ecx, 0x41e
        VxDCall  SHELL.SHELL_Event               ; cd2002801700
loc_11_093C:
        ret    
loc_11_093D:
        push   ebx
        push   ecx
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_11_096D
        call   sub_17_000F
        test   byte ptr [eax], 4
        je     loc_11_0967
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        pop    ecx
        pop    eax
        VxDCall  VKD.VKD_Reflect_Hot_Key         ; cd2005000d00
        jmp    loc_11_0970
loc_11_0967:
        call   dword ptr [0xa88]             ; -> obj1:0x0A88
loc_11_096D:
        add    esp, 8
loc_11_0970:
        ret    
        db  000h, 000h, 000h
_SHELL_SuggestSingleMSDOSMode:
        pushal 
        mov    ebx, dword ptr [esp + 0x24]
        mov    edx, dword ptr [esp + 0x28]
        test   dl, 1
        jne    sub_11_09D3
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_11_09C4
        mov    esi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        test   byte ptr [esi + ebx + 0x11], 1
        jne    loc_11_09C4
        test   dl, 0x20
        je     loc_11_09A8
        VMMCall  VMM.Get_VM_Exec_Time            ; cd2040000100
        mov    dword ptr [esi + ebx + 0x14], eax
        jmp    loc_11_09C4
loc_11_09A8:
        test   dl, 8
        je     loc_11_09B2
        or     dl, 1
        jmp    sub_11_09D3
loc_11_09B2:
        xor    eax, eax
        mov    ecx, 0x6b
        mov    esi, 0x9d3                    ; -> sub_11_09D3
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
loc_11_09C4:
        popal  
        ret    
loc_11_09C6:
        inc    ebx
        dec    edi
        dec    esi
        inc    ecx
        inc    edi
        inc    ebp
        dec    esi
        push   esp
        inc    ebp
        pop    eax
        inc    ebp
        db  000h
sub_11_09D3:
        xor    eax, eax
        test   dl, 1
        jne    loc_11_0A2A
        mov    esi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        mov    esi, dword ptr [esi + ebx + 0xc]
        test   byte ptr [esi + 0x1c4], 4
        jne    loc_11_0B08
        lea    esi, [esi]
        call   sub_17_0F19
        push   edx
        push   esi
        push   0x9c6                         ; -> loc_11_09C6
        VMMCall  VMM._lstrcmpi                   ; cd208e010100
        add    esp, 8
        pop    edx
        test   eax, eax
        je     loc_11_0B08
        VxDCall  SHELL.SHELL_CheckHotkeyAllowed  ; cd201a001700
        jb     loc_11_0B08
        xor    ecx, ecx
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_11_0A2A
        or     dl, 1
loc_11_0A2A:
        mov    edi, eax
        mov    esi, dword ptr [0xa84]        ; -> loc_1_0A84
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_11_0B09
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 0xc], edi
        push   eax
        mov    dword ptr [eax + 8], esp
        pushal 
        xchg   edi, eax
        mov    esi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        mov    esi, dword ptr [esi + ebx + 0xc]
        lea    edx, [edi + 0x114]
        call   sub_17_000F
        lea    ecx, [eax + 0x2e]
        push   0x20
        push   ecx
        push   edx
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        lea    edx, [edi + 0x134]
        lea    ecx, [esi]
        push   0x40
        push   ecx
        push   edx
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        lea    edx, [edi + 0x1b4]
        lea    ecx, [esi + 0x80]
        push   0x40
        push   ecx
        push   edx
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        lea    edx, [edi + 0x10]
        lea    ecx, [esi + 0xc0]
        push   0x104
        push   ecx
        push   edx
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        lea    edi, [edi + 0x174]
        lea    esi, [esi + 0x40]
        push   0x40
        pop    ecx
loc_11_0AC4:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        cmp    al, 0xd
        loopne loc_11_0AC4
        mov    byte ptr [edi - 1], 0
        popal  
        VMMCall  VMM.List_Attach                 ; cd209e000100
        mov    edi, eax
        push   0
        push   0
        push   0xb74                         ; -> loc_11_0B74
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        test   byte ptr [edi], 1
        jne    loc_11_0B1A
        mov    eax, dword ptr [edi + 0xc]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        pop    eax
        cmp    eax, 1
        je     loc_11_0B20
loc_11_0B08:
        ret    
loc_11_0B09:
        test   edi, edi
        je     loc_11_0B15
        mov    eax, edi
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
loc_11_0B15:
        test   dl, 1
        je     loc_11_0B08
loc_11_0B1A:
        VMMCall  VMM.Crash_Cur_VM                ; cd202f000100
loc_11_0B20:
        xor    ecx, ecx
        mov    eax, 0x1388
        VMMCall  VMM.Close_VM                    ; cd20ec000100
        ret    
loc_11_0B2E:
        inc    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [ecx], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x55], dl
        dec    esi
        inc    esp
        dec    esp
        dec    esp
        xor    esi, dword ptr [edx]
        add    byte ptr [ecx + 0x50], al
        push   eax
        push   edi
        dec    ecx
        pop    edx
        inc    ebx
        push   eax
        dec    esp
        sub    al, 0x53
        imul   ebp, dword ptr [esi + 0x67], 0x534d656c
        inc    esp
        dec    edi
        push   ebx
        push   edi
        db  069h, 07Ah, 061h, 072h, 064h, 000h
loc_11_0B74:
        push   0xb2e                         ; -> loc_11_0B2E
        VxDCall  SHELL._SHELL_ShellExecute       ; cd2007001700
        add    esp, 4
        ret    
        ; ---- dispatch table (4 entries) ----
        dd  loc_11_0BA6
        dd  loc_11_0BF6
        dd  loc_11_0BAD
        dd  loc_11_0BDE
loc_11_0B93:
        mov    eax, dword ptr [esi + 0xc]
        cmp    eax, 4
        jae    loc_11_0C30
        jmp    dword ptr [eax*4 + 0xb83]     ; -> obj11:0x0B83
loc_11_0BA6:
        xor    eax, eax
        mov    ecx, eax
        mov    ch, 4
        ret    
loc_11_0BAD:
        mov    ecx, dword ptr [esi + 0x1c]
        cmp    ecx, 0x1f4
        jne    loc_11_0C26
        mov    edi, dword ptr [esi + 0x20]
        mov    dword ptr [edi], ecx
        mov    edi, dword ptr [esi + 0x18]
        mov    esi, dword ptr [0xa84]        ; -> loc_1_0A84
        VMMCall  VMM.List_Remove_First           ; cd20a5000100
        je     loc_11_0C2B
        push   esi
        mov    esi, eax
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        xor    eax, eax
        ret    
loc_11_0BDE:
        cmp    dword ptr [esi + 0x14], 0x1f4
        jne    loc_11_0C26
        mov    eax, dword ptr [esi + 0x10]
        mov    eax, dword ptr [eax + 0xc]
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
        xor    eax, eax
        ret    
loc_11_0BF6:
        cmp    dword ptr [esi + 0x14], 0xc
        jne    loc_11_0C26
        cmp    dword ptr [esi + 0x1c], 4
        jne    loc_11_0C26
        mov    eax, dword ptr [esi + 0x10]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    ecx, dword ptr [eax]
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 8]
        or     eax, 0xc0001000
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        mov    ecx, dword ptr [esi + 0x18]
        mov    dword ptr [ecx], eax
        xor    eax, eax
        ret    
loc_11_0C26:
        xor    eax, eax
        mov    al, 0x57
        ret    
loc_11_0C2B:
        xor    eax, eax
        mov    al, 0xd
        ret    
loc_11_0C30:
        xor    eax, eax
        inc    eax
        ret    
loc_11_0C34:
        push   ebx
        outsd  dx, dword ptr [esi]
        je     0xcb0
        popal  
        jb     loc_11_0CA1
        pop    esp
        dec    ebp
        imul   esp, dword ptr [ebx + 0x72], 0x666f736f
        je     loc_11_0CA3
        push   edi
        imul   ebp, dword ptr [esi + 0x64], 0x5c73776f
        inc    ebx
        jne    0xcc4
        jb     0xcb9
        outsb  dx, byte ptr [esi]
        je     0xcad
        jb     loc_11_0CCD
        imul   ebp, dword ptr [edi + 0x6e], 0x2d534d5c
        inc    esp
        dec    edi
        push   ebx
        and    byte ptr [ebp + 0x6d], al
        jne    sub_11_0CD5
        popal  
        je     sub_11_0CD5
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        pop    esp
        inc    ecx
        jo     0xce2
        inc    ebx
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        jo     0xcd8
        je     sub_11_0C79
sub_11_0C79:
        sub    esp, 0x10
        pushal 
        lea    eax, [0xca9]                  ; -> obj17:0x0CA9
        push   eax
        push   0xc34                         ; -> loc_11_0C34
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        test   eax, eax
        jne    loc_11_0CD0
        xor    ebx, ebx
        lea    edi, [esp + 0x20]
loc_11_0CA1:
        push   0xd
loc_11_0CA3:
        push   edi
        push   ebx
        push   dword ptr [0xca9]             ; -> obj17:0x0CA9
        VMMCall  VMM._RegEnumKey                 ; cd204c010100
        add    esp, 0x10
        test   eax, eax
        jne    loc_11_0CD0
        mov    esi, edi
        call   sub_17_0F2A
        bts    dword ptr [0xcad], ecx        ; -> obj17:0x0CAD
        bts    dword ptr [0xcad], edx        ; -> obj17:0x0CAD
loc_11_0CCD:
        inc    ebx
        jmp    loc_11_0CA1
loc_11_0CD0:
        popal  
        add    esp, 0x10
        ret    
sub_11_0CD5:
        pushal 
        movzx  edx, word ptr [0xc]           ; -> obj4:0x000C
        add    edx, dword ptr [4]            ; -> obj4:0x0004
        mov    ah, 9
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    edx, 0x10
        call   sub_11_09D3
        test   eax, eax
        je     loc_11_0CFC
        stc    
loc_11_0CFC:
        popal  
        ret    
        db  000h, 000h
sub_11_0D00:
        mov    dword ptr [ebp - 0x14], esi
        mov    edi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        xor    eax, eax
        mov    dword ptr [ebp - 0x18], eax
        or     dword ptr [ebp - 8], 0xffffffff
        mov    dword ptr [ebp - 4], ebx
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_11_0DA2
        cmp    dword ptr [0xc0c], 0          ; -> loc_1_0C0C
        je     loc_11_0DA2
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_11_0DA2
        mov    ecx, 1
        call   sub_1_072C
        jb     loc_11_0DA2
        inc    byte ptr [ebp - 0x18]
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    dword ptr [ebp - 0xc], eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, 0x1000
        mov    ecx, 0xf
        mov    edx, ebp
        mov    esi, 0xdaa                    ; -> loc_11_0DAA
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        mov    dword ptr [ebp - 0x10], esi
        mov    ebx, dword ptr [ebp - 4]
        VMMCall  VMM.End_Crit_And_Suspend        ; cd2021000100
        jb     loc_11_0D80
        inc    byte ptr [ebp - 0x17]
        dec    byte ptr [ebp - 0x18]
        dec    byte ptr [ebp - 0x16]
        je     loc_11_0DA2
loc_11_0D80:
        mov    esi, dword ptr [ebp - 0x10]
        VMMCall  VMM.Cancel_Priority_VM_Event    ; cd2015000100
        dec    byte ptr [ebp - 0x17]
        js     loc_11_0D97
        mov    ebx, dword ptr [ebp - 4]
        VMMCall  VMM.Resume_VM                   ; cd202c000100
loc_11_0D97:
        dec    byte ptr [ebp - 0x18]
        js     loc_11_0DA2
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
loc_11_0DA2:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, -1
        cmc    
        ret    
loc_11_0DAA:
        mov    ebp, edx
        and    dword ptr [ebp - 0x10], 0
        mov    ebx, dword ptr [ebp - 4]
        test   byte ptr [ebx], 8
        je     loc_11_0DD6
        jmp    dword ptr [ebp - 0x14]
loc_11_0DBB:
        mov    dword ptr [ebp - 8], eax
        inc    byte ptr [ebp - 0x16]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, dword ptr [ebp - 0xc]
        VMMCall  VMM.Adjust_Execution_Time       ; cd2037000100
loc_11_0DD6:
        dec    byte ptr [ebp - 0x17]
        mov    ebx, dword ptr [ebp - 4]
        VMMCall  VMM.Resume_VM                   ; cd202c000100
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, 0xfffff000
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e800100
        add    byte ptr [ebx + 0xbe035], cl  ; -> loc_1_0BE0
        add    byte ptr [edx - 0x5edf3206], dl
        add    byte ptr [ecx], al
        add    bl, bh
        jb     loc_11_0E16
        push   2
        push   dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        call   ecx
        pop    eax
        pop    eax
loc_11_0E16:
        ret    
_SHELL_PostShellMessage:
        mov    ecx, dword ptr [0xbb0]        ; -> obj1:0x0BB0
        jecxz  loc_11_0E3A
        push   0
        push   0
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   0x506
        push   ecx
        VxDCall  SHELL._SHELL_PostMessage        ; cd2006001700
        add    esp, 0x18
loc_11_0E3A:
        ret    
loc_11_0E3B:
        VMMCall  VMM._GetAppFlatDSAlias          ; cd206d000100
        mov    word ptr [ebp + 0x38], ax
        mov    dword ptr [0xbd8], edi        ; -> loc_1_0BD8
        mov    ax, 0x2810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    edi, 0xbec                    ; -> loc_1_0BEC
        mov    ecx, 0xd
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movzx  eax, word ptr [ebp + 0x1c]
        mov    dword ptr [0xbb0], eax        ; -> obj1:0x0BB0
        mov    ax, word ptr [ebp + 0x3c]
        inc    ecx
        mov    word ptr [0xbdc], ax          ; -> loc_1_0BDC
        mov    esi, dword ptr [0xbe0]        ; -> loc_1_0BE0
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_11_0EA6
        push   ecx
        xor    eax, eax
        or     byte ptr [0xbe8], 1           ; -> loc_1_0BE8
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    ecx, 0xa
        mov    esi, 0x3a                     ; -> loc_16_003A
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    ecx
loc_11_0EA6:
        push   1
        mov    eax, esp
        push   0
        push   ecx
        push   0x219
        push   eax
        push   0
        VxDCall  SHELL._SHELL_BroadcastSystemMessage ; cd2010001700
        add    esp, 0x14
        pop    eax
        ret    
loc_11_0EC0:
        xor    ecx, ecx
        jmp    loc_11_0EA6
_SHELL_HookSystemBroadcast:
        push   esi
        mov    esi, dword ptr [0xbe4]        ; -> loc_1_0BE4
        or     esi, esi
        jne    loc_11_0ED4
        call   sub_5_06C0
loc_11_0ED4:
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_11_0F1D
        push   1
        mov    ecx, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 0x10]
        pop    dword ptr [eax + 0xc]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], edx
        mov    edx, dword ptr [esp + 0x10]
        cmp    edx, 1
        jae    loc_11_0EF9
        rcr    edx, 1
loc_11_0EF9:
        mov    dword ptr [eax + 8], edx
        xor    ecx, ecx
        push   eax
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        jmp    loc_11_0F0F
loc_11_0F07:
        mov    ecx, eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
loc_11_0F0F:
        je     loc_11_0F16
        cmp    dword ptr [eax + 8], edx
        jb     loc_11_0F07
loc_11_0F16:
        pop    eax
        VMMCall  VMM.List_Insert                 ; cd20a0000100
loc_11_0F1D:
        pop    esi
        ret    
_SHELL_UnhookSystemBroadcast:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    esi, dword ptr [0xbe4]        ; -> loc_1_0BE4
        jecxz  loc_11_0F38
        mov    dword ptr [ecx], 0xf3a        ; -> loc_11_0F3A
        xchg   ecx, eax
        call   sub_16_0028
loc_11_0F38:
        pop    esi
        ret    
loc_11_0F3A:
        xor    eax, eax
        inc    eax
        ret    
sub_11_0F3E:
        sub    esp, 0x10
        mov    word ptr [esp], ax
        or     dword ptr [esp + 0xc], 0xffffffff
        and    word ptr [esp + 0xa], 0
        or     edi, edi
        jne    loc_11_0F5C
        call   sub_17_000F
        lea    edi, [eax + 0x2e]
loc_11_0F5C:
        push   ecx
        push   0
        push   0x30000
        VxDCall  SHELL._SHELL_LocalAllocEx       ; cd2013001700
        add    esp, 0xc
        mov    dword ptr [esp + 6], eax
        or     eax, eax
        je     loc_11_0FC6
        push   edi
        push   0
        push   0x30000
        VxDCall  SHELL._SHELL_LocalAllocEx       ; cd2013001700
        add    esp, 0xc
        mov    dword ptr [esp + 2], eax
        or     eax, eax
        je     loc_11_0FB9
        lea    eax, [esp]
        push   eax
        push   0xc
        push   dword ptr [0xc08]             ; -> loc_1_0C08
        push   0
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
        movzx  eax, ax
        mov    dword ptr [esp + 0xc], eax
        push   dword ptr [esp + 2]
        VxDCall  SHELL._SHELL_LocalFree          ; cd2014001700
        add    esp, 4
loc_11_0FB9:
        push   dword ptr [esp + 6]
        VxDCall  SHELL._SHELL_LocalFree          ; cd2014001700
        add    esp, 4
loc_11_0FC6:
        mov    eax, dword ptr [esp + 0xc]
        add    esp, 0x10
        ret    
        db  000h, 000h
loc_11_0FD0:
        push   ebx
        outsd  dx, dword ptr [esi]
        je     0x104c
        popal  
        jb     loc_11_103D
        pop    esp
        dec    ebp
        imul   esp, dword ptr [ebx + 0x72], 0x666f736f
        je     loc_11_103F
        push   edi
        imul   ebp, dword ptr [esi + 0x64], 0x5c73776f
        inc    ebx
        jne    0x1060
        jb     0x1055
        outsb  dx, byte ptr [esi]
        je     loc_11_1049
        jb     0x1069
        imul   ebp, dword ptr [edi + 0x6e], 0x74654e5c
        ja     0x106e
        jb     0x106c
        pop    esp
        push   edx
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + 0x6f], cl
        and    byte ptr gs:[esi + 0x65], cl
        je     loc_11_1010
loc_11_1010:
        push   ebx
        je     0x1089
        jo     loc_11_1064
        db  000h
loc_11_1017:
        push   ebx
        je     0x1090
        jo     loc_11_106B
        push   eax
        popal  
        je     0x1089
        add    byte ptr [ebx], dh
        shl    byte ptr [ebx + 0xc20], 0x66  ; -> loc_1_0C20
        dec    eax
        xor    ecx, ecx
        push   ecx
        push   ecx
        push   ecx
        push   ecx
        push   0x1e
        push   eax
        VxDCall  SHELL._SHELL_PostMessage        ; cd2006001700
        add    esp, 0x18
loc_11_103D:
        ret    
        db  066h
loc_11_103F:
        cmp    dword ptr [ebp + 0x18], 0x24732400
        xor    esi, esi
        db  087h
loc_11_1049:
        xor    eax, 0xc20                    ; -> loc_1_0C20
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    eax, 0x7d0
        mov    esi, 0x1022                   ; -> obj11:0x1022
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
loc_11_1064:
        mov    dword ptr [0xc20], esi        ; -> loc_1_0C20
        stc    
loc_11_106B:
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 12  vsize=0x184  R-X
sub_12_0000:
        movzx  eax, word ptr [ebp + 0x14]
        cmp    eax, 0x13
        je     loc_12_0016
        cmp    eax, 0x26
        je     loc_12_0016
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_12_0022
loc_12_0016:
        cmp    eax, 0x28
        jae    loc_12_0022
        jmp    dword ptr [eax*4 + 0x38]      ; -> obj7:0x0038
loc_12_0022:
        mov    dword ptr [ebp + 0x1c], 0xffffffff
        or     byte ptr [ebp + 0x2c], 1
        ret    
        db  000h, 000h
sub_12_0030:
        mov    ebx, 0x81810000
        shld   edx, eax, 0x10
        sub    al, 0x60
        cmp    al, 9
        ja     loc_12_0045
        or     ebx, 0x200020
loc_12_0045:
        xchg   ah, al
        xor    dh, dh
        test   al, 8
        je     loc_12_004F
        inc    dh
loc_12_004F:
        test   al, 1
        je     loc_12_0056
        or     bl, 1
loc_12_0056:
        test   al, 4
        je     loc_12_005D
        or     bh, 1
loc_12_005D:
        test   al, 2
        je     loc_12_0064
        or     bl, 0x80
loc_12_0064:
        test   al, 0x10
        je     loc_12_006B
        or     bh, 0x80
loc_12_006B:
        xchg   edx, eax
        ret    
sub_12_006D:
        pushal 
        mov    esi, dword ptr [0xbbc]        ; -> loc_1_0BBC
        xchg   edx, eax
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_12_008A
loc_12_007D:
        cmp    word ptr [eax], dx
        je     loc_12_00E5
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_12_007D
loc_12_008A:
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_12_00E5
        xchg   edi, eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    word ptr [edi], ax
        call   sub_12_0030
        mov    cl, 0x91
        mov    edx, edi
        mov    esi, 0x891                    ; -> loc_17_0891
        mov    edi, 0xfa0
        VxDCall  VKD.VKD_Define_Hot_Key          ; cd2001000d00
        mov    esi, dword ptr [0xbbc]        ; -> loc_1_0BBC
        jae    loc_12_00C5
        xchg   edx, eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        stc    
        jmp    loc_12_00E5
loc_12_00C5:
        mov    dword ptr [edx + 8], eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VKD.VKD_Local_Disable_Hot_Key   ; cd2004000d00
        xor    eax, eax
        mov    dword ptr [edx + 4], eax
        mov    word ptr [edx + 2], ax
        xchg   edx, eax
        VMMCall  VMM.List_Attach                 ; cd209e000100
        clc    
loc_12_00E5:
        mov    dword ptr [esp + 0x14], eax
        popal  
        ret    
loc_12_00EB:
        mov    esi, dword ptr [0xbbc]        ; -> loc_1_0BBC
        jmp    loc_12_0103
loc_12_00F3:
        mov    edx, dword ptr [eax + 8]
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        xchg   edx, eax
        VxDCall  VKD.VKD_Remove_Hot_Key          ; cd2002000d00
loc_12_0103:
        VMMCall  VMM.List_Remove_First           ; cd20a5000100
        jne    loc_12_00F3
        movzx  ecx, word ptr [ebp + 0x18]
        mov    ax, 0x3c1c
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        jecxz  loc_12_0138
loc_12_011C:
        mov    eax, dword ptr [esi + 2]
        call   sub_12_006D
        jb     loc_12_0182
        cmp    word ptr [edx + 2], 0
        jne    loc_12_0133
        mov    eax, dword ptr [esi]
        mov    word ptr [edx + 2], ax
loc_12_0133:
        add    esi, 6
        loop   loc_12_011C
loc_12_0138:
        movzx  ecx, word ptr [ebp + 0x10]
        mov    ax, 0x3cff
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
loc_12_0147:
        jecxz  loc_12_0182
        mov    eax, dword ptr [ecx + esi + 0xc]
        mov    dl, byte ptr [ecx + esi + 0xa]
        mov    ah, dl
        and    ah, 2
        test   dl, 1
        je     loc_12_015E
        or     ah, 4
loc_12_015E:
        test   dl, 4
        je     loc_12_0166
        or     ah, 1
loc_12_0166:
        test   dl, 8
        je     loc_12_016E
        or     ah, 0x10
loc_12_016E:
        call   sub_12_006D
        jb     loc_12_0182
        mov    eax, dword ptr [ecx + esi + 0xa]
        mov    dword ptr [edx + 4], eax
        mov    cx, word ptr [ecx + esi]
        jmp    loc_12_0147
loc_12_0182:
        ret    
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 13  vsize=0x64c  R-X
SHELL_GetVMInfo:
        push   ebx
        add    ebx, dword ptr [0xa00]        ; -> obj1:0x0A00
        mov    eax, dword ptr [ebx + 0xc]
        and    eax, 0x4007ffe4
        or     edx, 0xffffffff
        mov    edi, edx
        mov    dx, word ptr [ebx + 0x36]
        mov    esi, edi
        pop    ebx
        ret    
loc_13_001C:
        sub    eax, eax
        xchg   dword ptr [ebp + 0x18], eax
        and    byte ptr [ebp + 0x2c], 0xfe
        mov    esi, dword ptr [0x20]         ; -> obj7:0x0020
        or     esi, esi
        je     loc_13_0084
        xchg   edi, eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        or     eax, eax
        jne    loc_13_0049
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_13_0084
        jmp    loc_13_005A
loc_13_0049:
        mov    edx, dword ptr [eax]
loc_13_004B:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_13_0084
        call   sub_13_00DD
        je     loc_13_004B
loc_13_005A:
        xchg   ebx, eax
        mov    dword ptr [ebp + 0x18], ebx
        push   edi
        lea    edi, [edi + 0x14]
        sub    eax, eax
        mov    byte ptr [edi], al
        mov    esi, dword ptr [ebx + 4]
        or     esi, esi
        je     loc_13_0073
loc_13_006D:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        or     al, al
        jne    loc_13_006D
loc_13_0073:
        pop    edi
        mov    esi, dword ptr [ebx]
        mov    dword ptr [edi], esi
        cmp    esi, 0xb
        jb     loc_13_0084
        stosd  dword ptr es:[edi], eax
loc_13_007E:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        or     al, al
        jne    loc_13_007E
loc_13_0084:
        ret    
loc_13_0085:
        mov    esi, dword ptr [0x20]         ; -> obj7:0x0020
        or     esi, esi
        je     loc_13_00D8
        xchg   edi, eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_13_00D8
loc_13_00A3:
        mov    edx, dword ptr [edi]
        and    edx, 0xff
        jne    loc_13_00B0
        lea    edx, [edi + 4]
loc_13_00B0:
        call   sub_13_00DD
        jne    loc_13_00CB
        mov    ebx, dword ptr [ebp + 0x10]
        xchg   edx, eax
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edx, eax
        push   eax
        call   dword ptr [eax + 8]
        pop    eax
loc_13_00CB:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_13_00A3
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_13_00D8:
        or     byte ptr [ebp + 0x2c], 1
        ret    
sub_13_00DD:
        or     edx, edx
        je     loc_13_011F
        cmp    dword ptr [eax], edx
        je     loc_13_011F
        cmp    edx, 0xb
        jb     loc_13_011F
        cmp    dword ptr [eax], 0xb
        jb     loc_13_011F
        push   ebx
        push   ecx
        push   esi
        push   edi
        mov    ebx, eax
        sub    eax, eax
        mov    ecx, 0x10
        mov    edi, edx
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ecx, 0x10
        cmp    ecx, 0x10
        jb     loc_13_010F
        mov    ecx, 0xf
loc_13_010F:
        mov    edi, edx
        mov    esi, dword ptr [ebx]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_13_0119
        cmp    byte ptr [esi], al
loc_13_0119:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ecx
        pop    ebx
loc_13_011F:
        ret    
loc_13_0120:
        mov    edi, dword ptr [0xa00]        ; -> obj1:0x0A00
        push   edi
        lea    edi, [edi + ebx + 0xc]
        push   edi
        mov    ecx, 0x5e
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        VMMCall  VMM.Get_Time_Slice_Priority     ; cd2032000100
        pop    esi
        pop    edi
        mov    ecx, dword ptr [esi]
        xor    ecx, eax
        and    ecx, 3
        xor    eax, ecx
        xor    ecx, ecx
        inc    ecx
        mov    edx, ecx
        VMMCall  VMM.Set_Time_Slice_Priority     ; cd2033000100
        jb     loc_13_0232
        push   esi
        movzx  edi, word ptr [esi + 0x1c]
        movzx  esi, word ptr [esi + 0x1a]
        cmp    di, 0x280
        jne    loc_13_016A
        or     di, 0xffff
loc_13_016A:
        cmp    si, 0x280
        jne    loc_13_0175
        or     si, 0xffff
loc_13_0175:
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        shl    eax, 2
        mov    ecx, eax
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        shl    eax, 2
        cmp    si, -1
        jne    loc_13_0194
        mov    esi, ecx
        jmp    loc_13_0196
loc_13_0194:
        add    esi, eax
loc_13_0196:
        cmp    di, -1
        jne    loc_13_01A0
        mov    edi, ecx
        jmp    loc_13_01A2
loc_13_01A0:
        add    edi, eax
loc_13_01A2:
        xor    ecx, ecx
        pop    eax
        push   eax
        test   byte ptr [eax + 2], 4
        je     loc_13_01AF
        or     ecx, 1
loc_13_01AF:
        VxDCall  V86MMGR.V86MMGR_Allocate_V86_Pages ; cd2001000600
        pop    esi
        jb     loc_13_0231
        mov    dword ptr [esi + 0x1a], eax
        mov    ecx, 1
        test   byte ptr [esi + 1], 0x20
        jne    loc_13_01CB
        mov    ecx, 2
loc_13_01CB:
        test   byte ptr [esi + 1], 0x80
        je     loc_13_01D4
        or     ecx, 8
loc_13_01D4:
        test   byte ptr [esi + 2], 1
        je     loc_13_01DD
        or     ecx, 4
loc_13_01DD:
        movzx  eax, word ptr [esi + 0x24]
        movzx  edx, word ptr [esi + 0x22]
        cmp    dx, -1
        jne    loc_13_01F0
        mov    edx, 0xffffffff
loc_13_01F0:
        movzx  edi, word ptr [esi + 0x26]
        cmp    di, -1
        jne    loc_13_01FF
        mov    edi, 0xffffffff
loc_13_01FF:
        push   esi
        movzx  esi, word ptr [esi + 0x28]
        VxDCall  V86MMGR.V86MMGR_Set_EMS_XMS_Limits ; cd2002000600
        pop    esi
        jb     loc_13_0231
        push   dword ptr [esi + 0x14]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 0xe]
        push   ebx
        VxDCall  DOSMGR._DOSMGR_Set_Exec_VM_Data ; cd2001001500
        add    esp, 0x10
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    esi, ebx
        pop    ebx
        VxDCall  DOSMGR.DOSMGR_Copy_VM_Drive_State ; cd2002001500
        clc    
loc_13_0231:
        ret    
loc_13_0232:
        mov    eax, 0x20008
        lea    edx, [0x9bc]                  ; -> obj1:0x09BC
        xor    ebx, ebx
        xor    ecx, ecx
        inc    ecx
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
        stc    
        ret    
loc_13_024A:
        mov    ecx, dword ptr [0xa00]        ; -> obj1:0x0A00
        mov    ax, word ptr [edx + 0xa]
        mov    word ptr [ecx + ebx + 2], ax
        ret    
loc_13_025A:
        mov    esi, dword ptr [0xa00]        ; -> obj1:0x0A00
        lea    esi, [esi + ebx + 0xc]
        mov    eax, dword ptr [esi]
        and    eax, 8
        VxDCall  VDD.VDD_Set_VMType              ; cd2004000a00
        mov    eax, dword ptr [esi + 4]
        and    eax, 0xffff
        shr    eax, 0
        VxDCall  VDD.VDD_PIF_State               ; cd2001000a00
        VxDCall  VMM_STRING.Service_0000h        ; cd2000001800
        jb     loc_13_0295
        test   byte ptr [esi + 1], 0x10
        setne  al
        VxDCall  VMM_STRING.Service_0001h        ; cd2001001800
loc_13_0295:
        test   byte ptr [esi + 2], 2
        sete   al
        VxDCall  VKD.VKD_Define_Paste_Mode       ; cd2009000d00
        push   ebx
        push   esi
        movzx  ecx, word ptr [esi - 0xa]
        call   sub_13_02C5
        pop    esi
        pop    ebx
        push   0
        push   0x17b                         ; -> loc_14_017B
        push   dword ptr [esi + 0x1a]
        push   ebx
        VxDCall  DOSMGR._DOSMGR_Exec_VM          ; cd2003001500
        add    esp, 0x10
        clc    
        ret    
sub_13_02C5:
        movzx  edx, byte ptr [0x484]
        inc    edx
        cmp    edx, ecx
        je     loc_13_0322
        mov    ax, 0x1202
        cmp    ecx, 0x19
        je     loc_13_02E5
        cmp    ecx, 0x32
        je     loc_13_02E5
        cmp    ecx, 0x2b
        jne    loc_13_0322
        dec    eax
loc_13_02E5:
        mov    bl, 0x30
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        cmp    al, 0x12
        jne    loc_13_0322
        xor    eax, eax
        mov    al, 3
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        cmp    ecx, 0x19
        je     loc_13_0322
        mov    ax, 0x1112
        xor    ebx, ebx
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    ax, 0x100
        mov    cx, 0x607
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_13_0322:
        mov    ah, 0x40
        xor    ebx, ebx
        inc    ebx
        mov    edx, 0x34                     ; -> obj7:0x0034
        lea    ecx, [ebx + 1]
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    ah, 8
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        xchg   ebx, eax
        mov    dl, byte ptr [0x44a]
        dec    edx
        mov    dh, byte ptr [0x484]
        mov    ax, 0x600
        xor    ecx, ecx
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        ret    
        db  000h, 000h
loc_13_0360:
        mov    ecx, dword ptr [0xa6c]        ; -> loc_1_0A6C
        VMMCall  VMM.Get_Last_Updated_VM_Exec_Time ; cd20d0000100
        test   eax, eax
        jne    loc_13_037B
        test   byte ptr [edx + 2], 4
        jne    loc_13_037B
        or     byte ptr [ecx + ebx + 0x10], 8
loc_13_037B:
        and    byte ptr [ecx + ebx + 0x11], 0xfe
        test   byte ptr [edx + 1], 4
        je     loc_13_038B
        or     byte ptr [ecx + ebx + 0x11], 1
loc_13_038B:
        ret    
loc_13_038C:
        call   sub_17_000F
        movzx  ecx, word ptr [edx]
        xor    ecx, dword ptr [eax]
        and    cx, 0xfe0
        xor    word ptr [eax], cx
        jmp    sub_13_045C
loc_13_03A3:
        call   sub_17_000F
        xchg   edi, eax
        mov    edi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        push   0x201
        push   0x1c8
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        test   eax, eax
        je     loc_13_03F1
        mov    dword ptr [edi + ebx + 0xc], eax
        xor    eax, eax
        mov    al, 0xe
        push   0x54
        pop    ecx
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_13_03F1
        mov    dword ptr [edi + ebx + 4], esi
        mov    edi, esp
        call   sub_17_0608
        jb     loc_13_03F1
        mov    byte ptr [eax + 2], 1
        call   sub_17_0740
loc_13_03EF:
        clc    
        ret    
loc_13_03F1:
        call   sub_14_02CC
        stc    
        ret    
loc_13_03F8:
        call   sub_17_000F
        test   byte ptr [eax], 2
        je     loc_13_0426
        push   ebx
        VMMCall  VMM.Get_Execution_Focus         ; cd2030000100
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_13_0425
        push   5
        pop    ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        call   sub_17_035F
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
loc_13_0425:
        pop    ebx
loc_13_0426:
        xor    eax, eax
        lea    ecx, [eax + 3]
        mov    esi, 0x438                    ; -> loc_13_0438
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        clc    
        ret    
loc_13_0438:
        mov    ecx, 0x426
        mov    eax, 0x200000
        mov    esi, 0x3ef                    ; -> loc_13_03EF
        VxDCall  SHELL.SHELL_Event               ; cd2002801700
loc_13_044D:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_13_045A
        call   sub_13_045C
loc_13_045A:
        clc    
        ret    
sub_13_045C:
        push   esi
        call   sub_17_000F
        mov    edx, dword ptr [eax]
        xor    edx, 0x80000000
        and    edx, 0xfffffffe
        mov    esi, 0xa8c                    ; -> loc_1_0A8C
        lodsd  eax, dword ptr [esi]
        xchg   ecx, eax
loc_13_0474:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_13_048D
        test   dword ptr [esi], edx
        je     loc_13_0487
        VxDCall  VKD.VKD_Local_Disable_Hot_Key   ; cd2004000d00
        jmp    loc_13_048D
loc_13_0487:
        VxDCall  VKD.VKD_Local_Enable_Hot_Key    ; cd2003000d00
loc_13_048D:
        add    esi, 0x14
        loop   loc_13_0474
        pop    esi
        ret    
loc_13_0494:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x1c], 0x400
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_13_04A9:
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        inc    esi
        je     loc_13_05BE
        dec    esi
        xor    edi, edi
        movzx  eax, word ptr [esi + 0xc]
        push   edi
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        add    dword ptr [esi + 8], eax
        movzx  eax, word ptr [esi + 0x12]
        push   edi
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        add    dword ptr [esi + 0xe], eax
        movzx  eax, word ptr [esi + 0x18]
        push   edi
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        add    dword ptr [esi + 0x14], eax
        mov    ax, 0x3cff
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    word ptr [eax + 0x10], 0x5043
        je     loc_13_050D
        and    dword ptr [esi + 0x2a], 0
loc_13_050D:
        mov    eax, dword ptr [esi + 4]
        and    eax, 0xffff
        mov    ecx, eax
        xor    ecx, 0x1e
        and    ecx, 0x7e
        xor    eax, ecx
        VxDCall  VDD.VDD_PIF_State               ; cd2001000a00
        mov    edi, ebx
        xor    eax, eax
        mov    al, 7
        VMMCall  VMM.System_Control              ; cd2093000100
        jb     loc_13_05B0
        push   esi
        push   ebx
        VMMCall  VMM.Get_Initial_Thread_Handle   ; cd200d010100
        VMMCall  VMM.Get_Thread_Win32_Pri        ; cd2066010100
        mov    ebx, edi
        mov    eax, ecx
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        xor    edx, edx
        VMMCall  VMM.Set_Inversion_Pri           ; cd2063010100
        pop    ebx
        pop    esi
        call   sub_17_000F
        add    edi, dword ptr [0xa04]        ; -> obj1:0x0A04
        mov    dword ptr [eax - 6], edi
        mov    dword ptr [edi], ebx
        mov    dword ptr [ebp + 0x1c], ebx
        cmp    dword ptr [eax + 0x2a], 0
        je     loc_13_05AB
        mov    esi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   0x1c8
        push   eax
        push   dword ptr [esi + ebx + 0xc]
        VMMCall  VMM._lmemcpy                    ; cd207d010100
        add    esp, 0xc
        cmp    byte ptr [eax + 0xc0], 1
        jb     loc_13_05AB
        jne    loc_13_05A2
        dec    byte ptr [eax + 0xc0]
loc_13_05A2:
        inc    byte ptr [esi + ebx + 0x18]
        or     byte ptr [esi + ebx + 0x11], 2
loc_13_05AB:
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_13_05B0:
        xor    ecx, ecx
        xor    ebx, ebx
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
        test   eax, eax
        jne    loc_13_05C5
loc_13_05BE:
        xor    edx, edx
        mov    eax, 0x20000
loc_13_05C5:
        mov    dword ptr [ebp + 0x1c], eax
        mov    dword ptr [ebp + 0x14], edx
        or     byte ptr [ebp + 0x2c], 1
        ret    
loc_13_05D0:
        or     byte ptr [ebp + 0x2c], 1
        cmp    byte ptr [0xe4], 0xff         ; -> obj7:0x00E4
        je     loc_13_05E9
        call   sub_11_0C79
        mov    byte ptr [0xe4], 0xff         ; -> obj7:0x00E4
loc_13_05E9:
        movzx  eax, word ptr [0xe0]          ; -> obj7:0x00E0
        or     eax, eax
        jne    loc_13_062D
        VMMCall  VMM._GetAppFlatDSAlias          ; cd206d000100
        mov    esi, eax
        push   0
        push   0
        push   eax
        VMMCall  VMM._GetDescriptor              ; cd207b000100
        add    esp, 0xc
        mov    ecx, edx
        or     ecx, eax
        je     loc_13_0645
        or     edx, 0x200
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        or     eax, eax
        je     loc_13_0645
        mov    word ptr [0xe0], ax           ; -> obj7:0x00E0
loc_13_062D:
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [0xd8]         ; -> obj7:0x00D8
        mov    dword ptr [ebp + 0x10], eax
        mov    eax, dword ptr [0xdc]         ; -> obj7:0x00DC
        mov    dword ptr [ebp + 0x18], eax
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_13_0645:
        mov    eax, esi
        jmp    loc_13_062D
        db  000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 14  vsize=0x698  R-X
        db  060h, 08Bh, 00Dh
        dd  obj1:0x0A00              ; fixup
        db  0F6h, 004h, 019h, 008h, 075h, 015h, 0F6h, 044h, 019h, 00Eh, 008h, 00Fh, 084h, 095h, 000h, 000h
        db  000h, 0F6h, 004h, 019h, 010h, 00Fh, 084h, 08Bh, 000h, 000h, 000h, 080h, 04Ch, 019h, 00Eh, 008h
        db  068h, 002h, 000h, 008h, 000h, 068h
        dd  loc_4_0000               ; fixup
        db  0CDh, 020h, 059h, 001h, 001h, 000h, 092h, 0B4h, 009h, 06Ah, 021h, 0CDh, 020h, 08Fh, 000h, 001h
        db  000h, 003h, 024h, 024h, 083h, 0ECh, 06Ch, 08Bh, 0FCh, 0CDh, 020h, 08Dh, 000h, 001h, 000h, 0CDh
        db  020h, 083h, 000h, 001h, 000h, 0CDh, 020h, 01Ah, 000h, 001h, 000h, 0C6h, 045h, 01Dh, 001h, 0B8h
        db  016h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 0F7h, 045h, 02Ch, 040h, 000h, 000h
        db  000h, 074h, 013h, 0B8h, 028h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 0CDh, 020h
        db  038h, 000h, 001h, 000h, 0EBh, 0D5h, 033h, 0C0h, 089h, 045h, 01Ch, 0B0h, 016h, 0CDh, 020h, 084h
        db  000h, 001h, 000h, 080h, 07Dh, 01Ch, 003h, 075h, 0C2h, 0CDh, 020h, 086h, 000h, 001h, 000h, 08Bh
        db  0F4h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 083h, 0C4h, 06Ch, 0EBh, 016h, 0F6h, 004h, 019h, 020h
        db  074h, 00Bh, 0F6h, 044h, 019h, 00Fh, 040h, 00Fh, 084h, 064h, 0FFh, 0FFh, 0FFh, 080h, 064h, 019h
        db  00Eh, 0F7h, 061h, 0C3h, 000h, 000h, 000h
loc_14_00C8:
        push   edx
        push   ebx
        mov    ebp, esp
        mov    esi, dword ptr [0xa00]        ; -> obj1:0x0A00
        test   dl, 1
        je     loc_14_00DC
        mov    byte ptr [esi + ebx + 0xb], 0x83
loc_14_00DC:
        VMMCall  VMM.Get_Execution_Focus         ; cd2030000100
        cmp    dword ptr [ebp], ebx
        jne    loc_14_00F9
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xor    eax, eax
        inc    eax
        push   eax
        push   ebx
        call   sub_17_01A7
        pop    ecx
        pop    ecx
loc_14_00F9:
        mov    ebx, dword ptr [ebp]
        xor    eax, eax
        xor    edx, edx
        test   byte ptr [ebp + 4], 4
        jne    loc_14_013A
        test   byte ptr [ebp + 4], 0x18
        je     loc_14_011D
        xor    ecx, ecx
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
        jne    loc_14_013A
        mov    eax, 8
        jmp    loc_14_0138
loc_14_011D:
        test   byte ptr [esi + ebx], 2
        jne    loc_14_0133
        test   byte ptr [ebp + 4], 1
        je     loc_14_0138
        xor    ecx, ecx
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
        jmp    loc_14_013A
loc_14_0133:
        movzx  eax, byte ptr [esi + ebx + 0xa]
loc_14_0138:
        xor    edx, edx
loc_14_013A:
        mov    ecx, dword ptr [ebp + 4]
        movzx  esi, byte ptr [esi + ebx + 0xb]
        push   esi
        push   edx
        push   eax
        push   ecx
        push   ebx
        call   sub_14_0349
        add    esp, 0x14
        inc    eax
        jne    loc_14_0160
        mov    edx, dword ptr [ebp]
        mov    esi, 0x165                    ; -> loc_14_0165
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
loc_14_0160:
        clc    
        add    esp, 8
        ret    
loc_14_0165:
        mov    ebx, edx
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_14_017A
        mov    eax, 0xc
        VMMCall  VMM.System_Control              ; cd2093000100
loc_14_017A:
        ret    
loc_14_017B:
        mov    esi, dword ptr [0xa00]        ; -> obj1:0x0A00
        test   byte ptr [ebp + 0x2c], 1
        jne    loc_14_0232
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_14_01E5
        sub    esp, 0x6c
        mov    edi, esp
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0x4d00
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  eax, word ptr [ebp + 0x1c]
        mov    byte ptr [esi + ebx + 0xb], al
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        test   byte ptr [0xa1b], 1           ; -> obj1:0x0A1B
        je     loc_14_01E0
        call   0
loc_14_01E0:
        and    byte ptr [esi + ebx], 0xfd
loc_14_01E4:
        ret    
loc_14_01E5:
        sub    esp, 0x6c
        mov    edi, esp
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0x4d00
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  eax, word ptr [ebp + 0x1c]
        or     ah, ah
        je     loc_14_0211
        mov    al, 0xff
loc_14_0211:
        mov    byte ptr [esi + ebx + 0xb], al
        VMMCall  VMM.Set_System_Exit_Code        ; cd20bd000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        jmp    loc_14_01E0
loc_14_0232:
        mov    byte ptr [esi + ebx + 0xb], 0x85
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_14_02BA
        cmp    word ptr [ebp + 0x1c], 8
        je     loc_11_0235
loc_14_024A:
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        mov    word ptr [ebp + 0x1c], 0x5900
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  eax, word ptr [ebp + 0x1c]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    esi, 0x4005
        cmp    eax, 2
        je     loc_14_029D
        inc    esi
        cmp    eax, 3
        je     loc_14_029D
        inc    esi
        cmp    eax, 4
        je     loc_14_029D
        inc    esi
        cmp    eax, 5
        je     loc_14_029D
        inc    esi
        cmp    eax, 0xb
        je     loc_14_029D
        inc    esi
        cmp    eax, 0x20
        je     loc_14_029D
        inc    esi
loc_14_029D:
        add    esi, 0x7c000
        push   esi
        push   0                             ; -> loc_4_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        pushal 
        mov    esi, eax
        xor    eax, eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
loc_14_02BA:
        movzx  eax, word ptr [ebp + 0x1c]
        mov    byte ptr [esi + ebx + 0xa], al
        or     byte ptr [esi + ebx], 2
        jmp    loc_14_01E4
        db  000h
sub_14_02CC:
        mov    edi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        xor    esi, esi
        xchg   dword ptr [edi + ebx + 8], esi
        test   esi, esi
        je     loc_14_02F9
        push   dword ptr [esi + 0xc]
        VxDCall  SHELL._SHELL_CancelAppyTimeEvent ; cd200f001700
        add    esp, 4
        mov    ecx, dword ptr [esi + 4]
        jecxz  loc_14_02F9
        push   0
        push   ecx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_14_02F9:
        xor    esi, esi
        xchg   dword ptr [edi + ebx + 4], esi
        or     esi, esi
        je     loc_14_0309
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_14_0309:
        xor    esi, esi
        xchg   dword ptr [edi + ebx + 0xc], esi
        or     esi, esi
        je     loc_14_031F
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_14_031F:
        call   sub_17_000F
        xchg   esi, eax
        xor    eax, eax
        xchg   word ptr [esi + 0x2a], ax
        xor    edi, edi
        xchg   dword ptr [esi - 6], edi
        or     edi, edi
        je     loc_14_0337
        and    dword ptr [edi], 0
loc_14_0337:
        ret    
loc_14_0338:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_14_0347
        and    dword ptr [0xc0c], 0          ; -> loc_1_0C0C
loc_14_0347:
        clc    
        ret    
sub_14_0349:
        push   ebp
        mov    ebp, esp
        test   byte ptr [ebp + 0xc], 4
        jne    loc_14_03F3
        cmp    ebx, dword ptr [0xbcc]        ; -> loc_1_0BCC
        jne    loc_14_0363
        call   sub_17_0C31
loc_14_0363:
        mov    esi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        test   byte ptr [esi + ebx + 0x1a], 1
        jne    loc_14_03DF
        pushal 
        mov    esi, dword ptr [0xa7c]        ; -> loc_1_0A7C
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_14_040C
        VMMCall  VMM.List_Attach                 ; cd209e000100
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 0xc], edi
        xor    esi, esi
        mov    dword ptr [eax + 0x10], esi
        mov    dword ptr [eax + 0x14], esi
        cmp    ecx, 0x10001
        jb     loc_14_03BF
        cmp    ecx, 0x10005
        ja     loc_14_03BF
        test   edx, edx
        je     loc_14_03BF
        mov    esi, edx
        lea    edi, [eax + 0x10]
        mov    edx, edi
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
loc_14_03BF:
        mov    dword ptr [eax + 8], edx
        mov    dword ptr [esp + 0x14], eax
        popal  
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 1
        mov    ecx, 0x418
        mov    esi, 0x41d                    ; -> loc_14_041D
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        jb     loc_14_03F7
loc_14_03DF:
        mov    edx, ebx
        xor    eax, eax
        xor    ebx, ebx
        mov    esi, 0x443                    ; -> loc_14_0443
        lea    ecx, [eax + 2]
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
loc_14_03F3:
        xor    eax, eax
        jmp    loc_14_041B
loc_14_03F7:
        xchg   edx, eax
        mov    esi, dword ptr [0xa7c]        ; -> loc_1_0A7C
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        jmp    loc_14_040D
loc_14_040C:
        popal  
loc_14_040D:
        mov    esi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        or     byte ptr [esi + ebx + 0x1a], 1
        or     eax, 0xffffffff
loc_14_041B:
        leave  
        ret    
loc_14_041D:
        jae    loc_14_042E
        mov    ebx, dword ptr [edx]
        mov    esi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        push   edx
        call   sub_14_04B0
        pop    edx
loc_14_042E:
        mov    eax, edx
        mov    esi, dword ptr [0xa7c]        ; -> loc_1_0A7C
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        ret    
loc_14_0443:
        mov    ebx, edx
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_14_046A
        mov    esi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        or     byte ptr [esi + ebx + 0x1a], 2
        cmp    byte ptr [esi + ebx + 0x1a], 3
        jne    loc_14_046A
        mov    eax, 0xc
        VMMCall  VMM.System_Control              ; cd2093000100
loc_14_046A:
        ret    
        db  000h
loc_14_046C:
        mov    ebx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_14_0529
        call   sub_17_000F
        and    word ptr [eax + 0x2a], 0
        mov    esi, dword ptr [0xa74]        ; -> loc_1_0A74
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     sub_14_04B0
loc_14_0493:
        cmp    dword ptr [eax + 0xc], ebx
        jne    loc_14_04A8
        or     byte ptr [eax + 0xb], 0x80
        pushal 
        stc    
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        jecxz  loc_14_04A7
        call   ecx
loc_14_04A7:
        popal  
loc_14_04A8:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_14_0493
sub_14_04B0:
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_14_0529
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_14_0529
        push   ebx
        VMMCall  VMM.Get_Execution_Focus         ; cd2030000100
        cmp    ebx, dword ptr [esp]
        jne    loc_14_04DA
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xor    eax, eax
        inc    eax
        call   sub_17_0212
loc_14_04DA:
        pop    ebx
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_14_0529
        mov    esi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        or     byte ptr [esi + ebx + 0x1a], 1
        test   byte ptr [ebx], 0x10
        jne    loc_14_0518
        test   byte ptr [ebx + 2], 1
        jne    loc_14_0529
        VMMCall  VMM.Get_Time_Slice_Priority     ; cd2032000100
        or     eax, 2
        VMMCall  VMM.Set_Time_Slice_Priority     ; cd2033000100
        mov    eax, 0x1388
        mov    ecx, 1
        VMMCall  VMM.Close_VM                    ; cd20ec800100
loc_14_0518:
        cmp    byte ptr [esi + ebx + 0x1a], 3
        jne    loc_14_0529
        xor    eax, eax
        mov    al, 0xc
        VMMCall  VMM.System_Control              ; cd2093000100
loc_14_0529:
        ret    
loc_14_052A:
        mov    ebx, dword ptr [ebp + 0x10]
        and    byte ptr [ebp + 0x2c], 0xfe
        jmp    loc_14_054F
loc_14_0533:
        mov    edx, dword ptr [eax + 8]
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        test   edx, 0x100000
        je     loc_14_054F
        call   sub_17_0B64
loc_14_054F:
        mov    esi, dword ptr [0xa74]        ; -> loc_1_0A74
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_14_0570
loc_14_055D:
        cmp    dword ptr [eax + 0xc], ebx
        jne    loc_14_0568
        test   byte ptr [eax + 0xb], 0x80
        jne    loc_14_0533
loc_14_0568:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_14_055D
loc_14_0570:
        ret    
loc_14_0571:
        xor    ecx, ecx
        xchg   word ptr [0xe0], cx           ; -> obj7:0x00E0
        or     ecx, ecx
        je     loc_14_058A
        push   0
        push   ecx
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
loc_14_058A:
        ret    
loc_14_058B:
        or     byte ptr [ebp + 0x2c], 1
        mov    ebx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_14_05AE
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_14_05B2
        xor    eax, eax
        mov    al, 0x13
        VMMCall  VMM.System_Control              ; cd2093000100
        jb     loc_14_05B2
loc_14_05AE:
        and    byte ptr [ebp + 0x2c], 0xfe
loc_14_05B2:
        ret    
        db  000h
loc_14_05B4:
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   ebx, eax
        mov    esi, dword ptr [ebp]
        test   esi, esi
        je     loc_14_066A
        test   byte ptr [esi + 0xb], 0x80
        jne    loc_14_066A
        mov    esi, dword ptr [esi + 4]
        mov    edi, ebx
        cld    
        push   0x18
        pop    ecx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    byte ptr [ebx + 0x18], cl
        mov    ecx, dword ptr [ebx + 8]
        jecxz  loc_14_05F6
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [ebx + 0x68], eax
        mov    dword ptr [ebx + 0x6c], edx
        mov    byte ptr [ebx + 0x70], 0
loc_14_05F6:
        mov    eax, cs
        cmp    word ptr [ebx + 0x14], ax
        jne    loc_14_0618
        or     byte ptr [ebx], 2
        lea    eax, [ebx + 0x18]
        push   eax
        push   dword ptr [ebx + 0x10]
        VMMCall  VMM._GetVxDName                 ; cd207e010100
        add    esp, 8
        or     eax, eax
        jne    loc_14_0618
        mov    byte ptr [ebx + 0x18], al
loc_14_0618:
        mov    esi, ebx
        xor    ebx, ebx
        xchg   dword ptr [esi], ebx
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_14_066A
        call   sub_17_000F
        test   byte ptr [eax + 7], 0x10
        je     loc_14_066A
        test   byte ptr [eax + 2], 8
        jne    loc_14_0668
        mov    ebx, dword ptr [ebx + 4]
        movzx  ecx, word ptr [ebx + 0x44c]
        shr    ecx, 1
        dec    ecx
        mov    eax, 0xb0000
        cmp    byte ptr [ebx + 0x449], 7
        je     loc_14_0655
        or     ah, 0x80
loc_14_0655:
        add    ax, word ptr [ebx + 0x44e]
        lea    edi, [eax + ebx + 2]
        mov    eax, dword ptr [edi - 2]
        repe scasw ax, word ptr es:[edi]
        jne    loc_14_066A
loc_14_0668:
        inc    dword ptr [esi]
loc_14_066A:
        ret    
        db  000h
_SHELL_CancelAppyTimeEvent:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [0xbe0]        ; -> loc_1_0BE0
        or     eax, eax
        je     loc_14_0696
        cli    
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        sti    
        jb     loc_14_0696
        mov    edx, dword ptr [eax + 8]
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    esi, edx
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
loc_14_0696:
        pop    esi
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 15  vsize=0x58  R-X
loc_15_0000:
        add    edi, dword ptr [0xa04]        ; -> obj1:0x0A04
        mov    ebx, dword ptr [edi]
        or     ebx, ebx
        je     loc_15_002A
        xor    edx, edx
        mov    eax, 0x10009
        lea    ecx, [edx + 1]
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
        call   sub_17_000F
        dec    ecx
        mov    dword ptr [eax - 6], ecx
        call   sub_14_04B0
loc_15_002A:
        clc    
        ret    
loc_15_002C:
        cmp    edi, dword ptr [0xbd8]        ; -> loc_1_0BD8
        je     loc_15_0036
        clc    
        ret    
loc_15_0036:
        xor    eax, eax
        mov    dword ptr [0xbd8], eax        ; -> loc_1_0BD8
        mov    dword ptr [0xbb0], eax        ; -> obj1:0x0BB0
        add    eax, 0x10
        mov    esi, 0xec0                    ; -> loc_11_0EC0
        mov    ecx, 0xb
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        clc    
        ret    
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 16  vsize=0x8bc  R-X
loc_16_0000:
        cmp    dword ptr [esp + 4], 0x219
        jne    loc_16_0024
        cmp    word ptr [esp + 8], 0x8042
        je     loc_11_0250
        cmp    word ptr [esp + 8], 0x8045
        je     loc_11_0283
loc_16_0024:
        xor    eax, eax
        inc    eax
        ret    
sub_16_0028:
        dec    dword ptr [eax + 0xc]
        jne    loc_16_0039
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_16_0039:
        ret    
loc_16_003A:
        mov    ecx, dword ptr [0xbb0]        ; -> obj1:0x0BB0
        jecxz  loc_16_0057
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   0x505
        push   ecx
        VxDCall  SHELL._SHELL_PostMessage        ; cd2006001700
        add    esp, 0x18
loc_16_0057:
        ret    
loc_16_0058:
        push   0
        push   dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [eax + 8]
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    esi, edx
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        call   ecx
        pop    eax
        pop    eax
loc_16_0074:
        mov    esi, dword ptr [0xbe0]        ; -> loc_1_0BE0
        cli    
        VMMCall  VMM.List_Remove_First           ; cd20a5000100
        sti    
        jne    loc_16_0058
        and    byte ptr [0xbe8], 0xfe        ; -> loc_1_0BE8
        ret    
_SHELL_BroadcastSystemMessage:
        push   edi
        push   ebp
        mov    ebp, esp
        push   0
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_16_009B
        mov    dword ptr [ebp + 0x10], esp
loc_16_009B:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [0xbd8], 0          ; -> loc_1_0BD8
        je     loc_16_00F3
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        btr    dword ptr [ebp + 0xc], 0x1f
        jb     loc_16_00C1
        cmp    dword ptr [eax], 1
        je     loc_16_00F3
        cmp    edi, dword ptr [0xbd8]        ; -> loc_1_0BD8
        jne    loc_16_00E0
loc_16_00C1:
        lea    eax, [ebp + 0xc]
        or     byte ptr [eax], 0x20
        push   eax
        mov    eax, esp
        push   eax
        push   4
        push   dword ptr [0xbf0]             ; -> loc_1_0BF0
        push   0
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
        jecxz  loc_16_00E5
loc_16_00E0:
        or     eax, 0xffffffff
        jmp    loc_16_00F0
loc_16_00E5:
        mov    edi, dword ptr [edi + 0x10]
        mov    edx, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [edi + 0x10]
        mov    dword ptr [edx], edi
loc_16_00F0:
        leave  
        pop    edi
        ret    
loc_16_00F3:
        mov    dword ptr [eax], 1
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x1c]
        call   sub_16_010C
        jmp    loc_16_00F0
sub_16_010C:
        push   esi
        push   edi
        mov    esi, dword ptr [0xbe4]        ; -> loc_1_0BE4
        test   esi, esi
        je     loc_16_0159
        VMMCall  VMM.List_Get_First              ; cd20a3000100
loc_16_011E:
        test   eax, eax
        je     loc_16_0159
        inc    dword ptr [eax + 0xc]
        push   esi
        push   eax
        push   dword ptr [eax + 4]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x28]
        call   dword ptr [eax]
        add    esp, 0x10
        pop    ecx
        pop    esi
        xchg   ecx, eax
        mov    edi, eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        xchg   edi, eax
        call   sub_16_0028
        xchg   edi, eax
        test   byte ptr [esp + 0x18], 1
        je     loc_16_011E
        cmp    ecx, 1
        jae    loc_16_011E
loc_16_0159:
        pop    edi
        pop    esi
        sbb    eax, eax
        inc    eax
        ret    0x10
loc_16_0161:
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        cmp    word ptr [esi], 0x219
        jne    loc_16_0194
        test   byte ptr [esi + 3], 0x80
        je     loc_16_0194
        xor    eax, eax
        xchg   word ptr [ebp + 0x1e], ax
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_16_01AA
        add    dword ptr [ebp + 0x1c], eax
loc_16_0194:
        push   dword ptr [esi + 8]
        xor    eax, eax
        mov    ax, word ptr [esi]
        push   eax
        mov    ax, word ptr [esi + 2]
        push   eax
        push   dword ptr [ebp + 0x1c]
        call   sub_16_010C
loc_16_01AA:
        mov    dword ptr [ebp + 0x1c], eax
        ret    
        db  000h, 000h
sub_16_01B0:
        VxDCall  SHELL._SHELL_QueryAppyTimeAvailable ; cd200d001700
        and    eax, 2
        ret    
_SHELL_LocalAllocEx:
        push   esi
        push   edi
        and    dword ptr [esp + 0xc], 0x30040
        call   sub_16_01B0
        je     loc_16_0213
        test   byte ptr [esp + 0xe], 1
        je     loc_16_01E6
        xor    eax, eax
        mov    edi, dword ptr [esp + 0x14]
        or     ecx, 0xffffffff
        cld    
        repne scasb al, byte ptr es:[edi]
        sub    edi, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x10], edi
loc_16_01E6:
        cmp    dword ptr [esp + 0x10], 0x10000
        jae    loc_16_0213
        mov    eax, dword ptr [esp + 0xa]
        mov    ax, word ptr [esp + 0x10]
        push   eax
        mov    eax, esp
        push   eax
        push   4
        push   dword ptr [0xbf4]             ; -> loc_1_0BF4
        push   0
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
        pop    edx
        jecxz  loc_16_0218
loc_16_0213:
        xor    eax, eax
        cdq    
        jmp    loc_16_0267
loc_16_0218:
        or     ax, ax
        je     loc_16_0213
        xchg   edx, eax
        mov    ax, 0x3c1c
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edx, eax
        mov    esi, dword ptr [esp + 0x14]
        or     esi, esi
        je     loc_16_023A
        mov    edi, edx
        cld    
        mov    ecx, dword ptr [esp + 0x10]
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_16_023A:
        rol    eax, 0x10
        mov    ax, word ptr [0xbdc]          ; -> loc_1_0BDC
        rol    eax, 0x10
        test   byte ptr [esp + 0xe], 2
        je     loc_16_0267
        pushal 
        push   eax
        push   eax
        mov    eax, esp
        push   eax
        push   8
        push   dword ptr [0xc1c]             ; -> loc_1_0C1C
        push   0
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x18
        popal  
loc_16_0267:
        pop    edi
        pop    esi
        ret    
_SHELL_LocalFree:
        call   sub_16_01B0
        je     loc_16_0296
        mov    edx, dword ptr [0xbdc]        ; -> loc_1_0BDC
        cmp    word ptr [esp + 6], dx
        jne    loc_16_0296
        lea    eax, [esp + 4]
        push   eax
        push   2
        push   dword ptr [0xbf8]             ; -> loc_1_0BF8
        push   0
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
loc_16_0296:
        ret    
_SHELL_LoadLibrary:
        call   sub_16_01B0
        je     loc_16_02DC
        push   dword ptr [esp + 4]
        push   0
        push   0x10000
        VxDCall  SHELL._SHELL_LocalAllocEx       ; cd2013001700
        add    esp, 0xc
        or     eax, eax
        je     loc_16_02DC
        push   eax
        mov    ecx, esp
        push   eax
        push   ecx
        push   4
        push   dword ptr [0xbfc]             ; -> loc_1_0BFC
        push   0
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
        sub    eax, ecx
        mov    dword ptr [esp + 4], eax
        VxDCall  SHELL._SHELL_LocalFree          ; cd2014001700
        pop    edx
        pop    eax
loc_16_02DC:
        ret    
_SHELL_FreeLibrary:
        call   sub_16_01B0
        je     loc_16_02FC
        lea    eax, [esp + 4]
        push   eax
        push   2
        push   dword ptr [0xc00]             ; -> loc_1_0C00
        push   0
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
loc_16_02FC:
        ret    
_SHELL_GetProcAddress:
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        xchg   dword ptr [esp + 8], eax
        mov    dword ptr [esp + 0xc], eax
        call   sub_16_01B0
        je     loc_16_0361
        xor    edi, edi
        cmp    word ptr [esp + 0xa], di
        je     loc_16_0337
        push   dword ptr [esp + 8]
        push   0
        push   0x10000
        VxDCall  SHELL._SHELL_LocalAllocEx       ; cd2013001700
        add    esp, 0xc
        or     eax, eax
        je     loc_16_0361
        mov    dword ptr [esp + 8], eax
        inc    edi
loc_16_0337:
        lea    eax, [esp + 8]
        push   eax
        push   6
        push   dword ptr [0xc04]             ; -> loc_1_0C04
        push   0
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
        sub    eax, ecx
        dec    edi
        js     loc_16_0361
        push   eax
        push   dword ptr [esp + 0xc]
        VxDCall  SHELL._SHELL_LocalFree          ; cd2014001700
        pop    eax
        pop    eax
loc_16_0361:
        pop    edi
        ret    
_SHELL_CallDll:
        sub    esp, 0x6c
        push   ebx
        push   esi
        push   edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        xor    edi, edi
        mov    ecx, dword ptr [esp + 0x80]
        cmp    ecx, dword ptr [0xbf0]        ; -> loc_1_0BF0
        jne    loc_16_038A
        lea    edi, [esp + 0xc]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
loc_16_038A:
        mov    ebx, dword ptr [ebx + 8]
        mov    eax, dword ptr [esp + 0x7c]
        mov    ecx, dword ptr [esp + 0x80]
        test   eax, eax
        je     loc_16_03C2
        push   eax
        VxDCall  SHELL._SHELL_LoadLibrary        ; cd2015001700
        add    esp, 4
        cmp    eax, 0x20
        jb     loc_16_040E
        mov    dword ptr [esp + 0x7c], eax
        push   dword ptr [esp + 0x80]
        push   eax
        VxDCall  SHELL._SHELL_GetProcAddress     ; cd2017001700
        add    esp, 8
        mov    ecx, eax
loc_16_03C2:
        jecxz  loc_16_03F9
        mov    dword ptr [ebx + 0x1c], ecx
        xor    eax, eax
        mov    dword ptr [ebx + 0x38], eax
        mov    dword ptr [ebx + 0x40], eax
        mov    dword ptr [ebx + 0x44], eax
        movzx  ecx, word ptr [0xbee]         ; -> loc_1_0BEE
        movzx  edx, word ptr [0xbec]         ; -> loc_1_0BEC
        push   dword ptr [esp + 0x88]
        push   dword ptr [esp + 0x88]
        push   edx
        push   ecx
        VMMCall  VMM._CallRing3                  ; cd2053010100
        add    esp, 0x10
loc_16_03F9:
        push   eax
        mov    ecx, dword ptr [esp + 0x80]
        jecxz  loc_16_040D
        push   ecx
        VxDCall  SHELL._SHELL_FreeLibrary        ; cd2016001700
        add    esp, 4
loc_16_040D:
        pop    eax
loc_16_040E:
        or     edi, edi
        je     loc_16_041A
        mov    esi, edi
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
loc_16_041A:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x6c
        ret    
        db  000h, 000h, 000h
_SHELL_ShellExecute:
        push   esi
        xor    esi, esi
        VMMCall  VMM.Get_Environment_String      ; cd20b5000100
        pop    esi
        push   dword ptr [esp + 4]
        push   edx
        mov    eax, esp
        push   eax
        push   8
        push   dword ptr [0xc10]             ; -> loc_1_0C10
        push   0
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x18
        ret    
sub_16_044A:
        push   0
        mov    eax, esp
        push   dword ptr [esp + 8]
        mov    edx, esp
        push   edx
        push   edi
        push   eax
        push   0
        push   dword ptr [esp + 0x20]
        push   ebp
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x20
        ret    8
loc_16_046A:
        and    byte ptr [edi], ch
        das    
        and    byte ptr [eax], al
loc_16_046F:
        inc    ebx
        dec    edi
        dec    ebp
        dec    ebp
        inc    ecx
        dec    esi
        inc    esp
        pop    esp
        push   ebx
        push   esp
        inc    ecx
        push   edx
        push   esp
        inc    ebp
        pop    eax
        inc    ebp
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
        db  000h
sub_16_0585:
        pushal 
        mov    edi, 0x481                    ; -> obj16:0x0481
        cmp    byte ptr [edi], 0
        ja     loc_16_0665
        VMMCall  VMM.Get_Config_Directory        ; cd20b7000100
        push   0x104
        push   edx
        push   edi
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        push   ecx
        push   0x46f                         ; -> loc_16_046F
        push   edx
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        mov    edx, edi
        mov    ax, 0x4300
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jae    loc_16_065F
        push   0
        mov    eax, esp
        push   eax
        push   0xfd0                         ; -> loc_11_0FD0
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        pop    ebp
        test   eax, eax
        jne    loc_16_0649
        push   0x1010                        ; -> loc_11_1010
        push   4
        call   sub_16_044A
        test   eax, eax
        jne    loc_16_060E
        cmp    dword ptr [edi], eax
        je     loc_16_060E
        push   0x1017                        ; -> loc_11_1017
        push   0xf2
        call   sub_16_044A
loc_16_060E:
        push   eax
        push   ebp
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        pop    eax
        test   eax, eax
        jne    loc_16_0649
        push   edi
        VMMCall  VMM._lstrlen                    ; cd207c010100
        add    esp, 4
        push   -1
        push   0x46f                         ; -> loc_16_046F
        push   edx
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        mov    edx, edi
        mov    ax, 0x4300
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jae    loc_16_065F
loc_16_0649:
        lea    edx, [edi - 0xa]
        VMMCall  VMM.OpenFile                    ; cd20b8000100
        jb     loc_16_0662
        xchg   ebx, eax
        mov    ah, 0x3e
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_16_065F:
        clc    
        jmp    loc_16_0665
loc_16_0662:
        mov    byte ptr [edi], 0
loc_16_0665:
        popal  
        ret    
loc_16_0667:
        cmp    word ptr [edx + 0x40], 1
        jae    loc_16_0676
        cmp    word ptr [edx + 0x1a], 0x1202
        jne    loc_16_06A2
loc_16_0676:
        mov    cx, word ptr [edx + 4]
        jcxz   loc_16_0688
        sub    cx, 0x14c
        cmp    cx, 2
        ja     loc_16_06A2
loc_16_0688:
        cmp    word ptr [edx + 0x5c], 2
        je     loc_16_0731
        cmp    word ptr [edx + 0x5c], 3
        jne    loc_16_06A2
        inc    dword ptr [esp]
        jmp    loc_16_0731
loc_16_06A2:
        jmp    loc_17_103E
loc_16_06A7:
        cmp    byte ptr [edx + 0x36], 2
        jne    loc_16_06A2
        cmp    dword ptr [edx + 0x14], 0
        je     loc_16_06A2
        cmp    byte ptr [edx + 2], 5
        jb     loc_16_06A2
        sub    word ptr [edx + 0x3e], 0x401
        cmp    word ptr [edx + 0x3e], 0xfeff
loc_16_06C5:
        jb     loc_16_06A2
        test   byte ptr [edx + 0x37], 2
        jne    loc_16_06A2
        movzx  esi, word ptr [edx + 4]
        sub    si, word ptr [edx + 0x2a]
loc_16_06D5:
        jbe    loc_16_06A2
        cmp    si, 0x60
        jbe    loc_16_06E2
        mov    esi, 0x60
loc_16_06E2:
        mov    eax, dword ptr [edx + 0x2a]
        sub    eax, 0x5e
        cwd    
        mov    ecx, edx
        xchg   edx, eax
        mov    ebx, dword ptr [esp + 4]
        mov    ax, 0x4201
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_16_06FD:
        jb     loc_16_06C5
        mov    ecx, esi
        lea    edx, [esp + 0x14]
        mov    ah, 0x3f
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_16_06FD
loc_16_0711:
        movzx  eax, byte ptr [edx]
        inc    edx
        cmp    al, 8
        jne    loc_16_072A
        cmp    dword ptr [edx], 0x43534f44
        jne    loc_16_072A
        cmp    dword ptr [edx + 4], 0x534c4c41
        je     loc_16_06D5
loc_16_072A:
        add    edx, eax
        stc    
        sbb    ecx, eax
        ja     loc_16_0711
loc_16_0731:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [esp + 0xc], eax
        movzx  edi, word ptr [eax]
        or     edi, edi
        jne    loc_16_075E
        xor    ecx, ecx
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        movzx  eax, ax
        shl    eax, 4
        movzx  edi, word ptr [eax + 0x2c]
loc_16_075E:
        shl    edi, 4
        mov    eax, dword ptr [esp + 0xc]
        movzx  esi, word ptr [eax + 4]
        shl    esi, 4
        movzx  edx, word ptr [eax + 2]
        add    esi, edx
        call   sub_16_0878
        movzx  eax, byte ptr [esi - 1]
        jb     loc_16_078B
        or     byte ptr [esp], 2
        push   esi
        VMMCall  VMM._lstrlen                    ; cd207c010100
        add    esp, 4
loc_16_078B:
        xchg   edi, eax
        push   dword ptr [esp + 8]
        VMMCall  VMM._lstrlen                    ; cd207c010100
        add    esp, 4
        add    edi, eax
        inc    edi
        inc    edi
        test   byte ptr [esp], 1
        jne    loc_16_07C0
        call   sub_16_0585
        jb     loc_16_06A2
        push   0x481                         ; -> obj16:0x0481
        VMMCall  VMM._lstrlen                    ; cd207c010100
        add    esp, 4
        lea    edi, [eax + edi + 4]
loc_16_07C0:
        add    edi, 0x108
        push   0x200
        push   edi
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        test   eax, eax
        je     loc_16_06A2
        mov    dword ptr [esp + 0x10], eax
        xchg   edx, eax
        mov    ecx, edi
        test   byte ptr [esp], 1
        jne    loc_16_080A
        push   ecx
        push   0x481                         ; -> obj16:0x0481
        push   edx
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        push   ecx
        push   0x46a                         ; -> loc_16_046A
        push   edx
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
loc_16_080A:
        push   ecx
        push   dword ptr [esp + 0xc]
        push   edx
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        cmp    byte ptr [esi], 0x20
        je     loc_16_0822
        mov    byte ptr [edx], 0x20
        inc    edx
loc_16_0822:
        test   byte ptr [esp], 2
        jne    loc_16_0836
        movzx  ecx, byte ptr [esi - 1]
        mov    edi, edx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        xchg   ecx, eax
        stosb  byte ptr es:[edi], al
        mov    edx, edi
        jmp    loc_16_0843
loc_16_0836:
        push   ecx
        push   esi
        push   edx
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        inc    edx
loc_16_0843:
        mov    esi, edx
        mov    ax, 0x7147
        call   sub_17_0DAF
        mov    edi, dword ptr [esp + 0x10]
        VxDCall  VCOND.VCOND_Launch_ConApp_Inherited ; cd2001003800
        and    byte ptr [ebp + 0x2c], 0xfe
        push   0
        push   dword ptr [esp + 0x14]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        jmp    loc_17_103E
loc_16_0870:
        inc    ebx
        dec    ebp
        inc    esp
        dec    esp
        dec    ecx
        dec    esi
        inc    ebp
        db  03Dh
sub_16_0878:
        lodsb  al, byte ptr [esi]
        push   esi
        cmp    al, 0x7f
        jne    loc_16_0897
        xor    eax, eax
        jmp    loc_16_0893
loc_16_0882:
        mov    esi, 0x870                    ; -> loc_16_0870
        lea    ecx, [eax + 8]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_16_089A
        or     ecx, 0xffffffff
        repne scasb al, byte ptr es:[edi]
loc_16_0893:
        cmp    byte ptr [edi], al
        jne    loc_16_0882
loc_16_0897:
        pop    esi
        stc    
        ret    
loc_16_089A:
        mov    esi, dword ptr [esp]
        mov    cl, byte ptr [esi]
        mov    esi, edi
        mov    edx, edi
loc_16_08A3:
        lodsb  al, byte ptr [esi]
        test   al, al
        je     loc_16_0897
        cmp    al, 0x22
        jne    loc_16_08B1
        xor    ah, 1
        jmp    loc_16_08A3
loc_16_08B1:
        cmp    al, cl
        jne    loc_16_08A3
        test   ah, ah
        jne    loc_16_08A3
        dec    esi
        pop    eax
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 17  vsize=0x1056  R-X
SHELL_Event:
        pushal 
        call   sub_17_0A20
        mov    dword ptr [esp + 0x1c], eax
        add    eax, 1
        popal  
        ret    
sub_17_000F:
        mov    eax, dword ptr [0xa00]        ; -> obj1:0x0A00
        lea    eax, [eax + ebx + 0xc]
        ret    
        db  000h, 000h, 000h
SHELL_CheckHotkeyAllowed:
        push   ebx
        add    ebx, dword ptr [0xa00]        ; -> obj1:0x0A00
        bt     dword ptr [ebx + 0xc], 0x1b
        pop    ebx
        ret    
sub_17_002A:
        VxDCall  SHELL.SHELL_CheckHotkeyAllowed  ; cd201a001700
        jae    loc_17_0038
        call   sub_1_0000
        stc    
loc_17_0038:
        ret    
        db  000h, 000h, 000h
loc_17_003C:
        call   sub_17_000F
        xchg   edi, eax
        inc    dword ptr [ebp + 0x24]
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    ecx, dword ptr [eax + 2]
        and    cx, 0x7f7f
        bts    dword ptr [edi + 4], 0x10
        jb     loc_17_00E8
        cmp    cx, 3
        jbe    loc_17_0075
        cmp    cx, 7
        je     loc_17_0075
        btr    dword ptr [edi + 4], 0x1c
        jmp    loc_17_007B
loc_17_0075:
        bts    dword ptr [edi + 4], 0x1c
        cmc    
loc_17_007B:
        jae    loc_17_00E4
        test   byte ptr [edi + 7], 0x10
        jne    loc_17_009E
        xchg   ebx, eax
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        xchg   ebx, eax
        xor    eax, ebx
        jne    loc_17_00E4
        and    byte ptr [edi + 3], 0xdf
        test   byte ptr [edi], 4
        je     loc_17_00E4
        or     byte ptr [edi + 3], 0x20
        jmp    loc_17_00AC
loc_17_009E:
        test   byte ptr [edi], 4
        jne    loc_17_00E4
        test   byte ptr [edi + 3], 0x20
        je     loc_17_00E4
        xor    eax, eax
        inc    eax
loc_17_00AC:
        or     eax, 0x200000
        or     byte ptr [edi + 6], 2
        mov    edx, ebx
        mov    esi, 0x2a0                    ; -> loc_11_02A0
        mov    ecx, 0x42b
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        jb     loc_17_00E4
        jmp    loc_17_00D6
loc_17_00CB:
        push   5
        push   edi
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        pop    ecx
        pop    ecx
loc_17_00D6:
        VMMCall  VMM.Get_Crit_Section_Status     ; cd2029000100
        jb     loc_17_00E4
        test   byte ptr [edi + 6], 2
        jne    loc_17_00CB
loc_17_00E4:
        and    byte ptr [edi + 6], 0xfe
loc_17_00E8:
        ret    
loc_17_00E9:
        or     edx, edx
        jne    loc_17_01A5
        push   edi
        push   esi
        call   sub_17_000F
        btr    dword ptr [eax], 2
        jae    loc_17_0118
        and    byte ptr [eax], 0xf7
        xor    eax, eax
        VxDCall  VDD.VDD_Set_VMType              ; cd2004000a00
        mov    ecx, 0x416
        xor    eax, eax
        xor    esi, esi
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
loc_17_0118:
        pop    esi
        pop    edi
        mov    ecx, 0x415
        mov    eax, 0x100000
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_17_015A
        mov    cx, 0x420
        xchg   edi, ebx
        cmp    esi, 2
        je     loc_17_015A
        inc    eax
        cmp    esi, 1
        jb     loc_17_0152
        mov    ecx, 0x421
        or     ebx, ebx
        je     loc_17_0152
        cmp    ebx, edi
        je     loc_17_0152
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jae    loc_17_015A
loc_17_0152:
        xchg   edi, ebx
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
loc_17_015A:
        bts    word ptr [0xb6d], 2           ; -> loc_1_0B6D
        jae    loc_17_017E
        cmp    ax, word ptr [0xb6e]          ; -> obj1:0x0B6E
        jne    loc_17_017E
        cmp    ebx, dword ptr [0xb70]        ; -> obj1:0x0B70
        jne    loc_17_017E
        cmp    ecx, dword ptr [0xb74]        ; -> obj1:0x0B74
        je     loc_17_01A5
loc_17_017E:
        mov    dword ptr [0xb70], ebx        ; -> obj1:0x0B70
        mov    dword ptr [0xb74], ecx        ; -> obj1:0x0B74
        mov    word ptr [0xb6e], ax          ; -> obj1:0x0B6E
        mov    esi, 0x2d5                    ; -> loc_11_02D5
        xor    edx, edx
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_17_01A5
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
loc_17_01A5:
        clc    
        ret    
sub_17_01A7:
        push   ebx
        push   esi
        push   edi
        call   sub_17_0212
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  0CDh, 020h, 004h, 000h, 001h, 000h, 074h, 01Bh, 08Bh, 0DAh, 0B8h, 002h, 000h, 010h, 000h, 0B9h
        db  014h, 004h, 000h, 000h, 0BEh
        dd  loc_17_0A16              ; fixup
        db  033h, 0D2h, 0CDh, 020h, 002h, 000h, 017h, 000h, 073h, 005h, 0E8h
        dd  sub_1_0000               ; fixup
        db  0C3h
loc_17_01DE:
        cmp    word ptr [ebp + 0x14], 0x26
        je     loc_17_01FE
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_17_01FA
        test   byte ptr [ebx], 0x20
        je     loc_17_01FA
        cmp    word ptr [ebp + 0x14], 0x100
        jb     loc_17_01FE
loc_17_01FA:
        or     eax, 0xffffffff
        ret    
loc_17_01FE:
        call   sub_12_0000
        xor    eax, eax
        ret    
        db  000h, 000h
loc_17_0208:
        mov    ebx, dword ptr [ebp + 0x10]
        and    byte ptr [ebp + 0x2c], 0xfe
        xor    eax, eax
        inc    eax
sub_17_0212:
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jae    loc_17_0224
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        or     byte ptr [ebp + 0x2c], 1
loc_17_0224:
        push   ebx
        push   eax
        call   sub_17_000F
        mov    esi, eax
        push   5
        pop    ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        xor    eax, eax
        mov    al, 0xf
        lea    edx, [eax - 2]
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_17_025E
        cmp    dword ptr [esp], 0
        je     loc_17_0251
        VMMCall  VMM.Set_Execution_Focus         ; cd2031000100
loc_17_0251:
        test   byte ptr [esi], 4
        je     loc_17_0264
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        jmp    loc_17_0264
loc_17_025E:
        VMMCall  VMM.System_Control              ; cd2093000100
loc_17_0264:
        dec    edx
        VMMCall  VMM.System_Control              ; cd2093000100
        dec    edx
        dec    edx
        VMMCall  VMM.System_Control              ; cd2093000100
        cmp    ebx, dword ptr [esp + 4]
        jne    loc_17_02B3
        call   sub_17_035F
loc_17_027E:
        mov    ebx, dword ptr [esp + 4]
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_17_0298
        cmp    dword ptr [esp], 0
        je     loc_17_02A5
        VMMCall  VMM.Set_Execution_Focus         ; cd2031000100
        jmp    loc_17_02A5
loc_17_0298:
        xor    eax, eax
        mov    al, 0xf
        lea    edx, [eax - 2]
        VMMCall  VMM.System_Control              ; cd2093000100
loc_17_02A5:
        call   sub_17_0B64
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        pop    eax
        pop    eax
        ret    
loc_17_02B3:
        VMMCall  VMM.Get_Time_Slice_Priority     ; cd2032000100
        cmp    byte ptr [0xe3], 0            ; -> obj7:0x00E3
        je     loc_17_02CF
        mov    byte ptr [0xe3], 0            ; -> obj7:0x00E3
        mov    ecx, dword ptr [0xbac]        ; -> obj1:0x0BAC
loc_17_02CF:
        or     eax, 0x202
        cmp    byte ptr [0xe2], 0            ; -> obj7:0x00E2
        jne    loc_17_02E6
        movzx  edx, dx
        mov    dword ptr [0xba8], edx        ; -> obj1:0x0BA8
loc_17_02E6:
        push   eax
        call   sub_17_000F
        mov    esi, eax
        pop    eax
        mov    edi, dword ptr [0xba8]        ; -> obj1:0x0BA8
loc_17_02F5:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_17_0326
        cmp    ebx, dword ptr [esp + 4]
        je     loc_17_031A
        test   byte ptr [ebx], 2
        je     loc_17_02F5
        cmp    di, word ptr [esi + 0x20]
        jae    loc_17_02F5
        mov    di, word ptr [esi + 0x20]
        jmp    loc_17_02F5
loc_17_031A:
        cmp    di, word ptr [esi + 0x1e]
        jae    loc_17_02F5
        mov    di, word ptr [esi + 0x1e]
        jmp    loc_17_02F5
loc_17_0326:
        shr    di, 1
        movzx  edi, di
        cmp    edi, dword ptr [0xba8]        ; -> obj1:0x0BA8
        jbe    loc_17_0347
        mov    edx, edi
        cmp    byte ptr [0xe2], 0            ; -> obj7:0x00E2
        jne    loc_17_0354
        inc    byte ptr [0xe2]               ; -> obj7:0x00E2
        jmp    loc_17_0354
loc_17_0347:
        mov    edx, dword ptr [0xba8]        ; -> obj1:0x0BA8
        mov    byte ptr [0xe2], 0            ; -> obj7:0x00E2
loc_17_0354:
        VMMCall  VMM.Set_Time_Slice_Priority     ; cd2033000100
        jmp    loc_17_027E
sub_17_035F:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VMMCall  VMM.Get_Time_Slice_Priority     ; cd2032000100
        and    eax, 0xfffffdff
        or     eax, 2
        cmp    byte ptr [0xe2], 0            ; -> obj7:0x00E2
        je     loc_17_038A
        mov    byte ptr [0xe2], 0            ; -> obj7:0x00E2
        mov    edx, dword ptr [0xba8]        ; -> obj1:0x0BA8
loc_17_038A:
        cmp    ebx, dword ptr [esp]
        jne    loc_17_03FE
        mov    esi, eax
        call   sub_17_000F
        xchg   esi, eax
        cmp    byte ptr [0xe3], 0            ; -> obj7:0x00E3
        jne    loc_17_03A9
        movzx  ecx, cx
        mov    dword ptr [0xbac], ecx        ; -> obj1:0x0BAC
loc_17_03A9:
        mov    edi, dword ptr [0xbac]        ; -> obj1:0x0BAC
loc_17_03AF:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_17_03CE
        test   byte ptr [ebx], 2
        je     loc_17_03AF
        cmp    di, word ptr [esi + 0x20]
        jae    loc_17_03AF
        mov    di, word ptr [esi + 0x20]
        jmp    loc_17_03AF
loc_17_03CE:
        shr    di, 1
        movzx  edi, di
        cmp    edi, dword ptr [0xbac]        ; -> obj1:0x0BAC
        jbe    loc_17_03EF
        mov    ecx, edi
        cmp    byte ptr [0xe3], 0            ; -> obj7:0x00E3
        jne    loc_17_0414
        inc    byte ptr [0xe3]               ; -> obj7:0x00E3
        jmp    loc_17_0414
loc_17_03EF:
        mov    ecx, dword ptr [0xbac]        ; -> obj1:0x0BAC
        mov    byte ptr [0xe3], 0            ; -> obj7:0x00E3
        jmp    loc_17_0414
loc_17_03FE:
        cmp    byte ptr [0xe3], 0            ; -> obj7:0x00E3
        je     loc_17_0414
        mov    byte ptr [0xe3], 0            ; -> obj7:0x00E3
        mov    ecx, dword ptr [0xbac]        ; -> obj1:0x0BAC
loc_17_0414:
        VMMCall  VMM.Set_Time_Slice_Priority     ; cd2033000100
        pop    ebx
        ret    
loc_17_041C:
        ret    
loc_17_041D:
        or     byte ptr [ebp + 0x2c], 1
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        inc    eax
        je     loc_17_041C
        dec    eax
        xchg   edi, eax
        mov    ebx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_17_041C
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_17_041C
        push   5
        pop    ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        call   sub_17_000F
        push   eax
        VMMCall  VMM.Get_Time_Slice_Priority     ; cd2032000100
        pop    esi
        mov    edx, eax
        and    eax, 3
        and    byte ptr [esi], 0xfc
        or     dword ptr [esi], eax
        mov    ecx, dword ptr [esi]
        xor    ecx, dword ptr [edi]
        and    ecx, 0x1f
        test   cl, 8
        je     loc_17_047A
        mov    eax, dword ptr [edi]
        and    eax, 8
        VxDCall  VDD.VDD_Set_VMType              ; cd2004000a00
loc_17_047A:
        test   cl, 0x10
        je     loc_17_0494
        test   byte ptr [edi], 0x10
        jne    loc_17_048C
        VMMCall  VMM.Resume_VM                   ; cd202c000100
        jmp    loc_17_0492
loc_17_048C:
        VMMCall  VMM.Suspend_VM                  ; cd202b000100
loc_17_0492:
        jb     loc_17_04AC
loc_17_0494:
        xor    dword ptr [esi], ecx
        mov    eax, dword ptr [esi]
        and    eax, 3
        xor    ecx, ecx
        inc    ecx
        mov    edx, ecx
        VMMCall  VMM.Set_Time_Slice_Priority     ; cd2033000100
        jb     loc_17_04AC
        and    byte ptr [ebp + 0x2c], 0xfe
loc_17_04AC:
        VMMCall  VMM.End_Critical_Section        ; cd2020800100
loc_17_04B2:
        or     byte ptr [ebp + 0x2c], 1
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        inc    edi
        je     loc_17_050B
        dec    edi
        mov    ebx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_17_050B
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_17_050B
        call   sub_17_000F
        push   eax
        VMMCall  VMM.Get_Time_Slice_Priority     ; cd2032000100
        pop    ebx
        and    eax, 3
        mov    ecx, dword ptr [ebx]
        and    ecx, 0xc
        or     eax, ecx
        and    byte ptr [edi], 0xf0
        and    byte ptr [ebx], 0xf0
        or     dword ptr [edi], eax
        or     dword ptr [ebx], eax
        lea    esi, [ebx + 0x2e]
        add    edi, 0x2e
        mov    ecx, 8
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        and    byte ptr [ebp + 0x2c], 0xfe
loc_17_050B:
        ret    
loc_17_050C:
        mov    eax, dword ptr [ebp]
        or     eax, eax
        je     loc_17_054A
        test   byte ptr [eax + 0xb], 0x80
        jne    loc_17_054A
        sbb    edi, edi
        mov    esi, dword ptr [0xa74]        ; -> loc_1_0A74
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        jb     loc_17_054A
        push   dword ptr [eax + 8]
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [eax + 4]
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        add    edi, 1
        call   ecx
        pop    edx
        test   edx, 0x100000
        je     loc_17_054A
        call   sub_17_0B64
loc_17_054A:
        ret    
loc_17_054B:
        mov    ebx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_17_057D
        mov    edx, ebx
        add    edx, dword ptr [0xa6c]        ; -> loc_1_0A6C
        bts    dword ptr [edx + 0x10], 5
        jb     loc_17_057D
        test   byte ptr [edx + 0x10], 0x40
        jne    loc_17_057D
        xor    eax, eax
        mov    esi, 0x57e                    ; -> loc_17_057E
        lea    ecx, [eax + 0x63]
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        mov    dword ptr [edx], esi
loc_17_057D:
        ret    
loc_17_057E:
        xor    ecx, ecx
        mov    dword ptr [edx], ecx
        or     byte ptr [edx + 0x10], 0x40
        mov    cl, 5
        jmp    loc_17_0594
loc_17_058A:
        push   ecx
        push   edx
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        pop    ecx
        pop    ecx
loc_17_0594:
        test   byte ptr [edx + 0x10], 0x20
        jne    loc_17_058A
        and    byte ptr [edx + 0x10], 0xbf
        ret    
loc_17_059F:
        mov    ebx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_17_05D0
        mov    edx, ebx
        add    edx, dword ptr [0xa6c]        ; -> loc_1_0A6C
        xor    esi, esi
        xchg   dword ptr [edx], esi
        VMMCall  VMM.Cancel_Priority_VM_Event    ; cd2015000100
        and    byte ptr [edx + 0x10], 0xdf
        test   byte ptr [edx + 0x10], 0x40
        je     loc_17_05D0
        push   edx
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
loc_17_05D0:
        ret    
loc_17_05D1:
        and    byte ptr [ebp + 0x2c], 0xfe
        mov    ebx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_17_05F7
        VxDCall  SHELL.SHELL_CheckHotkeyAllowed  ; cd201a001700
        jae    loc_17_05F7
        cmp    byte ptr [ebp + 0x1c], 0
        je     loc_17_05F3
        call   sub_1_0000
loc_17_05F3:
        or     byte ptr [ebp + 0x2c], 1
loc_17_05F7:
        ret    
sub_17_05F8:
        mov    eax, dword ptr [0xa6c]        ; -> loc_1_0A6C
        mov    esi, dword ptr [eax + ebx + 4]
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        ret    
sub_17_0608:
        push   edx
        push   edi
        mov    ecx, dword ptr [0xa6c]        ; -> loc_1_0A6C
        mov    esi, dword ptr [ecx + ebx + 4]
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_17_065E
        xor    edx, edx
        cmp    dword ptr [edi], 0x54534e49
        jne    loc_17_062F
        cmp    dword ptr [edi + 4], 0x4c4c41
        je     loc_17_063C
loc_17_062F:
        cmp    dword ptr [edi], 0x55544553
        jne    loc_17_063C
        cmp    word ptr [edi + 4], 0x50
loc_17_063C:
        jne    loc_17_0645
        or     dl, 2
        dec    byte ptr [ecx + ebx + 0x18]
loc_17_0645:
        inc    byte ptr [ecx + ebx + 0x18]
        push   eax
        xchg   edi, eax
        xor    eax, eax
        cld    
        lea    ecx, [eax + 0x15]
        rep stosd dword ptr es:[edi], eax
        pop    eax
        mov    byte ptr [eax + 3], dl
        VMMCall  VMM.List_Attach                 ; cd209e000100
        clc    
loc_17_065E:
        pop    edi
        pop    edx
loc_17_0660:
        ret    
sub_17_0661:
        mov    eax, dword ptr [0xa6c]        ; -> loc_1_0A6C
        bts    dword ptr [eax + ebx + 0x10], 0xa
        jb     loc_17_0660
        mov    ecx, 0x423
        xchg   edx, eax
        xor    esi, esi
        VxDCall  SHELL.SHELL_Event               ; cd2002801700
loc_17_067C:
        sub    esp, 0x14
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        movzx  edx, word ptr [ebp + 0x14]
        movzx  ecx, word ptr [ebp + 0x18]
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [esp + 0xc], ecx
        and    dword ptr [esp], 0
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_17_071E
        cmp    edx, 4
        ja     loc_17_071E
        cmp    eax, dword ptr [ebx + 4]
        je     loc_17_06B4
        cmp    eax, -1
        jne    loc_17_06BC
loc_17_06B4:
        mov    dword ptr [esp + 0x10], 0xbb2 ; -> obj1:0x0BB2
loc_17_06BC:
        test   dl, 1
        je     loc_17_06CE
        call   sub_17_000F
        add    eax, 0x2e
        push   0x1e
        pop    ecx
        jmp    loc_17_06D9
loc_17_06CE:
        call   sub_17_05F8
        add    eax, 4
        push   0x50
        pop    ecx
loc_17_06D9:
        mov    esi, dword ptr [esp + 0x10]
        xchg   edi, eax
        cmp    edx, 2
        jb     loc_17_0704
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        je     loc_17_06F6
        mov    ecx, dword ptr [0xa6c]        ; -> loc_1_0A6C
        and    byte ptr [ecx + ebx + 0x11], 0xfb
loc_17_06F6:
        xchg   edi, esi
        mov    ecx, dword ptr [esp + 0xc]
        cmp    edi, 0xbb2                    ; -> obj1:0x0BB2
        je     loc_17_071E
loc_17_0704:
        jecxz  loc_17_0711
loc_17_0706:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        or     al, al
        loopne loc_17_0706
        je     loc_17_0711
        mov    byte ptr [edi - 1], cl
loc_17_0711:
        cmp    edx, 2
        jae    loc_17_071B
        call   sub_17_0661
loc_17_071B:
        inc    dword ptr [esp]
loc_17_071E:
        mov    eax, dword ptr [esp]
        add    esp, 0x14
        mov    dword ptr [ebp + 0x1c], eax
        clc    
        ret    
loc_17_0729:
        mov    ebx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_17_073F
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [ebp + 0x14], eax
        jmp    loc_17_067C
loc_17_073F:
        ret    
sub_17_0740:
        mov    edi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        mov    ecx, dword ptr [edi + ebx + 0x10]
        and    byte ptr [edi + ebx + 0x10], 0x7f
        call   sub_17_05F8
loc_17_0754:
        test   byte ptr [eax + 2], 1
        je     loc_17_0767
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_17_0754
        or     byte ptr [edi + ebx + 0x10], 0x80
loc_17_0767:
        cmp    ecx, dword ptr [edi + ebx + 0x10]
        je     loc_17_077C
        mov    ecx, 0x42a
        xor    eax, eax
        xor    esi, esi
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
loc_17_077C:
        ret    
sub_17_077D:
        push   edx
        xor    ecx, ecx
        VMMCall  VMM.Get_Set_Real_DOS_PSP        ; cd2042010100
        xchg   ecx, eax
        pop    edx
        call   sub_17_05F8
        cmp    word ptr [eax], cx
        ret    
loc_17_0791:
        movsx  edx, word ptr [ebp + 0x14]
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_17_07B6
        mov    esi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        cmp    dh, 1
        jne    loc_17_07B8
        mov    eax, dword ptr [esi + ebx + 0x10]
        test   al, 2
        je     loc_17_07B6
        and    eax, 1
loc_17_07B3:
        mov    dword ptr [ebp + 0x1c], eax
loc_17_07B6:
        clc    
        ret    
loc_17_07B8:
        cmp    dh, 2
        ja     loc_17_07D7
        je     loc_17_083B
        call   sub_17_077D
        jne    loc_17_07B6
        and    edx, 1
        mov    byte ptr [eax + 2], dl
        jne    loc_17_07CE
loc_17_07CE:
        call   sub_17_0740
loc_17_07D3:
        xor    eax, eax
        jmp    loc_17_07B3
loc_17_07D7:
        cmp    dh, 4
        ja     loc_17_07F4
        je     loc_17_07E5
        call   sub_11_0874
        jmp    loc_17_07D3
loc_17_07E5:
        call   sub_17_077D
        jne    loc_17_07B6
        mov    ecx, dword ptr [ebp + 0x10]
        mov    word ptr [eax], cx
        jmp    loc_17_07D3
loc_17_07F4:
        cmp    edx, -2
        jb     loc_17_07B6
        cmp    byte ptr [esi + ebx + 0x18], 2
        jne    loc_17_0836
        and    byte ptr [esi + ebx + 0x19], 0xfe
        inc    edx
        jne    loc_17_0836
        or     byte ptr [esi + ebx + 0x19], 1
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        mov    ax, 0x3c00
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        call   sub_17_0DF6
        test   eax, eax
        je     loc_17_0836
        and    byte ptr [esi + ebx + 0x19], 0xfe
        mov    word ptr [ebp + 0x14], 0
loc_17_0836:
        jmp    loc_17_07B6
loc_17_083B:
        or     byte ptr [esi + ebx + 0x10], 1
        mov    ecx, 0x429
        xor    eax, eax
        mov    al, 0x30
        xor    esi, esi
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        jmp    loc_17_07D3
loc_17_0853:
        mov    ebx, dword ptr [ebp + 0x10]
        xor    eax, eax
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_17_086E
        mov    edi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        test   byte ptr [edi + ebx + 0x10], 0x80
        je     loc_17_086E
        inc    eax
loc_17_086E:
        mov    dword ptr [ebp + 0x1c], eax
        ret    
        db  000h, 000h
sub_17_0874:
        mov    esi, dword ptr [0xbbc]        ; -> loc_1_0BBC
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        jmp    loc_17_0888
loc_17_0882:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
loc_17_0888:
        je     loc_17_088F
        cmp    eax, edx
        jne    loc_17_0882
        ret    
loc_17_088F:
        stc    
        ret    
loc_17_0891:
        call   sub_17_0874
        jb     loc_17_08BA
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        call   sub_17_002A
        jb     loc_17_08BA
        mov    esi, 0x8ba                    ; -> loc_17_08BA
        mov    ecx, 0x422
        mov    eax, 0x100000
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
loc_17_08BA:
        ret    
loc_17_08BB:
        or     byte ptr [ebp + 0x2c], 1
        mov    edx, dword ptr [ebp]
        mov    edx, dword ptr [edx + 4]
        call   sub_17_0874
        jb     loc_17_08DC
        and    byte ptr [ebp + 0x2c], 0xfe
        mov    eax, dword ptr [edx + 2]
        mov    edx, dword ptr [edx + 4]
        mov    dword ptr [ebp], eax
        mov    dword ptr [ebp + 0x1c], edx
loc_17_08DC:
        ret    
        db  000h, 000h, 000h
loc_17_08E0:
        push   ebx
        push   ecx
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        call   sub_17_002A
        jb     loc_17_0928
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_17_0928
        xor    ecx, ecx
        inc    ecx
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        xor    esi, esi
        mov    eax, 0x100000
        mov    ecx, 0xffff
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        pop    ecx
        pop    eax
        VxDCall  VKD.VKD_Reflect_Hot_Key         ; cd2005000d00
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        jmp    loc_17_092A
loc_17_0928:
        pop    ecx
        pop    ebx
loc_17_092A:
        ret    
loc_17_092B:
        VxDCall  VKD.VKD_Cancel_Hot_Key_State    ; cd2006000d00
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        call   sub_17_002A
        jb     loc_17_096A
        bts    dword ptr [0xaa3], 0          ; -> obj1:0x0AA3
        jb     loc_17_096A
        mov    dword ptr [0xbc0], ecx        ; -> loc_1_0BC0
        mov    esi, 0x9a5                    ; -> loc_17_09A5
        mov    eax, 0x100002
        mov    ecx, 0x415
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        sbb    byte ptr [0xaa3], 0           ; -> obj1:0x0AA3
loc_17_096A:
        ret    
        db  0B8h, 0AAh
loc_17_096D:
        mov    edi, dword ptr [ebp + 4]
        jmp    loc_17_0978
        db  090h, 0D0h, 0ECh, 013h, 0C0h, 097h
loc_17_0978:
        mov    ecx, dword ptr [0xbc0]        ; -> loc_1_0BC0
        mov    eax, dword ptr [0xa94]        ; -> loc_1_0A94
        VxDCall  VKD.VKD_Reflect_Hot_Key         ; cd2005000d00
        mov    esi, 0x96b                    ; -> obj17:0x096B
loc_17_098E:
        shr    edi, 1
        jb     loc_17_099B
        xor    ecx, ecx
        inc    ecx
        VxDCall  VKD.VKD_Force_Keys              ; cd2007000d00
loc_17_099B:
        inc    esi
        cmp    esi, 0x96d                    ; -> loc_17_096D
        jb     loc_17_098E
        ret    
loc_17_09A5:
        and    byte ptr [0xaa3], 0xfe        ; -> obj1:0x0AA3
        ret    
loc_17_09AD:
        VxDCall  VKD.VKD_Cancel_Hot_Key_State    ; cd2006000d00
        push   ebx
        push   ecx
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_17_0A12
        call   sub_17_000F
        test   byte ptr [eax], 4
        je     loc_17_09F5
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        pop    ecx
        pop    eax
        VxDCall  VKD.VKD_Reflect_Hot_Key         ; cd2005000d00
        mov    eax, 0x100000
        mov    ecx, 0xffff
        xor    esi, esi
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        jmp    loc_17_0A15
loc_17_09F5:
        lea    eax, [edx + 0x100000]
        mov    ecx, 0x414
        mov    esi, 0xa16                    ; -> loc_17_0A16
        VxDCall  SHELL.SHELL_Event               ; cd2002001700
        jae    loc_17_0A12
        call   sub_1_0000
loc_17_0A12:
        add    esp, 8
loc_17_0A15:
        ret    
loc_17_0A16:
        jae    loc_17_0A1D
        call   sub_1_0000
loc_17_0A1D:
        ret    
        db  000h, 000h
sub_17_0A20:
        enter  0x14, 0
        mov    dword ptr [ebp - 4], ebx
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 0x10], esi
        mov    dword ptr [ebp - 0x14], edx
        cmp    dword ptr [0xc0c], 0          ; -> loc_1_0C0C
        je     loc_17_0ADD
        mov    esi, dword ptr [0xa74]        ; -> loc_1_0A74
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_17_0ADD
        VMMCall  VMM.List_Attach                 ; cd209e000100
        mov    esi, eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 0xc], ebx
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_17_0AC9
        call   sub_17_000F
        movzx  edx, word ptr [eax + 0x2a]
        or     edx, edx
        je     loc_17_0AC9
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0x30ffff
        mov    edi, dword ptr [ebp - 0x10]
        mov    dword ptr [esi + 8], ecx
        cmp    word ptr [ebp - 8], 0x41a
        je     loc_17_0A9A
        test   ecx, 0x300000
        je     loc_17_0AA0
loc_17_0A9A:
        mov    word ptr [esi + 0xa], 0x10
loc_17_0AA0:
        mov    dword ptr [esi], edi
        movzx  ecx, cx
        mov    eax, esi
        or     edi, edi
        jne    loc_17_0AAD
        xor    eax, eax
loc_17_0AAD:
        push   esi
        push   0xae3                         ; -> loc_17_0AE3
        push   eax
        push   ecx
        push   dword ptr [ebp - 8]
        push   edx
        VxDCall  SHELL._SHELL_PostMessage        ; cd2006001700
        add    esp, 0x18
        xor    edx, edx
        xchg   edx, eax
        or     edx, edx
        jne    loc_17_0AE0
loc_17_0AC9:
        mov    eax, esi
        mov    esi, dword ptr [0xa74]        ; -> loc_1_0A74
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_17_0ADD:
        or     eax, 0xffffffff
loc_17_0AE0:
        leave  
        ret    
        db  0C3h
loc_17_0AE3:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        push   ebp
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    edi, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp + 8], 0
        je     loc_17_0B37
        test   byte ptr [edi + 0xa], 0x10
        je     loc_17_0B30
        bts    dword ptr [0xbc8], 0          ; -> loc_1_0BC8
        jb     loc_17_0B30
        mov    eax, 0x7d0
        mov    esi, 0xb5d                    ; -> loc_17_0B5D
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        xchg   dword ptr [0xbc4], esi        ; -> loc_1_0BC4
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    eax, 0x10
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
loc_17_0B30:
        cmp    dword ptr [edi], 0
        jne    loc_17_0B57
        jmp    loc_17_0B42
loc_17_0B37:
        cmp    dword ptr [edi], 0
        je     loc_17_0B42
        mov    edx, dword ptr [edi + 4]
        stc    
        call   dword ptr [edi]
loc_17_0B42:
        mov    esi, dword ptr [0xa74]        ; -> loc_1_0A74
        mov    eax, dword ptr [ebp + 0xc]
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_17_0B57:
        pop    ebp
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_17_0B5D:
        and    dword ptr [0xbc4], 0          ; -> loc_1_0BC4
sub_17_0B64:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        btr    dword ptr [0xbc8], 0          ; -> loc_1_0BC8
        jae    loc_17_0B8B
        xor    esi, esi
        xchg   dword ptr [0xbc4], esi        ; -> loc_1_0BC4
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        push   -0x10
        pop    eax
        VMMCall  VMM.Adjust_Exec_Priority        ; cd201e000100
loc_17_0B8B:
        ret    
_SHELL_PostMessage:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    ecx, ecx
        cmp    dword ptr [0xc0c], ecx        ; -> loc_1_0C0C
        je     loc_17_0BE8
        mov    esi, dword ptr [0xa80]        ; -> loc_1_0A80
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_17_0BE8
        mov    edx, eax
        push   esi
        lea    esi, [ebp + 8]
        mov    ecx, 6
        mov    edi, eax
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    ecx, dword ptr [ebp + 0xe]
        and    ecx, 0xa
        xor    ecx, 7
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VMMCall  VMM.Wake_Up_VM                  ; cd2039000100
        mov    esi, 0x740                    ; -> loc_1_0740
        mov    eax, 0x1000
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        or     ecx, 0xffffffff
loc_17_0BE8:
        mov    eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h
sub_17_0BF0:
        push   ebx
        VMMCall  VMM.Get_Crit_Section_Status     ; cd2029000100
        or     eax, 0xffffffff
        jecxz  loc_17_0C02
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
loc_17_0C02:
        pop    ebx
        ret    
sub_17_0C04:
        call   sub_17_0BF0
        je     loc_17_0C21
        mov    eax, dword ptr [0xbd0]        ; -> loc_1_0BD0
        push   5
        pop    ecx
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    dword ptr [0xbcc], ebx        ; -> loc_1_0BCC
        clc    
        ret    
loc_17_0C21:
        stc    
        ret    
sub_17_0C23:
        mov    eax, dword ptr [0xbd4]        ; -> loc_1_0BD4
        push   5
        pop    ecx
        VMMCall  VMM.Wait_Semaphore              ; cd2027800100
sub_17_0C31:
        and    dword ptr [0xbcc], 0          ; -> loc_1_0BCC
        mov    eax, dword ptr [0xbd0]        ; -> loc_1_0BD0
        jmp    loc_17_0C44
loc_17_0C3F:
        mov    eax, dword ptr [0xbd4]        ; -> loc_1_0BD4
loc_17_0C44:
        VMMCall  VMM.Signal_Semaphore            ; cd2028800100
        add    byte ptr [eax], al
_SHELL_GetDOSAppInfo:
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    esi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_17_0CA3
        mov    edi, dword ptr [esi + ebx + 0xc]
        cmp    dword ptr [esp + 0x10], 1
        ja     loc_17_0CA3
        jb     loc_17_0C86
        mov    ecx, dword ptr [esp + 0x18]
        cmp    ecx, 0x20
        jae    loc_17_0CA4
        bt     dword ptr [edi + 0x1c4], ecx
        jb     loc_17_0CA5
        jmp    loc_17_0CA4
loc_17_0C86:
        lea    esi, [edi]
        call   sub_17_0F19
        push   dword ptr [esp + 0x14]
        push   esi
        VMMCall  VMM._lstrcmpi                   ; cd208e010100
        add    esp, 8
        test   eax, eax
        je     loc_17_0CA5
        xor    eax, eax
        jmp    loc_17_0CA4
loc_17_0CA3:
        inc    eax
loc_17_0CA4:
        inc    eax
loc_17_0CA5:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
sub_17_0DAD:
        mov    ah, 0x47
sub_17_0DAF:
        push   eax
        mov    ah, 0x19
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    dword ptr [esi], 0x5c3a41
        add    byte ptr [esi], al
        pop    eax
        xor    dl, dl
        add    esi, 3
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        sub    esi, 3
        ret    
sub_17_0DD4:
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        movzx  edi, word ptr [eax + 4]
        shl    edi, 4
        movzx  edx, word ptr [eax + 2]
        add    edi, edx
        inc    edi
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
sub_17_0DF6:
        sub    esp, 0x18
        push   esi
        mov    dword ptr [esp + 0x18], eax
        mov    esi, dword ptr [esi + ebx + 0xc]
        xor    ecx, ecx
        mov    dword ptr [esp + 4], esi
        mov    dword ptr [esi + 0x1c4], ecx
        lea    ecx, [esi]
        mov    dword ptr [esp + 0x14], ecx
        push   0x40
        push   eax
        push   ecx
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        lea    eax, [esi + 0x40]
        push   0x40
        push   edi
        push   eax
        VMMCall  VMM._lmemcpy                    ; cd207d010100
        add    esp, 0xc
        lea    esi, [esi + 0x80]
        call   sub_17_0DAD
        mov    esi, dword ptr [esp + 0x14]
        call   sub_17_0F19
        mov    edi, esi
        call   sub_17_0F2A
        jae    loc_17_0F12
        lea    eax, [esp + 0x10]
        push   eax
        push   edi
        push   dword ptr [0xca9]             ; -> obj17:0x0CA9
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        test   eax, eax
        jne    loc_17_0F12
        xor    esi, esi
loc_17_0E71:
        lea    ecx, [edi + 0x40]
        sub    ecx, dword ptr [esp + 0x14]
        lea    eax, [esp + 0xc]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esp + 4]
        lea    ecx, [ecx + 0x1c4]
        lea    edx, [esp + 8]
        mov    dword ptr [edx], 4
        push   edx
        push   ecx
        push   0
        push   0
        push   eax
        push   edi
        push   esi
        push   dword ptr [esp + 0x2c]
        VMMCall  VMM._RegEnumValue               ; cd2050010100
        add    esp, 0x20
        test   eax, eax
        jne    loc_17_0EC1
        mov    ah, 0x43
        mov    edx, dword ptr [esp + 0x14]
        inc    esi
        pushal 
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        popal  
        jb     loc_17_0E71
        jmp    loc_17_0ECD
loc_17_0EC1:
        mov    ecx, dword ptr [esp + 4]
        xor    edx, edx
        mov    dword ptr [ecx + 0x1c4], edx
loc_17_0ECD:
        push   dword ptr [esp + 0x10]
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        push   0x40
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x1c]
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        mov    ecx, dword ptr [esp + 4]
        test   byte ptr [ecx + 0x1c4], 1
        je     loc_17_0F12
        mov    esi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        test   byte ptr [esi + ebx + 0x11], 3
        jne    loc_17_0F12
        call   sub_11_0CD5
        jae    loc_17_0F12
        mov    al, 1
        jmp    loc_17_0F14
loc_17_0F12:
        xor    eax, eax
loc_17_0F14:
        pop    esi
        add    esp, 0x18
        ret    
sub_17_0F19:
        push   esi
loc_17_0F1A:
        lodsb  al, byte ptr [esi]
        test   al, al
        je     loc_17_0F28
        cmp    al, 0x5c
        jne    loc_17_0F1A
        mov    dword ptr [esp], esi
        jmp    loc_17_0F1A
loc_17_0F28:
        pop    esi
        ret    
sub_17_0F2A:
        xor    eax, eax
        mov    ecx, eax
        mov    edx, eax
loc_17_0F30:
        lodsb  al, byte ptr [esi]
        test   al, al
        je     loc_17_0F54
        cmp    al, 0x41
        jae    loc_17_0F3C
        add    ah, 0xd
loc_17_0F3C:
        and    al, 0x1f
        add    dx, dx
        rol    cx, 5
        xor    dl, al
        xor    cl, al
        add    dl, ah
        add    ah, ch
        and    ah, 0xf8
        xor    cl, ah
        jmp    loc_17_0F30
loc_17_0F54:
        and    ch, 7
        and    dh, 7
        bt     dword ptr [0xcad], ecx        ; -> obj17:0x0CAD
        jae    loc_17_0F6A
        bt     dword ptr [0xcad], edx        ; -> obj17:0x0CAD
loc_17_0F6A:
        ret    
        db  000h
loc_17_0F6C:
        stc    
        ret    
loc_17_0F6E:
        and    byte ptr [ebp + 0x2c], 0xfe
        clc    
        ret    
loc_17_0F74:
        mov    esi, dword ptr [0xa6c]        ; -> loc_1_0A6C
        cmp    word ptr [esi + ebx + 0x18], 2
        jb     loc_17_0F6C
        ja     loc_17_0F8D
        call   sub_17_0DD4
        test   eax, eax
        jne    loc_17_0F6E
loc_17_0F8D:
        test   byte ptr [esi + ebx + 0x10], 8
        je     loc_17_0F6C
        or     byte ptr [ebp + 0x2c], 1
        add    esp, -0x74
        xor    eax, eax
        mov    dword ptr [esp], eax
        mov    dword ptr [esp + 0x10], eax
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edx, eax
        mov    dword ptr [esp + 8], edx
        mov    ax, 0x3d40
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_17_104C
        mov    dword ptr [esp + 4], eax
        mov    ebx, eax
        mov    ecx, 0x40
        lea    edx, [esp + 0x14]
        mov    ah, 0x3f
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_17_103E
        cmp    eax, 0x40
        jne    loc_17_103E
        cmp    word ptr [edx], 0x5a4d
        jne    loc_17_103E
        mov    edx, dword ptr [esp + 0x50]
        test   edx, edx
        je     loc_17_103E
        shld   ecx, edx, 0x10
        mov    ebx, dword ptr [esp + 4]
        mov    ax, 0x4200
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_17_103E
        mov    ebx, dword ptr [esp + 4]
        mov    ecx, 0x5e
        lea    edx, [esp + 0x14]
        mov    ah, 0x3f
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_17_103E
        sub    eax, ecx
        jne    loc_17_103E
        cmp    dword ptr [edx], 0x4550
        je     loc_16_0667
        cmp    word ptr [edx], 0x454e
        je     loc_16_06A7
loc_17_103E:
        mov    ebx, dword ptr [esp + 4]
        mov    ah, 0x3e
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_17_104C:
        add    esp, 0x74
        bt     word ptr [ebp + 0x2c], 0
        ret    
VxD__LTEXT ENDS
