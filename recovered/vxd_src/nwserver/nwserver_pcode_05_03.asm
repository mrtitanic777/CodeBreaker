; nwserver_pcode_05_03.asm -- NWSERVER object 5 segment _PTEXT  (part 3/3)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_9702:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    esi, dword ptr [eax]
        mov    eax, dword ptr [esi + 40h]
        mov    bl, byte ptr [eax - 4]
        cmp    bl, 1
        jbe    SHORT loc_5_9774
        mov    edi, dword ptr [esp + 14h]
loc_5_971C:
        mov    eax, dword ptr [esi + 40h]
        movzx  ecx, bl
        movzx  edx, word ptr [eax - 4]
        cmp    edx, ecx
        jae    SHORT loc_5_972C
        jmp    SHORT loc_5_9736
loc_5_972C:
        cmp    dword ptr [eax + ecx*4], edx
        mov    eax, 1
        ja     SHORT loc_5_9738
loc_5_9736:
        sub    eax, eax
loc_5_9738:
        or     eax, eax
        je     SHORT loc_5_976D
        or     edi, edi
        jne    SHORT loc_5_9763
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    ebp, dword ptr [esi + 40h]
        mov    dx, word ptr [eax + 491h]
        mov    eax, dword ptr [ebp + ecx*4]
        sub    dx, word ptr [eax + 8]
        cmp    dx, 1
        sbb    eax, eax
        neg    eax
        or     eax, eax
        je     SHORT loc_5_976D
loc_5_9763:
        push   ebx
        push   esi
        call   sub_5_437F
        add    esp, 8
loc_5_976D:
        dec    bl
        cmp    bl, 1
        ja     SHORT loc_5_971C
loc_5_9774:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_9779:
        push   ebx
        push   esi
        push   edi
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [eax]
        mov    bl, byte ptr [eax + 491h]
        mov    eax, dword ptr [esi + 44h]
        db  08Bh, 00Dh, 094h, 0B8h, 000h, 000h
        mov    eax, dword ptr [eax + edi*4]
        mov    ax, word ptr [eax + 2]
        mov    word ptr [ecx + 491h], ax
        mov    eax, dword ptr [esi + 44h]
        mov    eax, dword ptr [eax + edi*4]
        mov    ax, word ptr [eax]
        push   eax
        db  0E8h, 04Ah, 094h, 0FAh, 0FFh
        add    esp, 4
        db  08Bh, 00Dh, 094h, 0B8h, 000h, 000h
        movzx  ax, bl
        mov    word ptr [ecx + 491h], ax
        push   edi
        push   esi
        db  0E8h, 0A9h, 0D0h, 0FAh, 0FFh
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_97D5:
        push   ebx
        push   esi
        push   edi
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [eax]
        mov    eax, dword ptr [esi + 44h]
        mov    bl, byte ptr [eax - 4]
        or     bl, bl
        je     SHORT loc_5_9837
loc_5_97ED:
        mov    eax, dword ptr [esi + 44h]
        movzx  ecx, bl
        movzx  edx, word ptr [eax - 4]
        cmp    edx, ecx
        jae    SHORT loc_5_97FD
        jmp    SHORT loc_5_9807
loc_5_97FD:
        cmp    dword ptr [eax + ecx*4], edx
        mov    eax, 1
        ja     SHORT loc_5_9809
loc_5_9807:
        sub    eax, eax
loc_5_9809:
        or     eax, eax
        je     SHORT loc_5_9833
        or     edi, edi
        jne    SHORT loc_5_982A
        mov    eax, dword ptr [esi + 44h]
        db  08Bh, 015h, 094h, 0B8h, 000h, 000h
        mov    eax, dword ptr [eax + ecx*4]
        mov    ax, word ptr [eax + 2]
        cmp    word ptr [edx + 491h], ax
        jne    SHORT loc_5_9833
loc_5_982A:
        push   ecx
        call   sub_5_9779
        add    esp, 4
loc_5_9833:
        dec    bl
        jne    SHORT loc_5_97ED
loc_5_9837:
        or     edi, edi
        je     SHORT loc_5_9864
        mov    edi, dword ptr [esi + 48h]
        or     edi, edi
        je     SHORT loc_5_9864
loc_5_9842:
        mov    ebx, dword ptr [edi]
        push   dword ptr [edi + 4]
        push   dword ptr [edi + 8]
        call   sub_5_A7A2
        add    esp, 8
        push   edi
        db  0E8h, 038h, 0ABh, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 48h], ebx
        mov    edi, ebx
        or     ebx, ebx
        jne    SHORT loc_5_9842
loc_5_9864:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_9868:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 6ah], 0
        je     SHORT loc_5_98C4
        db  08Bh, 015h, 050h, 0B4h, 000h, 000h
loc_5_987E:
        db  081h, 0FAh, 050h, 0B4h, 000h, 000h
        je     SHORT loc_5_9898
        mov    ecx, edx
        mov    edx, dword ptr [edx]
        cmp    ecx, eax
        je     SHORT loc_5_987E
        mov    esi, dword ptr [eax + 14h]
        cmp    dword ptr [ecx + 14h], esi
        jne    SHORT loc_5_987E
        jmp    SHORT loc_5_98C4
loc_5_9898:
        mov    dword ptr [ebp - 0ch], 0
        add    eax, 73h
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 4], 0
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 004h, 000h, 093h, 004h
        add    esp, 0ch
loc_5_98C4:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_5_98C9:
        push   ebx
        push   esi
        push   edi
        push   1
        call   sub_5_9702
        add    esp, 4
        push   1
        call   sub_5_97D5
        add    esp, 4
        push   1
        call   sub_5_7FDD
        mov    ebx, dword ptr [esp + 20h]
        add    esp, 4
        push   ebx
        call   sub_5_9868
        add    esp, 4
        mov    byte ptr [ebx + 6ah], 0
        mov    byte ptr [ebx + 72h], 0dh
        db  0BEh, 014h, 0B2h, 000h, 000h
        lea    edi, [ebx + 73h]
        movsd
        movsd
        movsd
        movsb
        sub    ax, ax
        mov    dword ptr [ebx + 14h], 0
        mov    word ptr [ebx + 18h], 100h
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_991F:
        push   0
        call   sub_5_9702
        add    esp, 4
        push   0
        call   sub_5_97D5
        add    esp, 4
        push   0
        call   sub_5_7FDD
        add    esp, 4
        sub    ax, ax
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_5_9950:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 24h]
        movzx  eax, word ptr [esi + 0ch]
        movzx  ecx, word ptr [esi + 0ah]
        movzx  edx, word ptr [esi + 8]
        movzx  edi, word ptr [esi + 6]
        movzx  ebx, word ptr [esi + 2]
        movzx  ebp, word ptr [esi]
        cmp    ebp, 7bch
        jae    SHORT loc_5_9981
        mov    ebp, 7bch
        jmp    SHORT loc_5_998E
loc_5_9981:
        cmp    ebp, 83bh
        jbe    SHORT loc_5_998E
        mov    ebp, 83bh
loc_5_998E:
        sub    ebp, 7bch
        cmp    ebx, 1
        jae    SHORT loc_5_99A0
        mov    ebx, 1
        jmp    SHORT loc_5_99AA
loc_5_99A0:
        cmp    ebx, 0ch
        jbe    SHORT loc_5_99AA
        mov    ebx, 0ch
loc_5_99AA:
        cmp    edi, 1
        jae    SHORT loc_5_99B6
        mov    edi, 1
        jmp    SHORT loc_5_99C0
loc_5_99B6:
        cmp    edi, 1fh
        jbe    SHORT loc_5_99C0
        mov    edi, 1fh
loc_5_99C0:
        cmp    edx, 17h
        jbe    SHORT loc_5_99CA
        mov    edx, 17h
loc_5_99CA:
        cmp    ecx, 3bh
        jbe    SHORT loc_5_99D4
        mov    ecx, 3bh
loc_5_99D4:
        cmp    eax, 3bh
        jbe    SHORT loc_5_99DE
        mov    eax, 3bh
loc_5_99DE:
        shr    eax, 1
        shl    bx, 5
        shl    bp, 9
        or     bx, bp
        or     bx, di
        mov    word ptr [esp + 16h], bx
        shl    cx, 5
        shl    dx, 0bh
        or     cx, dx
        or     cx, ax
        mov    word ptr [esp + 14h], cx
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 10h], eax
        push   eax
        db  0E8h, 014h, 0AAh, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [esp + 18h], eax
        mov    dword ptr [esp + 1ch], edx
        mov    eax, dword ptr [esp + 28h]
        mov    edx, 23c34600h
        imul   edx
        lea    ecx, [esp + 18h]
        sub    dword ptr [ecx], eax
        sbb    dword ptr [ecx + 4], edx
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        db  0E8h, 0E2h, 0A9h, 0FAh, 0FFh
        add    esp, 8
        mov    dword ptr [esp + 10h], eax
        mov    ax, word ptr [esp + 12h]
        shr    ax, 9
        add    ax, 7bch
        mov    word ptr [esi], ax
        mov    ax, word ptr [esp + 12h]
        and    ax, 1e0h
        shr    ax, 5
        mov    word ptr [esi + 2], ax
        mov    ax, word ptr [esp + 12h]
        and    ax, 1fh
        mov    word ptr [esi + 6], ax
        mov    ax, word ptr [esp + 10h]
        shr    ax, 0bh
        mov    word ptr [esi + 8], ax
        mov    ax, word ptr [esp + 10h]
        and    ax, 7e0h
        shr    ax, 5
        mov    word ptr [esi + 0ah], ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_5_9A9B:
        sub    esp, 104h
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 110h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 8]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    edi, [esp + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   1
        lea    eax, [esp + 10h]
        push   0
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        db  0E8h, 03Fh, 0A9h, 0FAh, 0FFh
        add    esp, 14h
        push   dword ptr [esp + 118h]
        lea    eax, [esp + 0ch]
        push   dword ptr [esp + 118h]
        push   eax
        db  0E8h, 0C0h, 0A2h, 0FAh, 0FFh
        add    esp, 0ch
        pop    edi
        pop    esi
        add    esp, 104h
        ret
sub_5_9B16:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, dword ptr [esp + 0ch]
        lea    eax, [ecx - 1]
        mov    byte ptr [edi], al
        inc    edi
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        pop    edi
        pop    esi
        ret
sub_5_9B44:
        sub    ecx, ecx
        db  0BAh, 030h, 0B2h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
loc_5_9B4F:
        cmp    dword ptr [edx], eax
        je     SHORT loc_5_9B64
        add    edx, 6
        inc    ecx
        db  081h, 0FAh, 054h, 0B2h, 000h, 000h
        jb     SHORT loc_5_9B4F
        mov    ax, 0ffh
loc_5_9B63:
        ret
loc_5_9B64:
        imul   ecx, ecx, 6
        db  066h, 08Bh, 081h, 034h, 0B2h, 000h, 000h
        jmp    SHORT loc_5_9B63
sub_5_9B70:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        db  08Bh, 035h, 024h, 0B2h, 000h, 000h
        lea    edi, [ecx - 1]
        db  081h, 0FEh, 024h, 0B2h, 000h, 000h
        je     SHORT loc_5_9BBC
loc_5_9B95:
        mov    eax, dword ptr [esi + 8]
        movzx  ecx, byte ptr [eax + 18h]
        cmp    ecx, edi
        jne    SHORT loc_5_9BB2
        push   edi
        add    eax, 19h
        push   eax
        push   ebx
        db  0E8h, 05Dh, 07Ch, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_9BC2
loc_5_9BB2:
        mov    esi, dword ptr [esi]
        db  081h, 0FEh, 024h, 0B2h, 000h, 000h
        jne    SHORT loc_5_9B95
loc_5_9BBC:
        sub    eax, eax
loc_5_9BBE:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_9BC2:
        mov    eax, esi
        jmp    SHORT loc_5_9BBE
sub_5_9BC6:
        push   ebx
        push   esi
        db  0A1h, 024h, 0B2h, 000h, 000h
        db  03Dh, 024h, 0B2h, 000h, 000h
        je     SHORT loc_5_9C0F
        mov    esi, dword ptr [esp + 0ch]
loc_5_9BD8:
        lea    ecx, [eax + 0eh]
        mov    edx, esi
loc_5_9BDD:
        mov    bl, byte ptr [ecx]
        cmp    bl, byte ptr [edx]
        jne    SHORT loc_5_9BFD
        or     bl, bl
        je     SHORT loc_5_9BF9
        mov    bl, byte ptr [ecx + 1]
        cmp    bl, byte ptr [edx + 1]
        jne    SHORT loc_5_9BFD
        add    ecx, 2
        add    edx, 2
        or     bl, bl
        jne    SHORT loc_5_9BDD
loc_5_9BF9:
        sub    ecx, ecx
        jmp    SHORT loc_5_9C02
loc_5_9BFD:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_5_9C02:
        or     ecx, ecx
        je     SHORT loc_5_9C11
        mov    eax, dword ptr [eax]
        db  03Dh, 024h, 0B2h, 000h, 000h
        jne    SHORT loc_5_9BD8
loc_5_9C0F:
        sub    eax, eax
loc_5_9C11:
        pop    esi
        pop    ebx
        ret
sub_5_9C14:
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, 0ffh
        db  083h, 03Dh, 0D8h, 0B3h, 000h, 000h, 000h
        je     SHORT loc_5_9C7D
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 097h, 0F8h, 0FAh, 0FFh
        mov    esi, dword ptr [esp + 14h]
        add    esp, 4
        push   esi
        call   sub_5_9B70
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_9C5A
        lea    eax, [esp + 8]
        push   eax
        push   dword ptr [esp + 18h]
        push   esi
        call   sub_5_A7D9
        add    esp, 0ch
        movzx  edi, ax
loc_5_9C5A:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0A8h, 0F8h, 0FAh, 0FFh
        add    esp, 4
        or     edi, edi
        jne    SHORT loc_5_9C7D
        mov    eax, dword ptr [esp + 18h]
        or     eax, eax
        je     SHORT loc_5_9C7D
        push   eax
        push   esi
        call   sub_5_9DCA
        add    esp, 8
loc_5_9C7D:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 4
        ret
sub_5_9C85:
        push   esi
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 038h, 0F8h, 0FAh, 0FFh
        add    esp, 4
        mov    esi, 0ffh
        push   dword ptr [esp + 8]
        call   sub_5_9B70
        add    esp, 4
        or     eax, eax
        je     SHORT loc_5_9CB6
        push   dword ptr [eax + 8]
        call   sub_5_31A8
        add    esp, 4
        movzx  esi, ax
loc_5_9CB6:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 04Ch, 0F8h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, esi
        pop    esi
        ret
loc_5_9CC7:
        push   ebx
        push   esi
        push   edi
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0F4h, 0F7h, 0FAh, 0FFh
        add    esp, 4
        sub    esi, esi
        push   dword ptr [esp + 10h]
        call   sub_5_9BC6
        add    esp, 4
        cmp    eax, esi
        je     SHORT loc_5_9D17
        mov    esi, dword ptr [eax + 8]
        mov    edx, dword ptr [esp + 14h]
        movzx  ebx, byte ptr [esi + 18h]
        add    esi, 19h
        mov    edi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [eax + 8]
        mov    esi, 1
        movzx  eax, byte ptr [eax + 18h]
        mov    byte ptr [edx + eax], 0
loc_5_9D17:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0EBh, 0F7h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_9D2A:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 08Dh, 0F7h, 0FAh, 0FFh
        add    esp, 4
        sub    ebx, ebx
        push   dword ptr [esp + 34h]
        call   sub_5_9B70
        add    esp, 4
        mov    esi, eax
        cmp    esi, ebx
        je     SHORT loc_5_9DB3
        lea    eax, [esp + 10h]
        db  068h, 058h, 0B2h, 000h, 000h
        push   eax
        call   sub_5_9B16
        mov    ebp, dword ptr [esp + 40h]
        add    esp, 8
        mov    byte ptr [ebp], bl
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esi + 8]
        call   sub_5_24EC
        add    esp, 8
        mov    esi, eax
        cmp    esi, ebx
        je     SHORT loc_5_9DB3
        mov    eax, dword ptr [esi + 4]
        cmp    eax, ebx
        je     SHORT loc_5_9DB3
        push   7fh
        add    eax, 4
        push   eax
        db  0E8h, 089h, 0A5h, 0FAh, 0FFh
        add    esp, 8
        mov    ebx, eax
        mov    esi, dword ptr [esi + 4]
        mov    edi, ebp
        add    esi, 4
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    byte ptr [ebp + ebx], 0
loc_5_9DB3:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 04Fh, 0F7h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
sub_5_9DCA:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0EEh, 0F6h, 0FAh, 0FFh
        add    esp, 4
        sub    edi, edi
        push   dword ptr [esp + 30h]
        call   sub_5_9B70
        add    esp, 4
        mov    esi, eax
        cmp    esi, edi
        je     SHORT loc_5_9E33
        lea    eax, [esp + 0ch]
        db  068h, 068h, 0B2h, 000h, 000h
        push   eax
        call   sub_5_9B16
        add    esp, 8
        mov    ebx, dword ptr [esp + 34h]
        push   7fh
        push   ebx
        db  0E8h, 00Dh, 0A5h, 0FAh, 0FFh
        add    esp, 8
        mov    edi, eax
        lea    eax, [esp + 0ch]
        push   eax
        push   dword ptr [esi + 8]
        call   sub_5_24EC
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_9E33
        push   edi
        push   ebx
        push   eax
        call   sub_5_2DD8
        add    esp, 0ch
loc_5_9E33:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0CFh, 0F6h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
sub_5_9E49:
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 070h, 0F6h, 0FAh, 0FFh
        add    esp, 4
        db  08Bh, 015h, 024h, 0B2h, 000h, 000h
        sub    ecx, ecx
        mov    eax, dword ptr [esp + 10h]
        cmp    eax, ecx
        je     SHORT loc_5_9E7A
loc_5_9E6B:
        db  081h, 0FAh, 024h, 0B2h, 000h, 000h
        je     SHORT loc_5_9ED4
        mov    edx, dword ptr [edx]
        inc    ecx
        cmp    eax, ecx
        ja     SHORT loc_5_9E6B
