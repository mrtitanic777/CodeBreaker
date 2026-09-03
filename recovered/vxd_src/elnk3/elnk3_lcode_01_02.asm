; elnk3_lcode_01_02.asm -- ELNK3 object 1 segment _LTEXT  (part 2/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_4D58:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ecx + 83h], 0
        push   edi
        jne    SHORT loc_1_4DAE
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_1_4DA1
        push   1
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    edx, dword ptr [ebp + 14h]
        push   edx
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        mov    eax, dword ptr [esi + 14h]
        push   eax
        inc    dword ptr [esi + 1ch]
        mov    ecx, dword ptr [ecx + 5eh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4D91:
        mov    edi, eax
        push   esi
        call   sub_1_3EF8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    14h
loc_1_4DA1:
        mov    edi, 0c0010002h
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    14h
loc_1_4DAE:
        mov    edi, 0c001000dh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4DC8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    edi, edi
        cmp    byte ptr [ecx + 83h], 0
        jne    SHORT loc_1_4E24
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_1_4E17
        inc    dword ptr [esi + 1ch]
        cmp    dword ptr [ecx + 134h], edi
        jne    SHORT loc_1_4E09
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    edi, dword ptr [esi + 14h]
        push   edi
        mov    eax, dword ptr [ecx + 5eh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4E07:
        mov    edi, eax
loc_1_4E09:
        push   esi
        call   sub_1_3EF8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
loc_1_4E17:
        mov    edi, 0c0010002h
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
loc_1_4E24:
        mov    edi, 0c001000dh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4E38:
        push   ebp
        mov    ecx, 3ch
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 52h]
        test   edx, edx
        je     SHORT loc_1_4E5B
loc_1_4E4B:
        mov    esi, dword ptr [edx + 18h]
        cmp    ecx, esi
        jae    SHORT loc_1_4E54
        mov    ecx, esi
loc_1_4E54:
        mov    edx, dword ptr [edx + 0ch]
        test   edx, edx
        jne    SHORT loc_1_4E4B
loc_1_4E5B:
        add    ecx, 0eh
        pop    esi
        pop    ebp
        mov    dword ptr [eax], ecx
        ret    4
        db  08Dh, 049h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        add    eax, 0ach
        push   edi
        mov    esi, dword ptr [ebp + 14h]
        push   esi
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_5406
        test   esi, esi
        je     SHORT loc_1_4EB5
        mov    ebx, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 0ch]
loc_1_4E91:
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        add    edi, 4
        mov    eax, dword ptr [edx + 0ach]
        lea    ecx, [ebp - 4]
        push   eax
        push   ebx
        push   ecx
        add    ebx, 4
        call   sub_1_5412
        dec    esi
        jne    SHORT loc_1_4E91
loc_1_4EB5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 0B9h, 000h, 000h, 000h, 000h
        db  08Bh, 0ECh, 056h, 08Bh, 075h, 00Ch, 08Bh, 046h, 00Ch, 085h, 0C0h, 074h, 010h, 08Bh, 056h, 004h
        db  08Dh, 00Ch, 010h, 0C1h, 0EAh, 00Ch, 049h, 0C1h, 0E9h, 00Ch, 02Bh, 0CAh, 041h, 08Bh, 045h, 018h
        db  039h, 008h, 072h, 010h, 050h, 08Bh, 045h, 014h, 050h, 056h, 0E8h, 02Dh, 005h, 000h, 000h, 05Eh
        db  05Dh, 0C2h, 014h, 000h, 05Eh, 089h, 008h, 05Dh, 0C2h, 014h, 000h, 08Dh, 064h, 024h, 000h, 08Dh
        db  064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 00Ch, 08Bh
        db  0ECh, 050h, 0E8h, 0F9h, 004h, 000h, 000h, 08Bh, 045h, 008h, 08Bh, 088h, 0ACh, 000h, 000h, 000h
        db  051h, 0E8h, 0D8h, 004h, 000h, 000h, 05Dh, 0C2h, 008h, 000h
