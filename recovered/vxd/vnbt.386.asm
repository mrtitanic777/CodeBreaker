; VNBT -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 27312 instructions recovered by recursive descent

Declare_Virtual_Device VNBT, 3, 0, VNBT_Control, 0049Bh, Undefined_Init_Order
;   DDK version 0400h   init_order 05400C000h   flags 00000h
;   service table: obj1:0x12e6c, 1 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x134a4  R-X
        db  0CDh, 020h, 001h, 080h, 088h, 004h, 0CDh, 020h, 002h, 080h, 088h, 004h
sub_1_000C:
        VMMCall  VMM.Get_System_Time             ; cd203f800100
sub_1_0012:
        VxDCall  Dev_0488.Service_8003h          ; cd2003808804
sub_1_0018:
        VxDCall  Dev_0488.Service_8004h          ; cd2004808804
sub_1_001E:
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d804000
sub_1_0024:
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e804000
sub_1_002A:
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011804000
sub_1_0030:
        VxDCall  Dev_0488.Service_8005h          ; cd2005808804
        VxDCall  Dev_0488.Service_800Dh          ; cd200d808804
sub_1_003C:
        VxDCall  Dev_0488.Service_8006h          ; cd2006808804
sub_1_0042:
        mov    eax, dword ptr [0x12e68]      ; -> loc_1_12E68
        VxDCall  Dev_0488.Service_8009h          ; cd2009808804
        mov    eax, dword ptr [0x12e68]      ; -> loc_1_12E68
        VxDCall  Dev_0488.Service_800Ch          ; cd200c808804
sub_1_0058:
        mov    eax, dword ptr [0x12e68]      ; -> loc_1_12E68
        VxDCall  Dev_0488.Service_800Bh          ; cd200b808804
        add    byte ptr [ebx], dh
        rcl    byte ptr [eax + 0x53], 0xe8
        add    eax, dword ptr [edi]
        add    byte ptr [eax], al
        add    esp, 8
        ret    
sub_1_0071:
        VxDCall  Dev_049A.Service_0000h          ; cd2000009a04
        jae    loc_1_007F
        mov    eax, 0x1a
        ret    
loc_1_007F:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        VxDCall  Dev_049A.Service_0001h          ; cd2001009a04
        add    esp, 0x10
        pop    ebp
        ret    
VNBT_Service_00:
        mov    ax, 0x100
        clc    
        ret    
loc_1_00A3:
        call   sub_1_35C0
        clc    
        ret    
VNBT_Control:
        cmp    eax, 1
        je     loc_2_002C
        cmp    eax, 0x1b
        je     loc_2_002C
        cmp    eax, 3
        je     loc_1_00A3
        cmp    eax, 0x23
        je     loc_1_00C8
        clc    
        ret    
loc_1_00C8:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esi + 0xc]
        cmp    ecx, 0
        je     loc_1_00D8
        cmp    ecx, -1
        jne    loc_1_00DC
loc_1_00D8:
        xor    eax, eax
        jmp    loc_1_0152
loc_1_00DC:
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_1_00F6
        push   dword ptr [esi + 0x14]
        push   eax
        call   sub_1_03BC
        add    esp, 8
        or     eax, eax
        je     loc_1_0156
        mov    dword ptr [esi + 0x10], eax
loc_1_00F6:
        mov    eax, dword ptr [esi + 0x18]
        or     eax, eax
        je     loc_1_0110
        push   dword ptr [esi + 0x1c]
        push   eax
        call   sub_1_03BC
        add    esp, 8
        or     eax, eax
        je     loc_1_0156
        mov    dword ptr [esi + 0x18], eax
loc_1_0110:
        mov    eax, 0
        push   eax
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        call   sub_1_3230
        add    esp, 0x18
        push   eax
        push   dword ptr [esi + 0x14]
        push   dword ptr [esi + 0x10]
        call   sub_1_0403
        add    esp, 8
        pop    eax
        push   eax
        push   dword ptr [esi + 0x1c]
        push   dword ptr [esi + 0x18]
        call   sub_1_0403
        add    esp, 8
        pop    eax
loc_1_0152:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0156:
        mov    eax, 0x6f8
        jmp    loc_1_0152
sub_1_015D:
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, 0x18a                    ; -> loc_1_018A
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Call_At_Appy_Time ; cd204e003300
        add    esp, 0xc
        cmp    eax, 0
        je     loc_1_0182
        mov    eax, 1
        jmp    loc_1_0184
loc_1_0182:
        xor    eax, eax
loc_1_0184:
        pop    ebp
        ret    
        db  000h, 000h, 000h, 000h
loc_1_018A:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ecx
        xor    eax, eax
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, 0x186                    ; -> obj1:0x0186
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Child    ; cd2004003300
        add    esp, 0xc
        mov    esi, dword ptr [0x186]        ; -> obj1:0x0186
        mov    edi, 0
        cmp    eax, 0
        je     loc_1_01C0
        mov    ecx, 1
        jmp    loc_1_0240
loc_1_01C0:
        cmp    edi, esi
        je     loc_1_0222
        xor    eax, eax
        push   eax
        push   esi
        mov    eax, 0x186                    ; -> obj1:0x0186
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Sibling  ; cd2005003300
        add    esp, 0xc
        mov    edi, dword ptr [0x186]        ; -> obj1:0x0186
        cmp    eax, 0
        je     loc_1_01F0
        cmp    eax, 0xd
        je     loc_1_01EE
        mov    ecx, 2
        jmp    loc_1_0240
loc_1_01EE:
        mov    edi, esi
loc_1_01F0:
        xor    eax, eax
        push   eax
        push   edi
        VxDCall  CONFIGMG._CONFIGMG_Query_Remove_SubTree ; cd200c003300
        add    esp, 8
        cmp    eax, 0
        je     loc_1_0209
        mov    ecx, 4
        jmp    loc_1_0240
loc_1_0209:
        xor    eax, eax
        push   eax
        push   edi
        VxDCall  CONFIGMG._CONFIGMG_Remove_SubTree ; cd200d003300
        add    esp, 8
        cmp    eax, 0
        je     loc_1_01C0
        mov    ecx, 5
        jmp    loc_1_0240
loc_1_0222:
        xor    eax, eax
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Reenumerate_DevNode ; cd2033003300
        add    esp, 8
        cmp    eax, 0
        je     loc_1_023E
        mov    ecx, 6
        jmp    loc_1_0240
loc_1_023E:
        xor    eax, eax
loc_1_0240:
        pop    ecx
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  053h, 0CDh, 020h, 004h, 000h, 015h, 000h, 0CDh, 020h, 001h, 000h, 001h, 000h, 0F7h, 003h, 020h
        db  000h, 000h, 000h, 075h, 003h, 003h, 043h, 004h, 00Fh, 0B7h, 000h, 05Bh, 0C3h
sub_1_0262:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld    
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        and    ecx, 3
        je     loc_1_027A
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_1_029C
loc_1_027A:
        mov    ecx, dword ptr [ebp + 0x10]
        shr    ecx, 2
        je     loc_1_0286
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_028F
loc_1_0286:
        sub    esi, dword ptr [ebp + 8]
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_1_028F:
        sub    esi, 4
        sub    edi, 4
        mov    ecx, 5
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
loc_1_029C:
        dec    esi
        sub    esi, dword ptr [ebp + 8]
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    
VNBT_PM_API:
        push   ebp
        push   ebx
        push   esi
        push   edi
        push   edx
        push   ecx
        movzx  edi, word ptr [ebp + 0x18]
        movzx  eax, word ptr [ebp + 0x10]
        push   eax
        movzx  eax, word ptr [ebp + 0x38]
        push   eax
        push   ebx
        call   sub_1_0374
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_0368
        or     eax, eax
        je     loc_1_02E6
        movzx  ecx, word ptr [ebp]
        push   ecx
        push   eax
        call   sub_1_03BC
        add    esp, 8
        or     eax, eax
        je     loc_1_0368
loc_1_02E6:
        mov    esi, eax
        movzx  eax, word ptr [ebp + 0x1c]
        push   eax
        movzx  eax, word ptr [ebp + 0x14]
        push   eax
        push   ebx
        call   sub_1_0374
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_0368
        or     eax, eax
        je     loc_1_0316
        movzx  ebx, word ptr [ebp + 4]
        push   ebx
        push   eax
        call   sub_1_03BC
        add    esp, 8
        or     eax, eax
        je     loc_1_0368
loc_1_0316:
        mov    edx, eax
        push   esi
        push   edx
        mov    eax, 1
        push   eax
        movzx  ecx, word ptr [ebp]
        movzx  ebx, word ptr [ebp + 4]
        push   ebx
        push   edx
        push   ecx
        push   esi
        push   edi
        call   sub_1_3230
        add    esp, 0x18
        pop    edx
        pop    esi
        mov    word ptr [ebp + 0x1c], ax
        or     esi, esi
        je     loc_1_034F
        push   edx
        movzx  ecx, word ptr [ebp]
        push   ecx
        push   esi
        call   sub_1_0403
        add    esp, 8
        pop    edx
loc_1_034F:
        or     edx, edx
        je     loc_1_0361
        movzx  ebx, word ptr [ebp + 4]
        push   ebx
        push   edx
        call   sub_1_0403
        add    esp, 8
loc_1_0361:
        pop    ecx
        pop    edx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_0368:
        cmp    eax, -1
        mov    word ptr [ebp + 0x1c], ax
        jmp    loc_1_0361
        db  000h, 000h, 000h
sub_1_0374:
        push   ebx
        push   ecx
        push   edx
        push   esi
        mov    ebx, dword ptr [esp + 0x14]
        movzx  eax, word ptr [esp + 0x18]
        movzx  esi, word ptr [esp + 0x1c]
        or     eax, eax
        je     loc_1_03B0
        test   dword ptr [ebx], 0x20
        je     loc_1_03B5
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_03B0
        add    eax, esi
        cmp    eax, 0x110000
        jae    loc_1_03B0
loc_1_03AD:
        add    eax, dword ptr [ebx + 4]
loc_1_03B0:
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret    
loc_1_03B5:
        shl    eax, 4
        add    eax, esi
        jmp    loc_1_03AD
sub_1_03BC:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esp + 0x14]
        or     eax, eax
        je     loc_1_03FB
        or     ebx, ebx
        je     loc_1_03FB
        movzx  ecx, ax
        and    ch, 0xf
        mov    esi, ecx
        add    ebx, ecx
        add    ebx, 0xfff
        shr    ebx, 0xc
        shr    eax, 0xc
        push   0x40000000
        push   ebx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_03FF
        add    eax, esi
loc_1_03FB:
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_1_03FF:
        xor    eax, eax
        jmp    loc_1_03FB
sub_1_0403:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esp + 0x14]
        or     eax, eax
        je     loc_1_043E
        or     ebx, ebx
        je     loc_1_043E
        movzx  ecx, ax
        and    ch, 0xf
        add    ebx, ecx
        add    ebx, 0xfff
        shr    ebx, 0xc
        shr    eax, 0xc
        push   0x40000000
        push   ebx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_0447
loc_1_043E:
        mov    eax, 1
loc_1_0443:
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_1_0447:
        xor    eax, eax
        jmp    loc_1_0443
        db  053h, 057h, 056h, 08Bh, 044h, 024h, 010h, 08Bh, 05Ch, 024h, 014h, 00Bh, 0C0h, 074h, 02Bh, 00Bh
        db  0DBh, 074h, 027h, 00Fh, 0B7h, 0C8h, 080h, 0E5h, 00Fh, 003h, 0D9h, 081h, 0C3h, 0FFh, 00Fh, 000h
        db  000h, 0C1h, 0EBh, 00Ch, 0C1h, 0E8h, 00Ch, 08Bh, 0CBh, 06Ah, 000h, 053h, 050h, 0CDh, 020h, 067h
        db  000h, 001h, 000h, 083h, 0C4h, 00Ch, 03Bh, 0C1h, 075h, 009h, 0B8h, 001h, 000h, 000h, 000h, 05Eh
        db  05Fh, 05Bh, 0C3h, 033h, 0C0h, 0EBh, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h
sub_1_04A0:
        mov    ax, word ptr [esp + 8]
        add    ax, 0x100
        push   eax
        call   sub_1_35F0
        add    esp, 4
        mov    dword ptr [0x12ec0], eax      ; -> obj1:0x12EC0
        or     eax, eax
        jne    loc_1_04C0
        sub    eax, eax
        ret    
        db  08Bh, 0FFh
loc_1_04C0:
        mov    dword ptr [0x12ec0], eax      ; -> obj1:0x12EC0
        add    eax, 0x100
        mov    dword ptr [0x12ec4], eax      ; -> obj1:0x12EC4
        mov    eax, 1
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_04E0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_64EC
        add    esp, 4
        push   esi
        call   sub_1_0024
        add    esp, 4
        sub    eax, eax
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0510:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        sub    ebp, ebp
        sub    ebx, ebx
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, edi
loc_1_0520:
        push   esi
        call   sub_1_05B0
        xor    ecx, ecx
        mov    cl, al
        add    esp, 4
        or     ecx, ecx
        je     loc_1_055C
        cmp    ecx, 0xa
        je     loc_1_0558
        cmp    ecx, 0xd
        je     loc_1_0520
        mov    ecx, dword ptr [esi + 8]
        mov    byte ptr [edi + ecx], al
        cmp    edi, 0xff
        jne    loc_1_056A
        mov    eax, dword ptr [esi + 8]
        mov    byte ptr [edi + eax], 0
        dec    edi
        mov    ebp, 1
        jmp    loc_1_056A
loc_1_0558:
        or     edi, edi
        je     loc_1_0520
loc_1_055C:
        mov    eax, dword ptr [esi + 8]
        mov    byte ptr [edi + eax], 0
        mov    ebp, 1
        mov    ebx, ebp
loc_1_056A:
        or     ebp, ebp
        jne    loc_1_0571
        inc    edi
        jmp    loc_1_0520
loc_1_0571:
        or     ebx, ebx
        jne    loc_1_0586
loc_1_0575:
        push   esi
        call   sub_1_05B0
        add    esp, 4
        or     al, al
        je     loc_1_0586
        cmp    al, 0xa
        jne    loc_1_0575
loc_1_0586:
        or     edi, edi
        je     loc_1_059B
        mov    eax, dword ptr [esp + 0x18]
        inc    dword ptr [esi + 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [esi + 8]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_059B:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_05B0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 0x14]
        cmp    dword ptr [esi + 0x18], ecx
        jbe    loc_1_05D0
        lea    eax, [ecx + 1]
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [esi + 0xc]
        mov    al, byte ptr [ecx + eax]
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_05D0:
        cmp    dword ptr [esi + 0x10], 0
        je     loc_1_05DA
        sub    al, al
        pop    esi
        ret    
loc_1_05DA:
        mov    eax, dword ptr [0x12ec0]      ; -> obj1:0x12EC0
        push   eax
        push   0x100
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_64B2
        add    esp, 0xc
        mov    dword ptr [esi + 0x14], 0
        cmp    eax, 0x100
        jae    loc_1_0605
        mov    dword ptr [esi + 0x10], 1
loc_1_0605:
        mov    dword ptr [esi + 0x18], eax
        or     eax, eax
        je     loc_1_061A
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [esi + 0x14], 1
        mov    al, byte ptr [eax]
        pop    esi
        ret    
loc_1_061A:
        sub    al, al
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_0620:
        push   ebx
        push   esi
        push   edi
        push   0x100
        call   sub_1_001E
        add    esp, 4
        mov    ebx, eax
        mov    eax, dword ptr [0x12ec8]      ; -> obj1:0x12EC8
        inc    dword ptr [0x12ec8]           ; -> obj1:0x12EC8
        or     eax, eax
        jne    loc_1_069D
        mov    edi, dword ptr [esp + 0x10]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edx, dword ptr [0x12ec4]      ; -> obj1:0x12EC4
        mov    edi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        or     ebx, ebx
        je     loc_1_069D
        cmp    dword ptr [0x12ec0], 0        ; -> obj1:0x12EC0
        je     loc_1_069D
        push   edx
        call   sub_1_6480
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_069D
        push   0x20
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    loc_1_06B6
        push   esi
        call   sub_1_64EC
        add    esp, 4
loc_1_069D:
        dec    dword ptr [0x12ec8]           ; -> obj1:0x12EC8
        or     ebx, ebx
        je     loc_1_06B0
        push   ebx
        call   sub_1_0024
        add    esp, 4
loc_1_06B0:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_06B6:
        mov    dword ptr [eax], esi
        mov    dword ptr [eax + 4], 0
        mov    ecx, dword ptr [0x12ec0]      ; -> obj1:0x12EC0
        mov    dword ptr [eax + 0xc], ecx
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 0x18], 0
        mov    dword ptr [eax + 0x14], 0
        mov    dword ptr [eax + 0x10], 0
        mov    ecx, dword ptr [0x12ec8]      ; -> obj1:0x12EC8
        dec    ecx
        mov    dword ptr [0x12ec8], ecx      ; -> obj1:0x12EC8
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0700:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   0x100
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esi + 0x1c], eax
        mov    edi, eax
        or     edi, edi
        jne    loc_1_0721
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
loc_1_0721:
        mov    esi, dword ptr [esi + 0xc]
        mov    ecx, 0x40
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0740:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, 0x40
        mov    edi, dword ptr [eax + 0xc]
        mov    esi, dword ptr [eax + 0x1c]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        call   sub_1_0024
        add    esp, 4
        pop    edi
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0770:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    bl, bl
        mov    edi, dword ptr [esp + 0x1c]
        or     edi, edi
        jne    loc_1_0790
        mov    al, 0x39
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_0790:
        push   edi
        call   sub_1_0A90
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_07A9
        mov    al, 0x23
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_07A9:
        cmp    byte ptr [ebp + 0x52], 0
        jne    loc_1_07B9
        mov    al, 0x23
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_07B9:
        mov    al, byte ptr [edi]
        and    al, 0x80
        cmp    al, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        jne    loc_1_0842
        push   0x1c
        call   sub_1_001E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_07E4
        mov    al, 0x38
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_07E4:
        mov    dword ptr [esi + 8], 0x8121994
        mov    dword ptr [esi + 4], esi
        mov    dword ptr [esi], esi
        mov    dword ptr [esi + 0xc], edi
        push   0xc
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esi + 0x10], eax
        or     eax, eax
        jne    loc_1_0817
        push   esi
        call   sub_1_0024
        add    esp, 4
        mov    al, 0x38
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_0817:
        mov    dword ptr [esi + 0x14], 0
        mov    dword ptr [esi + 0x18], 0
        mov    dword ptr [esi], 0x132ac      ; -> loc_1_132AC
        mov    eax, dword ptr [0x132b0]      ; -> loc_1_132B0
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x132b0]      ; -> loc_1_132B0
        mov    dword ptr [eax], esi
        mov    dword ptr [0x132b0], esi      ; -> loc_1_132B0
        jmp    loc_1_0846
loc_1_0842:
        mov    esi, dword ptr [esp + 0x14]
loc_1_0846:
        mov    byte ptr [edi + 1], 0xff
        mov    byte ptr [edi + 0x31], 0xff
        mov    al, byte ptr [edi]
        and    eax, 0x7f
        cmp    eax, 0x70
        jg     loc_1_0870
        je     loc_1_0943
        sub    eax, 0x10
        cmp    eax, 0x26
        ja     loc_1_088C
        shl    eax, 2
        jmp    loc_1_09EC
        db  090h
loc_1_0870:
        cmp    eax, 0x71
        jl     loc_1_088C
        cmp    eax, 0x72
        jle    loc_1_08B8
        cmp    eax, 0x78
        je     loc_1_0952
        cmp    eax, 0x79
        je     loc_1_0943
loc_1_088C:
        mov    bl, 3
        jmp    loc_1_095E
loc_1_0893:
        push   edi
        push   ebp
        call   sub_1_0DB0
        jmp    loc_1_0959
        db  090h
loc_1_08A0:
        push   edi
        push   ebp
        call   sub_1_1550
        jmp    loc_1_0959
loc_1_08AC:
        push   edi
        push   ebp
        call   sub_1_1410
        jmp    loc_1_0959
loc_1_08B8:
        push   edi
        push   ebp
        call   sub_1_0E40
        jmp    loc_1_0959
loc_1_08C4:
        push   1
        push   edi
        push   ebp
        call   sub_1_1210
        add    esp, 0xc
        jmp    loc_1_095C
loc_1_08D5:
        push   edi
        push   ebp
        call   sub_1_1010
        jmp    loc_1_0959
loc_1_08DE:
        push   edi
        push   ebp
        call   sub_1_0BA0
        add    esp, 8
        and    eax, 0xff
        push   eax
        call   sub_1_0AD0
        add    esp, 4
        jmp    loc_1_095C
loc_1_08F8:
        push   edi
        push   ebp
        call   sub_1_0CA0
        jmp    loc_1_0959
loc_1_0901:
        push   edi
        push   ebp
        call   sub_1_15E0
        jmp    loc_1_0959
loc_1_090A:
        push   edi
        push   ebp
        call   sub_1_16D0
        jmp    loc_1_0959
loc_1_0913:
        push   edi
        push   ebp
        call   sub_1_1F30
        jmp    loc_1_0959
        db  08Dh, 064h, 024h, 000h
loc_1_0920:
        mov    eax, dword ptr [esp + 0x20]
        push   eax
        push   edi
        push   ebp
        call   sub_1_1970
        add    esp, 0xc
        jmp    loc_1_095C
loc_1_0931:
        push   edi
        push   ebp
        call   sub_1_1B20
        jmp    loc_1_0959
loc_1_093A:
        push   edi
        push   ebp
        call   sub_1_21C0
        jmp    loc_1_0959
loc_1_0943:
        push   0
        push   0
        push   edi
        call   sub_1_25A0
        add    esp, 0xc
        jmp    loc_1_095E
loc_1_0952:
        push   edi
        push   ebp
        call   sub_1_1A80
loc_1_0959:
        add    esp, 8
loc_1_095C:
        mov    bl, al
loc_1_095E:
        cmp    bl, 0xff
        je     loc_1_0980
        or     bl, bl
        je     loc_1_0980
        mov    byte ptr [edi + 1], bl
        mov    byte ptr [edi + 0x31], bl
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_09DF
        mov    dword ptr [esi + 0x14], 1
        jmp    loc_1_0982
        db  08Dh, 049h, 000h
loc_1_0980:
        sub    bl, bl
loc_1_0982:
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_09DF
        cmp    dword ptr [esi + 0x14], 0
        jne    loc_1_09C0
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [esi + 0x18], 1
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        push   eax
        call   sub_1_0030
        add    esp, 4
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_09C0:
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_0024
        add    esp, 4
        push   esi
        call   sub_1_0024
        add    esp, 4
loc_1_09DF:
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_09EC:
        xchg   ebx, eax
        or     byte ptr [eax], al
        add    byte ptr [eax - 0x53fffff8], ah ; -> loc_1_08A0
        or     byte ptr [eax], al
        add    byte ptr [eax + ecx + 0x8b80000], cl ; -> loc_1_088C
        add    byte ptr [eax], al
        les    ecx, ptr [eax]
        add    byte ptr [eax], al
        aad    8
        add    byte ptr [eax], al
        mov    eax, 0x8c000008               ; -> loc_1_088C
        or     byte ptr [eax], al
        add    byte ptr [eax + ecx + 0x88c0000], cl ; -> loc_1_088C
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        fimul  word ptr [eax]
        add    byte ptr [eax], al
        clc    
        or     byte ptr [eax], al
        add    dh, bl                        ; -> loc_1_08DE
        or     byte ptr [eax], al
        add    al, bh                        ; -> loc_1_08F8
        or     byte ptr [eax], al
        add    byte ptr [eax + ecx + 0x88c0000], cl ; -> loc_1_088C
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        add    dword ptr [ecx], ecx
        add    byte ptr [eax], al
        or     cl, byte ptr [ecx]
        add    byte ptr [eax], al
        adc    ecx, dword ptr [ecx]
        add    byte ptr [eax], al
        and    byte ptr [ecx], cl
        add    byte ptr [eax], al
        xor    dword ptr [ecx], ecx
        add    byte ptr [eax], al
        cmp    cl, byte ptr [ecx]
        add    byte ptr [eax], al
        add    dword ptr [ecx], ecx
        add    byte ptr [eax], al
        lea    esp, [esp]
        lea    esp, [esp]
sub_1_0A90:
        push   ebx
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        jne    loc_1_0AA0
        sub    eax, eax
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_0AA0:
        sub    ecx, ecx
        mov    edx, 0x133c4                  ; -> loc_1_133C4
        mov    bl, byte ptr [eax + 0x30]
loc_1_0AAA:
        mov    eax, dword ptr [edx]
        cmp    byte ptr [eax + 0x51], bl
        je     loc_1_0AC1
        add    edx, 4
        inc    ecx
        cmp    edx, 0x133e4                  ; -> loc_1_133E4
        jb     loc_1_0AAA
        sub    eax, eax
        pop    ebx
        ret    
loc_1_0AC1:
        mov    eax, dword ptr [ecx*4 + 0x133c4] ; -> loc_1_133C4
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0AD0:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        jne    loc_1_0AE0
        sub    al, al
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_0AE0:
        cmp    eax, 0xff
        jne    loc_1_0AF0
        mov    al, 0xff
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0AF0:
        cmp    eax, 0xc0000206
        jg     loc_1_0B40
        je     loc_1_0B89
        cmp    eax, 0xc00000be
        jg     loc_1_0B25
        je     loc_1_0B7B
        cmp    eax, 0xc0000035
        je     loc_1_0B72
        cmp    eax, 0xc0000043
        je     loc_1_0B72
        cmp    eax, 0xc00000bc
        je     loc_1_0B75
        cmp    eax, 0xc00000bd
        je     loc_1_0B78
        mov    al, 0x40
        ret    
loc_1_0B25:
        cmp    eax, 0xc00000c1
        je     loc_1_0B80
        cmp    eax, 0xc00000ca
        je     loc_1_0B83
        cmp    eax, 0xc0000120
        je     loc_1_0B86
        mov    al, 0x40
        ret    
        db  08Dh, 049h, 000h
loc_1_0B40:
        cmp    eax, 9
        jg     loc_1_0B60
        je     loc_1_0B96
        cmp    eax, 0xc000020c
        je     loc_1_0B8C
        cmp    eax, 0xca000001
        je     loc_1_0B90
        cmp    eax, 1
        je     loc_1_0B93
        mov    al, 0x40
        ret    
        db  08Dh, 049h, 000h
loc_1_0B60:
        cmp    eax, 0xf
        je     loc_1_0B8C
        cmp    eax, 0x13
        je     loc_1_0B99
        cmp    eax, 0x15
        je     loc_1_0B9C
        mov    al, 0x40
        ret    
loc_1_0B72:
        mov    al, 0xd
        ret    
loc_1_0B75:
        mov    al, 0x12
        ret    
loc_1_0B78:
        mov    al, 0x16
        ret    
loc_1_0B7B:
        mov    al, 0x14
        ret    
        db  08Bh, 0FFh
loc_1_0B80:
        mov    al, 0x22
        ret    
loc_1_0B83:
        mov    al, 0x17
        ret    
loc_1_0B86:
        mov    al, 0xb
        ret    
loc_1_0B89:
        mov    al, 1
        ret    
loc_1_0B8C:
        mov    al, 0xa
        ret    
        db  090h
loc_1_0B90:
        mov    al, 0xf
        ret    
loc_1_0B93:
        mov    al, 9
        ret    
loc_1_0B96:
        mov    al, 6
        ret    
loc_1_0B99:
        mov    al, 0x18
        ret    
loc_1_0B9C:
        mov    al, 5
        ret    
        db  090h
sub_1_0BA0:
        sub    esp, 0x4c
        push   ebx
        push   esi
        push   edi
        lea    eax, [esp + 0xc]
        push   1
        mov    esi, dword ptr [esp + 0x64]
        push   eax
        mov    al, byte ptr [esi + 3]
        mov    edi, dword ptr [esp + 0x64]
        push   eax
        push   edi
        call   sub_1_2C70
        add    esp, 0x10
        or     al, al
        jne    loc_1_0C8E
        mov    eax, dword ptr [esp + 0xc]
        cmp    dword ptr [eax + 0x44], 0
        je     loc_1_0BE0
        mov    al, 0x15
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x4c
        ret    
        db  08Dh, 049h, 000h
loc_1_0BE0:
        mov    al, byte ptr [esi]
        and    eax, 0x7f
        cmp    eax, 0x22
        mov    edx, 0x12f3c                  ; -> loc_1_12F3C
        je     loc_1_0BF2
        lea    edx, [esi + 0xa]
loc_1_0BF2:
        mov    dword ptr [esp + 0x3c], 1
        mov    word ptr [esp + 0x40], 0x12
        mov    word ptr [esp + 0x42], 0x11
        mov    word ptr [esp + 0x44], 0
        lea    ecx, [esp + 0x46]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [ecx + 0xc], eax
        lea    eax, [esp + 0x3c]
        mov    dword ptr [esp + 0x34], 0x1a
        mov    dword ptr [esp + 0x38], eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esp + 0x14], eax
        push   esi
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        push   eax
        lea    eax, [esp + 0x38]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        call   sub_1_C598
        add    esp, 0x1c
        mov    edi, eax
        push   edi
        call   sub_1_0AD0
        add    esp, 4
        mov    bl, al
        or     bl, bl
        je     loc_1_0C7B
        cmp    bl, 0xff
        jne    loc_1_0C8C
loc_1_0C7B:
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        push   eax
        push   edi
        push   esi
        call   sub_1_25A0
        add    esp, 0xc
loc_1_0C8C:
        mov    al, bl
loc_1_0C8E:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x4c
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0CA0:
        sub    esp, 0x18
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x28]
        mov    cl, byte ptr [esi + 3]
        cmp    cl, 0xff
        je     loc_1_0D59
        lea    eax, [esp + 8]
        push   1
        push   eax
        mov    edi, dword ptr [esp + 0x2c]
        push   ecx
        push   edi
        call   sub_1_2C70
        add    esp, 0x10
        or     al, al
        jne    loc_1_0DA6
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 0x44], 0
        je     loc_1_0CE4
        mov    al, 0x15
        pop    edi
        pop    esi
        add    esp, 0x18
        ret    
loc_1_0CE4:
        mov    al, byte ptr [esi]
        and    eax, 0x7f
        cmp    eax, 0x23
        jne    loc_1_0D10
        lea    eax, [esp + 8]
        push   0
        push   eax
        mov    al, byte ptr [esi + 3]
        push   eax
        push   edi
        call   sub_1_2C70
        add    esp, 0x10
        or     al, al
        je     loc_1_0D10
        mov    al, 0x17
        pop    edi
        pop    esi
        add    esp, 0x18
        ret    
        db  08Bh, 0FFh
loc_1_0D10:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esp + 0x10], eax
        push   esi
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        push   eax
        push   0
        lea    eax, [esp + 0x28]
        push   0
        push   eax
        call   sub_1_C194
        add    esp, 0x20
        cmp    eax, 0xff
        jne    loc_1_0D4A
        mov    al, 0xff
        pop    edi
        pop    esi
        add    esp, 0x18
        ret    
loc_1_0D4A:
        push   eax
        call   sub_1_0AD0
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 0x18
        ret    
loc_1_0D59:
        push   0x1c
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    loc_1_0D70
        mov    al, 9
        pop    edi
        pop    esi
        add    esp, 0x18
        ret    
        db  090h
loc_1_0D70:
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 0x14], 0
        mov    dword ptr [eax + 0x18], 0
        xor    ecx, ecx
        mov    cx, word ptr [esi + 8]
        mov    edi, dword ptr [esp + 0x24]
        mov    dword ptr [eax + 0x10], ecx
        mov    ecx, dword ptr [esi + 4]
        add    edi, 0x34
        mov    dword ptr [eax + 0xc], ecx
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [eax], edi
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        mov    dword ptr [edi + 4], eax
        mov    al, 0xff
loc_1_0DA6:
        pop    edi
        pop    esi
        add    esp, 0x18
        ret    
        db  08Dh, 064h, 024h, 000h
sub_1_0DB0:
        sub    esp, 0x14
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x24]
        mov    dword ptr [esp + 8], 0
        cmp    byte ptr [edi + 0x1a], 0x2a
        jne    loc_1_0DD0
        mov    al, 0x15
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
        db  090h
loc_1_0DD0:
        lea    eax, [esp + 8]
        push   edi
        push   eax
        lea    eax, [esp + 0x14]
        mov    esi, dword ptr [esp + 0x28]
        push   eax
        push   esi
        call   sub_1_2A90
        add    esp, 0x10
        or     al, al
        jne    loc_1_0E37
        mov    eax, dword ptr [esp + 8]
        push   edi
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax]
        push   eax
        lea    eax, [esp + 0x18]
        push   0
        push   eax
        call   sub_1_B238
        add    esp, 0x14
        mov    edi, eax
        or     edi, edi
        je     loc_1_0E2E
        cmp    edi, 0xff
        je     loc_1_0E2E
        mov    eax, dword ptr [esp + 8]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        push   esi
        call   sub_1_3080
        add    esp, 0x10
loc_1_0E2E:
        push   edi
        call   sub_1_0AD0
        add    esp, 4
loc_1_0E37:
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
sub_1_0E40:
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], 0
        mov    esi, dword ptr [esp + 0x4c]
        sub    edi, edi
        lea    ebx, [esi + 0x32]
        lea    eax, [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x4c]
        push   esi
        push   eax
        call   sub_1_2D50
        add    esp, 0xc
        or     al, al
        jne    loc_1_1007
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0x68], 7
        jne    loc_1_0FC7
        lea    eax, [esp + 0x10]
        push   2
        push   eax
        push   0x1328c                       ; -> loc_1_1328C
        call   sub_1_FEF4
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_0FC0
        mov    al, byte ptr [esi]
        and    eax, 0x7f
        cmp    eax, 0x17
        je     loc_1_0EB9
        cmp    eax, 0x72
        je     loc_1_0EB9
        mov    dword ptr [esp + 0x40], 0
        jmp    loc_1_0ECF
loc_1_0EB9:
        xor    eax, eax
        mov    ax, word ptr [esi + 0xa]
        mov    edi, 0x80
        mov    dword ptr [esp + 0x40], eax
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [esp + 0x3c], eax
loc_1_0ECF:
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax + 1], 0
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        mov    edx, dword ptr [esp + 0x40]
        add    edx, eax
        add    eax, dword ptr [esp + 0x40]
        mov    ecx, edx
        and    ecx, 0xff00
        shl    edx, 0x10
        or     ecx, edx
        shl    ecx, 8
        mov    edx, eax
        and    edx, 0xff0000
        shr    edx, 8
        or     ecx, edx
        shr    eax, 0x18
        and    eax, 0xff
        or     ecx, eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    al, byte ptr [eax + 0x39]
        or     al, al
        mov    byte ptr [ebx + 0xc], al
        je     loc_1_0F4B
        mov    dword ptr [ebx], 0x13284      ; -> loc_1_13284
        mov    eax, dword ptr [0x13288]      ; -> loc_1_13288
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [0x13288]      ; -> loc_1_13288
        mov    dword ptr [eax], ebx
        mov    dword ptr [0x13288], ebx      ; -> loc_1_13288
loc_1_0F4B:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esp + 0x30], eax
        mov    dword ptr [esp + 0x2c], 4
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [esp + 0x38], eax
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        mov    dword ptr [esp + 0x20], 0x25a0 ; -> sub_1_25A0
        mov    dword ptr [esp + 0x34], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x24], esi
        mov    eax, dword ptr [eax + 0xc]
        push   edi
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esp + 0x20], eax
        lea    eax, [esp + 0x30]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        mov    eax, dword ptr [esp + 0x4c]
        add    eax, dword ptr [esp + 0x40]
        add    eax, 4
        push   eax
        lea    eax, [esp + 0x2c]
        push   0
        push   eax
        call   sub_1_3A60
        mov    ecx, dword ptr [esp + 0x30]
        add    esp, 0x18
        add    dword ptr [ebp + 0x24], ecx
        mov    al, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Dh, 049h, 000h
loc_1_0FC0:
        mov    edx, 1
        jmp    loc_1_0FCC
loc_1_0FC7:
        mov    edx, 0xf
loc_1_0FCC:
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_0FFE
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 0x1328c      ; -> loc_1_1328C
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [0x13290]      ; -> loc_1_13290
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [0x13290]      ; -> loc_1_13290
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [0x13290], eax      ; -> loc_1_13290
loc_1_0FFE:
        push   edx
        call   sub_1_0AD0
        add    esp, 4
loc_1_1007:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  090h
sub_1_1010:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x28]
        mov    dword ptr [esp + 0x14], 0
        mov    cl, byte ptr [edi + 3]
        cmp    cl, 0xff
        je     loc_1_1090
        lea    eax, [esp + 0x14]
        push   1
        push   eax
        mov    esi, dword ptr [esp + 0x2c]
        push   ecx
        push   esi
        call   sub_1_2C70
        add    esp, 0x10
        or     al, al
        jne    loc_1_1202
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [eax + 0x44], 0
        jne    loc_1_1082
        mov    eax, dword ptr [esi + 0x2c]
        sub    eax, esi
        cmp    eax, 0x2c
        jne    loc_1_109E
loc_1_105C:
        lea    eax, [esp + 0x10]
        push   4
        push   eax
        push   0x1329c                       ; -> loc_1_1329C
        call   sub_1_FEF4
        add    esp, 0xc
        or     eax, eax
        je     loc_1_1143
        mov    al, 0x38
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_1082:
        mov    al, 0x15
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_1090:
        mov    esi, dword ptr [esp + 0x24]
        mov    eax, dword ptr [esi + 0x2c]
        sub    eax, esi
        cmp    eax, 0x2c
        je     loc_1_105C
loc_1_109E:
        lea    eax, [esi + 0x2c]
        mov    dword ptr [esp + 0x18], eax
        mov    ebp, dword ptr [eax]
        cmp    eax, ebp
        je     loc_1_10F1
        add    eax, 0
loc_1_10B0:
        lea    ebx, [ebp - 0x28]
        cmp    byte ptr [edi + 3], 0xff
        je     loc_1_10F5
        mov    eax, dword ptr [ebx + 0xc]
        push   0x10
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax + 0x14]
        add    eax, 0x34
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax + 0x14]
        add    eax, 0x34
        push   eax
        call   sub_1_0262
        add    esp, 0xc
        cmp    eax, 0x10
        je     loc_1_10F5
        mov    ebp, dword ptr [ebp]
        cmp    dword ptr [esp + 0x18], ebp
        jne    loc_1_10B0
        jmp    loc_1_10F5
loc_1_10F1:
        mov    ebx, dword ptr [esp + 0x1c]
loc_1_10F5:
        cmp    dword ptr [esp + 0x18], ebp
        je     loc_1_105C
        mov    ecx, dword ptr [ebx + 0x28]
        lea    edx, [ebx + 0x28]
        mov    eax, dword ptr [ebx + 0x2c]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    byte ptr [ebx + 0x48], 0
        lea    eax, [edi + 2]
        mov    dword ptr [ebx + 0x2c], edx
        mov    dword ptr [edx], edx
        push   eax
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        push   esi
        call   sub_1_2DC0
        add    esp, 0xc
        or     al, al
        jne    loc_1_1202
        push   0
        push   edi
        push   esi
        call   sub_1_1210
        add    esp, 0xc
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_1143:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x14], 0x1900beef
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x18], 0
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x1c], edi
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x30], 0x4655424e
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x20], 0
        xor    eax, eax
        mov    ecx, dword ptr [esp + 0x10]
        mov    ax, word ptr [edi + 8]
        mov    dword ptr [ecx + 0x2c], eax
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 0x24], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x28], 0
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax + 0x35], 0x20
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [edi + 0x32], eax
        cmp    byte ptr [edi + 3], 0xff
        je     loc_1_11E0
        mov    edx, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [edx + 0x40]
        add    edx, 0x3c
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [edx + 4], eax
        mov    al, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_11E0:
        mov    ecx, dword ptr [esi + 0x28]
        add    esi, 0x24
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 4], eax
        mov    al, 0xff
loc_1_1202:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1210:
        sub    esp, 0x1c
        push   esi
        push   edi
        lea    eax, [esp + 0xc]
        mov    esi, dword ptr [esp + 0x2c]
        push   eax
        mov    eax, dword ptr [esp + 0x2c]
        push   esi
        push   eax
        call   sub_1_2D50
        add    esp, 0xc
        or     al, al
        jne    loc_1_1409
        mov    eax, dword ptr [esp + 0xc]
        push   4
        mov    edi, dword ptr [eax + 0xc]
        lea    eax, [esp + 0xc]
        push   eax
        push   0x1329c                       ; -> loc_1_1329C
        call   sub_1_FEF4
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_1407
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x14], 0x1900beef
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x18], edi
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x1c], esi
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x30], 0x4655424e
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x20], 0
        xor    eax, eax
        mov    ecx, dword ptr [esp + 8]
        mov    ax, word ptr [esi + 8]
        mov    dword ptr [ecx + 0x2c], eax
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 0x24], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x28], 0
        mov    eax, dword ptr [esp + 0xc]
        mov    cl, byte ptr [eax + 0x38]
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [eax + 0x34], cl
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 0x35], 0x20
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 0x32], eax
        cmp    word ptr [edi + 0x32], 3
        mov    eax, dword ptr [esp + 0xc]
        je     loc_1_1315
        cmp    byte ptr [eax + 0x6c], 0
        jne    loc_1_12E9
        cmp    dword ptr [esp + 0x30], 0
        je     loc_1_12E9
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 0x35], 0x100
loc_1_12E9:
        mov    edx, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [edx + 0x20]
        add    edx, 0x1c
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx + 4], eax
        mov    al, 0xff
        pop    edi
        pop    esi
        add    esp, 0x1c
        ret    
loc_1_1315:
        mov    dword ptr [eax + 0x50], 0
        mov    dword ptr [esp + 0x18], 0x4380 ; -> sub_1_4380
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esp + 0x1c], eax
        lea    edx, [edi + 0x28]
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x60], 0
        mov    word ptr [edi + 0x32], 2
        mov    eax, dword ptr [edi + 0x2c]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    byte ptr [edi + 0x48], 0
        mov    dword ptr [edi + 0x2c], edx
        mov    dword ptr [edx], edx
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [eax + 0x3c]
        sub    ecx, dword ptr [eax + 0x44]
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        cmp    ecx, eax
        jb     loc_1_1370
        mov    ecx, eax
loc_1_1370:
        mov    dword ptr [esp + 0x10], ecx
        or     ecx, ecx
        jne    loc_1_1391
        push   0
        mov    eax, dword ptr [esp + 0xc]
        push   0
        push   eax
        call   sub_1_4380
        add    esp, 0xc
        sub    al, al
        pop    edi
        pop    esi
        add    esp, 0x1c
        ret    
loc_1_1391:
        mov    eax, dword ptr [esp + 0xc]
        cmp    byte ptr [eax + 0x6c], 0
        jne    loc_1_13AB
        cmp    dword ptr [esp + 0x30], 0
        je     loc_1_13AB
        mov    word ptr [0x12f4c], 0x100     ; -> loc_1_12F4C
loc_1_13AB:
        mov    eax, dword ptr [esp + 8]
        add    eax, 0x20
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   0x12f4c                       ; -> loc_1_12F4C
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        call   dword ptr [eax + 0x28]
        add    esp, 0x10
        mov    edi, eax
        cmp    edi, 0xff
        jne    loc_1_13E0
        mov    al, 0xff
        pop    edi
        pop    esi
        add    esp, 0x1c
        ret    
        db  090h
loc_1_13E0:
        or     edi, edi
        je     loc_1_13F8
        cmp    edi, 0xff
        je     loc_1_13F8
        push   0
        push   edi
        push   esi
        call   sub_1_25A0
        add    esp, 0xc
loc_1_13F8:
        push   edi
        call   sub_1_0AD0
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 0x1c
        ret    
loc_1_1407:
        mov    al, 0x38
loc_1_1409:
        pop    edi
        pop    esi
        add    esp, 0x1c
        ret    
        db  090h
sub_1_1410:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x18], 0x3a98
        lea    eax, [esp + 0x14]
        mov    edi, dword ptr [esp + 0x34]
        push   eax
        mov    esi, dword ptr [esp + 0x34]
        push   edi
        push   esi
        call   sub_1_2D50
        add    esp, 0xc
        or     al, al
        je     loc_1_1459
        cmp    al, 0xa
        jne    loc_1_1539
        push   0
        push   0
        push   edi
        call   sub_1_25A0
        add    esp, 0xc
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_1_1459:
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        push   esi
        call   sub_1_2FB0
        mov    eax, dword ptr [esp + 0x1c]
        add    esp, 8
        mov    ebx, dword ptr [eax + 0x10]
        mov    edx, dword ptr [eax + 0xc]
        or     edx, edx
        je     loc_1_149A
        cmp    byte ptr [edx + 0x48], 1
        jne    loc_1_149A
        cmp    word ptr [edx + 0x32], 3
        jne    loc_1_149A
        mov    eax, dword ptr [edx + 0x28]
        lea    ecx, [edx + 0x28]
        mov    ebp, dword ptr [edx + 0x2c]
        mov    dword ptr [ebp], eax
        mov    dword ptr [eax + 4], ebp
        mov    byte ptr [edx + 0x48], 0
        mov    dword ptr [edx + 0x2c], ecx
        mov    dword ptr [ecx], ecx
loc_1_149A:
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x1c], eax
        push   0
        push   0
        push   0
        lea    eax, [esp + 0x24]
        push   4
        push   eax
        lea    eax, [esp + 0x30]
        push   eax
        call   sub_1_BDC0
        add    esp, 0x18
        push   0
        push   esi
        lea    eax, [esp + 0x24]
        push   0
        push   eax
        call   sub_1_AD68
        lea    eax, [esp + 0x2c]
        add    esp, 0x10
        push   eax
        call   sub_1_A9FC
        add    esp, 4
        push   1
        mov    al, byte ptr [edi + 2]
        push   eax
        push   esi
        call   sub_1_2F70
        add    esp, 0xc
        or     ebx, ebx
        je     loc_1_152A
        cmp    dword ptr [ebx + 0x44], 0
        je     loc_1_152A
        push   ebx
        call   sub_1_1770
        add    esp, 4
        or     eax, eax
        jne    loc_1_152A
        lea    eax, [esp + 0x13]
        push   eax
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        push   esi
        call   sub_1_2E00
        add    esp, 0xc
        or     al, al
        jne    loc_1_152A
        mov    al, byte ptr [esp + 0x13]
        push   1
        push   eax
        push   ebx
        push   0
        push   esi
        call   sub_1_17A0
        add    esp, 0x14
loc_1_152A:
        push   0
        push   0
        push   edi
        call   sub_1_25A0
        add    esp, 0xc
        sub    al, al
loc_1_1539:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1550:
        sub    esp, 0x14
        push   esi
        push   edi
        mov    dword ptr [esp + 8], 0
        mov    edi, dword ptr [esp + 0x24]
        lea    eax, [esp + 8]
        push   edi
        push   eax
        lea    eax, [esp + 0x14]
        mov    esi, dword ptr [esp + 0x28]
        push   eax
        push   esi
        call   sub_1_2A90
        add    esp, 0x10
        or     al, al
        jne    loc_1_15D3
        cmp    byte ptr [edi + 0xa], 0x2a
        mov    ecx, 0
        je     loc_1_158E
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
loc_1_158E:
        mov    eax, dword ptr [esp + 8]
        push   edi
        mov    eax, dword ptr [eax + 4]
        push   eax
        push   ecx
        lea    eax, [esp + 0x18]
        push   1
        push   eax
        call   sub_1_BD08
        add    esp, 0x14
        mov    edi, eax
        or     edi, edi
        je     loc_1_15CA
        cmp    edi, 0xff
        je     loc_1_15CA
        mov    eax, dword ptr [esp + 8]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        push   esi
        call   sub_1_3080
        add    esp, 0x10
loc_1_15CA:
        push   edi
        call   sub_1_0AD0
        add    esp, 4
loc_1_15D3:
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_15E0:
        sub    esp, 0x24
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x34]
        mov    al, byte ptr [esi + 0x1a]
        lea    ecx, [esi + 0x1a]
        cmp    al, 0x2a
        je     loc_1_16B9
        or     al, al
        je     loc_1_16B9
        mov    al, byte ptr [esi]
        and    eax, 0x7f
        cmp    eax, 0x30
        je     loc_1_1616
        cmp    eax, 0x36
        je     loc_1_1620
        mov    al, 0x40
        pop    edi
        pop    esi
        add    esp, 0x24
        ret    
loc_1_1616:
        mov    word ptr [esp + 0x18], 0
        jmp    loc_1_1627
        db  090h
loc_1_1620:
        mov    word ptr [esp + 0x18], 1
loc_1_1627:
        lea    edx, [esp + 0x1a]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [edx + 0xc], eax
        mov    eax, dword ptr [esp + 0x30]
        push   esi
        push   eax
        push   0
        mov    eax, dword ptr [eax + 0x58]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_A19C
        add    esp, 0x18
        mov    edi, eax
        or     edi, edi
        je     loc_1_166D
        cmp    edi, 0xff
        jne    loc_1_1697
loc_1_166D:
        mov    eax, dword ptr [esp + 8]
        push   eax
        push   0x4330                        ; -> loc_1_4330
        push   3
        push   eax
        call   sub_1_CD0C
        mov    eax, dword ptr [esp + 0x18]
        add    esp, 0x10
        push   eax
        push   0x4770                        ; -> loc_1_4770
        push   1
        push   eax
        call   sub_1_CD0C
        add    esp, 0x10
loc_1_1697:
        or     edi, edi
        jne    loc_1_16AA
        mov    eax, dword ptr [esp + 8]
        push   eax
        push   edi
        push   esi
        call   sub_1_25A0
        add    esp, 0xc
loc_1_16AA:
        push   edi
        call   sub_1_0AD0
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 0x24
        ret    
loc_1_16B9:
        mov    al, 0x15
        pop    edi
        pop    esi
        add    esp, 0x24
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_16D0:
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x18]
        mov    al, byte ptr [esi + 0x1a]
        lea    ecx, [esi + 0x1a]
        cmp    al, 0x2a
        je     loc_1_1760
        or     al, al
        je     loc_1_1760
        lea    eax, [esp + 0xc]
        push   eax
        lea    eax, [esp + 0xf]
        push   eax
        mov    edi, dword ptr [esp + 0x1c]
        push   ecx
        push   edi
        call   sub_1_2E40
        add    esp, 0x10
        or     al, al
        jne    loc_1_1762
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        call   sub_1_1770
        add    esp, 4
        or     eax, eax
        je     loc_1_1742
        mov    al, byte ptr [esp + 0xb]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        push   esi
        push   edi
        call   sub_1_17A0
        add    esp, 0x14
        push   0
        push   0xca000001
        push   esi
        call   sub_1_25A0
        add    esp, 0xc
        sub    al, al
        pop    edi
        pop    esi
        add    esp, 8
        ret    
loc_1_1742:
        mov    al, byte ptr [esp + 0xb]
        push   1
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        push   esi
        push   edi
        call   sub_1_17A0
        add    esp, 0x14
        pop    edi
        pop    esi
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_1760:
        mov    al, 0x15
loc_1_1762:
        pop    edi
        pop    esi
        add    esp, 8
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1770:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x1c]
        add    ecx, 0x1c
        cmp    eax, ecx
        je     loc_1_178E
        mov    edx, 1
loc_1_1783:
        cmp    dword ptr [eax + 0x68], edx
        jg     loc_1_1791
        mov    eax, dword ptr [eax]
        cmp    eax, ecx
        jne    loc_1_1783
loc_1_178E:
        sub    eax, eax
        ret    
loc_1_1791:
        mov    eax, 1
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_17A0:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x30]
        mov    dword ptr [ebx + 0x44], 1
        mov    eax, dword ptr [ebx + 0x2c]
        sub    eax, ebx
        cmp    eax, 0x2c
        je     loc_1_17F3
        lea    esp, [esp]
loc_1_17C0:
        mov    esi, dword ptr [ebx + 0x2c]
        push   0
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0xc00000ca
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        push   esi
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [ebx + 0x2c]
        sub    eax, ebx
        cmp    eax, 0x2c
        jne    loc_1_17C0
loc_1_17F3:
        mov    eax, dword ptr [ebx + 0x24]
        sub    eax, ebx
        cmp    eax, 0x24
        je     loc_1_1833
loc_1_1800:
        mov    esi, dword ptr [ebx + 0x24]
        push   0
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0xc00000ca
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        push   esi
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [ebx + 0x24]
        sub    eax, ebx
        cmp    eax, 0x24
        jne    loc_1_1800
loc_1_1833:
        lea    eax, [esp + 0x10]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 0x30]
        push   0
        push   eax
        call   sub_1_2C70
        add    esp, 0x10
        or     al, al
        jne    loc_1_1892
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, dword ptr [edi + 0x24]
        add    edi, 0x24
        cmp    esi, edi
        je     loc_1_1892
loc_1_185B:
        mov    edx, dword ptr [esi]
        mov    ebp, edx
        mov    ecx, dword ptr [esi + 8]
        mov    al, byte ptr [esp + 0x34]
        cmp    byte ptr [ecx + 3], al
        jne    loc_1_188C
        mov    eax, dword ptr [esi + 4]
        push   0
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        push   0xc00000ca
        push   ecx
        call   sub_1_25A0
        add    esp, 0xc
        push   esi
        call   sub_1_0024
        add    esp, 4
loc_1_188C:
        mov    esi, ebp
        cmp    ebp, edi
        jne    loc_1_185B
loc_1_1892:
        mov    eax, dword ptr [ebx + 0x3c]
        sub    eax, ebx
        cmp    eax, 0x3c
        je     loc_1_18CA
        lea    esp, [esp]
loc_1_18A0:
        mov    eax, dword ptr [ebx + 0x3c]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        push   0xc00000ca
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        mov    eax, dword ptr [ebx + 0x3c]
        sub    eax, ebx
        cmp    eax, 0x3c
        jne    loc_1_18A0
loc_1_18CA:
        sub    esi, esi
        cmp    dword ptr [esp + 0x38], esi
        je     loc_1_191C
        mov    dword ptr [esp + 0x14], ebx
        mov    edi, dword ptr [esp + 0x2c]
        mov    eax, dword ptr [esp + 0x28]
        push   edi
        push   eax
        lea    eax, [esp + 0x1c]
        push   esi
        push   eax
        call   sub_1_AB28
        add    esp, 0x10
        mov    esi, eax
        cmp    esi, 0xff
        je     loc_1_1908
        or     edi, edi
        je     loc_1_1908
        push   0
        push   esi
        push   edi
        call   sub_1_25A0
        add    esp, 0xc
loc_1_1908:
        mov    al, byte ptr [esp + 0x34]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 0x30]
        push   eax
        call   sub_1_2F70
        add    esp, 0xc
loc_1_191C:
        push   esi
        call   sub_1_0AD0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
sub_1_1930:
        sub    esp, 0x10
        mov    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esp], eax
        push   0
        mov    eax, dword ptr [ecx + 0x36]
        add    ecx, 0x32
        push   eax
        mov    eax, dword ptr [ecx]
        push   eax
        lea    eax, [esp + 0xc]
        push   eax
        call   sub_1_C110
        add    esp, 0x10
        cmp    eax, 0xff
        jne    loc_1_1961
        sub    eax, eax
loc_1_1961:
        add    esp, 0x10
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1970:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x38]
        xor    eax, eax
        mov    ax, word ptr [ebx + 8]
        mov    ecx, dword ptr [esp + 0x3c]
        mov    dword ptr [esp + 0x14], eax
        or     ecx, ecx
        jne    loc_1_1A00
        cmp    byte ptr [ebx + 0xa], 0x2a
        jne    loc_1_1A00
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x3c]
        push   eax
        call   sub_1_D62C
        add    esp, 0xc
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_19B9
        cmp    ebp, 9
        jne    loc_1_1A6A
loc_1_19B9:
        xor    edx, edx
        mov    dx, word ptr [ebx + 8]
        mov    eax, dword ptr [esp + 0x14]
        cmp    edx, eax
        jb     loc_1_19C9
        mov    edx, eax
loc_1_19C9:
        mov    edi, dword ptr [ebx + 4]
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [ebx + 8], dx
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        call   sub_1_0024
        add    esp, 4
        push   0
        push   ebp
        push   ebx
        call   sub_1_25A0
        add    esp, 0xc
        sub    ebp, ebp
        jmp    loc_1_1A6A
        db  090h
loc_1_1A00:
        mov    dword ptr [0x133a8], 1        ; -> loc_1_133A8
        mov    word ptr [0x133ac], 0x12      ; -> loc_1_133AC
        mov    word ptr [0x133ae], 0x11      ; -> loc_1_133AE
        mov    word ptr [0x133b0], 0         ; -> loc_1_133B0
        mov    edx, 0x133b2                  ; -> loc_1_133B2
        mov    eax, dword ptr [ebx + 0xa]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebx + 0xe]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ebx + 0x12]
        lea    esi, [ebx + 0xa]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [esp + 0x28], 0x1a
        mov    dword ptr [esp + 0x2c], 0x133a8 ; -> loc_1_133A8
        push   ecx
        mov    eax, dword ptr [esp + 0x38]
        push   ebx
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        call   sub_1_CEA8
        add    esp, 0x10
        mov    ebp, eax
loc_1_1A6A:
        push   ebp
        call   sub_1_0AD0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_1_1A80:
        sub    esp, 0x18
        push   esi
        mov    dword ptr [0x133a8], 1        ; -> loc_1_133A8
        mov    word ptr [0x133ac], 0x12      ; -> loc_1_133AC
        mov    word ptr [0x133ae], 0x11      ; -> loc_1_133AE
        mov    esi, dword ptr [esp + 0x24]
        mov    word ptr [0x133b0], 0         ; -> loc_1_133B0
        mov    edx, 0x133b2                  ; -> loc_1_133B2
        mov    eax, dword ptr [esi + 0xa]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esi + 0xe]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [esi + 0x12]
        lea    ecx, [esi + 0xa]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [esp + 0x14], 0x1a
        mov    dword ptr [esp + 0x18], 0x133a8 ; -> loc_1_133A8
        push   0
        mov    eax, dword ptr [esp + 0x24]
        push   esi
        push   eax
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_1_D104
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_1B09
        push   -1
        push   0
        push   esi
        call   sub_1_25A0
        add    esp, 0xc
        sub    al, al
        pop    esi
        add    esp, 0x18
        ret    
loc_1_1B09:
        push   eax
        call   sub_1_0AD0
        add    esp, 4
        pop    esi
        add    esp, 0x18
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B20:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [esp + 0x20], 0
        mov    esi, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 8]
        and    eax, 0xffff
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [esp + 0x10], 0
        mov    dword ptr [esp + 0x1c], 0
        cmp    eax, 4
        jae    loc_1_1B76
        push   0
        mov    eax, dword ptr [esp + 0x30]
        push   0xc0000206
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_1B76:
        mov    byte ptr [esi], 0
        mov    byte ptr [esi + 1], 0
        mov    byte ptr [esi + 2], 0
        mov    byte ptr [esi + 3], 0
        lea    eax, [esp + 0x1c]
        push   0
        push   eax
        mov    edi, dword ptr [esp + 0x30]
        push   0
        push   edi
        call   sub_1_2C70
        add    esp, 0x10
        or     al, al
        jne    loc_1_1DB4
        mov    eax, dword ptr [esp + 0x2c]
        add    eax, 0x1a
        cmp    byte ptr [eax], 0x2a
        jne    loc_1_1CD0
        mov    cl, 1
        cmp    byte ptr [edi + 0x4f], cl
        jb     loc_1_1C0E
loc_1_1BBA:
        xor    eax, eax
        mov    edx, dword ptr [edi + 0x48]
        mov    al, cl
        mov    eax, dword ptr [edx + eax*4]
        or     eax, eax
        je     loc_1_1C07
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esp + 0x10], eax
        mov    edx, dword ptr [eax + 0x24]
        sub    eax, edx
        cmp    eax, -0x24
        je     loc_1_1BE9
loc_1_1BD9:
        inc    byte ptr [esi + 2]
        mov    edx, dword ptr [edx]
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, edx
        cmp    eax, -0x24
        jne    loc_1_1BD9
loc_1_1BE9:
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [eax + 0x3c]
        sub    eax, edx
        cmp    eax, -0x3c
        je     loc_1_1C07
loc_1_1BF7:
        inc    byte ptr [esi + 3]
        mov    edx, dword ptr [edx]
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, edx
        cmp    eax, -0x3c
        jne    loc_1_1BF7
loc_1_1C07:
        inc    cl
        cmp    byte ptr [edi + 0x4f], cl
        jae    loc_1_1BBA
loc_1_1C0E:
        mov    eax, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [eax + 0x24]
        sub    eax, ecx
        cmp    eax, -0x24
        je     loc_1_1C30
        lea    esp, [esp]
loc_1_1C20:
        inc    byte ptr [esi + 2]
        mov    ecx, dword ptr [ecx]
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, ecx
        cmp    eax, -0x24
        jne    loc_1_1C20
loc_1_1C30:
        mov    ecx, dword ptr [edi + 0x34]
        mov    eax, edi
        sub    eax, ecx
        cmp    eax, -0x34
        je     loc_1_1C4E
        lea    esp, [esp]
loc_1_1C40:
        inc    byte ptr [esi + 2]
        mov    ecx, dword ptr [ecx]
        mov    eax, edi
        sub    eax, ecx
        cmp    eax, -0x34
        jne    loc_1_1C40
loc_1_1C4E:
        mov    byte ptr [esi], 0xff
        sub    dword ptr [esp + 0x14], 4
        lea    eax, [esi + 4]
        mov    dword ptr [esp + 0x18], eax
        mov    ebp, dword ptr [0x13274]      ; -> loc_1_13274
        cmp    ebp, 0x13274                  ; -> loc_1_13274
        je     loc_1_1D91
loc_1_1C70:
        cmp    dword ptr [esp + 0x20], 0
        jne    loc_1_1D91
        cmp    dword ptr [ebp + 0x1c], edi
        jne    loc_1_1CBE
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, ebp
        sub    eax, ebx
        cmp    eax, -0xc
        je     loc_1_1CBE
        lea    esp, [esp]
loc_1_1C90:
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   esi
        push   ebx
        push   edi
        call   sub_1_1DC0
        add    esp, 0x14
        or     eax, eax
        je     loc_1_1CB6
        mov    ebx, dword ptr [ebx]
        mov    eax, ebp
        sub    eax, ebx
        cmp    eax, -0xc
        jne    loc_1_1C90
        jmp    loc_1_1CBE
loc_1_1CB6:
        mov    dword ptr [esp + 0x20], 9
loc_1_1CBE:
        mov    ebp, dword ptr [ebp]
        cmp    ebp, 0x13274                  ; -> loc_1_13274
        jne    loc_1_1C70
        jmp    loc_1_1D91
        db  08Bh, 0FFh
loc_1_1CD0:
        lea    ecx, [esp + 0x10]
        push   ecx
        push   esi
        push   eax
        push   edi
        call   sub_1_2E40
        add    esp, 0x10
        or     al, al
        jne    loc_1_1DB4
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [eax + 0x24]
        sub    eax, ecx
        cmp    eax, -0x24
        je     loc_1_1D06
loc_1_1CF6:
        inc    byte ptr [esi + 2]
        mov    ecx, dword ptr [ecx]
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, ecx
        cmp    eax, -0x24
        jne    loc_1_1CF6
loc_1_1D06:
        mov    eax, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [eax + 0x24]
        sub    eax, ecx
        cmp    eax, -0x24
        je     loc_1_1D24
loc_1_1D14:
        inc    byte ptr [esi + 2]
        mov    ecx, dword ptr [ecx]
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, ecx
        cmp    eax, -0x24
        jne    loc_1_1D14
loc_1_1D24:
        mov    ecx, dword ptr [edi + 0x34]
        mov    eax, edi
        sub    eax, ecx
        cmp    eax, -0x34
        je     loc_1_1D3E
loc_1_1D30:
        inc    byte ptr [esi + 2]
        mov    ecx, dword ptr [ecx]
        mov    eax, edi
        sub    eax, ecx
        cmp    eax, -0x34
        jne    loc_1_1D30
loc_1_1D3E:
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [eax + 0x3c]
        sub    eax, ecx
        cmp    eax, -0x3c
        je     loc_1_1D60
        lea    esp, [esp]
loc_1_1D50:
        inc    byte ptr [esi + 3]
        mov    ecx, dword ptr [ecx]
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, ecx
        cmp    eax, -0x3c
        jne    loc_1_1D50
loc_1_1D60:
        sub    dword ptr [esp + 0x14], 4
        lea    eax, [esi + 4]
        mov    dword ptr [esp + 0x18], eax
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x18]
        push   esi
        push   eax
        push   edi
        call   sub_1_1DC0
        add    esp, 0x14
        or     eax, eax
        jne    loc_1_1D91
        mov    dword ptr [esp + 0x20], 9
loc_1_1D91:
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        shl    eax, 2
        lea    eax, [eax + eax*8]
        add    eax, 4
        push   eax
        mov    eax, dword ptr [esp + 0x24]
        push   eax
        mov    eax, dword ptr [esp + 0x34]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        sub    al, al
loc_1_1DB4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 064h, 024h, 000h
sub_1_1DC0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x1c]
        mov    eax, ebx
        mov    edi, dword ptr [ebx + 0x1c]
        sub    eax, edi
        cmp    eax, -0x1c
        je     loc_1_1EE9
        mov    esi, dword ptr [esp + 0x24]
        mov    ebp, dword ptr [esp + 0x28]
loc_1_1DE3:
        mov    dword ptr [esp + 0x10], 0
        mov    eax, dword ptr [ebp]
        cmp    eax, 0x24
        jb     loc_1_1F08
        sub    eax, 0x24
        mov    dword ptr [ebp], eax
        mov    eax, dword ptr [esp + 0x20]
        inc    byte ptr [eax + 1]
        mov    eax, dword ptr [esi]
        push   eax
        mov    byte ptr [eax + 0x22], 0
        mov    byte ptr [eax + 0x23], 0
        mov    eax, dword ptr [esp + 0x1c]
        push   edi
        push   eax
        call   sub_1_2DC0
        add    esp, 0xc
        mov    ecx, dword ptr [edi + 0x1c]
        mov    eax, edi
        sub    eax, ecx
        cmp    eax, -0x1c
        je     loc_1_1E3D
        mov    eax, dword ptr [esi]
        add    eax, 0x22
        mov    edi, edi
loc_1_1E30:
        inc    byte ptr [eax]
        mov    ecx, dword ptr [ecx]
        mov    edx, edi
        sub    edx, ecx
        cmp    edx, -0x1c
        jne    loc_1_1E30
loc_1_1E3D:
        mov    eax, dword ptr [edi + 0x68]
        sub    eax, 3
        cmp    eax, 6
        ja     loc_1_1E50
        jmp    dword ptr [eax*4 + 0x1f14]    ; -> obj1:0x1F14
        db  090h
loc_1_1E50:
        mov    eax, dword ptr [esi]
        mov    byte ptr [eax + 1], 6
        jmp    loc_1_1E8C
loc_1_1E58:
        cmp    byte ptr [edi + 0x6c], 0
        mov    eax, dword ptr [esi]
        je     loc_1_1E66
        mov    byte ptr [eax + 1], 2
        jmp    loc_1_1E8C
loc_1_1E66:
        mov    byte ptr [eax + 1], 1
        jmp    loc_1_1E8C
        db  08Dh, 064h, 024h, 000h
loc_1_1E70:
        mov    eax, dword ptr [esi]
        mov    byte ptr [eax + 1], 1
        jmp    loc_1_1E8C
loc_1_1E78:
        mov    eax, dword ptr [esi]
        mov    byte ptr [eax + 1], 2
loc_1_1E7E:
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 0x10], 1
        mov    byte ptr [eax + 1], 3
loc_1_1E8C:
        mov    eax, dword ptr [ebx + 0x10]
        mov    ecx, dword ptr [esi]
        mov    edx, dword ptr [eax + 0x14]
        add    ecx, 2
        mov    eax, dword ptr [edx + 0x34]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 0x38]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 0x3c]
        add    edx, 0x34
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [ecx + 0xc], eax
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_1ED7
        lea    eax, [edi + 0x28]
        mov    ecx, dword ptr [esi]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx + 0x12], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x16], edx
        add    ecx, 0x12
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], edx
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ecx + 0xc], eax
loc_1_1ED7:
        add    dword ptr [esi], 0x24
        mov    edi, dword ptr [edi]
        mov    eax, ebx
        sub    eax, edi
        cmp    eax, -0x1c
        jne    loc_1_1DE3
loc_1_1EE9:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_1EF6:
        mov    eax, dword ptr [esi]
        mov    byte ptr [eax + 1], 4
        jmp    loc_1_1E8C
        db  08Bh, 0FFh
loc_1_1F00:
        mov    eax, dword ptr [esi]
        mov    byte ptr [eax + 1], 5
        jmp    loc_1_1E8C
loc_1_1F08:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
        ; ---- dispatch table (7 entries) ----
        dd  loc_1_1E58
        dd  loc_1_1E70
        dd  loc_1_1E70
        dd  loc_1_1E78
        dd  loc_1_1E7E
        dd  loc_1_1EF6
        dd  loc_1_1F00
sub_1_1F30:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x2c]
        mov    edi, dword ptr [esp + 0x28]
        lea    esi, [ebp + 0x32]
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 4], 0
        mov    byte ptr [esi + 8], 0
        mov    eax, dword ptr [edi + 0x24]
        sub    eax, edi
        cmp    eax, 0x24
        je     loc_1_1F8A
loc_1_1F60:
        mov    eax, dword ptr [edi + 0x24]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        push   0xc000020c
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        mov    eax, dword ptr [edi + 0x24]
        sub    eax, edi
        cmp    eax, 0x24
        jne    loc_1_1F60
loc_1_1F8A:
        mov    eax, dword ptr [edi + 0x34]
        sub    eax, edi
        cmp    eax, 0x34
        je     loc_1_1FC7
loc_1_1F94:
        mov    ebx, dword ptr [edi + 0x34]
        push   0
        mov    eax, dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0xc00000ca
        mov    eax, dword ptr [ebx + 8]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        push   ebx
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [edi + 0x34]
        sub    eax, edi
        cmp    eax, 0x34
        jne    loc_1_1F94
loc_1_1FC7:
        mov    bl, 1
        cmp    byte ptr [edi + 0x4f], bl
        jb     loc_1_203F
        mov    edi, edi
loc_1_1FD0:
        xor    eax, eax
        mov    al, bl
        lea    ecx, [eax*4]
        mov    eax, dword ptr [edi + 0x48]
        cmp    dword ptr [ecx + eax], 0
        je     loc_1_2038
        mov    dword ptr [esp + 0x10], 0x3a98
        mov    eax, dword ptr [edi + 0x48]
        mov    eax, dword ptr [ecx + eax]
        mov    dword ptr [esp + 0x14], eax
        push   ebp
        push   0
        push   0
        lea    eax, [esp + 0x1c]
        push   4
        push   eax
        inc    dword ptr [esi]
        lea    eax, [esp + 0x28]
        push   eax
        call   sub_1_BDC0
        add    esp, 0x18
        cmp    eax, 0xff
        je     loc_1_2038
        dec    dword ptr [esi]
        push   0
        push   edi
        lea    eax, [esp + 0x1c]
        push   0
        push   eax
        call   sub_1_AD68
        add    esp, 0x10
        push   1
        push   ebx
        push   edi
        call   sub_1_2F70
        add    esp, 0xc
loc_1_2038:
        inc    bl
        cmp    byte ptr [edi + 0x4f], bl
        jae    loc_1_1FD0
loc_1_203F:
        cmp    dword ptr [esi], 0
        jne    loc_1_205C
        mov    dword ptr [esi], 0xffffffff
        push   ebp
        push   edi
        call   sub_1_2070
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_205C:
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2070:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x3c]
        add    ebp, 0x32
        mov    esi, dword ptr [esp + 0x38]
        mov    bl, 1
        cmp    byte ptr [esi + 0x4f], bl
        jb     loc_1_20C0
loc_1_2089:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0x48]
        mov    al, bl
        mov    eax, dword ptr [ecx + eax*4]
        or     eax, eax
        je     loc_1_20B9
        mov    dword ptr [esp + 0x24], eax
        push   0
        push   esi
        lea    eax, [esp + 0x2c]
        push   0
        push   eax
        call   sub_1_AD68
        add    esp, 0x10
        push   1
        push   ebx
        push   esi
        call   sub_1_2F70
        add    esp, 0xc
loc_1_20B9:
        inc    bl
        cmp    byte ptr [esi + 0x4f], bl
        jae    loc_1_2089
loc_1_20C0:
        sub    bl, bl
        cmp    byte ptr [esi + 0x4e], bl
        jb     loc_1_210E
loc_1_20C7:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0x44]
        mov    al, bl
        mov    eax, dword ptr [ecx + eax*4]
        or     eax, eax
        je     loc_1_2107
        mov    dword ptr [esp + 0x14], eax
        inc    dword ptr [ebp + 4]
        mov    eax, dword ptr [esp + 0x3c]
        push   eax
        push   esi
        lea    eax, [esp + 0x1c]
        push   0
        push   eax
        call   sub_1_AB28
        add    esp, 0x10
        cmp    eax, 0xff
        je     loc_1_20FB
        dec    dword ptr [ebp + 4]
loc_1_20FB:
        push   0
        push   ebx
        push   esi
        call   sub_1_2F70
        add    esp, 0xc
loc_1_2107:
        inc    bl
        cmp    byte ptr [esi + 0x4e], bl
        jae    loc_1_20C7
loc_1_210E:
        mov    eax, dword ptr [esp + 0x3c]
        mov    bl, byte ptr [eax + 2]
        cmp    byte ptr [esi + 0x4f], bl
        je     loc_1_2179
        or     bl, bl
        jne    loc_1_2120
        mov    bl, 6
loc_1_2120:
        mov    al, bl
        and    eax, 0xffff00ff
        mov    word ptr [esp + 0x10], ax
        lea    eax, [eax*4 + 4]
        and    eax, 0xffff
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_1_2150
        mov    byte ptr [ebp + 8], 0x38
        jmp    loc_1_2179
        db  08Dh, 049h, 000h
loc_1_2150:
        mov    eax, dword ptr [esi + 0x48]
        push   eax
        call   sub_1_0024
        add    esp, 4
        mov    byte ptr [esi + 0x4f], bl
        mov    dword ptr [esi + 0x48], edi
        mov    eax, dword ptr [esp + 0x10]
        and    eax, 0xffff
        lea    ecx, [eax*4 + 4]
        sub    eax, eax
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
loc_1_2179:
        mov    byte ptr [esi + 0x4c], 1
        mov    byte ptr [esi + 0x4d], 1
        push   esi
        call   sub_1_FBB8
        add    esp, 4
        or     eax, eax
        je     loc_1_2197
        push   esi
        call   sub_1_FBB8
        add    esp, 4
loc_1_2197:
        cmp    dword ptr [ebp + 4], 0
        jne    loc_1_21AE
        push   0
        mov    eax, dword ptr [esp + 0x40]
        push   0
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
loc_1_21AE:
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_21C0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x20]
        sub    bl, bl
        mov    esi, dword ptr [ecx + 4]
        mov    al, byte ptr [ecx + 0x30]
        cmp    byte ptr [esi + 0x30], al
        je     loc_1_21E2
        mov    al, 0x23
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_21E2:
        mov    ecx, dword ptr [esp + 0x20]
        cmp    byte ptr [ecx + 1], 0xff
        je     loc_1_21F6
        mov    al, 0x24
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_21F6:
        mov    al, byte ptr [esi]
        and    eax, 0x7f
        cmp    eax, 0x17
        jg     loc_1_2234
        je     loc_1_236A
        cmp    eax, 0x10
        je     loc_1_2266
        cmp    eax, 0x11
        je     loc_1_22F2
        cmp    eax, 0x14
        jl     loc_1_257B
        cmp    eax, 0x15
        jle    loc_1_236A
        cmp    eax, 0x16
        je     loc_1_2415
        jmp    loc_1_257B
loc_1_2234:
        cmp    eax, 0x21
        je     loc_1_2490
        cmp    eax, 0x23
        je     loc_1_2510
        cmp    eax, 0x35
        je     loc_1_257B
        cmp    eax, 0x71
        jl     loc_1_257B
        cmp    eax, 0x72
        jle    loc_1_236A
        jmp    loc_1_257B
loc_1_2266:
        lea    eax, [esp + 0x10]
        push   eax
        push   0
        lea    eax, [esi + 0x1a]
        mov    ebp, dword ptr [esp + 0x24]
        push   eax
        push   ebp
        call   sub_1_2E40
        add    esp, 0x10
        or     al, al
        mov    bl, 0x24
        je     loc_1_2289
        jmp    loc_1_257D
loc_1_2289:
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [eax + 0x1c]
        sub    eax, ecx
        cmp    eax, -0x1c
        je     loc_1_257D
        add    eax, 0
loc_1_22A0:
        mov    dword ptr [esp + 0x14], ecx
        cmp    dword ptr [ecx + 0x54], esi
        je     loc_1_22C0
        mov    ecx, dword ptr [ecx]
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, ecx
        cmp    eax, -0x1c
        jne    loc_1_22A0
        jmp    loc_1_257D
        db  005h, 000h, 000h, 000h, 000h
loc_1_22C0:
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax + 0x60]
        cmp    dword ptr [ecx + 0x68], 6
        jl     loc_1_22D8
        mov    bl, 0x24
        jmp    loc_1_257D
loc_1_22D8:
        or     word ptr [eax + 0x4c], 0x40
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [eax + 0x5c], ecx
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_22F2:
        lea    eax, [esp + 0x10]
        push   eax
        push   0
        lea    eax, [esi + 0x1a]
        mov    ebp, dword ptr [esp + 0x24]
        push   eax
        push   ebp
        call   sub_1_2E40
        add    esp, 0x10
        or     al, al
        mov    bl, 0x24
        je     loc_1_2315
        jmp    loc_1_257D
loc_1_2315:
        mov    eax, dword ptr [esp + 0x10]
        mov    edi, dword ptr [eax + 0x2c]
        sub    eax, edi
        cmp    eax, -0x2c
        je     loc_1_257D
loc_1_2327:
        cmp    dword ptr [edi + 8], esi
        je     loc_1_2340
        mov    edi, dword ptr [edi]
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, edi
        cmp    eax, -0x2c
        jne    loc_1_2327
        jmp    loc_1_257D
        db  08Bh, 0FFh
loc_1_2340:
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0xc0000120
        sub    bl, bl
        push   esi
        call   sub_1_25A0
        add    esp, 0xc
        push   edi
        call   sub_1_0024
        add    esp, 4
        jmp    loc_1_257D
loc_1_236A:
        lea    eax, [esp + 0x14]
        push   eax
        mov    ebp, dword ptr [esp + 0x20]
        push   esi
        push   ebp
        call   sub_1_2D50
        add    esp, 0xc
        or     al, al
        jne    loc_1_257D
        mov    al, byte ptr [esi]
        and    eax, 0x7f
        cmp    eax, 0x15
        mov    bl, 0x24
        jne    loc_1_23D3
        mov    eax, dword ptr [esp + 0x14]
        mov    edx, dword ptr [eax + 0x1c]
        sub    eax, edx
        cmp    eax, -0x1c
        je     loc_1_23D3
        nop    
loc_1_23A0:
        cmp    dword ptr [edx + 0x1c], esi
        je     loc_1_23B4
        mov    edx, dword ptr [edx]
        mov    eax, dword ptr [esp + 0x14]
        sub    eax, edx
        cmp    eax, -0x1c
        jne    loc_1_23A0
        jmp    loc_1_23D3
loc_1_23B4:
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        sub    bl, bl
        push   0
        push   0xc0000120
        mov    eax, dword ptr [edx + 0x1c]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
loc_1_23D3:
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        push   ebp
        call   sub_1_2FB0
        add    esp, 8
        push   0
        mov    eax, dword ptr [esp + 0x18]
        push   0
        push   eax
        push   ebp
        call   sub_1_3080
        mov    eax, dword ptr [esp + 0x24]
        add    esp, 0x10
        test   byte ptr [eax + 0x3a], 1
        je     loc_1_257D
        push   1
        mov    al, byte ptr [esi + 2]
        push   eax
        push   ebp
        call   sub_1_2F70
        add    esp, 0xc
        jmp    loc_1_257D
loc_1_2415:
        mov    cl, byte ptr [esi + 3]
        cmp    cl, 0xff
        jne    loc_1_2426
        mov    ebp, dword ptr [esp + 0x1c]
        add    ebp, 0x24
        jmp    loc_1_244C
loc_1_2426:
        lea    eax, [esp + 0x10]
        push   1
        push   eax
        mov    ebp, dword ptr [esp + 0x24]
        push   ecx
        push   ebp
        call   sub_1_2C70
        add    esp, 0x10
        mov    bl, al
        or     bl, bl
        jne    loc_1_257D
        mov    ebp, dword ptr [esp + 0x10]
        add    ebp, 0x3c
loc_1_244C:
        mov    bl, 0x24
        mov    edx, dword ptr [ebp]
        cmp    edx, ebp
        je     loc_1_257D
loc_1_2459:
        cmp    dword ptr [edx + 0x1c], esi
        je     loc_1_2469
        mov    edx, dword ptr [edx]
        cmp    edx, ebp
        jne    loc_1_2459
        jmp    loc_1_257D
loc_1_2469:
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        sub    bl, bl
        push   0
        push   0xc0000120
        mov    eax, dword ptr [edx + 0x1c]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        jmp    loc_1_257D
        db  08Dh, 049h, 000h
loc_1_2490:
        mov    cl, byte ptr [esi + 3]
        cmp    cl, 0xff
        jne    loc_1_24A1
        mov    ebp, dword ptr [esp + 0x1c]
        add    ebp, 0x34
        jmp    loc_1_24C7
loc_1_24A1:
        lea    eax, [esp + 0x10]
        push   1
        push   eax
        mov    ebp, dword ptr [esp + 0x24]
        push   ecx
        push   ebp
        call   sub_1_2C70
        add    esp, 0x10
        mov    bl, al
        or     bl, bl
        jne    loc_1_257D
        mov    ebp, dword ptr [esp + 0x10]
        add    ebp, 0x24
loc_1_24C7:
        mov    bl, 0x24
        mov    edi, dword ptr [ebp]
        cmp    edi, ebp
        je     loc_1_257D
loc_1_24D4:
        cmp    dword ptr [edi + 8], esi
        je     loc_1_24E4
        mov    edi, dword ptr [edi]
        cmp    edi, ebp
        jne    loc_1_24D4
        jmp    loc_1_257D
loc_1_24E4:
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0xc0000120
        sub    bl, bl
        push   esi
        call   sub_1_25A0
        add    esp, 0xc
        push   edi
        call   sub_1_0024
        add    esp, 4
        jmp    loc_1_257D
        db  08Bh, 0FFh
loc_1_2510:
        lea    eax, [esp + 0x10]
        push   0
        push   eax
        mov    ebp, dword ptr [esp + 0x24]
        push   0
        push   ebp
        call   sub_1_2C70
        add    esp, 0x10
        mov    bl, al
        or     bl, bl
        jne    loc_1_257D
        mov    eax, dword ptr [esp + 0x10]
        mov    bl, 0x24
        mov    edx, dword ptr [eax + 0x24]
        sub    eax, edx
        cmp    eax, -0x24
        je     loc_1_257D
        lea    esp, [esp]
loc_1_2540:
        cmp    dword ptr [edx + 8], esi
        je     loc_1_2554
        mov    edx, dword ptr [edx]
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, edx
        cmp    eax, -0x24
        jne    loc_1_2540
        jmp    loc_1_257D
loc_1_2554:
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   edx
        call   sub_1_0024
        add    esp, 4
        push   0
        push   0xc0000120
        push   esi
        call   sub_1_25A0
        add    esp, 0xc
        sub    bl, bl
        jmp    loc_1_257D
loc_1_257B:
        mov    bl, 0x26
loc_1_257D:
        push   0
        xor    eax, eax
        mov    al, bl
        mov    ecx, dword ptr [esp + 0x24]
        push   eax
        push   ecx
        call   sub_1_25A0
        add    esp, 0xc
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_1_25A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        sub    bl, bl
        mov    dword ptr [ebp - 4], ecx
        or     ecx, ecx
        je     loc_1_29F7
        mov    al, byte ptr [ecx]
        and    al, 0x80
        cmp    al, 1
        push   ecx
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 0x10], eax
        call   sub_1_0A90
        add    esp, 4
        mov    esi, eax
        mov    ecx, dword ptr [ebp + 0xc]
        or     ecx, ecx
        je     loc_1_25EF
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0x7f
        cmp    eax, 0x35
        je     loc_1_25EF
        push   ecx
        call   sub_1_0AD0
        add    esp, 4
        mov    bl, al
loc_1_25EF:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0x7f
        cmp    eax, 0x32
        jg     loc_1_2613
        je     loc_1_2911
        sub    eax, 0x10
        cmp    eax, 0x20
        ja     loc_1_263D
        shl    eax, 2
        jmp    loc_1_2A00
loc_1_2613:
        cmp    eax, 0x36
        jg     loc_1_26A0
        je     loc_1_28D0
        cmp    eax, 0x33
        je     loc_1_2975
        cmp    eax, 0x34
        je     loc_1_28B2
        cmp    eax, 0x35
        je     loc_1_299A
loc_1_263D:
        mov    eax, dword ptr [ebp - 4]
        inc    eax
        cmp    byte ptr [eax], 0xff
        jne    loc_1_2665
        mov    byte ptr [eax], bl
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 0x10], 0
        mov    byte ptr [eax + 0x31], bl
        je     loc_1_266F
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x2c]
        or     eax, eax
        je     loc_1_2665
        push   ebp
        mov    ebx, dword ptr [ebp - 4]
        call   eax
        pop    ebp
loc_1_2665:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_29F7
loc_1_266F:
        mov    eax, dword ptr [0x132ac]      ; -> loc_1_132AC
        cmp    eax, 0x132ac                  ; -> loc_1_132AC
        je     loc_1_29AB
        nop    
loc_1_2680:
        mov    esi, eax
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xc], ecx
        je     loc_1_29AE
        sub    esi, esi
        mov    eax, dword ptr [eax]
        cmp    eax, 0x132ac                  ; -> loc_1_132AC
        jne    loc_1_2680
        jmp    loc_1_29AE
        db  08Bh, 0FFh
loc_1_26A0:
        cmp    eax, 0x70
        je     loc_1_2964
        cmp    eax, 0x71
        jl     loc_1_263D
        cmp    eax, 0x72
        jle    loc_1_2780
        cmp    eax, 0x78
        je     loc_1_2975
        jmp    loc_1_263D
loc_1_26C5:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x32
        mov    dword ptr [ebp - 8], eax
        or     bl, bl
        je     loc_1_26E7
        push   0
        push   eax
        mov    eax, dword ptr [eax + 8]
        push   eax
        push   esi
        call   sub_1_3080
        add    esp, 0x10
        jmp    loc_1_263D
loc_1_26E7:
        mov    edi, dword ptr [ebp + 0x10]
        push   1
        push   edi
        mov    eax, dword ptr [ebp - 4]
        add    eax, 2
        push   eax
        push   esi
        call   sub_1_2EF0
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_2713
        push   0
        push   0
        push   edi
        push   esi
        call   sub_1_3080
        add    esp, 0x10
        mov    bl, 0x11
        jmp    loc_1_2757
loc_1_2713:
        mov    eax, dword ptr [ebp - 8]
        cmp    byte ptr [eax + 0xc], 0
        je     loc_1_273C
        lea    edx, [edi + 0x28]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx + 0xa], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 0xe], eax
        add    ecx, 0xa
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [ecx + 0xc], eax
loc_1_273C:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x2a]
        mov    byte ptr [edi + 0x38], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x2b]
        mov    byte ptr [edi + 0x39], al
        inc    dword ptr [edi + 0x64]
        mov    word ptr [edi + 0x3a], 0
loc_1_2757:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_31C0
        add    esp, 4
        cmp    dword ptr [edi + 0x68], 5
        jne    loc_1_263D
        push   0
        push   edi
        call   sub_1_1930
        add    esp, 8
        jmp    loc_1_263D
        db  08Dh, 049h, 000h
loc_1_2780:
        mov    edi, dword ptr [ebp - 4]
        add    edi, 0x32
        or     bl, bl
        je     loc_1_2829
        cmp    bl, 5
        je     loc_1_27A0
        cmp    byte ptr [edi + 0xc], 1
        jne    loc_1_27A0
        mov    bl, 5
        jmp    loc_1_2839
loc_1_27A0:
        mov    cl, byte ptr [edi + 0xc]
        lea    eax, [ecx - 1]
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ebp - 0x14], eax
        or     cl, cl
        je     loc_1_27BD
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
loc_1_27BD:
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_1_2D50
        add    esp, 0xc
        or     al, al
        je     loc_1_27E3
        mov    eax, dword ptr [edi + 8]
        push   eax
        call   sub_1_0024
        add    esp, 4
        jmp    loc_1_263D
loc_1_27E3:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   esi
        call   sub_1_2FB0
        add    esp, 8
        mov    eax, dword ptr [ebp - 0xc]
        test   byte ptr [eax + 0x3a], 1
        je     loc_1_280B
        mov    eax, dword ptr [ebp - 4]
        push   1
        mov    al, byte ptr [eax + 2]
        push   eax
        push   esi
        call   sub_1_2F70
        add    esp, 0xc
loc_1_280B:
        push   0
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   esi
        call   sub_1_3080
        add    esp, 0x10
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_1_2839
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_2829:
        cmp    byte ptr [edi + 0xc], 0
        je     loc_1_2839
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
loc_1_2839:
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [eax], 0x1328c      ; -> loc_1_1328C
        mov    ecx, dword ptr [edi + 8]
        mov    eax, dword ptr [0x13290]      ; -> loc_1_13290
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [0x13290]      ; -> loc_1_13290
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ecx], eax
        mov    dword ptr [0x13290], eax      ; -> loc_1_13290
        jmp    loc_1_263D
loc_1_2862:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x32]
        mov    dword ptr [eax], 0x1329c      ; -> loc_1_1329C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0x132a0]      ; -> loc_1_132A0
        mov    eax, dword ptr [eax + 0x32]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [ebp - 4]
        or     bl, bl
        mov    ecx, dword ptr [0x132a0]      ; -> loc_1_132A0
        mov    eax, dword ptr [eax + 0x32]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x32]
        mov    dword ptr [0x132a0], eax      ; -> loc_1_132A0
        je     loc_1_28A3
        cmp    bl, 6
        jne    loc_1_263D
loc_1_28A3:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0x10]
        mov    word ptr [eax + 8], di
        jmp    loc_1_263D
loc_1_28B2:
        or     bl, bl
        je     loc_1_28BF
        cmp    bl, 6
        jne    loc_1_263D
loc_1_28BF:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0x10]
        mov    word ptr [eax + 8], di
        jmp    loc_1_263D
        db  08Bh, 0FFh
loc_1_28D0:
        or     bl, bl
        jne    loc_1_263D
        mov    edi, dword ptr [ebp + 0x10]
        push   0
        push   edi
        mov    eax, dword ptr [ebp - 4]
        add    eax, 3
        push   eax
        push   esi
        call   sub_1_2EF0
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_263D
        mov    bl, 0xe
        mov    dword ptr [ebp - 0x28], edi
        push   0
        push   esi
        push   0
        lea    eax, [ebp - 0x28]
        push   eax
        call   sub_1_AB28
        add    esp, 0x10
        jmp    loc_1_263D
loc_1_2911:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 0x32]
        add    ecx, 0x32
        cmp    eax, -1
        jne    loc_1_292A
        cmp    dword ptr [ecx + 4], 0
        je     loc_1_295D
        cmp    eax, -1
        je     loc_1_2950
loc_1_292A:
        dec    eax
        mov    dword ptr [ecx], eax
        jne    loc_1_29F7
        mov    dword ptr [ecx], 0xffffffff
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_1_2070
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 049h, 000h
loc_1_2950:
        mov    eax, dword ptr [ecx + 4]
        dec    eax
        mov    dword ptr [ecx + 4], eax
        jne    loc_1_29F7
loc_1_295D:
        or     bl, bl
        jne    loc_1_2964
        mov    bl, byte ptr [ecx + 8]
loc_1_2964:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x31], bl
        jmp    loc_1_2665
loc_1_2975:
        or     bl, bl
        je     loc_1_2982
        cmp    bl, 6
        jne    loc_1_263D
loc_1_2982:
        mov    edi, dword ptr [ebp + 0x10]
        cmp    edi, -1
        je     loc_1_263D
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 8], di
        jmp    loc_1_263D
loc_1_299A:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x31], bl
        jmp    loc_1_263D
loc_1_29AB:
        mov    esi, dword ptr [ebp - 0x18]
loc_1_29AE:
        or     esi, esi
        je     loc_1_29F7
        cmp    dword ptr [esi + 0x18], 0
        je     loc_1_29F0
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_003C
        add    esp, 8
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_0024
        add    esp, 4
        push   esi
        call   sub_1_0024
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_29F0:
        mov    dword ptr [esi + 0x14], 1
loc_1_29F7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_2A00:
        lds    esp, ptr [esi]
        add    byte ptr [eax], al
        lds    esp, ptr [esi]
        add    byte ptr [eax], al
        cmp    eax, 0x3d000026               ; -> loc_1_263D
        add    byte ptr es:[eax], al
        and    byte ptr [edi], 0
        add    byte ptr [edx + 0x28], ah     ; -> loc_1_2862
        add    byte ptr [eax], al
        bound  ebp, qword ptr [eax]
        add    byte ptr [eax], al
        and    byte ptr [edi], 0
        add    byte ptr [0x3d000026], bh     ; -> loc_1_263D
        add    byte ptr es:[eax], al
        cmp    eax, 0x3d000026               ; -> loc_1_263D
        add    byte ptr es:[eax], al
        cmp    eax, 0x3d000026               ; -> loc_1_263D
        add    byte ptr es:[eax], al
        cmp    eax, 0x3d000026               ; -> loc_1_263D
        add    byte ptr es:[eax], al
        cmp    eax, 0xb2000026               ; -> loc_1_28B2
        sub    byte ptr [eax], al
        add    byte ptr [0xb2000026], bh     ; -> loc_1_263D
        sub    byte ptr [eax], al
        add    byte ptr [0x3d000026], bh     ; -> loc_1_263D
        add    byte ptr es:[eax], al
        cmp    eax, 0x3d000026               ; -> loc_1_263D
        add    byte ptr es:[eax], al
        cmp    eax, 0x3d000026               ; -> loc_1_263D
        add    byte ptr es:[eax], al
        cmp    eax, 0x3d000026               ; -> loc_1_263D
        add    byte ptr es:[eax], al
        cmp    eax, 0x3d000026               ; -> loc_1_263D
        add    byte ptr es:[eax], al
        cmp    eax, 0x3d000026               ; -> loc_1_263D
        add    byte ptr es:[eax], al
        shr    byte ptr [eax], 1
        add    byte ptr [eax], al
        add    eax, 0
        lea    esp, [esp]
sub_1_2A90:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x30]
        lea    eax, [esp + 0x14]
        mov    edi, dword ptr [esp + 0x34]
        push   eax
        push   0
        lea    eax, [edi + 0x1a]
        push   eax
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [esp + 0x34]
        push   eax
        call   sub_1_2E40
        add    esp, 0x10
        or     al, al
        jne    loc_1_2B6C
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [eax + 0x44], 0
        je     loc_1_2AD9
        mov    al, 0x15
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_2AD9:
        mov    eax, dword ptr [esp + 0x28]
        push   eax
        mov    ebx, dword ptr [esp + 0x30]
        push   0
        push   ebx
        call   sub_1_A810
        add    esp, 0xc
        or     eax, eax
        je     loc_1_2B02
        push   eax
        call   sub_1_0AD0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_2B02:
        mov    ebp, dword ptr [ebx]
        push   0
        mov    dword ptr [ebp + 0x24], ebp
        mov    eax, dword ptr [esp + 0x18]
        push   eax
        push   ebx
        call   sub_1_A978
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_2B4A
        lea    ecx, [edi + 0x32]
        mov    dword ptr [esi], ecx
        mov    al, byte ptr [edi + 0xa]
        sub    al, 0x2a
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   ecx
        call   sub_1_30E0
        mov    dword ptr [esp + 0x18], eax
        add    esp, 8
        or     eax, eax
        je     loc_1_2B74
        push   ebx
        call   sub_1_A9FC
        add    esp, 4
loc_1_2B4A:
        mov    eax, dword ptr [esp + 0x28]
        push   0
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   ebx
        call   sub_1_AD68
        mov    eax, dword ptr [esp + 0x20]
        add    esp, 0x10
        push   eax
        call   sub_1_0AD0
        add    esp, 4
loc_1_2B6C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_2B74:
        cmp    byte ptr [edi + 0xa], 0x2a
        je     loc_1_2BDD
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 0x14]
        mov    word ptr [eax + 4], 0x12
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax]
        lea    ecx, [edi + 0xa]
        mov    eax, dword ptr [eax + 0x14]
        mov    word ptr [eax + 6], 0x11
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 0x14]
        mov    word ptr [eax + 8], 0
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax]
        mov    edx, dword ptr [eax + 0x14]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx + 0xa], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 0xe], eax
        add    edx, 0xa
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [edx + 0xc], eax
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 0x10], 0x1a
loc_1_2BDD:
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0x14]
        mov    word ptr [eax + 4], 0x12
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 4]
        lea    ecx, [edi + 0x1a]
        mov    eax, dword ptr [eax + 0x14]
        mov    word ptr [eax + 6], 0x11
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0x14]
        mov    word ptr [eax + 8], 0
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 4]
        mov    edx, dword ptr [eax + 0x14]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx + 0xa], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 0xe], eax
        add    edx, 0xa
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [edx + 0xc], eax
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x10], 0x1a
        mov    cl, byte ptr [edi + 0xa]
        sub    cl, 0x2a
        cmp    cl, 1
        sbb    cl, cl
        neg    cl
        mov    byte ptr [eax + 0xc], cl
        mov    dword ptr [eax + 8], ebp
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2C70:
        sub    esp, 0x14
        push   esi
        mov    esi, dword ptr [esp + 0x1c]
        or     esi, esi
        jne    loc_1_2C83
        mov    al, 0x40
        pop    esi
        add    esp, 0x14
        ret    
loc_1_2C83:
        cmp    dword ptr [esp + 0x28], 0
        je     loc_1_2CB7
        mov    al, byte ptr [esp + 0x20]
        cmp    byte ptr [esi + 0x4e], al
        jb     loc_1_2CB0
        xor    ecx, ecx
        mov    cl, al
        mov    eax, dword ptr [esi + 0x44]
        mov    eax, dword ptr [eax + ecx*4]
        or     eax, eax
        je     loc_1_2CB0
        mov    edx, dword ptr [esp + 0x24]
        pop    esi
        mov    dword ptr [edx], eax
        sub    al, al
        add    esp, 0x14
        ret    
        db  08Bh, 0FFh
loc_1_2CB0:
        mov    al, 0x13
        pop    esi
        add    esp, 0x14
        ret    
loc_1_2CB7:
        lea    eax, [esp + 8]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0xc], 0
        lea    eax, [esp + 4]
        mov    byte ptr [esp + 8], 0x2a
        push   eax
        mov    eax, dword ptr [0x132d8]      ; -> loc_1_132D8
        push   eax
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [0x132b4]      ; -> loc_1_132B4
        push   eax
        call   sub_1_6954
        add    esp, 0x10
        or     eax, eax
        je     loc_1_2D10
        cmp    eax, 0xff
        je     loc_1_2D10
        mov    al, 0x13
        pop    esi
        add    esp, 0x14
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_2D10:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [eax + 0xc]
        add    eax, 0xc
        cmp    ecx, eax
        je     loc_1_2D3B
loc_1_2D21:
        cmp    dword ptr [ecx + 0x80], esi
        je     loc_1_2D31
        mov    ecx, dword ptr [ecx]
        cmp    ecx, eax
        jne    loc_1_2D21
        jmp    loc_1_2D37
loc_1_2D31:
        mov    edx, dword ptr [esp + 0x24]
        mov    dword ptr [edx], ecx
loc_1_2D37:
        cmp    ecx, eax
        jne    loc_1_2D42
loc_1_2D3B:
        mov    al, 0x13
        pop    esi
        add    esp, 0x14
        ret    
loc_1_2D42:
        sub    al, al
        pop    esi
        add    esp, 0x14
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2D50:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        or     esi, esi
        je     loc_1_2DB5
        mov    eax, dword ptr [esp + 0x10]
        or     eax, eax
        je     loc_1_2DB5
        mov    bl, byte ptr [eax + 2]
        cmp    byte ptr [esi + 0x4f], bl
        jb     loc_1_2DB0
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0x48]
        mov    al, bl
        mov    ecx, dword ptr [ecx + eax*4]
        or     ecx, ecx
        je     loc_1_2DB0
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ecx + 0x68]
        cmp    eax, 1
        je     loc_1_2D8A
        or     eax, eax
        jne    loc_1_2DAA
loc_1_2D8A:
        cmp    dword ptr [ecx + 0x64], 1
        jne    loc_1_2DAA
        push   ecx
        call   sub_1_DEE4
        add    esp, 4
        push   1
        push   ebx
        push   esi
        call   sub_1_2F70
        add    esp, 0xc
        mov    al, 0xa
        pop    esi
        pop    ebx
        ret    
loc_1_2DAA:
        sub    al, al
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_2DB0:
        mov    al, 8
        pop    esi
        pop    ebx
        ret    
loc_1_2DB5:
        mov    al, 0x40
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2DC0:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [edi], 0
        mov    dl, byte ptr [eax + 0x4f]
        mov    ecx, dword ptr [eax + 0x48]
        mov    eax, dword ptr [esp + 0x14]
loc_1_2DD8:
        mov    bl, byte ptr [edi]
        mov    esi, ebx
        and    esi, 0xff
        cmp    dword ptr [ecx + esi*4], eax
        je     loc_1_2DF5
        inc    bl
        mov    byte ptr [edi], bl
        cmp    bl, dl
        jbe    loc_1_2DD8
        mov    al, 8
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_2DF5:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_1_2E00:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [edi], 0
        mov    cl, byte ptr [eax + 0x4e]
        mov    ebx, dword ptr [eax + 0x44]
        mov    esi, dword ptr [esp + 0x14]
loc_1_2E18:
        mov    dl, byte ptr [edi]
        xor    eax, eax
        mov    al, dl
        mov    eax, dword ptr [ebx + eax*4]
        or     eax, eax
        je     loc_1_2E2A
        cmp    dword ptr [eax + 0x10], esi
        je     loc_1_2E38
loc_1_2E2A:
        inc    dl
        mov    byte ptr [edi], dl
        cmp    dl, cl
        jbe    loc_1_2E18
        mov    al, 0x13
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_2E38:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_2E40:
        sub    esp, 8
        push   esi
        mov    ecx, dword ptr [esp + 0x14]
        cmp    byte ptr [ecx], 0x2a
        jne    loc_1_2E54
        mov    al, 0x15
        pop    esi
        add    esp, 8
        ret    
loc_1_2E54:
        lea    eax, [esp + 8]
        push   eax
        mov    eax, dword ptr [0x132d8]      ; -> loc_1_132D8
        push   eax
        push   ecx
        mov    eax, dword ptr [0x132b4]      ; -> loc_1_132B4
        push   eax
        call   sub_1_6954
        add    esp, 0x10
        or     eax, eax
        je     loc_1_2E80
        cmp    eax, 0xff
        je     loc_1_2E80
        mov    al, 0x15
        pop    esi
        add    esp, 8
        ret    
loc_1_2E80:
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x28]
        or     eax, eax
        je     loc_1_2EA0
        test   dword ptr [esi + 0x88], eax
        jne    loc_1_2EA0
        mov    al, 0x15
        pop    esi
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_2EA0:
        lea    eax, [esp + 7]
        push   eax
        mov    eax, dword ptr [esp + 0xc]
        mov    eax, dword ptr [eax + 8]
        push   eax
        push   esi
        call   sub_1_2E00
        add    esp, 0xc
        or     al, al
        je     loc_1_2EC1
        mov    al, 0x15
        pop    esi
        add    esp, 8
        ret    
loc_1_2EC1:
        mov    eax, dword ptr [esp + 0x1c]
        push   1
        push   eax
        mov    al, byte ptr [esp + 0xf]
        push   eax
        push   esi
        call   sub_1_2C70
        mov    eax, dword ptr [esp + 0x28]
        add    esp, 0x10
        or     eax, eax
        je     loc_1_2EE4
        mov    cl, byte ptr [esp + 7]
        mov    byte ptr [eax], cl
loc_1_2EE4:
        sub    al, al
        pop    esi
        add    esp, 8
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_1_2EF0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    eax, eax
        sub    esi, esi
        cmp    dword ptr [esp + 0x20], eax
        mov    edi, dword ptr [esp + 0x14]
        jne    loc_1_2F10
        mov    bl, byte ptr [edi + 0x4e]
        mov    ecx, dword ptr [edi + 0x44]
        add    edi, 0x4c
        jmp    loc_1_2F19
        db  08Dh, 049h, 000h
loc_1_2F10:
        mov    bl, byte ptr [edi + 0x4f]
        mov    ecx, dword ptr [edi + 0x48]
        add    edi, 0x4d
loc_1_2F19:
        mov    dl, byte ptr [edi]
loc_1_2F1B:
        cmp    bl, dl
        jae    loc_1_2F21
        mov    dl, 1
loc_1_2F21:
        mov    ebp, edx
        and    ebp, 0xff
        cmp    dword ptr [ecx + ebp*4], 0
        je     loc_1_2F40
        cmp    dl, byte ptr [edi]
        jne    loc_1_2F3C
        or     esi, esi
        jne    loc_1_2F45
        mov    esi, 1
loc_1_2F3C:
        inc    dl
        jmp    loc_1_2F1B
loc_1_2F40:
        mov    eax, 1
loc_1_2F45:
        or     eax, eax
        je     loc_1_2F6B
        mov    ebp, edx
        and    ebp, 0xff
        mov    esi, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx + ebp*4], esi
        mov    ecx, dword ptr [esp + 0x18]
        mov    byte ptr [edi], dl
        mov    byte ptr [ecx], dl
        inc    dl
        mov    byte ptr [edi], dl
        cmp    bl, dl
        jae    loc_1_2F6B
        mov    byte ptr [edi], 1
loc_1_2F6B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_2F70:
        cmp    dword ptr [esp + 0xc], 0
        mov    eax, dword ptr [esp + 4]
        jne    loc_1_2F83
        mov    dl, byte ptr [eax + 0x4e]
        mov    eax, dword ptr [eax + 0x44]
        jmp    loc_1_2F89
loc_1_2F83:
        mov    dl, byte ptr [eax + 0x4f]
        mov    eax, dword ptr [eax + 0x48]
loc_1_2F89:
        mov    cl, byte ptr [esp + 8]
        cmp    dl, cl
        jb     loc_1_2FAB
        and    ecx, 0xff
        lea    eax, [eax + ecx*4]
        cmp    dword ptr [eax], 0
        je     loc_1_2FAB
        mov    dword ptr [eax], 0
        mov    eax, 1
        ret    
loc_1_2FAB:
        sub    eax, eax
        ret    
        db  08Bh, 0FFh
sub_1_2FB0:
        sub    esp, 4
        push   esi
        push   edi
        lea    eax, [esp + 0xb]
        mov    esi, dword ptr [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   esi
        sub    edi, edi
        push   eax
        call   sub_1_2DC0
        add    esp, 0xc
        or     al, al
        je     loc_1_2FE0
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_2FE0:
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_1_301F
        mov    edx, dword ptr [eax + 0x3c]
        mov    ecx, edx
        sub    ecx, eax
        cmp    ecx, 0x3c
        je     loc_1_301F
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 0x1c]
        mov    cl, byte ptr [esp + 0xb]
        mov    byte ptr [eax + 2], cl
        mov    edi, 1
        push   0
        push   0xc000020c
        mov    eax, dword ptr [edx + 0x1c]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
loc_1_301F:
        mov    eax, dword ptr [esi + 0x1c]
        sub    eax, esi
        cmp    eax, 0x1c
        je     loc_1_305A
        mov    edi, 1
        mov    edi, edi
loc_1_3030:
        mov    edx, dword ptr [esi + 0x1c]
        push   0
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0xc000020c
        mov    eax, dword ptr [edx + 0x1c]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x1c]
        sub    eax, esi
        cmp    eax, 0x1c
        jne    loc_1_3030
loc_1_305A:
        or     edi, edi
        je     loc_1_3077
        mov    ax, word ptr [esi + 0x3a]
        test   al, 1
        jne    loc_1_3077
        or     ax, 1
        mov    word ptr [esi + 0x3a], ax
        push   esi
        call   sub_1_DEE4
        add    esp, 4
loc_1_3077:
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  090h
sub_1_3080:
        sub    esp, 0x10
        mov    eax, dword ptr [esp + 0x18]
        or     eax, eax
        je     loc_1_30BC
        mov    dword ptr [esp], eax
        push   0
        push   0
        push   0
        push   2
        lea    eax, [esp + 0x10]
        push   0
        push   eax
        call   sub_1_BDC0
        add    esp, 0x18
        mov    eax, dword ptr [esp + 0x14]
        push   0
        push   eax
        lea    eax, [esp + 8]
        push   0
        push   eax
        call   sub_1_AD68
        add    esp, 0x10
loc_1_30BC:
        mov    eax, dword ptr [esp + 0x1c]
        or     eax, eax
        je     loc_1_30CD
        push   eax
        call   sub_1_31C0
        add    esp, 4
loc_1_30CD:
        add    esp, 0x10
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_30E0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   0x18
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 4], 0
        mov    dword ptr [esi + 8], 0
        mov    byte ptr [esi + 0xc], 0
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esi], eax
        or     eax, eax
        je     loc_1_3195
        push   0x18
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esi + 4], eax
        mov    edi, eax
        or     edi, edi
        je     loc_1_3195
        mov    eax, dword ptr [esi]
        push   0x1a
        mov    dword ptr [eax + 0x14], 0
        mov    dword ptr [edi + 0x14], 0
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [edi + 0x14], eax
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [eax + 0x14], 0
        je     loc_1_3170
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_3169
        push   0x1a
        call   sub_1_001E
        add    esp, 4
        mov    ecx, dword ptr [esi]
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 0x14], 0
        je     loc_1_3170
loc_1_3169:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_3170:
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 0x14]
        or     eax, eax
        je     loc_1_3182
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_3182:
        mov    eax, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 0x14]
        or     eax, eax
        je     loc_1_3195
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_3195:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_1_31A4
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_31A4:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_1_31B4
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_31B4:
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
sub_1_31C0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 0x14]
        or     eax, eax
        je     loc_1_31D7
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_31D7:
        mov    eax, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0024
        add    esp, 4
        pop    esi
        ret    
        db  090h
loc_1_3200:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        mov    esi, dword ptr [eax + 8]
        mov    edi, dword ptr [eax + 0xc]
        mov    ebx, dword ptr [eax + 0x10]
        call   sub_1_0024
        add    esp, 4
        push   edi
        push   esi
        push   ebx
        call   sub_1_E64C
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3230:
        sub    esp, 0x4c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x68]
        mov    dword ptr [esp + 0x18], eax
        mov    ecx, dword ptr [0x1323c]      ; -> loc_1_1323C
        mov    ax, word ptr [esp + 0x60]
        mov    word ptr [esp + 0x12], ax
        and    eax, 0xffff
        sub    eax, 0x65
        cmp    eax, 0xf
        ja     loc_1_3263
        jmp    dword ptr [eax*4 + 0x3578]    ; -> obj1:0x3578
loc_1_3263:
        mov    ebp, 0xc00000bb
        mov    ebx, dword ptr [esp + 0x64]
        jmp    loc_1_34F5
loc_1_3271:
        call   sub_1_DB70
        mov    ebp, eax
        mov    ebx, dword ptr [esp + 0x64]
        jmp    loc_1_34F5
loc_1_3281:
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   ecx
        call   sub_1_DBD0
        add    esp, 0xc
        mov    ebp, eax
        mov    ebx, dword ptr [esp + 0x64]
        jmp    loc_1_34F5
        db  090h
loc_1_32A0:
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   0
        call   sub_1_D968
        add    esp, 0xc
        mov    ebp, eax
        mov    ebx, dword ptr [esp + 0x64]
        jmp    loc_1_34F5
        db  090h
loc_1_32C0:
        mov    ebx, dword ptr [esp + 0x64]
        or     ebx, ebx
        je     loc_1_32F3
        cmp    dword ptr [esp + 0x68], 0
        je     loc_1_32F3
        cmp    word ptr [esp + 0x12], 0x69
        jne    loc_1_32D9
        sub    ecx, ecx
loc_1_32D9:
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   ecx
        call   sub_1_D62C
        add    esp, 0xc
        mov    ebp, eax
        jmp    loc_1_34F5
loc_1_32F3:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x4c
        ret    
loc_1_3300:
        xor    eax, eax
        mov    al, byte ptr [0x1334f]        ; -> obj1:0x1334F
        lea    eax, [eax*4 + 4]
        cmp    eax, dword ptr [esp + 0x68]
        jbe    loc_1_3321
        mov    eax, 9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x4c
        ret    
loc_1_3321:
        mov    ebx, dword ptr [esp + 0x64]
        or     ebx, ebx
        jne    loc_1_3336
        mov    eax, 0xc000000d
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x4c
        ret    
loc_1_3336:
        mov    ecx, ebx
        mov    edx, dword ptr [0x1323c]      ; -> loc_1_1323C
        cmp    edx, 0x1323c                  ; -> loc_1_1323C
        je     loc_1_335C
loc_1_3346:
        mov    eax, dword ptr [edx + 0x58]
        or     eax, eax
        je     loc_1_3352
        mov    dword ptr [ecx], eax
        add    ecx, 4
loc_1_3352:
        mov    edx, dword ptr [edx]
        cmp    edx, 0x1323c                  ; -> loc_1_1323C
        jne    loc_1_3346
loc_1_335C:
        mov    dword ptr [ecx], 0
        sub    ebp, ebp
        jmp    loc_1_34F5
loc_1_3369:
        mov    ebx, dword ptr [esp + 0x64]
        or     ebx, ebx
        jne    loc_1_3380
        mov    eax, 0xc000000d
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x4c
        ret    
        db  08Bh, 0FFh
loc_1_3380:
        lea    edi, [esp + 0x1c]
        sub    eax, eax
        mov    ecx, 0x10
        rep stosd dword ptr es:[edi], eax
        mov    byte ptr [esp + 0x1c], 0x33
        cmp    dword ptr [esp + 0x6c], 0
        mov    ax, word ptr [esp + 0x68]
        mov    word ptr [esp + 0x24], ax
        mov    eax, dword ptr [0x1323c]      ; -> loc_1_1323C
        mov    dword ptr [esp + 0x20], ebx
        mov    al, byte ptr [eax + 0x51]
        mov    byte ptr [esp + 0x4c], al
        jne    loc_1_33C0
        mov    eax, 0xc000000d
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x4c
        ret    
loc_1_33C0:
        mov    eax, dword ptr [esp + 0x6c]
        cmp    dword ptr [eax], 0
        je     loc_1_33D3
        mov    byte ptr [esp + 0x26], 0x2a
        mov    eax, dword ptr [eax]
        push   eax
        jmp    loc_1_33F7
loc_1_33D3:
        mov    ecx, dword ptr [esp + 0x6c]
        lea    edx, [esp + 0x26]
        mov    eax, dword ptr [ecx + 0xe]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 0x12]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 0x16]
        add    ecx, 0xe
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [edx + 0xc], eax
        push   0
loc_1_33F7:
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_0770
        add    esp, 8
        cmp    byte ptr [esp + 0x1d], 0
        jne    loc_1_3412
        sub    ebp, ebp
        jmp    loc_1_34F5
loc_1_3412:
        cmp    byte ptr [esp + 0x1d], 6
        jne    loc_1_3420
        jmp    loc_1_34F0
        db  08Bh, 0FFh
loc_1_3420:
        mov    ebp, 0xc0000001
        jmp    loc_1_34F5
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3430:
        xor    eax, eax
        mov    ax, word ptr [0x132d4]        ; -> loc_1_132D4
        add    eax, 0xb7
        mov    ebx, dword ptr [esp + 0x64]
        or     ebx, ebx
        je     loc_1_34F0
        cmp    eax, dword ptr [esp + 0x68]
        ja     loc_1_34F0
        sub    si, si
        mov    edx, 0x133c4                  ; -> loc_1_133C4
loc_1_345B:
        mov    edi, dword ptr [edx]
        or     edi, edi
        je     loc_1_34A2
        mov    eax, esi
        and    eax, 0xffff
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        lea    ecx, [ebx + eax*2]
        mov    al, byte ptr [edi + 0x51]
        and    eax, 0xffff00ff
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [edi + 0x58]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edi + 0x74]
        mov    dword ptr [ecx + 8], eax
        inc    si
        mov    eax, dword ptr [edi + 0x78]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [edi + 0x7c]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [edi + 0x80]
        mov    dword ptr [ecx + 0x14], eax
loc_1_34A2:
        add    edx, 4
        cmp    edx, 0x133e4                  ; -> loc_1_133E4
        jb     loc_1_345B
        mov    word ptr [ebx], si
        mov    ax, word ptr [0x13364]        ; -> loc_1_13364
        mov    word ptr [ebx + 0xb2], ax
        mov    ax, word ptr [0x132d4]        ; -> loc_1_132D4
        lea    edi, [ebx + 0xb6]
        mov    word ptr [ebx + 0xb4], ax
        xor    eax, eax
        mov    ax, word ptr [0x132d4]        ; -> loc_1_132D4
        mov    esi, dword ptr [0x132d8]      ; -> loc_1_132D8
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        sub    ebp, ebp
        jmp    loc_1_34F5
loc_1_34F0:
        mov    ebp, 9
loc_1_34F5:
        cmp    word ptr [esp + 0x12], 0x68
        je     loc_1_3515
        cmp    word ptr [esp + 0x12], 0x69
        je     loc_1_3515
        cmp    word ptr [esp + 0x12], 0x67
        je     loc_1_3515
        cmp    word ptr [esp + 0x12], 0x66
        jne    loc_1_3558
loc_1_3515:
        or     ebp, ebp
        je     loc_1_3526
        cmp    ebp, 0xff
        je     loc_1_3526
        cmp    ebp, 9
        jne    loc_1_3558
loc_1_3526:
        cmp    ebp, 9
        jne    loc_1_3531
        mov    eax, dword ptr [esp + 0x68]
        jmp    loc_1_3537
loc_1_3531:
        mov    eax, dword ptr [esp + 0x18]
        sub    ebp, ebp
loc_1_3537:
        mov    edi, ebx
        mov    esi, dword ptr [esp + 0x14]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_3558:
        cmp    dword ptr [esp + 0x6c], 0
        je     loc_1_356C
        cmp    dword ptr [esp + 0x74], 0
        je     loc_1_356C
        mov    eax, dword ptr [esp + 0x6c]
        mov    dword ptr [eax], ebp
loc_1_356C:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x4c
        ret    
        db  08Bh, 0FFh
        ; ---- dispatch table (16 entries) ----
        dd  loc_1_3271
        dd  loc_1_3281
        dd  loc_1_32A0
        dd  loc_1_32C0
        dd  loc_1_32C0
        dd  loc_1_3263
        dd  loc_1_3263
        dd  loc_1_3263
        dd  loc_1_3263
        dd  loc_1_3263
        dd  loc_1_3263
        dd  loc_1_3263
        dd  loc_1_3300
        dd  loc_1_3263
        dd  loc_1_3369
        dd  loc_1_3430
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_35C0:
        mov    byte ptr [0x13388], 0         ; -> loc_1_13388
        push   0
        push   1
        push   0
        mov    eax, dword ptr [0x13327]      ; -> obj1:0x13327
        push   eax
        call   sub_1_12638
        add    esp, 0x10
        cmp    eax, -1
        je     loc_1_35E7
        mov    byte ptr [0x13388], 1         ; -> loc_1_13388
loc_1_35E7:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_35F0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        and    esi, 0xffff
        push   esi
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    loc_1_361F
        cmp    dword ptr [0x12ecc], 0        ; -> loc_1_12ECC
        je     loc_1_361F
        call   sub_1_002A
        push   esi
        call   sub_1_001E
        add    esp, 4
loc_1_361F:
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3630:
        sub    esp, 0x10
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 0x12
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        mov    dword ptr [esi + 0x38], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0x18], eax
        lea    eax, [esi + 0x28]
        mov    dword ptr [esi + 0x3c], 1
        mov    byte ptr [esi + 0x44], 0x20
        mov    dword ptr [esi + 8], 0x11223344
        mov    byte ptr [esi + 0x48], 0
        mov    dword ptr [esi + 0x2c], eax
        mov    dword ptr [eax], eax
        lea    eax, [esp + 8]
        push   esi
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        call   dword ptr [eax + 8]
        mov    ecx, dword ptr [esp + 0x10]
        add    esp, 8
        mov    dword ptr [esi + 0x10], ecx
        pop    edi
        pop    esi
        add    esp, 0x10
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_1_3690:
        sub    esp, 0x10
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esp], eax
        mov    eax, dword ptr [esp + 0x18]
        push   eax
        lea    eax, [esp + 4]
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        call   dword ptr [eax + 0x10]
        add    esp, 0x18
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_36C0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        push   0x49
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esi + 0xc], eax
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_36EB
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_36EB:
        mov    edi, dword ptr [esp + 0x1c]
        push   edi
        push   ebp
        call   sub_1_3630
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_3720
        cmp    ebx, 0xff
        je     loc_1_3720
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        call   sub_1_0024
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_3720:
        mov    dword ptr [ebp + 0xc], esi
        mov    dword ptr [ebp + 0x38], 3
        or     ebx, ebx
        je     loc_1_3736
        cmp    ebx, 0xff
        jne    loc_1_37AD
loc_1_3736:
        push   3
        mov    eax, dword ptr [edi + 0x58]
        push   eax
        mov    eax, dword ptr [esp + 0x28]
        push   eax
        lea    eax, [ebp + 0x14]
        push   edi
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        push   0
        call   sub_1_3D60
        add    esp, 0x1c
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_3763
        cmp    ebx, 0xff
        jne    loc_1_3797
loc_1_3763:
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_3690
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_37C7
        cmp    ebx, 0xff
        je     loc_1_37C7
        push   ebp
        call   sub_1_3820
        add    esp, 4
        or     eax, eax
        je     loc_1_3797
        push   ebp
        call   sub_1_3820
        add    esp, 4
loc_1_3797:
        push   ebp
        call   sub_1_37F0
        add    esp, 4
        or     eax, eax
        je     loc_1_37AD
        push   ebp
        call   sub_1_37F0
        add    esp, 4
loc_1_37AD:
        mov    dword ptr [esi + 0xc], 0
        push   ebp
        call   sub_1_0024
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_37C7:
        mov    eax, dword ptr [edi + 0x18]
        add    edi, 0x14
        mov    dword ptr [ebp], edi
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        mov    dword ptr [edi + 4], ebp
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_37F0:
        sub    esp, 0x10
        mov    eax, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esp], eax
        mov    dword ptr [esp + 4], 0x3870   ; -> loc_1_3870
        lea    eax, [esp]
        mov    dword ptr [esp + 8], 0
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        call   dword ptr [eax + 0xc]
        add    esp, 0x14
        ret    
        db  090h
sub_1_3820:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        call   sub_1_3840
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3840:
        sub    esp, 0x10
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esp], eax
        mov    dword ptr [esp + 4], 0x3870   ; -> loc_1_3870
        mov    dword ptr [esp + 8], 0
        lea    eax, [esp]
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        call   dword ptr [eax + 4]
        add    esp, 0x14
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_3870:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3880:
        sub    esp, 4
        push   esi
        mov    dword ptr [esp + 4], 0
        lea    eax, [esp + 4]
        push   3
        push   eax
        push   0x13294                       ; -> loc_1_13294
        call   sub_1_FEF4
        add    esp, 0xc
        or     eax, eax
        je     loc_1_38B2
        mov    eax, 1
        mov    esi, dword ptr [esp + 0xc]
        jmp    loc_1_3980
loc_1_38B2:
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 4], 0x39b0   ; -> loc_1_39B0
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x1c], 0x4655424e
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 4]
        add    eax, 0x20
        mov    dword ptr [ecx + 0xc], eax
        mov    ecx, dword ptr [esp + 0x1c]
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx + 0x18], eax
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 0x10], eax
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x30], 0x4655424e
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x20], 0
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 0x2c], eax
        mov    eax, dword ptr [ecx + 0xc]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 0x24], eax
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x28], 0
        mov    eax, dword ptr [esp + 4]
        add    eax, 0xc
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        push   esi
        call   dword ptr [eax + 0x30]
        add    esp, 0x14
        or     eax, eax
        je     loc_1_39A0
        cmp    eax, 0xff
        je     loc_1_39A0
loc_1_3980:
        cmp    dword ptr [esp + 4], 0
        je     loc_1_39A0
        mov    ecx, dword ptr [esi + 4]
        or     ecx, ecx
        je     loc_1_39A0
        push   0
        push   eax
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        call   ecx
        add    esp, 0xc
        mov    eax, 0xff
loc_1_39A0:
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_39B0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        or     esi, esi
        je     loc_1_39EE
        mov    ecx, dword ptr [esi + 4]
        or     ecx, ecx
        je     loc_1_39D3
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   ecx
        add    esp, 0xc
loc_1_39D3:
        mov    dword ptr [esi], 0x13294      ; -> loc_1_13294
        mov    eax, dword ptr [0x13298]      ; -> loc_1_13298
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x13298]      ; -> loc_1_13298
        mov    dword ptr [eax], esi
        mov    dword ptr [0x13298], esi      ; -> loc_1_13298
loc_1_39EE:
        pop    esi
        ret    
sub_1_39F0:
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        mov    esi, dword ptr [esp + 0x10]
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        push   esi
        call   dword ptr [eax + 0x18]
        add    esp, 0x10
        or     eax, eax
        je     loc_1_3A2A
        cmp    eax, 0xff
        je     loc_1_3A2A
        push   0
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   dword ptr [esi + 4]
        add    esp, 0xc
        mov    eax, 0xff
loc_1_3A2A:
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
sub_1_3A30:
        mov    eax, dword ptr [esp + 0x10]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        call   dword ptr [eax + 0x1c]
        add    esp, 0x14
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3A60:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], 0
        sub    ebx, ebx
        sub    ebp, ebp
        lea    eax, [esp + 0x10]
        push   3
        push   eax
        push   0x13294                       ; -> loc_1_13294
        call   sub_1_FEF4
        add    esp, 0xc
        cmp    eax, ebx
        mov    esi, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esi + 4]
        je     loc_1_3AB0
        cmp    eax, ebx
        je     loc_1_3AA2
        push   ebx
        push   1
        mov    ecx, dword ptr [esi + 8]
        push   ecx
        call   eax
        add    esp, 0xc
loc_1_3AA2:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_3AB0:
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 0x10]
        test   byte ptr [esp + 0x2c], 0x80
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 4], 0x39b0   ; -> loc_1_39B0
        mov    ecx, dword ptr [esp + 0x28]
        je     loc_1_3B24
        mov    edx, dword ptr [ecx + 0x14]
        or     edx, edx
        je     loc_1_3B24
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x44], 0x4655424e
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x34], 0
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x40], edx
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [ecx + 0x10]
        mov    dword ptr [edx + 0x38], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x3c], 0
        mov    ebx, dword ptr [esp + 0x10]
        add    ebx, 0x34
loc_1_3B24:
        mov    edx, dword ptr [ecx + 8]
        or     edx, edx
        mov    edi, dword ptr [esp + 0x20]
        je     loc_1_3B68
        cmp    dword ptr [ecx], edi
        jae    loc_1_3B68
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x30], 0x4655424e
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x20], ebx
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x2c], edx
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [edx + 0x24], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x28], 0
        mov    ebp, dword ptr [esp + 0x10]
        add    ebp, 0x20
loc_1_3B68:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x1c], 0x4655424e
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0xc], ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx + 0x18], eax
        mov    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [esp + 0x10]
        add    eax, 0xc
        push   eax
        mov    ax, word ptr [esp + 0x20]
        push   edi
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        push   esi
        call   dword ptr [eax + 0x2c]
        add    esp, 0x10
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_3BE6
        cmp    ebx, 0xff
        je     loc_1_3BE6
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_3BDC
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_1_3BDC
        push   0
        mov    ecx, dword ptr [esp + 0x14]
        push   ebx
        push   ecx
        call   eax
        add    esp, 0xc
loc_1_3BDC:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_3BE6:
        mov    eax, dword ptr [esp + 0x24]
        pop    ebp
        mov    dword ptr [eax], edi
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3C00:
        push   esi
        push   0x39
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    loc_1_3C16
        mov    eax, 1
        pop    esi
        ret    
loc_1_3C16:
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0xc], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x29], ecx
        mov    esi, dword ptr [esp + 0x18]
        or     esi, esi
        mov    dword ptr [eax + 0x2d], esi
        je     loc_1_3C66
        cmp    byte ptr [esi + 0x52], 0
        jne    loc_1_3C50
        push   eax
        call   ecx
        add    esp, 4
        sub    eax, eax
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_3C50:
        mov    edx, dword ptr [esi + 0x40]
        add    esi, 0x3c
        mov    dword ptr [eax + 0x31], esi
        lea    ecx, [eax + 0x31]
        mov    dword ptr [eax + 0x35], edx
        mov    dword ptr [edx], ecx
        mov    dword ptr [esi + 4], ecx
        jmp    loc_1_3C87
loc_1_3C66:
        mov    dword ptr [eax + 0x31], 0x132a4 ; -> loc_1_132A4
        lea    edx, [eax + 0x31]
        mov    ecx, dword ptr [0x132a8]      ; -> loc_1_132A8
        mov    dword ptr [eax + 0x35], ecx
        mov    ecx, dword ptr [0x132a8]      ; -> loc_1_132A8
        mov    dword ptr [ecx], edx
        mov    dword ptr [0x132a8], edx      ; -> loc_1_132A8
loc_1_3C87:
        mov    dword ptr [eax + 0x21], 0
        mov    dword ptr [eax + 0x19], 0x3cc0 ; -> loc_1_3CC0
        mov    ecx, dword ptr [0x12e68]      ; -> loc_1_12E68
        mov    dword ptr [eax + 0x25], ecx
        push   eax
        add    eax, 0x15
        push   eax
        call   sub_1_0012
        add    esp, 8
        mov    eax, 0xff
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3CC0:
        mov    edx, dword ptr [esp + 8]
        cmp    dword ptr [edx + 0x2d], 0
        mov    eax, dword ptr [edx + 0x35]
        mov    ecx, dword ptr [edx + 0x31]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        je     loc_1_3CE0
        push   edx
        call   dword ptr [edx + 0x29]
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_3CE0:
        push   edx
        call   dword ptr [edx + 0x29]
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3CF0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], 0
        mov    esi, dword ptr [esp + 0x18]
        or     esi, esi
        lea    edi, [esi + 0x3c]
        jne    loc_1_3D0F
        mov    edi, 0x132a4                  ; -> loc_1_132A4
loc_1_3D0F:
        mov    ebx, dword ptr [edi]
        cmp    ebx, edi
        je     loc_1_3D4B
        mov    dword ptr [esp + 0x10], 1
loc_1_3D20:
        lea    ebp, [ebx - 0x31]
        mov    ebx, dword ptr [ebx]
        mov    eax, dword ptr [ebp + 0x35]
        mov    ecx, dword ptr [ebp + 0x31]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        lea    eax, [ebp + 0x15]
        push   eax
        call   sub_1_0018
        add    esp, 4
        or     esi, esi
        je     loc_1_3D47
        push   ebp
        call   dword ptr [ebp + 0x29]
        add    esp, 4
loc_1_3D47:
        cmp    ebx, edi
        jne    loc_1_3D20
loc_1_3D4B:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3D60:
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x20], 1
        mov    word ptr [esp + 0x24], 0xe
        mov    word ptr [esp + 0x26], 2
        mov    bx, word ptr [esp + 0x4c]
        mov    ah, bl
        mov    al, bh
        mov    word ptr [esp + 0x28], ax
        mov    eax, dword ptr [esp + 0x50]
        mov    edx, eax
        and    edx, 0xff00
        mov    ecx, eax
        shl    ecx, 0x10
        or     edx, ecx
        shl    edx, 8
        mov    ecx, eax
        sar    ecx, 0x18
        mov    dl, cl
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        lea    eax, [esp + 0x2e]
        mov    dword ptr [esp + 0x2a], edx
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    esi, dword ptr [esp + 0x54]
        mov    ebp, esi
        and    ebp, 2
        push   0
        cmp    ebp, 1
        sbb    eax, eax
        and    eax, 0xb
        add    eax, 6
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        call   dword ptr [eax]
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_3E87
        mov    edi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x44]
        mov    ecx, edi
        mov    dword ptr [eax], ecx
        test   esi, 1
        je     loc_1_3E48
        mov    esi, dword ptr [esp + 0x48]
        push   esi
        push   0x3e90                        ; -> sub_1_3E90
        push   3
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        push   edi
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
        push   esi
        push   0x4730                        ; -> loc_1_4730
        push   1
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        push   edi
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
        or     ebp, ebp
        je     loc_1_3E73
        push   esi
        push   0x4660                        ; -> loc_1_4660
        push   0
        jmp    loc_1_3E67
loc_1_3E48:
        cmp    bx, 0x8a
        mov    esi, dword ptr [esp + 0x48]
        push   esi
        jne    loc_1_3E60
        push   0x4870                        ; -> loc_1_4870
        jmp    loc_1_3E65
        db  005h, 000h, 000h, 000h, 000h
loc_1_3E60:
        push   0x4960                        ; -> loc_1_4960
loc_1_3E65:
        push   4
loc_1_3E67:
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        push   edi
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
loc_1_3E73:
        push   esi
        push   0x4b60                        ; -> loc_1_4B60
        push   2
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        push   edi
        call   dword ptr [eax + 0x38]
        add    esp, 0x10
loc_1_3E87:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  090h
sub_1_3E90:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], 0
        mov    edi, dword ptr [esp + 0x28]
        mov    esi, dword ptr [esp + 0x34]
        add    dword ptr [edi + 0x20], esi
        mov    eax, dword ptr [edi + 0x38]
        cmp    eax, 7
        jne    loc_1_42B0
        mov    ebp, dword ptr [edi + 0xc]
        mov    ebx, dword ptr [esp + 0x38]
        mov    dword ptr [ebp + 0x40], esi
        mov    dword ptr [ebx], 0
        xor    eax, eax
        mov    ax, word ptr [edi + 0x32]
        or     eax, eax
        je     loc_1_3EF7
        cmp    eax, 1
        je     loc_1_4190
        cmp    eax, 2
        je     loc_1_41F0
        cmp    eax, 3
        je     loc_1_42A2
        mov    eax, dword ptr [esp + 0x1c]
loc_1_3EEF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_3EF7:
        cmp    dword ptr [esp + 0x30], 4
        jae    loc_1_3FB2
        lea    eax, [esp + 0x10]
        push   4
        push   eax
        push   0x1329c                       ; -> loc_1_1329C
        call   sub_1_FEF4
        add    esp, 0xc
        or     eax, eax
        je     loc_1_3F27
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_3F27:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x14], 0x1900beef
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x18], edi
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x40]
        mov    dword ptr [eax + 0x1c], 0
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax + 0x35], 0x20
        mov    edx, dword ptr [esp + 0x10]
        add    edx, 0x20
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 8], 0x4500   ; -> loc_1_4500
        mov    dword ptr [ecx + 4], 4
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0xc], eax
        lea    eax, [edi + 0x1c]
        mov    dword ptr [ecx + 0x10], 0x12f50 ; -> loc_1_12F50
        mov    dword ptr [edx + 0x10], 0x4655424e
        mov    dword ptr [edx], 0
        mov    dword ptr [edx + 0xc], 4
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edx + 8], 0
        mov    word ptr [edi + 0x32], 1
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebp + 0x60], eax
        mov    eax, 0x18
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_3FB2:
        mov    eax, dword ptr [esp + 0x3c]
        mov    eax, dword ptr [eax]
        mov    ecx, eax
        and    ecx, 0xff00
        shl    ecx, 8
        mov    edx, eax
        and    edx, 0xff0000
        shr    edx, 8
        or     ecx, edx
        shr    eax, 0x18
        and    eax, 0xff
        or     ecx, eax
        lea    eax, [ecx + 4]
        mov    dword ptr [esp + 0x18], eax
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x40]
        push   eax
        push   ebx
        mov    eax, dword ptr [esp + 0x3c]
        push   esi
        push   eax
        mov    ax, word ptr [esp + 0x40]
        push   eax
        mov    eax, dword ptr [esp + 0x3c]
        push   edi
        push   eax
        call   sub_1_6A34
        add    esp, 0x20
        mov    ecx, dword ptr [ebx]
        sub    dword ptr [ebp + 0x40], ecx
        sub    dword ptr [esp + 0x30], ecx
        sub    esi, ecx
        add    dword ptr [esp + 0x3c], ecx
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_4104
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x18], edi
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x1c]
        add    eax, 2
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   ebp
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        call   sub_1_2DC0
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0x3c]
        mov    ecx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [ecx + 0x2c]
        cmp    eax, ecx
        jb     loc_1_4055
        mov    eax, ecx
loc_1_4055:
        mov    word ptr [edi + 0x32], 2
        mov    dword ptr [ebp + 0x60], 0
        mov    dword ptr [ebp + 0x50], 0
        cmp    dword ptr [esp + 0x30], eax
        jb     loc_1_40B0
        mov    ecx, dword ptr [esp + 0x10]
        cmp    word ptr [ecx + 0x35], 0x20
        jne    loc_1_40B0
        mov    edi, dword ptr [ecx + 0x24]
        mov    esi, dword ptr [esp + 0x3c]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    dword ptr [ebx], eax
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   0
        push   eax
        call   sub_1_4380
        add    esp, 0xc
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_40B0:
        mov    edx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x40]
        add    edx, 0x20
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 8], 0x4380   ; -> sub_1_4380
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [esp + 0x10]
        add    eax, 0x35
        cmp    word ptr [eax], 0x100
        jne    loc_1_40F0
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, 0x18
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_40F0:
        mov    dword ptr [ecx + 0x10], 0x12f50 ; -> loc_1_12F50
        mov    eax, 0x18
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_4104:
        mov    ecx, dword ptr [ebx]
        cmp    dword ptr [esp + 0x18], ecx
        ja     loc_1_4165
        cmp    eax, 0x17
        jne    loc_1_411D
        cmp    dword ptr [ebp + 0x3c], 0
        jne    loc_1_411D
        cmp    dword ptr [ebp + 0x68], 7
        je     loc_1_4165
loc_1_411D:
        sub    eax, eax
        cmp    ecx, esi
        jae    loc_1_3EEF
        mov    dword ptr [esp + 0x14], eax
        sub    dword ptr [edi + 0x20], esi
        mov    ecx, dword ptr [esp + 0x40]
        mov    eax, dword ptr [esp + 0x3c]
        push   ecx
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x3c]
        push   esi
        push   eax
        mov    ax, word ptr [esp + 0x40]
        push   eax
        mov    eax, dword ptr [esp + 0x3c]
        push   edi
        push   eax
        call   sub_1_3E90
        mov    ecx, dword ptr [esp + 0x34]
        add    esp, 0x20
        add    dword ptr [ebx], ecx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_4165:
        mov    word ptr [edi + 0x32], 3
        mov    ecx, dword ptr [edi + 0x18]
        mov    eax, dword ptr [ecx + 0x30]
        add    ecx, 0x2c
        mov    dword ptr [edi + 0x28], ecx
        lea    edx, [edi + 0x28]
        mov    dword ptr [edi + 0x2c], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 4], edx
        mov    byte ptr [edi + 0x48], 1
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_4190:
        mov    eax, dword ptr [ebp + 0x60]
        mov    dword ptr [esp + 0x10], eax
        xor    ebx, ebx
        mov    bx, word ptr [edi + 0x30]
        lea    eax, [edi + ebx]
        mov    ecx, dword ptr [esp + 0x10]
        add    eax, 0x1c
        mov    dword ptr [ecx + 0x24], eax
        mov    edx, 4
        mov    eax, dword ptr [esp + 0x10]
        sub    edx, ebx
        mov    dword ptr [eax + 0x2c], edx
        cmp    edx, esi
        mov    ecx, dword ptr [esp + 0x40]
        jb     loc_1_41C2
        mov    edx, esi
loc_1_41C2:
        mov    dword ptr [ecx + 4], edx
        mov    eax, dword ptr [esp + 0x10]
        add    eax, 0x20
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 8], 0x4500   ; -> loc_1_4500
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, 0x18
        mov    dword ptr [ecx + 0x10], 0x12f50 ; -> loc_1_12F50
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_41F0:
        mov    edx, dword ptr [ebp + 0x60]
        xor    eax, eax
        mov    ax, word ptr [edx + 8]
        sub    eax, dword ptr [ebp + 0x50]
        mov    ecx, dword ptr [edx + 0x32]
        mov    dword ptr [esp + 0x10], ecx
        mov    ecx, dword ptr [ebp + 0x3c]
        sub    ecx, dword ptr [ebp + 0x44]
        cmp    eax, ecx
        jb     loc_1_420F
        mov    eax, ecx
loc_1_420F:
        mov    dword ptr [ebp + 0x60], 0
        mov    edi, dword ptr [ebp + 0x50]
        mov    ecx, dword ptr [edx + 4]
        mov    esi, dword ptr [esp + 0x10]
        add    ecx, edi
        mov    dword ptr [esi + 0x24], ecx
        xor    ecx, ecx
        mov    cx, word ptr [edx + 8]
        mov    edx, dword ptr [esp + 0x10]
        sub    ecx, edi
        mov    dword ptr [edx + 0x2c], ecx
        cmp    dword ptr [esp + 0x30], eax
        jb     loc_1_4270
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0x3c]
        mov    edi, dword ptr [ecx + 0x24]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    dword ptr [ebx], eax
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   0
        push   eax
        call   sub_1_4380
        add    esp, 0xc
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_4270:
        mov    edx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x40]
        add    edx, 0x20
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 8], 0x4380   ; -> sub_1_4380
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, 0x18
        mov    dword ptr [ecx + 0x10], 0x12f50 ; -> loc_1_12F50
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_42A2:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_42B0:
        cmp    eax, 4
        jne    loc_1_42E6
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x40]
        mov    ebx, dword ptr [esp + 0x3c]
        push   eax
        push   ebx
        mov    eax, dword ptr [esp + 0x3c]
        push   esi
        push   eax
        mov    ax, word ptr [esp + 0x40]
        push   eax
        mov    eax, dword ptr [esp + 0x3c]
        push   edi
        push   eax
        call   sub_1_6C30
        add    esp, 0x20
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_42E6:
        cmp    eax, 6
        jne    loc_1_4320
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x40]
        mov    ebx, dword ptr [esp + 0x3c]
        push   eax
        push   ebx
        mov    eax, dword ptr [esp + 0x3c]
        push   esi
        push   eax
        mov    ax, word ptr [esp + 0x40]
        push   eax
        mov    eax, dword ptr [esp + 0x3c]
        push   edi
        push   eax
        call   sub_1_6FC8
        add    esp, 0x20
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_4320:
        mov    ebx, dword ptr [esp + 0x38]
        sub    eax, eax
        mov    dword ptr [ebx], esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_4330:
        push   esi
        mov    edx, dword ptr [esp + 0x24]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx], 0
        mov    esi, dword ptr [eax + 0x3c]
        mov    ecx, esi
        sub    ecx, eax
        cmp    ecx, 0x3c
        je     loc_1_4350
        jmp    loc_1_4362
        db  08Dh, 049h, 000h
loc_1_4350:
        mov    eax, dword ptr [eax + 0x80]
        mov    esi, dword ptr [eax + 0x24]
        mov    ecx, esi
        sub    ecx, eax
        cmp    ecx, 0x24
        je     loc_1_4375
loc_1_4362:
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, 0x18
        mov    dword ptr [edx], esi
        pop    esi
        ret    
loc_1_4375:
        sub    eax, eax
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4380:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 0x1c]
        or     edx, edx
        je     loc_1_43C4
        cmp    edx, 9
        je     loc_1_43C4
        mov    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [ecx + 0x18]
        or     eax, eax
        je     loc_1_43AD
        mov    eax, dword ptr [eax + 0xc]
        or     eax, eax
        je     loc_1_43AD
        mov    dword ptr [eax + 0x60], 0
loc_1_43AD:
        push   0
        push   edx
        mov    eax, dword ptr [ecx + 0x1c]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_43C4:
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [ecx + 0x18]
        mov    edi, dword ptr [esi + 0xc]
        mov    edx, dword ptr [esp + 0x20]
        add    dword ptr [edi + 0x44], edx
        add    dword ptr [edi + 0x50], edx
        mov    eax, dword ptr [edi + 0x40]
        cmp    edx, eax
        jb     loc_1_43E8
        mov    dword ptr [edi + 0x40], 0
        jmp    loc_1_43ED
loc_1_43E8:
        sub    eax, edx
        mov    dword ptr [edi + 0x40], eax
loc_1_43ED:
        mov    edx, dword ptr [edi + 0x3c]
        mov    eax, dword ptr [edi + 0x44]
        cmp    edx, eax
        jne    loc_1_4460
        mov    dword ptr [edi + 0x60], 0
        mov    word ptr [esi + 0x32], 0
        mov    eax, dword ptr [edi + 0x50]
        push   eax
        push   0
        mov    eax, dword ptr [ecx + 0x1c]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        mov    dword ptr [edi + 0x50], 0
        mov    dword ptr [edi + 0x44], 0
        mov    word ptr [esi + 0x30], 0
        cmp    dword ptr [edi + 0x40], 0
        je     loc_1_44EE
        mov    dword ptr [esp + 0x10], 0
        lea    eax, [esp + 0x10]
        push   0
        push   eax
        mov    eax, dword ptr [edi + 0x40]
        push   eax
        push   0
        push   0
        push   esi
        push   0
        call   sub_1_45B0
        add    esp, 0x1c
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_4460:
        jae    loc_1_4494
        mov    dword ptr [edi + 0x60], 0
        mov    word ptr [esi + 0x32], 0
        mov    dword ptr [edi + 0x44], 0
        mov    word ptr [esi + 0x30], 0
        push   0
        push   0x19
        mov    eax, dword ptr [ecx + 0x1c]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_4494:
        mov    ebp, dword ptr [ecx + 0x1c]
        xor    ebx, ebx
        mov    bx, word ptr [ebp + 8]
        cmp    dword ptr [edi + 0x50], ebx
        jb     loc_1_44E5
        mov    dword ptr [edi + 0x60], 0
        mov    dword ptr [edi + 0x50], 0
        mov    word ptr [esi + 0x32], 3
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [ecx + 0x30]
        add    ecx, 0x2c
        mov    dword ptr [esi + 0x28], ecx
        lea    edx, [esi + 0x28]
        mov    dword ptr [esi + 0x2c], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 4], edx
        mov    byte ptr [esi + 0x48], 1
        push   ebx
        push   9
        push   ebp
        call   sub_1_25A0
        add    esp, 0xc
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_44E5:
        mov    dword ptr [edi + 0x60], ebp
        mov    word ptr [esi + 0x32], 2
loc_1_44EE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4500:
        sub    esp, 4
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x10]
        cmp    dword ptr [esp + 0x14], 0
        mov    ecx, dword ptr [edx + 0x18]
        mov    esi, dword ptr [ecx + 0xc]
        je     loc_1_4540
        mov    dword ptr [edx], 0x1329c      ; -> loc_1_1329C
        mov    eax, dword ptr [0x132a0]      ; -> loc_1_132A0
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [0x132a0]      ; -> loc_1_132A0
        pop    edi
        mov    dword ptr [eax], edx
        mov    dword ptr [0x132a0], edx      ; -> loc_1_132A0
        mov    word ptr [ecx + 0x32], 0
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_4540:
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [esi + 0x40], eax
        jae    loc_1_454C
        mov    dword ptr [esi + 0x40], eax
loc_1_454C:
        add    ax, word ptr [ecx + 0x30]
        mov    word ptr [ecx + 0x30], ax
        cmp    ax, 4
        jne    loc_1_45A5
        mov    dword ptr [esp + 8], 0
        mov    word ptr [ecx + 0x32], 0
        mov    word ptr [ecx + 0x30], 0
        mov    dword ptr [edx], 0x1329c      ; -> loc_1_1329C
        mov    eax, dword ptr [0x132a0]      ; -> loc_1_132A0
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [0x132a0]      ; -> loc_1_132A0
        mov    dword ptr [eax], edx
        lea    eax, [ecx + 0x1c]
        push   eax
        mov    dword ptr [0x132a0], edx      ; -> loc_1_132A0
        lea    eax, [esp + 0xc]
        push   eax
        mov    eax, dword ptr [esi + 0x40]
        push   eax
        push   4
        push   0
        push   ecx
        push   0
        call   sub_1_45B0
        add    esp, 0x1c
loc_1_45A5:
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_1_45B0:
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x3c]
        lea    eax, [esp + 0x20]
        mov    ecx, dword ptr [esp + 0x48]
        push   eax
        sub    dword ptr [esi + 0x20], ecx
        mov    eax, dword ptr [esp + 0x54]
        push   eax
        mov    eax, dword ptr [esp + 0x54]
        push   eax
        mov    eax, dword ptr [esp + 0x50]
        push   ecx
        push   eax
        push   0
        push   esi
        push   0
        call   sub_1_3E90
        add    esp, 0x20
        mov    ebx, eax
        cmp    ebx, 0x18
        jne    loc_1_4652
        mov    edi, dword ptr [esp + 0x2c]
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [esp + 0xc], eax
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, edi
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, edi
        add    eax, 0x20
        push   eax
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x18]
        push   0x12f50                       ; -> loc_1_12F50
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        call   dword ptr [eax + 0x28]
        add    esp, 0x10
        mov    ebx, eax
        cmp    ebx, 0xff
        je     loc_1_4650
        push   0
        push   ebx
        mov    eax, dword ptr [edi + 0x1c]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  08Dh, 049h, 000h
loc_1_4650:
        sub    ebx, ebx
loc_1_4652:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_4660:
        sub    esp, 4
        lea    eax, [esp]
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        call   sub_1_7528
        add    esp, 0x18
        or     eax, eax
        je     loc_1_46A0
        cmp    eax, 0xff
        je     loc_1_46A0
        mov    eax, 0x17
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_46A0:
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [eax], 0x46e0       ; -> loc_1_46E0
        mov    ecx, dword ptr [esp]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [esp + 0x24]
        mov    ecx, dword ptr [esp]
        mov    dword ptr [eax], ecx
        mov    eax, 0x18
        add    esp, 4
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_46E0:
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        or     eax, eax
        je     loc_1_4720
        cmp    eax, 0xff
        je     loc_1_4720
        mov    edx, dword ptr [esp + 8]
        cmp    dword ptr [edx + 0x38], 4
        jne    loc_1_4720
        mov    dword ptr [edx + 0x38], 0
        mov    esi, dword ptr [edx + 0x18]
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        add    esi, 0x1c
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        dec    dword ptr [edx + 0x3c]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], esi
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [esi], edx
loc_1_4720:
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4730:
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        call   sub_1_7598
        add    esp, 0x1c
        or     eax, eax
        je     loc_1_476B
        cmp    eax, 0xff
        je     loc_1_476B
        mov    eax, 0xe
loc_1_476B:
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_4770:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x28]
        lea    eax, [esp + 0x12]
        mov    esi, dword ptr [edi + 0x80]
        mov    ebx, dword ptr [esp + 0x2c]
        push   eax
        push   ebx
        push   esi
        call   sub_1_2DC0
        mov    byte ptr [esp + 0x1f], al
        add    esp, 0xc
        push   ebx
        push   esi
        call   sub_1_2FB0
        add    esp, 8
        mov    eax, dword ptr [ebx + 0xc]
        or     eax, eax
        je     loc_1_47CE
        cmp    byte ptr [eax + 0x48], 1
        jne    loc_1_47CE
        cmp    word ptr [eax + 0x32], 3
        jne    loc_1_47CE
        mov    edx, dword ptr [eax + 0x28]
        lea    ebp, [eax + 0x28]
        mov    ecx, dword ptr [eax + 0x2c]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        mov    byte ptr [eax + 0x48], 0
        mov    dword ptr [eax + 0x2c], ebp
        mov    dword ptr [ebp], ebp
loc_1_47CE:
        mov    ax, word ptr [ebx + 0x3a]
        and    ax, 1
        cmp    ax, 1
        sbb    ebp, ebp
        inc    ebp
        mov    dword ptr [esp + 0x14], ebx
        push   0
        push   esi
        lea    eax, [esp + 0x1c]
        push   0
        push   eax
        call   sub_1_AD68
        lea    eax, [esp + 0x24]
        add    esp, 0x10
        push   eax
        call   sub_1_A9FC
        add    esp, 4
        cmp    byte ptr [esp + 0x13], 0
        jne    loc_1_481B
        or     ebp, ebp
        je     loc_1_481B
        mov    al, byte ptr [esp + 0x12]
        push   1
        push   eax
        push   esi
        call   sub_1_2F70
        add    esp, 0xc
loc_1_481B:
        cmp    dword ptr [edi + 0x44], 0
        je     loc_1_4864
        push   edi
        call   sub_1_1770
        add    esp, 4
        or     eax, eax
        jne    loc_1_4864
        lea    eax, [esp + 0x11]
        push   eax
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        push   esi
        call   sub_1_2E00
        add    esp, 0xc
        or     al, al
        je     loc_1_4851
        mov    eax, 0xc0000001
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_4851:
        mov    al, byte ptr [esp + 0x11]
        push   1
        push   eax
        push   edi
        push   0
        push   esi
        call   sub_1_17A0
        add    esp, 0x14
loc_1_4864:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Bh, 0FFh
loc_1_4870:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 0x10], 0
        lea    eax, [esp + 0x10]
        mov    esi, dword ptr [esp + 0x40]
        mov    dword ptr [esp + 0xc], 0
        push   eax
        mov    dword ptr [esi], 0
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        mov    ebx, dword ptr [esp + 0x40]
        push   eax
        mov    edi, dword ptr [esp + 0x40]
        push   ebx
        mov    eax, dword ptr [esp + 0x40]
        push   edi
        push   eax
        mov    eax, dword ptr [esp + 0x40]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        push   eax
        call   sub_1_7AFC
        add    esp, 0x30
        or     eax, eax
        je     loc_1_48DC
        cmp    eax, 0xff
        jne    loc_1_4955
loc_1_48DC:
        push   0x30
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    loc_1_48F6
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_48F6:
        mov    dword ptr [eax + 8], 0x4990   ; -> loc_1_4990
        sub    edi, dword ptr [ebx]
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax + 0xc], eax
        lea    edx, [eax + 0x1c]
        mov    dword ptr [eax], edx
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x14], ecx
        mov    dword ptr [eax + 0x10], 0
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x18], ecx
        mov    dword ptr [edx + 0x10], 0x4655424e
        mov    dword ptr [edx], 0
        mov    ecx, dword ptr [esp + 0xc]
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0xffff
        mov    dword ptr [edx + 0xc], ecx
        mov    ecx, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [edx + 8], 0
        mov    dword ptr [esi], eax
        mov    eax, 0x18
loc_1_4955:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_4960:
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x2c]
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        call   sub_1_80D8
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4990:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x24]
        cmp    dword ptr [esp + 0x28], 0
        mov    ecx, dword ptr [eax + 0x14]
        mov    dword ptr [esp + 0x14], ecx
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [esp + 0x18], eax
        jne    loc_1_49CB
        mov    cx, word ptr [ecx + 8]
        mov    eax, dword ptr [esp + 0x24]
        and    ecx, 0xffff
        cmp    ecx, dword ptr [eax + 4]
        jae    loc_1_49CB
        mov    dword ptr [esp + 0x28], 9
loc_1_49CB:
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_4B2F
        mov    eax, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [eax]
        lea    eax, [ecx + 0xc]
        mov    dword ptr [esp + 0x1c], eax
        mov    ebx, dword ptr [eax]
        mov    eax, dword ptr [esp + 0x18]
        cmp    byte ptr [eax + 0x15], 0
        je     loc_1_4A10
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [esp + 0x30]
        push   ecx
        mov    ecx, dword ptr [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [ecx + 4]
        push   eax
        call   sub_1_8018
        add    esp, 0x10
        jmp    loc_1_4B0C
        db  08Bh, 0FFh
loc_1_4A10:
        cmp    ebx, dword ptr [esp + 0x1c]
        je     loc_1_4AFD
        lea    ebx, [ebx]
loc_1_4A20:
        lea    eax, [ebx + 0x84]
        mov    dword ptr [esp + 0x10], eax
        inc    dword ptr [eax]
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [eax + 0x10], ebx
        je     loc_1_4AEB
        mov    ebp, dword ptr [ebx + 0x24]
        mov    eax, ebp
        sub    eax, ebx
        cmp    eax, 0x24
        je     loc_1_4AEB
        mov    eax, dword ptr [ebp + 4]
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [eax], ecx
        sub    edx, edx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [esp + 0x28], edx
        je     loc_1_4A70
        cmp    dword ptr [esp + 0x28], 9
        je     loc_1_4A70
        mov    ecx, dword ptr [esp + 0x28]
        jmp    loc_1_4AD7
        db  08Dh, 064h, 024h, 000h
loc_1_4A70:
        xor    edx, edx
        mov    dx, word ptr [eax + 8]
        mov    ecx, dword ptr [esp + 0x2c]
        cmp    edx, ecx
        jb     loc_1_4A80
        mov    edx, ecx
loc_1_4A80:
        mov    ecx, dword ptr [esp + 0x14]
        mov    edi, dword ptr [eax + 4]
        mov    esi, dword ptr [ecx + 4]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [esp + 0x14]
        lea    esi, [eax + 0xa]
        mov    edi, dword ptr [ecx + 0xa]
        mov    dword ptr [esi], edi
        mov    edi, dword ptr [ecx + 0xe]
        mov    dword ptr [esi + 4], edi
        mov    edi, dword ptr [ecx + 0x12]
        add    ecx, 0xa
        mov    dword ptr [esi + 8], edi
        mov    ecx, dword ptr [ecx + 0xc]
        mov    dword ptr [esi + 0xc], ecx
        xor    ecx, ecx
        mov    cx, word ptr [eax + 8]
        cmp    dword ptr [esp + 0x2c], ecx
        ja     loc_1_4AD2
        jne    loc_1_4ACE
        cmp    dword ptr [esp + 0x28], 9
        je     loc_1_4AD2
loc_1_4ACE:
        sub    ecx, ecx
        jmp    loc_1_4AD7
loc_1_4AD2:
        mov    ecx, 9
loc_1_4AD7:
        push   edx
        push   ecx
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        push   ebp
        call   sub_1_0024
        add    esp, 4
loc_1_4AEB:
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [ebx]
        dec    dword ptr [eax]
        cmp    ebx, dword ptr [esp + 0x1c]
        jne    loc_1_4A20
loc_1_4AFD:
        mov    eax, dword ptr [esp + 0x18]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_DDE4
        add    esp, 4
loc_1_4B0C:
        mov    eax, dword ptr [esp + 0x18]
        cmp    byte ptr [eax + 0x15], 0
        jne    loc_1_4B22
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_4B22:
        mov    eax, dword ptr [esp + 0x24]
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_4B2F:
        mov    eax, dword ptr [esp + 0x2c]
        push   eax
        mov    eax, dword ptr [esp + 0x2c]
        mov    ecx, dword ptr [esp + 0x18]
        push   eax
        push   ecx
        call   sub_1_25A0
        mov    eax, dword ptr [esp + 0x30]
        add    esp, 0xc
        push   eax
        call   sub_1_0024
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_4B60:
        mov    eax, 0xfe
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4B70:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [0x13274]      ; -> loc_1_13274
        cmp    eax, 0x13274                  ; -> loc_1_13274
        mov    dword ptr [esp + 0x10], eax
        je     loc_1_4C1A
loc_1_4B8B:
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, dword ptr [eax + 0xc]
        sub    eax, esi
        cmp    eax, -0xc
        je     loc_1_4C05
        sub    ebx, ebx
        mov    ebp, 7
loc_1_4BA0:
        mov    edi, dword ptr [esi + 0x1c]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x1c
        je     loc_1_4BF8
        lea    esp, [esp]
loc_1_4BB0:
        cmp    byte ptr [edi + 0x38], bl
        je     loc_1_4BED
        cmp    dword ptr [edi + 0x68], ebp
        jne    loc_1_4BED
        mov    ecx, dword ptr [edi + 0x1c]
        mov    eax, ecx
        sub    eax, edi
        cmp    eax, 0x1c
        je     loc_1_4BED
        mov    al, byte ptr [ecx + 0x34]
        cmp    al, 1
        jne    loc_1_4BE8
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        push   ebx
        push   0x15
        mov    eax, dword ptr [ecx + 0x1c]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        jmp    loc_1_4BED
loc_1_4BE8:
        dec    al
        mov    byte ptr [ecx + 0x34], al
loc_1_4BED:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x1c
        jne    loc_1_4BB0
loc_1_4BF8:
        mov    esi, dword ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, esi
        cmp    eax, -0xc
        jne    loc_1_4BA0
loc_1_4C05:
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 0x10], eax
        cmp    eax, 0x13274                  ; -> loc_1_13274
        jne    loc_1_4B8B
loc_1_4C1A:
        mov    eax, dword ptr [0x13284]      ; -> loc_1_13284
        nop    
loc_1_4C20:
        cmp    eax, 0x13284                  ; -> loc_1_13284
        je     loc_1_4C48
        mov    ecx, eax
        mov    eax, dword ptr [eax]
        mov    dl, byte ptr [ecx + 0xc]
        dec    dl
        mov    byte ptr [ecx + 0xc], dl
        cmp    dl, 1
        jne    loc_1_4C20
        push   0
        push   0x15
        sub    ecx, 0x32
        push   ecx
        call   sub_1_25A0
        add    esp, 0xc
loc_1_4C48:
        mov    dword ptr [0x13398], 0        ; -> loc_1_13398
        mov    dword ptr [0x133a0], 0        ; -> loc_1_133A0
        mov    eax, dword ptr [0x12e68]      ; -> loc_1_12E68
        mov    dword ptr [0x1339c], eax      ; -> loc_1_1339C
        push   0
        push   0x4b70                        ; -> sub_1_4B70
        push   0x1f4
        push   0x1338c                       ; -> loc_1_1338C
        call   0
        add    esp, 0x10
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_4C90:
        sub    esp, 4
        test   byte ptr [0x13364], 1         ; -> loc_1_13364
        jne    loc_1_4CEE
        cmp    dword ptr [0x132f0], 0        ; -> loc_1_132F0
        jne    loc_1_4CEE
        mov    dword ptr [0x132e0], 0xea600  ; -> loc_1_132E0
        mov    word ptr [0x13314], 0         ; -> loc_1_13314
        lea    eax, [esp]
        push   eax
        push   0
        push   0
        push   0
        push   0xf5c0                        ; -> loc_1_F5C0
        push   0
        push   0
        mov    eax, dword ptr [0x132fc]      ; -> loc_1_132FC
        push   eax
        call   sub_1_109B8
        add    esp, 0x20
        or     eax, eax
        je     loc_1_4CE5
        cmp    eax, 0xff
        jne    loc_1_4CF0
loc_1_4CE5:
        mov    eax, dword ptr [esp]
        mov    dword ptr [0x132f0], eax      ; -> loc_1_132F0
loc_1_4CEE:
        sub    eax, eax
loc_1_4CF0:
        add    esp, 4
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4D00:
        push   0x1338c                       ; -> loc_1_1338C
        call   6
        add    esp, 4
        ret    
        db  08Bh, 0FFh, 083h, 0ECh, 014h, 056h, 057h, 083h, 07Ch, 024h, 024h, 000h, 074h, 024h, 08Bh, 07Ch
        db  024h, 028h, 00Bh, 0FFh, 08Bh, 074h, 024h, 020h, 074h, 020h, 083h, 07Eh, 058h, 000h, 074h, 01Ah
        db  06Ah, 000h, 06Ah, 000h, 056h, 0E8h, 020h, 08Fh, 000h, 000h, 083h, 0C4h, 00Ch, 0EBh, 00Bh, 08Dh
        db  049h, 000h, 08Bh, 074h, 024h, 020h, 08Bh, 07Ch, 024h, 028h, 00Bh, 0FFh, 075h, 014h, 0C7h, 046h
        db  058h, 000h, 000h, 000h, 000h, 05Fh, 05Eh, 083h, 0C4h, 014h, 0C3h, 08Dh, 0A4h, 024h, 000h, 000h
        db  000h, 000h, 0C7h, 044h, 024h, 008h, 010h, 000h, 000h, 000h, 08Dh, 044h, 024h, 008h, 050h, 08Dh
        db  044h, 024h, 010h, 050h, 06Ah, 02Ch, 057h, 0E8h, 0F7h, 0B2h, 0FFh, 0FFh, 083h, 0C4h, 010h, 00Bh
        db  0C0h, 074h, 005h, 083h, 0F8h, 009h, 075h, 078h, 083h, 07Ch, 024h, 008h, 004h, 072h, 035h, 08Bh
        db  04Ch, 024h, 00Ch, 081h, 0E1h, 000h, 0FFh, 000h, 000h, 08Bh, 044h, 024h, 00Ch, 0C1h, 0E0h, 010h
        db  00Bh, 0C8h, 0C1h, 0E1h, 008h, 08Bh, 044h, 024h, 00Ch, 025h, 000h, 000h, 0FFh, 000h, 0C1h, 0E8h
        db  008h, 00Bh, 0C8h, 08Bh, 044h, 024h, 00Ch, 0C1h, 0E8h, 018h, 025h, 0FFh, 000h, 000h, 000h, 00Bh
        db  0C8h, 089h, 04Eh, 074h, 083h, 07Ch, 024h, 008h, 008h, 072h, 035h, 08Bh, 04Ch, 024h, 010h, 081h
        db  0E1h, 000h, 0FFh, 000h, 000h, 08Bh, 044h, 024h, 010h, 0C1h, 0E0h, 010h, 00Bh, 0C8h, 0C1h, 0E1h
        db  008h, 08Bh, 044h, 024h, 010h, 025h, 000h, 000h, 0FFh, 000h, 0C1h, 0E8h, 008h, 00Bh, 0C8h, 08Bh
        db  044h, 024h, 010h, 0C1h, 0E8h, 018h, 025h, 0FFh, 000h, 000h, 000h, 00Bh, 0C8h, 089h, 04Eh, 078h
        db  066h, 0A1h
        dd  loc_1_13364              ; fixup
        db  08Bh, 0C8h, 081h, 0E1h, 0FFh, 0FFh, 000h, 000h, 0F6h, 0C4h, 010h, 074h, 023h, 083h, 07Eh, 074h
        db  000h, 075h, 006h, 083h, 07Eh, 078h, 000h, 074h, 017h, 066h, 0C7h, 005h
        dd  loc_1_13364              ; fixup
        db  008h, 000h, 0F6h, 0C1h, 010h, 074h, 009h, 066h, 0C7h, 005h
        dd  loc_1_13364              ; fixup
        db  018h, 000h, 08Bh, 044h, 024h, 02Ch, 050h, 057h, 056h, 0E8h, 018h, 08Eh, 000h, 000h, 083h, 0C4h
        db  00Ch, 00Bh, 0C0h, 074h, 007h, 03Dh, 0FFh, 000h, 000h, 000h, 075h, 02Ch, 083h, 07Eh, 058h, 000h
        db  074h, 026h, 0F6h, 005h
        dd  loc_1_13364              ; fixup
        db  001h, 075h, 007h, 0E8h, 02Eh, 0B7h, 000h, 000h, 0EBh, 00Dh, 068h
        dd  loc_1_132F0              ; fixup
        db  0E8h, 012h, 0B8h, 000h, 000h, 083h, 0C4h, 004h, 056h, 0E8h, 041h, 0ADh, 000h, 000h, 083h, 0C4h
        db  004h, 05Fh, 05Eh, 083h, 0C4h, 014h, 0C3h
sub_1_4E80:
        sub    esp, 0x10
        push   esi
        mov    esi, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esi + 0x6c]
        mov    dword ptr [esp + 4], eax
        lea    eax, [esp + 4]
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        call   dword ptr [eax + 4]
        add    esp, 4
        mov    eax, dword ptr [esi + 0x68]
        mov    dword ptr [esp + 4], eax
        lea    eax, [esp + 4]
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        call   dword ptr [eax + 4]
        add    esp, 4
        mov    eax, dword ptr [esi + 0x70]
        mov    dword ptr [esp + 4], eax
        lea    eax, [esp + 4]
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        call   dword ptr [eax + 4]
        add    esp, 4
        mov    eax, dword ptr [esi + 0x54]
        mov    dword ptr [esp + 4], eax
        push   0
        push   esi
        lea    eax, [esp + 0xc]
        push   0
        push   eax
        call   sub_1_AB28
        add    esp, 0x10
        sub    eax, eax
        pop    esi
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_4EF0:
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x40]
        or     esi, esi
        jne    loc_1_4F10
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x2c
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4F10:
        cmp    dword ptr [esp + 0x50], 0
        je     loc_1_50C0
        cmp    dword ptr [0x12f60], 0        ; -> obj1:0x12F60
        jne    loc_1_4F63
        mov    bl, byte ptr [0x13364]        ; -> loc_1_13364
        call   sub_1_5CD0
        push   0
        mov    eax, dword ptr [0x12f80]      ; -> obj1:0x12F80
        push   eax
        call   sub_3_04D0
        add    esp, 8
        call   sub_1_5DF0
        test   bl, 0x10
        je     loc_1_4F59
        mov    ax, word ptr [0x13364]        ; -> loc_1_13364
        or     ax, 0x10
        mov    word ptr [0x13364], ax        ; -> loc_1_13364
loc_1_4F59:
        mov    dword ptr [0x12f60], 1        ; -> obj1:0x12F60
loc_1_4F63:
        lea    eax, [esp + 0x1c]
        push   eax
        push   esi
        call   sub_1_55E0
        lea    eax, [esp + 0x34]
        add    esp, 8
        push   eax
        push   esi
        call   sub_1_5510
        add    esp, 8
        sub    edi, edi
        mov    eax, 0x133c4                  ; -> loc_1_133C4
loc_1_4F86:
        cmp    dword ptr [eax], 0
        je     loc_1_4FA3
        add    eax, 4
        inc    edi
        cmp    eax, 0x133e4                  ; -> loc_1_133E4
        jb     loc_1_4F86
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x2c
        ret    
loc_1_4FA3:
        lea    eax, [esp + 0x14]
        push   eax
        push   esi
        call   sub_1_56B0
        mov    edx, dword ptr [esp + 0x4c]
        add    esp, 8
        mov    ecx, edx
        and    ecx, 0xff00
        mov    eax, edx
        shl    eax, 0x10
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edx
        and    eax, 0xff0000
        shr    eax, 8
        or     ecx, eax
        shr    edx, 0x18
        and    edx, 0xff
        or     ecx, edx
        mov    ebp, esi
        and    ebp, 0xff00
        mov    eax, esi
        shl    eax, 0x10
        or     ebp, eax
        shl    ebp, 8
        mov    eax, esi
        and    eax, 0xff0000
        shr    eax, 8
        or     ebp, eax
        shr    esi, 0x18
        and    esi, 0xff
        lea    eax, [esp + 0x14]
        push   0
        push   eax
        or     ebp, esi
        mov    eax, dword ptr [esp + 0x38]
        push   eax
        mov    eax, dword ptr [esp + 0x38]
        push   eax
        mov    eax, dword ptr [esp + 0x30]
        push   eax
        mov    eax, dword ptr [esp + 0x30]
        push   eax
        push   ecx
        push   ebp
        mov    eax, dword ptr [0x12f80]      ; -> obj1:0x12F80
        push   eax
        call   sub_3_0000
        mov    dword ptr [esp + 0x34], eax
        add    esp, 0x24
        or     eax, eax
        je     loc_1_5042
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x2c
        ret    
loc_1_5042:
        mov    cl, byte ptr [0x133f4]        ; -> loc_1_133F4
        cmp    cl, 0xff
        je     loc_1_5060
        mov    al, byte ptr [0x12f64]        ; -> obj1:0x12F64
        lea    ebx, [ecx + eax]
        inc    al
        mov    byte ptr [0x12f64], al        ; -> obj1:0x12F64
        jmp    loc_1_5062
        db  08Bh, 0FFh
loc_1_5060:
        mov    bl, 0xff
loc_1_5062:
        mov    esi, dword ptr [esp + 0x48]
loc_1_5066:
        xor    eax, eax
        mov    al, bl
        push   eax
        push   esi
        call   sub_1_6420
        add    esp, 8
        cmp    eax, 0xff
        jne    loc_1_50A1
        cmp    bl, 0xff
        je     loc_1_5084
        mov    bl, 0xff
        jmp    loc_1_5066
loc_1_5084:
        push   ebp
        mov    ecx, dword ptr [0x12f80]      ; -> obj1:0x12F80
        push   ecx
        call   sub_1_5110
        add    esp, 8
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x2c
        ret    
loc_1_50A1:
        mov    ecx, dword ptr [0x12f80]      ; -> obj1:0x12F80
        push   esi
        mov    ecx, dword ptr [ecx + 0xc]
        mov    dword ptr [edi*4 + 0x133c4], ecx ; -> loc_1_133C4
        mov    byte ptr [ecx + 0x51], al
        call   sub_1_015D
        add    esp, 4
        jmp    loc_1_50FC
        db  090h
loc_1_50C0:
        mov    eax, esi
        mov    ecx, esi
        and    ecx, 0xff00
        mov    edx, eax
        shl    edx, 0x10
        or     ecx, edx
        shl    ecx, 8
        and    esi, 0xff0000
        shr    esi, 8
        or     ecx, esi
        shr    eax, 0x18
        and    eax, 0xff
        or     ecx, eax
        push   ecx
        mov    eax, dword ptr [0x12f80]      ; -> obj1:0x12F80
        push   eax
        call   sub_1_5110
        mov    dword ptr [esp + 0x18], eax
        add    esp, 8
loc_1_50FC:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x2c
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_5110:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        mov    edx, dword ptr [esp + 0x24]
        mov    dword ptr [esp + 0x18], esi
        mov    edi, dword ptr [edx + 8]
        mov    eax, edx
        sub    eax, edi
        cmp    eax, -8
        je     loc_1_5149
        mov    eax, dword ptr [esp + 0x28]
loc_1_5131:
        cmp    dword ptr [edi + 0x58], eax
        jne    loc_1_513A
        mov    esi, edi
        jmp    loc_1_513E
loc_1_513A:
        mov    dword ptr [esp + 0x18], edi
loc_1_513E:
        mov    edi, dword ptr [edi]
        mov    ecx, edx
        sub    ecx, edi
        cmp    ecx, -8
        jne    loc_1_5131
loc_1_5149:
        or     esi, esi
        jne    loc_1_515A
        mov    eax, 0xc000000d
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_515A:
        mov    byte ptr [esi + 0x52], 0
        push   0
        push   0
        push   esi
        call   sub_1_DC58
        add    esp, 0xc
        dec    byte ptr [0x1334f]            ; -> obj1:0x1334F
        cmp    byte ptr [0x1334f], 1         ; -> obj1:0x1334F
        jne    loc_1_5181
        mov    byte ptr [0x13350], 0         ; -> loc_1_13350
loc_1_5181:
        cmp    byte ptr [0x1334f], 0         ; -> obj1:0x1334F
        jne    loc_1_51FB
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, esi
        cmp    eax, 0x24
        je     loc_1_51BE
loc_1_5194:
        mov    edx, dword ptr [esi + 0x24]
        push   0
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0xc00000ca
        mov    eax, dword ptr [edx + 0x1c]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, esi
        cmp    eax, 0x24
        jne    loc_1_5194
loc_1_51BE:
        mov    eax, dword ptr [esi + 0x34]
        sub    eax, esi
        cmp    eax, 0x34
        je     loc_1_51FB
loc_1_51C8:
        mov    edi, dword ptr [esi + 0x34]
        push   0
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0xc00000ca
        mov    eax, dword ptr [edi + 8]
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        push   edi
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [esi + 0x34]
        sub    eax, esi
        cmp    eax, 0x34
        jne    loc_1_51C8
loc_1_51FB:
        push   esi
        call   sub_1_5E10
        add    esp, 4
        push   esi
        call   sub_1_3CF0
        mov    dword ptr [esp + 0x20], 0
        add    esp, 4
        mov    eax, dword ptr [0x132b4]      ; -> loc_1_132B4
        cmp    dword ptr [eax], 0
        jle    loc_1_5295
        sub    edi, edi
loc_1_5221:
        mov    eax, dword ptr [0x132b4]      ; -> loc_1_132B4
        lea    ebp, [edi + eax + 8]
        mov    ebx, dword ptr [ebp]
        cmp    ebp, ebx
        je     loc_1_5280
loc_1_5231:
        mov    eax, ebx
        mov    ebx, dword ptr [ebx]
        test   byte ptr [eax + 0x20], 0x80
        je     loc_1_527C
        mov    ecx, dword ptr [eax + 0x14]
        or     ecx, ecx
        je     loc_1_527C
        mov    edx, dword ptr [ecx + 0x20]
        cmp    dword ptr [edx + 0x2c], esi
        jne    loc_1_527C
        mov    dword ptr [eax + 0x14], 0
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0xc
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_527C
        mov    eax, dword ptr [esp + 0x14]
        push   0xc00000ca
        push   eax
        call   dword ptr [esp + 0x18]
        add    esp, 8
loc_1_527C:
        cmp    ebp, ebx
        jne    loc_1_5231
loc_1_5280:
        add    edi, 8
        inc    dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [0x132b4]      ; -> loc_1_132B4
        mov    eax, dword ptr [esp + 0x1c]
        cmp    dword ptr [ecx], eax
        jg     loc_1_5221
loc_1_5295:
        sub    edi, edi
        cmp    byte ptr [esi + 0x4e], 0
        jb     loc_1_52C3
loc_1_52A0:
        mov    eax, dword ptr [esi + 0x44]
        mov    eax, dword ptr [eax + edi*4]
        or     eax, eax
        je     loc_1_52B9
        push   1
        push   edi
        push   eax
        push   0
        push   esi
        call   sub_1_17A0
        add    esp, 0x14
loc_1_52B9:
        inc    edi
        xor    eax, eax
        mov    al, byte ptr [esi + 0x4e]
        cmp    eax, edi
        jge    loc_1_52A0
loc_1_52C3:
        push   esi
        call   sub_1_4E80
        add    esp, 4
        mov    eax, dword ptr [esi + 0xc]
        sub    eax, esi
        cmp    eax, 0xc
        je     loc_1_52F6
loc_1_52D6:
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        mov    ecx, dword ptr [eax + 4]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [esi + 0xc]
        sub    eax, esi
        cmp    eax, 0xc
        jne    loc_1_52D6
loc_1_52F6:
        mov    eax, dword ptr [esi + 0x14]
        sub    eax, esi
        cmp    eax, 0x14
        je     loc_1_5320
loc_1_5300:
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        mov    ecx, dword ptr [eax + 4]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [esi + 0x14]
        sub    eax, esi
        cmp    eax, 0x14
        jne    loc_1_5300
loc_1_5320:
        sub    eax, eax
        mov    ecx, 0x133c4                  ; -> loc_1_133C4
loc_1_5327:
        cmp    dword ptr [ecx], esi
        je     loc_1_5340
        add    ecx, 4
        inc    eax
        cmp    ecx, 0x133e4                  ; -> loc_1_133E4
        jb     loc_1_5327
        jmp    loc_1_5360
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5340:
        lea    edi, [eax*4 + 0x133c4]        ; -> loc_1_133C4
        mov    eax, dword ptr [edi]
        mov    al, byte ptr [eax + 0x51]
        and    eax, 0xff
        push   eax
        call   sub_1_6453
        add    esp, 4
        mov    dword ptr [edi], 0
loc_1_5360:
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [0x13274]      ; -> loc_1_13274
        cmp    ecx, 0x13274                  ; -> loc_1_13274
        je     loc_1_539C
        mov    edx, dword ptr [esi + 0x88]
        not    edx
loc_1_5380:
        cmp    dword ptr [ecx + 0x1c], esi
        jne    loc_1_538C
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ecx + 0x1c], eax
loc_1_538C:
        mov    eax, dword ptr [ecx + 0x14]
        and    dword ptr [eax + 0x28], edx
        mov    ecx, dword ptr [ecx]
        cmp    ecx, 0x13274                  ; -> loc_1_13274
        jne    loc_1_5380
loc_1_539C:
        mov    eax, dword ptr [esi + 0x44]
        or     eax, eax
        je     loc_1_53AC
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_53AC:
        mov    eax, dword ptr [esi + 0x48]
        or     eax, eax
        je     loc_1_53BC
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_53BC:
        push   esi
        call   sub_1_0024
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
sub_1_53D0:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, 1
        sub    bl, bl
        mov    edi, edi
loc_1_53E0:
        xor    eax, eax
        mov    al, bl
        lea    edi, [esp + eax*4 + 0x14]
        lea    eax, [ebx + 0x31]
        mov    dword ptr [edi], 0x7f000000
        mov    byte ptr [0x12f72], al        ; -> obj1:0x12F72
        lea    eax, [esp + 0x10]
        push   2
        push   eax
        push   0x12f68                       ; -> obj1:0x12F68
        call   sub_1_5D30
        add    esp, 0xc
        or     eax, eax
        je     loc_1_5433
        mov    eax, dword ptr [esp + 0x10]
        push   edi
        push   eax
        call   sub_1_FD20
        add    esp, 8
        or     eax, eax
        je     loc_1_5426
        mov    dword ptr [edi], 0x7f000000
loc_1_5426:
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_5433:
        inc    bl
        cmp    bl, 4
        jb     loc_1_53E0
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [0x1333b], eax      ; -> obj1:0x1333B
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [0x1333f], eax      ; -> obj1:0x1333F
        sub    dl, dl
        mov    eax, 0x7f000000
loc_1_5453:
        xor    ecx, ecx
        mov    cl, dl
        mov    dword ptr [esp + ecx*4 + 0x14], eax
        inc    dl
        cmp    dl, 4
        jb     loc_1_5453
        lea    eax, [esp + 0x10]
        push   2
        push   eax
        push   0x12f74                       ; -> obj1:0x12F74
        call   sub_1_5D30
        add    esp, 0xc
        or     eax, eax
        je     loc_1_54EE
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, ecx
        mov    edi, ecx
        mov    al, byte ptr [eax]
        cmp    al, 0x30
        jb     loc_1_548C
        cmp    al, 0x39
        jbe    loc_1_5490
loc_1_548C:
        cmp    al, 0x2e
        jne    loc_1_54DA
loc_1_5490:
        sub    bl, bl
loc_1_5492:
        or     esi, esi
        je     loc_1_54DA
loc_1_5496:
        mov    al, byte ptr [edi]
        cmp    al, 0x30
        jb     loc_1_54A0
        cmp    al, 0x39
        jbe    loc_1_54A4
loc_1_54A0:
        cmp    al, 0x2e
        jne    loc_1_54A7
loc_1_54A4:
        inc    edi
        jmp    loc_1_5496
loc_1_54A7:
        cmp    byte ptr [edi], 0x2c
        jne    loc_1_54B2
        mov    byte ptr [edi], 0
        inc    edi
        jmp    loc_1_54B4
loc_1_54B2:
        sub    esi, esi
loc_1_54B4:
        xor    eax, eax
        mov    al, bl
        lea    ebp, [esp + eax*4 + 0x14]
        push   ebp
        push   ecx
        call   sub_1_FD20
        add    esp, 8
        or     eax, eax
        je     loc_1_54D1
        mov    dword ptr [ebp], 0x7f000000
loc_1_54D1:
        inc    bl
        mov    ecx, edi
        cmp    bl, 4
        jb     loc_1_5492
loc_1_54DA:
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_54EE
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_54EE:
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [0x13343], eax      ; -> obj1:0x13343
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [0x13347], eax      ; -> obj1:0x13347
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_5510:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        sub    cl, cl
        mov    esi, dword ptr [esp + 0x28]
loc_1_551C:
        xor    eax, eax
        mov    al, cl
        mov    dword ptr [esi + eax*4], 0x7f000000
        inc    cl
        cmp    cl, 4
        jb     loc_1_551C
        mov    ecx, dword ptr [0x13343]      ; -> obj1:0x13343
        mov    dword ptr [esi], ecx
        mov    eax, dword ptr [0x13347]      ; -> obj1:0x13347
        mov    dword ptr [esi + 4], eax
        cmp    ecx, 0x7f000000
        jne    loc_1_55CC
        cmp    eax, 0x7f000000
        jne    loc_1_55CC
        mov    dword ptr [esp + 0xc], 0x10
        lea    eax, [esp + 0xc]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x2c]
        push   6
        push   eax
        call   sub_1_0071
        add    esp, 0x10
        or     eax, eax
        je     loc_1_557B
        cmp    eax, 9
        jne    loc_1_55CC
loc_1_557B:
        sub    cl, cl
        lea    ecx, [ecx]
loc_1_5580:
        mov    edi, ecx
        and    edi, 0xff
        lea    eax, [edi*4 + 4]
        cmp    eax, dword ptr [esp + 0xc]
        ja     loc_1_55C5
        mov    edx, dword ptr [esp + edi*4 + 0x10]
        mov    ebx, edx
        and    ebx, 0xff00
        mov    eax, edx
        shl    eax, 0x10
        or     ebx, eax
        shl    ebx, 8
        mov    eax, edx
        and    eax, 0xff0000
        shr    eax, 8
        or     ebx, eax
        shr    edx, 0x18
        and    edx, 0xff
        or     ebx, edx
        mov    dword ptr [esi + edi*4], ebx
loc_1_55C5:
        inc    cl
        cmp    cl, 4
        jb     loc_1_5580
loc_1_55CC:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_55E0:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        sub    cl, cl
        mov    esi, dword ptr [esp + 0x28]
loc_1_55EC:
        xor    eax, eax
        mov    al, cl
        mov    dword ptr [esi + eax*4], 0x7f000000
        inc    cl
        cmp    cl, 4
        jb     loc_1_55EC
        mov    ecx, dword ptr [0x1333b]      ; -> obj1:0x1333B
        mov    dword ptr [esi], ecx
        mov    eax, dword ptr [0x1333f]      ; -> obj1:0x1333F
        mov    dword ptr [esi + 4], eax
        cmp    ecx, 0x7f000000
        jne    loc_1_569C
        cmp    eax, 0x7f000000
        jne    loc_1_569C
        mov    dword ptr [esp + 0xc], 0x10
        lea    eax, [esp + 0xc]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x2c]
        push   0x2c
        push   eax
        call   sub_1_0071
        add    esp, 0x10
        or     eax, eax
        je     loc_1_564B
        cmp    eax, 9
        jne    loc_1_569C
loc_1_564B:
        sub    cl, cl
        lea    ecx, [ecx]
loc_1_5650:
        mov    edi, ecx
        and    edi, 0xff
        lea    eax, [edi*4 + 4]
        cmp    eax, dword ptr [esp + 0xc]
        ja     loc_1_5695
        mov    edx, dword ptr [esp + edi*4 + 0x10]
        mov    ebx, edx
        and    ebx, 0xff00
        mov    eax, edx
        shl    eax, 0x10
        or     ebx, eax
        shl    ebx, 8
        mov    eax, edx
        and    eax, 0xff0000
        shr    eax, 8
        or     ebx, eax
        shr    edx, 0x18
        and    edx, 0xff
        or     ebx, edx
        mov    dword ptr [esi + edi*4], ebx
loc_1_5695:
        inc    cl
        cmp    cl, 4
        jb     loc_1_5650
loc_1_569C:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_56B0:
        sub    esp, 0x230
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x248]
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 4], 0
        mov    dword ptr [esp + 0x50], 0
        mov    dword ptr [esp + 0x54], 0
        mov    dword ptr [esp + 0x58], 0x100
        lea    eax, [esp + 0x140]
        mov    dword ptr [esp + 0x5c], 0x100
        mov    dword ptr [esp + 0x60], 0
        mov    dword ptr [esp + 0x10], 0x100
        mov    dword ptr [esp + 0x4c], 0x4655424e
        mov    dword ptr [esp + 0x3c], 0
        mov    dword ptr [esp + 0x48], 0x100
        mov    dword ptr [esp + 0x40], eax
        lea    eax, [esp + 0x2c]
        mov    dword ptr [esp + 0x44], 0
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        push   eax
        mov    dword ptr [eax + 0xc], 0
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x44]
        push   eax
        lea    eax, [esp + 0x5c]
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        push   0
        call   dword ptr [eax + 0x48]
        add    esp, 0x14
        or     eax, eax
        jne    loc_1_5BE1
        mov    eax, dword ptr [esp + 0x10]
        shr    eax, 3
        mov    dword ptr [esp + 0x20], eax
        sub    esi, esi
        mov    dword ptr [esp + 0x18], esi
        cmp    eax, esi
        je     loc_1_58CE
        lea    edi, [esp + 0xc0]
        lea    ebx, [esp + 0x140]
        sub    ebp, ebp
loc_1_5795:
        mov    eax, dword ptr [ebx]
        cmp    eax, 0x200
        jne    loc_1_58B9
        mov    dword ptr [esp + 0x50], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esp + 0x54], eax
        mov    dword ptr [esp + 0x58], 0x100
        lea    eax, [esp + 0x28]
        mov    dword ptr [esp + 0x5c], 0x100
        mov    dword ptr [esp + 0x60], 1
        mov    dword ptr [esp + 0x10], 4
        mov    dword ptr [esp + 0x4c], 0x4655424e
        mov    dword ptr [esp + 0x3c], ebp
        mov    dword ptr [esp + 0x48], 4
        mov    dword ptr [esp + 0x40], eax
        lea    eax, [esp + 0x2c]
        mov    dword ptr [esp + 0x44], ebp
        mov    dword ptr [eax], ebp
        mov    dword ptr [eax + 4], ebp
        mov    dword ptr [eax + 8], ebp
        push   eax
        mov    dword ptr [eax + 0xc], ebp
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x44]
        push   eax
        lea    eax, [esp + 0x5c]
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        push   ebp
        call   dword ptr [eax + 0x48]
        add    esp, 0x14
        cmp    eax, ebp
        jne    loc_1_5BE1
        cmp    dword ptr [esp + 0x28], 0x202
        jne    loc_1_58B9
        mov    dword ptr [esp + 0x10], 0xde
        push   0xde
        call   sub_1_35F0
        add    esp, 4
        mov    dword ptr [edi], eax
        cmp    eax, ebp
        mov    ecx, dword ptr [0x13494]      ; -> loc_1_13494
        je     loc_1_5AD3
        mov    dword ptr [esp + 0x58], 0x200
        mov    dword ptr [esp + 0x60], 1
        mov    dword ptr [esp + 0x10], 0xde
        mov    dword ptr [esp + 0x4c], 0x4655424e
        mov    dword ptr [esp + 0x3c], ebp
        mov    dword ptr [esp + 0x48], 0xde
        mov    dword ptr [esp + 0x40], eax
        lea    eax, [esp + 0x2c]
        mov    dword ptr [esp + 0x44], ebp
        mov    dword ptr [eax], ebp
        mov    dword ptr [eax + 4], ebp
        mov    dword ptr [eax + 8], ebp
        push   eax
        mov    dword ptr [eax + 0xc], ebp
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x44]
        push   eax
        lea    eax, [esp + 0x5c]
        push   eax
        push   ebp
        call   dword ptr [ecx + 0x48]
        add    esp, 0x14
        cmp    eax, ebp
        jne    loc_1_5B00
        add    edi, 4
        inc    esi
loc_1_58B9:
        add    ebx, 8
        inc    dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [esp + 0x20], eax
        ja     loc_1_5795
loc_1_58CE:
        mov    dword ptr [esp + 0x18], 0
        cmp    dword ptr [esp + 0x20], 0
        je     loc_1_5BC5
        lea    eax, [esp + 0x140]
        mov    dword ptr [esp + 0x1c], eax
loc_1_58EC:
        mov    eax, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [eax]
        cmp    eax, 0x301
        jne    loc_1_5BAE
        mov    dword ptr [esp + 0x50], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 0x54], eax
        mov    dword ptr [esp + 0x58], 0x100
        lea    eax, [esp + 0x24]
        mov    dword ptr [esp + 0x5c], 0x100
        mov    dword ptr [esp + 0x60], 1
        mov    dword ptr [esp + 0x10], 4
        mov    dword ptr [esp + 0x4c], 0x4655424e
        mov    dword ptr [esp + 0x3c], 0
        mov    dword ptr [esp + 0x48], 4
        mov    dword ptr [esp + 0x40], eax
        lea    eax, [esp + 0x2c]
        mov    dword ptr [esp + 0x44], 0
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        push   eax
        mov    dword ptr [eax + 0xc], 0
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x44]
        push   eax
        lea    eax, [esp + 0x5c]
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        push   0
        call   dword ptr [eax + 0x48]
        add    esp, 0x14
        or     eax, eax
        jne    loc_1_5BEC
        cmp    dword ptr [esp + 0x24], 0x303
        jne    loc_1_5BAE
        mov    dword ptr [esp + 0x58], 0x200
        mov    dword ptr [esp + 0x60], 1
        mov    dword ptr [esp + 0x10], 0x5c
        mov    dword ptr [esp + 0x4c], 0x4655424e
        lea    eax, [esp + 0x64]
        mov    dword ptr [esp + 0x3c], 0
        mov    dword ptr [esp + 0x48], 0x5c
        mov    dword ptr [esp + 0x40], eax
        lea    eax, [esp + 0x2c]
        mov    dword ptr [esp + 0x44], 0
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        push   eax
        mov    dword ptr [eax + 0xc], 0
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x44]
        push   eax
        lea    eax, [esp + 0x5c]
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        push   0
        call   dword ptr [eax + 0x48]
        add    esp, 0x14
        or     eax, eax
        jne    loc_1_5BAE
        cmp    dword ptr [esp + 0xb8], 1
        jb     loc_1_5BAE
        mov    eax, dword ptr [esp + 0xb8]
        lea    eax, [eax + eax*2]
        shl    eax, 3
        mov    dword ptr [esp + 0x10], eax
        push   eax
        call   sub_1_35F0
        mov    dword ptr [esp + 0x18], eax
        add    esp, 4
        or     eax, eax
        je     loc_1_5BAE
        mov    dword ptr [esp + 0x60], 0x102
        mov    dword ptr [esp + 0x4c], 0x4655424e
        mov    dword ptr [esp + 0x3c], 0
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esp + 0x48], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x40], eax
        lea    eax, [esp + 0x2c]
        mov    dword ptr [esp + 0x44], 0
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        push   eax
        mov    dword ptr [eax + 0xc], 0
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x44]
        push   eax
        lea    eax, [esp + 0x5c]
        push   eax
        mov    eax, dword ptr [0x13494]      ; -> loc_1_13494
        push   0
        call   dword ptr [eax + 0x48]
        add    esp, 0x14
        or     eax, eax
        je     loc_1_5B30
        jmp    loc_1_5BA1
loc_1_5AD3:
        or     esi, esi
        je     loc_1_5BE1
        lea    edi, [esp + 0xc0]
loc_1_5AE2:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0024
        add    esp, 4
        add    edi, 4
        dec    esi
        jne    loc_1_5AE2
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x230
        ret    
        db  08Bh, 0FFh
loc_1_5B00:
        or     esi, esi
        je     loc_1_5BE1
        lea    edi, [esp + 0xc0]
        nop    
loc_1_5B10:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0024
        add    esp, 4
        add    edi, 4
        dec    esi
        jne    loc_1_5B10
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x230
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_5B30:
        sub    edx, edx
        cmp    dword ptr [esp + 0xb8], edx
        jbe    loc_1_5B9A
        mov    ecx, dword ptr [esp + 0x14]
        nop    
loc_1_5B40:
        mov    eax, dword ptr [esp + 0x244]
        cmp    dword ptr [ecx], eax
        jne    loc_1_5B8D
        sub    edi, edi
        cmp    esi, edi
        je     loc_1_5B8D
        lea    ebx, [esp + 0xc0]
        mov    eax, dword ptr [ecx + 4]
loc_1_5B5B:
        mov    ebp, dword ptr [ebx]
        cmp    dword ptr [ebp], eax
        je     loc_1_5B70
        add    ebx, 4
        inc    edi
        cmp    esi, edi
        ja     loc_1_5B5B
        jmp    loc_1_5B8D
        db  08Dh, 064h, 024h, 000h
loc_1_5B70:
        mov    eax, dword ptr [esp + edi*4 + 0xc0]
        add    eax, 0x14
        mov    ebx, dword ptr [esp + 0x248]
        mov    edi, dword ptr [eax]
        mov    dword ptr [ebx], edi
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebx + 4], ax
loc_1_5B8D:
        add    ecx, 0x18
        inc    edx
        cmp    dword ptr [esp + 0xb8], edx
        ja     loc_1_5B40
loc_1_5B9A:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_5BAE
loc_1_5BA1:
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_5BAE:
        add    dword ptr [esp + 0x1c], 8
        inc    dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [esp + 0x20], eax
        ja     loc_1_58EC
loc_1_5BC5:
        or     esi, esi
        je     loc_1_5BE1
        lea    edi, [esp + 0xc0]
loc_1_5BD0:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0024
        add    esp, 4
        add    edi, 4
        dec    esi
        jne    loc_1_5BD0
loc_1_5BE1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x230
        ret    
loc_1_5BEC:
        or     esi, esi
        je     loc_1_5BE1
        lea    edi, [esp + 0xc0]
loc_1_5BF7:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0024
        add    esp, 4
        add    edi, 4
        dec    esi
        jne    loc_1_5BF7
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x230
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5C20:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        push   2
        mov    edi, dword ptr [esp + 0x10]
        push   esi
        push   edi
        call   sub_1_5D30
        add    esp, 0xc
        or     eax, eax
        je     loc_1_5C40
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_5C40:
        push   0
        push   edi
        call   sub_3_0270
        add    esp, 8
        mov    dword ptr [esi], eax
        or     eax, eax
        je     loc_1_5C56
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
loc_1_5C56:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_5C60:
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x18]
        mov    dword ptr [esp + 8], edi
        lea    eax, [esp + 8]
        push   0
        mov    esi, dword ptr [esp + 0x18]
        push   eax
        push   esi
        call   sub_1_5D30
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_5C93
        push   edi
        push   esi
        call   sub_3_0270
        mov    dword ptr [esp + 0x10], eax
        add    esp, 8
loc_1_5C93:
        mov    eax, dword ptr [esp + 0x1c]
        cmp    eax, dword ptr [esp + 8]
        jbe    loc_1_5CA1
        mov    dword ptr [esp + 8], eax
loc_1_5CA1:
        mov    eax, dword ptr [esp + 8]
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_1_5CB0:
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        call   sub_1_5C60
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
sub_1_5CD0:
        sub    esp, 0xc
        push   edi
        mov    edi, 0x12f54                  ; -> loc_1_12F54
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    word ptr [esp + 8], cx
        mov    ax, cx
        mov    word ptr [esp + 0xa], ax
        mov    dword ptr [esp + 0xc], 0x12f54 ; -> loc_1_12F54
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 8]
        push   0x12f5c                       ; -> obj1:0x12F5C
        push   eax
        call   sub_1_12D46
        cmp    dword ptr [esp + 4], 0
        je     loc_1_5D25
        mov    dword ptr [0x12f5c], 0        ; -> obj1:0x12F5C
        sub    eax, eax
        pop    edi
        add    esp, 0xc
        ret    
loc_1_5D25:
        mov    eax, 1
        pop    edi
        add    esp, 0xc
        ret    
        db  090h
sub_1_5D30:
        sub    esp, 0x10
        push   esi
        push   edi
        sub    esi, esi
        mov    edx, dword ptr [esp + 0x1c]
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    word ptr [esp + 0x10], cx
        mov    ax, cx
        mov    word ptr [esp + 0x12], ax
        mov    dword ptr [esp + 0x14], edx
        mov    edi, dword ptr [esp + 0x24]
        lea    eax, [esp + 0x10]
        push   edi
        push   eax
        mov    eax, dword ptr [0x12f5c]      ; -> obj1:0x12F5C
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_12E06
        cmp    dword ptr [esp + 0xc], esi
        jne    loc_1_5DE3
        cmp    edi, 2
        mov    eax, dword ptr [esp + 8]
        jne    loc_1_5DD5
        mov    ax, word ptr [eax + 4]
        inc    ax
        push   eax
        call   sub_1_35F0
        add    esp, 4
        mov    edx, eax
        cmp    edx, esi
        je     loc_1_5DE3
        mov    eax, dword ptr [esp + 8]
        mov    ecx, 0xffffffff
        mov    edi, dword ptr [eax + 8]
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [esp + 0x20]
        mov    esi, 1
        mov    dword ptr [ecx], edx
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 0x10
        ret    
loc_1_5DD5:
        mov    ecx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    esi, 1
loc_1_5DE3:
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 0x10
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_1_5DF0:
        cmp    dword ptr [0x12f5c], 0        ; -> obj1:0x12F5C
        je     loc_1_5E0E
        mov    eax, dword ptr [0x12f5c]      ; -> obj1:0x12F5C
        push   eax
        call   sub_1_12E0C
        mov    dword ptr [0x12f5c], 0        ; -> obj1:0x12F5C
loc_1_5E0E:
        ret    
        db  090h
sub_1_5E10:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [0x1327c]      ; -> loc_1_1327C
        cmp    edi, 0x1327c                  ; -> loc_1_1327C
        mov    esi, dword ptr [esp + 0x1c]
        je     loc_1_5E93
        sub    ebx, ebx
loc_1_5E2B:
        mov    ebp, edi
        mov    edi, dword ptr [edi]
        mov    ecx, dword ptr [ebp + 0x14]
        cmp    ecx, ebx
        je     loc_1_5E8B
        mov    eax, dword ptr [ecx + 0x20]
        cmp    dword ptr [eax + 0x2c], esi
        jne    loc_1_5E8B
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0xc
        cmp    dword ptr [esp + 0x14], ebx
        je     loc_1_5E8B
        mov    eax, dword ptr [ebp + 4]
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ebp + 4], ebp
        mov    dword ptr [ebp], ebp
        mov    dword ptr [ebp + 0x14], ebx
        push   ebp
        call   sub_1_DEA4
        add    esp, 4
        mov    eax, dword ptr [esp + 0x10]
        push   0xc00000ca
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        call   sub_1_E64C
        add    esp, 0xc
loc_1_5E8B:
        cmp    edi, 0x1327c                  ; -> loc_1_1327C
        jne    loc_1_5E2B
loc_1_5E93:
        mov    dword ptr [esp + 0x10], 0
        cmp    byte ptr [0x13378], 0         ; -> loc_1_13378
        je     loc_1_5EFE
        cmp    dword ptr [0x13374], 0        ; -> loc_1_13374
        je     loc_1_5EFE
        mov    eax, dword ptr [0x13374]      ; -> loc_1_13374
        mov    dword ptr [esp + 0x10], eax
        mov    edi, dword ptr [eax + 8]
        cmp    dword ptr [edi + 0x2c], esi
        jne    loc_1_5EFE
        mov    dword ptr [0x13374], 0        ; -> loc_1_13374
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [eax + 0xc]
        mov    eax, dword ptr [edi + 0x38]
        push   eax
        call   sub_1_12CC0
        add    esp, 4
        push   edi
        call   sub_1_F280
        add    esp, 4
        push   0xc00000ca
        mov    eax, dword ptr [esp + 0x18]
        push   ebx
        push   eax
        call   sub_1_E64C
        add    esp, 0xc
loc_1_5EFE:
        mov    edi, dword ptr [0x1336c]      ; -> loc_1_1336C
        cmp    edi, 0x1336c                  ; -> loc_1_1336C
        je     loc_1_5F6E
        lea    esp, [esp]
loc_1_5F10:
        mov    dword ptr [esp + 0x10], edi
        mov    eax, dword ptr [esp + 0x10]
        mov    edi, dword ptr [edi]
        mov    ebx, dword ptr [eax + 8]
        cmp    dword ptr [ebx + 0x2c], esi
        jne    loc_1_5F66
        mov    eax, dword ptr [eax + 4]
        mov    ecx, edi
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, dword ptr [eax + 0xc]
        mov    eax, dword ptr [ebx + 0x38]
        push   eax
        call   sub_1_12CC0
        add    esp, 4
        push   ebx
        call   sub_1_F280
        add    esp, 4
        push   0xc00000ca
        mov    eax, dword ptr [esp + 0x18]
        push   ebp
        push   eax
        call   sub_1_E64C
        add    esp, 0xc
loc_1_5F66:
        cmp    edi, 0x1336c                  ; -> loc_1_1336C
        jne    loc_1_5F10
loc_1_5F6E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5F80:
        push   esi
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        je     loc_1_601A
        mov    ecx, 0x12f54                  ; -> loc_1_12F54
loc_1_5F92:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    loc_1_5FB2
        or     dl, dl
        je     loc_1_5FAE
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    loc_1_5FB2
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    loc_1_5F92
loc_1_5FAE:
        sub    eax, eax
        jmp    loc_1_5FB7
loc_1_5FB2:
        sbb    eax, eax
        sbb    eax, -1
loc_1_5FB7:
        or     eax, eax
        jne    loc_1_601A
        mov    ecx, dword ptr [0x12f80]      ; -> obj1:0x12F80
        mov    eax, ecx
        mov    esi, dword ptr [ecx + 8]
        sub    eax, esi
        cmp    eax, -8
        je     loc_1_5FF0
loc_1_5FD0:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        mov    eax, dword ptr [eax + 0x58]
        push   eax
        push   ecx
        call   sub_1_5110
        add    esp, 8
        mov    ecx, dword ptr [0x12f80]      ; -> obj1:0x12F80
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, -8
        jne    loc_1_5FD0
loc_1_5FF0:
        push   0
        call   sub_1_3CF0
        add    esp, 4
        push   0
        push   0x4ef0                        ; -> loc_1_4EF0
        call   sub_1_645E
        add    esp, 8
        call   sub_1_6020
        push   0
        call   sub_1_0042
        add    esp, 4
        pop    esi
        ret    
loc_1_601A:
        sub    eax, eax
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_6020:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_4D00
        cmp    dword ptr [0x133e4], 0x133e4  ; -> loc_1_133E4
        je     loc_1_605A
        mov    esi, dword ptr [0x133e4]      ; -> loc_1_133E4
        mov    edi, edi
loc_1_6040:
        lea    eax, [esi - 0x18]
        mov    esi, dword ptr [esi]
        push   0
        push   0
        push   eax
        call   sub_1_10AEC
        add    esp, 0xc
        cmp    esi, 0x133e4                  ; -> loc_1_133E4
        jne    loc_1_6040
loc_1_605A:
        cmp    dword ptr [0x133ec], 0x133ec  ; -> loc_1_133EC
        je     loc_1_6098
        mov    esi, 0x133ec                  ; -> loc_1_133EC
        add    eax, 0
loc_1_6070:
        mov    edx, dword ptr [0x133ec]      ; -> loc_1_133EC
        mov    eax, edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        sub    edx, 0x18
        mov    eax, dword ptr [eax + 4]
        push   edx
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        call   sub_1_0024
        add    esp, 4
        cmp    dword ptr [0x133ec], esi      ; -> loc_1_133EC
        jne    loc_1_6070
loc_1_6098:
        mov    eax, dword ptr [0x12ec0]      ; -> obj1:0x12EC0
        or     eax, eax
        je     loc_1_60AA
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_60AA:
        cmp    dword ptr [0x1332f], 0        ; -> obj1:0x1332F
        je     loc_1_60C1
        mov    eax, dword ptr [0x1332f]      ; -> obj1:0x1332F
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_60C1:
        cmp    dword ptr [0x132d8], 0        ; -> loc_1_132D8
        je     loc_1_60D8
        mov    eax, dword ptr [0x132d8]      ; -> loc_1_132D8
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_60D8:
        cmp    dword ptr [0x13327], 0        ; -> obj1:0x13327
        je     loc_1_60EF
        mov    eax, dword ptr [0x13327]      ; -> obj1:0x13327
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_60EF:
        cmp    dword ptr [0x13333], 0        ; -> obj1:0x13333
        je     loc_1_6106
        mov    eax, dword ptr [0x13333]      ; -> obj1:0x13333
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_6106:
        cmp    dword ptr [0x13337], 0        ; -> obj1:0x13337
        je     loc_1_611D
        mov    eax, dword ptr [0x13337]      ; -> obj1:0x13337
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_611D:
        cmp    dword ptr [0x13274], 0x13274  ; -> loc_1_13274
        je     loc_1_61D0
        lea    ecx, [ecx]
loc_1_6130:
        mov    edx, dword ptr [0x13274]      ; -> loc_1_13274
        mov    eax, edx
        mov    dword ptr [esp + 0x10], edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        lea    esi, [edx + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi]
        sub    eax, edx
        cmp    eax, 0xc
        je     loc_1_61B3
loc_1_6154:
        mov    edi, dword ptr [esi]
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        lea    ebx, [edi + 0x1c]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebx]
        sub    eax, edi
        cmp    eax, 0x1c
        je     loc_1_619F
loc_1_6170:
        mov    ebp, dword ptr [ebx]
        mov    ecx, dword ptr [ebp]
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        or     eax, eax
        je     loc_1_618D
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_618D:
        push   ebp
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [ebx]
        sub    eax, edi
        cmp    eax, 0x1c
        jne    loc_1_6170
loc_1_619F:
        push   edi
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [esi]
        sub    eax, dword ptr [esp + 0x10]
        cmp    eax, 0xc
        jne    loc_1_6154
loc_1_61B3:
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        call   sub_1_0024
        add    esp, 4
        cmp    dword ptr [0x13274], 0x13274  ; -> loc_1_13274
        jne    loc_1_6130
loc_1_61D0:
        cmp    dword ptr [0x1348c], 0x1348c  ; -> loc_1_1348C
        je     loc_1_6216
        mov    edi, 0x1348c                  ; -> loc_1_1348C
loc_1_61E1:
        mov    esi, dword ptr [0x1348c]      ; -> loc_1_1348C
        mov    eax, esi
        mov    ecx, dword ptr [eax]
        mov    eax, esi
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 0xc]
        or     eax, eax
        je     loc_1_6205
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_6205:
        push   esi
        call   sub_1_0024
        add    esp, 4
        cmp    dword ptr [0x1348c], edi      ; -> loc_1_1348C
        jne    loc_1_61E1
loc_1_6216:
        cmp    dword ptr [0x132b4], 0        ; -> loc_1_132B4
        je     loc_1_6282
        sub    edi, edi
        sub    esi, esi
loc_1_6223:
        mov    eax, dword ptr [0x132b4]      ; -> loc_1_132B4
        cmp    dword ptr [eax], edi
        jle    loc_1_6274
        lea    ebx, [esi + 8]
        mov    eax, dword ptr [eax + esi + 8]
        sub    eax, dword ptr [0x132b4]      ; -> loc_1_132B4
        cmp    eax, ebx
        je     loc_1_626F
loc_1_6240:
        mov    eax, dword ptr [0x132b4]      ; -> loc_1_132B4
        mov    edx, dword ptr [esi + eax + 8]
        push   edx
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [0x132b4]      ; -> loc_1_132B4
        mov    eax, dword ptr [esi + eax + 8]
        sub    eax, dword ptr [0x132b4]      ; -> loc_1_132B4
        cmp    eax, ebx
        jne    loc_1_6240
loc_1_626F:
        mov    esi, ebx
        inc    edi
        jmp    loc_1_6223
loc_1_6274:
        mov    eax, dword ptr [0x132b4]      ; -> loc_1_132B4
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_6282:
        cmp    dword ptr [0x132b8], 0        ; -> loc_1_132B8
        je     loc_1_62F2
        sub    edi, edi
        sub    esi, esi
        nop    
loc_1_6290:
        mov    eax, dword ptr [0x132b8]      ; -> loc_1_132B8
        cmp    dword ptr [eax], edi
        jle    loc_1_62E4
        lea    ebx, [esi + 8]
        mov    eax, dword ptr [eax + esi + 8]
        sub    eax, dword ptr [0x132b8]      ; -> loc_1_132B8
        cmp    eax, ebx
        je     loc_1_62DF
        lea    ebx, [ebx]
loc_1_62B0:
        mov    eax, dword ptr [0x132b8]      ; -> loc_1_132B8
        mov    ecx, dword ptr [esi + eax + 8]
        push   ecx
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [0x132b8]      ; -> loc_1_132B8
        mov    eax, dword ptr [esi + eax + 8]
        sub    eax, dword ptr [0x132b8]      ; -> loc_1_132B8
        cmp    eax, ebx
        jne    loc_1_62B0
loc_1_62DF:
        mov    esi, ebx
        inc    edi
        jmp    loc_1_6290
loc_1_62E4:
        mov    eax, dword ptr [0x132b8]      ; -> loc_1_132B8
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_62F2:
        cmp    dword ptr [0x13264], 0x13264  ; -> loc_1_13264
        je     loc_1_6328
        mov    esi, 0x13264                  ; -> loc_1_13264
loc_1_6303:
        mov    edx, dword ptr [0x13264]      ; -> loc_1_13264
        mov    eax, edx
        push   edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        call   sub_1_0024
        add    esp, 4
        cmp    dword ptr [0x13264], esi      ; -> loc_1_13264
        jne    loc_1_6303
loc_1_6328:
        cmp    dword ptr [0x1327c], 0x1327c  ; -> loc_1_1327C
        je     loc_1_6365
        mov    esi, 0x1327c                  ; -> loc_1_1327C
        lea    esp, [esp]
loc_1_6340:
        mov    edx, dword ptr [0x1327c]      ; -> loc_1_1327C
        mov    eax, edx
        push   edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        call   sub_1_0024
        add    esp, 4
        cmp    dword ptr [0x1327c], esi      ; -> loc_1_1327C
        jne    loc_1_6340
loc_1_6365:
        cmp    dword ptr [0x1328c], 0x1328c  ; -> loc_1_1328C
        je     loc_1_639B
        mov    esi, 0x1328c                  ; -> loc_1_1328C
loc_1_6376:
        mov    edx, dword ptr [0x1328c]      ; -> loc_1_1328C
        mov    eax, edx
        push   edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        call   sub_1_0024
        add    esp, 4
        cmp    dword ptr [0x1328c], esi      ; -> loc_1_1328C
        jne    loc_1_6376
loc_1_639B:
        cmp    dword ptr [0x13294], 0x13294  ; -> loc_1_13294
        je     loc_1_63D5
        mov    esi, 0x13294                  ; -> loc_1_13294
        lea    esp, [esp]
loc_1_63B0:
        mov    edx, dword ptr [0x13294]      ; -> loc_1_13294
        mov    eax, edx
        push   edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        call   sub_1_0024
        add    esp, 4
        cmp    dword ptr [0x13294], esi      ; -> loc_1_13294
        jne    loc_1_63B0
loc_1_63D5:
        cmp    dword ptr [0x1329c], 0x1329c  ; -> loc_1_1329C
        je     loc_1_640B
        mov    esi, 0x1329c                  ; -> loc_1_1329C
loc_1_63E6:
        mov    edx, dword ptr [0x1329c]      ; -> loc_1_1329C
        mov    eax, edx
        push   edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        call   sub_1_0024
        add    esp, 4
        cmp    dword ptr [0x1329c], esi      ; -> loc_1_1329C
        jne    loc_1_63E6
loc_1_640B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6420:
        VxDCall  VNETBIOS.VNETBIOS_Get_Version   ; cd2000001400
        jae    loc_1_642F
        mov    eax, 0xff
        jmp    loc_1_6452
loc_1_642F:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        push   ebx
        push   edx
        mov    ebx, 1
        mov    edx, 0x64                     ; -> obj1:0x0064
        VxDCall  VNETBIOS.VNETBIOS_Register2     ; cd2005001400
        jae    loc_1_6450
        mov    eax, 0xff
loc_1_6450:
        pop    edx
        pop    ebx
loc_1_6452:
        ret    
sub_1_6453:
        mov    eax, dword ptr [esp + 4]
        VxDCall  VNETBIOS.VNETBIOS_Deregister    ; cd2004001400
        ret    
sub_1_645E:
        mov    eax, dword ptr [esp + 8]
        push   eax
        mov    eax, dword ptr [esp + 8]
        push   eax
        VxDCall  Dev_0489.Service_0006h          ; cd2006008904
        add    esp, 8
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_6480:
        push   edi
        push   esi
        push   ebx
        mov    ecx, 0
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, 0x3d02
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_1_64A3
        jmp    loc_1_64A8
loc_1_64A3:
        mov    eax, 0
loc_1_64A8:
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        pop    ebx
        pop    esi
        pop    edi
        ret    
sub_1_64B2:
        push   edi
        push   esi
        push   ebx
        mov    ecx, 0
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        mov    ebx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
        mov    edx, dword ptr [esp + 0x18]
        mov    eax, 0x3f00
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_1_64DD
        jmp    loc_1_64E2
loc_1_64DD:
        mov    eax, 0
loc_1_64E2:
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        pop    ebx
        pop    esi
        pop    edi
        ret    
sub_1_64EC:
        push   edi
        push   esi
        push   ebx
        mov    ecx, 0
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        mov    ebx, dword ptr [esp + 0x10]
        mov    eax, 0x3e00
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_1_650F
        jmp    loc_1_6514
loc_1_650F:
        mov    eax, 0
loc_1_6514:
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        pop    ebx
        pop    esi
        pop    edi
        ret    
sub_1_651E:
        push   edi
        push   esi
        push   ebx
        mov    ecx, 0
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        VMMCall  VMM.Get_Config_Directory        ; cd20b7000100
        mov    eax, edx
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        pop    ebx
        pop    esi
        pop    edi
        ret    
        db  000h, 000h
sub_1_6540:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        lea    eax, [esi*8 + 8]
        push   eax
        call   sub_1_35F0
        add    esp, 4
        mov    edx, dword ptr [esp + 0x10]
        mov    dword ptr [edx], eax
        or     eax, eax
        je     loc_1_6594
        or     esi, esi
        jle    loc_1_6583
        sub    edi, edi
        mov    ecx, esi
loc_1_6569:
        mov    eax, dword ptr [edx]
        add    eax, edi
        lea    ebx, [eax + 8]
        mov    dword ptr [eax + 0xc], ebx
        mov    eax, dword ptr [edx]
        add    eax, edi
        mov    ebx, dword ptr [eax + 0xc]
        mov    dword ptr [eax + 8], ebx
        add    edi, 8
        dec    ecx
        jne    loc_1_6569
loc_1_6583:
        mov    eax, dword ptr [edx]
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax], esi
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_6594:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_65A0:
        push   1
        push   dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 0xc]
        add    eax, 0x84
        push   eax
        call   sub_1_6540
        add    esp, 0xc
        sub    eax, eax
        ret    
        db  090h
sub_1_65BC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x28]
        push   esi
        mov    ebp, dword ptr [esp + 0x20]
        push   ebp
        mov    ebx, dword ptr [esp + 0x20]
        push   ebx
        mov    edi, dword ptr [esp + 0x20]
        push   edi
        call   sub_1_6954
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_65EC
        mov    eax, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_65EC:
        push   esi
        push   0
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x2c]
        push   ebp
        push   ebx
        push   edi
        call   sub_1_6678
        add    esp, 0x1c
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_6608:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        lea    edi, [esi + 0x34]
        lea    eax, [esp + 0xc]
        push   eax
        mov    ebx, dword ptr [esp + 0x1c]
        push   ebx
        push   edi
        push   dword ptr [0x132b8]           ; -> loc_1_132B8
        call   sub_1_6954
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_6654
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esi + 0xc]
        cmp    dword ptr [eax + 0xc], ecx
        jne    loc_1_6646
        mov    ecx, eax
        mov    eax, dword ptr [esi + 0x28]
        or     dword ptr [ecx + 0x28], eax
loc_1_6646:
        pop    edi
        mov    eax, 0xc0000001
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_6654:
        lea    eax, [esp + 0xc]
        push   eax
        push   esi
        push   0
        push   0
        push   ebx
        push   edi
        push   dword ptr [0x132b8]           ; -> loc_1_132B8
        call   sub_1_6678
        add    esp, 0x1c
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_6678:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        mov    al, byte ptr [esi + 1]
        and    eax, 0xf
        mov    cl, byte ptr [esi]
        and    ecx, 0xf
        shl    ecx, 4
        add    eax, ecx
        cdq    
        mov    ecx, dword ptr [esp + 0x18]
        idiv   dword ptr [ecx]
        mov    ebp, edx
        mov    ebx, dword ptr [esp + 0x2c]
        or     ebx, ebx
        jne    loc_1_670C
        push   0x44
        call   sub_1_001E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_67B4
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 0x11
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [ebx + 0x1c], 1
        mov    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [ecx + 4]
        or     eax, eax
        je     loc_1_66E1
        cmp    eax, 2
        mov    dword ptr [ebx + 0x24], 0xfacf0000
        jne    loc_1_66E8
loc_1_66E1:
        mov    dword ptr [ebx + 0x24], 0xdead0000
loc_1_66E8:
        cmp    dword ptr [esp + 0x28], 1
        sbb    eax, eax
        and    eax, 0xfffffffe
        add    eax, 4
        mov    dword ptr [ebx + 0x20], eax
        or     eax, 0x40
        mov    dword ptr [ebx + 0x20], eax
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [ebx + 0xc], eax
        lea    edi, [ebx + 0x34]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
loc_1_670C:
        mov    dword ptr [ebx + 0x14], 0
        mov    ax, word ptr [0x1331c]        ; -> loc_1_1331C
        mov    word ptr [ebx + 0x30], ax
        mov    ecx, dword ptr [esp + 0x18]
        lea    ecx, [ecx + ebp*8 + 8]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [ecx], ebx
        mov    ecx, dword ptr [esp + 0x18]
        cmp    dword ptr [ecx + 4], 0
        je     loc_1_6878
        mov    esi, dword ptr [esp + 0x20]
        cmp    byte ptr [esi], 0
        je     loc_1_6878
        lea    eax, [esp + 0x10]
        push   eax
        push   0
        push   esi
        push   dword ptr [esp + 0x24]
        call   sub_1_6954
        add    esp, 0x10
        or     eax, eax
        je     loc_1_686C
        cmp    eax, 0xff
        je     loc_1_686C
        sub    ebp, ebp
        mov    eax, esi
        cmp    byte ptr [esi], 0
        je     loc_1_678B
loc_1_677C:
        cmp    ebp, 0xff
        jg     loc_1_678B
        inc    ebp
        inc    eax
        cmp    byte ptr [eax], 0
        jne    loc_1_677C
loc_1_678B:
        cmp    ebp, 0xef
        jle    loc_1_67C4
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   ebx
        call   sub_1_0024
        add    esp, 4
        mov    eax, 0xc0000001
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_67B4:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_67C4:
        inc    ebp
        lea    eax, [ebp + 0x34]
        and    eax, 0xffff
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        jne    loc_1_67F4
        push   ebx
        call   sub_1_0024
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_67F4:
        mov    ecx, ebp
        mov    edi, dword ptr [esp + 0x10]
        add    edi, 0x34
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x20], 0x1010
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x1c], 2
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x14], ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    cl, byte ptr [eax + 0x34]
        and    ecx, 0xf
        shl    ecx, 4
        mov    al, byte ptr [eax + 0x35]
        and    eax, 0xf
        add    eax, ecx
        cdq    
        mov    ecx, dword ptr [esp + 0x18]
        idiv   dword ptr [ecx]
        lea    edx, [ecx + edx*8 + 8]
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [edx + 4], eax
        jmp    loc_1_687F
loc_1_686C:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 8], eax
        jmp    loc_1_687F
        db  08Dh, 049h, 000h
loc_1_6878:
        mov    dword ptr [ebx + 8], 0
loc_1_687F:
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [eax], ebx
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h, 083h, 0ECh, 004h, 08Dh, 044h, 024h, 000h, 050h, 06Ah, 000h, 0FFh, 074h, 024h, 014h, 0FFh
        db  074h, 024h, 014h, 0E8h, 0ADh, 000h, 000h, 000h, 083h, 0C4h, 010h, 00Bh, 0C0h, 074h, 012h, 03Dh
        db  0FFh, 000h, 000h, 000h, 074h, 00Bh, 0B8h, 001h, 000h, 000h, 0C0h, 083h, 0C4h, 004h, 0C3h, 08Bh
        db  0FFh, 08Bh, 044h, 024h, 000h, 081h, 060h, 020h, 00Fh, 0FFh, 0FFh, 0FFh, 08Bh, 044h, 024h, 000h
        db  083h, 048h, 020h, 040h, 0FFh, 074h, 024h, 000h, 0E8h, 0C8h, 075h, 000h, 000h, 083h, 0C4h, 004h
        db  02Bh, 0C0h, 083h, 0C4h, 004h, 0C3h, 08Dh, 049h, 000h, 083h, 0ECh, 004h, 056h, 08Dh, 044h, 024h
        db  004h, 050h, 0FFh, 074h, 024h, 018h, 0FFh, 074h, 024h, 018h, 0FFh, 074h, 024h, 018h, 0E8h, 052h
        db  000h, 000h, 000h, 083h, 0C4h, 010h, 00Bh, 0C0h, 074h, 013h, 03Dh, 0FFh, 000h, 000h, 000h, 074h
        db  00Ch, 0B8h, 001h, 000h, 000h, 0C0h, 05Eh, 083h, 0C4h, 004h, 0C3h, 08Bh, 0FFh, 08Bh, 044h, 024h
        db  004h, 081h, 060h, 020h, 00Fh, 0FFh, 0FFh, 0FFh, 08Bh, 044h, 024h, 004h, 08Bh, 074h, 024h, 018h
        db  009h, 070h, 020h, 083h, 0FEh, 020h, 075h, 013h, 0FFh, 074h, 024h, 004h, 0E8h, 064h, 075h, 000h
        db  000h, 083h, 0C4h, 004h, 08Bh, 044h, 024h, 004h, 009h, 070h, 020h, 02Bh, 0C0h, 05Eh, 083h, 0C4h
        db  004h, 0C3h, 08Dh, 049h, 000h
sub_1_6954:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x1c]
        mov    cl, byte ptr [eax]
        and    ecx, 0xf
        shl    ecx, 4
        mov    al, byte ptr [eax + 1]
        and    eax, 0xf
        add    eax, ecx
        cdq    
        mov    esi, dword ptr [esp + 0x18]
        idiv   dword ptr [esi]
        lea    eax, [esi + edx*8 + 8]
        mov    dword ptr [esp + 0x10], eax
        mov    edi, eax
        mov    ebx, dword ptr [esp + 0x20]
loc_1_6984:
        mov    edi, dword ptr [edi]
        cmp    edi, dword ptr [esp + 0x10]
        je     loc_1_6A24
        test   byte ptr [edi + 0x21], 0x10
        mov    ebp, 0x10
        je     loc_1_699E
        mov    ebp, dword ptr [edi + 0x14]
loc_1_699E:
        push   ebp
        lea    eax, [edi + 0x34]
        push   eax
        push   dword ptr [esp + 0x24]
        call   sub_1_0262
        add    esp, 0xc
        cmp    eax, ebp
        jne    loc_1_6984
        or     ebx, ebx
        je     loc_1_6A14
        cmp    esi, dword ptr [0x132b4]      ; -> loc_1_132B4
        jne    loc_1_69D0
        mov    ecx, dword ptr [0x132d8]      ; -> loc_1_132D8
        movzx  ebp, word ptr [0x132d4]       ; -> loc_1_132D4
        jmp    loc_1_69F2
        db  08Bh, 0FFh
loc_1_69D0:
        mov    eax, dword ptr [edi + 8]
        or     eax, eax
        jne    loc_1_69EC
        cmp    byte ptr [ebx], 0
        jne    loc_1_6984
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [eax], edi
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_69EC:
        lea    ecx, [eax + 0x34]
        mov    ebp, dword ptr [eax + 0x14]
loc_1_69F2:
        push   ebp
        push   ecx
        push   ebx
        call   sub_1_0262
        add    esp, 0xc
        cmp    eax, ebp
        jne    loc_1_6984
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [eax], edi
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_6A14:
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [eax], edi
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_6A24:
        pop    ebp
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_6A34:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x30]
        mov    dword ptr [esi], 0
        mov    edi, dword ptr [esp + 0x20]
        mov    ebx, dword ptr [edi + 0xc]
        cmp    dword ptr [ebx + 0x44], 0
        jne    loc_1_6AC0
        mov    ecx, dword ptr [esp + 0x34]
        mov    al, byte ptr [ecx]
        or     al, al
        jne    loc_1_6A9C
        mov    eax, dword ptr [ecx]
        mov    edx, eax
        and    edx, 0xff00
        shl    edx, 8
        mov    ebp, eax
        and    ebp, 0xff0000
        shr    ebp, 8
        or     edx, ebp
        shr    eax, 0x18
        and    eax, 0xff
        or     edx, eax
        mov    dword ptr [ebx + 0x3c], edx
        add    ecx, 4
        sub    dword ptr [esp + 0x28], 4
        mov    ebp, dword ptr [esp + 0x2c]
        sub    ebp, 4
        mov    dword ptr [esi], 4
        jmp    loc_1_6AC8
        db  08Bh, 0FFh
loc_1_6A9C:
        cmp    al, 0x85
        jne    loc_1_6AB0
        mov    dword ptr [esi], 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_6AB0:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 0x28]
        mov    dword ptr [esi], ecx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_6AC0:
        mov    ebp, dword ptr [esp + 0x2c]
        mov    ecx, dword ptr [esp + 0x34]
loc_1_6AC8:
        mov    edx, dword ptr [ebx + 0x1c]
        mov    eax, edx
        sub    eax, ebx
        cmp    eax, 0x1c
        je     loc_1_6AF4
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    esi, dword ptr [esp + 0x38]
        mov    dword ptr [esi], edx
        mov    eax, 0x18
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_6AF4:
        mov    edx, dword ptr [ebx + 0x10]
        cmp    dword ptr [edx + 0x50], 0
        je     loc_1_6C20
        mov    eax, dword ptr [ebx + 0x3c]
        sub    eax, dword ptr [ebx + 0x44]
        cmp    ebp, eax
        jb     loc_1_6B20
        mov    ebp, eax
        cmp    dword ptr [esp + 0x28], eax
        jbe    loc_1_6B17
        mov    dword ptr [esp + 0x28], eax
loc_1_6B17:
        or     word ptr [esp + 0x24], 0xc00
        jmp    loc_1_6B2E
loc_1_6B20:
        and    word ptr [esp + 0x24], 0xfbff
        or     word ptr [esp + 0x24], 0x800
loc_1_6B2E:
        lea    eax, [esp + 0x14]
        mov    dword ptr [esp + 0x10], 0
        push   eax
        push   ecx
        lea    eax, [esp + 0x18]
        push   eax
        push   ebp
        push   dword ptr [esp + 0x38]
        movzx  eax, word ptr [esp + 0x38]
        push   eax
        push   dword ptr [ebx + 0x24]
        push   dword ptr [edx + 0x54]
        call   dword ptr [edx + 0x50]
        add    esp, 0x20
        cmp    dword ptr [edi + 0xc], 0
        jne    loc_1_6B88
        cmp    eax, 0x18
        jne    loc_1_6C25
        push   0
        push   0xc0000120
        push   dword ptr [esp + 0x1c]
        call   sub_1_25A0
        add    esp, 0xc
        mov    dword ptr [esi], ebp
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_6B88:
        cmp    eax, 0x18
        jne    loc_1_6BAC
        mov    ecx, dword ptr [esp + 0x10]
        add    dword ptr [ebx + 0x44], ecx
        mov    ecx, dword ptr [esp + 0x10]
        add    dword ptr [esi], ecx
        mov    ecx, dword ptr [esp + 0x14]
        mov    esi, dword ptr [esp + 0x38]
        mov    dword ptr [esi], ecx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_6BAC:
        or     eax, eax
        jne    loc_1_6BE8
        mov    edx, dword ptr [ebx + 0x44]
        add    edx, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 0x44], edx
        mov    ecx, dword ptr [esp + 0x10]
        add    dword ptr [esi], ecx
        mov    ecx, dword ptr [ebx + 0x3c]
        cmp    ecx, edx
        jne    loc_1_6BD8
        mov    dword ptr [ebx + 0x44], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_6BD8:
        jae    loc_1_6C25
        mov    ecx, dword ptr [esp + 0x28]
        mov    dword ptr [esi], ecx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_6BE8:
        cmp    eax, 0x17
        jne    loc_1_6C25
        cmp    dword ptr [ebx + 0x3c], 0
        jne    loc_1_6C10
        cmp    dword ptr [ebx + 0x68], 7
        jne    loc_1_6C10
        mov    word ptr [edi + 0x32], 3
        mov    dword ptr [ebx + 0x48], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_6C10:
        cmp    dword ptr [esi], 0
        je     loc_1_6C25
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_6C20:
        mov    eax, 0x17
loc_1_6C25:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
sub_1_6C30:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 0x30]
        mov    ecx, dword ptr [esp + 0x38]
        mov    dword ptr [ecx], edx
        mov    esi, dword ptr [esp + 0x3c]
        mov    cl, byte ptr [esi]
        cmp    cl, 0x85
        jne    loc_1_6C58
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_6C58:
        mov    eax, dword ptr [esp + 0x28]
        cmp    dword ptr [eax + 0x38], 4
        jne    loc_1_6E8C
        cmp    cl, 0x81
        je     loc_1_6C90
        push   1
        push   0x8f
        push   0x83
        push   dword ptr [esp + 0x34]
        call   sub_1_72A4
        add    esp, 0x10
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_6C90:
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   edx
        push   dword ptr [esp + 0x38]
        push   esi
        call   sub_1_7324
        add    esp, 0x18
        or     eax, eax
        je     loc_1_6CD0
        push   1
        push   eax
        push   0x83
        push   dword ptr [esp + 0x34]
        call   sub_1_72A4
        add    esp, 0x10
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_6CD0:
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, dword ptr [eax + 0x2c]
        mov    eax, esi
        sub    eax, dword ptr [esp + 0x10]
        cmp    eax, 0x2c
        je     loc_1_6E48
        sub    ebx, ebx
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, esi
        cmp    eax, -0x2c
        je     loc_1_6D2B
        nop    
loc_1_6CF4:
        mov    eax, dword ptr [esi + 0x1c]
        or     eax, eax
        je     loc_1_6DD8
        mov    eax, dword ptr [eax + 0x14]
        or     eax, eax
        je     loc_1_6DD8
        push   0x10
        mov    ecx, dword ptr [esp + 0x18]
        add    ecx, 0xa
        push   ecx
        add    eax, 0xa
        push   eax
        call   sub_1_0262
        add    esp, 0xc
        cmp    eax, 0x10
        jne    loc_1_6DDE
        mov    ebx, esi
loc_1_6D2B:
        or     ebx, ebx
        je     loc_1_6E48
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [ebx + 0x20]
        or     eax, eax
        je     loc_1_6D61
        mov    edi, dword ptr [eax + 0x14]
        or     edi, edi
        je     loc_1_6D61
        mov    eax, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_6D61:
        mov    ebp, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [ebp + 0xc], eax
        mov    dword ptr [ebp + 0x68], 5
        mov    dword ptr [ebp + 0x60], 0
        mov    dword ptr [eax + 0xc], ebp
        mov    dword ptr [eax + 0x38], 5
        mov    word ptr [eax + 0x32], 0
        mov    ecx, dword ptr [ebp]
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ebp + 0x10]
        add    ecx, 0x1c
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebp], ecx
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        mov    dword ptr [ecx + 4], ebp
        mov    esi, dword ptr [esp + 0x14]
        lea    edi, [ebp + 0x28]
        add    esi, 0xa
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        test   byte ptr [ebx + 0x18], 1
        jne    loc_1_6DF4
        push   dword ptr [esp + 0x1c]
        push   dword ptr [ebx + 8]
        push   ebp
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x20]
        call   sub_1_6F0C
        add    esp, 0x14
        jmp    loc_1_6E12
        db  08Bh, 0FFh
loc_1_6DD8:
        or     ebx, ebx
        jne    loc_1_6DDE
        mov    ebx, esi
loc_1_6DDE:
        mov    esi, dword ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, esi
        cmp    eax, -0x2c
        jne    loc_1_6CF4
        jmp    loc_1_6D2B
loc_1_6DF4:
        push   ebp
        inc    dword ptr [ebp + 0x64]
        push   dword ptr [esp + 0x2c]
        call   sub_1_6E98
        add    esp, 8
        push   ebp
        push   0
        push   dword ptr [ebx + 8]
        call   sub_1_25A0
        add    esp, 0xc
loc_1_6E12:
        push   dword ptr [esp + 0x14]
        call   sub_1_0024
        add    esp, 4
        push   ebx
        call   sub_1_0024
        add    esp, 4
        push   dword ptr [esp + 0x10]
        call   sub_1_DD58
        add    esp, 4
        mov    eax, dword ptr [esp + 0x34]
        mov    ecx, dword ptr [esp + 0x38]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_6E48:
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [eax + 0x4c], 0
        jne    loc_1_6E80
        push   1
        push   0x80
        push   0x83
        push   dword ptr [esp + 0x34]
        call   sub_1_72A4
        add    esp, 0x10
        push   dword ptr [esp + 0x10]
        call   sub_1_DD58
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_6E80:
        push   dword ptr [esp + 0x14]
        call   sub_1_0024
        add    esp, 4
loc_1_6E8C:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
sub_1_6E98:
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 0x48], 0
        mov    dword ptr [ecx + 0x40], 0
        mov    dword ptr [ecx + 0x44], 0
        mov    dword ptr [ecx + 0x3c], 0
        mov    dword ptr [ecx + 0x50], 0
        mov    dword ptr [ecx + 0x60], 0
        mov    dword ptr [ecx + 0x54], 0
        mov    dword ptr [ecx + 0x5c], 0
        mov    dword ptr [ecx + 0x58], 0
        mov    byte ptr [ecx + 0x6f], 0
        mov    byte ptr [ecx + 0x70], 0
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [ecx + 0xc], eax
        lea    edx, [ecx + 0x1c]
        mov    dword ptr [ecx + 0x20], edx
        mov    dword ptr [edx], edx
        mov    dword ptr [eax + 0xc], ecx
        mov    word ptr [eax + 0x32], 0
        mov    dword ptr [eax + 0x20], 0
        mov    dword ptr [eax + 0x24], 0
        ret    
        db  08Bh, 0FFh
sub_1_6F0C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   esi
        mov    edi, dword ptr [esp + 0x14]
        push   edi
        call   sub_1_6E98
        add    esp, 8
        mov    dword ptr [esi + 0x68], 7
        mov    dword ptr [edi + 0x38], 7
        mov    ecx, dword ptr [esi + 0x10]
        add    ecx, 0x1c
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi], ecx
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 4], esi
        inc    dword ptr [esi + 0x64]
        push   0
        push   0x82
        push   edi
        call   sub_1_11788
        add    esp, 0xc
        or     eax, eax
        je     loc_1_6FB4
        cmp    eax, 0xff
        je     loc_1_6FB4
        push   1
        push   0x83
        push   0x83
        push   edi
        call   sub_1_72A4
        add    esp, 0x10
        push   esi
        call   sub_1_BCD4
        add    esp, 4
        lea    eax, [esp + 0x18]
        push   eax
        push   esi
        call   sub_1_727C
        add    esp, 8
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_6FC3
        push   0
        push   0xc0000001
        push   dword ptr [esp + 0x20]
        call   sub_1_25A0
        add    esp, 0xc
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
loc_1_6FB4:
        push   esi
        push   0
        push   dword ptr [esp + 0x20]
        call   sub_1_25A0
        add    esp, 0xc
loc_1_6FC3:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
sub_1_6FC8:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x28]
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [esp + 0x30]
        mov    ecx, dword ptr [esp + 0x38]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x3c]
        cmp    byte ptr [eax], 0x85
        jne    loc_1_6FF8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_6FF8:
        mov    esi, dword ptr [ebp + 0xc]
        or     esi, esi
        je     loc_1_7260
        cmp    dword ptr [esi + 0x68], 6
        jne    loc_1_7260
        mov    edi, dword ptr [esi + 0x60]
        or     edi, edi
        je     loc_1_726F
        mov    dword ptr [esi + 0x60], 0
        mov    eax, dword ptr [edi + 0x30]
        or     eax, eax
        je     loc_1_703A
        push   0
        push   0
        push   eax
        call   sub_1_10AEC
        add    esp, 0xc
        mov    dword ptr [edi + 0x30], 0
loc_1_703A:
        mov    eax, dword ptr [esp + 0x3c]
        mov    dl, byte ptr [eax]
        cmp    dl, 0x82
        jne    loc_1_70B0
        mov    dword ptr [esi + 0x44], 0
        mov    dword ptr [esi + 0x68], 7
        mov    dword ptr [ebp + 0x38], 7
        mov    eax, dword ptr [edi + 0x44]
        or     eax, eax
        jne    loc_1_7070
        push   5
        push   edi
        call   sub_1_D5C0
        add    esp, 8
        jmp    loc_1_7074
        db  08Bh, 0FFh
loc_1_7070:
        dec    eax
        mov    dword ptr [edi + 0x44], eax
loc_1_7074:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x60], 0
        lea    eax, [esp + 0x10]
        push   eax
        push   esi
        call   sub_1_727C
        add    esp, 8
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_726F
        push   esi
        push   0
        push   dword ptr [esp + 0x18]
        call   sub_1_25A0
        add    esp, 0xc
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_70B0:
        mov    eax, dword ptr [esi + 0x68]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [esp + 0x3c]
        movzx  eax, byte ptr [eax + 4]
        mov    dword ptr [esp + 0x14], eax
        cmp    dl, 0x84
        jne    loc_1_71B4
        mov    al, byte ptr [esi + 0x6e]
        lea    ecx, [eax - 1]
        mov    byte ptr [esi + 0x6e], cl
        or     al, al
        je     loc_1_71B4
        sub    ebx, ebx
        mov    dword ptr [esi + 0x68], 1
        cmp    dl, 0x84
        jne    loc_1_7140
        mov    eax, dword ptr [esp + 0x3c]
        mov    ax, word ptr [eax + 8]
        mov    cx, ax
        and    ecx, 0xff
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0xffff
        or     ecx, eax
        mov    dword ptr [edi + 0x34], ecx
        mov    eax, dword ptr [esp + 0x3c]
        mov    ecx, dword ptr [eax + 4]
        mov    ebx, ecx
        and    ebx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     ebx, eax
        shl    ebx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     ebx, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     ebx, ecx
        jmp    loc_1_714C
        db  090h
loc_1_7140:
        cmp    dword ptr [esp + 0x14], 0x82
        jne    loc_1_714C
        sub    ebx, ebx
loc_1_714C:
        mov    dword ptr [ebp + 0xc], 0
        mov    dword ptr [esi + 0xc], 0
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_716A
        push   esi
        call   sub_1_BCD4
        add    esp, 4
loc_1_716A:
        mov    eax, dword ptr [esi + 0x54]
        mov    dword ptr [esi + 0x68], 2
        mov    byte ptr [esi + 0x6e], 0
        mov    dword ptr [esp + 0x10], eax
        push   esi
        call   sub_1_DEE4
        add    esp, 4
        push   0
        push   0
        push   0
        push   ebp
        call   sub_1_72A4
        add    esp, 0x10
        push   dword ptr [esp + 0x1c]
        push   0xb178                        ; -> loc_1_B178
        push   0
        push   ebx
        push   edi
        call   sub_1_3C00
        add    esp, 0x14
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_71B4:
        mov    eax, dword ptr [edi + 0x38]
        cmp    dword ptr [eax + 0x24], 0xfacf0000
        jne    loc_1_71D5
        cmp    dword ptr [eax + 0x1c], 1
        jne    loc_1_71D5
        test   byte ptr [eax + 0x20], 0x10
        je     loc_1_71D5
        push   eax
        call   sub_1_DEA4
        add    esp, 4
loc_1_71D5:
        mov    dword ptr [esi + 0x68], 1
        mov    dword ptr [esi + 0xc], 0
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_71F3
        push   esi
        call   sub_1_BCD4
        add    esp, 4
loc_1_71F3:
        mov    eax, dword ptr [edi + 0x44]
        or     eax, eax
        jne    loc_1_7208
        push   5
        push   edi
        call   sub_1_D5C0
        add    esp, 8
        jmp    loc_1_720C
        db  090h
loc_1_7208:
        dec    eax
        mov    dword ptr [edi + 0x44], eax
loc_1_720C:
        lea    eax, [esp + 0x10]
        push   eax
        push   esi
        call   sub_1_727C
        add    esp, 8
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_7244
        mov    eax, 0xc00000bc
        cmp    dword ptr [esp + 0x14], 0x82
        jne    loc_1_7235
        mov    eax, 0xc00000be
loc_1_7235:
        push   0
        push   eax
        push   dword ptr [esp + 0x18]
        call   sub_1_25A0
        add    esp, 0xc
loc_1_7244:
        push   0
        push   0
        push   0
        push   ebp
        call   sub_1_72A4
        add    esp, 0x10
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_7260:
        push   0
        push   0
        push   0
        push   ebp
        call   sub_1_72A4
        add    esp, 0x10
loc_1_726F:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
sub_1_727C:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 0x54]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        mov    dword ptr [eax + 0x54], 0
        ret    
        db  08Dh, 049h, 000h, 08Bh, 044h, 024h, 014h, 08Bh, 04Ch, 024h, 018h, 089h, 001h, 02Bh, 0C0h, 0C3h
        db  08Dh, 049h, 000h
sub_1_72A4:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    bl, bl
        cmp    byte ptr [esp + 0x20], bl
        je     loc_1_72CC
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x1c]
        mov    edi, dword ptr [esp + 0x1c]
        push   edi
        call   sub_1_11788
        add    esp, 0xc
        jmp    loc_1_72D0
        db  08Dh, 049h, 000h
loc_1_72CC:
        mov    edi, dword ptr [esp + 0x14]
loc_1_72D0:
        mov    eax, dword ptr [edi + 0x38]
        cmp    eax, 8
        jge    loc_1_7308
        cmp    eax, 3
        jle    loc_1_7308
        mov    dword ptr [edi + 0x38], 8
        mov    esi, dword ptr [edi + 0xc]
        or     esi, esi
        je     loc_1_72FB
        mov    bl, 1
        mov    dword ptr [edi + 0xc], 0
        mov    dword ptr [esi + 0xc], 0
loc_1_72FB:
        push   edi
        call   sub_1_118A8
        add    esp, 4
        jmp    loc_1_730C
        db  08Bh, 0FFh
loc_1_7308:
        mov    esi, dword ptr [esp + 0xc]
loc_1_730C:
        or     bl, bl
        je     loc_1_731A
        push   edi
        push   esi
        call   sub_1_D61C
        add    esp, 8
loc_1_731A:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_7324:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x34]
        cmp    byte ptr [esi], 0x81
        je     loc_1_7344
        mov    eax, 0x8f
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_7344:
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        mov    ebx, dword ptr [esp + 0x48]
        lea    eax, [ebx - 4]
        push   eax
        lea    eax, [esi + 4]
        push   eax
        call   sub_1_FFAC
        add    esp, 0x14
        or     eax, eax
        je     loc_1_7380
        cmp    eax, 0xff
        je     loc_1_7380
        mov    eax, 0x8f
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  090h
loc_1_7380:
        mov    edi, dword ptr [esp + 0x38]
        cmp    dword ptr [edi + 0x38], 4
        je     loc_1_7398
        mov    eax, 0xc0000001
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  090h
loc_1_7398:
        lea    eax, [esp + 0x12]
        push   eax
        push   dword ptr [esp + 0x1c]
        lea    eax, [esp + 0x28]
        push   eax
        push   0
        call   sub_1_DFA0
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_73C4
        mov    eax, 0x82
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_73C4:
        mov    eax, dword ptr [eax + 8]
        lea    ecx, [eax + 0xc]
        mov    edx, dword ptr [ecx]
        mov    ebp, edx
        sub    ebp, eax
        cmp    ebp, 0xc
        jne    loc_1_73E4
        mov    eax, 0x80
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Bh, 0FFh
loc_1_73E4:
        cmp    edx, ecx
        mov    byte ptr [esp + 0x11], 0
        je     loc_1_741C
        mov    eax, dword ptr [edi + 0x18]
loc_1_73F0:
        mov    edi, edx
        cmp    dword ptr [edx + 0x80], eax
        jne    loc_1_740A
        mov    ebp, dword ptr [edx + 0x2c]
        sub    ebp, edx
        cmp    ebp, 0x2c
        jne    loc_1_7414
        cmp    dword ptr [edx + 0x4c], 0
        jne    loc_1_7414
loc_1_740A:
        mov    edx, dword ptr [edx]
        cmp    edx, ecx
        jne    loc_1_73F0
        jmp    loc_1_7420
        db  08Bh, 0FFh
loc_1_7414:
        mov    byte ptr [esp + 0x11], 1
        jmp    loc_1_7420
        db  090h
loc_1_741C:
        mov    edi, dword ptr [esp + 0x1c]
loc_1_7420:
        cmp    byte ptr [esp + 0x11], 0
        jne    loc_1_7434
        mov    eax, 0x80
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_7434:
        inc    dword ptr [edi + 0x84]
        mov    eax, dword ptr [esp + 0x14]
        lea    eax, [eax + esi + 5]
        push   dword ptr [esp + 0x48]
        push   dword ptr [esp + 0x48]
        sub    ebx, dword ptr [esp + 0x1c]
        push   ebx
        push   eax
        call   sub_1_74A4
        add    esp, 0x10
        mov    esi, eax
        or     esi, esi
        je     loc_1_7494
        cmp    esi, 0xff
        je     loc_1_7494
        push   edi
        call   sub_1_DD58
        add    esp, 4
        cmp    esi, 1
        jne    loc_1_7484
        mov    eax, 0x83
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_7484:
        pop    ebp
        mov    eax, 0x8f
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_7494:
        mov    eax, dword ptr [esp + 0x40]
        mov    dword ptr [eax], edi
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
sub_1_74A4:
        sub    esp, 0x18
        push   esi
        push   edi
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x34]
        call   sub_1_FFAC
        add    esp, 0x14
        or     eax, eax
        je     loc_1_74D3
        cmp    eax, 0xff
        jne    loc_1_7521
loc_1_74D3:
        push   0x1a
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    loc_1_74EC
        mov    eax, 1
        pop    edi
        pop    esi
        add    esp, 0x18
        ret    
loc_1_74EC:
        mov    dword ptr [eax], 1
        mov    word ptr [eax + 4], 0x12
        mov    word ptr [eax + 6], 0x11
        mov    word ptr [eax + 8], 0
        lea    esi, [esp + 0x10]
        lea    edi, [eax + 0xa]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [esp + 0x2c]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [eax], 0x1a
        sub    eax, eax
loc_1_7521:
        pop    edi
        pop    esi
        add    esp, 0x18
        ret    
        db  090h
sub_1_7528:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [edi + 0x1c]
        mov    eax, edx
        sub    eax, edi
        cmp    eax, 0x1c
        jne    loc_1_7544
        mov    eax, 0x17
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_7544:
        mov    esi, dword ptr [esp + 0x14]
        cmp    word ptr [esi + 6], 2
        je     loc_1_7558
        mov    eax, 0x17
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_7558:
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        add    edi, 0x14
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [edx], edi
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edx + 0x38], 4
        inc    dword ptr [edx + 0x3c]
        mov    word ptr [edx + 0x32], 0
        mov    byte ptr [edx + 0x45], 0
        mov    eax, dword ptr [esi + 0xa]
        mov    dword ptr [edx + 0x34], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [eax], edx
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  090h
sub_1_7598:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 0x11], 0
        sub    ebp, ebp
        mov    dword ptr [esp + 0x14], ebp
        mov    dword ptr [esp + 0x18], ebp
        mov    byte ptr [esp + 0x13], 0
        mov    byte ptr [esp + 0x12], 0
        mov    ebx, dword ptr [esp + 0x30]
        mov    esi, dword ptr [ebx + 0xc]
        cmp    esi, ebp
        je     loc_1_76F8
        mov    eax, dword ptr [esi + 8]
        cmp    eax, 0x87654321
        je     loc_1_75E8
        cmp    eax, 0x87654320
        je     loc_1_75E8
        mov    eax, 0xc0000001
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Dh, 049h, 000h
loc_1_75E8:
        mov    di, word ptr [esi + 0x68]
        mov    cx, word ptr [ebx + 0x38]
        cmp    di, 1
        jbe    loc_1_76CD
        cmp    di, 8
        jae    loc_1_76CD
        mov    eax, dword ptr [esi + 0x54]
        or     eax, eax
        je     loc_1_761A
        cmp    di, 3
        jbe    loc_1_761A
        mov    ebp, eax
        mov    dword ptr [esi + 0x54], 0
loc_1_761A:
        cmp    di, 7
        je     loc_1_7626
        cmp    di, 5
        jne    loc_1_7638
loc_1_7626:
        mov    eax, dword ptr [esi + 0x60]
        or     eax, eax
        je     loc_1_7638
        mov    dword ptr [esp + 0x18], eax
        mov    dword ptr [esi + 0x60], 0
loc_1_7638:
        mov    eax, dword ptr [esi + 0x58]
        mov    dword ptr [esp + 0x14], eax
        or     eax, eax
        je     loc_1_764A
        mov    dword ptr [esi + 0x58], 0
loc_1_764A:
        mov    dword ptr [esi + 0x68], 1
        mov    dword ptr [esi + 0xc], 0
        mov    al, byte ptr [esp + 0x44]
        mov    byte ptr [esi + 0x6f], al
        cmp    word ptr [ebx + 0x32], 3
        jne    loc_1_7683
        cmp    byte ptr [ebx + 0x48], 1
        jne    loc_1_7683
        mov    eax, dword ptr [ebx + 0x28]
        mov    edx, dword ptr [ebx + 0x2c]
        mov    dword ptr [edx], eax
        mov    dword ptr [eax + 4], edx
        mov    byte ptr [ebx + 0x48], 0
        lea    eax, [ebx + 0x28]
        mov    dword ptr [ebx + 0x2c], eax
        mov    dword ptr [eax], eax
loc_1_7683:
        mov    dword ptr [ebx + 0x38], 8
        mov    dword ptr [ebx + 0xc], 0
        mov    byte ptr [esp + 0x13], 1
        mov    byte ptr [esp + 0x12], 1
        mov    edx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        mov    edx, dword ptr [esi + 0x10]
        add    edx, 0x14
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi], edx
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [edx + 4], esi
        cmp    dword ptr [esp + 0x44], 4
        mov    dword ptr [ebx + 0x38], 9
        je     loc_1_76CD
        mov    dword ptr [ebx + 0x38], 0
loc_1_76CD:
        cmp    cx, 8
        jne    loc_1_76DC
        mov    dword ptr [ebx + 0x38], 9
        jmp    loc_1_76EE
loc_1_76DC:
        cmp    cx, 9
        jne    loc_1_76EE
        mov    byte ptr [esp + 0x11], 1
        mov    dword ptr [ebx + 0x38], 0
loc_1_76EE:
        cmp    cx, 3
        ja     loc_1_7750
        jmp    loc_1_774D
        db  08Bh, 0FFh
loc_1_76F8:
        sub    di, di
        mov    ax, word ptr [ebx + 0x38]
        cmp    ax, di
        je     loc_1_7720
        cmp    ax, 8
        jae    loc_1_7724
        mov    byte ptr [esp + 0x12], 1
        mov    dword ptr [ebx + 0x38], 9
        cmp    dword ptr [esp + 0x44], 4
        je     loc_1_7742
        jmp    loc_1_773B
        db  090h
loc_1_7720:
        cmp    ax, 8
loc_1_7724:
        jne    loc_1_7730
        mov    dword ptr [ebx + 0x38], 9
        jmp    loc_1_7742
        db  090h
loc_1_7730:
        cmp    ax, 9
        jne    loc_1_7742
        mov    byte ptr [esp + 0x11], 1
loc_1_773B:
        mov    dword ptr [ebx + 0x38], 0
loc_1_7742:
        cmp    ax, 3
        ja     loc_1_7750
        or     ax, ax
        je     loc_1_7750
loc_1_774D:
        inc    dword ptr [ebx + 0x3c]
loc_1_7750:
        movzx  eax, word ptr [ebx + 0x32]
        mov    dword ptr [esp + 0x1c], eax
        cmp    byte ptr [esp + 0x12], 0
        je     loc_1_7779
        cmp    dword ptr [esp + 0x44], 4
        jne    loc_1_7774
        push   ebx
        call   sub_1_118A8
        add    esp, 4
        jmp    loc_1_7779
        db  08Dh, 049h, 000h
loc_1_7774:
        mov    byte ptr [esp + 0x11], 1
loc_1_7779:
        movzx  eax, di
        cmp    eax, 9
        ja     loc_1_78B3
        jmp    loc_1_7908
loc_1_778C:
        or     ebp, ebp
        je     loc_1_78B3
        cmp    byte ptr [ebx + 0x45], 0
        push   0
        je     loc_1_77A8
        push   0xc00000be
        jmp    loc_1_78AA
        db  08Bh, 0FFh
loc_1_77A8:
        push   0xc0000001
        jmp    loc_1_78AA
        db  08Bh, 0FFh
loc_1_77B4:
        or     ebp, ebp
        je     loc_1_77C8
        push   0
        push   0xc0000120
        push   ebp
        call   sub_1_25A0
        add    esp, 0xc
loc_1_77C8:
        cmp    dword ptr [esp + 0x1c], 2
        jne    loc_1_77E9
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_77E9
        push   0
        push   0xc0000120
        push   dword ptr [esp + 0x20]
        call   sub_1_25A0
        add    esp, 0xc
loc_1_77E9:
        mov    eax, dword ptr [esi + 0x10]
        mov    ecx, dword ptr [eax + 0x58]
        or     ecx, ecx
        je     loc_1_781C
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_7823
        push   2
        push   dword ptr [esp + 0x44]
        push   dword ptr [esp + 0x44]
        push   dword ptr [esp + 0x44]
        push   dword ptr [esp + 0x44]
        push   dword ptr [esi + 0x24]
        push   dword ptr [eax + 0x5c]
        call   ecx
        add    esp, 0x1c
        jmp    loc_1_7843
        db  08Dh, 049h, 000h
loc_1_781C:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_7843
loc_1_7823:
        mov    eax, 0x16
        cmp    dword ptr [esp + 0x44], 4
        je     loc_1_7834
        mov    eax, 0x14
loc_1_7834:
        push   0
        push   eax
        push   dword ptr [esp + 0x1c]
        call   sub_1_25A0
        add    esp, 0xc
loc_1_7843:
        lea    eax, [esp + 0x20]
        push   eax
        push   esi
        call   sub_1_AF30
        add    esp, 8
        jmp    loc_1_78B3
        db  090h
loc_1_7854:
        mov    edi, dword ptr [esi + 0x60]
        or     edi, edi
        je     loc_1_788B
        mov    dword ptr [esi + 0x60], 0
        mov    eax, dword ptr [edi + 0x30]
        mov    dword ptr [edi + 0x30], 0
        or     eax, eax
        je     loc_1_7880
        push   0
        lea    ecx, [esp + 0x28]
        push   ecx
        push   eax
        call   sub_1_10AEC
        add    esp, 0xc
loc_1_7880:
        push   5
        push   edi
        call   sub_1_D5C0
        add    esp, 8
loc_1_788B:
        or     ebp, ebp
        je     loc_1_78B3
        push   0
        push   0xc00000bc
        jmp    loc_1_78AA
loc_1_7898:
        mov    dword ptr [esi + 0x68], 9
loc_1_789F:
        or     ebp, ebp
        je     loc_1_78B3
        push   0
        push   0xc0000120
loc_1_78AA:
        push   ebp
        call   sub_1_25A0
        add    esp, 0xc
loc_1_78B3:
        cmp    byte ptr [esp + 0x13], 0
        je     loc_1_78C4
        push   ebx
        push   esi
        call   sub_1_D61C
        add    esp, 8
loc_1_78C4:
        cmp    byte ptr [esp + 0x11], 0
        je     loc_1_78FB
        mov    eax, dword ptr [ebx + 0x40]
        or     eax, eax
        je     loc_1_78E6
        mov    dword ptr [ebx + 0x40], 0
        push   0
        push   0
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
loc_1_78E6:
        push   dword ptr [ebx + 0x18]
        push   0x7930                        ; -> loc_1_7930
        push   0
        push   ebx
        push   0
        call   sub_1_3C00
        add    esp, 0x14
loc_1_78FB:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Dh, 049h, 000h
loc_1_7908:
        lahf   
        js     loc_1_790B
loc_1_790B:
        add    byte ptr [edi - 0x4cffff88], bl ; -> loc_1_789F
        js     loc_1_7913
loc_1_7913:
        add    byte ptr [edi + esi*2 + 0x778c0000], cl ; -> loc_1_778C
        add    byte ptr [eax], al
        mov    ah, 0x77
        add    byte ptr [eax], al
        push   esp
        js     loc_1_7923
loc_1_7923:
        add    byte ptr [edi + esi*2 + 0x78980000], dh ; -> loc_1_77B4
        add    byte ptr [eax], al
        lahf   
        js     0x792f
        db  000h
loc_1_7930:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 0xc]
        cmp    byte ptr [edi + 0x45], 0
        jne    loc_1_7968
        mov    ebx, dword ptr [edi + 0x18]
        cmp    byte ptr [edi + 0x47], 0
        push   edi
        je     loc_1_794C
        jmp    loc_1_7972
loc_1_794C:
        call   sub_1_DF20
        add    esp, 4
        push   edi
        call   sub_1_DF4C
        add    esp, 4
        push   ebx
        call   sub_1_7988
        jmp    loc_1_7977
        db  08Dh, 049h, 000h
loc_1_7968:
        push   edi
        call   sub_1_DF20
        add    esp, 4
        push   edi
loc_1_7972:
        call   sub_1_DF4C
loc_1_7977:
        add    esp, 4
        push   esi
        call   sub_1_0024
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_7988:
        push   esi
        push   0x49
        call   sub_1_001E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_79BC
        push   dword ptr [esp + 8]
        push   esi
        call   sub_1_A908
        add    esp, 8
        or     eax, eax
        je     loc_1_79BC
        cmp    eax, 0xff
        je     loc_1_79BC
        push   esi
        call   sub_1_0024
        add    esp, 4
        sub    esi, esi
loc_1_79BC:
        or     esi, esi
        jne    loc_1_79D9
        push   dword ptr [esp + 8]
        push   0x79dc                        ; -> loc_1_79DC
        push   0
        push   dword ptr [esp + 0x14]
        push   0
        call   sub_1_3C00
        add    esp, 0x14
loc_1_79D9:
        pop    esi
        ret    
        db  090h
loc_1_79DC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi + 0xc]
        call   sub_1_7988
        add    esp, 4
        push   esi
        call   sub_1_0024
        add    esp, 4
        pop    esi
        ret    
        db  090h
sub_1_79F8:
        sub    esp, 0x1c
        push   esi
        lea    eax, [esp + 0xc]
        push   eax
        lea    eax, [esp + 0xc]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        mov    eax, dword ptr [esp + 0x34]
        sub    eax, 0xe
        push   eax
        mov    esi, dword ptr [esp + 0x34]
        lea    eax, [esi + 0xe]
        push   eax
        call   sub_1_FFAC
        add    esp, 0x14
        or     eax, eax
        je     loc_1_7A32
        cmp    eax, 0xff
        jne    loc_1_7AF6
loc_1_7A32:
        mov    ecx, dword ptr [esi + 4]
        mov    esi, ecx
        and    esi, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     esi, eax
        shl    esi, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     esi, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     esi, ecx
        je     loc_1_7AF6
        lea    eax, [esp + 4]
        push   eax
        push   0
        push   esi
        push   dword ptr [esp + 0x14]
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [0x132b8]           ; -> loc_1_132B8
        call   sub_1_65BC
        add    esp, 0x18
        or     eax, eax
        je     loc_1_7A8E
        cmp    eax, 0xff
        jne    loc_1_7AF6
loc_1_7A8E:
        cmp    eax, 0xff
        mov    eax, dword ptr [esp + 4]
        jne    loc_1_7ACC
        mov    eax, dword ptr [eax + 0x20]
        test   al, 0x10
        je     loc_1_7AF6
        test   al, 8
        jne    loc_1_7AF6
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0xc], esi
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0x30], 1
        mov    eax, dword ptr [esp + 0x2c]
        mov    eax, dword ptr [eax + 0x88]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 0x28], eax
        pop    esi
        add    esp, 0x1c
        ret    
        db  090h
loc_1_7ACC:
        mov    word ptr [eax + 0x30], 1
        mov    eax, dword ptr [esp + 4]
        and    dword ptr [eax + 0x20], 0xffffff0f
        mov    eax, dword ptr [esp + 4]
        or     dword ptr [eax + 0x20], 0x10
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 0x2c]
        mov    eax, dword ptr [eax + 0x88]
        or     dword ptr [ecx + 0x28], eax
loc_1_7AF6:
        pop    esi
        add    esp, 0x1c
        ret    
        db  090h
sub_1_7AFC:
        sub    esp, 0x40
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 0x6c], 0xb
        jae    loc_1_7B18
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x40
        ret    
        db  090h
loc_1_7B18:
        mov    esi, dword ptr [esp + 0x78]
        mov    dword ptr [esp + 0x14], 0x17
        mov    al, byte ptr [esi]
        cmp    al, 0x10
        jb     loc_1_800A
        cmp    al, 0x12
        ja     loc_1_800A
        cmp    dword ptr [esp + 0x6c], 0x52
        jae    loc_1_7B4C
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x40
        ret    
        db  08Bh, 0FFh
loc_1_7B4C:
        sub    eax, eax
        lea    edi, [esi + 0xf]
        mov    ecx, 0xffffffff
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    ebx, [ecx - 1]
        or     ebx, ebx
        jg     loc_1_7B70
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x40
        ret    
        db  08Bh, 0FFh
loc_1_7B70:
        lea    ecx, [ebx + 0xf]
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        lea    eax, [esp + 0x48]
        push   eax
        mov    eax, dword ptr [esp + 0x78]
        sub    eax, ecx
        push   eax
        lea    eax, [ebx + esi]
        add    eax, 0x10
        push   eax
        call   sub_1_FFAC
        add    esp, 0x14
        or     eax, eax
        je     loc_1_7BB0
        cmp    eax, 0xff
        je     loc_1_7BB0
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x40
        ret    
loc_1_7BB0:
        movzx  eax, word ptr [0x132d4]       ; -> loc_1_132D4
        add    eax, dword ptr [esp + 0x20]
        add    eax, 0x30
        cmp    eax, dword ptr [esp + 0x6c]
        jbe    loc_1_7BD4
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x40
        ret    
        db  08Dh, 049h, 000h
loc_1_7BD4:
        cmp    byte ptr [esp + 0x40], 0x2a
        jne    loc_1_7BE7
        lea    edi, [esp + 0x41]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
loc_1_7BE7:
        lea    eax, [esp + 0x12]
        push   eax
        push   dword ptr [esp + 0x28]
        lea    eax, [esp + 0x48]
        push   eax
        push   0
        call   sub_1_DFA0
        add    esp, 0x10
        mov    ecx, dword ptr [esp + 0x20]
        lea    edi, [ecx + ebx + 0x11]
        mov    ecx, dword ptr [esp + 0x70]
        mov    dword ptr [esp + 0x2c], ecx
        sub    dword ptr [esp + 0x70], edi
        mov    ecx, dword ptr [esp + 0x54]
        mov    edx, dword ptr [ecx + 0x88]
        or     eax, eax
        je     loc_1_7EC4
        mov    ebp, dword ptr [eax + 8]
        or     ebp, ebp
        je     loc_1_7EC4
        test   dword ptr [eax + 0x28], edx
        je     loc_1_7EC4
        mov    ecx, dword ptr [ebp + 0x18]
        inc    ecx
        mov    dword ptr [ebp + 0x18], ecx
        mov    eax, dword ptr [ebp + 0xc]
        mov    ebx, eax
        sub    ebx, ebp
        cmp    ebx, 0xc
        je     loc_1_7EC0
        mov    ecx, dword ptr [esp + 0x74]
        mov    dword ptr [ecx], edi
        mov    ecx, dword ptr [esp + 0x7c]
        mov    dword ptr [ecx], 0
        mov    byte ptr [esp + 0x10], 0
        mov    ecx, dword ptr [esp + 0x80]
        mov    dword ptr [ecx], 0
        lea    ecx, [ebp + 0xc]
        cmp    eax, ecx
        je     loc_1_7DAC
loc_1_7C7D:
        mov    ebx, eax
        mov    ecx, dword ptr [eax + 0x80]
        test   dword ptr [ecx + 0x88], edx
        jne    loc_1_7C9C
        mov    eax, dword ptr [eax]
        lea    ecx, [ebp + 0xc]
        cmp    eax, ecx
        jne    loc_1_7C7D
        jmp    loc_1_7DB0
        db  090h
loc_1_7C9C:
        cmp    byte ptr [esp + 0x40], 0x2a
        je     loc_1_7CDA
        mov    ecx, dword ptr [esp + 0x54]
        mov    edx, dword ptr [ecx + 0x34]
        mov    ecx, edx
        sub    ecx, dword ptr [esp + 0x54]
        mov    dword ptr [esp + 0x1c], ecx
        cmp    ecx, 0x34
        je     loc_1_7CDA
        mov    edi, dword ptr [edx]
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [ecx], edi
        mov    dword ptr [edi + 4], ecx
        lea    edi, [ebx + 0x24]
        mov    dword ptr [edx], edi
loc_1_7CC8:
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [ecx], edx
        mov    dword ptr [edi + 4], edx
        cmp    dword ptr [esp + 0x1c], 0x34
        jne    loc_1_7CC8
loc_1_7CDA:
        mov    ecx, dword ptr [ebx + 0x24]
        sub    ecx, ebx
        cmp    ecx, 0x24
        je     loc_1_7DA0
        sub    edx, edx
        mov    ebx, eax
        lea    ecx, [ebp + 0xc]
        cmp    eax, ecx
        je     loc_1_7D22
        nop    
loc_1_7CF4:
        mov    ecx, dword ptr [eax + 0x24]
        mov    edi, ecx
        sub    edi, eax
        cmp    edi, 0x24
        je     loc_1_7D17
        mov    dword ptr [esp + 0x18], ecx
        mov    ecx, dword ptr [ecx + 0x10]
        cmp    dword ptr [esp + 0x70], ecx
        jbe    loc_1_7D20
        cmp    edx, ecx
        jae    loc_1_7D15
        mov    ebx, eax
        mov    edx, ecx
loc_1_7D15:
        mov    eax, dword ptr [eax]
loc_1_7D17:
        lea    ecx, [ebp + 0xc]
        cmp    eax, ecx
        jne    loc_1_7CF4
        jmp    loc_1_7D22
loc_1_7D20:
        mov    ebx, eax
loc_1_7D22:
        mov    eax, dword ptr [ebx + 0x24]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    edx, dword ptr [eax + 8]
        mov    ecx, dword ptr [esp + 0x7c]
        mov    dword ptr [ecx], edx
        lea    ecx, [esi + 0xe]
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        add    edx, 0xa
        push   edx
        movzx  eax, byte ptr [ecx]
        inc    eax
        push   eax
        push   ecx
        call   sub_1_FFAC
        add    esp, 0x14
        push   dword ptr [esp + 0x18]
        call   sub_1_0024
        add    esp, 4
        cmp    byte ptr [ebp + 0x23], 0
        je     loc_1_7D8C
        cmp    byte ptr [0x13350], 0         ; -> loc_1_13350
        jne    loc_1_7D8C
        mov    byte ptr [esp + 0x10], 1
        mov    dword ptr [esp + 0x14], 0
        mov    byte ptr [esp + 0x11], 1
        jmp    loc_1_7DB0
        db  08Bh, 0FFh
loc_1_7D8C:
        push   ebp
        call   sub_1_DDE4
        add    esp, 4
        mov    dword ptr [esp + 0x14], 0
        jmp    loc_1_7DB0
        db  090h
loc_1_7DA0:
        push   ebp
        call   sub_1_DDE4
        add    esp, 4
        jmp    loc_1_7DB0
        db  090h
loc_1_7DAC:
        mov    ebx, dword ptr [esp + 0x38]
loc_1_7DB0:
        cmp    byte ptr [esi], 0x12
        je     loc_1_7E2A
        mov    eax, dword ptr [esp + 0x5c]
        mov    ecx, dword ptr [eax + 0xa]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     edx, ecx
        mov    ecx, dword ptr [esp + 0x54]
        mov    edi, dword ptr [ecx + 0x5c]
        mov    eax, dword ptr [ecx + 0x58]
        and    eax, edi
        and    edi, edx
        cmp    eax, edi
        jne    loc_1_7E19
        cmp    byte ptr [esp + 0x4f], 0x1e
        je     loc_1_7E2A
        cmp    byte ptr [esp + 0x4f], 0x1d
        je     loc_1_7E2A
        cmp    byte ptr [esp + 0x4f], 1
        je     loc_1_7E2A
        push   edx
        call   sub_1_A064
        add    esp, 4
        or     al, al
        jne    loc_1_7E2A
loc_1_7E19:
        push   dword ptr [esp + 0x54]
        push   dword ptr [esp + 0x70]
        push   esi
        call   sub_1_79F8
        add    esp, 0xc
loc_1_7E2A:
        cmp    byte ptr [esp + 0x10], 0
        je     loc_1_7ECC
        push   0x16
        call   sub_1_001E
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     loc_1_7EC4
        mov    byte ptr [edi + 0x15], 0
        mov    ecx, dword ptr [esp + 0x80]
        mov    dword ptr [ecx], edi
        mov    dword ptr [edi], ebp
        mov    dword ptr [edi + 0x10], ebx
        mov    al, byte ptr [esp + 0x11]
        mov    byte ptr [edi + 0x14], al
        mov    eax, dword ptr [esp + 0x68]
        mov    dword ptr [edi + 4], eax
        lea    eax, [esp + 0x30]
        push   eax
        lea    eax, [esp + 0x38]
        push   eax
        mov    eax, dword ptr [esp + 0x74]
        sub    eax, 0xe
        push   eax
        lea    eax, [esi + 0xe]
        push   eax
        call   sub_1_74A4
        add    esp, 0x10
        or     eax, eax
        je     loc_1_7EA8
        cmp    eax, 0xff
        je     loc_1_7EA8
        mov    ecx, dword ptr [esp + 0x80]
        mov    dword ptr [ecx], 0
        push   edi
        call   sub_1_0024
        add    esp, 4
        jmp    loc_1_7EC4
        db  08Bh, 0FFh
loc_1_7EA8:
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [edi + 0xc], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x40
        ret    
loc_1_7EC0:
        dec    ecx
        mov    dword ptr [ebp + 0x18], ecx
loc_1_7EC4:
        mov    dword ptr [esp + 0x14], 0x17
loc_1_7ECC:
        test   byte ptr [0x13364], 0x10      ; -> loc_1_13364
        je     loc_1_800A
        mov    eax, dword ptr [esp + 0x5c]
        add    eax, 0xa
        mov    dword ptr [esp + 0x28], eax
        mov    edx, dword ptr [eax]
        mov    ecx, edx
        and    ecx, 0xff00
        mov    eax, edx
        shl    eax, 0x10
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edx
        and    eax, 0xff0000
        shr    eax, 8
        or     ecx, eax
        shr    edx, 0x18
        and    edx, 0xff
        or     ecx, edx
        test   byte ptr [esi + 1], 0xfc
        jne    loc_1_800A
        cmp    byte ptr [esp + 0x40], 0x2a
        je     loc_1_800A
        push   ecx
        call   sub_1_A064
        add    esp, 4
        or     al, al
        jne    loc_1_800A
        lea    eax, [esp + 0x12]
        push   eax
        push   dword ptr [esp + 0x28]
        lea    eax, [esp + 0x48]
        push   eax
        push   1
        call   sub_1_DFA0
        add    esp, 0x10
        mov    edi, eax
        or     edi, edi
        je     loc_1_7FC4
        test   byte ptr [edi + 0x20], 8
        je     loc_1_800A
        mov    eax, dword ptr [esp + 0x28]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [esi + 4], eax
        jne    loc_1_800A
        mov    eax, dword ptr [esp + 0x6c]
        cmp    eax, dword ptr [esp + 0x2c]
        je     loc_1_7FAC
        push   0x16
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        je     loc_1_7FA0
        mov    ecx, dword ptr [esp + 0x80]
        mov    dword ptr [ecx], eax
        mov    byte ptr [eax + 0x15], 1
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [esp + 0x54]
        mov    dword ptr [eax + 8], ecx
        inc    dword ptr [edi + 0x1c]
        jmp    loc_1_8002
        db  090h
loc_1_7FA0:
        mov    dword ptr [esp + 0x14], 0xc0000001
        jmp    loc_1_800A
        db  08Bh, 0FFh
loc_1_7FAC:
        inc    dword ptr [edi + 0x1c]
        push   dword ptr [esp + 0x54]
        push   edi
        push   dword ptr [esp + 0x74]
        push   esi
        call   sub_1_8018
        add    esp, 0x10
        jmp    loc_1_8002
        db  090h
loc_1_7FC4:
        lea    eax, [esp + 0x3c]
        push   eax
        push   1
        push   0
        lea    eax, [esp + 0x4c]
        push   eax
        call   sub_1_A0EC
        add    esp, 0x10
        or     eax, eax
        je     loc_1_8002
        cmp    eax, 0xff
        je     loc_1_8002
        push   dword ptr [esp + 0x24]
        lea    eax, [esp + 0x44]
        push   eax
        push   dword ptr [esp + 0x28]
        push   0
        push   dword ptr [esp + 0x64]
        push   0
        call   sub_1_106A4
        add    esp, 0x18
loc_1_8002:
        mov    dword ptr [esp + 0x14], 0x17
loc_1_800A:
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x40
        ret    
        db  08Bh, 0FFh
sub_1_8018:
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_FE74
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_8040
        push   dword ptr [esp + 0x1c]
        call   sub_1_DEA4
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_8040:
        mov    ebp, dword ptr [esp + 0x18]
        push   ebp
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    loc_1_8070
        push   dword ptr [esp + 0x1c]
        call   sub_1_DEA4
        add    esp, 4
        push   ebx
        call   sub_1_0024
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_8070:
        mov    edi, eax
        mov    esi, dword ptr [esp + 0x14]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [ebx + 0x1c], eax
        mov    dword ptr [ebx + 0x18], ebp
        mov    dword ptr [ebx + 0x24], 0
        mov    dword ptr [ebx + 0x20], 0
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [ebx + 0x38], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [ebx + 0x2c], eax
        mov    dword ptr [ebx + 0x14], 0
        mov    dword ptr [ebx + 0x44], 0
        mov    dword ptr [ebx + 0x10], 0
        mov    dword ptr [ebx + 0x14], 0
        push   dword ptr [esp + 0x1c]
        push   ebx
        call   sub_1_CC18
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_80D8:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x1c]
        cmp    edx, 4
        jae    loc_1_80F0
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_80F0:
        mov    ecx, dword ptr [esp + 0x18]
        mov    di, word ptr [ecx + 2]
        mov    bx, word ptr [0x13364]        ; -> loc_1_13364
        mov    si, bx
        and    si, 2
        je     loc_1_8120
        test   bl, 0x10
        jne    loc_1_8120
        test   di, 0x1000
        je     loc_1_8120
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_8120:
        mov    ax, di
        and    eax, 0x78
        je     loc_1_8144
        cmp    eax, 0x28
        je     loc_1_8160
        cmp    eax, 0x30
        je     loc_1_81A4
        cmp    eax, 0x38
        je     loc_1_81C0
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_8144:
        push   edi
        push   edx
        push   ecx
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x20]
        call   sub_1_81E4
        add    esp, 0x14
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_8160:
        test   di, 0x80
        je     loc_1_8184
        push   edi
        push   edx
        push   ecx
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x20]
        call   sub_1_880C
        add    esp, 0x14
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_8184:
        or     si, si
        jne    loc_1_81D7
        push   edx
        push   ecx
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x1c]
        call   sub_1_8A74
        add    esp, 0x10
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_81A4:
        push   edx
        push   ecx
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x1c]
        call   sub_1_8D7C
        add    esp, 0x10
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_81C0:
        test   bl, 1
        jne    loc_1_81D7
        push   edx
        push   ecx
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x1c]
        call   sub_1_9154
        add    esp, 0x10
loc_1_81D7:
        pop    edi
        mov    eax, 0x17
        pop    esi
        pop    ebx
        ret    
        db  0C3h, 08Dh, 049h, 000h
sub_1_81E4:
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x48]
        mov    ecx, dword ptr [eax + 0xa]
        mov    esi, ecx
        and    esi, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     esi, eax
        shl    esi, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     esi, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     esi, ecx
        mov    eax, dword ptr [esp + 0x48]
        lea    ebp, [eax + 8]
        mov    ax, word ptr [ebp]
        mov    bh, al
        mov    bl, ah
        cmp    bx, 0x35
        jne    loc_1_825C
        push   dword ptr [esp + 0x54]
        push   dword ptr [esp + 0x54]
        push   dword ptr [esp + 0x54]
        push   dword ptr [esp + 0x54]
        push   dword ptr [esp + 0x54]
        call   sub_1_9DA0
        add    esp, 0x14
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Dh, 049h, 000h
loc_1_825C:
        movzx  eax, word ptr [0x132d4]       ; -> loc_1_132D4
        add    eax, 0x31
        cmp    eax, dword ptr [esp + 0x50]
        jle    loc_1_827C
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Dh, 049h, 000h
loc_1_827C:
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x38]
        push   eax
        push   dword ptr [esp + 0x5c]
        mov    edi, dword ptr [esp + 0x5c]
        lea    eax, [edi + 0xc]
        push   eax
        call   sub_1_FFAC
        add    esp, 0x14
        or     eax, eax
        je     loc_1_82B8
        cmp    eax, 0xff
        je     loc_1_82B8
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  090h
loc_1_82B8:
        test   byte ptr [esp + 0x54], 0x80
        je     loc_1_85F8
        movzx  eax, word ptr [0x132d4]       ; -> loc_1_132D4
        add    eax, 0x37
        cmp    eax, dword ptr [esp + 0x50]
        jle    loc_1_82E0
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
loc_1_82E0:
        test   byte ptr [esp + 0x54], 4
        jne    loc_1_82F4
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
loc_1_82F4:
        mov    eax, dword ptr [esp + 0x18]
        cmp    byte ptr [eax + edi + 0xe], 0x21
        jne    loc_1_8324
        push   esi
        push   dword ptr [esp + 0x1c]
        lea    eax, [esp + 0x38]
        push   eax
        push   dword ptr [esp + 0x5c]
        push   edi
        call   sub_1_93A4
        add    esp, 0x14
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  090h
loc_1_8324:
        lea    eax, [esp + 0x14]
        push   eax
        push   0
        push   edi
        lea    eax, [esp + 0x3c]
        push   eax
        call   sub_1_A0EC
        add    esp, 0x10
        or     eax, eax
        je     loc_1_83FC
        cmp    eax, 0xff
        je     loc_1_83FC
        cmp    byte ptr [0x13350], 0         ; -> loc_1_13350
        jne    loc_1_83EC
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x20]
        lea    eax, [esp + 0x38]
        push   eax
        push   dword ptr [0x132b8]           ; -> loc_1_132B8
        call   sub_1_6954
        add    esp, 0x10
        or     eax, eax
        je     loc_1_8380
        cmp    eax, 0xff
        jne    loc_1_83EC
loc_1_8380:
        mov    eax, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax + 0x20]
        test   ah, 8
        jne    loc_1_83EC
        mov    ecx, dword ptr [esp + 0x14]
        cmp    dword ptr [ecx + 0xc], esi
        je     loc_1_83EC
        test   al, 2
        je     loc_1_83EC
        test   al, 0x10
        je     loc_1_83EC
        mov    ebx, dword ptr [esp + 0x44]
        cmp    dword ptr [ebx + 0x74], esi
        je     loc_1_83EC
        cmp    dword ptr [ebx + 0x78], esi
        je     loc_1_83EC
        push   dword ptr [esp + 0x20]
        push   5
        push   7
        push   ebx
        push   ebp
        push   dword ptr [esp + 0x28]
        push   edi
        push   dword ptr [esp + 0x34]
        call   sub_1_110C8
        add    esp, 0x20
        mov    eax, dword ptr [esp + 0x14]
        and    dword ptr [eax + 0x20], 0xffffff0f
        mov    eax, dword ptr [esp + 0x14]
        or     dword ptr [eax + 0x20], 0x20
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [eax + 0x1c], 1
        jne    loc_1_83EC
        push   eax
        call   sub_1_DEA4
        add    esp, 4
loc_1_83EC:
        pop    ebp
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Dh, 049h, 000h
loc_1_83FC:
        mov    eax, dword ptr [esp + 0x18]
        lea    ebp, [eax + edi + 0xd]
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [eax + 0x14]
        or     ecx, ecx
        je     loc_1_83EC
        mov    edx, dword ptr [ecx + 0x20]
        cmp    byte ptr [eax + 0x32], 0
        jne    loc_1_844C
        test   byte ptr [esp + 0x55], 0xf
        je     loc_1_844C
        mov    ax, word ptr [edx + 0x4c]
        test   byte ptr [0x13364], 0xe       ; -> loc_1_13364
        je     loc_1_843E
        test   al, 6
        je     loc_1_843E
        mov    word ptr [ecx + 0x3e], 1
        or     ax, 0x200
        mov    word ptr [edx + 0x4c], ax
loc_1_843E:
        pop    ebp
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  090h
loc_1_844C:
        mov    eax, dword ptr [edx + 0x2c]
        mov    dword ptr [esp + 0x2c], eax
        lea    eax, [esp + 0x28]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0xc
        cmp    dword ptr [esp + 0x24], 0
        je     loc_1_83EC
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x14], 0
        test   byte ptr [esp + 0x55], 0xf
        jne    loc_1_8544
        push   ebx
        push   esi
        call   sub_1_9D64
        add    esp, 8
        mov    byte ptr [esp + 0x13], al
        or     al, al
        jne    loc_1_84CA
        lea    eax, [esp + 0x30]
        push   eax
        call   sub_1_FA64
        add    esp, 4
loc_1_84CA:
        push   dword ptr [esp + 0x13]
        push   0
        call   sub_1_FA40
        add    esp, 8
        test   byte ptr [0x13364], 0x10      ; -> loc_1_13364
        je     loc_1_84F7
        mov    eax, dword ptr [esp + 0x18]
        mov    al, byte ptr [eax + edi + 0x17]
        and    al, 0x60
        cmp    al, 0x20
        sete   al
        mov    ecx, dword ptr [esp + 0x14]
        mov    byte ptr [ecx + 0x33], al
loc_1_84F7:
        push   ebx
        mov    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x30]
        mov    eax, dword ptr [eax + 0x88]
        or     dword ptr [ecx + 0x28], eax
        push   esi
        call   sub_1_9D64
        add    esp, 8
        push   eax
        mov    ebx, dword ptr [esp + 0x48]
        push   ebx
        mov    eax, 0xfffffff6
        sub    eax, ebp
        add    eax, dword ptr [esp + 0x58]
        add    eax, edi
        push   eax
        push   dword ptr [esp + 0x20]
        add    ebp, 0xa
        push   ebp
        call   sub_1_9940
        add    esp, 0x14
        mov    dword ptr [esp + 0x14], 0
        sub    esi, esi
        jmp    loc_1_8596
        db  08Bh, 0FFh
loc_1_8544:
        mov    eax, dword ptr [esp + 0x14]
        and    dword ptr [eax + 0x20], 0xffffff0f
        mov    eax, dword ptr [esp + 0x14]
        or     dword ptr [eax + 0x20], 0x20
        test   byte ptr [0x13364], 0x10      ; -> loc_1_13364
        jne    loc_1_8564
        jmp    loc_1_8581
        db  08Bh, 0FFh
loc_1_8564:
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   dword ptr [esp + 0x18]
        call   sub_1_6608
        add    esp, 8
        or     eax, eax
        je     loc_1_858D
        cmp    eax, 0xff
        je     loc_1_858D
loc_1_8581:
        push   dword ptr [esp + 0x14]
        call   sub_1_DEA4
        add    esp, 4
loc_1_858D:
        mov    esi, 0xc00000be
        mov    ebx, dword ptr [esp + 0x44]
loc_1_8596:
        mov    ecx, dword ptr [ebx + 0x78]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     edx, ecx
        mov    eax, dword ptr [esp + 0x48]
        cmp    edx, dword ptr [eax + 0xa]
        jne    loc_1_85D4
        push   ebx
        call   sub_1_A08C
        add    esp, 4
loc_1_85D4:
        push   ebx
        push   0x3200                        ; -> loc_1_3200
        push   dword ptr [esp + 0x2c]
        push   esi
        push   dword ptr [esp + 0x38]
        call   sub_1_3C00
        add    esp, 0x14
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
loc_1_85F8:
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x20]
        lea    eax, [esp + 0x38]
        push   eax
        push   dword ptr [0x132b4]           ; -> loc_1_132B4
        call   sub_1_6954
        add    esp, 0x10
        or     eax, eax
        je     loc_1_861F
        cmp    eax, 0xff
        jne    loc_1_862E
loc_1_861F:
        mov    eax, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax + 0x20]
        test   al, 0x90
        jne    loc_1_87A4
loc_1_862E:
        test   byte ptr [esp + 0x55], 0x10
        jne    loc_1_866C
        mov    eax, dword ptr [esp + 0x18]
        cmp    byte ptr [eax + edi + 0xe], 0x20
        jne    loc_1_865C
        push   dword ptr [esp + 0x20]
        push   2
        push   0
        push   dword ptr [esp + 0x50]
        push   ebp
        push   0
        push   edi
        push   dword ptr [esp + 0x34]
        call   sub_1_110C8
        add    esp, 0x20
loc_1_865C:
        pop    ebp
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Dh, 049h, 000h
loc_1_866C:
        push   esi
        call   sub_1_A064
        add    esp, 4
        or     al, al
        je     loc_1_8688
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Bh, 0FFh
loc_1_8688:
        mov    eax, dword ptr [esp + 0x18]
        lea    eax, [eax + edi + 0xd]
        test   byte ptr [0x13364], 0x10      ; -> loc_1_13364
        je     loc_1_8794
        cmp    byte ptr [eax + 1], 0x20
        jne    loc_1_8794
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x20]
        lea    eax, [esp + 0x38]
        push   eax
        push   dword ptr [0x132b8]           ; -> loc_1_132B8
        call   sub_1_6954
        add    esp, 0x10
        or     eax, eax
        je     loc_1_8728
        cmp    eax, 0xff
        je     loc_1_8728
        lea    eax, [esp + 0x14]
        push   eax
        push   1
        push   edi
        lea    eax, [esp + 0x3c]
        push   eax
        call   sub_1_A0EC
        add    esp, 0x10
        or     eax, eax
        je     loc_1_8718
        cmp    eax, 0xff
        je     loc_1_8718
        push   dword ptr [esp + 0x1c]
        lea    eax, [esp + 0x34]
        push   eax
        push   dword ptr [esp + 0x20]
        push   edi
        push   dword ptr [esp + 0x54]
        push   0
        call   sub_1_106A4
        add    esp, 0x18
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  090h
loc_1_8718:
        pop    ebp
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Dh, 049h, 000h
loc_1_8728:
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [eax + 0x20]
        test   cl, 0x10
        je     loc_1_8784
        mov    ebx, dword ptr [esp + 0x44]
        mov    eax, dword ptr [ebx + 0x5c]
        mov    edx, dword ptr [esp + 0x14]
        mov    edx, dword ptr [edx + 0xc]
        and    edx, eax
        and    esi, eax
        cmp    edx, esi
        jne    loc_1_8759
        mov    eax, dword ptr [esp + 0x14]
        cmp    byte ptr [eax + 0x33], 0
        jne    loc_1_8759
        test   cl, 2
        jne    loc_1_8784
loc_1_8759:
        push   dword ptr [esp + 0x20]
        push   2
        push   0
        push   ebx
        push   ebp
        push   dword ptr [esp + 0x28]
        push   edi
        push   dword ptr [esp + 0x34]
        call   sub_1_110C8
        add    esp, 0x20
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Dh, 049h, 000h
loc_1_8784:
        pop    ebp
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Dh, 049h, 000h
loc_1_8794:
        pop    ebp
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Dh, 049h, 000h
loc_1_87A4:
        mov    eax, dword ptr [esp + 0x18]
        cmp    byte ptr [eax + edi + 0xe], 0x21
        jne    loc_1_87CC
        push   dword ptr [esp + 0x44]
        push   esi
        push   dword ptr [esp + 0x20]
        lea    eax, [esp + 0x3c]
        push   eax
        push   dword ptr [esp + 0x60]
        push   edi
        call   sub_1_96B8
        add    esp, 0x18
        jmp    loc_1_87FE
loc_1_87CC:
        push   esi
        call   sub_1_A064
        add    esp, 4
        or     al, al
        je     loc_1_87E0
        test   byte ptr [esp + 0x55], 0x10
        jne    loc_1_87FE
loc_1_87E0:
        push   dword ptr [esp + 0x20]
        push   2
        push   0
        push   dword ptr [esp + 0x50]
        push   ebp
        push   dword ptr [esp + 0x28]
        push   edi
        push   dword ptr [esp + 0x34]
        call   sub_1_110C8
        add    esp, 0x20
loc_1_87FE:
        pop    ebp
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  090h
sub_1_880C:
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x40]
        mov    edx, dword ptr [ecx + 0xa]
        mov    esi, edx
        and    esi, 0xff00
        mov    eax, edx
        shl    eax, 0x10
        or     esi, eax
        shl    esi, 8
        mov    eax, edx
        and    eax, 0xff0000
        shr    eax, 8
        or     esi, eax
        shr    edx, 0x18
        and    edx, 0xff
        or     esi, edx
        mov    ax, word ptr [ecx + 8]
        mov    bh, al
        mov    bl, ah
        movzx  eax, word ptr [0x132d4]       ; -> loc_1_132D4
        add    eax, 0x3d
        mov    edi, dword ptr [esp + 0x48]
        cmp    eax, edi
        jle    loc_1_886C
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  08Bh, 0FFh
loc_1_886C:
        push   esi
        call   sub_1_A064
        add    esp, 4
        or     al, al
        je     loc_1_8888
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  08Bh, 0FFh
loc_1_8888:
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        lea    eax, [esp + 0x30]
        push   eax
        push   edi
        mov    eax, dword ptr [esp + 0x54]
        add    eax, 0xc
        push   eax
        call   sub_1_FFAC
        add    esp, 0x14
        or     eax, eax
        je     loc_1_88C0
        cmp    eax, 0xff
        je     loc_1_88C0
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
loc_1_88C0:
        lea    eax, [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x28]
        lea    eax, [esp + 0x30]
        push   eax
        push   dword ptr [0x132b4]           ; -> loc_1_132B4
        call   sub_1_6954
        add    esp, 0x10
        or     eax, eax
        je     loc_1_88EB
        cmp    eax, 0xff
        jne    loc_1_8A64
loc_1_88EB:
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x28]
        mov    ecx, dword ptr [esp + 0x3c]
        test   dword ptr [ecx + 0x88], eax
        je     loc_1_8A64
        mov    edx, dword ptr [esp + 0x10]
        add    edx, 0x20
        mov    ecx, dword ptr [edx]
        mov    eax, ecx
        and    eax, 0xf0
        cmp    eax, 0x10
        je     loc_1_8922
        cmp    eax, 0x80
        jne    loc_1_8A64
loc_1_8922:
        mov    ax, word ptr [esp + 0x4c]
        mov    bp, ax
        and    bp, 0xf00
        jne    loc_1_8954
        test   ah, 0x80
        je     loc_1_898C
        test   cl, 0x80
        je     loc_1_898C
        push   ebx
        push   esi
        call   sub_1_9D64
        add    esp, 8
        push   eax
        push   1
        call   sub_1_FA40
        add    esp, 8
        jmp    loc_1_898C
        db  08Bh, 0FFh
loc_1_8954:
        cmp    bp, 0x600
        jb     loc_1_898C
        cmp    byte ptr [0x13350], 0         ; -> loc_1_13350
        je     loc_1_896F
        cmp    bp, 0x700
        je     loc_1_8A64
loc_1_896F:
        mov    edi, 0xc00000bd
        test   cl, 0x10
        je     loc_1_898E
        and    ecx, 0xffffff0f
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [esp + 0x10]
        or     dword ptr [eax + 0x20], 0x40
        jmp    loc_1_898E
        db  090h
loc_1_898C:
        sub    edi, edi
loc_1_898E:
        mov    ebx, dword ptr [esp + 0x10]
        add    ebx, 0x14
        mov    ecx, dword ptr [ebx]
        or     ecx, ecx
        je     loc_1_8A64
        mov    eax, dword ptr [ecx + 0x20]
        mov    edx, dword ptr [eax + 0x2c]
        mov    dword ptr [esp + 0x20], edx
        mov    dx, word ptr [eax + 0x40]
        mov    eax, dword ptr [esp + 0x44]
        cmp    word ptr [eax], dx
        je     loc_1_89C4
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  090h
loc_1_89C4:
        mov    dword ptr [ebx], 0
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0xc
        mov    ecx, dword ptr [esp + 0x3c]
        cmp    dword ptr [ecx + 0x74], esi
        jne    loc_1_89FC
        test   byte ptr [0x13364], 1         ; -> loc_1_13364
        jne    loc_1_8A34
        or     edi, edi
        jne    loc_1_8A34
        or     bp, bp
        jne    loc_1_8A34
        jmp    loc_1_8A20
        db  08Bh, 0FFh
loc_1_89FC:
        mov    ecx, dword ptr [esp + 0x3c]
        cmp    dword ptr [ecx + 0x78], esi
        jne    loc_1_8A34
        push   ecx
        call   sub_1_A08C
        add    esp, 4
        test   byte ptr [0x13364], 1         ; -> loc_1_13364
        jne    loc_1_8A34
        or     edi, edi
        jne    loc_1_8A34
        or     bp, bp
        jne    loc_1_8A34
loc_1_8A20:
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x4c]
        call   sub_1_9234
        add    esp, 0xc
loc_1_8A34:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_8A64
        or     bp, bp
        jne    loc_1_8A51
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0x20]
        mov    ecx, dword ptr [edx + 0x88]
        or     dword ptr [eax + 0x2c], ecx
loc_1_8A51:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_8A64
        push   edi
        push   dword ptr [esp + 0x20]
        call   dword ptr [esp + 0x1c]
        add    esp, 8
loc_1_8A64:
        pop    ebp
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  08Dh, 049h, 000h
sub_1_8A74:
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x40]
        mov    ecx, dword ptr [esi + 0xa]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     edx, ecx
        movzx  eax, word ptr [0x132d4]       ; -> loc_1_132D4
        add    eax, 0x43
        mov    edi, dword ptr [esp + 0x48]
        cmp    eax, edi
        jle    loc_1_8ACC
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  08Bh, 0FFh
loc_1_8ACC:
        push   edx
        call   sub_1_A064
        add    esp, 4
        or     al, al
        je     loc_1_8AE8
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  08Bh, 0FFh
loc_1_8AE8:
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x30]
        push   eax
        push   edi
        mov    edi, dword ptr [esp + 0x54]
        lea    eax, [edi + 0xc]
        push   eax
        call   sub_1_FFAC
        add    esp, 0x14
        or     eax, eax
        je     loc_1_8B20
        cmp    eax, 0xff
        je     loc_1_8B20
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
loc_1_8B20:
        lea    eax, [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x1c]
        lea    eax, [esp + 0x30]
        push   eax
        push   dword ptr [0x132b4]           ; -> loc_1_132B4
        call   sub_1_6954
        add    esp, 0x10
        or     eax, eax
        je     loc_1_8C9C
        cmp    eax, 0xff
        je     loc_1_8C9C
        test   byte ptr [0x13364], 0x10      ; -> loc_1_13364
        je     loc_1_8D6C
        cmp    byte ptr [0x13355], 0         ; -> obj1:0x13355
        je     loc_1_8D6C
        sub    bl, bl
        lea    eax, [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x1c]
        lea    eax, [esp + 0x30]
        push   eax
        push   dword ptr [0x132b8]           ; -> loc_1_132B8
        call   sub_1_6954
        add    esp, 0x10
        or     eax, eax
        je     loc_1_8BBC
        cmp    eax, 0xff
        je     loc_1_8BBC
        push   dword ptr [esp + 0x18]
        lea    eax, [esp + 0x2c]
        push   eax
        push   dword ptr [esp + 0x1c]
        push   edi
        push   dword ptr [esp + 0x4c]
        push   1
        call   sub_1_106A4
        add    esp, 0x18
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  090h
loc_1_8BBC:
        mov    eax, dword ptr [esp + 0x10]
        test   byte ptr [eax + 0x20], 0x10
        je     loc_1_8D6C
        push   dword ptr [esp + 0x14]
        push   edi
        call   sub_1_A0AC
        add    esp, 8
        and    ax, 0x8000
        jne    loc_1_8BEC
        mov    ecx, dword ptr [esp + 0x10]
        test   byte ptr [ecx + 0x20], 2
        jne    loc_1_8BEC
        jmp    loc_1_8C61
        db  08Dh, 049h, 000h
loc_1_8BEC:
        mov    ecx, dword ptr [esp + 0x14]
        mov    edx, dword ptr [ecx + edi + 0x1f]
        mov    ecx, edx
        and    ecx, 0xff00
        mov    ebp, edx
        shl    ebp, 0x10
        or     ecx, ebp
        shl    ecx, 8
        mov    ebp, edx
        and    ebp, 0xff0000
        shr    ebp, 8
        or     ecx, ebp
        shr    edx, 0x18
        and    edx, 0xff
        or     ecx, edx
        or     ax, ax
        je     loc_1_8C32
        mov    edx, dword ptr [esp + 0x10]
        test   byte ptr [edx + 0x20], 2
        jne    loc_1_8C41
        or     ax, ax
        jne    loc_1_8C63
loc_1_8C32:
        mov    eax, dword ptr [esp + 0x10]
        test   byte ptr [eax + 0x20], 2
        je     loc_1_8C63
        cmp    dword ptr [eax + 0xc], ecx
        je     loc_1_8C63
loc_1_8C41:
        mov    edx, dword ptr [esp + 0x3c]
        mov    edx, dword ptr [edx + 0x5c]
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0xc]
        and    eax, edx
        and    ecx, edx
        cmp    eax, ecx
        jne    loc_1_8C61
        mov    eax, dword ptr [esp + 0x10]
        cmp    byte ptr [eax + 0x33], 0
        je     loc_1_8C63
loc_1_8C61:
        mov    bl, 1
loc_1_8C63:
        or     bl, bl
        je     loc_1_8D6C
        push   dword ptr [esp + 0x24]
        push   5
        push   6
        push   dword ptr [esp + 0x48]
        add    esi, 8
        push   esi
        push   dword ptr [esp + 0x24]
        push   edi
        push   dword ptr [esp + 0x30]
        call   sub_1_110C8
        add    esp, 0x20
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  08Dh, 049h, 000h
loc_1_8C9C:
        push   dword ptr [esp + 0x14]
        push   edi
        call   sub_1_A0AC
        add    esp, 8
        cmp    byte ptr [esp + 0x28], 0x2a
        jne    loc_1_8CC0
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  08Dh, 049h, 000h
loc_1_8CC0:
        mov    ecx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [ecx + 0x20]
        test   cl, 2
        jne    loc_1_8CDE
        test   cl, 4
        je     loc_1_8D6C
        test   ah, 0x80
        jne    loc_1_8D6C
loc_1_8CDE:
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x28]
        mov    edx, dword ptr [esp + 0x3c]
        test   dword ptr [edx + 0x88], eax
        je     loc_1_8D6C
        and    ecx, 0xf0
        cmp    ecx, 0x10
        je     loc_1_8D08
        cmp    ecx, 0x80
        je     loc_1_8D2C
        jmp    loc_1_8D6C
        db  08Bh, 0FFh
loc_1_8D08:
        add    esi, 8
        push   dword ptr [esp + 0x24]
        push   5
        push   6
        push   dword ptr [esp + 0x48]
        push   esi
        push   dword ptr [esp + 0x24]
        push   edi
        push   dword ptr [esp + 0x30]
        call   sub_1_110C8
        add    esp, 0x20
        jmp    loc_1_8D6C
        db  090h
loc_1_8D2C:
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [eax + 0x14]
        or     ecx, ecx
        je     loc_1_8D6C
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0xc
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x14], 0
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_8D6C
        push   0xc00000bd
        push   dword ptr [esp + 0x24]
        call   dword ptr [esp + 0x24]
        add    esp, 8
loc_1_8D6C:
        pop    ebp
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  08Dh, 049h, 000h
sub_1_8D7C:
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x48]
        mov    ecx, dword ptr [esi + 0xa]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     edx, ecx
        mov    ax, word ptr [esi + 8]
        mov    bh, al
        mov    bl, ah
        movzx  eax, word ptr [0x132d4]       ; -> loc_1_132D4
        add    eax, 0x3d
        cmp    eax, dword ptr [esp + 0x50]
        jle    loc_1_8DD8
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
loc_1_8DD8:
        push   edx
        call   sub_1_A064
        add    esp, 4
        or     al, al
        je     loc_1_8DF4
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Bh, 0FFh
loc_1_8DF4:
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x38]
        push   eax
        push   dword ptr [esp + 0x5c]
        mov    eax, dword ptr [esp + 0x5c]
        add    eax, 0xc
        push   eax
        call   sub_1_FFAC
        add    esp, 0x14
        or     eax, eax
        je     loc_1_8E30
        cmp    eax, 0xff
        je     loc_1_8E30
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  090h
loc_1_8E30:
        mov    ecx, dword ptr [esp + 0x4c]
        mov    bp, word ptr [ecx + 2]
        mov    ecx, dword ptr [esi + 0xa]
        mov    edi, ecx
        and    edi, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edi, eax
        shl    edi, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edi, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     edi, ecx
        movzx  eax, word ptr [0x132d4]       ; -> loc_1_132D4
        test   bp, 0x80
        je     loc_1_8F8C
        add    eax, 0x3d
        cmp    eax, dword ptr [esp + 0x50]
        jle    loc_1_8E8C
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
loc_1_8E8C:
        lea    eax, [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x18]
        lea    eax, [esp + 0x38]
        push   eax
        push   dword ptr [0x132b4]           ; -> loc_1_132B4
        call   sub_1_6954
        add    esp, 0x10
        or     eax, eax
        je     loc_1_8EC0
        cmp    eax, 0xff
        je     loc_1_8EC0
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
loc_1_8EC0:
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, dword ptr [eax + 0x14]
        or     esi, esi
        jne    loc_1_8ED8
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
loc_1_8ED8:
        mov    ebp, dword ptr [esi + 0x20]
        mov    ax, word ptr [ebp + 0x40]
        mov    ecx, dword ptr [esp + 0x4c]
        cmp    word ptr [ecx], ax
        je     loc_1_8EF8
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Dh, 049h, 000h
loc_1_8EF8:
        mov    ax, word ptr [0x13364]        ; -> loc_1_13364
        and    eax, 0xf
        cmp    eax, 2
        je     loc_1_8F20
        cmp    eax, 4
        je     loc_1_8F64
        cmp    eax, 8
        je     loc_1_8F64
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Dh, 049h, 000h
loc_1_8F20:
        or     esi, esi
        je     loc_1_8F56
        lea    eax, [esp + 0x2c]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        push   esi
        call   sub_1_10AEC
        add    esp, 0xc
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x14], 0
        cmp    dword ptr [esp + 0x20], 0
        je     loc_1_8F56
        push   0
        push   dword ptr [esp + 0x30]
        call   dword ptr [esp + 0x28]
        add    esp, 8
loc_1_8F56:
        pop    ebp
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  090h
loc_1_8F64:
        push   ebx
        push   edi
        call   sub_1_9D64
        add    esp, 8
        or     al, al
        je     loc_1_8F7E
        test   byte ptr [ebp + 0x4c], 2
        je     loc_1_8F7E
        mov    word ptr [esi + 0x3e], 1
loc_1_8F7E:
        pop    ebp
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  090h
loc_1_8F8C:
        add    eax, 0x43
        cmp    eax, dword ptr [esp + 0x50]
        jle    loc_1_8FA4
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Bh, 0FFh
loc_1_8FA4:
        lea    eax, [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x18]
        lea    eax, [esp + 0x38]
        push   eax
        push   dword ptr [0x132b8]           ; -> loc_1_132B8
        call   sub_1_6954
        add    esp, 0x10
        sub    dl, dl
        or     eax, eax
        je     loc_1_8FF6
        cmp    eax, 0xff
        je     loc_1_8FEB
        lea    eax, [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x18]
        lea    eax, [esp + 0x38]
        push   eax
        push   dword ptr [0x132b4]           ; -> loc_1_132B4
        call   sub_1_6954
        add    esp, 0x10
        mov    dl, 1
loc_1_8FEB:
        or     eax, eax
        je     loc_1_8FF6
        cmp    eax, 0xff
        jne    loc_1_903A
loc_1_8FF6:
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0x4c]
        mov    ecx, dword ptr [edi + ecx + 0x1f]
        mov    edi, ecx
        and    edi, 0xff00
        mov    ebx, ecx
        shl    ebx, 0x10
        or     edi, ebx
        shl    edi, 8
        mov    ebx, ecx
        and    ebx, 0xff0000
        shr    ebx, 8
        or     edi, ebx
        shr    ecx, 0x18
        and    ecx, 0xff
        or     edi, ecx
        mov    ecx, dword ptr [esp + 0x10]
        cmp    edi, dword ptr [ecx + 0xc]
        je     loc_1_903A
        mov    eax, 0xc0000001
loc_1_903A:
        or     eax, eax
        je     loc_1_9050
        cmp    eax, 0xff
        je     loc_1_9050
        mov    eax, 3
        jmp    loc_1_9124
        db  090h
loc_1_9050:
        mov    edi, dword ptr [esp + 0x10]
        add    edi, 0x20
        mov    ecx, dword ptr [edi]
        test   cl, 2
        je     loc_1_9122
        mov    eax, ecx
        and    eax, 0xf0
        cmp    eax, 0x10
        je     loc_1_907C
        cmp    eax, 0x80
        je     loc_1_90CC
        jmp    loc_1_9122
        db  08Bh, 0FFh
loc_1_907C:
        or     dl, dl
        jne    loc_1_90B8
        test   ch, 8
        jne    loc_1_9122
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [eax + 0x1c], 1
        jbe    loc_1_90A8
        and    ecx, 0xffffff0f
        mov    dword ptr [edi], ecx
        mov    eax, dword ptr [esp + 0x10]
        or     dword ptr [eax + 0x20], 0x40
        jmp    loc_1_9122
        db  08Dh, 049h, 000h
loc_1_90A8:
        push   dword ptr [esp + 0x10]
        call   sub_1_DEA4
        add    esp, 4
        jmp    loc_1_9122
        db  08Bh, 0FFh
loc_1_90B8:
        and    ecx, 0xffffff0f
        mov    dword ptr [edi], ecx
        mov    eax, dword ptr [esp + 0x10]
        or     dword ptr [eax + 0x20], 0x40
        jmp    loc_1_9122
        db  08Bh, 0FFh
loc_1_90CC:
        mov    eax, dword ptr [esp + 0x10]
        add    eax, 0x14
        mov    ecx, dword ptr [eax]
        mov    dword ptr [eax], 0
        or     dl, dl
        jne    loc_1_90F4
        push   dword ptr [esp + 0x24]
        push   0xc00000bd
        push   ecx
        call   sub_1_1088C
        add    esp, 0xc
        jmp    loc_1_9122
        db  090h
loc_1_90F4:
        or     ecx, ecx
        je     loc_1_9122
        lea    eax, [esp + 0x28]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0xc
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_9122
        push   0xc00000bd
        push   dword ptr [esp + 0x2c]
        call   dword ptr [esp + 0x24]
        add    esp, 8
loc_1_9122:
        sub    eax, eax
loc_1_9124:
        test   bp, 0x1000
        jne    loc_1_914C
        push   dword ptr [esp + 0x24]
        push   6
        push   eax
        push   dword ptr [esp + 0x50]
        lea    eax, [esi + 8]
        push   eax
        push   0
        push   dword ptr [esp + 0x64]
        push   dword ptr [esp + 0x34]
        call   sub_1_110C8
        add    esp, 0x20
loc_1_914C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
sub_1_9154:
        sub    esp, 0x1c
        push   esi
        movzx  eax, word ptr [0x132d4]       ; -> loc_1_132D4
        add    eax, 0x39
        mov    ecx, dword ptr [esp + 0x30]
        cmp    eax, ecx
        jle    loc_1_9174
        mov    eax, 0x17
        pop    esi
        add    esp, 0x1c
        ret    
loc_1_9174:
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   ecx
        mov    esi, dword ptr [esp + 0x3c]
        lea    eax, [esi + 0xc]
        push   eax
        call   sub_1_FFAC
        add    esp, 0x14
        or     eax, eax
        je     loc_1_91AC
        cmp    eax, 0xff
        je     loc_1_91AC
        mov    eax, 0x17
        pop    esi
        add    esp, 0x1c
        ret    
        db  08Dh, 049h, 000h
loc_1_91AC:
        lea    eax, [esp + 4]
        push   eax
        push   dword ptr [esp + 0x10]
        lea    eax, [esp + 0x18]
        push   eax
        push   dword ptr [0x132b4]           ; -> loc_1_132B4
        call   sub_1_6954
        add    esp, 0x10
        or     eax, eax
        je     loc_1_91D3
        cmp    eax, 0xff
        jne    loc_1_9228
loc_1_91D3:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + eax + 0x11]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     edx, ecx
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x14]
        or     ecx, ecx
        je     loc_1_9228
        lea    eax, [edx + edx*4]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        shl    eax, 3
        cmp    dword ptr [ecx + 0x38], eax
        jae    loc_1_9228
        mov    dword ptr [ecx + 0x38], eax
        or     word ptr [ecx + 0x3c], 0x10
loc_1_9228:
        pop    esi
        mov    eax, 0x17
        add    esp, 0x1c
        ret    
        db  08Bh, 0FFh
sub_1_9234:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x18]
        mov    edx, dword ptr [ecx + eax + 0x11]
        mov    ecx, edx
        and    ecx, 0xff00
        mov    eax, edx
        shl    eax, 0x10
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edx
        and    eax, 0xff0000
        shr    eax, 8
        or     ecx, eax
        shr    edx, 0x18
        and    edx, 0xff
        or     ecx, edx
        cmp    ecx, 0x418937
        mov    esi, 0xfffffffe
        jae    loc_1_9289
        lea    eax, [ecx + ecx*4]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    esi, [eax*8]
loc_1_9289:
        or     esi, esi
        jne    loc_1_9294
        mov    esi, 0xffffffff
        jmp    loc_1_92A1
loc_1_9294:
        cmp    esi, 0x493e0
        jae    loc_1_92A1
        mov    esi, 0x493e0
loc_1_92A1:
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x18], esi
        cmp    dword ptr [0x132e0], esi      ; -> loc_1_132E0
        jbe    loc_1_9324
        mov    dword ptr [0x132e0], esi      ; -> loc_1_132E0
        mov    eax, dword ptr [0x132f0]      ; -> loc_1_132F0
        or     eax, eax
        je     loc_1_92D5
        mov    ecx, eax
        mov    dword ptr [ecx + 0x28], 0
        push   0
        push   0
        push   eax
        call   sub_1_10AEC
        add    esp, 0xc
loc_1_92D5:
        sub    edx, edx
        mov    ecx, 0x927c0
        mov    eax, dword ptr [0x132e0]      ; -> loc_1_132E0
        div    ecx
        mov    dword ptr [0x132e4], eax      ; -> loc_1_132E4
        cmp    eax, 8
        jae    loc_1_92F7
        mov    dword ptr [0x132e4], 8        ; -> loc_1_132E4
loc_1_92F7:
        push   0x132f0                       ; -> loc_1_132F0
        push   0
        push   0
        push   0
        push   0xf5c0                        ; -> loc_1_F5C0
        push   0
        push   0
        mov    eax, esi
        sub    edx, edx
        div    dword ptr [0x132e4]           ; -> loc_1_132E4
        push   eax
        call   sub_1_109B8
        add    esp, 0x20
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_9324:
        jae    loc_1_9389
        sub    ebx, ebx
        mov    eax, dword ptr [0x132b4]      ; -> loc_1_132B4
        mov    edx, dword ptr [eax]
        cmp    edx, ebx
        jle    loc_1_9364
        mov    edi, eax
        add    edi, 8
loc_1_9338:
        mov    eax, dword ptr [edi]
        cmp    edi, eax
        je     loc_1_935C
loc_1_933E:
        cmp    byte ptr [eax + 0x34], 0x2a
        je     loc_1_9356
        mov    ecx, dword ptr [eax + 0x20]
        test   cl, 0x10
        je     loc_1_9356
        cmp    dword ptr [eax + 0x18], esi
        jae    loc_1_9356
        test   cl, 1
        je     loc_1_9390
loc_1_9356:
        mov    eax, dword ptr [eax]
        cmp    edi, eax
        jne    loc_1_933E
loc_1_935C:
        inc    ebx
        add    edi, 8
        cmp    ebx, edx
        jl     loc_1_9338
loc_1_9364:
        mov    eax, esi
        mov    dword ptr [0x132e0], esi      ; -> loc_1_132E0
        mov    ecx, 0x927c0
        sub    edx, edx
        div    ecx
        mov    dword ptr [0x132e4], eax      ; -> loc_1_132E4
        cmp    eax, 8
        jae    loc_1_9389
        mov    dword ptr [0x132e4], 8        ; -> loc_1_132E4
loc_1_9389:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_9390:
        mov    eax, dword ptr [eax + 0x18]
        cmp    eax, 0x493e0
        jb     loc_1_9389
        mov    dword ptr [0x132e0], eax      ; -> loc_1_132E0
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_93A4:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [0x1326c]      ; -> loc_1_1326C
        cmp    ebp, 0x1326c                  ; -> loc_1_1326C
        je     loc_1_9483
loc_1_93BD:
        mov    ebx, ebp
        push   0x10
        mov    eax, dword ptr [ebp + 0x38]
        add    eax, 0x34
        push   eax
        push   dword ptr [esp + 0x2c]
        call   sub_1_0262
        add    esp, 0xc
        cmp    eax, 0x10
        jne    loc_1_9474
        mov    ecx, dword ptr [ebp]
        mov    edx, ecx
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        mov    ebp, ecx
        mov    eax, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esp + 0x28]
        lea    esi, [eax + ecx + 0xd]
        mov    eax, ecx
        add    eax, 0xd
        sub    dword ptr [esp + 0x20], eax
        mov    ecx, dword ptr [ebx + 0x30]
        or     ecx, ecx
        je     loc_1_9477
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0xc
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_9477
        push   dword ptr [esp + 0x20]
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        je     loc_1_9477
        mov    edx, dword ptr [esp + 0x20]
        mov    edi, eax
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [esp + 0x2c]
        mov    dword ptr [ebx + 0x44], ecx
        mov    dword ptr [ebx + 0x30], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [ebx + 0x40], eax
        push   0
        push   0x9490                        ; -> loc_1_9490
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x20]
        push   ebx
        call   sub_1_3C00
        add    esp, 0x14
        jmp    loc_1_9477
        db  08Dh, 049h, 000h
loc_1_9474:
        mov    ebp, dword ptr [ebp]
loc_1_9477:
        cmp    ebp, 0x1326c                  ; -> loc_1_1326C
        jne    loc_1_93BD
loc_1_9483:
        pop    ebp
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_9490:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x34]
        mov    ecx, dword ptr [eax + 8]
        mov    eax, dword ptr [ecx + 0x40]
        mov    dword ptr [esp + 0x24], eax
        mov    dword ptr [ecx + 0x40], 0
        mov    edx, dword ptr [ecx + 0x30]
        mov    dword ptr [esp + 0x28], edx
        mov    eax, dword ptr [ecx + 0x10]
        mov    dword ptr [esp + 0x2c], eax
        movzx  eax, byte ptr [edx + 0xa]
        mov    dword ptr [esp + 0x20], eax
        add    eax, eax
        lea    ebx, [eax + eax*8]
        lea    ecx, [ebx + 0x3c]
        mov    dword ptr [esp + 0x1c], ecx
        cmp    ecx, 0xffff
        jbe    loc_1_94E0
        mov    ebx, 0xc0000001
        jmp    loc_1_9687
loc_1_94E0:
        movzx  eax, word ptr [esp + 0x1c]
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_9500
        mov    ebx, 1
        jmp    loc_1_9687
        db  08Bh, 0FFh
loc_1_9500:
        mov    edi, ebp
        mov    edx, dword ptr [esp + 0x1c]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        lea    eax, [ebx + 0x2e]
        cmp    eax, dword ptr [esp + 0x24]
        ja     loc_1_952C
        mov    edx, dword ptr [esp + 0x28]
        lea    esi, [ebx + edx + 0xb]
        mov    edi, ebp
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
loc_1_952C:
        add    ebx, 0x39
        mov    byte ptr [ebp + 6], 3
        mov    byte ptr [ebp + 8], 0xfe
        cmp    ebx, dword ptr [esp + 0x24]
        ja     loc_1_955C
        mov    edx, dword ptr [esp + 0x28]
        mov    cl, byte ptr [edx + ebx - 2]
        movzx  ax, cl
        shl    ax, 8
        shr    cl, 8
        and    cx, 0xff
        or     ax, cx
        mov    word ptr [ebp + 0x36], ax
loc_1_955C:
        lea    eax, [ebp + 0x3c]
        mov    dword ptr [esp + 0x18], 0xb
        mov    byte ptr [esp + 0x13], 0
        mov    dword ptr [esp + 0x14], 0
        cmp    dword ptr [esp + 0x20], 0
        je     loc_1_9605
        mov    ecx, dword ptr [esp + 0x28]
        add    ecx, 0xb
loc_1_9586:
        add    dword ptr [esp + 0x18], 0x12
        mov    edx, dword ptr [esp + 0x18]
        cmp    edx, dword ptr [esp + 0x24]
        ja     loc_1_9600
        inc    word ptr [ebp + 0x3a]
        mov    dl, byte ptr [ecx + 0x10]
        mov    bl, dl
        and    bl, 0x80
        cmp    bl, 1
        cmc    
        sbb    bl, bl
        and    bl, 0x80
        mov    byte ptr [eax + 0x11], bl
        test   dl, 8
        je     loc_1_95C4
        test   dl, 0x10
        je     loc_1_95BC
        or     bl, 7
        jmp    loc_1_95D3
loc_1_95BC:
        or     bl, 6
        jmp    loc_1_95D3
        db  08Dh, 049h, 000h
loc_1_95C4:
        test   dl, 0x10
        je     loc_1_95D0
        or     bl, 5
        jmp    loc_1_95D3
        db  08Bh, 0FFh
loc_1_95D0:
        or     bl, 4
loc_1_95D3:
        mov    edi, eax
        mov    byte ptr [eax + 0x11], bl
        mov    edx, dword ptr [esp + 0x14]
        inc    dl
        mov    byte ptr [eax + 0x10], dl
        mov    esi, ecx
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        add    eax, 0x12
        add    ecx, 0x12
        inc    dword ptr [esp + 0x14]
        mov    edx, dword ptr [esp + 0x14]
        cmp    edx, dword ptr [esp + 0x20]
        jb     loc_1_9586
        jmp    loc_1_9605
        db  08Dh, 049h, 000h
loc_1_9600:
        mov    byte ptr [esp + 0x13], 1
loc_1_9605:
        cmp    byte ptr [esp + 0x13], 0
        je     loc_1_9614
        mov    ebx, 0xc0000001
        jmp    loc_1_967E
        db  090h
loc_1_9614:
        mov    eax, dword ptr [esp + 0x2c]
        movzx  esi, word ptr [eax + 8]
        cmp    dword ptr [esp + 0x1c], esi
        jbe    loc_1_9654
        cmp    esi, 0x3c
        jae    loc_1_9630
        mov    word ptr [ebp + 0x3a], 0
        jmp    loc_1_9641
        db  090h
loc_1_9630:
        lea    eax, [esi - 0x3c]
        mov    cx, 0x12
        sub    dx, dx
        div    cx
        mov    word ptr [ebp + 0x3a], ax
loc_1_9641:
        cmp    dword ptr [esp + 0x1c], esi
        jbe    loc_1_9654
        mov    ebx, 9
        mov    dword ptr [esp + 0x1c], esi
        jmp    loc_1_9656
        db  08Bh, 0FFh
loc_1_9654:
        sub    ebx, ebx
loc_1_9656:
        mov    esi, ebp
        mov    edx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esp + 0x2c]
        mov    edi, dword ptr [eax + 4]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x2c]
        mov    cx, word ptr [esp + 0x1c]
        mov    word ptr [eax + 8], cx
loc_1_967E:
        push   ebp
        call   sub_1_0024
        add    esp, 4
loc_1_9687:
        mov    eax, dword ptr [esp + 0x34]
        mov    edi, dword ptr [eax + 0xc]
        mov    esi, dword ptr [eax + 0x10]
        push   dword ptr [esp + 0x28]
        call   sub_1_0024
        add    esp, 4
        push   dword ptr [esp + 0x34]
        call   sub_1_0024
        add    esp, 4
        push   ebx
        push   edi
        call   esi
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
sub_1_96B8:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x34]
        add    eax, 0x11
        mov    ebp, dword ptr [esp + 0x2c]
        cmp    eax, ebp
        jae    loc_1_96DC
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  090h
loc_1_96DC:
        mov    edx, dword ptr [0x132d8]      ; -> loc_1_132D8
        mov    esi, dword ptr [esp + 0x28]
        lea    edi, [esi + 0x2d]
        lea    eax, [ebp - 0x31]
        movzx  ecx, word ptr [0x132d4]       ; -> loc_1_132D4
        cmp    ecx, eax
        je     loc_1_9704
        mov    eax, 0x17
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_9704:
        sub    ebx, ebx
        cmp    word ptr [0x132d4], bx        ; -> loc_1_132D4
        jbe    loc_1_971D
        nop    
loc_1_9710:
        mov    al, byte ptr [edx]
        cmp    byte ptr [edi], al
        jne    loc_1_9758
        inc    ebx
        inc    edi
        inc    edx
        cmp    ebx, ecx
        jb     loc_1_9710
loc_1_971D:
        push   0x13234                       ; -> loc_1_13234
        call   sub_1_10288
        add    esp, 4
        add    eax, eax
        lea    eax, [eax + eax*8]
        lea    ecx, [eax + ebp + 0x35]
        mov    dword ptr [esp + 0x20], ecx
        movzx  eax, cx
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_9768
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Bh, 0FFh
loc_1_9758:
        pop    ebp
        mov    eax, 0x17
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
loc_1_9768:
        mov    edi, ebx
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [ebx + 2], 0x84
        mov    byte ptr [ebx + 5], 0
        mov    byte ptr [ebx + 7], 1
        mov    eax, dword ptr [esp + 0x34]
        lea    ecx, [eax + ebx + 0xd]
        mov    dword ptr [esp + 0x18], ecx
        mov    dword ptr [ecx + 4], 0
        mov    edx, ecx
        add    edx, 0xb
        mov    eax, dword ptr [esp + 0x3c]
        mov    ecx, dword ptr [eax + 0x88]
        mov    dword ptr [esp + 0x1c], ecx
        mov    dword ptr [esp + 0x10], 0
        mov    ebp, dword ptr [0x13274]      ; -> loc_1_13274
        cmp    ebp, 0x13274                  ; -> loc_1_13274
        je     loc_1_9873
loc_1_97C6:
        mov    eax, dword ptr [ebp + 0x14]
        cmp    word ptr [ebp + 0x20], 1
        cmc    
        sbb    cl, cl
        and    cl, 0x80
        mov    byte ptr [edx + 0x10], cl
        or     cl, 4
        mov    byte ptr [edx + 0x10], cl
        mov    esi, dword ptr [eax + 0x20]
        and    esi, 0xf0
        cmp    esi, 0x20
        je     loc_1_97FC
        cmp    esi, 0x40
        je     loc_1_9804
        cmp    esi, 0x80
        je     loc_1_9864
        jmp    loc_1_980A
        db  08Bh, 0FFh
loc_1_97FC:
        or     cl, 0x10
        jmp    loc_1_9807
        db  08Dh, 049h, 000h
loc_1_9804:
        or     cl, 8
loc_1_9807:
        mov    byte ptr [edx + 0x10], cl
loc_1_980A:
        mov    cx, word ptr [0x13364]        ; -> loc_1_13364
        and    ecx, 0xf
        cmp    ecx, 2
        je     loc_1_9828
        cmp    ecx, 4
        je     loc_1_9830
        cmp    ecx, 8
        je     loc_1_9830
        jmp    loc_1_9834
        db  08Dh, 049h, 000h
loc_1_9828:
        or     byte ptr [edx + 0x10], 0x20
        jmp    loc_1_9834
        db  08Bh, 0FFh
loc_1_9830:
        or     byte ptr [edx + 0x10], 0x40
loc_1_9834:
        mov    edi, edx
        mov    byte ptr [edx + 0x11], 0
        lea    esi, [eax + 0x34]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [eax + 0x20]
        test   cl, 1
        jne    loc_1_9864
        cmp    byte ptr [eax + 0x34], 0x2a
        je     loc_1_9864
        test   cl, 0x80
        jne    loc_1_9864
        mov    ecx, dword ptr [esp + 0x1c]
        test   dword ptr [eax + 0x28], ecx
        je     loc_1_9864
        inc    dword ptr [esp + 0x10]
        add    edx, 0x12
loc_1_9864:
        mov    ebp, dword ptr [ebp]
        cmp    ebp, 0x13274                  ; -> loc_1_13274
        jne    loc_1_97C6
loc_1_9873:
        mov    al, byte ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x18]
        mov    byte ptr [ecx + 0xa], al
        mov    eax, dword ptr [esp + 0x10]
        add    eax, eax
        lea    eax, [eax + eax*8]
        lea    ecx, [eax + 0x2f]
        mov    dl, ch
        mov    dh, cl
        mov    ecx, dword ptr [esp + 0x18]
        mov    word ptr [ecx + 8], dx
        lea    edx, [eax + ecx + 0xb]
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0xb
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [esp + 0x3c]
        add    esi, 0x8c
        mov    edi, edx
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        lea    eax, [esp + 0x14]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_98E4
        cmp    esi, 0xff
        je     loc_1_98E4
        push   ebx
        call   sub_1_0024
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_98E4:
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [eax + 0x18], ecx
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x1c], ebx
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x20], 0
        push   0x10
        push   0x89
        push   dword ptr [esp + 0x1c]
        push   0x115c4                       ; -> loc_1_115C4
        mov    eax, dword ptr [esp + 0x4c]
        push   dword ptr [eax + 0x68]
        push   dword ptr [esp + 0x4c]
        push   dword ptr [esp + 0x2c]
        call   sub_1_115D4
        add    esp, 0x1c
        mov    esi, eax
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  090h
sub_1_9940:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x24]
        mov    eax, dword ptr [esi + 0x20]
        test   al, 0x80
        je     loc_1_9AF7
        and    eax, 0xffffff0f
        mov    dword ptr [esi + 0x20], eax
        or     eax, 0x10
        mov    dword ptr [esi + 0x20], eax
        mov    edi, dword ptr [esp + 0x20]
        mov    cx, word ptr [edi]
        test   cl, 0x80
        je     loc_1_9A9C
        and    eax, 0xfffffff0
        mov    dword ptr [esi + 0x20], eax
        mov    eax, dword ptr [esp + 0x28]
        cmp    eax, 6
        jne    loc_1_99E0
        cmp    byte ptr [esp + 0x30], 0
        je     loc_1_9990
        cmp    dword ptr [edi + 2], -1
        jne    loc_1_99E0
loc_1_9990:
        cmp    dword ptr [edi + 2], -1
        jne    loc_1_99A8
        mov    dword ptr [esi + 0xc], 0
        or     dword ptr [esi + 0x20], 4
        jmp    loc_1_9AF7
        db  08Bh, 0FFh
loc_1_99A8:
        mov    ecx, dword ptr [edi + 2]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     edx, ecx
        mov    dword ptr [esi + 0xc], edx
        or     dword ptr [esi + 0x20], 4
        jmp    loc_1_9AF7
loc_1_99E0:
        sub    edx, edx
        mov    ecx, 6
        div    ecx
        mov    dword ptr [esp + 0x10], eax
        cmp    eax, 0x3e8
        jle    loc_1_9A04
        mov    eax, 0xc0000001
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
loc_1_9A04:
        mov    eax, dword ptr [esp + 0x10]
        lea    eax, [eax*4 + 8]
        and    eax, 0xffff
        push   eax
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    loc_1_9A30
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
loc_1_9A30:
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [esp + 0x10]
        inc    ecx
        cmp    ecx, 1
        jle    loc_1_9A87
        lea    ebx, [eax + 4]
        inc    ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [esp + 0x14], ecx
loc_1_9A4C:
        mov    ecx, dword ptr [edi + 2]
        mov    edx, ecx
        and    edx, 0xff00
        mov    ebp, ecx
        shl    ebp, 0x10
        or     edx, ebp
        shl    edx, 8
        mov    ebp, ecx
        and    ebp, 0xff0000
        shr    ebp, 8
        or     edx, ebp
        shr    ecx, 0x18
        and    ecx, 0xff
        or     edx, ecx
        mov    dword ptr [ebx], edx
        add    edi, 6
        add    ebx, 4
        dec    dword ptr [esp + 0x14]
        jne    loc_1_9A4C
loc_1_9A87:
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + ecx*4 + 4], 0xffffffff
        mov    dword ptr [esi + 0xc], eax
        or     dword ptr [esi + 0x20], 8
        jmp    loc_1_9AF7
loc_1_9A9C:
        mov    eax, dword ptr [esp + 0x28]
        cmp    eax, 6
        jbe    loc_1_9AC8
        cmp    byte ptr [esp + 0x30], 0
        je     loc_1_9AC8
        lea    ecx, [esp + 0x18]
        push   ecx
        push   dword ptr [esp + 0x30]
        push   eax
        push   edi
        call   sub_1_9C54
        add    esp, 0x10
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0xc], eax
        jmp    loc_1_9AF7
loc_1_9AC8:
        mov    ecx, dword ptr [edi + 2]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     edx, ecx
        mov    dword ptr [esi + 0xc], edx
loc_1_9AF7:
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   esi
        call   sub_1_6608
        add    esp, 8
        or     eax, eax
        je     loc_1_9B1A
        cmp    eax, 0xff
        je     loc_1_9B1A
        push   esi
        call   sub_1_DEA4
        add    esp, 4
loc_1_9B1A:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_9B24:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    eax, eax
        mov    esi, dword ptr [0x1323c]      ; -> loc_1_1323C
        cmp    esi, 0x1323c                  ; -> loc_1_1323C
        je     loc_1_9C4A
        inc    ecx
        mov    ecx, dword ptr [esp + 0x2c]
loc_1_9B44:
        mov    edi, dword ptr [esp + 0x28]
        cmp    dword ptr [esp + 0x20], 0
        je     loc_1_9B59
        cmp    esi, dword ptr [esp + 0x20]
        jne    loc_1_9C3C
loc_1_9B59:
        cmp    byte ptr [esp + 0x34], 0
        je     loc_1_9B68
        mov    edx, dword ptr [esi + 0x58]
        and    edx, dword ptr [esi + 0x5c]
        jmp    loc_1_9B6B
loc_1_9B68:
        mov    edx, dword ptr [esi + 0x64]
loc_1_9B6B:
        mov    dword ptr [esp + 0x14], edx
        cmp    dword ptr [esp + 0x24], 0
        je     loc_1_9C3C
        mov    edx, dword ptr [esp + 0x24]
        mov    dword ptr [esp + 0x18], edx
loc_1_9B82:
        mov    edx, dword ptr [edi + 2]
        mov    ebx, edx
        and    ebx, 0xff00
        mov    ebp, edx
        shl    ebp, 0x10
        or     ebx, ebp
        shl    ebx, 8
        mov    ebp, edx
        and    ebp, 0xff0000
        shr    ebp, 8
        or     ebx, ebp
        shr    edx, 0x18
        and    edx, 0xff
        or     ebx, edx
        mov    dword ptr [esp + 0x10], ebx
        cmp    byte ptr [esp + 0x34], 0
        je     loc_1_9BD4
        mov    edx, dword ptr [esi + 0x5c]
        and    edx, ebx
        cmp    edx, dword ptr [esp + 0x14]
        jne    loc_1_9C2F
        mov    edx, dword ptr [esp + 0x30]
        shr    edx, 2
        cmp    edx, eax
        jbe    loc_1_9C2F
        mov    edx, ebx
        jmp    loc_1_9C2B
loc_1_9BD4:
        test   byte ptr [esp + 0x13], 0x80
        jne    loc_1_9BE8
        mov    ebx, dword ptr [esp + 0x10]
        and    ebx, 0xff000000
        jmp    loc_1_9C16
        db  090h
loc_1_9BE8:
        mov    dl, byte ptr [esp + 0x13]
        and    dl, 0xc0
        cmp    dl, 0x80
        jne    loc_1_9C00
        mov    ebx, dword ptr [esp + 0x10]
        and    ebx, 0xffff0000
        jmp    loc_1_9C16
loc_1_9C00:
        mov    dl, byte ptr [esp + 0x13]
        and    dl, 0xe0
        cmp    dl, 0xc0
        jne    loc_1_9C16
        mov    ebx, dword ptr [esp + 0x10]
        and    ebx, 0xffffff00
loc_1_9C16:
        cmp    ebx, dword ptr [esp + 0x14]
        jne    loc_1_9C2F
        mov    edx, dword ptr [esp + 0x30]
        shr    edx, 2
        cmp    edx, eax
        jbe    loc_1_9C2F
        mov    edx, dword ptr [esp + 0x10]
loc_1_9C2B:
        mov    dword ptr [ecx + eax*4], edx
        inc    eax
loc_1_9C2F:
        add    edi, 6
        dec    dword ptr [esp + 0x18]
        jne    loc_1_9B82
loc_1_9C3C:
        mov    esi, dword ptr [esi]
        cmp    esi, 0x1323c                  ; -> loc_1_1323C
        jne    loc_1_9B44
loc_1_9C4A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
sub_1_9C54:
        sub    esp, 0xf0
        push   ebx
        push   esi
        push   edi
        mov    ecx, 6
        mov    edi, dword ptr [esp + 0x104]
        mov    eax, edi
        sub    edx, edx
        div    ecx
        mov    esi, eax
        lea    eax, [esi + esi*2]
        add    eax, eax
        cmp    eax, edi
        jne    loc_1_9D54
        push   1
        push   0xf0
        lea    eax, [esp + 0x14]
        push   eax
        mov    edi, dword ptr [esp + 0x10c]
        push   edi
        push   esi
        push   dword ptr [esp + 0x11c]
        call   sub_1_9B24
        add    esp, 0x18
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_9CE0
        push   1
        push   0xf0
        lea    eax, [esp + 0x14]
        push   eax
        push   edi
        push   esi
        push   0
        call   sub_1_9B24
        add    esp, 0x18
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_9CE0
        push   0
        push   0xf0
        lea    eax, [esp + 0x14]
        push   eax
        push   edi
        push   esi
        push   0
        call   sub_1_9B24
        add    esp, 0x18
        mov    ebx, eax
loc_1_9CE0:
        call   sub_1_000C
        or     ebx, ebx
        je     loc_1_9D08
        shr    eax, 4
        sub    edx, edx
        div    ebx
        mov    ecx, dword ptr [esp + edx*4 + 0xc]
        mov    eax, dword ptr [esp + 0x10c]
        mov    dword ptr [eax], ecx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xf0
        ret    
        db  090h
loc_1_9D08:
        sub    edx, edx
        shr    eax, 4
        div    esi
        lea    eax, [edx + edx*2]
        mov    ecx, dword ptr [edi + eax*2 + 2]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     edx, ecx
        mov    eax, dword ptr [esp + 0x10c]
        mov    dword ptr [eax], edx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xf0
        ret    
        db  08Bh, 0FFh
loc_1_9D54:
        pop    edi
        mov    eax, 0xc0000001
        pop    esi
        pop    ebx
        add    esp, 0xf0
        ret    
        db  090h
sub_1_9D64:
        mov    ecx, dword ptr [0x1323c]      ; -> loc_1_1323C
        mov    ax, word ptr [esp + 8]
        cmp    word ptr [0x13310], ax        ; -> loc_1_13310
        jne    loc_1_9D98
        cmp    ecx, 0x1323c                  ; -> loc_1_1323C
        je     loc_1_9D98
        mov    eax, dword ptr [esp + 4]
loc_1_9D84:
        cmp    dword ptr [ecx + 0x74], eax
        je     loc_1_9D9C
        cmp    dword ptr [ecx + 0x78], eax
        je     loc_1_9D9C
        mov    ecx, dword ptr [ecx]
        cmp    ecx, 0x1323c                  ; -> loc_1_1323C
        jne    loc_1_9D84
loc_1_9D98:
        sub    al, al
        ret    
        db  090h
loc_1_9D9C:
        mov    al, 1
        ret    
        db  090h
sub_1_9DA0:
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    bx, word ptr [esp + 0x54]
        test   bl, 0x80
        je     loc_1_A05C
        mov    eax, dword ptr [esp + 0x48]
        mov    ecx, dword ptr [eax + 0xa]
        mov    esi, ecx
        and    esi, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     esi, eax
        shl    esi, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     esi, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     esi, ecx
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        push   dword ptr [esp + 0x58]
        mov    edi, dword ptr [esp + 0x58]
        lea    eax, [edi + 0xc]
        push   eax
        call   sub_1_10130
        add    esp, 0x10
        or     eax, eax
        je     loc_1_9E12
        cmp    eax, 0xff
        jne    loc_1_A05C
loc_1_9E12:
        lea    eax, [esp + 0x10]
        push   eax
        push   0
        push   edi
        lea    eax, [esp + 0x2c]
        push   eax
        call   sub_1_A0EC
        add    esp, 0x10
        or     eax, eax
        je     loc_1_9E5B
        cmp    eax, 0xff
        je     loc_1_9E4C
        mov    byte ptr [esp + 0x2f], 0
        lea    eax, [esp + 0x10]
        push   eax
        push   0
        push   edi
        lea    eax, [esp + 0x2c]
        push   eax
        call   sub_1_A0EC
        add    esp, 0x10
loc_1_9E4C:
        or     eax, eax
        je     loc_1_9E5B
        cmp    eax, 0xff
        jne    loc_1_A05C
loc_1_9E5B:
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, dword ptr [eax + 0x14]
        or     ebp, ebp
        je     loc_1_9E6C
        mov    word ptr [ebp + 0x3e], 1
loc_1_9E6C:
        cmp    dword ptr [esp + 0x50], 0x22
        jl     loc_1_A05C
        test   bh, 0xf
        jne    loc_1_A05C
        cmp    byte ptr [edi + 7], 0
        je     loc_1_A05C
        mov    eax, dword ptr [esp + 0x14]
        lea    ebx, [eax + edi + 0xd]
        mov    ecx, eax
        cmp    byte ptr [edi + 5], 0
        je     loc_1_9EBE
        add    ebx, 4
        mov    ecx, eax
        add    ecx, 4
        cmp    byte ptr [ebx], 0xc0
        jne    loc_1_9EB0
        add    ebx, 2
        add    ecx, 2
        jmp    loc_1_9EBE
        db  090h
loc_1_9EB0:
        mov    eax, dword ptr [esp + 0x14]
        inc    eax
        add    ebx, eax
        mov    eax, dword ptr [esp + 0x14]
        inc    eax
        add    ecx, eax
loc_1_9EBE:
        mov    edi, ebx
        mov    ax, word ptr [ebx]
        mov    dx, ax
        and    edx, 0xff
        shl    edx, 8
        shr    ax, 8
        and    eax, 0xffff
        or     edx, eax
        cmp    edx, 5
        jne    loc_1_9F42
        add    ebx, 0xa
        mov    ax, word ptr [edi + 8]
        mov    dx, ax
        and    edx, 0xff
        shl    edx, 8
        shr    ax, 8
        and    eax, 0xffff
        or     edx, eax
        add    ebx, edx
        lea    ecx, [edx + ecx + 0xa]
        cmp    byte ptr [ebx], 0xc0
        jne    loc_1_9F10
        add    ebx, 2
        jmp    loc_1_9F40
        db  08Dh, 049h, 000h
loc_1_9F10:
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x34]
        push   eax
        mov    eax, dword ptr [esp + 0x58]
        sub    eax, ecx
        push   eax
        push   ebx
        call   sub_1_10130
        add    esp, 0x10
        or     eax, eax
        je     loc_1_9F39
        cmp    eax, 0xff
        jne    loc_1_A05C
loc_1_9F39:
        mov    eax, dword ptr [esp + 0x14]
        inc    eax
        add    ebx, eax
loc_1_9F40:
        mov    edi, ebx
loc_1_9F42:
        or     ebp, ebp
        je     loc_1_A05C
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   ebp
        call   sub_1_10AEC
        add    esp, 0xc
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_A05C
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x10]
        add    eax, 0x20
        mov    ecx, dword ptr [eax]
        test   cl, 0x80
        je     loc_1_A02C
        and    ecx, 0xffffff0f
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 0x10]
        or     dword ptr [eax + 0x20], 0x10
        mov    ecx, dword ptr [edi + 0xa]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     edx, ecx
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0xc], edx
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x28], 0xffffffff
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   dword ptr [esp + 0x14]
        call   sub_1_6608
        add    esp, 8
        or     eax, eax
        je     loc_1_A020
        cmp    eax, 0xff
        je     loc_1_A020
        push   dword ptr [esp + 0x10]
        call   sub_1_DEA4
        add    esp, 4
loc_1_A020:
        push   1
        push   0
        call   sub_1_FA40
        add    esp, 8
loc_1_A02C:
        mov    eax, dword ptr [esp + 0x44]
        cmp    dword ptr [eax + 0x80], esi
        jne    loc_1_A044
        mov    ecx, dword ptr [eax + 0x7c]
        mov    dword ptr [eax + 0x80], ecx
        mov    dword ptr [eax + 0x7c], esi
loc_1_A044:
        push   eax
        push   0x3200                        ; -> loc_1_3200
        push   dword ptr [esp + 0x20]
        push   0
        push   dword ptr [esp + 0x2c]
        call   sub_1_3C00
        add    esp, 0x14
loc_1_A05C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
sub_1_A064:
        mov    ecx, dword ptr [0x1323c]      ; -> loc_1_1323C
        cmp    ecx, 0x1323c                  ; -> loc_1_1323C
        je     loc_1_A085
        mov    eax, dword ptr [esp + 4]
loc_1_A076:
        cmp    dword ptr [ecx + 0x58], eax
        je     loc_1_A088
        mov    ecx, dword ptr [ecx]
        cmp    ecx, 0x1323c                  ; -> loc_1_1323C
        jne    loc_1_A076
loc_1_A085:
        sub    al, al
        ret    
loc_1_A088:
        mov    al, 1
        ret    
        db  090h
sub_1_A08C:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x74]
        mov    edx, dword ptr [eax + 0x78]
        mov    dword ptr [eax + 0x74], edx
        mov    dword ptr [eax + 0x78], ecx
        mov    cl, byte ptr [eax + 0x93]
        not    cl
        mov    byte ptr [eax + 0x93], cl
        ret    
        db  090h
sub_1_A0AC:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        lea    edi, [esi + edx + 0x11]
        mov    al, byte ptr [edi]
        and    al, 0xc0
        cmp    al, 0xc0
        mov    ecx, 0x10
        je     loc_1_A0D5
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ecx, 0xe
loc_1_A0D5:
        lea    eax, [edx + ecx]
        mov    cl, byte ptr [eax + esi + 0xd]
        movzx  ax, cl
        mov    ah, al
        movsx  cx, cl
        mov    al, ch
        pop    edi
        pop    esi
        ret    
        db  090h
sub_1_A0EC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, 0x1327c                  ; -> loc_1_1327C
        mov    esi, dword ptr [esp + 0x14]
        mov    ebp, dword ptr [esp + 0x18]
        mov    bl, byte ptr [esp + 0x1c]
loc_1_A101:
        mov    edi, dword ptr [edi]
        cmp    edi, 0x1327c                  ; -> loc_1_1327C
        je     loc_1_A144
        or     bl, bl
        jne    loc_1_A137
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_A101
        mov    eax, dword ptr [eax + 0x20]
        mov    cx, word ptr [ebp]
        cmp    word ptr [eax + 0x40], cx
        jne    loc_1_A101
        push   0x10
        push   esi
        lea    eax, [edi + 0x34]
        push   eax
        call   sub_1_0262
        add    esp, 0xc
        cmp    eax, 0x10
        jne    loc_1_A101
loc_1_A137:
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [eax], edi
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_A144:
        pop    ebp
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh, 08Bh, 04Ch, 024h, 008h, 08Bh, 044h, 024h, 004h, 08Ah, 04Ch, 008h, 01Fh, 066h, 00Fh
        db  0BEh, 0C1h, 066h, 08Bh, 0D0h, 081h, 0E2h, 000h, 0FFh, 000h, 000h, 00Fh, 0BFh, 0C0h, 0C1h, 0E0h
        db  010h, 00Bh, 0D0h, 0C1h, 0E2h, 008h, 066h, 00Fh, 0BEh, 0C5h, 025h, 000h, 0FFh, 000h, 000h, 00Bh
        db  0D0h, 0C0h, 0F9h, 018h, 00Fh, 0B6h, 0C1h, 00Bh, 0D0h, 08Bh, 044h, 024h, 00Ch, 039h, 050h, 00Ch
        db  075h, 004h, 02Bh, 0C0h, 0C3h, 090h, 0B8h, 0BCh, 000h, 000h, 0C0h, 0C3h, 08Bh, 0FFh
sub_1_A19C:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 0x2c], 0
        jne    loc_1_A1B2
        mov    dword ptr [esp + 0x2c], 0x7f000000
loc_1_A1B2:
        mov    esi, dword ptr [esp + 0x28]
        mov    ax, word ptr [esi]
        mov    word ptr [esp + 0x12], ax
        add    esi, 2
        cmp    byte ptr [esi], 0x2a
        jne    loc_1_A1D1
        lea    edi, [esi + 1]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
loc_1_A1D1:
        lea    eax, [esp + 0x14]
        mov    dword ptr [esp + 0x14], 0
        push   eax
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   esi
        mov    eax, dword ptr [0x12f80]      ; -> obj1:0x12F80
        push   dword ptr [eax + 0x80]
        call   sub_1_6954
        add    esp, 0x10
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_A20A
        cmp    ebx, 0xff
        jne    loc_1_A4EC
loc_1_A20A:
        mov    eax, dword ptr [esp + 0x14]
        mov    edi, dword ptr [eax + 8]
        or     edi, edi
        je     loc_1_A4EC
        cmp    dword ptr [esp + 0x2c], 0x7f000000
        je     loc_1_A230
        test   byte ptr [eax + 0x20], 2
        je     loc_1_A230
        mov    ecx, dword ptr [esp + 0x2c]
        mov    dword ptr [eax + 0xc], ecx
loc_1_A230:
        inc    dword ptr [edi + 0x18]
        mov    eax, dword ptr [esp + 0x14]
        test   byte ptr [eax + 0x20], 0x40
        jne    loc_1_A242
        mov    ebx, 0xc0000001
loc_1_A242:
        or     ebx, ebx
        je     loc_1_A275
        cmp    ebx, 0xff
        je     loc_1_A275
        cmp    byte ptr [0x13350], 0         ; -> loc_1_13350
        je     loc_1_A270
        mov    eax, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax + 0x28]
        mov    ecx, dword ptr [esp + 0x34]
        test   dword ptr [ecx + 0x88], eax
        jne    loc_1_A270
        sub    ebx, ebx
        jmp    loc_1_A275
        db  08Bh, 0FFh
loc_1_A270:
        mov    ebx, 0xc0000001
loc_1_A275:
        or     ebx, ebx
        je     loc_1_A2EC
        cmp    ebx, 0xff
        je     loc_1_A2BC
        push   6
        mov    eax, dword ptr [esp + 0x38]
        add    eax, 0x8c
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        add    eax, 0x3e
        push   eax
        call   sub_1_0262
        add    esp, 0xc
        cmp    eax, 6
        jne    loc_1_A2B0
        mov    eax, dword ptr [edi + 0xc]
        mov    eax, dword ptr [eax]
        sub    eax, edi
        cmp    eax, 0xc
        jne    loc_1_A2BC
        jmp    loc_1_A2BA
loc_1_A2B0:
        mov    eax, dword ptr [esp + 0x14]
        test   byte ptr [eax + 0x20], 0x40
        je     loc_1_A2BC
loc_1_A2BA:
        sub    ebx, ebx
loc_1_A2BC:
        or     ebx, ebx
        je     loc_1_A2EC
        cmp    ebx, 0xff
        je     loc_1_A2EC
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [ecx], 0
        push   edi
        call   sub_1_DDE4
        add    esp, 4
        mov    ebx, 0xc0000043
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_A2EC:
        mov    eax, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax + 0x20]
        test   al, 0x40
        je     loc_1_A3B0
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [eax + 0x14]
        or     ecx, ecx
        je     loc_1_A343
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0xc
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_A338
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x28], 0
        push   0
        push   dword ptr [esp + 0x20]
        call   dword ptr [esp + 0x20]
        add    esp, 8
loc_1_A338:
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x14], 0
loc_1_A343:
        mov    eax, dword ptr [edi + 0xc]
        sub    eax, edi
        cmp    eax, 0xc
        jne    loc_1_A398
        mov    byte ptr [esp + 0x11], 1
        mov    eax, dword ptr [esp + 0x14]
        and    dword ptr [eax + 0x20], 0xffffff0f
        mov    eax, dword ptr [esp + 0x14]
        or     dword ptr [eax + 0x20], 0x80
loc_1_A368:
        mov    eax, dword ptr [esp + 0x14]
        push   dword ptr [eax + 8]
        call   sub_1_FB08
        add    esp, 4
        or     eax, eax
        jne    loc_1_A404
        push   edi
        call   sub_1_DDE4
        add    esp, 4
        mov    ebx, 1
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_A398:
        push   edi
        call   sub_1_DDE4
        add    esp, 4
        mov    ebx, 0xc00000bd
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_A3B0:
        mov    byte ptr [esp + 0x11], 0
        cmp    word ptr [esp + 0x12], 0
        je     loc_1_A3E4
        cmp    word ptr [esp + 0x12], 2
        je     loc_1_A3E4
        test   al, 4
        jne    loc_1_A368
        push   edi
        call   sub_1_DDE4
        add    esp, 4
        mov    ebx, 0xc0000043
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_A3E4:
        test   al, 2
        jne    loc_1_A368
        push   edi
        call   sub_1_DDE4
        add    esp, 4
        mov    ebx, 0xc0000043
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_A404:
        mov    edx, dword ptr [esp + 0x38]
        mov    dword ptr [eax + 0xc], edx
        mov    ecx, dword ptr [esp + 0x34]
        mov    dword ptr [eax + 0x80], ecx
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [ecx], eax
        mov    ebx, dword ptr [esp + 0x14]
        add    ebx, 0x28
        mov    edx, dword ptr [ebx]
        mov    ecx, dword ptr [esp + 0x34]
        mov    ecx, dword ptr [ecx + 0x88]
        test   ecx, edx
        jne    loc_1_A444
        or     edx, ecx
        mov    dword ptr [ebx], edx
        mov    ecx, dword ptr [esp + 0x14]
        test   byte ptr [ecx + 0x20], 0x10
        je     loc_1_A448
        mov    cl, 1
        jmp    loc_1_A44A
loc_1_A444:
        mov    byte ptr [edi + 0x23], 1
loc_1_A448:
        sub    cl, cl
loc_1_A44A:
        mov    edx, dword ptr [esp + 0x14]
        test   byte ptr [edx + 0x20], 0x10
        je     loc_1_A470
        or     cl, cl
        jne    loc_1_A470
        sub    ebx, ebx
        mov    dword ptr [eax + 0xc], ebx
        mov    byte ptr [eax + 0x89], bl
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_A470:
        or     cl, cl
        mov    edx, dword ptr [esp + 0x38]
        mov    dword ptr [eax + 0xc], edx
        jne    loc_1_A498
        cmp    byte ptr [esp + 0x11], 0
        jne    loc_1_A498
        mov    byte ptr [eax + 0x89], 1
        mov    ebx, 0xff
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_A498:
        mov    byte ptr [eax + 0x89], 0
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x16]
        push   0xa618                        ; -> loc_1_A618
        push   eax
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   esi
        push   0
        push   0
        call   sub_1_E71C
        add    esp, 0x20
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_A60D
        cmp    ebx, 0xff
        je     loc_1_A60D
        push   edi
        call   sub_1_DDE4
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_A4EC:
        push   0x24
        call   sub_1_001E
        add    esp, 4
        mov    ebp, eax
        mov    ebx, 1
        or     ebp, ebp
        je     loc_1_A60D
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        mov    ecx, dword ptr [esp + 0x34]
        mov    dword ptr [ebp + 0x1c], ecx
        mov    dword ptr [ebp + 0x18], ebx
        mov    byte ptr [ebp + 0x22], 2
        cmp    word ptr [esp + 0x12], ax
        je     loc_1_A531
        cmp    word ptr [esp + 0x12], 2
        mov    word ptr [ebp + 0x20], bx
        jne    loc_1_A537
loc_1_A531:
        mov    word ptr [ebp + 0x20], 0
loc_1_A537:
        push   ebp
        lea    eax, [ebp + 0xc]
        mov    dword ptr [ebp + 0x10], eax
        mov    dword ptr [eax], eax
        call   sub_1_FB08
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     loc_1_A604
        mov    edx, dword ptr [esp + 0x38]
        mov    dword ptr [edi + 0xc], edx
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [ecx], edi
        mov    dword ptr [ebp], 0x13274      ; -> loc_1_13274
        mov    eax, dword ptr [0x13278]      ; -> loc_1_13278
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        mov    dword ptr [0x13278], ebp      ; -> loc_1_13278
        mov    dword ptr [ebp + 8], 0x12345678
        mov    ecx, dword ptr [esp + 0x34]
        mov    dword ptr [edi + 0x80], ecx
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_A5AB
        cmp    dword ptr [esp + 0x2c], 0x7f000000
        je     loc_1_A5A3
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x2c]
        mov    dword ptr [eax + 0xc], ecx
loc_1_A5A3:
        mov    dword ptr [esp + 0x2c], 0
loc_1_A5AB:
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x16]
        push   0xa618                        ; -> loc_1_A618
        push   edi
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   esi
        push   dword ptr [esp + 0x44]
        push   0
        call   sub_1_E71C
        add    esp, 0x20
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_A60D
        cmp    ebx, 0xff
        je     loc_1_A60D
        push   edi
        call   sub_1_FAD8
        add    esp, 4
        mov    ecx, dword ptr [ebp]
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   ebp
        call   sub_1_FA98
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_A604:
        push   ebp
        call   sub_1_FA98
        add    esp, 4
loc_1_A60D:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_A618:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x14], 0
        lea    eax, [esp + 0x24]
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x24], eax
        mov    eax, dword ptr [esp + 0x30]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [esp + 0x30]
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [esp + 0x1c]
        inc    dword ptr [eax + 0x18]
        mov    esi, dword ptr [eax + 0x14]
        mov    edi, dword ptr [esp + 0x34]
        or     edi, edi
        je     loc_1_A678
        mov    eax, dword ptr [esi + 0x20]
        and    eax, 0xffffff0f
        mov    dword ptr [esi + 0x20], eax
        or     eax, 0x40
        mov    dword ptr [esi + 0x20], eax
        mov    edi, 0xc00000bd
        jmp    loc_1_A690
        db  08Bh, 0FFh
loc_1_A678:
        lea    eax, [esi + 0x20]
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [esi + 0x20]
        and    eax, 0xffffff0f
        mov    dword ptr [esi + 0x20], eax
        or     eax, 0x10
        mov    dword ptr [esi + 0x20], eax
loc_1_A690:
        inc    eax
        mov    eax, dword ptr [esp + 0x1c]
        add    eax, 0xc
        mov    dword ptr [esp + 0x10], eax
loc_1_A69C:
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [eax]
        cmp    ebx, eax
        je     loc_1_A73D
loc_1_A6AA:
        mov    ebp, ebx
        mov    ebx, dword ptr [ebx]
        mov    ecx, dword ptr [ebp + 0xc]
        or     ecx, ecx
        je     loc_1_A733
        cmp    ebp, dword ptr [esp + 0x30]
        je     loc_1_A6DD
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [ebp + 0x80], eax
        jne    loc_1_A6D0
        cmp    byte ptr [ebp + 0x89], 0
        jne    loc_1_A6DD
loc_1_A6D0:
        or     edi, edi
        je     loc_1_A733
        cmp    byte ptr [ebp + 0x89], 0
        je     loc_1_A733
loc_1_A6DD:
        or     edi, edi
        je     loc_1_A701
        cmp    edi, 0xff
        je     loc_1_A701
        mov    dword ptr [esi + 0x28], 0
        mov    dword ptr [ebp + 0x10], 0
        mov    edx, ebx
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
loc_1_A701:
        push   ebp
        mov    byte ptr [ebp + 0x89], 0
        inc    dword ptr [esp + 0x18]
        push   edi
        push   ecx
        call   sub_1_25A0
        add    esp, 0xc
        mov    dword ptr [ebp + 0xc], 0
        or     edi, edi
        je     loc_1_A733
        cmp    edi, 0xff
        je     loc_1_A733
        push   ebp
        call   sub_1_FAD8
        add    esp, 4
loc_1_A733:
        cmp    ebx, dword ptr [esp + 0x10]
        jne    loc_1_A6AA
loc_1_A73D:
        or     edi, edi
        je     loc_1_A74D
        cmp    edi, 0xff
        jne    loc_1_A7D4
loc_1_A74D:
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax]
loc_1_A754:
        cmp    eax, dword ptr [esp + 0x10]
        je     loc_1_A7F8
        mov    ebx, eax
        mov    eax, dword ptr [eax]
        cmp    byte ptr [ebx + 0x89], 0
        je     loc_1_A754
        mov    byte ptr [ebx + 0x89], 0
        mov    ebp, dword ptr [esi + 0x20]
        test   ebp, 2
        mov    ax, 0
        jne    loc_1_A785
        mov    ax, 1
loc_1_A785:
        test   ebp, 1
        je     loc_1_A791
        or     ax, 2
loc_1_A791:
        push   dword ptr [ebx + 0x80]
        push   eax
        push   0xa618                        ; -> loc_1_A618
        push   ebx
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        lea    eax, [esi + 0x34]
        push   eax
        push   0
        push   0
        call   sub_1_E71C
        add    esp, 0x20
        mov    edi, eax
        mov    dword ptr [esi + 0x20], ebp
        or     edi, edi
        je     loc_1_A7F8
        cmp    edi, 0xff
        je     loc_1_A7F8
        mov    byte ptr [ebx + 0x89], 1
        jmp    loc_1_A69C
        db  08Dh, 049h, 000h
loc_1_A7D4:
        mov    eax, dword ptr [esp + 0x14]
        dec    dword ptr [esp + 0x14]
        or     eax, eax
        je     loc_1_A7F8
loc_1_A7E0:
        push   dword ptr [esp + 0x1c]
        call   sub_1_DDE4
        add    esp, 4
        mov    eax, dword ptr [esp + 0x14]
        dec    dword ptr [esp + 0x14]
        or     eax, eax
        jne    loc_1_A7E0
loc_1_A7F8:
        push   dword ptr [esp + 0x1c]
        call   sub_1_DDE4
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Bh, 0FFh
sub_1_A810:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0x71
        call   sub_1_001E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_A830
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_A830:
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 0x1c
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        lea    eax, [esi + 0x14]
        mov    dword ptr [esi + 0x18], eax
        mov    dword ptr [eax], eax
        lea    eax, [esi + 0x1c]
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [eax], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x24], eax
        mov    dword ptr [esi + 0x68], 0
        mov    dword ptr [esi + 8], 0x87654321
        mov    dword ptr [esi + 0x64], 1
        mov    byte ptr [esi + 0x6d], 8
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax], esi
        mov    edi, dword ptr [esp + 0x1c]
        lea    ecx, [edi + 0xc]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [ecx], esi
        cmp    dword ptr [edi + 0x70], 0
        je     loc_1_A8F0
        push   0x49
        call   sub_1_001E
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_A8D0
        push   edi
        push   ebp
        call   sub_1_A908
        add    esp, 8
        mov    ebx, eax
        mov    dword ptr [ebp + 0x40], 0
        or     ebx, ebx
        je     loc_1_A8C8
        cmp    ebx, 0xff
        je     loc_1_A8C8
        push   ebp
        call   sub_1_0024
        add    esp, 4
        jmp    loc_1_A8D5
        db  08Dh, 049h, 000h
loc_1_A8C8:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_A8D0:
        mov    ebx, 1
loc_1_A8D5:
        push   edi
        push   esi
        call   sub_1_A8F8
        add    esp, 8
        push   esi
        call   sub_1_FAF0
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_A8F0:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_A8F8:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        ret    
        db  090h
sub_1_A908:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        push   esi
        call   sub_1_3630
        add    esp, 8
        mov    ebx, eax
        mov    dword ptr [esi + 0x14], 0
        or     ebx, ebx
        je     loc_1_A932
        cmp    ebx, 0xff
        jne    loc_1_A96F
loc_1_A932:
        push   dword ptr [edi + 0x70]
        push   dword ptr [esi + 0x10]
        call   sub_1_3690
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_A960
        cmp    ebx, 0xff
        je     loc_1_A960
        push   esi
        call   sub_1_37F0
        add    esp, 4
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_A960:
        add    edi, 0x1c
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [edi], esi
loc_1_A96F:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_A978:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [eax]
        cmp    dword ptr [esi + 8], 0x87654321
        jne    loc_1_A9F0
        mov    dword ptr [esp + 4], 0
        mov    edx, dword ptr [esp + 0x10]
        cmp    dword ptr [edx + 8], 0x18273645
        jne    loc_1_A9E4
        mov    dword ptr [esp + 4], 0
        cmp    dword ptr [esi + 0x68], 0
        je     loc_1_A9B8
        mov    eax, 0xc0000008
        pop    esi
        add    esp, 4
        ret    
loc_1_A9B8:
        mov    ecx, dword ptr [esi]
        mov    dword ptr [esi + 0x68], 1
        mov    dword ptr [esi + 0x10], edx
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        add    edx, 0x14
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi], edx
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [edx + 4], esi
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret    
        db  090h
loc_1_A9E4:
        pop    esi
        mov    eax, 0xc0000008
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_A9F0:
        pop    esi
        mov    eax, 0xc0000008
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_1_A9FC:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x2c]
        mov    esi, dword ptr [eax]
        cmp    dword ptr [esi + 8], 0x87654321
        jne    loc_1_AAF4
        mov    dword ptr [esp + 0x14], 0
        mov    edi, dword ptr [esi + 0x10]
        mov    eax, 4
        mov    ecx, dword ptr [esi + 0x68]
        dec    ecx
        cmp    ecx, 8
        ja     loc_1_AAE8
        jmp    dword ptr [ecx*4 + 0xab04]    ; -> obj1:0xAB04
        db  08Bh, 0FFh
loc_1_AA3C:
        mov    eax, 2
loc_1_AA41:
        push   0
        mov    dword ptr [esp + 0x1c], esi
        push   0
        push   0
        push   eax
        push   0x13368                       ; -> loc_1_13368
        lea    eax, [esp + 0x2c]
        push   eax
        call   sub_1_BDC0
        add    esp, 0x18
        mov    dword ptr [esp + 0x14], eax
loc_1_AA62:
        or     edi, edi
        mov    eax, dword ptr [edi + 0x80]
        mov    dword ptr [esp + 0x10], eax
        je     loc_1_AAAB
        add    edi, 0x2c
        mov    ebx, dword ptr [edi]
        cmp    ebx, edi
        je     loc_1_AAAB
loc_1_AA79:
        mov    ebp, ebx
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [ebp + 0x10], esi
        jne    loc_1_AAA7
        mov    ecx, ebx
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0xc0000120
        push   dword ptr [ebp + 8]
        call   sub_1_25A0
        add    esp, 0xc
        push   ebp
        call   sub_1_0024
        add    esp, 4
loc_1_AAA7:
        cmp    ebx, edi
        jne    loc_1_AA79
loc_1_AAAB:
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [esi + 4], esi
        mov    dword ptr [esi], esi
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], esi
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 0x68], 0
        mov    dword ptr [esi + 0x10], 0
        mov    byte ptr [esi + 0x6f], 0
        mov    ecx, dword ptr [esp + 0x10]
        add    ecx, 0xc
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi], ecx
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 4], esi
loc_1_AAE8:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Bh, 0FFh
loc_1_AAF4:
        pop    ebp
        mov    eax, 0xc0000008
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Dh, 049h, 000h
        ; ---- dispatch table (9 entries) ----
        dd  loc_1_AA62
        dd  loc_1_AA3C
        dd  loc_1_AA3C
        dd  loc_1_AA3C
        dd  loc_1_AA3C
        dd  loc_1_AA3C
        dd  loc_1_AA41
        dd  loc_1_AA62
        dd  loc_1_AA62
sub_1_AB28:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [eax]
        cmp    dword ptr [esi + 0x10], 0
        jne    loc_1_AB40
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret    
        db  090h
loc_1_AB40:
        cmp    dword ptr [esi + 8], 0x18273645
        jne    loc_1_AB80
        mov    dword ptr [esp + 4], 0
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0xc], eax
        push   dword ptr [esi + 0x80]
        push   esi
        call   sub_1_AB8C
        add    esp, 8
        mov    dword ptr [esp + 4], eax
        push   esi
        call   sub_1_DD58
        add    esp, 4
        mov    dword ptr [esp + 4], eax
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_AB80:
        pop    esi
        mov    eax, 0xc0000008
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_1_AB8C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        cmp    dword ptr [esi + 0x10], 0
        jne    loc_1_ABA4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_ABA4:
        lea    ebx, [esi + 0x24]
        mov    edi, dword ptr [ebx]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0x24
        je     loc_1_ABDF
        mov    dword ptr [esi + 0x28], ebx
        mov    dword ptr [ebx], ebx
        cmp    edi, ebx
        je     loc_1_ABDF
loc_1_ABBC:
        mov    ebp, edi
        push   0
        push   0xc00000ca
        push   dword ptr [edi + 8]
        call   sub_1_25A0
        add    esp, 0xc
        mov    edi, dword ptr [edi]
        push   ebp
        call   sub_1_0024
        add    esp, 4
        cmp    edi, ebx
        jne    loc_1_ABBC
loc_1_ABDF:
        mov    eax, dword ptr [esi + 0x80]
        mov    dword ptr [esi + 8], 0x18273640
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [esi + 0x1c]
        sub    eax, esi
        cmp    eax, 0x1c
        je     loc_1_AC79
        mov    ebx, dword ptr [esp + 0x18]
        add    ebx, 0xc
loc_1_AC01:
        mov    edi, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [edi]
        lea    ebp, [edi + 4]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ebp], edi
        mov    dword ptr [edi], edi
        mov    ecx, dword ptr [edi + 0xc]
        cmp    word ptr [ecx + 0x32], 3
        jne    loc_1_AC3D
        cmp    byte ptr [ecx + 0x48], 1
        jne    loc_1_AC3D
        mov    edx, dword ptr [ecx + 0x28]
        mov    eax, dword ptr [ecx + 0x2c]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        mov    byte ptr [ecx + 0x48], 0
        lea    eax, [ecx + 0x28]
        mov    dword ptr [ecx + 0x2c], eax
        mov    dword ptr [eax], eax
loc_1_AC3D:
        push   dword ptr [esp + 0x18]
        push   edi
        call   sub_1_ADBC
        add    esp, 8
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edi + 0x68], 0
        mov    dword ptr [edi + 0x10], 0
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [edi], ebx
        mov    dword ptr [ebp], eax
        mov    dword ptr [eax], edi
        mov    dword ptr [ebx + 4], edi
        mov    eax, dword ptr [esi + 0x1c]
        sub    eax, esi
        cmp    eax, 0x1c
        jne    loc_1_AC01
loc_1_AC79:
        lea    ebp, [esi + 0x14]
        mov    edi, dword ptr [ebp]
        mov    dword ptr [ebp + 4], ebp
        mov    dword ptr [ebp], ebp
        cmp    edi, ebp
        je     loc_1_ACE9
        mov    ebx, dword ptr [esp + 0x18]
        add    ebx, 0xc
loc_1_AC90:
        mov    eax, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [eax + 0x1c]
        mov    eax, ecx
        sub    eax, dword ptr [esp + 0x18]
        cmp    eax, 0x1c
        je     loc_1_ACB5
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        push   ecx
        call   sub_1_DF20
        add    esp, 4
loc_1_ACB5:
        mov    ecx, edi
        mov    eax, dword ptr [edi]
        mov    edi, eax
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [edx], eax
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ecx + 0x68], 0
        mov    dword ptr [ecx + 8], 0x87654320
        mov    dword ptr [ecx + 0x10], 0
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx], ebx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebx + 4], ecx
        cmp    edi, ebp
        jne    loc_1_AC90
loc_1_ACE9:
        lea    ebx, [esi + 0x2c]
        mov    ebp, dword ptr [ebx]
        mov    dword ptr [ebx + 4], ebx
        mov    dword ptr [ebx], ebx
        cmp    ebp, ebx
        je     loc_1_AD1C
loc_1_ACF8:
        mov    edi, ebp
        mov    ebp, dword ptr [ebp]
        push   0
        push   0xc00000ca
        push   dword ptr [edi + 8]
        call   sub_1_25A0
        add    esp, 0xc
        push   edi
        call   sub_1_0024
        add    esp, 4
        cmp    ebp, ebx
        jne    loc_1_ACF8
loc_1_AD1C:
        add    esi, 0x3c
        mov    ebx, dword ptr [esi]
        mov    dword ptr [esi + 4], esi
        mov    dword ptr [esi], esi
        cmp    ebx, esi
        je     loc_1_AD5F
loc_1_AD2A:
        mov    edi, ebx
        mov    ebx, dword ptr [ebx]
        push   1
        push   0xc00000ca
        push   dword ptr [edi + 0x1c]
        call   sub_1_25A0
        add    esp, 0xc
        mov    dword ptr [edi], 0x1329c      ; -> loc_1_1329C
        mov    eax, dword ptr [0x132a0]      ; -> loc_1_132A0
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [0x132a0]      ; -> loc_1_132A0
        mov    dword ptr [eax], edi
        mov    dword ptr [0x132a0], edi      ; -> loc_1_132A0
        cmp    ebx, esi
        jne    loc_1_AD2A
loc_1_AD5F:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_AD68:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [eax]
        cmp    dword ptr [esi + 8], 0x87654321
        jne    loc_1_ADB0
        mov    dword ptr [esp + 4], 0
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_1_ADBC
        add    esp, 8
        mov    dword ptr [esp + 4], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x58], eax
        push   esi
        call   sub_1_DEE4
        add    esp, 4
        mov    dword ptr [esp + 4], eax
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_ADB0:
        pop    esi
        mov    eax, 0xc0000008
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_1_ADBC:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x14], 0
        mov    bl, 1
        mov    esi, dword ptr [esp + 0x34]
        cmp    dword ptr [esi + 0x68], 0
        je     loc_1_AE52
        cmp    dword ptr [esi + 8], 0x87654321
        jne    loc_1_AE64
        mov    dword ptr [esp + 0x14], 0
        mov    eax, dword ptr [esi + 0x68]
        cmp    eax, 3
        je     loc_1_ADF9
        cmp    eax, 2
        jne    loc_1_AE1D
loc_1_ADF9:
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   dword ptr [esp + 0x40]
        push   esi
        call   sub_1_AFAC
        add    esp, 0x10
        mov    dword ptr [esp + 0x14], eax
        cmp    eax, 0xff
        je     loc_1_AE1D
        sub    bl, bl
loc_1_AE1D:
        or     bl, bl
        mov    dword ptr [esp + 0x20], esi
        je     loc_1_AE43
        push   0
        push   0
        push   0
        push   2
        push   0x13368                       ; -> loc_1_13368
        lea    eax, [esp + 0x34]
        push   eax
        call   sub_1_BDC0
        add    esp, 0x18
        mov    dword ptr [esp + 0x14], eax
loc_1_AE43:
        cmp    dword ptr [esp + 0x14], 0xf
        jne    loc_1_AE52
        mov    dword ptr [esp + 0x14], 0
loc_1_AE52:
        cmp    dword ptr [esi + 8], 0x87654321
        jne    loc_1_AE74
        mov    byte ptr [esp + 0x13], 1
        jmp    loc_1_AE79
        db  08Bh, 0FFh
loc_1_AE64:
        pop    ebp
        mov    eax, 0xc0000008
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_AE74:
        mov    byte ptr [esp + 0x13], 0
loc_1_AE79:
        mov    dword ptr [esi + 8], 0x87654320
        lea    eax, [esp + 0x18]
        push   eax
        push   esi
        call   sub_1_AF30
        add    esp, 8
        mov    ebp, dword ptr [esi + 0x10]
        or     ebp, ebp
        je     loc_1_AED2
        add    ebp, 0x2c
        mov    edi, dword ptr [ebp]
        cmp    edi, ebp
        je     loc_1_AED2
        nop    
loc_1_AEA0:
        mov    ebx, edi
        mov    edi, dword ptr [edi]
        cmp    dword ptr [ebx + 0x10], esi
        jne    loc_1_AECE
        mov    ecx, edi
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0xc0000120
        push   dword ptr [ebx + 8]
        call   sub_1_25A0
        add    esp, 0xc
        push   ebx
        call   sub_1_0024
        add    esp, 4
loc_1_AECE:
        cmp    edi, ebp
        jne    loc_1_AEA0
loc_1_AED2:
        cmp    byte ptr [esi + 0x6c], 0
        jne    loc_1_AF04
        cmp    byte ptr [esp + 0x13], 0
        je     loc_1_AF04
        mov    eax, dword ptr [esp + 0x38]
        mov    edx, dword ptr [eax + 0x1c]
        mov    eax, edx
        sub    eax, dword ptr [esp + 0x38]
        cmp    eax, 0x1c
        je     loc_1_AF04
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   edx
        call   sub_1_DF20
        add    esp, 4
loc_1_AF04:
        mov    ecx, dword ptr [esi]
        cmp    dword ptr [esi + 0x68], 0
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        jle    loc_1_AF22
        mov    dword ptr [esi + 0x68], 0
        mov    dword ptr [esi + 0x10], 0
loc_1_AF22:
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Bh, 0FFh
sub_1_AF30:
        push   esi
        mov    esi, dword ptr [esp + 8]
        add    esi, 0x1c
        cmp    dword ptr [esi], esi
        je     loc_1_AF5E
loc_1_AF3C:
        mov    eax, dword ptr [esi]
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        push   0
        push   0xc0000120
        push   dword ptr [eax + 0x1c]
        call   sub_1_25A0
        add    esp, 0xc
        cmp    dword ptr [esi], esi
        jne    loc_1_AF3C
loc_1_AF5E:
        pop    esi
        ret    
sub_1_AF60:
        push   esi
        mov    edx, 1
        mov    ecx, dword ptr [esp + 0xc]
loc_1_AF6A:
        mov    esi, dword ptr [0x1336c]      ; -> loc_1_1336C
        cmp    esi, 0x1336c                  ; -> loc_1_1336C
        je     loc_1_AF87
loc_1_AF78:
        cmp    dword ptr [esi + 8], ecx
        je     loc_1_AF98
        mov    esi, dword ptr [esi]
        cmp    esi, 0x1336c                  ; -> loc_1_1336C
        jne    loc_1_AF78
loc_1_AF87:
        mov    eax, edx
        dec    edx
        or     eax, eax
        jne    loc_1_AF6A
        mov    eax, 0xc0000001
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_AF98:
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], esi
        sub    eax, eax
        pop    esi
        ret    
sub_1_AFAC:
        sub    esp, 0x14
        push   esi
        push   edi
        mov    edi, 0xc0000001
        mov    dword ptr [esp + 0xc], 0
        mov    edx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [edx + 0xc]
        mov    ecx, dword ptr [edx + 0x68]
        cmp    ecx, 3
        jne    loc_1_B0E0
        cmp    dword ptr [eax + 0x38], 3
        jne    loc_1_AFE4
        mov    eax, 0xff
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
        db  08Bh, 0FFh
loc_1_AFE4:
        mov    esi, dword ptr [edx + 0x4c]
        or     esi, esi
        je     loc_1_B0EE
        mov    dword ptr [esp + 8], 0
        lea    eax, [esp + 8]
        push   eax
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        mov    eax, dword ptr [esi + 0x38]
        add    eax, 0x34
        push   eax
        push   dword ptr [0x132b8]           ; -> loc_1_132B8
        call   sub_1_6954
        add    esp, 0x10
        mov    edi, eax
        or     edi, edi
        je     loc_1_B029
        cmp    edi, 0xff
        jne    loc_1_B0F2
loc_1_B029:
        mov    eax, dword ptr [esp + 8]
        test   byte ptr [eax + 0x20], 0x10
        je     loc_1_B040
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
        db  08Bh, 0FFh
loc_1_B040:
        mov    eax, dword ptr [esp + 8]
        add    eax, 0x14
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     loc_1_B0A0
        mov    dword ptr [eax], 0
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0xc
        mov    edi, eax
        mov    eax, dword ptr [esp + 8]
        and    dword ptr [eax + 0x20], 0xffffff7f
        mov    eax, dword ptr [esp + 8]
        or     dword ptr [eax + 0x20], 0x20
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x10], 0
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_B099
        push   dword ptr [esp + 8]
        call   sub_1_DEA4
        add    esp, 4
loc_1_B099:
        sub    esi, esi
        jmp    loc_1_B0F2
        db  08Dh, 049h, 000h
loc_1_B0A0:
        cmp    dword ptr [0x13374], 0        ; -> loc_1_13374
        je     loc_1_B0C8
        mov    eax, dword ptr [0x13374]      ; -> loc_1_13374
        cmp    dword ptr [eax + 8], esi
        jne    loc_1_B0C8
        mov    dword ptr [esp + 0xc], eax
        mov    dword ptr [0x13374], 0        ; -> loc_1_13374
        sub    edi, edi
        jmp    loc_1_B0F2
        db  08Dh, 049h, 000h
loc_1_B0C8:
        lea    eax, [esp + 0xc]
        push   eax
        push   esi
        push   0x1336c                       ; -> loc_1_1336C
        call   sub_1_AF60
        add    esp, 0xc
        mov    edi, eax
        jmp    loc_1_B0F2
        db  090h
loc_1_B0E0:
        cmp    ecx, 2
        jne    loc_1_B0EE
        mov    eax, dword ptr [edx + 0x4c]
        mov    word ptr [eax + 0x4c], 0x40
loc_1_B0EE:
        mov    esi, dword ptr [esp + 0x18]
loc_1_B0F2:
        or     edi, edi
        je     loc_1_B0FE
        cmp    edi, 0xff
        jne    loc_1_B16D
loc_1_B0FE:
        cmp    dword ptr [esp + 0xc], 0
        je     loc_1_B13F
        mov    eax, dword ptr [esp + 0xc]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esp + 0x14], eax
        or     esi, esi
        je     loc_1_B133
        push   dword ptr [esi + 0x38]
        call   sub_1_12CC0
        add    esp, 4
        push   esi
        call   sub_1_F280
        add    esp, 4
loc_1_B133:
        push   dword ptr [esp + 0xc]
        call   sub_1_0024
        add    esp, 4
loc_1_B13F:
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_B168
        sub    edi, edi
        push   0xc0000120
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        call   sub_1_E64C
        add    esp, 0xc
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
loc_1_B168:
        mov    edi, 0xc0000001
loc_1_B16D:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
loc_1_B178:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x20]
        mov    esi, dword ptr [ebx + 8]
        mov    edi, dword ptr [esi + 0x14]
        mov    dword ptr [esi + 0x30], 0
        test   byte ptr [esi + 0x4c], 0x40
        je     loc_1_B1D8
        mov    eax, dword ptr [edi + 0x54]
        or     eax, eax
        je     loc_1_B1B3
        mov    dword ptr [edi + 0x54], 0
        push   0
        push   0xc0000120
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
loc_1_B1B3:
        mov    eax, dword ptr [esi + 0x44]
        or     eax, eax
        jne    loc_1_B1CC
        push   5
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_B1CC:
        dec    eax
        mov    dword ptr [esi + 0x44], eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_B1D8:
        mov    dword ptr [esp + 0xc], edi
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [edi + 0x68], 1
        push   eax
        push   esi
        push   0
        push   0
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_B238
        add    esp, 0x14
        mov    esi, eax
        push   ebx
        call   sub_1_0024
        add    esp, 4
        or     esi, esi
        je     loc_1_B22E
        cmp    esi, 0xff
        je     loc_1_B22E
        cmp    dword ptr [edi + 0x54], 0
        je     loc_1_B22E
        mov    dword ptr [edi + 0x54], 0
        push   0
        push   0xc00000bc
        push   0
        call   sub_1_25A0
        add    esp, 0xc
loc_1_B22E:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
sub_1_B238:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    eax, eax
        mov    ecx, dword ptr [esp + 0x34]
        mov    ebx, dword ptr [ecx]
        cmp    dword ptr [esp + 0x3c], eax
        je     loc_1_B26D
        mov    ecx, dword ptr [ebx + 0x10]
        mov    ecx, dword ptr [ecx + 0x80]
        cmp    dword ptr [ecx + 0x70], eax
        je     loc_1_B2A8
        cmp    dword ptr [ecx + 0x58], eax
        je     loc_1_B2A8
        cmp    dword ptr [ebx + 8], 0x87654321
        jne    loc_1_B298
        mov    dword ptr [esp + 0x14], eax
loc_1_B26D:
        mov    edx, dword ptr [esp + 0x40]
        cmp    dword ptr [esp + 0x3c], 0
        jne    loc_1_B2D0
        mov    dword ptr [esp + 0x18], edx
        test   byte ptr [edx + 0x4c], 0x40
        je     loc_1_B2D0
        mov    eax, dword ptr [edx + 0x44]
        or     eax, eax
        jne    loc_1_B2B8
        push   5
        push   edx
        call   sub_1_D5C0
        add    esp, 8
        jmp    loc_1_B2BC
        db  08Bh, 0FFh
loc_1_B298:
        pop    ebp
        mov    eax, 0xc0000008
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_B2A8:
        pop    ebp
        mov    eax, 0xc00000be
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_B2B8:
        dec    eax
        mov    dword ptr [edx + 0x44], eax
loc_1_B2BC:
        mov    dword ptr [esp + 0x14], 0xc0000120
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_B2D0:
        mov    ecx, dword ptr [ebx + 0x68]
        cmp    ecx, 1
        je     loc_1_B2F4
        cmp    ecx, 9
        je     loc_1_B2F4
        mov    dword ptr [esp + 0x14], 0xc0000010
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_B2F4:
        mov    ebp, dword ptr [ebx + 0x10]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ecx + 0x14]
        test   byte ptr [ecx + 0x20], 0x40
        je     loc_1_B318
        mov    dword ptr [esp + 0x14], 0xc00000bd
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  090h
loc_1_B318:
        mov    ecx, dword ptr [ebp + 0x80]
        mov    dword ptr [esp + 0x1c], ecx
        cmp    dword ptr [ecx + 0x58], 0
        jne    loc_1_B33C
        mov    dword ptr [esp + 0x14], 0xc00000be
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_B33C:
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [ebx + 0x68], 3
        add    dword ptr [ebx + 0x64], 2
        mov    esi, dword ptr [ebx + 4]
        mov    dword ptr [esi], ecx
        mov    dword ptr [ecx + 4], esi
        lea    esi, [ebp + 0x1c]
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebx], esi
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ecx], ebx
        mov    dword ptr [esi + 4], ebx
        cmp    dword ptr [esp + 0x3c], 0
        je     loc_1_B4AC
        mov    eax, dword ptr [esp + 0x44]
        mov    dword ptr [ebx + 0x54], eax
        mov    byte ptr [ebx + 0x6c], 1
        mov    byte ptr [ebx + 0x6e], 2
        lea    eax, [esp + 0x28]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        push   dword ptr [eax + 0x14]
        call   sub_1_FF70
        add    esp, 0xc
        mov    dword ptr [esp + 0x14], eax
        or     eax, eax
        je     loc_1_B3B4
        cmp    eax, 0xff
        je     loc_1_B3B4
        mov    dword ptr [ebx + 0x68], 1
        mov    dword ptr [esp + 0x14], 0xc00000be
        jmp    loc_1_B3DF
loc_1_B3B4:
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        mov    dword ptr [esp + 0x14], eax
        or     eax, eax
        je     loc_1_B410
        cmp    eax, 0xff
        je     loc_1_B410
        mov    dword ptr [ebx + 0x68], 1
        mov    dword ptr [esp + 0x14], 1
loc_1_B3DF:
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        add    ebp, 0x14
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [ebx], ebp
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [eax], ebx
        mov    dword ptr [ebp + 4], ebx
        mov    eax, dword ptr [ebx + 0x64]
        dec    eax
        mov    dword ptr [ebx + 0x64], eax
        dec    eax
        mov    dword ptr [ebx + 0x64], eax
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_B410:
        mov    eax, dword ptr [esp + 0x20]
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [ecx + 0x24], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0x20], 0
        mov    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x44]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0x44], 1
        mov    eax, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [eax + 0x2c], ecx
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0x14], ebx
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0x34], 0x8b
        mov    eax, dword ptr [esp + 0x20]
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [ecx + 0x38], eax
        mov    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x38]
        mov    dword ptr [ecx + 0x3c], eax
        mov    ax, word ptr [0x132d4]        ; -> loc_1_132D4
        lea    eax, [eax*2 + 0x46]
        mov    word ptr [esp + 0x12], ax
        mov    dword ptr [esp + 0x14], 1
        and    eax, 0xffff
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [ecx + 0x1c], eax
        lea    edi, [ebx + 0x28]
        mov    esi, dword ptr [esp + 0x20]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_B4B3
        db  090h
loc_1_B4AC:
        mov    dword ptr [esp + 0x18], edx
        inc    dword ptr [edx + 0x44]
loc_1_B4B3:
        mov    dword ptr [ebx + 0x5c], 0
        cmp    dword ptr [esp + 0x3c], 0
        je     loc_1_B537
        or     eax, eax
        je     loc_1_B70A
        cmp    dword ptr [esp + 0x3c], 0
        je     loc_1_B537
        mov    byte ptr [eax], 0x81
        mov    byte ptr [eax + 1], 0
        movzx  esi, word ptr [esp + 0x12]
        lea    ecx, [esi - 4]
        shr    ecx, 8
        mov    dx, word ptr [esp + 0x12]
        sub    dx, 4
        mov    ch, dl
        mov    word ptr [eax + 2], cx
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [ecx + 0x18], esi
        movzx  ecx, word ptr [0x132d4]       ; -> loc_1_132D4
        push   ecx
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   dword ptr [esp + 0x28]
        add    eax, 4
        push   eax
        call   sub_1_10064
        add    esp, 0x10
        movzx  ecx, word ptr [0x132d4]       ; -> loc_1_132D4
        push   ecx
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        mov    ecx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ecx + 0x14]
        add    ecx, 0x34
        push   ecx
        push   eax
        call   sub_1_10064
        add    esp, 0x10
loc_1_B537:
        push   0
        push   dword ptr [esp + 0x20]
        push   ebx
        call   sub_1_36C0
        add    esp, 0xc
        mov    dword ptr [esp + 0x14], eax
        or     eax, eax
        je     loc_1_B559
        cmp    eax, 0xff
        jne    loc_1_B70A
loc_1_B559:
        mov    eax, dword ptr [ebx + 0xc]
        mov    byte ptr [eax + 0x45], 1
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [eax + 0x38], 1
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ebx + 0x60], eax
        cmp    dword ptr [esp + 0x3c], 0
        je     loc_1_B5A0
        mov    ecx, dword ptr [esp + 0x1c]
        mov    edx, dword ptr [ecx + 0x1c]
        mov    eax, edx
        sub    eax, ecx
        cmp    eax, 0x1c
        je     loc_1_B5B2
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   edx
        call   sub_1_DF20
        add    esp, 4
        jmp    loc_1_B5B2
        db  08Dh, 049h, 000h
loc_1_B5A0:
        mov    eax, dword ptr [esp + 0x18]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [esp + 0x20], eax
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [ecx + 0x38], eax
loc_1_B5B2:
        lea    esi, [ebx + 0x4c]
        lea    eax, [esp + 0x24]
        push   eax
        push   1
        push   esi
        push   0xb78c                        ; -> loc_1_B78C
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x30]
        call   sub_1_C224
        add    esp, 0x1c
        mov    dword ptr [esp + 0x14], eax
        or     eax, eax
        jne    loc_1_B5FC
        mov    eax, dword ptr [esp + 0x24]
        cmp    dword ptr [eax + 0x24], 0xfacf0000
        jne    loc_1_B5F0
        mov    ecx, dword ptr [eax + 0xc]
        jmp    loc_1_B600
loc_1_B5F0:
        mov    ecx, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [ecx + 0x58]
        jmp    loc_1_B600
        db  08Dh, 049h, 000h
loc_1_B5FC:
        mov    ecx, dword ptr [esp + 0x2c]
loc_1_B600:
        cmp    dword ptr [esp + 0x3c], 0
        jne    loc_1_B612
        cmp    dword ptr [esp + 0x44], 0
        je     loc_1_B612
        mov    ecx, dword ptr [esp + 0x44]
loc_1_B612:
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_B700
        mov    eax, dword ptr [esp + 0x18]
        test   byte ptr [eax + 0x4c], 0x40
        je     loc_1_B640
        push   dword ptr [esp + 0x24]
        call   sub_1_DEA4
        add    esp, 4
        mov    dword ptr [esp + 0x14], 0xc0000120
        jmp    loc_1_B70A
loc_1_B640:
        mov    eax, dword ptr [esp + 0x24]
        test   byte ptr [eax + 0x20], 2
        jne    loc_1_B66C
        test   byte ptr [0x13364], 1         ; -> loc_1_13364
        jne    loc_1_B66C
        push   eax
        call   sub_1_DEA4
        add    esp, 4
        mov    dword ptr [esp + 0x14], 0xc00000be
        jmp    loc_1_B70A
        db  08Dh, 049h, 000h
loc_1_B66C:
        mov    eax, dword ptr [esp + 0x18]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [eax + 0x68], 3
        mov    eax, dword ptr [esp + 0x18]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [eax + 0x44], 0
        mov    eax, dword ptr [esp + 0x18]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [eax + 0x48], 0
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        mov    eax, ecx
        shr    eax, 0x18
        and    eax, 0xff
        or     edx, eax
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [eax + 0x34], edx
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [eax + 0x38], 3
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [esp + 0x18]
        push   dword ptr [eax + 0x34]
        push   0xb9e8                        ; -> loc_1_B9E8
        push   dword ptr [eax + 0x2c]
        push   ecx
        push   dword ptr [esp + 0x28]
        call   sub_1_116C0
        add    esp, 0x14
        mov    dword ptr [esp + 0x14], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Bh, 0FFh
loc_1_B700:
        cmp    dword ptr [esp + 0x14], 0xff
        je     loc_1_B780
loc_1_B70A:
        push   ebx
        mov    esi, dword ptr [ebx + 0xc]
        mov    dword ptr [ebx + 0xc], 0
        mov    dword ptr [ebx + 0x68], 1
        call   sub_1_BCD4
        add    esp, 4
        or     esi, esi
        je     loc_1_B754
        mov    dword ptr [esi + 0xc], 0
        push   esi
        push   ebx
        call   sub_1_D61C
        add    esp, 8
        inc    dword ptr [esi + 0x3c]
        push   dword ptr [esi + 0x18]
        push   0x7930                        ; -> loc_1_7930
        push   0
        push   esi
        push   0
        call   sub_1_3C00
        add    esp, 0x14
        jmp    loc_1_B75D
        db  090h
loc_1_B754:
        push   ebx
        call   sub_1_DEE4
        add    esp, 4
loc_1_B75D:
        push   5
        push   dword ptr [esp + 0x1c]
        call   sub_1_D5C0
        add    esp, 8
        push   ebx
        call   sub_1_DEE4
        add    esp, 4
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_B780:
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_B78C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        mov    ebp, dword ptr [esi + 0x14]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [esp + 0x20]
        or     edi, edi
        jne    loc_1_B879
        test   byte ptr [esi + 0x4c], 0x40
        jne    loc_1_B879
        mov    dword ptr [ebp + 0x4c], 0
        lea    eax, [esp + 0x14]
        push   eax
        push   esi
        call   sub_1_C4AC
        add    esp, 8
        or     eax, eax
        je     loc_1_B874
        cmp    dword ptr [eax + 0x24], 0xfacf0000
        jne    loc_1_B7E0
        mov    edx, dword ptr [eax + 0xc]
        jmp    loc_1_B7EC
        db  08Bh, 0FFh
loc_1_B7E0:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ecx + 0x80]
        mov    edx, dword ptr [ecx + 0x58]
loc_1_B7EC:
        test   byte ptr [esp + 0x14], 2
        jne    loc_1_B7FC
        test   byte ptr [0x13364], 1         ; -> loc_1_13364
        je     loc_1_B874
loc_1_B7FC:
        mov    edi, edx
        mov    dword ptr [ebx + 0x38], 3
        mov    dword ptr [ebp + 0x68], 3
        mov    dword ptr [ebp + 0x44], 0
        mov    dword ptr [ebp + 0x48], 0
        mov    dword ptr [esi + 0x38], eax
        and    edi, 0xff00
        mov    ecx, edx
        shl    ecx, 0x10
        or     edi, ecx
        shl    edi, 8
        mov    ecx, edx
        and    ecx, 0xff0000
        shr    ecx, 8
        or     edi, ecx
        mov    ecx, edx
        shr    ecx, 0x18
        and    ecx, 0xff
        or     edi, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x34], edi
        mov    dword ptr [ebp + 0x4c], 0
        inc    dword ptr [eax + 0x1c]
        push   dword ptr [esi + 0x34]
        push   0xb9e8                        ; -> loc_1_B9E8
        push   dword ptr [esi + 0x2c]
        push   edx
        push   esi
        call   sub_1_116C0
        add    esp, 0x14
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_B874:
        mov    edi, 0xc00000be
loc_1_B879:
        mov    eax, dword ptr [ebp + 0x54]
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [ebp + 0x54], 0
        mov    bx, word ptr [esi + 0x4c]
        and    bl, 0x40
        push   5
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        cmp    dword ptr [ebp + 0x68], 0
        je     loc_1_B8A6
        mov    dword ptr [ebp + 0x68], 1
loc_1_B8A6:
        sub    esi, esi
        or     bl, bl
        je     loc_1_B8B4
        mov    esi, dword ptr [ebp + 0x5c]
        mov    edi, 0xc0000120
loc_1_B8B4:
        push   ebp
        call   sub_1_B908
        add    esp, 4
        cmp    dword ptr [ebp + 0x68], 0
        je     loc_1_B8CC
        push   ebp
        call   sub_1_BCD4
        add    esp, 4
loc_1_B8CC:
        push   ebp
        call   sub_1_DEE4
        add    esp, 4
        cmp    edi, 0x15
        jne    loc_1_B8DF
        mov    edi, 0xc00000be
loc_1_B8DF:
        push   0
        push   edi
        push   dword ptr [esp + 0x18]
        call   sub_1_25A0
        add    esp, 0xc
        or     esi, esi
        je     loc_1_B8FF
        push   0
        push   0
        push   esi
        call   sub_1_25A0
        add    esp, 0xc
loc_1_B8FF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
sub_1_B908:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        or     ecx, ecx
        je     loc_1_B96D
        mov    esi, dword ptr [ecx + 0xc]
        or     esi, esi
        je     loc_1_B96D
        mov    eax, dword ptr [esi + 0x38]
        or     eax, eax
        jle    loc_1_B96D
        cmp    eax, 8
        jge    loc_1_B96D
        mov    dword ptr [esi + 0x38], 8
        cmp    dword ptr [ecx + 0x68], 0
        je     loc_1_B938
        mov    dword ptr [ecx + 0x68], 9
loc_1_B938:
        cmp    eax, 6
        mov    dword ptr [ecx + 0xc], 0
        mov    dword ptr [esi + 0xc], 0
        jae    loc_1_B94E
        inc    dword ptr [esi + 0x3c]
loc_1_B94E:
        push   esi
        push   ecx
        call   sub_1_D61C
        add    esp, 8
        push   dword ptr [esi + 0x18]
        push   0x7930                        ; -> loc_1_7930
        push   0
        push   esi
        push   0
        call   sub_1_3C00
        add    esp, 0x14
loc_1_B96D:
        pop    esi
        ret    
        db  090h
sub_1_B970:
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esi + 0x3c]
        or     eax, eax
        je     loc_1_B988
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 4], eax
        jmp    loc_1_B990
        db  090h
loc_1_B988:
        mov    dword ptr [esp + 4], 0
loc_1_B990:
        cmp    dword ptr [esp + 4], 0x2710
        jae    loc_1_B9A2
        mov    dword ptr [esp + 4], 0x2710
loc_1_B9A2:
        lea    eax, [esp + 8]
        push   eax
        push   0
        push   0xbc58                        ; -> loc_1_BC58
        push   esi
        push   0xbc58                        ; -> loc_1_BC58
        push   0
        push   esi
        push   dword ptr [esp + 0x20]
        call   sub_1_109B8
        add    esp, 0x20
        or     eax, eax
        je     loc_1_B9DC
        cmp    eax, 0xff
        je     loc_1_B9DC
        mov    dword ptr [esi + 0x30], 0
        pop    esi
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_B9DC:
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [esi + 0x30], ecx
        pop    esi
        add    esp, 8
        ret    
loc_1_B9E8:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x2c]
        mov    edi, dword ptr [esi + 0x14]
        push   dword ptr [esi + 0x38]
        call   sub_1_DEA4
        add    esp, 4
        mov    eax, dword ptr [edi + 0xc]
        test   byte ptr [esi + 0x4c], 0x40
        mov    ebp, dword ptr [esp + 0x30]
        jne    loc_1_BA89
        or     ebp, ebp
        je     loc_1_BA1A
        cmp    ebp, 0xff
        jne    loc_1_BA89
loc_1_BA1A:
        or     eax, eax
        mov    dword ptr [edi + 0x68], 6
        je     loc_1_BA84
        mov    dword ptr [eax + 0x38], 6
        inc    dword ptr [eax + 0x3c]
        mov    word ptr [eax + 0x32], 0
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esp + 0x18], eax
        mov    dword ptr [esp + 0x1c], 0xbb44 ; -> loc_1_BB44
        mov    dword ptr [esp + 0x20], esi
        push   edi
        push   esi
        call   sub_1_B970
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_BA60
        cmp    ebp, 0xff
        jne    loc_1_BA89
loc_1_BA60:
        add    esi, 0x18
        push   0
        push   esi
        lea    eax, [esp + 0x1c]
        push   eax
        push   dword ptr [esi]
        push   0
        lea    eax, [esp + 0x2c]
        push   eax
        call   sub_1_3A60
        add    esp, 0x18
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
loc_1_BA84:
        mov    ebp, 0xc0000001
loc_1_BA89:
        mov    eax, dword ptr [edi + 0x54]
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [edi + 0x54], 0
        mov    bx, word ptr [esi + 0x4c]
        and    bl, 0x40
        mov    eax, dword ptr [esi + 0x38]
        cmp    dword ptr [eax + 0x24], 0xfacf0000
        jne    loc_1_BABF
        cmp    dword ptr [eax + 0x1c], 1
        jne    loc_1_BABF
        test   byte ptr [eax + 0x20], 0x10
        je     loc_1_BABF
        push   eax
        call   sub_1_DEA4
        add    esp, 4
loc_1_BABF:
        push   5
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        cmp    dword ptr [edi + 0x68], 0
        je     loc_1_BAD7
        mov    dword ptr [edi + 0x68], 1
loc_1_BAD7:
        sub    esi, esi
        or     bl, bl
        je     loc_1_BAE5
        mov    esi, dword ptr [edi + 0x5c]
        mov    ebp, 0xc0000120
loc_1_BAE5:
        cmp    ebp, 0xe
        je     loc_1_BAEF
        cmp    ebp, 0x15
        jne    loc_1_BAF4
loc_1_BAEF:
        mov    ebp, 0xc00000be
loc_1_BAF4:
        push   edi
        call   sub_1_B908
        add    esp, 4
        cmp    dword ptr [edi + 0x68], 0
        je     loc_1_BB0C
        push   edi
        call   sub_1_BCD4
        add    esp, 4
loc_1_BB0C:
        push   edi
        call   sub_1_DEE4
        add    esp, 4
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_BB2B
        push   0
        push   ebp
        push   dword ptr [esp + 0x18]
        call   sub_1_25A0
        add    esp, 0xc
loc_1_BB2B:
        or     esi, esi
        je     loc_1_BB3C
        push   0
        push   0
        push   esi
        call   sub_1_25A0
        add    esp, 0xc
loc_1_BB3C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
loc_1_BB44:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        mov    edi, dword ptr [esi + 0x14]
        mov    ebx, dword ptr [esp + 0x1c]
        or     ebx, ebx
        je     loc_1_BC2C
        cmp    ebx, 0xff
        je     loc_1_BC2C
        mov    eax, dword ptr [esi + 0x30]
        or     eax, eax
        je     loc_1_BB8C
        push   0
        lea    ecx, [esp + 0x14]
        push   ecx
        push   eax
        call   sub_1_10AEC
        add    esp, 0xc
        mov    dword ptr [esi + 0x30], 0
        jmp    loc_1_BB94
        db  08Bh, 0FFh
loc_1_BB8C:
        mov    dword ptr [esp + 0x10], 0
loc_1_BB94:
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_BC04
        push   5
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        mov    dword ptr [edi + 0x60], 0
        mov    esi, dword ptr [edi + 0x68]
        mov    dword ptr [edi + 0x68], 1
        mov    ebp, dword ptr [edi + 0x54]
        mov    dword ptr [edi + 0x54], 0
        mov    dword ptr [edi + 0x5c], 0
        cmp    esi, 6
        jne    loc_1_BBD6
        push   edi
        call   sub_1_B908
        add    esp, 4
loc_1_BBD6:
        or     esi, esi
        je     loc_1_BBE3
        push   edi
        call   sub_1_BCD4
        add    esp, 4
loc_1_BBE3:
        push   edi
        call   sub_1_DEE4
        add    esp, 4
        or     ebp, ebp
        je     loc_1_BC4D
        push   0
        push   ebx
        push   ebp
        call   sub_1_25A0
        add    esp, 0xc
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_BC04:
        cmp    dword ptr [edi + 0x60], esi
        jne    loc_1_BC1B
        push   5
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        mov    dword ptr [edi + 0x60], 0
loc_1_BC1B:
        push   edi
        call   sub_1_DEE4
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_BC2C:
        mov    eax, dword ptr [esi + 0x44]
        or     eax, eax
        jne    loc_1_BC40
        push   5
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        jmp    loc_1_BC44
loc_1_BC40:
        dec    eax
        mov    dword ptr [esi + 0x44], eax
loc_1_BC44:
        push   edi
        call   sub_1_DEE4
        add    esp, 4
loc_1_BC4D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_BC58:
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_BCCD
        mov    eax, dword ptr [esp + 0x10]
        mov    edi, dword ptr [eax + 0x14]
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0x30], 0
        or     ecx, ecx
        je     loc_1_BCCD
        cmp    dword ptr [edi + 0x68], 6
        jne    loc_1_BCCD
        mov    ebx, dword ptr [edi + 0x54]
        or     ebx, ebx
        je     loc_1_BCCD
        mov    dword ptr [edi + 0x54], 0
        mov    si, word ptr [edi + 0x68]
        mov    dword ptr [edi + 0x68], 1
        push   edi
        call   sub_1_B908
        add    esp, 4
        or     si, si
        je     loc_1_BCAD
        push   edi
        call   sub_1_BCD4
        add    esp, 4
loc_1_BCAD:
        mov    eax, 0xc0000120
        cmp    dword ptr [esp + 0x14], 0xc0000120
        je     loc_1_BCC1
        mov    eax, 0x15
loc_1_BCC1:
        push   0
        push   eax
        push   ebx
        call   sub_1_25A0
        add    esp, 0xc
loc_1_BCCD:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_BCD4:
        mov    edx, dword ptr [esp + 4]
        cmp    dword ptr [edx + 0x68], 0
        je     loc_1_BD02
        mov    dword ptr [edx + 0x68], 1
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [edx + 0x10]
        add    ecx, 0x14
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 4], edx
loc_1_BD02:
        ret    
        db  090h, 02Bh, 0C0h, 0C3h, 090h
sub_1_BD08:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        mov    ebx, dword ptr [edi]
        cmp    dword ptr [ebx + 8], 0x87654321
        jne    loc_1_BDB4
        mov    dword ptr [esp + 0xc], 0
        cmp    dword ptr [ebx + 0x68], 1
        je     loc_1_BD3C
        mov    eax, 0xc0000008
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_BD3C:
        mov    esi, dword ptr [ebx + 0x10]
        push   0x24
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    loc_1_BD5C
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_BD5C:
        mov    dword ptr [eax + 0x10], ebx
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0x18], ecx
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [eax + 0x1c], ecx
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [eax + 0x20], ecx
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [eax + 0xc], ecx
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [eax + 0x14], ecx
        add    esi, 0x2c
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax], esi
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esp + 0xc], 0
        mov    dword ptr [esp + 0xc], 0xff
        mov    eax, 0xff
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_BDB4:
        pop    edi
        mov    eax, 0xc0000008
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_BDC0:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    byte ptr [esp + 0x13], 0
        mov    eax, dword ptr [esp + 0x28]
        mov    esi, dword ptr [eax]
        mov    eax, dword ptr [esi + 0x68]
        cmp    eax, 1
        jle    loc_1_C034
        cmp    eax, 8
        jge    loc_1_C034
        mov    ebx, dword ptr [esi + 0xc]
        mov    dword ptr [esi + 0x5c], edi
        cmp    dword ptr [esp + 0x30], 1
        jne    loc_1_BE20
        cmp    eax, 7
        jne    loc_1_BE10
        mov    eax, dword ptr [esp + 0x3c]
        mov    dword ptr [esi + 0x58], eax
        mov    eax, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  090h
loc_1_BE10:
        pop    ebp
        mov    eax, 0xf
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
loc_1_BE20:
        or     ebx, ebx
        je     loc_1_BFE0
        mov    eax, dword ptr [esi + 0x68]
        cmp    eax, 1
        jle    loc_1_BF90
        mov    edi, dword ptr [esi + 0x60]
        cmp    eax, 2
        je     loc_1_BECC
        cmp    eax, 3
        je     loc_1_BEEC
        cmp    eax, 6
        je     loc_1_BF40
loc_1_BE52:
        cmp    dword ptr [esi + 0x68], 7
        je     loc_1_BE60
        mov    dword ptr [esp + 0x30], 2
loc_1_BE60:
        mov    dword ptr [esi + 0x68], 1
        mov    dword ptr [esi + 0xc], 0
        mov    dword ptr [ebx + 0xc], 0
        mov    edi, dword ptr [ebx + 0x38]
        mov    ax, word ptr [ebx + 0x32]
        movzx  ebp, ax
        cmp    ax, 3
        jne    loc_1_BEA1
        cmp    byte ptr [ebx + 0x48], 1
        jne    loc_1_BEA1
        lea    edx, [ebx + 0x28]
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [ebx + 0x2c]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    byte ptr [ebx + 0x48], 0
        mov    dword ptr [ebx + 0x2c], edx
        mov    dword ptr [edx], edx
loc_1_BEA1:
        mov    dword ptr [ebx + 0x38], 8
        cmp    dword ptr [esi + 0x5c], 0
        jne    loc_1_BEB5
        mov    eax, dword ptr [esp + 0x3c]
        mov    dword ptr [ebx + 0x40], eax
loc_1_BEB5:
        push   ebx
        push   esi
        call   sub_1_D61C
        add    esp, 8
        mov    byte ptr [esp + 0x13], 1
        jmp    loc_1_BFA0
        db  08Dh, 049h, 000h
loc_1_BECC:
        lea    eax, [esp + 0x14]
        or     word ptr [edi + 0x4c], 0x40
        push   eax
        push   esi
        call   sub_1_AF30
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
loc_1_BEEC:
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        push   dword ptr [ebx + 0x18]
        push   esi
        call   sub_1_AFAC
        add    esp, 0x10
        cmp    eax, 0xc0000001
        jne    loc_1_BF24
        mov    word ptr [edi + 0x4c], 0x40
        mov    eax, dword ptr [esp + 0x3c]
        mov    dword ptr [esi + 0x5c], eax
        mov    eax, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  090h
loc_1_BF24:
        cmp    eax, 0xff
        jne    loc_1_C081
        mov    word ptr [edi + 0x4c], 0x40
        inc    dword ptr [ebx + 0x3c]
        jmp    loc_1_BE52
        db  08Dh, 049h, 000h
loc_1_BF40:
        mov    esi, dword ptr [edi + 0x30]
        or     esi, esi
        je     loc_1_BF80
        mov    dword ptr [edi + 0x30], 0
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   esi
        call   sub_1_10AEC
        add    esp, 0xc
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_BF76
        push   esi
        push   0
        push   dword ptr [esp + 0x24]
        call   dword ptr [esp + 0x24]
        add    esp, 0xc
loc_1_BF76:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_BF80:
        sub    eax, eax
        or     word ptr [edi + 0x4c], 0x40
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  090h
loc_1_BF90:
        sub    ebp, ebp
        mov    dword ptr [ebx + 0xc], 0
        mov    dword ptr [esi + 0xc], 0
loc_1_BFA0:
        cmp    ebp, 2
        jne    loc_1_BFD2
        mov    eax, dword ptr [esi + 0x60]
        or     eax, eax
        je     loc_1_BFCA
        mov    dword ptr [esi + 0x60], 0
        push   0
        push   0xc0000120
        push   eax
        call   sub_1_25A0
        add    esp, 0xc
        mov    dword ptr [esi + 0x60], 0
loc_1_BFCA:
        mov    dword ptr [esp + 0x30], 2
loc_1_BFD2:
        cmp    dword ptr [esi + 0x40], 0
        je     loc_1_BFE0
        mov    dword ptr [esp + 0x30], 2
loc_1_BFE0:
        lea    eax, [esp + 0x14]
        push   eax
        push   esi
        call   sub_1_AF30
        add    esp, 8
        cmp    byte ptr [esp + 0x13], 0
        je     loc_1_BFFE
        push   esi
        call   sub_1_BCD4
        add    esp, 4
loc_1_BFFE:
        mov    al, 1
        cmp    dword ptr [esp + 0x3c], 0
        je     loc_1_C009
        sub    al, al
loc_1_C009:
        push   eax
        push   dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x38]
        push   edi
        push   ebx
        call   sub_1_C08C
        add    esp, 0x14
        cmp    dword ptr [esi + 0x5c], 0
        je     loc_1_C081
        cmp    eax, 1
        je     loc_1_C081
        mov    eax, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_C034:
        mov    al, byte ptr [esi + 0x6f]
        or     al, al
        je     loc_1_C07C
        cmp    dword ptr [esp + 0x30], 1
        jne    loc_1_C06C
        cmp    al, 2
        jne    loc_1_C058
        mov    eax, 0x14
        mov    byte ptr [esi + 0x6f], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  090h
loc_1_C058:
        pop    ebp
        mov    eax, 0x16
        mov    byte ptr [esi + 0x6f], 0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
loc_1_C06C:
        sub    eax, eax
        mov    byte ptr [esi + 0x6f], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Bh, 0FFh
loc_1_C07C:
        mov    eax, 0xf
loc_1_C081:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
sub_1_C08C:
        sub    esp, 4
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [esp + 0x10]
        cmp    esi, edi
        je     loc_1_C107
        mov    eax, dword ptr [esp + 0x14]
        cmp    eax, 7
        ja     loc_1_C107
        cmp    eax, 3
        jb     loc_1_C107
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        cmp    eax, edi
        je     loc_1_C0D0
        cmp    eax, 0xff
        je     loc_1_C0D0
        mov    edi, 1
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_C0D0:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x14], esi
        push   dword ptr [esp + 0x20]
        mov    esi, dword ptr [esp + 0x1c]
        push   esi
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x14]
        call   sub_1_118E4
        add    esp, 0x10
        mov    edi, eax
        cmp    esi, 2
        jne    loc_1_C107
        push   0
        push   0
        push   dword ptr [esp + 0x10]
        call   sub_1_1192C
        add    esp, 0xc
loc_1_C107:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  090h
sub_1_C110:
        sub    esp, 4
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax + 8], 0x87654321
        jne    loc_1_C188
        mov    dword ptr [esp], 0
        cmp    dword ptr [eax + 0x68], 5
        jne    loc_1_C178
        mov    dword ptr [eax + 0x68], 7
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [eax + 0x38], 7
        mov    word ptr [eax + 0x32], 0
        push   0
        push   0x82
        push   eax
        call   sub_1_11788
        add    esp, 0xc
        mov    dword ptr [esp], eax
        or     eax, eax
        je     loc_1_C166
        cmp    eax, 0xff
        jne    loc_1_C180
loc_1_C166:
        mov    dword ptr [esp], 0
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_C178:
        mov    dword ptr [esp], 0xc0000001
loc_1_C180:
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret    
loc_1_C188:
        mov    eax, 0xc0000008
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_C194:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [eax]
        cmp    dword ptr [esi + 8], 0x18273645
        jne    loc_1_C218
        mov    dword ptr [esp + 4], 0
        push   0x1c
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    loc_1_C1C8
        mov    eax, 1
        pop    esi
        add    esp, 4
        ret    
        db  090h
loc_1_C1C8:
        mov    ecx, dword ptr [esp + 0x28]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x14], ecx
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x18], ecx
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0x10], ecx
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [eax + 0xc], ecx
        add    esi, 0x24
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax], esi
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esp + 4], 0
        mov    dword ptr [esp + 4], 0xff
        mov    eax, 0xff
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_C218:
        pop    esi
        mov    eax, 0xc0000008
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_1_C224:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x2c]
        or     ebx, ebx
        je     loc_1_C239
        mov    dword ptr [ebx], 0
loc_1_C239:
        mov    ebp, dword ptr [esp + 0x20]
        cmp    byte ptr [ebp], 0x2a
        je     loc_1_C2E4
        cmp    byte ptr [ebp + 0xf], 0x1d
        jne    loc_1_C258
        cmp    byte ptr [esp + 0x30], 0
        jne    loc_1_C2E4
loc_1_C258:
        mov    esi, 0xc0000001
        mov    dword ptr [esp + 0x10], 0
        cmp    byte ptr [ebp + 0xf], 0x1c
        je     loc_1_C287
        lea    eax, [esp + 0x10]
        push   eax
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   ebp
        push   dword ptr [0x132b4]           ; -> loc_1_132B4
        call   sub_1_6954
        add    esp, 0x10
        mov    esi, eax
loc_1_C287:
        or     esi, esi
        je     loc_1_C293
        cmp    esi, 0xff
        jne    loc_1_C2BE
loc_1_C293:
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x20]
        test   al, 0xc0
        jne    loc_1_C2BE
        test   al, 0xc
        je     loc_1_C357
        test   byte ptr [0x13364], 1         ; -> loc_1_13364
        jne    loc_1_C357
        cmp    byte ptr [esp + 0x30], 0
        jne    loc_1_C357
loc_1_C2BE:
        lea    eax, [esp + 0x10]
        mov    dword ptr [esp + 0x10], 0
        push   eax
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   ebp
        push   dword ptr [0x132b8]           ; -> loc_1_132B8
        call   sub_1_6954
        add    esp, 0x10
        mov    esi, eax
        jmp    loc_1_C357
loc_1_C2E4:
        push   0x44
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        je     loc_1_C49C
        mov    edi, eax
        add    edi, 0x34
        mov    esi, ebp
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [esp + 0x24]
        mov    eax, dword ptr [eax + 0x60]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x20], 0x14
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x1c], 0
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x24], 0xdead0000
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x28], 0xffffffff
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], eax
        sub    esi, esi
loc_1_C357:
        or     esi, esi
        je     loc_1_C367
        cmp    esi, 0xff
        jne    loc_1_C430
loc_1_C367:
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [eax + 0x20]
        test   dl, 0x20
        jne    loc_1_C430
        or     esi, esi
        je     loc_1_C383
        cmp    esi, 0xff
        jne    loc_1_C39A
loc_1_C383:
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [eax + 0x28]
        mov    eax, dword ptr [esp + 0x24]
        test   dword ptr [eax + 0x88], ecx
        je     loc_1_C430
loc_1_C39A:
        test   dl, 0x10
        je     loc_1_C3F4
        mov    eax, dword ptr [esp + 0x10]
        inc    dword ptr [eax + 0x1c]
        mov    ax, word ptr [0x1331c]        ; -> loc_1_1331C
        mov    ecx, dword ptr [esp + 0x10]
        mov    word ptr [ecx + 0x30], ax
        mov    edi, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [edi + 0x38]
        cmp    byte ptr [eax + 0xf], 0x1c
        jne    loc_1_C3D9
        push   0x1348c                       ; -> loc_1_1348C
        push   eax
        call   sub_1_12108
        add    esp, 8
        mov    dword ptr [edi + 0x14], eax
        or     eax, eax
        je     loc_1_C3D9
        inc    dword ptr [eax + 0x1c]
loc_1_C3D9:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [edi + 0x38], eax
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [eax], ecx
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_C3F4:
        test   dl, 0x80
        je     loc_1_C420
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x1c]
        push   ebp
        push   dword ptr [esp + 0x20]
        call   sub_1_C50C
        add    esp, 0x14
        mov    esi, eax
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_C420:
        pop    ebp
        mov    esi, 0xc00000c4
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_C430:
        or     esi, esi
        je     loc_1_C43C
        cmp    esi, 0xff
        jne    loc_1_C455
loc_1_C43C:
        mov    eax, dword ptr [esp + 0x10]
        test   byte ptr [eax + 0x20], 0x20
        je     loc_1_C455
        cmp    dword ptr [eax + 0x1c], 1
        jne    loc_1_C455
        push   eax
        call   sub_1_DEA4
        add    esp, 4
loc_1_C455:
        mov    edi, dword ptr [esp + 0x1c]
        lea    eax, [edi + 8]
        mov    dword ptr [edi + 0xc], eax
        mov    dword ptr [eax], eax
        lea    eax, [esp + 0x14]
        push   eax
        push   ebx
        push   dword ptr [esp + 0x2c]
        push   0
        mov    ax, word ptr [0x13364]        ; -> loc_1_13364
        and    eax, 0xf
        push   eax
        push   dword ptr [esp + 0x3c]
        push   edi
        push   0
        push   0
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   ebp
        call   sub_1_E088
        add    esp, 0x2c
        mov    esi, eax
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_C49C:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
sub_1_C4AC:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esp + 0x10]
        push   edi
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   dword ptr [esi + 0x38]
        push   1
        call   sub_1_DFA0
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_C4E8
        push   edi
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   dword ptr [esi + 0x38]
        push   0
        call   sub_1_DFA0
        add    esp, 0x10
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_C4E8:
        mov    ecx, dword ptr [esi + 0x2c]
        mov    ecx, dword ptr [ecx + 0x88]
        test   dword ptr [eax + 0x28], ecx
        je     loc_1_C504
        mov    cx, word ptr [0x1331c]        ; -> loc_1_1331C
        mov    word ptr [eax + 0x30], cx
        pop    edi
        pop    esi
        ret    
loc_1_C504:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_C50C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        cmp    byte ptr [esi + 0x32], 0
        je     loc_1_C56C
        push   0
        push   0
        push   dword ptr [esi + 0x14]
        call   sub_1_10AEC
        add    esp, 0xc
        mov    dword ptr [esi + 0x14], 0
        mov    dword ptr [esi + 0x20], 0x20
        lea    eax, [esp + 0x14]
        push   eax
        push   0
        mov    edi, dword ptr [esp + 0x20]
        push   dword ptr [edi + 0x2c]
        push   esi
        mov    ax, word ptr [0x13364]        ; -> loc_1_13364
        and    eax, 0xf
        push   eax
        push   dword ptr [esp + 0x30]
        push   edi
        push   0
        push   0
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   dword ptr [esp + 0x38]
        call   sub_1_E088
        add    esp, 0x2c
        pop    edi
        pop    esi
        ret    
loc_1_C56C:
        mov    edx, dword ptr [esi + 0x10]
        mov    edi, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [edi + 0x48], eax
        add    edx, 8
        mov    ecx, dword ptr [edx + 4]
        lea    eax, [edi + 8]
        mov    dword ptr [eax], edx
        mov    dword ptr [edi + 0xc], ecx
        mov    dword ptr [ecx], eax
        mov    dword ptr [edx + 4], eax
        mov    eax, 0xff
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_C598:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x1c]
        cmp    dword ptr [edi + 0x58], 0
        je     loc_1_C6C4
        cmp    dword ptr [edi + 0x6c], 0
        je     loc_1_C6C4
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [eax]
        cmp    dword ptr [ebx + 8], 0x18273645
        jne    loc_1_C6B8
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [ebp + 0xc]
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   dword ptr [esi + 0x14]
        call   sub_1_FF70
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_1_C600
        cmp    eax, 0xff
        je     loc_1_C600
        mov    eax, 0xc000000d
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  090h
loc_1_C600:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp - 0xc]
        push   ebx
        push   dword ptr [esi + 0x14]
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        call   sub_1_C6D0
        add    esp, 0x20
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_1_C634
        cmp    eax, 0xff
        je     loc_1_C634
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Bh, 0FFh
loc_1_C634:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x2c], edi
        lea    eax, [ebp - 0x10]
        push   eax
        push   1
        push   0
        push   0xc930                        ; -> loc_1_C930
        push   edi
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        call   sub_1_C224
        add    esp, 0x1c
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    loc_1_C66B
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 0x10]
        call   sub_1_C9D8
        add    esp, 8
loc_1_C66B:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_C698
        cmp    dword ptr [ebp - 4], 0xff
        je     loc_1_C6A1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x38], 0
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 8]
        call   sub_1_CCDC
        add    esp, 0xc
        jmp    loc_1_C6A8
        db  08Bh, 0FFh
loc_1_C698:
        cmp    dword ptr [ebp - 4], 0xff
        jne    loc_1_C6A8
loc_1_C6A1:
        mov    dword ptr [ebp - 4], 0
loc_1_C6A8:
        pop    edi
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_C6B8:
        pop    edi
        mov    eax, 0xc0000008
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Bh, 0FFh
loc_1_C6C4:
        pop    edi
        mov    eax, 0xc0000010
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Bh, 0FFh
sub_1_C6D0:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        movzx  eax, word ptr [0x132d4]       ; -> loc_1_132D4
        lea    ecx, [eax*2 + 0x50]
        mov    dword ptr [esp + 0x1c], ecx
        mov    ebp, ecx
        add    ebp, 3
        and    ebp, 0xfffffffc
        mov    ebx, dword ptr [esp + 0x24]
        lea    ecx, [ebp + ebx + 0x10]
        mov    dword ptr [esp + 0x14], ecx
        push   ecx
        lea    eax, [esp + 0x14]
        push   eax
        call   sub_1_C8D0
        add    esp, 8
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_C720
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_C720:
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x3c]
        mov    dword ptr [eax], ecx
        mov    cx, word ptr [0x1330c]        ; -> loc_1_1330C
        shr    cx, 0xa
        or     cl, 2
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax + 1], cl
        mov    ch, byte ptr [0x1330e]        ; -> loc_1_1330E
        mov    cl, byte ptr [0x1330f]        ; -> obj1:0x1330F
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax + 2], cx
        call   sub_1_C8B4
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax + 8], 0x8a00
        mov    ax, word ptr [0x132d4]        ; -> loc_1_132D4
        add    ax, ax
        movzx  ecx, ax
        movzx  eax, bx
        add    ecx, eax
        add    ecx, 0x42
        shr    ecx, 8
        mov    ax, word ptr [0x132d4]        ; -> loc_1_132D4
        lea    eax, [ebx + eax*2 + 0x42]
        mov    ch, al
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax + 0xa], cx
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax + 0xc], 0
        mov    eax, dword ptr [esp + 0x28]
        mov    ecx, dword ptr [eax + 0x58]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     edx, ecx
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 4], edx
        mov    ecx, dword ptr [esp + 0x10]
        add    ecx, 0xe
        movzx  eax, word ptr [0x132d4]       ; -> loc_1_132D4
        push   eax
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        mov    eax, dword ptr [esp + 0x38]
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax + 0x14]
        add    eax, 0x34
        push   eax
        push   ecx
        call   sub_1_10064
        add    esp, 0x10
        movzx  ecx, word ptr [0x132d4]       ; -> loc_1_132D4
        push   ecx
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        mov    esi, dword ptr [esp + 0x3c]
        push   esi
        push   eax
        call   sub_1_10064
        add    esp, 0x10
        mov    eax, dword ptr [esp + 0x10]
        add    eax, ebp
        mov    dword ptr [esp + 0x18], eax
        mov    edi, eax
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        lea    ebp, [ebp + eax + 0x10]
        or     ebx, ebx
        je     loc_1_C848
        mov    edi, ebp
        mov    esi, dword ptr [esp + 0x38]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_1_C84A
loc_1_C848:
        sub    ebp, ebp
loc_1_C84A:
        call   sub_1_FE74
        or     eax, eax
        je     loc_1_C894
        mov    dword ptr [eax + 0x24], ebp
        mov    dword ptr [eax + 0x20], ebx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x1c], ecx
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [eax + 0x18], ecx
        mov    dword ptr [eax + 0x14], 0
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0x38], ecx
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x44], ecx
        mov    dword ptr [eax + 0x14], 0
        mov    ecx, dword ptr [esp + 0x40]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_C894:
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x14]
        call   sub_1_C904
        add    esp, 8
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
sub_1_C8B4:
        inc    word ptr [0x1330e]            ; -> loc_1_1330E
        cmp    word ptr [0x1330e], 0xffff    ; -> loc_1_1330E
        jne    loc_1_C8CF
        mov    word ptr [0x1330e], 0         ; -> loc_1_1330E
loc_1_C8CF:
        ret    
sub_1_C8D0:
        mov    eax, dword ptr [0x133a4]      ; -> loc_1_133A4
        cmp    eax, dword ptr [0x1331f]      ; -> obj1:0x1331F
        jbe    loc_1_C8E8
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], 0
        ret    
loc_1_C8E8:
        mov    eax, dword ptr [esp + 8]
        add    dword ptr [0x133a4], eax      ; -> loc_1_133A4
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        ret    
        db  08Bh, 0FFh
sub_1_C904:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, dword ptr [0x133a4]      ; -> loc_1_133A4
        ja     loc_1_C918
        sub    dword ptr [0x133a4], eax      ; -> loc_1_133A4
        jmp    loc_1_C922
loc_1_C918:
        mov    dword ptr [0x133a4], 0        ; -> loc_1_133A4
loc_1_C922:
        push   dword ptr [esp + 4]
        call   sub_1_0024
        add    esp, 4
        ret    
        db  090h
loc_1_C930:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    esi, dword ptr [esp + 0x18]
        cmp    esi, ebx
        mov    edi, dword ptr [esp + 0x14]
        je     loc_1_C94D
        mov    eax, dword ptr [edi + 0x38]
        cmp    byte ptr [eax + 0xf], 0x1c
        jne    loc_1_C9B0
loc_1_C94D:
        lea    eax, [esp + 0xc]
        push   eax
        push   edi
        call   sub_1_C4AC
        add    esp, 8
        mov    ebx, eax
        mov    eax, dword ptr [edi + 0x38]
        cmp    byte ptr [eax + 0xf], 0x1c
        jne    loc_1_C984
        push   0x1348c                       ; -> loc_1_1348C
        push   eax
        call   sub_1_12108
        add    esp, 8
        mov    dword ptr [edi + 0x14], eax
        or     ebx, ebx
        jne    loc_1_C988
        mov    ebx, eax
        mov    dword ptr [edi + 0x14], 0
loc_1_C984:
        or     ebx, ebx
        je     loc_1_C9B4
loc_1_C988:
        inc    dword ptr [ebx + 0x1c]
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_C995
        inc    dword ptr [eax + 0x1c]
loc_1_C995:
        push   edi
        mov    dword ptr [edi + 0x38], ebx
        push   ebx
        call   sub_1_C9D8
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     loc_1_C9D1
        cmp    esi, 0xff
        je     loc_1_C9D1
loc_1_C9B0:
        or     ebx, ebx
        jne    loc_1_C9BB
loc_1_C9B4:
        mov    dword ptr [edi + 0x38], 0
loc_1_C9BB:
        cmp    esi, 0x15
        jne    loc_1_C9C5
        mov    esi, 0xc00000be
loc_1_C9C5:
        push   0
        push   esi
        push   edi
        call   sub_1_CCDC
        add    esp, 0xc
loc_1_C9D1:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_C9D8:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0xc]
        mov    esi, dword ptr [edx + 0x20]
        mov    edi, esi
        and    edi, 2
        je     loc_1_C9F4
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [ecx + 0x1c]
        mov    byte ptr [eax], 0x10
        jmp    loc_1_CA0B
loc_1_C9F4:
        cmp    byte ptr [edx + 0x34], 0x2a
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [ecx + 0x1c]
        jne    loc_1_CA08
        mov    byte ptr [eax], 0x12
        jmp    loc_1_CA0B
        db  08Bh, 0FFh
loc_1_CA08:
        mov    byte ptr [eax], 0x11
loc_1_CA0B:
        test   esi, 8
        je     loc_1_CA20
        push   edx
        push   ecx
        call   sub_1_CC18
        add    esp, 8
        pop    edi
        pop    esi
        ret    
loc_1_CA20:
        cmp    dword ptr [edx + 0x24], 0xfacf0000
        je     loc_1_CA38
        or     edi, edi
        je     loc_1_CA38
        mov    eax, dword ptr [ecx + 0x2c]
        mov    edx, dword ptr [eax + 0x58]
        jmp    loc_1_CA4E
        db  08Dh, 049h, 000h
loc_1_CA38:
        mov    edx, dword ptr [edx + 0xc]
        test   esi, 4
        je     loc_1_CA4E
        test   byte ptr [0x13364], 1         ; -> loc_1_13364
        je     loc_1_CA4E
        sub    edx, edx
loc_1_CA4E:
        mov    eax, dword ptr [ecx + 0x2c]
        mov    word ptr [ecx + 0x34], 0
        cmp    dword ptr [eax + 0x58], 0
        je     loc_1_CA64
        mov    eax, dword ptr [eax + 0x6c]
        jmp    loc_1_CA66
        db  08Bh, 0FFh
loc_1_CA64:
        sub    eax, eax
loc_1_CA66:
        push   0x20
        push   0x8a
        push   ecx
        push   0xcacc                        ; -> sub_1_CACC
        push   eax
        push   edx
        push   ecx
        call   sub_1_115D4
        add    esp, 0x1c
        mov    eax, 0xff
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_CA88:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 8]
        mov    edx, dword ptr [esi + 0xc]
        mov    eax, dword ptr [ecx + 0x2c]
        cmp    dword ptr [eax + 0x58], 0
        je     loc_1_CAA4
        mov    eax, dword ptr [eax + 0x6c]
        jmp    loc_1_CAA6
        db  08Dh, 049h, 000h
loc_1_CAA4:
        sub    eax, eax
loc_1_CAA6:
        push   0x20
        push   0x8a
        push   ecx
        push   0xcacc                        ; -> sub_1_CACC
        push   eax
        push   edx
        push   ecx
        call   sub_1_115D4
        add    esp, 0x1c
        push   esi
        call   sub_1_0024
        add    esp, 4
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_CACC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    dx, word ptr [esi + 0x34]
        dec    dword ptr [esi + 0x40]
        or     dx, dx
        je     loc_1_CB2C
        cmp    dx, 0xfffd
        jae    loc_1_CB1C
        movzx  ecx, dx
        mov    eax, dword ptr [esi + 0x38]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + ecx*4]
        inc    dx
        mov    word ptr [esi + 0x34], dx
        cmp    eax, -1
        je     loc_1_CB14
        inc    dword ptr [esi + 0x40]
        push   dword ptr [esi + 0x2c]
        push   0xca88                        ; -> loc_1_CA88
        push   0
        push   eax
        push   esi
        call   sub_1_3C00
        add    esp, 0x14
        pop    esi
        ret    
loc_1_CB14:
        mov    word ptr [esi + 0x34], 0xfffd
        pop    esi
        ret    
loc_1_CB1C:
        cmp    dword ptr [esi + 0x30], 0
        je     loc_1_CB60
        mov    word ptr [esi + 0x34], 0xfffe
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_CB2C:
        cmp    dword ptr [esi + 0x14], 0
        je     loc_1_CB60
        mov    eax, dword ptr [esi + 0x38]
        cmp    byte ptr [eax + 0x43], 0x1c
        jne    loc_1_CB60
        push   eax
        call   sub_1_DEA4
        add    esp, 4
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [esi + 0x14], 0
        mov    dword ptr [esi + 0x38], eax
        push   eax
        push   esi
        call   sub_1_CC18
        add    esp, 8
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_CB60:
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_CCDC
        add    esp, 0xc
        pop    esi
        ret    
        db  090h
loc_1_CB74:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        cmp    word ptr [edi + 0x34], 0xfffe
        jne    loc_1_CBF8
        cmp    dword ptr [edi + 0x40], 0
        jne    loc_1_CBE8
        mov    ebx, dword ptr [edi + 0x14]
        or     ebx, ebx
        je     loc_1_CBD4
        mov    dword ptr [edi + 0x14], 0
        mov    eax, dword ptr [ebx + 0xc]
        mov    esi, dword ptr [eax]
        push   dword ptr [edi + 0x38]
        mov    word ptr [edi + 0x34], 1
        call   sub_1_DEA4
        add    esp, 4
        mov    dword ptr [edi + 0x38], ebx
        inc    dword ptr [edi + 0x40]
        push   dword ptr [edi + 0x2c]
        push   0xca88                        ; -> loc_1_CA88
        push   0
        push   esi
        push   edi
        call   sub_1_3C00
        add    esp, 0x14
        mov    eax, dword ptr [esp + 0x18]
        or     word ptr [eax + 0x3c], 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_CBD4:
        push   0
        push   0
        push   edi
        call   sub_1_CCDC
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_CBE8:
        mov    eax, dword ptr [esp + 0x18]
        or     word ptr [eax + 0x3c], 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_CBF8:
        inc    dword ptr [edi + 0x40]
        push   0
        push   0
        push   edi
        call   sub_1_CACC
        add    esp, 0xc
        mov    eax, dword ptr [esp + 0x18]
        or     word ptr [eax + 0x3c], 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_CC18:
        push   esi
        push   edi
        mov    edi, 0xc0000001
        sub    edx, edx
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi + 0x38]
        inc    ecx
        mov    ecx, dword ptr [eax + 0xc]
loc_1_CC2C:
        mov    eax, edx
        inc    edx
        cmp    dword ptr [ecx + eax*4], -1
        jne    loc_1_CC2C
        mov    word ptr [esi + 0x34], dx
        cmp    dword ptr [esi + 0x44], 0
        mov    eax, 1
        je     loc_1_CC46
        sub    eax, eax
loc_1_CC46:
        mov    ecx, dword ptr [ecx + eax*4]
        cmp    ecx, -1
        je     loc_1_CCBB
        mov    word ptr [esi + 0x34], 1
        mov    dword ptr [esi + 0x40], 1
        mov    eax, dword ptr [esi + 0x2c]
        cmp    dword ptr [eax + 0x58], 0
        je     loc_1_CC6C
        mov    eax, dword ptr [eax + 0x6c]
        jmp    loc_1_CC6E
        db  08Dh, 049h, 000h
loc_1_CC6C:
        sub    eax, eax
loc_1_CC6E:
        push   0x20
        push   0x8a
        push   esi
        push   0xcacc                        ; -> sub_1_CACC
        push   eax
        push   ecx
        push   esi
        call   sub_1_115D4
        add    esp, 0x1c
        mov    edi, eax
        push   0
        push   dword ptr [esp + 0x14]
        push   1
        push   0xcb74                        ; -> loc_1_CB74
        push   esi
        push   0xcb74                        ; -> loc_1_CB74
        push   esi
        push   0x1f4
        call   sub_1_1094C
        add    esp, 0x20
        or     eax, eax
        je     loc_1_CCBB
        cmp    eax, 0xff
        je     loc_1_CCBB
        mov    dword ptr [esi + 0x30], 0
loc_1_CCBB:
        or     edi, edi
        je     loc_1_CCD7
        cmp    edi, 0xff
        je     loc_1_CCD7
        dec    dword ptr [esi + 0x40]
        push   0
        push   0
        push   esi
        call   sub_1_CCDC
        add    esp, 0xc
loc_1_CCD7:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
sub_1_CCDC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x38]
        or     eax, eax
        je     loc_1_CCF1
        push   eax
        call   sub_1_DEA4
        add    esp, 4
loc_1_CCF1:
        push   dword ptr [esi + 0x44]
        push   dword ptr [esi + 0x1c]
        call   sub_1_C904
        add    esp, 8
        push   esi
        call   sub_1_0024
        add    esp, 4
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_CD0C:
        sub    esp, 4
        mov    edx, dword ptr [esp + 8]
        cmp    dword ptr [edx + 8], 0x18273645
        jne    loc_1_CE64
        mov    dword ptr [esp], 0
        cmp    dword ptr [edx + 0x10], 0
        jne    loc_1_CD38
        mov    eax, 0xc0000001
        add    esp, 4
        ret    
        db  090h
loc_1_CD38:
        mov    ecx, dword ptr [esp + 0x10]
        or     ecx, ecx
        mov    eax, dword ptr [esp + 0xc]
        je     loc_1_CDD0
        cmp    eax, 6
        ja     loc_1_CE0C
        jmp    loc_1_CE70
loc_1_CD58:
        mov    dword ptr [edx + 0x48], ecx
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [edx + 0x4c], eax
        sub    eax, eax
        add    esp, 4
        ret    
loc_1_CD68:
        mov    dword ptr [edx + 0x58], ecx
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [edx + 0x5c], eax
        mov    dword ptr [edx + 0x60], ecx
        mov    dword ptr [edx + 0x64], eax
        sub    eax, eax
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_CD80:
        mov    dword ptr [edx + 0x50], ecx
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [edx + 0x54], eax
        sub    eax, eax
        add    esp, 4
        ret    
loc_1_CD90:
        mov    dword ptr [edx + 0x68], ecx
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [edx + 0x6c], eax
        sub    eax, eax
        add    esp, 4
        ret    
loc_1_CDA0:
        mov    dword ptr [edx + 0x70], ecx
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [edx + 0x74], eax
        sub    eax, eax
        add    esp, 4
        ret    
loc_1_CDB0:
        mov    dword ptr [edx + 0x78], ecx
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [edx + 0x7c], eax
        sub    eax, eax
        add    esp, 4
        ret    
loc_1_CDC0:
        mov    dword ptr [edx + 0x60], ecx
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [edx + 0x64], eax
        sub    eax, eax
        add    esp, 4
        ret    
loc_1_CDD0:
        cmp    eax, 6
        ja     loc_1_CE0C
        jmp    dword ptr [eax*4 + 0xce8c]    ; -> obj1:0xCE8C
loc_1_CDDC:
        sub    eax, eax
        mov    dword ptr [edx + 0x48], 0
        mov    dword ptr [edx + 0x4c], 0
        add    esp, 4
        ret    
loc_1_CDF0:
        mov    dword ptr [edx + 0x58], 0
        mov    dword ptr [edx + 0x5c], 0
loc_1_CDFE:
        mov    dword ptr [edx + 0x60], 0
        mov    dword ptr [edx + 0x64], 0
loc_1_CE0C:
        sub    eax, eax
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_CE14:
        sub    eax, eax
        mov    dword ptr [edx + 0x50], 0
        mov    dword ptr [edx + 0x54], 0
        add    esp, 4
        ret    
loc_1_CE28:
        sub    eax, eax
        mov    dword ptr [edx + 0x68], 0
        mov    dword ptr [edx + 0x6c], 0
        add    esp, 4
        ret    
loc_1_CE3C:
        sub    eax, eax
        mov    dword ptr [edx + 0x70], 0
        mov    dword ptr [edx + 0x74], 0
        add    esp, 4
        ret    
loc_1_CE50:
        sub    eax, eax
        mov    dword ptr [edx + 0x78], 0
        mov    dword ptr [edx + 0x7c], 0
        add    esp, 4
        ret    
loc_1_CE64:
        mov    eax, 0xc0000008
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_CE70:
        pop    eax
        int    0
        add    byte ptr [eax - 0x33], ch     ; -> loc_1_CD68
        add    byte ptr [eax], al
        ror    ch, 0
        add    byte ptr [eax - 0x6fffff33], al ; -> loc_1_CD80
        int    0
        add    byte ptr [eax - 0x4fffff33], ah ; -> loc_1_CDA0
        int    0
        add    ah, bl                        ; -> loc_1_CDDC
        int    0
        add    al, dh                        ; -> loc_1_CDF0
        int    0
        add    dh, bh                        ; -> loc_1_CDFE
        int    0
        add    byte ptr [esi + ecx*8], dl    ; -> loc_1_CE14
        add    byte ptr [eax], al
        sub    dh, cl
        add    byte ptr [eax], al
        cmp    al, 0xce
        add    byte ptr [eax], al
        push   eax
        into   
        add    byte ptr [eax], al
sub_1_CEA8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   esi
        push   edi
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 0x14]
        call   sub_1_FF70
        add    esp, 0xc
        or     eax, eax
        je     loc_1_CED5
        cmp    eax, 0xff
        jne    loc_1_D0F8
loc_1_CED5:
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_1_D0F8
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     loc_1_CEFA
        cmp    eax, 0xff
        jne    loc_1_D0FD
loc_1_CEFA:
        push   dword ptr [ebp - 4]
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        push   0
        push   0
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   dword ptr [ebp - 0x10]
        call   sub_1_10E60
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_CF38
        push   4
        push   dword ptr [ebp - 4]
        call   sub_1_D5C0
        add    esp, 8
        mov    eax, 1
        pop    edi
        pop    esi
        leave  
        ret    
        db  08Bh, 0FFh
loc_1_CF38:
        mov    eax, dword ptr [ebp - 0xc]
        and    word ptr [eax + 2], 0xfffe
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    byte ptr [eax + ecx - 3], 0x21
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x34], 2
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x38], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x2c], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0x1326c      ; -> loc_1_1326C
        mov    eax, dword ptr [0x13270]      ; -> loc_1_13270
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0x13270]      ; -> loc_1_13270
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0x13270], eax      ; -> loc_1_13270
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0xd298                        ; -> loc_1_D298
        push   esi
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 4]
        call   sub_1_C224
        add    esp, 0x1c
        mov    edi, eax
        or     edi, edi
        jne    loc_1_D0D4
        mov    ecx, dword ptr [ebp + 0x14]
        or     ecx, ecx
        je     loc_1_CFFF
        mov    eax, dword ptr [ebp - 0x10]
        cmp    byte ptr [eax], 0x2a
        jne    loc_1_CFFF
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_CFFF
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0xc], ecx
loc_1_CFFF:
        push   0
        push   dword ptr [ebp - 8]
        mov    ax, word ptr [0x132d0]        ; -> loc_1_132D0
        push   eax
        push   0xd390                        ; -> loc_1_D390
        push   dword ptr [ebp - 4]
        push   0xeca8                        ; -> loc_1_ECA8
        push   dword ptr [ebp - 4]
        push   dword ptr [0x132cc]           ; -> loc_1_132CC
        call   sub_1_1094C
        add    esp, 0x20
        mov    edi, eax
        or     edi, edi
        je     loc_1_D058
        cmp    edi, 0xff
        je     loc_1_D058
        push   dword ptr [ebp - 8]
        call   sub_1_DE94
        add    esp, 4
        push   7
        push   dword ptr [ebp - 4]
        call   sub_1_D5C0
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_D058:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x24], 0xfacf0000
        je     loc_1_D078
        test   byte ptr [eax + 0x20], 2
        je     loc_1_D078
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x2c]
        mov    ecx, dword ptr [eax + 0x58]
        jmp    loc_1_D07E
        db  08Dh, 049h, 000h
loc_1_D078:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0xc]
loc_1_D07E:
        mov    eax, 0
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_D08C
        mov    eax, dword ptr [esi + 0x68]
loc_1_D08C:
        push   0x10
        push   0x89
        push   dword ptr [ebp - 0xc]
        push   0x110a8                       ; -> loc_1_110A8
        push   eax
        push   ecx
        push   dword ptr [ebp - 4]
        call   sub_1_115D4
        add    esp, 0x1c
        mov    esi, eax
        push   dword ptr [ebp - 4]
        call   sub_1_F280
        add    esp, 4
        or     esi, esi
        je     loc_1_D0C8
        cmp    esi, 0xff
        je     loc_1_D0C8
        mov    eax, esi
        pop    edi
        pop    esi
        leave  
        ret    
        db  090h
loc_1_D0C8:
        pop    edi
        mov    eax, 0xff
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_D0D4:
        cmp    edi, 0xff
        je     loc_1_D0F0
        push   7
        push   dword ptr [ebp - 4]
        call   sub_1_D5C0
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        leave  
        ret    
        db  090h
loc_1_D0F0:
        mov    eax, edi
        pop    edi
        pop    esi
        leave  
        ret    
        db  08Bh, 0FFh
loc_1_D0F8:
        mov    eax, 0xc000000d
loc_1_D0FD:
        pop    edi
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
sub_1_D104:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    byte ptr [esp + 0x30], 0
        jne    loc_1_D14C
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x2c]
        push   dword ptr [eax + 0x14]
        call   sub_1_FF70
        add    esp, 0xc
        or     eax, eax
        je     loc_1_D136
        cmp    eax, 0xff
        jne    loc_1_D13D
loc_1_D136:
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_D14C
loc_1_D13D:
        pop    ebp
        mov    eax, 0xc000000d
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_D14C:
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     loc_1_D168
        cmp    eax, 0xff
        jne    loc_1_D213
loc_1_D168:
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0x2c]
        mov    dword ptr [eax + 0x10], esi
        mov    eax, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0x38], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0x28]
        mov    dword ptr [eax + 0x2c], edi
        lea    eax, [esp + 0x14]
        push   eax
        push   0
        push   0
        push   0xd21c                        ; -> loc_1_D21C
        push   edi
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x28]
        call   sub_1_C224
        add    esp, 0x1c
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_D1B6
        cmp    ebx, 0xff
        je     loc_1_D211
loc_1_D1B6:
        push   4
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, dword ptr [eax + 0x10]
        push   dword ptr [esp + 0x14]
        call   sub_1_D5C0
        add    esp, 8
        or     ebp, ebp
        je     loc_1_D1FC
        or     ebx, ebx
        jne    loc_1_D211
        push   dword ptr [edi + 0x58]
        push   esi
        push   dword ptr [esp + 0x1c]
        call   sub_1_D3DC
        add    esp, 0xc
        mov    ebx, eax
        push   dword ptr [esp + 0x14]
        call   sub_1_DE94
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_D1FC:
        or     ebx, ebx
        jne    loc_1_D20C
        push   dword ptr [esp + 0x14]
        call   sub_1_DE94
        add    esp, 4
loc_1_D20C:
        mov    ebx, 0xff
loc_1_D211:
        mov    eax, ebx
loc_1_D213:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_D21C:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 0x10]
        mov    dword ptr [esi + 0x10], 0
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_D278
        lea    eax, [esp + 8]
        push   eax
        push   esi
        call   sub_1_C4AC
        add    esp, 8
        or     eax, eax
        je     loc_1_D278
        mov    ecx, dword ptr [esi + 0x2c]
        push   dword ptr [ecx + 0x58]
        push   edi
        push   eax
        call   sub_1_D3DC
        add    esp, 0xc
        push   -1
        push   eax
        push   edi
        call   sub_1_25A0
        add    esp, 0xc
        push   4
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_D278:
        push   0
        push   0x15
        push   edi
        call   sub_1_25A0
        add    esp, 0xc
        push   4
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_D298:
        sub    esp, 8
        push   esi
        push   edi
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_1_D36C
        lea    eax, [esp + 0xc]
        push   eax
        mov    esi, dword ptr [esp + 0x18]
        push   esi
        call   sub_1_C4AC
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     loc_1_D370
        lea    eax, [esp + 8]
        push   eax
        inc    dword ptr [edi + 0x1c]
        mov    ax, word ptr [0x132d0]        ; -> loc_1_132D0
        push   eax
        push   0xd390                        ; -> loc_1_D390
        push   esi
        push   0xeca8                        ; -> loc_1_ECA8
        push   0
        push   esi
        push   dword ptr [0x132cc]           ; -> loc_1_132CC
        call   sub_1_109B8
        add    esp, 0x20
        or     eax, eax
        je     loc_1_D308
        cmp    eax, 0xff
        je     loc_1_D308
        push   edi
        call   sub_1_DEA4
        add    esp, 4
        jmp    loc_1_D370
        db  08Dh, 049h, 000h
loc_1_D308:
        mov    dword ptr [esi + 0x38], edi
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 0x30], eax
        cmp    dword ptr [edi + 0x24], 0xfacf0000
        je     loc_1_D32C
        test   byte ptr [edi + 0x20], 2
        je     loc_1_D32C
        mov    eax, dword ptr [esi + 0x2c]
        mov    ecx, dword ptr [eax + 0x58]
        jmp    loc_1_D32F
        db  08Dh, 049h, 000h
loc_1_D32C:
        mov    ecx, dword ptr [edi + 0xc]
loc_1_D32F:
        mov    eax, dword ptr [esi + 0x2c]
        cmp    dword ptr [eax + 0x58], 0
        je     loc_1_D340
        mov    eax, dword ptr [eax + 0x68]
        jmp    loc_1_D342
        db  08Dh, 049h, 000h
loc_1_D340:
        sub    eax, eax
loc_1_D342:
        push   0x10
        push   0x89
        push   dword ptr [esi + 0x1c]
        push   0x110a8                       ; -> loc_1_110A8
        push   eax
        push   ecx
        push   esi
        call   sub_1_115D4
        add    esp, 0x1c
        push   esi
        call   sub_1_F280
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 8
        ret    
        db  090h
loc_1_D36C:
        mov    esi, dword ptr [esp + 0x14]
loc_1_D370:
        push   7
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        push   0
        push   0x15
        push   dword ptr [esi + 0x10]
        call   sub_1_25A0
        add    esp, 0xc
        pop    edi
        pop    esi
        add    esp, 8
        ret    
loc_1_D390:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esi + 0x10]
        push   dword ptr [esi + 0x38]
        call   sub_1_DEA4
        add    esp, 4
        push   esi
        call   sub_1_F280
        add    esp, 4
        mov    eax, dword ptr [esp + 0x10]
        or     eax, eax
        je     loc_1_D3CC
        cmp    eax, 9
        je     loc_1_D3CC
        push   0
        push   0x15
        push   edi
        call   sub_1_25A0
        add    esp, 0xc
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_D3CC:
        push   -1
        push   eax
        push   edi
        call   sub_1_25A0
        add    esp, 0xc
        pop    edi
        pop    esi
        ret    
        db  090h
sub_1_D3DC:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x1c]
        test   byte ptr [ebx + 0x20], 8
        je     loc_1_D404
        sub    esi, esi
        mov    eax, dword ptr [ebx + 0xc]
        cmp    dword ptr [eax], -1
        je     loc_1_D409
loc_1_D3F8:
        inc    esi
        add    eax, 4
        cmp    dword ptr [eax], -1
        jne    loc_1_D3F8
        jmp    loc_1_D409
        db  090h
loc_1_D404:
        mov    esi, 1
loc_1_D409:
        mov    ecx, dword ptr [esp + 0x20]
        movzx  eax, word ptr [ecx + 8]
        mov    ecx, esi
        shl    ecx, 5
        lea    ecx, [ecx + esi + 4]
        mov    dword ptr [esp + 0x14], ecx
        cmp    ecx, eax
        jbe    loc_1_D447
        cmp    eax, 4
        mov    esi, 0
        jbe    loc_1_D43A
        mov    ecx, 0x21
        sub    eax, 4
        sub    edx, edx
        div    ecx
        mov    esi, eax
loc_1_D43A:
        mov    eax, esi
        shl    eax, 5
        lea    ecx, [eax + esi + 4]
        mov    dword ptr [esp + 0x14], ecx
loc_1_D447:
        cmp    dword ptr [esp + 0x14], 0xffff
        jbe    loc_1_D460
        mov    eax, 0xc0000001
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_D460:
        movzx  eax, word ptr [esp + 0x14]
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        jne    loc_1_D484
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_D484:
        sub    eax, eax
        mov    edx, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    edx, dword ptr [esp + 0x10]
        add    edx, 4
        mov    ecx, dword ptr [esp + 0x10]
        mov    word ptr [ecx], si
        mov    edi, dword ptr [ebx + 0x20]
        and    edi, 2
        cmp    edi, 1
        sbb    al, al
        and    al, 0x80
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [esp + 0x24]
        mov    ecx, eax
        and    ecx, 0xff00
        mov    ebp, eax
        shl    ebp, 0x10
        or     ecx, ebp
        shl    ecx, 8
        mov    ebp, eax
        and    ebp, 0xff0000
        shr    ebp, 8
        or     ecx, ebp
        shr    eax, 0x18
        and    eax, 0xff
        or     ecx, eax
        test   byte ptr [ebx + 0x20], 8
        je     loc_1_D538
        mov    eax, dword ptr [ebx + 0xc]
        or     esi, esi
        je     loc_1_D57E
loc_1_D4F8:
        mov    edi, dword ptr [eax]
        mov    ebx, edi
        and    ebx, 0xff00
        mov    ebp, edi
        shl    ebp, 0x10
        or     ebx, ebp
        shl    ebx, 8
        mov    ebp, edi
        and    ebp, 0xff0000
        shr    ebp, 8
        or     ebx, ebp
        shr    edi, 0x18
        and    edi, 0xff
        or     ebx, edi
        mov    dword ptr [edx + 0xb], ebx
        add    eax, 4
        mov    dword ptr [edx + 5], ecx
        add    edx, 0x21
        dec    esi
        jne    loc_1_D4F8
        jmp    loc_1_D57E
        db  08Dh, 049h, 000h
loc_1_D538:
        cmp    dword ptr [ebx + 0x24], 0xfacf0000
        je     loc_1_D54C
        or     edi, edi
        je     loc_1_D54C
        mov    dword ptr [edx + 0xb], ecx
        jmp    loc_1_D57B
        db  08Bh, 0FFh
loc_1_D54C:
        mov    esi, dword ptr [ebx + 0xc]
        mov    edi, esi
        and    edi, 0xff00
        mov    eax, esi
        shl    eax, 0x10
        or     edi, eax
        shl    edi, 8
        mov    eax, esi
        and    eax, 0xff0000
        shr    eax, 8
        or     edi, eax
        shr    esi, 0x18
        and    esi, 0xff
        or     edi, esi
        mov    dword ptr [edx + 0xb], edi
loc_1_D57B:
        mov    dword ptr [edx + 5], ecx
loc_1_D57E:
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x20]
        mov    edi, dword ptr [ecx + 4]
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x10]
        mov    ax, word ptr [esp + 0x18]
        mov    word ptr [ecx + 8], ax
        call   sub_1_0024
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
sub_1_D5C0:
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        test   al, 2
        mov    esi, dword ptr [esp + 8]
        je     loc_1_D5D7
        mov    edx, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
loc_1_D5D7:
        test   al, 1
        je     loc_1_D5EB
        mov    eax, dword ptr [esi + 0x1c]
        or     eax, eax
        je     loc_1_D5EB
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_D5EB:
        mov    ecx, dword ptr [esi + 0x4e]
        mov    dword ptr [esi + 0x1c], 0
        mov    eax, dword ptr [esi + 0x52]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [esi], 0x13264      ; -> loc_1_13264
        mov    eax, dword ptr [0x13268]      ; -> loc_1_13268
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [0x13268]      ; -> loc_1_13268
        mov    dword ptr [eax], esi
        mov    dword ptr [0x13268], esi      ; -> loc_1_13268
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_D61C:
        push   dword ptr [esp + 4]
        call   sub_1_DEE4
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_D62C:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x2c], 0
        cmp    dword ptr [esp + 0x38], 0
        je     loc_1_D694
        mov    eax, dword ptr [esp + 0x38]
        cmp    dword ptr [eax + 8], 0x54637281
        jne    loc_1_D684
        mov    dword ptr [esp + 0x30], 0
        cmp    dword ptr [esp + 0x38], 0
        je     loc_1_D694
        mov    ebp, 0x13274                  ; -> loc_1_13274
        mov    ebx, dword ptr [0x13274]      ; -> loc_1_13274
        push   0x13234                       ; -> loc_1_13234
        call   sub_1_10288
        add    esp, 4
        inc    eax
        mov    dword ptr [esp + 0x18], eax
        mov    ecx, 0x12
        jmp    loc_1_D6DB
        db  08Bh, 0FFh
loc_1_D684:
        pop    ebp
        mov    eax, 0xc0000008
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Dh, 049h, 000h
loc_1_D694:
        mov    edi, dword ptr [0x132b8]      ; -> loc_1_132B8
        mov    dword ptr [esp + 0x18], 0
        mov    dword ptr [esp + 0x24], edi
        mov    eax, edi
        mov    eax, dword ptr [eax]
        or     eax, eax
        jle    loc_1_D6CA
        inc    edx
        mov    edx, edi
        add    edx, 8
loc_1_D6B4:
        mov    ecx, dword ptr [edx]
        cmp    ecx, edx
        je     loc_1_D6C4
loc_1_D6BA:
        mov    ecx, dword ptr [ecx]
        inc    dword ptr [esp + 0x18]
        cmp    ecx, edx
        jne    loc_1_D6BA
loc_1_D6C4:
        add    edx, 8
        dec    eax
        jne    loc_1_D6B4
loc_1_D6CA:
        mov    ebp, dword ptr [0x132b8]      ; -> loc_1_132B8
        add    ebp, 8
        mov    ebx, dword ptr [ebp]
        mov    ecx, 0x1b
loc_1_D6DB:
        mov    eax, dword ptr [esp + 0x18]
        imul   eax, ecx
        lea    edx, [eax + 0x3c]
        mov    dword ptr [esp + 0x28], edx
        mov    eax, dword ptr [esp + 0x40]
        mov    eax, dword ptr [eax]
        cmp    edx, eax
        jle    loc_1_D725
        mov    dword ptr [esp + 0x2c], 1
        cmp    eax, 0x3c
        ja     loc_1_D70C
        mov    dword ptr [esp + 0x18], 0
        jmp    loc_1_D717
        db  08Bh, 0FFh
loc_1_D70C:
        sub    edx, edx
        sub    eax, 0x3c
        div    ecx
        mov    dword ptr [esp + 0x18], eax
loc_1_D717:
        mov    eax, dword ptr [esp + 0x18]
        imul   eax, ecx
        lea    edx, [eax + 0x3c]
        mov    dword ptr [esp + 0x28], edx
loc_1_D725:
        movzx  eax, word ptr [esp + 0x28]
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esp + 0x1c], eax
        or     eax, eax
        jne    loc_1_D748
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
loc_1_D748:
        sub    eax, eax
        mov    edx, dword ptr [esp + 0x28]
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        cmp    dword ptr [esp + 0x38], eax
        je     loc_1_D76C
        mov    esi, dword ptr [esp + 0x38]
        jmp    loc_1_D772
loc_1_D76C:
        mov    esi, dword ptr [0x1323c]      ; -> loc_1_1323C
loc_1_D772:
        add    esi, 0x8c
        mov    edi, dword ptr [esp + 0x1c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    edx, dword ptr [esp + 0x1c]
        mov    byte ptr [edx + 6], 3
        mov    byte ptr [edx + 8], 0xfe
        mov    eax, dword ptr [esp + 0x38]
        movzx  ax, byte ptr [eax + 0x4f]
        mov    word ptr [edx + 0x34], ax
        mov    word ptr [edx + 0x36], ax
        mov    word ptr [edx + 0x28], 0x40
        mov    word ptr [edx + 0x2a], 0x40
        mov    word ptr [edx + 0x2c], 0x40
        mov    word ptr [edx + 0x30], 0x200
        mov    word ptr [edx + 0x38], 0xffff
        mov    eax, edx
        add    eax, 0x3c
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [esp + 0x20], 0
        sub    ecx, ecx
        cmp    dword ptr [esp + 0x18], ecx
        je     loc_1_D937
        nop    
loc_1_D7D8:
        cmp    dword ptr [esp + 0x38], 0
        je     loc_1_D808
        cmp    ebp, ebx
        je     loc_1_D937
        mov    esi, dword ptr [ebx + 0x14]
        mov    ebx, dword ptr [ebx]
        cmp    byte ptr [esi + 0x34], 0x2a
        je     loc_1_D928
        test   byte ptr [esi + 0x20], 1
        je     loc_1_D899
        jmp    loc_1_D928
        db  08Dh, 049h, 000h
loc_1_D808:
        sub    al, al
        mov    byte ptr [esp + 0x13], 0
        mov    esi, ebx
        mov    edi, dword ptr [esp + 0x24]
        lea    edx, [edi + ecx*8 + 8]
        mov    edi, dword ptr [edi]
loc_1_D81C:
        cmp    ecx, edi
        je     loc_1_D830
        cmp    ebp, ebx
        jne    loc_1_D838
        add    edx, 8
        inc    ecx
        mov    ebp, edx
        mov    ebx, dword ptr [edx]
        mov    al, 1
        jmp    loc_1_D81C
loc_1_D830:
        mov    byte ptr [esp + 0x13], 1
        jmp    loc_1_D83E
        db  090h
loc_1_D838:
        or     al, al
        jne    loc_1_D83E
        mov    ebx, dword ptr [ebx]
loc_1_D83E:
        cmp    byte ptr [esp + 0x13], 0
        jne    loc_1_D937
        or     al, al
        jne    loc_1_D92C
        mov    edx, dword ptr [esi + 0x20]
        test   dh, 0x10
        jne    loc_1_D920
        test   dl, 0x10
        je     loc_1_D920
        test   dl, 8
        mov    eax, dword ptr [esi + 0xc]
        je     loc_1_D870
        mov    eax, dword ptr [eax]
loc_1_D870:
        test   dh, 8
        mov    edi, dword ptr [esp + 0x14]
        mov    dword ptr [edi + 0x13], eax
        mov    eax, 0xffffffff
        jne    loc_1_D892
        movzx  eax, word ptr [esi + 0x30]
        lea    eax, [eax*4 + 4]
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
loc_1_D892:
        mov    edx, dword ptr [esp + 0x14]
        mov    dword ptr [edx + 0x17], eax
loc_1_D899:
        mov    edx, dword ptr [esi + 0x20]
        mov    eax, edx
        and    eax, 2
        cmp    eax, 1
        sbb    al, al
        and    al, 0x80
        mov    edi, dword ptr [esp + 0x14]
        mov    byte ptr [edi + 0x11], al
        and    edx, 0xf0
        cmp    edx, 0x20
        je     loc_1_D8D4
        cmp    edx, 0x40
        je     loc_1_D8E0
        cmp    edx, 0x80
        je     loc_1_D8E8
        mov    eax, dword ptr [esp + 0x14]
        or     byte ptr [eax + 0x11], 4
        jmp    loc_1_D8E8
        db  08Dh, 049h, 000h
loc_1_D8D4:
        mov    eax, dword ptr [esp + 0x14]
        or     byte ptr [eax + 0x11], 5
        jmp    loc_1_D8E8
        db  08Bh, 0FFh
loc_1_D8E0:
        mov    eax, dword ptr [esp + 0x14]
        or     byte ptr [eax + 0x11], 6
loc_1_D8E8:
        add    esi, 0x34
        mov    eax, dword ptr [esp + 0x20]
        inc    al
        mov    edx, dword ptr [esp + 0x14]
        mov    byte ptr [edx + 0x10], al
        mov    edi, dword ptr [esp + 0x14]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        cmp    dword ptr [esp + 0x38], 0
        je     loc_1_D910
        add    dword ptr [esp + 0x14], 0x12
        jmp    loc_1_D915
        db  08Bh, 0FFh
loc_1_D910:
        add    dword ptr [esp + 0x14], 0x1b
loc_1_D915:
        dec    dword ptr [esp + 0x18]
        inc    dword ptr [esp + 0x20]
        jmp    loc_1_D92C
        db  090h
loc_1_D920:
        mov    edx, dword ptr [esp + 0x1c]
        dec    word ptr [edx + 0x3a]
loc_1_D928:
        dec    dword ptr [esp + 0x18]
loc_1_D92C:
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_1_D7D8
loc_1_D937:
        mov    edx, dword ptr [esp + 0x1c]
        cmp    dword ptr [esp + 0x2c], 1
        mov    ax, word ptr [esp + 0x20]
        mov    word ptr [edx + 0x3a], ax
        mov    eax, dword ptr [esp + 0x3c]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 0x40]
        mov    edx, dword ptr [esp + 0x28]
        mov    dword ptr [eax], edx
        cmc    
        sbb    eax, eax
        and    eax, 9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  090h
sub_1_D968:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x2c], 0
        sub    ebx, ebx
        mov    ecx, dword ptr [0x13274]      ; -> loc_1_13274
        cmp    ecx, 0x13274                  ; -> loc_1_13274
        je     loc_1_D9C5
        nop    
loc_1_D988:
        lea    esi, [ecx + 0xc]
        mov    edx, dword ptr [esi]
        cmp    esi, edx
        je     loc_1_D9BB
loc_1_D991:
        mov    edi, edx
        mov    edx, dword ptr [edx]
        lea    ebp, [edi + 0x1c]
        mov    eax, dword ptr [ebp]
        cmp    ebp, eax
        je     loc_1_D9A7
loc_1_D9A0:
        mov    eax, dword ptr [eax]
        inc    ebx
        cmp    ebp, eax
        jne    loc_1_D9A0
loc_1_D9A7:
        add    edi, 0x2c
        mov    eax, dword ptr [edi]
        cmp    edi, eax
        je     loc_1_D9B7
loc_1_D9B0:
        mov    eax, dword ptr [eax]
        inc    ebx
        cmp    edi, eax
        jne    loc_1_D9B0
loc_1_D9B7:
        cmp    esi, edx
        jne    loc_1_D991
loc_1_D9BB:
        mov    ecx, dword ptr [ecx]
        cmp    ecx, 0x13274                  ; -> loc_1_13274
        jne    loc_1_D988
loc_1_D9C5:
        lea    eax, [ebx*4]
        lea    eax, [eax + eax*2]
        lea    ecx, [ebx + eax*4 + 0x35]
        mov    dword ptr [esp + 0x24], ecx
        mov    eax, dword ptr [esp + 0x3c]
        mov    eax, dword ptr [eax]
        cmp    eax, ecx
        jge    loc_1_DA13
        mov    dword ptr [esp + 0x2c], 1
        cmp    eax, 0x35
        mov    ebx, 0
        jbe    loc_1_DA01
        mov    ecx, 0x31
        sub    eax, 0x35
        sub    edx, edx
        div    ecx
        mov    ebx, eax
loc_1_DA01:
        lea    eax, [ebx*4]
        lea    eax, [eax + eax*2]
        lea    ecx, [ebx + eax*4 + 0x35]
        mov    dword ptr [esp + 0x24], ecx
loc_1_DA13:
        push   dword ptr [esp + 0x24]
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esp + 0x28], eax
        or     eax, eax
        jne    loc_1_DA34
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_DA34:
        sub    eax, eax
        mov    edx, dword ptr [esp + 0x24]
        mov    edi, dword ptr [esp + 0x28]
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    ecx, dword ptr [esp + 0x28]
        mov    dword ptr [ecx], ebx
        mov    edx, ecx
        add    edx, 4
        mov    eax, dword ptr [0x13274]      ; -> loc_1_13274
        mov    dword ptr [esp + 0x20], eax
        sub    ecx, ecx
loc_1_DA62:
        cmp    dword ptr [esp + 0x20], 0x13274 ; -> loc_1_13274
        je     loc_1_DB48
        mov    esi, dword ptr [esp + 0x20]
        mov    eax, esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 0x20], eax
        lea    eax, [esi + 0xc]
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 0x14], eax
loc_1_DA89:
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [esp + 0x1c], eax
        je     loc_1_DB40
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esp + 0x18]
        add    eax, 0x1c
        mov    dword ptr [esp + 0x10], eax
        mov    ebp, dword ptr [eax]
loc_1_DAAE:
        cmp    dword ptr [esp + 0x10], ebp
        je     loc_1_DB00
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x10]
        mov    esi, dword ptr [eax + 0x14]
        lea    edi, [edx + 8]
        add    esi, 0x34
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    esi, dword ptr [ebp + 0xc]
        or     esi, esi
        je     loc_1_DAF0
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [edx + 4], eax
        mov    al, byte ptr [esi + 0x45]
        mov    byte ptr [edx + 0x30], al
        mov    eax, dword ptr [esi + 0x20]
        mov    dword ptr [edx + 0x28], eax
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [edx + 0x2c], eax
        lea    esi, [ebp + 0x28]
        lea    edi, [edx + 0x18]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
loc_1_DAF0:
        inc    ecx
        mov    eax, dword ptr [ebp + 0x68]
        mov    dword ptr [edx], eax
        add    edx, 0x31
        mov    ebp, dword ptr [ebp]
        cmp    ecx, ebx
        jne    loc_1_DAAE
loc_1_DB00:
        cmp    ecx, ebx
        je     loc_1_DB48
        mov    ebp, dword ptr [esp + 0x18]
        add    ebp, 0x2c
        mov    eax, dword ptr [ebp]
loc_1_DB0E:
        cmp    ebp, eax
        je     loc_1_DB38
        mov    esi, dword ptr [eax + 0x10]
        mov    esi, dword ptr [esi + 0x10]
        mov    esi, dword ptr [esi + 0x10]
        mov    esi, dword ptr [esi + 0x14]
        lea    edi, [edx + 8]
        add    esi, 0x34
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [edx], 0x14
        inc    ecx
        add    edx, 0x31
        mov    eax, dword ptr [eax]
        cmp    ecx, ebx
        jne    loc_1_DB0E
loc_1_DB38:
        cmp    ecx, ebx
        jne    loc_1_DA89
loc_1_DB40:
        cmp    ecx, ebx
        jne    loc_1_DA62
loc_1_DB48:
        mov    eax, dword ptr [esp + 0x38]
        mov    ecx, dword ptr [esp + 0x28]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 0x3c]
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [eax], ecx
        cmp    dword ptr [esp + 0x2c], 1
        cmc    
        sbb    eax, eax
        and    eax, 9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  090h
sub_1_DB70:
        sub    esp, 0x44
        push   esi
        sub    esi, esi
        movzx  eax, word ptr [0x1331c]       ; -> loc_1_1331C
        inc    eax
        cmp    eax, esi
        jle    loc_1_DBA0
loc_1_DB82:
        lea    eax, [esp + 4]
        push   eax
        push   0
        push   0
        call   sub_1_F7B8
        add    esp, 0xc
        inc    esi
        movzx  eax, word ptr [0x1331c]       ; -> loc_1_1331C
        inc    eax
        cmp    eax, esi
        jg     loc_1_DB82
loc_1_DBA0:
        call   sub_1_125E4
        push   0
        push   1
        push   0
        push   dword ptr [0x13327]           ; -> obj1:0x13327
        call   sub_1_12638
        add    esp, 0x10
        cmp    eax, -1
        jne    loc_1_DBC8
        mov    eax, 0xc0000001
        pop    esi
        add    esp, 0x44
        ret    
loc_1_DBC8:
        sub    eax, eax
        pop    esi
        add    esp, 0x44
        ret    
        db  090h
sub_1_DBD0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x1c]
        cmp    dword ptr [ebx], 0x94
        jae    loc_1_DBEC
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_DBEC:
        push   0x94
        call   sub_1_001E
        add    esp, 4
        mov    edx, eax
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0x25
        rep stosd dword ptr es:[edi], eax
        mov    esi, 0x133f8                  ; -> loc_1_133F8
        mov    edi, edx
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ebp, dword ptr [0x13408]      ; -> loc_1_13408
        lea    ecx, [edx + 0x84]
        mov    eax, 8
loc_1_DC25:
        mov    esi, ebp
        shl    esi, 4
        add    esi, 0x1340c                  ; -> loc_1_1340C
        inc    ebp
        mov    edi, ecx
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        sub    ecx, 0x10
        cmp    ebp, 8
        jl     loc_1_DC41
        sub    ebp, ebp
loc_1_DC41:
        dec    eax
        jne    loc_1_DC25
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax], edx
        mov    dword ptr [ebx], 0x94
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_DC58:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x20]
        or     ebx, ebx
        mov    esi, dword ptr [esp + 0x1c]
        jne    loc_1_DCAC
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_DCA2
        push   esi
        call   sub_1_FCF4
        add    esp, 4
        lea    eax, [esp + 0x10]
        push   eax
        push   esi
        call   sub_1_1030C
        add    esp, 8
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_1_10374
        add    esp, 4
        mov    word ptr [0x13308], ax        ; -> loc_1_13308
        mov    dword ptr [esi + 0x58], 0
loc_1_DCA2:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_DCAC:
        push   esi
        call   sub_1_4E80
        add    esp, 4
        mov    edi, ebx
        and    edi, 0xff00
        mov    eax, ebx
        shl    eax, 0x10
        or     edi, eax
        shl    edi, 8
        mov    eax, ebx
        and    eax, 0xff0000
        shr    eax, 8
        or     edi, eax
        shr    ebx, 0x18
        and    ebx, 0xff
        or     edi, ebx
        mov    ecx, dword ptr [esp + 0x24]
        mov    ebx, ecx
        and    ebx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     ebx, eax
        shl    ebx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     ebx, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     ebx, ecx
        mov    eax, dword ptr [esi + 0x88]
        shr    eax, 1
        or     eax, eax
        je     loc_1_DD1E
        nop    
loc_1_DD18:
        shr    eax, 1
        or     eax, eax
        jne    loc_1_DD18
loc_1_DD1E:
        push   esi
        call   sub_1_102C4
        add    esp, 4
        lea    ebp, [eax + 2]
        push   esi
        push   ebx
        push   edi
        call   sub_3_0380
        add    esp, 0xc
        or     eax, eax
        je     loc_1_DD40
        cmp    eax, 0xff
        jne    loc_1_DD50
loc_1_DD40:
        push   esi
        push   ebp
        push   0
        add    esi, 0x1c
        push   esi
        call   sub_1_104EC
        add    esp, 0x10
loc_1_DD50:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_DD58:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esi + 0x84]
        dec    eax
        mov    dword ptr [esi + 0x84], eax
        je     loc_1_DD78
        mov    eax, 0xff
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_DD78:
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [esi + 0x10]
        mov    edi, dword ptr [esi + 0xc]
        mov    ebx, dword ptr [ecx + 0x14]
        mov    eax, dword ptr [esi + 0x80]
        mov    edx, dword ptr [ebx + 0x28]
        mov    eax, dword ptr [eax + 0x88]
        not    eax
        test   eax, edx
        je     loc_1_DDB1
        cmp    byte ptr [ecx + 0x23], 0
        jne    loc_1_DDB1
        cmp    byte ptr [ebx + 0x43], 0x1d
        jne    loc_1_DDB1
        and    eax, edx
        mov    dword ptr [ebx + 0x28], eax
loc_1_DDB1:
        or     ecx, ecx
        je     loc_1_DDBE
        push   ecx
        call   sub_1_DDE4
        add    esp, 4
loc_1_DDBE:
        push   esi
        call   sub_1_0024
        add    esp, 4
        or     edi, edi
        je     loc_1_DDD8
        push   0
        push   0
        push   edi
        call   sub_1_25A0
        add    esp, 0xc
loc_1_DDD8:
        pop    edi
        mov    eax, 0xff
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_DDE4:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esi + 0x18]
        dec    eax
        mov    dword ptr [esi + 0x18], eax
        je     loc_1_DE00
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_DE00:
        mov    ecx, dword ptr [esi + 0x14]
        mov    edi, dword ptr [ecx + 0x20]
        mov    eax, edi
        and    eax, 0xffffff0f
        mov    dword ptr [ecx + 0x20], eax
        or     eax, 0x40
        mov    dword ptr [ecx + 0x20], eax
        mov    eax, dword ptr [ecx + 0x14]
        or     eax, eax
        je     loc_1_DE2D
        lea    ecx, [esp + 8]
        push   ecx
        push   0
        push   eax
        call   sub_1_10AEC
        add    esp, 0xc
loc_1_DE2D:
        mov    ecx, dword ptr [esi + 0x14]
        test   edi, 0x41
        jne    loc_1_DE74
        cmp    byte ptr [ecx + 0x34], 0x2a
        je     loc_1_DE74
        movzx  eax, word ptr [0x13364]       ; -> loc_1_13364
        push   eax
        push   0xf224                        ; -> sub_1_F224
        push   esi
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   ecx
        call   sub_1_EE34
        add    esp, 0x14
        or     eax, eax
        je     loc_1_DE66
        cmp    eax, 0xff
        jne    loc_1_DE74
loc_1_DE66:
        pop    edi
        mov    eax, 0xff
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_DE74:
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [eax + 0x28], 0
        push   0
        push   esi
        call   sub_1_F224
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_DE94:
        push   dword ptr [esp + 4]
        call   sub_1_DEA4
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_DEA4:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x1c]
        dec    eax
        mov    dword ptr [esi + 0x1c], eax
        jne    loc_1_DEDF
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        cmp    dword ptr [esi + 0x24], 0xfacf0000
        jne    loc_1_DED6
        test   byte ptr [esi + 0x20], 8
        je     loc_1_DED6
        push   dword ptr [esi + 0xc]
        call   sub_1_0024
        add    esp, 4
loc_1_DED6:
        push   esi
        call   sub_1_0024
        add    esp, 4
loc_1_DEDF:
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_DEE4:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 0x64]
        dec    eax
        mov    dword ptr [ecx + 0x64], eax
        or     eax, eax
        jle    loc_1_DEFC
        mov    eax, 0xff
        pop    esi
        ret    
        db  090h
loc_1_DEFC:
        push   ecx
        mov    esi, dword ptr [ecx + 0x58]
        call   sub_1_FAF0
        add    esp, 4
        or     esi, esi
        je     loc_1_DF19
        push   0
        push   0
        push   esi
        call   sub_1_25A0
        add    esp, 0xc
loc_1_DF19:
        pop    esi
        mov    eax, 0xff
        ret    
sub_1_DF20:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x3c]
        dec    eax
        mov    dword ptr [ecx + 0x3c], eax
        jne    loc_1_DF49
        mov    dword ptr [ecx + 0x3c], 0x3e8
        push   dword ptr [ecx + 0x18]
        push   0xdf68                        ; -> loc_1_DF68
        push   0
        push   ecx
        push   0
        call   sub_1_3C00
        add    esp, 0x14
loc_1_DF49:
        ret    
        db  08Bh, 0FFh
sub_1_DF4C:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        push   eax
        call   sub_1_DF20
        add    esp, 4
        sub    eax, eax
        ret    
        db  08Bh, 0FFh
loc_1_DF68:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esi + 0xc]
        push   edi
        call   sub_1_37F0
        add    esp, 4
        cmp    dword ptr [edi + 0x14], 0
        je     loc_1_DF89
        push   edi
        call   sub_1_3820
        add    esp, 4
loc_1_DF89:
        push   edi
        call   sub_1_0024
        add    esp, 4
        push   esi
        call   sub_1_0024
        add    esp, 4
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_DFA0:
        sub    esp, 4
        cmp    dword ptr [esp + 8], 0
        mov    eax, dword ptr [0x12f80]      ; -> obj1:0x12F80
        jne    loc_1_DFB8
        mov    eax, dword ptr [eax + 0x80]
        jmp    loc_1_DFBE
        db  090h
loc_1_DFB8:
        mov    eax, dword ptr [eax + 0x84]
loc_1_DFBE:
        lea    ecx, [esp]
        push   ecx
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x14]
        push   eax
        call   sub_1_6954
        add    esp, 0x10
        or     eax, eax
        je     loc_1_DFE8
        cmp    eax, 0xff
        je     loc_1_DFE8
        sub    eax, eax
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_DFE8:
        mov    eax, dword ptr [esp]
        mov    ecx, dword ptr [eax + 0x20]
        mov    eax, dword ptr [esp + 0x14]
        mov    word ptr [eax], cx
        mov    eax, dword ptr [esp]
        test   byte ptr [eax + 0x20], 0x10
        jne    loc_1_E008
        mov    dword ptr [esp], 0
loc_1_E008:
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret    
sub_1_E010:
        push   esi
        push   edi
        push   0x44
        call   sub_1_001E
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        je     loc_1_E080
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0x11
        rep stosd dword ptr es:[edi], eax
        lea    edi, [edx + 0x34]
        mov    esi, dword ptr [esp + 0xc]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [edx + 0x20], 0x80
        mov    dword ptr [edx + 0x1c], 1
        mov    dword ptr [edx + 0x24], 0xfacf0000
        mov    ax, word ptr [0x1331c]        ; -> loc_1_1331C
        mov    word ptr [edx + 0x30], ax
        mov    dword ptr [edx], 0x1327c      ; -> loc_1_1327C
        mov    eax, dword ptr [0x13280]      ; -> loc_1_13280
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [0x13280]      ; -> loc_1_13280
        mov    dword ptr [eax], edx
        mov    dword ptr [0x13280], edx      ; -> loc_1_13280
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], edx
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_E080:
        pop    edi
        mov    eax, 1
        pop    esi
        ret    
sub_1_E088:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     loc_1_E0AB
        cmp    eax, 0xff
        jne    loc_1_E2FF
loc_1_E0AB:
        mov    esi, dword ptr [esp + 0x40]
        or     esi, esi
        je     loc_1_E0B9
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi], eax
loc_1_E0B9:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x1c], 0
        mov    eax, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0x3c]
        mov    dword ptr [eax + 0x2c], edi
        mov    dword ptr [esp + 0x14], 0
        mov    eax, dword ptr [esp + 0x38]
        or     eax, eax
        jne    loc_1_E130
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x20]
        call   sub_1_E010
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_E118
        cmp    ebx, 0xff
        je     loc_1_E118
        push   4
        push   dword ptr [esp + 0x14]
        call   sub_1_D5C0
        add    esp, 8
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_E118:
        mov    ecx, dword ptr [esp + 0x14]
        cmp    word ptr [esp + 0x28], 1
        sbb    eax, eax
        and    eax, 0xfffffffe
        add    eax, 4
        mov    dword ptr [ecx + 0x20], eax
        jmp    loc_1_E13B
        db  090h
loc_1_E130:
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [eax + 0x1c], 1
loc_1_E13B:
        mov    eax, dword ptr [esp + 0x14]
        and    dword ptr [eax + 0x20], 0xffffff0f
        mov    eax, dword ptr [esp + 0x14]
        or     dword ptr [eax + 0x20], 0x80
        mov    eax, dword ptr [0x132e8]      ; -> loc_1_132E8
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [ecx + 0x18], eax
        mov    edx, dword ptr [esp + 0x34]
        and    edx, 0x10
        mov    eax, dword ptr [esp + 0x14]
        je     loc_1_E170
        mov    byte ptr [eax + 0x32], 1
        jmp    loc_1_E174
loc_1_E170:
        mov    byte ptr [eax + 0x32], 0
loc_1_E174:
        or     edx, edx
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0x38], eax
        mov    eax, dword ptr [0x12f80]      ; -> obj1:0x12F80
        mov    bx, word ptr [eax + 0x9c]
        mov    ecx, dword ptr [eax + 0x98]
        je     loc_1_E1B4
        mov    edx, 0x10720                  ; -> loc_1_10720
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax + 0x4c], 2
        mov    dword ptr [esp + 0x2c], 0
        jmp    loc_1_E24E
        db  08Dh, 049h, 000h
loc_1_E1B4:
        mov    edx, 0xe308                   ; -> loc_1_E308
        mov    ebp, dword ptr [esp + 0x10]
        test   byte ptr [esp + 0x34], 5
        mov    word ptr [ebp + 0x4c], 2
        jne    loc_1_E20C
        test   byte ptr [esp + 0x34], 8
        je     loc_1_E1E3
        cmp    dword ptr [edi + 0x74], 0x7f000000
        je     loc_1_E20C
        cmp    byte ptr [edi + 0x95], 0
        jne    loc_1_E20C
loc_1_E1E3:
        cmp    dword ptr [edi + 0x74], 0x7f000000
        je     loc_1_E1F5
        cmp    byte ptr [edi + 0x95], 0
        je     loc_1_E223
loc_1_E1F5:
        mov    ecx, 0xa
        mov    bx, 1
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax + 0x4c], 4
        jmp    loc_1_E223
        db  08Bh, 0FFh
loc_1_E20C:
        mov    ecx, dword ptr [eax + 0x94]
        mov    bx, word ptr [eax + 0x9e]
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax + 0x4c], 1
loc_1_E223:
        cmp    dword ptr [edi + 0x58], 0x7f000000
        jne    loc_1_E24E
        mov    bx, 1
        mov    ecx, 0xa
        mov    eax, dword ptr [esp + 0x10]
        test   byte ptr [esp + 0x34], 6
        mov    word ptr [eax + 0x4c], bx
        je     loc_1_E24E
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax + 0x4c], 4
loc_1_E24E:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x34], 2
        mov    edi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [edi + 0x10], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x14], 0
        push   0
        push   ecx
        movzx  eax, bx
        push   eax
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x3c]
        push   edx
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x34]
        call   sub_1_10C0C
        add    esp, 0x24
        mov    edi, eax
        push   dword ptr [esp + 0x10]
        call   sub_1_F280
        add    esp, 4
        or     edi, edi
        je     loc_1_E2F8
        cmp    edi, 0xff
        je     loc_1_E2F8
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x14], 0
        cmp    edi, 1
        je     loc_1_E2CC
        mov    eax, dword ptr [esp + 0x10]
        push   dword ptr [eax + 0x1c]
        call   sub_1_0024
        add    esp, 4
loc_1_E2CC:
        or     esi, esi
        je     loc_1_E2D6
        mov    dword ptr [esi], 0
loc_1_E2D6:
        push   dword ptr [esp + 0x10]
        call   sub_1_F29C
        add    esp, 4
        push   dword ptr [esp + 0x14]
        call   sub_1_DEA4
        add    esp, 4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_E2F8:
        mov    edi, 0xff
        mov    eax, edi
loc_1_E2FF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_E308:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x20]
        or     esi, esi
        je     loc_1_E5D8
        mov    ax, word ptr [esi + 0x3c]
        test   al, 0x10
        je     loc_1_E354
        and    ax, 0xffef
        mov    word ptr [esi + 0x3c], ax
        or     ax, 1
        mov    word ptr [esi + 0x3c], ax
        mov    edi, dword ptr [esp + 0x18]
        mov    eax, dword ptr [edi + 0x2c]
        cmp    dword ptr [eax + 0x68], 0
        jne    loc_1_E5E4
        mov    dword ptr [esi + 0x38], 0xa
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_E354:
        mov    ebp, dword ptr [esi + 0x2c]
        test   byte ptr [ebp + 0x4c], 0x40
        mov    ebx, dword ptr [esi + 0x30]
        je     loc_1_E3DC
        mov    edi, dword ptr [esp + 0x18]
        mov    edx, dword ptr [edi + 0x38]
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edx + 4], edx
        mov    eax, dword ptr [edi + 0x38]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [eax + 0x14], 0
        mov    dword ptr [esi + 0x30], 0
        mov    eax, dword ptr [edi + 0x38]
        and    dword ptr [eax + 0x20], 0xffffff0f
        mov    eax, dword ptr [edi + 0x38]
        or     dword ptr [eax + 0x20], 0x20
        mov    eax, dword ptr [edi + 0x38]
        mov    dword ptr [eax + 0x14], 0
        push   dword ptr [edi + 0x38]
        call   sub_1_DEA4
        add    esp, 4
        mov    dword ptr [edi + 0x38], 0
        push   0xc0000120
        push   ebp
        push   ebx
        call   sub_1_E64C
        add    esp, 0xc
        push   edi
        call   sub_1_F280
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_E3DC:
        or     ebx, ebx
        je     loc_1_E5E4
        mov    ax, word ptr [esi + 0x3e]
        dec    ax
        mov    word ptr [esi + 0x3e], ax
        jne    loc_1_E59C
        mov    ax, word ptr [0x132d0]        ; -> loc_1_132D0
        mov    word ptr [esi + 0x3e], ax
        mov    edi, dword ptr [esp + 0x18]
        mov    cx, word ptr [edi + 0x4c]
        mov    dx, cx
        and    dx, 0xfff8
        mov    word ptr [edi + 0x4c], dx
        test   cl, 1
        je     loc_1_E44C
        test   byte ptr [0x13364], 4         ; -> loc_1_13364
        je     loc_1_E44C
        mov    eax, dword ptr [edi + 0x2c]
        cmp    dword ptr [eax + 0x74], 0x7f000000
        je     loc_1_E44C
        cmp    byte ptr [eax + 0x95], 0
        jne    loc_1_E44C
        or     dx, 2
        mov    word ptr [edi + 0x4c], dx
        mov    eax, dword ptr [0x132cc]      ; -> loc_1_132CC
        mov    dword ptr [esi + 0x38], eax
        jmp    loc_1_E5A0
        db  08Bh, 0FFh
loc_1_E44C:
        test   cl, 2
        je     loc_1_E490
        test   byte ptr [0x13364], 1         ; -> loc_1_13364
        jne    loc_1_E490
        or     dx, 4
        mov    word ptr [edi + 0x4c], dx
        mov    eax, dword ptr [edi + 0x2c]
        cmp    dword ptr [eax + 0x78], 0x7f000000
        je     loc_1_E47B
        cmp    byte ptr [eax + 0x95], 0
        je     loc_1_E5A0
loc_1_E47B:
        mov    dword ptr [esi + 0x38], 0xa
        mov    word ptr [esi + 0x3e], 1
        jmp    loc_1_E5A0
        db  08Dh, 049h, 000h
loc_1_E490:
        test   cl, 4
        je     loc_1_E4D4
        test   byte ptr [0x13364], 8         ; -> loc_1_13364
        je     loc_1_E4D4
        or     dx, 1
        mov    word ptr [edi + 0x4c], dx
        mov    eax, dword ptr [0x132c8]      ; -> loc_1_132C8
        mov    dword ptr [esi + 0x38], eax
        mov    ax, word ptr [0x132d2]        ; -> loc_1_132D2
        mov    word ptr [esi + 0x3e], ax
        test   ch, 2
        jne    loc_1_E5A0
        push   dword ptr [edi + 0x2c]
        call   sub_1_E5EC
        add    esp, 4
        jmp    loc_1_E5A0
        db  08Dh, 049h, 000h
loc_1_E4D4:
        mov    eax, 0xc0000001
        mov    ecx, dword ptr [edi + 0x38]
        mov    ebp, dword ptr [ecx]
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [edx], ebp
        mov    dword ptr [ebp + 4], edx
        mov    dword ptr [ecx + 4], ecx
        mov    ecx, dword ptr [edi + 0x38]
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [esi + 0x34]
        mov    dword ptr [ecx + 0x14], 0
        mov    dword ptr [esi + 0x30], 0
        cmp    byte ptr [0x13354], 0         ; -> loc_1_13354
        jne    loc_1_E514
        cmp    byte ptr [0x13353], 0         ; -> obj1:0x13353
        je     loc_1_E53A
loc_1_E514:
        mov    ecx, dword ptr [edi + 0x38]
        cmp    byte ptr [ecx + 0x32], 0
        jne    loc_1_E53A
        or     ebx, ebx
        je     loc_1_E53A
        push   dword ptr [esp + 0x10]
        push   dword ptr [edi + 0x2c]
        push   0x12b54                       ; -> loc_1_12B54
        push   ebx
        push   dword ptr [esi + 0x2c]
        push   edi
        call   sub_1_12A74
        add    esp, 0x18
loc_1_E53A:
        or     eax, eax
        je     loc_1_E5E4
        cmp    eax, 0xff
        je     loc_1_E5E4
        mov    esi, dword ptr [esi + 0x2c]
        mov    eax, dword ptr [edi + 0x38]
        and    dword ptr [eax + 0x20], 0xffffff0f
        mov    eax, dword ptr [edi + 0x38]
        or     dword ptr [eax + 0x20], 0x20
        mov    eax, dword ptr [edi + 0x38]
        mov    dword ptr [eax + 0x14], 0
        push   dword ptr [edi + 0x38]
        call   sub_1_DEA4
        add    esp, 4
        mov    dword ptr [edi + 0x38], 0
        push   0x15
        push   esi
        push   ebx
        call   sub_1_E64C
        add    esp, 0xc
        push   edi
        call   sub_1_F280
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_E59C:
        mov    edi, dword ptr [esp + 0x18]
loc_1_E5A0:
        inc    dword ptr [edi + 0x34]
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   edi
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   dword ptr [edi + 0x38]
        call   sub_1_10C0C
        add    esp, 0x24
        push   edi
        call   sub_1_F280
        add    esp, 4
        or     word ptr [esi + 0x3c], 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_E5D8:
        push   dword ptr [esp + 0x18]
        call   sub_1_F29C
        add    esp, 4
loc_1_E5E4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_E5EC:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        cmp    byte ptr [esi + 0x95], 0
        jne    loc_1_E639
        test   byte ptr [0x13364], 1         ; -> loc_1_13364
        jne    loc_1_E639
        lea    eax, [esp + 4]
        push   eax
        push   1
        push   0
        push   0
        push   0xe640                        ; -> loc_1_E640
        push   0
        push   esi
        push   dword ptr [0x132ec]           ; -> loc_1_132EC
        call   sub_1_109B8
        add    esp, 0x20
        or     eax, eax
        je     loc_1_E632
        cmp    eax, 0xff
        jne    loc_1_E639
loc_1_E632:
        mov    byte ptr [esi + 0x95], 1
loc_1_E639:
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_E640:
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [eax + 0x95], 0
        ret    
sub_1_E64C:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    ebx, [esp + 0x18]
        mov    ebp, ebx
        mov    ecx, dword ptr [esp + 0x28]
        or     ecx, ecx
        je     loc_1_E694
        mov    edi, dword ptr [ecx + 0x2c]
        mov    eax, dword ptr [ecx + 8]
        mov    edx, eax
        sub    edx, ecx
        cmp    edx, 8
        je     loc_1_E698
        mov    dword ptr [esp + 0x18], eax
        mov    eax, ebx
        mov    edx, dword ptr [esp + 0x18]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, ebx
        mov    edx, dword ptr [esp + 0x1c]
        mov    dword ptr [edx], eax
        mov    ebp, dword ptr [esp + 0x18]
        jmp    loc_1_E698
        db  08Bh, 0FFh
loc_1_E694:
        mov    edi, dword ptr [esp + 0x10]
loc_1_E698:
        mov    esi, dword ptr [esp + 0x2c]
        push   esi
        push   ecx
        call   dword ptr [esp + 0x2c]
        add    esp, 8
        cmp    ebp, ebx
        je     loc_1_E711
loc_1_E6A9:
        lea    ecx, [ebp - 8]
        mov    ebp, dword ptr [ebp]
        cmp    dword ptr [ecx + 0x2c], edi
        je     loc_1_E6B8
        or     esi, esi
        jne    loc_1_E6CC
loc_1_E6B8:
        push   esi
        push   ecx
        call   dword ptr [ecx + 0x48]
        add    esp, 8
        cmp    ebp, ebx
        jne    loc_1_E6A9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_E6CC:
        lea    edx, [ecx + 8]
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx + 0xc], eax
        mov    dword ptr [eax], edx
        lea    eax, [esp + 0x18]
        cmp    dword ptr [edx], eax
        jne    loc_1_E6E2
        mov    dword ptr [edx], edx
loc_1_E6E2:
        lea    eax, [esp + 0x14]
        push   eax
        push   0
        push   dword ptr [ecx + 0x2c]
        push   0
        mov    ax, word ptr [0x13364]        ; -> loc_1_13364
        and    eax, 0xf
        push   eax
        push   dword ptr [ecx + 0x48]
        push   ecx
        push   0
        push   0
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   dword ptr [ecx + 0x38]
        call   sub_1_E088
        add    esp, 0x2c
loc_1_E711:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
sub_1_E71C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x14], 0
        mov    bx, word ptr [esp + 0x34]
        or     bx, bx
        je     loc_1_E73F
        cmp    bx, 2
        mov    di, 1
        jne    loc_1_E742
loc_1_E73F:
        sub    di, di
loc_1_E742:
        mov    esi, dword ptr [esp + 0x20]
        or     esi, esi
        lea    eax, [esp + 0x10]
        push   eax
        je     loc_1_E780
        push   0
        movzx  eax, di
        push   eax
        push   esi
        push   dword ptr [esp + 0x38]
        push   dword ptr [esp + 0x38]
        mov    eax, dword ptr [0x12f80]      ; -> obj1:0x12F80
        push   dword ptr [eax + 0x80]
        call   sub_1_6678
        add    esp, 0x1c
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0x2c], 0
        jmp    loc_1_E7DA
        db  08Bh, 0FFh
loc_1_E780:
        mov    eax, dword ptr [0x12f80]      ; -> obj1:0x12F80
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x2c]
        push   dword ptr [eax + 0x80]
        call   sub_1_6954
        add    esp, 0x10
        or     eax, eax
        je     loc_1_E7AA
        cmp    eax, 0xff
        jne    loc_1_EA63
loc_1_E7AA:
        mov    ecx, dword ptr [esp + 0x10]
        add    ecx, 0x20
        mov    edx, dword ptr [ecx]
        mov    ebp, edx
        and    ebp, 0xfffffff0
        mov    dword ptr [ecx], ebp
        mov    ebp, dword ptr [esp + 0x10]
        cmp    di, 1
        sbb    ecx, ecx
        and    ecx, 0xfffffffe
        add    ecx, 4
        or     dword ptr [ebp + 0x20], ecx
        test   dl, 1
        je     loc_1_E7DA
        mov    ecx, dword ptr [esp + 0x10]
        or     dword ptr [ecx + 0x20], 1
loc_1_E7DA:
        or     bx, bx
        je     loc_1_E7F0
        cmp    bx, 2
        je     loc_1_E7F0
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0xc], 0
loc_1_E7F0:
        or     eax, eax
        je     loc_1_E80C
        cmp    eax, 0xff
        je     loc_1_E80C
        mov    esi, 0xc0000001
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_E80C:
        mov    eax, dword ptr [0x132e0]      ; -> loc_1_132E0
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0x18], eax
        mov    edi, dword ptr [esp + 0x2c]
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [edi + 0x10]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, dword ptr [esp + 0x38]
        mov    ecx, dword ptr [ebp + 0x88]
        mov    edx, ecx
        not    edx
        and    dword ptr [eax + 0x2c], edx
        mov    eax, dword ptr [esp + 0x10]
        or     dword ptr [eax + 0x28], ecx
        mov    eax, dword ptr [esp + 0x10]
        and    dword ptr [eax + 0x20], 0xffffff0f
        mov    eax, dword ptr [esp + 0x24]
        cmp    byte ptr [eax], 0x2a
        jne    loc_1_E874
        mov    eax, dword ptr [esp + 0x10]
        or     dword ptr [eax + 0x20], 0x10
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_E874:
        mov    ecx, dword ptr [esp + 0x10]
        add    ecx, 0x20
        mov    eax, dword ptr [ecx]
        test   al, 1
        jne    loc_1_EA54
        cmp    bx, 2
        jae    loc_1_EA54
        cmp    esi, 0x7f000000
        jne    loc_1_E8A8
        or     eax, 0x10
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_E8A8:
        or     eax, 0x80
        mov    dword ptr [ecx], eax
        lea    eax, [esp + 0x14]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_E8E0
        cmp    esi, 0xff
        je     loc_1_E8E0
        push   dword ptr [esp + 0x10]
        call   sub_1_DEA4
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_E8E0:
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [ecx + 0x38], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x2c], ebp
        mov    edx, dword ptr [0x12f80]      ; -> obj1:0x12F80
        mov    cx, word ptr [edx + 0x9e]
        inc    cx
        mov    bx, cx
        mov    esi, dword ptr [edx + 0x94]
        mov    eax, dword ptr [esp + 0x14]
        mov    word ptr [eax + 0x4c], 1
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x34], 2
        test   byte ptr [0x13364], 0xa       ; -> loc_1_13364
        je     loc_1_E9A7
        mov    bx, word ptr [edx + 0x9c]
        inc    bx
        mov    esi, dword ptr [edx + 0x98]
        mov    eax, dword ptr [esp + 0x14]
        mov    word ptr [eax + 0x4c], 2
        cmp    dword ptr [ebp + 0x74], 0x7f000000
        je     loc_1_E969
        cmp    byte ptr [ebp + 0x95], 0
        je     loc_1_E9A7
loc_1_E969:
        test   byte ptr [0x13364], 8         ; -> loc_1_13364
        je     loc_1_E994
        mov    eax, dword ptr [esp + 0x14]
        mov    word ptr [eax + 0x4c], 1
        mov    bx, cx
        mov    esi, dword ptr [edx + 0x94]
        push   0
        push   1
        call   sub_1_FA40
        add    esp, 8
        jmp    loc_1_E9A7
        db  090h
loc_1_E994:
        mov    esi, 0xa
        mov    bx, 1
        mov    eax, dword ptr [esp + 0x14]
        mov    word ptr [eax + 0x4c], 4
loc_1_E9A7:
        push   3
        mov    eax, dword ptr [esp + 0x14]
        inc    dword ptr [eax + 0x1c]
        push   esi
        movzx  eax, bx
        push   eax
        push   dword ptr [esp + 0x3c]
        push   edi
        push   0xea6c                        ; -> loc_1_EA6C
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x44]
        push   dword ptr [esp + 0x30]
        call   sub_1_10C0C
        add    esp, 0x24
        mov    esi, eax
        push   dword ptr [esp + 0x14]
        call   sub_1_F280
        add    esp, 4
        push   dword ptr [esp + 0x10]
        call   sub_1_DEA4
        add    esp, 4
        or     esi, esi
        je     loc_1_EA44
        cmp    esi, 0xff
        je     loc_1_EA44
        mov    eax, dword ptr [esp + 0x10]
        and    dword ptr [eax + 0x20], 0xffffff0f
        mov    eax, dword ptr [esp + 0x10]
        or     dword ptr [eax + 0x20], 0x40
        cmp    esi, 1
        je     loc_1_EA20
        mov    eax, dword ptr [esp + 0x14]
        push   dword ptr [eax + 0x1c]
        call   sub_1_0024
        add    esp, 4
loc_1_EA20:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x14], 0
        push   dword ptr [esp + 0x14]
        call   sub_1_F29C
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_EA44:
        pop    ebp
        mov    eax, 0xff
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_EA54:
        or     eax, 0x10
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x10]
        or     dword ptr [eax + 0x20], 1
        sub    eax, eax
loc_1_EA63:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_EA6C:
        push   ebx
        push   esi
        push   edi
        mov    edi, 3
        mov    esi, dword ptr [esp + 0x18]
        or     esi, esi
        je     loc_1_EC98
        mov    cx, word ptr [esi + 0x3c]
        test   cl, 0x10
        je     loc_1_EAE4
        and    cx, 0xffef
        mov    word ptr [esi + 0x3c], cx
        or     cx, 1
        mov    word ptr [esi + 0x3c], cx
        mov    ebx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [ebx + 0x2c]
        cmp    dword ptr [ecx + 0x68], 0
        je     loc_1_EAD8
        mov    ax, word ptr [ebx + 0x4c]
        test   al, 2
        je     loc_1_EAB8
        cmp    dword ptr [ecx + 0x74], 0x7f000000
        je     loc_1_EAD8
loc_1_EAB8:
        test   al, 4
        je     loc_1_ECA4
        cmp    dword ptr [ecx + 0x78], 0x7f000000
        jne    loc_1_ECA4
        mov    dword ptr [esi + 0x38], 0xa
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_EAD8:
        pop    edi
        mov    dword ptr [esi + 0x38], 0xa
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_EAE4:
        cmp    dword ptr [esi + 0x30], 0
        je     loc_1_ECA4
        mov    ax, word ptr [esi + 0x3e]
        dec    ax
        mov    word ptr [esi + 0x3e], ax
        je     loc_1_EB38
        cmp    ax, 1
        mov    ebx, dword ptr [esp + 0x10]
        jne    loc_1_EC2D
        test   byte ptr [ebx + 0x4c], 1
        je     loc_1_EB18
        mov    edi, 4
        jmp    loc_1_EC2D
loc_1_EB18:
        test   byte ptr [0x13364], 0xa       ; -> loc_1_13364
        je     loc_1_EC2D
        or     cx, 1
        mov    word ptr [esi + 0x3c], cx
        mov    dword ptr [esi + 0x38], 5
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_EB38:
        mov    ebx, dword ptr [esp + 0x10]
        mov    dx, word ptr [ebx + 0x4c]
        mov    cx, dx
        and    cx, 0xfffc
        mov    word ptr [ebx + 0x4c], cx
        mov    eax, dword ptr [0x132cc]      ; -> loc_1_132CC
        mov    dword ptr [esi + 0x38], eax
        mov    ax, word ptr [0x132d0]        ; -> loc_1_132D0
        inc    ax
        mov    word ptr [esi + 0x3e], ax
        mov    ax, dx
        and    ax, 1
        je     loc_1_EBB0
        test   byte ptr [0x13364], 4         ; -> loc_1_13364
        je     loc_1_EBB0
        push   0
        push   1
        call   sub_1_FA40
        add    esp, 8
        or     word ptr [ebx + 0x4c], 2
        mov    eax, dword ptr [ebx + 0x2c]
        cmp    dword ptr [eax + 0x74], 0x7f000000
        je     loc_1_EB9C
        cmp    byte ptr [eax + 0x95], 0
        je     loc_1_EC2D
loc_1_EB9C:
        mov    dword ptr [esi + 0x38], 0xa
        mov    word ptr [esi + 0x3e], 1
        jmp    loc_1_EC2D
        db  08Bh, 0FFh
loc_1_EBB0:
        test   dl, 2
        je     loc_1_EBEC
        test   byte ptr [0x13364], 1         ; -> loc_1_13364
        jne    loc_1_EBEC
        or     cx, 4
        mov    word ptr [ebx + 0x4c], cx
        mov    eax, dword ptr [ebx + 0x2c]
        cmp    dword ptr [eax + 0x78], 0x7f000000
        je     loc_1_EBDB
        cmp    byte ptr [eax + 0x95], 0
        je     loc_1_EC2D
loc_1_EBDB:
        mov    dword ptr [esi + 0x38], 0xa
        mov    word ptr [esi + 0x3e], 1
        jmp    loc_1_EC2D
        db  08Bh, 0FFh
loc_1_EBEC:
        test   byte ptr [0x13364], 8         ; -> loc_1_13364
        je     loc_1_EC60
        or     ax, ax
        jne    loc_1_EC60
        test   dl, 4
        je     loc_1_EC2D
        or     cx, 1
        mov    word ptr [ebx + 0x4c], cx
        push   0
        push   1
        call   sub_1_FA40
        add    esp, 8
        mov    eax, dword ptr [0x132c8]      ; -> loc_1_132C8
        mov    dword ptr [esi + 0x38], eax
        mov    eax, dword ptr [0x12f80]      ; -> obj1:0x12F80
        mov    ax, word ptr [eax + 0x9e]
        inc    ax
        mov    word ptr [esi + 0x3e], ax
loc_1_EC2D:
        push   edi
        inc    dword ptr [ebx + 0x34]
        push   0
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   dword ptr [ebx + 0x38]
        call   sub_1_10C0C
        add    esp, 0x24
        push   ebx
        call   sub_1_F280
        add    esp, 4
        or     word ptr [esi + 0x3c], 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_EC60:
        test   byte ptr [0x13364], 1         ; -> loc_1_13364
        je     loc_1_EC75
        push   0
        push   1
        call   sub_1_FA40
        add    esp, 8
loc_1_EC75:
        push   dword ptr [ebx + 0x2c]
        call   sub_1_E5EC
        add    esp, 4
        push   ebx
        call   sub_1_F280
        add    esp, 4
        push   0
        push   esi
        call   sub_1_108C8
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_EC98:
        push   dword ptr [esp + 0x10]
        call   sub_1_F29C
        add    esp, 4
loc_1_ECA4:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_ECA8:
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        or     esi, esi
        je     loc_1_ED86
        mov    ax, word ptr [esi + 0x3e]
        dec    ax
        mov    word ptr [esi + 0x3e], ax
        je     loc_1_ED60
        mov    edi, dword ptr [esp + 0x14]
        inc    dword ptr [edi + 0x34]
        push   edi
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x10]
        push   eax
        push   0
        push   0
        push   0
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        mov    eax, dword ptr [edi + 0x38]
        add    eax, 0x34
        push   eax
        call   sub_1_10E60
        add    esp, 0x20
        or     eax, eax
        je     loc_1_ED4A
        mov    eax, dword ptr [esp + 8]
        and    word ptr [eax + 2], 0xfffe
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 8]
        mov    byte ptr [eax + ecx - 3], 0x21
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edi + 0x1c], eax
        mov    eax, dword ptr [edi + 0x2c]
        cmp    dword ptr [eax + 0x58], 0
        je     loc_1_ED28
        mov    eax, dword ptr [eax + 0x68]
        jmp    loc_1_ED2A
        db  08Bh, 0FFh
loc_1_ED28:
        sub    eax, eax
loc_1_ED2A:
        push   0x10
        push   0x89
        push   dword ptr [esp + 0x10]
        push   0x110a8                       ; -> loc_1_110A8
        push   eax
        mov    eax, dword ptr [edi + 0x38]
        push   dword ptr [eax + 0xc]
        push   edi
        call   sub_1_115D4
        add    esp, 0x1c
loc_1_ED4A:
        push   edi
        call   sub_1_F280
        add    esp, 4
        or     word ptr [esi + 0x3c], 1
        pop    edi
        pop    esi
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_ED60:
        mov    edx, dword ptr [esi + 0x30]
        mov    dword ptr [esi + 0x30], 0
        or     edx, edx
        je     loc_1_ED86
        mov    edi, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0x15
        push   dword ptr [esi + 0x2c]
        call   edx
        add    esp, 8
loc_1_ED86:
        pop    edi
        pop    esi
        add    esp, 8
        ret    
loc_1_ED8C:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        or     edi, edi
        je     loc_1_EDEB
        cmp    dword ptr [edi + 0x30], 0
        je     loc_1_EDEB
        mov    ax, word ptr [edi + 0x3e]
        dec    ax
        mov    word ptr [edi + 0x3e], ax
        je     loc_1_EDE0
        mov    esi, dword ptr [esp + 0xc]
        inc    dword ptr [esi + 0x34]
        push   7
        push   0
        push   0
        push   0
        push   0
        push   0
        push   esi
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   dword ptr [esi + 0x38]
        call   sub_1_10C0C
        add    esp, 0x24
        or     word ptr [edi + 0x3c], 1
        push   esi
        call   sub_1_F280
        add    esp, 4
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_EDE0:
        push   0x15
        push   edi
        call   sub_1_108C8
        add    esp, 8
loc_1_EDEB:
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_EDF0:
        push   esi
        push   edi
        mov    esi, 1
        mov    edx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [edx + 0x28]
        test   esi, ecx
        jne    loc_1_EE08
loc_1_EE02:
        add    esi, esi
        test   esi, ecx
        je     loc_1_EE02
loc_1_EE08:
        mov    edi, dword ptr [0x1323c]      ; -> loc_1_1323C
        mov    eax, 1
        cmp    esi, eax
        jbe    loc_1_EE20
        nop    
loc_1_EE18:
        mov    edi, dword ptr [edi]
        add    eax, eax
        cmp    eax, esi
        jb     loc_1_EE18
loc_1_EE20:
        not    esi
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x2c], edi
        and    esi, ecx
        mov    dword ptr [edx + 0x28], esi
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_EE34:
        sub    esp, 0xc
        push   esi
        push   edi
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     loc_1_EE55
        cmp    eax, 0xff
        jne    loc_1_EF64
loc_1_EE55:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x1c], 0
        mov    esi, dword ptr [esp + 0x18]
        push   esi
        push   dword ptr [esp + 0xc]
        call   sub_1_EDF0
        add    esp, 8
        mov    eax, dword ptr [esp + 0x28]
        and    eax, 0xf
        cmp    eax, 2
        je     loc_1_EEA0
        cmp    eax, 4
        je     loc_1_EEA0
        cmp    eax, 8
        je     loc_1_EEA0
        mov    dx, 1
        mov    eax, dword ptr [0x12f80]      ; -> obj1:0x12F80
        mov    ecx, dword ptr [eax + 0x94]
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 0x4c], dx
        jmp    loc_1_EEBC
loc_1_EEA0:
        mov    eax, dword ptr [0x12f80]      ; -> obj1:0x12F80
        mov    dx, word ptr [eax + 0x9c]
        mov    ecx, dword ptr [eax + 0x98]
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 0x4c], 2
loc_1_EEBC:
        push   6
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x34], 2
        push   ecx
        movzx  eax, dx
        push   eax
        push   dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x30]
        push   0xef6c                        ; -> loc_1_EF6C
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x38]
        push   esi
        call   sub_1_10C0C
        add    esp, 0x24
        mov    edi, eax
        push   dword ptr [esp + 8]
        call   sub_1_F280
        add    esp, 4
        or     edi, edi
        je     loc_1_EF62
        cmp    edi, 0xff
        je     loc_1_EF62
        mov    dword ptr [esp + 0xc], 0
        mov    ecx, dword ptr [esi + 0x14]
        or     ecx, ecx
        je     loc_1_EF38
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x10]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0xc
        mov    dword ptr [esi + 0x14], 0
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0xc
        ret    
        db  090h
loc_1_EF38:
        cmp    edi, 1
        je     loc_1_EF54
        push   5
        push   dword ptr [esp + 0xc]
        call   sub_1_D5C0
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0xc
        ret    
        db  090h
loc_1_EF54:
        push   4
        push   dword ptr [esp + 0xc]
        call   sub_1_D5C0
        add    esp, 8
loc_1_EF62:
        mov    eax, edi
loc_1_EF64:
        pop    edi
        pop    esi
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
loc_1_EF6C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        or     esi, esi
        je     loc_1_F01C
        mov    ax, word ptr [esi + 0x3e]
        dec    ax
        mov    word ptr [esi + 0x3e], ax
        jne    loc_1_EFE4
        test   byte ptr [0x13364], 4         ; -> loc_1_13364
        mov    edi, dword ptr [esp + 0xc]
        je     loc_1_EFC0
        mov    ax, word ptr [edi + 0x4c]
        test   al, 2
        je     loc_1_EFC0
        and    ax, 0xfffd
        mov    word ptr [edi + 0x4c], ax
        or     ax, 1
        mov    word ptr [edi + 0x4c], ax
        mov    eax, dword ptr [0x132c8]      ; -> loc_1_132C8
        mov    dword ptr [esi + 0x38], eax
        mov    ax, word ptr [0x132d2]        ; -> loc_1_132D2
        mov    word ptr [esi + 0x3e], ax
        jmp    loc_1_EFE8
        db  090h
loc_1_EFC0:
        push   0x15
        push   esi
        call   sub_1_108C8
        add    esp, 8
        or     eax, eax
        je     loc_1_EFD6
        cmp    eax, 0xff
        jne    loc_1_F028
loc_1_EFD6:
        push   edi
        call   sub_1_F280
        add    esp, 4
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_EFE4:
        mov    edi, dword ptr [esp + 0xc]
loc_1_EFE8:
        inc    dword ptr [edi + 0x34]
        push   6
        push   0
        push   0
        push   0
        push   0
        push   0
        push   edi
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   dword ptr [edi + 0x38]
        call   sub_1_10C0C
        add    esp, 0x24
        push   edi
        call   sub_1_F280
        add    esp, 4
        or     word ptr [esi + 0x3c], 1
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_F01C:
        push   dword ptr [esp + 0xc]
        call   sub_1_F29C
        add    esp, 4
loc_1_F028:
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_F02C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x14]
        mov    edx, dword ptr [edi + 0x2c]
        mov    ebx, dword ptr [esp + 0x1c]
        or     ebx, ebx
        jne    loc_1_F050
        push   edi
        call   sub_1_F29C
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_F050:
        mov    ax, word ptr [ebx + 0x3c]
        test   al, 0x10
        je     loc_1_F080
        and    ax, 0xffef
        mov    word ptr [ebx + 0x3c], ax
        or     ax, 1
        mov    word ptr [ebx + 0x3c], ax
        cmp    dword ptr [edx + 0x68], 0
        jne    loc_1_F21C
        mov    dword ptr [ebx + 0x38], 0xa
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_F080:
        mov    ebp, dword ptr [ebx + 0x30]
        or     ebp, ebp
        je     loc_1_F21C
        mov    esi, dword ptr [ebx + 0x2c]
        test   byte ptr [esi + 0x4c], 0x40
        je     loc_1_F0E8
        mov    eax, dword ptr [ebx + 0x34]
        mov    dword ptr [eax + 0x14], 0
        mov    dword ptr [ebx + 0x30], 0
        mov    eax, dword ptr [edi + 0x38]
        and    dword ptr [eax + 0x20], 0xffffff0f
        mov    eax, dword ptr [edi + 0x38]
        or     dword ptr [eax + 0x20], 0x20
        mov    eax, dword ptr [edi + 0x38]
        mov    dword ptr [eax + 0x14], 0
        push   dword ptr [edi + 0x38]
        call   sub_1_DEA4
        add    esp, 4
        push   0xc0000120
        push   esi
        push   ebp
        call   sub_1_E64C
        add    esp, 0xc
        push   edi
        call   sub_1_F280
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_F0E8:
        mov    ecx, 1
        mov    ax, word ptr [ebx + 0x3e]
        dec    ax
        mov    word ptr [ebx + 0x3e], ax
        jne    loc_1_F196
        mov    eax, dword ptr [edx + 0x80]
        or     eax, eax
        je     loc_1_F134
        cmp    eax, 0x7f000000
        je     loc_1_F134
        mov    ax, word ptr [edi + 0x4c]
        test   ah, 1
        jne    loc_1_F134
        mov    dx, word ptr [0x132d0]        ; -> loc_1_132D0
        mov    word ptr [ebx + 0x3e], dx
        and    ax, 0xff7f
        mov    word ptr [edi + 0x4c], ax
        or     ax, 0x100
        mov    word ptr [edi + 0x4c], ax
        jmp    loc_1_F196
loc_1_F134:
        mov    edx, dword ptr [edi + 0x3c]
        cmp    dword ptr [0x13333], edx      ; -> obj1:0x13333
        jne    loc_1_F154
        mov    eax, dword ptr [0x13337]      ; -> obj1:0x13337
        mov    dword ptr [edi + 0x3c], eax
        or     eax, eax
        je     loc_1_F150
        jmp    loc_1_F178
        db  08Dh, 049h, 000h
loc_1_F150:
        sub    ecx, ecx
        jmp    loc_1_F196
loc_1_F154:
        cmp    byte ptr [edx], 0x2c
        je     loc_1_F169
loc_1_F159:
        mov    al, byte ptr [edx]
        cmp    al, 0x20
        je     loc_1_F169
        or     al, al
        je     loc_1_F169
        inc    edx
        cmp    byte ptr [edx], 0x2c
        jne    loc_1_F159
loc_1_F169:
        cmp    byte ptr [edx], 0
        jne    loc_1_F174
        sub    ecx, ecx
        jmp    loc_1_F196
        db  08Bh, 0FFh
loc_1_F174:
        inc    edx
        mov    dword ptr [edi + 0x3c], edx
loc_1_F178:
        mov    ax, word ptr [edi + 0x4c]
        and    ax, 0xfeff
        mov    word ptr [edi + 0x4c], ax
        or     ax, 0x80
        mov    word ptr [edi + 0x4c], ax
        mov    ax, word ptr [0x132d0]        ; -> loc_1_132D0
        mov    word ptr [ebx + 0x3e], ax
loc_1_F196:
        or     ecx, ecx
        je     loc_1_F1D0
        inc    dword ptr [edi + 0x34]
        push   1
        push   0
        push   0
        push   0
        push   0
        push   0
        push   edi
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   dword ptr [edi + 0x38]
        call   sub_1_10C0C
        add    esp, 0x24
        push   edi
        call   sub_1_F280
        add    esp, 4
        or     word ptr [ebx + 0x3c], 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_F1D0:
        mov    eax, dword ptr [ebx + 0x34]
        mov    dword ptr [eax + 0x14], 0
        mov    dword ptr [ebx + 0x30], 0
        mov    eax, dword ptr [edi + 0x38]
        and    dword ptr [eax + 0x20], 0xffffff0f
        mov    eax, dword ptr [edi + 0x38]
        or     dword ptr [eax + 0x20], 0x20
        mov    eax, dword ptr [edi + 0x38]
        mov    dword ptr [eax + 0x14], 0
        push   dword ptr [edi + 0x38]
        call   sub_1_DEA4
        add    esp, 4
        push   0x15
        push   esi
        push   ebp
        call   sub_1_E64C
        add    esp, 0xc
        push   edi
        call   sub_1_F280
        add    esp, 4
loc_1_F21C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_F224:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    edx, dword ptr [esi + 0x14]
        cmp    dword ptr [edx + 0x28], 0
        je     loc_1_F254
        movzx  eax, word ptr [0x13364]       ; -> loc_1_13364
        push   eax
        push   0xf224                        ; -> sub_1_F224
        push   esi
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   edx
        call   sub_1_EE34
        add    esp, 0x14
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_F254:
        cmp    dword ptr [esi + 0x18], 0
        jne    loc_1_F27D
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edx + 8], 0
        push   edx
        call   sub_1_DEA4
        add    esp, 4
        push   esi
        call   sub_1_FA98
        add    esp, 4
loc_1_F27D:
        pop    esi
        ret    
        db  090h
sub_1_F280:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x34]
        dec    eax
        mov    dword ptr [ecx + 0x34], eax
        jne    loc_1_F298
        push   4
        push   ecx
        call   sub_1_D5C0
        add    esp, 8
loc_1_F298:
        ret    
        db  08Dh, 049h, 000h
sub_1_F29C:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x34]
        dec    ecx
        mov    dword ptr [eax + 0x34], ecx
        jne    loc_1_F2B4
        push   4
        push   eax
        call   sub_1_D5C0
        add    esp, 8
loc_1_F2B4:
        ret    
        db  08Dh, 049h, 000h, 08Bh, 044h, 024h, 004h, 08Bh, 048h, 034h, 049h, 089h, 048h, 034h, 075h, 00Bh
        db  06Ah, 005h, 050h, 0E8h, 0F3h, 0E2h, 0FFh, 0FFh, 083h, 0C4h, 008h, 0C3h, 08Dh, 049h, 000h
sub_1_F2D4:
        push   esi
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_1_F314
        lea    eax, [esp + 0xc]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     loc_1_F2FC
        cmp    eax, 0xff
        je     loc_1_F2FC
        mov    eax, 1
        pop    esi
        ret    
        db  090h
loc_1_F2FC:
        mov    eax, dword ptr [esp + 0xc]
        mov    word ptr [eax + 0x4c], 2
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x34], 2
        jmp    loc_1_F31B
        db  090h
loc_1_F314:
        mov    eax, dword ptr [esp + 0xc]
        inc    dword ptr [eax + 0x34]
loc_1_F31B:
        mov    eax, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x38], edx
        cmp    byte ptr [esp + 0x10], 0
        je     loc_1_F343
        mov    eax, dword ptr [0x1323c]      ; -> loc_1_1323C
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [ecx + 0x2c], eax
        mov    eax, dword ptr [esp + 0xc]
        mov    word ptr [eax + 0x40], 0
loc_1_F343:
        push   7
        push   dword ptr [0x132cc]           ; -> loc_1_132CC
        movzx  eax, word ptr [0x132d0]       ; -> loc_1_132D0
        push   eax
        push   0xf454                        ; -> loc_1_F454
        push   dword ptr [esp + 0x1c]
        push   0xed8c                        ; -> loc_1_ED8C
        push   dword ptr [esp + 0x24]
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   edx
        call   sub_1_10C0C
        add    esp, 0x24
        mov    esi, eax
        push   dword ptr [esp + 0xc]
        call   sub_1_F280
        add    esp, 4
        or     esi, esi
        je     loc_1_F3A0
        cmp    esi, 0xff
        je     loc_1_F3A0
        push   dword ptr [esp + 0xc]
        call   sub_1_F29C
        add    esp, 4
        mov    eax, esi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_F3A0:
        mov    eax, esi
        pop    esi
        ret    
sub_1_F3A4:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        movzx  edx, word ptr [0x1330a]       ; -> loc_1_1330A
        mov    eax, dword ptr [0x132b4]      ; -> loc_1_132B4
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 0x10], eax
        cmp    edx, eax
        jge    loc_1_F424
        mov    eax, dword ptr [0x132b4]      ; -> loc_1_132B4
        lea    eax, [eax + edx*8 + 8]
        mov    dword ptr [esp + 0x14], eax
        mov    esi, eax
        mov    ebx, dword ptr [esp + 0x1c]
loc_1_F3D4:
        or     ebx, ebx
        je     loc_1_F3E8
        test   byte ptr [ebx + 0x20], 0x10
        je     loc_1_F3E8
        mov    ebp, dword ptr [esp + 0x14]
        mov    edi, dword ptr [ebx]
        sub    ebx, ebx
        jmp    loc_1_F3EC
loc_1_F3E8:
        mov    ebp, esi
        mov    edi, dword ptr [esi]
loc_1_F3EC:
        cmp    edi, ebp
        je     loc_1_F41A
loc_1_F3F0:
        mov    eax, dword ptr [edi + 0x20]
        test   al, 0x10
        je     loc_1_F414
        cmp    byte ptr [edi + 0x34], 0x2a
        je     loc_1_F414
        test   al, 1
        jne    loc_1_F414
        mov    eax, 1
        mov    cl, byte ptr [0x1334f]        ; -> obj1:0x1334F
        shl    eax, cl
        dec    eax
        cmp    dword ptr [edi + 0x2c], eax
        jne    loc_1_F438
loc_1_F414:
        mov    edi, dword ptr [edi]
        cmp    edi, ebp
        jne    loc_1_F3F0
loc_1_F41A:
        inc    edx
        add    esi, 8
        cmp    edx, dword ptr [esp + 0x10]
        jl     loc_1_F3D4
loc_1_F424:
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [eax], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_F438:
        mov    eax, dword ptr [edi + 8]
        inc    dword ptr [eax + 0x18]
        mov    word ptr [0x1330a], dx        ; -> loc_1_1330A
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [eax], edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_F454:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 0x2c]
        push   0xf470                        ; -> loc_1_F470
        push   0
        push   dword ptr [esp + 0x14]
        push   eax
        call   sub_1_3C00
        add    esp, 0x14
        ret    
loc_1_F470:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    cl, cl
        mov    eax, dword ptr [esp + 0x18]
        mov    esi, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0xc]
        mov    edi, dword ptr [esi + 0x38]
        or     eax, eax
        jne    loc_1_F498
        mov    eax, dword ptr [esi + 0x2c]
        mov    eax, dword ptr [eax + 0x88]
        or     dword ptr [edi + 0x2c], eax
        jmp    loc_1_F4AE
loc_1_F498:
        cmp    eax, 0x15
        je     loc_1_F4B0
        mov    eax, dword ptr [edi + 0x20]
        and    eax, 0xffffff0f
        mov    dword ptr [edi + 0x20], eax
        or     eax, 0x40
        mov    dword ptr [edi + 0x20], eax
loc_1_F4AE:
        mov    cl, 1
loc_1_F4B0:
        cmp    byte ptr [0x13350], 0         ; -> loc_1_13350
        je     loc_1_F4BB
        mov    cl, 1
loc_1_F4BB:
        mov    eax, dword ptr [esi + 0x2c]
        mov    ebx, dword ptr [eax + 0x88]
        add    ebx, ebx
        mov    edx, dword ptr [edi + 0x28]
        cmp    ebx, edx
        ja     loc_1_F50D
loc_1_F4CD:
        test   edx, ebx
        jne    loc_1_F4D7
loc_1_F4D1:
        add    ebx, ebx
        test   edx, ebx
        je     loc_1_F4D1
loc_1_F4D7:
        mov    eax, dword ptr [0x1323c]      ; -> loc_1_1323C
        mov    ebp, 1
        cmp    ebx, ebp
        jbe    loc_1_F4ED
loc_1_F4E5:
        mov    eax, dword ptr [eax]
        add    ebp, ebp
        cmp    ebp, ebx
        jb     loc_1_F4E5
loc_1_F4ED:
        cmp    dword ptr [eax + 0x58], 0x7f000000
        je     loc_1_F4FF
        cmp    dword ptr [eax + 0x74], 0x7f000000
        jne    loc_1_F524
loc_1_F4FF:
        add    ebx, ebx
        cmp    ebx, edx
        jbe    loc_1_F4CD
        cmp    ebx, edx
        jbe    loc_1_F5AA
loc_1_F50D:
        or     cl, cl
        je     loc_1_F548
        lea    eax, [esp + 0x10]
        push   eax
        push   edi
        call   sub_1_F3A4
        add    esp, 8
        jmp    loc_1_F550
        db  08Dh, 049h, 000h
loc_1_F524:
        mov    dword ptr [esi + 0x2c], eax
        mov    dword ptr [edi + 0x14], 0
        push   0
        push   esi
        push   edi
        call   sub_1_F2D4
        add    esp, 0xc
        or     eax, eax
        je     loc_1_F5AA
        cmp    eax, 0xff
        je     loc_1_F5AA
        jmp    loc_1_F59A
        db  090h
loc_1_F548:
        mov    dword ptr [esp + 0x10], 0
loc_1_F550:
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_F581
        push   1
        push   esi
        push   dword ptr [esp + 0x18]
        call   sub_1_F2D4
        add    esp, 0xc
        or     eax, eax
        je     loc_1_F581
        cmp    eax, 0xff
        je     loc_1_F581
        mov    byte ptr [0x13356], 0         ; -> loc_1_13356
        push   esi
        call   sub_1_F280
        add    esp, 4
loc_1_F581:
        push   dword ptr [edi + 8]
        mov    dword ptr [edi + 0x14], 0
        call   sub_1_DDE4
        add    esp, 4
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_F5AA
loc_1_F59A:
        push   esi
        call   sub_1_F280
        add    esp, 4
        mov    byte ptr [0x13356], 0         ; -> loc_1_13356
loc_1_F5AA:
        push   dword ptr [esp + 0x18]
        call   sub_1_0024
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_F5C0:
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        or     esi, esi
        je     loc_1_F61E
        test   byte ptr [0x13364], 1         ; -> loc_1_13364
        je     loc_1_F5E4
        mov    word ptr [esi + 0x3c], 0
        mov    dword ptr [0x132f0], 0        ; -> loc_1_132F0
        pop    esi
        ret    
loc_1_F5E4:
        cmp    byte ptr [0x13356], 0         ; -> loc_1_13356
        jne    loc_1_F609
        mov    byte ptr [0x13356], 1         ; -> loc_1_13356
        push   0
        push   0xf620                        ; -> loc_1_F620
        push   0
        push   0
        push   0
        call   sub_1_3C00
        add    esp, 0x14
loc_1_F609:
        sub    edx, edx
        mov    eax, dword ptr [0x132e0]      ; -> loc_1_132E0
        div    dword ptr [0x132e4]           ; -> loc_1_132E4
        mov    dword ptr [esi + 0x38], eax
        or     word ptr [esi + 0x3c], 1
loc_1_F61E:
        pop    esi
        ret    
loc_1_F620:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 0x28]
        call   sub_1_0024
        add    esp, 4
        movzx  ecx, word ptr [0x13314]       ; -> loc_1_13314
        inc    word ptr [0x13314]            ; -> loc_1_13314
        movzx  eax, word ptr [0x13314]       ; -> loc_1_13314
        sub    edx, edx
        div    dword ptr [0x132e4]           ; -> loc_1_132E4
        mov    word ptr [0x13314], dx        ; -> loc_1_13314
        cmp    dword ptr [0x132e0], -1       ; -> loc_1_132E0
        jne    loc_1_F670
        mov    byte ptr [0x13356], 0         ; -> loc_1_13356
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  090h
loc_1_F670:
        mov    eax, dword ptr [0x132b4]      ; -> loc_1_132B4
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [0x132e4]      ; -> loc_1_132E4
        shr    eax, 1
        cmp    eax, ecx
        je     loc_1_F68C
        or     ecx, ecx
        jne    loc_1_F750
loc_1_F68C:
        mov    eax, dword ptr [0x132b4]      ; -> loc_1_132B4
        mov    dword ptr [esp + 0x14], 0
        mov    dword ptr [esp + 0x1c], 0
        cmp    dword ptr [eax], 0
        jle    loc_1_F750
        add    eax, 8
        mov    dword ptr [esp + 0x18], eax
loc_1_F6B1:
        mov    eax, dword ptr [esp + 0x18]
        mov    esi, dword ptr [eax]
        cmp    esi, eax
        je     loc_1_F737
loc_1_F6BC:
        mov    dword ptr [esp + 0x10], esi
        mov    eax, dword ptr [esi + 0x20]
        test   al, 0x10
        je     loc_1_F72F
        cmp    byte ptr [esi + 0x34], 0x2a
        je     loc_1_F72F
        test   al, 1
        jne    loc_1_F72F
        mov    ebp, dword ptr [0x1323c]      ; -> loc_1_1323C
        sub    ebx, ebx
        cmp    ebp, 0x1323c                  ; -> loc_1_1323C
        je     loc_1_F724
loc_1_F6E1:
        mov    cl, bl
        mov    edi, 1
        shl    edi, cl
        inc    ebx
        mov    ecx, ebp
        mov    ebp, dword ptr [ebp]
        mov    eax, dword ptr [esp + 0x10]
        test   dword ptr [eax + 0x28], edi
        je     loc_1_F71C
        test   dword ptr [esp + 0x14], edi
        jne    loc_1_F71C
        mov    eax, dword ptr [eax + 0x2c]
        and    eax, edi
        je     loc_1_F70F
        cmp    byte ptr [ecx + 0x93], 0
        je     loc_1_F71C
loc_1_F70F:
        push   ecx
        call   sub_1_A08C
        add    esp, 4
        or     dword ptr [esp + 0x14], edi
loc_1_F71C:
        cmp    ebp, 0x1323c                  ; -> loc_1_1323C
        jne    loc_1_F6E1
loc_1_F724:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x2c], 0
loc_1_F72F:
        mov    esi, dword ptr [esi]
        cmp    esi, dword ptr [esp + 0x18]
        jne    loc_1_F6BC
loc_1_F737:
        inc    dword ptr [esp + 0x1c]
        add    dword ptr [esp + 0x18], 8
        mov    eax, dword ptr [esp + 0x20]
        mov    ecx, dword ptr [esp + 0x1c]
        cmp    dword ptr [eax], ecx
        jg     loc_1_F6B1
loc_1_F750:
        lea    eax, [esp + 0x10]
        mov    word ptr [0x1330a], 0         ; -> loc_1_1330A
        push   eax
        push   0
        call   sub_1_F3A4
        add    esp, 8
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_F7A8
        push   1
        push   0
        push   dword ptr [esp + 0x18]
        call   sub_1_F2D4
        add    esp, 0xc
        or     eax, eax
        je     loc_1_F7AF
        cmp    eax, 0xff
        je     loc_1_F7AF
        mov    eax, dword ptr [esp + 0x10]
        push   dword ptr [eax + 8]
        call   sub_1_DDE4
        add    esp, 4
        mov    byte ptr [0x13356], 0         ; -> loc_1_13356
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_F7A8:
        mov    byte ptr [0x13356], 0         ; -> loc_1_13356
loc_1_F7AF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  090h
sub_1_F7B8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_F832
        mov    edi, dword ptr [0x132b8]      ; -> loc_1_132B8
        sub    ebx, ebx
        mov    eax, edi
        cmp    dword ptr [eax], ebx
        jle    loc_1_F829
        mov    esi, edi
        add    esi, 8
loc_1_F7D6:
        mov    ebp, dword ptr [esi]
        cmp    ebp, esi
        je     loc_1_F821
loc_1_F7DC:
        mov    eax, ebp
        mov    ebp, dword ptr [ebp]
        mov    dx, word ptr [eax + 0x30]
        or     dx, dx
        jne    loc_1_F804
        test   byte ptr [eax + 0x20], 0x30
        je     loc_1_F804
        cmp    dword ptr [eax + 0x1c], 1
        ja     loc_1_F804
        push   eax
        call   sub_1_DEA4
        add    esp, 4
        jmp    loc_1_F81D
        db  08Dh, 049h, 000h
loc_1_F804:
        mov    ecx, dword ptr [eax + 0x20]
        test   ch, 0x10
        jne    loc_1_F81D
        cmp    dword ptr [eax + 0x1c], 1
        ja     loc_1_F81D
        test   cl, 0x30
        je     loc_1_F81D
        dec    dx
        mov    word ptr [eax + 0x30], dx
loc_1_F81D:
        cmp    ebp, esi
        jne    loc_1_F7DC
loc_1_F821:
        inc    ebx
        add    esi, 8
        cmp    dword ptr [edi], ebx
        jg     loc_1_F7D6
loc_1_F829:
        mov    eax, dword ptr [esp + 0x1c]
        or     word ptr [eax + 0x3c], 1
loc_1_F832:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_F838:
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esi + 0x2c]
        mov    dword ptr [esp + 0xc], eax
        mov    edi, dword ptr [esi + 0x14]
        lea    eax, [esp + 8]
        push   eax
        push   edi
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x18]
        call   sub_1_F8AC
        add    esp, 0x10
        push   edi
        call   sub_1_DF20
        add    esp, 4
        cmp    dword ptr [esp + 8], 0
        je     loc_1_F898
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esi + 0x2c], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 0x14], eax
        push   0xf838                        ; -> loc_1_F838
        push   dword ptr [esp + 0xc]
        push   esi
        call   sub_1_11858
        add    esp, 0xc
        pop    edi
        pop    esi
        add    esp, 8
        ret    
loc_1_F898:
        push   5
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        pop    edi
        pop    esi
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
sub_1_F8AC:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        cmp    ebx, 0x1323c                  ; -> loc_1_1323C
        je     loc_1_F916
        inc    ecx
        mov    ecx, dword ptr [esp + 0x18]
loc_1_F8C0:
        mov    esi, ebx
        mov    ebx, dword ptr [ebx]
        lea    edi, [esi + 0x14]
        or     ecx, ecx
        je     loc_1_F8E0
        mov    eax, dword ptr [ecx + 0x38]
        cmp    eax, 7
        je     loc_1_F8D8
        cmp    eax, 4
        jne    loc_1_F8E0
loc_1_F8D8:
        mov    edx, dword ptr [ecx]
        sub    ecx, ecx
        jmp    loc_1_F8E2
        db  08Bh, 0FFh
loc_1_F8E0:
        mov    edx, dword ptr [edi]
loc_1_F8E2:
        cmp    edx, edi
        je     loc_1_F90E
loc_1_F8E6:
        mov    eax, dword ptr [edx + 0x38]
        cmp    eax, 7
        je     loc_1_F902
        cmp    eax, 4
        je     loc_1_F8F8
        jmp    loc_1_F908
        db  08Dh, 049h, 000h
loc_1_F8F8:
        cmp    eax, 7
        je     loc_1_F902
        cmp    eax, 4
        jne    loc_1_F90E
loc_1_F902:
        cmp    dword ptr [edx + 0x3c], 3
        jl     loc_1_F924
loc_1_F908:
        mov    edx, dword ptr [edx]
        cmp    edx, edi
        jne    loc_1_F8E6
loc_1_F90E:
        cmp    ebx, 0x1323c                  ; -> loc_1_1323C
        jne    loc_1_F8C0
loc_1_F916:
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [eax], 0
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_F924:
        inc    dword ptr [edx + 0x3c]
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax], esi
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_F938:
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        or     esi, esi
        je     loc_1_F95B
        push   0
        push   0xf960                        ; -> loc_1_F960
        push   0
        push   0
        push   0
        call   sub_1_3C00
        add    esp, 0x14
        or     word ptr [esi + 0x3c], 1
loc_1_F95B:
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_F960:
        sub    esp, 0xc
        push   esi
        mov    eax, dword ptr [0x1323c]      ; -> loc_1_1323C
        mov    dword ptr [esp + 0xc], eax
        lea    eax, [esp + 8]
        push   eax
        push   0
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x18]
        call   sub_1_F8AC
        add    esp, 0x10
        cmp    dword ptr [esp + 8], 0
        je     loc_1_FA2C
        push   5
        call   sub_1_001E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_FA38
        lea    eax, [esp + 4]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     loc_1_F9CC
        cmp    eax, 0xff
        je     loc_1_F9CC
        push   esi
        call   sub_1_0024
        add    esp, 4
        pop    esi
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
loc_1_F9CC:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x1c], esi
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x18], 4
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x24], 0
        mov    byte ptr [esi + 1], 0
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 0x2c], eax
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 0x14], eax
        mov    byte ptr [esi], 0x85
        mov    word ptr [esi + 2], 0
        push   0xf838                        ; -> loc_1_F838
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        call   sub_1_11858
        add    esp, 0xc
loc_1_FA2C:
        push   dword ptr [esp + 0x14]
        call   sub_1_0024
        add    esp, 4
loc_1_FA38:
        pop    esi
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
sub_1_FA40:
        sub    esp, 4
        cmp    byte ptr [esp + 0xc], 0
        je     loc_1_FA4F
        add    dword ptr [esp + 8], 2
loc_1_FA4F:
        mov    eax, dword ptr [esp + 8]
        lea    eax, [eax*4 + 0x133f8]        ; -> loc_1_133F8
        mov    dword ptr [esp], eax
        inc    dword ptr [eax]
        add    esp, 4
        ret    
sub_1_FA64:
        push   esi
        push   edi
        mov    eax, dword ptr [0x13408]      ; -> loc_1_13408
        shl    eax, 4
        lea    edi, [eax + 0x1340c]          ; -> loc_1_1340C
        mov    esi, dword ptr [esp + 0xc]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        inc    dword ptr [0x13408]           ; -> loc_1_13408
        cmp    dword ptr [0x13408], 8        ; -> loc_1_13408
        jb     loc_1_FA95
        mov    dword ptr [0x13408], 0        ; -> loc_1_13408
loc_1_FA95:
        pop    edi
        pop    esi
        ret    
sub_1_FA98:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 0x1c]
        push   0xfab4                        ; -> loc_1_FAB4
        push   0
        push   eax
        push   0
        call   sub_1_3C00
        add    esp, 0x14
        ret    
        db  08Bh, 0FFh
loc_1_FAB4:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0xc]
        or     eax, eax
        je     loc_1_FACD
        add    dword ptr [eax + 8], 0xa
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_FACD:
        push   esi
        call   sub_1_0024
        add    esp, 4
        pop    esi
        ret    
sub_1_FAD8:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     loc_1_FAED
        add    dword ptr [eax + 8], 0xa
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_FAED:
        ret    
        db  08Bh, 0FFh
sub_1_FAF0:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     loc_1_FB05
        add    dword ptr [eax + 8], 0xa
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_FB05:
        ret    
        db  08Bh, 0FFh
sub_1_FB08:
        push   edi
        push   0x8a
        call   sub_1_35F0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    loc_1_FB20
        sub    eax, eax
        pop    edi
        ret    
loc_1_FB20:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0x22
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dword ptr [edx + 0x48], eax
        mov    dword ptr [edx + 0x50], eax
        mov    dword ptr [edx + 0x54], eax
        mov    dword ptr [edx + 0x58], eax
        mov    dword ptr [edx + 0x60], eax
        mov    dword ptr [edx + 0x68], eax
        mov    dword ptr [edx + 0x70], eax
        mov    dword ptr [edx + 0x78], eax
        mov    dword ptr [edx + 0x84], 1
        mov    byte ptr [edx + 0x88], 0x10
        lea    eax, [edx + 0x24]
        mov    dword ptr [edx + 0x28], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 0x2c]
        mov    dword ptr [edx + 0x30], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 0x34]
        mov    dword ptr [edx + 0x38], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 0x1c]
        mov    dword ptr [edx + 0x20], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 0x14]
        mov    dword ptr [edx + 0x18], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 0x3c]
        mov    dword ptr [edx + 0x40], eax
        mov    dword ptr [eax], eax
        mov    dword ptr [edx + 0x44], 0
        mov    dword ptr [edx + 0xc], 0
        mov    dword ptr [edx + 8], 0x18273645
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [edx + 0x10], ecx
        add    ecx, 0xc
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 4], edx
        mov    eax, edx
        pop    edi
        ret    
        db  08Bh, 0FFh
sub_1_FBB8:
        sub    esp, 0x38
        push   ebx
        push   esi
        push   edi
        lea    edi, [esp + 0x10]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    ebx, dword ptr [esp + 0x48]
        lea    esi, [ebx + 0x8c]
        lea    edi, [esp + 0x1a]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    eax, dword ptr [ebx + 0x84]
        or     eax, eax
        je     loc_1_FC15
        push   0x10
        lea    ecx, [esp + 0x14]
        push   ecx
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax + 0x14]
        add    eax, 0x34
        push   eax
        call   sub_1_0262
        add    esp, 0xc
        cmp    eax, 0x10
        jne    loc_1_FC0C
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  08Bh, 0FFh
loc_1_FC0C:
        push   ebx
        call   sub_1_FCF4
        add    esp, 4
loc_1_FC15:
        lea    eax, [esp + 0xc]
        push   eax
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        lea    eax, [esp + 0x18]
        push   eax
        mov    eax, dword ptr [0x12f80]      ; -> obj1:0x12F80
        push   dword ptr [eax + 0x80]
        call   sub_1_6954
        add    esp, 0x10
        mov    esi, eax
        or     esi, esi
        je     loc_1_FC84
        cmp    esi, 0xff
        je     loc_1_FC84
        mov    dword ptr [esp + 0x20], 0
        mov    word ptr [esp + 0x30], 2
        lea    esi, [esp + 0x10]
        lea    edi, [esp + 0x32]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        push   0
        push   ebx
        push   0
        push   dword ptr [ebx + 0x58]
        lea    eax, [esp + 0x40]
        push   eax
        lea    eax, [esp + 0x34]
        push   eax
        call   sub_1_A19C
        add    esp, 0x18
        mov    esi, eax
        mov    eax, dword ptr [esp + 0x20]
        jmp    loc_1_FCD3
        db  090h
loc_1_FC84:
        mov    eax, dword ptr [esp + 0xc]
        push   dword ptr [eax + 8]
        call   sub_1_FB08
        add    esp, 4
        or     eax, eax
        jne    loc_1_FCA4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  090h
loc_1_FCA4:
        mov    ecx, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [ecx + 8]
        inc    dword ptr [ecx + 0x18]
        mov    ecx, dword ptr [ebx + 0x58]
        mov    edx, dword ptr [esp + 0xc]
        mov    dword ptr [edx + 0xc], ecx
        mov    dword ptr [eax + 0x80], ebx
        mov    ecx, dword ptr [esp + 0xc]
        mov    edx, dword ptr [ebx + 0x88]
        or     dword ptr [ecx + 0x28], edx
        mov    ecx, dword ptr [esp + 0xc]
        or     dword ptr [ecx + 0x20], 1
loc_1_FCD3:
        or     esi, esi
        je     loc_1_FCDF
        cmp    esi, 0xff
        jne    loc_1_FCEA
loc_1_FCDF:
        mov    dword ptr [ebx + 0x84], eax
        mov    ecx, dword ptr [ebx + 0x44]
        mov    dword ptr [ecx], eax
loc_1_FCEA:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  090h
sub_1_FCF4:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x84]
        or     ecx, ecx
        je     loc_1_FD1E
        mov    dword ptr [eax + 0x84], 0
        mov    eax, dword ptr [eax + 0x44]
        mov    dword ptr [eax], 0
        push   ecx
        call   sub_1_DD58
        add    esp, 4
loc_1_FD1E:
        ret    
        db  090h
sub_1_FD20:
        sub    esp, 4
        push   ebx
        push   esi
        sub    edx, edx
        sub    ecx, ecx
        mov    byte ptr [esp + 8], cl
        mov    eax, dword ptr [esp + 0x10]
        mov    bl, byte ptr [eax]
        inc    eax
        cmp    bl, cl
        je     loc_1_FD78
loc_1_FD38:
        cmp    bl, 0x20
        je     loc_1_FD78
        cmp    bl, 0x2e
        jne    loc_1_FD60
        cmp    edx, 0xff
        jg     loc_1_FDC0
        mov    byte ptr [esp + ecx + 8], dl
        inc    ecx
        cmp    ecx, 3
        ja     loc_1_FDB4
        mov    byte ptr [esp + ecx + 8], 0
        sub    edx, edx
        jmp    loc_1_FD71
        db  08Dh, 049h, 000h
loc_1_FD60:
        sub    bl, 0x30
        cmp    bl, 9
        ja     loc_1_FDCC
        movzx  esi, bl
        lea    edx, [edx + edx*4]
        lea    edx, [esi + edx*2]
loc_1_FD71:
        mov    bl, byte ptr [eax]
        inc    eax
        or     bl, bl
        jne    loc_1_FD38
loc_1_FD78:
        cmp    edx, 0xff
        jg     loc_1_FDD8
        cmp    ecx, 3
        jne    loc_1_FDD8
        mov    byte ptr [esp + ecx + 8], dl
        sub    edx, edx
        sub    cx, cx
loc_1_FD8E:
        movzx  eax, cx
        movzx  eax, byte ptr [esp + eax + 8]
        shl    edx, 8
        add    edx, eax
        inc    cx
        cmp    cx, 4
        jb     loc_1_FD8E
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax], edx
        sub    eax, eax
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_FDB4:
        pop    esi
        mov    eax, 0xc0000001
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_FDC0:
        pop    esi
        mov    eax, 0xc0000001
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_FDCC:
        pop    esi
        mov    eax, 0xc0000001
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_FDD8:
        pop    esi
        mov    eax, 0xc0000001
        pop    ebx
        add    esp, 4
        ret    
        db  090h
sub_1_FDE4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebp, ebp
        mov    ebx, dword ptr [esp + 0x1c]
        cmp    ebx, ebp
        jle    loc_1_FE1A
        mov    si, word ptr [esp + 0x18]
        inc    edi
        mov    edi, dword ptr [esp + 0x14]
loc_1_FDFC:
        push   esi
        call   sub_1_35F0
        add    esp, 4
        or     eax, eax
        je     loc_1_FE24
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edi], eax
        inc    ebp
        cmp    ebp, ebx
        jl     loc_1_FDFC
loc_1_FE1A:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_FE24:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_FE30:
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    edi, dword ptr [esp + 0x14]
        cmp    edi, ebx
        jle    loc_1_FE62
        mov    esi, dword ptr [esp + 0x10]
loc_1_FE41:
        call   sub_1_FE74
        or     eax, eax
        je     loc_1_FE68
        inc    word ptr [0x13244]            ; -> loc_1_13244
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [esi], eax
        inc    ebx
        cmp    edi, ebx
        jg     loc_1_FE41
loc_1_FE62:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_FE68:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_FE74:
        push   esi
        push   edi
        movzx  esi, word ptr [0x132d6]       ; -> loc_1_132D6
        add    esi, 0x76
        push   esi
        call   sub_1_35F0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        je     loc_1_FEEE
        mov    edi, edx
        sub    eax, eax
        mov    ecx, esi
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, esi
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        lea    ecx, [edx + 0x56]
        mov    dword ptr [edx + 0x28], ecx
        mov    eax, dword ptr [0x12f80]      ; -> obj1:0x12F80
        movzx  eax, word ptr [eax + 0xa2]
        add    eax, 8
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [edx + 0x28]
        lea    ecx, [eax + 0x18]
        mov    dword ptr [ecx], 1
        mov    si, word ptr [0x132d6]        ; -> loc_1_132D6
        mov    word ptr [ecx + 4], si
        mov    word ptr [ecx + 6], 2
        mov    word ptr [ecx + 8], 0x89
        mov    dword ptr [ecx + 0xa], 0
        mov    dword ptr [eax + 0x14], ecx
        lea    eax, [edx + 8]
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [eax], eax
loc_1_FEEE:
        mov    eax, edx
        pop    edi
        pop    esi
        ret    
        db  090h
sub_1_FEF4:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi]
        cmp    esi, eax
        jne    loc_1_FF58
        mov    edi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [0x12f80]      ; -> obj1:0x12F80
        mov    cx, word ptr [edi*2 + 0x13244] ; -> loc_1_13244
        cmp    word ptr [eax + edi*2 + 0x1a], cx
        ja     loc_1_FF20
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
loc_1_FF20:
        push   1
        movzx  eax, word ptr [eax + edi*2 + 0x24]
        push   eax
        push   esi
        call   sub_1_FDE4
        add    esp, 0xc
        or     eax, eax
        je     loc_1_FF3C
        cmp    eax, 0xff
        jne    loc_1_FF6C
loc_1_FF3C:
        mov    ecx, dword ptr [esp + 0x10]
        inc    word ptr [edi*2 + 0x13244]    ; -> loc_1_13244
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        jmp    loc_1_FF5E
loc_1_FF58:
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], eax
loc_1_FF5E:
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        sub    eax, eax
loc_1_FF6C:
        pop    edi
        pop    esi
        ret    
        db  090h
sub_1_FF70:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 6], 0x11
        je     loc_1_FF84
        mov    eax, 0xc000000d
        ret    
        db  08Dh, 049h, 000h
loc_1_FF84:
        cmp    word ptr [eax + 4], 0x12
        je     loc_1_FF94
        mov    eax, 0xc000000d
        ret    
        db  08Dh, 049h, 000h
loc_1_FF94:
        lea    edx, [eax + 0xa]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        movzx  ecx, word ptr [eax + 8]
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], ecx
        sub    eax, eax
        ret    
        db  08Bh, 0FFh
sub_1_FFAC:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x10]
        cmp    byte ptr [edx], 0x20
        jne    loc_1_10058
        mov    esi, dword ptr [esp + 0x14]
        cmp    esi, 0x20
        jle    loc_1_10058
        inc    edx
        inc    edi
        mov    edi, 8
loc_1_FFD0:
        mov    ecx, dword ptr [edx]
        sub    ecx, 0x41414141
        add    edx, 4
        mov    ebx, ecx
        and    ebx, 0xf0000
        shr    ebx, 4
        mov    eax, ecx
        and    eax, 0xf00
        shr    eax, 8
        add    ebx, eax
        mov    eax, ecx
        and    eax, 0xf
        shl    eax, 4
        add    ebx, eax
        and    ecx, 0xf000000
        shr    ecx, 0x10
        lea    eax, [ebx + ecx]
        mov    ecx, dword ptr [esp + 0x18]
        mov    word ptr [ecx], ax
        add    dword ptr [esp + 0x18], 2
        dec    edi
        jne    loc_1_FFD0
        sub    ecx, ecx
        mov    eax, edx
        sub    esi, 0x21
        cmp    esi, ecx
        jle    loc_1_1002D
loc_1_10022:
        cmp    byte ptr [eax], 0
        je     loc_1_1002D
        inc    eax
        inc    ecx
        cmp    ecx, esi
        jl     loc_1_10022
loc_1_1002D:
        inc    ecx
        cmp    ecx, 0xef
        jle    loc_1_10040
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_10040:
        add    ecx, 0x20
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [eax], ecx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_10058:
        pop    edi
        mov    eax, 0xc0000001
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_10064:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax], 0x20
        inc    eax
        mov    esi, 0x10
        mov    ecx, dword ptr [esp + 0x14]
loc_1_10078:
        mov    dl, byte ptr [ecx]
        mov    bl, dl
        shr    bl, 4
        add    bl, 0x41
        mov    byte ptr [eax], bl
        inc    eax
        and    dl, 0xf
        add    dl, 0x41
        mov    byte ptr [eax], dl
        inc    ecx
        inc    eax
        dec    esi
        jne    loc_1_10078
        mov    edx, dword ptr [esp + 0x1c]
        cmp    edx, 1
        jbe    loc_1_100B8
        mov    edi, eax
        mov    esi, dword ptr [esp + 0x18]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    eax, edx
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_100B8:
        pop    edi
        mov    byte ptr [eax], 0
        inc    eax
        pop    esi
        pop    ebx
        ret    
sub_1_100C0:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        inc    eax
        sub    ebx, ebx
        mov    ecx, dword ptr [esp + 0x14]
loc_1_100D0:
        mov    dl, byte ptr [ecx]
        cmp    dl, 0x20
        je     loc_1_100E1
        mov    byte ptr [eax], dl
        inc    ecx
        inc    eax
        inc    ebx
        cmp    ebx, 0x10
        jl     loc_1_100D0
loc_1_100E1:
        mov    byte ptr [esi], bl
        mov    esi, dword ptr [esp + 0x18]
        or     esi, esi
        je     loc_1_10126
        mov    edi, 1
loc_1_100F0:
        sub    ebx, ebx
        mov    ecx, eax
        inc    eax
        cmp    byte ptr [esi], 0x2e
        je     loc_1_1010F
loc_1_100FA:
        mov    dl, byte ptr [esi]
        or     dl, dl
        je     loc_1_1010F
        cmp    dl, 0x2c
        je     loc_1_1010F
        mov    byte ptr [eax], dl
        inc    esi
        inc    eax
        inc    ebx
        cmp    byte ptr [esi], 0x2e
        jne    loc_1_100FA
loc_1_1010F:
        mov    byte ptr [ecx], bl
        mov    cl, byte ptr [esi]
        or     cl, cl
        je     loc_1_10120
        cmp    cl, 0x2c
        je     loc_1_10120
        inc    esi
        jmp    loc_1_10122
        db  090h
loc_1_10120:
        sub    edi, edi
loc_1_10122:
        or     edi, edi
        jne    loc_1_100F0
loc_1_10126:
        pop    edi
        mov    byte ptr [eax], 0
        inc    eax
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_10130:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x14]
        movsx  esi, byte ptr [ebx]
        inc    ebx
        mov    edi, dword ptr [esp + 0x1c]
        mov    edx, edi
        sub    ecx, ecx
        cmp    esi, ecx
        je     loc_1_10158
loc_1_10148:
        mov    al, byte ptr [ebx]
        mov    byte ptr [edi], al
        inc    ebx
        cmp    ecx, 0x10
        jge    loc_1_10153
        inc    edi
loc_1_10153:
        inc    ecx
        cmp    esi, ecx
        ja     loc_1_10148
loc_1_10158:
        cmp    esi, 0x10
        jge    loc_1_10177
        mov    ebp, 0x10
        sub    ebp, esi
        mov    eax, 0x20202020
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebp
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
loc_1_10177:
        mov    ecx, 0x10
loc_1_1017C:
        mov    al, byte ptr [edx]
        cmp    al, 0x61
        jl     loc_1_1018A
        cmp    al, 0x7a
        jg     loc_1_1018A
        sub    al, 0x20
        mov    byte ptr [edx], al
loc_1_1018A:
        inc    edx
        dec    ecx
        jne    loc_1_1017C
        cmp    byte ptr [ebx], 0
        je     loc_1_101A3
        inc    eax
        mov    eax, dword ptr [esp + 0x18]
loc_1_10198:
        cmp    esi, eax
        jae    loc_1_101A3
        inc    ebx
        inc    esi
        cmp    byte ptr [ebx], 0
        jne    loc_1_10198
loc_1_101A3:
        inc    esi
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [eax], esi
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_101B4:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 4]
        cmp    ecx, eax
        je     loc_1_101CE
        cmp    byte ptr [ecx], al
        je     loc_1_101CE
loc_1_101C2:
        cmp    byte ptr [ecx], 0x2c
        je     loc_1_101CE
        inc    ecx
        inc    eax
        cmp    byte ptr [ecx], 0
        jne    loc_1_101C2
loc_1_101CE:
        ret    
        db  090h
sub_1_101D0:
        push   esi
        cmp    dword ptr [0x13264], 0x13264  ; -> loc_1_13264
        je     loc_1_10218
        mov    edx, dword ptr [0x13264]      ; -> loc_1_13264
        mov    eax, edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        lea    eax, [edx + 8]
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [eax], eax
        mov    dword ptr [edx + 0x30], 0
        mov    dword ptr [edx + 0x10], 0
        mov    word ptr [edx + 0x40], 0
        mov    word ptr [edx + 0x4c], 0
        jmp    loc_1_10252
        db  08Dh, 049h, 000h
loc_1_10218:
        mov    ax, word ptr [0x13244]        ; -> loc_1_13244
        cmp    word ptr [0x1324e], ax        ; -> loc_1_1324E
        ja     loc_1_10230
        mov    eax, 1
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_10230:
        call   sub_1_FE74
        mov    edx, eax
        or     edx, edx
        jne    loc_1_10244
        mov    eax, 1
        jmp    loc_1_10254
        db  08Bh, 0FFh
loc_1_10244:
        inc    word ptr [0x13244]            ; -> loc_1_13244
        mov    dword ptr [edx + 0x30], 0
loc_1_10252:
        sub    eax, eax
loc_1_10254:
        or     eax, eax
        je     loc_1_1025F
        cmp    eax, 0xff
        jne    loc_1_1027F
loc_1_1025F:
        lea    esi, [edx + 0x4e]
        mov    dword ptr [esi], 0x13380      ; -> loc_1_13380
        mov    ecx, dword ptr [0x13384]      ; -> loc_1_13384
        mov    dword ptr [edx + 0x52], ecx
        mov    ecx, dword ptr [0x13384]      ; -> loc_1_13384
        mov    dword ptr [ecx], esi
        mov    dword ptr [0x13384], esi      ; -> loc_1_13384
loc_1_1027F:
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        pop    esi
        ret    
        db  090h
sub_1_10288:
        push   esi
        sub    eax, eax
        mov    ecx, dword ptr [0x132b4]      ; -> loc_1_132B4
        mov    ecx, dword ptr [ecx]
        cmp    ecx, eax
        jle    loc_1_102BF
        mov    edx, dword ptr [0x132b4]      ; -> loc_1_132B4
        add    edx, 8
loc_1_102A0:
        mov    esi, dword ptr [edx]
        cmp    esi, edx
        je     loc_1_102B9
loc_1_102A6:
        test   byte ptr [esi + 0x20], 0x80
        jne    loc_1_102B3
        cmp    byte ptr [esi + 0x34], 0x2a
        je     loc_1_102B3
        inc    eax
loc_1_102B3:
        mov    esi, dword ptr [esi]
        cmp    esi, edx
        jne    loc_1_102A6
loc_1_102B9:
        add    edx, 8
        dec    ecx
        jne    loc_1_102A0
loc_1_102BF:
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_102C4:
        push   ebx
        push   esi
        push   edi
        sub    eax, eax
        cmp    dword ptr [0x13274], 0x13274  ; -> loc_1_13274
        je     loc_1_10305
        inc    ecx
        mov    ecx, dword ptr [0x13274]      ; -> loc_1_13274
loc_1_102DC:
        lea    edx, [ecx + 0xc]
        mov    edi, dword ptr [edx]
        cmp    edi, edx
        je     loc_1_102FB
loc_1_102E5:
        lea    esi, [edi + 0x14]
        mov    ebx, dword ptr [esi]
        cmp    ebx, esi
        je     loc_1_102F5
loc_1_102EE:
        inc    eax
        mov    ebx, dword ptr [ebx]
        cmp    ebx, esi
        jne    loc_1_102EE
loc_1_102F5:
        mov    edi, dword ptr [edi]
        cmp    edi, edx
        jne    loc_1_102E5
loc_1_102FB:
        mov    ecx, dword ptr [ecx]
        cmp    ecx, 0x13274                  ; -> loc_1_13274
        jne    loc_1_102DC
loc_1_10305:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_1030C:
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        lea    esi, [edi + 0x1c]
        mov    edx, dword ptr [esi]
        mov    eax, edx
        sub    eax, edi
        cmp    eax, 0x1c
        mov    ecx, dword ptr [esp + 0x18]
        je     loc_1_1033C
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [eax], ecx
        mov    dword ptr [edi + 0x20], esi
        mov    dword ptr [esi], esi
        jmp    loc_1_10341
        db  090h
loc_1_1033C:
        mov    dword ptr [ecx], ecx
        mov    dword ptr [ecx + 4], ecx
loc_1_10341:
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 0x10]
        push   edi
        call   sub_1_103A8
        add    esp, 0xc
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 8
        ret    
        db  08Bh, 0FFh, 08Bh, 044h, 024h, 008h, 08Bh, 008h, 08Bh, 054h, 024h, 004h, 089h, 04Ah, 01Ch, 08Bh
        db  040h, 004h, 089h, 042h, 020h, 02Bh, 0C0h, 0C3h, 08Bh, 0FFh
sub_1_10374:
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esi]
        cmp    ebx, esi
        je     loc_1_103A0
loc_1_10384:
        mov    edx, ebx
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ebx, eax
        inc    edi
        push   edx
        call   sub_1_DF20
        add    esp, 4
        cmp    ebx, esi
        jne    loc_1_10384
loc_1_103A0:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_103A8:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ecx, ecx
        mov    edi, dword ptr [esp + 0x20]
        add    edi, 0x14
        mov    eax, dword ptr [edi]
        cmp    eax, edi
        je     loc_1_103C9
loc_1_103BE:
        inc    ecx
        mov    byte ptr [eax + 0x47], 1
        mov    eax, dword ptr [eax]
        cmp    eax, edi
        jne    loc_1_103BE
loc_1_103C9:
        or     ecx, ecx
        je     loc_1_104E4
        lea    eax, [ecx*4]
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_104E4
        mov    eax, dword ptr [edi]
        mov    dword ptr [esp + 0x10], 0
        cmp    eax, edi
        je     loc_1_1040B
loc_1_103F9:
        mov    dword ptr [esi], eax
        add    esi, 4
        inc    dword ptr [eax + 0x3c]
        mov    eax, dword ptr [eax]
        inc    dword ptr [esp + 0x10]
        cmp    eax, edi
        jne    loc_1_103F9
loc_1_1040B:
        mov    eax, dword ptr [esp + 0x10]
        dec    dword ptr [esp + 0x10]
        or     eax, eax
        je     loc_1_104DB
        nop    
loc_1_1041C:
        sub    esi, 4
        mov    edi, dword ptr [esi]
        mov    eax, dword ptr [edi + 0x38]
        cmp    eax, 7
        je     loc_1_1045C
        cmp    eax, 3
        je     loc_1_10461
        or     eax, eax
        jne    loc_1_104C2
        mov    ecx, dword ptr [edi + 0xc]
        or     ecx, ecx
        je     loc_1_104C2
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   dword ptr [esp + 0x28]
        push   ecx
        call   sub_1_AFAC
        add    esp, 0x10
        jmp    loc_1_104C2
        db  08Bh, 0FFh
loc_1_1045C:
        cmp    eax, 3
        jne    loc_1_10464
loc_1_10461:
        inc    dword ptr [edi + 0x3c]
loc_1_10464:
        mov    eax, dword ptr [edi + 0xc]
        mov    ebx, dword ptr [eax + 0x10]
        mov    dword ptr [edi + 0x38], 8
        mov    ebp, eax
        mov    dword ptr [ebp + 0x68], 9
        mov    dword ptr [ebp + 0xc], 0
        mov    dword ptr [edi + 0xc], 0
        push   edi
        push   ebp
        call   sub_1_D61C
        add    esp, 8
        mov    eax, dword ptr [ebx + 0x58]
        or     eax, eax
        je     loc_1_104AE
        push   2
        push   0
        push   0
        push   0
        push   0
        push   dword ptr [ebp + 0x24]
        push   dword ptr [ebx + 0x5c]
        call   eax
        add    esp, 0x1c
loc_1_104AE:
        push   0
        push   0x13368                       ; -> loc_1_13368
        push   2
        push   7
        push   edi
        call   sub_1_C08C
        add    esp, 0x14
loc_1_104C2:
        push   edi
        call   sub_1_DF20
        add    esp, 4
        mov    eax, dword ptr [esp + 0x10]
        dec    dword ptr [esp + 0x10]
        or     eax, eax
        jne    loc_1_1041C
loc_1_104DB:
        push   esi
        call   sub_1_0024
        add    esp, 4
loc_1_104E4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_104EC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 0x12], 0
        cmp    dword ptr [esp + 0x20], 0
        jle    loc_1_10573
        mov    esi, dword ptr [esp + 0x18]
        mov    ebx, dword ptr [esp + 0x24]
loc_1_10509:
        push   0x49
        call   sub_1_001E
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_10580
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, 0x12
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        push   ebx
        push   ebp
        call   sub_1_3630
        add    esp, 8
        or     eax, eax
        je     loc_1_1053A
        cmp    eax, 0xff
        jne    loc_1_10575
loc_1_1053A:
        push   dword ptr [ebx + 0x70]
        push   dword ptr [ebp + 0x10]
        call   sub_1_3690
        add    esp, 8
        or     eax, eax
        je     loc_1_10553
        cmp    eax, 0xff
        jne    loc_1_10575
loc_1_10553:
        mov    dword ptr [ebp + 0x18], ebx
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp], eax
        mov    dword ptr [ebp + 4], esi
        mov    dword ptr [eax + 4], ebp
        mov    dword ptr [esi], ebp
        inc    word ptr [esp + 0x12]
        movzx  eax, word ptr [esp + 0x12]
        cmp    eax, dword ptr [esp + 0x20]
        jl     loc_1_10509
loc_1_10573:
        sub    eax, eax
loc_1_10575:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_10580:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h, 083h, 0ECh, 004h, 056h, 057h, 0A1h
        dd  loc_1_132F0              ; fixup
        db  089h, 044h, 024h, 008h, 00Bh, 0C0h, 074h, 01Ah, 06Ah, 000h, 06Ah, 000h, 0FFh, 074h, 024h, 010h
        db  0E8h, 03Dh, 005h, 000h, 000h, 083h, 0C4h, 00Ch, 0C7h, 005h
        dd  loc_1_132F0              ; fixup
        db  000h, 000h, 000h, 000h, 0A1h
        dd  loc_1_132FC              ; fixup
        db  0A3h
        dd  loc_1_132E0              ; fixup
        db  0C7h, 005h
        dd  loc_1_132E4              ; fixup
        db  008h, 000h, 000h, 000h, 066h, 0C7h, 005h
        dd  loc_1_13314              ; fixup
        db  003h, 000h, 08Dh, 044h, 024h, 008h, 050h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 068h
        dd  loc_1_F5C0               ; fixup
        db  06Ah, 000h, 06Ah, 000h, 0A1h
        dd  loc_1_132FC              ; fixup
        db  0C1h, 0E8h, 003h, 050h, 0E8h, 0BDh, 003h, 000h, 000h, 083h, 0C4h, 020h, 00Bh, 0C0h, 074h, 007h
        db  03Dh, 0FFh, 000h, 000h, 000h, 075h, 06Eh, 02Bh, 0D2h, 08Bh, 044h, 024h, 008h, 0A3h
        dd  loc_1_132F0              ; fixup
        db  0A1h
        dd  loc_1_132B4              ; fixup
        db  039h, 010h, 07Eh, 046h, 02Bh, 0F6h, 090h, 0A1h
        dd  loc_1_132B4              ; fixup
        db  08Dh, 04Ch, 030h, 008h, 08Bh, 079h, 000h, 03Bh, 0F9h, 074h, 026h, 08Bh, 047h, 020h, 0A8h, 010h
        db  074h, 019h, 080h, 07Fh, 034h, 02Ah, 074h, 013h, 0A8h, 001h, 075h, 00Fh, 0C7h, 047h, 02Ch, 000h
        db  000h, 000h, 000h, 0A1h
        dd  loc_1_132FC              ; fixup
        db  089h, 047h, 018h, 08Bh, 03Fh, 03Bh, 0F9h, 075h, 0DAh, 042h, 083h, 0C6h, 008h, 0A1h
        dd  loc_1_132B4              ; fixup
        db  039h, 010h, 07Fh, 0BDh, 0FFh, 035h
        dd  loc_1_132F0              ; fixup
        db  06Ah, 000h, 06Ah, 000h, 0E8h, 04Eh, 0EFh, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 02Bh, 0C0h, 05Fh, 05Eh
        db  083h, 0C4h, 004h, 0C3h, 08Dh, 049h, 000h, 056h, 08Bh, 074h, 024h, 008h, 08Bh, 006h, 00Bh, 0C0h
        db  074h, 013h, 06Ah, 000h, 06Ah, 000h, 050h, 0E8h, 057h, 004h, 000h, 000h, 083h, 0C4h, 00Ch, 0C7h
        db  006h, 000h, 000h, 000h, 000h, 02Bh, 0C0h, 05Eh, 0C3h, 08Bh, 0FFh
sub_1_106A4:
        sub    esp, 4
        cmp    dword ptr [esp + 8], 0
        je     loc_1_106E8
        mov    ecx, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [eax + ecx + 0x1f]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        and    ecx, 0xff
        or     edx, ecx
        jmp    loc_1_106EA
        db  08Dh, 049h, 000h
loc_1_106E8:
        sub    edx, edx
loc_1_106EA:
        sub    cl, cl
        lea    eax, [esp]
        push   eax
        push   0
        push   dword ptr [esp + 0x14]
        push   0
        push   0x10
        push   0x10818                       ; -> loc_1_10818
        push   0
        movzx  ax, cl
        push   eax
        push   edx
        push   dword ptr [esp + 0x40]
        push   dword ptr [esp + 0x40]
        call   sub_1_E088
        add    esp, 0x2c
        sub    eax, eax
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_10720:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x18]
        or     ebx, ebx
        je     loc_1_10808
        mov    ax, word ptr [ebx + 0x3e]
        dec    ax
        mov    word ptr [ebx + 0x3e], ax
        mov    edi, dword ptr [esp + 0x10]
        jne    loc_1_107D4
        mov    ax, word ptr [edi + 0x4c]
        test   al, 2
        je     loc_1_10768
        and    ax, 0xfffd
        mov    word ptr [edi + 0x4c], ax
        or     ax, 4
        mov    word ptr [edi + 0x4c], ax
        mov    ax, word ptr [0x132d0]        ; -> loc_1_132D0
        mov    word ptr [ebx + 0x3e], ax
        jmp    loc_1_107D4
        db  090h
loc_1_10768:
        mov    esi, dword ptr [edi + 0x38]
        mov    dword ptr [ebx + 0x30], 0
        mov    eax, dword ptr [esi + 0x20]
        and    eax, 0xffffff0f
        mov    dword ptr [esi + 0x20], eax
        or     eax, 0x20
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x14], 0
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [esi + 4], esi
        mov    dword ptr [esi], esi
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   esi
        call   sub_1_6608
        add    esp, 8
        or     eax, eax
        je     loc_1_107C0
        cmp    eax, 0xff
        je     loc_1_107C0
        push   esi
        call   sub_1_DEA4
        add    esp, 4
        jmp    loc_1_107C6
        db  08Bh, 0FFh
loc_1_107C0:
        mov    word ptr [esi + 0x30], 1
loc_1_107C6:
        push   edi
        call   sub_1_F280
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_107D4:
        inc    dword ptr [edi + 0x34]
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   edi
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   dword ptr [edi + 0x38]
        call   sub_1_10C0C
        add    esp, 0x24
        push   edi
        call   sub_1_F280
        add    esp, 4
        or     word ptr [ebx + 0x3c], 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_10808:
        push   dword ptr [esp + 0x10]
        call   sub_1_F29C
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_10818:
        ret    
        db  08Dh, 049h, 000h
sub_1_1081C:
        push   0x41
        push   0x133e4                       ; -> loc_1_133E4
        push   dword ptr [esp + 0xc]
        call   sub_1_10830
        add    esp, 0xc
        ret    
sub_1_10830:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 4], eax
        mov    dword ptr [eax], eax
        lea    esi, [eax + 8]
        mov    dword ptr [eax + 0xc], esi
        mov    dword ptr [esi], esi
        mov    edi, dword ptr [esp + 0x10]
        mov    eax, edi
        dec    edi
        or     eax, eax
        je     loc_1_10877
        mov    bx, word ptr [esp + 0x18]
loc_1_10854:
        push   ebx
        call   sub_1_35F0
        add    esp, 4
        or     eax, eax
        je     loc_1_10880
        mov    edx, dword ptr [esi]
        lea    ecx, [eax + 0x18]
        mov    dword ptr [ecx], edx
        mov    dword ptr [eax + 0x1c], esi
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [esi], ecx
        mov    eax, edi
        dec    edi
        or     eax, eax
        jne    loc_1_10854
loc_1_10877:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_10880:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_1088C:
        sub    esp, 8
        mov    ecx, dword ptr [esp + 0xc]
        or     ecx, ecx
        je     loc_1_108C3
        lea    eax, [esp + 4]
        push   eax
        lea    eax, [esp + 4]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0xc
        cmp    dword ptr [esp], 0
        je     loc_1_108C3
        push   0x15
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 8]
        call   sub_1_E64C
        add    esp, 0xc
loc_1_108C3:
        add    esp, 8
        ret    
        db  090h
sub_1_108C8:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x30]
        mov    dword ptr [eax + 0x30], 0
        or     ecx, ecx
        je     loc_1_108F4
        mov    edx, dword ptr [eax + 0x34]
        mov    dword ptr [edx + 0x14], 0
        push   dword ptr [esp + 8]
        push   dword ptr [eax + 0x2c]
        call   ecx
        add    esp, 8
        sub    eax, eax
        ret    
        db  090h
loc_1_108F4:
        mov    eax, 0xc0000001
        ret    
        db  08Bh, 0FFh
sub_1_108FC:
        push   esi
        sub    esi, esi
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [eax]
        cmp    eax, edx
        je     loc_1_10920
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, esi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_10920:
        push   dword ptr [esp + 0xc]
        call   sub_1_35F0
        add    esp, 4
        or     eax, eax
        jne    loc_1_1093C
        mov    esi, 1
        mov    eax, esi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_1093C:
        add    eax, 0x18
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, esi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_1094C:
        sub    esp, 4
        push   esi
        lea    eax, [esp + 4]
        push   eax
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   0
        mov    esi, dword ptr [esp + 0x28]
        push   esi
        push   dword ptr [esp + 0x28]
        call   sub_1_109B8
        add    esp, 0x20
        or     eax, eax
        je     loc_1_10983
        cmp    eax, 0xff
        jne    loc_1_109B2
loc_1_10983:
        cmp    byte ptr [esp + 0x28], 0
        je     loc_1_109A4
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 0x24]
        mov    dword ptr [ecx + 0x34], edx
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [edx + 0x14], ecx
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_109A4:
        mov    edx, dword ptr [esp + 0x24]
        mov    dword ptr [esi + 0x38], edx
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [esi + 0x30], ecx
loc_1_109B2:
        pop    esi
        add    esp, 4
        ret    
        db  090h
sub_1_109B8:
        sub    esp, 4
        push   esi
        push   edi
        lea    eax, [esp + 8]
        push   eax
        push   0x41
        push   0x133ec                       ; -> loc_1_133EC
        call   sub_1_108FC
        add    esp, 0xc
        mov    esi, eax
        or     esi, esi
        je     loc_1_109E3
        cmp    esi, 0xff
        jne    loc_1_10A83
loc_1_109E3:
        mov    edi, dword ptr [esp + 8]
        sub    edi, 0x18
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [edi + 0x38], ecx
        mov    byte ptr [edi + 0x40], 1
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [edi + 0x20], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [edi + 0x24], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [edi + 0x28], eax
        mov    word ptr [edi + 0x3c], 0
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [edi + 0x2c], eax
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [edi + 0x30], eax
        mov    ax, word ptr [esp + 0x28]
        mov    word ptr [edi + 0x3e], ax
        mov    dword ptr [edi + 0xc], 0
        mov    dword ptr [edi + 0x14], 0
        mov    eax, dword ptr [0x12e68]      ; -> loc_1_12E68
        mov    dword ptr [edi + 0x10], eax
        push   edi
        push   0x10b80                       ; -> loc_1_10B80
        push   ecx
        push   edi
        call   0
        add    esp, 0x10
        mov    eax, dword ptr [esp + 0x2c]
        or     eax, eax
        je     loc_1_10A57
        mov    dword ptr [eax], edi
loc_1_10A57:
        mov    eax, dword ptr [0x133e4]      ; -> loc_1_133E4
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 4], 0x133e4  ; -> loc_1_133E4
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [0x133e4]      ; -> loc_1_133E4
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [0x133e4], eax      ; -> loc_1_133E4
loc_1_10A83:
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  090h
sub_1_10A8C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x14]
        cmp    byte ptr [ebx + 0x40], 0
        jne    loc_1_10AE0
        mov    ecx, dword ptr [ebx + 0x28]
        mov    esi, dword ptr [ebx + 0x20]
        mov    edi, dword ptr [ebx + 0x24]
        lea    edx, [ebx + 0x18]
        mov    ebp, dword ptr [edx]
        mov    eax, dword ptr [ebx + 0x1c]
        mov    dword ptr [eax], ebp
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [edx], 0x133ec      ; -> loc_1_133EC
        mov    eax, dword ptr [0x133f0]      ; -> loc_1_133F0
        mov    dword ptr [ebx + 0x1c], eax
        mov    eax, dword ptr [0x133f0]      ; -> loc_1_133F0
        mov    dword ptr [eax], edx
        mov    dword ptr [0x133f0], edx      ; -> loc_1_133F0
        or     ecx, ecx
        je     loc_1_10AD8
        push   0
        push   edi
        push   esi
        call   ecx
        add    esp, 0xc
loc_1_10AD8:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_10AE0:
        pop    ebp
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_10AEC:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        or     edi, edi
        je     loc_1_10AFC
        mov    dword ptr [edi], 0
loc_1_10AFC:
        mov    esi, dword ptr [esp + 0xc]
        mov    cl, byte ptr [esi + 0x40]
        cmp    cl, 1
        jne    loc_1_10B44
        test   byte ptr [esi + 0x3c], 8
        jne    loc_1_10B17
        push   esi
        call   6
        add    esp, 4
loc_1_10B17:
        or     edi, edi
        je     loc_1_10B20
        mov    eax, dword ptr [esi + 0x30]
        mov    dword ptr [edi], eax
loc_1_10B20:
        mov    edx, dword ptr [esp + 0x14]
        or     edx, edx
        je     loc_1_10B2D
        mov    eax, dword ptr [esi + 0x2c]
        mov    dword ptr [edx], eax
loc_1_10B2D:
        push   esi
        mov    dword ptr [esi + 0x30], 0
        mov    byte ptr [esi + 0x40], 0
        call   sub_1_10A8C
        add    esp, 4
        pop    edi
        pop    esi
        ret    
loc_1_10B44:
        cmp    cl, 2
        jne    loc_1_10B78
        mov    eax, dword ptr [esi + 0x30]
        or     eax, eax
        je     loc_1_10B6A
        or     edi, edi
        je     loc_1_10B56
        mov    dword ptr [edi], eax
loc_1_10B56:
        mov    edx, dword ptr [esp + 0x14]
        or     edx, edx
        je     loc_1_10B63
        mov    eax, dword ptr [esi + 0x2c]
        mov    dword ptr [edx], eax
loc_1_10B63:
        mov    dword ptr [esi + 0x30], 0
loc_1_10B6A:
        inc    cl
        mov    byte ptr [esi + 0x40], cl
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_10B78:
        pop    edi
        mov    eax, 0xc0000001
        pop    esi
        ret    
loc_1_10B80:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    al, byte ptr [esi + 0x40]
        or     al, al
        je     loc_1_10C08
        inc    al
        mov    byte ptr [esi + 0x40], al
        and    word ptr [esi + 0x3c], 0xfffe
        push   esi
        push   dword ptr [esi + 0x24]
        push   dword ptr [esi + 0x20]
        call   dword ptr [esi + 0x28]
        add    esp, 0xc
        mov    al, byte ptr [esi + 0x40]
        dec    al
        mov    byte ptr [esi + 0x40], al
        test   byte ptr [esi + 0x3c], 1
        je     loc_1_10BDC
        cmp    al, 2
        jne    loc_1_10BC8
        mov    byte ptr [esi + 0x40], 0
        push   esi
        call   sub_1_10A8C
        add    esp, 4
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_10BC8:
        push   esi
        push   0x10b80                       ; -> loc_1_10B80
        push   dword ptr [esi + 0x38]
        push   esi
        call   0
        add    esp, 0x10
        pop    esi
        ret    
loc_1_10BDC:
        lea    edx, [esi + 0x18]
        mov    byte ptr [esi + 0x40], 0
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edx], 0x133ec      ; -> loc_1_133EC
        mov    eax, dword ptr [0x133f0]      ; -> loc_1_133F0
        mov    dword ptr [esi + 0x1c], eax
        mov    eax, dword ptr [0x133f0]      ; -> loc_1_133F0
        mov    dword ptr [eax], edx
        mov    dword ptr [0x133f0], edx      ; -> loc_1_133F0
loc_1_10C08:
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_10C0C:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x24]
        test   byte ptr [ebx + 0x20], 0xc
        mov    di, 1
        jne    loc_1_10C24
        sub    di, di
loc_1_10C24:
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     loc_1_10C4C
        cmp    eax, 0xff
        je     loc_1_10C4C
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_10C4C:
        mov    ebp, dword ptr [esp + 0x2c]
        push   ebp
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   dword ptr [esp + 0x50]
        push   edi
        push   0
        push   dword ptr [esp + 0x40]
        lea    eax, [ebx + 0x34]
        push   eax
        call   sub_1_10E60
        add    esp, 0x20
        mov    esi, eax
        or     esi, esi
        je     loc_1_10E50
        cmp    dword ptr [esp + 0x44], 7
        jne    loc_1_10C94
        mov    ax, word ptr [0x13358]        ; -> loc_1_13358
        mov    ecx, dword ptr [esp + 0x14]
        mov    word ptr [ecx + 2], ax
        jmp    loc_1_10CA4
        db  090h
loc_1_10C94:
        cmp    dword ptr [esp + 0x44], 0
        je     loc_1_10CA2
        cmp    dword ptr [esp + 0x44], 1
        jne    loc_1_10CA4
loc_1_10CA2:
        sub    esi, esi
loc_1_10CA4:
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x38], ebx
        mov    eax, dword ptr [ebp + 0x2c]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0x2c], eax
        mov    dword ptr [ebp + 0x38], ebx
        mov    eax, dword ptr [esp + 0x14]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp + 0x40], ax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x30], esi
        mov    eax, dword ptr [esp + 0x40]
        or     eax, eax
        je     loc_1_10D62
        lea    ecx, [esp + 0x18]
        push   ecx
        push   dword ptr [esp + 0x40]
        push   dword ptr [esp + 0x40]
        push   dword ptr [esp + 0x40]
        push   dword ptr [esp + 0x40]
        push   0
        push   ebp
        push   eax
        call   sub_1_109B8
        add    esp, 0x20
        or     eax, eax
        je     loc_1_10D54
        cmp    eax, 0xff
        je     loc_1_10D54
        push   dword ptr [esp + 0x14]
        call   sub_1_0024
        add    esp, 4
        push   4
        push   dword ptr [esp + 0x14]
        call   sub_1_D5C0
        add    esp, 8
        mov    eax, 0xc00000f4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_10D54:
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0x34], ebx
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ebx + 0x14], eax
loc_1_10D62:
        mov    ecx, dword ptr [ebp + 0x2c]
        mov    dx, word ptr [ebp + 0x4c]
        test   dl, 1
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x1c]
        je     loc_1_10D84
        or     byte ptr [eax + 3], 0x10
        mov    di, 0x89
        mov    edx, dword ptr [ecx + 0x60]
        jmp    loc_1_10DEE
        db  08Bh, 0FFh
loc_1_10D84:
        and    byte ptr [eax + 3], 0xef
        cmp    byte ptr [0x13350], 0         ; -> loc_1_13350
        je     loc_1_10DA8
        cmp    dword ptr [esp + 0x44], 3
        jne    loc_1_10DA8
        or     di, di
        jne    loc_1_10DA8
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x1c]
        or     byte ptr [eax + 2], 0x78
loc_1_10DA8:
        test   dl, 2
        mov    di, word ptr [0x13310]        ; -> loc_1_13310
        je     loc_1_10DBC
        mov    edx, dword ptr [ecx + 0x74]
        jmp    loc_1_10DE1
        db  08Dh, 049h, 000h
loc_1_10DBC:
        test   dl, 4
        je     loc_1_10DC8
        mov    edx, dword ptr [ecx + 0x78]
        jmp    loc_1_10DE1
        db  08Bh, 0FFh
loc_1_10DC8:
        test   dl, 0x80
        je     loc_1_10DD4
        mov    edx, dword ptr [ecx + 0x7c]
        jmp    loc_1_10DDA
        db  08Bh, 0FFh
loc_1_10DD4:
        mov    edx, dword ptr [ecx + 0x80]
loc_1_10DDA:
        mov    di, word ptr [0x13312]        ; -> loc_1_13312
loc_1_10DE1:
        mov    eax, dword ptr [esp + 0x14]
        cmp    byte ptr [eax + 7], 0xff
        jne    loc_1_10DEE
        mov    edx, dword ptr [ecx + 0x58]
loc_1_10DEE:
        or     esi, esi
        je     loc_1_10E20
        mov    eax, dword ptr [ecx + 0x58]
        mov    ebx, eax
        and    ebx, 0xff00
        mov    ebp, eax
        shl    ebp, 0x10
        or     ebx, ebp
        shl    ebx, 8
        mov    ebp, eax
        and    ebp, 0xff0000
        shr    ebp, 8
        or     ebx, ebp
        shr    eax, 0x18
        and    eax, 0xff
        or     ebx, eax
        mov    dword ptr [esi], ebx
loc_1_10E20:
        mov    eax, 0
        cmp    dword ptr [ecx + 0x58], 0
        je     loc_1_10E2E
        mov    eax, dword ptr [ecx + 0x68]
loc_1_10E2E:
        push   0x10
        push   edi
        push   dword ptr [esp + 0x18]
        push   0x110b8                       ; -> loc_1_110B8
        push   eax
        push   edx
        push   dword ptr [esp + 0x28]
        call   sub_1_115D4
        add    esp, 0x1c
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_10E50:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
sub_1_10E60:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x2c]
        cmp    esi, 1
        jne    loc_1_10E90
        mov    eax, dword ptr [esp + 0x38]
        push   dword ptr [eax + 0x3c]
        call   sub_1_101B4
        add    esp, 4
        inc    eax
        mov    dword ptr [esp + 0x10], eax
        cmp    eax, 1
        jbe    loc_1_10EA3
        inc    dword ptr [esp + 0x10]
        jmp    loc_1_10EA3
        db  090h
loc_1_10E90:
        sub    eax, eax
        mov    edi, dword ptr [esp + 0x20]
        mov    ecx, 0xffffffff
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    dword ptr [esp + 0x10], ecx
loc_1_10EA3:
        mov    ebp, dword ptr [esp + 0x10]
        add    ebp, 0x43
        or     esi, esi
        jne    loc_1_10EB4
        sub    ebp, 0x12
        jmp    loc_1_10EBC
        db  090h
loc_1_10EB4:
        cmp    esi, 1
        jne    loc_1_10EBC
        sub    ebp, 0x22
loc_1_10EBC:
        movzx  eax, bp
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_10ED8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_10ED8:
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebp
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    eax, dword ptr [esp + 0x38]
        mov    ax, word ptr [eax + 0x40]
        or     ax, ax
        je     loc_1_10EFC
        mov    word ptr [ebx], ax
        jmp    loc_1_10F25
loc_1_10EFC:
        mov    ah, byte ptr [0x1330e]        ; -> loc_1_1330E
        mov    al, byte ptr [0x1330f]        ; -> obj1:0x1330F
        mov    word ptr [ebx], ax
        inc    word ptr [0x1330e]            ; -> loc_1_1330E
        cmp    word ptr [0x1330e], 0xffff    ; -> loc_1_1330E
        jne    loc_1_10F25
        mov    word ptr [0x1330e], 0         ; -> loc_1_1330E
loc_1_10F25:
        mov    byte ptr [ebx + 5], 1
        mov    byte ptr [ebx + 7], 0
        mov    byte ptr [ebx + 9], 0
        cmp    esi, 1
        je     loc_1_10F58
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        lea    eax, [ebx + 0xc]
        push   eax
        call   sub_1_10064
        add    esp, 0x10
        mov    dword ptr [eax], 0x1002000
        jmp    loc_1_10F5C
        db  08Bh, 0FFh
loc_1_10F58:
        mov    eax, dword ptr [esp + 0x14]
loc_1_10F5C:
        cmp    esi, 7
        mov    ecx, dword ptr [esp + 0x30]
        mov    dword ptr [ecx], ebx
        mov    ecx, dword ptr [esp + 0x34]
        mov    dword ptr [ecx], ebp
        ja     loc_1_11066
        jmp    dword ptr [esi*4 + 0x11074]   ; -> obj1:0x11074
loc_1_10F78:
        test   byte ptr [0x13364], 1         ; -> loc_1_13364
        je     loc_1_10F98
        mov    word ptr [ebx + 2], 0x1001
        mov    byte ptr [ebx + 0xb], 0
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_10F98:
        mov    eax, ebx
        mov    word ptr [ebx + 2], 1
        mov    byte ptr [ebx + 0xb], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_10FAC:
        mov    eax, dword ptr [esp + 0x38]
        push   dword ptr [eax + 0x3c]
        push   dword ptr [esp + 0x20]
        lea    eax, [ebx + 0xc]
        push   eax
        call   sub_1_100C0
        add    esp, 0xc
        mov    dword ptr [eax], 0x1000100
        mov    word ptr [ebx + 2], 1
        mov    byte ptr [ebx + 0xb], 0
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_10FE0:
        cmp    esi, 4
        mov    word ptr [ebx + 2], 0x28
        je     loc_1_10FF1
        mov    word ptr [ebx + 2], 0x29
loc_1_10FF1:
        mov    dword ptr [eax + 0xa], 0xe0930400
loc_1_10FF8:
        cmp    esi, 6
        jne    loc_1_1100A
        mov    word ptr [ebx + 2], 0x30
        mov    dword ptr [eax + 0xa], 0
loc_1_1100A:
        mov    byte ptr [ebx + 0xb], 1
        mov    byte ptr [eax + 4], 0xc0
        mov    byte ptr [eax + 5], 0xc
        mov    dword ptr [eax + 6], 0x1002000
        mov    word ptr [eax + 0xe], 0x600
        mov    cx, word ptr [esp + 0x28]
        shl    cx, 0xf
        or     cx, word ptr [0x1330c]        ; -> loc_1_1330C
        mov    dh, cl
        mov    dl, ch
        mov    word ptr [eax + 0x10], dx
        mov    ecx, dword ptr [esp + 0x24]
        mov    ebx, ecx
        and    ebx, 0xff00
        mov    edx, ecx
        shl    edx, 0x10
        or     ebx, edx
        shl    ebx, 8
        mov    edx, ecx
        sar    edx, 0x18
        mov    bl, dl
        and    ecx, 0xff0000
        shr    ecx, 8
        or     ebx, ecx
        mov    dword ptr [eax + 0x12], ebx
loc_1_11066:
        pop    ebp
        add    eax, 0x12
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
        ; ---- dispatch table (8 entries) ----
        dd  loc_1_10F78
        dd  loc_1_10FAC
        dd  loc_1_11066
        dd  loc_1_10FE0
        dd  loc_1_10FE0
        dd  loc_1_11066
        dd  loc_1_10FF8
        dd  loc_1_10FE0
        db  08Bh, 044h, 024h, 004h, 066h, 081h, 060h, 04Ch, 07Fh, 0FFh, 050h, 0E8h, 0DCh, 0E1h, 0FFh, 0FFh
        db  083h, 0C4h, 004h, 0C3h
loc_1_110A8:
        push   dword ptr [esp + 4]
        call   sub_1_0024
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_110B8:
        push   5
        push   dword ptr [esp + 8]
        call   sub_1_D5C0
        add    esp, 8
        ret    
        db  090h
sub_1_110C8:
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 0x13], 1
        sub    esi, esi
        mov    byte ptr [esp + 0x12], 1
        mov    ecx, dword ptr [esp + 0x48]
        mov    edx, dword ptr [ecx + 2]
        mov    edi, edx
        and    edi, 0xff00
        mov    eax, edx
        shl    eax, 0x10
        or     edi, eax
        shl    edi, 8
        mov    eax, edx
        and    eax, 0xff0000
        shr    eax, 8
        or     edi, eax
        shr    edx, 0x18
        and    edx, 0xff
        or     edi, edx
        mov    dword ptr [esp + 0x30], edi
        cmp    byte ptr [0x13350], 0         ; -> loc_1_13350
        mov    ax, word ptr [ecx]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [esp + 0x14], cx
        je     loc_1_1115E
        cmp    byte ptr [0x13351], 0         ; -> obj1:0x13351
        jne    loc_1_1115E
        cmp    dword ptr [esp + 0x54], 2
        jne    loc_1_1115E
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x34]
        call   sub_1_9D64
        add    esp, 8
        or     al, al
        je     loc_1_1115E
        mov    byte ptr [esp + 0x13], 0
        movzx  eax, byte ptr [0x1334f]       ; -> obj1:0x1334F
        lea    eax, [eax + eax*2]
        lea    esi, [eax*2 - 6]
loc_1_1115E:
        mov    eax, dword ptr [esp + 0x3c]
        lea    eax, [esi + eax + 0x1d]
        mov    dword ptr [esp + 0x2c], eax
        and    eax, 0xffff
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esp + 0x18], eax
        or     eax, eax
        jne    loc_1_11190
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  08Dh, 049h, 000h
loc_1_11190:
        sub    eax, eax
        mov    edx, dword ptr [esp + 0x2c]
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    ecx, dword ptr [esp + 0x18]
        mov    byte ptr [ecx + 5], al
        mov    byte ptr [ecx + 7], 1
        mov    eax, dword ptr [esp + 0x3c]
        lea    esi, [ecx + eax + 0xd]
        mov    dword ptr [esp + 0x20], esi
        mov    dword ptr [esi], 0x1002000
        mov    dword ptr [esi + 4], 0
        mov    word ptr [esi + 8], 0x600
        mov    eax, esi
        add    eax, 0xa
        mov    ch, byte ptr [0x1330c]        ; -> loc_1_1330C
        mov    cl, byte ptr [0x1330d]        ; -> obj1:0x1330D
        mov    word ptr [eax], cx
        mov    edx, dword ptr [esp + 0x44]
        or     edx, edx
        je     loc_1_11200
        mov    ecx, dword ptr [edx + 0x20]
        and    ecx, 0xc
        or     ecx, ecx
        mov    cx, 0
        setne  cl
        jmp    loc_1_11205
        db  090h
loc_1_11200:
        mov    cx, word ptr [esp + 0x16]
loc_1_11205:
        shl    cx, 0xf
        or     cx, word ptr [0x1330c]        ; -> loc_1_1330C
        mov    bh, cl
        mov    bl, ch
        mov    word ptr [esp + 0x28], bx
        mov    si, bx
        mov    word ptr [eax], si
        sub    ebx, ebx
        mov    ecx, dword ptr [esp + 0x50]
        mov    bh, cl
        mov    bl, ch
        mov    ecx, dword ptr [esp + 0x54]
        cmp    ecx, 2
        je     loc_1_112AC
        cmp    ecx, 5
        jl     loc_1_11240
        cmp    ecx, 6
        jle    loc_1_114DC
loc_1_11240:
        mov    ecx, dword ptr [esp + 0x34]
loc_1_11244:
        or     ecx, ecx
        je     loc_1_11272
loc_1_11248:
        mov    edx, ecx
        shl    edx, 0x10
        mov    eax, ecx
        and    eax, 0xff00
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        sar    eax, 0x18
        mov    dl, al
        and    ecx, 0xff0000
        shr    ecx, 8
        or     edx, ecx
        mov    esi, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0xc], edx
loc_1_11272:
        lea    eax, [esp + 0x24]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     loc_1_11568
        cmp    eax, 0xff
        je     loc_1_11568
        push   dword ptr [esp + 0x18]
        call   sub_1_0024
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  090h
loc_1_112AC:
        mov    ecx, dword ptr [esp + 0x18]
        mov    word ptr [ecx + 2], 0x85
        mov    esi, dword ptr [esp + 0x40]
        mov    di, word ptr [esi]
        mov    word ptr [ecx], di
        mov    ebx, dword ptr [esp + 0x3c]
        inc    ebx
        add    esi, 0xc
        mov    edi, ecx
        add    edi, 0xc
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        or     edx, edx
        jne    loc_1_112FC
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x20]
        or     word ptr [ecx + 2], 0x300
        mov    word ptr [esi + 8], 0
        sub    ecx, ecx
        jmp    loc_1_11244
        db  08Bh, 0FFh
loc_1_112FC:
        mov    di, word ptr [0x13364]        ; -> loc_1_13364
        and    di, 0x10
        jne    loc_1_11358
        mov    ecx, dword ptr [edx + 0x28]
        mov    esi, dword ptr [esp + 0x4c]
        test   dword ptr [esi + 0x88], ecx
        jne    loc_1_11358
        mov    cx, word ptr [esp + 0x14]
        cmp    cx, word ptr [0x13310]        ; -> loc_1_13310
        jne    loc_1_1132F
        mov    ecx, dword ptr [esp + 0x30]
        cmp    dword ptr [esi + 0x74], ecx
        je     loc_1_11358
loc_1_1132F:
        mov    esi, dword ptr [esp + 0x4c]
        mov    ecx, dword ptr [esp + 0x30]
        cmp    dword ptr [esi + 0x78], ecx
        je     loc_1_11358
        push   dword ptr [esp + 0x18]
        call   sub_1_0024
        add    esp, 4
        mov    eax, 0xc0000001
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  08Dh, 049h, 000h
loc_1_11358:
        or     di, di
        mov    esi, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 4], 0xe0930400
        je     loc_1_113B4
        mov    byte ptr [esp + 0x12], 0
        test   byte ptr [edx + 0x20], 8
        je     loc_1_11384
        mov    esi, dword ptr [edx + 0xc]
        mov    ecx, dword ptr [esi + 4]
        cmp    ecx, -1
        jne    loc_1_113A7
        mov    ecx, dword ptr [esi]
        jmp    loc_1_113A7
        db  08Bh, 0FFh
loc_1_11384:
        cmp    dword ptr [edx + 0x24], 0xdead0000
        jne    loc_1_113A4
        cmp    byte ptr [0x13350], 0         ; -> loc_1_13350
        je     loc_1_113A4
        mov    byte ptr [esp + 0x12], 1
        mov    ecx, dword ptr [esp + 0x34]
        jmp    loc_1_113A7
        db  08Dh, 049h, 000h
loc_1_113A4:
        mov    ecx, dword ptr [edx + 0xc]
loc_1_113A7:
        or     ecx, ecx
        jne    loc_1_113B8
        mov    esi, dword ptr [esp + 0x4c]
        mov    ecx, dword ptr [esi + 0x60]
        jmp    loc_1_113B8
loc_1_113B4:
        mov    ecx, dword ptr [esp + 0x34]
loc_1_113B8:
        cmp    byte ptr [esp + 0x12], 0
        je     loc_1_11244
        cmp    byte ptr [esp + 0x13], 0
        je     loc_1_11428
        cmp    byte ptr [0x13352], 0         ; -> loc_1_13352
        je     loc_1_1141C
        sub    esi, esi
        sub    ecx, ecx
        nop    
loc_1_113D8:
        cmp    esi, 5
        jae    loc_1_11406
        inc    esi
        call   sub_1_000C
        shr    eax, 4
        movzx  ecx, byte ptr [0x1334f]       ; -> obj1:0x1334F
        sub    edx, edx
        div    ecx
        mov    eax, dword ptr [0x1323c]      ; -> loc_1_1323C
        or     edx, edx
        jle    loc_1_113FF
loc_1_113FA:
        mov    eax, dword ptr [eax]
        dec    edx
        jne    loc_1_113FA
loc_1_113FF:
        mov    ecx, dword ptr [eax + 0x58]
        or     ecx, ecx
        je     loc_1_113D8
loc_1_11406:
        or     ecx, ecx
        jne    loc_1_11248
        mov    esi, dword ptr [esp + 0x4c]
        mov    ecx, dword ptr [esi + 0x58]
        jmp    loc_1_11244
        db  08Bh, 0FFh
loc_1_1141C:
        mov    esi, dword ptr [esp + 0x4c]
        mov    ecx, dword ptr [esi + 0x58]
        jmp    loc_1_11244
loc_1_11428:
        sub    ecx, ecx
        mov    dword ptr [esp + 0x1c], 0
        mov    edi, dword ptr [0x1323c]      ; -> loc_1_1323C
        cmp    edi, 0x1323c                  ; -> loc_1_1323C
        je     loc_1_1149A
loc_1_11440:
        mov    esi, dword ptr [edx + 0x28]
        test   dword ptr [edi + 0x88], esi
        je     loc_1_11490
        cmp    dword ptr [edi + 0x58], 0
        je     loc_1_11490
        mov    si, word ptr [esp + 0x28]
        mov    word ptr [eax], si
        mov    esi, dword ptr [edi + 0x58]
        mov    ebp, esi
        and    ebp, 0xff00
        mov    ebx, esi
        shl    ebx, 0x10
        or     ebp, ebx
        shl    ebp, 8
        mov    ebx, esi
        and    ebx, 0xff0000
        shr    ebx, 8
        or     ebp, ebx
        shr    esi, 0x18
        and    esi, 0xff
        or     ebp, esi
        mov    dword ptr [eax + 2], ebp
        inc    dword ptr [esp + 0x1c]
        add    eax, 6
loc_1_11490:
        mov    edi, dword ptr [edi]
        cmp    edi, 0x1323c                  ; -> loc_1_1323C
        jne    loc_1_11440
loc_1_1149A:
        movzx  eax, byte ptr [0x1334f]       ; -> obj1:0x1334F
        cmp    eax, dword ptr [esp + 0x1c]
        je     loc_1_114BB
        mov    edx, dword ptr [esp + 0x1c]
        sub    edx, eax
        lea    edx, [edx + edx*2]
        mov    eax, dword ptr [esp + 0x2c]
        lea    eax, [eax + edx*2]
        mov    dword ptr [esp + 0x2c], eax
loc_1_114BB:
        mov    eax, dword ptr [esp + 0x1c]
        lea    edx, [eax + eax*2]
        add    edx, edx
        mov    dl, dh
        imul   ax, ax, 0x600
        mov    dh, ah
        mov    esi, dword ptr [esp + 0x20]
        mov    word ptr [esi + 8], dx
        jmp    loc_1_11244
        db  08Bh, 0FFh
loc_1_114DC:
        mov    eax, dword ptr [esp + 0x3c]
        add    eax, 0xd
        mov    edi, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x40]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cmp    dword ptr [esp + 0x54], 6
        jne    loc_1_11508
        or     bx, 0xb4
        jmp    loc_1_1150D
        db  090h
loc_1_11508:
        or     bx, 0x80ad
loc_1_1150D:
        mov    ecx, dword ptr [esp + 0x18]
        mov    word ptr [ecx + 2], bx
        mov    ecx, dword ptr [esp + 0x18]
        mov    byte ptr [ecx + 5], 0
        mov    byte ptr [ecx + 7], 1
        mov    byte ptr [ecx + 0xb], 0
        mov    byte ptr [ecx + 9], 0
        or     edx, edx
        je     loc_1_11560
        mov    eax, dword ptr [edx + 0x20]
        test   al, 8
        je     loc_1_11540
        mov    eax, dword ptr [edx + 0xc]
        mov    ecx, dword ptr [eax]
        jmp    loc_1_11244
        db  08Bh, 0FFh
loc_1_11540:
        mov    ecx, dword ptr [edx + 0xc]
        or     ecx, ecx
        jne    loc_1_11248
        test   al, 4
        je     loc_1_11244
        mov    esi, dword ptr [esp + 0x4c]
        mov    ecx, dword ptr [esi + 0x58]
        jmp    loc_1_11244
        db  090h
loc_1_11560:
        sub    ecx, ecx
        jmp    loc_1_11244
        db  090h
loc_1_11568:
        mov    ecx, dword ptr [esp + 0x24]
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [esp + 0x24]
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0x1c], ecx
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [eax + 0x24], 0
        push   0x10
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x2c]
        push   0x115c4                       ; -> loc_1_115C4
        mov    esi, dword ptr [esp + 0x5c]
        push   dword ptr [esi + 0x68]
        push   dword ptr [esp + 0x44]
        push   dword ptr [esp + 0x3c]
        call   sub_1_115D4
        add    esp, 0x1c
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  08Bh, 0FFh
loc_1_115C4:
        push   5
        push   dword ptr [esp + 8]
        call   sub_1_D5C0
        add    esp, 8
        ret    
        db  090h
sub_1_115D4:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x2c]
        cmp    eax, 0x7f000000
        je     loc_1_116A0
        mov    edx, dword ptr [esp + 0x30]
        or     edx, edx
        je     loc_1_116A0
        mov    esi, dword ptr [esp + 0x28]
        mov    ecx, dword ptr [esi + 0x28]
        or     eax, eax
        jne    loc_1_11607
        mov    eax, dword ptr [esi + 0x2c]
        mov    eax, dword ptr [eax + 0x60]
loc_1_11607:
        lea    ebp, [esi + 0x18]
        mov    dword ptr [esp + 0x14], edx
        mov    edx, dword ptr [esp + 0x34]
        mov    dword ptr [esp + 0x18], edx
        mov    edx, dword ptr [esp + 0x38]
        mov    dword ptr [esp + 0x1c], edx
        mov    edi, dword ptr [esi + 0x20]
        mov    edx, dword ptr [0x12f80]      ; -> obj1:0x12F80
        add    edi, dword ptr [ebp]
        mov    bx, word ptr [edx + 0xa2]
        movzx  edx, bx
        add    edx, 8
        mov    dword ptr [ecx + 0x10], edx
        mov    esi, dword ptr [ecx + 0x14]
        mov    dword ptr [esi], 1
        mov    word ptr [esi + 4], bx
        mov    word ptr [esi + 6], 2
        mov    bx, word ptr [esp + 0x3c]
        mov    dh, bl
        mov    dl, bh
        mov    word ptr [esi + 8], dx
        mov    edx, eax
        and    edx, 0xff00
        mov    ebx, eax
        shl    ebx, 0x10
        or     edx, ebx
        shl    edx, 8
        mov    ebx, eax
        sar    ebx, 0x18
        mov    dl, bl
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [esi + 0xa], edx
        push   0x10
        push   ebp
        lea    eax, [esp + 0x18]
        push   eax
        push   edi
        push   ecx
        lea    eax, [esp + 0x28]
        push   eax
        call   sub_1_3880
        add    esp, 0x18
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  090h
loc_1_116A0:
        mov    edx, dword ptr [esp + 0x34]
        or     edx, edx
        je     loc_1_116B5
        push   0
        push   0
        push   dword ptr [esp + 0x40]
        call   edx
        add    esp, 0xc
loc_1_116B5:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  090h
sub_1_116C0:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x24]
        mov    eax, dword ptr [edi + 0x28]
        mov    esi, dword ptr [edi + 0x14]
        mov    ecx, dword ptr [esi + 0xc]
        or     ecx, ecx
        je     loc_1_1176C
        mov    ecx, dword ptr [ecx + 0x10]
        mov    dword ptr [esp + 0x10], ecx
        mov    ecx, dword ptr [esp + 0x30]
        mov    dword ptr [esp + 0x14], ecx
        mov    dword ptr [esp + 0x18], edi
        mov    ecx, dword ptr [0x12f80]      ; -> obj1:0x12F80
        mov    dx, word ptr [ecx + 0xa2]
        movzx  ecx, dx
        add    ecx, 8
        mov    dword ptr [eax + 0x10], ecx
        mov    ecx, dword ptr [eax + 0x14]
        mov    dword ptr [ecx], 1
        mov    word ptr [ecx + 4], dx
        mov    word ptr [ecx + 6], 2
        mov    bx, word ptr [esp + 0x34]
        mov    dh, bl
        mov    dl, bh
        mov    word ptr [ecx + 8], dx
        mov    ebp, dword ptr [esp + 0x28]
        mov    edx, ebp
        shl    edx, 0x10
        mov    ebx, ebp
        and    ebx, 0xff00
        or     edx, ebx
        shl    edx, 8
        mov    ebx, ebp
        sar    ebx, 0x18
        mov    dl, bl
        and    ebp, 0xff0000
        shr    ebp, 8
        or     edx, ebp
        mov    dword ptr [ecx + 0xa], edx
        push   dword ptr [esi + 0x54]
        push   eax
        push   dword ptr [edi + 0x3c]
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_39F0
        add    esp, 0x10
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_1176C:
        push   0
        push   0xc0000120
        push   edi
        call   dword ptr [esp + 0x3c]
        add    esp, 0xc
        mov    eax, 0xc0000120
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
sub_1_11788:
        sub    esp, 4
        push   esi
        push   edi
        push   5
        call   sub_1_001E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_117A8
        mov    eax, 1
        pop    edi
        pop    esi
        add    esp, 4
        ret    
loc_1_117A8:
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     loc_1_117D4
        cmp    edi, 0xff
        je     loc_1_117D4
        push   esi
        call   sub_1_0024
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 4
        ret    
loc_1_117D4:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x1c], esi
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x20], 0
        mov    byte ptr [esi + 1], 0
        mov    eax, dword ptr [esp + 0x14]
        mov    byte ptr [esi], al
        cmp    eax, 0x82
        je     loc_1_1180C
        cmp    eax, 0x83
        je     loc_1_11820
        jmp    loc_1_11838
        db  090h
loc_1_1180C:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x18], 4
        mov    word ptr [esi + 2], 0
        jmp    loc_1_11838
        db  090h
loc_1_11820:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x18], 5
        mov    word ptr [esi + 2], 0x100
        mov    al, byte ptr [esp + 0x18]
        mov    byte ptr [esi + 4], al
loc_1_11838:
        push   0x11898                       ; -> loc_1_11898
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x10]
        call   sub_1_11858
        add    esp, 0xc
        mov    edi, eax
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  090h
sub_1_11858:
        sub    esp, 0x14
        mov    eax, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esp + 4], eax
        mov    edx, dword ptr [esp + 0x18]
        mov    dword ptr [esp + 0xc], edx
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esp + 8], eax
        lea    ecx, [edx + 0x18]
        push   0
        push   ecx
        lea    eax, [esp + 8]
        push   eax
        mov    eax, dword ptr [edx + 0x20]
        add    eax, dword ptr [ecx]
        push   eax
        push   0
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_3A60
        add    esp, 0x2c
        ret    
        db  090h
loc_1_11898:
        push   5
        push   dword ptr [esp + 8]
        call   sub_1_D5C0
        add    esp, 8
        ret    
        db  090h
sub_1_118A8:
        sub    esp, 4
        lea    eax, [esp]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     loc_1_118C3
        cmp    eax, 0xff
        jne    loc_1_118E0
loc_1_118C3:
        push   0
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [ecx + 0x14], eax
        push   4
        push   0
        push   dword ptr [esp + 0xc]
        call   sub_1_118E4
        add    esp, 0x10
loc_1_118E0:
        add    esp, 4
        ret    
sub_1_118E4:
        sub    esp, 0x10
        mov    edx, dword ptr [esp + 0x14]
        mov    eax, dword ptr [edx + 0x14]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esp], eax
        mov    dword ptr [esp + 8], edx
        mov    dword ptr [esp + 4], 0x1192c  ; -> sub_1_1192C
        mov    eax, dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x20]
        mov    word ptr [edx + 0x4c], ax
        mov    ecx, dword ptr [edx + 0x14]
        push   dword ptr [ecx + 0x40]
        push   dword ptr [edx + 0x28]
        push   eax
        push   dword ptr [esp + 0x28]
        lea    eax, [esp + 0x14]
        push   eax
        call   sub_1_3A30
        add    esp, 0x28
        ret    
        db  08Dh, 049h, 000h
sub_1_1192C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        mov    edi, dword ptr [esi + 0x14]
        cmp    byte ptr [0x13230], 0         ; -> loc_1_13230
        jne    loc_1_11964
        mov    eax, dword ptr [edi + 0x38]
        cmp    eax, 8
        jne    loc_1_11960
        cmp    word ptr [esi + 0x4c], 4
        jne    loc_1_11960
        mov    dword ptr [edi + 0x38], 9
        sub    bl, bl
        jmp    loc_1_11974
        db  08Dh, 049h, 000h
loc_1_11960:
        or     eax, eax
        je     loc_1_11970
loc_1_11964:
        mov    bl, 1
        mov    dword ptr [edi + 0x38], 0
        jmp    loc_1_11974
        db  090h
loc_1_11970:
        mov    bl, byte ptr [esp + 0x13]
loc_1_11974:
        mov    eax, dword ptr [edi + 0xc]
        or     eax, eax
        je     loc_1_119A0
        mov    ecx, dword ptr [eax + 0x58]
        or     ecx, ecx
        je     loc_1_119A0
        mov    dword ptr [eax + 0x58], 0
        cmp    byte ptr [eax + 0x6f], 2
        jne    loc_1_11998
        mov    eax, 0x14
        jmp    loc_1_119A6
        db  08Bh, 0FFh
loc_1_11998:
        mov    eax, 0x16
        jmp    loc_1_119A6
        db  090h
loc_1_119A0:
        sub    ecx, ecx
        mov    eax, dword ptr [esp + 0x14]
loc_1_119A6:
        mov    ebp, dword ptr [edi + 0x40]
        or     ebp, ebp
        je     loc_1_119B8
        mov    dword ptr [edi + 0x40], 0
        jmp    loc_1_119BA
        db  08Bh, 0FFh
loc_1_119B8:
        sub    ebp, ebp
loc_1_119BA:
        or     ecx, ecx
        je     loc_1_119CA
        push   0
        push   eax
        push   ecx
        call   sub_1_25A0
        add    esp, 0xc
loc_1_119CA:
        or     ebp, ebp
        je     loc_1_119DD
        push   0
        push   dword ptr [esp + 0x24]
        push   ebp
        call   sub_1_25A0
        add    esp, 0xc
loc_1_119DD:
        or     bl, bl
        je     loc_1_119F6
        push   dword ptr [edi + 0x18]
        push   0x7930                        ; -> loc_1_7930
        push   0
        push   edi
        push   0
        call   sub_1_3C00
        add    esp, 0x14
loc_1_119F6:
        push   4
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
sub_1_11A0C:
        sub    esp, 0x3c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x54]
        push   esi
        mov    edi, dword ptr [esp + 0x54]
        push   edi
        call   sub_1_12000
        add    esp, 8
        cmp    al, 1
        jne    loc_1_11A34
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x3c
        ret    
        db  090h
loc_1_11A34:
        push   edi
        call   sub_1_0620
        add    esp, 4
        mov    dword ptr [esp + 0x18], eax
        or     eax, eax
        jne    loc_1_11A50
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x3c
        ret    
        db  090h
loc_1_11A50:
        mov    ebx, dword ptr [esp + 0x5c]
        mov    byte ptr [ebx], 0
        mov    dword ptr [esp + 0x1c], 0
loc_1_11A5F:
        sub    ebp, ebp
loc_1_11A61:
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x1c]
        call   sub_1_0510
        add    esp, 8
        or     eax, eax
        je     loc_1_11BC8
        mov    dword ptr [esp + 0x10], 5
        lea    ecx, [esp + 0x10]
        push   ecx
        lea    ecx, [esp + 0x3c]
        push   ecx
        push   eax
        call   sub_1_11C8C
        add    esp, 0xc
        mov    dword ptr [esp + 0x14], eax
        shl    eax, 0x1c
        sar    eax, 0x1c
        cmp    eax, 1
        jb     loc_1_11A61
        cmp    eax, 2
        jbe    loc_1_11AC4
        cmp    eax, 3
        je     loc_1_11B60
        cmp    eax, 4
        je     loc_1_11BB8
        cmp    eax, 5
        je     loc_1_11A5F
        jmp    loc_1_11A61
        db  08Bh, 0FFh
loc_1_11AC4:
        test   byte ptr [esp + 0x14], 0x10
        jne    loc_1_11A61
        mov    eax, dword ptr [esp + 0x10]
        dec    eax
        cmp    eax, 1
        jl     loc_1_11A61
        mov    edi, dword ptr [esp + 0x3c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 0x10
        jne    loc_1_11B04
        push   0x10
        push   esi
        push   dword ptr [esp + 0x44]
        call   sub_1_0262
        add    esp, 0xc
        cmp    eax, 0x10
        je     loc_1_11B30
        jmp    loc_1_11A61
        db  090h
loc_1_11B04:
        push   0
        push   dword ptr [esp + 0x40]
        lea    eax, [esp + 0x2c]
        push   eax
        call   sub_1_122EC
        add    esp, 0xc
        push   0xf
        push   esi
        lea    eax, [esp + 0x2c]
        push   eax
        call   sub_1_0262
        add    esp, 0xc
        cmp    eax, 0xf
        jne    loc_1_11A61
loc_1_11B30:
        test   byte ptr [esp + 0x14], 0x20
        je     loc_1_11B3A
        mov    byte ptr [ebx], 1
loc_1_11B3A:
        lea    eax, [esp + 0x1c]
        push   eax
        push   dword ptr [esp + 0x3c]
        call   sub_1_FD20
        add    esp, 8
        or     eax, eax
        je     loc_1_11BC8
        cmp    eax, 0xff
        je     loc_1_11BC8
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_1_11BC8
loc_1_11B60:
        cmp    byte ptr [esp + 0x58], 0
        je     loc_1_11A61
        cmp    ebp, 2
        je     loc_1_11A61
        cmp    dword ptr [esp + 0x10], 2
        jl     loc_1_11A61
        push   ebx
        push   esi
        push   0x11a0c                       ; -> sub_1_11A0C
        push   dword ptr [esp + 0x48]
        call   sub_1_1239C
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_11BAC
        cmp    ebp, 1
        jne    loc_1_11A61
        mov    ebp, 2
        jmp    loc_1_11A61
        db  08Dh, 049h, 000h
loc_1_11BAC:
        cmp    eax, -1
        jne    loc_1_11BC4
        jmp    loc_1_11A61
        db  08Bh, 0FFh
loc_1_11BB8:
        mov    ebp, 1
        jmp    loc_1_11A61
        db  08Bh, 0FFh
loc_1_11BC4:
        mov    dword ptr [esp + 0x1c], eax
loc_1_11BC8:
        push   dword ptr [esp + 0x18]
        call   sub_1_04E0
        add    esp, 4
        or     eax, eax
        je     loc_1_11BE2
        cmp    eax, 0xff
        je     loc_1_11BE2
        mov    byte ptr [ebx], 0
loc_1_11BE2:
        mov    eax, dword ptr [esp + 0x1c]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x3c
        ret    
        db  08Bh, 0FFh
sub_1_11BF0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esi]
        lea    edx, [edi + 1]
        movsx  eax, byte ptr [edx]
        cmp    eax, 0x5c
        je     loc_1_11C80
        sub    eax, eax
        sub    ecx, ecx
        inc    edx
        cmp    byte ptr [edx], 0x78
        je     loc_1_11C1C
        mov    dword ptr [esi], edx
        mov    eax, 0xffffffff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_11C1C:
        inc    edx
        sub    ebp, ebp
        nop    
loc_1_11C20:
        mov    bl, byte ptr [edx]
        or     bl, bl
        je     loc_1_11C63
        cmp    bl, 0x20
        je     loc_1_11C63
        cmp    bl, 0x61
        jl     loc_1_11C3C
        movsx  ebx, bl
        shl    eax, 4
        lea    eax, [ebx + eax - 0x57]
        jmp    loc_1_11C5B
loc_1_11C3C:
        cmp    bl, 0x41
        jl     loc_1_11C50
        movsx  ebx, bl
        shl    eax, 4
        lea    eax, [ebx + eax - 0x37]
        jmp    loc_1_11C5B
        db  08Dh, 049h, 000h
loc_1_11C50:
        sub    eax, 3
        shl    eax, 4
        movsx  ebx, bl
        add    eax, ebx
loc_1_11C5B:
        inc    edx
        inc    ecx
        inc    ebp
        cmp    ebp, 2
        jl     loc_1_11C20
loc_1_11C63:
        cmp    ecx, 1
        jge    loc_1_11C74
        mov    dword ptr [esi], edx
        mov    eax, 0xffffffff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_11C74:
        add    ecx, edi
        add    ecx, 2
        mov    dword ptr [esi], ecx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_11C80:
        mov    dword ptr [esi], edx
        mov    eax, 0x5c
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_11C8C:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x2c]
        mov    eax, dword ptr [eax]
        lea    ecx, [eax*4]
        mov    edi, dword ptr [esp + 0x28]
        sub    eax, eax
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        sub    ebp, ebp
        mov    eax, dword ptr [esp + 0x14]
        and    eax, 0xfffffff1
        or     eax, 1
        mov    dword ptr [esp + 0x14], eax
        and    dword ptr [esp + 0x14], 0xffffffef
        and    dword ptr [esp + 0x14], 0xffffffdf
        mov    eax, dword ptr [esp + 0x2c]
        mov    edi, dword ptr [eax]
        sub    esi, esi
        mov    dword ptr [esp + 0x18], ebp
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [esp + 0x10], eax
        cmp    byte ptr [eax], 0
        je     loc_1_11F42
        mov    ebx, dword ptr [esp + 0x1c]
loc_1_11CE6:
        mov    eax, dword ptr [esp + 0x10]
        mov    al, byte ptr [eax]
        movzx  ecx, al
        cmp    ecx, 0x20
        jg     loc_1_11D20
        je     loc_1_11D3C
        cmp    ecx, 9
        je     loc_1_11D3C
        cmp    ecx, 0xa
        je     loc_1_11F31
        cmp    ecx, 0xd
        je     loc_1_11F31
loc_1_11D0D:
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_11E88
        mov    byte ptr [ebx], al
        inc    ebx
        jmp    loc_1_11EB2
loc_1_11D20:
        cmp    ecx, 0x22
        je     loc_1_11D70
        cmp    ecx, 0x23
        je     loc_1_11DD8
        cmp    ecx, 0x5c
        je     loc_1_11E64
        jmp    loc_1_11D0D
        db  08Dh, 049h, 000h
loc_1_11D3C:
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_11D4C
        mov    byte ptr [ebx], al
        inc    ebx
        jmp    loc_1_11EB2
        db  090h
loc_1_11D4C:
        cmp    ebp, 2
        jne    loc_1_11EB2
        mov    ebp, 1
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax], 0
        cmp    edi, esi
        jne    loc_1_11EB2
        jmp    loc_1_11F42
        db  08Bh, 0FFh
loc_1_11D70:
        cmp    ebp, 2
        jne    loc_1_11DA0
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_11DA0
        mov    ebp, 1
        mov    dword ptr [esp + 0x18], 0
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax], 0
        mov    byte ptr [ebx], 0
        cmp    edi, esi
        jne    loc_1_11EB2
        jmp    loc_1_11F42
loc_1_11DA0:
        mov    ebp, 2
        mov    dword ptr [esp + 0x18], 1
        mov    eax, dword ptr [esp + 0x2c]
        cmp    dword ptr [eax], edi
        jl     loc_1_11EC8
        cmp    edi, esi
        jle    loc_1_11EE0
        mov    eax, dword ptr [esp + 0x10]
        inc    eax
        mov    ecx, dword ptr [esp + 0x28]
        mov    dword ptr [ecx + esi*4], eax
        inc    esi
        mov    ebx, dword ptr [esp + 0x10]
        inc    ebx
        jmp    loc_1_11EB2
loc_1_11DD8:
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_11DE8
        mov    byte ptr [ebx], al
        inc    ebx
        jmp    loc_1_11EB2
        db  090h
loc_1_11DE8:
        or     ebp, ebp
        mov    eax, 0x131c8                  ; -> loc_1_131C8
        je     loc_1_11DF6
        mov    eax, 0x13158                  ; -> loc_1_13158
loc_1_11DF6:
        push   eax
        push   dword ptr [esp + 0x14]
        call   sub_1_11FB0
        add    esp, 8
        or     eax, eax
        je     loc_1_11F28
        mov    ebp, 2
        mov    edi, dword ptr [eax + 0xc]
        mov    edx, dword ptr [eax + 8]
        cmp    edx, 0x4000
        je     loc_1_11E54
        cmp    edx, 0x8000
        je     loc_1_11E5C
        mov    eax, dword ptr [esp + 0x2c]
        cmp    dword ptr [eax], edi
        jl     loc_1_11EF8
        cmp    edi, esi
        jle    loc_1_11F10
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x28]
        mov    dword ptr [ecx + esi*4], eax
        inc    esi
        xor    edx, dword ptr [esp + 0x14]
        and    edx, 0xf
        xor    dword ptr [esp + 0x14], edx
        jmp    loc_1_11EB2
        db  090h
loc_1_11E54:
        or     dword ptr [esp + 0x14], 0x20
        jmp    loc_1_11EB2
        db  090h
loc_1_11E5C:
        or     dword ptr [esp + 0x14], 0x10
        jmp    loc_1_11EB2
        db  090h
loc_1_11E64:
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_11E88
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_1_11BF0
        add    esp, 4
        cmp    eax, -1
        je     loc_1_11F68
        mov    byte ptr [ebx], al
        inc    ebx
        jmp    loc_1_11EB2
        db  08Bh, 0FFh
loc_1_11E88:
        cmp    ebp, 2
        je     loc_1_11EB2
        mov    ebp, 2
        mov    eax, dword ptr [esp + 0x2c]
        cmp    dword ptr [eax], edi
        jl     loc_1_11F80
        cmp    edi, esi
        jle    loc_1_11F98
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x28]
        mov    dword ptr [ecx + esi*4], eax
        inc    esi
loc_1_11EB2:
        inc    dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x10]
        cmp    byte ptr [eax], 0
        jne    loc_1_11CE6
        jmp    loc_1_11F42
loc_1_11EC8:
        mov    eax, dword ptr [esp + 0x14]
        and    eax, 0xfffffff6
        or     eax, 6
        mov    dword ptr [esp + 0x14], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_11EE0:
        mov    eax, dword ptr [esp + 0x14]
        and    eax, 0xfffffff6
        or     eax, 6
        mov    dword ptr [esp + 0x14], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_11EF8:
        mov    eax, dword ptr [esp + 0x14]
        and    eax, 0xfffffff6
        or     eax, 6
        mov    dword ptr [esp + 0x14], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_11F10:
        mov    eax, dword ptr [esp + 0x14]
        and    eax, 0xfffffff6
        or     eax, 6
        mov    dword ptr [esp + 0x14], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_11F28:
        or     ebp, ebp
        jne    loc_1_11F31
        and    dword ptr [esp + 0x14], 0xfffffff0
loc_1_11F31:
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax], 0
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_11F42
        mov    byte ptr [ebx], 0
loc_1_11F42:
        cmp    esi, 1
        jg     loc_1_11F55
        mov    eax, dword ptr [esp + 0x14]
        and    eax, 0xfffffff6
        or     eax, 6
        mov    dword ptr [esp + 0x14], eax
loc_1_11F55:
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_11F68:
        mov    eax, dword ptr [esp + 0x14]
        and    eax, 0xfffffff6
        or     eax, 6
        mov    dword ptr [esp + 0x14], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_11F80:
        mov    eax, dword ptr [esp + 0x14]
        and    eax, 0xfffffff6
        or     eax, 6
        mov    dword ptr [esp + 0x14], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_11F98:
        mov    eax, dword ptr [esp + 0x14]
        and    eax, 0xfffffff6
        or     eax, 6
        mov    dword ptr [esp + 0x14], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
sub_1_11FB0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x14]
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    esi, [ecx - 1]
        mov    edi, dword ptr [esp + 0x18]
        cmp    dword ptr [edi], 0
        je     loc_1_11FF0
loc_1_11FD1:
        mov    ebp, dword ptr [edi + 4]
        cmp    esi, ebp
        jb     loc_1_11FE8
        push   ebp
        push   dword ptr [edi]
        push   ebx
        call   sub_1_0262
        add    esp, 0xc
        cmp    eax, ebp
        je     loc_1_11FF8
loc_1_11FE8:
        add    edi, 0x10
        cmp    dword ptr [edi], 0
        jne    loc_1_11FD1
loc_1_11FF0:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_11FF8:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_12000:
        sub    esp, 0x48
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    bl, bl
        mov    esi, dword ptr [esp + 0x5c]
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    edx, [ecx - 1]
        mov    edi, 0x1320c                  ; -> obj1:0x1320C
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    edx, ecx
        jbe    loc_1_120FD
        mov    edi, 0x1320c                  ; -> obj1:0x1320C
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        push   ecx
        push   0x1320c                       ; -> obj1:0x1320C
        push   esi
        call   sub_1_0262
        add    esp, 0xc
        mov    edx, eax
        mov    edi, 0x1320c                  ; -> obj1:0x1320C
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    edx, ecx
        jne    loc_1_120FD
        mov    edi, 0x1320c                  ; -> obj1:0x1320C
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    ebp, [ecx + esi - 1]
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    edi, [ecx + esi - 2]
        push   0x5c
        push   ebp
        call   sub_1_12CE8
        add    esp, 8
        mov    esi, eax
        cmp    edi, esi
        je     loc_1_120FD
        mov    byte ptr [esi], bl
        push   0
        push   ebp
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_122EC
        add    esp, 0xc
        push   0xf
        push   dword ptr [esp + 0x64]
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_0262
        add    esp, 0xc
        cmp    eax, 0xf
        jne    loc_1_120D4
        jmp    loc_1_120F8
        db  090h
loc_1_120D4:
        lea    eax, [esp + 0x12]
        push   eax
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        lea    eax, [esp + 0x1c]
        push   eax
        push   1
        call   sub_1_DFA0
        add    esp, 0x10
        or     eax, eax
        je     loc_1_120F8
        test   byte ptr [eax + 0x21], 8
        jne    loc_1_120FA
loc_1_120F8:
        mov    bl, 1
loc_1_120FA:
        mov    byte ptr [esi], 0x5c
loc_1_120FD:
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x48
        ret    
        db  090h
sub_1_12108:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        mov    ebx, edi
        mov    esi, dword ptr [esp + 0x10]
loc_1_12115:
        mov    ebx, dword ptr [ebx]
        cmp    ebx, edi
        je     loc_1_12138
        push   0x10
        push   esi
        lea    eax, [ebx + 0x34]
        push   eax
        call   sub_1_0262
        add    esp, 0xc
        cmp    eax, 0x10
        jne    loc_1_12115
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_12138:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_12140:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    ecx, dword ptr [esp + 0x1c]
        cmp    dword ptr [ecx], ecx
        je     loc_1_12206
        push   ecx
        push   dword ptr [esp + 0x18]
        call   sub_1_12108
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_12206
        mov    ax, word ptr [ebx + 8]
        mov    dx, word ptr [ebx + 0xa]
        cmp    ax, dx
        jbe    loc_1_121A0
        mov    eax, dword ptr [ebx + 0xc]
        cmp    dword ptr [eax], -1
        je     loc_1_12186
loc_1_1217E:
        add    eax, 4
        cmp    dword ptr [eax], -1
        jne    loc_1_1217E
loc_1_12186:
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], 0xffffffff
        add    dx, 4
        mov    word ptr [ebx + 0xa], dx
        jmp    loc_1_12206
        db  08Dh, 049h, 000h
loc_1_121A0:
        add    ax, 0x18
        push   eax
        call   sub_1_35F0
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_12206
        movzx  eax, word ptr [ebx + 8]
        mov    edi, ebp
        mov    esi, dword ptr [ebx + 0xc]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   dword ptr [ebx + 0xc]
        call   sub_1_0024
        add    esp, 4
        mov    dword ptr [ebx + 0xc], ebp
        mov    ax, word ptr [ebx + 8]
        movzx  ecx, ax
        add    ebp, ecx
        sub    ebp, 4
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [ebp], ecx
        mov    dword ptr [ebp + 4], 0xffffffff
        add    word ptr [ebx + 0xa], 4
        add    ax, 0x18
        mov    word ptr [ebx + 8], ax
        mov    dword ptr [ebx + 0x24], 0xfacf0000
loc_1_12206:
        or     ebx, ebx
        jne    loc_1_12285
        push   0x44
        call   sub_1_35F0
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_12285
        push   0x18
        call   sub_1_35F0
        add    esp, 4
        or     eax, eax
        je     loc_1_1227C
        lea    edi, [ebx + 0x34]
        mov    esi, dword ptr [esp + 0x14]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [ebx + 0xc], eax
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], 0xffffffff
        mov    dword ptr [ebx + 0x1c], 1
        mov    dword ptr [ebx + 0x20], 8
        mov    word ptr [ebx + 8], 0x18
        mov    word ptr [ebx + 0xa], 8
        mov    dword ptr [ebx + 0x24], 0xfacf0000
        mov    ecx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [ecx], ebx
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_1227C:
        push   ebx
        call   sub_1_0024
        add    esp, 4
loc_1_12285:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1228C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi], esi
        je     loc_1_122E9
        mov    edi, dword ptr [0x1348c]      ; -> loc_1_1348C
        cmp    edi, 0x1348c                  ; -> loc_1_1348C
        je     loc_1_122C8
loc_1_122A4:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        mov    edx, edi
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [eax + 4], eax
        mov    dword ptr [eax], eax
        push   eax
        call   sub_1_DEA4
        add    esp, 4
        cmp    edi, 0x1348c                  ; -> loc_1_1348C
        jne    loc_1_122A4
loc_1_122C8:
        mov    eax, dword ptr [esi]
        mov    dword ptr [0x1348c], eax      ; -> loc_1_1348C
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [0x13490], eax      ; -> loc_1_13490
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax + 4], 0x1348c  ; -> loc_1_1348C
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], 0x1348c      ; -> loc_1_1348C
loc_1_122E9:
        pop    edi
        pop    esi
        ret    
sub_1_122EC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 0x18]
        mov    dword ptr [esp + 0x10], edx
        mov    ecx, dword ptr [esp + 0x1c]
        mov    ebx, ecx
        lea    esi, [edx + 0xf]
        cmp    byte ptr [ecx], 0
        je     loc_1_12318
loc_1_12309:
        cmp    edx, esi
        jae    loc_1_12337
        mov    al, byte ptr [ecx]
        mov    byte ptr [edx], al
        inc    ecx
        inc    edx
        cmp    byte ptr [ecx], 0
        jne    loc_1_12309
loc_1_12318:
        cmp    edx, esi
        jae    loc_1_12337
        mov    ebp, esi
        sub    ebp, edx
        mov    edi, edx
        mov    eax, 0x20202020
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebp
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        add    edx, ebp
loc_1_12337:
        mov    byte ptr [edx], 0
        mov    byte ptr [edx + 1], 0
        mov    ebp, dword ptr [esp + 0x10]
        cmp    ebp, esi
        jae    loc_1_12383
loc_1_12346:
        mov    al, byte ptr [ebx]
        inc    ebx
        or     al, al
        je     loc_1_1235F
        cmp    al, 0x61
        jl     loc_1_12357
        cmp    al, 0x7a
        jg     loc_1_12357
        sub    al, 0x20
loc_1_12357:
        mov    byte ptr [ebp], al
        inc    ebp
        cmp    ebp, esi
        jb     loc_1_12346
loc_1_1235F:
        cmp    ebp, esi
        jae    loc_1_12383
        mov    edx, dword ptr [esp + 0x10]
        sub    edx, ebp
        add    edx, 0xf
        mov    edi, ebp
        mov    eax, 0x20202020
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        add    ebp, edx
loc_1_12383:
        mov    al, byte ptr [esp + 0x20]
        mov    byte ptr [ebp], al
        mov    byte ptr [ebp + 1], 0
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_1_1239C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi], 0x22
        jne    loc_1_123B9
        inc    esi
        push   0x22
        push   esi
        call   sub_1_12CE8
        add    esp, 8
        or     eax, eax
        je     loc_1_123B9
        mov    byte ptr [eax], 0
loc_1_123B9:
        push   dword ptr [esp + 0x14]
        push   0
        push   dword ptr [esp + 0x18]
        push   esi
        call   dword ptr [esp + 0x1c]
        add    esp, 0x10
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_123D0:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [ecx + 8]
        mov    eax, dword ptr [esi + 0x2c]
        mov    eax, dword ptr [eax + 0x7c]
        or     eax, eax
        je     loc_1_1248C
        cmp    eax, 0x7f000000
        je     loc_1_1248C
        mov    ax, word ptr [esi + 0x4c]
        and    ax, 0xfff8
        mov    word ptr [esi + 0x4c], ax
        or     ax, 0x80
        mov    word ptr [esi + 0x4c], ax
        mov    ebx, dword ptr [ecx + 0xc]
        mov    edi, dword ptr [ecx + 0x10]
        push   ecx
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [esi + 0x38]
        mov    dword ptr [eax], 0x1327c      ; -> loc_1_1327C
        mov    eax, dword ptr [0x13280]      ; -> loc_1_13280
        mov    ecx, dword ptr [esi + 0x38]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 0x38]
        mov    ecx, dword ptr [0x13280]      ; -> loc_1_13280
        mov    dword ptr [ecx], eax
        mov    dword ptr [0x13280], eax      ; -> loc_1_13280
        mov    eax, dword ptr [0x12f80]      ; -> obj1:0x12F80
        mov    edx, dword ptr [eax + 0x98]
        mov    ax, word ptr [eax + 0x9c]
        inc    dword ptr [esi + 0x34]
        mov    ecx, dword ptr [0x13333]      ; -> obj1:0x13333
        mov    dword ptr [esi + 0x3c], ecx
        push   1
        push   edx
        and    eax, 0xffff
        push   eax
        push   edi
        push   ebx
        push   0xf02c                        ; -> loc_1_F02C
        push   esi
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   dword ptr [esi + 0x38]
        call   sub_1_10C0C
        add    esp, 0x24
        mov    edi, eax
        push   esi
        call   sub_1_F280
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_1248C:
        pop    edi
        mov    eax, 0xc00000be
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h, 08Bh, 015h
        dd  loc_1_13374              ; fixup
        db  00Bh, 0D2h, 074h, 032h, 08Bh, 0C2h, 08Bh, 040h, 008h, 08Bh, 048h, 038h, 0C7h, 005h
        dd  loc_1_13374              ; fixup
        db  000h, 000h, 000h, 000h, 08Bh, 044h, 024h, 004h, 089h, 041h, 00Ch, 08Bh, 041h, 020h, 025h, 00Fh
        db  0FFh, 0FFh, 0FFh, 089h, 041h, 020h, 083h, 0C8h, 018h, 089h, 041h, 020h, 08Bh, 044h, 024h, 008h
        db  089h, 010h, 0C3h, 090h, 08Bh, 044h, 024h, 008h, 0C7h, 000h, 000h, 000h, 000h, 000h, 0C3h, 090h
sub_1_124E0:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x34]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [ecx - 1]
        mov    dword ptr [esp + 0x14], eax
        cmp    eax, 0x10
        mov    dword ptr [esp + 0x18], 1
        je     loc_1_12512
        mov    dword ptr [esp + 0x18], 3
loc_1_12512:
        sub    ebp, ebp
        cmp    dword ptr [esp + 0x18], ebp
        jle    loc_1_125CD
        mov    ebx, dword ptr [esp + 0x38]
loc_1_12522:
        cmp    dword ptr [esp + 0x14], 0x10
        je     loc_1_12544
        mov    al, byte ptr [ebp + 0x13208]  ; -> obj1:0x13208
        push   eax
        push   dword ptr [esp + 0x38]
        lea    eax, [esp + 0x24]
        push   eax
        call   sub_1_122EC
        add    esp, 0xc
        jmp    loc_1_12550
        db  090h
loc_1_12544:
        lea    edi, [esp + 0x1c]
        mov    esi, dword ptr [esp + 0x34]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
loc_1_12550:
        lea    eax, [esp + 0x10]
        push   eax
        push   0
        push   ebx
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        lea    eax, [esp + 0x2c]
        push   eax
        push   dword ptr [0x132b8]           ; -> loc_1_132B8
        call   sub_1_65BC
        add    esp, 0x18
        or     eax, eax
        mov    eax, dword ptr [esp + 0x10]
        jne    loc_1_125B8
        mov    dword ptr [eax + 0x1c], 2
        mov    eax, dword ptr [esp + 0x10]
        or     dword ptr [eax + 0x20], 0x810
        mov    eax, dword ptr [esp + 0x10]
        and    dword ptr [eax + 0x20], 0xffffffbf
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x18], 0xffffffff
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x24], 0xfacf0000
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x28], 0xffffffff
        jmp    loc_1_125BB
        db  08Bh, 0FFh
loc_1_125B8:
        mov    dword ptr [eax + 0xc], ebx
loc_1_125BB:
        cmp    dword ptr [esp + 0x14], 0x10
        je     loc_1_125D8
        inc    ebp
        cmp    dword ptr [esp + 0x18], ebp
        jg     loc_1_12522
loc_1_125CD:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  090h
loc_1_125D8:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Bh, 0FFh
sub_1_125E4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [0x132b8]      ; -> loc_1_132B8
        sub    ebx, ebx
        mov    eax, edi
        cmp    dword ptr [eax], ebx
        jle    loc_1_12633
        mov    ebp, edi
        add    ebp, 8
        nop    
loc_1_125FC:
        mov    esi, dword ptr [ebp]
        cmp    esi, ebp
        je     loc_1_1262B
        nop    
loc_1_12604:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        test   byte ptr [eax + 0x21], 8
        je     loc_1_12627
        cmp    dword ptr [eax + 0x1c], 2
        jne    loc_1_12627
        mov    edx, esi
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_12627:
        cmp    esi, ebp
        jne    loc_1_12604
loc_1_1262B:
        inc    ebx
        add    ebp, 8
        cmp    dword ptr [edi], ebx
        jg     loc_1_125FC
loc_1_12633:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_12638:
        sub    esp, 0x44
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    byte ptr [0x13354], 0         ; -> loc_1_13354
        jne    loc_1_12654
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x44
        ret    
        db  08Bh, 0FFh
loc_1_12654:
        push   0x13220                       ; -> loc_1_13220
        lea    eax, [esp + 0x28]
        mov    dword ptr [esp + 0x2c], eax
        mov    dword ptr [esp + 0x28], eax
        mov    esi, dword ptr [esp + 0x5c]
        push   esi
        call   sub_1_12000
        add    esp, 8
        cmp    al, 1
        jne    loc_1_12680
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x44
        ret    
loc_1_12680:
        push   esi
        call   sub_1_0620
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_1269C
        mov    eax, 0xffffffff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x44
        ret    
loc_1_1269C:
        sub    edi, edi
loc_1_1269E:
        sub    ebx, ebx
loc_1_126A0:
        lea    eax, [esp + 0x20]
        push   eax
        push   esi
        call   sub_1_0510
        add    esp, 8
        or     eax, eax
        je     loc_1_1282C
        cmp    edi, 0x61
        jge    loc_1_1282C
        mov    dword ptr [esp + 0x14], 5
        lea    ecx, [esp + 0x14]
        push   ecx
        lea    ecx, [esp + 0x44]
        push   ecx
        push   eax
        call   sub_1_11C8C
        add    esp, 0xc
        mov    dword ptr [esp + 0x10], eax
        and    al, 0xf
        cmp    al, 6
        je     loc_1_126A0
        cmp    dword ptr [esp + 0x44], 0
        je     loc_1_126A0
        test   byte ptr [esp + 0x10], 0x10
        je     loc_1_12735
        lea    eax, [esp + 0x1c]
        push   eax
        push   dword ptr [esp + 0x44]
        call   sub_1_FD20
        add    esp, 8
        or     eax, eax
        je     loc_1_1270E
        cmp    eax, 0xff
        jne    loc_1_12735
loc_1_1270E:
        mov    eax, dword ptr [esp + 0x10]
        and    eax, 0x20
        shr    eax, 5
        push   eax
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x4c]
        call   sub_1_124E0
        add    esp, 0xc
        or     eax, eax
        je     loc_1_12734
        cmp    eax, 0xff
        jne    loc_1_12735
loc_1_12734:
        inc    edi
loc_1_12735:
        mov    eax, dword ptr [esp + 0x10]
        shl    eax, 0x1c
        sar    eax, 0x1c
        cmp    eax, 2
        je     loc_1_12760
        cmp    eax, 3
        je     loc_1_127C0
        cmp    eax, 4
        je     loc_1_12820
        cmp    eax, 5
        je     loc_1_1269E
        jmp    loc_1_126A0
loc_1_12760:
        mov    eax, dword ptr [esp + 0x14]
        dec    eax
        cmp    eax, 2
        jl     loc_1_126A0
        push   0x1c
        mov    eax, dword ptr [esp + 0x4c]
        add    eax, 5
        push   eax
        lea    eax, [esp + 0x34]
        push   eax
        call   sub_1_122EC
        add    esp, 0xc
        lea    eax, [esp + 0x18]
        push   eax
        push   dword ptr [esp + 0x44]
        call   sub_1_FD20
        add    esp, 8
        or     eax, eax
        je     loc_1_127A5
        cmp    eax, 0xff
        jne    loc_1_126A0
loc_1_127A5:
        lea    eax, [esp + 0x24]
        push   eax
        push   dword ptr [esp + 0x1c]
        lea    eax, [esp + 0x34]
        push   eax
        call   sub_1_12140
        add    esp, 0xc
        jmp    loc_1_126A0
loc_1_127C0:
        cmp    ebx, 2
        je     loc_1_126A0
        cmp    dword ptr [esp + 0x14], 2
        jl     loc_1_126A0
        push   esi
        call   sub_1_0700
        add    esp, 4
        or     eax, eax
        je     loc_1_126A0
        push   0
        push   0
        push   0x12638                       ; -> sub_1_12638
        push   dword ptr [esp + 0x50]
        call   sub_1_1239C
        add    esp, 0x10
        mov    ebp, eax
        push   esi
        call   sub_1_0740
        add    esp, 4
        cmp    ebp, -1
        je     loc_1_126A0
        cmp    ebx, 1
        jne    loc_1_12818
        mov    ebx, 2
loc_1_12818:
        add    edi, ebp
        jmp    loc_1_126A0
        db  090h
loc_1_12820:
        mov    ebx, 1
        jmp    loc_1_126A0
        db  08Bh, 0FFh
loc_1_1282C:
        push   esi
        call   sub_1_04E0
        add    esp, 4
        lea    eax, [esp + 0x24]
        push   eax
        call   sub_1_1228C
        add    esp, 4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x44
        ret    
sub_1_1284C:
        mov    ecx, dword ptr [0x13374]      ; -> loc_1_13374
        or     ecx, ecx
        je     loc_1_12868
        mov    dword ptr [0x13374], 0        ; -> loc_1_13374
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        ret    
        db  090h
loc_1_12868:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        ret    
        db  090h
sub_1_12874:
        push   esi
        push   edi
        sub    esi, esi
        cmp    byte ptr [esp + 0x14], 0
        je     loc_1_1288F
        mov    esi, dword ptr [0x13374]      ; -> loc_1_13374
        mov    dword ptr [0x13374], 0        ; -> loc_1_13374
loc_1_1288F:
        or     esi, esi
        je     loc_1_128FC
        mov    edi, dword ptr [esi + 8]
        mov    eax, dword ptr [edi + 0x38]
        and    dword ptr [eax + 0x20], 0xffffff0f
        mov    eax, dword ptr [edi + 0x38]
        or     dword ptr [eax + 0x20], 0x10
        mov    eax, dword ptr [esp + 0xc]
        cmp    eax, -1
        jne    loc_1_128B2
        sub    eax, eax
loc_1_128B2:
        mov    ecx, dword ptr [edi + 0x38]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [edi + 0x38]
        mov    dword ptr [eax + 0x28], 0xffffffff
        push   dword ptr [0x132d8]           ; -> loc_1_132D8
        push   dword ptr [edi + 0x38]
        call   sub_1_6608
        add    esp, 8
        or     eax, eax
        je     loc_1_128F0
        cmp    eax, 0xff
        je     loc_1_128F0
        push   dword ptr [edi + 0x38]
        call   sub_1_DEA4
        add    esp, 4
        mov    dword ptr [edi + 0x38], 0
loc_1_128F0:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], esi
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_128FC:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 0
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_1290C:
        sub    esp, 0xc
        push   esi
        push   edi
        lea    eax, [esp + 0xc]
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [esp + 0xc], eax
        cmp    dword ptr [0x13374], 0        ; -> loc_1_13374
        je     loc_1_12960
        mov    eax, dword ptr [0x13374]      ; -> loc_1_13374
        cmp    byte ptr [eax + 0x14], 0
        je     loc_1_12948
        mov    dword ptr [0x13374], 0        ; -> loc_1_13374
        push   0x15
        push   eax
        call   sub_1_12C6C
        add    esp, 8
        jmp    loc_1_12960
loc_1_12948:
        mov    eax, dword ptr [esp + 0x20]
        add    eax, 0x3c
        mov    dword ptr [esp + 8], eax
        or     word ptr [eax], 1
        mov    eax, dword ptr [0x13374]      ; -> loc_1_13374
        mov    byte ptr [eax + 0x14], 1
loc_1_12960:
        mov    esi, dword ptr [esp + 0x20]
        add    esi, 0x3c
        push   esi
        lea    eax, [esp + 0x10]
        push   eax
        push   0x1336c                       ; -> loc_1_1336C
        call   sub_1_129C4
        add    esp, 0xc
        lea    eax, [esp + 0xc]
        cmp    dword ptr [esp + 0xc], eax
        je     loc_1_129AD
        mov    edi, dword ptr [esp + 0xc]
        cmp    edi, eax
        je     loc_1_129AD
loc_1_1298C:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        mov    edx, edi
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        push   0x15
        push   eax
        call   sub_1_12C6C
        add    esp, 8
        lea    eax, [esp + 0xc]
        cmp    edi, eax
        jne    loc_1_1298C
loc_1_129AD:
        test   byte ptr [esi], 1
        jne    loc_1_129BC
        mov    dword ptr [0x13379], 0        ; -> obj1:0x13379
loc_1_129BC:
        pop    edi
        pop    esi
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
sub_1_129C4:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi]
        cmp    esi, edi
        je     loc_1_12A0C
        mov    eax, dword ptr [esp + 0x18]
        or     word ptr [eax], 1
        cmp    esi, edi
        je     loc_1_12A0C
        mov    edx, dword ptr [esp + 0x14]
loc_1_129E1:
        mov    ebx, edi
        mov    edi, dword ptr [edi]
        cmp    byte ptr [ebx + 0x14], 0
        je     loc_1_12A04
        mov    ecx, edi
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ebx], edx
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [eax], ebx
        mov    dword ptr [edx + 4], ebx
        jmp    loc_1_12A08
loc_1_12A04:
        mov    byte ptr [ebx + 0x14], 1
loc_1_12A08:
        cmp    esi, edi
        jne    loc_1_129E1
loc_1_12A0C:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_12A10:
        sub    esp, 4
        mov    eax, dword ptr [esp + 0xc]
        mov    byte ptr [eax + 0x14], 0
        cmp    dword ptr [0x13379], 0        ; -> obj1:0x13379
        jne    loc_1_12A6D
        lea    eax, [esp]
        push   eax
        push   0
        push   0x1290c                       ; -> loc_1_1290C
        push   0
        push   0x1290c                       ; -> loc_1_1290C
        push   0
        push   0
        push   dword ptr [0x13323]           ; -> obj1:0x13323
        call   sub_1_109B8
        add    esp, 0x20
        or     eax, eax
        je     loc_1_12A64
        cmp    eax, 0xff
        je     loc_1_12A64
        mov    dword ptr [0x13379], 0        ; -> obj1:0x13379
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_12A64:
        mov    eax, dword ptr [esp]
        mov    dword ptr [0x13379], eax      ; -> obj1:0x13379
loc_1_12A6D:
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_12A74:
        push   esi
        push   edi
        mov    esi, 0xc0000001
        push   0x15
        call   sub_1_001E
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     loc_1_12AF4
        mov    esi, dword ptr [esp + 0xc]
        mov    dword ptr [edi + 8], esi
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [edi + 0xc], eax
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [edi + 0x10], ecx
        cmp    byte ptr [0x13378], 0         ; -> loc_1_13378
        je     loc_1_12AC8
        mov    dword ptr [edi], 0x1336c      ; -> loc_1_1336C
        mov    eax, dword ptr [0x13370]      ; -> loc_1_13370
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [0x13370]      ; -> loc_1_13370
        mov    dword ptr [eax], edi
        mov    dword ptr [0x13370], edi      ; -> loc_1_13370
        jmp    loc_1_12AE8
        db  08Bh, 0FFh
loc_1_12AC8:
        mov    dword ptr [0x13374], edi      ; -> loc_1_13374
        mov    byte ptr [0x13378], 1         ; -> loc_1_13378
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x1c]
        push   ecx
        push   eax
        push   esi
        call   sub_1_3C00
        add    esp, 0x14
loc_1_12AE8:
        push   edi
        push   esi
        call   sub_1_12A10
        add    esp, 8
        sub    esi, esi
loc_1_12AF4:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_12AFC:
        push   esi
        push   edi
        mov    edx, dword ptr [0x13374]      ; -> loc_1_13374
        or     edx, edx
        jne    loc_1_12B2E
        cmp    dword ptr [0x1336c], 0x1336c  ; -> loc_1_1336C
        je     loc_1_12B44
        mov    edx, dword ptr [0x1336c]      ; -> loc_1_1336C
        mov    eax, edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [0x13374], edx      ; -> loc_1_13374
loc_1_12B2E:
        mov    eax, dword ptr [edx + 8]
        mov    esi, dword ptr [eax + 0x38]
        mov    edi, dword ptr [esp + 0xc]
        add    esi, 0x34
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
loc_1_12B44:
        pop    edi
        mov    byte ptr [0x13378], 0         ; -> loc_1_13378
        mov    eax, 0xc0000001
        pop    esi
        ret    
        db  090h
loc_1_12B54:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_1_0024
        add    esp, 4
        nop    
loc_1_12B68:
        sub    bl, bl
        mov    dword ptr [ebp - 8], 0
        lea    eax, [ebp - 0x18]
        push   eax
        call   sub_1_12AFC
        add    esp, 4
        or     eax, eax
        je     loc_1_12B8C
        cmp    eax, 0xff
        jne    loc_1_12C68
loc_1_12B8C:
        sub    eax, eax
        mov    esi, 0x15
        cmp    byte ptr [0x13354], al        ; -> loc_1_13354
        je     loc_1_12BFE
        cmp    byte ptr [0x13388], al        ; -> loc_1_13388
        jne    loc_1_12BC1
        push   eax
        push   1
        push   eax
        push   dword ptr [0x13327]           ; -> obj1:0x13327
        call   sub_1_12638
        add    esp, 0x10
        cmp    eax, -1
        je     loc_1_12BC1
        mov    byte ptr [0x13388], 1         ; -> loc_1_13388
loc_1_12BC1:
        lea    eax, [ebp - 1]
        push   eax
        push   1
        lea    eax, [ebp - 0x18]
        push   eax
        push   dword ptr [0x13327]           ; -> obj1:0x13327
        call   sub_1_11A0C
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_12C3C
        cmp    byte ptr [0x13353], 0         ; -> obj1:0x13353
        je     loc_1_12BFE
        lea    eax, [ebp - 1]
        push   eax
        push   1
        lea    eax, [ebp - 0x18]
        push   eax
        push   dword ptr [0x1332f]           ; -> obj1:0x1332F
        call   sub_1_11A0C
        add    esp, 0x10
loc_1_12BFE:
        or     eax, eax
        jne    loc_1_12C3C
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_1284C
        add    esp, 4
        cmp    byte ptr [0x13353], 0         ; -> obj1:0x13353
        je     loc_1_12C4D
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_12C4D
        push   dword ptr [ebp - 8]
        call   sub_1_123D0
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_12C36
        cmp    esi, 0xff
        jne    loc_1_12C4D
loc_1_12C36:
        mov    bl, 1
        jmp    loc_1_12C4D
        db  08Bh, 0FFh
loc_1_12C3C:
        push   1
        lea    ecx, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_12874
        add    esp, 0xc
        sub    esi, esi
loc_1_12C4D:
        or     bl, bl
        jne    loc_1_12B68
        push   esi
        push   dword ptr [ebp - 8]
        call   sub_1_12C6C
        add    esp, 8
        jmp    loc_1_12B68
        db  08Bh, 0FFh
loc_1_12C68:
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_12C6C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        or     eax, eax
        je     loc_1_12CBA
        mov    esi, dword ptr [eax + 8]
        mov    ebx, dword ptr [eax + 0x10]
        mov    edi, dword ptr [eax + 0xc]
        push   eax
        call   sub_1_0024
        add    esp, 4
        mov    ebp, dword ptr [esp + 0x18]
        or     ebp, ebp
        je     loc_1_12CA2
        mov    eax, dword ptr [esi + 0x38]
        or     eax, eax
        je     loc_1_12CA2
        push   eax
        call   sub_1_12CC0
        add    esp, 4
loc_1_12CA2:
        push   esi
        call   sub_1_F280
        add    esp, 4
        or     ebx, ebx
        je     loc_1_12CBA
        push   ebp
        push   edi
        push   ebx
        call   sub_1_E64C
        add    esp, 0xc
loc_1_12CBA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_12CC0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x20]
        and    ecx, 0xffffff0f
        mov    dword ptr [eax + 0x20], ecx
        or     ecx, 0x20
        mov    dword ptr [eax + 0x20], ecx
        mov    dword ptr [eax + 0x10], 0
        push   eax
        call   sub_1_DEA4
        add    esp, 4
        ret    
        db  090h
sub_1_12CE8:
        mov    eax, dword ptr [esp + 4]
        movsx  ecx, byte ptr [eax]
        mov    edx, dword ptr [esp + 8]
        cmp    ecx, edx
        je     loc_1_12D05
loc_1_12CF8:
        cmp    byte ptr [eax], 0
        je     loc_1_12D05
        inc    eax
        movsx  ecx, byte ptr [eax]
        cmp    ecx, edx
        jne    loc_1_12CF8
loc_1_12D05:
        movsx  ecx, byte ptr [eax]
        cmp    ecx, edx
        je     loc_1_12D0E
        sub    eax, eax
loc_1_12D0E:
        ret    
        db  090h, 0CDh, 020h, 029h, 080h, 028h, 000h, 0CDh, 020h, 02Ah, 080h, 028h, 000h, 0CDh, 020h, 02Bh
        db  080h, 028h, 000h, 0CDh, 020h, 02Ch, 080h, 028h, 000h, 0CDh, 020h, 02Dh, 080h, 028h, 000h, 0CDh
        db  020h, 02Eh, 080h, 028h, 000h, 0CDh, 020h, 02Fh, 080h, 028h, 000h, 0CDh, 020h, 030h, 080h, 028h
        db  000h, 0CDh, 020h, 050h, 080h, 028h, 000h
sub_1_12D46:
        VxDCall  VPOWERD.Service_8051h           ; cd2051802800
        VxDCall  VPOWERD.Service_8052h           ; cd2052802800
        VxDCall  VPOWERD.Service_8053h           ; cd2053802800
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_12E06:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_12E0C:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
        VxDCall  VPOWERD.Service_8042h           ; cd2042802800
        VxDCall  VPOWERD.Service_8012h           ; cd2012802800
        VxDCall  VPOWERD.Service_8013h           ; cd2013802800
        VxDCall  VPOWERD.Service_8014h           ; cd2014802800
        VxDCall  VPOWERD.Service_8015h           ; cd2015802800
        VxDCall  VPOWERD.Service_804Ah           ; cd204a802800
        VxDCall  VPOWERD.Service_804Bh           ; cd204b802800
        VxDCall  VPOWERD.Service_804Ch           ; cd204c802800
        VxDCall  VPOWERD.Service_804Dh           ; cd204d802800
        VxDCall  VPOWERD.Service_804Eh           ; cd204e802800
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
loc_1_12E68:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dd  VNBT_Service_00          ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0049Bh                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VNBT    '                  ; DDB_Name
        dd  05400C000h                  ; DDB_Init_Order
        dd  VNBT_Control             ; DDB_Control_Proc
        dd  VNBT_PM_API              ; DDB_V86_API_Proc
        dd  VNBT_PM_API              ; DDB_PM_API_Proc
        dd  obj1:0x12E6C             ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_12ECC:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_12ED4:
        dec    esi
        outsd  dx, dword ptr [esi]
        push   esp
        jns    0x12f4b
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_12EE0:
        push   ebx
        arpl   word ptr [edi + 0x70], bp
        dec    ecx
        db  064h, 000h
loc_1_12EE8:
        inc    edx
        jb     0x12f5a
        popal  
        arpl   word ptr fs:[ecx + 0x73], sp
        je     loc_1_12F33
        jb     loc_1_12F5B
        jae    loc_1_12F6B
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12EFC:
        aam    0x2e
        add    dword ptr [eax], eax
loc_1_12F00:
        add    byte ptr cs:[eax], al
        add    al, ah                        ; -> loc_1_12EE0
        add    dword ptr cs:[eax], eax
        das    
        add    byte ptr [eax], al
        add    al, ch                        ; -> loc_1_12EE8
        add    dword ptr cs:[eax], eax
        sbb    al, 0
        add    byte ptr [eax], al
loc_1_12F14:
        dec    esp
        inc    ecx
        dec    esi
        inc    ecx
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12F20:
        push   0x7374736f
        add    byte ptr [eax], al
        add    byte ptr [edi + ebp*2 + 0x6d], al
        popal  
        imul   ebp, dword ptr [esi], 0x61655300
loc_1_12F33:
        jb     loc_1_12F98
        push   0x7473694c
        add    byte ptr [eax], al
loc_1_12F3C:
        sub    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_12F4C:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12F50:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12F54:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
loc_1_12F5B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x61], cl
        insd   dword ptr es:[edi], dx
loc_1_12F6B:
        push   ebx
        jb     loc_1_12FE6
        jb     0x12f97
        add    byte ptr [esi + 0x61], cl
        insd   dword ptr es:[edi], dx
        push   ebx
        jb     loc_1_12FF2
        jb     loc_1_12F7F
loc_1_12F7F:
        add    byte ptr [edx + esi], dh      ; -> loc_1_13234
        add    dword ptr [eax], eax
loc_1_12F84:
        dec    esi
        popal  
        insd   dword ptr es:[edi], dx
        push   ebx
        jb     0x13002
        jb     0x12fdf
        outsd  dx, dword ptr [esi]
        jb     loc_1_13006
        add    byte ptr [eax], al
loc_1_12F94:
        inc    esp
        outsb  dx, byte ptr [esi]
        jae    0x12feb
loc_1_12F98:
        jb     0x13011
        jb     0x12fee
        outsd  dx, dword ptr [esi]
        jb     loc_1_13015
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x61], cl
        insd   dword ptr es:[edi], dx
        push   esp
        popal  
        bound  ebp, qword ptr [ebp + 0x53]
        imul   edi, dword ptr [edx + 0x65], 0x53000000
        jae    loc_1_1302B
        imul   ebp, dword ptr [edi + 0x6e], 0x6c626154
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0
loc_1_12FC8:
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        push   esp
        imul   ebp, dword ptr [ebp + 0x65], 0x74756f
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6e], cl
        imul   esi, dword ptr [ecx + ebp*2 + 0x61], 0x6665526c
        jb     loc_1_13049
        jae    0x1304e
loc_1_12FE6:
        push   esp
        dec    edi
        add    byte ptr cs:[eax], al
loc_1_12FEC:
        inc    edx
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_13040
loc_1_12FF2:
        popal  
        insd   dword ptr es:[edi], dx
        push   ecx
        jne    0x1305d
        jb     loc_1_13073
        inc    ebx
        outsd  dx, dword ptr [esi]
        jne    loc_1_1306C
        je     loc_1_13000
loc_1_13000:
        inc    edx
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_13057
loc_1_13006:
        jne    0x1306d
        jb     loc_1_13083
        push   esp
        imul   ebp, dword ptr [ebp + 0x65], 0x74756f
        add    byte ptr [eax], al
loc_1_13014:
        dec    esi
loc_1_13015:
        popal  
        insd   dword ptr es:[edi], dx
        push   ebx
        jb     0x13091
        push   ecx
        jne    loc_1_13083
        jb     0x13099
        inc    ebx
        outsd  dx, dword ptr [esi]
        jne    0x13092
        je     loc_1_13026
loc_1_13026:
        add    byte ptr [eax], al
loc_1_13028:
        dec    esi
        popal  
        insd   dword ptr es:[edi], dx
loc_1_1302B:
        push   ebx
        jb     0x130a5
        push   ecx
        jne    loc_1_13097
        jb     0x130ad
        push   esp
        imul   ebp, dword ptr [ebp + 0x65], 0x74756f
loc_1_1303C:
        push   ebx
        jae    0x130b3
loc_1_13040:
        imul   ebp, dword ptr [edi + 0x6e], 0x7065654b
        inc    ecx
        insb   byte ptr es:[edi], dx
loc_1_13049:
        imul   esi, dword ptr [esi + 0x65], 0
loc_1_13050:
        push   edx
        popal  
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        insd   dword ptr es:[edi], dx
        inc    ecx
loc_1_13057:
        popal  
        jo     loc_1_130CF
        jb     loc_1_1305E
loc_1_1305E:
        add    byte ptr [eax], al
loc_1_13060:
        push   ebx
        imul   ebp, dword ptr [esi + 0x67], 0x6552656c
        jae    0x130da
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_1_1306C:
        jae    loc_1_130D3
        add    byte ptr [eax], al
loc_1_13070:
        inc    ebp
        outsb  dx, byte ptr [esi]
        popal  
loc_1_13073:
        bound  ebp, qword ptr [ebp + 0x44]
        outsb  dx, byte ptr [esi]
        jae    loc_1_1307A
loc_1_1307A:
        add    byte ptr [eax], al
loc_1_1307C:
        dec    esp
        insd   dword ptr es:[edi], dx
        push   0x7374736f
loc_1_13083:
        push   esp
        imul   ebp, dword ptr [ebp + 0x65], 0x74756f
        add    byte ptr [ebp + 0x61], cl
        js     0x130d4
        jb     0x130f4
        insd   dword ptr es:[edi], dx
        inc    edx
        jne    loc_1_130FD
loc_1_13097:
        jb     loc_1_13104
        outsb  dx, byte ptr [esi]
        add    byte ptr [bx + si], al
        add    byte ptr [ebp + 0x6e], al
        popal  
        bound  ebp, qword ptr [ebp + 0x50]
        jb     loc_1_13118
        js     0x13124
        push   edx
        inc    ebx
        push   0x6b6365
loc_1_130B4:
        push   edi
        imul   ebp, dword ptr [esi + 0x73], 0x6e776f44
        push   esp
        imul   ebp, dword ptr [ebp + 0x65], 0x74756f
loc_1_130C4:
        push   edx
        jb     0x1312e
        jae    0x13133
        dec    edi
        jo     0x13111
        outsd  dx, dword ptr [esi]
loc_1_130CF:
        add    byte ptr gs:[eax], al
loc_1_130D3:
        add    byte ptr [ebp + 0x6e], al
        popal  
        bound  ebp, qword ptr [ebp + 0x50]
        jb     loc_1_1314C
        js     loc_1_13158
        add    byte ptr [ebx + 0x69], dl
        jp     0x13149
        das    
        push   ebx
        insd   dword ptr es:[edi], dx
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        das    
        dec    ebp
        imul   esi, dword ptr fs:[ebp + 0x6d], 0x72614c2f
        add    byte ptr gs:[bp + 0x6f], cl
        push   esp
loc_1_130FD:
        jns    0x1316f
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_13104:
        inc    edx
        jb     loc_1_13176
        popal  
        arpl   word ptr fs:[ecx + 0x73], sp
        je     0x1314f
        jb     0x13177
        jae    0x13187
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13118:
        inc    edx
        jb     0x1318a
        popal  
        arpl   word ptr fs:[ecx + 0x73], sp
        je     loc_1_13163
        jb     loc_1_1318B
        jae    loc_1_1319B
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1312C:
        push   ebx
        arpl   word ptr [edi + 0x70], bp
        dec    ecx
        add    byte ptr fs:[ebp + ebp*2 + 0x48], cl
        outsd  dx, dword ptr [esi]
        jae    loc_1_131AE
        inc    esi
        imul   ebp, dword ptr [ebp], 0x4f442300
        dec    ebp
        cmp    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_13148:
        and    edx, dword ptr [eax + 0x52]
        inc    ebp
loc_1_1314C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13150:
        and    ecx, dword ptr [esi + 0x4f]
        inc    esi
        dec    esi
        push   edx
        add    byte ptr [eax], al
loc_1_13158:
        inc    eax
        xor    dword ptr [ecx], eax
        add    byte ptr [0x2000000], al
        add    byte ptr [eax], al
loc_1_13163:
        add    byte ptr [0x48000000], al     ; -> loc_1_13148
        xor    dword ptr [ecx], eax
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x50000], al
loc_1_13176:
        add    byte ptr [eax], al
        push   eax
        xor    dword ptr [ecx], eax
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    eax, 0
        add    byte ptr [eax], al
loc_1_1318B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], ah
        dec    ecx
        dec    esi
loc_1_1319B:
        inc    ebx
        dec    esp
        push   ebp
        inc    esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_131A4:
        and    eax, dword ptr [edx + 0x45]
        inc    edi
        dec    ecx
        dec    esi
        pop    edi
        inc    ecx
        dec    esp
        push   esp
loc_1_131AE:
        inc    ebp
        push   edx
        dec    esi
        inc    ecx
        push   esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_131B8:
        and    eax, dword ptr [ebp + 0x4e]
        inc    esp
        pop    edi
        inc    ecx
        dec    esp
        push   esp
        inc    ebp
        push   edx
        dec    esi
        inc    ecx
        push   esp
        inc    ebp
        add    byte ptr [eax], al
loc_1_131C8:
        cwde   
        xor    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + esi + 0x100001], ah ; -> loc_1_131A4
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        mov    eax, 0xe000131
        add    byte ptr [eax], al
        add    byte ptr [0x1000000], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [ebx], al
        add    byte ptr [esp + eax*2 + 0x6f], bl
        jae    0x13255
        jbe    0x1327d
        arpl   word ptr [ebp + 0x73], sp
        pop    esp
        push   ebp
        dec    esi
        inc    ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13220:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13230:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13234:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1323C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13244:
        add    byte ptr [eax], al
loc_1_13246:
        add    byte ptr [eax], al
loc_1_13248:
        add    byte ptr [eax], al
loc_1_1324A:
        add    byte ptr [eax], al
loc_1_1324C:
        add    byte ptr [eax], al
loc_1_1324E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1325A:
        add    byte ptr [eax], al
loc_1_1325C:
        add    byte ptr [eax], al
loc_1_1325E:
        add    byte ptr [eax], al
loc_1_13260:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13264:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13268:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1326C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13270:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13274:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13278:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1327C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13280:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13284:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13288:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1328C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13290:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13294:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13298:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1329C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132D0:
        add    byte ptr [eax], al
loc_1_132D2:
        add    byte ptr [eax], al
loc_1_132D4:
        add    byte ptr [eax], al
loc_1_132D6:
        add    byte ptr [eax], al
loc_1_132D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13300:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13304:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13308:
        add    byte ptr [eax], al
loc_1_1330A:
        add    byte ptr [eax], al
loc_1_1330C:
        add    byte ptr [eax], al
loc_1_1330E:
        add    byte ptr [eax], al
loc_1_13310:
        add    byte ptr [eax], al
loc_1_13312:
        add    byte ptr [eax], al
loc_1_13314:
        add    byte ptr [eax], al
loc_1_13316:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1331C:
        add    byte ptr [eax], al
loc_1_1331E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13350:
        add    byte ptr [eax], al
loc_1_13352:
        add    byte ptr [eax], al
loc_1_13354:
        add    byte ptr [eax], al
loc_1_13356:
        add    byte ptr [eax], al
loc_1_13358:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1335C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13360:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13364:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13368:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1336C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13370:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13374:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13378:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13380:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13384:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13388:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1338C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13398:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1339C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133AC:
        add    byte ptr [eax], al
loc_1_133AE:
        add    byte ptr [eax], al
loc_1_133B0:
        add    byte ptr [eax], al
loc_1_133B2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13408:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1340C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1348C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13490:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13494:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_134A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xc10  R-X
        db  0CDh, 020h, 000h, 000h, 088h, 004h, 072h, 014h, 0CDh, 020h, 000h, 000h, 040h, 000h, 072h, 00Ch
        db  0CDh, 020h, 00Ah, 000h, 088h, 004h, 0A3h
        dd  loc_1_12E68              ; fixup
        db  0F8h, 0F5h, 01Bh, 0C0h, 0C3h
loc_2_0020:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
loc_2_0027:
        add    byte ptr [esi + 0x42], cl
        push   esp
        db  000h
loc_2_002C:
        cmp    byte ptr [0x26], 2            ; -> obj2:0x0026
        jne    loc_2_0038
        clc    
        jmp    loc_2_007F
loc_2_0038:
        mov    byte ptr [0x26], 1            ; -> obj2:0x0026
        mov    edi, 0x134a0                  ; -> loc_1_134A0
        mov    ecx, 0x134a0                  ; -> loc_1_134A0
        sub    ecx, edi
        shr    ecx, 2
        sub    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
        VxDCall  Dev_0488.Service_0000h          ; cd2000008804
        jb     loc_2_007F
        mov    eax, 0x20                     ; -> loc_2_0020
        push   eax
        VxDCall  Dev_0488.Service_0008h          ; cd2008008804
        add    esp, 4
        cmp    eax, 0
        jne    loc_2_0072
        stc    
        jmp    loc_2_007F
loc_2_0072:
        mov    dword ptr [0x13494], eax      ; -> loc_1_13494
        call   sub_2_00B0
        cmp    eax, 1
loc_2_007F:
        jb     loc_2_009F
        cmp    byte ptr [0x26], 1            ; -> obj2:0x0026
        je     loc_2_0097
        cmp    byte ptr [0x27], 1            ; -> loc_2_0027
        clc    
        je     loc_2_009F
        stc    
        jmp    loc_2_009F
loc_2_0097:
        clc    
        mov    byte ptr [0x27], 1            ; -> loc_2_0027
loc_2_009F:
        mov    byte ptr [0x26], 2            ; -> obj2:0x0026
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_00B0:
        push   esi
        push   edi
        sub    esi, esi
        call   0
        cmp    eax, esi
        je     loc_2_01B4
        push   0x5f80                        ; -> loc_1_5F80
        call   sub_1_0058
        add    esp, 4
        call   sub_1_5CD0
        call   sub_1_002A
        mov    edx, dword ptr [0x12f80]      ; -> obj1:0x12F80
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0x49
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        push   edx
        call   sub_2_01C0
        add    esp, 4
        cmp    eax, esi
        je     loc_2_0103
        cmp    eax, 0xff
        jne    loc_2_01B4
loc_2_0103:
        mov    ecx, dword ptr [0x12f80]      ; -> obj1:0x12F80
        lea    eax, [ecx + 0x70]
        mov    dword ptr [ecx + 0x74], eax
        mov    dword ptr [eax], eax
        lea    eax, [ecx + 0x78]
        mov    dword ptr [ecx + 0x7c], eax
        mov    dword ptr [eax], eax
        call   sub_2_03F0
        or     eax, eax
        je     loc_2_012D
        cmp    eax, 0xff
        jne    loc_2_01B4
loc_2_012D:
        call   sub_2_0664
        or     eax, eax
        je     loc_2_0150
        cmp    eax, 0xff
        je     loc_2_0150
        call   sub_2_07D4
        call   sub_1_5DF0
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
loc_2_0150:
        call   sub_1_53D0
        push   1
        push   0x4ef0                        ; -> loc_1_4EF0
        call   sub_1_645E
        add    esp, 8
        or     eax, eax
        jne    loc_2_0177
        call   sub_2_07D4
        call   sub_1_5DF0
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
loc_2_0177:
        call   sub_2_02B0
        push   0
        push   0
        call   sub_1_4B70
        add    esp, 8
        or     eax, eax
        jne    loc_2_01A0
        call   sub_2_07D4
        call   sub_1_5DF0
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_2_01A0:
        mov    dword ptr [0x12ecc], 0        ; -> loc_1_12ECC
        call   sub_1_002A
        mov    esi, 1
loc_2_01B4:
        call   sub_1_5DF0
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_2_01C0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 0x49
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        push   eax
        push   esi
        call   sub_2_0828
        add    esp, 8
        mov    eax, dword ptr [esi + 0xf3]
        or     eax, eax
        je     loc_2_0210
        push   0x104
        push   eax
        call   sub_1_04A0
        add    esp, 8
        or     eax, eax
        jne    loc_2_0201
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
loc_2_0201:
        mov    byte ptr [esi + 0x120], 1
        jmp    loc_2_0217
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0210:
        mov    byte ptr [esi + 0x120], 0
loc_2_0217:
        mov    word ptr [esi + 0xa2], 0xe
        xor    eax, eax
        mov    ax, word ptr [0x13364]        ; -> loc_1_13364
        cmp    eax, 1
        je     loc_2_0240
        cmp    eax, 2
        je     loc_2_0250
        cmp    eax, 4
        je     loc_2_0260
        cmp    eax, 8
        je     loc_2_0270
        jmp    loc_2_0279
        db  08Bh, 0FFh
loc_2_0240:
        mov    word ptr [esi + 0xd8], 0
        jmp    loc_2_0279
        db  005h, 000h, 000h, 000h, 000h
loc_2_0250:
        mov    word ptr [esi + 0xd8], 0x2000
        jmp    loc_2_0279
        db  005h, 000h, 000h, 000h, 000h
loc_2_0260:
        mov    word ptr [esi + 0xd8], 0x4000
        jmp    loc_2_0279
        db  005h, 000h, 000h, 000h, 000h
loc_2_0270:
        mov    word ptr [esi + 0xd8], 0x6000
loc_2_0279:
        push   0
        push   0xff
        push   0x12f14                       ; -> loc_1_12F14
        push   0
        call   sub_1_5C60
        add    esp, 0x10
        mov    byte ptr [0x133f4], al        ; -> loc_1_133F4
        mov    edi, 0x133c4                  ; -> loc_1_133C4
        sub    eax, eax
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_02B0:
        sub    esp, 4
        push   esi
        push   edi
        mov    dword ptr [0x1332f], 0        ; -> obj1:0x1332F
        call   sub_1_651E
        mov    esi, eax
        or     esi, esi
        je     loc_2_03B3
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edi, 0x12f20                  ; -> loc_1_12F20
        lea    edx, [ecx - 1]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [edx + ecx]
        push   eax
        call   sub_1_35F0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        je     loc_2_03B3
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0x12f20                  ; -> loc_1_12F20
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi - 1], eax
        mov    ax, word ptr [esi + 4]
        mov    word ptr [edi + 3], ax
        dec    edi
        mov    dword ptr [0x1332f], edx      ; -> obj1:0x1332F
        mov    dword ptr [0x13333], 0        ; -> obj1:0x13333
        mov    dword ptr [0x13337], 0        ; -> obj1:0x13337
        lea    eax, [esp + 8]
        push   eax
        push   0x12f28                       ; -> obj1:0x12F28
        call   sub_1_5C20
        add    esp, 8
        or     eax, eax
        jne    loc_2_037C
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [0x13333], eax      ; -> obj1:0x13333
loc_2_037C:
        lea    eax, [esp + 8]
        push   eax
        push   0x12f30                       ; -> obj1:0x12F30
        call   sub_1_5C20
        add    esp, 8
        or     eax, eax
        jne    loc_2_03B3
        mov    eax, dword ptr [esp + 8]
        cmp    byte ptr [eax], 0
        je     loc_2_03A6
        mov    dword ptr [0x13337], eax      ; -> obj1:0x13337
        pop    edi
        pop    esi
        add    esp, 4
        ret    
loc_2_03A6:
        mov    eax, dword ptr [esp + 8]
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_2_03B3:
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_03C0:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        sub    eax, eax
        cmp    byte ptr [ecx], al
        je     loc_2_03DF
        mov    edx, dword ptr [esp + 0xc]
        nop    
loc_2_03D0:
        movsx  esi, byte ptr [ecx]
        cmp    esi, edx
        jne    loc_2_03D9
        mov    eax, ecx
loc_2_03D9:
        inc    ecx
        cmp    byte ptr [ecx], 0
        jne    loc_2_03D0
loc_2_03DF:
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_03F0:
        push   edi
        mov    byte ptr [0x1334f], 0         ; -> obj1:0x1334F
        mov    byte ptr [0x13350], 0         ; -> loc_1_13350
        mov    byte ptr [0x13351], 0         ; -> obj1:0x13351
        mov    edi, 0x133f8                  ; -> loc_1_133F8
        sub    eax, eax
        mov    ecx, 0x25
        rep stosd dword ptr es:[edi], eax
        mov    ecx, dword ptr [0x12f80]      ; -> obj1:0x12F80
        lea    eax, [ecx + 8]
        mov    dword ptr [ecx + 0xc], eax
        mov    dword ptr [eax], eax
        mov    dword ptr [0x13278], 0x13274  ; -> loc_1_13278
        mov    dword ptr [0x13274], 0x13274  ; -> loc_1_13274
        mov    dword ptr [0x13280], 0x1327c  ; -> loc_1_13280
        mov    dword ptr [0x1327c], 0x1327c  ; -> loc_1_1327C
        mov    byte ptr [0x1331b], 0x40      ; -> obj1:0x1331B
        mov    byte ptr [0x13316], 1         ; -> loc_1_13316
        mov    dword ptr [0x133a4], 0        ; -> loc_1_133A4
        mov    dword ptr [0x13384], 0x13380  ; -> loc_1_13384
        mov    dword ptr [0x13380], 0x13380  ; -> loc_1_13380
        mov    dword ptr [0x13360], 0x1335c  ; -> loc_1_13360
        mov    dword ptr [0x1335c], 0x1335c  ; -> loc_1_1335C
        push   0
        movzx  eax, word ptr [ecx + 0x90]
        push   eax
        add    ecx, 0x80
        push   ecx
        call   sub_1_6540
        add    esp, 0xc
        or     eax, eax
        je     loc_2_04B2
        cmp    eax, 0xff
        jne    loc_2_0662
loc_2_04B2:
        mov    ecx, dword ptr [0x12f80]      ; -> obj1:0x12F80
        movzx  eax, word ptr [ecx + 0x8c]
        push   eax
        movzx  eax, word ptr [ecx + 0x8e]
        push   eax
        push   ecx
        call   sub_1_65A0
        add    esp, 0xc
        or     eax, eax
        je     loc_2_04E0
        cmp    eax, 0xff
        jne    loc_2_0662
loc_2_04E0:
        mov    dword ptr [0x13270], 0x1326c  ; -> loc_1_13270
        mov    dword ptr [0x1326c], 0x1326c  ; -> loc_1_1326C
        mov    dword ptr [0x13268], 0x13264  ; -> loc_1_13268
        mov    dword ptr [0x13264], 0x13264  ; -> loc_1_13264
        mov    dword ptr [0x13368], 0x2710   ; -> loc_1_13368
        mov    dword ptr [0x13288], 0x13284  ; -> loc_1_13288
        mov    dword ptr [0x13284], 0x13284  ; -> loc_1_13284
        mov    dword ptr [0x13290], 0x1328c  ; -> loc_1_13290
        mov    dword ptr [0x1328c], 0x1328c  ; -> loc_1_1328C
        mov    dword ptr [0x13298], 0x13294  ; -> loc_1_13298
        mov    dword ptr [0x13294], 0x13294  ; -> loc_1_13294
        mov    dword ptr [0x132a0], 0x1329c  ; -> loc_1_132A0
        mov    dword ptr [0x1329c], 0x1329c  ; -> loc_1_1329C
        mov    word ptr [0x1325c], 8         ; -> loc_1_1325C
        mov    word ptr [0x1325e], 0x48      ; -> loc_1_1325E
        mov    word ptr [0x13260], 0x37      ; -> loc_1_13260
        mov    word ptr [0x13248], 2         ; -> loc_1_13248
        push   2
        push   8
        push   0x1328c                       ; -> loc_1_1328C
        call   sub_1_FDE4
        add    esp, 0xc
        or     eax, eax
        je     loc_2_05A6
        cmp    eax, 0xff
        jne    loc_2_0662
loc_2_05A6:
        push   2
        mov    word ptr [0x1324a], 2         ; -> loc_1_1324A
        push   0x48
        push   0x13294                       ; -> loc_1_13294
        call   sub_1_FDE4
        add    esp, 0xc
        or     eax, eax
        je     loc_2_05CF
        cmp    eax, 0xff
        jne    loc_2_0662
loc_2_05CF:
        push   2
        mov    word ptr [0x1324c], 2         ; -> loc_1_1324C
        push   0x37
        push   0x1329c                       ; -> loc_1_1329C
        call   sub_1_FDE4
        add    esp, 0xc
        or     eax, eax
        je     loc_2_05F4
        cmp    eax, 0xff
        jne    loc_2_0662
loc_2_05F4:
        mov    eax, dword ptr [0x12f80]      ; -> obj1:0x12F80
        mov    word ptr [eax + 0x24], 0x56
        mov    word ptr [0x13244], 0         ; -> loc_1_13244
        push   2
        push   0x13264                       ; -> loc_1_13264
        call   sub_1_FE30
        add    esp, 8
        mov    ecx, eax
        or     ecx, ecx
        je     loc_2_062C
        cmp    ecx, 0xff
        je     loc_2_062C
        mov    eax, ecx
        pop    edi
        ret    
        db  08Dh, 049h, 000h
loc_2_062C:
        sub    eax, eax
        mov    edi, 0x1336c                  ; -> loc_1_1336C
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        mov    dword ptr [0x13490], 0x1348c  ; -> loc_1_13490
        mov    dword ptr [0x1348c], 0x1348c  ; -> loc_1_1348C
        mov    dword ptr [0x13370], 0x1336c  ; -> loc_1_13370
        mov    dword ptr [0x1336c], 0x1336c  ; -> loc_1_1336C
        mov    eax, ecx
loc_2_0662:
        pop    edi
        ret    
sub_2_0664:
        sub    esp, 4
        push   esi
        push   2
        call   sub_1_1081C
        add    esp, 4
        mov    word ptr [0x1325a], 0x41      ; -> loc_1_1325A
        mov    word ptr [0x13246], 2         ; -> loc_1_13246
        mov    dword ptr [0x132f0], 0        ; -> loc_1_132F0
        mov    dword ptr [0x132f8], 0        ; -> loc_1_132F8
        mov    dword ptr [0x132f4], 0        ; -> loc_1_132F4
        mov    dword ptr [0x132e4], 8        ; -> loc_1_132E4
        or     eax, eax
        je     loc_2_06BB
        cmp    eax, 0xff
        jne    loc_2_07CF
loc_2_06BB:
        test   byte ptr [0x13364], 1         ; -> loc_1_13364
        jne    loc_2_0714
        mov    eax, dword ptr [0x132fc]      ; -> loc_1_132FC
        mov    dword ptr [0x132e0], eax      ; -> loc_1_132E0
        mov    word ptr [0x13314], 0         ; -> loc_1_13314
        lea    eax, [esp + 4]
        push   eax
        push   0
        push   0
        push   0
        push   0xf5c0                        ; -> loc_1_F5C0
        push   0
        push   0
        mov    eax, dword ptr [0x132fc]      ; -> loc_1_132FC
        shr    eax, 3
        push   eax
        call   sub_1_109B8
        add    esp, 0x20
        or     eax, eax
        je     loc_2_070B
        cmp    eax, 0xff
        jne    loc_2_07CF
loc_2_070B:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [0x132f0], eax      ; -> loc_1_132F0
loc_2_0714:
        sub    edx, edx
        mov    ecx, 0xea60
        mov    eax, dword ptr [0x132e8]      ; -> loc_1_132E8
        div    ecx
        mov    word ptr [0x1331c], ax        ; -> loc_1_1331C
        or     ax, ax
        jne    loc_2_0736
        mov    word ptr [0x1331c], 1         ; -> loc_1_1331C
loc_2_0736:
        lea    eax, [esp + 4]
        push   eax
        push   0
        push   0
        push   0
        push   0xf7b8                        ; -> sub_1_F7B8
        push   0
        push   0
        push   0xea60
        call   sub_1_109B8
        add    esp, 0x20
        mov    esi, eax
        or     esi, esi
        je     loc_2_0774
        cmp    esi, 0xff
        je     loc_2_0774
        call   sub_2_07D4
        mov    eax, esi
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_2_0774:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [0x132f8], eax      ; -> loc_1_132F8
        cmp    dword ptr [0x13300], -1       ; -> loc_1_13300
        je     loc_2_07CD
        lea    eax, [esp + 4]
        push   eax
        push   0
        push   0
        push   0
        push   0xf938                        ; -> loc_1_F938
        push   0
        push   0
        push   dword ptr [0x13300]           ; -> loc_1_13300
        call   sub_1_109B8
        add    esp, 0x20
        mov    esi, eax
        or     esi, esi
        je     loc_2_07C4
        cmp    esi, 0xff
        je     loc_2_07C4
        call   sub_2_07D4
        mov    eax, esi
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_2_07C4:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [0x132f4], eax      ; -> loc_1_132F4
loc_2_07CD:
        sub    eax, eax
loc_2_07CF:
        pop    esi
        add    esp, 4
        ret    
sub_2_07D4:
        cmp    dword ptr [0x132f0], 0        ; -> loc_1_132F0
        je     loc_2_07EF
        push   0
        push   0
        push   dword ptr [0x132f0]           ; -> loc_1_132F0
        call   sub_1_10AEC
        add    esp, 0xc
loc_2_07EF:
        cmp    dword ptr [0x132f4], 0        ; -> loc_1_132F4
        je     loc_2_080A
        push   0
        push   0
        push   dword ptr [0x132f4]           ; -> loc_1_132F4
        call   sub_1_10AEC
        add    esp, 0xc
loc_2_080A:
        cmp    dword ptr [0x132f8], 0        ; -> loc_1_132F8
        je     loc_2_0825
        push   0
        push   0
        push   dword ptr [0x132f8]           ; -> loc_1_132F8
        call   sub_1_10AEC
        add    esp, 0xc
loc_2_0825:
        sub    eax, eax
        ret    
sub_2_0828:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        push   esi
        call   sub_3_04D0
        add    esp, 8
        push   0
        push   0x89
        push   0x12f84                       ; -> loc_1_12F84
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        mov    word ptr [esi + 0xdc], ax
        push   0
        push   0x35
        push   0x12f94                       ; -> loc_1_12F94
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        mov    word ptr [esi + 0xde], ax
        push   1
        push   0x11
        push   0x12fa4                       ; -> obj1:0x12FA4
        push   0
        call   sub_1_5C60
        add    esp, 0x10
        mov    word ptr [esi + 0x117], ax
        push   1
        push   0xff
        push   0x12fb4                       ; -> obj1:0x12FB4
        push   0
        call   sub_1_5C60
        add    esp, 0x10
        mov    word ptr [esi + 0x119], ax
        push   0xea60
        push   0x57e40
        push   0x12fc8                       ; -> loc_1_12FC8
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        mov    dword ptr [esi + 0xb4], eax
        push   0xea600
        push   0xea600
        push   0x12fd8                       ; -> obj1:0x12FD8
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        mov    dword ptr [esi + 0xc8], eax
        push   1
        push   3
        push   0x12fec                       ; -> loc_1_12FEC
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        mov    word ptr [esi + 0x9e], ax
        push   0x64
        push   0x2ee
        push   0x13000                       ; -> loc_1_13000
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        mov    dword ptr [esi + 0x94], eax
        push   1
        push   3
        push   0x13014                       ; -> loc_1_13014
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        mov    word ptr [esi + 0x9c], ax
        push   0x64
        push   0x5dc
        push   0x13028                       ; -> loc_1_13028
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        mov    dword ptr [esi + 0x98], eax
        push   0xea60
        push   -1
        push   0x1303c                       ; -> loc_1_1303C
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        mov    dword ptr [esi + 0xcc], eax
        push   0
        push   0
        push   0x13050                       ; -> loc_1_13050
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        mov    byte ptr [esi + 0x11e], al
        push   0
        push   0
        push   0x13060                       ; -> loc_1_13060
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        mov    byte ptr [esi + 0x11d], al
        push   0
        push   0
        push   0x13070                       ; -> loc_1_13070
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        mov    byte ptr [esi + 0x11f], al
        push   0x3e8
        push   0x1770
        push   0x1307c                       ; -> loc_1_1307C
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        mov    dword ptr [esi + 0xef], eax
        push   0x20000
        push   0x20000
        push   0x1308c                       ; -> obj1:0x1308C
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        mov    dword ptr [esi + 0xeb], eax
        push   0
        push   0
        push   0x130a0                       ; -> obj1:0x130A0
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        mov    byte ptr [esi + 0x121], al
        push   0x3e8
        push   0x3a98
        push   0x130b4                       ; -> loc_1_130B4
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        mov    dword ptr [esi + 0xb8], eax
        push   8
        push   8
        push   0x130c4                       ; -> loc_1_130C4
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        cmp    eax, 9
        mov    word ptr [esi + 0x124], 0x48
        je     loc_2_0A4A
        mov    word ptr [esi + 0x124], 0x40
loc_2_0A4A:
        push   0
        push   0
        push   0x130d4                       ; -> obj1:0x130D4
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        or     eax, eax
        je     loc_2_0A6D
        cmp    eax, 0x30
        jae    loc_2_0A6D
        or     word ptr [0x13364], 0x10      ; -> loc_1_13364
loc_2_0A6D:
        mov    ax, word ptr [0x13364]        ; -> loc_1_13364
        and    ax, 0x10
        cmp    ax, 1
        sbb    eax, eax
        and    eax, 0xfffffffe
        add    eax, 3
        push   eax
        push   eax
        push   0x130e0                       ; -> obj1:0x130E0
        push   edi
        call   sub_1_5C60
        add    esp, 0x10
        cmp    eax, 2
        je     loc_2_0AE8
        cmp    eax, 3
        je     loc_2_0B2C
        mov    word ptr [esi + 0x8a], 0xa
        mov    word ptr [esi + 0x8c], 0xa
        mov    word ptr [esi + 0x90], 0x10
        mov    word ptr [esi + 0x8e], 0x10
        mov    word ptr [esi + 0x1a], 0xffff
        mov    word ptr [esi + 0x1c], 0x14
        mov    word ptr [esi + 0x1e], 0xc8
        mov    word ptr [esi + 0x20], 0xc8
        mov    word ptr [esi + 0x22], 0xc8
        jmp    loc_2_0B6E
loc_2_0AE8:
        mov    word ptr [esi + 0x8a], 0x14
        mov    word ptr [esi + 0x8c], 0x64
        mov    word ptr [esi + 0x90], 0x80
        mov    word ptr [esi + 0x8e], 0x80
        mov    word ptr [esi + 0x1a], 0x3e8
        mov    word ptr [esi + 0x1c], 0x3e8
        mov    word ptr [esi + 0x1e], 0x3e8
        mov    word ptr [esi + 0x20], 0x3e8
        mov    word ptr [esi + 0x22], 0x3e8
        jmp    loc_2_0B6E
loc_2_0B2C:
        mov    word ptr [esi + 0x8a], 0xffff
        mov    word ptr [esi + 0x8c], 0xff
        mov    word ptr [esi + 0x90], 0xff
        mov    word ptr [esi + 0x8e], 0xff
        mov    word ptr [esi + 0x1a], 0xffff
        mov    word ptr [esi + 0x1c], 0xffff
        mov    word ptr [esi + 0x1e], 0xffff
        mov    word ptr [esi + 0x20], 0xffff
        mov    word ptr [esi + 0x22], 0xffff
loc_2_0B6E:
        push   edi
        push   esi
        call   sub_2_0B7C
        add    esp, 8
        pop    edi
        pop    esi
        ret    
        db  090h
sub_2_0B7C:
        sub    esp, 4
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 0xf3]
        or     eax, eax
        je     loc_2_0B96
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_2_0B96:
        lea    eax, [esp]
        push   eax
        push   0x13134                       ; -> obj1:0x13134
        call   sub_1_5C20
        add    esp, 8
        or     eax, eax
        je     loc_2_0BCC
        cmp    eax, 0xff
        je     loc_2_0BCC
        mov    dword ptr [0x13327], 0        ; -> obj1:0x13327
        mov    dword ptr [0x1332b], 0        ; -> obj1:0x1332B
        add    esp, 4
        ret    
        db  090h
loc_2_0BCC:
        push   0x5c
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [0x13327], eax      ; -> obj1:0x13327
        push   dword ptr [esp + 4]
        call   sub_2_03C0
        add    esp, 8
        or     eax, eax
        je     loc_2_0BF8
        sub    eax, dword ptr [esp]
        inc    eax
        mov    dword ptr [0x1332b], eax      ; -> obj1:0x1332B
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_2_0BF8:
        add    esp, 4
        mov    dword ptr [0x13327], 0        ; -> obj1:0x13327
        mov    dword ptr [0x1332b], 0        ; -> obj1:0x1332B
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x6dc  R-X
sub_3_0000:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0x96
        call   sub_1_35F0
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_3_0021
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_0021:
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 0x25
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dword ptr [ebx + 8], 0x54637281
        mov    byte ptr [ebx + 0x52], al
        mov    byte ptr [ebx + 0x92], 4
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [ebx + 0x74], ecx
        mov    edx, dword ptr [esp + 0x24]
        mov    dword ptr [ebx + 0x78], edx
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [ebx + 0x7c], eax
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [ebx + 0x80], eax
        mov    esi, dword ptr [esp + 0x30]
        lea    edi, [ebx + 0x8c]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi], eax
        mov    ax, word ptr [esi + 4]
        mov    word ptr [edi + 4], ax
        mov    ax, word ptr [0x13364]        ; -> loc_1_13364
        mov    esi, eax
        and    esi, 0xffff
        test   ah, 0x10
        je     loc_3_00A9
        or     ecx, ecx
        jne    loc_3_008F
        or     edx, edx
        je     loc_3_00A9
loc_3_008F:
        mov    word ptr [0x13364], 8         ; -> loc_1_13364
        test   esi, 0x10
        je     loc_3_00A9
        mov    word ptr [0x13364], 0x18      ; -> loc_1_13364
loc_3_00A9:
        call   sub_1_4C90
        mov    esi, eax
        or     esi, esi
        je     loc_3_00D0
        cmp    esi, 0xff
        je     loc_3_00D0
        push   ebx
        call   sub_1_0024
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_00D0:
        lea    eax, [ebx + 0xc]
        mov    dword ptr [ebx + 0x10], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 0x14]
        mov    dword ptr [ebx + 0x18], eax
        mov    dword ptr [eax], eax
        lea    esi, [ebx + 0x1c]
        mov    dword ptr [ebx + 0x20], esi
        mov    dword ptr [esi], esi
        lea    eax, [ebx + 0x24]
        mov    dword ptr [ebx + 0x28], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 0x34]
        mov    dword ptr [ebx + 0x38], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 0x2c]
        mov    dword ptr [ebx + 0x30], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 0x3c]
        mov    dword ptr [ebx + 0x40], eax
        mov    dword ptr [eax], eax
        mov    edi, dword ptr [esp + 0x14]
        mov    ecx, 1
        mov    eax, dword ptr [edi + 8]
        lea    ebp, [edi + 8]
        mov    edx, edi
        sub    edx, eax
        cmp    edx, -8
        je     loc_3_0135
loc_3_0120:
        test   dword ptr [eax + 0x88], ecx
        je     loc_3_0135
        add    ecx, ecx
        mov    eax, dword ptr [eax]
        mov    edx, edi
        sub    edx, eax
        cmp    edx, -8
        jne    loc_3_0120
loc_3_0135:
        mov    dword ptr [ebx + 0x88], ecx
        mov    al, byte ptr [esp + 0x34]
        mov    byte ptr [ebx + 0x50], al
        inc    byte ptr [0x1334f]            ; -> obj1:0x1334F
        cmp    byte ptr [0x1334f], 1         ; -> obj1:0x1334F
        jbe    loc_3_0158
        mov    byte ptr [0x13350], 1         ; -> loc_1_13350
loc_3_0158:
        mov    eax, dword ptr [esp + 0x1c]
        push   ebx
        push   eax
        mov    eax, dword ptr [esp + 0x20]
        push   eax
        call   sub_3_0380
        add    esp, 0xc
        or     eax, eax
        je     loc_3_017A
        cmp    eax, 0xff
        jne    loc_3_025F
loc_3_017A:
        mov    ax, word ptr [edi + 0x117]
        cmp    ax, 0xfe
        jb     loc_3_018B
        mov    ax, 0xfe
loc_3_018B:
        mov    byte ptr [ebx + 0x4e], al
        mov    cx, word ptr [edi + 0x119]
        cmp    cx, 0xfe
        jb     loc_3_01A0
        mov    cx, 0xfe
loc_3_01A0:
        mov    byte ptr [ebx + 0x4f], cl
        and    ax, 0xff
        lea    eax, [eax*4 + 4]
        push   eax
        call   sub_1_35F0
        add    esp, 4
        mov    dword ptr [ebx + 0x44], eax
        or     eax, eax
        je     loc_3_025A
        mov    al, byte ptr [ebx + 0x4f]
        and    eax, 0xffff00ff
        lea    eax, [eax*4 + 4]
        push   eax
        call   sub_1_35F0
        add    esp, 4
        mov    dword ptr [ebx + 0x48], eax
        or     eax, eax
        je     loc_3_025A
        xor    eax, eax
        mov    al, byte ptr [ebx + 0x4e]
        lea    ecx, [eax*4 + 4]
        mov    edi, dword ptr [ebx + 0x44]
        sub    eax, eax
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        xor    eax, eax
        mov    al, byte ptr [ebx + 0x4f]
        lea    ecx, [eax*4 + 4]
        mov    edi, dword ptr [ebx + 0x48]
        sub    eax, eax
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    byte ptr [ebx + 0x4c], 1
        mov    byte ptr [ebx + 0x4d], 1
        push   ebx
        push   2
        push   0x49
        push   esi
        call   sub_1_104EC
        add    esp, 0x10
        or     eax, eax
        je     loc_3_022E
        cmp    eax, 0xff
        jne    loc_3_025F
loc_3_022E:
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [ebx], ebp
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [eax], ebx
        mov    dword ptr [ebp + 4], ebx
        push   ebx
        call   sub_1_FBB8
        add    esp, 4
        or     eax, eax
        je     loc_3_024F
        cmp    eax, 0xff
        jne    loc_3_025F
loc_3_024F:
        mov    byte ptr [ebx + 0x52], 1
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_025A:
        mov    eax, 1
loc_3_025F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0270:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        mov    ecx, 0x12efc                  ; -> loc_1_12EFC
        mov    edx, dword ptr [esp + 0x18]
loc_3_0281:
        mov    eax, dword ptr [ecx]
        mov    edi, edx
loc_3_0285:
        mov    bl, byte ptr [eax]
        cmp    bl, byte ptr [edi]
        jne    loc_3_02A5
        or     bl, bl
        je     loc_3_02A1
        mov    bl, byte ptr [eax + 1]
        cmp    bl, byte ptr [edi + 1]
        jne    loc_3_02A5
        add    eax, 2
        add    edi, 2
        or     bl, bl
        jne    loc_3_0285
loc_3_02A1:
        sub    eax, eax
        jmp    loc_3_02AA
loc_3_02A5:
        sbb    eax, eax
        sbb    eax, -1
loc_3_02AA:
        or     eax, eax
        je     loc_3_02C5
        add    ecx, 8
        inc    esi
        cmp    ecx, 0x12f14                  ; -> loc_1_12F14
        jb     loc_3_0281
        mov    eax, dword ptr [esp + 0x1c]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_02C5:
        mov    esi, dword ptr [esi*8 + 0x12f00] ; -> loc_1_12F00
        cmp    esi, 0x2f
        je     loc_3_0320
        mov    dword ptr [esp + 0xc], 4
        lea    eax, [esp + 0xc]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        push   esi
        mov    eax, dword ptr [0x12ed0]      ; -> obj1:0x12ED0
        push   eax
        call   sub_1_0071
        add    esp, 0x10
        or     eax, eax
        je     loc_3_030B
        cmp    eax, 9
        je     loc_3_030B
        cmp    eax, 0x1a
        mov    eax, dword ptr [esp + 0x1c]
        pop    edi
        pop    esi
        pop    ebx
        je     loc_3_0316
        add    esp, 8
        ret    
loc_3_030B:
        mov    eax, dword ptr [esp + 0x10]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_0316:
        add    esp, 8
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_0320:
        mov    dword ptr [esp + 0xc], 0x100
        push   0x100
        call   sub_1_35F0
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_3_0344
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_0344:
        lea    eax, [esp + 0xc]
        push   eax
        push   edi
        push   esi
        mov    eax, dword ptr [0x12ed0]      ; -> obj1:0x12ED0
        push   eax
        call   sub_1_0071
        add    esp, 0x10
        or     eax, eax
        jne    loc_3_0366
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_0366:
        push   edi
        call   sub_1_0024
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_3_0380:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x40]
        mov    ebp, dword ptr [esp + 0x38]
        lea    ebx, [esi + 0x60]
        mov    dword ptr [esi + 0x58], ebp
        mov    eax, dword ptr [esp + 0x3c]
        mov    dword ptr [esi + 0x5c], eax
        cmp    byte ptr [0x1331e], 0         ; -> loc_1_1331E
        je     loc_3_03AC
        mov    eax, dword ptr [0x13304]      ; -> loc_1_13304
        jmp    loc_3_03B0
loc_3_03AC:
        not    eax
        or     eax, ebp
loc_3_03B0:
        mov    dword ptr [ebx], eax
        push   0
        push   ebp
        push   0x8a
        lea    eax, [esi + 0x6c]
        push   esi
        push   eax
        push   0
        push   0
        call   sub_1_3D60
        add    esp, 0x1c
        mov    edi, eax
        or     edi, edi
        je     loc_3_03DD
        cmp    edi, 0xff
        jne    loc_3_04A6
loc_3_03DD:
        push   0
        push   ebp
        push   0x89
        push   esi
        lea    eax, [esi + 0x68]
        push   eax
        push   0
        push   0
        call   sub_1_3D60
        add    esp, 0x1c
        mov    edi, eax
        or     edi, edi
        je     loc_3_0408
        cmp    edi, 0xff
        jne    loc_3_049A
loc_3_0408:
        push   3
        push   ebp
        push   0x8b
        push   esi
        lea    eax, [esi + 0x70]
        push   eax
        push   0
        push   0
        call   sub_1_3D60
        add    esp, 0x1c
        mov    edi, eax
        or     edi, edi
        je     loc_3_042F
        cmp    edi, 0xff
        jne    loc_3_048E
loc_3_042F:
        lea    eax, [esp + 0x22]
        mov    word ptr [esp + 0x20], 3
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0xc], 0
        mov    byte ptr [esp + 0x22], 0x2a
        push   0
        push   esi
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        lea    eax, [esp + 0x30]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        call   sub_1_A19C
        add    esp, 0x18
        mov    edi, eax
        or     edi, edi
        je     loc_3_04B0
        cmp    edi, 0xff
        je     loc_3_04B0
        mov    eax, dword ptr [esi + 0x70]
        push   eax
        call   sub_1_3840
        add    esp, 4
loc_3_048E:
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        call   sub_1_3840
        add    esp, 4
loc_3_049A:
        mov    eax, dword ptr [esi + 0x6c]
        push   eax
        call   sub_1_3840
        add    esp, 4
loc_3_04A6:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
loc_3_04B0:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x54], eax
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_04D0:
        push   esi
        push   edi
        push   0
        push   0
        push   0x130f8                       ; -> obj1:0x130F8
        mov    esi, dword ptr [esp + 0x1c]
        push   esi
        call   sub_1_5C60
        add    esp, 0x10
        cmp    eax, 1
        je     loc_3_0508
        cmp    eax, 2
        je     loc_3_0514
        cmp    eax, 4
        je     loc_3_0520
        cmp    eax, 8
        je     loc_3_052C
        mov    word ptr [0x13364], 0x1001    ; -> loc_1_13364
        jmp    loc_3_0535
        db  090h
loc_3_0508:
        mov    word ptr [0x13364], 1         ; -> loc_1_13364
        jmp    loc_3_0535
        db  090h
loc_3_0514:
        mov    word ptr [0x13364], 2         ; -> loc_1_13364
        jmp    loc_3_0535
        db  090h
loc_3_0520:
        mov    word ptr [0x13364], 4         ; -> loc_1_13364
        jmp    loc_3_0535
        db  090h
loc_3_052C:
        mov    word ptr [0x13364], 8         ; -> loc_1_13364
loc_3_0535:
        push   0
        push   -1
        push   0x13104                       ; -> loc_1_13104
        push   esi
        call   sub_1_5CB0
        add    esp, 0x10
        mov    edi, eax
        push   0
        push   0
        push   0x13118                       ; -> loc_1_13118
        push   esi
        call   sub_1_5CB0
        add    esp, 0x10
        cmp    eax, edi
        je     loc_3_0568
        mov    byte ptr [0x1331e], 0         ; -> loc_1_1331E
        jmp    loc_3_0574
loc_3_0568:
        mov    dword ptr [0x13304], eax      ; -> loc_1_13304
        mov    byte ptr [0x1331e], 1         ; -> loc_1_1331E
loc_3_0574:
        push   esi
        push   dword ptr [esp + 0x10]
        call   sub_3_0584
        add    esp, 8
        pop    edi
        pop    esi
        ret    
sub_3_0584:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 0x10]
        push   eax
        push   0x1312c                       ; -> loc_1_1312C
        call   sub_1_5C20
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        je     loc_3_05AF
        cmp    ebp, 0xff
        jne    loc_3_0672
loc_3_05AF:
        sub    eax, eax
        mov    edi, dword ptr [esp + 0x10]
        mov    ecx, 0xffffffff
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        je     loc_3_0672
        mov    eax, dword ptr [esp + 0x10]
        cmp    byte ptr [eax], 0x2a
        jne    loc_3_05D1
        mov    byte ptr [eax], 0
loc_3_05D1:
        sub    eax, eax
        mov    edi, dword ptr [esp + 0x10]
        mov    ecx, 0xffffffff
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    ebx, [ecx - 1]
        cmp    ebx, 0xff
        ja     loc_3_0672
        lea    eax, [ebx + 2]
        push   eax
        call   sub_1_35F0
        add    esp, 4
        or     eax, eax
        je     loc_3_0666
        lea    edx, [eax + 1]
        mov    edi, edx
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    byte ptr [ebx + eax + 1], 0
        mov    edi, 2
        mov    esi, eax
        mov    cl, byte ptr [edx]
        or     cl, cl
        je     loc_3_0651
loc_3_0628:
        inc    edi
        cmp    cl, 0x2e
        jne    loc_3_0641
        mov    ecx, edx
        sub    ecx, esi
        dec    cl
        mov    byte ptr [esi], cl
        cmp    cl, 0x3f
        ja     loc_3_064C
        or     cl, cl
        je     loc_3_064C
        mov    esi, edx
loc_3_0641:
        inc    edx
        mov    cl, byte ptr [edx]
        or     cl, cl
        jne    loc_3_0628
        jmp    loc_3_0651
        db  08Bh, 0FFh
loc_3_064C:
        mov    ebp, 0xc0000001
loc_3_0651:
        or     ebp, ebp
        je     loc_3_06B0
        cmp    ebp, 0xff
        je     loc_3_06B0
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_3_0666:
        push   dword ptr [esp + 0x10]
        call   sub_1_0024
        add    esp, 4
loc_3_0672:
        mov    esi, dword ptr [esp + 0x18]
        mov    word ptr [esi + 0xa0], 1
        mov    eax, dword ptr [esi + 0xa4]
        or     eax, eax
        je     loc_3_0692
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_3_0692:
        push   1
        call   sub_1_35F0
        add    esp, 4
        mov    dword ptr [esi + 0xa4], eax
        mov    byte ptr [eax], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_3_06B0:
        sub    edx, esi
        dec    dl
        mov    byte ptr [esi], dl
        mov    esi, dword ptr [esp + 0x18]
        mov    word ptr [esi + 0xa0], di
        mov    dword ptr [esi + 0xa4], eax
        push   dword ptr [esp + 0x10]
        call   sub_1_0024
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
VxD__LTEXT ENDS
