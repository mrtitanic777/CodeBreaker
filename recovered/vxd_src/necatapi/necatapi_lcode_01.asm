; necatapi_lcode_01.asm -- NECATAPI object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

NECATAPI_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        clc
        ret
        db  000h
loc_1_000C:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        mov    si, word ptr [ebx]
        cmp    si, 3
        db  00Fh, 084h, 0DDh, 0FFh, 0FEh, 0FFh
        cmp    si, 2
        db  00Fh, 084h, 054h, 000h, 0FFh, 0FFh
        mov    word ptr [ebx + 2], 0ffffh
        ret
loc_1_0034:
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [esp + 10h]
        cmp    word ptr [esi + 6ah], 10h
        jae    SHORT loc_1_005F
        mov    edi, dword ptr [esi + 8]
        cmp    dword ptr [edi + 0a2h], 31302e31h
        jne    SHORT loc_1_0056
        call   sub_1_0094
loc_1_0056:
        call   sub_1_0084
loc_1_005B:
        pop    ebx
        pop    esi
        pop    edi
        ret
loc_1_005F:
        call   sub_1_0081
        or     al, al
        je     SHORT loc_1_0056
        cmp    al, 2
        je     SHORT loc_1_005B
        add    dword ptr [esi + 14h], 8
        mov    eax, dword ptr [esi + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 10h], eax
        push   esi
        call   dword ptr [eax]
        add    esp, 4
        jmp    SHORT loc_1_005B
sub_1_0081:
        xor    al, al
        ret
sub_1_0084:
        mov    edi, dword ptr [esi + 14h]
        sub    edi, 8
        mov    dword ptr [esi + 14h], edi
        push   esi
        call   dword ptr [edi]
        add    esp, 4
        ret
sub_1_0094:
        push   ebx
        mov    ebx, dword ptr [esi + 20h]
        cmp    byte ptr [ebx + 30h], 43h
        jne    SHORT loc_1_00BA
        cmp    byte ptr [ebx + 39h], 40h
        jne    SHORT loc_1_00AB
        call   sub_1_00E9
        jmp    SHORT loc_1_00E7
loc_1_00AB:
        cmp    byte ptr [ebx + 39h], 0
        jne    SHORT loc_1_00B8
        call   sub_1_0107
        jmp    SHORT loc_1_00E7
loc_1_00B8:
        jmp    SHORT loc_1_00E7
loc_1_00BA:
        cmp    byte ptr [ebx + 30h], 42h
        jne    SHORT loc_1_00C7
        call   sub_1_013D
        jmp    SHORT loc_1_00E7
loc_1_00C7:
        cmp    byte ptr [ebx + 30h], 47h
        jne    SHORT loc_1_00D4
        call   sub_1_0169
        jmp    SHORT loc_1_00E7
loc_1_00D4:
        cmp    byte ptr [ebx + 30h], 44h
        jne    SHORT loc_1_00E7
        test   byte ptr [ebx + 31h], 2
        je     SHORT loc_1_00E7
        call   sub_1_017A
        jmp    SHORT loc_1_00E7
loc_1_00E7:
        pop    ebx
        ret
sub_1_00E9:
        mov    edi, dword ptr [esi + 80h]
        cmp    byte ptr [edi + 3], 1
        je     SHORT loc_1_00F9
        mov    byte ptr [edi + 3], 2
loc_1_00F9:
        add    edi, 8
        mov    ecx, 4
        call   sub_1_018E
        ret
sub_1_0107:
        push   ebx
        mov    ebx, 64h
        mov    edi, dword ptr [esi + 80h]
        add    edi, 4
loc_1_0116:
        mov    ecx, 4
loc_1_011B:
        mov    al, byte ptr [edi + 4]
        call   sub_1_019B
        mov    byte ptr [edi + 4], al
        inc    edi
        loop   loc_1_011B
        sub    edi, 4
        mov    al, byte ptr [edi + 2]
        cmp    al, 0aah
        je     SHORT loc_1_013B
        dec    ebx
        je     SHORT loc_1_013B
        add    edi, 8
        jmp    SHORT loc_1_0116
loc_1_013B:
        pop    ebx
        ret
sub_1_013D:
        mov    eax, dword ptr [esi + 7ch]
        cmp    eax, 0ch
        jl     SHORT loc_1_015A
        mov    edi, dword ptr [esi + 80h]
        cmp    word ptr [edi + 2], 0
        je     SHORT loc_1_015A
        add    edi, 4
        cmp    byte ptr [edi], 1
        je     SHORT loc_1_015B
loc_1_015A:
        ret
loc_1_015B:
        mov    ecx, 4
        add    ecx, 4
        call   sub_1_018E
        ret
sub_1_0169:
        mov    ecx, 3
        add    ecx, 3
        lea    edi, [ebx + 33h]
        call   sub_1_018E
        ret
sub_1_017A:
        mov    edi, dword ptr [esi + 80h]
        add    edi, 4
        mov    ecx, 4
        call   sub_1_018E
        ret
sub_1_018E:
        mov    al, byte ptr [edi]
        call   sub_1_019B
        mov    byte ptr [edi], al
        inc    edi
        loop   sub_1_018E
        ret
sub_1_019B:
        push   ecx
        mov    ch, al
        mov    cl, 4
        shr    al, cl
        mov    cl, 0ah
        mul    cl
        and    ch, 0fh
        add    al, ch
        pop    ecx
        ret
        db  000h, 000h, 000h
