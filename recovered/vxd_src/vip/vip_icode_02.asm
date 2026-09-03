; vip_icode_02.asm -- VIP object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0FEh, 005h, 0FBh, 0BBh, 000h, 000h
        jne    SHORT loc_2_0024
        db  0BFh, 070h, 0BFh, 000h, 000h
        db  0B9h, 000h, 0C3h, 000h, 000h
        sub    ecx, edi
        shr    ecx, 2
        sub    eax, eax
        cld
        rep stosd
        call   sub_2_02B8
        cmp    eax, 1
loc_2_0024:
        ret
        db  080h, 03Fh, 000h, 074h, 03Ah, 080h, 03Fh, 020h, 074h, 005h, 080h, 03Fh, 009h, 075h, 003h, 047h
        db  0EBh, 0EEh, 080h, 03Fh, 02Ch, 074h, 02Bh, 08Bh, 0F7h, 047h, 080h, 03Fh, 000h, 074h, 01Eh, 080h
        db  03Fh, 02Ch, 075h, 006h, 0C6h, 007h, 000h, 047h, 0EBh, 013h, 080h, 03Fh, 020h, 074h, 008h, 080h
        db  03Fh, 009h, 074h, 003h, 047h, 0EBh, 0E3h, 0C6h, 007h, 000h, 047h, 0EBh, 0DDh, 0F8h, 0C3h, 02Bh
        db  0F6h, 0C3h, 0F9h, 0C3h
loc_2_0069:
        push   eax
        db  0CDh, 020h, 0B2h, 000h, 001h, 000h
        mov    edx, eax
        pop    eax
        ret
loc_2_0074:
        push   eax
        push   edx
        db  0CDh, 020h, 0ACh, 000h, 001h, 000h
        cmp    edx, dword ptr [esp + 4]
        je     SHORT loc_2_0085
        mov    edx, eax
        stc
loc_2_0085:
        pop    eax
        pop    eax
        cmc
        ret
sub_2_0089:
        cmp    byte ptr [eax], 0
        je     SHORT loc_2_009B
        cmp    byte ptr [eax], 20h
        je     SHORT loc_2_0098
        cmp    byte ptr [eax], 9
        jne    SHORT loc_2_009B
loc_2_0098:
        inc    eax
        jmp    SHORT sub_2_0089
loc_2_009B:
        ret
loc_2_009C:
        push   ebx
        push   ecx
        push   esi
        mov    eax, edx
        sub    ecx, ecx
        sub    edx, edx
        sub    ebx, ebx
        sub    esi, esi
loc_2_00A9:
        shl    edx, 8
        or     edx, esi
        sub    esi, esi
        call   sub_2_0089
        mov    bl, byte ptr [eax]
        sub    bl, 30h
        cmp    bl, 9
        ja     SHORT loc_2_0107
loc_2_00BF:
        imul   esi, esi, 0ah
        add    esi, ebx
        inc    eax
        call   sub_2_0089
        cmp    byte ptr [eax], 2eh
        je     SHORT loc_2_00E5
        cmp    byte ptr [eax], 0
        je     SHORT loc_2_00FB
        cmp    byte ptr [eax], 2ch
        je     SHORT loc_2_00FB
        mov    bl, byte ptr [eax]
        sub    bl, 30h
        cmp    bl, 9
        ja     SHORT loc_2_0107
        jmp    SHORT loc_2_00BF
loc_2_00E5:
        inc    eax
        inc    ecx
        cmp    ecx, 4
        jb     SHORT loc_2_00A9
        call   sub_2_0089
        cmp    byte ptr [eax], 0
        je     SHORT loc_2_00FB
        cmp    byte ptr [eax], 2ch
        jne    SHORT loc_2_0107
loc_2_00FB:
        cmp    ecx, 3
        jne    SHORT loc_2_0107
        shl    edx, 8
        or     edx, esi
        jmp    SHORT loc_2_010A
loc_2_0107:
        sub    edx, edx
        stc
loc_2_010A:
        pop    esi
        pop    ecx
        pop    ebx
        ret
sub_2_010E:
        push   ebp
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 2ch
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        sub    eax, eax
        movzx  ecx, byte ptr [ebp + 19h]
        imul   ecx, ecx, 36ee80h
        add    eax, ecx
        movzx  ecx, byte ptr [ebp + 18h]
        imul   ecx, ecx, 0ea60h
        add    eax, ecx
        movzx  ecx, byte ptr [ebp + 15h]
        imul   ecx, ecx, 3e8h
        add    eax, ecx
        movzx  ecx, byte ptr [ebp + 14h]
        imul   ecx, ecx, 0ah
        add    eax, ecx
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebx
        pop    ebp
        ret
