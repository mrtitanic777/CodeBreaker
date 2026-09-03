; vwin32_lcode_01.asm -- VWIN32 object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VWIN32_Control:
        cmp    eax, 0
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 032h, 000h, 0FFh, 0FFh
        cmp    eax, 1dh
        db  00Fh, 084h, 026h, 02Eh, 0FFh, 0FFh
        cmp    eax, 1eh
        db  00Fh, 084h, 07Bh, 02Eh, 0FFh, 0FFh
        cmp    eax, 1fh
        db  00Fh, 084h, 0AFh, 02Eh, 0FFh, 0FFh
        cmp    eax, 21h
        db  00Fh, 084h, 0F6h, 02Eh, 0FFh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 0CBh, 001h, 0FFh, 0FFh
        cmp    eax, 15h
        db  00Fh, 084h, 006h, 004h, 0FFh, 0FFh
        cmp    eax, 16h
        db  00Fh, 084h, 0B6h, 004h, 0FFh, 0FFh
        cmp    eax, 23h
        db  00Fh, 084h, 0F6h, 008h, 0FFh, 0FFh
        clc
        ret
VWIN32_Get_Version:
        mov    eax, 103h
        clc
        ret
VWIN32_GetCurrentDirectory:
        push   ebx
        push   edi
        push   esi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_00A6
        db  08Bh, 00Dh, 0DCh, 010h, 000h, 000h
        jecxz  loc_1_00A6
        test   dword ptr [ecx + 20h], 8
        jne    SHORT loc_1_00A6
        mov    ecx, dword ptr [ecx + 40h]
        jecxz  loc_1_00A6
        mov    ecx, dword ptr [ecx + 0ch]
        jecxz  loc_1_00A6
        mov    edi, ecx
        push   edi
        xor    eax, eax
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        mov    edi, esi
        pop    esi
        rep movsb
        mov    eax, ecx
        jmp    SHORT loc_1_00AB
loc_1_00A6:
        mov    eax, 0ffffffffh
loc_1_00AB:
        pop    esi
        pop    edi
        pop    ebx
        ret
VWIN32_IFSRIPWhenLev2Taken:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        pop    ebx
        jne    SHORT loc_1_00D1
        db  0A1h, 0D8h, 010h, 000h, 000h
        db  08Bh, 015h, 0A4h, 002h, 000h, 000h
        mov    edx, dword ptr [edx + 8]
        sub    eax, edx
loc_1_00D1:
        ret
VWIN32_GetCurrentProcessHandle:
        db  0A1h, 0DCh, 010h, 000h, 000h
        clc
        ret
loc_1_00D9:
        mov    ebx, edx
VWIN32_DIOCCompletionRoutine:
        test   ebx, ebx
        je     SHORT loc_1_010B
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    edx, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        cmp    edx, ebx
        pop    ebx
        je     SHORT loc_1_0104
        push   esi
        db  0BEh, 0D9h, 000h, 000h, 000h
        xchg   ebx, edx
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
        pop    esi
        jmp    SHORT loc_1_010B
loc_1_0104:
        mov    eax, ebx
        db  0E8h, 056h, 03Dh, 0FFh, 0FFh
loc_1_010B:
        clc
        ret
VWIN32_ActiveTimeBiasSet:
        clc
        ret
_VWIN32_IsClientWin32:
        push   ebx
        push   edi
        push   ebp
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    ebx, dword ptr [edi + 14h]
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_0145
        mov    ebp, dword ptr [edi + 10h]
        movzx  eax, word ptr [ebp + 28h]
        db  066h, 03Bh, 005h, 060h, 003h, 000h, 000h
        jne    SHORT loc_1_0145
        movzx  eax, word ptr [ebp + 34h]
        db  066h, 02Bh, 005h, 062h, 003h, 000h, 000h
        jne    SHORT loc_1_0145
        inc    eax
loc_1_0141:
        pop    ebp
        pop    edi
        pop    ebx
        ret
loc_1_0145:
        sub    eax, eax
        jmp    SHORT loc_1_0141
        db  000h, 000h, 000h
VWIN32_BlueScreenPopup:
        xor    ecx, ecx
        test   dl, 1
        je     SHORT loc_1_0162
        db  0CDh, 020h, 000h, 000h, 002h, 000h
        or     eax, eax
        je     SHORT loc_1_0162
        mov    ecx, 1
loc_1_0162:
        db  0CDh, 020h, 001h, 000h, 009h, 000h
        push   edx
        sub    esp, 50h
        mov    eax, 1
        test   edx, 2
        je     SHORT loc_1_019F
        mov    edx, cs
        cmp    bx, dx
        jne    SHORT loc_1_019F
        mov    edx, esp
        push   ecx
        push   edx
        push   edi
        db  0CDh, 020h, 07Eh, 001h, 001h, 000h
        add    esp, 8
        pop    ecx
        or     eax, eax
        mov    eax, 1
        je     SHORT loc_1_019F
        mov    eax, 2
        mov    edx, esp
loc_1_019F:
        or     ecx, 40001000h
        add    eax, 180000h
        push   edx
        push   edi
        push   ebx
        push   esi
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        xchg   ecx, eax
        xor    edi, edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        add    esp, dword ptr [esp]
        add    esp, 50h
        pop    edx
        db  0CDh, 020h, 002h, 000h, 009h, 000h
        cmp    eax, 2
        je     SHORT loc_1_01F0
        test   dl, 4
        jne    SHORT loc_1_01EC
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  0CDh, 020h, 018h, 000h, 02Ah, 000h
loc_1_01EC:
        xor    eax, eax
        jmp    SHORT loc_1_01F3
