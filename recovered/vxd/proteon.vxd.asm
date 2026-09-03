; PROTEON -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 9762 instructions recovered by recursive descent

Declare_Virtual_Device PROTEON, 3, 10, PROTEON_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x7acc  R-X
loc_1_0000:
        mov    al, byte ptr [0x7c8]          ; -> obj1:0x07C8
        cmp    al, 0
        jne    loc_1_0025
        inc    byte ptr [0x7c8]              ; -> obj1:0x07C8
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0027
        push   0
        push   0
        call   sub_1_07D0
        cmp    eax, 0
        jne    loc_1_0027
loc_1_0025:
        clc    
        ret    
loc_1_0027:
        stc    
        ret    
PROTEON_Control:
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
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_04D0:
        push   esi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep outsb dx, byte ptr [esi]
        pop    esi
        ret    0xc
sub_1_04E4:
        push   esi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep outsw dx, word ptr [esi]
        pop    esi
        ret    0xc
        db  056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh
        db  05Eh, 0C2h, 00Ch, 000h
sub_1_050D:
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep insb byte ptr es:[edi], dx
        pop    edi
        ret    0xc
        db  057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h
        db  06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch
        db  024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
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
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_0574:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_057A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_0592:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_0598:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_059E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_05A4:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
sub_1_05B0:
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
sub_1_05B6:
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_05BC:
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
sub_1_05C2:
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
sub_1_05C8:
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
sub_1_068A:
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
sub_1_06CC:
        VxDCall  VPOWERD.Service_8012h           ; cd2012802800
sub_1_06D2:
        VxDCall  VPOWERD.Service_8013h           ; cd2013802800
sub_1_06D8:
        VxDCall  VPOWERD.Service_8014h           ; cd2014802800
sub_1_06DE:
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
        db  'PROTEON '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  PROTEON_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_07D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x54
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0x50]
        mov    dword ptr [ebp - 0x18], eax
        mov    word ptr [ebp - 0x14], 8
        mov    word ptr [ebp - 0x12], 8
        mov    dword ptr [ebp - 0x10], 0x7930 ; -> obj1:0x7930
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_054A
        mov    dword ptr [ebp - 0x54], 0xffffffff
        mov    eax, dword ptr [ebp - 0x54]
        push   eax
        push   0
        push   8
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0636
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_0832
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_0900
loc_1_0832:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    byte ptr [eax], 3
        mov    eax, dword ptr [ebp - 0x18]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 6], 0x13b9   ; -> loc_1_13B9
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0xa], 0x1664 ; -> sub_1_1664
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0xe], 0x408e ; -> loc_1_408E
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x12], 0x43b0 ; -> loc_1_43B0
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x16], 0x27a6 ; -> loc_1_27A6
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x1a], 0x1a34 ; -> loc_1_1A34
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x1e], 0x26cb ; -> loc_1_26CB
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x22], 0x33f2 ; -> loc_1_33F2
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x26], 0x2ed4 ; -> loc_1_2ED4
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x2a], 0x3345 ; -> loc_1_3345
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0x18]
        add    edx, 0x2e
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], ecx
        push   0x36
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0556
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_08F9
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0550
        mov    eax, 0xc0000001
        jmp    loc_1_0900
loc_1_08F9:
        sub    eax, eax
        jmp    loc_1_0900
loc_1_0900:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_0907:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x4c
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x3c], 0xc
        mov    word ptr [ebp - 0x3a], 0xc
        mov    dword ptr [ebp - 0x38], 0x7944 ; -> obj1:0x7944
        mov    dword ptr [ebp - 0x44], 0xffffffff
        jmp    loc_1_092F
loc_1_092F:
        mov    dword ptr [ebp - 0x48], 0xffffffff
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        push   0
        push   0xe9
        lea    eax, [ebp - 0x30]
        push   eax
        call   sub_1_0636
        mov    dword ptr [ebp - 0x34], eax
        cmp    dword ptr [ebp - 0x34], 0
        je     loc_1_0961
        mov    eax, 0xc000009a
        jmp    loc_1_0FBF
loc_1_0961:
        mov    edi, dword ptr [ebp - 0x30]
        sub    eax, eax
        mov    ecx, 0x3a
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    dword ptr [ecx + 0x91], eax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    ax, word ptr [eax + 0x10]
        and    eax, 0xffff
        cmp    eax, 0x10
        jne    loc_1_09C3
        mov    eax, dword ptr [ebp - 0x30]
        mov    dword ptr [eax + 0xdb], 0x4642
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax + 0x1c], 0x4642
        mov    eax, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [eax + 0x1c]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    dword ptr [ecx + 0xdf], eax
        jmp    loc_1_09DD
loc_1_09C3:
        mov    eax, dword ptr [ebp - 0x30]
        mov    dword ptr [eax + 0xdb], 0x1072
        mov    eax, dword ptr [ebp - 0x30]
        mov    dword ptr [eax + 0xdf], 0x1072
loc_1_09DD:
        mov    eax, dword ptr [ebp - 0x30]
        mov    dword ptr [eax + 0xba], 0x1052
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0xdb]
        sub    eax, 0x20
        mov    ecx, dword ptr [ebp - 0x30]
        mov    dword ptr [ecx + 0xe3], eax
        lea    edi, [ebp - 0x2c]
        sub    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    byte ptr [ebp - 0x28], 1
        mov    byte ptr [ebp - 0x27], 0
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 0x4c], eax
        jmp    loc_1_0A99
loc_1_0A20:
        mov    dword ptr [ebp - 0x22], 1
        mov    eax, dword ptr [ebp + 0x14]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x2c], eax
        mov    dword ptr [ebp - 0x1e], 6
        mov    eax, dword ptr [ebp - 0x30]
        mov    dword ptr [eax + 0x51], 6
        jmp    loc_1_0ABC
loc_1_0A4C:
        mov    dword ptr [ebp - 0x22], 3
        mov    dword ptr [ebp - 0x2c], 0
        mov    dword ptr [ebp - 0x1e], 6
        mov    eax, dword ptr [ebp - 0x30]
        mov    dword ptr [eax + 0x51], 6
        jmp    loc_1_0ABC
loc_1_0A70:
        mov    dword ptr [ebp - 0x22], 2
        mov    dword ptr [ebp - 0x2c], 0
        mov    dword ptr [ebp - 0x1e], 0x18
        mov    eax, dword ptr [ebp - 0x30]
        mov    dword ptr [eax + 0x51], 0x18
        jmp    loc_1_0ABC
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_0A99:
        cmp    dword ptr [ebp - 0x4c], 0
        je     loc_1_0A20
        cmp    dword ptr [ebp - 0x4c], 1
        je     loc_1_0A70
        cmp    dword ptr [ebp - 0x4c], 2
        je     loc_1_0A4C
        jmp    loc_1_0ABC
loc_1_0ABC:
        mov    eax, dword ptr [ebp - 0x30]
        cmp    dword ptr [eax + 0xdf], 0x4642
        jne    loc_1_0AE0
        mov    dword ptr [ebp - 0x1e], 2
        mov    eax, dword ptr [ebp - 0x30]
        mov    dword ptr [eax + 0x51], 2
loc_1_0AE0:
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0xdf]
        mov    dword ptr [ebp - 0x1a], eax
        mov    dword ptr [ebp - 0x16], 1
        mov    eax, dword ptr [ebp + 0x14]
        mov    ax, word ptr [eax + 8]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x12], eax
        mov    dword ptr [ebp - 0xe], 0x10
        mov    dword ptr [ebp - 0x26], 0x48e2 ; -> loc_1_48E2
        mov    eax, dword ptr [ebp + 0x14]
        mov    ax, word ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    word ptr [ecx + 0xe7], ax
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x91]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        add    eax, 0x95
        push   eax
        call   sub_1_0562
        mov    dword ptr [ebp - 0x34], eax
        cmp    dword ptr [ebp - 0x34], 0
        je     loc_1_0BB9
        mov    dword ptr [ebp - 0x1e], 6
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x91]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        add    eax, 0x95
        push   eax
        call   sub_1_0562
        mov    dword ptr [ebp - 0x34], eax
        cmp    dword ptr [ebp - 0x34], 0
        je     loc_1_0BAF
        push   0
        push   0xe9
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        call   sub_1_063C
        mov    eax, dword ptr [ebp - 0x34]
        jmp    loc_1_0FBF
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_0BAF:
        mov    eax, dword ptr [ebp - 0x30]
        mov    dword ptr [eax + 0x51], 6
loc_1_0BB9:
        mov    eax, dword ptr [ebp - 0x44]
        push   eax
        lea    eax, [ebp - 0x3c]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        add    eax, 0x59
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        add    eax, 0x55
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_1_06CC
        cmp    dword ptr [ebp - 0x34], 0
        je     loc_1_0C09
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_0568
        push   0
        push   0xe9
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        call   sub_1_063C
        mov    eax, dword ptr [ebp - 0x34]
        jmp    loc_1_0FBF
loc_1_0C09:
        mov    eax, dword ptr [ebp - 0x30]
        add    eax, 0x25
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0x2e4
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_05C2
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0C69
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x55]
        push   eax
        call   sub_1_06DE
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_0568
        push   0
        push   0xe9
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        call   sub_1_063C
        mov    eax, 0xc000009a
        jmp    loc_1_0FBF
loc_1_0C69:
        jmp    loc_1_0C6E
loc_1_0C6E:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x25]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    dword ptr [ecx + 0x21], eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    edi, dword ptr [eax]
        sub    eax, eax
        mov    ecx, 0xb9
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 0x2dc], eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x95]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 0x2e0], eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0xdf]
        lea    eax, [eax + eax*4]
        mov    dword ptr [ebp - 0x40], eax
        mov    eax, dword ptr [ebp - 0x30]
        add    eax, 0x31
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 0x40]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_05C2
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0D43
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x25]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax]
        push   eax
        push   0
        push   0x2e4
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x55]
        push   eax
        call   sub_1_06DE
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_0568
        push   0
        push   0xe9
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        call   sub_1_063C
        mov    eax, 0xc000009a
        jmp    loc_1_0FBF
loc_1_0D43:
        jmp    loc_1_0D48
loc_1_0D48:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    dword ptr [ecx + 0x35], eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0xdf]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp - 0x30]
        add    eax, dword ptr [ecx + 0x35]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    dword ptr [ecx + 0x39], eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x31]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    dword ptr [ecx + 0x29], eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0xdf]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp - 0x30]
        add    eax, dword ptr [ecx + 0x29]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    dword ptr [ecx + 0x2d], eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        call   sub_1_0FC6
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        call   sub_1_119D
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        call   sub_1_35E5
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        call   sub_1_341F
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_0E3D
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x31]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x35]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 0x40]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x25]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax]
        push   eax
        push   0
        push   0x2e4
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x55]
        push   eax
        call   sub_1_06DE
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_0568
        push   0
        push   0xe9
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        call   sub_1_063C
        mov    eax, 0xc0010006
        jmp    loc_1_0FBF
loc_1_0E3D:
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        call   sub_1_12FF
        or     eax, eax
        je     loc_1_0EC9
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x31]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x35]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 0x40]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x25]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax]
        push   eax
        push   0
        push   0x2e4
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x55]
        push   eax
        call   sub_1_06DE
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_0568
        push   0
        push   0xe9
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        call   sub_1_063C
        mov    eax, 0x80010004
        jmp    loc_1_0FBF
loc_1_0EC9:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x18]
        cmp    eax, 1
        jne    loc_1_0EE6
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_7363
loc_1_0EE6:
        mov    eax, dword ptr [ebp - 0x30]
        add    eax, 0x99
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        add    eax, 0x8d
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax]
        add    eax, 0x2b8
        push   eax
        push   0x2e0a                        ; -> loc_1_2E0A
        push   0x2b61                        ; -> loc_1_2B61
        push   0x2c7d                        ; -> loc_1_2C7D
        push   0x2c0c                        ; -> loc_1_2C0C
        call   sub_2_0000
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_0FB8
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        call   sub_1_35C5
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_76FE
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x31]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x35]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 0x40]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x25]
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax]
        push   eax
        push   0
        push   0x2e4
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x55]
        push   eax
        call   sub_1_06DE
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_0568
        push   0
        push   0xe9
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        call   sub_1_063C
        mov    eax, 0xc000009a
        jmp    loc_1_0FBF
loc_1_0FB8:
        sub    eax, eax
        jmp    loc_1_0FBF
loc_1_0FBF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x18
sub_1_0FC6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x29a], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x12]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x2aa], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xa]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x29c], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x29e], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x2a8], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x18]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x2a6], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x16]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x2a2], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xe]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x2a0], ax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x2a0]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_1_1082
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x2a4], 1
loc_1_1082:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x14]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0x296], ax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x74
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x78], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x78]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x74], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x7c
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x80], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x80]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x7c], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x84
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x88], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x88]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x84], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x70], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xc8], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xb5], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xbe], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xcb], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xcf], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xd3], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xd7], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x9d], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xa5], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xa9], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xad], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xb1], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xc2], 1
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_119D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x41], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x45], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x3d], 2
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x3f], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x49], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x51]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x4d], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x4d], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x6b], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x5d], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x6c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x6e], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x6f], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xc4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0xc6], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xc3], 1
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_123E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x35]
        mov    dword ptr [ebp - 4], eax
        push   0x4489                        ; -> loc_1_4489
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_5E1B
        mov    word ptr [ebp - 8], 0
        jmp    loc_1_126F
loc_1_126B:
        inc    word ptr [ebp - 8]
loc_1_126F:
        movsx  eax, word ptr [ebp - 8]
        cmp    eax, 3
        jge    loc_1_12A6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xdf]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_57C5
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xdf]
        add    dword ptr [ebp - 4], eax
        jmp    loc_1_126B
loc_1_12A6:
        sub    ax, ax
        jmp    loc_1_12AE
loc_1_12AE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_12B5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0x3c34                        ; -> loc_1_3C34
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_68BB
        push   0x3fd5                        ; -> sub_1_3FD5
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_68A2
        sub    ax, ax
        jmp    loc_1_12E3
loc_1_12E3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_12EA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ax, ax
        jmp    loc_1_12F8
loc_1_12F8:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_12FF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x55]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_06D2
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_133B
        cmp    dword ptr [ebp - 4], 0xc001001d
        je     loc_1_133B
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_13B2
loc_1_133B:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_7101
        movsx  eax, ax
        or     eax, eax
        je     loc_1_1373
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_35C5
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_76FE
        mov    eax, 0x80010004
        jmp    loc_1_13B2
loc_1_1373:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x55]
        push   eax
        call   sub_1_06D8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_123E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12B5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12EA
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xc2], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x6b], 1
        sub    eax, eax
        jmp    loc_1_13B2
loc_1_13B2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_13B9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_1_13E7
loc_1_13E4:
        inc    dword ptr [ebp - 0xc]
loc_1_13E7:
        mov    eax, dword ptr [ebp + 0x18]
        cmp    dword ptr [ebp - 0xc], eax
        jae    loc_1_140D
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0x14]
        cmp    dword ptr [ecx + eax*4], 1
        jne    loc_1_1408
        jmp    loc_1_140D
loc_1_1408:
        jmp    loc_1_13E4
loc_1_140D:
        mov    eax, dword ptr [ebp + 0x18]
        cmp    dword ptr [ebp - 0xc], eax
        jne    loc_1_1423
        mov    eax, 0xc0010019
        jmp    loc_1_15E1
