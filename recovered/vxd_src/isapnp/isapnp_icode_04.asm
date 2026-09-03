; isapnp_icode_04.asm -- ISAPNP object 4 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

sub_4_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0CDh, 020h, 000h, 000h, 033h, 000h
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 400h
        sbb    eax, eax
        mov    esp, ebp
        inc    eax
        pop    ebp
        ret
_ITEXT ends
        end