loc_5_9E7A:
        db  081h, 0FAh, 024h, 0B2h, 000h, 000h
        je     SHORT loc_5_9ED4
        lea    edi, [edx + 0eh]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [esp + 14h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    esi, dword ptr [edx + 8]
        mov    ebx, dword ptr [esp + 18h]
        movzx  eax, byte ptr [esi + 18h]
        add    esi, 19h
        mov    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [edx + 8]
        mov    esi, 1
        movzx  eax, byte ptr [eax + 18h]
        mov    byte ptr [ebx + eax], 0
loc_5_9ED4:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 02Eh, 0F6h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_9EE7:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], 0ffh
        db  083h, 03Dh, 0D8h, 0B3h, 000h, 000h, 000h
        je     NEAR PTR loc_5_9FA5
        mov    esi, dword ptr [esp + 20h]
        push   200h
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        db  0E8h, 045h, 0A4h, 0FAh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_5_9FA5
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 096h, 0F5h, 0FAh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 1ch]
        call   sub_5_9B70
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_5_9F8F
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esp + 14h], eax
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [ebx], cl
        movzx  eax, cl
        lea    edi, [ebx + 1]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 14h]
        push   ebx
        push   eax
        call   sub_5_19D5
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_9F8F
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ebp + 8], eax
loc_5_9F8F:
        push   ebx
        db  0E8h, 0FBh, 0A3h, 0FAh, 0FFh
        add    esp, 4
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 06Ah, 0F5h, 0FAh, 0FFh
        add    esp, 4
loc_5_9FA5:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_9FB1:
        push   esi
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 00Ch, 0F5h, 0FAh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 8]
        call   sub_5_9B70
        add    esp, 4
        cmp    eax, 1
        sbb    esi, esi
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 032h, 0F5h, 0FAh, 0FFh
        add    esp, 4
        lea    eax, [esi + 1]
        pop    esi
        ret
sub_5_9FE2:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 20h]
        db  0A1h, 04Ch, 0B9h, 000h, 000h
        push   edi
        mov    eax, dword ptr [eax + esi*4]
        add    eax, 4
        push   eax
        call   sub_5_3371
        add    esp, 8
        cmp    eax, edi
        je     SHORT loc_5_A012
        mov    eax, 1
        jmp    NEAR PTR loc_5_A0AB
loc_5_A012:
        push   200h
        push   105h
        db  0E8h, 043h, 0A3h, 0FAh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_5_A02E
        sub    eax, eax
        jmp    SHORT loc_5_A0AB
loc_5_A02E:
        lea    eax, [esp + 10h]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 24h]
        add    eax, 0eh
        push   eax
        call   sub_5_9A9B
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_A0A0
        lea    eax, [esp + 14h]
        push   eax
        push   105h
        push   ebx
        db  0A1h, 04Ch, 0B9h, 000h, 000h
        push   1
        mov    eax, dword ptr [eax + esi*4]
        add    eax, 19h
        push   eax
        push   0
        db  0E8h, 0A9h, 09Dh, 0FAh, 0FFh
        add    esp, 18h
        mov    ebp, eax
        push   dword ptr [esp + 10h]
        db  0E8h, 065h, 09Dh, 0FAh, 0FFh
        add    esp, 4
        or     ebp, ebp
        jne    SHORT loc_5_A0A0
        push   0
        push   0
        push   3
        db  0A1h, 04Ch, 0B9h, 000h, 000h
        push   ebx
        mov    eax, dword ptr [eax + esi*4]
        add    eax, 4
        push   eax
        call   sub_5_3495
        add    esp, 14h
        cmp    eax, 1
        sbb    edi, edi
        neg    edi
loc_5_A0A0:
        push   ebx
        db  0E8h, 0EAh, 0A2h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, edi
loc_5_A0AB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_5_A0B3:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 12h], 0
        cmp    word ptr [esp + 1ch], 0
        mov    word ptr [esp + 10h], 0
        je     NEAR PTR loc_5_A19C
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    eax, dword ptr [eax]
        cmp    byte ptr [eax + 6ah], 3
        je     NEAR PTR loc_5_A19C
        test   byte ptr [eax + 1ah], 10h
        jne    NEAR PTR loc_5_A19C
        mov    ebp, dword ptr [esp + 18h]
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_5_A102
        mov    ax, 0d1h
        jmp    NEAR PTR loc_5_A19F
loc_5_A102:
        push   200h
        push   105h
        db  0E8h, 053h, 0A2h, 0FAh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_5_A120
        mov    ax, 96h
        jmp    SHORT loc_5_A19F
loc_5_A120:
        db  0BEh, 078h, 0B2h, 000h, 000h
        mov    edi, ebx
        movsd
        movsw
        movsb
        mov    edi, dword ptr [ebp + 0ch]
        add    edi, 16h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 7]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 12h]
        push   eax
        push   0
        db  0A1h, 094h, 0B8h, 000h, 000h
        push   dword ptr [esp + 24h]
        push   ebx
        mov    eax, dword ptr [eax]
        add    eax, 73h
        push   eax
        push   0
        db  0E8h, 02Bh, 09Ch, 0FAh, 0FFh
        mov    ax, word ptr [esp + 2ah]
        add    esp, 18h
        not    ax
        test   word ptr [esp + 1ch], ax
        je     SHORT loc_5_A188
        mov    word ptr [esp + 10h], 0d3h
loc_5_A188:
        or     ebx, ebx
        je     SHORT loc_5_A195
        push   ebx
        db  0E8h, 0FEh, 0A1h, 0FAh, 0FFh
        add    esp, 4
loc_5_A195:
        mov    ax, word ptr [esp + 10h]
        jmp    SHORT loc_5_A19F
loc_5_A19C:
        sub    ax, ax
loc_5_A19F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_5_A1A7:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 12h], 0
        mov    word ptr [esp + 10h], 0
        mov    bx, word ptr [esp + 1ch]
        or     bx, bx
        je     NEAR PTR loc_5_A301
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    edx, dword ptr [eax]
        cmp    byte ptr [edx + 6ah], 3
        je     NEAR PTR loc_5_A301
        test   byte ptr [edx + 1ah], 10h
        jne    NEAR PTR loc_5_A301
        test   bl, 8
        je     SHORT loc_5_A1EF
        and    bx, 0fffeh
loc_5_A1EF:
        test   bl, 10h
        je     SHORT loc_5_A1F9
        and    bx, 0fffdh
loc_5_A1F9:
        test   bl, 3
        je     SHORT loc_5_A253
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, 0ffffffffh
        mov    edi, dword ptr [eax + 0ch]
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        movzx  eax, byte ptr [edx + 72h]
        cmp    ecx, eax
        jne    SHORT loc_5_A23C
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    eax, dword ptr [eax]
        movzx  ecx, byte ptr [eax + 72h]
        push   ecx
        add    eax, 73h
        push   eax
        mov    eax, dword ptr [esp + 20h]
        push   dword ptr [eax + 0ch]
        db  0E8h, 0D3h, 075h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_A253
loc_5_A23C:
        test   bl, 1
        je     SHORT loc_5_A245
        or     bx, 8
loc_5_A245:
        test   bl, 2
        je     SHORT loc_5_A24E
        or     bx, 10h
loc_5_A24E:
        and    bx, 0fffch
loc_5_A253:
        or     bx, bx
        jne    SHORT loc_5_A25D
        jmp    NEAR PTR loc_5_A301
loc_5_A25D:
        push   200h
        push   105h
        db  0E8h, 0F8h, 0A0h, 0FAh, 0FFh
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_5_A27E
        mov    ax, 96h
        jmp    NEAR PTR loc_5_A304
loc_5_A27E:
        db  0BEh, 080h, 0B2h, 000h, 000h
        mov    edi, ebp
        movsd
        movsw
        movsb
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, 0ffffffffh
        mov    edi, dword ptr [eax + 4]
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp + 7]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 12h]
        push   eax
        push   0
        push   ebx
        db  0A1h, 094h, 0B8h, 000h, 000h
        push   ebp
        mov    eax, dword ptr [eax]
        add    eax, 73h
        push   eax
        push   0
        db  0E8h, 0CFh, 09Ah, 0FAh, 0FFh
        mov    ax, word ptr [esp + 2ah]
        add    esp, 18h
        not    ax
        and    ax, bx
        je     SHORT loc_5_A2ED
        test   al, 80h
        mov    word ptr [esp + 10h], 0d3h
        jne    SHORT loc_5_A2ED
        mov    word ptr [esp + 10h], 0d6h
loc_5_A2ED:
        or     ebp, ebp
        je     SHORT loc_5_A2FA
        push   ebp
        db  0E8h, 099h, 0A0h, 0FAh, 0FFh
        add    esp, 4
loc_5_A2FA:
        mov    ax, word ptr [esp + 10h]
        jmp    SHORT loc_5_A304
loc_5_A301:
        sub    ax, ax
loc_5_A304:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_5_A30C:
        push   esi
        sub    eax, eax
        sub    edx, edx
        mov    ecx, dword ptr [esp + 8]
loc_5_A315:
        movzx  esi, byte ptr [edx + ecx]
        sub    esi, 30h
        cmp    esi, 9
        ja     SHORT loc_5_A32E
        imul   eax, eax, 0ah
        add    eax, esi
        inc    edx
        cmp    edx, 4
        jb     SHORT loc_5_A315
        jmp    SHORT loc_5_A333
loc_5_A32E:
        mov    eax, 0ffffffffh
loc_5_A333:
        pop    esi
        ret
loc_5_A335:
        sub    esp, 48h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 64h]
        cmp    eax, 1
        je     SHORT loc_5_A34F
        cmp    eax, 2
        je     SHORT loc_5_A36F
        jmp    NEAR PTR loc_5_A797
loc_5_A34F:
        mov    esi, dword ptr [esp + 5ch]
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [eax], ecx
        jmp    NEAR PTR loc_5_A797
loc_5_A36F:
        lea    eax, [esp + 38h]
        db  068h, 088h, 0B2h, 000h, 000h
        push   eax
        call   sub_5_9B16
        lea    eax, [esp + 40h]
        add    esp, 8
        mov    esi, dword ptr [esp + 5ch]
        push   eax
        push   esi
        call   sub_5_24EC
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_A3AE
        db  08Bh, 00Dh, 094h, 0B8h, 000h, 000h
        push   4
        mov    ecx, dword ptr [ecx]
        add    ecx, 14h
        push   ecx
        push   eax
        call   sub_5_2DD8
        add    esp, 0ch
loc_5_A3AE:
        lea    eax, [esp + 38h]
        db  068h, 090h, 0B2h, 000h, 000h
        push   eax
        call   sub_5_9B16
        lea    eax, [esp + 40h]
        add    esp, 8
        push   eax
        push   esi
        call   sub_5_24EC
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_A3E9
        db  08Bh, 00Dh, 094h, 0B8h, 000h, 000h
        push   4
        mov    ecx, dword ptr [ecx]
        add    ecx, 14h
        push   ecx
        push   eax
        call   sub_5_2DD8
        add    esp, 0ch
loc_5_A3E9:
        mov    ebx, dword ptr [esp + 60h]
        cmp    byte ptr [ebx + 14h], 0ch
        jne    NEAR PTR loc_5_A56B
        lea    eax, [ebx + 15h]
        push   0ch
        db  068h, 09Ch, 0B2h, 000h, 000h
        push   eax
        db  0E8h, 001h, 074h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_A46F
        sub    edi, edi
        mov    dword ptr [esp + 24h], edi
        lea    eax, [esp + 28h]
        push   edi
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 16h
        push   eax
        call   sub_5_9A9B
        add    esp, 0ch
        cmp    eax, edi
        jne    SHORT loc_5_A45C
        lea    eax, [esp + 24h]
        push   eax
        lea    eax, [esp + 30h]
        push   eax
        db  0A1h, 04Ch, 0B9h, 000h, 000h
        mov    eax, dword ptr [eax]
        add    eax, 19h
        push   eax
        push   dword ptr [esp + 34h]
        call   sub_5_AF56
        add    esp, 10h
        mov    edi, eax
        push   dword ptr [esp + 28h]
        db  0E8h, 083h, 099h, 0FAh, 0FFh
        add    esp, 4
loc_5_A45C:
        push   dword ptr [esp + 24h]
        push   edi
        push   ebx
        call   sub_5_2DD8
        add    esp, 0ch
        jmp    NEAR PTR loc_5_A797
loc_5_A46F:
        cmp    byte ptr [ebx + 14h], 0ch
        jne    NEAR PTR loc_5_A56B
        lea    eax, [ebx + 15h]
        push   0ch
        db  068h, 0ACh, 0B2h, 000h, 000h
        push   eax
        db  0E8h, 07Fh, 073h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_5_A56B
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 8
        push   eax
        call   sub_5_9FE2
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_5_A556
        push   200h
        push   105h
        db  0E8h, 0A8h, 09Eh, 0FAh, 0FFh
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_5_A797
        lea    edx, [ebp + 1]
        mov    byte ptr [ebp], 5ch
        mov    byte ptr [edx], 5ch
        inc    edx
        db  0BFh, 0ACh, 0B1h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BFh, 0ACh, 0B1h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edx, [edx + ecx - 1]
        mov    ecx, 0ffffffffh
        mov    byte ptr [edx], 5ch
        db  0A1h, 04Ch, 0B9h, 000h, 000h
        mov    edi, dword ptr [eax]
        add    edi, 4
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [edx + 1]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   ebp
        push   ebx
        call   sub_5_2DD8
        add    esp, 0ch
        jmp    NEAR PTR loc_5_A78E
loc_5_A556:
        push   1
        db  068h, 0BCh, 0B2h, 000h, 000h
        push   ebx
        call   sub_5_2DD8
        add    esp, 0ch
        jmp    NEAR PTR loc_5_A797
loc_5_A56B:
        cmp    byte ptr [ebx + 14h], 8
        jne    NEAR PTR loc_5_A797
        lea    eax, [ebx + 19h]
        push   eax
        call   sub_5_A30C
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
        db  0A1h, 02Ch, 0B9h, 000h, 000h
        mov    ecx, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 4], ecx
        jbe    NEAR PTR loc_5_A797
        lea    edi, [ebx + 15h]
        push   4
        db  068h, 0C0h, 0B2h, 000h, 000h
        push   edi
        db  0E8h, 061h, 072h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_5_A68F
        push   dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 8
        push   eax
        call   sub_5_9FE2
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_5_A67A
        push   200h
        push   105h
        db  0E8h, 088h, 09Dh, 0FAh, 0FFh
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_5_A797
        lea    edx, [ebp + 1]
        mov    byte ptr [ebp], 5ch
        mov    byte ptr [edx], 5ch
        inc    edx
        db  0BFh, 0ACh, 0B1h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BFh, 0ACh, 0B1h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        add    edx, ecx
        mov    byte ptr [edx], 5ch
        db  0A1h, 04Ch, 0B9h, 000h, 000h
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [eax + ecx*4]
        add    edi, 4
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [edx + 1]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   ebp
        push   ebx
        call   sub_5_2DD8
        add    esp, 0ch
        jmp    NEAR PTR loc_5_A78E
loc_5_A67A:
        push   1
        db  068h, 0C8h, 0B2h, 000h, 000h
        push   ebx
        call   sub_5_2DD8
        add    esp, 0ch
        jmp    NEAR PTR loc_5_A797
loc_5_A68F:
        push   4
        db  068h, 0CCh, 0B2h, 000h, 000h
        push   edi
        db  0E8h, 06Ch, 071h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_A709
        sub    edi, edi
        mov    dword ptr [esp + 1ch], edi
        lea    eax, [esp + 20h]
        push   edi
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 16h
        push   eax
        call   sub_5_9A9B
        add    esp, 0ch
        cmp    eax, edi
        jne    SHORT loc_5_A6F6
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 34h]
        push   eax
        db  0A1h, 04Ch, 0B9h, 000h, 000h
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + ecx*4]
        add    eax, 19h
        push   eax
        push   dword ptr [esp + 2ch]
        call   sub_5_AF56
        add    esp, 10h
        mov    edi, eax
        push   dword ptr [esp + 20h]
        db  0E8h, 0E9h, 096h, 0FAh, 0FFh
        add    esp, 4
loc_5_A6F6:
        push   dword ptr [esp + 1ch]
        push   edi
        push   ebx
        call   sub_5_2DD8
        add    esp, 0ch
        jmp    NEAR PTR loc_5_A797
loc_5_A709:
        push   4
        db  068h, 0D4h, 0B2h, 000h, 000h
        push   edi
        db  0E8h, 0F2h, 070h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_A797
        sub    ebp, ebp
        sub    edi, edi
        mov    dword ptr [esp + 14h], ebp
        cmp    dword ptr [esp + 10h], ebp
        jne    SHORT loc_5_A77C
        lea    eax, [esp + 18h]
        push   ebp
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 16h
        push   eax
        call   sub_5_9A9B
        add    esp, 0ch
        cmp    eax, ebp
        jne    SHORT loc_5_A77C
        lea    eax, [esp + 34h]
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_5_AE0D
        add    esp, 8
        mov    ebp, eax
        push   dword ptr [esp + 18h]
        db  0E8h, 07Ch, 096h, 0FAh, 0FFh
        add    esp, 4
        cmp    ebp, edi
        je     SHORT loc_5_A77C
        mov    edi, dword ptr [ebp + 1ch]
        or     edi, edi
        je     SHORT loc_5_A77C
        movzx  eax, word ptr [edi + 24h]
        movzx  ecx, word ptr [edi + 26h]
        add    eax, ecx
        mov    dword ptr [esp + 14h], eax
loc_5_A77C:
        push   dword ptr [esp + 14h]
        push   edi
        push   ebx
        call   sub_5_2DD8
        add    esp, 0ch
        or     ebp, ebp
        je     SHORT loc_5_A797
loc_5_A78E:
        push   ebp
        db  0E8h, 0FCh, 09Bh, 0FAh, 0FFh
        add    esp, 4
loc_5_A797:
        sub    ax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 48h
        ret