loc_1_4F38:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_00A8
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4F58:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   eax
        call   sub_1_00E8
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_4F80:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4FB0
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_4FB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_5050
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_4FF0
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_4FE5
        cmp    al, 0ffh
        jne    SHORT loc_1_4FE5
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_4FE5
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_5057
loc_1_4FE5:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_5057
        db  08Bh, 0FFh
loc_1_4FF0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_5040
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_5016
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_501D
loc_1_5016:
        mov    dword ptr [ebp - 10h], 1
loc_1_501D:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_5030
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_505A
        db  08Dh, 064h, 024h, 000h
loc_1_5030:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_505A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5040:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_505A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5050:
        mov    dword ptr [ebp - 4], 20h
loc_1_5057:
        mov    esi, dword ptr [ebp + 8]
loc_1_505A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_5151
loc_1_5065:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebp - 4]
        and    eax, ebx
        test   al, 9
        jne    SHORT loc_1_50BD
        test   al, 2
        je     SHORT loc_1_50A9
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 036h, 0B7h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_1_50A9
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        test   dword ptr [ecx + eax*4], edx
        jne    SHORT loc_1_50BD
loc_1_50A9:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_50B4
        test   bl, 4
        jne    SHORT loc_1_50BD
loc_1_50B4:
        test   bl, 20h
        je     NEAR PTR loc_1_5146
loc_1_50BD:
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_5332
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_5146
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_50FF
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_50FF:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_5110
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_5116
        db  08Bh, 0FFh
loc_1_5110:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_5116:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_5125
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_5338
loc_1_5125:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_53DC
        mov    edi, ebx
        jmp    SHORT loc_1_5149
loc_1_5146:
        mov    edi, dword ptr [edi + 10h]
loc_1_5149:
        or     edi, edi
        jne    NEAR PTR loc_1_5065
loc_1_5151:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_5160:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5170
        pop    ebp
        ret    4
sub_1_5170:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_51EE
loc_1_5180:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_51E7
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_5338
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_51E7
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_51AC
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_51AC:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_51C0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_51C6
        db  005h, 000h, 000h, 000h, 000h
loc_1_51C0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_51C6:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_53DC
        mov    edi, ebx
        jmp    SHORT loc_1_51EA
loc_1_51E7:
        mov    edi, dword ptr [edi + 10h]
loc_1_51EA:
        or     edi, edi
        jne    SHORT loc_1_5180
loc_1_51EE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5200:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_5215
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_5215:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_5251
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_5242
        cmp    bl, 0ffh
        jne    SHORT loc_1_5242
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_5242
        test   al, 8
        je     SHORT loc_1_523B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_523B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_5242:
        test   al, 4
        jne    SHORT loc_1_524A
        test   al, 2
        je     SHORT loc_1_5269
loc_1_524A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_5251:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_5269
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_5269
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_5269:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh
        db  05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h
        db  024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch
        db  024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch
        db  000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh
        db  04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_52EA:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_52F0:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_52F6:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_52FC:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_5302:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_5308:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
sub_1_530E:
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_5314:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_531A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_5326:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_5332:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_5338:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_533E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_5344:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_535C:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
        db  0CDh, 020h, 048h, 080h, 028h, 000h
        db  0CDh, 020h, 049h, 080h, 028h, 000h
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 057h, 080h, 028h, 000h
        db  0CDh, 020h, 058h, 080h, 028h, 000h
        db  0CDh, 020h, 059h, 080h, 028h, 000h
        db  0CDh, 020h, 05Ah, 080h, 028h, 000h
        db  0CDh, 020h, 05Bh, 080h, 028h, 000h
        db  0CDh, 020h, 05Ch, 080h, 028h, 000h
        db  0CDh, 020h, 05Dh, 080h, 028h, 000h
        db  0CDh, 020h, 05Eh, 080h, 028h, 000h
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_53B2:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_53B8:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_53BE:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_53C4:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_53D0:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_53D6:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_53DC:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_5400:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
sub_1_5406:
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
sub_1_5412:
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_5424:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_5448:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
sub_1_544E:
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_5454:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_545A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_5460:
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
sub_1_54A8:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
sub_1_54B4:
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_54BC:
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
sub_1_54DC:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_54FC
        or     ecx, ecx
        je     SHORT loc_1_5512
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_5510
loc_1_54FC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_5510
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_5510:
        rep movsb
