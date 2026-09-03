; crystal_lcode_01.asm -- CRYSTAL object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 043h, 052h, 059h, 053h
        db  054h, 041h, 04Ch, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  0CCh, 0CCh, 0CCh, 0CCh
loc_1_0060:
        or     edx, dword ptr [ebx + 47h]
        arpl   word ptr [ecx + edx*8 + 56h], si
        db  0D0h
loc_1_0068:
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_006C:
        rol    dword ptr [eax], 1
        add    byte ptr [eax], al
loc_1_0070:
        rol    byte ptr [eax], 1
        add    byte ptr [eax], al
loc_1_0074:
        adc    byte ptr [edi - 78efee6bh], cl
        xchg   ecx, eax
        adc    dword ptr [eax], edx
        mov    edx, dword ptr [eax - 6f7aefefh]
        adc    dword ptr [eax], edx
        adc    dword ptr [eax - 6f7eefefh], 11h
loc_1_008C:
        inc    ebx
        push   edx
        pop    ecx
        push   ebx
        push   esp
        inc    ecx
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h
loc_1_00A8:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
CRYSTAL_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_00BF
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_00BF:
        clc
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_012D:
        db  0CDh, 020h, 023h, 080h, 0A2h, 004h
sub_1_0133:
        db  0CDh, 020h, 024h, 080h, 0A2h, 004h
sub_1_0139:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        ret
sub_1_0140:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_0139
        dec    eax
        mov    dword ptr [ebp - 4], eax
loc_1_0152:
        call   sub_1_0139
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp + 8]
        jae    NEAR PTR loc_1_0168
        jmp    NEAR PTR loc_1_0152
loc_1_0168:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_016D:
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
        call   sub_1_06E8
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_0192
        jmp    NEAR PTR loc_1_01EE
loc_1_0192:
        push   0
        push   0
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06E8
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_01B1
        jmp    NEAR PTR loc_1_01EE
loc_1_01B1:
        push   32h
        call   sub_1_0140
        add    esp, 4
        push   0
        push   0
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06E8
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_01DA
        jmp    NEAR PTR loc_1_01EE
loc_1_01DA:
        push   32h
        call   sub_1_0140
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_01F5
loc_1_01EE:
        xor    eax, eax
        jmp    NEAR PTR loc_1_01F5
loc_1_01F5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_01FA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06E8
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_021F
        jmp    NEAR PTR loc_1_0233
loc_1_021F:
        push   28h
        call   sub_1_0140
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_023A
loc_1_0233:
        xor    eax, eax
        jmp    NEAR PTR loc_1_023A
loc_1_023A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_023F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_0248:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0763
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_0265
        jmp    NEAR PTR loc_1_0248
loc_1_0265:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_0274
loc_1_0271:
        inc    dword ptr [ebp - 4]
loc_1_0274:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 4], eax
        jae    NEAR PTR loc_1_02E5
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + ecx]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07DC
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_02A3
        jmp    NEAR PTR loc_1_02EF
loc_1_02A3:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0763
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_02C0
        jmp    NEAR PTR loc_1_02EF
loc_1_02C0:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        cmp    eax, ecx
        je     NEAR PTR loc_1_02E0
        jmp    NEAR PTR loc_1_02EF
loc_1_02E0:
        jmp    NEAR PTR loc_1_0271
loc_1_02E5:
        mov    eax, 1
        jmp    NEAR PTR loc_1_02F6
loc_1_02EF:
        xor    eax, eax
        jmp    NEAR PTR loc_1_02F6
loc_1_02F6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_02FB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        push   8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_023F
        add    esp, 0ch
        jmp    NEAR PTR loc_1_0319
loc_1_0319:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_031E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_023F
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_0347
        jmp    NEAR PTR loc_1_03BF
loc_1_0347:
        push   0c0h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07DC
        add    esp, 8
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0763
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0375
        jmp    NEAR PTR loc_1_03BF
loc_1_0375:
        mov    al, byte ptr [ebp - 4]
        and    eax, 0f0h
        cmp    al, 0c0h
        je     NEAR PTR loc_1_038A
        jmp    NEAR PTR loc_1_03BF
loc_1_038A:
        mov    al, byte ptr [ebp - 4]
        and    eax, 0fh
        db  0A2h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_023F
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_03B5
        jmp    NEAR PTR loc_1_03BF
loc_1_03B5:
        mov    eax, 1
        jmp    NEAR PTR loc_1_03C6
loc_1_03BF:
        xor    eax, eax
        jmp    NEAR PTR loc_1_03C6
loc_1_03C6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_03CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_03E3
loc_1_03E0:
        inc    dword ptr [ebp - 4]
loc_1_03E3:
        cmp    dword ptr [ebp - 4], 5
        jae    NEAR PTR loc_1_043A
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_016D
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_02FB
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_0426
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_031E
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_0426
        jmp    NEAR PTR loc_1_043A
loc_1_0426:
        cmp    dword ptr [ebp - 4], 4
        jne    NEAR PTR loc_1_0435
        jmp    NEAR PTR loc_1_0444
loc_1_0435:
        jmp    NEAR PTR loc_1_03E0
loc_1_043A:
        mov    eax, 1
        jmp    NEAR PTR loc_1_044B
loc_1_0444:
        xor    eax, eax
        jmp    NEAR PTR loc_1_044B
loc_1_044B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0450:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CB
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_0472
        jmp    NEAR PTR loc_1_04A9
