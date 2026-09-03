; vcache_lcode_01.asm -- VCACHE object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  060h, 08Bh, 0FEh, 0FCh, 033h, 0C0h, 0B9h, 000h, 004h, 000h, 000h, 0F3h, 0ABh, 08Bh, 0C6h, 08Dh
        db  058h, 040h, 03Bh, 0DFh, 077h, 027h, 089h, 000h, 089h, 040h, 004h, 089h, 040h, 038h, 089h, 040h
        db  03Ch, 0B9h, 070h, 00Ah, 000h, 000h, 08Bh, 051h, 004h, 089h, 002h, 089h, 008h, 089h, 050h, 004h
        db  089h, 041h, 004h, 0FFh, 005h, 0A8h, 009h, 000h, 000h, 08Bh, 0C3h, 0EBh, 0D2h, 08Bh, 00Dh, 0B8h
        db  009h, 000h, 000h, 08Bh, 03Dh, 0B0h, 009h, 000h, 000h, 033h, 0C0h, 0FCh, 0F2h, 0AFh, 089h, 077h
        db  0FCh, 0FFh, 005h, 0B4h, 009h, 000h, 000h, 061h, 0C3h
sub_1_0059:
        mov    ecx, dword ptr [ecx]
        push   ebx
        push   edx
        cmp    ecx, 0
        jge    SHORT loc_1_008F
loc_1_0062:
        db  08Bh, 035h, 070h, 00Ah, 000h, 000h
        mov    ebx, dword ptr [esi]
        mov    edx, dword ptr [esi + 4]
        mov    dword ptr [ebx + 4], edx
        mov    dword ptr [edx], ebx
        db  08Bh, 01Dh, 0A4h, 009h, 000h, 000h
        cmp    ebx, 0
        je     SHORT loc_1_00DA
        mov    edx, dword ptr [ebx + 100h]
        db  089h, 015h, 0A4h, 009h, 000h, 000h
        mov    dword ptr [esi + 10h], ebx
        pop    edx
        pop    ebx
        ret
loc_1_008F:
        db  083h, 03Dh, 0A8h, 009h, 000h, 000h, 000h
        jg     SHORT loc_1_00C3
        db  083h, 03Dh, 0ACh, 009h, 000h, 000h, 002h
        jb     SHORT loc_1_00DA
        db  08Bh, 035h, 0A4h, 009h, 000h, 000h
        cmp    esi, 0
        je     SHORT loc_1_00DA
        mov    ebx, dword ptr [esi + 100h]
        db  089h, 01Dh, 0A4h, 009h, 000h, 000h
        db  0FFh, 00Dh, 0ACh, 009h, 000h, 000h
        call   0
loc_1_00C3:
        db  083h, 03Dh, 0ACh, 009h, 000h, 000h, 000h
        je     SHORT loc_1_00DA
        db  0FFh, 00Dh, 0ACh, 009h, 000h, 000h
        db  0FFh, 00Dh, 0A8h, 009h, 000h, 000h
        jmp    SHORT loc_1_0062
loc_1_00DA:
        xor    esi, esi
        pop    edx
        pop    ebx
        ret
sub_1_00DF:
        push   ebx
        push   edx
        mov    ebx, dword ptr [esi + 10h]
        db  08Bh, 015h, 0A4h, 009h, 000h, 000h
        mov    dword ptr [ebx + 100h], edx
        db  089h, 01Dh, 0A4h, 009h, 000h, 000h
        mov    dword ptr [esi + 10h], 0
        db  0BBh, 070h, 00Ah, 000h, 000h
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [edx], esi
        mov    dword ptr [esi], ebx
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [ebx + 4], esi
        pop    edx
        pop    ebx
        ret
VCache_RelinquishPage:
        push   edx
        pushfd
        cli
        db  08Bh, 015h, 010h, 009h, 000h, 000h
        db  02Bh, 015h, 0B4h, 009h, 000h, 000h
        db  03Bh, 015h, 0F0h, 008h, 000h, 000h
        jbe    NEAR PTR loc_1_01FD
        db  083h, 03Dh, 0ACh, 009h, 000h, 000h, 000h
        je     SHORT loc_1_0187
        db  0A1h, 0A4h, 009h, 000h, 000h
        cmp    eax, 0
        je     NEAR PTR loc_1_01FD
        mov    edx, dword ptr [eax + 100h]
        db  089h, 015h, 0A4h, 009h, 000h, 000h
        db  0FFh, 00Dh, 0ACh, 009h, 000h, 000h
loc_1_0156:
        db  0FFh, 00Dh, 010h, 009h, 000h, 000h
        db  08Bh, 015h, 010h, 009h, 000h, 000h
        cmp    edx, 80h
        jae    SHORT loc_1_0179
        shr    edx, 3
        db  089h, 015h, 00Ch, 009h, 000h, 000h
        db  08Bh, 015h, 010h, 009h, 000h, 000h
loc_1_0179:
        lea    edx, [edx + edx*2]
        shr    edx, 1
        db  089h, 015h, 0F8h, 008h, 000h, 000h
        popfd
        pop    edx
        ret
loc_1_0187:
        push   esi
        db  0BEh, 030h, 00Ah, 000h, 000h
loc_1_018D:
        mov    esi, dword ptr [esi + 3ch]
        db  081h, 0FEh, 030h, 00Ah, 000h, 000h
        je     SHORT loc_1_01FC
        test   dword ptr [esi + 30h], 0ffffffh
        jne    SHORT loc_1_018D
        db  08Bh, 015h, 008h, 009h, 000h, 000h
        sub    edx, dword ptr [esi + 34h]
        db  03Bh, 015h, 00Ch, 009h, 000h, 000h
        jbe    SHORT loc_1_01FC
        movzx  edx, byte ptr [esi + 33h]
        sub    edx, 64h
        shl    edx, 4
        db  081h, 0C2h, 0C8h, 009h, 000h, 000h
        cmp    dword ptr [edx], 0
        jle    SHORT loc_1_018D
        dec    dword ptr [edx]
        call   sub_1_0354
        xor    ecx, ecx
        mov    byte ptr [esi + 33h], cl
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 0ch], ecx
        xchg   dword ptr [esi + 10h], ecx
        db  0B8h, 070h, 00Ah, 000h, 000h
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [edx], esi
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [eax + 4], esi
        db  0FFh, 005h, 0A8h, 009h, 000h, 000h
        mov    eax, ecx
        pop    esi
        jmp    NEAR PTR loc_1_0156
loc_1_01FC:
        pop    esi
loc_1_01FD:
        popfd
        pop    edx
        sub    eax, eax
        ret
VCache_UseThisPage:
        push   edx
        pushfd
        cli
        db  08Bh, 015h, 0A4h, 009h, 000h, 000h
        mov    dword ptr [eax + 100h], edx
        db  0A3h, 0A4h, 009h, 000h, 000h
        db  0FFh, 005h, 0ACh, 009h, 000h, 000h
        db  0FFh, 005h, 010h, 009h, 000h, 000h
        db  08Bh, 015h, 010h, 009h, 000h, 000h
        cmp    edx, 80h
        ja     SHORT loc_1_023F
        shr    edx, 3
        db  089h, 015h, 00Ch, 009h, 000h, 000h
        db  08Bh, 015h, 010h, 009h, 000h, 000h