sub_2_0184:
        sub    ecx, ecx
        movzx  edx, byte ptr [esi]
loc_2_0189:
        imul   ecx, ecx, 0ah
        sub    edx, 30h
        add    ecx, edx
        inc    esi
        movzx  edx, byte ptr [esi]
        cmp    dl, 30h
        jb     SHORT loc_2_019F
        cmp    dl, 39h
        jb     SHORT loc_2_0189
loc_2_019F:
        ret
sub_2_01A0:
        push   esi
        db  0BEh, 04Ch, 00Bh, 000h, 000h
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        jb     SHORT loc_2_022A
        mov    esi, edx
        add    esi, 3
        sub    eax, eax
        cmp    byte ptr [esi], 2dh
        jne    SHORT loc_2_01C2
        mov    eax, 2932e00h
        inc    esi
        jmp    SHORT loc_2_01C8
loc_2_01C2:
        cmp    byte ptr [esi], 2bh
        jne    SHORT loc_2_01C8
        inc    esi
loc_2_01C8:
        cmp    byte ptr [esi], 30h
        jb     SHORT loc_2_022A
        cmp    byte ptr [esi], 39h
        ja     SHORT loc_2_022A
        call   sub_2_0184
        cmp    ecx, 17h
        ja     SHORT loc_2_022A
        imul   ecx, ecx, 36ee80h
        add    eax, ecx
        cmp    byte ptr [esi], 3ah
        jne    SHORT loc_2_0228
        inc    esi
        cmp    byte ptr [esi], 30h
        jb     SHORT loc_2_022A
        cmp    byte ptr [esi], 39h
        ja     SHORT loc_2_022A
        call   sub_2_0184
        cmp    ecx, 3bh
        ja     SHORT loc_2_022A
        imul   ecx, ecx, 0ea60h
        add    eax, ecx
        cmp    byte ptr [esi], 3ah
        jne    SHORT loc_2_0228
        inc    esi
        cmp    byte ptr [esi], 30h
        jb     SHORT loc_2_022A
        cmp    byte ptr [esi], 39h
        ja     SHORT loc_2_022A
        call   sub_2_0184
        cmp    ecx, 3bh
        ja     SHORT loc_2_022A
        imul   ecx, ecx, 3e8h
        add    eax, ecx
loc_2_0228:
        pop    esi
        ret
loc_2_022A:
        mov    eax, 1b77400h
        pop    esi
        ret
        db  000h, 000h, 000h
sub_2_0234:
        push   esi
        push   edi
        db  0C7h, 005h, 0D0h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_2_01A0
        mov    esi, eax
        cmp    esi, -1
        jne    SHORT loc_2_025C
        db  0C7h, 005h, 094h, 0C0h, 000h, 000h, 000h, 000h, 000h, 080h
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_2_025C:
        call   sub_2_010E
        cmp    eax, 5265c00h
        jbe    SHORT loc_2_0278
        db  0C7h, 005h, 094h, 0C0h, 000h, 000h, 080h, 000h, 000h, 000h
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_2_0278:
        mov    edi, eax
        db  0E8h, 0D9h, 06Fh, 0FFh, 0FFh
        sub    edi, eax
        lea    eax, [edi + esi]
        db  0A3h, 0D0h, 0C1h, 000h, 000h
        db  0C7h, 005h, 094h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
sub_2_0298:
        push   esi
        db  08Bh, 035h, 090h, 0C0h, 000h, 000h
        or     esi, esi
        je     SHORT loc_2_02B3
        nop
loc_2_02A4:
        push   esi
        db  0E8h, 0DEh, 06Fh, 0FFh, 0FFh
        add    esp, 4
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_2_02A4
loc_2_02B3:
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_2_02B8:
        sub    esp, 4
        push   esi
        call   sub_2_0B28
        or     eax, eax
        jne    SHORT loc_2_02CC
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret
loc_2_02CC:
        db  0E8h, 0C3h, 06Fh, 0FFh, 0FFh
        db  0E8h, 01Ah, 064h, 0FFh, 0FFh
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_2_02E4
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret
        db  090h