loc_1_1423:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 0x70]
        mov    dword ptr [ebp - 0x14], 0xffffffff
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        push   0
        push   0x96
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0636
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_15C2
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1d], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1c], 0
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_1529
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xc4]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_1529
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xc4], 1
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_758C
        movsx  eax, ax
        or     eax, eax
        je     loc_1_1515
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xc4], 0
        push   0
        push   0x96
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_063C
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 0x70]
        mov    eax, 0xc0010007
        jmp    loc_1_15E1
loc_1_1515:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xc4], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xc3], 0
loc_1_1529:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0x99]
        push   eax
        call   sub_2_00F0
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_1573
        push   0
        push   0x96
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_063C
        mov    dword ptr [ebp - 0x10], 0xc0000001
        jmp    loc_1_15BD
loc_1_1573:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 0x1052
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x18]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0xba], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x74
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x78]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0x78]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x78], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x14]
loc_1_15BD:
        jmp    loc_1_15C9
loc_1_15C2:
        mov    dword ptr [ebp - 0x10], 0xc000009a
loc_1_15C9:
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 0x70]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xc8], 0
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_1_15E1
loc_1_15E1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x24
sub_1_15E8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x74]
        mov    dword ptr [ebp - 4], eax
loc_1_1601:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x74
        cmp    eax, dword ptr [ebp - 4]
        je     loc_1_163B
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x18], ecx
        jbe    loc_1_162E
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 8], eax
loc_1_162E:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1601
loc_1_163B:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_1651
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe3]
        mov    dword ptr [ebp - 8], eax
loc_1_1651:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xba], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_1664:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 0x70]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_1931
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x14]
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x99]
        push   eax
        call   sub_2_01D0
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_17A1
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x14], 2
        jne    loc_1_1728
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0xba]
        cmp    dword ptr [eax + 0x18], ecx
        jne    loc_1_1713
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_15E8
loc_1_1713:
        push   0
        push   0x96
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_063C
        jmp    loc_1_179C
loc_1_1728:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1d], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x7c
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x80]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x80], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x8c], 1
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [eax + 0x14], 2
        mov    dword ptr [ebp - 0xc], 0x103
loc_1_179C:
        jmp    loc_1_192C
loc_1_17A1:
        cmp    dword ptr [ebp - 0xc], 0x103
        jne    loc_1_1820
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1d], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x7c
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x80]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x80], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x8c], 1
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [eax + 0x14], 2
        jmp    loc_1_192C
loc_1_1820:
        cmp    dword ptr [ebp - 0xc], 0xc001000e
        jne    loc_1_18A5
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1d], 1
        mov    dword ptr [ebp - 0xc], 0x103
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x7c
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x80]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x80], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x8c], 1
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_192C
loc_1_18A5:
        cmp    dword ptr [ebp - 0xc], 0xc001000d
        jne    loc_1_192C
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1d], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp - 0x20]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x20]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x84
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x88]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0x88]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x88], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x8c], 1
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 0xc], 0x103
        jmp    loc_1_192C
loc_1_192C:
        jmp    loc_1_1938
loc_1_1931:
        mov    dword ptr [ebp - 0xc], 0xc0010002
loc_1_1938:
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 0x70]
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_1946
loc_1_1946:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_194D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xb5]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_1971
        jmp    loc_1_1A2D
loc_1_1971:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x8c]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_198C
        jmp    loc_1_1A2D
loc_1_198C:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x70]
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x8c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x7c]
        mov    dword ptr [ebp - 4], eax
loc_1_19A5:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x7c
        cmp    eax, dword ptr [ebp - 4]
        je     loc_1_1A22
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x14], 0
        jne    loc_1_1A0B
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_0574
        push   0
        push   0x96
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_063C
        jmp    loc_1_1A15
loc_1_1A0B:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x8c], 1
loc_1_1A15:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_19A5
loc_1_1A22:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x70]
        jmp    loc_1_1A2D
loc_1_1A2D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_1A34:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 0x70]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x10], 0
        jne    loc_1_1A7F
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2153
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_1B2A
loc_1_1A7F:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x10], 1
        jne    loc_1_1B23
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xc8]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_1ADA
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_1ACE
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_21CD
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_1AD5
loc_1_1ACE:
        mov    dword ptr [ebp - 0xc], 0xc0010002
loc_1_1AD5:
        jmp    loc_1_1B1E
loc_1_1ADA:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xb5]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_1AFC
        mov    dword ptr [ebp - 0xc], 0xc001000d
        jmp    loc_1_1B1E
loc_1_1AFC:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_1B1E
        mov    dword ptr [ebp - 0xc], 0xc0000001
        jmp    loc_1_1B1E
loc_1_1B1E:
        jmp    loc_1_1B2A
loc_1_1B23:
        mov    dword ptr [ebp - 0xc], 0x10001
loc_1_1B2A:
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 0x70]
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_1B38
loc_1_1B38:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_1B3F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x44
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x1c], 1
        mov    dword ptr [ebp - 0x10], 0
        mov    word ptr [ebp - 0x14], 0
        mov    dword ptr [ebp - 0x18], 0
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 4
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x40], eax
        jmp    loc_1_2058
loc_1_1B82:
        mov    dword ptr [ebp - 0x10], 7
        jmp    loc_1_2101
loc_1_1B8E:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        or     eax, eax
        jne    loc_1_1BAE
        mov    dword ptr [ebp - 0xc], 0x78cc ; -> obj1:0x78CC
        mov    dword ptr [ebp - 8], 0x5c
        jmp    loc_1_1BBC
loc_1_1BAE:
        mov    dword ptr [ebp - 0xc], 0x7850 ; -> obj1:0x7850
        mov    dword ptr [ebp - 8], 0x7c
loc_1_1BBC:
        jmp    loc_1_2101
loc_1_1BC1:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xb5]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_1BE3
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_1C44
loc_1_1BE3:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xc2]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_1C0F
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xc4]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_1C1B
loc_1_1C0F:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_1C44
loc_1_1C1B:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xc8]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_1C3D
        mov    dword ptr [ebp - 4], 4
        jmp    loc_1_1C44
loc_1_1C3D:
        mov    dword ptr [ebp - 4], 0
loc_1_1C44:
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 4
        jmp    loc_1_2101
loc_1_1C56:
        lea    eax, [ebp - 0x1c]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 4
        jmp    loc_1_2101
loc_1_1C68:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe3]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 4
        jmp    loc_1_2101
loc_1_1C86:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xdf]
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp + 0x10], 0x10106
        jne    loc_1_1CA3
        sub    dword ptr [ebp - 0x10], 0xe
loc_1_1CA3:
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 4
        jmp    loc_1_2101
loc_1_1CB5:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 0x2a8]
        and    eax, 0xffff
        cmp    eax, 0x10
        jne    loc_1_1CDB
        mov    dword ptr [ebp - 0x10], 0x27100
        jmp    loc_1_1CE2
loc_1_1CDB:
        mov    dword ptr [ebp - 0x10], 0x9c40
loc_1_1CE2:
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 4
        jmp    loc_1_2101
loc_1_1CF4:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xdf]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 4
        jmp    loc_1_2101
loc_1_1D12:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xdf]
        lea    eax, [eax + eax*2]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 4
        jmp    loc_1_2101
loc_1_1D33:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xdf]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 4
        jmp    loc_1_2101
loc_1_1D51:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xdf]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 4
        jmp    loc_1_2101
loc_1_1D6F:
        mov    al, byte ptr [0x784f]         ; -> loc_1_784F
        mov    byte ptr [ebp - 0x24], al
        mov    al, byte ptr [0x784e]         ; -> obj1:0x784E
        mov    byte ptr [ebp - 0x23], al
        mov    al, byte ptr [0x784d]         ; -> loc_1_784D
        mov    byte ptr [ebp - 0x22], al
        mov    al, byte ptr [0x784c]         ; -> obj1:0x784C
        mov    byte ptr [ebp - 0x21], al
        lea    eax, [ebp - 0x24]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 4
        jmp    loc_1_2101
loc_1_1DA1:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 0x44], eax
        jmp    loc_1_1DD8
loc_1_1DAF:
        mov    dword ptr [ebp - 0xc], 0x7950 ; -> obj1:0x7950
        jmp    loc_1_1DFB
loc_1_1DBB:
        mov    dword ptr [ebp - 0xc], 0x7978 ; -> loc_1_7978
        jmp    loc_1_1DFB
loc_1_1DC7:
        mov    dword ptr [ebp - 0xc], 0x79a0 ; -> loc_1_79A0
        jmp    loc_1_1DFB
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_1DD8:
        cmp    dword ptr [ebp - 0x44], 0
        je     loc_1_1DAF
        cmp    dword ptr [ebp - 0x44], 1
        je     loc_1_1DC7
        cmp    dword ptr [ebp - 0x44], 2
        je     loc_1_1DBB
        jmp    loc_1_1DFB
loc_1_1DFB:
        mov    dword ptr [ebp - 8], 0x27
        jmp    loc_1_2101
loc_1_1E07:
        mov    word ptr [ebp - 0x14], 0x300
        lea    eax, [ebp - 0x14]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 2
        jmp    loc_1_2101
loc_1_1E1F:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        or     eax, eax
        je     loc_1_1E3D
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x9d]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_1E49
loc_1_1E3D:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0x10], eax
loc_1_1E49:
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 4
        jmp    loc_1_2101
loc_1_1E5B:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        or     eax, eax
        jne    loc_1_1E76
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_1ED2
loc_1_1E76:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x74]
        mov    dword ptr [ebp - 0x28], eax
        mov    dword ptr [ebp - 0x10], 0
loc_1_1E86:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x74
        cmp    eax, dword ptr [ebp - 0x28]
        je     loc_1_1ED2
        mov    eax, dword ptr [ebp - 0x28]
        mov    dword ptr [ebp - 0x2c], eax
        mov    eax, dword ptr [ebp - 0x2c]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x18], ecx
        jbe    loc_1_1EC5
        mov    eax, dword ptr [ebp - 0x2c]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0x4622
        jne    loc_1_1EC5
        jmp    loc_1_1ED2
loc_1_1EC5:
        mov    eax, dword ptr [ebp - 0x28]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x28], eax
        jmp    loc_1_1E86
loc_1_1ED2:
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 4
        jmp    loc_1_2101
loc_1_1EE4:
        lea    eax, [ebp - 0x24]
        mov    dword ptr [ebp - 0x30], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        add    eax, 0x2b2
        mov    dword ptr [ebp - 0x34], eax
        mov    eax, dword ptr [ebp - 0x34]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0x34]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 0x34]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 0x34]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 0x34]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 0x34]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    byte ptr [ecx + 5], al
        lea    eax, [ebp - 0x24]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 6
        jmp    loc_1_2101
loc_1_1F4F:
        lea    eax, [ebp - 0x24]
        mov    dword ptr [ebp - 0x38], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        add    eax, 0x2b8
        mov    dword ptr [ebp - 0x3c], eax
        mov    eax, dword ptr [ebp - 0x3c]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 0x38]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0x3c]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 0x38]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 0x3c]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0x38]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 0x3c]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 0x38]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 0x3c]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x38]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 0x3c]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 0x38]
        mov    byte ptr [ecx + 5], al
        lea    eax, [ebp - 0x24]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 6
        jmp    loc_1_2101
loc_1_1FBA:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        or     eax, eax
        jne    loc_1_1FD8
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_1FE3
loc_1_1FD8:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 0x1a]
        mov    dword ptr [ebp - 0x10], eax
loc_1_1FE3:
        mov    eax, dword ptr [ebp - 0x10]
        shl    eax, 8
        and    eax, 0xff0000
        mov    ecx, dword ptr [ebp - 0x10]
        shr    ecx, 0x18
        mov    al, cl
        mov    ecx, dword ptr [ebp - 0x10]
        shl    ecx, 0x18
        and    ecx, 0xff000000
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x10]
        shr    ecx, 8
        and    ecx, 0xff00
        or     eax, ecx
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 4
        jmp    loc_1_2101
loc_1_2027:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 0x2c0]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 4
        jmp    loc_1_2101
loc_1_2047:
        mov    dword ptr [ebp - 0x18], 0xc00000bb
        jmp    loc_1_2101
        db  0E9h, 0A9h, 000h, 000h, 000h
loc_1_2058:
        cmp    dword ptr [ebp - 0x40], 0x2010101
        ja     0x20d5
        je     loc_1_1EE4
        sub    dword ptr [ebp - 0x40], 0x10101
        cmp    dword ptr [ebp - 0x40], 0x12
        ja     loc_1_2047
        shl    dword ptr [ebp - 0x40], 2
        mov    eax, dword ptr [ebp - 0x40]
        jmp    loc_1_2089
loc_1_2089:
        mov    ds, word ptr [ebx]
        add    byte ptr [eax], al
        rcr    dword ptr [ebx], 0
        add    byte ptr [esi + 0x1c], dl     ; -> loc_1_1C56
        add    byte ptr [eax], al
        push   esi
        sbb    al, 0
        add    byte ptr [eax + 0x1c], ch     ; -> loc_1_1C68
        add    byte ptr [eax], al
        xchg   byte ptr [eax + eax], bl
        add    byte ptr [ebp - 0xbffffe4], dh ; -> loc_1_1CB5
        sbb    al, 0
        add    byte ptr [edx], dl            ; -> loc_1_1D12
        sbb    eax, 0x1d330000               ; -> loc_1_1D33
        add    byte ptr [eax], al
        push   ecx
        sbb    eax, 0x1d6f0000               ; -> loc_1_1D6F
        add    byte ptr [eax], al
        mov    eax, dword ptr [0x1f00001d]   ; -> loc_1_1E1F
        push   ds
        add    byte ptr [eax], al
        pop    ebx
        push   ds
        add    byte ptr [eax], al
        pop    es
        push   ds
        add    byte ptr [eax], al
        xchg   byte ptr [eax + eax], bl
        add    byte ptr [edi + 0x20], al     ; -> loc_1_2047
        add    byte ptr [eax], al
        sbb    byte ptr [ebx], 0
        add    byte ptr [ecx + 0x102c07d], al
        add    dword ptr [edx], eax
        je     loc_1_1F4F
        cmp    dword ptr [ebp - 0x40], 0x2010103
        je     loc_1_1FBA
        cmp    dword ptr [ebp - 0x40], 0x2010104
        je     loc_1_2027
        jmp    loc_1_2047
loc_1_2101:
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_1_2144
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_1_212B
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebp - 0x18], 0xc0010014
        jmp    loc_1_2144
loc_1_212B:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_071C
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], eax
loc_1_2144:
        mov    eax, dword ptr [ebp - 0x18]
        jmp    loc_1_214C
loc_1_214C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x20
sub_1_2153:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 0x14], 0
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1B3F
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x20], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x24], eax
        mov    eax, dword ptr [ebp - 0x14]
        jmp    loc_1_21C6
loc_1_21C6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_21CD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0x18], eax
        mov    dword ptr [ebp - 0x20], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 4
        je     loc_1_2232
        mov    dword ptr [ebp - 0x20], 0xc0010014
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp - 0x20]
        jmp    loc_1_23E1
loc_1_2232:
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x24], eax
        jmp    loc_1_2376
loc_1_223D:
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2436
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_1_23BC
loc_1_225A:
        push   4
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_071C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_23E8
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_1_23BC
loc_1_2286:
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2487
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_1_23BC
loc_1_22A3:
        mov    dword ptr [ebp - 0x20], 0
        jmp    loc_1_23BC