loc_1_023F:
        lea    edx, [edx + edx*2]
        shr    edx, 1
        db  089h, 015h, 0F8h, 008h, 000h, 000h
        popfd
        pop    edx
        ret
VCache_SwapBuffers:
        mov    al, byte ptr [esi + 33h]
        cmp    byte ptr [edi + 33h], al
        stc
        jne    SHORT loc_1_026D
        cmp    word ptr [esi + 30h], 1
        jb     SHORT loc_1_026D
        cmp    word ptr [edi + 30h], 1
        jb     SHORT loc_1_026D
        mov    eax, dword ptr [esi + 10h]
        xchg   dword ptr [edi + 10h], eax
        mov    dword ptr [esi + 10h], eax
loc_1_026D:
        ret
VCACHE_Control:
        cmp    eax, 0
        db  00Fh, 084h, 089h, 0FDh, 0FEh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 0ADh, 0FDh, 0FEh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 0EAh, 0FFh, 0FEh, 0FFh
        clc
        ret
VCache_Get_Version:
        mov    eax, 100h
        xor    ebx, ebx
        clc
        ret
VCache_GetSize:
        sub    ah, 64h
        jb     SHORT loc_1_02BA
        db  038h, 025h, 0A0h, 009h, 000h, 000h
        jbe    SHORT loc_1_02BA
        movzx  edx, ah
        shl    edx, 4
        db  0A1h, 010h, 009h, 000h, 000h
        db  02Bh, 005h, 024h, 009h, 000h, 000h
        db  003h, 082h, 0CCh, 009h, 000h, 000h
        jmp    SHORT loc_1_02BF
loc_1_02BA:
        db  0A1h, 010h, 009h, 000h, 000h
loc_1_02BF:
        db  08Bh, 015h, 0F4h, 008h, 000h, 000h
        db  08Bh, 00Dh, 0F0h, 008h, 000h, 000h
        ret
VCache_CheckAvail:
        mov    dl, ah
        sub    ah, 64h
        jb     SHORT loc_1_0350
        db  038h, 025h, 0A0h, 009h, 000h, 000h
        jbe    SHORT loc_1_0350
        movzx  eax, ah
        shl    eax, 4
        db  005h, 0C8h, 009h, 000h, 000h
        push   edi
        db  08Bh, 03Dh, 010h, 009h, 000h, 000h
        db  02Bh, 03Dh, 024h, 009h, 000h, 000h
        sub    edi, dword ptr [eax]
        push   ecx
        db  0BBh, 030h, 00Ah, 000h, 000h
        db  02Bh, 00Dh, 0ACh, 009h, 000h, 000h
        jbe    SHORT loc_1_0346
        db  02Bh, 03Dh, 0B4h, 009h, 000h, 000h
        jae    SHORT loc_1_030D
        sub    edi, edi
loc_1_030D:
        mov    eax, eax
loc_1_0310:
        mov    ebx, dword ptr [ebx + 3ch]
        db  081h, 0FBh, 030h, 00Ah, 000h, 000h
        je     SHORT loc_1_034A
        test   dword ptr [ebx + 30h], 0ffffffh
        jne    SHORT loc_1_0310
        cmp    byte ptr [ebx + 33h], 0
        je     SHORT loc_1_033F
        db  0A1h, 008h, 009h, 000h, 000h
        sub    eax, dword ptr [ebx + 34h]
        db  03Bh, 005h, 0F8h, 008h, 000h, 000h
        jb     SHORT loc_1_034A
        cmp    byte ptr [ebx + 33h], dl
        je     SHORT loc_1_0344
loc_1_033F:
        or     edi, edi
        je     SHORT loc_1_0310
        dec    edi
loc_1_0344:
        loop   loc_1_0310
loc_1_0346:
        pop    eax
        pop    edi
        clc
        ret
loc_1_034A:
        pop    eax
        sub    eax, ecx
        pop    edi
        stc
        ret
loc_1_0350:
        xor    eax, eax
        stc
        ret
sub_1_0354:
        mov    ecx, dword ptr [esi]
        mov    edx, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [esi + 38h]
        mov    edx, dword ptr [esi + 3ch]
        mov    dword ptr [ecx + 3ch], edx
        mov    dword ptr [edx + 38h], ecx
        mov    ecx, dword ptr [esi + 8]
        or     ecx, dword ptr [esi + 0ch]
        je     SHORT loc_1_03B2
        movzx  ecx, byte ptr [esi + 33h]
        mov    dword ptr [esi + 38h], esi
        mov    dword ptr [esi + 3ch], esi
        sti
        pushad
        db  0FFh, 005h, 030h, 009h, 000h, 000h
        mov    eax, dword ptr [esi + 8]
        db  08Bh, 015h, 034h, 009h, 000h, 000h
        xor    eax, dword ptr [esi + 0ch]
        db  089h, 004h, 095h, 038h, 009h, 000h, 000h
        inc    edx
        cmp    edx, 1ah
        jb     SHORT loc_1_039F
        xor    edx, edx
loc_1_039F:
        db  089h, 015h, 034h, 009h, 000h, 000h
        sub    ecx, 64h
        shl    ecx, 4
        db  0FFh, 091h, 0D0h, 009h, 000h, 000h
        popad
loc_1_03B2:
        ret
        db  090h
VCache_FindBlock:
        push   ecx
        and    al, 0dfh
        push   eax
        mov    ecx, eax
        mov    eax, ebx
        or     eax, edi
        je     NEAR PTR loc_1_0456
        xor    eax, eax
        shld   eax, ebx, 10h
        xor    eax, ebx
        xor    eax, edi
        db  023h, 005h, 014h, 009h, 000h, 000h
        db  003h, 005h, 018h, 009h, 000h, 000h
        mov    esi, eax
loc_1_03DC:
        mov    esi, dword ptr [esi]
        cmp    esi, eax
        je     SHORT loc_1_0433
        cmp    dword ptr [esi + 8], ebx
        jne    SHORT loc_1_03DC
        cmp    dword ptr [esi + 0ch], edi
        jne    SHORT loc_1_03DC
        cmp    byte ptr [esi + 33h], ch
        jne    SHORT loc_1_03DC
        test   cl, 2
        je     SHORT loc_1_03FC
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
loc_1_03FC:
        db  0B8h, 030h, 00Ah, 000h, 000h
        mov    ecx, 1ah
        mov    eax, eax
loc_1_0408:
        mov    eax, dword ptr [eax + 3ch]
        db  03Dh, 030h, 00Ah, 000h, 000h
        je     SHORT loc_1_041E
        cmp    eax, esi
        loopne loc_1_0408
        jne    SHORT loc_1_041E
        db  0FFh, 005h, 02Ch, 009h, 000h, 000h
loc_1_041E:
        pop    eax
        pop    ecx
        test   al, 4
        jne    SHORT loc_1_0428
        mov    eax, dword ptr [esi + 10h]
        ret
