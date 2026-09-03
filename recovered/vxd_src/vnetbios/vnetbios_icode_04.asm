; vnetbios_icode_04.asm -- VNETBIOS object 4 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  068h, 080h, 000h, 000h, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah
        db  001h, 06Ah, 001h, 0CDh, 020h, 053h, 000h, 001h, 000h, 083h, 0C4h, 020h, 085h, 0C0h, 00Fh, 084h
        db  0B1h, 000h, 000h, 000h, 089h, 015h, 030h, 004h, 000h, 000h, 08Dh, 082h, 0C0h, 00Fh, 000h, 000h
        db  083h, 0C2h, 028h, 089h, 015h, 010h, 019h, 000h, 000h, 08Dh, 04Ah, 068h, 0C7h, 042h, 0F8h, 000h
        db  002h, 000h, 000h, 03Bh, 0C8h, 073h, 007h, 089h, 04Ah, 0FCh, 08Bh, 0D1h, 0EBh, 0EBh, 0C7h, 042h
        db  0FCh, 000h, 000h, 000h, 000h, 083h, 03Dh, 0D1h, 01Ah, 000h, 000h, 000h, 074h, 056h, 0A1h, 034h
        db  004h, 000h, 000h, 06Bh, 0C0h, 068h, 083h, 0C0h, 010h, 083h, 0E0h, 0F0h, 08Bh, 0F8h, 06Ah, 004h
        db  050h, 0CDh, 020h, 0A8h, 000h, 001h, 000h, 059h, 059h, 085h, 0C0h, 074h, 044h, 097h, 0C1h, 0EFh
        db  004h, 066h, 089h, 03Dh, 014h, 019h, 000h, 000h, 0C1h, 0E7h, 004h, 08Dh, 044h, 038h, 0C0h, 083h
        db  0C7h, 028h, 089h, 03Dh, 00Ch, 019h, 000h, 000h, 08Dh, 04Fh, 068h, 0C7h, 047h, 0F8h, 000h, 000h
        db  000h, 000h, 03Bh, 0C8h, 073h, 007h, 089h, 04Fh, 0FCh, 08Bh, 0F9h, 0EBh, 0EBh, 0C7h, 047h, 0FCh
        db  000h, 000h, 000h, 000h, 0F8h, 0C3h, 06Ah, 000h, 050h, 0CDh, 020h, 055h, 000h, 001h, 000h, 059h
        db  059h, 0A1h, 030h, 004h, 000h, 000h, 085h, 0C0h, 074h, 00Bh, 06Ah, 000h, 050h, 0CDh, 020h, 055h
        db  000h, 001h, 000h, 059h, 059h, 0B8h, 030h, 000h, 000h, 000h, 00Fh, 0B7h, 03Dh, 00Ch, 000h, 000h
        db  000h, 003h, 03Dh, 004h, 000h, 000h, 000h, 00Fh, 0B7h, 00Dh, 00Eh, 000h, 000h, 000h, 003h, 00Dh
        db  004h, 000h, 000h, 000h, 033h, 0F6h, 0CDh, 020h, 004h, 000h, 017h, 000h, 0F9h, 0C3h, 000h, 000h
sub_4_0100:
        sub    esi, esi
        db  0BFh, 096h, 004h, 000h, 000h
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
        jb     SHORT loc_4_0140
        sub    eax, eax
        mov    bl, byte ptr [edx]
        test   bl, bl
        je     SHORT loc_4_013A
loc_4_0117:
        sub    bl, 30h
        cmp    bl, 8
        jae    SHORT loc_4_0140
        movzx  ebx, bl
        bts    eax, ebx
        inc    edx
        mov    bl, byte ptr [edx]
        test   bl, bl
        je     SHORT loc_4_013A
        cmp    bl, 2ch
        jne    SHORT loc_4_0140
        inc    edx
        mov    bl, byte ptr [edx]
        test   bl, bl
        je     SHORT loc_4_0140
        jmp    SHORT loc_4_0117
