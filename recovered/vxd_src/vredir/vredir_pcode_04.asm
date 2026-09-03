; vredir_pcode_04.asm -- VREDIR object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  055h, 0B9h, 0FFh, 000h, 000h, 000h, 08Bh, 0ECh, 056h, 057h, 08Bh, 045h, 008h, 050h, 088h, 048h
        db  063h, 088h, 048h, 064h, 0E8h, 000h, 000h, 000h, 000h, 0C6h, 000h, 032h, 083h, 0C4h, 004h, 0C6h
        db  040h, 002h, 014h, 050h, 08Bh, 0F8h, 0C6h, 040h, 003h, 020h, 0E8h, 000h, 000h, 000h, 000h, 083h
        db  0C4h, 004h, 08Bh, 0F0h, 057h, 0E8h, 000h, 000h, 000h, 000h, 083h, 0C4h, 004h, 08Bh, 0C6h, 05Fh
        db  05Eh, 05Dh, 0C3h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0050:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        cmp    dword ptr [ecx + 0ch], 0
        push   esi
        push   edi
        jne    SHORT loc_4_008C
        mov    ax, word ptr [ecx + 6]
        mov    esi, dword ptr [ecx + 8]
        test   ax, ax
        mov    dword ptr [ecx + 0ch], esi
        je     SHORT loc_4_0085
        movzx  eax, ax
        xor    ebx, ebx
loc_4_0074:
        lea    edx, [esi + 50h]
        dec    eax
        mov    dword ptr [esi + 44h], ebx
        mov    byte ptr [esi + 31h], bl
        mov    dword ptr [esi + 40h], edx
        mov    esi, edx
        jne    SHORT loc_4_0074
loc_4_0085:
        mov    dword ptr [esi - 10h], 0
loc_4_008C:
        mov    esi, dword ptr [ecx + 4ch]
        mov    edx, dword ptr [ecx + 48h]
        mov    dword ptr [ecx + 4ch], 0
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        test   edi, edi
        jle    SHORT loc_4_00C5
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
loc_4_00A9:
        lea    eax, [edx + 14h]
        mov    dword ptr [edx + 4], esi
        add    esi, ebx
        mov    dword ptr [edx + 8], ebx
        dec    edi
        mov    dword ptr [edx + 0ch], ecx
        mov    dword ptr [edx + 10h], 0
        mov    dword ptr [edx], eax
        mov    edx, eax
        jne    SHORT loc_4_00A9
loc_4_00C5:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 4ch], edx
        mov    dword ptr [edx - 14h], 0
        test   eax, eax
        jle    SHORT loc_4_00F9
        mov    ebx, 200h
loc_4_00DD:
        lea    edi, [edx + 14h]
        mov    dword ptr [edx + 4], esi
        add    esi, ebx
        mov    dword ptr [edx + 8], ebx
        dec    eax
        mov    dword ptr [edx + 0ch], ecx
        mov    dword ptr [edx + 10h], 1
        mov    dword ptr [edx], edi
        mov    edx, edi
        jne    SHORT loc_4_00DD
loc_4_00F9:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 50h], edx
        mov    dword ptr [edx - 14h], 0
        test   edi, edi
        jle    SHORT loc_4_012E
        mov    ebx, 0a7h
loc_4_0112:
        lea    eax, [edx + 14h]
        mov    dword ptr [edx + 4], esi
        add    esi, ebx
        mov    dword ptr [edx + 8], ebx
        dec    edi
        mov    dword ptr [edx + 0ch], ecx
        mov    dword ptr [edx + 10h], 2
        mov    dword ptr [edx], eax
        mov    edx, eax
        jne    SHORT loc_4_0112
loc_4_012E:
        xor    eax, eax
        pop    edi
        mov    dword ptr [edx - 14h], 0
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_4_0140:
        push   ebx
        mov    ecx, 0ffffffffh
        push   esi
        sub    eax, eax
        push   edi
        db  0BFh, 000h, 000h, 000h, 000h
        repne scasb
        not    ecx
        db  0BEh, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        lea    ebx, [ecx - 1]
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_016B:
        add    esp, 8
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        cmp    ebx, 0fh
        jge    SHORT loc_4_01A1
        mov    edx, 0fh
        db  08Dh, 0BBh, 000h, 000h, 000h, 000h
        sub    edx, ebx
        mov    eax, 20202020h
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
loc_4_01A1:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        db  0C6h, 005h, 00Fh, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        sub    eax, eax
        repne scasb
        not    ecx
        lea    ebx, [ecx - 1]
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_01D0:
        add    esp, 8
        db  0B8h, 000h, 000h, 000h, 000h
        mov    ecx, 20202020h
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], ecx
        mov    word ptr [eax + 0ch], cx
        mov    byte ptr [eax + 0eh], cl
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        db  0C6h, 005h, 00Fh, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
sub_4_0210:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        xor    eax, eax
        push   ebx
        mov    ecx, 0ffh
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   edi
loc_4_0222:
        mov    byte ptr [ebx + eax + 63h], cl
        lea    esi, [ebx + eax]
        inc    eax
        mov    byte ptr [esi + 6dh], dl
        mov    byte ptr [esi + 68h], dl
        cmp    eax, 5
        jl     SHORT loc_4_0222
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_023A:
        push   0
        push   ebx
        call   dword ptr [ebx + 1ch]
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_4_026E
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 8
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        repe cmpsb
        je     SHORT loc_4_0269
        push   1
        push   ebx
        call   dword ptr [ebx + 1ch]
        add    esp, 8
        cmp    eax, 34h
        jne    SHORT loc_4_026E
loc_4_0269:
        mov    eax, 17e5h
loc_4_026E:
        cmp    word ptr [ebx + 58h], 0
        jne    SHORT loc_4_0285
        mov    word ptr [ebx + 58h], ax
        test   ax, ax
        jne    SHORT loc_4_0285
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_0285:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_4_0290:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        je     SHORT loc_4_02A0
        xor    eax, eax
        pop    edi
        pop    esi
        ret
loc_4_02A0:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        xor    eax, eax
        db  066h, 0A1h, 028h, 000h, 000h, 000h
        shl    eax, 2
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_02BB:
        add    esp, 4
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_4_02CF
        mov    eax, 8
        pop    edi
        pop    esi
        ret
loc_4_02CF:
        xor    ecx, ecx
        mov    edi, eax
        db  066h, 08Bh, 00Dh, 028h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        shl    ecx, 2
        xor    eax, eax
        shr    ecx, 2
        push   5ch
        rep stosd
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_02F0:
        add    esp, 4
        xor    edx, edx
        test   eax, eax
        je     SHORT loc_4_035A
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 58h], edx
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 4], ecx
        add    eax, 18h
        mov    esi, dword ptr [esi + 8]
        mov    dword ptr [eax - 10h], esi
        mov    word ptr [eax - 2], dx
        mov    dword ptr [eax - 6], edx
        mov    word ptr [eax - 8], 3
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0BAh, 000h, 000h, 000h, 000h
        mov    dword ptr [eax], ecx
        mov    esi, dword ptr [edx + 4]
        mov    dword ptr [eax + 4], esi
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        mov    ecx, dword ptr [edx + 8]
        mov    dword ptr [eax + 8], ecx
        mov    si, word ptr [edx + 0ch]
        mov    word ptr [eax + 0ch], si
        mov    dl, byte ptr [edx + 0eh]
        mov    byte ptr [eax + 0eh], dl
loc_4_035A:
        xor    eax, eax
        pop    edi
        pop    esi
        ret
        db  090h
