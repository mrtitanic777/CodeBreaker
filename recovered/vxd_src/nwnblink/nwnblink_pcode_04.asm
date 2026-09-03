; nwnblink_pcode_04.asm -- NWNBLINK object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_0000:
        db  0CDh, 020h, 00Eh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_0008:
        db  0CDh, 020h, 03Eh, 080h, 033h, 025h
_PTEXT ends
        end
