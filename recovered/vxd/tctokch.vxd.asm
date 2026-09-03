; TCTOK -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 7822 instructions recovered by recursive descent

Declare_Virtual_Device TCTOK, 3, 10, TCTOK_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x454c  R-X
loc_1_0000:
        mov    al, byte ptr [0x798]          ; -> obj1:0x0798
        cmp    al, 0
        jne    loc_1_0025
        inc    byte ptr [0x798]              ; -> obj1:0x0798
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0027
        push   0
        push   0
        call   sub_4_07A0
        cmp    eax, 0
        jne    loc_1_0027
loc_1_0025:
        clc    
        ret    
loc_1_0027:
        stc    
        ret    
TCTOK_Control:
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
        lea    eax, [esi + 0x18]
        mov    edx, dword ptr [ecx + 2]
        cmp    dword ptr [eax + 2], edx
        jne    loc_1_01C7
        mov    ax, word ptr [eax]
        cmp    word ptr [ecx], ax
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
        call   sub_1_060C
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
        call   sub_1_060C
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
        mov    al, byte ptr [edx]
        and    al, byte ptr [edx + 2]
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
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
sub_1_05C2:
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
sub_1_05C8:
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
        VxDCall  VPOWERD.Service_804Fh           ; cd204f802800
        VxDCall  VPOWERD.Service_8056h           ; cd2056802800
        add    byte ptr [eax], al
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_05E2:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_05E8:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_05EE:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_05F4:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_05FA:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_0600:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_0606:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_060C:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_0630:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_0654:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
sub_1_065A:
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_0678:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_0684:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_068A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_0690:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
        VxDCall  VPOWERD.Service_8042h           ; cd2042802800
sub_1_069C:
        VxDCall  VPOWERD.Service_8012h           ; cd2012802800
sub_1_06A2:
        VxDCall  VPOWERD.Service_8013h           ; cd2013802800
        VxDCall  VPOWERD.Service_8014h           ; cd2014802800
sub_1_06AE:
        VxDCall  VPOWERD.Service_8015h           ; cd2015802800
        VxDCall  VPOWERD.Service_804Ah           ; cd204a802800
        VxDCall  VPOWERD.Service_804Bh           ; cd204b802800
        VxDCall  VPOWERD.Service_804Ch           ; cd204c802800
        VxDCall  VPOWERD.Service_804Dh           ; cd204d802800
        VxDCall  VPOWERD.Service_804Eh           ; cd204e802800
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_06D8:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_06EC:
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
        db  'TCTOK   '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  TCTOK_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_07A0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0x10]
        or     edx, edx
        je     loc_1_07DD
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x127], 0
        je     loc_1_07C6
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   edx
        call   sub_1_060C
        pop    ebp
        ret    0x10
loc_1_07C6:
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   edx
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ecx + 0xf8]
        push   eax
        call   sub_1_05C8
loc_1_07DD:
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_07F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x167]
        or     eax, eax
        je     loc_1_0811
        mov    ecx, dword ptr [esi + 0x16b]
        push   ecx
        push   eax
        push   6
        push   esi
        call   sub_1_07A0
loc_1_0811:
        mov    eax, dword ptr [esi + 0x173]
        or     eax, eax
        je     loc_1_082B
        mov    ecx, dword ptr [esi + 0x177]
        push   ecx
        push   eax
        push   8
        push   esi
        call   sub_1_07A0
loc_1_082B:
        mov    eax, dword ptr [esi + 0x183]
        or     eax, eax
        je     loc_1_0845
        mov    ecx, dword ptr [esi + 0x187]
        push   ecx
        push   eax
        push   0x12
        push   esi
        call   sub_1_07A0
loc_1_0845:
        mov    eax, dword ptr [esi + 0x18b]
        or     eax, eax
        je     loc_1_085F
        mov    ecx, dword ptr [esi + 0x18f]
        push   ecx
        push   eax
        push   0xe
        push   esi
        call   sub_1_07A0
loc_1_085F:
        mov    eax, dword ptr [esi + 0x193]
        or     eax, eax
        je     loc_1_087E
        mov    ecx, dword ptr [esi + 0x197]
        push   ecx
        push   eax
        mov    eax, dword ptr [esi + 0x19f]
        push   eax
        push   esi
        call   sub_1_07A0
loc_1_087E:
        mov    eax, dword ptr [esi + 0x7a]
        or     eax, eax
        je     loc_1_089E
        mov    ecx, dword ptr [esi + 0x7e]
        push   ecx
        push   eax
        mov    eax, dword ptr [esi + 0x76]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    eax, ecx
        push   eax
        push   esi
        call   sub_1_07A0
loc_1_089E:
        mov    eax, dword ptr [esi + 0x6a]
        or     eax, eax
        je     loc_1_08B7
        mov    ecx, dword ptr [esi + 0x76]
        shl    ecx, 2
        push   0
        lea    ecx, [ecx + ecx*8]
        push   ecx
        push   eax
        call   sub_1_060C
loc_1_08B7:
        mov    eax, dword ptr [esi + 0x8a]
        or     eax, eax
        je     loc_1_08E1
        mov    ecx, dword ptr [esi + 0x8e]
        push   ecx
        push   eax
        mov    eax, dword ptr [esi + 0x82]
        imul   eax, dword ptr [esi + 0x86]
        imul   eax, dword ptr [esi + 0x76]
        push   eax
        push   esi
        call   sub_1_07A0
loc_1_08E1:
        mov    eax, dword ptr [esi + 0x5e]
        or     eax, eax
        je     loc_1_0901
        mov    ecx, dword ptr [esi + 0x62]
        push   ecx
        push   eax
        mov    eax, dword ptr [esi + 0x56]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    eax, ecx
        push   eax
        push   esi
        call   sub_1_07A0
loc_1_0901:
        mov    eax, dword ptr [esi + 0x42]
        or     eax, eax
        je     loc_1_091E
        mov    ecx, dword ptr [esi + 0x56]
        mov    edx, ecx
        lea    ecx, [ecx + ecx*4]
        push   0
        lea    ecx, [ecx + ecx*8]
        sub    ecx, edx
        push   ecx
        push   eax
        call   sub_1_060C
loc_1_091E:
        mov    eax, dword ptr [esi + 0x1a]
        or     eax, eax
        je     loc_1_0932
        push   0
        push   0x8c
        push   eax
        call   sub_1_060C
loc_1_0932:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0940:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    eax, eax
        mov    edi, dword ptr [esi + 0x17b]
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [esi + 0x23d]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        push   esi
        call   sub_1_3B30
        mov    ecx, dword ptr [esi + 0x17b]
        mov    word ptr [esi + 0x102], ax
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [esi + 0x23d]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 0x108], eax
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x17b]
        mov    word ptr [eax + 8], cx
        mov    ax, word ptr [esi + 0x108]
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x17b]
        mov    word ptr [eax + 0xa], cx
        mov    eax, dword ptr [esi + 0x23d]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 0x104], eax
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x17b]
        mov    word ptr [eax + 0xc], cx
        mov    ax, word ptr [esi + 0x104]
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x17b]
        mov    word ptr [eax + 0xe], cx
        mov    eax, dword ptr [esi + 0x17b]
        mov    word ptr [eax + 0x10], 0
        mov    eax, dword ptr [esi + 0x17b]
        mov    word ptr [eax + 0x12], 0
        mov    eax, dword ptr [esi + 0x17b]
        mov    byte ptr [eax + 0x1a], 6
        mov    eax, dword ptr [esi + 0x17b]
        mov    byte ptr [eax + 0x1b], 6
        cmp    byte ptr [esi + 0xea], 1
        jne    loc_1_0A87
        mov    eax, dword ptr [esi + 0x17b]
        mov    word ptr [eax + 0x14], 4
        mov    eax, dword ptr [esi + 0x17b]
        mov    word ptr [eax + 0x16], 0x610
        mov    eax, dword ptr [esi + 0x17b]
        mov    word ptr [eax + 0x18], 0xfebf
        jmp    loc_1_0AC6
loc_1_0A87:
        mov    ax, word ptr [esi + 0x100]
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x17b]
        mov    word ptr [eax + 0x14], cx
        mov    eax, dword ptr [esi + 0x17b]
        mov    word ptr [eax + 0x16], 0
        mov    eax, dword ptr [esi + 0x17b]
        mov    word ptr [eax + 0x18], 0
loc_1_0AC6:
        mov    eax, dword ptr [esi + 0x17b]
        add    eax, 2
        mov    dl, byte ptr [esi + 0x161]
        lea    ecx, [esi + 0x161]
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
        cmp    byte ptr [esi + 0x127], 0
        je     loc_1_0B36
        mov    eax, dword ptr [esi + 0x183]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x17b]
        mov    word ptr [eax + 0x1c], cx
        mov    ax, word ptr [esi + 0x183]
        jmp    loc_1_0B66
loc_1_0B36:
        mov    eax, dword ptr [esi + 0x187]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x17b]
        mov    word ptr [eax + 0x1c], cx
        mov    ax, word ptr [esi + 0x187]
loc_1_0B66:
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x17b]
        mov    word ptr [eax + 0x1e], cx
        mov    ecx, dword ptr [esi + 0x167]
        mov    ax, word ptr [ecx]
        mov    word ptr [ebp - 2], ax
        sub    eax, eax
loc_1_0B95:
        cmp    word ptr [ebp - 2], ax
        jne    loc_1_0B95
        mov    word ptr [ecx], 0x300
        cmp    byte ptr [esi + 0x127], 0
        je     loc_1_0BE0
        mov    eax, dword ptr [esi + 0x17b]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x167]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 0x17b]
        jmp    loc_1_0C10
        db  005h, 000h, 000h, 000h, 000h
loc_1_0BE0:
        mov    eax, dword ptr [esi + 0x17f]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x167]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 0x17f]
loc_1_0C10:
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x167]
        push   esi
        mov    word ptr [eax + 4], cx
        push   0x1900                        ; -> loc_1_1900
        push   esi
        call   sub_1_05FA
        lea    eax, [esi + 0xa3]
        push   0x8ca0
        mov    dword ptr [eax], 0
        mov    dword ptr [esi + 0xa7], 0x2980 ; -> loc_1_2980
        mov    dword ptr [esi + 0xab], esi
        push   eax
        call   sub_1_05E2
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0C70:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    cl, byte ptr [eax]
        mov    dl, byte ptr [eax + 1]
        mov    byte ptr [eax], dl
        mov    byte ptr [eax + 1], cl
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0C90:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0xea], 1
        mov    edx, dword ptr [ecx + 0x11f]
        je     loc_1_0CE0
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    ax, word ptr [ebp - 2]
        or     ax, 0x80
        mov    word ptr [ebp - 6], ax
        mov    cx, word ptr [ecx + 0x11f]
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, byte ptr [ebp - 5]
        jmp    loc_1_0D13
        db  08Dh, 049h, 000h
loc_1_0CE0:
        add    dx, 6
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    ax, word ptr [ebp - 2]
        or     ax, 0x4000
        mov    word ptr [ebp - 4], ax
        mov    cx, word ptr [ecx + 0x11f]
        lea    edx, [ecx + 6]
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, byte ptr [ebp - 3]
loc_1_0D13:
        out    dx, al
        push   0xfa
        call   sub_1_0600
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0D30:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0xea], 1
        je     loc_1_0DB1
        push   0x3e8
        call   sub_1_0600
        mov    ecx, dword ptr [esi + 0x11f]
        add    cx, 8
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 3], al
        cmp    byte ptr [esi + 0x127], 0
        je     loc_1_0D7B
        or     word ptr [ebp - 4], 0x100
loc_1_0D7B:
        or     word ptr [ebp - 4], 0x40
        and    word ptr [ebp - 4], 0xff7f
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ebp - 8], ax
        mov    cx, word ptr [esi + 0x11f]
        push   0x3e8
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, byte ptr [ebp - 7]
        out    dx, al
        call   sub_1_0600
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0DB1:
        mov    word ptr [ebp - 6], 0xff00
        mov    cx, word ptr [esi + 0x11f]
        mov    al, byte ptr [ebp - 6]
        lea    edx, [ecx + 6]
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        push   0x7d0
        call   sub_1_0600
        mov    ecx, dword ptr [esi + 0x11f]
        add    cx, 9
        mov    dx, cx
        in     al, dx
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 9
        or     al, 0x28
        out    dx, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0E00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        sub    cx, cx
        mov    eax, dword ptr [edx + 0x6a]
        mov    dword ptr [edx + 0x6e], eax
        cmp    dword ptr [edx + 0x76], 0
        jbe    loc_1_0E44
        mov    esi, 0x88
        mov    edi, 0xfeff
loc_1_0E22:
        mov    ebx, dword ptr [eax + 4]
        inc    cx
        mov    word ptr [ebx + 4], si
        mov    ebx, dword ptr [eax + 4]
        and    word ptr [ebx + 2], di
        mov    dword ptr [edx + 0x72], eax
        mov    ebx, ecx
        mov    eax, dword ptr [eax]
        and    ebx, 0xffff
        cmp    ebx, dword ptr [edx + 0x76]
        jb     loc_1_0E22
loc_1_0E44:
        mov    eax, dword ptr [edx + 0x72]
        pop    edi
        mov    eax, dword ptr [eax + 4]
        pop    esi
        or     word ptr [eax + 2], 0x100
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0E60:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x3c
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 0x1c], 0
        mov    byte ptr [ebp - 0x1b], 0
        mov    byte ptr [ebp - 0x1a], 0xc1
        mov    byte ptr [ebp - 0x19], 0xe2
        mov    byte ptr [ebp - 0x18], 0xd4
        mov    byte ptr [ebp - 0x17], 0x8b
        mov    byte ptr [ebp - 0x24], 0xff
        mov    byte ptr [ebp - 0x23], 0xff
        mov    byte ptr [ebp - 0x22], 0xd1
        mov    byte ptr [ebp - 0x21], 0xd7
        mov    edi, 0x40
        mov    byte ptr [ebp - 0x20], 0xc5
        mov    byte ptr [ebp - 0x1f], 0xd9
        mov    byte ptr [ebp - 0x1e], 0xc3
        mov    byte ptr [ebp - 0x1d], 0xd4
        mov    word ptr [ebp - 0xa], 0
        mov    esi, dword ptr [ebp + 8]
        nop    
loc_1_0EB0:
        mov    ebx, 0x7530
loc_1_0EB5:
        push   0x64
        call   sub_1_0600
        mov    ecx, dword ptr [esi + 0x11f]
        add    cx, 6
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 3], al
        and    word ptr [ebp - 4], 0xf0
        cmp    word ptr [ebp - 4], di
        je     loc_1_0F16
        dec    ebx
        jne    loc_1_0EB5
        cmp    word ptr [ebp - 4], di
        je     loc_1_0F16
        mov    word ptr [ebp - 8], 0xff00
        mov    cx, word ptr [esi + 0x11f]
        mov    al, byte ptr [ebp - 8]
        lea    edx, [ecx + 6]
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, byte ptr [ebp - 7]
        out    dx, al
        inc    word ptr [ebp - 0xa]
        cmp    word ptr [ebp - 0xa], 3
        jb     loc_1_0EB0
loc_1_0F16:
        cmp    word ptr [ebp - 4], 0x40
        jne    loc_1_1214
        lea    eax, [ebp - 0x3c]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0xc], 0
        mov    dword ptr [eax + 0x10], 0
        mov    word ptr [eax + 0x14], 0
        mov    word ptr [ebp - 0x3c], 0x9f00
        mov    word ptr [ebp - 0x30], 0x505
        cmp    byte ptr [esi + 0x127], 0
        je     loc_1_0FA7
        mov    ecx, dword ptr [esi + 0x167]
        shr    ecx, 0x10
        mov    al, cl
        and    eax, 0xffff00ff
        shl    ax, 8
        mov    cl, ch
        and    ecx, 0xffff00ff
        add    ax, cx
        mov    word ptr [ebp - 0x2e], ax
        mov    cx, word ptr [esi + 0x167]
        mov    al, cl
        and    eax, 0xffff00ff
        shl    ax, 8
        mov    cl, ch
        and    ecx, 0xffff00ff
        add    ax, cx
        mov    word ptr [ebp - 0x2c], ax
        jmp    loc_1_0FEB
loc_1_0FA7:
        mov    ecx, dword ptr [esi + 0x16b]
        shr    ecx, 0x10
        mov    al, cl
        and    eax, 0xffff00ff
        shl    ax, 8
        mov    cl, ch
        and    ecx, 0xffff00ff
        add    ax, cx
        mov    word ptr [ebp - 0x2e], ax
        mov    ax, word ptr [esi + 0x16b]
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    word ptr [ebp - 0x2c], cx
loc_1_0FEB:
        cmp    byte ptr [esi + 0x127], 0
        je     loc_1_1033
        mov    ecx, dword ptr [esi + 0x173]
        shr    ecx, 0x10
        mov    al, cl
        and    eax, 0xffff00ff
        shl    ax, 8
        mov    cl, ch
        and    ecx, 0xffff00ff
        add    ax, cx
        mov    word ptr [ebp - 0x2a], ax
        mov    cx, word ptr [esi + 0x173]
        mov    al, cl
        and    eax, 0xffff00ff
        shl    ax, 8
        mov    cl, ch
        and    ecx, 0xffff00ff
        jmp    loc_1_106D
loc_1_1033:
        mov    eax, dword ptr [esi + 0x177]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    word ptr [ebp - 0x2a], cx
        mov    cx, word ptr [esi + 0x177]
        mov    al, ch
        and    eax, 0xffff00ff
        and    cx, 0xff
        shl    cx, 8
loc_1_106D:
        add    ax, cx
        mov    word ptr [ebp - 0x28], ax
        lea    eax, [ebp - 0x2e]
        push   eax
        call   sub_1_0C70
        lea    eax, [ebp - 0x2c]
        push   eax
        call   sub_1_0C70
        lea    eax, [ebp - 0x2a]
        push   eax
        call   sub_1_0C70
        lea    eax, [ebp - 0x28]
        push   eax
        call   sub_1_0C70
        cmp    byte ptr [esi + 0xea], 1
        je     loc_1_10BC
        mov    word ptr [ebp - 0x10], 1
        mov    cx, word ptr [esi + 0x11f]
        mov    al, byte ptr [ebp - 0x10]
        lea    edx, [ecx + 0xc]
        out    dx, al
        lea    edx, [ecx + 0xd]
        mov    al, byte ptr [ebp - 0xf]
        out    dx, al
loc_1_10BC:
        mov    word ptr [ebp - 0xe], 0xa00
        mov    cx, word ptr [esi + 0x11f]
        mov    al, byte ptr [ebp - 0xe]
        lea    edx, [ecx + 4]
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, byte ptr [ebp - 0xd]
        out    dx, al
        lea    eax, [ebp - 0x3c]
        mov    dword ptr [ebp - 0x14], eax
        mov    di, 0xb
        mov    ecx, 2
loc_1_10E6:
        mov    eax, dword ptr [ebp - 0x14]
        mov    ax, word ptr [eax]
        add    dword ptr [ebp - 0x14], ecx
        mov    word ptr [ebp - 6], ax
        dec    di
        mov    bx, word ptr [esi + 0x11f]
        lea    edx, [ebx + 2]
        out    dx, al
        lea    edx, [ebx + 3]
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        jne    loc_1_10E6
        mov    word ptr [ebp - 0xc], 0x9080
        mov    cx, word ptr [esi + 0x11f]
        mov    al, byte ptr [ebp - 0xc]
        lea    edx, [ecx + 6]
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, byte ptr [ebp - 0xb]
        out    dx, al
        mov    dword ptr [esi + 0x97], 0
        mov    dword ptr [esi + 0x9b], 0x1230 ; -> loc_1_1230
        mov    dword ptr [esi + 0x9f], esi
        push   0x2af8
        lea    eax, [esi + 0x97]
        push   eax
        call   sub_1_05E2
        push   0xa
        call   sub_1_0600
        mov    ecx, 0x60
        add    eax, 0
loc_1_1160:
        cmp    byte ptr [esi + 0x96], 0
        jne    loc_1_1214
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 6
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 3], al
        test   word ptr [ebp - 4], cx
        jne    loc_1_1160
        test   byte ptr [ebp - 4], 0x10
        jne    loc_1_11EA
        mov    byte ptr [ebp - 1], 0
        lea    edi, [ebp - 0x1c]
        mov    eax, dword ptr [esi + 0x167]
        mov    dl, 6
loc_1_11A4:
        mov    cl, dl
        dec    dl
        or     cl, cl
        je     loc_1_11BC
        mov    ebx, eax
        inc    eax
        mov    ecx, edi
        inc    edi
        mov    cl, byte ptr [ecx]
        cmp    byte ptr [ebx], cl
        je     loc_1_11A4
        mov    byte ptr [ebp - 1], 1
loc_1_11BC:
        lea    edi, [ebp - 0x24]
        mov    eax, dword ptr [esi + 0x173]
        mov    dl, 8
loc_1_11C7:
        mov    cl, dl
        dec    dl
        or     cl, cl
        je     loc_1_11DF
        mov    ebx, eax
        inc    eax
        mov    ecx, edi
        inc    edi
        mov    cl, byte ptr [ecx]
        cmp    byte ptr [ebx], cl
        je     loc_1_11C7
        mov    byte ptr [ebp - 1], 1