loc_4_013A:
        db  021h, 005h, 0D1h, 01Ah, 000h, 000h
loc_4_0140:
        ret
loc_4_0141:
        db  089h, 015h, 0D1h, 01Ah, 000h, 000h
        db  089h, 015h, 0D5h, 01Ah, 000h, 000h
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h
        xor    eax, eax
        test   ebx, 40000h
        je     SHORT loc_4_0162
        mov    eax, 20h
loc_4_0162:
        xor    esi, esi
        db  0BFh, 040h, 004h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        add    eax, 3
        shr    eax, 2
        je     SHORT loc_4_017F
        xor    ecx, ecx
        db  0CDh, 020h, 00Dh, 000h, 004h, 000h
loc_4_017F:
        mov    eax, 2ah
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        db  089h, 00Dh, 0B4h, 019h, 000h, 000h
        db  089h, 015h, 0B8h, 019h, 000h, 000h
        db  0BEh, 098h, 00Dh, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jb     SHORT loc_4_01DD
        mov    ecx, eax
        movzx  edx, cx
        shr    ecx, 10h
        mov    eax, 2ah
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        db  0BEh, 0DDh, 00Dh, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jb     SHORT loc_4_01DD
        mov    ecx, eax
        movzx  edx, cx
        shr    ecx, 10h
        mov    eax, 5ch
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        call   sub_4_0100
        clc
        ret
loc_4_01DD:
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_4_01E3:
        xor    eax, eax
        xor    esi, esi
        db  0BFh, 07Ah, 004h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        cmp    eax, 78h
        jbe    SHORT loc_4_01FC
        mov    eax, 78h
loc_4_01FC:
        add    al, al
        db  0A2h, 08Ah, 01Ah, 000h, 000h
        db  0A0h, 08Bh, 01Ah, 000h, 000h
        xor    esi, esi
        db  0BFh, 08Ah, 004h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        db  0A2h, 08Bh, 01Ah, 000h, 000h
        mov    eax, 20h
        xor    esi, esi
        db  0BFh, 038h, 004h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        db  0A3h, 034h, 004h, 000h, 000h
        push   0
        push   8
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    SHORT loc_4_0247
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_4_0247:
        db  0A3h, 0A4h, 019h, 000h, 000h
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 1607h
        mov    word ptr [ebp + 10h], 14h
        xor    eax, eax
        mov    word ptr [ebp + 18h], ax
        mov    word ptr [ebp + 38h], ax
        mov    word ptr [ebp], ax
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 38h]
        or     ax, word ptr [ebp]
        je     SHORT loc_4_02A9
        xchg   esi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        db  0BFh, 008h, 01Ah, 000h, 000h
        mov    ecx, 20h
        cld
        rep movsd
loc_4_02A9:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  083h, 03Dh, 0D1h, 01Ah, 000h, 000h, 000h
        je     NEAR PTR loc_4_0357
        mov    eax, 0ch
        xor    esi, esi
        db  0BFh, 04Bh, 004h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        add    eax, 3
        shr    eax, 2
        cmp    eax, 0ffh
        jbe    SHORT loc_4_02EF
        mov    eax, 0ffh
loc_4_02EF:
        push   eax
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        mov    edx, eax
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        sub    edx, eax
        sub    edx, 20h
        jae    SHORT loc_4_030B
        mov    edx, 3
loc_4_030B:
        pop    eax
        cmp    eax, edx
        jbe    SHORT loc_4_0312
        mov    eax, edx
loc_4_0312:
        db  0A2h, 088h, 01Ah, 000h, 000h
        mov    bx, 1
        mov    bh, al
        mov    cl, 10h
        xor    eax, eax
        db  0CDh, 020h, 004h, 000h, 006h, 000h
        mov    eax, 1388h
        mov    ecx, 3
        xor    esi, esi
        db  0BFh, 057h, 004h, 000h, 000h
        db  0CDh, 020h, 0ADh, 000h, 001h, 000h
        db  0A3h, 0BCh, 019h, 000h, 000h
        xor    eax, eax
        xor    esi, esi
        db  0BFh, 068h, 004h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        db  0A2h, 089h, 01Ah, 000h, 000h
