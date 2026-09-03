; vnbt_lcode_01_01.asm -- VNBT object 1 segment _LTEXT  (part 1/5)
.386p
_LTEXT segment dword public use32 'LCODE'

        db  0CDh, 020h, 001h, 080h, 088h, 004h, 0CDh, 020h, 002h, 080h, 088h, 004h
sub_1_000C:
        db  0CDh, 020h, 03Fh, 080h, 001h, 000h
sub_1_0012:
        db  0CDh, 020h, 003h, 080h, 088h, 004h
sub_1_0018:
        db  0CDh, 020h, 004h, 080h, 088h, 004h
sub_1_001E:
        db  0CDh, 020h, 00Dh, 080h, 040h, 000h
sub_1_0024:
        db  0CDh, 020h, 00Eh, 080h, 040h, 000h
sub_1_002A:
        db  0CDh, 020h, 011h, 080h, 040h, 000h
sub_1_0030:
        db  0CDh, 020h, 005h, 080h, 088h, 004h
        db  0CDh, 020h, 00Dh, 080h, 088h, 004h
sub_1_003C:
        db  0CDh, 020h, 006h, 080h, 088h, 004h
sub_1_0042:
        db  0A1h, 068h, 02Eh, 001h, 000h
        db  0CDh, 020h, 009h, 080h, 088h, 004h
        db  0A1h, 068h, 02Eh, 001h, 000h
        db  0CDh, 020h, 00Ch, 080h, 088h, 004h
sub_1_0058:
        db  0A1h, 068h, 02Eh, 001h, 000h
        db  0CDh, 020h, 00Bh, 080h, 088h, 004h
        add    byte ptr [ebx], dh
        rcl    byte ptr [eax + 53h], 0e8h
        add    eax, dword ptr [edi]
        add    byte ptr [eax], al
        add    esp, 8
        ret
sub_1_0071:
        db  0CDh, 020h, 000h, 000h, 09Ah, 004h
        jae    SHORT loc_1_007F
        mov    eax, 1ah
        ret
loc_1_007F:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0CDh, 020h, 001h, 000h, 09Ah, 004h
        add    esp, 10h
        pop    ebp
        ret
VNBT_Service_00:
        mov    ax, 100h
        clc
        ret
loc_1_00A3:
        call   sub_1_35C0
        clc
        ret
VNBT_Control:
        cmp    eax, 1
        db  00Fh, 084h, 099h, 0FFh, 0FDh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 090h, 0FFh, 0FDh, 0FFh
        cmp    eax, 3
        je     SHORT loc_1_00A3
        cmp    eax, 23h
        je     SHORT loc_1_00C8
        clc
        ret
loc_1_00C8:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esi + 0ch]
        cmp    ecx, 0
        je     SHORT loc_1_00D8
        cmp    ecx, -1
        jne    SHORT loc_1_00DC
loc_1_00D8:
        xor    eax, eax
        jmp    SHORT loc_1_0152
loc_1_00DC:
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_1_00F6
        push   dword ptr [esi + 14h]
        push   eax
        call   sub_1_03BC
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_0156
        mov    dword ptr [esi + 10h], eax
loc_1_00F6:
        mov    eax, dword ptr [esi + 18h]
        or     eax, eax
        je     SHORT loc_1_0110
        push   dword ptr [esi + 1ch]
        push   eax
        call   sub_1_03BC
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_0156
        mov    dword ptr [esi + 18h], eax
loc_1_0110:
        mov    eax, 0
        push   eax
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    eax, dword ptr [esi + 10h]
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        call   sub_1_3230
        add    esp, 18h
        push   eax
        push   dword ptr [esi + 14h]
        push   dword ptr [esi + 10h]
        call   sub_1_0403
        add    esp, 8
        pop    eax
        push   eax
        push   dword ptr [esi + 1ch]
        push   dword ptr [esi + 18h]
        call   sub_1_0403
        add    esp, 8
        pop    eax
loc_1_0152:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0156:
        mov    eax, 6f8h
        jmp    SHORT loc_1_0152
sub_1_015D:
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0B8h, 08Ah, 001h, 000h, 000h
        push   eax
        db  0CDh, 020h, 04Eh, 000h, 033h, 000h
        add    esp, 0ch
        cmp    eax, 0
        je     SHORT loc_1_0182
        mov    eax, 1
        jmp    SHORT loc_1_0184
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
        db  0B8h, 086h, 001h, 000h, 000h
        push   eax
        db  0CDh, 020h, 004h, 000h, 033h, 000h
        add    esp, 0ch
        db  08Bh, 035h, 086h, 001h, 000h, 000h
        mov    edi, 0
        cmp    eax, 0
        je     SHORT loc_1_01C0
        mov    ecx, 1
        jmp    NEAR PTR loc_1_0240
loc_1_01C0:
        cmp    edi, esi
        je     SHORT loc_1_0222
        xor    eax, eax
        push   eax
        push   esi
        db  0B8h, 086h, 001h, 000h, 000h
        push   eax
        db  0CDh, 020h, 005h, 000h, 033h, 000h
        add    esp, 0ch
        db  08Bh, 03Dh, 086h, 001h, 000h, 000h
        cmp    eax, 0
        je     SHORT loc_1_01F0
        cmp    eax, 0dh
        je     SHORT loc_1_01EE
        mov    ecx, 2
        jmp    SHORT loc_1_0240
loc_1_01EE:
        mov    edi, esi
loc_1_01F0:
        xor    eax, eax
        push   eax
        push   edi
        db  0CDh, 020h, 00Ch, 000h, 033h, 000h
        add    esp, 8
        cmp    eax, 0
        je     SHORT loc_1_0209
        mov    ecx, 4
        jmp    SHORT loc_1_0240
loc_1_0209:
        xor    eax, eax
        push   eax
        push   edi
        db  0CDh, 020h, 00Dh, 000h, 033h, 000h
        add    esp, 8
        cmp    eax, 0
        je     SHORT loc_1_01C0
        mov    ecx, 5
        jmp    SHORT loc_1_0240
loc_1_0222:
        xor    eax, eax
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0CDh, 020h, 033h, 000h, 033h, 000h
        add    esp, 8
        cmp    eax, 0
        je     SHORT loc_1_023E
        mov    ecx, 6
        jmp    SHORT loc_1_0240
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
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 3
        je     SHORT loc_1_027A
        repe cmpsb
        jne    SHORT loc_1_029C
loc_1_027A:
        mov    ecx, dword ptr [ebp + 10h]
        shr    ecx, 2
        je     SHORT loc_1_0286
        repe cmpsd
        jne    SHORT loc_1_028F
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
        repe cmpsb
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
        movzx  edi, word ptr [ebp + 18h]
        movzx  eax, word ptr [ebp + 10h]
        push   eax
        movzx  eax, word ptr [ebp + 38h]
        push   eax
        push   ebx
        call   sub_1_0374
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_1_0368
        or     eax, eax
        je     SHORT loc_1_02E6
        movzx  ecx, word ptr [ebp]
        push   ecx
        push   eax
        call   sub_1_03BC
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_0368
loc_1_02E6:
        mov    esi, eax
        movzx  eax, word ptr [ebp + 1ch]
        push   eax
        movzx  eax, word ptr [ebp + 14h]
        push   eax
        push   ebx
        call   sub_1_0374
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_1_0368
        or     eax, eax
        je     SHORT loc_1_0316
        movzx  ebx, word ptr [ebp + 4]
        push   ebx
        push   eax
        call   sub_1_03BC
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_0368
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
        add    esp, 18h
        pop    edx
        pop    esi
        mov    word ptr [ebp + 1ch], ax
        or     esi, esi
        je     SHORT loc_1_034F
        push   edx
        movzx  ecx, word ptr [ebp]
        push   ecx
        push   esi
        call   sub_1_0403
        add    esp, 8
        pop    edx
loc_1_034F:
        or     edx, edx
        je     SHORT loc_1_0361
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
        mov    word ptr [ebp + 1ch], ax
        jmp    SHORT loc_1_0361
        db  000h, 000h, 000h
sub_1_0374:
        push   ebx
        push   ecx
        push   edx
        push   esi
        mov    ebx, dword ptr [esp + 14h]
        movzx  eax, word ptr [esp + 18h]
        movzx  esi, word ptr [esp + 1ch]
        or     eax, eax
        je     SHORT loc_1_03B0
        test   dword ptr [ebx], 20h
        je     SHORT loc_1_03B5
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_1_03B0
        add    eax, esi
        cmp    eax, 110000h
        jae    SHORT loc_1_03B0
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
        jmp    SHORT loc_1_03AD
sub_1_03BC:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_1_03FB
        or     ebx, ebx
        je     SHORT loc_1_03FB
        movzx  ecx, ax
        and    ch, 0fh
        mov    esi, ecx
        add    ebx, ecx
        add    ebx, 0fffh
        shr    ebx, 0ch
        shr    eax, 0ch
        push   40000000h
        push   ebx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_03FF
        add    eax, esi
loc_1_03FB:
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_1_03FF:
        xor    eax, eax
        jmp    SHORT loc_1_03FB
sub_1_0403:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_1_043E
        or     ebx, ebx
        je     SHORT loc_1_043E
        movzx  ecx, ax
        and    ch, 0fh
        add    ebx, ecx
        add    ebx, 0fffh
        shr    ebx, 0ch
        shr    eax, 0ch
        push   40000000h
        push   ebx
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_0447
loc_1_043E:
        mov    eax, 1
loc_1_0443:
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_1_0447:
        xor    eax, eax
        jmp    SHORT loc_1_0443
        db  053h, 057h, 056h, 08Bh, 044h, 024h, 010h, 08Bh, 05Ch, 024h, 014h, 00Bh, 0C0h, 074h, 02Bh, 00Bh
        db  0DBh, 074h, 027h, 00Fh, 0B7h, 0C8h, 080h, 0E5h, 00Fh, 003h, 0D9h, 081h, 0C3h, 0FFh, 00Fh, 000h
        db  000h, 0C1h, 0EBh, 00Ch, 0C1h, 0E8h, 00Ch, 08Bh, 0CBh, 06Ah, 000h, 053h, 050h, 0CDh, 020h, 067h
        db  000h, 001h, 000h, 083h, 0C4h, 00Ch, 03Bh, 0C1h, 075h, 009h, 0B8h, 001h, 000h, 000h, 000h, 05Eh
        db  05Fh, 05Bh, 0C3h, 033h, 0C0h, 0EBh, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h
sub_1_04A0:
        mov    ax, word ptr [esp + 8]
        add    ax, 100h
        push   eax
        call   sub_1_35F0
        add    esp, 4
        db  0A3h, 0C0h, 02Eh, 001h, 000h
        or     eax, eax
        jne    SHORT loc_1_04C0
        sub    eax, eax
        ret
        db  08Bh, 0FFh
loc_1_04C0:
        db  0A3h, 0C0h, 02Eh, 001h, 000h
        add    eax, 100h
        db  0A3h, 0C4h, 02Eh, 001h, 000h
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
        mov    esi, dword ptr [esp + 14h]
        mov    edi, edi
loc_1_0520:
        push   esi
        call   sub_1_05B0
        xor    ecx, ecx
        mov    cl, al
        add    esp, 4
        or     ecx, ecx
        je     SHORT loc_1_055C
        cmp    ecx, 0ah
        je     SHORT loc_1_0558
        cmp    ecx, 0dh
        je     SHORT loc_1_0520
        mov    ecx, dword ptr [esi + 8]
        mov    byte ptr [edi + ecx], al
        cmp    edi, 0ffh
        jne    SHORT loc_1_056A
        mov    eax, dword ptr [esi + 8]
        mov    byte ptr [edi + eax], 0
        dec    edi
        mov    ebp, 1
        jmp    SHORT loc_1_056A
loc_1_0558:
        or     edi, edi
        je     SHORT loc_1_0520
loc_1_055C:
        mov    eax, dword ptr [esi + 8]
        mov    byte ptr [edi + eax], 0
        mov    ebp, 1
        mov    ebx, ebp
loc_1_056A:
        or     ebp, ebp
        jne    SHORT loc_1_0571
        inc    edi
        jmp    SHORT loc_1_0520
loc_1_0571:
        or     ebx, ebx
        jne    SHORT loc_1_0586
loc_1_0575:
        push   esi
        call   sub_1_05B0
        add    esp, 4
        or     al, al
        je     SHORT loc_1_0586
        cmp    al, 0ah
        jne    SHORT loc_1_0575
loc_1_0586:
        or     edi, edi
        je     SHORT loc_1_059B
        mov    eax, dword ptr [esp + 18h]
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
        mov    ecx, dword ptr [esi + 14h]
        cmp    dword ptr [esi + 18h], ecx
        jbe    SHORT loc_1_05D0
        lea    eax, [ecx + 1]
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [esi + 0ch]
        mov    al, byte ptr [ecx + eax]
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_05D0:
        cmp    dword ptr [esi + 10h], 0
        je     SHORT loc_1_05DA
        sub    al, al
        pop    esi
        ret
loc_1_05DA:
        db  0A1h, 0C0h, 02Eh, 001h, 000h
        push   eax
        push   100h
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_64B2
        add    esp, 0ch
        mov    dword ptr [esi + 14h], 0
        cmp    eax, 100h
        jae    SHORT loc_1_0605
        mov    dword ptr [esi + 10h], 1
loc_1_0605:
        mov    dword ptr [esi + 18h], eax
        or     eax, eax
        je     SHORT loc_1_061A
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [esi + 14h], 1
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
        push   100h
        call   sub_1_001E
        add    esp, 4
        mov    ebx, eax
        db  0A1h, 0C8h, 02Eh, 001h, 000h
        db  0FFh, 005h, 0C8h, 02Eh, 001h, 000h
        or     eax, eax
        jne    SHORT loc_1_069D
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        db  08Bh, 015h, 0C4h, 02Eh, 001h, 000h
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        or     ebx, ebx
        je     SHORT loc_1_069D
        db  083h, 03Dh, 0C0h, 02Eh, 001h, 000h, 000h
        je     SHORT loc_1_069D
        push   edx
        call   sub_1_6480
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_069D
        push   20h
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_06B6
        push   esi
        call   sub_1_64EC
        add    esp, 4
loc_1_069D:
        db  0FFh, 00Dh, 0C8h, 02Eh, 001h, 000h
        or     ebx, ebx
        je     SHORT loc_1_06B0
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
        db  08Bh, 00Dh, 0C0h, 02Eh, 001h, 000h
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 18h], 0
        mov    dword ptr [eax + 14h], 0
        mov    dword ptr [eax + 10h], 0
        db  08Bh, 00Dh, 0C8h, 02Eh, 001h, 000h
        dec    ecx
        db  089h, 00Dh, 0C8h, 02Eh, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0700:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   100h
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esi + 1ch], eax
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_0721
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_1_0721:
        mov    esi, dword ptr [esi + 0ch]
        mov    ecx, 40h
        rep movsd
        mov    eax, 1
        pop    edi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0740:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, 40h
        mov    edi, dword ptr [eax + 0ch]
        mov    esi, dword ptr [eax + 1ch]
        rep movsd
        mov    eax, dword ptr [eax + 1ch]
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
        mov    edi, dword ptr [esp + 1ch]
        or     edi, edi
        jne    SHORT loc_1_0790
        mov    al, 39h
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
        jne    SHORT loc_1_07A9
        mov    al, 23h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_07A9:
        cmp    byte ptr [ebp + 52h], 0
        jne    SHORT loc_1_07B9
        mov    al, 23h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_07B9:
        mov    al, byte ptr [edi]
        and    al, 80h
        cmp    al, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        jne    SHORT loc_1_0842
        push   1ch
        call   sub_1_001E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_07E4
        mov    al, 38h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_07E4:
        mov    dword ptr [esi + 8], 8121994h
        mov    dword ptr [esi + 4], esi
        mov    dword ptr [esi], esi
        mov    dword ptr [esi + 0ch], edi
        push   0ch
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esi + 10h], eax
        or     eax, eax
        jne    SHORT loc_1_0817
        push   esi
        call   sub_1_0024
        add    esp, 4
        mov    al, 38h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_0817:
        mov    dword ptr [esi + 14h], 0
        mov    dword ptr [esi + 18h], 0
        db  0C7h, 006h, 0ACh, 032h, 001h, 000h
        db  0A1h, 0B0h, 032h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 0B0h, 032h, 001h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 0B0h, 032h, 001h, 000h
        jmp    SHORT loc_1_0846
loc_1_0842:
        mov    esi, dword ptr [esp + 14h]
loc_1_0846:
        mov    byte ptr [edi + 1], 0ffh
        mov    byte ptr [edi + 31h], 0ffh
        mov    al, byte ptr [edi]
        and    eax, 7fh
        cmp    eax, 70h
        jg     SHORT loc_1_0870
        je     NEAR PTR loc_1_0943
        sub    eax, 10h
        cmp    eax, 26h
        ja     SHORT loc_1_088C
        shl    eax, 2
        db  0FFh, 0A0h, 0ECh, 009h, 000h, 000h
        db  090h
loc_1_0870:
        cmp    eax, 71h
        jl     SHORT loc_1_088C
        cmp    eax, 72h
        jle    SHORT loc_1_08B8
        cmp    eax, 78h
        je     NEAR PTR loc_1_0952
        cmp    eax, 79h
        je     NEAR PTR loc_1_0943
loc_1_088C:
        mov    bl, 3
        jmp    NEAR PTR loc_1_095E
loc_1_0893:
        push   edi
        push   ebp
        call   sub_1_0DB0
        jmp    NEAR PTR loc_1_0959
        db  090h
loc_1_08A0:
        push   edi
        push   ebp
        call   sub_1_1550
        jmp    NEAR PTR loc_1_0959
loc_1_08AC:
        push   edi
        push   ebp
        call   sub_1_1410
        jmp    NEAR PTR loc_1_0959
loc_1_08B8:
        push   edi
        push   ebp
        call   sub_1_0E40
        jmp    NEAR PTR loc_1_0959
loc_1_08C4:
        push   1
        push   edi
        push   ebp
        call   sub_1_1210
        add    esp, 0ch
        jmp    NEAR PTR loc_1_095C
loc_1_08D5:
        push   edi
        push   ebp
        call   sub_1_1010
        jmp    SHORT loc_1_0959
loc_1_08DE:
        push   edi
        push   ebp
        call   sub_1_0BA0
        add    esp, 8
        and    eax, 0ffh
        push   eax
        call   sub_1_0AD0
        add    esp, 4
        jmp    SHORT loc_1_095C
