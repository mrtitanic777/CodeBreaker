; vfat_pcode_05.asm -- VFAT object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  083h, 0FBh, 001h, 074h, 008h, 0B8h, 000h, 000h, 033h, 0DBh, 033h, 0F6h, 0C3h, 0B8h, 001h, 080h
        db  0EBh, 0F6h
_PTEXT ends
        end
