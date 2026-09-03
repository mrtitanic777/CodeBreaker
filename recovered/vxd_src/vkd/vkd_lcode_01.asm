; vkd_lcode_01.asm -- VKD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VKD_Control:
        cmp    eax, 13h
        jae    SHORT loc_1_000C
        db  0FFh, 024h, 085h, 00Eh, 000h, 000h, 000h
loc_1_000C:
        clc
        ret
        db  018h, 000h, 000h, 000h, 0CBh, 001h, 000h, 000h, 07Ah, 003h, 000h, 000h, 083h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 00Eh, 000h, 000h, 000h, 052h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  02Ch, 000h, 000h, 000h, 083h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h
        db  086h, 000h, 000h, 000h, 0BBh, 000h, 000h, 000h, 077h, 010h, 000h, 000h, 08Bh, 0C0h
VKD_Get_Shift_State:
        db  0A1h, 010h, 012h, 000h, 000h
        add    eax, ebx
        mov    ax, word ptr [eax + 36h]
        ret
VKD_Set_Shift_State:
        pushad
        xchg   ebx, eax
        db  003h, 005h, 010h, 012h, 000h, 000h
        xchg   esi, eax
        mov    ax, word ptr [esi + 36h]
        call   sub_1_0655
        jbe    SHORT loc_1_0084
        mov    edx, esi
        call   sub_1_01B0
        clc
loc_1_0084:
        popad
        ret
VKD_BEGIN_MESSAGE_MODE:
        db  08Bh, 035h, 010h, 012h, 000h, 000h
        db  0E8h, 007h, 006h, 0FFh, 0FFh
        or     byte ptr [esi + ebx + 1], 40h
        db  089h, 01Dh, 05Ch, 012h, 000h, 000h
        xor    eax, eax
        db  0C7h, 005h, 060h, 012h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 064h, 012h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 088h, 012h, 000h, 000h, 000h
        clc
        ret
VKD_END_MESSAGE_MODE:
        db  08Bh, 035h, 010h, 012h, 000h, 000h
        and    byte ptr [esi + ebx + 1], 0bfh
        db  03Bh, 01Dh, 05Ch, 012h, 000h, 000h
        jne    SHORT loc_1_00D8
        db  0C7h, 005h, 05Ch, 012h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00D8:
        clc
        ret
VKD_Force_Keys:
        pushad
        jecxz  loc_1_0120
        db  08Bh, 015h, 014h, 012h, 000h, 000h
        db  003h, 015h, 010h, 012h, 000h, 000h
loc_1_00E9:
        cld
        lodsb
        call   VKD_Put_Byte
        jae    SHORT loc_1_0106
        push   esi
        push   eax
        push   ecx
        push   edx
        call   sub_1_01B0
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        call   VKD_Put_Byte
        jb     SHORT loc_1_0121
loc_1_0106:
        test   byte ptr [edx + 1], 40h
        je     SHORT loc_1_0119
        push   esi
        push   eax
        push   ecx
        push   edx
        call   sub_1_01B0
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
loc_1_0119:
        loop   loc_1_00E9
        call   sub_1_01B0
loc_1_0120:
        clc
loc_1_0121:
        popad
        ret
        db  090h
VKD_Get_Kbd_Owner:
        db  08Bh, 01Dh, 014h, 012h, 000h, 000h
        ret
VKD_Get_Msg_Key:
        db  03Bh, 01Dh, 05Ch, 012h, 000h, 000h
        jne    SHORT loc_1_0155
        db  0A1h, 060h, 012h, 000h, 000h
        db  03Bh, 005h, 064h, 012h, 000h, 000h
        je     SHORT loc_1_0155
        add    eax, 2
        and    eax, 1fh
        db  0A3h, 060h, 012h, 000h, 000h
        db  00Fh, 0B7h, 080h, 068h, 012h, 000h, 000h
        or     ebx, ebx
        ret
loc_1_0155:
        xor    eax, eax
        ret
VKD_Peek_Msg_Key:
        db  03Bh, 01Dh, 05Ch, 012h, 000h, 000h
        jne    SHORT loc_1_017D
        db  0A1h, 060h, 012h, 000h, 000h
        db  03Bh, 005h, 064h, 012h, 000h, 000h
        je     SHORT loc_1_017D
        add    eax, 2
        and    eax, 1fh
        db  00Fh, 0B7h, 080h, 068h, 012h, 000h, 000h
        or     ebx, ebx
        ret
loc_1_017D:
        xor    eax, eax
        ret
VKD_Flush_Msg_Key_Queue:
        db  03Bh, 01Dh, 05Ch, 012h, 000h, 000h
        jne    SHORT loc_1_01A4
        db  0C7h, 005h, 060h, 012h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 064h, 012h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 088h, 012h, 000h, 000h, 000h
        ret
loc_1_01A4:
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_01A8:
        mov    dword ptr [edx + 4], 0
        nop
sub_1_01B0:
        mov    esi, edx
        db  02Bh, 015h, 010h, 012h, 000h, 000h
loc_1_01B8:
        db  0F6h, 005h, 051h, 012h, 000h, 000h, 010h
        je     NEAR PTR loc_1_0280
        test   byte ptr [esi + 1], 8
        je     SHORT loc_1_01D0
        db  0E8h, 0E4h, 000h, 0FFh, 0FFh
loc_1_01D0:
        call   sub_1_03A4
        db  03Bh, 015h, 014h, 012h, 000h, 000h
        jne    SHORT loc_1_022B
        db  0F6h, 005h, 051h, 012h, 000h, 000h, 001h
        jne    NEAR PTR loc_1_02AA
        mov    ah, byte ptr [esi + 34h]
        mov    al, byte ptr [esi + 35h]
        cmp    al, ah
        je     SHORT loc_1_01FF
        jb     SHORT loc_1_01F9
        add    ah, 80h
loc_1_01F9:
        add    al, 3
        cmp    al, ah
        jae    SHORT loc_1_022B
loc_1_01FF:
        call   sub_1_0320
        jb     SHORT loc_1_022B
        call   sub_1_0888
        mov    edi, eax
        and    edi, 0ffffff7fh
        db  03Bh, 03Dh, 039h, 012h, 000h, 000h
        jne    SHORT loc_1_023E
        test   al, 80h
        je     SHORT loc_1_01D0
        db  0C7h, 005h, 039h, 012h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_01D0
loc_1_022B:
        ret
sub_1_022C:
        call   sub_1_08CF
        je     SHORT loc_1_023D
        test   al, 80h
        jne    SHORT loc_1_023D
        db  0CDh, 020h, 001h, 000h, 00Bh, 000h
loc_1_023D:
        ret
loc_1_023E:
        db  08Bh, 03Dh, 02Dh, 012h, 000h, 000h
        call   sub_1_098A
        call   sub_1_0A24
        jb     SHORT loc_1_0292
        test   byte ptr [esi + 1], 40h
        jne    SHORT loc_1_028D
        test   byte ptr [esi + 1], 0ch
        jne    SHORT sub_1_022C
        mov    word ptr [esi + 36h], cx
        test   byte ptr [edx], 10h
        jne    SHORT loc_1_022B
        test   byte ptr [edx], 8
        jne    SHORT sub_1_022C
        test   byte ptr [esi + 2], 80h
        je     NEAR PTR sub_1_057E
        call   sub_1_08CF
        jne    SHORT loc_1_02E6
        jmp    NEAR PTR sub_1_057E
loc_1_0280:
        call   sub_1_0320
        jb     SHORT loc_1_02E5
        test   byte ptr [esi + 1], 40h
        je     SHORT loc_1_02E5
loc_1_028D:
        jmp    NEAR PTR loc_1_05DF
loc_1_0292:
        push   eax
        push   ebx
        db  08Bh, 01Dh, 02Dh, 012h, 000h, 000h
        and    ebx, 70h
        movzx  eax, word ptr [esi + 36h]
        call   sub_1_0655
        pop    ebx
        pop    eax
        jmp    SHORT loc_1_02CF