loc_1_11DF:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_11EA
        or     word ptr [ebp - 4], 0x10
loc_1_11EA:
        lea    eax, [ebp - 2]
        push   eax
        lea    eax, [esi + 0x97]
        push   eax
        call   sub_1_05E8
        cmp    byte ptr [esi + 0x96], 0
        jne    loc_1_1214
        test   byte ptr [ebp - 4], 0x10
        jne    loc_1_1214
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1214:
        push   esi
        call   sub_1_0C90
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1230:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        pop    ebp
        mov    byte ptr [eax + 0x96], 1
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1250:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0C90
        cmp    byte ptr [esi + 0x18], 0
        je     loc_1_1269
        push   esi
        call   sub_1_05F4
loc_1_1269:
        push   esi
        call   sub_1_07F0
        mov    eax, dword ptr [esi + 0x23d]
        or     eax, eax
        je     loc_1_127F
        push   eax
        call   sub_2_00D0
loc_1_127F:
        cmp    byte ptr [esi + 0xe8], 0
        je     loc_1_1294
        mov    eax, dword ptr [esi + 0xf8]
        push   eax
        call   sub_1_0568
loc_1_1294:
        push   0
        push   0x241
        push   esi
        call   sub_1_060C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_12B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 0x19], 0
        jne    loc_1_1430
        mov    byte ptr [esi + 0x19], 1
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        dec    dword ptr [esi + 0x14]
        push   0
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 0x23d]
        push   eax
        call   sub_2_01D0
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_1350
        cmp    ebx, 0x103
        je     loc_1_13A0
        cmp    ebx, 0xc001000d
        je     loc_1_13D0
        cmp    ebx, 0xc001000e
        je     loc_1_1400
        lea    eax, [edi + 0xbf]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0xc3]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0xc3]
        mov    ebx, 0xc0000001
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 0xc3], esi
        mov    byte ptr [esi + 0x19], 0
        inc    dword ptr [esi + 0x14]
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1350:
        cmp    dword ptr [esi + 0x14], 0
        je     loc_1_1383
        lea    eax, [edi + 0xc7]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0xcb]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0xcb]
        mov    ebx, 0x103
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 0xcb], esi
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1383:
        push   0
        push   0x92
        push   esi
        call   sub_1_060C
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_13A0:
        lea    eax, [edi + 0xc7]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0xcb]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0xcb]
        push   edi
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 0xcb], esi
        call   sub_1_3170
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_13D0:
        lea    eax, [edi + 0xcf]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0xd3]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0xd3]
        mov    ebx, 0x103
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 0xd3], esi
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_1400:
        lea    eax, [edi + 0xc7]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0xcb]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0xcb]
        mov    ebx, 0x103
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 0xcb], esi
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_1430:
        mov    ebx, 0xc0010002
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1440:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [edi + 0x11e], 0
        jne    loc_1_1492
        cmp    byte ptr [esi + 0x19], 0
        jne    loc_1_1487
        inc    dword ptr [esi + 0x14]
        push   esi
        push   edi
        call   sub_1_14C0
        cmp    dword ptr [edi + 0x32], 0
        je     loc_1_1473
        mov    byte ptr [edi + 0xe7], 1
        jmp    loc_1_1479
loc_1_1473:
        push   edi
        call   sub_1_1570
loc_1_1479:
        mov    eax, 0x103
        dec    dword ptr [esi + 0x14]
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_1487:
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_1492:
        cmp    byte ptr [edi + 0x11a], 0
        je     loc_1_14A6
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_14A6:
        cmp    byte ptr [edi + 0x11d], 0
        mov    eax, 0xc0000001
        je     loc_1_14BA
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_14BA:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
sub_1_14C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    byte ptr [edi + 0x11e], 1
        mov    byte ptr [edi + 0x11a], 1
        mov    dword ptr [edi + 0xd7], ebx
        cmp    dword ptr [edi + 0x2a], 0
        je     loc_1_152D
        mov    esi, 2
loc_1_14EB:
        mov    ecx, dword ptr [edi + 0x2a]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edi + 0x2a], eax
        mov    eax, dword ptr [ecx + 4]
        or     eax, eax
        je     loc_1_1510
        dec    dword ptr [eax + 0x14]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_058C
        jmp    loc_1_1527
        db  08Bh, 0FFh
loc_1_1510:
        cmp    dword ptr [ecx + 0x10], esi
        jne    loc_1_1527
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [edi + 0xfc]
        push   eax
        call   sub_1_05AA
loc_1_1527:
        cmp    dword ptr [edi + 0x2a], 0
        jne    loc_1_14EB
loc_1_152D:
        mov    esi, dword ptr [edi + 0xbf]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0xffffff41
        je     loc_1_1561
        mov    edi, edi
loc_1_1540:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   0
        push   0
        push   0x40010004
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_059E
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0xffffff41
        jne    loc_1_1540
loc_1_1561:
        or     ebx, ebx
        je     loc_1_1568
        inc    dword ptr [ebx + 0x14]
loc_1_1568:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_1_1570:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0xea], 1
        mov    edx, dword ptr [ecx + 0x11f]
        lea    esi, [ecx + 0x11f]
        je     loc_1_15C0
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    word ptr [ebp - 2], 0xfff7
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 8], ax
        mov    di, word ptr [esi]
        lea    edx, [edi + 8]
        out    dx, al
        lea    edx, [edi + 9]
        mov    al, byte ptr [ebp - 7]
        jmp    loc_1_15ED
        db  090h
loc_1_15C0:
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi]
        add    dx, 0xa
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    word ptr [ebp - 2], 0xfffb
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 6], ax
        mov    di, word ptr [esi]
        lea    edx, [edi + 9]
        out    dx, al
        lea    edx, [edi + 0xa]
        mov    al, byte ptr [ebp - 5]
loc_1_15ED:
        mov    word ptr [ebp - 4], 0xff00
        mov    si, word ptr [esi]
        out    dx, al
        lea    edx, [esi + 6]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        lea    edx, [esi + 7]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        lea    eax, [ecx + 0xdb]
        push   0x3e8
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx + 0xdf], 0x1640 ; -> loc_1_1640
        mov    dword ptr [ecx + 0xe3], ecx
        push   eax
        call   sub_1_05E2
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1640:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        call   sub_1_0E60
        or     al, al
        je     loc_1_1663
        mov    word ptr [esi + 0xb1], 3
        push   esi
        call   sub_1_0940
loc_1_1663:
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     word ptr [ebp - 2], 8
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 4], ax
        mov    cx, word ptr [esi + 0x11f]
        pop    esi
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_16B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        cmp    byte ptr [ebp + 0xc], 0
        mov    esi, dword ptr [ebp + 8]
        je     loc_1_1790
        mov    byte ptr [esi + 0x11e], 0
        mov    byte ptr [esi + 0x11a], 0
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 8], 0
        push   esi
        call   sub_1_0E00
        mov    ecx, dword ptr [esi + 0x167]
        mov    ax, word ptr [ecx]
        mov    word ptr [ebp - 2], ax
        sub    eax, eax
loc_1_16F7:
        cmp    word ptr [ebp - 2], ax
        jne    loc_1_16F7
        mov    word ptr [ecx], 0x600
        cmp    byte ptr [esi + 0x127], 0
        je     loc_1_1737
        mov    eax, dword ptr [esi + 0x7a]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x167]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 0x7a]
        jmp    loc_1_1761
loc_1_1737:
        mov    eax, dword ptr [esi + 0x7e]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x167]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 0x7e]
loc_1_1761:
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x167]
        push   esi
        mov    word ptr [eax + 4], cx
        push   0x1900                        ; -> loc_1_1900
        push   esi
        call   sub_1_05FA
        jmp    loc_1_1807
        db  090h
loc_1_1790:
        mov    byte ptr [esi + 0x11e], 1
        mov    byte ptr [esi + 0x11d], 1
        mov    byte ptr [esi + 0x11b], 0
        mov    byte ptr [esi + 0x11a], 0
        mov    eax, dword ptr [ebp + 0x10]
        or     eax, eax
        mov    cx, word ptr [eax + 2]
        mov    word ptr [esi + 0xaf], cx
        mov    dword ptr [ebp - 8], 0xc0011000
        mov    dword ptr [ebp - 0xc], 0xc0000001
        je     loc_1_1807
        cmp    word ptr [eax], 0x300
        jne    loc_1_1807
        mov    ax, word ptr [eax + 2]
        test   al, 2
        je     loc_1_1807
        mov    al, ah
        and    eax, 0xff
        dec    eax
        cmp    eax, 9
        ja     loc_1_1807
        jmp    dword ptr [eax*4 + 0x189c]    ; -> obj1:0x189C
        db  090h
loc_1_17F0:
        mov    dword ptr [ebp - 0xc], 0x80010003
        jmp    loc_1_1807
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1800:
        mov    dword ptr [ebp - 0xc], 0x80010004
loc_1_1807:
        mov    edi, dword ptr [esi + 0xbf]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xffffff41
        je     loc_1_1859
loc_1_1818:
        mov    ebx, edi
        mov    edi, dword ptr [edi]
        cmp    byte ptr [ebp + 0xc], 0
        je     loc_1_1830
        add    ebx, 0x10
        push   0
        push   0
        push   0x40010005
        jmp    loc_1_183E
loc_1_1830:
        add    ebx, 0x10
        push   4
        lea    eax, [ebp - 8]
        push   eax
        push   0x40010007
loc_1_183E:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_05A4
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xffffff41
        jne    loc_1_1818
loc_1_1859:
        mov    eax, dword ptr [esi + 0xd7]
        or     eax, eax
        je     loc_1_187D
        mov    dword ptr [esi + 0xd7], 0
        dec    dword ptr [eax + 0x14]
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_0586
loc_1_187D:
        mov    eax, dword ptr [esi + 0xcf]
        sub    eax, esi
        cmp    eax, 0xcf
        je     loc_1_1892
        push   esi
        call   sub_1_2660
loc_1_1892:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
        ; ---- dispatch table (10 entries) ----
        dd  loc_1_17F0
        dd  loc_1_1800
        dd  loc_1_1807
        dd  loc_1_17F0
        dd  loc_1_1807
        dd  loc_1_1800
        dd  loc_1_1807
        dd  loc_1_1800
        dd  loc_1_1807
        dd  loc_1_17F0
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_18D0:
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
loc_1_1900:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   byte ptr [ebp - 2], 0x80
        je     loc_1_1937
        mov    esi, 0x80
loc_1_1923:
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 6
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   word ptr [ebp - 2], si
        jne    loc_1_1923
loc_1_1937:
        mov    word ptr [ebp - 2], 0x90
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 7
        mov    al, 0x90
        out    dx, al
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1960:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   byte ptr [ebp - 2], 0x80
        je     loc_1_1997
        mov    esi, 0x80
loc_1_1983:
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 6
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   word ptr [ebp - 2], si
        jne    loc_1_1983
loc_1_1997:
        mov    word ptr [ebp - 2], 0x84
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 7
        mov    al, 0x84
        out    dx, al
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_19C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0xea], 1
        je     loc_1_1AF8
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 3], al
        test   byte ptr [ebp - 3], 1
        je     loc_1_1AF8
        test   byte ptr [ebp - 3], 2
        je     loc_1_1AF8
        test   byte ptr [ebp - 3], 4
        mov    bl, 1
        jne    loc_1_1A13
        sub    bl, bl
loc_1_1A13:
        or     word ptr [ebp - 4], 0x800
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ebp - 0xa], ax
        mov    si, word ptr [ecx + 0x11f]
        lea    edx, [esi + 8]
        out    dx, al
        lea    edx, [esi + 9]
        mov    al, byte ptr [ebp - 9]
        out    dx, al
        lea    edx, [esi + 2]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 3
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 6
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 7], al
        mov    edx, dword ptr [ebp - 8]
        and    edx, 0xffff
        shl    edx, 0x10
        mov    dword ptr [ebp - 0x10], edx
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 4
        in     al, dx
        mov    byte ptr [ebp - 6], al
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 5
        in     al, dx
        mov    byte ptr [ebp - 5], al
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        add    dword ptr [ebp - 0x10], eax
        mov    dx, word ptr [ecx + 0x11f]
        or     bl, bl
        je     loc_1_1AC1
        sub    edi, edi
        cmp    word ptr [ebp - 2], di
        jbe    loc_1_1ADC
loc_1_1AAB:
        mov    esi, dword ptr [ebp - 0x10]
        in     al, dx
        mov    byte ptr [esi], al
        inc    edi
        xor    eax, eax
        inc    dword ptr [ebp - 0x10]
        mov    ax, word ptr [ebp - 2]
        cmp    eax, edi
        ja     loc_1_1AAB
        jmp    loc_1_1ADC
loc_1_1AC1:
        cmp    word ptr [ebp - 2], 0
        jbe    loc_1_1ADC
        xor    esi, esi
        mov    si, word ptr [ebp - 2]
        mov    edi, edi
loc_1_1AD0:
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        out    dx, al
        inc    dword ptr [ebp - 0x10]
        dec    esi
        jne    loc_1_1AD0
loc_1_1ADC:
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 3], al
loc_1_1AF8:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x22]
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 6
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   byte ptr [ebp - 2], 0x80
        jne    loc_1_1B50
        sub    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1B50:
        push   8
        mov    eax, dword ptr [esi + 0x173]
        push   eax
        lea    eax, [edi + 6]
        push   eax
        call   sub_1_06EC
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [edi + 4], ax
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x22], eax
        test   byte ptr [ebp - 1], 0x80
        je     loc_1_1BA9
        mov    edi, 0x8000
        lea    ebx, [ebx]
loc_1_1B80:
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 6
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 1], al
        push   2
        call   sub_1_0600
        test   word ptr [ebp - 2], di
        jne    loc_1_1B80
loc_1_1BA9:
        mov    word ptr [ebp - 4], 0xa000
        mov    cx, word ptr [esi + 0x11f]
        mov    al, byte ptr [ebp - 4]
        lea    edx, [ecx + 6]
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1BD0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x1e]
        cmp    dword ptr [ecx + 0x22], eax
        lea    edx, [ecx + 0x26]
        jne    loc_1_1BF2
        mov    dword ptr [edx], 0
        cmp    dword ptr [edx], 1
        sbb    al, al
        inc    al
        pop    ebp
        ret    4
loc_1_1BF2:
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 0x1e]
        pop    ebp
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx + 0x1e], eax
        cmp    dword ptr [edx], 1
        sbb    al, al
        inc    al
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1C10:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0xea], 1
        mov    edx, dword ptr [esi + 0x11f]
        jne    loc_1_1C48
        add    dx, 9
        in     al, dx
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 9
        or     al, 0x20
        out    dx, al
        mov    byte ptr [esi + 0x19], 0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1C48:
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 6], al
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 5], al
        mov    ax, word ptr [ebp - 6]
        or     ax, 8
        mov    word ptr [ebp - 4], ax
        mov    cx, word ptr [esi + 0x11f]
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    byte ptr [esi + 0x19], 0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1C90:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        sub    bl, bl
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_19C0
        cmp    byte ptr [esi + 0x19], bl
        jne    loc_1_1D13
        push   esi
        call   sub_1_1B10
        cmp    al, bl
        je     loc_1_1D13
        cmp    byte ptr [esi + 0xea], 1
        mov    edx, dword ptr [esi + 0x11f]
        je     loc_1_1CF8
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 3], al
        and    word ptr [ebp - 4], 0xfff7
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ebp - 6], ax
        mov    cx, word ptr [esi + 0x11f]
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, byte ptr [ebp - 5]
        jmp    loc_1_1D0C
loc_1_1CF8:
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 9
        and    al, 0xdf
loc_1_1D0C:
        out    dx, al
        mov    byte ptr [esi + 0x19], 1
        mov    bl, 1
loc_1_1D13:
        cmp    byte ptr [esi + 0xea], 1
        mov    edx, dword ptr [esi + 0x11f]
        je     loc_1_1D4A
        add    dx, 0xf
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 0xf
        mov    cl, al
        mov    al, cl
        or     al, 0x80
        out    dx, al
        mov    al, cl
        and    al, 0x7f
        out    dx, al
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1D4A:
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 9
        or     al, 0x80
        out    dx, al
        mov    al, byte ptr [ebp - 1]
        and    al, 0x7f
        out    dx, al
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_1D70:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        push   0x1bd0                        ; -> loc_1_1BD0
        push   esi
        call   sub_1_05FA
        or     al, al
        je     loc_1_1E77
        mov    edi, 0x80
loc_1_1D95:
        mov    ebx, dword ptr [esi + 0x26]
        mov    dword ptr [esi + 0x26], 0
        cmp    byte ptr [esi + 0x11a], 0
        mov    ax, word ptr [ebx + 4]
        je     loc_1_1E10
        and    al, 0xf
        cmp    al, 8
        jne    loc_1_1E4B
        cmp    word ptr [ebx + 6], 0x300
        jne    loc_1_1E4B
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0xa3]
        push   eax
        call   sub_1_05E8
        cmp    word ptr [ebx + 8], di
        jne    loc_1_1DE4
        push   0
        push   1
        push   esi
        call   sub_1_16B0
        jmp    loc_1_1E4B
loc_1_1DE4:
        mov    ax, word ptr [esi + 0xb1]
        or     ax, ax
        je     loc_1_1E01
        dec    ax
        mov    word ptr [esi + 0xb1], ax
        push   esi
        call   sub_1_0940
        jmp    loc_1_1E4B
loc_1_1E01:
        lea    eax, [ebx + 6]
        push   eax
        push   0
        push   esi
        call   sub_1_16B0
        jmp    loc_1_1E4B
        db  090h
loc_1_1E10:
        and    eax, 0xf
        cmp    eax, 0xc
        ja     loc_1_1E4B
        jmp    dword ptr [eax*4 + 0x1e8c]    ; -> obj1:0x1E8C
        db  090h
loc_1_1E20:
        push   ebx
        push   esi
        call   sub_1_2650
        jmp    loc_1_1E4B
loc_1_1E29:
        push   ebx
        push   esi
        call   sub_1_2500
        jmp    loc_1_1E4B
loc_1_1E32:
        push   ebx
        push   esi
        call   sub_1_2060
        jmp    loc_1_1E4B
loc_1_1E3B:
        push   ebx
        push   esi
        call   sub_1_22E0
        jmp    loc_1_1E4B
loc_1_1E44:
        push   ebx
        push   esi
        call   sub_1_1EC0
loc_1_1E4B:
        push   esi
        push   0x19c0                        ; -> sub_1_19C0
        push   esi
        call   sub_1_05FA
        push   esi
        push   0x1b10                        ; -> sub_1_1B10
        push   esi
        call   sub_1_05FA
        push   esi
        push   0x1bd0                        ; -> loc_1_1BD0
        push   esi
        call   sub_1_05FA
        or     al, al
        jne    loc_1_1D95
loc_1_1E77:
        push   esi
        push   0x1c10                        ; -> loc_1_1C10
        push   esi
        call   sub_1_05FA
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        ; ---- dispatch table (13 entries) ----
        dd  loc_1_1E20
        dd  loc_1_1E4B
        dd  loc_1_1E4B
        dd  loc_1_1E4B
        dd  loc_1_1E29
        dd  loc_1_1E4B
        dd  loc_1_1E4B
        dd  loc_1_1E4B
        dd  loc_1_1E32
        dd  loc_1_1E4B
        dd  loc_1_1E3B
        dd  loc_1_1E4B
        dd  loc_1_1E44
sub_1_1EC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0x46]
        mov    ecx, edx
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [ebp - 4], edx
        mov    di, word ptr [eax + 4]
        test   di, 0x20
        je     loc_1_1F14
        mov    edx, dword ptr [edx + 4]
        mov    dword ptr [ebp - 0xc], edx
        mov    edx, dword ptr [ebp - 4]
        test   di, 4
        mov    edx, dword ptr [edx]
        mov    dword ptr [ebp - 0x10], edx
        mov    word ptr [eax + 6], 0
        je     loc_1_1F05
        inc    dword ptr [esi + 0x1d4]
        jmp    loc_1_1F22
loc_1_1F05:
        inc    dword ptr [esi + 0x1cc]
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_1F29
loc_1_1F14:
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x10], 0
loc_1_1F22:
        mov    dword ptr [ebp - 8], 0xc0000001
loc_1_1F29:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    word ptr [eax + 8], 0x80
        je     loc_1_1F41
        inc    dword ptr [esi + 0x1d4]
        mov    dword ptr [ebp - 8], 0xc0000001
loc_1_1F41:
        sub    dx, dx
        cmp    dword ptr [esi + 0x5a], 0
        jbe    loc_1_1FB2
        lea    ebx, [ebx]
loc_1_1F50:
        cmp    byte ptr [esi + 0x127], 0
        jne    loc_1_1F6F
        mov    eax, edx
        and    eax, 0xffff
        lea    eax, [ecx + eax*8 + 0x18]
        cmp    dword ptr [eax], 0
        je     loc_1_1F6F
        mov    dword ptr [eax], 0
