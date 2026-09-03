; adaptec_lcode_01.asm -- ADAPTEC object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 041h, 044h, 041h, 050h
        db  054h, 045h, 043h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  0CCh, 0CCh, 0CCh, 0CCh
loc_1_0060:
        inc    ecx
        inc    esp
        inc    ecx
        push   eax
        push   esp
        inc    ebp
        inc    ebx
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h
loc_1_0078:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_007C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
ADAPTEC_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_008F
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_008F:
        clc
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_00FD:
        db  0CDh, 020h, 023h, 080h, 0A2h, 004h
sub_1_0103:
        db  0CDh, 020h, 024h, 080h, 0A2h, 004h
sub_1_0109:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        ret
sub_1_0110:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_0109
        dec    eax
        mov    dword ptr [ebp - 4], eax
loc_1_0122:
        call   sub_1_0109
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp + 8]
        jae    NEAR PTR loc_1_0138
        jmp    NEAR PTR loc_1_0122
loc_1_0138:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_013D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05F8
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_0168
        xor    eax, eax
        jmp    NEAR PTR loc_1_0275
loc_1_0168:
        push   7d0h
        call   sub_1_0110
        add    esp, 4
        push   30h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CD
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0190
        jmp    NEAR PTR loc_1_026D
loc_1_0190:
        push   31h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CD
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_01AB
        jmp    NEAR PTR loc_1_026D
loc_1_01AB:
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CD
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_01C6
        jmp    NEAR PTR loc_1_026D
loc_1_01C6:
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CD
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_01E1
        jmp    NEAR PTR loc_1_026D
loc_1_01E1:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_04D3
        add    esp, 4
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07EC
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0208
        jmp    NEAR PTR loc_1_026D
loc_1_0208:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0773
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_022E
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 4
        je     NEAR PTR loc_1_0233
loc_1_022E:
        jmp    NEAR PTR loc_1_026D
loc_1_0233:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05F8
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_024C
        jmp    NEAR PTR loc_1_026D
loc_1_024C:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 080h, 025h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        and    dword ptr [eax], 3eh
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 4], 7
        mov    dword ptr [ebp - 0ch], 1
loc_1_026D:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_0275
loc_1_0275:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_027A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_065C
        add    esp, 4
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_02AE
        jmp    NEAR PTR loc_1_035C
loc_1_02AE:
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 080h, 025h, 000h, 000h
        je     NEAR PTR loc_1_02DC
        push   2580h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0700
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_02DC
        jmp    NEAR PTR loc_1_035C
loc_1_02DC:
        mov    al, byte ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CD
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_02F9
        jmp    NEAR PTR loc_1_035C
loc_1_02F9:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     eax, 1
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CD
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_031B
        jmp    NEAR PTR loc_1_035C
loc_1_031B:
        cmp    dword ptr [ebp + 0ch], 2580h
        je     NEAR PTR loc_1_0345
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0700
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0345
        jmp    NEAR PTR loc_1_035C
loc_1_0345:
        mov    eax, dword ptr [ebp + 0ch]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0364
loc_1_035C:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0364
loc_1_0364:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0369:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        push   0
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06F8
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_0398
        jmp    NEAR PTR loc_1_03C1
loc_1_0398:
        push   0
        push   0
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06F8
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_03B7
        jmp    NEAR PTR loc_1_03C1
loc_1_03B7:
        mov    eax, 1
        jmp    NEAR PTR loc_1_03C8
loc_1_03C1:
        xor    eax, eax
        jmp    NEAR PTR loc_1_03C8
loc_1_03C8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_03CD:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_04D3
        add    esp, 4
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_03F1
loc_1_03EE:
        inc    dword ptr [ebp - 4]
loc_1_03F1:
        cmp    dword ptr [ebp - 4], 5
        jge    NEAR PTR loc_1_04BB
        mov    al, byte ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07EC
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0418
        jmp    NEAR PTR loc_1_04BB
loc_1_0418:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0773
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0435
        jmp    NEAR PTR loc_1_04BB
loc_1_0435:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0ch]
        cmp    eax, ecx
        je     NEAR PTR loc_1_044C
        jmp    NEAR PTR loc_1_03EE
loc_1_044C:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        shl    eax, 4
        or     eax, 0fh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07EC
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0471
        jmp    NEAR PTR loc_1_04BB
loc_1_0471:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0773
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_048E
        jmp    NEAR PTR loc_1_04BB
loc_1_048E:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_04B6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05F8
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_04CE
loc_1_04B6:
        jmp    NEAR PTR loc_1_03EE
loc_1_04BB:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05F8
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_04CE
loc_1_04CE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_04D3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_059E
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_04F7
        xor    eax, eax
        jmp    NEAR PTR loc_1_0599
loc_1_04F7:
        call   sub_1_0109
        mov    dword ptr [ebp - 4], eax
loc_1_04FF:
        push   0ffh
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07EC
        add    esp, 8
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0773
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_0575
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0c3h
        jne    NEAR PTR loc_1_0547
        mov    eax, 1
        jmp    NEAR PTR loc_1_0599
        db  0E9h, 02Eh, 000h, 000h, 000h
loc_1_0547:
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_0556
loc_1_0553:
        inc    dword ptr [ebp - 0ch]
