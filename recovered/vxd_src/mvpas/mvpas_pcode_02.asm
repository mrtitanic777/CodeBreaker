; mvpas_pcode_02.asm -- MVPAS object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_2_0000:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0017:
        add    esp, 14h
        leave
        clc
        ret
loc_2_001D:
        push   3
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        db  0CDh, 020h, 00Eh, 000h, 033h, 000h
        add    esp, 10h
        mov    eax, 0
        stc
        ret
        db  0CCh, 0CCh
sub_2_0038:
        db  0CDh, 020h, 04Bh, 081h, 001h, 000h
        int3
        int3
sub_2_0040:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        int3
        int3
sub_2_0048:
        db  0CDh, 020h, 03Fh, 080h, 033h, 000h
        int3
        int3
sub_2_0050:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        int3
        int3
sub_2_0058:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        int3
        int3
sub_2_0060:
        db  0CDh, 020h, 03Dh, 080h, 033h, 000h
        int3
        int3
sub_2_0068:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        int3
        int3
sub_2_0070:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
_PTEXT ends
        end