loc_1_1F6F:
        mov    eax, edx
        and    eax, 0xffff
        lea    eax, [eax + eax*2]
        lea    edi, [eax*2]
        mov    eax, dword ptr [ecx + 0xc]
        mov    word ptr [edi + eax + 8], 0
        mov    eax, dword ptr [ecx + 0xc]
        add    eax, edi
        inc    dx
        mov    bx, word ptr [eax + 8]
        mov    word ptr [eax + 0xc], bx
        mov    eax, dword ptr [ecx + 0xc]
        add    eax, edi
        mov    di, word ptr [eax + 0xc]
        mov    word ptr [eax + 0xa], di
        mov    eax, edx
        and    eax, 0xffff
        cmp    eax, dword ptr [esi + 0x5a]
        jb     loc_1_1F50
loc_1_1FB2:
        mov    eax, dword ptr [ecx + 0xc]
        test   byte ptr [eax + 4], 0x10
        jne    loc_1_1FC0
        mov    ecx, dword ptr [ecx + 8]
        jmp    loc_1_1F41
loc_1_1FC0:
        mov    eax, dword ptr [ecx + 0xc]
        mov    cx, word ptr [eax + 2]
        add    eax, 2
        test   ch, 1
        je     loc_1_1FD7
        and    cx, 0xfeff
        mov    word ptr [eax], cx
loc_1_1FD7:
        mov    eax, dword ptr [ebp - 4]
        lea    ebx, [ebx]
loc_1_1FE0:
        mov    edx, dword ptr [eax + 0xc]
        mov    cx, word ptr [edx + 4]
        add    edx, 4
        mov    word ptr [edx], 0
        inc    dword ptr [esi + 0x52]
        test   cl, 0x10
        mov    eax, dword ptr [eax + 8]
        jne    loc_1_2000
        jmp    loc_1_1FE0
        db  08Dh, 064h, 024h, 000h
loc_1_2000:
        mov    dword ptr [esi + 0x46], eax
        mov    dword ptr [esi + 0x3e], 0
        mov    edx, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x19b], 0
        mov    edi, dword ptr [edx + 0x28]
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edx
        mov    edx, dword ptr [ebp - 0x10]
        push   edx
        call   sub_1_057A
        dec    dword ptr [edi + 0x14]
        mov    eax, dword ptr [esi + 0xc7]
        sub    eax, esi
        cmp    eax, 0xc7
        je     loc_1_2040
        push   esi
        call   sub_1_2660
loc_1_2040:
        push   1
        push   esi
        call   sub_1_3D50
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2060:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    ax, word ptr [edi + 8]
        and    ax, 0xff
        sub    ax, 0x80
        cmp    ax, 1
        sbb    bl, bl
        neg    bl
        xor    eax, eax
        mov    ax, word ptr [edi + 6]
        cmp    eax, 0x300
        je     loc_1_20F0
        cmp    eax, 0x800
        je     loc_1_22C0
        cmp    eax, 0x900
        je     loc_1_22C0
        cmp    eax, 0xa00
        je     loc_1_22C0
        cmp    eax, 0xd00
        je     loc_1_22C0
        mov    esi, dword ptr [ebp + 8]
loc_1_20BC:
        mov    eax, dword ptr [esi + 0xc7]
        sub    eax, esi
        cmp    eax, 0xc7
        je     loc_1_20D1
        push   esi
        call   sub_1_2660
loc_1_20D1:
        cmp    byte ptr [esi + 0xe7], 0
        je     loc_1_20E7
        mov    byte ptr [esi + 0xe7], 0
        push   esi
        call   sub_1_1570
loc_1_20E7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_20F0:
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x11b], 0
        je     loc_1_20BC
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0xa3]
        push   eax
        call   sub_1_05E8
        mov    ax, word ptr [edi + 8]
        mov    word ptr [esi + 0xaf], ax
        or     bl, bl
        je     loc_1_2220
        mov    byte ptr [esi + 0x11e], 0
        mov    byte ptr [esi + 0x11d], 0
        mov    byte ptr [esi + 0x11b], 0
        mov    edi, dword ptr [esi + 0xbf]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xffffff41
        je     loc_1_216B
loc_1_2145:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        cmp    byte ptr [eax + 0x18], 0
        je     loc_1_2160
        mov    byte ptr [eax + 0x18], 0
        push   0
        push   0
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_056E
loc_1_2160:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xffffff41
        jne    loc_1_2145
loc_1_216B:
        push   esi
        call   sub_1_0E00
        mov    ecx, dword ptr [esi + 0x167]
        mov    ax, word ptr [ecx]
        mov    word ptr [ebp - 4], ax
        sub    eax, eax
loc_1_2180:
        cmp    word ptr [ebp - 4], ax
        jne    loc_1_2180
        mov    word ptr [ecx], 0x600
        cmp    byte ptr [esi + 0x127], 0
        je     loc_1_21C0
        mov    eax, dword ptr [esi + 0x7a]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x167]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 0x7a]
        jmp    loc_1_21EA
loc_1_21C0:
        mov    eax, dword ptr [esi + 0x7e]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x167]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 0x7e]
loc_1_21EA:
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x167]
        push   esi
        mov    word ptr [eax + 4], cx
        push   0x1900                        ; -> loc_1_1900
        push   esi
        call   sub_1_05FA
        jmp    loc_1_20BC
        db  005h, 000h, 000h, 000h, 000h
loc_1_2220:
        mov    ax, word ptr [esi + 0xb1]
        or     ax, ax
        je     loc_1_2240
        dec    ax
        mov    word ptr [esi + 0xb1], ax
        push   esi
        call   sub_1_0940
        jmp    loc_1_20BC
loc_1_2240:
        mov    byte ptr [esi + 0x11b], 0
        mov    byte ptr [esi + 0x11e], 1
        mov    byte ptr [esi + 0x11d], 1
        mov    edi, dword ptr [esi + 0xbf]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xffffff41
        je     loc_1_228F
loc_1_2266:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        cmp    byte ptr [eax + 0x18], 0
        je     loc_1_2284
        mov    byte ptr [eax + 0x18], 0
        push   0
        push   0xc0000001
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_056E
loc_1_2284:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xffffff41
        jne    loc_1_2266
loc_1_228F:
        xor    eax, eax
        mov    ax, word ptr [esi + 0xaf]
        push   eax
        push   0xf
        push   1
        push   3
        push   0xaa000003
        mov    eax, dword ptr [esi + 0xf8]
        push   eax
        call   sub_1_06D8
        add    esp, 0x18
        push   esi
        call   sub_1_0C90
        jmp    loc_1_20BC
        db  08Bh, 0FFh
loc_1_22C0:
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_3470
        or     al, al
        je     loc_1_20BC
        push   esi
        call   sub_1_3170
        jmp    loc_1_20BC
        db  08Dh, 049h, 000h
sub_1_22E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [ebp - 3], 1
        mov    ax, word ptr [eax + 8]
        cmp    ax, 0x80
        jne    loc_1_2303
        mov    byte ptr [ebp - 4], 0
        mov    edi, dword ptr [ebp + 8]
        jmp    loc_1_231F
loc_1_2303:
        cmp    ax, 0x40
        jne    loc_1_2312
        mov    byte ptr [ebp - 4], 1
        mov    edi, dword ptr [ebp + 8]
        jmp    loc_1_231F
loc_1_2312:
        mov    edi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 4], 0
        inc    dword ptr [edi + 0x1d8]
loc_1_231F:
        mov    esi, 0x10
loc_1_2324:
        cmp    byte ptr [ebp - 3], 0
        je     loc_1_24C9
        mov    ebx, dword ptr [edi + 0x6e]
        mov    eax, dword ptr [ebx + 4]
        test   byte ptr [eax + 4], 0x20
        je     loc_1_24C0
        inc    dword ptr [edi + 0x1d0]
        mov    eax, dword ptr [ebx + 4]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        push   eax
        call   sub_1_0C70
        mov    eax, dword ptr [ebx + 0xc]
        add    eax, 8
        test   byte ptr [eax], 0x80
        je     loc_1_2370
        mov    al, byte ptr [eax + 6]
        and    ax, 0x1f
        mov    word ptr [ebp - 8], ax
        jmp    loc_1_2376
loc_1_2370:
        mov    word ptr [ebp - 8], 0
loc_1_2376:
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        mov    ecx, dword ptr [edi + 0x82]
        cmp    ecx, eax
        jb     loc_1_2388
        mov    ecx, eax
loc_1_2388:
        mov    eax, dword ptr [edi + 0x23d]
        mov    byte ptr [ebp - 2], 1
        mov    eax, dword ptr [eax + 0x10]
        test   esi, eax
        je     loc_1_2408
        test   al, 0x20
        jne    loc_1_2408
        mov    eax, dword ptr [ebx + 0xc]
        mov    al, byte ptr [eax + 2]
        and    al, 0x80
        mov    byte ptr [ebp - 1], al
        or     al, al
        jne    loc_1_2408
        mov    edx, dword ptr [ebx + 0xc]
        lea    eax, [edi + 0x161]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [eax + 2]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [edx + 4]
        add    edx, 2
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x10], eax
        jae    loc_1_23D2
        mov    byte ptr [ebp - 1], 1
        jmp    loc_1_23FE
loc_1_23D2:
        mov    eax, dword ptr [ebp - 0x14]
        cmp    dword ptr [ebp - 0x10], eax
        jbe    loc_1_23E0
        mov    byte ptr [ebp - 1], 0xff
        jmp    loc_1_23FE
loc_1_23E0:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dx, word ptr [edx]
        mov    ax, word ptr [eax]
        cmp    ax, dx
        jae    loc_1_23F4
        mov    byte ptr [ebp - 1], 1
        jmp    loc_1_23FE
loc_1_23F4:
        mov    byte ptr [ebp - 1], 0xff
        ja     loc_1_23FE
        mov    byte ptr [ebp - 1], 0
loc_1_23FE:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_2408
        mov    byte ptr [ebp - 2], 0
loc_1_2408:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_246F
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        cmp    dword ptr [edi + 0x13c], eax
        jb     loc_1_246F
        cmp    word ptr [ebp - 6], 0xe
        jae    loc_1_2430
        push   0
        push   0
        push   0
        push   eax
        jmp    loc_1_2452
        db  08Dh, 064h, 024h, 000h
loc_1_2430:
        mov    edx, dword ptr [ebp - 8]
        and    edx, 0xffff
        sub    eax, edx
        sub    eax, 0xe
        push   eax
        sub    ecx, edx
        sub    ecx, 0xe
        push   ecx
        mov    eax, dword ptr [ebx + 0xc]
        add    eax, edx
        add    eax, 0xe
        push   eax
        add    edx, 0xe
        push   edx
loc_1_2452:
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        push   ebx
        mov    eax, dword ptr [edi + 0x23d]
        push   eax
        call   sub_1_00B0
        mov    eax, dword ptr [edi + 0x23d]
        push   eax
        call   sub_1_0300
loc_1_246F:
        mov    eax, dword ptr [ebx + 4]
        test   word ptr [eax + 4], si
        jne    loc_1_2492
        mov    ecx, 0x88
        lea    ecx, [ecx]
loc_1_2480:
        mov    eax, dword ptr [ebx + 4]
        mov    word ptr [eax + 4], cx
        mov    ebx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        test   word ptr [eax + 4], si
        je     loc_1_2480
loc_1_2492:
        mov    eax, dword ptr [ebx + 4]
        mov    word ptr [eax + 4], 0x88
        mov    eax, dword ptr [ebx]
        mov    dword ptr [edi + 0x6e], eax
        mov    eax, dword ptr [ebx + 4]
        or     word ptr [eax + 2], 0x100
        mov    eax, dword ptr [edi + 0x72]
        mov    eax, dword ptr [eax + 4]
        and    word ptr [eax + 2], 0xfeff
        mov    dword ptr [edi + 0x72], ebx
        jmp    loc_1_2324
        db  08Dh, 049h, 000h
loc_1_24C0:
        mov    byte ptr [ebp - 3], 0
        jmp    loc_1_2324
loc_1_24C9:
        cmp    byte ptr [ebp - 4], 0
        je     loc_1_24DB
        push   edi
        push   0x1960                        ; -> loc_1_1960
        push   edi
        call   sub_1_05FA
loc_1_24DB:
        mov    eax, dword ptr [edi + 0xc7]
        sub    eax, edi
        cmp    eax, 0xc7
        je     loc_1_24F0
        push   edi
        call   sub_1_2660
loc_1_24F0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2500:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], 0
        mov    bx, word ptr [eax + 8]
        mov    si, bx
        and    si, 0x8000
        je     loc_1_2576
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0x1cb], 0
        jne    loc_1_2576
        mov    byte ptr [eax + 0x1cb], 1
        mov    edx, eax
        add    edx, 0x1a3
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0xa
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0x10], 8
        cmp    dword ptr [eax + 0x2a], 0
        jne    loc_1_2561
        mov    dword ptr [eax + 0x2e], edx
        mov    dword ptr [eax + 0x2a], edx
        jmp    loc_1_256D
loc_1_2561:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x2e], edx
        mov    dword ptr [edx], 0
loc_1_256D:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3170
loc_1_2576:
        test   bh, 0x20
        je     loc_1_257F
        or     dword ptr [ebp - 4], 0x40
loc_1_257F:
        test   bh, 0x40
        je     loc_1_258B
        or     dword ptr [ebp - 4], 0x80
loc_1_258B:
        or     si, si
        je     loc_1_2597
        or     dword ptr [ebp - 4], 0x100
loc_1_2597:
        test   bl, 1
        je     loc_1_25A3
        or     dword ptr [ebp - 4], 0x200
loc_1_25A3:
        test   bl, 4
        je     loc_1_25AF
        or     dword ptr [ebp - 4], 0x400
loc_1_25AF:
        test   bl, 8
        je     loc_1_25BB
        or     dword ptr [ebp - 4], 0x800
loc_1_25BB:
        test   bl, 0x10
        je     loc_1_25C7
        or     dword ptr [ebp - 4], 0x1000
loc_1_25C7:
        test   bl, 0x20
        je     loc_1_25D3
        or     dword ptr [ebp - 4], 0x2000
loc_1_25D3:
        test   bl, 0x40
        je     loc_1_25DF
        or     dword ptr [ebp - 4], 0x4000
loc_1_25DF:
        test   bl, 0x80
        je     loc_1_25EB
        or     dword ptr [ebp - 4], 0x8000
loc_1_25EB:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_2638
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 0xbf]
        sub    eax, esi
        cmp    eax, 0xffffff41
        je     loc_1_2638
loc_1_2603:
        mov    ebx, esi
        inc    dword ptr [esi + 0x14]
        lea    edi, [esi + 0x14]
        mov    esi, dword ptr [esi]
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   0x40010006
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_05A4
        dec    dword ptr [edi]
        mov    eax, dword ptr [ebp + 8]
        sub    eax, esi
        cmp    eax, 0xffffff41
        jne    loc_1_2603
loc_1_2638:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2650:
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2660:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 0x11a], 0
        jne    loc_1_2777
        mov    eax, dword ptr [edi + 0xcf]
        sub    eax, edi
        cmp    eax, 0xcf
        je     loc_1_2739
        sub    esi, esi
        add    eax, 0
loc_1_2690:
        mov    ebx, dword ptr [edi + 0xcf]
        push   esi
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 0x23d]
        push   eax
        call   sub_2_01D0
        cmp    eax, esi
        je     loc_1_26C3
        cmp    eax, 0x103
        je     loc_1_2701
        jmp    loc_1_2726
loc_1_26C3:
        cmp    dword ptr [ebx + 0x14], esi
        je     loc_1_26E9
        lea    eax, [edi + 0xc7]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [edi + 0xcb]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [edi + 0xcb]
        mov    dword ptr [eax], ebx
        mov    dword ptr [edi + 0xcb], ebx
        jmp    loc_1_2726
loc_1_26E9:
        push   esi
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_0574
        push   esi
        push   0x92
        push   ebx
        call   sub_1_060C
        jmp    loc_1_2726
loc_1_2701:
        lea    eax, [edi + 0xc7]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [edi + 0xcb]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [edi + 0xcb]
        push   edi
        mov    dword ptr [eax], ebx
        mov    dword ptr [edi + 0xcb], ebx
        call   sub_1_3170
loc_1_2726:
        mov    eax, dword ptr [edi + 0xcf]
        sub    eax, edi
        cmp    eax, 0xcf
        jne    loc_1_2690
loc_1_2739:
        mov    esi, dword ptr [edi + 0xc7]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xc7
        je     loc_1_2777
        cmp    dword ptr [esi + 0x14], 0
        jne    loc_1_2777
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_0574
        push   0
        push   0x92
        push   esi
        call   sub_1_060C
loc_1_2777:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_2780:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x18], esi
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0x1c], ecx
        cmp    byte ptr [ecx + 0x11e], 0
        jne    loc_1_2931
        cmp    byte ptr [eax + 0x19], 0
        jne    loc_1_2923
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 0x10], 0
        mov    dword ptr [eax], 0
        lea    eax, [ebp - 8]
        push   0
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_0630
        cmp    dword ptr [ebp - 0x20], 0
        je     loc_1_2912
        mov    byte ptr [ebp - 1], 0
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0654
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 8
        test   byte ptr [eax], 0x80
        je     loc_1_2810
        mov    al, byte ptr [eax + 6]
        and    ax, 0x1f
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_2816
        db  08Bh, 0FFh
loc_1_2810:
        mov    word ptr [ebp - 4], 0
loc_1_2816:
        mov    edx, dword ptr [ebp - 4]
        and    edx, 0xffff
        mov    eax, dword ptr [esi + 0xc]
        mov    ecx, dword ptr [ebp - 0x1c]
        lea    edi, [edx + eax + 0xe]
        mov    esi, dword ptr [ecx + 0x82]
        sub    esi, edx
        sub    esi, 0xe
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_290A
        mov    edi, edi
loc_1_2840:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_2873
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0678
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_290A
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0654
        jmp    loc_1_28FE
loc_1_2873:
        or     esi, esi
        jne    loc_1_28B0
        inc    byte ptr [ebp - 1]
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [ebp - 1]
        cmp    eax, dword ptr [ecx + 0x86]
        jne    loc_1_2896
        mov    eax, dword ptr [ebp - 0x18]
        mov    byte ptr [ebp - 1], 0
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x18], eax
loc_1_2896:
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [ebp - 0x18]
        mov    esi, dword ptr [ecx + 0x82]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 1]
        mov    edi, dword ptr [eax + ecx*8 + 0xc]
        jmp    loc_1_28FE
        db  08Dh, 049h, 000h
loc_1_28B0:
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_28CF
        cmp    esi, dword ptr [ebp + 0x10]
        jae    loc_1_28C2
        sub    dword ptr [ebp + 0x10], esi
        sub    esi, esi
        jmp    loc_1_28FE
loc_1_28C2:
        add    edi, dword ptr [ebp + 0x10]
        sub    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp + 0x10], 0
loc_1_28CF:
        mov    ebx, dword ptr [ebp + 0x14]
        sub    ebx, dword ptr [ebp - 0x10]
        mov    eax, esi
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    eax, ecx
        jb     loc_1_28E0
        mov    eax, ecx
loc_1_28E0:
        cmp    ebx, eax
        jb     loc_1_28E6
        mov    ebx, eax
loc_1_28E6:
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_06EC
        add    dword ptr [ebp - 0x14], ebx
        add    edi, ebx
        add    dword ptr [ebp - 0x10], ebx
        sub    esi, ebx
        sub    dword ptr [ebp - 0xc], ebx
loc_1_28FE:
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    ecx, dword ptr [ebp + 0x14]
        jb     loc_1_2840
loc_1_290A:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [eax], ecx
loc_1_2912:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        dec    dword ptr [eax + 0x14]
        sub    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_2923:
        mov    eax, 0xc001000c
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_2931:
        mov    ecx, dword ptr [ebp - 0x1c]
        cmp    byte ptr [ecx + 0x11a], 0
        je     loc_1_2950
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  005h, 000h, 000h, 000h, 000h
loc_1_2950:
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    eax, 0xc0000001
        cmp    byte ptr [ecx + 0x11b], 0
        je     loc_1_296A
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_296A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2980:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x11a], 0
        je     loc_1_29A7
        push   esi
        call   sub_1_0C90
        push   0
        push   0
        push   esi
        call   sub_1_16B0
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
loc_1_29A7:
        cmp    byte ptr [esi + 0x11b], 0
        je     loc_1_2A05
        mov    byte ptr [esi + 0x11e], 1
        mov    byte ptr [esi + 0x11d], 1
        mov    byte ptr [esi + 0x11b], 0
        mov    edi, dword ptr [esi + 0xbf]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xffffff41
        je     loc_1_29FF
loc_1_29D6:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        cmp    byte ptr [eax + 0x18], 0
        je     loc_1_29F4
        mov    byte ptr [eax + 0x18], 0
        push   0
        push   0xc0000001
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_056E
loc_1_29F4:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xffffff41
        jne    loc_1_29D6
loc_1_29FF:
        push   esi
        call   sub_1_0C90
