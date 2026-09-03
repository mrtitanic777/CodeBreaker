; sb16_icode_06.asm -- SB16 object 6 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        push   word ptr [ebp + 8]
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_6_0024
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi], eax
        mov    eax, 0
        clc
        jmp    SHORT loc_6_002A
loc_6_0024:
        stc
        mov    eax, 1
loc_6_002A:
        pop    esi
        leave
        ret
_ITEXT ends
        end
