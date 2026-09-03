; oak_pcode_03.asm -- OAK object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    eax, dword ptr [ebp + 18h]
        db  0A3h, 019h, 001h, 000h, 000h
        movzx  eax, word ptr [ebp + 14h]
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
        ret
_PTEXT ends
        end
