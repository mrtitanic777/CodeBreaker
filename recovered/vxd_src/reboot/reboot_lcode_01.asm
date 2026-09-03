; reboot_lcode_01.asm -- REBOOT object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

REBOOT_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 092h, 000h, 0FFh, 0FFh
        cmp    eax, 7
        db  00Fh, 084h, 0E5h, 0FFh, 0FEh, 0FFh
        cmp    eax, 0ch
        db  00Fh, 084h, 0DCh, 0FFh, 0FEh, 0FFh
        cmp    eax, 23h
        db  00Fh, 084h, 06Ah, 000h, 0FFh, 0FFh
        cmp    eax, 4
        jne    SHORT loc_1_0039
        db  080h, 00Dh, 0E4h, 002h, 000h, 000h, 020h
loc_1_0039:
        clc
        ret
loc_1_003B:
        db  0F6h, 005h, 0E4h, 002h, 000h, 000h, 080h
        jne    NEAR PTR loc_1_0101
        db  0F6h, 005h, 0E4h, 002h, 000h, 000h, 031h
        jne    NEAR PTR REBOOT_Service_03
        db  0A0h, 0E6h, 002h, 000h, 000h
        cmp    al, 3
        je     NEAR PTR REBOOT_Service_03
        cmp    al, 1
        je     NEAR PTR loc_1_0102
        cmp    al, 2
        je     NEAR PTR loc_1_0102
        cmp    al, 0
        jne    NEAR PTR loc_1_0101
        db  083h, 03Dh, 0D8h, 002h, 000h, 000h, 000h
        je     NEAR PTR REBOOT_Service_03
        db  00Fh, 0B6h, 005h, 0E7h, 002h, 000h, 000h
        push   0
        push   0
        db  068h, 0E8h, 002h, 000h, 000h
        push   eax
        db  0CDh, 020h, 06Fh, 000h, 040h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_00B2
        db  083h, 03Dh, 0E8h, 002h, 000h, 000h, 002h
        jge    NEAR PTR loc_1_0152
loc_1_00B2:
        db  0C6h, 005h, 0E6h, 002h, 000h, 000h, 001h
        mov    eax, 3a98h
        db  0BEh, 03Fh, 001h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 0E0h, 002h, 000h, 000h
        db  080h, 00Dh, 0E5h, 002h, 000h, 000h, 001h
        db  08Bh, 03Dh, 0DCh, 002h, 000h, 000h
        mov    eax, 400000h
        db  0CDh, 020h, 014h, 001h, 001h, 000h
        push   edi
        push   0
        db  0FFh, 035h, 0D8h, 002h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
        push   edi
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    ecx
loc_1_0101:
        ret
loc_1_0102:
        db  0C6h, 005h, 0E6h, 002h, 000h, 000h, 008h
        mov    eax, 0c0001010h
        mov    ecx, 5
        db  00Fh, 0B7h, 03Dh, 012h, 000h, 000h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        call   sub_1_01D8
        db  083h, 03Dh, 0E0h, 002h, 000h, 000h, 000h
        je     SHORT loc_1_0152
        db  080h, 03Dh, 0E6h, 002h, 000h, 000h, 008h
        jne    SHORT loc_1_0101
        db  0C6h, 005h, 0E6h, 002h, 000h, 000h, 001h
        ret
loc_1_013F:
        db  0C7h, 005h, 0E0h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  080h, 03Dh, 0E6h, 002h, 000h, 000h, 008h
        je     SHORT loc_1_016E
loc_1_0152:
        mov    eax, 0c0001010h
        mov    ecx, 3
        db  00Fh, 0B7h, 03Dh, 00Eh, 000h, 000h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        call   sub_1_01D8
loc_1_016E:
        ret
REBOOT_Service_03:
        push   5
        pop    ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        db  066h, 00Fh, 0BAh, 02Dh, 0E4h, 002h, 000h, 000h, 001h
        jb     SHORT loc_1_018E
        mov    eax, 17h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
loc_1_018E:
        cli
        db  066h, 00Fh, 0BAh, 02Dh, 0E4h, 002h, 000h, 000h, 002h
        jb     SHORT loc_1_01A5
        mov    eax, 18h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
loc_1_01A5:
        db  066h, 00Fh, 0BAh, 02Dh, 0E4h, 002h, 000h, 000h, 003h
        jb     SHORT loc_1_01BB
        mov    eax, 12h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
loc_1_01BB:
        push   80001h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        mov    esi, eax
        mov    eax, 1
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
sub_1_01D8:
        db  080h, 00Dh, 0E4h, 002h, 000h, 000h, 010h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xchg   ecx, eax
        add    eax, 80000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        xchg   ecx, eax
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        add    esp, dword ptr [esp]
        db  080h, 025h, 0E4h, 002h, 000h, 000h, 0EFh
        ret
REBOOT_Service_00:
        xor    eax, eax
        mov    ah, 4
        ret
REBOOT_Service_01:
        db  080h, 00Dh, 0E4h, 002h, 000h, 000h, 010h
        ret
REBOOT_Service_02:
        db  080h, 025h, 0E4h, 002h, 000h, 000h, 0EFh
        ret
        db  000h, 000h
loc_1_0220:
        mov    eax, dword ptr [edi + 8]
        and    eax, 47h
        cmp    eax, 5
        jne    SHORT loc_1_0256
        mov    eax, dword ptr [edi + 4]
        sub    eax, 0ffh
loc_1_0233:
        db  00Fh, 084h, 08Fh, 0FEh, 0FEh, 0FFh
        test   eax, 3ffh
        jne    SHORT loc_1_0256
        shl    eax, 0ch
loc_1_0243:
        cmp    eax, dword ptr [ebx + 4]
        je     SHORT loc_1_0233
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_1_0243
loc_1_0256:
        stc
        ret
        db  009h, 002h, 000h, 000h, 00Eh, 002h, 000h, 000h, 016h, 002h, 000h, 000h, 06Fh, 001h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 009h, 000h, 004h, 000h, 000h, 000h, 052h, 045h, 042h, 04Fh
        db  04Fh, 054h, 020h, 020h, 000h, 000h, 000h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  058h, 002h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 030h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        db  045h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_02E7:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
