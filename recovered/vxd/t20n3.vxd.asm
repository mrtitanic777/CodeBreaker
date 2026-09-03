; T20N3 -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 7405 instructions recovered by recursive descent

Declare_Virtual_Device T20N3, 3, 10, T20N3_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x68f0  R-X
loc_1_0000:
        mov    al, byte ptr [0x790]          ; -> obj1:0x0790
        cmp    al, 0
        jne    loc_1_0025
        inc    byte ptr [0x790]              ; -> obj1:0x0790
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
T20N3_Control:
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
        call   sub_1_0604
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
        call   sub_1_0604
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
        db  000h
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
        db  05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch, 000h
sub_1_0521:
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep insw word ptr es:[edi], dx
        pop    edi
        ret    0xc
        db  057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh
        db  05Fh, 0C2h, 00Ch, 000h
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
sub_1_0586:
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
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_05BC:
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
sub_1_05C2:
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
sub_1_05C8:
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
        VxDCall  VPOWERD.Service_804Fh           ; cd204f802800
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_05DA:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_05E0:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_05E6:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_05EC:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_05F8:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_05FE:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_0604:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_0628:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_064C:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_0670:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_067C:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_0682:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_0688:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
sub_1_068E:
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
sub_1_06D0:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_06E4:
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
        dw  0030Ah                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'T20N3   '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  T20N3_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_07A0:
        push   esp
        xor    dh, byte ptr [eax]
        dec    esi
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        dec    ecx
        dec    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        db  000h
loc_1_07B4:
        dec    ecx
        dec    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
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
        add    byte ptr [ecx + 0x4e], cl
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ecx + ecx*2 + 0x4d], dl
        dec    ecx
        dec    esi
        inc    edi
        add    byte ptr [eax], al
loc_1_07E0:
        push   esp
        dec    ecx
        dec    ebp
        dec    ecx
        dec    esi
        inc    edi
        add    byte ptr [eax], al
        inc    esp
        dec    ebp
        inc    ecx
        inc    ebx
        dec    eax
        inc    ecx
        dec    esi
        dec    esi
        inc    ebp
        dec    esp
        add    byte ptr [eax], al
loc_1_07F4:
        inc    esp
        dec    ebp
        inc    ecx
        inc    ebx
        dec    eax
        inc    ecx
        dec    esi
        dec    esi
        inc    ebp
        dec    esp
        add    byte ptr [eax], al
        inc    edx
        inc    ecx
        dec    esi
        inc    esp
        push   edi
        dec    ecx
        inc    esp
        push   esp
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x41], al
        dec    esi
        inc    esp
        push   edi
        dec    ecx
        inc    esp
        push   esp
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x45], cl
        inc    esp
        dec    ecx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x45], cl
        inc    esp
        dec    ecx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x52], dl
        dec    ecx
        add    byte ptr [eax + 0x52], dl
        dec    ecx
        add    byte ptr [ebx + 0x45], dl
        inc    ebx
        db  000h
loc_1_0834:
        push   ebx
        inc    ebp
        inc    ebx
        add    byte ptr [ebp + eax*2 + 0x46], al
        db  000h
loc_1_083C:
        inc    esp
        inc    ebp
        inc    esi
        add    byte ptr [ecx + 0x4c], al
        push   esp
        add    byte ptr [ecx + 0x4c], al
        push   esp
        add    byte ptr [eax + 0x61], dl
        arpl   word ptr [ebx + 0x65], bp
        je     loc_1_08C2
        imul   edi, dword ptr [edx + 0x65], 0x61500000
        arpl   word ptr [ebx + 0x65], bp
        je     loc_1_08CE
        imul   edi, dword ptr [edx + 0x65], 0
        add    byte ptr [eax + 0x54682000], dh
        jae    0x8d3
        arpl   word ptr [eax], sp
        outsd  dx, dword ptr fs:[esi]
        and    byte ptr [ebp + 0x73], ah
        imul   esi, dword ptr [edx + 0x20], 0x6561656c
        jae    loc_1_089B
        outsb  dx, byte ptr fs:[esi]
        jne    0x8e4
        and    byte ptr fs:[edx + 0x69], dh
        insb   byte ptr es:[edi], dx
        arpl   word ptr gs:[ebx + 0x6e], sp
        and    byte ptr [ebp + 0x6e], ah
        dec    edi
        jns    loc_1_08FA
        and    byte ptr [esp + ebp*2], ch
        inc    ecx
        and    byte ptr [ecx + ebp*2 + 0x72], ch
        push   0x72747367
loc_1_089B:
        and    byte ptr [ebx + 0x65], dh
        jb     loc_1_0905
        jbe    loc_1_08D1
        pushal 
        add    dword ptr [edx + 1], eax
        add    byte ptr [eax], cl
        or     byte ptr [esi], dl
        mov    al, 3
        loopne loc_1_08B4
        add    byte ptr [eax], al
        and    byte ptr [ecx], al
loc_1_08B4:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FCh, 013h, 080h, 003h, 0A0h, 007h, 042h, 001h, 000h, 008h
loc_1_08C2:
        and    byte ptr [edi], al
        add    byte ptr [eax], al
        loopne 0x8cc
        add    byte ptr [ecx], al
        mov    eax, dword ptr [edi]
        db  000h, 03Dh
loc_1_08CE:
        pushal 
        db  001h, 042h
loc_1_08D1:
        add    dword ptr [eax - 0x74ecfe00], eax
        pop    es
        add    byte ptr [0x14ac80b], ch
        add    byte ptr [edx], al
        add    byte ptr [eax - 0x37fff660], ah
        add    ax, sp
        add    ah, byte ptr [eax + 0x407a000]
        add    dword ptr [eax], esp
        add    byte ptr [eax + 0x14001], ah
        db  0FEh, 020h, 048h
loc_1_08FA:
        sub    ah, al
        inc    edx
        add    eax, esp
        add    al, 2
        add    eax, esp
        add    al, 0x12
loc_1_0905:
        or     eax, esp
        add    al, 0x82
        add    dword ptr [eax + 1], esp
        sbb    al, 1
        add    al, 0
        add    edx, dword ptr [esi]
        loopne loc_1_0915
        inc    eax
loc_1_0915:
        add    dword ptr [eax], eax
        or     al, 0xa0
        push   es
        scasb  al, byte ptr es:[edi]
        mov    cl, 0xa0
        pop    es
        add    al, 1
        sub    eax, 0xc22000
        loopne 0x8af
        add    ah, byte ptr [ebx]
        loop   loc_1_093F
        push   ss
        mov    al, byte ptr [0x2e010407]
        add    byte ptr [eax + 1], ah
        inc    edx
        add    dword ptr [eax], eax
        add    ecx, dword ptr [0x407a016]
        add    dword ptr [ecx], esp
loc_1_093F:
        add    byte ptr [eax - 0x764ffff9], cl
        pop    es
        db  0FEh, 0FFh, 0A8h, 009h, 0A9h, 009h, 08Ah, 007h, 002h, 0E0h, 0A0h, 006h, 098h, 0F5h, 056h, 010h
        db  088h, 007h, 000h, 0A0h, 089h, 007h, 0FEh, 0AFh, 0A8h, 009h, 0A9h, 009h, 08Ah, 007h, 07Ah, 0E0h
        db  0A0h, 006h, 098h, 0F5h, 04Bh, 010h, 0A0h, 005h, 004h, 001h, 088h, 007h, 008h, 000h, 089h, 007h
        db  07Ah, 000h, 000h, 003h, 001h, 000h, 0A0h, 006h, 0D0h, 0F0h, 040h, 010h, 0A0h, 006h, 0AEh, 0B1h
        db  0E0h, 002h, 0F4h, 003h, 088h, 007h, 0A0h, 000h, 089h, 007h, 0FEh, 000h, 0A0h, 006h, 0D0h, 0F0h
        db  035h, 010h, 0E0h, 002h, 0A0h, 000h, 0E0h, 004h, 07Eh, 001h, 0C8h, 004h, 009h, 002h, 0F2h, 003h
        db  048h, 062h, 0E0h, 0C1h, 040h, 001h, 0A0h, 001h, 040h, 001h, 000h, 004h, 018h, 0CEh, 009h, 006h
        db  0FDh, 016h, 0A0h, 001h, 040h, 001h, 000h, 040h, 007h, 0C8h, 040h, 001h, 088h, 007h, 0F4h, 003h
        db  089h, 007h, 0FEh, 03Fh, 0A0h, 006h, 0D0h, 0F0h, 019h, 010h, 0E0h, 002h, 0A0h, 000h, 0A0h, 006h
        db  0F8h, 0F1h, 014h, 010h, 008h, 0C8h, 044h, 004h, 009h, 0C8h, 046h, 004h, 0A0h, 006h, 026h, 0F1h
        db  00Dh, 010h, 081h, 007h, 07Eh, 0E0h, 0B1h, 0C0h, 026h, 013h, 001h, 0C8h, 0E0h, 000h, 0A0h, 005h
        db  004h, 001h, 092h, 006h, 003h, 010h, 060h, 0C0h, 0E0h, 000h, 0F5h, 010h, 0E0h, 001h, 004h, 001h
        db  010h, 000h, 0B0h, 003h, 0FFh, 010h, 0A0h, 001h, 004h, 001h, 000h, 080h, 080h, 003h, 080h, 007h
        db  0A0h, 000h, 0C2h, 004h, 080h, 0CCh, 081h, 007h, 09Ch, 0B1h, 082h, 002h, 01Eh, 000h, 002h, 016h
        db  081h, 007h, 0A6h, 0B1h, 081h, 0C4h, 081h, 08Ch, 0E9h, 016h, 082h, 002h, 07Ch, 000h, 0F2h, 016h
        db  000h, 003h, 00Fh, 000h, 05Bh, 004h, 081h, 007h, 00Ah, 0E1h, 082h, 007h, 004h, 000h, 0E0h, 004h
        db  080h, 001h, 0E0h, 004h, 082h, 001h, 091h, 0C4h, 0B1h, 08Ch, 0D8h, 016h, 082h, 002h, 07Ch, 000h
        db  0FAh, 016h, 020h, 0C8h, 004h, 0E0h, 082h, 001h, 020h, 0E8h, 00Ch, 0E0h, 082h, 001h, 020h, 0C8h
        db  010h, 0E0h, 080h, 001h, 081h, 007h, 088h, 0E0h, 0B1h, 0C0h, 007h, 013h, 0B1h, 0C4h, 0FCh, 010h
        db  0A0h, 007h, 004h, 001h, 02Eh, 000h, 060h, 004h, 09Ch, 0B1h, 081h, 007h, 034h, 0E0h, 082h, 007h
        db  0FCh, 005h, 083h, 007h, 00Ah, 000h, 0B1h, 0CCh, 043h, 006h, 0FDh, 016h, 002h, 002h, 000h, 006h
        db  060h, 0D0h, 04Eh, 001h, 0EDh, 013h, 021h, 002h, 000h, 0F7h, 021h, 002h, 000h, 0C0h, 081h, 0DCh
        db  060h, 0D0h, 04Fh, 001h, 0C1h, 0C0h, 041h, 009h, 021h, 002h, 000h, 0F0h, 081h, 0DCh, 043h, 002h
        db  000h, 00Fh, 023h, 002h, 000h, 0F0h, 083h, 0DCh, 001h, 002h, 032h, 00Ch, 0A0h, 0C0h, 044h, 004h
        db  0E0h, 0C0h, 046h, 004h, 003h, 0C1h, 002h, 061h, 084h, 005h, 004h, 0C8h, 048h, 004h, 003h, 0C1h
        db  084h, 005h, 004h, 0A1h, 001h, 0A1h, 004h, 0C8h, 030h, 00Ch, 003h, 0C1h, 084h, 005h, 0F1h, 004h
        db  004h, 006h, 0FDh, 016h, 008h, 002h, 000h, 0B0h, 0A8h, 009h, 060h, 0C2h, 030h, 00Ch, 029h, 002h
        db  0FFh, 003h, 0A9h, 009h, 029h, 002h, 040h, 000h, 080h, 007h, 000h, 0A0h, 0A0h, 009h, 08Ah, 007h
        db  0FEh, 0AFh, 02Ah, 002h, 0FFh, 003h, 0AAh, 009h, 001h, 002h, 032h, 00Ch, 005h, 002h, 000h, 000h
        db  003h, 0C1h, 084h, 005h, 011h, 007h, 0C1h, 005h, 085h, 005h, 004h, 006h, 00Bh, 013h, 085h, 080h
        db  0F9h, 01Ah, 005h, 080h, 0F8h, 01Ah, 085h, 082h, 0F5h, 01Ah, 005h, 082h, 0F4h, 01Ah, 045h, 082h
        db  0F1h, 01Ah, 0F1h, 010h, 020h, 02Dh, 002h, 000h, 060h, 001h, 040h, 001h, 000h, 040h, 006h, 016h
        db  08Ah, 007h, 000h, 008h, 0A0h, 001h, 040h, 001h, 000h, 040h, 048h, 010h, 060h, 001h, 042h, 001h
        db  000h, 080h, 006h, 016h, 08Ah, 007h, 000h, 010h, 0A0h, 001h, 042h, 001h, 000h, 080h, 03Eh, 010h
        db  060h, 001h, 002h, 001h, 000h, 010h, 00Ah, 016h, 060h, 001h, 000h, 001h, 000h, 004h, 006h, 016h
        db  08Ah, 007h, 000h, 080h, 0A0h, 001h, 002h, 001h, 000h, 010h, 030h, 010h, 060h, 001h, 002h, 001h
        db  000h, 008h, 00Ah, 016h, 060h, 001h, 000h, 001h, 000h, 004h, 006h, 016h, 0A0h, 001h, 002h, 001h
        db  000h, 008h, 00Dh, 002h, 001h, 000h, 00Dh, 010h, 060h, 001h, 002h, 001h, 000h, 004h, 016h, 016h
        db  060h, 001h, 000h, 001h, 000h, 008h, 012h, 016h, 0A0h, 001h, 002h, 001h, 000h, 004h, 00Dh, 002h
        db  002h, 000h, 0A0h, 0C3h, 00Eh, 001h, 0E0h, 0C3h, 010h, 001h, 08Ah, 007h, 000h, 020h, 060h, 001h
        db  000h, 001h, 000h, 080h, 00Bh, 013h, 08Ah, 007h, 000h, 040h, 008h, 010h, 08Ah, 007h, 004h, 000h
        db  005h, 010h, 08Ah, 007h, 002h, 000h, 002h, 010h, 08Ah, 007h, 008h, 000h, 000h, 003h, 000h, 000h
        db  0E0h, 004h, 082h, 001h, 08Bh, 007h, 0E0h, 005h, 0CAh, 0CEh, 0CDh, 0CEh, 0CEh, 0CEh, 0CFh, 0C6h
        db  020h, 0C3h, 058h, 007h, 020h, 023h, 004h, 0E0h, 012h, 013h, 08Bh, 007h, 018h, 0FFh, 08Ah, 002h
        db  000h, 080h, 00Ah, 013h, 08Bh, 005h, 0CDh, 0A2h, 08Ah, 002h, 000h, 040h, 005h, 013h, 08Ah, 002h
        db  000h, 020h, 002h, 013h, 08Bh, 007h, 01Dh, 0FFh, 00Bh, 0C8h, 004h, 001h, 00Dh, 010h, 020h, 0D3h
        db  005h, 001h, 0FDh, 011h, 020h, 0D8h, 0DFh, 007h, 017h, 001h, 08Bh, 007h, 080h, 0FFh, 00Bh, 0C8h
        db  004h, 001h, 020h, 0E8h, 00Ah, 0E0h, 000h, 001h, 0E0h, 0C2h, 004h, 001h, 0E0h, 022h, 088h, 0E1h
        db  0FBh, 016h, 0E0h, 002h, 0A0h, 000h, 0E0h, 004h, 082h, 001h, 020h, 0E8h, 00Ah, 0E0h, 000h, 001h
        db  0E0h, 0C2h, 000h, 001h, 0E0h, 022h, 006h, 0E0h, 0F8h, 013h, 0A0h, 001h, 040h, 001h, 000h, 0F6h
        db  060h, 004h, 082h, 0B0h, 000h, 003h, 002h, 000h, 0A0h, 007h, 002h, 001h, 0FFh, 0DFh, 090h, 003h
        db  0FFh, 0FFh, 080h, 003h, 000h, 003h, 002h, 000h, 009h, 007h, 0A0h, 0C2h, 004h, 001h, 08Ah, 001h
        db  080h, 000h, 04Ah, 052h, 089h, 0D2h, 00Ah, 0C8h, 004h, 001h, 0A0h, 0D2h, 004h, 001h, 0F9h, 016h
        db  049h, 005h, 089h, 001h, 000h, 080h, 049h, 001h, 000h, 040h, 00Eh, 013h, 009h, 0F8h, 03Ah, 007h
        db  060h, 0C2h, 036h, 007h, 003h, 016h, 090h, 003h, 0FFh, 0FFh, 080h, 003h, 0E0h, 004h, 036h, 007h
        db  054h, 004h, 090h, 003h, 0FFh, 0FFh, 080h, 003h, 060h, 022h, 088h, 0E1h, 0C2h, 013h, 0E0h, 004h
        db  082h, 001h, 060h, 004h, 0D2h, 0B3h, 000h, 003h, 002h, 000h, 0C9h, 004h, 0A0h, 0C1h, 034h, 006h
        db  004h, 016h, 0A0h, 006h, 010h, 0C2h, 0E0h, 004h, 0D2h, 008h, 086h, 007h, 0E8h, 005h, 0A0h, 001h
        db  040h, 001h, 000h, 080h, 020h, 0C2h, 084h, 001h, 020h, 048h, 008h, 0E0h, 084h, 001h, 020h, 022h
        db  008h, 0E0h, 008h, 013h, 060h, 001h, 0AEh, 001h, 001h, 000h, 004h, 016h, 0E0h, 001h, 034h, 006h
        db  000h, 080h, 006h, 010h, 020h, 0C2h, 0E4h, 008h, 006h, 013h, 0E0h, 001h, 034h, 006h, 000h, 008h
        db  0E0h, 004h, 030h, 006h, 009h, 007h, 0A0h, 005h, 0EEh, 005h, 020h, 006h, 0ECh, 005h, 002h, 016h
        db  016h, 0C2h, 003h, 016h, 049h, 0C2h, 012h, 016h, 080h, 003h, 098h, 0C5h, 0E8h, 0C1h, 002h, 000h
        db  0E0h, 0E9h, 014h, 0E0h, 004h, 000h, 0D7h, 004h, 027h, 002h, 008h, 000h, 0A0h, 006h, 0A6h, 0C1h
        db  016h, 0C2h, 004h, 013h, 028h, 0C8h, 008h, 000h, 0ECh, 005h, 0EFh, 013h, 054h, 004h, 000h, 003h
        db  002h, 000h, 0E0h, 0C1h, 086h, 001h, 047h, 002h, 00Eh, 000h, 0A7h, 0C2h, 092h, 0E1h, 05Ah, 004h
        db  08Ah, 007h, 000h, 0A0h, 00Ah, 0C8h, 086h, 001h, 0C7h, 0A1h, 027h, 002h, 09Ah, 0E1h, 037h, 0E8h
        db  034h, 006h, 017h, 0E8h, 0D2h, 006h, 0E0h, 004h, 030h, 006h, 060h, 004h, 050h, 0B9h, 00Ah, 0E8h
        db  0D2h, 006h, 0E0h, 001h, 034h, 006h, 000h, 008h, 0E0h, 004h, 030h, 006h, 020h, 0E0h, 018h, 0E0h
        db  05Bh, 004h, 0A0h, 005h, 0D2h, 008h, 020h, 088h, 0D2h, 008h, 016h, 0E0h, 0E5h, 01Ah, 0E0h, 004h
        db  0D2h, 008h, 0A0h, 006h, 012h, 0E0h, 080h, 003h, 0A0h, 005h, 0E4h, 008h, 080h, 003h, 001h, 0C3h
        db  0FBh, 013h, 060h, 001h, 018h, 009h, 001h, 000h, 002h, 013h, 0A0h, 005h, 0E4h, 008h, 000h, 003h
        db  002h, 000h, 0E0h, 0C0h, 06Ch, 001h, 020h, 0C3h, 08Ah, 001h, 020h, 098h, 083h, 001h, 002h, 0E2h
        db  03Ch, 016h, 041h, 0C0h, 006h, 013h, 001h, 0C8h, 06Ch, 001h, 0E0h, 0C2h, 002h, 0FCh, 001h, 011h
        db  070h, 010h, 060h, 001h, 09Ch, 001h, 040h, 000h, 06Eh, 016h, 020h, 0D8h, 0E1h, 008h, 083h, 001h
        db  041h, 0C0h, 004h, 013h, 001h, 0C8h, 08Ah, 001h, 001h, 0C8h, 0CAh, 008h, 086h, 007h, 043h, 000h
        db  006h, 0C8h, 06Ch, 001h, 007h, 002h, 002h, 0FCh, 017h, 0C2h, 060h, 004h, 0DEh, 0B6h, 0E0h, 004h
        db  0CAh, 008h, 0C7h, 061h, 008h, 007h, 060h, 001h, 006h, 0FCh, 040h, 000h, 002h, 013h, 008h, 002h
        db  001h, 000h, 009h, 010h, 04Ch, 0C2h, 020h, 0C3h, 000h, 0FCh, 02Eh, 013h, 00Ch, 0C8h, 06Ch, 001h
        db  0E0h, 0C2h, 002h, 0FCh, 01Fh, 011h, 04Bh, 001h, 000h, 001h, 0F4h, 016h, 0C8h, 022h, 016h, 013h
        db  0CBh, 001h, 000h, 040h, 00Bh, 0C8h, 002h, 0FCh, 011h, 010h, 060h, 001h, 09Ch, 001h, 040h, 000h
        db  038h, 016h, 0E0h, 0C1h, 0CAh, 008h, 001h, 0C3h, 021h, 013h, 04Ch, 0C2h, 015h, 013h, 00Ch, 0C8h
        db  06Ch, 001h, 0E0h, 0C2h, 002h, 0FCh, 006h, 011h, 0CCh, 081h, 0D1h, 013h, 04Ch, 0C2h, 020h, 0C3h
        db  000h, 0FCh, 0F4h, 010h, 009h, 0C8h, 06Ch, 001h, 0E0h, 0C2h, 002h, 0FCh, 01Eh, 016h, 0A0h, 007h
        db  002h, 0FCh, 000h, 080h, 009h, 0C3h, 019h, 010h, 009h, 0C8h, 06Ch, 001h, 0E0h, 0C2h, 002h, 0FCh
        db  005h, 016h, 0A0h, 007h, 002h, 0FCh, 000h, 080h, 009h, 0C3h, 00Fh, 010h, 0E0h, 0C2h, 002h, 00Ch
        db  001h, 011h, 00Fh, 010h, 020h, 0D8h, 002h, 0E2h, 083h, 001h, 08Bh, 009h, 08Bh, 009h, 08Bh, 009h
        db  08Bh, 009h, 0A0h, 007h, 08Ah, 001h, 043h, 000h, 004h, 010h, 00Ch, 0C8h, 08Ah, 001h, 00Ch, 0C8h
        db  0CAh, 008h, 001h, 083h, 02Fh, 016h, 003h, 0C8h, 06Ch, 001h, 040h, 001h, 010h, 000h, 017h, 016h
        db  0E0h, 0C2h, 02Eh, 006h, 003h, 013h, 0E0h, 0C2h, 0F8h, 005h, 003h, 016h, 0E0h, 0C2h, 014h, 009h
        db  00Eh, 013h, 0E0h, 001h, 03Ah, 007h, 000h, 080h, 080h, 001h, 010h, 000h, 0E0h, 0C2h, 036h, 007h
        db  006h, 013h, 0E0h, 004h, 036h, 007h, 080h, 001h, 020h, 000h, 060h, 004h, 050h, 0B9h, 040h, 001h
        db  020h, 000h, 0F9h, 013h, 090h, 003h, 0FFh, 011h, 080h, 003h, 008h, 001h, 000h, 004h, 014h, 016h
        db  060h, 001h, 018h, 009h, 001h, 000h, 010h, 016h, 088h, 001h, 000h, 01Ah, 0C8h, 001h, 000h, 001h
        db  0C8h, 0C5h, 00Ah, 010h, 081h, 0C1h, 001h, 0C8h, 06Ch, 001h, 007h, 002h, 000h, 0FCh, 077h, 0C0h
        db  017h, 0C2h, 048h, 001h, 000h, 018h, 0E9h, 013h, 040h, 001h, 040h, 000h, 01Bh, 016h, 080h, 001h
        db  045h, 000h, 046h, 0C1h, 020h, 0D0h, 007h, 0FCh, 020h, 028h, 006h, 0E0h, 060h, 081h, 0CAh, 008h
        db  004h, 016h, 0E0h, 004h, 0CAh, 008h, 0C0h, 001h, 004h, 000h, 0E0h, 0C2h, 008h, 0FCh, 008h, 011h
        db  0E0h, 0C2h, 00Eh, 0FCh, 007h, 015h, 006h, 013h, 0E0h, 0C2h, 014h, 0FCh, 003h, 015h, 002h, 013h
        db  0C0h, 001h, 001h, 000h, 048h, 001h, 000h, 001h, 005h, 013h, 000h, 001h, 080h, 040h, 06Dh, 013h
        db  060h, 004h, 066h, 0B5h, 0C0h, 001h, 040h, 000h, 040h, 001h, 004h, 000h, 00Bh, 016h, 048h, 001h
        db  001h, 000h, 003h, 016h, 040h, 001h, 000h, 040h, 005h, 013h, 0C8h, 001h, 000h, 040h, 0A0h, 005h
        db  0E4h, 008h, 0C8h, 0C5h, 0B7h, 001h, 020h, 000h, 0D7h, 0C2h, 0C4h, 062h, 00Bh, 005h, 02Bh, 002h
        db  0FCh, 0FFh, 0CBh, 0C5h, 002h, 015h, 046h, 081h, 063h, 013h, 008h, 001h, 000h, 05Eh, 060h, 016h
        db  008h, 001h, 088h, 000h, 00Dh, 013h, 040h, 001h, 001h, 000h, 00Ah, 013h, 008h, 001h, 002h, 000h
        db  007h, 016h, 048h, 001h, 001h, 000h, 004h, 016h, 08Ah, 007h, 080h, 000h, 0A0h, 006h, 01Eh, 0B5h
        db  086h, 002h, 043h, 000h, 010h, 016h, 040h, 001h, 000h, 040h, 00Bh, 016h, 008h, 001h, 003h, 000h
        db  008h, 013h, 084h, 0C2h, 02Ah, 002h, 0D8h, 0FFh, 006h, 0C8h, 06Ch, 001h, 00Ah, 068h, 004h, 0FCh
        db  05Fh, 010h, 060h, 004h, 0B2h, 0B8h, 000h, 0C0h, 0FCh, 011h, 060h, 0C2h, 018h, 009h, 040h, 001h
        db  000h, 040h, 00Ah, 016h, 048h, 001h, 001h, 000h, 035h, 013h, 048h, 001h, 002h, 000h, 00Ah, 013h
        db  049h, 001h, 004h, 000h, 0EEh, 016h, 006h, 010h, 049h, 001h, 002h, 000h, 003h, 013h, 008h, 001h
        db  003h, 000h, 06Fh, 013h, 049h, 001h, 001h, 000h, 00Dh, 016h, 006h, 0C8h, 06Ch, 001h, 007h, 002h
        db  002h, 0FCh, 0E0h, 0A1h, 0DEh, 008h, 0E0h, 0CDh, 0EEh, 005h, 0E0h, 0C5h, 004h, 0FCh, 020h, 0C8h
        db  0DEh, 008h, 004h, 0FCh, 000h, 001h, 080h, 040h, 001h, 016h, 046h, 0C1h, 0E0h, 004h, 000h, 0FCh
        db  087h, 007h, 0F8h, 005h, 017h, 0C2h, 008h, 013h, 0C7h, 005h, 017h, 0C8h, 06Ch, 001h, 005h, 0C8h
        db  000h, 0FCh, 0C6h, 0C5h, 060h, 004h, 066h, 0B5h, 0C5h, 0CDh, 0C6h, 0C5h, 060h, 004h, 066h, 0B5h
        db  060h, 004h, 096h, 0B8h, 006h, 0C8h, 06Ch, 001h, 085h, 081h, 01Ah, 013h, 0E0h, 0C2h, 004h, 0FCh
        db  017h, 015h, 086h, 0C2h, 08Ah, 0A2h, 0AAh, 0C1h, 032h, 00Ch, 006h, 0C8h, 06Ch, 001h, 00Bh, 0A8h
        db  004h, 0FCh, 01Ah, 009h, 00Ah, 0C8h, 06Ch, 001h, 0E0h, 0C2h, 002h, 0FCh, 0E0h, 004h, 000h, 0FCh
        db  006h, 0C8h, 06Ch, 001h, 00Bh, 0C8h, 002h, 0FCh, 0A0h, 006h, 012h, 0C1h, 006h, 0C8h, 06Ch, 001h
        db  0E0h, 004h, 000h, 0FCh, 0A0h, 001h, 002h, 0FCh, 002h, 000h, 087h, 007h, 030h, 006h, 0E7h, 001h
        db  004h, 000h, 040h, 000h, 0D7h, 004h, 027h, 002h, 00Ch, 000h, 005h, 0C2h, 060h, 001h, 018h, 009h
        db  004h, 000h, 003h, 016h, 0E0h, 001h, 002h, 0FCh, 020h, 000h, 0A0h, 006h, 0BCh, 0C1h, 0C0h, 001h
        db  020h, 000h, 060h, 004h, 066h, 0B5h, 048h, 001h, 000h, 018h, 003h, 013h, 048h, 001h, 000h, 010h
        db  002h, 016h, 0A0h, 005h, 0E4h, 008h, 086h, 002h, 043h, 000h, 003h, 013h, 000h, 001h, 080h, 040h
        db  0A4h, 013h, 006h, 0C8h, 06Ch, 001h, 0E0h, 004h, 000h, 0FCh, 085h, 0C2h, 0A0h, 006h, 012h, 0C1h
        db  0E8h, 010h, 000h, 003h, 002h, 000h, 020h, 0C2h, 08Ch, 001h, 0E0h, 0C0h, 06Ch, 001h, 020h, 0C2h
        db  0F4h, 005h, 028h, 0C8h, 008h, 000h, 06Ch, 001h, 0E8h, 0C1h, 00Ah, 000h, 020h, 0C3h, 002h, 0FCh
        db  08Ch, 001h, 020h, 000h, 00Ch, 0C8h, 002h, 0FCh, 00Ch, 001h, 000h, 0FEh, 03Fh, 016h, 047h, 001h
        db  040h, 000h, 054h, 013h, 0E0h, 004h, 0F4h, 005h, 00Ch, 0CAh, 008h, 000h, 047h, 001h, 008h, 000h
        db  006h, 016h, 0A8h, 0C2h, 006h, 000h, 0A0h, 006h, 012h, 0C1h, 0E8h, 004h, 006h, 000h, 007h, 001h
        db  020h, 000h, 029h, 016h, 0E8h, 0C1h, 002h, 000h, 0E7h, 001h, 004h, 000h, 020h, 000h, 0D7h, 0C2h
        db  003h, 013h, 0D7h, 004h, 0C3h, 001h, 000h, 080h, 027h, 002h, 010h, 000h, 0A0h, 006h, 0A6h, 0C1h
        db  020h, 0C2h, 0F0h, 005h, 007h, 013h, 0E0h, 0C2h, 0F6h, 005h, 004h, 016h, 018h, 0C8h, 0F0h, 005h
        db  0A0h, 006h, 0BCh, 0BBh, 003h, 0C8h, 06Ch, 001h, 003h, 011h, 090h, 003h, 0FFh, 011h, 080h, 003h
        db  090h, 003h, 0F8h, 011h, 0E0h, 002h, 0C0h, 000h, 060h, 0C3h, 0FAh, 000h, 0A0h, 0C3h, 0FCh, 000h
        db  0E0h, 0C3h, 0FEh, 000h, 054h, 004h, 0E8h, 004h, 002h, 000h, 0E2h, 010h, 0E8h, 0C2h, 008h, 000h
        db  0A8h, 0C2h, 006h, 000h, 00Ch, 0C3h, 033h, 011h, 020h, 023h, 00Ah, 0E0h, 045h, 013h, 020h, 023h
        db  010h, 0E0h, 046h, 013h, 020h, 023h, 00Eh, 0E0h, 013h, 013h, 0E0h, 021h, 016h, 0E0h, 0B2h, 016h
        db  020h, 023h, 006h, 0E0h, 003h, 016h, 020h, 027h, 06Ah, 0E4h, 00Ah, 013h, 0E8h, 0C2h, 008h, 000h
        db  0A8h, 0C2h, 006h, 000h, 04Ch, 001h, 088h, 000h, 0A5h, 016h, 00Ch, 001h, 044h, 000h, 0A2h, 016h
        db  020h, 006h, 0C8h, 008h, 09Fh, 013h, 00Ah, 0C8h, 06Ch, 001h, 020h, 0C8h, 004h, 0E0h, 002h, 0FCh
        db  00Bh, 0C8h, 06Ch, 001h, 0A0h, 007h, 002h, 0FCh, 000h, 081h, 020h, 0C3h, 080h, 001h, 0A0h, 001h
        db  080h, 001h, 000h, 0C4h, 00Ch, 0C8h, 080h, 001h, 00Ah, 0C8h, 08Ch, 001h, 0B3h, 010h, 00Ah, 0C2h
        db  00Fh, 013h, 008h, 0C8h, 06Ch, 001h, 0A0h, 0C2h, 000h, 0FCh, 020h, 0C3h, 002h, 0FCh, 020h, 023h
        db  012h, 0E0h, 0F5h, 016h, 00Bh, 0C8h, 06Ch, 001h, 00Ch, 0C8h, 002h, 0FCh, 060h, 004h, 0CEh, 0B8h
        db  08Ah, 007h, 000h, 004h, 060h, 004h, 07Ch, 0B3h, 08Ah, 007h, 020h, 000h, 060h, 004h, 07Ch, 0B3h
        db  08Ah, 007h, 000h, 002h, 020h, 027h, 00Eh, 0E0h, 002h, 016h, 0A0h, 006h, 01Eh, 0B5h, 0A8h, 0C2h
        db  006h, 000h, 060h, 004h, 0F4h, 0B8h, 000h, 003h, 002h, 000h, 0C0h, 001h, 010h, 000h, 0E0h, 0C2h
        db  02Eh, 006h, 003h, 013h, 0E0h, 0C2h, 0F8h, 005h, 003h, 016h, 0E0h, 0C2h, 014h, 009h, 005h, 013h
        db  0E0h, 001h, 03Ah, 007h, 000h, 080h, 080h, 001h, 010h, 000h, 090h, 003h, 0FFh, 07Fh, 080h, 003h
        db  000h, 003h, 002h, 000h, 020h, 0C2h, 0F6h, 005h, 020h, 0E2h, 0F4h, 005h, 02Fh, 016h, 020h, 0D8h
        db  0E0h, 008h, 080h, 001h, 02Bh, 0C8h, 006h, 000h, 08Ch, 001h, 0A0h, 007h, 0C8h, 008h, 004h, 000h
        db  00Bh, 0C8h, 0F4h, 005h, 090h, 003h, 0FFh, 0FFh, 080h, 003h, 000h, 003h, 002h, 000h, 00Bh, 0C2h
        db  020h, 0C3h, 0F4h, 005h, 00Fh, 013h, 0A8h, 0C2h, 00Ah, 000h, 04Ah, 001h, 010h, 000h, 017h, 016h
        db  0A0h, 022h, 004h, 0E0h, 01Bh, 016h, 008h, 0C3h, 0A0h, 006h, 07Ah, 0BBh, 00Ch, 0C2h, 020h, 0C3h
        db  0F4h, 005h, 014h, 016h, 068h, 001h, 00Ah, 000h, 010h, 000h, 003h, 013h, 0E0h, 0C2h, 0F6h, 005h
        db  006h, 016h, 0A0h, 006h, 0BCh, 0BBh, 090h, 003h, 0FFh, 0FFh, 080h, 003h, 00Bh, 0C2h, 087h, 007h
        db  0F0h, 005h, 0A0h, 006h, 0A6h, 0C1h, 090h, 003h, 0FFh, 0FFh, 080h, 003h, 087h, 007h, 0F0h, 005h
        db  0A0h, 006h, 0ECh, 0C1h, 080h, 003h, 000h, 003h, 002h, 000h, 087h, 007h, 0F0h, 005h, 0CBh, 0C2h
        db  008h, 016h, 0A0h, 006h, 07Ah, 0BBh, 020h, 007h, 0F6h, 005h, 060h, 0CBh, 0F4h, 005h, 002h, 000h
        db  080h, 003h, 0E0h, 004h, 0F6h, 005h, 020h, 0C2h, 0F4h, 005h, 005h, 016h, 017h, 0C2h, 003h, 013h
        db  0D8h, 0C5h, 0A0h, 006h, 0BCh, 0BBh, 080h, 003h, 000h, 003h, 002h, 000h, 08Bh, 0C2h, 0A0h, 006h
        db  07Ah, 0BBh, 0CAh, 0C1h, 027h, 002h, 010h, 000h, 088h, 007h, 0F0h, 005h, 088h, 0C1h, 018h, 0C2h
        db  026h, 013h, 0A8h, 082h, 002h, 000h, 0FAh, 016h, 0E8h, 0C2h, 00Ah, 000h, 0E0h, 022h, 01Eh, 0E0h
        db  0F5h, 016h, 098h, 0C5h, 0E0h, 022h, 01Ch, 0E0h, 00Bh, 016h, 028h, 0C8h, 006h, 000h, 0F4h, 000h
        db  0E0h, 002h, 0E0h, 000h, 0A0h, 006h, 012h, 0C1h, 0E0h, 002h, 0C0h, 000h, 0E8h, 004h, 006h, 000h
        db  0E0h, 022h, 018h, 0E0h, 0E4h, 013h, 020h, 0EAh, 022h, 0E0h, 00Ah, 000h, 0A0h, 0EAh, 018h, 0E0h
        db  004h, 000h, 0DAh, 004h, 0A0h, 006h, 0A6h, 0C1h, 047h, 006h, 006h, 0C2h, 0D8h, 010h, 006h, 0C8h
        db  0F2h, 005h, 060h, 0CBh, 0F4h, 005h, 002h, 000h, 054h, 004h, 020h, 0C2h, 0F4h, 005h, 013h, 013h
        db  0E0h, 001h, 09Ch, 001h, 000h, 040h, 08Bh, 00Bh, 08Bh, 00Bh, 060h, 001h, 09Ch, 001h, 000h, 040h
        db  00Ah, 016h, 060h, 0C2h, 06Ch, 001h, 028h, 0C8h, 006h, 000h, 06Ch, 001h, 0A0h, 0C2h, 002h, 0FCh
        db  003h, 011h, 009h, 0C8h, 06Ch, 001h, 05Bh, 004h, 009h, 0C8h, 06Ch, 001h, 04Bh, 0C2h, 087h, 007h
        db  0F0h, 005h, 0A0h, 006h, 0ECh, 0C1h, 0E0h, 004h, 0F4h, 005h, 059h, 004h, 0A8h, 0C2h, 00Ah, 000h
        db  00Dh, 011h, 0A0h, 0F2h, 0E0h, 008h, 00Ah, 0D8h, 080h, 001h, 028h, 0C8h, 006h, 000h, 08Ch, 001h
        db  0A0h, 007h, 0C8h, 008h, 004h, 000h, 008h, 0C8h, 0F4h, 005h, 05Bh, 004h, 020h, 0C3h, 06Ch, 001h
        db  028h, 0C8h, 006h, 000h, 06Ch, 001h, 020h, 0C8h, 00Eh, 0FCh, 0BCh, 001h, 020h, 0C8h, 010h, 0FCh
        db  0B0h, 001h, 020h, 0C8h, 012h, 0FCh, 0B2h, 001h, 00Ch, 0C8h, 06Ch, 001h, 0A0h, 0F2h, 0E0h, 008h
        db  08Ah, 001h, 000h, 010h, 0A0h, 001h, 080h, 001h, 000h, 0C4h, 0DDh, 010h, 033h, 007h, 033h, 007h
        db  00Ch, 010h, 013h, 007h, 023h, 007h, 002h, 000h, 0CBh, 0C8h, 006h, 000h, 023h, 002h, 018h, 000h
        db  0E0h, 0CCh, 06Ch, 001h, 0CDh, 0CCh, 0CEh, 0CCh, 0CFh, 0CCh, 083h, 007h, 030h, 006h, 0D3h, 0C1h
        db  00Ah, 013h, 083h, 007h, 036h, 007h, 0D3h, 0C1h, 006h, 013h, 083h, 007h, 0A0h, 000h, 093h, 000h
        db  00Ch, 0C8h, 06Ch, 001h, 080h, 003h, 063h, 007h, 002h, 000h, 039h, 015h, 063h, 0C2h, 004h, 000h
        db  063h, 042h, 006h, 000h, 0DBh, 013h, 063h, 0C3h, 01Ah, 000h, 049h, 0D2h, 00Ch, 013h, 0C9h, 006h
        db  049h, 072h, 069h, 0D2h, 0C2h, 0E1h, 0C9h, 006h, 049h, 072h, 0E9h, 048h, 004h, 0E0h, 004h, 000h
        db  049h, 0CBh, 002h, 000h, 052h, 004h, 069h, 0D2h, 0C2h, 0E1h, 0C9h, 006h, 049h, 072h, 029h, 002h
        db  010h, 000h, 0C3h, 0C2h, 0E9h, 0A2h, 0AAh, 0E1h, 01Bh, 0C3h, 089h, 002h, 012h, 000h, 00Dh, 016h
        db  00Ch, 0C8h, 06Ch, 001h, 0E0h, 0C6h, 000h, 0FCh, 00Dh, 016h, 0E9h, 048h, 004h, 0E0h, 004h, 000h
        db  049h, 0CBh, 002h, 000h, 04Ch, 0CBh, 004h, 000h, 052h, 004h, 0DCh, 0C6h, 003h, 016h, 0E9h, 048h
        db  004h, 0E0h, 004h, 000h, 049h, 0CBh, 002h, 000h, 04Ch, 0CBh, 004h, 000h, 052h, 004h, 090h, 003h
        db  0FFh, 001h, 093h, 000h, 00Ch, 0C8h, 06Ch, 001h, 080h, 003h, 000h, 003h, 002h, 000h, 0DBh, 0C2h
        db  063h, 0C2h, 004h, 000h, 04Bh, 042h, 09Dh, 013h, 049h, 0D2h, 00Eh, 013h, 0C9h, 006h, 049h, 072h
        db  069h, 0D2h, 0C2h, 0E1h, 0C9h, 006h, 049h, 072h, 0E9h, 048h, 004h, 0E0h, 004h, 000h, 049h, 0CBh
        db  002h, 000h, 090h, 003h, 0FFh, 0FFh, 080h, 003h, 069h, 0D2h, 0C2h, 0E1h, 0C9h, 006h, 049h, 072h
        db  029h, 002h, 010h, 000h, 0C3h, 0C2h, 0E9h, 0A2h, 0AAh, 0E1h, 01Bh, 0C3h, 089h, 002h, 012h, 000h
        db  00Fh, 016h, 00Ch, 0C8h, 06Ch, 001h, 0E0h, 0C6h, 000h, 0FCh, 003h, 016h, 0E9h, 048h, 004h, 0E0h
        db  004h, 000h, 049h, 0CBh, 002h, 000h, 04Ch, 0CBh, 004h, 000h, 090h, 003h, 0FFh, 0FFh, 080h, 003h
        db  0DCh, 0C6h, 0F4h, 013h, 0F6h, 010h, 000h, 003h, 002h, 000h, 0BBh, 0C2h, 0BBh, 0C1h, 086h, 0D1h
        db  003h, 013h, 086h, 0EAh, 004h, 000h, 013h, 010h, 0A6h, 0D1h, 0C2h, 0E1h, 0C6h, 006h, 086h, 071h
        db  0CAh, 0C1h, 0E6h, 0A1h, 0BAh, 0E1h, 0A6h, 0EAh, 014h, 0E0h, 004h, 000h, 01Bh, 0C2h, 086h, 002h
        db  002h, 000h, 003h, 016h, 0A0h, 006h, 0CCh, 0C1h, 002h, 010h, 0A0h, 006h, 0A6h, 0C1h, 0DAh, 004h
        db  080h, 003h, 000h, 003h, 002h, 000h, 0ABh, 0C2h, 006h, 000h, 08Ch, 007h, 0E8h, 005h, 05Ch, 0C2h
        db  016h, 013h, 0A0h, 0C1h, 0ECh, 005h, 08Ah, 081h, 01Ah, 01Ah, 0C6h, 0C1h, 009h, 0C2h, 059h, 0C2h
        db  020h, 013h, 0E9h, 0A1h, 008h, 000h, 087h, 082h, 0F9h, 012h, 0A9h, 0A2h, 008h, 000h, 087h, 062h
        db  0CAh, 0CAh, 008h, 000h, 04Ah, 06Ah, 008h, 000h, 0C9h, 0C6h, 00Bh, 0C6h, 080h, 003h, 0CAh, 0CAh
        db  008h, 000h, 00Ah, 0C8h, 0ECh, 005h, 0DBh, 004h, 00Bh, 0CFh, 00Bh, 0C7h, 080h, 003h, 08Ah, 061h
        db  046h, 0CAh, 008h, 000h, 0CAh, 0CAh, 008h, 000h, 00Ah, 0C8h, 0ECh, 005h, 0C9h, 0C6h, 00Bh, 0C7h
        db  080h, 003h, 087h, 062h, 0CAh, 0CAh, 008h, 000h, 0DBh, 004h, 00Bh, 0C6h, 00Bh, 0CBh, 002h, 000h
        db  080h, 003h, 000h, 003h, 002h, 000h, 0BBh, 0C1h, 0DBh, 0C2h, 08Ch, 007h, 0E8h, 005h, 04Ch, 0C2h
        db  0EDh, 004h, 002h, 000h, 009h, 0C2h, 059h, 0C2h, 018h, 013h, 0A9h, 081h, 002h, 000h, 0FAh, 016h
        db  0E9h, 082h, 004h, 000h, 0F7h, 016h, 049h, 0CBh, 004h, 000h, 099h, 0C2h, 00Ah, 0C6h, 00Ah, 013h
        db  008h, 083h, 004h, 013h, 0A9h, 0AAh, 008h, 000h, 008h, 000h, 080h, 003h, 02Ah, 0A8h, 008h, 000h
        db  0ECh, 005h, 080h, 003h, 008h, 0CBh, 002h, 000h, 080h, 003h, 02Dh, 007h, 002h, 000h, 08Ch, 007h
        db  008h, 000h, 006h, 0A3h, 04Ch, 0C2h, 009h, 0C2h, 059h, 0C2h, 013h, 013h, 0E9h, 082h, 004h, 000h
        db  0FAh, 016h, 0ADh, 007h, 002h, 000h, 001h, 000h, 049h, 0CBh, 004h, 000h, 019h, 0C6h, 001h, 013h
        db  080h, 003h, 008h, 083h, 004h, 016h, 0A0h, 049h, 014h, 0E0h, 004h, 000h, 080h, 003h, 008h, 0CBh
        db  002h, 000h, 080h, 003h, 000h, 003h, 002h, 000h, 00Bh, 006h, 015h, 011h, 043h, 013h, 08Bh, 007h
        db  000h, 04Eh, 060h, 001h, 042h, 001h, 080h, 000h, 002h, 013h, 08Bh, 007h, 000h, 03Ah, 00Bh, 0C8h
        db  044h, 001h, 0E0h, 001h, 040h, 001h, 000h, 002h, 0E0h, 001h, 040h, 001h, 000h, 020h, 084h, 007h
        db  01Ch, 0BCh, 060h, 004h, 02Ah, 0BCh, 020h, 0C8h, 016h, 0E0h, 0E0h, 000h, 0E0h, 0C2h, 018h, 009h
        db  0E0h, 022h, 010h, 0E0h, 003h, 016h, 020h, 0E8h, 014h, 0E0h, 0E0h, 000h, 020h, 0C8h, 004h, 0E0h
        db  082h, 001h, 020h, 0C8h, 0E2h, 000h, 08Ah, 001h, 0E0h, 004h, 0CAh, 008h, 0E0h, 004h, 0F4h, 005h
        db  0E0h, 004h, 0F8h, 005h, 0E0h, 004h, 0F0h, 005h, 0E0h, 004h, 042h, 007h, 0A0h, 007h, 088h, 001h
        db  020h, 000h, 0E0h, 0C2h, 0E2h, 008h, 009h, 013h, 0A0h, 007h, 088h, 001h, 080h, 000h, 020h, 0E8h
        db  016h, 0E0h, 080h, 001h, 0E0h, 001h, 082h, 001h, 000h, 003h, 08Bh, 007h, 000h, 0A0h, 00Bh, 0E8h
        db  086h, 001h, 080h, 003h, 0E0h, 004h, 086h, 001h, 0E0h, 001h, 09Ch, 001h, 040h, 000h, 0E0h, 001h
        db  09Ch, 001h, 000h, 040h, 0CBh, 004h, 0B0h, 003h, 00Bh, 006h, 004h, 013h, 060h, 001h, 09Ch, 001h
        db  000h, 040h, 0F9h, 016h, 0E0h, 004h, 080h, 001h, 020h, 0E8h, 008h, 0E0h, 018h, 009h, 08Bh, 007h
        db  000h, 080h, 00Bh, 0C8h, 098h, 007h, 00Bh, 0C8h, 078h, 007h, 08Bh, 007h, 06Fh, 087h, 00Bh, 048h
        db  03Ah, 007h, 0E0h, 0C2h, 050h, 007h, 08Bh, 002h, 058h, 007h, 010h, 013h, 020h, 0E8h, 00Ah, 0E0h
        db  000h, 001h, 0E0h, 0C2h, 000h, 001h, 0E0h, 022h, 006h, 0E0h, 0F8h, 013h, 08Bh, 007h, 058h, 007h
        db  00Bh, 0C8h, 050h, 007h, 08Bh, 007h, 05Ch, 0C4h, 00Bh, 0C8h, 052h, 007h, 080h, 003h, 000h, 003h
        db  002h, 000h, 0E0h, 0C2h, 0CCh, 008h, 00Ch, 013h, 020h, 006h, 0CEh, 008h, 00Bh, 0C8h, 06Ch, 001h
        db  020h, 0C8h, 000h, 0FCh, 0CCh, 008h, 04Bh, 0CBh, 002h, 000h, 090h, 003h, 0FFh, 0FFh, 080h, 003h
        db  041h, 0C0h, 00Fh, 013h, 081h, 080h, 00Dh, 013h, 082h, 0A0h, 0E2h, 0C2h, 032h, 00Ch, 012h, 009h
        db  00Bh, 0C8h, 06Ch, 001h, 0E0h, 004h, 000h, 0FCh, 020h, 0C3h, 002h, 0FCh, 007h, 011h, 002h, 0C8h
        db  000h, 0FCh, 0EDh, 004h, 002h, 000h, 0E0h, 004h, 06Ch, 001h, 080h, 003h, 042h, 0CBh, 002h, 000h
        db  002h, 0C8h, 06Ch, 001h, 08Bh, 0C0h, 080h, 003h, 000h, 003h, 002h, 000h, 083h, 007h, 000h, 080h
        db  060h, 0C2h, 02Ch, 009h, 009h, 0C1h, 024h, 002h, 0F8h, 0FFh, 0A9h, 008h, 001h, 002h, 000h, 000h
        db  002h, 002h, 000h, 000h, 00Bh, 002h, 000h, 000h, 00Ch, 002h, 000h, 000h, 007h, 002h, 000h, 000h
        db  02Ch, 0CBh, 032h, 00Ch, 032h, 00Ch, 006h, 013h, 08Bh, 005h, 0CCh, 005h, 00Bh, 088h, 046h, 004h
        db  027h, 01Bh, 0F6h, 010h, 009h, 0C2h, 08Bh, 0C2h, 008h, 006h, 00Ah, 013h, 08Bh, 005h, 0CCh, 005h
        db  00Bh, 088h, 046h, 004h, 01Dh, 01Bh, 02Ch, 0CBh, 032h, 00Ch, 032h, 00Ch, 0EDh, 016h, 0F4h, 010h
        db  082h, 0C0h, 014h, 013h, 002h, 0C8h, 06Ch, 001h, 00Ah, 0C8h, 000h, 0FCh, 00Ah, 0C8h, 06Ch, 001h
        db  0E0h, 004h, 000h, 0FCh, 0A0h, 007h, 002h, 0FCh, 000h, 080h, 004h, 0C8h, 004h, 0FCh, 00Ah, 0C2h
        db  008h, 0A2h, 002h, 0CAh, 032h, 00Ch, 08Ah, 0C0h, 087h, 005h, 0D6h, 010h, 04Ah, 0C0h, 0EEh, 010h
        db  047h, 0CBh, 002h, 000h, 0E0h, 004h, 06Ch, 001h, 08Bh, 007h, 043h, 000h, 0E0h, 004h, 000h, 00Ch
        db  000h, 003h, 002h, 000h, 00Bh, 0C8h, 06Ch, 001h, 08Bh, 002h, 043h, 000h, 004h, 013h, 060h, 001h
        db  002h, 0FCh, 020h, 000h, 006h, 013h, 08Bh, 0C2h, 0A0h, 006h, 016h, 0C1h, 090h, 003h, 07Fh, 000h
        db  080h, 003h, 0A0h, 001h, 002h, 0FCh, 020h, 000h, 060h, 001h, 018h, 009h, 001h, 000h, 00Bh, 016h
        db  00Ah, 002h, 002h, 0FCh, 0A0h, 0A2h, 0DEh, 008h, 0A0h, 0CEh, 0EEh, 005h, 0A0h, 0C6h, 004h, 0FCh
        db  020h, 0C8h, 0DEh, 008h, 004h, 0FCh, 08Ah, 007h, 0F8h, 005h, 05Ah, 0C2h, 008h, 013h, 0CAh, 005h
        db  05Ah, 0C2h, 009h, 0C8h, 06Ch, 001h, 00Bh, 0C8h, 000h, 0FCh, 08Bh, 0C6h, 002h, 010h, 08Bh, 0CEh
        db  08Bh, 0C6h, 020h, 020h, 01Ah, 0E0h, 005h, 016h, 020h, 0E8h, 004h, 0E0h, 03Ah, 007h, 0E0h, 004h
        db  036h, 007h, 090h, 003h, 07Fh, 000h, 080h, 003h, 000h, 003h, 002h, 000h, 00Bh, 0C8h, 06Ch, 001h
        db  0CCh, 004h, 0E0h, 004h, 000h, 0FCh, 08Bh, 0C2h, 0A0h, 006h, 024h, 0C1h, 090h, 003h, 07Fh, 000h
        db  080h, 003h, 0A0h, 007h, 002h, 0FCh, 000h, 080h, 020h, 0C8h, 08Eh, 0E1h, 004h, 0FCh, 041h, 0C0h
        db  00Fh, 016h, 020h, 0D8h, 002h, 0E2h, 083h, 001h, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh
        db  08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 00Ah, 0C8h, 08Ah, 001h
        db  05Bh, 004h, 00Ah, 0C8h, 06Ch, 001h, 020h, 0C3h, 000h, 0FCh, 0E0h, 004h, 000h, 0FCh, 08Ah, 002h
        db  043h, 000h, 0DFh, 013h, 0A0h, 007h, 002h, 0FCh, 000h, 080h, 004h, 0C8h, 004h, 0FCh, 020h, 098h
        db  032h, 009h, 0CFh, 008h, 00Ah, 013h, 020h, 0C8h, 0CCh, 008h, 000h, 0FCh, 00Ah, 0C8h, 0CCh, 008h
        db  0A0h, 005h, 0CEh, 008h, 08Ch, 0C2h, 0E5h, 016h, 05Bh, 004h, 041h, 0C0h, 010h, 013h, 08Ah, 0A2h
        db  082h, 0CAh, 032h, 00Ch, 01Ah, 009h, 002h, 0C8h, 06Ch, 001h, 00Ah, 0C8h, 000h, 0FCh, 08Ah, 0C0h
        db  020h, 098h, 083h, 001h, 002h, 0E2h, 009h, 013h, 08Ch, 0C2h, 0D3h, 016h, 05Bh, 004h, 04Ah, 0C0h
        db  08Ah, 0C0h, 020h, 098h, 083h, 001h, 002h, 0E2h, 011h, 016h, 0E0h, 001h, 09Ch, 001h, 040h, 000h
        db  060h, 001h, 09Ch, 001h, 040h, 000h, 0FCh, 016h, 060h, 001h, 002h, 00Ch, 000h, 001h, 0ECh, 013h
        db  020h, 0D8h, 0E1h, 008h, 083h, 001h, 0A0h, 007h, 002h, 00Ch, 000h, 080h, 00Ah, 0C8h, 08Ah, 001h
        db  00Ah, 0C8h, 0CAh, 008h, 0E1h, 010h, 0D8h, 004h, 057h, 0C2h, 003h, 016h, 0C8h, 0CDh, 0C8h, 0C5h
        db  05Bh, 004h, 0C7h, 005h, 057h, 0C2h, 048h, 0C6h, 0C8h, 0C5h, 05Bh, 004h, 008h, 0C8h, 06Ch, 001h
        db  008h, 0A2h, 020h, 0CAh, 000h, 0FCh, 032h, 00Ch, 018h, 009h, 002h, 010h, 008h, 0C8h, 06Ch, 001h
        db  0E0h, 004h, 000h, 0FCh, 057h, 0C2h, 003h, 016h, 0C8h, 0CDh, 0C8h, 0C5h, 05Bh, 004h, 0C7h, 005h
        db  017h, 0C8h, 06Ch, 001h, 008h, 0C8h, 000h, 0FCh, 0C8h, 0C5h, 05Bh, 004h, 017h, 0C6h, 002h, 016h
        db  0C8h, 0C9h, 002h, 000h, 0C8h, 0C5h, 05Bh, 004h, 017h, 0C2h, 008h, 0C8h, 06Ch, 001h, 007h, 013h
        db  0E0h, 0C5h, 000h, 0FCh, 008h, 0A2h, 028h, 0C8h, 032h, 00Ch, 000h, 0FCh, 018h, 009h, 05Bh, 004h
        db  060h, 001h, 082h, 001h, 000h, 020h, 00Ah, 016h, 060h, 0C2h, 084h, 001h, 0A0h, 001h, 082h, 001h
        db  000h, 020h, 0E0h, 001h, 082h, 001h, 000h, 020h, 009h, 0C8h, 084h, 001h, 0C9h, 004h, 05Bh, 004h
        db  0A0h, 006h, 048h, 0C4h, 0D3h, 004h, 0E0h, 004h, 002h, 001h, 020h, 0E8h, 014h, 0E0h, 000h, 001h
        db  020h, 0C8h, 016h, 0E0h, 004h, 001h, 005h, 02Ch, 020h, 048h, 014h, 0E0h, 000h, 001h, 08Ch, 007h
        db  000h, 00Ah, 08Dh, 007h, 0D8h, 007h, 08Eh, 007h, 018h, 000h, 07Ch, 0CFh, 04Eh, 006h, 0FDh, 016h
        db  0E0h, 002h, 0D8h, 007h, 08Fh, 007h, 011h, 0FFh, 08Bh, 002h, 03Bh, 059h, 021h, 016h, 08Ah, 002h
        db  03Bh, 059h, 01Eh, 013h, 08Fh, 005h, 020h, 020h, 016h, 0E0h, 001h, 016h, 019h, 010h, 020h, 020h
        db  004h, 0E0h, 016h, 016h, 000h, 001h, 0BFh, 000h, 013h, 016h, 08Bh, 007h, 0C0h, 040h, 000h, 001h
        db  000h, 060h, 010h, 013h, 040h, 001h, 000h, 060h, 00Bh, 016h, 08Bh, 007h, 0C4h, 044h, 0A0h, 0C3h
        db  002h, 001h, 00Eh, 048h, 002h, 001h, 04Eh, 001h, 000h, 010h, 004h, 016h, 08Fh, 007h, 018h, 0FFh
        db  060h, 004h, 01Eh, 0C4h, 00Bh, 0C3h, 04Bh, 0C3h, 020h, 020h, 00Ah, 0E0h, 002h, 016h, 06Bh, 002h
        db  020h, 020h, 020h, 020h, 00Ch, 0E0h, 002h, 016h, 06Ch, 002h, 000h, 020h, 020h, 020h, 00Eh, 0E0h
        db  002h, 016h, 06Ch, 002h, 020h, 000h, 08Fh, 005h, 020h, 020h, 010h, 0E0h, 007h, 016h, 06Dh, 002h
        db  020h, 000h, 020h, 021h, 022h, 0E0h, 0E4h, 013h, 004h, 0C1h, 002h, 016h, 084h, 007h, 0FEh, 07Fh
        db  08Fh, 005h, 020h, 020h, 012h, 0E0h, 002h, 016h, 06Dh, 002h, 000h, 020h, 060h, 021h, 022h, 0E0h
        db  0D7h, 013h, 045h, 0C1h, 002h, 016h, 085h, 007h, 0FEh, 07Fh, 08Fh, 005h, 086h, 0D1h, 00Bh, 013h
        db  0A0h, 025h, 026h, 0E0h, 008h, 013h, 08Fh, 005h, 020h, 026h, 022h, 0E0h, 004h, 016h, 08Fh, 005h
        db  0A0h, 026h, 022h, 0E0h, 002h, 013h, 060h, 004h, 01Eh, 0C4h, 001h, 0D8h, 09Eh, 008h, 020h, 0D8h
        db  0DBh, 007h, 0B2h, 008h, 002h, 0D8h, 0A8h, 008h, 020h, 0D8h, 0DDh, 007h, 094h, 008h, 0E0h, 002h
        db  058h, 007h, 0A0h, 006h, 048h, 0C4h, 089h, 007h, 05Eh, 0E3h, 0E0h, 004h, 01Ah, 001h, 020h, 0C8h
        db  0E4h, 007h, 018h, 001h, 019h, 0C8h, 00Ch, 001h, 039h, 0C8h, 00Ah, 001h, 039h, 0C8h, 012h, 001h
        db  009h, 016h, 079h, 0C3h, 00Fh, 002h, 000h, 0E0h, 04Fh, 063h, 02Dh, 002h, 000h, 0A0h, 00Dh, 0C8h
        db  014h, 001h, 002h, 010h, 039h, 0C8h, 014h, 001h, 0F9h, 0C3h, 03Fh, 0C8h, 00Eh, 001h, 01Fh, 0C8h
        db  010h, 001h, 0E0h, 004h, 0C6h, 008h, 0B9h, 0C2h, 01Ah, 0D8h, 000h, 001h, 096h, 006h, 089h, 002h
        db  086h, 0E3h, 0E0h, 016h, 08Fh, 007h, 01Ch, 0FFh, 08Ch, 007h, 000h, 00Ah, 08Dh, 007h, 086h, 0E3h
        db  08Eh, 007h, 010h, 000h, 07Ch, 08Fh, 03Bh, 016h, 04Eh, 006h, 0FCh, 016h, 08Ch, 007h, 096h, 0E3h
        db  08Dh, 007h, 000h, 00Ah, 08Eh, 007h, 0A6h, 0E3h, 08Ch, 063h, 07Ch, 0CFh, 04Eh, 006h, 0FDh, 016h
        db  0E0h, 004h, 0E2h, 008h, 020h, 001h, 042h, 001h, 000h, 004h, 002h, 016h, 020h, 007h, 0E2h, 008h
        db  060h, 0C2h, 062h, 001h, 0E0h, 004h, 062h, 001h, 08Eh, 007h, 000h, 080h, 08Ch, 007h, 0E6h, 008h
        db  08Dh, 007h, 006h, 000h, 03Eh, 0DFh, 08Eh, 005h, 00Dh, 006h, 0FCh, 016h, 0FEh, 0D3h, 0CFh, 006h
        db  08Eh, 005h, 0FEh, 0D3h, 0CFh, 006h, 08Ch, 007h, 0E6h, 008h, 009h, 0C8h, 062h, 001h, 0C9h, 004h
        db  05Ch, 0A3h, 07Ch, 0E2h, 05Ch, 0A3h, 07Ch, 0E2h, 05Ch, 0A3h, 07Ch, 0E2h, 002h, 013h, 0CDh, 083h
        db  009h, 013h, 020h, 007h, 0E6h, 008h, 006h, 010h, 08Fh, 007h, 019h, 0FFh, 0CDh, 0A3h, 00Fh, 0C8h
        db  004h, 001h, 0FFh, 010h, 0A0h, 001h, 002h, 001h, 000h, 010h, 0E0h, 0C3h, 0EEh, 007h, 0E0h, 043h
        db  006h, 0E0h, 00Fh, 0D8h, 000h, 001h, 020h, 0C0h, 004h, 0E0h, 0E0h, 004h, 0FEh, 006h, 0D3h, 004h
        db  0E0h, 004h, 004h, 001h, 060h, 004h, 05Ch, 0C4h, 08Ch, 007h, 000h, 00Ah, 08Dh, 007h, 018h, 000h
        db  08Eh, 007h, 03Bh, 059h, 00Eh, 0CFh, 04Dh, 006h, 0FDh, 016h, 05Bh, 004h, 0D3h, 0C3h, 0C5h, 043h
        db  00Ch, 016h, 0E0h, 0C3h, 098h, 007h, 003h, 011h, 0E0h, 002h, 098h, 007h, 051h, 004h, 0E0h, 0C3h
        db  078h, 007h, 003h, 011h, 0E0h, 002h, 078h, 007h, 051h, 004h, 005h, 02Ch, 041h, 0A0h, 021h, 004h
        db  0C2h, 0E2h, 08Bh, 007h, 05Ch, 0C4h, 000h, 001h, 000h, 040h, 005h, 013h, 0C0h, 001h, 000h, 020h
        db  0DDh, 001h, 080h, 000h, 05Bh, 004h, 04Bh, 0C2h, 0A0h, 0D2h, 0EFh, 007h, 08Ch, 007h, 008h, 000h
        db  09Dh, 001h, 080h, 000h, 0BDh, 0C0h, 0A0h, 0C3h, 0EAh, 007h, 0E0h, 0C3h, 0ECh, 007h, 0A0h, 006h
        db  054h, 0C6h, 0C0h, 001h, 000h, 040h, 002h, 0D8h, 017h, 001h, 062h, 002h, 080h, 0FFh, 0A0h, 006h
        db  0B0h, 0C6h, 002h, 0C8h, 004h, 001h, 059h, 004h, 080h, 001h, 000h, 040h, 000h, 001h, 0FEh, 000h
        db  014h, 013h, 0C0h, 0C3h, 0CFh, 073h, 0EFh, 0D3h, 0C2h, 0E1h, 0CFh, 006h, 0CFh, 073h, 0AFh, 0C3h
        db  080h, 0EAh, 09Eh, 0C3h, 04Eh, 001h, 080h, 000h, 008h, 013h, 02Fh, 040h, 014h, 0E0h, 0CFh, 0A3h
        db  02Fh, 004h, 0F2h, 0E2h, 040h, 001h, 000h, 040h, 0B1h, 013h, 040h, 001h, 000h, 020h, 004h, 013h
        db  000h, 001h, 0FEh, 000h, 0E6h, 016h, 0AAh, 010h, 08Dh, 007h, 094h, 008h, 05Dh, 001h, 080h, 000h
        db  008h, 013h, 02Dh, 002h, 00Ah, 000h, 08Dh, 002h, 0C6h, 008h, 0F8h, 016h, 080h, 001h, 000h, 020h
        db  0EFh, 010h, 058h, 004h, 040h, 001h, 000h, 040h, 005h, 016h, 020h, 0E0h, 014h, 0E0h, 065h, 002h
        db  000h, 058h, 094h, 010h, 020h, 0D8h, 0DEh, 007h, 017h, 001h, 08Fh, 007h, 086h, 0FFh, 00Fh, 0C8h
        db  004h, 001h, 0C0h, 001h, 000h, 040h, 045h, 002h, 0FFh, 0A7h, 088h, 010h, 020h, 0C3h, 0FEh, 006h
        db  020h, 027h, 03Ah, 0E3h, 007h, 013h, 020h, 023h, 022h, 0E0h, 01Ah, 013h, 065h, 002h, 0FFh, 0DFh
        db  020h, 040h, 014h, 0E0h, 020h, 0E0h, 016h, 0E0h, 00Ch, 0C8h, 098h, 008h, 08Dh, 007h, 094h, 008h
        db  058h, 004h, 020h, 048h, 008h, 0E0h, 0FEh, 006h, 020h, 0C3h, 098h, 008h, 020h, 027h, 03Ah, 0E3h
        db  019h, 016h, 080h, 003h, 002h, 0C3h, 06Ch, 0C2h, 00Ah, 000h, 099h, 006h, 060h, 004h, 05Ch, 0C4h
        db  0A0h, 0D2h, 0EFh, 007h, 08Ch, 007h, 001h, 000h, 08Dh, 007h, 006h, 006h, 0CEh, 004h, 0E0h, 0C3h
        db  008h, 006h, 001h, 013h, 097h, 006h, 020h, 0D8h, 007h, 006h, 017h, 001h, 08Bh, 007h, 082h, 0FFh
        db  00Bh, 0C8h, 004h, 001h, 0A0h, 006h, 098h, 0CAh, 060h, 004h, 05Ch, 0C4h, 0A0h, 0C2h, 0EEh, 007h
        db  08Ch, 007h, 006h, 000h, 08Dh, 007h, 0A0h, 008h, 0A0h, 0C3h, 0E6h, 007h, 0E0h, 0C3h, 0E8h, 007h
        db  097h, 006h, 0A0h, 0D2h, 0EFh, 007h, 08Dh, 007h, 0A6h, 008h, 0DDh, 004h, 08Ch, 007h, 002h, 000h
        db  097h, 006h, 08Dh, 007h, 000h, 080h, 0A0h, 0C2h, 0A0h, 008h, 00Ah, 088h, 00Ch, 006h, 014h, 01Bh
        db  082h, 007h, 024h, 0C6h, 0A0h, 0C3h, 0A2h, 008h, 0E0h, 0C3h, 0A4h, 008h, 08Bh, 007h, 00Eh, 0E3h
        db  08Ah, 002h, 014h, 000h, 004h, 01Ah, 08Bh, 007h, 07Ch, 0EAh, 02Ah, 002h, 0ECh, 0FFh, 08Ah, 0A2h
        db  0CAh, 0A2h, 0DBh, 0C2h, 001h, 013h, 09Bh, 006h, 020h, 0C8h, 0A0h, 008h, 0A4h, 008h, 020h, 0C8h
        db  020h, 0E0h, 0A0h, 008h, 00Dh, 0C8h, 0A2h, 008h, 08Dh, 007h, 09Eh, 008h, 020h, 0E0h, 018h, 0E0h
        db  065h, 002h, 000h, 058h, 058h, 004h, 045h, 002h, 0FFh, 0A7h, 080h, 003h, 060h, 0C0h, 0EEh, 005h
        db  021h, 002h, 0E8h, 003h, 020h, 001h, 002h, 001h, 006h, 000h, 007h, 016h, 001h, 088h, 0EEh, 005h
        db  0F9h, 016h, 03Dh, 010h, 060h, 0D0h, 003h, 001h, 0F1h, 013h, 0E0h, 004h, 01Ah, 001h, 001h, 002h
        db  00Ah, 001h, 04Ch, 0CCh, 04Ch, 0CCh, 04Eh, 0CCh, 04Fh, 0CCh, 0B1h, 007h, 040h, 000h, 04Dh, 0CCh
        db  00Ah, 0D8h, 000h, 001h, 05Bh, 004h, 060h, 0C0h, 0EEh, 005h, 021h, 002h, 0E8h, 003h, 020h, 001h
        db  002h, 001h, 006h, 000h, 007h, 016h, 001h, 088h, 0EEh, 005h, 0F9h, 016h, 020h, 010h, 060h, 0D0h
        db  003h, 001h, 0F1h, 013h, 0E0h, 004h, 01Ah, 001h, 001h, 002h, 00Ah, 001h, 04Ch, 0CCh, 04Ch, 0CCh
        db  04Eh, 0CCh, 04Fh, 0CCh, 0B1h, 007h, 040h, 000h, 04Dh, 0CCh, 00Ah, 0D8h, 000h, 001h, 0B0h, 003h
        db  060h, 0D0h, 003h, 001h, 001h, 013h, 05Bh, 004h, 060h, 0C0h, 0EEh, 005h, 021h, 002h, 0E8h, 003h
        db  020h, 001h, 002h, 001h, 006h, 000h, 0F7h, 016h, 001h, 088h, 0EEh, 005h, 0F9h, 016h, 0CDh, 004h
        db  08Ah, 007h, 000h, 040h, 020h, 0C3h, 000h, 001h, 00Ch, 001h, 000h, 080h, 002h, 013h, 08Ah, 007h
        db  000h, 020h, 0A0h, 0C3h, 00Eh, 001h, 0E0h, 0C3h, 010h, 001h, 0B0h, 003h, 020h, 0C3h, 058h, 007h
        db  020h, 023h, 004h, 0E0h, 002h, 013h, 060h, 004h, 018h, 0C4h, 060h, 004h, 07Ch, 0B3h, 08Dh, 007h
        db  000h, 020h, 020h, 020h, 00Ah, 0E0h, 001h, 016h, 05Bh, 004h, 00Dh, 002h, 032h, 00Ch, 05Dh, 0C2h
        db  001h, 011h, 0DDh, 004h, 0CDh, 005h, 00Dh, 088h, 030h, 00Ch, 0F9h, 016h, 060h, 0C2h, 00Ah, 006h
        db  08Dh, 007h, 018h, 009h, 0A0h, 006h, 0E0h, 0CAh, 009h, 002h, 068h, 000h, 0E0h, 0C3h, 0E2h, 008h
        db  003h, 016h, 0E0h, 001h, 018h, 009h, 010h, 000h, 0E0h, 0C2h, 018h, 009h, 00Fh, 002h, 000h, 001h
        db  0C9h, 026h, 002h, 013h, 060h, 004h, 06Ah, 0C9h, 009h, 002h, 000h, 012h, 04Bh, 001h, 010h, 000h
        db  002h, 013h, 009h, 002h, 000h, 013h, 009h, 0D8h, 0E0h, 008h, 08Fh, 007h, 000h, 040h, 089h, 007h
        db  01Ah, 009h, 0CBh, 004h, 0F9h, 0E2h, 0F9h, 0E2h, 0F9h, 0E2h, 007h, 016h, 08Bh, 007h, 0E6h, 008h
        db  08Ch, 007h, 01Ah, 009h, 03Bh, 0CFh, 03Bh, 0CFh, 01Bh, 0C7h, 020h, 0C3h, 01Ah, 009h, 018h, 011h
        db  08Fh, 007h, 000h, 020h, 089h, 007h, 028h, 009h, 0A0h, 006h, 092h, 0C7h, 0A0h, 006h, 092h, 0C7h
        db  011h, 010h, 019h, 0C3h, 002h, 016h, 08Ch, 007h, 01Ah, 000h, 08Ch, 002h, 00Eh, 000h, 006h, 013h
        db  08Ch, 002h, 014h, 000h, 003h, 013h, 08Ch, 002h, 01Ah, 000h, 002h, 016h, 04Ch, 0CEh, 05Bh, 004h
        db  060h, 004h, 06Ah, 0C9h, 08Fh, 007h, 000h, 010h, 0D9h, 0C2h, 0FAh, 011h, 002h, 016h, 08Bh, 007h
        db  000h, 004h, 04Bh, 0C3h, 08Dh, 002h, 020h, 000h, 002h, 014h, 00Dh, 002h, 020h, 000h, 08Dh, 002h
        db  000h, 004h, 002h, 012h, 00Dh, 002h, 000h, 004h, 02Dh, 002h, 0F8h, 0FFh, 00Dh, 0C8h, 0DEh, 008h
        db  02Bh, 002h, 0FFh, 003h, 08Bh, 001h, 0FFh, 003h, 04Bh, 0CEh, 0CBh, 004h, 0E0h, 004h, 0F8h, 005h
        db  0E0h, 004h, 014h, 009h, 020h, 0C8h, 030h, 00Ch, 02Eh, 009h, 0A0h, 007h, 030h, 009h, 0FEh, 0DFh
        db  08Dh, 007h, 0FEh, 0DFh, 0E0h, 0C3h, 0D8h, 007h, 0E0h, 023h, 016h, 0E0h, 024h, 016h, 0E0h, 0C3h
        db  030h, 00Ch, 04Fh, 063h, 0FFh, 004h, 0FFh, 004h, 04Dh, 006h, 0FDh, 016h, 08Dh, 007h, 0FEh, 0DFh
        db  020h, 004h, 064h, 0EAh, 0A0h, 0C3h, 064h, 0EAh, 0EEh, 0C3h, 012h, 000h, 0C1h, 016h, 06Eh, 0C3h
        db  018h, 000h, 0ADh, 009h, 08Ch, 007h, 000h, 0E0h, 0ACh, 009h, 00Dh, 063h, 00Ch, 013h, 06Eh, 0C3h
        db  018h, 000h, 0ADh, 009h, 02Dh, 002h, 040h, 000h, 01Dh, 00Ah, 02Dh, 002h, 032h, 00Ch, 0BDh, 007h
        db  0FFh, 07Fh, 00Ch, 006h, 0FCh, 016h, 020h, 0C3h, 046h, 004h, 08Ch, 002h, 080h, 000h, 013h, 01Ah
        db  0ACh, 002h, 00Ch, 0C8h, 09Ah, 000h, 0E0h, 002h, 080h, 000h, 088h, 007h, 080h, 000h, 060h, 0C2h
        db  046h, 004h, 0A0h, 006h, 026h, 0F1h, 002h, 010h, 09Dh, 000h, 005h, 010h, 09Dh, 000h, 08Fh, 007h
        db  000h, 008h, 060h, 004h, 06Ah, 0C9h, 04Bh, 02Dh, 081h, 0C3h, 0C9h, 005h, 08Fh, 007h, 000h, 010h
        db  08Eh, 002h, 002h, 000h, 06Ah, 011h, 08Fh, 007h, 000h, 004h, 0C9h, 005h, 0D9h, 0C2h, 0E0h, 026h
        db  026h, 0E0h, 002h, 016h, 02Bh, 002h, 006h, 000h, 04Bh, 0CEh, 04Bh, 0C3h, 0CBh, 072h, 02Eh, 002h
        db  0FEh, 0FFh, 08Bh, 083h, 05Ah, 01Bh, 0CDh, 006h, 04Dh, 073h, 0CDh, 082h, 056h, 01Bh, 0E0h, 004h
        db  0CCh, 008h, 0E0h, 004h, 0CEh, 008h, 0B9h, 0C3h, 0D9h, 0C3h, 089h, 007h, 012h, 000h, 08Dh, 007h
        db  0ECh, 008h, 0A0h, 006h, 0E0h, 0CAh, 060h, 0C3h, 0D8h, 007h, 060h, 023h, 016h, 0E0h, 009h, 016h
        db  020h, 0E8h, 010h, 0E0h, 018h, 009h, 020h, 0E8h, 018h, 0E0h, 098h, 007h, 020h, 0E8h, 012h, 0E0h
        db  078h, 007h, 060h, 0C3h, 018h, 009h, 060h, 023h, 01Eh, 0E0h, 003h, 016h, 020h, 048h, 0A6h, 0E3h
        db  018h, 009h, 060h, 023h, 022h, 0E0h, 006h, 013h, 060h, 027h, 0A8h, 0E3h, 003h, 013h, 020h, 0E8h
        db  010h, 0E0h, 018h, 009h, 020h, 02Dh, 000h, 000h, 08Eh, 007h, 000h, 000h, 0A0h, 006h, 0C0h, 0CAh
        db  04Eh, 005h, 00Eh, 02Ch, 0A0h, 0C0h, 0FEh, 007h, 0EFh, 0C3h, 006h, 000h, 01Bh, 016h, 0A0h, 0C3h
        db  020h, 009h, 0E0h, 0C3h, 022h, 009h, 0A0h, 006h, 0A6h, 0C9h, 0A0h, 0C3h, 024h, 009h, 0E0h, 0C3h
        db  026h, 009h, 0A0h, 006h, 0C4h, 0C9h, 020h, 0E0h, 00Ah, 0E0h, 060h, 0C3h, 0D8h, 007h, 060h, 023h
        db  016h, 0E0h, 005h, 016h, 0E0h, 004h, 02Eh, 006h, 060h, 041h, 004h, 0E0h, 04Dh, 02Eh, 08Dh, 007h
        db  000h, 080h, 052h, 004h, 0CFh, 073h, 02Fh, 002h, 000h, 002h, 04Fh, 0C3h, 052h, 004h, 020h, 020h
        db  00Ah, 0E0h, 003h, 013h, 08Dh, 007h, 000h, 010h, 05Bh, 004h, 020h, 040h, 00Ah, 0E0h, 040h, 002h
        db  0FFh, 0F0h, 08Eh, 007h, 002h, 000h, 0A0h, 006h, 0C0h, 0CAh, 04Eh, 005h, 00Eh, 02Ch, 0A0h, 0C0h
        db  0FEh, 007h, 0A0h, 006h, 098h, 0CAh, 060h, 0C3h, 0D8h, 007h, 060h, 023h, 016h, 0E0h, 066h, 016h
        db  020h, 004h, 078h, 0EAh, 063h, 010h, 06Eh, 002h, 000h, 080h, 08Dh, 007h, 000h, 0C0h, 00Dh, 0C8h
        db  0A6h, 001h, 00Eh, 0C8h, 020h, 009h, 00Fh, 0C8h, 022h, 009h, 00Eh, 0C8h, 0A8h, 001h, 00Fh, 0C8h
        db  0AAh, 001h, 012h, 010h, 08Fh, 001h, 001h, 000h, 08Ah, 007h, 024h, 009h, 0A0h, 0E3h, 000h, 009h
        db  08Eh, 0CEh, 09Ah, 001h, 0FEh, 0FFh, 0E0h, 0E3h, 002h, 009h, 08Fh, 0E6h, 08Ah, 007h, 0ACh, 001h
        db  08Eh, 0CEh, 09Ah, 001h, 0FEh, 0FFh, 08Fh, 0E6h, 020h, 020h, 00Ah, 0E0h, 03Fh, 013h, 08Dh, 007h
        db  000h, 010h, 05Bh, 004h, 020h, 020h, 00Ah, 0E0h, 003h, 013h, 00Dh, 002h, 000h, 010h, 05Bh, 004h
        db  08Eh, 0C3h, 004h, 013h, 0E0h, 001h, 002h, 009h, 000h, 001h, 006h, 010h, 0A0h, 001h, 002h, 009h
        db  000h, 001h, 0A0h, 001h, 026h, 009h, 000h, 001h, 0A0h, 0C3h, 024h, 009h, 0E0h, 0C3h, 026h, 009h
        db  0A0h, 0E3h, 000h, 009h, 0E0h, 0E3h, 002h, 009h, 00Eh, 0C8h, 0ACh, 001h, 00Fh, 0C8h, 0AEh, 001h
        db  00Eh, 0C8h, 024h, 009h, 00Fh, 0C8h, 026h, 009h, 019h, 010h, 06Eh, 002h, 000h, 080h, 00Eh, 0C8h
        db  0A6h, 001h, 020h, 020h, 00Ah, 0E0h, 012h, 013h, 00Dh, 002h, 000h, 010h, 05Bh, 004h, 08Dh, 007h
        db  028h, 007h, 089h, 007h, 00Eh, 000h, 0A0h, 006h, 0ECh, 0CAh, 08Dh, 007h, 028h, 007h, 0FDh, 004h
        db  08Dh, 002h, 036h, 007h, 0FCh, 016h, 020h, 048h, 014h, 0E0h, 0FEh, 006h, 08Dh, 007h, 000h, 080h
        db  052h, 004h, 0A0h, 0C2h, 0EEh, 007h, 08Ch, 007h, 004h, 000h, 08Dh, 007h, 0A2h, 008h, 097h, 006h
        db  07Dh, 0C2h, 05Dh, 0C3h, 060h, 043h, 022h, 0E0h, 0A0h, 006h, 0ECh, 0CAh, 0EFh, 010h, 00Eh, 0C8h
        db  006h, 006h, 00Fh, 0C8h, 008h, 006h, 0EAh, 010h, 08Ch, 007h, 014h, 0E0h, 01Ch, 048h, 094h, 008h
        db  01Ch, 048h, 0A8h, 008h, 01Ch, 048h, 0B2h, 008h, 040h, 002h, 000h, 0C0h, 0E0h, 004h, 006h, 006h
        db  08Ch, 007h, 010h, 040h, 0CCh, 044h, 0E0h, 004h, 0FEh, 006h, 085h, 007h, 040h, 080h, 05Bh, 004h
        db  002h, 0C8h, 0FEh, 007h, 08Fh, 007h, 0F4h, 007h, 0CEh, 0CBh, 002h, 000h, 08Eh, 007h, 036h, 007h
        db  0CEh, 0CBh, 004h, 000h, 08Dh, 007h, 030h, 006h, 08Eh, 007h, 010h, 000h, 04Dh, 02Ch, 05Bh, 004h
        db  020h, 0C8h, 0E2h, 007h, 01Ah, 001h, 0A0h, 0C2h, 0F2h, 007h, 005h, 010h, 020h, 0C8h, 0E0h, 007h
        db  01Ah, 001h, 0A0h, 0D2h, 0F3h, 007h, 00Bh, 0C8h, 09Ch, 008h, 009h, 0C3h, 00Ah, 013h, 0A0h, 006h
        db  08Eh, 0C6h, 0A0h, 0C2h, 000h, 001h, 0A0h, 0E2h, 006h, 0E0h, 04Ch, 0A3h, 0CCh, 0A3h, 001h, 017h
        db  08Eh, 005h, 04Ch, 062h, 0E0h, 0C2h, 09Ch, 008h, 05Bh, 004h, 08Dh, 007h, 000h, 010h, 020h, 020h
        db  00Ah, 0E0h, 001h, 013h, 05Bh, 004h, 00Dh, 002h, 068h, 000h, 0E0h, 0C3h, 0E2h, 008h, 002h, 016h
        db  0CEh, 001h, 010h, 000h, 08Dh, 027h, 003h, 013h, 00Dh, 002h, 000h, 001h, 052h, 004h, 000h, 003h
        db  002h, 000h, 060h, 0C3h, 018h, 009h, 04Dh, 002h, 008h, 080h, 04Eh, 002h, 0F7h, 07Fh, 08Dh, 0E3h
        db  0E0h, 0C3h, 0D8h, 007h, 0E0h, 023h, 016h, 0E0h, 004h, 013h, 08Dh, 007h, 006h, 000h, 08Dh, 027h
        db  002h, 013h, 0A0h, 0E3h, 010h, 0E0h, 00Eh, 0C8h, 018h, 009h, 00Dh, 002h, 000h, 012h, 04Eh, 001h
        db  010h, 000h, 002h, 013h, 00Dh, 002h, 000h, 013h, 00Dh, 0D8h, 0E0h, 008h, 060h, 0C3h, 080h, 001h
        db  04Eh, 002h, 001h, 000h, 04Dh, 002h, 0FEh, 0FFh, 04Eh, 0E3h, 00Dh, 0C8h, 080h, 001h, 020h, 0D8h
        db  042h, 0E2h, 0E1h, 008h, 020h, 001h, 018h, 009h, 006h, 000h, 003h, 013h, 020h, 0D8h, 0D2h, 0E1h
        db  0E1h, 008h, 020h, 098h, 083h, 001h, 002h, 0E2h, 003h, 013h, 020h, 0D8h, 0E1h, 008h, 083h, 001h
        db  000h, 003h, 00Fh, 000h, 060h, 004h, 06Ch, 0CAh, 020h, 020h, 00Ah, 0E0h, 003h, 013h, 00Dh, 002h
        db  000h, 010h, 05Bh, 004h, 009h, 002h, 008h, 000h, 00Dh, 002h, 00Ah, 009h, 0A0h, 006h, 0E0h, 0CAh
        db  0A0h, 007h, 002h, 002h, 000h, 000h, 00Dh, 002h, 000h, 004h, 0E0h, 0C3h, 00Ah, 009h, 00Fh, 001h
        db  000h, 07Ch, 001h, 013h, 052h, 004h, 08Fh, 0C3h, 04Eh, 002h, 00Fh, 000h, 0FBh, 013h, 08Eh, 002h
        db  00Fh, 000h, 0F8h, 013h, 00Dh, 002h, 000h, 040h, 04Fh, 0C2h, 049h, 009h, 049h, 002h, 03Fh, 000h
        db  009h, 001h, 001h, 000h, 0EFh, 016h, 089h, 002h, 006h, 000h, 0ECh, 01Ah, 089h, 002h, 020h, 000h
        db  0E9h, 014h, 0C9h, 006h, 01Fh, 009h, 04Fh, 002h, 000h, 040h, 04Fh, 0E2h, 069h, 002h, 000h, 080h
        db  009h, 0C8h, 00Ah, 009h, 00Fh, 002h, 0FFh, 0FFh, 04Eh, 0C2h, 01Fh, 009h, 009h, 006h, 0FDh, 016h
        db  04Fh, 005h, 00Dh, 002h, 000h, 020h, 060h, 0C2h, 00Ch, 009h, 0D4h, 013h, 04Fh, 026h, 0D2h, 016h
        db  00Dh, 002h, 000h, 010h, 060h, 0C2h, 00Eh, 009h, 0CDh, 013h, 04Fh, 026h, 0CBh, 016h, 00Dh, 002h
        db  000h, 030h, 020h, 088h, 00Ch, 009h, 00Eh, 009h, 0C5h, 013h, 0E0h, 0C3h, 00Ch, 009h, 04Eh, 0C2h
        db  01Fh, 00Ah, 009h, 006h, 0FDh, 016h, 0E0h, 0E3h, 010h, 009h, 00Fh, 0C8h, 00Ch, 009h, 0E0h, 0C3h
        db  00Eh, 009h, 04Eh, 0C2h, 01Fh, 00Ah, 009h, 006h, 0FDh, 016h, 0E0h, 0E3h, 010h, 009h, 00Fh, 0C8h
        db  00Eh, 009h, 00Fh, 002h, 0FFh, 0FFh, 04Eh, 0C2h, 01Fh, 00Ah, 009h, 006h, 0FDh, 016h, 00Dh, 002h
        db  000h, 008h, 060h, 0C2h, 010h, 009h, 04Fh, 026h, 0A5h, 016h, 04Fh, 005h, 00Fh, 0C8h, 010h, 009h
        db  00Fh, 002h, 002h, 002h, 00Eh, 002h, 003h, 000h, 060h, 0C3h, 040h, 001h, 00Ch, 002h, 0EEh, 0CCh
        db  0A0h, 001h, 040h, 001h, 000h, 004h, 0CFh, 005h, 009h, 002h, 055h, 055h, 09Ch, 006h, 049h, 005h
        db  09Ch, 006h, 009h, 007h, 09Ch, 006h, 049h, 005h, 09Ch, 006h, 00Eh, 006h, 0F4h, 016h, 0A0h, 001h
        db  040h, 001h, 000h, 040h, 00Dh, 0C8h, 040h, 001h, 009h, 002h, 008h, 000h, 00Eh, 002h, 00Ah, 009h
        db  00Fh, 002h, 002h, 002h, 0FEh, 0CFh, 049h, 006h, 0FDh, 016h, 060h, 004h, 06Ch, 0CAh, 0C9h, 0C7h
        db  05Fh, 082h, 001h, 016h, 05Bh, 004h, 0A0h, 001h, 040h, 001h, 000h, 040h, 00Dh, 0C8h, 040h, 001h
        db  00Dh, 002h, 000h, 001h, 052h, 004h, 08Dh, 007h, 000h, 010h, 020h, 020h, 00Ah, 0E0h, 00Ah, 016h
        db  08Dh, 007h, 000h, 008h, 020h, 020h, 010h, 0E0h, 005h, 013h, 08Dh, 007h, 000h, 040h, 04Fh, 001h
        db  001h, 000h, 001h, 016h, 05Bh, 004h, 020h, 0E0h, 010h, 0E0h, 08Dh, 007h, 036h, 007h, 089h, 007h
        db  04Ah, 008h, 04Dh, 0CAh, 002h, 000h, 0A0h, 006h, 006h, 0CFh, 089h, 007h, 062h, 008h, 04Dh, 0CAh
        db  002h, 000h, 0A0h, 006h, 006h, 0CFh, 089h, 007h, 07Ah, 008h, 04Dh, 0CAh, 002h, 000h, 0A0h, 006h
        db  006h, 0CFh, 0E0h, 004h, 0B8h, 008h, 0E0h, 004h, 0BAh, 008h, 00Eh, 0C8h, 022h, 008h, 00Fh, 0C8h
        db  024h, 008h, 00Eh, 0C8h, 040h, 008h, 00Fh, 0C8h, 042h, 008h, 0E0h, 004h, 030h, 008h, 0E0h, 004h
        db  036h, 008h, 0E0h, 002h, 078h, 007h, 0E0h, 004h, 046h, 008h, 0E0h, 004h, 048h, 008h, 020h, 040h
        db  042h, 0E3h, 020h, 0E0h, 00Ch, 0E0h, 060h, 004h, 058h, 0D2h, 080h, 001h, 000h, 0F0h, 00Dh, 010h
        db  0C0h, 001h, 000h, 0F0h, 020h, 040h, 006h, 0E0h, 008h, 010h, 0C0h, 001h, 000h, 0F0h, 080h, 001h
        db  000h, 020h, 0E0h, 0C3h, 046h, 008h, 001h, 016h, 05Bh, 004h, 04Bh, 0C0h, 020h, 004h, 07Ch, 0EAh
        db  040h, 001h, 000h, 020h, 0FBh, 016h, 051h, 004h, 0A0h, 0C2h, 0D8h, 007h, 04Ah, 001h, 040h, 000h
        db  001h, 016h, 05Bh, 004h, 0E0h, 002h, 078h, 007h, 020h, 020h, 00Ch, 0E0h, 0EFh, 016h, 043h, 0C2h
        db  002h, 013h, 0A0h, 006h, 0F4h, 0CEh, 020h, 02Fh, 036h, 007h, 020h, 040h, 00Ch, 0E0h, 0A0h, 006h
        db  09Ah, 0CDh, 0A0h, 0C3h, 046h, 008h, 0FBh, 016h, 0A0h, 006h, 02Eh, 0CEh, 08Eh, 007h, 0B6h, 008h
        db  09Eh, 007h, 000h, 080h, 020h, 020h, 010h, 0E0h, 005h, 016h, 08Fh, 007h, 022h, 008h, 0BFh, 0CFh
        db  0BFh, 0CFh, 09Fh, 0C7h, 0A0h, 006h, 052h, 0CEh, 0B0h, 003h, 020h, 0E8h, 03Eh, 0E3h, 062h, 007h
        db  0A0h, 006h, 02Eh, 0CEh, 0B0h, 003h, 020h, 048h, 03Eh, 0E3h, 062h, 007h, 020h, 040h, 042h, 0E3h
        db  020h, 0E0h, 004h, 0E0h, 0B0h, 003h, 020h, 048h, 010h, 0E0h, 058h, 007h, 05Bh, 004h, 080h, 001h
        db  000h, 0F0h, 020h, 0E0h, 004h, 0E0h, 0E0h, 0C3h, 0B2h, 008h, 0E0h, 023h, 014h, 0E0h, 002h, 013h
        db  09Bh, 006h, 0B4h, 010h, 0B0h, 003h, 020h, 0E8h, 01Eh, 0E0h, 058h, 007h, 0AFh, 010h, 09Bh, 006h
        db  080h, 003h, 0E0h, 002h, 058h, 007h, 08Dh, 007h, 0B2h, 008h, 0A0h, 006h, 086h, 0C4h, 0E0h, 002h
        db  078h, 007h, 05Bh, 004h, 060h, 0C2h, 05Ch, 007h, 020h, 006h, 046h, 008h, 0A9h, 0C2h, 008h, 000h
        db  0A0h, 006h, 0F4h, 0CEh, 069h, 001h, 00Ah, 000h, 001h, 000h, 036h, 013h, 049h, 0C3h, 02Dh, 002h
        db  00Eh, 000h, 00Ah, 0C3h, 01Dh, 0D3h, 08Ch, 001h, 000h, 084h, 0CCh, 001h, 000h, 040h, 00Ah, 001h
        db  000h, 05Eh, 008h, 013h, 00Eh, 002h, 000h, 023h, 04Eh, 0DBh, 001h, 000h, 0CCh, 001h, 000h, 004h
        db  04Ch, 0D7h, 001h, 010h, 04Ch, 0C7h, 0A9h, 0C3h, 010h, 000h, 0E9h, 0C3h, 012h, 000h, 041h, 0CAh
        db  010h, 000h, 02Fh, 002h, 004h, 000h, 001h, 017h, 08Eh, 005h, 08Ch, 007h, 002h, 000h, 0A0h, 0D2h
        db  0F1h, 007h, 0A0h, 006h, 054h, 0C6h, 069h, 0C0h, 010h, 000h, 029h, 0C8h, 014h, 000h, 0B8h, 008h
        db  029h, 0C8h, 016h, 000h, 0BAh, 008h, 0A0h, 007h, 0B6h, 008h, 000h, 040h, 069h, 001h, 00Eh, 000h
        db  000h, 008h, 002h, 016h, 0A0h, 006h, 052h, 0CEh, 040h, 001h, 000h, 040h, 001h, 016h, 051h, 004h
        db  060h, 004h, 0ACh, 0CDh, 0A9h, 0C3h, 00Ch, 000h, 006h, 013h, 00Eh, 068h, 048h, 008h, 0A9h, 0C3h
        db  006h, 000h, 001h, 013h, 01Eh, 02Eh, 0E9h, 004h, 006h, 000h, 0E9h, 004h, 00Ch, 000h, 029h, 007h
        db  004h, 000h, 05Bh, 004h, 081h, 007h, 020h, 020h, 089h, 007h, 022h, 008h, 041h, 0CEh, 063h, 0CEh
        db  010h, 000h, 063h, 0C6h, 012h, 000h, 0A0h, 006h, 0B0h, 0C6h, 043h, 0C2h, 002h, 013h, 0A0h, 006h
        db  0F4h, 0CEh, 020h, 0E0h, 010h, 0E0h, 060h, 004h, 0DAh, 0CDh, 0A0h, 006h, 0B0h, 0C6h, 0A0h, 0C3h
        db  022h, 008h, 0E0h, 0C3h, 024h, 008h, 04Fh, 001h, 001h, 000h, 01Dh, 016h, 040h, 001h, 000h, 004h
        db  0E1h, 016h, 0A0h, 006h, 09Ah, 0CDh, 0A0h, 0C3h, 046h, 008h, 0FBh, 016h, 0A0h, 0C2h, 0F0h, 007h
        db  08Ch, 007h, 004h, 000h, 08Dh, 007h, 022h, 008h, 0A0h, 0C3h, 040h, 008h, 0E0h, 0C3h, 042h, 008h
        db  0A0h, 006h, 08Eh, 0C6h, 0A0h, 0C3h, 022h, 008h, 0E0h, 0C3h, 024h, 008h, 04Fh, 001h, 001h, 000h
        db  002h, 016h, 060h, 004h, 0DAh, 0CDh, 0A0h, 0C2h, 0F0h, 007h, 020h, 0C3h, 02Ah, 009h, 08Dh, 007h
        db  022h, 008h, 0A0h, 006h, 08Eh, 0C6h, 0C0h, 006h, 020h, 0D0h, 026h, 008h, 0C0h, 006h, 040h, 001h
        db  000h, 004h, 00Ah, 016h, 040h, 001h, 080h, 000h, 007h, 013h, 00Eh, 0C8h, 022h, 008h, 00Fh, 0C8h
        db  024h, 008h, 0A0h, 006h, 090h, 0CDh, 0DEh, 010h, 00Eh, 0C8h, 040h, 008h, 00Fh, 0C8h, 042h, 008h
        db  040h, 001h, 000h, 004h, 006h, 013h, 040h, 001h, 020h, 000h, 030h, 016h, 081h, 007h, 010h, 020h
        db  0A3h, 010h, 083h, 007h, 04Ah, 008h, 063h, 007h, 004h, 000h, 00Eh, 011h, 083h, 007h, 062h, 008h
        db  063h, 007h, 004h, 000h, 009h, 011h, 083h, 007h, 07Ah, 008h, 063h, 007h, 004h, 000h, 004h, 011h
        db  0C3h, 060h, 0A0h, 006h, 09Ah, 0CDh, 0EDh, 010h, 0E0h, 0C8h, 026h, 008h, 00Eh, 000h, 0E0h, 0C8h
        db  040h, 008h, 010h, 000h, 0E0h, 0C8h, 042h, 008h, 012h, 000h, 0C7h, 004h, 040h, 001h, 020h, 000h
        db  0DDh, 016h, 020h, 0C8h, 028h, 008h, 044h, 008h, 009h, 016h, 081h, 007h, 080h, 020h, 0E0h, 0C8h
        db  040h, 008h, 014h, 000h, 0E0h, 0C8h, 042h, 008h, 016h, 000h, 0D2h, 010h, 0E0h, 0C8h, 040h, 008h
        db  014h, 000h, 0E0h, 0C8h, 042h, 008h, 016h, 000h, 080h, 001h, 000h, 004h, 082h, 007h, 02Ah, 008h
        db  032h, 0C1h, 008h, 011h, 072h, 0C1h, 092h, 0C1h, 082h, 007h, 03Ch, 008h, 004h, 0C1h, 007h, 016h
        db  060h, 004h, 006h, 0D1h, 072h, 0C1h, 0B2h, 0C1h, 084h, 001h, 000h, 080h, 0F9h, 013h, 004h, 068h
        db  044h, 008h, 0C7h, 0C1h, 023h, 016h, 020h, 098h, 049h, 008h, 033h, 009h, 00Ah, 016h, 081h, 007h
        db  040h, 020h, 0E0h, 0C1h, 046h, 008h, 065h, 013h, 0A0h, 006h, 09Ah, 0CDh, 0F4h, 010h, 0A0h, 006h
        db  08Ah, 0CDh, 081h, 02Dh, 001h, 0C2h, 0FBh, 013h, 0A0h, 005h, 048h, 008h, 0C3h, 0C1h, 027h, 002h
        db  006h, 000h, 0A0h, 006h, 0CCh, 0C1h, 0A3h, 005h, 00Ch, 000h, 008h, 0C8h, 06Ch, 001h, 008h, 0C8h
        db  092h, 008h, 008h, 002h, 004h, 0FCh, 0F8h, 0C1h, 004h, 0C1h, 059h, 013h, 0E0h, 0D2h, 003h, 001h
        db  00Bh, 016h, 0E0h, 0C2h, 03Ah, 007h, 0E0h, 042h, 062h, 007h, 0E0h, 026h, 03Ch, 0E3h, 002h, 013h
        db  0A0h, 006h, 08Ah, 0CDh, 0A0h, 006h, 0B0h, 0C6h, 020h, 0C8h, 0E2h, 007h, 01Ah, 001h, 00Bh, 002h
        db  00Ah, 001h, 0C4h, 0CEh, 0C7h, 0CEh, 0C5h, 0CEh, 0C6h, 0CEh, 0FBh, 004h, 009h, 002h, 000h, 004h
        db  048h, 0A2h, 0C9h, 0C6h, 020h, 0A8h, 092h, 008h, 012h, 001h, 020h, 0D8h, 0F2h, 007h, 000h, 001h
        db  047h, 0C2h, 0C4h, 081h, 001h, 014h, 044h, 0C2h, 0C9h, 061h, 009h, 0A2h, 089h, 0A1h, 001h, 017h
        db  085h, 005h, 009h, 061h, 0AEh, 016h, 082h, 002h, 03Ch, 008h, 005h, 016h, 040h, 001h, 010h, 000h
        db  026h, 013h, 060h, 004h, 03Ah, 0CFh, 060h, 004h, 040h, 0D0h, 0C1h, 006h, 0C1h, 0D8h, 00Fh, 000h
        db  0A0h, 006h, 09Ah, 0CDh, 020h, 0E0h, 006h, 0E0h, 043h, 0C2h, 081h, 007h, 058h, 0D2h, 060h, 004h
        db  06Ch, 0CEh, 020h, 020h, 012h, 0E0h, 005h, 016h, 081h, 007h, 044h, 000h, 020h, 020h, 012h, 0E0h
        db  0ECh, 013h, 060h, 004h, 018h, 0CFh, 081h, 007h, 080h, 020h, 0F8h, 010h, 081h, 007h, 080h, 020h
        db  0F0h, 010h, 081h, 007h, 002h, 020h, 0F2h, 010h, 081h, 007h, 004h, 020h, 0EFh, 010h, 023h, 0C8h
        db  008h, 000h, 06Ch, 001h, 007h, 005h, 0E0h, 0A1h, 0E8h, 000h, 00Ch, 002h, 002h, 0FCh, 0BCh, 007h
        db  000h, 081h, 007h, 0CFh, 0E0h, 0D2h, 003h, 001h, 00Bh, 016h, 0E0h, 0C2h, 03Ah, 007h, 0E0h, 042h
        db  062h, 007h, 0E0h, 026h, 03Ch, 0E3h, 002h, 013h, 0A0h, 006h, 08Ah, 0CDh, 0A0h, 006h, 0B0h, 0C6h
        db  023h, 0C8h, 006h, 000h, 06Ch, 001h, 0A3h, 0C2h, 00Eh, 000h, 04Ah, 001h, 000h, 001h, 00Bh, 013h
        db  00Ch, 002h, 00Eh, 0FCh, 05Ch, 0C2h, 049h, 002h, 000h, 080h, 00Dh, 002h, 01Ah, 009h, 07Dh, 0E2h
        db  009h, 0CFh, 03Dh, 0CFh, 03Dh, 0CFh, 00Ch, 002h, 000h, 0FCh, 06Ch, 0C3h, 006h, 000h, 04Dh, 002h
        db  0FFh, 0E0h, 04Ah, 002h, 000h, 002h, 08Ah, 0A2h, 08Ah, 0A2h, 04Ah, 0E3h, 00Dh, 0CBh, 006h, 000h
        db  0CDh, 006h, 0E0h, 0C2h, 044h, 008h, 0B7h, 016h, 00Bh, 002h, 00Fh, 000h, 0ECh, 082h, 004h, 000h
        db  0B2h, 011h, 0ECh, 0C3h, 00Eh, 000h, 011h, 015h, 010h, 013h, 06Ch, 0C2h, 014h, 000h, 049h, 002h
        db  000h, 01Fh, 0ACh, 013h, 0C9h, 006h, 089h, 002h, 012h, 000h, 0A8h, 01Bh, 049h, 001h, 001h, 000h
        db  0A5h, 013h, 0C9h, 0A2h, 0ECh, 082h, 004h, 000h, 0A1h, 011h, 04Dh, 0A3h, 0A2h, 018h, 014h, 011h
        db  060h, 001h, 018h, 009h, 000h, 080h, 018h, 013h, 01Dh, 009h, 0CCh, 0A2h, 0EBh, 0C2h, 008h, 000h
        db  07Bh, 009h, 04Bh, 002h, 01Eh, 000h, 0A0h, 0C3h, 0F0h, 006h, 0ABh, 023h, 004h, 0E0h, 094h, 016h
        db  060h, 027h, 040h, 0E3h, 091h, 016h, 04Dh, 0A3h, 04Dh, 0A3h, 04Dh, 0A3h, 0CDh, 006h, 04Dh, 002h
        db  007h, 000h, 00Dh, 088h, 0EEh, 006h, 00Dh, 015h, 0A3h, 007h, 00Ah, 000h, 004h, 002h, 090h, 003h
        db  07Fh, 040h, 053h, 02Fh, 0A0h, 005h, 046h, 008h, 0C3h, 004h, 0C0h, 001h, 000h, 004h, 060h, 004h
        db  03Eh, 0CFh, 060h, 001h, 018h, 009h, 000h, 080h, 0EFh, 013h, 001h, 002h, 008h, 020h, 060h, 004h
        db  03Ch, 0D1h, 08Dh, 007h, 000h, 010h, 020h, 020h, 00Ah, 0E0h, 00Ah, 016h, 08Dh, 007h, 000h, 008h
        db  020h, 020h, 00Eh, 0E0h, 005h, 013h, 08Dh, 007h, 000h, 040h, 04Fh, 001h, 001h, 000h, 001h, 016h
        db  05Bh, 004h, 020h, 0E0h, 00Eh, 0E0h, 0A0h, 007h, 0ACh, 008h, 000h, 080h, 00Eh, 0C8h, 0F4h, 007h
        db  00Fh, 0C8h, 0F6h, 007h, 00Eh, 0C8h, 014h, 008h, 00Fh, 0C8h, 016h, 008h, 0E0h, 004h, 002h, 008h
        db  0E0h, 004h, 008h, 008h, 0E0h, 002h, 098h, 007h, 020h, 040h, 04Eh, 0E3h, 020h, 007h, 02Eh, 006h
        db  060h, 004h, 02Eh, 0D6h, 000h, 070h, 04Bh, 0C0h, 0E0h, 004h, 02Eh, 006h, 00Bh, 010h, 020h, 0F0h
        db  04Dh, 0E3h, 002h, 010h, 020h, 0F0h, 04Ch, 0E3h, 04Bh, 0C0h, 0E0h, 004h, 02Eh, 006h, 0E0h, 001h
        db  062h, 007h, 040h, 000h, 020h, 0E8h, 048h, 0E3h, 062h, 007h, 020h, 004h, 07Ch, 0EAh, 040h, 001h
        db  000h, 020h, 004h, 013h, 0FAh, 010h, 040h, 001h, 000h, 040h, 0F7h, 016h, 020h, 007h, 02Eh, 006h
        db  020h, 050h, 052h, 0E3h, 051h, 004h, 020h, 024h, 054h, 0E3h, 0EFh, 013h, 0E0h, 080h, 020h, 008h
        db  003h, 013h, 0E0h, 0C2h, 020h, 008h, 01Bh, 02Eh, 003h, 0C3h, 00Ch, 0C8h, 06Ch, 001h, 0E0h, 0C2h
        db  002h, 0FCh, 04Bh, 001h, 000h, 001h, 003h, 013h, 020h, 0C3h, 000h, 0FCh, 0F6h, 010h, 0CCh, 080h
        db  003h, 013h, 020h, 0C8h, 000h, 0FCh, 0F8h, 005h, 0E0h, 004h, 000h, 0FCh, 020h, 024h, 050h, 0E3h
        db  00Bh, 013h, 08Bh, 007h, 001h, 000h, 020h, 0C3h, 060h, 0EAh, 00Bh, 0CBh, 018h, 000h, 003h, 0CBh
        db  014h, 000h, 020h, 004h, 060h, 0EAh, 001h, 010h, 013h, 02Eh, 020h, 040h, 054h, 0E3h, 020h, 007h
        db  02Eh, 006h, 060h, 004h, 02Eh, 0D6h, 0C1h, 010h, 0E0h, 002h, 058h, 007h, 08Dh, 007h, 0A8h, 008h
        db  0A0h, 006h, 086h, 0C4h, 0E0h, 002h, 098h, 007h, 05Bh, 004h, 0E0h, 002h, 058h, 007h, 020h, 020h
        db  006h, 0E0h, 010h, 016h, 020h, 0E0h, 01Ch, 0E0h, 0E0h, 002h, 098h, 007h, 0C0h, 001h, 000h, 080h
        db  0A4h, 010h, 020h, 0C1h, 0A8h, 008h, 020h, 021h, 014h, 0E0h, 006h, 016h, 020h, 0E8h, 01Ch, 0E0h
        db  058h, 007h, 080h, 003h, 0E0h, 002h, 098h, 007h, 020h, 0D8h, 0DCh, 007h, 017h, 001h, 08Fh, 007h
        db  08Eh, 0FFh, 00Fh, 0C8h, 004h, 001h, 020h, 0E8h, 006h, 0E0h, 058h, 007h, 080h, 001h, 000h, 080h
        db  05Bh, 004h, 0E0h, 0C2h, 020h, 008h, 0C3h, 082h, 003h, 013h, 0DBh, 02Dh, 003h, 0C8h, 020h, 008h
        db  003h, 0C8h, 06Ch, 001h, 060h, 0C2h, 002h, 0FCh, 049h, 001h, 000h, 001h, 002h, 016h, 060h, 004h
        db  072h, 0D5h, 0E0h, 0C0h, 0F8h, 005h, 0FDh, 013h, 000h, 003h, 002h, 000h, 0E0h, 0C0h, 0F8h, 005h
        db  003h, 0C8h, 06Ch, 001h, 020h, 0C8h, 000h, 0FCh, 0F8h, 005h, 000h, 003h, 00Fh, 000h, 003h, 0C8h
        db  06Ch, 001h, 088h, 007h, 002h, 0FCh, 078h, 0C2h, 0F8h, 0C1h, 049h, 001h, 000h, 001h, 04Dh, 016h
        db  009h, 001h, 000h, 05Eh, 029h, 016h, 049h, 001h, 002h, 000h, 00Bh, 016h, 060h, 001h, 012h, 008h
        db  000h, 002h, 00Ah, 016h, 027h, 002h, 004h, 000h, 007h, 088h, 02Ch, 009h, 005h, 012h, 027h, 002h
        db  0FCh, 0FFh, 0A0h, 001h, 012h, 008h, 000h, 002h, 0C7h, 0C1h, 037h, 015h, 0D3h, 02Dh, 0E0h, 0C0h
        db  020h, 008h, 007h, 0A8h, 01Eh, 008h, 007h, 0A8h, 01Ch, 008h, 00Ch, 015h, 020h, 0C8h, 014h, 008h
        db  0F4h, 007h, 020h, 0C8h, 016h, 008h, 0F6h, 007h, 020h, 0C8h, 018h, 008h, 014h, 008h, 020h, 0C8h
        db  01Ah, 008h, 016h, 008h, 060h, 004h, 072h, 0D5h, 0A0h, 006h, 0B0h, 0C6h, 0D3h, 02Dh, 0E0h, 0C2h
        db  020h, 008h, 0C3h, 082h, 001h, 013h, 0DBh, 02Dh, 020h, 088h, 016h, 008h, 010h, 008h, 00Dh, 016h
        db  020h, 088h, 014h, 008h, 00Eh, 008h, 009h, 016h, 0E0h, 004h, 01Ch, 008h, 082h, 007h, 0FCh, 007h
        db  004h, 061h, 0E0h, 004h, 01Eh, 008h, 060h, 004h, 036h, 0D6h, 020h, 0C8h, 00Eh, 008h, 0F4h, 007h
        db  020h, 0C8h, 010h, 008h, 0F6h, 007h, 060h, 004h, 02Eh, 0D6h, 007h, 0A8h, 01Eh, 008h, 004h, 0C1h
        db  019h, 016h, 082h, 002h, 00Eh, 008h, 008h, 016h, 060h, 001h, 0F6h, 007h, 001h, 000h, 002h, 016h
        db  0A0h, 006h, 0A0h, 0D7h, 0A0h, 006h, 020h, 0D7h, 032h, 0C1h, 007h, 011h, 072h, 0C1h, 092h, 0C1h
        db  082h, 007h, 00Eh, 008h, 004h, 0C1h, 006h, 016h, 0ECh, 010h, 072h, 0C1h, 0B2h, 0C1h, 084h, 001h
        db  000h, 080h, 0E7h, 013h, 0E0h, 0D2h, 003h, 001h, 00Ah, 016h, 020h, 001h, 03Ah, 007h, 000h, 070h
        db  004h, 013h, 0A0h, 006h, 0C4h, 0D2h, 020h, 007h, 02Eh, 006h, 0A0h, 006h, 0B0h, 0C6h, 020h, 0C8h
        db  0E0h, 007h, 01Ah, 001h, 00Bh, 002h, 00Ah, 001h, 0C4h, 0CEh, 0C7h, 0CEh, 0C5h, 0CEh, 0C6h, 0CEh
        db  0FBh, 004h, 009h, 002h, 000h, 004h, 048h, 0A2h, 0C9h, 0C6h, 003h, 0A8h, 012h, 001h, 020h, 0D8h
        db  0F3h, 007h, 000h, 001h, 047h, 0C2h, 0C4h, 081h, 001h, 014h, 044h, 0C2h, 009h, 0A8h, 01Ch, 008h
        db  009h, 061h, 009h, 0A2h, 089h, 0A1h, 001h, 017h, 085h, 005h, 0C9h, 061h, 0B8h, 016h, 040h, 001h
        db  040h, 000h, 015h, 016h, 087h, 007h, 020h, 000h, 0E0h, 061h, 01Ch, 008h, 0C4h, 081h, 008h, 01Ah
        db  007h, 0A8h, 01Eh, 008h, 007h, 0A8h, 01Ch, 008h, 007h, 061h, 087h, 0A1h, 001h, 017h, 085h, 005h
        db  080h, 001h, 040h, 000h, 003h, 0C8h, 06Ch, 001h, 0E0h, 0C1h, 004h, 0FCh, 09Eh, 010h, 060h, 004h
        db  0C2h, 0D3h, 0E0h, 0D2h, 003h, 001h, 00Ah, 016h, 020h, 001h, 03Ah, 007h, 000h, 070h, 004h, 013h
        db  0A0h, 006h, 0C4h, 0D2h, 020h, 007h, 02Eh, 006h, 0A0h, 006h, 0B0h, 0C6h, 090h, 003h, 03Fh, 040h
        db  0D3h, 02Dh, 060h, 001h, 0F6h, 007h, 001h, 000h, 002h, 016h, 0A0h, 006h, 0A0h, 0D7h, 060h, 0D2h
        db  012h, 008h, 089h, 001h, 000h, 0F1h, 0C9h, 001h, 000h, 070h, 020h, 088h, 016h, 008h, 010h, 008h
        db  004h, 016h, 020h, 088h, 014h, 008h, 00Eh, 008h, 014h, 013h, 089h, 001h, 000h, 010h, 08Dh, 007h
        db  01Ch, 008h, 09Dh, 007h, 000h, 050h, 0A0h, 0D2h, 0F1h, 007h, 08Ch, 007h, 002h, 000h, 0A0h, 0C3h
        db  014h, 008h, 0E0h, 0C3h, 016h, 008h, 02Fh, 002h, 004h, 000h, 001h, 017h, 08Eh, 005h, 0A0h, 006h
        db  054h, 0C6h, 08Dh, 007h, 0F8h, 007h, 049h, 0CFh, 060h, 0C7h, 01Eh, 008h, 04Dh, 006h, 0A0h, 0D2h
        db  0F1h, 007h, 08Ch, 007h, 004h, 000h, 0A0h, 0C3h, 00Eh, 008h, 0E0h, 0C3h, 010h, 008h, 0CCh, 0A3h
        db  001h, 017h, 08Eh, 005h, 0A0h, 006h, 054h, 0C6h, 020h, 0C8h, 014h, 008h, 0AEh, 008h, 020h, 0C8h
        db  016h, 008h, 0B0h, 008h, 009h, 001h, 000h, 00Ch, 00Ah, 013h, 049h, 001h, 000h, 004h, 005h, 016h
        db  0A0h, 006h, 068h, 0D3h, 0A0h, 006h, 0D4h, 0D2h, 002h, 010h, 0A0h, 006h, 068h, 0D3h, 0C0h, 001h
        db  080h, 000h, 0A0h, 006h, 020h, 0D7h, 0E0h, 0C2h, 042h, 007h, 02Eh, 013h, 0E0h, 0C2h, 02Eh, 006h
        db  02Bh, 013h, 0E0h, 002h, 058h, 007h, 08Fh, 007h, 0BFh, 0FFh, 00Fh, 02Ch, 0E0h, 002h, 098h, 007h
        db  0E0h, 0C0h, 05Ch, 007h, 003h, 0C8h, 020h, 008h, 003h, 0C8h, 06Ch, 001h, 0C3h, 0C2h, 0CBh, 0A2h
        db  0EBh, 0C2h, 032h, 00Ch, 015h, 013h, 00Bh, 0C8h, 000h, 0FCh, 00Bh, 0C3h, 04Bh, 0C3h, 00Bh, 0C8h
        db  06Ch, 001h, 0E0h, 0C2h, 000h, 0FCh, 0FAh, 016h, 000h, 003h, 002h, 000h, 020h, 0C8h, 0F8h, 005h
        db  000h, 0FCh, 002h, 016h, 00Dh, 0C8h, 0FAh, 005h, 00Ch, 0C8h, 0F8h, 005h, 000h, 003h, 00Fh, 000h
        db  040h, 001h, 020h, 000h, 020h, 016h, 01Fh, 010h, 0A0h, 0C3h, 02Eh, 006h, 003h, 013h, 0E0h, 0C0h
        db  0F8h, 005h, 012h, 016h, 020h, 0C3h, 014h, 009h, 002h, 013h, 020h, 004h, 058h, 0EAh, 04Fh, 02Eh
        db  0C0h, 001h, 000h, 080h, 0A0h, 001h, 062h, 007h, 000h, 080h, 08Eh, 0C3h, 003h, 013h, 0A0h, 001h
        db  062h, 007h, 040h, 000h, 060h, 004h, 0EAh, 0D2h, 003h, 0C8h, 06Ch, 001h, 020h, 0C8h, 000h, 0FCh
        db  0F8h, 005h, 003h, 0C8h, 020h, 008h, 060h, 001h, 018h, 009h, 000h, 004h, 002h, 013h, 060h, 004h
        db  0FEh, 0D3h, 08Ch, 007h, 00Eh, 000h, 003h, 0C8h, 06Ch, 001h, 020h, 0C2h, 00Eh, 0FCh, 00Ah, 015h
        db  009h, 013h, 020h, 0C2h, 014h, 0FCh, 048h, 002h, 000h, 01Fh, 0C8h, 006h, 088h, 002h, 012h, 000h
        db  0EEh, 01Bh, 008h, 0A3h, 088h, 007h, 002h, 0FCh, 078h, 0C2h, 0F8h, 0C1h, 007h, 083h, 0E7h, 01Ah
        db  0CCh, 061h, 007h, 0C8h, 004h, 0FCh, 0CCh, 0C1h, 0C0h, 001h, 040h, 000h, 060h, 004h, 00Ah, 0D4h
        db  00Bh, 0C1h, 0A0h, 0C2h, 0F0h, 007h, 020h, 0C3h, 028h, 009h, 08Dh, 007h, 0F4h, 007h, 0A0h, 0C3h
        db  0F4h, 007h, 0E0h, 0C3h, 0F6h, 007h, 0A0h, 006h, 054h, 0C6h, 020h, 0C8h, 014h, 008h, 018h, 008h
        db  020h, 0C8h, 016h, 008h, 01Ah, 008h, 00Eh, 0C8h, 014h, 008h, 00Fh, 0C8h, 016h, 008h, 082h, 007h
        db  0FCh, 007h, 0E0h, 004h, 01Ch, 008h, 040h, 001h, 080h, 000h, 006h, 016h, 00Eh, 0C8h, 00Eh, 008h
        db  00Fh, 0C8h, 010h, 008h, 0E0h, 004h, 01Eh, 008h, 0A0h, 006h, 0B0h, 0C6h, 0E0h, 0C2h, 0F8h, 007h
        db  00Dh, 011h, 00Eh, 0C8h, 0F4h, 007h, 00Fh, 0C8h, 0F6h, 007h, 020h, 0C8h, 018h, 008h, 014h, 008h
        db  020h, 0C8h, 01Ah, 008h, 016h, 008h, 0A0h, 006h, 0CEh, 0D2h, 0CBh, 010h, 040h, 001h, 080h, 000h
        db  002h, 016h, 00Bh, 0C8h, 012h, 008h, 0C4h, 0C2h, 004h, 061h, 080h, 001h, 080h, 000h, 05Bh, 004h
        db  08Bh, 0C0h, 0A0h, 0C2h, 0F0h, 007h, 08Ch, 007h, 004h, 000h, 08Dh, 007h, 0F4h, 007h, 0A0h, 0C3h
        db  014h, 008h, 0E0h, 0C3h, 016h, 008h, 0A0h, 006h, 08Eh, 0C6h, 060h, 001h, 0F6h, 007h, 001h, 000h
        db  004h, 013h, 0A0h, 007h, 0ACh, 008h, 000h, 080h, 052h, 004h, 0E0h, 0C3h, 0A8h, 008h, 0E0h, 023h
        db  014h, 0E0h, 007h, 016h, 020h, 0D0h, 004h, 0E0h, 020h, 0E8h, 01Ah, 0E0h, 058h, 007h, 060h, 004h
        db  0DAh, 0D2h, 0A0h, 007h, 0ACh, 008h, 000h, 040h, 020h, 0C8h, 014h, 008h, 0AEh, 008h, 020h, 0C8h
        db  016h, 008h, 0B0h, 008h, 0A0h, 006h, 068h, 0D3h, 0A0h, 006h, 0D4h, 0D2h, 0D2h, 010h, 0ADh, 0C2h
        db  002h, 000h, 06Dh, 0C2h, 000h, 000h, 005h, 016h, 0AAh, 007h, 002h, 000h, 036h, 007h, 09Ah, 02Ch
        db  080h, 003h, 0EAh, 02Ch, 002h, 000h, 041h, 0CBh, 000h, 000h, 080h, 003h, 02Dh, 0C3h, 018h, 000h
        db  0ACh, 007h, 002h, 000h, 036h, 007h, 020h, 04Bh, 098h, 0EAh, 00Ah, 000h, 020h, 0EBh, 092h, 0EAh
        db  00Ah, 000h, 09Ch, 02Eh, 080h, 003h, 0A0h, 0C2h, 022h, 0E0h, 060h, 004h, 07Ch, 0B3h, 0EDh, 0C0h
        db  018h, 000h, 0A0h, 006h, 06Eh, 0D8h, 080h, 003h, 044h, 0C2h, 0C3h, 0C0h, 002h, 013h, 0A0h, 006h
        db  06Eh, 0D8h, 019h, 0C3h, 009h, 0CBh, 018h, 000h, 0C9h, 005h, 019h, 0CBh, 016h, 000h, 04Ch, 0C2h
        db  02Ch, 002h, 01Ah, 000h, 00Dh, 0CFh, 00Eh, 0CFh, 00Fh, 0C7h, 099h, 000h, 05Bh, 004h, 08Ch, 007h
        db  0BCh, 008h, 09Ch, 0C2h, 0A0h, 022h, 014h, 0E0h, 006h, 013h, 0A0h, 0C2h, 058h, 007h, 0A0h, 022h
        db  020h, 0E0h, 001h, 016h, 080h, 003h, 003h, 0C1h, 0C3h, 004h, 08Ah, 007h, 004h, 000h, 084h, 0A2h
        db  03Ah, 0CFh, 03Ah, 0CFh, 03Ah, 0CFh, 03Ah, 0CFh, 03Ah, 0CFh, 0E0h, 002h, 058h, 007h, 08Dh, 007h
        db  0BCh, 008h, 00Bh, 0C8h, 0C2h, 007h, 0A0h, 006h, 086h, 0C4h, 0E0h, 0C2h, 0C2h, 007h, 020h, 0E0h
        db  020h, 0E0h, 0E0h, 002h, 0B8h, 007h, 05Bh, 004h, 02Dh, 0C3h, 018h, 000h, 08Ch, 0C2h, 060h, 0C2h
        db  06Ch, 001h, 00Ah, 0C8h, 06Ch, 001h, 0E0h, 0C2h, 000h, 0FCh, 002h, 013h, 08Bh, 0C2h, 0F9h, 010h
        db  009h, 0C8h, 06Ch, 001h, 08Bh, 007h, 0F8h, 005h, 05Bh, 0C2h, 00Ch, 013h, 0CBh, 005h, 05Bh, 0C2h
        db  0CAh, 0C6h, 0E0h, 0C2h, 06Ch, 001h, 009h, 0C8h, 06Ch, 001h, 00Ch, 0C8h, 000h, 0FCh, 00Bh, 0C8h
        db  06Ch, 001h, 002h, 010h, 0CCh, 0CEh, 0CAh, 0C6h, 0A0h, 0C2h, 0E0h, 000h, 0A0h, 022h, 01Ah, 0E0h
        db  006h, 016h, 020h, 0E8h, 004h, 0E0h, 03Ah, 007h, 020h, 048h, 01Ah, 0E0h, 0E0h, 000h, 080h, 003h
        db  0E0h, 0D3h, 0ADh, 0E3h, 0E0h, 0C1h, 0A8h, 006h, 005h, 016h, 007h, 002h, 0A2h, 006h, 0A0h, 006h
        db  0F8h, 0C1h, 00Bh, 016h, 0E0h, 0C1h, 0BAh, 006h, 020h, 016h, 007h, 002h, 0B4h, 006h, 0A0h, 006h
        db  0F8h, 0C1h, 01Bh, 013h, 007h, 002h, 0B8h, 006h, 002h, 010h, 007h, 002h, 0A6h, 006h, 060h, 0C1h
        db  002h, 0FCh, 025h, 0C8h, 00Ch, 000h, 002h, 0FCh, 0C5h, 0C9h, 00Ch, 000h, 0F5h, 0CDh, 0F5h, 0CDh
        db  0F5h, 0CDh, 0F5h, 0CDh, 0F5h, 0CDh, 0F5h, 0C5h, 0B7h, 001h, 028h, 000h, 027h, 002h, 0F4h, 0FFh
        db  0A7h, 007h, 004h, 000h, 058h, 0DAh, 097h, 02Eh, 0D5h, 010h, 000h, 003h, 002h, 000h, 0A0h, 006h
        db  010h, 0C2h, 000h, 003h, 00Fh, 000h, 020h, 02Ch, 03Ah, 0EDh, 0E0h, 093h, 0ADh, 0E3h, 003h, 016h
        db  081h, 002h, 016h, 000h, 0C7h, 016h, 021h, 0C1h, 0A2h, 0EAh, 054h, 004h, 0E0h, 093h, 010h, 0E0h
        db  003h, 016h, 0A0h, 0D2h, 0AAh, 0E3h, 00Bh, 010h, 0CFh, 0D3h, 009h, 016h, 0A0h, 023h, 008h, 0E0h
        db  006h, 016h, 084h, 007h, 020h, 000h, 004h, 0E8h, 0D2h, 006h, 0A0h, 0D2h, 00Ch, 0E0h, 060h, 004h
        db  0D0h, 0DAh, 060h, 004h, 040h, 0DDh, 07Bh, 010h, 022h, 0C1h, 004h, 000h, 0E2h, 004h, 002h, 000h
        db  054h, 004h, 002h, 0C8h, 06Ch, 001h, 082h, 0A0h, 022h, 0C8h, 032h, 00Ch, 000h, 0FCh, 002h, 002h
        db  000h, 0FCh, 0E0h, 093h, 0ACh, 0E3h, 070h, 013h, 0A0h, 006h, 0D2h, 0DBh, 0A0h, 048h, 004h, 0E0h
        db  00Eh, 000h, 085h, 002h, 007h, 000h, 00Eh, 013h, 00Eh, 001h, 003h, 000h, 00Bh, 013h, 0A0h, 023h
        db  022h, 0E0h, 003h, 016h, 0A0h, 0D2h, 00Eh, 0E0h, 002h, 010h, 0A0h, 0D2h, 0AAh, 0E3h, 08Eh, 001h
        db  003h, 000h, 05Ah, 010h, 005h, 0C8h, 0FCh, 006h, 0C3h, 0C0h, 053h, 016h, 0A0h, 043h, 010h, 0E0h
        db  022h, 088h, 00Eh, 000h, 01Ah, 009h, 00Ah, 016h, 022h, 088h, 010h, 000h, 01Ch, 009h, 006h, 016h
        db  022h, 088h, 012h, 000h, 01Eh, 009h, 002h, 016h, 0A0h, 0E3h, 010h, 0E0h, 085h, 002h, 009h, 000h
        db  002h, 013h, 0A0h, 006h, 0DEh, 0E1h, 045h, 0A1h, 065h, 0C1h, 0AEh, 0E3h, 055h, 004h, 062h, 0C0h
        db  004h, 000h, 022h, 0C8h, 006h, 000h, 06Ch, 001h, 082h, 002h, 048h, 004h, 002h, 01Bh, 0A0h, 043h
        db  00Ch, 0E0h, 022h, 0C1h, 00Eh, 000h, 051h, 004h, 042h, 0C0h, 0E1h, 004h, 002h, 000h, 0A2h, 0C0h
        db  00Ch, 000h, 022h, 0C1h, 00Ah, 000h, 020h, 021h, 018h, 0E0h, 007h, 013h, 0A1h, 0C8h, 00Ah, 000h
        db  00Ah, 000h, 0A1h, 0C8h, 008h, 000h, 008h, 000h, 0E2h, 010h, 022h, 0C8h, 006h, 000h, 06Ch, 001h
        db  0A0h, 006h, 09Ch, 0E0h, 060h, 004h, 0AEh, 0DAh, 002h, 0C8h, 0D4h, 006h, 062h, 0C1h, 002h, 000h
        db  065h, 0C1h, 0D0h, 0E3h, 055h, 004h, 00Bh, 010h, 085h, 007h, 0F4h, 003h, 0F5h, 004h, 060h, 0CDh
        db  018h, 0EDh, 0A0h, 006h, 0C8h, 0E2h, 0A0h, 0E3h, 00Ch, 0E0h, 0A0h, 02Eh, 0F4h, 003h, 060h, 004h
        db  014h, 0D9h, 0A0h, 006h, 068h, 0DFh, 00Ch, 010h, 0A0h, 006h, 09Ch, 0E0h, 009h, 010h, 0A0h, 006h
        db  050h, 0E2h, 006h, 010h, 0A0h, 006h, 09Ch, 0E0h, 003h, 0C8h, 0DCh, 008h, 0A0h, 0D2h, 0ACh, 0E3h
        db  0A0h, 006h, 042h, 0DBh, 0A0h, 092h, 026h, 0E0h, 00Ch, 016h, 0E0h, 0D3h, 026h, 0E0h, 0E0h, 023h
        db  014h, 0E0h, 00Ah, 013h, 00Ah, 0C1h, 0C4h, 083h, 007h, 013h, 0C4h, 0C3h, 024h, 0C1h, 0D4h, 0E3h
        db  054h, 004h, 0CAh, 093h, 0DCh, 013h, 0CAh, 0D3h, 0B0h, 003h, 00Fh, 0D8h, 059h, 006h, 004h, 071h
        db  024h, 0C1h, 0E0h, 0E3h, 054h, 004h, 0A0h, 023h, 00Ch, 0E0h, 0D1h, 013h, 04Dh, 0C3h, 0CFh, 013h
        db  086h, 007h, 002h, 000h, 084h, 007h, 026h, 000h, 046h, 023h, 003h, 013h, 044h, 006h, 086h, 0A1h
        db  0FBh, 010h, 046h, 043h, 0B9h, 010h, 085h, 007h, 01Ch, 007h, 086h, 007h, 01Ah, 004h, 076h, 06Dh
        db  076h, 06Dh, 076h, 06Dh, 0C6h, 005h, 076h, 06Dh, 076h, 06Dh, 076h, 06Dh, 083h, 007h, 000h, 090h
        db  0BEh, 010h, 00Bh, 0C3h, 086h, 007h, 000h, 001h, 085h, 007h, 000h, 080h, 020h, 0C1h, 0D2h, 006h
        db  037h, 013h, 0C4h, 004h, 060h, 0C0h, 0D2h, 006h, 045h, 020h, 004h, 013h, 084h, 005h, 015h, 009h
        db  0F9h, 016h, 02Eh, 010h, 0CFh, 0D3h, 006h, 016h, 0E0h, 023h, 014h, 0E0h, 003h, 016h, 00Eh, 001h
        db  003h, 000h, 003h, 013h, 0E0h, 004h, 0D2h, 006h, 023h, 010h, 064h, 0D0h, 01Ch, 007h, 046h, 0B0h
        db  010h, 018h, 001h, 0D9h, 01Ch, 007h, 060h, 023h, 020h, 0E0h, 00Bh, 013h, 081h, 007h, 018h, 000h
        db  061h, 0C0h, 0F0h, 0E3h, 011h, 088h, 018h, 0EDh, 004h, 013h, 008h, 002h, 018h, 080h, 0A0h, 006h
        db  01Ch, 0DFh, 064h, 0D0h, 028h, 007h, 046h, 0B0h, 008h, 018h, 001h, 0D9h, 028h, 007h, 046h, 0B0h
        db  004h, 017h, 083h, 007h, 040h, 080h, 0A0h, 006h, 050h, 0E2h, 005h, 048h, 0D2h, 006h, 0CAh, 016h
        db  020h, 0C1h, 0E4h, 008h, 001h, 016h, 05Ch, 004h, 004h, 002h, 007h, 000h, 020h, 006h, 0E4h, 008h
        db  0C9h, 010h, 00Bh, 0C3h, 0C5h, 004h, 042h, 0C0h, 0C7h, 004h, 020h, 0C2h, 06Ch, 001h, 0E1h, 0A1h
        db  004h, 000h, 011h, 0C8h, 06Ch, 001h, 0FBh, 016h, 008h, 0C8h, 06Ch, 001h, 0C8h, 004h, 0A0h, 043h
        db  01Ah, 0E0h, 022h, 0C1h, 00Eh, 000h, 00Dh, 015h, 00Ch, 013h, 0A0h, 0E3h, 01Ah, 0E0h, 0A0h, 006h
        db  03Ah, 0E2h, 008h, 0C2h, 048h, 013h, 088h, 002h, 012h, 000h, 045h, 01Bh, 020h, 022h, 022h, 0E0h
        db  042h, 013h, 002h, 0C1h, 008h, 0A1h, 008h, 005h, 028h, 002h, 0F2h, 0FFh, 007h, 0A2h, 083h, 007h
        db  001h, 080h, 088h, 002h, 004h, 000h, 06Eh, 011h, 064h, 0C2h, 016h, 000h, 049h, 0D2h, 002h, 016h
        db  002h, 081h, 031h, 016h, 009h, 001h, 000h, 0F0h, 028h, 016h, 049h, 0C1h, 045h, 071h, 0C3h, 004h
        db  085h, 002h, 009h, 000h, 07Ch, 013h, 083h, 007h, 002h, 080h, 0A4h, 0C1h, 014h, 000h, 088h, 081h
        db  076h, 016h, 083h, 005h, 085h, 002h, 010h, 000h, 013h, 01Bh, 083h, 005h, 049h, 099h, 0C2h, 0EAh
        db  00Ah, 013h, 009h, 098h, 00Eh, 0E0h, 06Bh, 016h, 025h, 098h, 0C2h, 0EAh, 00Ch, 0E0h, 067h, 016h
        db  0E0h, 0C1h, 0ECh, 006h, 064h, 016h, 0C3h, 004h, 052h, 0C2h, 00Fh, 013h, 083h, 007h, 009h, 080h
        db  0E0h, 0C1h, 018h, 009h, 047h, 001h, 000h, 010h, 05Ah, 016h, 0A0h, 0C0h, 06Ch, 001h, 0A0h, 006h
        db  0F4h, 0E0h, 060h, 004h, 0AEh, 0DAh, 060h, 004h, 0B8h, 0DAh, 089h, 007h, 00Eh, 007h, 0C7h, 004h
        db  0E5h, 0D1h, 0D3h, 0EAh, 005h, 013h, 0C7h, 006h, 027h, 002h, 0E4h, 0EAh, 077h, 0CEh, 0FEh, 015h
        db  044h, 0C0h, 021h, 002h, 018h, 000h, 028h, 002h, 0FCh, 0FFh, 036h, 013h, 091h, 0C1h, 086h, 0D1h
        db  01Fh, 013h, 0C6h, 006h, 087h, 007h, 00Eh, 007h, 0F7h, 0C0h, 046h, 002h, 0FFh, 0BFh, 043h, 002h
        db  0FFh, 03Fh, 0A0h, 091h, 03Fh, 0EDh, 009h, 016h, 0B0h, 003h, 020h, 098h, 00Eh, 0E0h, 05Dh, 006h
        db  00Fh, 016h, 021h, 0C8h, 002h, 000h, 00Ch, 007h, 017h, 010h, 047h, 082h, 00Ch, 01Bh, 0C6h, 090h
        db  0EBh, 016h, 047h, 006h, 0F7h, 004h, 0B0h, 003h, 020h, 098h, 05Dh, 006h, 057h, 006h, 00Ch, 013h
        db  083h, 007h, 005h, 080h, 01Ch, 010h, 0D1h, 0C0h, 0E0h, 020h, 016h, 0E0h, 003h, 016h, 083h, 007h
        db  008h, 080h, 015h, 010h, 060h, 044h, 026h, 0E0h, 086h, 071h, 046h, 0A0h, 006h, 062h, 083h, 007h
        db  005h, 080h, 008h, 0C2h, 0CBh, 015h, 00Bh, 016h, 0C3h, 004h, 087h, 007h, 00Eh, 007h, 077h, 0C0h
        db  047h, 082h, 005h, 01Bh, 060h, 020h, 006h, 0E0h, 0FAh, 016h, 083h, 007h, 007h, 080h, 05Ch, 004h
        db  0A0h, 092h, 00Eh, 0E0h, 00Fh, 016h, 020h, 0C8h, 020h, 0E0h, 008h, 007h, 0E0h, 004h, 084h, 001h
        db  060h, 005h, 002h, 007h, 03Bh, 013h, 020h, 048h, 006h, 0E0h, 082h, 001h, 083h, 007h, 000h, 0C0h
        db  0A0h, 006h, 050h, 0E2h, 020h, 0C8h, 01Eh, 0E0h, 002h, 007h, 0A0h, 0E3h, 004h, 0E0h, 008h, 002h
        db  024h, 080h, 0A0h, 006h, 01Ch, 0DFh, 034h, 010h, 020h, 0C1h, 084h, 001h, 044h, 002h, 000h, 088h
        db  01Eh, 013h, 004h, 048h, 084h, 001h, 020h, 006h, 002h, 007h, 0F1h, 016h, 0A0h, 023h, 022h, 0E0h
        db  005h, 016h, 0A0h, 043h, 022h, 0E0h, 0A0h, 0D2h, 00Eh, 0E0h, 0D5h, 010h, 0E0h, 023h, 014h, 0E0h
        db  004h, 013h, 020h, 098h, 0ABh, 0E3h, 065h, 006h, 006h, 016h, 0A0h, 092h, 00Eh, 0E0h, 0CBh, 013h
        db  0A0h, 0D2h, 0AAh, 0E3h, 0D7h, 010h, 083h, 007h, 002h, 000h, 060h, 004h, 0C8h, 0DAh, 020h, 0E8h
        db  006h, 0E0h, 082h, 001h, 020h, 007h, 002h, 007h, 0A0h, 043h, 004h, 0E0h, 020h, 0C8h, 070h, 0E4h
        db  086h, 001h, 020h, 088h, 020h, 0E0h, 008h, 007h, 003h, 016h, 020h, 0C8h, 0FCh, 0EAh, 008h, 007h
        db  060h, 004h, 0D0h, 0DAh, 00Eh, 001h, 003h, 000h, 016h, 013h, 0CFh, 0D3h, 008h, 016h, 0A0h, 023h
        db  020h, 0E0h, 003h, 016h, 0A0h, 0D2h, 0AAh, 0E3h, 002h, 010h, 0A0h, 0D2h, 00Eh, 0E0h, 08Eh, 001h
        db  003h, 000h, 009h, 010h, 060h, 0C1h, 084h, 001h, 060h, 021h, 00Ah, 0E0h, 004h, 016h, 083h, 007h
        db  000h, 084h, 060h, 004h, 0C8h, 0DAh, 020h, 0C8h, 02Eh, 0E0h, 084h, 001h, 008h, 002h, 006h, 080h
        db  0A0h, 006h, 01Ch, 0DFh, 060h, 004h, 0D0h, 0DAh, 060h, 0E3h, 020h, 0E0h, 060h, 004h, 0D0h, 0DAh
        db  0E0h, 093h, 026h, 0E0h, 010h, 016h, 0A0h, 023h, 008h, 0E0h, 00Dh, 016h, 0A0h, 023h, 006h, 0E0h
        db  002h, 013h, 060h, 0E3h, 01Ch, 0E0h, 060h, 0E3h, 018h, 0E0h, 0A0h, 043h, 006h, 0E0h, 008h, 002h
        db  03Ch, 080h, 0A0h, 006h, 01Ch, 0DFh, 060h, 004h, 0D0h, 0DAh, 0A0h, 092h, 0AAh, 0E3h, 003h, 013h
        db  0A0h, 092h, 0ABh, 0E3h, 01Eh, 016h, 0E0h, 023h, 014h, 0E0h, 008h, 013h, 020h, 098h, 0ABh, 0E3h
        db  065h, 006h, 004h, 013h, 083h, 007h, 007h, 000h, 060h, 004h, 0C8h, 0DAh, 0A0h, 0D2h, 00Eh, 0E0h
        db  020h, 0C8h, 020h, 0E0h, 008h, 007h, 0A0h, 027h, 004h, 0E0h, 00Bh, 016h, 020h, 0C8h, 01Eh, 0E0h
        db  008h, 007h, 0E0h, 093h, 0AAh, 0E3h, 005h, 016h, 0A0h, 023h, 012h, 0E0h, 002h, 013h, 020h, 006h
        db  008h, 007h, 060h, 004h, 0D0h, 0DAh, 0E0h, 023h, 014h, 0E0h, 036h, 013h, 0B0h, 003h, 020h, 098h
        db  00Eh, 0E0h, 06Fh, 006h, 00Fh, 016h, 0CFh, 0D3h, 02Fh, 016h, 0A0h, 0D2h, 0AAh, 0E3h, 060h, 004h
        db  0D0h, 0DAh, 0A0h, 092h, 00Ch, 0E0h, 028h, 016h, 0E0h, 023h, 014h, 0E0h, 0F6h, 013h, 083h, 007h
        db  006h, 000h, 007h, 010h, 083h, 007h, 005h, 000h, 0E0h, 093h, 00Eh, 0E0h, 002h, 016h, 083h, 007h
        db  007h, 000h, 060h, 004h, 0C8h, 0DAh, 060h, 0E3h, 012h, 0E0h, 0E0h, 023h, 014h, 0E0h, 00Dh, 013h
        db  020h, 098h, 00Ch, 0E0h, 065h, 006h, 003h, 016h, 020h, 0D8h, 0ABh, 0E3h, 065h, 006h, 00Ch, 010h
        db  020h, 0C1h, 084h, 001h, 020h, 021h, 006h, 0E0h, 0D6h, 016h, 020h, 048h, 006h, 0E0h, 084h, 001h
        db  008h, 002h, 030h, 080h, 0A0h, 006h, 01Ch, 0DFh, 060h, 004h, 0D0h, 0DAh, 0A0h, 0E3h, 00Ah, 0E0h
        db  008h, 0C2h, 002h, 011h, 0A0h, 043h, 00Ah, 0E0h, 020h, 042h, 004h, 0E0h, 028h, 002h, 0F0h, 0E3h
        db  058h, 0C0h, 002h, 0C0h, 011h, 088h, 018h, 0EDh, 003h, 016h, 0D1h, 02Ch, 058h, 0C0h, 0D1h, 004h
        db  080h, 0C0h, 00Eh, 001h, 000h, 010h, 00Fh, 013h, 060h, 0CCh, 018h, 0EDh, 0C8h, 005h, 078h, 0CCh
        db  003h, 016h, 041h, 006h, 060h, 0CCh, 0D6h, 006h, 058h, 0C4h, 002h, 016h, 060h, 0C4h, 000h, 007h
        db  021h, 002h, 0FAh, 0FFh, 091h, 02Ch, 05Bh, 004h, 00Bh, 0C3h, 0A0h, 006h, 004h, 0E0h, 0A0h, 006h
        db  0DEh, 0DFh, 008h, 0C2h, 005h, 016h, 062h, 0C2h, 002h, 000h, 060h, 026h, 06Ah, 0E4h, 00Dh, 016h
        db  042h, 0C2h, 0C9h, 005h, 060h, 0CEh, 03Ch, 0EDh, 060h, 0C6h, 0FEh, 0EAh, 0A0h, 006h, 046h, 0E0h
        db  018h, 0CAh, 00Ah, 000h, 020h, 046h, 026h, 0E0h, 004h, 016h, 0A0h, 0C0h, 06Ch, 001h, 012h, 02Eh
        db  01Dh, 010h, 012h, 0C1h, 005h, 013h, 060h, 0C1h, 06Ch, 001h, 014h, 02Eh, 005h, 0C8h, 06Ch, 001h
        db  0D2h, 004h, 048h, 006h, 084h, 007h, 002h, 000h, 048h, 0C1h, 0A0h, 0C0h, 06Ch, 001h, 002h, 0C0h
        db  0A0h, 006h, 0C8h, 0E2h, 060h, 0C5h, 002h, 0FCh, 007h, 002h, 0A2h, 006h, 025h, 002h, 0F4h, 0FFh
        db  005h, 0C8h, 002h, 0FCh, 020h, 0C2h, 06Ch, 001h, 0A0h, 006h, 0BCh, 0C1h, 05Ch, 004h, 042h, 0C2h
        db  029h, 002h, 008h, 000h, 039h, 0C2h, 048h, 002h, 000h, 0C0h, 088h, 002h, 000h, 0C0h, 008h, 016h
        db  060h, 08Eh, 02Eh, 0E0h, 005h, 016h, 060h, 086h, 02Eh, 0E0h, 002h, 016h, 0C8h, 004h, 05Bh, 004h
        db  008h, 007h, 05Bh, 004h, 020h, 088h, 090h, 0E1h, 06Ch, 001h, 002h, 016h, 060h, 004h, 0B8h, 0DAh
        db  05Bh, 004h, 088h, 007h, 0AEh, 001h, 020h, 0E8h, 00Eh, 0E0h, 080h, 001h, 008h, 006h, 0FEh, 016h
        db  020h, 048h, 00Eh, 0E0h, 080h, 001h, 05Bh, 004h, 0C2h, 004h, 0A0h, 023h, 00Ch, 0E0h, 00Ah, 016h
        db  020h, 02Fh, 030h, 006h, 082h, 007h, 0DFh, 0FFh, 002h, 02Ch, 082h, 002h, 0F4h, 003h, 0FAh, 016h
        db  0A0h, 043h, 00Ch, 0E0h, 05Bh, 004h, 042h, 0C2h, 088h, 007h, 00Eh, 000h, 009h, 0A2h, 029h, 002h
        db  008h, 000h, 078h, 0CEh, 078h, 0CEh, 078h, 0CEh, 060h, 0CEh, 01Ah, 009h, 060h, 0CEh, 01Ch, 009h
        db  060h, 0CEh, 01Eh, 009h, 0A0h, 023h, 01Ah, 0E0h, 00Fh, 016h, 058h, 0C2h, 049h, 002h, 080h, 01Fh
        db  060h, 02Ah, 014h, 0E0h, 0A0h, 0E8h, 004h, 0E0h, 00Eh, 000h, 009h, 0C6h, 049h, 002h, 000h, 01Fh
        db  0C9h, 006h, 009h, 0A2h, 089h, 0A8h, 004h, 000h, 028h, 002h, 002h, 000h, 058h, 0C2h, 049h, 00Ah
        db  049h, 002h, 000h, 0F0h, 009h, 0D6h, 0E2h, 004h, 006h, 000h, 05Bh, 004h, 000h, 007h, 082h, 0C0h
        db  053h, 013h, 0A0h, 0C0h, 06Ch, 001h, 0A0h, 0C1h, 006h, 0FCh, 046h, 002h, 00Fh, 000h, 086h, 002h
        db  001h, 000h, 03Dh, 012h, 006h, 088h, 0F2h, 006h, 012h, 016h, 001h, 002h, 00Eh, 0FCh, 031h, 088h
        db  0F4h, 006h, 00Dh, 016h, 031h, 088h, 0F6h, 006h, 00Ah, 016h, 031h, 088h, 0F8h, 006h, 007h, 016h
        db  086h, 002h, 002h, 000h, 02Ch, 016h, 020h, 088h, 00Ah, 007h, 0FAh, 006h, 028h, 013h, 020h, 0C1h
        db  018h, 009h, 044h, 001h, 000h, 008h, 006h, 013h, 086h, 002h, 002h, 000h, 020h, 016h, 044h, 001h
        db  080h, 000h, 01Dh, 016h, 000h, 007h, 0E0h, 023h, 014h, 0E0h, 019h, 016h, 082h, 002h, 043h, 000h
        db  016h, 013h, 000h, 002h, 002h, 0FCh, 040h, 0C0h, 0B0h, 001h, 020h, 000h, 060h, 001h, 018h, 009h
        db  001h, 000h, 007h, 016h, 060h, 0A0h, 0DEh, 008h, 060h, 0CCh, 0EEh, 005h, 050h, 0C4h, 020h, 0C4h
        db  0DEh, 008h, 080h, 007h, 036h, 007h, 081h, 007h, 040h, 000h, 040h, 02Ch, 0C0h, 004h, 084h, 007h
        db  0F2h, 006h, 006h, 0CDh, 001h, 002h, 00Eh, 0FCh, 031h, 0CDh, 031h, 0CDh, 031h, 0CDh, 020h, 0C5h
        db  00Ah, 007h, 000h, 0C0h, 001h, 013h, 012h, 02Eh, 0E0h, 004h, 06Ch, 001h, 05Bh, 004h, 00Bh, 0C8h
        db  0D4h, 008h, 0A0h, 006h, 06Ch, 0E1h, 008h, 002h, 042h, 080h, 0A0h, 006h, 01Ch, 0DFh, 008h, 002h
        db  030h, 080h, 0A0h, 006h, 01Ch, 0DFh, 0E0h, 0C2h, 0D4h, 008h, 05Bh, 004h, 00Bh, 0C3h, 020h, 048h
        db  000h, 0EBh, 082h, 001h, 020h, 048h, 06Eh, 0E4h, 080h, 001h, 020h, 048h, 022h, 0E0h, 0AEh, 001h
        db  020h, 048h, 022h, 0E0h, 026h, 009h, 060h, 043h, 018h, 0E0h, 0A0h, 043h, 008h, 0E0h, 008h, 002h
        db  042h, 000h, 0A0h, 006h, 01Ch, 0DFh, 05Ch, 004h, 00Bh, 0C3h, 020h, 0E8h, 00Eh, 0E0h, 082h, 001h
        db  020h, 0E8h, 022h, 0E0h, 0AEh, 001h, 020h, 0E8h, 022h, 0E0h, 026h, 009h, 0A0h, 0E3h, 008h, 0E0h
        db  060h, 0E3h, 018h, 0E0h, 0A0h, 043h, 006h, 0E0h, 008h, 002h, 03Ch, 080h, 0A0h, 006h, 01Ch, 0DFh
        db  008h, 002h, 042h, 080h, 0A0h, 006h, 01Ch, 0DFh, 05Ch, 004h, 00Bh, 0C3h, 083h, 007h, 000h, 068h
        db  0A0h, 006h, 050h, 0E2h, 083h, 007h, 010h, 080h, 0A0h, 006h, 050h, 0E2h, 05Ch, 004h, 00Bh, 0C3h
        db  0A0h, 006h, 03Ah, 0E2h, 002h, 0A2h, 068h, 0C2h, 014h, 000h, 029h, 002h, 0FCh, 0FFh, 024h, 013h
        db  028h, 002h, 018h, 000h, 087h, 007h, 00Eh, 000h, 081h, 007h, 00Eh, 007h, 0F1h, 004h, 047h, 006h
        db  0FDh, 015h, 058h, 0C0h, 0B0h, 003h, 001h, 078h, 063h, 006h, 041h, 002h, 03Fh, 000h, 00Eh, 013h
        db  081h, 002h, 01Fh, 000h, 00Bh, 01Bh, 041h, 0A0h, 061h, 0C0h, 050h, 0E4h, 0F8h, 0C1h, 0C7h, 006h
        db  0C7h, 071h, 047h, 006h, 078h, 0CCh, 047h, 006h, 0FDh, 015h, 004h, 010h, 058h, 0C0h, 0C1h, 006h
        db  041h, 070h, 001h, 0A2h, 049h, 0C2h, 0E5h, 015h, 05Ch, 004h, 0A0h, 023h, 01Ah, 0E0h, 002h, 013h
        db  0C8h, 004h, 05Bh, 004h, 022h, 0C2h, 014h, 000h, 048h, 002h, 000h, 01Fh, 0C8h, 006h, 05Bh, 004h
        db  083h, 002h, 00Fh, 000h, 017h, 01Bh, 0A0h, 0C1h, 0D4h, 006h, 035h, 013h, 026h, 002h, 004h, 000h
        db  0A0h, 0CDh, 018h, 0EDh, 083h, 0C5h, 004h, 013h, 04Ah, 0C2h, 039h, 00Ah, 0C9h, 0E0h, 083h, 0C5h
        db  086h, 007h, 036h, 007h, 087h, 007h, 010h, 000h, 020h, 0C2h, 0D4h, 006h, 0E0h, 004h, 0D4h, 006h
        db  046h, 02Ch, 05Bh, 004h, 060h, 0C0h, 0FEh, 006h, 020h, 0C2h, 018h, 009h, 048h, 002h, 000h, 060h
        db  020h, 022h, 006h, 0E0h, 004h, 016h, 020h, 0E2h, 00Ah, 0E0h, 020h, 0E2h, 018h, 0E0h, 013h, 00Ah
        db  004h, 018h, 041h, 005h, 003h, 048h, 0FEh, 006h, 006h, 010h, 083h, 002h, 002h, 000h, 001h, 016h
        db  013h, 009h, 003h, 0E8h, 0FEh, 006h, 0C8h, 040h, 0C1h, 040h, 005h, 013h, 088h, 007h, 036h, 007h
        db  089h, 007h, 000h, 040h, 048h, 02Ch, 05Bh, 004h, 0C9h, 004h, 024h, 0C1h, 016h, 0EBh, 084h, 0C1h
        db  086h, 071h, 026h, 002h, 0C8h, 0EBh, 0C4h, 006h, 004h, 071h, 024h, 002h, 03Eh, 0EBh, 014h, 0D2h
        db  0C8h, 009h, 008h, 0A2h, 0B0h, 003h, 034h, 0D8h, 05Fh, 006h, 047h, 002h, 00Fh, 000h, 0C7h, 0A1h
        db  028h, 0C2h, 004h, 0EBh, 058h, 004h, 076h, 0CDh, 047h, 006h, 0FDh, 016h, 032h, 010h, 036h, 0C2h
        db  026h, 010h, 017h, 009h, 047h, 0A1h, 02Dh, 010h, 017h, 009h, 047h, 061h, 02Ah, 010h, 0A0h, 043h
        db  016h, 0E0h, 05Bh, 004h, 0A0h, 043h, 016h, 0E0h, 049h, 0C2h, 003h, 016h, 044h, 0C2h, 006h, 0C8h
        db  0D4h, 008h, 027h, 0C1h, 0D8h, 0ECh, 084h, 0C1h, 086h, 071h, 026h, 002h, 00Eh, 0EDh, 0C4h, 006h
        db  004h, 071h, 024h, 002h, 0F4h, 0ECh, 0D3h, 010h, 009h, 0C1h, 0A0h, 0C1h, 0D4h, 008h, 0C9h, 004h
        db  010h, 010h, 036h, 0C2h, 078h, 0D5h, 060h, 041h, 022h, 0E0h, 0C5h, 005h, 00Ah, 010h, 078h, 0CDh
        db  047h, 006h, 0FDh, 015h, 006h, 010h, 0A0h, 023h, 016h, 0E0h, 0CDh, 016h, 049h, 0C2h, 0ECh, 016h
        db  0D6h, 010h, 0A0h, 0E3h, 016h, 0E0h, 0BBh, 010h, 008h, 002h, 05Ah, 080h, 0A0h, 006h, 01Ch, 0DFh
        db  03Eh, 010h, 0A0h, 092h, 00Ch, 0E0h, 015h, 016h, 044h, 002h, 000h, 05Eh, 014h, 016h, 020h, 048h
        db  06Eh, 0E4h, 080h, 001h, 0A0h, 006h, 098h, 0E1h, 020h, 0C8h, 09Eh, 001h, 09Eh, 001h, 0E0h, 02Eh
        db  001h, 000h, 0A0h, 043h, 018h, 0E0h, 0A0h, 0D2h, 026h, 0E0h, 083h, 007h, 010h, 000h, 0A0h, 006h
        db  050h, 0E2h, 060h, 004h, 0D0h, 0DAh, 084h, 007h, 008h, 000h, 060h, 004h, 098h, 0DAh, 085h, 007h
        db  003h, 002h, 005h, 0C8h, 0CEh, 006h, 0A0h, 043h, 012h, 0E0h, 0E0h, 004h, 0FAh, 006h, 0A0h, 006h
        db  0CAh, 0E1h, 008h, 002h, 048h, 080h, 0A0h, 006h, 01Ch, 0DFh, 011h, 010h, 0A0h, 027h, 02Ch, 0E0h
        db  004h, 016h, 008h, 002h, 054h, 080h, 0A0h, 006h, 01Ch, 0DFh, 083h, 007h, 000h, 0A8h, 020h, 088h
        db  008h, 007h, 020h, 0E0h, 002h, 016h, 083h, 007h, 000h, 0E8h, 0A0h, 006h, 050h, 0E2h, 008h, 002h
        db  036h, 000h, 0A0h, 006h, 01Ch, 0DFh, 020h, 0E8h, 00Ch, 0E0h, 082h, 001h, 0A0h, 023h, 018h, 0E0h
        db  006h, 013h, 0A0h, 0E3h, 018h, 0E0h, 0E0h, 02Eh, 000h, 000h, 041h, 0C0h, 0FAh, 016h, 0A0h, 006h
        db  028h, 0E0h, 082h, 0C0h, 002h, 013h, 04Fh, 002h, 080h, 0FFh, 0C4h, 004h, 00Fh, 0D1h, 0C4h, 006h
        db  060h, 004h, 098h, 0DAh, 0A0h, 006h, 04Ah, 0E4h, 008h, 002h, 036h, 080h, 0A0h, 007h, 0D6h, 006h
        db  038h, 0E4h, 0A0h, 006h, 01Ch, 0DFh, 010h, 010h, 0A0h, 006h, 04Ah, 0E4h, 020h, 0D1h, 0CEh, 006h
        db  0E6h, 013h, 020h, 078h, 012h, 0E0h, 0CEh, 006h, 0E2h, 010h, 020h, 0C1h, 016h, 004h, 014h, 00Ah
        db  0C4h, 006h, 00Ah, 091h, 001h, 016h, 05Bh, 004h, 060h, 004h, 0D0h, 0DAh, 0CAh, 004h, 0CDh, 004h
        db  0CEh, 004h, 0CFh, 004h, 0E0h, 004h, 0A8h, 006h, 0E0h, 004h, 0BAh, 006h, 084h, 007h, 0A0h, 001h
        db  085h, 007h, 010h, 000h, 0F4h, 004h, 045h, 006h, 0FDh, 015h, 084h, 007h, 0D8h, 006h, 085h, 007h
        db  034h, 007h, 044h, 061h, 0F4h, 004h, 045h, 006h, 0FDh, 015h, 084h, 007h, 0C8h, 000h, 004h, 0C8h
        db  000h, 007h, 084h, 007h, 0FFh, 07Fh, 004h, 0C8h, 0F0h, 006h, 084h, 007h, 006h, 000h, 004h, 0C8h
        db  0EEh, 006h, 0B0h, 003h, 020h, 0D8h, 010h, 0E0h, 065h, 006h, 060h, 004h, 0D0h, 0DAh, 020h, 0F8h
        db  063h, 0EDh, 082h, 001h, 020h, 0C8h, 010h, 0E0h, 0C6h, 006h, 020h, 0C8h, 020h, 0E0h, 0C8h, 006h
        db  0E0h, 02Eh, 000h, 000h, 0A0h, 006h, 028h, 0E0h, 020h, 0C8h, 01Ah, 009h, 0A0h, 001h, 020h, 0C8h
        db  01Ch, 009h, 0A2h, 001h, 020h, 0C8h, 01Eh, 009h, 0A4h, 001h, 020h, 0C8h, 01Ch, 009h, 0B0h, 001h
        db  020h, 0C8h, 01Eh, 009h, 0B2h, 001h, 020h, 0C8h, 01Eh, 009h, 0CCh, 006h, 020h, 0F8h, 062h, 0EDh
        db  080h, 001h, 0A0h, 006h, 012h, 0E0h, 020h, 048h, 004h, 0E0h, 080h, 001h, 0E0h, 004h, 09Eh, 001h
        db  008h, 002h, 012h, 080h, 0A0h, 006h, 01Ch, 0DFh, 084h, 007h, 00Ah, 000h, 085h, 007h, 0F4h, 003h
        db  020h, 088h, 0C6h, 006h, 020h, 0E0h, 004h, 01Bh, 084h, 007h, 01Ch, 000h, 085h, 007h, 0F8h, 003h
        db  0A0h, 006h, 0C8h, 0E2h, 085h, 007h, 03Ch, 0E5h, 005h, 0C8h, 0F8h, 003h, 0A0h, 023h, 00Ch, 0E0h
        db  023h, 013h, 0A0h, 0E3h, 00Ch, 0E0h, 0A0h, 02Eh, 0F4h, 003h, 01Eh, 010h, 0B0h, 003h, 020h, 098h
        db  010h, 0E0h, 065h, 006h, 019h, 016h, 044h, 002h, 000h, 05Eh, 004h, 016h, 020h, 006h, 0C6h, 006h
        db  0D7h, 016h, 008h, 010h, 0B0h, 003h, 020h, 098h, 010h, 0E0h, 065h, 006h, 00Dh, 016h, 020h, 006h
        db  0C8h, 006h, 0C4h, 016h, 00Eh, 001h, 003h, 000h, 003h, 013h, 083h, 007h, 000h, 082h, 002h, 010h
        db  083h, 007h, 001h, 000h, 060h, 004h, 0C8h, 0DAh, 060h, 004h, 0AEh, 0DAh, 00Eh, 001h, 003h, 000h
        db  00Ah, 013h, 008h, 002h, 00Ch, 080h, 0A0h, 006h, 01Ch, 0DFh, 0E0h, 0E3h, 014h, 0E0h, 020h, 0C8h
        db  070h, 0E4h, 086h, 001h, 026h, 010h, 020h, 048h, 00Ch, 0E0h, 082h, 001h, 0E0h, 02Eh, 001h, 000h
        db  060h, 0C1h, 0D0h, 008h, 035h, 00Ah, 005h, 0E8h, 082h, 001h, 020h, 0C1h, 018h, 009h, 004h, 001h
        db  006h, 000h, 006h, 013h, 020h, 0D8h, 0D2h, 0E1h, 0E1h, 008h, 020h, 0D8h, 0D2h, 0E1h, 083h, 001h
        db  020h, 021h, 022h, 0E0h, 003h, 016h, 020h, 0E8h, 022h, 0E0h, 080h, 001h, 020h, 021h, 004h, 0E0h
        db  004h, 016h, 0A0h, 0E3h, 014h, 0E0h, 060h, 004h, 0BAh, 0DEh, 008h, 002h, 000h, 080h, 0A0h, 006h
        db  01Ch, 0DFh, 020h, 0E8h, 008h, 0E0h, 082h, 001h, 060h, 004h, 0AEh, 0DAh, 020h, 0C8h, 070h, 0E4h
        db  086h, 001h, 008h, 002h, 000h, 080h, 0A0h, 006h, 01Ch, 0DFh, 020h, 0C2h, 0D0h, 008h, 008h, 0A2h
        db  008h, 005h, 028h, 0C8h, 022h, 0E0h, 0CAh, 006h, 020h, 0C8h, 020h, 0E0h, 0C6h, 006h, 020h, 0C8h
        db  020h, 0E0h, 0C8h, 006h, 060h, 0E3h, 016h, 0E0h, 060h, 004h, 0D0h, 0DAh, 044h, 0C1h, 044h, 002h
        db  000h, 05Eh, 0F8h, 016h, 060h, 025h, 06Ah, 0E4h, 00Fh, 016h, 020h, 006h, 0C6h, 006h, 0F2h, 016h
        db  020h, 006h, 0CAh, 006h, 003h, 013h, 0A0h, 005h, 0CCh, 006h, 0E6h, 010h, 0B0h, 003h, 020h, 0D8h
        db  00Ch, 0E0h, 065h, 006h, 060h, 004h, 0D0h, 0DAh, 020h, 006h, 0C8h, 006h, 0E3h, 016h, 020h, 088h
        db  01Eh, 009h, 0CCh, 006h, 003h, 016h, 083h, 007h, 008h, 000h, 002h, 010h, 083h, 007h, 00Ch, 000h
        db  060h, 004h, 0C8h, 0DAh, 060h, 004h, 0D0h, 0DAh, 0A0h, 023h, 008h, 0E0h, 003h, 013h, 060h, 023h
        db  012h, 0E0h, 006h, 016h, 0B0h, 003h, 020h, 0D8h, 0ABh, 0E3h, 065h, 006h, 060h, 004h, 0D0h, 0DAh
        db  008h, 002h, 000h, 080h, 0A0h, 006h, 01Ch, 0DFh, 060h, 004h, 0AEh, 0DAh, 008h, 002h, 000h, 080h
        db  0A0h, 006h, 01Ch, 0DFh, 020h, 0C8h, 01Eh, 0E0h, 0C6h, 006h, 020h, 0C8h, 01Eh, 0E0h, 0C8h, 006h
        db  060h, 0E3h, 010h, 0E0h, 060h, 004h, 0D0h, 0DAh, 0E0h, 023h, 014h, 0E0h, 019h, 013h, 044h, 0C1h
        db  044h, 002h, 000h, 01Eh, 0F5h, 016h, 060h, 025h, 06Ah, 0E4h, 006h, 016h, 020h, 006h, 0C8h, 006h
        db  0EFh, 016h, 0C2h, 004h, 060h, 004h, 01Ah, 0EBh, 008h, 002h, 01Eh, 080h, 0A0h, 006h, 01Ch, 0DFh
        db  007h, 010h, 020h, 006h, 0C6h, 006h, 0E4h, 016h, 083h, 007h, 009h, 000h, 0A0h, 006h, 0C8h, 0DAh
        db  060h, 004h, 0AEh, 0DAh, 0CEh, 004h, 0E0h, 004h, 0DCh, 008h, 0E0h, 0D3h, 0ACh, 0E3h, 08Fh, 0C2h
        db  020h, 0C8h, 072h, 0E4h, 086h, 001h, 020h, 048h, 008h, 0E0h, 082h, 001h, 086h, 007h, 005h, 000h
        db  084h, 007h, 072h, 006h, 054h, 0C1h, 001h, 013h, 0D4h, 02Ch, 024h, 002h, 00Ah, 000h, 006h, 006h
        db  0F9h, 016h, 008h, 002h, 02Ah, 080h, 0A0h, 006h, 01Ch, 0DFh, 020h, 02Ch, 01Ah, 0E0h, 060h, 004h
        db  07Ah, 0D9h, 0A0h, 006h, 06Ch, 0E1h, 0CDh, 004h, 0A0h, 023h, 01Ch, 0E0h, 00Dh, 013h, 00Eh, 001h
        db  003h, 000h, 00Ah, 013h, 0A0h, 0E3h, 01Ch, 0E0h, 0B0h, 003h, 020h, 0D8h, 010h, 0E0h, 065h, 006h
        db  0A0h, 0D2h, 026h, 0E0h, 0CFh, 004h, 008h, 010h, 020h, 02Dh, 001h, 000h, 0E0h, 0C0h, 0DCh, 008h
        db  0A0h, 006h, 050h, 0E2h, 0A0h, 0D2h, 0ADh, 0E3h, 060h, 004h, 0D0h, 0DAh, 0A0h, 006h, 0DEh, 0DFh
        db  008h, 0C2h, 019h, 013h, 083h, 007h, 080h, 080h, 0E0h, 023h, 014h, 0E0h, 002h, 013h, 083h, 007h
        db  00Ah, 000h, 060h, 004h, 0C4h, 0DAh, 020h, 0C1h, 006h, 006h, 00Dh, 013h, 0A0h, 006h, 0DEh, 0DFh
        db  008h, 0C2h, 009h, 013h, 083h, 007h, 00Bh, 000h, 0E0h, 023h, 014h, 0E0h, 002h, 016h, 083h, 007h
        db  001h, 080h, 060h, 004h, 0C4h, 0DAh, 083h, 007h, 00Ah, 080h, 060h, 004h, 0B2h, 0DAh, 0E0h, 093h
        db  00Eh, 0E0h, 05Eh, 013h, 0E0h, 093h, 010h, 0E0h, 017h, 013h, 0E0h, 023h, 014h, 0E0h, 004h, 013h
        db  083h, 007h, 007h, 000h, 060h, 004h, 0C4h, 0DAh, 083h, 007h, 000h, 0A0h, 0A0h, 006h, 050h, 0E2h
        db  083h, 007h, 000h, 048h, 0A0h, 006h, 050h, 0E2h, 0A0h, 0D2h, 010h, 0E0h, 020h, 0C8h, 01Ch, 0E0h
        db  0CAh, 006h, 020h, 0C8h, 020h, 0E0h, 0CCh, 006h, 0A0h, 006h, 06Ch, 0E1h, 008h, 002h, 04Eh, 080h
        db  0A0h, 006h, 01Ch, 0DFh, 0A0h, 023h, 01Ch, 0E0h, 020h, 013h, 020h, 088h, 01Ah, 009h, 00Eh, 007h
        db  01Ch, 016h, 020h, 088h, 01Ch, 009h, 010h, 007h, 018h, 016h, 020h, 088h, 01Eh, 009h, 012h, 007h
        db  014h, 016h, 020h, 088h, 00Ah, 007h, 022h, 0E0h, 010h, 013h, 020h, 006h, 0CAh, 006h, 038h, 016h
        db  0A0h, 0E3h, 020h, 0E0h, 006h, 010h, 0E0h, 023h, 014h, 0E0h, 0CAh, 016h, 0A0h, 0E3h, 022h, 0E0h
        db  0C2h, 004h, 0A0h, 0D2h, 0ACh, 0E3h, 060h, 004h, 0B8h, 0DAh, 020h, 0C8h, 01Ch, 0E0h, 0CAh, 006h
        db  0A0h, 088h, 0DCh, 006h, 00Eh, 000h, 010h, 016h, 0A0h, 088h, 0DEh, 006h, 010h, 000h, 00Ch, 016h
        db  0A0h, 088h, 0E0h, 006h, 012h, 000h, 008h, 016h, 020h, 006h, 0CCh, 006h, 019h, 016h, 020h, 0E8h
        db  00Eh, 0E0h, 082h, 001h, 0A0h, 0E3h, 01Eh, 0E0h, 020h, 0C8h, 020h, 0E0h, 0CCh, 006h, 010h, 010h
        db  0A0h, 023h, 010h, 0E0h, 008h, 016h, 064h, 0C1h, 006h, 000h, 060h, 021h, 00Ch, 0E0h, 008h, 013h
        db  0A0h, 0D2h, 0AAh, 0E3h, 005h, 010h, 020h, 088h, 00Ah, 007h, 008h, 007h, 096h, 012h, 000h, 010h
        db  060h, 004h, 0B8h, 0DAh, 0E0h, 093h, 00Eh, 0E0h, 050h, 013h, 0E0h, 093h, 0ABh, 0E3h, 04Dh, 013h
        db  0E0h, 093h, 0AAh, 0E3h, 01Ch, 013h, 0A0h, 006h, 0CAh, 0E1h, 0A0h, 023h, 010h, 0E0h, 045h, 013h
        db  0A0h, 023h, 008h, 0E0h, 006h, 016h, 060h, 0E3h, 01Eh, 0E0h, 020h, 0C8h, 022h, 0E0h, 006h, 007h
        db  034h, 010h, 0E0h, 023h, 014h, 0E0h, 031h, 016h, 060h, 0C1h, 018h, 009h, 060h, 021h, 012h, 0E0h
        db  02Ch, 016h, 0A0h, 006h, 02Eh, 0E9h, 031h, 010h, 0A0h, 0D2h, 0AAh, 0E3h, 02Eh, 010h, 0A0h, 0E3h
        db  012h, 0E0h, 0A0h, 006h, 02Eh, 0E9h, 064h, 0C1h, 006h, 000h, 060h, 021h, 00Ch, 0E0h, 025h, 013h
        db  020h, 088h, 00Eh, 007h, 0DCh, 006h, 014h, 016h, 020h, 088h, 010h, 007h, 0DEh, 006h, 010h, 016h
        db  020h, 088h, 012h, 007h, 0E0h, 006h, 00Ch, 016h, 020h, 098h, 0CEh, 006h, 0CFh, 006h, 015h, 013h
        db  020h, 006h, 0CEh, 006h, 012h, 016h, 060h, 0E3h, 01Ah, 0E0h, 0A0h, 0D2h, 00Ch, 0E0h, 00Dh, 010h
        db  060h, 0E3h, 01Eh, 0E0h, 020h, 0C8h, 032h, 0E0h, 006h, 007h, 0A0h, 006h, 06Ch, 0E1h, 008h, 002h
        db  048h, 080h, 0A0h, 006h, 01Ch, 0DFh, 0A0h, 0D2h, 0ABh, 0E3h, 060h, 004h, 0B8h, 0DAh, 022h, 088h
        db  00Eh, 000h, 01Ah, 009h, 0C9h, 01Ah, 00Bh, 01Bh, 022h, 088h, 010h, 000h, 01Ch, 009h, 0C4h, 01Ah
        db  006h, 01Bh, 022h, 088h, 012h, 000h, 01Eh, 009h, 0BFh, 01Ah, 001h, 01Bh, 05Bh, 004h, 060h, 0C1h
        db  06Ch, 001h, 085h, 002h, 043h, 000h, 0E1h, 013h, 0E0h, 093h, 0AAh, 0E3h, 0DEh, 016h, 0A0h, 0C8h
        db  04Ah, 0EDh, 002h, 000h, 084h, 007h, 00Eh, 000h, 042h, 0C1h, 0A0h, 0C0h, 06Ch, 001h, 002h, 0C0h
        db  025h, 002h, 048h, 000h, 081h, 007h, 080h, 0E9h, 083h, 007h, 014h, 0AEh, 060h, 004h, 08Ch, 0ECh
        db  002h, 002h, 000h, 0FCh, 0CAh, 010h, 020h, 098h, 00Eh, 0E0h, 065h, 006h, 003h, 016h, 020h, 0D8h
        db  0AAh, 0E3h, 065h, 006h, 0E0h, 093h, 0ABh, 0E3h, 00Dh, 013h, 0A0h, 023h, 008h, 0E0h, 019h, 016h
        db  0A0h, 023h, 010h, 0E0h, 016h, 013h, 060h, 0E3h, 01Eh, 0E0h, 020h, 0C8h, 020h, 0E0h, 006h, 007h
        db  0A0h, 006h, 06Ch, 0E1h, 0A0h, 043h, 018h, 0E0h, 0E0h, 02Eh, 001h, 000h, 0A0h, 0D2h, 026h, 0E0h
        db  083h, 007h, 010h, 000h, 0A0h, 006h, 050h, 0E2h, 0E0h, 023h, 014h, 0E0h, 002h, 016h, 0A0h, 006h
        db  04Eh, 0E1h, 0A0h, 043h, 02Ch, 0E0h, 020h, 0C8h, 020h, 0E0h, 0D6h, 008h, 060h, 004h, 0B8h, 0DAh
        db  0A0h, 043h, 00Eh, 0E0h, 0A0h, 0C1h, 0D6h, 008h, 002h, 013h, 020h, 006h, 0D6h, 008h, 0E0h, 023h
        db  014h, 0E0h, 003h, 013h, 0A0h, 023h, 008h, 0E0h, 026h, 016h, 008h, 002h, 042h, 080h, 0A0h, 006h
        db  01Ch, 0DFh, 0A0h, 023h, 008h, 0E0h, 01Fh, 016h, 0A0h, 023h, 010h, 0E0h, 00Ah, 016h, 022h, 0C1h
        db  002h, 000h, 020h, 025h, 06Ah, 0E4h, 023h, 016h, 083h, 007h, 020h, 080h, 0A0h, 006h, 050h, 0E2h
        db  012h, 010h, 0A0h, 006h, 06Ch, 0E1h, 0E0h, 023h, 014h, 0E0h, 002h, 016h, 0A0h, 006h, 04Eh, 0E1h
        db  060h, 0E3h, 01Eh, 0E0h, 020h, 0C8h, 020h, 0E0h, 006h, 007h, 0A0h, 023h, 008h, 0E0h, 003h, 016h
        db  0A0h, 023h, 006h, 0E0h, 051h, 013h, 020h, 098h, 00Eh, 0E0h, 065h, 006h, 003h, 016h, 020h, 0D8h
        db  0AAh, 0E3h, 065h, 006h, 022h, 0C1h, 002h, 000h, 020h, 025h, 06Ah, 0E4h, 00Eh, 013h, 083h, 007h
        db  020h, 000h, 0A0h, 006h, 050h, 0E2h, 022h, 0C8h, 00Eh, 000h, 0E6h, 006h, 022h, 0C8h, 010h, 000h
        db  0E8h, 006h, 022h, 0C8h, 012h, 000h, 0EAh, 006h, 037h, 010h, 022h, 088h, 00Eh, 000h, 0DCh, 006h
        db  008h, 016h, 022h, 088h, 010h, 000h, 0DEh, 006h, 004h, 016h, 022h, 088h, 012h, 000h, 0E0h, 006h
        db  00Bh, 013h, 022h, 0C8h, 00Eh, 000h, 0DCh, 006h, 022h, 0C8h, 010h, 000h, 0DEh, 006h, 022h, 0C8h
        db  012h, 000h, 0E0h, 006h, 060h, 0E3h, 014h, 0E0h, 0A0h, 023h, 00Eh, 0E0h, 008h, 016h, 0A0h, 0C1h
        db  0D6h, 008h, 01Ah, 016h, 086h, 007h, 000h, 010h, 006h, 0E8h, 0D2h, 006h, 015h, 010h, 0A0h, 0E3h
        db  00Eh, 0E0h, 0A0h, 023h, 008h, 0E0h, 009h, 016h, 0A0h, 0E3h, 006h, 0E0h, 0E0h, 004h, 0E6h, 006h
        db  0E0h, 004h, 0E8h, 006h, 0E0h, 004h, 0EAh, 006h, 007h, 010h, 008h, 002h, 036h, 080h, 0A0h, 007h
        db  0D6h, 006h, 0E6h, 0DEh, 0A0h, 006h, 01Ch, 0DFh, 060h, 004h, 0B8h, 0DAh, 020h, 098h, 065h, 006h
        db  010h, 0E0h, 003h, 016h, 020h, 0D8h, 00Eh, 0E0h, 065h, 006h, 060h, 004h, 0B8h, 0DAh, 0E0h, 023h
        db  014h, 0E0h, 002h, 013h, 060h, 004h, 0B8h, 0DAh, 02Eh, 010h, 0B0h, 003h, 020h, 098h, 0ABh, 0E3h
        db  06Fh, 006h, 019h, 016h, 024h, 0C2h, 008h, 000h, 016h, 011h, 0E0h, 0E3h, 014h, 0E0h, 083h, 007h
        db  000h, 000h, 0A0h, 006h, 050h, 0E2h, 0A0h, 023h, 014h, 0E0h, 004h, 013h, 008h, 002h, 006h, 080h
        db  0A0h, 006h, 01Ch, 0DFh, 008h, 002h, 01Eh, 000h, 0A0h, 006h, 01Ch, 0DFh, 0A0h, 023h, 008h, 0E0h
        db  002h, 013h, 0A0h, 006h, 04Eh, 0E1h, 082h, 0C0h, 002h, 016h, 060h, 004h, 0D0h, 0DAh, 020h, 0E8h
        db  066h, 0EDh, 0F0h, 006h, 020h, 099h, 00Eh, 0E0h, 016h, 000h, 005h, 016h, 0E0h, 004h, 0ECh, 006h
        db  020h, 048h, 014h, 0E0h, 0F0h, 006h, 083h, 007h, 001h, 000h, 060h, 004h, 0B2h, 0DAh, 064h, 0C2h
        db  014h, 000h, 024h, 002h, 018h, 000h, 0C4h, 0C1h, 0C2h, 061h, 027h, 002h, 0FCh, 0FFh, 074h, 0C1h
        db  085h, 0C1h, 045h, 071h, 085h, 002h, 027h, 000h, 046h, 016h, 054h, 0C1h, 045h, 002h, 0CFh, 0FFh
        db  042h, 016h, 0C8h, 004h, 064h, 0C1h, 008h, 000h, 006h, 015h, 005h, 013h, 024h, 0C2h, 00Eh, 000h
        db  048h, 002h, 000h, 01Fh, 0C8h, 006h, 028h, 002h, 011h, 000h, 004h, 0A2h, 018h, 098h, 06Bh, 0EDh
        db  032h, 016h, 042h, 0C1h, 025h, 002h, 004h, 000h, 047h, 065h, 035h, 0C2h, 074h, 0CDh, 048h, 006h
        db  0FDh, 015h, 0A0h, 0C0h, 06Ch, 001h, 002h, 0C1h, 004h, 0C8h, 06Ch, 001h, 0A0h, 0C1h, 000h, 0FCh
        db  005h, 013h, 020h, 0C8h, 002h, 0EBh, 002h, 0FCh, 006h, 0C1h, 0F6h, 010h, 002h, 0C8h, 06Ch, 001h
        db  020h, 0C8h, 04Ah, 0EDh, 002h, 0FCh, 081h, 007h, 0F6h, 0EBh, 004h, 0C0h, 083h, 007h, 010h, 002h
        db  084h, 007h, 00Eh, 000h, 03Bh, 010h, 084h, 007h, 00Ch, 000h, 0E2h, 0C0h, 008h, 000h, 005h, 002h
        db  000h, 0FCh, 0A0h, 0C0h, 06Ch, 001h, 002h, 0C0h, 0A0h, 0C1h, 000h, 0FCh, 02Fh, 013h, 0E0h, 004h
        db  000h, 0FCh, 016h, 02Eh, 02Bh, 010h, 0A0h, 0C8h, 06Ch, 0EDh, 00Eh, 000h, 0A0h, 0C8h, 06Eh, 0EDh
        db  010h, 000h, 0A0h, 0C8h, 070h, 0EDh, 012h, 000h, 083h, 007h, 006h, 080h, 060h, 004h, 0B2h, 0DAh
        db  060h, 004h, 0D0h, 0DAh, 084h, 007h, 010h, 000h, 085h, 007h, 034h, 000h, 009h, 010h, 084h, 007h
        db  012h, 000h, 085h, 007h, 032h, 000h, 004h, 010h, 084h, 007h, 014h, 000h, 085h, 007h, 038h, 000h
        db  0A0h, 006h, 004h, 0E0h, 085h, 0C8h, 004h, 000h, 0A0h, 006h, 046h, 0E0h, 0A0h, 0C8h, 04Ah, 0EDh
        db  002h, 000h, 0A0h, 0C0h, 06Ch, 001h, 002h, 0C0h, 048h, 006h, 048h, 0C1h, 0A0h, 006h, 0C8h, 0E2h
        db  007h, 002h, 0A2h, 006h, 060h, 0C5h, 002h, 0FCh, 025h, 002h, 0F4h, 0FFh, 005h, 0C8h, 002h, 0FCh
        db  020h, 0C2h, 06Ch, 001h, 0A0h, 006h, 0BCh, 0C1h, 060h, 004h, 0AEh, 0DAh, 0A0h, 006h, 0C8h, 0E2h
        db  007h, 002h, 0B4h, 006h, 0EFh, 010h, 00Bh, 0C3h, 000h, 003h, 002h, 000h, 082h, 007h, 0C0h, 000h
        db  020h, 0C8h, 00Ch, 000h, 0C0h, 000h, 020h, 0C8h, 00Eh, 000h, 0C2h, 000h, 020h, 0C8h, 010h, 000h
        db  0C4h, 000h, 020h, 0C8h, 012h, 000h, 0C6h, 000h, 020h, 0C8h, 014h, 000h, 0C8h, 000h, 020h, 0C8h
        db  016h, 000h, 0CAh, 000h, 020h, 0C8h, 004h, 000h, 0CCh, 000h, 020h, 0C8h, 006h, 000h, 0CEh, 000h
        db  002h, 0C8h, 00Ch, 000h, 0A0h, 007h, 00Eh, 000h, 036h, 0EDh, 002h, 0C8h, 010h, 000h, 0A0h, 007h
        db  012h, 000h, 040h, 0EDh, 002h, 0C8h, 014h, 000h, 0A0h, 007h, 016h, 000h, 070h, 0EDh, 002h, 0C8h
        db  004h, 000h, 0A0h, 007h, 006h, 000h, 086h, 0EDh, 060h, 001h, 01Ch, 001h, 004h, 000h, 009h, 016h
        db  0E0h, 001h, 040h, 001h, 000h, 008h, 0A0h, 001h, 040h, 001h, 000h, 040h, 0E0h, 001h, 040h, 001h
        db  000h, 004h, 0A0h, 006h, 046h, 0F0h, 005h, 002h, 000h, 080h, 005h, 0D8h, 080h, 004h, 0C7h, 004h
        db  000h, 003h, 00Fh, 000h, 088h, 007h, 000h, 010h, 009h, 002h, 000h, 020h, 08Ah, 007h, 09Eh, 0EDh
        db  003h, 002h, 000h, 0E6h, 05Ah, 004h, 000h, 003h, 000h, 000h, 020h, 0D2h, 087h, 001h, 006h, 010h
        db  000h, 003h, 000h, 000h, 020h, 0C2h, 08Ah, 001h, 008h, 002h, 000h, 01Ah, 060h, 0C2h, 0AEh, 000h
        db  048h, 0DAh, 080h, 004h, 089h, 005h, 089h, 002h, 006h, 000h, 007h, 015h, 088h, 007h, 000h, 080h
        db  048h, 0DAh, 080h, 004h, 009h, 0C8h, 0AEh, 000h, 080h, 003h, 0E0h, 002h, 0A0h, 000h, 05Ch, 004h
        db  000h, 003h, 000h, 000h, 060h, 001h, 09Ch, 001h, 020h, 000h, 0E2h, 013h, 020h, 0C2h, 08Ch, 001h
        db  008h, 002h, 000h, 01Ch, 0E3h, 010h, 000h, 003h, 000h, 000h, 060h, 001h, 040h, 001h, 000h, 040h
        db  0ECh, 016h, 0A0h, 001h, 040h, 001h, 000h, 040h, 008h, 002h, 000h, 002h, 0D7h, 010h, 0B3h, 0C0h
        db  092h, 006h, 0FDh, 010h, 0B3h, 0C0h, 048h, 0C0h, 072h, 0CCh, 072h, 0CCh, 032h, 0C1h, 044h, 0CCh
        db  072h, 0DCh, 004h, 006h, 0FDh, 016h, 05Bh, 004h, 048h, 0C0h, 002h, 002h, 092h, 0E9h, 084h, 007h
        db  006h, 000h, 0F6h, 010h, 002h, 002h, 0E0h, 0E5h, 049h, 0C0h, 084h, 007h, 006h, 000h, 0F0h, 010h
        db  0B3h, 0C0h, 032h, 0C1h, 001h, 002h, 001h, 000h, 044h, 0D0h, 0C1h, 006h, 044h, 002h, 0FFh, 000h
        db  0E7h, 010h, 033h, 0C1h, 073h, 0C0h, 044h, 0D1h, 044h, 002h, 0FFh, 000h, 045h, 0DCh, 004h, 006h
        db  0FDh, 016h, 05Ah, 004h, 0A0h, 006h, 0C6h, 0EFh, 033h, 0C8h, 09Eh, 001h, 05Ah, 004h, 0A0h, 006h
        db  0C4h, 0EDh, 089h, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h, 006h, 0C8h, 08Ah, 001h, 088h, 0C1h
        db  0A6h, 009h, 066h, 002h, 040h, 000h, 006h, 0C8h, 08Ch, 001h, 0C2h, 004h, 0C7h, 0C1h, 003h, 016h
        db  002h, 006h, 0FCh, 016h, 04Dh, 010h, 05Ah, 004h, 0A0h, 006h, 010h, 0EFh, 089h, 0C1h, 0A6h, 009h
        db  066h, 002h, 040h, 000h, 006h, 0C8h, 08Ah, 001h, 088h, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h
        db  006h, 0C8h, 08Ch, 001h, 033h, 0C8h, 09Eh, 001h, 0E8h, 010h, 033h, 08Ah, 002h, 000h, 038h, 016h
        db  073h, 08Ah, 002h, 000h, 035h, 016h, 05Ah, 004h, 020h, 08Ah, 08Ch, 0E9h, 002h, 000h, 030h, 016h
        db  060h, 08Ah, 090h, 0E9h, 002h, 000h, 02Ch, 016h, 082h, 007h, 036h, 0EAh, 001h, 010h, 0B3h, 0C0h
        db  004h, 002h, 080h, 004h, 052h, 0D1h, 003h, 013h, 032h, 09Dh, 022h, 016h, 0FBh, 010h, 085h, 007h
        db  000h, 080h, 005h, 0D8h, 080h, 004h, 0C7h, 004h, 05Ah, 004h, 020h, 0C8h, 0C0h, 000h, 00Ch, 000h
        db  020h, 0C8h, 0C2h, 000h, 00Eh, 000h, 020h, 0C8h, 0C4h, 000h, 010h, 000h, 020h, 0C8h, 0C6h, 000h
        db  012h, 000h, 020h, 0C8h, 0C8h, 000h, 014h, 000h, 020h, 0C8h, 0CAh, 000h, 016h, 000h, 020h, 0C8h
        db  0CCh, 000h, 004h, 000h, 020h, 0C8h, 0CEh, 000h, 006h, 000h, 000h, 003h, 00Fh, 000h, 0CCh, 005h
        db  05Ch, 004h, 0E0h, 004h, 082h, 001h, 002h, 002h, 0DAh, 0E5h, 032h, 0C8h, 082h, 001h, 032h, 0C8h
        db  080h, 001h, 0A0h, 006h, 0DCh, 0EEh, 012h, 0C8h, 082h, 001h, 0CAh, 0C2h, 084h, 007h, 0D0h, 007h
        db  0E0h, 004h, 084h, 001h, 004h, 006h, 0FCh, 016h, 020h, 0C1h, 084h, 001h, 0E9h, 016h, 004h, 002h
        db  032h, 000h, 085h, 007h, 000h, 080h, 005h, 0D8h, 080h, 004h, 0C7h, 004h, 060h, 0C1h, 086h, 001h
        db  004h, 006h, 0FCh, 016h, 020h, 0C1h, 084h, 001h, 05Bh, 004h, 0B3h, 0C0h, 0B3h, 0C4h, 05Bh, 004h
        db  048h, 0C0h, 0B3h, 0C0h, 073h, 0A0h, 042h, 0C4h, 05Bh, 004h, 033h, 088h, 084h, 001h, 0E6h, 016h
        db  05Ah, 004h, 089h, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h, 006h, 0C8h, 08Ah, 001h, 05Bh, 004h
        db  0C5h, 004h, 0A0h, 007h, 09Ch, 001h, 040h, 000h, 060h, 001h, 09Ch, 001h, 040h, 000h, 003h, 013h
        db  005h, 006h, 0F7h, 016h, 05Ch, 004h, 05Bh, 004h, 0A0h, 006h, 064h, 0EFh, 060h, 0C0h, 040h, 001h
        db  005h, 0C8h, 040h, 001h, 002h, 0C5h, 001h, 0C8h, 040h, 001h, 05Ah, 004h, 0A0h, 006h, 064h, 0EFh
        db  008h, 0A1h, 0F4h, 010h, 0B3h, 0C0h, 033h, 0C1h, 060h, 0C1h, 040h, 001h, 085h, 001h, 000h, 004h
        db  0C5h, 001h, 000h, 010h, 05Bh, 004h, 008h, 0C1h, 009h, 0C2h, 044h, 0C2h, 05Bh, 004h, 005h, 002h
        db  0C8h, 000h, 005h, 006h, 0FEh, 016h, 05Bh, 004h, 033h, 0C1h, 003h, 0C0h, 0C4h, 0C0h, 05Bh, 004h
        db  0C0h, 0C0h, 05Bh, 004h, 0E0h, 094h, 09Eh, 001h, 0C2h, 016h, 0C3h, 005h, 05Bh, 004h, 073h, 0C0h
        db  0A0h, 006h, 0DEh, 0EFh, 02Dh, 002h, 008h, 000h, 085h, 007h, 008h, 000h, 071h, 09Fh, 0B7h, 016h
        db  005h, 006h, 0FCh, 016h, 05Ah, 004h, 002h, 002h, 0E6h, 0E5h, 060h, 004h, 0C8h, 0EDh, 0E9h, 08Ch
        db  004h, 000h, 0ADh, 016h, 05Bh, 004h, 020h, 0C1h, 080h, 001h, 085h, 007h, 0D0h, 007h, 0E0h, 001h
        db  080h, 001h, 000h, 004h, 045h, 006h, 0FEh, 016h, 004h, 0C8h, 080h, 001h, 05Bh, 004h, 033h, 0C1h
        db  048h, 0C3h, 004h, 0C1h, 004h, 013h, 02Dh, 002h, 000h, 004h, 004h, 006h, 0FCh, 016h, 05Bh, 004h
        db  08Dh, 0C3h, 0A0h, 006h, 0DEh, 0EFh, 08Dh, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h, 086h, 0C7h
        db  000h, 000h, 000h, 0F0h, 0DCh, 007h, 05Ah, 004h, 08Dh, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h
        db  006h, 0C8h, 08Ah, 001h, 05Bh, 004h, 08Dh, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h, 006h, 0C8h
        db  08Ch, 001h, 05Bh, 004h, 04Dh, 0C0h, 004h, 002h, 028h, 000h, 085h, 007h, 000h, 055h, 060h, 004h
        db  0ECh, 0EDh, 04Dh, 0C0h, 0B3h, 0C0h, 032h, 0C1h, 060h, 004h, 0B0h, 0EDh, 033h, 0C1h, 060h, 001h
        db  01Ch, 001h, 004h, 000h, 001h, 016h, 05Bh, 004h, 0C4h, 0C0h, 05Bh, 004h, 089h, 007h, 028h, 0E5h
        db  039h, 0C2h, 007h, 013h, 039h, 0C6h, 039h, 086h, 025h, 016h, 039h, 0C6h, 039h, 086h, 022h, 016h
        db  0F7h, 010h, 002h, 002h, 064h, 0F0h, 0C4h, 004h, 0C5h, 004h, 039h, 0C2h, 002h, 013h, 060h, 004h
        db  0A0h, 0F0h, 002h, 002h, 072h, 0F0h, 0C4h, 004h, 039h, 0C2h, 003h, 013h, 079h, 0C1h, 060h, 004h
        db  0A0h, 0F0h, 002h, 002h, 082h, 0F0h, 0C5h, 004h, 039h, 0C2h, 003h, 013h, 039h, 0C1h, 060h, 004h
        db  0A0h, 0F0h, 079h, 0C0h, 0B9h, 0C0h, 081h, 060h, 0C2h, 005h, 012h, 009h, 0F1h, 004h, 002h, 006h
        db  0FDh, 016h, 05Bh, 004h, 05Ch, 004h, 001h, 002h, 0AAh, 0AAh, 001h, 0C6h, 044h, 0E0h, 045h, 040h
        db  058h, 080h, 0F8h, 016h, 001h, 002h, 014h, 000h, 001h, 006h, 0FEh, 016h, 001h, 002h, 055h, 055h
        db  001h, 0C6h, 044h, 0E0h, 045h, 040h, 058h, 080h, 0EDh, 016h, 052h, 004h, 048h, 0C0h, 089h, 0C0h
        db  081h, 060h, 0C2h, 005h, 05Bh, 004h, 00Bh, 0C3h, 0A0h, 006h, 0C6h, 0F0h, 041h, 0CCh, 042h, 006h
        db  0FDh, 016h, 0A0h, 006h, 0C6h, 0F0h, 001h, 0C1h, 044h, 08Ch, 012h, 016h, 0C4h, 005h, 042h, 006h
        db  0FBh, 016h, 004h, 002h, 00Ch, 0F1h, 003h, 002h, 001h, 001h, 094h, 006h, 003h, 002h, 05Ah, 05Ah
        db  094h, 006h, 043h, 005h, 094h, 006h, 003h, 007h, 094h, 006h, 0C3h, 004h, 094h, 006h, 0CCh, 005h
        db  05Ch, 004h, 0CBh, 0C1h, 0A0h, 006h, 0C6h, 0F0h, 043h, 0CCh, 042h, 006h, 0FDh, 016h, 0A0h, 006h
        db  0C6h, 0F0h, 043h, 08Ch, 0F5h, 016h, 042h, 006h, 0FCh, 016h, 057h, 004h, 08Bh, 0C2h, 008h, 0C0h
        db  049h, 0C1h, 085h, 005h, 080h, 002h, 040h, 000h, 003h, 011h, 080h, 002h, 04Fh, 000h, 045h, 012h
        db  001h, 002h, 0C6h, 0F0h, 0A1h, 009h, 001h, 080h, 040h, 013h, 001h, 002h, 0F6h, 0F1h, 0A1h, 009h
        db  001h, 080h, 03Bh, 013h, 060h, 0C0h, 006h, 000h, 0A1h, 009h, 001h, 080h, 036h, 013h, 081h, 005h
        db  001h, 080h, 033h, 013h, 04Ah, 0C0h, 0A1h, 009h, 001h, 080h, 02Fh, 013h, 000h, 0C8h, 06Ah, 001h
        db  080h, 002h, 080h, 000h, 017h, 014h, 001h, 002h, 000h, 0F8h, 0A0h, 0C1h, 040h, 001h, 0A0h, 001h
        db  040h, 001h, 000h, 004h, 002h, 002h, 000h, 010h, 003h, 002h, 000h, 004h, 0B1h, 0CCh, 043h, 006h
        db  0FDh, 016h, 0A0h, 001h, 040h, 001h, 000h, 040h, 008h, 002h, 010h, 0F8h, 006h, 0C8h, 040h, 001h
        db  000h, 0C0h, 002h, 013h, 008h, 002h, 000h, 0F8h, 009h, 002h, 0FEh, 0FBh, 0A0h, 006h, 0D0h, 0F0h
        db  025h, 010h, 080h, 002h, 080h, 000h, 009h, 014h, 001h, 002h, 000h, 0F8h, 002h, 002h, 000h, 010h
        db  003h, 002h, 000h, 004h, 072h, 0CCh, 043h, 006h, 0FDh, 016h, 080h, 005h, 080h, 002h, 080h, 000h
        db  004h, 012h, 060h, 001h, 004h, 001h, 020h, 000h, 005h, 013h, 040h, 081h, 0ABh, 016h, 080h, 002h
        db  080h, 000h, 00Bh, 014h, 0A0h, 007h, 06Ah, 001h, 07Eh, 000h, 002h, 002h, 000h, 010h, 003h, 002h
        db  000h, 004h, 0C1h, 004h, 081h, 0CCh, 043h, 006h, 0FDh, 016h, 0CAh, 005h, 05Ah, 004h, 000h, 002h
        db  0E8h, 0F1h, 001h, 002h, 018h, 0F3h, 040h, 002h, 000h, 0FCh, 041h, 002h, 000h, 0FCh, 040h, 080h
        db  004h, 013h, 0A0h, 007h, 004h, 001h, 03Ch, 000h, 05Bh, 004h, 0C0h, 004h, 001h, 002h, 008h, 000h
        db  002h, 002h, 000h, 012h, 0E0h, 0C1h, 040h, 001h, 0A0h, 001h, 040h, 001h, 000h, 004h, 003h, 002h
        db  000h, 001h, 000h, 0C8h, 06Ah, 001h, 0A0h, 0CCh, 010h, 0F8h, 080h, 005h, 003h, 006h, 0F9h, 016h
        db  022h, 002h, 000h, 002h, 001h, 006h, 0F3h, 016h, 0A0h, 001h, 040h, 001h, 000h, 040h, 007h, 0C8h
        db  040h, 001h, 000h, 002h, 000h, 008h, 040h, 0C0h, 001h, 006h, 001h, 0C8h, 06Ah, 001h, 061h, 002h
        db  000h, 080h, 001h, 0C8h, 010h, 0F8h, 000h, 006h, 0F6h, 016h, 0C0h, 004h, 0C8h, 004h, 0C9h, 004h
        db  003h, 002h, 000h, 008h, 000h, 0C8h, 06Ah, 001h, 080h, 0C1h, 066h, 002h, 000h, 080h, 020h, 0C1h
        db  010h, 0F8h, 006h, 081h, 015h, 016h, 008h, 0C2h, 006h, 013h, 080h, 005h, 003h, 006h, 0F2h, 016h
        db  008h, 0C2h, 00Dh, 013h, 019h, 010h, 0A0h, 007h, 010h, 0F8h, 055h, 055h, 020h, 0C1h, 010h, 0F8h
        db  084h, 002h, 055h, 055h, 002h, 016h, 006h, 0C2h, 0F0h, 010h, 006h, 081h, 0EEh, 013h, 05Bh, 004h
        db  0A0h, 007h, 010h, 0F8h, 055h, 055h, 060h, 0C1h, 010h, 0F8h, 005h, 081h, 003h, 013h, 085h, 002h
        db  055h, 055h, 0F5h, 016h, 008h, 0C2h, 0E1h, 013h, 040h, 0C2h, 009h, 006h, 048h, 002h, 0FFh, 007h
        db  0C0h, 004h, 001h, 002h, 008h, 000h, 002h, 002h, 000h, 012h, 003h, 002h, 000h, 001h, 000h, 0C8h
        db  06Ah, 001h, 032h, 0C8h, 010h, 0F8h, 080h, 005h, 003h, 006h, 0F9h, 016h, 022h, 002h, 000h, 002h
        db  001h, 006h, 0F3h, 016h, 088h, 002h, 040h, 000h, 013h, 015h, 089h, 002h, 04Fh, 000h, 010h, 011h
        db  0C0h, 004h, 002h, 002h, 000h, 012h, 001h, 002h, 008h, 000h, 003h, 002h, 000h, 001h, 080h, 0CCh
        db  003h, 006h, 0FDh, 016h, 022h, 002h, 000h, 002h, 001h, 006h, 0F7h, 016h, 0CBh, 005h, 05Bh, 004h
        db  0A0h, 007h, 004h, 001h, 037h, 000h, 05Bh, 004h, 0E0h, 002h, 0A0h, 000h, 088h, 007h, 0C0h, 000h
        db  009h, 002h, 076h, 0F3h, 084h, 007h, 0ECh, 0E5h, 005h, 002h, 001h, 000h, 08Bh, 0C2h, 0CCh, 004h
        db  0A0h, 006h, 080h, 0F3h, 060h, 02Ch, 001h, 000h, 099h, 006h, 0A0h, 02Ch, 002h, 000h, 099h, 006h
        db  020h, 02Dh, 004h, 000h, 099h, 006h, 020h, 02Eh, 008h, 000h, 099h, 006h, 0A0h, 02Fh, 010h, 000h
        db  08Ch, 005h, 009h, 016h, 080h, 0CCh, 081h, 0C4h, 083h, 007h, 0C4h, 0F3h, 088h, 0C0h, 002h, 004h
        db  08Ch, 005h, 001h, 016h, 033h, 010h, 0E0h, 002h, 0A0h, 000h, 05Ah, 004h, 08Ch, 005h, 0FBh, 016h
        db  080h, 0CCh, 081h, 0C4h, 015h, 00Ah, 0B4h, 0C0h, 012h, 0C0h, 088h, 0CCh, 052h, 0C0h, 0B4h, 0C4h
        db  042h, 006h, 05Bh, 004h, 02Dh, 007h, 018h, 000h, 041h, 08Bh, 00Ah, 000h, 0ECh, 016h, 0C1h, 082h
        db  0EAh, 016h, 0C2h, 002h, 042h, 002h, 000h, 002h, 0E6h, 016h, 080h, 003h, 081h, 007h, 001h, 000h
        db  0F1h, 010h, 001h, 002h, 002h, 000h, 0EEh, 010h, 001h, 002h, 004h, 000h, 0EBh, 010h, 001h, 002h
        db  008h, 000h, 0E8h, 010h, 001h, 002h, 010h, 000h, 0E5h, 010h, 0A1h, 002h, 041h, 08Bh, 010h, 000h
        db  002h, 013h, 060h, 004h, 070h, 0F3h, 02Dh, 007h, 018h, 000h, 080h, 003h, 009h, 002h, 000h, 008h
        db  003h, 002h, 004h, 000h, 0C7h, 004h, 0A0h, 006h, 0F0h, 0F4h, 060h, 001h, 01Ch, 001h, 004h, 000h
        db  01Ch, 016h, 0A0h, 001h, 040h, 001h, 000h, 008h, 0E0h, 001h, 040h, 001h, 000h, 010h, 004h, 002h
        db  001h, 000h, 044h, 0CEh, 0C4h, 006h, 044h, 0C6h, 0A0h, 001h, 040h, 001h, 000h, 010h, 049h, 006h
        db  0A0h, 001h, 040h, 001h, 000h, 040h, 0E0h, 001h, 040h, 001h, 000h, 004h, 0E0h, 001h, 040h, 001h
        db  000h, 008h, 0A0h, 006h, 08Eh, 0F5h, 0A0h, 006h, 08Eh, 0F5h, 0C7h, 005h, 004h, 002h, 0A6h, 0E4h
        db  0E0h, 004h, 0D0h, 003h, 074h, 0C1h, 0B4h, 0C1h, 086h, 005h, 01Ch, 013h, 0E0h, 002h, 0C0h, 000h
        db  000h, 002h, 000h, 000h, 040h, 0C0h, 080h, 0C0h, 0C0h, 0C0h, 000h, 0C1h, 040h, 0C1h, 080h, 0C1h
        db  0C0h, 0C1h, 000h, 0C2h, 040h, 0C2h, 080h, 0C2h, 0C0h, 0C2h, 000h, 0C3h, 040h, 0C3h, 080h, 0C3h
        db  0C0h, 0C3h, 0A0h, 004h, 0AAh, 000h, 0D0h, 003h, 0D0h, 003h, 03Fh, 010h, 085h, 005h, 085h, 081h
        db  0E1h, 013h, 0E4h, 010h, 0C7h, 005h, 005h, 002h, 0FFh, 07Fh, 045h, 0A1h, 0D0h, 003h, 0D0h, 003h
        db  034h, 010h, 0C0h, 0CCh, 0C1h, 0C4h, 003h, 002h, 028h, 000h, 0A0h, 006h, 0F0h, 0F4h, 0E0h, 001h
        db  042h, 001h, 000h, 010h, 0C7h, 005h, 0D0h, 003h, 0D0h, 003h, 027h, 010h, 0C7h, 005h, 0A0h, 0C1h
        db  04Ah, 001h, 0A0h, 007h, 04Ah, 001h, 000h, 00Eh, 0A0h, 001h, 040h, 001h, 000h, 002h, 020h, 007h
        db  044h, 001h, 060h, 0C1h, 044h, 001h, 085h, 002h, 000h, 0FFh, 017h, 016h, 0E0h, 001h, 040h, 001h
        db  000h, 022h, 005h, 002h, 080h, 000h, 005h, 006h, 0D0h, 003h, 0FDh, 016h, 060h, 0C1h, 046h, 001h
        db  085h, 002h, 000h, 0FFh, 00Ah, 013h, 005h, 002h, 0DDh, 021h, 005h, 006h, 000h, 010h, 000h, 010h
        db  000h, 010h, 000h, 010h, 000h, 010h, 0D0h, 003h, 0F8h, 016h, 051h, 010h, 006h, 0C8h, 04Ah, 001h
        db  0C0h, 0CCh, 0C1h, 0C4h, 04Bh, 010h, 013h, 0C0h, 0C8h, 0CCh, 053h, 0C0h, 002h, 002h, 000h, 0F5h
        db  0C2h, 0C4h, 043h, 006h, 05Bh, 004h, 060h, 0C0h, 0AEh, 000h, 0C4h, 002h, 044h, 002h, 00Fh, 000h
        db  044h, 088h, 08Ch, 0E4h, 03Ch, 016h, 081h, 002h, 008h, 000h, 027h, 013h, 021h, 0C1h, 09Eh, 0E4h
        db  014h, 0C1h, 021h, 021h, 07Ch, 0E4h, 033h, 016h, 021h, 0C1h, 084h, 0E4h, 081h, 002h, 000h, 000h
        db  00Bh, 013h, 00Dh, 002h, 0A0h, 000h, 084h, 083h, 009h, 013h, 0C4h, 005h, 084h, 083h, 006h, 013h
        db  0C4h, 005h, 084h, 083h, 003h, 013h, 023h, 010h, 00Eh, 081h, 021h, 016h, 021h, 0C1h, 09Eh, 0E4h
        db  021h, 045h, 07Ch, 0E4h, 0E0h, 001h, 042h, 001h, 000h, 010h, 0A0h, 001h, 042h, 001h, 000h, 010h
        db  0A1h, 0C3h, 096h, 0E4h, 00Fh, 002h, 02Fh, 000h, 080h, 003h, 0A0h, 001h, 040h, 001h, 000h, 002h
        db  0A0h, 001h, 040h, 001h, 000h, 080h, 06Dh, 0C0h, 00Ah, 000h, 009h, 013h, 081h, 002h, 00Ah, 000h
        db  006h, 015h, 00Eh, 002h, 0E6h, 0F4h, 00Fh, 002h, 00Fh, 000h, 080h, 003h, 0CAh, 005h, 0E0h, 002h
        db  0A0h, 000h, 05Ah, 004h, 059h, 0CEh, 020h, 088h, 0A6h, 0E4h, 0A6h, 0E4h, 0F8h, 010h, 0C1h, 004h
        db  048h, 062h, 089h, 005h, 0A0h, 0C0h, 06Ch, 001h, 008h, 0C8h, 06Ch, 001h, 003h, 002h, 000h, 0FCh
        db  004h, 002h, 000h, 002h, 073h, 0A0h, 004h, 006h, 0FDh, 016h, 088h, 005h, 009h, 006h, 0F4h, 016h
        db  002h, 0C8h, 06Ch, 001h, 081h, 086h, 001h, 016h, 0CBh, 005h, 05Bh, 004h, 043h, 028h, 031h, 029h
        db  038h, 039h, 02Dh, 033h, 038h, 038h, 042h, 020h, 020h, 059h, 049h, 054h, 04Bh, 0C2h, 0A8h, 002h
        db  098h, 000h, 083h, 007h, 002h, 000h, 028h, 002h, 008h, 000h, 023h, 0C6h, 0F8h, 0E4h, 048h, 006h
        db  0C4h, 0C0h, 073h, 00Ah, 065h, 017h, 0A0h, 006h, 0BCh, 0F6h, 0C8h, 0C1h, 0C7h, 005h, 003h, 002h
        db  0A5h, 000h, 0B0h, 003h, 0F8h, 0CDh, 0F8h, 0CDh, 0A6h, 002h, 006h, 062h, 088h, 002h, 00Ah, 000h
        db  057h, 016h, 003h, 029h, 055h, 016h, 005h, 029h, 0C4h, 080h, 052h, 016h, 015h, 009h, 050h, 017h
        db  015h, 009h, 04Eh, 018h, 085h, 002h, 029h, 000h, 04Bh, 016h, 0C6h, 005h, 096h, 000h, 003h, 007h
        db  0C4h, 004h, 045h, 006h, 095h, 000h, 044h, 005h, 043h, 016h, 044h, 081h, 041h, 016h, 000h, 003h
        db  005h, 000h, 0C4h, 002h, 000h, 003h, 00Ah, 000h, 044h, 002h, 00Fh, 000h, 084h, 002h, 005h, 000h
        db  037h, 016h, 0C4h, 002h, 000h, 003h, 00Fh, 000h, 044h, 002h, 00Fh, 000h, 084h, 002h, 00Ah, 000h
        db  02Fh, 016h, 004h, 002h, 0FEh, 0FFh, 02Ch, 013h, 02Bh, 015h, 02Ah, 01Ah, 084h, 005h, 028h, 012h
        db  027h, 015h, 026h, 01Ah, 025h, 018h, 084h, 005h, 023h, 016h, 022h, 01Bh, 021h, 017h, 084h, 005h
        db  01Fh, 013h, 01Eh, 01Ah, 01Dh, 011h, 004h, 006h, 01Bh, 016h, 0A5h, 002h, 0C5h, 0C1h, 025h, 002h
        db  006h, 000h, 003h, 002h, 0A5h, 0A5h, 083h, 0C1h, 095h, 000h, 003h, 038h, 094h, 000h, 083h, 002h
        db  02Eh, 06Bh, 00Eh, 016h, 084h, 002h, 059h, 01Ch, 00Bh, 016h, 024h, 002h, 069h, 000h, 095h, 000h
        db  003h, 03Ch, 094h, 000h, 083h, 081h, 004h, 016h, 084h, 002h, 069h, 000h, 001h, 016h, 0C9h, 005h
        db  059h, 004h, 0C3h, 0D0h, 0FDh, 013h, 001h, 01Ch, 0FBh, 010h, 0E0h, 090h, 0FFh, 0E4h, 0F8h, 016h
        db  0C3h, 006h, 0C3h, 0D0h, 0F5h, 01Ch, 0F4h, 016h, 0E0h, 090h, 0FCh, 0E4h, 0F1h, 016h, 05Bh, 004h
        db  00Bh, 0C3h, 009h, 002h, 000h, 0E5h, 0A0h, 006h, 04Ah, 0F0h, 0CCh, 005h, 05Ch, 004h, 088h, 007h
        db  000h, 0B0h, 089h, 007h, 0FEh, 0FFh, 0A8h, 009h, 0A9h, 009h, 08Ah, 007h, 002h, 0E0h, 0A0h, 006h
        db  098h, 0F5h, 000h, 000h, 088h, 007h, 000h, 0A0h, 089h, 007h, 0FEh, 0AFh, 0A8h, 009h, 0A9h, 009h
        db  08Ah, 007h, 07Ah, 0E0h, 0A0h, 006h, 098h, 0F5h, 000h, 000h, 0A0h, 006h, 0D6h, 0F5h, 000h, 000h
        db  0E6h, 010h, 0E5h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 0A0h, 000h, 008h, 023h, 0E2h, 0FFh, 09Ah, 000h, 080h, 000h, 040h, 000h, 020h, 000h, 010h
        db  000h, 008h, 000h, 004h, 000h, 002h, 000h, 001h, 080h, 000h, 040h, 000h, 020h, 000h, 010h, 000h
        db  008h, 000h, 004h, 000h, 002h, 000h, 001h, 000h, 000h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 000h, 0F0h
        db  00Fh, 000h, 0FFh, 0FFh, 0FFh, 07Fh, 003h, 000h, 000h, 000h, 0C3h, 000h, 0E7h, 0E7h, 0F2h, 0E7h
        db  0F3h, 0F2h, 043h, 028h, 020h, 029h, 04Fh, 043h, 059h, 050h, 049h, 052h, 048h, 047h, 020h, 054h
        db  042h, 049h, 020h, 04Dh, 039h, 031h, 033h, 038h, 034h, 02Ch, 035h, 02Ch, 036h, 02Ch, 043h, 028h
        db  020h, 029h, 04Fh, 043h, 059h, 050h, 049h, 052h, 048h, 047h, 020h, 054h, 049h, 054h, 031h, 020h
        db  038h, 039h, 02Dh, 033h, 039h, 038h, 039h, 02Ch, 02Ch, 030h, 02Ch, 031h, 000h, 032h, 000h, 000h
        db  075h, 031h, 0D6h, 0F5h, 022h, 0F3h, 096h, 0ECh, 0DAh, 0F6h, 000h, 000h, 0C4h, 000h, 0BEh, 0BCh
        db  04Ah, 006h, 050h, 006h, 04Ch, 006h, 010h, 0D9h, 04Eh, 006h, 00Fh, 000h, 032h, 006h, 001h, 000h
        db  050h, 007h, 058h, 007h, 052h, 007h, 030h, 0C2h, 054h, 007h, 00Fh, 000h, 038h, 007h, 001h, 000h
        db  0BAh, 000h, 0A0h, 000h, 0BCh, 000h, 0E8h, 0F6h, 0BEh, 000h, 00Fh, 000h, 05Eh, 007h, 03Ah, 007h
        db  062h, 007h, 040h, 080h, 064h, 007h, 0B0h, 0C6h, 066h, 007h, 08Eh, 0C6h, 068h, 007h, 082h, 0C4h
        db  098h, 007h, 000h, 080h, 078h, 007h, 000h, 080h, 094h, 008h, 004h, 000h, 096h, 008h, 001h, 000h
        db  09Eh, 008h, 008h, 000h, 0A8h, 008h, 00Ah, 000h, 0AAh, 008h, 006h, 000h, 0B2h, 008h, 00Ch, 000h
        db  0B4h, 008h, 004h, 000h, 0AEh, 001h, 000h, 000h, 0D0h, 008h, 000h, 000h, 014h, 009h, 000h, 000h
        db  00Ch, 006h, 013h, 000h, 00Ah, 006h, 020h, 000h, 000h, 000h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h
        db  0D8h, 0B2h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h, 0EEh, 0B4h, 0E0h, 000h, 05Eh, 0B5h, 0E0h, 000h
        db  0C2h, 0B8h, 0E0h, 000h, 004h, 0B4h, 0C0h, 000h, 014h, 0B4h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h
        db  078h, 0B3h, 0C0h, 000h, 066h, 0B4h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h
        db  078h, 0B3h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h, 078h, 0B3h, 0C0h, 000h, 0CAh, 0BCh, 0C0h, 000h
        db  036h, 0BDh, 0C0h, 000h, 072h, 0BDh, 0C0h, 000h, 0E2h, 0BDh, 0C0h, 000h, 064h, 0BEh, 0E0h, 000h
        db  0B8h, 0BFh, 0E0h, 000h, 05Eh, 0BFh, 0E0h, 000h, 0C8h, 0C0h, 0E0h, 000h, 050h, 0C0h, 0E0h, 000h
        db  026h, 0BAh, 0C0h, 000h, 07Ah, 0BAh, 0C0h, 000h, 0D6h, 0BAh, 0C0h, 000h, 008h, 0BBh, 0C0h, 000h
        db  050h, 0BAh, 0C0h, 000h, 072h, 0B3h, 0C0h, 000h, 072h, 0B3h, 000h, 03Fh, 000h, 07Fh, 000h, 05Eh
        db  030h, 000h, 028h, 000h, 043h, 000h, 0A4h, 0B6h, 0A4h, 0B6h, 008h, 0B5h, 000h, 0B5h, 032h, 0B5h
        db  032h, 0B5h, 04Eh, 0B5h, 0A4h, 0B6h, 000h, 040h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 008h
        db  000h, 020h, 000h, 008h, 000h, 080h, 000h, 008h, 000h, 001h, 000h, 010h, 000h, 000h, 008h, 000h
        db  00Ch, 000h, 010h, 000h, 014h, 000h, 00Eh, 010h, 00Ch, 00Ch, 00Ah, 00Ah, 00Ah, 00Ah, 008h, 008h
        db  008h, 008h, 008h, 008h, 008h, 008h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h
        db  006h, 006h, 006h, 006h, 006h, 006h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h
        db  002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h
        db  002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h
        db  002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h
        db  002h, 002h, 002h, 002h, 002h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 098h, 007h, 098h, 0D6h, 058h, 007h, 04Ch, 0C5h, 058h, 007h
        db  0C8h, 0C4h, 058h, 007h, 0BCh, 0C5h, 058h, 007h, 024h, 0C5h, 098h, 007h, 0F6h, 0D2h, 098h, 007h
        db  0EEh, 0D2h, 078h, 007h, 0B0h, 0CDh, 058h, 007h, 084h, 0C5h, 098h, 007h, 050h, 0D6h, 078h, 007h
        db  064h, 0CEh, 058h, 007h, 066h, 0D3h, 058h, 007h, 034h, 0C5h, 058h, 007h, 072h, 0C5h, 058h, 007h
        db  036h, 0C6h, 098h, 007h, 0E2h, 0D7h, 098h, 007h, 092h, 0D3h, 078h, 007h, 04Eh, 0CEh, 0B8h, 007h
        db  048h, 0D8h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 0C6h, 006h, 0CDh, 0B8h, 0CDh, 072h, 0D2h
        db  06Eh, 0C9h, 0A6h, 0C9h, 0C4h, 0C9h, 04Eh, 0CAh, 072h, 0CAh, 08Eh, 0CAh, 01Ah, 0CBh, 01Ah, 0CBh
        db  03Ah, 0CAh, 0B8h, 0CBh, 0F4h, 0C9h, 000h, 000h, 000h, 000h, 000h, 02Ch, 000h, 00Ch, 001h, 00Fh
        db  0FFh, 0FEh, 000h, 058h, 000h, 00Eh, 0FFh, 0FEh, 00Eh, 000h, 000h, 070h, 040h, 080h, 000h, 05Eh
        db  0A0h, 0C0h, 0DFh, 0FFh, 000h, 018h, 000h, 0E0h, 000h, 078h, 000h, 050h, 000h, 060h, 000h, 070h
        db  000h, 00Ch, 006h, 000h, 000h, 000h, 086h, 0E3h, 0E6h, 007h, 0EFh, 007h, 008h, 000h, 000h, 000h
        db  08Ch, 0E3h, 0EAh, 007h, 0EFh, 007h, 006h, 000h, 040h, 000h, 000h, 00Ah, 0E6h, 007h, 0EEh, 007h
        db  008h, 000h, 040h, 000h, 006h, 00Ah, 0EAh, 007h, 0EEh, 007h, 000h, 000h, 0E2h, 0C1h, 08Bh, 0D4h
        db  0FFh, 0FFh, 0D7h, 0D1h, 0D9h, 0C5h, 0D4h, 0C3h, 03Bh, 059h, 0E6h, 008h, 0FCh, 005h, 01Ah, 009h
        db  0D8h, 006h, 006h, 004h, 07Ch, 0EAh, 0E2h, 008h, 048h, 004h, 080h, 008h, 006h, 000h, 00Ah, 006h
        db  00Eh, 00Ch, 0B8h, 0DAh, 076h, 0E7h, 09Eh, 0E7h, 084h, 0E8h, 086h, 0E9h, 0E0h, 0E9h, 03Ah, 0EAh
        db  0ECh, 0EAh, 0B8h, 0DAh, 06Eh, 0EBh, 0FEh, 0EAh, 05Ch, 0E7h, 00Ah, 0EBh, 00Ah, 0EBh, 034h, 0ECh
        db  03Eh, 0ECh, 048h, 0ECh, 05Ch, 0E4h, 0E4h, 0E6h, 0AEh, 0DAh, 0AEh, 0E4h, 07Ch, 0E5h, 0ECh, 0E5h
        db  068h, 0E6h, 08Ch, 0E6h, 006h, 0DBh, 0AEh, 0DAh, 0CCh, 0E3h, 0AEh, 0E3h, 068h, 0E3h, 0AEh, 0DAh
        db  0F0h, 0E6h, 0AEh, 0DAh, 072h, 006h, 0A6h, 0DEh, 008h, 007h, 072h, 006h, 004h, 0DEh, 0F4h, 001h
        db  072h, 006h, 0E4h, 0DDh, 008h, 007h, 07Ch, 006h, 054h, 0E5h, 004h, 000h, 07Ch, 006h, 028h, 0DEh
        db  000h, 000h, 07Ch, 006h, 0D2h, 0E6h, 0FAh, 000h, 086h, 006h, 078h, 0DDh, 005h, 000h, 090h, 006h
        db  022h, 0E7h, 028h, 000h, 090h, 006h, 000h, 0DFh, 004h, 001h, 090h, 006h, 000h, 000h, 002h, 000h
        db  090h, 006h, 030h, 0DEh, 0BCh, 002h, 09Ah, 006h, 0B6h, 0DEh, 0DCh, 005h, 09Ah, 006h, 05Ah, 0DEh
        db  064h, 000h, 09Ah, 006h, 0BAh, 0DEh, 014h, 000h, 09Ah, 006h, 016h, 0E8h, 040h, 006h, 09Ah, 006h
        db  0C2h, 0DEh, 064h, 000h, 000h, 000h, 00Ah, 007h, 00Eh, 007h, 004h, 007h, 0D8h, 006h, 000h, 007h
        db  0F0h, 006h, 0EEh, 006h, 0ECh, 006h, 00Ch, 007h, 0E6h, 006h, 018h, 007h, 000h, 050h, 0CCh, 000h
        db  003h, 000h, 000h, 084h, 000h, 0A8h, 000h, 0A0h, 000h, 020h, 000h, 080h, 000h, 040h, 000h, 008h
        db  000h, 040h, 000h, 080h, 000h, 040h, 000h, 010h, 096h, 0F5h, 05Ch, 0F4h, 076h, 0F4h, 090h, 0F4h
        db  000h, 000h, 000h, 000h, 000h, 000h, 009h, 000h, 009h, 000h, 0FEh, 0F4h, 066h, 0F4h, 07Ch, 0F4h
        db  096h, 0F4h, 040h, 001h, 042h, 001h, 042h, 001h, 042h, 001h, 000h, 000h, 07Fh, 000h, 0A0h, 000h
        db  0FFh, 000h, 010h, 002h, 01Fh, 002h, 030h, 002h, 03Fh, 002h, 050h, 002h, 05Fh, 002h, 070h, 002h
        db  07Fh, 002h, 090h, 002h, 09Fh, 002h, 0B0h, 002h, 0BFh, 002h, 0D0h, 002h, 0DFh, 002h, 0E1h, 002h
        db  0FFh, 002h, 001h, 003h, 07Fh, 003h, 081h, 003h, 08Fh, 003h, 091h, 003h, 09Fh, 003h, 0A1h, 003h
        db  0AFh, 003h, 0B1h, 003h, 0BFh, 003h, 0C1h, 003h, 0CFh, 003h, 0E1h, 003h, 0FFh, 003h, 0C0h, 007h
        db  0FFh, 007h, 000h, 00Ch, 0FFh, 00Fh, 000h, 030h, 0FFh, 037h, 0FFh, 0FFh, 0FFh, 0FFh, 0BCh, 0FEh
        db  007h, 000h, 05Eh, 002h, 000h, 001h, 0FFh, 0BAh, 080h, 0BAh, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Ch, 001h, 00Ah, 001h, 00Eh, 001h, 010h, 001h, 014h, 001h, 000h, 000h, 012h, 001h, 000h, 0F8h
        db  016h, 001h, 000h, 0FFh, 000h, 000h, 000h, 000h, 00Ah, 001h, 01Ch, 001h, 082h, 001h, 066h, 096h
        db  066h, 096h, 055h, 055h, 000h, 000h, 082h, 001h, 02Ah, 08Ah, 02Ah, 08Ah, 018h, 0C9h, 018h, 0C9h
        db  086h, 001h, 0AAh, 0A2h, 01Eh, 0A0h, 055h, 055h, 01Eh, 054h, 08Ah, 001h, 000h, 050h, 000h, 000h
        db  000h, 0A8h, 000h, 000h, 08Ch, 001h, 000h, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 08Eh, 001h
        db  000h, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 090h, 001h, 000h, 050h, 000h, 000h, 000h, 0A8h
        db  000h, 000h, 092h, 001h, 000h, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 094h, 001h, 000h, 050h
        db  000h, 000h, 000h, 0A8h, 000h, 000h, 096h, 001h, 000h, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h
        db  098h, 001h, 000h, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 09Ah, 001h, 000h, 050h, 000h, 000h
        db  000h, 0A8h, 000h, 000h, 09Ch, 001h, 055h, 055h, 0C0h, 07Fh, 0AAh, 0AAh, 0C0h, 07Fh, 000h, 000h
        db  0A2h, 001h, 0A4h, 001h, 0A8h, 001h, 0AAh, 001h, 0AEh, 001h, 0B0h, 001h, 0B2h, 001h, 080h, 001h
        db  000h, 000h, 088h, 001h, 000h, 0FFh, 09Eh, 001h, 0FFh, 000h, 0A0h, 001h, 000h, 080h, 0ACh, 001h
        db  000h, 080h, 000h, 000h, 0A6h, 001h, 000h, 080h, 000h, 000h, 080h, 001h, 0B2h, 001h, 000h, 088h
        db  000h, 006h, 000h, 0C8h, 000h, 000h, 000h, 080h, 030h, 000h, 000h, 000h, 000h, 080h, 002h, 000h
        db  044h, 000h, 0A6h, 0F3h, 048h, 000h, 0ACh, 0F3h, 050h, 000h, 0B2h, 0F3h, 060h, 000h, 0B8h, 0F3h
        db  078h, 000h, 0BEh, 0F3h, 0C2h, 0EEh, 0D0h, 0EDh, 0FEh, 0E9h, 0E2h, 0EDh, 014h, 055h, 0A0h, 001h
        db  0A4h, 0EDh, 092h, 0E9h, 0FEh, 0EDh, 058h, 0EEh, 0B8h, 0EDh, 010h, 0EFh, 000h, 000h, 01Eh, 000h
        db  0FEh, 0EDh, 04Ah, 0EEh, 000h, 041h, 001h, 041h, 06Eh, 0EEh, 035h, 0EAh, 0D0h, 0EDh, 00Ah, 0EAh
        db  0A4h, 0EDh, 0C6h, 0E9h, 00Eh, 0EEh, 01Ah, 0EEh, 06Eh, 0EEh, 030h, 0EAh, 01Ah, 0EFh, 000h, 000h
        db  0EEh, 0EEh, 0A4h, 0EDh, 0BCh, 0E9h, 00Eh, 0EEh, 01Ah, 0EEh, 0EEh, 0EEh, 01Ah, 0EFh, 000h, 000h
        db  0A4h, 0EDh, 0B2h, 0E9h, 0C4h, 0EDh, 002h, 0EEh, 01Ah, 0EEh, 0EEh, 0EEh, 0A4h, 0EDh, 0BCh, 0E9h
        db  00Eh, 0EEh, 01Ah, 0EEh, 0EEh, 0EEh, 01Ah, 0EFh, 000h, 020h, 0E2h, 0EDh, 014h, 055h, 0A0h, 001h
        db  0D0h, 0EDh, 012h, 0EAh, 0A4h, 0EDh, 092h, 0E9h, 010h, 0EFh, 050h, 055h, 00Ch, 000h, 0FEh, 0EDh
        db  04Ah, 0EEh, 000h, 001h, 000h, 000h, 06Eh, 0EEh, 037h, 0EAh, 0B8h, 0EDh, 010h, 0EFh, 055h, 055h
        db  00Ch, 000h, 00Eh, 0EEh, 058h, 0EEh, 0B8h, 0EDh, 010h, 0EFh, 0FFh, 0FFh, 008h, 000h, 010h, 0EFh
        db  002h, 010h, 006h, 000h, 0FEh, 0EDh, 04Ah, 0EEh, 000h, 001h, 001h, 001h, 06Eh, 0EEh, 042h, 0EAh
        db  0B8h, 0EDh, 010h, 0EFh, 000h, 0C0h, 008h, 000h, 010h, 0EFh, 0FFh, 0FFh, 00Ah, 000h, 010h, 0EFh
        db  0FFh, 0FFh, 00Ch, 000h, 010h, 0EFh, 00Dh, 010h, 006h, 000h, 0FEh, 0EDh, 04Ah, 0EEh, 000h, 001h
        db  001h, 001h, 06Eh, 0EEh, 036h, 0EAh, 01Ah, 0EFh, 008h, 020h, 0B8h, 0EDh, 00Ah, 0EFh, 082h, 001h
        db  002h, 0C9h, 0FEh, 0EDh, 06Eh, 0EEh, 042h, 0EAh, 01Ah, 0EFh, 034h, 020h, 0B8h, 0EDh, 010h, 0EFh
        db  000h, 010h, 006h, 000h, 0FEh, 0EDh, 07Eh, 0EFh, 06Eh, 0EEh, 03Ah, 0EAh, 00Ah, 0EFh, 09Ch, 001h
        db  040h, 000h, 0D0h, 0EDh, 012h, 0EAh, 0E2h, 0EDh, 0FFh, 000h, 080h, 007h, 0DEh, 0EFh, 003h, 000h
        db  01Eh, 0F0h, 02Ch, 0F0h, 0D4h, 0E9h, 0F0h, 0EFh, 000h, 000h, 02Ch, 0F0h, 0DEh, 0E9h, 0F0h, 0EFh
        db  004h, 000h, 02Ch, 0F0h, 0E6h, 0E9h, 0F0h, 0EFh, 007h, 000h, 02Ch, 0F0h, 0EEh, 0E9h, 0DEh, 0EFh
        db  001h, 000h, 02Ch, 0F0h, 0F6h, 0E9h, 0F0h, 0EFh, 002h, 000h, 02Ch, 0F0h, 0F6h, 0E9h, 0F0h, 0EFh
        db  006h, 000h, 02Ch, 0F0h, 0F6h, 0E9h, 0F0h, 0EFh, 005h, 000h, 02Ch, 0F0h, 0F6h, 0E9h, 0DEh, 0EFh
        db  001h, 000h, 002h, 0F0h, 0DEh, 0EFh, 003h, 000h, 010h, 0F0h, 01Ah, 0EEh, 09Eh, 0EFh, 09Ah, 0E9h
        db  001h, 000h, 09Eh, 0EFh, 0E7h, 0E9h, 002h, 000h, 09Eh, 0EFh, 0F1h, 0E9h, 006h, 000h, 09Eh, 0EFh
        db  0FCh, 0E9h, 005h, 000h, 06Eh, 0EEh, 036h, 0EAh, 0EEh, 0EEh, 0A4h, 0EDh, 092h, 0E9h, 00Eh, 0EEh
        db  07Eh, 0EFh, 0C4h, 0EDh, 04Ah, 0EEh, 000h, 001h, 000h, 080h, 06Eh, 0EEh, 03Ah, 0EAh, 0B8h, 0EDh
        db  0B6h, 0EFh, 00Ah, 0EFh, 080h, 001h, 041h, 08Eh, 002h, 0EEh, 04Ah, 0EEh, 000h, 001h, 001h, 01Bh
        db  0BEh, 0EFh, 0E4h, 0FFh, 06Eh, 0EEh, 03Eh, 0EAh, 076h, 0EFh, 0D0h, 0EDh, 018h, 0EAh, 0C4h, 0EDh
        db  022h, 0EFh, 0F4h, 0EDh, 000h, 0E0h, 07Eh, 0EFh, 06Eh, 0EEh, 048h, 0EAh, 0F4h, 0EDh, 000h, 0E8h
        db  01Ah, 0EEh, 06Eh, 0EEh, 047h, 0EAh, 0F4h, 0EDh, 000h, 008h, 07Eh, 0EFh, 06Eh, 0EEh, 048h, 0EAh
        db  0F4h, 0EDh, 000h, 0F8h, 01Ah, 0EEh, 06Eh, 0EEh, 047h, 0EAh, 00Ah, 0EFh, 080h, 001h, 000h, 002h
        db  0F4h, 0EDh, 000h, 0E0h, 01Ah, 0EEh, 06Eh, 0EEh, 047h, 0EAh, 00Ah, 0EFh, 084h, 001h, 000h, 000h
        db  01Ah, 0EFh, 034h, 000h, 0F4h, 0EDh, 000h, 000h, 07Eh, 0EFh, 01Ah, 0EFh, 034h, 060h, 0C6h, 0EFh
        db  00Ah, 0EFh, 084h, 001h, 000h, 000h, 06Eh, 0EEh, 048h, 0EAh, 00Ah, 0EFh, 082h, 001h, 000h, 0C8h
        db  0F4h, 0EDh, 000h, 0E0h, 07Eh, 0EFh, 0F4h, 0EDh, 000h, 010h, 07Eh, 0EFh, 01Ah, 0EFh, 034h, 060h
        db  00Ah, 0EFh, 080h, 001h, 000h, 006h, 0F4h, 0EDh, 010h, 000h, 030h, 0EFh, 0EEh, 0EEh, 00Ah, 0EFh
        db  084h, 001h, 000h, 000h, 01Ah, 0EFh, 034h, 000h, 0A4h, 0EDh, 092h, 0E9h, 0D0h, 0EDh, 01Eh, 0EAh
        db  088h, 0EFh, 054h, 0E9h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh, 040h, 0F0h, 006h, 000h, 000h, 0C7h
        db  058h, 0EEh, 094h, 0EFh, 000h, 0E0h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh, 040h, 0D0h, 006h, 000h
        db  000h, 0E0h, 058h, 0EEh, 094h, 0EFh, 000h, 0C0h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh, 040h, 090h
        db  006h, 000h, 000h, 0A0h, 058h, 0EEh, 094h, 0EFh, 000h, 080h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh
        db  040h, 050h, 006h, 000h, 000h, 060h, 058h, 0EEh, 094h, 0EFh, 000h, 040h, 0B8h, 0EDh, 0C4h, 0EDh
        db  028h, 0EEh, 040h, 010h, 006h, 000h, 000h, 020h, 058h, 0EEh, 094h, 0EFh, 000h, 000h, 088h, 0EFh
        db  054h, 0E9h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh, 040h, 0D0h, 006h, 000h, 000h, 0A6h, 058h, 0EEh
        db  094h, 0EFh, 000h, 0C0h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh, 040h, 090h, 006h, 000h, 000h, 0C0h
        db  058h, 0EEh, 094h, 0EFh, 000h, 080h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh, 040h, 050h, 006h, 000h
        db  000h, 040h, 058h, 0EEh, 094h, 0EFh, 000h, 040h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh, 040h, 070h
        db  006h, 000h, 000h, 060h, 058h, 0EEh, 094h, 0EFh, 000h, 060h, 036h, 0F0h, 052h, 0E9h, 0D0h, 0EDh
        db  024h, 0EAh, 0A4h, 0EDh, 092h, 0E9h, 05Ch, 0EFh, 055h, 055h, 016h, 000h, 0FEh, 0EDh, 04Ah, 0EEh
        db  000h, 000h, 000h, 000h, 06Eh, 0EEh, 04Dh, 0EAh, 0C2h, 0EEh, 0D0h, 0EDh, 024h, 0EAh, 010h, 0EFh
        db  055h, 055h, 016h, 000h, 0B8h, 0EDh, 0FEh, 0EDh, 058h, 0EEh, 0E2h, 0EDh, 0FFh, 000h, 000h, 008h
        db  0E2h, 0EDh, 0FFh, 000h, 000h, 00Ch, 0E2h, 0EDh, 0FFh, 000h, 000h, 010h, 0E2h, 0EDh, 0FFh, 000h
        db  000h, 014h, 0E2h, 0EDh, 0FFh, 000h, 000h, 018h, 0E2h, 0EDh, 0FFh, 000h, 000h, 01Ch, 0E2h, 0EDh
        db  0FFh, 000h, 000h, 020h, 0E2h, 0EDh, 0FFh, 000h, 000h, 024h, 0E2h, 0EDh, 0FFh, 000h, 000h, 028h
        db  0E2h, 0EDh, 0FFh, 000h, 000h, 02Ch, 08Ah, 0EEh, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh, 040h, 030h
        db  006h, 000h, 000h, 001h, 058h, 0EEh, 094h, 0EFh, 000h, 020h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh
        db  040h, 070h, 006h, 000h, 000h, 043h, 058h, 0EEh, 094h, 0EFh, 000h, 060h, 0B8h, 0EDh, 0C4h, 0EDh
        db  028h, 0EEh, 040h, 0B0h, 006h, 000h, 000h, 085h, 058h, 0EEh, 094h, 0EFh, 000h, 0A0h, 090h, 0EFh
        db  000h, 001h, 003h, 001h, 001h, 001h, 000h, 000h, 000h, 081h, 01Ah, 000h, 040h, 010h, 055h, 055h
        db  055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 072h, 082h, 04Ah, 0A9h, 0A5h, 05Ah
        db  0DAh, 0E7h, 003h, 009h, 011h, 09Dh, 000h, 000h, 000h, 081h, 004h, 000h, 0D8h, 090h, 000h, 010h
        db  000h, 000h, 000h, 081h, 004h, 000h, 0D8h, 090h, 0D8h, 0B4h, 000h, 000h, 000h, 081h, 008h, 000h
        db  0D8h, 090h, 046h, 016h, 000h, 040h, 0D8h, 0B4h, 008h, 000h, 000h, 000h, 000h, 080h, 013h, 000h
        db  040h, 010h, 016h, 000h, 000h, 000h, 000h, 080h, 010h, 000h, 015h, 000h, 000h, 000h, 000h, 080h
        db  00Fh, 000h, 015h, 000h, 000h, 000h, 000h, 081h, 00Fh, 000h, 006h, 000h, 000h, 000h, 000h, 080h
        db  012h, 000h, 00Ah, 080h, 040h, 09Eh, 000h, 0C8h, 000h, 000h, 05Eh, 080h, 00Fh, 000h, 006h, 080h
        db  040h, 0FEh, 000h, 0CCh, 000h, 000h, 004h, 080h, 040h, 08Eh, 000h, 0C9h, 004h, 080h, 000h, 006h
        db  000h, 0CCh, 004h, 080h, 040h, 00Ah, 000h, 0C8h, 00Ah, 080h, 040h, 08Ah, 000h, 0C8h, 000h, 000h
        db  05Eh, 080h, 00Fh, 000h, 00Ah, 008h, 080h, 01Ch, 00Ah, 000h, 01Ch, 01Ah, 000h, 080h, 01Ch, 00Ch
        db  000h, 080h, 01Ch, 01Ah, 000h, 080h, 01Ah, 00Eh, 080h, 01Ch, 004h, 000h, 000h, 080h, 080h, 002h
        db  002h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  058h, 007h, 05Ch, 0C4h, 016h, 0E0h, 094h, 008h, 09Eh, 008h, 0A8h, 008h, 016h, 0E0h, 0B2h, 008h
        db  0BCh, 008h, 0B8h, 007h, 0FEh, 0D7h, 080h, 002h, 0B8h, 007h, 01Ch, 0D8h, 084h, 0FFh, 0B8h, 007h
        db  03Eh, 0D8h, 0B8h, 007h, 0B8h, 0D8h, 098h, 0D9h, 08Ch, 0D9h, 0B2h, 0D9h, 0B6h, 0D9h, 0AEh, 0D9h
        db  096h, 0DAh, 096h, 0DAh, 096h, 0DAh, 0B8h, 0D9h, 0C2h, 0D9h, 03Eh, 0DAh, 088h, 0DAh, 096h, 0DAh
        db  096h, 0DAh, 096h, 0DAh, 096h, 0DAh, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 000h
        db  008h, 001h, 005h, 008h, 008h, 008h, 000h, 008h, 002h, 000h, 004h, 004h, 004h, 004h, 008h, 000h
        db  000h, 00Ah, 00Ch, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 004h, 041h, 006h, 00Bh, 008h, 0C2h
        db  000h, 0E6h, 000h, 0E7h, 004h, 006h, 004h, 007h, 004h, 003h, 006h, 004h, 004h, 005h, 004h, 088h
        db  003h, 000h, 01Ch, 000h, 000h, 00Ch, 000h, 080h, 0F6h, 0E2h, 0FEh, 0E2h, 042h, 0E3h, 002h, 0E3h
        db  00Eh, 0E3h, 014h, 0E3h, 038h, 0E3h, 008h, 0E3h, 056h, 0E3h, 000h, 006h, 000h, 000h, 006h, 007h
        db  014h, 00Eh, 01Eh, 014h, 04Ch, 02Ah, 07Ch, 038h, 078h, 03Eh, 092h, 043h, 09Ch, 058h, 0B2h, 04Dh
        db  064h, 02Fh, 034h, 01Dh, 0D0h, 06Ch, 03Eh, 023h, 02Ah, 01Dh, 0C0h, 065h, 0E0h, 072h, 0EEh, 078h
        db  0FCh, 07Fh, 032h, 001h, 001h, 057h, 032h, 011h, 081h, 051h, 002h, 056h, 003h, 055h, 054h, 011h
        db  056h, 081h, 055h, 002h, 054h, 002h, 056h, 081h, 001h, 076h, 002h, 034h, 002h, 055h, 081h, 054h
        db  002h, 058h, 002h, 055h, 081h, 054h, 002h, 058h, 011h, 012h, 002h, 052h, 058h, 083h, 052h, 005h
        db  081h, 004h, 005h, 058h, 003h, 052h, 05Ch, 015h, 053h, 05Bh, 052h, 087h, 011h, 003h, 041h, 051h
        db  078h, 051h, 034h, 011h, 081h, 011h, 020h, 031h, 054h, 057h, 001h, 053h, 05Ah, 012h, 081h, 051h
        db  020h, 031h, 05Bh, 057h, 001h, 05Ah, 001h, 011h, 051h, 011h, 031h, 081h, 057h, 020h, 015h, 001h
        db  013h, 001h, 011h, 001h, 011h, 011h, 081h, 051h, 005h, 058h, 002h, 052h, 05Bh, 054h, 05Dh, 081h
        db  052h, 005h, 054h, 002h, 058h, 081h, 050h, 002h, 013h, 003h, 058h, 081h, 050h, 002h, 011h, 003h
        db  081h, 054h, 072h, 05Dh, 050h, 003h, 013h, 003h, 013h, 001h, 040h, 054h, 00Eh, 000h, 020h, 006h
        db  056h, 006h, 024h, 0E4h, 024h, 000h, 002h, 010h, 016h, 000h, 002h, 000h, 001h, 004h, 008h, 007h
        db  024h, 0E4h, 020h, 000h, 003h, 010h, 012h, 000h, 003h, 000h, 072h, 0E3h, 014h, 08Eh, 020h, 000h
        db  004h, 010h, 012h, 000h, 004h, 000h, 0D0h, 0DAh, 020h, 000h, 005h, 0E0h, 012h, 000h, 005h, 000h
        db  0D0h, 0DAh, 020h, 000h, 006h, 060h, 012h, 000h, 006h, 000h, 01Ch, 0E6h, 012h, 000h, 001h, 060h
        db  01Ah, 009h, 0CCh, 006h, 004h, 000h, 007h, 000h, 000h, 000h, 030h, 006h, 03Ch, 0E5h, 0F0h, 005h
        db  000h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 0E2h, 005h, 008h, 000h, 026h, 0FFh, 0DCh, 005h
        db  0A8h, 0E6h, 03Eh, 000h, 000h, 060h, 000h, 0C0h, 000h, 000h, 002h, 000h, 030h, 000h, 020h, 050h
        db  023h, 00Ch, 0FCh, 005h, 052h, 006h, 056h, 006h, 000h, 000h, 000h, 081h, 016h, 000h, 000h, 060h
        db  000h, 0C0h, 000h, 000h, 010h, 000h, 008h, 000h, 02Ah, 040h, 02Ah, 004h, 056h, 006h, 026h, 000h
        db  063h, 0ECh, 02Bh, 006h, 020h, 009h, 022h, 000h, 024h, 000h, 079h, 0ECh, 023h, 00Ch, 0FCh, 005h
        db  028h, 008h, 0E6h, 008h, 029h, 008h, 058h, 007h, 078h, 007h, 098h, 007h, 023h, 000h, 02Ah, 000h
        db  087h, 0ECh, 006h, 004h, 0F0h, 006h, 007h, 004h, 0EEh, 006h, 024h, 000h, 0D0h, 0DAh, 034h, 000h
        db  000h, 060h, 000h, 0C0h, 000h, 000h, 010h, 000h, 026h, 000h, 025h, 040h, 0D0h, 0DAh, 020h, 000h
        db  000h, 060h, 000h, 0C0h, 000h, 000h, 010h, 000h, 012h, 000h, 026h, 040h, 0D0h, 0DAh, 01Ah, 000h
        db  000h, 060h, 00Ch, 000h, 027h, 060h, 00Ah, 008h, 0E6h, 006h, 0D0h, 0DAh, 024h, 000h, 000h, 060h
        db  016h, 000h, 028h, 060h, 030h, 004h, 006h, 007h, 026h, 0DBh, 000h, 081h, 030h, 000h, 000h, 060h
        db  022h, 000h, 029h, 060h, 02Dh, 008h, 01Ch, 007h, 02Eh, 008h, 022h, 007h, 000h, 000h, 008h, 002h
        db  006h, 001h, 014h, 006h, 018h, 008h, 020h, 00Ch, 026h, 00Eh, 030h, 00Fh, 034h, 011h, 03Eh, 012h
        db  042h, 014h, 046h, 016h, 01Ch, 00Ah, 04Ah, 018h, 013h, 003h, 011h, 083h, 001h, 011h, 011h, 081h
        db  012h, 081h, 013h, 001h, 052h, 083h, 081h, 085h, 085h, 011h, 012h, 081h, 012h, 081h, 019h, 081h
        db  060h, 085h, 000h, 0C0h, 000h, 000h, 008h, 000h, 01Ah, 009h, 000h, 000h, 030h, 006h, 0F6h, 0EBh
        db  054h, 006h, 050h, 006h, 038h, 002h, 021h, 004h, 0D0h, 008h, 00Bh, 006h, 0D8h, 006h, 002h, 008h
        db  0DCh, 006h, 000h, 0C0h, 0FFh, 0FFh, 0FFh, 0FFh, 041h, 000h, 041h, 000h, 014h, 0AEh, 000h, 000h
        db  000h, 081h, 009h, 004h, 00Ch, 007h, 041h, 000h, 041h, 000h, 014h, 002h, 000h, 000h, 000h, 081h
        db  00Bh, 006h, 0D8h, 006h, 02Ch, 006h, 024h, 009h, 022h, 014h, 0ECh, 008h, 041h, 000h, 041h, 000h
        db  054h, 002h, 000h, 000h, 000h, 081h, 0D8h, 006h, 000h, 084h, 000h, 048h, 0FCh, 0FFh, 009h, 000h
        db  000h, 0C0h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 0B8h, 0FFh, 020h, 000h, 043h, 028h, 031h, 029h, 038h, 039h
        db  02Dh, 033h, 039h, 038h, 039h, 02Ch, 02Ch, 030h, 02Ch, 031h, 020h, 032h, 065h, 054h, 061h, 078h
        db  020h, 073h, 06Eh, 049h, 074h, 073h, 075h, 072h, 065h, 06Dh, 074h, 06Eh, 00Ah, 073h, 043h, 028h
        db  031h, 029h, 038h, 039h, 02Ch, 033h, 02Ch, 034h, 02Ch, 035h, 020h, 036h, 042h, 049h, 020h, 04Dh
        db  06Fh, 043h, 070h, 072h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0F8h, 0FFh, 001h, 000h
        db  034h, 0A0h, 000h, 000h, 0FAh, 0FFh, 001h, 000h, 0B8h, 0FFh, 000h, 000h, 0FCh, 0FFh, 002h, 000h
        db  080h, 000h, 03Eh, 0B0h, 001h, 000h, 000h, 000h, 000h, 000h, 043h, 054h, 052h, 04Fh, 04Eh, 020h
        db  034h, 02Fh, 031h, 036h, 020h, 04Dh, 062h, 070h, 073h, 020h, 054h, 052h, 000h, 000h
loc_1_68A4:
        inc    ebx
        push   esp
        push   edx
        dec    edi
        dec    esi
        and    byte ptr [edi + ebp], dh
        xor    dword ptr [esi], esi
        and    byte ptr [ebp + 0x62], cl
        jo     0x6926
        and    byte ptr [edx + edx*2], dl
        db  000h
loc_1_68B8:
        inc    ebx
        popal  
        bound  ebp, qword ptr [ebp + 0x74]
        jb     0x692f
        outsb  dx, byte ptr [esi]
        and    byte ptr [edi + ebp*2 + 0x6b], dl
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [edx + 0x69], dl
        outsb  dx, byte ptr [esi]
        and    byte ptr [bp + 0x65], cl
        je     0x6948
        outsd  dx, dword ptr [esi]
        jb     0x693f
        and    byte ptr [ebx + 0x61], al
        jb     0x693d
        and    byte ptr [esi + 0x6f], ah
        jb     0x68fe
        dec    ecx
        push   ebx
        inc    ecx
        and    byte ptr [edx + 0x75], ah
        jae    0x6914
        add    byte ptr [eax], al
loc_1_68E8:
        add    byte ptr [eax], al
loc_1_68EA:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x570  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        mov    eax, dword ptr [0x68ec]       ; -> obj1:0x68EC
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   0x32
        push   eax
        call   sub_1_05FE
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
        call   sub_1_0604
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
        mov    eax, dword ptr [0x68ec]       ; -> obj1:0x68EC
        push   eax
        push   0
        push   0x1f
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05FE
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
        push   0x68e8                        ; -> loc_1_68E8
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
        push   0x68e8                        ; -> loc_1_68E8
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
        call   sub_1_0604
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
        mov    al, byte ptr [0x68e8]         ; -> loc_1_68E8
        cmp    byte ptr [ecx], al
        jne    loc_2_0408
        mov    al, byte ptr [0x68e9]         ; -> obj1:0x68E9
        cmp    byte ptr [ecx + 1], al
        jne    loc_2_0408
        mov    al, byte ptr [0x68ea]         ; -> loc_1_68EA
        cmp    byte ptr [ecx + 2], al
        jne    loc_2_0408
        mov    al, byte ptr [0x68eb]         ; -> obj1:0x68EB
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

VxD__LDATA SEGMENT   ; object 4  vsize=0x10c  RW-
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 006h, 000h, 006h, 000h
        dd  loc_1_07A0               ; fixup
        db  001h, 001h, 001h, 000h, 002h, 001h, 001h, 000h, 003h, 001h, 001h, 000h, 004h, 001h, 001h, 000h
        db  005h, 001h, 001h, 000h, 006h, 001h, 001h, 000h, 011h, 001h, 001h, 000h, 013h, 001h, 001h, 000h
        db  012h, 001h, 001h, 000h, 007h, 001h, 001h, 000h, 008h, 001h, 001h, 000h, 009h, 001h, 001h, 000h
        db  00Ah, 001h, 001h, 000h, 00Bh, 001h, 001h, 000h, 00Ch, 001h, 001h, 000h, 00Dh, 001h, 001h, 000h
        db  010h, 001h, 001h, 000h, 00Eh, 001h, 001h, 000h, 00Fh, 001h, 001h, 000h, 001h, 001h, 002h, 000h
        db  002h, 001h, 002h, 000h, 003h, 001h, 002h, 000h, 004h, 001h, 002h, 000h, 005h, 001h, 002h, 000h
        db  001h, 001h, 001h, 002h, 002h, 001h, 001h, 002h, 003h, 001h, 001h, 002h, 004h, 001h, 001h, 002h
        db  005h, 001h, 001h, 002h, 001h, 001h, 002h, 002h, 002h, 001h, 002h, 002h, 006h, 001h, 001h, 002h
        db  007h, 001h, 001h, 002h, 001h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 003h, 002h, 002h, 002h
        db  004h, 002h, 002h, 002h, 005h, 002h, 002h, 002h, 006h, 002h, 002h, 002h, 007h, 002h, 002h, 002h
        db  001h, 001h, 001h, 000h, 002h, 001h, 001h, 000h, 003h, 001h, 001h, 000h, 004h, 001h, 001h, 000h
        db  005h, 001h, 001h, 000h, 006h, 001h, 001h, 000h, 011h, 001h, 001h, 000h, 013h, 001h, 001h, 000h
        db  012h, 001h, 001h, 000h, 007h, 001h, 001h, 000h, 008h, 001h, 001h, 000h, 009h, 001h, 001h, 000h
        db  00Ah, 001h, 001h, 000h, 00Bh, 001h, 001h, 000h, 00Ch, 001h, 001h, 000h, 00Dh, 001h, 001h, 000h
        db  010h, 001h, 001h, 000h, 00Eh, 001h, 001h, 000h, 00Fh, 001h, 001h, 000h, 001h, 001h, 001h, 002h
        db  002h, 001h, 001h, 002h, 003h, 001h, 001h, 002h, 004h, 001h, 001h, 002h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x517f  R-X
sub_5_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x48
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0x48]
        mov    dword ptr [ebp - 0x10], eax
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_054A
loc_5_0022:
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   eax
        push   0
        push   8
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05FE
loc_5_0035:
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_004C
        mov    eax, 0xc000009a
        jmp    loc_5_011F
loc_5_004C:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 3
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 6], 0        ; -> loc_5_045B
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0xa], 0      ; -> loc_5_0661
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x1a], 0     ; -> loc_5_3808
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0xe], 0      ; -> loc_5_24CC
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x12], 0     ; -> loc_5_4708
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x16], 0     ; -> loc_5_1016
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x22], 0     ; -> loc_5_0E8C
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x1e], 0     ; -> loc_5_4583
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x26], 0     ; -> loc_5_0980
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x2a], 0     ; -> loc_5_0E1A
        mov    eax, dword ptr [0]            ; -> obj4:0x0008
        mov    ecx, dword ptr [4]            ; -> obj4:0x000C
        mov    edx, dword ptr [ebp - 0x10]
        add    edx, 0x2e
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], ecx
        push   0x36
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0556
loc_5_00F9:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_0118
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0550
loc_5_010E:
        mov    eax, 0xc0000001
        jmp    loc_5_011F
loc_5_0118:
        xor    eax, eax
        jmp    loc_5_011F
loc_5_011F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_5_0126:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 0x34], 0
        jmp    loc_5_0138
loc_5_0138:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_0EC4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_0FAB
        lea    edi, [ebp - 0x30]
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dword ptr [ebp - 0x26], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ce]
        mov    dword ptr [ebp - 0x30], eax
        mov    byte ptr [ebp - 0x2c], 1
        mov    byte ptr [ebp - 0x2b], 0
        mov    dword ptr [ebp - 0x2a], 0
        mov    dword ptr [ebp - 0x22], 0
        mov    dword ptr [ebp - 0x1a], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c6]
        mov    dword ptr [ebp - 0x16], eax
        mov    dword ptr [ebp - 0x12], 4
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x52]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x56
        push   eax
        call   sub_1_0562
loc_5_01BE:
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_01D3
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_5_0454
loc_5_01D3:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        test   eax, eax
        je     loc_5_020C
        push   0
        push   0xaa00000a
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_06D0
loc_5_01F3:
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_0568
loc_5_0202:
        mov    eax, 0xc0000001
        jmp    loc_5_0454
loc_5_020C:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_12AC
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        test   eax, eax
        je     loc_5_0240
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_0568
loc_5_0236:
        mov    eax, 0xc0010006
        jmp    loc_5_0454
loc_5_0240:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_4848
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_0260
        mov    eax, 0xc000009a
        jmp    loc_5_0454
loc_5_0260:
        push   1
        push   1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ba]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ba]
        push   eax
        push   0                             ; -> loc_5_2E3C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0                             ; -> loc_5_2DEC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05E6
loc_5_029A:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_02DB
        push   3
        push   0
        push   2
        push   0xaa000005
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_06D0
loc_5_02BB:
        add    esp, 0x14
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_4D48
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_0568
loc_5_02D3:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_5_0454
loc_5_02DB:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x23c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x240], 0    ; -> loc_5_31DF
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x244], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x5a
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x4e
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1ed
        push   eax
        push   0                             ; -> loc_5_34CA
        push   0                             ; -> loc_5_3476
        push   0                             ; -> loc_5_3427
        push   0                             ; -> loc_5_33D8
        call   sub_2_0000
loc_5_0331:
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_0399
        push   2
        push   0
        push   2
        push   0xaa000002
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_06D0
loc_5_0355:
        add    esp, 0x14
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1c], 1
        lea    eax, [ebp - 0x34]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x23c
        push   eax
        call   sub_1_05E0
loc_5_0371:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05EC
loc_5_037A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_4D48
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_0568
loc_5_038F:
        mov    eax, 0xc000009a
        jmp    loc_5_0454
loc_5_0399:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_12FC
        and    ax, 0xff
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        test   eax, eax
        je     loc_5_0426
        mov    byte ptr [ebp + 0x14], 1
        push   1
        push   0
        push   2
        push   0xaa000003
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_06D0
loc_5_03D6:
        add    esp, 0x14
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1c], 1
        lea    eax, [ebp - 0x34]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x23c
        push   eax
        call   sub_1_05E0
loc_5_03F2:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5a]
        push   eax
        call   sub_2_00D0
loc_5_03FE:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05EC
loc_5_0407:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_4D48
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_0568
loc_5_041C:
        mov    eax, 0xc0010006
        jmp    loc_5_0454
loc_5_0426:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x248], 0x3c
        push   0x3e8
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x23c
        push   eax
        call   sub_1_05DA
loc_5_044D:
        xor    eax, eax
        jmp    loc_5_0454
loc_5_0454:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
loc_5_045B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_5_0483
loc_5_0480:
        inc    dword ptr [ebp - 0xc]
loc_5_0483:
        mov    eax, dword ptr [ebp + 0x18]
        cmp    dword ptr [ebp - 0xc], eax
        jae    loc_5_04A9
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0x14]
        cmp    dword ptr [ecx + eax*4], 1
        jne    loc_5_04A4
        jmp    loc_5_04A9
loc_5_04A4:
        jmp    loc_5_0480
loc_5_04A9:
        mov    eax, dword ptr [ebp + 0x18]
        cmp    dword ptr [ebp - 0xc], eax
        jne    loc_5_04BF
        mov    eax, 0xc0010019
        jmp    loc_5_065A
loc_5_04BF:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   eax
        push   0
        push   0x96
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05FE
loc_5_04E9:
        test   eax, eax
        jne    loc_5_05E2
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
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0x5a]
        push   eax
        call   sub_2_00F0
loc_5_0555:
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_0598
        push   4
        push   1
        push   2
        push   0xaa000002
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_06D0
loc_5_0579:
        add    esp, 0x14
        push   0
        push   0x96
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0604
loc_5_058C:
        mov    dword ptr [ebp - 0x10], 0xc0000001
        jmp    loc_5_05DD
loc_5_0598:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 0xf0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x1e], 0xf0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x36
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x3a]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0x3a]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x3a], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x14]
loc_5_05DD:
        jmp    loc_5_0606
loc_5_05E2:
        push   5
        push   1
        push   2
        push   0xaa000002
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_06D0
loc_5_05F9:
        add    esp, 0x14
        mov    eax, 0xc000009a
        jmp    loc_5_065A
loc_5_0606:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        dec    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [ebp - 0x14]
        cmp    dword ptr [eax + 0x2a], 0
        jne    loc_5_0652
        mov    eax, dword ptr [ebp - 0x14]
        mov    al, byte ptr [eax + 0x7d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_0644
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x14]
        add    ecx, 0x2e
        cmp    dword ptr [eax + 0x2e], ecx
        je     loc_5_0652
loc_5_0644:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_5_2F61
        jmp    loc_5_0652
loc_5_0652:
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_5_065A
loc_5_065A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x24
loc_5_0661:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_091E
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 0x14]
        push   0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x5a]
        push   eax
        call   sub_2_01D0
loc_5_06B4:
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_07A6
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x14], 2
        jne    loc_5_0740
        mov    eax, dword ptr [ebp - 8]
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
        mov    eax, dword ptr [ebp - 8]
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
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x1e]
        cmp    dword ptr [eax + 0x18], ecx
        jne    loc_5_072B
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_3579
loc_5_072B:
        push   0
        push   0x96
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0604
loc_5_073B:
        jmp    loc_5_07A1
loc_5_0740:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x1d], 1
        mov    eax, dword ptr [ebp - 8]
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
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x2e
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x32]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x32]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x32], eax
        mov    eax, dword ptr [ebp - 8]
        sub    dword ptr [eax + 0x14], 2
        mov    dword ptr [ebp - 0xc], 0x103
loc_5_07A1:
        jmp    loc_5_0919
loc_5_07A6:
        cmp    dword ptr [ebp - 0xc], 0x103
        jne    loc_5_081B
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_27AF
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x1d], 1
        mov    eax, dword ptr [ebp - 8]
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
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x2e
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x32]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x32]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x32], eax
        mov    eax, dword ptr [ebp - 8]
        sub    dword ptr [eax + 0x14], 2
        jmp    loc_5_0919
loc_5_081B:
        cmp    dword ptr [ebp - 0xc], 0xc001000e
        jne    loc_5_088D
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x1d], 1
        mov    dword ptr [ebp - 0xc], 0x103
        mov    eax, dword ptr [ebp - 8]
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
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x2e
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x32]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x32]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x32], eax
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_0919
loc_5_088D:
        cmp    dword ptr [ebp - 0xc], 0xc001000d
        jne    loc_5_08FF
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x1d], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x24]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x24]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x3e
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x42]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x42]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x42], eax
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 0xc], 0x103
        jmp    loc_5_0919
loc_5_08FF:
        push   1
        push   0xa
        push   2
        push   0xaa000006
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_06D0
loc_5_0916:
        add    esp, 0x14
loc_5_0919:
        jmp    loc_5_0925
loc_5_091E:
        mov    dword ptr [ebp - 0xc], 0xc0010002
loc_5_0925:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x28], eax
        mov    eax, dword ptr [ebp - 0x28]
        dec    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [ebp - 0x28]
        cmp    dword ptr [eax + 0x2a], 0
        jne    loc_5_0971
        mov    eax, dword ptr [ebp - 0x28]
        mov    al, byte ptr [eax + 0x7d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_0963
        mov    eax, dword ptr [ebp - 0x28]
        mov    ecx, dword ptr [ebp - 0x28]
        add    ecx, 0x2e
        cmp    dword ptr [eax + 0x2e], ecx
        je     loc_5_0971
loc_5_0963:
        mov    eax, dword ptr [ebp - 0x28]
        push   eax
        call   sub_5_2F61
        jmp    loc_5_0971
loc_5_0971:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_5_0979
loc_5_0979:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_5_0980:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x78
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x70], 0xa
        mov    word ptr [ebp - 0x6e], 0xa
        mov    dword ptr [ebp - 0x6c], 0     ; -> loc_1_07B4
        mov    word ptr [ebp - 0x1c], 0xa
        mov    word ptr [ebp - 0x1a], 0xa
        mov    dword ptr [ebp - 0x18], 0     ; -> obj1:0x07CC
        mov    word ptr [ebp - 0x54], 7
        mov    word ptr [ebp - 0x52], 7
        mov    dword ptr [ebp - 0x50], 0     ; -> loc_1_07E0
        mov    word ptr [ebp - 0x68], 0xb
        mov    word ptr [ebp - 0x66], 0xb
        mov    dword ptr [ebp - 0x64], 0     ; -> loc_1_07F4
        mov    word ptr [ebp - 0x4c], 0xa
        mov    word ptr [ebp - 0x4a], 0xa
        mov    dword ptr [ebp - 0x48], 0     ; -> obj1:0x080C
        mov    word ptr [ebp - 0x3c], 6
        mov    word ptr [ebp - 0x3a], 6
        mov    dword ptr [ebp - 0x38], 0     ; -> obj1:0x0820
        mov    word ptr [ebp - 0x60], 4
        mov    word ptr [ebp - 0x5e], 4
        mov    dword ptr [ebp - 0x5c], 0     ; -> obj1:0x082C
        mov    word ptr [ebp - 0x24], 4
        mov    word ptr [ebp - 0x22], 4
        mov    dword ptr [ebp - 0x20], 0     ; -> loc_1_0834
        mov    word ptr [ebp - 0x44], 4
        mov    word ptr [ebp - 0x42], 4
        mov    dword ptr [ebp - 0x40], 0     ; -> loc_1_083C
        mov    word ptr [ebp - 0x14], 4
        mov    word ptr [ebp - 0x12], 4
        mov    dword ptr [ebp - 0x10], 0     ; -> obj1:0x0844
        mov    dword ptr [ebp - 0x34], 0
        mov    word ptr [ebp - 0x78], 0xb
        mov    word ptr [ebp - 0x76], 0xb
        mov    dword ptr [ebp - 0x74], 0     ; -> obj1:0x0854
        mov    byte ptr [ebp - 0xc], 0
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   eax
        push   0
        push   0x2de
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05FE
loc_5_0A7B:
        test   eax, eax
        je     loc_5_0A8D
        mov    eax, 0xc000009a
        jmp    loc_5_0E13
loc_5_0A8D:
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 0xb7
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x21d], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x52], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x1f3], 0x1194
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x58]
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_1_067C
loc_5_0AD1:
        cmp    dword ptr [ebp - 0x34], 0
        je     loc_5_0AF5
        push   0
        push   0x2de
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0604
loc_5_0AEB:
        mov    eax, 0xc0000001
        jmp    loc_5_0E13
loc_5_0AF5:
        push   1
        lea    eax, [ebp - 0x70]
        push   eax
        mov    eax, dword ptr [ebp - 0x58]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_1_0682
loc_5_0B0C:
        cmp    dword ptr [ebp - 0x34], 0
        jne    loc_5_0B2A
        mov    eax, dword ptr [ebp - 0x28]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x1c6], eax
        jmp    loc_5_0B2E
loc_5_0B2A:
        mov    byte ptr [ebp - 0xc], 1
loc_5_0B2E:
        push   0
        lea    eax, [ebp - 0x68]
        push   eax
        mov    eax, dword ptr [ebp - 0x58]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_1_0682
loc_5_0B45:
        cmp    dword ptr [ebp - 0x34], 0
        jne    loc_5_0B63
        mov    eax, dword ptr [ebp - 0x28]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x1ce], eax
        jmp    loc_5_0B67
loc_5_0B63:
        mov    byte ptr [ebp - 0xc], 1
loc_5_0B67:
        push   0
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x58]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_1_0682
loc_5_0B7E:
        cmp    dword ptr [ebp - 0x34], 0
        jne    loc_5_0B9C
        mov    eax, dword ptr [ebp - 0x28]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x1ba], eax
        jmp    loc_5_0BA0
loc_5_0B9C:
        mov    byte ptr [ebp - 0xc], 1
loc_5_0BA0:
        push   0
        lea    eax, [ebp - 0x4c]
        push   eax
        mov    eax, dword ptr [ebp - 0x58]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_1_0682
loc_5_0BB7:
        cmp    dword ptr [ebp - 0x34], 0
        jne    loc_5_0BF2
        mov    eax, dword ptr [ebp - 0x28]
        cmp    dword ptr [eax + 4], 1
        jne    loc_5_0BE0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x1ca], 1
        jmp    loc_5_0BED
loc_5_0BE0:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x1ca], 0
loc_5_0BED:
        jmp    loc_5_0BF6
loc_5_0BF2:
        mov    byte ptr [ebp - 0xc], 1
loc_5_0BF6:
        push   2
        lea    eax, [ebp - 0x3c]
        push   eax
        mov    eax, dword ptr [ebp - 0x58]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_1_0682
loc_5_0C0D:
        cmp    dword ptr [ebp - 0x34], 0
        jne    loc_5_0C82
        push   1
        lea    eax, [ebp - 0x60]
        push   eax
        mov    eax, dword ptr [ebp - 0x28]
        add    eax, 4
        push   eax
        call   sub_1_068E
loc_5_0C29:
        and    eax, 0xff
        test   eax, eax
        je     loc_5_0C48
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x1c2], 1
        jmp    loc_5_0C7D
loc_5_0C48:
        push   1
        lea    eax, [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x28]
        add    eax, 4
        push   eax
        call   sub_1_068E
loc_5_0C5A:
        and    eax, 0xff
        test   eax, eax
        je     loc_5_0C79
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x1c2], 0
        jmp    loc_5_0C7D
loc_5_0C79:
        mov    byte ptr [ebp - 0xc], 1
loc_5_0C7D:
        jmp    loc_5_0C86
loc_5_0C82:
        mov    byte ptr [ebp - 0xc], 1
loc_5_0C86:
        push   2
        lea    eax, [ebp - 0x54]
        push   eax
        mov    eax, dword ptr [ebp - 0x58]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_1_0682
loc_5_0C9D:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x1d2], 0
        cmp    dword ptr [ebp - 0x34], 0
        jne    loc_5_0D1A
        push   1
        lea    eax, [ebp - 0x44]
        push   eax
        mov    eax, dword ptr [ebp - 0x28]
        add    eax, 4
        push   eax
        call   sub_1_068E
loc_5_0CC6:
        and    eax, 0xff
        test   eax, eax
        je     loc_5_0CE5
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x1d2], 0
        jmp    loc_5_0D1A
loc_5_0CE5:
        push   1
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x28]
        add    eax, 4
        push   eax
        call   sub_1_068E
loc_5_0CF7:
        and    eax, 0xff
        test   eax, eax
        je     loc_5_0D16
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x1d2], 1
        jmp    loc_5_0D1A
loc_5_0D16:
        mov    byte ptr [ebp - 0xc], 1
loc_5_0D1A:
        mov    eax, dword ptr [ebp - 0x58]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x2c]
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_1_05BC
loc_5_0D2F:
        cmp    dword ptr [ebp - 0x34], 0
        jne    loc_5_0D80
        cmp    dword ptr [ebp - 4], 6
        jbe    loc_5_0D4A
        mov    dword ptr [ebp - 4], 6
loc_5_0D4A:
        mov    dword ptr [ebp - 0x30], 0
        jmp    loc_5_0D59
loc_5_0D56:
        inc    dword ptr [ebp - 0x30]
loc_5_0D59:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 0x30], eax
        jae    loc_5_0D80
        mov    eax, dword ptr [ebp - 0x30]
        mov    ecx, dword ptr [ebp - 0x2c]
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx + 0x1ed], al
        jmp    loc_5_0D56
loc_5_0D80:
        push   0
        lea    eax, [ebp - 0x78]
        push   eax
        mov    eax, dword ptr [ebp - 0x58]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_1_0682
loc_5_0D97:
        cmp    dword ptr [ebp - 0x34], 0
        jne    loc_5_0DB0
        mov    eax, dword ptr [ebp - 0x28]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x1f3], eax
loc_5_0DB0:
        mov    eax, dword ptr [ebp - 0x58]
        push   eax
        call   sub_1_0688
loc_5_0DB9:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x1f3], 0x1194
        jbe    loc_5_0DD9
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x1f3], 0x1194
loc_5_0DD9:
        mov    al, byte ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_0126
        mov    dword ptr [ebp - 0x34], eax
        cmp    dword ptr [ebp - 0x34], 0
        je     loc_5_0E0B
        push   0
        push   0x2de
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0604
loc_5_0E0B:
        mov    eax, dword ptr [ebp - 0x34]
        jmp    loc_5_0E13
loc_5_0E13:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
loc_5_0E1A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1c], 1
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x23c
        push   eax
        call   sub_1_05E0
loc_5_0E46:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_05EC
loc_5_0E4F:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x5a]
        push   eax
        call   sub_2_00D0
loc_5_0E5B:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_4D48
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_0568
loc_5_0E70:
        push   0
        push   0x2de
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0604
loc_5_0E80:
        jmp    loc_5_0E85
loc_5_0E85:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_5_0E8C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_055C
loc_5_0EAB:
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_0550
loc_5_0EB8:
        jmp    loc_5_0EBD
loc_5_0EBD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_0EC4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x36
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x3a], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x3a]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x36], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2e
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x32], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x32]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x2e], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x3e
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x42], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x42]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x3e], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x248], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1d], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x7f], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x7a], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x7e], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x7c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x255], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x46], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x7d], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x80], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x84], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x88], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x72], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x22], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x26], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_0FAB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x62], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x66], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x5e], 0
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
        mov    dword ptr [ebp - 4], 0
        jmp    loc_5_0FEE
loc_5_0FEB:
        inc    dword ptr [ebp - 4]
loc_5_0FEE:
        mov    eax, dword ptr [ebp + 8]
        shl    eax, 2
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        cmp    eax, dword ptr [ebp - 4]
        jbe    loc_5_100F
        push   0xa
        call   sub_1_05F8
loc_5_100A:
        jmp    loc_5_0FEB
loc_5_100F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_5_1016:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0x103
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x7d]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_105A
        mov    dword ptr [ebp - 0xc], 0xc001000d
        jmp    loc_5_10B1
loc_5_105A:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x7e]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_1079
        mov    dword ptr [ebp - 0xc], 0xc0000001
        jmp    loc_5_10B1
loc_5_1079:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_10AA
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_121E
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_10B1
loc_5_10AA:
        mov    dword ptr [ebp - 0xc], 0xc0010002
loc_5_10B1:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x2a], 0
        jne    loc_5_10FD
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x7d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_10EF
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x10]
        add    ecx, 0x2e
        cmp    dword ptr [eax + 0x2e], ecx
        je     loc_5_10FD
loc_5_10EF:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_5_2F61
        jmp    loc_5_10FD
loc_5_10FD:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_5_1105
loc_5_1105:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_110C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
loc_5_1115:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x62], 0
        je     loc_5_1171
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x62]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x62], eax
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x80]
        push   0xc0000001
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_057A
loc_5_1166:
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_1115
loc_5_1171:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_1181
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_1181:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_0FAB
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x7d], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x7c], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x4a], 0
        je     loc_5_1217
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x4a]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x36]
        mov    dword ptr [ebp - 4], eax
loc_5_11C0:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x36
        cmp    eax, dword ptr [ebp - 4]
        je     loc_5_1203
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0
        push   0
        push   0x40010005
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_059E
loc_5_11EA:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_05A4
loc_5_11F6:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_5_11C0
loc_5_1203:
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_0586
loc_5_1217:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_121E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_5_1271
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x36]
        mov    dword ptr [ebp - 4], eax
loc_5_123A:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x36
        cmp    eax, dword ptr [ebp - 4]
        je     loc_5_1271
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0
        push   0
        push   0x40010004
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_059E
loc_5_1264:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_5_123A
loc_5_1271:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x7d], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x7c], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x4a], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x46], 0
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_5_12A2
        mov    eax, dword ptr [ebp + 0xc]
        inc    dword ptr [eax + 0x14]
loc_5_12A2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
        db  000h, 000h, 000h
sub_5_12AC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_5_166C
        test   ax, ax
        je     loc_5_12C7
        mov    ax, 1
        pop    esi
        pop    ebp
        ret    4
loc_5_12C7:
        push   esi
        call   sub_5_153C
        test   ax, ax
        je     loc_5_12DB
        mov    ax, 1
        pop    esi
        pop    ebp
        ret    4
loc_5_12DB:
        push   esi
        call   sub_5_171C
        cmp    ax, 1
        pop    esi
        sbb    eax, eax
        pop    ebp
        inc    eax
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_12FC:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 0x14d], ax
        mov    dword ptr [esi + 0x122], eax
        push   esi
        mov    dword ptr [esi + 0x11e], eax
        mov    dword ptr [esi + 0x12a], 1
        mov    dword ptr [esi + 0x126], 2
        call   sub_5_192C
        test   ax, ax
        je     loc_5_1340
        mov    ax, 1
        pop    esi
        pop    ebp
        ret    4
loc_5_1340:
        push   esi
        call   sub_5_193C
        test   ax, ax
        je     loc_5_1354
        mov    ax, 1
        pop    esi
        pop    ebp
        ret    4
loc_5_1354:
        push   esi
        call   sub_5_136C
        cmp    ax, 1
        pop    esi
        sbb    eax, eax
        pop    ebp
        inc    eax
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_5_136C:
        push   ebp
        mov    edx, 0                        ; -> loc_1_68A4
        mov    ebp, esp
        sub    esp, 0x14
        mov    eax, dword ptr [edx]
        push   esi
        push   edi
        lea    ecx, [ebp - 0x14]
        mov    esi, dword ptr [edx + 4]
        mov    edi, dword ptr [edx + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 0xc]
        mov    dx, word ptr [edx + 0x10]
        mov    dword ptr [ecx + 4], esi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], edi
        mov    dword ptr [ecx + 0xc], eax
        xor    eax, eax
        mov    word ptr [ecx + 0x10], dx
loc_5_139F:
        mov    dl, byte ptr [ebp + eax - 0x14]
        mov    ecx, dword ptr [esi + 0x10]
        inc    eax
        cmp    eax, 0x12
        mov    byte ptr [ecx + eax - 1], dl
        jb     loc_5_139F
        mov    eax, dword ptr [esi + 8]
        xor    edi, edi
        mov    word ptr [eax], di
        mov    eax, dword ptr [esi + 8]
        mov    dx, word ptr [esi + 0x161]
        mov    word ptr [eax + 2], dx
        mov    eax, dword ptr [esi + 8]
        mov    dx, word ptr [esi + 0x163]
        mov    word ptr [eax + 4], dx
        mov    eax, dword ptr [esi + 8]
        mov    dx, word ptr [esi + 0x165]
        mov    word ptr [eax + 6], dx
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 8], di
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 0xa], di
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 0xc], di
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 0xe], di
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 0x10], 0xe
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 0x12], 0x1a
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 0x14], 0x3f8
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 0x16], di
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 0x18], di
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [eax + 0x1a], 0x40c
        mov    eax, dword ptr [esi + 0x98]
        mov    ecx, dword ptr [esi + 8]
        shr    eax, 0x10
        push   2
        mov    word ptr [ecx + 0x1c], ax
        mov    eax, dword ptr [esi + 0x98]
        mov    ecx, dword ptr [esi + 8]
        mov    word ptr [ecx + 0x1e], ax
        mov    eax, dword ptr [esi + 8]
        add    eax, 2
        push   eax
        call   sub_5_18DC
        push   2
        mov    eax, dword ptr [esi + 8]
        add    eax, 4
        push   eax
        call   sub_5_18DC
        push   2
        mov    eax, dword ptr [esi + 8]
        add    eax, 6
        push   eax
        call   sub_5_18DC
        push   0x20
        mov    ecx, dword ptr [esi + 8]
        push   ecx
        call   sub_5_1D8C
        mov    dword ptr [esi + 0x126], 3
        push   esi
        call   sub_5_1A5C
        mov    dx, word ptr [esi + 0x118]
        mov    ecx, dword ptr [esi]
        mov    word ptr [ecx], dx
        mov    ecx, dword ptr [esi]
        mov    dx, word ptr [esi + 0x11a]
        push   0x9080
        mov    word ptr [ecx + 2], dx
        push   esi
        mov    ax, word ptr [esi + 0x11c]
        mov    ecx, dword ptr [esi]
        mov    word ptr [ecx + 4], ax
        call   sub_5_151C
loc_5_14C3:
        mov    eax, dword ptr [esi + 0x126]
        cmp    eax, 1
        je     loc_5_14F5
        cmp    eax, 5
        je     loc_5_1500
        cmp    eax, 4
        je     loc_5_150C
        push   0xa
        inc    edi
        push   esi
        call   sub_5_4FD0
        cmp    edi, 0x1194
        jb     loc_5_14C3
        mov    ax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_14F5:
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_1500:
        mov    ax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_150C:
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_5_151C:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x13a]
        add    ecx, 6
        push   ecx
        push   eax
        call   sub_5_502B
        pop    ebp
        ret    8
        db  090h
sub_5_153C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x136], 0
        mov    di, 0x100
        je     loc_5_1555
        mov    di, 0x140
loc_5_1555:
        cmp    dword ptr [esi + 0x13e], 0
        je     loc_5_1563
        or     di, 0x80
loc_5_1563:
        cmp    dword ptr [esi + 0x146], 0
        je     loc_5_1571
        or     di, 0x200
loc_5_1571:
        mov    eax, dword ptr [esi + 0x12e]
        sub    eax, 3
        cmp    eax, 0xc
        ja     loc_5_1586
        jmp    dword ptr [eax*4]             ; -> obj5:0x1630
loc_5_1586:
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_1590:
        or     di, 1
        jmp    loc_5_15D0
loc_5_1596:
        or     di, 2
        jmp    loc_5_15D0
loc_5_159C:
        or     di, 3
        jmp    loc_5_15D0
loc_5_15A2:
        or     di, 4
        jmp    loc_5_15D0
loc_5_15A8:
        or     di, 5
        jmp    loc_5_15D0
loc_5_15AE:
        or     di, 6
        jmp    loc_5_15D0
loc_5_15B4:
        or     di, 7
        jmp    loc_5_15D0
loc_5_15BA:
        or     di, 8
        jmp    loc_5_15D0
loc_5_15C0:
        or     di, 9
        jmp    loc_5_15D0
loc_5_15C6:
        or     di, 0xa
        jmp    loc_5_15D0
loc_5_15CC:
        or     di, 0xb
loc_5_15D0:
        mov    eax, dword ptr [esi + 0x142]
        cmp    eax, 5
        je     loc_5_15EF
        cmp    eax, 6
        je     loc_5_15F5
        cmp    eax, 7
        je     loc_5_15FB
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_15EF:
        or     di, 0x10
        jmp    loc_5_15FF
loc_5_15F5:
        or     di, 0x20
        jmp    loc_5_15FF
loc_5_15FB:
        or     di, 0x30
loc_5_15FF:
        push   edi
        mov    eax, dword ptr [esi + 0x13a]
        and    di, 0xfeff
        add    eax, 0x10
        push   eax
        push   esi
        call   sub_5_502B
        push   edi
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 0x10
        push   eax
        push   esi
        call   sub_5_502B
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
        ; ---- dispatch table (13 entries) ----
        dd  loc_5_1590
        dd  loc_5_1596
        dd  loc_5_159C
        dd  loc_5_15A2
        dd  loc_5_15A8
        dd  loc_5_1586
        dd  loc_5_15AE
        dd  loc_5_15B4
        dd  loc_5_15BA
        dd  loc_5_15C0
        dd  loc_5_1586
        dd  loc_5_15C6
        dd  loc_5_15CC
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_5_166C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 0xc
        push   eax
        push   esi
        call   sub_5_502B
        push   0
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 4
        xor    edi, edi
        push   eax
        push   esi
        call   sub_5_502B
loc_5_169A:
        mov    eax, dword ptr [esi + 0x13a]
        inc    edi
        add    eax, 2
        push   eax
        push   esi
        call   sub_5_5087
        mov    byte ptr [esi + edi + 0x15a], ah
        cmp    edi, 6
        jb     loc_5_169A
        cmp    byte ptr [esi + 0x161], 0
        jne    loc_5_1709
        cmp    byte ptr [esi + 0x162], 0
        jne    loc_5_1709
        cmp    byte ptr [esi + 0x163], 0
        jne    loc_5_1709
        cmp    byte ptr [esi + 0x164], 0
        jne    loc_5_1709
        cmp    byte ptr [esi + 0x165], 0
        jne    loc_5_1709
        cmp    byte ptr [esi + 0x166], 0
        jne    loc_5_1709
        xor    cx, cx
loc_5_16F0:
        movsx  eax, cx
        inc    cx
        add    eax, esi
        cmp    cx, 6
        mov    dl, byte ptr [eax + 0x15b]
        mov    byte ptr [eax + 0x161], dl
        jl     loc_5_16F0
loc_5_1709:
        mov    al, byte ptr [esi + 0x15d]
        pop    edi
        sub    al, 0xb8
        pop    esi
        pop    ebp
        cmp    al, 1
        sbb    eax, eax
        inc    eax
        ret    4
sub_5_171C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 8
        push   eax
        push   esi
        call   sub_5_5087
        and    ax, 0x107c
        mov    edi, 0                        ; -> obj1:0x0860
        push   eax
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 8
        push   eax
        push   esi
        call   sub_5_502B
loc_5_1752:
        mov    eax, edi
        add    edi, 2
        mov    ax, word ptr [eax]
        push   eax
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 0xc
        push   eax
        push   esi
        call   sub_5_502B
        mov    ax, word ptr [edi]
        mov    ecx, edi
        add    edi, 2
        push   eax
        mov    eax, dword ptr [esi + 0x13a]
        add    edi, 2
        add    eax, 4
        push   eax
        push   esi
        call   sub_5_502B
        mov    ax, word ptr [edi - 2]
        test   ax, ax
        je     loc_5_17B8
        jbe    loc_5_1752
        mov    ebx, eax
        and    ebx, 0xffff
loc_5_179A:
        mov    eax, edi
        add    edi, 2
        mov    ax, word ptr [eax]
        push   eax
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 2
        push   eax
        push   esi
        call   sub_5_502B
        dec    ebx
        jne    loc_5_179A
        jmp    loc_5_1752
loc_5_17B8:
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 8
        push   eax
        push   esi
        call   sub_5_5087
        and    ax, 0xffbf
        push   eax
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 8
        xor    edi, edi
        push   eax
        push   esi
        call   sub_5_502B
        mov    word ptr [ebp - 2], 1
loc_5_17E5:
        xor    ebx, ebx
loc_5_17E7:
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 6
        push   eax
        push   esi
        call   sub_5_5087
        and    al, 0xf0
        cmp    al, 0x40
        je     loc_5_1810
        push   0xa
        inc    ebx
        push   esi
        call   sub_5_4FD0
        cmp    ebx, 0x12c
        jb     loc_5_17E7
        jmp    loc_5_1816
loc_5_1810:
        mov    word ptr [ebp - 2], 0
loc_5_1816:
        cmp    word ptr [ebp - 2], 0
        je     loc_5_1838
        push   0xff00
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 6
        inc    edi
        push   eax
        push   esi
        call   sub_5_502B
        cmp    edi, 3
        jb     loc_5_17E5
loc_5_1838:
        mov    ax, word ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_184C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0xff00
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 6
        xor    edi, edi
        push   eax
        push   esi
        call   sub_5_502B
        mov    word ptr [ebp - 2], 1
loc_5_1875:
        xor    ebx, ebx
loc_5_1877:
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 6
        push   eax
        push   esi
        call   sub_5_5087
        and    al, 0xf0
        cmp    al, 0x40
        je     loc_5_18A0
        push   0xa
        inc    ebx
        push   esi
        call   sub_5_4FD0
        cmp    ebx, 0x12c
        jb     loc_5_1877
        jmp    loc_5_18A6
loc_5_18A0:
        mov    word ptr [ebp - 2], 0
loc_5_18A6:
        cmp    word ptr [ebp - 2], 0
        je     loc_5_18C8
        push   0xff00
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 6
        inc    edi
        push   eax
        push   esi
        call   sub_5_502B
        cmp    edi, 3
        jb     loc_5_1875
loc_5_18C8:
        mov    ax, word ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_18DC:
        push   ebp
        mov    al, byte ptr [esp + 0xc]
        mov    ebp, esp
        cmp    al, 1
        je     loc_5_1919
        cmp    al, 2
        jne    loc_5_18FC
        mov    edx, dword ptr [ebp + 8]
        pop    ebp
        mov    al, byte ptr [edx + 1]
        mov    cl, byte ptr [edx]
        mov    byte ptr [edx + 1], cl
        mov    byte ptr [edx], al
        ret    8
loc_5_18FC:
        cmp    al, 4
        jne    loc_5_1919
        mov    edx, dword ptr [ebp + 8]
        mov    al, byte ptr [edx]
        mov    cl, byte ptr [edx + 3]
        mov    byte ptr [edx], cl
        mov    byte ptr [edx + 3], al
        mov    al, byte ptr [edx + 1]
        mov    cl, byte ptr [edx + 2]
        mov    byte ptr [edx + 1], cl
        mov    byte ptr [edx + 2], al
loc_5_1919:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_192C:
        xor    ax, ax
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_193C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ecx, ecx
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xc]
        mov    word ptr [eax], 0x9f
        mov    eax, dword ptr [esi + 0xc]
        mov    byte ptr [eax + 2], cl
        mov    eax, dword ptr [esi + 0xc]
        mov    byte ptr [eax + 3], cl
        mov    eax, dword ptr [esi + 0xc]
        mov    byte ptr [eax + 4], cl
        mov    eax, dword ptr [esi + 0xc]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [esi + 0xc]
        mov    byte ptr [eax + 6], cl
        mov    eax, dword ptr [esi + 0xc]
        mov    byte ptr [eax + 7], cl
        mov    eax, dword ptr [esi + 0xc]
        mov    word ptr [eax + 8], cx
        mov    eax, dword ptr [esi + 0xc]
        mov    word ptr [eax + 0xa], cx
        mov    eax, dword ptr [esi + 0xc]
        mov    word ptr [eax + 0xc], 0x505
        mov    edx, dword ptr [esi + 0x88]
        mov    eax, dword ptr [esi + 0xc]
        push   0x16
        mov    dword ptr [eax + 0xe], edx
        mov    ebx, dword ptr [esi + 0x8c]
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [eax + 0x12], ebx
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        call   sub_5_1D8C
        push   4
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xe
        push   eax
        call   sub_5_18DC
        push   4
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0x12
        push   eax
        call   sub_5_18DC
        push   1
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 0xc
        push   eax
        push   esi
        call   sub_5_502B
        push   0xa00
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 4
        push   eax
        push   esi
        call   sub_5_502B
        mov    ebx, dword ptr [esi + 0xc]
        mov    edi, 0xb
loc_5_19F8:
        mov    eax, ebx
        add    ebx, 2
        mov    ax, word ptr [eax]
        push   eax
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 2
        push   eax
        push   esi
        call   sub_5_502B
        dec    edi
        jne    loc_5_19F8
        push   0x9080
        push   esi
        call   sub_5_151C
loc_5_1A1F:
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 6
        push   eax
        push   esi
        call   sub_5_5087
        test   al, 0x70
        je     loc_5_1A50
        test   al, 0x10
        je     loc_5_1A1F
        mov    ax, 1
loc_5_1A3B:
        mov    ecx, dword ptr [esi]
        cmp    word ptr [ecx], 0
        jne    loc_5_1A49
        cmp    word ptr [ecx + 2], 0xe2c1
loc_5_1A49:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_1A50:
        xor    ax, ax
        jmp    loc_5_1A3B
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1A5C:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        lea    edx, [ecx + 0x118]
        mov    eax, dword ptr [ecx + 0x90]
        mov    word ptr [edx], 3
        mov    esi, eax
        mov    word ptr [ecx + 0x11c], ax
        push   6
        shr    esi, 0x10
        push   edx
        mov    word ptr [ecx + 0x11a], si
        call   sub_5_1D8C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1A9C:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0x157], 0
        lea    edi, [esi + 0x18]
        mov    edx, edi
loc_5_1AB6:
        cmp    ecx, 3
        jae    loc_5_1AD5
        mov    eax, dword ptr [edx + 0x8c]
        mov    ebx, dword ptr [edx]
        shr    eax, 0x10
        mov    word ptr [ebx], ax
        mov    eax, dword ptr [edx + 0x8c]
        mov    ebx, dword ptr [edx]
        mov    word ptr [ebx + 2], ax
loc_5_1AD5:
        mov    eax, dword ptr [edx]
        add    edx, 4
        mov    word ptr [eax + 4], 0
        inc    ecx
        cmp    ecx, 4
        jb     loc_5_1AB6
        mov    eax, dword ptr [esi + 0xa0]
        mov    ecx, dword ptr [esi + 0x24]
        shr    eax, 0x10
        mov    ebx, 4
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [esi + 0xa0]
        mov    ecx, dword ptr [esi + 0x24]
        mov    word ptr [ecx + 2], ax
loc_5_1B07:
        mov    eax, dword ptr [edi]
        push   0x1a
        push   eax
        add    edi, 4
        call   sub_5_1D8C
        dec    ebx
        jne    loc_5_1B07
        lea    ecx, [esi + 0x118]
        mov    eax, dword ptr [esi + 0xa0]
        mov    word ptr [ecx], 4
        mov    edx, eax
        mov    word ptr [esi + 0x11c], ax
        push   6
        shr    edx, 0x10
        push   ecx
        mov    word ptr [esi + 0x11a], dx
        call   sub_5_1D8C
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_5_1B4C:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0x153], 0
        lea    edi, [esi + 0x28]
        mov    edx, edi
loc_5_1B66:
        cmp    ecx, 9
        jae    loc_5_1B85
        mov    eax, dword ptr [edx + 0x8c]
        mov    ebx, dword ptr [edx]
        shr    eax, 0x10
        mov    word ptr [ebx], ax
        mov    eax, dword ptr [edx + 0x8c]
        mov    ebx, dword ptr [edx]
        mov    word ptr [ebx + 2], ax
loc_5_1B85:
        mov    eax, dword ptr [edx]
        add    edx, 4
        mov    word ptr [eax + 4], 0x8800
        mov    eax, dword ptr [edx - 4]
        inc    ecx
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [edx - 4]
        mov    word ptr [eax + 8], 0x1194
        mov    eax, dword ptr [edx + 0xac]
        mov    ebx, dword ptr [edx - 4]
        shr    eax, 0x10
        mov    word ptr [ebx + 0xa], ax
        mov    eax, dword ptr [edx + 0xac]
        mov    ebx, dword ptr [edx - 4]
        mov    word ptr [ebx + 0xc], ax
        mov    ebx, dword ptr [edx - 4]
        mov    word ptr [ebx + 0xe], 0
        mov    eax, dword ptr [edx + 0xac]
        mov    ebx, dword ptr [edx - 4]
        shr    eax, 0x10
        mov    word ptr [ebx + 0x10], ax
        mov    eax, dword ptr [edx + 0xac]
        mov    ebx, dword ptr [edx - 4]
        mov    word ptr [ebx + 0x12], ax
        mov    ebx, dword ptr [edx - 4]
        mov    word ptr [ebx + 0x14], 0
        mov    eax, dword ptr [edx + 0xac]
        mov    ebx, dword ptr [edx - 4]
        shr    eax, 0x10
        cmp    ecx, 0xa
        mov    word ptr [ebx + 0x16], ax
        mov    eax, dword ptr [edx + 0xac]
        mov    ebx, dword ptr [edx - 4]
        mov    word ptr [ebx + 0x18], ax
        jb     loc_5_1B66
        mov    eax, dword ptr [esi + 0xb0]
        mov    ecx, dword ptr [esi + 0x4c]
        shr    eax, 0x10
        mov    ebx, 0xa
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [esi + 0xb0]
        mov    ecx, dword ptr [esi + 0x4c]
        mov    word ptr [ecx + 2], ax
loc_5_1C36:
        mov    eax, dword ptr [edi]
        push   0x1a
        push   eax
        add    edi, 4
        call   sub_5_1D8C
        dec    ebx
        jne    loc_5_1C36
        lea    ecx, [esi + 0x118]
        mov    eax, dword ptr [esi + 0xb0]
        mov    word ptr [ecx], 6
        mov    edx, eax
        mov    word ptr [esi + 0x11c], ax
        push   6
        shr    edx, 0x10
        push   ecx
        mov    word ptr [esi + 0x11a], dx
        call   sub_5_1D8C
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_5_1C7C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        lea    edx, [eax + 0x118]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [edx], 9
        mov    esi, ecx
        mov    word ptr [eax + 0x11c], cx
        push   6
        shr    esi, 0x10
        push   edx
        mov    word ptr [eax + 0x11a], si
        call   sub_5_1D8C
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1CBC:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        lea    edx, [eax + 0x118]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [edx], 8
        mov    esi, ecx
        mov    word ptr [eax + 0x11c], cx
        push   6
        shr    esi, 0x10
        push   edx
        mov    word ptr [eax + 0x11a], si
        call   sub_5_1D8C
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1CFC:
        push   ebp
        mov    ax, word ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        push   6
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 0x11a], ax
        lea    ecx, [esi + 0x118]
        mov    word ptr [ecx], 0xd
        push   ecx
        mov    word ptr [esi + 0x11c], 0
        call   sub_5_1D8C
        mov    cx, word ptr [esi + 0x11a]
        mov    edx, dword ptr [esi + 8]
        mov    word ptr [edx], cx
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1D4C:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        lea    ecx, [edx + 0x118]
        mov    eax, dword ptr [edx + 0x9c]
        mov    word ptr [ecx], 0xa
        mov    esi, eax
        mov    word ptr [edx + 0x11c], ax
        push   6
        shr    esi, 0x10
        push   ecx
        mov    word ptr [edx + 0x11a], si
        call   sub_5_1D8C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1D8C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        test   edi, 0xfffffffe
        jbe    loc_5_1DB2
        shr    edi, 1
        mov    esi, dword ptr [ebp + 8]
loc_5_1DA2:
        mov    eax, esi
        push   2
        add    esi, 2
        push   eax
        call   sub_5_18DC
        dec    edi
        jne    loc_5_1DA2
loc_5_1DB2:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_5_1DBC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x13a]
        add    eax, 6
        push   eax
        push   esi
        call   sub_5_5087
        test   al, 0x80
        jne    loc_5_1DDF
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
loc_5_1DDF:
        and    ax, 0xf
        mov    word ptr [esi + 0x185], ax
        pop    esi
        mov    ax, 1
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1DFC:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   0
        push   eax
        call   sub_5_151C
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1E1C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   0xa080
        push   eax
        call   sub_5_151C
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1E3C:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 0x153]
        mov    edx, dword ptr [ecx + eax*4 + 0x28]
        mov    word ptr [edx + 4], 0x88
        mov    eax, dword ptr [ecx + 0x153]
        inc    eax
        mov    dword ptr [ecx + 0x153], eax
        cmp    eax, 0xa
        jb     loc_5_1E6F
        mov    dword ptr [ecx + 0x153], 0
loc_5_1E6F:
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_5_1E7C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x153]
        mov    eax, dword ptr [esi + eax*4 + 0x28]
        test   byte ptr [eax + 4], 0x80
        je     loc_5_1E9C
        mov    ax, 0xffff
        pop    esi
        pop    ebp
        ret    4
loc_5_1E9C:
        push   2
        add    eax, 6
        push   eax
        call   sub_5_18DC
        mov    ecx, dword ptr [esi + 0x153]
        mov    edx, dword ptr [esi + ecx*4 + 0x28]
        lea    ecx, [esi + ecx*4]
        mov    dx, word ptr [edx + 6]
        mov    ecx, dword ptr [ecx + 0x50]
        mov    ax, dx
        mov    dword ptr [esi + 0x187], ecx
        inc    dword ptr [esi + 0x16f]
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1EDC:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   0x8280
        push   eax
        call   sub_5_151C
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1EFC:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0x157]
        mov    edx, dword ptr [eax + ecx*4 + 0x18]
        pop    ebp
        mov    ax, word ptr [edx + 4]
        and    ax, 0x80
        cmp    ax, 1
        sbb    eax, eax
        and    eax, 2
        dec    eax
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1F2C:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 0x157]
        pop    ebp
        mov    edx, dword ptr [ecx + eax*4 + 0x78]
        xor    ax, ax
        mov    dword ptr [ecx + 0x18b], edx
        mov    word ptr [ecx + 0x18f], 0
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1F5C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    dx, word ptr [esp + 0x10]
        mov    ebp, esp
        test   dx, dx
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0x18b]
        je     loc_5_1F86
        mov    di, dx
        mov    ecx, dword ptr [ebp + 0xc]
loc_5_1F7C:
        mov    bl, byte ptr [ecx]
        inc    ecx
        mov    byte ptr [esi], bl
        inc    esi
        dec    di
        jne    loc_5_1F7C
loc_5_1F86:
        add    word ptr [eax + 0x18f], dx
        pop    edi
        mov    dword ptr [eax + 0x18b], esi
        pop    esi
        xor    ax, ax
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1FAC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 0x18f]
        mov    eax, dword ptr [esi + 0x157]
        mov    edx, dword ptr [esi + eax*4 + 0x18]
        mov    word ptr [edx + 6], cx
        mov    cx, word ptr [esi + 0x18f]
        mov    eax, dword ptr [esi + 0x157]
        mov    edx, dword ptr [esi + eax*4 + 0x18]
        mov    word ptr [edx + 8], cx
        mov    eax, dword ptr [esi + 0x157]
        mov    ecx, dword ptr [esi + eax*4 + 0x100]
        lea    eax, [esi + eax*4]
        shr    ecx, 0x10
        mov    eax, dword ptr [eax + 0x18]
        mov    word ptr [eax + 0xa], cx
        mov    edx, dword ptr [esi + 0x157]
        mov    ecx, dword ptr [esi + edx*4 + 0x100]
        lea    eax, [esi + edx*4]
        mov    edx, dword ptr [esi + edx*4 + 0x18]
        mov    word ptr [edx + 0xc], cx
        xor    ecx, ecx
        mov    eax, dword ptr [esi + 0x157]
        mov    edx, dword ptr [esi + eax*4 + 0x18]
        mov    word ptr [edx + 0xe], cx
        mov    eax, dword ptr [esi + 0x157]
        mov    edx, dword ptr [esi + eax*4 + 0x18]
        mov    word ptr [edx + 0x10], cx
        mov    eax, dword ptr [esi + 0x157]
        mov    edx, dword ptr [esi + eax*4 + 0x18]
        mov    word ptr [edx + 0x12], cx
        mov    eax, dword ptr [esi + 0x157]
        mov    edx, dword ptr [esi + eax*4 + 0x18]
        mov    word ptr [edx + 0x14], cx
        push   2
        mov    eax, dword ptr [esi + 0x157]
        mov    edx, dword ptr [esi + eax*4 + 0x18]
        mov    word ptr [edx + 0x16], cx
        mov    eax, dword ptr [esi + 0x157]
        mov    edx, dword ptr [esi + eax*4 + 0x18]
        mov    word ptr [edx + 0x18], cx
        mov    eax, dword ptr [esi + 0x157]
        mov    eax, dword ptr [esi + eax*4 + 0x18]
        add    eax, 6
        push   eax
        call   sub_5_18DC
        push   2
        mov    ecx, dword ptr [esi + 0x157]
        mov    eax, dword ptr [esi + ecx*4 + 0x18]
        add    eax, 8
        push   eax
        call   sub_5_18DC
        push   2
        mov    ecx, dword ptr [esi + 0x157]
        mov    eax, dword ptr [esi + ecx*4 + 0x18]
        add    eax, 0xa
        push   eax
        call   sub_5_18DC
        push   2
        mov    ecx, dword ptr [esi + 0x157]
        mov    eax, dword ptr [esi + ecx*4 + 0x18]
        add    eax, 0xc
        push   eax
        call   sub_5_18DC
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_20CC:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0x8180
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 0x157]
        mov    ecx, dword ptr [esi + eax*4 + 0x18]
        mov    word ptr [ecx + 4], 0xb8
        call   sub_5_151C
        mov    eax, dword ptr [esi + 0x157]
        inc    eax
        mov    dword ptr [esi + 0x157], eax
        cmp    eax, 4
        jb     loc_5_210A
        mov    dword ptr [esi + 0x157], 0
loc_5_210A:
        xor    ax, ax
        inc    dword ptr [esi + 0x16b]
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_5_211C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   0xa
        lea    eax, [ebp - 4]
        push   eax
        call   sub_5_5131
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + 0x118]
        mov    word ptr [edx], ax
        mov    edx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + 0x11a]
        mov    word ptr [edx + 2], ax
        mov    edx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + 0x11c]
        mov    word ptr [edx + 4], ax
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 6], 0
        cmp    dword ptr [ecx + 0x122], 0
        mov    eax, dword ptr [ebp - 4]
        jne    loc_5_2184
        mov    dword ptr [ecx + 0x122], eax
        mov    dword ptr [ecx + 0x11e], eax
        xor    ax, ax
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_2184:
        mov    edx, dword ptr [ecx + 0x11e]
        mov    dword ptr [edx + 6], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x11e], eax
        mov    esp, ebp
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_21AC:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi + 0x14d], 1
        jne    loc_5_21C7
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_21C7:
        mov    word ptr [esi + 0x14d], 1
        mov    ecx, dword ptr [esi]
        cmp    word ptr [ecx], 0
        je     loc_5_21EA
        xor    ax, ax
        pop    edi
        mov    word ptr [esi + 0x14d], 0
        pop    esi
        pop    ebp
        ret    4
loc_5_21EA:
        mov    edi, dword ptr [esi + 0x122]
        test   edi, edi
        jne    loc_5_2206
        xor    ax, ax
        pop    edi
        mov    word ptr [esi + 0x14d], 0
        pop    esi
        pop    ebp
        ret    4
loc_5_2206:
        mov    eax, dword ptr [edi + 6]
        mov    dword ptr [esi + 0x122], eax
        mov    dx, word ptr [edi]
        mov    word ptr [ecx], dx
        mov    ecx, dword ptr [esi]
        mov    ax, word ptr [edi + 2]
        push   0x9080
        mov    word ptr [ecx + 2], ax
        push   esi
        mov    dx, word ptr [edi + 4]
        mov    ecx, dword ptr [esi]
        mov    word ptr [ecx + 4], dx
        call   sub_5_151C
        push   0xa
        push   edi
        call   sub_5_515B
        mov    word ptr [esi + 0x14d], 0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
sub_5_224C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    di, di
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    dx, word ptr [eax]
        mov    word ptr [esi + 0x110], dx
        mov    cx, word ptr [eax + 2]
        mov    word ptr [esi + 0x112], cx
        mov    bx, word ptr [eax + 4]
        mov    word ptr [esi + 0x114], bx
        mov    ax, word ptr [eax + 6]
        mov    word ptr [esi + 0x116], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 0x185]
        cmp    eax, 0xc
        ja     loc_5_2414
        xor    ebx, ebx
        mov    bl, byte ptr [eax]            ; -> obj5:0x2438
        jmp    dword ptr [ebx*4]             ; -> obj5:0x2420
loc_5_22A7:
        mov    dword ptr [esi + 0x12a], 2
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_22BB:
        test   cl, 8
        je     loc_5_22DE
        mov    dword ptr [esi + 0x126], 4
        mov    dword ptr [esi + 0x12a], 3
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_22DE:
        test   cl, 4
        je     loc_5_2301
        mov    dword ptr [esi + 0x126], 4
        mov    dword ptr [esi + 0x12a], 3
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_2301:
        test   cl, 1
        je     loc_5_2414
        mov    dword ptr [esi + 0x126], 4
        mov    dword ptr [esi + 0x12a], 3
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_2328:
        mov    eax, edx
        and    eax, 0xffff
        cmp    eax, 0x400
        jg     loc_5_2414
        je     loc_5_23E2
        cmp    eax, 0x300
        jne    loc_5_2414
        cmp    cx, 0x80
        jne    loc_5_23A4
        mov    dword ptr [esi + 0x126], 1
        mov    eax, dword ptr [esi + 0x17b]
        push   eax
        push   esi
        call   sub_5_1C7C
        push   esi
        call   sub_5_211C
        mov    eax, dword ptr [esi + 0x17f]
        push   eax
        push   esi
        call   sub_5_1CBC
        push   esi
        call   sub_5_211C
        push   esi
        call   sub_5_1A9C
        push   esi
        call   sub_5_211C
        push   esi
        call   sub_5_1B4C
        push   esi
        call   sub_5_211C
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_23A4:
        cmp    dword ptr [esi + 0x126], 3
        mov    dword ptr [esi + 0x126], 4
        push   esi
        jne    loc_5_23CD
        call   sub_5_1A5C
        push   esi
        call   sub_5_211C
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_23CD:
        call   sub_5_1A5C
        push   esi
        call   sub_5_211C
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_23E2:
        mov    di, 8
        push   esi
        call   sub_5_1A9C
        push   esi
        call   sub_5_211C
        mov    ax, di
        inc    dword ptr [esi + 0x173]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_2402:
        mov    di, 1
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_5_2410:
        mov    di, 2
loc_5_2414:
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
        ; ---- dispatch table (6 entries) ----
        dd  loc_5_22A7
        dd  loc_5_22BB
        dd  loc_5_2328
        dd  loc_5_2402
        dd  loc_5_2410
        dd  loc_5_2414
        db  000h, 005h, 005h, 005h, 001h, 005h, 005h, 005h, 002h, 005h, 003h, 005h, 004h, 08Dh, 0A4h, 024h
        db  000h, 000h, 000h, 000h
sub_5_244C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x12a]
        cmp    eax, 2
        jne    loc_5_2498
        mov    dword ptr [esi + 0x12a], 1
        push   esi
        call   sub_5_184C
        push   esi
        call   sub_5_193C
        mov    dword ptr [esi + 0x126], 4
        push   esi
        call   sub_5_1A5C
        push   esi
        call   sub_5_211C
        push   esi
        call   sub_5_21AC
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
loc_5_2498:
        cmp    eax, 3
        jne    loc_5_24B9
        mov    dword ptr [esi + 0x12a], 1
        push   esi
        call   sub_5_1A5C
        push   esi
        call   sub_5_211C
        push   esi
        call   sub_5_21AC
loc_5_24B9:
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_24CC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0x103
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0628
loc_5_24F8:
        cmp    dword ptr [ebp - 8], 0xe
        jb     loc_5_2514
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x1f3], ecx
        jae    loc_5_251E
loc_5_2514:
        mov    eax, 0xc001000f
        jmp    loc_5_266E
loc_5_251E:
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x7c]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_25C2
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_25B6
        mov    eax, dword ptr [ebp - 0x10]
        inc    dword ptr [eax + 0x14]
        lea    eax, [ebp - 0x14]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0628
loc_5_2569:
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_5_2585
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0x14]
        cmp    dword ptr [eax + 0x1f3], ecx
        jae    loc_5_2591
loc_5_2585:
        mov    dword ptr [ebp - 0xc], 0xc000009a
        jmp    loc_5_25AB
loc_5_2591:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_2675
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_27AF
loc_5_25AB:
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_25BD
loc_5_25B6:
        mov    dword ptr [ebp - 0xc], 0xc0010002
loc_5_25BD:
        jmp    loc_5_261A
loc_5_25C2:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x7d]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_25E1
        mov    dword ptr [ebp - 0xc], 0xc001000d
        jmp    loc_5_261A
loc_5_25E1:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x7e]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_2600
        mov    dword ptr [ebp - 0xc], 0xc0000001
        jmp    loc_5_261A
loc_5_2600:
        push   2
        push   0xb
        push   2
        push   0xaa000006
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_06D0
loc_5_2617:
        add    esp, 0x14
loc_5_261A:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        dec    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [ebp - 0x18]
        cmp    dword ptr [eax + 0x2a], 0
        jne    loc_5_2666
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + 0x7d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_2658
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x18]
        add    ecx, 0x2e
        cmp    dword ptr [eax + 0x2e], ecx
        je     loc_5_2666
loc_5_2658:
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_5_2F61
        jmp    loc_5_2666
loc_5_2666:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_5_266E
loc_5_266E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_5_2675:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x80]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x62], 0
        jne    loc_5_26BD
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x62], eax
        jmp    loc_5_26C9
loc_5_26BD:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x66]
        mov    dword ptr [ecx + 0x24], eax
loc_5_26C9:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x66], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0xc]
        inc    dword ptr [eax + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_5_26E8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x18], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_1F2C
        push   0
        lea    eax, [ebp - 0x10]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0628
loc_5_2719:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_5_2762
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_064C
loc_5_2734:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_1F5C
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ebp - 0x18], eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0670
loc_5_275D:
        jmp    loc_5_2719
loc_5_2762:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_1FAC
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_20CC
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x14], eax
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_057A
loc_5_27A2:
        mov    eax, dword ptr [ebp - 0x14]
        dec    dword ptr [eax + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_5_27AF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x255]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_5_27D4
        jmp    loc_5_292F
loc_5_27D4:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x255], 1
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1b2], 1
        jne    loc_5_289C
        mov    dword ptr [ebp - 4], 0
        jmp    loc_5_27FD
loc_5_27FA:
        inc    dword ptr [ebp - 4]
loc_5_27FD:
        cmp    dword ptr [ebp - 4], 4
        jae    loc_5_2897
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_1EFC
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        cmp    eax, -1
        jne    loc_5_2835
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x255], 0
        jmp    loc_5_292F
loc_5_2835:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x62], 0
        je     loc_5_2883
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x62]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 0x24
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x62], eax
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x80]
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x5e], eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_26E8
        jmp    loc_5_2892
loc_5_2883:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x255], 0
        jmp    loc_5_292F
loc_5_2892:
        jmp    loc_5_27FA
loc_5_2897:
        jmp    loc_5_2925
loc_5_289C:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_5_28AB
loc_5_28A8:
        inc    dword ptr [ebp - 4]
loc_5_28AB:
        cmp    dword ptr [ebp - 4], 4
        jae    loc_5_2925
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x62], 0
        je     loc_5_2911
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x62]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, 0x24
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x62], eax
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x80]
        push   0xc0000001
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_057A
loc_5_2906:
        mov    eax, dword ptr [ebp - 0x1c]
        dec    dword ptr [eax + 0x14]
        jmp    loc_5_2920
loc_5_2911:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x255], 0
        jmp    loc_5_292F
loc_5_2920:
        jmp    loc_5_28A8
loc_5_2925:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x255], 0
loc_5_292F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        db  000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_5_2960
        jmp    loc_5_2A75
loc_5_2960:
        push   0
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0628
loc_5_2975:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_5_2984
        jmp    loc_5_2A75
loc_5_2984:
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_064C
loc_5_2995:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp + 0x10], eax
        jbe    loc_5_2A6D
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_5_29DD
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_0670
loc_5_29B8:
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_5_29C7
        jmp    loc_5_2A6D
loc_5_29C7:
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_064C
loc_5_29D8:
        jmp    loc_5_2995
loc_5_29DD:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_5_2A25
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0xc], eax
        jbe    loc_5_2A0E
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0x10]
        neg    eax
        sub    dword ptr [ebp + 0xc], eax
        mov    dword ptr [ebp - 0x10], 0
        jmp    loc_5_2995
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_2A0E:
        mov    eax, dword ptr [ebp + 0xc]
        add    dword ptr [ebp - 8], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 0xc]
        neg    eax
        sub    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp + 0xc], 0
loc_5_2A25:
        mov    eax, dword ptr [ebp + 0x10]
        sub    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    eax, ecx
        jb     loc_5_2A38
        mov    eax, ecx
loc_5_2A38:
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        call   sub_1_06E4
loc_5_2A4C:
        mov    eax, dword ptr [ebp - 0x18]
        add    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [ebp - 0x18]
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0x18]
        add    dword ptr [ebp - 0xc], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0x18]
        neg    eax
        sub    dword ptr [ebp - 0x10], eax
        jmp    loc_5_2995
loc_5_2A6D:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx], eax
loc_5_2A75:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
sub_5_2A7C:
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
        jne    loc_5_2AA4
        jmp    loc_5_2BC5
loc_5_2AA4:
        push   0
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_0628
loc_5_2AB9:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_2AC8
        jmp    loc_5_2BC5
loc_5_2AC8:
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_064C
loc_5_2AD9:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
loc_5_2ADF:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0xc], eax
        jbe    loc_5_2BBD
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_2B27
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_0670
loc_5_2B02:
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_5_2B11
        jmp    loc_5_2BBD
loc_5_2B11:
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_064C
loc_5_2B22:
        jmp    loc_5_2ADF
loc_5_2B27:
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_5_2B6F
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp + 0x14], eax
        jbe    loc_5_2B58
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0xc]
        neg    eax
        sub    dword ptr [ebp + 0x14], eax
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_5_2ADF
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_2B58:
        mov    eax, dword ptr [ebp + 0x14]
        add    dword ptr [ebp - 0x14], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 0x14]
        neg    eax
        sub    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp + 0x14], 0
loc_5_2B6F:
        mov    eax, dword ptr [ebp + 0xc]
        sub    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp - 0x1c]
        cmp    eax, ecx
        jb     loc_5_2B8E
        mov    eax, ecx
loc_5_2B8E:
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_06E4
loc_5_2BA2:
        mov    eax, dword ptr [ebp - 0x1c]
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x1c]
        add    dword ptr [ebp - 0x10], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0x1c]
        neg    eax
        sub    dword ptr [ebp - 0xc], eax
        jmp    loc_5_2ADF
loc_5_2BBD:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx], eax
loc_5_2BC5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x14], 0
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_5_2BF4
        jmp    loc_5_2DE5
loc_5_2BF4:
        push   0
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0628
loc_5_2C09:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_2C18
        jmp    loc_5_2DE5
loc_5_2C18:
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_064C
loc_5_2C29:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        call   sub_1_0628
loc_5_2C3E:
        cmp    dword ptr [ebp - 0x20], 0
        jne    loc_5_2C4D
        jmp    loc_5_2DE5
loc_5_2C4D:
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_064C
loc_5_2C5E:
        mov    eax, dword ptr [ebp - 0x14]
        cmp    dword ptr [ebp + 0x10], eax
        jbe    loc_5_2DDD
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_5_2CA6
        lea    eax, [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_0670
loc_5_2C81:
        cmp    dword ptr [ebp - 0x24], 0
        jne    loc_5_2C90
        jmp    loc_5_2DDD
loc_5_2C90:
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_064C
loc_5_2CA1:
        jmp    loc_5_2C5E
loc_5_2CA6:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_5_2CE2
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0670
loc_5_2CBD:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_2CCC
        jmp    loc_5_2DDD
loc_5_2CCC:
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_064C
loc_5_2CDD:
        jmp    loc_5_2C5E
loc_5_2CE2:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_5_2D2A
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0xc], eax
        jbe    loc_5_2D13
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0x10]
        neg    eax
        sub    dword ptr [ebp + 0xc], eax
        mov    dword ptr [ebp - 0x10], 0
        jmp    loc_5_2C5E
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_2D13:
        mov    eax, dword ptr [ebp + 0xc]
        add    dword ptr [ebp - 0x1c], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 0xc]
        neg    eax
        sub    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp + 0xc], 0
loc_5_2D2A:
        cmp    dword ptr [ebp + 0x18], 0
        je     loc_5_2D72
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 0x18], eax
        jbe    loc_5_2D5B
        xor    eax, eax
        sub    eax, dword ptr [ebp - 4]
        neg    eax
        sub    dword ptr [ebp + 0x18], eax
        mov    dword ptr [ebp - 4], 0
        jmp    loc_5_2C5E
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_2D5B:
        mov    eax, dword ptr [ebp + 0x18]
        add    dword ptr [ebp - 0x18], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 0x18]
        neg    eax
        sub    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp + 0x18], 0
loc_5_2D72:
        mov    eax, dword ptr [ebp + 0x10]
        sub    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x2c], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    eax, ecx
        jb     loc_5_2D8B
        mov    eax, ecx
loc_5_2D8B:
        mov    dword ptr [ebp - 0x28], eax
        mov    eax, dword ptr [ebp - 0x2c]
        mov    ecx, dword ptr [ebp - 0x28]
        cmp    eax, ecx
        jb     loc_5_2D9E
        mov    eax, ecx
loc_5_2D9E:
        mov    dword ptr [ebp - 0x28], eax
        mov    eax, dword ptr [ebp - 0x28]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_1_06E4
loc_5_2DB2:
        mov    eax, dword ptr [ebp - 0x28]
        add    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x28]
        add    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x28]
        add    dword ptr [ebp - 0x14], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0x28]
        neg    eax
        sub    dword ptr [ebp - 4], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0x28]
        neg    eax
        sub    dword ptr [ebp - 0x10], eax
        jmp    loc_5_2C5E
loc_5_2DDD:
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], eax
loc_5_2DE5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x18
loc_5_2DEC:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x8c
        push   eax
        call   sub_5_1DBC
        mov    word ptr [ebp - 8], ax
        movsx  ecx, word ptr [ebp - 8]
        test   ecx, ecx
        jne    loc_5_2E20
        xor    al, al
        jmp    loc_5_2E35
loc_5_2E20:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x8c
        push   eax
        call   sub_5_1DFC
        mov    al, 1
        jmp    loc_5_2E35
loc_5_2E35:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_5_2E3C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_224C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_5_2E9F
loc_5_2E73:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_3284
        jmp    loc_5_2EE9
loc_5_2E81:
        jmp    loc_5_2EE9
loc_5_2E86:
        jmp    loc_5_2EE9
loc_5_2E8B:
        jmp    loc_5_2EE9
loc_5_2E90:
        jmp    loc_5_2EE9
loc_5_2E95:
        jmp    loc_5_2EE9
        db  0E9h, 04Ah, 000h, 000h, 000h
loc_5_2E9F:
        dec    dword ptr [ebp - 0x18]
        cmp    dword ptr [ebp - 0x18], 0xf
        ja     loc_5_2EE9
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax]            ; -> obj5:0x2ED9
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj5:0x2EC1
        ; ---- dispatch table (6 entries) ----
        dd  loc_5_2E73
        dd  loc_5_2E81
        dd  loc_5_2E86
        dd  loc_5_2E8B
        dd  loc_5_2E90
        dd  loc_5_2E95
        db  000h, 001h, 005h, 002h, 005h, 005h, 005h, 003h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 004h
loc_5_2EE9:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_27AF
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_21AC
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        dec    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [ebp - 0x14]
        cmp    dword ptr [eax + 0x2a], 0
        jne    loc_5_2F4C
        mov    eax, dword ptr [ebp - 0x14]
        mov    al, byte ptr [eax + 0x7d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_2F3E
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x14]
        add    ecx, 0x2e
        cmp    dword ptr [eax + 0x2e], ecx
        je     loc_5_2F4C
loc_5_2F3E:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_5_2F61
        jmp    loc_5_2F4C
loc_5_2F4C:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_1E1C
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_5_2F61:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x7d]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_2FA3
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x46], 0
        jne    loc_5_2FA3
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x46], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_110C
loc_5_2FA3:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x7d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_3150
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x3e
        cmp    dword ptr [eax + 0x3e], ecx
        je     loc_5_3150
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x3e]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x14]
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5a]
        push   eax
        call   sub_2_01D0
loc_5_2FEC:
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_30BD
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x14], 2
        jne    loc_5_3065
        mov    eax, dword ptr [ebp - 4]
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
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x1e]
        cmp    dword ptr [eax + 0x18], ecx
        jne    loc_5_3042
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3579
loc_5_3042:
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_0574
loc_5_3050:
        push   0
        push   0x96
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0604
loc_5_3060:
        jmp    loc_5_30B8
loc_5_3065:
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
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2e
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x32]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x32]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x32], eax
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [eax + 0x14], 2
loc_5_30B8:
        jmp    loc_5_3150
loc_5_30BD:
        cmp    dword ptr [ebp - 8], 0x103
        jne    loc_5_3132
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_27AF
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
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2e
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x32]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x32]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x32], eax
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [eax + 0x14], 2
        jmp    loc_5_3150
loc_5_3132:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0xa
        push   7
        push   3
        push   0xaa000006
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_06D0
loc_5_314D:
        add    esp, 0x18
loc_5_3150:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x2e
        cmp    dword ptr [eax + 0x2e], ecx
        je     loc_5_31D2
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2e]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x14], 0
        jne    loc_5_31D2
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x1e]
        cmp    dword ptr [eax + 0x18], ecx
        jne    loc_5_3193
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3579
loc_5_3193:
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_0574
loc_5_31A1:
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
        push   0
        push   0x96
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0604
loc_5_31D2:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x2a]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_5_31DF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1c]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_5_3207
        jmp    loc_5_327D
loc_5_3207:
        push   0x3e8
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x23c
        push   eax
        call   sub_1_05DA
loc_5_321A:
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 0x248]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x248], 1
        jae    loc_5_325C
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x248], 0xa
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x8c
        push   eax
        call   sub_5_1D4C
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x8c
        push   eax
        call   sub_5_211C
loc_5_325C:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x8c
        push   eax
        call   sub_5_244C
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x8c
        push   eax
        call   sub_5_21AC
        jmp    loc_5_327D
loc_5_327D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_5_3284:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 8], 0
        jmp    loc_5_32A0
loc_5_329D:
        inc    dword ptr [ebp - 8]
loc_5_32A0:
        cmp    dword ptr [ebp - 8], 0xa
        jae    loc_5_33A6
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_1E7C
        movsx  eax, ax
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], -1
        jne    loc_5_32CD
        jmp    loc_5_33A6
loc_5_32CD:
        mov    byte ptr [ebp - 0xc], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x213]
        mov    dword ptr [ebp - 0x1c], eax
        mov    dword ptr [ebp - 0x14], 0xe
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1e]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 8]
        and    eax, 0xff
        test   al, 0x80
        je     loc_5_3314
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 0xe]
        and    eax, 0x1f
        add    eax, 0xe
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_5_331B
loc_5_3314:
        mov    dword ptr [ebp - 0x14], 0xe
loc_5_331B:
        mov    eax, dword ptr [ebp - 0x14]
        cmp    dword ptr [ebp - 0x18], eax
        jae    loc_5_335D
        cmp    dword ptr [ebp - 0x18], 6
        jb     loc_5_3358
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [ebp - 4], eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, dword ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5a]
        push   eax
        call   sub_1_00B0
loc_5_3358:
        jmp    loc_5_3393
loc_5_335D:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x10]
        sub    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        sub    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, dword ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5a]
        push   eax
        call   sub_1_00B0
loc_5_3393:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_1E3C
        jmp    loc_5_329D
loc_5_33A6:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        cmp    eax, 1
        jne    loc_5_33CE
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5a]
        push   eax
        call   sub_1_0300
loc_5_33C0:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_1EDC
loc_5_33CE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        db  000h, 000h, 000h
loc_5_33D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x22], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0xc], eax
        je     loc_5_3418
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_364F
        mov    dword ptr [ebp - 4], eax
loc_5_3418:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_5_3420
loc_5_3420:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
loc_5_3427:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x26], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0xc], eax
        je     loc_5_3467
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_36FA
        mov    dword ptr [ebp - 4], eax
loc_5_3467:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_5_346F
loc_5_346F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
loc_5_3476:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x72]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x76], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x72], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_364F
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_376A
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_5_34C3
loc_5_34C3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
loc_5_34CA:
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
sub_5_34DD:
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
        mov    eax, dword ptr [eax + 0x5a]
        push   eax
        call   sub_2_02C0
loc_5_3513:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        dec    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_5_3524
loc_5_3524:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_5_352B:
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
        mov    eax, dword ptr [eax + 0x5a]
        push   eax
        call   sub_2_0360
loc_5_3561:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        dec    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_5_3572
loc_5_3572:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_5_3579:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x36]
        mov    dword ptr [ebp - 4], eax
loc_5_3592:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x36
        cmp    eax, dword ptr [ebp - 4]
        je     loc_5_35CC
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x18], ecx
        jbe    loc_5_35BF
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 8], eax
loc_5_35BF:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_5_3592
loc_5_35CC:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_35DD
        mov    dword ptr [ebp - 8], 0xf0
loc_5_35DD:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1e], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_35ED:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        test   dword ptr [ebp + 0x14], 0x8056
        je     loc_5_3614
        mov    eax, 0xc00000bb
        jmp    loc_5_3648
loc_5_3614:
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
        mov    eax, dword ptr [eax + 0x5a]
        push   eax
        call   sub_2_04F0
loc_5_3637:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        dec    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_5_3648
loc_5_3648:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_5_364F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0x7fffffff
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x73], 0x20
        je     loc_5_3677
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_5_3699
loc_5_3677:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x73], 0x40
        je     loc_5_3692
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x22]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_5_3699
loc_5_3692:
        mov    dword ptr [ebp - 4], 0
loc_5_3699:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x207], ecx
        jne    loc_5_36B2
        xor    eax, eax
        jmp    loc_5_36F3
loc_5_36B2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x207], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_1C7C
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_211C
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_21AC
        xor    eax, eax
        jmp    loc_5_36F3
loc_5_36F3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_36FA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x20b]
        cmp    dword ptr [eax + 0x26], ecx
        jne    loc_5_371C
        xor    eax, eax
        jmp    loc_5_3763
loc_5_371C:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x26]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x20b], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x26]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_1CBC
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_211C
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_21AC
        xor    eax, eax
        jmp    loc_5_3763
loc_5_3763:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_376A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x72], 0x20
        je     loc_5_3795
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        or     eax, 2
        mov    word ptr [ebp - 4], ax
loc_5_3795:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x20f]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        cmp    eax, ecx
        jne    loc_5_37BC
        xor    eax, eax
        jmp    loc_5_37FF
loc_5_37BC:
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x20f], ax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_1CFC
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_211C
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        call   sub_5_21AC
        xor    eax, eax
        jmp    loc_5_37FF
loc_5_37FF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        db  000h, 000h
loc_5_3808:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x10], 0
        jne    loc_5_3853
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_3F34
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_5_390F
loc_5_3853:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x10], 1
        jne    loc_5_3908
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x7c]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_38AB
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_389F
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_3FAE
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_5_38A6
loc_5_389F:
        mov    dword ptr [ebp - 0xc], 0xc0010002
loc_5_38A6:
        jmp    loc_5_3903
loc_5_38AB:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x7d]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_38CA
        mov    dword ptr [ebp - 0xc], 0xc001000d
        jmp    loc_5_3903
loc_5_38CA:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x7e]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_38E9
        mov    dword ptr [ebp - 0xc], 0xc0000001
        jmp    loc_5_3903
loc_5_38E9:
        push   3
        push   0xb
        push   2
        push   0xaa000006
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_06D0
loc_5_3900:
        add    esp, 0x14
loc_5_3903:
        jmp    loc_5_390F
loc_5_3908:
        mov    dword ptr [ebp - 0xc], 0x10001
loc_5_390F:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x2a], 0
        jne    loc_5_395B
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x7d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_394D
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x10]
        add    ecx, 0x2e
        cmp    dword ptr [eax + 0x2e], ecx
        je     loc_5_395B
loc_5_394D:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_5_2F61
        jmp    loc_5_395B
loc_5_395B:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_5_3963
loc_5_3963:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_5_396A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x44
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x20], 1
        mov    dword ptr [ebp - 0x10], 0
        mov    word ptr [ebp - 0x28], 0
        mov    dword ptr [ebp - 0x24], 0
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x44], eax
        jmp    loc_5_3E3C
loc_5_39A0:
        mov    dword ptr [ebp - 0x10], 6
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_5_3EE2
loc_5_39B9:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        test   eax, eax
        jne    loc_5_39D9
        mov    dword ptr [ebp - 0x14], 0     ; -> obj4:0x00B0
        mov    dword ptr [ebp - 4], 0x5c
        jmp    loc_5_39E7
loc_5_39D9:
        mov    dword ptr [ebp - 0x14], 0     ; -> obj4:0x0010
        mov    dword ptr [ebp - 4], 0xa0
loc_5_39E7:
        jmp    loc_5_3EE2
loc_5_39EC:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x7d]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_3A0B
        mov    dword ptr [ebp - 8], 2
        jmp    loc_5_3A50
loc_5_3A0B:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x7f]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_3A2A
        mov    dword ptr [ebp - 8], 1
        jmp    loc_5_3A50
loc_5_3A2A:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x7c]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_3A49
        mov    dword ptr [ebp - 8], 4
        jmp    loc_5_3A50
loc_5_3A49:
        mov    dword ptr [ebp - 8], 0
loc_5_3A50:
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_5_3EE2
loc_5_3A62:
        lea    eax, [ebp - 0x20]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_5_3EE2
loc_5_3A74:
        mov    dword ptr [ebp - 0x10], 0xf0
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_5_3EE2
loc_5_3A8D:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1f3]
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp + 0x10], 0x10106
        jne    loc_5_3AAA
        sub    dword ptr [ebp - 0x10], 0xe
loc_5_3AAA:
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_5_3EE2
loc_5_3ABC:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1ca], 1
        jne    loc_5_3AD8
        mov    dword ptr [ebp - 0x10], 0x9c40
        jmp    loc_5_3ADF
loc_5_3AD8:
        mov    dword ptr [ebp - 0x10], 0x27100
loc_5_3ADF:
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_5_3EE2
loc_5_3AF1:
        mov    dword ptr [ebp - 0x10], 0x4650
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_5_3EE2
loc_5_3B0A:
        mov    dword ptr [ebp - 0x10], 0xafc8
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_5_3EE2
loc_5_3B23:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1f3]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_5_3EE2
loc_5_3B41:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1f3]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_5_3EE2
loc_5_3B5F:
        push   3
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1e7
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_06E4
loc_5_3B73:
        mov    byte ptr [ebp - 9], 0
        lea    eax, [ebp - 0xc]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_5_3EE2
loc_5_3B89:
        mov    dword ptr [ebp - 0x14], 0     ; -> loc_1_68B8
        mov    dword ptr [ebp - 4], 0x2f
        jmp    loc_5_3EE2
loc_5_3B9C:
        mov    word ptr [ebp - 0x28], 0x300
        lea    eax, [ebp - 0x28]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 2
        jmp    loc_5_3EE2
loc_5_3BB4:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        test   eax, eax
        je     loc_5_3BCF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x72]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_5_3BDB
loc_5_3BCF:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0x10], eax
loc_5_3BDB:
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_5_3EE2
loc_5_3BED:
        mov    dword ptr [ebp - 0x10], 0
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_5_3EE2
loc_5_3C06:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        test   eax, eax
        jne    loc_5_3C21
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_5_3C7D
loc_5_3C21:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x36]
        mov    dword ptr [ebp - 0x2c], eax
        mov    dword ptr [ebp - 0x10], 0
loc_5_3C31:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x36
        cmp    eax, dword ptr [ebp - 0x2c]
        je     loc_5_3C7D
        mov    eax, dword ptr [ebp - 0x2c]
        mov    dword ptr [ebp - 0x30], eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x18], ecx
        jbe    loc_5_3C70
        mov    eax, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0xf0
        jne    loc_5_3C70
        jmp    loc_5_3C7D
loc_5_3C70:
        mov    eax, dword ptr [ebp - 0x2c]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x2c], eax
        jmp    loc_5_3C31
loc_5_3C7D:
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_5_3EE2
loc_5_3C8F:
        lea    eax, [ebp - 0x1c]
        mov    dword ptr [ebp - 0x34], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1e7
        mov    dword ptr [ebp - 0x38], eax
        mov    eax, dword ptr [ebp - 0x38]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 0x34]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0x38]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 0x34]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 0x38]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0x34]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 0x38]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 0x34]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 0x38]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x34]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 0x38]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 0x34]
        mov    byte ptr [ecx + 5], al
        lea    eax, [ebp - 0x1c]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 6
        jmp    loc_5_3EE2
loc_5_3CF8:
        lea    eax, [ebp - 0x1c]
        mov    dword ptr [ebp - 0x3c], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1ed
        mov    dword ptr [ebp - 0x40], eax
        mov    eax, dword ptr [ebp - 0x40]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 0x3c]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0x40]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 0x3c]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 0x40]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0x3c]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 0x40]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 0x3c]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 0x40]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x3c]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 0x40]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 0x3c]
        mov    byte ptr [ecx + 5], al
        lea    eax, [ebp - 0x1c]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 6
        jmp    loc_5_3EE2
loc_5_3D61:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        test   eax, eax
        jne    loc_5_3D7F
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_5_3D8B
loc_5_3D7F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x207]
        mov    dword ptr [ebp - 0x10], eax
loc_5_3D8B:
        mov    eax, dword ptr [ebp - 0x10]
        shr    eax, 0x18
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0x10]
        shl    ecx, 8
        and    ecx, 0xff0000
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x10]
        shr    ecx, 8
        and    ecx, 0xff00
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x10]
        shl    ecx, 0x18
        and    ecx, 0xff000000
        or     eax, ecx
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_5_3EE2
loc_5_3DD5:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x20b]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        shr    eax, 0x18
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0x10]
        shl    ecx, 8
        and    ecx, 0xff0000
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x10]
        shr    ecx, 8
        and    ecx, 0xff00
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x10]
        shl    ecx, 0x18
        and    ecx, 0xff000000
        or     eax, ecx
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], 4
        jmp    loc_5_3EE2
loc_5_3E2B:
        mov    dword ptr [ebp - 0x24], 0xc00000bb
        jmp    loc_5_3EE2
        db  0E9h, 0A6h, 000h, 000h, 000h
loc_5_3E3C:
        cmp    dword ptr [ebp - 0x44], 0x2010101
        ja     loc_5_3EB6
        je     loc_5_3C8F
        sub    dword ptr [ebp - 0x44], 0x10101
        cmp    dword ptr [ebp - 0x44], 0x12
        ja     loc_5_3E2B
        mov    eax, dword ptr [ebp - 0x44]
        jmp    dword ptr [eax*4]             ; -> obj5:0x3E6A
        ; ---- dispatch table (19 entries) ----
        dd  loc_5_39B9
        dd  loc_5_39EC
        dd  loc_5_3A62
        dd  loc_5_3A62
        dd  loc_5_3A74
        dd  loc_5_3A8D
        dd  loc_5_3ABC
        dd  loc_5_3AF1
        dd  loc_5_3B0A
        dd  loc_5_3B23
        dd  loc_5_3B41
        dd  loc_5_3B5F
        dd  loc_5_3B89
        dd  loc_5_3BB4
        dd  loc_5_3C06
        dd  loc_5_3B9C
        dd  loc_5_3A8D
        dd  loc_5_3BED
        dd  loc_5_39A0
loc_5_3EB6:
        cmp    dword ptr [ebp - 0x44], 0x2010102
        je     loc_5_3CF8
        cmp    dword ptr [ebp - 0x44], 0x2010103
        je     loc_5_3D61
        cmp    dword ptr [ebp - 0x44], 0x2010104
        je     loc_5_3DD5
        jmp    loc_5_3E2B
loc_5_3EE2:
        cmp    dword ptr [ebp - 0x24], 0
        jne    loc_5_3F25
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_5_3F0C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebp - 0x24], 0xc0010014
        jmp    loc_5_3F25
loc_5_3F0C:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_06E4
loc_5_3F1D:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], eax
loc_5_3F25:
        mov    eax, dword ptr [ebp - 0x24]
        jmp    loc_5_3F2D
loc_5_3F2D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x20
sub_5_3F34:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x14], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0x10], 0
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_396A
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x20], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x24], eax
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_5_3FA7
loc_5_3FA7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_5_3FAE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x20], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 0x18], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 4
        je     loc_5_4013
        mov    dword ptr [ebp - 0x18], 0xc0010014
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp - 0x18]
        jmp    loc_5_41B8
loc_5_4013:
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x24], eax
        jmp    loc_5_4142
loc_5_401E:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_34DD
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_5_4193
loc_5_403B:
        push   4
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_1_06E4
loc_5_404A:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_35ED
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_5_4193
loc_5_4067:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_352B
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_5_4193
        db  0E9h, 00Ah, 001h, 000h, 000h
loc_5_4089:
        push   4
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_06E4
loc_5_4098:
        cmp    dword ptr [ebp - 0x1c], 0xf0
        ja     loc_5_4111
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x1c]
        cmp    dword ptr [eax + 0x1e], ecx
        jae    loc_5_40CB
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1e], eax
        jmp    loc_5_410C
loc_5_40CB:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x1e]
        cmp    dword ptr [eax + 0x18], ecx
        jne    loc_5_4103
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0x1c]
        cmp    dword ptr [eax + 0x18], ecx
        jbe    loc_5_4103
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3579
        jmp    loc_5_410C
loc_5_4103:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x18], eax
loc_5_410C:
        jmp    loc_5_4118
loc_5_4111:
        mov    dword ptr [ebp - 0x18], 0xc0010014
loc_5_4118:
        jmp    loc_5_4193
loc_5_411D:
        mov    dword ptr [ebp - 0x18], 0xc0010017
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x24], 0
        jmp    loc_5_4193
        db  0E9h, 051h, 000h, 000h, 000h
loc_5_4142:
        cmp    dword ptr [ebp - 0x24], 0x2010103
        ja     loc_5_4181
        je     loc_5_401E
        cmp    dword ptr [ebp - 0x24], 0x1010e
        je     loc_5_403B
        cmp    dword ptr [ebp - 0x24], 0x1010f
        je     loc_5_4089
        cmp    dword ptr [ebp - 0x24], 0x10112
        je     loc_5_4193
        jmp    loc_5_411D
loc_5_4181:
        cmp    dword ptr [ebp - 0x24], 0x2010104
        je     loc_5_4067
        jmp    loc_5_411D
loc_5_4193:
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_5_41B0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x20], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x24], 0
loc_5_41B0:
        mov    eax, dword ptr [ebp - 0x18]
        jmp    loc_5_41B8
loc_5_41B8:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_5_41BF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x14], 0
        mov    dword ptr [ebp - 0x1c], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 0x18], 0
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 4], 0xc
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_396A
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [ebp - 0x18], 0xc00000bb
        jne    loc_5_4562
        mov    dword ptr [ebp - 0x18], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_5_4458
loc_5_4246:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1f7]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_5_4524
loc_5_4257:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1fb]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_5_4524
loc_5_4268:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ff]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_5_4524
loc_5_4279:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x203]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_5_4524
loc_5_428A:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xa0]
        mov    al, byte ptr [eax + 7]
        and    eax, 0xff
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_5_4524
loc_5_42A3:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xa0]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_5_4524
loc_5_42BB:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xa0]
        mov    al, byte ptr [eax + 6]
        and    eax, 0xff
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_5_4524
loc_5_42D4:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1b2], 1
        jne    loc_5_42F0
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_5_42F7
loc_5_42F0:
        mov    dword ptr [ebp - 0xc], 0xc0010007
loc_5_42F7:
        jmp    loc_5_4524
loc_5_42FC:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1b6], 1
        jne    loc_5_4318
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_5_434B
loc_5_4318:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1b6], 2
        jne    loc_5_4334
        mov    dword ptr [ebp - 0xc], 0x8000
        jmp    loc_5_434B
loc_5_4334:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1b6], 3
        jne    loc_5_434B
        mov    dword ptr [ebp - 0xc], 0x4000
loc_5_434B:
        jmp    loc_5_4524
loc_5_4350:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1b2], 1
        jne    loc_5_436C
        mov    dword ptr [ebp - 0xc], 1
        jmp    loc_5_43C7
loc_5_436C:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1b2], 3
        jne    loc_5_4388
        mov    dword ptr [ebp - 0xc], 3
        jmp    loc_5_43C7
loc_5_4388:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1b2], 4
        jne    loc_5_43A4
        mov    dword ptr [ebp - 0xc], 3
        jmp    loc_5_43C7
loc_5_43A4:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1b2], 5
        jne    loc_5_43C0
        mov    dword ptr [ebp - 0xc], 5
        jmp    loc_5_43C7
loc_5_43C0:
        mov    dword ptr [ebp - 0xc], 2
loc_5_43C7:
        jmp    loc_5_4524
loc_5_43CC:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xa0]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_5_4524
loc_5_43E5:
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_5_4524
loc_5_43F1:
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_5_4524
loc_5_43FD:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xa0]
        mov    al, byte ptr [eax + 8]
        and    eax, 0xff
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_5_4524
loc_5_4416:
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_5_4524
loc_5_4422:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xa0]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_5_4524
loc_5_443B:
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_5_4524
loc_5_4447:
        mov    dword ptr [ebp - 0x18], 0xc0010017
        jmp    loc_5_4524
        db  0E9h, 0CCh, 000h, 000h, 000h
loc_5_4458:
        cmp    dword ptr [ebp - 0x20], 0x2010105
        ja     loc_5_449A
        je     loc_5_42D4
        sub    dword ptr [ebp - 0x20], 0x20101
        cmp    dword ptr [ebp - 0x20], 4
        ja     loc_5_4447
        mov    eax, dword ptr [ebp - 0x20]
        jmp    dword ptr [eax*4]             ; -> obj5:0x4486
        ; ---- dispatch table (5 entries) ----
        dd  loc_5_4246
        dd  loc_5_4257
        dd  loc_5_4268
        dd  loc_5_4279
        dd  loc_5_428A
loc_5_449A:
        cmp    dword ptr [ebp - 0x20], 0x2020101
        ja     loc_5_44CC
        je     loc_5_42A3
        cmp    dword ptr [ebp - 0x20], 0x2010106
        je     loc_5_42FC
        cmp    dword ptr [ebp - 0x20], 0x2010107
        je     loc_5_4350
        jmp    loc_5_4447
loc_5_44CC:
        cmp    dword ptr [ebp - 0x20], 0x2020201
        ja     loc_5_44F1
        je     loc_5_43CC
        cmp    dword ptr [ebp - 0x20], 0x2020102
        je     loc_5_42BB
        jmp    loc_5_4447
loc_5_44F1:
        sub    dword ptr [ebp - 0x20], 0x2020202
        cmp    dword ptr [ebp - 0x20], 5
        ja     loc_5_4447
        mov    eax, dword ptr [ebp - 0x20]
        jmp    dword ptr [eax*4]             ; -> obj5:0x450C
        ; ---- dispatch table (6 entries) ----
        dd  loc_5_43E5
        dd  loc_5_43F1
        dd  loc_5_43FD
        dd  loc_5_4416
        dd  loc_5_4422
        dd  loc_5_443B
loc_5_4524:
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_5_4562
        cmp    dword ptr [ebp - 8], 4
        jb     loc_5_4550
        push   4
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_06E4
loc_5_4547:
        add    dword ptr [ebp - 0x14], 4
        jmp    loc_5_4562
loc_5_4550:
        mov    eax, 4
        sub    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x1c], eax
        mov    dword ptr [ebp - 0x18], 0xc0010014
loc_5_4562:
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x20], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x24], eax
        mov    eax, dword ptr [ebp - 0x18]
        jmp    loc_5_457C
loc_5_457C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
loc_5_4583:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_5_45CF
        db  0E9h, 0E0h, 000h, 000h, 000h
loc_5_45B2:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_5_4692
loc_5_45BE:
        mov    dword ptr [ebp - 8], 0xc0010017
        jmp    loc_5_4692
        db  0E9h, 0C3h, 000h, 000h, 000h
loc_5_45CF:
        cmp    dword ptr [ebp - 0x10], 0x10113
        ja     loc_5_4601
        je     loc_5_4692
        cmp    dword ptr [ebp - 0x10], 0x10101
        jb     loc_5_45BE
        cmp    dword ptr [ebp - 0x10], 0x10111
        jbe    loc_5_4692
        jmp    loc_5_45BE
loc_5_4601:
        cmp    dword ptr [ebp - 0x10], 0x2010103
        ja     loc_5_463A
        cmp    dword ptr [ebp - 0x10], 0x2010101
        jae    loc_5_4692
        cmp    dword ptr [ebp - 0x10], 0x20101
        jb     loc_5_45BE
        cmp    dword ptr [ebp - 0x10], 0x20105
        jbe    loc_5_45B2
        jmp    loc_5_45BE
loc_5_463A:
        cmp    dword ptr [ebp - 0x10], 0x2020102
        ja     loc_5_4673
        cmp    dword ptr [ebp - 0x10], 0x2020101
        jae    loc_5_45B2
        cmp    dword ptr [ebp - 0x10], 0x2010106
        jb     loc_5_45BE
        cmp    dword ptr [ebp - 0x10], 0x2010107
        jbe    loc_5_45B2
        jmp    loc_5_45BE
loc_5_4673:
        cmp    dword ptr [ebp - 0x10], 0x2020201
        jb     loc_5_45BE
        cmp    dword ptr [ebp - 0x10], 0x2020207
        jbe    loc_5_45B2
        jmp    loc_5_45BE
loc_5_4692:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_46AC
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_41BF
        mov    dword ptr [ebp - 8], eax
loc_5_46AC:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [eax + 0x2a], 0
        jne    loc_5_46F8
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0x7d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_46EA
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        add    ecx, 0x2e
        cmp    dword ptr [eax + 0x2e], ecx
        je     loc_5_46F8
loc_5_46EA:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_5_2F61
        jmp    loc_5_46F8
loc_5_46F8:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_5_4700
loc_5_4700:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
        db  000h
loc_5_4708:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x7c]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_4793
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_4787
        mov    eax, dword ptr [ebp - 0xc]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], 0
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
        call   sub_5_2A7C
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 8], 0
        jmp    loc_5_478E
loc_5_4787:
        mov    dword ptr [ebp - 8], 0xc001000c
loc_5_478E:
        jmp    loc_5_47EB
loc_5_4793:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x7d]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_47B2
        mov    dword ptr [ebp - 8], 0xc001000d
        jmp    loc_5_47EB
loc_5_47B2:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x7f]
        and    eax, 0xff
        test   eax, eax
        je     loc_5_47D1
        mov    dword ptr [ebp - 8], 0xc0000001
        jmp    loc_5_47EB
loc_5_47D1:
        push   1
        push   0xb
        push   2
        push   0xaa000006
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_06D0
loc_5_47E8:
        add    esp, 0x14
loc_5_47EB:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x2a]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x2a], 0
        jne    loc_5_4837
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x7d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_5_4829
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x10]
        add    ecx, 0x2e
        cmp    dword ptr [eax + 0x2e], ecx
        je     loc_5_4837
loc_5_4829:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_5_2F61
        jmp    loc_5_4837
loc_5_4837:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_5_483F
loc_5_483F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x18
        db  000h, 000h
sub_5_4848:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x256
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x8c
        push   eax
        push   0
        push   0x106
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C2
loc_5_4876:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0x8c], 0
        jne    loc_5_488D
        xor    al, al
        jmp    loc_5_4D41
loc_5_488D:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x8c]
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x256]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x114], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x25a
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x90
        push   eax
        push   0
        push   0x108
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C2
loc_5_48D9:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0x90], 0
        jne    loc_5_48F0
        xor    al, al
        jmp    loc_5_4D41
loc_5_48F0:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x90]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x25a]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x118], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x25e
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x94
        push   eax
        push   0
        push   0x20
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C2
loc_5_493A:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0x94], 0
        jne    loc_5_4951
        xor    al, al
        jmp    loc_5_4D41
loc_5_4951:
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [eax + 0x94]
        xor    eax, eax
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x25e]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x11c], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x262
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x98
        push   eax
        push   0
        push   0x16
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C2
loc_5_4997:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0x98], 0
        jne    loc_5_49AE
        xor    al, al
        jmp    loc_5_4D41
loc_5_49AE:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x98]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0xc], 0
        mov    dword ptr [eax + 0x10], 0
        mov    word ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x262]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x120], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x266
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x9c
        push   eax
        push   0
        push   0x12
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C2
loc_5_4A13:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0x9c], 0
        jne    loc_5_4A2A
        xor    al, al
        jmp    loc_5_4D41
loc_5_4A2A:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x9c]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0xc], 0
        mov    word ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x266]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x124], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x26a
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xa0
        push   eax
        push   0
        push   0xe
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C2
loc_5_4A88:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0xa0], 0
        jne    loc_5_4A9F
        xor    al, al
        jmp    loc_5_4D41
loc_5_4A9F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xa0]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    word ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x26a]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x128], eax
        mov    dword ptr [ebp - 4], 0
        jmp    loc_5_4AE3
loc_5_4AE0:
        inc    dword ptr [ebp - 4]
loc_5_4AE3:
        cmp    dword ptr [ebp - 4], 4
        jae    loc_5_4B6D
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x26e
        push   eax
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 0xa4
        push   eax
        push   0
        push   0x1a
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C2
loc_5_4B1B:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + ecx*4 + 0xa4], 0
        jne    loc_5_4B36
        xor    al, al
        jmp    loc_5_4D41
loc_5_4B36:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx + eax*4 + 0xa4]
        xor    eax, eax
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x26e]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x12c], eax
        jmp    loc_5_4AE0
loc_5_4B6D:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_5_4B7C
loc_5_4B79:
        inc    dword ptr [ebp - 4]
loc_5_4B7C:
        cmp    dword ptr [ebp - 4], 0xa
        jae    loc_5_4C06
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x27e
        push   eax
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 0xb4
        push   eax
        push   0
        push   0x1a
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C2
loc_5_4BB4:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + ecx*4 + 0xb4], 0
        jne    loc_5_4BCF
        xor    al, al
        jmp    loc_5_4D41
loc_5_4BCF:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx + eax*4 + 0xb4]
        xor    eax, eax
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x27e]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x13c], eax
        jmp    loc_5_4B79
loc_5_4C06:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_5_4C15
loc_5_4C12:
        inc    dword ptr [ebp - 4]
loc_5_4C15:
        cmp    dword ptr [ebp - 4], 0xa
        jae    loc_5_4CA0
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x2a6
        push   eax
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 0xdc
        push   eax
        push   0
        push   0x1194
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C2
loc_5_4C50:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + ecx*4 + 0xdc], 0
        jne    loc_5_4C6B
        xor    al, al
        jmp    loc_5_4D41
loc_5_4C6B:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx + eax*4 + 0xdc]
        xor    eax, eax
        mov    ecx, 0x465
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x2a6]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x164], eax
        jmp    loc_5_4C12
loc_5_4CA0:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_5_4CAF
loc_5_4CAC:
        inc    dword ptr [ebp - 4]
loc_5_4CAF:
        cmp    dword ptr [ebp - 4], 4
        jae    loc_5_4D3A
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x2ce
        push   eax
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x104
        push   eax
        push   0
        push   0x1194
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C2
loc_5_4CEA:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + ecx*4 + 0x104], 0
        jne    loc_5_4D05
        xor    al, al
        jmp    loc_5_4D41
loc_5_4D05:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx + eax*4 + 0x104]
        xor    eax, eax
        mov    ecx, 0x465
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x2ce]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x18c], eax
        jmp    loc_5_4CAC
loc_5_4D3A:
        mov    al, 1
        jmp    loc_5_4D41
loc_5_4D41:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_5_4D48:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x256]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x8c]
        push   eax
        push   0
        push   0x106
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C8
loc_5_4D78:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x25a]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x90]
        push   eax
        push   0
        push   0x108
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C8
loc_5_4D9F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x25e]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x94]
        push   eax
        push   0
        push   0x20
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C8
loc_5_4DC3:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x262]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x98]
        push   eax
        push   0
        push   0x16
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C8
loc_5_4DE7:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x266]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x9c]
        push   eax
        push   0
        push   0x12
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C8
loc_5_4E0B:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x26a]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xa0]
        push   eax
        push   0
        push   0xe
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C8
loc_5_4E2F:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_5_4E3E
loc_5_4E3B:
        inc    dword ptr [ebp - 4]
loc_5_4E3E:
        cmp    dword ptr [ebp - 4], 4
        jae    loc_5_4E79
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x26e]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0xa4]
        push   eax
        push   0
        push   0x1a
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C8
loc_5_4E74:
        jmp    loc_5_4E3B
loc_5_4E79:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_5_4E88
loc_5_4E85:
        inc    dword ptr [ebp - 4]
loc_5_4E88:
        cmp    dword ptr [ebp - 4], 0xa
        jae    loc_5_4EC3
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x27e]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0xb4]
        push   eax
        push   0
        push   0x1a
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C8
loc_5_4EBE:
        jmp    loc_5_4E85
loc_5_4EC3:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_5_4ED2
loc_5_4ECF:
        inc    dword ptr [ebp - 4]
loc_5_4ED2:
        cmp    dword ptr [ebp - 4], 0xa
        jae    loc_5_4F10
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x2a6]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0xdc]
        push   eax
        push   0
        push   0x1194
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C8
loc_5_4F0B:
        jmp    loc_5_4ECF
loc_5_4F10:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_5_4F1F
loc_5_4F1C:
        inc    dword ptr [ebp - 4]
loc_5_4F1F:
        cmp    dword ptr [ebp - 4], 4
        jae    loc_5_4F5D
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x2ce]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x104]
        push   eax
        push   0
        push   0x1194
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x56]
        push   eax
        call   sub_1_05C8
loc_5_4F58:
        jmp    loc_5_4F1C
loc_5_4F5D:
        mov    al, 1
        jmp    loc_5_4F64
loc_5_4F64:
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
        mov    dword ptr [ebp - 4], eax
        add    dword ptr [ebp - 4], 0xff
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffffff00
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_5_4F94
loc_5_4F94:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        db  000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    loc_5_4FB4
loc_5_4FB1:
        inc    dword ptr [ebp - 4]
loc_5_4FB4:
        cmp    dword ptr [ebp - 4], 0x28
        jae    loc_5_4FC3
        jmp    loc_5_4FB1
loc_5_4FC3:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_5_4FCB
loc_5_4FCB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_5_4FD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        shl    eax, 3
        mov    dword ptr [ebp + 0xc], eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_05F8
loc_5_4FF1:
        xor    ax, ax
        jmp    loc_5_4FF9
loc_5_4FF9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x191]
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [ebp + 0x10]
        out    dx, al
        xor    ax, ax
        jmp    loc_5_5024
loc_5_5024:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_5_502B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x191]
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        out    dx, ax
        xor    ax, ax
        jmp    loc_5_5055
loc_5_5055:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x191]
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp + 0xc]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        jmp    loc_5_5080
loc_5_5080:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_5_5087:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x191]
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp + 0xc]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    ax, word ptr [ebp - 8]
        jmp    loc_5_50AE
loc_5_50AE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x191]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x13a]
        add    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0521
loc_5_50E4:
        xor    ax, ax
        jmp    loc_5_50EC
loc_5_50EC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x191]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x13a]
        add    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_04E4
loc_5_5122:
        xor    ax, ax
        jmp    loc_5_512A
loc_5_512A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_5_5131:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05FE
loc_5_514C:
        xor    ax, ax
        jmp    loc_5_5154
loc_5_5154:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_5_515B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0604
loc_5_5170:
        xor    ax, ax
        jmp    loc_5_5178
loc_5_5178:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x3108
VxD__LTEXT ENDS