loc_1_0428:
        db  0CDh, 020h, 006h, 000h, 08Bh, 004h
        clc
        mov    eax, dword ptr [esi + 10h]
        ret
loc_1_0433:
        push   eax
        push   ecx
        push   edi
        mov    eax, ebx
        mov    ecx, 1ah
        xor    eax, edi
        db  0BFh, 038h, 009h, 000h, 000h
        repne scasd
        jne    SHORT loc_1_044E
        db  0FFh, 005h, 028h, 009h, 000h, 000h
loc_1_044E:
        pop    edi
        pop    ecx
        pop    eax
        test   cl, 1
        jne    SHORT loc_1_0460
loc_1_0456:
        pop    eax
        pop    ecx
        xor    esi, esi
        stc
        ret
loc_1_045C:
        pop    ebp
        pop    eax
        jmp    SHORT loc_1_0456
loc_1_0460:
        sub    edx, edx
        mov    dl, ch
        sub    dl, 64h
        jb     SHORT loc_1_0456
        db  03Ah, 015h, 0A0h, 009h, 000h, 000h
        jae    SHORT loc_1_0456
        shl    edx, 4
        db  081h, 0C2h, 0C8h, 009h, 000h, 000h
        push   edx
        push   ebp
        push   ecx
        cli
        mov    ecx, edx
        call   sub_1_0059
        test   esi, esi
        je     SHORT loc_1_0495
        mov    ecx, dword ptr [esp + 8]
        inc    dword ptr [ecx]
        pop    ecx
        jmp    NEAR PTR loc_1_05B8
loc_1_0495:
        sti
        pop    ecx
        bt     ecx, 4
        cmc
        sbb    ebp, ebp
        db  023h, 02Dh, 0F8h, 008h, 000h, 000h
        db  0BEh, 030h, 00Ah, 000h, 000h
        mov    eax, eax
loc_1_04AC:
        mov    esi, dword ptr [esi + 3ch]
        db  081h, 0FEh, 030h, 00Ah, 000h, 000h
        je     SHORT loc_1_045C
        test   dword ptr [esi + 30h], 0ffffffh
        jne    SHORT loc_1_04AC
        db  068h, 0ACh, 004h, 000h, 000h
        cmp    byte ptr [esi + 33h], 0
        je     NEAR PTR sub_1_0578
        pop    ecx
        db  08Bh, 00Dh, 008h, 009h, 000h, 000h
        sub    ecx, dword ptr [esi + 34h]
        db  068h, 0ACh, 004h, 000h, 000h
        cmp    ebp, ecx
        jbe    NEAR PTR sub_1_0578
        pop    ecx
        db  08Bh, 00Dh, 004h, 009h, 000h, 000h
loc_1_04ED:
        db  08Bh, 015h, 0B0h, 009h, 000h, 000h
        lea    esi, [edx + ecx*4]
        ror    ecx, 10h
        mov    edx, dword ptr [esi]
        cmp    edx, 0
        je     SHORT loc_1_0556
        db  08Bh, 035h, 000h, 009h, 000h, 000h
        and    esi, 0fffh
        add    esi, edx
        add    edx, 0fc0h
        mov    cx, 40h
loc_1_0518:
        add    esi, 40h
        cmp    esi, edx
        jbe    SHORT loc_1_0527
        mov    esi, edx
        sub    esi, 0fc0h
loc_1_0527:
        cmp    dword ptr [esi + 10h], 0
        je     SHORT loc_1_054E
        test   dword ptr [esi + 30h], 0ffffffh
        jne    SHORT loc_1_054E
        push   edx
        db  08Bh, 015h, 008h, 009h, 000h, 000h
        sub    edx, dword ptr [esi + 34h]
        db  03Bh, 015h, 00Ch, 009h, 000h, 000h
        pop    edx
        jbe    SHORT loc_1_054E
        call   sub_1_0578
loc_1_054E:
        dec    cx
        cmp    cx, 0
        jne    SHORT loc_1_0518
loc_1_0556:
        ror    ecx, 10h
        inc    ecx
        db  03Bh, 00Dh, 0B4h, 009h, 000h, 000h
        jb     SHORT loc_1_0564
        sub    ecx, ecx
loc_1_0564:
        db  03Bh, 00Dh, 004h, 009h, 000h, 000h
        jne    SHORT loc_1_04ED
        jmp    NEAR PTR loc_1_045C
loc_1_0571:
        sti
        pop    edx
        pop    ecx
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_0578:
        cli
        push   ecx
        push   edx
        mov    ecx, dword ptr [esp + 10h]
        movzx  edx, byte ptr [esp + 15h]
        cmp    dword ptr [ecx + 8], -1
        je     SHORT loc_1_0571
        cmp    byte ptr [esi + 33h], dl
        je     SHORT loc_1_05AA
        movzx  edx, byte ptr [esi + 33h]
        sub    edx, 64h
        jb     SHORT loc_1_05A8
        shl    edx, 4
        db  081h, 0C2h, 0C8h, 009h, 000h, 000h
        cmp    dword ptr [edx], 0
        jle    SHORT loc_1_0571
        dec    dword ptr [edx]
loc_1_05A8:
        inc    dword ptr [ecx]
loc_1_05AA:
        pop    ecx
        pop    ecx
        pop    ecx
        cmp    byte ptr [esi + 33h], 0
        je     SHORT loc_1_05B8
        call   sub_1_0354
loc_1_05B8:
        xor    ecx, ecx
        mov    dword ptr [esi + 14h], ecx
        mov    dword ptr [esi + 18h], ecx
        mov    dword ptr [esi + 1ch], ecx
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], ecx
        mov    dword ptr [esi + 28h], ecx
        mov    dword ptr [esi + 2ch], ecx
        cli
        mov    dword ptr [esi + 8], ebx
        mov    dword ptr [esi + 0ch], edi
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi], ecx
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [eax], esi
        db  0BAh, 030h, 00Ah, 000h, 000h
        mov    ecx, dword ptr [edx + 38h]
        mov    dword ptr [esi + 38h], ecx
        mov    dword ptr [esi + 3ch], edx
        mov    dword ptr [ecx + 3ch], esi
        mov    dword ptr [edx + 38h], esi
        pop    ebp
        pop    eax
        pop    eax
        mov    byte ptr [esi + 33h], ah
        db  089h, 035h, 000h, 009h, 000h, 000h
        db  08Bh, 00Dh, 004h, 009h, 000h, 000h
        inc    ecx
        db  03Bh, 00Dh, 0B4h, 009h, 000h, 000h
        jb     SHORT loc_1_0613
        xor    ecx, ecx
loc_1_0613:
        db  089h, 00Dh, 004h, 009h, 000h, 000h
        sti
        db  08Bh, 00Dh, 008h, 009h, 000h, 000h
        mov    dword ptr [esi + 34h], ecx
        inc    ecx
        db  089h, 00Dh, 008h, 009h, 000h, 000h
        test   al, 2
        je     SHORT loc_1_0634
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
loc_1_0634:
        mov    eax, dword ptr [esi + 10h]
        pop    ecx
        stc
        ret
        db  08Bh, 0C0h
