; enable_pcode_07.asm -- ENABLE object 7 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_7_0000:
        push   esi
        db  083h, 03Dh, 056h, 003h, 000h, 000h, 000h
        je     SHORT loc_7_0088
        db  083h, 025h, 056h, 003h, 000h, 000h, 000h
        db  0F6h, 005h, 076h, 002h, 000h, 000h, 001h
        je     SHORT loc_7_0088
        db  0A1h, 0CCh, 003h, 000h, 000h
        test   eax, 0ffff8fffh
        je     SHORT loc_7_0088
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    ebx, eax
        db  02Bh, 005h, 0C8h, 003h, 000h, 000h
        add    eax, 37h
        db  03Bh, 005h, 07Ah, 002h, 000h, 000h
        jb     SHORT loc_7_006D
        db  0F6h, 005h, 076h, 002h, 000h, 000h, 002h
        je     SHORT loc_7_005F
        xor    ecx, ecx
        inc    ecx
        lea    eax, [ecx + 1]
        mov    ebx, 0c8h
        db  0E8h, 020h, 00Eh, 0F9h, 0FFh
        db  0C6h, 005h, 062h, 003h, 000h, 000h, 001h
loc_7_005F:
        db  0E8h, 0D4h, 002h, 0F9h, 0FFh
        db  0C6h, 005h, 062h, 003h, 000h, 000h, 000h
        jmp    SHORT loc_7_0088
loc_7_006D:
        xchg   ebx, eax
        db  0A1h, 07Ah, 002h, 000h, 000h
        sub    eax, ebx
        xor    edx, edx
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 056h, 003h, 000h, 000h
loc_7_0088:
        pop    esi
        clc
        ret
sub_7_008B:
        push   eax
        db  080h, 03Dh, 0C3h, 003h, 000h, 000h, 002h
        jb     NEAR PTR loc_7_01A7
        jne    NEAR PTR loc_7_0122
        db  083h, 03Dh, 0D2h, 002h, 000h, 000h, 0FFh
        je     SHORT loc_7_00BC
        db  0A1h, 0D2h, 002h, 000h, 000h
        db  0A3h, 030h, 002h, 000h, 000h
        db  0C7h, 005h, 0D2h, 002h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_7_00BC:
        db  083h, 03Dh, 0D6h, 002h, 000h, 000h, 0FFh
        je     SHORT loc_7_00D9
        db  0A1h, 0D6h, 002h, 000h, 000h
        db  0A3h, 02Ch, 002h, 000h, 000h
        db  0C7h, 005h, 0D6h, 002h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_7_00D9:
        db  083h, 03Dh, 0DAh, 002h, 000h, 000h, 0FFh
        je     SHORT loc_7_00F6
        db  0A1h, 0DAh, 002h, 000h, 000h
        db  0A3h, 028h, 002h, 000h, 000h
        db  0C7h, 005h, 0DAh, 002h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_7_00F6:
        db  083h, 03Dh, 0DEh, 002h, 000h, 000h, 0FFh
        je     SHORT loc_7_0113
        db  0A1h, 0DEh, 002h, 000h, 000h
        db  0A3h, 034h, 002h, 000h, 000h
        db  0C7h, 005h, 0DEh, 002h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_7_0113:
        mov    edx, 100h
        db  0E8h, 0E3h, 0FEh, 0F8h, 0FFh
        jmp    NEAR PTR loc_7_01A7
loc_7_0122:
        db  080h, 03Dh, 0C3h, 003h, 000h, 000h, 003h
        jne    SHORT loc_7_014B
        db  0A1h, 034h, 002h, 000h, 000h
        db  0A3h, 0DEh, 002h, 000h, 000h
        db  0C7h, 005h, 034h, 002h, 000h, 000h, 0E8h, 003h, 000h, 000h
        mov    edx, 100h
        db  0E8h, 0B7h, 0FEh, 0F8h, 0FFh
        jmp    SHORT loc_7_01A7
loc_7_014B:
        db  0A1h, 030h, 002h, 000h, 000h
        db  0A3h, 0D2h, 002h, 000h, 000h
        db  0A1h, 02Ch, 002h, 000h, 000h
        db  0A3h, 0D6h, 002h, 000h, 000h
        db  0A1h, 028h, 002h, 000h, 000h
        db  0A3h, 0DAh, 002h, 000h, 000h
        db  081h, 03Dh, 028h, 002h, 000h, 000h, 0D0h, 007h, 000h, 000h
        jae    SHORT loc_7_017F
        db  0C7h, 005h, 028h, 002h, 000h, 000h, 0D0h, 007h, 000h, 000h
