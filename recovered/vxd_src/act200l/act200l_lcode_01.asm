; act200l_lcode_01.asm -- ACT200L object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 041h, 043h, 054h, 032h
        db  030h, 030h, 04Ch, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  08Fh, 095h, 011h, 000h
loc_1_0070:
        xchg   dword ptr [ecx - 6f74ffefh], edx
        adc    dword ptr [eax], eax
loc_1_0078:
        test   dword ptr [eax - 6f7cffefh], edx
        adc    dword ptr [eax], eax
loc_1_0080:
        adc    dword ptr [eax + 43410011h], 52492d54h
        xor    dh, byte ptr [eax]
        xor    byte ptr [eax + eax], cl
loc_1_0090:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00A0:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
ACT200L_Control:
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
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_0165
        jmp    NEAR PTR loc_1_0179
loc_1_0165:
        push   32h
        call   sub_1_05F7
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_0180
loc_1_0179:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0180
loc_1_0180:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0185:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_01AD
        jmp    NEAR PTR loc_1_0318
loc_1_01AD:
        push   0
        push   0
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_01CC
        jmp    NEAR PTR loc_1_0318
loc_1_01CC:
        push   0
        push   0
        push   0dh
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_01EB
        jmp    NEAR PTR loc_1_0318
loc_1_01EB:
        push   32h
        call   sub_1_05F7
        add    esp, 4
        push   0
        push   0
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_0214
        jmp    NEAR PTR loc_1_0318
loc_1_0214:
        push   0
        push   0
        push   0ch
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_0233
        jmp    NEAR PTR loc_1_0318
loc_1_0233:
        push   32h
        call   sub_1_05F7
        add    esp, 4
        push   0
        push   0
        push   0dh
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_025C
        jmp    NEAR PTR loc_1_0318
loc_1_025C:
        push   0
        push   0
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_027B
        jmp    NEAR PTR loc_1_0318
loc_1_027B:
        push   32h
        call   sub_1_05F7
        add    esp, 4
        push   0
        push   0
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0778
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_02A4
        jmp    NEAR PTR loc_1_0318
loc_1_02A4:
        push   32h
        call   sub_1_05F7
        add    esp, 4
        push   0f0h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_086C
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_02CC
        jmp    NEAR PTR loc_1_0318
loc_1_02CC:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07F3
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_02E9
        jmp    NEAR PTR loc_1_0318
loc_1_02E9:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0f4h
        je     NEAR PTR loc_1_030E
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0f5h
        je     NEAR PTR loc_1_030E
        jmp    NEAR PTR loc_1_0318
loc_1_030E:
        mov    eax, 1
        jmp    NEAR PTR loc_1_031F
loc_1_0318:
        xor    eax, eax
        jmp    NEAR PTR loc_1_031F
loc_1_031F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0324:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_033C
loc_1_0339:
        inc    dword ptr [ebp - 4]
loc_1_033C:
        cmp    dword ptr [ebp - 4], 5
        jae    NEAR PTR loc_1_039A
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0185
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_05D4
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_0386
        db  068h, 000h, 000h, 000h, 000h
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0624
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_0386
        jmp    NEAR PTR loc_1_039A
loc_1_0386:
        cmp    dword ptr [ebp - 4], 4
        jne    NEAR PTR loc_1_0395
        jmp    NEAR PTR loc_1_03A4
loc_1_0395:
        jmp    NEAR PTR loc_1_0339
loc_1_039A:
        mov    eax, 1
        jmp    NEAR PTR loc_1_03AB
loc_1_03A4:
        xor    eax, eax
        jmp    NEAR PTR loc_1_03AB
loc_1_03AB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_03B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0324
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_03CF
        jmp    NEAR PTR loc_1_03EB
loc_1_03CF:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06E0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        and    dword ptr [eax], 3fh
        mov    eax, 1
        jmp    NEAR PTR loc_1_03F2
loc_1_03EB:
        xor    eax, eax
        jmp    NEAR PTR loc_1_03F2
loc_1_03F2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_03F7:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_045F
loc_1_040B:
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_04C8
loc_1_0417:
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_04C8
loc_1_0423:
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_04C8
loc_1_042F:
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_04C8
loc_1_043B:
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_04C8
loc_1_0447:
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_04C8
loc_1_0453:
        xor    eax, eax
        jmp    NEAR PTR loc_1_057E
        db  0E9h, 069h, 000h, 000h, 000h
loc_1_045F:
        cmp    dword ptr [ebp - 8], 2580h
        ja     NEAR PTR loc_1_0484
        je     NEAR PTR loc_1_0417
        cmp    dword ptr [ebp - 8], 960h
        je     NEAR PTR loc_1_040B
        jmp    NEAR PTR loc_1_0453
loc_1_0484:
        cmp    dword ptr [ebp - 8], 9600h
        ja     NEAR PTR loc_1_04A9
        je     NEAR PTR loc_1_042F
        cmp    dword ptr [ebp - 8], 4b00h
        je     NEAR PTR loc_1_0423
        jmp    NEAR PTR loc_1_0453
