; nwnblink_lcode_01_01.asm -- NWNBLINK object 1 segment _LTEXT  (part 1/2)
.386p
_LTEXT segment dword public use32 'LCODE'

NWNBLINK_Control:
        cmp    eax, 0
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 0F6h, 0FFh, 0FEh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 011h, 000h, 0FFh, 0FFh
        cmp    eax, 4
        je     SHORT loc_1_0027
        cmp    eax, 22h
        je     SHORT loc_1_0032
        clc
        ret
loc_1_0027:
        clc
        ret
loc_1_0029:
        cld
        push   ebx
        call   sub_1_2CE0
        pop    ebx
        ret
loc_1_0032:
        push   edx
        push   ebx
        call   sub_1_7530
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_0041
        stc
loc_1_0041:
        ret
sub_1_0042:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [ebx]
        mov    ebx, 1
        db  0BAh, 029h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 005h, 000h, 014h, 000h
        jae    SHORT loc_1_0068
        mov    eax, 0ffffffffh
        jmp    SHORT loc_1_006F
loc_1_0068:
        mov    ebx, dword ptr [ebp + 8]
        mov    byte ptr [ebx], al
        xor    eax, eax
loc_1_006F:
        pop    ebx
        leave
        ret
sub_1_0072:
        push   ebp
        mov    ebp, esp
        movzx  eax, byte ptr [ebp + 8]
        db  0CDh, 020h, 004h, 000h, 014h, 000h
        leave
        ret
sub_1_0081:
        mov    eax, 1
        db  0FFh, 005h, 014h, 077h, 000h, 000h
        jne    SHORT loc_1_008F
loc_1_008E:
        ret
loc_1_008F:
        db  0FFh, 00Dh, 014h, 077h, 000h, 000h
        xor    eax, eax
        jmp    SHORT loc_1_008E
sub_1_0099:
        mov    eax, 1
        db  0FFh, 005h, 018h, 077h, 000h, 000h
        jne    SHORT loc_1_00A7
loc_1_00A6:
        ret
loc_1_00A7:
        db  0FFh, 00Dh, 018h, 077h, 000h, 000h
        xor    eax, eax
        jmp    SHORT loc_1_00A6
sub_1_00B1:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 038h, 000h, 001h, 000h
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        cld
        ret
sub_1_00E9:
        push   esi
        db  00Fh, 0B7h, 005h, 091h, 079h, 000h, 000h
        mov    edx, 0
        db  0BEh, 024h, 001h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 05Ch, 003h, 000h, 000h
        cld
        pop    esi
        ret
sub_1_010A:
        push   esi
        db  08Bh, 035h, 05Ch, 003h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        db  0C7h, 005h, 05Ch, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        cld
        pop    esi
        ret
loc_1_0124:
        db  083h, 03Dh, 05Ch, 003h, 000h, 000h, 000h
        je     SHORT loc_1_0150
        db  00Fh, 0B7h, 005h, 091h, 079h, 000h, 000h
        mov    edx, 0
        db  0BEh, 024h, 001h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 05Ch, 003h, 000h, 000h
        cld
        call   sub_1_3C50
loc_1_0150:
        ret
sub_1_0151:
        push   ebp
        mov    ebp, esp
        pushad
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebx + 31h], al
        xor    eax, eax
        cmp    eax, dword ptr [ebx + 2ch]
        je     SHORT loc_1_0168
        call   dword ptr [ebx + 2ch]
loc_1_0168:
        popad
        leave
        ret
sub_1_016B:
        push   ebp
        mov    ebp, esp
        mov    eax, 0
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        leave
        ret
sub_1_0187:
        push   ebp
        mov    ebp, esp
        mov    ax, word ptr [ebp + 8]
        xchg   al, ah
        leave
        ret
sub_1_0192:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    al, byte ptr [ebp + 10h]
        mov    ah, al
        mov    cx, ax
        shl    eax, 10h
        mov    ax, cx
        mov    ecx, dword ptr [ebp + 0ch]
        shr    ecx, 2
        mov    edi, dword ptr [ebp + 8]
        cld
        rep stosd
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 3
        rep stosb
        pop    edi
        leave
        ret
sub_1_01BB:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        movzx  ecx, word ptr [ebp + 10h]
        mov    eax, ecx
        cld
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        pop    edi
        pop    esi
        leave
        ret
sub_1_01DD:
        db  0CDh, 020h, 00Dh, 080h, 040h, 000h
        push   ebp
        mov    ebp, esp
        mov    eax, 2
        push   eax
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        leave
        ret
sub_1_01FA:
        db  0CDh, 020h, 00Eh, 080h, 040h, 000h
        ret
sub_1_0201:
        push   ebp
        mov    ebp, esp
        push   ebx
        xor    ebx, ebx
        mov    edx, 5504h
        mov    al, byte ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    ebx
        leave
        ret
loc_1_0219:
        sub    esp, 4
        mov    eax, esp
        push   eax
        push   esi
        push   edi
        push   ecx
        call   sub_1_5FA0
        add    esp, 10h
        pop    ebx
        mov    esi, eax
        ret
sub_1_022E:
        push   ebx
        push   esi
        mov    ebx, 0eh
        mov    edx, 5504h
        db  0BEh, 019h, 002h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    esi
        pop    ebx
        ret
        db  053h, 0BBh, 00Ah, 000h, 000h, 000h, 0CDh, 020h, 001h, 000h, 087h, 004h, 0FCh, 05Bh, 0C3h
sub_1_0258:
        push   ebx
        mov    edx, 5504h
        mov    ebx, 1
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    ebx
        ret
sub_1_026C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, 2
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        or     eax, eax
        je     SHORT loc_1_028A
        xor    ecx, ecx
loc_1_028A:
        movzx  eax, cx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0292:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ebx, 3
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        db  0FFh, 005h, 040h, 078h, 000h, 000h
        cld
        pop    esi
        pop    ebx
        leave
        ret
sub_1_02B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ebx, 4
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    esi
        pop    ebx
        leave
        ret
sub_1_02CA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 9
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    esi
        pop    ebx
        leave
        ret
sub_1_02E2:
        push   ebx
        mov    ebx, 1ah
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    ebx
        ret
sub_1_02F1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ebx, 6
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 000h, 000h, 000h, 04Eh, 057h, 04Eh, 042h
        db  04Ch, 049h, 04Eh, 04Bh, 001h, 060h, 000h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 04Eh, 057h, 04Eh, 042h, 04Ch, 049h, 04Eh, 04Bh, 000h, 04Eh, 057h, 04Eh
        db  042h, 04Ch, 049h, 04Eh, 04Bh, 03Ah, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_0380:
        mov    eax, dword ptr [esp + 4]
        cmp    byte ptr [eax + 0ah], 2ah
        jne    SHORT loc_1_0390
        mov    al, 14h
        ret
        db  08Dh, 049h, 000h
loc_1_0390:
        db  0C7h, 040h, 03Ch, 020h, 009h, 000h, 000h
        push   eax
        call   sub_1_27D0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_03A7
        mov    al, 9
        ret
loc_1_03A7:
        mov    al, 0ffh
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_03B0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        lea    eax, [esi + 1ah]
        push   eax
        call   sub_1_3240
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_040B
        test   byte ptr [eax + 14h], 4
        je     SHORT loc_1_040B
        cmp    byte ptr [esi + 0ah], 2ah
        jne    SHORT loc_1_03F0
        lea    edx, [esi + 34h]
        add    eax, 30h
        mov    dword ptr [edx], eax
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [ecx], edx
        mov    dword ptr [eax + 4], edx
        mov    al, 0ffh
        mov    byte ptr [esi + 32h], 3
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_03F0:
        lea    ecx, [esi + 34h]
        add    eax, 28h
        mov    dword ptr [ecx], eax
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax + 4], ecx
        mov    al, 0ffh
        mov    byte ptr [esi + 32h], 2
        pop    esi
        ret
loc_1_040B:
        mov    al, 15h
        pop    esi
        ret
        db  090h
sub_1_0410:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        sub    bl, bl
        mov    al, byte ptr [esi + 32h]
        cmp    al, 2
        je     SHORT loc_1_042A
        cmp    al, 3
        je     SHORT loc_1_042A
        mov    bl, 26h
        mov    al, bl
        pop    esi
        pop    ebx
        ret
loc_1_042A:
        lea    eax, [esi + 34h]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [esi + 32h], 0
        mov    byte ptr [esi + 1], 0bh
        mov    al, bl
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0450:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        sub    bl, bl
        cmp    byte ptr [esi + 32h], 8
        jne    SHORT loc_1_0476
        lea    eax, [esi + 34h]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [esi + 32h], bl
        mov    byte ptr [esi + 1], 0bh
        mov    al, bl
        pop    esi
        pop    ebx
        ret
loc_1_0476:
        mov    bl, 26h
        mov    al, bl
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_0480:
        push   esi
        push   edi
        sub    al, al
        mov    edi, dword ptr [esp + 0ch]
        xor    edx, edx
        db  08Bh, 00Dh, 07Ch, 079h, 000h, 000h
        mov    dl, byte ptr [edi + 2]
        mov    esi, dword ptr [ecx + edx*4]
        or     esi, esi
        jne    SHORT loc_1_049C
        mov    al, 8
loc_1_049C:
        or     al, al
        jne    SHORT loc_1_0507
        mov    cl, byte ptr [esi + 6]
        cmp    cl, 1
        je     SHORT loc_1_04AD
        cmp    cl, 6
        jne    SHORT loc_1_04AF
loc_1_04AD:
        mov    al, 8
loc_1_04AF:
        or     al, al
        jne    SHORT loc_1_0507
        mov    dword ptr [esi + 70h], edi
        mov    byte ptr [edi + 32h], 0ah
        cmp    byte ptr [esi + 7], 0
        jne    SHORT loc_1_0500
        mov    byte ptr [esi + 6], 6
        db  0A1h, 094h, 079h, 000h, 000h
        mov    word ptr [esi + 28h], ax
        mov    eax, dword ptr [esi + 68h]
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_04F3
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   0
        mov    eax, dword ptr [esi + 68h]
        push   eax
        push   esi
        call   sub_1_0B20
        add    esp, 0ch
        mov    al, 0ffh
        pop    edi
        pop    esi
        ret
loc_1_04F3:
        or     word ptr [esi + 4], 1000h
        mov    al, 0ffh
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_0500:
        or     word ptr [esi + 4], 10h
        mov    al, 0ffh
loc_1_0507:
        pop    edi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0510:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0
        push   0a0h
        call   sub_1_01DD
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_0738
        db  08Ah, 015h, 050h, 079h, 000h, 000h
        db  08Bh, 00Dh, 07Ch, 079h, 000h, 000h
        lea    esp, [esp]
loc_1_0540:
        inc    dl
        cmp    dl, 0ffh
        jne    SHORT loc_1_0549
        mov    dl, 1
loc_1_0549:
        xor    eax, eax
        mov    al, dl
        cmp    dword ptr [ecx + eax*4], 0
        jne    SHORT loc_1_0540
        mov    bl, dl
        and    ebx, 0ffff00ffh
        mov    word ptr [esp + 10h], bx
        mov    eax, ebx
        and    eax, 0ffffh
        lea    edi, [eax*4]
        mov    dword ptr [ecx + edi], esi
        push   0
        push   0a0h
        push   esi
        call   sub_1_0192
        add    esp, 0ch
        mov    word ptr [esi + 4], 1
        db  066h, 0A1h, 050h, 079h, 000h, 000h
        and    ax, 0ff00h
        or     ax, word ptr [esp + 10h]
        db  066h, 0A3h, 050h, 079h, 000h, 000h
        mov    word ptr [esi + 10h], ax
        db  0A1h, 068h, 079h, 000h, 000h
        mov    ecx, 0ah
        mov    word ptr [esi + 30h], ax
        xor    eax, eax
        mov    al, byte ptr [esp + 28h]
        add    eax, 9
        cdq
        idiv   ecx
        mov    byte ptr [esi + 2dh], al
        xor    eax, eax
        mov    al, byte ptr [esp + 2ch]
        add    eax, 9
        cdq
        idiv   ecx
        mov    byte ptr [esi + 2eh], al
        push   0
        push   0a4h
        call   sub_1_01DD
        add    esp, 8
        mov    dword ptr [esi + 68h], eax
        or     eax, eax
        jne    SHORT loc_1_0603
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    dword ptr [edi + eax], 0
        push   0
        push   esi
        call   sub_1_01FA
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_0603:
        mov    word ptr [eax + 0ah], 5504h
        mov    eax, dword ptr [esi + 68h]
        mov    byte ptr [eax + 8], 0
        mov    eax, dword ptr [esi + 68h]
        mov    word ptr [eax + 3ch], 2
        mov    eax, dword ptr [esi + 68h]
        lea    ecx, [eax + 4ch]
        mov    dword ptr [eax + 24h], ecx
        mov    eax, dword ptr [esi + 68h]
        mov    ecx, dword ptr [esp + 18h]
        mov    word ptr [eax + 5ch], 5504h
        lea    edx, [esi + 7ah]
        mov    byte ptr [esi + 7], 0
        mov    word ptr [esi + 20h], 1
        mov    word ptr [esi + 34h], 1
        mov    word ptr [esi + 22h], 1
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 0ch], eax
        mov    edi, dword ptr [esp + 1ch]
        lea    edx, [esi + 8ah]
        mov    eax, dword ptr [edi]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [edx + 0ch], eax
        mov    ebx, dword ptr [esi + 68h]
        mov    edx, dword ptr [esp + 20h]
        lea    edi, [ebx + 52h]
        mov    eax, dword ptr [edx]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [edi + 4], eax
        add    ebx, 4ch
        mov    ax, word ptr [edx + 8]
        mov    word ptr [edi + 8], ax
        mov    edi, dword ptr [esp + 24h]
        mov    eax, dword ptr [esi + 68h]
        mov    ebp, dword ptr [edi]
        mov    dword ptr [eax + 1ch], ebp
        mov    bp, word ptr [edi + 4]
        mov    word ptr [eax + 20h], bp
        add    eax, 1ch
        mov    byte ptr [ebx + 5], 4
        mov    eax, dword ptr [esi + 68h]
        mov    word ptr [eax + 28h], 30h
        mov    eax, dword ptr [esi + 68h]
        or     word ptr [eax + 3ch], 80h
        mov    eax, dword ptr [esi + 68h]
        mov    bx, word ptr [esp + 10h]
        mov    word ptr [eax + 3eh], bx
        lea    ebx, [esi + 3ch]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ebx + 4], eax
        mov    ax, word ptr [edx + 8]
        lea    edx, [esi + 36h]
        mov    word ptr [ebx + 8], ax
        mov    eax, dword ptr [edi]
        mov    dword ptr [edx], eax
        mov    ax, word ptr [edi + 4]
        mov    word ptr [edx + 4], ax
        sub    edx, edx
        mov    eax, dword ptr [ecx + 38h]
        mov    dword ptr [esi], eax
        mov    dword ptr [ecx + 38h], esi
        mov    dword ptr [esi + 48h], ecx
        mov    ecx, 32h
        db  0A1h, 0E8h, 078h, 000h, 000h
        div    ecx
        cmp    eax, 1
        jb     SHORT loc_1_071D
        mov    eax, 1
loc_1_071D:
        mov    byte ptr [esi + 2ch], al
        lea    eax, [esi + 54h]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 58h], eax
        lea    eax, [esi + 5ch]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 60h], eax
        lea    eax, [esi + 4ch]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 50h], eax
loc_1_0738:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0750:
        push   esi
        mov    esi, dword ptr [esp + 8]
        xor    ecx, ecx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    cl, byte ptr [esi + 10h]
        mov    dword ptr [eax + ecx*4], 0
        mov    eax, dword ptr [esi + 48h]
        or     eax, eax
        je     SHORT loc_1_077D
        test   byte ptr [eax + 14h], 40h
        je     SHORT loc_1_077D
        push   esi
        push   eax
        call   sub_1_36E0
        add    esp, 8
loc_1_077D:
        mov    eax, dword ptr [esi + 68h]
        or     eax, eax
        je     SHORT loc_1_078D
        push   eax
        call   sub_1_3820
        add    esp, 4
loc_1_078D:
        mov    word ptr [esi + 4], 0
        push   0
        push   esi
        call   sub_1_01FA
        add    esp, 8
        pop    esi
        ret
sub_1_07A0:
        push   ebx
        push   esi
        push   edi
        mov    bl, byte ptr [esp + 14h]
        mov    edi, ebx
        and    edi, 0ffh
        mov    esi, dword ptr [esp + 10h]
        push   edi
        lea    eax, [esi + 54h]
        push   eax
        call   sub_1_7080
        add    esp, 8
        push   edi
        lea    eax, [esi + 5ch]
        push   eax
        call   sub_1_7080
        add    esp, 8
        push   edi
        lea    eax, [esi + 4ch]
        push   eax
        call   sub_1_7080
        add    esp, 8
        mov    eax, dword ptr [esi + 68h]
        or     eax, eax
        je     SHORT loc_1_080D
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_0800
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 68h]
        push   eax
        call   sub_1_3820
        add    esp, 4
        jmp    SHORT loc_1_0806
        db  08Bh, 0FFh
loc_1_0800:
        mov    word ptr [eax + 3eh], 0ffh
loc_1_0806:
        mov    dword ptr [esi + 68h], 0
loc_1_080D:
        mov    eax, dword ptr [esi + 6ch]
        or     eax, eax
        je     SHORT loc_1_0836
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_0836
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 6ch]
        push   eax
        call   sub_1_3820
        add    esp, 4
        mov    dword ptr [esi + 6ch], 0
loc_1_0836:
        test   byte ptr [esi + 4], 40h
        je     SHORT loc_1_086F
        db  0A1h, 0D4h, 078h, 000h, 000h
        db  03Dh, 0D4h, 078h, 000h, 000h
        je     SHORT loc_1_0869
loc_1_0848:
        mov    ecx, esi
        sub    ecx, eax
        cmp    ecx, -8
        je     SHORT loc_1_0860
        mov    eax, dword ptr [eax]
        db  03Dh, 0D4h, 078h, 000h, 000h
        jne    SHORT loc_1_0848
        jmp    SHORT loc_1_0869
        db  08Dh, 064h, 024h, 000h
loc_1_0860:
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_0869:
        and    word ptr [esi + 4], 0ffbfh
loc_1_086F:
        mov    eax, dword ptr [esi + 48h]
        mov    byte ptr [esi + 7], 0
        mov    edx, dword ptr [eax + 18h]
        mov    ecx, edx
        sub    ecx, eax
        cmp    ecx, 18h
        mov    edi, 0
        je     SHORT loc_1_0893
        lea    edi, [edx - 34h]
        push   edx
        call   sub_1_3B50
        add    esp, 4
loc_1_0893:
        or     edi, edi
        je     SHORT loc_1_08C4
        mov    al, byte ptr [esi + 10h]
        mov    byte ptr [edi + 2], al
        mov    byte ptr [edi + 1], bl
        test   byte ptr [edi], 80h
        je     SHORT loc_1_08C4
        db  0C7h, 047h, 034h, 090h, 078h, 000h, 000h
        add    edi, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [edi + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], edi
        db  089h, 03Dh, 094h, 078h, 000h, 000h
loc_1_08C4:
        mov    ecx, dword ptr [esi + 70h]
        mov    dword ptr [esi + 70h], 0
        or     ecx, ecx
        je     SHORT loc_1_090E
        mov    al, byte ptr [ecx]
        and    eax, 7fh
        cmp    eax, 12h
        jne    SHORT loc_1_090E
        sub    bl, 0ah
        cmp    bl, 1
        cmc
        sbb    al, al
        and    al, 18h
        mov    byte ptr [ecx + 1], al
        test   byte ptr [ecx], 80h
        je     SHORT loc_1_090E
        db  0C7h, 041h, 034h, 090h, 078h, 000h, 000h
        add    ecx, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 094h, 078h, 000h, 000h
loc_1_090E:
        mov    byte ptr [esi + 6], 0
        push   esi
        call   sub_1_0750
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_0920:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        lea    eax, [esi + 1ah]
        push   eax
        call   sub_1_3240
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_09D1
        test   byte ptr [eax + 14h], 4
        je     NEAR PTR loc_1_09D1
        mov    edx, dword ptr [esp + 10h]
        or     edx, edx
        jne    SHORT loc_1_0953
        mov    al, 14h
        jmp    NEAR PTR loc_1_09D3
loc_1_0953:
        mov    cl, byte ptr [edx + 28h]
        and    cl, 86h
        cmp    cl, 4
        je     SHORT loc_1_0962
        mov    al, 14h
        jmp    SHORT loc_1_09D3
loc_1_0962:
        mov    cl, byte ptr [esi + 2bh]
        push   ecx
        mov    cl, byte ptr [esi + 2ah]
        push   ecx
        lea    ecx, [edx + 1ah]
        push   ecx
        lea    ecx, [edx + 10h]
        push   ecx
        push   edx
        push   eax
        call   sub_1_0510
        add    esp, 18h
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_0986
        mov    al, 11h
        jmp    SHORT loc_1_09D3
loc_1_0986:
        mov    dword ptr [edi + 70h], esi
        mov    byte ptr [edi + 6], 1
        call   sub_1_74F0
        mov    word ptr [edi + 9ah], ax
        mov    word ptr [edi + 9ch], 25h
        mov    word ptr [edi + 9eh], 0dh
        db  0A1h, 094h, 079h, 000h, 000h
        mov    word ptr [edi + 28h], ax
        mov    al, byte ptr [edi + 10h]
        mov    byte ptr [esi + 2], al
        mov    byte ptr [esi + 32h], 0ah
        push   0
        mov    eax, dword ptr [edi + 68h]
        push   eax
        push   edi
        call   sub_1_0A10
        add    esp, 0ch
        mov    al, 0ffh
        jmp    SHORT loc_1_09D3
loc_1_09D1:
        mov    al, 15h
loc_1_09D3:
        cmp    al, 0ffh
        je     SHORT loc_1_09FE
        mov    byte ptr [esi + 1], al
        test   byte ptr [esi], 80h
        je     SHORT loc_1_09FE
        db  0C7h, 046h, 034h, 090h, 078h, 000h, 000h
        add    esi, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 094h, 078h, 000h, 000h
loc_1_09FE:
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A10:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        lea    edx, [ecx + 4ch]
        lea    esi, [ecx + 6ah]
        mov    byte ptr [edx + 5], 4
        mov    word ptr [ecx + 28h], 30h
        lea    eax, [edi + 7ah]
        mov    dword ptr [ecx + 2ah], eax
        mov    word ptr [ecx + 2eh], 26h
        mov    word ptr [ecx + 22h], 2
        mov    al, byte ptr [edi + 10h]
        and    eax, 0ffff00ffh
        mov    word ptr [ecx + 3eh], ax
        add    edx, 6
        mov    eax, dword ptr [edi + 3ch]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [edi + 40h]
        mov    dword ptr [edx + 4], eax
        mov    ax, word ptr [edi + 44h]
        lea    ebx, [edi + 3ch]
        mov    word ptr [edx + 8], ax
        lea    eax, [edi + 36h]
        lea    ebx, [ecx + 1ch]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ebx], edx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebx + 4], ax
        db  0BAh, 040h, 010h, 000h, 000h
        mov    ax, word ptr [edi + 10h]
        mov    word ptr [esi + 2], ax
        mov    al, byte ptr [esp + 18h]
        or     al, al
        jne    SHORT loc_1_0A8E
        db  0BAh, 0E0h, 00Ch, 000h, 000h
