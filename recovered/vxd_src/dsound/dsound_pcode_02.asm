; dsound_pcode_02.asm -- DSOUND object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
        db  0CDh, 020h, 053h, 080h, 001h, 000h
        int3
        int3
sub_2_0008:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  0CCh
sub_2_0014:
        db  0CDh, 020h, 055h, 080h, 001h, 000h
        int3
        int3
sub_2_001C:
        db  0CDh, 020h, 01Dh, 081h, 001h, 000h
        int3
        int3
sub_2_0024:
        db  0CDh, 020h, 01Fh, 081h, 001h, 000h
        int3
        int3
sub_2_002C:
        db  0CDh, 020h, 063h, 080h, 001h, 000h
        int3
        int3
sub_2_0034:
        db  0CDh, 020h, 064h, 080h, 001h, 000h
        int3
        int3
sub_2_003C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_004C:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_2_005C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_006C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
        db  0CCh, 0CCh
sub_2_0080:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_0090:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        mov    esi, ecx
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  0CCh
sub_2_00A4:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_2_00B4:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_2_00BC:
        pop    edx
        pop    eax
        pop    ecx
        push   esi
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
sub_2_00D0:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
_PTEXT ends
        end
