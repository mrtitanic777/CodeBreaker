; lptenum_pcode_02.asm -- LPTENUM object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
        push   ebp
        mov    ebp, esp
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    ebp
        ret
_PTEXT ends
        end