loc_1_5512:
        popfd
        pop    edi
        pop    esi
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 045h, 04Ch, 04Eh, 04Bh
        db  033h, 020h, 020h, 020h, 000h, 040h, 000h, 054h, 029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 045h, 04Ch, 04Eh, 04Bh, 033h, 000h, 000h, 000h
loc_1_5574:
        inc    ebp
        dec    esp
        dec    esi
        dec    ebx
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        dec    ebp
        popad
        db  078h, 069h
        insd
        jne    SHORT loc_1_55F0
        dec    ebp
        jne    SHORT loc_1_55F2
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_55D9
        imul   esi, dword ptr [ebx + 74h], 0
        dec    ebp
        popad
        js     SHORT loc_1_5601
        insd
        jne    SHORT loc_1_5608
        dec    ebp
        db  075h, 06Ch
        je     SHORT loc_1_5609
        arpl   word ptr [ecx + 73h], sp
        db  074h, 04Ch
        imul   esi, dword ptr [ebx + 74h], 0
        inc    esp
        push   esp
        inc    ecx
        push   ebx
        outsd
        jne    SHORT loc_1_5625
        arpl   word ptr [ebp + 43h], sp
        outsd
        jne    SHORT loc_1_5627
        db  074h, 000h
        db  000h
loc_1_55BC:
        inc    esp
        push   esp
        inc    ecx
        push   ebx
        outsd
        jne    SHORT loc_1_5635
        arpl   word ptr [ebp + 43h], sp
        outsd
        db  075h, 06Eh
        je     SHORT loc_1_55CB
loc_1_55CB:
        add    byte ptr [esi + 45h], cl
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    esi
loc_1_55D9:
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        inc    edx
        db  075h, 073h
        push   esp
        db  079h, 070h
        add    byte ptr gs:[edx + 75h], al
        db  073h, 054h
loc_1_55F0:
        db  079h, 070h
loc_1_55F2:
        add    byte ptr gs:[ecx + 4fh], cl
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        db  000h
loc_1_5600:
        dec    ecx
loc_1_5601:
        dec    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
loc_1_5608:
        push   ebx
loc_1_5609:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 4ch], dl
        dec    edi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5614:
        push   ebx
        dec    esp
        dec    edi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ecx
        inc    esp
        push   eax
        dec    edi
        push   edx
        push   esp
        add    byte ptr [eax], al
loc_1_5624:
        dec    ecx
loc_1_5625:
        inc    esp
        push   eax
loc_1_5627:
        dec    edi
        push   edx
        push   esp
        add    byte ptr [eax], al
        dec    ecx
        dec    esi
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
loc_1_5635:
        add    byte ptr [eax], al
        db  000h
loc_1_5638:
        dec    ecx
        dec    esi
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
loc_1_5648:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 0d000201h
        add    al, byte ptr [edx]
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [ecx]
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [edx]
        add    al, byte ptr [ecx]
        add    eax, 6010202h
        add    al, byte ptr [edx]
        add    dword ptr [edi], eax
        add    al, byte ptr [edx]
        add    dword ptr [edx + eax], eax
        add    al, byte ptr [ecx]
loc_1_56E4:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
loc_1_5740:
        inc    ebp
        dec    esp
        dec    esi
        dec    ebx
        xor    esp, dword ptr [eax]
        inc    ebp
        je     SHORT loc_1_57B1
        db  065h, 072h, 06Eh
        je     NEAR PTR loc_1_576F
        inc    ecx
        popad
        jo     SHORT loc_1_57C8
        jb     NEAR PTR loc_1_5757
loc_1_5757:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_576F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5799:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57B1:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_57C8:
        add    byte ptr [eax], al
loc_1_57CA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57CE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57D2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57D6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57DA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57E2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57E6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