VCache_FreeBlock:
        pushfd
        cli
        cmp    dword ptr [esi + 38h], esi
        je     SHORT loc_1_06A5
        mov    ecx, dword ptr [esi]
        mov    edx, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        mov    dword ptr [esi], esi
        mov    dword ptr [esi + 4], esi
        mov    ecx, dword ptr [esi + 38h]
        mov    edx, dword ptr [esi + 3ch]
        mov    dword ptr [ecx + 3ch], edx
        mov    dword ptr [edx + 38h], ecx
        mov    dword ptr [esi + 38h], esi
        mov    dword ptr [esi + 3ch], esi
        xor    ecx, ecx
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 0ch], ecx
        mov    word ptr [esi + 30h], cx
        mov    cl, byte ptr [esi + 33h]
        sub    ecx, 64h
        shl    ecx, 4
        mov    dword ptr [esi + 30h], 0
        push   ecx
        call   sub_1_00DF
        pop    ecx
        db  08Bh, 091h, 0C8h, 009h, 000h, 000h
        cmp    edx, 0
        jle    SHORT loc_1_069E
        db  0FFh, 005h, 0ACh, 009h, 000h, 000h
        db  0FFh, 005h, 0A8h, 009h, 000h, 000h
loc_1_069E:
        dec    edx
        db  089h, 091h, 0C8h, 009h, 000h, 000h
loc_1_06A5:
        popfd
        ret
        db  090h
VCache_MakeMRU:
        db  0B8h, 030h, 00Ah, 000h, 000h
        cmp    dword ptr [eax + 38h], esi
        je     SHORT loc_1_06CD
        mov    edx, dword ptr [esi + 38h]
        mov    ecx, dword ptr [esi + 3ch]
        mov    dword ptr [edx + 3ch], ecx
        mov    dword ptr [ecx + 38h], edx
        mov    ecx, dword ptr [eax + 38h]
        mov    dword ptr [esi + 38h], ecx
        mov    dword ptr [esi + 3ch], eax
        mov    dword ptr [ecx + 3ch], esi
        mov    dword ptr [eax + 38h], esi
loc_1_06CD:
        db  08Bh, 00Dh, 008h, 009h, 000h, 000h
        mov    dword ptr [esi + 34h], ecx
        inc    ecx
        db  089h, 00Dh, 008h, 009h, 000h, 000h
        ret
        db  08Bh, 0C0h
VCache_Hold:
        inc    word ptr [esi + 30h]
        ret
        db  02Eh, 08Bh, 0C0h
VCache_Unhold:
        dec    word ptr [esi + 30h]
        ret
        db  02Eh, 08Bh, 0C0h
VCache_TestHold:
        movzx  eax, word ptr [esi + 30h]
        or     eax, eax
        ret
VCache_Enum:
        cmp    ah, 64h
        jb     SHORT loc_1_074C
        xor    edi, edi
        db  08Bh, 035h, 0B0h, 009h, 000h, 000h
loc_1_0704:
        db  03Bh, 03Dh, 0B8h, 009h, 000h, 000h
        jae    SHORT loc_1_074C
        push   esi
        push   edi
        mov    esi, dword ptr [esi]
        cmp    esi, 0
        je     SHORT loc_1_0727
        lea    edi, [esi + 0fc0h]
loc_1_071B:
        cmp    byte ptr [esi + 33h], ah
        je     SHORT loc_1_072F
loc_1_0720:
        add    esi, 40h
        cmp    esi, edi
        jbe    SHORT loc_1_071B
loc_1_0727:
        pop    edi
        pop    esi
        inc    edi
        add    esi, 4
        jmp    SHORT loc_1_0704
loc_1_072F:
        cmp    dword ptr [esi + 8], 0
        jne    SHORT loc_1_073B
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_1_0720
loc_1_073B:
        push   eax
        push   esi
        push   edx
        push   edi
        cmp    word ptr [esi + 30h], 0
        call   edx
        pop    edi
        pop    edx
        pop    esi
        pop    eax
        jmp    SHORT loc_1_0720
loc_1_074C:
        ret
VCACHE_PM_API:
        stc
        ret
        db  090h
VCache_TestHandle:
        pushfd
        push   eax
        push   ecx
        push   edx
        db  08Bh, 00Dh, 0B8h, 009h, 000h, 000h
        db  08Bh, 015h, 0B0h, 009h, 000h, 000h
        mov    eax, esi
        sub    eax, 1000h
        jb     SHORT loc_1_0793
loc_1_0769:
        cmp    dword ptr [edx], eax
        ja     SHORT loc_1_0774
loc_1_076D:
        add    edx, 4
        loop   loc_1_0769
        jmp    SHORT loc_1_0793
loc_1_0774:
        cmp    dword ptr [edx], esi
        ja     SHORT loc_1_076D
        mov    eax, esi
        sub    eax, dword ptr [edx]
        xor    edx, edx
        mov    ecx, 40h
        div    ecx
        test   edx, edx
        jne    SHORT loc_1_0793
        pop    edx
        pop    ecx
        pop    eax
        cmp    byte ptr [esi + 33h], ah
        jne    SHORT loc_1_0798
loc_1_0791:
        popfd
        ret
loc_1_0793:
        pop    edx
        pop    ecx
        pop    eax
        jmp    SHORT loc_1_0791
loc_1_0798:
        jmp    SHORT loc_1_0791
VCache_RecalcSums:
        ret
        db  090h
VCache_VerifySums:
        xor    ecx, ecx
        ret
        db  090h
VCache_GetStats:
        xor    ebx, ebx
        xor    ecx, ecx
        xor    edi, edi
        db  087h, 01Dh, 028h, 009h, 000h, 000h
        db  087h, 00Dh, 02Ch, 009h, 000h, 000h
        db  087h, 03Dh, 030h, 009h, 000h, 000h
        db  08Bh, 015h, 0FCh, 008h, 000h, 000h
        ret
loc_1_07BF:
        push   0
        push   0
        push   0
        db  068h, 090h, 012h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 10h
        ret
sub_1_07D4:
        db  0CDh, 020h, 00Dh, 080h, 040h, 000h
sub_1_07DA:
        db  0CDh, 020h, 00Eh, 080h, 040h, 000h
sub_1_07E0:
        db  0CDh, 020h, 011h, 080h, 040h, 000h
        add    byte ptr [eax], al
sub_1_07E8:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_07F0:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_07F8:
        db  0CDh, 020h, 04Ah, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0800:
        db  0CDh, 020h, 04Bh, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0808:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0810:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0818:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  000h, 000h, 000h
