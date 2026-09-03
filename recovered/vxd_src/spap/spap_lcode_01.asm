; spap_lcode_01.asm -- SPAP object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

SPAP_Control:
        cmp    eax, 0
        jne    SHORT loc_1_000F
        push   ebx
        db  0E8h, 035h, 000h, 0FFh, 0FFh
        cmp    eax, 1
        ret
loc_1_000F:
        cmp    eax, 1
        jne    SHORT loc_1_001E
        push   ebx
        db  0E8h, 036h, 000h, 0FFh, 0FFh
        cmp    eax, 1
        ret
loc_1_001E:
        cmp    eax, 2
        jne    SHORT loc_1_002D
        push   ebx
        db  0E8h, 037h, 000h, 0FFh, 0FFh
        cmp    eax, 1
        ret
loc_1_002D:
        cmp    eax, 1bh
        jne    SHORT loc_1_003B
        db  0E8h, 039h, 000h, 0FFh, 0FFh
        cmp    eax, 1
        ret
loc_1_003B:
        cmp    eax, 1ch
        jne    SHORT loc_1_0049
        db  0E8h, 03Bh, 000h, 0FFh, 0FFh
        cmp    eax, 1
        ret
loc_1_0049:
        clc
        ret
SPAP_V86_API:
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 1ch]
        call   sub_1_00A0
        mov    dword ptr [ebp + 1ch], eax
        ret
SPAP_PM_API:
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 1ch]
        call   sub_1_00E0
        mov    dword ptr [ebp + 1ch], eax
        ret
sub_1_006F:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0080:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        call   sub_1_006F
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_00A0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        je     SHORT loc_1_00B6
        cmp    eax, 2
        je     SHORT loc_1_00C3
        sub    eax, eax
        pop    ebp
        ret    0ch
loc_1_00B6:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0120
        pop    ebp
        ret    0ch
loc_1_00C3:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0130
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_00E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        je     SHORT loc_1_00F6
        cmp    eax, 2
        je     SHORT loc_1_0103
        sub    eax, eax
        pop    ebp
        ret    0ch
loc_1_00F6:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0140
        pop    ebp
        ret    0ch
loc_1_0103:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0150
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0120:
        mov    eax, 1
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0130:
        mov    eax, 2
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0140:
        mov    eax, 1
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0150:
        mov    eax, 2
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_0166:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_016C:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_018A:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_0190:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
        db  0CDh, 020h, 005h, 080h, 028h, 000h
        db  0CDh, 020h, 006h, 080h, 028h, 000h
        db  0CDh, 020h, 007h, 080h, 028h, 000h
        db  0CDh, 020h, 042h, 080h, 028h, 000h
        db  0CDh, 020h, 012h, 080h, 028h, 000h
        db  0CDh, 020h, 013h, 080h, 028h, 000h
        db  0CDh, 020h, 014h, 080h, 028h, 000h
        db  0CDh, 020h, 015h, 080h, 028h, 000h
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
        db  0CDh, 020h, 046h, 080h, 028h, 000h
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_0270:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        rep movsd
        mov    cl, al
        rep movsb
        pop    edi
        pop    esi
        ret    0ch
        db  056h, 057h, 09Ch, 08Bh, 04Ch, 024h, 018h, 08Bh, 074h, 024h, 014h, 08Bh, 07Ch, 024h, 010h, 03Bh
        db  0F7h, 073h, 00Dh, 00Bh, 0C9h, 074h, 01Fh, 0FDh, 049h, 003h, 0F1h, 003h, 0F9h, 041h, 0EBh, 014h
        db  08Bh, 0C6h, 02Bh, 0C7h, 083h, 0F8h, 004h, 072h, 00Bh, 08Ah, 0C1h, 024h, 003h, 0C1h, 0E9h, 002h
        db  0F3h, 0A5h, 08Ah, 0C8h, 0F3h, 0A4h, 09Dh, 05Fh, 05Eh, 0C2h, 00Ch, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 053h, 050h, 041h, 050h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 04Bh, 000h, 000h, 000h
        db  05Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h
sub_1_0320:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        sub    ax, ax
        cmp    byte ptr [ecx], al
        je     SHORT loc_1_0335
loc_1_032D:
        inc    ecx
        inc    ax
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_1_032D
loc_1_0335:
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0340:
        push   ebp
        mov    ebp, esp
        mov    ax, word ptr [ebp + 8]
        xchg   ah, al
        mov    word ptr [ebp + 8], ax
        mov    ax, word ptr [ebp + 8]
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx]
        mov    dl, al
        and    edx, 0ffff00ffh
        or     al, al
        je     SHORT loc_1_0398
        not    al
        add    al, 0ah
        mov    byte ptr [ecx], al
        inc    ecx
        mov    ax, dx
        dec    dx
        or     ax, ax
        je     SHORT loc_1_0398
loc_1_0385:
        mov    al, byte ptr [ecx]
        not    al
        add    al, 6
        mov    byte ptr [ecx], al
        inc    ecx
        mov    ax, dx
        dec    dx
        or     ax, ax
        jne    SHORT loc_1_0385
loc_1_0398:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        sub    edx, edx
        mov    ecx, dword ptr [ebp + 10h]
        cmp    ecx, edx
        jle    SHORT loc_1_03BD
        mov    eax, dword ptr [ebp + 8]
        mov    bl, byte ptr [ebp + 0ch]
loc_1_03B3:
        cmp    byte ptr [eax], bl
        jne    SHORT loc_1_03C7
        inc    edx
        inc    eax
        cmp    edx, ecx
        jl     SHORT loc_1_03B3
loc_1_03BD:
        mov    eax, 1
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_03C7:
        sub    eax, eax
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    bx, word ptr [ebp + 0ch]
        mov    eax, ebx
        and    eax, 0ffffh
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [eax + esi]
        mov    ax, 40h
        sub    ax, bx
        push   eax
        push   edi
        call   sub_1_06E8
        add    esp, 8
        mov    byte ptr [edi], 80h
        shl    bx, 3
        mov    byte ptr [esi + 38h], bl
        mov    byte ptr [esi + 39h], bh
        mov    dword ptr [ebp - 10h], 67452301h
        mov    dword ptr [ebp - 0ch], 0efcdab89h
        mov    edx, esi
        mov    dword ptr [ebp - 8], 98badcfeh
        mov    dword ptr [ebp - 4], 10325476h
        mov    ecx, 4
