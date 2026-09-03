; dbkvssd_pcode_02.asm -- DBKVSSD object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
        db  0CDh, 020h, 06Ch, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_2_0008:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_2_0010:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_2_0018:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  000h, 000h, 000h
sub_2_0034:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
        db  000h
sub_2_0044:
        db  0CDh, 020h, 002h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_004C:
        db  0CDh, 020h, 00Bh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_0054:
        db  0CDh, 020h, 00Dh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_005C:
        db  0CDh, 020h, 00Eh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_0064:
        db  0CDh, 020h, 00Fh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_006C:
        db  0CDh, 020h, 015h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_0074:
        db  0CDh, 020h, 018h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_007C:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_0084:
        db  0CDh, 020h, 01Ch, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_008C:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_0094:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_009C:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_00A4:
        db  0CDh, 020h, 049h, 080h, 033h, 024h
_PTEXT ends
        end