sub_4_0360:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        push   195h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0377:
        db  0A3h, 000h, 000h, 000h, 000h
        add    esp, 4
        mov    edx, eax
        test   edx, edx
        je     NEAR PTR loc_4_040F
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        mov    byte ptr [edx + 0dh], 9
        mov    byte ptr [edx + 0eh], 2
        mov    dword ptr [edx + 0fh], 1
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [edx + 23h]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        xor    ecx, ecx
        mov    eax, 8
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        mov    ebx, 5ch
        dec    ecx
        db  089h, 015h, 000h, 000h, 000h, 000h
        add    edx, 50h
loc_4_03DA:
        mov    byte ptr [edx], bl
        add    edx, 2ch
        dec    eax
        mov    byte ptr [edx - 2dh], bl
        mov    byte ptr [edx - 3eh], bl
        mov    byte ptr [edx - 3fh], bl
        mov    dword ptr [edx - 43h], ecx
        jne    SHORT loc_4_03DA
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0402:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        add    esp, 0ch
loc_4_040F:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0420:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        push   eax
        mov    cl, byte ptr [eax + 14h]
        shl    edx, cl
        db  009h, 015h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_043D:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0450:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    edi, 1
        mov    cl, byte ptr [edx + 14h]
        shl    edi, cl
        not    edi
        db  0A1h, 000h, 000h, 000h, 000h
        and    eax, edi
        test   esi, esi
        db  0A3h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0487
loc_4_047B:
        and    dword ptr [esi + 8], edi
        and    dword ptr [esi + 4], edi
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_4_047B
loc_4_0487:
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_048D:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_04A0:
        db  0A1h, 000h, 000h, 000h, 000h
        push   ebp
        db  0A3h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  0A1h, 004h, 000h, 000h, 000h
        db  0BAh, 000h, 000h, 000h, 000h
        db  0A3h, 004h, 000h, 000h, 000h
        db  0B9h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    ax, word ptr [edx + 0ch]
        mov    dl, byte ptr [edx + 0eh]
        mov    word ptr [ecx + 0ch], ax
        mov    byte ptr [ecx + 0eh], dl
        db  0BAh, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 00Fh, 000h, 000h, 000h, 003h
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx + 0ch], ecx
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 58h], 0
        jne    SHORT loc_4_054A
        cmp    byte ptr [ecx + 2], 2
        jne    SHORT loc_4_0526
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_4_051E
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0516:
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
loc_4_051E:
        inc    eax
        pop    ebp
        db  0A3h, 000h, 000h, 000h, 000h
        ret
loc_4_0526:
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_052C:
        db  0C7h, 040h, 02Ch, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 4ch], 0
        add    esp, 4
        db  0C7h, 040h, 040h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_054A:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 080h, 078h, 002h, 002h, 075h, 006h, 0FFh
        db  00Dh, 000h, 000h, 000h, 000h, 05Dh, 0C3h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h
        db  000h, 000h
sub_4_0570:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        db  0B9h, 000h, 000h, 000h, 000h
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_4_0580:
        add    eax, eax
        add    eax, dword ptr [ecx]
        add    ecx, 4
        db  081h, 0F9h, 00Ch, 000h, 000h, 000h
        jb     SHORT loc_4_0580
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_4_059E
        inc    eax
        db  0A3h, 000h, 000h, 000h, 000h
loc_4_059E:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        db  08Bh, 03Dh, 00Ch, 000h, 000h, 000h
        db  08Bh, 035h, 004h, 000h, 000h, 000h
        not    ecx
        mov    ebx, ecx
        mov    ecx, 10h
        rep movsb
        db  0A1h, 00Ch, 000h, 000h, 000h
        db  08Bh, 03Dh, 008h, 000h, 000h, 000h
        db  08Bh, 035h, 004h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    byte ptr [eax + 0fh], 1eh
        rep movsb
        db  08Bh, 00Dh, 008h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 0fh], 1dh
        cmp    byte ptr [edx + 46h], 0
        je     NEAR PTR loc_4_06D5
        push   82h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0604:
        db  0A3h, 000h, 000h, 000h, 000h
        add    esp, 4
        mov    edx, eax
        test   edx, edx
        je     NEAR PTR loc_4_06D5
        lea    eax, [edx + 36h]
        mov    byte ptr [edx], 11h
        mov    byte ptr [edx + 1bh], 3
        sub    ax, dx
        db  0BFh, 000h, 000h, 000h, 000h
        add    ax, 20h
        mov    ecx, 0ffffffffh
        mov    word ptr [edx + 1dh], 1
        mov    word ptr [edx + 1fh], 1
        mov    word ptr [edx + 21h], 2
        mov    word ptr [edx + 19h], ax
        sub    eax, eax
        repne scasb
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [edx + 25h]
        not    ecx
        dec    ecx
        mov    eax, ecx
        mov    dword ptr [ebp - 4], ecx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + 10h]
        lea    ecx, [ebx + eax]
        add    cx, 0ah
        mov    word ptr [edx + 36h], 1
        mov    word ptr [edx + 3], cx
        lea    edi, [edx + 40h]
        mov    word ptr [edx + 17h], cx
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        add    cx, word ptr [ebp - 4]
        mov    word ptr [edx + 23h], cx
        lea    ecx, [edx + 36h]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        lea    edi, [ebx + edx + 40h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ecx + 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        sub    eax, edx
        lea    eax, [edx + eax + 40h]
        add    eax, ebx
        db  0A3h, 000h, 000h, 000h, 000h
loc_4_06D5:
        db  0BFh, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    ecx, 22h
        rep stosd
        db  0B8h, 036h, 000h, 000h, 000h
        db  0B9h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 011h
        db  0C6h, 005h, 01Bh, 000h, 000h, 000h, 003h
        sub    ax, cx
        db  0BFh, 000h, 000h, 000h, 000h
        add    ax, 20h
        mov    ecx, 0ffffffffh
        db  066h, 0C7h, 005h, 01Dh, 000h, 000h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 01Fh, 000h, 000h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 021h, 000h, 000h, 000h, 002h, 000h
        db  066h, 0A3h, 019h, 000h, 000h, 000h
        sub    eax, eax
        repne scasb
        db  0BEh, 000h, 000h, 000h, 000h
        db  0BFh, 025h, 000h, 000h, 000h
        not    ecx
        mov    edx, ecx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 036h, 000h, 000h, 000h
        db  0C6h, 005h, 036h, 000h, 000h, 000h, 001h
        mov    eax, dword ptr [ecx + 10h]
        db  0BFh, 03Ch, 000h, 000h, 000h
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        lea    ecx, [eax + 20h]
        db  066h, 089h, 00Dh, 003h, 000h, 000h, 000h
        db  066h, 089h, 00Dh, 017h, 000h, 000h, 000h
        add    cx, dx
        lea    edx, [ebx - 1]
        db  066h, 089h, 00Dh, 023h, 000h, 000h, 000h
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        db  0BFh, 056h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ecx + 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  02Dh, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        db  005h, 056h, 000h, 000h, 000h
        mov    ecx, 22h
        db  0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        rep stosd
        db  0B8h, 036h, 000h, 000h, 000h
        db  0B9h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 011h
        db  0C6h, 005h, 01Bh, 000h, 000h, 000h, 003h
        sub    ax, cx
        db  0BFh, 000h, 000h, 000h, 000h
        add    ax, 20h
        mov    ecx, 0ffffffffh
        db  066h, 0C7h, 005h, 01Dh, 000h, 000h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 01Fh, 000h, 000h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 021h, 000h, 000h, 000h, 002h, 000h
        db  066h, 0A3h, 019h, 000h, 000h, 000h
        sub    eax, eax
        repne scasb
        db  0BEh, 000h, 000h, 000h, 000h
        db  0BFh, 025h, 000h, 000h, 000h
        not    ecx
        mov    edx, ecx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 036h, 000h, 000h, 000h
        db  0C6h, 005h, 036h, 000h, 000h, 000h, 00Ch
        lea    eax, [ebx + 20h]
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        db  066h, 0A3h, 003h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        db  066h, 0A3h, 017h, 000h, 000h, 000h
        db  0C7h, 005h, 04Eh, 000h, 000h, 000h, 000h, 020h, 040h, 080h
        add    ax, dx
        db  066h, 0A3h, 023h, 000h, 000h, 000h
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        shr    ecx, 2
        mov    esi, edi
        db  0BFh, 03Ch, 000h, 000h, 000h
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        db  0BFh, 056h, 000h, 000h, 000h
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        db  081h, 0EBh, 000h, 000h, 000h, 000h
        and    ecx, 3
        push   1
        rep movsb
        db  08Dh, 083h, 056h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08DD:
        add    esp, 4
        db  0A3h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0908:
        add    esp, 8
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_091A:
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VREDIR_Service_05:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        pop    ebp
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        ret
        db  08Dh, 064h, 024h, 000h
VREDIR_Service_13:
        push   ebp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        xor    esi, esi
        test   ecx, ecx
        je     SHORT loc_4_09CB
        mov    eax, dword ptr [ebp + 8]
loc_4_0967:
        xor    edx, edx
        mov    dl, byte ptr [ecx + 5ah]
        cmp    edx, eax
        je     SHORT loc_4_097F
        mov    ecx, dword ptr [ecx + 18h]
        test   ecx, ecx
        jne    SHORT loc_4_0967
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_4_097F:
        test   byte ptr [ecx + 3], 2
        mov    esi, 1
        je     SHORT loc_4_0997
        mov    esi, 3
        pop    edi
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_4_0997:
        cmp    byte ptr [ecx + 2], 2
        jne    SHORT loc_4_09CB
        mov    edi, dword ptr [ecx + 10h]
        test   edi, edi
        je     SHORT loc_4_09CB
loc_4_09A4:
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [edi]
        push   eax
        push   ecx
        call   sub_4_1DF0
        add    esp, 8
        test   al, al
        jne    SHORT loc_4_09C6
        mov    edi, dword ptr [edi + 4]
        test   edi, edi
        jne    SHORT loc_4_09A4
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_4_09C6:
        mov    esi, 3
loc_4_09CB:
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VREDIR_Service_14:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_4_0A01
        mov    bl, byte ptr [ebp + 0ch]
loc_4_09F5:
        cmp    byte ptr [esi + 5ah], bl
        je     SHORT loc_4_0A0C
        mov    esi, dword ptr [esi + 18h]
        test   esi, esi
        jne    SHORT loc_4_09F5
loc_4_0A01:
        mov    eax, 0fffffffeh
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_0A0C:
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A16:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_4_0A28
        mov    eax, 0ffffffffh
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_0A28:
        cmp    byte ptr [esi + 65h], 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1dh], ecx
        je     SHORT loc_4_0A3E
        push   0
        push   0
        push   ebx
        call   ecx
        add    esp, 0ch