loc_2_02E4:
        push   esi
        mov    eax, dword ptr [esi + 2ch]
        inc    eax
        db  0A3h, 000h, 0BCh, 000h, 000h
        call   sub_2_08FC
        add    esp, 4
        db  0A3h, 090h, 0C0h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_2_0308
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_2_0308:
        call   sub_2_04C4
        or     eax, eax
        jne    SHORT loc_2_0328
        db  0FFh, 035h, 090h, 0C0h, 000h, 000h
        db  0E8h, 06Ch, 06Fh, 0FFh, 0FFh
        add    esp, 4
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_2_0328:
        db  0E8h, 067h, 06Fh, 0FFh, 0FFh
        push   esi
        call   sub_2_0500
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_0350
        db  0FFh, 035h, 090h, 0C0h, 000h, 000h
        db  0E8h, 043h, 06Fh, 0FFh, 0FFh
        add    esp, 4
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret
        db  090h
loc_2_0350:
        db  0C6h, 005h, 0ECh, 0BFh, 000h, 000h, 03Ch
        db  0C7h, 005h, 0E8h, 0BFh, 000h, 000h, 0FCh, 0BFh, 000h, 000h
        cmp    dword ptr [esi], 0
        db  0C7h, 005h, 03Ch, 0C1h, 000h, 000h, 054h, 000h, 000h, 000h
        je     SHORT loc_2_037A
        db  0C7h, 005h, 03Ch, 0C1h, 000h, 000h, 068h, 000h, 000h, 000h
loc_2_037A:
        mov    eax, dword ptr [esi + 18h]
        db  0A3h, 05Ch, 0C1h, 000h, 000h
        mov    eax, dword ptr [esi + 1ch]
        db  0A3h, 060h, 0C0h, 000h, 000h
        mov    edx, dword ptr [esi + 20h]
        db  089h, 015h, 0F8h, 0C2h, 000h, 000h
        mov    eax, dword ptr [esi + 24h]
        sub    eax, 0ffh
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, 0ffh
        db  0A3h, 058h, 0C1h, 000h, 000h
        mov    eax, dword ptr [esi + 28h]
        and    eax, 0fch
        db  0A3h, 034h, 0C2h, 000h, 000h
        cmp    edx, 2
        jbe    SHORT loc_2_03C5
        db  0C7h, 005h, 0F8h, 0C2h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_03C5:
        call   sub_2_0234
        db  083h, 03Dh, 054h, 0C1h, 000h, 000h, 000h
        je     NEAR PTR loc_2_04AC
        db  0C7h, 005h, 058h, 0C0h, 000h, 000h, 001h, 000h, 000h, 000h
        cmp    dword ptr [esi], 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        db  0A3h, 0D8h, 0C1h, 000h, 000h
        db  0A1h, 058h, 0C1h, 000h, 000h
        db  0A3h, 0DCh, 0C1h, 000h, 000h
        db  0C7h, 005h, 00Ch, 0C2h, 000h, 000h, 03Ch, 000h, 000h, 000h
        db  0E8h, 040h, 0AFh, 0FEh, 0FFh
        or     eax, eax
        jne    SHORT loc_2_0428
        db  0E8h, 03Fh, 002h, 0FEh, 0FFh
        call   sub_2_0298
        push   esi
        db  0E8h, 0E4h, 06Dh, 0FFh, 0FFh
        add    esp, 4
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_2_0428:
        push   eax
        db  0E8h, 062h, 0ADh, 0FEh, 0FFh
        add    esp, 4
        push   32h
        db  068h, 0F4h, 0C2h, 000h, 000h
        lea    eax, [esp + 0ch]
        push   eax
        db  0E8h, 00Ah, 070h, 0FFh, 0FFh
        db  0E8h, 04Dh, 06Eh, 0FFh, 0FFh
        push   5
        call   sub_2_0890
        add    esp, 4
        call   sub_2_0AD0
        or     eax, eax
        jne    SHORT loc_2_0464
        db  0C7h, 005h, 0F8h, 0C2h, 000h, 000h, 001h, 000h, 000h, 000h
loc_2_0464:
        db  0E8h, 02Bh, 06Eh, 0FFh, 0FFh
        push   esi
        call   sub_2_05A0
        add    esp, 4
        push   esi
        db  0E8h, 088h, 06Dh, 0FFh, 0FFh
        add    esp, 4
        db  0E8h, 014h, 06Eh, 0FFh, 0FFh
        db  0E8h, 00Fh, 06Eh, 0FFh, 0FFh
        db  08Bh, 035h, 090h, 0C0h, 000h, 000h
        or     esi, esi
        je     SHORT loc_2_049F
        nop
