; vnbt_lcode_01_05.asm -- VNBT object 1 segment _LTEXT  (part 5/5)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_1239C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi], 22h
        jne    SHORT loc_1_123B9
        inc    esi
        push   22h
        push   esi
        call   sub_1_12CE8
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_123B9
        mov    byte ptr [eax], 0
loc_1_123B9:
        push   dword ptr [esp + 14h]
        push   0
        push   dword ptr [esp + 18h]
        push   esi
        call   dword ptr [esp + 1ch]
        add    esp, 10h
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_123D0:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [ecx + 8]
        mov    eax, dword ptr [esi + 2ch]
        mov    eax, dword ptr [eax + 7ch]
        or     eax, eax
        je     NEAR PTR loc_1_1248C
        cmp    eax, 7f000000h
        je     NEAR PTR loc_1_1248C
        mov    ax, word ptr [esi + 4ch]
        and    ax, 0fff8h
        mov    word ptr [esi + 4ch], ax
        or     ax, 80h
        mov    word ptr [esi + 4ch], ax
        mov    ebx, dword ptr [ecx + 0ch]
        mov    edi, dword ptr [ecx + 10h]
        push   ecx
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [esi + 38h]
        db  0C7h, 000h, 07Ch, 032h, 001h, 000h
        db  0A1h, 080h, 032h, 001h, 000h
        mov    ecx, dword ptr [esi + 38h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 38h]
        db  08Bh, 00Dh, 080h, 032h, 001h, 000h
        mov    dword ptr [ecx], eax
        db  0A3h, 080h, 032h, 001h, 000h
        db  0A1h, 080h, 02Fh, 001h, 000h
        mov    edx, dword ptr [eax + 98h]
        mov    ax, word ptr [eax + 9ch]
        inc    dword ptr [esi + 34h]
        db  08Bh, 00Dh, 033h, 033h, 001h, 000h
        mov    dword ptr [esi + 3ch], ecx
        push   1
        push   edx
        and    eax, 0ffffh
        push   eax
        push   edi
        push   ebx
        db  068h, 02Ch, 0F0h, 000h, 000h
        push   esi
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   dword ptr [esi + 38h]
        call   sub_1_10C0C
        add    esp, 24h
        mov    edi, eax
        push   esi
        call   sub_1_F280
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_1248C:
        pop    edi
        mov    eax, 0c00000beh
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h, 08Bh, 015h, 074h, 033h, 001h, 000h, 00Bh, 0D2h, 074h, 032h, 08Bh, 0C2h, 08Bh
        db  040h, 008h, 08Bh, 048h, 038h, 0C7h, 005h, 074h, 033h, 001h, 000h, 000h, 000h, 000h, 000h, 08Bh
        db  044h, 024h, 004h, 089h, 041h, 00Ch, 08Bh, 041h, 020h, 025h, 00Fh, 0FFh, 0FFh, 0FFh, 089h, 041h
        db  020h, 083h, 0C8h, 018h, 089h, 041h, 020h, 08Bh, 044h, 024h, 008h, 089h, 010h, 0C3h, 090h, 08Bh
        db  044h, 024h, 008h, 0C7h, 000h, 000h, 000h, 000h, 000h, 0C3h, 090h
sub_1_124E0:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 34h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        mov    dword ptr [esp + 14h], eax
        cmp    eax, 10h
        mov    dword ptr [esp + 18h], 1
        je     SHORT loc_1_12512
        mov    dword ptr [esp + 18h], 3
loc_1_12512:
        sub    ebp, ebp
        cmp    dword ptr [esp + 18h], ebp
        jle    NEAR PTR loc_1_125CD
        mov    ebx, dword ptr [esp + 38h]
loc_1_12522:
        cmp    dword ptr [esp + 14h], 10h
        je     SHORT loc_1_12544
        db  08Ah, 085h, 008h, 032h, 001h, 000h
        push   eax
        push   dword ptr [esp + 38h]
        lea    eax, [esp + 24h]
        push   eax
        call   sub_1_122EC
        add    esp, 0ch
        jmp    SHORT loc_1_12550
        db  090h
loc_1_12544:
        lea    edi, [esp + 1ch]
        mov    esi, dword ptr [esp + 34h]
        movsd
        movsd
        movsd
        movsd
