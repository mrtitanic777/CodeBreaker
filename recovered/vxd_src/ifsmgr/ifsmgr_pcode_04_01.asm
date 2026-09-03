; ifsmgr_pcode_04_01.asm -- IFSMGR object 4 segment _PTEXT  (part 1/5)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_0000:
        add    byte ptr [eax], al
        db  0BFh, 0B9h, 002h, 000h, 000h
        db  0BAh, 0DCh, 055h, 000h, 000h
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
        jb     NEAR PTR loc_4_01D4
        mov    esi, edx
        lea    edi, [esp + 24h]
        call   sub_4_026B
        jb     NEAR PTR loc_4_01D4
        mov    eax, dword ptr [esp + 20h]
        cmp    eax, dword ptr [esp + 24h]
        jne    SHORT loc_4_0038
        mov    byte ptr [esp + 10h], 1
loc_4_0038:
        mov    ecx, 1fch
        mov    bx, word ptr [esp + 2ch]
        db  0BAh, 08Dh, 000h, 000h, 000h
sub_4_0047:
        mov    ah, 3fh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     NEAR PTR loc_4_01D4
        mov    edi, edx
        mov    ecx, dword ptr [edi]
        lea    edi, [edi + 4]
loc_4_005E:
        mov    esi, edx
        add    esi, 1f8h
loc_4_0066:
        cmp    edi, esi
        jae    NEAR PTR loc_4_01B0
        mov    ebx, dword ptr [edi]
        cmp    dword ptr [esp + 20h], ebx
        je     SHORT loc_4_0086
        cmp    dword ptr [esp + 24h], ebx
        je     SHORT loc_4_008C
loc_4_007C:
        add    edi, 1ch
        loop   loc_4_0066
        jmp    NEAR PTR loc_4_01D4
loc_4_0086:
        mov    dword ptr [esp + 18h], edi
        jmp    SHORT loc_4_0090
loc_4_008C:
        mov    dword ptr [esp + 14h], edi
loc_4_0090:
        cmp    byte ptr [esp + 10h], 1
        je     SHORT loc_4_00A7
        cmp    dword ptr [esp + 14h], -1
        je     SHORT loc_4_007C
        cmp    dword ptr [esp + 18h], -1
        je     SHORT loc_4_007C
        jmp    SHORT loc_4_00AF
loc_4_00A7:
        mov    dword ptr [esp + 18h], edi
        mov    dword ptr [esp + 14h], edi
loc_4_00AF:
        mov    esi, ebx
        mov    bx, word ptr [esp + 2ch]
loc_4_00B6:
        or     edi, edi
        je     NEAR PTR loc_4_01DB
        mov    ax, 4200h
sub_4_00C2:
        mov    dx, word ptr [edi + 4]
        mov    cx, word ptr [edi + 6]
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     NEAR PTR loc_4_01D4
        push   100h
        push   dword ptr [edi + 8]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_01D4
        mov    edx, eax
        mov    ecx, dword ptr [edi + 8]
        mov    ah, 3fh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     NEAR PTR loc_4_01D4
        db  08Dh, 02Dh, 094h, 001h, 000h, 000h
        mov    eax, dword ptr [esp + 20h]
        movzx  ecx, byte ptr [esp + 10h]
        push   ebx
        mov    ebx, dword ptr [edi + 8]
        call   sub_4_0228
        pop    ebx
        mov    ax, 4200h
        mov    dx, word ptr [edi + 10h]
        mov    cx, word ptr [edi + 12h]
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     NEAR PTR loc_4_01D4
        push   100h
        push   dword ptr [edi + 14h]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_01D4
        mov    edx, eax
        mov    ecx, dword ptr [edi + 14h]
        mov    ah, 3fh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_4_01D4
        db  08Dh, 02Dh, 08Ch, 001h, 000h, 000h
        mov    eax, dword ptr [esp + 20h]
        movzx  ecx, byte ptr [esp + 10h]
        push   ebx
        mov    ebx, dword ptr [edi + 14h]
        call   sub_4_0228
        pop    ebx
        or     ecx, ecx
        jne    SHORT loc_4_01DB
        cmp    esi, dword ptr [esp + 20h]
        je     SHORT loc_4_019B
        mov    dword ptr [esp + 14h], 0
        mov    esi, dword ptr [esp + 20h]
        mov    edi, dword ptr [esp + 18h]
        jmp    NEAR PTR loc_4_00B6
loc_4_019B:
        mov    dword ptr [esp + 18h], 0
        mov    esi, dword ptr [esp + 24h]
        mov    edi, dword ptr [esp + 14h]
        jmp    NEAR PTR loc_4_00B6
loc_4_01B0:
        push   ecx
        mov    ecx, 1f8h
        mov    bx, word ptr [esp + 2ch]
        db  0BAh, 08Dh, 000h, 000h, 000h
        mov    ah, 3fh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ecx
        jb     SHORT loc_4_01D4
        mov    edi, edx
        jmp    NEAR PTR loc_4_005E
loc_4_01D4:
        call   sub_4_029B
        jmp    SHORT loc_4_01E8
loc_4_01DB:
        mov    eax, dword ptr [esp + 20h]
        mov    edx, dword ptr [esp + 24h]
        call   sub_4_0300
loc_4_01E8:
        mov    ecx, dword ptr [esp + 28h]
        jecxz  loc_4_01FA
        push   ecx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        or     eax, eax
loc_4_01FA:
        mov    bx, word ptr [esp + 2ch]
        or     bx, bx
        je     SHORT loc_4_020E
        mov    ah, 3eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_4_020E:
        db  08Dh, 00Dh, 058h, 035h, 001h, 000h
        db  081h, 0E9h, 062h, 029h, 001h, 000h
        db  089h, 00Dh, 038h, 000h, 000h, 000h
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        add    esp, 20h
        ret
sub_4_0228:
        cmp    esi, eax
        jne    SHORT loc_4_0231
        mov    dword ptr [ebp + 4], edx
        jmp    SHORT loc_4_0234
loc_4_0231:
        mov    dword ptr [ebp], edx
loc_4_0234:
        mov    eax, 0ch
        db  02Bh, 005h, 07Ch, 003h, 000h, 000h
        shl    eax, 2
        db  005h, 004h, 000h, 000h, 000h
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], ebx
        db  083h, 02Dh, 07Ch, 003h, 000h, 000h, 002h
        jecxz  loc_4_026A
        add    eax, 8
        mov    dword ptr [eax], edx
sub_4_025A:
        mov    dword ptr [eax + 4], ebx
        db  083h, 02Dh, 07Ch, 003h, 000h, 000h, 002h
        mov    dword ptr [ebp + 4], edx
        mov    dword ptr [ebp], edx
loc_4_026A:
        ret
sub_4_026B:
        xor    ebx, ebx
        xor    eax, eax
        lodsb
        or     al, al
        je     SHORT loc_4_0297
        xor    edx, edx
        mov    ecx, 0ah
        jmp    SHORT loc_4_0282
loc_4_027D:
        lodsb
        or     al, al
        je     SHORT loc_4_0298
loc_4_0282:
        cmp    al, 30h
        jb     SHORT loc_4_0297
        cmp    al, 39h
        ja     SHORT loc_4_0297
        sub    al, 30h
        xchg   ebx, eax
        mul    ecx
        add    eax, ebx
        adc    edx, 0
        xchg   ebx, eax
        jmp    SHORT loc_4_027D
loc_4_0297:
        stc
loc_4_0298:
        mov    dword ptr [edi], ebx
        ret
sub_4_029B:
        db  0C7h, 005h, 07Ch, 003h, 000h, 000h, 00Ch, 000h, 000h, 000h
        mov    edi, 1b5h
sub_4_02AA:
        db  08Dh, 02Dh, 094h, 001h, 000h, 000h
        mov    ebx, 22ch
        db  0BEh, 0B8h, 019h, 000h, 000h
        call   sub_4_0343
        db  08Dh, 02Dh, 08Ch, 001h, 000h, 000h
        mov    ebx, 27dh
        db  0BEh, 0E4h, 01Bh, 000h, 000h
        call   sub_4_0343
        mov    edi, 4e4h
        db  08Dh, 02Dh, 094h, 001h, 000h, 000h
        mov    ebx, 22ch
        db  0BEh, 064h, 01Eh, 000h, 000h
        call   sub_4_0343
        db  08Dh, 02Dh, 08Ch, 001h, 000h, 000h
        mov    ebx, 19dh
        db  0BEh, 090h, 020h, 000h, 000h
        jmp    SHORT sub_4_0343
sub_4_0300:
        push   ecx
        push   esi
        push   edi
        db  0BEh, 0B8h, 027h, 000h, 000h
        cmp    eax, 359h
        je     SHORT loc_4_0332
        cmp    edx, 4e6h
        je     SHORT loc_4_0332
        db  0BEh, 030h, 022h, 000h, 000h
        cmp    eax, 2e1h
        je     SHORT loc_4_0332
        cmp    eax, 365h
        je     SHORT loc_4_0332
        cmp    edx, 4e5h
        jne    SHORT loc_4_033F
loc_4_0332:
        mov    ecx, 587h
        db  0BFh, 0F2h, 001h, 000h, 000h
        cld
        rep movsb
loc_4_033F:
        pop    edi
        pop    esi
        pop    ecx
        ret
sub_4_0343:
        push   edi
        push   100h
        push   ebx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_036E
        mov    edi, eax
        mov    ecx, ebx
        rep movsb
        pop    edi
        mov    esi, edi
        mov    edx, eax
        mov    eax, 1b5h
        xor    ecx, ecx
        jmp    NEAR PTR sub_4_0228
loc_4_036E:
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
        mov    edx, 1
        db  08Bh, 035h, 0B8h, 061h, 000h, 000h
        mov    eax, esi
        cmp    word ptr [esi + 19h], -1
        je     SHORT loc_4_03A1
loc_4_0388:
        mov    eax, esi
        mov    esi, dword ptr [esi + 19h]
        cmp    si, -1
        je     SHORT loc_4_03A1
        inc    edx
        movzx  ecx, si
        shr    esi, 10h
        shl    esi, 4
        add    esi, ecx
        jmp    SHORT loc_4_0388
loc_4_03A1:
        db  0A3h, 0D7h, 061h, 000h, 000h
        db  0A1h, 040h, 066h, 000h, 000h
        sub    eax, edx
        js     SHORT loc_4_03D3
        db  0A2h, 0CEh, 061h, 000h, 000h
        mov    ecx, 3dh
        mul    ecx
        push   204h
        push   eax
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_03D3
        db  0A3h, 0CFh, 061h, 000h, 000h
loc_4_03D3:
        ret
        db  083h, 0ECh, 010h, 055h, 056h, 057h, 053h, 08Dh, 07Ch, 024h, 01Ch, 08Dh, 035h, 0C2h, 002h, 000h
        db  000h, 057h, 056h, 068h, 002h, 000h, 000h, 080h, 0CDh, 020h, 048h, 001h, 001h, 000h, 083h, 0C4h
        db  00Ch, 00Bh, 0C0h, 075h, 060h, 08Dh, 01Dh, 044h, 003h, 000h, 000h
loc_4_03FF:
        mov    ecx, dword ptr [ebx]
        jecxz  loc_4_0449
        lea    edx, [esp + 18h]
        mov    eax, dword ptr [ebx + 4]
        lea    edi, [esp + 14h]
        lea    esi, [esp + 10h]
        mov    dword ptr [edx], 3
        mov    dword ptr [edi], ecx
        mov    ecx, dword ptr [esp + 1ch]
        push   edi
        push   esi
        push   edx
        push   0
        push   eax
        push   ecx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_4_0444
        cmp    dword ptr [esp + 18h], 3
        jne    SHORT loc_4_0444
        lea    esi, [esp + 10h]
        mov    edi, dword ptr [ebx + 8]
        mov    ecx, dword ptr [ebx]
        rep movsb
loc_4_0444:
        add    ebx, 0ch
IFSMgr_Yield:
        jmp    SHORT loc_4_03FF
loc_4_0449:
        mov    ecx, dword ptr [esp + 1ch]
        push   ecx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        sub    eax, eax
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        add    esp, 10h
        ret
        db  000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   80h
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0fffh
        shr    eax, 0ch
        push   eax
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        mov    ecx, eax
        or     ecx, edx
        je     SHORT loc_4_0495
        mov    eax, edx
loc_4_0495:
        leave
        ret
        db  06Ah, 000h, 0CDh, 020h, 059h, 000h, 001h, 000h, 083h, 0C4h, 004h, 092h, 0C1h, 0E0h, 00Ch, 0C3h
        db  000h, 0B8h, 02Fh, 000h, 000h, 000h, 0BEh, 008h, 012h, 000h, 000h, 0CDh
loc_4_04B3:
        and    byte ptr [ecx], al
        add    dword ptr [eax], eax
        ret
        db  0B8h, 025h, 000h, 000h, 000h, 0BEh, 0D1h, 016h, 000h, 000h, 0CDh, 020h, 041h, 000h, 001h, 000h
        db  0B8h, 026h, 000h, 000h, 000h, 0BEh, 0D1h, 016h, 000h, 000h, 0CDh, 020h, 041h, 000h, 001h, 000h
        db  0B8h, 025h, 000h, 000h, 000h, 0CDh, 020h, 044h, 000h, 001h, 000h, 089h, 00Dh, 052h, 000h, 000h
        db  000h, 089h, 015h, 04Eh, 000h, 000h, 000h, 0BAh, 025h, 000h, 000h, 000h, 0BEh, 074h, 016h, 000h
        db  000h, 0CDh, 020h, 00Ch, 000h, 001h, 000h, 00Fh, 0B7h, 0D0h, 08Bh, 0C8h, 0C1h, 0E9h, 010h, 0B8h
        db  025h, 000h, 000h, 000h, 0CDh, 020h, 045h, 000h, 001h, 000h, 0B8h, 026h, 000h, 000h, 000h, 0CDh
        db  020h, 044h, 000h, 001h, 000h, 089h, 00Dh, 05Ah, 000h, 000h, 000h, 089h, 015h, 056h, 000h, 000h
        db  000h, 0BAh, 026h, 000h, 000h, 000h, 0BEh, 074h, 016h, 000h, 000h, 0CDh, 020h, 00Ch, 000h, 001h
        db  000h, 00Fh, 0B7h, 0D0h, 08Bh, 0C8h, 0C1h, 0E9h, 010h, 0B8h, 026h, 000h, 000h, 000h
sub_4_0547:
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        ret
        db  000h, 000h, 0B8h, 017h, 000h, 000h, 000h, 0BEh, 01Eh, 017h, 000h, 000h, 0CDh, 020h, 041h, 000h
        db  001h, 000h, 0C3h, 000h, 000h, 000h, 0B8h, 021h, 000h, 000h, 000h, 0BEh, 016h, 015h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h, 083h, 0ECh, 06Ch, 057h, 08Dh, 07Ch, 024h, 004h, 0CDh, 020h
        db  08Dh, 000h, 001h, 000h, 05Fh, 0CDh, 020h, 082h, 000h, 001h, 000h, 066h, 0C7h, 045h, 01Ch, 021h
        db  035h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 00Fh, 0B7h, 045h, 038h
        db  0C1h, 0E0h, 004h, 00Fh, 0B7h, 04Dh, 010h, 003h, 0C1h, 0A3h, 008h, 06Bh, 000h, 000h, 0CDh, 020h
        db  086h, 000h, 001h, 000h, 056h, 08Dh, 074h, 024h, 004h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 05Eh
        db  083h, 0C4h, 06Ch, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 044h, 000h, 001h, 000h, 089h, 00Dh
        db  0FCh, 06Ah, 000h, 000h, 089h, 015h, 0F8h, 06Ah, 000h, 000h, 0BEh, 06Ch, 014h, 000h, 000h, 0CDh
        db  020h, 00Ch, 000h, 001h, 000h, 00Fh, 0B7h, 0D0h, 08Bh, 0C8h, 0C1h, 0E9h, 010h, 0B8h, 021h, 000h
        db  000h, 000h, 0CDh, 020h, 045h, 000h, 001h, 000h, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 0A1h, 044h, 059h, 000h, 000h, 00Fh, 0B7h, 000h, 003h, 005h, 0B0h, 065h, 000h, 000h
        db  0A3h, 09Ch, 065h, 000h, 000h, 0A1h, 044h, 059h, 000h, 000h, 00Fh, 0B7h, 040h, 004h, 003h, 005h
        db  0B0h, 065h, 000h, 000h, 0A3h, 0A0h, 065h, 000h, 000h, 0A1h, 044h, 059h, 000h, 000h, 0C6h, 040h
        db  011h, 003h, 0F6h, 005h, 078h, 003h, 000h, 000h, 001h, 074h, 010h, 0A1h, 044h, 059h, 000h, 000h
        db  080h, 00Dh, 00Ch, 06Bh, 000h, 000h, 001h, 080h, 048h, 011h, 004h, 0A1h, 044h, 059h, 000h, 000h
        db  0C6h, 080h, 02Eh, 001h, 000h, 000h, 000h, 0FFh, 035h, 040h, 059h, 000h, 000h, 06Ah, 001h, 0E8h
        db  060h, 0EAh, 0FCh, 0FFh, 083h, 0C4h, 008h, 00Bh, 0C0h, 075h, 002h, 02Bh, 0C0h, 0C3h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0BFh, 05Ch, 02Dh, 000h, 000h
        mov    ebx, 3
loc_4_067F:
        cmp    dword ptr [edi + 8], ebx
        lea    esi, [edi + 14h]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [esi]
        push   dword ptr [edi + 4]
        push   dword ptr [edi]
        je     SHORT loc_4_0699
        call   sub_4_085C
        jmp    SHORT loc_4_069E
loc_4_0699:
        call   sub_4_083D
loc_4_069E:
        add    esp, 10h
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_06B3
        cmp    dword ptr [edi + 0ch], eax
        ja     SHORT loc_4_06B1
        cmp    dword ptr [edi + 10h], eax
        jae    SHORT loc_4_06B3
loc_4_06B1:
        mov    eax, dword ptr [esi]
loc_4_06B3:
        mov    ecx, dword ptr [edi + 18h]
        add    edi, 1ch
        mov    dword ptr [ecx], eax
        db  081h, 0FFh, 078h, 02Dh, 000h, 000h
        jb     SHORT loc_4_067F
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   ebx
        push   esi
        push   edi
        db  08Bh, 00Dh, 0B4h, 055h, 000h, 000h
        sub    edx, edx
        mov    eax, dword ptr [ebp + 8]
        add    ecx, eax
        sub    bx, bx
        db  039h, 015h, 0D0h, 066h, 000h, 000h
        jle    SHORT loc_4_0760
        mov    esi, 80h
loc_4_06F9:
        mov    edi, dword ptr [ecx + 4ch]
        movzx  eax, bx
        mov    eax, dword ptr [edi + eax*4]
        cmp    word ptr [eax], 0
        je     SHORT loc_4_0753
        test   word ptr [eax + 5], si
        je     SHORT loc_4_0753
        mov    eax, dword ptr [eax + 7]
        db  039h, 005h, 0E4h, 06Fh, 000h, 000h
        je     SHORT loc_4_073F
        db  039h, 005h, 008h, 070h, 000h, 000h
        je     SHORT loc_4_073F
        db  039h, 005h, 0E8h, 06Fh, 000h, 000h
        jne    SHORT loc_4_0730
        mov    eax, 1
        jmp    SHORT loc_4_0741
loc_4_0730:
        db  039h, 005h, 0ECh, 06Fh, 000h, 000h
        jne    SHORT loc_4_0753
        mov    eax, 2
        jmp    SHORT loc_4_0741
loc_4_073F:
        sub    eax, eax
loc_4_0741:
        db  088h, 09Ah, 098h, 083h, 000h, 000h
        db  088h, 082h, 0A4h, 083h, 000h, 000h
        inc    edx
        cmp    edx, 8
sub_4_0751:
        jge    SHORT loc_4_0785
loc_4_0753:
        inc    bx
        movzx  eax, bx
        db  03Bh, 005h, 0D0h, 066h, 000h, 000h
        jl     SHORT loc_4_06F9
loc_4_0760:
        cmp    edx, 8
        jge    SHORT loc_4_0785
        mov    esi, 8
        sub    esi, edx
        db  08Dh, 0BAh, 0A4h, 083h, 000h, 000h
        mov    eax, 0ffffffffh
        mov    ecx, esi
        shr    ecx, 2
        rep stosd
        mov    ecx, esi
        and    ecx, 3
        rep stosb
loc_4_0785:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 068h, 0C0h, 0D4h, 001h, 000h, 068h, 08Ch, 06Eh, 000h, 000h, 0E8h, 0A9h
        db  0F7h, 0FCh, 0FFh, 083h, 0C4h, 008h, 0C3h
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        lea    eax, [ebp - 104h]
sub_4_07B2:
        push   eax
        db  0E8h, 099h, 0EFh, 0FCh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 80h], eax
        mov    dword ptr [ebp - 0e0h], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [ebp - 0feh], ax
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 0F1h, 0FFh, 0FCh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_080E
        push   dword ptr [ebp + 10h]
        push   1
        push   dword ptr [ebp + 8]
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 06Eh, 09Ch, 0FDh, 0FFh
        add    esp, 10h
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 0BAh, 000h, 0FDh, 0FFh
        add    esp, 4
loc_4_080E:
        movsx  eax, word ptr [ebp - 0eah]
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 056h, 057h, 08Bh, 07Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h
        db  014h, 0CDh, 020h, 0B3h, 000h, 001h, 000h, 08Bh, 0C2h, 01Bh, 0D2h, 08Bh, 04Ch, 024h, 018h, 089h
        db  011h, 05Fh, 05Eh, 0C3h
sub_4_083D:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 14h]
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        sbb    edx, edx
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [ecx], edx
        pop    edi
        pop    esi
        ret
