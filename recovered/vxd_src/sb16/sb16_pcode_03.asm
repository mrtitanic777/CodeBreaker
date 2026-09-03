; sb16_pcode_03.asm -- SB16 object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_3_0000:
        db  0CDh, 020h, 03Fh, 080h, 033h, 000h
        int3
        int3
sub_3_0008:
        db  0CDh, 020h, 044h, 080h, 033h, 000h
        int3
        int3
sub_3_0010:
        db  0CDh, 020h, 00Fh, 080h, 033h, 000h
        int3
        int3
sub_3_0018:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        int3
        int3
sub_3_0020:
        db  0CDh, 020h, 04Bh, 081h, 001h, 000h
        int3
        int3
sub_3_0028:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        int3
        int3
sub_3_0030:
        db  0CDh, 020h, 03Dh, 080h, 033h, 000h
        int3
        int3
sub_3_0038:
        db  0CDh, 020h, 007h, 080h, 033h, 000h
        int3
        int3
sub_3_0040:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
        int3
        int3
sub_3_0048:
        db  0CDh, 020h, 01Bh, 080h, 033h, 000h
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
        db  0CDh, 020h, 01Ch, 080h, 033h, 000h
        int3
        int3
sub_3_0068:
        db  0CDh, 020h, 018h, 080h, 033h, 000h
        int3
        int3
sub_3_0070:
        db  0CDh, 020h, 00Bh, 080h, 033h, 000h
        int3
        int3
sub_3_0078:
        db  0CDh, 020h, 004h, 080h, 033h, 000h
        int3
        int3
sub_3_0080:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
_PTEXT ends
        end