loc_2_0490:
        push   esi
        db  0E8h, 052h, 050h, 0FFh, 0FFh
        add    esp, 4
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_2_0490
loc_2_049F:
        pop    esi
        mov    eax, 1
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_2_04AC:
        call   sub_2_0298
        push   esi
        db  0E8h, 049h, 06Dh, 0FFh, 0FFh
        add    esp, 4
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_2_04C4:
        sub    esp, 4
        db  0C7h, 005h, 0D2h, 0BCh, 000h, 000h, 060h, 0BCh, 000h, 000h
        push   42h
        db  068h, 094h, 0BCh, 000h, 000h
        db  068h, 0E4h, 0BFh, 000h, 000h
        lea    eax, [esp + 0ch]
        push   eax
        db  0E8h, 021h, 06Eh, 0FFh, 0FFh
        cmp    dword ptr [esp], 0
        jne    SHORT loc_2_04F8
        mov    eax, 1
        add    esp, 4
        ret
        db  090h
loc_2_04F8:
        sub    eax, eax
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_2_0500:
        push   edi
        db  0E8h, 08Eh, 06Dh, 0FFh, 0FFh
        db  0C7h, 005h, 0CCh, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0C8h, 0C2h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0BFh, 048h, 0C2h, 000h, 000h
        sub    eax, eax
        mov    ecx, 20h
        rep stosd
        push   eax
        push   2
        push   1
        push   100h
        db  0A1h, 050h, 0C0h, 000h, 000h
        push   dword ptr [eax + 0ch]
        push   -1
        push   0ffh
        push   7fh
        db  0E8h, 078h, 0F9h, 0FEh, 0FFh
        add    esp, 20h
        db  0C6h, 005h, 050h, 0C1h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        mov    al, byte ptr [eax]
        db  0A2h, 0A4h, 0BFh, 000h, 000h
        db  0A2h, 0D4h, 0C1h, 000h, 000h
        db  0C7h, 005h, 030h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 038h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 06Ch, 0BFh, 000h, 000h
        db  0A3h, 034h, 0C1h, 000h, 000h
        push   0
        db  068h, 0DCh, 081h, 000h, 000h
        push   0ea60h
        db  068h, 024h, 0C1h, 000h, 000h
        db  0E8h, 0AAh, 06Ch, 0FFh, 0FFh
        add    esp, 10h
        mov    eax, 1
        pop    edi
        ret
sub_2_05A0:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        sub    ebx, ebx
        db  089h, 01Dh, 048h, 0C1h, 000h, 000h
        db  038h, 01Dh, 0D4h, 0C1h, 000h, 000h
        je     NEAR PTR loc_2_0880
        db  0E8h, 0D2h, 06Ch, 0FFh, 0FFh
        push   14h
        db  0E8h, 0B3h, 06Ch, 0FFh, 0FFh
        add    esp, 4
        db  0A3h, 0FCh, 0BCh, 000h, 000h
        cmp    eax, ebx
        je     NEAR PTR loc_2_06ED
        db  0C7h, 000h, 060h, 085h, 000h, 000h
        mov    edi, dword ptr [esp + 38h]
        mov    eax, dword ptr [edi + 10h]
        db  0A3h, 0F8h, 0BFh, 000h, 000h
        mov    edx, dword ptr [edi + 14h]
        db  089h, 015h, 0C0h, 0C1h, 000h, 000h
        mov    ecx, dword ptr [edi + 8]
        sub    ecx, eax
        sbb    ebp, ebp
        and    ecx, ebp
        lea    ebp, [ecx + eax]
        mov    eax, dword ptr [edi + 0ch]
        sub    eax, edx
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, edx
        mov    dword ptr [esp + 24h], eax
        db  08Bh, 00Dh, 090h, 0C0h, 000h, 000h
        cmp    ecx, ebx
        je     SHORT loc_2_063F
