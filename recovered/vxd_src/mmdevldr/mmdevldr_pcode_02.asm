; mmdevldr_pcode_02.asm -- MMDEVLDR object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
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
sub_2_0014:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_2_0024:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_0034:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_2_0044:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        int3
        int3
sub_2_004C:
        db  0CDh, 020h, 02Fh, 080h, 033h, 000h
        int3
        int3
sub_2_0054:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        int3
        int3
sub_2_005C:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        int3
        int3
sub_2_0064:
        db  0CDh, 020h, 03Dh, 080h, 033h, 000h
        int3
        int3
sub_2_006C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_007C:
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
sub_2_0090:
        db  0CDh, 020h, 030h, 080h, 033h, 000h
        int3
        int3
sub_2_0098:
        db  0CDh, 020h, 04Ch, 081h, 001h, 000h
        int3
        int3
sub_2_00A0:
        db  0CDh, 020h, 04Ah, 081h, 001h, 000h
        int3
        int3
sub_2_00A8:
        db  0CDh, 020h, 050h, 081h, 001h, 000h
        int3
        int3
sub_2_00B0:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        int3
        int3
sub_2_00B8:
        db  0CDh, 020h, 04Bh, 081h, 001h, 000h
        int3
        int3
sub_2_00C0:
        db  0CDh, 020h, 055h, 081h, 001h, 000h
        int3
        int3
sub_2_00C8:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_00D8:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
_PTEXT ends
        end
