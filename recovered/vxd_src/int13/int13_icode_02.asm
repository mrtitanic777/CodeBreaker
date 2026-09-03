; int13_icode_02.asm -- INT13 object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        xor    esi, esi
        lea    eax, [esi - 1]
        db  0BFh, 0D1h, 000h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        jae    SHORT loc_2_0021
        xor    eax, eax
        db  0BFh, 0C4h, 000h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        not    eax
loc_2_0021:
        xor    ecx, ecx
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        db  0A3h, 0E0h, 009h, 000h, 000h
        db  0C7h, 005h, 0ECh, 000h, 000h, 000h, 0CCh, 006h, 000h, 000h
        db  0C7h, 005h, 0F0h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  068h, 0E0h, 000h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 010h, 000h
        add    esp, 4
        ret
loc_2_0051:
        clc
        ret
loc_2_0053:
        db  0C6h, 005h, 02Ah, 00Ah, 000h, 000h, 001h
        db  080h, 03Dh, 0E8h, 009h, 000h, 000h, 000h
        je     SHORT loc_2_00B1
        db  0E8h, 0F4h, 0FFh, 0FDh, 0FFh
        push   0
        push   4
        push   0f003eh
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    dword ptr [eax], 6c6c6544h
        jne    SHORT loc_2_00B1
        push   0
        push   1
        push   4b4h
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        test   byte ptr [eax], 20h
        jne    SHORT loc_2_00B1
        cld
        db  0BEh, 018h, 001h, 000h, 000h
        mov    dx, 5bh
        mov    ecx, 4
loc_2_00A8:
        lodsb
        out    dx, al
        jmp    SHORT loc_2_00AC
loc_2_00AC:
        jmp    SHORT loc_2_00AE
loc_2_00AE:
        dec    edx
        loop   loc_2_00A8
loc_2_00B1:
        clc
        ret
Int13_Device_Registered:
        pushad
        sub    edi, 0c3h
        db  0E8h, 053h, 0FFh, 0FDh, 0FFh
        popad
        ret
        db  000h, 000h, 000h
loc_2_00C4:
        dec    ecx
        dec    esi
        inc    esp
        dec    edi
        push   ebx
        push   eax
        dec    edi
        dec    esp
        dec    esp
        dec    ecx
        dec    esi
        inc    edi
        db  000h
loc_2_00D1:
        dec    edi
        db  076h, 065h
        db  072h, 06Ch
        popad
        db  070h, 070h
        dec    ecx
        dec    edi
        add    byte ptr [eax], al
        db  000h
loc_2_00E0:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 69h], al
        db  078h, 065h
        and    byte ptr fs:[ecx + ebp*2 + 73h], al
        imul   esp, dword ptr [eax], 49h
        dec    esi
        push   esp
        xor    dword ptr [ebx], esi
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
loc_2_0118:
        push   edi
        dec    ecx
        dec    esi
        db  034h
_ITEXT ends
        end