sub_4_085C:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 14h]
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
        sbb    edx, edx
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [ecx], edx
        pop    edi
        pop    esi
        ret
        db  056h, 057h, 08Bh, 07Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 044h, 024h, 014h, 0CDh, 020h
        db  0B1h, 000h, 001h, 000h, 01Bh, 0D2h, 08Bh, 04Ch, 024h, 018h, 089h, 011h, 083h, 0CAh, 001h, 023h
        db  0C2h, 05Fh, 05Eh, 0C3h, 053h, 056h, 057h, 0CDh, 020h, 000h, 000h, 00Dh, 000h, 0B1h, 095h, 03Dh
        db  00Ah, 002h, 000h, 000h, 072h, 003h, 080h, 0F1h, 0C4h, 0B0h, 02Eh, 0B4h, 000h, 0BBh, 080h, 000h
        db  080h, 001h, 0BEh, 05Ah, 001h, 001h, 000h, 02Bh, 0FFh, 0CDh, 020h, 001h, 000h, 00Dh, 000h, 072h
        db  011h, 0F6h, 0C1h, 040h, 075h, 00Ch, 08Bh, 01Dh, 0B0h, 055h, 000h, 000h, 0CDh, 020h, 004h, 000h
        db  00Dh, 000h, 05Fh, 05Eh, 05Bh, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 06Ah, 001h, 0E8h, 05Ah, 01Bh, 0FDh, 0FFh, 083h, 0C4h, 004h, 0A3h
        db  00Ch, 071h, 000h, 000h, 00Bh, 0C0h, 0B8h, 001h, 000h, 000h, 000h, 00Fh, 084h, 0AAh, 000h, 000h
        db  000h, 06Ah, 001h, 0E8h, 03Eh, 01Bh, 0FDh, 0FFh, 083h, 0C4h, 004h, 0A3h, 010h, 071h, 000h, 000h
        db  0C7h, 040h, 00Ch, 000h, 010h, 000h, 000h, 0A1h, 010h, 071h, 000h, 000h, 0C7h, 040h, 008h, 000h
        db  000h, 000h, 000h, 0A1h, 00Ch, 071h, 000h, 000h, 0C7h, 040h, 00Ch, 000h, 010h, 000h, 000h, 0A1h
        db  00Ch, 071h, 000h, 000h, 0C7h, 040h, 004h, 049h, 046h, 053h, 048h, 0A1h, 00Ch, 071h, 000h, 000h
        db  0C7h, 040h, 008h, 000h, 000h, 000h, 000h, 0A1h, 00Ch, 071h, 000h, 000h, 08Bh, 050h, 00Ch, 083h
        db  0EAh, 020h, 066h, 089h, 050h, 018h, 08Bh, 00Dh, 00Ch, 071h, 000h, 000h, 0A1h, 00Ch, 071h, 000h
        db  000h, 083h, 0C1h, 01Ch, 089h, 048h, 014h, 08Bh, 0C2h, 0F7h, 0D8h, 089h, 001h, 0A1h, 00Ch, 071h
        db  000h, 000h, 003h, 0CAh, 089h, 048h, 010h, 0A1h, 00Ch, 071h, 000h, 000h, 06Ah, 001h, 066h, 0C7h
        db  040h, 01Ah, 000h, 000h, 0C7h, 001h, 000h, 000h, 000h, 000h, 068h, 0A0h, 036h, 000h, 000h, 0E8h
        db  004h, 01Dh, 0FDh, 0FFh, 083h, 0C4h, 008h, 068h, 030h, 075h, 000h, 000h, 068h, 020h, 071h, 000h
        db  000h, 0E8h, 097h, 0F5h, 0FCh, 0FFh, 083h, 0C4h, 008h, 02Bh, 0C0h, 0C3h, 000h, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h
        db  000h, 000h, 000h, 0FFh, 000h, 018h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 002h, 000h, 003h
        db  000h, 004h, 000h, 005h, 000h, 006h, 000h, 007h, 000h, 008h, 000h, 009h, 000h, 00Ah, 000h, 00Bh
        db  000h, 00Ch, 000h, 00Dh, 000h, 00Eh, 000h, 00Fh, 000h, 010h, 000h, 011h, 000h, 012h, 000h, 013h
        db  000h, 014h, 000h, 015h, 000h, 016h, 000h, 017h, 000h, 018h, 000h, 019h, 000h, 01Ah, 000h, 01Bh
        db  000h, 01Ch, 000h, 01Dh, 000h, 01Eh, 000h, 01Fh, 000h, 020h, 000h, 021h, 000h, 022h, 000h, 023h
        db  000h, 024h, 000h, 025h, 000h, 026h, 000h, 027h, 000h, 028h, 000h, 029h, 000h, 02Ah, 000h, 02Bh
        db  000h, 02Ch, 000h, 02Dh, 000h, 02Eh, 000h, 02Fh, 000h, 030h, 000h, 031h, 000h, 032h, 000h, 033h
        db  000h, 034h, 000h, 035h, 000h, 036h, 000h, 037h, 000h, 038h, 000h, 039h, 000h, 03Ah, 000h, 03Bh
        db  000h, 03Ch, 000h, 03Dh, 000h, 03Eh, 000h, 03Fh, 000h, 040h, 000h, 041h, 000h, 042h, 000h, 043h
        db  000h, 044h, 000h, 045h, 000h, 046h, 000h, 047h, 000h, 048h, 000h, 049h, 000h, 04Ah, 000h, 04Bh
        db  000h, 04Ch, 000h, 04Dh, 000h, 04Eh, 000h, 04Fh, 000h, 050h, 000h, 051h, 000h, 052h, 000h, 053h
        db  000h, 054h, 000h, 055h, 000h, 056h, 000h, 057h, 000h, 058h, 000h, 059h, 000h, 05Ah, 000h, 05Bh
        db  000h, 05Ch, 000h, 05Dh, 000h, 05Eh, 000h, 05Fh, 000h, 060h, 000h, 061h, 000h, 062h, 000h, 063h
        db  000h, 064h, 000h, 065h, 000h, 066h, 000h, 067h, 000h, 068h, 000h, 069h, 000h, 06Ah, 000h, 06Bh
        db  000h, 06Ch, 000h, 06Dh, 000h, 06Eh, 000h, 06Fh, 000h, 070h, 000h, 071h, 000h, 072h, 000h, 073h
        db  000h, 074h, 000h, 075h, 000h, 076h, 000h, 077h, 000h, 078h, 000h, 079h, 000h, 07Ah, 000h, 07Bh
        db  000h, 07Ch, 000h, 07Dh, 000h, 07Eh, 000h, 07Fh, 000h, 0C7h, 000h, 0FCh, 000h, 0E9h, 000h, 0E2h
        db  000h, 0E4h, 000h, 0E0h, 000h, 0E5h, 000h, 0E7h, 000h, 0EAh, 000h, 0EBh, 000h, 0E8h, 000h, 0EFh
        db  000h, 0EEh, 000h, 0ECh, 000h, 0C4h, 000h, 0C5h, 000h, 0C9h, 000h, 0E6h, 000h, 0C6h, 000h, 0F4h
        db  000h, 0F6h, 000h, 0F2h, 000h, 0FBh, 000h, 0F9h, 000h, 0FFh, 000h, 0D6h, 000h, 0DCh, 000h, 0A2h
        db  000h, 0A3h, 000h, 0A5h, 000h, 0A7h, 020h, 092h, 001h, 0E1h, 000h, 0EDh, 000h, 0F3h, 000h, 0FAh
        db  000h, 0F1h, 000h, 0D1h, 000h, 0AAh, 000h, 0BAh, 000h, 0BFh, 000h, 010h, 023h, 0ACh, 000h, 0BDh
        db  000h, 0BCh, 000h, 0A1h, 000h, 0ABh, 000h, 0BBh, 000h, 091h, 025h, 092h, 025h, 093h, 025h, 002h
        db  025h, 024h, 025h, 061h, 025h, 062h, 025h, 056h, 025h, 055h, 025h, 063h, 025h, 051h, 025h, 057h
        db  025h, 05Dh, 025h, 05Ch, 025h, 05Bh, 025h, 010h, 025h, 014h, 025h, 034h, 025h, 02Ch, 025h, 01Ch
        db  025h, 000h, 025h, 03Ch, 025h, 05Eh, 025h, 05Fh, 025h, 05Ah, 025h, 054h, 025h
loc_4_0B78:
        imul   esp, dword ptr [25602566h], 256c2550h
        and    eax, 25642568h
        and    eax, 25582559h
        push   edx
        and    eax, 256b2553h
        push   25h
        sbb    byte ptr [2588250ch], ah
        test   byte ptr [2590258ch], ah
        and    byte ptr [0df03b1h], 93h
        add    eax, eax
        add    esp, dword ptr [ebx - 4afc3cfdh]
        add    ah, al
        add    esp, dword ptr [esi - 56fc67fdh]
        add    esi, dword ptr [ebx + eax + 3c6221eh]
        mov    ch, 3
        sub    dword ptr [edx], esp
        popad
        and    dh, byte ptr [ecx + 64226500h]
        and    ah, byte ptr [eax]
        and    esp, dword ptr [ecx]
        and    esi, edi
        add    byte ptr [eax + 22h], cl
        mov    al, 0
        sbb    dword ptr [edx], esp
        mov    bh, 0
        sbb    ah, byte ptr [edx]
        db  07Fh, 020h
        mov    dl, 0
        mov    al, byte ptr [230225h]
        db  000h, 048h
IFSMgr_Win32GetVMCurdir:
        add    byte ptr [eax], al
        add    bh, bh
        add    byte ptr [ecx], ah
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [edx + 21019201h], dl
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 2303c603h], dl
        add    eax, dword ptr [eax]
        add    byte ptr [eax + eax], cl
        xor    byte ptr [eax], al
        db  07Fh, 020h
        db  07Fh, 020h
        mov    eax, dword ptr [ebx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    al, 0
        cmpsd
        and    byte ptr [edi + 38d20h], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl
        and    ch, byte ptr [ecx]
        and    cl, byte ptr [edi + 24000003h]
        add    byte ptr [eax + eax + 48h], ch
        and    cl, byte ptr [eax + 22h]
        mov    cl, 3
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pushad
        add    byte ptr [ecx + 22h], ah
        and    dh, byte ptr gs:[ebx + 3]
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        and    edx, dword ptr [eax]
        and    edi, dword ptr [ebp + 54000003h]
        add    byte ptr [eax], bh
        and    byte ptr [ebx], ah
        and    dword ptr [ebx], esp
        fild   dword ptr [ebx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        test   byte ptr [eax], al
        db  000h, 025h, 03Ch
sub_4_0C63:
        and    eax, 3dfh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        and    eax, 45925a0h
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    al, byte ptr [ebx]
        add    al, 5
        push   es
        pop    es
        or     byte ptr [ecx], cl
        or     cl, byte ptr [ebx]
        or     al, 0dh
        push   cs
        movups xmm2, xmmword ptr [ecx]
        adc    dl, byte ptr [ebx]
        adc    al, 15h
        push   ss
        pop    ss
        sbb    byte ptr [ecx], bl
        sbb    bl, byte ptr [ebx]
        sbb    al, 1dh
        push   ds
        pop    ds
        and    byte ptr [ecx], ah
        and    ah, byte ptr [ebx]
        and    al, 25h
        daa
        sub    byte ptr [ecx], ch
        sub    ch, byte ptr [ebx]
        sub    al, 2dh
        das
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
        cmp    byte ptr [ecx], bh
        cmp    bh, byte ptr [ebx]
        cmp    al, 3dh
        aas
        inc    eax
        inc    ecx
        inc    edx
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
        inc    edi
        dec    eax
        dec    ecx
        dec    edx
        dec    ebx
        dec    esp
        dec    ebp
        dec    esi
        dec    edi
        push   eax
        push   ecx
        push   edx
        push   ebx
        push   esp
        push   ebp
        push   esi
        push   edi
        pop    eax
        pop    ecx
        pop    edx
        pop    ebx
        pop    esp
        pop    ebp
        pop    esi
        pop    edi
        pushad
        popad
        bound  esp, qword ptr [ebx + 64h]
        push   6a69h
        imul   ebp, dword ptr [ebp + ebp*2 + 6eh], 6fh
        jo     SHORT loc_4_0D57
        db  072h, 073h
        db  074h, 075h
        db  076h, 077h
        js     SHORT loc_4_0D67
        db  07Ah, 07Bh
        db  07Ch, 07Dh
        jle    SHORT loc_4_0D73
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        lodsd
        wait
        pushfd
        pop    edi
        popfd
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        cmpsb
        scasb
        stosb
        pop    edi
        pop    edi
        pop    edi
        clc
        int1
        std
        pop    edi
        pop    edi
        out    5fh, al
        cli
sub_4_0D2C:
        pop    edi
        pop    edi
        cmpsd
        scasd
        lodsb
        stosd
        pop    edi
        test   al, 5fh
        pop    edi
        pop    edi
        pop    edi
        mov    cs, word ptr [edi - 6fa07f6eh]
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        movsd
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        cdq
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        lcall  83a0h, 85e15f5fh
loc_4_0D57:
        pop    edi
        test   byte ptr [esi - 7d75786fh], al
        mov    byte ptr [ecx - 74735e73h], cl
        pop    edi
        movsb
        xchg   ebp, eax
loc_4_0D67:
        mov    byte ptr [0f6945f93h], al
        pop    edi
        xchg   edi, eax
        mov    dword ptr [5f5f8196h], eax
loc_4_0D73:
        cwde
        lahf
        loop   loc_4_0DD6
        pop    edi
        pop    edi
        pop    edi
        db  0E9h, 05Fh, 05Fh, 05Fh, 05Fh
        db  05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 0E4h, 05Fh, 05Fh, 0E8h, 05Fh, 05Fh, 0EAh, 05Fh, 05Fh, 05Fh
        db  05Fh, 05Fh, 05Fh, 05Fh, 0E0h, 05Fh, 05Fh, 0EBh, 0EEh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh
        db  05Fh, 05Fh, 05Fh, 0E3h, 05Fh, 05Fh, 0E5h, 0E7h, 05Fh, 0EDh, 0FCh, 09Eh, 0F9h, 0FBh, 05Fh, 05Fh
        db  05Fh, 0ECh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 0EFh, 0F7h, 0F0h, 05Fh
        db  05Fh, 0F3h, 0F2h, 0FFh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh
        db  05Fh, 0A9h, 0F4h, 0F5h, 0C4h, 05Fh, 0B3h
loc_4_0DD6:
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        ficomp dword ptr [edi + 5fh]
        pop    edi
        mov    edi, 0c05f5f5fh
        pop    edi
        pop    edi
        pop    edi
        fstp   dword ptr [edi + 5fh]
        pop    edi
        ret
        db  05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 0B4h, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 0C2h
        db  05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 0C1h, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 0C5h
        db  0CDh, 0BAh, 0D5h, 0D6h, 0C9h, 0B8h, 0B7h, 0BBh, 0D4h, 0D3h, 0C8h, 0BEh, 0BDh, 0BCh, 0C6h, 0C7h
        db  0CCh, 0B5h, 0B6h, 0B9h, 0D1h, 0D2h, 0CBh, 0CFh, 0D0h, 0CAh, 0D8h, 0D7h, 0CEh, 05Fh, 05Fh, 05Fh
        db  05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh
        db  0DFh, 05Fh, 05Fh, 05Fh, 0DCh, 05Fh, 05Fh, 05Fh, 0DBh, 05Fh, 05Fh, 05Fh, 0DDh, 05Fh, 05Fh, 05Fh
        db  0DEh, 0B0h, 0B1h, 0B2h, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh, 05Fh
        db  0FEh, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 018h, 000h, 000h, 000h
        db  000h, 000h, 001h, 000h, 002h, 000h, 003h, 000h, 004h, 000h, 005h, 000h, 006h, 000h, 007h, 000h
        db  008h, 000h, 009h, 000h, 00Ah, 000h, 00Bh, 000h, 00Ch, 000h, 00Dh, 000h, 00Eh, 000h, 00Fh, 000h
        db  010h, 000h, 011h, 000h, 012h, 000h, 013h, 000h, 014h, 000h, 015h, 000h, 016h, 000h, 017h, 000h
        db  018h, 000h, 019h, 000h, 01Ah, 000h, 01Bh, 000h, 01Ch, 000h, 01Dh, 000h, 01Eh, 000h, 01Fh, 000h
        db  020h, 000h, 021h, 000h, 022h, 000h, 023h, 000h, 024h, 000h
loc_4_0EDA:
        and    eax, 27002600h
        add    byte ptr [eax], ch
        add    byte ptr [ecx], ch
        add    byte ptr [edx], ch
        add    byte ptr [ebx], ch
        add    byte ptr [eax + eax], ch
        sub    eax, 2f002e00h
        add    byte ptr [eax], dh
        add    byte ptr [ecx], dh
        add    byte ptr [edx], dh
        add    byte ptr [ebx], dh
        add    byte ptr [eax + eax], dh
        xor    eax, 37003600h
        add    byte ptr [eax], bh
        add    byte ptr [ecx], bh
        add    byte ptr [edx], bh
        add    byte ptr [ebx], bh
        add    byte ptr [eax + eax], bh
        cmp    eax, 3f003e00h
        add    byte ptr [eax], al
        inc    ecx
        add    byte ptr [edx], al
        inc    ebx
        add    byte ptr [eax + eax + 45h], al
        add    byte ptr [esi], al
        inc    edi
        add    byte ptr [eax], cl
        dec    ecx
        add    byte ptr [edx], cl
        dec    ebx
        add    byte ptr [eax + eax + 4dh], cl
        add    byte ptr [esi], cl
        dec    edi
        add    byte ptr [eax], dl
        push   ecx
        add    byte ptr [edx], dl
        push   ebx
        add    byte ptr [eax + eax + 55h], dl
        add    byte ptr [esi], dl
        push   edi
        add    byte ptr [eax], bl
        pop    ecx
        add    byte ptr [edx], bl
        pop    ebx
        add    byte ptr [eax + eax + 5dh], bl
        add    byte ptr [esi], bl
        pop    edi
        add    byte ptr [eax], ah
        popad
        add    byte ptr [edx], ah
        arpl   word ptr [eax], ax
        add    byte ptr fs:[ebp], ah
        add    byte ptr [edi], ah
        push   6a006900h
        add    byte ptr [ebx], ch
        insb
        add    byte ptr [ebp], ch
        outsb
        add    byte ptr [edi], ch
        jo     SHORT loc_4_0F72
loc_4_0F72:
        jno    SHORT loc_4_0F74
loc_4_0F74:
        jb     SHORT loc_4_0F76
loc_4_0F76:
        jae    SHORT loc_4_0F78
loc_4_0F78:
        je     SHORT loc_4_0F7A
loc_4_0F7A:
        jne    SHORT loc_4_0F7C
loc_4_0F7C:
        jbe    SHORT loc_4_0F7E
loc_4_0F7E:
        ja     SHORT loc_4_0F80
loc_4_0F80:
        js     SHORT loc_4_0F82
loc_4_0F82:
        jns    SHORT loc_4_0F84
loc_4_0F84:
        jp     SHORT loc_4_0F86
loc_4_0F86:
        jnp    SHORT loc_4_0F88
loc_4_0F88:
        jl     SHORT loc_4_0F8A
loc_4_0F8A:
        jge    SHORT loc_4_0F8C
loc_4_0F8C:
        jle    SHORT loc_4_0F8E
loc_4_0F8E:
        jg     SHORT loc_4_0F90
loc_4_0F90:
        add    byte ptr [eax], 81h
        add    byte ptr [edx], bl
        and    byte ptr [edx + 26201e01h], dl
        and    byte ptr [eax], ah
        and    byte ptr [ecx], ah
        and    dh, al
        add    dh, byte ptr [eax]
        and    byte ptr [eax + 1], ah
        cmp    dword ptr [eax], esp
        push   edx
        add    dword ptr [ebp - 70ff7200h], ecx
        add    byte ptr [eax + 19201800h], dl
        and    byte ptr [eax], bl
        sbb    eax, 13202220h
        and    byte ptr [eax], dl
        fadd   qword ptr [edx]
        and    ah, byte ptr [ecx]
        popad
        add    dword ptr [edx], edi
        and    byte ptr [ebx + 1], dl
        popfd
        add    byte ptr [esi - 5ffe8800h], bl
        add    byte ptr [ecx - 5cff5e00h], ah
        add    byte ptr [eax + eax + 0a600a5h], ah
        cmpsd
        add    byte ptr [eax - 55ff5700h], ch
        add    byte ptr [ebx - 52ff5400h], ch
        add    byte ptr [esi - 4fff5100h], ch
        add    byte ptr [ecx - 4cff4e00h], dh
        add    byte ptr [eax + eax + 0b600b5h], dh
        mov    bh, 0
        mov    eax, 0ba00b900h
        add    byte ptr [ebx - 42ff4400h], bh
        add    byte ptr [esi - 3fff4100h], bh
        add    cl, al
        add    dl, al
        add    bl, al
        add    ah, al
        add    ch, al
        add    dh, al
        add    bh, al
        add    al, cl
        add    cl, cl
        add    dl, cl
        add    bl, cl
        add    ah, cl
        add    ch, cl
        add    dh, cl
        add    bh, cl
        add    al, dl
        add    cl, dl
        add    dl, dl
        add    bl, dl
        add    ah, dl
        add    ch, dl
        add    dh, dl
        add    bh, dl
        add    al, bl
        add    cl, bl
        add    dl, bl
        add    bl, bl
        add    ah, bl
        add    ch, bl
        add    dh, bl
        add    bh, bl
        add    al, ah
        add    cl, ah
        add    dl, ah
        add    bl, ah
        add    ah, ah
        add    ch, ah
        add    dh, ah
        add    bh, ah
        add    al, ch
        add    cl, ch
        add    dl, ch
        add    bl, ch
        add    ah, ch
        add    ch, ch
        add    dh, ch
        add    bh, ch
        add    al, dh
        add    cl, dh
        add    dl, dh
        add    bl, dh
        add    ah, dh
        add    ch, dh
        add    dh, dh
        add    bh, dh
        add    al, bh
        add    cl, bh
        add    dl, bh
        add    bl, bh
        add    ah, bh
        add    ch, bh
        add    dh, bh
        add    bh, bh
        add    byte ptr [eax], al
        add    byte ptr [eax], dh
        add    byte ptr [eax], al
        add    bh, bh
        add    cl, bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx + 1], dl
        push   ebx
        add    ecx, ebx
        add    al, byte ptr [eax]
        add    byte ptr [eax + eax], cl
        and    al, 0
        pushad
        add    dword ptr [ecx + 1], esp
        fld    qword ptr [edx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  078h, 001h
        db  078h, 001h
        loope  loc_4_10C0
        add    byte ptr [eax], al
loc_4_10C0:
        sbb    byte ptr [eax], al
        dec    eax
        add    byte ptr [edx - 1cfe6dffh], dl
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dh, al
        add    al, dh
        add    ah, ch
        add    al, byte ptr [eax]
        add    byte ptr [eax + eax], bh
        push   esp
        add    ah, bl
        add    bl, ah
        add    ah, bh
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        adc    esp, dword ptr [eax]
        cmp    ah, byte ptr [eax]
        jmp    NEAR PTR loc_4_10F3
        db  000h, 000h
loc_4_10F3:
        add    byte ptr [edx], ah
        and    dword ptr [edx], esp
        and    dword ptr [ecx], edi
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    dword ptr [edx], eax
        add    eax, dword ptr [eax + 9080706h]
        or     cl, byte ptr [ebx]
        or     al, 0dh
        push   cs
        movups xmm2, xmmword ptr [ecx]
        adc    dl, byte ptr [ebx]
        adc    al, 15h
        push   ss
        pop    ss
        sbb    byte ptr [ecx], bl
        sbb    bl, byte ptr [ebx]
        sbb    al, 1dh
        push   ds
        pop    ds
        and    byte ptr [ecx], ah
        and    ah, byte ptr [ebx]
        and    al, 25h
        daa
        sub    byte ptr [ecx], ch
        sub    ch, byte ptr [ebx]
        sub    al, 2dh
        das
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
        cmp    byte ptr [ecx], bh
        cmp    bh, byte ptr [ebx]
        cmp    al, 3dh
        aas
        inc    eax
        inc    ecx
        inc    edx
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
        inc    edi
        dec    eax
        dec    ecx
        dec    edx
        dec    ebx
loc_4_1148:
        dec    esp
        dec    ebp
        dec    esi
        dec    edi
        push   eax
        push   ecx
        push   edx
        push   ebx
        push   esp
        push   ebp
        push   esi
        push   edi
        pop    eax
        pop    ecx
        pop    edx
        pop    ebx
        pop    esp
        pop    ebp
        pop    esi
        pop    edi
        pushad
        popad
        bound  esp, qword ptr [ebx + 64h]
        push   6a69h
        imul   ebp, dword ptr [ebp + ebp*2 + 6eh], 6fh
        jo     SHORT loc_4_11DF
        jb     SHORT loc_4_11E3
        je     SHORT loc_4_11E7
        jbe    SHORT loc_4_11EB
loc_4_1174:
        js     SHORT loc_4_11EF
        jp     SHORT loc_4_11F3
        jl     SHORT loc_4_11F7
        jle    SHORT loc_4_11FB
        add    byte ptr [ecx + 5f5f5f5fh], 5fh
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        lea    ecx, [esi + 5f5f908fh]
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
loc_4_1198:
        pop    edi
        popfd
        sahf
        pop    edi
        mov    al, byte ptr [0a4a3a2a1h]
        movsd
        cmpsb
        cmpsd
        test   al, 0a9h
        stosb
        stosd
        lodsb
        lodsd
        scasb
        scasd
        mov    al, 0b1h
        mov    dl, 0b3h
        mov    ah, 0b5h
        mov    dh, 0b7h
        mov    eax, 0bcbbbab9h
        mov    ebp, 0c1c0bfbeh
        ret    0c4c3h
        db  0C5h, 0C6h, 0C7h, 0C8h
loc_4_11C5:
        leave
        retf   0cccbh
        db  0CDh, 0CEh, 0CFh, 0D0h, 0D1h, 0D2h, 0D3h, 0D4h, 0D5h, 0D6h, 0D7h, 0D8h
loc_4_11D5:
        fstpnce st(2), st(0)
        fcmovnu st(0), st(4)
        fstp   st(6)
        fnstsw ax
        db  0E1h, 0E2h
loc_4_11DF:
        jecxz  loc_4_11C5
        in     eax, 0e6h
loc_4_11E3:
        out    0e8h, eax
        db  0E9h, 0EAh
loc_4_11E7:
        jmp    SHORT loc_4_11D5
        db  0EDh, 0EEh
loc_4_11EB:
        out    dx, eax
        db  0F0h, 0F1h, 0F2h
loc_4_11EF:
        hlt
        cmc
        db  0F6h
loc_4_11F3:
        idiv   eax
        stc
        cli
loc_4_11F7:
        sti
        cld
        std
        db  0FEh
loc_4_11FB:
        dec    dword ptr [esp + ebx*4 - 7c606576h]
        mov    byte ptr [eax + 5f5f9796h], bl
        pop    edi
        xchg   ecx, eax
        xchg   edx, eax
        sbb    byte ptr [edi - 6dh], 94h
        test   byte ptr [edi - 7ah], bl
        xchg   dword ptr [ebp - 7aa0a0a1h], edx
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        mov    dword ptr [edi + 5fh], ebx
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        pop    edi
        mov    ebx, dword ptr [ebx + 99h]
        add    byte ptr [edx], al
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    ecx, dword ptr [eax]
        add    cl, byte ptr [eax]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        add    al, 7
        pop    es
        or     byte ptr [esi], al
        add    dword ptr [eax], ecx
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [edx], al
        pop    es
        add    al, byte ptr [edi]
        add    al, byte ptr [8080808h]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [edi], al
        add    dword ptr [edi], eax
        or     byte ptr [eax], cl
        or     byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], cl
        add    cl, byte ptr [eax]
        or     byte ptr [eax], cl
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
loc_4_12CC:
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
loc_4_131A:
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], cl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax + ecx + 0ah], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [eax + 8], cl
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     byte ptr [edx], cl
        or     byte ptr [eax], cl
        or     byte ptr [edx], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     byte ptr [edx], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax]
        or     byte ptr [edx], cl
        or     byte ptr [eax], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax]
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax + ecx]
        or     cl, byte ptr [eax + ecx]
        or     cl, byte ptr [eax + ecx]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     ah, byte ptr [eax + ecx]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