loc_1_01F0:
        xor    eax, eax
        inc    eax
loc_1_01F3:
        ret
sub_1_01F4:
        push   edi
        db  08Bh, 03Dh, 0E0h, 010h, 000h, 000h
loc_1_01FB:
        mov    eax, dword ptr [esi]
        cmp    eax, -1
        jne    SHORT loc_1_021A
        mov    dword ptr [edi + 44h], esi
        push   1
        push   esi
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
        xor    eax, eax
        xchg   dword ptr [edi + 44h], eax
        or     eax, eax
        jne    SHORT loc_1_01FB
loc_1_021A:
        pop    edi
        ret
loc_1_021C:
        xor    ecx, ecx
        xchg   dword ptr [edx + 18h], ecx
        jecxz  loc_1_0241
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xor    eax, eax
        db  08Bh, 00Dh, 01Ch, 011h, 000h, 000h
        mov    eax, 1000h
        db  0BEh, 08Fh, 019h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
loc_1_0241:
        ret
loc_1_0242:
        xor    ecx, ecx
        xchg   dword ptr [edx + 18h], ecx
        jecxz  loc_1_0267
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xor    eax, eax
        db  08Bh, 00Dh, 01Ch, 011h, 000h, 000h
        mov    eax, 1000h
        db  0BEh, 0CEh, 019h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
loc_1_0267:
        ret
loc_1_0268:
        enter  8, 0
        push   ebx
        push   edi
        push   esi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    esi, edi
        xor    ecx, ecx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        mov    dword ptr [ebp - 8], eax
        movzx  eax, word ptr [esi + 2ah]
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 0A2h, 011h, 000h, 000h
        mov    ecx, 1
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        db  0A1h, 09Eh, 011h, 000h, 000h
        mov    word ptr [esi + 2ah], ax
        push   31h
        db  0FFh, 035h, 0B6h, 011h, 000h, 000h
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        add    esp, 8
        mov    edi, dword ptr [ebp + 8]
        mov    edi, dword ptr [edi]
        rol    edi, 3
        mov    ebx, edi
        shr    ebx, 16h
        db  00Fh, 0B7h, 01Ch, 05Dh, 09Ch, 011h, 000h, 000h
        mov    edx, edi
        and    edx, 7ffffh
        shl    edx, 9
        shld   ecx, edx, 10h
        mov    ax, 4200h
        push   21h
        db  0CDh, 020h, 03Bh, 001h, 001h, 000h
        jb     SHORT loc_1_0316
        mov    edx, edi
        mov    ecx, 1000h
        and    edx, 380000h
        je     SHORT loc_1_0307
        mov    ecx, edx
        shr    ecx, 13h
        neg    ecx
        and    ecx, 7
        shl    ecx, 9
loc_1_0307:
        mov    edx, dword ptr [ebp + 0ch]
        mov    ax, 3f00h
        push   21h
        db  0CDh, 020h, 03Bh, 001h, 001h, 000h
loc_1_0316:
        sbb    ebx, ebx
        inc    ebx
        db  0FFh, 035h, 0B6h, 011h, 000h, 000h
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        add    esp, 4
        or     ebx, ebx
        je     SHORT loc_1_0348
        mov    ecx, edi
        and    ecx, 380000h
        je     SHORT loc_1_0348
        shr    ecx, 0ch
        mov    edi, dword ptr [ebp + 0ch]
        add    edi, 0ffch
        sub    eax, eax
        std
        rep stosd
        cld
loc_1_0348:
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 2ah], ax
        pop    eax
        pop    esi
        pop    edi
        pop    ebx
        leave
        ret
sub_1_036A:
        enter  8, 0
        push   ebx
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        cmp    ebx, 0ffffh
        jb     SHORT loc_1_039F
        mov    eax, 8000d600h
        mov    al, byte ptr [ebp + 20h]
        sub    al, 3fh
        xor    ecx, ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebp + 10h]
        db  0CDh, 020h, 032h, 000h, 040h, 000h
        sbb    eax, eax
        inc    eax
        jmp    NEAR PTR loc_1_0460
loc_1_039F:
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    esi, edi
        xor    ecx, ecx
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        mov    dword ptr [ebp - 8], eax
        movzx  eax, word ptr [esi + 2ah]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, 1
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        pop    ebx
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [esi + 2ah], ax
        push   31h
        db  0FFh, 035h, 0B2h, 011h, 000h, 000h
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        add    esp, 8
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        shld   ecx, edx, 10h
        mov    ax, 4200h
        push   21h
        db  0CDh, 020h, 03Bh, 001h, 001h, 000h
        jb     SHORT loc_1_0410
        mov    edx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    ah, byte ptr [ebp + 20h]
        push   21h
        db  0CDh, 020h, 03Bh, 001h, 001h, 000h
loc_1_0410:
        sbb    ebx, ebx
        inc    ebx
        db  0FFh, 035h, 0B2h, 011h, 000h, 000h
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        add    esp, 4
        or     ebx, ebx
        je     SHORT loc_1_0443
        cmp    byte ptr [ebp + 20h], 40h
        je     SHORT loc_1_0443
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 14h]
        mov    edi, dword ptr [ebp + 10h]
        add    edi, ecx
        neg    ecx
        add    ecx, 1000h
        shr    ecx, 2
        rep stosd
loc_1_0443:
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 2ah], ax
        pop    eax
loc_1_0460:
        pop    esi
        pop    edi
        pop    ebx
        leave
        ret
