; act220l_lcode_01.asm -- ACT220L object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 041h, 043h, 054h, 032h
        db  032h, 030h, 04Ch, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  0CCh, 0CCh, 0CCh, 0CCh
loc_1_0060:
        inc    ecx
        inc    ebx
        push   esp
        sub    eax, 32325249h
        xor    byte ptr [eax + eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0070:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0080:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
ACT220L_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_009F
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_009F:
        clc
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_010D:
        db  0CDh, 020h, 023h, 080h, 0A2h, 004h
sub_1_0113:
        db  0CDh, 020h, 024h, 080h, 0A2h, 004h
sub_1_0119:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        ret
sub_1_0120:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_0119
        dec    eax
        mov    dword ptr [ebp - 4], eax
loc_1_0132:
        call   sub_1_0119
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp + 8]
        jae    NEAR PTR loc_1_0148
        jmp    NEAR PTR loc_1_0132
loc_1_0148:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_014D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], ecx
        push   0
        push   0
        push   6
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0420
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_0178
        jmp    NEAR PTR loc_1_01B5
loc_1_0178:
        push   32h
        call   sub_1_0120
        add    esp, 4
        push   0
        push   0
        push   5
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0420
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_01A1
        jmp    NEAR PTR loc_1_01B5
loc_1_01A1:
        push   32h
        call   sub_1_0120
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_01BC
loc_1_01B5:
        xor    eax, eax
        jmp    NEAR PTR loc_1_01BC
loc_1_01BC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_01C1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0330
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_01E0
        jmp    NEAR PTR loc_1_01F0
loc_1_01E0:
        mov    eax, dword ptr [ebp + 0ch]
        and    dword ptr [eax], 36h
        mov    eax, 1
        jmp    NEAR PTR loc_1_01F7
loc_1_01F0:
        xor    eax, eax
        jmp    NEAR PTR loc_1_01F7
loc_1_01F7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_01FC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_024C
loc_1_0210:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_0290
loc_1_021C:
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_1_0290
loc_1_0228:
        mov    dword ptr [ebp - 4], 2
        jmp    NEAR PTR loc_1_0290
loc_1_0234:
        mov    dword ptr [ebp - 4], 3
        jmp    NEAR PTR loc_1_0290
loc_1_0240:
        xor    eax, eax
        jmp    NEAR PTR loc_1_02F2
        db  0E9h, 044h, 000h, 000h, 000h
loc_1_024C:
        cmp    dword ptr [ebp - 0ch], 4b00h
        ja     NEAR PTR loc_1_0271
        je     NEAR PTR loc_1_021C
        cmp    dword ptr [ebp - 0ch], 2580h
        je     NEAR PTR loc_1_0210
        jmp    NEAR PTR loc_1_0240
loc_1_0271:
        cmp    dword ptr [ebp - 0ch], 0e100h
        je     NEAR PTR loc_1_0228
        cmp    dword ptr [ebp - 0ch], 1c200h
        je     NEAR PTR loc_1_0234
        jmp    NEAR PTR loc_1_0240
loc_1_0290:
        mov    ecx, dword ptr [ebp + 8]
        call   sub_1_014D
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_02A7
loc_1_02A4:
        inc    dword ptr [ebp - 8]
loc_1_02A7:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        jbe    NEAR PTR loc_1_02C4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03B0
        add    esp, 4
        jmp    NEAR PTR loc_1_02A4
loc_1_02C4:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0430
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_02E1
        jmp    NEAR PTR loc_1_02EB
loc_1_02E1:
        mov    eax, 1
        jmp    NEAR PTR loc_1_02F2
loc_1_02EB:
        xor    eax, eax
        jmp    NEAR PTR loc_1_02F2
loc_1_02F2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_02F7:
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
        call   sub_1_0420
        add    esp, 10h
        push   0
        push   0
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0420
        add    esp, 10h
        mov    eax, 1
        jmp    NEAR PTR loc_1_032B
loc_1_032B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0330:
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
        call   sub_1_0420
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_0355
        jmp    NEAR PTR loc_1_0398
loc_1_0355:
        push   0
        push   0
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0420
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_0374
        jmp    NEAR PTR loc_1_0398
loc_1_0374:
        push   32h
        call   sub_1_0120
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        call   sub_1_014D
        test   eax, eax
        je     NEAR PTR loc_1_0398
        mov    eax, 1
        jmp    NEAR PTR loc_1_039F
loc_1_0398:
        xor    eax, eax
        jmp    NEAR PTR loc_1_039F
loc_1_039F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_03B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0420
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_03D5
        jmp    NEAR PTR loc_1_0412
loc_1_03D5:
        push   32h
        call   sub_1_0120
        add    esp, 4
        push   0
        push   0
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0420
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_03FE
        jmp    NEAR PTR loc_1_0412
loc_1_03FE:
        push   32h
        call   sub_1_0120
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_0419
loc_1_0412:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0419
loc_1_0419:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh
sub_1_0420:
        db  0CDh, 020h, 00Eh, 080h, 02Bh, 000h
        int3
        int3
        int3
        int3
        int3
        int3
        int3
        int3
        int3
        int3
sub_1_0430:
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
        call   sub_1_05A4
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0458
        xor    eax, eax
        jmp    NEAR PTR loc_1_049E
loc_1_0458:
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
        call   sub_1_059C
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_0494
        xor    eax, eax
        jmp    NEAR PTR loc_1_049E
loc_1_0494:
        mov    eax, 1
        jmp    NEAR PTR loc_1_049E
loc_1_049E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        call   sub_1_0119
        mov    dword ptr [ebp - 4], eax
loc_1_04B4:
        lea    eax, [ebp - 8]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05B4
        add    esp, 10h
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_0501
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05AC
        add    esp, 0ch
        call   sub_1_0119
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, 0ch
        jb     NEAR PTR loc_1_04FC
        jmp    NEAR PTR loc_1_0510
loc_1_04FC:
        jmp    NEAR PTR loc_1_050B
loc_1_0501:
        mov    eax, 1
        jmp    NEAR PTR loc_1_0517
loc_1_050B:
        jmp    NEAR PTR loc_1_04B4
loc_1_0510:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0517
loc_1_0517:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
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
        call   sub_1_05BC
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_0556
        cmp    dword ptr [ebp - 4], 1
        je     NEAR PTR loc_1_055D
loc_1_0556:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0567
loc_1_055D:
        mov    eax, 1
        jmp    NEAR PTR loc_1_0567
loc_1_0567:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_056C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_010D
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
        call   sub_1_0113
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh
sub_1_059C:
        db  0CDh, 020h, 005h, 080h, 02Bh, 000h
        int3
        int3
sub_1_05A4:
        db  0CDh, 020h, 006h, 080h, 02Bh, 000h
        int3
        int3
sub_1_05AC:
        db  0CDh, 020h, 00Bh, 080h, 02Bh, 000h
        int3
        int3
sub_1_05B4:
        db  0CDh, 020h, 013h, 080h, 02Bh, 000h
        int3
        int3
sub_1_05BC:
        db  0CDh, 020h, 012h, 080h, 02Bh, 000h
        int3
        int3
_LTEXT ends
        end