loc_4_1455:
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        cmp    byte ptr [edx], dh
        or     byte ptr [eax], dh
        xor    byte ptr [eax], cl
        sub    al, 8
        or     byte ptr cs:[eax], cl
        or     byte ptr [eax], cl
        xor    byte ptr [eax], cl
        or     byte ptr [eax + ecx], dh
        or     byte ptr [eax], cl
        or     byte ptr [esi], dh
        cmp    cl, byte ptr [eax]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        cmp    cl, byte ptr [eax]
        or     byte ptr [eax + ecx], bh
        or     byte ptr [esi], bh
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [edx + 8], al
        or     byte ptr [eax], cl
        or     byte ptr [edx + 8], al
        inc    eax
        inc    eax
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
loc_4_14C2:
        inc    esp
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
loc_4_14F5:
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
loc_4_14FD:
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [edx + edx], dl
        adc    dl, byte ptr [edx]
        or     byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        push   cs
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [edx], ah
        and    byte ptr [eax], ah
        or     byte ptr [esi], bl
        sbb    al, 8
        or     byte ptr [eax], cl
        push   ss
        sbb    cl, byte ptr [eax]
        or     byte ptr [eax], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        sub    byte ptr [esi], ah
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
loc_4_1674:
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        or     byte ptr [esi], ah
        db  026h, 026h, 026h, 026h, 026h, 026h, 026h, 026h, 026h, 026h, 026h, 026h, 026h, 026h, 008h, 00Ah
        db  008h, 00Ah, 008h, 00Ah, 008h, 00Ah, 008h, 00Ah, 008h, 00Ah, 008h, 00Ah, 008h, 00Ah, 008h, 00Ah
        db  008h, 00Ah, 008h, 00Ah, 008h, 00Ah, 008h, 00Ah, 008h, 00Ah, 008h, 00Ah, 008h, 00Ah, 008h, 00Ah
        db  008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 00Ah
        db  008h, 00Ah, 008h, 00Ah, 008h, 00Ah, 008h, 00Ah, 008h, 00Ah, 008h, 00Ah, 008h, 00Ah, 008h
loc_4_16D1:
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        db  008h
loc_4_16FC:
        or     cl, byte ptr [eax]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
loc_4_170C:
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
loc_4_171E:
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], al
        add    byte ptr [edx], al
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    ecx, dword ptr [eax]
        add    cl, byte ptr [eax]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        add    al, 7
        pop    es
        or     byte ptr [esi], al
        add    dword ptr [eax], ecx
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [edx], al
        pop    es
        add    al, byte ptr [edi]
        add    al, byte ptr [8080808h]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [edi], al
        add    dword ptr [edi], eax
        or     byte ptr [eax], cl
        or     byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], cl
        add    cl, byte ptr [eax]
        or     byte ptr [eax], cl
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
loc_4_1840:
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
loc_4_1870:
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
loc_4_1884:
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], cl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax + ecx + 0ah], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [eax + 8], cl
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     byte ptr [edx], cl
        or     byte ptr [eax], cl
        or     byte ptr [edx], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     byte ptr [edx], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax]
        or     byte ptr [edx], cl
        or     byte ptr [eax], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax]
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax + ecx]
        or     cl, byte ptr [eax + ecx]
        or     cl, byte ptr [eax + ecx]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     ah, byte ptr [eax + ecx]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
loc_4_19FB:
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        cmp    byte ptr [edx], dh
        or     byte ptr [eax], dh
        xor    byte ptr [eax], cl
        sub    al, 8
        or     byte ptr cs:[eax], cl
        or     byte ptr [eax], cl
        xor    byte ptr [eax], cl
        or     byte ptr [eax + ecx], dh
        or     byte ptr [eax], cl
        or     byte ptr [esi], dh
        cmp    cl, byte ptr [eax]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        cmp    cl, byte ptr [eax]
        or     byte ptr [eax + ecx], bh
        or     byte ptr [esi], bh
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [edx + 8], al
        or     byte ptr [eax], cl
        or     byte ptr [edx + 8], al
        inc    eax
        inc    eax
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        inc    esp
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        db  008h
loc_4_1A8E:
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
loc_4_1A9E:
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
loc_4_1AB2:
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
loc_4_1AB8:
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        adc    al, 14h
        adc    al, 14h
        or     byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        and    ah, byte ptr [eax]
        and    byte ptr [eax], cl
        push   ds
        sbb    al, 8
        or     byte ptr [eax], cl
        push   ss
        sbb    cl, byte ptr [eax]
        or     byte ptr [eax], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
loc_4_1BA2:
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        sub    byte ptr [esi], ah
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        or     byte ptr [esi], ah
        db  026h, 026h, 026h, 026h, 026h, 026h, 026h, 026h, 026h, 026h, 026h, 026h, 026h, 026h, 008h, 00Ah
        db  008h, 00Ah, 008h, 00Ah, 008h
loc_4_1C1F:
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
loc_4_1C43:
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     cl, byte ptr [eax]
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [edx], cl
        or     byte ptr [eax], cl
        db  008h
loc_4_1C88:
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        db  008h
sub_4_1C8F:
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        sbb    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + 46h], cl
        push   ebx
        dec    ebp
        inc    edi
        push   edx
        add    byte ptr [eax], al
        push   eax
        push   edx
        dec    ecx
        dec    esi
        push   esp
        inc    edx
        push   ebp
        inc    esi
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esp
        db  02Dh, 000h, 000h, 04Ch, 02Dh
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
IFSMgr_ServerDOSCall:
        db  0FFh, 0FFh, 000h, 000h, 02Dh, 000h, 000h, 000h, 0DCh, 06Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_4_1E10:
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
loc_4_1EA0:
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
loc_4_1ED2:
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
loc_4_1F14:
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
loc_4_1F2C:
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
loc_4_1F68:
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
loc_4_1FDC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1FE0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_4_1FEB:
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
        add    byte ptr [ebp - 75h], dl
        in     al, dx
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 034h, 085h, 014h, 059h, 000h, 000h
        db  00Fh, 0B6h, 088h, 02Ch, 059h, 000h, 000h
        db  00Fh, 0B6h, 098h
IFSMgr_SetReqHook:
        and    al, 59h
        add    byte ptr [eax], al
        sub    edx, edx
        dec    edx
        cmp    dword ptr [ebp + 8], 0
        jne    SHORT loc_4_202C
        add    esi, ebx
        btr    edx, 0
loc_4_202C:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_4_203D
        sub    eax, dword ptr [ebp + 0ch]
        btr    edx, eax
        add    esi, ebx
        loop   loc_4_202C
loc_4_203D:
        bsf    eax, edx
        add    eax, dword ptr [ebp + 0ch]
        pop    esi
        pop    ebx
        leave
        ret
sub_4_2047:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 3
        ja     SHORT loc_4_2087
        db  00Fh, 0B6h, 088h, 028h, 059h, 000h, 000h
        db  00Fh, 0B6h, 090h, 024h, 059h, 000h, 000h
        db  08Bh, 034h, 085h, 014h, 059h, 000h, 000h
        cmp    eax, 0
        jne    SHORT loc_4_2079
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_4_2078
IFSMgr_SetPathHook:
        sub    eax, eax
        pop    esi
        leave
        ret
loc_4_2078:
        inc    ecx
loc_4_2079:
        mov    eax, dword ptr [ebp + 0ch]
        push   ecx
loc_4_207D:
        cmp    dword ptr [esi + 4], eax
        je     SHORT loc_4_208F
        add    esi, edx
        loop   loc_4_207D
        pop    eax
loc_4_2087:
        mov    eax, 0ffffffffh
        pop    esi
        leave
        ret
loc_4_208F:
        pop    eax
        sub    eax, ecx
        pop    esi
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   edi
        db  08Dh, 03Dh, 094h, 056h, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 029h, 059h, 000h, 000h
        jecxz  loc_4_20BD
        mov    eax, dword ptr [ebp + 8]
loc_4_20AB:
        cmp    dword ptr [edi + 10h], eax
        je     SHORT loc_4_20B7
        add    edi, 14h
        loop   loc_4_20AB
        jmp    SHORT loc_4_20BD
loc_4_20B7:
        mov    eax, dword ptr [edi + 4]
        pop    edi
        leave
        ret
loc_4_20BD:
        sub    eax, eax
        pop    edi
        leave
        ret
sub_4_20C2:
        push   ebp
        mov    ebp, esp
        push   ebx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
sub_4_20D4:
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp + 0ch]
        shr    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        db  00Fh, 0B7h, 015h, 03Ch, 059h, 000h, 000h
        db  066h, 08Bh, 00Dh, 03Eh, 059h, 000h, 000h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    ax, word ptr [ebx + 14h]
        shl    eax, 10h
        mov    ax, word ptr [ebx + 1ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        cld
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebx
        leave
        ret
        db  08Bh, 0C0h
sub_4_2154:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        leave
        ret
        db  08Bh, 0C0h
sub_4_2164:
        db  0CDh, 020h, 063h, 080h, 001h, 000h
        mov    eax, eax
sub_4_216C:
        db  0CDh, 020h, 064h, 080h, 001h, 000h
        mov    eax, eax
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        lea    ecx, [eax - 1]
        mov    edi, edx
        repne scasb
        not    ecx
        dec    ecx
        je     SHORT loc_4_219E
        mov    ebx, ecx
        mov    eax, 2a3fh
loc_4_2191:
        mov    edi, edx
        mov    ecx, ebx
        repne scasb
        je     SHORT loc_4_219E
        shr    eax, 8
        jne    SHORT loc_4_2191
loc_4_219E:
        pop    edi
        pop    ebx
        leave
        ret
sub_4_21A2:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     SHORT loc_4_21D7
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 3ch]
        mov    word ptr [ebx + 4], ax
        shr    eax, 10h
        mov    word ptr [ebx + 3ch], ax
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        pop    dword ptr [ebx + 3ch]
        pop    dword ptr [ebx + 4]
loc_4_21D7:
        pop    ebx
        leave
        ret
sub_4_21DA:
        push   ebp
        mov    ebp, esp
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        push   0
        push   dword ptr [ebp + 8]
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_4_21FB
        add    eax, dword ptr [ebp + 0ch]
loc_4_21FB:
        pop    ebx
        leave
        ret
sub_4_21FE:
        push   ebp
        mov    ebp, esp
        push   ebx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        mov    word ptr [ebx + 1ch], 1218h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [eax + 4], cx
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 90h]
        db  08Bh, 015h, 054h, 059h, 000h, 000h
        add    edx, dword ptr [eax + 4]
        movzx  eax, word ptr [edx + 1ch]
        movzx  edx, word ptr [edx + 1ah]
        shl    eax, 4
        add    eax, edx
        movzx  ecx, word ptr [eax + 4]
        lea    edx, [eax + 6]
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0fffffh
        sub    eax, edx
        sub    edx, edx
        db  0F7h, 035h, 0CCh, 066h, 000h, 000h
        test   edx, edx
        jne    SHORT loc_4_22A3
        cmp    eax, ecx
        jae    SHORT loc_4_22A3
        add    ax, 100h
        leave
        ret
loc_4_22A3:
        mov    eax, 0ffh
        leave
        ret
sub_4_22AA:
        push   ebp
        mov    ebp, esp
        add    esp, -4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 035h, 054h, 059h, 000h, 000h
        add    esi, dword ptr [ebx + 4]
        lea    edx, [esi + 4]
        mov    dword ptr [ebp - 4], 0
        sub    eax, eax
loc_4_22CB:
        db  08Bh, 03Dh, 0B4h, 055h, 000h, 000h
        add    edi, ebx
        mov    edi, dword ptr [edi + 4ch]
        add    edi, eax
loc_4_22D8:
        movzx  eax, word ptr [edx]
        movzx  edx, word ptr [edx + 2]
        shl    edx, 4
        add    edx, eax
        add    edx, dword ptr [ebx + 4]
        lea    eax, [edx + 6]
        movzx  ecx, word ptr [edx + 4]
loc_4_22EE:
        stosd
        db  003h, 005h, 0CCh, 066h, 000h, 000h
        inc    dword ptr [ebp - 4]
        loop   loc_4_22EE
        cmp    word ptr [edx], -1
        jne    SHORT loc_4_22D8
        test   esi, esi
        je     SHORT loc_4_2310
        lea    edx, [esi + 1ah]
        sub    esi, esi
        mov    eax, 400h
        jmp    SHORT loc_4_22CB
loc_4_2310:
        mov    eax, dword ptr [ebp - 4]
        db  02Bh, 005h, 0D4h, 066h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_231E:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        test   dword ptr [ebp + 14h], 2
        jne    SHORT loc_4_2344
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        jmp    SHORT loc_4_234A
loc_4_2344:
        db  0CDh, 020h, 083h, 000h, 001h, 000h
loc_4_234A:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        push   dword ptr [ebx + 30h]
        mov    ax, word ptr [ebp + 12h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 10h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 0eh]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 0ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    cx, word ptr [ebp + 0ah]
        movzx  edx, word ptr [ebp + 8]
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        pop    dword ptr [ebx + 30h]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
        db  0CCh, 0B8h, 003h, 000h, 000h, 000h, 0C3h
sub_4_23B6:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    ecx, 100h
        sub    eax, eax
        db  0BFh, 0B0h, 083h, 000h, 000h
        repne scasd
        jne    SHORT loc_4_23E1
        lea    ecx, [edi - 4]
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ecx], edi
        db  081h, 0E9h, 0B0h, 083h, 000h, 000h
        shr    ecx, 2
        mov    byte ptr [edi + 3], cl
        pop    edi
        leave
        ret
loc_4_23E1:
        dec    eax
IFSMgr_MountChildVolume:
        pop    edi
        leave
        ret
sub_4_23E5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        or     edx, edx
        jne    SHORT loc_4_2404
        mov    ecx, 40h
        db  0BFh, 014h, 058h, 000h, 000h
        repne scasd
        je     SHORT loc_4_242C
        jmp    SHORT loc_4_243E
loc_4_2404:
        mov    ecx, 40h
        db  0BFh, 014h, 058h, 000h, 000h
        xor    eax, eax
        dec    eax
        repne scasd
        jne    SHORT loc_4_243E
        sub    edi, 4
        sub    ecx, 40h
        not    ecx
        shr    ecx, 1
        mov    byte ptr [edx + 3], cl
        mov    dword ptr [edi + 4], edx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edi], ecx
        jmp    SHORT loc_4_2438
loc_4_242C:
        sub    ecx, 40h
        not    ecx
        shr    ecx, 1
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [edx], ecx
loc_4_2438:
        xor    eax, eax
        pop    edi
        pop    esi
        leave
        ret
loc_4_243E:
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        leave
        ret
        db  0CDh, 020h, 083h, 000h, 001h, 000h, 0CDh, 020h, 01Ah, 000h, 001h, 000h, 0CDh, 020h, 085h, 000h
IFSMgr_UnmountChildVolume:
        add    dword ptr [eax], eax
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        cld
        ret
sub_4_2461:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        db  0BEh, 0B3h, 004h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        db  08Bh, 035h, 0B4h, 055h, 000h, 000h
        mov    dword ptr [edx + esi + 20h], eax
        and    byte ptr [edx + esi + 8], 7fh
        pop    esi
        leave
        ret
sub_4_248A:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        db  003h, 005h, 0B4h, 055h, 000h, 000h
        xor    esi, esi
        xchg   dword ptr [eax + 20h], esi
        test   byte ptr [eax + 8], 80h
        je     SHORT loc_4_24AA
        db  0CDh, 020h, 015h, 000h, 001h, 000h
        jmp    SHORT loc_4_24B0
loc_4_24AA:
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_4_24B0:
        pop    esi
        leave
        ret
        db  08Bh, 0DAh, 08Bh, 03Dh, 0B4h, 055h, 000h, 000h, 0C7h, 044h, 03Bh, 020h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 001h, 000h, 072h, 023h, 0BEh, 0EFh, 004h, 000h, 000h, 0BAh, 09Ah, 03Ah
        db  000h, 000h, 0B8h, 010h, 000h, 000h, 000h, 0B9h, 003h, 000h, 000h, 000h, 0CDh, 020h, 014h, 000h
        db  001h, 000h, 089h, 074h, 03Bh, 020h, 080h, 04Ch, 03Bh, 008h, 080h, 0C3h, 08Bh, 03Dh, 0B4h, 055h
        db  000h, 000h, 083h, 07Ch, 03Bh, 010h, 000h, 075h, 02Bh, 083h, 0ECh, 06Ch, 057h, 08Dh, 07Ch, 024h
        db  004h, 0CDh, 020h, 08Dh, 000h, 001h, 000h, 05Fh, 0B9h, 0D3h, 000h, 000h, 000h, 02Bh, 0F6h, 0E8h
        db  06Dh, 010h, 0FBh, 0FFh, 056h, 08Dh, 074h, 024h, 004h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 05Eh
        db  083h, 0C4h, 06Ch, 0C3h, 053h, 068h, 000h, 010h, 000h, 000h, 0E8h, 02Fh, 0FFh, 0FFh, 0FFh, 083h
        db  0C4h, 008h, 0C3h
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
IFSMgr_SwapDrives:
        mov    ebx, dword ptr [ebp + 8]
        sub    eax, eax
        mov    dword ptr [ebx + 24h], eax
        pop    ebx
        pop    edi
        leave
        ret
sub_4_2547:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   dword ptr [edi + 90h]
        push   dword ptr [edi + 94h]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    esi, ebx
loc_4_2564:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        test   dword ptr [ebx], 10h
        jne    SHORT loc_4_258E
        mov    dword ptr [edi + 90h], ebx
        db  0A1h, 0B4h, 055h, 000h, 000h
        add    eax, ebx
        mov    dword ptr [edi + 94h], eax
        push   edi
        call   dword ptr [ebp + 0ch]
        pop    ecx
        test   eax, eax
        jne    SHORT loc_4_25A5
loc_4_258E:
        cmp    esi, ebx
        jne    SHORT loc_4_2564
        pop    dword ptr [edi + 94h]
        pop    dword ptr [edi + 90h]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_4_25A5:
        pop    dword ptr [edi + 94h]
        pop    dword ptr [edi + 90h]
        db  08Bh, 015h, 0B4h, 055h, 000h, 000h
        add    edx, esi
        mov    dword ptr [edi + 94h], edx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_25C4:
        push   ebp
        mov    ebp, esp
        add    esp, -0ch
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [esi + 45h]
        db  08Bh, 00Dh, 044h, 059h, 000h, 000h
        add    ecx, dword ptr [ebx + 4]
        movzx  edx, word ptr [ecx + 0ch]
        movzx  ecx, word ptr [ecx + 0eh]
        shl    ecx, 4
        add    edx, ecx
        add    edx, dword ptr [ebx + 4]
        lea    edx, [edx + 4]
        mov    dword ptr [ebp - 4], 0
        sub    edi, edi
loc_4_25FE:
        movzx  eax, word ptr [edx]
        movzx  edx, word ptr [edx + 2]
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 0ch], eax
        add    dword ptr [ebp - 0ch], 6
        shl    edx, 4
        add    edx, eax
        add    edx, dword ptr [ebx + 4]
        lea    eax, [edx + 6]
        movzx  ecx, word ptr [edx + 4]
loc_4_261E:
        cmp    word ptr [eax], 0
        je     SHORT loc_4_2653
        cmp    dword ptr [eax + 7], esi
        jne    SHORT loc_4_2653
        test   word ptr [eax + 5], 8080h
        jne    SHORT loc_4_2653
        inc    dword ptr [ebp - 4]
        push   eax
        push   ecx
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 78h], eax
        mov    word ptr [edx + 6], di
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_4_4714
        add    esp, 0ch
        pop    edx
        pop    ecx
        pop    eax
loc_4_2653:
        db  003h, 005h, 0CCh, 066h, 000h, 000h
        push   eax
        db  0A1h, 0CCh, 066h, 000h, 000h
        add    dword ptr [ebp - 0ch], eax
        pop    eax
        inc    edi
        loop   loc_4_261E
        cmp    word ptr [edx], -1
        jne    SHORT loc_4_25FE
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_2674:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 38h], cx
        mov    word ptr [ebp], ax
        mov    eax, 1230h
        db  066h