loc_4_0A3E:
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0A50:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        jne    SHORT loc_4_0AB4
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A67:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_4_0A79
        mov    word ptr [esi + 58h], ax
        pop    esi
        movzx  eax, ax
        pop    ebx
        pop    ebp
        ret
loc_4_0A79:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A7F:
        add    esp, 4
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        test   ecx, ecx
        jle    SHORT loc_4_0AAD
        xor    ebx, ebx
loc_4_0A9A:
        lea    eax, [edx + 64h]
        dec    ecx
        mov    dword ptr [edx + 3ch], ebx
        mov    byte ptr [edx + 9], bl
        mov    byte ptr [edx + 8], bl
        mov    dword ptr [edx], eax
        mov    edx, eax
        jne    SHORT loc_4_0A9A
loc_4_0AAD:
        mov    dword ptr [edx - 64h], 0
loc_4_0AB4:
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_0AC0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        test   edi, edi
        je     SHORT loc_4_0AFE
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
loc_4_0AD7:
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [esi], 0
        test   ecx, ecx
        je     SHORT loc_4_0AF3
loc_4_0AE4:
        inc    dword ptr [esi]
        cmp    dword ptr [ecx], edx
        jne    SHORT loc_4_0AEC
        mov    eax, edi
loc_4_0AEC:
        mov    ecx, dword ptr [ecx + 4]
        test   ecx, ecx
        jne    SHORT loc_4_0AE4
loc_4_0AF3:
        test   eax, eax
        jne    SHORT loc_4_0B00
        mov    edi, dword ptr [edi + 18h]
        test   edi, edi
        jne    SHORT loc_4_0AD7
loc_4_0AFE:
        xor    eax, eax
loc_4_0B00:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0B10:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_4_0B2A
        mov    ecx, 2
loc_4_0B1E:
        cmp    byte ptr [eax + 2], cl
        je     SHORT loc_4_0B2C
        mov    eax, dword ptr [eax + 18h]
        test   eax, eax
        jne    SHORT loc_4_0B1E
loc_4_0B2A:
        xor    eax, eax
loc_4_0B2C:
        ret
        db  08Dh, 049h, 000h
sub_4_0B30:
        push   ebp
        mov    ebp, esp
        push   8
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0B3A:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_4_0B48
        mov    eax, 8
        pop    ebp
        ret
loc_4_0B48:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [ecx + 10h]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ecx + 10h], eax
        xor    eax, eax
        ret
        db  08Dh, 049h, 000h
sub_4_0B60:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [edx + 10h]
        mov    edi, ecx
        mov    esi, ecx
        test   esi, esi
        je     SHORT loc_4_0BA1
        mov    eax, dword ptr [ebp + 0ch]
loc_4_0B77:
        cmp    dword ptr [esi], eax
        je     SHORT loc_4_0B88
        mov    edi, esi
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    SHORT loc_4_0B77
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_0B88:
        mov    eax, dword ptr [esi + 4]
        cmp    esi, ecx
        mov    dword ptr [edi + 4], eax
        jne    SHORT loc_4_0B98
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edx + 10h], eax
loc_4_0B98:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0B9E:
        add    esp, 4
loc_4_0BA1:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0BB0:
        push   ebx
        mov    ecx, 1
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    eax, edx
        test   edx, edx
        je     SHORT loc_4_0BCE
        xor    ebx, ebx
loc_4_0BC4:
        mov    byte ptr [eax + 5fh], bl
        mov    eax, dword ptr [eax + 18h]
        test   eax, eax
        jne    SHORT loc_4_0BC4
loc_4_0BCE:
        mov    eax, edx
        test   edx, edx
        je     SHORT loc_4_0BEC
        db  08Ah, 01Dh, 000h, 000h, 000h, 000h
loc_4_0BDA:
        cmp    byte ptr [eax + 5ah], bl
        je     SHORT loc_4_0BE8
        mov    eax, dword ptr [eax + 18h]
        test   eax, eax
        jne    SHORT loc_4_0BDA
        jmp    SHORT loc_4_0BEC
loc_4_0BE8:
        mov    byte ptr [eax + 5fh], 1
