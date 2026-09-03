; torisan3_lcode_01.asm -- TORISAN3 object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

TORISAN3_Control:
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
        db  00Fh, 084h, 00Fh, 000h, 0FFh, 0FFh
        cmp    si, 4
        db  00Fh, 084h, 084h, 001h, 0FFh, 0FFh
        cmp    si, 2
        db  00Fh, 084h, 09Ch, 001h, 0FFh, 0FFh
        mov    word ptr [ebx + 2], 0ffffh
        ret
        db  000h, 000h
loc_1_0040:
        push   ebp
        mov    ebx, dword ptr [esp + 8]
        push   edi
        push   esi
        mov    esi, dword ptr [ebx + 8]
        mov    dword ptr [ebx + 4], esi
        mov    eax, dword ptr [ebx + 14h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebx + 10h], eax
        mov    edi, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebx + 20h]
        mov    eax, dword ptr [ecx + 1ch]
        cmp    byte ptr [ecx + 3], 1
        je     SHORT loc_1_0077
        test   byte ptr [ecx + 3], 80h
        je     SHORT loc_1_00C8
        cmp    byte ptr [eax + 2], 2
        je     SHORT loc_1_00C8
        cmp    byte ptr [eax + 2], 6
        je     SHORT loc_1_00C8
loc_1_0077:
        cmp    byte ptr [ecx + 30h], 0
        jne    SHORT loc_1_00AF
        movzx  edx, byte ptr [esi + 7dh]
        movzx  edx, byte ptr [edx + edi + 10h]
        or     edx, edx
        je     SHORT loc_1_00BF
        push   edi
        push   esi
        mov    edi, dword ptr [ebx + 20h]
        mov    byte ptr [edi + 3], 84h
        movzx  ecx, byte ptr [edi + 0bh]
        mov    edi, dword ptr [edi + 1ch]
        db  0BEh, 0D8h, 000h, 000h, 000h
        rep movsb
        pop    esi
        pop    edi
        mov    byte ptr [eax + 0ch], dl
        cmp    dl, 28h
        je     SHORT loc_1_00AF
        mov    byte ptr [eax + 2], 2
loc_1_00AF:
        call   sub_1_0120
        jmp    NEAR PTR loc_1_016F
        db  0C6h, 041h, 030h, 02Bh, 0EBh, 09Dh
loc_1_00BF:
        mov    eax, dword ptr [ebx + 20h]
        mov    byte ptr [eax + 3], 1
        jmp    SHORT loc_1_00AF
loc_1_00C8:
        mov    dword ptr [edi + 0ch], 0ffh
        mov    dword ptr [edi + 10h], 28282828h
        jmp    SHORT loc_1_00AF
loc_1_00D8:
        jo     SHORT loc_1_00DA
loc_1_00DA:
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_00EA:
        push   ebp
        mov    ebx, dword ptr [esp + 8]
        push   edi
        push   esi
        mov    esi, dword ptr [ebx + 8]
        mov    dword ptr [ebx + 4], esi
        mov    edi, dword ptr [ebx + 14h]
        push   dword ptr [edi + 4]
        sub    edi, 8
        mov    dword ptr [ebx + 14h], edi
        push   dword ptr [ebx + 68h]
        push   ebx
        call   dword ptr [edi]
        add    esp, 4
        pop    eax
        cmp    ax, 23h
        pop    eax
        je     SHORT loc_1_0118
        pop    esi
        pop    edi
        pop    ebp
        ret
loc_1_0118:
        mov    edi, dword ptr [eax + 8]
        jmp    NEAR PTR loc_1_016F
sub_1_0120:
        push   edi
        btr    dword ptr [edi + 4], 0
        mov    edi, dword ptr [ebx + 14h]
        sub    edi, 8
        mov    dword ptr [ebx + 14h], edi
        push   ebx
        call   dword ptr [edi]
        add    esp, 4
        pop    edi
        ret
        db  000h
loc_1_0138:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 4]
        mov    edx, dword ptr [ebx + 10h]
        mov    edi, dword ptr [edx + 8]
        test   byte ptr [ebx + 6dh], 80h
        je     NEAR PTR loc_1_03E1
        mov    ecx, dword ptr [ebx + 20h]
        and    byte ptr [ecx + 31h], 1fh
        test   byte ptr [ebx + 6dh], 8
        je     SHORT loc_1_016A
        cmp    byte ptr [ecx + 30h], 3
        je     NEAR PTR loc_1_03E1
loc_1_016A:
        call   sub_1_0430
