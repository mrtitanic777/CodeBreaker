; parallax_lcode_01.asm -- PARALLAX object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 050h, 041h, 052h, 041h
        db  04Ch, 04Ch, 041h, 058h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  0CCh, 0CCh, 0CCh, 0CCh
loc_1_0060:
        push   eax
        inc    ecx
        push   edx
        inc    ecx
        dec    esp
        dec    esp
        inc    ecx
        pop    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
PARALLAX_Control:
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
loc_1_0120:
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
        call   sub_1_0380
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_0145
        jmp    NEAR PTR loc_1_019C
loc_1_0145:
        push   0
        push   0
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0380
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_0164
        jmp    NEAR PTR loc_1_019C
loc_1_0164:
        push   1
        call   sub_1_0353
        add    esp, 4
        push   2580h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_01A8
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_018C
        jmp    NEAR PTR loc_1_019C
loc_1_018C:
        mov    eax, dword ptr [ebp + 0ch]
        and    dword ptr [eax], 3fh
        mov    eax, 1
        jmp    NEAR PTR loc_1_01A3
loc_1_019C:
        xor    eax, eax
        jmp    NEAR PTR loc_1_01A3
loc_1_01A3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_01A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0380
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_01D0
        jmp    NEAR PTR loc_1_0347
loc_1_01D0:
        push   1
        call   sub_1_0353
        add    esp, 4
        push   0
        push   0
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0380
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_01F9
        jmp    NEAR PTR loc_1_0347
loc_1_01F9:
        push   1
        call   sub_1_0353
        add    esp, 4
        cmp    dword ptr [ebp + 0ch], 1c200h
        jne    NEAR PTR loc_1_021C
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_02B7
loc_1_021C:
        cmp    dword ptr [ebp + 0ch], 0e100h
        jne    NEAR PTR loc_1_0235
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_1_02B7
loc_1_0235:
        cmp    dword ptr [ebp + 0ch], 9600h
        jne    NEAR PTR loc_1_024E
        mov    dword ptr [ebp - 4], 2
        jmp    NEAR PTR loc_1_02B7
loc_1_024E:
        cmp    dword ptr [ebp + 0ch], 4b00h
        jne    NEAR PTR loc_1_0267
        mov    dword ptr [ebp - 4], 3
        jmp    NEAR PTR loc_1_02B7
loc_1_0267:
        cmp    dword ptr [ebp + 0ch], 2580h
        jne    NEAR PTR loc_1_0280
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_1_02B7
loc_1_0280:
        cmp    dword ptr [ebp + 0ch], 12c0h
        jne    NEAR PTR loc_1_0299
        mov    dword ptr [ebp - 4], 5
        jmp    NEAR PTR loc_1_02B7
loc_1_0299:
        cmp    dword ptr [ebp + 0ch], 960h
        jne    NEAR PTR loc_1_02B2
        mov    dword ptr [ebp - 4], 6
        jmp    NEAR PTR loc_1_02B7
loc_1_02B2:
        jmp    NEAR PTR loc_1_0347
loc_1_02B7:
        jmp    NEAR PTR loc_1_02BF
loc_1_02BC:
        dec    dword ptr [ebp - 4]
loc_1_02BF:
        cmp    dword ptr [ebp - 4], 0
        jle    NEAR PTR loc_1_0320
        push   0
        push   0
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0380
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_02E8
        jmp    NEAR PTR loc_1_0347
loc_1_02E8:
        push   1
        call   sub_1_0353
        add    esp, 4
        push   0
        push   0
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0380
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_0311
        jmp    NEAR PTR loc_1_0347
loc_1_0311:
        push   1
        call   sub_1_0353
        add    esp, 4
        jmp    NEAR PTR loc_1_02BC
loc_1_0320:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0390
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_033D
        jmp    NEAR PTR loc_1_0347
loc_1_033D:
        mov    eax, 1
        jmp    NEAR PTR loc_1_034E
loc_1_0347:
        xor    eax, eax
        jmp    NEAR PTR loc_1_034E
loc_1_034E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0353:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_0119
        dec    eax
        mov    dword ptr [ebp - 4], eax
loc_1_0365:
        call   sub_1_0119
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp + 8]
        jae    NEAR PTR loc_1_037B
        jmp    NEAR PTR loc_1_0365
loc_1_037B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0380:
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
sub_1_0390:
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
        call   sub_1_0504
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_03B8
        xor    eax, eax
        jmp    NEAR PTR loc_1_03FE
loc_1_03B8:
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
        call   sub_1_04FC
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_03F4
        xor    eax, eax
        jmp    NEAR PTR loc_1_03FE
loc_1_03F4:
        mov    eax, 1
        jmp    NEAR PTR loc_1_03FE
loc_1_03FE:
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
loc_1_0414:
        lea    eax, [ebp - 8]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0514
        add    esp, 10h
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_0461
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_050C
        add    esp, 0ch
        call   sub_1_0119
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, 0ch
        jb     NEAR PTR loc_1_045C
        jmp    NEAR PTR loc_1_0470
loc_1_045C:
        jmp    NEAR PTR loc_1_046B
loc_1_0461:
        mov    eax, 1
        jmp    NEAR PTR loc_1_0477
loc_1_046B:
        jmp    NEAR PTR loc_1_0414
loc_1_0470:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0477
loc_1_0477:
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
        call   sub_1_051C
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_04B6
        cmp    dword ptr [ebp - 4], 1
        je     NEAR PTR loc_1_04BD
loc_1_04B6:
        xor    eax, eax
        jmp    NEAR PTR loc_1_04C7
loc_1_04BD:
        mov    eax, 1
        jmp    NEAR PTR loc_1_04C7
loc_1_04C7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_04CC:
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
sub_1_04FC:
        db  0CDh, 020h, 005h, 080h, 02Bh, 000h
        int3
        int3
sub_1_0504:
        db  0CDh, 020h, 006h, 080h, 02Bh, 000h
        int3
        int3
sub_1_050C:
        db  0CDh, 020h, 00Bh, 080h, 02Bh, 000h
        int3
        int3
sub_1_0514:
        db  0CDh, 020h, 013h, 080h, 02Bh, 000h
        int3
        int3
sub_1_051C:
        db  0CDh, 020h, 012h, 080h, 02Bh, 000h
        int3
        int3
_LTEXT ends
        end