loc_4_0BEC:
        mov    eax, edx
        test   edx, edx
        je     SHORT loc_4_0C10
loc_4_0BF2:
        mov    bl, byte ptr [eax + 3]
        test   bl, 4
        je     SHORT loc_4_0C09
        cmp    byte ptr [eax + 5fh], 0
        jne    SHORT loc_4_0C09
        test   bl, 2
        jne    SHORT loc_4_0C09
        mov    byte ptr [eax + 5fh], 1
loc_4_0C09:
        mov    eax, dword ptr [eax + 18h]
        test   eax, eax
        jne    SHORT loc_4_0BF2
loc_4_0C10:
        mov    eax, edx
        test   edx, edx
        je     SHORT loc_4_0C49
loc_4_0C16:
        mov    bl, byte ptr [eax + 3]
        test   bl, 4
        je     SHORT loc_4_0C2D
        cmp    byte ptr [eax + 5fh], 0
        jne    SHORT loc_4_0C2D
        test   bl, 2
        je     SHORT loc_4_0C2D
        mov    byte ptr [eax + 5fh], cl
        inc    ecx
loc_4_0C2D:
        mov    eax, dword ptr [eax + 18h]
        test   eax, eax
        jne    SHORT loc_4_0C16
        test   edx, edx
        je     SHORT loc_4_0C49
loc_4_0C38:
        cmp    byte ptr [edx + 5fh], 0
        jne    SHORT loc_4_0C42
        mov    byte ptr [edx + 5fh], cl
        inc    ecx
loc_4_0C42:
        mov    edx, dword ptr [edx + 18h]
        test   edx, edx
        jne    SHORT loc_4_0C38
loc_4_0C49:
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_4_0C50:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        push   ebx
        cmp    byte ptr [eax + 44h], 0
        push   esi
        jne    SHORT loc_4_0C73
        mov    eax, 1
        pop    esi
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    ebx
        pop    ebp
        ret
loc_4_0C73:
        push   74h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0C7A:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_4_0C96
        push   8
        call   sub_4_17C0
        add    esp, 4
        mov    eax, 8
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_0C96:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, 2
        mov    dword ptr [esi + 14h], ebx
        mov    byte ptr [esi + 2], al
        db  089h, 034h, 09Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 5dh], al
        mov    byte ptr [esi + 5ah], 10h
        mov    byte ptr [esi + 3], 84h
        mov    dword ptr [esi + 48h], 0
        mov    dword ptr [esi + 4ch], 0
        db  0C7h, 046h, 01Ch, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 020h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 024h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 028h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 02Ch, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 030h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 034h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 038h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 03Ch, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 4
        db  0A3h, 000h, 000h, 000h, 000h
        shl    eax, 2
        db  0C7h, 046h, 044h, 000h, 000h, 000h, 000h
        lea    ecx, [eax + eax*4]
        db  0C7h, 046h, 040h, 000h, 000h, 000h, 000h
        lea    edx, [ecx + ecx*4]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D2D:
        add    esp, 4
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_4_0D55
        push   8
        call   sub_4_17C0
        add    esp, 4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D49:
        add    esp, 4
        mov    eax, 8
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_0D55:
        mov    cl, bl
        mov    al, 1
        shl    al, cl
        push   esi
        mov    byte ptr [esi + 5eh], al
        xor    eax, eax
        mov    al, byte ptr [esi + 5ah]
        db  088h, 088h, 000h, 000h, 000h, 000h
        call   sub_4_0A50
        add    esp, 4
        call   sub_4_0290
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_4_0D8C
        push   esi
        call   sub_4_17C0
        add    esp, 4
        mov    eax, esi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_0D8C:
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0DA0:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0E4A
        push   5205h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0DC2:
        add    esp, 4
        push   5405h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0DCF:
        add    esp, 4
        push   5305h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0DDC:
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        db  08Dh, 01Ch, 085h, 000h, 000h, 000h, 000h
        xor    eax, eax
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebx]
        mov    al, byte ptr [ecx + 5ah]
        db  0C6h, 080h, 000h, 000h, 000h, 000h, 0FFh
        mov    eax, dword ptr [ecx + 54h]
        test   eax, eax
        je     SHORT loc_4_0E0F
        mov    dword ptr [eax + 19h], 0
loc_4_0E0F:
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E15:
        add    esp, 4
        mov    dword ptr [ebx], 0ffffffffh
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_4_0E36
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E2F:
        add    esp, 4
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_0E36:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        jle    SHORT loc_4_0E4A
        xor    ecx, ecx
loc_4_0E41:
        mov    dword ptr [esi + 4], ecx
        add    esi, 64h
        dec    eax
        jne    SHORT loc_4_0E41
loc_4_0E4A:
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_4_0E50:
        push   ebp
        mov    cl, byte ptr [esp + 0ch]
        mov    ebp, esp
        db  0A1h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        mov    ebx, 1
        shl    ebx, cl
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E6C:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_4_0E89
        push   8
        call   sub_4_17C0
        add    esp, 4
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_0E89:
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    edi, esi
        mov    ecx, edx
        shr    ecx, 2
        db  089h, 034h, 085h, 000h, 000h, 000h, 000h
        xor    eax, eax
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp + 0ch]
        mov    dword ptr [esi + 14h], eax
        mov    byte ptr [esi + 5ah], cl
        mov    cl, byte ptr [ebp + 10h]
        mov    al, byte ptr [ebp + 14h]
        mov    byte ptr [esi + 3], cl
        test   al, al
        mov    byte ptr [esi + 2], 0
        jg     SHORT loc_4_0ED4
        jge    SHORT loc_4_0EDB
        db  0A1h, 000h, 000h, 000h, 000h
        test   dword ptr [eax + 50h], ebx
        je     SHORT loc_4_0EDB
loc_4_0ED4:
        mov    al, cl
        or     al, 2
        mov    byte ptr [esi + 3], al