loc_1_12550:
        lea    eax, [esp + 10h]
        push   eax
        push   0
        push   ebx
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        lea    eax, [esp + 2ch]
        push   eax
        db  0FFh, 035h, 0B8h, 032h, 001h, 000h
        call   sub_1_65BC
        add    esp, 18h
        or     eax, eax
        mov    eax, dword ptr [esp + 10h]
        jne    SHORT loc_1_125B8
        mov    dword ptr [eax + 1ch], 2
        mov    eax, dword ptr [esp + 10h]
        or     dword ptr [eax + 20h], 810h
        mov    eax, dword ptr [esp + 10h]
        and    dword ptr [eax + 20h], 0ffffffbfh
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 18h], 0ffffffffh
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 24h], 0facf0000h
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 28h], 0ffffffffh
        jmp    SHORT loc_1_125BB
        db  08Bh, 0FFh
loc_1_125B8:
        mov    dword ptr [eax + 0ch], ebx
loc_1_125BB:
        cmp    dword ptr [esp + 14h], 10h
        je     SHORT loc_1_125D8
        inc    ebp
        cmp    dword ptr [esp + 18h], ebp
        jg     NEAR PTR loc_1_12522
loc_1_125CD:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  090h
loc_1_125D8:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Bh, 0FFh
sub_1_125E4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 03Dh, 0B8h, 032h, 001h, 000h
        sub    ebx, ebx
        mov    eax, edi
        cmp    dword ptr [eax], ebx
        jle    SHORT loc_1_12633
        mov    ebp, edi
        add    ebp, 8
        nop
loc_1_125FC:
        mov    esi, dword ptr [ebp]
        cmp    esi, ebp
        je     SHORT loc_1_1262B
        nop
loc_1_12604:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        test   byte ptr [eax + 21h], 8
        je     SHORT loc_1_12627
        cmp    dword ptr [eax + 1ch], 2
        jne    SHORT loc_1_12627
        mov    edx, esi
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_12627:
        cmp    esi, ebp
        jne    SHORT loc_1_12604
loc_1_1262B:
        inc    ebx
        add    ebp, 8
        cmp    dword ptr [edi], ebx
        jg     SHORT loc_1_125FC
loc_1_12633:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_12638:
        sub    esp, 44h
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  080h, 03Dh, 054h, 033h, 001h, 000h, 000h
        jne    SHORT loc_1_12654
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 44h
        ret
        db  08Bh, 0FFh
loc_1_12654:
        db  068h, 020h, 032h, 001h, 000h
        lea    eax, [esp + 28h]
        mov    dword ptr [esp + 2ch], eax
        mov    dword ptr [esp + 28h], eax
        mov    esi, dword ptr [esp + 5ch]
        push   esi
        call   sub_1_12000
        add    esp, 8
        cmp    al, 1
        jne    SHORT loc_1_12680
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 44h
        ret
loc_1_12680:
        push   esi
        call   sub_1_0620
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_1269C
        mov    eax, 0ffffffffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 44h
        ret
loc_1_1269C:
        sub    edi, edi
loc_1_1269E:
        sub    ebx, ebx
loc_1_126A0:
        lea    eax, [esp + 20h]
        push   eax
        push   esi
        call   sub_1_0510
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_1282C
        cmp    edi, 61h
        jge    NEAR PTR loc_1_1282C
        mov    dword ptr [esp + 14h], 5
        lea    ecx, [esp + 14h]
        push   ecx
        lea    ecx, [esp + 44h]
        push   ecx
        push   eax
        call   sub_1_11C8C
        add    esp, 0ch
        mov    dword ptr [esp + 10h], eax
        and    al, 0fh
        cmp    al, 6
        je     SHORT loc_1_126A0
        cmp    dword ptr [esp + 44h], 0
        je     SHORT loc_1_126A0
        test   byte ptr [esp + 10h], 10h
        je     SHORT loc_1_12735
        lea    eax, [esp + 1ch]
        push   eax
        push   dword ptr [esp + 44h]
        call   sub_1_FD20
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_1270E
        cmp    eax, 0ffh
        jne    SHORT loc_1_12735
loc_1_1270E:
        mov    eax, dword ptr [esp + 10h]
        and    eax, 20h
        shr    eax, 5
        push   eax
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 4ch]
        call   sub_1_124E0
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_12734
        cmp    eax, 0ffh
        jne    SHORT loc_1_12735
loc_1_12734:
        inc    edi
loc_1_12735:
        mov    eax, dword ptr [esp + 10h]
        shl    eax, 1ch
        sar    eax, 1ch
        cmp    eax, 2
        je     SHORT loc_1_12760
        cmp    eax, 3
        je     SHORT loc_1_127C0
        cmp    eax, 4
        je     NEAR PTR loc_1_12820
        cmp    eax, 5
        je     NEAR PTR loc_1_1269E
        jmp    NEAR PTR loc_1_126A0