loc_1_042B:
        mov    edi, dword ptr [ebp - 0ch]
        not    edi
        and    edi, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, dword ptr [ebp - 8]
        or     edi, eax
        add    edi, dword ptr [edx]
        add    edi, dword ptr [ebp - 10h]
        add    edx, 4
        lea    eax, [edi*8]
        shr    edi, 1dh
        or     eax, edi
        mov    dword ptr [ebp - 10h], eax
        mov    edi, eax
        not    edi
        and    edi, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, dword ptr [ebp - 10h]
        or     edi, eax
        add    edi, dword ptr [edx]
        add    edi, dword ptr [ebp - 4]
        add    edx, 4
        mov    eax, edi
        shl    eax, 7
        shr    edi, 19h
        or     eax, edi
        mov    dword ptr [ebp - 4], eax
        mov    edi, eax
        not    edi
        and    edi, dword ptr [ebp - 0ch]
        and    eax, dword ptr [ebp - 10h]
        or     edi, eax
        add    edi, dword ptr [edx]
        add    edi, dword ptr [ebp - 8]
        add    edx, 4
        mov    eax, edi
        shl    eax, 0bh
        shr    edi, 15h
        or     eax, edi
        mov    dword ptr [ebp - 8], eax
        mov    edi, eax
        not    edi
        and    edi, dword ptr [ebp - 10h]
        and    eax, dword ptr [ebp - 4]
        or     edi, eax
        add    edi, dword ptr [edx]
        add    edi, dword ptr [ebp - 0ch]
        add    edx, 4
        mov    eax, edi
        shl    eax, 13h
        shr    edi, 0dh
        or     eax, edi
        mov    dword ptr [ebp - 0ch], eax
        dec    ecx
        jne    NEAR PTR loc_1_042B
        db  0BAh, 034h, 01Bh, 000h, 000h
        mov    ecx, 4
loc_1_04C9:
        mov    edi, dword ptr [ebp - 8]
        or     edi, dword ptr [ebp - 4]
        and    edi, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 8]
        and    eax, dword ptr [ebp - 4]
        or     edi, eax
        xor    eax, eax
        mov    al, byte ptr [edx]
        add    edi, dword ptr [esi + eax]
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [edi + eax + 5a827999h]
        inc    edx
        lea    edi, [eax*8]
        shr    eax, 1dh
        or     edi, eax
        mov    dword ptr [ebp - 10h], edi
        mov    edi, dword ptr [ebp - 8]
        or     edi, dword ptr [ebp - 10h]
        and    edi, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 8]
        and    eax, dword ptr [ebp - 10h]
        or     edi, eax
        xor    eax, eax
        mov    al, byte ptr [edx]
        add    edi, dword ptr [esi + eax]
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [edi + eax + 5a827999h]
        inc    edx
        mov    edi, eax
        shr    edi, 1bh
        shl    eax, 5
        or     edi, eax
        mov    dword ptr [ebp - 4], edi
        or     edi, dword ptr [ebp - 10h]
        and    edi, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 4]
        and    eax, dword ptr [ebp - 10h]
        or     edi, eax
        xor    eax, eax
        mov    al, byte ptr [edx]
        add    edi, dword ptr [esi + eax]
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [edi + eax + 5a827999h]
        inc    edx
        mov    edi, eax
        shr    edi, 17h
        shl    eax, 9
        or     edi, eax
        mov    dword ptr [ebp - 8], edi
        or     edi, dword ptr [ebp - 10h]
        and    edi, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        and    eax, dword ptr [ebp - 10h]
        or     edi, eax
        xor    eax, eax
        mov    al, byte ptr [edx]
        add    edi, dword ptr [esi + eax]
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [edi + eax + 5a827999h]
        inc    edx
        mov    edi, eax
        shr    edi, 13h
        shl    eax, 0dh
        or     edi, eax
        mov    dword ptr [ebp - 0ch], edi
        dec    ecx
        jne    NEAR PTR loc_1_04C9
        db  0BAh, 044h, 01Bh, 000h, 000h
        mov    ecx, 4
loc_1_0596:
        mov    edi, dword ptr [ebp - 0ch]
        xor    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    al, byte ptr [edx]
        mov    ebx, dword ptr [esi + eax]
        mov    eax, dword ptr [ebp - 4]
        xor    eax, edi
        add    ebx, eax
        mov    eax, dword ptr [ebp - 10h]
        lea    eax, [ebx + eax + 6ed9eba1h]
        inc    edx
        lea    ebx, [eax*8]
        shr    eax, 1dh
        or     ebx, eax
        mov    dword ptr [ebp - 10h], ebx
        xor    eax, eax
        mov    al, byte ptr [edx]
        mov    ebx, dword ptr [esi + eax]
        mov    eax, dword ptr [ebp - 10h]
        xor    eax, edi
        add    ebx, eax
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [ebx + eax + 6ed9eba1h]
        inc    edx
        mov    edi, eax
        shr    edi, 17h
        shl    eax, 9
        or     edi, eax
        mov    dword ptr [ebp - 4], edi
        xor    eax, eax
        mov    al, byte ptr [edx]
        mov    edi, dword ptr [esi + eax]
        mov    eax, dword ptr [ebp - 0ch]
        xor    eax, dword ptr [ebp - 4]
        inc    edx
        xor    eax, dword ptr [ebp - 10h]
        add    edi, eax
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [edi + eax + 6ed9eba1h]
        mov    edi, eax
        shl    edi, 0bh
        shr    eax, 15h
        or     edi, eax
        mov    dword ptr [ebp - 8], edi
        xor    eax, eax
        mov    al, byte ptr [edx]
        mov    edi, dword ptr [esi + eax]
        mov    eax, dword ptr [ebp - 8]
        xor    eax, dword ptr [ebp - 4]
        inc    edx
        xor    eax, dword ptr [ebp - 10h]
        add    edi, eax
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [edi + eax + 6ed9eba1h]
        mov    edi, eax
        shl    edi, 0fh
        shr    eax, 11h
        or     edi, eax
        mov    dword ptr [ebp - 0ch], edi
        dec    ecx
        jne    NEAR PTR loc_1_0596
        add    dword ptr [ebp - 10h], 67452301h
        sub    dword ptr [ebp - 0ch], 10325477h
        sub    dword ptr [ebp - 8], 67452302h
        add    dword ptr [ebp - 4], 10325476h
        lea    edi, [ebp - 10h]
