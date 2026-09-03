; elpc3_lcode_01_02.asm -- ELPC3 object 1 segment _LTEXT  (part 2/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_4E60:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_4F30
        mov    dx, ax
        mov    ecx, dword ptr [ebp + 0ch]
        sub    dx, word ptr [ecx + 8]
        mov    word ptr [ecx + 8], ax
        mov    edi, edx
        mov    eax, dword ptr [esi + 198h]
        and    edi, 0ffffh
        cmp    edi, eax
        jbe    SHORT loc_1_4E92
        mov    dx, ax
loc_1_4E92:
        mov    di, word ptr [ecx + 0ah]
        inc    di
        mov    ax, word ptr [ecx + 0ch]
        add    ax, dx
        mov    word ptr [ecx + 0ah], di
        mov    word ptr [ecx + 0ch], ax
        cmp    di, 4
        jb     SHORT loc_1_4EBD
        shr    di, 1
        shr    ax, 1
        mov    word ptr [ecx + 0ah], di
        mov    word ptr [ecx + 0ch], ax
loc_1_4EBD:
        mov    ax, word ptr [ecx + 0ch]
        mov    edi, dword ptr [ecx]
        sub    dx, dx
        and    edi, 7ffh
        div    word ptr [ecx + 0ah]
        mov    esi, dword ptr [esi + 194h]
        and    eax, 0ffffh
        mov    ebx, esi
        sub    ebx, eax
        imul   ebx, edi
        mov    eax, ebx
        cdq
        idiv   esi
        mov    esi, eax
        test   esi, esi
        jl     SHORT loc_1_4F04
        cmp    edi, esi
        jge    SHORT loc_1_4EF3
        mov    esi, edi
loc_1_4EF3:
        add    edi, esi
        cmp    edi, 5eah
        jle    SHORT loc_1_4F1F
        mov    edi, 5eah
        jmp    SHORT loc_1_4F1F
loc_1_4F04:
        neg    esi
        mov    eax, edi
        cdq
        sub    eax, edx
        sar    eax, 1
        cmp    eax, esi
        jge    SHORT loc_1_4F14
        mov    esi, eax
loc_1_4F14:
        sub    edi, esi
        mov    eax, dword ptr [ecx + 4]
        cmp    edi, eax
        jge    SHORT loc_1_4F1F
        mov    edi, eax
loc_1_4F1F:
        add    edi, 9800h
        mov    dword ptr [ecx], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_1_4F30:
        push   ebp
        mov    eax, 806h
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 15ch]
        mov    edx, esi
        out    dx, ax
        mov    eax, 0b000h
        out    dx, ax
        mov    edx, dword ptr [ecx + 158h]
        add    edx, 6
        in     al, dx
        and    eax, 0ffh
        mov    edx, esi
        pop    esi
        add    dword ptr [ecx + 1b8h], eax
        mov    eax, 0a800h
        pop    ebp
        out    dx, ax
        mov    eax, 801h
        out    dx, ax
        mov    eax, dword ptr [ecx + 1b8h]
        ret    4
        db  08Dh, 049h, 000h
sub_1_4F80:
        push   ebp
        mov    eax, 5800h
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 15ch]
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [esi + 160h]
loc_1_4F9C:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_4F9C
        mov    edx, ecx
        mov    eax, 4800h
        out    dx, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4FC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        add    eax, dword ptr [esi + 0e4h]
        mov    dword ptr [esi + 0e4h], eax
        cmp    eax, 3e8h
        jbe    SHORT loc_1_4FF2
        mov    dword ptr [esi + 0e4h], 0
        push   esi
        call   sub_1_38B0
loc_1_4FF2:
        cmp    byte ptr [esi + 0b2h], 0
        jne    SHORT loc_1_5044
        mov    eax, dword ptr [esi + 1d0h]
        cmp    eax, 3e8h
        jbe    SHORT loc_1_5044
        shl    eax, 2
        cmp    dword ptr [esi + 1b8h], eax
        jae    SHORT loc_1_5044
        mov    ecx, dword ptr [esi + 15ch]
        mov    eax, 804h
        mov    byte ptr [esi + 0b2h], 1
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [esi + 158h]
        add    edx, 0ah
        in     ax, dx
        and    eax, 0fff7h
        out    dx, ax
        mov    edx, ecx
        mov    eax, 801h
        out    dx, ax