loc_1_12760:
        mov    eax, dword ptr [esp + 14h]
        dec    eax
        cmp    eax, 2
        jl     NEAR PTR loc_1_126A0
        push   1ch
        mov    eax, dword ptr [esp + 4ch]
        add    eax, 5
        push   eax
        lea    eax, [esp + 34h]
        push   eax
        call   sub_1_122EC
        add    esp, 0ch
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 44h]
        call   sub_1_FD20
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_127A5
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_126A0
loc_1_127A5:
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [esp + 1ch]
        lea    eax, [esp + 34h]
        push   eax
        call   sub_1_12140
        add    esp, 0ch
        jmp    NEAR PTR loc_1_126A0
loc_1_127C0:
        cmp    ebx, 2
        je     NEAR PTR loc_1_126A0
        cmp    dword ptr [esp + 14h], 2
        jl     NEAR PTR loc_1_126A0
        push   esi
        call   sub_1_0700
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_126A0
        push   0
        push   0
        db  068h, 038h, 026h, 001h, 000h
        push   dword ptr [esp + 50h]
        call   sub_1_1239C
        add    esp, 10h
        mov    ebp, eax
        push   esi
        call   sub_1_0740
        add    esp, 4
        cmp    ebp, -1
        je     NEAR PTR loc_1_126A0
        cmp    ebx, 1
        jne    SHORT loc_1_12818
        mov    ebx, 2
loc_1_12818:
        add    edi, ebp
        jmp    NEAR PTR loc_1_126A0
        db  090h
loc_1_12820:
        mov    ebx, 1
        jmp    NEAR PTR loc_1_126A0
        db  08Bh, 0FFh
loc_1_1282C:
        push   esi
        call   sub_1_04E0
        add    esp, 4
        lea    eax, [esp + 24h]
        push   eax
        call   sub_1_1228C
        add    esp, 4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 44h
        ret
sub_1_1284C:
        db  08Bh, 00Dh, 074h, 033h, 001h, 000h
        or     ecx, ecx
        je     SHORT loc_1_12868
        db  0C7h, 005h, 074h, 033h, 001h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        ret
        db  090h
loc_1_12868:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        ret
        db  090h
sub_1_12874:
        push   esi
        push   edi
        sub    esi, esi
        cmp    byte ptr [esp + 14h], 0
        je     SHORT loc_1_1288F
        db  08Bh, 035h, 074h, 033h, 001h, 000h
        db  0C7h, 005h, 074h, 033h, 001h, 000h, 000h, 000h, 000h, 000h
loc_1_1288F:
        or     esi, esi
        je     SHORT loc_1_128FC
        mov    edi, dword ptr [esi + 8]
        mov    eax, dword ptr [edi + 38h]
        and    dword ptr [eax + 20h], 0ffffff0fh
        mov    eax, dword ptr [edi + 38h]
        or     dword ptr [eax + 20h], 10h
        mov    eax, dword ptr [esp + 0ch]
        cmp    eax, -1
        jne    SHORT loc_1_128B2
        sub    eax, eax
loc_1_128B2:
        mov    ecx, dword ptr [edi + 38h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [edi + 38h]
        mov    dword ptr [eax + 28h], 0ffffffffh
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   dword ptr [edi + 38h]
        call   sub_1_6608
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_128F0
        cmp    eax, 0ffh
        je     SHORT loc_1_128F0
        push   dword ptr [edi + 38h]
        call   sub_1_DEA4
        add    esp, 4
        mov    dword ptr [edi + 38h], 0
loc_1_128F0:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], esi
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_128FC:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_1290C:
        sub    esp, 0ch
        push   esi
        push   edi
        lea    eax, [esp + 0ch]
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [esp + 0ch], eax
        db  083h, 03Dh, 074h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_12960
        db  0A1h, 074h, 033h, 001h, 000h
        cmp    byte ptr [eax + 14h], 0
        je     SHORT loc_1_12948
        db  0C7h, 005h, 074h, 033h, 001h, 000h, 000h, 000h, 000h, 000h
        push   15h
        push   eax
        call   sub_1_12C6C
        add    esp, 8
        jmp    SHORT loc_1_12960
loc_1_12948:
        mov    eax, dword ptr [esp + 20h]
        add    eax, 3ch
        mov    dword ptr [esp + 8], eax
        or     word ptr [eax], 1
        db  0A1h, 074h, 033h, 001h, 000h
        mov    byte ptr [eax + 14h], 1