loc_1_08F8:
        push   edi
        push   ebp
        call   sub_1_0CA0
        jmp    SHORT loc_1_0959
loc_1_0901:
        push   edi
        push   ebp
        call   sub_1_15E0
        jmp    SHORT loc_1_0959
loc_1_090A:
        push   edi
        push   ebp
        call   sub_1_16D0
        jmp    SHORT loc_1_0959
loc_1_0913:
        push   edi
        push   ebp
        call   sub_1_1F30
        jmp    SHORT loc_1_0959
        db  08Dh, 064h, 024h, 000h
loc_1_0920:
        mov    eax, dword ptr [esp + 20h]
        push   eax
        push   edi
        push   ebp
        call   sub_1_1970
        add    esp, 0ch
        jmp    SHORT loc_1_095C
loc_1_0931:
        push   edi
        push   ebp
        call   sub_1_1B20
        jmp    SHORT loc_1_0959
loc_1_093A:
        push   edi
        push   ebp
        call   sub_1_21C0
        jmp    SHORT loc_1_0959
loc_1_0943:
        push   0
        push   0
        push   edi
        call   sub_1_25A0
        add    esp, 0ch
        jmp    SHORT loc_1_095E
loc_1_0952:
        push   edi
        push   ebp
        call   sub_1_1A80
loc_1_0959:
        add    esp, 8
loc_1_095C:
        mov    bl, al
loc_1_095E:
        cmp    bl, 0ffh
        je     SHORT loc_1_0980
        or     bl, bl
        je     SHORT loc_1_0980
        mov    byte ptr [edi + 1], bl
        mov    byte ptr [edi + 31h], bl
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_09DF
        mov    dword ptr [esi + 14h], 1
        jmp    SHORT loc_1_0982
        db  08Dh, 049h, 000h
loc_1_0980:
        sub    bl, bl
loc_1_0982:
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_09DF
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_1_09C0
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [esi + 18h], 1
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
        mov    eax, dword ptr [esi + 10h]
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
        db  000h, 0A0h, 008h, 000h, 000h, 0ACh
        or     byte ptr [eax], al
        db  000h, 08Ch, 008h, 000h, 000h, 0B8h, 008h
        add    byte ptr [eax], al
        les    ecx, ptr [eax]
        add    byte ptr [eax], al
        aad    8
        add    byte ptr [eax], al
        db  0B8h, 008h, 000h, 000h, 08Ch
        or     byte ptr [eax], al
        db  000h, 08Ch, 008h, 000h, 000h, 08Ch, 008h
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
        db  000h, 0DEh
        or     byte ptr [eax], al
        db  000h, 0F8h
        or     byte ptr [eax], al
        db  000h, 08Ch, 008h, 000h, 000h, 08Ch, 008h
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
        jne    SHORT loc_1_0AA0
        sub    eax, eax
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_0AA0:
        sub    ecx, ecx
        db  0BAh, 0C4h, 033h, 001h, 000h
        mov    bl, byte ptr [eax + 30h]
loc_1_0AAA:
        mov    eax, dword ptr [edx]
        cmp    byte ptr [eax + 51h], bl
        je     SHORT loc_1_0AC1
        add    edx, 4
        inc    ecx
        db  081h, 0FAh, 0E4h, 033h, 001h, 000h
        jb     SHORT loc_1_0AAA
        sub    eax, eax
        pop    ebx
        ret
loc_1_0AC1:
        db  08Bh, 004h, 08Dh, 0C4h, 033h, 001h, 000h
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0AD0:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        jne    SHORT loc_1_0AE0
        sub    al, al
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_0AE0:
        cmp    eax, 0ffh
        jne    SHORT loc_1_0AF0
        mov    al, 0ffh
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0AF0:
        cmp    eax, 0c0000206h
        jg     SHORT loc_1_0B40
        je     NEAR PTR loc_1_0B89
        cmp    eax, 0c00000beh
        jg     SHORT loc_1_0B25
        je     SHORT loc_1_0B7B
        cmp    eax, 0c0000035h
        je     SHORT loc_1_0B72
        cmp    eax, 0c0000043h
        je     SHORT loc_1_0B72
        cmp    eax, 0c00000bch
        je     SHORT loc_1_0B75
        cmp    eax, 0c00000bdh
        je     SHORT loc_1_0B78
        mov    al, 40h
        ret
loc_1_0B25:
        cmp    eax, 0c00000c1h
        je     SHORT loc_1_0B80
        cmp    eax, 0c00000cah
        je     SHORT loc_1_0B83
        cmp    eax, 0c0000120h
        je     SHORT loc_1_0B86
        mov    al, 40h
        ret
        db  08Dh, 049h, 000h
loc_1_0B40:
        cmp    eax, 9
        jg     SHORT loc_1_0B60
        je     SHORT loc_1_0B96
        cmp    eax, 0c000020ch
        je     SHORT loc_1_0B8C
        cmp    eax, 0ca000001h
        je     SHORT loc_1_0B90
        cmp    eax, 1
        je     SHORT loc_1_0B93
        mov    al, 40h
        ret
        db  08Dh, 049h, 000h
loc_1_0B60:
        cmp    eax, 0fh
        je     SHORT loc_1_0B8C
        cmp    eax, 13h
        je     SHORT loc_1_0B99
        cmp    eax, 15h
        je     SHORT loc_1_0B9C
        mov    al, 40h
        ret
loc_1_0B72:
        mov    al, 0dh
        ret
loc_1_0B75:
        mov    al, 12h
        ret
loc_1_0B78:
        mov    al, 16h
        ret
loc_1_0B7B:
        mov    al, 14h
        ret
        db  08Bh, 0FFh
loc_1_0B80:
        mov    al, 22h
        ret
loc_1_0B83:
        mov    al, 17h
        ret
loc_1_0B86:
        mov    al, 0bh
        ret
loc_1_0B89:
        mov    al, 1
        ret
loc_1_0B8C:
        mov    al, 0ah
        ret
        db  090h
loc_1_0B90:
        mov    al, 0fh
        ret
loc_1_0B93:
        mov    al, 9
        ret
loc_1_0B96:
        mov    al, 6
        ret
loc_1_0B99:
        mov    al, 18h
        ret
loc_1_0B9C:
        mov    al, 5
        ret
        db  090h
sub_1_0BA0:
        sub    esp, 4ch
        push   ebx
        push   esi
        push   edi
        lea    eax, [esp + 0ch]
        push   1
        mov    esi, dword ptr [esp + 64h]
        push   eax
        mov    al, byte ptr [esi + 3]
        mov    edi, dword ptr [esp + 64h]
        push   eax
        push   edi
        call   sub_1_2C70
        add    esp, 10h
        or     al, al
        jne    NEAR PTR loc_1_0C8E
        mov    eax, dword ptr [esp + 0ch]
        cmp    dword ptr [eax + 44h], 0
        je     SHORT loc_1_0BE0
        mov    al, 15h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
        db  08Dh, 049h, 000h
loc_1_0BE0:
        mov    al, byte ptr [esi]
        and    eax, 7fh
        cmp    eax, 22h
        db  0BAh, 03Ch, 02Fh, 001h, 000h
        je     SHORT loc_1_0BF2
        lea    edx, [esi + 0ah]
loc_1_0BF2:
        mov    dword ptr [esp + 3ch], 1
        mov    word ptr [esp + 40h], 12h
        mov    word ptr [esp + 42h], 11h
        mov    word ptr [esp + 44h], 0
        lea    ecx, [esp + 46h]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        lea    eax, [esp + 3ch]
        mov    dword ptr [esp + 34h], 1ah
        mov    dword ptr [esp + 38h], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esp + 14h], eax
        push   esi
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        push   eax
        lea    eax, [esp + 38h]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        call   sub_1_C598
        add    esp, 1ch
        mov    edi, eax
        push   edi
        call   sub_1_0AD0
        add    esp, 4
        mov    bl, al
        or     bl, bl
        je     SHORT loc_1_0C7B
        cmp    bl, 0ffh
        jne    SHORT loc_1_0C8C
loc_1_0C7B:
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        push   eax
        push   edi
        push   esi
        call   sub_1_25A0
        add    esp, 0ch
loc_1_0C8C:
        mov    al, bl
loc_1_0C8E:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0CA0:
        sub    esp, 18h
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 28h]
        mov    cl, byte ptr [esi + 3]
        cmp    cl, 0ffh
        je     NEAR PTR loc_1_0D59
        lea    eax, [esp + 8]
        push   1
        push   eax
        mov    edi, dword ptr [esp + 2ch]
        push   ecx
        push   edi
        call   sub_1_2C70
        add    esp, 10h
        or     al, al
        jne    NEAR PTR loc_1_0DA6
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 44h], 0
        je     SHORT loc_1_0CE4
        mov    al, 15h
        pop    edi
        pop    esi
        add    esp, 18h
        ret
loc_1_0CE4:
        mov    al, byte ptr [esi]
        and    eax, 7fh
        cmp    eax, 23h
        jne    SHORT loc_1_0D10
        lea    eax, [esp + 8]
        push   0
        push   eax
        mov    al, byte ptr [esi + 3]
        push   eax
        push   edi
        call   sub_1_2C70
        add    esp, 10h
        or     al, al
        je     SHORT loc_1_0D10
        mov    al, 17h
        pop    edi
        pop    esi
        add    esp, 18h
        ret
        db  08Bh, 0FFh
loc_1_0D10:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esp + 10h], eax
        push   esi
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        push   eax
        push   0
        lea    eax, [esp + 28h]
        push   0
        push   eax
        call   sub_1_C194
        add    esp, 20h
        cmp    eax, 0ffh
        jne    SHORT loc_1_0D4A
        mov    al, 0ffh
        pop    edi
        pop    esi
        add    esp, 18h
        ret
loc_1_0D4A:
        push   eax
        call   sub_1_0AD0
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 18h
        ret
loc_1_0D59:
        push   1ch
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_0D70
        mov    al, 9
        pop    edi
        pop    esi
        add    esp, 18h
        ret
        db  090h
loc_1_0D70:
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 14h], 0
        mov    dword ptr [eax + 18h], 0
        xor    ecx, ecx
        mov    cx, word ptr [esi + 8]
        mov    edi, dword ptr [esp + 24h]
        mov    dword ptr [eax + 10h], ecx
        mov    ecx, dword ptr [esi + 4]
        add    edi, 34h
        mov    dword ptr [eax + 0ch], ecx
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [eax], edi
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        mov    dword ptr [edi + 4], eax
        mov    al, 0ffh
loc_1_0DA6:
        pop    edi
        pop    esi
        add    esp, 18h
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_0DB0:
        sub    esp, 14h
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 24h]
        mov    dword ptr [esp + 8], 0
        cmp    byte ptr [edi + 1ah], 2ah
        jne    SHORT loc_1_0DD0
        mov    al, 15h
        pop    edi
        pop    esi
        add    esp, 14h
        ret
        db  090h
loc_1_0DD0:
        lea    eax, [esp + 8]
        push   edi
        push   eax
        lea    eax, [esp + 14h]
        mov    esi, dword ptr [esp + 28h]
        push   eax
        push   esi
        call   sub_1_2A90
        add    esp, 10h
        or     al, al
        jne    SHORT loc_1_0E37
        mov    eax, dword ptr [esp + 8]
        push   edi
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        lea    eax, [esp + 18h]
        push   0
        push   eax
        call   sub_1_B238
        add    esp, 14h
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_0E2E
        cmp    edi, 0ffh
        je     SHORT loc_1_0E2E
        mov    eax, dword ptr [esp + 8]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        push   esi
        call   sub_1_3080
        add    esp, 10h
loc_1_0E2E:
        push   edi
        call   sub_1_0AD0
        add    esp, 4
loc_1_0E37:
        pop    edi
        pop    esi
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
sub_1_0E40:
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], 0
        mov    esi, dword ptr [esp + 4ch]
        sub    edi, edi
        lea    ebx, [esi + 32h]
        lea    eax, [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 4ch]
        push   esi
        push   eax
        call   sub_1_2D50
        add    esp, 0ch
        or     al, al
        jne    NEAR PTR loc_1_1007
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 68h], 7
        jne    NEAR PTR loc_1_0FC7
        lea    eax, [esp + 10h]
        push   2
        push   eax
        db  068h, 08Ch, 032h, 001h, 000h
        call   sub_1_FEF4
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_0FC0
        mov    al, byte ptr [esi]
        and    eax, 7fh
        cmp    eax, 17h
        je     SHORT loc_1_0EB9
        cmp    eax, 72h
        je     SHORT loc_1_0EB9
        mov    dword ptr [esp + 40h], 0
        jmp    SHORT loc_1_0ECF
loc_1_0EB9:
        xor    eax, eax
        mov    ax, word ptr [esi + 0ah]
        mov    edi, 80h
        mov    dword ptr [esp + 40h], eax
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [esp + 3ch], eax
loc_1_0ECF:
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax + 1], 0
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        mov    edx, dword ptr [esp + 40h]
        add    edx, eax
        add    eax, dword ptr [esp + 40h]
        mov    ecx, edx
        and    ecx, 0ff00h
        shl    edx, 10h
        or     ecx, edx
        shl    ecx, 8
        mov    edx, eax
        and    edx, 0ff0000h
        shr    edx, 8
        or     ecx, edx
        shr    eax, 18h
        and    eax, 0ffh
        or     ecx, eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [esp + 14h]
        mov    al, byte ptr [eax + 39h]
        or     al, al
        mov    byte ptr [ebx + 0ch], al
        je     SHORT loc_1_0F4B
        db  0C7h, 003h, 084h, 032h, 001h, 000h
        db  0A1h, 088h, 032h, 001h, 000h
        mov    dword ptr [ebx + 4], eax
        db  0A1h, 088h, 032h, 001h, 000h
        mov    dword ptr [eax], ebx
        db  089h, 01Dh, 088h, 032h, 001h, 000h
loc_1_0F4B:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 30h], eax
        mov    dword ptr [esp + 2ch], 4
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [esp + 38h], eax
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        db  0C7h, 044h, 024h, 020h, 0A0h, 025h, 000h, 000h
        mov    dword ptr [esp + 34h], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 24h], esi
        mov    eax, dword ptr [eax + 0ch]
        push   edi
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 20h], eax
        lea    eax, [esp + 30h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        mov    eax, dword ptr [esp + 4ch]
        add    eax, dword ptr [esp + 40h]
        add    eax, 4
        push   eax
        lea    eax, [esp + 2ch]
        push   0
        push   eax
        call   sub_1_3A60
        mov    ecx, dword ptr [esp + 30h]
        add    esp, 18h
        add    dword ptr [ebp + 24h], ecx
        mov    al, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Dh, 049h, 000h
loc_1_0FC0:
        mov    edx, 1
        jmp    SHORT loc_1_0FCC
loc_1_0FC7:
        mov    edx, 0fh
loc_1_0FCC:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_0FFE
        mov    eax, dword ptr [esp + 10h]
        db  0C7h, 000h, 08Ch, 032h, 001h, 000h
        mov    ecx, dword ptr [esp + 10h]
        db  0A1h, 090h, 032h, 001h, 000h
        mov    dword ptr [ecx + 4], eax
        db  08Bh, 00Dh, 090h, 032h, 001h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 10h]
        db  0A3h, 090h, 032h, 001h, 000h
loc_1_0FFE:
        push   edx
        call   sub_1_0AD0
        add    esp, 4
loc_1_1007:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  090h
sub_1_1010:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 28h]
        mov    dword ptr [esp + 14h], 0
        mov    cl, byte ptr [edi + 3]
        cmp    cl, 0ffh
        je     SHORT loc_1_1090
        lea    eax, [esp + 14h]
        push   1
        push   eax
        mov    esi, dword ptr [esp + 2ch]
        push   ecx
        push   esi
        call   sub_1_2C70
        add    esp, 10h
        or     al, al
        jne    NEAR PTR loc_1_1202
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [eax + 44h], 0
        jne    SHORT loc_1_1082
        mov    eax, dword ptr [esi + 2ch]
        sub    eax, esi
        cmp    eax, 2ch
        jne    SHORT loc_1_109E
loc_1_105C:
        lea    eax, [esp + 10h]
        push   4
        push   eax
        db  068h, 09Ch, 032h, 001h, 000h
        call   sub_1_FEF4
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_1_1143
        mov    al, 38h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_1082:
        mov    al, 15h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_1090:
        mov    esi, dword ptr [esp + 24h]
        mov    eax, dword ptr [esi + 2ch]
        sub    eax, esi
        cmp    eax, 2ch
        je     SHORT loc_1_105C
loc_1_109E:
        lea    eax, [esi + 2ch]
        mov    dword ptr [esp + 18h], eax
        mov    ebp, dword ptr [eax]
        cmp    eax, ebp
        je     SHORT loc_1_10F1
        add    eax, 0
loc_1_10B0:
        lea    ebx, [ebp - 28h]
        cmp    byte ptr [edi + 3], 0ffh
        je     SHORT loc_1_10F5
        mov    eax, dword ptr [ebx + 0ch]
        push   10h
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 14h]
        add    eax, 34h
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 14h]
        add    eax, 34h
        push   eax
        call   sub_1_0262
        add    esp, 0ch
        cmp    eax, 10h
        je     SHORT loc_1_10F5
        mov    ebp, dword ptr [ebp]
        cmp    dword ptr [esp + 18h], ebp
        jne    SHORT loc_1_10B0
        jmp    SHORT loc_1_10F5
loc_1_10F1:
        mov    ebx, dword ptr [esp + 1ch]
loc_1_10F5:
        cmp    dword ptr [esp + 18h], ebp
        je     NEAR PTR loc_1_105C
        mov    ecx, dword ptr [ebx + 28h]
        lea    edx, [ebx + 28h]
        mov    eax, dword ptr [ebx + 2ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    byte ptr [ebx + 48h], 0
        lea    eax, [edi + 2]
        mov    dword ptr [ebx + 2ch], edx
        mov    dword ptr [edx], edx
        push   eax
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        push   esi
        call   sub_1_2DC0
        add    esp, 0ch
        or     al, al
        jne    NEAR PTR loc_1_1202
        push   0
        push   edi
        push   esi
        call   sub_1_1210
        add    esp, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_1143:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 14h], 1900beefh
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 1ch], edi
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 30h], 4655424eh
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 20h], 0
        xor    eax, eax
        mov    ecx, dword ptr [esp + 10h]
        mov    ax, word ptr [edi + 8]
        mov    dword ptr [ecx + 2ch], eax
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 28h], 0
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax + 35h], 20h
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edi + 32h], eax
        cmp    byte ptr [edi + 3], 0ffh
        je     SHORT loc_1_11E0
        mov    edx, dword ptr [esp + 14h]
        mov    ecx, dword ptr [edx + 40h]
        add    edx, 3ch
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edx + 4], eax
        mov    al, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_11E0:
        mov    ecx, dword ptr [esi + 28h]
        add    esi, 24h
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 4], eax
        mov    al, 0ffh