sub_5_A7A2:
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        push   3
        push   ebx
        push   ebx
        mov    esi, dword ptr [esp + 1ch]
        push   dword ptr [esp + 20h]
        push   esi
        db  0E8h, 041h, 096h, 0FAh, 0FFh
        add    esp, 14h
        mov    edi, eax
        cmp    esi, ebx
        je     SHORT loc_5_A7CD
        push   esi
        db  0E8h, 014h, 096h, 0FAh, 0FFh
        add    esp, 4
        mov    ebx, eax
loc_5_A7CD:
        or     edi, edi
        mov    eax, edi
        jne    SHORT loc_5_A7D5
        mov    eax, ebx
loc_5_A7D5:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_A7D9:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   200h
        push   64h
        db  0E8h, 078h, 09Bh, 0FAh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_5_A7FE
        mov    ax, 96h
        jmp    NEAR PTR loc_5_AD06
loc_5_A7FE:
        mov    byte ptr [ebx], 1
        mov    byte ptr [ebx + 1], 30h
        push   dword ptr [esp + 1ch]
        mov    word ptr [ebx + 2], 300h
        lea    eax, [ebx + 4]
        push   eax
        call   sub_5_9B16
        add    esp, 8
        push   dword ptr [esp + 24h]
        push   ebx
        call   sub_5_18BC
        add    esp, 8
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_ACFA
        mov    esi, dword ptr [esp + 20h]
        push   200h
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ecx, 16h
        push   ecx
        db  0E8h, 011h, 09Bh, 0FAh, 0FFh
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_A867
        mov    si, 96h
        jmp    NEAR PTR loc_5_ACFA
loc_5_A867:
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 24h]
        mov    byte ptr [ecx + 4], 1
        db  0C7h, 001h, 035h, 0A3h, 000h, 000h
        mov    edi, esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx + 10h], eax
        mov    byte ptr [ecx + 14h], 0
        mov    byte ptr [ecx + 15h], 2
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [esp + 14h]
        add    edi, 16h
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 14h]
        db  0C7h, 041h, 008h, 024h, 0B2h, 000h, 000h
        db  0A1h, 028h, 0B2h, 000h, 000h
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, ecx
        db  08Bh, 015h, 028h, 0B2h, 000h, 000h
        add    eax, 8
        mov    dword ptr [edx], eax
        mov    eax, ecx
        add    eax, 8
        db  0A3h, 028h, 0B2h, 000h, 000h
        mov    eax, dword ptr [esp + 24h]
        db  068h, 0DCh, 0B2h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 0ch], ecx
        lea    eax, [ebx + 2]
        mov    byte ptr [ebx], 2
        mov    byte ptr [ebx + 1], 30h
        push   eax
        call   sub_5_9B16
        lea    eax, [esp + 18h]
        add    esp, 8
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   ebx
        push   dword ptr [eax]
        call   sub_5_2583
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_ACFA
        push   1
        db  068h, 0E8h, 0B2h, 000h, 000h
        push   dword ptr [esp + 18h]
        call   sub_5_2DD8
        add    esp, 0ch
        mov    byte ptr [ebx], 2
        mov    byte ptr [ebx + 1], 30h
        db  068h, 0ECh, 0B2h, 000h, 000h
        lea    eax, [ebx + 2]
        push   eax
        call   sub_5_9B16
        lea    eax, [esp + 18h]
        add    esp, 8
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   ebx
        push   dword ptr [eax]
        call   sub_5_2583
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_ACFA
        push   1
        db  068h, 0F8h, 0B2h, 000h, 000h
        push   dword ptr [esp + 18h]
        call   sub_5_2DD8
        add    esp, 0ch
        mov    byte ptr [ebx], 2
        mov    byte ptr [ebx + 1], 30h
        db  068h, 0FCh, 0B2h, 000h, 000h
        lea    eax, [ebx + 2]
        push   eax
        call   sub_5_9B16
        lea    eax, [esp + 18h]
        add    esp, 8
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   ebx
        push   dword ptr [eax]
        call   sub_5_2583
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_ACFA
        push   1
        db  068h, 004h, 0B3h, 000h, 000h
        push   dword ptr [esp + 18h]
        call   sub_5_2DD8
        add    esp, 0ch
        mov    byte ptr [ebx], 0
        mov    byte ptr [ebx + 1], 33h
        db  068h, 008h, 0B3h, 000h, 000h
        lea    eax, [ebx + 2]
        push   eax
        call   sub_5_9B16
        lea    eax, [esp + 18h]
        add    esp, 8
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   ebx
        push   dword ptr [eax]
        call   sub_5_2583
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_ACFA
        push   15h
        db  068h, 014h, 0B3h, 000h, 000h
        push   0ffh
        push   1
        push   dword ptr [esp + 20h]
        call   sub_5_2CCA
        add    esp, 14h
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_ACFA
        mov    byte ptr [ebx], 0
        mov    byte ptr [ebx + 1], 40h
        db  068h, 02Ch, 0B3h, 000h, 000h
        lea    eax, [ebx + 2]
        push   eax
        call   sub_5_9B16
        lea    eax, [esp + 18h]
        add    esp, 8
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   ebx
        push   dword ptr [eax]
        call   sub_5_2583
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_ACFA
        db  0FFh, 035h, 064h, 0B9h, 000h, 000h
        db  0FFh, 035h, 02Ch, 0B9h, 000h, 000h
        push   dword ptr [esp + 18h]
        call   sub_5_2DD8
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_ACFA
        db  0A1h, 02Ch, 0B9h, 000h, 000h
        sub    ebp, ebp
        cmp    dword ptr [eax + 4], ebp
        jbe    NEAR PTR loc_5_ABFC
loc_5_AA8A:
        mov    byte ptr [ebx], 0
        mov    byte ptr [ebx + 1], 40h
        db  068h, 03Ch, 0B3h, 000h, 000h
        lea    eax, [ebx + 2]
        push   eax
        call   sub_5_9B16
        db  0A1h, 04Ch, 0B9h, 000h, 000h
        add    esp, 8
        mov    esi, dword ptr [eax + ebp*4]
        movzx  eax, byte ptr [ebx + 2]
        lea    edi, [ebx + eax + 3]
        add    esi, 14h
        movsd
        add    byte ptr [ebx + 2], 4
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   ebx
        push   dword ptr [eax]
        call   sub_5_2583
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_ACFA
        push   1
        db  068h, 044h, 0B3h, 000h, 000h
        push   dword ptr [esp + 18h]
        call   sub_5_2DD8
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_ACFA
        mov    byte ptr [ebx], 0
        mov    byte ptr [ebx + 1], 40h
        db  068h, 048h, 0B3h, 000h, 000h
        lea    eax, [ebx + 2]
        push   eax
        call   sub_5_9B16
        db  0A1h, 04Ch, 0B9h, 000h, 000h
        add    esp, 8
        mov    esi, dword ptr [eax + ebp*4]
        movzx  eax, byte ptr [ebx + 2]
        lea    edi, [ebx + eax + 3]
        add    esi, 14h
        movsd
        add    byte ptr [ebx + 2], 4
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   ebx
        push   dword ptr [eax]
        call   sub_5_2583
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_ACFA
        push   1
        db  068h, 050h, 0B3h, 000h, 000h
        push   dword ptr [esp + 18h]
        call   sub_5_2DD8
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_ACFA
        mov    byte ptr [ebx], 0
        mov    byte ptr [ebx + 1], 40h
        db  068h, 054h, 0B3h, 000h, 000h
        lea    eax, [ebx + 2]
        push   eax
        call   sub_5_9B16
        db  0A1h, 04Ch, 0B9h, 000h, 000h
        add    esp, 8
        mov    esi, dword ptr [eax + ebp*4]
        movzx  eax, byte ptr [ebx + 2]
        lea    edi, [ebx + eax + 3]
        add    esi, 14h
        movsd
        add    byte ptr [ebx + 2], 4
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   ebx
        push   dword ptr [eax]
        call   sub_5_2583
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_ACFA
        push   1
        db  068h, 05Ch, 0B3h, 000h, 000h
        push   dword ptr [esp + 18h]
        call   sub_5_2DD8
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_ACFA
        mov    eax, dword ptr [esp + 14h]
        push   ebp
        add    eax, 8
        push   eax
        call   sub_5_9FE2
        add    esp, 8
        db  0E8h, 0D7h, 091h, 0FAh, 0FFh
        db  0A1h, 02Ch, 0B9h, 000h, 000h
        inc    ebp
        cmp    dword ptr [eax + 4], ebp
        ja     NEAR PTR loc_5_AA8A
loc_5_ABFC:
        mov    byte ptr [ebx], 0
        mov    byte ptr [ebx + 1], 40h
        db  068h, 060h, 0B3h, 000h, 000h
        lea    eax, [ebx + 2]
        push   eax
        call   sub_5_9B16
        lea    eax, [esp + 18h]
        add    esp, 8
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   ebx
        push   dword ptr [eax]
        call   sub_5_2583
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_ACFA
        push   1
        db  068h, 070h, 0B3h, 000h, 000h
        push   0ffh
        push   1
        push   dword ptr [esp + 20h]
        call   sub_5_2CCA
        add    esp, 14h
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_ACFA
        mov    byte ptr [ebx], 0
        mov    byte ptr [ebx + 1], 40h
        db  068h, 074h, 0B3h, 000h, 000h
        lea    eax, [ebx + 2]
        push   eax
        call   sub_5_9B16
        lea    eax, [esp + 18h]
        add    esp, 8
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   ebx
        push   dword ptr [eax]
        call   sub_5_2583
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    SHORT loc_5_ACFA
        push   1
        db  068h, 084h, 0B3h, 000h, 000h
        push   0ffh
        push   1
        push   dword ptr [esp + 20h]
        call   sub_5_2CCA
        add    esp, 14h
        mov    si, ax
        or     si, si
        jne    SHORT loc_5_ACFA
        mov    byte ptr [ebx], 0
        mov    byte ptr [ebx + 1], 30h
        db  068h, 088h, 0B3h, 000h, 000h
        lea    eax, [ebx + 2]
        push   eax
        call   sub_5_9B16
        lea    eax, [esp + 18h]
        add    esp, 8
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   ebx
        push   dword ptr [eax]
        call   sub_5_2583
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    SHORT loc_5_ACFA
        push   1
        db  068h, 098h, 0B3h, 000h, 000h
        push   dword ptr [esp + 18h]
        call   sub_5_2DD8
        add    esp, 0ch
        mov    si, ax
loc_5_ACFA:
        push   ebx
        db  0E8h, 090h, 096h, 0FAh, 0FFh
        add    esp, 4
        mov    ax, si
loc_5_AD06:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_AD0E:
        mov    ax, 0d3h
        ret
loc_5_AD13:
        push   esi
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0AAh, 0E7h, 0FAh, 0FFh
        mov    eax, dword ptr [esp + 18h]
        add    esp, 4
        cmp    byte ptr [eax + 6ah], 3
        je     SHORT loc_5_AD37
        test   byte ptr [eax + 1ah], 10h
        jne    SHORT loc_5_AD37
        mov    ax, 0f1h
        jmp    SHORT loc_5_AD7A
loc_5_AD37:
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [eax + 0ah]
        call   sub_5_125F
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_AD50
        mov    si, 0fch
        jmp    SHORT loc_5_AD6A
loc_5_AD50:
        cmp    word ptr [eax + 16h], 300h
        je     SHORT loc_5_AD5E
        mov    si, 0d1h
        jmp    SHORT loc_5_AD6A
loc_5_AD5E:
        push   eax
        call   sub_5_31A8
        add    esp, 4
        mov    si, ax
loc_5_AD6A:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 098h, 0E7h, 0FAh, 0FFh
        add    esp, 4
        mov    ax, si
loc_5_AD7A:
        pop    esi
        ret
sub_5_AD7C:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    edi, dword ptr [esp + 20h]
        mov    dword ptr [esp + 10h], 400h
        mov    dword ptr [edi], 7ah
        sub    ebp, ebp
        mov    esi, dword ptr [esp + 18h]
loc_5_AD9D:
        cmp    ebp, 0ah
        jae    SHORT loc_5_ADEF
        or     ebx, ebx
        je     SHORT loc_5_ADAF
        push   ebx
        db  0E8h, 0E4h, 095h, 0FAh, 0FFh
        add    esp, 4
loc_5_ADAF:
        push   200h
        push   dword ptr [esp + 14h]
        db  0E8h, 0A7h, 095h, 0FAh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_5_ADE9
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 14h]
        push   ebx
        push   2
        inc    ebp
        push   dword ptr [esp + 2ch]
        push   esi
        db  0E8h, 017h, 090h, 0FAh, 0FFh
        add    esp, 18h
        mov    dword ptr [edi], eax
        cmp    eax, 7ah
        je     SHORT loc_5_AD9D
        jmp    SHORT loc_5_ADEF
loc_5_ADE9:
        mov    dword ptr [edi], 8
loc_5_ADEF:
        cmp    dword ptr [edi], 0
        je     SHORT loc_5_AE03
        or     ebx, ebx
        je     SHORT loc_5_AE03
        push   ebx
        db  0E8h, 092h, 095h, 0FAh, 0FFh
        add    esp, 4
        sub    ebx, ebx
loc_5_AE03:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_5_AE0D:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        mov    ebp, dword ptr [esp + 1ch]
        mov    dword ptr [esp + 10h], 400h
        mov    dword ptr [ebp], 7ah
        sub    edi, edi
        mov    ebx, dword ptr [esp + 18h]
loc_5_AE2F:
        cmp    edi, 0ah
        jae    SHORT loc_5_AE7F
        or     esi, esi
        je     SHORT loc_5_AE41
        push   esi
        db  0E8h, 052h, 095h, 0FAh, 0FFh
        add    esp, 4
loc_5_AE41:
        push   200h
        push   dword ptr [esp + 14h]
        db  0E8h, 015h, 095h, 0FAh, 0FFh
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_5_AE78
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 14h]
        push   esi
        push   2
        inc    edi
        push   ebx
        db  0E8h, 095h, 08Fh, 0FAh, 0FFh
        add    esp, 14h
        mov    dword ptr [ebp], eax
        cmp    eax, 7ah
        je     SHORT loc_5_AE2F
        jmp    SHORT loc_5_AE7F
loc_5_AE78:
        mov    dword ptr [ebp], 8
loc_5_AE7F:
        cmp    dword ptr [ebp], 0
        je     SHORT loc_5_AE94
        or     esi, esi
        je     SHORT loc_5_AE94
        push   esi
        db  0E8h, 001h, 095h, 0FAh, 0FFh
        add    esp, 4
        sub    esi, esi
loc_5_AE94:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_5_AE9E:
        push   esi
        push   dword ptr [esp + 0ch]
        sub    esi, esi
        push   dword ptr [esp + 0ch]
        call   sub_5_AE0D
        add    esp, 8
        cmp    eax, esi
        je     SHORT loc_5_AEC1
        mov    esi, dword ptr [eax + 4ch]
        push   eax
        db  0E8h, 0D2h, 094h, 0FAh, 0FFh
        add    esp, 4
loc_5_AEC1:
        mov    eax, esi
        pop    esi
        ret
sub_5_AEC5:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    edi, dword ptr [esp + 20h]
        mov    dword ptr [esp + 10h], 400h
        mov    dword ptr [edi], 7ah
        sub    ebp, ebp
        mov    esi, dword ptr [esp + 18h]
loc_5_AEE6:
        cmp    ebp, 0ah
        jae    SHORT loc_5_AF38
        or     ebx, ebx
        je     SHORT loc_5_AEF8
        push   ebx
        db  0E8h, 09Bh, 094h, 0FAh, 0FFh
        add    esp, 4
loc_5_AEF8:
        push   200h
        push   dword ptr [esp + 14h]
        db  0E8h, 05Eh, 094h, 0FAh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_5_AF32
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 14h]
        push   ebx
        push   3
        inc    ebp
        push   dword ptr [esp + 2ch]
        push   esi
        db  0E8h, 0E6h, 08Eh, 0FAh, 0FFh
        add    esp, 18h
        mov    dword ptr [edi], eax
        cmp    eax, 7ah
        je     SHORT loc_5_AEE6
        jmp    SHORT loc_5_AF38
loc_5_AF32:
        mov    dword ptr [edi], 8
loc_5_AF38:
        cmp    dword ptr [edi], 0
        je     SHORT loc_5_AF4C
        or     ebx, ebx
        je     SHORT loc_5_AF4C
        push   ebx
        db  0E8h, 049h, 094h, 0FAh, 0FFh
        add    esp, 4
        sub    ebx, ebx
loc_5_AF4C:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_5_AF56:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], 1ah
        push   dword ptr [esp + 3ch]
        sub    ebx, ebx
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 3ch]
        call   sub_5_AEC5
        mov    ebp, eax
        mov    eax, dword ptr [esp + 48h]
        add    esp, 0ch
        cmp    dword ptr [eax], ebx
        jne    NEAR PTR loc_5_B20C
        mov    dword ptr [esp + 18h], ebx
        mov    dword ptr [esp + 1ch], ebx
        mov    dword ptr [esp + 14h], ebx
        mov    dword ptr [esp + 20h], ebx
        mov    dword ptr [esp + 24h], ebx
        mov    dword ptr [esp + 28h], ebx
        mov    dword ptr [esp + 2ch], ebx
        mov    edi, dword ptr [ebp + 4]
        cmp    edi, ebx
        je     SHORT loc_5_AFC7
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        mov    dword ptr [esp + 2ch], eax
        mov    ecx, eax
        add    ecx, 1ah
        mov    dword ptr [esp + 10h], ecx
loc_5_AFC7:
        mov    edi, dword ptr [ebp + 0ch]
        or     edi, edi
        je     SHORT loc_5_AFEA
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        mov    dword ptr [esp + 28h], eax
        mov    ecx, dword ptr [esp + 10h]
        add    ecx, eax
        mov    dword ptr [esp + 10h], ecx
loc_5_AFEA:
        mov    edi, dword ptr [ebp + 10h]
        or     edi, edi
        je     SHORT loc_5_B00D
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        mov    dword ptr [esp + 24h], eax
        mov    ecx, dword ptr [esp + 10h]
        add    ecx, eax
        mov    dword ptr [esp + 10h], ecx