loc_2_0619:
        mov    eax, dword ptr [ecx + 0ch]
        lea    edx, [eax + 54h]
        mov    dword ptr [edx], edx
        mov    dword ptr [eax + 58h], edx
        mov    dword ptr [eax + 64h], 0
        mov    dword ptr [eax + 5ch], 0
        mov    dword ptr [eax + 60h], 0
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_2_0619
loc_2_063F:
        push   14h
        db  0E8h, 036h, 06Ch, 0FFh, 0FFh
        add    esp, 4
        db  0A3h, 048h, 0C1h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_06ED
        mov    dword ptr [eax], eax
        db  0A1h, 048h, 0C1h, 000h, 000h
        mov    dword ptr [eax + 4], eax
        db  0A1h, 048h, 0C1h, 000h, 000h
        mov    dword ptr [eax + 8], 0
        db  0A1h, 048h, 0C1h, 000h, 000h
        mov    dword ptr [eax + 0ch], 0
        db  0A1h, 048h, 0C1h, 000h, 000h
        mov    dword ptr [eax + 10h], 0
        db  0E8h, 00Bh, 06Ch, 0FFh, 0FFh
        shr    ebp, 8
        mov    eax, ebp
        shl    eax, 8
        mov    dword ptr [esp + 30h], eax
        mov    eax, dword ptr [esp + 24h]
        shl    eax, 2
        lea    eax, [eax + eax*4]
        push   eax
        db  0E8h, 0D7h, 06Bh, 0FFh, 0FFh
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_2_06ED
        push   dword ptr [esp + 24h]
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        db  0E8h, 08Bh, 06Dh, 0FFh, 0FFh
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_2_06ED
        push   50h
        push   dword ptr [esp + 28h]
        lea    eax, [esp + 28h]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        db  0E8h, 027h, 06Dh, 0FFh, 0FFh
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_2_0754
loc_2_06E4:
        push   dword ptr [esp + 1ch]
        db  0E8h, 06Bh, 06Dh, 0FFh, 0FFh
loc_2_06ED:
        db  083h, 03Dh, 0FCh, 0BCh, 000h, 000h, 000h
        je     SHORT loc_2_0704
        db  0FFh, 035h, 0FCh, 0BCh, 000h, 000h
        db  0E8h, 087h, 06Bh, 0FFh, 0FFh
        add    esp, 4
loc_2_0704:
        db  083h, 03Dh, 048h, 0C1h, 000h, 000h, 000h
        je     SHORT loc_2_071B
        db  0FFh, 035h, 048h, 0C1h, 000h, 000h
        db  0E8h, 070h, 06Bh, 0FFh, 0FFh
        add    esp, 4
loc_2_071B:
        or     esi, esi
        je     SHORT loc_2_0728
        push   esi
        db  0E8h, 063h, 06Bh, 0FFh, 0FFh
        add    esp, 4
loc_2_0728:
        or     ebx, ebx
        je     SHORT loc_2_0735
        push   ebx
        db  0E8h, 056h, 06Bh, 0FFh, 0FFh
        add    esp, 4
loc_2_0735:
        db  0C6h, 005h, 050h, 0C1h, 000h, 000h, 000h
        db  0C6h, 005h, 0A4h, 0BFh, 000h, 000h, 000h
        db  0C6h, 005h, 0D4h, 0C1h, 000h, 000h, 000h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
loc_2_0754:
        db  0E8h, 03Bh, 06Bh, 0FFh, 0FFh
        push   dword ptr [esp + 30h]
        db  0E8h, 01Ah, 06Bh, 0FFh, 0FFh
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_2_077C
        push   dword ptr [esp + 20h]
        db  0E8h, 09Ch, 06Ch, 0FFh, 0FFh
        jmp    NEAR PTR loc_2_06E4
        db  08Dh, 049h, 000h
loc_2_077C:
        push   ebp
        lea    eax, [esp + 2ch]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        db  0E8h, 0C0h, 06Ch, 0FFh, 0FFh
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_2_07A4
        push   dword ptr [esp + 20h]
        db  0E8h, 074h, 06Ch, 0FFh, 0FFh
        jmp    NEAR PTR loc_2_06E4
        db  08Dh, 049h, 000h
loc_2_07A4:
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_2_082A
        inc    eax
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [esp + 2ch], eax
loc_2_07B4:
        push   14h
        push   esi
        push   dword ptr [esp + 24h]
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        db  0E8h, 09Ah, 06Ch, 0FFh, 0FFh
        push   dword ptr [esp + 20h]
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        db  0E8h, 03Fh, 06Ch, 0FFh, 0FFh
        mov    edi, dword ptr [esp + 14h]
        add    edi, 38h
        sub    eax, eax
        mov    ecx, 14h
        rep stosd
        mov    ecx, dword ptr [esp + 14h]
        add    ecx, 38h
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 1ch], eax
        mov    dword ptr [ecx + 20h], esi
        mov    byte ptr [ecx + 5], 8
        mov    byte ptr [ecx + 4], 1
        db  0A1h, 0FCh, 0BCh, 000h, 000h
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ecx + 10h], eax
        push   dword ptr [esp + 14h]
        db  0E8h, 05Ah, 0FEh, 0FEh, 0FFh
        add    esp, 4
        add    esi, 14h
        dec    dword ptr [esp + 2ch]
        jne    SHORT loc_2_07B4