loc_1_1202:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1210:
        sub    esp, 1ch
        push   esi
        push   edi
        lea    eax, [esp + 0ch]
        mov    esi, dword ptr [esp + 2ch]
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   esi
        push   eax
        call   sub_1_2D50
        add    esp, 0ch
        or     al, al
        jne    NEAR PTR loc_1_1409
        mov    eax, dword ptr [esp + 0ch]
        push   4
        mov    edi, dword ptr [eax + 0ch]
        lea    eax, [esp + 0ch]
        push   eax
        db  068h, 09Ch, 032h, 001h, 000h
        call   sub_1_FEF4
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_1407
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 14h], 1900beefh
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 18h], edi
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 1ch], esi
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 30h], 4655424eh
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 20h], 0
        xor    eax, eax
        mov    ecx, dword ptr [esp + 8]
        mov    ax, word ptr [esi + 8]
        mov    dword ptr [ecx + 2ch], eax
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 28h], 0
        mov    eax, dword ptr [esp + 0ch]
        mov    cl, byte ptr [eax + 38h]
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [eax + 34h], cl
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 35h], 20h
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 32h], eax
        cmp    word ptr [edi + 32h], 3
        mov    eax, dword ptr [esp + 0ch]
        je     SHORT loc_1_1315
        cmp    byte ptr [eax + 6ch], 0
        jne    SHORT loc_1_12E9
        cmp    dword ptr [esp + 30h], 0
        je     SHORT loc_1_12E9
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 35h], 100h
loc_1_12E9:
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [edx + 20h]
        add    edx, 1ch
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx + 4], eax
        mov    al, 0ffh
        pop    edi
        pop    esi
        add    esp, 1ch
        ret
loc_1_1315:
        mov    dword ptr [eax + 50h], 0
        db  0C7h, 044h, 024h, 018h, 080h, 043h, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esp + 1ch], eax
        lea    edx, [edi + 28h]
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 60h], 0
        mov    word ptr [edi + 32h], 2
        mov    eax, dword ptr [edi + 2ch]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    byte ptr [edi + 48h], 0
        mov    dword ptr [edi + 2ch], edx
        mov    dword ptr [edx], edx
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [eax + 3ch]
        sub    ecx, dword ptr [eax + 44h]
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        cmp    ecx, eax
        jb     SHORT loc_1_1370
        mov    ecx, eax
loc_1_1370:
        mov    dword ptr [esp + 10h], ecx
        or     ecx, ecx
        jne    SHORT loc_1_1391
        push   0
        mov    eax, dword ptr [esp + 0ch]
        push   0
        push   eax
        call   sub_1_4380
        add    esp, 0ch
        sub    al, al
        pop    edi
        pop    esi
        add    esp, 1ch
        ret
loc_1_1391:
        mov    eax, dword ptr [esp + 0ch]
        cmp    byte ptr [eax + 6ch], 0
        jne    SHORT loc_1_13AB
        cmp    dword ptr [esp + 30h], 0
        je     SHORT loc_1_13AB
        db  066h, 0C7h, 005h, 04Ch, 02Fh, 001h, 000h, 000h, 001h
loc_1_13AB:
        mov    eax, dword ptr [esp + 8]
        add    eax, 20h
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        db  068h, 04Ch, 02Fh, 001h, 000h
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        call   dword ptr [eax + 28h]
        add    esp, 10h
        mov    edi, eax
        cmp    edi, 0ffh
        jne    SHORT loc_1_13E0
        mov    al, 0ffh
        pop    edi
        pop    esi
        add    esp, 1ch
        ret
        db  090h
loc_1_13E0:
        or     edi, edi
        je     SHORT loc_1_13F8
        cmp    edi, 0ffh
        je     SHORT loc_1_13F8
        push   0
        push   edi
        push   esi
        call   sub_1_25A0
        add    esp, 0ch
loc_1_13F8:
        push   edi
        call   sub_1_0AD0
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 1ch
        ret
loc_1_1407:
        mov    al, 38h
loc_1_1409:
        pop    edi
        pop    esi
        add    esp, 1ch
        ret
        db  090h
sub_1_1410:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 18h], 3a98h
        lea    eax, [esp + 14h]
        mov    edi, dword ptr [esp + 34h]
        push   eax
        mov    esi, dword ptr [esp + 34h]
        push   edi
        push   esi
        call   sub_1_2D50
        add    esp, 0ch
        or     al, al
        je     SHORT loc_1_1459
        cmp    al, 0ah
        jne    NEAR PTR loc_1_1539
        push   0
        push   0
        push   edi
        call   sub_1_25A0
        add    esp, 0ch
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_1459:
        mov    eax, dword ptr [esp + 14h]
        push   eax
        push   esi
        call   sub_1_2FB0
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 8
        mov    ebx, dword ptr [eax + 10h]
        mov    edx, dword ptr [eax + 0ch]
        or     edx, edx
        je     SHORT loc_1_149A
        cmp    byte ptr [edx + 48h], 1
        jne    SHORT loc_1_149A
        cmp    word ptr [edx + 32h], 3
        jne    SHORT loc_1_149A
        mov    eax, dword ptr [edx + 28h]
        lea    ecx, [edx + 28h]
        mov    ebp, dword ptr [edx + 2ch]
        mov    dword ptr [ebp], eax
        mov    dword ptr [eax + 4], ebp
        mov    byte ptr [edx + 48h], 0
        mov    dword ptr [edx + 2ch], ecx
        mov    dword ptr [ecx], ecx
loc_1_149A:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 1ch], eax
        push   0
        push   0
        push   0
        lea    eax, [esp + 24h]
        push   4
        push   eax
        lea    eax, [esp + 30h]
        push   eax
        call   sub_1_BDC0
        add    esp, 18h
        push   0
        push   esi
        lea    eax, [esp + 24h]
        push   0
        push   eax
        call   sub_1_AD68
        lea    eax, [esp + 2ch]
        add    esp, 10h
        push   eax
        call   sub_1_A9FC
        add    esp, 4
        push   1
        mov    al, byte ptr [edi + 2]
        push   eax
        push   esi
        call   sub_1_2F70
        add    esp, 0ch
        or     ebx, ebx
        je     SHORT loc_1_152A
        cmp    dword ptr [ebx + 44h], 0
        je     SHORT loc_1_152A
        push   ebx
        call   sub_1_1770
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_152A
        lea    eax, [esp + 13h]
        push   eax
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        push   esi
        call   sub_1_2E00
        add    esp, 0ch
        or     al, al
        jne    SHORT loc_1_152A
        mov    al, byte ptr [esp + 13h]
        push   1
        push   eax
        push   ebx
        push   0
        push   esi
        call   sub_1_17A0
        add    esp, 14h
loc_1_152A:
        push   0
        push   0
        push   edi
        call   sub_1_25A0
        add    esp, 0ch
        sub    al, al
loc_1_1539:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1550:
        sub    esp, 14h
        push   esi
        push   edi
        mov    dword ptr [esp + 8], 0
        mov    edi, dword ptr [esp + 24h]
        lea    eax, [esp + 8]
        push   edi
        push   eax
        lea    eax, [esp + 14h]
        mov    esi, dword ptr [esp + 28h]
        push   eax
        push   esi
        call   sub_1_2A90
        add    esp, 10h
        or     al, al
        jne    SHORT loc_1_15D3
        cmp    byte ptr [edi + 0ah], 2ah
        mov    ecx, 0
        je     SHORT loc_1_158E
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
loc_1_158E:
        mov    eax, dword ptr [esp + 8]
        push   edi
        mov    eax, dword ptr [eax + 4]
        push   eax
        push   ecx
        lea    eax, [esp + 18h]
        push   1
        push   eax
        call   sub_1_BD08
        add    esp, 14h
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_15CA
        cmp    edi, 0ffh
        je     SHORT loc_1_15CA
        mov    eax, dword ptr [esp + 8]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        push   esi
        call   sub_1_3080
        add    esp, 10h
loc_1_15CA:
        push   edi
        call   sub_1_0AD0
        add    esp, 4
loc_1_15D3:
        pop    edi
        pop    esi
        add    esp, 14h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_15E0:
        sub    esp, 24h
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 34h]
        mov    al, byte ptr [esi + 1ah]
        lea    ecx, [esi + 1ah]
        cmp    al, 2ah
        je     NEAR PTR loc_1_16B9
        or     al, al
        je     NEAR PTR loc_1_16B9
        mov    al, byte ptr [esi]
        and    eax, 7fh
        cmp    eax, 30h
        je     SHORT loc_1_1616
        cmp    eax, 36h
        je     SHORT loc_1_1620
        mov    al, 40h
        pop    edi
        pop    esi
        add    esp, 24h
        ret
loc_1_1616:
        mov    word ptr [esp + 18h], 0
        jmp    SHORT loc_1_1627
        db  090h
loc_1_1620:
        mov    word ptr [esp + 18h], 1
loc_1_1627:
        lea    edx, [esp + 1ah]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 0ch], eax
        mov    eax, dword ptr [esp + 30h]
        push   esi
        push   eax
        push   0
        mov    eax, dword ptr [eax + 58h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_A19C
        add    esp, 18h
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_166D
        cmp    edi, 0ffh
        jne    SHORT loc_1_1697
loc_1_166D:
        mov    eax, dword ptr [esp + 8]
        push   eax
        db  068h, 030h, 043h, 000h, 000h
        push   3
        push   eax
        call   sub_1_CD0C
        mov    eax, dword ptr [esp + 18h]
        add    esp, 10h
        push   eax
        db  068h, 070h, 047h, 000h, 000h
        push   1
        push   eax
        call   sub_1_CD0C
        add    esp, 10h
loc_1_1697:
        or     edi, edi
        jne    SHORT loc_1_16AA
        mov    eax, dword ptr [esp + 8]
        push   eax
        push   edi
        push   esi
        call   sub_1_25A0
        add    esp, 0ch
loc_1_16AA:
        push   edi
        call   sub_1_0AD0
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 24h
        ret
loc_1_16B9:
        mov    al, 15h
        pop    edi
        pop    esi
        add    esp, 24h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_16D0:
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 18h]
        mov    al, byte ptr [esi + 1ah]
        lea    ecx, [esi + 1ah]
        cmp    al, 2ah
        je     SHORT loc_1_1760
        or     al, al
        je     SHORT loc_1_1760
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 0fh]
        push   eax
        mov    edi, dword ptr [esp + 1ch]
        push   ecx
        push   edi
        call   sub_1_2E40
        add    esp, 10h
        or     al, al
        jne    SHORT loc_1_1762
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        call   sub_1_1770
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1742
        mov    al, byte ptr [esp + 0bh]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        push   esi
        push   edi
        call   sub_1_17A0
        add    esp, 14h
        push   0
        push   0ca000001h
        push   esi
        call   sub_1_25A0
        add    esp, 0ch
        sub    al, al
        pop    edi
        pop    esi
        add    esp, 8
        ret
loc_1_1742:
        mov    al, byte ptr [esp + 0bh]
        push   1
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        push   esi
        push   edi
        call   sub_1_17A0
        add    esp, 14h
        pop    edi
        pop    esi
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_1760:
        mov    al, 15h
loc_1_1762:
        pop    edi
        pop    esi
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1770:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 1ch]
        add    ecx, 1ch
        cmp    eax, ecx
        je     SHORT loc_1_178E
        mov    edx, 1
loc_1_1783:
        cmp    dword ptr [eax + 68h], edx
        jg     SHORT loc_1_1791
        mov    eax, dword ptr [eax]
        cmp    eax, ecx
        jne    SHORT loc_1_1783
loc_1_178E:
        sub    eax, eax
        ret
loc_1_1791:
        mov    eax, 1
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_17A0:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 30h]
        mov    dword ptr [ebx + 44h], 1
        mov    eax, dword ptr [ebx + 2ch]
        sub    eax, ebx
        cmp    eax, 2ch
        je     SHORT loc_1_17F3
        lea    esp, [esp]
loc_1_17C0:
        mov    esi, dword ptr [ebx + 2ch]
        push   0
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0c00000cah
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        push   esi
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [ebx + 2ch]
        sub    eax, ebx
        cmp    eax, 2ch
        jne    SHORT loc_1_17C0
loc_1_17F3:
        mov    eax, dword ptr [ebx + 24h]
        sub    eax, ebx
        cmp    eax, 24h
        je     SHORT loc_1_1833
loc_1_1800:
        mov    esi, dword ptr [ebx + 24h]
        push   0
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0c00000cah
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        push   esi
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [ebx + 24h]
        sub    eax, ebx
        cmp    eax, 24h
        jne    SHORT loc_1_1800
loc_1_1833:
        lea    eax, [esp + 10h]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   0
        push   eax
        call   sub_1_2C70
        add    esp, 10h
        or     al, al
        jne    SHORT loc_1_1892
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [edi + 24h]
        add    edi, 24h
        cmp    esi, edi
        je     SHORT loc_1_1892
loc_1_185B:
        mov    edx, dword ptr [esi]
        mov    ebp, edx
        mov    ecx, dword ptr [esi + 8]
        mov    al, byte ptr [esp + 34h]
        cmp    byte ptr [ecx + 3], al
        jne    SHORT loc_1_188C
        mov    eax, dword ptr [esi + 4]
        push   0
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        push   0c00000cah
        push   ecx
        call   sub_1_25A0
        add    esp, 0ch
        push   esi
        call   sub_1_0024
        add    esp, 4
loc_1_188C:
        mov    esi, ebp
        cmp    ebp, edi
        jne    SHORT loc_1_185B
loc_1_1892:
        mov    eax, dword ptr [ebx + 3ch]
        sub    eax, ebx
        cmp    eax, 3ch
        je     SHORT loc_1_18CA
        lea    esp, [esp]
loc_1_18A0:
        mov    eax, dword ptr [ebx + 3ch]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        push   0c00000cah
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        mov    eax, dword ptr [ebx + 3ch]
        sub    eax, ebx
        cmp    eax, 3ch
        jne    SHORT loc_1_18A0
loc_1_18CA:
        sub    esi, esi
        cmp    dword ptr [esp + 38h], esi
        je     SHORT loc_1_191C
        mov    dword ptr [esp + 14h], ebx
        mov    edi, dword ptr [esp + 2ch]
        mov    eax, dword ptr [esp + 28h]
        push   edi
        push   eax
        lea    eax, [esp + 1ch]
        push   esi
        push   eax
        call   sub_1_AB28
        add    esp, 10h
        mov    esi, eax
        cmp    esi, 0ffh
        je     SHORT loc_1_1908
        or     edi, edi
        je     SHORT loc_1_1908
        push   0
        push   esi
        push   edi
        call   sub_1_25A0
        add    esp, 0ch
loc_1_1908:
        mov    al, byte ptr [esp + 34h]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   eax
        call   sub_1_2F70
        add    esp, 0ch
loc_1_191C:
        push   esi
        call   sub_1_0AD0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
sub_1_1930:
        sub    esp, 10h
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp], eax
        push   0
        mov    eax, dword ptr [ecx + 36h]
        add    ecx, 32h
        push   eax
        mov    eax, dword ptr [ecx]
        push   eax
        lea    eax, [esp + 0ch]
        push   eax
        call   sub_1_C110
        add    esp, 10h
        cmp    eax, 0ffh
        jne    SHORT loc_1_1961
        sub    eax, eax
loc_1_1961:
        add    esp, 10h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1970:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 38h]
        xor    eax, eax
        mov    ax, word ptr [ebx + 8]
        mov    ecx, dword ptr [esp + 3ch]
        mov    dword ptr [esp + 14h], eax
        or     ecx, ecx
        jne    SHORT loc_1_1A00
        cmp    byte ptr [ebx + 0ah], 2ah
        jne    SHORT loc_1_1A00
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 3ch]
        push   eax
        call   sub_1_D62C
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_19B9
        cmp    ebp, 9
        jne    NEAR PTR loc_1_1A6A
loc_1_19B9:
        xor    edx, edx
        mov    dx, word ptr [ebx + 8]
        mov    eax, dword ptr [esp + 14h]
        cmp    edx, eax
        jb     SHORT loc_1_19C9
        mov    edx, eax
loc_1_19C9:
        mov    edi, dword ptr [ebx + 4]
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    word ptr [ebx + 8], dx
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_1_0024
        add    esp, 4
        push   0
        push   ebp
        push   ebx
        call   sub_1_25A0
        add    esp, 0ch
        sub    ebp, ebp
        jmp    SHORT loc_1_1A6A
        db  090h
loc_1_1A00:
        db  0C7h, 005h, 0A8h, 033h, 001h, 000h, 001h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 0ACh, 033h, 001h, 000h, 012h, 000h
        db  066h, 0C7h, 005h, 0AEh, 033h, 001h, 000h, 011h, 000h
        db  066h, 0C7h, 005h, 0B0h, 033h, 001h, 000h, 000h, 000h
        db  0BAh, 0B2h, 033h, 001h, 000h
        mov    eax, dword ptr [ebx + 0ah]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebx + 0eh]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ebx + 12h]
        lea    esi, [ebx + 0ah]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [esp + 28h], 1ah
        db  0C7h, 044h, 024h, 02Ch, 0A8h, 033h, 001h, 000h
        push   ecx
        mov    eax, dword ptr [esp + 38h]
        push   ebx
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        call   sub_1_CEA8
        add    esp, 10h
        mov    ebp, eax
loc_1_1A6A:
        push   ebp
        call   sub_1_0AD0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_1_1A80:
        sub    esp, 18h
        push   esi
        db  0C7h, 005h, 0A8h, 033h, 001h, 000h, 001h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 0ACh, 033h, 001h, 000h, 012h, 000h
        db  066h, 0C7h, 005h, 0AEh, 033h, 001h, 000h, 011h, 000h
        mov    esi, dword ptr [esp + 24h]
        db  066h, 0C7h, 005h, 0B0h, 033h, 001h, 000h, 000h, 000h
        db  0BAh, 0B2h, 033h, 001h, 000h
        mov    eax, dword ptr [esi + 0ah]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esi + 0eh]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [esi + 12h]
        lea    ecx, [esi + 0ah]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [esp + 14h], 1ah
        db  0C7h, 044h, 024h, 018h, 0A8h, 033h, 001h, 000h
        push   0
        mov    eax, dword ptr [esp + 24h]
        push   esi
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        call   sub_1_D104
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_1B09
        push   -1
        push   0
        push   esi
        call   sub_1_25A0
        add    esp, 0ch
        sub    al, al
        pop    esi
        add    esp, 18h
        ret