loc_1_02AA:
        call   sub_1_0320
        jb     SHORT loc_1_02E5
        call   sub_1_098A
        db  066h, 0F7h, 005h, 02Dh, 012h, 000h, 000h, 00Eh, 07Eh
        je     SHORT loc_1_02E0
        call   sub_1_0A24
        jb     SHORT loc_1_02CF
        call   sub_1_022C
        jmp    SHORT loc_1_02D4
loc_1_02CF:
        db  0E8h, 033h, 002h, 0FFh, 0FFh
loc_1_02D4:
        call   sub_1_0F45
        je     SHORT loc_1_02E5
        jmp    NEAR PTR loc_1_01B8
loc_1_02E0:
        db  0E8h, 002h, 003h, 0FFh, 0FFh
loc_1_02E5:
        ret
loc_1_02E6:
        test   ah, 40h
        jne    SHORT loc_1_02E5
        btr    dword ptr [esi], 19h
        jae    SHORT loc_1_02F9
        push   esi
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    ecx
loc_1_02F9:
        test   al, 80h
        je     SHORT loc_1_02E5
        and    byte ptr [esi + 2], 7fh
        ret
loc_1_0302:
        test   byte ptr [edx + 2], 80h
        je     SHORT loc_1_031D
        or     byte ptr [edx + 3], 2
loc_1_030C:
        push   5
        push   edx
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   byte ptr [edx + 3], 2
        jne    SHORT loc_1_030C
loc_1_031D:
        ret
        db  08Bh, 0C0h
sub_1_0320:
        db  00Fh, 0BAh, 02Dh, 051h, 012h, 000h, 000h, 001h
        jb     SHORT loc_1_0348
loc_1_032A:
        call   sub_1_0F54
        jb     SHORT loc_1_0341
        call   sub_1_034A
        je     SHORT loc_1_032A
        db  080h, 025h, 051h, 012h, 000h, 000h, 0FDh
        clc
        ret
loc_1_0341:
        db  080h, 025h, 051h, 012h, 000h, 000h, 0FDh
loc_1_0348:
        stc
        ret
sub_1_034A:
        push   ebx
        db  08Bh, 01Dh, 03Dh, 012h, 000h, 000h
        cmp    al, 0e0h
        je     SHORT loc_1_0381
        cmp    al, 0e1h
        je     SHORT loc_1_038E
        test   bh, 40h
        jne    SHORT loc_1_0393
        test   bh, 80h
        je     SHORT loc_1_036A
        call   sub_1_08C3
        je     SHORT loc_1_037D
loc_1_036A:
        mov    bl, al
        mov    eax, ebx
        db  0C7h, 005h, 03Dh, 012h, 000h, 000h, 000h, 000h, 000h, 000h
        or     bl, 1
        pop    ebx
        ret
loc_1_037D:
        xor    ebx, ebx
        jmp    SHORT loc_1_0384
loc_1_0381:
        or     bh, 80h
loc_1_0384:
        db  089h, 01Dh, 03Dh, 012h, 000h, 000h
        xor    ebx, ebx
        pop    ebx
        ret
loc_1_038E:
        or     bh, 40h
        jmp    SHORT loc_1_0384
loc_1_0393:
        cmp    al, 1dh
        je     SHORT loc_1_0384
        cmp    al, 45h
        je     SHORT loc_1_0384
        cmp    al, 9dh
        je     SHORT loc_1_0384
        jmp    SHORT loc_1_036A
        db  02Eh, 08Bh, 0C0h
sub_1_03A4:
        test   word ptr [esi + 1], 110h
        jne    NEAR PTR loc_1_046A
        bts    dword ptr [esi], 1ch
        jb     NEAR PTR loc_1_046A
        push   eax
        push   ebx
        push   ecx
        db  0A1h, 00Ch, 012h, 000h, 000h
        mov    ebx, edx
        db  0CDh, 020h, 006h, 000h, 003h, 000h
        test   byte ptr [esi + 3ch], 1
        je     SHORT loc_1_03D9
        test   cl, 2
        jne    NEAR PTR loc_1_0463
loc_1_03D9:
        test   byte ptr [esi + 3bh], 1
        jne    SHORT loc_1_0442
        test   byte ptr [esi], 7
        jne    SHORT loc_1_0463
        test   byte ptr [esi + 3ch], 10h
        jne    SHORT loc_1_0463
        call   sub_1_0D73
        jae    SHORT loc_1_043D
        movzx  eax, byte ptr [esi + 34h]
        cmp    al, byte ptr [esi + 35h]
        je     SHORT loc_1_0463
        inc    eax
        and    eax, 7fh
        mov    byte ptr [esi + 34h], al
        mov    al, byte ptr [esi + eax + 7ah]
        db  083h, 03Dh, 055h, 012h, 000h, 000h, 000h
        je     SHORT loc_1_0419
        push   eax
        db  0CDh, 020h, 040h, 000h, 001h, 000h
        mov    dword ptr [esi + 30h], eax
        pop    eax
loc_1_0419:
        xor    ah, ah
        test   byte ptr [esi + 1], 8
        jne    SHORT loc_1_043D
        mov    ah, al
        and    ah, 7fh
        call   sub_1_08C3
        je     SHORT loc_1_043D
        cmp    ah, 1dh
        je     SHORT loc_1_043D
        cmp    ah, 38h
        je     SHORT loc_1_043D
        db  0CDh, 020h, 006h, 000h, 00Ah, 000h
loc_1_043D:
        call   sub_1_0DDE
loc_1_0442:
        test   byte ptr [esi + 3ch], 1
        je     SHORT loc_1_0463
        test   cl, 1
        jne    SHORT loc_1_0463
        cmp    ah, 38h
        jne    SHORT loc_1_0456
        or     byte ptr [esi + 2], 1
loc_1_0456:
        mov    ebx, edx
        db  0A1h, 00Ch, 012h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_1_0463:
        pop    ecx
        pop    ebx
        pop    eax
        and    byte ptr [esi + 3], 0efh
loc_1_046A:
        ret
        db  090h
sub_1_046C:
        push   ebx
        movzx  ebx, byte ptr [esi + 35h]
        inc    ebx
        and    ebx, 7fh
        cmp    bl, byte ptr [esi + 34h]
        je     SHORT loc_1_0484
        mov    byte ptr [esi + 35h], bl
        mov    byte ptr [esi + ebx + 7ah], al
        clc
        jmp    SHORT loc_1_0485
loc_1_0484:
        stc
loc_1_0485:
        pop    ebx
        ret
        db  090h
sub_1_0488:
        push   eax
        test   byte ptr [esi + 2], 0ch
        je     SHORT loc_1_04D3
        cmp    eax, dword ptr [esi + 28h]
        je     SHORT loc_1_04B6
        btr    ax, 7
        jae    SHORT loc_1_04BA
        cmp    eax, dword ptr [esi + 28h]
        je     SHORT loc_1_04BA
        cmp    al, 2ah
        jne    SHORT loc_1_04AA
        and    byte ptr [esi + 2], 0fbh
        jmp    SHORT loc_1_04B6
loc_1_04AA:
        cmp    al, 36h
        jne    SHORT loc_1_04B6
        and    byte ptr [esi + 2], 0f7h
        jmp    SHORT loc_1_04B6
loc_1_04B4:
        pop    edx
        pop    ecx
loc_1_04B6:
        xor    al, al
        pop    eax
        ret
loc_1_04BA:
        mov    ah, 36h
        btr    dword ptr [esi], 13h
        call   sub_1_0565
        mov    ah, 2ah
        btr    dword ptr [esi], 12h
        call   sub_1_0565
        mov    eax, dword ptr [esp]