loc_4_0EDB:
        test   cl, 80h
        mov    al, byte ptr [ebp + 18h]
        mov    byte ptr [esi + 4], al
        je     SHORT loc_4_0EF2
        mov    byte ptr [esi + 5dh], 2
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_0EFB
loc_4_0EF2:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 5dh], cl
loc_4_0EFB:
        lea    eax, [esi + 74h]
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 48h], eax
        lea    ebx, [ecx + edi]
        add    ebx, edx
        shl    ebx, 2
        lea    ebx, [ebx + ebx*4]
        add    ebx, eax
        db  0A1h, 000h, 000h, 000h, 000h
        add    ax, di
        mov    dword ptr [esi + 4ch], ebx
        add    ax, cx
        add    ax, dx
        add    ax, 2
        movzx  ecx, ax
        mov    word ptr [esi + 6], ax
        lea    eax, [ecx + ecx*4]
        shl    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F42:
        add    esp, 4
        mov    dword ptr [esi + 8], eax
        test   eax, eax
        jne    SHORT loc_4_0F60
        push   8
        call   sub_4_17C0
        add    esp, 4
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_0F60:
        db  0C7h, 046h, 01Ch, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 020h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 024h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 028h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 02Ch, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 030h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 034h, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 038h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [ebp + 8]
        mov    al, 1
        shl    al, cl
        db  0C7h, 046h, 03Ch, 000h, 000h, 000h, 000h
        push   esi
        mov    byte ptr [esi + 5eh], al
        db  0C7h, 046h, 040h, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    al, byte ptr [esi + 5ah]
        db  0C7h, 046h, 044h, 000h, 000h, 000h, 000h
        db  088h, 088h, 000h, 000h, 000h, 000h
        call   sub_4_0050
        add    esp, 4
        call   sub_4_0290
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_4_0FE6
        push   esi
        call   sub_4_17C0
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_0FE6:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_4_0FF0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        db  08Dh, 034h, 085h, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    ecx, dword ptr [esi]
        cmp    dword ptr [ebp + 0ch], 0
        mov    al, byte ptr [ecx + 5ah]
        db  0C6h, 080h, 000h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_4_101F
        mov    eax, dword ptr [ecx + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_101C:
        add    esp, 4
loc_4_101F:
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [ecx + 54h]
        test   eax, eax
        je     SHORT loc_4_102F
        mov    dword ptr [eax + 19h], 0
loc_4_102F:
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1035:
        add    esp, 4
        mov    dword ptr [esi], 0ffffffffh
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1050:
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1057:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_105C:
        push   0
        db  0BFh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1068:
        add    esp, 4
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        mov    ecx, edx
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 8
        rep stosd
        xor    edx, edx
        db  0A1h, 000h, 000h, 000h, 000h
        db  066h, 08Bh, 015h, 01Eh, 000h, 000h, 000h
        mov    ecx, eax
        shl    ecx, 5
        add    edx, 64h
        add    ecx, eax
        db  089h, 015h, 000h, 000h, 000h, 000h
        add    edx, 14h
        lea    eax, [eax + ecx*4]
        db  00Fh, 0AFh, 015h, 000h, 000h, 000h, 000h
        lea    edx, [edx + eax*4]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        shl    ecx, 5
        sub    ecx, eax
        lea    esi, [ecx + ecx*2]
        lea    ecx, [eax + esi*2]
        lea    eax, [edx + ecx + 74h]
        db  0A3h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_10F1:
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    eax, -1
        jne    SHORT loc_4_1103
        mov    eax, 6eh
        pop    edi
        pop    esi
        ret
loc_4_1103:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1108:
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    eax, -1
        jne    SHORT loc_4_111A
        mov    eax, 6eh
        pop    edi
        pop    esi
        ret
loc_4_111A:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_111F:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1124:
        db  066h, 081h, 03Dh, 022h, 000h, 000h, 000h, 080h, 000h
        jb     SHORT loc_4_1138
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 0FFh
        jmp    SHORT loc_4_1145
loc_4_1138:
        db  066h, 0A1h, 022h, 000h, 000h, 000h
        add    al, al
        db  0A2h, 000h, 000h, 000h, 000h
loc_4_1145:
        call   sub_4_0140
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        call   sub_4_1350
        xor    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
sub_4_1160:
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        db  066h, 039h, 00Dh, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        jb     SHORT loc_4_1185
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1185:
        xor    edi, edi
        db  0B8h, 000h, 000h, 000h, 000h
loc_4_118C:
        cmp    dword ptr [eax], -1
        je     SHORT loc_4_119C
        add    eax, 4
        inc    edi
        db  03Dh, 020h, 000h, 000h, 000h
        jb     SHORT loc_4_118C
loc_4_119C:
        movzx  eax, cx
        cmp    eax, edi
        jg     SHORT loc_4_11AF
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_11AF:
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    ebx, 0ffh
        jbe    SHORT loc_4_11EA
        call   sub_4_0B10
        test   eax, eax
        je     SHORT loc_4_11D7
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        call   sub_4_0B30
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_11D7:
        push   edi
        call   sub_4_0C50
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_4_133F
        jmp    SHORT loc_4_1240
loc_4_11EA:
        cmp    ebx, 11h
        ja     NEAR PTR loc_4_133A
        db  080h, 0BBh, 000h, 000h, 000h, 000h, 0FFh
        jne    NEAR PTR loc_4_133A
        cmp    dword ptr [ebp + 8], 0
        mov    al, 0
        je     SHORT loc_4_1218
        lea    eax, [ebp - 1]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_4_1DF0
        add    esp, 8
loc_4_1218:
        test   al, al
        jne    SHORT loc_4_1225
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 1], cl
loc_4_1225:
        mov    ecx, dword ptr [ebp - 1]
        push   ecx
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   ebx
        push   edi
        call   sub_4_0E50
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_4_133F
loc_4_1240:
        mov    ecx, dword ptr [ebp + 8]
        db  08Bh, 004h, 0BDh, 000h, 000h, 000h, 000h
        push   ecx
        push   eax
        call   sub_4_0B30
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_4_133F
        db  08Dh, 034h, 0BDh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_0210
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_4_12B3
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        push   ecx
        push   eax
        call   sub_4_0B60
        add    esp, 8
        cmp    ebx, 0ffh
        push   1
        push   edi
        jbe    SHORT loc_4_12A1
        call   sub_4_0DA0
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_12A1:
        call   sub_4_0FF0
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_12B3:
        db  066h, 0FFh, 005h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 18h], ecx
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_4_0BB0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_12EE
        xor    eax, eax
        db  066h, 0A1h, 012h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_12E6:
        add    esp, 4
        call   sub_4_0570
loc_4_12EE:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_04A0
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 002h
        test   eax, eax
        je     SHORT loc_4_131C
        call   sub_4_14A0
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_4_131C
        call   sub_4_0360
loc_4_131C:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_0420
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_133A:
        mov    eax, 1
loc_4_133F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1350:
        push   ebx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        cmp    byte ptr [ecx + 5ch], 0
        je     SHORT loc_4_13DD
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_13DD
        cmp    byte ptr [ecx + 5dh], 0
        je     SHORT loc_4_13A7
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        xor    ebx, ebx
        not    edi
        db  023h, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, 1
        cmp    edi, esi
        jb     SHORT loc_4_13A7
loc_4_1387:
        cmp    ebx, 20h
        jae    SHORT loc_4_13A7
        test   esi, edi
        je     SHORT loc_4_13A0
        push   81h
        push   ebx
        push   0
        call   sub_4_1160
        add    esp, 0ch
loc_4_13A0:
        add    esi, esi
        inc    ebx
        cmp    edi, esi
        jae    SHORT loc_4_1387
loc_4_13A7:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        xor    ebx, ebx
        not    edi
        db  023h, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, 1
        cmp    edi, esi
        jb     SHORT loc_4_13DD
loc_4_13C0:
        cmp    ebx, 20h
        jae    SHORT loc_4_13DD
        test   esi, edi
        je     SHORT loc_4_13D6
        push   0
        push   ebx
        push   0
        call   sub_4_1160
        add    esp, 0ch
loc_4_13D6:
        add    esi, esi
        inc    ebx
        cmp    edi, esi
        jae    SHORT loc_4_13C0
loc_4_13DD:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_13F0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_4_0AC0
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_4_1415
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1415:
        cmp    dword ptr [ebp - 4], 1
        jle    SHORT loc_4_1422
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1422:
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_4_142C
        or     byte ptr [esi + 61h], 2
loc_4_142C:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_143C
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_4_143C:
        push   esi
        call   sub_4_0450
        add    esp, 4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_144B:
        add    esp, 4
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VREDIR_Service_16:
        push   ebp
        mov    ebp, esp
        call   sub_4_0B10
        test   eax, eax
        je     SHORT loc_4_1477
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    word ptr [eax + 58h], cx
        xor    eax, eax
        ret
loc_4_1477:
        mov    eax, 1
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_4_1480:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_1497
        db  0A1h, 004h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + 13h], cl
