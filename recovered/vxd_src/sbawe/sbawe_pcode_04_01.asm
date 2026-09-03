; sbawe_pcode_04_01.asm -- SBAWE object 4 segment _PTEXT  (part 1/2)
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0FFh, 074h, 024h, 004h, 0E8h, 000h, 000h, 000h, 000h, 083h, 0C4h, 004h, 085h, 0C0h, 075h, 004h
        db  033h, 0C0h, 0EBh, 006h, 08Bh, 080h, 0D1h, 016h, 000h, 000h, 0C3h
sub_4_001B:
        push   dword ptr [esp + 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0024:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_4_0035
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 16d1h], ecx
loc_4_0035:
        ret
sub_4_0036:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax], 1
        je     SHORT loc_4_0044
        xor    eax, eax
        jmp    SHORT loc_4_0072
loc_4_0044:
        cmp    word ptr [eax + 2], 1
        je     SHORT loc_4_004F
        xor    eax, eax
        jmp    SHORT loc_4_0072
loc_4_004F:
        cmp    word ptr [eax + 0eh], 10h
        je     SHORT loc_4_005A
        xor    eax, eax
        jmp    SHORT loc_4_0072
loc_4_005A:
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 0fa0h
        jae    SHORT loc_4_0068
        xor    eax, eax
        jmp    SHORT loc_4_0072
loc_4_0068:
        mov    ecx, 157c0h
        cmp    ecx, eax
        sbb    eax, eax
        inc    eax
loc_4_0072:
        ret
loc_4_0073:
        push   ebp
        mov    ecx, 10h
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 10h]
        db  0BFh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [edx], 0
        repe cmpsb
        mov    eax, dword ptr [ebp + 8]
        jne    SHORT loc_4_0097
        mov    dword ptr [edx], eax
loc_4_0097:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, dword ptr [ebp + 0ch]
        repe cmpsb
        jne    SHORT loc_4_00AD
        add    eax, 4
        mov    dword ptr [edx], eax
loc_4_00AD:
        mov    eax, dword ptr [edx]
        test   eax, eax
        je     SHORT loc_4_00BD
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
        jmp    SHORT loc_4_00C2
loc_4_00BD:
        mov    eax, 80004002h
loc_4_00C2:
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_4_00C8:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 10h]
        inc    eax
        mov    dword ptr [ecx + 10h], eax
        ret    4
loc_4_00D6:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    eax, dword ptr [edi + 10h]
        dec    eax
        mov    dword ptr [edi + 10h], eax
        jne    SHORT loc_4_0132
        mov    eax, dword ptr [edi + 14h]
        mov    ebx, dword ptr [edi + 18h]
        push   0ffffh
        mov    esi, dword ptr [eax + 1ch]
        mov    eax, ebx
        or     al, 60h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_00FF:
        push   0
        mov    eax, ebx
        or     al, 20h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_010C:
        push   0
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0115:
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_011C:
        add    esp, 8
        mov    eax, dword ptr [edi + 14h]
        push   0
        dec    dword ptr [eax + 20h]
        push   edi
        call   sub_4_4D5C
        add    esp, 8
        xor    eax, eax
loc_4_0132:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
loc_4_0138:
        push   dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0ch]
        mov    ecx, dword ptr [eax + 4]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax]
        ret    0ch
loc_4_014F:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        ret    4
loc_4_015F:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 8]
        ret    4
loc_4_016F:
        xor    eax, eax
        ret    20h
loc_4_0174:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [esi + 14h]
        test   edi, edi
        mov    ebx, dword ptr [ecx + 1ch]
        je     SHORT loc_4_01B3
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 30h]
        sub    eax, dword ptr [esi + 2ch]
        add    ecx, eax
        mov    dword ptr [ebp - 4], eax
        shr    ecx, 1
        mov    eax, edi
        shr    eax, 1
        push   eax
        push   dword ptr [ebp + 0ch]
        push   ecx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_01B0:
        add    esp, 10h
loc_4_01B3:
        mov    eax, dword ptr [ebp + 18h]
        test   eax, eax
        je     SHORT loc_4_01CF
        shr    eax, 1
        mov    ecx, dword ptr [esi + 30h]
        shr    ecx, 1
        push   eax
        push   dword ptr [ebp + 14h]
        push   ecx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_01CC:
        add    esp, 10h
loc_4_01CF:
        mov    eax, dword ptr [ebp - 4]
        add    eax, edi
        cmp    eax, dword ptr [esi + 28h]
        jne    SHORT loc_4_020E
        and    edi, 0fffffffeh
        mov    eax, dword ptr [ebp + 0ch]
        movsx  ecx, word ptr [edi + eax - 2]
        mov    eax, ecx
        lea    edi, [ebp - 24h]
        shl    eax, 10h
        mov    ax, cx
        mov    ecx, 8
        rep stosd
        push   10h
        lea    eax, [ebp - 24h]
        mov    ecx, dword ptr [esi + 30h]
        push   eax
        add    ecx, dword ptr [esi + 28h]
        shr    ecx, 1
        push   ecx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_020B:
        add    esp, 10h
loc_4_020E:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_4_0219:
        mov    eax, 88780032h
        ret    8
loc_4_0221:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [eax + 8]
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, dword ptr [eax + 14h]
        mov    esi, dword ptr [eax + 18h]
        mov    ebx, dword ptr [ecx + 1ch]
        or     esi, 20h
        mov    dword ptr [eax + 24h], edi
        push   esi
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0246:
        test   eax, 0ffff0000h
        mov    ecx, eax
        je     SHORT loc_4_0270
        shl    edi, 0eh
        mov    ebp, 0ac44h
        mov    eax, edi
        sub    edx, edx
        div    ebp
        shl    eax, 10h
        and    ecx, 0ffffh
        or     eax, ecx
        push   eax
        push   esi
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0270:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_0279:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 14h]
        mov    edx, dword ptr [esi + 18h]
        mov    eax, dword ptr [ecx + 1ch]
        mov    edi, dword ptr [ebx + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], edx
        cmp    dword ptr [esi + 34h], edi
        je     SHORT loc_4_02E1
        mov    eax, edx
        mov    dword ptr [esi + 34h], edi
        or     al, 60h
        push   eax
        mov    dword ptr [ebp - 0ch], eax
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_02B6:
        mov    ecx, edi
        and    eax, 0ffffh
        shl    ecx, 10h
        and    di, 0ff00h
        or     ecx, eax
        push   ecx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_02D3:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 4]
        shl    ecx, 5
        mov    word ptr [ecx + eax + 52h], di
loc_4_02E1:
        mov    eax, dword ptr [ebx + 14h]
        cmp    dword ptr [esi + 38h], eax
        jne    SHORT loc_4_02F1
        mov    eax, dword ptr [ebx + 18h]
        cmp    dword ptr [esi + 3ch], eax
        je     SHORT loc_4_035E
loc_4_02F1:
        mov    eax, dword ptr [ebp - 8]
        or     al, 20h
        push   eax
        mov    dword ptr [ebp - 0ch], eax
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0302:
        mov    edi, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        or     edi, 0c0h
        push   edi
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0317:
        mov    ecx, dword ptr [ebx + 14h]
        mov    edx, dword ptr [ebx + 18h]
        shr    edx, 8
        mov    dword ptr [ebp - 8], ecx
        shr    dword ptr [ebp - 8], 8
        mov    dword ptr [esi + 38h], ecx
        mov    ecx, dword ptr [ebx + 18h]
        mov    dword ptr [esi + 3ch], ecx
        mov    ecx, dword ptr [ebp - 10h]
        and    cl, 0
        mov    esi, eax
        or     ecx, edx
        and    esi, 0ffffffh
        mov    eax, dword ptr [ebp - 8]
        push   ecx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 4]
        shl    eax, 18h
        or     esi, eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0354:
        push   esi
        push   edi
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_035E:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0369:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 8]
        mov    ecx, dword ptr [edi + 14h]
        mov    esi, dword ptr [edi + 18h]
        or     esi, 200h
        mov    ebx, dword ptr [ecx + 1ch]
        push   esi
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0389:
        mov    ecx, dword ptr [edi + 30h]
        and    eax, 0ff000000h
        add    ecx, dword ptr [esp + 14h]
        shr    ecx, 1
        dec    ecx
        or     eax, ecx
        push   eax
        push   esi
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03A2:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_03AA:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 8]
        mov    edx, dword ptr [esi + 14h]
        mov    ecx, dword ptr [esi + 18h]
        or     ch, 2
        push   ecx
        push   dword ptr [edx + 1ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03C4:
        and    eax, 0ffffffh
        lea    ecx, [eax*2 + 2]
        mov    eax, dword ptr [esi + 30h]
        and    al, 0feh
        sub    ecx, eax
        mov    eax, dword ptr [esi + 28h]
        cmp    ecx, eax
        jb     SHORT loc_4_03E0
        mov    ecx, eax
loc_4_03E0:
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 10h]
        pop    esi
        mov    dword ptr [eax], ecx
        xor    eax, eax
        mov    dword ptr [edx], ecx
        ret    0ch
loc_4_03F2:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    ecx, dword ptr [esi + 14h]
        mov    edi, dword ptr [esi + 18h]
        mov    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [esi + 24h]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 14h], ecx
        mov    ecx, dword ptr [ebp + 14h]
        and    ecx, 1
        je     SHORT loc_4_0430
        mov    edx, dword ptr [esi + 30h]
        mov    ebx, dword ptr [esi + 28h]
        mov    eax, edx
        add    ebx, edx
        shr    eax, 1
        shr    ebx, 1
        mov    dword ptr [ebp - 10h], eax
        dec    ebx
        jmp    SHORT loc_4_043E
loc_4_0430:
        mov    eax, dword ptr [esi + 28h]
        add    eax, dword ptr [esi + 30h]
        shr    eax, 1
        mov    dword ptr [ebp - 10h], eax
        lea    ebx, [eax + 0fh]
loc_4_043E:
        mov    eax, edi
        test   ecx, ecx
        je     SHORT loc_4_047C
        or     al, 0c0h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 10h]
        dec    eax
        or     eax, 0c0000000h
        push   eax
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_045E:
        mov    eax, edi
        dec    ebx
        or     al, 0e0h
        push   eax
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_046F:
        and    eax, 0ff000000h
        or     ebx, eax
        push   ebx
        push   dword ptr [ebp - 8]
        jmp    SHORT loc_4_04B2
loc_4_047C:
        or     al, 0e0h
        dec    ebx
        push   eax
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_048B:
        and    eax, 0ff000000h
        or     ebx, eax
        push   ebx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_049E:
        mov    eax, edi
        or     al, 0c0h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 10h]
        dec    eax
        or     eax, 0c0000000h
        push   eax
        push   dword ptr [ebp - 0ch]
loc_4_04B2:
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_04BA:
        mov    ebx, edi
        push   -1
        or     bl, 60h
        or     edi, 20h
        push   ebx
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_04CD:
        push   edi
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_04D6:
        mov    ecx, 0ac44h
        sub    edx, edx
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 14h]
        shl    eax, 0eh
        div    ecx
        shl    eax, 10h
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        or     eax, ecx
        and    al, 80h
        or     al, 80h
        push   eax
        push   edi
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0504:
        mov    eax, dword ptr [esi + 34h]
        push   ebx
        push   dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0513:
        mov    ecx, dword ptr [ebp - 8]
        and    eax, 0ffffh
        shl    ecx, 10h
        or     ecx, eax
        push   ecx
        push   ebx
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_052A:
        push   edi
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0533:
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 4]
        mov    ebx, eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0540:
        mov    ecx, dword ptr [esi + 3ch]
        and    bl, 0
        shr    ecx, 8
        mov    edx, dword ptr [esi + 38h]
        shl    edx, 10h
        or     ecx, ebx
        mov    esi, eax
        push   ecx
        xor    esi, edx
        push   edi
        and    esi, 0ffffffh
        xor    esi, edx
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0567:
        push   esi
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0573:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_057E:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [eax + 14h]
        mov    esi, dword ptr [eax + 18h]
        mov    edi, esi
        mov    ebx, dword ptr [ecx + 1ch]
        or     edi, 20h
        push   edi
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_059D:
        and    eax, 0ffffh
        push   eax
        push   edi
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_05AA:
        push   0
        push   esi
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_05B3:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    4
loc_4_05BB:
        push   ebp
        mov    ecx, 10h
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 10h]
        db  0BFh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [edx], 0
        repe cmpsb
        mov    eax, dword ptr [ebp + 8]
        jne    SHORT loc_4_05DF
        mov    dword ptr [edx], eax
loc_4_05DF:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, dword ptr [ebp + 0ch]
        repe cmpsb
        jne    SHORT loc_4_05F5
        add    eax, 4
        mov    dword ptr [edx], eax
loc_4_05F5:
        mov    eax, dword ptr [edx]
        test   eax, eax
        je     SHORT loc_4_0605
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
        jmp    SHORT loc_4_060A
loc_4_0605:
        mov    eax, 80004002h
loc_4_060A:
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_4_0610:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 10h]
        inc    eax
        mov    dword ptr [ecx + 10h], eax
        ret    4
loc_4_061E:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 10h]
        dec    eax
        mov    dword ptr [ecx + 10h], eax
        ret    4
loc_4_062C:
        push   dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0ch]
        mov    ecx, dword ptr [eax + 4]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax]
        ret    0ch
loc_4_0643:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        ret    4
loc_4_0653:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 8]
        ret    4
loc_4_0663:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [eax + 8]
        mov    esi, dword ptr [esp + 18h]
        mov    ebp, 0ffh
        mov    dword ptr [esi], 4
        call   sub_4_0BD4
        push   ebp
        lea    ecx, [esi + 4]
        push   eax
        lea    edi, [esi + 104h]
        push   ecx
        call   sub_4_4D64
        add    esp, 0ch
        push   ebp
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_4_4D64
        add    esp, 0ch
        mov    eax, dword ptr [ebx + 14h]
        mov    dword ptr [esi + 210h], 1
        mov    dword ptr [esi + 218h], 400000h
        mov    word ptr [esi + 208h], 32a4h
        mov    dword ptr [esi + 204h], eax
        mov    ecx, dword ptr [ebx + 1ch]
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esi + 220h], 20h
        add    eax, eax
        add    eax, 3fffffh
        mov    dword ptr [esi + 21ch], eax
        mov    eax, 0b3h
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_06FD
loc_4_06F2:
        movsx  ecx, byte ptr [edi]
        add    eax, ecx
        inc    edi
        cmp    byte ptr [edi], 0
        jne    SHORT loc_4_06F2
loc_4_06FD:
        mov    ecx, 0f7h
        sub    edx, edx
        div    ecx
        mov    word ptr [esi + 20ah], dx
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_0716:
        xor    eax, eax
        ret    4
loc_4_071B:
        xor    eax, eax
        ret    4
loc_4_0720:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    eax, dword ptr [eax + 8]
        xor    edi, edi
        mov    ecx, 20h
        mov    dword ptr [esi], 910h
        mov    dword ptr [esi + 0ch], edi
        mov    dword ptr [esi + 10h], ecx
        mov    edx, ecx
        mov    dword ptr [esi + 14h], ecx
        mov    dword ptr [esi + 4], 64h
        mov    dword ptr [esi + 8], 186a0h
        mov    dword ptr [esi + 18h], edi
        sub    edx, dword ptr [eax + 20h]
        mov    dword ptr [esi + 1ch], edx
        sub    ecx, dword ptr [eax + 20h]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], edi
        mov    dword ptr [esi + 28h], edi
        mov    dword ptr [esi + 2ch], edi
        mov    dword ptr [esi + 30h], edi
        mov    dword ptr [esi + 34h], edi
        mov    dword ptr [esi + 38h], edi
        mov    dword ptr [esi + 3ch], edi
        mov    eax, dword ptr [eax + 1ch]
        mov    eax, dword ptr [eax + 8]
        add    eax, eax
        mov    dword ptr [esi + 44h], edi
        mov    dword ptr [esi + 40h], eax
        mov    dword ptr [esi + 48h], edi
        xor    eax, eax
        pop    edi
        pop    esi
        ret    8
loc_4_078F:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    ecx, dword ptr [ebp + 20h]
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 8]
        push   201h
        push   40h
        mov    dword ptr [ecx], 0
        call   sub_4_4D6C
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_4_07C7
        mov    eax, 8007000eh
        jmp    NEAR PTR loc_4_0908
loc_4_07C7:
        mov    eax, 0ffffh
        db  0C7h, 006h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 004h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 8], esi
        push   dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 0ch], esi
        mov    dword ptr [esi + 14h], edi
        mov    dword ptr [esi + 34h], eax
        mov    dword ptr [esi + 38h], eax
        mov    dword ptr [esi + 3ch], eax
        mov    ebx, dword ptr [edi + 1ch]
        call   sub_4_0036
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_4_0812
        push   0
        push   esi
        call   sub_4_4D5C
        add    esp, 8
        mov    eax, 88780064h
        jmp    NEAR PTR loc_4_0908
loc_4_0812:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp + 18h]
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 30h], ecx
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi + 28h], eax
        lea    eax, [ebp - 4]
        mov    edx, dword ptr [ecx]
        push   eax
        push   ebx
        mov    dword ptr [esi + 2ch], edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0838:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_0854
        push   0
        push   esi
        call   sub_4_4D5C
        add    esp, 8
        mov    eax, 8878000ah
        jmp    NEAR PTR loc_4_0908
loc_4_0854:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        push   80h
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 2a0h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_087A:
        push   0ffffh
        mov    eax, dword ptr [ebp - 4]
        or     al, 60h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_088B:
        push   0
        mov    eax, dword ptr [ebp - 4]
        or     al, 20h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0899:
        push   0
        push   dword ptr [ebp - 4]
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08A4:
        mov    eax, dword ptr [esi + 30h]
        shr    eax, 1
        dec    eax
        or     eax, 0c0000000h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     al, 0c0h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08BC:
        mov    eax, dword ptr [ebp - 4]
        or     al, 0e0h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08C8:
        mov    ecx, dword ptr [esi + 30h]
        and    eax, 0ff000000h
        shr    ecx, 1
        or     ecx, eax
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        or     al, 0e0h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08E1:
        mov    eax, dword ptr [esi + 30h]
        shr    eax, 1
        dec    eax
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     ah, 2
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08F5:
        lea    edx, [esi + 4]
        mov    ecx, dword ptr [ebp + 20h]
        push   esi
        inc    dword ptr [edi + 20h]
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [esi]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_4_0908:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_0911:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        mov    ecx, dword ptr [eax + 8]
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   edi
        mov    dword ptr [ebp - 8], ecx
        mov    edi, dword ptr [edx + 8]
        mov    ecx, dword ptr [ebp + 10h]
        push   201h
        push   40h
        mov    dword ptr [ecx], 0
        call   sub_4_4D6C
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_4_0952
        mov    eax, 8007000eh
        jmp    NEAR PTR loc_4_0A79
loc_4_0952:
        mov    eax, dword ptr [ebp - 8]
        db  0C7h, 006h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 004h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 8], esi
        mov    dword ptr [esi + 0ch], esi
        mov    dword ptr [esi + 14h], eax
        mov    eax, 0ffffh
        mov    dword ptr [esi + 34h], eax
        mov    dword ptr [esi + 38h], eax
        mov    dword ptr [esi + 3ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [edi + 1ch]
        mov    ebx, dword ptr [eax + 1ch]
        mov    dword ptr [esi + 1ch], ecx
        mov    eax, dword ptr [edi + 30h]
        mov    dword ptr [esi + 30h], eax
        mov    edx, dword ptr [edi + 28h]
        mov    dword ptr [esi + 28h], edx
        lea    edx, [ebp - 4]
        mov    ecx, dword ptr [edi + 2ch]
        push   edx
        push   ebx
        mov    dword ptr [esi + 2ch], ecx
        mov    eax, dword ptr [edi + 20h]
        mov    dword ptr [esi + 20h], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_09A7:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_09C3
        push   0
        push   esi
        call   sub_4_4D5C
        add    esp, 8
        mov    eax, 8878000ah
        jmp    NEAR PTR loc_4_0A79
loc_4_09C3:
        mov    eax, dword ptr [ebp - 4]
        push   80h
        mov    dword ptr [esi + 18h], eax
        mov    ecx, dword ptr [esi + 20h]
        mov    dword ptr [esi + 24h], ecx
        mov    eax, dword ptr [ebp - 4]
        or     eax, 2a0h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_09E3:
        push   0ffffh
        mov    eax, dword ptr [ebp - 4]
        or     al, 60h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_09F4:
        push   0
        mov    eax, dword ptr [ebp - 4]
        or     al, 20h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A02:
        push   0
        push   dword ptr [ebp - 4]
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A0D:
        mov    eax, dword ptr [esi + 30h]
        shr    eax, 1
        dec    eax
        or     eax, 0c0000000h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     al, 0c0h
        mov    edi, 0e0h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A2A:
        mov    eax, dword ptr [ebp - 4]
        or     eax, edi
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A36:
        mov    ecx, dword ptr [esi + 30h]
        and    eax, 0ff000000h
        shr    ecx, 1
        or     ecx, eax
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        or     eax, edi
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A4F:
        mov    eax, dword ptr [esi + 30h]
        shr    eax, 1
        dec    eax
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     ah, 2
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A63:
        mov    ecx, dword ptr [ebp - 8]
        lea    eax, [esi + 4]
        mov    edx, dword ptr [ebp + 10h]
        push   esi
        inc    dword ptr [ecx + 20h]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esi]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_4_0A79:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_4_0A82:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   201h
        push   24h
        call   sub_4_4D6C
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     NEAR PTR loc_4_0B63
        lea    edi, [esi + 4]
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        db  0C7h, 006h, 000h, 000h, 000h, 000h
        push   1
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        push   ecx
        lea    edx, [ebp - 18h]
        push   edx
        mov    dword ptr [esi + 8], esi
        mov    dword ptr [esi + 0ch], esi
        mov    dword ptr [esi + 14h], ebx
        mov    dword ptr [esi + 1ch], eax
        call   sub_4_4D88
        test   eax, eax
        je     SHORT loc_4_0AE1
        push   0
        jmp    SHORT loc_4_0B5A
loc_4_0AE1:
        xor    eax, eax
        db  0CDh, 020h, 000h, 000h, 0A5h, 032h
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 414h
        mov    eax, dword ptr [ebp - 4]
        jae    SHORT loc_4_0B09
        push   dword ptr [eax + 0bh]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_4_4D74
        push   0
        jmp    SHORT loc_4_0B5A