loc_2_082A:
        or     ebp, ebp
        je     SHORT loc_2_0864
loc_2_082E:
        push   100h
        push   ebx
        push   dword ptr [esp + 30h]
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        db  0E8h, 01Dh, 06Ch, 0FFh, 0FFh
        db  0A1h, 05Ch, 0C0h, 000h, 000h
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 10h]
        db  0A3h, 05Ch, 0C0h, 000h, 000h
        add    ebx, 100h
        dec    ebp
        jne    SHORT loc_2_082E
loc_2_0864:
        db  0C7h, 005h, 040h, 0C2h, 000h, 000h, 003h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 24h]
        db  0A3h, 020h, 0C1h, 000h, 000h
        mov    eax, dword ptr [esp + 30h]
        db  0A3h, 0FCh, 0C2h, 000h, 000h
loc_2_0880:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 049h, 000h
sub_2_0890:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  089h, 035h, 0ACh, 0C0h, 000h, 000h
        db  0C7h, 005h, 064h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 044h, 0C2h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, esi
        dec    esi
        or     eax, eax
        je     SHORT loc_2_08DE
loc_2_08B6:
        push   60h
        db  0E8h, 0BFh, 069h, 0FFh, 0FFh
        add    esp, 4
        or     eax, eax
        db  08Bh, 00Dh, 044h, 0C2h, 000h, 000h
        je     SHORT loc_2_08DE
        mov    dword ptr [eax], ecx
        db  0A3h, 044h, 0C2h, 000h, 000h
        db  0FFh, 005h, 064h, 0C0h, 000h, 000h
        mov    eax, esi
        dec    esi
        or     eax, eax
        jne    SHORT loc_2_08B6
loc_2_08DE:
        push   0
        db  068h, 074h, 097h, 000h, 000h
        db  068h, 0D4h, 092h, 000h, 000h
        db  068h, 07Ch, 098h, 000h, 000h
        push   1
        db  0E8h, 09Ah, 0FDh, 0FDh, 0FFh
        add    esp, 14h
        pop    esi
        ret
        db  090h
sub_2_08FC:
        sub    esp, 44h
        push   edi
        db  0E8h, 08Fh, 069h, 0FFh, 0FFh
        push   44h
        db  0E8h, 070h, 069h, 0FFh, 0FFh
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        db  089h, 015h, 050h, 0C0h, 000h, 000h
        jne    SHORT loc_2_0924
        mov    eax, edx
        pop    edi
        add    esp, 44h
        ret
        db  08Bh, 0FFh