loc_4_1497:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_14A0:
        push   ebp
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 1b8h
        test   edx, edx
        push   ebx
        push   esi
        push   edi
        jne    SHORT loc_4_14BF
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_14BF:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        cmp    word ptr [edx], 4954h
        je     SHORT loc_4_14DC
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_14DC:
        cmp    byte ptr [edx + 2], 2
        je     SHORT loc_4_14EE
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_14EE:
        lea    edi, [ebp - 0b4h]
        xor    eax, eax
        mov    ecx, 1dh
        add    edx, 66h
        rep stosd
        lea    eax, [edx - 31h]
        lea    ecx, [ebp - 0b4h]
        push   ecx
        mov    dword ptr [ebp - 88h], eax
        mov    byte ptr [ebp - 0afh], 1
        mov    dword ptr [ebp - 8ch], edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1522:
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_4_16B8
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], 0
        cmp    byte ptr [ecx + 0a7h], 0
        lea    esi, [ecx + 0a8h]
        mov    dword ptr [ebp - 8], esi
        jle    NEAR PTR loc_4_16B8
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_4_1556:
        xor    ebx, ebx
        mov    edx, dword ptr [ebp - 8]
        xor    eax, eax
        add    edx, 2
        mov    ax, word ptr [edx - 2]
        mov    word ptr [ebp - 26h], 1
        mov    word ptr [ebp - 24h], bx
        add    dword ptr [ebp - 8], eax
        mov    word ptr [ebp - 2ch], bx
        movsx  eax, byte ptr [edx + 1]
        test   eax, eax
        je     SHORT loc_4_1589
        cmp    eax, 1
        je     SHORT loc_4_15A1
        cmp    eax, 2
        je     SHORT loc_4_15B8
        jmp    SHORT loc_4_15BC
loc_4_1589:
        mov    al, byte ptr [edx]
        xor    ebx, ebx
        mov    byte ptr [ebp - 3fh], 3ah
        mov    byte ptr [ebp - 3eh], 0
        mov    byte ptr [ebp - 40h], al
        movsx  esi, byte ptr [edx]
        mov    bl, byte ptr [esi + ecx - 41h]
        jmp    SHORT loc_4_15BC
loc_4_15A1:
        mov    byte ptr [ebp - 40h], 4ch
        mov    byte ptr [ebp - 3fh], 50h
        mov    byte ptr [ebp - 3eh], 54h
        mov    al, byte ptr [edx]
        mov    byte ptr [ebp - 3ch], 0
        mov    byte ptr [ebp - 3dh], al
        jmp    SHORT loc_4_15BC
loc_4_15B8:
        mov    byte ptr [ebp - 40h], 0
loc_4_15BC:
        db  066h, 08Bh, 035h, 000h, 000h, 000h, 000h
        add    edx, 2
        db  0B8h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 1b8h]
        mov    word ptr [ecx], si
        mov    edi, edx
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ecx + 2], al
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    dword ptr [ebp - 4], ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        lea    edi, [ebp - 1b8h]
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, dword ptr [ebp - 4]
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 3
        rep movsb
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    edx, ecx
        lea    edi, [ebp - 1b8h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        mov    word ptr [edi - 1], ax
        mov    edi, edx
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    dword ptr [ebp - 4], ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        lea    edi, [ebp - 1b8h]
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, dword ptr [ebp - 4]
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [ebp - 4]
        push   0ffh
        and    ecx, 3
        push   ebx
        rep movsb
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        push   -1
        repne scasb
        not    ecx
        add    edx, ecx
        lea    eax, [ebp - 40h]
        lea    ecx, [ebp - 1b8h]
        push   eax
        mov    dword ptr [ebp - 36h], ecx
        mov    dword ptr [ebp - 32h], edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1696:
        add    esp, 10h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        movsx  edx, byte ptr [ecx + 0a7h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        inc    dword ptr [ebp - 0ch]
        cmp    edx, dword ptr [ebp - 0ch]
        jg     NEAR PTR loc_4_1556
loc_4_16B8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
sub_4_16C0:
        push   esi
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   edi
        mov    ecx, 0ffffffffh
        mov    eax, dword ptr [edx]
        mov    esi, dword ptr [edx + 4]
        mov    edi, esi
        db  0A3h, 000h, 000h, 000h, 000h
        sub    eax, eax
        repne scasb
        not    ecx
        db  0BFh, 000h, 000h, 000h, 000h
        lea    eax, [ecx - 1]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    esi, dword ptr [edx + 8]
        mov    ecx, 0ffffffffh
        mov    edi, esi
        sub    eax, eax
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        repne scasb
        not    ecx
        db  0BFh, 000h, 000h, 000h, 000h
        lea    eax, [ecx - 1]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_4_2130
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + 14h]
        mov    edx, dword ptr [ecx + 1ch]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + 18h]
        db  066h, 089h, 015h, 01Eh, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + 20h]
        db  066h, 0A3h, 012h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx + 24h]
        db  066h, 089h, 015h, 022h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + 28h]
        db  066h, 0A3h, 004h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx + 2ch]
        db  089h, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + 30h]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ecx + 5eh]
        db  0A2h, 000h, 000h, 000h, 000h
        cmp    byte ptr [ecx + 45h], 0
        je     SHORT loc_4_17A7
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1794:
        test   eax, eax
        je     SHORT loc_4_17A7
        xor    eax, eax
        pop    edi
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 007h, 000h, 000h, 000h
        pop    esi
        ret
loc_4_17A7:
        xor    eax, eax
        pop    edi
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        pop    esi
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_17C0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        db  0A3h, 000h, 000h, 000h, 000h
        pop    ebp
        ret
        db  08Bh, 0FFh, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 056h, 057h, 050h, 0E8h, 0E1h, 0FFh, 0FFh
        db  0FFh, 083h, 0C4h, 004h, 08Bh, 075h, 00Ch, 08Bh, 0FEh, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h
        db  0F2h, 0AEh, 0F7h, 0D1h, 066h, 001h, 00Dh, 008h, 000h, 000h, 000h, 08Bh, 0C1h, 0C1h, 0E9h, 002h
        db  0BFh, 00Ch, 000h, 000h, 000h, 0F3h, 0A5h, 08Bh, 0C8h, 083h, 0E1h, 003h, 0F3h, 0A4h, 0C6h, 005h
        db  00Bh, 000h, 000h, 000h, 001h, 05Fh, 05Eh, 05Dh, 0C3h, 08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h
        db  000h, 000h
sub_4_1820:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_4_17C0
        add    esp, 4
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, ecx
        mov    edx, dword ptr [ebp + 14h]
        shl    ax, 2
        db  088h, 00Dh, 00Ah, 000h, 000h, 000h
        db  066h, 001h, 005h, 008h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 10h]
        pop    ebp
        db  089h, 00Dh, 00Ch, 000h, 000h, 000h
        db  089h, 015h, 010h, 000h, 000h, 000h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_1860:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_4_17C0
        add    esp, 4
        xor    ecx, ecx
        db  0C6h, 005h, 00Ah, 000h, 000h, 000h, 004h
        db  066h, 083h, 005h, 008h, 000h, 000h, 000h, 010h
        mov    eax, dword ptr [ebp + 10h]
        mov    cl, ah
        and    eax, 0ffh
        db  089h, 00Dh, 00Ch, 000h, 000h, 000h
        db  0A3h, 010h, 000h, 000h, 000h
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    cl, ah
        pop    ebp
        and    eax, 0ffh
        db  089h, 00Dh, 014h, 000h, 000h, 000h
        db  0A3h, 018h, 000h, 000h, 000h
        ret
        db  090h