loc_1_12960:
        mov    esi, dword ptr [esp + 20h]
        add    esi, 3ch
        push   esi
        lea    eax, [esp + 10h]
        push   eax
        db  068h, 06Ch, 033h, 001h, 000h
        call   sub_1_129C4
        add    esp, 0ch
        lea    eax, [esp + 0ch]
        cmp    dword ptr [esp + 0ch], eax
        je     SHORT loc_1_129AD
        mov    edi, dword ptr [esp + 0ch]
        cmp    edi, eax
        je     SHORT loc_1_129AD
loc_1_1298C:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        mov    edx, edi
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        push   15h
        push   eax
        call   sub_1_12C6C
        add    esp, 8
        lea    eax, [esp + 0ch]
        cmp    edi, eax
        jne    SHORT loc_1_1298C
loc_1_129AD:
        test   byte ptr [esi], 1
        jne    SHORT loc_1_129BC
        db  0C7h, 005h, 079h, 033h, 001h, 000h, 000h, 000h, 000h, 000h
loc_1_129BC:
        pop    edi
        pop    esi
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
sub_1_129C4:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi]
        cmp    esi, edi
        je     SHORT loc_1_12A0C
        mov    eax, dword ptr [esp + 18h]
        or     word ptr [eax], 1
        cmp    esi, edi
        je     SHORT loc_1_12A0C
        mov    edx, dword ptr [esp + 14h]
loc_1_129E1:
        mov    ebx, edi
        mov    edi, dword ptr [edi]
        cmp    byte ptr [ebx + 14h], 0
        je     SHORT loc_1_12A04
        mov    ecx, edi
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ebx], edx
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [eax], ebx
        mov    dword ptr [edx + 4], ebx
        jmp    SHORT loc_1_12A08
loc_1_12A04:
        mov    byte ptr [ebx + 14h], 1
loc_1_12A08:
        cmp    esi, edi
        jne    SHORT loc_1_129E1
loc_1_12A0C:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_12A10:
        sub    esp, 4
        mov    eax, dword ptr [esp + 0ch]
        mov    byte ptr [eax + 14h], 0
        db  083h, 03Dh, 079h, 033h, 001h, 000h, 000h
        jne    SHORT loc_1_12A6D
        lea    eax, [esp]
        push   eax
        push   0
        db  068h, 00Ch, 029h, 001h, 000h
        push   0
        db  068h, 00Ch, 029h, 001h, 000h
        push   0
        push   0
        db  0FFh, 035h, 023h, 033h, 001h, 000h
        call   sub_1_109B8
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_1_12A64
        cmp    eax, 0ffh
        je     SHORT loc_1_12A64
        db  0C7h, 005h, 079h, 033h, 001h, 000h, 000h, 000h, 000h, 000h
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_12A64:
        mov    eax, dword ptr [esp]
        db  0A3h, 079h, 033h, 001h, 000h
loc_1_12A6D:
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_12A74:
        push   esi
        push   edi
        mov    esi, 0c0000001h
        push   15h
        call   sub_1_001E
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_12AF4
        mov    esi, dword ptr [esp + 0ch]
        mov    dword ptr [edi + 8], esi
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edi + 0ch], eax
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [edi + 10h], ecx
        db  080h, 03Dh, 078h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_12AC8
        db  0C7h, 007h, 06Ch, 033h, 001h, 000h
        db  0A1h, 070h, 033h, 001h, 000h
        mov    dword ptr [edi + 4], eax
        db  0A1h, 070h, 033h, 001h, 000h
        mov    dword ptr [eax], edi
        db  089h, 03Dh, 070h, 033h, 001h, 000h
        jmp    SHORT loc_1_12AE8
        db  08Bh, 0FFh
loc_1_12AC8:
        db  089h, 03Dh, 074h, 033h, 001h, 000h
        db  0C6h, 005h, 078h, 033h, 001h, 000h, 001h
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        push   ecx
        push   eax
        push   esi
        call   sub_1_3C00
        add    esp, 14h
loc_1_12AE8:
        push   edi
        push   esi
        call   sub_1_12A10
        add    esp, 8
        sub    esi, esi
loc_1_12AF4:
        mov    eax, esi
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_12AFC:
        push   esi
        push   edi
        db  08Bh, 015h, 074h, 033h, 001h, 000h
        or     edx, edx
        jne    SHORT loc_1_12B2E
        db  081h, 03Dh, 06Ch, 033h, 001h, 000h, 06Ch, 033h, 001h, 000h
        je     SHORT loc_1_12B44
        db  08Bh, 015h, 06Ch, 033h, 001h, 000h
        mov    eax, edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        db  089h, 015h, 074h, 033h, 001h, 000h
