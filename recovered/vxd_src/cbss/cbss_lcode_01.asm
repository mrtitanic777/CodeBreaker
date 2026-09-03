; cbss_lcode_01.asm -- CBSS object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 004h, 000h, 000h, 000h, 043h, 042h, 053h, 053h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 053h, 081h, 078h, 004h, 043h, 042h, 053h, 053h, 074h
        db  009h, 08Bh, 045h, 00Ch, 0C6h, 040h, 01Dh, 001h, 0EBh, 065h, 08Dh, 048h, 025h, 08Bh, 050h, 018h
        db  08Ah, 09Ah, 004h, 008h, 000h, 000h, 080h, 0E3h, 00Fh, 008h, 019h, 08Bh, 01Ah, 080h, 0E3h, 006h
        db  080h, 0FBh, 006h, 074h, 005h, 0F6h, 002h, 006h, 074h, 003h, 080h, 009h, 008h, 0F6h, 040h, 00Bh
        db  004h, 0C7h, 002h, 00Fh, 000h, 000h, 000h, 074h, 007h, 0C7h, 042h, 004h, 006h, 000h, 000h, 000h
        db  08Ah, 001h, 084h, 082h, 005h, 008h, 000h, 000h, 075h, 018h, 0A8h, 008h, 074h, 009h, 08Bh, 042h
        db  004h, 024h, 006h, 03Ch, 006h, 074h, 00Bh, 08Bh, 045h, 00Ch, 066h, 0C7h, 040h, 018h, 000h, 000h
        db  0EBh, 009h, 08Bh, 045h, 00Ch, 066h, 0C7h, 040h, 018h, 001h, 000h, 0C6h, 040h, 01Dh, 000h, 080h
        db  078h, 01Dh, 000h, 075h, 006h, 080h, 060h, 020h, 0FEh, 0EBh, 004h, 080h, 048h, 020h, 001h, 05Bh
        db  05Dh, 0C3h, 0CCh, 0CCh
CBSS_Control:
        cmp    eax, 22h
        jne    SHORT loc_1_012A
        push   edx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0128:
        stc
        ret
loc_1_012A:
        cmp    eax, 1bh
        jne    SHORT loc_1_0138
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0134:
        cmp    eax, 1
        ret
loc_1_0138:
        clc
        ret
loc_1_013A:
        pushfd
        pushad
        mov    ebp, esp
        cmp    ah, 9eh
        jne    SHORT loc_1_014E
        push   ebp
        push   esi
        db  0E8h, 03Eh, 0FFh, 0FFh, 0FFh
        pop    ecx
        pop    ecx
        jmp    SHORT loc_1_01A9
loc_1_014E:
        cmp    dword ptr [esi + 4], 53534243h
        je     SHORT loc_1_0161
        mov    byte ptr [ebp + 1dh], 1
loc_1_015B:
        or     dword ptr [ebp + 20h], 1
        jmp    SHORT loc_1_01A9
loc_1_0161:
        bts    dword ptr [esi + 8], 1fh
        jae    SHORT loc_1_016E
        mov    byte ptr [ebp + 1dh], 18h
        jmp    SHORT loc_1_015B
loc_1_016E:
        sub    ah, 83h
        jae    SHORT loc_1_017D
loc_1_0173:
        mov    byte ptr [ebp + 1dh], 15h
        or     dword ptr [ebp + 20h], 1
        jmp    SHORT loc_1_01A2
loc_1_017D:
        cmp    ah, 0eh
        jbe    SHORT loc_1_0192
        cmp    ah, 1ah
        jne    SHORT loc_1_0173
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_018E:
        pop    ecx
        pop    ecx
        jmp    SHORT loc_1_01A2
loc_1_0192:
        movzx  eax, ah
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        push   ebp
        push   esi
        call   eax
        pop    ecx
        pop    ecx
loc_1_01A2:
        and    dword ptr [esi + 8], 7fffffffh
loc_1_01A9:
        popad
        popfd
        ret
_LTEXT ends
        end
