; pccard_icode_06.asm -- PCCARD object 6 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

sub_6_0000:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 02Ch, 000h
        push   esi
        xor    edx, edx
        push   edi
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        mov    edi, esi
        xor    eax, eax
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 012h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 024h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 040h, 000h
        mov    ecx, 8
        rep stosd
        stosb
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        db  066h, 0A3h, 016h, 000h, 000h, 000h
        mov    ecx, 6
        db  066h, 0FFh, 005h, 000h, 000h, 000h, 000h
        xor    eax, eax
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        rep stosd
        stosw
        stosb
        pop    edi
        db  088h, 015h, 01Ah, 000h, 000h, 000h
        db  089h, 035h, 010h, 000h, 000h, 000h
        pop    esi
        ret
_ITEXT ends
        end
