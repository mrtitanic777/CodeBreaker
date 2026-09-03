; IBMTOK -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 7464 instructions recovered by recursive descent

Declare_Virtual_Device IBMTOK, 3, 10, IBMTOK_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x56d0  R-X
loc_1_0000:
        mov    al, byte ptr [0x7c8]          ; -> obj1:0x07C8
        cmp    al, 0
        jne    loc_1_0025
        inc    byte ptr [0x7c8]              ; -> obj1:0x07C8
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0027
        push   0
        push   0
        call   sub_5_0000
        cmp    eax, 0
        jne    loc_1_0027
loc_1_0025:
        clc    
        ret    
loc_1_0027:
        stc    
        ret    
IBMTOK_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_0038
        jmp    dword ptr [eax*4 + 0x3a]      ; -> obj1:0x003A
loc_1_0038:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_0000
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0000
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_00B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_00E0
        pop    ebp
        ret    0x1c
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_00E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0x10]
        add    ecx, 2
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, 8
        cmp    dword ptr [ebp + 0x14], 0xe
        jb     loc_1_0200
        mov    al, byte ptr [eax]
        shr    al, 7
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ecx]
        and    al, 0x80
        mov    byte ptr [ebp - 2], al
        or     al, al
        je     loc_1_01A4
        mov    al, byte ptr [ecx]
        cmp    al, 0xff
        jne    loc_1_0122
        cmp    byte ptr [ecx + 1], 0xff
        je     loc_1_012C
loc_1_0122:
        cmp    al, 0xc0
        jne    loc_1_0148
        cmp    byte ptr [ecx + 1], 0
        jne    loc_1_0148
loc_1_012C:
        cmp    byte ptr [ecx + 2], 0xff
        jne    loc_1_0148
        cmp    byte ptr [ecx + 3], 0xff
        jne    loc_1_0148
        cmp    byte ptr [ecx + 4], 0xff
        jne    loc_1_0148
        cmp    byte ptr [ecx + 5], 0xff
        mov    byte ptr [ebp - 2], 1
        je     loc_1_014C
loc_1_0148:
        mov    byte ptr [ebp - 2], 0
loc_1_014C:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_0160
        mov    dword ptr [ebp - 8], 8
        jmp    loc_1_020B
        db  08Bh, 0FFh
loc_1_0160:
        mov    al, byte ptr [ecx + 2]
        and    al, byte ptr [ecx]
        and    al, 0x80
        mov    byte ptr [ebp - 2], al
        or     al, al
        mov    dword ptr [ebp - 8], 0x1000
        jne    loc_1_017C
        mov    dword ptr [ebp - 8], 0x4000
loc_1_017C:
        xor    edx, edx
        mov    dl, byte ptr [ecx + 4]
        xor    eax, eax
        mov    al, byte ptr [ecx + 2]
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [ecx + 3]
        shl    eax, 0x10
        or     edx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 5]
        or     edx, eax
        mov    dword ptr [ebp - 0xc], edx
        jmp    loc_1_020B
loc_1_01A4:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x10], 0x30
        je     loc_1_01F0
        lea    edx, [esi + 0x18]
        mov    eax, dword ptr [ecx + 2]
        cmp    dword ptr [edx + 2], eax
        jne    loc_1_01C7
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_01CE
loc_1_01C7:
        mov    dword ptr [ebp - 0x10], 1
loc_1_01CE:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_01E0
        mov    dword ptr [ebp - 8], 1
        jmp    loc_1_020E
        db  08Dh, 049h, 000h
loc_1_01E0:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_020E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_01F0:
        mov    dword ptr [ebp - 8], 1
        jmp    loc_1_020E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0200:
        mov    dword ptr [ebp - 8], 0x20
        mov    byte ptr [ebp - 1], 0
loc_1_020B:
        mov    esi, dword ptr [ebp + 8]
loc_1_020E:
        mov    edi, dword ptr [esi + 0x14]
        or     edi, edi
        je     loc_1_02F1
loc_1_0219:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, ecx
        and    eax, dword ptr [ebp - 8]
        test   al, 9
        jne    loc_1_025D
        test   ah, 0x40
        je     loc_1_0232
        mov    eax, dword ptr [edi + 0xc]
        test   dword ptr [ebp - 0xc], eax
        jne    loc_1_025D
loc_1_0232:
        test   byte ptr [ebp - 7], 0x40
        je     loc_1_023D
        test   ch, 0x20
        jne    loc_1_025D
loc_1_023D:
        test   byte ptr [ebp - 7], 0x10
        je     loc_1_0249
        cmp    byte ptr [edi + 0x1d], 0
        jne    loc_1_025D
loc_1_0249:
        test   cl, 0x10
        je     loc_1_0254
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_025D
loc_1_0254:
        test   cl, 0x20
        je     loc_1_02E6
loc_1_025D:
        inc    dword ptr [edi + 0x18]
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_0592
        mov    byte ptr [edi + 0x1e], 1
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_1_02E6
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_029F
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_029F:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_02B0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_02B6
        db  08Bh, 0FFh
loc_1_02B0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_1_02B6:
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_1_02C5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0598
loc_1_02C5:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x2a]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x1c]
        shl    eax, cl
        or     dword ptr [esi + 0x2e], eax
        push   0
        push   0x1f
        push   edi
        call   sub_1_063C
        mov    edi, ebx
        jmp    loc_1_02E9
loc_1_02E6:
        mov    edi, dword ptr [edi + 0x10]
loc_1_02E9:
        or     edi, edi
        jne    loc_1_0219
loc_1_02F1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0300:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0310
        pop    ebp
        ret    4
sub_1_0310:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x14]
        or     edi, edi
        je     loc_1_038E
loc_1_0320:
        inc    dword ptr [edi + 0x18]
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_1_0336
        mov    byte ptr [edi + 0x1e], 0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0598
loc_1_0336:
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_1_0387
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_034C
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_034C:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_0360
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_0366
        db  005h, 000h, 000h, 000h, 000h
loc_1_0360:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_1_0366:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x2a]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x1c]
        shl    eax, cl
        or     dword ptr [esi + 0x2e], eax
        push   0
        push   0x1f
        push   edi
        call   sub_1_063C
        mov    edi, ebx
        jmp    loc_1_038A
loc_1_0387:
        mov    edi, dword ptr [edi + 0x10]
loc_1_038A:
        or     edi, edi
        jne    loc_1_0320
loc_1_038E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    edx, dword ptr [ebp + 0x10]
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 4]
        xor    eax, eax
        mov    al, byte ptr [edx + 2]
        shl    eax, 0x10
        or     ecx, eax
        shl    ecx, 8
        xor    eax, eax
        mov    al, byte ptr [edx + 3]
        shl    eax, 0x10
        or     ecx, eax
        xor    eax, eax
        mov    al, byte ptr [edx + 5]
        or     ecx, eax
        mov    al, byte ptr [edx + 2]
        and    al, byte ptr [edx]
        and    al, 0x80
        mov    byte ptr [ebp - 1], al
        or     al, al
        mov    eax, dword ptr [ebp + 8]
        je     loc_1_03F2
        cmp    dword ptr [eax + 8], ecx
        jne    loc_1_03F2
        cmp    dword ptr [eax + 0xc], 0
        je     loc_1_03F2
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_03F2:
        mov    esi, dword ptr [eax + 0x10]
        or     esi, esi
        jne    loc_1_0402
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0402:
        test   esi, 0x20
        je     loc_1_0413
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0413:
        mov    al, byte ptr [edx]
        shr    al, 7
        or     al, al
        je     loc_1_0430
        test   esi, 0x10
        je     loc_1_0430
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
loc_1_0430:
        mov    eax, dword ptr [ebp + 0xc]
        mov    cl, byte ptr [eax]
        and    cl, 0x80
        mov    byte ptr [ebp - 1], cl
        or     cl, cl
        je     loc_1_04BA
        mov    cl, byte ptr [eax]
        cmp    cl, 0xff
        jne    loc_1_044C
        cmp    byte ptr [eax + 1], 0xff
        je     loc_1_0457
loc_1_044C:
        cmp    cl, 0xc0
        jne    loc_1_0473
        cmp    byte ptr [eax + 1], 0
        jne    loc_1_0473
loc_1_0457:
        cmp    byte ptr [eax + 2], 0xff
        jne    loc_1_0473
        cmp    byte ptr [eax + 3], 0xff
        jne    loc_1_0473
        cmp    byte ptr [eax + 4], 0xff
        jne    loc_1_0473
        cmp    byte ptr [eax + 5], 0xff
        mov    byte ptr [ebp - 1], 1
        je     loc_1_0477
loc_1_0473:
        mov    byte ptr [ebp - 1], 0
loc_1_0477:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_04A0
        test   esi, 8
        je     loc_1_0490
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_1_0490:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_04A0:
        test   esi, 0x6000
        je     loc_1_04B1
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_04B1:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_04BA:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 056h, 08Bh, 04Ch
        db  024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh, 05Eh, 0C2h, 00Ch
        db  000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh
        db  04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh
        db  07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh
        db  04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h, 06Dh, 05Fh
        db  0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h
        db  0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_054A:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_0550:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_0556:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_055C:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_0562:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_0568:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
sub_1_056E:
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_0574:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_057A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_0586:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
sub_1_058C:
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_0592:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_0598:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_059E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_05A4:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
sub_1_05AA:
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
sub_1_05B0:
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_05BC:
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
        VxDCall  VPOWERD.Service_804Fh           ; cd204f802800
        VxDCall  VPOWERD.Service_8057h           ; cd2057802800
        VxDCall  VPOWERD.Service_8058h           ; cd2058802800
        VxDCall  VPOWERD.Service_8059h           ; cd2059802800
        VxDCall  VPOWERD.Service_805Ah           ; cd205a802800
        VxDCall  VPOWERD.Service_805Bh           ; cd205b802800
        VxDCall  VPOWERD.Service_805Ch           ; cd205c802800
        VxDCall  VPOWERD.Service_805Dh           ; cd205d802800
        VxDCall  VPOWERD.Service_805Eh           ; cd205e802800
sub_1_0604:
        VxDCall  VPOWERD.Service_8056h           ; cd2056802800
        add    byte ptr [eax], al
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_0612:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_0618:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_061E:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_0624:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_062A:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_0630:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_0636:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_063C:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_0660:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_0684:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_06A8:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_06B4:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_06BA:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_06C0:
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
sub_1_0702:
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_0708:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
sub_1_070E:
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
sub_1_0714:
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_071C:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x14]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        ret    0xc
        db  056h, 057h, 09Ch, 08Bh, 04Ch, 024h, 018h, 08Bh, 074h, 024h, 014h, 08Bh, 07Ch, 024h, 010h, 03Bh
        db  0F7h, 073h, 00Dh, 00Bh, 0C9h, 074h, 01Fh, 0FDh, 049h, 003h, 0F1h, 003h, 0F9h, 041h, 0EBh, 014h
        db  08Bh, 0C6h, 02Bh, 0C7h, 083h, 0F8h, 004h, 072h, 00Bh, 08Ah, 0C1h, 024h, 003h, 0C1h, 0E9h, 002h
        db  0F3h, 0A5h, 08Ah, 0C8h, 0F3h, 0A4h, 09Dh, 05Fh, 05Eh, 0C2h, 00Ch, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'IBMTOK  '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  IBMTOK_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_07D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0A00
        push   esi
        call   sub_1_0A50
        lea    edi, [ebp - 0x2c]
        sub    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        cmp    byte ptr [ebp + 0x14], 1
        sbb    eax, eax
        and    eax, 0xfffffffe
        add    eax, 3
        mov    dword ptr [ebp - 0x22], eax
        mov    dword ptr [ebp - 0x16], 1
        mov    eax, dword ptr [esi + 0x59]
        mov    dword ptr [ebp - 0x12], eax
        lea    edi, [esi + 0x81]
        mov    dword ptr [ebp - 0xe], 4
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 0x7d]
        push   eax
        push   edi
        call   sub_1_0562
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_1_0844
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_0844:
        cmp    byte ptr [ebp + 0x18], 0
        je     loc_1_0871
        push   0
        push   0xaa00000a
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0708
        add    esp, 0xc
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0568
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_0871:
        push   esi
        call   sub_1_2480
        or     al, al
        jne    loc_1_0890
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0568
        mov    eax, 0xc0010006
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_0890:
        cmp    byte ptr [esi + 0x3a], 0
        je     loc_1_08A3
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e18], 0xc0
loc_1_08A3:
        mov    edx, dword ptr [esi + 0x59]
        inc    dx
        sub    al, al
        out    dx, al
        push   0xc350
        call   sub_1_0630
        mov    ecx, dword ptr [esi + 0x59]
        add    cx, 2
        mov    dx, cx
        sub    al, al
        out    dx, al
        cmp    byte ptr [esi + 0x38], 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x33]
        push   eax
        push   eax
        push   0x3540                        ; -> loc_1_3540
        push   esi
        push   0x3360                        ; -> loc_1_3360
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [ebp - 4]
        push   esi
        push   eax
        call   sub_1_061E
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_09CC
        push   esi
        call   sub_1_0AE0
        or     eax, eax
        je     loc_1_0931
        push   1
        push   0
        push   2
        push   0xaa000004
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0708
        add    esp, 0x14
        push   esi
        call   sub_1_0624
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0568
        mov    eax, 0xc0010006
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_0931:
        lea    eax, [esi + 0x85]
        push   eax
        lea    eax, [esi + 0x79]
        push   eax
        lea    eax, [esi + 0x4d]
        push   eax
        push   0x1ca0                        ; -> loc_1_1CA0
        push   0x1b30                        ; -> loc_1_1B30
        push   0x1c30                        ; -> loc_1_1C30
        push   0x1bc0                        ; -> loc_1_1BC0
        call   sub_2_0000
        or     al, al
        jne    loc_1_0990
        push   2
        push   0
        push   2
        push   0xaa000002
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0708
        add    esp, 0x14
        push   esi
        call   sub_1_0624
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0568
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_1_0990:
        lea    eax, [esi + 0x20]
        mov    dword ptr [esi + 0x1c], 0x53b0 ; -> loc_1_53B0
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [esi + 0x1c]
        mov    dword ptr [esi + 0x24], ecx
        mov    dword ptr [esi + 0x28], esi
        push   0x7530
        push   eax
        call   sub_1_0612
        push   0x23f0                        ; -> loc_1_23F0
        push   esi
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_070E
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_09CC:
        push   3
        push   0
        push   2
        push   0xaa000005
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0708
        add    esp, 0x14
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0568
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A00:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [ecx + 0x61]
        mov    dword ptr [ecx + 0x65], eax
        mov    dword ptr [eax], eax
        lea    eax, [ecx + 0x69]
        mov    dword ptr [ecx + 0x6d], eax
        mov    dword ptr [eax], eax
        lea    eax, [ecx + 0x71]
        mov    dword ptr [ecx + 0x75], eax
        mov    dword ptr [eax], eax
        mov    byte ptr [ecx + 0x16f], 1
        mov    byte ptr [ecx + 0x171], 1
        mov    word ptr [ecx + 0xe5], 0x100
        mov    word ptr [ecx + 0xdb], 1
        mov    byte ptr [ecx + 0x16e], 1
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A50:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edx, dword ptr [ebp + 8]
        sub    eax, eax
        mov    dword ptr [edx + 0xe7], 0
        mov    dword ptr [edx + 0xeb], 0
        mov    dword ptr [edx + 0xef], 0
        mov    dword ptr [edx + 0xf3], 0
        lea    edi, [edx + 0x179]
        mov    dword ptr [edx + 0xce], 0
        mov    ecx, 0x80
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [edx + 0x89], eax
        mov    dword ptr [edx + 0x8d], eax
        mov    byte ptr [edx + 0xcb], 1
        mov    byte ptr [edx + 0xcc], 1
        mov    byte ptr [edx + 0x48], al
        mov    byte ptr [edx + 0x4a], al
        mov    byte ptr [edx + 0xcd], al
        mov    word ptr [edx + 0xd2], 0xffff
        mov    word ptr [edx + 0xd4], 0xffff
        mov    byte ptr [edx + 0xda], 1
        pop    edi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_0AE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    edi, 0x32
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_1E90
        cmp    byte ptr [esi + 0x2e], 0
        jne    loc_1_0B30
loc_1_0B00:
        or     edi, edi
        je     loc_1_0B15
        push   0x186a0
        call   sub_1_0630
        dec    edi
        cmp    byte ptr [esi + 0x2e], 0
        je     loc_1_0B00
loc_1_0B15:
        cmp    byte ptr [esi + 0x2e], 0
        jne    loc_1_0B30
        mov    eax, 0xc0010006
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0B30:
        mov    eax, dword ptr [esi + 0xad]
        mov    al, byte ptr [eax + 0x1e02]
        mov    byte ptr [ebp - 3], al
        mov    eax, dword ptr [esi + 0xad]
        mov    cl, byte ptr [ebp - 3]
        and    ecx, 0xffff00ff
        mov    al, byte ptr [eax + 0x1e03]
        mov    byte ptr [ebp - 2], al
        shl    cx, 8
        and    ax, 0xff
        add    ax, cx
        mov    ecx, eax
        and    ecx, 0xffff
        add    ecx, dword ptr [esi + 0xb1]
        mov    word ptr [esi + 0xb5], ax
        mov    ax, word ptr [ecx + 6]
        mov    word ptr [ebp - 6], ax
        or     ax, ax
        je     loc_1_0BE1
        cmp    ax, 0x30
        jne    loc_1_0BB2
        push   7
        push   4
        push   0x32
        push   0xaa00000a
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x14
        mov    eax, 0xc0010006
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0BB2:
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        push   eax
        push   7
        push   4
        push   3
        push   0xaa000003
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x18
        mov    eax, 0xc0010006
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0BE1:
        mov    byte ptr [esi + 0x16e], 0
        mov    byte ptr [esi + 0x2e], 1
        mov    al, byte ptr [ecx + 1]
        mov    byte ptr [ebp - 6], al
        lea    edx, [esi + 0x37]
        test   byte ptr [ebp - 6], 1
        je     loc_1_0C00
        mov    byte ptr [edx], 1
        jmp    loc_1_0C03
loc_1_0C00:
        mov    byte ptr [edx], 0
loc_1_0C03:
        mov    edi, dword ptr [esi + 0x3f]
        cmp    edi, 0x2000
        jbe    loc_1_0D60
        mov    al, byte ptr [esi + 0xdb]
        mov    byte ptr [ebp - 1], al
        cmp    edi, 0x10000
        jne    loc_1_0C30
        sub    edi, 0x2830
        jmp    loc_1_0C36
        db  005h, 000h, 000h, 000h, 000h
loc_1_0C30:
        sub    edi, 0x630
loc_1_0C36:
        cmp    byte ptr [edx], 0
        je     loc_1_0C41
        mov    ax, word ptr [esi + 0x45]
        jmp    loc_1_0C4B
loc_1_0C41:
        lea    eax, [esi + 0xdd]
        mov    ax, word ptr [esi + 0x43]
loc_1_0C4B:
        mov    word ptr [esi + 0xdd], ax
        mov    ax, word ptr [esi + 0xdd]
        mov    edx, eax
        and    edx, 0xffff
        cmp    edx, edi
        jbe    loc_1_0C78
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0xe5]
        mov    eax, edi
        sub    edx, edx
        div    ebx
        shr    eax, 1
        jmp    loc_1_0C8E
loc_1_0C78:
        lea    edx, [esi + 0xe3]
        lea    edx, [esi + 0xe5]
        sub    dx, dx
        div    word ptr [esi + 0xe5]
loc_1_0C8E:
        inc    ax
        mov    word ptr [esi + 0xe3], ax
        xor    eax, eax
        xor    edx, edx
        mov    dx, word ptr [esi + 0xe5]
        mov    ax, word ptr [esi + 0xe3]
        imul   eax, edx
        sub    edi, eax
        xor    ebx, ebx
        mov    bl, byte ptr [ebp - 1]
        mov    eax, edi
        sub    edx, edx
        div    ebx
        xor    edx, edx
        mov    dx, word ptr [esi + 0xdd]
        cmp    edx, eax
        jbe    loc_1_0D09
        cmp    eax, 0x1000
        jae    loc_1_0CD8
        mov    word ptr [esi + 0xdd], 0x800
        jmp    loc_1_0D09
loc_1_0CD8:
        cmp    eax, 0x2000
        jae    loc_1_0CF0
        mov    word ptr [esi + 0xdd], 0x1000
        jmp    loc_1_0D09
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0CF0:
        cmp    eax, 0x4000
        mov    word ptr [esi + 0xdd], 0x2000
        jb     loc_1_0D09
        mov    word ptr [esi + 0xdd], 0x4000
loc_1_0D09:
        mov    eax, dword ptr [esi + 0xdf]
        xor    edx, edx
        mov    dx, word ptr [esi + 0xdd]
        cmp    edx, eax
        jbe    loc_1_0D23
        mov    word ptr [esi + 0xdd], ax
loc_1_0D23:
        xor    eax, eax
        mov    ax, word ptr [esi + 0xdd]
        sub    eax, 6
        mov    dword ptr [esi + 0xdf], eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0xdd]
        imul   eax, ebx
        sub    edi, eax
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0xe5]
        mov    eax, edi
        sub    edx, edx
        div    ebx
        add    word ptr [esi + 0xe3], ax
        jmp    loc_1_0DA7
        db  005h, 000h, 000h, 000h, 000h
loc_1_0D60:
        mov    word ptr [esi + 0xdd], 0x800
        mov    word ptr [esi + 0xdb], 1
        mov    eax, dword ptr [esi + 0xdf]
        xor    edx, edx
        mov    dx, word ptr [esi + 0xdd]
        cmp    edx, eax
        jbe    loc_1_0D8C
        mov    word ptr [esi + 0xdd], ax
loc_1_0D8C:
        xor    eax, eax
        mov    ax, word ptr [esi + 0xdd]
        sub    eax, 6
        mov    dword ptr [esi + 0xdf], eax
        mov    word ptr [esi + 0xe3], 0xf
