; qemmfix_icode_02.asm -- QEMMFIX object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        mov    eax, 2
        mov    ecx, 8
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_2_004A
        db  089h, 035h, 08Ch, 000h, 000h, 000h
        mov    eax, 2
        mov    ecx, 14h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_2_004A
        db  089h, 035h, 090h, 000h, 000h, 000h
        mov    eax, 150007h
        db  0BEh, 08Ah, 001h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        jb     SHORT loc_2_004A
        db  089h, 035h, 094h, 000h, 000h, 000h
        clc
        ret
loc_2_004A:
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_2_0050:
        db  0BFh, 028h, 002h, 000h, 000h
        xor    esi, esi
        xor    edx, edx
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
        db  089h, 015h, 034h, 002h, 000h, 000h
        jb     SHORT loc_2_00B0
        jmp    SHORT loc_2_0084
loc_2_0069:
        xor    esi, esi
        db  08Bh, 015h, 034h, 002h, 000h, 000h
        db  0BFh, 028h, 002h, 000h, 000h
        db  0CDh, 020h, 0B4h, 000h, 001h, 000h
        db  089h, 015h, 034h, 002h, 000h, 000h
        jb     SHORT loc_2_00B0
loc_2_0084:
        db  08Bh, 035h, 08Ch, 000h, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        mov    edi, eax
        mov    esi, edx
        mov    ecx, 8
        cld
loc_2_00A0:
        lodsb
        or     al, al
        je     SHORT loc_2_00AA
        stosb
        loop   loc_2_00A0
        jmp    SHORT loc_2_0069
loc_2_00AA:
        mov    al, 20h
        rep stosb
        jmp    SHORT loc_2_0069
loc_2_00B0:
        clc
        ret
sub_2_00B2:
        push   ebx
        db  08Bh, 03Dh, 030h, 002h, 000h, 000h
        or     edi, edi
        jne    SHORT loc_2_0117
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    edi, dword ptr [ebx + 8]
        mov    word ptr [edi + 1ch], 5200h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  ebx, word ptr [edi + 38h]
        shl    ebx, 4
        movzx  edi, word ptr [edi + 10h]
        add    edi, ebx
        mov    edi, dword ptr [edi + 22h]
        db  089h, 03Dh, 030h, 002h, 000h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
loc_2_0117:
        pop    ebx
        ret
sub_2_0119:
        push   ecx
        movzx  ecx, di
        shr    edi, 10h
        shl    edi, 4
        add    edi, ecx
        test   byte ptr [edi + 5], 80h
        je     SHORT loc_2_013C
        push   esi
        push   edi
        lea    edi, [edi + 0ah]
        mov    ecx, 8
        cld
        repe cmpsb
        pop    edi
        pop    esi
        je     SHORT loc_2_013D
loc_2_013C:
        stc
loc_2_013D:
        pop    ecx
        ret
sub_2_013F:
        push   edi
loc_2_0140:
        cmp    di, -1
        je     SHORT loc_2_0187
        call   sub_2_0119
        jae    SHORT loc_2_0151
        mov    edi, dword ptr [edi]
        jmp    SHORT loc_2_0140
loc_2_0151:
        mov    eax, dword ptr [edi + 12h]
        cmp    eax, edi
        je     SHORT loc_2_0187
        mov    edi, eax
        call   sub_2_0119
        jb     SHORT loc_2_0187
        shr    eax, 10h
        mov    edx, eax
        dec    eax
        shl    eax, 4
        cmp    byte ptr [eax], 4dh
        je     SHORT loc_2_0174
        cmp    byte ptr [eax], 5ah
        jne    SHORT loc_2_0187
loc_2_0174:
        cmp    dx, word ptr [eax + 1]
        jne    SHORT loc_2_0187
        movzx  eax, word ptr [eax + 3]
        shl    eax, 4
        shl    edx, 4
        clc
        jmp    SHORT loc_2_0188
loc_2_0187:
        stc
loc_2_0188:
        pop    edi
        ret
loc_2_018A:
        pushad
        call   sub_2_01E9
        jb     SHORT loc_2_01A6
        call   sub_2_00B2
        call   sub_2_013F
        jb     SHORT loc_2_01A6
        call   sub_2_01AD
        popad
        clc
        ret
loc_2_01A6:
        popad
        db  0FFh, 025h, 094h, 000h, 000h, 000h
sub_2_01AD:
        pushad
        mov    edi, eax
        db  08Bh, 035h, 090h, 000h, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], edx
        mov    dword ptr [eax + 0ch], edi
        mov    dword ptr [eax + 10h], 200h
        push   0
        push   eax
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        popad
        ret
sub_2_01E9:
        push   esi
        push   edi
        push   ecx
        mov    edi, esi
        db  08Bh, 035h, 08Ch, 000h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_2_0216
loc_2_01FC:
        mov    esi, eax
        cld
        mov    ecx, 8
        repe cmpsb
        je     SHORT loc_2_0219
        db  08Bh, 035h, 08Ch, 000h, 000h, 000h
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_2_01FC
loc_2_0216:
        clc
        jmp    SHORT loc_2_021A
loc_2_0219:
        stc
loc_2_021A:
        pop    ecx
        pop    edi
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_0228:
        inc    edi
        dec    esp
        dec    edi
        inc    edx
        inc    ecx
        dec    esp
        add    byte ptr [eax], al
loc_2_0230:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0234:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_ITEXT ends
        end