loc_4_0B09:
        push   esi
        mov    dword ptr [esi + 18h], eax
        push   ebx
        call   sub_4_001B
        add    esp, 8
        mov    dword ptr [ebp - 0ch], edi
        mov    dword ptr [ebp - 10h], 0
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 001h, 000h, 0A5h, 032h
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        xor    edi, edi
        cmp    dword ptr [ebp - 8], edi
        je     SHORT loc_4_0B63
        push   edi
        push   ebx
        call   sub_4_001B
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0bh]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_4_4D74
        push   edi
loc_4_0B5A:
        push   esi
        call   sub_4_4D5C
        add    esp, 8
loc_4_0B63:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_0B6A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   dword ptr [ebp + 8]
        call   0
        add    esp, 4
        mov    esi, eax
        xor    eax, eax
        test   esi, esi
        je     SHORT loc_4_0BCD
        mov    dword ptr [ebp - 8], eax
        lea    eax, [esi + 4]
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 002h, 000h, 0A5h, 032h
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_4_001B
        add    esp, 8
        mov    eax, dword ptr [esi + 18h]
        push   dword ptr [eax + 0bh]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_4_4D74
        push   0
        push   esi
        call   sub_4_4D5C
        add    esp, 8
loc_4_0BCD:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh, 0CCh
sub_4_0BD4:
        db  00Fh, 0B7h, 005h, 00Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        ret
sub_4_0BE2:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 4]
        mov    edx, dword ptr [edx]
        mov    ecx, 4354454dh
        xor    eax, eax
        db  0CDh, 020h, 000h, 000h, 0A5h, 032h
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_4_0C1E
        xor    eax, eax
        cmp    ecx, 454d4354h
        jne    SHORT loc_4_0C1E
        mov    eax, edx
loc_4_0C1E:
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx], eax
        xor    eax, eax
        cmp    dword ptr [ebp - 8], eax
        je     SHORT loc_4_0C41
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0C41
        push   eax
        push   eax
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0C3A:
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_4_0C46
loc_4_0C41:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0C46:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0C4B:
        mov    esp, ebp
        pop    ebp
        ret
sub_4_0C4F:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_0C6B
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_0C6B
        xor    eax, eax
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_0C70
loc_4_0C6B:
        mov    eax, 1
loc_4_0C70:
        ret
sub_4_0C71:
        xor    eax, eax
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0C8E
        push   eax
        push   eax
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 10h
loc_4_0C8E:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0CC0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0CA2:
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        push   dword ptr [eax + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0CB2:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_4_0C8E
        mov    eax, 1
        jmp    SHORT loc_4_0CC2
loc_4_0CC0:
        xor    eax, eax
loc_4_0CC2:
        ret
sub_4_0CC3:
        sub    esp, 18h
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 24h]
        push   edi
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0CD4:
        add    esp, 4
        lea    edx, [esi + 24h]
        xor    ecx, ecx
loc_4_0CDC:
        mov    di, word ptr [edx]
        xor    eax, eax
loc_4_0CE1:
        lea    ebx, [eax + edi]
        lea    ebp, [eax + ecx]
        mov    word ptr [esp + ebp*2 + 10h], bx
        inc    eax
        cmp    eax, 4
        jl     SHORT loc_4_0CE1
        add    edx, 2
        add    ecx, 4
        cmp    ecx, 0ch
        jl     SHORT loc_4_0CDC
        lea    eax, [esp + 10h]
        push   eax
        push   0ch
        push   dword ptr [esi]
        call   sub_4_0DCB
        add    esp, 0ch
        xor    eax, eax
        mov    dword ptr [esi], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
sub_4_0D1E:
        sub    esp, 18h
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 24h]
        push   edi
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D2F:
        add    esp, 4
        lea    ecx, [esi + 24h]
        xor    eax, eax
loc_4_0D37:
        mov    dx, word ptr [ecx]
        xor    edi, edi
loc_4_0D3C:
        lea    ebx, [edx + edi]
        lea    ebp, [eax + edi]
        mov    word ptr [esp + ebp*2 + 10h], bx
        inc    edi
        cmp    edi, 4
        jl     SHORT loc_4_0D3C
        add    ecx, 2
        add    eax, 4
        cmp    eax, 0ch
        jl     SHORT loc_4_0D37
        lea    eax, [esp + 10h]
        push   eax
        push   0ch
        push   dword ptr [esi]
        call   sub_4_0DEE
        add    esp, 0ch
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  0CCh
sub_4_0D74:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   101h
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        pop    ebx
        pop    esi
        pop    edi
        pop    ebp
        ret
sub_4_0D91:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    ebx
        pop    esi
        pop    edi
        pop    ebp
        ret
sub_4_0DAB:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
loc_4_0DB7:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        inc    edi
        inc    edi
        loop   loc_4_0DB7
        xor    eax, eax
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
sub_4_0DCB:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 10h]
loc_4_0DDA:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    edi
        inc    edi
        loop   loc_4_0DDA
        xor    eax, eax
        pop    ebx
        pop    esi
        pop    edi
        pop    ebp
        ret
sub_4_0DEE:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 10h]
loc_4_0DFD:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    edi
        inc    edi
        loop   loc_4_0DFD
        xor    eax, eax
        pop    ebx
        pop    esi
        pop    edi
        pop    ebp
        ret
sub_4_0E11:
        push   ebp
        mov    ebp, esp
        pushad
        mov    edi, dword ptr [ebp + 14h]
        mov    ecx, 0ah
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        cld
        rep movsb
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [ebp + 10h]
        sub    eax, esi
        sub    eax, 0ah
        mov    dword ptr [esi + 6], eax
        mov    ecx, dword ptr [ebp + 8]
loc_4_0E40:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_4_0E57
        inc    edi
        inc    edi
        loop   loc_4_0E40
        xor    eax, eax
        mov    dword ptr [esp + 1ch], eax
        jmp    SHORT loc_4_0E77
loc_4_0E57:
        sub    ecx, dword ptr [ebp + 8]
        neg    ecx
        jecxz  loc_4_0E6E
        mov    edi, dword ptr [ebp + 0ch]
loc_4_0E61:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        inc    edi
        inc    edi
        loop   loc_4_0E61
loc_4_0E6E:
        mov    eax, 1
        mov    dword ptr [esp + 1ch], eax
loc_4_0E77:
        popad
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        db  0CDh, 020h, 000h, 000h, 001h, 000h
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi], ax
        clc
        pop    esi
        pop    ebp
        ret
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h, 0C3h, 0CCh, 0CCh, 0CCh, 0A1h, 000h, 000h, 000h, 000h, 0C3h
loc_4_0E9E:
        call   sub_4_0C4F
        or     eax, eax
        jne    SHORT loc_4_0EB9
        mov    eax, 15000eh
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        clc
        ret
loc_4_0EB9:
        stc
        ret
loc_4_0EBB:
        call   sub_4_0C71
        or     eax, eax
        jne    SHORT loc_4_0ED6
        mov    eax, 15000eh
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
        clc
        ret
loc_4_0ED6:
        stc
        ret
loc_4_0ED8:
        pushfd
        pushad
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_0EFD
        xor    eax, eax
        db  0A3h, 000h, 000h, 000h, 000h
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_0EFD
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0EFA:
        add    esp, 4
loc_4_0EFD:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
loc_4_0F03:
        or     esi, esi
        je     SHORT loc_4_0F1D
        cmp    ebx, dword ptr [esi]
        je     SHORT loc_4_0F10
        mov    esi, dword ptr [esi + 4]
        jmp    SHORT loc_4_0F03
loc_4_0F10:
        xor    eax, eax
        mov    dword ptr [esi], eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F1A:
        add    esp, 4
loc_4_0F1D:
        popad
        popfd
        clc
        ret
sub_4_0F21:
        push   ebp
        mov    ebp, esp
        push   0
        push   0
        push   92h
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        pop    ebp
        ret
sub_4_0F4B:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
        pop    ebp
        ret
loc_4_0F5E:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        test   eax, eax
        push   esi
        push   edi
        je     SHORT loc_4_0F96
        cmp    eax, 1
        je     SHORT loc_4_0FB1
        cmp    eax, 2
        je     NEAR PTR loc_4_0FFF
        cmp    eax, 3
        je     NEAR PTR loc_4_1045
        cmp    eax, 4
        je     SHORT loc_4_0FFF
        cmp    eax, 0dh
        je     SHORT loc_4_0FFF
        mov    eax, 1
        jmp    NEAR PTR loc_4_1069
loc_4_0F96:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F9B:
        test   eax, eax
        je     SHORT loc_4_0FAA
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0FA7:
        add    esp, 4
loc_4_0FAA:
        xor    eax, eax
        jmp    NEAR PTR loc_4_1069
loc_4_0FB1:
        cmp    dword ptr [ebp + 0ch], 1
        jbe    SHORT loc_4_0FC1
        mov    eax, 1
        jmp    NEAR PTR loc_4_1069
loc_4_0FC1:
        push   16d5h
        call   sub_4_0D74
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_4_0FDE
        mov    eax, 13h
        jmp    NEAR PTR loc_4_1069
loc_4_0FDE:
        push   dword ptr [ebp + 10h]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0FE7:
        add    esp, 8
        mov    edi, eax
        lea    eax, [esi + 24h]
        push   eax
        push   dword ptr [esi + 8]
        call   sub_4_0A82
        add    esp, 8
        mov    eax, edi
        jmp    SHORT loc_4_1069
loc_4_0FFF:
        push   dword ptr [ebp + 10h]
        call   sub_4_0B6A
        add    esp, 4
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1012:
        add    esp, 4
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_101D:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_4_1041
        cmp    dword ptr [esi + 1558h], 10h
        jne    SHORT loc_4_1038
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1035:
        add    esp, 4
loc_4_1038:
        push   esi
        call   sub_4_0D91
        add    esp, 4
loc_4_1041:
        xor    eax, eax
        jmp    SHORT loc_4_1069
loc_4_1045:
        cmp    dword ptr [ebp + 0ch], 1
        mov    eax, 1
        ja     SHORT loc_4_1069
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1058:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
loc_4_1069:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_106D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 80h
        push   edi
        push   2beh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_108A:
        push   -20h
        push   0deh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1097:
        push   0ffffe8h
        push   0feh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_10A7:
        push   0
        push   3eh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_10B1:
        push   0
        push   1eh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_10BB:
        push   0ffffe3h
        push   21eh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_10CB:
        push   edi
        push   2bfh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_10D7:
        push   0fffff0h
        mov    edi, 0ffh
        push   0dfh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_10EC:
        push   0fffff8h
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_10F8:
        push   edi
        push   3fh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1101:
        push   8000h
        push   1fh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_110E:
        push   0fffff3h
        push   21fh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_111E:
        mov    cx, word ptr [esi + 4]
        add    cx, 2
        mov    ax, word ptr [esi + 2]
        mov    word ptr [ebp - 2], ax
        mov    di, word ptr [esi]
        mov    eax, dword ptr [esi + 152ah]
        sub    eax, 2
        cmp    eax, 1
        sbb    ebx, ebx
        and    ebx, 0ffffffd8h
        lea    ebx, [ebx + 4828h]
        pushfd
        mov    edx, dword ptr [esp]
        test   dh, 2
        je     SHORT loc_4_1152
        cli
loc_4_1152:
        mov    dx, cx
        mov    ax, 3eh
        out    dx, ax
        mov    dx, di
        xor    eax, eax
        out    dx, ax
        add    di, 2
loc_4_1166:
        mov    dx, cx
        in     ax, dx
        test   ah, 10h
        je     SHORT loc_4_1166
loc_4_1170:
        mov    dx, cx
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_4_1170
        mov    dx, di
        mov    ax, bx
        out    dx, ax
        mov    dx, cx
        mov    ax, 3ch
        out    dx, ax
        mov    dx, word ptr [ebp - 2]
        xor    eax, eax
        out    dx, ax
        pop    eax
        test   ah, 2
        je     SHORT loc_4_119A
        sti
loc_4_119A:
        mov    edi, 8000ffffh
        push   edi
        push   7eh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_11A8:
        push   edi
        push   7fh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_11B1:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_4_11BC:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0ch]
        push   edi
        mov    edi, dword ptr [esp + 14h]
        lea    esi, [ebx + 0ch]
        cmp    dword ptr [esi], edi
        jae    SHORT loc_4_11D2
        xor    eax, eax
        jmp    SHORT loc_4_122A
loc_4_11D2:
        xor    ecx, ecx
        lea    eax, [ebx + 0aeeh]
loc_4_11DA:
        cmp    dword ptr [eax], 0
        je     SHORT loc_4_11E4
        cmp    dword ptr [eax + 4], edi
        jae    SHORT loc_4_11F0
loc_4_11E4:
        add    eax, 8
        inc    ecx
        cmp    ecx, 80h
        jb     SHORT loc_4_11DA
loc_4_11F0:
        cmp    ecx, 80h
        jne    SHORT loc_4_1201
        push   edi
        push   ebx
        call   sub_4_122E
        mov    ecx, eax
loc_4_1201:
        lea    edx, [ebx + ecx*8 + 0aeeh]
        lea    ebx, [ebx + ecx*8 + 0af2h]
        mov    eax, dword ptr [edx]
        cmp    dword ptr [ebx], edi
        jbe    SHORT loc_4_1220
        lea    ecx, [edi + eax]
        mov    dword ptr [edx], ecx
        sub    dword ptr [ebx], edi
        sub    dword ptr [esi], edi
        jmp    SHORT loc_4_122A
loc_4_1220:
        mov    dword ptr [edx], 0
        mov    edx, dword ptr [ebx]
        sub    dword ptr [esi], edx
loc_4_122A:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_4_122E:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
loc_4_123C:
        push   0
        push   0
        push   0ffh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_124B:
        mov    byte ptr [ebp + edi - 2ch], al
        add    esp, 10h
        cmp    eax, 0ffh
        mov    ebx, eax
        je     NEAR PTR loc_4_12ED
        shl    eax, 5
        cmp    byte ptr [eax + esi + 59h], 0
        je     SHORT loc_4_126C
        dec    dword ptr [esi + 18h]
loc_4_126C:
        push   80h
        mov    eax, ebx
        or     eax, 2a0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_127F:
        push   0
        mov    eax, ebx
        or     al, 60h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_128C:
        push   0
        mov    eax, ebx
        or     al, 40h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1299:
        push   0
        mov    eax, ebx
        or     al, 0c0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_12A6:
        push   0
        mov    eax, ebx
        or     al, 0e0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_12B3:
        push   40000000h
        mov    eax, ebx
        or     al, 20h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_12C3:
        push   40000000h
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_12CF:
        test   edi, 1
        je     SHORT loc_4_12DE
        push   4000000h
        jmp    SHORT loc_4_12E3
loc_4_12DE:
        push   6000000h
loc_4_12E3:
        or     bh, 2
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_12ED:
        inc    edi
        cmp    edi, 0ah
        jb     NEAR PTR loc_4_123C
        xor    ecx, ecx
        mov    edx, 0ffffffffh
        xor    edi, edi
        lea    eax, [esi + 0aeeh]
        mov    dword ptr [ebp - 1ch], ecx
        mov    dword ptr [ebp - 8], ecx
loc_4_130C:
        mov    ebx, dword ptr [eax]
        test   ebx, ebx
        je     SHORT loc_4_131E
        cmp    edx, ebx
        jbe    SHORT loc_4_131E
        mov    edx, ebx
        mov    dword ptr [ebp - 1ch], edi
        mov    dword ptr [ebp - 8], eax
loc_4_131E:
        add    eax, 8
        inc    edi
        cmp    edi, 80h
        jb     SHORT loc_4_130C
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 4
        mov    dword ptr [ebp - 4], ecx
        mov    edi, dword ptr [ecx]
        add    edi, edx
loc_4_1337:
        mov    dword ptr [ebp - 20h], 0
        lea    eax, [esi + 662h]
        mov    dword ptr [ebp - 14h], eax
loc_4_1347:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        test   eax, eax
        je     NEAR PTR loc_4_1468
        mov    ebx, dword ptr [eax + 18h]
        cmp    dword ptr [ebx + 1ch], edi
        ja     NEAR PTR loc_4_1468
        movzx  eax, word ptr [eax + 2ah]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        imul   eax, eax, 24h
        add    eax, ebx
        mov    edx, dword ptr [eax - 4]
        add    edx, dword ptr [eax - 8]
        cmp    edx, dword ptr [ecx]
        jb     NEAR PTR loc_4_1468
        mov    dword ptr [ebp - 18h], 0
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_4_1468
loc_4_138E:
        cmp    dword ptr [ebx + 1ch], edi
        jne    NEAR PTR loc_4_1456
        cmp    dword ptr [ebx + 20h], 0
        je     NEAR PTR loc_4_1456
        or     byte ptr [ebx + 3], 80h
        push   dword ptr [ebx + 1ch]
        push   234h
        mov    eax, dword ptr [ebx + 20h]
        push   esi
        mov    dword ptr [ebp - 10h], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_13B9:
        mov    ecx, dword ptr [ebp - 8]
        push   dword ptr [ecx]
        push   236h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_13C9:
        push   23ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_13D4:
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [ebp - 10h]
        test   eax, eax
        je     SHORT loc_4_1402
loc_4_13DE:
        push   23ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_13E9:
        movzx  eax, ax
        push   eax
        push   23ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_13F8:
        mov    ecx, dword ptr [ebp - 10h]
        dec    dword ptr [ebp - 10h]
        test   ecx, ecx
        jne    SHORT loc_4_13DE
loc_4_1402:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx]
        sub    dword ptr [ebx], eax
        mov    eax, dword ptr [ecx]
        sub    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [ecx]
        sub    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [ecx]
        mov    ecx, dword ptr [ebp - 14h]
        sub    dword ptr [ebx + 0ch], eax
        mov    edx, dword ptr [ecx]
        add    edx, 40h
        mov    eax, dword ptr [edx]
        cmp    dword ptr [ebx + 1ch], eax
        jne    SHORT loc_4_142E
        mov    ecx, dword ptr [ebp - 4]
        sub    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
loc_4_142E:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx]
        mov    ecx, dword ptr [ebp - 8]
        sub    dword ptr [ebx + 1ch], eax
        mov    eax, dword ptr [ebp - 18h]
        and    byte ptr [ebx + 3], 0
        mov    edx, dword ptr [ebx + 20h]
        sub    eax, dword ptr [ebp - 0ch]
        add    dword ptr [ecx], edx
        add    edi, dword ptr [ebx + 20h]
        cmp    eax, -1
        je     SHORT loc_4_147E
        cmp    dword ptr [ebx + 44h], 0
        je     SHORT loc_4_147E
loc_4_1456:
        add    ebx, 24h
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 18h]
        cmp    dword ptr [ebp - 18h], eax
        jb     NEAR PTR loc_4_138E
loc_4_1468:
        add    dword ptr [ebp - 14h], 4
        inc    dword ptr [ebp - 20h]
        cmp    dword ptr [ebp - 20h], 120h
        jb     NEAR PTR loc_4_1347
        jmp    SHORT loc_4_14BF
loc_4_147E:
        xor    edx, edx
        lea    eax, [esi + 0aeeh]
loc_4_1486:
        cmp    dword ptr [eax], edi
        je     SHORT loc_4_149B
        add    eax, 8
        inc    edx
        cmp    edx, 80h
        jb     SHORT loc_4_1486
        jmp    NEAR PTR loc_4_1337
loc_4_149B:
        mov    eax, dword ptr [esi + edx*8 + 0af2h]
        mov    ecx, dword ptr [ebp - 4]
        add    edi, eax
        add    dword ptr [ecx], eax
        mov    dword ptr [esi + edx*8 + 0aeeh], 0
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx], eax
        jb     NEAR PTR loc_4_1337
loc_4_14BF:
        xor    edi, edi
loc_4_14C1:
        movzx  ebx, byte ptr [ebp + edi - 2ch]
        cmp    ebx, 0ffh
        je     SHORT loc_4_151F
        push   0
        mov    eax, ebx
        or     ah, 2
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_14DC:
        push   1d8h
        mov    eax, ebx
        or     al, 0c0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_14EC:
        push   1e0h
        mov    eax, ebx
        or     al, 0e0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_14FC:
        push   48000000h
        mov    eax, ebx
        shl    ebx, 5
        or     al, 20h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_150F:
        lea    eax, [ebx + esi]
        mov    word ptr [eax + 42h], 0ffeh
        mov    byte ptr [eax + 59h], 1
        inc    dword ptr [esi + 18h]