loc_1_0A8E:
        mov    dword ptr [ecx + 4], edx
        or     al, al
        je     SHORT loc_1_0AA0
        mov    dx, word ptr [edi + 12h]
        mov    word ptr [esi + 4], dx
        jmp    SHORT loc_1_0AA6
        db  090h
loc_1_0AA0:
        mov    word ptr [esi + 4], 0ffffh
loc_1_0AA6:
        mov    byte ptr [esi], 0
        db  080h, 03Dh, 08Eh, 078h, 000h, 000h, 000h
        je     SHORT loc_1_0ABF
        test   byte ptr [edi + 5], 80h
        jne    SHORT loc_1_0ABC
        or     al, al
        jne    SHORT loc_1_0ABF
loc_1_0ABC:
        mov    byte ptr [esi], 1
loc_1_0ABF:
        cmp    al, 1
        sbb    dl, dl
        and    dl, 0c0h
        add    dl, 80h
        or     byte ptr [esi], dl
        cmp    al, 1
        mov    word ptr [esi + 6], 0
        sbb    eax, eax
        inc    eax
        mov    word ptr [esi + 0eh], ax
        mov    byte ptr [esi + 1], 6
        mov    word ptr [esi + 0ch], 26h
        mov    word ptr [esi + 8], 26h
        add    ecx, 44h
        mov    word ptr [esi + 0ah], 0
        mov    word ptr [esi + 10h], 0
        db  0C7h, 001h, 0A0h, 078h, 000h, 000h
        db  0A1h, 0A4h, 078h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 0A4h, 078h, 000h, 000h
        pop    edi
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 0A4h, 078h, 000h, 000h
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0B20:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 10h]
        mov    cl, byte ptr [esp + 14h]
        lea    esi, [edx + 6ah]
        cmp    cl, 1
        sbb    al, al
        add    al, 8
        mov    edi, dword ptr [esp + 0ch]
        mov    byte ptr [esi + 1], al
        mov    ax, word ptr [edi + 10h]
        mov    word ptr [esi + 2], ax
        cmp    cl, 1
        mov    ax, word ptr [edi + 12h]
        mov    word ptr [esi + 4], ax
        mov    ax, word ptr [edi + 14h]
        mov    word ptr [esi + 6], ax
        mov    ax, word ptr [edi + 1ch]
        mov    word ptr [esi + 0eh], ax
        sbb    al, al
        and    al, 0c0h
        add    al, 80h
        mov    byte ptr [esi], al
        mov    word ptr [esi + 0ch], 0
        mov    word ptr [esi + 0ah], 0
        mov    word ptr [esi + 8], 0
        db  0B8h, 0C0h, 00Bh, 000h, 000h
        or     cl, cl
        jne    SHORT loc_1_0B86
        db  0B8h, 060h, 00Ch, 000h, 000h
loc_1_0B86:
        mov    dword ptr [edx + 4], eax
        mov    word ptr [edx + 22h], 1
        db  0C7h, 042h, 044h, 0A0h, 078h, 000h, 000h
        add    edx, 44h
        db  0A1h, 0A4h, 078h, 000h, 000h
        mov    dword ptr [edx + 4], eax
        db  0A1h, 0A4h, 078h, 000h, 000h
        pop    edi
        mov    dword ptr [eax], edx
        db  089h, 015h, 0A4h, 078h, 000h, 000h
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0BC0:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        xor    edx, edx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    dx, word ptr [ecx + 3eh]
        mov    esi, dword ptr [eax + edx*4]
        or     esi, esi
        je     SHORT loc_1_0BF2
        push   ecx
        call   sub_1_3820
        add    esp, 4
        mov    dword ptr [esi + 68h], 0
        push   0ah
        push   esi
        call   sub_1_07A0
        add    esp, 8
loc_1_0BF2:
        call   sub_1_2E80
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0C00:
        push   esi
        mov    edx, dword ptr [esp + 8]
        xor    ecx, ecx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    cx, word ptr [edx + 3eh]
        mov    esi, dword ptr [eax + ecx*4]
        or     esi, esi
        je     SHORT loc_1_0C50
        mov    ax, word ptr [esi + 28h]
        dec    ax
        mov    word ptr [esi + 28h], ax
        je     SHORT loc_1_0C31
        push   0
        push   edx
        push   esi
        call   sub_1_0B20
        add    esp, 0ch
        pop    esi
        ret
loc_1_0C31:
        mov    dword ptr [esi + 68h], 0
        push   edx
        call   sub_1_3820
        add    esp, 4
        push   18h
        push   esi
        call   sub_1_07A0
        add    esp, 8
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_0C50:
        push   edx
        call   sub_1_3820
        add    esp, 4
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_0C60:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        xor    edx, edx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    dx, word ptr [ecx + 3eh]
        mov    esi, dword ptr [eax + edx*4]
        or     esi, esi
        je     SHORT loc_1_0CC4
        cmp    byte ptr [ecx + 9], 0
        je     SHORT loc_1_0CA0
        mov    dword ptr [esi + 68h], 0
        push   ecx
        call   sub_1_3820
        add    esp, 4
        push   18h
        push   esi
        call   sub_1_07A0
        add    esp, 8
        call   sub_1_2E80
        pop    esi
        ret
        db  090h
loc_1_0CA0:
        mov    eax, dword ptr [esi + 68h]
        db  0C7h, 040h, 004h, 000h, 00Ch, 000h, 000h
        xor    eax, eax
        mov    ax, word ptr [esi + 30h]
        push   eax
        mov    eax, dword ptr [esi + 68h]
        push   eax
        call   sub_1_3BE0
        add    esp, 8
        call   sub_1_2E80
        pop    esi
        ret
loc_1_0CC4:
        push   ecx
        call   sub_1_3820
        add    esp, 4
        call   sub_1_2E80
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0CE0:
        mov    edx, dword ptr [esp + 4]
        xor    ecx, ecx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    cx, word ptr [edx + 3eh]
        mov    eax, dword ptr [eax + ecx*4]
        or     eax, eax
        je     SHORT loc_1_0D40
        cmp    byte ptr [eax + 6], 1
        jne    SHORT loc_1_0D20
        db  0C7h, 042h, 004h, 050h, 00Dh, 000h, 000h
        mov    ax, word ptr [eax + 30h]
        and    eax, 0ffffh
        push   eax
        push   edx
        call   sub_1_3BE0
        add    esp, 8
        jmp    NEAR PTR sub_1_2E80
        db  005h, 000h, 000h, 000h, 000h
loc_1_0D20:
        mov    word ptr [edx + 22h], 1
        mov    word ptr [edx + 28h], 30h
        mov    ecx, dword ptr [eax + 68h]
        mov    ax, word ptr [eax + 12h]
        mov    word ptr [ecx + 6eh], ax
        push   edx
        call   sub_1_1040
        add    esp, 4
loc_1_0D40:
        jmp    NEAR PTR sub_1_2E80
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D50:
        mov    edx, dword ptr [esp + 4]
        xor    ecx, ecx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    cx, word ptr [edx + 3eh]
        mov    ecx, dword ptr [eax + ecx*4]
        or     ecx, ecx
        je     NEAR PTR loc_1_0DF6
        cmp    byte ptr [ecx + 6], 1
        jne    SHORT loc_1_0DD6
        mov    ax, word ptr [ecx + 28h]
        or     ax, ax
        je     SHORT loc_1_0D90
        dec    ax
        mov    word ptr [ecx + 28h], ax
        push   0
        push   edx
        push   ecx
        call   sub_1_0A10
        add    esp, 0ch
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_0D90:
        mov    eax, dword ptr [ecx + 70h]
        mov    byte ptr [eax + 1], 12h
        mov    eax, dword ptr [ecx + 70h]
        mov    byte ptr [eax + 32h], 0
        mov    edx, dword ptr [ecx + 70h]
        test   byte ptr [edx], 80h
        je     SHORT loc_1_0DC5
        db  0C7h, 042h, 034h, 090h, 078h, 000h, 000h
        add    edx, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [edx + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], edx
        db  089h, 015h, 094h, 078h, 000h, 000h
loc_1_0DC5:
        mov    dword ptr [ecx + 70h], 0
        push   ecx
        call   sub_1_0750
        add    esp, 4
        ret
loc_1_0DD6:
        mov    word ptr [edx + 22h], 1
        mov    word ptr [edx + 28h], 30h
        mov    eax, dword ptr [ecx + 68h]
        mov    cx, word ptr [ecx + 12h]
        mov    word ptr [eax + 6eh], cx
        push   edx
        call   sub_1_1040
        add    esp, 4
loc_1_0DF6:
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0E00:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
        lea    esi, [ecx + 6ah]
        mov    ax, word ptr [edx + 14h]
        mov    word ptr [esi + 6], ax
        mov    ax, word ptr [edx + 1ch]
        mov    word ptr [esi + 0eh], ax
        mov    byte ptr [esi], 80h
        mov    ax, word ptr [edx + 4]
        test   ah, 8
        je     SHORT loc_1_0E33
        and    ax, 0f7ffh
        mov    word ptr [edx + 4], ax
        mov    byte ptr [esi], 88h
loc_1_0E33:
        db  0A1h, 064h, 078h, 000h, 000h
        cmp    dword ptr [edx + 74h], eax
        jae    SHORT loc_1_0E40
        or     byte ptr [esi], 40h
loc_1_0E40:
        test   byte ptr [edx + 5], 80h
        je     SHORT loc_1_0E52
        mov    ax, word ptr [edx + 22h]
        add    ax, word ptr [edx + 1ch]
        dec    ax
        jmp    SHORT loc_1_0E56
loc_1_0E52:
        mov    ax, word ptr [edx + 1eh]
loc_1_0E56:
        mov    word ptr [esi + 10h], ax
        mov    word ptr [esi + 0ch], 0
        mov    word ptr [esi + 0ah], 0
        mov    word ptr [esi + 8], 0
        add    ecx, 44h
        and    word ptr [edx + 4], 0dfffh
        mov    word ptr [edx + 2ah], 0
        db  0C7h, 041h, 0C0h, 040h, 010h, 000h, 000h
        mov    word ptr [ecx - 22h], 1
        db  0C7h, 001h, 0A0h, 078h, 000h, 000h
        db  0A1h, 0A4h, 078h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 0A4h, 078h, 000h, 000h
        pop    esi
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 0A4h, 078h, 000h, 000h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0EB0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 74h]
        add    ecx, 4b0h
        db  03Bh, 00Dh, 064h, 078h, 000h, 000h
        jae    SHORT loc_1_0EEB
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        call   sub_1_3820
        add    esp, 4
        mov    dword ptr [esi + 68h], 0
        mov    byte ptr [esi + 6], 6
        push   18h
        push   esi
        call   sub_1_07A0
        add    esp, 8
        pop    esi
        ret
loc_1_0EEB:
        mov    eax, dword ptr [esp + 0ch]
        push   esi
        push   eax
        call   sub_1_0E00
        add    esp, 8
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_0F00:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        xor    ecx, ecx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    cx, word ptr [esi + 3eh]
        mov    edi, dword ptr [eax + ecx*4]
        or     edi, edi
        jne    SHORT loc_1_0F26
        push   esi
        call   sub_1_3820
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0F26:
        cmp    byte ptr [edi + 6], 3
        jne    SHORT loc_1_0FA6
        cmp    byte ptr [edi + 2dh], 0
        je     SHORT loc_1_0FA6
        mov    al, byte ptr [edi + 2fh]
        or     al, al
        je     SHORT loc_1_0FA6
        dec    al
        mov    byte ptr [edi + 2fh], al
        jne    SHORT loc_1_0FA6
        mov    ecx, dword ptr [edi + 54h]
        mov    eax, ecx
        sub    eax, edi
        cmp    eax, 54h
        je     SHORT loc_1_0FA6
        lea    ebx, [ecx - 34h]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [ebx + 32h], 0
        mov    byte ptr [ebx + 1], 5
        test   byte ptr [ebx], 80h
        je     SHORT loc_1_0F84
        db  0C7h, 043h, 034h, 090h, 078h, 000h, 000h
        add    ebx, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [ebx + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], ebx
        db  089h, 01Dh, 094h, 078h, 000h, 000h
loc_1_0F84:
        cmp    word ptr [edi + 78h], 0
        je     SHORT loc_1_0FA0
        mov    byte ptr [edi + 6], 6
        push   0
        push   esi
        push   edi
        call   sub_1_0B20
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_0FA0:
        mov    al, byte ptr [edi + 2dh]
        mov    byte ptr [edi + 2fh], al
loc_1_0FA6:
        mov    al, byte ptr [edi + 2ch]
        dec    al
        mov    byte ptr [edi + 2ch], al
        jne    SHORT loc_1_0FE9
        test   byte ptr [edi + 5], 80h
        je     SHORT loc_1_0FC0
        mov    byte ptr [edi + 2ch], 6
        jmp    SHORT loc_1_0FDB
        db  08Dh, 064h, 024h, 000h
loc_1_0FC0:
        mov    ecx, 32h
        db  0A1h, 0E8h, 078h, 000h, 000h
        sub    edx, edx
        div    ecx
        cmp    eax, 1
        jb     SHORT loc_1_0FD8
        mov    eax, 1
loc_1_0FD8:
        mov    byte ptr [edi + 2ch], al
loc_1_0FDB:
        push   esi
        push   edi
        call   sub_1_0EB0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0FE9:
        push   esi
        call   sub_1_1040
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1000:
        mov    edx, dword ptr [esp + 4]
        xor    ecx, ecx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    cx, word ptr [edx + 3eh]
        mov    eax, dword ptr [eax + ecx*4]
        or     eax, eax
        jne    SHORT loc_1_1020
        push   edx
        call   sub_1_3820
        add    esp, 4
        ret
loc_1_1020:
        and    word ptr [eax + 4], 0dfffh
        push   eax
        push   edx
        call   sub_1_0E00
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1040:
        mov    edx, dword ptr [esp + 4]
        xor    ecx, ecx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    cx, word ptr [edx + 3eh]
        mov    ecx, dword ptr [eax + ecx*4]
        or     ecx, ecx
        je     NEAR PTR loc_1_1109
        mov    ax, word ptr [ecx + 4]
        test   al, 2
        je     SHORT loc_1_1079
        and    ax, 0fffdh
        mov    word ptr [ecx + 4], ax
        push   ecx
        push   edx
        call   sub_1_0E00
        add    esp, 8
        jmp    NEAR PTR sub_1_2E80
loc_1_1079:
        test   ah, 10h
        je     SHORT loc_1_1097
        and    ax, 0efffh
        mov    word ptr [ecx + 4], ax
        push   0
        push   edx
        push   ecx
        call   sub_1_0B20
        add    esp, 0ch
        jmp    NEAR PTR sub_1_2E80
loc_1_1097:
        test   ah, 1
        je     SHORT loc_1_10B5
        and    ax, 0feffh
        mov    word ptr [ecx + 4], ax
        push   1
        push   edx
        push   ecx
        call   sub_1_0A10
        add    esp, 0ch
        jmp    NEAR PTR sub_1_2E80
loc_1_10B5:
        test   al, 8
        je     SHORT loc_1_10D2
        and    ax, 0fff7h
        mov    word ptr [ecx + 4], ax
        push   1
        push   edx
        push   ecx
        call   sub_1_0B20
        add    esp, 0ch
        jmp    NEAR PTR sub_1_2E80
loc_1_10D2:
        test   ah, 20h
        je     SHORT loc_1_10F2
        db  0C7h, 042h, 004h, 000h, 010h, 000h, 000h
        db  0A1h, 03Ch, 079h, 000h, 000h
        push   eax
        push   edx
        call   sub_1_3BE0
        add    esp, 8
        jmp    NEAR PTR sub_1_2E80
loc_1_10F2:
        db  0C7h, 042h, 004h, 000h, 00Fh, 000h, 000h
        push   32h
        push   edx
        call   sub_1_3BE0
        add    esp, 8
        jmp    NEAR PTR sub_1_2E80
loc_1_1109:
        push   edx
        call   sub_1_3820
        add    esp, 4
        jmp    NEAR PTR sub_1_2E80
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1120:
        sub    esp, 8
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [esp + 14h]
        lea    edi, [eax*4]
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    esi, dword ptr [edi + eax]
        or     esi, esi
        jne    SHORT loc_1_1149
        mov    eax, 0ffh
        pop    edi
        pop    esi
        add    esp, 8
        ret
loc_1_1149:
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esi + 3ch]
        push   eax
        call   sub_1_026C
        add    esp, 8
        db  08Bh, 00Dh, 07Ch, 079h, 000h, 000h
        cmp    dword ptr [edi + ecx], esi
        je     SHORT loc_1_1170
        mov    eax, 0ffh
        pop    edi
        pop    esi
        add    esp, 8
        ret
loc_1_1170:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 32h]
        cmp    ecx, eax
        jae    SHORT loc_1_1180
        inc    ecx
        sub    eax, ecx
        jmp    SHORT loc_1_1182
        db  090h
loc_1_1180:
        sub    eax, eax
loc_1_1182:
        cmp    eax, 1eh
        jbe    SHORT loc_1_118C
        mov    eax, 1eh
loc_1_118C:
        add    word ptr [esi + 30h], ax
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_11A0:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 24h]
        add    ecx, 1eh
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [esp + 18h], ecx
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        push   eax
        call   sub_1_0187
        mov    ecx, dword ptr [esp + 1ch]
        add    esp, 4
        mov    dx, word ptr [ecx + 8]
        and    eax, 0ffffh
        mov    ecx, edx
        and    ecx, 0ffffh
        sub    eax, ecx
        cmp    eax, 30h
        jne    NEAR PTR loc_1_14AA
        cmp    dx, 26h
        jb     NEAR PTR loc_1_14AA
        mov    eax, dword ptr [esp + 24h]
        add    eax, 30h
        mov    dword ptr [esp + 10h], eax
        add    eax, 10h
        push   eax
        call   sub_1_3240
        mov    dword ptr [esp + 18h], eax
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_14AA
        test   byte ptr [eax + 14h], 4
        je     NEAR PTR loc_1_14AA
        mov    ebx, dword ptr [eax + 38h]
        or     ebx, ebx
        je     SHORT loc_1_1257
        mov    ecx, dword ptr [esp + 18h]
        mov    ax, word ptr [ecx + 2]
        mov    edi, edi
loc_1_1230:
        cmp    word ptr [ebx + 12h], ax
        jne    SHORT loc_1_1249
        lea    esi, [ebx + 8ah]
        mov    ecx, 4
        mov    edi, dword ptr [esp + 10h]
        repe cmpsd
        je     SHORT loc_1_124F
loc_1_1249:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_1_1230
loc_1_124F:
        or     ebx, ebx
        jne    NEAR PTR loc_1_1483
loc_1_1257:
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, dword ptr [eax + 28h]
        sub    eax, ebp
        cmp    eax, -28h
        je     SHORT loc_1_1293
loc_1_1265:
        lea    esi, [ebp - 2ah]
        lea    ebx, [ebp - 34h]
        mov    ecx, 4
        mov    edi, dword ptr [esp + 10h]
        repe cmpsd
        je     SHORT loc_1_1288
        mov    ebp, dword ptr [ebp]
        mov    eax, dword ptr [esp + 14h]
        sub    eax, ebp
        cmp    eax, -28h
        jne    SHORT loc_1_1265
        jmp    SHORT loc_1_1297
loc_1_1288:
        push   ebp
        call   sub_1_3B50
        add    esp, 4
        jmp    SHORT loc_1_1297
loc_1_1293:
        mov    ebx, dword ptr [esp + 1ch]
loc_1_1297:
        mov    eax, dword ptr [esp + 14h]
        sub    eax, ebp
        cmp    eax, -28h
        jne    SHORT loc_1_12C5
        mov    eax, dword ptr [esp + 14h]
        mov    ebx, 0
        mov    ecx, dword ptr [eax + 30h]
        mov    eax, ecx
        sub    eax, dword ptr [esp + 14h]
        cmp    eax, 30h
        je     SHORT loc_1_12C5
        lea    ebx, [ecx - 34h]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
loc_1_12C5:
        or     ebx, ebx
        je     NEAR PTR loc_1_14AA
        mov    al, byte ptr [ebx + 2bh]
        push   eax
        mov    al, byte ptr [ebx + 2ah]
        push   eax
        mov    byte ptr [ebx + 32h], 0
        mov    eax, dword ptr [esp + 30h]
        push   eax
        mov    eax, dword ptr [esp + 30h]
        add    eax, 12h
        push   eax
        mov    eax, dword ptr [esp + 20h]
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   eax
        call   sub_1_0510
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_1452
        mov    ecx, dword ptr [esp + 18h]
        test   byte ptr [ecx], 1
        je     SHORT loc_1_1330
        db  080h, 03Dh, 08Eh, 078h, 000h, 000h, 000h
        je     SHORT loc_1_1330
        or     word ptr [esi + 4], 8000h
        db  0A1h, 064h, 078h, 000h, 000h
        add    eax, 384h
        mov    dword ptr [esi + 74h], eax
        jmp    SHORT loc_1_1341
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1330:
        db  0A1h, 0E8h, 078h, 000h, 000h
        lea    ecx, [eax + eax*2]
        db  003h, 00Dh, 064h, 078h, 000h, 000h
        mov    dword ptr [esi + 74h], ecx
loc_1_1341:
        mov    eax, dword ptr [esp + 24h]
        mov    al, byte ptr [eax + 4]
        and    ax, 0fh
        mov    word ptr [esi + 32h], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 10h]
        push   eax
        db  068h, 020h, 011h, 000h, 000h
        call   sub_1_016B
        add    esp, 8
        mov    al, byte ptr [esi + 10h]
        mov    byte ptr [ebx + 2], al
        mov    edx, dword ptr [esp + 10h]
        lea    ecx, [ebx + 0ah]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, dword ptr [esp + 18h]
        mov    ax, word ptr [ecx + 2]
        mov    word ptr [esi + 12h], ax
        mov    word ptr [esi + 1ch], 1
        mov    byte ptr [esi + 6], 3
        call   sub_1_74F0
        mov    eax, dword ptr [esp + 10h]
        db  066h, 08Bh, 00Dh, 0E4h, 078h, 000h, 000h
        mov    ax, word ptr [eax + 20h]
        cmp    ax, cx
        jb     SHORT loc_1_13B9
        mov    ax, cx
loc_1_13B9:
        mov    word ptr [esi + 9ah], ax
        db  080h, 03Dh, 014h, 078h, 000h, 000h, 000h
        je     SHORT loc_1_13DF
        cmp    word ptr [esi + 32h], 0
        je     SHORT loc_1_13DF
        cmp    ax, 400h
        jbe    SHORT loc_1_13DF
        mov    word ptr [esi + 9ah], 400h
loc_1_13DF:
        mov    ax, word ptr [esi + 32h]
        lea    eax, [eax*2 + 4]
        mov    word ptr [esi + 22h], ax
        db  080h, 03Dh, 060h, 079h, 000h, 000h, 000h
        jne    SHORT loc_1_140C
        mov    ecx, eax
        and    ecx, 0ffffh
        db  0A1h, 04Ch, 079h, 000h, 000h
        cmp    ecx, eax
        jbe    SHORT loc_1_140C
        mov    word ptr [esi + 22h], ax