loc_1_12B2E:
        mov    eax, dword ptr [edx + 8]
        mov    esi, dword ptr [eax + 38h]
        mov    edi, dword ptr [esp + 0ch]
        add    esi, 34h
        movsd
        movsd
        movsd
        movsd
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_1_12B44:
        pop    edi
        db  0C6h, 005h, 078h, 033h, 001h, 000h, 000h
        mov    eax, 0c0000001h
        pop    esi
        ret
        db  090h
loc_1_12B54:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_1_0024
        add    esp, 4
        nop
loc_1_12B68:
        sub    bl, bl
        mov    dword ptr [ebp - 8], 0
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_1_12AFC
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_12B8C
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_12C68
loc_1_12B8C:
        sub    eax, eax
        mov    esi, 15h
        db  038h, 005h, 054h, 033h, 001h, 000h
        je     SHORT loc_1_12BFE
        db  038h, 005h, 088h, 033h, 001h, 000h
        jne    SHORT loc_1_12BC1
        push   eax
        push   1
        push   eax
        db  0FFh, 035h, 027h, 033h, 001h, 000h
        call   sub_1_12638
        add    esp, 10h
        cmp    eax, -1
        je     SHORT loc_1_12BC1
        db  0C6h, 005h, 088h, 033h, 001h, 000h, 001h
loc_1_12BC1:
        lea    eax, [ebp - 1]
        push   eax
        push   1
        lea    eax, [ebp - 18h]
        push   eax
        db  0FFh, 035h, 027h, 033h, 001h, 000h
        call   sub_1_11A0C
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_12C3C
        db  080h, 03Dh, 053h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_12BFE
        lea    eax, [ebp - 1]
        push   eax
        push   1
        lea    eax, [ebp - 18h]
        push   eax
        db  0FFh, 035h, 02Fh, 033h, 001h, 000h
        call   sub_1_11A0C
        add    esp, 10h
loc_1_12BFE:
        or     eax, eax
        jne    SHORT loc_1_12C3C
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_1284C
        add    esp, 4
        db  080h, 03Dh, 053h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_12C4D
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_12C4D
        push   dword ptr [ebp - 8]
        call   sub_1_123D0
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_12C36
        cmp    esi, 0ffh
        jne    SHORT loc_1_12C4D
loc_1_12C36:
        mov    bl, 1
        jmp    SHORT loc_1_12C4D
        db  08Bh, 0FFh
loc_1_12C3C:
        push   1
        lea    ecx, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_12874
        add    esp, 0ch
        sub    esi, esi
loc_1_12C4D:
        or     bl, bl
        jne    NEAR PTR loc_1_12B68
        push   esi
        push   dword ptr [ebp - 8]
        call   sub_1_12C6C
        add    esp, 8
        jmp    NEAR PTR loc_1_12B68
        db  08Bh, 0FFh
loc_1_12C68:
        pop    esi
        pop    ebx
        leave
        ret
sub_1_12C6C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_1_12CBA
        mov    esi, dword ptr [eax + 8]
        mov    ebx, dword ptr [eax + 10h]
        mov    edi, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_0024
        add    esp, 4
        mov    ebp, dword ptr [esp + 18h]
        or     ebp, ebp
        je     SHORT loc_1_12CA2
        mov    eax, dword ptr [esi + 38h]
        or     eax, eax
        je     SHORT loc_1_12CA2
        push   eax
        call   sub_1_12CC0
        add    esp, 4
loc_1_12CA2:
        push   esi
        call   sub_1_F280
        add    esp, 4
        or     ebx, ebx
        je     SHORT loc_1_12CBA
        push   ebp
        push   edi
        push   ebx
        call   sub_1_E64C
        add    esp, 0ch
loc_1_12CBA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_12CC0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 20h]
        and    ecx, 0ffffff0fh
        mov    dword ptr [eax + 20h], ecx
        or     ecx, 20h
        mov    dword ptr [eax + 20h], ecx
        mov    dword ptr [eax + 10h], 0
        push   eax
        call   sub_1_DEA4
        add    esp, 4
        ret
        db  090h
sub_1_12CE8:
        mov    eax, dword ptr [esp + 4]
        movsx  ecx, byte ptr [eax]
        mov    edx, dword ptr [esp + 8]
        cmp    ecx, edx
        je     SHORT loc_1_12D05
