; vcd_pcode_06.asm -- VCD object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

VCD_PM_API:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_6_001F
        movzx  eax, word ptr [ebp + 14h]
        cmp    eax, 7
        jae    SHORT loc_6_001F
        and    byte ptr [ebp + 2ch], 0feh
        db  0FFh, 014h, 085h, 0F4h, 003h, 000h, 000h
        ret
loc_6_001F:
        mov    dword ptr [ebp + 1ch], 0ffffffffh
loc_6_0026:
        or     byte ptr [ebp + 2ch], 1
        ret
loc_6_002B:
        xor    eax, eax
        db  08Bh, 035h, 0DCh, 003h, 000h, 000h
        db  08Bh, 00Dh, 034h, 000h, 000h, 000h
        jecxz  loc_6_0058
        cmp    ecx, 10h
        jbe    SHORT loc_6_0045
        mov    ecx, 10h
loc_6_0045:
        mov    edx, 1
loc_6_004A:
        cmp    dword ptr [esi], 0
        je     SHORT loc_6_0051
        or     eax, edx
loc_6_0051:
        shl    edx, 1
        add    esi, 4
        loop   loc_6_004A
loc_6_0058:
        mov    word ptr [ebp + 1ch], ax
        ret
sub_6_005D:
        movzx  ecx, word ptr [ebp + 18h]
        db  03Bh, 00Dh, 034h, 000h, 000h, 000h
        jae    SHORT loc_6_0078
        shl    ecx, 2
        db  003h, 00Dh, 0DCh, 003h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        jecxz  loc_6_0078
        clc
        ret
loc_6_0078:
        stc
        ret
loc_6_007A:
        call   sub_6_005D
        jb     SHORT loc_6_0026
        mov    eax, dword ptr [ecx + 0ch]
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 10h], ax
        ret
loc_6_0090:
        call   sub_6_005D
        jb     SHORT loc_6_0026
        movzx  eax, word ptr [ebp + 10h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 1ch]
        mov    dword ptr [ecx + 0ch], eax
        ret
loc_6_00A6:
        xchg   esi, eax
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        movzx  edi, word ptr [ebp]
        movzx  ecx, word ptr [ebp + 1ch]
        btr    ecx, 1
        jae    NEAR PTR loc_6_0026
        movzx  eax, word ptr [ebp + 18h]
        inc    eax
        db  080h, 025h, 023h, 000h, 000h, 000h, 0FEh
        db  080h, 00Dh, 023h, 000h, 000h, 000h, 002h
        db  0CDh, 020h, 006h, 000h, 00Eh, 000h
        jb     SHORT loc_6_00FD
        mov    word ptr [ebp + 18h], cs
        db  0C7h, 045h, 000h, 0C1h, 002h, 000h, 000h
        mov    dword ptr [ebp + 10h], eax
        mov    ecx, dword ptr [eax + 18h]
        movzx  edx, byte ptr [eax + 5]
        mov    word ptr [ebp + 1ch], cx
        mov    word ptr [ebp + 14h], dx
        ret
loc_6_00FD:
        je     SHORT loc_6_0108
        and    byte ptr [ebp + 2ch], 0bfh
        jmp    NEAR PTR loc_6_0026
loc_6_0108:
        or     byte ptr [ebp + 2ch], 40h
        jmp    NEAR PTR loc_6_0026
loc_6_0111:
        call   sub_6_005D
        jb     SHORT loc_6_0120
        mov    eax, ecx
        db  0CDh, 020h, 007h, 000h, 00Eh, 000h
loc_6_0120:
        ret
loc_6_0121:
        mov    byte ptr [ebp + 1ch], 0
        jmp    NEAR PTR loc_6_0026
loc_6_012A:
        sub    esp, 4
        push   esi
        push   edi
        push   ebx
        lea    esi, [esp + 0ch]
        push   esi
        db  068h, 094h, 000h, 000h, 000h
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_6_0190
        db  0C7h, 005h, 00Dh, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  068h, 00Dh, 000h, 000h, 000h
        db  068h, 00Eh, 000h, 000h, 000h
        db  068h, 0D3h, 000h, 000h, 000h
        push   0
        db  068h, 0C6h, 000h, 000h, 000h
        push   dword ptr [esi]
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    edi, eax
        push   dword ptr [esi]
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        test   edi, edi
        je     SHORT loc_6_01BE
        cmp    edi, 0eah
        je     SHORT loc_6_01BE
loc_6_0190:
        db  0B8h, 00Dh, 000h, 000h, 000h
        push   4
        pop    ecx
        mov    dword ptr [eax], ecx
        push   0
        push   eax
        push   esi
        push   3
        db  068h, 0D8h, 000h, 000h, 000h
        push   0
        push   dword ptr [esp + 30h]
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_6_01D0
        cmp    dword ptr [esp + 0ch], eax
        je     SHORT loc_6_01D0