sub_1_01B0:
        pop    ds
        add    eax, dword ptr [eax]
        db  000h, 01Fh
        add    eax, dword ptr [eax]
        db  000h, 01Fh
        add    eax, dword ptr [eax]
        db  000h, 01Fh
        add    eax, dword ptr [eax]
        db  000h, 01Fh
        add    eax, dword ptr [eax]
        db  000h, 01Fh
        add    eax, dword ptr [eax]
        db  000h, 01Fh
        add    eax, dword ptr [eax]
        db  000h, 01Fh
        add    eax, dword ptr [eax]
        db  000h, 01Fh
        add    eax, dword ptr [eax]
        db  000h, 01Fh
        add    eax, dword ptr [eax]
        db  000h, 01Fh
        add    eax, dword ptr [eax]
        db  000h, 01Fh
        add    eax, dword ptr [eax]
        db  000h, 01Fh
        add    eax, dword ptr [eax]
        db  000h, 01Fh
        add    eax, dword ptr [eax]
        db  000h
loc_1_01E8:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 14h]
        db  0C7h, 000h, 034h, 000h, 000h, 000h
        mov    edx, dword ptr [ebx + 10h]
        mov    dword ptr [eax + 4], edx
        add    dword ptr [ebx + 14h], 8
        mov    esi, dword ptr [ebx + 4]
        test   byte ptr [ebx + 6dh], 80h
        jne    SHORT loc_1_0269
        movzx  eax, word ptr [ebx + 68h]
        cmp    eax, 0eh
        jae    SHORT loc_1_0222
        db  0FFh, 014h, 085h, 0B0h, 001h, 000h, 000h
loc_1_021E:
        or     al, al
        jne    SHORT loc_1_0235
loc_1_0222:
        mov    eax, dword ptr [ebx + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebx + 10h], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
loc_1_0231:
        pop    esi
        pop    edi
        pop    ebp
        ret
loc_1_0235:
        dec    al
        jne    SHORT loc_1_024A
        mov    eax, dword ptr [ebx + 14h]
        sub    eax, 10h
        mov    dword ptr [ebx + 14h], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
        jmp    SHORT loc_1_0231
loc_1_024A:
        dec    al
        je     SHORT loc_1_0231
        mov    al, 1
        mov    word ptr [ebx + 6ah], 16h
        cmp    word ptr [ebx + 68h], 0dh
        jne    SHORT loc_1_0235
        mov    dword ptr [ebx + 0a0h], 1fh
        jmp    SHORT loc_1_0235
loc_1_0269:
        mov    edi, dword ptr [ebx + 20h]
        mov    al, byte ptr [edi + 30h]
        cmp    dword ptr [esi + 0a2h], 31302e31h
        jne    SHORT loc_1_0280
        call   sub_1_0284
loc_1_0280:
        xor    al, al
        jmp    SHORT loc_1_021E
sub_1_0284:
        cmp    al, 0beh
        jne    SHORT loc_1_028E
        mov    byte ptr [edi + 30h], 0d4h
        jmp    SHORT loc_1_02F1
loc_1_028E:
        cmp    al, 47h
        jne    SHORT loc_1_0299
        call   sub_1_02F2
        jmp    SHORT loc_1_02F1
loc_1_0299:
        cmp    al, 0d5h
        jne    SHORT loc_1_02A4
        call   sub_1_02F2
        jmp    SHORT loc_1_02F1
loc_1_02A4:
        cmp    al, 1ah
        jne    SHORT loc_1_02BF
        mov    al, byte ptr [edi + 32h]
        mov    ah, al
        and    ah, 0c0h
        and    al, 3fh
        cmp    al, 2ah
        jne    SHORT loc_1_02B8
        mov    al, 0fh
loc_1_02B8:
        or     al, ah
        mov    byte ptr [edi + 32h], al
        jmp    SHORT loc_1_02F1
loc_1_02BF:
        cmp    al, 43h
        jne    SHORT loc_1_02CF
        cmp    byte ptr [edi + 39h], 80h
        jne    SHORT loc_1_02CF
        mov    byte ptr [edi + 39h], 0
        jmp    SHORT loc_1_02F1
loc_1_02CF:
        cmp    al, 49h
        jne    SHORT loc_1_02E0
        mov    al, byte ptr [edi + 36h]
        call   sub_1_030C
        mov    byte ptr [edi + 36h], al
        jmp    SHORT loc_1_02F1
loc_1_02E0:
        cmp    al, 0a9h
        jne    SHORT loc_1_02F1
        mov    al, byte ptr [edi + 3ah]
        call   sub_1_030C
        mov    byte ptr [edi + 3ah], al
        jmp    SHORT loc_1_02F1
loc_1_02F1:
        ret
sub_1_02F2:
        push   edi
        push   ebx
        push   ecx
        mov    ecx, 6
loc_1_02FA:
        mov    al, byte ptr [edi + 33h]
        call   sub_1_030C
        mov    byte ptr [edi + 33h], al
        inc    edi
        loop   loc_1_02FA
        pop    ecx
        pop    ebx
        pop    edi
        ret
sub_1_030C:
        push   ebx
        xor    ah, ah
        mov    bl, 0ah
        div    bl
        shl    al, 1
        shl    al, 1
        shl    al, 1
        shl    al, 1
        add    al, ah
        pop    ebx
        ret
loc_1_031F:
        xor    al, al
        ret
        db  000h, 000h
sub_1_0324:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 004h, 000h, 000h, 000h, 04Eh, 045h, 043h, 041h
        db  054h, 041h, 050h, 049h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 024h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
_LTEXT ends
        end