loc_1_12CF8:
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_12D05
        inc    eax
        movsx  ecx, byte ptr [eax]
        cmp    ecx, edx
        jne    SHORT loc_1_12CF8
loc_1_12D05:
        movsx  ecx, byte ptr [eax]
        cmp    ecx, edx
        je     SHORT loc_1_12D0E
        sub    eax, eax
loc_1_12D0E:
        ret
        db  090h, 0CDh, 020h, 029h, 080h, 028h, 000h, 0CDh, 020h, 02Ah, 080h, 028h, 000h, 0CDh, 020h, 02Bh
        db  080h, 028h, 000h, 0CDh, 020h, 02Ch, 080h, 028h, 000h, 0CDh, 020h, 02Dh, 080h, 028h, 000h, 0CDh
        db  020h, 02Eh, 080h, 028h, 000h, 0CDh, 020h, 02Fh, 080h, 028h, 000h, 0CDh, 020h, 030h, 080h, 028h
        db  000h, 0CDh, 020h, 050h, 080h, 028h, 000h
sub_1_12D46:
        db  0CDh, 020h, 051h, 080h, 028h, 000h
        db  0CDh, 020h, 052h, 080h, 028h, 000h
        db  0CDh, 020h, 053h, 080h, 028h, 000h
        db  0CDh, 020h, 000h, 080h, 028h, 000h
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
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
sub_1_12E06:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_12E0C:
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
loc_1_12E68:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  09Dh, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 09Bh, 004h, 003h, 000h, 000h, 000h, 056h, 04Eh, 042h, 054h
        db  020h, 020h, 020h, 020h, 000h, 0C0h, 000h, 054h, 0AAh, 000h, 000h, 000h, 0A6h, 002h, 000h, 000h
        db  0A6h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  06Ch, 02Eh, 001h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_12ECC:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_12ED4:
        dec    esi
        outsd
        push   esp
        db  079h, 070h
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_12EE0:
        push   ebx
        arpl   word ptr [edi + 70h], bp
        dec    ecx
        db  064h, 000h
loc_1_12EE8:
        inc    edx
        db  072h, 06Fh
        popad
        arpl   word ptr fs:[ecx + 73h], sp
        je     SHORT loc_1_12F33
        jb     NEAR PTR loc_1_12F5B
        jae    SHORT loc_1_12F6B
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12EFC:
        aam    2eh
        add    dword ptr [eax], eax
loc_1_12F00:
        add    byte ptr cs:[eax], al
        db  000h, 0E0h
        add    dword ptr cs:[eax], eax
        das
        add    byte ptr [eax], al
        db  000h, 0E8h
        add    dword ptr cs:[eax], eax
        sbb    al, 0
        add    byte ptr [eax], al
loc_1_12F14:
        dec    esp
        inc    ecx
        dec    esi
        inc    ecx
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12F20:
        push   7374736fh
        add    byte ptr [eax], al
        add    byte ptr [edi + ebp*2 + 6dh], al
        popad
        imul   ebp, dword ptr [esi], 61655300h
loc_1_12F33:
        jb     SHORT loc_1_12F98
        push   7473694ch
        add    byte ptr [eax], al
loc_1_12F3C:
        sub    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_12F4C:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12F50:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12F54:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
loc_1_12F5B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 61h], cl
        insd
loc_1_12F6B:
        push   ebx
        jb     NEAR PTR loc_1_12FE6
        db  065h, 072h, 024h
        add    byte ptr [esi + 61h], cl
        insd
        push   ebx
        jb     NEAR PTR loc_1_12FF2
        jb     NEAR PTR loc_1_12F7F
loc_1_12F7F:
        db  000h, 034h, 032h
        add    dword ptr [eax], eax
loc_1_12F84:
        dec    esi
        popad
        insd
        push   ebx
        db  065h, 072h, 076h
        db  065h, 072h, 050h
        outsd
        jb     SHORT loc_1_13006
        add    byte ptr [eax], al
loc_1_12F94:
        inc    esp
        outsb
        db  073h, 053h
loc_1_12F98:
        db  065h, 072h, 076h
        db  065h, 072h, 050h
        outsd
        jb     SHORT loc_1_13015
        add    byte ptr [eax], al
        add    byte ptr [esi + 61h], cl
        insd
        push   esp
        popad
        bound  ebp, qword ptr [ebp + 53h]
        imul   edi, dword ptr [edx + 65h], 53000000h
        jae    NEAR PTR loc_1_1302B
        imul   ebp, dword ptr [edi + 6eh], 6c626154h
        push   ebx
        imul   edi, dword ptr [edx + 65h], 0