loc_5_B00D:
        mov    edi, dword ptr [ebp + 14h]
        or     edi, edi
        je     SHORT loc_5_B030
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        mov    dword ptr [esp + 20h], eax
        mov    ecx, dword ptr [esp + 10h]
        add    ecx, eax
        mov    dword ptr [esp + 10h], ecx
loc_5_B030:
        mov    eax, dword ptr [ebp + 1ch]
        or     eax, eax
        je     SHORT loc_5_B05C
        mov    dword ptr [esp + 14h], 0
        cmp    word ptr [eax], 0
        je     SHORT loc_5_B050
loc_5_B045:
        inc    eax
        inc    dword ptr [esp + 14h]
        cmp    word ptr [eax], 0
        jne    SHORT loc_5_B045
loc_5_B050:
        mov    ecx, dword ptr [esp + 10h]
        add    ecx, dword ptr [esp + 14h]
        mov    dword ptr [esp + 10h], ecx
loc_5_B05C:
        mov    edi, dword ptr [ebp + 20h]
        or     edi, edi
        je     SHORT loc_5_B07F
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        mov    dword ptr [esp + 1ch], eax
        mov    ecx, dword ptr [esp + 10h]
        add    ecx, eax
        mov    dword ptr [esp + 10h], ecx
loc_5_B07F:
        mov    edi, dword ptr [ebp + 18h]
        or     edi, edi
        je     SHORT loc_5_B0A2
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        mov    dword ptr [esp + 18h], eax
        mov    ecx, dword ptr [esp + 10h]
        add    ecx, eax
        mov    dword ptr [esp + 10h], ecx
loc_5_B0A2:
        add    dword ptr [esp + 10h], 8
        push   200h
        push   dword ptr [esp + 14h]
        db  0E8h, 0AFh, 092h, 0FAh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_5_B0CD
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [eax], 7ah
        jmp    NEAR PTR loc_5_B20C
loc_5_B0CD:
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ebx], ecx
        mov    dword ptr [ebx + 4], 1ah
        mov    eax, dword ptr [ebp]
        lea    edx, [ebx + 1ah]
        mov    dword ptr [ebx + 8], eax
        mov    eax, edx
        sub    eax, ebx
        mov    word ptr [ebx + 0ch], ax
        mov    eax, dword ptr [esp + 2ch]
        mov    edi, edx
        mov    esi, dword ptr [ebp + 4]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    edx, dword ptr [esp + 2ch]
        mov    byte ptr [edx], 0
        inc    edx
        mov    eax, edx
        sub    eax, ebx
        mov    word ptr [ebx + 0eh], ax
        mov    eax, dword ptr [esp + 28h]
        mov    edi, edx
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    edx, dword ptr [esp + 28h]
        mov    byte ptr [edx], 0
        inc    edx
        mov    eax, edx
        sub    eax, ebx
        mov    word ptr [ebx + 10h], ax
        mov    eax, dword ptr [esp + 24h]
        mov    edi, edx
        mov    esi, dword ptr [ebp + 10h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    edx, dword ptr [esp + 24h]
        mov    byte ptr [edx], 0
        inc    edx
        mov    eax, edx
        sub    eax, ebx
        mov    word ptr [ebx + 12h], ax
        mov    eax, dword ptr [esp + 20h]
        mov    edi, edx
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    edx, dword ptr [esp + 20h]
        mov    byte ptr [edx], 0
        inc    edx
        mov    eax, edx
        sub    eax, ebx
        mov    word ptr [ebx + 14h], ax
        mov    eax, dword ptr [esp + 14h]
        mov    edi, edx
        mov    esi, dword ptr [ebp + 1ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    edx, dword ptr [esp + 14h]
        mov    byte ptr [edx], 0
        inc    edx
        mov    byte ptr [edx], 0
        inc    edx
        mov    eax, edx
        sub    eax, ebx
        mov    word ptr [ebx + 16h], ax
        mov    eax, dword ptr [esp + 1ch]
        mov    edi, edx
        mov    esi, dword ptr [ebp + 20h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    edx, dword ptr [esp + 1ch]
        mov    byte ptr [edx], 0
        inc    edx
        mov    eax, edx
        sub    eax, ebx
        mov    word ptr [ebx + 18h], ax
        mov    eax, dword ptr [esp + 18h]
        mov    edi, edx
        mov    esi, dword ptr [ebp + 18h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 18h]
        mov    byte ptr [edx + eax], 0
        mov    eax, dword ptr [esp + 40h]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [eax], 0
loc_5_B20C:
        or     ebp, ebp
        je     SHORT loc_5_B219
        push   ebp
        db  0E8h, 07Ah, 091h, 0FAh, 0FFh
        add    esp, 4
loc_5_B219:
        mov    eax, dword ptr [esp + 3ch]
        cmp    dword ptr [eax], 0
        je     SHORT loc_5_B231
        or     ebx, ebx
        je     SHORT loc_5_B231
        push   ebx
        db  0E8h, 064h, 091h, 0FAh, 0FFh
        add    esp, 4
        sub    ebx, ebx
loc_5_B231:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_5_B23B:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 1ch]
        sub    esi, esi
        mov    ebx, dword ptr [eax + 0ah]
        mov    dword ptr [esp + 10h], 0
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 06Bh, 0E2h, 0FAh, 0FFh
        add    esp, 4
        push   ebx
        call   sub_5_125F
        add    esp, 4
        cmp    eax, esi
        jne    SHORT loc_5_B276
        mov    di, 0fch
        jmp    NEAR PTR loc_5_B351
loc_5_B276:
        mov    ebp, dword ptr [eax + 0ch]
        or     ebp, ebp
        je     NEAR PTR loc_5_B34D
        cmp    byte ptr [ebp + 4], 1
        jne    NEAR PTR loc_5_B34D
        push   80h
        push   eax
        call   sub_5_A0B3
        add    esp, 8
        mov    di, ax
        or     di, di
        jne    NEAR PTR loc_5_B351
        lea    eax, [esp + 10h]
        push   0
        push   eax
        lea    eax, [ebp + 16h]
        push   eax
        call   sub_5_9A9B
        mov    dword ptr [esp + 20h], eax
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_B2CD
        mov    dword ptr [esp + 10h], 0
        jmp    NEAR PTR loc_5_B34D
loc_5_B2CD:
        mov    edi, dword ptr [esp + 20h]
        mov    dword ptr [edi + 8], ebx
        mov    al, byte ptr [ebp + 15h]
        and    al, 1
        or     al, 2
        mov    byte ptr [edi + 0ch], al
        lea    eax, [esp + 14h]
        push   eax
        mov    byte ptr [edi + 0eh], 0
        push   dword ptr [esp + 14h]
        call   sub_5_AE0D
        add    esp, 8
        mov    esi, eax
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_B30D
        push   dword ptr [esp + 14h]
        call   sub_5_9B44
        add    esp, 4
        mov    di, ax
        jmp    SHORT loc_5_B351
loc_5_B30D:
        mov    eax, dword ptr [esi + 4ch]
        cmp    eax, 0fah
        mov    byte ptr [edi + 0dh], al
        jbe    SHORT loc_5_B31E
        mov    byte ptr [edi + 0dh], 0fah
loc_5_B31E:
        test   byte ptr [esi + 48h], 1
        je     SHORT loc_5_B328
        or     byte ptr [edi + 0ch], 4
loc_5_B328:
        movzx  eax, byte ptr [edi + 0eh]
        imul   eax, eax, 5
        mov    byte ptr [edi + eax + 0fh], 19h
        movzx  cx, byte ptr [edi + 0eh]
        imul   cx, cx, 5
        mov    eax, dword ptr [esp + 24h]
        add    cx, 8
        mov    word ptr [eax], cx
        sub    di, di
        jmp    SHORT loc_5_B351
loc_5_B34D:
        mov    di, 0d1h
loc_5_B351:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0B1h, 0E1h, 0FAh, 0FFh
        add    esp, 4
        or     esi, esi
        je     SHORT loc_5_B36B
        push   esi
        db  0E8h, 028h, 090h, 0FAh, 0FFh
        add    esp, 4
loc_5_B36B:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_5_B37E
        push   dword ptr [esp + 10h]
        db  0E8h, 061h, 08Ah, 0FAh, 0FFh
        add    esp, 4
loc_5_B37E:
        mov    ax, di
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_B389:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 1ch]
        sub    edi, edi
        mov    ebx, dword ptr [eax + 0ah]
        mov    dword ptr [esp + 10h], 0
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 01Dh, 0E1h, 0FAh, 0FFh
        add    esp, 4
        push   ebx
        call   sub_5_125F
        add    esp, 4
        cmp    eax, edi
        jne    SHORT loc_5_B3C4
        mov    si, 0fch
        jmp    NEAR PTR loc_5_B49B
loc_5_B3C4:
        mov    ebp, dword ptr [eax + 0ch]
        or     ebp, ebp
        je     NEAR PTR loc_5_B497
        cmp    byte ptr [ebp + 4], 1
        jne    NEAR PTR loc_5_B497
        push   80h
        push   eax
        call   sub_5_A0B3
        add    esp, 8
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_B49B
        lea    eax, [esp + 10h]
        push   0
        push   eax
        lea    eax, [ebp + 16h]
        push   eax
        call   sub_5_9A9B
        mov    dword ptr [esp + 20h], eax
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_B418
        mov    dword ptr [esp + 10h], 0
        jmp    SHORT loc_5_B497
loc_5_B418:
        mov    esi, dword ptr [esp + 20h]
        mov    dword ptr [esi + 8], ebx
        mov    al, byte ptr [ebp + 15h]
        and    al, 1
        or     al, 2
        mov    dword ptr [esi + 14h], 0
        movzx  eax, al
        mov    dword ptr [esi + 0ch], eax
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 14h]
        call   sub_5_AE0D
        add    esp, 8
        mov    edi, eax
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_B45E
        push   dword ptr [esp + 14h]
        call   sub_5_9B44
        add    esp, 4
        mov    si, ax
        jmp    SHORT loc_5_B49B
loc_5_B45E:
        mov    eax, dword ptr [edi + 4ch]
        cmp    eax, 0fah
        ja     SHORT loc_5_B470
        movzx  eax, al
        mov    dword ptr [esi + 10h], eax
        jmp    SHORT loc_5_B477
loc_5_B470:
        mov    dword ptr [esi + 10h], 0fah
loc_5_B477:
        test   byte ptr [edi + 48h], 1
        je     SHORT loc_5_B481
        or     dword ptr [esi + 0ch], 4
loc_5_B481:
        mov    ecx, dword ptr [esi + 14h]
        mov    eax, dword ptr [esp + 24h]
        lea    ecx, [ecx*8 + 10h]
        mov    word ptr [eax], cx
        sub    si, si
        jmp    SHORT loc_5_B49B
loc_5_B497:
        mov    si, 0d1h
loc_5_B49B:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 067h, 0E0h, 0FAh, 0FFh
        add    esp, 4
        or     edi, edi
        je     SHORT loc_5_B4B5
        push   edi
        db  0E8h, 0DEh, 08Eh, 0FAh, 0FFh
        add    esp, 4
loc_5_B4B5:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_5_B4C8
        push   dword ptr [esp + 10h]
        db  0E8h, 017h, 089h, 0FAh, 0FFh
        add    esp, 4
loc_5_B4C8:
        mov    ax, si
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_B4D3:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 0ch], 0
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0DDh, 0DFh, 0FAh, 0FFh
        mov    edi, dword ptr [esp + 20h]
        add    esp, 4
        push   dword ptr [edi + 0ah]
        call   sub_5_125F
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_B50A
        mov    bx, 0fch
        jmp    NEAR PTR loc_5_B5C0
loc_5_B50A:
        mov    esi, dword ptr [eax + 0ch]
        or     esi, esi
        je     NEAR PTR loc_5_B5BC
        cmp    byte ptr [esi + 4], 1
        jne    NEAR PTR loc_5_B5BC
        push   20h
        push   eax
        call   sub_5_A0B3
        add    esp, 8
        mov    bx, ax
        or     bx, bx
        jne    NEAR PTR loc_5_B5C0
        mov    al, byte ptr [edi + 0eh]
        mov    byte ptr [esi + 15h], al
        lea    eax, [esp + 0ch]
        push   0
        push   eax
        lea    eax, [esi + 16h]
        push   eax
        call   sub_5_9A9B
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_B565
        mov    dword ptr [esp + 0ch], 0
        mov    bx, 0d1h
        jmp    SHORT loc_5_B5C0
loc_5_B565:
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 10h]
        call   sub_5_AE0D
        add    esp, 8
        mov    edi, eax
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_5_B5AB
        test   byte ptr [edi + 48h], 1
        je     SHORT loc_5_B58F
        test   byte ptr [esi + 15h], 4
        jne    SHORT loc_5_B5AB
        push   2
        jmp    SHORT loc_5_B597
loc_5_B58F:
        test   byte ptr [esi + 15h], 4
        je     SHORT loc_5_B5AB
        push   1
loc_5_B597:
        push   0
        push   0
        push   dword ptr [esp + 18h]
        db  0E8h, 062h, 088h, 0FAh, 0FFh
        mov    dword ptr [esp + 20h], eax
        add    esp, 10h
loc_5_B5AB:
        push   dword ptr [esp + 10h]
        call   sub_5_9B44
        add    esp, 4
        mov    bx, ax
        jmp    SHORT loc_5_B5C4
loc_5_B5BC:
        mov    bx, 0d1h
loc_5_B5C0:
        mov    edi, dword ptr [esp + 14h]
loc_5_B5C4:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 03Eh, 0DFh, 0FAh, 0FFh
        add    esp, 4
        or     edi, edi
        je     SHORT loc_5_B5DE
        push   edi
        db  0E8h, 0B5h, 08Dh, 0FAh, 0FFh
        add    esp, 4
loc_5_B5DE:
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_5_B5F1
        push   dword ptr [esp + 0ch]
        db  0E8h, 0EEh, 087h, 0FAh, 0FFh
        add    esp, 4
loc_5_B5F1:
        mov    ax, bx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_B5FB:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 0ch], 0
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0B5h, 0DEh, 0FAh, 0FFh
        mov    edi, dword ptr [esp + 20h]
        add    esp, 4
        push   dword ptr [edi + 0ah]
        call   sub_5_125F
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_B632
        mov    bx, 0fch
        jmp    NEAR PTR loc_5_B6E8
loc_5_B632:
        mov    esi, dword ptr [eax + 0ch]
        or     esi, esi
        je     NEAR PTR loc_5_B6E4
        cmp    byte ptr [esi + 4], 1
        jne    NEAR PTR loc_5_B6E4
        push   20h
        push   eax
        call   sub_5_A0B3
        add    esp, 8
        mov    bx, ax
        or     bx, bx
        jne    NEAR PTR loc_5_B6E8
        mov    al, byte ptr [edi + 0eh]
        mov    byte ptr [esi + 15h], al
        lea    eax, [esp + 0ch]
        push   0
        push   eax
        lea    eax, [esi + 16h]
        push   eax
        call   sub_5_9A9B
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_B68D
        mov    dword ptr [esp + 0ch], 0
        mov    bx, 0d1h
        jmp    SHORT loc_5_B6E8
loc_5_B68D:
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 10h]
        call   sub_5_AE0D
        add    esp, 8
        mov    edi, eax
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_5_B6D3
        test   byte ptr [edi + 48h], 1
        je     SHORT loc_5_B6B7
        test   byte ptr [esi + 15h], 4
        jne    SHORT loc_5_B6D3
        push   2
        jmp    SHORT loc_5_B6BF
loc_5_B6B7:
        test   byte ptr [esi + 15h], 4
        je     SHORT loc_5_B6D3
        push   1
loc_5_B6BF:
        push   0
        push   0
        push   dword ptr [esp + 18h]
        db  0E8h, 03Ah, 087h, 0FAh, 0FFh
        mov    dword ptr [esp + 20h], eax
        add    esp, 10h
loc_5_B6D3:
        push   dword ptr [esp + 10h]
        call   sub_5_9B44
        add    esp, 4
        mov    bx, ax
        jmp    SHORT loc_5_B6EC
loc_5_B6E4:
        mov    bx, 0d1h
loc_5_B6E8:
        mov    edi, dword ptr [esp + 14h]
loc_5_B6EC:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 016h, 0DEh, 0FAh, 0FFh
        add    esp, 4
        or     edi, edi
        je     SHORT loc_5_B706
        push   edi
        db  0E8h, 08Dh, 08Ch, 0FAh, 0FFh
        add    esp, 4
loc_5_B706:
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_5_B719
        push   dword ptr [esp + 0ch]
        db  0E8h, 0C6h, 086h, 0FAh, 0FFh
        add    esp, 4