loc_1_0DA7:
        mov    ax, word ptr [ecx + 8]
        mov    word ptr [ebp - 6], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 5]
        push   6
        mov    edi, eax
        and    edi, 0xffff
        add    edi, dword ptr [esi + 0xb1]
        push   edi
        lea    eax, [esi + 0x53]
        push   eax
        call   sub_1_071C
        lea    eax, [esi + 0x4d]
        cmp    byte ptr [eax], 0
        jne    loc_1_0DFD
        cmp    byte ptr [esi + 0x4e], 0
        jne    loc_1_0DFD
        cmp    byte ptr [esi + 0x4f], 0
        jne    loc_1_0DFD
        cmp    byte ptr [esi + 0x50], 0
        jne    loc_1_0DFD
        cmp    byte ptr [esi + 0x51], 0
        jne    loc_1_0DFD
        cmp    byte ptr [esi + 0x52], 0
        jne    loc_1_0DFD
        push   6
        push   edi
        push   eax
        call   sub_1_071C
loc_1_0DFD:
        mov    eax, dword ptr [esi + 0xad]
        pop    edi
        mov    byte ptr [eax + 0x1e0f], 0xff
        sub    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0E20:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        sub    edx, edx
        mov    esi, dword ptr [ecx + 0x61]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, -0x61
        je     loc_1_0E49
loc_1_0E35:
        mov    eax, dword ptr [esi + 0x18]
        cmp    eax, edx
        jbe    loc_1_0E3E
        mov    edx, eax
loc_1_0E3E:
        mov    esi, dword ptr [esi]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, -0x61
        jne    loc_1_0E35
loc_1_0E49:
        or     edx, edx
        jne    loc_1_0E52
        mov    edx, 0xf0
loc_1_0E52:
        mov    dword ptr [ecx + 0x164], edx
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0E60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        inc    dword ptr [edi + 0x5d]
        cmp    byte ptr [esi + 0x1d], 0
        jne    loc_1_1060
        inc    dword ptr [esi + 0x14]
        push   0
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 0x85]
        push   eax
        call   sub_2_01D0
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_0F27
        cmp    dword ptr [esi + 0x14], 2
        jne    loc_1_0EF3
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edi + 0x61]
        sub    eax, edi
        cmp    eax, 0x61
        jne    loc_1_0ED0
        mov    dword ptr [edi + 0x18], 0xfe
loc_1_0ED0:
        mov    eax, dword ptr [edi + 0x164]
        cmp    dword ptr [esi + 0x18], eax
        jne    loc_1_0EE1
        push   edi
        call   sub_1_0E20
loc_1_0EE1:
        push   0
        push   0x96
        push   esi
        call   sub_1_063C
        jmp    loc_1_1065
loc_1_0EF3:
        mov    byte ptr [esi + 0x1d], 1
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [edi + 0x69]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0x6d]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0x6d]
        mov    ebx, 0x103
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 0x6d], esi
        sub    dword ptr [esi + 0x14], 2
        jmp    loc_1_1065
loc_1_0F27:
        cmp    ebx, 0x103
        jne    loc_1_0FD0
        cmp    dword ptr [edi + 0x379], 1
        jne    loc_1_0F72
        sub    eax, eax
        sub    edx, edx
loc_1_0F40:
        mov    ecx, dword ptr [edi + 0xad]
        mov    cl, byte ptr [edx + ecx + 0x1f40]
        and    ecx, 0xf
        shl    eax, 4
        add    eax, ecx
        add    edx, 2
        cmp    edx, 0x10
        jb     loc_1_0F40
        cmp    eax, 0x5049434f
        mov    byte ptr [edi + 0x175], 1
        jne    loc_1_0F72
        mov    byte ptr [edi + 0x175], 0
loc_1_0F72:
        cmp    byte ptr [edi + 0x175], 0
        je     loc_1_0F85
        push   0
        push   edi
        call   sub_1_5220
        jmp    loc_1_0F9B
loc_1_0F85:
        cmp    byte ptr [edi + 0xcb], 0
        je     loc_1_0F9B
        mov    byte ptr [edi + 0xcb], 0
        push   edi
        call   sub_1_48C0
loc_1_0F9B:
        mov    byte ptr [esi + 0x1d], 1
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [edi + 0x69]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0x6d]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0x6d]
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 0x6d], esi
        sub    dword ptr [esi + 0x14], 2
        jmp    loc_1_1065
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0FD0:
        cmp    ebx, 0xc001000e
        jne    loc_1_1008
        mov    byte ptr [esi + 0x1d], 1
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ebx, 0x103
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [edi + 0x69]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0x6d]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0x6d]
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 0x6d], esi
        dec    dword ptr [esi + 0x14]
        jmp    loc_1_1065
loc_1_1008:
        cmp    ebx, 0xc001000d
        jne    loc_1_1040
        mov    byte ptr [esi + 0x1d], 1
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [edi + 0x71]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0x75]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0x75]
        mov    ebx, 0x103
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 0x75], esi
        dec    dword ptr [esi + 0x14]
        jmp    loc_1_1065
loc_1_1040:
        push   1
        push   0xa
        push   2
        push   0xaa000006
        mov    eax, dword ptr [edi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x14
        jmp    loc_1_1065
        db  08Dh, 064h, 024h, 000h
loc_1_1060:
        mov    ebx, 0xc0010002
loc_1_1065:
        mov    eax, dword ptr [edi + 0x5d]
        dec    eax
        mov    dword ptr [edi + 0x5d], eax
        jne    loc_1_1087
        cmp    byte ptr [edi + 0x15f], 0
        jne    loc_1_1081
        mov    eax, dword ptr [edi + 0x69]
        sub    eax, edi
        cmp    eax, 0x69
        je     loc_1_1087
loc_1_1081:
        push   edi
        call   sub_1_5080
loc_1_1087:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_10A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        sub    esi, esi
        mov    edi, dword ptr [ecx + 8]
        mov    edx, dword ptr [ebp + 0xc]
        inc    dword ptr [edi + 0x5d]
        mov    eax, dword ptr [edx + 0x10]
        cmp    eax, esi
        jne    loc_1_10C9
        push   edx
        push   ecx
        push   edi
        call   0x1590
        mov    esi, eax
        jmp    loc_1_1148
loc_1_10C9:
        cmp    eax, 1
        jne    loc_1_1143
        cmp    byte ptr [edi + 0x174], 0
        je     loc_1_10E0
        mov    esi, 0xc0010008
        jmp    loc_1_1148
        db  08Bh, 0FFh
loc_1_10E0:
        cmp    byte ptr [edi + 0x171], 0
        jne    loc_1_1107
        cmp    byte ptr [ecx + 0x1d], 0
        jne    loc_1_1100
        push   edx
        push   ecx
        push   edi
        call   sub_1_15E0
        mov    esi, eax
        jmp    loc_1_1148
        db  005h, 000h, 000h, 000h, 000h
loc_1_1100:
        mov    esi, 0xc0010002
        jmp    loc_1_1148
loc_1_1107:
        cmp    byte ptr [edi + 0x15f], 0
        je     loc_1_1117
        mov    esi, 0xc001000d
        jmp    loc_1_1148
loc_1_1117:
        cmp    byte ptr [edi + 0x16f], 0
        je     loc_1_1127
        mov    esi, 0xc0000001
        jmp    loc_1_1148
loc_1_1127:
        push   3
        push   0xb
        push   2
        push   0xaa000006
        mov    eax, dword ptr [edi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x14
        jmp    loc_1_1148
loc_1_1143:
        mov    esi, 0x10001
loc_1_1148:
        mov    eax, dword ptr [edi + 0x5d]
        dec    eax
        mov    dword ptr [edi + 0x5d], eax
        jne    loc_1_116A
        cmp    byte ptr [edi + 0x15f], 0
        jne    loc_1_1164
        mov    eax, dword ptr [edi + 0x69]
        sub    eax, edi
        cmp    eax, 0x69
        je     loc_1_116A
loc_1_1164:
        push   edi
        call   sub_1_5080
loc_1_116A:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1180:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x10], 1
        sub    edi, edi
        lea    ecx, [ebp - 8]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], edi
        mov    edx, dword ptr [ebp + 0x10]
        mov    eax, edx
        cmp    eax, 0x2010101
        ja     loc_1_11C2
        je     loc_1_1430
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_11E3
        shl    eax, 2
        jmp    loc_1_1540
loc_1_11C2:
        cmp    eax, 0x2010102
        je     loc_1_1467
        cmp    eax, 0x2010103
        je     loc_1_14A0
        cmp    eax, 0x2010104
        je     loc_1_14D0
loc_1_11E3:
        mov    edi, 0xc00000bb
        jmp    loc_1_1504
        db  08Dh, 049h, 000h
loc_1_11F0:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_1205
        mov    ecx, 0x5504                   ; -> loc_1_5504
        mov    esi, 0x5c
        jmp    loc_1_1504
loc_1_1205:
        mov    ecx, 0x5480                   ; -> loc_1_5480
        mov    esi, 0x84
        jmp    loc_1_1504
loc_1_1214:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x15f], 0
        je     loc_1_1230
        mov    dword ptr [ebp - 0xc], 2
        jmp    loc_1_1267
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1230:
        cmp    byte ptr [ebx + 0x16e], 0
        jne    loc_1_1260
        cmp    byte ptr [ebx + 0x170], 0
        jne    loc_1_1260
        cmp    byte ptr [ebx + 0x171], 0
        je     loc_1_1254
        mov    dword ptr [ebp - 0xc], 4
        jmp    loc_1_1267
loc_1_1254:
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_1_1267
        db  08Dh, 049h, 000h
loc_1_1260:
        mov    dword ptr [ebp - 0xc], 1
loc_1_1267:
        lea    ecx, [ebp - 0xc]
        mov    esi, 4
        jmp    loc_1_1504
loc_1_1274:
        lea    ecx, [ebp - 0x10]
        mov    esi, 4
        jmp    loc_1_1504
loc_1_1281:
        mov    dword ptr [ebp - 8], 0xf0
        jmp    loc_1_1504
        db  08Dh, 049h, 000h
loc_1_1290:
        mov    ebx, dword ptr [ebp + 8]
        cmp    edx, 0x10106
        mov    eax, dword ptr [ebx + 0xdf]
        mov    dword ptr [ebp - 8], eax
        jne    loc_1_1504
        sub    dword ptr [ebp - 8], 0xe
        jmp    loc_1_1504
loc_1_12B1:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x37], 1
        sbb    eax, eax
        and    eax, 0xfffe2b40
        add    eax, 0x27100
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1504
        db  08Dh, 064h, 024h, 000h
loc_1_12D0:
        mov    ebx, dword ptr [ebp + 8]
        xor    edx, edx
        xor    eax, eax
        mov    ax, word ptr [ebx + 0xdd]
        mov    dx, word ptr [ebx + 0xdb]
        imul   edx, eax
        jmp    loc_1_1501
        db  08Dh, 049h, 000h
loc_1_12F0:
        mov    ebx, dword ptr [ebp + 8]
        xor    edx, edx
        xor    eax, eax
        mov    ax, word ptr [ebx + 0xe5]
        mov    dx, word ptr [ebx + 0xe3]
        imul   edx, eax
        jmp    loc_1_1501
        db  08Dh, 049h, 000h
loc_1_1310:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0xdd]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1504
loc_1_1324:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0xe5]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1504
loc_1_1338:
        mov    ebx, dword ptr [ebp + 8]
        push   3
        lea    eax, [ebx + 0x53]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_071C
        and    dword ptr [ebp - 8], 0xffffff00
        cmp    byte ptr [ebx + 0x38], 0
        je     loc_1_135B
        or     dword ptr [ebp - 8], 1
loc_1_135B:
        lea    ecx, [ebp - 8]
        mov    esi, 4
        jmp    loc_1_1504
loc_1_1368:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x38], 0
        je     loc_1_1380
        mov    ecx, 0x55a0                   ; -> loc_1_55A0
        mov    esi, 0x2c
        jmp    loc_1_1504
loc_1_1380:
        mov    ecx, 0x55cc                   ; -> obj1:0x55CC
        mov    esi, 0x29
        jmp    loc_1_1504
        db  090h
loc_1_1390:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_13A7
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x95]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1504
loc_1_13A7:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1504
loc_1_13B8:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_13D0
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1504
        db  08Dh, 064h, 024h, 000h
loc_1_13D0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, ebx
        mov    edx, dword ptr [ebx + 0x61]
        mov    dword ptr [ebp - 8], 0
        sub    eax, edx
        cmp    eax, -0x61
        je     loc_1_1504
loc_1_13EA:
        mov    eax, dword ptr [edx + 0x18]
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_1400
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0xf0
        je     loc_1_1504
loc_1_1400:
        mov    edx, dword ptr [edx]
        mov    eax, ebx
        sub    eax, edx
        cmp    eax, -0x61
        jne    loc_1_13EA
        jmp    loc_1_1504
loc_1_1410:
        mov    word ptr [ebp - 2], 0x300
        lea    ecx, [ebp - 2]
        mov    esi, 2
        jmp    loc_1_1504
loc_1_1423:
        mov    dword ptr [ebp - 8], 6
        jmp    loc_1_1504
        db  090h
loc_1_1430:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 0x53]
        add    ebx, 0x53
        mov    byte ptr [ebp - 0x18], al
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 0x17], al
        mov    al, byte ptr [ebx + 2]
        mov    byte ptr [ebp - 0x16], al
        lea    ecx, [ebp - 0x18]
        mov    al, byte ptr [ebx + 3]
        mov    byte ptr [ebp - 0x15], al
        mov    al, byte ptr [ebx + 4]
        mov    byte ptr [ebp - 0x14], al
        mov    esi, 6
        mov    al, byte ptr [ebx + 5]
        mov    byte ptr [ebp - 0x13], al
        jmp    loc_1_1504
loc_1_1467:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 0x4d]
        add    ebx, 0x4d
        mov    byte ptr [ebp - 0x18], al
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 0x17], al
        mov    al, byte ptr [ebx + 2]
        mov    byte ptr [ebp - 0x16], al
        lea    ecx, [ebp - 0x18]
        mov    al, byte ptr [ebx + 3]
        mov    byte ptr [ebp - 0x15], al
        mov    al, byte ptr [ebx + 4]
        mov    byte ptr [ebp - 0x14], al
        mov    esi, 6
        mov    al, byte ptr [ebx + 5]
        mov    byte ptr [ebp - 0x13], al
        jmp    loc_1_1504
        db  005h, 000h, 000h, 000h, 000h
loc_1_14A0:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_14B1
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_1_14BA
loc_1_14B1:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0xa1]
loc_1_14BA:
        mov    dword ptr [ebp - 8], eax
        mov    edx, dword ptr [ebp - 8]
        and    edx, 0xff00
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_14E4
        db  005h, 000h, 000h, 000h, 000h
loc_1_14D0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0xa9]
        mov    dword ptr [ebp - 8], eax
        mov    edx, eax
        and    edx, 0xff00
loc_1_14E4:
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xff0000
        mov    ebx, dword ptr [ebp - 8]
        shr    ebx, 0x10
        or     eax, ebx
        shr    eax, 8
        or     edx, eax
loc_1_1501:
        mov    dword ptr [ebp - 8], edx
loc_1_1504:
        or     edi, edi
        jne    loc_1_1532
        cmp    dword ptr [ebp + 0x1c], esi
        jae    loc_1_1522
        mov    eax, dword ptr [ebp + 0x20]
        mov    edi, 0xc0010014
        mov    dword ptr [eax], esi
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_1_1522:
        push   esi
        push   ecx
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_071C
        mov    eax, dword ptr [ebp + 0x24]
        add    dword ptr [eax], esi
loc_1_1532:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 049h, 000h
loc_1_1540:
        lock adc dword ptr [eax], eax
        add    byte ptr [edx + edx], dl      ; -> loc_1_1214
        add    byte ptr [eax], al
        je     loc_1_155C
        add    byte ptr [eax], al
        je     loc_1_1560
        add    byte ptr [eax], al
        adc    dword ptr [edx], 0x12900000   ; -> loc_1_1290
        add    byte ptr [eax], al
        mov    cl, 0x12
        add    byte ptr [eax], al
loc_1_155C:
        rcl    byte ptr [edx]
        add    byte ptr [eax], al
loc_1_1560:
        lock adc al, byte ptr [eax]
        add    byte ptr [eax], dl            ; -> loc_1_1310
        adc    eax, dword ptr [eax]
        add    byte ptr [ebx + edx], ah      ; -> loc_1_1324
        add    byte ptr [eax], al
        cmp    byte ptr [ebx], dl
        add    byte ptr [eax], al
        push   0x90000013                    ; -> loc_1_1390
        adc    eax, dword ptr [eax]
        add    byte ptr [eax + 0x10000013], bh ; -> loc_1_13B8
        adc    al, 0
        add    byte ptr [eax - 0x1cffffee], dl ; -> loc_1_1290
        adc    dword ptr [eax], eax
        add    byte ptr [ebx], ah            ; -> loc_1_1423
        adc    al, 0
        add    byte ptr [ebp + 0x55002464], cl
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1180
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x24], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_1_15E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x10]
        sub    ebx, ebx
        mov    esi, dword ptr [edi + 0x1c]
        mov    eax, dword ptr [edi + 0x18]
        mov    ecx, dword ptr [edi + 0x14]
        cmp    esi, 4
        je     loc_1_1610
        mov    dword ptr [edi + 0x20], ebx
        mov    dword ptr [edi + 0x24], ebx
        mov    eax, 0xc0010014
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1610:
        cmp    ecx, 0x1010e
        je     loc_1_165C
        cmp    ecx, 0x1010f
        je     loc_1_1680
        cmp    ecx, 0x10112
        je     loc_1_16F0
        cmp    ecx, 0x2010103
        je     loc_1_16F4
        cmp    ecx, 0x2010104
        je     loc_1_1705
        mov    ebx, 0xc0010017
        mov    dword ptr [edi + 0x20], 0
        mov    dword ptr [edi + 0x24], 0
        jmp    loc_1_1716
loc_1_165C:
        push   4
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_071C
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1730
        jmp    loc_1_1714
        db  090h
loc_1_1680:
        push   4
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_071C
        add    dword ptr [ebp - 4], 0x20
        cmp    dword ptr [ebp - 4], 0xf0
        ja     loc_1_16E8
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x164]
        cmp    edx, dword ptr [ebp - 4]
        jae    loc_1_16C0
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x164], ecx
        jmp    loc_1_1716
        db  005h, 000h, 000h, 000h, 000h
loc_1_16C0:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x18], edx
        jne    loc_1_16E0
        mov    edx, dword ptr [ecx + 0x18]
        cmp    edx, dword ptr [ebp - 4]
        jbe    loc_1_16E0
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], edx
        push   eax
        call   sub_1_0E20
        jmp    loc_1_1716
        db  08Bh, 0FFh
loc_1_16E0:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_1_1716
loc_1_16E8:
        mov    ebx, 0xc0010014
        jmp    loc_1_1716
        db  090h
loc_1_16F0:
        sub    ebx, ebx
        jmp    loc_1_1716
loc_1_16F4:
        push   eax
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_17A0
        jmp    loc_1_1714
loc_1_1705:
        push   eax
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1800
loc_1_1714:
        mov    ebx, eax
loc_1_1716:
        or     ebx, ebx
        jne    loc_1_1724
        mov    dword ptr [edi + 0x20], esi
        mov    dword ptr [edi + 0x24], 0
loc_1_1724:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
sub_1_1730:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x14]
        test   eax, 0x8076
        je     loc_1_1750
        mov    eax, 0xc00000bb
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h
loc_1_1750:
        mov    esi, dword ptr [ebp + 0xc]
        push   1
        inc    dword ptr [esi + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0xc]
        mov    edi, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [edi + 0x85]
        push   eax
        call   sub_2_04F0
        mov    ebx, eax
        dec    dword ptr [esi + 0x14]
        cmp    ebx, 0x103
        jne    loc_1_1793
        cmp    byte ptr [edi + 0xcb], 0
        je     loc_1_1793
        mov    byte ptr [edi + 0xcb], 0
        push   edi
        call   sub_1_48C0
loc_1_1793:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h
sub_1_17A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        push   1
        inc    dword ptr [esi + 0x14]
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 0x85]
        push   eax
        call   sub_2_02C0
        mov    edi, eax
        dec    dword ptr [esi + 0x14]
        cmp    edi, 0x103
        jne    loc_1_17EE
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0xcb], 0
        je     loc_1_17EE
        mov    byte ptr [eax + 0xcb], 0
        push   eax
        call   sub_1_48C0
loc_1_17EE:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1800:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        push   1
        inc    dword ptr [esi + 0x14]
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 0x85]
        push   eax
        call   sub_2_0360
        mov    edi, eax
        dec    dword ptr [esi + 0x14]
        cmp    edi, 0x103
        jne    loc_1_184E
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0xcb], 0
        je     loc_1_184E
        mov    byte ptr [eax + 0xcb], 0
        push   eax
        call   sub_1_48C0
loc_1_184E:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1860:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0x14], eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        push   1
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_1_1180
        mov    edi, eax
        cmp    edi, 0xc00000bb
        jne    loc_1_19A0
        sub    edi, edi
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x2010105
        ja     loc_1_18F0
        je     loc_1_1940
        cmp    eax, 0x20101
        je     loc_1_1913
        cmp    eax, 0x20102
        je     loc_1_1920
        cmp    eax, 0x20103
        je     loc_1_1928
        cmp    eax, 0x20104
        je     loc_1_1930
        cmp    eax, 0x20105
        je     loc_1_1938
        mov    edi, 0xc0010017
        jmp    loc_1_1971
loc_1_18F0:
        cmp    eax, 0x2010106
        je     loc_1_1950
        cmp    eax, 0x2010107
        je     loc_1_1958
        cmp    eax, 0x2020101
        je     loc_1_1960
        cmp    eax, 0x2020102
        je     loc_1_1968
        mov    edi, 0xc0010017
        jmp    loc_1_1971
