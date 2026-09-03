; irlamp_icode_02.asm -- IRLAMP object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0CDh, 020h, 000h, 000h, 0A2h, 004h
        jb     SHORT loc_2_000F
        call   sub_2_0014
        clc
        ret
loc_2_000F:
        stc
        ret
        db  000h, 000h, 000h
sub_2_0014:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0024:
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_2_0031
loc_2_0031:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
_ITEXT ends
        end