loc_1_04D3:
        test   al, 80h
        jne    SHORT loc_1_04B6
        test   ah, 80h
        je     SHORT loc_1_04B6
        movzx  eax, al
        cmp    al, 53h
        ja     SHORT loc_1_04B6
        sub    al, 35h
        jl     SHORT loc_1_04B6
        db  00Fh, 0A3h, 005h, 041h, 012h, 000h, 000h
        jae    SHORT loc_1_04B6
        push   ecx
        push   edx
        movzx  ecx, word ptr [esi + 36h]
        and    ecx, 1a1h
        xor    eax, eax
        mov    edx, dword ptr [esp + 8]
        cmp    dl, 37h
        je     SHORT loc_1_0515
        or     al, 1
        cmp    dl, 35h
        je     SHORT loc_1_0515
        test   cl, 20h
        je     SHORT loc_1_0515
        xor    al, al
loc_1_0515:
        and    cl, 0dfh
        cmp    cx, ax
        jne    SHORT loc_1_04B4
        and    byte ptr [esi + 2], 0fdh
        mov    ah, 2ah
        mov    edx, 12h
        test   cl, 1
        je     SHORT loc_1_054F
        movzx  ecx, word ptr [esi + 36h]
        or     byte ptr [esi + 2], 2
        test   cl, 2
        je     SHORT loc_1_0548
        test   ch, 2
        je     SHORT loc_1_054F
        bts    dword ptr [esi], edx
        stc
        call   sub_1_0565
loc_1_0548:
        mov    ah, 36h
        mov    edx, 13h
loc_1_054F:
        bts    dword ptr [esi], edx
        stc
        call   sub_1_0565
        btc    dword ptr [esi], 11h
        or     al, 1
        pop    edx
        pop    ecx
        pop    eax
        mov    dword ptr [esi + 28h], eax
        ret
sub_1_0565:
        jae    SHORT loc_1_057D
        mov    al, 0e0h
        call   sub_1_046C
        mov    al, ah
        test   byte ptr [esi + 2], 2
        je     SHORT loc_1_0578
        or     al, 80h
loc_1_0578:
        call   sub_1_046C
loc_1_057D:
        ret
sub_1_057E:
        test   ah, 40h
        jne    SHORT loc_1_05B3
        test   al, 80h
        jne    SHORT loc_1_058E
        call   sub_1_0488
        jne    SHORT loc_1_0593
loc_1_058E:
        test   ah, 80h
        je     SHORT loc_1_059C
loc_1_0593:
        push   eax
        mov    al, 0e0h
        call   sub_1_046C
        pop    eax
loc_1_059C:
        call   sub_1_046C
        test   al, 80h
        je     SHORT loc_1_05AA
        call   sub_1_0488
loc_1_05AA:
        mov    dword ptr [esi + 2ch], eax
        call   sub_1_03A4
        ret
loc_1_05B3:
        mov    al, 0e1h
        call   sub_1_046C
        mov    al, 1dh
        call   sub_1_046C
        mov    al, 45h
        call   sub_1_046C
        mov    al, 0e1h
        call   sub_1_046C
        mov    al, 9dh
        call   sub_1_046C
        mov    al, 0c5h
        call   sub_1_046C
        jmp    SHORT loc_1_05AA
loc_1_05DF:
        db  03Bh, 015h, 05Ch, 012h, 000h, 000h
        jne    SHORT loc_1_063D
        push   eax
        push   ebx
        call   sub_1_08CF
        je     SHORT loc_1_063A
        test   al, 80h
        jne    SHORT loc_1_062D
        db  03Ah, 005h, 088h, 012h, 000h, 000h
        je     SHORT loc_1_063A
        db  08Bh, 01Dh, 02Dh, 012h, 000h, 000h
        or     bl, bh
        and    bl, 0eh
        and    ah, 80h
        or     ah, bl
        db  08Bh, 01Dh, 064h, 012h, 000h, 000h
        add    ebx, 2
        and    ebx, 1fh
        db  03Bh, 01Dh, 060h, 012h, 000h, 000h
        je     SHORT loc_1_0634
        db  089h, 01Dh, 064h, 012h, 000h, 000h
        db  066h, 089h, 083h, 068h, 012h, 000h, 000h
loc_1_062D:
        db  0A2h, 088h, 012h, 000h, 000h
        jmp    SHORT loc_1_063A
loc_1_0634:
        db  0CDh, 020h, 001h, 000h, 00Bh, 000h
loc_1_063A:
        pop    ebx
        pop    eax
        ret
loc_1_063D:
        ret
sub_1_063E:
        push   ebx
        or     ebx, edx
        test   bh, 8
        pop    ebx
        clc
        je     SHORT loc_1_0654
        mov    ah, al
        mov    al, 0e0h
        db  0FFh, 015h, 035h, 012h, 000h, 000h
        mov    al, ah
loc_1_0654:
        ret
sub_1_0655:
        push   ecx
        push   edi
        push   edx
        mov    edx, eax
        mov    edi, ebx
        mov    ecx, 0fe7eh
        and    edx, ecx
        and    edi, ecx
        cmp    edi, edx
        je     SHORT loc_1_06DF
        mov    ecx, 0f787h
        and    eax, ecx
        cmp    eax, 100h
        jne    SHORT loc_1_06AF
        and    ecx, ebx
        jne    SHORT loc_1_06AF
        mov    al, byte ptr [esi + 2ch]
        and    al, 7fh
        cmp    al, 38h
        jne    SHORT loc_1_06AF
        or     al, 80h
        call   sub_1_063E
        jb     SHORT loc_1_06AF
        db  0FFh, 015h, 035h, 012h, 000h, 000h
        jb     SHORT loc_1_06DF
        xor    al, 80h
        db  0FFh, 015h, 035h, 012h, 000h, 000h
        jb     SHORT loc_1_06DF
        or     al, 80h
        db  0FFh, 015h, 035h, 012h, 000h, 000h
        jb     SHORT loc_1_06DF
        and    edx, 0fffff7f7h
loc_1_06AF:
        mov    ecx, edx
        xor    ecx, edi
        je     SHORT loc_1_06D2
        and    ecx, 70h
        je     SHORT loc_1_06C3
        call   sub_1_06E3
        jb     SHORT loc_1_06DF
        jmp    SHORT loc_1_06AF
loc_1_06C3:
        mov    ecx, edx
        xor    ecx, edi
        je     SHORT loc_1_06D2
        call   sub_1_06E3
        jb     SHORT loc_1_06DF
        jmp    SHORT loc_1_06C3
loc_1_06D2:
        mov    eax, edx
        call   sub_1_096C
        mov    word ptr [esi + 36h], ax
        or     esi, esi
loc_1_06DF:
        pop    edx
        pop    edi
        pop    ecx
        ret
sub_1_06E3:
        bsf    ecx, ecx
        xor    ebx, ebx
        inc    ebx
        shl    ebx, cl
        xor    edx, ebx
        shl    ecx, 1
        db  08Ah, 081h, 0E0h, 012h, 000h, 000h
        db  00Fh, 0B6h, 089h, 0E1h, 012h, 000h, 000h
        db  0FFh, 0A1h, 002h, 007h, 000h, 000h
loc_1_0702:
        and    byte ptr [edi], al
        add    byte ptr [eax], al
        adc    al, byte ptr [edi]
        add    byte ptr [eax], al
        sub    dword ptr [edi], eax
        add    byte ptr [eax], al
        dec    ebx
        pop    es
        add    byte ptr [eax], al
loc_1_0712:
        mov    cl, al
        mov    al, 0e0h
        db  0FFh, 015h, 035h, 012h, 000h, 000h
        jb     SHORT loc_1_077F
        mov    al, cl
loc_1_0720:
        test   bx, di
        jne    SHORT loc_1_0779
loc_1_0725:
        or     al, 80h
        jmp    SHORT loc_1_0779