loc_1_0465:
        enter  8, 0
        push   ebx
        push   edi
        push   esi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    esi, edi
        xor    ecx, ecx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        mov    dword ptr [ebp - 8], eax
        movzx  eax, word ptr [esi + 2ah]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, 1
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [esi + 2ah], ax
        push   31h
        db  0FFh, 035h, 0B2h, 011h, 000h, 000h
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        add    esp, 8
        push   esi
        mov    ax, 716ch
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dx, 1
        xor    cx, cx
        push   21h
        db  0CDh, 020h, 03Bh, 001h, 001h, 000h
        pushfd
        xor    ebx, ebx
        popfd
        jae    SHORT loc_1_04D8
        or     ebx, 0ffff0000h
loc_1_04D8:
        mov    bx, ax
        pop    esi
        push   ebx
        db  0FFh, 035h, 0B2h, 011h, 000h, 000h
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 2ah], ax
        pop    eax
        pop    esi
        pop    edi
        pop    ebx
        leave
        ret
loc_1_050D:
        enter  8, 0
        push   ebx
        push   edi
        push   esi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    esi, edi
        xor    ecx, ecx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        mov    dword ptr [ebp - 8], eax
        movzx  eax, word ptr [esi + 2ah]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, 1
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [esi + 2ah], ax
        push   31h
        db  0FFh, 035h, 0B2h, 011h, 000h, 000h
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        add    esp, 8
        mov    ebx, dword ptr [ebp + 8]
        mov    ax, 3e00h
        push   21h
        db  0CDh, 020h, 03Bh, 001h, 001h, 000h
        pushfd
        xor    eax, eax
        popfd
        jb     SHORT loc_1_0570
        inc    eax
loc_1_0570:
        push   eax
        db  0FFh, 035h, 0B2h, 011h, 000h, 000h
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 2ah], ax
        pop    eax
        pop    esi
        pop    edi
        pop    ebx
        leave
        ret
sub_1_05A1:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        db  0C7h, 003h, 0E4h, 003h, 000h, 000h
        mov    ebx, dword ptr [ebp + 0ch]
        db  0C7h, 003h, 0F9h, 003h, 000h, 000h
        mov    ebx, dword ptr [ebp + 10h]
        db  0C7h, 003h, 017h, 004h, 000h, 000h
        mov    ebx, dword ptr [ebp + 14h]
        db  0C7h, 003h, 008h, 004h, 000h, 000h
        mov    ebx, dword ptr [ebp + 18h]
        db  0C7h, 003h, 026h, 004h, 000h, 000h
        mov    ebx, dword ptr [ebp + 1ch]
        db  0C7h, 003h, 03Bh, 004h, 000h, 000h
        mov    ebx, dword ptr [ebp + 20h]
        db  0C7h, 003h, 065h, 004h, 000h, 000h
        mov    ebx, dword ptr [ebp + 24h]
        db  0C7h, 003h, 00Dh, 005h, 000h, 000h
        mov    ebx, dword ptr [ebp + 28h]
        db  0C7h, 003h, 0B6h, 00Fh, 000h, 000h
        mov    ebx, dword ptr [ebp + 2ch]
        db  0C7h, 003h, 00Ch, 012h, 000h, 000h
        db  0B8h, 06Ah, 003h, 000h, 000h
        pop    ebx
        leave
        ret
        db  000h
sub_1_0608:
        db  08Bh, 015h, 0E0h, 010h, 000h, 000h
        or     byte ptr [edx + 2ch], 10h
loc_1_0612:
        lea    eax, [edx + 289816h]
        push   11h
        push   eax
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
        test   byte ptr [edx + 2ch], 10h
        jne    SHORT loc_1_0612
        ret
sub_1_062B:
        db  08Bh, 015h, 0E0h, 010h, 000h, 000h
loc_1_0631:
        test   byte ptr [edx + 2eh], 4
        je     SHORT loc_1_064B
        lea    eax, [edx + 569595h]
        push   11h
        push   eax
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_1_0631
loc_1_064B:
        ret
sub_1_064C:
        push   ebx
        mov    ebx, dword ptr [esp + 8]
        mov    eax, dword ptr [ebx + 5ch]
        db  03Bh, 005h, 014h, 011h, 000h, 000h
        je     SHORT loc_1_0674
        mov    fs, word ptr [ebx + 1ch]
        mov    word ptr fs:[6], 1
        push   -1
        call   sub_1_0813
        add    esp, 4
loc_1_0674:
        or     al, 0ffh
        db  086h, 005h, 0C0h, 011h, 000h, 000h
        or     al, al
        je     SHORT loc_1_06A1
        push   esi
        push   edi
        mov    fs, word ptr [ebx + 1ch]
        mov    word ptr fs:[6], 1
        db  0CDh, 020h, 00Ah, 001h, 001h, 000h
        xor    ecx, ecx
        db  0E8h, 022h, 0FFh, 0FEh, 0FFh
        pop    edi
        pop    esi
        jmp    SHORT loc_1_0674
loc_1_06A1:
        pop    ebx
        ret    4
sub_1_06A5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  08Bh, 015h, 0ECh, 010h, 000h, 000h
        mov    edx, dword ptr [edx + edi]
        btr    dword ptr [edx + 2ch], 2
        jae    SHORT loc_1_06DD
        cmp    dword ptr [edx + 20h], 0
        je     SHORT loc_1_06DD
loc_1_06C7:
        lea    eax, [edx + 20h]
        push   11h
        push   eax
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
        btr    dword ptr [edx + 2ch], 3
        jae    SHORT loc_1_06C7