loc_4_151F:
        inc    edi
        cmp    edi, 0ah
        jb     SHORT loc_4_14C1
        mov    eax, dword ptr [ebp - 1ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_4_1531:
        mov    eax, dword ptr [esp + 0ch]
        sub    esp, 4
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        push   ebp
        je     NEAR PTR loc_4_161E
        mov    edi, dword ptr [esp + 1ch]
        cmp    edi, 200000h
        jb     NEAR PTR loc_4_161E
        cmp    edi, 1000000h
        jae    NEAR PTR loc_4_161E
        lea    ecx, [edi + eax]
        mov    esi, dword ptr [esp + 18h]
        xor    ebx, ebx
        mov    dword ptr [esp + 10h], ecx
        lea    edx, [esi + 0aeeh]
        add    dword ptr [esi + 0ch], eax
loc_4_1576:
        xor    eax, eax
        mov    ecx, edx
loc_4_157A:
        mov    ebp, dword ptr [ecx]
        cmp    dword ptr [esp + 10h], ebp
        je     NEAR PTR loc_4_1626
        mov    ebp, dword ptr [ecx + 4]
        add    ebp, dword ptr [ecx]
        cmp    ebp, edi
        je     NEAR PTR loc_4_163D
        add    ecx, 8
        inc    eax
        cmp    eax, 80h
        jb     SHORT loc_4_157A
        xor    eax, eax
loc_4_15A0:
        cmp    dword ptr [edx], 0
        je     SHORT loc_4_15B0
        add    edx, 8
        inc    eax
        cmp    eax, 80h
        jb     SHORT loc_4_15A0
loc_4_15B0:
        cmp    eax, 80h
        je     SHORT loc_4_15CB
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [esi + eax*8 + 0aeeh], edi
        sub    ecx, edi
        mov    dword ptr [esi + eax*8 + 0af2h], ecx
loc_4_15CB:
        xor    ebx, ebx
        cmp    word ptr [esi + 6], bx
        jbe    SHORT loc_4_161E
        lea    ebp, [esi + 42h]
loc_4_15D6:
        movzx  eax, word ptr [ebp]
        and    al, 0
        cmp    eax, 0ff00h
        je     SHORT loc_4_1612
        mov    eax, ebx
        or     ah, 2
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_15EF:
        cmp    eax, edi
        jb     SHORT loc_4_1612
        cmp    eax, dword ptr [esp + 10h]
        jae    SHORT loc_4_1612
        push   807fh
        mov    eax, ebx
        or     eax, 2a0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_160C:
        mov    word ptr [ebp], 0ffeh
loc_4_1612:
        add    ebp, 20h
        inc    ebx
        movzx  eax, word ptr [esi + 6]
        cmp    eax, ebx
        ja     SHORT loc_4_15D6
loc_4_161E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_4_1626:
        mov    ecx, dword ptr [esi + eax*8 + 0af2h]
        add    dword ptr [esp + 10h], ecx
        mov    dword ptr [esi + eax*8 + 0aeeh], ebx
        jmp    NEAR PTR loc_4_1576
loc_4_163D:
        lea    eax, [esi + eax*8 + 0aeeh]
        mov    edi, dword ptr [eax]
        mov    dword ptr [eax], ebx
        jmp    NEAR PTR loc_4_1576
sub_4_164D:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    edi, dword ptr [esp + 14h]
        push   ebp
        movzx  eax, word ptr [edi]
        mov    eax, dword ptr [esi + eax*4 + 662h]
        movzx  ebx, word ptr [eax + 2ah]
        mov    ecx, dword ptr [eax + 18h]
        mov    eax, ebx
        dec    ebx
        test   eax, eax
        je     SHORT loc_4_1698
        mov    eax, ebx
        imul   eax, eax, 24h
        lea    ebp, [eax + ecx + 1ch]
loc_4_167A:
        mov    eax, dword ptr [ebp]
        test   eax, eax
        je     SHORT loc_4_1698
        push   dword ptr [ebp + 4]
        push   eax
        push   esi
        sub    ebp, 24h
        call   sub_4_1531
        add    esp, 0ch
        mov    ecx, ebx
        dec    ebx
        test   ecx, ecx
        jne    SHORT loc_4_167A
loc_4_1698:
        xor    eax, eax
        pop    ebp
        movzx  ecx, word ptr [edi]
        pop    edi
        mov    dword ptr [esi + ecx*4 + 662h], 0
        pop    esi
        pop    ebx
        ret    8
sub_4_16AF:
        push   ebp
        mov    ebp, esp
        sub    esp, 3ch
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 2ch], esi
        mov    dword ptr [ebp - 20h], esi
        mov    dword ptr [ebp - 18h], esi
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 14h], esi
        mov    dword ptr [ebp - 1ch], esi
        mov    dword ptr [ebp - 10h], esi
        mov    eax, dword ptr [ecx + 3ch]
        test   eax, eax
        je     SHORT loc_4_1704
        add    eax, 10h
        push   eax
        push   ecx
        call   sub_4_11BC
        add    esp, 8
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, esi
        mov    dword ptr [ecx + 38h], eax
        jne    SHORT loc_4_16FA
        mov    eax, 2
        jmp    NEAR PTR loc_4_1E09
loc_4_16FA:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 3ch], 0
loc_4_1704:
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 10h]
        add    ecx, 40h
        lea    ebx, [edi + 4ch]
        mov    ax, word ptr [ecx]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edi + 48h], ax
        mov    dword ptr [ebp - 34h], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edx + 20h]
        add    ecx, 14h
        mov    dword ptr [ebp - 38h], eax
        mov    dword ptr [ebp - 8], ecx
        test   eax, eax
        je     NEAR PTR loc_4_1B85
loc_4_1734:
        mov    eax, dword ptr [ebp - 8]
        add    dword ptr [ebp - 8], 8
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [eax + 4]
        mov    eax, edx
        mov    dword ptr [ebp - 28h], ecx
        mov    dword ptr [ebp - 24h], edx
        lea    ecx, [eax + 8]
        sub    dword ptr [ebp - 38h], ecx
        js     NEAR PTR loc_4_1BEF
        mov    ecx, dword ptr [ebp - 28h]
        cmp    ecx, 67616269h
        je     SHORT loc_4_17A8
        cmp    ecx, 67616270h
        je     SHORT loc_4_17D2
        cmp    ecx, 6e656769h
        je     NEAR PTR loc_4_17FC
        cmp    ecx, 6e656770h
        je     NEAR PTR loc_4_185D
        cmp    ecx, 72646870h
        je     NEAR PTR loc_4_18BE
        cmp    ecx, 72646873h
        je     NEAR PTR loc_4_1988
        cmp    ecx, 74736e69h
        je     NEAR PTR loc_4_1B38
        jmp    NEAR PTR loc_4_1B6F
loc_4_17A8:
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, eax
        mov    dword ptr [edi + 10h], ebx
        shr    ecx, 2
        mov    edi, ebx
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 24h]
        add    ebx, dword ptr [ebp - 24h]
        shr    eax, 2
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_4_1B6F
loc_4_17D2:
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, eax
        mov    dword ptr [edi + 4], ebx
        shr    ecx, 2
        mov    edi, ebx
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 24h]
        add    ebx, dword ptr [ebp - 24h]
        shr    eax, 2
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_4_1B6F
loc_4_17FC:
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [edi + 14h], ebx
        shr    eax, 2
        mov    dword ptr [ebp - 10h], eax
        test   eax, eax
        je     NEAR PTR loc_4_1B6F
        mov    edi, eax
loc_4_1818:
        mov    al, byte ptr [esi]
        mov    ecx, dword ptr [ebp - 34h]
        mov    byte ptr [ebx], al
        cmp    word ptr [ecx], 1
        jne    SHORT loc_4_183A
        movsx  eax, word ptr [esi + 2]
        movzx  ecx, word ptr [esi]
        push   1
        push   eax
        push   ecx
        call   sub_4_2758
        add    esp, 0ch
        jmp    SHORT loc_4_184B
loc_4_183A:
        movsx  eax, word ptr [esi + 2]
        movzx  ecx, word ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2923
        add    esp, 8
loc_4_184B:
        mov    word ptr [ebx + 1], ax
        add    esi, 4
        add    ebx, 3
        dec    edi
        jne    SHORT loc_4_1818
        jmp    NEAR PTR loc_4_1B6F
loc_4_185D:
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [edi + 8], ebx
        shr    eax, 2
        mov    dword ptr [ebp - 18h], eax
        test   eax, eax
        je     NEAR PTR loc_4_1B6F
        mov    edi, eax
loc_4_1879:
        mov    al, byte ptr [esi]
        mov    ecx, dword ptr [ebp - 34h]
        mov    byte ptr [ebx], al
        cmp    word ptr [ecx], 1
        jne    SHORT loc_4_189B
        movsx  eax, word ptr [esi + 2]
        movzx  ecx, word ptr [esi]
        push   1
        push   eax
        push   ecx
        call   sub_4_2758
        add    esp, 0ch
        jmp    SHORT loc_4_18AC
loc_4_189B:
        movsx  eax, word ptr [esi + 2]
        movzx  ecx, word ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2A95
        add    esp, 8
loc_4_18AC:
        mov    word ptr [ebx + 1], ax
        add    esi, 4
        add    ebx, 3
        dec    edi
        jne    SHORT loc_4_1879
        jmp    NEAR PTR loc_4_1B6F
loc_4_18BE:
        mov    eax, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 10h]
        sub    edx, edx
        mov    ecx, 26h
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [edi], ebx
        mov    word ptr [edi + 24h], 0ffffh
        mov    word ptr [edi + 26h], 0ffffh
        mov    eax, dword ptr [ebp - 24h]
        div    ecx
        mov    dword ptr [ebp - 2ch], eax
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 2ch]
        dec    ecx
        mov    dword ptr [ebp - 30h], ecx
        cmp    ecx, eax
        je     SHORT loc_4_196C
        mov    edx, 80h
loc_4_18F6:
        mov    word ptr [ebx], 1
        mov    ecx, dword ptr [ebp - 0ch]
        mov    cx, word ptr [ecx + 16h]
        mov    word ptr [ebp - 3ch], cx
        cmp    cx, dx
        mov    ecx, dword ptr [ebp + 0ch]
        jne    SHORT loc_4_1935
        mov    cl, byte ptr [ecx]
        mov    esi, dword ptr [ebp - 0ch]
        mov    byte ptr [ebx + 3], cl
        mov    cx, word ptr [esi + 14h]
        or     cl, dl
        mov    byte ptr [ebx + 2], cl
        cmp    cl, dl
        jne    SHORT loc_4_1947
        mov    edi, dword ptr [ebp + 10h]
        movzx  cx, byte ptr [ebx + 3]
        mov    word ptr [edi + 24h], cx
        mov    word ptr [edi + 26h], ax
        jmp    SHORT loc_4_1947
loc_4_1935:
        mov    cx, word ptr [ecx]
        mov    esi, dword ptr [ebp - 0ch]
        add    cl, byte ptr [ebp - 3ch]
        mov    byte ptr [ebx + 3], cl
        mov    cl, byte ptr [esi + 14h]
        mov    byte ptr [ebx + 2], cl
loc_4_1947:
        lea    edi, [ebx + 6]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    cx, word ptr [ecx + 18h]
        mov    esi, dword ptr [ebp - 0ch]
        mov    word ptr [ebx + 4], cx
        add    ebx, 1ah
        mov    ecx, 5
        rep movsd
        add    dword ptr [ebp - 0ch], 26h
        inc    eax
        cmp    dword ptr [ebp - 30h], eax
        ja     SHORT loc_4_18F6
loc_4_196C:
        mov    word ptr [ebx], 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ebx + 3], 0ffh
        mov    ax, word ptr [ecx + 18h]
        add    ebx, 1ah
        mov    word ptr [ebx - 16h], ax
        jmp    NEAR PTR loc_4_1B6F
loc_4_1988:
        mov    edi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [edi + 18h], ebx
        cmp    word ptr [eax], 1
        jne    SHORT loc_4_1A03
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 1524h], 0
        je     SHORT loc_4_19B9
        cmp    dword ptr [edx], 100h
        jae    NEAR PTR loc_4_1BF9
loc_4_19B9:
        mov    eax, dword ptr [ebp - 24h]
        mov    edi, dword ptr [ebp + 10h]
        shr    eax, 4
        mov    word ptr [edi + 2ah], ax
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     NEAR PTR loc_4_1A94
        mov    ecx, eax
        xor    esi, esi
loc_4_19D5:
        mov    eax, dword ptr [edx]
        add    edx, 10h
        mov    dword ptr [ebx], eax
        add    ebx, 24h
        mov    edi, dword ptr [edx - 0ch]
        dec    ecx
        mov    dword ptr [ebx - 20h], edi
        mov    eax, dword ptr [edx - 8]
        mov    dword ptr [ebx - 1ch], eax
        mov    edi, dword ptr [edx - 4]
        mov    dword ptr [ebx - 18h], edi
        mov    dword ptr [ebx - 8], esi
        mov    dword ptr [ebx - 4], esi
        mov    word ptr [ebx - 0ah], si
        jne    SHORT loc_4_19D5
        jmp    NEAR PTR loc_4_1A94
loc_4_1A03:
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 1524h], 0
        je     SHORT loc_4_1A1F
        cmp    dword ptr [esi + 14h], 100h
        jae    NEAR PTR loc_4_1C03
loc_4_1A1F:
        mov    eax, dword ptr [ebp - 24h]
        sub    edx, edx
        mov    ecx, 2eh
        mov    edi, dword ptr [ebp + 10h]
        div    ecx
        mov    word ptr [edi + 2ah], ax
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_4_1A94
        xor    edi, edi
        mov    dword ptr [ebp - 30h], eax
loc_4_1A3E:
        mov    eax, dword ptr [esi + 14h]
        add    esi, 2eh
        mov    dword ptr [ebx], eax
        add    ebx, 24h
        mov    ecx, dword ptr [esi - 16h]
        mov    dword ptr [ebx - 20h], ecx
        mov    edx, dword ptr [esi - 12h]
        mov    dword ptr [ebx - 1ch], edx
        mov    eax, dword ptr [esi - 0eh]
        mov    dword ptr [ebx - 18h], eax
        mov    dword ptr [ebx - 8], edi
        mov    dword ptr [ebx - 4], edi
        mov    word ptr [ebx - 0ah], di
        push   dword ptr [esi - 0ah]
        call   sub_4_26D3
        add    esp, 4
        mov    dword ptr [ebx - 0eh], eax
        mov    cl, byte ptr [esi - 6]
        dec    dword ptr [ebp - 30h]
        mov    byte ptr [ebx - 10h], cl
        mov    dl, byte ptr [esi - 5]
        mov    byte ptr [ebx - 0fh], dl
        mov    ax, word ptr [esi - 4]
        mov    word ptr [ebx - 14h], ax
        mov    cx, word ptr [esi - 2]
        mov    word ptr [ebx - 12h], cx
        jne    SHORT loc_4_1A3E
loc_4_1A94:
        mov    edi, dword ptr [ebp + 10h]
        xor    eax, eax
        cmp    dword ptr [ebp - 4], eax
        mov    esi, dword ptr [edi + 18h]
        je     NEAR PTR loc_4_1B6F
loc_4_1AA5:
        cmp    dword ptr [esi], 100h
        jb     SHORT loc_4_1ABB
        inc    eax
        add    esi, 24h
        cmp    dword ptr [ebp - 4], eax
        ja     SHORT loc_4_1AA5
        jmp    NEAR PTR loc_4_1B6F
loc_4_1ABB:
        mov    edx, dword ptr [ebp + 8]
        add    edx, 38h
        mov    ecx, dword ptr [edx]
        cmp    ecx, 1000000h
        jb     SHORT loc_4_1ACE
        add    ecx, 10h
loc_4_1ACE:
        cmp    dword ptr [ebp - 4], eax
        jbe    SHORT loc_4_1B1C
        mov    edi, dword ptr [ebp - 4]
        sub    edi, eax
        mov    dword ptr [ebp - 30h], edi
loc_4_1ADB:
        mov    dword ptr [esi + 1ch], ecx
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_4_1B14
        mov    eax, dword ptr [edx]
        mov    dword ptr [ebp - 0ch], esi
        add    eax, 10h
        add    dword ptr [esi], eax
        mov    eax, dword ptr [edx]
        add    eax, 10h
        add    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edx]
        add    eax, 10h
        add    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [edx]
        mov    edi, dword ptr [esi + 4]
        lea    eax, [edi + eax + 10h]
        mov    dword ptr [esi + 4], eax
        sub    eax, ecx
        inc    eax
        mov    dword ptr [esi + 20h], eax
        mov    ecx, dword ptr [esi + 4]
        inc    ecx
loc_4_1B14:
        add    esi, 24h
        dec    dword ptr [ebp - 30h]
        jne    SHORT loc_4_1ADB
loc_4_1B1C:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_4_1B6F
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ecx + 1ch]
        sub    ecx, dword ptr [eax + 1ch]
        add    ecx, dword ptr [edx]
        add    ecx, 10h
        mov    dword ptr [eax + 20h], ecx
        jmp    SHORT loc_4_1B6F
loc_4_1B38:
        mov    ecx, dword ptr [ebp - 8]
        mov    esi, 16h
        mov    eax, dword ptr [ebp - 24h]
        sub    edx, edx
        div    esi
        mov    edi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 14h], eax
        dec    ax
        mov    dword ptr [edi + 0ch], ebx
        cmp    dword ptr [ebp - 14h], 0
        mov    word ptr [edi + 28h], ax
        je     SHORT loc_4_1B6F
        mov    eax, dword ptr [ebp - 14h]
loc_4_1B5F:
        mov    dx, word ptr [ecx + 14h]
        add    ecx, 16h
        mov    word ptr [ebx], dx
        add    ebx, 2
        dec    eax
        jne    SHORT loc_4_1B5F
loc_4_1B6F:
        mov    eax, dword ptr [ebp - 24h]
        add    dword ptr [ebp - 8], eax
        test   bl, 1
        je     SHORT loc_4_1B7B
        inc    ebx
loc_4_1B7B:
        cmp    dword ptr [ebp - 38h], 0
        jne    NEAR PTR loc_4_1734
loc_4_1B85:
        xor    ecx, ecx
        cmp    dword ptr [ebp - 4], ecx
        je     NEAR PTR loc_4_1DE1
        cmp    dword ptr [ebp - 2ch], ecx
        je     NEAR PTR loc_4_1DE1
        cmp    dword ptr [ebp - 20h], ecx
        je     NEAR PTR loc_4_1DE1
        cmp    dword ptr [ebp - 18h], ecx
        je     NEAR PTR loc_4_1DE1
        cmp    dword ptr [ebp - 14h], ecx
        je     NEAR PTR loc_4_1DE1
        cmp    dword ptr [ebp - 1ch], ecx
        je     NEAR PTR loc_4_1DE1
        cmp    dword ptr [ebp - 10h], ecx
        je     NEAR PTR loc_4_1DE1
loc_4_1BC6:
        mov    eax, dword ptr [ebp - 2ch]
        dec    dword ptr [ebp - 2ch]
        test   eax, eax
        je     SHORT loc_4_1C0D
        mov    edx, dword ptr [ebp - 2ch]
        mov    edi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [edi]
        imul   edx, edx, 1ah
        movzx  edx, word ptr [edx + eax + 4]
        cmp    edx, dword ptr [ebp - 20h]
        jb     SHORT loc_4_1BC6
        mov    esi, 20h
        jmp    NEAR PTR loc_4_1D7A
loc_4_1BEF:
        mov    eax, 3
        jmp    NEAR PTR loc_4_1E09
loc_4_1BF9:
        mov    esi, 4
        jmp    NEAR PTR loc_4_1D7A
loc_4_1C03:
        mov    esi, 4
        jmp    NEAR PTR loc_4_1D7A
loc_4_1C0D:
        mov    edi, dword ptr [ebp + 10h]
        dec    dword ptr [ebp - 20h]
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [edi + 4]
        movzx  edx, word ptr [ecx + eax*4]
        cmp    edx, dword ptr [ebp - 18h]
        ja     SHORT loc_4_1C38
loc_4_1C22:
        mov    eax, dword ptr [ebp - 20h]
        dec    dword ptr [ebp - 20h]
        test   eax, eax
        je     SHORT loc_4_1C42
        mov    eax, dword ptr [ebp - 20h]
        movzx  edx, word ptr [ecx + eax*4]
        cmp    edx, dword ptr [ebp - 18h]
        jb     SHORT loc_4_1C22
loc_4_1C38:
        mov    esi, 21h
        jmp    NEAR PTR loc_4_1D7A
loc_4_1C42:
        mov    eax, dword ptr [ebp - 18h]
        dec    dword ptr [ebp - 18h]
        test   eax, eax
        je     SHORT loc_4_1C75
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 18h]
        imul   ecx, ecx, 3
        mov    eax, dword ptr [edi + 8]
        add    eax, ecx
        cmp    byte ptr [eax], 29h
        jne    SHORT loc_4_1C42
        movsx  eax, word ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 14h]
        dec    ecx
        cmp    eax, ecx
        jb     SHORT loc_4_1C42
        mov    esi, 22h
        jmp    NEAR PTR loc_4_1D7A
loc_4_1C75:
        mov    edi, dword ptr [ebp + 10h]
        dec    dword ptr [ebp - 1ch]
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [edi + 10h]
        movzx  edx, word ptr [ecx + eax*4]
        cmp    edx, dword ptr [ebp - 10h]
        ja     SHORT loc_4_1CA0
loc_4_1C8A:
        mov    eax, dword ptr [ebp - 1ch]
        dec    dword ptr [ebp - 1ch]
        test   eax, eax
        je     SHORT loc_4_1CAA
        mov    eax, dword ptr [ebp - 1ch]
        movzx  edx, word ptr [ecx + eax*4]
        cmp    edx, dword ptr [ebp - 10h]
        jb     SHORT loc_4_1C8A
loc_4_1CA0:
        mov    esi, 24h
        jmp    NEAR PTR loc_4_1D7A
loc_4_1CAA:
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [ebp - 10h]
        test   eax, eax
        je     SHORT loc_4_1D04
        mov    ecx, dword ptr [ebp - 10h]
        imul   ecx, ecx, 3
loc_4_1CBA:
        mov    edi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [edi + 14h]
        add    eax, ecx
        cmp    byte ptr [eax], 29h
        jne    SHORT loc_4_1CD7
        movsx  edx, word ptr [eax + 1]
        mov    esi, dword ptr [ebp - 14h]
        dec    esi
        cmp    edx, esi
        jae    NEAR PTR loc_4_1DA1
loc_4_1CD7:
        cmp    byte ptr [eax], 35h
        jne    SHORT loc_4_1CF7
        movsx  edx, word ptr [eax + 1]
        cmp    dword ptr [ebp - 4], edx
        jbe    NEAR PTR loc_4_1DA1
        mov    edi, dword ptr [ebp + 10h]
        imul   edx, edx, 24h
        mov    eax, dword ptr [edi + 18h]
        inc    word ptr [eax + edx + 1ah]
loc_4_1CF7:
        mov    eax, dword ptr [ebp - 10h]
        sub    ecx, 3
        dec    dword ptr [ebp - 10h]
        test   eax, eax
        jne    SHORT loc_4_1CBA
loc_4_1D04:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    word ptr [ecx], 80h
        ja     SHORT loc_4_1D59
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 1ch]
        mov    eax, dword ptr [ecx + 38h]
        lea    ecx, [edx + eax + 10h]
        mov    edx, dword ptr [ebp - 4]
        dec    dword ptr [ebp - 4]
        test   edx, edx
        je     SHORT loc_4_1D59
        mov    edi, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 18h]
        imul   edx, edx, 24h
        lea    esi, [eax + edx + 1ch]
loc_4_1D35:
        mov    edx, dword ptr [esi]
        test   edx, edx
        je     SHORT loc_4_1D59
        mov    eax, dword ptr [esi - 18h]
        test   eax, eax
        je     SHORT loc_4_1D4C
        cmp    ecx, edx
        jb     SHORT loc_4_1DA8
        cmp    ecx, eax
        jb     SHORT loc_4_1DA8
        mov    ecx, edx
loc_4_1D4C:
        mov    eax, dword ptr [ebp - 4]
        sub    esi, 24h
        dec    dword ptr [ebp - 4]
        test   eax, eax
        jne    SHORT loc_4_1D35
loc_4_1D59:
        mov    eax, dword ptr [ebp - 14h]
        dec    dword ptr [ebp - 14h]
        test   eax, eax
        je     SHORT loc_4_1DAF
        mov    edi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [edi + 0ch]
        movzx  edx, word ptr [ecx + eax*2]
        cmp    edx, dword ptr [ebp - 1ch]
        jb     SHORT loc_4_1D59
        mov    esi, 23h