loc_1_140C:
        xor    eax, eax
        mov    ax, word ptr [esi + 22h]
        db  08Ah, 080h, 0B0h, 075h, 000h, 000h
        and    eax, 0ffff00ffh
        mov    word ptr [esi + 34h], ax
        mov    eax, dword ptr [esp + 10h]
        push   1
        mov    ax, word ptr [eax + 22h]
        mov    word ptr [esi + 9ch], ax
        mov    eax, dword ptr [esp + 14h]
        mov    ax, word ptr [eax + 24h]
        mov    word ptr [esi + 9eh], ax
        mov    eax, dword ptr [esi + 68h]
        push   eax
        push   esi
        call   sub_1_0A10
        add    esp, 0ch
        sub    al, al
        jmp    SHORT loc_1_1454
loc_1_1452:
        mov    al, 11h
loc_1_1454:
        mov    byte ptr [ebx + 1], al
        test   byte ptr [ebx], 80h
        je     SHORT loc_1_14AA
        db  0C7h, 043h, 034h, 090h, 078h, 000h, 000h
        add    ebx, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [ebx + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        pop    ebp
        mov    dword ptr [eax], ebx
        db  089h, 01Dh, 094h, 078h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_1483:
        cmp    byte ptr [ebx + 6], 3
        jne    SHORT loc_1_14AA
        mov    eax, dword ptr [ebx + 68h]
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_14AA
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   1
        mov    eax, dword ptr [ebx + 68h]
        push   eax
        push   ebx
        call   sub_1_0A10
        add    esp, 0ch
loc_1_14AA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_14C0:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 14h]
        xor    eax, eax
        lea    edi, [ebx + 1eh]
        mov    ax, word ptr [ebx + 2]
        push   eax
        call   sub_1_0187
        add    esp, 4
        cmp    ax, 56h
        jb     SHORT loc_1_1500
        lea    eax, [ebx + 30h]
        mov    esi, dword ptr [esp + 10h]
        mov    ax, word ptr [eax + 20h]
        cmp    word ptr [esi + 9ah], ax
        jbe    SHORT loc_1_1504
        mov    word ptr [esi + 9ah], ax
        jmp    SHORT loc_1_1504
        db  08Dh, 064h, 024h, 000h
loc_1_1500:
        mov    esi, dword ptr [esp + 10h]
loc_1_1504:
        cmp    word ptr [edi + 6], 0
        jne    NEAR PTR loc_1_1642
        cmp    word ptr [edi + 0eh], 1
        jne    NEAR PTR loc_1_1642
        test   byte ptr [edi], 1
        je     SHORT loc_1_1534
        or     word ptr [esi + 4], 8000h
        db  0A1h, 064h, 078h, 000h, 000h
        add    eax, 384h
        mov    dword ptr [esi + 74h], eax
        jmp    SHORT loc_1_1545
loc_1_1534:
        db  0A1h, 0E8h, 078h, 000h, 000h
        lea    ecx, [eax + eax*2]
        db  003h, 00Dh, 064h, 078h, 000h, 000h
        mov    dword ptr [esi + 74h], ecx
loc_1_1545:
        mov    al, byte ptr [ebx + 4]
        and    ax, 0fh
        mov    word ptr [esi + 32h], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 10h]
        push   eax
        db  068h, 020h, 011h, 000h, 000h
        call   sub_1_016B
        add    esp, 8
        mov    ax, word ptr [edi + 2]
        mov    word ptr [esi + 12h], ax
        mov    word ptr [esi + 24h], 1
        mov    word ptr [esi + 14h], 1
        mov    al, byte ptr [ebx + 4]
        and    ax, 0fh
        lea    eax, [eax*2 + 4]
        mov    word ptr [esi + 22h], ax
        db  080h, 03Dh, 060h, 079h, 000h, 000h, 000h
        jne    SHORT loc_1_15A8
        mov    ecx, eax
        and    ecx, 0ffffh
        db  0A1h, 04Ch, 079h, 000h, 000h
        cmp    ecx, eax
        jbe    SHORT loc_1_15A8
        mov    word ptr [esi + 22h], ax
loc_1_15A8:
        mov    ax, word ptr [esi + 22h]
        mov    word ptr [esi + 20h], ax
        and    eax, 0ffffh
        db  08Ah, 080h, 0B0h, 075h, 000h, 000h
        and    eax, 0ffff00ffh
        mov    word ptr [esi + 34h], ax
        mov    eax, dword ptr [esi + 68h]
        mov    byte ptr [esi + 6], 3
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_160C
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 68h]
        mov    word ptr [eax + 22h], 1
        mov    eax, dword ptr [esi + 68h]
        push   32h
        mov    word ptr [eax + 28h], 30h
        mov    ecx, dword ptr [esi + 68h]
        mov    ax, word ptr [edi + 2]
        mov    word ptr [ecx + 6eh], ax
        db  0C7h, 041h, 004h, 000h, 00Fh, 000h, 000h
        mov    eax, dword ptr [esi + 68h]
        push   eax
        call   sub_1_3BE0
        add    esp, 8
loc_1_160C:
        mov    ecx, dword ptr [esi + 70h]
        mov    dword ptr [esi + 70h], 0
        mov    byte ptr [ecx + 32h], 0
        mov    byte ptr [ecx + 1], 0
        test   byte ptr [ecx], 80h
        je     SHORT loc_1_1642
        db  0C7h, 041h, 034h, 090h, 078h, 000h, 000h
        add    ecx, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 094h, 078h, 000h, 000h
loc_1_1642:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1650:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 064h, 078h, 000h, 000h
        mov    esi, dword ptr [esp + 14h]
        add    eax, 384h
        mov    dword ptr [esi + 74h], eax
        cmp    byte ptr [esi + 7], 0
        mov    edi, dword ptr [esp + 18h]
        jne    SHORT loc_1_1690
        mov    ax, word ptr [edi + 0eh]
        cmp    word ptr [esi + 14h], ax
        jne    NEAR PTR loc_1_1915
        mov    cx, word ptr [edi + 10h]
        sub    cx, ax
        inc    cx
        mov    word ptr [esi + 20h], cx
        jmp    NEAR PTR loc_1_1915
        db  090h
loc_1_1690:
        mov    cx, word ptr [edi + 0eh]
        mov    dx, cx
        sub    dx, word ptr [esi + 24h]
        je     NEAR PTR loc_1_1915
        cmp    dx, 8
        ja     NEAR PTR loc_1_1915
        mov    ax, dx
        dec    dx
        or     ax, ax
        je     SHORT loc_1_16ED
loc_1_16B5:
        mov    ax, word ptr [esi + 26h]
        xor    ebx, ebx
        mov    bx, word ptr [esi + 16h]
        mov    ebp, eax
        and    ebp, 0ffffh
        sub    ebx, ebp
        xor    ebp, ebp
        mov    bp, word ptr [esi + 9ah]
        cmp    ebx, ebp
        jl     SHORT loc_1_16D8
        mov    ebx, ebp
loc_1_16D8:
        add    bx, ax
        inc    word ptr [esi + 24h]
        mov    word ptr [esi + 26h], bx
        mov    ax, dx
        dec    dx
        or     ax, ax
        jne    SHORT loc_1_16B5
loc_1_16ED:
        mov    ax, word ptr [esi + 24h]
        mov    dx, word ptr [esi + 14h]
        sub    dx, ax
        cmp    dx, 8
        jbe    SHORT loc_1_170A
        mov    word ptr [esi + 14h], ax
        mov    ax, word ptr [esi + 26h]
        mov    word ptr [esi + 18h], ax
loc_1_170A:
        mov    ax, word ptr [edi + 10h]
        sub    ax, cx
        inc    ax
        cmp    ax, 8
        jb     SHORT loc_1_171D
        mov    ax, 8
loc_1_171D:
        mov    bx, word ptr [esi + 20h]
        cmp    ax, bx
        jbe    SHORT loc_1_173E
        and    eax, 0ffffh
        mov    ecx, ebx
        and    ecx, 0ffffh
        sub    eax, ecx
        inc    eax
        cdq
        sub    eax, edx
        sar    eax, 1
        add    ax, bx
loc_1_173E:
        mov    word ptr [esi + 20h], ax
        db  0A1h, 094h, 079h, 000h, 000h
        mov    word ptr [esi + 28h], ax
        mov    ax, word ptr [esi + 14h]
        cmp    word ptr [esi + 24h], ax
        jne    SHORT loc_1_1778
        mov    eax, dword ptr [esi + 6ch]
        or     eax, eax
        je     SHORT loc_1_1778
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 6ch]
        push   eax
        call   sub_1_3820
        add    esp, 4
        mov    dword ptr [esi + 6ch], 0
loc_1_1778:
        mov    ax, word ptr [esi + 16h]
        cmp    word ptr [esi + 26h], ax
        jb     NEAR PTR loc_1_18CC
        mov    word ptr [esi + 26h], 0
        mov    ecx, dword ptr [esi + 5ch]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 5ch
        mov    ebx, 0
        je     SHORT loc_1_17A9
        lea    ebx, [ecx - 34h]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
loc_1_17A9:
        mov    byte ptr [ebx + 32h], 0
        mov    al, byte ptr [ebx]
        and    eax, 7fh
        cmp    eax, 48h
        jne    SHORT loc_1_17F0
        mov    ax, word ptr [ebx + 0ah]
        mov    word ptr [ebx + 8], ax
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ebx + 4], eax
        db  0C7h, 043h, 03Ch, 010h, 047h, 000h, 000h
        db  0C7h, 043h, 034h, 084h, 078h, 000h, 000h
        add    ebx, 34h
        db  0A1h, 088h, 078h, 000h, 000h
        mov    dword ptr [ebx + 4], eax
        db  0A1h, 088h, 078h, 000h, 000h
        mov    dword ptr [eax], ebx
        db  089h, 01Dh, 088h, 078h, 000h, 000h
        jmp    SHORT loc_1_1818
        db  08Dh, 049h, 000h
loc_1_17F0:
        mov    byte ptr [ebx + 1], 0
        test   byte ptr [ebx], 80h
        je     SHORT loc_1_1818
        db  0C7h, 043h, 034h, 090h, 078h, 000h, 000h
        add    ebx, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [ebx + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], ebx
        db  089h, 01Dh, 094h, 078h, 000h, 000h
loc_1_1818:
        test   byte ptr [esi + 4], 10h
        je     SHORT loc_1_188B
        mov    byte ptr [esi + 6], 6
        mov    eax, dword ptr [esi + 6ch]
        or     eax, eax
        je     SHORT loc_1_184B
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_184B
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 6ch]
        push   eax
        call   sub_1_3820
        add    esp, 4
        mov    dword ptr [esi + 6ch], 0
loc_1_184B:
        mov    eax, dword ptr [esi + 68h]
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_1880
        push   eax
        call   sub_1_3C30
        add    esp, 4
        db  0A1h, 094h, 079h, 000h, 000h
        mov    word ptr [esi + 28h], ax
        push   0
        mov    eax, dword ptr [esi + 68h]
        push   eax
        push   esi
        call   sub_1_0B20
        add    esp, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1880:
        or     word ptr [esi + 4], 1000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_188B:
        mov    edx, dword ptr [esi + 5ch]
        mov    eax, edx
        sub    eax, esi
        cmp    eax, 5ch
        je     SHORT loc_1_18C8
        mov    cx, word ptr [edx - 2ch]
        sub    edx, 34h
        mov    word ptr [esi + 16h], cx
        mov    al, byte ptr [edx]
        and    eax, 7fh
        cmp    eax, 17h
        je     SHORT loc_1_18B1
        cmp    eax, 72h
        jne    SHORT loc_1_18BC
loc_1_18B1:
        mov    ax, word ptr [edx + 0ah]
        add    ax, cx
        mov    word ptr [esi + 16h], ax
loc_1_18BC:
        mov    word ptr [esi + 18h], 0
        mov    byte ptr [esi + 7], 1
        jmp    SHORT loc_1_18CC
loc_1_18C8:
        mov    byte ptr [esi + 7], 0
loc_1_18CC:
        cmp    byte ptr [esi + 7], 8
        jne    SHORT loc_1_18E0
        mov    ax, word ptr [esi + 16h]
        cmp    word ptr [esi + 18h], ax
        jae    SHORT loc_1_18E0
        mov    byte ptr [esi + 7], 1
loc_1_18E0:
        cmp    byte ptr [esi + 7], 1
        jne    SHORT loc_1_1915
        mov    ax, word ptr [esi + 4]
        test   al, 40h
        jne    SHORT loc_1_1915
        or     ax, 40h
        mov    word ptr [esi + 4], ax
        db  0C7h, 046h, 008h, 0D4h, 078h, 000h, 000h
        lea    ecx, [esi + 8]
        db  0A1h, 0D8h, 078h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 0D8h, 078h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 0D8h, 078h, 000h, 000h
loc_1_1915:
        test   byte ptr [edi], 40h
        je     SHORT loc_1_1945
        mov    eax, dword ptr [esi + 68h]
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_1940
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   esi
        mov    eax, dword ptr [esi + 68h]
        push   eax
        call   sub_1_0E00
        add    esp, 8
        jmp    SHORT loc_1_1945
        db  005h, 000h, 000h, 000h, 000h
loc_1_1940:
        or     word ptr [esi + 4], 2
loc_1_1945:
        test   byte ptr [edi], 8
        je     SHORT loc_1_197C
        mov    eax, dword ptr [esi + 6ch]
        or     eax, eax
        je     SHORT loc_1_1973
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_1973
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 6ch]
        push   eax
        call   sub_1_3820
        add    esp, 4
        mov    dword ptr [esi + 6ch], 0
loc_1_1973:
        push   esi
        call   sub_1_43D0
        add    esp, 4
loc_1_197C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1990:
        push   ebx
        push   esi
        push   edi
        db  0A1h, 0E8h, 078h, 000h, 000h
        mov    esi, dword ptr [esp + 10h]
        lea    ecx, [eax + eax*2]
        db  0A1h, 064h, 078h, 000h, 000h
        add    ecx, eax
        mov    dword ptr [esi + 74h], ecx
        mov    al, byte ptr [esi + 7]
        or     al, al
        jne    SHORT loc_1_19B5
        jmp    NEAR PTR loc_1_1BF0
loc_1_19B5:
        cmp    al, 8
        jne    NEAR PTR loc_1_1BF0
        mov    edi, dword ptr [esp + 14h]
        mov    cx, word ptr [esi + 14h]
        mov    ax, word ptr [edi + 0eh]
        mov    dx, ax
        sub    dx, cx
        cmp    dx, 1
        je     SHORT loc_1_19EC
        cmp    cx, ax
        jne    NEAR PTR loc_1_1BF4
        mov    ax, word ptr [esi + 18h]
        cmp    word ptr [edi + 10h], ax
        jbe    NEAR PTR loc_1_1BF4
loc_1_19EC:
        mov    byte ptr [esi + 7], 1
        mov    eax, dword ptr [esi + 6ch]
        or     eax, eax
        je     SHORT loc_1_1A19
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_1A19
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 6ch]
        push   eax
        call   sub_1_3820
        add    esp, 4
        mov    dword ptr [esi + 6ch], 0
loc_1_1A19:
        mov    ax, word ptr [edi + 10h]
        mov    word ptr [esi + 18h], ax
        mov    ax, word ptr [edi + 0eh]
        mov    word ptr [esi + 14h], ax
        db  0A1h, 094h, 079h, 000h, 000h
        mov    word ptr [esi + 28h], ax
        mov    ax, word ptr [esi + 18h]
        or     ax, ax
        je     SHORT loc_1_1A87
        cmp    word ptr [esi + 16h], ax
        je     SHORT loc_1_1A87
        mov    ax, word ptr [esi + 4]
        test   al, 40h
        jne    NEAR PTR loc_1_1BF4
        mov    ecx, dword ptr [esi + 5ch]
        sub    ecx, esi
        cmp    ecx, 5ch
        je     NEAR PTR loc_1_1BF4
        or     ax, 40h
        mov    word ptr [esi + 4], ax
        db  0C7h, 046h, 008h, 0D4h, 078h, 000h, 000h
        lea    ecx, [esi + 8]
        db  0A1h, 0D8h, 078h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 0D8h, 078h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 0D8h, 078h, 000h, 000h
        jmp    NEAR PTR loc_1_1BF4
loc_1_1A87:
        mov    ecx, dword ptr [esi + 5ch]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 5ch
        mov    ebx, 0
        je     SHORT loc_1_1AA4
        lea    ebx, [ecx - 34h]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
loc_1_1AA4:
        test   byte ptr [esi + 4], 10h
        je     SHORT loc_1_1AEB
        mov    eax, dword ptr [esi + 68h]
        mov    byte ptr [esi + 6], 6
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_1AE0
        push   eax
        call   sub_1_3C30
        add    esp, 4
        db  0A1h, 094h, 079h, 000h, 000h
        mov    word ptr [esi + 28h], ax
        push   0
        mov    eax, dword ptr [esi + 68h]
        push   eax
        push   esi
        call   sub_1_0B20
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1B7B
        db  08Dh, 049h, 000h
loc_1_1AE0:
        or     word ptr [esi + 4], 1000h
        jmp    NEAR PTR loc_1_1B7B
loc_1_1AEB:
        mov    edx, dword ptr [esi + 5ch]
        mov    eax, edx
        sub    eax, esi
        cmp    eax, 5ch
        jne    SHORT loc_1_1B21
        mov    byte ptr [esi + 7], 0
        mov    byte ptr [esi + 6], 3
        mov    ax, word ptr [esi + 4]
        test   al, 40h
        je     SHORT loc_1_1B7B
        test   ax, 1f9eh
        jne    SHORT loc_1_1B7B
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        and    word ptr [esi + 4], 0ffbfh
        jmp    SHORT loc_1_1B7B
loc_1_1B21:
        sub    edx, 34h
        mov    word ptr [esi + 18h], 0
        mov    cx, word ptr [edx + 8]
        mov    word ptr [esi + 16h], cx
        mov    al, byte ptr [edx]
        and    eax, 7fh
        cmp    eax, 17h
        je     SHORT loc_1_1B41
        cmp    eax, 72h
        jne    SHORT loc_1_1B4C
loc_1_1B41:
        mov    ax, word ptr [edx + 0ah]
        add    ax, cx
        mov    word ptr [esi + 16h], ax
loc_1_1B4C:
        mov    ax, word ptr [esi + 4]
        test   al, 40h
        jne    SHORT loc_1_1B7B
        or     ax, 40h
        mov    word ptr [esi + 4], ax
        db  0C7h, 046h, 008h, 0D4h, 078h, 000h, 000h
        lea    ecx, [esi + 8]
        db  0A1h, 0D8h, 078h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 0D8h, 078h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 0D8h, 078h, 000h, 000h
loc_1_1B7B:
        mov    al, byte ptr [ebx]
        and    eax, 7fh
        cmp    eax, 48h
        jne    SHORT loc_1_1BC0
        mov    ax, word ptr [ebx + 0ah]
        mov    word ptr [ebx + 8], ax
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ebx + 4], eax
        db  0C7h, 043h, 03Ch, 010h, 047h, 000h, 000h
        db  0C7h, 043h, 034h, 084h, 078h, 000h, 000h
        add    ebx, 34h
        db  0A1h, 088h, 078h, 000h, 000h
        mov    dword ptr [ebx + 4], eax
        db  0A1h, 088h, 078h, 000h, 000h
        mov    dword ptr [eax], ebx
        db  089h, 01Dh, 088h, 078h, 000h, 000h
        jmp    SHORT loc_1_1BF4
        db  005h, 000h, 000h, 000h, 000h
loc_1_1BC0:
        mov    byte ptr [ebx + 1], 0
        test   byte ptr [ebx], 80h
        je     SHORT loc_1_1BF4
        db  0C7h, 043h, 034h, 090h, 078h, 000h, 000h
        add    ebx, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [ebx + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], ebx
        db  089h, 01Dh, 094h, 078h, 000h, 000h
        jmp    SHORT loc_1_1BF4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1BF0:
        mov    edi, dword ptr [esp + 14h]
loc_1_1BF4:
        test   byte ptr [edi], 40h
        je     SHORT loc_1_1C25
        mov    eax, dword ptr [esi + 68h]
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_1C20
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   esi
        mov    eax, dword ptr [esi + 68h]
        push   eax
        call   sub_1_0E00
        add    esp, 8
        jmp    SHORT loc_1_1C25
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1C20:
        or     word ptr [esi + 4], 2
loc_1_1C25:
        test   byte ptr [edi], 8
        je     SHORT loc_1_1C4C
        mov    eax, dword ptr [esi + 6ch]
        or     eax, eax
        je     SHORT loc_1_1C4C
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_1C4C
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 6ch]
        push   eax
        call   sub_1_4230
        add    esp, 4
loc_1_1C4C:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1C50:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 18h]
        xor    eax, eax
        mov    ax, word ptr [ebx + 4eh]
        push   eax
        call   sub_1_0187
        mov    edi, dword ptr [esp + 20h]
        add    esp, 4
        mov    dx, word ptr [edi + 6]
        mov    esi, dword ptr [esp + 14h]
        mov    cx, word ptr [esi + 1ch]
        cmp    dx, cx
        jne    SHORT loc_1_1CC0
        xor    ebp, ebp
        mov    bp, word ptr [edi + 0ch]
        and    eax, 0ffffh
        sub    eax, 30h
        cmp    ebp, eax
        ja     SHORT loc_1_1CC0
        mov    ax, word ptr [esi + 4]
        and    ax, 0ff7fh
        mov    word ptr [esi + 4], ax
        test   ah, 80h
        push   edi
        push   ebx
        push   esi
        je     SHORT loc_1_1CB0
        call   sub_1_4B30
        add    esp, 0ch
        jmp    SHORT loc_1_1CDC
        db  08Dh, 064h, 024h, 000h
loc_1_1CB0:
        call   sub_1_4960
        add    esp, 0ch
        jmp    SHORT loc_1_1CDC
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1CC0:
        sub    dx, cx
        cmp    dx, 8
        ja     SHORT loc_1_1CDC
        mov    ax, word ptr [esi + 4]
        test   al, 80h
        jne    SHORT loc_1_1CDC
        or     ax, 880h
        mov    word ptr [esi + 4], ax
        or     byte ptr [edi], 40h
loc_1_1CDC:
        test   byte ptr [esi + 5], 80h
        je     SHORT loc_1_1D46
        push   edi
        push   esi
        call   sub_1_1650
        add    esp, 8
        db  080h, 03Dh, 090h, 079h, 000h, 000h, 000h
        je     NEAR PTR loc_1_1D85
        test   byte ptr [edi], 40h
        jne    NEAR PTR loc_1_1D85
        mov    ax, word ptr [esi + 4]
        test   ah, 20h
        jne    SHORT loc_1_1D85
        or     ax, 2000h
        mov    word ptr [esi + 4], ax
        mov    eax, dword ptr [esi + 68h]
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_1D85
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 68h]
        db  0C7h, 040h, 004h, 000h, 010h, 000h, 000h
        db  0A1h, 03Ch, 079h, 000h, 000h
        push   eax
        mov    eax, dword ptr [esi + 68h]
        push   eax
        call   sub_1_3BE0
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_1D46:
        db  0A1h, 0E8h, 078h, 000h, 000h
        lea    ecx, [eax + eax*2]
        db  003h, 00Dh, 064h, 078h, 000h, 000h
        mov    dword ptr [esi + 74h], ecx
        test   byte ptr [edi], 40h
        je     SHORT loc_1_1D85
        mov    eax, dword ptr [esi + 68h]
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_1D80
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   esi
        mov    eax, dword ptr [esi + 68h]
        push   eax
        call   sub_1_0E00
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_1D80:
        or     word ptr [esi + 4], 2
