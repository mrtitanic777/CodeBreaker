; vmd_icode_02.asm -- VMD object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0BEh, 007h, 006h, 000h, 000h
        xor    edx, edx
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jae    SHORT loc_2_0015
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_2_0015:
        mov    ecx, eax
        shr    ecx, 10h
        mov    edx, eax
        movzx  edx, dx
        mov    eax, 33h
        db  0CDh, 020h, 045h, 080h, 001h, 000h
loc_2_002A:
        test   edx, edx
        je     SHORT loc_2_008A
        db  0C6h, 005h, 098h, 00Ah, 000h, 000h, 0FFh
        cmp    dh, 2
        jne    SHORT loc_2_0054
        xor    eax, eax
        db  0BFh, 03Ch, 002h, 000h, 000h
        xor    esi, esi
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        or     al, al
        je     SHORT loc_2_004F
        mov    al, 21h
loc_2_004F:
        db  0A2h, 09Eh, 00Ah, 000h, 000h
loc_2_0054:
        cmp    dh, 4
        jne    SHORT loc_2_0075
        mov    eax, 1
        db  0BFh, 04Ah, 002h, 000h, 000h
        xor    esi, esi
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        or     al, al
        je     SHORT loc_2_0075
        db  0FEh, 005h, 09Fh, 00Ah, 000h, 000h
loc_2_0075:
        cmp    dh, 0
        je     SHORT loc_2_008A
        test   dl, dl
        je     SHORT loc_2_008A
        movzx  eax, dl
        movzx  ecx, dh
        db  0CDh, 020h, 001h, 000h, 00Ch, 000h
loc_2_008A:
        push   0
        push   64h
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        je     NEAR PTR loc_2_015A
        db  0A3h, 094h, 00Ah, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 00Ah, 000h
        cmp    eax, 30ah
        jb     SHORT loc_2_00B7
        db  0C6h, 005h, 099h, 00Ah, 000h, 000h, 0FFh
loc_2_00B7:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 1607h
        mov    word ptr [ebp + 10h], 0ch
        mov    word ptr [ebp + 18h], 1
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 1ch]
        mov    cx, word ptr [ebp + 3ch]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 4]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        test   eax, eax
        jne    SHORT loc_2_0129
        db  089h, 00Dh, 090h, 00Ah, 000h, 000h
        db  0BEh, 0F0h, 001h, 000h, 000h
        mov    eax, 33h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        jmp    SHORT loc_2_0139
loc_2_0129:
        mov    eax, 33h
        db  0BEh, 020h, 003h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
loc_2_0139:
        db  0BEh, 00Bh, 007h, 000h, 000h
        xor    edx, edx
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        db  066h, 0A3h, 09Ch, 00Ah, 000h, 000h
        shr    eax, 10h
        db  066h, 0A3h, 09Ah, 00Ah, 000h, 000h
        db  0E9h, 0A6h, 0FEh, 0FDh, 0FFh
loc_2_015A:
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_2_0160:
        db  080h, 03Dh, 098h, 00Ah, 000h, 000h, 0FFh
        jne    NEAR PTR loc_2_01FF
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 1607h
        mov    word ptr [ebp + 10h], 0ch
        mov    word ptr [ebp + 18h], 0
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 18h]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        test   eax, eax
        jne    SHORT loc_2_01FF
        movzx  edi, word ptr [0ceh]
        mov    eax, edi
        dec    edi
        shl    edi, 4
        cmp    byte ptr [edi], 4dh
        jne    SHORT loc_2_0201
        cmp    word ptr [edi + 1], ax
        jne    SHORT loc_2_0201
        movzx  ecx, word ptr [edi + 3]
        shl    ecx, 4
        add    edi, 10h
        db  0BEh, 020h, 002h, 000h, 000h
        mov    dword ptr [esi + 8], edi
        mov    dword ptr [esi + 0ch], ecx
        mov    dword ptr [esi + 10h], 200h
        push   0
        push   esi
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_2_020E
loc_2_01FF:
        clc
        ret
loc_2_0201:
        db  0BEh, 034h, 002h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 015h, 000h
        jmp    SHORT loc_2_01FF
loc_2_020E:
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_2_0214:
        db  089h, 01Dh, 004h, 00Bh, 000h, 000h
        jmp    NEAR PTR loc_2_002A
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h
loc_2_0234:
        dec    ebp
        push   ebx
        and    al, 4dh
        dec    edi
        push   ebp
        push   ebx
        inc    ebp
loc_2_023C:
        dec    ebp
        outsd
        db  075h, 073h
        push   ebx
        outsd
        db  066h, 074h, 049h
        outsb
        imul   esi, dword ptr [eax + eax + 4dh], 6573756fh
        dec    eax
        popad
        db  072h, 064h
        push   edx
        db  065h, 073h, 065h
        db  074h, 041h
        db  074h, 049h
        outsb
        db  069h, 074h, 000h
_ITEXT ends
        end