sub_1_0834:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        add    byte ptr [eax], al
        db  08Bh, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 094h, 002h, 000h, 000h, 0CCh, 002h, 000h, 000h
        db  0B4h, 003h, 000h, 000h, 03Ch, 006h, 000h, 000h, 0A8h, 006h, 000h, 000h, 0E0h, 006h, 000h, 000h
        db  0E8h, 006h, 000h, 000h, 0F7h, 006h, 000h, 000h, 050h, 007h, 000h, 000h, 09Ch, 007h, 000h, 000h
        db  09Ah, 007h, 000h, 000h, 0F0h, 006h, 000h, 000h, 0A0h, 007h, 000h, 000h, 0E3h, 000h, 000h, 000h
        db  027h, 001h, 000h, 000h, 04Dh, 002h, 000h, 000h, 012h, 001h, 000h, 000h, 002h, 002h, 000h, 000h
        db  0F0h, 012h, 000h, 000h, 0B0h, 013h, 000h, 000h, 0F0h, 013h, 000h, 000h, 010h, 014h, 000h, 000h
        db  0A0h, 014h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 08Bh, 004h, 003h, 001h, 000h, 000h, 056h, 043h, 041h, 043h
        db  048h, 045h, 020h, 020h, 000h, 001h, 000h, 000h, 06Eh, 002h, 000h, 000h, 04Dh, 007h, 000h, 000h
        db  04Dh, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  03Ch, 008h, 000h, 000h, 019h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  011h, 0F1h, 0FFh, 0FFh
loc_1_08F4:
        adc    ecx, esi
        db  0FFh, 0FFh
loc_1_08F8:
        adc    ecx, esi
        db  0FFh, 0FFh
loc_1_08FC:
        adc    ecx, esi
        db  0FFh, 0FFh
loc_1_0900:
        adc    ecx, esi
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h
loc_1_0908:
        adc    ecx, esi
        db  0FFh, 0FFh
loc_1_090C:
        adc    ecx, esi
        db  0FFh, 0FFh
loc_1_0910:
        adc    ecx, esi
        db  0FFh, 0FFh
loc_1_0914:
        adc    ecx, esi
        db  0FFh, 0FFh
loc_1_0918:
        adc    ecx, esi
        db  0FFh, 0FFh, 011h, 0F1h, 0FFh, 0FFh
loc_1_0920:
        adc    ecx, esi
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_09D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A30:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A70:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0A80:
        push   esi
        sub    eax, eax
        cmp    ecx, eax
        je     SHORT loc_1_0A90
loc_1_0A87:
        movzx  esi, byte ptr [edx]
        add    eax, esi
        inc    edx
        dec    ecx
        jne    SHORT loc_1_0A87
loc_1_0A90:
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0AA0:
        db  083h, 03Dh, 0E8h, 015h, 000h, 000h, 000h
        jne    SHORT loc_1_0ABF
        push   0
        push   493e0h
        db  068h, 0BFh, 007h, 000h, 000h
        call   sub_1_0818
        db  0A3h, 0E8h, 015h, 000h, 000h
loc_1_0ABF:
        ret
sub_1_0AC0:
        push   esi
        push   edi
        mov    edi, dword ptr [ecx + 8]
        mov    esi, dword ptr [edi]
        cmp    ecx, esi
        je     SHORT loc_1_0AEB
        mov    edx, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ecx], esi
        mov    dword ptr [ecx + 4], edi
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [edi], ecx
        or     dword ptr [edi + 1ch], 20000h
        call   sub_1_0AA0
loc_1_0AEB:
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_0AF0:
        push   ebx
        push   esi
        db  0A1h, 0E0h, 015h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0B30
loc_1_0AFB:
        mov    edx, ecx
        mov    esi, dword ptr [eax + 0ch]
loc_1_0B00:
        mov    bl, byte ptr [edx]
        cmp    bl, byte ptr [esi]
        jne    SHORT loc_1_0B20
        or     bl, bl
        je     SHORT loc_1_0B1C
        mov    bl, byte ptr [edx + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    SHORT loc_1_0B20
        add    edx, 2
        add    esi, 2
        or     bl, bl
        jne    SHORT loc_1_0B00
loc_1_0B1C:
        sub    edx, edx
        jmp    SHORT loc_1_0B25
loc_1_0B20:
        sbb    edx, edx
        sbb    edx, -1
loc_1_0B25:
        or     edx, edx
        je     SHORT loc_1_0B32
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        jne    SHORT loc_1_0AFB
loc_1_0B30:
        sub    eax, eax
loc_1_0B32:
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0B40:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, ecx
        mov    dword ptr [esp + 1ch], 4
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esi + 1ch]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   0
        db  068h, 0F0h, 015h, 000h, 000h
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_0808
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_1_0DA8
        mov    dword ptr [esp + 1ch], 4
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esi + 14h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   0
        db  068h, 0F8h, 015h, 000h, 000h
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_0808
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_1_0DA8
        mov    dword ptr [esp + 1ch], 4
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esi + 18h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   0
        db  068h, 004h, 016h, 000h, 000h
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_0808
        add    esp, 18h
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_0BE4
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
loc_1_0BE4:
        sub    edi, edi
        cmp    ebp, edi
        jne    NEAR PTR loc_1_0DA6
        mov    edi, edi
loc_1_0BF0:
        cmp    dword ptr [esi + 14h], edi
        jbe    NEAR PTR loc_1_0DA6
        push   edi
        db  068h, 010h, 016h, 000h, 000h
        lea    eax, [esp + 28h]
        db  068h, 014h, 016h, 000h, 000h
        push   eax
        call   sub_1_0834
        add    esp, 10h
        push   edi
        db  068h, 01Ch, 016h, 000h, 000h
        lea    eax, [esp + 30h]
        db  068h, 024h, 016h, 000h, 000h
        push   eax
        call   sub_1_0834
        mov    dword ptr [esp + 20h], 0
        lea    eax, [esp + 20h]
        add    esp, 10h
        push   eax
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        lea    eax, [esp + 2ch]
        push   0
        push   eax
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_0808
        add    esp, 18h
        mov    ebp, eax
        cmp    ebp, 0eah
        je     SHORT loc_1_0C62
        or     ebp, ebp
        jne    NEAR PTR loc_1_0D79
loc_1_0C62:
        mov    dword ptr [esp + 14h], 0
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        lea    eax, [esp + 34h]
        push   0
        push   eax
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_0808
        add    esp, 18h
        mov    ebp, eax
        cmp    ebp, 0eah
        je     SHORT loc_1_0C9B
        or     ebp, ebp
        jne    NEAR PTR loc_1_0D79
loc_1_0C9B:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        lea    eax, [eax + ecx + 28h]
        push   eax
        call   sub_1_07D4
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_0D70
        mov    dword ptr [ebx + 8], esi
        lea    ecx, [ebx + 28h]
        mov    dword ptr [ebx + 20h], ecx
        mov    eax, dword ptr [esp + 10h]
        add    eax, ecx
        mov    dword ptr [ebx + 24h], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ebx + 14h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 10h], eax
        lea    eax, [esp + 10h]
        mov    dword ptr [ebx + 18h], edi
        mov    dword ptr [ebx + 1ch], 0
        push   eax
        lea    eax, [esp + 1ch]
        push   ecx
        push   eax
        lea    eax, [esp + 2ch]
        push   0
        push   eax
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_0808
        add    esp, 18h
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_0D60
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [esp + 14h]
        push   eax
        mov    eax, dword ptr [ebx + 24h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 34h]
        push   0
        push   eax
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_0808
        add    esp, 18h
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_0D55
        mov    edx, dword ptr [ebx + 20h]
        mov    ecx, dword ptr [esp + 10h]
        call   sub_1_0A80
        mov    dword ptr [ebx + 0ch], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 4], ebx
        mov    dword ptr [ebx], esi
        jmp    SHORT loc_1_0D75
