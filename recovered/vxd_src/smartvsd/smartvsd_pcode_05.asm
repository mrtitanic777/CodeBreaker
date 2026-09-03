; smartvsd_pcode_05.asm -- SMARTVSD object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_0000:
        db  0CDh, 020h, 003h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_5_0008:
        db  0CDh, 020h, 03Bh, 080h, 033h, 026h
_PTEXT ends
        end
