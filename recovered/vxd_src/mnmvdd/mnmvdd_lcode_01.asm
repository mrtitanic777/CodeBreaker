; mnmvdd_lcode_01.asm -- MNMVDD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

DCGVDD_Control:
        clc
        ret
DCGVDD_PM_API:
        mov    eax, dword ptr [ebp + 1ch]
        cmp    eax, 6
        jae    SHORT loc_1_0014
        shl    eax, 2
        db  02Eh, 0FFh, 0A0h, 000h, 000h, 000h, 000h
loc_1_0014:
        or     word ptr [ebp + 2ch], 1
        ret
loc_1_001A:
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
loc_1_0032:
        mov    word ptr [ebp + 1ch], 100h
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_1_003E:
        push   es
        push   esi
        push   eax
        push   ebx
        push   ecx
        and    word ptr [ebp + 2ch], 0fffeh
        mov    ecx, 1
        mov    eax, dword ptr [ebp + 18h]
        test   eax, 100h
        je     SHORT loc_1_0064
        shl    eax, 8
        or     eax, 0e0h
        mov    ecx, 2
loc_1_0064:
        db  0A3h, 000h, 000h, 000h, 000h
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 00Dh, 000h
        jae    SHORT loc_1_007F
        mov    dword ptr [ebp + 18h], ecx
        or     word ptr [ebp + 2ch], 1
loc_1_007F:
        pop    ecx
        pop    ebx
        pop    eax
        pop    esi
        pop    es
        ret
loc_1_0085:
        mov    ebx, dword ptr [ebp + 10h]
        mov    eax, 0fh
        mov    edx, 0dh
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        ret
loc_1_0099:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
loc_1_009F:
        push   ebx
        db  0CDh, 020h, 005h, 000h, 017h, 000h
        cmp    dx, word ptr [ebp + 14h]
        pop    ebx
        je     SHORT loc_1_00BD
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_1_009F
        xor    ebx, ebx
loc_1_00BD:
        mov    dword ptr [ebp + 14h], ebx
        ret
loc_1_00C1:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    dword ptr [ebp + 10h], ebx
        ret
loc_1_00CB:
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    dword ptr [ebp + 14h], edi
        ret
_LTEXT ends
        end
