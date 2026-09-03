; vxdldr_icode_03.asm -- VXDLDR object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_3_0000:
        xor    ebp, ebp
        db  0BFh, 0E0h, 000h, 000h, 000h
        jmp    NEAR PTR loc_3_00FF
loc_3_000C:
        cmp    ebp, 14h
        jbe    SHORT loc_3_0035
        xor    ebp, ebp
        db  068h, 00Fh, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0CDh, 020h, 0C4h, 000h, 001h, 000h
        db  068h, 00Ch, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        je     NEAR PTR loc_3_0109
loc_3_0035:
        mov    ax, word ptr [edi + 9]
        mov    esi, dword ptr [edi + 5]
        mov    dl, byte ptr [edi + 4]
        db  068h, 036h, 001h, 000h, 000h
loc_3_0044:
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        push   dword ptr [edi + 0bh]
sub_3_004D:
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 03Dh, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        test   esi, esi
        jns    SHORT loc_3_006D
        db  068h, 061h, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_3_006D:
        jle    SHORT loc_3_007A
        db  068h, 06Eh, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_3_007A:
        jne    SHORT loc_3_0087
        db  068h, 07Ch, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_3_0087:
        inc    ebp
        cmp    dword ptr [edi + 0fh], 444c5658h
        je     SHORT sub_3_009E
        db  068h, 087h, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        jmp    SHORT loc_3_00FF
sub_3_009E:
        mov    ecx, dword ptr [edi + 13h]
        add    ebp, ecx
        jecxz  loc_3_00FF
        mov    esi, dword ptr [edi + 17h]
        test   esi, esi
        jne    SHORT loc_3_00B9
        db  068h, 0AEh, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        jmp    SHORT loc_3_00FF
loc_3_00B9:
        mov    eax, dword ptr [esi]
        mov    ebx, dword ptr [esi + 4]
        mov    edx, dword ptr [esi + 8]
        db  068h, 0C1h, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        cmp    dword ptr [esi + 0ch], 0
loc_3_00D0:
        je     SHORT loc_3_00DD
        db  068h, 0DCh, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_3_00DD:
        inc    edx
        cmp    edx, 20h
        jbe    SHORT loc_3_00EE
        db  068h, 0E8h, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_3_00EE:
        db  068h, 00Ch, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        inc    ebp
        add    esi, 10h
        loop   loc_3_00B9
loc_3_00FF:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    NEAR PTR loc_3_000C
loc_3_0109:
        ret
        db  000h, 000h, 00Dh, 00Ah, 000h, 050h, 072h, 065h, 073h, 073h, 020h, 053h, 050h, 041h, 043h, 045h
        db  020h, 074h, 06Fh, 020h, 073h, 065h, 065h, 020h, 06Dh, 06Fh, 072h, 065h, 02Ch, 020h, 045h, 053h
        db  043h, 020h, 074h, 06Fh, 020h, 071h, 075h, 069h, 074h, 03Ah, 020h, 000h, 023h, 045h, 044h, 049h
        db  020h, 05Bh, 000h, 05Dh, 020h, 044h, 065h, 076h, 069h, 063h, 065h, 020h, 049h, 044h, 020h, 03Dh
        db  020h, 023h, 041h, 058h, 02Ch, 020h
loc_3_0150:
        push   edx
        inc    bx
        outsd
        db  075h, 06Eh
        db  074h, 020h
        cmp    eax, 4c442320h
        sub    al, 20h
        add    byte ptr [esp + eax*2 + 42h], al
        and    byte ptr [53452320h], bh
        dec    ecx
        or     eax, 544e000ah
        and    byte ptr [ebp + 69h], ch
        outsb
        imul   esi, dword ptr [eax + 6fh], 0a0d7472h
        add    byte ptr [ebp + 6eh], dl
        insb
        outsd
        popad
        or     eax, 2020000ah
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        and    byte ptr [ecx + eax*2 + 4dh], al
        inc    ecx
        inc    edi
        inc    ebp
loc_3_0194:
        inc    esp
        and    byte ptr [4142202dh], ch
        inc    esp
_ITEXT ends
        end