loc_1_1B09:
        push   eax
        call   sub_1_0AD0
        add    esp, 4
        pop    esi
        add    esp, 18h
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B20:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [esp + 20h], 0
        mov    esi, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [esp + 10h], 0
        mov    dword ptr [esp + 1ch], 0
        cmp    eax, 4
        jae    SHORT loc_1_1B76
        push   0
        mov    eax, dword ptr [esp + 30h]
        push   0c0000206h
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_1B76:
        mov    byte ptr [esi], 0
        mov    byte ptr [esi + 1], 0
        mov    byte ptr [esi + 2], 0
        mov    byte ptr [esi + 3], 0
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        mov    edi, dword ptr [esp + 30h]
        push   0
        push   edi
        call   sub_1_2C70
        add    esp, 10h
        or     al, al
        jne    NEAR PTR loc_1_1DB4
        mov    eax, dword ptr [esp + 2ch]
        add    eax, 1ah
        cmp    byte ptr [eax], 2ah
        jne    NEAR PTR loc_1_1CD0
        mov    cl, 1
        cmp    byte ptr [edi + 4fh], cl
        jb     SHORT loc_1_1C0E
loc_1_1BBA:
        xor    eax, eax
        mov    edx, dword ptr [edi + 48h]
        mov    al, cl
        mov    eax, dword ptr [edx + eax*4]
        or     eax, eax
        je     SHORT loc_1_1C07
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 10h], eax
        mov    edx, dword ptr [eax + 24h]
        sub    eax, edx
        cmp    eax, -24h
        je     SHORT loc_1_1BE9
loc_1_1BD9:
        inc    byte ptr [esi + 2]
        mov    edx, dword ptr [edx]
        mov    eax, dword ptr [esp + 10h]
        sub    eax, edx
        cmp    eax, -24h
        jne    SHORT loc_1_1BD9
loc_1_1BE9:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [eax + 3ch]
        sub    eax, edx
        cmp    eax, -3ch
        je     SHORT loc_1_1C07
loc_1_1BF7:
        inc    byte ptr [esi + 3]
        mov    edx, dword ptr [edx]
        mov    eax, dword ptr [esp + 10h]
        sub    eax, edx
        cmp    eax, -3ch
        jne    SHORT loc_1_1BF7
loc_1_1C07:
        inc    cl
        cmp    byte ptr [edi + 4fh], cl
        jae    SHORT loc_1_1BBA
loc_1_1C0E:
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [eax + 24h]
        sub    eax, ecx
        cmp    eax, -24h
        je     SHORT loc_1_1C30
        lea    esp, [esp]
loc_1_1C20:
        inc    byte ptr [esi + 2]
        mov    ecx, dword ptr [ecx]
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, ecx
        cmp    eax, -24h
        jne    SHORT loc_1_1C20
loc_1_1C30:
        mov    ecx, dword ptr [edi + 34h]
        mov    eax, edi
        sub    eax, ecx
        cmp    eax, -34h
        je     SHORT loc_1_1C4E
        lea    esp, [esp]
loc_1_1C40:
        inc    byte ptr [esi + 2]
        mov    ecx, dword ptr [ecx]
        mov    eax, edi
        sub    eax, ecx
        cmp    eax, -34h
        jne    SHORT loc_1_1C40
loc_1_1C4E:
        mov    byte ptr [esi], 0ffh
        sub    dword ptr [esp + 14h], 4
        lea    eax, [esi + 4]
        mov    dword ptr [esp + 18h], eax
        db  08Bh, 02Dh, 074h, 032h, 001h, 000h
        db  081h, 0FDh, 074h, 032h, 001h, 000h
        je     NEAR PTR loc_1_1D91
loc_1_1C70:
        cmp    dword ptr [esp + 20h], 0
        jne    NEAR PTR loc_1_1D91
        cmp    dword ptr [ebp + 1ch], edi
        jne    SHORT loc_1_1CBE
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, ebp
        sub    eax, ebx
        cmp    eax, -0ch
        je     SHORT loc_1_1CBE
        lea    esp, [esp]
loc_1_1C90:
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   esi
        push   ebx
        push   edi
        call   sub_1_1DC0
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_1CB6
        mov    ebx, dword ptr [ebx]
        mov    eax, ebp
        sub    eax, ebx
        cmp    eax, -0ch
        jne    SHORT loc_1_1C90
        jmp    SHORT loc_1_1CBE
loc_1_1CB6:
        mov    dword ptr [esp + 20h], 9
loc_1_1CBE:
        mov    ebp, dword ptr [ebp]
        db  081h, 0FDh, 074h, 032h, 001h, 000h
        jne    SHORT loc_1_1C70
        jmp    NEAR PTR loc_1_1D91
        db  08Bh, 0FFh
loc_1_1CD0:
        lea    ecx, [esp + 10h]
        push   ecx
        push   esi
        push   eax
        push   edi
        call   sub_1_2E40
        add    esp, 10h
        or     al, al
        jne    NEAR PTR loc_1_1DB4
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax + 24h]
        sub    eax, ecx
        cmp    eax, -24h
        je     SHORT loc_1_1D06
loc_1_1CF6:
        inc    byte ptr [esi + 2]
        mov    ecx, dword ptr [ecx]
        mov    eax, dword ptr [esp + 10h]
        sub    eax, ecx
        cmp    eax, -24h
        jne    SHORT loc_1_1CF6
loc_1_1D06:
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [eax + 24h]
        sub    eax, ecx
        cmp    eax, -24h
        je     SHORT loc_1_1D24
loc_1_1D14:
        inc    byte ptr [esi + 2]
        mov    ecx, dword ptr [ecx]
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, ecx
        cmp    eax, -24h
        jne    SHORT loc_1_1D14
loc_1_1D24:
        mov    ecx, dword ptr [edi + 34h]
        mov    eax, edi
        sub    eax, ecx
        cmp    eax, -34h
        je     SHORT loc_1_1D3E
loc_1_1D30:
        inc    byte ptr [esi + 2]
        mov    ecx, dword ptr [ecx]
        mov    eax, edi
        sub    eax, ecx
        cmp    eax, -34h
        jne    SHORT loc_1_1D30
loc_1_1D3E:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax + 3ch]
        sub    eax, ecx
        cmp    eax, -3ch
        je     SHORT loc_1_1D60
        lea    esp, [esp]
loc_1_1D50:
        inc    byte ptr [esi + 3]
        mov    ecx, dword ptr [ecx]
        mov    eax, dword ptr [esp + 10h]
        sub    eax, ecx
        cmp    eax, -3ch
        jne    SHORT loc_1_1D50
loc_1_1D60:
        sub    dword ptr [esp + 14h], 4
        lea    eax, [esi + 4]
        mov    dword ptr [esp + 18h], eax
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   esi
        push   eax
        push   edi
        call   sub_1_1DC0
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_1_1D91
        mov    dword ptr [esp + 20h], 9
loc_1_1D91:
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        shl    eax, 2
        lea    eax, [eax + eax*8]
        add    eax, 4
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   eax
        mov    eax, dword ptr [esp + 34h]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        sub    al, al
loc_1_1DB4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_1DC0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        mov    eax, ebx
        mov    edi, dword ptr [ebx + 1ch]
        sub    eax, edi
        cmp    eax, -1ch
        je     NEAR PTR loc_1_1EE9
        mov    esi, dword ptr [esp + 24h]
        mov    ebp, dword ptr [esp + 28h]
loc_1_1DE3:
        mov    dword ptr [esp + 10h], 0
        mov    eax, dword ptr [ebp]
        cmp    eax, 24h
        jb     NEAR PTR loc_1_1F08
        sub    eax, 24h
        mov    dword ptr [ebp], eax
        mov    eax, dword ptr [esp + 20h]
        inc    byte ptr [eax + 1]
        mov    eax, dword ptr [esi]
        push   eax
        mov    byte ptr [eax + 22h], 0
        mov    byte ptr [eax + 23h], 0
        mov    eax, dword ptr [esp + 1ch]
        push   edi
        push   eax
        call   sub_1_2DC0
        add    esp, 0ch
        mov    ecx, dword ptr [edi + 1ch]
        mov    eax, edi
        sub    eax, ecx
        cmp    eax, -1ch
        je     SHORT loc_1_1E3D
        mov    eax, dword ptr [esi]
        add    eax, 22h
        mov    edi, edi
loc_1_1E30:
        inc    byte ptr [eax]
        mov    ecx, dword ptr [ecx]
        mov    edx, edi
        sub    edx, ecx
        cmp    edx, -1ch
        jne    SHORT loc_1_1E30
loc_1_1E3D:
        mov    eax, dword ptr [edi + 68h]
        sub    eax, 3
        cmp    eax, 6
        ja     SHORT loc_1_1E50
        db  0FFh, 024h, 085h, 014h, 01Fh, 000h, 000h
        db  090h
loc_1_1E50:
        mov    eax, dword ptr [esi]
        mov    byte ptr [eax + 1], 6
        jmp    SHORT loc_1_1E8C
loc_1_1E58:
        cmp    byte ptr [edi + 6ch], 0
        mov    eax, dword ptr [esi]
        je     SHORT loc_1_1E66
        mov    byte ptr [eax + 1], 2
        jmp    SHORT loc_1_1E8C
loc_1_1E66:
        mov    byte ptr [eax + 1], 1
        jmp    SHORT loc_1_1E8C
        db  08Dh, 064h, 024h, 000h
loc_1_1E70:
        mov    eax, dword ptr [esi]
        mov    byte ptr [eax + 1], 1
        jmp    SHORT loc_1_1E8C
loc_1_1E78:
        mov    eax, dword ptr [esi]
        mov    byte ptr [eax + 1], 2
loc_1_1E7E:
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 10h], 1
        mov    byte ptr [eax + 1], 3
loc_1_1E8C:
        mov    eax, dword ptr [ebx + 10h]
        mov    ecx, dword ptr [esi]
        mov    edx, dword ptr [eax + 14h]
        add    ecx, 2
        mov    eax, dword ptr [edx + 34h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 38h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 3ch]
        add    edx, 34h
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_1ED7
        lea    eax, [edi + 28h]
        mov    ecx, dword ptr [esi]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx + 12h], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 16h], edx
        add    ecx, 12h
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], edx
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 0ch], eax
loc_1_1ED7:
        add    dword ptr [esi], 24h
        mov    edi, dword ptr [edi]
        mov    eax, ebx
        sub    eax, edi
        cmp    eax, -1ch
        jne    NEAR PTR loc_1_1DE3
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
        jmp    SHORT loc_1_1E8C
        db  08Bh, 0FFh
loc_1_1F00:
        mov    eax, dword ptr [esi]
        mov    byte ptr [eax + 1], 5
        jmp    SHORT loc_1_1E8C
loc_1_1F08:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh, 058h, 01Eh, 000h, 000h, 070h, 01Eh, 000h, 000h, 070h, 01Eh, 000h, 000h, 078h, 01Eh
        db  000h, 000h, 07Eh, 01Eh, 000h, 000h, 0F6h, 01Eh, 000h, 000h, 000h, 01Fh, 000h, 000h
sub_1_1F30:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 2ch]
        mov    edi, dword ptr [esp + 28h]
        lea    esi, [ebp + 32h]
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 4], 0
        mov    byte ptr [esi + 8], 0
        mov    eax, dword ptr [edi + 24h]
        sub    eax, edi
        cmp    eax, 24h
        je     SHORT loc_1_1F8A
loc_1_1F60:
        mov    eax, dword ptr [edi + 24h]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        push   0c000020ch
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        mov    eax, dword ptr [edi + 24h]
        sub    eax, edi
        cmp    eax, 24h
        jne    SHORT loc_1_1F60
loc_1_1F8A:
        mov    eax, dword ptr [edi + 34h]
        sub    eax, edi
        cmp    eax, 34h
        je     SHORT loc_1_1FC7
loc_1_1F94:
        mov    ebx, dword ptr [edi + 34h]
        push   0
        mov    eax, dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0c00000cah
        mov    eax, dword ptr [ebx + 8]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        push   ebx
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [edi + 34h]
        sub    eax, edi
        cmp    eax, 34h
        jne    SHORT loc_1_1F94
loc_1_1FC7:
        mov    bl, 1
        cmp    byte ptr [edi + 4fh], bl
        jb     SHORT loc_1_203F
        mov    edi, edi
loc_1_1FD0:
        xor    eax, eax
        mov    al, bl
        lea    ecx, [eax*4]
        mov    eax, dword ptr [edi + 48h]
        cmp    dword ptr [ecx + eax], 0
        je     SHORT loc_1_2038
        mov    dword ptr [esp + 10h], 3a98h
        mov    eax, dword ptr [edi + 48h]
        mov    eax, dword ptr [ecx + eax]
        mov    dword ptr [esp + 14h], eax
        push   ebp
        push   0
        push   0
        lea    eax, [esp + 1ch]
        push   4
        push   eax
        inc    dword ptr [esi]
        lea    eax, [esp + 28h]
        push   eax
        call   sub_1_BDC0
        add    esp, 18h
        cmp    eax, 0ffh
        je     SHORT loc_1_2038
        dec    dword ptr [esi]
        push   0
        push   edi
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        call   sub_1_AD68
        add    esp, 10h
        push   1
        push   ebx
        push   edi
        call   sub_1_2F70
        add    esp, 0ch
loc_1_2038:
        inc    bl
        cmp    byte ptr [edi + 4fh], bl
        jae    SHORT loc_1_1FD0
loc_1_203F:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_205C
        mov    dword ptr [esi], 0ffffffffh
        push   ebp
        push   edi
        call   sub_1_2070
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_205C:
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2070:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 3ch]
        add    ebp, 32h
        mov    esi, dword ptr [esp + 38h]
        mov    bl, 1
        cmp    byte ptr [esi + 4fh], bl
        jb     SHORT loc_1_20C0
loc_1_2089:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 48h]
        mov    al, bl
        mov    eax, dword ptr [ecx + eax*4]
        or     eax, eax
        je     SHORT loc_1_20B9
        mov    dword ptr [esp + 24h], eax
        push   0
        push   esi
        lea    eax, [esp + 2ch]
        push   0
        push   eax
        call   sub_1_AD68
        add    esp, 10h
        push   1
        push   ebx
        push   esi
        call   sub_1_2F70
        add    esp, 0ch
loc_1_20B9:
        inc    bl
        cmp    byte ptr [esi + 4fh], bl
        jae    SHORT loc_1_2089
loc_1_20C0:
        sub    bl, bl
        cmp    byte ptr [esi + 4eh], bl
        jb     SHORT loc_1_210E
loc_1_20C7:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 44h]
        mov    al, bl
        mov    eax, dword ptr [ecx + eax*4]
        or     eax, eax
        je     SHORT loc_1_2107
        mov    dword ptr [esp + 14h], eax
        inc    dword ptr [ebp + 4]
        mov    eax, dword ptr [esp + 3ch]
        push   eax
        push   esi
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        call   sub_1_AB28
        add    esp, 10h
        cmp    eax, 0ffh
        je     SHORT loc_1_20FB
        dec    dword ptr [ebp + 4]
loc_1_20FB:
        push   0
        push   ebx
        push   esi
        call   sub_1_2F70
        add    esp, 0ch
loc_1_2107:
        inc    bl
        cmp    byte ptr [esi + 4eh], bl
        jae    SHORT loc_1_20C7
loc_1_210E:
        mov    eax, dword ptr [esp + 3ch]
        mov    bl, byte ptr [eax + 2]
        cmp    byte ptr [esi + 4fh], bl
        je     SHORT loc_1_2179
        or     bl, bl
        jne    SHORT loc_1_2120
        mov    bl, 6
loc_1_2120:
        mov    al, bl
        and    eax, 0ffff00ffh
        mov    word ptr [esp + 10h], ax
        lea    eax, [eax*4 + 4]
        and    eax, 0ffffh
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_2150
        mov    byte ptr [ebp + 8], 38h
        jmp    SHORT loc_1_2179
        db  08Dh, 049h, 000h
loc_1_2150:
        mov    eax, dword ptr [esi + 48h]
        push   eax
        call   sub_1_0024
        add    esp, 4
        mov    byte ptr [esi + 4fh], bl
        mov    dword ptr [esi + 48h], edi
        mov    eax, dword ptr [esp + 10h]
        and    eax, 0ffffh
        lea    ecx, [eax*4 + 4]
        sub    eax, eax
        shr    ecx, 2
        rep stosd
loc_1_2179:
        mov    byte ptr [esi + 4ch], 1
        mov    byte ptr [esi + 4dh], 1
        push   esi
        call   sub_1_FBB8
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_2197
        push   esi
        call   sub_1_FBB8
        add    esp, 4
loc_1_2197:
        cmp    dword ptr [ebp + 4], 0
        jne    SHORT loc_1_21AE
        push   0
        mov    eax, dword ptr [esp + 40h]
        push   0
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
loc_1_21AE:
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_21C0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 20h]
        sub    bl, bl
        mov    esi, dword ptr [ecx + 4]
        mov    al, byte ptr [ecx + 30h]
        cmp    byte ptr [esi + 30h], al
        je     SHORT loc_1_21E2
        mov    al, 23h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_21E2:
        mov    ecx, dword ptr [esp + 20h]
        cmp    byte ptr [ecx + 1], 0ffh
        je     SHORT loc_1_21F6
        mov    al, 24h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_21F6:
        mov    al, byte ptr [esi]
        and    eax, 7fh
        cmp    eax, 17h
        jg     SHORT loc_1_2234
        je     NEAR PTR loc_1_236A
        cmp    eax, 10h
        je     SHORT loc_1_2266
        cmp    eax, 11h
        je     NEAR PTR loc_1_22F2
        cmp    eax, 14h
        jl     NEAR PTR loc_1_257B
        cmp    eax, 15h
        jle    NEAR PTR loc_1_236A
        cmp    eax, 16h
        je     NEAR PTR loc_1_2415
        jmp    NEAR PTR loc_1_257B