loc_1_0729:
        xchg   bl, bh
        test   bx, dx
        je     SHORT loc_1_073C
        or     al, 80h
        db  0FFh, 015h, 035h, 012h, 000h, 000h
        jb     SHORT loc_1_077F
        xor    al, 80h
loc_1_073C:
        db  0FFh, 015h, 035h, 012h, 000h, 000h
        jb     SHORT loc_1_077F
        test   bx, di
        jne    SHORT loc_1_077F
        je     SHORT loc_1_0725
loc_1_074B:
        call   sub_1_063E
        jb     SHORT loc_1_077F
        test   bx, di
        jne    SHORT loc_1_0779
        or     al, 80h
        jb     SHORT loc_1_077F
        db  0FFh, 015h, 035h, 012h, 000h, 000h
        jb     SHORT loc_1_077F
        xor    al, 80h
        db  0FFh, 015h, 035h, 012h, 000h, 000h
        jb     SHORT loc_1_077F
        db  0FFh, 015h, 035h, 012h, 000h, 000h
        jb     SHORT loc_1_077F
        or     al, 80h
        jmp    SHORT loc_1_0779
loc_1_0779:
        db  0FFh, 015h, 035h, 012h, 000h, 000h
loc_1_077F:
        ret
loc_1_0780:
        mov    edi, ebx
        db  003h, 03Dh, 010h, 012h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        sti
        test   byte ptr [edi + 1], 8
        je     SHORT loc_1_079B
        or     byte ptr [edi + 1], 10h
        jmp    SHORT loc_1_07A6
loc_1_079B:
        db  0A1h, 024h, 012h, 000h, 000h
        db  0CDh, 020h, 037h, 000h, 001h, 000h
loc_1_07A6:
        and    byte ptr [edi], 0feh
        mov    edx, edi
        jmp    NEAR PTR sub_1_01B0
loc_1_07B0:
        call   sub_1_08F6
        mov    eax, 417h
        add    eax, dword ptr [ebx + 4]
        mov    al, byte ptr [eax]
        and    al, 70h
        db  003h, 01Dh, 010h, 012h, 000h, 000h
        mov    byte ptr [ebx + 42h], al
        ret
        db  090h
loc_1_07CC:
        sti
        mov    edx, ebx
        db  003h, 015h, 010h, 012h, 000h, 000h
        mov    esi, edx
        mov    eax, 417h
        add    eax, dword ptr [ebx + 4]
        mov    al, byte ptr [eax]
        and    al, 70h
        cmp    al, byte ptr [esi + 42h]
        je     SHORT loc_1_07EB
        mov    byte ptr [esi + 41h], al
loc_1_07EB:
        test   byte ptr [esi + 2], 1
        jne    SHORT loc_1_0816
        test   byte ptr [esi + 1], 8
        je     SHORT loc_1_0811
        mov    al, byte ptr [esi + 3ah]
        and    al, 7fh
        cmp    al, 38h
        mov    eax, dword ptr [esi + 4bh]
        jne    SHORT loc_1_0806
        mov    eax, dword ptr [esi + 47h]
loc_1_0806:
        db  0BEh, 024h, 008h, 000h, 000h
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
loc_1_0811:
        jmp    NEAR PTR sub_1_01B0
loc_1_0816:
        mov    eax, dword ptr [esi + 43h]
        db  0BEh, 0CAh, 003h, 000h, 000h
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
loc_1_0824:
        and    byte ptr [edx + 1], 0efh
        jmp    NEAR PTR sub_1_01B0
        db  0EBh, 006h, 0FFh, 025h, 031h, 012h, 000h, 000h
loc_1_0835:
        push   ebx
        push   esi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    esi, ebx
        db  003h, 035h, 010h, 012h, 000h, 000h
        cmp    dword ptr [esi + 30h], 0
        je     SHORT loc_1_0865
        push   eax
        db  0CDh, 020h, 040h, 000h, 001h, 000h
        sub    eax, dword ptr [esi + 30h]
        db  03Bh, 005h, 055h, 012h, 000h, 000h
        pop    eax
        jb     SHORT loc_1_0882
        mov    dword ptr [esi + 30h], 0
loc_1_0865:
        test   byte ptr [esi + 1], 0ch
        jne    SHORT loc_1_0873
        pop    esi
        pop    ebx
        db  0FFh, 025h, 031h, 012h, 000h, 000h
loc_1_0873:
        test   byte ptr [esi + 1], 4
        je     SHORT loc_1_0882
        and    byte ptr [esi + 1], 0feh
        db  0E8h, 088h, 0FBh, 0FEh, 0FFh
loc_1_0882:
        pop    esi
        pop    ebx
        clc
        ret
        db  08Bh, 0C0h
sub_1_0888:
        db  0BFh, 028h, 012h, 000h, 000h
        test   ah, 80h
        je     SHORT loc_1_08AF
        cmp    byte ptr [edi], 0
        je     SHORT loc_1_08A4
        cmp    al, 0dbh
        je     SHORT loc_1_089F
        cmp    al, 0dch
        jne    SHORT loc_1_08B4
loc_1_089F:
        mov    byte ptr [edi], 0
        jmp    SHORT loc_1_08B4
loc_1_08A4:
        cmp    al, 5bh
        je     SHORT loc_1_08AC
        cmp    al, 5ch
        jne    SHORT loc_1_08C2
loc_1_08AC:
        mov    byte ptr [edi], 1
loc_1_08AF:
        cmp    byte ptr [edi], 0
        je     SHORT loc_1_08C2
loc_1_08B4:
        db  08Bh, 015h, 029h, 012h, 000h, 000h
        db  08Bh, 035h, 010h, 012h, 000h, 000h
        add    esi, edx
loc_1_08C2:
        ret
sub_1_08C3:
        push   eax
        and    al, 7fh
        cmp    al, 2ah
        je     SHORT loc_1_08CD
        cmp    al, 36h
        stc
loc_1_08CD:
        pop    eax
        ret
sub_1_08CF:
        push   eax
        test   ah, 40h
        jne    SHORT loc_1_08F2
        and    al, 7fh
        cmp    al, 46h
        ja     SHORT loc_1_08F2
        sub    al, 1dh
        jb     SHORT loc_1_08F2
        movzx  eax, al
        shl    eax, 1
        db  066h, 083h, 0B8h, 08Ch, 012h, 000h, 000h, 000h
        je     SHORT loc_1_08F2
        xor    eax, eax
        jmp    SHORT loc_1_08F4
loc_1_08F2:
        or     al, 1
loc_1_08F4:
        pop    eax
        ret
sub_1_08F6:
        push   ebx
        call   sub_1_0922
        jb     SHORT loc_1_0920
        and    al, 70h
        mov    ah, al
        db  003h, 01Dh, 010h, 012h, 000h, 000h
        xor    al, byte ptr [ebx + 41h]
        je     SHORT loc_1_0920
        xor    byte ptr [ebx + 41h], al
        xor    ah, byte ptr [ebx + 36h]
        and    ah, al
        je     SHORT loc_1_0920
        db  030h, 005h, 02Dh, 012h, 000h, 000h
        xor    byte ptr [ebx + 36h], al
loc_1_0920:
        pop    ebx
        ret
sub_1_0922:
        test   byte ptr [ebx], 8
        je     SHORT loc_1_0933
        db  0A1h, 010h, 012h, 000h, 000h
        movzx  eax, word ptr [eax + ebx + 36h]
        stc
        ret
loc_1_0933:
        push   ebx
        mov    ebx, dword ptr [ebx + 4]
        movzx  eax, word ptr [ebx + 417h]
        mov    bh, byte ptr [ebx + 496h]
        and    bh, 0ch
        mov    bl, ah
        and    bl, 3
        shl    bl, 2
        and    ah, 0f0h
        test   al, 0ch
        je     SHORT loc_1_0960
        or     ah, bh
        or     bh, bl
        je     SHORT loc_1_0960
        and    al, 0f3h
        or     al, bl
