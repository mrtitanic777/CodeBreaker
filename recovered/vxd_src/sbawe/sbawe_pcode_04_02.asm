; sbawe_pcode_04_02.asm -- SBAWE object 4 segment _PTEXT  (part 2/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_4D5C:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_4_4D64:
        db  0CDh, 020h, 07Bh, 081h, 001h, 000h
        int3
        int3
sub_4_4D6C:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_4_4D74:
        pop    ecx
        pop    eax
        pop    edx
        push   ebx
        mov    ebx, eax
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        sbb    edx, edx
        and    eax, edx
        pop    ebx
        jmp    ecx
        db  0CCh
sub_4_4D88:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_4_4DA6
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_4_4DA6:
        ret    10h
        db  0CCh, 0CCh, 0CCh
sub_4_4DAC:
        db  0CDh, 020h, 076h, 081h, 001h, 000h
        int3
        int3
sub_4_4DB4:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        int3
        int3
sub_4_4DBC:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        int3
        int3
sub_4_4DC4:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        int3
        int3
sub_4_4DCC:
        db  0CDh, 020h, 003h, 080h, 033h, 000h
        int3
        int3
sub_4_4DD4:
        db  0CDh, 020h, 005h, 080h, 033h, 000h
        int3
        int3
sub_4_4DDC:
        db  0CDh, 020h, 044h, 080h, 033h, 000h
        int3
        int3
sub_4_4DE4:
        db  0CDh, 020h, 004h, 080h, 033h, 000h
        int3
        int3
sub_4_4DEC:
        db  0CDh, 020h, 056h, 080h, 033h, 000h
        int3
        int3
sub_4_4DF4:
        db  0CDh, 020h, 013h, 080h, 033h, 000h
        int3
        int3
sub_4_4DFC:
        db  0CDh, 020h, 01Ch, 080h, 033h, 000h
        int3
        int3
sub_4_4E04:
        db  0CDh, 020h, 036h, 080h, 033h, 000h
        int3
        int3
sub_4_4E0C:
        db  0CDh, 020h, 00Dh, 080h, 017h, 000h
        int3
        int3
sub_4_4E14:
        db  0CDh, 020h, 019h, 080h, 033h, 000h
        int3
        int3
sub_4_4E1C:
        db  0CDh, 020h, 007h, 080h, 033h, 000h
        int3
        int3
sub_4_4E24:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
        int3
        int3
sub_4_4E2C:
        db  0CDh, 020h, 03Dh, 080h, 033h, 000h
        int3
        int3
        db  0CDh, 020h, 03Fh, 080h, 033h, 000h
        int3
        int3
sub_4_4E3C:
        db  0CDh, 020h, 018h, 080h, 033h, 000h
        int3
        int3
sub_4_4E44:
        db  0CDh, 020h, 00Bh, 080h, 033h, 000h
_PTEXT ends
        end
