; irmatrac_lcode_01_02.asm -- IRMATRAC object 1 segment _LTEXT  (part 2/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_55A0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_55B0
        pop    ebp
        ret    4
sub_1_55B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 14h]
        or     edi, edi
        je     SHORT loc_1_562E
loc_1_55C0:
        inc    dword ptr [edi + 18h]
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_1_55D6
        mov    byte ptr [edi + 1eh], 0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_5838
loc_1_55D6:
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_5627
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_55EC
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_55EC:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_5600
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_5606
        db  005h, 000h, 000h, 000h, 000h
loc_1_5600:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_1_5606:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 2ah]
        mov    eax, 1
        mov    cl, byte ptr [edi + 1ch]
        shl    eax, cl
        or     dword ptr [esi + 2eh], eax
        push   0
        push   1fh
        push   edi
        call   sub_1_58DC
        mov    edi, ebx
        jmp    SHORT loc_1_562A
loc_1_5627:
        mov    edi, dword ptr [edi + 10h]
loc_1_562A:
        or     edi, edi
        jne    SHORT loc_1_55C0
loc_1_562E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    edx, dword ptr [ebp + 10h]
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 4]
        xor    eax, eax
        mov    al, byte ptr [edx + 2]
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        xor    eax, eax
        mov    al, byte ptr [edx + 3]
        shl    eax, 10h
        or     ecx, eax
        xor    eax, eax
        mov    al, byte ptr [edx + 5]
        or     ecx, eax
        mov    al, byte ptr [edx]
        and    al, byte ptr [edx + 2]
        and    al, 80h
        mov    byte ptr [ebp - 1], al
        or     al, al
        mov    eax, dword ptr [ebp + 8]
        je     SHORT loc_1_5692
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_1_5692
        cmp    dword ptr [eax + 0ch], 0
        je     SHORT loc_1_5692
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_5692:
        mov    esi, dword ptr [eax + 10h]
        or     esi, esi
        jne    SHORT loc_1_56A2
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_56A2:
        test   esi, 20h
        je     SHORT loc_1_56B3
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_56B3:
        mov    al, byte ptr [edx]
        shr    al, 7
        or     al, al
        je     SHORT loc_1_56D0
        test   esi, 10h
        je     SHORT loc_1_56D0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_56D0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [eax]
        and    cl, 80h
        mov    byte ptr [ebp - 1], cl
        or     cl, cl
        je     SHORT loc_1_575A
        mov    cl, byte ptr [eax]
        cmp    cl, 0ffh
        jne    SHORT loc_1_56EC
        cmp    byte ptr [eax + 1], 0ffh
        je     SHORT loc_1_56F7
loc_1_56EC:
        cmp    cl, 0c0h
        jne    SHORT loc_1_5713
        cmp    byte ptr [eax + 1], 0
        jne    SHORT loc_1_5713
loc_1_56F7:
        cmp    byte ptr [eax + 2], 0ffh
        jne    SHORT loc_1_5713
        cmp    byte ptr [eax + 3], 0ffh
        jne    SHORT loc_1_5713
        cmp    byte ptr [eax + 4], 0ffh
        jne    SHORT loc_1_5713
        cmp    byte ptr [eax + 5], 0ffh
        mov    byte ptr [ebp - 1], 1
        je     SHORT loc_1_5717
loc_1_5713:
        mov    byte ptr [ebp - 1], 0
loc_1_5717:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_5740
        test   esi, 8
        je     SHORT loc_1_5730
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_1_5730:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5740:
        test   esi, 6000h
        je     SHORT loc_1_5751
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_5751:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_575A:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5770:
        push   esi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep outsb
        pop    esi
        ret    0ch
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h
        db  06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch
        db  024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h
sub_1_57AD:
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep insb
        pop    edi
        ret    0ch
        db  057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h
        db  06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch
        db  024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_57EA:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_57F0:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_57F6:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_57FC:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_5802:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_5808:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
sub_1_580E:
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_5814:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_581A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_5826:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
sub_1_582C:
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_5832:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_5838:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_583E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_5844:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
sub_1_584A:
        db  0CDh, 020h, 041h, 080h, 028h, 000h