loc_1_0664:
        sub    ecx, ecx
        mov    eax, dword ptr [edi]
loc_1_0668:
        mov    edx, eax
        shr    edx, cl
        mov    byte ptr [esi], dl
        inc    esi
        add    ecx, 8
        cmp    ecx, 20h
        jl     SHORT loc_1_0668
        add    edi, 4
        lea    eax, [ebp]
        cmp    edi, eax
        jb     SHORT loc_1_0664
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0690:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_06A8
loc_1_06A4:
        inc    word ptr [ebp - 4]
loc_1_06A8:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_06E3
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx], al
        jmp    NEAR PTR loc_1_06A4
loc_1_06E3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_06E8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_0700
loc_1_06FC:
        inc    word ptr [ebp - 4]
loc_1_0700:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_072D
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx], 0
        jmp    NEAR PTR loc_1_06FC
loc_1_072D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0732:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ecx, ecx
        cmp    dword ptr [ebp + 14h], ecx
        jle    SHORT loc_1_0781
        mov    esi, dword ptr [ebp + 18h]
        and    esi, 0ffffh
loc_1_0748:
        mov    eax, ecx
        cdq
        idiv   dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    bl, byte ptr [edx + eax]
        lea    eax, [esi + ecx]
        mov    edi, 1fh
        cdq
        idiv   edi
        mov    edi, 11h
        db  032h, 09Ah, 054h, 01Bh, 000h, 000h
        mov    eax, ecx
        cdq
        idiv   edi
        mov    eax, dword ptr [ebp + 10h]
        db  032h, 09Ah, 074h, 01Bh, 000h, 000h
        mov    byte ptr [eax + ecx], bl
        inc    ecx
        cmp    ecx, dword ptr [ebp + 14h]
        jl     SHORT loc_1_0748
loc_1_0781:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0790:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        push   13h
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_06E8
        add    esp, 8
        push   40h
        lea    eax, [ebp - 40h]
        push   eax
        call   sub_1_06E8
        add    esp, 8
        call   sub_1_0080
        mov    di, ax
        push   edi
        push   11h
        lea    eax, [ebp - 40h]
        push   eax
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_1_0320
        movsx  eax, ax
        inc    eax
        push   eax
        push   ebx
        call   sub_1_0732
        push   edi
        call   sub_1_0340
        mov    word ptr [esi], ax
        push   11h
        lea    eax, [ebp - 40h]
        push   eax
        add    esi, 2
        push   esi
        call   sub_1_0690
        add    esp, 0ch
        mov    eax, 13h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0810:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi]
        push   eax
        call   sub_1_0340
        push   eax
        push   11h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        add    esi, 2
        push   11h
        push   esi
        call   sub_1_0732
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0840:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0ch], 0
        mov    dword ptr [eax], 4ch
        mov    word ptr [eax + 4], 1
        mov    word ptr [eax + 6], 27c0h
        jne    SHORT loc_1_0864
        mov    word ptr [eax + 6], 23c1h
loc_1_0864:
        mov    word ptr [eax + 8], 0
        mov    dword ptr [eax + 0ah], 38h
        mov    byte ptr [eax + 0eh], 1
        mov    byte ptr [eax + 0fh], 6
        mov    dword ptr [eax + 10h], 0
        mov    dword ptr [eax + 14h], 0
        db  0C7h, 040h, 018h, 080h, 00Ch, 000h, 000h
        db  0C7h, 040h, 01Ch, 0A0h, 00Ch, 000h, 000h
        db  0C7h, 040h, 020h, 030h, 00Dh, 000h, 000h
        db  0C7h, 040h, 024h, 040h, 00Dh, 000h, 000h
        db  0C7h, 040h, 028h, 050h, 00Dh, 000h, 000h
        db  0C7h, 040h, 02Ch, 080h, 00Dh, 000h, 000h
        db  0C7h, 040h, 030h, 090h, 00Dh, 000h, 000h
        db  0C7h, 040h, 034h, 050h, 00Eh, 000h, 000h
        db  0C7h, 040h, 038h, 0C0h, 00Fh, 000h, 000h
        mov    dword ptr [eax + 3ch], 0
        db  0C7h, 040h, 040h, 050h, 009h, 000h, 000h
        db  0C7h, 040h, 044h, 060h, 009h, 000h, 000h
        db  0C7h, 040h, 048h, 090h, 009h, 000h, 000h
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_08F0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4bh
        mov    word ptr [eax], 1
        mov    dword ptr [eax + 2], 1
        mov    word ptr [eax + 6], 1
        mov    word ptr [eax + 8], 1
        mov    dword ptr [eax + 0ah], 0
        mov    dword ptr [eax + 0eh], 0
        mov    word ptr [eax + 12h], 0
        mov    word ptr [eax + 14h], 0
        mov    word ptr [eax + 1ah], 0
        mov    dword ptr [eax + 1ch], 0
        mov    word ptr [eax + 20h], 0
        mov    word ptr [eax + 22h], 0
        mov    dword ptr [eax + 24h], 5
        pop    ebp
        ret    4
        db  090h
loc_1_0950:
        mov    eax, 4
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0960:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax], 1
        add    dword ptr [ebp + 0ch], 2
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax], 100h
        mov    eax, 4
        add    dword ptr [ebp + 0ch], 2
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0990:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 4bh
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        cmp    eax, 1
        je     SHORT loc_1_09B1
        cmp    eax, 2
        je     SHORT loc_1_09D6
        cmp    eax, 3
        je     SHORT loc_1_09DD
        sub    eax, eax
        pop    ebp
        ret
loc_1_09B1:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        sub    eax, 2
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 2
        push   eax
        push   ecx
        call   sub_1_0BD0
        or     eax, eax
        je     SHORT loc_1_09D2
        mov    eax, 1
        pop    ebp
        ret
loc_1_09D2:
        sub    eax, eax
        pop    ebp
        ret
loc_1_09D6:
        mov    eax, 1
        pop    ebp
        ret
loc_1_09DD:
        sub    eax, eax
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_09F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        lea    edx, [eax + 4bh]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    ebx, dword ptr [eax + 0feh]
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax], 0
        mov    ecx, dword ptr [ebx + 20h]
        or     ecx, ecx
        jne    SHORT loc_1_0A20
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_0A20:
        mov    eax, dword ptr [ebp + 0ch]
        add    cl, 2
        mov    byte ptr [eax], cl
        mov    esi, eax
        inc    esi
        sub    cx, cx
        cmp    dword ptr [ebx + 20h], 0
        jbe    SHORT loc_1_0A53