loc_1_06DD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_06E4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  08Bh, 03Dh, 0ECh, 010h, 000h, 000h
        mov    edi, dword ptr [edi + esi]
        mov    ebx, dword ptr [edi + 20h]
        cmp    ebx, 7fh
        jge    SHORT loc_1_0737
        inc    dword ptr [edi + 20h]
        or     ebx, ebx
        jg     SHORT loc_1_072C
        btr    dword ptr [edi + 2ch], 3
        jb     SHORT loc_1_072C
        bts    dword ptr [edi + 2ch], 2
        jb     SHORT loc_1_072C
        push   2
        push   esi
        push   0
        db  068h, 0A5h, 006h, 000h, 000h
        call   _VWIN32_QueueKernelAPC
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_0733
        mov    dword ptr [edi + 24h], eax
loc_1_072C:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0733:
        and    byte ptr [edi + 2ch], 0fbh
loc_1_0737:
        mov    ebx, 0ffffffffh
        jmp    SHORT loc_1_072C
sub_1_073E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        mov    edi, dword ptr [ebp + 8]
        db  08Bh, 015h, 0ECh, 010h, 000h, 000h
        mov    edx, dword ptr [edx + edi]
        mov    ebx, dword ptr [edx + 20h]
        or     ebx, ebx
        je     SHORT loc_1_0773
        dec    dword ptr [edx + 20h]
        jne    SHORT loc_1_0773
        call   sub_1_0779
        jb     SHORT loc_1_0773
        or     byte ptr [edx + 2ch], 8
        lea    eax, [edx + 20h]
        push   eax
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
loc_1_0773:
        mov    eax, ebx
        pop    edi
        pop    ebx
        leave
        ret
sub_1_0779:
        btr    dword ptr [edx + 2ch], 2
        jae    SHORT loc_1_078F
        xor    eax, eax
        xchg   dword ptr [edx + 24h], eax
        push   edx
        push   esi
        call   sub_1_0A44
        pop    esi
        pop    edx
        stc
loc_1_078F:
        ret
sub_1_0790:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  08Bh, 015h, 0E0h, 010h, 000h, 000h
        mov    dword ptr [edx + 10h], 0
        cmp    dword ptr [edx + 1ch], 0
        jg     SHORT loc_1_07F4
        mov    eax, dword ptr [ebp + 8]
        inc    eax
        je     SHORT loc_1_07F4
        dec    eax
        jne    SHORT loc_1_07D1
        db  0CDh, 020h, 061h, 001h, 001h, 000h
        or     byte ptr [edx + 2ch], 2
        cmp    dword ptr [edx + 1ch], 0
        jg     SHORT loc_1_07F4
        mov    dword ptr [edx + 14h], 102h
        jmp    SHORT loc_1_0807
loc_1_07D1:
        push   edx
        mov    edx, edi
        db  0BEh, 07Dh, 008h, 000h, 000h
        mov    ecx, eax
        sub    ecx, 7fffffffh
        jbe    SHORT loc_1_07EA
        mov    eax, ecx
        db  0BEh, 09Bh, 008h, 000h, 000h
loc_1_07EA:
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        pop    edx
        mov    dword ptr [edx + 10h], esi
loc_1_07F4:
        dec    dword ptr [edx + 1ch]
        jge    SHORT loc_1_0807
        mov    eax, dword ptr [edx + 18h]
        mov    ecx, 35h
        db  0CDh, 020h, 027h, 000h, 001h, 000h