loc_6_01BE:
        mov    edx, dword ptr [esp + 1ch]
        add    dl, 2
        in     al, dx
        jmp    SHORT loc_6_01C8
loc_6_01C8:
        test   al, 30h
        jne    NEAR PTR loc_6_0301
loc_6_01D0:
        mov    ecx, dword ptr [esp + 18h]
        db  0E8h, 0A7h, 009h, 0FAh, 0FFh
        je     SHORT loc_6_01F7
        mov    eax, dword ptr [esp + 1ch]
        push   4
        pop    ecx
        db  0BFh, 000h, 000h, 000h, 000h
        cld
        repne scasw
        jne    NEAR PTR loc_6_0301
        xor    eax, eax
        mov    al, 4
        sub    eax, ecx
loc_6_01F7:
        mov    dword ptr [esp + 0ch], eax
        db  0F6h, 005h, 008h, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_6_02B9
        mov    ebx, eax
        db  0BEh, 0DCh, 003h, 000h, 000h
        db  03Bh, 005h, 034h, 000h, 000h, 000h
        jbe    SHORT loc_6_0237
        db  0A3h, 034h, 000h, 000h, 000h
        shl    eax, 2
        push   1
        push   eax
        push   dword ptr [esi]
        db  0CDh, 020h, 050h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_6_0301
        mov    dword ptr [esi], eax
loc_6_0237:
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + ebx*4 - 4], 0
        jne    NEAR PTR loc_6_02FD
        mov    ecx, dword ptr [esp + 20h]
        mov    esi, dword ptr [esp + 1ch]
        shl    esi, 10h
        movzx  si, cl
        mov    edx, dword ptr [esp + 0ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edi, edi
        xor    ebx, ebx
        db  0F6h, 005h, 00Bh, 000h, 000h, 000h, 00Ch
        je     SHORT loc_6_026B
        or     bl, 1
loc_6_026B:
        db  0CDh, 020h, 00Bh, 000h, 00Eh, 000h
        jb     NEAR PTR loc_6_0301
        push   esi
        lea    edi, [esi + 10h]
        db  0BEh, 011h, 000h, 000h, 000h
        push   8
        pop    ecx
loc_6_0283:
        lodsb
        stosb
        dec    ecx
        or     al, al
        jne    SHORT loc_6_0283
        dec    edi
        inc    ecx
        mov    al, 20h
        rep stosb
        pop    esi
        mov    ebx, dword ptr [esp + 18h]
        mov    dword ptr [esi + 64h], ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
loc_6_029E:
        push   esi
        or     byte ptr [esi + 7], 10h
        db  0E8h, 072h, 003h, 0FAh, 0FFh
        pop    esi
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_6_029E
        jmp    SHORT loc_6_02FD
loc_6_02B9:
        db  0E8h, 069h, 0FDh, 0F9h, 0FFh
        jb     SHORT loc_6_0301
        db  08Bh, 035h, 0D8h, 003h, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_6_0301
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    edi, eax
        mov    ebx, edi
        xor    eax, eax
        mov    ecx, 18h
        rep stosb
        mov    eax, dword ptr [esp + 20h]
        mov    byte ptr [ebx + 2], al
        mov    eax, dword ptr [esp + 1ch]
        mov    word ptr [ebx], ax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [ebx + 10h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ebx + 14h], eax
loc_6_02FD:
        or     al, 1
        jmp    SHORT loc_6_0303
loc_6_0301:
        xor    eax, eax
loc_6_0303:
        pop    ebx
        pop    edi
        pop    esi
        add    esp, 4
        ret
loc_6_030A:
        push   esi
        push   edi
        push   ebx
        db  0F7h, 005h, 020h, 004h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_6_037D
        db  08Bh, 035h, 0DCh, 003h, 000h, 000h
        mov    eax, dword ptr [esp + 14h]
        db  08Bh, 00Dh, 034h, 000h, 000h, 000h
        jecxz  loc_6_037D
loc_6_032B:
        mov    edx, dword ptr [esi]
        test   edx, edx
        je     SHORT loc_6_0336
        cmp    dword ptr [edx + 18h], eax
        je     SHORT loc_6_033D
loc_6_0336:
        add    esi, 4
        loop   loc_6_032B
        jmp    SHORT loc_6_037D
loc_6_033D:
        mov    dword ptr [esi], 0
        mov    esi, edx
        movzx  edx, byte ptr [esi + 4]
        mov    ecx, dword ptr [esi + 18h]
        db  0CDh, 020h, 00Ch, 000h, 00Eh, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
loc_6_0358:
        mov    eax, dword ptr [ebx + 4]
        cmp    word ptr [eax + edx*2 + 3feh], cx
        jne    SHORT loc_6_036F
        mov    word ptr [eax + edx*2 + 3feh], 0
loc_6_036F:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_6_0358
loc_6_037D:
        pop    ebx
        pop    edi
        pop    esi
        ret
        db  021h, 056h, 043h
_PTEXT ends
        end