loc_1_2234:
        cmp    eax, 21h
        je     NEAR PTR loc_1_2490
        cmp    eax, 23h
        je     NEAR PTR loc_1_2510
        cmp    eax, 35h
        je     NEAR PTR loc_1_257B
        cmp    eax, 71h
        jl     NEAR PTR loc_1_257B
        cmp    eax, 72h
        jle    NEAR PTR loc_1_236A
        jmp    NEAR PTR loc_1_257B
loc_1_2266:
        lea    eax, [esp + 10h]
        push   eax
        push   0
        lea    eax, [esi + 1ah]
        mov    ebp, dword ptr [esp + 24h]
        push   eax
        push   ebp
        call   sub_1_2E40
        add    esp, 10h
        or     al, al
        mov    bl, 24h
        je     SHORT loc_1_2289
        jmp    NEAR PTR loc_1_257D
loc_1_2289:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax + 1ch]
        sub    eax, ecx
        cmp    eax, -1ch
        je     NEAR PTR loc_1_257D
        add    eax, 0
loc_1_22A0:
        mov    dword ptr [esp + 14h], ecx
        cmp    dword ptr [ecx + 54h], esi
        je     SHORT loc_1_22C0
        mov    ecx, dword ptr [ecx]
        mov    eax, dword ptr [esp + 10h]
        sub    eax, ecx
        cmp    eax, -1ch
        jne    SHORT loc_1_22A0
        jmp    NEAR PTR loc_1_257D
        db  005h, 000h, 000h, 000h, 000h
loc_1_22C0:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 60h]
        cmp    dword ptr [ecx + 68h], 6
        jl     SHORT loc_1_22D8
        mov    bl, 24h
        jmp    NEAR PTR loc_1_257D
loc_1_22D8:
        or     word ptr [eax + 4ch], 40h
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [eax + 5ch], ecx
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_22F2:
        lea    eax, [esp + 10h]
        push   eax
        push   0
        lea    eax, [esi + 1ah]
        mov    ebp, dword ptr [esp + 24h]
        push   eax
        push   ebp
        call   sub_1_2E40
        add    esp, 10h
        or     al, al
        mov    bl, 24h
        je     SHORT loc_1_2315
        jmp    NEAR PTR loc_1_257D
loc_1_2315:
        mov    eax, dword ptr [esp + 10h]
        mov    edi, dword ptr [eax + 2ch]
        sub    eax, edi
        cmp    eax, -2ch
        je     NEAR PTR loc_1_257D
loc_1_2327:
        cmp    dword ptr [edi + 8], esi
        je     SHORT loc_1_2340
        mov    edi, dword ptr [edi]
        mov    eax, dword ptr [esp + 10h]
        sub    eax, edi
        cmp    eax, -2ch
        jne    SHORT loc_1_2327
        jmp    NEAR PTR loc_1_257D
        db  08Bh, 0FFh
loc_1_2340:
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0c0000120h
        sub    bl, bl
        push   esi
        call   sub_1_25A0
        add    esp, 0ch
        push   edi
        call   sub_1_0024
        add    esp, 4
        jmp    NEAR PTR loc_1_257D
loc_1_236A:
        lea    eax, [esp + 14h]
        push   eax
        mov    ebp, dword ptr [esp + 20h]
        push   esi
        push   ebp
        call   sub_1_2D50
        add    esp, 0ch
        or     al, al
        jne    NEAR PTR loc_1_257D
        mov    al, byte ptr [esi]
        and    eax, 7fh
        cmp    eax, 15h
        mov    bl, 24h
        jne    SHORT loc_1_23D3
        mov    eax, dword ptr [esp + 14h]
        mov    edx, dword ptr [eax + 1ch]
        sub    eax, edx
        cmp    eax, -1ch
        je     SHORT loc_1_23D3
        nop
loc_1_23A0:
        cmp    dword ptr [edx + 1ch], esi
        je     SHORT loc_1_23B4
        mov    edx, dword ptr [edx]
        mov    eax, dword ptr [esp + 14h]
        sub    eax, edx
        cmp    eax, -1ch
        jne    SHORT loc_1_23A0
        jmp    SHORT loc_1_23D3
loc_1_23B4:
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        sub    bl, bl
        push   0
        push   0c0000120h
        mov    eax, dword ptr [edx + 1ch]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
loc_1_23D3:
        mov    eax, dword ptr [esp + 14h]
        push   eax
        push   ebp
        call   sub_1_2FB0
        add    esp, 8
        push   0
        mov    eax, dword ptr [esp + 18h]
        push   0
        push   eax
        push   ebp
        call   sub_1_3080
        mov    eax, dword ptr [esp + 24h]
        add    esp, 10h
        test   byte ptr [eax + 3ah], 1
        je     NEAR PTR loc_1_257D
        push   1
        mov    al, byte ptr [esi + 2]
        push   eax
        push   ebp
        call   sub_1_2F70
        add    esp, 0ch
        jmp    NEAR PTR loc_1_257D
loc_1_2415:
        mov    cl, byte ptr [esi + 3]
        cmp    cl, 0ffh
        jne    SHORT loc_1_2426
        mov    ebp, dword ptr [esp + 1ch]
        add    ebp, 24h
        jmp    SHORT loc_1_244C
loc_1_2426:
        lea    eax, [esp + 10h]
        push   1
        push   eax
        mov    ebp, dword ptr [esp + 24h]
        push   ecx
        push   ebp
        call   sub_1_2C70
        add    esp, 10h
        mov    bl, al
        or     bl, bl
        jne    NEAR PTR loc_1_257D
        mov    ebp, dword ptr [esp + 10h]
        add    ebp, 3ch
loc_1_244C:
        mov    bl, 24h
        mov    edx, dword ptr [ebp]
        cmp    edx, ebp
        je     NEAR PTR loc_1_257D
loc_1_2459:
        cmp    dword ptr [edx + 1ch], esi
        je     SHORT loc_1_2469
        mov    edx, dword ptr [edx]
        cmp    edx, ebp
        jne    SHORT loc_1_2459
        jmp    NEAR PTR loc_1_257D
loc_1_2469:
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        sub    bl, bl
        push   0
        push   0c0000120h
        mov    eax, dword ptr [edx + 1ch]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        jmp    NEAR PTR loc_1_257D
        db  08Dh, 049h, 000h
loc_1_2490:
        mov    cl, byte ptr [esi + 3]
        cmp    cl, 0ffh
        jne    SHORT loc_1_24A1
        mov    ebp, dword ptr [esp + 1ch]
        add    ebp, 34h
        jmp    SHORT loc_1_24C7
loc_1_24A1:
        lea    eax, [esp + 10h]
        push   1
        push   eax
        mov    ebp, dword ptr [esp + 24h]
        push   ecx
        push   ebp
        call   sub_1_2C70
        add    esp, 10h
        mov    bl, al
        or     bl, bl
        jne    NEAR PTR loc_1_257D
        mov    ebp, dword ptr [esp + 10h]
        add    ebp, 24h
loc_1_24C7:
        mov    bl, 24h
        mov    edi, dword ptr [ebp]
        cmp    edi, ebp
        je     NEAR PTR loc_1_257D
loc_1_24D4:
        cmp    dword ptr [edi + 8], esi
        je     SHORT loc_1_24E4
        mov    edi, dword ptr [edi]
        cmp    edi, ebp
        jne    SHORT loc_1_24D4
        jmp    NEAR PTR loc_1_257D
loc_1_24E4:
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0c0000120h
        sub    bl, bl
        push   esi
        call   sub_1_25A0
        add    esp, 0ch
        push   edi
        call   sub_1_0024
        add    esp, 4
        jmp    NEAR PTR loc_1_257D
        db  08Bh, 0FFh
loc_1_2510:
        lea    eax, [esp + 10h]
        push   0
        push   eax
        mov    ebp, dword ptr [esp + 24h]
        push   0
        push   ebp
        call   sub_1_2C70
        add    esp, 10h
        mov    bl, al
        or     bl, bl
        jne    SHORT loc_1_257D
        mov    eax, dword ptr [esp + 10h]
        mov    bl, 24h
        mov    edx, dword ptr [eax + 24h]
        sub    eax, edx
        cmp    eax, -24h
        je     SHORT loc_1_257D
        lea    esp, [esp]
loc_1_2540:
        cmp    dword ptr [edx + 8], esi
        je     SHORT loc_1_2554
        mov    edx, dword ptr [edx]
        mov    eax, dword ptr [esp + 10h]
        sub    eax, edx
        cmp    eax, -24h
        jne    SHORT loc_1_2540
        jmp    SHORT loc_1_257D
loc_1_2554:
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   edx
        call   sub_1_0024
        add    esp, 4
        push   0
        push   0c0000120h
        push   esi
        call   sub_1_25A0
        add    esp, 0ch
        sub    bl, bl
        jmp    SHORT loc_1_257D
loc_1_257B:
        mov    bl, 26h
loc_1_257D:
        push   0
        xor    eax, eax
        mov    al, bl
        mov    ecx, dword ptr [esp + 24h]
        push   eax
        push   ecx
        call   sub_1_25A0
        add    esp, 0ch
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
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        sub    bl, bl
        mov    dword ptr [ebp - 4], ecx
        or     ecx, ecx
        je     NEAR PTR loc_1_29F7
        mov    al, byte ptr [ecx]
        and    al, 80h
        cmp    al, 1
        push   ecx
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 10h], eax
        call   sub_1_0A90
        add    esp, 4
        mov    esi, eax
        mov    ecx, dword ptr [ebp + 0ch]
        or     ecx, ecx
        je     SHORT loc_1_25EF
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 7fh
        cmp    eax, 35h
        je     SHORT loc_1_25EF
        push   ecx
        call   sub_1_0AD0
        add    esp, 4
        mov    bl, al
loc_1_25EF:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 7fh
        cmp    eax, 32h
        jg     SHORT loc_1_2613
        je     NEAR PTR loc_1_2911
        sub    eax, 10h
        cmp    eax, 20h
        ja     SHORT loc_1_263D
        shl    eax, 2
        db  0FFh, 0A0h, 000h, 02Ah, 000h, 000h
loc_1_2613:
        cmp    eax, 36h
        jg     NEAR PTR loc_1_26A0
        je     NEAR PTR loc_1_28D0
        cmp    eax, 33h
        je     NEAR PTR loc_1_2975
        cmp    eax, 34h
        je     NEAR PTR loc_1_28B2
        cmp    eax, 35h
        je     NEAR PTR loc_1_299A
loc_1_263D:
        mov    eax, dword ptr [ebp - 4]
        inc    eax
        cmp    byte ptr [eax], 0ffh
        jne    SHORT loc_1_2665
        mov    byte ptr [eax], bl
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 10h], 0
        mov    byte ptr [eax + 31h], bl
        je     SHORT loc_1_266F
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 2ch]
        or     eax, eax
        je     SHORT loc_1_2665
        push   ebp
        mov    ebx, dword ptr [ebp - 4]
        call   eax
        pop    ebp
loc_1_2665:
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_29F7
loc_1_266F:
        db  0A1h, 0ACh, 032h, 001h, 000h
        db  03Dh, 0ACh, 032h, 001h, 000h
        je     NEAR PTR loc_1_29AB
        nop
loc_1_2680:
        mov    esi, eax
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0ch], ecx
        je     NEAR PTR loc_1_29AE
        sub    esi, esi
        mov    eax, dword ptr [eax]
        db  03Dh, 0ACh, 032h, 001h, 000h
        jne    SHORT loc_1_2680
        jmp    NEAR PTR loc_1_29AE
        db  08Bh, 0FFh
loc_1_26A0:
        cmp    eax, 70h
        je     NEAR PTR loc_1_2964
        cmp    eax, 71h
        jl     SHORT loc_1_263D
        cmp    eax, 72h
        jle    NEAR PTR loc_1_2780
        cmp    eax, 78h
        je     NEAR PTR loc_1_2975
        jmp    NEAR PTR loc_1_263D
loc_1_26C5:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 32h
        mov    dword ptr [ebp - 8], eax
        or     bl, bl
        je     SHORT loc_1_26E7
        push   0
        push   eax
        mov    eax, dword ptr [eax + 8]
        push   eax
        push   esi
        call   sub_1_3080
        add    esp, 10h
        jmp    NEAR PTR loc_1_263D
loc_1_26E7:
        mov    edi, dword ptr [ebp + 10h]
        push   1
        push   edi
        mov    eax, dword ptr [ebp - 4]
        add    eax, 2
        push   eax
        push   esi
        call   sub_1_2EF0
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_2713
        push   0
        push   0
        push   edi
        push   esi
        call   sub_1_3080
        add    esp, 10h
        mov    bl, 11h
        jmp    SHORT loc_1_2757
loc_1_2713:
        mov    eax, dword ptr [ebp - 8]
        cmp    byte ptr [eax + 0ch], 0
        je     SHORT loc_1_273C
        lea    edx, [edi + 28h]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx + 0ah], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 0eh], eax
        add    ecx, 0ah
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [ecx + 0ch], eax
loc_1_273C:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 2ah]
        mov    byte ptr [edi + 38h], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 2bh]
        mov    byte ptr [edi + 39h], al
        inc    dword ptr [edi + 64h]
        mov    word ptr [edi + 3ah], 0
loc_1_2757:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_31C0
        add    esp, 4
        cmp    dword ptr [edi + 68h], 5
        jne    NEAR PTR loc_1_263D
        push   0
        push   edi
        call   sub_1_1930
        add    esp, 8
        jmp    NEAR PTR loc_1_263D
        db  08Dh, 049h, 000h
loc_1_2780:
        mov    edi, dword ptr [ebp - 4]
        add    edi, 32h
        or     bl, bl
        je     NEAR PTR loc_1_2829
        cmp    bl, 5
        je     SHORT loc_1_27A0
        cmp    byte ptr [edi + 0ch], 1
        jne    SHORT loc_1_27A0
        mov    bl, 5
        jmp    NEAR PTR loc_1_2839
loc_1_27A0:
        mov    cl, byte ptr [edi + 0ch]
        lea    eax, [ecx - 1]
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ebp - 14h], eax
        or     cl, cl
        je     SHORT loc_1_27BD
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
loc_1_27BD:
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_1_2D50
        add    esp, 0ch
        or     al, al
        je     SHORT loc_1_27E3
        mov    eax, dword ptr [edi + 8]
        push   eax
        call   sub_1_0024
        add    esp, 4
        jmp    NEAR PTR loc_1_263D
loc_1_27E3:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   esi
        call   sub_1_2FB0
        add    esp, 8
        mov    eax, dword ptr [ebp - 0ch]
        test   byte ptr [eax + 3ah], 1
        je     SHORT loc_1_280B
        mov    eax, dword ptr [ebp - 4]
        push   1
        mov    al, byte ptr [eax + 2]
        push   eax
        push   esi
        call   sub_1_2F70
        add    esp, 0ch
loc_1_280B:
        push   0
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   esi
        call   sub_1_3080
        add    esp, 10h
        cmp    dword ptr [ebp - 14h], 0
        je     SHORT loc_1_2839
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_2829:
        cmp    byte ptr [edi + 0ch], 0
        je     SHORT loc_1_2839
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
loc_1_2839:
        mov    eax, dword ptr [edi + 8]
        db  0C7h, 000h, 08Ch, 032h, 001h, 000h
        mov    ecx, dword ptr [edi + 8]
        db  0A1h, 090h, 032h, 001h, 000h
        mov    dword ptr [ecx + 4], eax
        db  08Bh, 00Dh, 090h, 032h, 001h, 000h
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ecx], eax
        db  0A3h, 090h, 032h, 001h, 000h
        jmp    NEAR PTR loc_1_263D
loc_1_2862:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 32h]
        db  0C7h, 000h, 09Ch, 032h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 0A0h, 032h, 001h, 000h
        mov    eax, dword ptr [eax + 32h]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [ebp - 4]
        or     bl, bl
        db  08Bh, 00Dh, 0A0h, 032h, 001h, 000h
        mov    eax, dword ptr [eax + 32h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 32h]
        db  0A3h, 0A0h, 032h, 001h, 000h
        je     SHORT loc_1_28A3
        cmp    bl, 6
        jne    NEAR PTR loc_1_263D
loc_1_28A3:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 10h]
        mov    word ptr [eax + 8], di
        jmp    NEAR PTR loc_1_263D
loc_1_28B2:
        or     bl, bl
        je     SHORT loc_1_28BF
        cmp    bl, 6
        jne    NEAR PTR loc_1_263D
loc_1_28BF:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 10h]
        mov    word ptr [eax + 8], di
        jmp    NEAR PTR loc_1_263D
        db  08Bh, 0FFh
loc_1_28D0:
        or     bl, bl
        jne    NEAR PTR loc_1_263D
        mov    edi, dword ptr [ebp + 10h]
        push   0
        push   edi
        mov    eax, dword ptr [ebp - 4]
        add    eax, 3
        push   eax
        push   esi
        call   sub_1_2EF0
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_1_263D
        mov    bl, 0eh
        mov    dword ptr [ebp - 28h], edi
        push   0
        push   esi
        push   0
        lea    eax, [ebp - 28h]
        push   eax
        call   sub_1_AB28
        add    esp, 10h
        jmp    NEAR PTR loc_1_263D
loc_1_2911:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 32h]
        add    ecx, 32h
        cmp    eax, -1
        jne    SHORT loc_1_292A
        cmp    dword ptr [ecx + 4], 0
        je     SHORT loc_1_295D
        cmp    eax, -1
        je     SHORT loc_1_2950
loc_1_292A:
        dec    eax
        mov    dword ptr [ecx], eax
        jne    NEAR PTR loc_1_29F7
        mov    dword ptr [ecx], 0ffffffffh
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
        jne    NEAR PTR loc_1_29F7
loc_1_295D:
        or     bl, bl
        jne    SHORT loc_1_2964
        mov    bl, byte ptr [ecx + 8]
loc_1_2964:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 31h], bl
        jmp    NEAR PTR loc_1_2665
loc_1_2975:
        or     bl, bl
        je     SHORT loc_1_2982
        cmp    bl, 6
        jne    NEAR PTR loc_1_263D
loc_1_2982:
        mov    edi, dword ptr [ebp + 10h]
        cmp    edi, -1
        je     NEAR PTR loc_1_263D
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 8], di
        jmp    NEAR PTR loc_1_263D
loc_1_299A:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 31h], bl
        jmp    NEAR PTR loc_1_263D
loc_1_29AB:
        mov    esi, dword ptr [ebp - 18h]