loc_1_0960:
        test   al, 1
        je     SHORT loc_1_096B
        and    al, 0feh
        or     eax, 200h
loc_1_096B:
        pop    ebx
sub_1_096C:
        and    ax, 0fe7eh
        test   ax, 202h
        je     SHORT loc_1_0978
        or     al, 1
loc_1_0978:
        test   ax, 404h
        je     SHORT loc_1_0980
        or     al, 80h
loc_1_0980:
        test   ax, 808h
        je     SHORT loc_1_0989
        or     ah, 1
loc_1_0989:
        ret
sub_1_098A:
        push   eax
        push   ebx
        push   edx
        db  08Bh, 01Dh, 02Dh, 012h, 000h, 000h
        movzx  edx, word ptr [esi + 36h]
        mov    ecx, eax
        and    al, 7fh
        cmp    al, 46h
        ja     SHORT loc_1_0A1C
        sub    al, 1dh
        jb     SHORT loc_1_0A1C
        movzx  eax, al
        shl    eax, 1
        db  066h, 08Bh, 080h, 08Ch, 012h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0A1C
        test   ax, 70h
        jne    SHORT loc_1_09D3
        test   ch, 80h
        je     SHORT loc_1_09C0
        xchg   al, ah
loc_1_09C0:
        test   cl, 80h
        je     SHORT loc_1_09CD
        not    eax
        and    ebx, eax
        and    edx, eax
        jmp    SHORT loc_1_0A04
loc_1_09CD:
        or     ebx, eax
        or     edx, eax
        jmp    SHORT loc_1_0A04
loc_1_09D3:
        mov    ah, al
        test   bx, 80h
        jne    SHORT loc_1_09E5
        db  0F6h, 005h, 051h, 012h, 000h, 000h, 001h
        je     SHORT loc_1_09E7
loc_1_09E5:
        and    al, 8fh
loc_1_09E7:
        test   cl, 80h
        jne    SHORT loc_1_09F2
        test   ah, bh
        je     SHORT loc_1_09FC
        jmp    SHORT loc_1_0A1C
loc_1_09F2:
        and    al, 8fh
        not    ah
        and    bh, ah
        and    dh, ah
        xor    ah, ah
loc_1_09FC:
        xor    bl, al
        xor    dl, al
        or     bh, ah
        or     dh, ah
loc_1_0A04:
        mov    eax, ebx
        call   sub_1_096C
        mov    ebx, eax
        mov    eax, edx
        call   sub_1_096C
        mov    edx, eax
        db  089h, 01Dh, 02Dh, 012h, 000h, 000h
loc_1_0A1C:
        mov    ecx, edx
        pop    edx
        pop    ebx
        pop    eax
        ret
        db  000h, 000h
sub_1_0A24:
        push   eax
        db  0F7h, 005h, 02Dh, 012h, 000h, 000h, 00Eh, 07Eh, 000h, 000h
        jne    SHORT loc_1_0A40
        and    eax, 807fh
        cmp    eax, 8045h
        je     SHORT loc_1_0A4B
        mov    eax, dword ptr [esp]
loc_1_0A40:
        test   al, 80h
        je     SHORT loc_1_0A4D
        db  0F6h, 005h, 051h, 012h, 000h, 000h, 001h
loc_1_0A4B:
        je     SHORT loc_1_0AC0
loc_1_0A4D:
        pushad
        mov    edi, esi
        mov    cx, ax
        db  08Bh, 035h, 008h, 013h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_0A8D
        and    cx, 807fh
loc_1_0A66:
        cmp    cl, byte ptr [eax]
        jne    SHORT loc_1_0A85
        cmp    byte ptr [eax + 2], 0ffh
        je     SHORT loc_1_0A75
        cmp    ch, byte ptr [eax + 2]
        jne    SHORT loc_1_0A85
loc_1_0A75:
        db  08Bh, 01Dh, 02Dh, 012h, 000h, 000h
        and    bx, word ptr [eax + 4]
        cmp    bx, word ptr [eax + 6]
        je     SHORT loc_1_0A90
loc_1_0A85:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_1_0A66
loc_1_0A8D:
        clc
        jmp    SHORT loc_1_0ABF
loc_1_0A90:
        mov    ebx, dword ptr [eax + 8]
        bt     dword ptr [edi + 0ch], ebx
        jae    SHORT loc_1_0AB0
        test   byte ptr [esp + 1ch], 80h
        je     SHORT loc_1_0A85
        and    byte ptr [eax + 3], 7fh
        mov    bl, byte ptr [esp + 1ch]
        db  088h, 01Dh, 014h, 013h, 000h, 000h
        jmp    SHORT loc_1_0A85
loc_1_0AB0:
        test   byte ptr [eax + 3], 40h
        db  00Fh, 085h, 032h, 0FAh, 0FEh, 0FFh
        stc
        mov    dword ptr [esp + 10h], eax
loc_1_0ABF:
        popad
loc_1_0AC0:
        pop    eax
        ret
        db  000h, 000h
loc_1_0AC4:
        mov    esi, ebx
        db  003h, 035h, 010h, 012h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_0AD7
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0AD7:
        je     SHORT loc_1_0AE8
        call   sub_1_10A4
        jb     SHORT loc_1_0AE7
        in     al, 64h
        and    al, 14h
        or     al, byte ptr [esi + 3bh]
loc_1_0AE7:
        ret
loc_1_0AE8:
        movzx  eax, al
        db  08Ah, 0A0h, 01Ch, 013h, 000h, 000h
        or     ah, ah
        je     NEAR PTR loc_1_0B80
        mov    byte ptr [esi + 3dh], al
        mov    dl, ah
        and    dl, 1fh
        mov    byte ptr [esi + 40h], dl
        mov    dl, ah
        and    dl, 0c0h
        cmp    dl, 40h
        je     SHORT loc_1_0B59
        cmp    dl, 0c0h
        je     SHORT loc_1_0B1B
        db  03Bh, 01Dh, 014h, 012h, 000h, 000h
        jne    SHORT loc_1_0B59
loc_1_0B1B:
        mov    ecx, 10000h
loc_1_0B20:
        db  0F6h, 005h, 051h, 012h, 000h, 000h, 004h
        loopne loc_1_0B20
        db  089h, 01Dh, 018h, 013h, 000h, 000h
        cmp    byte ptr [esi + 40h], 0
        je     SHORT loc_1_0B4E
        test   ah, 20h
        jne    SHORT loc_1_0B54
        db  080h, 00Dh, 051h, 012h, 000h, 000h, 004h
        push   eax
        mov    al, 0adh
        call   sub_1_0F3D
        or     byte ptr [esi + 3bh], 2
        pop    eax
loc_1_0B4E:
        call   sub_1_0F3D
        ret
loc_1_0B54:
        or     byte ptr [esi], 20h
        jmp    SHORT loc_1_0BD6
loc_1_0B59:
        test   ah, 20h
        jne    SHORT loc_1_0B7B
        xor    al, al
        movzx  ecx, byte ptr [esi + 40h]
        cmp    cl, 1fh
        jne    SHORT loc_1_0B6B
        mov    cl, 1
loc_1_0B6B:
        xchg   ecx, eax
        call   sub_1_0D89
        xchg   ecx, eax
        loop   loc_1_0B6B
        mov    edx, ebx
        jmp    NEAR PTR loc_1_0D9D
loc_1_0B7B:
        or     byte ptr [esi], 40h
        jmp    SHORT loc_1_0BD6
loc_1_0B80:
        cmp    al, 0adh
        jne    SHORT loc_1_0B8A
        or     byte ptr [esi + 3ch], 10h
        jmp    SHORT loc_1_0BD6
loc_1_0B8A:
        cmp    al, 0aeh
        jne    SHORT loc_1_0B94
        and    byte ptr [esi + 3ch], 0efh
        jmp    SHORT loc_1_0BD6