loc_7_017F:
        db  0C7h, 005h, 030h, 002h, 000h, 000h, 0F8h, 07Fh, 000h, 000h
        db  0C7h, 005h, 02Ch, 002h, 000h, 000h, 0F8h, 07Fh, 000h, 000h
        db  0C7h, 005h, 034h, 002h, 000h, 000h, 0F8h, 07Fh, 000h, 000h
        mov    edx, 100h
        db  0E8h, 059h, 0FEh, 0F8h, 0FFh
loc_7_01A7:
        db  0C6h, 005h, 0C1h, 003h, 000h, 000h, 000h
        db  0C6h, 005h, 0C7h, 003h, 000h, 000h, 000h
        db  0C6h, 005h, 0C3h, 003h, 000h, 000h, 000h
        pop    eax
        ret
loc_7_01BE:
        xor    eax, eax
        db  0A2h, 0C0h, 003h, 000h, 000h
        db  0A2h, 0C1h, 003h, 000h, 000h
        db  080h, 03Dh, 0C7h, 003h, 000h, 000h, 001h
        jne    SHORT loc_7_01DA
        db  0C6h, 005h, 0C7h, 003h, 000h, 000h, 0FFh
loc_7_01DA:
        db  0A2h, 0C3h, 003h, 000h, 000h
        db  0A3h, 0BCh, 003h, 000h, 000h
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  0A3h, 052h, 003h, 000h, 000h
        db  0A3h, 0C8h, 003h, 000h, 000h
        xchg   edx, eax
        push   eax
        db  086h, 005h, 0F9h, 001h, 000h, 000h
        db  032h, 005h, 0F9h, 001h, 000h, 000h
        je     SHORT loc_7_021D
        db  084h, 005h, 0F9h, 001h, 000h, 000h
        jne    SHORT loc_7_021D
        db  0A1h, 0CCh, 003h, 000h, 000h
        and    eax, 4001h
        cmp    eax, 4001h
        jne    SHORT loc_7_021D
loc_7_021D:
        pop    eax
        db  0C6h, 005h, 0C6h, 003h, 000h, 000h, 000h
        clc
        ret
loc_7_0227:
        xor    ebx, ebx
        mov    bx, dx
        shr    edx, 10h
        xor    eax, eax
        xor    ecx, ecx
        mov    cl, dl
        mov    al, dh
        db  0E9h, 03Ch, 00Ch, 0F9h, 0FFh
loc_7_023C:
        bsf    eax, edx
        je     SHORT loc_7_024E
        cmp    ax, 0ch
        ja     SHORT loc_7_024E
        db  0FFh, 024h, 085h, 0EEh, 002h, 000h, 000h
loc_7_024E:
        clc
        ret
loc_7_0250:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        xchg   ebx, eax
        mov    ebx, edx
        push   0
        push   ebx
        db  068h, 0F9h, 001h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        clc
        ret
loc_7_026C:
        db  0C7h, 005h, 05Ah, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        db  080h, 03Dh, 0C7h, 003h, 000h, 000h, 0FFh
        jne    SHORT loc_7_0284
        jmp    NEAR PTR loc_7_0401
loc_7_0284:
        db  080h, 03Dh, 0F9h, 001h, 000h, 000h, 000h
        jne    NEAR PTR loc_7_0403
        db  0F6h, 005h, 024h, 002h, 000h, 000h, 004h
        je     NEAR PTR loc_7_0403
        db  0F6h, 005h, 024h, 002h, 000h, 000h, 002h
        je     NEAR PTR loc_7_0403
        db  080h, 03Dh, 0C3h, 003h, 000h, 000h, 000h
        jne    SHORT loc_7_0301
        db  080h, 03Dh, 0C7h, 003h, 000h, 000h, 001h
        jne    NEAR PTR loc_7_0403
        db  0F6h, 005h, 024h, 002h, 000h, 000h, 001h
        jne    SHORT loc_7_02F0
        db  0F6h, 005h, 024h, 002h, 000h, 000h, 008h
        je     SHORT loc_7_02F0
        db  0F6h, 005h, 024h, 002h, 000h, 000h, 010h
        je     SHORT loc_7_02F0
        mov    eax, 0
        mov    ebx, 1f4h
        mov    ecx, 3
        db  0E8h, 038h, 0FDh, 0F8h, 0FFh