loc_1_29AE:
        or     esi, esi
        je     SHORT loc_1_29F7
        cmp    dword ptr [esi + 18h], 0
        je     SHORT loc_1_29F0
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_003C
        add    esp, 8
        mov    eax, dword ptr [esi + 10h]
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
        mov    dword ptr [esi + 14h], 1
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
        db  03Dh, 026h, 000h, 000h, 03Dh
        add    byte ptr es:[eax], al
        and    byte ptr [edi], 0
        db  000h, 062h, 028h
        add    byte ptr [eax], al
        bound  ebp, qword ptr [eax]
        add    byte ptr [eax], al
        and    byte ptr [edi], 0
        db  000h, 03Dh, 026h, 000h, 000h, 03Dh
        add    byte ptr es:[eax], al
        db  03Dh, 026h, 000h, 000h, 03Dh
        add    byte ptr es:[eax], al
        db  03Dh, 026h, 000h, 000h, 03Dh
        add    byte ptr es:[eax], al
        db  03Dh, 026h, 000h, 000h, 03Dh
        add    byte ptr es:[eax], al
        db  03Dh, 026h, 000h, 000h, 0B2h
        sub    byte ptr [eax], al
        db  000h, 03Dh, 026h, 000h, 000h, 0B2h
        sub    byte ptr [eax], al
        db  000h, 03Dh, 026h, 000h, 000h, 03Dh
        add    byte ptr es:[eax], al
        db  03Dh, 026h, 000h, 000h, 03Dh
        add    byte ptr es:[eax], al
        db  03Dh, 026h, 000h, 000h, 03Dh
        add    byte ptr es:[eax], al
        db  03Dh, 026h, 000h, 000h, 03Dh
        add    byte ptr es:[eax], al
        db  03Dh, 026h, 000h, 000h, 03Dh
        add    byte ptr es:[eax], al
        db  03Dh, 026h, 000h, 000h, 03Dh
        add    byte ptr es:[eax], al
        shr    byte ptr [eax], 1
        add    byte ptr [eax], al
        add    eax, 0
        lea    esp, [esp]
sub_1_2A90:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 30h]
        lea    eax, [esp + 14h]
        mov    edi, dword ptr [esp + 34h]
        push   eax
        push   0
        lea    eax, [edi + 1ah]
        push   eax
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [esp + 34h]
        push   eax
        call   sub_1_2E40
        add    esp, 10h
        or     al, al
        jne    NEAR PTR loc_1_2B6C
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [eax + 44h], 0
        je     SHORT loc_1_2AD9
        mov    al, 15h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_2AD9:
        mov    eax, dword ptr [esp + 28h]
        push   eax
        mov    ebx, dword ptr [esp + 30h]
        push   0
        push   ebx
        call   sub_1_A810
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_2B02
        push   eax
        call   sub_1_0AD0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_2B02:
        mov    ebp, dword ptr [ebx]
        push   0
        mov    dword ptr [ebp + 24h], ebp
        mov    eax, dword ptr [esp + 18h]
        push   eax
        push   ebx
        call   sub_1_A978
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_2B4A
        lea    ecx, [edi + 32h]
        mov    dword ptr [esi], ecx
        mov    al, byte ptr [edi + 0ah]
        sub    al, 2ah
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   ecx
        call   sub_1_30E0
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_2B74
        push   ebx
        call   sub_1_A9FC
        add    esp, 4
loc_1_2B4A:
        mov    eax, dword ptr [esp + 28h]
        push   0
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   ebx
        call   sub_1_AD68
        mov    eax, dword ptr [esp + 20h]
        add    esp, 10h
        push   eax
        call   sub_1_0AD0
        add    esp, 4
loc_1_2B6C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_2B74:
        cmp    byte ptr [edi + 0ah], 2ah
        je     SHORT loc_1_2BDD
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 14h]
        mov    word ptr [eax + 4], 12h
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax]
        lea    ecx, [edi + 0ah]
        mov    eax, dword ptr [eax + 14h]
        mov    word ptr [eax + 6], 11h
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 14h]
        mov    word ptr [eax + 8], 0
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax]
        mov    edx, dword ptr [eax + 14h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx + 0ah], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 0eh], eax
        add    edx, 0ah
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 0ch], eax
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 10h], 1ah
loc_1_2BDD:
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 14h]
        mov    word ptr [eax + 4], 12h
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 4]
        lea    ecx, [edi + 1ah]
        mov    eax, dword ptr [eax + 14h]
        mov    word ptr [eax + 6], 11h
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 14h]
        mov    word ptr [eax + 8], 0
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 4]
        mov    edx, dword ptr [eax + 14h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx + 0ah], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 0eh], eax
        add    edx, 0ah
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 0ch], eax
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 10h], 1ah
        mov    cl, byte ptr [edi + 0ah]
        sub    cl, 2ah
        cmp    cl, 1
        sbb    cl, cl
        neg    cl
        mov    byte ptr [eax + 0ch], cl
        mov    dword ptr [eax + 8], ebp
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2C70:
        sub    esp, 14h
        push   esi
        mov    esi, dword ptr [esp + 1ch]
        or     esi, esi
        jne    SHORT loc_1_2C83
        mov    al, 40h
        pop    esi
        add    esp, 14h
        ret
loc_1_2C83:
        cmp    dword ptr [esp + 28h], 0
        je     SHORT loc_1_2CB7
        mov    al, byte ptr [esp + 20h]
        cmp    byte ptr [esi + 4eh], al
        jb     SHORT loc_1_2CB0
        xor    ecx, ecx
        mov    cl, al
        mov    eax, dword ptr [esi + 44h]
        mov    eax, dword ptr [eax + ecx*4]
        or     eax, eax
        je     SHORT loc_1_2CB0
        mov    edx, dword ptr [esp + 24h]
        pop    esi
        mov    dword ptr [edx], eax
        sub    al, al
        add    esp, 14h
        ret
        db  08Bh, 0FFh
loc_1_2CB0:
        mov    al, 13h
        pop    esi
        add    esp, 14h
        ret
loc_1_2CB7:
        lea    eax, [esp + 8]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        lea    eax, [esp + 4]
        mov    byte ptr [esp + 8], 2ah
        push   eax
        db  0A1h, 0D8h, 032h, 001h, 000h
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        db  0A1h, 0B4h, 032h, 001h, 000h
        push   eax
        call   sub_1_6954
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_2D10
        cmp    eax, 0ffh
        je     SHORT loc_1_2D10
        mov    al, 13h
        pop    esi
        add    esp, 14h
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_2D10:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [eax + 0ch]
        add    eax, 0ch
        cmp    ecx, eax
        je     SHORT loc_1_2D3B
loc_1_2D21:
        cmp    dword ptr [ecx + 80h], esi
        je     SHORT loc_1_2D31
        mov    ecx, dword ptr [ecx]
        cmp    ecx, eax
        jne    SHORT loc_1_2D21
        jmp    SHORT loc_1_2D37
loc_1_2D31:
        mov    edx, dword ptr [esp + 24h]
        mov    dword ptr [edx], ecx
loc_1_2D37:
        cmp    ecx, eax
        jne    SHORT loc_1_2D42
loc_1_2D3B:
        mov    al, 13h
        pop    esi
        add    esp, 14h
        ret
loc_1_2D42:
        sub    al, al
        pop    esi
        add    esp, 14h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2D50:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        or     esi, esi
        je     SHORT loc_1_2DB5
        mov    eax, dword ptr [esp + 10h]
        or     eax, eax
        je     SHORT loc_1_2DB5
        mov    bl, byte ptr [eax + 2]
        cmp    byte ptr [esi + 4fh], bl
        jb     SHORT loc_1_2DB0
        xor    eax, eax
        mov    ecx, dword ptr [esi + 48h]
        mov    al, bl
        mov    ecx, dword ptr [ecx + eax*4]
        or     ecx, ecx
        je     SHORT loc_1_2DB0
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ecx + 68h]
        cmp    eax, 1
        je     SHORT loc_1_2D8A
        or     eax, eax
        jne    SHORT loc_1_2DAA
loc_1_2D8A:
        cmp    dword ptr [ecx + 64h], 1
        jne    SHORT loc_1_2DAA
        push   ecx
        call   sub_1_DEE4
        add    esp, 4
        push   1
        push   ebx
        push   esi
        call   sub_1_2F70
        add    esp, 0ch
        mov    al, 0ah
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
        mov    al, 40h
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2DC0:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [edi], 0
        mov    dl, byte ptr [eax + 4fh]
        mov    ecx, dword ptr [eax + 48h]
        mov    eax, dword ptr [esp + 14h]
loc_1_2DD8:
        mov    bl, byte ptr [edi]
        mov    esi, ebx
        and    esi, 0ffh
        cmp    dword ptr [ecx + esi*4], eax
        je     SHORT loc_1_2DF5
        inc    bl
        mov    byte ptr [edi], bl
        cmp    bl, dl
        jbe    SHORT loc_1_2DD8
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
        mov    edi, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [edi], 0
        mov    cl, byte ptr [eax + 4eh]
        mov    ebx, dword ptr [eax + 44h]
        mov    esi, dword ptr [esp + 14h]
loc_1_2E18:
        mov    dl, byte ptr [edi]
        xor    eax, eax
        mov    al, dl
        mov    eax, dword ptr [ebx + eax*4]
        or     eax, eax
        je     SHORT loc_1_2E2A
        cmp    dword ptr [eax + 10h], esi
        je     SHORT loc_1_2E38
loc_1_2E2A:
        inc    dl
        mov    byte ptr [edi], dl
        cmp    dl, cl
        jbe    SHORT loc_1_2E18
        mov    al, 13h
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
        mov    ecx, dword ptr [esp + 14h]
        cmp    byte ptr [ecx], 2ah
        jne    SHORT loc_1_2E54
        mov    al, 15h
        pop    esi
        add    esp, 8
        ret
loc_1_2E54:
        lea    eax, [esp + 8]
        push   eax
        db  0A1h, 0D8h, 032h, 001h, 000h
        push   eax
        push   ecx
        db  0A1h, 0B4h, 032h, 001h, 000h
        push   eax
        call   sub_1_6954
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_2E80
        cmp    eax, 0ffh
        je     SHORT loc_1_2E80
        mov    al, 15h
        pop    esi
        add    esp, 8
        ret
loc_1_2E80:
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 28h]
        or     eax, eax
        je     SHORT loc_1_2EA0
        test   dword ptr [esi + 88h], eax
        jne    SHORT loc_1_2EA0
        mov    al, 15h
        pop    esi
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_2EA0:
        lea    eax, [esp + 7]
        push   eax
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        push   esi
        call   sub_1_2E00
        add    esp, 0ch
        or     al, al
        je     SHORT loc_1_2EC1
        mov    al, 15h
        pop    esi
        add    esp, 8
        ret
loc_1_2EC1:
        mov    eax, dword ptr [esp + 1ch]
        push   1
        push   eax
        mov    al, byte ptr [esp + 0fh]
        push   eax
        push   esi
        call   sub_1_2C70
        mov    eax, dword ptr [esp + 28h]
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_2EE4
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
        cmp    dword ptr [esp + 20h], eax
        mov    edi, dword ptr [esp + 14h]
        jne    SHORT loc_1_2F10
        mov    bl, byte ptr [edi + 4eh]
        mov    ecx, dword ptr [edi + 44h]
        add    edi, 4ch
        jmp    SHORT loc_1_2F19
        db  08Dh, 049h, 000h
loc_1_2F10:
        mov    bl, byte ptr [edi + 4fh]
        mov    ecx, dword ptr [edi + 48h]
        add    edi, 4dh
loc_1_2F19:
        mov    dl, byte ptr [edi]
loc_1_2F1B:
        cmp    bl, dl
        jae    SHORT loc_1_2F21
        mov    dl, 1
loc_1_2F21:
        mov    ebp, edx
        and    ebp, 0ffh
        cmp    dword ptr [ecx + ebp*4], 0
        je     SHORT loc_1_2F40
        cmp    dl, byte ptr [edi]
        jne    SHORT loc_1_2F3C
        or     esi, esi
        jne    SHORT loc_1_2F45
        mov    esi, 1
loc_1_2F3C:
        inc    dl
        jmp    SHORT loc_1_2F1B
loc_1_2F40:
        mov    eax, 1
loc_1_2F45:
        or     eax, eax
        je     SHORT loc_1_2F6B
        mov    ebp, edx
        and    ebp, 0ffh
        mov    esi, dword ptr [esp + 1ch]
        mov    dword ptr [ecx + ebp*4], esi
        mov    ecx, dword ptr [esp + 18h]
        mov    byte ptr [edi], dl
        mov    byte ptr [ecx], dl
        inc    dl
        mov    byte ptr [edi], dl
        cmp    bl, dl
        jae    SHORT loc_1_2F6B
        mov    byte ptr [edi], 1
loc_1_2F6B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_2F70:
        cmp    dword ptr [esp + 0ch], 0
        mov    eax, dword ptr [esp + 4]
        jne    SHORT loc_1_2F83
        mov    dl, byte ptr [eax + 4eh]
        mov    eax, dword ptr [eax + 44h]
        jmp    SHORT loc_1_2F89
loc_1_2F83:
        mov    dl, byte ptr [eax + 4fh]
        mov    eax, dword ptr [eax + 48h]
loc_1_2F89:
        mov    cl, byte ptr [esp + 8]
        cmp    dl, cl
        jb     SHORT loc_1_2FAB
        and    ecx, 0ffh
        lea    eax, [eax + ecx*4]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_2FAB
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
        lea    eax, [esp + 0bh]
        mov    esi, dword ptr [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   esi
        sub    edi, edi
        push   eax
        call   sub_1_2DC0
        add    esp, 0ch
        or     al, al
        je     SHORT loc_1_2FE0
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_2FE0:
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_1_301F
        mov    edx, dword ptr [eax + 3ch]
        mov    ecx, edx
        sub    ecx, eax
        cmp    ecx, 3ch
        je     SHORT loc_1_301F
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 1ch]
        mov    cl, byte ptr [esp + 0bh]
        mov    byte ptr [eax + 2], cl
        mov    edi, 1
        push   0
        push   0c000020ch
        mov    eax, dword ptr [edx + 1ch]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
loc_1_301F:
        mov    eax, dword ptr [esi + 1ch]
        sub    eax, esi
        cmp    eax, 1ch
        je     SHORT loc_1_305A
        mov    edi, 1
        mov    edi, edi
loc_1_3030:
        mov    edx, dword ptr [esi + 1ch]
        push   0
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0c000020ch
        mov    eax, dword ptr [edx + 1ch]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        mov    eax, dword ptr [esi + 1ch]
        sub    eax, esi
        cmp    eax, 1ch
        jne    SHORT loc_1_3030
loc_1_305A:
        or     edi, edi
        je     SHORT loc_1_3077
        mov    ax, word ptr [esi + 3ah]
        test   al, 1
        jne    SHORT loc_1_3077
        or     ax, 1
        mov    word ptr [esi + 3ah], ax
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
        sub    esp, 10h
        mov    eax, dword ptr [esp + 18h]
        or     eax, eax
        je     SHORT loc_1_30BC
        mov    dword ptr [esp], eax
        push   0
        push   0
        push   0
        push   2
        lea    eax, [esp + 10h]
        push   0
        push   eax
        call   sub_1_BDC0
        add    esp, 18h
        mov    eax, dword ptr [esp + 14h]
        push   0
        push   eax
        lea    eax, [esp + 8]
        push   0
        push   eax
        call   sub_1_AD68
        add    esp, 10h
loc_1_30BC:
        mov    eax, dword ptr [esp + 1ch]
        or     eax, eax
        je     SHORT loc_1_30CD
        push   eax
        call   sub_1_31C0
        add    esp, 4
loc_1_30CD:
        add    esp, 10h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_30E0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   18h
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 4], 0
        mov    dword ptr [esi + 8], 0
        mov    byte ptr [esi + 0ch], 0
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esi], eax
        or     eax, eax
        je     NEAR PTR loc_1_3195
        push   18h
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esi + 4], eax
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_3195
        mov    eax, dword ptr [esi]
        push   1ah
        mov    dword ptr [eax + 14h], 0
        mov    dword ptr [edi + 14h], 0
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [edi + 14h], eax
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [eax + 14h], 0
        je     SHORT loc_1_3170
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_3169
        push   1ah
        call   sub_1_001E
        add    esp, 4
        mov    ecx, dword ptr [esi]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 14h], 0
        je     SHORT loc_1_3170
loc_1_3169:
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_3170:
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 14h]
        or     eax, eax
        je     SHORT loc_1_3182
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_3182:
        mov    eax, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 14h]
        or     eax, eax
        je     SHORT loc_1_3195
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_3195:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_1_31A4
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_31A4:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_1_31B4
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
        mov    eax, dword ptr [eax + 14h]
        or     eax, eax
        je     SHORT loc_1_31D7
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_31D7:
        mov    eax, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 14h]
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
        mov    eax, dword ptr [esp + 10h]
        push   eax
        mov    esi, dword ptr [eax + 8]
        mov    edi, dword ptr [eax + 0ch]
        mov    ebx, dword ptr [eax + 10h]
        call   sub_1_0024
        add    esp, 4
        push   edi
        push   esi
        push   ebx
        call   sub_1_E64C
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3230:
        sub    esp, 4ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 68h]
        mov    dword ptr [esp + 18h], eax
        db  08Bh, 00Dh, 03Ch, 032h, 001h, 000h
        mov    ax, word ptr [esp + 60h]
        mov    word ptr [esp + 12h], ax
        and    eax, 0ffffh
        sub    eax, 65h
        cmp    eax, 0fh
        ja     SHORT loc_1_3263
        db  0FFh, 024h, 085h, 078h, 035h, 000h, 000h
loc_1_3263:
        mov    ebp, 0c00000bbh
        mov    ebx, dword ptr [esp + 64h]
        jmp    NEAR PTR loc_1_34F5
loc_1_3271:
        call   sub_1_DB70
        mov    ebp, eax
        mov    ebx, dword ptr [esp + 64h]
        jmp    NEAR PTR loc_1_34F5
loc_1_3281:
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   ecx
        call   sub_1_DBD0
        add    esp, 0ch
        mov    ebp, eax
        mov    ebx, dword ptr [esp + 64h]
        jmp    NEAR PTR loc_1_34F5
        db  090h
loc_1_32A0:
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   0
        call   sub_1_D968
        add    esp, 0ch
        mov    ebp, eax
        mov    ebx, dword ptr [esp + 64h]
        jmp    NEAR PTR loc_1_34F5
        db  090h