loc_1_1D85:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1D90:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_3900
        add    esp, 4
        jmp    NEAR PTR sub_1_2E80
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1DB0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_3860
        mov    edx, dword ptr [esp + 1ch]
        mov    dword ptr [esp + 10h], eax
        mov    ax, word ptr [edx + 22h]
        lea    ebp, [edx + 1eh]
        mov    si, ax
        and    esi, 0ffff00ffh
        mov    word ptr [esp + 14h], si
        db  08Bh, 00Dh, 07Ch, 079h, 000h, 000h
        and    esi, 0ffffh
        mov    ebx, dword ptr [ecx + esi*4]
        or     ebx, ebx
        je     NEAR PTR loc_1_1FD0
        cmp    word ptr [ebx + 10h], ax
        jne    NEAR PTR loc_1_1FD0
        lea    edi, [ebx + 3ch]
        lea    esi, [edx + 12h]
        mov    ecx, 2
        repe cmpsd
        jne    SHORT loc_1_1E0F
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
loc_1_1E0F:
        je     SHORT loc_1_1E30
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_1E25
        mov    ecx, dword ptr [esp + 10h]
        push   ecx
        call   sub_1_3900
        add    esp, 4
loc_1_1E25:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_1E30:
        mov    al, byte ptr [ebx + 6]
        cmp    al, 1
        jne    SHORT loc_1_1E50
        test   byte ptr [ebp], 80h
        je     SHORT loc_1_1E50
        push   edx
        push   ebx
        call   sub_1_14C0
        jmp    NEAR PTR loc_1_1FE8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1E50:
        mov    cx, word ptr [ebx + 12h]
        cmp    word ptr [ebp + 2], cx
        jne    NEAR PTR loc_1_1FEB
        cmp    al, 3
        jne    NEAR PTR loc_1_1FEB
        test   byte ptr [ebp], 80h
        je     SHORT loc_1_1E8A
        test   byte ptr [ebx + 5], 80h
        push   ebp
        push   ebx
        je     SHORT loc_1_1E80
        call   sub_1_1650
        jmp    NEAR PTR loc_1_1FE8
        db  08Bh, 0FFh
loc_1_1E80:
        call   sub_1_1990
        jmp    NEAR PTR loc_1_1FE8
loc_1_1E8A:
        xor    eax, eax
        mov    ax, word ptr [edx + 2]
        push   eax
        call   sub_1_0187
        add    esp, 4
        mov    dx, word ptr [ebp + 6]
        mov    cx, word ptr [ebx + 1ch]
        cmp    dx, cx
        jne    SHORT loc_1_1F08
        xor    esi, esi
        mov    si, word ptr [ebp + 0ch]
        and    eax, 0ffffh
        sub    eax, 30h
        cmp    esi, eax
        ja     SHORT loc_1_1F08
        mov    ax, word ptr [ebx + 4]
        and    ax, 0ff7fh
        cmp    dword ptr [esp + 10h], 0
        mov    word ptr [ebx + 4], ax
        je     SHORT loc_1_1F25
        mov    ecx, dword ptr [esp + 10h]
        mov    si, word ptr [esp + 14h]
        mov    word ptr [ecx + 3eh], si
        test   ah, 80h
        mov    word ptr [ecx + 28h], 30h
        db  0C7h, 041h, 004h, 070h, 050h, 000h, 000h
        je     SHORT loc_1_1EF7
        push   ecx
        push   ebp
        push   ebx
        call   sub_1_4EC0
        add    esp, 0ch
        jmp    SHORT loc_1_1F25
loc_1_1EF7:
        mov    ecx, dword ptr [esp + 10h]
        push   ecx
        push   ebp
        push   ebx
        call   sub_1_4D40
        add    esp, 0ch
        jmp    SHORT loc_1_1F25
loc_1_1F08:
        sub    dx, cx
        cmp    dx, 8
        ja     SHORT loc_1_1F25
        mov    ax, word ptr [ebx + 4]
        test   al, 80h
        jne    SHORT loc_1_1F25
        or     ax, 880h
        mov    word ptr [ebx + 4], ax
        or     byte ptr [ebp], 40h
loc_1_1F25:
        test   byte ptr [ebx + 5], 80h
        je     SHORT loc_1_1F92
        push   ebp
        push   ebx
        call   sub_1_1650
        add    esp, 8
        db  080h, 03Dh, 090h, 079h, 000h, 000h, 000h
        je     NEAR PTR loc_1_1FEB
        test   byte ptr [ebp], 40h
        jne    NEAR PTR loc_1_1FEB
        mov    ax, word ptr [ebx + 4]
        test   ah, 20h
        jne    NEAR PTR loc_1_1FEB
        or     ax, 2000h
        mov    word ptr [ebx + 4], ax
        mov    eax, dword ptr [ebx + 68h]
        test   byte ptr [eax + 3ch], 8
        je     NEAR PTR loc_1_1FEB
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [ebx + 68h]
        db  0C7h, 040h, 004h, 000h, 010h, 000h, 000h
        db  0A1h, 03Ch, 079h, 000h, 000h
        push   eax
        mov    eax, dword ptr [ebx + 68h]
        push   eax
        call   sub_1_3BE0
        jmp    SHORT loc_1_1FE8
loc_1_1F92:
        db  0A1h, 0E8h, 078h, 000h, 000h
        lea    ecx, [eax + eax*2]
        db  003h, 00Dh, 064h, 078h, 000h, 000h
        mov    dword ptr [ebx + 74h], ecx
        test   byte ptr [ebp], 40h
        je     SHORT loc_1_1FEB
        mov    eax, dword ptr [ebx + 68h]
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_1FC7
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   ebx
        mov    eax, dword ptr [ebx + 68h]
        push   eax
        call   sub_1_0E00
        jmp    SHORT loc_1_1FE8
loc_1_1FC7:
        or     word ptr [ebx + 4], 2
        jmp    SHORT loc_1_1FEB
        db  08Bh, 0FFh
loc_1_1FD0:
        cmp    ax, 0ffffh
        jne    SHORT loc_1_1FEB
        cmp    word ptr [ebp + 6], 0
        jne    SHORT loc_1_1FEB
        mov    eax, dword ptr [esp + 20h]
        push   eax
        push   edx
        call   sub_1_11A0
loc_1_1FE8:
        add    esp, 8
loc_1_1FEB:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_200A
        mov    ecx, dword ptr [esp + 10h]
        cmp    word ptr [ecx + 22h], 1
        jne    SHORT loc_1_200A
        mov    word ptr [ecx + 22h], 0
        db  0C7h, 041h, 004h, 090h, 01Dh, 000h, 000h
loc_1_200A:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2020:
        push   ebx
        push   esi
        push   edi
        mov    edx, 0
        db  0BBh, 020h, 078h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
loc_1_2031:
        mov    ecx, 4
        mov    esi, eax
        mov    edi, ebx
        repe cmpsd
        je     SHORT loc_1_2050
        add    ebx, 10h
        inc    edx
        db  081h, 0FBh, 040h, 078h, 000h, 000h
        jb     SHORT loc_1_2031
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_2050:
        db  08Bh, 004h, 095h, 000h, 076h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_1_2060:
        push   esi
        db  08Bh, 015h, 000h, 076h, 000h, 000h
        sub    ecx, ecx
        mov    esi, 1
        mov    edi, edi
loc_1_2070:
        db  08Bh, 004h, 0B5h, 000h, 076h, 000h, 000h
        cmp    edx, eax
        jbe    SHORT loc_1_207F
        mov    ecx, esi
        mov    edx, eax
loc_1_207F:
        inc    esi
        cmp    esi, 2
        jl     SHORT loc_1_2070
        mov    edx, dword ptr [esp + 8]
        mov    esi, ecx
        shl    esi, 4
        db  081h, 0C6h, 020h, 078h, 000h, 000h
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [esi + 0ch], eax
        db  0A1h, 064h, 078h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 076h, 000h, 000h
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_20C0:
        push   esi
        push   edi
        mov    edx, 0
        db  0B8h, 020h, 078h, 000h, 000h
        lea    esp, [esp]
loc_1_20D0:
        db  0BEh, 0CAh, 075h, 000h, 000h
        mov    ecx, 4
        mov    edi, eax
        repe cmpsd
        je     SHORT loc_1_20EB
        add    eax, 10h
        inc    edx
        db  03Dh, 040h, 078h, 000h, 000h
        jb     SHORT loc_1_20D0
loc_1_20EB:
        db  0C6h, 005h, 0F1h, 075h, 000h, 000h, 000h
        cmp    edx, 2
        jge    SHORT loc_1_212C
        db  080h, 03Dh, 0C1h, 075h, 000h, 000h, 000h
        jne    SHORT loc_1_2120
        mov    eax, edx
        shl    eax, 4
        db  0C7h, 004h, 095h, 000h, 076h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 020h, 078h, 000h, 000h, 02Ah, 02Ah, 02Ah, 02Ah
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_2120:
        db  0A1h, 064h, 078h, 000h, 000h
        db  089h, 004h, 095h, 000h, 076h, 000h, 000h
loc_1_212C:
        pop    edi
        pop    esi
        ret
        db  090h
sub_1_2130:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 3]
        db  0A1h, 050h, 078h, 000h, 000h
        mov    al, byte ptr [ecx + eax]
        and    eax, 0ffh
        db  039h, 005h, 084h, 079h, 000h, 000h
        jbe    NEAR PTR loc_1_2250
        mov    ecx, eax
        shl    ecx, 4
        add    ecx, eax
        db  0A1h, 0F4h, 078h, 000h, 000h
        test   byte ptr [eax + ecx*4 + 14h], 4
        je     NEAR PTR loc_1_2250
        mov    ax, word ptr [esi + 8]
        db  066h, 08Bh, 00Dh, 0E4h, 078h, 000h, 000h
        cmp    cx, 200h
        jb     SHORT loc_1_2182
        mov    cx, 200h
loc_1_2182:
        cmp    ax, cx
        jbe    SHORT loc_1_2190
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_2190:
        sub    edi, edi
        db  080h, 03Dh, 014h, 078h, 000h, 000h, 000h
        je     SHORT loc_1_221A
        lea    ebx, [esi + 0ah]
        push   ebx
        call   sub_1_2020
        add    esp, 4
        cmp    eax, edi
        je     SHORT loc_1_2208
        db  080h, 03Dh, 0F1h, 075h, 000h, 000h, 000h
        jne    SHORT loc_1_221A
        db  08Bh, 00Dh, 064h, 078h, 000h, 000h
        sub    ecx, eax
        cmp    ecx, 7d0h
        jbe    SHORT loc_1_221A
        db  0B9h, 0CAh, 075h, 000h, 000h
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        db  0C6h, 005h, 0F1h, 075h, 000h, 000h, 0FFh
        db  068h, 0C0h, 075h, 000h, 000h
        call   sub_1_2750
        add    esp, 4
        and    eax, 0ffh
        cmp    eax, 0ffh
        je     SHORT loc_1_221A
        db  0C6h, 005h, 0F1h, 075h, 000h, 000h, 000h
        jmp    SHORT loc_1_221A
loc_1_2208:
        db  0C7h, 046h, 03Ch, 0E0h, 025h, 000h, 000h
        push   esi
        call   sub_1_27D0
        add    esp, 4
        mov    edi, eax
loc_1_221A:
        or     edi, edi
        jne    SHORT loc_1_2248
        mov    dword ptr [esi + 3ch], 0
        db  0C7h, 046h, 034h, 048h, 078h, 000h, 000h
        lea    ecx, [esi + 34h]
        db  0A1h, 04Ch, 078h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 04Ch, 078h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 04Ch, 078h, 000h, 000h
        mov    byte ptr [esi + 32h], 0ch
loc_1_2248:
        mov    al, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_2250:
        mov    al, 13h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2260:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    al, byte ptr [ecx + 3]
        cmp    al, 0ffh
        jne    SHORT loc_1_2293
        db  0C7h, 041h, 034h, 06Ch, 079h, 000h, 000h
        lea    edx, [ecx + 34h]
        db  0A1h, 070h, 079h, 000h, 000h
        mov    dword ptr [edx + 4], eax
        db  0A1h, 070h, 079h, 000h, 000h
        pop    esi
        mov    dword ptr [eax], edx
        mov    al, 0ffh
        db  089h, 015h, 070h, 079h, 000h, 000h
        mov    byte ptr [ecx + 32h], 4
        ret
loc_1_2293:
        xor    edx, edx
        mov    dl, al
        db  0A1h, 050h, 078h, 000h, 000h
        mov    al, byte ptr [edx + eax]
        and    eax, 0ffh
        db  039h, 005h, 084h, 079h, 000h, 000h
        jbe    SHORT loc_1_22DC
        mov    edx, eax
        shl    edx, 4
        add    edx, eax
        db  0A1h, 0F4h, 078h, 000h, 000h
        lea    edx, [eax + edx*4]
        test   byte ptr [edx + 14h], 4
        je     SHORT loc_1_22DC
        lea    eax, [ecx + 34h]
        add    edx, 20h
        mov    dword ptr [eax], edx
        mov    esi, dword ptr [edx + 4]
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [esi], eax
        mov    dword ptr [edx + 4], eax
        mov    al, 0ffh
        mov    byte ptr [ecx + 32h], 4
        pop    esi
        ret
loc_1_22DC:
        mov    al, 13h
        pop    esi
        ret
loc_1_22E0:
        sub    esp, 10h
        push   esi
        mov    ecx, dword ptr [esp + 18h]
        lea    edx, [esp + 4]
        mov    eax, dword ptr [ecx + 0ah]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 0eh]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 12h]
        lea    esi, [ecx + 0ah]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [ecx + 16h], 0
        mov    dword ptr [ecx + 12h], 0
        mov    dword ptr [ecx + 0eh], 0
        mov    dword ptr [esi], 0
        mov    byte ptr [esi], 2ah
        push   ecx
        call   sub_1_2130
        lea    edx, [esp + 8]
        add    esp, 4
        mov    ecx, dword ptr [edx]
        mov    dword ptr [esi], ecx
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], ecx
        mov    ecx, dword ptr [edx + 8]
        mov    dword ptr [esi + 8], ecx
        mov    ecx, dword ptr [edx + 0ch]
        mov    dword ptr [esi + 0ch], ecx
        pop    esi
        add    esp, 10h
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_2350:
        mov    edx, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 3]
        db  0A1h, 050h, 078h, 000h, 000h
        mov    al, byte ptr [ecx + eax]
        and    eax, 0ffh
        db  039h, 005h, 084h, 079h, 000h, 000h
        jbe    SHORT loc_1_23A7
        mov    ecx, eax
        shl    ecx, 4
        add    ecx, eax
        db  0A1h, 0F4h, 078h, 000h, 000h
        test   byte ptr [eax + ecx*4 + 14h], 4
        je     SHORT loc_1_23A7
        db  0C7h, 042h, 034h, 018h, 079h, 000h, 000h
        lea    ecx, [edx + 34h]
        db  0A1h, 01Ch, 079h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 01Ch, 079h, 000h, 000h
        mov    dword ptr [eax], ecx
        mov    al, 0ffh
        db  089h, 00Dh, 01Ch, 079h, 000h, 000h
        mov    byte ptr [edx + 32h], 4
        ret
loc_1_23A7:
        mov    al, 13h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_23B0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 32h], 4
        jne    SHORT loc_1_23D0
        lea    eax, [esi + 34h]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [esi + 1], 0bh
        sub    al, al
        pop    esi
        ret
        db  090h
loc_1_23D0:
        mov    al, 26h
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_23E0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   byte ptr [esi + 3ch], 20h
        je     SHORT loc_1_2425
        mov    ecx, dword ptr [esi + 40h]
        or     ecx, ecx
        je     SHORT loc_1_2425
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_2425
        mov    byte ptr [ecx + 1], 0
        test   byte ptr [ecx], 80h
        je     SHORT loc_1_2420
        db  0C7h, 041h, 034h, 090h, 078h, 000h, 000h
        add    ecx, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 094h, 078h, 000h, 000h
loc_1_2420:
        call   sub_1_2E80
loc_1_2425:
        push   esi
        call   sub_1_3820
        add    esp, 4
        pop    esi
        ret
sub_1_2430:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 18h]
        lea    esi, [ecx + 1ch]
        mov    word ptr [ecx + 28h], 40h
        mov    edx, dword ptr [eax]
        mov    dword ptr [esi], edx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [esi + 4], ax
        db  0C7h, 041h, 004h, 0E0h, 023h, 000h, 000h
        mov    edx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 40h], edx
        or     word ptr [ecx + 3ch], 20h
        mov    word ptr [ecx + 22h], 2
        add    ecx, 4ch
        mov    ax, word ptr [edx + 8]
        mov    word ptr [ecx - 1eh], ax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx - 22h], eax
        mov    byte ptr [ecx + 5], 4
        mov    esi, dword ptr [esp + 14h]
        lea    edi, [ecx + 6]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 4], eax
        add    ecx, 1eh
        mov    ax, word ptr [esi + 8]
        mov    word ptr [edi + 8], ax
        movzx  esi, byte ptr [edx + 3]
        db  0A1h, 050h, 078h, 000h, 000h
        lea    edi, [ecx + 2]
        mov    al, byte ptr [esi + eax]
        and    eax, 0ffh
        db  039h, 005h, 084h, 079h, 000h, 000h
        jbe    SHORT loc_1_24C4
        mov    esi, eax
        shl    esi, 4
        add    esi, eax
        db  0A1h, 0F4h, 078h, 000h, 000h
        lea    edi, [eax + esi*4]
loc_1_24C4:
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 6], eax
        lea    esi, [ecx + 2]
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 0ch], eax
        mov    al, byte ptr [edx]
        and    eax, 7fh
        cmp    eax, 20h
        jne    SHORT loc_1_2510
        lea    esi, [ecx + 12h]
        mov    eax, dword ptr [edx + 0ah]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edx + 0eh]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edx + 12h]
        add    edx, 0ah
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [esi + 0ch], eax
        mov    byte ptr [ecx + 1], 0bh
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_2510:
        mov    byte ptr [ecx + 1], 0ch
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2520:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 40h]
        or     edi, edi
        je     NEAR PTR loc_1_25CD
        cmp    byte ptr [edi + 1], 0ffh
        jne    NEAR PTR loc_1_25CD
        mov    edx, dword ptr [edi + 3ch]
        or     edx, edx
        je     SHORT loc_1_2587
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 52h], eax
        lea    eax, [edx + 8]
        lea    ebx, [esi + 1ch]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebx + 4], ax
        add    esi, 44h
        mov    eax, dword ptr [edx]
        mov    dword ptr [edi + 3ch], eax
        db  0C7h, 006h, 0A0h, 078h, 000h, 000h
        db  0A1h, 0A4h, 078h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 0A4h, 078h, 000h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 0A4h, 078h, 000h, 000h
        call   sub_1_2E80
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_2587:
        lea    eax, [edi + 0ah]
        push   eax
        db  068h, 0C0h, 078h, 000h, 000h
        call   sub_1_2780
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_25A0
        dec    word ptr [eax + 2ah]
loc_1_25A0:
        mov    byte ptr [edi + 1], 0
        test   byte ptr [edi], 80h
        je     SHORT loc_1_25C8
        db  0C7h, 047h, 034h, 090h, 078h, 000h, 000h
        add    edi, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [edi + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], edi
        db  089h, 03Dh, 094h, 078h, 000h, 000h
loc_1_25C8:
        call   sub_1_2E80
loc_1_25CD:
        push   esi
        call   sub_1_3820
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_25E0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        or     eax, eax
        mov    dword ptr [esi + 3ch], eax
        je     SHORT loc_1_25F6
        inc    word ptr [eax + 2ah]
        jmp    SHORT loc_1_2602
loc_1_25F6:
        lea    eax, [esi + 0ah]
        push   eax
        call   sub_1_2060
        add    esp, 4
loc_1_2602:
        db  0C7h, 046h, 034h, 048h, 078h, 000h, 000h
        lea    ecx, [esi + 34h]
        db  0A1h, 04Ch, 078h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 04Ch, 078h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 04Ch, 078h, 000h, 000h
        mov    byte ptr [esi + 32h], 0ch
        pop    esi
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2630:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  081h, 03Dh, 048h, 078h, 000h, 000h, 048h, 078h, 000h, 000h
        je     NEAR PTR loc_1_273B
        db  0BBh, 048h, 078h, 000h, 000h
        lea    esp, [esp]
loc_1_2650:
        call   sub_1_37F0
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_273B
        db  039h, 01Dh, 048h, 078h, 000h, 000h
        mov    edi, 0
        je     SHORT loc_1_2683
        db  08Bh, 03Dh, 048h, 078h, 000h, 000h
        sub    edi, 34h
        db  0A1h, 048h, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_2683:
        mov    byte ptr [edi + 32h], 0
        mov    ebp, dword ptr [edi + 3ch]
        or     ebp, ebp
        jne    SHORT loc_1_26A7
        db  068h, 0ACh, 078h, 000h, 000h
        db  068h, 0A8h, 078h, 000h, 000h
        push   edi
        push   esi
        call   sub_1_2430
        add    esp, 10h
        jmp    NEAR PTR loc_1_2726
loc_1_26A7:
        test   byte ptr [ebp + 28h], 80h
        je     SHORT loc_1_2710
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_1_26C5
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi + 3ch], eax
        jmp    SHORT loc_1_26D5
loc_1_26C5:
        db  0A1h, 020h, 079h, 000h, 000h
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [edi + 3ch], 0
loc_1_26D5:
        mov    dword ptr [esp + 14h], 0ffffffffh
        mov    word ptr [esp + 18h], 0ffffh
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_1_26F0
        add    eax, 8
        jmp    SHORT loc_1_26F5
loc_1_26F0:
        db  0B8h, 0ACh, 078h, 000h, 000h
loc_1_26F5:
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        push   edi
        push   esi
        call   sub_1_2430
        add    esp, 10h
        db  0C7h, 046h, 004h, 020h, 025h, 000h, 000h
        jmp    SHORT loc_1_2726
        db  08Bh, 0FFh
loc_1_2710:
        lea    eax, [ebp + 1ah]
        push   eax
        lea    eax, [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_1_2430
        add    esp, 10h
        dec    word ptr [ebp + 2ah]
loc_1_2726:
        push   esi
        call   sub_1_0292
        add    esp, 4
        db  039h, 01Dh, 048h, 078h, 000h, 000h
        jne    NEAR PTR loc_1_2650
loc_1_273B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2750:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 8], 4
        jae    SHORT loc_1_2760
        mov    al, 1
        ret
        db  08Bh, 0FFh