loc_1_22AF:
        push   4
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_071C
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x14]
        cmp    dword ptr [eax + 0xe3], ecx
        jb     loc_1_2345
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x14]
        cmp    dword ptr [eax + 0xba], ecx
        jae    loc_1_22FC
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xba], eax
        jmp    loc_1_2340
loc_1_22FC:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xba]
        cmp    dword ptr [eax + 0x18], ecx
        jne    loc_1_2337
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0x14]
        cmp    dword ptr [eax + 0x18], ecx
        jbe    loc_1_2337
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_15E8
        jmp    loc_1_2340
loc_1_2337:
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x18], eax
loc_1_2340:
        jmp    loc_1_234C
loc_1_2345:
        mov    dword ptr [ebp - 0x20], 0xc0010014
loc_1_234C:
        jmp    loc_1_23BC
loc_1_2351:
        mov    dword ptr [ebp - 0x20], 0xc0010017
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x24], 0
        jmp    loc_1_23BC
        db  0E9h, 046h, 000h, 000h, 000h
loc_1_2376:
        cmp    dword ptr [ebp - 0x24], 0x1010e
        je     loc_1_225A
        cmp    dword ptr [ebp - 0x24], 0x1010f
        je     loc_1_22AF
        cmp    dword ptr [ebp - 0x24], 0x10112
        je     loc_1_22A3
        cmp    dword ptr [ebp - 0x24], 0x2010103
        je     loc_1_223D
        cmp    dword ptr [ebp - 0x24], 0x2010104
        je     loc_1_2286
        jmp    loc_1_2351
loc_1_23BC:
        cmp    dword ptr [ebp - 0x20], 0
        jne    loc_1_23D9
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x20], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x24], 0
loc_1_23D9:
        mov    eax, dword ptr [ebp - 0x20]
        jmp    loc_1_23E1
loc_1_23E1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_23E8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0xc]
        inc    dword ptr [eax + 0x14]
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x99]
        push   eax
        call   sub_2_04F0
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        dec    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_242F
loc_1_242F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_1_2436:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0xc]
        inc    dword ptr [eax + 0x14]
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0x99]
        push   eax
        call   sub_2_02C0
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        dec    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_2480
loc_1_2480:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_1_2487:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0xc]
        inc    dword ptr [eax + 0x14]
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0x99]
        push   eax
        call   sub_2_0360
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        dec    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_24D1
loc_1_24D1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_1_24D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x1c], 0
        mov    dword ptr [ebp - 0x10], 0
        mov    dword ptr [ebp - 4], 0xc
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1B3F
        mov    dword ptr [ebp - 0x1c], eax
        cmp    dword ptr [ebp - 0x1c], 0xc00000bb
        jne    loc_1_26AA
        mov    dword ptr [ebp - 0x1c], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_1_25F4
loc_1_255F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xcb]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_266C
loc_1_2570:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xcf]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_266C
loc_1_2581:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd3]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_266C
loc_1_2592:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd7]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_266C
loc_1_25A3:
        mov    dword ptr [ebp - 0x10], 0
        jmp    loc_1_266C
loc_1_25AF:
        mov    dword ptr [ebp - 0x10], 0
        jmp    loc_1_266C
loc_1_25BB:
        mov    dword ptr [ebp - 0x10], 0
        jmp    loc_1_266C
loc_1_25C7:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xc6]
        and    eax, 0xffff
        or     eax, 0xc0011000
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_266C
loc_1_25E3:
        mov    dword ptr [ebp - 0x1c], 0xc0010017
        jmp    loc_1_266C
        db  0E9h, 078h, 000h, 000h, 000h
loc_1_25F4:
        cmp    dword ptr [ebp - 0x20], 0x20105
        ja     loc_1_2640
        je     loc_1_25A3
        cmp    dword ptr [ebp - 0x20], 0x20101
        je     loc_1_255F
        cmp    dword ptr [ebp - 0x20], 0x20102
        je     loc_1_2570
        cmp    dword ptr [ebp - 0x20], 0x20103
        je     loc_1_2581
        cmp    dword ptr [ebp - 0x20], 0x20104
        je     loc_1_2592
        jmp    loc_1_25E3
loc_1_2640:
        cmp    dword ptr [ebp - 0x20], 0x2010105
        je     loc_1_25C7
        cmp    dword ptr [ebp - 0x20], 0x2020101
        je     loc_1_25AF
        cmp    dword ptr [ebp - 0x20], 0x2020102
        je     loc_1_25BB
        jmp    loc_1_25E3
loc_1_266C:
        cmp    dword ptr [ebp - 0x1c], 0
        jne    loc_1_26AA
        cmp    dword ptr [ebp - 0x18], 4
        jb     loc_1_2698
        push   4
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_071C
        add    dword ptr [ebp - 8], 4
        jmp    loc_1_26AA
loc_1_2698:
        mov    eax, 4
        sub    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 0x1c], 0xc0010014
loc_1_26AA:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x20], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x24], eax
        mov    eax, dword ptr [ebp - 0x1c]
        jmp    loc_1_26C4
loc_1_26C4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
loc_1_26CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x70]
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_270B
        db  0E9h, 083h, 000h, 000h, 000h
loc_1_26FA:
        mov    dword ptr [ebp - 8], 0xc0010017
        jmp    loc_1_277D
        db  0E9h, 072h, 000h, 000h, 000h
loc_1_270B:
        cmp    dword ptr [ebp - 0xc], 0x2010103
        ja     loc_1_275E
        cmp    dword ptr [ebp - 0xc], 0x2010101
        jae    loc_1_277D
        cmp    dword ptr [ebp - 0xc], 0x10101
        jb     loc_1_26FA
        cmp    dword ptr [ebp - 0xc], 0x10113
        jbe    loc_1_277D
        cmp    dword ptr [ebp - 0xc], 0x20101
        jb     loc_1_26FA
        cmp    dword ptr [ebp - 0xc], 0x20105
        jbe    loc_1_277D
        jmp    loc_1_26FA
loc_1_275E:
        cmp    dword ptr [ebp - 0xc], 0x2020101
        jb     loc_1_26FA
        cmp    dword ptr [ebp - 0xc], 0x2020102
        jbe    loc_1_277D
        jmp    loc_1_26FA
loc_1_277D:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_2797
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_24D8
        mov    dword ptr [ebp - 8], eax
loc_1_2797:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_279F
loc_1_279F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
loc_1_27A6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x14], 0x103
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        inc    dword ptr [eax + 0x70]
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0xc8]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_28A3
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_2897
        mov    eax, dword ptr [ebp - 0x18]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    dx, word ptr [eax + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 0x10], ax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x10]
        and    ecx, 0xfff7
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x74]
        mov    dword ptr [ebp - 4], eax
loc_1_282F:
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x74
        cmp    eax, dword ptr [ebp - 4]
        je     loc_1_2866
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0
        push   0
        push   0x40010004
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_282F
loc_1_2866:
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0xb5], 1
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0xc8], 1
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 0xbe], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_28F6
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_1_289E
loc_1_2897:
        mov    dword ptr [ebp - 0x14], 0xc0010002
loc_1_289E:
        jmp    loc_1_28E7
loc_1_28A3:
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0xb5]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_28C5
        mov    dword ptr [ebp - 0x14], 0xc001000d
        jmp    loc_1_28E7
loc_1_28C5:
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_28E7
        mov    dword ptr [ebp - 0x14], 0xc0000001
        jmp    loc_1_28E7
loc_1_28E7:
        mov    eax, dword ptr [ebp - 0x14]
        jmp    loc_1_28EF
loc_1_28EF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_28F6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x70]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xb5]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_2B02
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x41], 0
        je     loc_1_29A0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x41]
        mov    dword ptr [ebp - 0x1c], eax
loc_1_2931:
        mov    eax, dword ptr [ebp - 0x1c]
        add    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0xd3]
        push   0xc001000c
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_057A
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xf], 1
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xf]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_2988
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3BA2
loc_1_2988:
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x1c], eax
        cmp    dword ptr [ebp - 0x1c], 0
        jne    loc_1_2931
loc_1_29A0:
        mov    byte ptr [ebp - 8], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_76FE
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x55]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        call   sub_1_06D2
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_29DA
        cmp    dword ptr [ebp - 0x18], 0xc001001d
        jne    loc_1_2A34
loc_1_29DA:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_6D21
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_7101
        movsx  eax, ax
        or     eax, eax
        jne    loc_1_2A34
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_123E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12B5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12EA
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_758C
        movsx  eax, ax
        or     eax, eax
        jne    loc_1_2A34
        mov    byte ptr [ebp - 8], 0
loc_1_2A34:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x55]
        push   eax
        call   sub_1_06D8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_119D
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     eax, eax
        je     loc_1_2A85
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x6b], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xc3], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xb5], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xc8], 0
        mov    eax, 0x80010004
        jmp    loc_1_2B5A
loc_1_2A85:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x6b], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xc3], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xb5], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xc8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x74]
        mov    dword ptr [ebp - 0xc], eax
loc_1_2AB3:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x74
        cmp    eax, dword ptr [ebp - 0xc]
        je     loc_1_2AF6
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x14], eax
        push   0
        push   0
        push   0x40010005
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_05A4
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_2AB3
loc_1_2AF6:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xbe]
        dec    dword ptr [eax + 0x14]
loc_1_2B02:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x84
        cmp    dword ptr [eax + 0x84], ecx
        je     loc_1_2B53
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x84]
        mov    dword ptr [ebp - 0xc], eax
loc_1_2B26:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x84
        cmp    eax, dword ptr [ebp - 0xc]
        je     loc_1_2B53
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_1664
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_2B26
loc_1_2B53:
        sub    eax, eax
        jmp    loc_1_2B5A
loc_1_2B5A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_2B61:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0xb5]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_2B9B
        mov    dword ptr [ebp - 8], 0xc001000d
        jmp    loc_1_2BFD
loc_1_2B9B:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0xc], eax
        jne    loc_1_2BA7
loc_1_2BA7:
        test   byte ptr [ebp + 0xc], 0x24
        jne    loc_1_2BBB
        test   byte ptr [ebp + 0xd], 0x80
        je     loc_1_2BC7
loc_1_2BBB:
        mov    dword ptr [ebp - 8], 0xc00000bb
        jmp    loc_1_2BFD
loc_1_2BC7:
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x9d]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 0xa1], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 0x9d], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_2D36
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 0x14]
loc_1_2BFD:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_2C05
loc_1_2C05:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
loc_1_2C0C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0xb5]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_2C4D
        mov    dword ptr [ebp - 8], 0xc001000d
        jmp    loc_1_2C6E
loc_1_2C4D:
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 0xa5], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_2D36
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 0x14]
loc_1_2C6E:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_2C76
loc_1_2C76:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
loc_1_2C7D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    byte ptr [ebp - 8], 0xc0
        mov    byte ptr [ebp - 7], 0
        mov    word ptr [ebp - 0x18], 0
        jmp    loc_1_2CB3
loc_1_2CAF:
        inc    word ptr [ebp - 0x18]
loc_1_2CB3:
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        cmp    eax, 4
        jge    loc_1_2CEA
        mov    eax, 3
        mov    ecx, dword ptr [ebp - 0x18]
        and    ecx, 0xffff
        sub    eax, ecx
        mov    al, byte ptr [ebp + eax + 0xc]
        mov    ecx, dword ptr [ebp - 0x18]
        and    ecx, 0xffff
        mov    byte ptr [ebp + ecx - 6], al
        jmp    loc_1_2CAF
loc_1_2CEA:
        mov    eax, dword ptr [ebp - 0x14]
        mov    al, byte ptr [eax + 0xb5]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_2D0C
        mov    dword ptr [ebp - 0x10], 0xc001000d
        jmp    loc_1_2D27
loc_1_2D0C:
        mov    eax, dword ptr [ebp - 0xc]
        inc    dword ptr [eax + 0x14]
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_74AC
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
loc_1_2D27:
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_1_2D2F
loc_1_2D2F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
sub_1_2D36:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0x7fffffff
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x9e], 0x20
        je     loc_1_2D61
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_2D89
loc_1_2D61:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x9e], 0x40
        je     loc_1_2D82
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xa5]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_2D89
loc_1_2D82:
        mov    dword ptr [ebp - 4], 0
loc_1_2D89:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xa9], ecx
        je     loc_1_2DFC
        mov    word ptr [ebp - 0x10], 0
        jmp    loc_1_2DAA
loc_1_2DA6:
        inc    word ptr [ebp - 0x10]
loc_1_2DAA:
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        cmp    eax, 4
        jge    loc_1_2DE1
        mov    eax, 3
        mov    ecx, dword ptr [ebp - 0x10]
        and    ecx, 0xffff
        sub    eax, ecx
        mov    al, byte ptr [ebp + eax - 4]
        mov    ecx, dword ptr [ebp - 0x10]
        and    ecx, 0xffff
        mov    byte ptr [ebp + ecx - 8], al
        jmp    loc_1_2DA6
loc_1_2DE1:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_73B0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xa9], eax
loc_1_2DFC:
        sub    eax, eax
        jmp    loc_1_2E03
loc_1_2E03:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_2E0A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_2E1D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x154
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x14], 1
        jmp    loc_1_2E38
loc_1_2E35:
        inc    dword ptr [ebp - 0x14]
loc_1_2E38:
        cmp    dword ptr [ebp - 0x14], 0x10
        jae    loc_1_2E76
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_1_05B0
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_2E71
        mov    eax, 2
        jmp    loc_1_2ECD
loc_1_2E71:
        jmp    loc_1_2E35
loc_1_2E76:
        mov    dword ptr [ebp - 0x14], 1
        jmp    loc_1_2E85
loc_1_2E82:
        inc    dword ptr [ebp - 0x14]
loc_1_2E85:
        cmp    dword ptr [ebp - 0x14], 0x10
        jae    loc_1_2EC6
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 0x154]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_1_05B6
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_2EC1
        mov    eax, 1
        jmp    loc_1_2ECD
loc_1_2EC1:
        jmp    loc_1_2E82
loc_1_2EC6:
        sub    eax, eax
        jmp    loc_1_2ECD
loc_1_2ECD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_2ED4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x88
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    word ptr [ebp - 0x80], 7
        mov    word ptr [ebp - 0x7e], 7
        mov    dword ptr [ebp - 0x7c], 0x79d0 ; -> loc_1_79D0
        mov    word ptr [ebp - 0x70], 9
        mov    word ptr [ebp - 0x6e], 9
        mov    dword ptr [ebp - 0x6c], 0x79e4 ; -> loc_1_79E4
        mov    word ptr [ebp - 0x78], 0xb
        mov    word ptr [ebp - 0x76], 0xb
        mov    dword ptr [ebp - 0x74], 0x79fc ; -> loc_1_79FC
        mov    word ptr [ebp - 0x58], 0xa
        mov    word ptr [ebp - 0x56], 0xa
        mov    dword ptr [ebp - 0x54], 0x7a14 ; -> obj1:0x7A14
        mov    word ptr [ebp - 0x68], 0xa
        mov    word ptr [ebp - 0x66], 0xa
        mov    dword ptr [ebp - 0x64], 0x7a2c ; -> obj1:0x7A2C
        mov    word ptr [ebp - 0x50], 0xb
        mov    word ptr [ebp - 0x4e], 0xb
        mov    dword ptr [ebp - 0x4c], 0x7a44 ; -> loc_1_7A44
        mov    word ptr [ebp - 0x18], 4
        mov    word ptr [ebp - 0x16], 4
        mov    dword ptr [ebp - 0x14], 0x7a54 ; -> obj1:0x7A54
        mov    word ptr [ebp - 0xc], 0xb
        mov    word ptr [ebp - 0xa], 0xb
        mov    dword ptr [ebp - 8], 0x7a64   ; -> obj1:0x7A64
        mov    byte ptr [ebp - 0x24], 1
        mov    dword ptr [ebp - 0x1c], 0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_2E1D
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_06B4
        cmp    dword ptr [ebp - 0x1c], 0
        je     loc_1_2FB8
        mov    eax, dword ptr [ebp - 0x1c]
        jmp    loc_1_333E