loc_1_2A05:
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h
loc_1_2A10:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ecx + 8]
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        jne    loc_1_2A46
        lea    eax, [esi + 0x20]
        push   eax
        lea    eax, [esi + 0x24]
        push   eax
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        push   ecx
        push   edx
        call   sub_1_2D10
        pop    esi
        pop    ebp
        ret    8
loc_1_2A46:
        cmp    eax, 1
        jne    loc_1_2AA0
        cmp    byte ptr [edx + 0x11e], 0
        je     loc_1_2A83
        cmp    byte ptr [edx + 0x11a], 0
        je     loc_1_2A67
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    8
loc_1_2A67:
        cmp    byte ptr [edx + 0x11d], 0
        mov    eax, 0xc0000001
        pop    esi
        pop    ebp
        je     loc_1_2A80
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2A80:
        ret    8
loc_1_2A83:
        cmp    byte ptr [ecx + 0x19], 0
        je     loc_1_2A93
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    8
loc_1_2A93:
        push   esi
        push   ecx
        push   edx
        call   sub_1_2AE0
        pop    esi
        pop    ebp
        ret    8
loc_1_2AA0:
        mov    eax, 0x10001
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2AB0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_1_3680
        mov    esi, eax
        cmp    esi, 0x103
        jne    loc_1_2AD2
        push   edi
        call   sub_1_3170
loc_1_2AD2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2AE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        sub    ebx, ebx
        mov    edi, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x14]
        cmp    edi, 4
        je     loc_1_2B10
        mov    dword ptr [esi + 0x20], ebx
        mov    dword ptr [esi + 0x24], ebx
        mov    ebx, 0xc0010014
        jmp    loc_1_2B97
        db  08Dh, 064h, 024h, 000h
loc_1_2B10:
        cmp    eax, 0x1010e
        je     loc_1_2B41
        cmp    eax, 0x1010f
        je     loc_1_2B61
        cmp    eax, 0x2010103
        je     loc_1_2B75
        cmp    eax, 0x2010104
        je     loc_1_2B86
        mov    ebx, 0xc0010017
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_1_2B97
loc_1_2B41:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_06EC
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3850
        jmp    loc_1_2B95
loc_1_2B61:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [eax + 0x82], ecx
        jae    loc_1_2B97
        mov    ebx, 0xc0010014
        jmp    loc_1_2B97
loc_1_2B75:
        push   ecx
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3780
        jmp    loc_1_2B95
loc_1_2B86:
        push   ecx
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_36D0
loc_1_2B95:
        mov    ebx, eax
loc_1_2B97:
        or     ebx, ebx
        jne    loc_1_2BA5
        mov    dword ptr [esi + 0x20], edi
        mov    dword ptr [esi + 0x24], 0
loc_1_2BA5:
        cmp    ebx, 0x103
        jne    loc_1_2BB6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3170
loc_1_2BB6:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2BD0:
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
        call   sub_1_2D10
        mov    edi, eax
        cmp    edi, 0xc00000bb
        jne    loc_1_2CF0
        sub    edi, edi
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x20105
        ja     loc_1_2C56
        je     loc_1_2C98
        cmp    eax, 0x20101
        je     loc_1_2C72
        cmp    eax, 0x20102
        je     loc_1_2C80
        cmp    eax, 0x20103
        je     loc_1_2C88
        cmp    eax, 0x20104
        je     loc_1_2C90
        mov    edi, 0xc0010017
        jmp    loc_1_2CC1
loc_1_2C56:
        cmp    eax, 0x2010105
        je     loc_1_2CA0
        cmp    eax, 0x2020101
        je     loc_1_2CB0
        cmp    eax, 0x2020102
        je     loc_1_2CB8
        mov    edi, 0xc0010017
        jmp    loc_1_2CC1
loc_1_2C72:
        mov    eax, dword ptr [ebx + 0x1cc]
        jmp    loc_1_2CBE
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2C80:
        mov    eax, dword ptr [ebx + 0x1d0]
        jmp    loc_1_2CBE
loc_1_2C88:
        mov    eax, dword ptr [ebx + 0x1d4]
        jmp    loc_1_2CBE
loc_1_2C90:
        mov    eax, dword ptr [ebx + 0x1d8]
        jmp    loc_1_2CBE
loc_1_2C98:
        mov    eax, dword ptr [ebx + 0x1f4]
        jmp    loc_1_2CBE
loc_1_2CA0:
        xor    eax, eax
        mov    ax, word ptr [ebx + 0xaf]
        or     eax, 0xc0011000
        jmp    loc_1_2CBE
loc_1_2CB0:
        mov    eax, dword ptr [ebx + 0x1dc]
        jmp    loc_1_2CBE
loc_1_2CB8:
        mov    eax, dword ptr [ebx + 0x1f0]
loc_1_2CBE:
        mov    dword ptr [ebp - 4], eax
loc_1_2CC1:
        or     edi, edi
        jne    loc_1_2CF0
        cmp    dword ptr [ebp - 0x10], 4
        jb     loc_1_2CE0
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_06EC
        add    dword ptr [ebp - 0xc], 4
        jmp    loc_1_2CF0
loc_1_2CE0:
        mov    eax, 4
        sub    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 8], eax
        mov    edi, 0xc0010014
loc_1_2CF0:
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
sub_1_2D10:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x68
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x10], 1
        sub    ebx, ebx
        lea    edi, [ebp - 8]
        mov    esi, 4
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 0x2010101
        ja     loc_1_2D50
        je     loc_1_2FA0
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_2D71
        shl    eax, 2
        jmp    loc_1_30CC
        db  08Dh, 049h, 000h
loc_1_2D50:
        cmp    eax, 0x2010102
        je     loc_1_2FE0
        cmp    eax, 0x2010103
        je     loc_1_3020
        cmp    eax, 0x2010104
        je     loc_1_3061
loc_1_2D71:
        mov    ebx, 0xc00000bb
        jmp    loc_1_3093
        db  005h, 000h, 000h, 000h, 000h
loc_1_2D80:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_2D95
        mov    edi, 0x4490                   ; -> obj1:0x4490
        mov    esi, 0x5c
        jmp    loc_1_3093
loc_1_2D95:
        mov    edi, 0x4414                   ; -> loc_1_4414
        mov    esi, 0x7c
        jmp    loc_1_3093
loc_1_2DA4:
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x11a], 0
        je     loc_1_2DC0
        mov    dword ptr [ebp - 0xc], 2
        jmp    loc_1_2DE9
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2DC0:
        cmp    byte ptr [ecx + 0x11b], 0
        je     loc_1_2DD2
        mov    dword ptr [ebp - 0xc], 1
        jmp    loc_1_2DE9
loc_1_2DD2:
        cmp    byte ptr [ecx + 0x11e], 0
        mov    dword ptr [ebp - 0xc], 4
        jne    loc_1_2DE9
        mov    dword ptr [ebp - 0xc], 0
loc_1_2DE9:
        lea    edi, [ebp - 0xc]
        mov    esi, 4
        jmp    loc_1_3093
loc_1_2DF6:
        lea    edi, [ebp - 0x10]
        mov    esi, 4
        jmp    loc_1_3093
loc_1_2E03:
        lea    edi, [ebp - 0x10]
        mov    esi, 4
        jmp    loc_1_3093
loc_1_2E10:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x82]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_3093
loc_1_2E21:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x13c]
        sub    eax, 0xe
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_3093
loc_1_2E35:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x130]
        sub    eax, 0x10
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x1d4c0
        add    eax, 0x9c40
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_3093
loc_1_2E58:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x13c]
        imul   eax, dword ptr [ecx + 0x56]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_3093
        db  08Dh, 049h, 000h
loc_1_2E70:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x82]
        imul   eax, dword ptr [ecx + 0x86]
        imul   eax, dword ptr [ecx + 0x76]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_3093
        db  08Dh, 064h, 024h, 000h
loc_1_2E90:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x13c]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_3093
loc_1_2EA1:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x13c]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_3093
loc_1_2EB2:
        push   3
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x15b
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_06EC
        and    dword ptr [ebp - 8], 0xffffff00
        mov    esi, 3
        jmp    loc_1_3093
loc_1_2ED8:
        mov    esi, 0x44ec                   ; -> loc_1_44EC
        lea    edi, [ebp - 0x40]
        mov    ecx, 9
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0x4514                   ; -> loc_1_4514
        lea    edi, [ebp - 0x68]
        mov    ecx, 9
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x134]
        cmp    eax, 1
        je     loc_1_2F18
        cmp    eax, 2
        je     loc_1_2F18
        lea    edi, [ebp - 0x68]
        mov    esi, 0x25
        jmp    loc_1_3093
loc_1_2F18:
        lea    edi, [ebp - 0x40]
        mov    esi, 0x25
        jmp    loc_1_3093
loc_1_2F25:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_2F40
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x23d]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_3093
        db  090h
loc_1_2F40:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_3093
loc_1_2F51:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x82]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_3093
loc_1_2F62:
        mov    word ptr [ebp - 2], 0x30a
        lea    edi, [ebp - 2]
        mov    esi, 2
        jmp    loc_1_3093
loc_1_2F75:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x13c]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_3093
loc_1_2F86:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_3093
loc_1_2F92:
        mov    dword ptr [ebp - 8], 6
        jmp    loc_1_3093
        db  08Bh, 0FFh
loc_1_2FA0:
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 0x15b]
        add    ecx, 0x15b
        mov    byte ptr [ebp - 0x18], al
        mov    al, byte ptr [ecx + 1]
        mov    byte ptr [ebp - 0x17], al
        mov    al, byte ptr [ecx + 2]
        mov    byte ptr [ebp - 0x16], al
        lea    edi, [ebp - 0x18]
        mov    al, byte ptr [ecx + 3]
        mov    byte ptr [ebp - 0x15], al
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [ebp - 0x14], al
        mov    esi, 6
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 0x13], al
        jmp    loc_1_3093
        db  08Dh, 049h, 000h
loc_1_2FE0:
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 0x161]
        add    ecx, 0x161
        mov    byte ptr [ebp - 0x18], al
        mov    al, byte ptr [ecx + 1]
        mov    byte ptr [ebp - 0x17], al
        mov    al, byte ptr [ecx + 2]
        mov    byte ptr [ebp - 0x16], al
        lea    edi, [ebp - 0x18]
        mov    al, byte ptr [ecx + 3]
        mov    byte ptr [ebp - 0x15], al
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [ebp - 0x14], al
        mov    esi, 6
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 0x13], al
        jmp    loc_1_3093
        db  08Dh, 049h, 000h
loc_1_3020:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_3031
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_1_303D
loc_1_3031:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x23d]
        mov    eax, dword ptr [eax + 4]
loc_1_303D:
        mov    ecx, eax
        and    ecx, 0xff00
        mov    edx, eax
        shl    edx, 0x10
        or     ecx, edx
        shl    ecx, 8
        mov    edx, eax
        and    edx, 0xff0000
        shr    eax, 0x10
        or     edx, eax
        shr    edx, 8
        jmp    loc_1_308E
loc_1_3061:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x23d]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, eax
        shr    ecx, 0x10
        mov    edx, eax
        and    edx, 0xff0000
        or     ecx, edx
        shr    ecx, 8
        mov    edx, eax
        shl    edx, 0x10
        and    eax, 0xff00
        or     edx, eax
        shl    edx, 8
loc_1_308E:
        or     ecx, edx
        mov    dword ptr [ebp - 8], ecx
loc_1_3093:
        or     ebx, ebx
        jne    loc_1_30C1
        cmp    esi, dword ptr [ebp + 0x1c]
        jbe    loc_1_30B1
        mov    eax, dword ptr [ebp + 0x20]
        mov    ebx, 0xc0010014
        mov    dword ptr [eax], esi
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_1_30B1:
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_06EC
        mov    eax, dword ptr [ebp + 0x24]
        add    dword ptr [eax], esi
loc_1_30C1:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_1_30CC:
        sub    byte ptr [0x2da40000], 0      ; -> loc_1_2DA4
        add    dh, dh                        ; -> loc_1_2DF6
        sub    eax, 0x2e030000               ; -> loc_1_2E03
        add    byte ptr [eax], al
        adc    byte ptr [esi], ch
        add    byte ptr [eax], al
        and    dword ptr [esi], ebp
        add    byte ptr [eax], al
        xor    eax, 0x5800002e               ; -> loc_1_2E58
        add    byte ptr cs:[eax], al
        jo     loc_1_311C
        add    byte ptr [eax], al
        nop    
        add    byte ptr cs:[eax], al
        mov    eax, dword ptr [0xb200002e]   ; -> loc_1_2EB2
        add    byte ptr cs:[eax], al
        fsubr  dword ptr [esi]
        add    byte ptr [eax], al
        and    eax, 0x5100002f               ; -> loc_1_2F51
        das    
        add    byte ptr [eax], al
        bound  ebp, qword ptr [edi]
        add    byte ptr [eax], al
        jne    0x313d
        add    byte ptr [eax], al
        xchg   byte ptr [edi], ch
        add    byte ptr [eax], al
        xchg   edx, eax
        das    
        add    byte ptr [eax], al
        lea    esp, [esp]
loc_1_311C:
        lea    esp, [esp]
loc_1_3120:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [eax + 0xc7]
        sub    ecx, eax
        cmp    ecx, 0xc7
        je     loc_1_3167
        cmp    dword ptr [esi + 0x14], 0
        jne    loc_1_3167
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_0574
        push   0
        push   0x92
        push   esi
        call   sub_1_060C
loc_1_3167:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_3170:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        cmp    dword ptr [esi + 0x32], 0
        je     loc_1_31A0
        mov    dword ptr [ebp - 8], 0x103
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_31A0:
        sub    ebx, ebx
loc_1_31A2:
        mov    edi, dword ptr [esi + 0x2a]
        cmp    edi, ebx
        je     loc_1_343C
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x2a], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [edi + 0x10]
        dec    eax
        cmp    eax, 9
        ja     loc_1_31C8
        jmp    dword ptr [eax*4 + 0x3448]    ; -> obj1:0x3448
loc_1_31C8:
        mov    dword ptr [ebp - 8], 0xc0000001
        jmp    loc_1_33AC
loc_1_31D4:
        mov    eax, dword ptr [edi + 0x14]
        cmp    eax, 0x1010e
        je     loc_1_31F1
        cmp    eax, 0x2010103
        je     loc_1_324B
        cmp    eax, 0x2010104
        je     loc_1_3260
        jmp    loc_1_33AC
loc_1_31F1:
        mov    eax, dword ptr [edi + 8]
        push   eax
        push   esi
        call   sub_1_3AC0
        mov    dword ptr [ebp - 8], eax
        cmp    eax, ebx
        jne    loc_1_3240
        mov    word ptr [edi + 8], 2
        mov    eax, dword ptr [esi + 0x23d]
        test   byte ptr [eax + 0x11], 0x20
        je     loc_1_3227
        push   0x7fffffff
        push   esi
        call   sub_1_3A10
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_33AC
loc_1_3227:
        mov    eax, dword ptr [eax + 4]
        push   eax
        push   esi
        call   sub_1_3A10
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_33AC
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3240:
        mov    word ptr [edi + 8], 1
        jmp    loc_1_33AC
loc_1_324B:
        mov    eax, dword ptr [edi + 8]
        push   eax
        push   esi
        call   sub_1_3A10
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_33AC
        db  08Dh, 049h, 000h
loc_1_3260:
        mov    eax, dword ptr [edi + 8]
        push   eax
        push   esi
        call   sub_1_3960
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_33AC
loc_1_3272:
        mov    eax, dword ptr [edi + 8]
        push   eax
        push   esi
        call   sub_1_3AC0
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_33AC
loc_1_3284:
        mov    eax, dword ptr [edi + 8]
        push   eax
        push   esi
        call   sub_1_3A10
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_33AC
loc_1_3296:
        mov    eax, dword ptr [edi + 8]
        push   eax
        push   esi
        call   sub_1_3960
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_33AC
loc_1_32A8:
        mov    eax, dword ptr [esi + 0x167]
        mov    cx, word ptr [eax]
        mov    word ptr [ebp - 4], cx
loc_1_32B5:
        cmp    word ptr [ebp - 4], bx
        jne    loc_1_32B5
        mov    word ptr [eax], 0xa00
        cmp    byte ptr [esi + 0x127], bl
        je     loc_1_3355
        mov    eax, dword ptr [esi + 0x18b]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x167]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 0x18b]
        jmp    loc_1_3385
loc_1_3301:
        mov    eax, dword ptr [esi + 0x167]
        mov    cx, word ptr [eax]
        mov    word ptr [ebp - 2], cx
        mov    edi, edi
loc_1_3310:
        cmp    word ptr [ebp - 2], bx
        jne    loc_1_3310
        mov    word ptr [eax], 0xa00
        cmp    byte ptr [esi + 0x127], bl
        je     loc_1_3355
        mov    eax, dword ptr [esi + 0x18b]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x167]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 0x18b]
        jmp    loc_1_3385
loc_1_3355:
        mov    eax, dword ptr [esi + 0x18f]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x167]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 0x18f]
loc_1_3385:
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [esi + 0x167]
        mov    dword ptr [ebp - 8], 0x103
        mov    word ptr [eax + 4], cx
loc_1_33AC:
        cmp    dword ptr [ebp - 8], ebx
        jne    loc_1_3420
        mov    eax, dword ptr [edi + 0x10]
        cmp    eax, 1
        je     loc_1_33D0
        cmp    eax, 4
        je     loc_1_33F0
        cmp    eax, 9
        je     loc_1_3400
        cmp    eax, 0xa
        je     loc_1_3410
        jmp    loc_1_31A2
        db  08Dh, 049h, 000h
loc_1_33D0:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   edi
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_058C
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_31A2
        db  08Dh, 064h, 024h, 000h
loc_1_33F0:
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_31A2
        db  005h, 000h, 000h, 000h, 000h
loc_1_3400:
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_31A2
        db  005h, 000h, 000h, 000h, 000h
loc_1_3410:
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_31A2
        db  005h, 000h, 000h, 000h, 000h
loc_1_3420:
        cmp    dword ptr [ebp - 8], 0x103
        jne    loc_1_31A2
        mov    dword ptr [esi + 0x32], edi
        push   esi
        push   0x1900                        ; -> loc_1_1900
        push   esi
        call   sub_1_05FA
loc_1_343C:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        ; ---- dispatch table (10 entries) ----
        dd  loc_1_31D4
        dd  loc_1_32A8
        dd  loc_1_31C8
        dd  loc_1_3272
        dd  loc_1_31C8
        dd  loc_1_31C8
        dd  loc_1_31C8
        dd  loc_1_3301
        dd  loc_1_3284
        dd  loc_1_3296
sub_1_3470:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x32]
        or     edi, edi
        je     loc_1_363D
        mov    dword ptr [esi + 0x32], 0
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [edi + 0x10]
        dec    eax
        cmp    eax, 9
        ja     loc_1_362B
        jmp    dword ptr [eax*4 + 0x3650]    ; -> obj1:0x3650
loc_1_34A8:
        mov    eax, dword ptr [edi + 0x14]
        cmp    eax, 0x1010e
        je     loc_1_34F2
        cmp    eax, 0x2010103
        je     loc_1_3571
        cmp    eax, 0x2010104
        je     loc_1_3590
        mov    bl, byte ptr [ebp + 0xc]
loc_1_34CB:
        cmp    dword ptr [esi + 0x32], 0
        jne    loc_1_363D
        cmp    bl, 1
        sbb    eax, eax
        and    eax, 0xc0000001
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   edi
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_058C
        jmp    loc_1_362B
loc_1_34F2:
        mov    bl, byte ptr [ebp + 0xc]
        or     bl, bl
        je     loc_1_34CB
        xor    eax, eax
        mov    ax, word ptr [edi + 8]
        cmp    eax, 1
        je     loc_1_3510
        cmp    eax, 2
        je     loc_1_3560
        jmp    loc_1_34CB
        db  005h, 000h, 000h, 000h, 000h
loc_1_3510:
        mov    ax, word ptr [esi + 0x10c]
        mov    word ptr [esi + 0x102], ax
        mov    eax, dword ptr [esi + 0x23d]
        test   byte ptr [eax + 0x11], 0x20
        je     loc_1_3531
        mov    eax, 0x7fffffff
        jmp    loc_1_3534
loc_1_3531:
        mov    eax, dword ptr [eax + 4]
loc_1_3534:
        push   eax
        push   esi
        call   sub_1_3A10
        cmp    eax, 0x103
        jne    loc_1_34CB
        mov    dword ptr [esi + 0x32], edi
        mov    word ptr [edi + 8], 2
        push   esi
        push   0x1900                        ; -> loc_1_1900
        push   esi
        call   sub_1_05FA
        jmp    loc_1_34CB
        db  08Dh, 064h, 024h, 000h
loc_1_3560:
        mov    eax, dword ptr [esi + 0x10e]
        mov    dword ptr [esi + 0x104], eax
        jmp    loc_1_34CB
loc_1_3571:
        mov    bl, byte ptr [ebp + 0xc]
        or     bl, bl
        je     loc_1_34CB
        mov    eax, dword ptr [esi + 0x10e]
        mov    dword ptr [esi + 0x104], eax
        jmp    loc_1_34CB
        db  08Dh, 049h, 000h