loc_1_1913:
        mov    eax, dword ptr [ebx + 0xf7]
        jmp    loc_1_196E
        db  005h, 000h, 000h, 000h, 000h
loc_1_1920:
        mov    eax, dword ptr [ebx + 0xfb]
        jmp    loc_1_196E
loc_1_1928:
        mov    eax, dword ptr [ebx + 0xff]
        jmp    loc_1_196E
loc_1_1930:
        mov    eax, dword ptr [ebx + 0x103]
        jmp    loc_1_196E
loc_1_1938:
        mov    eax, dword ptr [ebx + 0x11f]
        jmp    loc_1_196E
loc_1_1940:
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x172]
        or     eax, 0xc0011000
        jmp    loc_1_196E
loc_1_1950:
        mov    eax, dword ptr [ebx + 0x157]
        jmp    loc_1_196E
loc_1_1958:
        mov    eax, dword ptr [ebx + 0x15b]
        jmp    loc_1_196E
loc_1_1960:
        mov    eax, dword ptr [ebx + 0x107]
        jmp    loc_1_196E
loc_1_1968:
        mov    eax, dword ptr [ebx + 0x11b]
loc_1_196E:
        mov    dword ptr [ebp - 4], eax
loc_1_1971:
        or     edi, edi
        jne    loc_1_19A0
        cmp    dword ptr [ebp - 0x10], 4
        jb     loc_1_1990
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_071C
        add    dword ptr [ebp - 0xc], 4
        jmp    loc_1_19A0
loc_1_1990:
        mov    eax, 4
        sub    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 8], eax
        mov    edi, 0xc0010014
loc_1_19A0:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x20], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_19C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        mov    ecx, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x5d]
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, 0x20105
        ja     loc_1_1A00
        cmp    eax, 0x20101
        jae    loc_1_1A23
        cmp    eax, 0x10101
        jb     loc_1_1A1C
        cmp    eax, 0x10111
        jbe    loc_1_1A28
        cmp    eax, 0x10113
        je     loc_1_1A28
        mov    edi, 0xc0010017
        jmp    loc_1_1A28
        db  08Dh, 049h, 000h
loc_1_1A00:
        cmp    eax, 0x2010101
        jb     loc_1_1A1C
        cmp    eax, 0x2010107
        jbe    loc_1_1A28
        cmp    eax, 0x2020101
        jb     loc_1_1A1C
        cmp    eax, 0x2020102
        jbe    loc_1_1A23
loc_1_1A1C:
        mov    edi, 0xc0010017
        jmp    loc_1_1A28
loc_1_1A23:
        mov    edi, 0x103
loc_1_1A28:
        cmp    edi, 0x103
        jne    loc_1_1A77
        mov    dword ptr [ecx], 0
        mov    byte ptr [ecx + 4], 0
        cmp    dword ptr [esi + 0x89], 0
        jne    loc_1_1A51
        mov    dword ptr [esi + 0x8d], ecx
        mov    dword ptr [esi + 0x89], ecx
        jmp    loc_1_1A59
loc_1_1A51:
        mov    eax, dword ptr [esi + 0x8d]
        mov    dword ptr [eax], ecx
loc_1_1A59:
        cmp    byte ptr [esi + 0xcb], 0
        je     loc_1_1A6F
        mov    byte ptr [esi + 0xcb], 0
        push   esi
        call   sub_1_48C0
loc_1_1A6F:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_1A77:
        or     edi, edi
        jne    loc_1_1A84
        push   ecx
        push   esi
        call   sub_1_1860
        mov    edi, eax
loc_1_1A84:
        mov    eax, dword ptr [esi + 0x5d]
        dec    eax
        mov    dword ptr [esi + 0x5d], eax
        jne    loc_1_1AA6
        cmp    byte ptr [esi + 0x15f], 0
        jne    loc_1_1AA0
        mov    eax, dword ptr [esi + 0x69]
        sub    eax, esi
        cmp    eax, 0x69
        je     loc_1_1AA6
loc_1_1AA0:
        push   esi
        call   sub_1_5080
loc_1_1AA6:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_1AB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 0x103
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [edi + 0x5d]
        cmp    byte ptr [edi + 0x15f], 0
        je     loc_1_1AD4
        mov    esi, 0xc001000d
        jmp    loc_1_1B05
loc_1_1AD4:
        cmp    byte ptr [edi + 0x16f], 0
        je     loc_1_1AE4
        mov    esi, 0xc0000001
        jmp    loc_1_1B05
loc_1_1AE4:
        cmp    byte ptr [ebx + 0x1d], 0
        jne    loc_1_1B00
        inc    dword ptr [ebx + 0x14]
        push   ebx
        push   edi
        call   sub_1_1EF0
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_1B05
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1B00:
        mov    esi, 0xc0010002
loc_1_1B05:
        mov    eax, dword ptr [edi + 0x5d]
        dec    eax
        mov    dword ptr [edi + 0x5d], eax
        jne    loc_1_1B27
        cmp    byte ptr [edi + 0x15f], 0
        jne    loc_1_1B21
        mov    eax, dword ptr [edi + 0x69]
        sub    eax, edi
        cmp    eax, 0x69
        je     loc_1_1B27
loc_1_1B21:
        push   edi
        call   sub_1_5080
loc_1_1B27:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1B30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [edx + 8]
        or     esi, esi
        jne    loc_1_1B4B
        mov    dword ptr [edx + 0x2e], 4
        lea    esi, [edx + 0x1e]
loc_1_1B4B:
        cmp    byte ptr [ecx + 0x15f], 0
        je     loc_1_1B60
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_1_1B60:
        mov    eax, dword ptr [ebp + 0xc]
        test   al, 0x30
        je     loc_1_1B71
        mov    eax, 0xc0000001
        pop    esi
        pop    ebp
        ret    0x14
loc_1_1B71:
        mov    dword ptr [esi + 4], edx
        cmp    dword ptr [esi + 0x10], 4
        mov    dword ptr [esi + 8], eax
        je     loc_1_1B80
        mov    dword ptr [esi + 8], eax
loc_1_1B80:
        mov    dword ptr [esi], 0
        cmp    dword ptr [ecx + 0x89], 0
        jne    loc_1_1BA0
        mov    dword ptr [ecx + 0x8d], esi
        mov    dword ptr [ecx + 0x89], esi
        jmp    loc_1_1BA8
        db  08Dh, 049h, 000h
loc_1_1BA0:
        mov    eax, dword ptr [ecx + 0x8d]
        mov    dword ptr [eax], esi
loc_1_1BA8:
        inc    dword ptr [edx + 0x14]
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1BC0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [edx + 8]
        or     esi, esi
        jne    loc_1_1BDB
        mov    dword ptr [edx + 0x56], 9
        lea    esi, [edx + 0x46]
loc_1_1BDB:
        cmp    byte ptr [ecx + 0x15f], 0
        je     loc_1_1BF0
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_1_1BF0:
        mov    dword ptr [esi + 4], edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi], 0
        cmp    dword ptr [ecx + 0x89], 0
        jne    loc_1_1C16
        mov    dword ptr [ecx + 0x8d], esi
        mov    dword ptr [ecx + 0x89], esi
        jmp    loc_1_1C1E
loc_1_1C16:
        mov    eax, dword ptr [ecx + 0x8d]
        mov    dword ptr [eax], esi
loc_1_1C1E:
        inc    dword ptr [edx + 0x14]
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h
loc_1_1C30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [edx + 8]
        or     esi, esi
        jne    loc_1_1C4B
        mov    dword ptr [edx + 0x7e], 0xa
        lea    esi, [edx + 0x6e]
loc_1_1C4B:
        cmp    byte ptr [ecx + 0x15f], 0
        je     loc_1_1C60
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_1_1C60:
        mov    dword ptr [esi + 4], edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi], 0
        cmp    dword ptr [ecx + 0x89], 0
        jne    loc_1_1C86
        mov    dword ptr [ecx + 0x8d], esi
        mov    dword ptr [ecx + 0x89], esi
        jmp    loc_1_1C8E
loc_1_1C86:
        mov    eax, dword ptr [ecx + 0x8d]
        mov    dword ptr [eax], esi
loc_1_1C8E:
        inc    dword ptr [edx + 0x14]
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h
loc_1_1CA0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 0x14]
        ret    4
        db  08Dh, 049h, 000h
sub_1_1CB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   0x3350                        ; -> loc_1_3350
        push   esi
        call   sub_1_062A
        mov    eax, dword ptr [esi + 0x59]
        inc    ax
        mov    dx, ax
        sub    al, al
        out    dx, al
        push   0xc350
        call   sub_1_0630
        mov    ecx, dword ptr [esi + 0x59]
        add    cx, 2
        mov    dx, cx
        sub    al, al
        out    dx, al
        cmp    byte ptr [esi + 0x3a], al
        je     loc_1_1CFA
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e18], 0xc0
loc_1_1CFA:
        cmp    dword ptr [esi + 0x379], 1
        jne    loc_1_1D62
        mov    ecx, 0x30
        cmp    dword ptr [esi + 0x37d], 0x10
        jne    loc_1_1D16
        mov    ecx, 0x32
loc_1_1D16:
        mov    eax, dword ptr [esi + 0x385]
        cmp    eax, 0x4000
        je     loc_1_1D33
        cmp    eax, 0x8000
        je     loc_1_1D38
        cmp    eax, 0x10000
        je     loc_1_1D40
        jmp    loc_1_1D43
loc_1_1D33:
        or     ecx, 4
        jmp    loc_1_1D43
loc_1_1D38:
        or     ecx, 8
        jmp    loc_1_1D43
        db  08Dh, 049h, 000h
loc_1_1D40:
        or     ecx, 0xc
loc_1_1D43:
        mov    edx, dword ptr [esi + 0x59]
        cmp    edx, 0xa20
        je     loc_1_1D51
        or     ecx, 1
loc_1_1D51:
        mov    al, 0xc
        out    dx, al
        mov    al, 0x12
        out    dx, al
        mov    al, 0x26
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    al, 0x40
        out    dx, al
        jmp    loc_1_1D68
loc_1_1D62:
        cmp    byte ptr [esi + 0x38], 0
        je     loc_1_1D77
loc_1_1D68:
        mov    eax, dword ptr [esi + 0xad]
        mov    cl, byte ptr [esi + 0x47]
        mov    byte ptr [eax + 0x1e00], cl
loc_1_1D77:
        cmp    dword ptr [esi + 0x379], 1
        je     loc_1_1D90
        push   esi
        push   0x3330                        ; -> loc_1_3330
        push   esi
        call   sub_1_062A
        jmp    loc_1_1DC1
        db  08Bh, 0FFh
loc_1_1D90:
        mov    eax, dword ptr [esi + 0xad]
        mov    al, byte ptr [eax + 0x1e08]
        mov    cl, al
        mov    eax, dword ptr [esi + 0xad]
        and    cl, 0x3f
        mov    byte ptr [eax + 0x1e08], cl
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [esi + 0x168], 1
        mov    byte ptr [eax + 0x1e08], 0xc0
loc_1_1DC1:
        mov    eax, dword ptr [esi + 0xad]
        push   0xc001000c
        mov    byte ptr [eax + 0x1e08], 0xc0
        push   esi
        call   sub_1_52E0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_1DE0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0A50
        cmp    byte ptr [esi + 0x176], 0
        je     loc_1_1E0D
        mov    byte ptr [esi + 0x176], 0
        mov    byte ptr [esi + 0x174], 0
        mov    byte ptr [esi + 0x177], 0
loc_1_1E0D:
        mov    byte ptr [esi + 0x15f], 0
        mov    byte ptr [esi + 0x175], 0
        mov    byte ptr [esi + 0x168], 0
        mov    byte ptr [esi + 0x169], 0
        mov    byte ptr [esi + 0x171], 0
        push   esi
        call   sub_1_4930
        mov    ebx, dword ptr [esi + 0x16a]
        or     ebx, ebx
        je     loc_1_1E80
        mov    edi, dword ptr [esi + 0x61]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x61
        je     loc_1_1E72
loc_1_1E4C:
        push   0
        push   0
        push   0x40010005
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_05A4
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x61
        jne    loc_1_1E4C
loc_1_1E72:
        dec    dword ptr [ebx + 0x14]
        push   0
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_0586
loc_1_1E80:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1E90:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0xad]
        mov    byte ptr [eax + 0x1e08], 0xc0
        mov    eax, dword ptr [ecx + 0xad]
        mov    byte ptr [eax + 0x1e0f], 1
        mov    eax, dword ptr [ecx + 0xad]
        mov    byte ptr [eax + 0x1e0c], 0
        cmp    dword ptr [ecx + 0x379], 1
        jne    loc_1_1ED7
        mov    eax, dword ptr [ecx + 0xad]
        pop    ebp
        mov    byte ptr [eax + 0x1e00], 0xd0
        ret    4
loc_1_1ED7:
        cmp    byte ptr [ecx + 0x38], 0
        je     loc_1_1EEC
        mov    eax, dword ptr [ecx + 0xad]
        mov    dl, byte ptr [ecx + 0x47]
        mov    byte ptr [eax + 0x1e00], dl
loc_1_1EEC:
        pop    ebp
        ret    4
sub_1_1EF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        or     edi, edi
        mov    esi, dword ptr [ebp + 8]
        je     loc_1_1F29
        mov    ebx, dword ptr [esi + 0x61]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -0x61
        je     loc_1_1F29
loc_1_1F0C:
        push   0
        push   0
        push   0x40010004
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_059E
        mov    ebx, dword ptr [ebx]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -0x61
        jne    loc_1_1F0C
loc_1_1F29:
        mov    byte ptr [esi + 0x15f], 1
        mov    byte ptr [esi + 0x171], 1
        mov    dword ptr [esi + 0x16a], edi
        mov    dword ptr [esi + 0x160], 0
        or     edi, edi
        je     loc_1_1F4E
        inc    dword ptr [edi + 0x14]
loc_1_1F4E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1F60:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xa0
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x60], 0xc
        mov    word ptr [ebp - 0x5e], 0xc
        mov    dword ptr [ebp - 0x5c], 0x5604 ; -> obj1:0x5604
        mov    word ptr [ebp - 0x58], 8
        mov    word ptr [ebp - 0x56], 8
        mov    dword ptr [ebp - 0x54], 0x5618 ; -> obj1:0x5618
        mov    word ptr [ebp - 0x38], 7
        mov    word ptr [ebp - 0x36], 7
        mov    dword ptr [ebp - 0x34], 0x5628 ; -> loc_1_5628
        mov    word ptr [ebp - 0x40], 0xe
        mov    word ptr [ebp - 0x3e], 0xe
        mov    dword ptr [ebp - 0x3c], 0x5658 ; -> obj1:0x5658
        mov    word ptr [ebp - 0x50], 0xa
        mov    bl, 1
        mov    word ptr [ebp - 0x4e], 0xa
        mov    dword ptr [ebp - 0x4c], 0x5684 ; -> obj1:0x5684
        mov    word ptr [ebp - 0x48], 0xa
        mov    word ptr [ebp - 0x46], 0xa
        mov    dword ptr [ebp - 0x44], 0x56b4 ; -> obj1:0x56B4
        mov    dword ptr [ebp - 0xc], 0
        mov    byte ptr [ebp - 1], 0
        mov    byte ptr [ebp - 2], 0
        mov    eax, dword ptr [0x56c8]       ; -> obj1:0x56C8
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   0x38d
        push   eax
        call   sub_1_0636
        or     eax, eax
        je     loc_1_2017
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_2017:
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 0xe3
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0xdf], 0x4628
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x15b], 2
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x175], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x7d], eax
        lea    eax, [ebp - 0x14]
        push   esi
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_06B4
        cmp    dword ptr [ebp - 0xc], 0
        push   0
        je     loc_1_2089
        push   0x38d
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_063C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_2089:
        lea    eax, [ebp - 0x60]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_20B3
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x379], eax
loc_1_20B3:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x379], 1
        je     loc_1_20E9
        push   1
        lea    eax, [ebp - 0x58]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_20E9
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 4], 3
        jne    loc_1_20E9
        mov    byte ptr [ebp - 2], 1
loc_1_20E9:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_2122
        lea    eax, [ebp - 0x30]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        push   esi
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_05B0
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_2110
        mov    byte ptr [ebp - 1], 1
        jmp    loc_1_233C
loc_1_2110:
        mov    al, byte ptr [ebp - 0x2d]
        and    eax, 1
        je     loc_1_2188
        cmp    eax, 1
        je     loc_1_2194
        jmp    loc_1_219E
loc_1_2122:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x379], 1
        jne    loc_1_2159
        push   1
        lea    eax, [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_219E
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x59], eax
        jmp    loc_1_219E
loc_1_2159:
        push   0
        lea    eax, [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_2184
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 4]
        dec    eax
        cmp    eax, 1
        sbb    bl, bl
        neg    bl
loc_1_2184:
        or     bl, bl
        je     loc_1_2194
loc_1_2188:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x59], 0xa20
        jmp    loc_1_219E
loc_1_2194:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x59], 0xa24
loc_1_219E:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x379], 1
        jne    loc_1_22C2
        mov    word ptr [ebp - 0xa0], 0
        lea    edi, [ebp - 0x9e]
        sub    eax, eax
        mov    ecx, 7
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        push   0x20
        lea    eax, [ebp - 0xa0]
        push   eax
        push   1
        push   esi
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_0604
        add    esp, 0x1c
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_21F7
        mov    byte ptr [ebp - 1], 1
        jmp    loc_1_233C
loc_1_21F7:
        mov    eax, dword ptr [ebp - 0x98]
        sub    eax, dword ptr [ebp - 0x9c]
        inc    eax
        cmp    eax, 0x2000
        je     loc_1_2214
        mov    byte ptr [ebp - 1], 1
        jmp    loc_1_233C
loc_1_2214:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0x9c]
        mov    dword ptr [ecx + 0x389], eax
        mov    ecx, dword ptr [ebp - 0x8c]
        mov    edx, dword ptr [ebp - 0x88]
        sub    edx, ecx
        inc    edx
        cmp    edx, 0x2000
        je     loc_1_225B
        cmp    edx, 0x4000
        je     loc_1_225B
        cmp    edx, 0x8000
        je     loc_1_225B
        cmp    edx, 0x10000
        je     loc_1_225B
        mov    byte ptr [ebp - 1], 1
        jmp    loc_1_233C
loc_1_225B:
        mov    eax, dword ptr [ebp - 8]
        push   0
        mov    dword ptr [eax + 0x385], edx
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x381], ecx
        lea    eax, [ebp - 0x50]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_2299
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x37d], eax
loc_1_2299:
        push   0
        lea    eax, [ebp - 0x48]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_22C2
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x33], eax
loc_1_22C2:
        push   0
        lea    eax, [ebp - 0x40]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_22EE
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0xdf], eax
loc_1_22EE:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_05BC
        cmp    dword ptr [ebp - 0x1c], 6
        jne    loc_1_233C
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_233C
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    edx, ecx
        add    eax, 0x4d
        mov    dl, byte ptr [edx]
        mov    byte ptr [eax], dl
        mov    dl, byte ptr [ecx + 1]
        mov    byte ptr [eax + 1], dl
        mov    dl, byte ptr [ecx + 2]
        mov    byte ptr [eax + 2], dl
        mov    dl, byte ptr [ecx + 3]
        mov    byte ptr [eax + 3], dl
        mov    dl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 4], dl
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [eax + 5], cl
loc_1_233C:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_06C0
        mov    al, byte ptr [ebp - 1]
        push   eax
        mov    al, byte ptr [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_07D0
        mov    dword ptr [ebp - 0xc], eax
        or     eax, eax
        je     loc_1_2372
        push   0
        push   0x38d
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_063C
loc_1_2372:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_1_2380:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0714
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0x20]
        push   eax
        call   sub_1_0618
        mov    ecx, dword ptr [esi + 0x59]
        inc    cx
        mov    dx, cx
        sub    al, al
        out    dx, al
        push   0x2625a0
        call   sub_1_0630
        push   esi
        call   sub_1_0624
        mov    eax, dword ptr [esi + 0x85]
        push   eax
        call   sub_2_00D0
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0568
        push   0
        push   0x38d
        push   esi
        call   sub_1_063C
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_23F0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x171], 1
        cmp    byte ptr [eax + 0xcb], 0
        je     loc_1_243A
        mov    ecx, dword ptr [eax + 0xb7]
        or     ecx, ecx
        je     loc_1_243A
        mov    edx, dword ptr [eax + 0xad]
        pop    ebp
        mov    byte ptr [edx + 0x1e08], 0x80
        mov    word ptr [ecx], 0
        mov    byte ptr [ecx + 2], 0
        mov    byte ptr [ecx], 4
        mov    eax, dword ptr [eax + 0xad]
        mov    byte ptr [eax + 0x1e4b], 0x20
        ret    4
loc_1_243A:
        mov    edx, dword ptr [eax + 0x59]
        inc    dx
        sub    al, al
        out    dx, al
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2450:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_055C
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0550
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2480:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x38
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x379], 1
        jne    loc_1_2639
        mov    ecx, 0x30
        cmp    dword ptr [esi + 0x37d], 0x10
        jne    loc_1_24AC
        mov    ecx, 0x32
loc_1_24AC:
        mov    eax, dword ptr [esi + 0x385]
        cmp    eax, 0x4000
        je     loc_1_24C9
        cmp    eax, 0x8000
        je     loc_1_24D0
        cmp    eax, 0x10000
        je     loc_1_24D5
        jmp    loc_1_24D8
loc_1_24C9:
        or     ecx, 4
        jmp    loc_1_24D8
        db  08Bh, 0FFh
loc_1_24D0:
        or     ecx, 8
        jmp    loc_1_24D8
loc_1_24D5:
        or     ecx, 0xc
loc_1_24D8:
        mov    edx, dword ptr [esi + 0x59]
        cmp    edx, 0xa20
        je     loc_1_24E6
        or     ecx, 1