loc_1_2FB8:
        lea    eax, [ebp - 0x44]
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 4], 0
        mov    word ptr [ebp - 0x3c], 0xa20
        mov    word ptr [ebp - 0x3a], 5
        mov    word ptr [ebp - 0x38], 1
        mov    word ptr [ebp - 0x36], 5
        mov    word ptr [ebp - 0x34], 0x10
        mov    word ptr [ebp - 0x32], 1
        mov    word ptr [ebp - 0x30], 0
        mov    word ptr [ebp - 0x2c], 1
        mov    word ptr [ebp - 0x2e], 8
        mov    dword ptr [ebp - 0x28], 0x1072
        push   0
        lea    eax, [ebp - 0x80]
        push   eax
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        lea    eax, [ebp - 0x5c]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0x1c], 0
        jne    loc_1_3036
        mov    eax, dword ptr [ebp - 0x5c]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [ebp - 0x3c], ax
        mov    dword ptr [ebp - 0x10], 0
loc_1_3036:
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        lea    eax, [ebp - 0x5c]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0x1c], 0
        jne    loc_1_3085
        mov    eax, dword ptr [ebp - 0x5c]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x28], eax
        cmp    dword ptr [ebp - 0x28], 0x4642
        jbe    loc_1_3074
        mov    dword ptr [ebp - 0x28], 0x4642
loc_1_3074:
        cmp    dword ptr [ebp - 0x28], 0xf
        jae    loc_1_3085
        mov    dword ptr [ebp - 0x28], 0xf
loc_1_3085:
        push   0
        lea    eax, [ebp - 0x70]
        push   eax
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        lea    eax, [ebp - 0x5c]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0x1c], 0
        jne    loc_1_30B0
        mov    eax, dword ptr [ebp - 0x5c]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [ebp - 0x3a], ax
loc_1_30B0:
        push   0
        lea    eax, [ebp - 0x78]
        push   eax
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        lea    eax, [ebp - 0x5c]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0x1c], 0
        jne    loc_1_30DB
        mov    eax, dword ptr [ebp - 0x5c]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [ebp - 0x36], ax
loc_1_30DB:
        push   0
        lea    eax, [ebp - 0x58]
        push   eax
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        lea    eax, [ebp - 0x5c]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0x1c], 0
        jne    loc_1_3106
        mov    eax, dword ptr [ebp - 0x5c]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [ebp - 0x34], ax
loc_1_3106:
        push   0
        lea    eax, [ebp - 0x50]
        push   eax
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        lea    eax, [ebp - 0x5c]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0x1c], 0
        jne    loc_1_3131
        mov    eax, dword ptr [ebp - 0x5c]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [ebp - 0x30], ax
loc_1_3131:
        push   0
        lea    eax, [ebp - 0x68]
        push   eax
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        lea    eax, [ebp - 0x5c]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0x1c], 0
        jne    loc_1_3178
        mov    eax, dword ptr [ebp - 0x5c]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_1_3172
        mov    word ptr [ebp - 0x32], 1
        jmp    loc_1_3178
loc_1_3172:
        mov    word ptr [ebp - 0x32], 0
loc_1_3178:
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        lea    eax, [ebp - 0x60]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_05BC
        cmp    dword ptr [ebp - 0x1c], 0
        jne    loc_1_3206
        cmp    dword ptr [ebp - 0x60], 6
        jne    loc_1_3206
        mov    eax, dword ptr [ebp - 0x20]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0x20]
        mov    cl, byte ptr [ecx + 3]
        and    ecx, 0xff
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x20]
        mov    cl, byte ptr [ecx + 4]
        and    ecx, 0xff
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x20]
        mov    cl, byte ptr [ecx + 5]
        and    ecx, 0xff
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x20]
        mov    cl, byte ptr [ecx]
        and    ecx, 0xff
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x20]
        mov    cl, byte ptr [ecx + 1]
        and    ecx, 0xff
        or     eax, ecx
        je     loc_1_3206
        push   6
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x44]
        push   eax
        call   sub_1_071C
loc_1_3206:
        mov    eax, dword ptr [ebp - 0x48]
        push   eax
        call   sub_1_06C0
        cmp    dword ptr [ebp - 0x10], 2
        jne    loc_1_3291
        mov    eax, dword ptr [ebp - 0x30]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_1_3284
        mov    word ptr [ebp - 0x84], 1
        jmp    loc_1_323E
loc_1_3237:
        inc    word ptr [ebp - 0x84]
loc_1_323E:
        mov    eax, dword ptr [ebp - 0x84]
        and    eax, 0xffff
        cmp    eax, 0x10
        jge    loc_1_327F
        mov    ax, word ptr [ebp - 0x84]
        mov    word ptr [ebp - 0x30], ax
        lea    eax, [ebp - 0x44]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_3618
        movsx  eax, ax
        or     eax, eax
        jne    loc_1_327A
        jmp    loc_1_327F
loc_1_327A:
        jmp    loc_1_3237
loc_1_327F:
        jmp    loc_1_3291
loc_1_3284:
        lea    eax, [ebp - 0x44]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_3618
loc_1_3291:
        cmp    dword ptr [ebp - 0x10], 1
        jne    loc_1_3313
        mov    eax, dword ptr [ebp - 0x30]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_1_3306
        mov    word ptr [ebp - 0x88], 1
        jmp    loc_1_32C0
loc_1_32B9:
        inc    word ptr [ebp - 0x88]
loc_1_32C0:
        mov    eax, dword ptr [ebp - 0x88]
        and    eax, 0xffff
        cmp    eax, 0x10
        jge    loc_1_3301
        mov    ax, word ptr [ebp - 0x88]
        mov    word ptr [ebp - 0x30], ax
        lea    eax, [ebp - 0x44]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_3760
        movsx  eax, ax
        or     eax, eax
        jne    loc_1_32FC
        jmp    loc_1_3301
loc_1_32FC:
        jmp    loc_1_32B9
loc_1_3301:
        jmp    loc_1_3313
loc_1_3306:
        lea    eax, [ebp - 0x44]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_3760
loc_1_3313:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    al, byte ptr [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x44]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_0907
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        jmp    loc_1_333E
loc_1_333E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
loc_1_3345:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_35C5
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_76FE
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x99]
        push   eax
        call   sub_2_00D0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xdf]
        lea    eax, [eax + eax*4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x31]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x35]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x25]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        push   eax
        push   0
        push   0x2e4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x95]
        push   eax
        call   sub_1_0568
        push   0
        push   0xe9
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_063C
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_33F2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_055C
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_0550
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_341F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_6D21
        mov    word ptr [ebp - 4], ax
        movsx  ecx, word ptr [ebp - 4]
        or     ecx, ecx
        je     loc_1_344F
        sub    al, al
        jmp    loc_1_3456
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_344F:
        mov    al, 1
        jmp    loc_1_3456
loc_1_3456:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        db  000h, 000h, 000h
sub_1_3460:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x29e]
        and    eax, 0xffff
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x29c]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x29c]
        and    eax, 0xffff
        push   eax
        push   0x399d                        ; -> loc_1_399D
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0x3856                        ; -> loc_1_3856
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2e0]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_061E
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_34EC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        mov    byte ptr [eax + 0xca], 1
loc_1_34EC:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_34F4
loc_1_34F4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_34FB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_3540
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        add    eax, 4
        push   eax
        call   sub_1_0624
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        mov    byte ptr [eax + 0xca], 0
loc_1_3540:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_3547:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_194D
        push   0x32
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x5f
        push   eax
        call   sub_1_0612
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_1_3574:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0xc350
        call   sub_1_0630
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x2dc]
        inc    word ptr [ecx + 0x5d]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x2dc]
        mov    ax, word ptr [ecx + 0x5d]
        jmp    loc_1_35A3
loc_1_35A3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_35AA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0x32
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x5f
        push   eax
        call   sub_1_0612
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_35C5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x5f
        push   eax
        call   sub_1_0618
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_35E5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x5f], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x63], 0x3547 ; -> loc_1_3547
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x67], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_35AA
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_3618:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05B0
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_3649
        mov    ax, 0xffff
        jmp    loc_1_3759
loc_1_3649:
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        cmp    eax, 0x202
        jne    loc_1_3689
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 6], 0x1890
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0x12], 0
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xe]
        test   al, 8
        je     loc_1_3684
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0xe], 0
loc_1_3684:
        jmp    loc_1_370E
loc_1_3689:
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        cmp    eax, 0x700
        je     loc_1_36BD
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        cmp    eax, 0x130
        je     loc_1_36BD
        mov    ax, 0xfffe
        jmp    loc_1_3759
        db  0E9h, 051h, 000h, 000h, 000h
loc_1_36BD:
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 6], 0x1892
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xe]
        test   al, 4
        je     loc_1_36E1
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0x12], 0
        jmp    loc_1_36EA
loc_1_36E1:
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0x12], 1
loc_1_36EA:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xe]
        test   al, 8
        je     loc_1_3705
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0x10], 0x10
        jmp    loc_1_370E
loc_1_3705:
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0x10], 4
loc_1_370E:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x14], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0xc], 0
        mov    al, byte ptr [ebp - 0xb]
        and    eax, 0x38
        shr    eax, 3
        mov    al, byte ptr [eax + 0x7a70]   ; -> obj1:0x7A70
        and    eax, 0xffff00ff
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0xa], ax
        mov    al, byte ptr [ebp - 0xb]
        and    eax, 7
        mov    ax, word ptr [eax*2 + 0x7a78] ; -> obj1:0x7A78
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 8], ax
        sub    ax, ax
        jmp    loc_1_3759
loc_1_3759:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_3760:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x148
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0x140]
        push   eax
        lea    eax, [ebp - 0x148]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 0x144]
        push   eax
        call   sub_1_05B6
        cmp    dword ptr [ebp - 0x144], 0
        je     loc_1_37A0
        mov    ax, 0xffff
        jmp    loc_1_382E
loc_1_37A0:
        cmp    dword ptr [ebp - 0x140], 0x60110e
        je     loc_1_37CF
        mov    eax, dword ptr [ebp - 0x140]
        and    eax, 0xffffff
        cmp    eax, 0x604f42
        je     loc_1_37CF
        mov    ax, 0xfffe
        jmp    loc_1_382E
loc_1_37CF:
        mov    eax, dword ptr [ebp - 0x148]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0x14], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x14]
        and    eax, 0xffff
        shl    eax, 0xc
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 8], ax
        mov    al, byte ptr [ebp - 0x8e]
        and    ax, 0xf
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx + 0xa], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0xc], 1
        mov    al, byte ptr [ebp - 0x8e]
        shr    al, 5
        test   al, 1
        je     loc_1_3826
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0xc], 0
loc_1_3826:
        sub    ax, ax
        jmp    loc_1_382E
loc_1_382E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
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
loc_1_3856:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dx, word ptr [eax + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        test   cl, 8
        jne    loc_1_388E
        sub    al, al
        jmp    loc_1_392F
loc_1_388E:
        mov    eax, dword ptr [ebp - 8]
        mov    dx, word ptr [eax + 0x1ea]
        in     ax, dx
        mov    word ptr [ebp - 0x10], ax
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        test   al, 0x80
        je     loc_1_3928
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        test   al, 0x88
        je     loc_1_38CF
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0x2d0], ax
loc_1_38CF:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x2dc]
        mov    ax, word ptr [eax + 0x6c]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x10]
        and    ecx, 0xffff
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0x2dc]
        mov    word ptr [ecx + 0x6c], ax
        mov    eax, dword ptr [ebp - 8]
        mov    dx, word ptr [eax + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xfff7
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
        mov    al, 1
        jmp    loc_1_392F
loc_1_3928:
        sub    al, al
        jmp    loc_1_392F
loc_1_392F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_3936:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_3996
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xc5]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_3996
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xc5], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xc9], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x99]
        push   eax
        call   sub_1_0300
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xc5], 0
loc_1_3996:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_399D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_53EF
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x2dc]
        push   eax
        call   sub_1_3FD5
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x2dc]
        push   eax
        call   sub_1_3936
        mov    ecx, dword ptr [ebp - 4]
        mov    dx, word ptr [ecx + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        or     ecx, 8
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        mov    eax, dword ptr [eax + 0x21]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        sub    eax, dword ptr [ebp + 8]
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0x14]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 4]
        shr    eax, 0x10
        mov    ecx, dword ptr [ebp + 0x10]
        mov    word ptr [ecx], ax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_1_3A52:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        mov    eax, dword ptr [eax + 0x29]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x2dc]
        sub    eax, dword ptr [ecx + 0x35]
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0x14]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 4]
        shr    eax, 0x10
        mov    ecx, dword ptr [ebp + 0x10]
        mov    word ptr [ecx], ax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
        db  000h, 000h, 000h
sub_1_3AA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x41], 0
        jne    loc_1_3AC0
        int3   
        mov    ax, 0xffff
        jmp    loc_1_3B9B
loc_1_3AC0:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x41]
        add    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x41], ecx
        jne    loc_1_3B13
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], 0
        jne    loc_1_3B00
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x41], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x45], 0
        jmp    loc_1_3B0B
loc_1_3B00:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x41], eax
loc_1_3B0B:
        sub    ax, ax
        jmp    loc_1_3B9B
loc_1_3B13:
        int3   
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x41]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x10], eax
loc_1_3B25:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_3B92
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0xc], eax
        jne    loc_1_3B7F
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], 0
        jne    loc_1_3B5E
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x45], eax
        jmp    loc_1_3B77
loc_1_3B5E:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
loc_1_3B77:
        sub    ax, ax
        jmp    loc_1_3B9B
loc_1_3B7F:
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_3B25
loc_1_3B92:
        mov    ax, 0xfffe
        jmp    loc_1_3B9B
loc_1_3B9B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_3BA2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, word ptr [eax + 8]
        cmp    eax, -1
        jne    loc_1_3BD0
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x3d]
        jmp    loc_1_3C2D
loc_1_3BD0:
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0660
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, word ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
loc_1_3BEF:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_3C24
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x51], ecx
        jne    loc_1_3C12
        mov    dword ptr [ebp - 8], 0
loc_1_3C12:
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_06A8
        jmp    loc_1_3BEF
loc_1_3C24:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x4d], eax
loc_1_3C2D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
loc_1_3C34:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_3BA2
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xf], 2
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 0xcb]
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_3AA0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_057A
        mov    ecx, dword ptr [ebp - 0xc]
        dec    dword ptr [ecx + 0x14]
        sub    ax, ax
        jmp    loc_1_3CA5
loc_1_3CA5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_3CAC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x3f]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        imul   eax, dword ptr [ecx + 0xdf]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0x39]
        mov    dword ptr [ebp - 8], eax
        push   0
        lea    eax, [ebp - 0x10]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0660