loc_1_0807:
        and    byte ptr [edx + 2ch], 0feh
        mov    eax, dword ptr [edx + 14h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0813:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_0790
        add    esp, 4
        db  08Bh, 015h, 0E0h, 010h, 000h, 000h
        and    byte ptr [edx + 2ch], 0fdh
        mov    eax, dword ptr [edx + 14h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0836:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  08Bh, 015h, 0ECh, 010h, 000h, 000h
        mov    edx, dword ptr [edx + esi]
        and    dword ptr [edx + 2ch], 0fffffffeh
        xor    esi, esi
        xchg   dword ptr [edx + 10h], esi
        or     esi, esi
        je     SHORT loc_1_085B
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_1_085B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 14h], eax
        bts    dword ptr [edx + 2ch], 1
        jb     SHORT loc_1_0876
        inc    dword ptr [edx + 1ch]
        jg     SHORT loc_1_0876
        mov    eax, dword ptr [edx + 18h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_1_0876:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_087D:
        db  0A1h, 0ECh, 010h, 000h, 000h
        mov    eax, dword ptr [eax + edx]
        mov    dword ptr [eax + 10h], 0
        push   102h
        push   edx
        call   sub_1_0836
        add    esp, 8
        ret
loc_1_089B:
        db  08Bh, 03Dh, 0ECh, 010h, 000h, 000h
        mov    edi, dword ptr [edi + edx]
        db  0BEh, 07Dh, 008h, 000h, 000h
        mov    eax, 7fffffffh
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [edi + 10h], esi
        ret
sub_1_08B8:
        push   ebx
        push   esi
        push   edi
        push   1
        push   0ch
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_08F3
        mov    esi, eax
        sub    ecx, ecx
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jb     SHORT loc_1_08E7
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi], 78747665h
        mov    eax, esi
loc_1_08E3:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_08E7:
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_1_08F3:
        sub    eax, eax
        jmp    SHORT loc_1_08E3
sub_1_08F7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 78747665h
        jne    SHORT loc_1_091F
        mov    eax, dword ptr [eax + 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        push   0
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_1_091F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0924:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 78747665h
        jne    SHORT loc_1_0952
        dec    dword ptr [eax + 8]
        je     SHORT loc_1_0948
        mov    eax, dword ptr [eax + 4]
        mov    ecx, 35h
        db  0CDh, 020h, 027h, 000h, 001h, 000h
loc_1_0948:
        mov    eax, 1
loc_1_094D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0952:
        sub    eax, eax
        jmp    SHORT loc_1_094D
sub_1_0956:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 78747665h
        jne    SHORT loc_1_097E
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 8], 1
        je     SHORT loc_1_097E
        inc    dword ptr [eax + 8]
        jg     SHORT loc_1_097E
        mov    eax, dword ptr [eax + 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_1_097E:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0985:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    ebx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        mov    eax, edi
        mov    edi, dword ptr [ebp + 8]
        db  0CDh, 020h, 06Bh, 001h, 001h, 000h
        db  0CDh, 020h, 078h, 001h, 001h, 000h
        pop    ebx
        pop    edi
        leave
        ret
loc_1_09A8:
        db  08Bh, 015h, 0ECh, 010h, 000h, 000h
        mov    edx, dword ptr [edi + edx]
        or     edx, edx
        je     SHORT loc_1_0A2A
        mov    eax, dword ptr [edx]
        db  0A3h, 0D8h, 010h, 000h, 000h
        mov    eax, dword ptr [edx + 4]
        db  0A3h, 0DCh, 010h, 000h, 000h
        db  089h, 015h, 0E0h, 010h, 000h, 000h
        mov    ax, word ptr [edx + 72h]
        db  08Bh, 01Dh, 0E4h, 010h, 000h, 000h
        mov    word ptr [ebx], ax
        db  08Bh, 035h, 0F4h, 010h, 000h, 000h
        cmp    edx, esi
        je     SHORT loc_1_0A14
        push   ebx
        push   edx
        xor    ecx, ecx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        pop    edx
        mov    word ptr [esi + 76h], ax
        mov    ax, word ptr [edx + 76h]
        mov    ecx, 1
        push   edx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        pop    edx
        pop    ebx
        db  089h, 015h, 0F4h, 010h, 000h, 000h
loc_1_0A14:
        mov    eax, dword ptr [edx + 8]
        test   eax, eax
        je     SHORT loc_1_0A2A
        db  0A3h, 0E8h, 010h, 000h, 000h
        push   eax
        db  0CDh, 020h, 032h, 001h, 001h, 000h
        add    esp, 4
loc_1_0A2A:
        ret
VWIN32_GetContextHandle:
        cmp    dword ptr [edi + 2ch], 2ah
        jne    SHORT loc_1_0A41
        db  08Bh, 01Dh, 0ECh, 010h, 000h, 000h
        mov    ebx, dword ptr [edi + ebx]
        or     ebx, ebx
        je     SHORT loc_1_0A41
        mov    ebx, dword ptr [ebx + 8]
loc_1_0A41:
        ret
        db  000h, 000h
sub_1_0A44:
        call   sub_1_0A9F
        jb     SHORT loc_1_0A74
        xor    esi, esi
        xchg   dword ptr [eax + 0ch], esi
        or     esi, esi
        je     SHORT sub_1_0A7C
        cmp    esi, -1
        je     SHORT loc_1_0A61
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        jmp    SHORT sub_1_0A7C
loc_1_0A61:
        or     byte ptr [eax + 1ah], 4
        and    byte ptr [eax + 1ah], 0feh
        xchg   dword ptr [eax + 0ch], esi
        db  0C7h, 040h, 008h, 075h, 00Ah, 000h, 000h
        clc
loc_1_0A74:
        ret
loc_1_0A75:
        push   ebp
        mov    ebp, esp
        leave
        ret    4
sub_1_0A7C:
        call   sub_1_0A9F
        jb     SHORT loc_1_0A9E
        mov    edx, dword ptr [eax]
        mov    dword ptr [esi], edx
        and    byte ptr [eax + 1ah], 0feh
        test   byte ptr [eax + 1ah], 2
        jne    SHORT loc_1_0A9D
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_1_0A9D:
        clc
loc_1_0A9E:
        ret
sub_1_0A9F:
        mov    edx, edi
        db  003h, 015h, 0ECh, 010h, 000h, 000h
        mov    edx, dword ptr [edx]
        add    edx, 3ch
loc_1_0AAC:
        mov    ecx, dword ptr [edx]
        stc
        jecxz  loc_1_0ABA
        cmp    ecx, eax
        mov    esi, edx
        mov    edx, ecx
        jne    SHORT loc_1_0AAC
        clc
loc_1_0ABA:
        ret
sub_1_0ABB:
        push   esi
        db  08Bh, 035h, 0C8h, 011h, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_1_0AD7
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 0ch], 0
loc_1_0AD7:
        pop    esi
        ret
sub_1_0AD9:
        push   esi
        db  08Bh, 035h, 0C8h, 011h, 000h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        pop    esi
        ret
sub_1_0AE8:
        mov    esi, edx
        db  003h, 035h, 0ECh, 010h, 000h, 000h
        mov    esi, dword ptr [esi]
        or     esi, esi
        je     SHORT loc_1_0B1F
        mov    ecx, dword ptr [esi + 38h]
        jecxz  loc_1_0B07
loc_1_0AFB:
        mov    ebx, ecx
        mov    ecx, dword ptr [ecx]
        jecxz  loc_1_0B03
        jmp    SHORT loc_1_0AFB
loc_1_0B03:
        mov    dword ptr [ebx], eax
        jmp    SHORT loc_1_0B1F
loc_1_0B07:
        mov    dword ptr [esi + 38h], eax
        btr    dword ptr [esi + 2ch], 0
        jae    SHORT loc_1_0B1F
        push   0c0h
        push   edx
        call   sub_1_0836
        add    esp, 8
loc_1_0B1F:
        ret
_VWIN32_QueueUserApc:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 10h]
        cmp    edx, -1
        jne    SHORT loc_1_0B40
        db  08Bh, 015h, 010h, 011h, 000h, 000h
        or     edx, edx
        jne    SHORT loc_1_0B40
        db  0CDh, 020h, 00Ah, 001h, 001h, 000h
        mov    edx, edi