loc_1_0472:
        xor    eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        cmp    eax, 1
        jne    NEAR PTR loc_1_048D
        mov    eax, dword ptr [ebp + 0ch]
        and    dword ptr [eax], 1fh
        jmp    NEAR PTR loc_1_0493
loc_1_048D:
        mov    eax, dword ptr [ebp + 0ch]
        and    dword ptr [eax], 3fh
loc_1_0493:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06A0
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_04B0
loc_1_04A9:
        xor    eax, eax
        jmp    NEAR PTR loc_1_04B0
loc_1_04B0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_04B5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_051D
loc_1_04C9:
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_0586
loc_1_04D5:
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_0586
loc_1_04E1:
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_0586
loc_1_04ED:
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_0586
loc_1_04F9:
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_0586
loc_1_0505:
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_0586
loc_1_0511:
        xor    eax, eax
        jmp    NEAR PTR loc_1_063C
        db  0E9h, 069h, 000h, 000h, 000h
loc_1_051D:
        cmp    dword ptr [ebp - 8], 2580h
        ja     NEAR PTR loc_1_0542
        je     NEAR PTR loc_1_04D5
        cmp    dword ptr [ebp - 8], 960h
        je     NEAR PTR loc_1_04C9
        jmp    NEAR PTR loc_1_0511
loc_1_0542:
        cmp    dword ptr [ebp - 8], 9600h
        ja     NEAR PTR loc_1_0567
        je     NEAR PTR loc_1_04ED
        cmp    dword ptr [ebp - 8], 4b00h
        je     NEAR PTR loc_1_04E1
        jmp    NEAR PTR loc_1_0511
loc_1_0567:
        cmp    dword ptr [ebp - 8], 0e100h
        je     NEAR PTR loc_1_04F9
        cmp    dword ptr [ebp - 8], 1c200h
        je     NEAR PTR loc_1_0505
        jmp    NEAR PTR loc_1_0511
loc_1_0586:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_01FA
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_023F
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_0602
        push   2580h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06F0
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_05CA
        jmp    NEAR PTR loc_1_0635
loc_1_05CA:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03CB
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_05E3
        jmp    NEAR PTR loc_1_0635
loc_1_05E3:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_023F
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_0602
        jmp    NEAR PTR loc_1_0635
loc_1_0602:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06A0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06F0
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_062B
        jmp    NEAR PTR loc_1_0635
loc_1_062B:
        mov    eax, 1
        jmp    NEAR PTR loc_1_063C
loc_1_0635:
        xor    eax, eax
        jmp    NEAR PTR loc_1_063C
loc_1_063C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0641:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_016D
        add    esp, 4
        push   2580h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06F0
        add    esp, 8
        db  068h, 000h, 000h, 000h, 000h
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_023F
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06A0
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_068D
loc_1_068D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_06A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06E8
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_06C5
        jmp    NEAR PTR loc_1_06D9
loc_1_06C5:
        push   32h
        call   sub_1_0140
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_06E0
loc_1_06D9:
        xor    eax, eax
        jmp    NEAR PTR loc_1_06E0
loc_1_06E0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh
sub_1_06E8:
        db  0CDh, 020h, 00Eh, 080h, 02Bh, 000h
        int3
        int3
sub_1_06F0:
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
        call   sub_1_0864
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0718
        xor    eax, eax
        jmp    NEAR PTR loc_1_075E
loc_1_0718:
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
        call   sub_1_085C
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_0754
        xor    eax, eax
        jmp    NEAR PTR loc_1_075E
loc_1_0754:
        mov    eax, 1
        jmp    NEAR PTR loc_1_075E
loc_1_075E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0763:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        call   sub_1_0139
        mov    dword ptr [ebp - 4], eax
loc_1_0774:
        lea    eax, [ebp - 8]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0874
        add    esp, 10h
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_07C1
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_086C
        add    esp, 0ch
        call   sub_1_0139
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, 0ch
        jb     NEAR PTR loc_1_07BC
        jmp    NEAR PTR loc_1_07D0
loc_1_07BC:
        jmp    NEAR PTR loc_1_07CB
loc_1_07C1:
        mov    eax, 1
        jmp    NEAR PTR loc_1_07D7
loc_1_07CB:
        jmp    NEAR PTR loc_1_0774
loc_1_07D0:
        xor    eax, eax
        jmp    NEAR PTR loc_1_07D7
loc_1_07D7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_07DC:
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
        call   sub_1_087C
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_0816
        cmp    dword ptr [ebp - 4], 1
        je     NEAR PTR loc_1_081D
loc_1_0816:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0827
loc_1_081D:
        mov    eax, 1
        jmp    NEAR PTR loc_1_0827
loc_1_0827:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_082C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_012D
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
        call   sub_1_0133
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh
sub_1_085C:
        db  0CDh, 020h, 005h, 080h, 02Bh, 000h
        int3
        int3
sub_1_0864:
        db  0CDh, 020h, 006h, 080h, 02Bh, 000h
        int3
        int3
sub_1_086C:
        db  0CDh, 020h, 00Bh, 080h, 02Bh, 000h
        int3
        int3
sub_1_0874:
        db  0CDh, 020h, 013h, 080h, 02Bh, 000h
        int3
        int3
sub_1_087C:
        db  0CDh, 020h, 012h, 080h, 02Bh, 000h
        int3
        int3
_LTEXT ends
        end