loc_1_3590:
        mov    bl, byte ptr [ebp + 0xc]
        or     bl, bl
        je     loc_1_34CB
        mov    eax, dword ptr [esi + 0x112]
        mov    dword ptr [esi + 0x108], eax
        jmp    loc_1_34CB
        db  08Dh, 064h, 024h, 000h
loc_1_35B0:
        cmp    byte ptr [ebp + 0xc], 0
        mov    eax, 0xc0000001
        je     loc_1_35CA
        push   esi
        call   sub_1_3B90
        push   edi
        push   esi
        call   sub_1_2BD0
        sub    eax, eax
loc_1_35CA:
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 0xfc]
        push   eax
        call   sub_1_05AA
        jmp    loc_1_362B
loc_1_35DA:
        cmp    byte ptr [ebp + 0xc], 0
        je     loc_1_362B
        mov    ax, word ptr [esi + 0x10c]
        mov    word ptr [esi + 0x102], ax
        jmp    loc_1_362B
loc_1_35F0:
        cmp    byte ptr [ebp + 0xc], 0
        je     loc_1_35FC
        push   esi
        call   sub_1_3B90
loc_1_35FC:
        mov    byte ptr [esi + 0x1cb], 0
        jmp    loc_1_362B
loc_1_3605:
        cmp    byte ptr [ebp + 0xc], 0
        je     loc_1_362B
        mov    eax, dword ptr [esi + 0x10e]
        mov    dword ptr [esi + 0x104], eax
        jmp    loc_1_362B
loc_1_3619:
        cmp    byte ptr [ebp + 0xc], 0
        je     loc_1_362B
        mov    eax, dword ptr [esi + 0x112]
        mov    dword ptr [esi + 0x108], eax
loc_1_362B:
        cmp    dword ptr [esi + 0x32], 0
        jne    loc_1_363D
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_363D
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 0x14]
loc_1_363D:
        cmp    dword ptr [esi + 0x32], 1
        sbb    al, al
        neg    al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
        ; ---- dispatch table (10 entries) ----
        dd  loc_1_34A8
        dd  loc_1_35B0
        dd  loc_1_362B
        dd  loc_1_35DA
        dd  loc_1_362B
        dd  loc_1_362B
        dd  loc_1_362B
        dd  loc_1_35F0
        dd  loc_1_3605
        dd  loc_1_3619
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3680:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0x11a], 0
        je     loc_1_3698
        mov    eax, 0xc001000d
        pop    ebp
        ret    8
loc_1_3698:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 0x2a], 0
        je     loc_1_36C0
        mov    eax, dword ptr [eax + 0x2e]
        pop    ebp
        mov    dword ptr [eax], ecx
        mov    eax, 0x103
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_36C0:
        mov    dword ptr [eax + 0x2e], ecx
        mov    dword ptr [eax + 0x2a], ecx
        mov    eax, 0x103
        pop    ebp
        ret    8
        db  090h
sub_1_36D0:
        push   ebp
        mov    ebp, esp
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 8]
        push   ecx
        mov    eax, dword ptr [eax + 0x23d]
        push   eax
        call   sub_2_0360
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3700:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ecx + 8]
        or     esi, esi
        jne    loc_1_371B
        mov    dword ptr [ecx + 0x7a], 0xa
        lea    esi, [ecx + 0x6a]
loc_1_371B:
        cmp    byte ptr [edx + 0x11a], 0
        je     loc_1_3730
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_1_3730:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [edx + 0x108], eax
        jne    loc_1_3742
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
loc_1_3742:
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi + 8], eax
        inc    dword ptr [ecx + 0x14]
        mov    dword ptr [esi], 0
        cmp    dword ptr [edx + 0x2a], 0
        je     loc_1_3766
        mov    eax, dword ptr [edx + 0x2e]
        mov    dword ptr [eax], esi
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    0x14
loc_1_3766:
        mov    dword ptr [edx + 0x2e], esi
        mov    dword ptr [edx + 0x2a], esi
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3780:
        push   ebp
        mov    ebp, esp
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 8]
        push   ecx
        mov    eax, dword ptr [eax + 0x23d]
        push   eax
        call   sub_2_02C0
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_37B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ecx + 8]
        or     esi, esi
        jne    loc_1_37CB
        mov    dword ptr [ecx + 0x52], 9
        lea    esi, [ecx + 0x42]
loc_1_37CB:
        cmp    byte ptr [edx + 0x11a], 0
        je     loc_1_37E0
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_1_37E0:
        mov    eax, dword ptr [edx + 0x23d]
        test   byte ptr [eax + 0x11], 0x20
        je     loc_1_3800
        cmp    dword ptr [edx + 0x104], 0x7fffffff
        jne    loc_1_3800
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
        db  090h
loc_1_3800:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [edx + 0x104], eax
        jne    loc_1_3812
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
loc_1_3812:
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 4], ecx
        inc    dword ptr [ecx + 0x14]
        mov    dword ptr [esi], 0
        cmp    dword ptr [edx + 0x2a], 0
        je     loc_1_3836
        mov    eax, dword ptr [edx + 0x2e]
        mov    dword ptr [eax], esi
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    0x14
loc_1_3836:
        mov    dword ptr [edx + 0x2e], esi
        mov    dword ptr [edx + 0x2a], esi
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3850:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x14]
        test   al, 0x46
        je     loc_1_3863
        mov    eax, 0xc00000bb
        pop    ebp
        ret    0x10
loc_1_3863:
        push   1
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x23d]
        push   eax
        call   sub_2_04F0
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3890:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0x14]
        or     edi, edi
        mov    ebx, dword ptr [esi + 8]
        jne    loc_1_38AD
        mov    dword ptr [esi + 0x2a], 4
        lea    edi, [esi + 0x1a]
loc_1_38AD:
        cmp    byte ptr [ebx + 0x11a], 0
        je     loc_1_38C2
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
loc_1_38C2:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   ebx
        call   sub_1_3B30
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 0x102]
        and    eax, 0xffff
        cmp    ecx, eax
        jne    loc_1_3920
        test   byte ptr [ebp + 0xd], 0x20
        mov    eax, 0x7fffffff
        jne    loc_1_38F2
        mov    eax, dword ptr [ebx + 0x23d]
        mov    eax, dword ptr [eax + 4]
loc_1_38F2:
        cmp    dword ptr [ebx + 0x104], eax
        je     loc_1_3915
        mov    word ptr [edi + 8], 2
        mov    cl, byte ptr [ebp + 0x18]
        push   ecx
        push   edi
        push   esi
        push   eax
        push   0
        call   sub_1_37B0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
loc_1_3915:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_1_3920:
        cmp    dword ptr [edi + 0x10], 4
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edi + 8], eax
        jne    loc_1_392C
loc_1_392C:
        mov    eax, 0x103
        mov    dword ptr [edi + 4], esi
        inc    dword ptr [esi + 0x14]
        mov    dword ptr [edi], 0
        cmp    dword ptr [ebx + 0x2a], 0
        je     loc_1_3950
        mov    ecx, dword ptr [ebx + 0x2e]
        mov    dword ptr [ecx], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
        db  090h
loc_1_3950:
        mov    dword ptr [ebx + 0x2e], edi
        mov    dword ptr [ebx + 0x2a], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
        db  08Dh, 049h, 000h
sub_1_3960:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x108], eax
        jne    loc_1_3980
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_3980:
        mov    eax, dword ptr [esi + 0x167]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 2], ax
        sub    eax, eax
loc_1_3990:
        cmp    word ptr [ebp - 2], ax
        jne    loc_1_3990
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x112], eax
        mov    eax, dword ptr [esi + 0x167]
        push   4
        mov    word ptr [eax], 0x800
        lea    eax, [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [esi + 0x167]
        add    eax, 2
        push   eax
        call   sub_1_06EC
        mov    eax, dword ptr [esi + 0x167]
        add    eax, 2
        push   eax
        call   sub_1_0C70
        mov    eax, dword ptr [esi + 0x167]
        add    eax, 4
        push   eax
        call   sub_1_0C70
        mov    eax, dword ptr [esi + 0x167]
        mov    dx, word ptr [eax + 2]
        mov    cx, word ptr [eax + 4]
        mov    word ptr [eax + 2], cx
        mov    eax, dword ptr [esi + 0x167]
        pop    esi
        mov    word ptr [eax + 4], dx
        mov    eax, 0x103
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x104], eax
        jne    loc_1_3A30
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_3A30:
        mov    eax, dword ptr [esi + 0x167]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 2], ax
        sub    eax, eax
loc_1_3A40:
        cmp    word ptr [ebp - 2], ax
        jne    loc_1_3A40
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x10e], eax
        mov    eax, dword ptr [esi + 0x167]
        push   4
        mov    word ptr [eax], 0x900
        lea    eax, [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [esi + 0x167]
        add    eax, 2
        push   eax
        call   sub_1_06EC
        mov    eax, dword ptr [esi + 0x167]
        add    eax, 2
        push   eax
        call   sub_1_0C70
        mov    eax, dword ptr [esi + 0x167]
        add    eax, 4
        push   eax
        call   sub_1_0C70
        mov    eax, dword ptr [esi + 0x167]
        mov    dx, word ptr [eax + 2]
        mov    cx, word ptr [eax + 4]
        mov    word ptr [eax + 2], cx
        mov    eax, dword ptr [esi + 0x167]
        pop    esi
        mov    word ptr [eax + 4], dx
        mov    eax, 0x103
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3AC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_3B30
        cmp    word ptr [esi + 0x102], ax
        jne    loc_1_3AE6
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3AE6:
        mov    ecx, dword ptr [esi + 0x167]
        mov    cx, word ptr [ecx]
        mov    word ptr [ebp - 2], cx
        sub    ecx, ecx
loc_1_3AF5:
        cmp    word ptr [ebp - 2], cx
        jne    loc_1_3AF5
        mov    ecx, dword ptr [esi + 0x167]
        mov    word ptr [esi + 0x10c], ax
        mov    word ptr [ecx], 0xd00
        mov    ecx, dword ptr [esi + 0x167]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [esi + 0x167]
        pop    esi
        mov    word ptr [eax + 4], 0
        mov    eax, 0x103
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_1_3B30:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   esi
        mov    dword ptr [ebp - 0x14], 0x10
        mov    word ptr [ebp - 0x10], 0x200
        mov    dword ptr [ebp - 0xe], 0x8000
        sub    si, si
        mov    word ptr [ebp - 0xa], 0x400
        mov    dword ptr [ebp - 8], 0x20
        mov    word ptr [ebp - 4], 0x8218
        mov    ax, 0x21
        mov    edx, dword ptr [ebp + 0xc]
loc_1_3B68:
        mov    ecx, esi
        and    ecx, 0xffff
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        test   dword ptr [ebp + ecx - 0x14], edx
        je     loc_1_3B80
        or     ax, word ptr [ebp + ecx - 0x10]
loc_1_3B80:
        inc    si
        cmp    si, 3
        jb     loc_1_3B68
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_1_3B90:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x18b]
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        add    dword ptr [ecx + 0x1dc], eax
        mov    eax, dword ptr [ecx + 0x18b]
        mov    al, byte ptr [eax + 3]
        and    eax, 0xff
        add    dword ptr [ecx + 0x1e4], eax
        mov    eax, dword ptr [ecx + 0x18b]
        mov    al, byte ptr [eax + 6]
        and    eax, 0xff
        add    dword ptr [ecx + 0x1f0], eax
        mov    eax, dword ptr [ecx + 0x18b]
        mov    al, byte ptr [eax + 7]
        and    eax, 0xff
        add    dword ptr [ecx + 0x1f4], eax
        mov    eax, dword ptr [ecx + 0x18b]
        mov    al, byte ptr [eax + 8]
        and    eax, 0xff
        add    dword ptr [ecx + 0x1f8], eax
        mov    eax, dword ptr [ecx + 0x18b]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        add    dword ptr [ecx + 0x200], eax
        mov    eax, dword ptr [ecx + 0x18b]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        add    dword ptr [ecx + 0x204], eax
        mov    eax, dword ptr [ecx + 0x18b]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        add    dword ptr [ecx + 0x208], eax
        mov    eax, dword ptr [ecx + 0x18b]
        pop    ebp
        mov    al, byte ptr [eax + 0xd]
        and    eax, 0xff
        add    dword ptr [ecx + 0x20c], eax
        ret    4
        db  08Bh, 0FFh
sub_1_3C50:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        sub    edi, edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], ecx
        mov    esi, dword ptr [eax + 0x193]
        cmp    ecx, edi
        mov    ebx, dword ptr [eax + 0x19f]
        je     loc_1_3CB4
loc_1_3C74:
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0654
        cmp    dword ptr [ebp - 4], ebx
        ja     loc_1_3CC8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   esi
        call   sub_1_06EC
        add    esi, dword ptr [ebp - 4]
        add    edi, dword ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 4]
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0678
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_3C74
loc_1_3CB4:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x19b], edi
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3CC8:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3CE0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 8]
        cmp    byte ptr [edx + 0x11c], 0
        je     loc_1_3D00
        mov    eax, 0xc0000001
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_3D00:
        cmp    byte ptr [edx + 0x11a], 0
        je     loc_1_3D13
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    8
loc_1_3D13:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x28], esi
        mov    dword ptr [ecx + 0x2c], ecx
        lea    eax, [ecx + 0x24]
        mov    dword ptr [eax], 0
        cmp    dword ptr [edx + 0x36], 0
        jne    loc_1_3D30
        mov    dword ptr [edx + 0x36], ecx
        jmp    loc_1_3D36
loc_1_3D30:
        mov    eax, dword ptr [edx + 0x3a]
        mov    dword ptr [eax + 0x24], ecx
loc_1_3D36:
        mov    dword ptr [edx + 0x3a], ecx
        inc    dword ptr [esi + 0x14]
        push   0
        push   edx
        call   sub_1_3D50
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_1_3D50:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x84
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x36]
        or     edi, edi
        je     loc_1_42A5
        cmp    dword ptr [esi + 0x52], 0
        je     loc_1_42A5
        cmp    dword ptr [esi + 0x3e], 0
        jne    loc_1_42A5
        mov    dword ptr [esi + 0x3e], edi
        mov    ecx, dword ptr [edi + 0x24]
        lea    eax, [edi + 0x24]
        mov    dword ptr [esi + 0x36], ecx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x34], eax
        lea    eax, [ebp - 0x30]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x2c]
        push   eax
        push   edi
        call   sub_1_0630
        and    dword ptr [ebp - 0x2c], 0x7fffffff
        dec    dword ptr [esi + 0x52]
        mov    edx, dword ptr [esi + 0x4e]
        mov    dword ptr [ebp - 0x24], edx
        mov    eax, dword ptr [ebp - 0x34]
        mov    ebx, edx
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], edi
        mov    ax, word ptr [ebp - 0x30]
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [edx + 0xc]
        mov    word ptr [eax + 6], cx
        cmp    byte ptr [esi + 0x127], 0
        mov    eax, dword ptr [esi + 0x5a]
        jne    loc_1_3FE4
        imul   eax, dword ptr [esi + 0x52]
        cmp    eax, dword ptr [ebp - 0x2c]
        jb     loc_1_3FA0
        cmp    byte ptr [esi + 0x15a], 0
        jne    loc_1_3FA0
        mov    word ptr [ebp - 6], 0
        cmp    dword ptr [ebp - 0x18], 0
        mov    word ptr [ebp - 2], 0
        mov    edi, dword ptr [ebp - 0x28]
        jbe    loc_1_3F94
loc_1_3E27:
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [eax + 0xc]
        or     ecx, ecx
        je     loc_1_3E45
        mov    eax, dword ptr [eax + 4]
        add    ecx, eax
        dec    ecx
        shr    ecx, 0xc
        shr    eax, 0xc
        sub    ecx, eax
        inc    ecx
        mov    dword ptr [ebp - 0x10], ecx
        jmp    loc_1_3E4C
loc_1_3E45:
        mov    dword ptr [ebp - 0x10], 0
loc_1_3E4C:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_3F58
        mov    eax, dword ptr [ebp - 0x14]
        xor    ecx, ecx
        mov    cx, word ptr [ebp - 2]
        mov    dword ptr [ebx + ecx*8 + 0x18], eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x84]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_065A
        cmp    dword ptr [ebp - 0x10], 0
        mov    word ptr [ebp - 4], 0
        jbe    loc_1_3F58
loc_1_3E87:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        lea    ecx, [eax*8]
        mov    eax, dword ptr [ebp + ecx - 0x84]
        xor    edx, edx
        mov    dx, word ptr [ebp - 2]
        shr    eax, 0x10
        lea    edx, [edx + edx*2]
        add    edx, edx
        mov    di, ax
        and    edi, 0xffff00ff
        shl    di, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    di, ax
        mov    eax, dword ptr [ebx + 0xc]
        mov    word ptr [edx + eax + 0xa], di
        mov    ax, word ptr [ebp + ecx - 0x84]
        movzx  di, ah
        and    ax, 0xff
        shl    ax, 8
        add    di, ax
        mov    eax, dword ptr [ebx + 0xc]
        mov    word ptr [edx + eax + 0xc], di
        mov    ax, word ptr [ebp + ecx - 0x80]
        mov    cx, ax
        shr    cx, 8
        or     cl, 0x80
        and    cx, 0xff
        and    ax, 0xff
        inc    word ptr [ebp - 4]
        shl    ax, 8
        inc    word ptr [ebp - 2]
        add    cx, ax
        mov    eax, dword ptr [ebx + 0xc]
        mov    word ptr [edx + eax + 8], cx
        mov    eax, dword ptr [ebx + 0xc]
        lea    edi, [edx + eax + 8]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        cmp    dword ptr [ebp - 0x10], ecx
        ja     loc_1_3F38
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        cmp    eax, dword ptr [ebp - 0x18]
        jae    loc_1_3F4F
loc_1_3F38:
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        cmp    eax, dword ptr [esi + 0x5a]
        jne    loc_1_3F4F
        mov    word ptr [ebp - 2], 0
        dec    dword ptr [esi + 0x52]
        mov    ebx, dword ptr [ebx + 8]
loc_1_3F4F:
        cmp    dword ptr [ebp - 0x10], ecx
        ja     loc_1_3E87
loc_1_3F58:
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_0678
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_1_3F77
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        inc    eax
        cmp    eax, dword ptr [ebp - 0x18]
        jb     loc_1_3F94
loc_1_3F77:
        xor    eax, eax
        inc    word ptr [ebp - 6]
        mov    ax, word ptr [ebp - 6]
        cmp    eax, dword ptr [ebp - 0x18]
        jb     loc_1_3E27
        and    word ptr [edi], 0xff7f
        jmp    loc_1_419A
loc_1_3F94:
        and    word ptr [edi], 0xff7f
        jmp    loc_1_419A
        db  08Bh, 0FFh
loc_1_3FA0:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        push   esi
        call   sub_1_3C50
        or     al, al
        je     loc_1_419A
        mov    eax, dword ptr [esi + 0x197]
        shr    eax, 0x10
        mov    cl, ah
        and    ecx, 0xffff00ff
        and    ax, 0xff
        shl    ax, 8
        mov    edx, dword ptr [ebp - 0x24]
        add    cx, ax
        mov    eax, dword ptr [edx + 0xc]
        mov    word ptr [eax + 0xa], cx
        mov    ax, word ptr [esi + 0x197]
        jmp    loc_1_4159
loc_1_3FE4:
        imul   eax, dword ptr [esi + 0x52]
        cmp    eax, dword ptr [ebp - 0x18]
        jb     loc_1_411B
        cmp    byte ptr [esi + 0x15a], 0
        jne    loc_1_411B
        mov    word ptr [ebp - 6], 0
        mov    word ptr [ebp - 2], 0
        mov    edi, dword ptr [ebp - 0x28]
loc_1_4010:
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        cmp    eax, dword ptr [ebp - 0x18]
        jae    loc_1_4111
        lea    eax, [ebp - 0x1c]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_0654
        cmp    dword ptr [ebp - 0x1c], 0
        je     loc_1_40E6
        mov    eax, dword ptr [ebp - 0x20]
        xor    ecx, ecx
        mov    cx, word ptr [ebp - 2]
        shr    eax, 0x10
        lea    ecx, [ecx + ecx*2]
        lea    edx, [ecx*2]
        mov    cl, ah
        and    ecx, 0xffff00ff
        and    ax, 0xff
        shl    ax, 8
        add    cx, ax
        mov    eax, dword ptr [ebx + 0xc]
        mov    word ptr [edx + eax + 0xa], cx
        mov    ax, word ptr [ebp - 0x20]
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [ebx + 0xc]
        mov    word ptr [edx + eax + 0xc], cx
        mov    ax, word ptr [ebp - 0x1c]
        mov    cx, ax
        shr    cx, 8
        or     cl, 0x80
        and    cx, 0xff
        and    ax, 0xff
        inc    word ptr [ebp - 6]
        shl    ax, 8
        inc    word ptr [ebp - 2]
        add    cx, ax
        mov    eax, dword ptr [ebx + 0xc]
        mov    word ptr [edx + eax + 8], cx
        mov    eax, dword ptr [ebx + 0xc]
        lea    edi, [edx + eax + 8]
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        cmp    eax, dword ptr [ebp - 0x18]
        jae    loc_1_40EA
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        cmp    eax, dword ptr [esi + 0x5a]
        jne    loc_1_40EA
        mov    word ptr [ebp - 2], 0
        dec    dword ptr [esi + 0x52]
        mov    ebx, dword ptr [ebx + 8]
        jmp    loc_1_40EA