loc_1_12FC8:
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 74756fh
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6eh], cl
        imul   esi, dword ptr [ecx + ebp*2 + 61h], 6665526ch
        jb     SHORT loc_1_13049
        db  073h, 068h
loc_1_12FE6:
        push   esp
        dec    edi
        add    byte ptr cs:[eax], al
loc_1_12FEC:
        inc    edx
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_13040
loc_1_12FF2:
        popad
        insd
        push   ecx
        db  075h, 065h
        jb     SHORT loc_1_13073
        inc    ebx
        outsd
        jne    SHORT loc_1_1306C
        je     SHORT loc_1_13000
loc_1_13000:
        inc    edx
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_13057
loc_1_13006:
        db  075h, 065h
        jb     SHORT loc_1_13083
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 74756fh
        add    byte ptr [eax], al
loc_1_13014:
        dec    esi
loc_1_13015:
        popad
        insd
        push   ebx
        db  072h, 076h
        push   ecx
        jne    SHORT loc_1_13083
        db  072h, 079h
        inc    ebx
        outsd
        db  075h, 06Eh
        je     SHORT loc_1_13026
loc_1_13026:
        add    byte ptr [eax], al
loc_1_13028:
        dec    esi
        popad
        insd
loc_1_1302B:
        push   ebx
        db  072h, 076h
        push   ecx
        jne    SHORT loc_1_13097
        db  072h, 079h
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 74756fh
loc_1_1303C:
        push   ebx
        db  065h, 073h, 073h
loc_1_13040:
        imul   ebp, dword ptr [edi + 6eh], 7065654bh
        inc    ecx
        insb
loc_1_13049:
        imul   esi, dword ptr [esi + 65h], 0
loc_1_13050:
        push   edx
        popad
        outsb
        outsd
        insd
        inc    ecx
loc_1_13057:
        popad
        jo     SHORT loc_1_130CF
        jb     NEAR PTR loc_1_1305E
loc_1_1305E:
        add    byte ptr [eax], al
loc_1_13060:
        push   ebx
        imul   ebp, dword ptr [esi + 67h], 6552656ch
        db  073h, 070h
        outsd
        outsb
loc_1_1306C:
        jae    SHORT loc_1_130D3
        add    byte ptr [eax], al
loc_1_13070:
        inc    ebp
        outsb
        popad
loc_1_13073:
        bound  ebp, qword ptr [ebp + 44h]
        outsb
        jae    SHORT loc_1_1307A
loc_1_1307A:
        add    byte ptr [eax], al
loc_1_1307C:
        dec    esp
        insd
        push   7374736fh
loc_1_13083:
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 74756fh
        add    byte ptr [ebp + 61h], cl
        db  078h, 044h
        db  067h, 072h, 061h
        insd
        inc    edx
        jne    SHORT loc_1_130FD
loc_1_13097:
        jb     NEAR PTR loc_1_13104
        outsb
        add    byte ptr [bx + si], al
        add    byte ptr [ebp + 6eh], al
        popad
        bound  ebp, qword ptr [ebp + 50h]
        jb     SHORT loc_1_13118
        db  078h, 079h
        push   edx
        inc    ebx
        push   6b6365h
loc_1_130B4:
        push   edi
        imul   ebp, dword ptr [esi + 73h], 6e776f44h
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 74756fh
loc_1_130C4:
        push   edx
        db  065h, 066h, 072h, 065h
        db  073h, 068h
        dec    edi
        db  070h, 043h
        outsd
loc_1_130CF:
        add    byte ptr gs:[eax], al
loc_1_130D3:
        add    byte ptr [ebp + 6eh], al
        popad
        bound  ebp, qword ptr [ebp + 50h]
        jb     SHORT loc_1_1314C
        js     SHORT loc_1_13158
        add    byte ptr [ebx + 69h], dl
        db  07Ah, 065h
        das
        push   ebx
        insd
        popad
        insb
        insb
        das
        dec    ebp
        imul   esi, dword ptr fs:[ebp + 6dh], 72614c2fh
        add    byte ptr gs:[bp + 6fh], cl
        push   esp
