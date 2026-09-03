; dosmgr_mcode_03.asm -- DOSMGR object 3 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  060h, 068h, 0B0h, 012h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h, 0B4h, 012h, 000h
        db  000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h, 0D2h, 012h, 000h, 000h, 0CDh, 020h, 0F3h, 000h
        db  001h, 000h, 068h, 0E7h, 012h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h, 0FCh, 012h
        db  000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h, 011h, 013h, 000h, 000h, 0CDh, 020h, 0F3h
        db  000h, 001h, 000h, 068h, 026h, 013h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h, 03Bh
        db  013h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h, 050h, 013h, 000h, 000h, 0CDh, 020h
        db  0F3h, 000h, 001h, 000h, 068h, 065h, 013h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h
        db  084h
_LMSGTABLE ends
        end