sub_1_5850:
        db  0CDh, 020h, 009h, 080h, 028h, 000h
sub_1_5856:
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_585C:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
sub_1_5862:
        db  0CDh, 020h, 048h, 080h, 028h, 000h
sub_1_5868:
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
sub_1_58B2:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_58B8:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_58BE:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_58C4:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_58CA:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_58D0:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_58D6:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_58DC:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_5900:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
sub_1_5906:
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
sub_1_590C:
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
loc_1_5912:
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
sub_1_591E:
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_5924:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_5948:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_5954:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_595A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_5960:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
        db  0CDh, 020h, 042h, 080h, 028h, 000h
sub_1_596C:
        db  0CDh, 020h, 012h, 080h, 028h, 000h
sub_1_5972:
        db  0CDh, 020h, 013h, 080h, 028h, 000h
sub_1_5978:
        db  0CDh, 020h, 014h, 080h, 028h, 000h
sub_1_597E:
        db  0CDh, 020h, 015h, 080h, 028h, 000h
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_59A8:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_59BC:
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
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 049h, 052h, 04Dh, 041h
        db  054h, 052h, 041h, 043h, 000h, 040h, 000h, 054h, 0D5h, 034h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 049h, 052h, 04Dh, 041h
        db  054h, 052h, 041h, 043h, 000h, 000h, 000h, 000h
loc_1_5A80:
        dec    ecx
        push   edx
        dec    ebp
        inc    ecx
        push   esp
        push   edx
        inc    ecx
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
        push   ebp
        push   edx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [edx + 55h], al
        push   edx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [edx + 55h], al
        push   ebx
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        add    byte ptr [edx + 55h], al
        push   ebx
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        add    byte ptr [ecx + 4fh], cl
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4fh], cl
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4eh], cl
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4eh], cl
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ebp + ecx*2 + 41h], al
        inc    ebx
        dec    eax
        inc    ecx
        dec    esi
        dec    esi
        inc    ebp
        dec    esp
        add    byte ptr [eax], al
loc_1_5AE8:
        inc    esp
        dec    ebp
        inc    ecx
        inc    ebx
        dec    eax
        inc    ecx
        dec    esi
        dec    esi
        inc    ebp
        dec    esp
        add    byte ptr [eax], al
        inc    esp
        inc    ecx
        push   esp
        inc    ecx
        push   edx
        inc    ecx
        push   esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5B00:
        inc    esp
        inc    ecx
        push   esp
        inc    ecx
        push   edx
        inc    ecx
        push   esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ebp
        inc    ecx
        push   edx
        dec    esp
        pop    ecx
        push   edx
        inc    ebp
        dec    esp
        inc    ebp
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5B1C:
        inc    ebp
        inc    ecx
        push   edx
        dec    esp
        pop    ecx
        push   edx
        inc    ebp
        dec    esp
        inc    ebp
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ebx
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebx + 41h], al
        inc    edx
        dec    esp
        inc    ebp
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ecx + eax*2 + 54h], al
        inc    ecx
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        inc    esi
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5B54:
        inc    esp
        inc    ecx
        push   esp
        inc    ecx
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        inc    esi
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        xor    edx, dword ptr [edx + 45h]
        inc    ebx
        push   esi
        inc    edx
        push   ebp
        inc    esi
        inc    ebx
        dec    edi
        push   ebp
        dec    esi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [esi + 44h], cl
        dec    ecx
        push   ebx
        xor    edx, dword ptr [edx + 45h]
        inc    ebx
        push   esi
        inc    edx
        push   ebp
        inc    esi
        inc    ebx
        dec    edi
        push   ebp
        dec    esi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [esi + 44h], cl
        dec    ecx
        push   ebx
        xor    ecx, dword ptr [ebp + 41h]
        pop    eax
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        dec    ebp
        dec    ecx
        push   esp
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [esi + 44h], cl
        dec    ecx
        push   ebx
        xor    ecx, dword ptr [ebp + 41h]
        pop    eax
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        dec    ebp
        dec    ecx
        push   esp
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [esi + 44h], cl
        dec    ecx
        push   ebx
        xor    ecx, dword ptr [ebp + 41h]
        pop    eax
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        dec    esp
        inc    ebp
        dec    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5BC8:
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        xor    ecx, dword ptr [ebp + 41h]
        pop    eax
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        dec    esp
        inc    ebp
        dec    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        dec    esp
        dec    edi
        push   esp
        dec    esi
        push   ebp
        dec    ebp
        inc    edx
        inc    ebp
        push   edx
        add    byte ptr [eax], al
