; qic117_pcode_02.asm -- QIC117 object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
        db  0CDh, 020h, 002h, 080h, 033h, 000h
        int3
        int3
sub_2_0008:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        int3
        int3
sub_2_0010:
        db  0CDh, 020h, 005h, 080h, 033h, 000h
        int3
        int3
sub_2_0018:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        int3
        int3
sub_2_0020:
        db  0CDh, 020h, 022h, 080h, 033h, 000h
        int3
        int3
sub_2_0028:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        int3
        int3
sub_2_0030:
        db  0CDh, 020h, 004h, 080h, 033h, 000h
_PTEXT ends
        end
