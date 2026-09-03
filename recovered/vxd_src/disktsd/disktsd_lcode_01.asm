; disktsd_lcode_01.asm -- DISKTSD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

DiskTSD_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        clc
        ret
        db  000h
loc_1_000C:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        mov    si, word ptr [ebx]
        cmp    si, 0
        db  00Fh, 084h, 0DDh, 0FFh, 0FEh, 0FFh
        cmp    si, 3
        db  00Fh, 084h, 05Fh, 000h, 0FFh, 0FFh
        cmp    si, 4
        db  00Fh, 084h, 01Eh, 001h, 0FFh, 0FFh
        cmp    si, 2
        db  00Fh, 084h, 0DCh, 001h, 0FFh, 0FFh
        call   sub_1_0047
        ret
sub_1_0047:
        mov    word ptr [ebx + 2], 0ffffh
        ret
        db  0C3h, 000h
sub_1_0050:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_005C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0064:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_006C:
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
loc_1_0084:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    bx, word ptr [edi + 68h]
        cmp    bl, 2
        ja     SHORT loc_1_009D
        call   sub_1_0120
loc_1_0099:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_009D:
        cmp    bl, 0ch
        je     SHORT loc_1_00AE
        cmp    bl, 0dh
        je     SHORT loc_1_00C9
loc_1_00A7:
        call   sub_1_0135
        jmp    SHORT loc_1_0099
loc_1_00AE:
        mov    esi, dword ptr [edi + 4]
        push   edi
        db  0E8h, 0F1h, 004h, 0FFh, 0FFh
        pop    edi
loc_1_00B8:
        mov    eax, dword ptr [edi + 14h]
        sub    eax, 8
        mov    dword ptr [edi + 14h], eax
        push   edi
        call   dword ptr [eax]
        add    esp, 4
        jmp    SHORT loc_1_0099
loc_1_00C9:
        test   byte ptr [edi + 6dh], 40h
        jne    SHORT loc_1_00E1
        mov    ebx, dword ptr [edi + 4]
        test   byte ptr [ebx + 0b8h], 4
        jne    SHORT loc_1_00E1
        test   byte ptr [ebx + 22h], 8
        jne    SHORT loc_1_0107
loc_1_00E1:
        movzx  ebx, word ptr [edi + 92h]
        and    ebx, 0ffh
        cmp    ebx, 12h
        jg     SHORT loc_1_00A7
        db  0FFh, 014h, 09Dh, 0E8h, 012h, 000h, 000h
        or     al, al
        je     SHORT loc_1_00B8
        dec    al
        je     SHORT loc_1_0099
        jmp    SHORT loc_1_00A7
loc_1_0107:
        mov    ebx, 1fh
        cmp    word ptr [edi + 6ah], 18h
        jne    SHORT loc_1_0118
        mov    ebx, 15h
loc_1_0118:
        mov    dword ptr [edi + 0a0h], ebx
        jmp    SHORT loc_1_00B8
sub_1_0120:
        mov    eax, dword ptr [edi + 8]
        test   byte ptr [eax + 21h], 40h
        je     SHORT loc_1_012F
        test   byte ptr [eax + 16h], 80h
        jne    SHORT loc_1_0159
loc_1_012F:
        test   byte ptr [edi + 6eh], 8
        jne    SHORT loc_1_0147
sub_1_0135:
        mov    eax, dword ptr [edi + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [edi + 10h], eax
        push   edi
        call   dword ptr [eax]
        add    esp, 4
        xor    eax, eax
        ret
loc_1_0147:
        bts    dword ptr [edi + 6ch], 14h
        jb     SHORT sub_1_0135
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebx + 28h]
        add    dword ptr [edi + 74h], eax
        jmp    SHORT sub_1_0135
loc_1_0159:
        mov    word ptr [edi + 6ah], 20h
        mov    eax, dword ptr [edi + 14h]
        sub    eax, 8
        mov    dword ptr [edi + 14h], eax
        push   edi
        call   dword ptr [eax]
        add    esp, 4
        ret
loc_1_016F:
        enter  0, 0
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    word ptr [edi + 6ah], 2
        ja     SHORT loc_1_01A7
        cmp    word ptr [edi + 68h], 0eh
        je     SHORT loc_1_01EF
loc_1_0185:
        mov    dword ptr [edi + 0a0h], 0
loc_1_018F:
        mov    word ptr [edi + 68h], 0dh
        mov    eax, dword ptr [edi + 14h]
        sub    eax, 8
        mov    dword ptr [edi + 14h], eax
        push   edi
        call   dword ptr [eax]
        add    esp, 4
loc_1_01A4:
        pop    edi
        leave
        ret
loc_1_01A7:
        mov    ebx, dword ptr [edi + 14h]
        mov    ebx, dword ptr [ebx + 4]
        dec    word ptr [ebx]
        je     SHORT loc_1_01DA
        cmp    word ptr [edi + 68h], 2
        jne    SHORT loc_1_01C9
        cmp    word ptr [edi + 92h], 42h
        jne    SHORT loc_1_01C9
        mov    word ptr [edi + 68h], 0eh
loc_1_01C9:
        push   esi
        db  0BEh, 02Dh, 002h, 000h, 000h
        mov    edx, edi
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    esi
        jmp    SHORT loc_1_01A4
loc_1_01DA:
        movzx  ecx, word ptr [edi + 6ah]
        db  0CDh, 020h, 00Fh, 000h, 010h, 000h
        mov    dword ptr [edi + 0a0h], eax
        jmp    SHORT loc_1_018F
loc_1_01EC:
        pop    esi
        jmp    SHORT loc_1_0185
loc_1_01EF:
        push   esi
        mov    esi, dword ptr [edi + 8]
        test   byte ptr [esi + 24h], 40h
        jne    SHORT loc_1_01EC
        movzx  eax, word ptr [edi + 5eh]
        mov    ecx, dword ptr [esi + 60h]
        mul    ecx
        or     edx, edx
        mov    ecx, dword ptr [edi + 60h]
        mov    dword ptr [edi + 7ch], ecx
        mul    ecx
        or     edx, edx
        mov    ebx, eax
        movzx  eax, byte ptr [edi + 5ch]
        mul    ecx
        or     edx, edx
        add    eax, ebx
        mov    dword ptr [edi + 74h], eax
        mov    dword ptr [edi + 78h], 0
        mov    word ptr [edi + 68h], 2
        pop    esi
        jmp    SHORT loc_1_01C9
loc_1_022D:
        push   esi
        push   edi
        mov    esi, edx
        call   sub_1_0244
        add    dword ptr [esi + 14h], 8
        mov    edi, esi
        call   sub_1_0135
        pop    edi
        pop    esi
        ret
sub_1_0244:
        push   ebx
        mov    ebx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebx + 8]
loc_1_024B:
        db  081h, 038h, 084h, 000h, 000h, 000h
        je     SHORT loc_1_025A
        mov    eax, dword ptr [eax + 0ch]
        or     eax, eax
        jne    SHORT loc_1_024B
loc_1_025A:
        mov    dword ptr [esi + 10h], eax
        pop    ebx
        ret
        db  000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 044h, 069h, 073h, 06Bh
        db  054h, 053h, 044h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 039h, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_02BC:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
_LTEXT ends
        end