loc_1_0556:
        cmp    dword ptr [ebp - 0ch], 3
        jae    NEAR PTR loc_1_0575
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0773
        add    esp, 8
        jmp    NEAR PTR loc_1_0553
loc_1_0575:
        call   sub_1_0109
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, 0dch
        jb     NEAR PTR loc_1_058D
        jmp    NEAR PTR loc_1_0592
loc_1_058D:
        jmp    NEAR PTR loc_1_04FF
loc_1_0592:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0599
loc_1_0599:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_059E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06F8
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_05C3
        jmp    NEAR PTR loc_1_05EC
loc_1_05C3:
        push   0
        push   0
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06F8
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_05E2
        jmp    NEAR PTR loc_1_05EC
loc_1_05E2:
        mov    eax, 1
        jmp    NEAR PTR loc_1_05F3
loc_1_05EC:
        xor    eax, eax
        jmp    NEAR PTR loc_1_05F3
loc_1_05F3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_05F8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06F8
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_061D
        jmp    NEAR PTR loc_1_0650
loc_1_061D:
        push   0
        push   0
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06F8
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_063C
        jmp    NEAR PTR loc_1_0650
loc_1_063C:
        push   14h
        call   sub_1_0110
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_0657
loc_1_0650:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0657
loc_1_0657:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_065C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 0ffh
        cmp    dword ptr [ebp + 8], 960h
        jne    NEAR PTR loc_1_067F
        mov    byte ptr [ebp - 4], 10h
        jmp    NEAR PTR loc_1_06E8
loc_1_067F:
        cmp    dword ptr [ebp + 8], 2580h
        jne    NEAR PTR loc_1_0695
        mov    byte ptr [ebp - 4], 30h
        jmp    NEAR PTR loc_1_06E8
loc_1_0695:
        cmp    dword ptr [ebp + 8], 4b00h
        jne    NEAR PTR loc_1_06AB
        mov    byte ptr [ebp - 4], 40h
        jmp    NEAR PTR loc_1_06E8
loc_1_06AB:
        cmp    dword ptr [ebp + 8], 9600h
        jne    NEAR PTR loc_1_06C1
        mov    byte ptr [ebp - 4], 50h
        jmp    NEAR PTR loc_1_06E8
loc_1_06C1:
        cmp    dword ptr [ebp + 8], 0e100h
        jne    NEAR PTR loc_1_06D7
        mov    byte ptr [ebp - 4], 60h
        jmp    NEAR PTR loc_1_06E8
loc_1_06D7:
        cmp    dword ptr [ebp + 8], 1c200h
        jne    NEAR PTR loc_1_06E8
        mov    byte ptr [ebp - 4], 70h
loc_1_06E8:
        mov    al, byte ptr [ebp - 4]
        jmp    NEAR PTR loc_1_06F0
loc_1_06F0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh
sub_1_06F8:
        db  0CDh, 020h, 00Eh, 080h, 02Bh, 000h
        int3
        int3
sub_1_0700:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0874
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0728
        xor    eax, eax
        jmp    NEAR PTR loc_1_076E
loc_1_0728:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 2ch], eax
        mov    byte ptr [ebp - 1ah], 8
        mov    byte ptr [ebp - 19h], 0
        mov    byte ptr [ebp - 18h], 0
        mov    dword ptr [ebp - 34h], 55h
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_086C
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_0764
        xor    eax, eax
        jmp    NEAR PTR loc_1_076E
loc_1_0764:
        mov    eax, 1
        jmp    NEAR PTR loc_1_076E
loc_1_076E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0773:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        call   sub_1_0109
        mov    dword ptr [ebp - 4], eax
loc_1_0784:
        lea    eax, [ebp - 8]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0884
        add    esp, 10h
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_07D1
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_087C
        add    esp, 0ch
        call   sub_1_0109
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, 0ch
        jb     NEAR PTR loc_1_07CC
        jmp    NEAR PTR loc_1_07E0
loc_1_07CC:
        jmp    NEAR PTR loc_1_07DB
loc_1_07D1:
        mov    eax, 1
        jmp    NEAR PTR loc_1_07E7
loc_1_07DB:
        jmp    NEAR PTR loc_1_0784
loc_1_07E0:
        xor    eax, eax
        jmp    NEAR PTR loc_1_07E7
loc_1_07E7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_07EC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [ebp + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_088C
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_0826
        cmp    dword ptr [ebp - 4], 1
        je     NEAR PTR loc_1_082D
loc_1_0826:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0837
loc_1_082D:
        mov    eax, 1
        jmp    NEAR PTR loc_1_0837
loc_1_0837:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_083C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_00FD
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_0103
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh
sub_1_086C:
        db  0CDh, 020h, 005h, 080h, 02Bh, 000h
        int3
        int3
sub_1_0874:
        db  0CDh, 020h, 006h, 080h, 02Bh, 000h
        int3
        int3
sub_1_087C:
        db  0CDh, 020h, 00Bh, 080h, 02Bh, 000h
        int3
        int3
sub_1_0884:
        db  0CDh, 020h, 013h, 080h, 02Bh, 000h
        int3
        int3
sub_1_088C:
        db  0CDh, 020h, 012h, 080h, 02Bh, 000h
        int3
        int3
_LTEXT ends
        end