loc_1_0B94:
        cmp    al, 20h
        jne    SHORT loc_1_0BA2
        mov    al, byte ptr [esi + 3ch]
        call   sub_1_0DB8
        jmp    SHORT loc_1_0BD6
loc_1_0BA2:
        cmp    al, 60h
        jne    SHORT loc_1_0BAB
        or     byte ptr [esi], 8
        jmp    SHORT loc_1_0BD6
loc_1_0BAB:
        cmp    al, 0d1h
        jne    SHORT loc_1_0BB4
        or     byte ptr [esi], 10h
        jmp    SHORT loc_1_0BD6
loc_1_0BB4:
        cmp    al, 0f0h
        jb     SHORT loc_1_0BC3
        or     al, 2
        test   al, 1
        jne    SHORT loc_1_0B4E
        db  0E9h, 01Dh, 0F5h, 0FEh, 0FFh
loc_1_0BC3:
        call   sub_1_10EC
        je     SHORT loc_1_0BD6
        db  03Bh, 01Dh, 014h, 012h, 000h, 000h
        je     NEAR PTR loc_1_0B4E
loc_1_0BD6:
        ret
        db  090h
loc_1_0BD8:
        mov    esi, ebx
        db  003h, 035h, 010h, 012h, 000h, 000h
        cmp    ecx, 4
        jbe    SHORT loc_1_0BEB
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0BEB:
        je     SHORT loc_1_0C3D
        call   sub_1_1100
        jb     SHORT loc_1_0C3C
        mov    al, byte ptr [esi + 39h]
        mov    byte ptr [esi + 3bh], 0
        and    byte ptr [esi], 0feh
        push   eax
        db  0A1h, 00Ch, 012h, 000h, 000h
        db  0CDh, 020h, 006h, 000h, 003h, 000h
        test   cl, 2
        jne    SHORT loc_1_0C2D
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        test   byte ptr [esi + 2], 1
        je     SHORT loc_1_0C2D
        push   esi
        mov    eax, dword ptr [esi + 43h]
        mov    edx, esi
        db  0BEh, 0CAh, 003h, 000h, 000h
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
        pop    esi
loc_1_0C2D:
        pop    eax
        call   sub_1_0D6A
        je     SHORT loc_1_0C3C
        mov    edx, ebx
        jmp    NEAR PTR sub_1_03A4
loc_1_0C3C:
        ret
loc_1_0C3D:
        mov    ecx, dword ptr [esi]
        test   cl, 20h
        jne    SHORT loc_1_0CB2
        test   cl, 40h
        jne    NEAR PTR loc_1_0CDF
        test   cl, 8
        jne    SHORT loc_1_0C95
        test   cl, 10h
        jne    SHORT loc_1_0C9D
        test   cl, 6
        je     NEAR PTR loc_1_0D03
        btr    ecx, 1
        jae    SHORT loc_1_0C84
        test   al, 80h
        jne    NEAR PTR loc_1_0D00
        db  03Bh, 01Dh, 014h, 012h, 000h, 000h
        jne    NEAR PTR loc_1_0D61
        call   sub_1_0FD3
        jmp    NEAR PTR loc_1_0D61
loc_1_0C84:
        test   al, 80h
        jne    SHORT loc_1_0D00
        call   sub_1_1014
        and    cl, 0fbh
        jmp    NEAR PTR loc_1_0D61
loc_1_0C95:
        and    byte ptr [esi], 0f7h
        mov    byte ptr [esi + 3ch], al
        jmp    SHORT loc_1_0C3C
loc_1_0C9D:
        and    byte ptr [esi], 0efh
        or     al, 2
        push   eax
        mov    al, 0d1h
        call   sub_1_0F3D
        pop    eax
        call   sub_1_0F35
        jmp    SHORT loc_1_0C3C
loc_1_0CB2:
        mov    cl, al
        call   sub_1_0D89
        mov    ch, byte ptr [esi + 40h]
        cmp    ch, 1fh
        jne    SHORT loc_1_0CC7
        or     cl, cl
        je     SHORT loc_1_0CD2
        inc    ch
loc_1_0CC7:
        dec    ch
        mov    byte ptr [esi + 40h], ch
        jne    NEAR PTR loc_1_0C3C
loc_1_0CD2:
        call   sub_1_0DE9
        and    byte ptr [esi], 0dfh
        jmp    NEAR PTR loc_1_0C3C
loc_1_0CDF:
        mov    cl, byte ptr [esi + 40h]
        cmp    cl, 1fh
        jne    SHORT loc_1_0CED
        or     cl, cl
        je     SHORT loc_1_0CF8
        inc    cl
loc_1_0CED:
        dec    cl
        mov    byte ptr [esi + 40h], al
        jne    NEAR PTR loc_1_0C3C
loc_1_0CF8:
        and    byte ptr [esi], 0bfh
        jmp    NEAR PTR loc_1_0C3C
loc_1_0D00:
        and    cl, 0f8h
loc_1_0D03:
        cmp    al, 93h
        je     SHORT loc_1_0D32
        test   byte ptr [esi + 3], 8
        jne    SHORT loc_1_0D38
        cmp    al, 0edh
        jb     SHORT loc_1_0D38
        ja     SHORT loc_1_0D18
        or     ecx, 2
        jmp    SHORT loc_1_0D61
loc_1_0D18:
        cmp    al, 0f3h
        jne    SHORT loc_1_0D21
        or     ecx, 4
        jmp    SHORT loc_1_0D61
loc_1_0D21:
        cmp    al, 0eeh
        jne    SHORT loc_1_0D29
        mov    al, 0eeh
        jmp    SHORT loc_1_0D66
loc_1_0D29:
        cmp    al, 0feh
        jne    SHORT loc_1_0D61
        mov    al, byte ptr [esi + 3ah]
        jmp    SHORT loc_1_0D66
loc_1_0D32:
        or     byte ptr [esi + 3], 4
        jmp    SHORT loc_1_0D50
loc_1_0D38:
        btr    dword ptr [esi], 1ah
        jae    SHORT loc_1_0D50
        cmp    al, 1
        je     SHORT loc_1_0D4C
        cmp    al, 5
        ja     SHORT loc_1_0D50
        and    byte ptr [esi + 3], 0f7h
        jmp    SHORT loc_1_0D50
loc_1_0D4C:
        or     byte ptr [esi + 3], 8
loc_1_0D50:
        and    byte ptr [esi], 80h
        db  03Bh, 01Dh, 014h, 012h, 000h, 000h
        jne    SHORT loc_1_0D60
        call   sub_1_0F35
loc_1_0D60:
        ret
loc_1_0D61:
        or     ecx, 1
        mov    al, 0fah
loc_1_0D66:
        mov    dword ptr [esi], ecx
        jmp    SHORT sub_1_0DB8
sub_1_0D6A:
        push   eax
        mov    al, byte ptr [esi + 3eh]
        cmp    al, byte ptr [esi + 3fh]
        pop    eax
        ret
sub_1_0D73:
        movzx  eax, byte ptr [esi + 3eh]
        cmp    al, byte ptr [esi + 3fh]
        stc
        je     SHORT loc_1_0D88
        inc    eax
        and    al, 1fh
        mov    byte ptr [esi + 3eh], al
        mov    al, byte ptr [esi + eax + 5ah]
        clc
loc_1_0D88:
        ret
sub_1_0D89:
        movzx  eax, byte ptr [esi + 3fh]
        inc    eax
        and    al, 1fh
        cmp    al, byte ptr [esi + 3eh]
        je     SHORT loc_1_0D9C
        mov    byte ptr [esi + 3fh], al
        mov    byte ptr [esi + eax + 5ah], cl
loc_1_0D9C:
        ret
loc_1_0D9D:
        mov    ebx, edx
        mov    esi, edx
        db  003h, 035h, 010h, 012h, 000h, 000h
        call   sub_1_0D73
        and    byte ptr [esi + 3bh], 0fdh
        call   sub_1_0DB8
        ret
        db  08Bh, 0C0h