loc_1_5044:
        mov    cx, word ptr [esi + 188h]
        mov    ax, word ptr [esi + 18ah]
        mov    ebx, ecx
        sub    dx, dx
        and    ebx, 0ffffh
        div    cx
        mov    word ptr [ebp - 4], ax
        sub    edx, edx
        mov    cx, word ptr [ebp - 4]
        mov    eax, dword ptr [esi + 18ch]
        div    ebx
        imul   cx, cx
        sub    ax, cx
        mov    edx, dword ptr [esi + 4ah]
        mov    cx, ax
        shr    ax, 0ch
        mov    edi, eax
        and    edi, 0ffffh
        mov    bl, byte ptr [edi + edx + 144h]
        and    ebx, 0ffff00ffh
        test   bx, bx
        jne    SHORT loc_1_50A8
        mov    bx, cx
        shr    bx, 5
        add    bx, 2
loc_1_50A8:
        mov    ax, cx
        sub    dx, dx
        div    bx
        mov    edi, eax
        mov    eax, ebx
        sub    dx, dx
        and    edi, 0ffffh
        and    eax, 0ffffh
        add    edi, eax
        sar    edi, 1
        mov    ax, cx
        div    di
        mov    ecx, eax
        mov    eax, edi
        and    ecx, 0ffffh
        and    eax, 0ffffh
        add    ecx, eax
        mov    eax, dword ptr [ebp - 4]
        sar    ecx, 1
        and    eax, 0ffffh
        mov    edx, ecx
        and    edx, 0ffffh
        lea    ecx, [eax + edx*2]
        mov    eax, dword ptr [esi + 4]
        shl    ecx, 2
        add    eax, dword ptr [esi + 0e8h]
        mov    dword ptr [esi + 190h], ecx
        mov    dword ptr [esi + 0e8h], eax
        cmp    eax, 3e8h
        jbe    SHORT loc_1_5138
        mov    dword ptr [esi + 0e8h], 0
        cmp    dword ptr [esi + 0ech], 64h
        jb     SHORT loc_1_512E
        add    dword ptr [esi + 0d8h], 8
loc_1_512E:
        mov    dword ptr [esi + 0ech], 0
loc_1_5138:
        push   esi
        call   sub_1_4280
        cmp    byte ptr [esi + 0b0h], 0
        jne    SHORT loc_1_5156
        mov    ax, word ptr [esi + 0d4h]
        mov    edx, dword ptr [esi + 15ch]
        out    dx, ax
loc_1_5156:
        push   esi
        xor    edi, edi
        call   sub_1_4F30
        and    eax, 0ffffh
        mov    ebx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
loc_1_5169:
        test   edi, edi
        je     SHORT loc_1_5174
        cmp    edi, 1
        je     SHORT loc_1_517C
        jmp    SHORT loc_1_5182
loc_1_5174:
        lea    ebx, [esi + 19ch]
        jmp    SHORT loc_1_5182
loc_1_517C:
        lea    ebx, [esi + 1aah]
loc_1_5182:
        mov    eax, dword ptr [ebp - 4]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 8]
        sub    eax, ecx
        cmp    eax, dword ptr [esi + 198h]
        jl     SHORT loc_1_519C
        push   ebx
        push   esi
        call   sub_1_4E60
loc_1_519C:
        inc    edi
        cmp    edi, 2
        jl     SHORT loc_1_5169
        cmp    dword ptr [esi + 0f8h], 0
        jne    SHORT loc_1_51BA
        mov    ax, word ptr [esi + 19ch]
        mov    edx, dword ptr [esi + 15ch]
        out    dx, ax
loc_1_51BA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_51D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 56h], 1
        jne    SHORT loc_1_51F1
        mov    edx, dword ptr [eax + 15ch]
        mov    eax, 2000h
        out    dx, ax
        mov    eax, 4800h
        out    dx, ax
loc_1_51F1:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5200:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 56h], 0
        jne    SHORT loc_1_521A
        mov    edx, dword ptr [eax + 15ch]
        mov    eax, 1800h
        out    dx, ax