loc_4_1D7A:
        mov    ecx, dword ptr [ebp + 8]
        xor    edi, edi
        mov    edx, dword ptr [ecx + 38h]
        test   edx, edx
        je     SHORT loc_4_1D9D
        mov    eax, dword ptr [ecx + 1ch]
        add    eax, 10h
        push   eax
        push   edx
        push   ecx
        call   sub_4_1531
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 38h], edi
loc_4_1D9D:
        mov    eax, esi
        jmp    SHORT loc_4_1E09
loc_4_1DA1:
        mov    esi, 25h
        jmp    SHORT loc_4_1D7A
loc_4_1DA8:
        mov    esi, 26h
        jmp    SHORT loc_4_1D7A
loc_4_1DAF:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 10h]
        add    esi, 150ah
        add    edi, 2ch
        mov    ecx, 5
        rep movsd
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        movzx  eax, word ptr [ecx]
        mov    edi, dword ptr [ebp + 10h]
        sub    ebx, edi
        mov    dword ptr [edx + eax*4 + 662h], edi
        xor    eax, eax
        mov    dword ptr [ecx + 10h], ebx
        jmp    SHORT loc_4_1E09
loc_4_1DE1:
        mov    ecx, dword ptr [ebp + 8]
        xor    esi, esi
        mov    edx, dword ptr [ecx + 38h]
        test   edx, edx
        je     SHORT loc_4_1E04
        mov    eax, dword ptr [ecx + 1ch]
        add    eax, 10h
        push   eax
        push   edx
        push   ecx
        call   sub_4_1531
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 38h], esi
loc_4_1E04:
        mov    eax, 1
loc_4_1E09:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_4_1E12:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   ebp
        lea    edi, [esi + 24h]
        mov    eax, dword ptr [esi + 1ch]
        cmp    dword ptr [edi], eax
        jb     SHORT loc_4_1E2B
        mov    eax, 1
        jmp    SHORT loc_4_1EA5
loc_4_1E2B:
        mov    eax, dword ptr [esi + 3ch]
        test   eax, eax
        je     SHORT loc_4_1E54
        add    eax, 10h
        push   eax
        push   esi
        call   sub_4_11BC
        add    esp, 8
        mov    dword ptr [esi + 38h], eax
        test   eax, eax
        jne    SHORT loc_4_1E4D
        mov    eax, 2
        jmp    SHORT loc_4_1EA5
loc_4_1E4D:
        mov    dword ptr [esi + 3ch], 0
loc_4_1E54:
        mov    eax, dword ptr [edi]
        mov    ebx, dword ptr [esi + 1ch]
        sub    ebx, eax
        mov    ecx, 400h
        cmp    ebx, ecx
        mov    ebp, ebx
        mov    edx, dword ptr [esp + 18h]
        jb     SHORT loc_4_1E6C
        mov    ebp, ecx
loc_4_1E6C:
        push   ebp
        add    edx, 14h
        push   eax
        push   dword ptr [esi + 38h]
        push   10h
        push   edx
        push   esi
        call   sub_4_1EAC
        cmp    ebx, 400h
        ja     SHORT loc_4_1E9D
        mov    eax, dword ptr [edi]
        push   0fh
        add    eax, ebx
        push   eax
        mov    dword ptr [edi], eax
        push   dword ptr [esi + 38h]
        push   0
        push   0
        push   esi
        call   sub_4_1EAC
        jmp    SHORT loc_4_1EA3
loc_4_1E9D:
        add    dword ptr [edi], 400h
loc_4_1EA3:
        xor    eax, eax
loc_4_1EA5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_4_1EAC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
loc_4_1EBA:
        push   0
        push   0
        push   0ffh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1EC9:
        mov    byte ptr [ebp + edi - 0ch], al
        add    esp, 10h
        cmp    eax, 0ffh
        mov    ebx, eax
        je     SHORT loc_4_1F58
        shl    eax, 5
        cmp    byte ptr [eax + esi + 59h], 0
        je     SHORT loc_4_1EE6
        dec    dword ptr [esi + 18h]
loc_4_1EE6:
        push   80h
        mov    eax, ebx
        or     eax, 2a0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1EF9:
        push   0
        mov    eax, ebx
        or     al, 60h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F06:
        push   0
        mov    eax, ebx
        or     al, 40h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F13:
        push   0
        mov    eax, ebx
        or     al, 0c0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F20:
        push   0
        mov    eax, ebx
        or     al, 0e0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F2D:
        push   40000000h
        mov    eax, ebx
        or     al, 20h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F3D:
        push   40000000h
        push   ebx
        push   esi
        or     bh, 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F4C:
        push   6000000h
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F58:
        inc    edi
        cmp    edi, 0ah
        jb     NEAR PTR loc_4_1EBA
        mov    ecx, dword ptr [ebp + 18h]
        xor    edi, edi
        test   ecx, ecx
        jne    SHORT loc_4_1F9C
        mov    dword ptr [esi + 28h], edi
        mov    dword ptr [esi + 2ch], edi
        mov    ebx, 0eh
        mov    dword ptr [esi + 30h], edi
        push   dword ptr [ebp + 14h]
        push   236h
        push   esi
        mov    dword ptr [esi + 34h], edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F8A:
        push   0
        push   23ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F97:
        dec    ebx
        jne    SHORT loc_4_1F8A
        jmp    SHORT loc_4_1FAF
loc_4_1F9C:
        mov    eax, dword ptr [ebp + 14h]
        lea    ecx, [ecx + eax + 0eh]
        push   ecx
        push   236h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1FAF:
        mov    ax, word ptr [ebp + 10h]
        test   ax, ax
        jne    SHORT loc_4_1FFC
        push   0
        mov    edi, 23ah
        push   esi
        call   sub_4_20A9
        movsx  eax, ax
        push   eax
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1FD0:
        push   0
        push   esi
        call   sub_4_20A9
        movsx  eax, ax
        push   eax
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1FE3:
        mov    ebx, dword ptr [ebp + 1ch]
        test   ebx, ebx
        je     SHORT loc_4_203A
loc_4_1FEA:
        push   0
        push   23ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1FF7:
        dec    ebx
        jne    SHORT loc_4_1FEA
        jmp    SHORT loc_4_203A
loc_4_1FFC:
        cmp    ax, 10h
        jne    SHORT loc_4_203A
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 1ch]
        test   ebx, ebx
        je     SHORT loc_4_202D
loc_4_200C:
        movsx  ecx, word ptr [edi]
        mov    eax, edi
        push   ecx
        add    edi, 2
        push   esi
        call   sub_4_20A9
        movsx  ecx, ax
        push   ecx
        push   23ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_202A:
        dec    ebx
        jne    SHORT loc_4_200C
loc_4_202D:
        push   0
        push   23ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_203A:
        xor    edi, edi
loc_4_203C:
        movzx  ebx, byte ptr [ebp + edi - 0ch]
        cmp    ebx, 0ffh
        je     SHORT loc_4_209A
        push   0
        mov    eax, ebx
        or     ah, 2
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2057:
        push   1d8h
        mov    eax, ebx
        or     al, 0c0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2067:
        push   1e0h
        mov    eax, ebx
        or     al, 0e0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2077:
        push   48000000h
        mov    eax, ebx
        shl    ebx, 5
        or     al, 20h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_208A:
        lea    eax, [ebx + esi]
        mov    word ptr [eax + 42h], 0ffeh
        mov    byte ptr [eax + 59h], 1
        inc    dword ptr [esi + 18h]
loc_4_209A:
        inc    edi
        cmp    edi, 0ah
        jb     SHORT loc_4_203C
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
sub_4_20A9:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    edi, [edx + 2ch]
        mov    esi, dword ptr [esp + 18h]
        mov    ebx, dword ptr [edi]
        test   ebx, ebx
        jne    SHORT loc_4_20EA
        test   esi, esi
        jne    SHORT loc_4_20C8
        cmp    dword ptr [edx + 28h], 0
        je     SHORT loc_4_20D4
loc_4_20C8:
        cmp    dword ptr [edx + 30h], 0
        jne    SHORT loc_4_20EA
        cmp    dword ptr [edx + 34h], 0
        jne    SHORT loc_4_20EA
loc_4_20D4:
        mov    eax, dword ptr [edx + 30h]
        mov    ecx, dword ptr [edx + 28h]
        mov    dword ptr [edx + 34h], eax
        mov    dword ptr [edx + 30h], ebx
        xor    ax, ax
        mov    dword ptr [edi], ecx
        mov    dword ptr [edx + 28h], esi
        jmp    SHORT loc_4_2107
loc_4_20EA:
        mov    ebp, dword ptr [edx + 30h]
        mov    ecx, dword ptr [edx + 28h]
        mov    eax, ebx
        mov    dword ptr [edx + 34h], ebp
        mov    dword ptr [edx + 30h], ebx
        mov    dword ptr [edi], ecx
        imul   eax, eax, 6
        sub    eax, ebp
        mov    dword ptr [edx + 28h], esi
        sub    eax, ecx
        sar    eax, 3
loc_4_2107:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_4_210E:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        mov    edx, dword ptr [ebp + 0ch]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   edi
        add    edx, 14h
        mov    ecx, edx
        mov    dword ptr [ebx + 3ch], eax
        add    edx, 0ch
        mov    dword ptr [ebx + 38h], eax
        cmp    dword ptr [ecx], 46464952h
        je     SHORT loc_4_213F
        mov    eax, 2
        jmp    NEAR PTR loc_4_234E
loc_4_213F:
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4b424653h
        je     SHORT loc_4_215A
        cmp    eax, 6b626673h
        je     SHORT loc_4_215A
        mov    eax, 2
        jmp    NEAR PTR loc_4_234E
loc_4_215A:
        mov    eax, dword ptr [ecx + 4]
        add    eax, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, edx
        add    edx, 0ch
        cmp    dword ptr [eax], 5453494ch
        je     SHORT loc_4_217A
        mov    eax, 2
        jmp    NEAR PTR loc_4_234E
loc_4_217A:
        cmp    dword ptr [eax + 8], 4f464e49h
        je     SHORT loc_4_218D
        mov    eax, 2
        jmp    NEAR PTR loc_4_234E
loc_4_218D:
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0ch], eax
        sub    eax, 4
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 7ech
        jb     SHORT loc_4_21AA
        mov    eax, 2
        jmp    NEAR PTR loc_4_234E
loc_4_21AA:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_4_228E
loc_4_21B4:
        mov    ecx, dword ptr [edx]
        mov    eax, edx
        add    edx, 8
        cmp    ecx, 4d414e49h
        je     SHORT loc_4_21DB
        cmp    ecx, 6c696669h
        je     SHORT loc_4_21F2
        cmp    ecx, 6d6f7269h
        je     SHORT loc_4_221C
        add    eax, 4
        jmp    NEAR PTR loc_4_227E
loc_4_21DB:
        lea    edi, [ebx + 150ah]
        mov    esi, edx
        mov    ecx, 5
        rep movsd
        add    eax, 4
        jmp    NEAR PTR loc_4_227E
loc_4_21F2:
        add    eax, 4
        cmp    dword ptr [eax], 4
        jne    NEAR PTR loc_4_22A5
        mov    ecx, dword ptr [edx]
        add    edx, 4
        test   cx, cx
        je     NEAR PTR loc_4_22AF
        cmp    cx, 2
        ja     NEAR PTR loc_4_22AF
        mov    word ptr [ebx + 40h], cx
        jmp    SHORT loc_4_2280
loc_4_221C:
        add    eax, 4
        cmp    dword ptr [eax], 0
        je     SHORT loc_4_2280
        mov    esi, 0
        jbe    SHORT loc_4_225E
loc_4_222B:
        cmp    byte ptr [ebx + esi + 151eh], 0
        je     SHORT loc_4_2268
        mov    cl, byte ptr [edx + esi]
        cmp    cl, 61h
        jb     SHORT loc_4_224A
        cmp    cl, 7ah
        ja     SHORT loc_4_224A
        movzx  ecx, cl
        sub    ecx, 20h
        jmp    SHORT loc_4_224D
loc_4_224A:
        movzx  ecx, cl
loc_4_224D:
        movsx  edi, byte ptr [ebx + esi + 151eh]
        cmp    edi, ecx
        jne    SHORT loc_4_225E
        inc    esi
        cmp    esi, dword ptr [eax]
        jb     SHORT loc_4_222B
loc_4_225E:
        cmp    byte ptr [ebx + esi + 151eh], 0
        jne    SHORT loc_4_2277
loc_4_2268:
        cmp    byte ptr [edx + esi], 0
        jne    SHORT loc_4_2277
        mov    byte ptr [ebx + 1524h], 0
        jmp    SHORT loc_4_227E
loc_4_2277:
        mov    byte ptr [ebx + 1524h], 1
loc_4_227E:
        add    edx, dword ptr [eax]
loc_4_2280:
        mov    eax, dword ptr [eax]
        add    eax, 8
        sub    dword ptr [ebp - 4], eax
        jne    NEAR PTR loc_4_21B4
loc_4_228E:
        mov    esi, edx
        add    edx, 0ch
        cmp    dword ptr [esi], 5453494ch
        je     SHORT loc_4_22B9
        mov    eax, 2
        jmp    NEAR PTR loc_4_234E
loc_4_22A5:
        mov    eax, 2
        jmp    NEAR PTR loc_4_234E
loc_4_22AF:
        mov    eax, 2
        jmp    NEAR PTR loc_4_234E
loc_4_22B9:
        cmp    dword ptr [esi + 8], 61746473h
        je     SHORT loc_4_22CC
        mov    eax, 2
        jmp    NEAR PTR loc_4_234E
loc_4_22CC:
        mov    edi, edx
        mov    ecx, dword ptr [ebp + 0ch]
        add    edx, 8
        sub    edx, dword ptr [ebp + 0ch]
        sub    edx, 14h
        mov    dword ptr [ecx + 4], edx
        cmp    dword ptr [edi], 6d616e73h
        jne    SHORT loc_4_22FD
        mov    eax, dword ptr [edi + 4]
        add    esi, 4
        add    eax, edx
        add    eax, 8
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi]
        sub    eax, dword ptr [edi + 4]
        sub    eax, 14h
        jmp    SHORT loc_4_2305
loc_4_22FD:
        add    esi, 4
        mov    eax, dword ptr [esi]
        sub    eax, 0ch
loc_4_2305:
        test   eax, eax
        jge    SHORT loc_4_230B
        xor    eax, eax
loc_4_230B:
        mov    dword ptr [ebx + 24h], 0
        shr    eax, 1
        mov    dword ptr [ebx + 3ch], eax
        mov    dword ptr [ebx + 1ch], eax
        add    eax, 3ffh
        cdq
        and    edx, 3ffh
        add    eax, edx
        mov    edx, dword ptr [ebp + 0ch]
        sar    eax, 0ah
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [edx + 2], ax
        mov    eax, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [esi]
        lea    edx, [esi + eax + 28h]
        mov    eax, dword ptr [ebp - 8]
        sub    eax, edx
        mov    dword ptr [ecx + 8], edx
        mov    dword ptr [ecx + 0ch], eax
        mov    dword ptr [ebx + 20h], eax
        xor    eax, eax
loc_4_234E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_4_2357:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 38h]
        test   eax, eax
        je     SHORT loc_4_2374
        mov    ecx, dword ptr [esi + 1ch]
        add    ecx, 10h
        push   ecx
        push   eax
        push   esi
        call   sub_4_1531
        add    esp, 0ch
loc_4_2374:
        xor    eax, eax
        mov    dword ptr [esi + 38h], eax
        mov    dword ptr [esi + 3ch], eax
        pop    esi
        ret    4
sub_4_2380:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        mov    dword ptr [ebp - 10h], 240h
        mov    dword ptr [ebp - 0ch], 340h
        mov    dword ptr [ebp - 8], 260h
        mov    dword ptr [ebp - 4], 360h
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
loc_4_23A9:
        test   edi, 1
        jne    SHORT loc_4_23BC
        db  00Fh, 0B7h, 004h, 07Dh, 000h, 000h, 000h, 000h
        push   eax
        jmp    SHORT loc_4_23CB
loc_4_23BC:
        db  066h, 08Bh, 004h, 07Dh, 000h, 000h, 000h, 000h
        or     ah, 80h
        movzx  ecx, ax
        push   ecx
loc_4_23CB:
        mov    eax, edi
        mov    ecx, edi
        and    al, 0e7h
        and    ecx, 1fh
        shr    eax, 3
        inc    edi
        mov    eax, dword ptr [ebp + eax - 10h]
        or     eax, ecx
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_23E5:
        cmp    edi, 80h
        jb     SHORT loc_4_23A9
        xor    edi, edi
        push   edi
        push   229h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_23FB:
        push   83h
        push   22ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_240B:
        push   8000h
        push   22dh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_241B:
        push   edi
        push   22eh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2427:
        db  00Fh, 0B7h, 004h, 07Dh, 000h, 000h, 000h, 000h
        push   eax
        mov    ecx, edi
        mov    eax, edi
        and    ecx, 1fh
        and    al, 0e7h
        inc    edi
        shr    eax, 3
        mov    eax, dword ptr [ebp + eax - 10h]
        or     eax, ecx
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_244A:
        cmp    edi, 80h
        jb     SHORT loc_4_2427
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_4_2458:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], -1
        jne    SHORT loc_4_246B
        mov    dword ptr [ebp + 0ch], 5
loc_4_246B:
        mov    ebx, dword ptr [ebp + 10h]
        cmp    ebx, -1
        jne    SHORT loc_4_248E
        mov    ebx, 5
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 151eh], 4d474d31h
        jne    SHORT loc_4_2491
        mov    ebx, 9
        jmp    SHORT loc_4_2491
loc_4_248E:
        mov    esi, dword ptr [ebp + 8]
loc_4_2491:
        mov    al, byte ptr [ebp + 0ch]
        mov    byte ptr [esi + 17h], bl
        mov    byte ptr [esi + 16h], al
        mov    eax, dword ptr [ebp + 0ch]
        imul   eax, eax, 6
        db  00Fh, 0B7h, 088h, 000h, 000h, 000h, 000h
        push   ecx
        push   361h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_24B3:
        mov    eax, dword ptr [ebp + 0ch]
        imul   eax, eax, 6
        db  00Fh, 0B7h, 088h, 002h, 000h, 000h, 000h
        push   ecx
        push   371h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_24CC:
        imul   ebx, ebx, 12h
        db  00Fh, 0B7h, 083h, 000h, 000h, 000h, 000h
        push   eax
        push   271h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_24E2:
        db  00Fh, 0B7h, 08Bh, 002h, 000h, 000h, 000h
        push   ecx
        push   273h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_24F5:
        db  00Fh, 0B7h, 08Bh, 004h, 000h, 000h, 000h
        push   ecx
        push   27bh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2508:
        db  00Fh, 0B7h, 08Bh, 006h, 000h, 000h, 000h
        push   ecx
        push   367h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_251B:
        db  00Fh, 0B7h, 08Bh, 008h, 000h, 000h, 000h
        push   ecx
        push   36bh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_252E:
        db  00Fh, 0B7h, 08Bh, 00Ah, 000h, 000h, 000h
        push   ecx
        push   36dh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2541:
        db  00Fh, 0B7h, 08Bh, 00Ch, 000h, 000h, 000h
        push   ecx
        push   377h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2554:
        db  00Fh, 0B7h, 08Bh, 00Eh, 000h, 000h, 000h
        push   ecx
        push   379h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2567:
        db  00Fh, 0B7h, 08Bh, 010h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        imul   eax, eax, 6
        db  00Fh, 0B7h, 090h, 004h, 000h, 000h, 000h
        lea    eax, [ecx + edx + 262h]
        lea    edi, [ecx + edx]
        add    edi, 8362h
        push   eax
        push   375h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2597:
        push   edi
        push   37dh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_25A3:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_25AA:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        cmp    word ptr [esi + 12h], 0
        jne    NEAR PTR loc_4_2647
        mov    ebx, dword ptr [esp + 10h]
        cmp    ebx, -1
        jne    SHORT loc_4_25C9
        mov    ebx, 2
loc_4_25C9:
        mov    byte ptr [esi + 15h], bl
        imul   ebx, ebx, 0eh
        db  00Fh, 0B7h, 083h, 000h, 000h, 000h, 000h
        push   eax
        push   269h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_25E2:
        db  00Fh, 0B7h, 08Bh, 002h, 000h, 000h, 000h
        push   ecx
        push   26ch
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_25F5:
        db  00Fh, 0B7h, 08Bh, 004h, 000h, 000h, 000h
        push   ecx
        push   363h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2608:
        db  0FFh, 0B3h, 006h, 000h, 000h, 000h
        push   229h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2619:
        db  0FFh, 0B3h, 00Ah, 000h, 000h, 000h
        push   22ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_262A:
        push   8000h
        push   22dh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_263A:
        push   0
        push   22eh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2647:
        xor    eax, eax
        pop    esi
        pop    ebx
        ret
sub_4_264C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    word ptr [edi + 12h], 0
        jne    SHORT loc_4_26CA
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    ebx, -1
        jne    SHORT loc_4_266C
        mov    ebx, 4
loc_4_266C:
        mov    word ptr [ebp - 8], 240h
        xor    esi, esi
        mov    byte ptr [edi + 14h], bl
        mov    word ptr [ebp - 6], 340h
        imul   ebx, ebx, 1ch
        mov    word ptr [ebp - 4], 260h
        mov    word ptr [ebp - 2], 360h
loc_4_268C:
        db  08Ah, 086h, 000h, 000h, 000h, 000h
        lea    ecx, [ebx + esi]
        db  00Fh, 0B7h, 014h, 04Dh, 000h, 000h, 000h, 000h
        push   edx
        mov    cl, al
        shr    cl, 4
        inc    esi
        movzx  ecx, cl
        and    cl, 0feh
        mov    dx, word ptr [ebp + ecx - 8]
        xor    cx, cx
        mov    cl, al
        and    cx, 1fh
        or     dx, cx
        movzx  eax, dx
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_26C5:
        cmp    esi, 1ch
        jl     SHORT loc_4_268C
loc_4_26CA:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_26D3:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        test   ecx, ecx
        jne    SHORT loc_4_26E0
        xor    eax, eax
        jmp    SHORT loc_4_272D
loc_4_26E0:
        imul   ecx, ecx, 2f8eh
        mov    edx, 1fh
loc_4_26EB:
        test   ecx, 80000000h
        jne    SHORT loc_4_26FC
        add    ecx, ecx
        dec    edx
        test   edx, edx
        jg     SHORT loc_4_26EB
        jmp    SHORT loc_4_272D