loc_2_0924:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 11h
        rep stosd
        mov    dword ptr [edx + 4], 100007fh
        mov    dword ptr [edx + 8], 0ffh
        mov    word ptr [edx + 3eh], 1
        mov    word ptr [edx + 14h], 0dh
        mov    word ptr [edx + 3ch], 100h
        db  0C7h, 042h, 00Ch, 0B8h, 0C0h, 000h, 000h
        db  089h, 015h, 0BCh, 0C0h, 000h, 000h
        db  0C7h, 005h, 0C0h, 0C0h, 000h, 000h, 0E8h, 0A1h, 000h, 000h
        db  0C7h, 005h, 0C4h, 0C0h, 000h, 000h, 054h, 0A2h, 000h, 000h
        db  0C7h, 005h, 0C8h, 0C0h, 000h, 000h, 0A4h, 0A3h, 000h, 000h
        db  0C7h, 005h, 0CCh, 0C0h, 000h, 000h, 0A8h, 0A3h, 000h, 000h
        db  0C7h, 005h, 0D8h, 0C0h, 000h, 000h, 0ACh, 0A3h, 000h, 000h
        db  0C7h, 005h, 0DCh, 0C0h, 000h, 000h, 0E0h, 0A4h, 000h, 000h
        db  0C7h, 005h, 0E0h, 0C0h, 000h, 000h, 064h, 0A5h, 000h, 000h
        db  0C7h, 005h, 0D0h, 0C0h, 000h, 000h, 05Ch, 0A5h, 000h, 000h
        db  0C7h, 005h, 0D4h, 0C0h, 000h, 000h, 05Ch, 0A5h, 000h, 000h
        db  0C7h, 005h, 0F4h, 0C0h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0C7h, 005h, 0FCh, 0C0h, 000h, 000h, 080h, 096h, 098h, 000h
        db  0C7h, 005h, 0F8h, 0C0h, 000h, 000h, 000h, 001h, 000h, 000h
        mov    dword ptr [esp + 8], 114h
        db  0A1h, 040h, 0C1h, 000h, 000h
        mov    dword ptr [esp + 10h], eax
        db  0C7h, 044h, 024h, 01Ch, 0A4h, 0A3h, 000h, 000h
        db  0C7h, 044h, 024h, 020h, 05Ch, 0A5h, 000h, 000h
        db  0C7h, 044h, 024h, 024h, 05Ch, 0A5h, 000h, 000h
        mov    dword ptr [esp + 3ch], 1
        db  08Bh, 00Dh, 054h, 0C0h, 000h, 000h
        inc    ecx
        db  089h, 00Dh, 040h, 0C1h, 000h, 000h
        db  089h, 00Dh, 0E8h, 0C0h, 000h, 000h
        db  0C7h, 005h, 0A4h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 09Ch, 0C0h, 000h, 000h, 084h, 0A0h, 000h, 000h
        db  0A1h, 06Ch, 0BFh, 000h, 000h
        db  0A3h, 0A8h, 0C0h, 000h, 000h
        db  089h, 00Dh, 060h, 0C1h, 000h, 000h
        db  0C7h, 005h, 064h, 0C1h, 000h, 000h, 018h, 000h, 000h, 000h
        db  0C7h, 005h, 068h, 0C1h, 000h, 000h, 014h, 001h, 000h, 000h
        db  0C7h, 005h, 06Ch, 0C1h, 000h, 000h, 080h, 096h, 098h, 000h
        db  0C7h, 005h, 07Ch, 0C1h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 080h, 0C1h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 0B8h, 0C1h, 000h, 000h, 01Ah, 000h, 000h, 000h
        db  0C7h, 005h, 0B4h, 0C0h, 000h, 000h, 0B8h, 0C0h, 000h, 000h
        db  089h, 00Dh, 054h, 0C0h, 000h, 000h
        db  0FFh, 005h, 054h, 0C1h, 000h, 000h
        lea    eax, [esp + 4]
        push   eax
        push   edx
        db  0E8h, 027h, 011h, 0FEh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_0AA8
        sub    eax, eax
        pop    edi
        add    esp, 44h
        ret
        db  090h
loc_2_0AA8:
        db  0FFh, 035h, 050h, 0C0h, 000h, 000h
        db  0E8h, 00Dh, 010h, 0FEh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_0AC4
        sub    eax, eax
        pop    edi
        add    esp, 44h
        ret
        db  08Dh, 049h, 000h
loc_2_0AC4:
        pop    edi
        db  0A1h, 050h, 0C0h, 000h, 000h
        add    esp, 44h
        ret
        db  08Bh, 0FFh
sub_2_0AD0:
        db  083h, 03Dh, 0F8h, 0C2h, 000h, 000h, 002h
        je     SHORT loc_2_0AE0
        mov    eax, 1
        ret
        db  090h
loc_2_0AE0:
        db  0C7h, 005h, 0B0h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 0F0h, 0C2h, 000h, 000h, 000h
        db  0C7h, 005h, 0F0h, 0BFh, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        push   0
        db  068h, 0D4h, 092h, 000h, 000h
        db  068h, 038h, 0A6h, 000h, 000h
        push   2
        db  0E8h, 080h, 0FBh, 0FDh, 0FFh
        add    esp, 14h
        db  0A3h, 04Ch, 0C0h, 000h, 000h
        or     eax, eax
        je     SHORT loc_2_0B24
        mov    eax, 1
        ret
        db  08Bh, 0FFh
loc_2_0B24:
        sub    eax, eax
        ret
        db  090h
sub_2_0B28:
        db  0CDh, 020h, 000h, 000h, 088h, 004h
        jb     SHORT loc_2_0B44
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        jb     SHORT loc_2_0B44
        db  0CDh, 020h, 00Ah, 000h, 088h, 004h
        db  0A3h, 06Ch, 0BFh, 000h, 000h
        clc
loc_2_0B44:
        cmc
        sbb    eax, eax
        ret
        db  001h, 000h, 000h, 000h
loc_2_0B4C:
        push   esp
        pop    edx
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
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [edx + 69h], al
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 69724400h
        db  076h, 065h
        jb     SHORT loc_2_0C63