loc_5_B719:
        mov    ax, bx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_5_B723:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0
        push   dword ptr [esp + 3ch]
        call   sub_5_C04E
        mov    dword ptr [esp + 20h], eax
        lea    eax, [esp + 1ch]
        add    esp, 8
        push   eax
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 3ch]
        call   sub_5_AD7C
        add    esp, 0ch
        mov    ebx, eax
        cmp    dword ptr [esp + 14h], 0
        jne    NEAR PTR loc_5_B976
        push   dword ptr [esp + 44h]
        push   ebx
        call   sub_5_A1A7
        add    esp, 8
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_B985
        mov    esi, dword ptr [esp + 40h]
        cmp    esi, 1
        sbb    edx, edx
        and    edx, 0ffffff36h
        add    edx, 118h
        mov    ebp, dword ptr [esp + 3ch]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    word ptr [esp + 1ch], 100h
        mov    edi, dword ptr [ebx + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [esp + 10h], ecx
        cmp    ecx, 0fh
        jbe    SHORT loc_5_B7C9
        mov    dword ptr [esp + 10h], 0fh
loc_5_B7C9:
        mov    al, byte ptr [esp + 10h]
        mov    byte ptr [esp + 1eh], al
        push   dword ptr [esp + 10h]
        lea    eax, [esp + 23h]
        push   dword ptr [ebx + 0ch]
        push   eax
        db  0E8h, 0A6h, 08Ah, 0FAh, 0FFh
        add    esp, 0ch
        push   dword ptr [esp + 10h]
        push   0
        lea    eax, [esp + 27h]
        push   1
        push   eax
        lea    eax, [esp + 2fh]
        push   eax
        db  0E8h, 032h, 08Ch, 0FAh, 0FFh
        lea    eax, [esp + 24h]
        add    esp, 14h
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        call   sub_5_1C3C
        add    esp, 8
        or     ax, ax
        jne    SHORT loc_5_B81D
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebp + 12h], eax
loc_5_B81D:
        mov    dword ptr [ebp + 16h], 0ffffffffh
        lea    edi, [ebp + 1ah]
        mov    eax, 0ffffffffh
        stosd
        stosw
        lea    edi, [ebx + 50h]
        db  0E8h, 0E5h, 08Bh, 0FAh, 0FFh
        push   eax
        push   edi
        call   sub_5_9950
        add    esp, 8
        mov    ax, word ptr [edi]
        sub    al, 6ch
        mov    byte ptr [ebp + 20h], al
        mov    al, byte ptr [ebx + 52h]
        mov    byte ptr [ebp + 21h], al
        mov    al, byte ptr [ebx + 56h]
        mov    byte ptr [ebp + 22h], al
        mov    al, byte ptr [ebx + 58h]
        mov    byte ptr [ebp + 23h], al
        mov    al, byte ptr [ebx + 5ah]
        mov    byte ptr [ebp + 24h], al
        mov    al, byte ptr [ebx + 5ch]
        mov    byte ptr [ebp + 25h], al
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [ebp + 26h], eax
        mov    eax, dword ptr [ebx + 3ch]
        mov    word ptr [ebp + 2ch], ax
        test   byte ptr [ebx + 34h], 1
        je     SHORT loc_5_B881
        or     word ptr [ebp + 2eh], 0c0h
loc_5_B881:
        test   byte ptr [ebx + 34h], 8
        je     SHORT loc_5_B88C
        or     word ptr [ebp + 2eh], 20h
loc_5_B88C:
        or     esi, esi
        je     NEAR PTR loc_5_B96E
        mov    edi, dword ptr [ebx + 10h]
        or     edi, edi
        je     SHORT loc_5_B8E4
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [esp + 10h], ecx
        cmp    ecx, 32h
        jbe    SHORT loc_5_B8B8
        mov    dword ptr [esp + 10h], 32h
loc_5_B8B8:
        lea    eax, [ebp + 4eh]
        mov    edx, dword ptr [esp + 10h]
        mov    edi, eax
        mov    esi, dword ptr [ebx + 10h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        push   dword ptr [esp + 10h]
        push   0
        push   1
        push   eax
        push   eax
        db  0E8h, 04Dh, 08Bh, 0FAh, 0FFh
        add    esp, 14h
loc_5_B8E4:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_5_B901
        mov    esi, dword ptr [esp + 18h]
        add    esi, 0fh
        lea    edi, [ebp + 80h]
        mov    ecx, 26h
        rep movsd
        jmp    SHORT loc_5_B96E
loc_5_B901:
        push   1
        db  0E8h, 058h, 081h, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [ebp + 82h], ax
        mov    word ptr [ebp + 84h], 800h
        mov    edi, dword ptr [ebx + 0ch]
        or     edi, edi
        je     SHORT loc_5_B96E
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [esp + 10h], ecx
        cmp    ecx, 0ch
        jbe    SHORT loc_5_B93F
        mov    dword ptr [esp + 10h], 0ch
loc_5_B93F:
        add    ebp, 0a0h
        mov    eax, dword ptr [esp + 10h]
        mov    edi, ebp
        mov    esi, dword ptr [ebx + 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   dword ptr [esp + 10h]
        push   0
        push   1
        push   ebp
        push   ebp
        db  0E8h, 0C3h, 08Ah, 0FAh, 0FFh
        add    esp, 14h
loc_5_B96E:
        mov    dword ptr [esp + 14h], 0
loc_5_B976:
        push   dword ptr [esp + 14h]
        call   sub_5_9B44
        add    esp, 4
        mov    si, ax
loc_5_B985:
        or     ebx, ebx
        je     SHORT loc_5_B992
        push   ebx
        db  0E8h, 001h, 08Ah, 0FAh, 0FFh
        add    esp, 4
loc_5_B992:
        mov    ax, si
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
sub_5_B99D:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 0
        mov    dword ptr [esp + 10h], 0
        push   0
        push   dword ptr [esp + 2ch]
        call   sub_5_C04E
        mov    dword ptr [esp + 24h], eax
        lea    eax, [esp + 20h]
        add    esp, 8
        push   eax
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        call   sub_5_AD7C
        add    esp, 0ch
        mov    ebx, eax
        cmp    dword ptr [esp + 18h], 0
        jne    NEAR PTR loc_5_BB70
        push   dword ptr [esp + 3ch]
        push   ebx
        call   sub_5_A1A7
        add    esp, 8
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_BB7F
        db  0C7h, 043h, 008h, 09Ch, 0B3h, 000h, 000h
        cmp    dword ptr [esp + 34h], 0
        je     SHORT loc_5_BA73
        mov    ebp, dword ptr [esp + 38h]
        push   200h
        movzx  eax, byte ptr [ebp + 72h]
        inc    eax
        push   eax
        db  0E8h, 041h, 089h, 0FAh, 0FFh
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_BA73
        movzx  eax, byte ptr [ebp + 72h]
        lea    esi, [ebp + 73h]
        mov    edi, dword ptr [esp + 14h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  ecx, byte ptr [ebp + 72h]
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [ecx + eax], 0
        movzx  eax, byte ptr [ebp + 72h]
        push   eax
        push   1
        push   0
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 24h]
        db  0E8h, 0C5h, 089h, 0FAh, 0FFh
        mov    eax, dword ptr [esp + 28h]
        add    esp, 14h
        mov    dword ptr [ebx + 0ch], eax
loc_5_BA73:
        cmp    dword ptr [esp + 30h], 0
        je     SHORT loc_5_BAE5
        mov    eax, dword ptr [esp + 2ch]
        mov    esi, eax
        mov    byte ptr [eax + 7fh], 0
        add    esi, 4eh
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    ebp, [ecx - 1]
        push   200h
        lea    eax, [ebp + 1]
        push   eax
        db  0E8h, 0BFh, 088h, 0FAh, 0FFh
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_BAE5
        mov    edi, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 10h]
        push   ebp
        push   1
        push   0
        mov    byte ptr [ebp + eax], 0
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 20h]
        db  0E8h, 053h, 089h, 0FAh, 0FFh
        mov    eax, dword ptr [esp + 24h]
        add    esp, 14h
        mov    dword ptr [ebx + 10h], eax
loc_5_BAE5:
        push   0
        push   ebx
        push   2
        mov    dword ptr [ebx + 3ch], 0
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 34h]
        db  0E8h, 0FCh, 082h, 0FAh, 0FFh
        mov    dword ptr [esp + 2ch], eax
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_5_BB70
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_5_BB2F
        cmp    dword ptr [esp + 30h], 0
        je     SHORT loc_5_BB2F
        mov    esi, dword ptr [esp + 2ch]
        add    esi, 80h
        mov    edi, dword ptr [esp + 1ch]
        add    edi, 0fh
        mov    ecx, 26h
        rep movsd
loc_5_BB2F:
        sub    ecx, ecx
        test   byte ptr [ebx + 34h], 1
        mov    eax, dword ptr [esp + 2ch]
        jne    SHORT loc_5_BB48
        test   byte ptr [eax + 2eh], 0c0h
        je     SHORT loc_5_BB57
        mov    ecx, 1
        jmp    SHORT loc_5_BB57
loc_5_BB48:
        mov    ax, word ptr [eax + 2eh]
        and    al, 0c0h
        cmp    al, 0c0h
        je     SHORT loc_5_BB57
        mov    ecx, 2
loc_5_BB57:
        or     ecx, ecx
        je     SHORT loc_5_BB70
        push   ecx
        push   0
        push   0
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 34h]
        db  0E8h, 08Dh, 082h, 0FAh, 0FFh
        add    esp, 14h
loc_5_BB70:
        push   dword ptr [esp + 18h]
        call   sub_5_9B44
        add    esp, 4
        mov    si, ax
loc_5_BB7F:
        or     ebx, ebx
        je     SHORT loc_5_BB8C
        push   ebx
        db  0E8h, 007h, 088h, 0FAh, 0FFh
        add    esp, 4
loc_5_BB8C:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_BB9F
        push   dword ptr [esp + 14h]
        db  0E8h, 0F4h, 087h, 0FAh, 0FFh
        add    esp, 4
loc_5_BB9F:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_5_BBB2
        push   dword ptr [esp + 10h]
        db  0E8h, 0E1h, 087h, 0FAh, 0FFh
        add    esp, 4
loc_5_BBB2:
        mov    ax, si
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_5_BBBD:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0
        push   dword ptr [esp + 3ch]
        call   sub_5_C04E
        mov    dword ptr [esp + 20h], eax
        lea    eax, [esp + 1ch]
        add    esp, 8
        push   eax
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 3ch]
        call   sub_5_AD7C
        add    esp, 0ch
        mov    ebx, eax
        cmp    dword ptr [esp + 14h], 0
        jne    NEAR PTR loc_5_BE0B
        push   dword ptr [esp + 44h]
        push   ebx
        call   sub_5_A1A7
        add    esp, 8
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_BE1A
        cmp    dword ptr [esp + 40h], 1
        sbb    edx, edx
        and    edx, 0ffffff36h
        add    edx, 100h
        mov    ebp, dword ptr [esp + 3ch]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    word ptr [esp + 1ch], 100h
        mov    edi, dword ptr [ebx + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [esp + 10h], ecx
        cmp    ecx, 0fh
        jbe    SHORT loc_5_BC61
        mov    dword ptr [esp + 10h], 0fh
loc_5_BC61:
        mov    al, byte ptr [esp + 10h]
        mov    byte ptr [esp + 1eh], al
        mov    eax, dword ptr [esp + 10h]
        lea    edi, [esp + 1fh]
        mov    esi, dword ptr [ebx + 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   dword ptr [esp + 10h]
        push   0
        lea    eax, [esp + 27h]
        push   1
        push   eax
        lea    eax, [esp + 2fh]
        push   eax
        db  0E8h, 095h, 087h, 0FAh, 0FFh
        lea    eax, [esp + 24h]
        add    esp, 14h
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        call   sub_5_1C3C
        add    esp, 8
        or     ax, ax
        jne    SHORT loc_5_BCBA
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebp + 2], eax
loc_5_BCBA:
        mov    dword ptr [ebp + 6], 0ffffffffh
        lea    edi, [ebp + 0ah]
        mov    eax, 0ffffffffh
        stosd
        stosw
        lea    esi, [ebx + 50h]
        db  0E8h, 048h, 087h, 0FAh, 0FFh
        push   eax
        push   esi
        call   sub_5_9950
        add    esp, 8
        mov    ax, word ptr [esi]
        sub    al, 6ch
        mov    byte ptr [ebp + 10h], al
        mov    al, byte ptr [ebx + 52h]
        mov    byte ptr [ebp + 11h], al
        mov    al, byte ptr [ebx + 56h]
        mov    byte ptr [ebp + 12h], al
        mov    al, byte ptr [ebx + 58h]
        mov    byte ptr [ebp + 13h], al
        mov    al, byte ptr [ebx + 5ah]
        mov    byte ptr [ebp + 14h], al
        mov    al, byte ptr [ebx + 5ch]
        mov    byte ptr [ebp + 15h], al
        mov    ax, word ptr [esp + 38h]
        mov    word ptr [ebp + 16h], ax
        mov    al, byte ptr [ebx + 3ch]
        mov    byte ptr [ebp + 1ah], al
        test   byte ptr [ebx + 34h], 1
        je     SHORT loc_5_BD1D
        or     byte ptr [ebp + 1bh], 0c0h
loc_5_BD1D:
        test   byte ptr [ebx + 34h], 8
        je     SHORT loc_5_BD27
        or     byte ptr [ebp + 1bh], 20h
loc_5_BD27:
        cmp    dword ptr [esp + 40h], 0
        je     NEAR PTR loc_5_BE03
        mov    edi, dword ptr [ebx + 10h]
        or     edi, edi
        je     SHORT loc_5_BD82
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [esp + 10h], ecx
        cmp    ecx, 32h
        jbe    SHORT loc_5_BD56
        mov    dword ptr [esp + 10h], 32h
loc_5_BD56:
        lea    eax, [ebp + 36h]
        mov    edx, dword ptr [esp + 10h]
        mov    edi, eax
        mov    esi, dword ptr [ebx + 10h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        push   dword ptr [esp + 10h]
        push   0
        push   1
        push   eax
        push   eax
        db  0E8h, 0AFh, 086h, 0FAh, 0FFh
        add    esp, 14h
loc_5_BD82:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_5_BD9C
        mov    esi, dword ptr [esp + 18h]
        add    esi, 0fh
        lea    edi, [ebp + 68h]
        mov    ecx, 26h
        rep movsd
        jmp    SHORT loc_5_BE03
loc_5_BD9C:
        push   1
        db  0E8h, 0BDh, 07Ch, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [ebp + 6ah], ax
        mov    word ptr [ebp + 6ch], 800h
        mov    edi, dword ptr [ebx + 0ch]
        or     edi, edi
        je     SHORT loc_5_BE03
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [esp + 10h], ecx
        cmp    ecx, 0ch
        jbe    SHORT loc_5_BDD4
        mov    dword ptr [esp + 10h], 0ch
loc_5_BDD4:
        add    ebp, 88h
        mov    eax, dword ptr [esp + 10h]
        mov    edi, ebp
        mov    esi, dword ptr [ebx + 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   dword ptr [esp + 10h]
        push   0
        push   1
        push   ebp
        push   ebp
        db  0E8h, 02Eh, 086h, 0FAh, 0FFh
        add    esp, 14h
loc_5_BE03:
        mov    dword ptr [esp + 14h], 0
loc_5_BE0B:
        push   dword ptr [esp + 14h]
        call   sub_5_9B44
        add    esp, 4
        mov    si, ax
loc_5_BE1A:
        or     ebx, ebx
        je     SHORT loc_5_BE27
        push   ebx
        db  0E8h, 06Ch, 085h, 0FAh, 0FFh
        add    esp, 4
loc_5_BE27:
        mov    ax, si
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
sub_5_BE32:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 0
        mov    dword ptr [esp + 10h], 0
        push   0
        push   dword ptr [esp + 2ch]
        call   sub_5_C04E
        mov    dword ptr [esp + 24h], eax
        lea    eax, [esp + 20h]
        add    esp, 8
        push   eax
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        call   sub_5_AD7C
        add    esp, 0ch
        mov    ebx, eax
        cmp    dword ptr [esp + 18h], 0
        jne    NEAR PTR loc_5_C001
        push   dword ptr [esp + 3ch]
        push   ebx
        call   sub_5_A1A7
        add    esp, 8
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_C010
        db  0C7h, 043h, 008h, 0A0h, 0B3h, 000h, 000h
        cmp    dword ptr [esp + 34h], 0
        je     SHORT loc_5_BF08
        mov    ebp, dword ptr [esp + 38h]
        push   200h
        movzx  eax, byte ptr [ebp + 72h]
        inc    eax
        push   eax
        db  0E8h, 0ACh, 084h, 0FAh, 0FFh
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_BF08
        movzx  eax, byte ptr [ebp + 72h]
        lea    esi, [ebp + 73h]
        mov    edi, dword ptr [esp + 14h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  ecx, byte ptr [ebp + 72h]
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [ecx + eax], 0
        movzx  eax, byte ptr [ebp + 72h]
        push   eax
        push   1
        push   0
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 24h]
        db  0E8h, 030h, 085h, 0FAh, 0FFh
        mov    eax, dword ptr [esp + 28h]
        add    esp, 14h
        mov    dword ptr [ebx + 0ch], eax
loc_5_BF08:
        cmp    dword ptr [esp + 30h], 0
        je     SHORT loc_5_BF7A
        mov    eax, dword ptr [esp + 2ch]
        mov    esi, eax
        mov    byte ptr [eax + 67h], 0
        add    esi, 36h
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    ebp, [ecx - 1]
        push   200h
        lea    eax, [ebp + 1]
        push   eax
        db  0E8h, 02Ah, 084h, 0FAh, 0FFh
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_BF7A
        mov    edi, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 10h]
        push   ebp
        push   1
        push   0
        mov    byte ptr [ebp + eax], 0
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 20h]
        db  0E8h, 0BEh, 084h, 0FAh, 0FFh
        mov    eax, dword ptr [esp + 24h]
        add    esp, 14h
        mov    dword ptr [ebx + 10h], eax
loc_5_BF7A:
        push   0
        push   ebx
        push   2
        mov    dword ptr [ebx + 3ch], 0
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 34h]
        db  0E8h, 067h, 07Eh, 0FAh, 0FFh
        mov    dword ptr [esp + 2ch], eax
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_5_C001
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_5_BFC1
        cmp    dword ptr [esp + 30h], 0
        je     SHORT loc_5_BFC1
        mov    esi, dword ptr [esp + 2ch]
        add    esi, 68h
        mov    edi, dword ptr [esp + 1ch]
        add    edi, 0fh
        mov    ecx, 26h
        rep movsd
loc_5_BFC1:
        sub    ecx, ecx
        test   byte ptr [ebx + 34h], 1
        mov    eax, dword ptr [esp + 2ch]
        jne    SHORT loc_5_BFDA
        test   byte ptr [eax + 1bh], 0c0h
        je     SHORT loc_5_BFE8
        mov    ecx, 1
        jmp    SHORT loc_5_BFE8
loc_5_BFDA:
        mov    al, byte ptr [eax + 1bh]
        and    al, 0c0h
        cmp    al, 0c0h
        je     SHORT loc_5_BFE8
        mov    ecx, 2
loc_5_BFE8:
        or     ecx, ecx
        je     SHORT loc_5_C001
        push   ecx
        push   0
        push   0
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 34h]
        db  0E8h, 0FCh, 07Dh, 0FAh, 0FFh
        add    esp, 14h