sub_4_26B0:
        mov    dword ptr [ebp + 1ch], eax
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        test   word ptr [ebp + 2ch], 1
        jne    SHORT loc_4_26EB
        mov    cx, word ptr [ebp + 18h]
        mov    word ptr [edx], cx
        mov    cx, word ptr [ebp + 4]
        mov    word ptr [edx + 2], cx
        mov    cx, word ptr [ebp + 14h]
        mov    word ptr [edx + 4], cx
        xor    eax, eax
loc_4_26EB:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebp
        or     eax, eax
        jne    SHORT loc_4_2714
        mov    esi, dword ptr [ebp + 8]
        lea    esi, [esi + 20h]
        lea    edi, [edx + 6]
        mov    cx, 0bh
        rep movsb
loc_4_2714:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_2719:
        push   ebp
        mov    ebp, esp
        push   ebx
        db  08Bh, 00Dh, 0B8h, 061h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        db  08Bh, 01Dh, 054h, 059h, 000h, 000h
        mov    ebx, dword ptr [ebx]
loc_4_272E:
        cmp    byte ptr [ecx], dl
        je     SHORT loc_4_274C
        mov    ecx, dword ptr [ecx + 19h]
        cmp    cx, -1
        je     SHORT loc_4_274A
        mov    ebx, ecx
        movzx  eax, cx
        shr    ecx, 10h
        shl    ecx, 4
        add    ecx, eax
        jmp    SHORT loc_4_272E
loc_4_274A:
        sub    ebx, ebx
loc_4_274C:
        mov    eax, ebx
        pop    ebx
        leave
        ret
sub_4_2751:
        push   edi
        push   ebx
        mov    edi, dword ptr [esp + 0ch]
        mov    ecx, 41h
        sub    eax, eax
        rep stosd
        mov    edi, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    dword ptr [edi + 90h], ebx
        mov    eax, dword ptr [ebx + 0ch]
        db  003h, 01Dh, 0B4h, 055h, 000h, 000h
        mov    dword ptr [edi + 94h], ebx
        db  08Bh, 01Dh, 09Ch, 065h, 000h, 000h
        movzx  ebx, word ptr [ebx + 10h]
        mov    ecx, ebx
        shl    ecx, 4
        mov    dword ptr [edi + 80h], ecx
        add    ebx, eax
        shl    eax, 10h
        or     ebx, eax
        mov    dword ptr [edi + 8], ebx
        mov    eax, 0fffffbbbh
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [edi + 8ch], eax
        mov    dword ptr [edi + 2ch], eax
        pop    ebx
        pop    edi
        ret
sub_4_27B2:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        movzx  eax, word ptr [edx + 6]
        sub    eax, 200h
        jb     SHORT loc_4_2806
        mov    ebx, eax
        shr    ebx, 8
        db  08Bh, 01Ch, 09Dh, 0E0h, 066h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_4_2806
        xor    ah, ah
        lea    ebx, [ebx + eax*8]
        mov    eax, dword ptr [ebx]
        or     eax, eax
        je     SHORT loc_4_2806
        test   byte ptr [edx + 8bh], 80h
        jne    SHORT loc_4_27FE
        mov    ecx, dword ptr [edx + 80h]
        db  03Bh, 00Dh, 0B8h, 055h, 000h, 000h
        je     SHORT loc_4_27FE
        mov    ecx, dword ptr [edx + 8]
        cmp    ecx, dword ptr [ebx + 4]
        jne    SHORT loc_4_2806
loc_4_27FE:
        mov    dword ptr [edx + 74h], eax
        xor    eax, eax
        pop    ebx
        leave
        ret
loc_4_2806:
        mov    eax, 6
        mov    word ptr [edx + 1ah], ax
        pop    ebx
        leave
        ret
        db  08Bh, 00Dh, 0B0h, 055h, 000h, 000h, 0F6h, 005h, 00Ch, 06Bh, 000h, 000h, 001h, 075h, 041h, 0F6h
        db  005h, 00Ch, 06Bh, 000h, 000h, 002h, 074h, 00Ch, 003h, 00Dh, 0B4h, 055h, 000h, 000h, 0F6h, 041h
        db  008h, 010h, 075h, 02Ch, 00Fh, 0B7h, 0C3h, 02Dh, 000h, 002h, 000h, 000h, 072h, 022h, 08Bh, 0C8h
        db  0C1h, 0E9h, 008h, 08Bh, 00Ch, 08Dh, 0E0h, 066h, 000h, 000h, 00Bh, 0C9h, 074h, 012h, 032h, 0E4h
        db  08Dh, 00Ch, 0C1h, 08Bh, 001h, 00Bh, 0C0h, 074h, 007h, 08Bh, 050h, 018h, 08Bh, 0D8h, 0F8h, 0C3h
        db  0F9h, 0C3h
sub_4_2864:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  edx, word ptr [ebp + 0ch]
        mov    ebx, dword ptr [esi + 90h]
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 4500h
        mov    word ptr [ebp + 10h], dx
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    eax, 0ffffffffh
        test   word ptr [ebp + 2ch], 1
        jne    SHORT loc_4_28B6
        movzx  eax, word ptr [ebp + 1ch]
loc_4_28B6:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_28D1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    edi, dword ptr [edi + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [edi + 1dh], 51h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    bx, word ptr [edi + 10h]
        push   ebx
        mov    ebx, dword ptr [ebp + 14h]
        mov    word ptr [edi + 10h], bx
        mov    byte ptr [edi + 1dh], 50h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 1ch]
        mov    esi, dword ptr [ebp + 18h]
        mov    dl, byte ptr [eax + esi]
        mov    ecx, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dh, byte ptr [ebx + ecx]
        mov    byte ptr [eax + esi], dh
        mov    ebx, eax
        mov    word ptr [edi + 10h], ax
        mov    byte ptr [edi + 1dh], 45h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   dword ptr [edi + 2ch], 1
        movzx  eax, word ptr [edi + 1ch]
        jne    SHORT loc_4_295A
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ecx], eax
        xor    eax, eax
loc_4_295A:
        mov    byte ptr [ebx + esi], dl
        pop    ebx
        push   eax
        mov    byte ptr [edi + 1dh], 50h
        mov    word ptr [edi + 10h], bx
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        pop    eax
        db  0CDh, 020h, 086h, 000h, 001h
sub_4_2978:
        add    byte ptr [esi - 73h], dl
        db  074h, 024h
        add    al, 0cdh
        and    byte ptr [esi + 5e000100h], cl
        add    esp, 6ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_298D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp + 0ch]
        push   ebp
        mov    ebx, dword ptr [esi + 90h]
        mov    ebp, dword ptr [ebx + 8]
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0A1h, 09Ch, 065h, 000h, 000h
        mov    edx, dword ptr [eax + 0ch]
        db  0A1h, 0C4h, 061h, 000h, 000h
        shr    eax, 4
        mov    word ptr [ebp + 3ch], ax
        mov    word ptr [ebp + 14h], 104h
        mov    byte ptr [ebp + 1dh], 1ah
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    word ptr [ebp + 14h], 0
        mov    byte ptr [ebp + 1dh], cl
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        xor    ebx, ebx
        test   dword ptr [ebp + 2ch], 1
        je     SHORT loc_4_29F7
        mov    ebx, dword ptr [ebp + 1ch]
        mov    word ptr [esi + 1ah], bx
loc_4_29F7:
        mov    word ptr [ebp + 14h], dx
        shr    edx, 10h
        mov    word ptr [ebp + 3ch], dx
        mov    byte ptr [ebp + 1dh], 1ah
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, bx
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        pop    ebp
        pop    esi
        pop    ebx
        leave
        ret
sub_4_2A1F:
        push   ebp
        mov    ebp, esp
        xor    ecx, ecx
        movzx  eax, byte ptr [ebp + 8]
        db  03Bh, 005h, 044h, 066h, 000h, 000h
        ja     SHORT loc_4_2A3F
        dec    eax
        db  08Bh, 004h, 085h, 048h, 066h, 000h, 000h
        test   byte ptr [eax + 44h], 40h
        je     SHORT loc_4_2A3F
        inc    ecx
loc_4_2A3F:
        mov    eax, ecx
        leave
        ret
sub_4_2A43:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 2ch]
        mov    esi, edi
        xor    eax, eax
        mov    ecx, 104h
        repne scasw
        sub    ecx, 104h
        neg    ecx
        mov    edi, dword ptr [ebx + 14h]
        rep movsw
        mov    esi, dword ptr [ebx + 14h]
        mov    edx, esi
        cmp    word ptr [edx + 2], 3ah
        jne    SHORT loc_4_2A78
        add    edx, 4
loc_4_2A78:
        cmp    word ptr [edx], 5ch
        je     SHORT loc_4_2A84
        cmp    word ptr [edx], 2fh
        jne    SHORT loc_4_2A87
loc_4_2A84:
        add    edx, 2
loc_4_2A87:
        sub    edi, 4
        pushfd
        std
        xchg   edi, esi
loc_4_2A8E:
        lodsw
        cmp    ax, 5ch
        je     SHORT loc_4_2AA8
        cmp    ax, 2fh
        je     SHORT loc_4_2AA8
        cmp    ax, 3ah
        je     SHORT loc_4_2AA8
        cmp    esi, edi
        jb     SHORT loc_4_2AAB
        jmp    SHORT loc_4_2A8E
loc_4_2AA8:
        add    esi, 2
loc_4_2AAB:
        popfd
        add    esi, 2
        mov    ecx, esi
        sub    ecx, edi
        jecxz  loc_4_2AC2
        cmp    edx, esi
        jne    SHORT loc_4_2ABC
        add    esi, 2
loc_4_2ABC:
        mov    word ptr [esi - 2], 0
loc_4_2AC2:
        mov    eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_2AC9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, edi
        xor    eax, eax
        mov    ecx, 104h
        cld
        repne scasb
        mov    ecx, edi
        sub    ecx, esi
        mov    ebx, dword ptr [ebp + 8]
        test   byte ptr [ebx + 8bh], 2
        jne    NEAR PTR loc_4_2B73
        movzx  ebx, byte ptr [ebx + 8bh]
        and    ebx, 3
        push   esi
        call   sub_4_14F01
        pop    esi
        mov    ecx, edi
        sub    ecx, esi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        add    esi, ecx
        mov    dword ptr [ebx + 14h], esi
        jecxz  loc_4_2B73
        cmp    byte ptr [ebx + 88h], 4eh
        jne    SHORT loc_4_2B26
        test   byte ptr [ebx + 8bh], 40h
        jne    SHORT loc_4_2B73
loc_4_2B26:
        lea    esi, [ebx + 98h]
        cmp    byte ptr [ebx + 88h], 6ch
        je     SHORT loc_4_2B48
        cmp    byte ptr [ebx + 88h], 60h
        je     SHORT loc_4_2B48
        movzx  edx, word ptr [esi + 14h]
        add    word ptr [esi + 14h], cx
        jmp    SHORT loc_4_2B56
loc_4_2B48:
        movzx  edx, word ptr [esi + 4]
        or     edx, 80000000h
        add    word ptr [esi + 4], cx
loc_4_2B56:
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        push   esi
        sub    eax, eax
        db  0BEh, 078h, 00Bh, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
        pop    esi
        mov    edi, esi
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
loc_4_2B73:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  085h, 0D2h, 079h, 006h, 066h, 089h, 055h, 004h, 0EBh, 004h, 066h, 089h, 055h, 014h, 0C3h, 044h
        db  000h, 045h, 000h, 056h, 000h, 000h, 000h
sub_4_2B8F:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cld
        cmp    word ptr [esi + 2], 3ah
        jne    SHORT loc_4_2BA2
        add    esi, 4
loc_4_2BA2:
        lodsw
        cmp    ax, 5ch
        je     SHORT loc_4_2BB0
        cmp    ax, 2fh
        jne    SHORT loc_4_2BDA
loc_4_2BB0:
        push   esi
        call   sub_4_14D65
        pop    esi
        shl    eax, 1
        push   eax
        push   esi
        push   esi
        call   sub_4_14A8A
        add    esp, 0ch
        add    eax, 2
        shr    eax, 1
        mov    ecx, 4
        cmp    eax, ecx
        jne    SHORT loc_4_2BDA
        db  0BFh, 087h, 00Bh, 000h, 000h
        repe cmpsw
loc_4_2BDA:
        mov    eax, 0
        jne    SHORT loc_4_2BE3
        not    eax
loc_4_2BE3:
        pop    edi
        pop    esi
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        movzx  edx, byte ptr [ebp + 8]
        mov    eax, 0fh
        cmp    edx, 20h
        jae    SHORT loc_4_2C5E
        mov    edi, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        db  003h, 005h, 0B4h, 055h, 000h, 000h
        mov    esi, dword ptr [eax + edx*4 + 50h]
        or     esi, esi
        je     SHORT loc_4_2C33
        push   1
        push   103h
        movzx  eax, word ptr [esi + 2]
        push   eax
        lea    esi, [esi + 4]
        push   esi
        push   edi
        call   sub_4_14A00
        add    esp, 14h
        mov    byte ptr [eax + edi], 0
        jmp    SHORT loc_4_2C5C
loc_4_2C33:
        mov    eax, 0fh
        db  03Bh, 015h, 044h, 066h, 000h, 000h
        jae    SHORT loc_4_2C5E
        db  08Bh, 034h, 095h, 048h, 066h, 000h, 000h
        movzx  eax, word ptr [esi + 4fh]
        add    esi, eax
        cmp    byte ptr [esi], 0
        je     SHORT loc_4_2C53
        inc    esi
loc_4_2C53:
        add    esi, dword ptr [ebx + 4]
loc_4_2C56:
        lodsb
        stosb
        or     al, al
        jne    SHORT loc_4_2C56
loc_4_2C5C:
        xor    eax, eax
loc_4_2C5E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    edx, ebx
        mov    ecx, dword ptr [ebp + 8]
loc_4_2C72:
        cmp    dword ptr [ebx + 0ch], ecx
        je     SHORT loc_4_2C86
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, edx
        jne    SHORT loc_4_2C72
        xor    eax, eax
        pop    ebx
        leave
        ret
loc_4_2C86:
        mov    eax, dword ptr [ebx + 4]
        pop    ebx
        leave
        ret
sub_4_2C8C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        db  0CDh, 020h, 05Bh, 000h, 001h, 000h
        pop    eax
        cmp    edx, dword ptr [ebp + 8]
        jae    SHORT loc_4_2CD0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        mov    ecx, eax
        or     eax, edx
        je     SHORT loc_4_2CD0
        push   0
        push   ecx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_4_2CD5
loc_4_2CD0:
        mov    eax, 0ffffffffh
loc_4_2CD5:
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
        mov    edi, dword ptr [ebp + 8]
        db  0CDh, 020h, 000h, 000h, 021h, 000h
        jb     SHORT loc_4_2D21
        cmp    eax, 400h
        jb     SHORT loc_4_2D21
        lea    edx, [edi + 98h]
        xor    bh, bh
        mov    word ptr [edx + 1ch], bx
        db  0CDh, 020h, 007h, 000h, 021h, 000h
        lea    edx, [edi + 98h]
        mov    word ptr [edx + 10h], ax
        shr    eax, 10h
        mov    word ptr [edx + 18h], ax
        mov    edi, dword ptr [edi + 14h]
        cld
loc_4_2D19:
        lodsb
        stosb
        or     al, al
        jne    SHORT loc_4_2D19
        jmp    SHORT loc_4_2D27
loc_4_2D21:
        mov    word ptr [edi + 1ah], 5
loc_4_2D27:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_2D2C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        db  08Dh, 035h, 004h, 000h, 000h, 000h
        mov    ebx, 0eh
        cld
loc_4_2D3D:
        lodsd
        mov    ecx, eax
        lodsd
        shr    ecx, 0ch
        shr    eax, 0ch
        inc    eax
        push   0
        push   eax
        push   ecx
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_4_2D5A
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        jmp    SHORT loc_4_2D60
loc_4_2D5A:
        db  0CDh, 020h, 063h, 000h, 001h, 000h
loc_4_2D60:
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_2D72
        sub    ebx, 2
        jne    SHORT loc_4_2D3D
        xor    eax, eax
        pop    esi
        pop    ebx
        leave
        ret
loc_4_2D72:
        cmp    dword ptr [ebp + 8], 0
        jne    SHORT loc_4_2DA4
        mov    ecx, 0eh
        sub    ecx, ebx
        jecxz  loc_4_2DA4
        db  08Dh, 035h, 004h, 000h, 000h, 000h
loc_4_2D87:
        lodsd
        mov    edx, eax
        lodsd
        shr    edx, 0ch
        shr    eax, 0ch
        inc    eax
        push   ecx
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        dec    ecx
        loop   loc_4_2D87
loc_4_2DA4:
        mov    eax, 8
        pop    esi
        pop    ebx
        leave
        ret
sub_4_2DAD:
        push   ebp
        mov    ebp, esp
        push   0
        push   0
        push   92h
        push   19h
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   1
        push   eax
        push   edx
        push   dword ptr [ebp + 0ch]
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        leave
        ret
sub_4_2DD8:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    edi, dword ptr [ebp + 10h]
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        pop    edi
        pop    ebx
        leave
        ret
sub_4_2E09:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 90h]
        test   dword ptr [edx], 20h
        jne    SHORT loc_4_2E46
        movzx  edx, byte ptr [esi + 89h]
        cmp    dl, 0ffh
        je     SHORT loc_4_2E46
        dec    edx
        push   edx
        call   sub_4_120CA
        pop    edx
        or     eax, eax
        je     SHORT loc_4_2E46
        push   edx
        db  068h, 07Ah, 00Eh, 000h, 000h
        push   esi
        call   sub_4_2E4A
        add    esp, 0ch
loc_4_2E46:
        pop    edi
        pop    esi
        leave
        ret
sub_4_2E4A:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    esi, [esi + 98h]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        push   esi
        xor    eax, eax
        mov    edx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
        pop    esi
        mov    edi, esi
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        pop    esi
        leave
        ret
        db  066h, 0F7h, 045h, 02Ch, 001h, 000h, 075h, 057h, 066h, 0B8h, 000h, 038h, 0CDh, 020h, 01Ch, 000h
        db  001h, 000h, 083h, 0F8h, 0FFh, 074h, 048h, 08Bh, 0F0h, 066h, 081h, 03Eh, 05Ch, 05Ch, 074h, 007h
        db  066h, 081h, 03Eh, 02Fh, 02Fh, 075h, 038h, 080h, 0C2h, 041h, 0B6h, 02Eh, 066h, 039h, 056h, 002h
        db  075h, 02Dh, 080h, 07Eh, 004h, 05Ch, 074h, 006h, 080h, 07Eh, 004h, 02Fh, 075h, 021h, 08Bh, 0FEh
        db  088h, 017h, 0C6h, 047h, 001h, 03Ah, 083h, 0C7h, 002h, 083h, 0C6h, 007h, 0FCh, 080h, 03Eh, 000h
        db  075h, 007h, 066h, 0C7h, 007h, 05Ch, 000h, 0EBh, 006h, 0ACh, 0AAh, 00Ah, 0C0h, 075h, 0FAh, 0C3h
        db  066h, 0F7h, 045h, 02Ch, 001h, 000h, 074h, 018h, 066h, 083h, 07Dh, 01Ch, 002h, 075h, 011h, 066h
        db  0C7h, 045h, 01Ch, 010h, 000h, 066h, 0C7h, 045h, 018h, 010h, 000h, 066h, 083h, 065h, 02Ch, 0FEh
        db  0C3h
sub_4_2EFB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 14h]
        mov    esi, edi
        mov    ecx, 0ffffffffh
        xor    eax, eax
        cld
        repne scasb
        mov    ax, 3
        jne    NEAR PTR loc_4_2FE9
        not    ecx
        dec    ecx
        movzx  eax, byte ptr [ebx + 8bh]
        and    al, 3
        push   eax
        push   ecx
        push   esi
        mov    esi, dword ptr [ebx + 0ch]
        add    esi, 208h
        push   esi
        call   sub_4_14A69
        add    esp, 10h
        mov    word ptr [eax + esi], 0
        lea    esi, [eax + esi - 2]
        shr    eax, 1
        mov    ecx, eax
        mov    edx, ecx
        std
loc_4_2F4F:
        lodsw
        cmp    ax, 5ch
        je     SHORT loc_4_2F68
        cmp    ax, 2fh
        je     SHORT loc_4_2F68
        cmp    ax, 3ah
        je     SHORT loc_4_2F68
        loop   loc_4_2F4F
        sub    esi, 2
loc_4_2F68:
        add    esi, 4
        cld
        sub    edx, ecx
        mov    ecx, edx
        cmp    ecx, 8
        jbe    SHORT loc_4_2FE7
        mov    ax, 2eh
        mov    edi, esi
        repne scasw
        je     SHORT loc_4_2FE7
        add    esi, 10h
        sub    edx, 8
        mov    ecx, edx
        lea    edi, [esi + 2]
        mov    dx, word ptr [esi]
loc_4_2F8E:
        mov    eax, edx
        mov    dx, word ptr [edi]
        stosw
        loop   loc_4_2F8E
        xor    eax, eax
        stosw
        mov    word ptr [esi], 2eh
        push   201h
        push   208h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    edx, eax
        or     eax, eax
        mov    eax, 8
        je     SHORT loc_4_2FE9
        mov    dword ptr [ebx + 14h], edx
        movzx  ecx, byte ptr [ebx + 8bh]
        and    cl, 3
        push   ecx
        push   103h
        mov    ecx, dword ptr [ebx + 0ch]
        add    ecx, 208h
        sub    edi, ecx
        push   edi
        push   ecx
        push   edx
        call   sub_4_14A00
        add    esp, 14h
loc_4_2FE7:
        xor    eax, eax
loc_4_2FE9:
        mov    word ptr [ebx + 1ah], ax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  053h, 0CDh, 020h, 001h, 000h, 001h, 000h, 08Bh, 05Bh, 008h, 083h, 0ECh, 06Ch, 057h, 08Dh, 07Ch
        db  024h, 004h, 0CDh, 020h, 08Dh, 000h, 001h, 000h, 05Fh, 0CDh, 020h, 082h, 000h, 001h, 000h, 0C6h
        db  043h, 01Dh, 00Dh, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h
sub_4_301C:
        test   byte ptr [eax], al
        add    dword ptr [eax], eax
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebx
        ret
        db  000h, 0EDh, 012h, 000h, 000h, 055h, 014h, 000h, 000h, 055h, 014h, 000h, 000h, 055h, 014h, 000h
        db  000h, 055h, 014h, 000h, 000h, 055h, 014h, 000h, 000h, 01Ah, 013h, 000h, 000h, 019h, 012h, 000h
        db  000h, 01Ah, 013h, 000h, 000h, 01Ah, 013h, 000h, 000h, 019h, 012h, 000h, 000h, 019h, 012h, 000h
        db  000h, 044h, 015h, 000h, 000h, 055h, 014h, 000h, 000h, 055h, 014h, 000h, 000h, 055h, 014h, 000h
        db  000h, 055h, 014h, 000h, 000h, 055h, 014h, 000h, 000h, 055h, 014h, 000h, 000h, 055h, 014h, 000h
        db  000h, 055h, 014h, 000h, 000h, 055h, 014h, 000h, 000h, 055h, 014h, 000h, 000h, 055h, 014h, 000h
        db  000h, 055h, 014h, 000h, 000h, 055h, 014h, 000h, 000h, 0C2h, 014h, 000h, 000h, 055h, 014h, 000h
        db  000h, 0C2h, 014h, 000h, 000h, 0FDh, 014h, 000h, 000h, 019h, 012h, 000h, 000h, 019h, 012h, 000h
        db  000h, 019h, 012h, 000h, 000h, 059h, 013h, 000h, 000h, 019h, 012h, 000h, 000h