loc_1_2760:
        db  0C7h, 040h, 03Ch, 050h, 02Ah, 000h, 000h
        push   eax
        call   sub_1_27D0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_2777
        mov    al, 9
        ret
loc_1_2777:
        mov    al, 0ffh
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2780:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 18h]
        mov    ebp, dword ptr [edx + 4]
        cmp    ebp, edx
        je     SHORT loc_1_27B0
        mov    ebx, dword ptr [esp + 1ch]
loc_1_2796:
        lea    eax, [ebp - 20h]
        mov    ecx, 4
        mov    esi, ebx
        mov    edi, eax
        repe cmpsd
        je     SHORT loc_1_27B4
        mov    ebp, dword ptr [ebp + 4]
        cmp    ebp, edx
        jne    SHORT loc_1_2796
        jmp    SHORT loc_1_27B4
        db  090h
loc_1_27B0:
        mov    eax, dword ptr [esp + 10h]
loc_1_27B4:
        cmp    ebp, edx
        jne    SHORT loc_1_27BA
        sub    eax, eax
loc_1_27BA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_27D0:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        lea    ebx, [edi + 0ah]
        push   ebx
        db  068h, 0C0h, 078h, 000h, 000h
        call   sub_1_2780
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_2800
        push   esi
        push   edi
        call   dword ptr [edi + 3ch]
        add    esp, 8
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_2800:
        push   ebx
        db  068h, 0ECh, 078h, 000h, 000h
        call   sub_1_2780
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_2831
        lea    ecx, [edi + 34h]
        lea    edx, [esi + 34h]
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
        mov    eax, esi
        mov    dword ptr [edx + 4], ecx
        mov    byte ptr [edi + 32h], 8
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_2831:
        db  081h, 03Dh, 0CCh, 078h, 000h, 000h, 0CCh, 078h, 000h, 000h
        mov    esi, 0
        je     SHORT loc_1_2859
        db  08Bh, 035h, 0CCh, 078h, 000h, 000h
        sub    esi, 20h
        db  0A1h, 0CCh, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_2859:
        or     esi, esi
        jne    SHORT loc_1_28B9
        db  0B8h, 0C0h, 078h, 000h, 000h
        sub    ecx, ecx
loc_1_2864:
        mov    eax, dword ptr [eax]
        db  03Dh, 0C0h, 078h, 000h, 000h
        je     SHORT loc_1_288D
        cmp    word ptr [eax + 0ah], cx
        lea    esi, [eax - 20h]
        jne    SHORT loc_1_2864
        db  0A1h, 0C0h, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
        push   esi
        call   sub_1_2BC0
        add    esp, 4
loc_1_288D:
        db  081h, 03Dh, 0CCh, 078h, 000h, 000h, 0CCh, 078h, 000h, 000h
        mov    esi, 0
        je     SHORT loc_1_28B5
        db  08Bh, 035h, 0CCh, 078h, 000h, 000h
        sub    esi, 20h
        db  0A1h, 0CCh, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_28B5:
        or     esi, esi
        je     SHORT loc_1_2926
loc_1_28B9:
        mov    word ptr [esi + 2ah], 1
        mov    dword ptr [esi + 10h], 0
        mov    byte ptr [esi + 28h], 40h
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi], eax
        lea    ecx, [esi + 34h]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 8], eax
        lea    edx, [edi + 34h]
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [ecx], ecx
        mov    dword ptr [esi + 38h], ecx
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 4], edx
        lea    ecx, [esi + 20h]
        mov    byte ptr [edi + 32h], 8
        db  0C7h, 001h, 0ECh, 078h, 000h, 000h
        db  0A1h, 0F0h, 078h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 0F0h, 078h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 0F0h, 078h, 000h, 000h
        db  0A0h, 054h, 078h, 000h, 000h
        mov    byte ptr [esi + 29h], al
        or     byte ptr [esi + 28h], 8
loc_1_2926:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_2930:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
        and    byte ptr [esi + 28h], 0f7h
        lea    ecx, [edx + 4ch]
        db  0C7h, 042h, 004h, 0A0h, 02Bh, 000h, 000h
        mov    dword ptr [edx + 40h], esi
        mov    dword ptr [edx + 1ch], 0ffffffffh
        mov    word ptr [edx + 20h], 0ffffh
        mov    byte ptr [ecx + 5], 4
        mov    dword ptr [ecx + 0ah], 0ffffffffh
        mov    word ptr [ecx + 0eh], 0ffffh
        add    ecx, 1eh
        db  0A1h, 020h, 079h, 000h, 000h
        mov    dword ptr [ecx - 18h], eax
        mov    byte ptr [ecx + 21h], 1
        mov    byte ptr [ecx + 1], 1
        mov    byte ptr [ecx + 20h], 0
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx + 22h], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 26h], eax
        add    ecx, 22h
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    word ptr [edx + 28h], 50h
        push   edx
        call   sub_1_0292
        add    esp, 4
        dec    byte ptr [esi + 29h]
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_29B0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    al, byte ptr [esi + 28h]
        and    al, 80h
        cmp    al, 1
        sbb    ecx, ecx
        and    ecx, 0ffff92a0h
        add    ecx, 7530h
        db  003h, 00Dh, 064h, 078h, 000h, 000h
        mov    dword ptr [esi + 2ch], ecx
        or     byte ptr [esi + 28h], 20h
        db  0A1h, 0C4h, 078h, 000h, 000h
        mov    ecx, eax
        sub    ecx, 20h
        db  03Dh, 0C0h, 078h, 000h, 000h
        je     SHORT loc_1_2A02
        mov    edx, dword ptr [esi + 2ch]
loc_1_29F0:
        cmp    dword ptr [ecx + 2ch], edx
        jbe    SHORT loc_1_2A02
        mov    eax, dword ptr [eax + 4]
        db  03Dh, 0C0h, 078h, 000h, 000h
        lea    ecx, [eax - 20h]
        jne    SHORT loc_1_29F0
loc_1_2A02:
        push   eax
        lea    eax, [esi + 20h]
        push   eax
        call   sub_1_3B30
        add    esp, 8
        sub    ebx, ebx
loc_1_2A11:
        mov    eax, dword ptr [esi + 34h]
        mov    ecx, eax
        sub    ecx, esi
        cmp    ecx, 34h
        mov    edi, ebx
        je     SHORT loc_1_2A2B
        lea    edi, [eax - 34h]
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_2A2B:
        cmp    edi, ebx
        je     SHORT loc_1_2A40
        mov    byte ptr [edi + 32h], bl
        push   esi
        push   edi
        call   dword ptr [edi + 3ch]
        add    esp, 8
        jmp    SHORT loc_1_2A11
        db  08Dh, 064h, 024h, 000h
loc_1_2A40:
        dec    word ptr [esi + 2ah]
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_2A50:
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        mov    edx, dword ptr [esp + 4]
        je     SHORT loc_1_2A92
        mov    ecx, dword ptr [edx + 4]
        mov    word ptr [ecx], 1
        test   byte ptr [eax + 28h], 80h
        je     SHORT loc_1_2A82
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_1_2A7C
loc_1_2A73:
        inc    word ptr [ecx]
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_2A73
loc_1_2A7C:
        mov    byte ptr [ecx + 3], 1
        jmp    SHORT loc_1_2A86
loc_1_2A82:
        mov    byte ptr [ecx + 3], 0
loc_1_2A86:
        mov    word ptr [edx + 8], 4
        mov    byte ptr [edx + 1], 0
        jmp    SHORT loc_1_2A96
loc_1_2A92:
        mov    byte ptr [edx + 1], 5
loc_1_2A96:
        test   byte ptr [edx], 80h
        je     SHORT loc_1_2ABA
        db  0C7h, 042h, 034h, 090h, 078h, 000h, 000h
        add    edx, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [edx + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], edx
        db  089h, 015h, 094h, 078h, 000h, 000h
loc_1_2ABA:
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_1_2AC0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        sub    ebx, ebx
loc_1_2AC9:
        mov    eax, dword ptr [esi + 34h]
        mov    ecx, eax
        sub    ecx, esi
        cmp    ecx, 34h
        mov    edi, ebx
        je     SHORT loc_1_2AE3
        lea    edi, [eax - 34h]
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_2AE3:
        cmp    edi, ebx
        je     SHORT loc_1_2AF4
        mov    byte ptr [edi + 32h], bl
        push   ebx
        push   edi
        call   dword ptr [edi + 3ch]
        add    esp, 8
        jmp    SHORT loc_1_2AC9
loc_1_2AF4:
        dec    word ptr [esi + 2ah]
        push   esi
        call   sub_1_2BC0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2B10:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 40h]
        or     esi, esi
        je     SHORT loc_1_2B94
        mov    cl, byte ptr [esi + 28h]
        test   cl, 40h
        je     SHORT loc_1_2B94
        test   cl, 20h
        jne    SHORT loc_1_2B94
        mov    cl, byte ptr [esi + 29h]
        or     cl, cl
        jne    SHORT loc_1_2B70
        push   eax
        call   sub_1_3820
        add    esp, 4
        lea    eax, [esi + 20h]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        mov    al, byte ptr [esi + 28h]
        and    al, 0c7h
        cmp    al, 0c4h
        push   esi
        jne    SHORT loc_1_2B60
        call   sub_1_29B0
        add    esp, 4
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2B60:
        call   sub_1_2AC0
        add    esp, 4
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2B70:
        db  0C7h, 040h, 004h, 0A0h, 02Bh, 000h, 000h
        db  080h, 03Dh, 014h, 078h, 000h, 000h, 000h
        je     SHORT loc_1_2B84
        mov    byte ptr [eax + 51h], 14h
loc_1_2B84:
        dec    cl
        mov    byte ptr [esi + 29h], cl
        push   eax
        call   sub_1_0292
        add    esp, 4
        pop    esi
        ret
loc_1_2B94:
        push   eax
        call   sub_1_3820
        add    esp, 4
        pop    esi
        ret
        db  090h
loc_1_2BA0:
        mov    eax, dword ptr [esp + 4]
        db  0C7h, 040h, 004h, 010h, 02Bh, 000h, 000h
        db  08Bh, 00Dh, 048h, 079h, 000h, 000h
        push   ecx
        push   eax
        call   sub_1_3BE0
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_2BC0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   byte ptr [esi + 28h], 80h
        je     SHORT loc_1_2BED
        mov    edx, dword ptr [esi + 10h]
        or     edx, edx
        je     SHORT loc_1_2BED
        db  08Bh, 00Dh, 064h, 079h, 000h, 000h
loc_1_2BD8:
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi + 10h], eax
        mov    dword ptr [edx], ecx
        mov    ecx, edx
        db  089h, 00Dh, 064h, 079h, 000h, 000h
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_2BD8
loc_1_2BED:
        mov    byte ptr [esi + 28h], 0
        db  0C7h, 046h, 020h, 0CCh, 078h, 000h, 000h
        add    esi, 20h
        db  0A1h, 0D0h, 078h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 0D0h, 078h, 000h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 0D0h, 078h, 000h, 000h
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2C20:
        push   ebx
        push   esi
        push   edi
        db  081h, 03Dh, 0C0h, 078h, 000h, 000h, 0C0h, 078h, 000h, 000h
        je     SHORT loc_1_2C78
        db  0A1h, 064h, 078h, 000h, 000h
        sub    edi, edi
        db  0BBh, 0C0h, 078h, 000h, 000h
        add    eax, 0
loc_1_2C40:
        db  08Bh, 035h, 0C0h, 078h, 000h, 000h
        sub    esi, 20h
        cmp    dword ptr [esi + 2ch], eax
        ja     SHORT loc_1_2C78
        cmp    word ptr [esi + 2ah], di
        jne    SHORT loc_1_2C78
        db  0A1h, 0C0h, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
        push   esi
        call   sub_1_2BC0
        add    esp, 4
        db  039h, 01Dh, 0C0h, 078h, 000h, 000h
        db  0A1h, 064h, 078h, 000h, 000h
        jne    SHORT loc_1_2C40
loc_1_2C78:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_2C80:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        mov    esi, dword ptr [edx + 10h]
        mov    eax, esi
        or     eax, eax
        je     SHORT loc_1_2C9F
loc_1_2C94:
        cmp    dword ptr [eax + 4], ecx
        je     SHORT loc_1_2C9F
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_2C94
loc_1_2C9F:
        or     eax, eax
        jne    SHORT loc_1_2CD3
        db  08Bh, 03Dh, 064h, 079h, 000h, 000h
        or     edi, edi
        je     SHORT loc_1_2CD3
        mov    eax, dword ptr [edi]
        db  0A3h, 064h, 079h, 000h, 000h
        or     edi, edi
        je     SHORT loc_1_2CD3
        mov    dword ptr [edi + 4], ecx
        mov    eax, dword ptr [esp + 18h]
        lea    ebx, [edi + 8]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebx + 4], ax
        mov    dword ptr [edi], esi
        mov    dword ptr [edx + 10h], edi
loc_1_2CD3:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2CE0:
        sub    esp, 4
        push   ebx
        push   esi
        mov    bl, 0ffh
        db  0F6h, 005h, 0B5h, 078h, 000h, 000h, 080h
        jne    SHORT loc_1_2CF2
        mov    bl, 0feh
loc_1_2CF2:
        mov    esi, dword ptr [esp + 10h]
        mov    al, byte ptr [esi]
        mov    byte ptr [esp + 0bh], al
        and    eax, 0ffh
        mov    ecx, eax
        and    ecx, 0ffffff7fh
        cmp    ecx, 37h
        jb     NEAR PTR loc_1_2DA0
        and    eax, 0ffffff7fh
        cmp    eax, 72h
        jg     SHORT loc_1_2D32
        je     SHORT loc_1_2D70
        cmp    eax, 48h
        je     SHORT loc_1_2D40
        cmp    eax, 70h
        je     SHORT loc_1_2D50
        cmp    eax, 71h
        je     SHORT loc_1_2D60
        jmp    NEAR PTR loc_1_2DB3
loc_1_2D32:
        cmp    eax, 78h
        je     SHORT loc_1_2D80
        cmp    eax, 79h
        je     SHORT loc_1_2D90
        jmp    SHORT loc_1_2DB3
        db  08Bh, 0FFh
loc_1_2D40:
        db  0C7h, 046h, 03Ch, 050h, 03Dh, 000h, 000h
        jmp    SHORT loc_1_2DB5
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2D50:
        db  0C7h, 046h, 03Ch, 030h, 032h, 000h, 000h
        jmp    SHORT loc_1_2DB5
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2D60:
        db  0C7h, 046h, 03Ch, 030h, 03Eh, 000h, 000h
        jmp    SHORT loc_1_2DB5
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2D70:
        db  0C7h, 046h, 03Ch, 040h, 03Eh, 000h, 000h
        jmp    SHORT loc_1_2DB5
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2D80:
        db  0C7h, 046h, 03Ch, 050h, 027h, 000h, 000h
        jmp    SHORT loc_1_2DB5
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2D90:
        db  0C7h, 046h, 03Ch, 020h, 032h, 000h, 000h
        jmp    SHORT loc_1_2DB5
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2DA0:
        and    eax, 0ffffff7fh
        db  08Bh, 004h, 085h, 008h, 076h, 000h, 000h
        mov    dword ptr [esi + 3ch], eax
        or     eax, eax
        jne    SHORT loc_1_2DB5
loc_1_2DB3:
        mov    bl, 3
loc_1_2DB5:
        mov    byte ptr [esi + 32h], 0
        mov    byte ptr [esi + 33h], 0
        mov    byte ptr [esi + 1], bl
        mov    byte ptr [esi + 31h], bl
        cmp    bl, 0ffh
        jne    NEAR PTR loc_1_2E71
        call   sub_1_0081
        or     eax, eax
        je     SHORT loc_1_2DF8
        push   esi
        call   dword ptr [esi + 3ch]
        add    esp, 4
        mov    bl, al
        db  0A1h, 014h, 077h, 000h, 000h
        dec    eax
        db  0A3h, 014h, 077h, 000h, 000h
        cmp    bl, 0ffh
        je     SHORT loc_1_2DF1
        mov    byte ptr [esi + 1], bl
loc_1_2DF1:
        call   sub_1_2E80
        jmp    SHORT loc_1_2E1B
loc_1_2DF8:
        db  0C7h, 046h, 034h, 084h, 078h, 000h, 000h
        lea    ecx, [esi + 34h]
        db  0A1h, 088h, 078h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 088h, 078h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 088h, 078h, 000h, 000h
        mov    byte ptr [esi + 32h], 1
loc_1_2E1B:
        test   byte ptr [esp + 0bh], 80h
        je     SHORT loc_1_2E55
        cmp    bl, 0ffh
        je     SHORT loc_1_2E4B
        db  0C7h, 046h, 034h, 090h, 078h, 000h, 000h
        add    esi, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 094h, 078h, 000h, 000h
        call   sub_1_2E80
loc_1_2E4B:
        sub    bl, bl
        mov    al, bl
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_2E55:
        test   byte ptr [esi + 1], 80h
        je     SHORT loc_1_2E6B
        add    eax, 0
loc_1_2E60:
        call   sub_1_00B1
        test   byte ptr [esi + 1], 80h
        jne    SHORT loc_1_2E60
loc_1_2E6B:
        mov    bl, byte ptr [esi + 1]
        mov    byte ptr [esi + 31h], bl
loc_1_2E71:
        mov    al, bl
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2E80:
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_0081
        or     eax, eax
        je     NEAR PTR loc_1_302B
        db  0BFh, 058h, 078h, 000h, 000h
        db  0BBh, 084h, 078h, 000h, 000h
        db  0BDh, 0A0h, 078h, 000h, 000h
        db  039h, 03Dh, 058h, 078h, 000h, 000h
        je     SHORT loc_1_2EEB
loc_1_2EA8:
        db  08Bh, 035h, 058h, 078h, 000h, 000h
        sub    esi, 44h
        db  0A1h, 058h, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
        push   esi
        call   dword ptr [esi + 4]
        add    esp, 4
        test   byte ptr [esi + 3ch], 1
        je     SHORT loc_1_2EE3
        push   esi
        call   sub_1_3930
        add    esp, 4
        db  080h, 03Dh, 060h, 079h, 000h, 000h, 000h
        jne    SHORT loc_1_2EE3
        call   sub_1_39A0
loc_1_2EE3:
        db  039h, 03Dh, 058h, 078h, 000h, 000h
        jne    SHORT loc_1_2EA8
loc_1_2EEB:
        db  083h, 025h, 01Ch, 077h, 000h, 000h, 0FEh
        call   sub_1_42F0
        db  039h, 01Dh, 084h, 078h, 000h, 000h
        je     SHORT loc_1_2F55
        nop
loc_1_2F00:
        db  08Bh, 035h, 084h, 078h, 000h, 000h
        mov    byte ptr [esi - 2], 0
        sub    esi, 34h
        db  0A1h, 084h, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
        push   esi
        call   dword ptr [esi + 3ch]
        add    esp, 4
        cmp    al, 0ffh
        je     SHORT loc_1_2F4D
        mov    byte ptr [esi + 1], al
        test   byte ptr [esi], 80h
        je     SHORT loc_1_2F4D
        db  0C7h, 046h, 034h, 090h, 078h, 000h, 000h
        add    esi, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 094h, 078h, 000h, 000h
loc_1_2F4D:
        db  039h, 01Dh, 084h, 078h, 000h, 000h
        jne    SHORT loc_1_2F00
loc_1_2F55:
        call   sub_1_2630
        db  039h, 02Dh, 0A0h, 078h, 000h, 000h
        je     SHORT loc_1_2F8A
loc_1_2F62:
        db  08Bh, 035h, 0A0h, 078h, 000h, 000h
        sub    esi, 44h
        db  0A1h, 0A0h, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
        push   esi
        call   sub_1_0292
        add    esp, 4
        db  039h, 02Dh, 0A0h, 078h, 000h, 000h
        jne    SHORT loc_1_2F62
loc_1_2F8A:
        db  039h, 03Dh, 058h, 078h, 000h, 000h
        jne    NEAR PTR loc_1_2EA8
        db  0FFh, 00Dh, 014h, 077h, 000h, 000h
        call   sub_1_0099
        or     eax, eax
        je     NEAR PTR loc_1_302B
        db  081h, 03Dh, 0F8h, 078h, 000h, 000h, 0F8h, 078h, 000h, 000h
        je     SHORT loc_1_2FBA
        call   sub_1_3030
loc_1_2FBA:
        db  0BFh, 090h, 078h, 000h, 000h
        sub    ebx, ebx
loc_1_2FC1:
        db  039h, 03Dh, 090h, 078h, 000h, 000h
        mov    esi, ebx
        je     SHORT loc_1_2FE2
        db  08Bh, 035h, 090h, 078h, 000h, 000h
        sub    esi, 34h
        db  0A1h, 090h, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_2FE2:
        cmp    esi, ebx
        je     SHORT loc_1_3020
        cmp    byte ptr [esi + 33h], bl
        je     SHORT loc_1_3010
        db  0C7h, 046h, 034h, 0F8h, 078h, 000h, 000h
        add    esi, 34h
        db  0A1h, 0FCh, 078h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 0FCh, 078h, 000h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 0FCh, 078h, 000h, 000h
        jmp    SHORT loc_1_2FC1
        db  08Dh, 064h, 024h, 000h
loc_1_3010:
        push   esi
        call   sub_1_0151
        add    esp, 4
        jmp    SHORT loc_1_2FC1
        db  005h, 000h, 000h, 000h, 000h
loc_1_3020:
        db  0A1h, 018h, 077h, 000h, 000h
        dec    eax
        db  0A3h, 018h, 077h, 000h, 000h
loc_1_302B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_3030:
        push   ebx
        push   esi
        push   edi
        db  0A1h, 0F8h, 078h, 000h, 000h
        db  03Dh, 0F8h, 078h, 000h, 000h
        je     SHORT loc_1_3067
        sub    ebx, ebx
loc_1_3041:
        mov    edi, dword ptr [eax]
        cmp    byte ptr [eax - 1], bl
        lea    esi, [eax - 34h]
        jne    SHORT loc_1_305D
        push   eax
        call   sub_1_3B50
        add    esp, 4
        push   esi
        call   sub_1_0151
        add    esp, 4
loc_1_305D:
        mov    eax, edi
        db  081h, 0FFh, 0F8h, 078h, 000h, 000h
        jne    SHORT loc_1_3041
loc_1_3067:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_3070:
        call   sub_1_70E0
        db  0A1h, 090h, 078h, 000h, 000h
        db  03Dh, 090h, 078h, 000h, 000h
        je     SHORT loc_1_3093
loc_1_3081:
        mov    dl, byte ptr [eax - 33h]
        mov    byte ptr [eax - 3], dl
        lea    ecx, [eax - 34h]
        mov    eax, dword ptr [eax]
        db  03Dh, 090h, 078h, 000h, 000h
        jne    SHORT loc_1_3081
loc_1_3093:
        db  0C7h, 005h, 090h, 078h, 000h, 000h, 090h, 078h, 000h, 000h
        db  0C7h, 005h, 094h, 078h, 000h, 000h, 090h, 078h, 000h, 000h
        call   sub_1_73F0
        sub    al, al
        ret
        db  090h