loc_4_26FC:
        mov    esi, ecx
        and    ecx, 0fe0000h
        shr    ecx, 11h
        and    esi, 7f000000h
        mov    eax, esi
        sub    edx, 0fh
        shr    eax, 18h
        db  00Fh, 0BEh, 080h, 000h, 000h, 000h, 000h
        imul   eax, ecx
        shr    esi, 16h
        shl    edx, 14h
        db  003h, 086h, 000h, 000h, 000h, 000h
        add    eax, edx
loc_4_272D:
        pop    esi
        ret
sub_4_272F:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        test   ecx, ecx
        je     SHORT loc_4_2752
        xor    eax, eax
        db  0BAh, 000h, 000h, 000h, 000h
loc_4_273F:
        movzx  esi, word ptr [edx]
        cmp    esi, ecx
        jb     SHORT loc_4_2756
        add    edx, 2
        inc    eax
        db  081h, 0FAh, 000h, 001h, 000h, 000h
        jb     SHORT loc_4_273F
loc_4_2752:
        mov    ax, 7fh
loc_4_2756:
        pop    esi
        ret
sub_4_2758:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    eax, 5
        push   esi
        cmp    eax, 2bh
        ja     SHORT loc_4_2776
        db  00Fh, 0B6h, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_4_2776:
        mov    eax, dword ptr [ebp + 0ch]
        jmp    NEAR PTR loc_4_2898
loc_4_277E:
        cmp    dword ptr [ebp + 0ch], 7fh
        jle    SHORT loc_4_278E
        mov    eax, 7fh
        jmp    NEAR PTR loc_4_2898
loc_4_278E:
        cmp    dword ptr [ebp + 0ch], -7fh
        mov    eax, 0ffffff81h
        jl     NEAR PTR loc_4_2898
        mov    eax, dword ptr [ebp + 0ch]
        jmp    NEAR PTR loc_4_2898
loc_4_27A5:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, eax
        jmp    NEAR PTR loc_4_2898
loc_4_27AF:
        mov    eax, dword ptr [ebp + 0ch]
        cdq
        and    edx, 7
        add    eax, edx
        sar    eax, 3
        jmp    NEAR PTR loc_4_2898
loc_4_27C0:
        mov    esi, 8000h
        mov    ecx, 2d5h
        movsx  eax, word ptr [ebp + 10h]
        imul   eax, dword ptr [ebp + 0ch]
        imul   eax, eax, 3e8h
        cdq
        idiv   ecx
        movsx  edx, ax
        sub    esi, edx
        mov    eax, esi
        jmp    NEAR PTR loc_4_2898
loc_4_27E7:
        movzx  eax, word ptr [ebp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_27F1:
        movsx  eax, ax
        add    esp, 4
        jmp    NEAR PTR loc_4_2898
loc_4_27FC:
        mov    ecx, 0ffffffa4h
        mov    eax, dword ptr [ebp + 0ch]
        cdq
        idiv   ecx
        lea    ecx, [eax + 7fh]
        test   ecx, ecx
        jge    SHORT loc_4_2815
        xor    eax, eax
        jmp    NEAR PTR loc_4_2898
loc_4_2815:
        mov    eax, 7fh
        cmp    ecx, 7fh
        jg     SHORT loc_4_2898
        mov    eax, ecx
        jmp    SHORT loc_4_2898
loc_4_2823:
        movsx  eax, word ptr [ebp + 10h]
        imul   eax, dword ptr [ebp + 0ch]
        movzx  ecx, ax
        push   ecx
        call   sub_4_272F
        movsx  eax, ax
        add    esp, 4
        jmp    SHORT loc_4_2898
loc_4_283C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, 4bh
        imul   eax, eax, 64h
        cdq
        idiv   ecx
        mov    ecx, eax
        test   ecx, ecx
        jge    SHORT loc_4_2854
        xor    eax, eax
        jmp    SHORT loc_4_2898
loc_4_2854:
        mov    eax, 7fh
        cmp    ecx, 7fh
        jg     SHORT loc_4_2898
        mov    eax, ecx
        jmp    SHORT loc_4_2898
loc_4_2862:
        movsx  eax, word ptr [ebp + 10h]
        imul   eax, dword ptr [ebp + 0ch]
        movzx  ecx, ax
        push   ecx
        call   sub_4_272F
        movsx  eax, ax
        add    esp, 4
        jmp    SHORT loc_4_2898
loc_4_287B:
        movsx  eax, word ptr [ebp + 10h]
        imul   eax, dword ptr [ebp + 0ch]
        jmp    SHORT loc_4_2898
loc_4_2885:
        mov    eax, 7fh
        mov    ecx, 0ah
        sub    eax, dword ptr [ebp + 0ch]
        imul   eax, eax, 4bh
        cdq
        idiv   ecx
loc_4_2898:
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h
        db  002h, 003h, 003h, 016h, 004h, 005h, 016h, 016h, 016h, 016h, 016h, 016h, 006h, 007h, 008h, 009h
        db  00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh, 016h, 016h, 010h, 011h, 016h, 012h, 013h, 014h, 016h, 016h
        db  016h, 016h, 016h, 016h, 016h, 016h, 016h, 015h
sub_4_2923:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    eax, 5
        cmp    eax, 34h
        ja     SHORT loc_4_2940
        db  00Fh, 0B6h, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_4_2940:
        mov    eax, dword ptr [ebp + 0ch]
        jmp    NEAR PTR loc_4_29EA
loc_4_2948:
        mov    eax, dword ptr [ebp + 0ch]
        imul   eax, eax, 1b4fh
        add    eax, 8000h
        cdq
        and    edx, 0ffffh
        add    eax, edx
        sar    eax, 10h
        jmp    NEAR PTR loc_4_29EA
loc_4_2967:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, 1dh
        add    eax, 0fh
        cdq
        idiv   ecx
        sub    eax, 99h
        jmp    SHORT loc_4_29EA
loc_4_297C:
        mov    ecx, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        cdq
        idiv   ecx
        jmp    SHORT loc_4_29EA
loc_4_2989:
        mov    eax, dword ptr [ebp + 0ch]
        imul   eax, eax, 919h
        add    eax, 8000h
        cdq
        and    edx, 0ffffh
        add    eax, edx
        sar    eax, 10h
        jmp    SHORT loc_4_29EA
loc_4_29A5:
        mov    eax, dword ptr [ebp + 0ch]
        imul   eax, eax, 48dh
        add    eax, 8000h
        cdq
        and    edx, 0ffffh
        add    eax, edx
        sar    eax, 10h
        jmp    SHORT loc_4_29EA
loc_4_29C1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, 78h
        shl    eax, 7
        cdq
        idiv   ecx
        jmp    SHORT loc_4_29EA
loc_4_29D1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, 3e8h
        imul   eax, eax, -7fh
        cdq
        idiv   ecx
        add    eax, 7fh
        jmp    SHORT loc_4_29EA
loc_4_29E4:
        mov    eax, dword ptr [ebp + 0ch]
        imul   eax, eax, 64h
loc_4_29EA:
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 002h, 003h, 004h, 01Ch, 005h, 01Ch, 006h, 01Ch
        db  007h, 01Ch, 008h, 01Ch, 009h, 01Ch, 00Ah, 01Ch, 00Bh, 01Ch, 00Ch, 01Ch, 00Dh, 01Ch, 00Eh, 01Ch
        db  00Fh, 01Ch, 010h, 01Ch, 011h, 01Ch, 012h, 01Ch, 013h, 01Ch, 014h, 01Ch, 015h, 01Ch, 016h, 01Ch
        db  017h, 01Ch, 018h, 01Ch, 019h, 01Ch, 01Ah, 01Ch, 01Bh
sub_4_2A95:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        lea    eax, [esi - 5]
        cmp    eax, 35h
        ja     SHORT loc_4_2AB1
        db  00Fh, 0B6h, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_4_2AB1:
        mov    eax, dword ptr [esp + 10h]
        jmp    SHORT loc_4_2AD5
loc_4_2AB7:
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_4_2923
        add    esp, 8
        mov    edi, eax
        push   0
        push   esi
        call   sub_4_2923
        add    esp, 8
        sub    edi, eax
        mov    eax, edi
loc_4_2AD5:
        pop    edi
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 001h, 018h, 002h, 018h
        db  003h, 018h, 004h, 018h, 005h, 018h, 006h, 018h, 007h, 018h, 008h, 018h, 009h, 018h, 00Ah, 018h
        db  00Bh, 018h, 00Ch, 018h, 00Dh, 018h, 00Eh, 018h, 00Fh, 018h, 010h, 018h, 011h, 012h, 012h, 018h
        db  013h, 018h, 014h, 014h, 018h, 015h, 018h, 016h, 018h, 017h
sub_4_2B72:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    eax, 15h
        push   ebx
        cmp    eax, 25h
        push   esi
        push   edi
        ja     SHORT loc_4_2B92
        db  00Fh, 0B6h, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_4_2B92:
        mov    eax, dword ptr [ebp + 0ch]
        jmp    NEAR PTR loc_4_2D5C
loc_4_2B9A:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 0ffffd120h
        jg     SHORT loc_4_2BAE
        mov    eax, 8000h
        jmp    NEAR PTR loc_4_2D5C
loc_4_2BAE:
        add    eax, 30e4h
        mov    ecx, 4b0h
        shl    eax, 10h
        mov    edi, 8000h
        cdq
        idiv   ecx
        mov    ebx, eax
        mov    esi, 0ffffh
        and    ebx, esi
        mov    cl, 10h
        cdq
        add    ebx, 10000h
        and    edx, esi
        add    eax, edx
        sar    eax, cl
        sub    cl, al
        sar    ebx, cl
        sub    edi, ebx
        mov    eax, edi
        test   eax, eax
        jge    NEAR PTR loc_4_2D5C
        xor    eax, eax
        jmp    NEAR PTR loc_4_2D5C
loc_4_2BF2:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 0ffffc180h
        jg     SHORT loc_4_2C03
        xor    eax, eax
        jmp    NEAR PTR loc_4_2D5C
loc_4_2C03:
        add    eax, 23a6h
        mov    ecx, 4b0h
        shl    eax, 10h
        mov    esi, 0ffffh
        cdq
        idiv   ecx
        mov    edi, eax
        mov    cl, 10h
        cdq
        and    edi, esi
        and    edx, esi
        add    edi, 10000h
        add    eax, edx
        sar    eax, cl
        sub    cl, al
        sar    edi, cl
        mov    eax, edi
        mov    ecx, 0ffh
        cmp    eax, ecx
        jle    NEAR PTR loc_4_2D5C
        mov    eax, ecx
        jmp    NEAR PTR loc_4_2D5C
loc_4_2C45:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 0fffffda8h
        jle    SHORT loc_4_2C99
        cmp    eax, 10cch
        jl     SHORT loc_4_2C60
        mov    eax, 1
        jmp    NEAR PTR loc_4_2D5C
loc_4_2C60:
        add    eax, 1f4h
        mov    ecx, 96h
        cdq
        idiv   ecx
        cdq
        mov    esi, eax
        and    edx, 7
        not    esi
        and    esi, 7
        add    eax, edx
        sar    eax, 3
        or     esi, 8
        mov    cl, al
        sar    esi, cl
        cmp    esi, 1
        mov    eax, esi
        jge    NEAR PTR loc_4_2D5C
        mov    eax, 1
        jmp    NEAR PTR loc_4_2D5C
loc_4_2C99:
        mov    ecx, 25h
        mov    esi, 4bh
        sub    ecx, eax
        mov    eax, ecx
        mov    ecx, 80h
        cdq
        idiv   esi
        add    eax, 8
        cmp    eax, ecx
        jle    NEAR PTR loc_4_2D5C
        mov    eax, ecx
        jmp    NEAR PTR loc_4_2D5C
loc_4_2CC1:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 0ffffeb08h
        jl     NEAR PTR loc_4_2D57
        add    eax, 1022h
        mov    ecx, 4b0h
        shl    eax, 10h
        mov    edi, 7fh
        cdq
        idiv   ecx
        mov    ebx, eax
        mov    esi, 0ffffh
        and    ebx, esi
        mov    cl, 10h
        cdq
        add    ebx, 10000h
        and    edx, esi
        add    eax, edx
        sar    eax, cl
        sub    cl, al
        sar    ebx, cl
        sub    edi, ebx
        mov    eax, edi
        test   eax, eax
        jge    SHORT loc_4_2D5C
        xor    eax, eax
        jmp    SHORT loc_4_2D5C
loc_4_2D0C:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 708h
        jle    SHORT loc_4_2D3E
        mov    ecx, 96h
        sub    eax, 708h
        cdq
        idiv   ecx
        cdq
        mov    esi, eax
        and    edx, 7
        not    esi
        and    esi, 7
        add    eax, edx
        sar    eax, 3
        or     esi, 8
        mov    cl, al
        sar    esi, cl
        mov    eax, esi
        jmp    SHORT loc_4_2D5C
loc_4_2D3E:
        mov    ecx, 25h
        mov    esi, 4bh
        sub    ecx, eax
        mov    eax, ecx
        cdq
        idiv   esi
        add    eax, 27h
        cmp    eax, 7fh
        jle    SHORT loc_4_2D5C
loc_4_2D57:
        mov    eax, 7fh
loc_4_2D5C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h
        db  019h, 008h, 019h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 019h, 00Eh, 019h, 00Fh, 019h, 010h, 019h, 011h
        db  019h, 012h, 019h, 013h, 019h, 014h, 019h, 015h, 019h, 016h, 019h, 017h, 019h, 018h
sub_4_2DEF:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        add    edi, 662h
        mov    ebx, dword ptr [ebp + 10h]
loc_4_2E06:
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     SHORT loc_4_2E32
        mov    eax, dword ptr [eax]
        xor    esi, esi
        cmp    word ptr [eax], 0ffffh
        je     SHORT loc_4_2E32
loc_4_2E17:
        cmp    word ptr [eax], 0
        je     SHORT loc_4_2E27
        cmp    byte ptr [eax + 3], dl
        jne    SHORT loc_4_2E27
        cmp    byte ptr [eax + 2], bl
        je     SHORT loc_4_2E45
loc_4_2E27:
        add    eax, 1ah
        inc    esi
        cmp    word ptr [eax], 0ffffh
        jne    SHORT loc_4_2E17
loc_4_2E32:
        add    edi, 4
        inc    ecx
        cmp    ecx, 120h
        jb     SHORT loc_4_2E06
        xor    eax, eax
loc_4_2E40:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_2E45:
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebp + 18h]
        mov    eax, 1
        mov    dword ptr [ecx], esi
        jmp    SHORT loc_4_2E40
sub_4_2E56:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [esi + edi*4 + 662h]
        test   ebx, ebx
        je     SHORT loc_4_2EA4
        mov    word ptr [ebp - 18h], di
        lea    eax, [ebp - 18h]
        push   eax
        push   esi
        add    esi, 456h
        call   sub_4_164D
        push   ebx
        call   sub_4_0D91
        add    esp, 4
        mov    eax, 11h
loc_4_2E92:
        movzx  ecx, word ptr [esi]
        cmp    ecx, edi
        jne    SHORT loc_4_2E9E
        mov    word ptr [esi], 0ffffh
loc_4_2E9E:
        add    esi, 20h
        dec    eax
        jne    SHORT loc_4_2E92
loc_4_2EA4:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_2EAD:
        mov    ecx, dword ptr [esp + 4]
        push   ebx
        push   esi
        add    ecx, 662h
        xor    esi, esi
        mov    edx, dword ptr [esp + 10h]
loc_4_2EBF:
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     SHORT loc_4_2EE5
        mov    eax, dword ptr [eax]
        cmp    word ptr [eax], 0ffffh
        je     SHORT loc_4_2EE5
loc_4_2ECE:
        cmp    word ptr [eax], 0
        je     SHORT loc_4_2EDB
        mov    bl, byte ptr [edx]
        cmp    byte ptr [eax + 3], bl
        je     SHORT loc_4_2EFE
loc_4_2EDB:
        add    eax, 1ah
        cmp    word ptr [eax], 0ffffh
        jne    SHORT loc_4_2ECE
loc_4_2EE5:
        add    ecx, 4
        inc    esi
        cmp    esi, 120h
        jb     SHORT loc_4_2EBF
        mov    word ptr [edx], 0ffffh
        mov    eax, 8005h
loc_4_2EFB:
        pop    esi
        pop    ebx
        ret
loc_4_2EFE:
        cmp    esi, 80h
        jae    SHORT loc_4_2F09
        mov    word ptr [edx], si
loc_4_2F09:
        xor    eax, eax
        jmp    SHORT loc_4_2EFB
sub_4_2F0D:
        push   ebp
        mov    ebp, esp
        push   dword ptr [esp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_2E56
        add    esp, 8
        push   0ffffh
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_30D1
        add    esp, 0ch
        xor    eax, eax
        pop    ebp
        ret
sub_4_2F36:
        mov    eax, dword ptr [esp + 4]
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [edi]
        mov    esi, dword ptr [eax + ecx*4 + 662h]
        test   esi, esi
        je     SHORT loc_4_2F5E
        add    esi, 2ch
        add    edi, 8
        mov    ecx, 5
        rep movsd
        xor    eax, eax
        jmp    SHORT loc_4_2F67
loc_4_2F5E:
        mov    byte ptr [edi + 8], 0
        mov    eax, 8005h
loc_4_2F67:
        pop    edi
        pop    esi
        ret
sub_4_2F6A:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 4
        cmp    dword ptr [eax + 14h], 72646870h
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_4_2F8A
        mov    eax, 1
        jmp    NEAR PTR loc_4_30CA
loc_4_2F8A:
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 26h
        add    edi, 1ch
        mov    eax, dword ptr [ebp + 0ch]
        sub    edx, edx
        mov    eax, dword ptr [eax + 18h]
        div    ecx
        lea    esi, [eax - 1]
        test   esi, esi
        je     SHORT loc_4_2FE2
        mov    ebx, 80h
loc_4_2FAB:
        mov    ax, word ptr [edi + 16h]
        test   ax, ax
        je     SHORT loc_4_2FDC
        cmp    ax, bx
        je     SHORT loc_4_2FDC
        movzx  ecx, ax
        mov    eax, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [eax]
        lea    edx, [eax + ecx]
        cmp    edx, ebx
        je     SHORT loc_4_2FDC
        movzx  edx, word ptr [edi + 14h]
        push   edx
        add    eax, ecx
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_30D1
        add    esp, 0ch
loc_4_2FDC:
        add    edi, 26h
        dec    esi
        jne    SHORT loc_4_2FAB
loc_4_2FE2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        add    eax, 800h
        push   eax
        call   sub_4_0D74
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_4_3007
        mov    eax, 8003h
        jmp    NEAR PTR loc_4_30CA
loc_4_3007:
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_16AF
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_4_302D
        push   dword ptr [ebp - 4]
        call   sub_4_0D91
        add    esp, 4
        mov    eax, esi
        jmp    NEAR PTR loc_4_30CA
loc_4_302D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        add    eax, 80h
        push   eax
        call   sub_4_0D74
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_4_304E
        mov    eax, 8003h
        jmp    SHORT loc_4_30CA
loc_4_304E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edi, ebx
        mov    esi, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   dword ptr [ebp - 4]
        call   sub_4_0D91
        add    esp, 4
        mov    ecx, dword ptr [ebp + 0ch]
        movzx  edi, word ptr [ecx]
        xor    esi, esi
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + edi*4 + 662h], esi
        mov    edx, ebx
        sub    edx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 40h], esi
        add    dword ptr [ebx], edx
        add    dword ptr [ebx + 4], edx
        add    dword ptr [ebx + 8], edx
        add    dword ptr [ebx + 0ch], edx
        add    dword ptr [ebx + 10h], edx
        add    dword ptr [ebx + 14h], edx
        add    dword ptr [ebx + 18h], edx
        mov    eax, dword ptr [ebx + 1ch]
        test   eax, eax
        je     SHORT loc_4_30AC
        add    eax, edx
        mov    dword ptr [ebx + 1ch], eax
loc_4_30AC:
        mov    eax, dword ptr [ebx + 20h]
        test   eax, eax
        je     SHORT loc_4_30B8
        add    eax, edx
        mov    dword ptr [ebx + 20h], eax
loc_4_30B8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        movzx  esi, word ptr [eax]
        xor    eax, eax
        mov    dword ptr [edx + esi*4 + 662h], ebx
loc_4_30CA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_30D1:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        add    eax, 662h
        sub    esp, 38h
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 20h], 120h
        push   ebx
        push   esi
        push   edi
loc_4_30ED:
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax]
        test   esi, esi
        je     NEAR PTR loc_4_331A
        mov    dword ptr [ebp - 10h], 1
        mov    edi, dword ptr [esi]
        cmp    word ptr [edi], 0ffffh
        je     NEAR PTR loc_4_32FD
loc_4_310E:
        cmp    word ptr [edi], 0
        je     NEAR PTR loc_4_32C6
        mov    al, byte ptr [ebp + 0ch]
        cmp    byte ptr [edi + 3], al
        je     SHORT loc_4_312C
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_4_32C6
loc_4_312C:
        cmp    dword ptr [ebp + 10h], 0ffffh
        je     SHORT loc_4_3149
        mov    al, byte ptr [ebp + 10h]
        cmp    byte ptr [edi + 2], al
        je     SHORT loc_4_3149
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_4_32C6
loc_4_3149:
        cmp    dword ptr [ebp + 10h], 80h
        jne    SHORT loc_4_315E
        mov    word ptr [esi + 24h], 0ffffh
        mov    word ptr [esi + 26h], 0ffffh
loc_4_315E:
        cmp    dword ptr [esi + 40h], 0
        jne    NEAR PTR loc_4_32D6
        movzx  eax, word ptr [edi + 4]
        mov    ecx, dword ptr [esi + 4]
        movzx  ebx, word ptr [edi + 1eh]
        movzx  edx, word ptr [ecx + eax*4]
        movzx  eax, word ptr [ecx + ebx*4]
        mov    dword ptr [ebp - 0ch], edx
        mov    dword ptr [ebp - 1ch], eax
        cmp    eax, edx
        jbe    NEAR PTR loc_4_32C1
loc_4_3189:
        mov    eax, dword ptr [ebp - 0ch]
        imul   eax, eax, 3
        add    eax, dword ptr [esi + 8]
        cmp    byte ptr [eax], 29h
        jne    NEAR PTR loc_4_32B2
        movsx  edx, word ptr [eax + 1]
        add    edx, edx
        mov    ecx, dword ptr [esi + 10h]
        add    edx, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 4], 0
        movzx  eax, word ptr [edx]
        movzx  edx, word ptr [edx + 2]
        movzx  ebx, word ptr [ecx + eax*4]
        movzx  eax, word ptr [ecx + edx*4]
        mov    dword ptr [ebp - 2ch], ebx
        mov    dword ptr [ebp - 38h], eax