loc_7_02F0:
        db  0C6h, 005h, 0C3h, 003h, 000h, 000h, 001h
        mov    eax, 0fa0h
        jmp    NEAR PTR loc_7_03E1
loc_7_0301:
        db  080h, 03Dh, 0C3h, 003h, 000h, 000h, 001h
        jne    SHORT loc_7_0379
        db  080h, 03Dh, 0C7h, 003h, 000h, 000h, 001h
        jne    NEAR PTR loc_7_0403
        db  0F6h, 005h, 024h, 002h, 000h, 000h, 001h
        je     SHORT loc_7_034E
        db  0C6h, 005h, 0C3h, 003h, 000h, 000h, 002h
        call   sub_7_008B
        db  0F6h, 005h, 024h, 002h, 000h, 000h, 010h
        je     SHORT loc_7_0349
        mov    eax, 2
        mov    ebx, 0c8h
        mov    ecx, 1
        db  0E8h, 02Fh, 00Bh, 0F9h, 0FFh
loc_7_0349:
        jmp    NEAR PTR loc_7_0403
loc_7_034E:
        db  0F6h, 005h, 024h, 002h, 000h, 000h, 010h
        je     SHORT loc_7_036B
        mov    eax, 1
        mov    ebx, 0c8h
        mov    ecx, 1
        db  0E8h, 0BDh, 0FCh, 0F8h, 0FFh
loc_7_036B:
        db  0C6h, 005h, 0C3h, 003h, 000h, 000h, 002h
        mov    eax, 0fa0h
        jmp    SHORT loc_7_03E1
loc_7_0379:
        db  080h, 03Dh, 0C3h, 003h, 000h, 000h, 002h
        jne    SHORT loc_7_03B6
        db  080h, 03Dh, 0C7h, 003h, 000h, 000h, 001h
        jne    SHORT loc_7_0403
        db  0F6h, 005h, 024h, 002h, 000h, 000h, 010h
        je     SHORT loc_7_03A8
        mov    eax, 1
        mov    ebx, 0c8h
        mov    ecx, 2
        db  0E8h, 080h, 0FCh, 0F8h, 0FFh
loc_7_03A8:
        db  0C6h, 005h, 0C3h, 003h, 000h, 000h, 003h
        mov    eax, 0fa0h
        jmp    SHORT loc_7_03E1
loc_7_03B6:
        db  0F6h, 005h, 024h, 002h, 000h, 000h, 010h
        je     SHORT loc_7_03D3
        mov    eax, 1
        mov    ebx, 0c8h
        mov    ecx, 3
        db  0E8h, 055h, 0FCh, 0F8h, 0FFh
loc_7_03D3:
        db  0C6h, 005h, 0C3h, 003h, 000h, 000h, 004h
        call   sub_7_008B
        jmp    SHORT loc_7_0403
loc_7_03E1:
        pushad
        db  0BEh, 06Ch, 002h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xor    edx, edx
        mov    eax, 0fa0h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 05Ah, 003h, 000h, 000h
        popad
loc_7_0401:
        clc
        ret
loc_7_0403:
        db  0C6h, 005h, 0C1h, 003h, 000h, 000h, 000h
        db  0C6h, 005h, 0C7h, 003h, 000h, 000h, 000h
        db  0C6h, 005h, 0C3h, 003h, 000h, 000h, 000h
        db  0C7h, 005h, 05Ah, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        clc
        ret
loc_7_0424:
        db  0C6h, 005h, 0C3h, 003h, 000h, 000h, 000h
        db  0BEh, 06Ch, 002h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xor    edx, edx
        mov    eax, 0fa0h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 05Ah, 003h, 000h, 000h
        clc
        ret
loc_7_044B:
        db  0C6h, 005h, 0C7h, 003h, 000h, 000h, 000h
        db  083h, 03Dh, 05Ah, 003h, 000h, 000h, 000h
        je     SHORT loc_7_0473
        push   esi
        db  08Bh, 035h, 05Ah, 003h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        db  0C7h, 005h, 05Ah, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    esi
loc_7_0473:
        clc
        ret
sub_7_0475:
        db  0BEh, 0DAh, 003h, 000h, 000h
        mov    ecx, 2
        db  0CDh, 020h, 007h, 000h, 00Dh, 000h
        ret
        db  000h, 000h
_PTEXT ends
        end
