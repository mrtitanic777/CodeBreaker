; unimodem_pcode_05.asm -- UNIMODEM object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_0000:
        db  0CDh, 020h, 00Fh, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_5_0008:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_5_0010:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_5_0018:
        db  0CDh, 020h, 050h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_5_0020:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_5_0028:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_5_0030:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_5_0038:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
        db  000h
sub_5_0048:
        db  0CDh, 020h, 03Dh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_5_0050:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_5_0058:
        db  0CDh, 020h, 001h, 080h, 09Dh, 004h
_PTEXT ends
        end