loc_4_30C4:
        cwde
        adc    dword ptr [eax], eax
        db  000h, 019h
        adc    al, byte ptr [eax]
        db  000h, 0CCh
        adc    al, byte ptr [eax]
        db  000h, 019h
        adc    al, byte ptr [eax]
        db  000h, 019h
        adc    al, byte ptr [eax]
        db  000h, 019h
        adc    al, byte ptr [eax]
        db  000h, 019h
        adc    al, byte ptr [eax]
        db  000h, 019h
        adc    al, byte ptr [eax]
        db  000h, 019h
        adc    al, byte ptr [eax]
        db  000h, 019h
        adc    al, byte ptr [eax]
        db  000h, 019h
        adc    al, byte ptr [eax]
        db  000h, 055h, 014h
        add    byte ptr [eax], al
        sbb    dword ptr [edx], edx
        add    byte ptr [eax], al
        sbb    dword ptr [edx], edx
        add    byte ptr [eax], al
        jb     SHORT loc_4_3111
        add    byte ptr [eax], al
        cmc
        adc    al, 0
        db  000h, 0F5h
        adc    al, 0
        db  000h, 0F5h
        adc    al, 0
        db  000h, 019h
        adc    al, byte ptr [eax]
        db  000h, 0F5h
loc_4_3111:
        adc    al, 0
        db  000h, 019h
        adc    al, byte ptr [eax]
        db  000h, 0F5h
        adc    al, 0
        db  000h, 019h
        adc    al, byte ptr [eax]
        db  000h, 019h
        adc    al, byte ptr [eax]
        db  000h, 019h
        adc    al, byte ptr [eax]
        db  000h, 019h
        adc    al, byte ptr [eax]
        db  000h, 0F5h
        adc    al, 0
        db  000h, 0F5h
        adc    al, 0
        db  000h, 0F5h
        adc    al, 0
        db  000h, 0F5h
        adc    al, 0
        db  000h, 0F5h
        adc    al, 0
        db  000h, 0F5h
        adc    al, 0
        db  000h, 0F5h
        adc    al, 0
        add    byte ptr [edi], cl
        mov    dh, 4dh
        sbb    al, 8bh
        db  015h, 044h, 059h, 000h, 000h
        add    edx, dword ptr [ebx + 4]
        test   byte ptr [ecx + edx + 2eh], 80h
        db  00Fh, 084h, 0B9h, 000h, 0FBh, 0FFh
        cmp    cl, 80h
        cmc
        sbb    edx, edx
        and    edx, 0fffffec8h
        db  0FFh, 0A4h, 08Ah, 038h, 010h, 000h, 000h
        db  090h, 08Bh, 055h, 01Ch, 084h, 0D2h, 075h, 005h, 0C6h, 045h, 01Ch, 0FFh, 0C3h, 0B9h, 0D2h, 000h
        db  000h, 000h, 052h, 0E8h, 0F9h, 003h, 0FBh, 0FFh, 05Ah, 0F6h, 045h, 02Ch, 001h, 075h, 001h, 0C3h
        db  089h, 055h, 01Ch, 0F9h, 0C3h, 066h, 0B8h, 004h, 03Ch, 0CDh, 020h, 01Ch, 000h, 001h, 000h, 08Bh
        db  015h, 0B4h, 055h, 000h, 000h, 00Fh, 0B6h, 054h, 013h, 00Ah, 080h, 038h, 000h, 074h, 024h, 066h
        db  081h, 038h, 05Ch, 05Ch, 074h, 046h, 066h, 081h, 038h, 02Fh, 02Fh, 074h, 03Fh, 080h, 078h, 001h
        db  03Ah, 075h, 010h, 08Ah, 010h, 080h, 0CAh, 020h, 080h, 0EAh, 061h, 080h, 0FAh, 020h, 077h, 02Ch
        db  083h, 0C0h, 002h, 083h, 0FAh, 020h, 073h, 024h, 0F6h, 082h, 08Ch, 000h, 000h, 000h, 001h, 075h
        db  00Eh, 052h, 003h, 015h, 044h, 059h, 000h, 000h, 0F6h, 042h, 02Fh, 001h, 05Ah, 074h, 00Dh, 033h
        db  0C0h, 0BEh, 001h, 012h, 000h, 000h, 0CDh, 020h, 00Dh, 000h, 001h, 000h, 0F9h, 0C3h, 0F6h, 045h
        db  02Ch, 001h, 074h, 06Ch, 066h, 0B8h, 004h, 03Ch, 0CDh, 020h, 01Ch, 000h, 001h, 000h, 096h, 08Bh
        db  0FEh, 0B9h, 080h, 000h, 000h, 000h, 0FCh, 033h, 0C0h, 0F2h, 0AEh, 075h, 053h, 04Fh, 08Bh, 0CFh
        db  02Bh, 0CEh, 051h, 052h, 056h, 0FDh, 08Dh, 077h, 0FFh, 0E3h, 046h, 0BAh, 020h, 020h, 020h, 020h
        db  08Bh, 0FAh, 0ACh, 03Ch, 03Ah, 074h, 025h, 03Ch, 05Ch, 074h, 021h, 03Ch, 02Fh, 074h, 01Dh, 03Ch
        db  02Eh, 075h, 003h, 049h, 0EBh, 0E3h, 00Fh, 0A4h, 0D7h, 008h, 0C1h, 0E2h, 008h, 03Ch, 061h, 072h
        db  006h, 03Ch, 07Ah, 077h, 002h, 02Ch, 020h, 08Ah, 0D0h, 0E2h, 0D7h, 04Eh, 0FCh, 0E8h, 0E0h, 02Dh
        db  001h, 000h, 072h, 00Dh, 058h, 05Ah, 059h, 083h, 0C6h, 002h, 02Bh, 0F0h, 066h, 001h, 075h, 004h
        db  0C3h, 05Eh, 05Ah, 059h, 0FCh, 080h, 07Eh, 001h, 03Ah, 075h, 00Ch, 080h, 07Eh, 002h, 05Ch, 074h
        db  0EFh, 080h, 07Eh, 002h, 02Fh, 074h, 0E9h, 066h, 0B8h, 000h, 038h, 0CDh, 020h, 01Ch, 000h, 001h
        db  000h, 097h, 080h, 07Eh, 001h, 03Ah, 074h, 008h, 0B4h, 03Ah, 08Ah, 0C2h, 004h, 041h, 066h, 0ABh
        db  041h, 0F3h, 0A4h, 08Bh, 004h, 095h, 048h, 066h, 000h, 000h, 08Bh, 00Dh, 0A0h, 065h, 000h, 000h
        db  0C1h, 0C8h, 004h, 066h, 089h, 041h, 006h, 0C1h, 0C0h, 004h, 066h, 083h, 0E0h, 00Fh, 066h, 089h
        db  041h, 004h, 080h, 065h, 02Ch, 0FEh, 0C3h, 08Bh, 0C0h, 080h, 0F9h, 007h, 075h, 00Ch, 066h, 08Bh
        db  00Dh, 04Ch, 000h, 000h, 000h, 066h, 089h, 04Dh, 014h, 0C3h, 080h, 0F9h, 008h, 075h, 009h, 08Ah
        db  04Dh, 014h, 088h, 00Dh, 04Ch, 000h, 000h, 000h, 0F9h, 0C3h, 0CDh, 020h, 000h, 000h, 041h, 000h
        db  072h, 01Bh, 066h, 0B8h, 030h, 034h, 0CDh, 020h, 01Ch, 000h, 001h, 000h, 066h, 081h, 038h, 0DAh
        db  0DAh, 075h, 005h, 066h, 0C7h, 000h, 0ADh, 0ADh, 066h, 083h, 065h, 02Ch, 0BFh, 0C6h, 045h, 01Ch
        db  0FFh, 066h, 083h, 065h
loc_4_3317:
        sub    al, 0feh
        ret
        db  0E8h, 0F9h, 001h, 000h, 000h, 00Fh, 082h, 0F4h, 0FEh, 0FAh, 0FFh, 083h, 0C1h, 076h, 066h, 089h
        db  055h, 010h, 08Bh, 050h, 015h, 066h, 089h, 055h, 01Ch, 0C1h, 0EAh, 010h, 066h, 089h, 055h, 014h
        db  050h, 0E8h, 044h, 002h, 0FBh, 0FFh, 058h, 066h, 0F7h, 045h, 02Ch, 001h, 000h, 075h, 00Fh, 066h
        db  08Bh, 055h, 014h, 0C1h, 0E2h, 010h, 066h, 08Bh, 055h, 01Ch, 089h, 050h, 015h, 0F8h, 0C3h, 0E8h
        db  0BAh, 001h, 000h, 000h, 00Fh, 082h, 0B5h, 0FEh, 0FAh, 0FFh, 066h, 089h, 055h, 010h, 0B9h, 097h
        db  000h, 000h, 000h, 0E9h, 012h, 002h, 0FBh, 0FFh, 0E8h, 0A1h, 001h, 000h, 000h, 00Fh, 082h, 09Ch
        db  0FEh, 0FAh, 0FFh, 0B9h, 0B8h, 000h, 000h, 000h, 0E9h, 0FDh, 001h, 0FBh, 0FFh, 080h, 03Dh, 030h
        db  059h, 000h, 000h, 000h, 075h, 002h
loc_4_3390:
        stc
        ret
        db  080h, 0F9h, 01Bh, 074h, 04Fh, 080h, 0F9h, 019h, 074h, 04Ah, 080h, 0F9h, 01Ch, 075h, 0EFh, 0A1h
        db  09Ch, 065h, 000h, 000h, 005h, 09Eh, 001h, 000h, 000h, 0BAh, 0E0h, 000h, 000h, 000h, 022h, 010h
        db  080h, 0FAh, 080h, 075h, 0D9h, 08Ah, 010h, 080h, 0E2h, 01Fh, 08Bh, 008h, 003h, 015h, 044h, 059h
        db  000h, 000h, 0F6h, 042h, 02Eh, 001h, 075h, 0C6h, 08Bh, 0C1h, 0CDh, 020h, 04Dh, 000h, 001h, 000h
        db  08Bh, 00Dh, 05Eh, 000h, 000h, 000h, 00Fh, 0B7h, 0D1h, 0C1h, 0E9h, 010h, 0CDh, 020h, 048h, 000h
        db  001h, 000h, 0EBh, 0AAh, 08Bh, 00Dh, 048h, 066h, 000h, 000h, 08Bh, 015h, 0A0h, 065h, 000h, 000h
        db  00Fh, 0B7h, 042h, 004h, 066h, 083h, 0F8h, 0FFh, 074h, 037h, 00Fh, 0B7h, 052h, 006h
loc_4_3400:
        shl    edx, 4
        add    eax, edx
        sub    eax, ecx
        cdq
        db  08Bh, 00Dh, 0C8h, 066h, 000h, 000h
        div    ecx
        push   eax
        call   sub_4_120CA
        or     eax, eax
        pop    eax
        jne    NEAR PTR loc_4_3390
        inc    eax
loc_4_3420:
        mov    edx, eax
        mov    ecx, edx
        db  080h, 0C9h
loc_4_3426:
        db  080h, 0A1h, 09Ch, 065h, 000h, 000h, 005h
        sahf
        add    dword ptr [eax], eax
        add    bl, ch
        mov    edi, dword ptr [eax + 1fh]
        jmp    SHORT loc_4_3420
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h, 0CDh, 020h, 04Eh, 000h, 001h, 000h, 08Bh, 015h, 09Ch, 065h
        db  000h, 000h, 081h, 0C2h, 09Eh, 001h, 000h, 000h, 088h, 002h, 0C3h, 08Bh, 03Dh, 0A0h, 065h, 000h
        db  000h, 066h, 083h, 07Fh, 004h, 0FFh, 074h, 03Ah, 00Fh, 0B7h, 07Fh, 014h, 003h, 03Dh, 0B0h, 065h
        db  000h, 000h, 066h, 08Bh, 007h, 080h, 0FCh, 03Ah, 00Fh, 085h, 00Fh, 0FFh, 0FFh, 0FFh, 00Ch, 020h
        db  02Ch, 060h, 00Fh, 0B6h, 0C0h, 083h, 0F8h, 020h, 077h, 016h, 0F6h, 080h, 08Bh, 000h, 000h, 000h
        db  001h, 075h, 01Ch, 08Bh, 015h, 044h, 059h, 000h, 000h, 0F6h, 044h, 010h, 02Eh, 001h, 075h, 00Fh
        db  0F9h, 0C3h, 080h, 03Dh, 0E1h, 055h, 000h, 000h, 000h, 00Fh, 084h, 0DEh, 0FEh, 0FFh, 0FFh, 066h
        db  0B8h, 030h, 034h, 0CDh, 020h, 01Ch, 000h, 001h, 000h, 066h, 08Bh, 000h, 066h, 089h, 045h, 01Ch
        db  083h, 0C1h, 076h, 0E9h, 0C2h, 000h, 0FBh, 0FFh, 0A1h, 09Ch, 065h, 000h, 000h, 00Fh, 0B7h, 050h
        db  00Eh, 00Fh, 0B7h, 040h, 00Ch, 0C1h, 0E2h, 004h, 003h, 0C2h, 08Ah, 010h, 080h, 0E2h, 0E0h, 080h
        db  0FAh, 080h, 075h, 015h, 00Fh, 0B6h, 000h, 08Bh, 015h, 044h, 059h, 000h, 000h, 0F6h, 044h, 010h
        db  0AEh, 001h, 075h, 0BBh, 0E9h, 094h, 0FEh, 0FFh, 0FFh, 0F9h, 0C3h, 083h, 0C1h, 026h, 0E9h, 087h
        db  000h, 0FBh, 0FFh, 0B9h, 093h, 000h, 000h, 000h, 0A1h, 0B4h, 055h, 000h, 000h, 081h, 07Ch, 003h
        db  00Ch, 0E1h, 008h, 000h, 000h, 075h, 005h, 0E8h, 06Eh, 000h, 0FBh, 0FFh, 0F9h, 0C3h, 066h, 0B8h
        db  000h, 038h, 0CDh, 020h, 01Ch, 000h, 001h, 000h, 081h, 078h, 007h
sub_4_3525:
        add    dword ptr [eax], eax
        add    dword ptr [eax], eax
        jne    SHORT loc_4_3542
        movzx  edx, word ptr [eax + 1bh]
        db  08Bh, 03Dh, 0B4h, 055h, 000h, 000h
        add    edi, ebx
        cmp    dword ptr [edi + edx*4 + 0d4h], 0
        je     SHORT loc_4_3542
        ret
loc_4_3542:
        stc
        ret
        db  00Fh, 0B7h, 045h, 038h, 0C1h, 0E0h, 004h, 00Fh, 0B7h, 04Dh, 000h, 003h, 0C1h, 08Dh, 03Dh, 048h
        db  066h, 000h, 000h, 0B9h, 020h, 000h, 000h, 000h, 0F2h, 0AFh, 075h, 029h, 083h, 0E9h, 020h, 0F7h
        db  0D9h, 0F6h, 081h, 08Bh, 000h, 000h, 000h, 001h, 075h, 011h, 08Bh, 03Dh, 044h, 059h, 000h, 000h
        db  003h, 07Bh, 004h, 08Ah, 054h, 039h, 02Eh, 0D0h, 0EAh, 073h, 00Ah, 0B9h, 082h, 000h, 000h, 000h
        db  0E9h, 0FBh, 0FFh, 0FAh, 0FFh, 0F9h, 0C3h
sub_4_358B:
        mov    eax, 13h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 041h, 001h, 001h, 000h
        db  039h, 005h, 0DDh, 055h, 000h, 000h
        jne    SHORT loc_4_35A6
        xor    eax, eax
loc_4_35A6:
        ret
sub_4_35A7:
        pushad
        push   1
        push   800h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_35E5
        push   eax
        mov    ebx, eax
        mov    eax, 201h
        xor    ecx, ecx
        inc    cl
        movzx  edx, word ptr [esp + 28h]
        push   13h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    eax
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_35E5:
        popad
        ret
        db  083h, 0FAh, 020h, 00Fh, 083h, 082h, 000h, 000h, 000h, 0F6h, 082h, 08Ch, 000h, 000h, 000h, 001h
        db  074h, 06Ah, 083h, 0F8h, 025h, 0F8h, 075h, 063h, 0F6h, 082h, 08Ch, 000h, 000h, 000h, 040h, 074h
        db  05Ah, 0F6h, 082h, 08Ch, 000h, 000h, 000h, 010h, 075h, 051h, 050h, 057h, 083h, 0ECh
loc_4_3615:
        add    al, 8bh
        cld
        mov    byte ptr [edi], 0ah
        mov    byte ptr [edi + 2], dl
        push   edi
        db  0CDh, 020h, 008h, 000h, 010h, 000h
        add    esp, 4
        movzx  ecx, byte ptr [edi + 3]
        movzx  eax, byte ptr [edi + 1]
        add    esp, 4
        or     eax, eax
        pop    edi
        pop    eax
        jne    SHORT loc_4_363E
        cmp    ecx, 2
        je     SHORT loc_4_3662
loc_4_363E:
        push   eax
        mov    eax, 13h
        db  0CDh, 020h, 041h, 001h, 001h, 000h
        db  039h, 005h, 0DDh, 055h, 000h, 000h
        je     SHORT loc_4_3661
        push   0
        push   edx
        call   sub_4_14881
        add    esp, 8
        stc
loc_4_3661:
        pop    eax
loc_4_3662:
        ret
        db  08Bh, 00Dh, 044h, 059h, 000h, 000h, 003h, 04Bh, 004h, 08Ah, 054h, 00Ah, 02Fh, 0D0h, 0EAh, 0F5h
        db  0C3h, 08Bh, 0C2h, 00Fh, 0B6h, 055h, 01Ch, 0E8h, 068h, 0FFh, 0FFh, 0FFh, 072h, 02Dh, 0B9h, 0DDh
        db  000h, 000h, 000h, 083h, 0F8h, 025h, 074h, 005h, 0B9h, 0DEh, 000h, 000h, 000h, 0CDh, 020h, 047h
        db  000h, 001h, 000h, 0A1h, 0B4h, 055h, 000h, 000h, 0BAh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 054h, 003h
        db  00Ch, 08Bh
loc_4_36A5:
        inc    ebp
        sub    al, 0cdh
        and    byte ptr [ebp], cl
        add    dword ptr [eax], eax
        ret
        db  08Bh, 00Dh, 052h, 000h, 000h, 000h, 08Bh, 015h, 04Eh, 000h, 000h, 000h, 083h, 0F8h, 025h, 074h
        db  00Ch, 08Bh, 00Dh, 05Ah, 000h, 000h, 000h, 08Bh, 015h, 056h, 000h, 000h, 000h, 0CDh, 020h, 049h
        db  080h, 001h, 000h, 00Fh, 0B6h, 055h, 01Ch, 0E8h, 00Dh, 0FFh, 0FFh, 0FFh, 072h, 01Eh, 0B9h, 0DDh
        db  000h, 000h, 000h, 083h, 0F8h, 025h, 074h, 005h, 0B9h, 0DEh, 000h, 000h, 000h, 0E8h, 094h, 0FEh
        db  0FAh, 0FFh, 08Bh, 045h, 02Ch, 0CDh, 020h, 04Dh, 000h, 001h, 000h, 0F8h, 0C3h, 000h, 00Eh, 000h
        db  004h, 000h, 00Ah, 000h, 04Ch, 000h, 050h, 000h, 054h, 000h, 000h, 000h, 000h, 000h, 0E8h, 00Dh
        db  000h, 000h, 000h, 066h, 0B8h, 000h, 000h, 073h, 002h, 066h, 040h, 066h, 089h, 045h, 010h, 0C3h
        db  00Fh, 0B7h, 045h, 014h, 083h, 0F8h, 003h, 00Fh, 087h, 082h, 000h, 000h, 000h, 083h, 03Ch, 085h
        db  034h, 06Eh, 000h, 000h, 000h, 075h, 072h, 083h, 03Ch, 085h, 058h, 06Eh, 000h, 000h, 000h, 075h
        db  068h, 080h, 07Dh, 01Dh, 000h, 075h, 068h, 053h, 08Bh, 0D8h, 081h, 0ECh, 004h, 001h, 000h, 000h
        db  08Bh, 0F4h, 056h, 0E8h, 0FBh, 0EFh, 0FFh, 0FFh, 083h, 0C4h, 004h, 056h, 0E8h, 073h, 000h, 0FBh
        db  0FFh, 083h, 0C4h, 004h, 00Bh, 0C0h, 075h, 031h, 0B9h, 008h, 000h, 000h, 000h, 08Bh, 07Eh
loc_4_376D:
        or     al, 0beh
        cld
        push   ss
        add    byte ptr [eax], al
        cld
        rep movsw
        mov    esi, esp
        add    ebx, 31h
        mov    eax, dword ptr [esi + 0ch]
        mov    word ptr [eax + 0ch], bx
        push   esi
        call   sub_4_F22B
        add    esp, 4
        push   eax
        push   esi
        db  0E8h, 032h, 001h, 0FBh, 0FFh
        add    esp, 4
        pop    eax
        add    esp, 104h
        sub    ebx, 31h
        or     eax, eax
        mov    eax, ebx
        pop    ebx
        jne    SHORT loc_4_37AD
        cmp    byte ptr [ebp + 1dh], 2
        jbe    SHORT loc_4_37E5
loc_4_37AD:
        stc
        ret
loc_4_37AF:
        cmp    byte ptr [esi + 12fh], 0
        jne    SHORT loc_4_37D5
        push   ebx
        push   ebp
        push   edi
        push   esi
        mov    ecx, 0cch
        mov    esi, 0ffffffffh
        db  0E8h, 0B9h, 0FDh, 0FAh, 0FFh
        pop    esi
        pop    edi
        pop    ebp
        pop    ebx
        cmp    byte ptr [edi + 1], 0
        jne    SHORT loc_4_3802
loc_4_37D5:
        cmp    byte ptr [edi + 1], 0
        setne  al
        shl    al, 7
        or     al, 10h
        mov    byte ptr [ebp + 1dh], al
        ret
loc_4_37E5:
        db  08Bh, 035h, 044h, 059h, 000h, 000h
        add    esi, dword ptr [ebx + 4]
        movzx  edi, word ptr [esi + eax*2 + 28h]
        add    edi, dword ptr [ebx + 4]
        db  003h, 03Dh, 0ACh, 065h, 000h, 000h
        cmp    byte ptr [edi + 1], 0
        je     SHORT loc_4_37AF
loc_4_3802:
        cmp    byte ptr [ebp + 1dh], 0
        jne    SHORT loc_4_37D5
        inc    word ptr [esi + 130h]
        movzx  eax, byte ptr [edi]
        mov    cl, byte ptr [ebp + 1ch]
        mov    byte ptr [eax + edi + 2], cl
        mov    byte ptr [ebp + 1dh], 10h
        inc    byte ptr [edi]
        dec    byte ptr [edi + 1]
        je     SHORT loc_4_3826
loc_4_3824:
        clc
        ret