loc_5_C001:
        push   dword ptr [esp + 18h]
        call   sub_5_9B44
        add    esp, 4
        mov    si, ax
loc_5_C010:
        or     ebx, ebx
        je     SHORT loc_5_C01D
        push   ebx
        db  0E8h, 076h, 083h, 0FAh, 0FFh
        add    esp, 4
loc_5_C01D:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_C030
        push   dword ptr [esp + 14h]
        db  0E8h, 063h, 083h, 0FAh, 0FFh
        add    esp, 4
loc_5_C030:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_5_C043
        push   dword ptr [esp + 10h]
        db  0E8h, 050h, 083h, 0FAh, 0FFh
        add    esp, 4
loc_5_C043:
        mov    ax, si
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_5_C04E:
        db  0A1h, 094h, 0B8h, 000h, 000h
        sub    edx, edx
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 48h]
        cmp    eax, edx
        je     SHORT loc_5_C06F
        mov    ecx, dword ptr [esp + 4]
loc_5_C062:
        cmp    dword ptr [eax + 4], ecx
        je     SHORT loc_5_C06F
        mov    edx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_5_C062
loc_5_C06F:
        mov    ecx, dword ptr [esp + 8]
        or     ecx, ecx
        je     SHORT loc_5_C079
        mov    dword ptr [ecx], edx
loc_5_C079:
        ret
sub_5_C07A:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 20h]
        mov    esi, dword ptr [edi]
        or     esi, esi
        jne    SHORT loc_5_C099
        mov    dword ptr [esp + 10h], 1000h
        mov    ebx, dword ptr [esp + 24h]
        jmp    SHORT loc_5_C0A3
loc_5_C099:
        mov    ebx, dword ptr [esp + 24h]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 10h], eax
loc_5_C0A3:
        sub    eax, eax
        sub    ebp, ebp
loc_5_C0A7:
        cmp    eax, 7ah
        jne    SHORT loc_5_C0BD
        push   esi
        db  0E8h, 0DEh, 082h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [edi], 0
        sub    esi, esi
loc_5_C0BD:
        or     esi, esi
        jne    SHORT loc_5_C0E0
        push   200h
        push   dword ptr [esp + 14h]
        db  0E8h, 095h, 082h, 0FAh, 0FFh
        add    esp, 8
        mov    esi, eax
        mov    dword ptr [edi], esi
        or     esi, esi
        je     SHORT loc_5_C112
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx], eax
loc_5_C0E0:
        lea    eax, [esp + 10h]
        push   dword ptr [esp + 28h]
        push   eax
        push   dword ptr [esp + 18h]
        push   esi
        push   2
        push   2710h
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 34h]
        db  0E8h, 0ECh, 07Ch, 0FAh, 0FFh
        add    esp, 20h
        cmp    eax, 7ah
        jne    SHORT loc_5_C11D
        inc    ebp
        cmp    ebp, 0ah
        jb     SHORT loc_5_C0A7
        jmp    SHORT loc_5_C11D
loc_5_C112:
        mov    dword ptr [ebx], 0
        mov    eax, 8
loc_5_C11D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_C125:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 24h]
        db  068h, 028h, 09Bh, 000h, 000h
        mov    esi, dword ptr [eax + 0ah]
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 20h], 0
        db  0E8h, 073h, 0D3h, 0FAh, 0FFh
        add    esp, 4
        push   esi
        call   sub_5_125F
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_C16E
        mov    si, 0fch
        jmp    NEAR PTR loc_5_C254
loc_5_C16E:
        mov    edi, dword ptr [eax + 0ch]
        or     edi, edi
        je     NEAR PTR loc_5_C250
        cmp    byte ptr [edi + 4], 1
        jne    NEAR PTR loc_5_C250
        push   80h
        push   eax
        call   sub_5_A0B3
        add    esp, 8
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_C254
        lea    eax, [esp + 18h]
        push   0
        push   eax
        add    edi, 16h
        push   edi
        call   sub_5_9A9B
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_C1C1
        mov    dword ptr [esp + 18h], 0
        jmp    NEAR PTR loc_5_C250
loc_5_C1C1:
        sub    ebx, ebx
        sub    esi, esi
        mov    edi, dword ptr [esp + 28h]
loc_5_C1C9:
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   ebx
        push   dword ptr [esp + 28h]
        call   sub_5_C07A
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_5_C242
        sub    eax, eax
        cmp    dword ptr [esp + 10h], eax
        jbe    SHORT loc_5_C217
        sub    edx, edx
        lea    ecx, [edi + esi*2 + 0ah]
loc_5_C1F7:
        mov    ebp, dword ptr [esp + 14h]
        mov    ebp, dword ptr [edx + ebp]
        mov    word ptr [ecx], bp
        add    ecx, 2
        inc    esi
        cmp    esi, 0fah
        jae    SHORT loc_5_C222
        add    edx, 68h
        inc    eax
        cmp    eax, dword ptr [esp + 10h]
        jb     SHORT loc_5_C1F7
loc_5_C217:
        add    ebx, dword ptr [esp + 10h]
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_5_C1C9
loc_5_C222:
        push   esi
        db  0E8h, 038h, 078h, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [edi + 8], ax
        mov    eax, dword ptr [esp + 2ch]
        lea    ecx, [esi*2 + 2]
        mov    word ptr [eax], cx
        sub    si, si
        jmp    SHORT loc_5_C254
loc_5_C242:
        push   eax
        call   sub_5_9B44
        add    esp, 4
        mov    si, ax
        jmp    SHORT loc_5_C254
loc_5_C250:
        mov    si, 0d1h
loc_5_C254:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0AEh, 0D2h, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_C274
        push   dword ptr [esp + 14h]
        db  0E8h, 01Fh, 081h, 0FAh, 0FFh
        add    esp, 4
loc_5_C274:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_5_C287
        push   dword ptr [esp + 18h]
        db  0E8h, 058h, 07Bh, 0FAh, 0FFh
        add    esp, 4
loc_5_C287:
        mov    ax, si
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_C292:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 2ch]
        db  068h, 028h, 09Bh, 000h, 000h
        mov    esi, dword ptr [edi + 0ah]
        mov    dword ptr [esp + 20h], 0
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 24h], 0
        db  0E8h, 006h, 0D2h, 0FAh, 0FFh
        add    esp, 4
        push   esi
        call   sub_5_125F
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_C2DB
        mov    si, 0fch
        jmp    NEAR PTR loc_5_C3FB
loc_5_C2DB:
        mov    ebx, dword ptr [eax + 0ch]
        or     ebx, ebx
        je     NEAR PTR loc_5_C3F7
        cmp    byte ptr [ebx + 4], 1
        jne    NEAR PTR loc_5_C3F7
        push   80h
        push   eax
        call   sub_5_A0B3
        add    esp, 8
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_C3FB
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        add    ebx, 16h
        push   ebx
        call   sub_5_9A9B
        mov    dword ptr [esp + 24h], eax
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_C332
        mov    dword ptr [esp + 1ch], 0
        jmp    NEAR PTR loc_5_C3F7
loc_5_C332:
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_5_AE9E
        mov    dword ptr [esp + 2ch], eax
        add    esp, 8
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_5_C353
        jmp    NEAR PTR loc_5_C3F7
loc_5_C353:
        mov    ebx, dword ptr [edi + 0eh]
        or     ebx, ebx
        jne    SHORT loc_5_C35F
        mov    ebx, 1
loc_5_C35F:
        sub    esi, esi
        dec    ebx
        mov    edi, dword ptr [esp + 30h]
loc_5_C366:
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   ebx
        push   dword ptr [esp + 2ch]
        call   sub_5_C07A
        mov    dword ptr [esp + 2ch], eax
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_5_C3E6
        sub    ecx, ecx
        cmp    dword ptr [esp + 10h], ecx
        jbe    SHORT loc_5_C3B4
        sub    edx, edx
        lea    eax, [edi + esi*4 + 10h]
loc_5_C398:
        mov    ebp, dword ptr [esp + 14h]
        mov    ebp, dword ptr [edx + ebp]
        mov    dword ptr [eax], ebp
        add    eax, 4
        inc    esi
        cmp    esi, 7dh
        jae    SHORT loc_5_C3BF
        add    edx, 68h
        inc    ecx
        cmp    dword ptr [esp + 10h], ecx
        ja     SHORT loc_5_C398
loc_5_C3B4:
        add    ebx, dword ptr [esp + 10h]
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_5_C366
loc_5_C3BF:
        cmp    esi, dword ptr [esp + 24h]
        jbe    SHORT loc_5_C3C9
        mov    dword ptr [esp + 24h], esi
loc_5_C3C9:
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi + 0ch], esi
        mov    eax, dword ptr [esp + 34h]
        lea    ecx, [esi*4 + 8]
        mov    word ptr [eax], cx
        sub    si, si
        jmp    SHORT loc_5_C3FB
loc_5_C3E6:
        push   dword ptr [esp + 18h]
        call   sub_5_9B44
        add    esp, 4
        mov    si, ax
        jmp    SHORT loc_5_C3FB
loc_5_C3F7:
        mov    si, 0d1h
loc_5_C3FB:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 007h, 0D1h, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_C41B
        push   dword ptr [esp + 14h]
        db  0E8h, 078h, 07Fh, 0FAh, 0FFh
        add    esp, 4
loc_5_C41B:
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_5_C42E
        push   dword ptr [esp + 1ch]
        db  0E8h, 0B1h, 079h, 0FAh, 0FFh
        add    esp, 4
loc_5_C42E:
        mov    ax, si
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
sub_5_C439:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 10h]
        push   eax
        sub    edi, edi
        push   dword ptr [esp + 20h]
        call   sub_5_C04E
        add    esp, 8
        mov    esi, eax
        cmp    esi, edi
        je     SHORT loc_5_C474
        mov    edi, dword ptr [esi + 8]
        mov    ax, word ptr [esi + 0ch]
        cmp    ax, 0ffffh
        je     SHORT loc_5_C4BD
        movzx  eax, ax
        push   eax
        call   sub_5_9779
        add    esp, 4
        jmp    SHORT loc_5_C4BD
loc_5_C474:
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 20h]
        push   0
        call   sub_5_AD7C
        add    esp, 0ch
        mov    ebp, eax
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_C4A1
        push   dword ptr [esp + 14h]
        call   sub_5_9B44
        add    esp, 4
        mov    bx, ax
        jmp    SHORT loc_5_C4FD
loc_5_C4A1:
        push   2
        push   ebp
        call   sub_5_A1A7
        add    esp, 8
        mov    bx, ax
        push   ebp
        db  0E8h, 0DBh, 07Eh, 0FAh, 0FFh
        add    esp, 4
        or     bx, bx
        jne    SHORT loc_5_C4FD
loc_5_C4BD:
        push   dword ptr [esp + 1ch]
        push   edi
        call   sub_5_A7A2
        add    esp, 8
        push   eax
        call   sub_5_9B44
        add    esp, 4
        mov    bx, ax
        or     esi, esi
        je     SHORT loc_5_C4FD
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_5_C4EB
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_5_C4F4
loc_5_C4EB:
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax + 48h], ecx
loc_5_C4F4:
        push   esi
        db  0E8h, 096h, 07Eh, 0FAh, 0FFh
        add    esp, 4
loc_5_C4FD:
        mov    ax, bx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_5_C508:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        lea    eax, [esp + 14h]
        mov    edi, dword ptr [esp + 1ch]
        push   eax
        push   edi
        call   sub_5_C04E
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_C55A
        lea    eax, [esp + 10h]
        push   eax
        push   edi
        push   0
        call   sub_5_AD7C
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_C543
        push   eax
        db  0E8h, 050h, 07Eh, 0FAh, 0FFh
        add    esp, 4
loc_5_C543:
        sub    ax, ax
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_5_C607
        mov    ax, 0d5h
        jmp    NEAR PTR loc_5_C607
loc_5_C55A:
        mov    ax, word ptr [esi + 0ch]
        cmp    ax, 0ffffh
        je     SHORT loc_5_C5BA
        test   byte ptr [esi + 14h], 8
        jne    SHORT loc_5_C5AD
        movzx  ecx, ax
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 44h]
        mov    eax, dword ptr [eax + ecx*4]
        mov    bx, word ptr [eax]
        lea    eax, [esp + 0eh]
        push   eax
        push   ebx
        db  0E8h, 08Dh, 065h, 0FAh, 0FFh
        add    esp, 8
        cmp    word ptr [esp + 0eh], 0
        jne    SHORT loc_5_C5AD
        or     eax, eax
        je     SHORT loc_5_C5AD
        lea    ecx, [esp + 0eh]
        push   ecx
        push   1
        push   eax
        db  068h, 054h, 0B2h, 000h, 000h
        push   ebx
        db  0E8h, 0CEh, 065h, 0FAh, 0FFh
        add    esp, 14h
loc_5_C5AD:
        movzx  eax, word ptr [esi + 0ch]
        push   eax
        call   sub_5_9779
        add    esp, 4
loc_5_C5BA:
        push   edi
        push   dword ptr [esi + 8]
        db  0E8h, 013h, 078h, 0FAh, 0FFh
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        push   dword ptr [esi + 8]
        db  0E8h, 00Ah, 078h, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_C5E6
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_5_C5F2
loc_5_C5E6:
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 48h], ecx
loc_5_C5F2:
        push   esi
        db  0E8h, 098h, 07Dh, 0FAh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 10h]
        call   sub_5_9B44
        add    esp, 4
loc_5_C607:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_C60E:
        push   esi
        mov    eax, dword ptr [esp + 8]
        db  068h, 028h, 09Bh, 000h, 000h
        mov    esi, dword ptr [eax + 0eh]
        and    esi, 0ffffh
        db  0E8h, 0A2h, 0CEh, 0FAh, 0FFh
        add    esp, 4
        push   80h
        mov    eax, dword ptr [esp + 10h]
        push   1
        add    eax, 8
        push   eax
        push   esi
        push   0
        call   sub_5_B723
        add    esp, 14h
        mov    si, ax
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0BCh, 0CEh, 0FAh, 0FFh
        add    esp, 4
        or     si, si
        jne    SHORT loc_5_C661
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax], 118h
loc_5_C661:
        mov    ax, si
        pop    esi
        ret
loc_5_C666:
        push   esi
        mov    eax, dword ptr [esp + 8]
        db  068h, 028h, 09Bh, 000h, 000h
        movzx  esi, word ptr [eax + 0eh]
        db  0E8h, 04Fh, 0CEh, 0FAh, 0FFh
        add    esp, 4
        push   80h
        mov    eax, dword ptr [esp + 10h]
        push   1
        add    eax, 8
        push   eax
        push   esi
        push   0
        call   sub_5_BBBD
        add    esp, 14h
        mov    si, ax
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 069h, 0CEh, 0FAh, 0FFh
        add    esp, 4
        or     si, si
        jne    SHORT loc_5_C6B4
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax], 100h
loc_5_C6B4:
        mov    ax, si
        pop    esi
        ret
loc_5_C6B9:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        db  068h, 028h, 09Bh, 000h, 000h
        mov    ebx, dword ptr [eax + 0ah]
        mov    ebp, dword ptr [eax + 0eh]
        and    ebp, 0ffffh
        db  0E8h, 0EEh, 0CDh, 0FAh, 0FFh
        lea    eax, [esp + 14h]
        add    esp, 4
        push   eax
        push   ebp
        push   0
        call   sub_5_AD7C
        add    esp, 0ch
        mov    edi, eax
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_5_C707
        push   dword ptr [esp + 10h]
        call   sub_5_9B44
        add    esp, 4
        mov    si, ax
        jmp    SHORT loc_5_C741
loc_5_C707:
        push   80h
        push   edi
        call   sub_5_A1A7
        add    esp, 8
        mov    si, ax
        or     si, si
        jne    SHORT loc_5_C741
        mov    esi, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 8], ebx
        mov    dword ptr [esi + 0ch], ebp
        push   dword ptr [edi + 4ch]
        db  0E8h, 039h, 073h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [esp + 20h]
        sub    si, si
        mov    word ptr [eax], 0ch
loc_5_C741:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0C1h, 0CDh, 0FAh, 0FFh
        add    esp, 4
        or     edi, edi
        je     SHORT loc_5_C75B
        push   edi
        db  0E8h, 038h, 07Ch, 0FAh, 0FFh
        add    esp, 4
loc_5_C75B:
        mov    ax, si
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_C766:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        db  068h, 028h, 09Bh, 000h, 000h
        mov    ebx, dword ptr [eax + 0ah]
        movzx  ebp, word ptr [eax + 0eh]
        db  0E8h, 046h, 0CDh, 0FAh, 0FFh
        lea    eax, [esp + 14h]
        add    esp, 4
        push   eax
        push   ebp
        push   0
        call   sub_5_AD7C
        add    esp, 0ch
        mov    edi, eax
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_5_C7AF
        push   dword ptr [esp + 10h]
        call   sub_5_9B44
        add    esp, 4
        mov    si, ax
        jmp    SHORT loc_5_C7EA
loc_5_C7AF:
        push   80h
        push   edi
        call   sub_5_A1A7
        add    esp, 8
        mov    si, ax
        or     si, si
        jne    SHORT loc_5_C7EA
        mov    esi, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 8], ebx
        mov    word ptr [esi + 0ch], bp
        push   dword ptr [edi + 4ch]
        db  0E8h, 090h, 072h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 0eh], eax
        mov    eax, dword ptr [esp + 20h]
        sub    si, si
        mov    word ptr [eax], 0ah
loc_5_C7EA:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 018h, 0CDh, 0FAh, 0FFh
        add    esp, 4
        or     edi, edi
        je     SHORT loc_5_C804
        push   edi
        db  0E8h, 08Fh, 07Bh, 0FAh, 0FFh
        add    esp, 4
loc_5_C804:
        mov    ax, si
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_C80F:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 18h]
        db  068h, 028h, 09Bh, 000h, 000h
        movzx  edi, word ptr [ebx + 0eh]
        db  0E8h, 0A0h, 0CCh, 0FAh, 0FFh
        lea    eax, [esp + 14h]
        add    esp, 4
        push   eax
        push   edi
        push   0
        call   sub_5_AD7C
        add    esp, 0ch
        mov    esi, eax
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_5_C84A
        push   dword ptr [esp + 10h]
        jmp    SHORT loc_5_C875