sub_1_30B0:
        mov    al, 24h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_30C0:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 10h]
        mov    esi, dword ptr [eax + 4]
        cmp    byte ptr [esi + 1], 0ffh
        je     SHORT loc_1_30D6
        mov    al, 24h
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_30D6:
        mov    al, byte ptr [esi]
        and    eax, 7fh
        cmp    eax, 33h
        jg     SHORT loc_1_30F3
        je     SHORT loc_1_3131
        sub    eax, 10h
        cmp    eax, 13h
        ja     SHORT loc_1_30FD
        shl    eax, 2
        db  0FFh, 0A0h, 0CCh, 031h, 000h, 000h
loc_1_30F3:
        cmp    eax, 71h
        jl     SHORT loc_1_30FD
        cmp    eax, 72h
        jle    SHORT loc_1_3119
loc_1_30FD:
        mov    bl, 26h
        jmp    SHORT loc_1_313C
loc_1_3101:
        push   esi
        call   sub_1_0450
        jmp    SHORT loc_1_3137
loc_1_3109:
        push   esi
        call   sub_1_0410
        jmp    SHORT loc_1_3137
loc_1_3111:
        push   esi
        call   sub_1_30B0
        jmp    SHORT loc_1_3137
loc_1_3119:
        push   esi
        call   sub_1_3E50
        jmp    SHORT loc_1_3137
loc_1_3121:
        push   esi
        call   sub_1_4830
        jmp    SHORT loc_1_3137
loc_1_3129:
        push   esi
        call   sub_1_23B0
        jmp    SHORT loc_1_3137
loc_1_3131:
        push   esi
        call   sub_1_6660
loc_1_3137:
        add    esp, 4
        mov    bl, al
loc_1_313C:
        cmp    bl, 26h
        jne    SHORT loc_1_315D
        cmp    byte ptr [esi + 32h], 1
        jne    SHORT loc_1_315D
        mov    byte ptr [esi + 32h], 0
        lea    eax, [esi + 34h]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [esi + 1], 0bh
        sub    bl, bl
loc_1_315D:
        or     bl, bl
        jne    SHORT loc_1_31C3
        test   byte ptr [esi], 80h
        je     SHORT loc_1_3185
        db  0C7h, 046h, 034h, 090h, 078h, 000h, 000h
        add    esi, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 094h, 078h, 000h, 000h
loc_1_3185:
        db  0BFh, 090h, 078h, 000h, 000h
        lea    ebx, [ebx]
loc_1_3190:
        db  039h, 03Dh, 090h, 078h, 000h, 000h
        mov    esi, 0
        je     SHORT loc_1_31B4
        db  08Bh, 035h, 090h, 078h, 000h, 000h
        sub    esi, 34h
        db  0A1h, 090h, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_31B4:
        or     esi, esi
        je     SHORT loc_1_31C3
        push   esi
        call   sub_1_0151
        add    esp, 4
        jmp    SHORT loc_1_3190
loc_1_31C3:
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_31CC:
        add    dword ptr [ecx], esi
        add    byte ptr [eax], al
        or     dword ptr [ecx], esi
        add    byte ptr [eax], al
        adc    dword ptr [ecx], esi
        add    byte ptr [eax], al
        std
        xor    byte ptr [eax], al
        db  000h, 019h
        xor    dword ptr [eax], eax
        db  000h, 021h
        xor    dword ptr [eax], eax
        db  000h, 021h
        xor    dword ptr [eax], eax
        db  000h, 019h
        xor    dword ptr [eax], eax
        db  000h, 0FDh
        xor    byte ptr [eax], al
        db  000h, 0FDh
        xor    byte ptr [eax], al
        db  000h, 0FDh
        xor    byte ptr [eax], al
        db  000h, 0FDh
        xor    byte ptr [eax], al
        db  000h, 0FDh
        xor    byte ptr [eax], al
        db  000h, 0FDh
        xor    byte ptr [eax], al
        db  000h, 0FDh
        xor    byte ptr [eax], al
        db  000h, 0FDh
        xor    byte ptr [eax], al
        db  000h, 0FDh
        xor    byte ptr [eax], al
        db  000h, 029h
        xor    dword ptr [eax], eax
        db  000h, 0FDh
        xor    byte ptr [eax], al
        db  000h, 029h
        xor    dword ptr [eax], eax
        add    byte ptr [ebp - 4fffdb9ch], cl
        add    eax, ebx
        lea    ebx, [ebx]
        lea    esp, [esp]
loc_1_3230:
        sub    al, al
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3240:
        push   esi
        db  0A1h, 080h, 078h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_3273
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx]
loc_1_3250:
        cmp    dword ptr [eax], esi
        jne    SHORT loc_1_326C
        mov    ecx, dword ptr [edx + 4]
        cmp    dword ptr [eax + 4], ecx
        jne    SHORT loc_1_326C
        mov    ecx, dword ptr [edx + 8]
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_1_326C
        mov    ecx, dword ptr [edx + 0ch]
        cmp    dword ptr [eax + 0ch], ecx
        je     SHORT loc_1_3273
loc_1_326C:
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_1_3250
loc_1_3273:
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3280:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        db  08Ah, 01Dh, 074h, 079h, 000h, 000h
        mov    cl, byte ptr [esi + 1ah]
        lea    eax, [esi + 1ah]
        cmp    cl, 2ah
        je     NEAR PTR loc_1_3332
        or     cl, cl
        je     NEAR PTR loc_1_3332
        push   eax
        call   sub_1_3240
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_32B7
        mov    al, 0dh
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_32B7:
        sub    ecx, ecx
        db  08Bh, 03Dh, 084h, 079h, 000h, 000h
        cmp    edi, ecx
        db  08Bh, 015h, 0F4h, 078h, 000h, 000h
        je     SHORT loc_1_32DD
        lea    eax, [edx + 14h]
        lea    esp, [esp]
loc_1_32D0:
        test   byte ptr [eax], 40h
        je     SHORT loc_1_32DD
        add    eax, 44h
        inc    ecx
        cmp    edi, ecx
        ja     SHORT loc_1_32D0
loc_1_32DD:
        cmp    edi, ecx
        ja     SHORT loc_1_32E7
        mov    al, 0eh
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_32E7:
        db  08Bh, 03Dh, 050h, 078h, 000h, 000h
        lea    ecx, [ecx]
loc_1_32F0:
        inc    bl
        cmp    bl, 0ffh
        jne    SHORT loc_1_32F9
        mov    bl, 1
loc_1_32F9:
        xor    eax, eax
        mov    al, bl
        cmp    byte ptr [edi + eax], 0ffh
        jne    SHORT loc_1_32F0
        xor    eax, eax
        mov    al, bl
        mov    byte ptr [edi + eax], cl
        mov    eax, ecx
        shl    eax, 4
        add    eax, ecx
        lea    eax, [edx + eax*4]
        mov    byte ptr [esi + 3], bl
        or     byte ptr [eax + 14h], 40h
        mov    byte ptr [eax + 15h], bl
        db  088h, 01Dh, 074h, 079h, 000h, 000h
        push   eax
        push   esi
        call   sub_1_3420
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_3332:
        mov    al, 15h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3340:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        add    esi, 1ah
        push   esi
        call   sub_1_3240
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_1_3412
        test   byte ptr [edi + 14h], 4
        jne    SHORT loc_1_3370
        mov    bl, 15h
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_3370:
        push   edi
        call   sub_1_3760
        add    esp, 4
        mov    bl, al
        xor    ecx, ecx
        mov    cl, byte ptr [edi + 15h]
        db  0A1h, 050h, 078h, 000h, 000h
        mov    byte ptr [ecx + eax], 0ffh
        call   sub_1_37F0
        or     eax, eax
        je     SHORT loc_1_3404
        mov    word ptr [eax + 28h], 50h
        mov    dword ptr [eax + 1ch], 0ffffffffh
        mov    word ptr [eax + 20h], 0ffffh
        lea    edx, [eax + 4ch]
        db  080h, 03Dh, 014h, 078h, 000h, 000h, 001h
        sbb    cl, cl
        and    cl, 0f0h
        add    cl, 14h
        mov    byte ptr [edx + 5], cl
        db  08Bh, 00Dh, 020h, 079h, 000h, 000h
        mov    dword ptr [edx + 6], ecx
        mov    dword ptr [edx + 0ah], 0ffffffffh
        mov    word ptr [edx + 0eh], 0ffffh
        mov    byte ptr [edx + 3fh], 5
        mov    byte ptr [edx + 1fh], 5
        add    edx, 1eh
        mov    cl, byte ptr [edi + 14h]
        mov    byte ptr [edx + 20h], cl
        mov    ecx, dword ptr [esi]
        mov    dword ptr [edx + 22h], ecx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [edx + 26h], ecx
        add    edx, 22h
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [esi + 0ch]
        mov    dword ptr [edx + 0ch], ecx
        push   eax
        call   sub_1_0292
        add    esp, 4
loc_1_3404:
        or     bl, bl
        jne    SHORT loc_1_3414
        mov    byte ptr [edi + 14h], 0
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_3412:
        mov    bl, 15h
loc_1_3414:
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3420:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi + 1ah]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 1eh]
        lea    ebx, [esi + 1ah]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [edi + 0ch], eax
        mov    al, byte ptr [esi]
        and    eax, 7fh
        cmp    eax, 36h
        jne    SHORT loc_1_3453
        or     byte ptr [edi + 14h], 80h
loc_1_3453:
        lea    eax, [edi + 18h]
        mov    dword ptr [eax], eax
        mov    dword ptr [edi + 1ch], eax
        lea    eax, [edi + 20h]
        mov    dword ptr [eax], eax
        mov    dword ptr [edi + 24h], eax
        lea    eax, [edi + 28h]
        mov    dword ptr [eax], eax
        mov    dword ptr [edi + 2ch], eax
        lea    eax, [edi + 30h]
        mov    dword ptr [eax], eax
        mov    dword ptr [edi + 34h], eax
        mov    dword ptr [edi + 38h], 0
        db  0A0h, 054h, 078h, 000h, 000h
        mov    byte ptr [edi + 17h], al
        mov    dword ptr [edi + 3ch], esi
        call   sub_1_37F0
        or     eax, eax
        jne    SHORT loc_1_34B0
        xor    ecx, ecx
        mov    cl, byte ptr [edi + 15h]
        db  0A1h, 050h, 078h, 000h, 000h
        mov    byte ptr [ecx + eax], 0ffh
        mov    byte ptr [edi + 14h], 0
        mov    al, 9
        mov    byte ptr [esi + 1], 9
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_34B0:
        db  08Bh, 00Dh, 080h, 078h, 000h, 000h
        mov    dword ptr [edi + 10h], ecx
        db  089h, 03Dh, 080h, 078h, 000h, 000h
        db  0C7h, 040h, 004h, 0E0h, 035h, 000h, 000h
        lea    edx, [eax + 4ch]
        mov    dword ptr [eax + 1ch], 0ffffffffh
        mov    word ptr [eax + 20h], 0ffffh
        mov    cl, byte ptr [edi + 15h]
        and    ecx, 0ffff00ffh
        or     word ptr [eax + 3ch], 100h
        mov    word ptr [eax + 3eh], cx
        db  080h, 03Dh, 014h, 078h, 000h, 000h, 001h
        sbb    cl, cl
        and    cl, 0f0h
        add    cl, 14h
        mov    byte ptr [edx + 5], cl
        db  08Bh, 00Dh, 020h, 079h, 000h, 000h
        mov    dword ptr [edx + 6], ecx
        mov    dword ptr [edx + 0ah], 0ffffffffh
        mov    word ptr [edx + 0eh], 0ffffh
        mov    byte ptr [edx + 3fh], 3
        mov    byte ptr [edx + 1fh], 3
        add    edx, 1eh
        mov    cl, byte ptr [edi + 14h]
        mov    byte ptr [edx + 20h], cl
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [edx + 22h], ecx
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [edx + 26h], ecx
        add    edx, 22h
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [edx + 0ch], ecx
        mov    word ptr [eax + 28h], 50h
        push   eax
        call   sub_1_0292
        add    esp, 4
        mov    al, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3560:
        push   esi
        db  0A1h, 050h, 078h, 000h, 000h
        db  08Bh, 035h, 0F4h, 078h, 000h, 000h
        mov    byte ptr [eax + 1], 0
        mov    byte ptr [esi + 15h], 1
        mov    byte ptr [esi + 14h], 44h
        push   0
        push   0ah
        push   esi
        call   sub_1_0192
        add    esp, 0ch
        db  0B8h, 024h, 079h, 000h, 000h
        lea    edx, [esi + 0ah]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        lea    eax, [esi + 18h]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 1ch], eax
        lea    eax, [esi + 20h]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 24h], eax
        lea    eax, [esi + 28h]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 2ch], eax
        lea    eax, [esi + 30h]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 34h], eax
        mov    dword ptr [esi + 38h], 0
        db  0A1h, 080h, 078h, 000h, 000h
        mov    dword ptr [esi + 10h], eax
        db  089h, 035h, 080h, 078h, 000h, 000h
        mov    dword ptr [esi + 3ch], 0
        pop    esi
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_35E0:
        push   esi
        mov    edx, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [edx + 3eh]
        db  0A1h, 050h, 078h, 000h, 000h
        mov    cl, byte ptr [ecx + eax]
        and    ecx, 0ffh
        db  039h, 00Dh, 084h, 079h, 000h, 000h
        jbe    SHORT loc_1_3629
        mov    esi, ecx
        shl    esi, 4
        db  0A1h, 0F4h, 078h, 000h, 000h
        add    esi, ecx
        dec    byte ptr [eax + esi*4 + 17h]
        db  0C7h, 042h, 004h, 040h, 036h, 000h, 000h
        db  0A1h, 048h, 079h, 000h, 000h
        push   eax
        push   edx
        call   sub_1_3BE0
        add    esp, 8
        pop    esi
        ret
loc_1_3629:
        push   edx
        call   sub_1_3820
        add    esp, 4
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3640:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0ch]
        xor    ecx, ecx
        lea    esi, [edx + 6ah]
        mov    cx, word ptr [edx + 3eh]
        db  0A1h, 050h, 078h, 000h, 000h
        mov    al, byte ptr [ecx + eax]
        and    eax, 0ffh
        db  039h, 005h, 084h, 079h, 000h, 000h
        jbe    SHORT loc_1_36D4
        mov    ecx, eax
        shl    ecx, 4
        add    ecx, eax
        db  0A1h, 0F4h, 078h, 000h, 000h
        lea    edi, [eax + ecx*4]
        mov    al, byte ptr [edi + 14h]
        mov    cl, al
        and    cl, 0c7h
        cmp    byte ptr [esi + 20h], cl
        jne    SHORT loc_1_36D4
        cmp    byte ptr [edi + 17h], 0
        je     SHORT loc_1_3699
        db  0C7h, 042h, 004h, 0E0h, 035h, 000h, 000h
        push   edx
        call   sub_1_0292
        add    esp, 4
        pop    edi
        pop    esi
        ret
loc_1_3699:
        or     al, 4
        mov    byte ptr [edi + 14h], al
        mov    ecx, dword ptr [edi + 3ch]
        or     ecx, ecx
        je     SHORT loc_1_36D4
        mov    dword ptr [edi + 3ch], 0
        mov    byte ptr [ecx + 1], 0
        test   byte ptr [ecx], 80h
        je     SHORT loc_1_36D4
        db  0C7h, 041h, 034h, 090h, 078h, 000h, 000h
        add    ecx, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 094h, 078h, 000h, 000h
loc_1_36D4:
        push   edx
        call   sub_1_3820
        add    esp, 4
        pop    edi
        pop    esi
        ret
sub_1_36E0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esi + 38h]
        cmp    ecx, edx
        jne    SHORT loc_1_36F7
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi + 38h], eax
        jmp    SHORT loc_1_370F
loc_1_36F7:
        or     edx, edx
        je     SHORT loc_1_370F
loc_1_36FB:
        mov    eax, dword ptr [edx]
        cmp    ecx, eax
        je     SHORT loc_1_3707
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_36FB
loc_1_3707:
        or     edx, edx
        je     SHORT loc_1_370F
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
loc_1_370F:
        cmp    dword ptr [esi + 38h], 0
        jne    SHORT loc_1_371F
        cmp    byte ptr [esi + 14h], 1
        jne    SHORT loc_1_371F
        mov    byte ptr [esi + 14h], 0
loc_1_371F:
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0C3h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0C3h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0C3h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3760:
        push   ebx
        push   esi
        db  0A1h, 080h, 078h, 000h, 000h
        mov    edx, eax
        or     eax, eax
        mov    esi, dword ptr [esp + 0ch]
        je     SHORT loc_1_377E
loc_1_3771:
        mov    ecx, dword ptr [edx + 10h]
        cmp    esi, ecx
        je     SHORT loc_1_377E
        mov    edx, ecx
        or     edx, edx
        jne    SHORT loc_1_3771
loc_1_377E:
        cmp    esi, eax
        jne    SHORT loc_1_378C
        mov    eax, dword ptr [eax + 10h]
        db  0A3h, 080h, 078h, 000h, 000h
        jmp    SHORT loc_1_3796
loc_1_378C:
        or     edx, edx
        je     SHORT loc_1_3796
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [edx + 10h], eax
loc_1_3796:
        mov    al, byte ptr [esi + 14h]
        and    al, 0fbh
        mov    byte ptr [esi + 14h], al
        or     al, 1
        mov    byte ptr [esi + 14h], al
        and    al, 2
        cmp    al, 1
        sbb    bl, bl
        and    bl, 0feh
        add    bl, 19h
        push   ebx
        lea    eax, [esi + 28h]
        push   eax
        call   sub_1_7080
        add    esp, 8
        push   ebx
        lea    eax, [esi + 30h]
        push   eax
        call   sub_1_7080
        add    esp, 8
        push   ebx
        lea    eax, [esi + 18h]
        push   eax
        call   sub_1_7080
        add    esp, 8
        push   ebx
        lea    eax, [esi + 20h]
        push   eax
        call   sub_1_7080
        add    esp, 8
        sub    al, al
        cmp    dword ptr [esi + 38h], 0
        je     SHORT loc_1_37ED
        mov    al, 0fh
loc_1_37ED:
        pop    esi
        pop    ebx
        ret
sub_1_37F0:
        db  0A1h, 054h, 079h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_3814
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 054h, 079h, 000h, 000h
        mov    word ptr [eax + 3ch], 2
        mov    dword ptr [eax], 0
        db  0C7h, 040h, 004h, 020h, 03Bh, 000h, 000h
loc_1_3814:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3820:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 3ch], 80h
        je     SHORT loc_1_3836
        push   0
        push   eax
        call   sub_1_01FA
        add    esp, 8
        ret
loc_1_3836:
        mov    byte ptr [eax + 8], 0
        mov    word ptr [eax + 3ch], 0
        mov    word ptr [eax + 22h], 1
        db  08Bh, 00Dh, 054h, 079h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 054h, 079h, 000h, 000h
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3860:
        db  0A1h, 040h, 079h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_387D
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 040h, 079h, 000h, 000h
        mov    word ptr [eax + 3ch], 1
        mov    dword ptr [eax], 0
loc_1_387D:
        ret
        db  08Bh, 0FFh
sub_1_3880:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 040h, 079h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_38F0
        db  08Bh, 01Dh, 0C8h, 078h, 000h, 000h
        cmp    ebx, 3
        jb     SHORT loc_1_38F0
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 040h, 079h, 000h, 000h
        mov    word ptr [eax + 3ch], 1
        db  0C7h, 040h, 004h, 010h, 03Ah, 000h, 000h
        lea    esi, [eax + 2ah]
        db  08Bh, 03Dh, 034h, 079h, 000h, 000h
        db  066h, 08Bh, 015h, 08Ch, 078h, 000h, 000h
        sub    dx, 8
        mov    ecx, 3
loc_1_38C6:
        lea    ebp, [edi + 8]
        mov    dword ptr [esi], ebp
        mov    word ptr [esi + 4], dx
        inc    word ptr [eax + 22h]
        mov    edi, dword ptr [edi]
        add    esi, 6
        dec    ecx
        jne    SHORT loc_1_38C6
        db  089h, 03Dh, 034h, 079h, 000h, 000h
        sub    ebx, 3
        db  089h, 01Dh, 0C8h, 078h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_38F0:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3900:
        mov    ecx, dword ptr [esp + 4]
        mov    word ptr [ecx + 3ch], 0
        mov    byte ptr [ecx + 8], 0
        mov    word ptr [ecx + 22h], 1
        mov    word ptr [ecx + 28h], 58h
        db  0A1h, 040h, 079h, 000h, 000h
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 040h, 079h, 000h, 000h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3930:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    word ptr [edx + 3ch], 0
        db  0A1h, 040h, 079h, 000h, 000h
        mov    dword ptr [edx], eax
        db  089h, 015h, 040h, 079h, 000h, 000h
        mov    ax, word ptr [edx + 22h]
        cmp    ax, 1
        jbe    SHORT loc_1_399C
        dec    ax
        db  08Bh, 035h, 034h, 079h, 000h, 000h
        mov    ecx, eax
        and    ecx, 0ffffh
        db  0A1h, 0C8h, 078h, 000h, 000h
        add    eax, ecx
        db  0A3h, 0C8h, 078h, 000h, 000h
        mov    edi, edi
loc_1_3970:
        mov    cx, word ptr [edx + 22h]
        mov    eax, ecx
        and    eax, 0ffffh
        dec    cx
        lea    eax, [eax + eax*2]
        mov    eax, dword ptr [edx + eax*2 + 1eh]
        mov    dword ptr [eax - 8], esi
        sub    eax, 8
        mov    esi, eax
        db  089h, 035h, 034h, 079h, 000h, 000h
        mov    word ptr [edx + 22h], cx
        cmp    cx, 1
        ja     SHORT loc_1_3970
loc_1_399C:
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_39A0:
        push   esi
        call   sub_1_3880
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_39B0
        sub    eax, eax
        pop    esi
        ret
loc_1_39B0:
        push   esi
        call   sub_1_02B0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_39CA
        push   esi
        call   sub_1_3930
        add    esp, 4
        sub    eax, eax
        pop    esi
        ret
loc_1_39CA:
        db  0A1h, 04Ch, 079h, 000h, 000h
        inc    eax
        db  0A3h, 04Ch, 079h, 000h, 000h
        mov    eax, 1
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_39E0:
        mov    eax, dword ptr [esp + 4]
        mov    cx, word ptr [eax + 3ch]
        test   cl, 2
        je     SHORT loc_1_39F7
        push   eax
        call   sub_1_3820
        add    esp, 4
        ret
loc_1_39F7:
        test   cl, 1
        je     SHORT loc_1_3A05
        push   eax
        call   sub_1_3930
        add    esp, 4
loc_1_3A05:
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3A10:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    al, byte ptr [esi + 9]
        or     al, al
        jne    NEAR PTR loc_1_3AF8
        db  0A1h, 01Ch, 077h, 000h, 000h
        or     eax, 1
        db  0A3h, 01Ch, 077h, 000h, 000h
        db  0A1h, 078h, 079h, 000h, 000h
        inc    eax
        db  0A3h, 078h, 079h, 000h, 000h
        cmp    word ptr [esi + 68h], 5504h
        lea    edi, [esi + 4ch]
        je     SHORT loc_1_3A68
        db  0FFh, 005h, 044h, 078h, 000h, 000h
        push   esi
        call   sub_1_3930
        add    esp, 4
        db  080h, 03Dh, 060h, 079h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_3B1D
        call   sub_1_39A0
        pop    edi
        pop    esi
        ret
