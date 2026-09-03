; nwredir_pcode_08.asm -- NWREDIR object 8 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_8_0000:
        db  0CDh, 020h, 003h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_8_0008:
        db  0CDh, 020h, 007h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_8_0010:
        db  0CDh, 020h, 00Eh, 080h, 033h, 024h
_PTEXT ends
        end