loc_4_3826:
        cmp    byte ptr [esi + 12fh], 0
        jne    SHORT loc_4_3824
        mov    ecx, 0cch
        mov    esi, 0ffffffffh
        db  0E9h, 046h, 0FDh
loc_4_383C:
        cli
        inc    dword ptr [eax]
        db  000h, 08Bh, 015h, 044h, 059h, 000h
        add    byte ptr [ebx], al
        push   ebx
        add    al, 2bh
        leave
        xchg   word ptr [edx + 1c4h], cx
        jecxz  loc_4_386D
        db  08Bh, 03Dh, 09Ch, 065h, 000h, 000h
        mov    di, word ptr [edi + 10h]
        cmp    di, word ptr [edx + 1c6h]
        jne    SHORT loc_4_386D
        mov    word ptr [ebp + 1ch], cx
        pop    ecx
        ret
loc_4_386D:
        stc
        ret
        db  090h, 08Bh, 015h, 044h, 059h, 000h, 000h, 003h, 053h, 004h, 066h, 0C7h, 082h, 0C4h, 001h, 000h
        db  000h, 000h, 000h, 0F9h, 0C3h, 080h, 03Dh, 029h, 059h, 000h, 000h, 000h, 074h, 041h, 00Fh, 0B6h
        db  055h, 01Ch, 0F7h, 003h, 020h, 000h, 000h, 000h, 075h, 026h, 080h, 03Dh, 0E1h, 055h, 000h, 000h
        db  000h, 075h, 00Ch, 080h, 0FAh, 003h, 074h, 029h, 080h, 0FAh, 004h, 074h, 037h, 072h, 020h, 08Bh
        db  03Dh, 044h, 059h, 000h, 000h, 003h, 07Bh, 004h, 08Ah, 054h, 03Ah, 02Eh, 0C0h, 0EAh, 002h, 0C3h
        db  080h, 0FAh, 080h, 073h, 0EAh, 08Bh, 045h, 008h, 03Dh, 044h, 04Ah, 052h, 041h, 074h, 0E0h, 0F9h
        db  0C3h, 050h, 066h, 0B8h, 004h, 03Ch, 0CDh, 020h, 01Ch, 000h, 001h, 000h, 080h, 038h, 000h, 058h
        db  075h, 0EDh, 0F8h, 0C3h, 050h, 066h, 0B8h, 004h, 03Ch, 0CDh, 020h, 01Ch, 000h, 001h, 000h, 080h
        db  038h, 02Fh, 074h, 003h, 080h, 038h, 05Ch, 058h, 074h, 0E8h, 0F9h, 0C3h, 090h, 08Ah, 055h, 01Ch
        db  080h, 0EAh, 002h, 072h, 004h, 080h, 0FAh, 004h, 0F5h, 0C3h, 02Eh, 08Bh, 0C0h, 0F7h, 003h, 020h
        db  000h, 000h, 000h, 074h, 020h, 08Bh, 015h, 0B4h, 055h, 000h, 000h, 00Fh, 0B7h, 045h, 03Ch, 089h
        db  044h, 013h, 018h, 08Bh, 045h, 014h, 0F7h, 003h, 080h, 001h, 000h, 000h, 075h, 003h, 00Fh, 0B7h
        db  0C0h, 089h, 044h, 013h, 01Ch, 0F9h, 0C3h, 0F7h, 003h, 020h, 000h, 000h, 000h, 075h, 06Ch, 0A1h
        db  0B4h, 055h, 000h, 000h, 066h, 00Fh, 0BAh, 0ACh, 003h, 0D0h, 000h, 000h, 000h, 000h, 072h, 05Bh
        db  00Fh, 0B6h, 045h, 014h, 083h, 0F8h, 001h, 075h, 045h, 0F6h, 080h, 08Ch, 000h, 000h, 000h, 008h
        db  074h, 03Ch, 0CDh, 020h, 004h, 000h, 001h, 000h, 075h, 034h, 0F6h, 080h, 08Ch, 000h, 000h, 000h
        db  080h, 075h, 02Bh, 08Bh, 00Ch, 085h, 0C0h, 065h, 000h, 000h, 0E3h, 00Ah, 08Bh, 009h, 0E3h, 006h
        db  0F6h, 041h, 01Bh, 008h, 075h, 018h, 058h, 0A1h, 044h, 066h, 000h, 000h, 088h, 045h, 01Ch, 0A1h
        db  0B4h, 055h, 000h, 000h, 080h, 0A4h, 003h, 0D0h, 000h, 000h, 000h, 0FEh, 0F8h, 0C3h, 02Bh, 0C0h
        db  0BEh, 0ACh, 019h, 000h, 000h, 0CDh, 020h, 00Dh, 000h, 001h, 000h, 0F9h, 0C3h, 083h, 0ECh, 06Ch
        db  057h, 08Dh, 07Ch, 024h, 004h, 0CDh, 020h, 08Dh, 000h, 001h, 000h, 05Fh, 0CDh, 020h, 082h, 000h
        db  001h, 000h, 0C6h, 045h, 01Dh, 019h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h
        db  000h, 08Ah, 055h, 01Ch, 08Bh, 00Dh, 0B4h, 055h, 000h, 000h, 088h, 054h, 00Bh, 00Ah, 080h, 0A4h
        db  00Bh, 0D0h
sub_4_39E1:
        add    byte ptr [eax], al
        add    dh, bh
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
        db  0B8h, 0A4h, 015h, 000h, 000h, 052h, 00Fh, 0B6h, 055h, 01Ch, 080h, 0FAh, 024h, 073h, 007h, 08Bh
        db  004h, 095h, 054h, 012h, 000h, 000h, 05Ah, 0FFh, 0E0h, 080h, 07Dh, 01Ch, 00Dh, 075h, 036h, 080h
        db  07Dh, 019h, 008h, 074h, 006h, 080h, 07Dh, 019h, 048h, 075h, 02Ah, 080h
loc_4_3A27:
        db  07Dh, 018h
        popad
        je     SHORT loc_4_3A7E
        cmp    byte ptr [ebp + 18h], 41h
        je     SHORT loc_4_3A7E
        cmp    byte ptr [ebp + 18h], 42h
        je     SHORT loc_4_3A7E
        cmp    byte ptr [ebp + 18h], 62h
        je     SHORT loc_4_3A7E
        cmp    byte ptr [ebp + 18h], 71h
        je     SHORT loc_4_3A7E
        cmp    byte ptr [ebp + 18h], 49h
        je     SHORT loc_4_3A7E
        cmp    byte ptr [ebp + 18h], 4ch
        je     SHORT loc_4_3A8D
        test   dword ptr [ebx], 20h
        jne    SHORT loc_4_3A8C
        cmp    word ptr [ebp + 1ch], 440dh
        jne    SHORT loc_4_3A7E
        cmp    byte ptr [ebp + 19h], 8
        je     SHORT loc_4_3A6C
        cmp    byte ptr [ebp + 19h], 48h
        jne    SHORT loc_4_3A8C
loc_4_3A6C:
        cmp    byte ptr [ebp + 18h], 4bh
        je     SHORT loc_4_3A8D
        cmp    byte ptr [ebp + 18h], 6bh
        je     SHORT loc_4_3A8D
        cmp    byte ptr [ebp + 18h], 6eh
        je     SHORT loc_4_3A8D
loc_4_3A7E:
        test   edx, 1
        jne    SHORT loc_4_3A8C
        movzx  edx, byte ptr [ebp + 10h]
        jmp    SHORT loc_4_3AC4
loc_4_3A8C:
        stc
loc_4_3A8D:
        ret
        db  0F7h, 0C2h, 001h, 000h, 000h, 000h, 075h, 006h, 00Fh, 0B6h, 055h, 010h
loc_4_3A9A:
        jmp    SHORT loc_4_3AC4
        db  0F9h, 0C3h, 00Fh, 0BAh, 023h, 005h, 072h, 00Ch, 0F7h, 0C2h, 001h, 000h, 000h, 000h, 075h, 004h
        db  033h, 0D2h, 0EBh, 014h, 0F9h, 0C3h, 00Fh, 0BAh, 023h, 005h, 072h, 02Bh, 0F7h, 0C2h, 001h, 000h
        db  000h, 000h, 075h, 033h, 00Fh, 0B6h, 055h, 014h
loc_4_3AC4:
        cmp    dl, 0
        jne    SHORT loc_4_3AD5
        db  08Bh, 015h, 0B4h, 055h, 000h, 000h
        movzx  edx, byte ptr [ebx + edx + 0ah]
        inc    edx
loc_4_3AD5:
        cmp    edx, 20h
        ja     SHORT loc_4_3AF3
        db  0F6h, 082h, 08Bh, 000h, 000h, 000h, 001h
        je     SHORT loc_4_3AE4
        ret
loc_4_3AE4:
        db  08Bh, 03Dh, 044h, 059h, 000h, 000h
        add    edi, dword ptr [ebx + 4]
        mov    dl, byte ptr [edx + edi + 2eh]
        shr    dl, 1
loc_4_3AF3:
        cmc
        ret
        db  0F7h, 0C1h, 000h, 000h, 000h, 080h, 074h, 00Ch, 0F7h, 0C1h, 000h, 000h, 000h, 004h, 074h, 004h
        db  08Bh, 0D7h, 0EBh, 022h, 0F7h, 003h, 020h, 000h, 000h, 000h, 075h, 076h, 0F7h, 0C2h, 001h, 000h
        db  000h, 000h, 075h, 06Ch, 0A1h, 09Ch, 065h, 000h, 000h, 00Fh, 0B7h, 050h, 00Eh, 00Fh, 0B7h, 040h
        db  00Ch, 0C1h, 0E2h, 004h, 003h, 0D0h, 08Ah, 002h, 024h, 0E0h, 03Ch, 080h, 075h, 052h, 00Fh, 0B6h
        db  002h, 03Ch, 09Fh, 075h, 00Bh, 080h, 03Dh, 029h, 059h, 000h, 000h, 000h, 074h, 042h, 0EBh, 029h
        db  003h, 005h, 044h, 059h, 000h, 000h, 0F6h, 040h, 0AEh, 001h, 074h, 034h, 080h, 03Dh, 031h, 059h
        db  000h, 000h, 001h, 075h, 014h, 00Fh, 0B6h, 002h, 024h, 03Fh, 08Bh, 004h, 085h, 0BCh, 065h, 000h
        db  000h, 08Bh, 000h, 08Ah, 040h, 003h, 088h, 042h, 013h, 00Fh, 0B6h, 042h, 013h, 08Bh, 004h, 085h
        db  0B0h, 083h
sub_4_3B77:
        add    byte ptr [eax], al
        test   eax, eax
        je     SHORT loc_4_3B85
        mov    dl, byte ptr [edx + 14h]
        cmp    byte ptr [eax + 2], dl
        je     SHORT loc_4_3B86
loc_4_3B85:
        stc
loc_4_3B86:
        ret
        db  0A1h, 0B4h, 055h, 000h, 000h, 051h, 0FFh, 074h, 003h, 01Ch, 0FFh, 074h, 003h, 018h, 0E8h, 040h
        db  0E6h, 0FFh, 0FFh, 083h, 0C4h, 008h, 059h, 08Bh, 0D0h, 0EBh, 089h, 0F7h, 003h, 020h, 000h, 000h
        db  000h, 075h, 073h, 080h, 07Dh, 01Ch, 021h, 075h, 06Dh, 08Bh, 015h, 0B4h, 055h, 000h, 000h, 003h
        db  0D3h, 08Bh, 03Dh, 044h, 059h, 000h, 000h, 051h, 00Fh, 0B7h, 04Dh, 03Ch, 0C1h, 0E1h, 004h, 00Fh
        db  0B7h, 045h, 014h, 003h, 0C8h, 03Bh, 00Dh, 008h, 06Bh, 000h, 000h, 059h, 075h, 026h, 0F6h, 042h
        db  008h, 010h, 074h, 042h, 080h, 062h, 008h, 0EFh, 080h, 0A7h, 02Eh, 001h, 000h, 000h, 0FEh, 0FEh
        db  00Dh, 00Dh, 06Bh, 000h, 000h, 075h, 02Fh, 080h, 025h, 00Ch, 06Bh, 000h, 000h, 0FDh, 080h, 067h
        db  011h, 0F7h, 0EBh, 022h, 0F6h, 042h, 008h, 010h, 075h, 01Ch, 080h, 04Ah, 008h, 010h, 080h, 08Fh
        db  02Eh, 001h, 000h, 000h, 001h, 0FEh, 005h, 00Dh, 06Bh, 000h, 000h, 080h, 00Dh, 00Ch, 06Bh, 000h
        db  000h, 002h, 080h, 04Fh, 011h, 008h, 0F9h, 0C3h, 0F7h, 003h, 020h, 000h, 000h, 000h, 075h, 01Ah
        db  083h, 0FAh, 002h, 074h, 015h, 0CDh, 020h, 004h, 000h, 001h, 000h, 075h, 00Dh, 02Bh, 0C0h, 0BEh
        db  043h, 01Ch, 000h, 000h, 0CDh, 020h, 00Dh, 000h, 001h, 000h, 0F9h, 0C3h, 080h, 07Dh, 01Ch, 0FFh
        db  074h, 03Dh, 066h, 0B8h, 000h, 038h, 0CDh, 020h, 01Ch, 000h, 001h, 000h, 040h, 074h, 030h, 00Fh
        db  0B6h, 040h, 0FFh, 00Bh, 0C0h, 075h, 00Ch, 0A1h, 0B4h, 055h, 000h, 000h, 003h, 0C3h, 00Fh, 0B6h
        db  040h, 00Ah, 040h, 083h, 0F8h, 002h, 075h, 017h, 048h, 0F6h, 080h, 08Ch, 000h, 000h, 000h, 008h
        db  074h, 00Dh, 0F6h, 080h, 08Ch, 000h, 000h, 000h, 080h, 075h, 004h, 0C6h, 045h, 01Ch, 0FFh, 0C3h
        db  090h, 0B8h, 004h, 03Ch, 000h, 000h, 0EBh, 005h
sub_4_3C8F:
        mov    eax, 3c14h
        test   ecx, 80000000h
        je     SHORT loc_4_3CC6
        cmp    cl, 4eh
        jne    SHORT loc_4_3CAD
        test   ecx, 40000000h
        jne    NEAR PTR loc_4_3D5A
loc_4_3CAD:
        test   ecx, 4000000h
        je     SHORT loc_4_3CD2
        push   ecx
        push   esi
        mov    esi, dword ptr [ebp + 14h]
        cmp    eax, 3c14h
        je     SHORT loc_4_3CDC
        mov    esi, dword ptr [ebp + 4]
        jmp    SHORT loc_4_3CDC
loc_4_3CC6:
        test   edx, 1
        je     NEAR PTR loc_4_3D5A
loc_4_3CD2:
        push   ecx
        push   esi
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    esi, eax
loc_4_3CDC:
        sub    esp, 104h
        mov    edi, esp
        push   ecx
        push   edi
        call   sub_4_2751
        pop    edi
        pop    ecx
        mov    dword ptr [edi + 14h], esi
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    edi, dword ptr [edi]
        and    edi, 30000h
        shl    edi, 8
        or     ecx, edi
        pop    edi
        rol    ecx, 8
        mov    byte ptr [edi + 8bh], cl
        mov    eax, dword ptr [edi + 94h]
        mov    al, byte ptr [eax + 0ah]
        inc    al
        mov    byte ptr [edi + 89h], al
        push   edi
        call   sub_4_15558
        pop    edi
        push   eax
        push   edi
        db  0E8h, 098h, 0FBh, 0FAh, 0FFh
        pop    edi
        pop    eax
        add    esp, 104h
        pop    esi
        pop    ecx
        test   ecx, 80000000h
        je     SHORT loc_4_3D4B
        and    eax, 0ffffffh
        cmp    eax, 6
        je     SHORT loc_4_3D59
        clc
        ret
loc_4_3D4B:
        test   eax, 20000000h
        jne    SHORT loc_4_3D5B
        test   eax, 4000000h
        jne    SHORT loc_4_3D5B
loc_4_3D59:
        stc
loc_4_3D5A:
        ret
loc_4_3D5B:
        mov    word ptr [ebp + 1ch], 3
        or     word ptr [ebp + 2ch], 1
        xor    ecx, ecx
        dec    ecx
        stc
        ret
        db  090h, 080h, 0F9h, 074h, 00Fh, 083h, 097h, 000h, 000h, 000h, 08Bh, 016h, 089h, 055h, 01Ch, 08Bh
        db  056h, 004h, 089h, 055h, 010h, 08Bh, 056h, 008h, 089h, 055h, 018h, 08Bh, 056h, 00Ch, 089h, 055h
        db  014h, 08Bh, 056h, 010h, 089h, 055h, 004h, 08Bh, 056h, 014h, 089h, 055h, 000h, 08Bh, 056h, 018h
        db  089h, 055h, 008h, 056h, 081h, 0C9h, 000h, 000h, 000h, 0A4h, 08Bh, 07Eh, 020h, 033h, 0D2h, 0FFh
        db  014h, 08Dh, 09Ch, 012h, 000h, 070h, 05Eh, 072h, 023h, 0F6h, 046h, 024h, 004h, 074h, 006h, 081h
        db  0C9h, 000h, 000h, 000h, 008h, 08Bh, 056h, 021h, 081h, 0E2h, 000h, 000h, 000h, 003h, 00Bh, 0CAh
        db  08Bh, 056h, 020h, 0A1h, 0B4h, 055h, 000h, 000h, 0FFh, 064h, 003h, 00Ch, 066h, 0B8h, 000h, 05Ch
        db  080h, 07Eh, 001h, 0BCh, 074h, 021h, 066h, 040h, 080h, 07Eh, 001h, 0BDh, 074h, 019h, 066h, 08Bh
        db  006h, 080h, 0FCh, 071h, 075h, 014h, 03Ch, 0A0h, 073h, 010h, 03Ch, 04Eh, 074h, 00Ch, 03Ch, 04Fh
        db  074h, 008h, 08Ah, 0E0h, 08Ah, 046h, 004h, 066h, 089h, 006h, 0CDh, 020h, 011h, 000h, 015h, 000h
        db  0C3h, 0F9h, 0C3h, 08Bh, 0C0h, 080h, 07Dh, 01Ch, 000h, 00Fh, 085h, 081h, 000h, 000h, 000h, 083h
        db  0ECh, 06Ch, 08Bh, 0FCh, 0CDh, 020h, 08Dh, 000h, 001h, 000h, 092h, 066h, 0B8h, 014h, 03Ch, 0CDh
        db  020h, 01Ch, 000h, 001h, 000h, 066h, 08Bh, 008h, 066h, 089h, 04Dh, 01Ch, 066h, 08Bh, 048h, 002h
        db  066h, 089h, 04Dh, 010h, 066h, 08Bh, 048h, 004h, 066h, 089h, 04Dh, 018h, 066h, 08Bh, 048h, 006h
        db  066h, 089h, 04Dh, 014h, 066h, 08Bh, 048h, 008h, 066h, 089h, 04Dh, 004h, 066h, 08Bh, 048h, 00Ah
        db  066h, 089h, 04Dh, 000h, 066h, 08Bh, 048h, 00Ch, 066h, 089h, 04Dh, 03Ch, 066h, 08Bh, 048h, 00Eh
        db  066h, 089h, 04Dh, 038h, 08Bh, 0F0h, 092h, 00Fh, 0B6h, 04Dh, 01Dh, 080h, 0F9h, 074h, 073h, 015h
        db  081h, 0C9h, 000h, 000h, 000h, 080h, 033h, 0D2h, 0FFh, 014h, 08Dh, 09Ch, 012h, 000h, 000h, 072h
        db  004h, 083h, 0C4h, 06Ch, 0C3h, 08Bh, 0F4h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 083h, 0C4h, 06Ch
        db  0F9h, 0C3h, 0F9h, 0C3h, 090h, 0F7h, 0C1h, 000h, 000h, 000h, 040h, 074h, 009h, 00Fh, 0B6h, 055h
        db  014h, 0E9h, 013h, 0FCh, 0FFh, 0FFh, 0F7h, 003h, 020h, 000h, 000h, 000h, 075h, 017h, 0F7h, 0C2h
        db  001h, 000h, 000h, 000h, 075h, 00Fh, 08Bh, 0D1h, 02Bh, 0C0h, 0BEh, 0D2h, 01Eh, 000h, 000h, 0CDh
        db  020h, 00Dh, 000h, 001h, 000h, 0F9h, 0C3h, 08Bh, 0CAh, 0E9h, 0ABh, 0F6h, 0FAh, 0FFh, 080h, 07Dh
        db  01Ch, 001h, 075h, 016h, 0A1h, 0B4h, 055h, 000h, 000h, 080h, 064h, 003h, 008h, 0DFh, 08Ah, 055h
        db  014h, 00Ah, 0D2h, 074h, 005h, 080h, 04Ch, 003h, 008h, 020h, 0F9h, 0C3h, 050h, 066h, 08Bh, 045h
        db  01Ch, 080h, 0FCh, 071h, 075h, 010h, 03Ch, 0A3h, 074h, 00Dh, 03Ch, 0A4h, 074h, 009h, 03Ch, 0A5h
        db  074h, 005h, 03Ch, 0A8h, 074h, 001h, 0F9h, 058h, 0C3h
loc_4_3F14:
        cmp    dword ptr [edx], edi
        db  03Bh, 041h
loc_4_3F18:
        inc    ebx
        inc    edi
        dec    esi
        dec    edi
        push   esi
        insb
        pushad
sub_4_3F1F:
        mov    al, byte ptr [0a4a3a2a1h]
        movsd
        cmpsb
        cmpsd
        test   al, 0a9h
        stosb
        stosd
        or     eax, 0e04fdcdbh
        loope  loc_4_3F14
        jecxz  loc_4_3F18
        in     eax, 0e6h
        out    0eah, eax
        db  0E0h, 01Fh
        add    byte ptr [eax], al
        db  0E0h, 01Fh
        add    byte ptr [eax], al
        loopne loc_4_3F61
        add    byte ptr [eax], al
        db  0E0h, 01Fh
        add    byte ptr [eax], al
        loopne loc_4_3F69
        add    byte ptr [eax], al
        db  0E0h, 01Fh
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        db  0E0h, 01Fh
        add    byte ptr [eax], al
        loopne loc_4_3F79
        add    byte ptr [eax], al
        mov    byte ptr [eax + eax], bl
        db  000h, 0E0h
loc_4_3F61:
        pop    ds
        add    byte ptr [eax], al
        loopne loc_4_3F85
        add    byte ptr [eax], al
        push   eax
loc_4_3F69:
        push   ecx
        mov    ecx, 18h
        movzx  eax, byte ptr [ebp + 1ch]
        db  0BFh, 014h, 01Fh, 000h, 000h
        cld
loc_4_3F79:
        repne scasb
        pop    ecx
        jne    SHORT loc_4_3FDD
        movzx  edi, al
        cmp    al, 0a0h
        jb     SHORT loc_4_3F8F
loc_4_3F85:
        sub    al, 0a0h
        db  08Ah, 080h, 02Ch, 01Fh, 000h, 000h
        jmp    SHORT loc_4_3F98
loc_4_3F8F:
        mov    byte ptr [ebp + 1dh], al
        mov    ah, byte ptr [ebp + 10h]
        mov    byte ptr [ebp + 1ch], ah
loc_4_3F98:
        mov    cl, al
        pop    eax
        or     ecx, 40000000h
        cmp    cl, 4eh
        jne    SHORT loc_4_3FB7
        cmp    edx, 1
        jne    SHORT loc_4_3FB7
        call   sub_4_3C8F
        cmp    ecx, -1
        je     SHORT loc_4_3FDE
        clc
        ret
