; atapchng_lcode_01.asm -- ATAPCHNG object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

ATAPCHNG_Control:
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
        db  00Fh, 084h, 0DEh, 0FFh, 0FEh, 0FFh
        cmp    si, 4
        db  00Fh, 084h, 00Dh, 002h, 0FFh, 0FFh
        cmp    si, 2
        db  00Fh, 084h, 025h, 002h, 0FFh, 0FFh
        mov    word ptr [ebx + 2], 0ffffh
        ret
        db  000h, 000h
loc_1_0040:
        push   ebp
        push   edi
        mov    ebx, dword ptr [esp + 0ch]
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
        je     SHORT loc_1_0081
        cmp    byte ptr [eax + 2], 2
        je     SHORT loc_1_0081
        cmp    byte ptr [eax + 2], 6
        je     SHORT loc_1_0081
loc_1_0077:
        call   sub_1_00C0
        jmp    NEAR PTR loc_1_010F
loc_1_0081:
        mov    dword ptr [edi + 10h], 0ffh
        jmp    SHORT loc_1_0077
loc_1_008A:
        push   ebp
        push   edi
        mov    ebx, dword ptr [esp + 0ch]
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
        je     SHORT loc_1_00B8
        pop    esi
        pop    edi
        pop    ebp
        ret
loc_1_00B8:
        mov    edi, dword ptr [eax + 8]
        jmp    NEAR PTR loc_1_010F
sub_1_00C0:
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
loc_1_00D8:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 4]
        mov    edx, dword ptr [ebx + 10h]
        mov    edi, dword ptr [edx + 8]
        test   byte ptr [ebx + 6dh], 80h
        je     NEAR PTR loc_1_04AF
        mov    ecx, dword ptr [ebx + 20h]
        and    byte ptr [ecx + 31h], 1fh
        test   byte ptr [ebx + 6dh], 8
        je     SHORT loc_1_010A
        cmp    byte ptr [ecx + 30h], 3
        je     NEAR PTR loc_1_04AF
loc_1_010A:
        call   sub_1_0510
loc_1_010F:
        cli
        bts    dword ptr [edi + 4], 0
        jb     NEAR PTR loc_1_01F5
        call   sub_1_04E8
        jb     NEAR PTR loc_1_01FA
        mov    edx, dword ptr [ebx + 10h]
        mov    edi, dword ptr [edx + 8]
        sti
        mov    esi, dword ptr [ebx + 8]
        movzx  eax, byte ptr [esi + 7dh]
        mov    ecx, dword ptr [ebx + 20h]
        cmp    byte ptr [ecx + 30h], 0
        je     NEAR PTR loc_1_02AA
        cmp    byte ptr [ecx + 30h], 12h
        je     SHORT loc_1_01A7
        cmp    dword ptr [edi + 10h], eax
        je     SHORT loc_1_01A7
        cmp    byte ptr [ecx + 30h], 42h
        je     NEAR PTR loc_1_0455
loc_1_0156:
        mov    edx, ebx
        db  0BEh, 068h, 001h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        jmp    NEAR PTR loc_1_01F5
loc_1_0168:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 4]
        mov    edx, dword ptr [ebx + 10h]
        mov    ecx, 1
        db  0E8h, 0C8h, 002h, 0FFh, 0FFh
        mov    dword ptr [edi + 30h], 0a6h
        mov    dword ptr [edi + 34h], 3
        movzx  eax, byte ptr [esi + 7dh]
        mov    dword ptr [edi + 38h], eax
        db  0C7h, 043h, 070h, 001h, 002h, 000h, 000h
        db  0BAh, 0D8h, 000h, 000h, 000h
        db  0E8h, 0C7h, 003h, 0FFh, 0FFh
        jmp    SHORT loc_1_01F5
loc_1_01A7:
        mov    eax, dword ptr [ebx + 14h]
        db  0C7h, 000h, 040h, 000h, 000h, 000h
        mov    dword ptr [eax + 4], edx
        mov    ecx, dword ptr [edx + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ebx + 4], ecx
        add    dword ptr [ebx + 14h], 8
        mov    ecx, dword ptr [ebx + 20h]
        cmp    byte ptr [ecx + 30h], 0
        je     NEAR PTR loc_1_02AA
        cmp    byte ptr [ecx + 30h], 55h
        jne    SHORT loc_1_01D7
        mov    byte ptr [ecx + 37h], 0
loc_1_01D7:
        cli
        mov    esi, dword ptr [ebx + 4]
        cmp    byte ptr [esi + 76h], 0
        jne    NEAR PTR loc_1_0477
