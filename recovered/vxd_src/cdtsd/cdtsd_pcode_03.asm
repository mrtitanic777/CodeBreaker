; cdtsd_pcode_03.asm -- CDTSD object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        sub    esp, 4
        mov    esi, dword ptr [ebx + 0ch]
        cmp    byte ptr [esi + 40h], 5
        jne    NEAR PTR loc_3_00E5
        test   dword ptr [esi + 1ch], 23h
        je     SHORT loc_3_0021
        mov    eax, dword ptr [esi + 1ch]
        jmp    NEAR PTR loc_3_00E5
loc_3_0021:
        db  08Bh, 015h, 060h, 000h, 000h, 000h
        mov    al, 5
        call   sub_3_0194
        or     ah, ah
        je     NEAR PTR loc_3_00F4
        bsf    eax, dword ptr [edx + 48h]
        je     NEAR PTR loc_3_00E9
        push   eax
        push   edx
        db  08Dh, 015h, 000h, 000h, 000h, 000h
        add    al, 41h
        mov    byte ptr [esp + 8], al
        mov    byte ptr [esp + 9], al
        mov    byte ptr [esp + 0ah], 0
        lea    ecx, [esp + 8]
        push   0
        push   3
        push   ecx
        push   1
        push   edx
        push   0
        push   dword ptr [esi + 70h]
        db  0CDh, 020h, 03Fh, 000h, 033h, 000h
        add    esp, 1ch
        pop    edx
        pop    eax
loc_3_0073:
        btr    dword ptr [edx + 48h], eax
        mov    byte ptr [esi + 15h], al
        db  0FEh, 005h, 01Ch, 000h, 000h, 000h
        or     dword ptr [esi + 20h], 0c000h
        mov    dword ptr [esi + 28h], 0
        push   ebp
        sub    esp, 18h
        mov    ebp, esp
        mov    dword ptr [ebp + 4], esi
        db  0C7h, 045h, 008h, 078h, 000h, 000h, 000h
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [ebp + 16h], al
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp + 0ch], eax
        xor    eax, eax
        mov    word ptr [ebp + 10h], ax
        mov    eax, dword ptr [esi + 1ch]
        and    eax, 0ffffffe3h
        mov    dword ptr [ebp + 12h], eax
        mov    word ptr [ebp], 5
        push   ebp
        db  0FFh, 015h, 044h, 000h, 000h, 000h
        add    esp, 10h
        mov    ebp, esp
        mov    word ptr [ebp], 6
        mov    dword ptr [ebp + 4], esi
        mov    al, byte ptr [esi + 15h]
        mov    byte ptr [ebp + 8], al
        push   ebp
        db  0FFh, 015h, 044h, 000h, 000h, 000h
        add    esp, 10h
        pop    ebp
loc_3_00E5:
        add    esp, 4
        ret
loc_3_00E9:
        xor    al, al
        call   sub_3_0194
        or     ah, ah
        jne    SHORT loc_3_00E5
loc_3_00F4:
        movzx  eax, al
        jmp    NEAR PTR loc_3_0073
loc_3_00FC:
        mov    esi, dword ptr [ebx + 0ch]
        cmp    byte ptr [esi + 40h], 5
        jne    SHORT loc_3_010B
        db  0FEh, 00Dh, 01Ch, 000h, 000h, 000h
loc_3_010B:
        ret
loc_3_010C:
        db  080h, 03Dh, 01Ch, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_011B
        mov    word ptr [ebx + 2], 0ffffh
loc_3_011B:
        ret
        db  060h, 0CDh, 020h, 001h, 000h, 001h, 000h, 08Bh, 073h, 008h, 083h, 0ECh, 06Ch, 057h, 08Dh, 07Ch
        db  024h, 004h, 0CDh, 020h, 08Dh, 000h, 001h, 000h, 05Fh, 0CDh, 020h, 082h, 000h, 001h, 000h, 0C6h
        db  046h, 01Dh, 052h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 00Fh, 0B7h
        db  05Eh, 038h, 0C1h, 0E3h, 004h, 00Fh, 0B7h, 04Eh, 010h, 003h, 0D9h, 0CDh, 020h, 086h, 000h, 001h
        db  000h, 056h, 08Dh, 074h, 024h, 004h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 05Eh, 083h, 0C4h, 06Ch
        db  00Fh, 0B7h, 043h, 016h, 00Fh, 0B7h, 053h, 018h, 0C1h, 0E2h, 004h, 003h, 0C2h, 0BFh, 00Ch, 001h
        db  000h, 000h, 0BAh, 058h, 000h, 000h, 000h, 00Fh, 0B6h, 04Bh, 021h, 088h, 00Dh, 074h, 001h, 000h
        db  000h, 0ABh, 003h, 0C2h, 0E2h, 0FBh, 061h, 0C3h
sub_3_0194:
        push   ebx
        sub    esp, 0ch
        mov    ebx, esp
        mov    dword ptr [ebx + 4], esi
        mov    byte ptr [ebx + 0ah], al
        mov    word ptr [ebx], 10h
        push   ebx
        db  0FFh, 015h, 044h, 000h, 000h, 000h
        mov    ax, word ptr [ebx + 2]
        or     ah, al
        mov    al, byte ptr [ebx + 8]
        add    esp, 10h
        pop    ebx
        ret
        db  000h, 000h
_PTEXT ends
        end
