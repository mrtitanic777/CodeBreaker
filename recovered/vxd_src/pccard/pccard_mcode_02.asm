; pccard_mcode_02.asm -- PCCARD object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 015h, 000h
loc_2_0010:
        push   eax
        inc    ebx
        dec    ebp
        inc    ebx
        dec    ecx
        inc    ecx
        and    byte ptr [ebx + 61h], al
        db  072h, 064h
        and    byte ptr [ebx + 65h], dl
        db  072h, 076h
        db  069h, 063h, 065h, 073h, 000h
_LMSGTABLE ends
        end
