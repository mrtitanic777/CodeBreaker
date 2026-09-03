; vnetbios_pcode_06.asm -- VNETBIOS object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_6_0000:
        db  0CDh, 020h, 04Ah, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_6_0008:
        db  0CDh, 020h, 04Bh, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_6_0010:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_6_0018:
        db  0CDh, 020h, 03Eh, 080h, 033h, 025h
_PTEXT ends
        end
