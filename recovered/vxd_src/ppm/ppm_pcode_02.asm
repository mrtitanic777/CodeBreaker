; ppm_pcode_02.asm -- PPM object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_2_0010:
        db  0CDh, 020h, 001h, 080h, 02Bh, 000h
        int3
        int3
sub_2_0018:
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
sub_2_002C:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  0CCh
sub_2_0038:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_0048:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_2_0058:
        db  0CDh, 020h, 022h, 080h, 033h, 000h
        int3
        int3
sub_2_0060:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        int3
        int3
sub_2_0068:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        int3
        int3
sub_2_0070:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        int3
        int3
sub_2_0078:
        db  0CDh, 020h, 022h, 080h, 02Bh, 000h
        int3
        int3
sub_2_0080:
        db  0CDh, 020h, 021h, 080h, 02Bh, 000h
        int3
        int3
sub_2_0088:
        db  0CDh, 020h, 019h, 080h, 02Bh, 000h
        int3
        int3
sub_2_0090:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_2_0098:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_00A8:
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
sub_2_00BC:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_2_00CC:
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
sub_2_00E0:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
_PTEXT ends
        end