loc_5_C84A:
        push   2
        push   esi
        call   sub_5_A1A7
        add    esp, 8
        mov    bp, ax
        or     bp, bp
        jne    SHORT loc_5_C880
        movzx  eax, byte ptr [ebx + 10h]
        mov    dword ptr [esi + 3ch], eax
        push   0
        push   esi
        push   2
        push   edi
        push   0
        db  0E8h, 089h, 075h, 0FAh, 0FFh
        add    esp, 14h
        push   eax
loc_5_C875:
        call   sub_5_9B44
        add    esp, 4
        mov    bp, ax
loc_5_C880:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 082h, 0CCh, 0FAh, 0FFh
        add    esp, 4
        or     esi, esi
        je     SHORT loc_5_C89A
        push   esi
        db  0E8h, 0F9h, 07Ah, 0FAh, 0FFh
        add    esp, 4
loc_5_C89A:
        mov    ax, bp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_C8A5:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        db  068h, 028h, 09Bh, 000h, 000h
        mov    edi, dword ptr [esi + 0eh]
        and    edi, 0ffffh
        db  0E8h, 005h, 0CCh, 0FAh, 0FFh
        lea    eax, [esp + 14h]
        add    esp, 4
        push   eax
        push   edi
        push   0
        call   sub_5_AD7C
        add    esp, 0ch
        mov    ebx, eax
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_5_C8E5
        push   dword ptr [esp + 10h]
        jmp    SHORT loc_5_C910
loc_5_C8E5:
        push   2
        push   ebx
        call   sub_5_A1A7
        add    esp, 8
        mov    bp, ax
        or     bp, bp
        jne    SHORT loc_5_C91B
        movzx  eax, byte ptr [esi + 12h]
        mov    dword ptr [ebx + 3ch], eax
        push   0
        push   ebx
        push   2
        push   edi
        push   0
        db  0E8h, 0EEh, 074h, 0FAh, 0FFh
        add    esp, 14h
        push   eax
loc_5_C910:
        call   sub_5_9B44
        add    esp, 4
        mov    bp, ax
loc_5_C91B:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0E7h, 0CBh, 0FAh, 0FFh
        add    esp, 4
        or     ebx, ebx
        je     SHORT loc_5_C935
        push   ebx
        db  0E8h, 05Eh, 07Ah, 0FAh, 0FFh
        add    esp, 4
loc_5_C935:
        mov    ax, bp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_C940:
        mov    eax, dword ptr [esp + 4]
        push   2
        mov    edx, dword ptr [eax + 34h]
        and    edx, 0ffffh
        push   dword ptr [esp + 14h]
        push   0
        push   1
        add    eax, 0eh
        push   eax
        push   edx
        push   0
        call   sub_5_B99D
        add    esp, 1ch
        ret
loc_5_C967:
        push   2
        push   dword ptr [esp + 14h]
        push   0
        mov    eax, dword ptr [esp + 10h]
        push   1
        lea    ecx, [eax + 0eh]
        push   ecx
        movzx  eax, word ptr [eax + 24h]
        push   eax
        push   0
        call   sub_5_BE32
        add    esp, 1ch
        ret
loc_5_C989:
        mov    eax, dword ptr [esp + 4]
        movzx  eax, word ptr [eax + 0eh]
        push   eax
        call   sub_5_C508
        add    esp, 4
        ret
loc_5_C99B:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0eh]
        and    eax, 0ffffh
        push   eax
        call   sub_5_C508
        add    esp, 4
        ret
loc_5_C9B1:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [esp + 10h]
        movzx  eax, word ptr [eax + 0eh]
        push   eax
        call   sub_5_C439
        add    esp, 8
        ret
loc_5_C9C7:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax + 0eh]
        and    ecx, 0ffffh
        push   ecx
        call   sub_5_C439
        add    esp, 8
        ret
loc_5_C9E2:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 28h]
        sub    ebx, ebx
        mov    edi, dword ptr [esi + 0ah]
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 14h], 64h
        mov    dword ptr [esp + 20h], 0ffffffffh
        mov    word ptr [esp + 12h], 0ffffh
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    dword ptr [esp + 18h], ebx
        mov    al, byte ptr [eax + 491h]
        mov    byte ptr [esp + 11h], al
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 09Ah, 0CAh, 0FAh, 0FFh
        add    esp, 4
        push   edi
        call   sub_5_125F
        add    esp, 4
        cmp    eax, ebx
        jne    SHORT loc_5_CA47
        mov    di, 0fch
        jmp    NEAR PTR loc_5_CC49
loc_5_CA47:
        mov    ebp, dword ptr [eax + 0ch]
        or     ebp, ebp
        je     NEAR PTR loc_5_CC45
        cmp    byte ptr [ebp + 4], 1
        jne    NEAR PTR loc_5_CC45
        push   4
        push   eax
        call   sub_5_A0B3
        add    esp, 8
        mov    di, ax
        or     di, di
        jne    NEAR PTR loc_5_CC49
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        add    ebp, 16h
        push   ebp
        call   sub_5_9A9B
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_CA97
        mov    dword ptr [esp + 1ch], 0
        jmp    NEAR PTR loc_5_CC45
loc_5_CA97:
        mov    eax, 7ah
        sub    ebp, ebp
loc_5_CA9E:
        cmp    ebp, 0ah
        jae    SHORT loc_5_CAE9
        or     ebx, ebx
        je     SHORT loc_5_CAB0
        push   ebx
        db  0E8h, 0E3h, 078h, 0FAh, 0FFh
        add    esp, 4
loc_5_CAB0:
        push   200h
        push   dword ptr [esp + 18h]
        db  0E8h, 0A6h, 078h, 0FAh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_5_CC49
        lea    eax, [esp + 14h]
        push   eax
        inc    ebp
        push   dword ptr [esp + 18h]
        push   ebx
        push   1
        push   dword ptr [esp + 2ch]
        db  0E8h, 0EFh, 072h, 0FAh, 0FFh
        add    esp, 14h
        cmp    eax, 7ah
        je     SHORT loc_5_CA9E
loc_5_CAE9:
        or     eax, eax
        je     SHORT loc_5_CAFE
        push   eax
        call   sub_5_9B44
        add    esp, 4
        mov    di, ax
        jmp    NEAR PTR loc_5_CC49
loc_5_CAFE:
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esp + 20h], eax
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    word ptr [eax + 491h], 0ffh
        lea    eax, [esp + 12h]
        push   eax
        push   dword ptr [ebx]
        db  0E8h, 0FCh, 063h, 0FAh, 0FFh
        add    esp, 8
        mov    di, ax
        or     di, di
        jne    NEAR PTR loc_5_CC49
        mov    ebp, dword ptr [esp + 34h]
        push   0
        push   ebp
        push   1
        lea    eax, [esi + 0eh]
        push   1
        push   eax
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 34h]
        call   sub_5_B99D
        add    esp, 1ch
        mov    di, ax
        or     di, di
        jne    NEAR PTR loc_5_CC49
        push   0
        mov    eax, dword ptr [esp + 30h]
        push   0
        add    eax, 8
        push   eax
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        call   sub_5_B723
        add    esp, 14h
        mov    di, ax
        or     di, di
        jne    NEAR PTR loc_5_CC49
        push   200h
        push   0a7h
        db  0E8h, 0D4h, 077h, 0FAh, 0FFh
        mov    dword ptr [esp + 20h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_CBA4
        mov    di, 96h
        jmp    NEAR PTR loc_5_CC49
loc_5_CBA4:
        add    esi, 8eh
        mov    edi, dword ptr [esp + 18h]
        add    edi, 0fh
        mov    ecx, 26h
        rep movsd
        mov    edi, dword ptr [ebx]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    esi, ecx
        je     SHORT loc_5_CBD7
loc_5_CBCA:
        mov    eax, dword ptr [ebx]
        lea    edx, [esi + eax]
        dec    esi
        mov    al, byte ptr [edx - 1]
        mov    byte ptr [edx], al
        jne    SHORT loc_5_CBCA
loc_5_CBD7:
        mov    eax, dword ptr [ebx]
        push   0
        mov    byte ptr [eax], cl
        movzx  eax, word ptr [esp + 16h]
        push   dword ptr [ebx]
        push   eax
        push   ebp
        db  0E8h, 0F5h, 09Bh, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_5_CBF8
        mov    di, 96h
        jmp    SHORT loc_5_CC49
loc_5_CBF8:
        db  08Bh, 015h, 094h, 0B8h, 000h, 000h
        movzx  cx, byte ptr [esp + 11h]
        mov    word ptr [edx + 491h], cx
        mov    ecx, dword ptr [esp + 2ch]
        mov    dword ptr [ecx + 46h], eax
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx + 8], edx
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ecx + 4], eax
        mov    al, byte ptr [esp + 11h]
        sub    di, di
        mov    byte ptr [ecx + 0eh], al
        mov    eax, dword ptr [ebp + 48h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [ebp + 48h], ecx
        mov    word ptr [eax], 4eh
        jmp    SHORT loc_5_CC49
loc_5_CC45:
        mov    di, 0d1h
loc_5_CC49:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0B9h, 0C8h, 0FAh, 0FFh
        add    esp, 4
        or     ebx, ebx
        je     SHORT loc_5_CC63
        push   ebx
        db  0E8h, 030h, 077h, 0FAh, 0FFh
        add    esp, 4
loc_5_CC63:
        or     di, di
        je     SHORT loc_5_CCA7
        cmp    dword ptr [esp + 20h], -1
        je     SHORT loc_5_CC7F
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        call   sub_5_A7A2
        add    esp, 8
loc_5_CC7F:
        cmp    word ptr [esp + 12h], 0ffffh
        je     SHORT loc_5_CC94
        push   dword ptr [esp + 12h]
        db  0E8h, 06Ch, 05Fh, 0FAh, 0FFh
        add    esp, 4
loc_5_CC94:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_5_CCA7
        push   dword ptr [esp + 18h]
        db  0E8h, 0ECh, 076h, 0FAh, 0FFh
        add    esp, 4
loc_5_CCA7:
        mov    ax, di
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_5_CCB2:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 28h]
        sub    ebx, ebx
        mov    edi, dword ptr [esi + 0ah]
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 14h], 64h
        mov    dword ptr [esp + 20h], 0ffffffffh
        mov    word ptr [esp + 12h], 0ffffh
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    dword ptr [esp + 18h], ebx
        mov    al, byte ptr [eax + 491h]
        mov    byte ptr [esp + 11h], al
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0CAh, 0C7h, 0FAh, 0FFh
        add    esp, 4
        push   edi
        call   sub_5_125F
        add    esp, 4
        cmp    eax, ebx
        jne    SHORT loc_5_CD17
        mov    di, 0fch
        jmp    NEAR PTR loc_5_CF22
loc_5_CD17:
        mov    ebp, dword ptr [eax + 0ch]
        or     ebp, ebp
        je     NEAR PTR loc_5_CF1E
        cmp    byte ptr [ebp + 4], 1
        jne    NEAR PTR loc_5_CF1E
        push   4
        push   eax
        call   sub_5_A0B3
        add    esp, 8
        mov    di, ax
        or     di, di
        jne    NEAR PTR loc_5_CF22
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        add    ebp, 16h
        push   ebp
        call   sub_5_9A9B
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_CD67
        mov    dword ptr [esp + 1ch], 0
        jmp    NEAR PTR loc_5_CF1E
loc_5_CD67:
        mov    eax, 7ah
        sub    ebp, ebp
loc_5_CD6E:
        cmp    ebp, 0ah
        jae    SHORT loc_5_CDB9
        or     ebx, ebx
        je     SHORT loc_5_CD80
        push   ebx
        db  0E8h, 013h, 076h, 0FAh, 0FFh
        add    esp, 4
loc_5_CD80:
        push   200h
        push   dword ptr [esp + 18h]
        db  0E8h, 0D6h, 075h, 0FAh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_5_CF22
        lea    eax, [esp + 14h]
        push   eax
        inc    ebp
        push   dword ptr [esp + 18h]
        push   ebx
        push   1
        push   dword ptr [esp + 2ch]
        db  0E8h, 01Fh, 070h, 0FAh, 0FFh
        add    esp, 14h
        cmp    eax, 7ah
        je     SHORT loc_5_CD6E
loc_5_CDB9:
        or     eax, eax
        je     SHORT loc_5_CDCE
        push   eax
        call   sub_5_9B44
        add    esp, 4
        mov    di, ax
        jmp    NEAR PTR loc_5_CF22
loc_5_CDCE:
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esp + 20h], eax
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    word ptr [eax + 491h], 0ffh
        lea    eax, [esp + 12h]
        push   eax
        push   dword ptr [ebx]
        db  0E8h, 02Ch, 061h, 0FAh, 0FFh
        add    esp, 8
        mov    di, ax
        or     di, di
        jne    NEAR PTR loc_5_CF22
        push   0
        lea    eax, [esi + 0eh]
        push   dword ptr [esp + 38h]
        push   1
        push   1
        push   eax
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 34h]
        call   sub_5_BE32
        add    esp, 1ch
        mov    di, ax
        or     di, di
        jne    NEAR PTR loc_5_CF22
        push   0
        mov    ebp, dword ptr [esp + 30h]
        push   0
        lea    eax, [ebp + 8]
        push   eax
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        call   sub_5_BBBD
        add    esp, 14h
        mov    di, ax
        or     di, di
        jne    NEAR PTR loc_5_CF22
        push   200h
        push   0a7h
        db  0E8h, 005h, 075h, 0FAh, 0FFh
        mov    dword ptr [esp + 20h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_CE73
        mov    di, 96h
        jmp    NEAR PTR loc_5_CF22
loc_5_CE73:
        add    esi, 76h
        mov    edi, dword ptr [esp + 18h]
        add    edi, 0fh
        mov    ecx, 26h
        rep movsd
        mov    edi, dword ptr [ebx]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    esi, ecx
        je     SHORT loc_5_CEA3
loc_5_CE96:
        mov    eax, dword ptr [ebx]
        lea    edx, [esi + eax]
        dec    esi
        mov    al, byte ptr [edx - 1]
        mov    byte ptr [edx], al
        jne    SHORT loc_5_CE96
loc_5_CEA3:
        mov    eax, dword ptr [ebx]
        push   0
        mov    byte ptr [eax], cl
        movzx  eax, word ptr [esp + 16h]
        push   dword ptr [ebx]
        push   eax
        push   dword ptr [esp + 40h]
        db  0E8h, 026h, 099h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_5_CEC7
        mov    di, 96h
        jmp    SHORT loc_5_CF22
loc_5_CEC7:
        db  08Bh, 015h, 094h, 0B8h, 000h, 000h
        movzx  cx, byte ptr [esp + 11h]
        mov    word ptr [edx + 491h], cx
        mov    dword ptr [ebp + 34h], eax
        mov    word ptr [ebp + 32h], 0
        mov    edx, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [edx + 8], ecx
        mov    word ptr [edx + 0ch], ax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [edx + 4], eax
        mov    al, byte ptr [esp + 11h]
        mov    byte ptr [edx + 0eh], al
        mov    eax, dword ptr [esp + 34h]
        sub    di, di
        mov    eax, dword ptr [eax + 48h]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [eax + 48h], edx
        mov    eax, dword ptr [esp + 30h]
        mov    word ptr [eax], 36h
        jmp    SHORT loc_5_CF22
loc_5_CF1E:
        mov    di, 0d1h
loc_5_CF22:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0E0h, 0C5h, 0FAh, 0FFh
        add    esp, 4
        or     ebx, ebx
        je     SHORT loc_5_CF3C
        push   ebx
        db  0E8h, 057h, 074h, 0FAh, 0FFh
        add    esp, 4
loc_5_CF3C:
        or     di, di
        je     SHORT loc_5_CF80
        cmp    dword ptr [esp + 20h], -1
        je     SHORT loc_5_CF58
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        call   sub_5_A7A2
        add    esp, 8
loc_5_CF58:
        cmp    word ptr [esp + 12h], 0ffffh
        je     SHORT loc_5_CF6D
        push   dword ptr [esp + 12h]
        db  0E8h, 093h, 05Ch, 0FAh, 0FFh
        add    esp, 4
loc_5_CF6D:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_5_CF80
        push   dword ptr [esp + 18h]
        db  0E8h, 013h, 074h, 0FAh, 0FFh
        add    esp, 4
loc_5_CF80:
        mov    ax, di
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
sub_5_CF8B:
        db  0E8h, 04Eh, 0B4h, 0FAh, 0FFh
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_5_CF9E
        mov    eax, 1
loc_5_CF9E:
        ret
sub_5_CF9F:
        ret
loc_5_CFA0:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 030h, 0B4h, 000h, 000h
        mov    dword ptr [esp + 18h], eax
        mov    ebx, dword ptr [eax + 24h]
        db  068h, 058h, 0B4h, 000h, 000h
        lea    eax, [ebx + 1eh]
        mov    dword ptr [esp + 18h], eax
        db  0E8h, 004h, 0C5h, 0FAh, 0FFh
        add    esp, 4
        db  083h, 03Dh, 018h, 0B4h, 000h, 000h, 000h
        jne    NEAR PTR loc_5_D1EF
        db  0A1h, 01Ch, 0B4h, 000h, 000h
        db  039h, 005h, 020h, 0B4h, 000h, 000h
        jbe    NEAR PTR loc_5_D1EF
        push   1
        push   0a4h
        db  0E8h, 073h, 073h, 0FAh, 0FFh
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_5_D1EF
        db  0A1h, 094h, 0B8h, 000h, 000h
        push   1
        mov    word ptr [eax + 491h], 0ffh
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    dword ptr [eax], ebp
        db  0E8h, 0BEh, 0C2h, 0FAh, 0FFh
        mov    dword ptr [esp + 20h], eax
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_5_D185
        mov    dword ptr [ebp + 10h], eax
        push   100h
        push   44h
        db  0E8h, 02Ch, 073h, 0FAh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp + 4ch], eax
        or     eax, eax
        je     NEAR PTR loc_5_D179
        push   3e8h
        push   10h
        mov    edx, 10h
        lea    ecx, [ebp + 44h]
        db  0E8h, 036h, 05Fh, 0FAh, 0FFh
        or     eax, eax
        je     NEAR PTR loc_5_D16E
        push   ebp
        call   sub_5_43D3
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_5_D15F
        mov    word ptr [ebp + 18h], 100h
        db  066h, 0A1h, 0E4h, 0AFh, 000h, 000h
        mov    word ptr [ebp + 26h], ax
        mov    byte ptr [ebp + 6ah], 0
        db  0BEh, 074h, 0B4h, 000h, 000h
        mov    byte ptr [ebp + 72h], 0dh
        lea    edi, [ebp + 73h]
        movsd
        movsd
        movsd
        movsb
        lea    ebx, [ebx + 12h]
        lea    eax, [ebp + 28h]
        mov    edi, eax
        mov    esi, ebx
        movsd
        movsd
        movsd
        db  0BFh, 004h, 0B4h, 000h, 000h
        mov    ecx, 2
        mov    esi, ebx
        repe cmpsd
        jne    SHORT loc_5_D0C1
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
        jne    SHORT loc_5_D0C1
        or     word ptr [ebp + 1ah], 10h