sub_1_0DB8:
        call   sub_1_0DDE
        or     byte ptr [esi + 3bh], 8
        test   byte ptr [esi + 3ch], 1
        je     SHORT loc_1_0DDD
        db  0A1h, 00Ch, 012h, 000h, 000h
        db  0CDh, 020h, 006h, 000h, 003h, 000h
        test   cl, 2
        jne    SHORT loc_1_0DDD
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_1_0DDD:
        ret
sub_1_0DDE:
        mov    byte ptr [esi + 3bh], 1
        mov    byte ptr [esi + 39h], al
        mov    byte ptr [esi + 3ah], al
        ret
sub_1_0DE9:
        mov    al, byte ptr [esi + 3dh]
        call   sub_1_0F3D
loc_1_0DF1:
        call   sub_1_0D73
        jb     SHORT loc_1_0DFF
        call   sub_1_0F35
        jmp    SHORT loc_1_0DF1
loc_1_0DFF:
        ret
VKD_Filter_Keyboard_Input:
        clc
        ret
        db  08Bh, 0C0h
loc_1_0E04:
        xor    ecx, ecx
        dec    ch
loc_1_0E08:
        in     al, 64h
        test   al, 1
        jne    SHORT loc_1_0E12
        test   al, 2
        loopne loc_1_0E08
loc_1_0E12:
        mov    cl, al
        db  0A1h, 00Ch, 012h, 000h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  080h, 03Dh, 020h, 012h, 000h, 000h, 001h
        jne    SHORT loc_1_0E2E
        db  0CDh, 020h, 00Ch, 000h, 017h, 000h
loc_1_0E2E:
        test   cl, 1
        je     NEAR PTR loc_1_0EED
        in     al, 60h
        mov    cl, al
        db  08Bh, 035h, 010h, 012h, 000h, 000h
        db  0F6h, 005h, 051h, 012h, 000h, 000h, 004h
        jne    SHORT loc_1_0EB1
        cmp    cl, 0fah
        je     SHORT loc_1_0E95
        db  0CDh, 020h, 012h, 000h, 00Dh, 000h
        jb     NEAR PTR loc_1_0EED
        db  0A1h, 024h, 014h, 000h, 000h
        inc    eax
        and    eax, 7fh
        db  03Bh, 005h, 028h, 014h, 000h, 000h
        je     SHORT loc_1_0E77
        db  0A3h, 024h, 014h, 000h, 000h
        db  088h, 088h, 02Ch, 014h, 000h, 000h
loc_1_0E77:
        db  08Bh, 015h, 014h, 012h, 000h, 000h
        add    edx, esi
        cmp    dword ptr [edx + 4], 0
        jne    SHORT loc_1_0EED
        db  0BEh, 0A8h, 001h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    dword ptr [edx + 4], esi
        jmp    SHORT loc_1_0EED
loc_1_0E95:
        db  0BAh, 020h, 014h, 000h, 000h
        cmp    byte ptr [edx], 0
        je     SHORT loc_1_0EF8
        xor    al, al
        xchg   byte ptr [edx + 1], al
        btr    dword ptr [edx], 6
        jae    SHORT loc_1_0EEF
        call   sub_1_0F35
        jmp    SHORT loc_1_0EED
loc_1_0EB1:
        db  08Bh, 015h, 018h, 013h, 000h, 000h
        add    esi, edx
        call   sub_1_0D89
        mov    ch, byte ptr [esi + 40h]
        cmp    ch, 1fh
        jne    SHORT loc_1_0ECC
        or     cl, cl
        je     SHORT loc_1_0ED3
        inc    ch
loc_1_0ECC:
        dec    ch
        mov    byte ptr [esi + 40h], ch
        jne    SHORT loc_1_0EED
loc_1_0ED3:
        mov    al, 0aeh
        call   sub_1_0F3D
        db  080h, 025h, 051h, 012h, 000h, 000h, 0FBh
loc_1_0EE1:
        db  0BEh, 09Dh, 00Dh, 000h, 000h
        sti
        db  0CDh, 020h, 010h, 000h, 001h, 000h
loc_1_0EED:
        clc
        ret
loc_1_0EEF:
        db  0C6h, 005h, 020h, 014h, 000h, 000h, 000h
        jmp    SHORT loc_1_0EED
loc_1_0EF8:
        db  08Bh, 015h, 014h, 012h, 000h, 000h
        add    esi, edx
        call   sub_1_0D89
        jmp    SHORT loc_1_0EE1
sub_1_0F07:
        push   eax
        push   ecx
        db  0A1h, 00Ch, 012h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0F18
        db  0CDh, 020h, 008h, 000h, 003h, 000h
loc_1_0F18:
        mov    ecx, 10000h
loc_1_0F1D:
        in     al, 64h
        test   al, 2
        loopne loc_1_0F1D
        db  0A1h, 00Ch, 012h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0F32
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_1_0F32:
        pop    ecx
        pop    eax
        ret
sub_1_0F35:
        call   sub_1_0F07
        out    60h, al
        ret
sub_1_0F3D:
        call   sub_1_0F07
        out    64h, al
        ret
sub_1_0F45:
        db  0A1h, 028h, 014h, 000h, 000h
        db  03Bh, 005h, 024h, 014h, 000h, 000h
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_0F54:
        stc
        pushfd
        cli
        db  0A1h, 028h, 014h, 000h, 000h
        db  03Bh, 005h, 024h, 014h, 000h, 000h
        je     SHORT loc_1_0F77
        inc    eax
        and    eax, 7fh
        db  0A3h, 028h, 014h, 000h, 000h
        db  08Ah, 080h, 02Ch, 014h, 000h, 000h
        and    dword ptr [esp], 0fffffffeh
loc_1_0F77:
        popfd
        ret
VKD_Put_Byte:
        pushfd
        push   ebx
        cli
        db  08Bh, 01Dh, 024h, 014h, 000h, 000h
        inc    ebx
        and    ebx, 7fh
        db  03Bh, 01Dh, 028h, 014h, 000h, 000h
        je     SHORT loc_1_0F9F
        db  089h, 01Dh, 024h, 014h, 000h, 000h
        db  088h, 083h, 02Ch, 014h, 000h, 000h
        pop    ebx
        popfd
        clc
        jmp    SHORT loc_1_0FA2
loc_1_0F9F:
        pop    ebx
        popfd
        stc
loc_1_0FA2:
        ret
sub_1_0FA3:
        push   eax
loc_1_0FA4:
        in     al, 64h
        test   al, 1
        je     SHORT loc_1_0FAE
        in     al, 60h
        jmp    SHORT loc_1_0FA4
loc_1_0FAE:
        pop    eax
        ret
sub_1_0FB0:
        push   eax
        db  0CDh, 020h, 007h, 000h, 005h, 000h
        mov    esi, eax
loc_1_0FB9:
        db  080h, 03Dh, 020h, 014h, 000h, 000h, 000h
        je     SHORT loc_1_0FD1
        db  0CDh, 020h, 007h, 000h, 005h, 000h
        sub    eax, esi
        cmp    eax, 0f4240h
        jb     SHORT loc_1_0FB9
loc_1_0FD1:
        pop    eax
        ret
sub_1_0FD3:
        pushad
        call   sub_1_0FB0
        db  0A2h, 021h, 014h, 000h, 000h
        db  0C6h, 005h, 020h, 014h, 000h, 000h, 0C0h
        mov    al, 0edh
        call   sub_1_0F35
        xor    esi, esi
        db  087h, 035h, 01Ch, 014h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        mov    eax, 3e8h
        xor    edx, edx
        db  0BEh, 055h, 010h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 01Ch, 014h, 000h, 000h
        popad
        ret
