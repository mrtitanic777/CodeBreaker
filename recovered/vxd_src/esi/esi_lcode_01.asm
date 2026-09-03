; esi_lcode_01.asm -- ESI object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 045h, 053h, 049h, 020h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  0CCh, 0CCh, 0CCh, 0CCh
loc_1_0060:
        inc    ebp
        push   ebx
        dec    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_0078:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
ESI_Control:
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
loc_1_0110:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   2580h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_013C
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        and    dword ptr [eax], 26h
        mov    eax, 1
        jmp    NEAR PTR loc_1_0137
loc_1_0137:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_013C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_0195
loc_1_0150:
        mov    dword ptr [ebp - 4], 3
        mov    dword ptr [ebp - 8], 6
        jmp    NEAR PTR loc_1_01C1
loc_1_0163:
        mov    dword ptr [ebp - 4], 5
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_01C1
loc_1_0176:
        mov    dword ptr [ebp - 4], 3
        mov    dword ptr [ebp - 8], 5
        jmp    NEAR PTR loc_1_01C1
loc_1_0189:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0231
        db  0E9h, 02Ch, 000h, 000h, 000h
loc_1_0195:
        cmp    dword ptr [ebp - 0ch], 2580h
        je     NEAR PTR loc_1_0150
        cmp    dword ptr [ebp - 0ch], 4b00h
        je     NEAR PTR loc_1_0163
        cmp    dword ptr [ebp - 0ch], 1c200h
        je     NEAR PTR loc_1_0176
        jmp    NEAR PTR loc_1_0189
loc_1_01C1:
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_01D2:
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_01E2
        jmp    NEAR PTR loc_1_022A
loc_1_01E2:
        push   0
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_01F3:
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_0203
        jmp    NEAR PTR loc_1_022A
loc_1_0203:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0240
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0220
        jmp    NEAR PTR loc_1_022A
loc_1_0220:
        mov    eax, 1
        jmp    NEAR PTR loc_1_0231
loc_1_022A:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0231
loc_1_0231:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_1_0240:
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
        call   sub_1_03B4
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0268
        xor    eax, eax
        jmp    NEAR PTR loc_1_02AE
loc_1_0268:
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
        call   sub_1_03AC
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_02A4
        xor    eax, eax
        jmp    NEAR PTR loc_1_02AE
loc_1_02A4:
        mov    eax, 1
        jmp    NEAR PTR loc_1_02AE
loc_1_02AE:
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
        call   sub_1_0109
        mov    dword ptr [ebp - 4], eax
loc_1_02C4:
        lea    eax, [ebp - 8]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03C4
        add    esp, 10h
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_0311
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_03BC
        add    esp, 0ch
        call   sub_1_0109
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, 0ch
        jb     NEAR PTR loc_1_030C
        jmp    NEAR PTR loc_1_0320
loc_1_030C:
        jmp    NEAR PTR loc_1_031B
loc_1_0311:
        mov    eax, 1
        jmp    NEAR PTR loc_1_0327
loc_1_031B:
        jmp    NEAR PTR loc_1_02C4
loc_1_0320:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0327
loc_1_0327:
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
        call   sub_1_03CC
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_0366
        cmp    dword ptr [ebp - 4], 1
        je     NEAR PTR loc_1_036D
loc_1_0366:
        xor    eax, eax
        jmp    NEAR PTR loc_1_0377
loc_1_036D:
        mov    eax, 1
        jmp    NEAR PTR loc_1_0377
loc_1_0377:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_037C:
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
sub_1_03AC:
        db  0CDh, 020h, 005h, 080h, 02Bh, 000h
        int3
        int3
sub_1_03B4:
        db  0CDh, 020h, 006h, 080h, 02Bh, 000h
        int3
        int3
sub_1_03BC:
        db  0CDh, 020h, 00Bh, 080h, 02Bh, 000h
        int3
        int3
sub_1_03C4:
        db  0CDh, 020h, 013h, 080h, 02Bh, 000h
        int3
        int3
sub_1_03CC:
        db  0CDh, 020h, 012h, 080h, 02Bh, 000h
        int3
        int3
_LTEXT ends
        end