loc_4_31C3:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_31DA
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + eax*4 - 2ch]
        cmp    dword ptr [ebp + eax*4 - 38h], ecx
        jbe    NEAR PTR loc_4_32B2
loc_4_31DA:
        mov    edx, dword ptr [ebp - 4]
        shl    edx, 2
        lea    ecx, [ebp + edx - 2ch]
        mov    eax, dword ptr [ecx]
        cmp    dword ptr [ebp + edx - 38h], eax
        jne    SHORT loc_4_31F1
        dec    dword ptr [ebp - 4]
        jmp    SHORT loc_4_31C3
loc_4_31F1:
        imul   eax, eax, 3
        add    eax, dword ptr [esi + 14h]
        cmp    byte ptr [eax], 29h
        jne    SHORT loc_4_3237
        movsx  ebx, word ptr [eax + 1]
        mov    eax, dword ptr [esi + 0ch]
        inc    dword ptr [ebp - 4]
        movzx  ebx, word ptr [eax + ebx*2]
        mov    eax, dword ptr [esi + 10h]
        movzx  ebx, word ptr [eax + ebx*4]
        mov    dword ptr [ebp + edx - 28h], ebx
        mov    ebx, dword ptr [esi + 14h]
        mov    eax, dword ptr [ecx]
        imul   eax, eax, 3
        movsx  ebx, word ptr [eax + ebx + 1]
        mov    eax, dword ptr [esi + 0ch]
        inc    dword ptr [ecx]
        movzx  ebx, word ptr [eax + ebx*2 + 2]
        mov    eax, dword ptr [esi + 10h]
        movzx  ebx, word ptr [eax + ebx*4]
        mov    dword ptr [ebp + edx - 34h], ebx
loc_4_3237:
        mov    eax, dword ptr [ebp - 4]
        lea    ecx, [ebp + eax*4 - 2ch]
        mov    dword ptr [ebp - 14h], ecx
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 18h], eax
        imul   eax, eax, 3
        add    eax, dword ptr [esi + 14h]
        cmp    byte ptr [eax], 35h
        je     SHORT loc_4_325C
        mov    eax, dword ptr [ebp - 18h]
        inc    eax
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_4_31C3
loc_4_325C:
        movsx  ebx, word ptr [eax + 1]
        mov    eax, dword ptr [esi + 18h]
        imul   ebx, ebx, 24h
        lea    eax, [eax + ebx + 1ah]
        mov    cx, word ptr [eax]
        test   cx, cx
        je     SHORT loc_4_32A4
        dec    cx
        mov    word ptr [eax], cx
        mov    eax, dword ptr [esi + 18h]
        add    eax, ebx
        cmp    word ptr [eax + 1ah], 0
        jne    SHORT loc_4_32A4
        mov    ecx, dword ptr [eax + 1ch]
        test   ecx, ecx
        je     SHORT loc_4_32A4
        push   dword ptr [eax + 20h]
        push   ecx
        push   dword ptr [ebp + 8]
        call   sub_4_1531
        add    esp, 0ch
        mov    ecx, dword ptr [esi + 18h]
        mov    dword ptr [ecx + ebx + 20h], 0
loc_4_32A4:
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 14h]
        inc    eax
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_4_31C3
loc_4_32B2:
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 1ch], eax
        ja     NEAR PTR loc_4_3189
loc_4_32C1:
        mov    word ptr [edi], 0
loc_4_32C6:
        add    edi, 1ah
        cmp    word ptr [edi], 0ffffh
        jne    NEAR PTR loc_4_310E
        jmp    SHORT loc_4_32FD
loc_4_32D6:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esi + 44h]
        add    eax, 10h
        push   eax
        push   dword ptr [esi + 40h]
        push   dword ptr [ebp + 8]
        call   sub_4_1531
        add    esp, 0ch
        push   esi
        call   sub_4_0D91
        add    esp, 4
loc_4_32FD:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_4_331A
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        je     SHORT loc_4_331A
        push   esi
        mov    dword ptr [eax], 0
        call   sub_4_0D91
        add    esp, 4
loc_4_331A:
        add    dword ptr [ebp - 8], 4
        dec    dword ptr [ebp - 20h]
        jne    NEAR PTR loc_4_30ED
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_3330:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        lea    ecx, [ebp - 8]
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   ecx
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [edi + 4]
        push   dword ptr [edi]
        push   esi
        call   sub_4_2DEF
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_4_337C
        mov    eax, dword ptr [ebp - 8]
        add    edi, 8
        mov    ecx, dword ptr [esi + eax*4 + 662h]
        mov    edx, dword ptr [ecx]
        mov    ecx, dword ptr [ebp - 4]
        imul   ecx, ecx, 1ah
        lea    esi, [edx + ecx + 6]
        mov    ecx, 5
        rep movsd
        xor    eax, eax
        jmp    SHORT loc_4_3385
loc_4_337C:
        mov    byte ptr [edi + 8], 0
        mov    eax, 8005h
loc_4_3385:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_4_338B:
        mov    edx, 81h
        mov    ecx, dword ptr [esp + 4]
        lea    eax, [ecx + 866h]
loc_4_339A:
        cmp    dword ptr [eax], 0
        je     SHORT loc_4_33B1
        add    eax, 4
        inc    edx
        cmp    edx, 120h
        jb     SHORT loc_4_339A
        mov    eax, 4
loc_4_33B0:
        ret
loc_4_33B1:
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax], dx
        push   eax
        push   ecx
        call   sub_4_210E
        jmp    SHORT loc_4_33B0
sub_4_33C1:
        mov    edx, dword ptr [esp + 0ch]
        push   ebx
        mov    ecx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        lea    esi, [edx + 10h]
        push   ebp
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        je     SHORT loc_4_3433
        mov    edi, dword ptr [esp + 14h]
loc_4_33DB:
        lea    ebx, [edi + 1ch]
        mov    eax, dword ptr [ebx]
        test   eax, 80000000h
        je     SHORT loc_4_3429
        cmp    dword ptr [edi + 20h], 0
        je     SHORT loc_4_3429
        sub    dword ptr [edi], eax
        mov    eax, dword ptr [ebx]
        sub    dword ptr [edi + 8], eax
        mov    eax, dword ptr [ebx]
        sub    dword ptr [edi + 0ch], eax
        mov    eax, dword ptr [ebx]
        sub    dword ptr [edi + 4], eax
        add    dword ptr [edi], esi
        add    dword ptr [edi + 8], esi
        add    dword ptr [edi + 0ch], esi
        add    dword ptr [edi + 4], esi
        mov    eax, dword ptr [edi + 20h]
        test   eax, eax
        je     SHORT loc_4_3423
        mov    ebp, edx
        sub    ebp, esi
        cmp    ebp, -10h
        jne    SHORT loc_4_3423
        add    eax, 10h
        mov    dword ptr [edi + 20h], eax
        mov    dword ptr [ebx], edx
        jmp    SHORT loc_4_3425
loc_4_3423:
        mov    dword ptr [ebx], esi
loc_4_3425:
        mov    esi, dword ptr [edi + 4]
        inc    esi
loc_4_3429:
        add    edi, 24h
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        jne    SHORT loc_4_33DB
loc_4_3433:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_4_343A:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        add    edx, 0ah
        sub    esp, 3ch
        mov    dword ptr [ebp - 18h], edx
        cmp    dword ptr [edx + 14h], 72646870h
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_4_3460
        mov    eax, 1
        jmp    NEAR PTR loc_4_3D41
loc_4_3460:
        mov    esi, dword ptr [ebp - 18h]
        mov    edi, 26h
        add    esi, 1ch
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ecx + 18h]
        sub    edx, edx
        div    edi
        dec    eax
        je     SHORT loc_4_34A4
        mov    edx, dword ptr [ebp + 10h]
        mov    cx, word ptr [edx + 2]
loc_4_3486:
        cmp    word ptr [esi + 16h], cx
        jne    SHORT loc_4_3499
        mov    edx, dword ptr [ebp + 10h]
        mov    dx, word ptr [edx + 4]
        cmp    word ptr [esi + 14h], dx
        je     SHORT loc_4_34A4
loc_4_3499:
        add    esi, 26h
        inc    dword ptr [ebp - 0ch]
        cmp    eax, dword ptr [ebp - 0ch]
        ja     SHORT loc_4_3486
loc_4_34A4:
        cmp    eax, dword ptr [ebp - 0ch]
        jne    SHORT loc_4_34B3
        mov    eax, 8005h
        jmp    NEAR PTR loc_4_3D41
loc_4_34B3:
        xor    edi, edi
        mov    edx, dword ptr [ebp + 10h]
        cmp    word ptr [edx], di
        je     NEAR PTR loc_4_361C
        add    edx, 8
        mov    esi, dword ptr [ebp + 10h]
        add    esi, 6
        lea    ecx, [ebp - 8]
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 0ch], edx
        movzx  ecx, word ptr [edx]
        movzx  eax, word ptr [esi]
        push   ecx
        push   eax
        push   dword ptr [ebp + 0ch]
        call   sub_4_2DEF
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_4_35BB
        cmp    dword ptr [ebp - 4], 80h
        jbe    NEAR PTR loc_4_35BB
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 38h
        mov    dword ptr [ebp - 10h], ecx
        lea    eax, [ecx + eax*4 + 62ah]
        mov    ebx, dword ptr [eax]
        mov    dword ptr [ecx + 4], edi
        push   dword ptr [ebp - 18h]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebx + 40h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebx + 44h]
        push   eax
        mov    dword ptr [eax + 1ch], edx
        call   sub_4_2F6A
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
        cmp    eax, edi
        jne    SHORT loc_4_35AA
        mov    ecx, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [ecx]
        mov    ecx, dword ptr [ebp - 10h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edx + eax*4 + 662h]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax + 40h], edx
        mov    ecx, dword ptr [edi + 1ch]
        mov    dword ptr [eax + 44h], ecx
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dl, byte ptr [esi]
        mov    esi, dword ptr [eax]
        mov    byte ptr [esi + 3], dl
        mov    dl, byte ptr [ecx]
        mov    esi, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [esi + 2], dl
        mov    dword ptr [ecx], 80000000h
        movzx  edx, word ptr [ebx + 2ah]
        movzx  ecx, word ptr [eax + 2ah]
        mov    dword ptr [ebp - 24h], edx
        cmp    ecx, edx
        jae    SHORT loc_4_3585
        mov    dword ptr [ebp - 24h], ecx
loc_4_3585:
        mov    edi, dword ptr [eax + 18h]
        mov    esi, dword ptr [ebx + 18h]
        imul   edx, edx, 24h
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        push   ebx
        and    ecx, 3
        rep movsb
        mov    cx, word ptr [ebp - 24h]
        mov    word ptr [eax + 2ah], cx
        jmp    NEAR PTR loc_4_3D37
loc_4_35AA:
        push   ebx
        call   sub_4_0D91
        add    esp, 4
        mov    eax, dword ptr [ebp - 14h]
        jmp    NEAR PTR loc_4_3D41
loc_4_35BB:
        mov    ecx, dword ptr [ebp - 0ch]
        movzx  edx, word ptr [esi]
        movzx  eax, word ptr [ecx]
        push   eax
        push   edx
        push   dword ptr [ebp + 0ch]
        call   sub_4_30D1
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 18h]
        push   ecx
        mov    dword ptr [ecx + 3ch], 0
        mov    dword ptr [ecx + 38h], 7ffffff0h
        call   sub_4_2F6A
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_4_3D41
        mov    ecx, dword ptr [ebp - 18h]
        mov    ebx, dword ptr [ebp + 0ch]
        movzx  edx, word ptr [ecx]
        mov    edx, dword ptr [ebx + edx*4 + 662h]
        mov    bl, byte ptr [esi]
        mov    esi, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [edx]
        mov    byte ptr [ecx + 3], bl
        mov    bl, byte ptr [esi]
        mov    ecx, dword ptr [edx]
        mov    byte ptr [ecx + 2], bl
        jmp    NEAR PTR loc_4_3D41
loc_4_361C:
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ecx + 0ch]
        add    eax, 800h
        push   eax
        call   sub_4_0D74
        add    esp, 4
        mov    dword ptr [ebp - 30h], eax
        test   eax, eax
        jne    SHORT loc_4_3641
        mov    eax, 8003h
        jmp    NEAR PTR loc_4_3D41
loc_4_3641:
        mov    edx, dword ptr [ebp + 10h]
        movzx  eax, word ptr [edx + 8]
        movzx  ecx, word ptr [edx + 6]
        push   eax
        push   ecx
        push   dword ptr [ebp + 0ch]
        call   sub_4_30D1
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 18h]
        push   ecx
        mov    dword ptr [ecx + 3ch], 0
        mov    dword ptr [ecx + 38h], 7ffffff0h
        call   sub_4_16AF
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_4_368E
        push   dword ptr [ebp - 30h]
        call   sub_4_0D91
        add    esp, 4
        mov    eax, edi
        jmp    NEAR PTR loc_4_3D41
loc_4_368E:
        mov    ecx, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [ecx]
        mov    ebx, dword ptr [edx + eax*4 + 662h]
        movzx  eax, word ptr [ebx + 28h]
        shl    eax, 2
        push   eax
        call   sub_4_0D74
        add    esp, 4
        mov    dword ptr [ebp - 1ch], eax
        test   eax, eax
        jne    SHORT loc_4_36CA
        push   dword ptr [ebp - 30h]
        call   sub_4_0D91
        add    esp, 4
        mov    eax, 8003h
        jmp    NEAR PTR loc_4_3D41
loc_4_36CA:
        movzx  eax, word ptr [ebx + 2ah]
        shl    eax, 2
        push   eax
        call   sub_4_0D74
        add    esp, 4
        mov    dword ptr [ebp - 34h], eax
        test   eax, eax
        jne    SHORT loc_4_3701
        push   dword ptr [ebp - 1ch]
        call   sub_4_0D91
        add    esp, 4
        push   dword ptr [ebp - 30h]
        call   sub_4_0D91
        add    esp, 4
        mov    eax, 8003h
        jmp    NEAR PTR loc_4_3D41
loc_4_3701:
        xor    eax, eax
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 2ch], eax
        cmp    dword ptr [ebp - 0ch], eax
        jne    SHORT loc_4_3726
        mov    eax, dword ptr [ebx]
        mov    esi, dword ptr [ebx + 4]
        movzx  ecx, word ptr [eax + 1eh]
        movzx  edx, word ptr [esi + ecx*4]
        mov    dword ptr [ebp - 2ch], ecx
        mov    dword ptr [ebp - 14h], edx
        jmp    NEAR PTR loc_4_37D6
loc_4_3726:
        mov    edi, dword ptr [ebx]
        mov    edx, dword ptr [ebp - 0ch]
        imul   edx, edx, 1ah
        mov    ecx, 6
        lea    esi, [edi + edx]
        rep movsd
        movsw
        mov    edi, dword ptr [ebx]
        xor    ecx, ecx
        mov    word ptr [edi + 4], cx
        mov    edx, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [ebx]
        imul   edx, edx, 1ah
        movzx  eax, word ptr [edi + edx + 4]
        mov    edx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    edi, dword ptr [ebx]
        imul   edx, edx, 1ah
        movzx  eax, word ptr [edi + edx + 1eh]
        cmp    eax, dword ptr [ebp - 4]
        jbe    SHORT loc_4_37D6
loc_4_3764:
        mov    dx, word ptr [ebp - 14h]
        mov    eax, dword ptr [ebx + 4]
        mov    word ptr [eax + ecx], dx
        inc    dword ptr [ebp - 2ch]
        add    ecx, 4
        mov    edx, dword ptr [ebp - 4]
        shl    edx, 2
        mov    esi, dword ptr [ebx + 4]
        movzx  eax, word ptr [esi + edx]
        mov    dword ptr [ebp - 8], eax
        mov    esi, dword ptr [ebx + 4]
        movzx  edx, word ptr [esi + edx + 4]
        cmp    edx, eax
        jbe    SHORT loc_4_37C1
        mov    edx, dword ptr [ebp - 14h]
        imul   edx, edx, 3
loc_4_3797:
        mov    eax, dword ptr [ebx + 8]
        mov    esi, dword ptr [ebp - 8]
        imul   esi, esi, 3
        lea    edi, [edx + eax]
        add    esi, eax
        movsw
        movsb
        add    edx, 3
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [ebp - 14h]
        inc    dword ptr [ebp - 8]
        mov    esi, dword ptr [ebx + 4]
        movzx  edi, word ptr [esi + eax*4 + 4]
        cmp    edi, dword ptr [ebp - 8]
        ja     SHORT loc_4_3797
loc_4_37C1:
        mov    edx, dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebx]
        imul   edx, edx, 1ah
        movzx  eax, word ptr [eax + edx + 1eh]
        cmp    eax, dword ptr [ebp - 4]
        ja     SHORT loc_4_3764
loc_4_37D6:
        mov    edx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebx]
        mov    cl, byte ptr [edx + 6]
        mov    byte ptr [eax + 3], cl
        mov    cl, byte ptr [edx + 8]
        mov    eax, dword ptr [ebx]
        mov    byte ptr [eax + 2], cl
        mov    eax, dword ptr [ebx]
        mov    word ptr [eax + 1ah], 0ffffh
        mov    cx, word ptr [ebp - 2ch]
        mov    eax, dword ptr [ebx]
        mov    byte ptr [eax + 1dh], 0ffh
        mov    dx, word ptr [ebp - 14h]
        mov    eax, dword ptr [ebx]
        cmp    dword ptr [ebp - 14h], 0
        mov    word ptr [eax + 1eh], cx
        mov    ecx, dword ptr [ebp - 2ch]
        mov    esi, dword ptr [ebx + 4]
        mov    word ptr [esi + ecx*4], dx
        je     SHORT loc_4_3892
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 14h]
loc_4_381A:
        mov    eax, dword ptr [ebx + 8]
        add    eax, edx
        cmp    byte ptr [eax], 29h
        jne    SHORT loc_4_388C
        movsx  esi, word ptr [eax + 1]
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + esi*4], 1
        mov    edi, dword ptr [ebx + 0ch]
        movzx  eax, word ptr [edi + esi*2]
        mov    edi, eax
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebx + 10h]
        movzx  edi, word ptr [eax + edi*4]
        mov    dword ptr [ebp - 4], edi
        mov    eax, dword ptr [ebx + 0ch]
        movzx  esi, word ptr [eax + esi*2 + 2]
        mov    dword ptr [ebp - 8], esi
        mov    eax, dword ptr [ebx + 10h]
        movzx  edi, word ptr [eax + esi*4]
        mov    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], edi
        jbe    SHORT loc_4_388C
loc_4_3865:
        mov    eax, dword ptr [ebp - 4]
        imul   eax, eax, 3
        add    eax, dword ptr [ebx + 14h]
        cmp    byte ptr [eax], 29h
        jne    SHORT loc_4_3881
        movsx  esi, word ptr [eax + 1]
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + esi*4], 1
loc_4_3881:
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], eax
        ja     SHORT loc_4_3865
loc_4_388C:
        add    edx, 3
        dec    ecx
        jne    SHORT loc_4_381A
loc_4_3892:
        mov    edx, 1
        xor    eax, eax
        cmp    word ptr [ebx + 28h], ax
        jbe    SHORT loc_4_38B6
        mov    ecx, dword ptr [ebp - 1ch]
loc_4_38A2:
        cmp    dword ptr [ecx], 0
        je     SHORT loc_4_38AA
        mov    dword ptr [ecx], edx
        inc    edx
loc_4_38AA:
        add    ecx, 4
        inc    eax
        movzx  esi, word ptr [ebx + 28h]
        cmp    esi, eax
        ja     SHORT loc_4_38A2
loc_4_38B6:
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_4_3940
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 14h]
loc_4_38C5:
        mov    edi, dword ptr [ebx + 8]
        add    edi, edx
        cmp    byte ptr [edi], 29h
        jne    SHORT loc_4_393A
        mov    eax, dword ptr [ebp - 1ch]
        movsx  esi, word ptr [edi + 1]
        mov    eax, dword ptr [eax + esi*4]
        dec    ax
        mov    word ptr [edi + 1], ax
        mov    edi, dword ptr [ebx + 0ch]
        movzx  eax, word ptr [edi + esi*2]
        mov    edi, eax
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebx + 10h]
        movzx  edi, word ptr [eax + edi*4]
        mov    dword ptr [ebp - 4], edi
        mov    eax, dword ptr [ebx + 0ch]
        movzx  esi, word ptr [eax + esi*2 + 2]
        mov    edi, esi
        mov    dword ptr [ebp - 8], esi
        mov    esi, dword ptr [ebx + 10h]
        movzx  eax, word ptr [esi + edi*4]
        mov    dword ptr [ebp - 8], eax
        cmp    eax, dword ptr [ebp - 4]
        jbe    SHORT loc_4_393A
loc_4_3911:
        mov    esi, dword ptr [ebp - 4]
        imul   esi, esi, 3
        add    esi, dword ptr [ebx + 14h]
        cmp    byte ptr [esi], 29h
        jne    SHORT loc_4_392F
        mov    eax, dword ptr [ebp - 1ch]
        movsx  edi, word ptr [esi + 1]
        mov    eax, dword ptr [eax + edi*4]
        dec    ax
        mov    word ptr [esi + 1], ax
loc_4_392F:
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], eax
        ja     SHORT loc_4_3911
loc_4_393A:
        add    edx, 3
        dec    ecx
        jne    SHORT loc_4_38C5
loc_4_3940:
        xor    esi, esi
        xor    ecx, ecx
        cmp    word ptr [ebx + 28h], si
        mov    dword ptr [ebp - 28h], esi
        jbe    SHORT loc_4_396B
        mov    edx, dword ptr [ebp - 1ch]
        mov    eax, edx
loc_4_3952:
        cmp    dword ptr [eax], 0
        je     SHORT loc_4_395F
        mov    dword ptr [edx], ecx
        inc    dword ptr [ebp - 28h]
        add    edx, 4
loc_4_395F:
        add    eax, 4
        inc    ecx
        movzx  edi, word ptr [ebx + 28h]
        cmp    edi, ecx
        ja     SHORT loc_4_3952
