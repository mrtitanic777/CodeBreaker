; snapshot_lcode_01.asm -- SNAPSHOT object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  0DBh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h
        db  0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h
        db  0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h
        db  0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h
        db  0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h
        db  0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0F8h, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h
        db  0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h
        db  0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h
        db  0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h
        db  0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h
        db  0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h
        db  0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h
        db  0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h
SNAPVXD_Control:
        cmp    eax, 33h
        jae    SHORT loc_1_00DC
        db  02Eh, 0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_00DB:
        clc
loc_1_00DC:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
SNAPVXD_END_PM_APP:
        db  080h, 03Dh, 0E3h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_01B1
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    NEAR PTR loc_1_01B1
        push   80000003h
        db  0CDh, 020h, 055h, 001h, 001h, 000h
        add    esp, 4
        push   80000002h
        db  0CDh, 020h, 055h, 001h, 001h, 000h
        add    esp, 4
        push   80000000h
        db  0CDh, 020h, 055h, 001h, 001h, 000h
        add    esp, 4
        push   80000001h
        db  0CDh, 020h, 055h, 001h, 001h, 000h
        add    esp, 4
        push   ebx
        push   4
        push   -1
        db  0CDh, 020h, 02Eh, 000h, 040h, 000h
        add    esp, 8
        sub    ecx, ecx
loc_1_0159:
        db  08Bh, 081h, 0E4h, 000h, 000h, 000h
        cmp    eax, -1
        je     SHORT loc_1_0171
        test   eax, eax
        je     SHORT loc_1_016C
        push   ecx
        call   eax
        pop    ecx
loc_1_016C:
        add    ecx, 4
        jmp    SHORT loc_1_0159
loc_1_0171:
        pop    ebx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 0dh
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0CDh, 020h, 003h, 000h, 009h, 000h
loc_1_01B1:
        clc
        ret
SNAPVXD_Service_00:
        mov    eax, 300h
        clc
        ret
SNAPVXD_Service_01:
        sub    ecx, ecx
loc_1_01BC:
        db  083h, 0B9h, 0E4h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_01D3
        db  083h, 0B9h, 0E4h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_1_01DB
        add    ecx, 4
        jmp    SHORT loc_1_01BC
loc_1_01D3:
        db  089h, 081h, 0E4h, 000h, 000h, 000h
        clc
        ret
loc_1_01DB:
        stc
        ret
SNAPVXD_PM_API:
        ret
        db  000h, 000h, 0B3h, 001h, 000h, 000h, 0BAh, 001h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 082h, 004h, 003h, 000h, 000h, 000h, 053h, 04Eh, 041h, 050h
        db  056h, 058h, 044h, 020h, 0FFh, 07Fh, 001h, 0A0h, 0CCh, 000h, 000h, 000h, 0DDh, 001h, 000h, 000h
        db  0DDh, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0E0h, 001h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
_LTEXT ends
        end