sub_4_18B0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 10h]
        test   ecx, ecx
        je     SHORT loc_4_18CC
        mov    eax, dword ptr [ebp + 0ch]
loc_4_18C1:
        cmp    dword ptr [ecx], eax
        je     SHORT loc_4_18D0
        mov    ecx, dword ptr [ecx + 4]
        test   ecx, ecx
        jne    SHORT loc_4_18C1
loc_4_18CC:
        xor    eax, eax
        pop    ebp
        ret
loc_4_18D0:
        mov    eax, 1
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_18E0:
        mov    eax, 1
        db  0CDh, 020h, 061h, 001h, 001h, 000h
        ret
        db  08Dh, 064h, 024h, 000h
sub_4_18F0:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1900:
        push   ebp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        test   ecx, ecx
        push   ebx
        push   esi
        push   edi
        mov    bl, 1
        mov    edi, ecx
        mov    esi, dword ptr [ebp + 8]
        je     SHORT loc_4_195B
loc_4_1917:
        test   bl, bl
        jne    SHORT loc_4_195B
        cmp    edi, ecx
        je     SHORT loc_4_195B
        xor    bl, bl
        xor    eax, eax
        movsx  edx, byte ptr [ebp + 0ch]
        mov    al, byte ptr [edi + 1ch]
        test   edx, eax
        jne    SHORT loc_4_1945
        push   esi
        mov    eax, dword ptr [edi + 76h]
        push   eax
        call   sub_4_18B0
        add    esp, 8
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_4_194E
loc_4_1945:
        mov    edi, dword ptr [edi + 4]
        test   edi, edi
        jne    SHORT loc_4_1917
        jmp    SHORT loc_4_195B
loc_4_194E:
        mov    al, byte ptr [ebp + 0ch]
        or     byte ptr [edi + 1ch], al
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_195B:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        call   sub_4_18F0
        add    esp, 8
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  090h
sub_4_1970:
        push   ebp
        mov    dl, 1
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 0ch]
        mov    eax, esi
        test   eax, eax
        je     SHORT loc_4_19AE
loc_4_1984:
        test   dl, dl
        jne    SHORT loc_4_19AE
        cmp    eax, esi
        je     SHORT loc_4_19AE
        xor    dl, dl
        mov    cl, byte ptr [eax + 17h]
        movsx  edi, byte ptr [ebp + 0ch]
        xor    ebx, ebx
        mov    bl, cl
        test   ebx, edi
        jne    SHORT loc_4_19A7
        mov    bl, byte ptr [ebp + 0ch]
        not    bl
        and    bl, cl
        mov    byte ptr [eax + 17h], bl
loc_4_19A7:
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_4_1984
loc_4_19AE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_19C0:
        push   ebp
        mov    dl, 1
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp + 0ch]
        mov    esi, dword ptr [eax + 0ch]
        mov    eax, esi
        test   eax, eax
        je     SHORT loc_4_19FE
loc_4_19D7:
        test   dl, dl
        jne    SHORT loc_4_19FE
        cmp    eax, esi
        je     SHORT loc_4_19FE
        xor    dl, dl
        xor    ebx, ebx
        movsx  edi, cl
        mov    bl, byte ptr [eax + 17h]
        test   edi, ebx
        je     SHORT loc_4_19F6
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_4_19D7
        jmp    SHORT loc_4_19FE
loc_4_19F6:
        pop    edi
        or     byte ptr [eax + 17h], cl
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_19FE:
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_1970
        add    esp, 8
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1A20:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
loc_4_1A29:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_4_1900
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_4_1A91
loc_4_1A3F:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        call   sub_4_19C0
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_1A29
        push   20000001h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1A5C:
        add    esp, 0ch
        mov    ebx, eax
        cmp    esi, 8001h
        jne    SHORT loc_4_1A3F
        test   ebx, ebx
        je     SHORT loc_4_1A3F
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_4_18F0
        add    esp, 8
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        push   edi
        call   sub_4_1970
        add    esp, 8
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_1A91:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_1AA0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
loc_4_1AAC:
        mov    esi, dword ptr [ecx]
        cmp    esi, eax
        je     SHORT loc_4_1AC3
        test   esi, esi
        je     SHORT loc_4_1AC1
        inc    edx
        add    ecx, 4
        cmp    edx, 8
        jl     SHORT loc_4_1AAC
        jmp    SHORT loc_4_1AC3
loc_4_1AC1:
        mov    dword ptr [ecx], eax
loc_4_1AC3:
        sub    edx, 8
        pop    esi
        pop    ebp
        cmp    edx, 1
        sbb    eax, eax
        neg    eax
        ret
sub_4_1AD0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
loc_4_1ADB:
        cmp    dword ptr [ecx], eax
        je     SHORT loc_4_1AEA
        inc    edx
        add    ecx, 4
        cmp    edx, 8
        jl     SHORT loc_4_1ADB
        jmp    SHORT loc_4_1AFF
loc_4_1AEA:
        cmp    dword ptr [ebp + 0ch], 1
        jne    SHORT loc_4_1AF6
        mov    dword ptr [ecx], 0
loc_4_1AF6:
        mov    eax, dword ptr [ebp + 14h]
        test   eax, eax
        je     SHORT loc_4_1AFF
        mov    dword ptr [eax], edx
loc_4_1AFF:
        mov    eax, 1
        cmp    edx, 8
        jl     SHORT loc_4_1B0B
        xor    eax, eax
loc_4_1B0B:
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_4_1B10:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    eax
        push   esi
        push   edi
        xor    esi, esi
        cmp    eax, 0dh
        ja     NEAR PTR loc_4_1D1E
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_4_1B34:
        push   0
        mov    edi, dword ptr [ebp + 10h]
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   edi
        call   sub_4_1AD0
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_4_1D1E
        push   edi
        call   sub_4_1EF0
        add    esp, 4
        mov    esi, eax
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_1B62:
        push   0
        mov    edi, dword ptr [ebp + 10h]
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   edi
        call   sub_4_1AD0
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_4_1B8C
        push   0
        push   0
        push   edi
        push   0
        push   0ch
        call   sub_4_1B10
        add    esp, 14h
loc_4_1B8C:
        push   0
        push   0
        push   edi
        push   0
        push   0eh
        call   sub_4_1B10
        add    esp, 14h
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_4_1AA0
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_4_1D1E
        mov    eax, 13h
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_1BBC:
        cmp    dword ptr [ebp + 0ch], 1
        jne    NEAR PTR loc_4_1D1E
        push   1
        mov    edi, dword ptr [ebp + 10h]
        push   8001h
        push   edi
        call   sub_4_1A20
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_1BDD:
        db  068h, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [ebp + 10h]
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   edi
        call   sub_4_1AD0
        add    esp, 10h
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_1BFB:
        cmp    dword ptr [ebp + 0ch], 1
        jne    NEAR PTR loc_4_1D1E
        push   2
        mov    edi, dword ptr [ebp + 10h]
        push   8002h
        push   edi
        call   sub_4_1A20
        add    esp, 0ch
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_1C1E:
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_4_1D1E
        db  068h, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        call   sub_4_1AA0
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_1C48
        mov    esi, 13h
        pop    edi
        mov    eax, esi
        pop    esi
        pop    ebp
        ret