loc_1_521A:
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 055h, 0B8h, 000h, 058h, 000h, 000h, 08Bh, 0ECh, 056h, 08Bh, 04Dh, 008h, 08Bh, 0B1h
        db  05Ch, 001h, 000h, 000h, 08Bh, 089h, 060h, 001h, 000h, 000h, 08Bh, 0D6h, 066h, 0EFh, 08Bh, 0D1h
        db  066h, 0EDh, 0F6h, 0C4h, 010h, 075h, 0F9h, 08Bh, 0D6h, 0B8h, 000h, 028h, 000h, 000h, 066h, 0EFh
        db  08Bh, 0D1h, 066h, 0EDh, 0F6h, 0C4h, 010h, 075h, 0F9h, 08Bh, 0D6h, 0B8h, 000h, 078h, 000h, 000h
        db  066h, 0EFh, 0B8h, 0FFh, 068h, 000h, 000h, 05Eh, 066h, 0EFh, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h
        db  024h, 000h
sub_1_5270:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_38B0
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5290:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_52A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 0b9h], 1
        call   sub_1_4F80
        mov    ax, word ptr [esi + 19ch]
        mov    edx, dword ptr [esi + 15ch]
        mov    dword ptr [esi + 0f8h], 0
        out    dx, ax
        mov    eax, 1800h
        pop    esi
        out    dx, ax
        mov    eax, 7840h
        pop    ebp
        out    dx, ax
        mov    eax, 6000h
        out    dx, ax
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_52F0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_36E0
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5310:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   eax
        call   sub_1_3720
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        add    eax, 0ach
        push   edi
        mov    esi, dword ptr [ebp + 14h]
        push   esi
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_5596
        test   esi, esi
        je     SHORT loc_1_537D
        mov    ebx, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 0ch]
loc_1_5359:
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        add    edi, 4
        mov    eax, dword ptr [edx + 0ach]
        lea    ecx, [ebp - 4]
        push   eax
        push   ebx
        push   ecx
        add    ebx, 4
        call   sub_1_55A2
        dec    esi
        jne    SHORT loc_1_5359
loc_1_537D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 0B9h, 000h, 000h, 000h, 000h
        db  08Bh, 0ECh, 056h, 08Bh, 075h, 00Ch, 08Bh, 046h, 00Ch, 085h, 0C0h, 074h, 010h, 08Bh, 056h, 004h
        db  08Dh, 00Ch, 010h, 0C1h, 0EAh, 00Ch, 049h, 0C1h, 0E9h, 00Ch, 02Bh, 0CAh, 041h, 08Bh, 045h, 018h
        db  039h, 008h, 072h, 010h, 050h, 08Bh, 045h, 014h, 050h, 056h, 0E8h, 0F5h, 001h, 000h, 000h, 05Eh
        db  05Dh, 0C2h, 014h, 000h, 05Eh, 089h, 008h, 05Dh, 0C2h, 014h, 000h, 08Dh, 064h, 024h, 000h, 08Dh
        db  064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 00Ch, 08Bh
        db  0ECh, 050h, 0E8h, 0C1h, 001h, 000h, 000h, 08Bh, 045h, 008h, 08Bh, 088h, 0ACh, 000h, 000h, 000h
        db  051h, 0E8h, 0A0h, 001h, 000h, 000h, 05Dh, 0C2h, 008h, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh
        db  074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh
        db  04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh
        db  0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h
        db  0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch
        db  08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h
        db  08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
        db  057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh
        db  05Fh, 0C2h, 00Ch, 000h