loc_1_40E6:
        inc    word ptr [ebp - 6]
loc_1_40EA:
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_0678
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_1_4010
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        inc    eax
        cmp    eax, dword ptr [ebp - 0x18]
        jae    loc_1_4010
loc_1_4111:
        and    word ptr [edi], 0xff7f
        jmp    loc_1_419A
loc_1_411B:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        push   esi
        call   sub_1_3C50
        or     al, al
        je     loc_1_419A
        mov    eax, dword ptr [esi + 0x193]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        mov    edx, dword ptr [ebp - 0x24]
        add    cx, ax
        mov    eax, dword ptr [edx + 0xc]
        mov    word ptr [eax + 0xa], cx
        mov    ax, word ptr [esi + 0x193]
loc_1_4159:
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [edx + 0xc]
        mov    word ptr [eax + 0xc], cx
        mov    ax, word ptr [esi + 0x19b]
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [edx + 0xc]
        mov    word ptr [eax + 8], cx
loc_1_419A:
        mov    eax, dword ptr [ebx + 0xc]
        mov    edx, dword ptr [ebp - 0x24]
        or     word ptr [eax + 4], 0x10
        mov    eax, dword ptr [ebx + 0xc]
        or     word ptr [eax + 2], 0x100
        mov    dword ptr [esi + 0x4a], ebx
        lea    ecx, [esi + 0x167]
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 0x4e], eax
        mov    eax, dword ptr [edx + 0xc]
        mov    bl, byte ptr [ebp + 0xc]
        or     word ptr [eax + 4], 0xa1
        mov    eax, dword ptr [ecx]
        or     bl, bl
        mov    ax, word ptr [eax]
        je     loc_1_41E0
        mov    word ptr [ebp - 0xa], ax
        sub    eax, eax
loc_1_41D8:
        cmp    word ptr [ebp - 0xa], ax
        jne    loc_1_41D8
        jmp    loc_1_41EC
loc_1_41E0:
        mov    word ptr [ebp - 8], ax
        sub    eax, eax
loc_1_41E6:
        cmp    word ptr [ebp - 8], ax
        jne    loc_1_41E6
loc_1_41EC:
        mov    eax, dword ptr [ecx]
        mov    word ptr [eax], 0x400
        cmp    byte ptr [esi + 0x127], 0
        mov    eax, dword ptr [esi + 0x46]
        je     loc_1_4240
        mov    eax, dword ptr [eax + 0xc]
        shr    eax, 0x10
        mov    dl, al
        and    edx, 0xffff00ff
        shl    dx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    dx, ax
        mov    eax, dword ptr [ecx]
        mov    word ptr [eax + 2], dx
        mov    eax, dword ptr [esi + 0x46]
        mov    ax, word ptr [eax + 0xc]
        mov    dl, al
        and    edx, 0xffff00ff
        shl    dx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        jmp    loc_1_4279
        db  08Dh, 049h, 000h
loc_1_4240:
        mov    eax, dword ptr [eax + 0x10]
        shr    eax, 0x10
        mov    dl, al
        and    edx, 0xffff00ff
        shl    dx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    dx, ax
        mov    eax, dword ptr [ecx]
        mov    word ptr [eax + 2], dx
        mov    eax, dword ptr [esi + 0x46]
        mov    ax, word ptr [eax + 0x10]
        mov    dl, ah
        and    edx, 0xffff00ff
        and    ax, 0xff
        shl    ax, 8
loc_1_4279:
        add    dx, ax
        mov    eax, dword ptr [ecx]
        or     bl, bl
        mov    word ptr [eax + 4], dx
        push   esi
        push   0x1900                        ; -> loc_1_1900
        push   esi
        je     loc_1_42A0
        call   sub_1_05FA
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_42A0:
        call   sub_1_05FA
loc_1_42A5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 054h, 068h, 06Fh, 06Dh, 061h, 073h, 02Dh, 043h, 06Fh, 06Eh, 072h, 061h, 064h, 020h
        db  054h, 06Fh, 06Bh, 065h, 06Eh, 020h, 052h, 069h, 06Eh, 067h, 020h, 044h, 072h, 069h, 076h, 065h
        db  072h, 020h, 066h, 06Fh, 072h, 020h, 04Eh, 044h, 049h, 053h, 020h, 033h, 02Eh, 030h, 02Ch, 020h
        db  028h, 043h, 029h, 020h, 043h, 06Fh, 070h, 079h, 072h, 069h, 067h, 068h, 074h, 020h, 031h, 039h
        db  039h, 030h, 02Dh, 039h, 034h, 020h, 054h, 068h, 06Fh, 06Dh, 061h, 073h, 02Dh, 043h, 06Fh, 06Eh
        db  072h, 061h, 064h, 02Ch, 020h, 049h, 06Eh, 063h, 02Eh, 02Ch, 020h, 041h, 06Ch, 06Ch, 020h, 052h
        db  069h, 067h, 068h, 074h, 073h, 020h, 052h, 065h, 073h, 065h, 072h, 076h, 065h, 064h, 02Ch, 020h
        db  031h, 02Eh, 030h, 033h, 028h, 039h, 034h, 030h, 037h, 030h, 036h, 029h, 000h, 000h
loc_1_432C:
        push   esp
        arpl   word ptr [edi + ebp*2 + 0x6b], si
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x75], al
        jae    loc_1_438C
        jns    loc_1_43AA
        db  065h, 000h
loc_1_433C:
        inc    edx
        jne    0x43b2
        push   esp
        jns    0x43b2
        add    byte ptr gs:[ebx + 0x70], dl
        add    byte ptr fs:[eax], al
        add    byte ptr [ebx + 0x70], dl
        add    byte ptr fs:[eax], al
        add    byte ptr [ebx + 0x6c], dl
        outsd  dx, dword ptr [esi]
        je     loc_1_43A7
        jne    0x43c8
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
loc_1_4360:
        push   ebx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        je     0x43b3
        jne    0x43d4
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        inc    esp
        insd   dword ptr es:[edi], dx
        popal  
        inc    ebx
        push   0x656e6e61
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
loc_1_4378:
        inc    esp
        insd   dword ptr es:[edi], dx
        popal  
        inc    ebx
        push   0x656e6e61
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        dec    ecx
        outsb  dx, byte ptr [esi]
        je     0x43ed
        jb     loc_1_43FC
        jne    loc_1_43FC
loc_1_438C:
        je     loc_1_438E
loc_1_438E:
        add    byte ptr [eax], al
loc_1_4390:
        dec    ecx
        outsb  dx, byte ptr [esi]
        je     0x43f9
        jb     loc_1_4408
        jne    loc_1_4408
        je     loc_1_439A
loc_1_439A:
        add    byte ptr [eax], al
        dec    ecx
        outsd  dx, dword ptr [esi]
        inc    edx
        popal  
        jae    0x4407
        add    byte ptr [eax], al
loc_1_43A4:
        dec    ecx
        outsd  dx, dword ptr [esi]
        inc    edx
loc_1_43A7:
        popal  
        jae    0x440f
loc_1_43AA:
        add    byte ptr [eax], al
        dec    esi
        je     0x43f1
        jb     0x4419
        jae    loc_1_4429
        add    byte ptr [eax], al
loc_1_43B8:
        dec    esi
        je     0x43fd
        jb     0x4425
        jae    loc_1_4435
        add    byte ptr [eax], al
        push   eax
        popal  
        arpl   word ptr [ebx + 0x65], bp
        je     loc_1_441E
        imul   edi, dword ptr [edx + 0x65], 0x61500000
        arpl   word ptr [ebx + 0x65], bp
        je     0x442a
        imul   edi, dword ptr [edx + 0x65], 0x72500000
        outsd  dx, dword ptr [esi]
        jne    0x4445
        je     loc_1_442D
        add    byte ptr fs:[eax], al
        add    byte ptr [eax + 0x72], dl
        outsd  dx, dword ptr [esi]
        jne    loc_1_4451
        je     loc_1_4439
        add    byte ptr fs:[eax], al
        add    byte ptr [ebp + 0x61], ah
        insb   byte ptr es:[di], dx
        arpl   word ptr gs:[ecx + 0x66], sp
loc_1_43FC:
        bound  ebp, qword ptr cs:[ecx + 0x6e]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4404:
        popal  
        insb   byte ptr es:[di], dx
loc_1_4408:
        arpl   word ptr gs:[ecx + 0x66], sp
        bound  ebp, qword ptr cs:[ecx + 0x6e]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4414:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
loc_1_441E:
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 0x6000101
loc_1_4429:
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
loc_1_442D:
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
loc_1_4435:
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
loc_1_4439:
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0xe000101
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [eax], dl
loc_1_4451:
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
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
        add    eax, 0x1020101
        add    dword ptr [edx], eax
        add    al, byte ptr [edx]
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    dword ptr [ecx], eax
        add    byte ptr [edx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], al
        add    dword ptr [eax], eax
        add    eax, 0x6000101
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
        or     eax, 0xe000101
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [eax], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
loc_1_44EC:
        push   esp
        push   0x73616d6f
        sub    eax, 0x726e6f43
        popal  
        and    byte ptr fs:[edi + ebp*2 + 0x6b], dl
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [edx + 0x69], dl
        outsb  dx, byte ptr [esi]
        and    byte ptr [bx + di + 0x64], al
        popal  
        jo     0x457e
        jb     loc_1_453C
        inc    ecx
        push   esp
        add    byte ptr cs:[eax], al
        add    byte ptr [eax], al
loc_1_4514:
        push   esp
        push   0x73616d6f
        sub    eax, 0x726e6f43
        popal  
        and    byte ptr fs:[edi + ebp*2 + 0x6b], dl
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [edx + 0x69], dl
        outsb  dx, byte ptr [esi]
        and    byte ptr [bx + di + 0x64], al
        popal  
        jo     0x45a6
        jb     0x4564
        dec    ebp
        inc    ebx
        add    byte ptr cs:[eax], al
        add    byte ptr [eax], al
loc_1_453C:
        add    byte ptr [eax], al
loc_1_453E:
        add    byte ptr [eax], al
loc_1_4540:
        push   es
        add    byte ptr [esi], al
        add    byte ptr [ebx + eax*2], ch    ; -> loc_1_432C
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x570  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        mov    eax, dword ptr [0x4548]       ; -> obj1:0x4548
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   0x32
        push   eax
        call   sub_1_0606
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
        call   sub_1_060C
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
        mov    eax, dword ptr [0x4548]       ; -> obj1:0x4548
        push   eax
        push   0
        push   0x1f
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0606
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
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_04F0
        mov    edi, eax
        or     edi, edi
        je     loc_2_01F9
        cmp    edi, 0x103
        jne    loc_2_0211
loc_2_01F9:
        push   0
        push   0x453c                        ; -> loc_1_453C
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        push   ebx
        call   sub_2_02C0
        or     eax, eax
        je     loc_2_0211
        mov    edi, eax
loc_2_0211:
        or     edi, edi
        je     loc_2_021D
        cmp    edi, 0x103
        jne    loc_2_0248
loc_2_021D:
        cmp    byte ptr [esi + 0x1d], 0
        je     loc_2_0248
        dec    dword ptr [ebx + 0xc]
        mov    byte ptr [esi + 0x1d], 0
        cmp    dword ptr [ebx + 0xc], 0
        jne    loc_2_0248
        push   0
        push   0x453c                        ; -> loc_1_453C
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        push   ebx
        call   sub_2_0360
        or     eax, eax
        je     loc_2_0248
        mov    edi, eax
loc_2_0248:
        or     edi, edi
        je     loc_2_0254
        cmp    edi, 0x103
        jne    loc_2_02B5
loc_2_0254:
        mov    eax, dword ptr [esi + 0x18]
        dec    eax
        mov    dword ptr [esi + 0x18], eax
        jne    loc_2_02B0
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_026A
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_026A:
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_2_0279
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_027F
loc_2_0279:
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [ebx + 0x14], eax
loc_2_027F:
        cmp    byte ptr [esi + 0x1e], 0
        je     loc_2_028E
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0598
loc_2_028E:
        mov    eax, 1
        mov    cl, byte ptr [esi + 0x1c]
        shl    eax, cl
        or     dword ptr [ebx + 0x2e], eax
        push   0
        push   0x1f
        push   esi
        call   sub_1_060C
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_2_02B0:
        mov    edi, 0xc001000e
loc_2_02B5:
        mov    eax, edi
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
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        xor    edx, edx
        mov    ebx, dword ptr [esi + 4]
        mov    ecx, dword ptr [edi + 0xc]
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
        mov    dword ptr [edi + 0xc], eax
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [esi + 4], 0
        or     eax, eax
        je     loc_2_0320
loc_2_0313:
        mov    ecx, dword ptr [eax + 0xc]
        or     dword ptr [esi + 4], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_0313
loc_2_0320:
        mov    ecx, dword ptr [esi + 4]
        cmp    ebx, ecx
        je     loc_2_0354
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 0x1e]
        or     eax, eax
        je     loc_2_0356
        cmp    eax, 0x103
        je     loc_2_0356
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0xc], ecx
        mov    dword ptr [esi + 4], ebx
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
        mov    al, byte ptr [0x453c]         ; -> loc_1_453C
        cmp    byte ptr [ecx], al
        jne    loc_2_0408
        mov    al, byte ptr [0x453d]         ; -> obj1:0x453D
        cmp    byte ptr [ecx + 1], al
        jne    loc_2_0408
        mov    al, byte ptr [0x453e]         ; -> loc_1_453E
        cmp    byte ptr [ecx + 2], al
        jne    loc_2_0408
        mov    al, byte ptr [0x453f]         ; -> obj1:0x453F
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

VxD__LTEXT SEGMENT   ; object 4  vsize=0x1ab0  R-X
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0x14]
        cmp    byte ptr [eax + 0x127], 0
        je     loc_4_003A
        mov    dword ptr [edi], 0
        push   0xffffff
        push   0
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        call   sub_1_0606
        or     eax, eax
        je     loc_4_0070
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
loc_4_003A:
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        push   0
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    eax, dword ptr [eax + 0xf8]
        push   eax
        call   sub_1_05C2
        cmp    dword ptr [esi], 0
        jne    loc_4_0060
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
loc_4_0060:
        cmp    dword ptr [edi], 0xffffff
        jbe    loc_4_0070
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
loc_4_0070:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_0080:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 0x16b]
        push   eax
        lea    eax, [esi + 0x167]
        push   eax
        push   6
        push   esi
        call   0
        or     al, al
        jne    loc_4_00B1
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_00B1:
        lea    eax, [esi + 0x177]
        push   eax
        lea    eax, [esi + 0x173]
        push   eax
        push   8
        push   esi
        call   0
        or     al, al
        jne    loc_4_00D6
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_00D6:
        lea    eax, [esi + 0x187]
        push   eax
        lea    eax, [esi + 0x183]
        push   eax
        push   0x12
        push   esi
        call   0
        or     al, al
        jne    loc_4_0100
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_0100:
        lea    eax, [esi + 0x18f]
        push   eax
        lea    eax, [esi + 0x18b]
        push   eax
        push   0xe
        push   esi
        call   0
        or     al, al
        jne    loc_4_0125
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_0125:
        mov    eax, dword ptr [esi + 0x13c]
        mov    dword ptr [esi + 0x19f], eax
        lea    edi, [esi + 0x197]
        lea    ebx, [esi + 0x193]
        push   edi
        push   ebx
        push   eax
        push   esi
        call   0
        or     al, al
        jne    loc_4_0155
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_0155:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 0x17b], eax
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x17f], eax
        lea    eax, [esi + 0x7e]
        push   eax
        lea    eax, [esi + 0x7a]
        push   eax
        mov    eax, dword ptr [esi + 0x76]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    eax, ecx
        push   eax
        push   esi
        call   0
        or     al, al
        jne    loc_4_0190
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_0190:
        push   0
        mov    eax, dword ptr [esi + 0x76]
        shl    eax, 2
        push   0
        lea    eax, [eax + eax*8]
        push   eax
        lea    eax, [esi + 0x6a]
        push   eax
        call   sub_1_0606
        or     eax, eax
        je     loc_4_01B6
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_01B6:
        mov    ebx, dword ptr [esi + 0x82]
        imul   ebx, dword ptr [esi + 0x86]
        imul   ebx, dword ptr [esi + 0x76]
        lea    eax, [esi + 0x8e]
        push   eax
        lea    eax, [esi + 0x8a]
        push   eax
        push   ebx
        push   esi
        call   0
        or     al, al
        jne    loc_4_01F0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_01F0:
        mov    edx, dword ptr [esi + 0x76]
        shl    edx, 2
        mov    edi, dword ptr [esi + 0x6a]
        lea    edx, [edx + edx*8]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    edx, dword ptr [esi + 0x76]
        mov    eax, edx
        lea    edx, [edx + edx*4]
        mov    edi, dword ptr [esi + 0x7a]
        lea    edx, [edx + edx*4]
        add    edx, eax
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    edi, dword ptr [esi + 0x8a]
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    edx, dword ptr [esi + 0x6a]
        mov    dword ptr [ebp - 8], edx
        mov    eax, dword ptr [esi + 0x7a]
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [esi + 0x7e]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [esi + 0x8a]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [esi + 0x8e]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0xc], 0
        cmp    dword ptr [esi + 0x76], 0
        jbe    loc_4_04D2
loc_4_0275:
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [edx + 8], eax
        mov    eax, edx
        add    eax, 4
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebp - 0x20]
        mov    dword ptr [eax], ecx
        sub    ecx, ecx
        cmp    dword ptr [esi + 0x86], ecx
        jbe    loc_4_038A
        add    edx, 0xc
        sub    eax, eax
        mov    edi, edi
loc_4_02A0:
        mov    edi, dword ptr [esi + 0x86]
        sub    edi, ecx
        cmp    edi, 1
        jne    loc_4_02B6
        mov    bx, word ptr [esi + 0x82]
        jmp    loc_4_02C1
loc_4_02B6:
        mov    ebx, dword ptr [esi + 0x82]
        or     bx, 0x8000
loc_4_02C1:
        mov    di, bx
        and    edi, 0xffff00ff
        shl    di, 8
        mov    bl, bh
        and    ebx, 0xffff00ff
        add    di, bx
        mov    ebx, dword ptr [ebp - 0x20]
        mov    word ptr [eax + ebx + 8], di
        cmp    byte ptr [esi + 0x127], 0
        je     loc_4_0310
        mov    ebx, dword ptr [ebp - 0x18]
        shr    ebx, 0x10
        movzx  di, bh
        and    bx, 0xff
        shl    bx, 8
        add    di, bx
        mov    ebx, dword ptr [ebp - 0x20]
        mov    word ptr [eax + ebx + 0xa], di
        mov    bx, word ptr [ebp - 0x18]
        jmp    loc_4_033A
        db  08Bh, 0FFh
loc_4_0310:
        mov    ebx, dword ptr [ebp - 0x14]
        shr    ebx, 0x10
        mov    di, bx
        and    edi, 0xffff00ff
        shl    di, 8
        mov    bl, bh
        and    ebx, 0xffff00ff
        add    di, bx
        mov    ebx, dword ptr [ebp - 0x20]
        mov    word ptr [eax + ebx + 0xa], di
        mov    bx, word ptr [ebp - 0x14]
loc_4_033A:
        mov    di, bx
        and    edi, 0xffff00ff
        shl    di, 8
        mov    bl, bh
        and    ebx, 0xffff00ff
        add    di, bx
        mov    ebx, dword ptr [ebp - 0x20]
        mov    word ptr [eax + ebx + 0xc], di
        mov    edi, dword ptr [ebp - 0x18]
        mov    dword ptr [edx], edi
        add    edx, 8
        mov    edi, dword ptr [ebp - 0x14]
        mov    dword ptr [edx - 4], edi
        mov    edi, dword ptr [esi + 0x82]
        add    dword ptr [ebp - 0x18], edi
        add    eax, 6
        mov    edi, dword ptr [esi + 0x82]
        add    dword ptr [ebp - 0x14], edi
        inc    ecx
        cmp    dword ptr [esi + 0x86], ecx
        ja     loc_4_02A0
loc_4_038A:
        mov    eax, dword ptr [esi + 0x76]
        dec    eax
        cmp    eax, dword ptr [ebp - 0xc]
        jbe    loc_4_0444
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [edx + 8], eax
        add    dword ptr [ebp - 0x1c], 0x1a
        add    dword ptr [ebp - 0x20], 0x1a
        cmp    byte ptr [esi + 0x127], 0
        je     loc_4_03F0
        mov    eax, dword ptr [ebp - 0x20]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax], cx
        mov    ax, word ptr [ebp - 0x20]
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        jmp    loc_4_0428
        db  08Bh, 0FFh