loc_1_3CF7:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_3D41
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0684
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_071C
        mov    eax, dword ptr [ebp - 0x14]
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0x14]
        add    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_06A8
        jmp    loc_1_3CF7
loc_1_3D41:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    dword ptr [eax + 0xdf], ecx
        jae    loc_1_3D53
loc_1_3D53:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x3f]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        imul   eax, dword ptr [ecx + 0xdf]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0x2d]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0xe], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    word ptr [ecx + 0xc], ax
        mov    eax, dword ptr [ebp + 0x10]
        mov    word ptr [eax + 0xa], 1
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 8], 0xffff
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x3d]
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x3f]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x3f]
        and    eax, 0xffff
        cmp    eax, 2
        jne    loc_1_3DBD
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x3f], 0
loc_1_3DBD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_3DC4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x50
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        mov    word ptr [ebp - 0x4c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x49]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0660
loc_1_3DFC:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_3EA0
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_068A
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x51], ecx
        jne    loc_1_3E30
        mov    dword ptr [ebp - 8], 0
loc_1_3E30:
        mov    dword ptr [ebp - 0x50], 0
        jmp    loc_1_3E3F
loc_1_3E3C:
        inc    dword ptr [ebp - 0x50]
loc_1_3E3F:
        mov    eax, dword ptr [ebp - 0x18]
        cmp    dword ptr [ebp - 0x50], eax
        jae    loc_1_3E8E
        jmp    loc_1_3E50
loc_1_3E50:
        mov    eax, dword ptr [ebp - 0x50]
        mov    eax, dword ptr [ebp + eax*8 - 0x48]
        mov    ecx, dword ptr [ebp - 0x4c]
        and    ecx, 0xffff
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [edx + ecx*2 + 0xe], eax
        mov    eax, dword ptr [ebp - 0x50]
        mov    eax, dword ptr [ebp + eax*8 - 0x44]
        mov    ecx, dword ptr [ebp - 0x4c]
        and    ecx, 0xffff
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp + 0x10]
        mov    word ptr [edx + ecx*2 + 0xc], ax
        inc    word ptr [ebp - 0x4c]
        jmp    loc_1_3E3C
loc_1_3E8E:
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_06A8
        jmp    loc_1_3DFC
loc_1_3EA0:
        mov    eax, dword ptr [ebp - 0x4c]
        and    eax, 0xffff
        cmp    eax, 6
        jle    loc_1_3EB1
loc_1_3EB1:
        mov    ax, word ptr [ebp - 0x4c]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    word ptr [ecx + 0xa], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x49]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 8], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x49], eax
        sub    eax, eax
        sub    eax, dword ptr [ebp - 0x14]
        neg    eax
        mov    ecx, dword ptr [ebp + 8]
        sub    dword ptr [ecx + 0x4d], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_3EE6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x4d]
        ja     loc_1_3F25
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xc]
        and    eax, 0xffff
        cmp    eax, 6
        jle    loc_1_3F7B
loc_1_3F25:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x3d]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_1_3F42
        mov    ax, 0xffff
        jmp    loc_1_3FCE
loc_1_3F42:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 0x244]
        and    eax, 0xffff
        cmp    eax, 1
        jg     loc_1_3F65
        mov    ax, 0xfffd
        jmp    loc_1_3FCE
loc_1_3F65:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3CAC
        jmp    loc_1_3FC6
loc_1_3F7B:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xc]
        and    eax, 0xffff
        mov    ecx, 3
        add    eax, 2
        cdq    
        idiv   ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    cx, word ptr [ecx + 0x244]
        and    ecx, 0xffff
        cmp    eax, ecx
        jl     loc_1_3FB5
        mov    ax, 0xfffd
        jmp    loc_1_3FCE
loc_1_3FB5:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3DC4
loc_1_3FC6:
        sub    ax, ax
        jmp    loc_1_3FCE
loc_1_3FCE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_3FD5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x38
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x41], 0
        je     loc_1_407F
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x41]
        mov    dword ptr [ebp - 0x38], eax
loc_1_3FF4:
        mov    eax, dword ptr [ebp - 0x38]
        add    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xf]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_406D
        lea    eax, [ebp - 0x34]
        push   eax
        mov    eax, dword ptr [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3EE6
        movsx  eax, ax
        or     eax, eax
        je     loc_1_4031
        jmp    loc_1_407F
loc_1_4031:
        mov    eax, dword ptr [ebp - 0x38]
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_5FBB
        movsx  eax, ax
        or     eax, eax
        jne    loc_1_405B
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xf], 1
        jmp    loc_1_406D
loc_1_405B:
        mov    eax, dword ptr [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3BA2
        jmp    loc_1_407F
loc_1_406D:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x38], eax
        cmp    dword ptr [ebp - 0x38], 0
        jne    loc_1_3FF4
loc_1_407F:
        sub    ax, ax
        jmp    loc_1_4087
loc_1_4087:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_408E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0x103
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 0x18]
        push   eax
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0660
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x18]
        cmp    dword ptr [eax + 0xdf], ecx
        jb     loc_1_40E3
        cmp    dword ptr [ebp - 0x18], 0xf
        jae    loc_1_40ED
loc_1_40E3:
        mov    eax, 0xc001000f
        jmp    loc_1_41E9
loc_1_40ED:
        mov    eax, dword ptr [ebp - 0x10]
        inc    dword ptr [eax + 0x70]
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0xc8]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_4196
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_4196
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_418A
        mov    eax, dword ptr [ebp - 0x1c]
        inc    dword ptr [eax + 0x14]
        test   byte ptr [ebp + 0xc], 1
        je     loc_1_4148
loc_1_4148:
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0xa], ax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0xc], ax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0xe], al
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_41F0
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_3FD5
        mov    eax, dword ptr [ebp - 0x1c]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_4191
loc_1_418A:
        mov    dword ptr [ebp - 0xc], 0xc0010002
loc_1_4191:
        jmp    loc_1_41DB
loc_1_4196:
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0xb5]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_41B8
        mov    dword ptr [ebp - 0xc], 0xc001000d
        jmp    loc_1_41DB
loc_1_41B8:
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_41DA
        mov    dword ptr [ebp - 0xc], 0xc0000001
        jmp    loc_1_41DB
loc_1_41DA:
        int3   
loc_1_41DB:
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x70]
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_41E9
loc_1_41E9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_41F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_4207
loc_1_4207:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xf], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x41], 0
        jne    loc_1_4232
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x41], eax
        jmp    loc_1_423E
loc_1_4232:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x45]
        mov    dword ptr [ecx + 0x24], eax
loc_1_423E:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x45], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0xc]
        inc    dword ptr [eax + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
        db  000h, 000h, 000h
sub_1_4260:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_4288
        jmp    loc_1_43A9
loc_1_4288:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_0660
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_42AC
        jmp    loc_1_43A9
loc_1_42AC:
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0684
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
loc_1_42C3:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp - 0x10], eax
        jae    loc_1_43A1
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_1_430B
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_06A8
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_42F5
        jmp    loc_1_43A1
loc_1_42F5:
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0684
        jmp    loc_1_42C3
loc_1_430B:
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_4353
        mov    eax, dword ptr [ebp + 0x14]
        cmp    dword ptr [ebp - 0x14], eax
        jae    loc_1_433C
        sub    eax, eax
        sub    eax, dword ptr [ebp - 0x14]
        neg    eax
        sub    dword ptr [ebp + 0x14], eax
        mov    dword ptr [ebp - 0x14], 0
        jmp    loc_1_42C3
        db  0E9h, 017h, 000h, 000h, 000h
loc_1_433C:
        mov    eax, dword ptr [ebp + 0x14]
        add    dword ptr [ebp - 0x18], eax
        sub    eax, eax
        sub    eax, dword ptr [ebp + 0x14]
        neg    eax
        sub    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp + 0x14], 0
loc_1_4353:
        mov    eax, dword ptr [ebp + 0xc]
        sub    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp - 0x1c]
        cmp    eax, ecx
        jb     loc_1_4372
        mov    eax, ecx
loc_1_4372:
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_071C
        mov    eax, dword ptr [ebp - 0x1c]
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x1c]
        add    dword ptr [ebp - 0x10], eax
        sub    eax, eax
        sub    eax, dword ptr [ebp - 0x1c]
        neg    eax
        sub    dword ptr [ebp - 0x14], eax
        jmp    loc_1_42C3
loc_1_43A1:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx], eax
loc_1_43A9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
loc_1_43B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 0x70]
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xc8]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_4435
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_4429
        mov    eax, dword ptr [ebp - 0xc]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_4260
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_4430
loc_1_4429:
        mov    dword ptr [ebp - 4], 0xc001000c
loc_1_4430:
        jmp    loc_1_4474
loc_1_4435:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xb5]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_4457
        mov    dword ptr [ebp - 4], 0xc001000d
        jmp    loc_1_4474
loc_1_4457:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xc4]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_4474
        mov    dword ptr [ebp - 4], 0xc0000001
loc_1_4474:
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 0x70]
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_4482
loc_1_4482:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x18
loc_1_4489:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        mov    dword ptr [ebp - 0xc], eax
        mov    ax, word ptr [ebp + 0x14]
        mov    word ptr [ebp - 0x10], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0x14]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_1_44C5
        mov    ax, 1
        jmp    loc_1_4590
loc_1_44C5:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax + 0xba]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x10]
        and    ecx, 0xffff
        cmp    eax, ecx
        jl     loc_1_44E7
        mov    eax, ecx
loc_1_44E7:
        mov    word ptr [ebp - 0x10], ax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 8]
        and    eax, 0xff
        test   al, 0x80
        je     loc_1_4513
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xe]
        and    eax, 0x1f
        add    eax, 0xe
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_4519
loc_1_4513:
        mov    word ptr [ebp - 4], 0xe
loc_1_4519:
        mov    eax, dword ptr [ebp + 0x14]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        sub    eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        sub    eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x99]
        push   eax
        call   sub_1_00B0
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 0xc9], 1
        mov    ecx, dword ptr [ebp - 0xc]
        inc    dword ptr [ecx + 0xcf]
        sub    ax, ax
        jmp    loc_1_4590
loc_1_4590:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_45A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x29a]
        mov    word ptr [ebp - 4], ax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1e2], ax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1e4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1e6], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1e8], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 6
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1ea], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 6
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1ec], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1ee], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 0xa
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1f0], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 0xc
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1f2], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 0xe
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1f4], ax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_4682:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x10], eax
loc_1_4691:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 0xc], ax
        add    dword ptr [ebp - 0x10], 2
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 8], ax
        add    dword ptr [ebp - 0x10], 2
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 4], ax
        add    dword ptr [ebp - 0x10], 2
        movsx  eax, word ptr [ebp - 0xc]
        cmp    eax, 1
        jne    loc_1_46E5
        movsx  eax, word ptr [ebp - 8]
        or     eax, eax
        jne    loc_1_46E5
        movsx  eax, word ptr [ebp - 4]
        or     eax, eax
        jne    loc_1_46E5
        jmp    loc_1_4746
loc_1_46E5:
        movsx  eax, word ptr [ebp - 4]
        or     eax, eax
        je     loc_1_4741
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1f2]
        mov    ax, word ptr [ebp - 0xc]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1f0]
        mov    ax, word ptr [ebp - 8]
        out    dx, ax
        jmp    loc_1_471A
loc_1_4716:
        dec    word ptr [ebp - 4]
loc_1_471A:
        movsx  eax, word ptr [ebp - 4]
        or     eax, eax
        jle    loc_1_4741
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dx, word ptr [eax + 0x1e6]
        mov    ax, word ptr [ecx]
        out    dx, ax
        add    dword ptr [ebp - 0x10], 2
        jmp    loc_1_4716
loc_1_4741:
        jmp    loc_1_4691
loc_1_4746:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_474D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
        inc    dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 8]
        movsx  eax, byte ptr [eax]
        cmp    eax, 0x47
        je     loc_1_4777
        mov    ax, 0xffff
        jmp    loc_1_48DB
loc_1_4777:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], eax
        inc    dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 0xc]
        movsx  eax, byte ptr [eax]
        cmp    eax, 0x41
        je     loc_1_4798
        mov    ax, 0xffff
        jmp    loc_1_48DB
loc_1_4798:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x10], eax
        inc    dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 0x10]
        movsx  eax, byte ptr [eax]
        cmp    eax, 0x48
        je     loc_1_47B9
        mov    ax, 0xffff
        jmp    loc_1_48DB
loc_1_47B9:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x14], eax
        inc    dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 0x14]
        movsx  eax, byte ptr [eax]
        cmp    eax, 0x21
        je     loc_1_47DA
        mov    ax, 0xffff
        jmp    loc_1_48DB
loc_1_47DA:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x18], eax
        inc    dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 0x18]
        movsx  eax, byte ptr [eax]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0x10]
        cmp    eax, ecx
        je     loc_1_4861
        inc    dword ptr [ebp + 0xc]
        inc    dword ptr [ebp + 0xc]
        inc    dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0xc]
        movsx  ax, byte ptr [eax]
        mov    word ptr [ebp - 4], ax
        inc    dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0xc]
        movsx  ax, byte ptr [eax]
        and    eax, 0xffff
        shl    eax, 8
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        or     eax, ecx
        mov    word ptr [ebp - 4], ax
        inc    dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 0x68
        add    dword ptr [ebp + 0xc], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x1c], eax
        inc    dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 0x1c]
        movsx  eax, byte ptr [eax]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0x10]
        cmp    eax, ecx
        je     loc_1_4861
        mov    ax, 0x3b
        jmp    loc_1_48DB
loc_1_4861:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x20], eax
        inc    dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 0x20]
        movsx  eax, byte ptr [eax]
        or     eax, eax
        je     loc_1_4881
        mov    ax, 0xffff
        jmp    loc_1_48DB
loc_1_4881:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x24], eax
        inc    dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 0x24]
        movsx  eax, byte ptr [eax]
        cmp    eax, 2
        je     loc_1_48A2
        mov    ax, 0xffff
        jmp    loc_1_48DB
loc_1_48A2:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x28], eax
        inc    dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 0x28]
        movsx  eax, byte ptr [eax]
        or     eax, eax
        je     loc_1_48C2
        mov    ax, 0xffff
        jmp    loc_1_48DB
loc_1_48C2:
        add    dword ptr [ebp + 0xc], 0x66
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4682
        sub    ax, ax
        jmp    loc_1_48DB
loc_1_48DB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
loc_1_48E2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x95], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x1c], 0
        jne    loc_1_4951
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xe7]
        and    eax, 0xffff
        add    eax, 0x13
        mov    dword ptr [ebp - 8], eax
        mov    dx, word ptr [ebp - 8]
        in     al, dx
        mov    byte ptr [ebp - 0xc], al
        mov    al, byte ptr [ebp - 0xc]
        and    eax, 0xcf
        mov    byte ptr [ebp - 0xc], al
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 3
        mov    al, byte ptr [eax + 0x7a94]   ; -> obj1:0x7A94
        and    eax, 0xff
        or     al, byte ptr [ebp - 0xc]
        mov    byte ptr [ebp - 0xc], al
        mov    dx, word ptr [ebp - 8]
        mov    al, byte ptr [ebp - 0xc]
        out    dx, al
loc_1_4951:
        sub    eax, eax
        jmp    loc_1_4958