loc_4_0357:
        db  068h, 08Ch, 019h, 000h, 000h
        db  068h, 03Ch, 019h, 000h, 000h
        db  0CDh, 020h, 029h, 001h, 001h, 000h
        add    esp, 8
        clc
        call   sub_4_0371
        ret
sub_4_0371:
        push   0
        push   7
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    SHORT loc_4_0387
        stc
        jmp    NEAR PTR loc_4_042F
loc_4_0387:
        mov    word ptr [eax], 0cbfah
        push   eax
        shl    eax, 0ch
        shr    ax, 0ch
        db  0A3h, 0DDh, 01Ah, 000h, 000h
        call   0
        pop    eax
        mov    edi, eax
        jb     NEAR PTR loc_4_042F
        mov    eax, 2ah
        db  0BEh, 0F0h, 00Dh, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        mov    eax, 5ch
        db  0BEh, 020h, 00Eh, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        db  0BEh, 012h, 014h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        jb     SHORT loc_4_042F
        db  0A3h, 0ACh, 019h, 000h, 000h
        xor    eax, eax
        db  00Bh, 005h, 0D5h, 01Ah, 000h, 000h
        jne    SHORT loc_4_042E
        xor    eax, eax
        mov    al, 5ch
        db  0CDh, 020h, 042h, 000h, 001h, 000h
        db  066h, 089h, 015h, 0E1h, 01Ah, 000h, 000h
        db  066h, 089h, 00Dh, 0E3h, 01Ah, 000h, 000h
        mov    eax, edi
        add    eax, 2
        mov    word ptr [eax + 1], dx
        mov    word ptr [eax + 3], cx
        mov    byte ptr [eax], 0eah
        or     dx, cx
        jne    SHORT loc_4_0414
        dec    eax
        mov    byte ptr [eax], 0cfh
loc_4_0414:
        shl    eax, 0ch
        shr    ax, 0ch
        movzx  edx, ax
        shr    eax, 10h
        mov    ecx, eax
        mov    eax, 5ch
        db  0CDh, 020h, 043h, 000h, 001h, 000h
loc_4_042E:
        clc
loc_4_042F:
        ret
        db  000h, 000h, 000h, 000h
loc_4_0434:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0438:
        dec    esi
        db  075h, 06Dh
        dec    eax
        inc    ebx
        inc    edx
        jae    SHORT loc_4_0440
loc_4_0440:
        dec    esi
        db  065h, 074h, 044h
        dec    ebp
        inc    ecx
        push   ebx
        imul   edi, dword ptr [edx + 65h], 74654e00h
        dec    eax
        popad
        db  070h, 053h
        imul   edi, dword ptr [edx + 65h], 74654e00h
        inc    ecx
        db  073h, 079h
        outsb
        arpl   word ptr [eax + 54h], bp
        imul   ebp, dword ptr [ebp + 65h], 74756fh
loc_4_0468:
        dec    esi
        db  065h, 074h, 041h
        db  073h, 079h
        outsb
        arpl   word ptr [eax + 46h], bp
        popad
        insb
        insb
        bound  esp, qword ptr [ecx + 63h]
        db  06Bh, 000h
loc_4_047A:
        dec    esi
        db  065h, 074h, 052h
        imul   esi, dword ptr fs:[edx + 54h], 6f656d69h
        db  075h, 074h
        add    byte ptr [esi + 6fh], cl
        push   edi
        popad
        imul   esi, dword ptr [esi + ecx*2 + 65h], 4f4974h
loc_4_0496:
        push   esi
        cmp    byte ptr [esi], dh
        dec    ebp
        outsd
        dec    esp
        popad
        outsb
        popad
        db  073h, 000h
        db  000h
_ITEXT ends
        end