loc_1_0B40:
        call   sub_1_0ABB
        jae    SHORT loc_1_0B4B
        xor    eax, eax
        jmp    SHORT loc_1_0B67
loc_1_0B4B:
        mov    dword ptr [eax + 0ch], 0
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], ebx
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 4], ebx
        call   sub_1_0AE8
        mov    eax, esi
        or     eax, eax
loc_1_0B67:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0B6C:
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    ebx, edi
        db  003h, 01Dh, 0ECh, 010h, 000h, 000h
        mov    ebx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 38h]
        or     eax, eax
        je     SHORT loc_1_0B9F
loc_1_0B86:
        test   dword ptr [eax + 0ch], 1
        je     SHORT loc_1_0B9F
        mov    edx, dword ptr [eax]
        mov    dword ptr [ebx + 38h], edx
        call   sub_1_0AD9
        mov    eax, edx
        or     eax, eax
        jne    SHORT loc_1_0B86
loc_1_0B9F:
        pop    edi
        pop    esi
        pop    ebx
        ret
_VWIN32_QueueKernelAPC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 10h]
        cmp    ebx, -1
        jne    SHORT loc_1_0BC3
        db  08Bh, 01Dh, 010h, 011h, 000h, 000h
        or     ebx, ebx
        jne    SHORT loc_1_0BC3
        db  0CDh, 020h, 00Ah, 001h, 001h, 000h
        mov    ebx, edi
loc_1_0BC3:
        mov    edi, ebx
        db  003h, 03Dh, 0ECh, 010h, 000h, 000h
        mov    edi, dword ptr [edi]
        lea    edx, [edi + 48h]
        test   byte ptr [ebp + 14h], 2
        jne    SHORT loc_1_0BE9
        push   1
        push   1ch
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_0C4C
        mov    edx, eax
loc_1_0BE9:
        xor    eax, eax
        bts    word ptr [edx + 1ah], 0
        jb     SHORT loc_1_0C4C
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 4], ecx
        btr    word ptr [edx + 1ah], 2
        jb     SHORT loc_1_0C33
        mov    dword ptr [edx + 0ch], eax
        mov    eax, dword ptr [edi + 3ch]
        mov    dword ptr [edx], eax
        mov    dword ptr [edi + 3ch], edx
        test   byte ptr [ebp + 14h], 1
        jne    SHORT loc_1_0C1E
        cmp    dword ptr [edi + 28h], 0
        jne    SHORT loc_1_0C33
loc_1_0C1E:
        db  0BEh, 053h, 00Ch, 000h, 000h
        xor    eax, eax
        mov    ecx, 1c8h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    dword ptr [edx + 0ch], esi
loc_1_0C33:
        test   byte ptr [ebp + 14h], 4
        je     SHORT loc_1_0C4A
        push   edx
        push   dword ptr [ebp + 10h]
        push   0
        push   0
        call   _VWIN32_QueueUserApc
        add    esp, 0ch
        pop    edx
loc_1_0C4A:
        mov    eax, edx
loc_1_0C4C:
        or     eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0C53:
        or     eax, 0ffffffffh
        xchg   dword ptr [edx + 0ch], eax
        or     eax, eax
        je     SHORT loc_1_0C88
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [edx + 10h], eax
        mov    eax, dword ptr [ebp + 24h]
        mov    dword ptr [edx + 14h], eax
        mov    ax, word ptr [ebp + 28h]
        mov    word ptr [edx + 18h], ax
        mov    dword ptr [ebp + 1ch], edx
        db  066h, 08Bh, 00Dh, 0C6h, 011h, 000h, 000h
        db  00Fh, 0B7h, 015h, 0C4h, 011h, 000h, 000h
        db  0CDh, 020h, 049h, 000h, 001h, 000h
loc_1_0C88:
        ret
loc_1_0C89:
        mov    esi, dword ptr [ebp + 1ch]
        mov    edx, dword ptr [esi + 10h]
        mov    dword ptr [ebp + 1ch], edx
        mov    cx, word ptr [esi + 18h]
        mov    edx, dword ptr [esi + 14h]
        db  0CDh, 020h, 049h, 000h, 001h, 000h
        and    byte ptr [esi + 1ah], 0fbh
        push   dword ptr [esi + 4]
        mov    ebx, dword ptr [esi + 8]
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, esi
        call   sub_1_0A7C
        call   ebx
        ret
sub_1_0CB9:
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
loc_1_0CC2:
        mov    esi, edi
        db  003h, 035h, 0ECh, 010h, 000h, 000h
        mov    esi, dword ptr [esi]
        add    esi, 3ch
loc_1_0CCF:
        mov    ecx, dword ptr [esi]
        jecxz  loc_1_0CEC
        lea    esi, [ecx]
        cmp    dword ptr [ecx + 0ch], 0
        jne    SHORT loc_1_0CCF
        push   dword ptr [ecx + 4]
        mov    ebx, dword ptr [ecx + 8]
        mov    eax, ecx
        call   sub_1_0A7C
        call   ebx
        jmp    SHORT loc_1_0CC2
loc_1_0CEC:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0CF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_0D12
        db  0A1h, 0E0h, 010h, 000h, 000h
        cmp    dword ptr [eax + 38h], 0
        je     SHORT loc_1_0D0E
        mov    dword ptr [eax + 14h], 0c0h
        jmp    SHORT loc_1_0D27
loc_1_0D0E:
        or     byte ptr [eax + 2ch], 1