loc_1_24E6:
        mov    al, 0xc
        out    dx, al
        mov    al, 0x12
        out    dx, al
        mov    al, 0x26
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    al, 0x40
        out    dx, al
        push   0x2000
        mov    eax, dword ptr [esi + 0x389]
        push   eax
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        lea    eax, [esi + 0xad]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_0702
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_2540
        push   0
        push   0xaa000001
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0xc
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_2540:
        sub    eax, eax
        lea    ecx, [ebp - 0x38]
loc_1_2545:
        mov    dword ptr [ecx], 0
        sub    edi, edi
        lea    ecx, [ecx]
loc_1_2550:
        mov    edx, dword ptr [esi + 0xad]
        add    edx, edi
        mov    dl, byte ptr [eax + edx + 0x1f30]
        mov    byte ptr [ebp - 2], dl
        and    edx, 0xf
        mov    ebx, dword ptr [ecx]
        shl    ebx, 4
        add    edx, ebx
        mov    dword ptr [ecx], edx
        add    edi, 2
        cmp    edi, 0x10
        jb     loc_1_2550
        add    eax, 0x10
        add    ecx, 4
        lea    edx, [ebp - 0x2c]
        cmp    ecx, edx
        jb     loc_1_2545
        mov    eax, dword ptr [0x5560]       ; -> loc_1_5560
        cmp    dword ptr [ebp - 0x38], eax
        jne    loc_1_2618
        mov    eax, dword ptr [0x5564]       ; -> loc_1_5564
        cmp    dword ptr [ebp - 0x34], eax
        jne    loc_1_2618
        mov    eax, dword ptr [0x5568]       ; -> loc_1_5568
        cmp    dword ptr [ebp - 0x30], eax
        jne    loc_1_2618
        mov    byte ptr [esi + 0x38], 1
        mov    eax, dword ptr [esi + 0x385]
        push   eax
        mov    eax, dword ptr [esi + 0x381]
        push   eax
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        lea    eax, [esi + 0xb1]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_0702
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_25F5
        push   0
        push   0xaa000001
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0xc
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_25F5:
        mov    byte ptr [esi + 0x3a], 0
        mov    eax, dword ptr [esi + 0x385]
        mov    dword ptr [esi + 0x3f], eax
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [esi + 0x47], 0xd0
        mov    byte ptr [eax + 0x1e00], 0xd0
        jmp    loc_1_29C3
loc_1_2618:
        push   0
        push   0xaa00000d
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0xc
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2639:
        mov    dx, word ptr [esi + 0x59]
        in     al, dx
        mov    byte ptr [ebp - 5], al
        mov    edx, dword ptr [esi + 0x59]
        add    dx, 2
        in     al, dx
        mov    byte ptr [ebp - 6], al
        mov    cl, al
        and    ecx, 1
        shl    ecx, 0x13
        mov    al, byte ptr [ebp - 5]
        and    eax, 0xfc
        shl    eax, 0xb
        or     ecx, eax
        lea    eax, [esi + 0xad]
        push   0x2000
        push   ecx
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        lea    eax, [esi + 0xad]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_0702
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_26AB
        push   0
        push   0xaa000001
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0xc
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_26AB:
        sub    ecx, ecx
        lea    eax, [ebp - 0x2c]
loc_1_26B0:
        mov    dword ptr [eax], 0
        sub    edi, edi
loc_1_26B8:
        mov    edx, dword ptr [esi + 0xad]
        add    edx, edi
        mov    dl, byte ptr [ecx + edx + 0x1f30]
        mov    byte ptr [ebp - 1], dl
        and    edx, 0xf
        mov    ebx, dword ptr [eax]
        shl    ebx, 4
        add    edx, ebx
        mov    dword ptr [eax], edx
        add    edi, 2
        cmp    edi, 0x10
        jb     loc_1_26B8
        add    ecx, 0x10
        add    eax, 4
        lea    edx, [ebp - 0x20]
        cmp    eax, edx
        jb     loc_1_26B0
        mov    eax, dword ptr [ebp - 0x2c]
        cmp    dword ptr [0x5578], eax       ; -> loc_1_5578
        jne    loc_1_2712
        mov    eax, dword ptr [ebp - 0x28]
        cmp    dword ptr [0x557c], eax       ; -> loc_1_557C
        jne    loc_1_2712
        mov    eax, dword ptr [ebp - 0x24]
        cmp    dword ptr [0x5580], eax       ; -> loc_1_5580
        jne    loc_1_2712
        mov    byte ptr [esi + 0x38], 1
        jmp    loc_1_2746
loc_1_2712:
        mov    eax, dword ptr [ebp - 0x2c]
        cmp    dword ptr [0x5584], eax       ; -> loc_1_5584
        jne    loc_1_2A84
        mov    eax, dword ptr [ebp - 0x28]
        cmp    dword ptr [0x5588], eax       ; -> loc_1_5588
        jne    loc_1_2A84
        mov    eax, dword ptr [ebp - 0x24]
        cmp    dword ptr [0x558c], eax       ; -> loc_1_558C
        jne    loc_1_2A84
        lea    eax, [esi + 0x38]
        mov    byte ptr [esi + 0x38], 0
loc_1_2746:
        mov    eax, dword ptr [esi + 0xad]
        mov    al, byte ptr [eax + 0x1e01]
        mov    byte ptr [ebp - 4], al
        and    al, 0xc
        shr    al, 2
        mov    byte ptr [ebp - 4], al
        cmp    byte ptr [esi + 0x38], 0
        je     loc_1_2840
        mov    dl, byte ptr [ebp - 5]
        and    dl, 0xfd
        shr    dl, 1
        or     dl, 0x80
        add    dl, 2
        mov    byte ptr [esi + 0x47], dl
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        jl     loc_1_279C
        cmp    eax, 1
        jle    loc_1_280A
        cmp    eax, 2
        je     loc_1_2820
        cmp    eax, 3
        je     loc_1_2830
loc_1_279C:
        mov    edi, dword ptr [ebp - 0x18]
        mov    al, byte ptr [ebp - 7]
loc_1_27A2:
        xor    ecx, ecx
        mov    cl, al
        xor    ebx, ebx
        mov    bl, dl
        dec    ecx
        test   ebx, ecx
        je     loc_1_27BB
        lea    ecx, [eax - 1]
        not    cl
        and    cl, dl
        add    cl, al
        mov    byte ptr [esi + 0x47], cl
loc_1_27BB:
        mov    dword ptr [esi + 0x3f], edi
        xor    eax, eax
        mov    al, byte ptr [esi + 0x47]
        shl    eax, 0xc
        push   edi
        push   eax
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        lea    eax, [esi + 0xb1]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_0702
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_28CA
        push   0
        push   0xaa000001
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0xc
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_280A:
        cmp    byte ptr [ebp - 4], 1
        sbb    edi, edi
        and    edi, 0xffffe000
        add    edi, 0x4000
        mov    al, 4
        jmp    loc_1_27A2
loc_1_2820:
        mov    edi, 0x8000
        mov    al, 8
        jmp    loc_1_27A2
        db  08Dh, 064h, 024h, 000h
loc_1_2830:
        mov    edi, 0x10000
        mov    al, 0x10
        jmp    loc_1_27A2
        db  08Dh, 064h, 024h, 000h
loc_1_2840:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        je     loc_1_28A6
        cmp    eax, 1
        je     loc_1_28B0
        cmp    eax, 2
        je     loc_1_28B7
        cmp    eax, 3
        je     loc_1_28C0
        mov    edi, dword ptr [ebp - 0x1c]
loc_1_285B:
        mov    al, byte ptr [ebp - 6]
        and    eax, 0xfe
        shl    eax, 0xc
        push   edi
        push   eax
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        lea    eax, [esi + 0xb1]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_0702
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_28C7
        push   0
        push   0xaa000001
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0xc
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_28A6:
        mov    edi, 0x2000
        jmp    loc_1_285B
        db  08Dh, 049h, 000h
loc_1_28B0:
        mov    edi, 0x4000
        jmp    loc_1_285B
loc_1_28B7:
        mov    edi, 0x8000
        jmp    loc_1_285B
        db  08Bh, 0FFh
loc_1_28C0:
        mov    edi, 0x10000
        jmp    loc_1_285B
loc_1_28C7:
        mov    dword ptr [esi + 0x3f], edi
loc_1_28CA:
        mov    al, byte ptr [ebp - 5]
        and    eax, 3
        je     loc_1_2912
        cmp    eax, 1
        je     loc_1_2919
        cmp    eax, 2
        je     loc_1_2920
        cmp    eax, 3
        je     loc_1_2930
        mov    eax, dword ptr [ebp - 0x20]
loc_1_28E4:
        mov    dword ptr [esi + 0x33], eax
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [esi + 0x39], 0
        mov    al, byte ptr [eax + 0x1fa6]
        mov    byte ptr [ebp - 3], al
        xor    eax, eax
        and    byte ptr [ebp - 3], 0xf
        mov    al, byte ptr [ebp - 3]
        lea    ecx, [eax - 0xa]
        cmp    ecx, 5
        ja     loc_1_2940
        jmp    dword ptr [ecx*4 + 0x2aa8]    ; -> obj1:0x2AA8
loc_1_2912:
        mov    eax, 2
        jmp    loc_1_28E4
loc_1_2919:
        mov    eax, 3
        jmp    loc_1_28E4
loc_1_2920:
        cmp    byte ptr [esi + 0x38], 1
        sbb    eax, eax
        and    eax, 4
        add    eax, 6
        jmp    loc_1_28E4
        db  08Bh, 0FFh
loc_1_2930:
        cmp    byte ptr [esi + 0x38], 1
        sbb    eax, eax
        and    eax, 4
        add    eax, 7
        jmp    loc_1_28E4
        db  08Bh, 0FFh
loc_1_2940:
        push   eax
        push   6
        push   2
        push   3
        push   0xaa00000a
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x18
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2966:
        mov    dword ptr [esi + 0x3b], 0x10000
        jmp    loc_1_29B3
        db  090h
loc_1_2970:
        mov    dword ptr [esi + 0x3b], 0x10000
        mov    byte ptr [esi + 0x39], 1
        jmp    loc_1_29B3
        db  08Dh, 049h, 000h
loc_1_2980:
        mov    dword ptr [esi + 0x3b], 0x8000
        jmp    loc_1_29B3
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2990:
        mov    dword ptr [esi + 0x3b], 0x4000
        jmp    loc_1_29B3
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_29A0:
        mov    dword ptr [esi + 0x3b], 0x2000
        jmp    loc_1_29B3
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_29B0:
        mov    dword ptr [esi + 0x3b], edi
loc_1_29B3:
        mov    eax, dword ptr [esi + 0xad]
        mov    al, byte ptr [eax + 0x1fa8]
        mov    byte ptr [esi + 0x3a], 0
loc_1_29C3:
        mov    eax, dword ptr [esi + 0xad]
        mov    al, byte ptr [eax + 0x1faa]
        mov    byte ptr [ebp - 3], al
        and    byte ptr [ebp - 3], 0xf
        xor    eax, eax
        mov    al, byte ptr [ebp - 3]
        cmp    eax, 0xd
        je     loc_1_29F0
        cmp    eax, 0xe
        je     loc_1_29F8
        mov    word ptr [esi + 0x43], 0x800
        jmp    loc_1_29FE
        db  08Dh, 049h, 000h
loc_1_29F0:
        mov    word ptr [esi + 0x43], 0x1170
        jmp    loc_1_29FE
loc_1_29F8:
        mov    word ptr [esi + 0x43], 0x1000
loc_1_29FE:
        mov    eax, dword ptr [esi + 0xad]
        mov    al, byte ptr [eax + 0x1fac]
        mov    byte ptr [ebp - 3], al
        and    byte ptr [ebp - 3], 0xf
        xor    eax, eax
        mov    al, byte ptr [ebp - 3]
        cmp    eax, 0xb
        je     loc_1_2A40
        cmp    eax, 0xc
        je     loc_1_2A51
        cmp    eax, 0xd
        je     loc_1_2A62
        cmp    eax, 0xe
        je     loc_1_2A73
        mov    word ptr [esi + 0x45], 0x800
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_2A40:
        mov    word ptr [esi + 0x45], 0x4628
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2A51:
        mov    word ptr [esi + 0x45], 0x4000
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2A62:
        mov    word ptr [esi + 0x45], 0x2000
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2A73:
        mov    word ptr [esi + 0x45], 0x1000
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2A84:
        push   0
        push   0xaa00000d
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0xc
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_2966
        dd  loc_1_2970
        dd  loc_1_2980
        dd  loc_1_2990
        dd  loc_1_29A0
        dd  loc_1_29B0
loc_1_2AC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0x103
        mov    esi, dword ptr [edi + 8]
        push   eax
        push   0
        push   0
        push   0
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        call   sub_1_0660
        cmp    dword ptr [ebp - 8], 0xe
        jb     loc_1_2BFA
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 0xdf], eax
        jb     loc_1_2BFA
        inc    dword ptr [esi + 0x5d]
        cmp    byte ptr [esi + 0x174], 0
        je     loc_1_2B20
        mov    dword ptr [ebp - 4], 0xc0010008
        jmp    loc_1_2BCC
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2B20:
        cmp    byte ptr [esi + 0x171], 0
        jne    loc_1_2B87
        lea    eax, [ebx + 0x24]
        cmp    byte ptr [edi + 0x1d], 0
        je     loc_1_2B40
        mov    dword ptr [ebp - 4], 0xc0010002
        jmp    loc_1_2BCC
        db  08Bh, 0FFh
loc_1_2B40:
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax + 8], ebx
        cmp    dword ptr [esi + 0xe7], 0
        jne    loc_1_2B57
        mov    dword ptr [esi + 0xe7], ebx
        jmp    loc_1_2B60
loc_1_2B57:
        mov    ecx, dword ptr [esi + 0xeb]
        mov    dword ptr [ecx + 0x24], ebx
loc_1_2B60:
        mov    dword ptr [esi + 0xeb], ebx
        mov    dword ptr [eax], 0
        inc    dword ptr [edi + 0x14]
        cmp    byte ptr [esi + 0xcb], 0
        je     loc_1_2BCC
        mov    byte ptr [esi + 0xcb], 0
        push   esi
        call   sub_1_48C0
        jmp    loc_1_2BCC
loc_1_2B87:
        cmp    byte ptr [esi + 0x15f], 0
        je     loc_1_2BA0
        mov    dword ptr [ebp - 4], 0xc001000d
        jmp    loc_1_2BCC
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2BA0:
        cmp    byte ptr [esi + 0x16f], 0
        je     loc_1_2BB2
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_2BCC
loc_1_2BB2:
        push   2
        push   0xb
        push   2
        push   0xaa000006
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x14
loc_1_2BCC:
        mov    eax, dword ptr [esi + 0x5d]
        dec    eax
        mov    dword ptr [esi + 0x5d], eax
        jne    loc_1_2BEE
        cmp    byte ptr [esi + 0x15f], 0
        jne    loc_1_2BE8
        mov    eax, dword ptr [esi + 0x69]
        sub    eax, esi
        cmp    eax, 0x69
        je     loc_1_2BEE
loc_1_2BE8:
        push   esi
        call   sub_1_5080
loc_1_2BEE:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2BFA:
        mov    eax, 0xc001000f
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_2C10:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x5d]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x171]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_2EB9
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_2EAD
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xd6]
        mov    word ptr [ebp - 0x10], ax
        mov    dword ptr [ebp - 0x34], 0
        mov    eax, dword ptr [ebp - 0xc]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], 0
        jmp    loc_1_2C80
loc_1_2C80:
        jmp    loc_1_2C85
loc_1_2C85:
        push   0
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x2c]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_0660
        cmp    dword ptr [ebp - 0x2c], 0
        je     loc_1_2E9B
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_0684
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        add    eax, dword ptr [ecx + 0xb1]
        add    eax, 2
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xd8]
        and    eax, 0xffff
        add    eax, dword ptr [ebp - 0x14]
        add    eax, 6
        mov    dword ptr [ebp - 0x30], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 0x28], ax
        mov    eax, dword ptr [ebp - 0x28]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 0x28]
        and    ecx, 0xffff
        mov    al, ch
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0xd8]
        and    ecx, 0xffff
        sub    eax, ecx
        mov    dword ptr [ebp - 0x18], eax
loc_1_2D28:
        mov    eax, dword ptr [ebp + 0x14]
        cmp    dword ptr [ebp - 0x34], eax
        jae    loc_1_2E93
        cmp    dword ptr [ebp - 0x20], 0
        jne    loc_1_2D70
        lea    eax, [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_06A8
        cmp    dword ptr [ebp - 0x24], 0
        jne    loc_1_2D5A
        jmp    loc_1_2E93
loc_1_2D5A:
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_0684
        jmp    loc_1_2D28
loc_1_2D70:
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_1_2DE0
        mov    eax, dword ptr [ebp - 0x14]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 0x10], ax
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_1_2D99
        jmp    loc_1_2E93
loc_1_2D99:
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        add    eax, dword ptr [ecx + 0xb1]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 6
        mov    dword ptr [ebp - 0x30], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 0x18], ax
        sub    eax, eax
        mov    ah, byte ptr [ebp - 0x18]
        mov    al, byte ptr [ebp - 0x17]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_1_2D28
loc_1_2DE0:
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_2E28
        mov    eax, dword ptr [ebp - 0x18]
        cmp    dword ptr [ebp + 0x10], eax
        jbe    loc_1_2E11
        sub    eax, eax
        sub    eax, dword ptr [ebp - 0x18]
        neg    eax
        sub    dword ptr [ebp + 0x10], eax
        mov    dword ptr [ebp - 0x18], 0
        jmp    loc_1_2D28
        db  0E9h, 017h, 000h, 000h, 000h
loc_1_2E11:
        mov    eax, dword ptr [ebp + 0x10]
        add    dword ptr [ebp - 0x30], eax
        sub    eax, eax
        sub    eax, dword ptr [ebp + 0x10]
        neg    eax
        sub    dword ptr [ebp - 0x18], eax
        mov    dword ptr [ebp + 0x10], 0
loc_1_2E28:
        mov    eax, dword ptr [ebp + 0x14]
        sub    eax, dword ptr [ebp - 0x34]
        mov    dword ptr [ebp - 0x3c], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x20]
        cmp    eax, ecx
        jb     loc_1_2E41
        mov    eax, ecx
loc_1_2E41:
        mov    dword ptr [ebp - 0x38], eax
        mov    eax, dword ptr [ebp - 0x3c]
        mov    ecx, dword ptr [ebp - 0x38]
        cmp    eax, ecx
        jb     loc_1_2E54
        mov    eax, ecx
loc_1_2E54:
        mov    dword ptr [ebp - 0x38], eax
        mov    eax, dword ptr [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_1_071C
        mov    eax, dword ptr [ebp - 0x38]
        add    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x38]
        add    dword ptr [ebp - 0x30], eax
        mov    eax, dword ptr [ebp - 0x38]
        add    dword ptr [ebp - 0x34], eax
        sub    eax, eax
        sub    eax, dword ptr [ebp - 0x38]
        neg    eax
        sub    dword ptr [ebp - 0x18], eax
        sub    eax, eax
        sub    eax, dword ptr [ebp - 0x38]
        neg    eax
        sub    dword ptr [ebp - 0x20], eax
        jmp    loc_1_2D28
loc_1_2E93:
        mov    eax, dword ptr [ebp - 0x34]
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], eax
loc_1_2E9B:
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_2EB4
loc_1_2EAD:
        mov    dword ptr [ebp - 8], 0xc001000c
loc_1_2EB4:
        jmp    loc_1_2F1A
loc_1_2EB9:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x15f]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_2EDB
        mov    dword ptr [ebp - 8], 0xc001000d
        jmp    loc_1_2F1A
loc_1_2EDB:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x170]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_2EFD
        mov    dword ptr [ebp - 8], 0xc0000001
        jmp    loc_1_2F1A
loc_1_2EFD:
        push   1
        push   0xb
        push   2
        push   0xaa000006
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x14
loc_1_2F1A:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x40], eax
        mov    eax, dword ptr [ebp - 0x40]
        dec    dword ptr [eax + 0x5d]
        mov    eax, dword ptr [ebp - 0x40]
        cmp    dword ptr [eax + 0x5d], 0
        jne    loc_1_2F69
        mov    eax, dword ptr [ebp - 0x40]
        mov    al, byte ptr [eax + 0x15f]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_2F5B
        mov    eax, dword ptr [ebp - 0x40]
        mov    ecx, dword ptr [ebp - 0x40]
        add    ecx, 0x69
        cmp    dword ptr [eax + 0x69], ecx
        je     loc_1_2F69
loc_1_2F5B:
        mov    eax, dword ptr [ebp - 0x40]
        push   eax
        call   sub_1_5080
        jmp    loc_1_2F69
loc_1_2F69:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_2F71
loc_1_2F71:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x18
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2F80:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        sub    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_304A
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0660
        cmp    dword ptr [ebp - 0x10], esi
        je     loc_1_304A
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0684
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_3045
        mov    edi, dword ptr [ebp + 0xc]
        mov    edi, edi
loc_1_2FD0:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_2FFC
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_06A8
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_3045
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0684
        jmp    loc_1_3040
loc_1_2FFC:
        or     edi, edi
        je     loc_1_3019
        cmp    dword ptr [ebp - 4], edi
        jae    loc_1_3011
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_3040
loc_1_3011:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_3019:
        mov    ebx, dword ptr [ebp + 0x10]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     loc_1_3027
        mov    ebx, eax
loc_1_3027:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        add    esi, ebx
        push   eax
        call   sub_1_071C
        add    dword ptr [ebp + 0x14], ebx
        add    dword ptr [ebp - 0xc], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_3040:
        cmp    esi, dword ptr [ebp + 0x10]
        jb     loc_1_2FD0
loc_1_3045:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_304A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        sub    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0xc], esi
        je     loc_1_312B
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_0660
        cmp    dword ptr [ebp - 0x14], esi
        je     loc_1_312B
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0684
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp + 0xc], esi
        je     loc_1_3126
        mov    edi, dword ptr [ebp + 0x14]