loc_4_3FB7:
        cmp    cl, 4fh
        je     SHORT loc_4_3FDC
        cmp    edi, 0a0h
        jb     SHORT loc_4_3FCD
        db  0FFh, 014h, 0BDh, 0B8h, 01Ch, 000h, 000h
        jmp    SHORT loc_4_3FDC
loc_4_3FCD:
        movzx  edi, cl
        db  068h, 0DCh, 01Fh, 000h, 000h
        db  0FFh, 024h, 0BDh, 09Ch, 012h, 000h, 000h
loc_4_3FDC:
        ret
loc_4_3FDD:
        pop    eax
loc_4_3FDE:
        stc
        ret
        db  081h, 0C9h, 000h, 000h, 000h, 040h, 0C3h, 032h, 036h, 0E8h, 0E9h, 080h, 07Dh, 01Ch, 002h, 072h
        db  027h, 080h, 07Dh, 01Ch, 005h, 077h, 020h, 080h, 07Dh, 01Ch, 003h, 074h, 00Bh, 00Fh, 0B6h, 055h
        db  014h, 0E8h, 0BEh, 0FAh, 0FFh, 0FFh, 072h, 010h, 00Fh, 0B6h, 045h, 01Ch, 083h, 0E8h, 002h, 00Fh
        db  0B6h, 088h, 0E7h, 01Fh, 000h, 000h, 0F9h, 0F5h, 0C3h
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        movzx  edx, word ptr [ebp + 8]
        db  0BBh, 09Ch, 012h, 000h, 000h
        cmp    edx, 74h
        jb     SHORT loc_4_4039
        db  08Bh, 01Dh, 000h, 06Bh, 000h, 000h
        test   ebx, ebx
        je     SHORT loc_4_4043
loc_4_4036:
        sub    edx, 74h
loc_4_4039:
        mov    eax, dword ptr [ebp + 0ch]
        xchg   dword ptr [ebx + edx*4], eax
loc_4_403F:
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_4043:
        push   1
        push   230h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_403F
        mov    ebx, eax
        mov    edi, eax
        mov    ecx, 8ch
        db  0B8h, 0A4h, 015h, 000h, 000h
        rep stosd
        db  089h, 01Dh, 000h, 06Bh, 000h, 000h
        movzx  edx, word ptr [ebp + 8]
        jmp    SHORT loc_4_4036
        db  08Bh, 044h, 024h, 004h, 087h, 005h, 004h, 06Bh, 000h, 000h, 0C3h, 000h, 000h
sub_4_4080:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edi
        push   edx
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        sub    esp, 10h
        mov    edi, esp
        mov    byte ptr [edi], 0
        mov    byte ptr [edi + 2], 1
        mov    ecx, dword ptr [ebp + 10h]
        or     ecx, ecx
        jne    SHORT loc_4_40AB
        movzx  ecx, byte ptr [ebx + 89h]
        mov    byte ptr [edi + 2], 0
loc_4_40AB:
        mov    dword ptr [edi + 3], ecx
        mov    dword ptr [edi + 7], edx
        push   edi
        db  0CDh, 020h, 008h, 000h, 010h, 000h
        add    esp, 4
        movzx  eax, byte ptr [edi + 1]
        xor    ecx, ecx
        or     eax, eax
        jne    SHORT loc_4_40C8
        mov    ecx, dword ptr [edi + 0bh]
loc_4_40C8:
        mov    dword ptr [ebx + 10h], ecx
        add    esp, 10h
        pop    edx
        pop    edi
        pop    ecx
        pop    ebx
        leave
        ret
sub_4_40D4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebx + 10h]
        mov    ecx, dword ptr [ebx + 1ch]
        mov    bl, byte ptr [ebx + 89h]
        sub    esp, 0ch
        mov    edi, esp
        mov    byte ptr [edi], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [edi + 3], al
        mov    byte ptr [edi + 2], bl
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi + 8], ecx
        push   edi
        db  0CDh, 020h, 008h, 000h, 010h, 000h
        add    esp, 4
        add    esp, 0ch
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        leave
        ret
sub_4_4114:
        push   ebp
        mov    ebp, esp
        push   edi
        push   edx
        push   ecx
        push   1
        push   104h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    edi, eax
        mov    eax, 8
        or     edi, edi
        je     NEAR PTR loc_4_41EF
        push   edi
        call   sub_4_2751
        add    esp, 4
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        db  0F6h, 082h, 08Ch, 000h, 000h, 000h, 080h
        je     SHORT loc_4_4186
        db  08Bh, 014h, 095h, 0C0h, 065h, 000h, 000h
        movzx  edx, byte ptr [edx + 0fh]
        db  08Bh, 00Ch, 095h, 0BCh, 065h, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_4_416F
        mov    word ptr [edi + 1ah], 0fh
loc_4_416D:
        jmp    SHORT loc_4_41DD
loc_4_416F:
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        je     SHORT loc_4_4185
        cmp    dword ptr [ecx + 1ch], 0
        je     SHORT loc_4_4185
        cmp    eax, 1
        jne    SHORT loc_4_4185
loc_4_4180:
        mov    eax, 0
loc_4_4185:
        dec    edx
loc_4_4186:
        mov    byte ptr [edi + 89h], dl
        db  08Bh, 015h, 0B8h, 061h, 000h, 000h
        mov    dword ptr [edi + 1ch], edx
        push   edi
        cmp    eax, 2
        jae    SHORT loc_4_41F4
loc_4_419B:
        push   eax
        call   sub_4_E6BD
        pop    eax
        mov    edx, dword ptr [ebp + 8]
        db  0F6h, 082h, 08Ch, 000h, 000h, 000h, 080h
        je     SHORT loc_4_41DC
        mov    byte ptr [edi + 89h], dl
        db  08Bh, 014h, 095h, 0C0h, 065h, 000h, 000h
        movzx  edx, byte ptr [edx + 0fh]
        db  0F6h, 082h, 08Bh, 000h, 000h, 000h, 080h
        jne    SHORT loc_4_41D4
        db  08Bh, 014h, 095h, 0BCh, 065h, 000h, 000h
        mov    edx, dword ptr [edx]
        mov    dword ptr [edi + 7ch], edx
loc_4_41D4:
        push   0
        db  0E8h, 005h, 0FDh, 0FAh, 0FFh
        pop    eax
loc_4_41DC:
        pop    eax
loc_4_41DD:
        movzx  eax, word ptr [edi + 1ah]
        push   eax
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    eax
loc_4_41EF:
        pop    ecx
        pop    edx
        pop    edi
        pop    ebp
        ret
loc_4_41F4:
        je     SHORT loc_4_4213
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0C0h, 065h, 000h, 000h
        mov    eax, dword ptr [eax]
        pop    edi
        mov    dword ptr [edi + 7ch], eax
        push   2
        push   edi
        call   sub_4_E4EA
        add    esp, 4
        jmp    SHORT loc_4_41DC
loc_4_4213:
        mov    word ptr [edi + 18h], 1
        jmp    SHORT loc_4_419B
sub_4_421B:
        db  08Bh, 03Ch, 085h, 0C0h, 065h, 000h, 000h
        or     edi, edi
        je     NEAR PTR loc_4_42D5
        db  0F6h, 080h, 08Ch, 000h, 000h, 000h, 080h
        je     SHORT loc_4_424D
        movzx  eax, byte ptr [edi + 0fh]
        dec    eax
        db  0F6h, 080h, 08Ch, 000h, 000h, 000h, 080h
        jne    SHORT loc_4_424D
        cmp    dword ptr [edi], 0
        db  08Bh, 03Ch, 085h, 0C0h, 065h, 000h, 000h
        je     SHORT loc_4_4253
loc_4_424D:
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_4_425E
loc_4_4253:
        jecxz  loc_4_429C
        xor    edi, edi
        call   sub_4_4315
        jmp    SHORT loc_4_429C
loc_4_425E:
        db  00Fh, 0A3h, 005h, 064h, 000h, 000h, 000h
        jae    SHORT loc_4_4281
        cmp    dword ptr [edi + 1ch], 0
        je     SHORT loc_4_4253
        mov    edi, dword ptr [edi + 20h]
        test   dword ptr [edi + 4], 3
        jne    SHORT loc_4_4282
loc_4_4279:
        test   dword ptr [edi], 2000h
        jne    SHORT loc_4_42DE
loc_4_4281:
        ret
loc_4_4282:
        jecxz  loc_4_4281
        test   dword ptr [edi + 4], 1
        jne    SHORT loc_4_429C
        test   dword ptr [edi], 40000h
        je     SHORT loc_4_429C
        call   sub_4_4315
        je     SHORT loc_4_4279
loc_4_429C:
        db  0F6h, 080h, 08Ch, 000h, 000h, 000h, 080h
        jne    SHORT loc_4_42D3
        or     eax, eax
        jne    SHORT loc_4_42D3
        db  0F6h, 080h, 08Ch, 000h, 000h, 000h, 008h
        je     SHORT loc_4_42D3
        db  03Bh, 01Dh, 0B0h, 055h, 000h, 000h
        jne    SHORT loc_4_42D3
        sub    esp, 4
        mov    ecx, esp
        mov    byte ptr [ecx], 4
        mov    byte ptr [ecx + 2], 0
        push   ecx
        db  0CDh, 020h, 008h, 000h, 010h, 000h
        add    esp, 4
        add    esp, 4
loc_4_42D3:
        stc
        ret
loc_4_42D5:
        jecxz  loc_4_42D3
        call   sub_4_4315
        jmp    SHORT loc_4_42D3
loc_4_42DE:
        mov    edi, eax
        db  066h, 087h, 03Dh, 0ACh, 000h, 000h, 000h
        cmp    edi, eax
        jne    SHORT loc_4_42EC
        ret
loc_4_42EC:
        db  08Bh, 00Ch, 085h, 0C0h, 065h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        mov    ecx, dword ptr [ecx + 20h]
        db  08Bh, 03Ch, 0BDh, 0C0h, 065h, 000h, 000h
        or     edi, edi
        je     SHORT loc_4_429C
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     SHORT loc_4_429C
        cmp    byte ptr [edi + 1ah], 1
        jne    SHORT loc_4_429C
        cmp    dword ptr [edi + 20h], ecx
        jne    SHORT loc_4_429C
        ret
sub_4_4315:
        sub    esp, 8
        mov    ecx, esp
        mov    byte ptr [ecx], 3
        mov    dword ptr [ecx + 3], edi
        mov    byte ptr [ecx + 2], al
        push   ecx
        db  0CDh, 020h, 008h, 000h, 010h, 000h
        add    esp, 4
        mov    cl, byte ptr [ecx + 1]
        add    esp, 8
        or     cl, cl
        ret
sub_4_4336:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edi + 90h]
        cmp    eax, 20h
        jae    SHORT IFSMgr_DosToNetTime
        db  08Bh, 00Ch, 085h, 0C0h, 065h, 000h, 000h
        jecxz  loc_4_435F
        mov    ecx, dword ptr [ecx]
        jecxz  loc_4_435F
        test   byte ptr [ecx + 1bh], 8
        jne    SHORT loc_4_4390
loc_4_435F:
        cmp    eax, 1
        jne    SHORT loc_4_4386
        db  0F6h, 080h, 08Ch, 000h, 000h, 000h, 008h
        je     SHORT loc_4_4386
        db  0F6h, 080h, 08Ch, 000h, 000h, 000h, 080h
        jne    SHORT loc_4_4386
        db  03Bh, 01Dh, 0B0h, 055h, 000h, 000h
        jne    SHORT loc_4_4386
IFSMgr_DosToNetTime:
        mov    word ptr [edi + 1ah], 0fh
        jmp    SHORT loc_4_4390
loc_4_4386:
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_4_421B
        jb     SHORT loc_4_4396
loc_4_4390:
        xor    eax, eax
loc_4_4392:
        pop    ebx
        pop    edi
        pop    ebp
        ret
loc_4_4396:
        mov    eax, 1
        jmp    SHORT loc_4_4392
sub_4_439D:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        mov    edx, dword ptr [esp + 10h]
        sub    esp, 14h
        mov    ecx, esp
        mov    byte ptr [ecx], 8
        mov    word ptr [ecx + 2], ax
        mov    dword ptr [ecx + 4], edx
        push   ecx
        db  0CDh, 020h, 008h, 000h, 010h, 000h
        add    esp, 4
        mov    ecx, esp
        mov    eax, dword ptr [ecx + 0ch]
        mov    edx, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ecx + 10h]
        add    esp, 14h
        push   ecx
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [esp + 0ch]
        pop    dword ptr [ecx]
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    ebx, 20h
        jae    SHORT loc_4_4450
        db  0F6h, 083h, 08Ch, 000h, 000h, 000h, 001h
        je     SHORT loc_4_4450
        mov    eax, dword ptr [ebp + 8]
        db  0BEh, 080h, 06Eh, 000h, 000h
loc_4_4401:
        mov    esi, dword ptr [esi + 4]
        or     esi, esi
        je     SHORT loc_4_4450
        cmp    dword ptr [esi + 20h], eax
        jne    SHORT loc_4_4401
        sub    esp, 104h
        mov    edi, esp
        push   edi
        call   sub_4_2751
        add    esp, 4
        movzx  eax, word ptr [esi + 14h]
        push   eax
        push   dword ptr [esi + 8]
        push   dword ptr [ebp + 8]
        push   dword ptr [esi + 1ch]
        db  033h
IFSMgr_DosToWin32Time:
        ror    byte ptr [edi], 0abh
        fcom   dword ptr [eax + 57h]
        call   sub_4_EBB1
        add    esp, 11ch
        db  08Bh, 004h, 09Dh, 0C0h, 065h, 000h, 000h
IFSMgr_Win32ToDosTime:
        or     byte ptr [eax + 0bh], 4
        xor    eax, eax
loc_4_444B:
        pop    esi
        pop    edi
        pop    ebx
        leave
        ret
loc_4_4450:
        mov    eax, 0fh
        jmp    SHORT loc_4_444B
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 0ch]
loc_4_4460:
        cmp    ebx, 20h
        jae    NEAR PTR loc_4_4534
        db  0F6h, 083h, 08Ch, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_4_4534
        db  08Bh, 034h, 09Dh, 0C0h, 065h, 000h, 000h
        mov    esi, dword ptr [esi]
        or     esi, esi
        je     NEAR PTR loc_4_4534
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 20h], eax
        jne    NEAR PTR loc_4_4534
        sub    esp, 104h
        mov    edi, esp
        push   edi
        call   sub_4_2751
        add    esp, 4
        mov    dword ptr [edi + 7ch], esi
        mov    byte ptr [edi + 89h], bl
        push   0
        push   edi
        call   sub_4_E407
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_44E5
        cmp    eax, 4
        jne    SHORT loc_4_452E
        mov    ecx, dword ptr [edi + 7ch]
        mov    ecx, dword ptr [ecx + 24h]
        jecxz  loc_4_452E
loc_4_44C9:
        test   byte ptr [ecx + 1fh], 8
        je     SHORT loc_4_452E
        mov    ecx, dword ptr [ecx + 2ch]
        jecxz  loc_4_44D6
        jmp    SHORT loc_4_44C9
loc_4_44D6:
        push   2
        push   edi
        call   sub_4_E407
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_452E
loc_4_44E5:
        push   0
        push   edi
        call   sub_4_13AC3
        add    esp, 8
        push   edi
        call   sub_4_E48A
        call   sub_4_E63C
        add    esp, 4
        dec    byte ptr [esi + 19h]
        db  08Bh, 004h, 09Dh, 0C0h, 065h, 000h, 000h
        mov    dword ptr [eax], 0
        or     byte ptr [esi + 3eh], 2
        push   edi
        db  0E8h, 0E6h, 003h, 0FBh, 0FFh
        mov    ecx, dword ptr [edi + 7ch]
        add    esp, 108h
        jecxz  loc_4_4527
        and    byte ptr [ecx + 3eh], 0fdh
loc_4_4527:
        xor    eax, eax
loc_4_4529:
        pop    esi
        pop    edi
        pop    ebx
        leave
        ret
loc_4_452E:
        add    esp, 104h
loc_4_4534:
        mov    eax, 0fh
        jmp    SHORT loc_4_4529
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    ebx, 20h
        jae    NEAR PTR loc_4_467A
        db  0F6h, 083h, 08Ch, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_4_467A
        mov    ecx, dword ptr [ebp + 10h]
        cmp    ecx, ebx
        je     NEAR PTR loc_4_467A
        cmp    ecx, 20h
        jae    NEAR PTR loc_4_467A
        db  0F6h, 081h, 08Ch, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_4_467A
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 014h, 09Dh, 0C0h, 065h, 000h, 000h
        mov    esi, dword ptr [edx]
        or     esi, esi
        je     NEAR PTR loc_4_467A
        cmp    dword ptr [esi + 20h], eax
        jne    NEAR PTR loc_4_467A
        db  08Bh, 014h, 08Dh, 0C0h, 065h, 000h, 000h
        mov    edi, dword ptr [edx]
        or     edi, edi
        je     NEAR PTR loc_4_467A
        cmp    dword ptr [edi + 20h], eax
        jne    NEAR PTR loc_4_467A
        test   dword ptr [eax], 4000h
        je     SHORT loc_4_45D6
        db  08Bh, 004h, 09Dh, 0C0h, 065h, 000h, 000h
        bt     dword ptr [eax + 0bh], 2
        sbb    eax, eax
        bt     dword ptr [edx + 0bh], 2
        adc    eax, 0
        je     NEAR PTR loc_4_467A
loc_4_45D6:
        db  08Bh, 004h, 09Dh, 0C0h, 065h, 000h, 000h
        cli
        mov    dword ptr [eax], edi
        mov    dword ptr [edx], esi
        mov    word ptr [esi + 14h], cx
        mov    word ptr [edi + 14h], bx
        push   edi
        push   esi
        mov    ecx, 15h
        add    edi, 44h
        add    esi, 44h
        cld
loc_4_45F8:
        mov    al, byte ptr [esi]
        xchg   byte ptr [edi], al
        mov    byte ptr [esi], al
        inc    edi
        inc    esi
        loop   loc_4_45F8
        pop    esi
        pop    edi
        sti
        mov    edx, edi
        sub    esp, 74h
        mov    edi, esp
        mov    ecx, 1dh
        xor    eax, eax
        rep stosd
        mov    edi, esp
        mov    byte ptr [edi + 4], 5
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [edi + 1ch], eax
        movzx  eax, word ptr [esi + 14h]
        mov    word ptr [edi + 18h], ax
        mov    eax, dword ptr [esi + 1ch]
        push   edx
        push   eax
        push   0
        call   sub_4_2047
        lea    esp, [esp + 8]
        pop    edx
        mov    bl, 10h
        mul    bl
        db  08Dh, 098h, 0F4h, 055h, 000h, 000h
        mov    eax, dword ptr [ebx]
        push   edi
        mov    esi, edx
        call   eax
        add    esp, 4
        mov    edi, esp
        mov    ecx, 1dh
        xor    eax, eax
        rep stosd
        mov    edi, esp
        mov    byte ptr [edi + 4], 5
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [edi + 1ch], eax
        movzx  eax, word ptr [esi + 14h]
        mov    word ptr [edi + 18h], ax
        mov    eax, dword ptr [ebx]
        push   edi
        call   eax
        add    esp, 78h
loc_4_4675:
        pop    esi
        pop    edi
        pop    ebx
        leave
        ret
loc_4_467A:
        mov    eax, 0fh
        jmp    SHORT loc_4_4675
sub_4_4681:
        db  0A1h, 064h, 000h, 000h, 000h
        mov    edx, dword ptr [esp + 4]
        not    edx
        and    edx, eax
        db  089h, 015h, 064h, 000h, 000h, 000h
        ret
sub_4_4695:
        db  0A1h, 064h, 000h, 000h, 000h
        mov    edx, dword ptr [esp + 4]
        db  089h, 015h, 064h, 000h, 000h, 000h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_4_46B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        db  08Bh, 03Dh, 088h, 065h, 000h, 000h
        db  081h, 0FFh, 088h, 065h, 000h, 000h
        mov    esi, dword ptr [ecx + 94h]
        je     SHORT loc_4_4709
        mov    edx, dword ptr [ebp + 0ch]
loc_4_46D0:
        cmp    edi, esi
        je     SHORT loc_4_46FF
        mov    dword ptr [ecx + 94h], edi
        movzx  eax, word ptr [ecx + 6]
        mov    dword ptr [edi + eax*4 + 0d4h], edx
        or     edx, edx
        movzx  eax, word ptr [ecx + 6]
        db  08Bh, 01Dh, 044h, 059h, 000h, 000h
        je     SHORT loc_4_46FA
        or     byte ptr [eax + ebx + 2eh], 4
        jmp    SHORT loc_4_46FF
loc_4_46FA:
        and    byte ptr [eax + ebx + 2eh], 0fbh
loc_4_46FF:
        mov    edi, dword ptr [edi]
        db  081h, 0FFh, 088h, 065h, 000h, 000h
        jne    SHORT loc_4_46D0
loc_4_4709:
        mov    dword ptr [ecx + 94h], esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_4714:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   1
        mov    eax, dword ptr [ebx + 90h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebx + 78h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 10h], eax
        push   4bh
        db  0E8h, 0BAh, 03Eh, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_4_4787
        mov    edi, eax
        add    edi, 10h
        mov    esi, dword ptr [ebx + 78h]
        mov    ecx, 0eh
        rep movsd
        movsw
        movsb
        mov    edx, dword ptr [ebp - 4]
        movzx  eax, word ptr [ebx + 6]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebx + 78h]
        sub    ecx, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], ecx
        movzx  eax, byte ptr [ebx + 89h]
        mov    dword ptr [edx + 0ch], eax
        db  0A1h, 0E0h, 06Ch, 000h, 000h
        mov    dword ptr [edx], eax
        db  089h, 015h, 0E0h, 06Ch, 000h, 000h
loc_4_4787:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebx + 4], al
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [ebx + 18h], 1
        test   byte ptr [eax + 6], 10h
        je     SHORT loc_4_47A5
        mov    word ptr [ebx + 18h], 81h
loc_4_47A5:
        mov    eax, dword ptr [ebp - 8]
        push   ebx
        movzx  ecx, word ptr [eax + 31h]
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        imul   eax, eax, 10001h
        add    ecx, eax
        mov    dword ptr [ebx + 8], ecx
        db  0E8h, 0B8h, 005h, 0FBh, 0FFh
        add    esp, 4
        push   0
        push   ebx
        db  0E8h, 082h, 0F7h, 0FAh, 0FFh
        add    esp, 8
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [edx + 25h]
        mov    dword ptr [eax + 15h], ecx
        mov    eax, dword ptr [ebx + 74h]
        mov    dword ptr [eax + 18h], ecx
        mov    eax, dword ptr [ebx + 74h]
        or     byte ptr [eax + 1fh], 10h
        mov    eax, dword ptr [ebp - 8]
        push   dword ptr [ebp + 10h]
        movzx  eax, byte ptr [eax + 4]
        mov    dword ptr [ebx], eax
        lea    eax, [ebp - 24h]
        mov    dword ptr [ebx + 0ch], 0
        mov    dword ptr [ebx + 24h], 0
        mov    dword ptr [ebx + 28h], eax
        push   dword ptr [ebp + 0ch]
        push   eax
        push   dword ptr [ebx + 78h]
        call   sub_4_2674
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_484B
        push   0
        push   ebx
        mov    eax, dword ptr [ebx + 7ch]
        push   24h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 18h]
        db  0E8h, 060h, 0F3h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_484B
        mov    eax, dword ptr [ebx + 74h]
        mov    word ptr [eax + 20h], 1
        mov    ecx, dword ptr [ebx + 74h]
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [ecx + 0ch], eax
        jmp    SHORT loc_4_485A