loc_5_D0C1:
        lea    eax, [ebp + 8]
        mov    dword ptr [ebp + 0ch], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebp + 58h]
        mov    dword ptr [ebp + 5ch], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebp + 6bh]
        push   eax
        db  0E8h, 0BAh, 069h, 0FAh, 0FFh
        add    esp, 4
        db  08Bh, 01Dh, 050h, 0B4h, 000h, 000h
        db  081h, 0FBh, 050h, 0B4h, 000h, 000h
        je     SHORT loc_5_D143
loc_5_D0EB:
        mov    dword ptr [esp + 10h], ebx
        mov    ecx, dword ptr [esp + 10h]
        mov    ebx, dword ptr [ebx]
        push   dword ptr [ecx + 10h]
        db  0E8h, 0FEh, 0C1h, 0FAh, 0FFh
        mov    esi, dword ptr [esp + 14h]
        add    esp, 4
        lea    esi, [esi + 28h]
        mov    ecx, 3
        lea    edi, [ebp + 28h]
        repe cmpsd
        je     SHORT loc_5_D12C
        mov    ecx, dword ptr [esp + 10h]
        push   dword ptr [ecx + 10h]
        db  0E8h, 0D2h, 0C1h, 0FAh, 0FFh
        add    esp, 4
        db  081h, 0FBh, 050h, 0B4h, 000h, 000h
        jne    SHORT loc_5_D0EB
        jmp    SHORT loc_5_D143
loc_5_D12C:
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], ecx
        db  0E8h, 0F0h, 0A9h, 0FAh, 0FFh
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    dword ptr [eax], ebp
loc_5_D143:
        db  0E8h, 08Dh, 0A8h, 0FAh, 0FFh
        mov    bx, ax
        or     bx, bx
        jne    SHORT loc_5_D190
        mov    eax, dword ptr [ebp + 40h]
        sub    eax, 4
        push   eax
        db  0E8h, 034h, 072h, 0FAh, 0FFh
        add    esp, 4
loc_5_D15F:
        mov    eax, dword ptr [ebp + 44h]
        sub    eax, 4
        push   eax
        db  0E8h, 025h, 072h, 0FAh, 0FFh
        add    esp, 4
loc_5_D16E:
        push   dword ptr [ebp + 4ch]
        db  0E8h, 01Ah, 072h, 0FAh, 0FFh
        add    esp, 4
loc_5_D179:
        push   dword ptr [esp + 1ch]
        db  0E8h, 065h, 0C1h, 0FAh, 0FFh
        add    esp, 4
loc_5_D185:
        push   ebp
        db  0E8h, 005h, 072h, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_5_D1EF
loc_5_D190:
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [ebp + 24h], bx
        mov    byte ptr [eax + 3], bl
        mov    byte ptr [eax + 5], bh
        mov    byte ptr [eax + 4], 1
        db  068h, 050h, 0B4h, 000h, 000h
        push   ebp
        db  0E8h, 083h, 02Eh, 0FAh, 0FFh
        add    esp, 8
        db  0A1h, 04Ch, 0B4h, 000h, 000h
        movzx  ecx, bx
        mov    dword ptr [eax + ecx*4], ebp
        db  0FFh, 005h, 01Ch, 0B4h, 000h, 000h
        db  0A1h, 01Ch, 0B4h, 000h, 000h
        db  039h, 005h, 028h, 0B4h, 000h, 000h
        jae    SHORT loc_5_D1D3
        db  0A3h, 028h, 0B4h, 000h, 000h
loc_5_D1D3:
        db  039h, 00Dh, 02Ch, 0B4h, 000h, 000h
        jae    SHORT loc_5_D1E1
        db  089h, 00Dh, 02Ch, 0B4h, 000h, 000h
loc_5_D1E1:
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [eax + 6], 0
        mov    byte ptr [eax + 7], 0
        jmp    SHORT loc_5_D20A
loc_5_D1EF:
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [eax + 6], 0ffh
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [eax + 7], 4
loc_5_D20A:
        mov    eax, dword ptr [esp + 14h]
        mov    edx, 26h
        mov    word ptr [eax], 3333h
        mov    ecx, dword ptr [esp + 18h]
        db  0E8h, 043h, 047h, 0FAh, 0FFh
        mov    eax, dword ptr [esp + 18h]
        push   eax
        db  0C7h, 040h, 004h, 050h, 019h, 000h, 000h
        mov    dword ptr [eax + 30h], 0
        db  0E8h, 054h, 05Fh, 0FAh, 0FFh
        add    esp, 4
        db  068h, 058h, 0B4h, 000h, 000h
        db  0E8h, 0C6h, 0C2h, 0FAh, 0FFh
        add    esp, 4
        db  0FFh, 00Dh, 034h, 0B4h, 000h, 000h
        jne    SHORT loc_5_D26A
        db  083h, 03Dh, 040h, 0B4h, 000h, 000h, 000h
        je     SHORT loc_5_D26A
        db  0C7h, 005h, 040h, 0B4h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 00Dh, 03Ch, 0B4h, 000h, 000h
loc_5_D26A:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_D280:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebp, ebp
        db  0A1h, 00Ch, 0B9h, 000h, 000h
        mov    dword ptr [esp + 10h], eax
        db  089h, 02Dh, 00Ch, 0B9h, 000h, 000h
        db  089h, 02Dh, 010h, 0B9h, 000h, 000h
        db  0FFh, 035h, 0C4h, 0B8h, 000h, 000h
        db  0E8h, 0B3h, 06Fh, 0FAh, 0FFh
        add    esp, 4
        db  089h, 02Dh, 0C4h, 0B8h, 000h, 000h
        db  039h, 02Dh, 0B8h, 0B8h, 000h, 000h
        je     SHORT loc_5_D2C6
        db  0FFh, 00Dh, 0B8h, 0B8h, 000h, 000h
        db  0FFh, 00Dh, 03Ch, 0B4h, 000h, 000h
loc_5_D2C6:
        test   byte ptr [esp + 10h], 3
        je     NEAR PTR loc_5_D3D1
        test   byte ptr [esp + 10h], 2
        mov    edi, 0
        jne    SHORT loc_5_D2E2
        mov    edi, 1
loc_5_D2E2:
        db  0BBh, 050h, 0B4h, 000h, 000h
loc_5_D2E7:
        or     edi, edi
        db  068h, 058h, 0B4h, 000h, 000h
        je     SHORT loc_5_D2F7
        db  0E8h, 047h, 0C1h, 0FAh, 0FFh
        jmp    SHORT loc_5_D2FC
loc_5_D2F7:
        db  0E8h, 0CCh, 0C1h, 0FAh, 0FFh
loc_5_D2FC:
        add    esp, 4
        db  0A1h, 050h, 0B4h, 000h, 000h
        db  0A3h, 018h, 0B9h, 000h, 000h
        cmp    eax, ebx
        je     NEAR PTR loc_5_D3AD
loc_5_D311:
        db  0A1h, 094h, 0B8h, 000h, 000h
        db  08Bh, 035h, 018h, 0B9h, 000h, 000h
        mov    dword ptr [eax], esi
        db  0A1h, 018h, 0B9h, 000h, 000h
        mov    eax, dword ptr [eax]
        db  0A3h, 018h, 0B9h, 000h, 000h
        mov    al, byte ptr [esi + 23h]
        cmp    byte ptr [esi + 21h], al
        je     SHORT loc_5_D344
        test   byte ptr [esi + 22h], 80h
        jne    SHORT loc_5_D344
        mov    byte ptr [esi + 22h], 0
        mov    al, byte ptr [esi + 21h]
        mov    byte ptr [esi + 23h], al
        jmp    SHORT loc_5_D3A1
loc_5_D344:
        mov    al, byte ptr [esi + 22h]
        lea    eax, [edi + eax]
        mov    byte ptr [esi + 22h], al
        cmp    al, 10h
        jb     SHORT loc_5_D36C
        mov    ebp, 1
        or     edi, edi
        jne    SHORT loc_5_D3A1
        push   dword ptr [esi + 10h]
        db  0E8h, 099h, 0BFh, 0FAh, 0FFh
        add    esp, 4
        db  0E8h, 0C2h, 0A7h, 0FAh, 0FFh
        jmp    SHORT loc_5_D3A1
loc_5_D36C:
        or     edi, edi
        je     SHORT loc_5_D3A1
        cmp    al, 5
        jbe    SHORT loc_5_D3A1
        push   dword ptr [esi + 10h]
        db  0E8h, 07Fh, 0BFh, 0FAh, 0FFh
        add    esp, 4
        push   esi
        db  0E8h, 0A3h, 0C3h, 0FAh, 0FFh
        add    esp, 4
        push   dword ptr [esi + 10h]
        db  0E8h, 061h, 0BFh, 0FAh, 0FFh
        add    esp, 4
        db  0FFh, 035h, 0CCh, 0B8h, 000h, 000h
        db  0E8h, 05Dh, 0BFh, 0FAh, 0FFh
        add    esp, 4
loc_5_D3A1:
        db  039h, 01Dh, 018h, 0B9h, 000h, 000h
        jne    NEAR PTR loc_5_D311
loc_5_D3AD:
        or     edi, edi
        db  068h, 058h, 0B4h, 000h, 000h
        je     SHORT loc_5_D3C9
        db  0E8h, 0D5h, 0C0h, 0FAh, 0FFh
        add    esp, 4
        or     ebp, ebp
        je     SHORT loc_5_D3D1
        sub    edi, edi
        jmp    NEAR PTR loc_5_D2E7
loc_5_D3C9:
        db  0E8h, 03Eh, 0C1h, 0FAh, 0FFh
        add    esp, 4
loc_5_D3D1:
        mov    esi, dword ptr [esp + 10h]
        and    esi, 1
        je     NEAR PTR loc_5_D463
        db  08Bh, 03Dh, 008h, 0A2h, 000h, 000h
        db  0A1h, 0F8h, 0A1h, 000h, 000h
        db  0A3h, 008h, 0A2h, 000h, 000h
        db  03Bh, 03Dh, 0FCh, 0A1h, 000h, 000h
        jbe    SHORT loc_5_D41E
loc_5_D3F6:
        db  0A1h, 0F8h, 0A1h, 000h, 000h
        db  039h, 005h, 0FCh, 0A1h, 000h, 000h
        jae    SHORT loc_5_D41E
        db  0E8h, 058h, 053h, 0FAh, 0FFh
        or     eax, eax
        je     SHORT loc_5_D41E
        push   eax
        db  0E8h, 0B8h, 054h, 0FAh, 0FFh
        add    esp, 4
        dec    edi
        db  03Bh, 03Dh, 0FCh, 0A1h, 000h, 000h
        ja     SHORT loc_5_D3F6
loc_5_D41E:
        db  08Bh, 03Dh, 020h, 0A2h, 000h, 000h
        db  0A1h, 010h, 0A2h, 000h, 000h
        db  0A3h, 020h, 0A2h, 000h, 000h
        db  03Bh, 03Dh, 014h, 0A2h, 000h, 000h
        jbe    SHORT loc_5_D45E
loc_5_D436:
        db  0A1h, 010h, 0A2h, 000h, 000h
        db  039h, 005h, 014h, 0A2h, 000h, 000h
        jae    SHORT loc_5_D45E
        db  0E8h, 08Bh, 053h, 0FAh, 0FFh
        or     eax, eax
        je     SHORT loc_5_D45E
        push   eax
        db  0E8h, 090h, 054h, 0FAh, 0FFh
        add    esp, 4
        dec    edi
        db  03Bh, 03Dh, 014h, 0A2h, 000h, 000h
        ja     SHORT loc_5_D436
loc_5_D45E:
        db  0E8h, 08Bh, 05Fh, 0FAh, 0FFh
loc_5_D463:
        test   byte ptr [esp + 10h], 4
        je     NEAR PTR loc_5_D534
        db  083h, 03Dh, 094h, 0B0h, 000h, 000h, 000h
        je     NEAR PTR loc_5_D534
        db  0C7h, 005h, 014h, 0B9h, 000h, 000h, 000h, 000h, 000h, 000h
        db  068h, 058h, 0B4h, 000h, 000h
        db  0E8h, 0ADh, 0BFh, 0FAh, 0FFh
        add    esp, 4
        db  0A1h, 050h, 0B4h, 000h, 000h
        db  0A3h, 018h, 0B9h, 000h, 000h
        db  03Dh, 050h, 0B4h, 000h, 000h
        je     SHORT loc_5_D4FE
        sub    ebx, ebx
        db  0BDh, 050h, 0B4h, 000h, 000h
loc_5_D4AA:
        db  0A1h, 094h, 0B8h, 000h, 000h
        db  08Bh, 03Dh, 018h, 0B9h, 000h, 000h
        mov    dword ptr [eax], edi
        db  0A1h, 018h, 0B9h, 000h, 000h
        mov    eax, dword ptr [eax]
        db  0A3h, 018h, 0B9h, 000h, 000h
        cmp    word ptr [edi + 60h], bx
        je     SHORT loc_5_D4F6
        push   dword ptr [edi + 10h]
        db  0E8h, 02Ah, 0BEh, 0FAh, 0FFh
        add    esp, 4
        push   edi
        db  0E8h, 0AEh, 0C2h, 0FAh, 0FFh
        add    esp, 4
        push   dword ptr [edi + 10h]
        db  0E8h, 00Ch, 0BEh, 0FAh, 0FFh
        add    esp, 4
        db  0FFh, 035h, 0CCh, 0B8h, 000h, 000h
        db  0E8h, 008h, 0BEh, 0FAh, 0FFh
        add    esp, 4
loc_5_D4F6:
        db  039h, 02Dh, 018h, 0B9h, 000h, 000h
        jne    SHORT loc_5_D4AA
loc_5_D4FE:
        db  068h, 058h, 0B4h, 000h, 000h
        db  0E8h, 088h, 0BFh, 0FAh, 0FFh
        add    esp, 4
        db  083h, 03Dh, 094h, 0B0h, 000h, 000h, 000h
        je     SHORT loc_5_D534
        db  083h, 03Dh, 0C8h, 0B8h, 000h, 000h, 000h
        jne    SHORT loc_5_D534
        push   7d0h
        db  068h, 01Ah, 098h, 000h, 000h
        db  0E8h, 018h, 06Dh, 0FAh, 0FFh
        add    esp, 8
        db  0A3h, 0C8h, 0B8h, 000h, 000h
loc_5_D534:
        or     esi, esi
        je     NEAR PTR loc_5_D5EA
        db  0FFh, 035h, 064h, 0B8h, 000h, 000h
        db  0E8h, 0B4h, 0BDh, 0FAh, 0FFh
        add    esp, 4
        db  0A1h, 05Ch, 0B8h, 000h, 000h
        db  0C7h, 005h, 05Ch, 0B8h, 000h, 000h, 000h, 000h, 000h, 000h
        db  03Bh, 005h, 04Ch, 0B8h, 000h, 000h
        jae    SHORT loc_5_D57B
        db  03Bh, 005h, 054h, 0B8h, 000h, 000h
        ja     SHORT loc_5_D56E
        db  0A1h, 054h, 0B8h, 000h, 000h
loc_5_D56E:
        db  03Bh, 005h, 04Ch, 0B8h, 000h, 000h
        jae    SHORT loc_5_D57B
        db  0A3h, 058h, 0B8h, 000h, 000h
loc_5_D57B:
        db  0A1h, 04Ch, 0B8h, 000h, 000h
        db  039h, 005h, 058h, 0B8h, 000h, 000h
        jae    SHORT loc_5_D5C1
        db  0A1h, 058h, 0B8h, 000h, 000h
        inc    eax
        db  03Bh, 005h, 04Ch, 0B8h, 000h, 000h
        jae    SHORT loc_5_D59B
        db  0E8h, 0A5h, 093h, 0FAh, 0FFh
loc_5_D59B:
        or     esi, esi
        je     SHORT loc_5_D5B6
        push   0ea60h
        db  068h, 004h, 097h, 000h, 000h
        db  0E8h, 096h, 06Ch, 0FAh, 0FFh
        add    esp, 8
        db  0A3h, 0C0h, 0B8h, 000h, 000h
loc_5_D5B6:
        db  0FEh, 00Dh, 0D1h, 0B1h, 000h, 000h
        db  0E8h, 0AFh, 0B6h, 0FAh, 0FFh
loc_5_D5C1:
        db  0FFh, 035h, 064h, 0B8h, 000h, 000h
        db  0E8h, 025h, 0BDh, 0FAh, 0FFh
        add    esp, 4
        or     esi, esi
        je     SHORT loc_5_D5EA
        push   0ea60h
        db  068h, 004h, 097h, 000h, 000h
        db  0E8h, 062h, 06Ch, 0FAh, 0FFh
        add    esp, 8
        db  0A3h, 0C0h, 0B8h, 000h, 000h
loc_5_D5EA:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
_PTEXT ends
        end