loc_1_0A34:
        mov    edi, ecx
        and    edi, 0ffffh
        mov    eax, dword ptr [ebx + 30h]
        mov    al, byte ptr [edi + eax]
        mov    byte ptr [esi], al
        inc    cx
        inc    esi
        mov    eax, ecx
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebx + 20h]
        jb     SHORT loc_1_0A34
loc_1_0A53:
        cmp    dword ptr [ebx + 28h], 0
        je     SHORT loc_1_0AA3
        db  0BFh, 034h, 01Dh, 000h, 000h
        sub    eax, eax
        mov    ecx, 10h
        rep stosd
        mov    eax, dword ptr [edx + 2]
        push   eax
        db  068h, 034h, 01Dh, 000h, 000h
        mov    eax, dword ptr [ebx + 38h]
        inc    esi
        push   eax
        call   sub_1_0790
        lea    ecx, [eax + 2]
        mov    byte ptr [esi - 1], cl
        sub    cx, cx
        cmp    ax, cx
        je     SHORT loc_1_0AA7
loc_1_0A88:
        mov    edx, ecx
        and    edx, 0ffffh
        inc    esi
        db  08Ah, 092h, 034h, 01Dh, 000h, 000h
        mov    byte ptr [esi - 1], dl
        inc    cx
        cmp    ax, cx
        ja     SHORT loc_1_0A88
        jmp    SHORT loc_1_0AA7
loc_1_0AA3:
        mov    byte ptr [esi], 2
        inc    esi
loc_1_0AA7:
        sub    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 10h]
        inc    si
        mov    word ptr [eax], si
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_1_0AC0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        lea    esi, [eax + 4bh]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    edx, dword ptr [eax + 0feh]
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax - 4], 6
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax], 0
        cmp    dword ptr [edx + 20h], 0
        jne    SHORT loc_1_0AF6
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_0AF6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, eax
        mov    byte ptr [eax], 1
        inc    ebx
        mov    eax, dword ptr [edx + 20h]
        add    al, 2
        mov    byte ptr [ebx], al
        inc    ebx
        sub    cx, cx
        cmp    dword ptr [edx + 20h], 0
        jbe    SHORT loc_1_0B2E
loc_1_0B10:
        mov    eax, ecx
        and    eax, 0ffffh
        mov    edi, dword ptr [edx + 30h]
        mov    al, byte ptr [eax + edi]
        mov    byte ptr [ebx], al
        inc    cx
        inc    ebx
        mov    eax, ecx
        and    eax, 0ffffh
        cmp    eax, dword ptr [edx + 20h]
        jb     SHORT loc_1_0B10
loc_1_0B2E:
        mov    byte ptr [ebx], 2
        inc    ebx
        cmp    dword ptr [edx + 28h], 0
        je     SHORT loc_1_0B82
        db  0BFh, 034h, 01Dh, 000h, 000h
        sub    eax, eax
        mov    ecx, 10h
        rep stosd
        mov    eax, dword ptr [esi + 2]
        push   eax
        db  068h, 034h, 01Dh, 000h, 000h
        mov    eax, dword ptr [edx + 38h]
        inc    ebx
        push   eax
        call   sub_1_0790
        lea    ecx, [eax + 2]
        mov    byte ptr [ebx - 1], cl
        sub    dx, dx
        cmp    ax, dx
        je     SHORT loc_1_0B86
loc_1_0B67:
        mov    ecx, edx
        and    ecx, 0ffffh
        inc    ebx
        db  08Ah, 089h, 034h, 01Dh, 000h, 000h
        mov    byte ptr [ebx - 1], cl
        inc    dx
        cmp    ax, dx
        ja     SHORT loc_1_0B67
        jmp    SHORT loc_1_0B86
loc_1_0B82:
        mov    byte ptr [ebx], 2
        inc    ebx
loc_1_0B86:
        mov    byte ptr [ebx], 3
        inc    ebx
        mov    byte ptr [ebx], 4
        inc    ebx
        mov    ax, word ptr [esi + 6]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx], cx
        add    ebx, 2
        mov    byte ptr [ebx], 4
        inc    ebx
        mov    byte ptr [ebx], 4
        inc    ebx
        mov    ax, word ptr [esi + 8]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx], cx
        add    ebx, 2
        sub    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 10h]
        inc    bx
        mov    word ptr [eax], bx
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0BD0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_1_0BF0
        cmp    eax, 2
        je     SHORT loc_1_0C09
        cmp    eax, 4
        je     SHORT loc_1_0C40
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
loc_1_0BF0:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 1
        mov    dword ptr [esi + 2], 0
        mov    word ptr [esi], 0
        pop    esi
        pop    ebp
        ret    0ch
loc_1_0C09:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        mov    esi, dword ptr [ebp + 8]
        or     ecx, eax
        mov    dword ptr [esi + 2], ecx
        mov    word ptr [esi], 0
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
loc_1_0C40:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx]
        mov    dx, ax
        and    edx, 0ffh
        shl    edx, 8
        shr    ax, 8
        and    eax, 0ffffh
        mov    esi, dword ptr [ebp + 8]
        or     edx, eax
        mov    dword ptr [esi + 2], edx
        mov    ax, word ptr [ecx + 4]
        add    ecx, 4
        mov    ch, al
        mov    cl, ah
        mov    word ptr [esi], cx
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
loc_1_0C80:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 094h, 01Bh, 000h, 000h
        push   eax
        call   sub_1_08F0
        mov    eax, 800h
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0CA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        lea    esi, [ecx + 4bh]
        mov    eax, dword ptr [ecx + 4]
        mov    edi, dword ptr [eax + 0ch]
        db  089h, 00Dh, 094h, 01Bh, 000h, 000h
        cmp    dword ptr [edi + 0dch], ecx
        jne    SHORT loc_1_0D00
        mov    dword ptr [edi + 0d4h], 0
        mov    dword ptr [edi + 0dch], 0
        mov    dword ptr [edi + 0e0h], 0
        mov    eax, dword ptr [edi + 0d8h]
        or     eax, eax
        je     SHORT loc_1_0CF6
        push   0
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        push   eax
        call   sub_1_0190
loc_1_0CF6:
        mov    dword ptr [edi + 0d8h], 0