loc_4_03F0:
        mov    eax, dword ptr [ebp - 0x1c]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax], cx
        mov    ax, word ptr [ebp - 0x1c]
        mov    cl, ah
        and    ecx, 0xffff00ff
        and    ax, 0xff
        shl    ax, 8
loc_4_0428:
        add    cx, ax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax + 2], cx
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x24
        mov    dword ptr [edx], eax
        mov    dword ptr [ebp - 8], eax
        jmp    loc_4_04C3
loc_4_0444:
        cmp    byte ptr [esi + 0x127], 0
        je     loc_4_0477
        mov    eax, dword ptr [esi + 0x7a]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax], cx
        mov    ax, word ptr [esi + 0x7a]
        jmp    loc_4_049C
loc_4_0477:
        mov    eax, dword ptr [esi + 0x7e]
        shr    eax, 0x10
        mov    cl, ah
        and    ecx, 0xffff00ff
        and    ax, 0xff
        shl    ax, 8
        add    cx, ax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax], cx
        mov    ax, word ptr [esi + 0x7e]
loc_4_049C:
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax + 2], cx
        mov    eax, dword ptr [esi + 0x6a]
        mov    dword ptr [edx], eax
loc_4_04C3:
        inc    dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [esi + 0x76], eax
        ja     loc_4_0275
loc_4_04D2:
        lea    eax, [esi + 0x62]
        push   eax
        lea    eax, [esi + 0x5e]
        push   eax
        mov    eax, dword ptr [esi + 0x56]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    eax, ecx
        push   eax
        push   esi
        call   0
        or     al, al
        jne    loc_4_0500
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_4_0500:
        push   0
        mov    eax, dword ptr [esi + 0x56]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        push   0
        lea    eax, [eax + eax*8]
        sub    eax, ecx
        push   eax
        lea    eax, [esi + 0x42]
        push   eax
        call   sub_1_0606
        or     eax, eax
        je     loc_4_052A
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_052A:
        mov    edx, dword ptr [esi + 0x56]
        mov    eax, edx
        lea    edx, [edx + edx*4]
        mov    edi, dword ptr [esi + 0x42]
        lea    edx, [edx + edx*8]
        sub    edx, eax
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    edx, dword ptr [esi + 0x56]
        mov    eax, edx
        lea    edx, [edx + edx*4]
        mov    edi, dword ptr [esi + 0x5e]
        lea    edx, [edx + edx*4]
        add    edx, eax
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    edx, dword ptr [esi + 0x42]
        mov    dword ptr [esi + 0x4e], edx
        mov    dword ptr [esi + 0x46], edx
        mov    eax, dword ptr [esi + 0x56]
        mov    edi, 0x1a
        mov    dword ptr [esi + 0x52], eax
        mov    eax, dword ptr [esi + 0x5e]
        mov    dword ptr [ebp - 0x28], eax
        mov    eax, dword ptr [esi + 0x62]
        mov    dword ptr [ebp - 0x24], eax
        sub    eax, eax
loc_4_058C:
        mov    ecx, dword ptr [ebp - 0x28]
        mov    dword ptr [edx + 0xc], ecx
        cmp    byte ptr [esi + 0x127], 0
        jne    loc_4_05B9
        mov    ecx, dword ptr [ebp - 0x24]
        mov    dword ptr [edx + 0x10], ecx
        sub    ecx, ecx
        cmp    dword ptr [esi + 0x5a], ecx
        jbe    loc_4_05BC
        lea    ebx, [edx + 0x14]
loc_4_05AB:
        mov    dword ptr [ebx], eax
        inc    eax
        add    ebx, 8
        inc    ecx
        cmp    dword ptr [esi + 0x5a], ecx
        ja     loc_4_05AB
        jmp    loc_4_05BC
loc_4_05B9:
        add    eax, dword ptr [esi + 0x5a]
loc_4_05BC:
        mov    ecx, dword ptr [esi + 0x5a]
        imul   ecx, dword ptr [esi + 0x56]
        cmp    ecx, eax
        jbe    loc_4_0655
        add    dword ptr [ebp - 0x28], edi
        add    dword ptr [ebp - 0x24], edi
        cmp    byte ptr [esi + 0x127], 0
        je     loc_4_0603
        mov    ecx, dword ptr [ebp - 0x28]
        shr    ecx, 0x10
        mov    bl, cl
        and    ebx, 0xffff00ff
        shl    bx, 8
        mov    cl, ch
        and    ecx, 0xffff00ff
        add    bx, cx
        mov    ecx, dword ptr [edx + 0xc]
        mov    word ptr [ecx], bx
        mov    cx, word ptr [ebp - 0x28]
        jmp    loc_4_062A
loc_4_0603:
        mov    ecx, dword ptr [ebp - 0x24]
        shr    ecx, 0x10
        mov    bl, cl
        and    ebx, 0xffff00ff
        shl    bx, 8
        mov    cl, ch
        and    ecx, 0xffff00ff
        add    bx, cx
        mov    ecx, dword ptr [edx + 0xc]
        mov    word ptr [ecx], bx
        mov    cx, word ptr [ebp - 0x24]
loc_4_062A:
        mov    bl, cl
        and    ebx, 0xffff00ff
        shl    bx, 8
        mov    cl, ch
        and    ecx, 0xffff00ff
        add    bx, cx
        mov    ecx, dword ptr [edx + 0xc]
        mov    word ptr [ecx + 2], bx
        lea    ecx, [edx + 0x2c]
        mov    dword ptr [edx + 8], ecx
        mov    edx, ecx
        jmp    loc_4_058C
loc_4_0655:
        cmp    byte ptr [esi + 0x127], 0
        je     loc_4_0686
        mov    eax, dword ptr [esi + 0x5e]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [edx + 0xc]
        mov    word ptr [eax], cx
        mov    ax, word ptr [esi + 0x5e]
        jmp    loc_4_06AC
loc_4_0686:
        mov    eax, dword ptr [esi + 0x62]
        shr    eax, 0x10
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [edx + 0xc]
        mov    word ptr [eax], cx
        mov    ax, word ptr [esi + 0x62]
loc_4_06AC:
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, ah
        and    eax, 0xffff00ff
        add    cx, ax
        mov    eax, dword ptr [edx + 0xc]
        lea    ebx, [esi + 0x1a]
        mov    word ptr [eax + 2], cx
        mov    eax, dword ptr [esi + 0x42]
        mov    dword ptr [edx + 8], eax
        push   0
        push   0
        push   0x8c
        push   ebx
        call   sub_1_0606
        or     eax, eax
        je     loc_4_06F0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_06F0:
        mov    edi, dword ptr [ebx]
        sub    eax, eax
        mov    ecx, 0x23
        rep stosd dword ptr es:[edi], eax
        mov    edx, dword ptr [ebx]
        mov    eax, 9
loc_4_0702:
        lea    ecx, [edx + 0xe]
        mov    dword ptr [edx], ecx
        mov    edx, ecx
        dec    eax
        jne    loc_4_0702
        mov    eax, dword ptr [ebx]
        mov    dword ptr [edx - 0xe], eax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 0x22], eax
        mov    dword ptr [esi + 0x1e], eax
        mov    eax, dword ptr [esi + 0x173]
        mov    dword ptr [eax], 0
        mov    ecx, 8
        mov    word ptr [eax + 4], 0
        mov    eax, dword ptr [esi + 0x173]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        sub    eax, eax
        mov    edi, dword ptr [esi + 0x17b]
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [esi + 0x183]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0xc], 0
        mov    word ptr [eax + 0x10], 0
        mov    eax, dword ptr [esi + 0x18b]
        pop    edi
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    word ptr [eax + 0xc], 0
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_4_07A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x44
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_054A
        push   0
        push   0
        push   8
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_0606
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    loc_4_0890
        mov    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 0x44], 3
        mov    byte ptr [ebp - 0x43], 0xa
        mov    dword ptr [ebp - 0x3e], 0x8a0 ; -> loc_4_08A0
        mov    dword ptr [ebp - 0x3a], 0x12b0 ; -> loc_1_12B0
        mov    dword ptr [ebp - 0x36], 0x3ce0 ; -> loc_1_3CE0
        mov    dword ptr [ebp - 0x32], 0x2780 ; -> loc_1_2780
        mov    dword ptr [ebp - 0x2e], 0x1440 ; -> loc_1_1440
        mov    dword ptr [ebp - 0x2a], 0x2a10 ; -> loc_1_2A10
        mov    dword ptr [ebp - 0x26], 0x2ab0 ; -> loc_1_2AB0
        mov    dword ptr [ebp - 0x22], 0x18d0 ; -> loc_1_18D0
        mov    dword ptr [ebp - 0x1e], 0xa70 ; -> loc_4_0A70
        mov    dword ptr [ebp - 0x1a], 0x1250 ; -> sub_1_1250
        mov    eax, dword ptr [0x4540]       ; -> loc_1_4540
        mov    ecx, dword ptr [0x4544]       ; -> obj1:0x4544
        mov    dword ptr [ebp - 0x16], eax
        lea    eax, [ebp - 0x44]
        mov    dword ptr [ebp - 0x12], ecx
        push   0x36
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0556
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_0870
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_4_0870:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0550
        mov    dword ptr [ebp - 4], 0xc0000001
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_4_0890:
        mov    dword ptr [ebp - 4], 0xc000009a
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_08A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    edx, edx
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [ebp + 0x18]
        cmp    ecx, edx
        je     loc_4_08D1
        mov    eax, dword ptr [ebp + 0x14]
        lea    esp, [esp]
loc_4_08C0:
        cmp    dword ptr [eax], 1
        je     loc_4_08CD
        add    eax, 4
        inc    edx
        cmp    ecx, edx
        ja     loc_4_08C0
loc_4_08CD:
        cmp    ecx, edx
        jne    loc_4_08E0
loc_4_08D1:
        mov    eax, 0xc0010019
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 049h, 000h
loc_4_08E0:
        mov    eax, dword ptr [ebp + 0x10]
        push   0
        mov    dword ptr [eax], edx
        push   0
        lea    eax, [ebp - 4]
        push   0x92
        push   eax
        call   sub_1_0606
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    loc_4_0A40
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 0x20]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x18], 0
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [eax + 0x18]
        mov    byte ptr [eax + 0x19], cl
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0x23d]
        push   eax
        call   sub_2_00F0
        or     al, al
        je     loc_4_0A05
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [esi + 0xbf]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0xc3]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [esi + 0xc3]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0xc3], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x14]
        cmp    byte ptr [esi + 0x11d], 0
        je     loc_4_09F4
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x18], 1
        mov    dword ptr [ebp - 8], 0x103
        cmp    byte ptr [esi + 0x11b], 0
        jne    loc_4_09E3
        mov    byte ptr [esi + 0x11b], 1
        mov    word ptr [esi + 0xb1], 3
        push   esi
        call   sub_1_0940
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_09E3:
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_09F4:
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_0A05:
        push   0
        push   0xaa000002
        mov    eax, dword ptr [esi + 0xf8]
        push   eax
        call   sub_1_06D8
        add    esp, 0xc
        push   0
        push   0x92
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_060C
        mov    dword ptr [ebp - 8], 0xc0000001
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 064h, 024h, 000h
loc_4_0A40:
        push   0
        mov    esi, dword ptr [ebp + 0x20]
        push   0xaa000002
        mov    eax, dword ptr [esi + 0xf8]
        push   eax
        call   sub_1_06D8
        add    esp, 0xc
        mov    dword ptr [ebp - 8], 0xc000009a
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_0A70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x68
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    word ptr [ebp - 0x68], 8
        mov    word ptr [ebp - 0x66], 8
        mov    dword ptr [ebp - 0x64], 0x433c ; -> loc_1_433C
        mov    word ptr [ebp - 0x40], 6
        mov    word ptr [ebp - 0x3e], 6
        mov    dword ptr [ebp - 0x3c], 0x434c ; -> obj1:0x434C
        mov    word ptr [ebp - 0x60], 0xb
        mov    word ptr [ebp - 0x5e], 0xb
        mov    dword ptr [ebp - 0x5c], 0x4360 ; -> loc_1_4360
        mov    word ptr [ebp - 0x48], 0xb
        mov    word ptr [ebp - 0x46], 0xb
        mov    dword ptr [ebp - 0x44], 0x4378 ; -> loc_1_4378
        mov    word ptr [ebp - 0x58], 0xa
        mov    word ptr [ebp - 0x56], 0xa
        mov    dword ptr [ebp - 0x54], 0x4390 ; -> loc_1_4390
        mov    word ptr [ebp - 0x50], 7
        mov    word ptr [ebp - 0x4e], 7
        mov    dword ptr [ebp - 0x4c], 0x43a4 ; -> loc_1_43A4
        mov    word ptr [ebp - 0x38], 0xb
        mov    word ptr [ebp - 0x36], 0xb
        mov    dword ptr [ebp - 0x34], 0x43b8 ; -> loc_1_43B8
        mov    word ptr [ebp - 0x30], 0xb
        mov    word ptr [ebp - 0x2e], 0xb
        mov    dword ptr [ebp - 0x2c], 0x43d0 ; -> obj1:0x43D0
        mov    word ptr [ebp - 0x28], 0xa
        mov    word ptr [ebp - 0x26], 0xa
        mov    dword ptr [ebp - 0x24], 0x43e8 ; -> obj1:0x43E8
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0684
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_10E9
        push   0
        push   0
        push   0x241
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0606
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    loc_4_10E0
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0x90
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        push   eax
        lea    eax, [ebp - 0x68]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_068A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_10C4
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x134], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x134]
        cmp    eax, 3
        jne    loc_4_0CB0
        push   0
        lea    eax, [ebp - 0x60]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_068A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_10C4
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x138], eax
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x138
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05B0
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_10C4
        cmp    word ptr [ebp - 0x18], 0x51
        jne    loc_4_0CA0
        mov    byte ptr [ebp - 0x20], 9
        mov    byte ptr [ebp - 0x1f], 0xa
        mov    byte ptr [ebp - 0x1e], 0xb
        mov    byte ptr [ebp - 0x1d], 0xc
        mov    byte ptr [ebp - 0x1c], 0xe
        mov    byte ptr [ebp - 0x1b], 0xf
        mov    byte ptr [ebp - 0x1a], 3
        mov    byte ptr [ebp - 0x19], 4
        mov    al, byte ptr [ebp - 0x15]
        and    eax, 7
        mov    al, byte ptr [ebp + eax - 0x20]
        mov    ecx, dword ptr [ebp - 4]
        and    eax, 0xff
        mov    dword ptr [ecx + 0x128], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x12c], 2
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x123], 0
        mov    al, byte ptr [ebp - 0x16]
        and    al, 0xe
        shr    al, 1
        and    eax, 0xff
        shl    eax, 0xc
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 0x1a20
        mov    dword ptr [ecx + 0x11f], eax
        mov    al, byte ptr [ebp - 0x15]
        and    al, 0xf0
        cmp    al, 0xf0
        mov    eax, dword ptr [ebp - 4]
        jne    loc_4_0C90
        jmp    loc_4_0D93
        db  090h
loc_4_0C90:
        mov    byte ptr [eax + 0x127], 0
        jmp    loc_4_0D9A
        db  08Dh, 064h, 024h, 000h
loc_4_0CA0:
        mov    dword ptr [ebp - 8], 0xc0000001
        jmp    loc_4_0D9A
        db  08Dh, 064h, 024h, 000h
loc_4_0CB0:
        cmp    eax, 1
        je     loc_4_0CD8
        cmp    eax, 2
        je     loc_4_0CD8
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0xaa00000a
        mov    eax, dword ptr [eax + 0xf8]
        push   eax
        call   sub_1_06D8
        add    esp, 0xc
        jmp    loc_4_0D9A
loc_4_0CD8:
        push   0
        lea    eax, [ebp - 0x58]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_068A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_10C4
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x128], eax
        mov    eax, dword ptr [ebp - 4]
        push   1
        mov    dword ptr [eax + 0x12c], 1
        lea    eax, [ebp - 0x50]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_068A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_10C4
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x11f], eax
        lea    eax, [ebp - 0x48]
        push   0
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_068A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_10C4
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x123], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x123], 0
        je     loc_4_0D90
        mov    byte ptr [eax + 0x127], 0
        jmp    loc_4_0D9A
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_0D90:
        mov    eax, dword ptr [ebp - 4]
loc_4_0D93:
        mov    byte ptr [eax + 0x127], 1
loc_4_0D9A:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_10CA
        push   0
        lea    eax, [ebp - 0x40]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_068A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_10CA
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [eax + 4], 4
        mov    eax, dword ptr [ebp - 4]
        jne    loc_4_0DE0
        mov    dword ptr [eax + 0x130], 4
        jmp    loc_4_0DEA
        db  08Dh, 049h, 000h
loc_4_0DE0:
        mov    dword ptr [eax + 0x130], 0x10
loc_4_0DEA:
        push   2
        lea    eax, [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_068A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_0F2B
        mov    eax, dword ptr [ebp - 0xc]
        sub    ecx, ecx
        mov    esi, dword ptr [eax + 8]
        mov    di, word ptr [eax + 4]
        cmp    di, cx
        je     loc_4_0EF6
loc_4_0E20:
        cmp    ecx, 6
        jge    loc_4_0EF9
        mov    dl, byte ptr [esi]
        cmp    dl, 0x30
        jl     loc_4_0E35
        cmp    dl, 0x39
        jle    loc_4_0E49
loc_4_0E35:
        cmp    dl, 0x61
        jl     loc_4_0E3F
        cmp    dl, 0x66
        jle    loc_4_0E49
loc_4_0E3F:
        cmp    dl, 0x41
        jl     loc_4_0E50
        cmp    dl, 0x46
        jg     loc_4_0E50
loc_4_0E49:
        mov    eax, 1
        jmp    loc_4_0E52
loc_4_0E50:
        sub    eax, eax
loc_4_0E52:
        or     eax, eax
        je     loc_4_0EF6
        mov    al, byte ptr [esi + 1]
        inc    esi
        cmp    al, 0x30
        jl     loc_4_0E66
        cmp    al, 0x39
        jle    loc_4_0E76
loc_4_0E66:
        cmp    al, 0x61
        jl     loc_4_0E6E
        cmp    al, 0x66
        jle    loc_4_0E76
loc_4_0E6E:
        cmp    al, 0x41
        jl     loc_4_0E80
        cmp    al, 0x46
        jg     loc_4_0E80
loc_4_0E76:
        mov    eax, 1
        jmp    loc_4_0E82
        db  08Dh, 049h, 000h
loc_4_0E80:
        sub    eax, eax
loc_4_0E82:
        or     eax, eax
        je     loc_4_0EF6
        cmp    dl, 0x61
        jl     loc_4_0E95
        cmp    dl, 0x66
        jg     loc_4_0E95
        sub    dl, 0x61
        jmp    loc_4_0EA7
loc_4_0E95:
        cmp    dl, 0x41
        jl     loc_4_0EA4
        cmp    dl, 0x46
        jg     loc_4_0EA4
        sub    dl, 0x41
        jmp    loc_4_0EA7
loc_4_0EA4:
        sub    dl, 0x30
loc_4_0EA7:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [ecx + eax + 0x140], dl
        mov    al, byte ptr [esi]
        cmp    al, 0x61
        jl     loc_4_0EC3
        cmp    al, 0x66
        jg     loc_4_0EC3
        movsx  eax, al
        sub    eax, 0x61
        jmp    loc_4_0ED9
loc_4_0EC3:
        cmp    al, 0x41
        jl     loc_4_0ED3
        cmp    al, 0x46
        jg     loc_4_0ED3
        movsx  eax, al
        sub    eax, 0x41
        jmp    loc_4_0ED9
loc_4_0ED3:
        movsx  eax, al
        sub    eax, 0x30
loc_4_0ED9:
        mov    edx, dword ptr [ebp - 4]
        shl    al, 4
        inc    esi
        add    byte ptr [edx + ecx + 0x140], al
        cmp    byte ptr [esi], 0x2d
        jne    loc_4_0EED
        inc    esi
loc_4_0EED:
        inc    ecx
        dec    di
        jne    loc_4_0E20
loc_4_0EF6:
        cmp    ecx, 6
loc_4_0EF9:
        jne    loc_4_0F2B
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [eax + 0x142]
        or     cl, byte ptr [eax + 0x141]
        or     cl, byte ptr [eax + 0x143]
        or     cl, byte ptr [eax + 0x144]
        or     cl, byte ptr [eax + 0x145]
        or     cl, byte ptr [eax + 0x140]
        je     loc_4_0F2B
        mov    byte ptr [eax + 0x146], 1
loc_4_0F2B:
        push   0
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_068A
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x130]
        sub    eax, 0x10
        cmp    eax, 1
        sbb    ecx, ecx
        and    ecx, 0x3390
        add    ecx, 0x1130
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_0F7B
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 0xe
        ja     loc_4_0F75
        mov    eax, 0xe
loc_4_0F75:
        cmp    ecx, eax
        jb     loc_4_0F7B
        mov    ecx, eax