loc_1_130FD:
        db  079h, 070h
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_13104:
        inc    edx
        jb     SHORT loc_1_13176
        popad
        arpl   word ptr fs:[ecx + 73h], sp
        db  074h, 041h
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13118:
        inc    edx
        db  072h, 06Fh
        popad
        arpl   word ptr fs:[ecx + 73h], sp
        je     SHORT loc_1_13163
        jb     NEAR PTR loc_1_1318B
        jae    SHORT loc_1_1319B
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1312C:
        push   ebx
        arpl   word ptr [edi + 70h], bp
        dec    ecx
        add    byte ptr fs:[ebp + ebp*2 + 48h], cl
        outsd
        jae    SHORT loc_1_131AE
        inc    esi
        imul   ebp, dword ptr [ebp], 4f442300h
        dec    ebp
        cmp    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_13148:
        and    edx, dword ptr [eax + 52h]
        inc    ebp
loc_1_1314C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13150:
        and    ecx, dword ptr [esi + 4fh]
        inc    esi
        dec    esi
        push   edx
        add    byte ptr [eax], al
loc_1_13158:
        inc    eax
        xor    dword ptr [ecx], eax
        add    byte ptr [2000000h], al
        add    byte ptr [eax], al
loc_1_13163:
        db  000h, 005h, 000h, 000h, 000h, 048h
        xor    dword ptr [ecx], eax
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 50000h], al
loc_1_13176:
        add    byte ptr [eax], al
        push   eax
        xor    dword ptr [ecx], eax
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    eax, 0
        add    byte ptr [eax], al
loc_1_1318B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], ah
        dec    ecx
        dec    esi
loc_1_1319B:
        inc    ebx
        dec    esp
        push   ebp
        inc    esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_131A4:
        and    eax, dword ptr [edx + 45h]
        inc    edi
        dec    ecx
        dec    esi
        pop    edi
        inc    ecx
        dec    esp
        push   esp
loc_1_131AE:
        inc    ebp
        push   edx
        dec    esi
        inc    ecx
        push   esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_131B8:
        and    eax, dword ptr [ebp + 4eh]
        inc    esp
        pop    edi
        inc    ecx
        dec    esp
        push   esp
        inc    ebp
        push   edx
        dec    esi
        inc    ecx
        push   esp
        inc    ebp
        add    byte ptr [eax], al
loc_1_131C8:
        cwde
        xor    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  000h, 0A4h, 031h, 001h, 000h, 010h, 000h
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        mov    eax, 0e000131h
        add    byte ptr [eax], al
        add    byte ptr [1000000h], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [ebx], al
        add    byte ptr [esp + eax*2 + 6fh], bl
        db  073h, 044h
        db  065h, 076h, 069h
        arpl   word ptr [ebp + 73h], sp
        pop    esp
        push   ebp
        dec    esi
        inc    ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13220:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13230:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13234:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1323C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13244:
        add    byte ptr [eax], al
loc_1_13246:
        add    byte ptr [eax], al
loc_1_13248:
        add    byte ptr [eax], al
loc_1_1324A:
        add    byte ptr [eax], al
loc_1_1324C:
        add    byte ptr [eax], al
loc_1_1324E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1325A:
        add    byte ptr [eax], al
loc_1_1325C:
        add    byte ptr [eax], al
loc_1_1325E:
        add    byte ptr [eax], al
loc_1_13260:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13264:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13268:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1326C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13270:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13274:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13278:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1327C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13280:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13284:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13288:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1328C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13290:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13294:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13298:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1329C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132D0:
        add    byte ptr [eax], al
loc_1_132D2:
        add    byte ptr [eax], al
loc_1_132D4:
        add    byte ptr [eax], al
loc_1_132D6:
        add    byte ptr [eax], al
loc_1_132D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_132FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13300:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13304:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13308:
        add    byte ptr [eax], al
loc_1_1330A:
        add    byte ptr [eax], al
loc_1_1330C:
        add    byte ptr [eax], al
loc_1_1330E:
        add    byte ptr [eax], al
loc_1_13310:
        add    byte ptr [eax], al
loc_1_13312:
        add    byte ptr [eax], al
loc_1_13314:
        add    byte ptr [eax], al
loc_1_13316:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1331C:
        add    byte ptr [eax], al
loc_1_1331E:
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
loc_1_13350:
        add    byte ptr [eax], al
loc_1_13352:
        add    byte ptr [eax], al
loc_1_13354:
        add    byte ptr [eax], al
loc_1_13356:
        add    byte ptr [eax], al
loc_1_13358:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1335C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13360:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13364:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13368:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1336C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13370:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13374:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13378:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13380:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13384:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13388:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1338C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13398:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1339C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133AC:
        add    byte ptr [eax], al
loc_1_133AE:
        add    byte ptr [eax], al
loc_1_133B0:
        add    byte ptr [eax], al
loc_1_133B2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133C4:
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
loc_1_133E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_133F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13408:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1340C:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1348C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13490:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13494:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_134A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