loc_1_0D00:
        cmp    dword ptr [esi + 34h], 0
        je     SHORT loc_1_0D1A
        mov    dword ptr [esi + 34h], 0
        lea    eax, [ebp - 1]
        push   eax
        add    esi, 28h
        push   esi
        call   sub_1_016C
loc_1_0D1A:
        mov    eax, 800h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D30:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 094h, 01Bh, 000h, 000h
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_0D40:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 094h, 01Bh, 000h, 000h
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_0D50:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        db  0A3h, 094h, 01Bh, 000h, 000h
        mov    dword ptr [eax + 73h], 0
        db  0C7h, 040h, 077h, 070h, 019h, 000h, 000h
        mov    dword ptr [eax + 7bh], eax
        lea    ecx, [eax + 4bh]
        mov    word ptr [eax + 23h], 2710h
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D80:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 094h, 01Bh, 000h, 000h
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_0D90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 4]
        lea    esi, [ecx + 4bh]
        mov    eax, dword ptr [edx + 0ch]
        cmp    dword ptr [edx + 10h], 0
        mov    edi, dword ptr [eax + 0feh]
        je     NEAR PTR loc_1_0E38
        xor    eax, eax
        mov    ax, word ptr [esi]
        or     eax, eax
        mov    eax, dword ptr [edi + 20h]
        je     SHORT loc_1_0E04
        mov    edx, dword ptr [ebp + 0ch]
        add    ax, 2
        mov    word ptr [edx], ax
        cmp    dword ptr [edi + 28h], 0
        jbe    SHORT loc_1_0DD3
        add    ax, 15h
        jmp    SHORT loc_1_0DD7
loc_1_0DD3:
        add    ax, 2
loc_1_0DD7:
        mov    word ptr [edx], ax
        mov    di, word ptr [edx]
        add    di, 8
        mov    word ptr [edx], di
        cmp    dword ptr [esi + 0ah], 0
        je     SHORT loc_1_0E2C
        mov    ax, word ptr [esi + 12h]
        add    ax, di
        add    ax, 2
        mov    word ptr [edx], ax
        mov    al, 1
        db  089h, 00Dh, 094h, 01Bh, 000h, 000h
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_0E04:
        mov    edx, dword ptr [ebp + 0ch]
        inc    ax
        mov    word ptr [edx], ax
        cmp    dword ptr [edi + 28h], 0
        jbe    SHORT loc_1_0E18
        add    ax, 14h
        jmp    SHORT loc_1_0E1A
loc_1_0E18:
        inc    ax
loc_1_0E1A:
        mov    word ptr [edx], ax
        cmp    dword ptr [esi + 0ah], 0
        je     SHORT loc_1_0E2C
        mov    ax, word ptr [esi + 12h]
        inc    ax
        add    word ptr [edx], ax
loc_1_0E2C:
        mov    al, 1
        db  089h, 00Dh, 094h, 01Bh, 000h, 000h
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_0E38:
        mov    edx, dword ptr [ebp + 0ch]
        sub    al, al
        mov    word ptr [edx], 0
        db  089h, 00Dh, 094h, 01Bh, 000h, 000h
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_0E50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 4]
        lea    edx, [ecx + 4bh]
        db  089h, 00Dh, 094h, 01Bh, 000h, 000h
        cmp    dword ptr [eax + 10h], 0
        je     SHORT loc_1_0E90
        xor    eax, eax
        mov    ax, word ptr [edx]
        or     eax, eax
        lea    eax, [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   ecx
        je     SHORT loc_1_0E86
        call   sub_1_0AC0
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0E86:
        call   sub_1_09F0
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
loc_1_0E90:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 0
        mov    byte ptr [eax + 1], 0
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_1_0EA0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        inc    word ptr [edx + 5fh]
        add    edx, 4bh
        cmp    word ptr [ebp + 10h], 4
        jbe    SHORT loc_1_0EE0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        mov    dword ptr [edx + 16h], ecx
        mov    eax, 1
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_0EE0:
        mov    dword ptr [edx + 16h], 0
        mov    eax, 1
        pop    ebp
        ret    0ch
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        inc    word ptr [edx + 65h]
        add    edx, 4bh
        cmp    word ptr [ebp + 10h], 4
        lea    esi, [edx + 1ch]
        jbe    SHORT loc_1_0F30
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        mov    dword ptr [esi], ecx
        jmp    SHORT loc_1_0F36
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0F30:
        mov    dword ptr [esi], 0
loc_1_0F36:
        cmp    dword ptr [esi], 3
        jne    SHORT loc_1_0F4C
        mov    dword ptr [edx + 0eh], 0
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    0ch
loc_1_0F4C:
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0B8h, 001h, 000h
        db  000h, 000h, 0C2h, 00Ch, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 0B8h, 001h, 000h
        db  000h, 000h, 0C2h, 00Ch, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0F80:
        push   esi
        sub    esi, esi
        db  0E8h, 008h, 0F1h, 0FEh, 0FFh
        push   1
        db  068h, 09Ch, 01Ch, 000h, 000h
        call   sub_1_0840
        call   sub_1_1A10
        cmp    eax, esi
        je     SHORT loc_1_0FB6
        db  068h, 09Ch, 01Ch, 000h, 000h
        call   sub_1_1A30
        add    esp, 4
        sub    eax, 800h
        cmp    eax, 1
        sbb    esi, esi
        neg    esi
loc_1_0FB6:
        mov    eax, esi
        pop    esi
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0FC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 4]
        lea    eax, [esi + 4bh]
        mov    dword ptr [ebp - 10h], edx
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [esi + 2fh]
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        mov    edi, dword ptr [edx + 0ch]
        mov    bl, 10h
        mov    edx, dword ptr [edi + 0feh]
        mov    dword ptr [ebp - 14h], edx
        db  089h, 035h, 094h, 01Bh, 000h, 000h
        mov    al, byte ptr [esi + 10h]
        cmp    al, 3
        mov    byte ptr [ebp - 0ch], al
        jne    SHORT loc_1_1007
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1007:
        cmp    ecx, 5
        jne    SHORT loc_1_1022
        mov    edx, dword ptr [esi + 2fh]
        mov    al, byte ptr [esi + 15h]
        cmp    byte ptr [edx + 1], al
        je     SHORT loc_1_1022
        mov    byte ptr [esi + 11h], 10h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1022:
        dec    ecx
        cmp    ecx, 5
        ja     SHORT loc_1_1030
        db  0FFh, 024h, 08Dh, 09Ch, 011h, 000h, 000h
        db  090h