loc_4_1C48:
        push   0
        push   0
        push   edi
        push   0
        push   0ch
        call   sub_4_1B10
        add    esp, 14h
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_1C5F:
        push   0
        mov    edi, dword ptr [ebp + 10h]
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   edi
        call   sub_4_1AD0
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_4_1D1E
        push   4
        push   8003h
        push   edi
        call   sub_4_1A20
        add    esp, 0ch
        push   0
        push   edi
        call   sub_4_13F0
        add    esp, 8
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_1C9D:
        push   0
        mov    edi, dword ptr [ebp + 10h]
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   edi
        call   sub_4_1AD0
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_4_1D1E
        push   8
        push   8004h
        push   edi
        call   sub_4_1A20
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 1
        je     SHORT loc_4_1CD5
        cmp    eax, 2
        je     SHORT loc_4_1CE0
        jmp    SHORT loc_4_1CEF
loc_4_1CD5:
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        or     cl, 1
        jmp    SHORT loc_4_1CE9
loc_4_1CE0:
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        or     cl, 2
loc_4_1CE9:
        db  088h, 00Dh, 000h, 000h, 000h, 000h
loc_4_1CEF:
        cmp    eax, 1
        je     SHORT loc_4_1CFE
        cmp    eax, 2
        mov    eax, 1
        jne    SHORT loc_4_1D00
loc_4_1CFE:
        xor    eax, eax
loc_4_1D00:
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1D07:
        add    esp, 8
        push   20000002h
        push   0
        push   8004h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1D1B:
        add    esp, 0ch
loc_4_1D1E:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 001h, 002h, 003h, 008h, 008h, 008h, 008h, 004h, 005h, 008h, 006h
        db  008h, 007h, 08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1D60:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 48h
        test   eax, eax
        push   esi
        mov    dword ptr [ebp - 8], 40h
        push   edi
        jne    SHORT loc_4_1D7F
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1D7F:
        push   0
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_4_21C8
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_4_1DDA
        push   1
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 48h]
        push   ecx
        mov    edx, dword ptr [ebp - 4]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        call   sub_4_21C0
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_4_1DDA
        mov    eax, dword ptr [ebp + 10h]
        lea    esi, [ebp - 48h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1DDA:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1DF0:
        push   ebp
        db  0A0h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4ch
        mov    dword ptr [ebp - 0ch], 0
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   esi
        mov    ebx, dword ptr [ebp + 0ch]
        push   edi
        push   0
        push   ecx
        mov    byte ptr [ebx], al
        lea    eax, [ebp - 4]
        push   eax
        call   sub_4_21C8
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_4_1EE1
loc_4_1E24:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 4ch]
        push   ecx
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], 40h
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   edx
        call   sub_4_21C0
        add    esp, 1ch
        test   eax, eax
        jne    NEAR PTR loc_4_1EE1
        lea    esi, [ebp - 4ch]
        db  0BFh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], 4
        mov    ecx, 4
        repe cmpsb
        jne    SHORT loc_4_1E8D
        push   1
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 0ch]
        push   ecx
        mov    edx, dword ptr [ebp - 4]
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   edx
        call   sub_4_21C0
        add    esp, 1ch
        test   eax, eax
        je     SHORT loc_4_1EAD
loc_4_1E8D:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_4_21C8
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_4_1E24
        mov    al, byte ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1EAD:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_4_1EE1
        push   1
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], 1
        push   ebx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   ecx
        call   sub_4_21C0
        add    esp, 1ch
        test   eax, eax
        je     SHORT loc_4_1EE1
        db  0A0h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebx], al
loc_4_1EE1:
        mov    al, byte ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_4_1EF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   esi
        push   eax
        call   sub_4_21C8
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_4_1F19
        mov    eax, 13h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1F19:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F22:
        add    esp, 4
        mov    edi, eax
        cmp    edi, -1
        jne    SHORT loc_4_1F47
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F35:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_4_1F47
        mov    eax, 13h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1F47:
        push   85h
        push   edi
        push   esi
        call   sub_4_1160
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_4_1F65
        mov    eax, 13h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1F65:
        cmp    edi, -1
        jne    SHORT loc_4_1F94
        mov    edi, 20000002h
        lea    eax, [ebp - 1]
        push   eax
        push   esi
        call   sub_4_1DF0
        add    esp, 8
        test   al, al
        je     SHORT loc_4_1F85
        mov    edi, 20000006h
loc_4_1F85:
        push   edi
        push   esi
        push   8000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F91:
        add    esp, 0ch
loc_4_1F94:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_4_1FA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     SHORT loc_4_1FC8
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        call   sub_4_0AC0
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_4_1FC8
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_4_1FC8:
        mov    edi, 20000002h
        test   esi, esi
        je     SHORT loc_4_1FE7
        lea    eax, [ebp - 1]
        push   eax
        push   esi
        call   sub_4_1DF0
        add    esp, 8
        test   al, al
        je     SHORT loc_4_1FE7
        mov    edi, 20000006h
loc_4_1FE7:
        push   edi
        push   esi
        push   8000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1FF3:
        add    esp, 0ch
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_4_2000:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   0
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_4_21D0
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_2020:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   3
        push   0
        mov    esi, dword ptr [ebp + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_4_21D8
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_4_20E1
        xor    edi, edi
        db  0B9h, 000h, 000h, 000h, 000h
loc_4_2049:
        cmp    dword ptr [ecx], esi
        je     SHORT loc_4_2059
        add    ecx, 4
        inc    edi
        db  081h, 0F9h, 020h, 000h, 000h, 000h
        jb     SHORT loc_4_2049
loc_4_2059:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_2085
        push   esi
        call   sub_4_1EF0
        add    esp, 4
        cmp    edi, 8
        jge    SHORT loc_4_20E1
        db  0C7h, 004h, 0BDh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_2085:
        cmp    edi, 8
        jge    SHORT loc_4_2090
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_2090:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  08Dh, 00Ch, 08Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx], 0
        je     SHORT loc_4_20AB
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_20AB:
        mov    edx, 8
        mov    dword ptr [ecx], esi
        xor    ecx, ecx
        db  089h, 00Dh, 000h, 000h, 000h, 000h
loc_4_20BA:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  039h, 00Ch, 0B5h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_20E5
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        db  039h, 015h, 000h, 000h, 000h, 000h
        jl     SHORT loc_4_20BA
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_20E1:
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_20E5:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_4_20F0:
        push   esi
        db  0BEh, 000h, 000h, 000h, 000h
loc_4_20F6:
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_4_210B
        push   eax
        call   sub_4_1EF0
        add    esp, 4
        mov    dword ptr [esi], 0
loc_4_210B:
        add    esi, 4
        db  081h, 0FEh, 020h, 000h, 000h, 000h
        jb     SHORT loc_4_20F6
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2130:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    dword ptr [ebp - 8], 14h
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   80000002h
        call   sub_4_21F0
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_4_21B5
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        call   sub_4_21E8
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_4_2185
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_2185:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], 4
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        call   sub_4_21E8
        add    esp, 18h
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        call   sub_4_21E0
loc_4_21B5:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_21C0:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        int3
        int3
sub_4_21C8:
        db  0CDh, 020h, 003h, 080h, 033h, 000h
        int3
        int3
sub_4_21D0:
        db  0CDh, 020h, 04Eh, 080h, 033h, 000h
        int3
        int3
sub_4_21D8:
        db  0CDh, 020h, 00Eh, 080h, 033h, 000h
        int3
        int3
sub_4_21E0:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        int3
        int3
sub_4_21E8:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        int3
        int3
sub_4_21F0:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
_PTEXT ends
        end