loc_1_4958:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_495F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_496C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_4979:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 8], 0xf8
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2a4]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_49B3
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x2a0], 0
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     al, 2
        mov    byte ptr [ebp - 8], al
loc_1_49B3:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2a8]
        and    eax, 0xffff
        cmp    eax, 0x10
        jne    loc_1_49D6
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xdf
        mov    byte ptr [ebp - 8], al
loc_1_49D6:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2aa]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_1_49F7
        mov    al, byte ptr [ebp - 8]
        and    eax, 0x7f
        mov    byte ptr [ebp - 8], al
loc_1_49F7:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2a6]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_1_4A19
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xef
        mov    byte ptr [ebp - 8], al
loc_1_4A19:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x28a]
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x28a]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        or     cl, 4
        mov    dx, word ptr [eax + 0x28a]
        mov    al, cl
        out    dx, al
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x28a]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp - 8]
        and    ecx, 0xfb
        mov    dx, word ptr [eax + 0x28a]
        mov    al, cl
        out    dx, al
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x28a]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x294]
        and    eax, 0xffff
        cmp    eax, 0x1392
        jne    loc_1_4AFD
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x29c]
        and    eax, 0xffff
        mov    al, byte ptr [eax + 0x7a85]   ; -> loc_1_7A85
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2a0]
        and    eax, 3
        mov    al, byte ptr [eax + 0x7a94]   ; -> obj1:0x7A94
        and    eax, 0xff
        or     al, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2a2]
        and    eax, 0xffff
        sar    eax, 2
        mov    al, byte ptr [eax + 0x7a98]   ; -> loc_1_7A98
        and    eax, 0xff
        or     al, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x28c]
        mov    al, byte ptr [ebp - 8]
        out    dx, al
loc_1_4AFD:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        or     ecx, 0xc0
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x1ee]
        and    ecx, 0xffff
        inc    ecx
        mov    dx, cx
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_4B82
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        or     eax, 0x10
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        or     ecx, 0xd0
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
loc_1_4B82:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        or     eax, 0x40
        and    eax, 0xffffff7f
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, word ptr [ebp - 0xc]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2a4]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_4BDB
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        or     ecx, 0x100
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
loc_1_4BDB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_4BE2:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x29a]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_45A0
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x294], 0x1390
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        add    ecx, 0x11
        mov    ebx, dword ptr [ebp + 8]
        mov    word ptr [ebx + 0x28a], cx
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        add    ecx, 0x1f
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        cmp    ecx, 0x1f
        jne    loc_1_4CEC
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        add    ecx, 0x1e
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        add    ecx, 0x1e
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        add    ecx, 0x1e
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 7
        cmp    al, 2
        jne    loc_1_4CEC
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x294], 0x1392
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        add    eax, 0x13
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x28c], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        add    eax, 0x12
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x292], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        add    eax, 0x1e
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x28e], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        add    eax, 0x1f
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x290], ax
loc_1_4CEC:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x28a]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 1
        jne    loc_1_4D18
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x298], 1
        jmp    loc_1_4D3C
loc_1_4D18:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x2a4], 1
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x298], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x2a0], 0
loc_1_4D3C:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x29e], 1
        sub    ax, ax
        jmp    loc_1_4D50
loc_1_4D50:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_4D57:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        movsx  eax, word ptr [ebp + 0xc]
        or     eax, eax
        je     loc_1_4DB1
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x28a]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        test   cl, 4
        jne    loc_1_4DB1
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   al, 0xc0
        jne    loc_1_4DB1
        mov    ax, 1
        jmp    loc_1_4DC2
loc_1_4DB1:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4979
        sub    ax, ax
        jmp    loc_1_4DC2
loc_1_4DC2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ax, ax
        jmp    loc_1_4DD7
loc_1_4DD7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_4DDE:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_4DEB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ax, ax
        jmp    loc_1_4E06
loc_1_4E06:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_4E0D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        or     ecx, 0xc0
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x1ee]
        and    ecx, 0xffff
        inc    ecx
        mov    dx, cx
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_4E9B
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        or     eax, 0x10
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        or     ecx, 0xd0
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
loc_1_4E9B:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        or     eax, 0x40
        and    eax, 0xffffff7f
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, word ptr [ebp - 8]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2a4]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_4EF4
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        or     ecx, 0x100
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
loc_1_4EF4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_4EFB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x298], 0xb
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x29e], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_45A0
        sub    ax, ax
        jmp    loc_1_4F2A
loc_1_4F2A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_4F31:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        movsx  eax, word ptr [ebp + 0xc]
        or     eax, eax
        je     loc_1_4F6F
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   al, 0xc0
        jne    loc_1_4F6F
        mov    ax, 1
        jmp    loc_1_4F80
loc_1_4F6F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4E0D
        sub    ax, ax
        jmp    loc_1_4F80
loc_1_4F80:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_4F87:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_4F94:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ax, ax
        jmp    loc_1_4FAF
loc_1_4FAF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_4FB6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x29a]
        and    eax, 0xffff
        add    eax, 0xc84
        mov    dx, ax
        sub    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        mov    word ptr [ebp - 0xc], ax
loc_1_4FE6:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        sub    eax, ecx
        cmp    eax, 2
        jge    loc_1_500D
        jmp    loc_1_4FE6
loc_1_500D:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x29a]
        and    eax, 0xffff
        add    eax, 0xc84
        mov    dx, ax
        mov    al, 1
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        mov    word ptr [ebp - 0xc], ax
loc_1_5034:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        sub    eax, ecx
        cmp    eax, 5
        jge    loc_1_505B
        jmp    loc_1_5034
loc_1_505B:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x29a]
        and    eax, 0xffff
        add    eax, 0x80
        mov    dx, ax
        sub    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        mov    word ptr [ebp - 0xc], ax
loc_1_5082:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        sub    eax, ecx
        cmp    eax, 5
        jge    loc_1_50A9
        jmp    loc_1_5082
loc_1_50A9:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        or     ecx, 0xc0
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x1ee]
        and    ecx, 0xffff
        inc    ecx
        mov    dx, cx
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_512E
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        or     eax, 0x10
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        or     ecx, 0xd0
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
loc_1_512E:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        or     ecx, 0x40
        and    ecx, 0xffffff7f
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_5156:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x29a]
        and    eax, 0xffff
        add    eax, 0xc85
        mov    dx, ax
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 8
        shr    eax, 3
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x2aa], ax
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 4
        je     loc_1_51AB
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x2a8], 4
        jmp    loc_1_51B7
loc_1_51AB:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x2a8], 0x10
loc_1_51B7:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x294], 0x1990
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x298], 0x16
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_45A0
        sub    ax, ax
        jmp    loc_1_51E0
loc_1_51E0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_51E7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        movsx  eax, word ptr [ebp + 0xc]
        or     eax, eax
        je     loc_1_5225
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   al, 0xc0
        jne    loc_1_5225
        mov    ax, 1
        jmp    loc_1_5236
loc_1_5225:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4FB6
        sub    ax, ax
        jmp    loc_1_5236
loc_1_5236:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_523D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 0x3e8
        jmp    loc_1_5255
loc_1_5251:
        dec    word ptr [ebp - 8]
loc_1_5255:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        or     eax, eax
        jle    loc_1_5290
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   ah, 2
        jne    loc_1_528B
        jmp    loc_1_5290
loc_1_528B:
        jmp    loc_1_5251
loc_1_5290:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_5297:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        or     eax, 0x800
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, word ptr [ebp - 0xc]
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x1f4]
        in     ax, dx
        mov    word ptr [ebp - 0x10], ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x1ec]
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x1e8]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   ah, 4
        jne    loc_1_534B
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        dec    eax
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1e4]
        and    eax, 0xffff
        push   eax
        call   sub_1_04D0
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp - 0x10]
        and    ebx, 0xffff
        mov    esi, dword ptr [ebp - 4]
        mov    dx, word ptr [ecx + 0x1e4]
        mov    al, byte ptr [ebx + esi - 1]
        out    dx, al
        jmp    loc_1_5389
loc_1_534B:
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        dec    eax
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1e4]
        and    eax, 0xffff
        push   eax
        call   sub_1_050D
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x1e4]
        in     al, dx
        mov    ecx, dword ptr [ebp - 0x10]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx - 1], al
loc_1_5389:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_523D
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_5399:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2a4]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_53E8
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   ah, 2
        je     loc_1_53E8
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5297
loc_1_53E8:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_53EF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
loc_1_53F8:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2a4]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_5443
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   ah, 2
        je     loc_1_5443
        mov    ax, word ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5297
        jmp    loc_1_53F8
loc_1_5443:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ea]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   al, 0x80
        je     loc_1_5476
        mov    ax, word ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6C5D
        jmp    loc_1_547B
loc_1_5476:
        jmp    loc_1_5480
loc_1_547B:
        jmp    loc_1_53F8
loc_1_5480:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_54D3
loc_1_54A4:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_495F
        jmp    loc_1_54F6
loc_1_54B2:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4F87
        jmp    loc_1_54F6
loc_1_54C0:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4DDE
        jmp    loc_1_54F6
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_54D3:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_54A4
        cmp    dword ptr [ebp - 4], 1
        je     loc_1_54B2
        cmp    dword ptr [ebp - 4], 2
        je     loc_1_54C0
        jmp    loc_1_54F6
loc_1_54F6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_5549
loc_1_551A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_496C
        jmp    loc_1_556C
loc_1_5528:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4F94
        jmp    loc_1_556C
loc_1_5536:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4DEB
        jmp    loc_1_556C
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_5549:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_551A
        cmp    dword ptr [ebp - 4], 1
        je     loc_1_5528
        cmp    dword ptr [ebp - 4], 2
        je     loc_1_5536
        jmp    loc_1_556C
loc_1_556C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_5573:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_55CB
loc_1_5590:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4BE2
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_55EE
loc_1_55A2:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5156
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_55EE
loc_1_55B4:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4EFB
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_55EE
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_55CB:
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_5590
        cmp    dword ptr [ebp - 8], 1
        je     loc_1_55A2
        cmp    dword ptr [ebp - 8], 2
        je     loc_1_55B4
        jmp    loc_1_55EE
loc_1_55EE:
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_55F7
loc_1_55F7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_55FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_5665
loc_1_561B:
        mov    ax, word ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4D57
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_5688
loc_1_5632:
        mov    ax, word ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_51E7
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_5688
loc_1_5649:
        mov    ax, word ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4F31
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_5688
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_5665:
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_561B
        cmp    dword ptr [ebp - 8], 1
        je     loc_1_5632
        cmp    dword ptr [ebp - 8], 2
        je     loc_1_5649
        jmp    loc_1_5688
loc_1_5688:
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_5691
loc_1_5691:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_5698:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_1_56A1:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_56D9
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2d4]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_56D4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5399
loc_1_56D4:
        jmp    loc_1_56DE
loc_1_56D9:
        jmp    loc_1_56E3
loc_1_56DE:
        jmp    loc_1_56A1
loc_1_56E3:
        mov    eax, dword ptr [ebp + 0x18]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_56FF
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x2d0], 0
loc_1_56FF:
        mov    ax, word ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx], ax
        mov    ax, word ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2], ax
        mov    ax, word ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ea]
        mov    ax, 0x9080
        out    dx, ax
        mov    eax, dword ptr [ebp + 0x18]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_57B6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        mov    word ptr [ebp - 4], ax
loc_1_574C:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2d4]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_576C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5399
loc_1_576C:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0x18]
        and    ecx, 0xffff
        cmp    eax, ecx
        jl     loc_1_579F
        mov    ax, 0xffff
        jmp    loc_1_57BE
loc_1_579F:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2d0]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_574C
loc_1_57B6:
        sub    ax, ax
        jmp    loc_1_57BE
loc_1_57BE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
sub_1_57C5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x220]
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 0x222]
        sub    eax, ecx
        mov    word ptr [ebp - 0xc], ax
        movsx  eax, word ptr [ebp - 0xc]
        or     eax, eax
        jg     loc_1_57FF
        movsx  eax, word ptr [ebp - 0xc]
        add    eax, 6
        mov    word ptr [ebp - 0xc], ax
loc_1_57FF:
        movsx  eax, word ptr [ebp - 0xc]
        cmp    eax, 1
        jne    loc_1_5815
        mov    ax, 0xffff
        jmp    loc_1_5A22
loc_1_5815:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1fa]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_1_5836
        mov    ax, 0xffff
        jmp    loc_1_5A22
loc_1_5836:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x1fa]
        mov    ax, word ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 0x222]
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + ecx*2 + 0x1fc], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 0x222]
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*2 + 0x1fe], eax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x222]
        cmp    eax, 5
        jne    loc_1_589A
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x222], 0
        jmp    loc_1_58A4
loc_1_589A:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x222]
loc_1_58A4:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A52
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    al, ch
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 0x1f8]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0x38], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        mov    al, ch
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 0x1f8]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0x3a], ax
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 0x1f8]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0x36], ax
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x224]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x226]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x224]
        and    ecx, 0xffff
        cmp    eax, ecx
        jge    loc_1_5983
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x224]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x226], ax
loc_1_5983:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x1f8]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [eax + ecx + 0x32], 0x88
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x1f8]
        cmp    eax, 5
        jne    loc_1_59C3
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1f8], 0
        jmp    loc_1_59CD
loc_1_59C3:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x1f8]
loc_1_59CD:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2d4]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_5A0A
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2d6]
        and    eax, 0xffff
        or     eax, 0x8200
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x2d6], ax
        sub    ax, ax
        jmp    loc_1_5A22
loc_1_5A0A:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ea]
        mov    ax, 0x8280
        out    dx, ax
        sub    ax, ax
        jmp    loc_1_5A22
loc_1_5A22:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_5A29:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1de], 0
        je     loc_1_5A7A
        mov    eax, dword ptr [ebp + 0x14]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 0x1de]
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        or     eax, eax
        je     loc_1_5A7A
        jmp    loc_1_5A8C
loc_1_5A7A:
        mov    ax, word ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_57C5
loc_1_5A8C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_1_5A93:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1de], 0
        je     loc_1_5AE3
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x226]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x224]
        and    ecx, 0xffff
        cmp    eax, ecx
        je     loc_1_5AE3
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 0x1de]
loc_1_5AE3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_5AEA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
loc_1_5AF3:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x224]
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x1fa]
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x1f6]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0x32]
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x1f6]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [eax + ecx + 0x32], 0
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x1f6]
        cmp    eax, 5
        jne    loc_1_5B65
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1f6], 0
        jmp    loc_1_5B6F
loc_1_5B65:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x1f6]
loc_1_5B6F:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x220]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*2 + 0x1fe]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x220]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0x1fc]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x220]
        cmp    eax, 5
        jne    loc_1_5BC9
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x220], 0
        jmp    loc_1_5BD3
loc_1_5BC9:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x220]
loc_1_5BD3:
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_57C5
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   al, 0x10
        je     loc_1_5BFA
        jmp    loc_1_5BFF
loc_1_5BFA:
        jmp    loc_1_5AF3
loc_1_5BFF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_5C06:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
loc_1_5C0F:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x224]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_1_5C34
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5A93
        jmp    loc_1_5D9E
loc_1_5C34:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x1f6]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0x32]
        and    eax, 0xffff
        test   al, 0x40
        jne    loc_1_5C69
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5A93
        jmp    loc_1_5D9E
loc_1_5C69:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x1f6]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0x32]
        and    eax, 0xffff
        test   al, 0x10
        jne    loc_1_5C9E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5AEA
        jmp    loc_1_5C0F