loc_1_30B4:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_30E0
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_06A8
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_3126
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0684
        jmp    loc_1_3121
loc_1_30E0:
        or     edi, edi
        je     loc_1_30FD
        cmp    dword ptr [ebp - 4], edi
        jae    loc_1_30F5
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_3121
loc_1_30F5:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_30FD:
        mov    eax, dword ptr [ebp + 0xc]
        sub    eax, esi
        mov    ebx, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     loc_1_310B
        mov    ebx, eax
loc_1_310B:
        push   ebx
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    esi, ebx
        push   eax
        call   sub_1_071C
        add    dword ptr [ebp - 0x10], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_3121:
        cmp    esi, dword ptr [ebp + 0xc]
        jb     loc_1_30B4
loc_1_3126:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_312B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 0x1c], 0
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_32C1
        push   0
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x24]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0660
        cmp    dword ptr [ebp - 0x24], 0
        je     loc_1_32C1
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0684
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        call   sub_1_0660
        cmp    dword ptr [ebp - 0x20], 0
        je     loc_1_32C1
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0684
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_32B9
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 0x18]
loc_1_31D3:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_3206
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_06A8
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_32B9
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0684
        jmp    loc_1_32AD
loc_1_3206:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_3236
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_06A8
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_32B9
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0684
        jmp    loc_1_32AD
loc_1_3236:
        or     esi, esi
        je     loc_1_3258
        cmp    esi, dword ptr [ebp - 4]
        jbe    loc_1_3250
        sub    esi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_32AD
        db  005h, 000h, 000h, 000h, 000h
loc_1_3250:
        add    dword ptr [ebp - 0x18], esi
        sub    dword ptr [ebp - 4], esi
        sub    esi, esi
loc_1_3258:
        or     edi, edi
        je     loc_1_3278
        cmp    edi, dword ptr [ebp - 8]
        jbe    loc_1_3270
        sub    edi, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_32AD
        db  08Dh, 049h, 000h
loc_1_3270:
        add    dword ptr [ebp - 0x14], edi
        sub    dword ptr [ebp - 8], edi
        sub    edi, edi
loc_1_3278:
        mov    ebx, dword ptr [ebp + 0x10]
        sub    ebx, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    eax, ecx
        jb     loc_1_328A
        mov    eax, ecx
loc_1_328A:
        cmp    ebx, eax
        jb     loc_1_3290
        mov    ebx, eax
loc_1_3290:
        push   ebx
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_071C
        add    dword ptr [ebp - 0x18], ebx
        add    dword ptr [ebp - 0x14], ebx
        add    dword ptr [ebp - 0x1c], ebx
        sub    dword ptr [ebp - 8], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_32AD:
        mov    ecx, dword ptr [ebp - 0x1c]
        cmp    dword ptr [ebp + 0x10], ecx
        ja     loc_1_31D3
loc_1_32B9:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax], ecx
loc_1_32C1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_32D0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx]
        mov    dl, byte ptr [eax + 0x48]
        mov    eax, dword ptr [ecx + 4]
        and    dl, 0x24
        mov    byte ptr [eax], dl
        mov    eax, dword ptr [ecx]
        pop    ebp
        and    byte ptr [eax + 0x48], 0xdb
        mov    al, 1
        ret    4
        db  090h
loc_1_32F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 0x379], 1
        jne    loc_1_330D
        mov    eax, dword ptr [0x56c0]       ; -> obj1:0x56C0
        push   eax
        call   sub_1_0630
loc_1_330D:
        mov    eax, dword ptr [esi]
        mov    cl, byte ptr [eax + 0x48]
        mov    eax, dword ptr [esi + 4]
        and    cl, 0x18
        mov    byte ptr [eax], cl
        mov    eax, dword ptr [esi]
        pop    esi
        and    byte ptr [eax + 0x48], 0xe7
        mov    al, 1
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3330:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    byte ptr [eax + 0x168], 1
        mov    al, 1
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3350:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    byte ptr [eax + 0x48], 0
        mov    al, 1
        ret    4
loc_1_3360:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0xad]
        mov    al, byte ptr [eax + 0x1e09]
        cmp    al, 0xff
        mov    byte ptr [ebp - 1], al
        jne    loc_1_33CF
        cmp    byte ptr [ecx + 0x16e], 0
        jne    loc_1_33CF
        sub    eax, eax
        sub    edx, edx
        add    eax, 0
loc_1_3390:
        mov    esi, dword ptr [ecx + 0xad]
        mov    bl, byte ptr [edx + esi + 0x1f40]
        mov    byte ptr [ebp - 2], bl
        and    ebx, 0xf
        shl    eax, 4
        add    eax, ebx
        add    edx, 2
        cmp    edx, 0x10
        jb     loc_1_3390
        cmp    eax, 0x5049434f
        je     loc_1_33C8
        mov    byte ptr [ecx + 0x175], 1
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_33C8:
        mov    byte ptr [ecx + 0x175], 0
loc_1_33CF:
        cmp    byte ptr [ecx + 0x2e], 0
        jne    loc_1_33F3
        mov    eax, dword ptr [ecx + 0x18]
        inc    eax
        mov    dword ptr [ecx + 0x18], eax
        cmp    eax, 0xff
        jb     loc_1_33F3
        mov    edx, dword ptr [ecx + 0x59]
        inc    dx
        sub    al, al
        out    dx, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_33F3:
        mov    dl, byte ptr [ebp - 1]
        mov    eax, dword ptr [ecx + 0xad]
        not    dl
        mov    byte ptr [eax + 0x1e29], dl
        cmp    byte ptr [ecx + 0x171], 0
        je     loc_1_3420
        mov    al, byte ptr [ebp - 1]
        and    al, 0xdf
        or     byte ptr [ecx + 0x49], al
        and    byte ptr [ebp - 1], 0x20
        jmp    loc_1_342A
        db  005h, 000h, 000h, 000h, 000h
loc_1_3420:
        mov    al, byte ptr [ecx + 0x49]
        or     byte ptr [ebp - 1], al
        mov    byte ptr [ecx + 0x49], 0
loc_1_342A:
        mov    al, byte ptr [ebp - 1]
        or     byte ptr [ecx + 0x48], al
        cmp    byte ptr [ecx + 0x168], 0
        je     loc_1_3440
        mov    byte ptr [ecx + 0x169], 1
loc_1_3440:
        cmp    byte ptr [ecx + 0x16e], 0
        je     loc_1_34E0
        mov    eax, dword ptr [ecx + 0xad]
        mov    al, byte ptr [eax + 0x1e02]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ecx + 0xad]
        mov    al, byte ptr [eax + 0x1e03]
        mov    byte ptr [ebp - 3], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0xffff00ff
        shl    ax, 8
        mov    dl, byte ptr [ebp - 3]
        and    edx, 0xffff00ff
        add    ax, dx
        test   al, 1
        je     loc_1_34A1
        cmp    byte ptr [ecx + 0x38], 0
        je     loc_1_3497
        mov    edx, dword ptr [ecx + 0x59]
        add    dx, 3
        sub    al, al
        out    dx, al
loc_1_3497:
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_34A1:
        mov    word ptr [ecx + 0xb5], ax
        mov    edx, dword ptr [ecx + 0xb1]
        and    eax, 0xffff
        mov    ax, word ptr [edx + eax + 6]
        or     ax, ax
        jne    loc_1_34C1
        mov    byte ptr [ecx + 0x2e], 1
loc_1_34C1:
        cmp    byte ptr [ecx + 0x38], 0
        je     loc_1_34D1
        mov    edx, dword ptr [ecx + 0x59]
        add    dx, 3
        sub    al, al
        out    dx, al
loc_1_34D1:
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_34E0:
        cmp    byte ptr [ecx + 0x38], 0
        je     loc_1_34F0
        mov    edx, dword ptr [ecx + 0x59]
        add    dx, 3
        sub    al, al
        out    dx, al
loc_1_34F0:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_3520
        mov    eax, dword ptr [ecx + 0xad]
        mov    al, byte ptr [eax + 0x1e08]
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], 0x1c
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ecx + 0x4a], al
        mov    dl, byte ptr [ebp - 1]
        mov    eax, dword ptr [ecx + 0xad]
        not    dl
        mov    byte ptr [eax + 0x1e28], dl
loc_1_3520:
        cmp    byte ptr [ecx + 0x48], 0
        je     loc_1_3530
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3530:
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3540:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x18], 0
        mov    byte ptr [esi + 0x2c], 0
        cmp    byte ptr [esi + 0x4a], 0
        je     loc_1_355C
        mov    byte ptr [esi + 0x4a], 0
loc_1_355C:
        test   byte ptr [esi + 0x48], 0x24
        je     loc_1_356E
        cmp    byte ptr [esi + 0x4b], 0
        jne    loc_1_356E
        push   esi
        call   sub_1_3590
loc_1_356E:
        test   byte ptr [esi + 0x48], 0x18
        je     loc_1_3580
        cmp    byte ptr [esi + 0x4c], 0
        jne    loc_1_3580
        push   esi
        call   sub_1_3BE0
loc_1_3580:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3590:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x5d]
        cmp    byte ptr [esi + 0x15f], 0
        je     loc_1_35F0
        cmp    byte ptr [esi + 0x169], 0
        je     loc_1_35F0
        push   esi
        call   sub_1_44F0
        mov    eax, dword ptr [esi + 0x5d]
        dec    eax
        mov    dword ptr [esi + 0x5d], eax
        jne    loc_1_3BD2
        cmp    byte ptr [esi + 0x15f], 0
        jne    loc_1_35DB
        mov    eax, dword ptr [esi + 0x69]
        sub    eax, esi
        cmp    eax, 0x69
        je     loc_1_3BD2
loc_1_35DB:
        push   esi
        call   sub_1_5080
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_35F0:
        mov    byte ptr [esi + 0x4b], 1
        cmp    dword ptr [esi + 0x379], 1
        je     loc_1_3617
        mov    dword ptr [ebp - 0x24], esi
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0x20], eax
        lea    eax, [ebp - 0x24]
        push   eax
        push   0x32d0                        ; -> loc_1_32D0
        push   esi
        call   sub_1_062A
        jmp    loc_1_3652
loc_1_3617:
        mov    eax, dword ptr [esi + 0xad]
        mov    al, byte ptr [eax + 0x1e08]
        mov    byte ptr [ebp - 8], al
        mov    cl, al
        and    cl, 0x3f
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e08], cl
        cli    
        mov    al, byte ptr [esi + 0x48]
        and    al, 0x24
        mov    byte ptr [ebp - 4], al
        and    byte ptr [esi + 0x48], 0xdb
        sti    
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e08], 0xc0
loc_1_3652:
        test   byte ptr [ebp - 4], 0x24
        je     loc_1_3BAC
        lea    esp, [esp]
loc_1_3660:
        cmp    byte ptr [esi + 0x174], 0
        je     loc_1_3676
        cmp    byte ptr [esi + 0x176], 0
        je     loc_1_3BAC
loc_1_3676:
        test   byte ptr [ebp - 4], 0x20
        je     loc_1_3A01
        cmp    byte ptr [esi + 0x170], 0
        je     loc_1_3926
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0xb5]
        add    ecx, dword ptr [esi + 0xb1]
        mov    al, byte ptr [ecx + 2]
        or     al, al
        jne    loc_1_3886
        mov    ax, word ptr [ecx + 0xa]
        cmp    byte ptr [esi + 0x3a], 0
        mov    word ptr [ebp - 0xc], ax
        je     loc_1_3782
        mov    ah, al
        mov    al, byte ptr [ebp - 0xb]
        mov    word ptr [ebp - 0xc], ax
        and    eax, 0x3fff
        mov    edx, dword ptr [esi + 0xb1]
        add    edx, eax
        mov    dword ptr [esi + 0xb7], edx
        mov    ax, word ptr [ebp - 0xc]
        shr    ax, 0xe
        mov    byte ptr [esi + 0xc7], al
        mov    ax, word ptr [ecx + 0xe]
        mov    word ptr [ebp - 0xc], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 0xb]
        mov    word ptr [ebp - 0xc], ax
        and    eax, 0x3fff
        mov    edx, dword ptr [esi + 0xb1]
        add    edx, eax
        mov    dword ptr [esi + 0xbb], edx
        mov    ax, word ptr [ebp - 0xc]
        shr    ax, 0xe
        mov    byte ptr [esi + 0xc8], al
        mov    ax, word ptr [ecx + 0xc]
        mov    word ptr [ebp - 0xc], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 0xb]
        mov    word ptr [ebp - 0xc], ax
        lea    edx, [esi + 0xbf]
        and    eax, 0x3fff
        mov    edi, dword ptr [esi + 0xb1]
        add    edi, eax
        mov    dword ptr [edx], edi
        mov    ax, word ptr [ebp - 0xc]
        shr    ax, 0xe
        mov    byte ptr [esi + 0xc9], al
        mov    ax, word ptr [ecx + 8]
        mov    word ptr [ebp - 0xc], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 0xb]
        mov    word ptr [ebp - 0xc], ax
        lea    edi, [esi + 0xc3]
        and    eax, 0x3fff
        mov    ebx, dword ptr [esi + 0xb1]
        add    ebx, eax
        mov    dword ptr [edi], ebx
        mov    ax, word ptr [ebp - 0xc]
        shr    ax, 0xe
        mov    byte ptr [esi + 0xca], al
        jmp    loc_1_3804
loc_1_3782:
        lea    eax, [esi + 0xb7]
        mov    ah, byte ptr [ebp - 0xc]
        mov    al, byte ptr [ebp - 0xb]
        and    eax, 0xffff
        add    eax, dword ptr [esi + 0xb1]
        mov    dword ptr [esi + 0xb7], eax
        mov    ax, word ptr [ecx + 0xe]
        mov    word ptr [ebp - 0xc], ax
        lea    eax, [esi + 0xbb]
        mov    ah, byte ptr [ebp - 0xc]
        mov    al, byte ptr [ebp - 0xb]
        and    eax, 0xffff
        add    eax, dword ptr [esi + 0xb1]
        mov    dword ptr [esi + 0xbb], eax
        mov    ax, word ptr [ecx + 0xc]
        mov    word ptr [ebp - 0xc], ax
        lea    edx, [esi + 0xbf]
        mov    ah, al
        mov    al, byte ptr [ebp - 0xb]
        and    eax, 0xffff
        add    eax, dword ptr [esi + 0xb1]
        mov    dword ptr [edx], eax
        mov    ax, word ptr [ecx + 8]
        mov    word ptr [ebp - 0xc], ax
        lea    edi, [esi + 0xc3]
        mov    ah, al
        mov    al, byte ptr [ebp - 0xb]
        and    eax, 0xffff
        add    eax, dword ptr [esi + 0xb1]
        mov    dword ptr [edi], eax
loc_1_3804:
        mov    eax, dword ptr [esi + 0x3f]
        add    eax, dword ptr [esi + 0xb1]
        cmp    dword ptr [esi + 0xb7], eax
        ja     loc_1_3886
        cmp    dword ptr [esi + 0xbb], eax
        ja     loc_1_3886
        cmp    dword ptr [edx], eax
        ja     loc_1_3886
        cmp    dword ptr [edi], eax
        ja     loc_1_3886
        mov    dword ptr [esi + 0x15b], 1
        mov    byte ptr [esi + 0x170], 0
        mov    word ptr [esi + 0x172], 0
        mov    byte ptr [esi + 0x16f], 0
        mov    eax, esi
        mov    byte ptr [esi + 0x171], 0
        mov    edi, dword ptr [esi + 0x61]
        sub    eax, edi
        cmp    eax, -0x61
        je     loc_1_387B
loc_1_3859:
        cmp    byte ptr [edi + 0x1c], 0
        je     loc_1_3870
        mov    byte ptr [edi + 0x1c], 0
        push   0
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_056E
loc_1_3870:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x61
        jne    loc_1_3859
loc_1_387B:
        push   esi
        call   sub_1_4930
        jmp    loc_1_3A01
loc_1_3886:
        mov    dword ptr [esi + 0x15b], 5
        mov    byte ptr [esi + 0x170], 0
        mov    ax, word ptr [ecx + 6]
        mov    word ptr [esi + 0x172], ax
        mov    ch, al
        mov    cl, ah
        mov    word ptr [esi + 0x172], cx
        mov    byte ptr [esi + 0x16f], 1
        mov    byte ptr [esi + 0x171], 1
        mov    edi, dword ptr [esi + 0x61]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x61
        je     loc_1_3A01
        add    eax, 0
loc_1_38D0:
        mov    ebx, edi
        cmp    byte ptr [edi + 0x1c], 0
        je     loc_1_3918
        mov    byte ptr [edi + 0x1c], 0
        xor    eax, eax
        mov    ax, word ptr [esi + 0x172]
        or     eax, 0xc0011000
        push   eax
        push   0xc0010007
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_056E
        mov    eax, dword ptr [ebx + 4]
        mov    edi, dword ptr [edi]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0x96
        push   ebx
        call   sub_1_063C
        dec    dword ptr [esi + 0x5d]
loc_1_3918:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x61
        jne    loc_1_38D0
        jmp    loc_1_3A01
loc_1_3926:
        cmp    dword ptr [esi + 0xce], 0
        je     loc_1_3948
        lea    eax, [ebp - 9]
        push   eax
        push   esi
        call   sub_1_4810
        cmp    byte ptr [ebp - 9], 0
        je     loc_1_3A01
        jmp    loc_1_39FB
loc_1_3948:
        cmp    byte ptr [esi + 0xcb], 0
        jne    loc_1_3A01
        mov    eax, dword ptr [esi + 0xb7]
        mov    cl, byte ptr [eax + 2]
        mov    byte ptr [ebp - 7], cl
        mov    al, byte ptr [eax]
        or     al, al
        mov    byte ptr [ebp - 2], al
        je     loc_1_39FB
        cmp    al, 8
        je     loc_1_39A7
        cmp    al, 7
        je     loc_1_39A7
        cmp    al, 6
        je     loc_1_39A7
        cmp    al, 0x1e
        je     loc_1_39A7
        and    eax, 0xff
        push   eax
        push   8
        push   4
        push   3
        push   0xaa00000b
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x18
        mov    byte ptr [esi + 0x175], 1
        jmp    loc_1_3A01
loc_1_39A7:
        mov    eax, dword ptr [esi + 0x91]
        cmp    dword ptr [eax + 0x10], 8
        jne    loc_1_39E8
        cmp    byte ptr [ebp - 7], 0
        jne    loc_1_39CC
        cmp    dword ptr [eax + 4], 0
        push   esi
        je     loc_1_39C7
        call   sub_1_4A80
        jmp    loc_1_39CC
loc_1_39C7:
        call   sub_1_4A00
loc_1_39CC:
        push   0
        push   0x14
        mov    eax, dword ptr [esi + 0x91]
        push   eax
        call   sub_1_063C
        mov    dword ptr [esi + 0x91], 0
        jmp    loc_1_39FB
loc_1_39E8:
        cmp    byte ptr [ebp - 7], 1
        sbb    al, al
        neg    al
        push   eax
        push   esi
        call   sub_1_4D60
        or     al, al
        je     loc_1_3A01
loc_1_39FB:
        push   esi
        call   sub_1_48C0
loc_1_3A01:
        test   byte ptr [ebp - 4], 4
        je     loc_1_3B44
        mov    ecx, dword ptr [esi + 0xbb]
        mov    al, byte ptr [ecx]
        mov    byte ptr [ebp - 2], al
        cmp    al, 0xa
        jne    loc_1_3B10
        xor    eax, eax
        mov    al, byte ptr [esi + 0xcd]
        add    eax, 0x7f
        cdq    
        xor    eax, edx
        sub    eax, edx
        and    eax, 0x7f
        xor    eax, edx
        sub    eax, edx
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ecx + 1]
        mov    byte ptr [ebp - 3], al
        mov    al, byte ptr [ecx + 2]
        or     al, al
        mov    byte ptr [ebp - 6], al
        jne    loc_1_3A50
        sub    edi, edi
        jmp    loc_1_3A85
        db  08Dh, 064h, 024h, 000h
loc_1_3A50:
        cmp    byte ptr [ebp - 6], 0x22
        jne    loc_1_3A80
        mov    al, byte ptr [ecx + 6]
        mov    byte ptr [ebp - 5], al
        shr    al, 6
        mov    cl, byte ptr [ebp - 5]
        and    cl, 0xc
        shr    cl, 2
        cmp    cl, al
        jne    loc_1_3A74
        or     al, al
        jne    loc_1_3A74
        sub    edi, edi
        jmp    loc_1_3A85
loc_1_3A74:
        mov    edi, 0x10001
        jmp    loc_1_3A85
        db  005h, 000h, 000h, 000h, 000h
loc_1_3A80:
        mov    edi, 0xc0000001
loc_1_3A85:
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        inc    eax
        cdq    
        xor    eax, edx
        sub    eax, edx
        and    eax, 0x7f
        xor    eax, edx
        sub    eax, edx
        mov    byte ptr [ebp - 1], al
        and    eax, 0xff
        mov    ecx, dword ptr [esi + eax*4 + 0x179]
        or     ecx, ecx
        je     loc_1_3AE7
        mov    byte ptr [ecx + 0x30], 0
        xor    eax, eax
        mov    al, byte ptr [ecx + 0x31]
        mov    dword ptr [esi + eax*4 + 0x179], 0
        mov    ebx, dword ptr [ecx + 0x28]
        or     edi, edi
        jne    loc_1_3AD0
        inc    dword ptr [esi + 0xf7]
        jmp    loc_1_3AD6
        db  08Dh, 049h, 000h
loc_1_3AD0:
        inc    dword ptr [esi + 0xff]
loc_1_3AD6:
        push   edi
        mov    eax, dword ptr [ecx + 0x2c]
        push   eax
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_057A
        dec    dword ptr [ebx + 0x14]