loc_1_3A68:
        mov    ax, word ptr [edi + 2]
        push   eax
        call   sub_1_0187
        add    esp, 4
        db  080h, 03Dh, 060h, 079h, 000h, 000h, 000h
        jne    SHORT loc_1_3A89
        db  0A1h, 04Ch, 079h, 000h, 000h
        dec    eax
        db  0A3h, 04Ch, 079h, 000h, 000h
loc_1_3A89:
        cmp    byte ptr [edi + 5], 14h
        jne    SHORT loc_1_3A94
        mov    al, byte ptr [edi + 3fh]
        jmp    SHORT loc_1_3A97
loc_1_3A94:
        mov    al, byte ptr [edi + 1fh]
loc_1_3A97:
        dec    al
        cmp    al, 0ch
        jae    SHORT loc_1_3AD3
        and    eax, 0ffh
        db  08Bh, 004h, 085h, 0E4h, 076h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0C7h, 046h, 044h, 058h, 078h, 000h, 000h
        add    esi, 44h
        db  0A1h, 05Ch, 078h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 05Ch, 078h, 000h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 05Ch, 078h, 000h, 000h
        call   sub_1_2E80
        pop    edi
        pop    esi
        ret
loc_1_3AD3:
        db  0FFh, 005h, 044h, 078h, 000h, 000h
        push   esi
        call   sub_1_3930
        add    esp, 4
        db  080h, 03Dh, 060h, 079h, 000h, 000h, 000h
        jne    SHORT loc_1_3AF0
        call   sub_1_39A0
loc_1_3AF0:
        call   sub_1_2E80
        pop    edi
        pop    esi
        ret
loc_1_3AF8:
        cmp    al, 0fch
        je     SHORT loc_1_3B12
        cmp    al, 0ffh
        je     SHORT loc_1_3B12
        db  0FFh, 005h, 044h, 078h, 000h, 000h
        push   esi
        call   sub_1_3930
        add    esp, 4
        pop    edi
        pop    esi
        ret
loc_1_3B12:
        db  0A1h, 04Ch, 079h, 000h, 000h
        dec    eax
        db  0A3h, 04Ch, 079h, 000h, 000h
loc_1_3B1D:
        pop    edi
        pop    esi
        ret
loc_1_3B20:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_3820
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_1_3B30:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [ecx], eax
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3B50:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
        ret
        db  090h
sub_1_3B60:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 18h]
        and    ecx, 0ffffh
        mov    esi, dword ptr [esp + 14h]
        xor    ebx, ebx
        mov    bx, word ptr [esi + 4]
        mov    di, word ptr [esp + 20h]
        sub    ebx, ecx
        mov    eax, edi
        and    eax, 0ffffh
        cmp    ebx, eax
        jl     SHORT loc_1_3B8C
        mov    ebx, eax
loc_1_3B8C:
        or     bx, bx
        je     SHORT loc_1_3BA6
        mov    ebp, dword ptr [esp + 1ch]
        push   ebx
        push   ebp
        mov    eax, dword ptr [esi]
        add    eax, ecx
        push   eax
        call   sub_1_01BB
        add    esp, 0ch
        jmp    SHORT loc_1_3BAA
loc_1_3BA6:
        mov    ebp, dword ptr [esp + 1ch]
loc_1_3BAA:
        sub    di, bx
        je     SHORT loc_1_3BDA
        nop
loc_1_3BB0:
        add    esi, 6
        mov    eax, ebx
        and    eax, 0ffffh
        add    ebp, eax
        mov    bx, word ptr [esi + 4]
        cmp    bx, di
        jb     SHORT loc_1_3BC8
        mov    bx, di
loc_1_3BC8:
        push   ebx
        push   ebp
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_01BB
        add    esp, 0ch
        sub    di, bx
        jne    SHORT loc_1_3BB0
loc_1_3BDA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_3BE0:
        push   esi
        db  08Bh, 035h, 064h, 078h, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        add    esi, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 10h], esi
        or     word ptr [eax + 3ch], 8
        db  08Bh, 015h, 030h, 079h, 000h, 000h
        mov    ecx, edx
        sub    ecx, 44h
        db  081h, 0FAh, 02Ch, 079h, 000h, 000h
        je     SHORT loc_1_3C1D
loc_1_3C0A:
        cmp    dword ptr [ecx + 10h], esi
        jbe    SHORT loc_1_3C1D
        mov    edx, dword ptr [edx + 4]
        db  081h, 0FAh, 02Ch, 079h, 000h, 000h
        lea    ecx, [edx - 44h]
        jne    SHORT loc_1_3C0A
loc_1_3C1D:
        push   edx
        add    eax, 44h
        push   eax
        call   sub_1_3B30
        add    esp, 8
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_3C30:
        push   esi
        mov    esi, dword ptr [esp + 8]
        lea    eax, [esi + 44h]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        and    word ptr [esi + 3ch], 0fff7h
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3C50:
        push   ebx
        push   esi
        push   edi
        db  0FFh, 005h, 064h, 078h, 000h, 000h
        call   sub_1_0081
        or     eax, eax
        jne    SHORT loc_1_3CD2
        db  08Bh, 00Dh, 02Ch, 079h, 000h, 000h
        mov    esi, ecx
        sub    esi, 44h
        db  081h, 0F9h, 02Ch, 079h, 000h, 000h
        je     NEAR PTR loc_1_3D45
        db  0A1h, 064h, 078h, 000h, 000h
        db  0BFh, 02Ch, 079h, 000h, 000h
loc_1_3C83:
        cmp    dword ptr [esi + 10h], eax
        ja     NEAR PTR loc_1_3D45
        and    word ptr [esi + 3ch], 0fff7h
        push   ecx
        call   sub_1_3B50
        add    esp, 4
        db  0C7h, 046h, 044h, 058h, 078h, 000h, 000h
        add    esi, 44h
        db  0A1h, 05Ch, 078h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 05Ch, 078h, 000h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 05Ch, 078h, 000h, 000h
        db  08Bh, 00Dh, 02Ch, 079h, 000h, 000h
        mov    esi, ecx
        sub    esi, 44h
        cmp    ecx, edi
        db  0A1h, 064h, 078h, 000h, 000h
        jne    SHORT loc_1_3C83
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_3CD2:
        sub    edi, edi
        db  08Bh, 00Dh, 02Ch, 079h, 000h, 000h
        mov    esi, ecx
        sub    esi, 44h
        db  081h, 0F9h, 02Ch, 079h, 000h, 000h
        je     SHORT loc_1_3D2C
        db  0A1h, 064h, 078h, 000h, 000h
        db  0BBh, 02Ch, 079h, 000h, 000h
loc_1_3CF1:
        cmp    dword ptr [esi + 10h], eax
        ja     SHORT loc_1_3D2C
        push   ecx
        call   sub_1_3B50
        add    esp, 4
        and    word ptr [esi + 3ch], 0fff7h
        mov    ecx, dword ptr [esi + 4]
        or     ecx, ecx
        db  0A1h, 064h, 078h, 000h, 000h
        je     SHORT loc_1_3D1C
        push   esi
        call   ecx
        add    esp, 4
        db  0A1h, 064h, 078h, 000h, 000h
loc_1_3D1C:
        inc    edi
        db  08Bh, 00Dh, 02Ch, 079h, 000h, 000h
        mov    esi, ecx
        sub    esi, 44h
        cmp    ecx, ebx
        jne    SHORT loc_1_3CF1
loc_1_3D2C:
        call   sub_1_2C20
        db  0A1h, 014h, 077h, 000h, 000h
        dec    eax
        db  0A3h, 014h, 077h, 000h, 000h
        or     edi, edi
        je     SHORT loc_1_3D45
        call   sub_1_2E80
loc_1_3D45:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3D50:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        xor    ecx, ecx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    cl, byte ptr [esi + 2]
        mov    ecx, dword ptr [eax + ecx*4]
        or     ecx, ecx
        jne    SHORT loc_1_3D70
        mov    al, 8
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_3D70:
        cmp    byte ptr [ecx + 6], 3
        je     SHORT loc_1_3D80
        mov    al, 8
        pop    edi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_3D80:
        lea    eax, [esi + 34h]
        lea    edx, [ecx + 5ch]
        mov    dword ptr [eax], edx
        mov    edi, dword ptr [edx + 4]
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [edi], eax
        mov    dword ptr [edx + 4], eax
        mov    byte ptr [esi + 32h], 7
        mov    al, byte ptr [esi]
        and    eax, 7fh
        cmp    eax, 17h
        jne    SHORT loc_1_3DC0
        xor    edx, edx
        mov    dx, word ptr [esi + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 0ah]
        add    eax, edx
        cmp    eax, 0ffffh
        jbe    SHORT loc_1_3DC6
        mov    al, 1
        pop    edi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_3DC0:
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
loc_1_3DC6:
        cmp    byte ptr [ecx + 7], 0
        jne    SHORT loc_1_3DE4
        mov    byte ptr [ecx + 7], 1
        db  08Bh, 015h, 094h, 079h, 000h, 000h
        mov    word ptr [ecx + 28h], dx
        mov    word ptr [ecx + 16h], ax
        mov    word ptr [ecx + 18h], 0
loc_1_3DE4:
        mov    ax, word ptr [ecx + 4]
        test   al, 40h
        jne    SHORT loc_1_3E13
        or     ax, 40h
        mov    word ptr [ecx + 4], ax
        db  0C7h, 041h, 008h, 0D4h, 078h, 000h, 000h
        add    ecx, 8
        db  0A1h, 0D8h, 078h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 0D8h, 078h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 0D8h, 078h, 000h, 000h
loc_1_3E13:
        mov    al, 0ffh
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3E20:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_3D50
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_3E30:
        mov    al, 3
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3E40:
        mov    al, 3
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3E50:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 32h], 7
        jne    SHORT loc_1_3E8B
        lea    eax, [esi + 34h]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [esi + 32h], 0
        xor    ecx, ecx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    cl, byte ptr [esi + 2]
        mov    eax, dword ptr [eax + ecx*4]
        mov    byte ptr [esi + 1], 0bh
        push   18h
        push   eax
        call   sub_1_07A0
        add    esp, 8
        sub    al, al
        pop    esi
        ret
loc_1_3E8B:
        mov    al, 26h
        pop    esi
        ret
        db  090h
sub_1_3E90:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_37F0
        or     eax, eax
        jne    SHORT loc_1_3EB0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3EB0:
        mov    edx, dword ptr [esp + 20h]
        lea    ecx, [eax + 4ch]
        mov    edi, dword ptr [edx + 5ch]
        sub    edi, 34h
        mov    dword ptr [esp + 14h], ecx
        lea    ebx, [ecx + 1eh]
        mov    dword ptr [esp + 10h], ebx
        mov    dword ptr [eax + 40h], edi
        inc    byte ptr [edi + 33h]
        mov    cl, byte ptr [edx + 10h]
        and    ecx, 0ffff00ffh
        mov    word ptr [eax + 3eh], cx
        mov    word ptr [eax + 28h], 30h
        lea    ebx, [eax + 1ch]
        mov    ecx, dword ptr [edx + 36h]
        mov    dword ptr [ebx], ecx
        mov    cx, word ptr [edx + 3ah]
        mov    word ptr [ebx + 4], cx
        lea    esi, [edx + 36h]
        mov    cl, byte ptr [edi]
        and    ecx, 7fh
        cmp    ecx, 17h
        jne    NEAR PTR loc_1_3FB0
        mov    si, word ptr [edi + 8]
        mov    cx, word ptr [edx + 18h]
        cmp    si, cx
        jbe    SHORT loc_1_3F76
        mov    word ptr [eax + 22h], 2
        and    ecx, 0ffffh
        xor    ebx, ebx
        mov    bx, word ptr [edx + 9ah]
        and    esi, 0ffffh
        sub    esi, ecx
        cmp    esi, ebx
        jl     SHORT loc_1_3F32
        mov    esi, ebx
loc_1_3F32:
        mov    word ptr [eax + 2eh], si
        mov    ebp, dword ptr [edi + 4]
        add    ebp, ecx
        mov    dword ptr [eax + 2ah], ebp
        cmp    si, word ptr [edx + 9ah]
        jae    NEAR PTR loc_1_3FE2
        mov    word ptr [eax + 22h], 3
        xor    ecx, ecx
        mov    cx, word ptr [edi + 0ah]
        mov    ebp, esi
        and    ebp, 0ffffh
        sub    ebx, ebp
        cmp    ecx, ebx
        jl     SHORT loc_1_3F67
        mov    ecx, ebx
loc_1_3F67:
        mov    word ptr [eax + 34h], cx
        add    si, cx
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 30h], ecx
        jmp    SHORT loc_1_3FE2
loc_1_3F76:
        mov    ebp, ecx
        and    ebp, 0ffffh
        mov    ebx, ebp
        mov    ecx, esi
        and    ecx, 0ffffh
        sub    ebx, ecx
        mov    word ptr [eax + 22h], 2
        xor    esi, esi
        mov    si, word ptr [edx + 16h]
        xor    ecx, ecx
        mov    cx, word ptr [edx + 9ah]
        sub    esi, ebp
        cmp    esi, ecx
        jl     SHORT loc_1_3FA7
        mov    esi, ecx
loc_1_3FA7:
        mov    word ptr [eax + 2eh], si
        mov    ecx, dword ptr [edi + 0ch]
        jmp    SHORT loc_1_3FDD
loc_1_3FB0:
        mov    word ptr [eax + 22h], 2
        lea    ecx, [edx + 18h]
        xor    ebx, ebx
        mov    bx, word ptr [edx + 18h]
        xor    esi, esi
        mov    si, word ptr [edx + 16h]
        xor    ecx, ecx
        mov    cx, word ptr [edx + 9ah]
        sub    esi, ebx
        cmp    esi, ecx
        jl     SHORT loc_1_3FD6
        mov    esi, ecx
loc_1_3FD6:
        mov    word ptr [eax + 2eh], si
        mov    ecx, dword ptr [edi + 4]
loc_1_3FDD:
        add    ecx, ebx
        mov    dword ptr [eax + 2ah], ecx
loc_1_3FE2:
        mov    ecx, dword ptr [esp + 14h]
        lea    ebx, [edx + 3ch]
        mov    byte ptr [ecx + 5], 4
        mov    edi, ecx
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [edi + 6], ecx
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [edi + 0ah], ecx
        add    edi, 6
        mov    cx, word ptr [ebx + 8]
        mov    ebx, dword ptr [esp + 10h]
        mov    word ptr [edi + 8], cx
        mov    byte ptr [ebx + 1], 6
        mov    cx, word ptr [edx + 10h]
        mov    word ptr [ebx + 2], cx
        mov    cx, word ptr [edx + 12h]
        mov    word ptr [ebx + 4], cx
        mov    cx, word ptr [edx + 14h]
        mov    word ptr [ebx + 6], cx
        mov    cx, word ptr [edx + 16h]
        mov    word ptr [ebx + 8], cx
        mov    cx, word ptr [edx + 18h]
        mov    word ptr [ebx + 0ah], cx
        mov    word ptr [edx + 1ah], si
        mov    word ptr [ebx + 0ch], si
        mov    cx, word ptr [edx + 1ch]
        mov    word ptr [ebx + 0eh], cx
        test   byte ptr [edx + 5], 80h
        je     SHORT loc_1_4090
        mov    di, word ptr [edx + 22h]
        add    di, cx
        dec    di
        mov    word ptr [ebx + 10h], di
        mov    word ptr [edx + 2ah], 0
        db  080h, 03Dh, 090h, 079h, 000h, 000h, 001h
        mov    di, word ptr [edx + 4]
        sbb    ecx, ecx
        neg    ecx
        mov    bx, di
        and    ebx, 4
        or     ecx, ebx
        cmp    ecx, 1
        cmc
        sbb    cl, cl
        and    cl, 40h
        mov    ebx, dword ptr [esp + 10h]
        and    di, 0fffbh
        mov    byte ptr [ebx], cl
        mov    word ptr [edx + 4], di
        jmp    SHORT loc_1_409F
        db  090h
loc_1_4090:
        mov    ebx, dword ptr [esp + 10h]
        mov    cx, word ptr [edx + 1eh]
        mov    word ptr [ebx + 10h], cx
        mov    byte ptr [ebx], 40h
loc_1_409F:
        xor    edi, edi
        mov    di, word ptr [edx + 18h]
        mov    ecx, esi
        and    ecx, 0ffffh
        add    edi, ecx
        xor    ecx, ecx
        mov    cx, word ptr [edx + 16h]
        mov    ebx, dword ptr [esp + 10h]
        sub    edi, ecx
        cmp    edi, 1
        sbb    cl, cl
        and    cl, 10h
        or     byte ptr [ebx], cl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
sub_1_40D0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 64h], ecx
        db  0C7h, 041h, 004h, 0F0h, 040h, 000h, 000h
        mov    byte ptr [eax + 7], 8
        push   ecx
        call   sub_1_0292
        add    esp, 4
        ret
loc_1_40F0:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        xor    ecx, ecx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    cx, word ptr [edi + 3eh]
        mov    esi, dword ptr [eax + ecx*4]
        mov    eax, dword ptr [edi + 40h]
        dec    byte ptr [eax + 33h]
        or     esi, esi
        je     NEAR PTR loc_1_4213
        cmp    byte ptr [esi + 7], 8
        jne    NEAR PTR loc_1_4213
        mov    ax, word ptr [esi + 28h]
        or     ax, ax
        je     SHORT loc_1_4176
        dec    ax
        mov    word ptr [esi + 28h], ax
        mov    eax, dword ptr [esi + 6ch]
        or     eax, eax
        je     SHORT loc_1_414D
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_414D
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 6ch]
        push   eax
        call   sub_1_3820
        add    esp, 4
loc_1_414D:
        mov    dword ptr [esi + 6ch], edi
        db  0C7h, 047h, 004h, 030h, 042h, 000h, 000h
        xor    eax, eax
        mov    ax, word ptr [esi + 30h]
        push   eax
        push   edi
        call   sub_1_3BE0
        add    esp, 8
        mov    dword ptr [esi + 64h], 0
        call   sub_1_2E80
        pop    edi
        pop    esi
        ret
loc_1_4176:
        push   edi
        call   sub_1_3820
        add    esp, 4
        mov    ecx, dword ptr [esi + 5ch]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 5ch
        mov    edi, 0
        je     SHORT loc_1_419C
        lea    edi, [ecx - 34h]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
loc_1_419C:
        mov    byte ptr [esi + 7], 1
        mov    byte ptr [edi + 1], 5
        test   byte ptr [edi], 80h
        je     SHORT loc_1_41C8
        db  0C7h, 047h, 034h, 090h, 078h, 000h, 000h
        add    edi, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [edi + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], edi
        db  089h, 03Dh, 094h, 078h, 000h, 000h
loc_1_41C8:
        mov    dword ptr [esi + 64h], 0
        mov    byte ptr [esi + 6], 6
        db  0A1h, 094h, 079h, 000h, 000h
        mov    word ptr [esi + 28h], ax
        mov    eax, dword ptr [esi + 68h]
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_4205
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   0
        mov    eax, dword ptr [esi + 68h]
        push   eax
        push   esi
        call   sub_1_0B20
        add    esp, 0ch
        call   sub_1_2E80
        pop    edi
        pop    esi
        ret
loc_1_4205:
        or     word ptr [esi + 4], 1000h
        call   sub_1_2E80
        pop    edi
        pop    esi
        ret
loc_1_4213:
        push   edi
        call   sub_1_3820
        add    esp, 4
        call   sub_1_2E80
        pop    edi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4230:
        push   esi
        mov    edx, dword ptr [esp + 8]
        xor    ecx, ecx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    cx, word ptr [edx + 3eh]
        mov    esi, dword ptr [eax + ecx*4]
        push   edx
        call   sub_1_3820
        add    esp, 4
        mov    dword ptr [esi + 6ch], 0
        cmp    byte ptr [esi + 7], 8
        jne    SHORT loc_1_4293
        db  0A1h, 05Ch, 079h, 000h, 000h
        inc    eax
        db  0A3h, 05Ch, 079h, 000h, 000h
        mov    ax, word ptr [esi + 4]
        test   al, 40h
        jne    SHORT loc_1_4293
        or     ax, 40h
        mov    word ptr [esi + 4], ax
        db  0C7h, 046h, 008h, 0D4h, 078h, 000h, 000h
        add    esi, 8
        db  0A1h, 0D8h, 078h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 0D8h, 078h, 000h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 0D8h, 078h, 000h, 000h
loc_1_4293:
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_42A0:
        sub    esp, 4
        push   esi
        db  081h, 03Dh, 0ECh, 078h, 000h, 000h, 0ECh, 078h, 000h, 000h
        je     SHORT loc_1_42E9
        db  0A1h, 0ECh, 078h, 000h, 000h
        mov    ecx, 8
loc_1_42BA:
        test   byte ptr [eax + 8], cl
        lea    esi, [eax - 20h]
        jne    SHORT loc_1_42CB
        mov    eax, dword ptr [eax]
        db  03Dh, 0ECh, 078h, 000h, 000h
        jne    SHORT loc_1_42BA
loc_1_42CB:
        db  03Dh, 0ECh, 078h, 000h, 000h
        je     SHORT loc_1_42E9
        call   sub_1_37F0
        or     eax, eax
        je     SHORT loc_1_42E9
        and    byte ptr [esi + 28h], 0f7h
        push   eax
        push   esi
        call   sub_1_2930
        add    esp, 8
loc_1_42E9:
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_1_42F0:
        push   esi
        push   edi
        call   sub_1_42A0
        db  081h, 03Dh, 0D4h, 078h, 000h, 000h, 0D4h, 078h, 000h, 000h
        je     NEAR PTR loc_1_43C8
        db  0BFh, 0D4h, 078h, 000h, 000h
        db  039h, 03Dh, 0D4h, 078h, 000h, 000h
        mov    esi, 0
        je     SHORT loc_1_4330
loc_1_4319:
        db  08Bh, 035h, 0D4h, 078h, 000h, 000h
        sub    esi, 8
        db  0A1h, 0D4h, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_4330:
        test   byte ptr [esi + 5], 80h
        push   esi
        je     SHORT loc_1_4350
        call   sub_1_4590
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_4390
        and    word ptr [esi + 4], 0ffbfh
        jmp    SHORT loc_1_4380
        db  005h, 000h, 000h, 000h, 000h
loc_1_4350:
        call   sub_1_3E90
        add    esp, 4
        or     eax, eax
        db  08Bh, 00Dh, 094h, 079h, 000h, 000h
        je     SHORT loc_1_43B0
        cmp    byte ptr [esi + 7], 1
        jne    SHORT loc_1_4370
        mov    word ptr [esi + 28h], cx
        mov    byte ptr [esi + 7], 4
loc_1_4370:
        and    word ptr [esi + 4], 0ffbfh
        push   eax
        push   esi
        call   sub_1_40D0
        add    esp, 8