loc_1_5BE8:
        push   ebx
        dec    esp
        dec    edi
        push   esp
        dec    esi
        push   ebp
        dec    ebp
        inc    edx
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        dec    ecx
        push   edx
        dec    ebp
        inc    ecx
        push   esp
        push   edx
        inc    ecx
        inc    ebx
        and    byte ptr [esi + 44h], cl
        dec    ecx
        push   ebx
        and    byte ptr [ebx], dh
        pop    eax
        and    byte ptr [eax], al
        add    byte ptr [ecx + 52h], cl
        dec    ebp
        inc    ecx
        push   esp
        push   edx
        inc    ecx
        inc    ebx
        and    byte ptr [esi + 44h], cl
        dec    ecx
        push   ebx
        and    byte ptr [ebx], dh
        pop    eax
        and    byte ptr [eax], al
        add    byte ptr [edx], al
        add    ecx, dword ptr [edx]
        or     eax, dword ptr [esi + eax]
        add    eax, 7
        add    dword ptr [edx + eax], eax
        add    eax, 6000003h
        pop    es
loc_1_5C30:
        inc    esp
        inc    ebx
        inc    ecx
        dec    ebp
        inc    ecx
        inc    ebx
        inc    edx
        dec    ecx
        dec    esi
        add    byte ptr [eax], al
loc_1_5C3C:
        or     eax, dword ptr [eax]
        or     eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5C44:
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
        add    eax, 1000201h
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
        add    eax, 6020101h
        add    dword ptr [ecx], eax
        add    al, byte ptr [edi]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    al, byte ptr [edx]
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx + eax]
        add    al, byte ptr [edx]
        add    eax, 6020202h
        add    al, byte ptr [edx]
        add    al, byte ptr [edi]
        add    al, byte ptr [edx]
        add    al, byte ptr [ecx]
        add    dword ptr [ecx], eax
        add    byte ptr [edx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], al
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
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
        add    eax, 6020101h
        add    dword ptr [ecx], eax
        add    al, byte ptr [edi]
        add    dword ptr [ecx], eax
        add    cl, byte ptr [ecx + 72h]
        insd
        popad
        db  074h, 072h
        popad
        arpl   word ptr [eax], sp
        push   esp
        outsd
        imul   esp, dword ptr [ebp + 6eh], 20h
        push   edx
        imul   ebp, dword ptr [esi + 67h], 74654e20h
        db  077h, 06Fh
        db  072h, 06Bh
        and    byte ptr [ebx + 61h], al
        db  072h, 064h
        and    byte ptr [esi + 6fh], ah
        db  072h, 020h
        push   eax
        inc    ebx
        and    byte ptr [ecx + 2fh], cl
        dec    edi
        and    byte ptr [edx + 75h], ah
        db  073h, 02Eh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5D80:
        dec    ecx
        db  072h, 06Dh
        popad
        db  074h, 072h
        popad
        arpl   word ptr [eax], sp
        push   esp
        outsd
        imul   esp, dword ptr [ebp + 6eh], 20h
        push   edx
        imul   ebp, dword ptr [esi + 67h], 74654e20h
        db  077h, 06Fh
        db  072h, 06Bh
        and    byte ptr [ebx + 61h], al
        db  072h, 064h
        and    byte ptr [esi + 6fh], ah
        db  072h, 020h
        dec    ebp
        inc    ebx
        inc    ecx
        and    byte ptr [edx + 75h], ah
        db  073h, 02Eh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5DB1:
        add    byte ptr [eax], al
loc_1_5DB3:
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