loc_1_3AE7:
        mov    al, byte ptr [ebp - 1]
        cmp    al, byte ptr [ebp - 3]
        jne    loc_1_3A85
        and    eax, 0xff
        inc    eax
        cdq    
        xor    eax, edx
        sub    eax, edx
        and    eax, 0x7f
        xor    eax, edx
        sub    eax, edx
        mov    byte ptr [esi + 0xcd], al
        push   esi
        call   sub_1_48C0
        jmp    loc_1_3B37
        db  090h
loc_1_3B10:
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        push   eax
        push   8
        push   4
        push   3
        push   0xaa00000b
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x18
        mov    byte ptr [esi + 0x175], 1
loc_1_3B37:
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e4b], 1
loc_1_3B44:
        cmp    dword ptr [esi + 0x379], 1
        je     loc_1_3B67
        mov    dword ptr [ebp - 0x1c], esi
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0x18], eax
        lea    eax, [ebp - 0x1c]
        push   eax
        push   0x32d0                        ; -> loc_1_32D0
        push   esi
        call   sub_1_062A
        jmp    loc_1_3BA2
loc_1_3B67:
        mov    eax, dword ptr [esi + 0xad]
        mov    al, byte ptr [eax + 0x1e08]
        mov    byte ptr [ebp - 8], al
        mov    cl, al
        and    cl, 0x3f
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e08], cl
        cli    
        mov    al, byte ptr [esi + 0x48]
        and    al, 0x24
        mov    byte ptr [ebp - 4], al
        and    byte ptr [esi + 0x48], 0xdb
        sti    
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e08], 0xc0
loc_1_3BA2:
        test   byte ptr [ebp - 4], 0x24
        jne    loc_1_3660
loc_1_3BAC:
        mov    byte ptr [esi + 0x4b], 0
        mov    eax, dword ptr [esi + 0x5d]
        dec    eax
        mov    dword ptr [esi + 0x5d], eax
        jne    loc_1_3BD2
        cmp    byte ptr [esi + 0x15f], 0
        jne    loc_1_3BCC
        mov    eax, dword ptr [esi + 0x69]
        sub    eax, esi
        cmp    eax, 0x69
        je     loc_1_3BD2
loc_1_3BCC:
        push   esi
        call   sub_1_5080
loc_1_3BD2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_3BE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x3c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x5d]
        mov    byte ptr [esi + 0x4c], 1
        cmp    dword ptr [esi + 0x379], 1
        je     loc_1_3C16
        mov    dword ptr [ebp - 0x3c], esi
        lea    eax, [ebp - 2]
        mov    dword ptr [ebp - 0x38], eax
        lea    eax, [ebp - 0x3c]
        push   eax
        push   0x32f0                        ; -> loc_1_32F0
        push   esi
        call   sub_1_062A
        jmp    loc_1_3C51
loc_1_3C16:
        mov    eax, dword ptr [esi + 0xad]
        mov    al, byte ptr [eax + 0x1e08]
        mov    byte ptr [ebp - 5], al
        mov    cl, al
        and    cl, 0x3f
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e08], cl
        cli    
        mov    al, byte ptr [esi + 0x48]
        and    al, 0x18
        mov    byte ptr [ebp - 2], al
        and    byte ptr [esi + 0x48], 0xe7
        sti    
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e08], 0xc0
loc_1_3C51:
        test   byte ptr [ebp - 2], 0x18
        je     loc_1_43B5
loc_1_3C5B:
        test   byte ptr [ebp - 2], 8
        je     loc_1_4213
        mov    eax, dword ptr [esi + 0xbf]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 1], al
        and    eax, 0xff
        cmp    eax, 0x81
        je     loc_1_3CB0
        cmp    eax, 0x82
        je     loc_1_3E40
        cmp    eax, 0x83
        je     loc_1_3F60
        cmp    eax, 0x84
        je     loc_1_4020
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e4b], 2
        jmp    loc_1_4213
        db  090h
loc_1_3CB0:
        cmp    byte ptr [esi + 0x174], 0
        je     loc_1_3CC6
        cmp    byte ptr [esi + 0x176], 0
        je     loc_1_4213
loc_1_3CC6:
        mov    edx, dword ptr [esi + 0xbf]
        mov    ax, word ptr [edx + 6]
        mov    word ptr [ebp - 0xa], ax
        mov    word ptr [esi + 0xd6], ax
        mov    ah, byte ptr [ebp - 0xa]
        mov    al, byte ptr [ebp - 9]
        mov    ecx, eax
        and    ecx, 0xffff
        mov    eax, dword ptr [esi + 0xb1]
        lea    ecx, [ecx + eax + 2]
        mov    ax, word ptr [edx + 0xa]
        mov    word ptr [ebp - 0x20], ax
        mov    al, ah
        mov    ah, byte ptr [ebp - 0x20]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x20], eax
        mov    ax, word ptr [ecx + 4]
        mov    word ptr [ebp - 0x14], ax
        mov    al, ah
        mov    ah, byte ptr [ebp - 0x14]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [esi + 0xad]
        lea    edi, [ecx + 6]
        mov    byte ptr [eax + 0x1e4b], 2
        mov    al, byte ptr [edi + 8]
        mov    byte ptr [ebp - 1], al
        test   al, 0x80
        je     loc_1_3D47
        mov    al, byte ptr [edi + 0xe]
        mov    byte ptr [ebp - 1], al
        mov    dl, al
        and    edx, 0x1f
        add    edx, 0xe
        jmp    loc_1_3D4C
loc_1_3D47:
        mov    edx, 0xe
loc_1_3D4C:
        mov    word ptr [esi + 0xd8], dx
        inc    dword ptr [esi + 0xfb]
        cmp    edx, dword ptr [ebp - 0x14]
        jbe    loc_1_3D74
        cmp    dword ptr [ebp - 0x14], 6
        jb     loc_1_3D97
        or     edi, edi
        je     loc_1_3D97
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        jmp    loc_1_3D89
loc_1_3D74:
        or     edi, edi
        je     loc_1_3D97
        mov    eax, dword ptr [ebp - 0x20]
        sub    eax, edx
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        sub    eax, edx
        push   eax
        lea    eax, [edi + edx]
        push   eax
        push   edx
loc_1_3D89:
        push   edi
        push   ecx
        mov    eax, dword ptr [esi + 0x85]
        push   eax
        call   sub_1_00B0
loc_1_3D97:
        mov    eax, dword ptr [esi + 0x85]
        push   eax
        call   sub_1_0300
        cmp    byte ptr [esi + 0xcc], 0
        je     loc_1_3DD7
        mov    byte ptr [esi + 0xcc], 0
        mov    byte ptr [esi + 0xda], 0
        mov    ax, word ptr [ebp - 0xa]
        push   eax
        push   esi
        call   sub_1_5010
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e4b], 0x10
        jmp    loc_1_4213
loc_1_3DD7:
        mov    ax, word ptr [esi + 0xd4]
        cmp    ax, 0xffff
        jne    loc_1_3DF1
        mov    ax, word ptr [ebp - 0xa]
        mov    word ptr [esi + 0xd2], ax
        jmp    loc_1_3E0A
loc_1_3DF1:
        mov    ch, al
        mov    cl, ah
        mov    edx, dword ptr [esi + 0xb1]
        mov    eax, ecx
        and    eax, 0xffff
        mov    cx, word ptr [ebp - 0xa]
        mov    word ptr [eax + edx], cx
loc_1_3E0A:
        mov    ax, word ptr [ebp - 0xa]
        cmp    byte ptr [esi + 0x178], 0
        mov    word ptr [esi + 0xd4], ax
        jne    loc_1_4213
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [esi + 0x178], 1
        mov    byte ptr [eax + 0x1e4b], 4
        jmp    loc_1_4213
        db  005h, 000h, 000h, 000h, 000h
loc_1_3E40:
        cmp    byte ptr [esi + 0x174], 0
        je     loc_1_3E56
        cmp    byte ptr [esi + 0x176], 0
        je     loc_1_4213
loc_1_3E56:
        mov    ebx, dword ptr [esi + 0xbf]
        mov    byte ptr [ebp - 4], 0
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 1], al
        and    eax, 0xff
        mov    edi, dword ptr [esi + eax*4 + 0x179]
        or     edi, edi
        je     loc_1_3E78
        jmp    loc_1_3E96
loc_1_3E78:
        lea    eax, [ebp - 6]
        push   eax
        push   esi
        call   sub_1_4810
        mov    edi, eax
        or     edi, edi
        je     loc_1_4213
        cmp    byte ptr [ebp - 6], 0
        je     loc_1_4213
loc_1_3E96:
        mov    ax, word ptr [ebx + 6]
        mov    word ptr [ebp - 0xc], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 0xb]
        mov    ebx, eax
        and    ebx, 0xffff
        mov    eax, dword ptr [esi + 0xad]
        add    ebx, dword ptr [esi + 0xb1]
        mov    byte ptr [eax + 0x1e4b], 2
        lea    eax, [ebp - 0x24]
        push   eax
        push   0
        push   0
        push   0
        push   edi
        call   sub_1_0660
        lea    eax, [ebp - 0x2c]
        push   eax
        push   ebx
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        push   0
        push   edi
        call   sub_1_2F80
        cmp    byte ptr [esi + 0xcc], 0
        je     loc_1_3F0C
        mov    byte ptr [esi + 0xcc], 0
        mov    byte ptr [esi + 0xda], 1
        push   edi
        push   esi
        call   sub_1_4990
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e4b], 0x10
        jmp    loc_1_3F30
loc_1_3F0C:
        push   edi
        push   esi
        call   sub_1_5040
        cmp    byte ptr [esi + 0x178], 0
        jne    loc_1_3F30
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [esi + 0x178], 1
        mov    byte ptr [eax + 0x1e4b], 4
loc_1_3F30:
        cmp    byte ptr [ebp - 4], 0
        je     loc_1_4213
        cmp    byte ptr [esi + 0xcb], 0
        je     loc_1_4213
        mov    byte ptr [esi + 0xcb], 0
        push   esi
        call   sub_1_48C0
        jmp    loc_1_4213
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3F60:
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e4b], 2
        mov    eax, dword ptr [esi + 0xbf]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 0xc], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 0xb]
        test   al, 0x40
        je     loc_1_4213
        mov    eax, dword ptr [0x56c8]       ; -> obj1:0x56C8
        push   eax
        push   0
        push   0x14
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_0636
        or     eax, eax
        je     loc_1_3FC0
        push   5
        push   4
        push   2
        push   0xaa000002
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x14
        jmp    loc_1_4213
        db  08Bh, 0FFh
loc_1_3FC0:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 0x10], 8
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [esi + 0x89]
        or     eax, eax
        jne    loc_1_3FF2
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [esi + 0x8d], eax
        jmp    loc_1_3FFA
loc_1_3FF2:
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x1c]
loc_1_3FFA:
        mov    dword ptr [esi + 0x89], eax
        cmp    byte ptr [esi + 0xcb], 0
        je     loc_1_4213
        mov    byte ptr [esi + 0xcb], 0
        push   esi
        call   sub_1_48C0
        jmp    loc_1_4213
        db  090h
loc_1_4020:
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [esi + 0xbf]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 8], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 7]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e4b], 2
        test   byte ptr [ebp - 8], 0x80
        je     loc_1_40E6
        mov    eax, dword ptr [0x56c8]       ; -> obj1:0x56C8
        push   eax
        push   0
        push   0x14
        lea    eax, [ebp - 0x18]
        push   eax
        call   sub_1_0636
        or     eax, eax
        je     loc_1_4090
        push   5
        push   4
        push   2
        push   0xaa000002
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x14
        jmp    loc_1_4213
        db  005h, 000h, 000h, 000h, 000h
loc_1_4090:
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x10], 8
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [esi + 0x89]
        or     eax, eax
        jne    loc_1_40C2
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [esi + 0x8d], eax
        jmp    loc_1_40CA
loc_1_40C2:
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x18]
loc_1_40CA:
        mov    dword ptr [esi + 0x89], eax
        cmp    byte ptr [esi + 0xcb], 0
        je     loc_1_40E6
        mov    byte ptr [esi + 0xcb], 0
        push   esi
        call   sub_1_48C0
loc_1_40E6:
        test   byte ptr [ebp - 8], 0x20
        je     loc_1_40F0
        or     dword ptr [ebp - 0x10], 0x40
loc_1_40F0:
        test   byte ptr [ebp - 8], 0x40
        je     loc_1_40FD
        or     dword ptr [ebp - 0x10], 0x80
loc_1_40FD:
        test   byte ptr [ebp - 8], 0x80
        je     loc_1_410A
        or     dword ptr [ebp - 0x10], 0x100
loc_1_410A:
        test   byte ptr [ebp - 7], 1
        je     loc_1_4117
        or     dword ptr [ebp - 0x10], 0x200
loc_1_4117:
        test   byte ptr [ebp - 7], 4
        je     loc_1_4124
        or     dword ptr [ebp - 0x10], 0x400
loc_1_4124:
        test   byte ptr [ebp - 7], 8
        je     loc_1_4131
        or     dword ptr [ebp - 0x10], 0x800
loc_1_4131:
        test   byte ptr [ebp - 7], 0x10
        je     loc_1_413E
        or     dword ptr [ebp - 0x10], 0x1000
loc_1_413E:
        test   byte ptr [ebp - 7], 0x20
        je     loc_1_414B
        or     dword ptr [ebp - 0x10], 0x2000
loc_1_414B:
        test   byte ptr [ebp - 7], 0x40
        je     loc_1_4158
        or     dword ptr [ebp - 0x10], 0x4000
loc_1_4158:
        test   byte ptr [ebp - 7], 0x80
        je     loc_1_4165
        or     dword ptr [ebp - 0x10], 0x8000
loc_1_4165:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_41B0
        mov    edi, dword ptr [esi + 0x61]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x61
        je     loc_1_41A7
loc_1_4177:
        inc    dword ptr [edi + 0x14]
        lea    ebx, [edi + 0x14]
        push   4
        lea    eax, [ebp - 0x10]
        push   eax
        push   0x40010006
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_05A4
        mov    edi, dword ptr [edi]
        dec    dword ptr [ebx]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x61
        jne    loc_1_4177
loc_1_41A7:
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [esi + 0x157], eax
loc_1_41B0:
        mov    ax, word ptr [ebp - 8]
        and    ah, 0x50
        cmp    ah, 0x50
        jne    loc_1_41E0
        mov    byte ptr [esi + 0x174], 1
        push   0xc0010008
        push   esi
        call   sub_1_5220
        test   byte ptr [ebp - 7], 8
        je     loc_1_4213
        mov    byte ptr [esi + 0x177], 1
        jmp    loc_1_4213
        db  08Dh, 049h, 000h
loc_1_41E0:
        test   byte ptr [ebp - 8], 0x20
        je     loc_1_4213
        test   byte ptr [ebp - 7], 0x40
        jne    loc_1_4213
        cmp    byte ptr [esi + 0x174], 0
        je     loc_1_4213
        cmp    byte ptr [esi + 0x176], 0
        jne    loc_1_4213
        mov    byte ptr [esi + 0x176], 1
        push   0
        push   esi
        call   sub_1_1EF0
        push   esi
        call   sub_1_5080
loc_1_4213:
        test   byte ptr [ebp - 2], 0x10
        je     loc_1_4347
        mov    byte ptr [ebp - 3], 0
        sub    bl, bl
        cmp    byte ptr [esi + 0x174], bl
        je     loc_1_4237
        cmp    byte ptr [esi + 0x176], bl
        je     loc_1_43B5
loc_1_4237:
        mov    cx, word ptr [esi + 0xd2]
        cmp    cx, 0xffff
        je     loc_1_4249
        mov    byte ptr [ebp - 3], 1
loc_1_4249:
        mov    edx, dword ptr [esi + 0xef]
        or     edx, edx
        je     loc_1_4255
        mov    bl, 1
loc_1_4255:
        cmp    byte ptr [ebp - 3], 0
        je     loc_1_42E6
        or     bl, bl
        je     loc_1_426C
        cmp    byte ptr [esi + 0xda], 0
        je     loc_1_42E6
loc_1_426C:
        cmp    word ptr [esi + 0xd4], cx
        jne    loc_1_4289
        mov    word ptr [esi + 0xd2], 0xffff
        mov    word ptr [esi + 0xd4], 0xffff
        jmp    loc_1_42A3
loc_1_4289:
        mov    ah, cl
        mov    al, ch
        and    eax, 0xffff
        mov    edx, dword ptr [esi + 0xb1]
        mov    ax, word ptr [eax + edx]
        mov    word ptr [esi + 0xd2], ax
loc_1_42A3:
        mov    byte ptr [esi + 0xcc], 0
        mov    byte ptr [esi + 0xda], 0
        push   ecx
        push   esi
        call   sub_1_5010
        or     bl, bl
        je     loc_1_42D0
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e4b], 0x14
        jmp    loc_1_4347
        db  005h, 000h, 000h, 000h, 000h
loc_1_42D0:
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [esi + 0x178], 0
        mov    byte ptr [eax + 0x1e4b], 0x10
        jmp    loc_1_4347
loc_1_42E6:
        or     bl, bl
        je     loc_1_4340
        mov    eax, dword ptr [edx + 0x24]
        mov    dword ptr [esi + 0xef], eax
        mov    byte ptr [esi + 0xcc], 0
        mov    byte ptr [esi + 0xda], 1
        push   edx
        push   esi
        call   sub_1_4990
        cmp    byte ptr [ebp - 3], 0
        jne    loc_1_4330
        cmp    dword ptr [esi + 0xef], 0
        jne    loc_1_4330
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [esi + 0x178], 0
        mov    byte ptr [eax + 0x1e4b], 0x10
        jmp    loc_1_4347
        db  08Dh, 049h, 000h
loc_1_4330:
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e4b], 0x14
        jmp    loc_1_4347
        db  090h
loc_1_4340:
        mov    byte ptr [esi + 0xcc], 1
loc_1_4347:
        cmp    dword ptr [esi + 0x379], 1
        je     loc_1_4370
        mov    dword ptr [ebp - 0x34], esi
        lea    eax, [ebp - 2]
        mov    dword ptr [ebp - 0x30], eax
        lea    eax, [ebp - 0x34]
        push   eax
        push   0x32f0                        ; -> loc_1_32F0
        push   esi
        call   sub_1_062A
        jmp    loc_1_43AB
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_4370:
        mov    eax, dword ptr [esi + 0xad]
        mov    al, byte ptr [eax + 0x1e08]
        mov    byte ptr [ebp - 5], al
        mov    cl, al
        and    cl, 0x3f
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e08], cl
        cli    
        mov    al, byte ptr [esi + 0x48]
        and    al, 0x18
        mov    byte ptr [ebp - 2], al
        and    byte ptr [esi + 0x48], 0xe7
        sti    
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e08], 0xc0
loc_1_43AB:
        test   byte ptr [ebp - 2], 0x18
        jne    loc_1_3C5B
loc_1_43B5:
        mov    byte ptr [esi + 0x4c], 0
        mov    eax, dword ptr [esi + 0x5d]
        dec    eax
        mov    dword ptr [esi + 0x5d], eax
        jne    loc_1_43DB
        cmp    byte ptr [esi + 0x15f], 0
        jne    loc_1_43D5
        mov    eax, dword ptr [esi + 0x69]
        sub    eax, esi
        cmp    eax, 0x69
        je     loc_1_43DB
loc_1_43D5:
        push   esi
        call   sub_1_5080
loc_1_43DB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_43F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x176], 0
        jne    loc_1_4483
        mov    dword ptr [esi + 0x15b], 6
        mov    edi, dword ptr [esi + 0x61]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x61
        je     loc_1_445F
        add    eax, 0
loc_1_4420:
        inc    dword ptr [edi + 0x14]
        lea    ebx, [edi + 0x14]
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_4434
        push   4
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        jmp    loc_1_443B
loc_1_4434:
        lea    eax, [edi + 0x10]
        push   0
        push   0
loc_1_443B:
        push   0x40010007
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_05A4
        mov    edi, dword ptr [edi]
        dec    dword ptr [ebx]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x61
        jne    loc_1_4420
loc_1_445F:
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   7
        push   3
        push   4
        push   0xaa000003
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x1c
        jmp    loc_1_4491
loc_1_4483:
        mov    byte ptr [esi + 0x177], 1
        mov    byte ptr [esi + 0x176], 0