loc_1_04A9:
        cmp    dword ptr [ebp - 8], 0e100h
        je     NEAR PTR loc_1_043B
        cmp    dword ptr [ebp - 8], 1c200h
        je     NEAR PTR loc_1_0447
        jmp    NEAR PTR loc_1_0453
loc_1_04C8:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0140
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0624
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_0544
        push   2580h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0780
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_050C
        jmp    NEAR PTR loc_1_0577
loc_1_050C:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0324
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_0525
        jmp    NEAR PTR loc_1_0577
loc_1_0525:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0624
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_0544
        jmp    NEAR PTR loc_1_0577
loc_1_0544:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06E0
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0780
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_056D
        jmp    NEAR PTR loc_1_0577
loc_1_056D:
        mov    eax, 1
        jmp    NEAR PTR loc_1_057E
loc_1_0577:
        xor    eax, eax
        jmp    NEAR PTR loc_1_057E
loc_1_057E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0583:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0185
        add    esp, 4
        push   2580h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0780
        add    esp, 8
        db  068h, 000h, 000h, 000h, 000h
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0624
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0730
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_05CF
loc_1_05CF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_05D4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        push   8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0624
        add    esp, 0ch
        jmp    NEAR PTR loc_1_05F2
loc_1_05F2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_05F7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_0139
        dec    eax
        mov    dword ptr [ebp - 4], eax
loc_1_0609:
        call   sub_1_0139
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp + 8]
        jae    NEAR PTR loc_1_061F
        jmp    NEAR PTR loc_1_0609
loc_1_061F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0624:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_062D:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07F3
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_064A
        jmp    NEAR PTR loc_1_062D
loc_1_064A:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_0659
loc_1_0656:
        inc    dword ptr [ebp - 4]
loc_1_0659:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 4], eax
        jae    NEAR PTR loc_1_06CA
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + ecx]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_086C
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0688
        jmp    NEAR PTR loc_1_06D4
loc_1_0688:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_07F3
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_06A5
        jmp    NEAR PTR loc_1_06D4
loc_1_06A5:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        cmp    eax, ecx
        je     NEAR PTR loc_1_06C5
        jmp    NEAR PTR loc_1_06D4
loc_1_06C5:
        jmp    NEAR PTR loc_1_0656
loc_1_06CA:
        mov    eax, 1
        jmp    NEAR PTR loc_1_06DB
loc_1_06D4:
        xor    eax, eax
        jmp    NEAR PTR loc_1_06DB
loc_1_06DB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_06E0:
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
        call   sub_1_0778
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_0705
        jmp    NEAR PTR loc_1_0719
loc_1_0705:
        push   32h
        call   sub_1_05F7
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_0720
loc_1_0719:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0720
loc_1_0720:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_0730:
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
        call   sub_1_0778
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_0755
        jmp    NEAR PTR loc_1_0769
loc_1_0755:
        push   32h
        call   sub_1_05F7
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_0770
loc_1_0769:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0770
loc_1_0770:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh
sub_1_0778:
        db  0CDh, 020h, 00Eh, 080h, 02Bh, 000h
        int3
        int3
sub_1_0780:
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
        call   sub_1_08F4
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_07A8
        xor    eax, eax
        jmp    NEAR PTR loc_1_07EE
loc_1_07A8:
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
        call   sub_1_08EC
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_07E4
        xor    eax, eax
        jmp    NEAR PTR loc_1_07EE
loc_1_07E4:
        mov    eax, 1
        jmp    NEAR PTR loc_1_07EE
loc_1_07EE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_07F3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        call   sub_1_0139
        mov    dword ptr [ebp - 4], eax
loc_1_0804:
        lea    eax, [ebp - 8]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0904
        add    esp, 10h
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_0851
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_08FC
        add    esp, 0ch
        call   sub_1_0139
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, 0ch
        jb     NEAR PTR loc_1_084C
        jmp    NEAR PTR loc_1_0860
loc_1_084C:
        jmp    NEAR PTR loc_1_085B
loc_1_0851:
        mov    eax, 1
        jmp    NEAR PTR loc_1_0867
loc_1_085B:
        jmp    NEAR PTR loc_1_0804
loc_1_0860:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0867
loc_1_0867:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_086C:
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
        call   sub_1_090C
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_08A6
        cmp    dword ptr [ebp - 4], 1
        je     NEAR PTR loc_1_08AD
loc_1_08A6:
        xor    eax, eax
        jmp    NEAR PTR loc_1_08B7
loc_1_08AD:
        mov    eax, 1
        jmp    NEAR PTR loc_1_08B7
loc_1_08B7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_08BC:
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
sub_1_08EC:
        db  0CDh, 020h, 005h, 080h, 02Bh, 000h
        int3
        int3
sub_1_08F4:
        db  0CDh, 020h, 006h, 080h, 02Bh, 000h
        int3
        int3
sub_1_08FC:
        db  0CDh, 020h, 00Bh, 080h, 02Bh, 000h
        int3
        int3
sub_1_0904:
        db  0CDh, 020h, 013h, 080h, 02Bh, 000h
        int3
        int3
sub_1_090C:
        db  0CDh, 020h, 012h, 080h, 02Bh, 000h
        int3
        int3
_LTEXT ends
        end
