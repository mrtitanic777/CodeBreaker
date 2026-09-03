; compaq_pcode_03.asm -- COMPAQ object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    eax, dword ptr [ebp]
        cmp    ax, 0
        je     SHORT loc_3_0014
        cmp    ax, 1
        je     SHORT loc_3_0056
        jmp    NEAR PTR loc_3_00A0
loc_3_0014:
        shr    eax, 0ah
        db  0A3h, 034h, 002h, 000h, 000h
        mov    eax, dword ptr [ebp + 18h]
        shl    eax, 0ah
        db  0A3h, 040h, 002h, 000h, 000h
        movzx  eax, word ptr [ebp + 14h]
        or     eax, eax
        je     SHORT loc_3_0035
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
loc_3_0035:
        mov    eax, dword ptr [ebp + 14h]
        shr    eax, 10h
        or     eax, eax
        je     SHORT loc_3_0045
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
loc_3_0045:
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0A3h, 030h, 002h, 000h, 000h
        jmp    SHORT loc_3_00A0
loc_3_0056:
        mov    ecx, dword ptr [ebp + 18h]
        push   0
        db  068h, 0CCh, 002h, 000h, 000h
        db  068h, 04Bh, 002h, 000h, 000h
        push   3
        db  068h, 0D0h, 002h, 000h, 000h
        push   0
        push   ecx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        mov    ecx, eax
        cmp    eax, 0
        jne    SHORT loc_3_009D
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    edi, eax
        db  0BEh, 04Bh, 002h, 000h, 000h
        mov    ecx, 20h
        cld
        rep movsd
        mov    ecx, 0
loc_3_009D:
        mov    dword ptr [ebp + 18h], ecx
loc_3_00A0:
        ret
_PTEXT ends
        end