loc_4_396B:
        xor    ecx, ecx
        mov    dword ptr [ebp - 10h], ecx
        mov    dword ptr [ebp - 20h], ecx
        mov    dword ptr [ebp - 0ch], ecx
        cmp    dword ptr [ebp - 28h], ecx
        je     NEAR PTR loc_4_3A6F
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 24h], eax
loc_4_3985:
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [ebp - 0ch], eax
        jne    SHORT loc_4_39A9
        mov    eax, dword ptr [ebx + 0ch]
        mov    edi, dword ptr [ebx + 10h]
        movzx  esi, word ptr [eax + ecx + 2]
        movzx  edx, word ptr [edi + esi*4]
        mov    dword ptr [ebp - 20h], esi
        mov    dword ptr [ebp - 10h], edx
        jmp    NEAR PTR loc_4_3A59
loc_4_39A9:
        mov    di, word ptr [ebp - 20h]
        mov    edx, dword ptr [ebx + 0ch]
        mov    si, word ptr [edx + eax*2]
        mov    word ptr [ecx + edx], si
        mov    edx, dword ptr [ebx + 0ch]
        mov    word ptr [edx + ecx], di
        mov    edx, dword ptr [ebx + 0ch]
        movzx  esi, word ptr [edx + eax*2]
        mov    dword ptr [ebp - 4], esi
        mov    edx, dword ptr [ebx + 0ch]
        movzx  edi, word ptr [edx + eax*2 + 2]
        cmp    edi, esi
        jbe    NEAR PTR loc_4_3A59
        mov    esi, dword ptr [ebp - 20h]
        shl    esi, 2
        mov    dword ptr [ebp - 3ch], esi
loc_4_39E2:
        mov    dx, word ptr [ebp - 10h]
        mov    edi, dword ptr [ebx + 10h]
        mov    esi, dword ptr [ebp - 3ch]
        inc    dword ptr [ebp - 20h]
        mov    word ptr [edi + esi], dx
        mov    esi, dword ptr [ebp - 4]
        shl    esi, 2
        mov    edx, dword ptr [ebx + 10h]
        add    dword ptr [ebp - 3ch], 4
        movzx  edi, word ptr [edx + esi]
        mov    dword ptr [ebp - 8], edi
        mov    edx, dword ptr [ebx + 10h]
        movzx  esi, word ptr [edx + esi + 4]
        cmp    esi, edi
        jbe    SHORT loc_4_3A49
        mov    edx, dword ptr [ebp - 10h]
        imul   edx, edx, 3
        mov    dword ptr [ebp - 38h], edx
loc_4_3A1C:
        mov    edx, dword ptr [ebx + 14h]
        mov    esi, dword ptr [ebp - 8]
        imul   esi, esi, 3
        mov    edi, dword ptr [ebp - 38h]
        add    esi, edx
        add    edi, edx
        movsw
        movsb
        mov    edx, dword ptr [ebp - 4]
        inc    dword ptr [ebp - 10h]
        inc    dword ptr [ebp - 8]
        mov    edi, dword ptr [ebx + 10h]
        add    dword ptr [ebp - 38h], 3
        movzx  esi, word ptr [edi + edx*4 + 4]
        cmp    esi, dword ptr [ebp - 8]
        ja     SHORT loc_4_3A1C
loc_4_3A49:
        inc    dword ptr [ebp - 4]
        mov    edx, dword ptr [ebx + 0ch]
        movzx  esi, word ptr [edx + eax*2 + 2]
        cmp    esi, dword ptr [ebp - 4]
        ja     SHORT loc_4_39E2
loc_4_3A59:
        add    ecx, 2
        mov    edi, dword ptr [ebp - 28h]
        add    dword ptr [ebp - 24h], 4
        inc    dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 0ch], edi
        jb     NEAR PTR loc_4_3985
loc_4_3A6F:
        mov    si, word ptr [ebp - 20h]
        mov    eax, dword ptr [ebx + 0ch]
        mov    dx, word ptr [ebp - 10h]
        mov    edi, dword ptr [ebp - 28h]
        mov    word ptr [eax + edi*2], si
        xor    ecx, ecx
        mov    eax, dword ptr [ebx + 10h]
        mov    esi, dword ptr [ebp - 20h]
        mov    word ptr [eax + esi*4], dx
        cmp    word ptr [ebx + 2ah], cx
        jbe    SHORT loc_4_3AAB
        xor    edx, edx
loc_4_3A95:
        mov    eax, dword ptr [ebx + 18h]
        add    edx, 24h
        mov    word ptr [eax + edx - 0ah], 0
        movzx  eax, word ptr [ebx + 2ah]
        inc    ecx
        cmp    eax, ecx
        ja     SHORT loc_4_3A95
loc_4_3AAB:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_4_3AE7
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 10h]
loc_4_3AB6:
        mov    eax, dword ptr [ebx + 14h]
        add    eax, edx
        cmp    byte ptr [eax], 35h
        jne    SHORT loc_4_3AE1
        mov    esi, dword ptr [ebp - 34h]
        movsx  eax, word ptr [eax + 1]
        mov    dword ptr [esi + eax*4], 1
        mov    esi, dword ptr [ebx + 18h]
        mov    edi, dword ptr [ebx + 14h]
        movsx  eax, word ptr [edi + edx + 1]
        imul   eax, eax, 24h
        inc    word ptr [eax + esi + 1ah]
loc_4_3AE1:
        add    edx, 3
        dec    ecx
        jne    SHORT loc_4_3AB6
loc_4_3AE7:
        mov    ecx, 1
        xor    eax, eax
        cmp    word ptr [ebx + 2ah], ax
        jbe    SHORT loc_4_3B0B
        mov    edx, dword ptr [ebp - 34h]
loc_4_3AF7:
        cmp    dword ptr [edx], 0
        je     SHORT loc_4_3AFF
        mov    dword ptr [edx], ecx
        inc    ecx
loc_4_3AFF:
        add    edx, 4
        inc    eax
        movzx  esi, word ptr [ebx + 2ah]
        cmp    esi, eax
        ja     SHORT loc_4_3AF7
loc_4_3B0B:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_4_3B36
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 10h]
loc_4_3B16:
        mov    edi, dword ptr [ebx + 14h]
        add    edi, edx
        cmp    byte ptr [edi], 35h
        jne    SHORT loc_4_3B30
        mov    esi, dword ptr [ebp - 34h]
        movsx  eax, word ptr [edi + 1]
        mov    eax, dword ptr [esi + eax*4]
        dec    ax
        mov    word ptr [edi + 1], ax
loc_4_3B30:
        add    edx, 3
        dec    ecx
        jne    SHORT loc_4_3B16
loc_4_3B36:
        xor    edx, edx
        cmp    word ptr [ebx + 2ah], dx
        mov    dword ptr [ebp - 24h], edx
        mov    dword ptr [ebp - 0ch], edx
        jbe    SHORT loc_4_3B83
        xor    eax, eax
        mov    esi, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 3ch], edx
        mov    dword ptr [ebp - 38h], esi
loc_4_3B4F:
        mov    ecx, dword ptr [ebp - 38h]
        cmp    dword ptr [ecx], 0
        je     SHORT loc_4_3B6F
        mov    ecx, dword ptr [ebx + 18h]
        mov    esi, dword ptr [ebp - 3ch]
        add    esi, ecx
        lea    edi, [eax + ecx]
        mov    ecx, 9
        rep movsd
        add    eax, 24h
        inc    dword ptr [ebp - 24h]
loc_4_3B6F:
        add    dword ptr [ebp - 3ch], 24h
        movzx  ecx, word ptr [ebx + 2ah]
        add    dword ptr [ebp - 38h], 4
        inc    dword ptr [ebp - 0ch]
        cmp    ecx, dword ptr [ebp - 0ch]
        ja     SHORT loc_4_3B4F
loc_4_3B83:
        push   dword ptr [ebp - 1ch]
        call   sub_4_0D91
        add    esp, 4
        push   dword ptr [ebp - 34h]
        call   sub_4_0D91
        add    esp, 4
        mov    ecx, dword ptr [ebp - 24h]
        mov    edi, dword ptr [ebp - 28h]
        mov    eax, dword ptr [ebp - 10h]
        imul   ecx, ecx, 9
        add    ecx, dword ptr [ebp - 20h]
        add    eax, dword ptr [ebp - 14h]
        add    ecx, dword ptr [ebp - 2ch]
        imul   eax, eax, 3
        lea    edx, [edi + ecx*2 + 54h]
        mov    ecx, dword ptr [ebp - 18h]
        lea    eax, [eax + edx*2]
        push   eax
        mov    dword ptr [ecx + 10h], eax
        call   sub_4_0D74
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_4_3BE0
        push   dword ptr [ebp - 30h]
        call   sub_4_0D91
        add    esp, 4
        mov    eax, 8003h
        jmp    NEAR PTR loc_4_3D41
loc_4_3BE0:
        mov    ecx, dword ptr [ebp - 18h]
        mov    esi, dword ptr [ebp + 0ch]
        movzx  edx, word ptr [ecx]
        mov    edi, eax
        mov    ecx, 12h
        mov    dword ptr [esi + edx*4 + 662h], 0
        mov    esi, ebx
        mov    edx, eax
        rep movsd
        movsw
        add    eax, 4ch
        mov    esi, dword ptr [edx]
        mov    edi, eax
        mov    ecx, 0dh
        rep movsd
        mov    dword ptr [edx], eax
        add    eax, 34h
        mov    ecx, eax
        mov    esi, dword ptr [edx + 4]
        and    ecx, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 2ch]
        add    eax, 4
        lea    ecx, [ecx*4 + 4]
        shr    ecx, 2
        mov    edi, eax
        rep movsd
        mov    ecx, dword ptr [ebp - 2ch]
        mov    dword ptr [edx + 4], eax
        mov    ebx, dword ptr [ebp - 14h]
        lea    eax, [eax + ecx*4 + 4]
        mov    ecx, eax
        mov    esi, dword ptr [edx + 8]
        imul   ebx, ebx, 3
        and    ecx, 3
        sub    eax, ecx
        mov    ecx, ebx
        shr    ecx, 2
        add    eax, 4
        mov    edi, eax
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebp - 28h]
        mov    dword ptr [edx + 8], eax
        add    eax, ebx
        lea    ebx, [edi*2 + 2]
        mov    ecx, eax
        mov    esi, dword ptr [edx + 0ch]
        and    ecx, 3
        sub    eax, ecx
        mov    ecx, ebx
        add    eax, 4
        mov    edi, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebp - 28h]
        mov    dword ptr [edx + 0ch], eax
        mov    esi, dword ptr [ebp - 20h]
        lea    eax, [eax + edi*2 + 2]
        mov    ecx, eax
        and    ecx, 3
        sub    eax, ecx
        lea    ecx, [esi*4 + 4]
        shr    ecx, 2
        add    eax, 4
        mov    edi, eax
        mov    esi, dword ptr [edx + 10h]
        rep movsd
        mov    esi, dword ptr [ebp - 20h]
        mov    dword ptr [edx + 10h], eax
        mov    ebx, dword ptr [ebp - 10h]
        lea    eax, [eax + esi*4 + 4]
        mov    ecx, eax
        mov    esi, dword ptr [edx + 14h]
        imul   ebx, ebx, 3
        and    ecx, 3
        sub    eax, ecx
        mov    ecx, ebx
        shr    ecx, 2
        add    eax, 4
        mov    edi, eax
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    dword ptr [edx + 14h], eax
        add    eax, ebx
        mov    ecx, eax
        mov    ebx, dword ptr [ebp - 24h]
        and    ecx, 3
        mov    esi, dword ptr [edx + 18h]
        imul   ebx, ebx, 24h
        sub    eax, ecx
        mov    ecx, ebx
        shr    ecx, 2
        add    eax, 4
        mov    edi, eax
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    di, word ptr [ebp - 28h]
        mov    dword ptr [edx + 18h], eax
        mov    dword ptr [edx + 40h], 0
        mov    word ptr [edx + 28h], di
        mov    bx, word ptr [ebp - 24h]
        mov    ecx, dword ptr [ebp - 18h]
        push   dword ptr [ebp - 30h]
        mov    word ptr [edx + 2ah], bx
        mov    ebx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [ecx]
        mov    dword ptr [ebx + eax*4 + 662h], edx
loc_4_3D37:
        call   sub_4_0D91
        add    esp, 4
        xor    eax, eax
loc_4_3D41:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_3D48:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   ebp
        cmp    dword ptr [esi + 38h], 1000000h
        jb     NEAR PTR loc_4_3E20
        xor    ebx, ebx
        mov    edx, dword ptr [esp + 18h]
        movzx  eax, word ptr [edx]
        mov    edi, dword ptr [esi + eax*4 + 662h]
        mov    ax, word ptr [edi + 2ah]
        mov    ebp, dword ptr [edi + 18h]
        test   ax, ax
        je     SHORT loc_4_3D8E
        movzx  eax, ax
        lea    ecx, [ebp + 1ch]
loc_4_3D7F:
        test   byte ptr [ecx + 3], 80h
        je     SHORT loc_4_3D88
        add    ebx, dword ptr [ecx + 4]
loc_4_3D88:
        add    ecx, 24h
        dec    eax
        jne    SHORT loc_4_3D7F
loc_4_3D8E:
        xor    eax, eax
        test   ebx, ebx
        jne    SHORT loc_4_3DA0
        mov    edx, dword ptr [esp + 18h]
        mov    dword ptr [edx + 14h], eax
        jmp    NEAR PTR loc_4_3E2A
loc_4_3DA0:
        lea    eax, [ebx + 10h]
        cmp    dword ptr [esi + 0ch], eax
        jae    SHORT loc_4_3DAF
        mov    eax, 1
        jmp    SHORT loc_4_3E2A
loc_4_3DAF:
        push   eax
        push   esi
        call   sub_4_11BC
        add    esp, 8
        xor    ecx, ecx
        mov    dword ptr [esi + 38h], eax
        mov    dword ptr [esi + 1ch], ebx
        mov    dword ptr [edi + 40h], eax
        mov    dword ptr [edi + 44h], ebx
        mov    dword ptr [esi + 24h], ecx
        mov    esi, dword ptr [esp + 18h]
        add    esi, 14h
        cmp    word ptr [edi + 2ah], cx
        jbe    SHORT loc_4_3E05
loc_4_3DD7:
        mov    ebx, dword ptr [ebp + 1ch]
        test   ebx, 80000000h
        je     SHORT loc_4_3DF9
        mov    edx, dword ptr [esp + 18h]
        mov    edx, dword ptr [edx + 4]
        lea    ebx, [edx + ebx*2]
        mov    dword ptr [esi], ebx
        add    esi, 8
        mov    edx, dword ptr [ebp + 20h]
        add    edx, edx
        mov    dword ptr [esi - 4], edx
loc_4_3DF9:
        inc    ecx
        add    ebp, 24h
        movzx  edx, word ptr [edi + 2ah]
        cmp    edx, ecx
        ja     SHORT loc_4_3DD7
loc_4_3E05:
        push   eax
        mov    dword ptr [esi], 0
        movzx  edx, word ptr [edi + 2ah]
        push   edx
        push   dword ptr [edi + 18h]
        call   sub_4_33C1
        add    esp, 0ch
        xor    eax, eax
        jmp    SHORT loc_4_3E2A
loc_4_3E20:
        push   dword ptr [esp + 18h]
        push   esi
        call   sub_4_1E12
loc_4_3E2A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_4_3E2F:
        push   esi
        mov    ecx, 5
        push   edi
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [esp + 0ch]
        mov    word ptr [edi], 1
        add    edi, 2
        rep movsd
        movsw
        xor    eax, eax
        pop    edi
        pop    esi
        ret
sub_4_3E50:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        movzx  eax, word ptr [edi]
        test   eax, eax
        je     SHORT loc_4_3E67
        mov    eax, 1
        jmp    NEAR PTR loc_4_3EF7
loc_4_3E67:
        mov    cx, word ptr [edi + 2]
        movzx  eax, cx
        test   eax, eax
        je     SHORT loc_4_3E88
        cmp    eax, 1
        je     SHORT loc_4_3EA3
        cmp    eax, 2
        jl     SHORT loc_4_3E81
        cmp    eax, 3
        jle    SHORT loc_4_3EBE
loc_4_3E81:
        mov    eax, 1
        jmp    SHORT loc_4_3EF7
loc_4_3E88:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 28h
        mov    word ptr [edi + 4], 8
        mov    word ptr [edi + 6], 4
        add    edi, 8
        jmp    SHORT loc_4_3EF3
loc_4_3EA3:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 28h
        mov    word ptr [edi + 4], 8
        mov    word ptr [edi + 6], 2
        add    edi, 8
        jmp    SHORT loc_4_3EF3
loc_4_3EBE:
        mov    word ptr [edi + 4], 0ch
        mov    eax, dword ptr [esp + 0ch]
        mov    word ptr [edi + 6], 5
        cmp    dword ptr [eax + 151eh], 4d474d31h
        jne    SHORT loc_4_3EE6
        cmp    cx, 3
        jne    SHORT loc_4_3EE6
        mov    word ptr [edi + 6], 9
loc_4_3EE6:
        db  0BEh, 000h, 000h, 000h, 000h
        add    edi, 8
        mov    ecx, 3ch
loc_4_3EF3:
        rep movsd
        xor    eax, eax
loc_4_3EF7:
        pop    edi
        pop    esi
        ret
sub_4_3EFA:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        movzx  eax, word ptr [edi]
        test   eax, eax
        jne    NEAR PTR loc_4_3F8D
        mov    esi, dword ptr [esp + 0ch]
        cmp    word ptr [esi + 12h], 0
        je     SHORT loc_4_3F25
        push   esi
        call   sub_4_2380
        mov    word ptr [esi + 12h], 0
        add    esp, 4
loc_4_3F25:
        mov    cl, byte ptr [edi + 2]
        cmp    cl, 8
        jge    SHORT loc_4_3F8D
        mov    al, byte ptr [edi + 3]
        cmp    al, 8
        jge    SHORT loc_4_3F8D
        cmp    cl, 0ffh
        jl     SHORT loc_4_3F8D
        cmp    al, 0ffh
        jl     SHORT loc_4_3F8D
        mov    al, byte ptr [edi + 4]
        cmp    al, 0ch
        jge    SHORT loc_4_3F8D
        mov    dl, byte ptr [edi + 5]
        cmp    dl, 0ch
        jge    SHORT loc_4_3F8D
        cmp    al, 0ffh
        jl     SHORT loc_4_3F8D
        cmp    dl, 0ffh
        jl     SHORT loc_4_3F8D
        movsx  eax, cl
        push   eax
        push   esi
        call   sub_4_264C
        add    esp, 8
        movsx  ecx, byte ptr [edi + 3]
        push   ecx
        push   esi
        call   sub_4_25AA
        movsx  ecx, byte ptr [edi + 5]
        add    esp, 8
        movsx  eax, byte ptr [edi + 4]
        push   ecx
        push   eax
        push   esi
        call   sub_4_2458
        add    esp, 0ch
        mov    ecx, dword ptr [esi + 14h]
        xor    eax, eax
        mov    dword ptr [edi + 2], ecx
        jmp    SHORT loc_4_3F92
loc_4_3F8D:
        mov    eax, 1
loc_4_3F92:
        pop    edi
        pop    esi
        ret
sub_4_3F95:
        push   esi
        mov    cl, 1eh
        mov    esi, dword ptr [esp + 8]
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    al, byte ptr [esi + 458h]
        mov    byte ptr [edi], al
        mov    al, byte ptr [esi + 44ch]
        imul   cl
        mov    byte ptr [edi + 1], al
        mov    al, byte ptr [esi + 446h]
        mov    byte ptr [edi + 40h], al
        mov    dl, byte ptr [esi + 44bh]
        mov    byte ptr [edi + 7], dl
        mov    cl, byte ptr [esi + 449h]
        mov    byte ptr [edi + 0ah], cl
        mov    ecx, 9
        mov    al, byte ptr [esi + 44ah]
        mov    byte ptr [edi + 0bh], al
        movzx  eax, byte ptr [esi + 447h]
        imul   eax, eax, 0ah
        cdq
        idiv   ecx
        mov    byte ptr [edi + 5bh], al
        movzx  eax, byte ptr [esi + 448h]
        imul   eax, eax, 0ah
        cdq
        idiv   ecx
        mov    byte ptr [edi + 5dh], al
        xor    eax, eax
        pop    edi
        pop    esi
        ret
sub_4_4002:
        sub    esp, 0ch
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 18h]
        push   edi
        push   ebp
        xor    edi, edi
        mov    ebp, 0ffh
loc_4_4014:
        push   0
        push   0
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_401F:
        mov    byte ptr [esp + edi + 20h], al
        add    esp, 10h
        cmp    eax, ebp
        mov    ebx, eax
        je     SHORT loc_4_40AB
        shl    eax, 5
        cmp    byte ptr [eax + esi + 59h], 0
        je     SHORT loc_4_4039
        dec    dword ptr [esi + 18h]
loc_4_4039:
        push   80h
        mov    eax, ebx
        or     eax, 2a0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_404C:
        push   0
        mov    eax, ebx
        or     al, 60h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4059:
        push   0
        mov    eax, ebx
        or     al, 40h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4066:
        push   0
        mov    eax, ebx
        or     al, 0c0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4073:
        push   0
        mov    eax, ebx
        or     al, 0e0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4080:
        push   40000000h
        mov    eax, ebx
        or     al, 20h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4090:
        push   40000000h
        push   ebx
        push   esi
        or     bh, 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_409F:
        push   4000000h
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_40AB:
        inc    edi
        cmp    edi, 0ah
        jb     NEAR PTR loc_4_4014
        mov    ebx, dword ptr [esp + 24h]
        push   dword ptr [ebx]
        push   234h
        lea    edi, [ebx + 8]
        push   esi
        add    ebx, 4
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_40CC:
        cmp    dword ptr [ebx], 0
        je     SHORT loc_4_40E9
loc_4_40D1:
        push   23ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_40DC:
        mov    word ptr [edi], ax
        add    edi, 2
        mov    eax, dword ptr [ebx]
        dec    eax
        mov    dword ptr [ebx], eax
        jne    SHORT loc_4_40D1
loc_4_40E9:
        xor    edi, edi
loc_4_40EB:
        movzx  ebx, byte ptr [esp + edi + 10h]
        cmp    ebx, 0ffh
        je     SHORT loc_4_4149
        push   0
        mov    eax, ebx
        or     ah, 2
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4106:
        push   1d8h
        mov    eax, ebx
        or     al, 0c0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4116:
        push   1e0h
        mov    eax, ebx
        or     al, 0e0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4126:
        push   48000000h
        mov    eax, ebx
        shl    ebx, 5
        or     al, 20h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4139:
        lea    eax, [ebx + esi]
        mov    word ptr [eax + 42h], 0ffeh
        mov    byte ptr [eax + 59h], 1
        inc    dword ptr [esi + 18h]