loc_1_1030:
        mov    bl, 0ch
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_1040:
        mov    bl, 5
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_1050:
        cmp    byte ptr [ebp - 0ch], 9
        je     SHORT loc_1_1093
        sub    al, al
        mov    edx, dword ptr [ebp - 14h]
        sub    cl, cl
        cmp    dword ptr [edx], 0
        je     SHORT loc_1_1064
        mov    al, 1
loc_1_1064:
        mov    edx, dword ptr [ebp - 10h]
        cmp    dword ptr [edx + 10h], 0
        je     SHORT loc_1_106F
        mov    cl, 1
loc_1_106F:
        cmp    al, cl
        je     SHORT loc_1_108F
        mov    byte ptr [esi + 11h], 6
        mov    word ptr [esi + 39h], 0
        push   esi
        call   sub_1_1AD0
        add    esp, 4
        push   esi
        call   sub_1_1AF0
        add    esp, 4
loc_1_108F:
        mov    bl, 8
        jmp    SHORT loc_1_1095
loc_1_1093:
        mov    bl, 10h
loc_1_1095:
        mov    ax, word ptr [esi + 33h]
        push   eax
        mov    eax, dword ptr [esi + 2fh]
        push   eax
        push   esi
        call   sub_1_0EA0
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_10B0:
        mov    eax, dword ptr [ebp - 8]
        push   edi
        mov    dword ptr [eax + 0ah], 1
        inc    word ptr [eax + 14h]
        mov    dword ptr [edi + 0d0h], 805h
        mov    dword ptr [edi + 0d8h], 0
        mov    dword ptr [edi + 0dch], esi
        call   sub_1_1B10
        add    esp, 4
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_10F0:
        cmp    byte ptr [esi + 22h], 0
        je     SHORT loc_1_1107
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 16h]
        push   eax
        call   sub_1_016C
        mov    byte ptr [esi + 22h], 0
loc_1_1107:
        mov    dword ptr [edi + 0d0h], 800h
        mov    dword ptr [edi + 0d4h], 901h
        mov    dword ptr [edi + 0d8h], 0
        mov    dword ptr [edi + 0dch], esi
        db  0C7h, 087h, 0E0h, 000h, 000h, 000h, 0B0h, 019h, 000h, 000h
        push   edi
        call   sub_1_1B10
        add    esp, 4
        mov    byte ptr [esi + 11h], 10h
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_1150:
        mov    eax, dword ptr [ebp - 8]
        cmp    word ptr [eax + 22h], 0
        je     SHORT loc_1_1166
        mov    bl, 10h
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1166:
        lea    eax, [esi + 39h]
        push   eax
        mov    eax, dword ptr [esi + 35h]
        push   eax
        mov    ax, word ptr [esi + 33h]
        push   eax
        mov    eax, dword ptr [esi + 2fh]
        push   eax
        push   esi
        call   sub_1_1690
        or     eax, eax
        je     SHORT loc_1_1190
        mov    bl, 10h
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_1190:
        mov    bl, 7
loc_1_1192:
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  040h, 010h, 000h, 000h, 050h, 010h, 000h, 000h, 0B0h, 010h, 000h, 000h, 0F0h, 010h, 000h, 000h
        db  092h, 011h, 000h, 000h, 050h, 011h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h
        db  000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        lea    ecx, [eax - 4]
        jne    SHORT loc_1_11D3
        sub    eax, eax
        pop    ebp
        ret    0ch
loc_1_11D3:
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [ecx], 2
        mov    word ptr [eax], 0
        mov    eax, 1
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_11F0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        lea    ecx, [eax - 4]
        jne    SHORT loc_1_1203
        sub    eax, eax
        pop    ebp
        ret    10h
loc_1_1203:
        mov    byte ptr [ecx], 4
        mov    cl, byte ptr [ebp + 14h]
        or     cl, cl
        je     SHORT loc_1_1220
        mov    byte ptr [eax], cl
        mov    eax, dword ptr [ebp + 10h]
        pop    ebp
        mov    word ptr [eax], 1
        mov    eax, 1
        ret    10h
loc_1_1220:
        mov    eax, dword ptr [ebp + 10h]
        pop    ebp
        mov    word ptr [eax], 0
        mov    eax, 1
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1240:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        lea    ebx, [esi + 4bh]
        mov    edi, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [edi + 0feh]
        dec    word ptr [ebx + 22h]
        mov    eax, dword ptr [edi + 0d8h]
        mov    dword ptr [ebp - 8], ecx
        mov    cl, byte ptr [eax + 14h]
        mov    byte ptr [ebp - 1], cl
        mov    dword ptr [edi + 0d0h], 801h
        mov    dword ptr [edi + 0d4h], 0
        mov    dword ptr [edi + 0dch], 0
        mov    dword ptr [edi + 0e0h], 0
        push   0
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        push   eax
        call   sub_1_0190
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 0d8h], 0
        mov    dword ptr [ecx + 10h], 801h
        push   6
        push   edi
        call   sub_1_1A50
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_13A9
        mov    edx, dword ptr [eax + 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        add    edx, dword ptr [eax + 20h]
        cmp    dword ptr [ecx + 18h], 0
        je     SHORT loc_1_1323
        mov    byte ptr [edx], 2
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [edx + 1], cl
        mov    word ptr [edx + 2], 600h
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [edx + 4], 0
        mov    dword ptr [ecx + 10h], 1
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 14h], edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ecx + 20h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 18h], edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ecx + 24h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 1ch], edi
        lea    edi, [esi + 11h]
        mov    byte ptr [edi], 6
        jmp    SHORT loc_1_133E
loc_1_1323:
        mov    byte ptr [edx], 4
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [edx + 1], cl
        mov    word ptr [edx + 2], 600h
        lea    edi, [esi + 11h]
        mov    word ptr [edx + 4], 0
        mov    byte ptr [edi], 7
loc_1_133E:
        mov    dword ptr [esi + 3bh], eax
        mov    dword ptr [esi + 35h], edx
        mov    word ptr [esi + 39h], 6
        push   esi
        call   sub_1_13C0
        cmp    byte ptr [edi], 5
        je     SHORT loc_1_1367
        push   esi
        call   sub_1_1AD0
        add    esp, 4
        push   esi
        call   sub_1_1AF0
        add    esp, 4
