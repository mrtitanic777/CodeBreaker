; javasup_icode_03.asm -- JAVASUP object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_3_0000:
        db  0CDh, 020h, 000h, 000h, 02Ah, 000h
        cmp    eax, 103h
        cmc
        ret
_ITEXT ends
        end