loc_1_016F:
        cli
        bts    dword ptr [edi + 4], 0
        jb     NEAR PTR loc_1_0274
        call   sub_1_0408
        jb     NEAR PTR loc_1_0279
        mov    edx, dword ptr [ebx + 10h]
        mov    edi, dword ptr [edx + 8]
        sti
        mov    esi, dword ptr [ebx + 8]
        mov    ecx, dword ptr [ebx + 20h]
        cmp    byte ptr [ecx + 30h], 12h
        je     NEAR PTR loc_1_021D
        movzx  eax, byte ptr [esi + 7dh]
        cmp    dword ptr [edi + 0ch], eax
        je     SHORT loc_1_021D
        cmp    dword ptr [edi + 0ch], 0ffh
        je     SHORT loc_1_01C6
        cmp    byte ptr [ecx + 30h], 42h
        je     NEAR PTR loc_1_0387
        cmp    byte ptr [ecx + 30h], 0
        jne    SHORT loc_1_01C6
        cmp    byte ptr [eax + edi + 10h], 0
        je     SHORT loc_1_021D
loc_1_01C6:
        or     byte ptr [edi + 4], 2
        mov    edx, ebx
        db  0BEh, 0DCh, 001h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        jmp    NEAR PTR loc_1_0274
loc_1_01DC:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 4]
        mov    edx, dword ptr [ebx + 10h]
        db  0E8h, 0C6h, 001h, 0FFh, 0FFh
        mov    dword ptr [edi + 30h], 0
        mov    dword ptr [edi + 38h], 0
        movzx  eax, byte ptr [esi + 7dh]
        cmp    eax, 0ffh
        ror    eax, 8
        mov    dword ptr [edi + 34h], eax
        mov    byte ptr [edi + 0ah], 0ch
        db  0C7h, 043h, 070h, 080h, 002h, 000h, 000h
        db  0E8h, 0BDh, 002h, 0FFh, 0FFh
        jmp    SHORT loc_1_0274
loc_1_021D:
        mov    eax, dword ptr [ebx + 14h]
        db  0C7h, 000h, 040h, 000h, 000h, 000h
        mov    dword ptr [eax + 4], edx
        mov    ecx, dword ptr [edx + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ebx + 4], ecx
        add    dword ptr [ebx + 14h], 8
        mov    ecx, dword ptr [ebx + 20h]
        cmp    byte ptr [ecx + 30h], 0
        je     NEAR PTR loc_1_032D
        cmp    byte ptr [ecx + 30h], 55h
        jne    SHORT loc_1_024D
        mov    byte ptr [ecx + 37h], 0
loc_1_024D:
        cli
        mov    esi, dword ptr [ebx + 4]
        cmp    byte ptr [esi + 76h], 0
        jne    NEAR PTR loc_1_03A9
loc_1_025B:
        sti
        mov    edx, dword ptr [ebx + 10h]
        mov    eax, dword ptr [edx + 8]
        mov    eax, dword ptr [eax + 8]
        cmp    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [ebx + 10h], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
loc_1_0274:
        sti
        pop    esi
        pop    edi
        pop    ebp
        ret
loc_1_0279:
        btr    dword ptr [edi + 4], 0
        jmp    SHORT loc_1_0274
loc_1_0280:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 4]
        mov    edi, dword ptr [ebx + 20h]
        push   esi
        mov    esi, dword ptr [ebx + 20h]
        movzx  eax, byte ptr [esi + 3]
        mov    edx, dword ptr [ebx + 90h]
        mov    edi, dword ptr [edx + 20h]
        mov    byte ptr [edi + 3], al
        movzx  ecx, byte ptr [edi + 0bh]
        mov    esi, dword ptr [esi + 1ch]
        mov    edi, dword ptr [edi + 1ch]
        rep movsb
        pop    esi
        call   sub_1_044B
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx + 20h]
        mov    edx, dword ptr [ebx + 10h]
        mov    edi, dword ptr [edx + 8]
        cmp    byte ptr [eax + 3], 1
        je     SHORT loc_1_030B
        mov    eax, dword ptr [eax + 1ch]
        cmp    byte ptr [eax + 0ch], 3ah
        je     SHORT loc_1_02F2
        btr    dword ptr [edi + 4], 1
        jae    SHORT loc_1_02DE
        cmp    byte ptr [eax + 0ch], 28h
        je     SHORT loc_1_034A
loc_1_02DE:
        mov    edx, ebx
        db  0BEh, 0DCh, 001h, 000h, 000h
        mov    eax, 3e8h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        jmp    SHORT loc_1_0274
loc_1_02F2:
        movzx  ecx, byte ptr [esi + 7dh]
        mov    byte ptr [ecx + edi + 10h], 3ah
loc_1_02FB:
        mov    esi, dword ptr [ebx + 8]
        mov    dword ptr [ebx + 4], esi
        call   sub_1_0120
        jmp    NEAR PTR loc_1_016F