loc_1_0D55:
        push   ebx
        call   sub_1_07DA
        add    esp, 4
        jmp    SHORT loc_1_0D75
loc_1_0D60:
        push   ebx
        call   sub_1_07DA
        add    esp, 4
        jmp    SHORT loc_1_0D75
        db  005h, 000h, 000h, 000h, 000h
loc_1_0D70:
        mov    ebp, 0eh
loc_1_0D75:
        or     ebp, ebp
        je     SHORT loc_1_0D9D
loc_1_0D79:
        cmp    dword ptr [esi + 4], esi
        je     SHORT loc_1_0D9D
        mov    edi, edi
loc_1_0D80:
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    ebx, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx], ecx
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [esi + 4], ebx
        call   sub_1_07DA
        add    esp, 4
        cmp    ebx, esi
        jne    SHORT loc_1_0D80
loc_1_0D9D:
        inc    edi
        or     ebp, ebp
        je     NEAR PTR loc_1_0BF0
loc_1_0DA6:
        mov    eax, ebp
loc_1_0DA8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
sub_1_0DB0:
        or     dword ptr [ecx + 1ch], 10000h
        jmp    NEAR PTR sub_1_0AA0
        db  08Dh, 064h, 024h, 000h
sub_1_0DC0:
        or     dword ptr [ecx + 1ch], 10000h
        mov    ecx, dword ptr [ecx + 8]
        jmp    NEAR PTR sub_1_0DB0
        db  090h
sub_1_0DD0:
        mov    eax, dword ptr [ecx + 8]
        or     dword ptr [ecx + 1ch], 20000h
        dec    dword ptr [eax + 14h]
        or     dword ptr [eax + 1ch], 20000h
        jmp    NEAR PTR sub_1_0AA0
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0DF0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, edx
        mov    esi, ecx
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ecx, 24h
        push   ecx
        call   sub_1_07D4
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_0E87
        lea    edx, [ebp + 24h]
        mov    dword ptr [ebp + 0ch], edx
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    dword ptr [ebp + 20h], 0
        mov    dword ptr [ebp + 1ch], 0
        mov    dword ptr [ebp + 10h], 0
        mov    dword ptr [ebp], ebp
        mov    ecx, ebp
        mov    dword ptr [ebp + 4], ebp
        mov    dword ptr [ebp + 14h], 0
        mov    dword ptr [ebp + 18h], ebx
        db  0A1h, 0E0h, 015h, 000h, 000h
        mov    dword ptr [ebp + 8], eax
        db  089h, 02Dh, 0E0h, 015h, 000h, 000h
        call   sub_1_0DB0
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax], ebp
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_1_0E87:
        mov    eax, 0eh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0EA0:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], edx
        mov    ebp, ecx
        lea    eax, [esp + 14h]
        push   eax
        push   ebp
        db  0A1h, 09Ch, 016h, 000h, 000h
        push   eax
        call   sub_1_07E8
        add    esp, 0ch
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_1_0F91
        call   sub_1_07E0
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ecx, 24h
        push   ecx
        call   sub_1_07D4
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_0F85
        lea    edx, [ebx + 24h]
        mov    dword ptr [ebx + 0ch], edx
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ebx + 20h], eax
        mov    dword ptr [ebx + 10h], 0
        mov    dword ptr [ebx], ebx
        mov    ecx, ebx
        mov    dword ptr [ebx + 4], ebx
        mov    dword ptr [ebx + 1ch], 0
        call   sub_1_0B40
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_0F70
        mov    eax, dword ptr [esp + 14h]
        push   eax
        call   sub_1_07F0
        add    esp, 4
        push   ebp
        db  0A1h, 09Ch, 016h, 000h, 000h
        push   eax
        call   sub_1_0800
        add    esp, 8
        push   ebx
        call   sub_1_07DA
        add    esp, 4
        call   sub_1_07E0
        jmp    SHORT loc_1_0F95
        db  08Dh, 049h, 000h
loc_1_0F70:
        db  0A1h, 0E0h, 015h, 000h, 000h
        mov    dword ptr [ebx + 8], eax
        db  089h, 01Dh, 0E0h, 015h, 000h, 000h
        call   sub_1_07E0
        jmp    SHORT loc_1_0F95
loc_1_0F85:
        mov    esi, 0eh
        call   sub_1_07E0
        jmp    SHORT loc_1_0F95
loc_1_0F91:
        mov    ebx, dword ptr [esp + 18h]
loc_1_0F95:
        or     esi, esi
        jne    SHORT loc_1_0FA6
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_0FA6
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], ebx
loc_1_0FA6:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_0FB0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], edx
        mov    ebx, ecx
        mov    edx, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 10h]
        call   sub_1_0A80
        mov    edx, eax
        mov    eax, ebx
        mov    edi, edi
loc_1_0FD0:
        mov    eax, dword ptr [eax]
        cmp    eax, ebx
        je     SHORT loc_1_100D
        cmp    dword ptr [eax + 0ch], edx
        jne    SHORT loc_1_0FD0
        test   byte ptr [eax + 1eh], 2
        jne    SHORT loc_1_0FD0
        mov    ebp, dword ptr [eax + 10h]
        cmp    dword ptr [esp + 10h], ebp
        jne    SHORT loc_1_0FD0
        mov    ecx, ebp
        shr    ecx, 2
        mov    esi, dword ptr [eax + 20h]
        mov    edi, dword ptr [esp + 18h]
        repe cmpsd
        jne    SHORT loc_1_0FD0
        mov    ecx, ebp
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_1_0FD0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
loc_1_100D:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1020:
        db  068h, 09Ch, 016h, 000h, 000h
        db  068h, 02Ch, 016h, 000h, 000h
        push   80000002h
        call   sub_1_07F8
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_1045
        db  0C7h, 005h, 0ECh, 015h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_1045:
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1050:
        sub    esp, 0ch
        push   esi
        push   edi
        mov    edi, ecx
        mov    esi, dword ptr [edi + 8]
        mov    eax, dword ptr [edi + 18h]
        push   eax
        db  068h, 060h, 016h, 000h, 000h
        lea    eax, [esp + 10h]
        db  068h, 064h, 016h, 000h, 000h
        push   eax
        call   sub_1_0834
        add    esp, 10h
        mov    eax, dword ptr [edi + 10h]
        push   eax
        mov    eax, dword ptr [edi + 20h]
        push   eax
        push   3
        lea    eax, [esp + 14h]
        push   0
        push   eax
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_0810
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_1_10CE
        mov    eax, dword ptr [edi + 18h]
        push   eax
        db  068h, 06Ch, 016h, 000h, 000h
        lea    eax, [esp + 10h]
        db  068h, 074h, 016h, 000h, 000h
        push   eax
        call   sub_1_0834
        add    esp, 10h
        mov    eax, dword ptr [edi + 14h]
        push   eax
        mov    eax, dword ptr [edi + 24h]
        push   eax
        push   3
        lea    eax, [esp + 14h]
        push   0
        push   eax
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_0810
        add    esp, 18h