loc_1_32C0:
        mov    ebx, dword ptr [esp + 64h]
        or     ebx, ebx
        je     SHORT loc_1_32F3
        cmp    dword ptr [esp + 68h], 0
        je     SHORT loc_1_32F3
        cmp    word ptr [esp + 12h], 69h
        jne    SHORT loc_1_32D9
        sub    ecx, ecx
loc_1_32D9:
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   ecx
        call   sub_1_D62C
        add    esp, 0ch
        mov    ebp, eax
        jmp    NEAR PTR loc_1_34F5
loc_1_32F3:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
loc_1_3300:
        xor    eax, eax
        db  0A0h, 04Fh, 033h, 001h, 000h
        lea    eax, [eax*4 + 4]
        cmp    eax, dword ptr [esp + 68h]
        jbe    SHORT loc_1_3321
        mov    eax, 9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
loc_1_3321:
        mov    ebx, dword ptr [esp + 64h]
        or     ebx, ebx
        jne    SHORT loc_1_3336
        mov    eax, 0c000000dh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
loc_1_3336:
        mov    ecx, ebx
        db  08Bh, 015h, 03Ch, 032h, 001h, 000h
        db  081h, 0FAh, 03Ch, 032h, 001h, 000h
        je     SHORT loc_1_335C
loc_1_3346:
        mov    eax, dword ptr [edx + 58h]
        or     eax, eax
        je     SHORT loc_1_3352
        mov    dword ptr [ecx], eax
        add    ecx, 4
loc_1_3352:
        mov    edx, dword ptr [edx]
        db  081h, 0FAh, 03Ch, 032h, 001h, 000h
        jne    SHORT loc_1_3346
loc_1_335C:
        mov    dword ptr [ecx], 0
        sub    ebp, ebp
        jmp    NEAR PTR loc_1_34F5
loc_1_3369:
        mov    ebx, dword ptr [esp + 64h]
        or     ebx, ebx
        jne    SHORT loc_1_3380
        mov    eax, 0c000000dh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
        db  08Bh, 0FFh
loc_1_3380:
        lea    edi, [esp + 1ch]
        sub    eax, eax
        mov    ecx, 10h
        rep stosd
        mov    byte ptr [esp + 1ch], 33h
        cmp    dword ptr [esp + 6ch], 0
        mov    ax, word ptr [esp + 68h]
        mov    word ptr [esp + 24h], ax
        db  0A1h, 03Ch, 032h, 001h, 000h
        mov    dword ptr [esp + 20h], ebx
        mov    al, byte ptr [eax + 51h]
        mov    byte ptr [esp + 4ch], al
        jne    SHORT loc_1_33C0
        mov    eax, 0c000000dh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
loc_1_33C0:
        mov    eax, dword ptr [esp + 6ch]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_33D3
        mov    byte ptr [esp + 26h], 2ah
        mov    eax, dword ptr [eax]
        push   eax
        jmp    SHORT loc_1_33F7
loc_1_33D3:
        mov    ecx, dword ptr [esp + 6ch]
        lea    edx, [esp + 26h]
        mov    eax, dword ptr [ecx + 0eh]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 12h]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 16h]
        add    ecx, 0eh
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 0ch], eax
        push   0
loc_1_33F7:
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_0770
        add    esp, 8
        cmp    byte ptr [esp + 1dh], 0
        jne    SHORT loc_1_3412
        sub    ebp, ebp
        jmp    NEAR PTR loc_1_34F5
loc_1_3412:
        cmp    byte ptr [esp + 1dh], 6
        jne    SHORT loc_1_3420
        jmp    NEAR PTR loc_1_34F0
        db  08Bh, 0FFh
loc_1_3420:
        mov    ebp, 0c0000001h
        jmp    NEAR PTR loc_1_34F5
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3430:
        xor    eax, eax
        db  066h, 0A1h, 0D4h, 032h, 001h, 000h
        add    eax, 0b7h
        mov    ebx, dword ptr [esp + 64h]
        or     ebx, ebx
        je     NEAR PTR loc_1_34F0
        cmp    eax, dword ptr [esp + 68h]
        ja     NEAR PTR loc_1_34F0
        sub    si, si
        db  0BAh, 0C4h, 033h, 001h, 000h
loc_1_345B:
        mov    edi, dword ptr [edx]
        or     edi, edi
        je     SHORT loc_1_34A2
        mov    eax, esi
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        lea    ecx, [ebx + eax*2]
        mov    al, byte ptr [edi + 51h]
        and    eax, 0ffff00ffh
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [edi + 58h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edi + 74h]
        mov    dword ptr [ecx + 8], eax
        inc    si
        mov    eax, dword ptr [edi + 78h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [edi + 7ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [edi + 80h]
        mov    dword ptr [ecx + 14h], eax
loc_1_34A2:
        add    edx, 4
        db  081h, 0FAh, 0E4h, 033h, 001h, 000h
        jb     SHORT loc_1_345B
        mov    word ptr [ebx], si
        db  066h, 0A1h, 064h, 033h, 001h, 000h
        mov    word ptr [ebx + 0b2h], ax
        db  066h, 0A1h, 0D4h, 032h, 001h, 000h
        lea    edi, [ebx + 0b6h]
        mov    word ptr [ebx + 0b4h], ax
        xor    eax, eax
        db  066h, 0A1h, 0D4h, 032h, 001h, 000h
        db  08Bh, 035h, 0D8h, 032h, 001h, 000h
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        sub    ebp, ebp
        jmp    SHORT loc_1_34F5
loc_1_34F0:
        mov    ebp, 9
loc_1_34F5:
        cmp    word ptr [esp + 12h], 68h
        je     SHORT loc_1_3515
        cmp    word ptr [esp + 12h], 69h
        je     SHORT loc_1_3515
        cmp    word ptr [esp + 12h], 67h
        je     SHORT loc_1_3515
        cmp    word ptr [esp + 12h], 66h
        jne    SHORT loc_1_3558
loc_1_3515:
        or     ebp, ebp
        je     SHORT loc_1_3526
        cmp    ebp, 0ffh
        je     SHORT loc_1_3526
        cmp    ebp, 9
        jne    SHORT loc_1_3558
loc_1_3526:
        cmp    ebp, 9
        jne    SHORT loc_1_3531
        mov    eax, dword ptr [esp + 68h]
        jmp    SHORT loc_1_3537
loc_1_3531:
        mov    eax, dword ptr [esp + 18h]
        sub    ebp, ebp
loc_1_3537:
        mov    edi, ebx
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 14h]
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_3558:
        cmp    dword ptr [esp + 6ch], 0
        je     SHORT loc_1_356C
        cmp    dword ptr [esp + 74h], 0
        je     SHORT loc_1_356C
        mov    eax, dword ptr [esp + 6ch]
        mov    dword ptr [eax], ebp
loc_1_356C:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
        db  08Bh, 0FFh, 071h, 032h, 000h, 000h, 081h, 032h, 000h, 000h, 0A0h, 032h, 000h, 000h, 0C0h, 032h
        db  000h, 000h, 0C0h, 032h, 000h, 000h, 063h, 032h, 000h, 000h, 063h, 032h, 000h, 000h, 063h, 032h
        db  000h, 000h, 063h, 032h, 000h, 000h, 063h, 032h, 000h, 000h, 063h, 032h, 000h, 000h, 063h, 032h
        db  000h, 000h, 000h, 033h, 000h, 000h, 063h, 032h, 000h, 000h, 069h, 033h, 000h, 000h, 030h, 034h
        db  000h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_35C0:
        db  0C6h, 005h, 088h, 033h, 001h, 000h, 000h
        push   0
        push   1
        push   0
        db  0A1h, 027h, 033h, 001h, 000h
        push   eax
        call   sub_1_12638
        add    esp, 10h
        cmp    eax, -1
        je     SHORT loc_1_35E7
        db  0C6h, 005h, 088h, 033h, 001h, 000h, 001h
loc_1_35E7:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_35F0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        and    esi, 0ffffh
        push   esi
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_361F
        db  083h, 03Dh, 0CCh, 02Eh, 001h, 000h, 000h
        je     SHORT loc_1_361F
        call   sub_1_002A
        push   esi
        call   sub_1_001E
        add    esp, 4
loc_1_361F:
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3630:
        sub    esp, 10h
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 12h
        rep stosd
        stosb
        mov    dword ptr [esi + 38h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esi + 18h], eax
        lea    eax, [esi + 28h]
        mov    dword ptr [esi + 3ch], 1
        mov    byte ptr [esi + 44h], 20h
        mov    dword ptr [esi + 8], 11223344h
        mov    byte ptr [esi + 48h], 0
        mov    dword ptr [esi + 2ch], eax
        mov    dword ptr [eax], eax
        lea    eax, [esp + 8]
        push   esi
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        call   dword ptr [eax + 8]
        mov    ecx, dword ptr [esp + 10h]
        add    esp, 8
        mov    dword ptr [esi + 10h], ecx
        pop    edi
        pop    esi
        add    esp, 10h
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_1_3690:
        sub    esp, 10h
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp], eax
        mov    eax, dword ptr [esp + 18h]
        push   eax
        lea    eax, [esp + 4]
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        call   dword ptr [eax + 10h]
        add    esp, 18h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_36C0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        push   49h
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esi + 0ch], eax
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_36EB
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_36EB:
        mov    edi, dword ptr [esp + 1ch]
        push   edi
        push   ebp
        call   sub_1_3630
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_3720
        cmp    ebx, 0ffh
        je     SHORT loc_1_3720
        mov    eax, dword ptr [esi + 0ch]
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
        mov    dword ptr [ebp + 0ch], esi
        mov    dword ptr [ebp + 38h], 3
        or     ebx, ebx
        je     SHORT loc_1_3736
        cmp    ebx, 0ffh
        jne    SHORT loc_1_37AD
loc_1_3736:
        push   3
        mov    eax, dword ptr [edi + 58h]
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   eax
        lea    eax, [ebp + 14h]
        push   edi
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        push   0
        call   sub_1_3D60
        add    esp, 1ch
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_3763
        cmp    ebx, 0ffh
        jne    SHORT loc_1_3797
loc_1_3763:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_3690
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_37C7
        cmp    ebx, 0ffh
        je     SHORT loc_1_37C7
        push   ebp
        call   sub_1_3820
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_3797
        push   ebp
        call   sub_1_3820
        add    esp, 4
loc_1_3797:
        push   ebp
        call   sub_1_37F0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_37AD
        push   ebp
        call   sub_1_37F0
        add    esp, 4
loc_1_37AD:
        mov    dword ptr [esi + 0ch], 0
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
        mov    eax, dword ptr [edi + 18h]
        add    edi, 14h
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
        sub    esp, 10h
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp], eax
        db  0C7h, 044h, 024h, 004h, 070h, 038h, 000h, 000h
        lea    eax, [esp]
        mov    dword ptr [esp + 8], 0
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        call   dword ptr [eax + 0ch]
        add    esp, 14h
        ret
        db  090h
sub_1_3820:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_3840
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3840:
        sub    esp, 10h
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp], eax
        db  0C7h, 044h, 024h, 004h, 070h, 038h, 000h, 000h
        mov    dword ptr [esp + 8], 0
        lea    eax, [esp]
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        call   dword ptr [eax + 4]
        add    esp, 14h
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
        db  068h, 094h, 032h, 001h, 000h
        call   sub_1_FEF4
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_38B2
        mov    eax, 1
        mov    esi, dword ptr [esp + 0ch]
        jmp    NEAR PTR loc_1_3980
loc_1_38B2:
        mov    esi, dword ptr [esp + 0ch]
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
        db  0C7h, 046h, 004h, 0B0h, 039h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 1ch], 4655424eh
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 4]
        add    eax, 20h
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, dword ptr [esp + 1ch]
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx + 18h], eax
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 10h], eax
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 30h], 4655424eh
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 20h], 0
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 2ch], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 28h], 0
        mov    eax, dword ptr [esp + 4]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        push   esi
        call   dword ptr [eax + 30h]
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_39A0
        cmp    eax, 0ffh
        je     SHORT loc_1_39A0
loc_1_3980:
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_39A0
        mov    ecx, dword ptr [esi + 4]
        or     ecx, ecx
        je     SHORT loc_1_39A0
        push   0
        push   eax
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        call   ecx
        add    esp, 0ch
        mov    eax, 0ffh
loc_1_39A0:
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_39B0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        or     esi, esi
        je     SHORT loc_1_39EE
        mov    ecx, dword ptr [esi + 4]
        or     ecx, ecx
        je     SHORT loc_1_39D3
        mov    eax, dword ptr [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   ecx
        add    esp, 0ch
loc_1_39D3:
        db  0C7h, 006h, 094h, 032h, 001h, 000h
        db  0A1h, 098h, 032h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 098h, 032h, 001h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 098h, 032h, 001h, 000h
loc_1_39EE:
        pop    esi
        ret
sub_1_39F0:
        push   esi
        mov    eax, dword ptr [esp + 10h]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 10h]
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        push   esi
        call   dword ptr [eax + 18h]
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_3A2A
        cmp    eax, 0ffh
        je     SHORT loc_1_3A2A
        push   0
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   dword ptr [esi + 4]
        add    esp, 0ch
        mov    eax, 0ffh
loc_1_3A2A:
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_3A30:
        mov    eax, dword ptr [esp + 10h]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        call   dword ptr [eax + 1ch]
        add    esp, 14h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3A60:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], 0
        sub    ebx, ebx
        sub    ebp, ebp
        lea    eax, [esp + 10h]
        push   3
        push   eax
        db  068h, 094h, 032h, 001h, 000h
        call   sub_1_FEF4
        add    esp, 0ch
        cmp    eax, ebx
        mov    esi, dword ptr [esp + 18h]
        mov    eax, dword ptr [esi + 4]
        je     SHORT loc_1_3AB0
        cmp    eax, ebx
        je     SHORT loc_1_3AA2
        push   ebx
        push   1
        mov    ecx, dword ptr [esi + 8]
        push   ecx
        call   eax
        add    esp, 0ch
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
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 10h]
        test   byte ptr [esp + 2ch], 80h
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 8], eax
        db  0C7h, 046h, 004h, 0B0h, 039h, 000h, 000h
        mov    ecx, dword ptr [esp + 28h]
        je     SHORT loc_1_3B24
        mov    edx, dword ptr [ecx + 14h]
        or     edx, edx
        je     SHORT loc_1_3B24
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 44h], 4655424eh
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 34h], 0
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 40h], edx
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [ecx + 10h]
        mov    dword ptr [edx + 38h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 3ch], 0
        mov    ebx, dword ptr [esp + 10h]
        add    ebx, 34h
loc_1_3B24:
        mov    edx, dword ptr [ecx + 8]
        or     edx, edx
        mov    edi, dword ptr [esp + 20h]
        je     SHORT loc_1_3B68
        cmp    dword ptr [ecx], edi
        jae    SHORT loc_1_3B68
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 30h], 4655424eh
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 20h], ebx
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 2ch], edx
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 24h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 28h], 0
        mov    ebp, dword ptr [esp + 10h]
        add    ebp, 20h
loc_1_3B68:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 1ch], 4655424eh
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 0ch], ebp
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx + 18h], eax
        mov    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [esp + 10h]
        add    eax, 0ch
        push   eax
        mov    ax, word ptr [esp + 20h]
        push   edi
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        push   esi
        call   dword ptr [eax + 2ch]
        add    esp, 10h
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_3BE6
        cmp    ebx, 0ffh
        je     SHORT loc_1_3BE6
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_3BDC
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_1_3BDC
        push   0
        mov    ecx, dword ptr [esp + 14h]
        push   ebx
        push   ecx
        call   eax
        add    esp, 0ch
loc_1_3BDC:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_3BE6:
        mov    eax, dword ptr [esp + 24h]
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
        push   39h
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_3C16
        mov    eax, 1
        pop    esi
        ret
loc_1_3C16:
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 0ch], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 10h], ecx
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [eax + 29h], ecx
        mov    esi, dword ptr [esp + 18h]
        or     esi, esi
        mov    dword ptr [eax + 2dh], esi
        je     SHORT loc_1_3C66
        cmp    byte ptr [esi + 52h], 0
        jne    SHORT loc_1_3C50
        push   eax
        call   ecx
        add    esp, 4
        sub    eax, eax
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_3C50:
        mov    edx, dword ptr [esi + 40h]
        add    esi, 3ch
        mov    dword ptr [eax + 31h], esi
        lea    ecx, [eax + 31h]
        mov    dword ptr [eax + 35h], edx
        mov    dword ptr [edx], ecx
        mov    dword ptr [esi + 4], ecx
        jmp    SHORT loc_1_3C87
loc_1_3C66:
        db  0C7h, 040h, 031h, 0A4h, 032h, 001h, 000h
        lea    edx, [eax + 31h]
        db  08Bh, 00Dh, 0A8h, 032h, 001h, 000h
        mov    dword ptr [eax + 35h], ecx
        db  08Bh, 00Dh, 0A8h, 032h, 001h, 000h
        mov    dword ptr [ecx], edx
        db  089h, 015h, 0A8h, 032h, 001h, 000h
loc_1_3C87:
        mov    dword ptr [eax + 21h], 0
        db  0C7h, 040h, 019h, 0C0h, 03Ch, 000h, 000h
        db  08Bh, 00Dh, 068h, 02Eh, 001h, 000h
        mov    dword ptr [eax + 25h], ecx
        push   eax
        add    eax, 15h
        push   eax
        call   sub_1_0012
        add    esp, 8
        mov    eax, 0ffh
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3CC0:
        mov    edx, dword ptr [esp + 8]
        cmp    dword ptr [edx + 2dh], 0
        mov    eax, dword ptr [edx + 35h]
        mov    ecx, dword ptr [edx + 31h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        je     SHORT loc_1_3CE0
        push   edx
        call   dword ptr [edx + 29h]
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_3CE0:
        push   edx
        call   dword ptr [edx + 29h]
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3CF0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], 0
        mov    esi, dword ptr [esp + 18h]
        or     esi, esi
        lea    edi, [esi + 3ch]
        jne    SHORT loc_1_3D0F
        db  0BFh, 0A4h, 032h, 001h, 000h
loc_1_3D0F:
        mov    ebx, dword ptr [edi]
        cmp    ebx, edi
        je     SHORT loc_1_3D4B
        mov    dword ptr [esp + 10h], 1