loc_1_4491:
        mov    dword ptr [esi + 0x160], 4
        push   esi
        call   sub_1_0A50
        mov    byte ptr [esi + 0x170], 0
        mov    byte ptr [esi + 0x171], 1
        mov    byte ptr [esi + 0x15f], 0
        mov    byte ptr [esi + 0x175], 0
        mov    byte ptr [esi + 0x168], 0
        mov    byte ptr [esi + 0x169], 0
        mov    eax, dword ptr [esi + 0x16a]
        or     eax, eax
        je     loc_1_44E6
        dec    dword ptr [eax + 0x14]
        push   0xc0000001
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_0586
loc_1_44E6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  090h
sub_1_44F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x160]
        cmp    eax, 1
        je     loc_1_4530
        cmp    eax, 2
        je     loc_1_4628
        cmp    eax, 3
        je     loc_1_4768
        cmp    eax, 4
        je     loc_1_47D0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_4530:
        mov    eax, dword ptr [esi + 0xad]
        mov    al, byte ptr [eax + 0x1e02]
        mov    byte ptr [ebp - 5], al
        mov    eax, dword ptr [esi + 0xad]
        mov    al, byte ptr [eax + 0x1e03]
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 5]
        and    eax, 0xffff00ff
        shl    ax, 8
        mov    cl, byte ptr [ebp - 4]
        and    ecx, 0xffff00ff
        add    ax, cx
        mov    word ptr [esi + 0xb5], ax
        and    eax, 0xffff
        mov    ecx, dword ptr [esi + 0xb1]
        mov    ax, word ptr [eax + ecx + 6]
        mov    word ptr [ebp - 8], ax
        or     ax, ax
        je     loc_1_45A0
        push   1
        and    eax, 0xffff
        push   eax
        push   0
        push   esi
        call   sub_1_43F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_45A0:
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0xb5]
        add    ebx, dword ptr [esi + 0xb1]
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 0xf
        rep stosd dword ptr es:[edi], eax
        mov    byte ptr [ebx], 3
        mov    word ptr [ebx + 8], 1
        push   6
        lea    eax, [esi + 0x4d]
        push   eax
        lea    eax, [ebx + 0xa]
        push   eax
        call   sub_1_071C
        mov    ax, word ptr [esi + 0xe3]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 0x18], cx
        mov    ax, word ptr [esi + 0xe5]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 0x1a], cx
        mov    ax, word ptr [esi + 0xdd]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 0x1c], cx
        mov    al, byte ptr [esi + 0xdb]
        mov    byte ptr [ebx + 0x1e], al
        mov    eax, dword ptr [esi + 0xad]
        mov    dword ptr [esi + 0x160], 2
        mov    byte ptr [eax + 0x1e4b], 0x28
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4628:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0xb5]
        add    ecx, dword ptr [esi + 0xb1]
        mov    al, byte ptr [ecx + 2]
        mov    byte ptr [ebp - 3], al
        or     al, al
        je     loc_1_4683
        mov    cx, word ptr [ecx + 6]
        mov    word ptr [esi + 0x172], cx
        mov    ah, cl
        mov    al, ch
        mov    word ptr [esi + 0x172], ax
        and    eax, 0xffff
        or     eax, 0xc0011000
        mov    dword ptr [ebp - 0xc], eax
        push   2
        xor    eax, eax
        mov    ax, word ptr [esi + 0x172]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   esi
        call   sub_1_43F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4683:
        mov    ax, word ptr [ecx + 0xa]
        mov    word ptr [ebp - 8], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 7]
        and    eax, 0xffff
        add    eax, dword ptr [esi + 0xb1]
        mov    dword ptr [esi + 0xb7], eax
        mov    ax, word ptr [ecx + 0xe]
        mov    word ptr [ebp - 8], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 7]
        and    eax, 0xffff
        add    eax, dword ptr [esi + 0xb1]
        mov    dword ptr [esi + 0xbb], eax
        mov    ax, word ptr [ecx + 0xc]
        mov    word ptr [ebp - 8], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 7]
        and    eax, 0xffff
        add    eax, dword ptr [esi + 0xb1]
        mov    dword ptr [esi + 0xbf], eax
        mov    ax, word ptr [ecx + 8]
        mov    word ptr [ebp - 8], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 7]
        and    eax, 0xffff
        add    eax, dword ptr [esi + 0xb1]
        mov    dword ptr [esi + 0xc3], eax
        mov    dword ptr [esi + 0xa1], 0
        push   esi
        call   sub_1_4EF0
        or     eax, eax
        jne    loc_1_4748
        push   esi
        call   sub_1_4FA0
        or     eax, eax
        jne    loc_1_4728
        push   esi
        call   sub_1_1DE0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4728:
        mov    eax, dword ptr [esi + 0xad]
        mov    dword ptr [esi + 0x160], 4
        mov    byte ptr [eax + 0x1e4b], 0x28
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4748:
        mov    eax, dword ptr [esi + 0xad]
        mov    dword ptr [esi + 0x160], 3
        mov    byte ptr [eax + 0x1e4b], 0x28
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4768:
        mov    eax, dword ptr [esi + 0xb7]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 2], al
        or     al, al
        jne    loc_1_47B1
        push   esi
        call   sub_1_4FA0
        or     eax, eax
        jne    loc_1_4791
        push   esi
        call   sub_1_1DE0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4791:
        mov    eax, dword ptr [esi + 0xad]
        mov    dword ptr [esi + 0x160], 4
        mov    byte ptr [eax + 0x1e4b], 0x28
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_47B1:
        cmp    byte ptr [ebp - 2], 0xfe
        je     loc_1_4806
        push   3
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        push   eax
        push   0
        push   esi
        call   sub_1_43F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_47D0:
        mov    eax, dword ptr [esi + 0xb7]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 1], al
        or     al, al
        jne    loc_1_47F0
        push   esi
        call   sub_1_1DE0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_47F0:
        cmp    byte ptr [ebp - 1], 0xfe
        je     loc_1_4806
        push   4
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        push   eax
        push   0
        push   esi
        call   sub_1_43F0
loc_1_4806:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_4810:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [edi + 0xb7]
        mov    al, byte ptr [edx + 2]
        cmp    al, 0xfe
        mov    byte ptr [ebp - 1], al
        jne    loc_1_4840
        mov    ecx, dword ptr [ebp + 0xc]
        sub    eax, eax
        mov    byte ptr [ecx], 0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_4840:
        mov    eax, dword ptr [edi + 0xce]
        mov    dword ptr [edi + 0xce], 0
        or     eax, eax
        mov    ecx, dword ptr [ebp + 0xc]
        jne    loc_1_4864
        mov    byte ptr [ecx], 0
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_4864:
        mov    byte ptr [ecx], 1
        cmp    byte ptr [ebp - 1], 0xff
        je     loc_1_4895
        mov    esi, dword ptr [eax + 0x28]
        inc    dword ptr [edi + 0xff]
        push   0xc0000001
        mov    eax, dword ptr [eax + 0x2c]
        push   eax
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_057A
        dec    dword ptr [esi + 0x14]
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_4895:
        mov    byte ptr [eax + 0x30], 1
        mov    cl, byte ptr [edx + 1]
        mov    byte ptr [eax + 0x31], cl
        and    ecx, 0xff
        mov    dword ptr [edi + ecx*4 + 0x179], eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_48C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x89], 0
        je     loc_1_48F0
        push   esi
        call   sub_1_4B50
        cmp    eax, 0x103
        jne    loc_1_48F0
        mov    eax, dword ptr [esi + 0xad]
        pop    esi
        mov    byte ptr [eax + 0x1e4b], 0x20
        pop    ebp
        ret    4
        db  090h
loc_1_48F0:
        mov    eax, dword ptr [esi + 0xe7]
        or     eax, eax
        je     loc_1_4922
        mov    ecx, dword ptr [eax + 0x24]
        mov    dword ptr [esi + 0xe7], ecx
        mov    dword ptr [esi + 0xce], eax
        push   eax
        push   esi
        call   sub_1_4960
        mov    eax, dword ptr [esi + 0xad]
        pop    esi
        mov    byte ptr [eax + 0x1e4b], 0x20
        pop    ebp
        ret    4
loc_1_4922:
        mov    byte ptr [esi + 0xcb], 1
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_4930:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    ecx, dword ptr [eax + 0xb7]
        mov    byte ptr [eax + 0xcb], 0
        mov    byte ptr [ecx], 0
        mov    byte ptr [ecx + 2], 0xfe
        mov    eax, dword ptr [eax + 0xad]
        mov    byte ptr [eax + 0x1e4b], 0x20
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_4960:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0xb7]
        mov    byte ptr [eax], 0xa
        mov    byte ptr [eax + 1], 0
        mov    byte ptr [eax + 2], 0xfe
        mov    word ptr [eax + 4], 0
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4990:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        push   0
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        call   sub_1_0660
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc3]
        mov    byte ptr [eax], 0xa
        mov    cl, byte ptr [esi + 0x31]
        mov    byte ptr [eax + 1], cl
        mov    byte ptr [eax + 2], 0
        mov    cl, byte ptr [ebp - 3]
        mov    ch, byte ptr [ebp - 4]
        mov    word ptr [eax + 6], cx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_49E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0xb7]
        mov    byte ptr [eax], 0x1e
        mov    word ptr [eax + 4], 0
        mov    byte ptr [eax + 2], 0x80
        ret    4
        db  08Dh, 049h, 000h
sub_1_4A00:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0xb7]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 4], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 3]
        mov    edx, eax
        and    edx, 0xffff
        add    edx, dword ptr [ecx + 0xb7]
        mov    ax, word ptr [edx]
        mov    word ptr [ebp - 4], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 3]
        and    eax, 0xffff
        add    dword ptr [ecx + 0xf7], eax
        mov    ax, word ptr [edx + 2]
        mov    word ptr [ebp - 4], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 3]
        and    eax, 0xffff
        add    dword ptr [ecx + 0xfb], eax
        mov    al, byte ptr [edx + 4]
        and    eax, 0xff
        add    dword ptr [ecx + 0xff], eax
        mov    al, byte ptr [edx + 5]
        and    eax, 0xff
        add    dword ptr [ecx + 0x103], eax
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_4A80:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0xb7]
        mov    al, byte ptr [edx + 6]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x107], eax
        mov    al, byte ptr [edx + 7]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x10b], eax
        mov    al, byte ptr [edx + 8]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x10f], eax
        mov    al, byte ptr [edx + 9]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x113], eax
        mov    al, byte ptr [edx + 0xa]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x117], eax
        mov    al, byte ptr [edx + 0xc]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x11b], eax
        mov    al, byte ptr [edx + 0xd]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x11f], eax
        mov    al, byte ptr [edx + 0xe]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x123], eax
        mov    al, byte ptr [edx + 0xf]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x127], eax
        mov    al, byte ptr [edx + 0x10]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x12b], eax
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_4B30:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0xb7]
        mov    byte ptr [eax], 8
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4B50:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x89], 0
        je     loc_1_4D39
        mov    edi, dword ptr [ebp - 8]
        lea    esp, [esp]
loc_1_4B70:
        mov    ebx, dword ptr [esi + 0x89]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 0x89], eax
        or     eax, eax
        jne    loc_1_4B8C
        mov    dword ptr [esi + 0x8d], 0
loc_1_4B8C:
        mov    eax, dword ptr [ebx + 0x10]
        cmp    eax, 8
        jne    loc_1_4BC0
        cmp    dword ptr [ebx + 4], 0
        push   esi
        je     loc_1_4BA2
        call   sub_1_4B30
        jmp    loc_1_4BA7
loc_1_4BA2:
        call   sub_1_49E0
loc_1_4BA7:
        mov    eax, dword ptr [esi + 0xad]
        mov    edi, 0x103
        mov    byte ptr [eax + 0x1e4b], 0x20
        jmp    loc_1_4CA5
        db  08Bh, 0FFh
loc_1_4BC0:
        cmp    eax, 1
        je     loc_1_4C05
        cmp    eax, 2
        je     loc_1_4C48
        cmp    eax, 4
        je     loc_1_4C66
        cmp    eax, 9
        je     loc_1_4C83
        cmp    eax, 0xa
        je     loc_1_4C94
        push   eax
        push   1
        push   9
        push   3
        push   0xaa000006
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x18
        jmp    loc_1_4CA5
loc_1_4C05:
        mov    eax, dword ptr [ebx + 0x14]
        cmp    eax, 0x1010e
        jne    loc_1_4C30
        mov    eax, dword ptr [esi + 0x95]
        mov    dword ptr [esi + 0x99], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 0x95], eax
        push   esi
        call   sub_1_4EF0
        jmp    loc_1_4CA3
        db  08Dh, 064h, 024h, 000h
loc_1_4C30:
        cmp    eax, 0x2010103
        mov    eax, dword ptr [ebx + 8]
        jne    loc_1_4C97
        mov    dword ptr [esi + 0x9d], eax
        push   esi
        call   sub_1_4EF0
        jmp    loc_1_4CA3
loc_1_4C48:
        push   esi
        call   sub_1_4B30
        mov    byte ptr [ebx + 4], 1
        mov    eax, dword ptr [esi + 0xad]
        mov    edi, 0x103
        mov    byte ptr [eax + 0x1e4b], 0x20
        jmp    loc_1_4CA5
loc_1_4C66:
        mov    eax, dword ptr [esi + 0x95]
        mov    dword ptr [esi + 0x99], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 0x95], eax
        push   esi
        call   sub_1_4EF0
        jmp    loc_1_4CA3
loc_1_4C83:
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 0x9d], eax
        push   esi
        call   sub_1_4EF0
        jmp    loc_1_4CA3
loc_1_4C94:
        mov    eax, dword ptr [ebx + 8]
loc_1_4C97:
        mov    dword ptr [esi + 0xa5], eax
        push   esi
        call   sub_1_4FA0
loc_1_4CA3:
        mov    edi, eax
loc_1_4CA5:
        cmp    edi, 0x103
        je     loc_1_4D44
        or     edi, edi
        jne    loc_1_4D11
        mov    eax, dword ptr [ebx + 0x10]
        cmp    eax, 1
        je     loc_1_4CD6
        cmp    eax, 2
        je     loc_1_4CF0
        cmp    eax, 4
        je     loc_1_4D01
        cmp    eax, 9
        jl     loc_1_4CD1
        cmp    eax, 0xa
        jle    loc_1_4D09
loc_1_4CD1:
        push   eax
        push   9
        jmp    loc_1_4D14
loc_1_4CD6:
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp - 4], eax
        push   0
        push   ebx
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_058C
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_4D2C
loc_1_4CF0:
        push   0
        push   ebx
        mov    eax, dword ptr [esi + 0x7d]
        push   eax
        call   sub_1_05AA
        dec    dword ptr [esi + 0x5d]
        jmp    loc_1_4D2C
loc_1_4D01:
        mov    eax, dword ptr [ebx + 4]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_4D2C
loc_1_4D09:
        mov    eax, dword ptr [ebx + 4]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_4D2C
loc_1_4D11:
        push   edi
        push   0xa
loc_1_4D14:
        push   5
        push   3
        push   0xaa000006
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x18
loc_1_4D2C:
        cmp    dword ptr [esi + 0x89], 0
        jne    loc_1_4B70
loc_1_4D39:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4D44:
        mov    dword ptr [esi + 0x91], ebx
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4D60:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x91]
        mov    eax, dword ptr [edi + 0x10]
        cmp    eax, 1
        je     loc_1_4DA0
        cmp    eax, 2
        je     loc_1_4E01
        cmp    eax, 4
        je     loc_1_4E50
        cmp    eax, 9
        jl     loc_1_4E56
        cmp    eax, 0xa
        jle    loc_1_4E50
        jmp    loc_1_4E56
loc_1_4DA0:
        cmp    byte ptr [ebp + 0xc], 0
        je     loc_1_4DE0
        cmp    dword ptr [edi + 0x14], 0x2010104
        jne    loc_1_4DBB
        mov    eax, dword ptr [esi + 0xa5]
        mov    dword ptr [esi + 0xa9], eax
loc_1_4DBB:
        mov    dword ptr [esi + 0x91], 0
        mov    ebx, dword ptr [edi + 4]
        push   0
        push   edi
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_058C
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_4E56
        db  08Dh, 064h, 024h, 000h
loc_1_4DE0:
        mov    dword ptr [esi + 0x91], 0
        mov    ebx, dword ptr [edi + 4]
        push   0xc0000001
        push   edi
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_058C
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_4E56
loc_1_4E01:
        cmp    byte ptr [ebp + 0xc], 0
        je     loc_1_4E30
        push   esi
        call   sub_1_4A80
        push   edi
        push   esi
        call   sub_1_1860
        mov    dword ptr [esi + 0x91], 0
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 0x7d]
        push   eax
        call   sub_1_05AA
        dec    dword ptr [esi + 0x5d]
        jmp    loc_1_4E56
        db  08Bh, 0FFh
loc_1_4E30:
        mov    dword ptr [esi + 0x91], 0
        push   0xc0000001
        push   edi
        mov    eax, dword ptr [esi + 0x7d]
        push   eax
        call   sub_1_05AA
        dec    dword ptr [esi + 0x5d]
        jmp    loc_1_4E56
        db  08Bh, 0FFh
loc_1_4E50:
        mov    eax, dword ptr [edi + 4]
        dec    dword ptr [eax + 0x14]
loc_1_4E56:
        cmp    byte ptr [ebp + 0xc], 0
        jne    loc_1_4EDA
        mov    eax, dword ptr [edi + 0x10]
        cmp    eax, 1
        je     loc_1_4EA0
        cmp    eax, 2
        je     loc_1_4EDA
        cmp    eax, 4
        je     loc_1_4EDA
        cmp    eax, 9
        jl     loc_1_4E7C
        cmp    eax, 0xa
        jle    loc_1_4EDA
loc_1_4E7C:
        push   eax
        push   9
        push   6
        push   3
        push   0xaa000006
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x18
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_4EA0:
        cmp    dword ptr [edi + 0x14], 0x1010e
        jne    loc_1_4ED0
        mov    eax, dword ptr [esi + 0x99]
        mov    dword ptr [esi + 0x95], eax
        mov    dword ptr [esi + 0xa1], 0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_4ED0:
        mov    dword ptr [esi + 0x9d], 0
loc_1_4EDA:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4EF0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x95]
        test   ah, 0x20
        je     loc_1_4F10
        mov    esi, 0x7fffffff
        jmp    loc_1_4F20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_4F10:
        test   ah, 0x40
        mov    esi, 0
        je     loc_1_4F20
        mov    esi, dword ptr [edi + 0x9d]
loc_1_4F20:
        cmp    dword ptr [edi + 0xa1], esi
        jne    loc_1_4F30
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_4F30:
        push   esi
        push   edi
        call   sub_1_4F50
        mov    dword ptr [edi + 0xa1], esi
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4F50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xb7]
        mov    eax, ecx
        mov    byte ptr [edx], 7
        mov    byte ptr [edx + 2], 0xfe
        shr    eax, 0x18
        mov    byte ptr [ebp - 4], al
        mov    eax, ecx
        shr    eax, 0x10
        mov    byte ptr [ebp - 3], al
        mov    byte ptr [ebp - 2], ch
        mov    byte ptr [ebp - 1], cl
        sub    eax, eax
loc_1_4F81:
        mov    cl, byte ptr [ebp + eax - 4]
        mov    byte ptr [edx + eax + 6], cl
        inc    eax
        cmp    eax, 4
        jb     loc_1_4F81
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4FA0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0xa5]
        push   ecx
        push   eax
        call   sub_1_4FC0
        mov    eax, 0x103
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_4FC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xb7]
        mov    eax, ecx
        mov    byte ptr [edx], 6
        mov    byte ptr [edx + 2], 0xfe
        shr    eax, 0x18
        mov    byte ptr [ebp - 4], al
        mov    eax, ecx
        shr    eax, 0x10
        mov    byte ptr [ebp - 3], al
        mov    byte ptr [ebp - 2], ch
        mov    byte ptr [ebp - 1], cl
        sub    eax, eax
loc_1_4FF1:
        mov    cl, byte ptr [ebp + eax - 4]
        mov    byte ptr [edx + eax + 6], cl
        inc    eax
        cmp    eax, 4
        jb     loc_1_4FF1
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5010:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0xc3]
        mov    ax, word ptr [ebp + 0xc]
        mov    byte ptr [ecx], 0x81
        mov    byte ptr [ecx + 2], 0
        mov    word ptr [ecx + 4], 0
        mov    word ptr [ecx + 6], ax
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5040:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        lea    eax, [esi + 0x24]
        mov    ecx, dword ptr [edx + 0xef]
        or     ecx, ecx
        jne    loc_1_5060
        mov    dword ptr [edx + 0xef], esi
        jmp    loc_1_5063
        db  090h
loc_1_5060:
        mov    dword ptr [ecx + 0x24], esi
loc_1_5063:
        mov    dword ptr [edx + 0xf3], esi
        mov    dword ptr [eax], 0
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5080:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x5d]
        cmp    byte ptr [esi + 0x15f], 0
        je     loc_1_50BA
        cmp    dword ptr [esi + 0x160], 0
        jne    loc_1_50AD
        mov    dword ptr [esi + 0x160], 1
        push   esi
        call   sub_1_1CB0
loc_1_50AD:
        cmp    byte ptr [esi + 0x15f], 0
        jne    loc_1_51B4
loc_1_50BA:
        mov    edi, dword ptr [esi + 0x71]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0x71
        je     loc_1_51B4
        inc    dword ptr [edi + 0x14]
        push   0
        mov    eax, dword ptr [edi + 0xc]
        push   eax
        mov    eax, dword ptr [esi + 0x85]
        push   eax
        call   sub_2_01D0
        or     eax, eax
        jne    loc_1_5150
        cmp    dword ptr [edi + 0x14], 2
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        jne    loc_1_5137
        mov    eax, dword ptr [esi + 0x164]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_5109
        push   esi
        call   sub_1_0E20
loc_1_5109:
        mov    eax, dword ptr [esi + 0x61]
        sub    eax, esi
        cmp    eax, 0x61
        jne    loc_1_511A
        mov    dword ptr [esi + 0x18], 0xfe
loc_1_511A:
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_0574
        push   0
        push   0x96
        push   edi
        call   sub_1_063C
        jmp    loc_1_51B4
loc_1_5137:
        lea    eax, [esi + 0x69]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 0x6d]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi + 0x6d]
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 0x6d], edi
        sub    dword ptr [edi + 0x14], 2
        jmp    loc_1_51B4
loc_1_5150:
        cmp    eax, 0x103
        jne    loc_1_5199
        cmp    byte ptr [esi + 0xcb], 0
        je     loc_1_516D
        mov    byte ptr [esi + 0xcb], 0
        push   esi
        call   sub_1_48C0
loc_1_516D:
        mov    byte ptr [edi + 0x1d], 1
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [esi + 0x69]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 0x6d]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi + 0x6d]
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 0x6d], edi
        sub    dword ptr [edi + 0x14], 2
        jmp    loc_1_51B4
loc_1_5199:
        push   eax
        push   0xa
        push   7
        push   3
        push   0xaa000006
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x18
loc_1_51B4:
        mov    edi, dword ptr [esi + 0x69]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0x69
        je     loc_1_520F
        cmp    dword ptr [edi + 0x14], 0
        jne    loc_1_520F
        mov    eax, dword ptr [esi + 0x164]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_51D7
        push   esi
        call   sub_1_0E20
loc_1_51D7:
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_0574
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 0x61]
        sub    eax, esi
        cmp    eax, 0x61
        jne    loc_1_5202
        mov    dword ptr [esi + 0x18], 0xfe