loc_1_10CE:
        pop    edi
        pop    esi
        add    esp, 0ch
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_10E0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, ecx
        mov    eax, dword ptr [esi + 1ch]
        test   eax, 20000h
        je     NEAR PTR loc_1_11E0
        and    eax, 0fffdffffh
        mov    dword ptr [esi + 1ch], eax
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_07F0
        add    esp, 4
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        db  0A1h, 09Ch, 016h, 000h, 000h
        push   eax
        call   sub_1_0800
        add    esp, 8
        lea    eax, [esi + 20h]
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        db  0A1h, 09Ch, 016h, 000h, 000h
        push   eax
        call   sub_1_07F8
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_1175
        sub    edi, edi
        mov    ebx, dword ptr [esi]
        cmp    esi, ebx
        je     SHORT loc_1_1172
loc_1_1143:
        test   byte ptr [ebx + 1eh], 2
        je     SHORT loc_1_1161
        mov    ebp, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebp], eax
        mov    dword ptr [eax + 4], ebp
        push   ebx
        call   sub_1_07DA
        add    esp, 4
        mov    ebx, ebp
        jmp    SHORT loc_1_116C
loc_1_1161:
        mov    dword ptr [ebx + 18h], edi
        inc    edi
        mov    ecx, ebx
        call   sub_1_1050
loc_1_116C:
        mov    ebx, dword ptr [ebx]
        cmp    esi, ebx
        jne    SHORT loc_1_1143
loc_1_1172:
        mov    dword ptr [esi + 14h], edi
loc_1_1175:
        cmp    dword ptr [esp + 10h], 0
        jne    NEAR PTR loc_1_1214
        push   4
        lea    eax, [esi + 14h]
        push   eax
        push   3
        push   0
        db  068h, 07Ch, 016h, 000h, 000h
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_0810
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_1_1214
        push   4
        lea    eax, [esi + 18h]
        push   eax
        push   3
        push   0
        db  068h, 088h, 016h, 000h, 000h
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_0810
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_1_1214
        push   4
        lea    eax, [esi + 1ch]
        push   eax
        push   3
        push   0
        db  068h, 094h, 016h, 000h, 000h
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_0810
        add    esp, 18h
        jmp    SHORT loc_1_1214
        db  005h, 000h, 000h, 000h, 000h
loc_1_11E0:
        test   eax, 10000h
        je     SHORT loc_1_1214
        and    eax, 0fffeffffh
        mov    dword ptr [esi + 1ch], eax
        mov    edi, dword ptr [esi]
        cmp    esi, edi
        je     SHORT loc_1_1214
loc_1_11F5:
        mov    eax, dword ptr [edi + 1ch]
        test   eax, 10000h
        je     SHORT loc_1_120E
        and    eax, 0fffeffffh
        mov    dword ptr [edi + 1ch], eax
        mov    ecx, edi
        call   sub_1_1050
loc_1_120E:
        mov    edi, dword ptr [edi]
        cmp    esi, edi
        jne    SHORT loc_1_11F5
loc_1_1214:
        cmp    dword ptr [esi + 10h], 0
        jne    SHORT loc_1_127B
        mov    edi, dword ptr [esi]
        mov    ecx, edi
        cmp    esi, ecx
        je     SHORT loc_1_123B
loc_1_1222:
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        push   ecx
        call   sub_1_07DA
        add    esp, 4
        mov    ecx, edi
        cmp    esi, edi
        jne    SHORT loc_1_1222
loc_1_123B:
        db  08Bh, 00Dh, 0E0h, 015h, 000h, 000h
        cmp    esi, ecx
        jne    SHORT loc_1_1250
        mov    eax, dword ptr [esi + 8]
        db  0A3h, 0E0h, 015h, 000h, 000h
        jmp    SHORT loc_1_1266
        db  090h
loc_1_1250:
        cmp    dword ptr [ecx + 8], esi
        je     SHORT loc_1_125D
loc_1_1255:
        mov    ecx, dword ptr [ecx + 8]
        cmp    dword ptr [ecx + 8], esi
        jne    SHORT loc_1_1255
loc_1_125D:
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], eax
loc_1_1266:
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_07F0
        add    esp, 4
        push   esi
        call   sub_1_07DA
        add    esp, 4
loc_1_127B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1290:
        push   esi
        db  0C7h, 005h, 0ECh, 015h, 000h, 000h, 002h, 000h, 000h, 000h
        db  0C7h, 005h, 0E8h, 015h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 035h, 0E0h, 015h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_12BE
        nop
loc_1_12B0:
        mov    ecx, esi
        call   sub_1_10E0
        mov    esi, dword ptr [esi + 8]
        or     esi, esi
        jne    SHORT loc_1_12B0
loc_1_12BE:
        db  0C7h, 005h, 0ECh, 015h, 000h, 000h, 001h, 000h, 000h, 000h
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_12D0:
        db  0A1h, 0E0h, 015h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_12E4
loc_1_12D9:
        cmp    eax, ecx
        je     SHORT loc_1_12E4
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        jne    SHORT loc_1_12D9
loc_1_12E4:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_VCache_CreateLookupCache:
        sub    esp, 8
        push   esi
        push   edi
        db  083h, 03Dh, 0ECh, 015h, 000h, 000h, 001h
        je     SHORT loc_1_1309
        mov    eax, 1
        pop    edi
        pop    esi
        add    esp, 8
        ret
loc_1_1309:
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, esi
        call   sub_1_0AF0
        mov    dword ptr [esp + 8], eax
        or     eax, eax
        jne    SHORT loc_1_1380
        lea    edx, [esp + 8]
        mov    ecx, esi
        call   sub_1_0EA0
        mov    edi, eax
        cmp    edi, 0eh
        jne    SHORT loc_1_1361
        push   2000h
        call   sub_1_07D4
        mov    dword ptr [esp + 0ch], eax
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1361
        push   eax
        call   sub_1_07DA
        mov    dword ptr [esp + 0ch], 0
        lea    edx, [esp + 0ch]
        add    esp, 4
        mov    ecx, esi
        call   sub_1_0EA0
        mov    edi, eax
loc_1_1361:
        or     edi, edi
        je     SHORT loc_1_1388
        lea    eax, [esp + 8]
        push   eax
        mov    ecx, esi
        mov    eax, dword ptr [esp + 20h]
        mov    edx, dword ptr [esp + 1ch]
        push   eax
        call   sub_1_0DF0
        mov    edi, eax
        jmp    SHORT loc_1_1384
        db  08Bh, 0FFh
loc_1_1380:
        mov    edi, dword ptr [esp + 0ch]
loc_1_1384:
        or     edi, edi
        jne    SHORT loc_1_1399
loc_1_1388:
        mov    eax, dword ptr [esp + 8]
        inc    dword ptr [eax + 10h]
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax], ecx
loc_1_1399:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_VCache_CloseLookupCache:
        db  083h, 03Dh, 0ECh, 015h, 000h, 000h, 001h
        je     SHORT loc_1_13C0
        mov    eax, 1
        ret
        db  090h