loc_1_0D12:
        db  0E8h, 02Fh, 02Dh, 0FFh, 0FFh
        push   dword ptr [ebp + 8]
        call   sub_1_0790
        add    esp, 4
        db  0E8h, 006h, 02Dh, 0FFh, 0FFh
loc_1_0D27:
        db  0A1h, 0D8h, 010h, 000h, 000h
        mov    eax, dword ptr [eax + 54h]
        or     eax, eax
        je     SHORT loc_1_0D38
        db  0E8h, 049h, 02Eh, 0FFh, 0FFh
loc_1_0D38:
        db  0A1h, 0E0h, 010h, 000h, 000h
        mov    ebx, dword ptr [eax + 14h]
        and    byte ptr [eax + 2ch], 0fdh
        db  0E8h, 0FDh, 02Ch, 0FFh, 0FFh
        mov    eax, ebx
        pop    ebx
        leave
        ret
_VWIN32_CloseVxDHandle:
        cmp    dword ptr [eax], 2
        je     SHORT loc_1_0D56
        xor    eax, eax
        ret
loc_1_0D56:
        dec    dword ptr [eax + 4]
        jne    SHORT loc_1_0D6F
        inc    dword ptr [eax + 4]
        push   -1
        push   eax
        db  0FFh, 035h, 000h, 011h, 000h, 000h
        call   _VWIN32_QueueUserApc
        add    esp, 0ch
loc_1_0D6F:
        ret
_VWIN32_CheckDelayedNpxTrap:
        push   ebp
        mov    ebp, esp
        sub    esp, 80h
        push   ebx
        push   esi
        push   edi
        xor    edi, edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    dword ptr [ebp - 10h], edi
        mov    byte ptr [ebp - 4], 0
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], 1
        mov    ebx, dword ptr [ecx + 24h]
loc_1_0D95:
        push   0
        lea    eax, [ebp - 1]
        push   1
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0DA3:
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_0E5E
        movzx  eax, byte ptr [ebp - 1]
        inc    ebx
        cmp    eax, 26h
        je     SHORT loc_1_0DD5
        cmp    eax, 2eh
        je     SHORT loc_1_0DD5
        cmp    eax, 36h
        je     SHORT loc_1_0DD5
        cmp    eax, 3eh
        je     SHORT loc_1_0DD5
        cmp    eax, 64h
        jl     SHORT loc_1_0DD1
        cmp    eax, 65h
        jle    SHORT loc_1_0DD5
loc_1_0DD1:
        mov    byte ptr [ebp - 2], 0
loc_1_0DD5:
        cmp    byte ptr [ebp - 2], 0
        jne    SHORT loc_1_0D95
        push   0
        lea    eax, [ebp - 3]
        push   1
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0DE9:
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_1_0E5E
        mov    cl, byte ptr [ebp - 3]
        inc    ebx
        mov    dword ptr [ebp - 0ch], 0
        and    cl, 38h
        shr    cl, 3
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0E5E
loc_1_0E0A:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dl, byte ptr [ebp - 1]
        shl    eax, 2
        db  038h, 090h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0E3F
        db  080h, 0B8h, 002h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0E34
        db  038h, 088h, 001h, 000h, 000h, 000h
        jne    SHORT loc_1_0E3F
        db  08Ah, 080h, 003h, 000h, 000h, 000h
        jmp    SHORT loc_1_0E5B
loc_1_0E34:
        mov    dl, byte ptr [ebp - 3]
        db  038h, 090h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_0E51
loc_1_0E3F:
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        db  080h, 03Ch, 085h, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0E0A
        jmp    SHORT loc_1_0E5E
loc_1_0E51:
        mov    eax, dword ptr [ebp - 0ch]
        db  08Ah, 004h, 085h, 003h, 000h, 000h, 000h
loc_1_0E5B:
        mov    byte ptr [ebp - 4], al
loc_1_0E5E:
        cmp    byte ptr [ebp - 4], 0
        jne    SHORT loc_1_0E6B
        xor    eax, eax
        jmp    NEAR PTR loc_1_0FAF
loc_1_0E6B:
        cmp    byte ptr [ebp - 4], 1
        jne    SHORT loc_1_0E7B
        mov    eax, 2
        jmp    NEAR PTR loc_1_0FAF
loc_1_0E7B:
        mov    edi, dword ptr [ebp - 10h]
        lea    esi, [ebp - 80h]
        db  0CDh, 020h, 005h, 000h, 011h, 000h
        cmp    byte ptr [ebp - 4], 4
        jne    SHORT loc_1_0E9F
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 7ch]
        mov    word ptr [ecx + 1ch], ax
        mov    dword ptr [ecx + 24h], ebx
        jmp    NEAR PTR loc_1_0FAA
loc_1_0E9F:
        cmp    byte ptr [ebp - 4], 2
        jne    SHORT loc_1_0EAA
        mov    eax, dword ptr [ebp - 80h]
        jmp    SHORT loc_1_0EAD
loc_1_0EAA:
        mov    eax, dword ptr [ebp - 7ch]
loc_1_0EAD:
        mov    word ptr [ebp - 8], ax
        mov    al, byte ptr [ebp - 3]
        shr    al, 4
        xor    esi, esi
        movzx  eax, al
        and    al, 0fch
        db  08Dh, 0B8h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp - 3]
        and    al, 7
        mov    byte ptr [ebp - 5], al
        cmp    byte ptr [edi], al
        je     SHORT loc_1_0F3F
        cmp    byte ptr [edi + 1], al
        jne    SHORT loc_1_0F2E
        push   esi
        lea    eax, [ebp - 1]
        push   1
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0EE2:
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_1_0FAA
        inc    ebx
        mov    al, byte ptr [ebp - 1]
        and    al, 38h
        shr    al, 3
        movzx  ecx, al
        mov    dword ptr [ebp - 0ch], ecx
        cmp    ecx, 4
        je     SHORT loc_1_0F16
        db  00Fh, 0B6h, 091h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx + ecx]
        mov    cl, byte ptr [ebp - 1]
        shr    cl, 6
        shl    esi, cl