loc_1_01E5:
        sti
        mov    edx, dword ptr [ebx + 10h]
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [ebx + 10h], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
loc_1_01F5:
        sti
        pop    esi
        pop    edi
        pop    ebp
        ret
loc_1_01FA:
        btr    dword ptr [edi + 4], 0
        jmp    SHORT loc_1_01F5
loc_1_0201:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
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
        call   sub_1_04CD
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebx + 20h]
        mov    edx, dword ptr [ebx + 10h]
        mov    edi, dword ptr [edx + 8]
        cmp    byte ptr [eax + 3], 1
        je     SHORT loc_1_0284
        mov    eax, dword ptr [eax + 1ch]
        cmp    byte ptr [eax + 0ch], 3ah
        jne    SHORT loc_1_0274
loc_1_0252:
        movzx  ecx, byte ptr [esi + 7dh]
        mov    byte ptr [ecx + edi + 18h], 3ah
        push   edi
        push   esi
        mov    edi, dword ptr [ebx + 20h]
        mov    byte ptr [edi + 3], 84h
        movzx  ecx, byte ptr [edi + 0bh]
        mov    edi, dword ptr [edi + 1ch]
        db  0BEh, 024h, 004h, 000h, 000h
        rep movsb
        pop    esi
        pop    edi
loc_1_0274:
        mov    esi, dword ptr [ebx + 8]
        mov    dword ptr [ebx + 4], esi
        call   sub_1_00C0
        jmp    NEAR PTR loc_1_010F
loc_1_0284:
        movzx  ecx, byte ptr [esi + 7dh]
        mov    byte ptr [ecx + edi + 18h], 0
        mov    dword ptr [edi + 10h], ecx
        mov    ecx, dword ptr [ebx + 20h]
        cmp    byte ptr [ecx + 30h], 0
        je     NEAR PTR loc_1_03F4
        movzx  eax, word ptr [ebx + 0eh]
        mov    word ptr [ebx + 0ch], ax
        jmp    NEAR PTR loc_1_01A7
loc_1_02AA:
        mov    edx, ebx
        db  0BEh, 0BCh, 002h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        jmp    NEAR PTR loc_1_01F5
loc_1_02BC:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 8]
        mov    edx, dword ptr [ebx + 10h]
        mov    ecx, 1
        db  0E8h, 074h, 001h, 0FFh, 0FFh
        mov    edx, dword ptr [ebx + 10h]
        mov    eax, dword ptr [edx + 8]
        mov    ecx, dword ptr [eax + 14h]
        mov    dword ptr [ebx + 80h], ecx
        mov    dword ptr [edi + 30h], 0bdh
        mov    dword ptr [edi + 34h], 0
        mov    ecx, dword ptr [eax + 0ch]
        lea    ecx, [ecx*4 + 8]
        db  0C7h, 043h, 070h, 02Fh, 003h, 000h, 000h
        mov    dword ptr [ebx + 7ch], ecx
        mov    dword ptr [edi + 10h], ecx
        ror    cx, 8
        mov    dword ptr [edi + 38h], ecx
        push   ebx
        db  0FFh, 015h, 040h, 005h, 000h, 000h
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_0320
        or     dword ptr [ebx + 6ch], 40h
loc_1_0320:
        db  0BAh, 0D8h, 000h, 000h, 000h
        db  0E8h, 042h, 002h, 0FFh, 0FFh
        jmp    NEAR PTR loc_1_01F5
loc_1_032F:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
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
        call   sub_1_04CD
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 8]
        mov    edi, dword ptr [ebx + 20h]
        mov    edx, dword ptr [ebx + 10h]
        mov    edx, dword ptr [edx + 8]
        cmp    byte ptr [edi + 3], 1
        mov    edi, edx
        jne    NEAR PTR loc_1_0436
        mov    ebp, dword ptr [edx + 14h]
        mov    ecx, dword ptr [edx + 0ch]
loc_1_0380:
        test   dword ptr [ebp + ecx*4 + 4], 1
        je     SHORT loc_1_0396
        mov    byte ptr [ecx + edx + 17h], 0ffh
        mov    dword ptr [edx + 10h], 0ffh
loc_1_0396:
        loop   loc_1_0380
        movzx  ecx, byte ptr [esi + 7dh]
        mov    ecx, dword ptr [ebp + ecx*4 + 8]
        test   byte ptr [edx + 4], 4
        je     SHORT loc_1_03C9
        test   ecx, 80h
        je     NEAR PTR loc_1_0252
        movzx  eax, byte ptr [esi + 7dh]
        cmp    byte ptr [eax + edx + 18h], 3ah
        jne    SHORT loc_1_03C9
        test   ecx, 1
        je     NEAR PTR loc_1_0156