loc_1_13C0:
        mov    ecx, dword ptr [esp + 4]
        call   sub_1_12D0
        or     eax, eax
        je     SHORT loc_1_13E0
        mov    ecx, dword ptr [eax + 10h]
        or     ecx, ecx
        je     SHORT loc_1_13E0
        dec    ecx
        mov    dword ptr [eax + 10h], ecx
        call   sub_1_0AA0
        sub    eax, eax
        ret
loc_1_13E0:
        mov    eax, 57h
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_VCache_DeleteLookupCache:
        db  083h, 03Dh, 0ECh, 015h, 000h, 000h, 001h
        je     SHORT loc_1_1400
        mov    eax, 1
        ret
        db  090h
loc_1_1400:
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_140A
        sub    eax, eax
        ret
loc_1_140A:
        sub    eax, eax
        ret
        db  08Dh, 049h, 000h
_VCache_Lookup:
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 0ECh, 015h, 000h, 000h, 001h
        je     SHORT loc_1_1425
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_1425:
        mov    ecx, dword ptr [esp + 10h]
        call   sub_1_12D0
        or     eax, eax
        jne    SHORT loc_1_1440
        mov    eax, 57h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_1440:
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 14h]
        push   ecx
        mov    ecx, eax
        call   sub_1_0FB0
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_1496
        mov    ecx, ebx
        call   sub_1_0AC0
        mov    edx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [ebx + 14h]
        cmp    dword ptr [edx], eax
        ja     SHORT loc_1_1488
        mov    edi, dword ptr [esp + 20h]
        mov    esi, dword ptr [ebx + 24h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebx + 14h]
        mov    dword ptr [edx], ecx
        pop    edi
        pop    esi
        sub    eax, eax
        pop    ebx
        ret
loc_1_1488:
        mov    ecx, dword ptr [ebx + 14h]
        mov    dword ptr [edx], ecx
        pop    edi
        pop    esi
        mov    eax, 0eah
        pop    ebx
        ret
loc_1_1496:
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
_VCache_UpdateLookup:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  083h, 03Dh, 0ECh, 015h, 000h, 000h, 001h
        je     SHORT loc_1_14B7
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_14B7:
        mov    ecx, dword ptr [esp + 14h]
        call   sub_1_12D0
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_14D0
        mov    eax, 57h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_14D0:
        mov    esi, dword ptr [esp + 1ch]
        mov    edx, dword ptr [esp + 18h]
        push   esi
        mov    ecx, edi
        call   sub_1_0FB0
        mov    ebp, eax
        or     ebp, ebp
        mov    ebx, dword ptr [esp + 20h]
        je     SHORT loc_1_14F8
        cmp    dword ptr [ebp + 14h], ebx
        je     SHORT loc_1_14F8
        mov    ecx, ebp
        call   sub_1_0DD0
        sub    ebp, ebp
loc_1_14F8:
        or     ebp, ebp
        jne    NEAR PTR loc_1_1590
        mov    eax, dword ptr [esp + 18h]
        lea    eax, [ebx + eax + 28h]
        push   eax
        call   sub_1_07D4
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_1_15B0
        mov    eax, dword ptr [edi + 14h]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_153E
        mov    ecx, dword ptr [edi + 4]
        test   byte ptr [ecx + 1eh], 2
        je     SHORT loc_1_1539
        mov    eax, 20000h
loc_1_1531:
        mov    ecx, dword ptr [ecx + 4]
        test   dword ptr [ecx + 1ch], eax
        jne    SHORT loc_1_1531
loc_1_1539:
        call   sub_1_0DD0
loc_1_153E:
        mov    eax, dword ptr [edi + 14h]
        inc    eax
        mov    dword ptr [edi + 14h], eax
        mov    dword ptr [ebp + 1ch], 0
        mov    dword ptr [ebp + 8], edi
        mov    dword ptr [ebp + 4], ebp
        lea    edi, [ebp + 28h]
        mov    dword ptr [ebp], ebp
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ebp + 10h], eax
        mov    dword ptr [ebp + 20h], edi
        add    eax, edi
        mov    dword ptr [ebp + 24h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [ebp + 20h]
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_1_0A80
        mov    dword ptr [ebp + 0ch], eax
        mov    ecx, ebp
        call   sub_1_0AC0
        jmp    SHORT loc_1_15B0
loc_1_1590:
        cmp    dword ptr [ebp + 14h], ebx
        jne    SHORT loc_1_15B0
        mov    ecx, ebx
        shr    ecx, 2
        mov    esi, dword ptr [ebp + 24h]
        mov    edi, dword ptr [esp + 24h]
        repe cmpsd
        jne    SHORT loc_1_15B0
        mov    ecx, ebx
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_1_15B0
        sub    ebp, ebp
loc_1_15B0:
        or     ebp, ebp
        je     SHORT loc_1_15D3
        mov    dword ptr [ebp + 14h], ebx
        mov    edi, dword ptr [ebp + 24h]
        mov    esi, dword ptr [esp + 24h]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    ecx, ebp
        call   sub_1_0DC0
loc_1_15D3:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_15F0:
        inc    esi
        insb
        popad
        jae    NEAR PTR loc_1_15F6
loc_1_15F6:
        add    byte ptr [eax], al
loc_1_15F8:
        dec    esi
        db  075h, 06Dh
        inc    ebp
        insb
        insd
        outsb
        db  074h, 073h
        add    byte ptr [ebp + 61h], cl
        db  078h, 045h
        insb
        insd
        outsb
        db  074h, 073h
        add    byte ptr [ebx + 65h], cl
        jns    SHORT loc_1_1614
loc_1_1614:
        and    eax, 34302573h
        jne    SHORT loc_1_161B
loc_1_161B:
        add    byte ptr [ecx + 74h], al
        popad
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1624:
        and    eax, 34302573h
        db  075h, 000h
        db  000h
loc_1_162C:
        push   ebx
        db  079h, 073h
        je     SHORT loc_1_1696
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        db  072h, 065h
        outsb
        db  074h, 043h
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        push   ebx
        db  065h, 072h, 076h
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
        pop    esp
        push   esi
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        pop    esp
        dec    esp
        outsd
        outsd
        imul   esi, dword ptr [ebp + 70h], 0
loc_1_1660:
        dec    ebx
        jns    NEAR PTR loc_1_1664
loc_1_1664:
        and    eax, 34302573h
        db  075h, 000h
        db  000h
loc_1_166C:
        inc    esp
        popad
        db  074h, 061h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1674:
        and    eax, 34302573h
        db  075h, 000h
        db  000h
loc_1_167C:
        dec    esi
        db  075h, 06Dh
        inc    ebp
        insb
        insd
        outsb
        db  074h, 073h
        add    byte ptr [ebp + 61h], cl
        db  078h, 045h
        insb
        insd
        outsb
        db  074h, 073h
        add    byte ptr [esi + 6ch], al
loc_1_1696:
        popad
        jae    NEAR PTR loc_1_169A
loc_1_169A:
        add    byte ptr [eax], al
loc_1_169C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