loc_4_0F7B:
        mov    eax, dword ptr [ebp - 4]
        push   2
        mov    dword ptr [eax + 0x13c], ecx
        lea    eax, [ebp - 0x28]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_068A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_109E
        mov    eax, dword ptr [ebp - 0xc]
        sub    ecx, ecx
        mov    esi, dword ptr [eax + 8]
        mov    di, word ptr [eax + 4]
        cmp    di, cx
        je     loc_4_1090
        lea    ebx, [ebx]
loc_4_0FC0:
        cmp    ecx, 0x12
        jae    loc_4_1090
        mov    dl, byte ptr [esi]
        cmp    dl, 0x30
        jl     loc_4_0FD5
        cmp    dl, 0x39
        jle    loc_4_0FE9
loc_4_0FD5:
        cmp    dl, 0x61
        jl     loc_4_0FDF
        cmp    dl, 0x66
        jle    loc_4_0FE9
loc_4_0FDF:
        cmp    dl, 0x41
        jl     loc_4_0FF0
        cmp    dl, 0x46
        jg     loc_4_0FF0
loc_4_0FE9:
        mov    eax, 1
        jmp    loc_4_0FF2
loc_4_0FF0:
        sub    eax, eax
loc_4_0FF2:
        or     eax, eax
        je     loc_4_1090
        mov    al, byte ptr [esi + 1]
        inc    esi
        cmp    al, 0x30
        jl     loc_4_1006
        cmp    al, 0x39
        jle    loc_4_1016
loc_4_1006:
        cmp    al, 0x61
        jl     loc_4_100E
        cmp    al, 0x66
        jle    loc_4_1016
loc_4_100E:
        cmp    al, 0x41
        jl     loc_4_1020
        cmp    al, 0x46
        jg     loc_4_1020
loc_4_1016:
        mov    eax, 1
        jmp    loc_4_1022
        db  08Dh, 049h, 000h
loc_4_1020:
        sub    eax, eax
loc_4_1022:
        or     eax, eax
        je     loc_4_1090
        cmp    dl, 0x61
        jl     loc_4_1035
        cmp    dl, 0x66
        jg     loc_4_1035
        sub    dl, 0x61
        jmp    loc_4_1047
loc_4_1035:
        cmp    dl, 0x41
        jl     loc_4_1044
        cmp    dl, 0x46
        jg     loc_4_1044
        sub    dl, 0x41
        jmp    loc_4_1047
loc_4_1044:
        sub    dl, 0x30
loc_4_1047:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [ecx + eax + 0x147], dl
        mov    al, byte ptr [esi]
        cmp    al, 0x61
        jl     loc_4_1063
        cmp    al, 0x66
        jg     loc_4_1063
        movsx  eax, al
        sub    eax, 0x61
        jmp    loc_4_1079
loc_4_1063:
        cmp    al, 0x41
        jl     loc_4_1073
        cmp    al, 0x46
        jg     loc_4_1073
        movsx  eax, al
        sub    eax, 0x41
        jmp    loc_4_1079
loc_4_1073:
        movsx  eax, al
        sub    eax, 0x30
loc_4_1079:
        mov    edx, dword ptr [ebp - 4]
        shl    al, 4
        add    byte ptr [edx + ecx + 0x147], al
        inc    esi
        inc    ecx
        dec    di
        jne    loc_4_0FC0
loc_4_1090:
        or     ecx, ecx
        je     loc_4_109E
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x159], 1
loc_4_109E:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_4_1100
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     loc_4_10C4
        mov    dword ptr [ebp - 4], 0
loc_4_10C4:
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_10E0
loc_4_10CA:
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_10E0
        push   0
        push   0x241
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_060C
loc_4_10E0:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0690
loc_4_10E9:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1100:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x38
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        lea    eax, [esi + 0xb7]
        mov    dword ptr [esi + 0xbb], eax
        mov    dword ptr [eax], eax
        lea    eax, [esi + 0xbf]
        mov    dword ptr [esi + 0xc3], eax
        mov    dword ptr [eax], eax
        lea    eax, [esi + 0xc7]
        mov    dword ptr [esi + 0xcb], eax
        mov    dword ptr [eax], eax
        lea    eax, [esi + 0xcf]
        mov    dword ptr [esi + 0xd3], eax
        mov    dword ptr [eax], eax
        mov    byte ptr [esi + 0x11b], 0
        mov    byte ptr [esi + 0x11d], 1
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0xfc], edx
        lea    edi, [ebp - 0x38]
        mov    dword ptr [esi + 0x56], 0xa
        mov    dword ptr [esi + 0x5a], 3
        mov    dword ptr [esi + 0x76], 0xf
        mov    dword ptr [esi + 0x86], 3
        sub    eax, eax
        mov    dword ptr [esi + 0x82], 0x3f8
        mov    word ptr [esi + 0x100], 0x400
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dword ptr [ebp - 0x22], 1
        mov    eax, dword ptr [esi + 0x11f]
        mov    dword ptr [ebp - 0x1e], eax
        mov    dword ptr [ebp - 0x1a], 0xf
        mov    eax, dword ptr [esi + 0x134]
        cmp    eax, 1
        jl     loc_4_11F8
        cmp    eax, 2
        jle    loc_4_11D0
        cmp    eax, 3
        je     loc_4_11E6
        jmp    loc_4_11F8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_11D0:
        mov    byte ptr [ebp - 0x33], 0
        mov    eax, dword ptr [esi + 0x123]
        mov    dword ptr [ebp - 0x38], eax
        mov    dword ptr [ebp - 0x2e], 1
        jmp    loc_4_11F8
loc_4_11E6:
        mov    byte ptr [ebp - 0x33], 1
        mov    dword ptr [ebp - 0x38], 0
        mov    dword ptr [ebp - 0x2e], 3
loc_4_11F8:
        cmp    byte ptr [esi + 0x127], 0
        je     loc_4_1220
        mov    byte ptr [ebp - 0x34], 0
        mov    dword ptr [ebp - 0x38], 0
        mov    dword ptr [ebp - 0x2a], 0
        mov    dword ptr [ebp - 0x26], 0
        jmp    loc_4_1237
        db  08Dh, 064h, 024h, 000h
loc_4_1220:
        mov    byte ptr [ebp - 0x34], 1
        mov    eax, dword ptr [esi + 0x13c]
        mov    dword ptr [ebp - 0x26], eax
        mov    eax, dword ptr [esi + 0x5a]
        imul   eax, dword ptr [esi + 0x56]
        mov    dword ptr [ebp - 0x2a], eax
loc_4_1237:
        mov    dword ptr [ebp - 0x32], 0x1490 ; -> loc_4_1490
        lea    eax, [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        lea    edi, [esi + 0xf8]
        push   eax
        push   esi
        push   edx
        push   edi
        call   sub_1_0562
        mov    dword ptr [ebp - 0x10], eax
        or     eax, eax
        jne    loc_4_147E
        mov    byte ptr [esi + 0xe8], 1
        push   esi
        call   sub_4_0080
        or     al, al
        je     loc_4_1461
        mov    byte ptr [esi + 0x23c], 1
        mov    eax, dword ptr [esi + 0x12c]
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x128]
        push   eax
        push   eax
        push   0x1d70                        ; -> loc_1_1D70
        push   esi
        push   0x1c90                        ; -> loc_1_1C90
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [ebp - 0x10]
        push   esi
        push   eax
        call   sub_1_05EE
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_4_1449
        mov    byte ptr [esi + 0x18], 1
        push   esi
        call   sub_4_1830
        or     al, al
        je     loc_4_1440
        cmp    byte ptr [esi + 0xea], 1
        je     loc_4_1337
        mov    word ptr [ebp - 0xa], 0xa00
        mov    cx, word ptr [esi + 0x11f]
        mov    al, byte ptr [ebp - 0xa]
        lea    edx, [ecx + 0xc]
        out    dx, al
        lea    edx, [ecx + 0xd]
        mov    al, byte ptr [ebp - 9]
        mov    word ptr [ebp - 8], 1
        mov    cx, word ptr [esi + 0x11f]
        out    dx, al
        lea    edx, [ecx + 4]
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, byte ptr [ebp - 7]
        out    dx, al
        sub    ecx, ecx
loc_4_1305:
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 2
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 3
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    ax, word ptr [ebp - 2]
        mov    byte ptr [esi + ecx + 0x15b], ah
        inc    ecx
        cmp    ecx, 6
        jb     loc_4_1305
        jmp    loc_4_13BB
loc_4_1337:
        mov    word ptr [ebp - 6], 0xa00
        mov    cx, word ptr [esi + 0x11f]
        mov    al, byte ptr [ebp - 6]
        lea    edx, [ecx + 4]
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi + 0x11f]
        inc    dx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 4], ax
        mov    cx, word ptr [esi + 0x11f]
        lea    edx, [ecx + 4]
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        sub    ecx, ecx
loc_4_1381:
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 2
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 3
        in     al, dx
        mov    byte ptr [ebp - 1], al
        lea    edx, [esi + ecx]
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [edx + 0x15c], al
        mov    ax, word ptr [ebp - 2]
        mov    byte ptr [edx + 0x15b], ah
        add    ecx, 2
        cmp    ecx, 6
        jb     loc_4_1381
loc_4_13BB:
        cmp    byte ptr [esi + 0x146], 0
        lea    ecx, [esi + 0x161]
        jne    loc_4_13D2
        lea    eax, [esi + 0x15b]
        jmp    loc_4_13D8
loc_4_13D2:
        lea    eax, [esi + 0x140]
loc_4_13D8:
        mov    dl, byte ptr [eax]
        mov    byte ptr [ecx], dl
        mov    dl, byte ptr [eax + 1]
        mov    byte ptr [ecx + 1], dl
        mov    dl, byte ptr [eax + 2]
        mov    byte ptr [ecx + 2], dl
        lea    ebx, [esi + 0x23d]
        mov    dl, byte ptr [eax + 3]
        mov    byte ptr [ecx + 3], dl
        mov    dl, byte ptr [eax + 4]
        mov    byte ptr [ecx + 4], dl
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ecx + 5], al
        lea    eax, [esi + 0x238]
        push   ebx
        push   eax
        push   ecx
        push   0x3120                        ; -> loc_1_3120
        push   0x3890                        ; -> loc_1_3890
        push   0x3700                        ; -> loc_1_3700
        push   0x37b0                        ; -> sub_1_37B0
        call   sub_2_0000
        or     al, al
        je     loc_4_1431
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_1431:
        mov    dword ptr [ebx], 0
        push   2
        jmp    loc_4_1463
        db  005h, 000h, 000h, 000h, 000h
loc_4_1440:
        mov    dword ptr [ebp - 0x10], 0xc0000001
        jmp    loc_4_147E
loc_4_1449:
        push   1
        push   0
        push   2
        push   0xaa000005
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_06D8
        add    esp, 0x14
        jmp    loc_4_147E
loc_4_1461:
        push   3
loc_4_1463:
        push   0
        push   2
        push   0xaa000002
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_06D8
        add    esp, 0x14
        mov    dword ptr [ebp - 0x10], 0xc000009a
loc_4_147E:
        push   esi
        call   sub_1_1250
        mov    eax, dword ptr [ebp - 0x10]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_1490:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x5c
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 7], al
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 6], al
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 5], al
        mov    eax, dword ptr [ecx + 0x134]
        cmp    eax, 1
        je     loc_4_14F2
        cmp    eax, 2
        je     loc_4_14F2
        lea    esi, [ecx + 0xea]
        mov    byte ptr [esi], 3
        jmp    loc_4_17B9
loc_4_14F2:
        mov    si, 0x64
loc_4_14F6:
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    ax, word ptr [ebp - 4]
        and    word ptr [ebp - 8], ax
        or     word ptr [ebp - 6], ax
        dec    si
        jne    loc_4_14F6
        cmp    word ptr [ebp - 6], 0xffff
        je     loc_4_1805
        mov    ax, word ptr [ebp - 8]
        cmp    word ptr [ebp - 6], ax
        jne    loc_4_1805
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 0xe
        sub    al, al
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    ax, 0xff
        shl    ax, 8
        mov    word ptr [ebp - 4], ax
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 0xe
        mov    al, 1
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    dl, al
        and    edx, 0xffff00ff
        or     word ptr [ebp - 4], dx
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 0xf
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    al, 4
        mov    byte ptr [ecx + 0xe9], al
        cmp    word ptr [ebp - 4], 0x4045
        mov    edx, dword ptr [ecx + 0x11f]
        jne    loc_4_170A
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    ax, word ptr [ebp - 4]
        or     ax, 0x80
        mov    word ptr [ebp - 0xa], ax
        mov    di, word ptr [ecx + 0x11f]
        lea    esi, [edi + 8]
        mov    dx, si
        out    dx, al
        lea    edx, [edi + 9]
        mov    al, byte ptr [ebp - 9]
        out    dx, al
        mov    dx, si
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 3], al
        test   byte ptr [ebp - 4], 4
        je     loc_4_1606
        test   byte ptr [ebp - 3], 0xe0
        lea    esi, [ecx + 0xea]
        je     loc_4_1601
        mov    byte ptr [esi], 6
        jmp    loc_4_161A
loc_4_1601:
        mov    byte ptr [esi], 5
        jmp    loc_4_161A
loc_4_1606:
        test   byte ptr [ebp - 3], 0xe0
        lea    esi, [ecx + 0xea]
        je     loc_4_1617
        mov    byte ptr [esi], 7
        jmp    loc_4_161A
loc_4_1617:
        mov    byte ptr [esi], 4
loc_4_161A:
        mov    dword ptr [ebp - 0x1c], 1
        mov    dword ptr [ebp - 0x18], 5
        mov    dword ptr [ebp - 0x14], 6
        mov    dword ptr [ebp - 0x10], 7
        mov    dword ptr [ebp - 0x3c], 0xc
        mov    dword ptr [ebp - 0x38], 0xb
        mov    dword ptr [ebp - 0x34], 0xa
        mov    dword ptr [ebp - 0x30], 5
        mov    al, 3
        mov    dword ptr [ebp - 0x2c], 7
        mov    dword ptr [ebp - 0x28], 6
        mov    dword ptr [ebp - 0x24], 3
        mov    dword ptr [ebp - 0x20], 2
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 0xe
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    dl, al
        and    dl, 6
        shr    dl, 1
        mov    eax, dword ptr [ecx + 0x123]
        or     eax, eax
        je     loc_4_16C9
        and    edx, 0xff
        cmp    dword ptr [ebp + edx*4 - 0x1c], eax
        jne    loc_4_16A5
        cmp    byte ptr [ecx + 0xe9], 0
        je     loc_4_16C9
loc_4_16A5:
        push   0
        push   0xaa00000a
        mov    eax, dword ptr [ecx + 0xf8]
        push   eax
        call   sub_1_06D8
        add    esp, 0xc
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_16C9:
        mov    al, byte ptr [ebp - 2]
        and    al, 0x38
        shr    al, 3
        and    eax, 0xff
        mov    edx, dword ptr [ecx + 0x128]
        cmp    dword ptr [ebp + eax*4 - 0x3c], edx
        je     loc_4_17B9
        push   0
        push   0xaa00000a
        mov    eax, dword ptr [ecx + 0xf8]
        push   eax
        call   sub_1_06D8
        add    esp, 0xc
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_170A:
        add    dx, 0xf
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    bl, al
        and    bl, 0x2e
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 0xf
        or     al, 0x51
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 1], al
        xor    byte ptr [ebp - 1], 0x51
        xor    byte ptr [ebp - 1], bl
        test   byte ptr [ebp - 1], 0xf7
        je     loc_4_1762
        mov    byte ptr [ecx + 0xea], 1
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 9
        or     al, 0x28
        out    dx, al
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_1762:
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 0xf
        mov    al, bl
        and    al, 0x28
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 1], al
        xor    byte ptr [ebp - 1], bl
        test   byte ptr [ebp - 1], 0xf7
        je     loc_4_17B0
        mov    byte ptr [ecx + 0xea], 1
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 9
        or     al, 0x28
        out    dx, al
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
loc_4_17B0:
        lea    esi, [ecx + 0xea]
        mov    byte ptr [esi], 2
loc_4_17B9:
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 0xf
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    byte ptr [esi], 4
        je     loc_4_17E0
        cmp    dword ptr [ecx + 0x130], 0x10
        jne    loc_4_17E0
        and    byte ptr [ebp - 1], 0xdf
        jmp    loc_4_17E4
        db  005h, 000h, 000h, 000h, 000h
loc_4_17E0:
        or     byte ptr [ebp - 1], 0x20
loc_4_17E4:
        mov    edx, dword ptr [ecx + 0x11f]
        add    dx, 0xf
        mov    al, byte ptr [ebp - 1]
        or     al, 0x80
        out    dx, al
        mov    al, byte ptr [ebp - 1]
        or     al, 8
        out    dx, al
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_1805:
        push   0
        push   0xaa000004
        mov    eax, dword ptr [ecx + 0xf8]
        push   eax
        call   sub_1_06D8
        add    esp, 0xc
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1830:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    di, di
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 0xffbf
loc_4_1844:
        push   esi
        call   sub_1_0C90
        push   esi
        call   sub_1_0D30
        cmp    byte ptr [esi + 0xea], 1
        je     loc_4_18A7
        push   esi
        call   sub_4_1900
        or     al, al
        je     loc_4_18E0
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi + 0x11f]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    word ptr [ebp - 2], bx
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 4], ax
        mov    cx, word ptr [esi + 0x11f]
        push   0x3e8
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        call   sub_1_0600
loc_4_18A7:
        push   esi
        call   sub_1_0E60
        or     al, al
        jne    loc_4_18F1
        inc    di
        cmp    di, 3
        jb     loc_4_1844
        push   0xb
        push   0
        push   2
        push   0xaa000003
        mov    eax, dword ptr [esi + 0xf8]
        push   eax
        call   sub_1_06D8
        add    esp, 0x14
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_18E0:
        push   esi
        call   sub_1_0C90
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_18F1:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_1900:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x2c], 0xd
        lea    eax, [ebp - 0x2c]
        mov    word ptr [ebp - 0x2a], 0xd
        mov    dword ptr [ebp - 0x28], 0x4404 ; -> loc_1_4404
        push   -1
        push   eax
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_069C
        cmp    dword ptr [ebp - 0x1c], 0
        jne    loc_4_1A80
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_06A2
        cmp    dword ptr [ebp - 0x1c], 0
        jne    loc_4_1A80
        mov    eax, dword ptr [ebp - 0x10]
        mov    cx, word ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0x82]
        add    dword ptr [ebp - 0x10], 0x100
        mov    word ptr [ebp - 0xa], ax
        mov    eax, dword ptr [ebp - 0x10]
        dec    cx
        mov    dword ptr [ebp - 0x14], eax
        mov    si, 0xffff
        or     cx, cx
        je     loc_4_1A60
        mov    eax, ecx
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x18], eax
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 2
loc_4_199B:
        mov    eax, dword ptr [ebp - 0x10]
        mov    dx, word ptr [eax]
        add    dword ptr [ebp - 0x10], ecx
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax]
        add    dword ptr [ebp - 0x10], ecx
        mov    word ptr [ebp - 6], ax
        mov    eax, dword ptr [ebp - 0x10]
        xor    ebx, ebx
        mov    bx, word ptr [eax]
        mov    ax, word ptr [ebp - 6]
        add    dword ptr [ebp - 0x10], ecx
        add    ax, dx
        add    ax, bx
        add    si, ax
        cmp    dx, 0xffff
        je     loc_4_1A15
        mov    word ptr [ebp - 8], dx
        mov    dx, word ptr [edi + 0x11f]
        add    dx, 0xc
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    dx, word ptr [edi + 0x11f]
        add    dx, 0xd
        mov    al, byte ptr [ebp - 7]
        out    dx, al
        mov    ax, word ptr [ebp - 6]
        mov    word ptr [ebp - 4], ax
        mov    dx, word ptr [edi + 0x11f]
        add    dx, 4
        out    dx, al
        mov    dx, word ptr [edi + 0x11f]
        add    dx, 5
        mov    al, byte ptr [ebp - 3]
        out    dx, al
loc_4_1A15:
        or     ebx, ebx
        je     loc_4_1A47
loc_4_1A19:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax]
        add    si, ax
        add    dword ptr [ebp - 0x10], ecx
        mov    word ptr [ebp - 2], ax
        mov    dx, word ptr [edi + 0x11f]
        add    dx, 2
        out    dx, al
        mov    dx, word ptr [edi + 0x11f]
        add    dx, 3
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        dec    ebx
        jne    loc_4_1A19
loc_4_1A47:
        add    dword ptr [ebp - 0x14], 0x100
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x10], eax
        dec    dword ptr [ebp - 0x18]
        jne    loc_4_199B
        jmp    loc_4_1A63
        db  090h
loc_4_1A60:
        mov    edi, dword ptr [ebp + 8]
loc_4_1A63:
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        call   sub_1_06AE
        cmp    word ptr [ebp - 0xa], si
        jne    loc_4_1A83
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_4_1A80:
        mov    edi, dword ptr [ebp + 8]
loc_4_1A83:
        push   0xd
        push   0
        push   2
        push   0xaa000006
        mov    eax, dword ptr [edi + 0xf8]
        push   eax
        call   sub_1_06D8
        add    esp, 0x14
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 031h
VxD__LTEXT ENDS
