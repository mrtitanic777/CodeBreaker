; vtdapi_icode_02.asm -- VTDAPI object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        push   edx
        db  08Bh, 015h, 0E4h, 002h, 000h, 000h
        db  068h, 020h, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  08Bh, 015h, 0D8h, 002h, 000h, 000h
        db  068h, 036h, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        or     edx, edx
        db  00Fh, 084h, 0F0h, 000h, 000h, 000h
        db  068h, 04Ah, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    esi, edx
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        db  00Fh, 084h, 0D7h, 000h, 000h, 000h
        db  068h, 056h, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    edx, dword ptr [eax]
        db  068h, 063h, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    edx, dword ptr [eax + 4]
        db  068h, 077h, 001h, 000h, 000h
_ITEXT ends
        end