loc_1_3D20:
        lea    ebp, [ebx - 31h]
        mov    ebx, dword ptr [ebx]
        mov    eax, dword ptr [ebp + 35h]
        mov    ecx, dword ptr [ebp + 31h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        lea    eax, [ebp + 15h]
        push   eax
        call   sub_1_0018
        add    esp, 4
        or     esi, esi
        je     SHORT loc_1_3D47
        push   ebp
        call   dword ptr [ebp + 29h]
        add    esp, 4
loc_1_3D47:
        cmp    ebx, edi
        jne    SHORT loc_1_3D20
loc_1_3D4B:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3D60:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 20h], 1
        mov    word ptr [esp + 24h], 0eh
        mov    word ptr [esp + 26h], 2
        mov    bx, word ptr [esp + 4ch]
        mov    ah, bl
        mov    al, bh
        mov    word ptr [esp + 28h], ax
        mov    eax, dword ptr [esp + 50h]
        mov    edx, eax
        and    edx, 0ff00h
        mov    ecx, eax
        shl    ecx, 10h
        or     edx, ecx
        shl    edx, 8
        mov    ecx, eax
        sar    ecx, 18h
        mov    dl, cl
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        lea    eax, [esp + 2eh]
        mov    dword ptr [esp + 2ah], edx
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    esi, dword ptr [esp + 54h]
        mov    ebp, esi
        and    ebp, 2
        push   0
        cmp    ebp, 1
        sbb    eax, eax
        and    eax, 0bh
        add    eax, 6
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        call   dword ptr [eax]
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_1_3E87
        mov    edi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 44h]
        mov    ecx, edi
        mov    dword ptr [eax], ecx
        test   esi, 1
        je     SHORT loc_1_3E48
        mov    esi, dword ptr [esp + 48h]
        push   esi
        db  068h, 090h, 03Eh, 000h, 000h
        push   3
        db  0A1h, 094h, 034h, 001h, 000h
        push   edi
        call   dword ptr [eax + 38h]
        add    esp, 10h
        push   esi
        db  068h, 030h, 047h, 000h, 000h
        push   1
        db  0A1h, 094h, 034h, 001h, 000h
        push   edi
        call   dword ptr [eax + 38h]
        add    esp, 10h
        or     ebp, ebp
        je     SHORT loc_1_3E73
        push   esi
        db  068h, 060h, 046h, 000h, 000h
        push   0
        jmp    SHORT loc_1_3E67
loc_1_3E48:
        cmp    bx, 8ah
        mov    esi, dword ptr [esp + 48h]
        push   esi
        jne    SHORT loc_1_3E60
        db  068h, 070h, 048h, 000h, 000h
        jmp    SHORT loc_1_3E65
        db  005h, 000h, 000h, 000h, 000h
loc_1_3E60:
        db  068h, 060h, 049h, 000h, 000h
loc_1_3E65:
        push   4
loc_1_3E67:
        db  0A1h, 094h, 034h, 001h, 000h
        push   edi
        call   dword ptr [eax + 38h]
        add    esp, 10h
loc_1_3E73:
        push   esi
        db  068h, 060h, 04Bh, 000h, 000h
        push   2
        db  0A1h, 094h, 034h, 001h, 000h
        push   edi
        call   dword ptr [eax + 38h]
        add    esp, 10h
loc_1_3E87:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  090h
sub_1_3E90:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], 0
        mov    edi, dword ptr [esp + 28h]
        mov    esi, dword ptr [esp + 34h]
        add    dword ptr [edi + 20h], esi
        mov    eax, dword ptr [edi + 38h]
        cmp    eax, 7
        jne    NEAR PTR loc_1_42B0
        mov    ebp, dword ptr [edi + 0ch]
        mov    ebx, dword ptr [esp + 38h]
        mov    dword ptr [ebp + 40h], esi
        mov    dword ptr [ebx], 0
        xor    eax, eax
        mov    ax, word ptr [edi + 32h]
        or     eax, eax
        je     SHORT loc_1_3EF7
        cmp    eax, 1
        je     NEAR PTR loc_1_4190
        cmp    eax, 2
        je     NEAR PTR loc_1_41F0
        cmp    eax, 3
        je     NEAR PTR loc_1_42A2
        mov    eax, dword ptr [esp + 1ch]
loc_1_3EEF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_3EF7:
        cmp    dword ptr [esp + 30h], 4
        jae    NEAR PTR loc_1_3FB2
        lea    eax, [esp + 10h]
        push   4
        push   eax
        db  068h, 09Ch, 032h, 001h, 000h
        call   sub_1_FEF4
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_3F27
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_3F27:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 14h], 1900beefh
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 18h], edi
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 40h]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax + 35h], 20h
        mov    edx, dword ptr [esp + 10h]
        add    edx, 20h
        mov    dword ptr [ecx], edx
        db  0C7h, 041h, 008h, 000h, 045h, 000h, 000h
        mov    dword ptr [ecx + 4], 4
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 0ch], eax
        lea    eax, [edi + 1ch]
        db  0C7h, 041h, 010h, 050h, 02Fh, 001h, 000h
        mov    dword ptr [edx + 10h], 4655424eh
        mov    dword ptr [edx], 0
        mov    dword ptr [edx + 0ch], 4
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edx + 8], 0
        mov    word ptr [edi + 32h], 1
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebp + 60h], eax
        mov    eax, 18h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_3FB2:
        mov    eax, dword ptr [esp + 3ch]
        mov    eax, dword ptr [eax]
        mov    ecx, eax
        and    ecx, 0ff00h
        shl    ecx, 8
        mov    edx, eax
        and    edx, 0ff0000h
        shr    edx, 8
        or     ecx, edx
        shr    eax, 18h
        and    eax, 0ffh
        or     ecx, eax
        lea    eax, [ecx + 4]
        mov    dword ptr [esp + 18h], eax
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 40h]
        push   eax
        push   ebx
        mov    eax, dword ptr [esp + 3ch]
        push   esi
        push   eax
        mov    ax, word ptr [esp + 40h]
        push   eax
        mov    eax, dword ptr [esp + 3ch]
        push   edi
        push   eax
        call   sub_1_6A34
        add    esp, 20h
        mov    ecx, dword ptr [ebx]
        sub    dword ptr [ebp + 40h], ecx
        sub    dword ptr [esp + 30h], ecx
        sub    esi, ecx
        add    dword ptr [esp + 3ch], ecx
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_1_4104
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 18h], edi
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 1ch]
        add    eax, 2
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   ebp
        mov    eax, dword ptr [eax + 80h]
        push   eax
        call   sub_1_2DC0
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 3ch]
        mov    ecx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [ecx + 2ch]
        cmp    eax, ecx
        jb     SHORT loc_1_4055
        mov    eax, ecx
loc_1_4055:
        mov    word ptr [edi + 32h], 2
        mov    dword ptr [ebp + 60h], 0
        mov    dword ptr [ebp + 50h], 0
        cmp    dword ptr [esp + 30h], eax
        jb     SHORT loc_1_40B0
        mov    ecx, dword ptr [esp + 10h]
        cmp    word ptr [ecx + 35h], 20h
        jne    SHORT loc_1_40B0
        mov    edi, dword ptr [ecx + 24h]
        mov    esi, dword ptr [esp + 3ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    dword ptr [ebx], eax
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   0
        push   eax
        call   sub_1_4380
        add    esp, 0ch
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_40B0:
        mov    edx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 40h]
        add    edx, 20h
        mov    dword ptr [ecx], edx
        db  0C7h, 041h, 008h, 080h, 043h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [esp + 10h]
        add    eax, 35h
        cmp    word ptr [eax], 100h
        jne    SHORT loc_1_40F0
        mov    dword ptr [ecx + 10h], eax
        mov    eax, 18h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_40F0:
        db  0C7h, 041h, 010h, 050h, 02Fh, 001h, 000h
        mov    eax, 18h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_4104:
        mov    ecx, dword ptr [ebx]
        cmp    dword ptr [esp + 18h], ecx
        ja     SHORT loc_1_4165
        cmp    eax, 17h
        jne    SHORT loc_1_411D
        cmp    dword ptr [ebp + 3ch], 0
        jne    SHORT loc_1_411D
        cmp    dword ptr [ebp + 68h], 7
        je     SHORT loc_1_4165
loc_1_411D:
        sub    eax, eax
        cmp    ecx, esi
        jae    NEAR PTR loc_1_3EEF
        mov    dword ptr [esp + 14h], eax
        sub    dword ptr [edi + 20h], esi
        mov    ecx, dword ptr [esp + 40h]
        mov    eax, dword ptr [esp + 3ch]
        push   ecx
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 3ch]
        push   esi
        push   eax
        mov    ax, word ptr [esp + 40h]
        push   eax
        mov    eax, dword ptr [esp + 3ch]
        push   edi
        push   eax
        call   sub_1_3E90
        mov    ecx, dword ptr [esp + 34h]
        add    esp, 20h
        add    dword ptr [ebx], ecx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_4165:
        mov    word ptr [edi + 32h], 3
        mov    ecx, dword ptr [edi + 18h]
        mov    eax, dword ptr [ecx + 30h]
        add    ecx, 2ch
        mov    dword ptr [edi + 28h], ecx
        lea    edx, [edi + 28h]
        mov    dword ptr [edi + 2ch], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 4], edx
        mov    byte ptr [edi + 48h], 1
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_4190:
        mov    eax, dword ptr [ebp + 60h]
        mov    dword ptr [esp + 10h], eax
        xor    ebx, ebx
        mov    bx, word ptr [edi + 30h]
        lea    eax, [edi + ebx]
        mov    ecx, dword ptr [esp + 10h]
        add    eax, 1ch
        mov    dword ptr [ecx + 24h], eax
        mov    edx, 4
        mov    eax, dword ptr [esp + 10h]
        sub    edx, ebx
        mov    dword ptr [eax + 2ch], edx
        cmp    edx, esi
        mov    ecx, dword ptr [esp + 40h]
        jb     SHORT loc_1_41C2
        mov    edx, esi
loc_1_41C2:
        mov    dword ptr [ecx + 4], edx
        mov    eax, dword ptr [esp + 10h]
        add    eax, 20h
        mov    dword ptr [ecx], eax
        db  0C7h, 041h, 008h, 000h, 045h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, 18h
        db  0C7h, 041h, 010h, 050h, 02Fh, 001h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_41F0:
        mov    edx, dword ptr [ebp + 60h]
        xor    eax, eax
        mov    ax, word ptr [edx + 8]
        sub    eax, dword ptr [ebp + 50h]
        mov    ecx, dword ptr [edx + 32h]
        mov    dword ptr [esp + 10h], ecx
        mov    ecx, dword ptr [ebp + 3ch]
        sub    ecx, dword ptr [ebp + 44h]
        cmp    eax, ecx
        jb     SHORT loc_1_420F
        mov    eax, ecx
loc_1_420F:
        mov    dword ptr [ebp + 60h], 0
        mov    edi, dword ptr [ebp + 50h]
        mov    ecx, dword ptr [edx + 4]
        mov    esi, dword ptr [esp + 10h]
        add    ecx, edi
        mov    dword ptr [esi + 24h], ecx
        xor    ecx, ecx
        mov    cx, word ptr [edx + 8]
        mov    edx, dword ptr [esp + 10h]
        sub    ecx, edi
        mov    dword ptr [edx + 2ch], ecx
        cmp    dword ptr [esp + 30h], eax
        jb     SHORT loc_1_4270
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 3ch]
        mov    edi, dword ptr [ecx + 24h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    dword ptr [ebx], eax
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   0
        push   eax
        call   sub_1_4380
        add    esp, 0ch
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_4270:
        mov    edx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 40h]
        add    edx, 20h
        mov    dword ptr [ecx], edx
        db  0C7h, 041h, 008h, 080h, 043h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, 18h
        db  0C7h, 041h, 010h, 050h, 02Fh, 001h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_42A2:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_42B0:
        cmp    eax, 4
        jne    SHORT loc_1_42E6
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 40h]
        mov    ebx, dword ptr [esp + 3ch]
        push   eax
        push   ebx
        mov    eax, dword ptr [esp + 3ch]
        push   esi
        push   eax
        mov    ax, word ptr [esp + 40h]
        push   eax
        mov    eax, dword ptr [esp + 3ch]
        push   edi
        push   eax
        call   sub_1_6C30
        add    esp, 20h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_42E6:
        cmp    eax, 6
        jne    SHORT loc_1_4320
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 40h]
        mov    ebx, dword ptr [esp + 3ch]
        push   eax
        push   ebx
        mov    eax, dword ptr [esp + 3ch]
        push   esi
        push   eax
        mov    ax, word ptr [esp + 40h]
        push   eax
        mov    eax, dword ptr [esp + 3ch]
        push   edi
        push   eax
        call   sub_1_6FC8
        add    esp, 20h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_4320:
        mov    ebx, dword ptr [esp + 38h]
        sub    eax, eax
        mov    dword ptr [ebx], esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_4330:
        push   esi
        mov    edx, dword ptr [esp + 24h]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx], 0
        mov    esi, dword ptr [eax + 3ch]
        mov    ecx, esi
        sub    ecx, eax
        cmp    ecx, 3ch
        je     SHORT loc_1_4350
        jmp    SHORT loc_1_4362
        db  08Dh, 049h, 000h
loc_1_4350:
        mov    eax, dword ptr [eax + 80h]
        mov    esi, dword ptr [eax + 24h]
        mov    ecx, esi
        sub    ecx, eax
        cmp    ecx, 24h
        je     SHORT loc_1_4375
loc_1_4362:
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, 18h
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
        mov    edx, dword ptr [esp + 1ch]
        or     edx, edx
        je     SHORT loc_1_43C4
        cmp    edx, 9
        je     SHORT loc_1_43C4
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [ecx + 18h]
        or     eax, eax
        je     SHORT loc_1_43AD
        mov    eax, dword ptr [eax + 0ch]
        or     eax, eax
        je     SHORT loc_1_43AD
        mov    dword ptr [eax + 60h], 0
loc_1_43AD:
        push   0
        push   edx
        mov    eax, dword ptr [ecx + 1ch]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_43C4:
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [ecx + 18h]
        mov    edi, dword ptr [esi + 0ch]
        mov    edx, dword ptr [esp + 20h]
        add    dword ptr [edi + 44h], edx
        add    dword ptr [edi + 50h], edx
        mov    eax, dword ptr [edi + 40h]
        cmp    edx, eax
        jb     SHORT loc_1_43E8
        mov    dword ptr [edi + 40h], 0
        jmp    SHORT loc_1_43ED
loc_1_43E8:
        sub    eax, edx
        mov    dword ptr [edi + 40h], eax
loc_1_43ED:
        mov    edx, dword ptr [edi + 3ch]
        mov    eax, dword ptr [edi + 44h]
        cmp    edx, eax
        jne    SHORT loc_1_4460
        mov    dword ptr [edi + 60h], 0
        mov    word ptr [esi + 32h], 0
        mov    eax, dword ptr [edi + 50h]
        push   eax
        push   0
        mov    eax, dword ptr [ecx + 1ch]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        mov    dword ptr [edi + 50h], 0
        mov    dword ptr [edi + 44h], 0
        mov    word ptr [esi + 30h], 0
        cmp    dword ptr [edi + 40h], 0
        je     NEAR PTR loc_1_44EE
        mov    dword ptr [esp + 10h], 0
        lea    eax, [esp + 10h]
        push   0
        push   eax
        mov    eax, dword ptr [edi + 40h]
        push   eax
        push   0
        push   0
        push   esi
        push   0
        call   sub_1_45B0
        add    esp, 1ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_4460:
        jae    SHORT loc_1_4494
        mov    dword ptr [edi + 60h], 0
        mov    word ptr [esi + 32h], 0
        mov    dword ptr [edi + 44h], 0
        mov    word ptr [esi + 30h], 0
        push   0
        push   19h
        mov    eax, dword ptr [ecx + 1ch]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_4494:
        mov    ebp, dword ptr [ecx + 1ch]
        xor    ebx, ebx
        mov    bx, word ptr [ebp + 8]
        cmp    dword ptr [edi + 50h], ebx
        jb     SHORT loc_1_44E5
        mov    dword ptr [edi + 60h], 0
        mov    dword ptr [edi + 50h], 0
        mov    word ptr [esi + 32h], 3
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [ecx + 30h]
        add    ecx, 2ch
        mov    dword ptr [esi + 28h], ecx
        lea    edx, [esi + 28h]
        mov    dword ptr [esi + 2ch], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 4], edx
        mov    byte ptr [esi + 48h], 1
        push   ebx
        push   9
        push   ebp
        call   sub_1_25A0
        add    esp, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_44E5:
        mov    dword ptr [edi + 60h], ebp
        mov    word ptr [esi + 32h], 2
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
        mov    edx, dword ptr [esp + 10h]
        cmp    dword ptr [esp + 14h], 0
        mov    ecx, dword ptr [edx + 18h]
        mov    esi, dword ptr [ecx + 0ch]
        je     SHORT loc_1_4540
        db  0C7h, 002h, 09Ch, 032h, 001h, 000h
        db  0A1h, 0A0h, 032h, 001h, 000h
        mov    dword ptr [edx + 4], eax
        db  0A1h, 0A0h, 032h, 001h, 000h
        pop    edi
        mov    dword ptr [eax], edx
        db  089h, 015h, 0A0h, 032h, 001h, 000h
        mov    word ptr [ecx + 32h], 0
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_4540:
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [esi + 40h], eax
        jae    SHORT loc_1_454C
        mov    dword ptr [esi + 40h], eax
loc_1_454C:
        add    ax, word ptr [ecx + 30h]
        mov    word ptr [ecx + 30h], ax
        cmp    ax, 4
        jne    SHORT loc_1_45A5
        mov    dword ptr [esp + 8], 0
        mov    word ptr [ecx + 32h], 0
        mov    word ptr [ecx + 30h], 0
        db  0C7h, 002h, 09Ch, 032h, 001h, 000h
        db  0A1h, 0A0h, 032h, 001h, 000h
        mov    dword ptr [edx + 4], eax
        db  0A1h, 0A0h, 032h, 001h, 000h
        mov    dword ptr [eax], edx
        lea    eax, [ecx + 1ch]
        push   eax
        db  089h, 015h, 0A0h, 032h, 001h, 000h
        lea    eax, [esp + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 40h]
        push   eax
        push   4
        push   0
        push   ecx
        push   0
        call   sub_1_45B0
        add    esp, 1ch
loc_1_45A5:
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  005h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