loc_1_030B:
        movzx  ecx, byte ptr [esi + 7dh]
        mov    byte ptr [ecx + edi + 10h], 0
        mov    dword ptr [edi + 0ch], ecx
        mov    ecx, dword ptr [ebx + 20h]
        cmp    byte ptr [ecx + 30h], 0
        je     SHORT loc_1_035A
        movzx  eax, word ptr [ebx + 0eh]
        mov    word ptr [ebx + 0ch], ax
        jmp    NEAR PTR loc_1_021D
loc_1_032D:
        mov    ecx, dword ptr [edi + 0ch]
        mov    eax, dword ptr [ebx + 20h]
        shl    ecx, 8
        mov    word ptr [eax + 36h], cx
        mov    dword ptr [eax + 38h], 0
        mov    byte ptr [eax + 0ah], 0ch
        jmp    NEAR PTR loc_1_024D
loc_1_034A:
        mov    dword ptr [edi + 0ch], 0ffh
        mov    dword ptr [edi + 10h], 28282828h
        jmp    SHORT loc_1_02FB
loc_1_035A:
        push   edi
        push   esi
        mov    edi, dword ptr [ebx + 20h]
        mov    byte ptr [edi + 3], 84h
        movzx  ecx, byte ptr [edi + 0bh]
        mov    edi, dword ptr [edi + 1ch]
        db  0BEh, 075h, 003h, 000h, 000h
        rep movsb
        pop    esi
        pop    edi
        jmp    SHORT loc_1_02FB
loc_1_0375:
        jo     SHORT loc_1_0377
loc_1_0377:
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_0387:
        push   esi
        push   edi
        push   ecx
        mov    edi, dword ptr [ebx + 80h]
        mov    ecx, 4
        db  08Dh, 035h, 0D1h, 003h, 000h, 000h
        rep movsd
        pop    ecx
        pop    edi
        pop    esi
        mov    byte ptr [ecx + 3], 1
        jmp    NEAR PTR loc_1_02FB
loc_1_03A9:
        test   byte ptr [ebx + 6dh], 8
        jne    NEAR PTR loc_1_025B
        mov    esi, dword ptr [ebx + 8]
        mov    dword ptr [ebx + 4], esi
        mov    edi, dword ptr [ebx + 10h]
        mov    edi, dword ptr [edi + 8]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx + 64h], eax
        mov    dword ptr [edi], ebx
        btr    dword ptr [edi + 4], 0
        sti
        jmp    NEAR PTR loc_1_016F
        db  000h, 015h, 000h, 00Ch, 001h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h
loc_1_03E1:
        mov    eax, dword ptr [ebx + 14h]
        db  0C7h, 000h, 0EAh, 000h, 000h, 000h
        mov    dword ptr [eax + 4], edx
        mov    ecx, dword ptr [edx + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ebx + 4], ecx
        add    dword ptr [ebx + 14h], 8
        jmp    NEAR PTR loc_1_024D
        db  0C6h, 041h, 030h, 0BEh, 0E9h, 045h, 0FEh, 0FFh, 0FFh
sub_1_0408:
        mov    ebx, dword ptr [edi]
        lea    eax, [edi - 64h]
        mov    esi, dword ptr [edi + 8]
loc_1_0410:
        or     ebx, ebx
        stc
        je     SHORT loc_1_042F
        cmp    byte ptr [esi + 76h], 0
        je     SHORT loc_1_0428
        test   byte ptr [ebx + 6dh], 8
        jne    SHORT loc_1_0428
        mov    eax, ebx
        mov    ebx, dword ptr [ebx + 64h]
        jmp    SHORT loc_1_0410
loc_1_0428:
        mov    ecx, dword ptr [ebx + 64h]
        mov    dword ptr [eax + 64h], ecx
        clc
loc_1_042F:
        ret
sub_1_0430:
        cli
        lea    eax, [edi - 64h]
loc_1_0434:
        cmp    dword ptr [eax + 64h], 0
        je     SHORT loc_1_043F
        mov    eax, dword ptr [eax + 64h]
        jmp    SHORT loc_1_0434
loc_1_043F:
        mov    dword ptr [eax + 64h], ebx
        mov    dword ptr [ebx + 64h], 0
        sti
        ret
sub_1_044B:
        sub    ebx, dword ptr [esi + 4]
        sub    esp, 8
        mov    eax, esp
        mov    dword ptr [eax + 4], ebx
        mov    word ptr [eax], 4
        push   eax
        db  0FFh, 015h, 068h, 004h, 000h, 000h
        add    esp, 0ch
        ret
        db  000h, 000h
sub_1_0468:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0474:
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
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 004h, 000h, 000h, 000h, 054h, 04Fh, 052h, 049h
        db  053h, 041h, 04Eh, 033h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 024h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
_LTEXT ends
        end
