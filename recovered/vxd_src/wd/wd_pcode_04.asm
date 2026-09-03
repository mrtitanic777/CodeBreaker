; wd_pcode_04.asm -- WD object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        db  0C6h, 005h, 058h, 001h, 000h, 000h, 0FFh
        mov    eax, dword ptr [ebp + 18h]
        db  0A3h, 054h, 001h, 000h, 000h
        movzx  eax, word ptr [ebp + 14h]
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
        ret
_PTEXT ends
        end
