; ndis_pcode_03.asm -- NDIS object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_3_0000:
        db  0CDh, 020h, 054h, 080h, 033h, 000h
        int3
        int3
sub_3_0008:
        db  0CDh, 020h, 044h, 080h, 033h, 000h
        int3
        int3
sub_3_0010:
        db  0CDh, 020h, 009h, 080h, 033h, 000h
        int3
        int3
sub_3_0018:
        db  0CDh, 020h, 003h, 080h, 033h, 000h
        int3
        int3
sub_3_0020:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        int3
        int3
sub_3_0028:
        db  0CDh, 020h, 032h, 080h, 033h, 000h
        int3
        int3
sub_3_0030:
        db  0CDh, 020h, 050h, 081h, 001h, 000h
        int3
        int3
sub_3_0038:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        int3
        int3
sub_3_0040:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        int3
        int3
sub_3_0048:
        db  0CDh, 020h, 022h, 080h, 033h, 000h
        int3
        int3
sub_3_0050:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        int3
        int3
sub_3_0058:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        int3
        int3
sub_3_0060:
        db  0CDh, 020h, 00Eh, 080h, 017h, 000h
        int3
        int3
sub_3_0068:
        db  0CDh, 020h, 04Eh, 080h, 033h, 000h
        int3
        int3
sub_3_0070:
        db  0CDh, 020h, 00Dh, 080h, 033h, 000h
        int3
        int3
sub_3_0078:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        int3
        int3
sub_3_0080:
        db  0CDh, 020h, 00Fh, 080h, 033h, 000h
        int3
        int3
sub_3_0088:
        db  0CDh, 020h, 00Eh, 080h, 033h, 000h
        int3
        int3
sub_3_0090:
        db  0CDh, 020h, 03Dh, 080h, 033h, 000h
        int3
        int3
sub_3_0098:
        db  0CDh, 020h, 00Bh, 080h, 033h, 000h
        int3
        int3
sub_3_00A0:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_3_00BE
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_3_00BE:
        ret    10h
        db  0CCh, 0CCh, 0CCh
sub_3_00C4:
        db  0CDh, 020h, 02Fh, 080h, 033h, 000h
        int3
        int3
sub_3_00CC:
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
sub_3_00E0:
        db  0CDh, 020h, 033h, 080h, 033h, 000h
        int3
        int3
sub_3_00E8:
        mov    eax, dword ptr [esp + 4]
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        ret    4
        db  0CCh, 0CCh, 0CCh
sub_3_00F8:
        mov    eax, dword ptr [esp + 4]
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        ret    4
        db  0CCh, 0CCh, 0CCh
sub_3_0108:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  0CCh, 0CCh, 0CCh
sub_3_0124:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
        db  0CCh
sub_3_0134:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  0CCh
sub_3_0140:
        mov    eax, dword ptr [esp + 4]
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        ret    4
        db  0CCh, 0CCh, 0CCh
sub_3_0150:
        push   edi
        mov    edi, dword ptr [esp + 8]
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        cmc
        sbb    ecx, ecx
        pop    edi
        and    eax, ecx
        ret    4
_PTEXT ends
        end
