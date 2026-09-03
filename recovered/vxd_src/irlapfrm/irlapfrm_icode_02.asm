; irlapfrm_icode_02.asm -- IRLAPFRM object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0013:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_0035
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0032:
        add    esp, 8
loc_2_0035:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_003D
loc_2_003D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_ITEXT ends
        end