sub_1_1014:
        pushad
        call   sub_1_0FB0
        db  0A2h, 021h, 014h, 000h, 000h
        db  0C6h, 005h, 020h, 014h, 000h, 000h, 0C0h
        mov    al, 0f3h
        call   sub_1_0F35
        xor    esi, esi
        db  087h, 035h, 01Ch, 014h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        mov    eax, 3e8h
        xor    edx, edx
        db  0BEh, 055h, 010h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 01Ch, 014h, 000h, 000h
        popad
        ret
loc_1_1055:
        db  0C7h, 005h, 01Ch, 014h, 000h, 000h, 000h, 000h, 000h, 000h
        db  080h, 03Dh, 020h, 014h, 000h, 000h, 000h
        je     SHORT loc_1_1076
        xor    eax, eax
        db  0A3h, 020h, 014h, 000h, 000h
        mov    al, 0f4h
        call   sub_1_0F35
loc_1_1076:
        ret
VKD_REBOOT_PROCESSOR:
        db  080h, 03Dh, 05Ah, 012h, 000h, 000h, 000h
        je     SHORT loc_1_10A2
        push   0
        push   2
        push   472h
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        inc    eax
        je     SHORT loc_1_109B
        dec    eax
        mov    word ptr [eax], 1234h
loc_1_109B:
        mov    al, 0feh
        call   sub_1_0F3D
loc_1_10A2:
        clc
        ret
sub_1_10A4:
        db  080h, 03Dh, 011h, 015h, 000h, 000h, 0FFh
        je     SHORT loc_1_10B8
        db  080h, 03Dh, 012h, 015h, 000h, 000h, 0FFh
        jne    SHORT loc_1_10D9
        clc
        ret
loc_1_10B8:
        push   ecx
        pushfd
        cli
        db  0E8h, 0C0h, 0F1h, 0FEh, 0FFh
        jae    SHORT loc_1_10E6
        db  08Ah, 004h, 04Dh, 0B9h, 014h, 000h, 000h
        mov    ah, al
        and    ah, 0fdh
        db  088h, 024h, 04Dh, 0B9h, 014h, 000h, 000h
loc_1_10D5:
        popfd
        pop    ecx
        stc
        ret
loc_1_10D9:
        push   ecx
        pushfd
        cli
        in     al, 64h
        test   al, 1
        je     SHORT loc_1_10E6
        test   al, 20h
        jne    SHORT loc_1_10D5
loc_1_10E6:
        popfd
        pop    ecx
        clc
        ret
        db  08Bh, 0C0h
sub_1_10EC:
        cmp    al, 0a7h
        je     SHORT loc_1_10FE
        cmp    al, 0a8h
        je     SHORT loc_1_10FE
        cmp    al, 0a9h
        je     SHORT loc_1_10FE
        cmp    al, 0d3h
        je     SHORT loc_1_10FE
        cmp    al, 0d4h
loc_1_10FE:
        ret
        db  090h
sub_1_1100:
        db  080h, 03Dh, 011h, 015h, 000h, 000h, 0FFh
        je     SHORT loc_1_1114
        db  080h, 03Dh, 012h, 015h, 000h, 000h, 0FFh
        jne    SHORT loc_1_1151
        clc
        ret
loc_1_1114:
        push   ecx
        pushfd
        cli
        db  0E8h, 064h, 0F1h, 0FEh, 0FFh
        jae    SHORT loc_1_1162
        db  08Ah, 004h, 04Dh, 0B8h, 014h, 000h, 000h
        db  066h, 0FFh, 00Dh, 0FCh, 014h, 000h, 000h
        inc    cx
        and    cx, 1fh
        db  066h, 089h, 00Dh, 0FAh, 014h, 000h, 000h
        push   eax
        db  0A1h, 0FEh, 014h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0C6h, 005h, 002h, 015h, 000h, 000h, 000h
        pop    eax
loc_1_114D:
        popfd
        pop    ecx
        stc
        ret
loc_1_1151:
        push   ecx
        pushfd
        cli
        in     al, 64h
        test   al, 1
        je     SHORT loc_1_1162
        test   al, 20h
        je     SHORT loc_1_1162
        in     al, 60h
        jmp    SHORT loc_1_114D
loc_1_1162:
        popfd
        pop    ecx
        clc
        ret
        db  000h, 000h, 064h, 001h, 000h, 000h, 06Bh, 001h, 000h, 000h, 0EAh, 001h, 000h, 000h, 02Eh, 002h
        db  000h, 000h, 045h, 002h, 000h, 000h, 05Ch, 002h, 000h, 000h, 09Eh, 002h, 000h, 000h, 0DAh, 000h
        db  000h, 000h, 024h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 019h, 000h, 000h, 000h, 0B8h, 000h
        db  000h, 000h, 02Bh, 001h, 000h, 000h, 058h, 001h, 000h, 000h, 080h, 001h, 000h, 000h, 03Ch, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 05Ch, 000h, 000h, 000h, 000h, 00Eh, 000h, 000h, 079h, 00Fh
        db  000h, 000h, 068h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 00Dh, 000h, 002h, 00Ah, 000h, 000h, 056h, 04Bh, 044h, 020h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  068h, 011h, 000h, 000h, 015h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h
loc_1_1231:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_1235:
        insb
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [7d5c00h], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_125A:
        inc    dword ptr [eax]
loc_1_125C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1260:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1264:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1268:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1288:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_128C:
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
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
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
loc_1_12E1:
        add    byte ptr [edx], ch
        add    byte ptr [46003800h], bl
        or     byte ptr [ebp + 8], al
        cmp    cl, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr ss:[46043804h], bl
        add    byte ptr [ebp], al
        cmp    al, byte ptr [eax]
loc_1_12FE:
        push   edx
        dec    edi
        push   eax
        push   ecx
        dec    ebx
        dec    esp
        dec    ebp
        inc    edi
        dec    eax
        dec    ecx
loc_1_1308:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_130C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1310:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1314:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1318:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_131C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    cl, al
        rol    ecx, 0c1h
        rol    ecx, 0c1h
        rol    ecx, 0c1h
        rol    ecx, 0c1h
        rol    ecx, 0c1h
        rol    ecx, 0c1h
        rol    ecx, 0c1h
        rol    ecx, 0c1h
        rol    ecx, 0c1h
        rol    ecx, 0c1h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    cl, ah
        db  0E1h, 0E1h
        db  0E1h, 0E1h
        db  0E1h, 0E1h
        db  0E1h, 0E1h
        db  0E1h, 0E1h
        db  0E1h, 0E1h
        db  0E1h, 0E1h
        db  0E1h, 0E1h
        db  0E1h, 0E1h
        db  0E1h, 0E1h
        db  0E1h, 0E1h
        db  0E1h, 0E1h
        db  0E1h, 0E1h
        db  0E1h, 0E1h
        db  0E1h, 0E1h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13C0:
        add    byte ptr [eax], al
loc_1_13C2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    ecx, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    cl, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    cl, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_1411
loc_1_1411:
        add    byte ptr [eax], al
        loopne loc_1_1415
loc_1_1415:
        add    byte ptr [eax], al
        loopne loc_1_1419
loc_1_1419:
        add    byte ptr [eax], al
        loopne loc_1_141D
loc_1_141D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1421:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_14B9:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1503:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1511:
        add    byte ptr [eax], al
loc_1_1513:
        add    byte ptr [eax], al
loc_1_1515:
        add    byte ptr [eax], al
        db  000h, 0CDh
        db  001h, 000h, 000h, 0E6h, 001h, 000h, 000h, 0FAh, 001h, 000h, 000h, 0ADh, 001h, 000h, 000h, 006h
        db  002h, 000h, 000h, 0EEh, 001h, 000h, 000h, 01Ch, 002h, 000h, 000h, 059h, 002h, 000h, 000h
loc_1_1538:
        add    eax, dword ptr [ebx + eax]
        add    eax, dword ptr [esp + eax]
        add    al, 0
_LTEXT ends
        end