loc_1_1367:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 18h], 0
        jne    SHORT loc_1_13A9
        mov    eax, dword ptr [ebx + 24h]
        dec    eax
        mov    dword ptr [ebx + 24h], eax
        jne    SHORT loc_1_1394
        push   esi
        call   sub_1_1AD0
        add    esp, 4
        push   esi
        call   sub_1_1AF0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1394:
        push   0ea60h
        lea    eax, [ebx + 28h]
        push   eax
        call   sub_1_0166
        mov    dword ptr [ebx + 34h], 1
loc_1_13A9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_13C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    edi, dword ptr [eax + 0feh]
        mov    al, byte ptr [esi + 11h]
        cmp    al, 7
        jne    SHORT loc_1_1400
        mov    byte ptr [esi + 10h], 6
        push   esi
        call   sub_1_1AD0
        add    esp, 4
        push   esi
        call   sub_1_1AF0
        add    esp, 4
        mov    byte ptr [esi + 11h], 5
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_1400:
        cmp    al, 6
        jne    SHORT loc_1_1449
        sub    ebx, ebx
        sub    cl, cl
        sub    dl, dl
        cmp    byte ptr [esi + 10h], 9
        jne    SHORT loc_1_1419
        mov    byte ptr [esi + 10h], 6
        mov    ebx, 1
loc_1_1419:
        cmp    dword ptr [edi], 0
        je     SHORT loc_1_1420
        mov    cl, 1
loc_1_1420:
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [eax + 10h], 0
        je     SHORT loc_1_142B
        mov    dl, 1
loc_1_142B:
        or     ebx, ebx
        jne    SHORT loc_1_1433
        cmp    dl, cl
        je     SHORT loc_1_1449
loc_1_1433:
        push   esi
        call   sub_1_1AD0
        add    esp, 4
        push   esi
        call   sub_1_1AF0
        add    esp, 4
        mov    byte ptr [esi + 11h], 8
loc_1_1449:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_1_1450:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        and    ebx, 0ffffh
        mov    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp + 14h]
        mov    esi, dword ptr [eax + 0ch]
        and    ecx, 0ffffh
        mov    dword ptr [ebp - 8], ecx
        lea    edi, [ebx + ecx + 2ah]
        db  0A1h, 090h, 01Bh, 000h, 000h
        push   eax
        push   0
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_018A
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], edi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 10h], 800h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 20h], ecx
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [ebp + 1ch]
        push   ebx
        mov    dword ptr [ecx + 14h], eax
        mov    edi, dword ptr [ebp - 4]
        add    edi, 28h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        add    edi, ebx
        call   sub_1_0270
        mov    byte ptr [edi], 0
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        inc    edi
        push   edi
        call   sub_1_0270
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [edi + eax], 0
        mov    dword ptr [esi + 0d0h], 800h
        mov    dword ptr [esi + 0d4h], 900h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0d8h], eax
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0dch], ecx
        db  0C7h, 086h, 0E0h, 000h, 000h, 000h, 040h, 012h, 000h, 000h
        inc    word ptr [ecx + 6dh]
        push   esi
        call   sub_1_1B10
        mov    ecx, dword ptr [ebp + 8]
        add    esp, 4
        mov    byte ptr [ecx + 11h], 10h
        sub    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 6ch
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        lea    edi, [ebp - 6ch]
        lea    esi, [edx + 4bh]
        sub    eax, eax
        mov    ecx, 10h
        rep stosd
        stosw
        lea    edi, [ebp - 28h]
        mov    ecx, 8
        rep stosd
        stosw
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        mov    bl, byte ptr [ecx]
        and    ebx, 0ffff00ffh
        inc    ecx
        or     bx, bx
        je     NEAR PTR loc_1_1669
        cmp    bx, 40h
        jae    NEAR PTR loc_1_1669
        lea    edi, [ebp - 6ch]
        or     bx, bx
        je     SHORT loc_1_15BA
loc_1_15B0:
        mov    al, byte ptr [ecx]
        mov    byte ptr [edi], al
        inc    ecx
        inc    edi
        dec    bx
        jne    SHORT loc_1_15B0
loc_1_15BA:
        mov    byte ptr [edi], 0
        inc    edi
        cmp    dword ptr [ebp - 4], ecx
        jae    SHORT loc_1_15E0
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   edx
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_1_15E0:
        mov    bl, byte ptr [ecx]
        and    ebx, 0ffff00ffh
        inc    ecx
        cmp    bx, 20h
        jb     SHORT loc_1_160A
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   edx
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_160A:
        or     bx, bx
        je     SHORT loc_1_165B
        mov    eax, dword ptr [esi + 2]
        or     eax, eax
        je     SHORT loc_1_1629
        cmp    eax, 1
        je     SHORT loc_1_1648
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_1629:
        or     bx, bx
        je     SHORT loc_1_165B
        mov    edi, edi
loc_1_1630:
        mov    al, byte ptr [ecx]
        mov    byte ptr [edi], al
        inc    ecx
        inc    edi
        dec    bx
        jne    SHORT loc_1_1630
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_1648:
        lea    eax, [ebp - 28h]
        push   eax
        push   ecx
        call   sub_1_0810
        lea    eax, [ebp - 28h]
        push   eax
        call   sub_1_0320
loc_1_165B:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_1669:
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   edx
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1690:
        push   ebp
        mov    ebp, esp
        sub    esp, 78h
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        add    edx, 4bh
        mov    dword ptr [ebp - 8], edx
        lea    edi, [ebp - 78h]
        sub    eax, eax
        mov    ecx, 10h
        rep stosd
        stosw
        mov    word ptr [ebp - 4], ax
        lea    edi, [ebp - 34h]
        mov    ecx, 8
        rep stosd
        stosw
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        lea    esi, [edx + 4]
        and    ecx, 0ffffh
        lea    ecx, [edx + ecx - 4]
        mov    dword ptr [ebp - 0ch], ecx
        mov    cl, byte ptr [edx + 1]
        mov    byte ptr [ebp - 1], cl
        mov    edi, dword ptr [ebp - 10h]
        nop