sub_1_547A:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_5480:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_5486:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_548C:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_5492:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_5498:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
sub_1_549E:
        db  0CDh, 020h, 037h, 080h, 028h, 000h
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_54AA:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_54B6:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_54C2:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_54C8:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_54CE:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_54D4:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_54EC:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
        db  0CDh, 020h, 048h, 080h, 028h, 000h
        db  0CDh, 020h, 049h, 080h, 028h, 000h
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 057h, 080h, 028h, 000h
        db  0CDh, 020h, 058h, 080h, 028h, 000h
        db  0CDh, 020h, 059h, 080h, 028h, 000h
        db  0CDh, 020h, 05Ah, 080h, 028h, 000h
        db  0CDh, 020h, 05Bh, 080h, 028h, 000h
        db  0CDh, 020h, 05Ch, 080h, 028h, 000h
        db  0CDh, 020h, 05Dh, 080h, 028h, 000h
        db  0CDh, 020h, 05Eh, 080h, 028h, 000h
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        int3
        int3
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_5542:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_5548:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_554E:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_5554:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_5560:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_5566:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_556C:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_5590:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
sub_1_5596:
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
sub_1_55A2:
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_55B4:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_55D8:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
sub_1_55DE:
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_55E4:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_55EA:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_55F0:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
        db  0CDh, 020h, 042h, 080h, 028h, 000h
        db  0CDh, 020h, 012h, 080h, 028h, 000h
        db  0CDh, 020h, 013h, 080h, 028h, 000h
        db  0CDh, 020h, 014h, 080h, 028h, 000h
        db  0CDh, 020h, 015h, 080h, 028h, 000h
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_5638:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
sub_1_5644:
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        int3
        int3
        int3
        int3
        int3
        int3
sub_1_5650:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5680
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_5680:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_5720
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_56C0
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_56B5
        cmp    al, 0ffh
        jne    SHORT loc_1_56B5
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_56B5
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_5727
loc_1_56B5:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_5727
        db  08Bh, 0FFh
loc_1_56C0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_5710
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_56E6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_56ED
loc_1_56E6:
        mov    dword ptr [ebp - 10h], 1
loc_1_56ED:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_5700
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_572A
        db  08Dh, 064h, 024h, 000h
loc_1_5700:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_572A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5710:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_572A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5720:
        mov    dword ptr [ebp - 4], 20h
loc_1_5727:
        mov    esi, dword ptr [ebp + 8]
loc_1_572A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_5821
loc_1_5735:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    SHORT loc_1_578D
        test   al, 2
        je     SHORT loc_1_5779
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_575A:
        or     al, al
        je     SHORT loc_1_5779
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    SHORT loc_1_578D
loc_1_5779:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_5784
        test   bl, 4
        jne    SHORT loc_1_578D
loc_1_5784:
        test   bl, 20h
        je     NEAR PTR loc_1_5816
loc_1_578D:
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_54C2
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_5816
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_57CF
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_57CF:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_57E0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_57E6
        db  08Bh, 0FFh
loc_1_57E0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_57E6:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_57F5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_54C8
loc_1_57F5:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_556C
        mov    edi, ebx
        jmp    SHORT loc_1_5819
loc_1_5816:
        mov    edi, dword ptr [edi + 10h]
loc_1_5819:
        or     edi, edi
        jne    NEAR PTR loc_1_5735
loc_1_5821:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_5830:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5840
        pop    ebp
        ret    4
sub_1_5840:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_58BE
loc_1_5850:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_58B7
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_54C8
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_58B7
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_587C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_587C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_5890
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_5896
        db  005h, 000h, 000h, 000h, 000h
loc_1_5890:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_5896:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_556C
        mov    edi, ebx
        jmp    SHORT loc_1_58BA
loc_1_58B7:
        mov    edi, dword ptr [edi + 10h]
loc_1_58BA:
        or     edi, edi
        jne    SHORT loc_1_5850
loc_1_58BE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_58D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_58E5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_58E5:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_5921
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_5912
        cmp    bl, 0ffh
        jne    SHORT loc_1_5912
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_5912
        test   al, 8
        je     SHORT loc_1_590B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_590B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_5912:
        test   al, 4
        jne    SHORT loc_1_591A
        test   al, 2
        je     SHORT loc_1_5939
loc_1_591A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_5921:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_5939
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_5939
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_5939:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
sub_1_5940:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        rep movsd
        mov    cl, al
        rep movsb
        pop    edi
        pop    esi
        ret    0ch
sub_1_5960:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_5980
        or     ecx, ecx
        je     SHORT loc_1_5996
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_5994
loc_1_5980:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_5994
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_5994:
        rep movsb
loc_1_5996:
        popfd
        pop    edi
        pop    esi
        ret    0ch
_LTEXT ends
        end