loc_2_0C63:
        dec    ecx
        outsb
        db  074h, 065h
        jb     SHORT loc_2_0CCF
        popad
        arpl   word ptr [ebp + 73h], sp
        add    byte ptr [ebp + 66h], al
        popad
        jne    SHORT loc_2_0CE0
        db  074h, 047h
        popad
        je     SHORT loc_2_0CDE
        db  077h, 061h
        jns    SHORT loc_2_0C7D
loc_2_0C7D:
        inc    ebp
        je     SHORT loc_2_0CE8
        jb     NEAR PTR loc_2_0CF1
        je     NEAR PTR loc_2_0CD9
        dec    esi
        inc    ecx
        push   eax
        add    byte ptr [ecx + 52h], al
        push   eax
        inc    ecx
        insb
        db  077h, 061h
        jns    SHORT loc_2_0D06
        push   ebx
        outsd
        jne    SHORT loc_2_0D09
        arpl   word ptr [ebp + 52h], sp
        outsd
        jne    SHORT loc_2_0D11
        add    byte ptr gs:[ebp + 6eh], al
        popad
        bound  ebp, qword ptr [ebp + 52h]
        outsd
        jne    SHORT loc_2_0D1D
        imul   ebp, dword ptr [esi + 67h], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 069h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0CCF:
        inc    esi
        outsd
        db  072h, 077h
        popad
        db  072h, 064h
        inc    edx
        db  072h, 06Fh
loc_2_0CD9:
        popad
        arpl   word ptr fs:[ecx + 73h], sp
loc_2_0CDE:
        db  074h, 073h
loc_2_0CE0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0CE8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 069h, 000h
loc_2_0CF1:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esp
        popad
        inc    edi
        push   edi
        inc    esp
loc_2_0D06:
        je     NEAR PTR loc_2_0D6E
loc_2_0D09:
        arpl   word ptr [eax + eax], si
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0D11:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0D1D:
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 10000h
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        dec    ebp
        push   esp
        push   ebp
        inc    esp
        imul   esi, dword ptr [ebx + 63h], 7265766fh
        jns    SHORT loc_2_0D3D
loc_2_0D3D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 10000h
        add    byte ptr [eax], al
        sbb    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ecx
        inc    edi
        dec    ebp
        push   eax
        dec    esp
        db  065h, 076h, 065h
        insb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0D6E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 074h, 000h, 000h
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 66h], al
        popad
        jne    SHORT loc_2_0E01
        je     SHORT loc_2_0DEB
        push   esp
        dec    esp
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
        je     SHORT loc_2_0DB1
loc_2_0DB1:
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        and    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esp
        popaw
        jne    SHORT loc_2_0E31
        je     SHORT loc_2_0E1B
        dec    edi
        push   ebx
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
        je     SHORT loc_2_0DE1
loc_2_0DE1:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0DEB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   edx
        outsd
        db  075h, 074h
        imul   ebp, dword ptr [esi + 67h], 53667542h
        imul   edi, dword ptr [edx + 65h], 0
loc_2_0E01:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        je     SHORT loc_2_0E11
loc_2_0E11:
        add    byte ptr [eax], al
        add    byte ptr [edx], ah
        add    dword ptr [eax], eax
        or     byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0E1B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   edx
        outsd
        jne    SHORT loc_2_0E97
        imul   ebp, dword ptr [esi + 67h], 6b636150h
        je     NEAR PTR loc_2_0EA0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0E31:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        je     SHORT loc_2_0E41
loc_2_0E41:
        add    byte ptr [eax], al
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ecx
        push   eax
        inc    ecx
        jb     NEAR PTR loc_2_0EBB
        jae    SHORT loc_2_0ECB
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
        db  000h, 09Ch, 000h, 000h, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        dec    ecx
        push   eax
        dec    ebp
        popad
        db  073h, 06Bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0E97:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pushfd
loc_2_0EA0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        pop    edx
        jb     NEAR PTR loc_2_0F22
        inc    edx
        jb     SHORT loc_2_0F25
        popad
        arpl   word ptr fs:[ecx + 73h], sp
loc_2_0EBB:
        je     SHORT loc_2_0EBD
loc_2_0EBD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0ECB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        add    byte ptr [eax], al
        sbb    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        popad
        js     SHORT loc_2_0F30
        push   esp
        push   ebp
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
        je     SHORT loc_2_0F01
loc_2_0F01:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 000h, 000h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_0F22:
        add    byte ptr [eax], al
        db  000h
loc_2_0F25:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_2_0F30:
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
        add    byte ptr [eax + 4ch], cl
        push   ebx
        and    byte ptr [eax], al
_ITEXT ends
        end