loc_4_4149:
        inc    edi
        cmp    edi, 0ah
        jb     SHORT loc_4_40EB
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_4_4159:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        mov    eax, dword ptr [edi]
        cmp    eax, 1
        je     SHORT loc_4_4176
        cmp    eax, 2
        je     SHORT loc_4_419A
        mov    eax, 8006h
        jmp    NEAR PTR loc_4_421C
loc_4_4176:
        add    edi, 4
        mov    esi, dword ptr [esp + 10h]
        mov    dword ptr [edi], 41h
        cmp    dword ptr [esi + 154eh], 0
        je     SHORT loc_4_4192
        mov    dword ptr [edi], 51h
loc_4_4192:
        or     byte ptr [edi], 20h
        jmp    NEAR PTR loc_4_421A
loc_4_419A:
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [esi + 1558h], 10h
        je     SHORT loc_4_41B0
        mov    dword ptr [edi + 4], 0ffffffffh
        jmp    SHORT loc_4_41BA
loc_4_41B0:
        movzx  eax, byte ptr [esi + 155ch]
        mov    dword ptr [edi + 4], eax
loc_4_41BA:
        movzx  eax, byte ptr [esi + 155dh]
        mov    dword ptr [edi + 8], eax
        movzx  ecx, word ptr [esi + 155eh]
        mov    dword ptr [edi + 0ch], ecx
        mov    bx, word ptr [esi + 24h]
        movzx  edx, word ptr [esi + 26h]
        movzx  eax, bx
        mov    ecx, eax
        sub    ecx, edx
        cmp    ecx, -4
        jne    SHORT loc_4_41F9
        xor    eax, eax
        mov    ax, bx
        and    eax, 0fff8h
        mov    dword ptr [edi + 10h], eax
        xor    eax, eax
        mov    dword ptr [edi + 14h], eax
        mov    dword ptr [edi + 18h], eax
        jmp    SHORT loc_4_420A
loc_4_41F9:
        mov    dword ptr [edi + 10h], eax
        movzx  eax, word ptr [esi + 26h]
        mov    dword ptr [edi + 14h], eax
        movzx  ecx, word ptr [esi + 28h]
        mov    dword ptr [edi + 18h], ecx
loc_4_420A:
        add    esi, 1560h
        add    edi, 1ch
        mov    ecx, 20h
        rep movsd
loc_4_421A:
        xor    eax, eax
loc_4_421C:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_4_4220:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 152eh]
        or     al, 4
        movzx  edx, ax
        push   edx
        push   23fh
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_423C:
        xor    eax, eax
        ret
sub_4_423F:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   23eh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4250:
        movzx  ax, al
        mov    di, ax
        and    di, 40h
        shr    di, 1
        mov    cx, ax
        push   23dh
        and    cx, 90h
        push   esi
        or     di, cx
        shr    di, 1
        and    ax, 20h
        or     di, ax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_427E:
        shr    di, 2
        push   0
        xor    cx, cx
        push   23fh
        mov    cl, al
        push   esi
        and    cx, 1
        or     di, cx
        mov    word ptr [esi + 152eh], di
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_42A2:
        xor    eax, eax
        pop    edi
        pop    esi
        ret
sub_4_42A7:
        push   ebp
        movzx  eax, word ptr [esp + 0ch]
        mov    ebp, esp
        sub    eax, 14h
        sub    esp, 4
        cmp    eax, 2bh
        push   esi
        push   edi
        ja     SHORT loc_4_42C3
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_42C3:
        mov    eax, 8006h
        jmp    NEAR PTR loc_4_43EA
loc_4_42CD:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_4_210E
        jmp    NEAR PTR loc_4_43EA
loc_4_42DD:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_4_1E12
        jmp    NEAR PTR loc_4_43EA
loc_4_42ED:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_4_2F6A
        add    esp, 8
        jmp    NEAR PTR loc_4_43EA
loc_4_4300:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_4_338B
        add    esp, 8
        jmp    NEAR PTR loc_4_43EA
loc_4_4313:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        call   sub_4_343A
        add    esp, 0ch
        jmp    NEAR PTR loc_4_43EA
loc_4_4329:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_4_3D48
        add    esp, 8
        jmp    NEAR PTR loc_4_43EA
loc_4_433C:
        mov    ecx, dword ptr [ebp + 14h]
        movzx  eax, word ptr [ecx]
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_4_2E56
        add    esp, 8
        jmp    NEAR PTR loc_4_43EA
loc_4_4353:
        mov    ecx, dword ptr [ebp + 14h]
        movzx  eax, word ptr [ecx]
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_4_2F0D
        add    esp, 8
        jmp    NEAR PTR loc_4_43EA
loc_4_436A:
        mov    ecx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ecx]
        mov    ecx, eax
        shr    eax, 10h
        and    ecx, 0ffffh
        push   ecx
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_4_30D1
        add    esp, 0ch
        jmp    SHORT loc_4_43EA
loc_4_4389:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_4_2F36
        add    esp, 8
        jmp    SHORT loc_4_43EA
loc_4_4399:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_4_3330
        add    esp, 8
        jmp    SHORT loc_4_43EA
loc_4_43A9:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_4_2EAD
        add    esp, 8
        jmp    SHORT loc_4_43EA
loc_4_43B9:
        push   dword ptr [ebp + 10h]
        call   sub_4_2357
        jmp    SHORT loc_4_43EA
loc_4_43C3:
        mov    ecx, dword ptr [ebp + 14h]
        push   dword ptr [ecx]
        push   dword ptr [ebp + 10h]
        call   sub_4_11BC
        add    esp, 8
        jmp    SHORT loc_4_43EA
loc_4_43D5:
        mov    ecx, dword ptr [ebp + 14h]
        push   dword ptr [ecx + 4]
        push   dword ptr [ecx]
        push   dword ptr [ebp + 10h]
        call   sub_4_1531
        add    esp, 0ch
loc_4_43E8:
        xor    eax, eax
loc_4_43EA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_4_43F0:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_4_3EFA
        add    esp, 8
        jmp    SHORT loc_4_43EA
loc_4_4400:
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp + 10h]
        movzx  ax, byte ptr [ecx + 1]
        mov    word ptr [edx + 10h], ax
        jmp    SHORT loc_4_43E8
loc_4_4411:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_4437
        db  0A1h, 000h, 000h, 000h, 000h
        sub    eax, dword ptr [ebp + 10h]
        cmp    eax, -24h
        jne    SHORT loc_4_4437
        mov    ecx, dword ptr [ebp + 14h]
        push   dword ptr [ecx]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4432:
        add    esp, 4
        jmp    SHORT loc_4_43E8
loc_4_4437:
        mov    eax, 8007h
        jmp    SHORT loc_4_43EA
loc_4_443E:
        mov    ecx, dword ptr [ebp + 10h]
        cmp    dword ptr [ecx + 152ah], 0
        jne    SHORT loc_4_4451
        mov    eax, 8007h
        jmp    SHORT loc_4_43EA
loc_4_4451:
        mov    ecx, dword ptr [ebp + 14h]
        cmp    dword ptr [ecx], 0
        je     SHORT loc_4_44AF
        mov    edx, dword ptr [ebp + 10h]
        add    edx, 419h
        cmp    byte ptr [edx], 0
        jne    SHORT loc_4_446D
        mov    ecx, dword ptr [ebp + 10h]
        inc    dword ptr [ecx + 18h]
loc_4_446D:
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 439h
        cmp    byte ptr [eax], 0
        jne    SHORT loc_4_4480
        mov    ecx, dword ptr [ebp + 10h]
        inc    dword ptr [ecx + 18h]
loc_4_4480:
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [edx], 1
        push   ecx
        mov    byte ptr [eax], 1
        mov    word ptr [ecx + 402h], 0ffeh
        mov    word ptr [ecx + 422h], 0ffeh
        call   sub_4_106D
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 6], 1eh
        jmp    NEAR PTR loc_4_43E8
loc_4_44AF:
        mov    ecx, dword ptr [ebp + 10h]
        mov    esi, 0ffffh
        mov    word ptr [ecx + 6], 20h
        push   esi
        push   7eh
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_44C6:
        push   esi
        push   7fh
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_44D1:
        jmp    NEAR PTR loc_4_43E8
loc_4_44D6:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_4_43E8
loc_4_44E5:
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 10h]
        mov    ax, word ptr [ecx]
        mov    edx, dword ptr [ebp + 10h]
        and    eax, 0fff8h
        mov    ecx, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 151eh]
        mov    dword ptr [ecx + 4], eax
        movzx  edx, word ptr [edx + 1526h]
        movzx  eax, word ptr [esi + 1528h]
        shl    edx, 10h
        or     edx, eax
        mov    dword ptr [ecx + 8], edx
        jmp    NEAR PTR loc_4_43E8
loc_4_4521:
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx], eax
        jmp    NEAR PTR loc_4_43E8
loc_4_4531:
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edx], eax
        jmp    NEAR PTR loc_4_43E8
loc_4_4541:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [ebp + 14h]
        movsd
        movsd
        jmp    NEAR PTR loc_4_43E8
loc_4_4550:
        push   dword ptr [ebp + 14h]
        call   sub_4_3E2F
        add    esp, 4
        jmp    NEAR PTR loc_4_43EA
loc_4_4560:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_4_3E50
        add    esp, 8
        jmp    NEAR PTR loc_4_43EA
loc_4_4573:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_4_3F95
        add    esp, 8
        jmp    NEAR PTR loc_4_43EA
loc_4_4586:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_4_4002
        add    esp, 8
        jmp    NEAR PTR loc_4_43EA
loc_4_4599:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_45BF
        db  0A1h, 000h, 000h, 000h, 000h
        sub    eax, dword ptr [ebp + 10h]
        cmp    eax, -24h
        jne    SHORT loc_4_45BF
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_4_43E8
loc_4_45BF:
        mov    eax, 8007h
        jmp    NEAR PTR loc_4_43EA
loc_4_45C9:
        mov    ecx, dword ptr [ebp + 10h]
        cmp    dword ptr [ecx + 152ah], 0
        je     SHORT loc_4_45EF
        mov    ax, word ptr [ecx + 6]
        mov    ecx, dword ptr [ebp + 14h]
        sub    ax, 1eh
        cmp    ax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_4_43E8
loc_4_45EF:
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], 0
        jmp    NEAR PTR loc_4_43E8
loc_4_45FD:
        push   dword ptr [ebp + 10h]
        call   sub_4_4220
        add    esp, 4
        jmp    NEAR PTR loc_4_43EA
loc_4_460D:
        push   dword ptr [ebp + 10h]
        call   sub_4_423F
        add    esp, 4
        jmp    NEAR PTR loc_4_43EA
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
sub_4_46CD:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], ebx
        mov    dword ptr [ebp - 4], ebx
        lea    edi, [esi + 42h]
loc_4_46E4:
        cmp    word ptr [edi], 2000h
        jae    SHORT loc_4_4718
        inc    bx
        push   80h
        mov    eax, dword ptr [ebp - 4]
        or     eax, 2a0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4701:
        mov    word ptr [edi], 0ffffh
        cmp    byte ptr [edi + 17h], 0
        je     SHORT loc_4_4718
        mov    eax, dword ptr [esi + 18h]
        dec    eax
        mov    dword ptr [esi + 18h], eax
        cmp    eax, 2
        jbe    SHORT loc_4_4724
loc_4_4718:
        add    edi, 20h
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 1eh
        jb     SHORT loc_4_46E4
loc_4_4724:
        test   bx, bx
        jne    SHORT loc_4_4733
        mov    eax, 7
        jmp    NEAR PTR loc_4_4D55
loc_4_4733:
        xor    edi, edi
        lea    ebx, [esi + 42h]
loc_4_4738:
        cmp    word ptr [ebx], 0ffffh
        jne    SHORT loc_4_47AF
        push   0
        mov    eax, edi
        or     al, 60h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_474C:
        push   0
        mov    eax, edi
        or     al, 40h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4759:
        push   0
        mov    eax, edi
        or     al, 0c0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4766:
        push   0
        mov    eax, edi
        or     al, 0e0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4773:
        push   40000000h
        mov    eax, edi
        or     al, 20h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4783:
        push   40000000h
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_478F:
        test   edi, 1
        je     SHORT loc_4_479E
        push   7000000h
        jmp    SHORT loc_4_47A3
loc_4_479E:
        push   5000000h
loc_4_47A3:
        mov    eax, edi
        or     ah, 2
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_47AF:
        add    ebx, 20h
        inc    edi
        cmp    edi, 1eh
        jb     NEAR PTR loc_4_4738
        mov    dword ptr [ebp - 18h], 1
        mov    edi, 200000h
        mov    ebx, 33ah
        push   edi
        push   237h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_47D9:
        push   0
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_47E2:
        push   edi
        push   235h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_47EE:
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_47F5:
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_47FC:
        test   ax, ax
        jne    NEAR PTR loc_4_4CE2
        push   edi
        push   237h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4811:
        push   0aaaah
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_481D:
        push   edi
        push   235h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4829:
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4830:
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4837:
        cmp    ax, 0aaaah
        jne    NEAR PTR loc_4_4CE2
        push   edi
        push   237h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_484D:
        push   5555h
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4859:
        push   edi
        push   235h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4865:
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_486C:
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4873:
        cmp    ax, 5555h
        jne    NEAR PTR loc_4_4CE2
        mov    eax, dword ptr [esi + 8]
        cmp    eax, 80000h
        ja     SHORT loc_4_4892
        je     SHORT loc_4_48D1
        cmp    eax, 40000h
        je     SHORT loc_4_48C8
        jmp    SHORT loc_4_48BC
loc_4_4892:
        cmp    eax, 100000h
        je     SHORT loc_4_48DA
        cmp    eax, 200000h
        je     SHORT loc_4_48E3
        cmp    eax, 400000h
        je     SHORT loc_4_48EC
        cmp    eax, 800000h
        je     SHORT loc_4_48F5
        cmp    eax, 0dfffe0h
        je     SHORT loc_4_48F5
        cmp    eax, 0e00000h
        je     SHORT loc_4_48F5
loc_4_48BC:
        mov    dword ptr [ebp - 18h], 5
        jmp    NEAR PTR loc_4_4CE2
loc_4_48C8:
        mov    dword ptr [ebp - 8], 9
        jmp    SHORT loc_4_48FC
loc_4_48D1:
        mov    dword ptr [ebp - 8], 13h
        jmp    SHORT loc_4_48FC
loc_4_48DA:
        mov    dword ptr [ebp - 8], 14h
        jmp    SHORT loc_4_48FC
loc_4_48E3:
        mov    dword ptr [ebp - 8], 15h
        jmp    SHORT loc_4_48FC
loc_4_48EC:
        mov    dword ptr [ebp - 8], 16h
        jmp    SHORT loc_4_48FC
loc_4_48F5:
        mov    dword ptr [ebp - 8], 17h
loc_4_48FC:
        push   8000h
        call   sub_4_0D74
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        test   eax, eax
        jne    SHORT loc_4_491C
        mov    dword ptr [ebp - 18h], 6
        jmp    NEAR PTR loc_4_4CE2
loc_4_491C:
        xor    edi, edi
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ebp - 2], di
        push   200000h
        push   237h
        mov    dword ptr [ebp - 0ch], ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4938:
        push   edi
        push   33ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4944:
        mov    dword ptr [ebp - 14h], edi
        cmp    dword ptr [ebp - 8], edi
        je     NEAR PTR loc_4_49D5
loc_4_4950:
        lea    edi, [edi*2 + 1]
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, edi
        cmp    dword ptr [ebp - 14h], eax
        jae    SHORT loc_4_49C6
        sub    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 18h], eax
loc_4_4967:
        inc    word ptr [ebp - 2]
        lea    eax, [ebx + 200000h]
        push   eax
        push   235h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_497D:
        push   33ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4988:
        push   33ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4993:
        mov    ecx, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 0ch], 2
        mov    word ptr [ecx], ax
        lea    eax, [ebx + 200000h]
        push   eax
        add    ebx, ebx
        push   237h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_49B1:
        movzx  ecx, word ptr [ebp - 2]
        push   ecx
        push   33ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_49C1:
        dec    dword ptr [ebp - 18h]
        jne    SHORT loc_4_4967
loc_4_49C6:
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 14h], eax
        jb     NEAR PTR loc_4_4950
loc_4_49D5:
        xor    edi, edi
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [ebp - 2], di
        push   200000h
        push   235h
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 18h], 2
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_49F8:
        push   33ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4A03:
        push   33ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4A0E:
        test   ax, ax
        jne    NEAR PTR loc_4_4CE2
        mov    dword ptr [ebp - 14h], edi
        cmp    dword ptr [ebp - 8], edi
        je     NEAR PTR loc_4_4AD7
loc_4_4A23:
        lea    edi, [edi*2 + 1]
        mov    eax, dword ptr [ebp - 14h]
        mov    ebx, edi
        mov    dword ptr [ebp - 1ch], eax
        cmp    eax, dword ptr [ebp - 8]
        jae    NEAR PTR loc_4_4AC8
loc_4_4A3B:
        mov    dword ptr [ebp - 20h], 80h
loc_4_4A42:
        push   234h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4A4D:
        test   eax, 80000000h
        je     SHORT loc_4_4A59
        dec    dword ptr [ebp - 20h]
        jne    SHORT loc_4_4A42
loc_4_4A59:
        inc    word ptr [ebp - 2]
        lea    eax, [ebx + 200000h]
        push   eax
        push   235h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4A6F:
        push   33ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4A7A:
        push   33ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4A85:
        cmp    word ptr [ebp - 2], ax
        jne    NEAR PTR loc_4_4CE2
        lea    eax, [ebx + 200000h]
        push   eax
        push   237h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4AA1:
        mov    ecx, dword ptr [ebp - 0ch]
        add    ebx, ebx
        add    dword ptr [ebp - 0ch], 2
        movzx  eax, word ptr [ecx]
        push   eax
        push   33ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4AB9:
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 1ch]
        cmp    dword ptr [ebp - 1ch], eax
        jb     NEAR PTR loc_4_4A3B
loc_4_4AC8:
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 14h], eax
        jb     NEAR PTR loc_4_4A23
loc_4_4AD7:
        mov    ebx, dword ptr [ebp - 10h]
        mov    edi, 200000h
        mov    dword ptr [ebp - 18h], 4
        push   edi
        push   237h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4AF2:
        push   edi
        push   235h
        push   esi
        mov    edi, 33ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4B03:
        push   33ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4B0E:
        mov    dword ptr [ebp - 4], 2000h
loc_4_4B15:
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4B1C:
        mov    word ptr [ebx], ax
        push   0
        add    ebx, 2
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4B2B:
        dec    dword ptr [ebp - 4]
        jne    SHORT loc_4_4B15
        mov    ebx, 200000h
        mov    edi, 33ah
loc_4_4B3A:
        push   ebx
        push   237h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4B46:
        push   0ffffh
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4B52:
        push   ebx
        push   235h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4B5E:
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4B65:
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4B6C:
        cmp    ax, 0ffffh
        jne    NEAR PTR loc_4_4CE2
        push   ebx
        push   237h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4B82:
        push   0
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4B8B:
        push   ebx
        push   235h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4B97:
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4B9E:
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4BA5:
        test   ax, ax
        jne    NEAR PTR loc_4_4CE2
        inc    ebx
        cmp    ebx, 202000h
        jb     SHORT loc_4_4B3A
        mov    ebx, dword ptr [ebp - 10h]
        push   200000h
        push   237h
        mov    edi, 2000h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4BCF:
        movzx  ecx, word ptr [ebx]
        mov    eax, ebx
        push   ecx
        add    ebx, 2
        push   33ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4BE3:
        dec    edi
        jne    SHORT loc_4_4BCF
        mov    ecx, dword ptr [ebp - 10h]
        mov    edi, 200000h
        push   edi
        mov    dword ptr [ebp - 0ch], ecx
        mov    dword ptr [ebp - 18h], 3
        mov    word ptr [ebp - 2], 0
        push   237h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4C0A:
        push   edi
        push   235h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4C16:
        push   33ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4C21:
        mov    eax, dword ptr [esi + 8]
        add    eax, edi
        cmp    eax, edi
        jbe    NEAR PTR loc_4_4CDB
        mov    ebx, 33ah
loc_4_4C33:
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4C3A:
        mov    ecx, dword ptr [ebp - 0ch]
        push   5a5ah
        add    dword ptr [ebp - 0ch], 2
        mov    word ptr [ecx], ax
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4C50:
        inc    word ptr [ebp - 2]
        cmp    word ptr [ebp - 2], 4000h
        jne    SHORT loc_4_4CCA
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [edi - 3fffh]
        push   eax
        push   237h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4C74:
        lea    eax, [edi - 3fffh]
        push   eax
        push   235h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4C86:
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4C8D:
        mov    word ptr [ebp - 2], 0
loc_4_4C93:
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4C9A:
        cmp    ax, 5a5ah
        jne    SHORT loc_4_4CE2
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 0ch], 2
        movzx  ecx, word ptr [eax]
        push   ecx
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4CB2:
        inc    word ptr [ebp - 2]
        cmp    word ptr [ebp - 2], 4000h
        jb     SHORT loc_4_4C93
        mov    word ptr [ebp - 2], 0
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 0ch], ecx
loc_4_4CCA:
        inc    edi
        mov    eax, dword ptr [esi + 8]
        add    eax, 200000h
        cmp    eax, edi
        ja     NEAR PTR loc_4_4C33
loc_4_4CDB:
        mov    dword ptr [ebp - 18h], 0
loc_4_4CE2:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_4_4CF3
        push   dword ptr [ebp - 10h]
        call   sub_4_0D91
        add    esp, 4
loc_4_4CF3:
        xor    edi, edi
        lea    ebx, [esi + 42h]
loc_4_4CF8:
        cmp    word ptr [ebx], 0ffffh
        jne    SHORT loc_4_4D49
        mov    word ptr [ebx], 0ffeh
        push   0
        mov    byte ptr [ebx + 17h], 1
        mov    eax, edi
        inc    dword ptr [esi + 18h]
        or     ah, 2
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4D19:
        push   1d8h
        mov    eax, edi
        or     al, 0c0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4D29:
        push   1e0h
        mov    eax, edi
        or     al, 0e0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4D39:
        push   48000000h
        mov    eax, edi
        or     al, 20h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4D49:
        add    ebx, 20h
        inc    edi
        cmp    edi, 1eh
        jb     SHORT loc_4_4CF8
        mov    eax, dword ptr [ebp - 18h]
loc_4_4D55:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
_PTEXT ends
        end