loc_1_5202:
        push   0
        push   0x96
        push   edi
        call   sub_1_063C
loc_1_520F:
        dec    dword ptr [esi + 0x5d]
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_5220:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x89], 0
        je     loc_1_52A9
loc_1_5235:
        mov    ecx, dword ptr [esi + 0x89]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x89], eax
        or     eax, eax
        jne    loc_1_5251
        mov    dword ptr [esi + 0x8d], 0
loc_1_5251:
        mov    eax, dword ptr [ecx + 0x10]
        dec    eax
        cmp    eax, 9
        ja     loc_1_52A0
        jmp    dword ptr [eax*4 + 0x52b8]    ; -> obj1:0x52B8
loc_1_5261:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   ecx
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_058C
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_52A0
loc_1_5274:
        push   edi
        push   ecx
        mov    eax, dword ptr [esi + 0x7d]
        push   eax
        call   sub_1_05AA
        dec    dword ptr [esi + 0x5d]
        jmp    loc_1_52A0
loc_1_5284:
        push   0
        push   0x14
        push   ecx
        call   sub_1_063C
        mov    dword ptr [esi + 0x91], 0
        jmp    loc_1_52A0
loc_1_529A:
        mov    eax, dword ptr [ecx + 4]
        dec    dword ptr [eax + 0x14]
loc_1_52A0:
        cmp    dword ptr [esi + 0x89], 0
        jne    loc_1_5235
loc_1_52A9:
        push   edi
        push   esi
        call   sub_1_52E0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
        ; ---- dispatch table (10 entries) ----
        dd  loc_1_5261
        dd  loc_1_5274
        dd  loc_1_52A0
        dd  loc_1_529A
        dd  loc_1_52A0
        dd  loc_1_52A0
        dd  loc_1_52A0
        dd  loc_1_5284
        dd  loc_1_529A
        dd  loc_1_529A
sub_1_52E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0xce]
        or     ecx, ecx
        je     loc_1_5314
        mov    dword ptr [esi + 0xce], 0
        mov    edi, dword ptr [ecx + 0x28]
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   ecx
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_057A
        dec    dword ptr [edi + 0x14]
loc_1_5314:
        cmp    dword ptr [esi + 0xe7], 0
        je     loc_1_534F
        lea    ecx, [ecx]
loc_1_5320:
        mov    edx, dword ptr [esi + 0xe7]
        mov    ecx, dword ptr [edx + 0x24]
        lea    eax, [edx + 0x24]
        mov    dword ptr [esi + 0xe7], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   edx
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_057A
        dec    dword ptr [edi + 0x14]
        cmp    dword ptr [esi + 0xe7], 0
        jne    loc_1_5320
loc_1_534F:
        lea    edi, [esi + 0x179]
        mov    dword ptr [ebp - 4], 0x80
loc_1_535C:
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_1_538D
        mov    byte ptr [ecx + 0x30], 0
        xor    eax, eax
        mov    al, byte ptr [ecx + 0x31]
        mov    dword ptr [esi + eax*4 + 0x179], 0
        mov    ebx, dword ptr [ecx + 0x28]
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ecx + 0x2c]
        push   eax
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_057A
        dec    dword ptr [ebx + 0x14]
loc_1_538D:
        add    edi, 4
        dec    dword ptr [ebp - 4]
        jne    loc_1_535C
        mov    dword ptr [esi + 0xef], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_53B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x2c], 0
        je     loc_1_5421
        cmp    dword ptr [esi + 0xce], 0
        jne    loc_1_53D8
        cmp    dword ptr [esi + 0xe7], 0
        jne    loc_1_53D8
        cmp    dword ptr [esi + 0x89], 0
        je     loc_1_5421
loc_1_53D8:
        mov    byte ptr [esi + 0x171], 1
        mov    byte ptr [esi + 0x2c], 0
        push   0
        push   esi
        call   sub_1_5220
        mov    al, byte ptr [esi + 0x2d]
        cmp    al, 0xa
        jae    loc_1_541B
        inc    al
        mov    byte ptr [esi + 0x2d], al
        push   0
        push   0xaa000003
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0xc
        push   0
        push   esi
        call   sub_1_1EF0
        push   esi
        call   sub_1_5080
loc_1_541B:
        mov    byte ptr [esi + 0x2c], 0
        jmp    loc_1_5440
loc_1_5421:
        cmp    dword ptr [esi + 0xce], 0
        jne    loc_1_543C
        cmp    dword ptr [esi + 0xe7], 0
        jne    loc_1_543C
        cmp    dword ptr [esi + 0x89], 0
        je     loc_1_5440
loc_1_543C:
        mov    byte ptr [esi + 0x2c], 1
loc_1_5440:
        cmp    byte ptr [esi + 0x177], 0
        je     loc_1_5467
        cmp    byte ptr [esi + 0x176], 0
        jne    loc_1_5467
        mov    byte ptr [esi + 0x176], 1
        push   0
        push   esi
        call   sub_1_1EF0
        push   esi
        call   sub_1_5080
loc_1_5467:
        push   0x7530
        add    esi, 0x20
        push   esi
        call   sub_1_0612
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_5480:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 0x6000101
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0x10000101
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 0x1000201
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
        add    eax, 0x6020101
        add    dword ptr [ecx], eax
        add    al, byte ptr [edi]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    al, byte ptr [edx]
        add    dword ptr [edx], eax
        db  002h
loc_1_5504:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 0x6000101
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0x10000101
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
loc_1_5560:
        dec    edi
        inc    ebx
        dec    ecx
        push   eax
loc_1_5564:
        xor    byte ptr [ecx], dh
        xor    dword ptr [esi], esi
loc_1_5568:
        and    byte ptr [eax], dh
        cmp    dword ptr [ecx], edi
        push   ebx
        push   edx
        inc    ecx
        dec    ebp
        xor    al, 0x58
        xor    esi, dword ptr [esi]
        and    byte ptr [eax], bh
        db  031h, 035h
loc_1_5578:
        dec    edi
        inc    ebx
        dec    ecx
        push   eax
loc_1_557C:
        xor    byte ptr [ecx], dh
        xor    dword ptr [esi], esi
loc_1_5580:
        and    byte ptr [eax], dh
        cmp    dword ptr [ecx], edi
loc_1_5584:
        push   ebx
        push   edx
        inc    ecx
        dec    ebp
loc_1_5588:
        xor    al, 0x58
        xor    esi, dword ptr [esi]
loc_1_558C:
        and    byte ptr [eax], bh
        xor    dword ptr [0x546d6249], esi
        outsd  dx, dword ptr [esi]
        imul   eax, dword ptr [eax], 0
loc_1_5598:
        dec    ecx
        bound  ebp, qword ptr [ebp + 0x54]
        outsd  dx, dword ptr [esi]
        imul   eax, dword ptr [eax], 0
loc_1_55A0:
        dec    ecx
        bound  ebp, qword ptr [ebp + 0x20]
        push   esp
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebp + 0x6e], 0x20
        push   edx
        imul   ebp, dword ptr [esi + 0x67], 0x74654e20
        ja     loc_1_5623
        jb     0x5621
        and    byte ptr [ebx + 0x61], al
        jb     0x561f
        and    byte ptr [esi + 0x6f], ah
        jb     loc_1_55E0
        push   eax
        inc    ebx
        and    byte ptr [ecx + 0x2f], cl
        dec    edi
        and    byte ptr [edx + 0x75], ah
        jae    loc_1_55F9
        add    byte ptr [ecx + 0x62], cl
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + ebp*2 + 0x6b], dl
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [edx + 0x69], dl
        outsb  dx, byte ptr [esi]
        and    byte ptr [bp + 0x65], cl
        je     0x5656
        outsd  dx, dword ptr [esi]
loc_1_55E0:
        jb     loc_1_564D
        and    byte ptr [ebx + 0x61], al
        jb     loc_1_564B
        and    byte ptr [esi + 0x6f], ah
        jb     loc_1_560C
        dec    ebp
        inc    ebx
        inc    ecx
        and    byte ptr [edx + 0x75], ah
        jae    loc_1_5622
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ecx
loc_1_55F9:
        popal  
        jo     loc_1_5671
        jb     loc_1_5654
        jns    loc_1_5672
        add    byte ptr gs:[ecx + 0x64], al
        popal  
        jo     loc_1_567D
        jb     loc_1_5660
loc_1_560C:
        jns    loc_1_567E
        add    byte ptr gs:[edx + 0x75], al
        jae    0x5668
        jns    loc_1_5686
        add    byte ptr gs:[edx + 0x75], al
        jae    loc_1_5670
        jns    0x568e
        add    byte ptr gs:[ecx + 0x4f], cl
loc_1_5622:
        inc    edx
loc_1_5623:
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
loc_1_5628:
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    ebp
        inc    ecx
        pop    eax
loc_1_564B:
        push   eax
        inc    ecx
loc_1_564D:
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        push   ebx
        dec    ecx
        pop    edx
loc_1_5654:
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x41], cl
        pop    eax
        push   eax
        inc    ecx
        inc    ebx
        dec    ebx
        inc    ebp
loc_1_5660:
        push   esp
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x4d], cl
        dec    ecx
        dec    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5670:
        dec    ebp
loc_1_5671:
        dec    ebp
loc_1_5672:
        dec    ecx
        dec    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   edx
        dec    ecx
        dec    esi
        inc    edi
        push   ebx
loc_1_567D:
        push   eax
loc_1_567E:
        inc    ebp
        inc    ebp
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x49], dl
loc_1_5686:
        dec    esi
        inc    edi
        push   ebx
        push   eax
        inc    ebp
        inc    ebp
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x41], dl
        dec    ebp
        add    byte ptr [edx + 0x41], dl
        dec    ebp
        add    byte ptr [edx + 0x41], dl
        dec    ebp
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [edx + 0x41], dl
        dec    ebp
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [ecx + 0x4e], cl
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x4e], cl
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56C5:
        add    byte ptr [eax], al
loc_1_56C7:
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x570  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        mov    eax, dword ptr [0x56cc]       ; -> obj1:0x56CC
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   0x32
        push   eax
        call   sub_1_0636
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_0030
        sub    al, al
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  090h
loc_2_0030:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x18]
        mov    dword ptr [eax + 0x2e], 0xffffffff
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx + 0x18], al
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [ecx + 0x19], al
        add    ecx, 0x18
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ecx + 2], al
        mov    al, byte ptr [edx + 3]
        mov    byte ptr [ecx + 3], al
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1e], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x22], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x26], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 0x2a], eax
        mov    al, 1
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_00D0:
        push   ebp
        mov    ebp, esp
        push   0
        push   0x32
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_063C
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_00F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x2e], 0
        jne    loc_2_0109
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0109:
        mov    eax, dword ptr [0x56cc]       ; -> obj1:0x56CC
        push   eax
        push   0
        push   0x1f
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0636
        or     eax, eax
        je     loc_2_0129
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0129:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x2e]
        mov    edi, edi
loc_2_0130:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_0143
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_0130
        jmp    loc_2_0152
loc_2_0143:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x2e], eax
loc_2_0152:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_2_0168
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_0168:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 1
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 0x1c], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1d], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1e], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  090h
sub_2_01D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_04F0
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_01F9
        cmp    ebx, 0x103
        jne    loc_2_0211
loc_2_01F9:
        push   0
        push   0x56c4                        ; -> obj1:0x56C4
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_02C0
        or     eax, eax
        je     loc_2_0211
        mov    ebx, eax
loc_2_0211:
        or     ebx, ebx
        je     loc_2_021D
        cmp    ebx, 0x103
        jne    loc_2_0248
loc_2_021D:
        cmp    byte ptr [edi + 0x1d], 0
        je     loc_2_0248
        dec    dword ptr [esi + 0xc]
        mov    byte ptr [edi + 0x1d], 0
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_2_0248
        push   0
        push   0x56c4                        ; -> obj1:0x56C4
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_0360
        or     eax, eax
        je     loc_2_0248
        mov    ebx, eax
loc_2_0248:
        or     ebx, ebx
        je     loc_2_0254
        cmp    ebx, 0x103
        jne    loc_2_02B5
loc_2_0254:
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_2_02B0
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_026A
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_026A:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_0279
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_027F
loc_2_0279:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_2_027F:
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_2_028E
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0598
loc_2_028E:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x1c]
        shl    eax, cl
        or     dword ptr [esi + 0x2e], eax
        push   0
        push   0x1f
        push   edi
        call   sub_1_063C
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_2_02B0:
        mov    ebx, 0xc001000e
loc_2_02B5:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_2_02C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        xor    edx, edx
        mov    esi, dword ptr [edi + 4]
        mov    ecx, dword ptr [ebx + 0xc]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 2]
        mov    dl, byte ptr [eax]
        shl    edx, 0x10
        or     ecx, edx
        shl    ecx, 8
        xor    edx, edx
        mov    dl, byte ptr [eax + 1]
        shl    edx, 0x10
        or     ecx, edx
        mov    al, byte ptr [eax + 3]
        and    eax, 0xff
        or     ecx, eax
        mov    eax, ecx
        mov    dword ptr [ebx + 0xc], eax
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [edi + 4], 0
        or     eax, eax
        je     loc_2_0320
loc_2_0313:
        mov    ecx, dword ptr [eax + 0xc]
        or     dword ptr [edi + 4], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_0313
loc_2_0320:
        mov    ecx, dword ptr [edi + 4]
        cmp    esi, ecx
        je     loc_2_0354
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   esi
        call   dword ptr [edi + 0x1e]
        or     eax, eax
        je     loc_2_0356
        cmp    eax, 0x103
        je     loc_2_0356
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0xc], ecx
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0354:
        sub    eax, eax
loc_2_0356:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  090h
sub_2_0360:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [ebp + 0x14]
        xor    edx, edx
        mov    dl, byte ptr [ecx]
        mov    al, byte ptr [ecx + 2]
        shl    edx, 0x10
        or     edx, eax
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [ecx + 1]
        shl    eax, 0x10
        or     edx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 3]
        or     edx, eax
        mov    dword ptr [ebp - 4], edx
        mov    al, byte ptr [0x56c4]         ; -> obj1:0x56C4
        cmp    byte ptr [ecx], al
        jne    loc_2_0408
        mov    al, byte ptr [0x56c5]         ; -> loc_1_56C5
        cmp    byte ptr [ecx + 1], al
        jne    loc_2_0408
        mov    al, byte ptr [0x56c6]         ; -> obj1:0x56C6
        cmp    byte ptr [ecx + 2], al
        jne    loc_2_0408
        mov    al, byte ptr [0x56c7]         ; -> loc_1_56C7
        cmp    byte ptr [ecx + 3], al
        jne    loc_2_0408
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_03E9
        dec    dword ptr [esi + 0xc]
        mov    byte ptr [ebx + 0x1d], 0
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_0483
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_03E9:
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_03FD
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_03FD:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0408:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 8], eax
        jne    loc_2_0440
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_0424
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0424:
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_0483
        mov    byte ptr [ebx + 0x1d], 1
        inc    dword ptr [esi + 0xc]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
loc_2_0440:
        mov    eax, dword ptr [esi + 0xc]
        cmp    eax, 1
        jbe    loc_2_0456
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0456:
        mov    ebx, dword ptr [ebp + 0xc]
        jne    loc_2_0470
        cmp    byte ptr [ebx + 0x1d], 0
        jne    loc_2_0470
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  090h
loc_2_0470:
        cmp    eax, 1
        jne    loc_2_0483
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_0483
        dec    eax
        mov    dword ptr [esi + 0xc], eax
        mov    byte ptr [ebx + 0x1d], 0
loc_2_0483:
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 8], ecx
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   edi
        call   dword ptr [esi + 0x22]
        or     eax, eax
        je     loc_2_04B6
        cmp    eax, 0x103
        je     loc_2_04B6
        mov    dword ptr [esi + 8], edi
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0xc], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_04B6:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_04D0
        mov    byte ptr [ebx + 0x1d], 0
        mov    dword ptr [esi + 0xc], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_04D0:
        mov    byte ptr [ebx + 0x1d], 1
        mov    dword ptr [esi + 0xc], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_04F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [esi + 0x10]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        mov    dword ptr [esi + 0x10], 0
        je     loc_2_052D
        lea    esp, [esp]
loc_2_0520:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0x10], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_0520
loc_2_052D:
        mov    ecx, dword ptr [esi + 0x10]
        cmp    ebx, ecx
        je     loc_2_0561
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 0x26]
        or     eax, eax
        je     loc_2_0563
        cmp    eax, 0x103
        je     loc_2_0563
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0x10], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0561:
        sub    eax, eax
loc_2_0563:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x16  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 008h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 001h, 000h, 0C3h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x240  R-X
loc_4_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    ebx, ebx
        mov    dword ptr [eax], ebx
        sub    edx, edx
        mov    ecx, dword ptr [ebp + 0x18]
        cmp    edx, ecx
        je     loc_4_0031
        mov    eax, dword ptr [ebp + 0x14]
        lea    esp, [esp]
loc_4_0020:
        cmp    dword ptr [eax], 1
        je     loc_4_002D
        add    eax, 4
        inc    edx
        cmp    edx, ecx
        jb     loc_4_0020
loc_4_002D:
        cmp    edx, ecx
        jne    loc_4_0040
loc_4_0031:
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  090h
loc_4_0040:
        mov    eax, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], edx
        inc    dword ptr [esi + 0x5d]
        mov    eax, dword ptr [0x56c8]       ; -> obj1:0x56C8
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   0x96
        push   eax
        call   sub_1_0636
        or     eax, eax
        jne    loc_4_0213
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, ecx
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1d], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1c], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0x85]
        push   eax
        call   sub_2_00F0
        or     al, al
        jne    loc_4_0100
        push   4
        push   1
        push   2
        push   0xaa000002
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x14
        push   0
        push   0x96
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_063C
        mov    ebx, 0xc0000001
        jmp    loc_4_01E6
        db  08Bh, 0FFh
loc_4_0100:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 0xf0
        mov    dword ptr [esi + 0x164], 0xf0
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [esi + 0x61]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0x65]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [esi + 0x65]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x65], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x14]
        cmp    byte ptr [esi + 0x16f], 0
        je     loc_4_01E6
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, 0x103
        mov    byte ptr [eax + 0x1c], 1
        cmp    byte ptr [esi + 0x170], 0
        jne    loc_4_01E6
        mov    byte ptr [esi + 0x170], 1
        mov    dword ptr [esi + 0x15b], 3
        xor    edx, edx
        mov    dx, word ptr [esi + 0xb5]
        add    edx, dword ptr [esi + 0xb1]
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0xf
        rep stosd dword ptr es:[edi], eax
        mov    byte ptr [edx], 3
        mov    word ptr [edx + 8], 1
        sub    ecx, ecx
loc_4_0195:
        mov    al, byte ptr [esi + ecx + 0x4d]
        mov    byte ptr [edx + ecx + 0xa], al
        inc    ecx
        cmp    ecx, 6
        jb     loc_4_0195
        mov    ax, word ptr [esi + 0xe3]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [edx + 0x18], cx
        mov    ax, word ptr [esi + 0xe5]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [edx + 0x1a], cx
        mov    ax, word ptr [esi + 0xdd]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [edx + 0x1c], cx
        mov    al, byte ptr [esi + 0xdb]
        mov    byte ptr [edx + 0x1e], al
        mov    eax, dword ptr [esi + 0xad]
        mov    byte ptr [eax + 0x1e4b], 0x20
loc_4_01E6:
        mov    eax, dword ptr [esi + 0x5d]
        dec    eax
        mov    dword ptr [esi + 0x5d], eax
        jne    loc_4_0208
        cmp    byte ptr [esi + 0x15f], 0
        jne    loc_4_0202
        mov    eax, dword ptr [esi + 0x69]
        sub    eax, esi
        cmp    eax, 0x69
        je     loc_4_0208
loc_4_0202:
        push   esi
        call   sub_1_5080
loc_4_0208:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_0213:
        push   5
        push   1
        push   2
        push   0xaa000002
        mov    eax, dword ptr [esi + 0x81]
        push   eax
        call   sub_1_0708
        add    esp, 0x14
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  005h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x110  R-X
sub_5_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x58
        mov    word ptr [ebp - 0x14], 7
        mov    word ptr [ebp - 0x12], 7
        mov    dword ptr [ebp - 0x10], 0x5598 ; -> loc_1_5598
        mov    word ptr [ebp - 0x20], 0
        mov    word ptr [ebp - 0x1e], 2
        mov    word ptr [ebp - 0x1c], 0xe000
        mov    word ptr [ebp - 0x1a], 0xe001
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_054A
        mov    eax, dword ptr [0x56c8]       ; -> obj1:0x56C8
        push   eax
        push   0
        push   8
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_0636
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     loc_5_0069
        mov    eax, 0xc000009a
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0069:
        mov    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 0x58], 3
        mov    byte ptr [ebp - 0x57], 0
        mov    dword ptr [ebp - 0x52], 0     ; -> loc_4_0000
        mov    dword ptr [ebp - 0x4e], 0xe60 ; -> loc_1_0E60
        mov    dword ptr [ebp - 0x3e], 0x10a0 ; -> loc_1_10A0
        mov    dword ptr [ebp - 0x4a], 0x2ac0 ; -> loc_1_2AC0
        mov    dword ptr [ebp - 0x46], 0x2c10 ; -> loc_1_2C10
        mov    dword ptr [ebp - 0x42], 0x1ab0 ; -> loc_1_1AB0
        mov    dword ptr [ebp - 0x36], 0x2450 ; -> loc_1_2450
        mov    dword ptr [ebp - 0x3a], 0x19c0 ; -> loc_1_19C0
        mov    dword ptr [ebp - 0x32], 0x1f60 ; -> loc_1_1F60
        mov    dword ptr [ebp - 0x2e], 0x2380 ; -> loc_1_2380
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x2a], eax
        lea    eax, [ebp - 0x58]
        mov    dword ptr [ebp - 0x26], ecx
        push   0x36
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0556
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_0105
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0550
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0105:
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 032h
VxD__LTEXT ENDS