loc_1_03C9:
        test   ecx, 1
        jne    NEAR PTR loc_1_0156
        movzx  ecx, byte ptr [esi + 7dh]
        cmp    byte ptr [ecx + edx + 18h], 0ffh
        je     NEAR PTR loc_1_0156
        cmp    byte ptr [ecx + edx + 18h], 3ah
        je     NEAR PTR loc_1_0252
        jmp    NEAR PTR loc_1_0274
loc_1_03F4:
        push   edi
        push   esi
        mov    edi, dword ptr [ebx + 20h]
        mov    byte ptr [edi + 3], 84h
        movzx  ecx, byte ptr [edi + 0bh]
        mov    edi, dword ptr [edi + 1ch]
        db  0BEh, 012h, 004h, 000h, 000h
        rep movsb
        pop    esi
        pop    edi
        jmp    NEAR PTR loc_1_0274
loc_1_0412:
        jo     SHORT loc_1_0414
loc_1_0414:
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_0424:
        jo     SHORT loc_1_0426
loc_1_0426:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0436:
        mov    eax, dword ptr [ebx + 20h]
        mov    eax, dword ptr [eax + 1ch]
        cmp    byte ptr [eax + 2], 6
        jne    NEAR PTR loc_1_0274
        cmp    byte ptr [eax + 0ch], 28h
        jne    NEAR PTR loc_1_0274
        jmp    NEAR PTR loc_1_02AA
loc_1_0455:
        push   esi
        push   edi
        push   ecx
        mov    edi, dword ptr [ebx + 80h]
        mov    ecx, 4
        db  08Dh, 035h, 09Fh, 004h, 000h, 000h
        rep movsd
        pop    ecx
        pop    edi
        pop    esi
        mov    byte ptr [ecx + 3], 1
        jmp    NEAR PTR loc_1_0274
loc_1_0477:
        test   byte ptr [ebx + 6dh], 8
        jne    NEAR PTR loc_1_01E5
        mov    esi, dword ptr [ebx + 8]
        mov    dword ptr [ebx + 4], esi
        mov    edi, dword ptr [ebx + 10h]
        mov    edi, dword ptr [edi + 8]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx + 64h], eax
        mov    dword ptr [edi], ebx
        btr    dword ptr [edi + 4], 0
        sti
        jmp    NEAR PTR loc_1_010F
        db  000h, 015h, 000h, 00Ch, 001h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h
loc_1_04AF:
        mov    eax, dword ptr [ebx + 14h]
        db  0C7h, 000h, 08Ah, 000h, 000h, 000h
        mov    dword ptr [eax + 4], edx
        mov    ecx, dword ptr [edx + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ebx + 4], ecx
        add    dword ptr [ebx + 14h], 8
        jmp    NEAR PTR loc_1_01D7
sub_1_04CD:
        sub    ebx, dword ptr [esi + 4]
        sub    esp, 8
        mov    eax, esp
        mov    dword ptr [eax + 4], ebx
        mov    word ptr [eax], 4
        push   eax
        db  0FFh, 015h, 02Ch, 005h, 000h, 000h
        add    esp, 0ch
        ret
sub_1_04E8:
        mov    ebx, dword ptr [edi]
        lea    eax, [edi - 64h]
        mov    esi, dword ptr [edi + 8]
loc_1_04F0:
        or     ebx, ebx
        stc
        je     SHORT loc_1_050F
        cmp    byte ptr [esi + 76h], 0
        je     SHORT loc_1_0508
        test   byte ptr [ebx + 6dh], 8
        jne    SHORT loc_1_0508
        mov    eax, ebx
        mov    ebx, dword ptr [ebx + 64h]
        jmp    SHORT loc_1_04F0
loc_1_0508:
        mov    ecx, dword ptr [ebx + 64h]
        mov    dword ptr [eax + 64h], ecx
        clc
loc_1_050F:
        ret
sub_1_0510:
        cli
        lea    eax, [edi - 64h]
loc_1_0514:
        cmp    dword ptr [eax + 64h], 0
        je     SHORT loc_1_051F
        mov    eax, dword ptr [eax + 64h]
        jmp    SHORT loc_1_0514
loc_1_051F:
        mov    dword ptr [eax + 64h], ebx
        mov    dword ptr [ebx + 64h], 0
        sti
        ret
        db  000h
sub_1_052C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0538:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0540:
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
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 004h, 000h, 000h, 000h, 041h, 054h, 041h, 050h
        db  043h, 048h, 04Eh, 047h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 024h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
_LTEXT ends
        end
