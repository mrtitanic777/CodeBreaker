; irlamp_pcode_03.asm -- IRLAMP object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_3_0000:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 004h, 000h, 005h, 000h
        cmc
        sbb    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_3_0017:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 005h, 000h
        cmc
        sbb    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_PTEXT ends
        end
