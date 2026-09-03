; reboot_pcode_04.asm -- REBOOT object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

REBOOT_PM_API:
        movzx  eax, word ptr [ebp + 1ch]
        dec    ah
        jne    SHORT loc_4_0016
        test   eax, eax
        db  00Fh, 084h, 05Fh, 001h, 0FCh, 0FFh
        dec    eax
        je     SHORT loc_4_001B
        dec    eax
        je     SHORT loc_4_0024
loc_4_0016:
        or     byte ptr [ebp + 2ch], 1
        ret
loc_4_001B:
        db  080h, 00Dh, 0E4h, 002h, 000h, 000h, 080h
        jmp    SHORT loc_4_002B
loc_4_0024:
        db  080h, 025h, 0E4h, 002h, 000h, 000h, 07Fh
loc_4_002B:
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_4_0030:
        mov    eax, dword ptr [esp + 0ch]
        db  0A3h, 0D8h, 002h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        db  0A3h, 0DCh, 002h, 000h, 000h
        ret    10h
loc_4_0045:
        mov    al, byte ptr [esp + 0ch]
        db  0A2h, 0E6h, 002h, 000h, 000h
        cmp    al, 0
        je     SHORT loc_4_005A
        cmp    al, 8
        je     SHORT loc_4_005A
        cmp    al, 3
        jne    SHORT loc_4_0068
loc_4_005A:
        xor    esi, esi
        db  087h, 035h, 0E0h, 002h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_4_0068:
        cmp    al, 0
        jne    SHORT loc_4_0088
        db  066h, 00Fh, 0BAh, 035h, 0E4h, 002h, 000h, 000h, 008h
        jae    SHORT loc_4_0088
        db  08Bh, 03Dh, 0DCh, 002h, 000h, 000h
        mov    eax, 0ffc00000h
        db  0CDh, 020h, 014h, 001h, 001h, 000h
loc_4_0088:
        ret    10h
        db  0A8h, 000h, 000h, 000h, 0AFh, 000h, 000h, 000h, 0B9h, 000h, 000h, 000h
loc_4_0097:
        mov    eax, dword ptr [esi + 0ch]
        cmp    eax, 3
        jae    SHORT loc_4_00C3
        db  0FFh, 024h, 085h, 08Bh, 000h, 000h, 000h
loc_4_00A8:
        xor    eax, eax
        mov    ecx, eax
        mov    ch, 4
        ret
loc_4_00AF:
        db  080h, 00Dh, 0E4h, 002h, 000h, 000h, 080h
        xor    eax, eax
        ret
loc_4_00B9:
        db  080h, 025h, 0E4h, 002h, 000h, 000h, 07Fh
        xor    eax, eax
        ret
loc_4_00C3:
        xor    eax, eax
        inc    eax
        ret
        db  000h
loc_4_00C8:
        db  0E8h, 06Ah, 0FFh, 0FBh, 0FFh
        mov    eax, 64h
        db  0BEh, 067h, 000h, 000h, 000h
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
        clc
        ret
loc_4_00DF:
        sub    ecx, ecx
        mov    eax, 1388h
        db  0CDh, 020h, 0ECh, 000h, 001h, 000h
        clc
        ret
_PTEXT ends
        end