loc_1_5C9E:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x224]
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x1f6]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0x34]
        and    eax, 0xffff
        mov    al, ah
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 0x1f6]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ecx + edx + 0x34]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x1f6]
        cmp    eax, 5
        jne    loc_1_5D0A
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1f6], 0
        jmp    loc_1_5D14
loc_1_5D0A:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x1f6]
loc_1_5D14:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x1fa]
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x220]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*2 + 0x1fe]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x220]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0x1fc]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x220]
        cmp    eax, 5
        jne    loc_1_5D78
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x220], 0
        jmp    loc_1_5D82
loc_1_5D78:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x220]
loc_1_5D82:
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    ax, word ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5A29
        jmp    loc_1_5C0F
loc_1_5D9E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_5DA5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x1f6]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x2e
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        push   0
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        mov    al, ch
        push   eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    al, ch
        push   eax
        push   0x600
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_5E1B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1de], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_5E34:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1fa], 6
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1f6], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1f8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x220], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x222], 0
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_5E88
loc_1_5E84:
        inc    word ptr [ebp - 4]
loc_1_5E88:
        movsx  eax, word ptr [ebp - 4]
        cmp    eax, 5
        jge    loc_1_5F4D
        movsx  eax, word ptr [ebp - 4]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [eax + ecx + 0x32], 0
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        movsx  eax, word ptr [ebp - 4]
        lea    eax, [eax*2 + 2]
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x2e
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        movsx  eax, word ptr [ebp - 4]
        lea    eax, [eax*2 + 2]
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x2e
        movsx  ecx, word ptr [ebp - 4]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + edx + 0x48], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        mov    al, ch
        movsx  ecx, word ptr [ebp - 4]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0x2e], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 0xc]
        movsx  ecx, word ptr [ebp - 4]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0x30], ax
        jmp    loc_1_5E84
loc_1_5F4D:
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2e
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x2e
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0xde], ecx
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        mov    ch, cl
        mov    edx, dword ptr [ebp - 8]
        and    edx, 0xffff
        mov    cl, dh
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 0xc4], cx
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        mov    cl, ch
        mov    ch, byte ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 0xc6], cx
        sub    ax, ax
        jmp    loc_1_5FB4
loc_1_5FB4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_5FBB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x244]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 0x268]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x246], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x242]
        mov    word ptr [ebp - 0x14], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x242]
        mov    word ptr [ebp - 0x10], ax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [eax + ecx + 0xe6], 0x20
        mov    word ptr [ebp - 8], 0
        mov    word ptr [ebp - 4], 0
        mov    word ptr [ebp - 0x18], 0
        jmp    loc_1_6039
loc_1_6035:
        inc    word ptr [ebp - 8]
loc_1_6039:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xa]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_6231
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ax, word ptr [ecx + eax*2 + 0xc]
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xa]
        and    eax, 0xffff
        dec    eax
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        cmp    eax, ecx
        je     loc_1_60B3
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        or     eax, 0x8000
        mov    word ptr [ebp - 0xc], ax
loc_1_60B3:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        mov    al, ch
        mov    ecx, dword ptr [ebp - 0x18]
        and    ecx, 0xffff
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp - 0x10]
        and    edx, 0xffff
        add    edx, edx
        lea    edx, [edx + edx*2]
        lea    edx, [edx + edx*4]
        lea    ecx, [edx + ecx*2]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0xea], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ax, word ptr [ecx + eax*2 + 0x10]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp + 0xc]
        mov    cx, word ptr [edx + ecx*2 + 0x10]
        and    ecx, 0xffff
        mov    al, ch
        mov    ecx, dword ptr [ebp - 0x18]
        and    ecx, 0xffff
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp - 0x10]
        and    edx, 0xffff
        add    edx, edx
        lea    edx, [edx + edx*2]
        lea    edx, [edx + edx*4]
        lea    ecx, [edx + ecx*2]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0xec], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ax, word ptr [ecx + eax*2 + 0xe]
        and    eax, 0xffff
        mov    al, ah
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp + 0xc]
        mov    ah, byte ptr [edx + ecx*2 + 0xe]
        mov    ecx, dword ptr [ebp - 0x18]
        and    ecx, 0xffff
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp - 0x10]
        and    edx, 0xffff
        add    edx, edx
        lea    edx, [edx + edx*2]
        lea    edx, [edx + edx*4]
        lea    ecx, [edx + ecx*2]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0xee], ax
        inc    word ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xa]
        and    eax, 0xffff
        dec    eax
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        cmp    eax, ecx
        je     loc_1_622C
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        cmp    eax, 3
        jne    loc_1_622C
        mov    word ptr [ebp - 0x18], 0
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x244]
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        cmp    eax, 7
        jne    loc_1_620B
        mov    word ptr [ebp - 0x10], 0
        jmp    loc_1_620F
loc_1_620B:
        inc    word ptr [ebp - 0x10]
loc_1_620F:
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [eax + ecx + 0xe6], 0
loc_1_622C:
        jmp    loc_1_6035
loc_1_6231:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x26e]
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x26a]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x26a]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x26c]
        and    ecx, 0xffff
        cmp    eax, ecx
        jl     loc_1_6280
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x26a]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x26c], ax
loc_1_6280:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x268]
        cmp    eax, 7
        jne    loc_1_62A4
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x268], 0
        jmp    loc_1_62AE
loc_1_62A4:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x268]
loc_1_62AE:
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0xe6]
        and    eax, 0xffff
        or     eax, 0x10
        mov    ecx, dword ptr [ebp - 0x10]
        and    ecx, 0xffff
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0xe6], ax
        mov    ax, word ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x242], ax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x242]
        cmp    eax, 7
        jne    loc_1_631F
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x242], 0
        jmp    loc_1_6329
loc_1_631F:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x242]
loc_1_6329:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    al, ch
        mov    ecx, dword ptr [ebp - 0x14]
        and    ecx, 0xffff
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0xe8], ax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0xe6]
        and    eax, 0xffff
        or     eax, 0x88
        mov    ecx, dword ptr [ebp - 0x14]
        and    ecx, 0xffff
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0xe6], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2d4]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_63D8
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2d6]
        and    eax, 0xffff
        or     eax, 0x8100
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x2d6], ax
        sub    ax, ax
        jmp    loc_1_63F0
loc_1_63D8:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ea]
        mov    ax, 0x8180
        out    dx, ax
        sub    ax, ax
        jmp    loc_1_63F0
loc_1_63F0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_63F7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x26a]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1da], 0
        je     loc_1_6467
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x266]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x246]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 0x1da]
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x266]
        cmp    eax, 7
        jne    loc_1_645D
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x266], 0
        jmp    loc_1_6467
loc_1_645D:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x266]
loc_1_6467:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_646E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 0x1d6]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_648E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
loc_1_6494:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x26a]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_1_64B5
        jmp    loc_1_65C4
        db  0E9h, 00Ah, 001h, 000h, 000h
loc_1_64B5:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x240]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0xe6]
        and    eax, 0xffff
        test   al, 0x80
        jne    loc_1_65BA
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x240]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0xe6]
        and    eax, 0xffff
        test   al, 0x10
        jne    loc_1_656B
loc_1_6509:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x244]
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x240]
        cmp    eax, 7
        jne    loc_1_6537
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x240], 0
        jmp    loc_1_6541
loc_1_6537:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x240]
loc_1_6541:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x240]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0xe6]
        and    eax, 0xffff
        test   al, 0x10
        je     loc_1_6509
loc_1_656B:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x244]
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x240]
        cmp    eax, 7
        jne    loc_1_6599
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x240], 0
        jmp    loc_1_65A3
loc_1_6599:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x240]
loc_1_65A3:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_63F7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_646E
        jmp    loc_1_65BF
loc_1_65BA:
        jmp    loc_1_65C4
loc_1_65BF:
        jmp    loc_1_6494
loc_1_65C4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_65CB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
loc_1_65D1:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x26a]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_6677
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x240]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0xe6]
        and    eax, 0xff7f
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 0x240]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0xe6], ax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x240]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0xe6]
        and    eax, 0xffff
        or     eax, 0x40
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 0x240]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0xe6], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_648E
        jmp    loc_1_65D1
loc_1_6677:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_667E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x240]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0xe2
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        push   0
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        mov    al, ch
        push   eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    al, ch
        push   eax
        push   0x400
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_66F6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x244], 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x240], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x242], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x266], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x268], 0
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_674A
loc_1_6746:
        inc    word ptr [ebp - 4]
loc_1_674A:
        movsx  eax, word ptr [ebp - 4]
        cmp    eax, 7
        jge    loc_1_6827
        movsx  eax, word ptr [ebp - 4]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [eax + ecx + 0xe6], 0
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        movsx  eax, word ptr [ebp - 4]
        lea    eax, [eax*2 + 2]
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0xe2
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        movsx  eax, word ptr [ebp - 4]
        lea    eax, [eax*2 + 2]
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0xe2
        movsx  ecx, word ptr [ebp - 4]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + edx + 0xfc], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        mov    al, ch
        movsx  ecx, word ptr [ebp - 4]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0xe2], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        mov    al, ch
        movsx  ecx, word ptr [ebp - 4]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0xe4], ax
        jmp    loc_1_6746
loc_1_6827:
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xe2
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0xe2
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0x1ce], ecx
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        mov    ch, cl
        mov    edx, dword ptr [ebp - 8]
        and    edx, 0xffff
        mov    cl, dh
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 0x1b4], cx
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        mov    ch, cl
        mov    edx, dword ptr [ebp - 0xc]
        and    edx, 0xffff
        mov    cl, dh
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 0x1b6], cx
        sub    ax, ax
        jmp    loc_1_689B
loc_1_689B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_68A2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1d6], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_68BB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1da], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_68D4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2c0]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x16], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2c2]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x18], ax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xe
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    al, ch
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        mov    al, ch
        mov    word ptr [ebp - 8], ax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x2a], ax
        mov    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x2c], ax
        mov    ax, word ptr [ebp + 0xc]
        push   eax
        mov    ax, word ptr [ebp - 8]
        push   eax
        mov    ax, word ptr [ebp - 4]
        push   eax
        push   0x300
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
        jmp    loc_1_6981
loc_1_6981:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_6988:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2be]
        push   eax
        push   0xf00
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
loc_1_69AE:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5399
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x1ea]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        cmp    byte ptr [ebp - 4], 0x84
        jne    loc_1_69FF
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        and    eax, 0xffff
        mov    al, ah
        mov    ecx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ecx + 8]
        and    eax, 0xffff
        test   ah, 0xd
        je     loc_1_69FF
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x2d2], 0x211
loc_1_69FF:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   al, 0x80
        je     loc_1_6A1F
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ea]
        mov    ax, 0xa000
        out    dx, ax
loc_1_6A1F:
        cmp    byte ptr [ebp - 4], 0x88
        jne    loc_1_69AE
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_6A30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0xffff
        mov    al, ch
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x2d2], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x2d0], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ea]
        mov    ax, 0xa000
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2d0]
        and    eax, 0xffff
        cmp    eax, 0x300
        jne    loc_1_6B7F
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2d2]
        and    eax, 0xffff
        cmp    eax, 0x8000
        jne    loc_1_6B59
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2c0]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x2c2]
        and    ecx, 0xffff
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x2be]
        and    ecx, 0xffff
        or     eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_6B30
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6988
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2d2]
        and    eax, 0xffff
        cmp    eax, 0x8000
        je     loc_1_6B30
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x2d4], 0
        jmp    loc_1_6B8B
loc_1_6B30:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_648E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5C06
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5DA5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_667E
        jmp    loc_1_6B7F
loc_1_6B59:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x2ce]
        or     eax, eax
        je     loc_1_6B7F
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_65CB
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68D4
loc_1_6B7F:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x2d4], 0
loc_1_6B8B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_6B92:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        and    eax, 0xffff
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0xffff
        mov    al, ch
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ea]
        mov    ax, 0xa000
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1d2], 0
        je     loc_1_6BF3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 0x1d2]
        mov    word ptr [ebp - 8], ax
loc_1_6BF3:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   ah, 0xd
        je     loc_1_6C31
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_6C31
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x2ce]
        or     eax, eax
        je     loc_1_6C31
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68D4
loc_1_6C31:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x2d4], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1d2], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_6C5D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x2c6]
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x2d4], 1
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xff
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_6CFB
loc_1_6C8B:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6A30
        jmp    loc_1_6D1A
loc_1_6C99:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6B92
        jmp    loc_1_6D1A
loc_1_6CA7:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x2d6], 0xa080
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ea]
        sub    ax, ax
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5C06
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_648E
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x2d4], 0
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x1ea]
        mov    ax, word ptr [ebx + 0x2d6]
        out    dx, ax
        jmp    loc_1_6D1A
loc_1_6CFB:
        cmp    dword ptr [ebp - 4], 0x84
        je     loc_1_6C99
        cmp    dword ptr [ebp - 4], 0x88
        je     loc_1_6C8B
        jmp    loc_1_6CA7
loc_1_6D1A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_6D21:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        mov    al, byte ptr [eax + 0xb5]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_6D63
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5573
        movsx  eax, ax
        or     eax, eax
        je     loc_1_6D63
        mov    ax, 0xffff
        jmp    loc_1_6E06
loc_1_6D63:
        mov    word ptr [0x7aa8], 0x9f00     ; -> loc_1_7AA8
        mov    word ptr [0x7ab4], 0x505      ; -> loc_1_7AB4
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [0x7ab6], ax         ; -> loc_1_7AB6
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [0x7ab8], ax         ; -> loc_1_7AB8
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 6
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        mov    cx, word ptr [ebp - 4]
        mov    word ptr [0x7aba], cx         ; -> loc_1_7ABA
        mov    cx, word ptr [ebp - 8]
        mov    word ptr [0x7abc], cx         ; -> loc_1_7ABC
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0xe], 0x60
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1e], 0x1a00
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x20], 0x1a00
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x22], 4
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x28], 0xa
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x29], 0x19
        sub    ax, ax
        jmp    loc_1_6E06
loc_1_6E06:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_6E0D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], eax
        push   4
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_474D
        mov    byte ptr [ebp - 0x10], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        or     eax, eax
        je     loc_1_6E72
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        cmp    eax, 0x3b
        jne    loc_1_6E72
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp + 0xc], eax
        push   3
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_474D
        movsx  eax, ax
        or     eax, eax
        je     loc_1_6E72
        mov    ax, 0xffff
        jmp    loc_1_7014
loc_1_6E72:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xff3f
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        mov    word ptr [ebp - 0x14], ax
loc_1_6EA7:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ea]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    al, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x3b
        jne    loc_1_6ED0
        jmp    loc_1_6F28
loc_1_6ED0:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x40
        jne    loc_1_6EE6
        sub    ax, ax
        jmp    loc_1_7014
loc_1_6EE6:
        mov    al, byte ptr [ebp - 4]
        and    eax, 0x30
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x30
        jne    loc_1_6F06
        mov    ax, 0xfffe
        jmp    loc_1_7014
loc_1_6F06:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x14]
        and    ecx, 0xffff
        sub    eax, ecx
        cmp    eax, 0x5b
        jle    loc_1_6EA7
loc_1_6F28:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x3b
        jne    loc_1_700B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7777
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp + 0xc], eax
        push   3
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_474D
        movsx  eax, ax
        or     eax, eax
        je     loc_1_6F68
        mov    ax, 0xffff
        jmp    loc_1_7014