loc_1_4380:
        db  039h, 03Dh, 0D4h, 078h, 000h, 000h
        jne    SHORT loc_1_4319
        pop    edi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_4390:
        add    esi, 8
        db  08Bh, 00Dh, 0D4h, 078h, 000h, 000h
        mov    eax, ecx
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 4], esi
        pop    edi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_43B0:
        add    esi, 8
        db  08Bh, 00Dh, 0D4h, 078h, 000h, 000h
        mov    eax, ecx
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 4], esi
loc_1_43C8:
        pop    edi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_1_43D0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    ax, word ptr [esi + 28h]
        dec    ax
        mov    word ptr [esi + 28h], ax
        jne    NEAR PTR loc_1_4471
        mov    byte ptr [esi + 7], 8
        mov    byte ptr [esi + 6], 6
        db  0A1h, 094h, 079h, 000h, 000h
        mov    word ptr [esi + 28h], ax
        mov    edi, 0
        mov    ecx, dword ptr [esi + 5ch]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 5ch
        je     SHORT loc_1_4414
        lea    edi, [ecx - 34h]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
loc_1_4414:
        or     edi, edi
        je     SHORT loc_1_4444
        mov    byte ptr [edi + 32h], 0
        mov    byte ptr [edi + 1], 5
        test   byte ptr [edi], 80h
        je     SHORT loc_1_4444
        db  0C7h, 047h, 034h, 090h, 078h, 000h, 000h
        add    edi, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [edi + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], edi
        db  089h, 03Dh, 094h, 078h, 000h, 000h
loc_1_4444:
        mov    eax, dword ptr [esi + 68h]
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_4468
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   0
        mov    eax, dword ptr [esi + 68h]
        push   eax
        push   esi
        call   sub_1_0B20
        add    esp, 0ch
        pop    edi
        pop    esi
        ret
loc_1_4468:
        or     word ptr [esi + 4], 1000h
        pop    edi
        pop    esi
        ret
loc_1_4471:
        db  0A1h, 05Ch, 079h, 000h, 000h
        inc    eax
        db  0A3h, 05Ch, 079h, 000h, 000h
        mov    word ptr [esi + 20h], 1
        mov    ax, word ptr [esi + 24h]
        mov    word ptr [esi + 14h], ax
        mov    ax, word ptr [esi + 26h]
        mov    word ptr [esi + 18h], ax
        mov    byte ptr [esi + 7], 1
        mov    ax, word ptr [esi + 4]
        or     ax, 4
        mov    word ptr [esi + 4], ax
        test   al, 40h
        jne    SHORT loc_1_44CD
        or     ax, 40h
        mov    word ptr [esi + 4], ax
        db  0C7h, 046h, 008h, 0D4h, 078h, 000h, 000h
        add    esi, 8
        db  0A1h, 0D8h, 078h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 0D8h, 078h, 000h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 0D8h, 078h, 000h, 000h
loc_1_44CD:
        pop    edi
        pop    esi
        ret
loc_1_44D0:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        xor    edx, edx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    dx, word ptr [ecx + 3eh]
        mov    esi, dword ptr [eax + edx*4]
        push   ecx
        mov    dword ptr [esi + 6ch], 0
        call   sub_1_3820
        add    esp, 4
        cmp    byte ptr [esi + 6], 3
        jne    SHORT loc_1_4502
        push   esi
        call   sub_1_43D0
        add    esp, 4
loc_1_4502:
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4510:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        xor    ecx, ecx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    cx, word ptr [esi + 3eh]
        mov    edi, dword ptr [eax + ecx*4]
        mov    eax, dword ptr [esi + 40h]
        dec    byte ptr [eax + 33h]
        or     edi, edi
        jne    SHORT loc_1_4540
        push   esi
        call   sub_1_3820
        add    esp, 4
        pop    edi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_4540:
        mov    dword ptr [edi + 64h], 0
        mov    eax, dword ptr [edi + 6ch]
        or     eax, eax
        je     SHORT loc_1_4563
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [edi + 6ch]
        push   eax
        call   sub_1_3820
        add    esp, 4
loc_1_4563:
        mov    dword ptr [edi + 6ch], esi
        db  0C7h, 046h, 004h, 0D0h, 044h, 000h, 000h
        xor    eax, eax
        mov    ax, word ptr [edi + 30h]
        push   eax
        push   esi
        call   sub_1_3BE0
        add    esp, 8
        call   sub_1_2E80
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4590:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        cmp    byte ptr [esi + 7], 1
        jne    NEAR PTR loc_1_466B
        db  0BFh, 010h, 045h, 000h, 000h
        mov    ebx, 8
        lea    esp, [esp]
loc_1_45B0:
        mov    eax, dword ptr [esi + 5ch]
        sub    eax, esi
        cmp    eax, 5ch
        je     NEAR PTR loc_1_4667
        push   esi
        call   sub_1_3E90
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_1_4660
        mov    ax, word ptr [esi + 1ah]
        inc    word ptr [esi + 14h]
        add    ax, word ptr [esi + 18h]
        mov    word ptr [esi + 18h], ax
        cmp    word ptr [esi + 16h], ax
        jne    SHORT loc_1_45F0
        mov    byte ptr [esi + 7], bl
        jmp    SHORT loc_1_4605
        db  08Dh, 064h, 024h, 000h
loc_1_45F0:
        mov    ax, word ptr [esi + 14h]
        sub    ax, word ptr [esi + 24h]
        cmp    ax, word ptr [esi + 20h]
        jb     SHORT loc_1_4605
        mov    byte ptr [esi + 7], bl
        or     byte ptr [ebp + 6ah], 40h
loc_1_4605:
        mov    dword ptr [esi + 64h], ebp
        mov    dword ptr [ebp + 4], edi
        test   byte ptr [esi + 5], 20h
        je     SHORT loc_1_4641
        mov    eax, dword ptr [esi + 68h]
        test   word ptr [eax + 3ch], bx
        je     SHORT loc_1_463B
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 68h]
        push   32h
        db  0C7h, 040h, 004h, 000h, 00Fh, 000h, 000h
        mov    eax, dword ptr [esi + 68h]
        push   eax
        call   sub_1_3BE0
        add    esp, 8
loc_1_463B:
        and    word ptr [esi + 4], 0dfffh
loc_1_4641:
        push   ebp
        call   sub_1_0292
        add    esp, 4
        cmp    byte ptr [esi + 7], 1
        je     NEAR PTR loc_1_45B0
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_4660:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_4667:
        mov    byte ptr [esi + 7], 0
loc_1_466B:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4680:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        xor    ecx, ecx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    cl, byte ptr [edi + 2]
        mov    esi, dword ptr [eax + ecx*4]
        or     esi, esi
        je     SHORT loc_1_4700
        cmp    byte ptr [esi + 6], 3
        jne    SHORT loc_1_46FB
        cmp    byte ptr [esi + 2fh], 0
        jne    SHORT loc_1_46A9
        mov    al, byte ptr [esi + 2dh]
        mov    byte ptr [esi + 2fh], al
loc_1_46A9:
        lea    ecx, [edi + 34h]
        lea    edx, [esi + 54h]
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx + 4], ecx
        mov    byte ptr [edi + 32h], 5
        mov    ax, word ptr [esi + 4]
        test   ah, 4
        je     SHORT loc_1_46F6
        and    ax, 0fbffh
        mov    word ptr [esi + 4], ax
        mov    eax, dword ptr [esi + 68h]
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_46F6
        push   eax
        call   sub_1_3C30
        add    esp, 4
        or     word ptr [esi + 4], 800h
        push   esi
        mov    eax, dword ptr [esi + 68h]
        push   eax
        call   sub_1_0E00
        add    esp, 8
loc_1_46F6:
        mov    al, 0ffh
        pop    edi
        pop    esi
        ret
loc_1_46FB:
        mov    al, 8
        pop    edi
        pop    esi
        ret
loc_1_4700:
        mov    al, 8
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4710:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    al, byte ptr [ecx + 3]
        cmp    al, 0ffh
        jne    SHORT loc_1_4747
        db  0C7h, 041h, 034h, 074h, 078h, 000h, 000h
        lea    edx, [ecx + 34h]
        db  0A1h, 078h, 078h, 000h, 000h
        mov    dword ptr [edx + 4], eax
        db  0A1h, 078h, 078h, 000h, 000h
        pop    edi
        mov    dword ptr [eax], edx
        mov    al, 0ffh
        db  089h, 015h, 078h, 078h, 000h, 000h
        mov    byte ptr [ecx + 32h], 6
        pop    esi
        pop    ebx
        ret
loc_1_4747:
        xor    edx, edx
        mov    dl, al
        db  0A1h, 050h, 078h, 000h, 000h
        mov    al, byte ptr [edx + eax]
        and    eax, 0ffh
        db  039h, 005h, 084h, 079h, 000h, 000h
        jbe    NEAR PTR loc_1_482A
        mov    edx, eax
        shl    edx, 4
        add    edx, eax
        db  0A1h, 0F4h, 078h, 000h, 000h
        lea    edi, [eax + edx*4]
        test   byte ptr [edi + 14h], 4
        je     NEAR PTR loc_1_482A
        lea    edx, [ecx + 34h]
        lea    ebx, [edi + 18h]
        mov    dword ptr [edx], ebx
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ebx + 4], edx
        mov    byte ptr [ecx + 32h], 6
        mov    esi, dword ptr [edi + 38h]
        or     esi, esi
        je     NEAR PTR loc_1_4824
        mov    eax, 3
        mov    ecx, 400h
loc_1_47A9:
        cmp    byte ptr [esi + 6], al
        jne    SHORT loc_1_47B4
        test   word ptr [esi + 4], cx
        jne    SHORT loc_1_47C0
loc_1_47B4:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_47A9
        mov    al, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_47C0:
        mov    eax, dword ptr [ebx]
        mov    ecx, eax
        sub    ecx, edi
        cmp    ecx, 18h
        mov    edi, 0
        je     SHORT loc_1_47DC
        lea    edi, [eax - 34h]
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_47DC:
        mov    al, byte ptr [esi + 10h]
        mov    byte ptr [edi + 2], al
        lea    eax, [edi + 34h]
        lea    ecx, [esi + 54h]
        mov    dword ptr [eax], ecx
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [edx], eax
        mov    dword ptr [ecx + 4], eax
        mov    byte ptr [edi + 32h], 5
        mov    eax, dword ptr [esi + 68h]
        and    word ptr [esi + 4], 0fbffh
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_4824
        push   eax
        call   sub_1_3C30
        add    esp, 4
        or     word ptr [esi + 4], 800h
        push   esi
        mov    eax, dword ptr [esi + 68h]
        push   eax
        call   sub_1_0E00
        add    esp, 8
loc_1_4824:
        mov    al, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_482A:
        mov    al, 13h
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_4830:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        sub    bl, bl
        mov    al, byte ptr [esi + 32h]
        cmp    al, 5
        jne    SHORT loc_1_489A
        lea    eax, [esi + 34h]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [esi + 32h], bl
        xor    ecx, ecx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    cl, byte ptr [esi + 2]
        mov    edi, dword ptr [eax + ecx*4]
        mov    byte ptr [esi + 1], 0bh
        mov    eax, dword ptr [edi + 68h]
        or     eax, eax
        je     SHORT loc_1_4889
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_4889
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    byte ptr [edi + 6], 6
        push   0
        mov    eax, dword ptr [edi + 68h]
        push   eax
        push   edi
        call   sub_1_0B20
        add    esp, 0ch
loc_1_4889:
        push   18h
        push   edi
        call   sub_1_07A0
        add    esp, 8
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_489A:
        cmp    al, 6
        jne    SHORT loc_1_48B8
        lea    eax, [esi + 34h]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [esi + 32h], 0
        mov    byte ptr [esi + 1], 0bh
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_48B8:
        mov    bl, 26h
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_48C0:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        sub    edi, edi
        mov    eax, dword ptr [ebx + 54h]
        lea    esi, [ebx + 54h]
        mov    ecx, eax
        sub    ecx, ebx
        cmp    ecx, 54h
        je     SHORT loc_1_48E0
        sub    eax, 34h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_48E0:
        mov    edx, dword ptr [ebx + 48h]
        test   byte ptr [edx + 14h], 4
        je     SHORT loc_1_4906
        mov    ecx, dword ptr [edx + 18h]
        mov    eax, ecx
        sub    eax, edx
        cmp    eax, 18h
        mov    edi, 0
        je     SHORT loc_1_4906
        lea    edi, [ecx - 34h]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
loc_1_4906:
        or     edi, edi
        jne    SHORT loc_1_4936
        db  081h, 03Dh, 074h, 078h, 000h, 000h, 074h, 078h, 000h, 000h
        mov    edi, 0
        je     SHORT loc_1_4932
        db  08Bh, 03Dh, 074h, 078h, 000h, 000h
        sub    edi, 34h
        db  0A1h, 074h, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_4932:
        or     edi, edi
        je     SHORT loc_1_495A
loc_1_4936:
        mov    al, byte ptr [ebx + 10h]
        mov    byte ptr [edi + 2], al
        mov    eax, dword ptr [ebx + 48h]
        mov    al, byte ptr [eax + 15h]
        mov    byte ptr [edi + 3], al
        mov    dword ptr [edi + 34h], esi
        lea    eax, [edi + 34h]
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 4], eax
        mov    byte ptr [edi + 32h], 5
loc_1_495A:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_4960:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 20h]
        mov    ax, word ptr [esi + 1eh]
        cmp    word ptr [ecx + 0ah], ax
        jne    NEAR PTR loc_1_4B1A
        mov    ebx, dword ptr [esp + 1ch]
        add    ebx, 24h
        mov    word ptr [esp + 10h], 30h
        mov    ax, word ptr [ecx + 0ch]
        mov    word ptr [esp + 12h], ax
loc_1_4994:
        push   esi
        call   sub_1_48C0
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_1_4B14
        mov    bp, word ptr [edi + 8]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 78h]
        mov    eax, ebp
        and    eax, 0ffffh
        xor    edx, edx
        mov    dx, word ptr [esp + 12h]
        sub    eax, ecx
        cmp    eax, edx
        jge    SHORT loc_1_49D0
        sub    bp, word ptr [esi + 78h]
        jmp    SHORT loc_1_49D5
        db  005h, 000h, 000h, 000h, 000h
loc_1_49D0:
        mov    bp, word ptr [esp + 12h]
loc_1_49D5:
        push   ebp
        mov    eax, dword ptr [edi + 4]
        add    eax, ecx
        push   eax
        mov    ax, word ptr [esp + 18h]
        push   eax
        push   ebx
        call   sub_1_3B60
        add    esp, 10h
        mov    dx, word ptr [esi + 1eh]
        add    word ptr [esi + 78h], bp
        add    dx, bp
        sub    word ptr [esp + 12h], bp
        mov    word ptr [esi + 1eh], dx
        xor    eax, eax
        mov    ax, word ptr [ebx + 4]
        mov    ecx, dword ptr [esp + 10h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, ebp
        and    ecx, 0ffffh
        cmp    eax, ecx
        jg     SHORT loc_1_4A39
loc_1_4A20:
        sub    bp, word ptr [ebx + 4]
        add    bp, word ptr [esp + 10h]
        add    ebx, 6
        mov    word ptr [esp + 10h], 0
        cmp    word ptr [ebx + 4], bp
        jbe    SHORT loc_1_4A20
loc_1_4A39:
        mov    ecx, dword ptr [esp + 20h]
        add    word ptr [esp + 10h], bp
        cmp    word ptr [ecx + 8], dx
        je     SHORT loc_1_4AB0
        mov    ax, word ptr [esi + 78h]
        cmp    word ptr [edi + 8], ax
        jne    SHORT loc_1_4A9C
        mov    word ptr [esi + 78h], 0
        mov    byte ptr [edi + 1], 6
        mov    byte ptr [edi + 32h], 0
        mov    al, byte ptr [esi + 2dh]
        or     al, al
        je     SHORT loc_1_4A74
        mov    ecx, dword ptr [esi + 54h]
        sub    ecx, esi
        cmp    ecx, 54h
        je     SHORT loc_1_4A74
        mov    byte ptr [esi + 2fh], al
loc_1_4A74:
        lea    ebp, [edi + 34h]
        push   ebp
        call   sub_1_3B50
        add    esp, 4
        test   byte ptr [edi], 80h
        je     SHORT loc_1_4A9C
        db  0C7h, 045h, 000h, 090h, 078h, 000h, 000h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        db  089h, 02Dh, 094h, 078h, 000h, 000h
loc_1_4A9C:
        cmp    word ptr [esp + 12h], 0
        jne    NEAR PTR loc_1_4994
        jmp    SHORT loc_1_4B10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_4AB0:
        mov    ax, word ptr [esi + 78h]
        inc    word ptr [esi + 1ch]
        mov    word ptr [edi + 8], ax
        mov    word ptr [esi + 78h], 0
        mov    word ptr [esi + 1eh], 0
        mov    byte ptr [edi + 1], 0
        mov    byte ptr [edi + 32h], 0
        mov    cl, byte ptr [esi + 2dh]
        or     cl, cl
        je     SHORT loc_1_4AE4
        mov    eax, dword ptr [esi + 54h]
        sub    eax, esi
        cmp    eax, 54h
        je     SHORT loc_1_4AE4
        mov    byte ptr [esi + 2fh], cl
loc_1_4AE4:
        lea    ebx, [edi + 34h]
        push   ebx
        call   sub_1_3B50
        add    esp, 4
        test   byte ptr [edi], 80h
        je     SHORT loc_1_4B10
        db  0C7h, 003h, 090h, 078h, 000h, 000h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [ebx + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], ebx
        db  089h, 01Dh, 094h, 078h, 000h, 000h
loc_1_4B10:
        or     edi, edi
        jne    SHORT loc_1_4B1A
loc_1_4B14:
        or     word ptr [esi + 4], 400h
loc_1_4B1A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4B30:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 20h]
        mov    word ptr [esp + 10h], 30h
        mov    ax, word ptr [eax + 0ch]
        add    edi, 24h
        mov    esi, dword ptr [esp + 18h]
        mov    word ptr [esp + 12h], ax
        mov    ax, word ptr [esi + 1eh]
        or     ax, ax
        je     SHORT loc_1_4BA2
        cmp    word ptr [esp + 12h], ax
        jbe    NEAR PTR loc_1_4D32
        xor    ecx, ecx
        sub    word ptr [esp + 12h], ax
        mov    cx, word ptr [edi + 4]
        sub    ecx, 30h
        mov    edx, eax
        and    edx, 0ffffh
        cmp    ecx, edx
        jg     SHORT loc_1_4B9D
loc_1_4B84:
        sub    ax, word ptr [edi + 4]
        add    ax, word ptr [esp + 10h]
        add    edi, 6
        mov    word ptr [esp + 10h], 0
        cmp    word ptr [edi + 4], ax
        jbe    SHORT loc_1_4B84
loc_1_4B9D:
        add    word ptr [esp + 10h], ax
loc_1_4BA2:
        push   esi
        call   sub_1_48C0
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_4CF6
        mov    bp, word ptr [ebx + 8]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 78h]
        mov    eax, ebp
        and    eax, 0ffffh
        xor    edx, edx
        mov    dx, word ptr [esp + 12h]
        sub    eax, ecx
        cmp    eax, edx
        jge    SHORT loc_1_4BD9
        sub    bp, word ptr [esi + 78h]
        jmp    SHORT loc_1_4BDE
loc_1_4BD9:
        mov    bp, word ptr [esp + 12h]
loc_1_4BDE:
        push   ebp
        mov    eax, dword ptr [ebx + 4]
        add    eax, ecx
        push   eax
        mov    ax, word ptr [esp + 18h]
        push   eax
        push   edi
        call   sub_1_3B60
        add    esp, 10h
        add    word ptr [esi + 1eh], bp
        add    word ptr [esi + 78h], bp
        xor    ecx, ecx
        sub    word ptr [esp + 12h], bp
        mov    cx, word ptr [edi + 4]
        mov    eax, dword ptr [esp + 10h]
        and    eax, 0ffffh
        sub    ecx, eax
        mov    eax, ebp
        and    eax, 0ffffh
        cmp    ecx, eax
        jg     SHORT loc_1_4C39
loc_1_4C20:
        sub    bp, word ptr [edi + 4]
        add    bp, word ptr [esp + 10h]
        add    edi, 6
        mov    word ptr [esp + 10h], 0
        cmp    word ptr [edi + 4], bp
        jbe    SHORT loc_1_4C20
loc_1_4C39:
        add    word ptr [esp + 10h], bp
        cmp    word ptr [esp + 12h], 0
        jne    SHORT loc_1_4C92
        mov    eax, dword ptr [esp + 20h]
        test   byte ptr [eax], 10h
        je     SHORT loc_1_4C92
        mov    ax, word ptr [esi + 78h]
        mov    word ptr [ebx + 8], ax
        mov    word ptr [esi + 78h], 0
        mov    word ptr [esi + 1eh], 0
        mov    byte ptr [ebx + 1], 0
        mov    byte ptr [ebx + 32h], 0
        mov    al, byte ptr [esi + 2dh]
        or     al, al
        je     SHORT loc_1_4C7F
        mov    ecx, dword ptr [esi + 54h]
        sub    ecx, esi
        cmp    ecx, 54h
        je     SHORT loc_1_4C7F
        mov    byte ptr [esi + 2fh], al
loc_1_4C7F:
        lea    ebp, [ebx + 34h]
        push   ebp
        call   sub_1_3B50
        add    esp, 4
        test   byte ptr [ebx], 80h
        je     SHORT loc_1_4CE6
        jmp    SHORT loc_1_4CCF
loc_1_4C92:
        mov    ax, word ptr [esi + 78h]
        cmp    word ptr [ebx + 8], ax
        jne    SHORT loc_1_4CE6
        mov    word ptr [esi + 78h], 0
        mov    byte ptr [ebx + 1], 6
        mov    byte ptr [ebx + 32h], 0
        mov    al, byte ptr [esi + 2dh]
        or     al, al
        je     SHORT loc_1_4CBE
        mov    ecx, dword ptr [esi + 54h]
        sub    ecx, esi
        cmp    ecx, 54h
        je     SHORT loc_1_4CBE
        mov    byte ptr [esi + 2fh], al
loc_1_4CBE:
        lea    ebp, [ebx + 34h]
        push   ebp
        call   sub_1_3B50
        add    esp, 4
        test   byte ptr [ebx], 80h
        je     SHORT loc_1_4CE6
loc_1_4CCF:
        db  0C7h, 045h, 000h, 090h, 078h, 000h, 000h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        db  089h, 02Dh, 094h, 078h, 000h, 000h
loc_1_4CE6:
        cmp    word ptr [esp + 12h], 0
        jne    NEAR PTR loc_1_4BA2
        or     ebx, ebx
        jne    SHORT loc_1_4D04
loc_1_4CF6:
        or     word ptr [esi + 4], 480h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_4D04:
        inc    word ptr [esi + 1ch]
        mov    eax, dword ptr [esp + 20h]
        mov    word ptr [esi + 1eh], 0
        mov    cl, byte ptr [eax]
        test   cl, 10h
        jne    SHORT loc_1_4D32
        mov    ax, word ptr [esi + 2ah]
        inc    ax
        cmp    word ptr [esi + 34h], ax
        mov    word ptr [esi + 2ah], ax
        ja     SHORT loc_1_4D32
        mov    eax, dword ptr [esp + 20h]
        or     cl, 40h
        mov    byte ptr [eax], cl
loc_1_4D32:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
_LTEXT ends
        end