loc_1_16E0:
        cmp    esi, dword ptr [ebp - 0ch]
        jae    NEAR PTR loc_1_1920
        mov    cl, byte ptr [esi]
        and    ecx, 0ffff00ffh
        mov    bl, byte ptr [esi + 1]
        inc    esi
        and    ebx, 0ffff00ffh
        inc    esi
        and    ecx, 0ffffh
        cmp    ecx, 1
        je     SHORT loc_1_1745
        cmp    ecx, 2
        je     SHORT loc_1_1780
        cmp    ecx, 3
        je     NEAR PTR loc_1_17E0
        cmp    ecx, 4
        je     NEAR PTR loc_1_1832
        cmp    ecx, 5
        je     NEAR PTR loc_1_1900
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_1745:
        lea    ecx, [ebx - 2]
        mov    word ptr [ebp - 4], cx
        or     cx, cx
        je     NEAR PTR loc_1_1884
        cmp    cx, 40h
        jae    NEAR PTR loc_1_1884
        lea    edi, [ebp - 78h]
        or     cx, cx
        je     SHORT loc_1_1774
        mov    dx, cx
loc_1_176A:
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        inc    esi
        inc    edi
        dec    dx
        jne    SHORT loc_1_176A
loc_1_1774:
        mov    byte ptr [edi], 0
        inc    edi
        jmp    NEAR PTR loc_1_16E0
        db  08Dh, 049h, 000h
loc_1_1780:
        lea    eax, [ebx - 2]
        or     ax, ax
        je     SHORT loc_1_17CA
        cmp    ax, 20h
        jae    NEAR PTR loc_1_18A2
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [edx + 2]
        or     ecx, ecx
        je     SHORT loc_1_17A3
        cmp    ecx, 1
        je     SHORT loc_1_17B7
        jmp    SHORT loc_1_17CA
loc_1_17A3:
        or     ax, ax
        je     SHORT loc_1_17CA
        mov    dx, ax
loc_1_17AB:
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        inc    esi
        inc    edi
        dec    dx
        jne    SHORT loc_1_17AB
        jmp    SHORT loc_1_17CA
loc_1_17B7:
        lea    eax, [ebp - 34h]
        push   eax
        push   esi
        call   sub_1_0810
        lea    eax, [ebp - 34h]
        push   eax
        call   sub_1_0320
loc_1_17CA:
        mov    ecx, ebx
        and    ecx, 0ffffh
        sub    ecx, 2
        add    esi, ecx
        jmp    NEAR PTR loc_1_16E0
        db  08Dh, 064h, 024h, 000h
loc_1_17E0:
        cmp    bx, 4
        jb     NEAR PTR loc_1_18C0
        mov    dx, word ptr [esi]
        mov    ch, dl
        mov    cl, dh
        mov    edx, dword ptr [ebp - 8]
        mov    word ptr [edx + 6], cx
        mov    edx, ebx
        and    edx, 0ffffh
        sub    edx, 2
        add    esi, edx
        and    ecx, 0ffffh
        cmp    ecx, 1
        je     NEAR PTR loc_1_16E0
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_1832:
        cmp    bx, 4
        jb     NEAR PTR loc_1_18E0
        mov    dx, word ptr [esi]
        mov    ch, dl
        mov    cl, dh
        mov    edx, dword ptr [ebp - 8]
        mov    word ptr [edx + 8], cx
        mov    edx, ebx
        and    edx, 0ffffh
        sub    edx, 2
        add    esi, edx
        and    ecx, 0ffffh
        cmp    ecx, 1
        je     NEAR PTR loc_1_16E0
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_1884:
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_18A2:
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_18C0:
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_1_18E0:
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_1_1900:
        push   3
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_1_1920:
        cmp    word ptr [ebp - 4], 0
        jne    SHORT loc_1_1945
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_1945:
        mov    cl, byte ptr [ebp - 1]
        push   ecx
        lea    ecx, [ebp - 34h]
        push   ecx
        push   eax
        lea    eax, [ebp - 78h]
        push   eax
        mov    cx, word ptr [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1450
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_1_1970:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 7fh], 0
        lea    eax, [esi + 4bh]
        je     SHORT loc_1_19A1
        mov    dword ptr [eax + 34h], 0
        mov    byte ptr [esi + 10h], 6
        mov    byte ptr [esi + 11h], 5
        push   esi
        call   sub_1_1AD0
        add    esp, 4
        push   esi
        call   sub_1_1AF0
        add    esp, 4
loc_1_19A1:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_19B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [eax + 0d0h], 801h
        mov    dword ptr [eax + 0d4h], 0
        mov    dword ptr [eax + 0dch], 0
        mov    dword ptr [eax + 0e0h], 0
        mov    eax, dword ptr [ebp + 0ch]
        push   esi
        mov    dword ptr [eax + 8], 801h
        mov    byte ptr [esi + 11h], 9
        call   sub_1_1AD0
        add    esp, 4
        push   esi
        call   sub_1_1AF0
        add    esp, 4
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0CDh, 020h, 000h, 000h, 099h, 004h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1A30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 099h, 004h
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_1A50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 002h, 000h, 099h, 004h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 004h, 000h, 099h, 004h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
        push   ebp
        mov    ebp, esp
        push   word ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 005h, 000h, 099h, 004h
        add    esp, 8
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   word ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 006h, 000h, 099h, 004h
        add    esp, 8
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1AD0:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 007h, 000h, 099h, 004h
        add    esp, 4
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1AF0:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 008h, 000h, 099h, 004h
loc_1_1AFC:
        add    esp, 4
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B10:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 009h, 000h, 099h, 004h
        add    esp, 4
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 010h, 020h, 030h, 004h, 014h, 024h, 034h, 008h, 018h, 028h, 038h, 00Ch
        db  01Ch, 02Ch, 03Ch, 000h, 020h, 010h, 030h, 008h, 028h, 018h, 038h, 004h, 024h, 014h, 034h, 00Ch
        db  02Ch, 01Ch, 03Ch
loc_1_1B54:
        cdq
        inc    eax
        pop    ss
        and    bl, dl
        leave
        or     bl, dh
        or     byte ptr [ecx + 79d4da4h], ch
        push   2
        inc    edi
        pop    esi
        push   esp
        fsubp  st(4)
        scasb
        sti
        sub    esi, esi
        jge    SHORT loc_1_1AFC
        mov    word ptr [esi - 52h], fs
        db  0BAh, 000h
loc_1_1B74:
        rol    byte ptr [edx + 1d29b6deh], cl
        db  0DFh, 0D5h, 0C5h, 0DDh, 0ACh, 0BFh, 095h, 097h, 0EAh, 092h, 0F5h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
