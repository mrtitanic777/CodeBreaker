; mssp_lcode_01.asm -- MSSP object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

SECPROV_Control:
        sub    eax, 1
        cmp    eax, 23h
        jae    SHORT loc_1_000F
        db  0FFh, 024h, 085h, 011h, 000h, 000h, 000h
loc_1_000F:
        clc
        ret
        db  000h, 000h, 000h, 000h, 00Bh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 007h, 000h, 000h, 000h
loc_1_009D:
        push   dword ptr [esp]
        mov    dword ptr [esp + 4], edx
        db  0E9h, 096h, 010h, 0FFh, 0FFh
loc_1_00A9:
        push   dword ptr [esp]
        mov    dword ptr [esp + 4], edx
        db  0E9h, 0EFh, 010h, 0FFh, 0FFh
sub_1_00B5:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 0FBh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        pop    edi
        leave
        ret    0ch
sub_1_00D1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 1ch]
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    eax, esi
        pop    esi
        pop    edi
        pop    ebx
        leave
        ret    18h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0100:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    ebp, 1
        mov    edi, dword ptr [esp + 18h]
        mov    ebx, dword ptr [esp + 1ch]
loc_1_0115:
        or     edi, edi
        jne    SHORT loc_1_0143
        cmp    dword ptr [esi + 4], ebp
        jb     SHORT loc_1_0139
        or     ebx, ebx
        je     SHORT loc_1_0154
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -10h
        je     SHORT loc_1_0139
        lea    eax, [esi + 10h]
        push   eax
        push   ebx
        call   sub_1_016B
        or     eax, eax
        jne    SHORT loc_1_0154
loc_1_0139:
        or     edi, edi
        jne    SHORT loc_1_0143
        cmp    dword ptr [esi + 4], 2
        jae    SHORT loc_1_015B
loc_1_0143:
        cmp    dword ptr [esi], 0
        je     SHORT loc_1_0162
        push   ebp
        push   esi
        call   sub_1_019C
        add    esp, 8
        jmp    SHORT loc_1_0115
loc_1_0154:
        mov    eax, 1
        jmp    SHORT loc_1_0164
loc_1_015B:
        mov    eax, 2
        jmp    SHORT loc_1_0164
loc_1_0162:
        sub    eax, eax
loc_1_0164:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
sub_1_016B:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
loc_1_0173:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    SHORT loc_1_0193
        or     dl, dl
        je     SHORT loc_1_018F
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    SHORT loc_1_0193
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    SHORT loc_1_0173
loc_1_018F:
        sub    eax, eax
        jmp    SHORT loc_1_0198
loc_1_0193:
        sbb    eax, eax
        sbb    eax, -1
loc_1_0198:
        ret    8
        db  000h
sub_1_019C:
        db  0CDh, 020h, 0FAh, 080h, 001h, 000h
        add    byte ptr [eax], al
        db  000h, 000h, 000h, 000h, 092h, 007h, 000h, 000h, 062h, 009h, 000h, 000h, 002h, 00Dh, 000h, 000h
        db  043h, 00Dh, 000h, 000h, 009h, 00Eh, 000h, 000h, 0C7h, 009h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 093h, 004h, 004h, 000h, 000h, 000h, 053h, 045h, 043h, 050h
        db  052h, 04Fh, 056h, 020h, 000h, 0FFh, 002h, 0A0h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A4h, 001h, 000h, 000h, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
_LTEXT ends
        end
