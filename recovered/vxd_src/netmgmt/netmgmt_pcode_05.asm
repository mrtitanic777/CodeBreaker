; netmgmt_pcode_05.asm -- NETMGMT object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_0000:
        db  0CDh, 020h, 001h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_5_0008:
        db  0CDh, 020h, 002h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_5_0010:
        db  0CDh, 020h, 02Eh, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_5_0018:
        db  0CDh, 020h, 02Fh, 081h, 001h, 025h
_PTEXT ends
        end