loc_4_484B:
        mov    esi, dword ptr [ebx + 7ch]
        push   ebx
        call   sub_4_71C5
        add    esp, 4
        mov    dword ptr [ebx + 7ch], esi
loc_4_485A:
        push   ebx
        db  0E8h, 09Eh, 000h, 0FBh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebx + 8], eax
        push   dword ptr [ebx + 74h]
        mov    word ptr [ebx + 1ah], 0
        push   ebx
        call   sub_4_46B0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_4882:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 0E0h, 06Ch, 000h, 000h
        or     ebx, ebx
        mov    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_4_4969
loc_4_48A0:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 0ch], 1
        jne    SHORT loc_4_48BA
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [ecx + 89h]
        cmp    eax, dword ptr [ebx + 0ch]
        je     SHORT loc_4_48C8
loc_4_48BA:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_4_48C8
        mov    dword ptr [ebp - 8], ebx
        jmp    NEAR PTR loc_4_495E
loc_4_48C8:
        mov    edi, dword ptr [ebx + 4]
        lea    esi, [ebx + 10h]
        mov    eax, dword ptr [edi + 15h]
        mov    dword ptr [ebx + 25h], eax
        mov    eax, dword ptr [edi + 11h]
        mov    dword ptr [ebx + 21h], eax
        mov    ecx, 0eh
        rep movsd
        movsw
        movsb
        mov    eax, dword ptr [ebx + 8]
        db  08Bh, 00Dh, 044h, 059h, 000h, 000h
        and    byte ptr [eax + ecx + 2eh], 0fbh
        cmp    dword ptr [ebp + 0ch], 1
        jne    SHORT loc_4_493B
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 8]
        mov    ecx, dword ptr [ecx + 94h]
        mov    edx, dword ptr [ecx + eax*4 + 0d4h]
        or     edx, edx
        je     SHORT loc_4_493B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 74h], edx
        mov    eax, dword ptr [ebx + 8]
        mov    word ptr [ecx + 6], ax
        mov    dword ptr [ecx + 78h], 0
        mov    esi, dword ptr [ecx + 7ch]
        mov    eax, dword ptr [edx + 10h]
        mov    dword ptr [ecx + 7ch], eax
        push   ecx
        db  0E8h, 065h, 0F9h, 0FAh, 0FFh
        mov    ecx, dword ptr [ebp + 8]
        add    esp, 4
        mov    dword ptr [ecx + 7ch], esi
loc_4_493B:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_494B
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_4_4953
loc_4_494B:
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 0E0h, 06Ch, 000h, 000h
loc_4_4953:
        push   0
        push   ebx
        db  0E8h, 0A5h, 03Ch, 001h, 000h
        add    esp, 8
loc_4_495E:
        mov    ebx, dword ptr [ebp - 4]
        or     ebx, ebx
        jne    NEAR PTR loc_4_48A0
loc_4_4969:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  053h, 056h, 057h, 0CCh, 05Fh, 05Eh, 05Bh, 0C3h
sub_4_4978:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [ebp + 0ch]
        mov    di, word ptr [esi + 0b4h]
        mov    bx, word ptr [esi + 1ah]
        mov    word ptr [esi + 0b4h], ax
        mov    al, byte ptr [esi + 8bh]
        and    eax, 4
        push   eax
        push   dword ptr [esi + 84h]
        lea    eax, [esi + 98h]
        push   eax
        push   esi
        db  0E8h, 0BCh, 0DDh, 0FAh, 0FFh
        add    esp, 10h
        mov    word ptr [esi + 1ah], bx
        mov    word ptr [esi + 0b4h], di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  08Bh, 01Dh, 0B0h, 065h, 000h, 000h
        mov    edx, dword ptr [esi + 90h]
        mov    ecx, dword ptr [edx + 4]
        add    ebx, ecx
        test   byte ptr [edx], 8
        je     NEAR PTR loc_4_4A7A
        db  08Bh, 03Dh, 0B4h, 055h, 000h, 000h
        add    edi, edx
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        jne    SHORT loc_4_4A61
        push   1
        push   1ah
        db  0E8h, 0F3h, 03Bh, 001h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_4A1C
        mov    word ptr [esi + 1ah], 8
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_4_4C73
loc_4_4A1C:
        mov    dword ptr [edi + 14h], eax
        lea    edx, [eax + 16h]
        mov    dword ptr [eax + 10h], edx
        mov    edi, 1
        mov    cl, byte ptr [esi + 89h]
        dec    cl
        shl    edi, cl
        mov    dword ptr [edx], edi
        db  0C7h, 040h, 00Ch, 07Ah, 02Ch, 000h, 000h
        mov    byte ptr [eax + 14h], 30h
        mov    ecx, dword ptr [esi + 90h]
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 4], 0
        push   0
        push   eax
        db  0E8h, 0EFh, 0E4h, 0FAh, 0FFh
        add    esp, 8
        jmp    NEAR PTR loc_4_4C71
loc_4_4A61:
        mov    eax, dword ptr [eax + 10h]
        mov    edx, 1
        mov    cl, byte ptr [esi + 89h]
        dec    cl
        shl    edx, cl
        or     dword ptr [eax], edx
        jmp    NEAR PTR loc_4_4C71
loc_4_4A7A:
        movzx  eax, byte ptr [esi + 89h]
        shl    eax, 2
        db  08Bh, 0B8h, 044h, 066h, 000h, 000h
        add    edi, ecx
        db  003h, 00Dh, 044h, 059h, 000h, 000h
        mov    dword ptr [ebp - 4], ecx
        db  083h, 0B8h, 0BCh, 065h, 000h, 000h, 000h
        je     NEAR PTR loc_4_4BA6
        mov    ax, word ptr [edi + 43h]
        and    ax, 4000h
        add    ax, 2eb2h
        mov    word ptr [edi + 4dh], ax
        movzx  eax, byte ptr [esi + 89h]
        db  08Bh, 004h, 085h, 0BCh, 065h, 000h, 000h
        mov    eax, dword ptr [eax]
        test   byte ptr [eax + 1bh], 10h
        je     SHORT loc_4_4B0F
        mov    ax, word ptr [edi + 43h]
        or     ax, 4000h
        mov    word ptr [edi + 43h], ax
        and    ax, 7fffh
        mov    word ptr [edi + 43h], ax
        mov    word ptr [edi + 4bh], 0ffffh
        mov    word ptr [edi + 49h], 0ffffh
        movzx  eax, byte ptr [esi + 89h]
        or     byte ptr [ebx + eax + 1307h], 1
        movzx  eax, byte ptr [esi + 89h]
        db  0F6h, 080h, 08Bh, 000h, 000h, 000h, 080h
        je     SHORT loc_4_4B26
        or     word ptr [edi + 43h], 1000h
        jmp    SHORT loc_4_4B26
loc_4_4B0F:
        or     word ptr [edi + 43h], 0c000h
        movzx  eax, byte ptr [esi + 89h]
        db  08Bh, 004h, 085h, 0BCh, 065h, 000h, 000h
        mov    dword ptr [edi + 49h], eax
loc_4_4B26:
        movzx  eax, byte ptr [esi + 89h]
        db  08Ah, 088h, 08Bh, 000h, 000h, 000h
        test   cl, 10h
        jne    SHORT loc_4_4B3D
        test   cl, 1
        jne    SHORT loc_4_4B4E
loc_4_4B3D:
        and    ecx, 1
        push   ecx
        dec    eax
        push   eax
        db  0E8h, 0EAh, 01Eh, 001h, 000h
        add    esp, 8
        mov    dword ptr [edi + 45h], eax
loc_4_4B4E:
        db  0A1h, 044h, 059h, 000h, 000h
        mov    eax, dword ptr [eax + 1c8h]
        or     eax, eax
        je     SHORT loc_4_4B60
        mov    dword ptr [edi + 52h], eax
loc_4_4B60:
        cmp    byte ptr [esi + 4], 0
        jne    SHORT loc_4_4B92
        mov    word ptr [edi + 4fh], 2
        mov    byte ptr [edi + 3], 0
        mov    ecx, dword ptr [esi + 94h]
        movzx  eax, byte ptr [esi + 89h]
        lea    eax, [ecx + eax*4 + 4ch]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [eax], 0
        push   ecx
        call   sub_4_61C3
        add    esp, 4
loc_4_4B92:
        movzx  eax, byte ptr [esi + 89h]
        mov    ecx, dword ptr [ebp - 4]
        or     byte ptr [eax + ecx + 2eh], 1
        jmp    NEAR PTR loc_4_4C71
loc_4_4BA6:
        mov    ecx, dword ptr [esi + 94h]
        push   dword ptr [eax + ecx + 4ch]
        call   sub_4_61C3
        add    esp, 4
        mov    ecx, dword ptr [esi + 94h]
        movzx  eax, byte ptr [esi + 89h]
        mov    dword ptr [ecx + eax*4 + 4ch], 0
        mov    ax, word ptr [edi + 4dh]
        movzx  ecx, ax
        and    ecx, 0ffffbfffh
        cmp    ecx, 2eb2h
        jne    SHORT loc_4_4C37
        test   byte ptr [edi + 44h], 10h
        je     SHORT loc_4_4C13
        movzx  eax, byte ptr [esi + 89h]
        db  0F6h, 080h, 08Bh, 000h, 000h, 000h, 001h
        je     SHORT loc_4_4C0B
        dec    eax
        push   eax
        call   sub_4_2719
loc_4_4BFF:
        add    esp, 4
        mov    dword ptr [edi + 45h], eax
        mov    ax, word ptr [edi + 4dh]
        jmp    SHORT loc_4_4C13
loc_4_4C0B:
        mov    word ptr [edi + 43h], 0
        jmp    SHORT loc_4_4C1B
loc_4_4C13:
        and    ax, 4000h
        mov    word ptr [edi + 43h], ax
loc_4_4C1B:
        mov    word ptr [edi + 4bh], 0
        mov    word ptr [edi + 49h], 0
        mov    word ptr [edi + 4dh], 0
        mov    word ptr [edi + 4fh], 2
        mov    byte ptr [edi + 3], 0
loc_4_4C37:
        movzx  eax, byte ptr [esi + 89h]
        mov    ecx, dword ptr [ebp - 4]
        and    byte ptr [eax + ecx + 2eh], 0feh
        movzx  eax, byte ptr [esi + 89h]
        and    byte ptr [ebx + eax + 1307h], 0eeh
        movzx  eax, byte ptr [esi + 89h]
        db  08Ah, 080h, 08Bh, 000h, 000h, 000h
        test   al, 10h
        jne    SHORT loc_4_4C6A
        test   al, 1
        jne    SHORT loc_4_4C71
loc_4_4C6A:
        mov    dword ptr [edi + 45h], 0
loc_4_4C71:
        sub    eax, eax
loc_4_4C73:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    esi, 1
        mov    ecx, dword ptr [eax + 90h]
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 015h, 0B4h, 055h, 000h, 000h
        mov    eax, dword ptr [eax + 10h]
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        db  0A1h, 044h, 059h, 000h, 000h
        add    eax, edi
        mov    dword ptr [ebp - 0ch], eax
        db  0A1h, 0B0h, 065h, 000h, 000h
        add    eax, edi
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [edx + ecx + 14h], 0
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_4_4EFB
        mov    edi, 4
loc_4_4CD0:
        test   byte ptr [ebp - 8], 1
        jbe    NEAR PTR loc_4_4EEA
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 90h]
        mov    ebx, dword ptr [eax + 4]
        db  003h, 09Fh, 044h, 066h, 000h, 000h
        db  08Bh, 087h, 0BCh, 065h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_4_4DF0
        mov    eax, dword ptr [eax]
        test   byte ptr [eax + 1bh], 10h
        je     SHORT loc_4_4D0F
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [esi + eax + 1307h], 1
        je     SHORT loc_4_4D15
loc_4_4D0F:
        test   byte ptr [ebx + 44h], 80h
        jne    SHORT loc_4_4D35
loc_4_4D15:
        movzx  eax, word ptr [ebx + 4dh]
        and    eax, 0ffffbfffh
        cmp    eax, 2eb2h
        je     SHORT loc_4_4D35
        mov    ax, word ptr [ebx + 43h]
        and    ax, 4000h
        add    ax, 2eb2h
        mov    word ptr [ebx + 4dh], ax
loc_4_4D35:
        db  08Bh, 087h, 0BCh, 065h, 000h, 000h
        mov    eax, dword ptr [eax]
        test   byte ptr [eax + 1bh], 10h
        je     SHORT loc_4_4D7F
        mov    ax, word ptr [ebx + 43h]
        or     ax, 4000h
        mov    word ptr [ebx + 43h], ax
loc_4_4D4F:
        and    ax, 7fffh
        mov    word ptr [ebx + 43h], ax
        mov    word ptr [ebx + 4bh], 0ffffh
        mov    word ptr [ebx + 49h], 0ffffh
        mov    eax, dword ptr [ebp - 4]
        or     byte ptr [esi + eax + 1307h], 1
        db  0F6h, 086h, 08Bh, 000h, 000h, 000h, 080h
        je     SHORT loc_4_4D8E
        or     word ptr [ebx + 43h], 1000h
        jmp    SHORT loc_4_4D8E
loc_4_4D7F:
        or     word ptr [ebx + 43h], 0c000h
        db  08Bh, 087h, 0BCh, 065h, 000h, 000h
        mov    dword ptr [ebx + 49h], eax
loc_4_4D8E:
        db  08Ah, 086h, 08Bh, 000h, 000h, 000h
        test   al, 10h
        jne    SHORT loc_4_4D9C
        test   al, 1
        jne    SHORT loc_4_4DAF
loc_4_4D9C:
        and    eax, 1
        push   eax
        lea    eax, [esi - 1]
        push   eax
        db  0E8h, 089h, 01Ch, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebx + 45h], eax
loc_4_4DAF:
        db  0A1h, 044h, 059h, 000h, 000h
        mov    eax, dword ptr [eax + 1c8h]
        or     eax, eax
        je     SHORT loc_4_4DC1
        mov    dword ptr [ebx + 52h], eax
loc_4_4DC1:
        mov    byte ptr [ebx + 3], 0
        mov    eax, dword ptr [ebp - 0ch]
        or     byte ptr [esi + eax + 2eh], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 94h]
        lea    ecx, [edi + eax + 4ch]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ecx], 0
        push   eax
        call   sub_4_61C3
        add    esp, 4
        jmp    NEAR PTR loc_4_4EEA
loc_4_4DF0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 94h]
        push   dword ptr [edi + eax + 4ch]
        call   sub_4_61C3
        mov    eax, dword ptr [ebp + 0ch]
        add    esp, 4
        mov    eax, dword ptr [eax + 94h]
        mov    dword ptr [edi + eax + 4ch], 0
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [esi + eax + 1307h], 1
        jne    SHORT loc_4_4E2D
        test   byte ptr [ebx + 44h], 80h
        je     NEAR PTR loc_4_4EB8
loc_4_4E2D:
        mov    ax, word ptr [ebx + 4dh]
        movzx  ecx, ax
        and    ecx, 0ffffbfffh
        cmp    ecx, 2eb2h
        jne    SHORT loc_4_4EB8
        test   byte ptr [ebx + 44h], 10h
        je     SHORT loc_4_4E6E
        db  0F6h, 086h, 08Bh, 000h, 000h, 000h, 001h
        je     SHORT loc_4_4E66
        lea    eax, [esi - 1]
        push   eax
        call   sub_4_2719
        add    esp, 4
        mov    dword ptr [ebx + 45h], eax
        mov    ax, word ptr [ebx + 4dh]
        jmp    SHORT loc_4_4E6E
loc_4_4E66:
        mov    word ptr [ebx + 43h], 0
        jmp    SHORT loc_4_4E76
loc_4_4E6E:
        and    ax, 4000h
        mov    word ptr [ebx + 43h], ax
loc_4_4E76:
        test   byte ptr [ebx + 44h], 10h
        je     SHORT loc_4_4E94
        db  0F6h, 086h, 08Bh, 000h, 000h, 000h, 001h
        je     SHORT loc_4_4E94
        lea    eax, [esi - 1]
        push   eax
        call   sub_4_2719
        add    esp, 4
        mov    dword ptr [ebx + 45h], eax
loc_4_4E94:
        mov    ax, word ptr [ebx + 4dh]
        and    ax, 4000h
        mov    word ptr [ebx + 43h], ax
        mov    word ptr [ebx + 4dh], 0
        mov    word ptr [ebx + 4bh], 0
        mov    word ptr [ebx + 49h], 0
        mov    word ptr [ebx + 4fh], 2
loc_4_4EB8:
        mov    eax, dword ptr [ebp + 0ch]
        movzx  ecx, byte ptr [eax + 89h]
        mov    eax, dword ptr [ebp - 4]
        and    byte ptr [ecx + eax + 1307h], 0eeh
        mov    eax, dword ptr [ebp - 0ch]
        and    byte ptr [esi + eax + 2eh], 0feh
        db  08Ah, 086h, 08Bh, 000h, 000h, 000h
        test   al, 10h
        jne    SHORT loc_4_4EE3
        test   al, 1
        jne    SHORT loc_4_4EEA
loc_4_4EE3:
        mov    dword ptr [ebx + 45h], 0
loc_4_4EEA:
        shr    dword ptr [ebp - 8], 1
        add    edi, 4
        inc    esi
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_4_4CD0
loc_4_4EFB:
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 0FBh, 036h, 001h, 000h
        add    esp, 8
        push   dword ptr [ebp + 0ch]
        db  0E8h, 0A5h, 0E8h, 0FAh, 0FFh
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_4F1A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
loc_4_4F28:
        db  08Bh, 004h, 0B5h, 0E0h, 066h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_4_4F7F
        sub    ebx, ebx
loc_4_4F38:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + ebx*8]
        or     ecx, ecx
        je     SHORT loc_4_4F6D
        mov    edx, dword ptr [ecx + 10h]
        or     edx, edx
        je     SHORT loc_4_4F6D
        mov    al, byte ptr [ebp + 10h]
        test   byte ptr [edx + 1bh], al
        je     SHORT loc_4_4F6D
        mov    dword ptr [edi + 74h], ecx
        lea    eax, [esi + 2]
        shl    ax, 8
        add    ax, bx
        mov    word ptr [edi + 6], ax
        push   edi
        call   dword ptr [ebp + 0ch]
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_4FA0
loc_4_4F6D:
        inc    ebx
        cmp    ebx, 0ffh
        jle    SHORT loc_4_4F38
        inc    esi
        cmp    esi, 0ffh
        jle    SHORT loc_4_4F28
loc_4_4F7F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 28h], eax
        mov    al, byte ptr [ebp + 10h]
        mov    byte ptr [edi + 4], al
        db  068h, 0A7h, 02Fh, 000h, 000h
        push   edi
        call   sub_4_2547
        add    esp, 8
loc_4_4F99:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_4FA0:
        mov    eax, 0ffffffffh
        jmp    SHORT loc_4_4F99
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    ebx, ebx
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [ebp - 8], eax
        db  039h, 01Dh, 0D8h, 066h, 000h, 000h
        movzx  ax, byte ptr [esi + 4]
        mov    word ptr [ebp - 2], ax
        jle    SHORT loc_4_500C
        sub    edi, edi
loc_4_4FCE:
        mov    eax, dword ptr [esi + 94h]
        mov    ecx, dword ptr [edi + eax + 0d4h]
        or     ecx, ecx
        je     SHORT loc_4_5000
        mov    eax, dword ptr [ecx + 10h]
        or     eax, eax
        je     SHORT loc_4_5000
        movzx  eax, byte ptr [eax + 1bh]
        movzx  edx, word ptr [ebp - 2]
        test   edx, eax
        je     SHORT loc_4_5000
        mov    dword ptr [esi + 74h], ecx
        push   esi
        call   dword ptr [ebp - 8]
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_5015
loc_4_5000:
        add    edi, 4
        inc    ebx
        db  03Bh, 01Dh, 0D8h, 066h, 000h, 000h
        jl     SHORT loc_4_4FCE
loc_4_500C:
        sub    eax, eax
loc_4_500E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_5015:
        mov    eax, 0ffffffffh
        jmp    SHORT loc_4_500E
sub_4_501C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp + 0ch]
        mov    byte ptr [eax + 89h], cl
        db  068h, 03Ah, 030h, 000h, 000h
        push   eax
        call   sub_4_2547
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 94h]
        mov    al, byte ptr [eax + 89h]
        cmp    byte ptr [ecx + 0ah], al
        mov    eax, 0ffffffffh
        je     SHORT loc_4_5058
        sub    eax, eax
loc_4_5058:
        pop    ebp
        ret
sub_4_505A:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 94h]
        cmp    dword ptr [eax + 48h], 0
        jne    SHORT loc_4_50BF
        push   1
        push   16h
        db  0E8h, 082h, 035h, 001h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_508A
        mov    word ptr [esi + 1ah], 8
        mov    eax, 0ffffffffh
        jmp    SHORT loc_4_50C1
loc_4_508A:
        mov    ecx, dword ptr [esi + 94h]
        mov    dword ptr [ecx + 48h], eax
        lea    ecx, [eax + 16h]
        mov    dword ptr [eax + 10h], ecx
        db  0C7h, 040h, 00Ch, 0C4h, 030h, 000h, 000h
        mov    byte ptr [eax + 14h], 30h
        mov    ecx, dword ptr [esi + 90h]
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 4], 0
        push   0
        push   eax
        db  0E8h, 08Ch, 0DEh, 0FAh, 0FFh
        add    esp, 8
loc_4_50BF:
        sub    eax, eax
loc_4_50C1:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        push   0
        mov    eax, dword ptr [esi + 94h]
        push   dword ptr [ebp + 8]
        mov    bl, byte ptr [eax + 0bh]
        db  032h, 01Dh, 0D8h, 06Ch, 000h, 000h
        mov    dword ptr [eax + 48h], 0
        mov    dword ptr [esi + 30h], 0
        db  0E8h, 00Dh, 035h, 001h, 000h
        add    esp, 8
        mov    byte ptr [esi + 89h], 0
        or     bl, bl
        je     SHORT loc_4_513F
loc_4_5101:
        test   bl, 1
        je     SHORT loc_4_5133
        movzx  eax, byte ptr [esi + 89h]
        shl    eax, 2
        db  083h, 0B8h, 034h, 06Eh, 000h, 000h, 000h
        jne    SHORT loc_4_512A
        db  083h, 0B8h, 058h, 06Eh, 000h, 000h, 000h
        jne    SHORT loc_4_512A
        push   esi
        call   sub_4_590D
        jmp    SHORT loc_4_5130
loc_4_512A:
        push   esi
        call   sub_4_5317
loc_4_5130:
        add    esp, 4
loc_4_5133:
        shr    bl, 1
        inc    byte ptr [esi + 89h]
        or     bl, bl
        jne    SHORT loc_4_5101
loc_4_513F:
        push   esi
        db  0E8h, 070h, 0E6h, 0FAh, 0FFh
        add    esp, 4
        pop    esi
        pop    ebx
        pop    ebp
        ret
_PTEXT ends
        end
