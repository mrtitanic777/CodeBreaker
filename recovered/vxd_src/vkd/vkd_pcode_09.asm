; vkd_pcode_09.asm -- VKD object 9 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

VKD_VM_SUSPEND:
        db  03Bh, 01Dh, 014h, 012h, 000h, 000h
        jne    SHORT loc_9_000D
        db  0E8h, 0E9h, 008h, 0F7h, 0FFh
loc_9_000D:
        clc
        ret
        db  000h
_PTEXT ends
        end