loc_1_0F16:
        mov    al, byte ptr [ebp - 1]
        mov    ecx, dword ptr [ebp + 8]
        and    eax, 7
        db  00Fh, 0B6h, 090h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        add    esi, dword ptr [edx + ecx]
        jmp    SHORT loc_1_0F3F
loc_1_0F2E:
        movzx  eax, byte ptr [ebp - 5]
        mov    ecx, dword ptr [ebp + 8]
        db  00Fh, 0B6h, 090h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edx + ecx]
loc_1_0F3F:
        mov    edx, 1
        mov    cl, byte ptr [ebp - 5]
        shl    edx, cl
        movzx  eax, byte ptr [edi + 2]
        test   edx, eax
        je     SHORT loc_1_0F8F
        cmp    byte ptr [edi + 3], 4
        push   0
        jne    SHORT loc_1_0F71
        push   4
        lea    eax, [ebp - 0ch]
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0F65:
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_1_0FAA
        add    ebx, 4
        jmp    SHORT loc_1_0F8C
loc_1_0F71:
        push   1
        lea    eax, [ebp - 1]
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0F7D:
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_1_0FAA
        movsx  eax, byte ptr [ebp - 1]
        inc    ebx
        mov    dword ptr [ebp - 0ch], eax
loc_1_0F8C:
        add    esi, dword ptr [ebp - 0ch]
loc_1_0F8F:
        push   0
        lea    eax, [ebp - 8]
        push   2
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0F9D:
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_1_0FAA
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 24h], ebx
loc_1_0FAA:
        mov    eax, 1
loc_1_0FAF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0FB6:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, edx
        push   40h
        and    ecx, 0fffcffffh
        and    edx, 3ffffh
        shr    ecx, 10h
        shl    edx, 0ch
        mov    ecx, dword ptr [ecx + eax]
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        push   eax
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        push   eax
        mov    eax, dword ptr [ecx + 10h]
        sub    eax, edx
        cmp    eax, 1000h
        jb     SHORT loc_1_0FFB
        mov    eax, 1000h
loc_1_0FFB:
        push   eax
        push   dword ptr [esp + 18h]
        push   edx
        push   dword ptr [ecx + 24h]
        call   sub_1_036A
        add    esp, 1ch
        ret
        db  000h, 000h, 000h, 05Ch, 000h, 000h, 000h, 0DBh, 000h, 000h, 000h, 020h, 00Bh, 000h, 000h, 0C2h
        db  02Fh, 000h, 000h, 019h, 032h, 000h, 000h, 0A1h, 00Eh, 000h, 000h, 008h, 00Fh, 000h, 000h, 01Bh
        db  049h, 000h, 000h, 070h, 00Dh, 000h, 000h, 0ACh, 02Bh, 000h, 000h, 01Bh, 02Ch, 000h, 000h, 0B6h
        db  015h, 000h, 000h, 02Bh, 00Ah, 000h, 000h, 0D2h, 000h, 000h, 000h, 061h, 03Eh, 000h, 000h, 093h
        db  03Eh, 000h, 000h, 0CCh, 03Eh, 000h, 000h, 0EEh, 03Fh, 000h, 000h, 0F3h, 03Eh, 000h, 000h, 075h
        db  02Dh, 000h, 000h, 04Eh, 00Dh, 000h, 000h, 00Dh, 001h, 000h, 000h, 063h, 000h, 000h, 000h, 04Ch
        db  001h, 000h, 000h, 0D0h, 012h, 000h, 000h, 0A3h, 00Bh, 000h, 000h, 0ADh, 014h, 000h, 000h, 00Fh
        db  001h, 000h, 000h, 0AFh, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 02Ah, 000h, 001h, 003h, 000h, 000h, 056h, 057h, 049h, 04Eh
        db  033h, 032h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  07Fh, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  010h, 010h, 000h, 000h, 01Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1120:
        and    al, 11h
        add    byte ptr [eax], al
loc_1_1124:
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
loc_1_1150:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_118C:
        js     SHORT loc_1_1192
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1192:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        idiv   dword ptr [edi]
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_11CC:
        sbb    al, 18h
        adc    al, 10h
        xor    byte ptr [eax], cl
        add    al, 0
loc_1_11D4:
        sbb    al, 18h
        adc    al, 10h
        db  01Dh, 019h, 015h, 011h
loc_1_11DC:
        add    eax, 0ff042004h
        add    al, 0ffh
        add    edi, edi
        add    al, 0ffh
        add    al, 0ffh
        inc    dword ptr [eax]
        add    bl, bl
loc_1_11ED:
        jecxz  loc_1_11EF
loc_1_11EF:
        add    ebx, ebx
        loop   loc_1_11F3
loc_1_11F3:
        add    ecx, ebx
        push   es
        add    dword ptr [ecx], eax
        fld    qword ptr [esi]
        add    dword ptr [ecx], eax
        fld    dword ptr [edi]
        add    dword ptr [edx], eax
        fld    qword ptr [edi]
        add    dword ptr [ebx], eax
        fnstsw ax
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        db  001h
loc_1_120C:
        lodsd
        sti
        db  0FFh, 0FFh
_LTEXT ends
        end