loc_1_6F68:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xff3f
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        mov    word ptr [ebp - 0x14], ax
loc_1_6F9D:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ea]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    al, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x40
        jne    loc_1_6FC9
        sub    ax, ax
        jmp    loc_1_7014
loc_1_6FC9:
        mov    al, byte ptr [ebp - 4]
        and    eax, 0x30
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x30
        jne    loc_1_6FE9
        mov    ax, 0xfffe
        jmp    loc_1_7014
loc_1_6FE9:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x14]
        and    ecx, 0xffff
        sub    eax, ecx
        cmp    eax, 0x5b
        jle    loc_1_6F9D
loc_1_700B:
        mov    ax, 0xfffe
        jmp    loc_1_7014
loc_1_7014:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_701B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1f2]
        mov    ax, 1
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1f0]
        mov    ax, 0xa00
        out    dx, ax
        jmp    loc_1_7049
loc_1_7049:
        jmp    loc_1_704E
loc_1_704E:
        push   0xb
        push   0x7aa8                        ; -> loc_1_7AA8
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1e6]
        and    eax, 0xffff
        push   eax
        call   sub_1_04E4
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x1ea]
        mov    ax, 0x9080
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        mov    word ptr [ebp - 0xc], ax
loc_1_7087:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ea]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    al, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x70
        je     loc_1_70C5
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x10
        je     loc_1_70C0
        mov    ax, 0xfffd
        jmp    loc_1_70FA
loc_1_70C0:
        jmp    loc_1_70CD
loc_1_70C5:
        sub    ax, ax
        jmp    loc_1_70FA
loc_1_70CD:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        sub    eax, ecx
        cmp    eax, 0xb4
        jle    loc_1_7087
        mov    ax, 0xfffd
        jmp    loc_1_70FA
loc_1_70FA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_7101:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_55FE
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        or     eax, eax
        je     loc_1_712E
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_735C
loc_1_712E:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        mov    al, byte ptr [eax + 0xb5]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_7164
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3460
        or     eax, eax
        je     loc_1_7164
        mov    ax, 0x10
        jmp    loc_1_735C
loc_1_7164:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        or     ecx, 8
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6E0D
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        or     eax, eax
        je     loc_1_71B5
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_735C
loc_1_71B5:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_701B
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        or     eax, eax
        je     loc_1_71D7
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_735C
loc_1_71D7:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_66F6
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        or     eax, eax
        je     loc_1_71F9
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_735C
loc_1_71F9:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5E34
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        or     eax, eax
        je     loc_1_721B
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_735C
loc_1_721B:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1f2]
        mov    ax, 1
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1f0]
        mov    ax, 0xa00
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x1e2]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1f0]
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x1e6]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    ch, cl
        mov    edx, dword ptr [ebp - 4]
        and    edx, 0xffff
        mov    cl, dh
        mov    word ptr [ebp - 4], cx
        mov    cx, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 0x2b2], cx
        mov    cx, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 0x2b8], cx
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x1e6]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    ch, cl
        mov    edx, dword ptr [ebp - 4]
        and    edx, 0xffff
        mov    cl, dh
        mov    word ptr [ebp - 4], cx
        mov    cx, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 0x2b4], cx
        mov    cx, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 0x2ba], cx
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x1e6]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    ch, cl
        mov    edx, dword ptr [ebp - 4]
        and    edx, 0xffff
        mov    cl, dh
        mov    word ptr [ebp - 4], cx
        mov    cx, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 0x2b6], cx
        mov    cx, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 0x2bc], cx
        mov    ecx, dword ptr [ebp + 8]
        dec    dword ptr [ecx + 0x2c6]
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x1ea]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6C5D
        sub    ax, ax
        jmp    loc_1_735C
loc_1_735C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_7363:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x10], ax
        add    dword ptr [ebp - 4], 2
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x12], ax
        add    dword ptr [ebp - 4], 2
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x14], ax
        sub    ax, ax
        jmp    loc_1_73A9
loc_1_73A9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_73B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1a], ax
        add    dword ptr [ebp - 4], 2
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x1c], ax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x2ce]
        or     eax, eax
        je     loc_1_740F
        push   0x24
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1a]
        push   eax
        push   0x900
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
loc_1_740F:
        sub    ax, ax
        jmp    loc_1_7417
loc_1_7417:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_741E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x2ce]
        or     eax, eax
        je     loc_1_7450
        push   0x24
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xe]
        push   eax
        push   0xd00
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
loc_1_7450:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ax, word ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x2b0], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xf9ff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0xe], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0xe], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_741E
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_74AC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x2be], ax
        add    dword ptr [ebp - 4], 2
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x2c0], ax
        add    dword ptr [ebp - 4], 2
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x2c2], ax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x2ce]
        or     eax, eax
        je     loc_1_7558
        push   0x24
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2c2]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2c0]
        push   eax
        push   0x800
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
        movsx  eax, ax
        or     eax, eax
        je     loc_1_7558
        push   0x24
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2be]
        push   eax
        push   0xf00
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
        jmp    loc_1_7560
loc_1_7558:
        sub    ax, ax
        jmp    loc_1_7560
loc_1_7560:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ea]
        mov    ax, 0x8880
        out    dx, ax
        sub    ax, ax
        jmp    loc_1_7585
loc_1_7585:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_758C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x14]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x12]
        and    ecx, 0xffff
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x10]
        and    ecx, 0xffff
        or     eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        or     eax, eax
        jne    loc_1_75E3
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2b2
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_75EC
loc_1_75E3:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x10
        mov    dword ptr [ebp - 8], eax
loc_1_75EC:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x2b8], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x2ba], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x2bc], ax
        push   0x276
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68D4
        mov    word ptr [ebp - 0xc], ax
        movsx  eax, word ptr [ebp - 0xc]
        or     eax, eax
        je     loc_1_764B
        mov    ax, word ptr [ebp - 0xc]
        jmp    loc_1_76C2
loc_1_764B:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2d2]
        and    eax, 0xffff
        test   ah, 0x80
        jne    loc_1_76AE
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2d2]
        and    eax, 0xffff
        test   ah, 0x40
        je     loc_1_7684
        mov    ax, 0xfff3
        jmp    loc_1_76C2
loc_1_7684:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2d2]
        and    eax, 0xffff
        test   ah, 2
        je     loc_1_76A5
        mov    ax, 0xfff5
        jmp    loc_1_76C2
loc_1_76A5:
        mov    ax, 0xfff4
        jmp    loc_1_76C2
loc_1_76AE:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x2ce], 1
        sub    ax, ax
        jmp    loc_1_76C2
loc_1_76C2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x2ce], 0
        push   0x24
        push   0
        push   0
        push   0x700
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
        sub    ax, ax
        jmp    loc_1_76F7
loc_1_76F7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_76FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xfff7
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2dc]
        mov    al, byte ptr [eax + 0xb5]
        and    eax, 0xff
        or     eax, eax
        jne    loc_1_7754
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_34FB
loc_1_7754:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x2ce], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ea]
        mov    ax, 0xff00
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_7777:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        or     eax, 0xc0
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xff7f
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x2a4]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_7802
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        or     ecx, 0x100
        mov    dx, word ptr [eax + 0x1ee]
        mov    ax, cx
        out    dx, ax
loc_1_7802:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_7810:
        inc    ebx
        outsd  dx, dword ptr [esi]
        jo     0x788d
        jb     loc_1_787F
        push   0x43282074
        sub    dword ptr [eax], esp
        xor    dword ptr [ecx], edi
        cmp    dword ptr [ebx], esi
        and    byte ptr [eax + 0x72], dl
        outsd  dx, dword ptr [esi]
        je     0x788d
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        sub    al, 0x20
        dec    ecx
        outsb  dx, byte ptr [esi]
        arpl   word ptr [esi], bp
        and    byte ptr [ecx + 0x6c], al
        insb   byte ptr es:[edi], dx
        and    byte ptr [edx + 0x69], dl
        push   0x52207374
        jae    loc_1_78A5
        jb     0x78b8
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax], bh
        add    byte ptr [eax], al
loc_1_784D:
        add    cl, cl
loc_1_784F:
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    byte ptr [edx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], al
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
loc_1_787F:
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
loc_1_78A5:
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
        push   eax
        push   edx
        dec    edi
        push   esp
        inc    ebp
        dec    edi
        dec    esi
        add    byte ptr [eax + 0x52], dl
        dec    edi
        push   esp
        inc    ebp
        dec    edi
        dec    esi
        add    byte ptr [eax + 0x52], dl
        dec    edi
        push   edx
        inc    ecx
        push   eax
        dec    ebp
        inc    esp
        push   edi
        dec    esi
        add    byte ptr [eax + 0x52], dl
        dec    edi
        push   edx
        inc    ecx
        push   eax
        dec    ebp
        inc    esp
        push   edi
        dec    esi
        add    byte ptr [eax + 0x72], dl
        outsd  dx, dword ptr [esi]
        je     0x79ba
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [eax + 0x31], dl
        xor    edi, dword ptr [ecx]
        js     loc_1_797E
        push   esp
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebp + 0x6e], 0x20
        push   edx
        imul   ebp, dword ptr [esi + 0x67], 0x74654e20
        ja     0x79dd
        jb     loc_1_79DB
        and    byte ptr [ebx + 0x61], al
        jb     loc_1_79D9
        add    byte ptr cs:[eax], al
loc_1_7978:
        push   eax
        jb     0x79ea
        je     loc_1_79E2
        outsd  dx, dword ptr [esi]
loc_1_797E:
        outsb  dx, byte ptr [esi]
        and    byte ptr [eax + 0x31], dl
        cmp    byte ptr [ecx], bh
        js     loc_1_79A6
        push   esp
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebp + 0x6e], 0x20
        push   edx
        imul   ebp, dword ptr [esi + 0x67], 0x74654e20
        ja     loc_1_7A05
        jb     0x7a03
        and    byte ptr [ebx + 0x61], al
        jb     0x7a01
        add    byte ptr cs:[eax], al
loc_1_79A0:
        push   eax
        jb     0x7a12
        je     loc_1_7A0A
        outsd  dx, dword ptr [esi]
loc_1_79A6:
        outsb  dx, byte ptr [esi]
        and    byte ptr [eax + 0x31], dl
        cmp    dword ptr [ecx], edi
        js     loc_1_79CE
        push   esp
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebp + 0x6e], 0x20
        push   edx
        imul   ebp, dword ptr [esi + 0x67], 0x74654e20
        ja     0x7a2d
        jb     loc_1_7A2B
        and    byte ptr [ebx + 0x61], al
        jb     0x7a29
        add    byte ptr cs:[eax], al
        dec    ecx
        outsd  dx, dword ptr [esi]
        inc    edx
        popal  
        jae    0x7a33
loc_1_79CE:
        add    byte ptr [eax], al
loc_1_79D0:
        dec    ecx
        outsd  dx, dword ptr [esi]
        inc    edx
        popal  
        jae    loc_1_7A3B
        add    byte ptr [eax], al
        dec    ecx
loc_1_79D9:
        dec    esi
        push   esp
loc_1_79DB:
        dec    esp
        jbe    loc_1_7A44
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
loc_1_79E2:
        add    byte ptr [eax], al
loc_1_79E4:
        dec    ecx
        dec    esi
        push   esp
        dec    esp
        jbe    loc_1_7A50
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esp
        dec    ebp
        inc    ecx
        inc    ebx
        push   0x656e6e61
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
loc_1_79FC:
        inc    esp
        dec    ebp
        inc    ecx
        inc    ebx
        push   0x656e6e61
loc_1_7A05:
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        inc    ebx
        popal  
loc_1_7A0A:
        jb     0x7a70
        push   ebx
        jo     loc_1_7A74
        add    byte ptr fs:[eax], al
        add    byte ptr [ebx + 0x61], al
        jb     0x7a7c
        push   ebx
        jo     0x7a80
        add    byte ptr fs:[eax], al
        add    byte ptr [ebx + 0x61], al
        bound  ebp, qword ptr [ebp + 0x54]
        jns    loc_1_7A98
        add    byte ptr gs:[eax], al
loc_1_7A2B:
        add    byte ptr [ebx + 0x61], al
        bound  ebp, qword ptr [ebp + 0x54]
        jns    0x7aa4
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 0x6c], dl
        outsd  dx, dword ptr [esi]
loc_1_7A3B:
        je     loc_1_7A8B
loc_1_7A3D:
        jne    loc_1_7AAC
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
loc_1_7A44:
        push   ebx
loc_1_7A45:
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        je     0x7a97
        jne    loc_1_7AB8
loc_1_7A4B:
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
loc_1_7A50:
        push   ebx
        push   esp
        push   eax
loc_1_7A53:
        add    byte ptr [ebx + 0x54], dl
        push   eax
loc_1_7A57:
        add    byte ptr [eax + 0x61], dl
        arpl   word ptr [ebx + 0x65], bp
        je     loc_1_7AB2
        imul   edi, dword ptr [edx + 0x65], 0x61500000
        arpl   word ptr [ebx + 0x65], bp
        je     loc_1_7ABE
        imul   edi, dword ptr [edx + 0x65], 0x4050000
        add    ecx, dword ptr [edi]
loc_1_7A74:
        push   cs
        or     al, 0xb
        or     al, al
        jno    loc_1_7A4B
        jno    loc_1_7A3D
        jne    0x7a4f
        jne    0x7a41
        jns    loc_1_7A53
        jns    loc_1_7A45
loc_1_7A85:
        jge    loc_1_7A57
        jge    loc_1_7A8D
        add    eax, dword ptr [edx]
loc_1_7A8B:
        add    dword ptr [eax], eax
loc_1_7A8D:
        add    byte ptr [eax], cl
        pop    es
        push   es
        add    eax, 0x20300000
        adc    byte ptr [eax], al
loc_1_7A98:
        rol    byte ptr [eax + 0x400000], 0
        add    byte ptr [eax], al
        add    eax, 0xf0b0a09
        add    byte ptr [eax], al
loc_1_7AA8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7AAC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7AB2:
        add    byte ptr [eax], al
loc_1_7AB4:
        add    byte ptr [eax], al
loc_1_7AB6:
        add    byte ptr [eax], al
loc_1_7AB8:
        add    byte ptr [eax], al
loc_1_7ABA:
        add    byte ptr [eax], al
loc_1_7ABC:
        add    byte ptr [eax], al
loc_1_7ABE:
        add    byte ptr [eax], al
loc_1_7AC0:
        add    byte ptr [eax], al
loc_1_7AC2:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x570  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        mov    eax, dword ptr [0x7ac8]       ; -> obj1:0x7AC8
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
        mov    eax, dword ptr [0x7ac8]       ; -> obj1:0x7AC8
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
        push   0x7ac0                        ; -> loc_1_7AC0
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
        push   0x7ac0                        ; -> loc_1_7AC0
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
        mov    al, byte ptr [0x7ac0]         ; -> loc_1_7AC0
        cmp    byte ptr [ecx], al
        jne    loc_2_0408
        mov    al, byte ptr [0x7ac1]         ; -> obj1:0x7AC1
        cmp    byte ptr [ecx + 1], al
        jne    loc_2_0408
        mov    al, byte ptr [0x7ac2]         ; -> loc_1_7AC2
        cmp    byte ptr [ecx + 2], al
        jne    loc_2_0408
        mov    al, byte ptr [0x7ac3]         ; -> obj1:0x7AC3
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
