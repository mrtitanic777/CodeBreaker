; ibmtok_lcode_01_02.asm -- IBMTOK object 1 segment _LTEXT  (part 2/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_5010:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0c3h]
        mov    ax, word ptr [ebp + 0ch]
        mov    byte ptr [ecx], 81h
        mov    byte ptr [ecx + 2], 0
        mov    word ptr [ecx + 4], 0
        mov    word ptr [ecx + 6], ax
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5040:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        lea    eax, [esi + 24h]
        mov    ecx, dword ptr [edx + 0efh]
        or     ecx, ecx
        jne    SHORT loc_1_5060
        mov    dword ptr [edx + 0efh], esi
        jmp    SHORT loc_1_5063
        db  090h
loc_1_5060:
        mov    dword ptr [ecx + 24h], esi
loc_1_5063:
        mov    dword ptr [edx + 0f3h], esi
        mov    dword ptr [eax], 0
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5080:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 5dh]
        cmp    byte ptr [esi + 15fh], 0
        je     SHORT loc_1_50BA
        cmp    dword ptr [esi + 160h], 0
        jne    SHORT loc_1_50AD
        mov    dword ptr [esi + 160h], 1
        push   esi
        call   sub_1_1CB0
loc_1_50AD:
        cmp    byte ptr [esi + 15fh], 0
        jne    NEAR PTR loc_1_51B4
loc_1_50BA:
        mov    edi, dword ptr [esi + 71h]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 71h
        je     NEAR PTR loc_1_51B4
        inc    dword ptr [edi + 14h]
        push   0
        mov    eax, dword ptr [edi + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 85h]
        push   eax
        db  0E8h, 0F1h, 0B0h, 0FEh, 0FFh
        or     eax, eax
        jne    SHORT loc_1_5150
        cmp    dword ptr [edi + 14h], 2
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        jne    SHORT loc_1_5137
        mov    eax, dword ptr [esi + 164h]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_5109
        push   esi
        call   sub_1_0E20
loc_1_5109:
        mov    eax, dword ptr [esi + 61h]
        sub    eax, esi
        cmp    eax, 61h
        jne    SHORT loc_1_511A
        mov    dword ptr [esi + 18h], 0feh
loc_1_511A:
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_0574
        push   0
        push   96h
        push   edi
        call   sub_1_063C
        jmp    NEAR PTR loc_1_51B4
loc_1_5137:
        lea    eax, [esi + 69h]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 6dh]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi + 6dh]
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 6dh], edi
        sub    dword ptr [edi + 14h], 2
        jmp    SHORT loc_1_51B4
loc_1_5150:
        cmp    eax, 103h
        jne    SHORT loc_1_5199
        cmp    byte ptr [esi + 0cbh], 0
        je     SHORT loc_1_516D
        mov    byte ptr [esi + 0cbh], 0
        push   esi
        call   sub_1_48C0
loc_1_516D:
        mov    byte ptr [edi + 1dh], 1
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [esi + 69h]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 6dh]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi + 6dh]
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 6dh], edi
        sub    dword ptr [edi + 14h], 2
        jmp    SHORT loc_1_51B4
loc_1_5199:
        push   eax
        push   0ah
        push   7
        push   3
        push   0aa000006h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 18h
loc_1_51B4:
        mov    edi, dword ptr [esi + 69h]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 69h
        je     SHORT loc_1_520F
        cmp    dword ptr [edi + 14h], 0
        jne    SHORT loc_1_520F
        mov    eax, dword ptr [esi + 164h]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_51D7
        push   esi
        call   sub_1_0E20
loc_1_51D7:
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_0574
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 61h]
        sub    eax, esi
        cmp    eax, 61h
        jne    SHORT loc_1_5202
        mov    dword ptr [esi + 18h], 0feh
loc_1_5202:
        push   0
        push   96h
        push   edi
        call   sub_1_063C
loc_1_520F:
        dec    dword ptr [esi + 5dh]
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_5220:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 89h], 0
        je     SHORT loc_1_52A9
loc_1_5235:
        mov    ecx, dword ptr [esi + 89h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 89h], eax
        or     eax, eax
        jne    SHORT loc_1_5251
        mov    dword ptr [esi + 8dh], 0
loc_1_5251:
        mov    eax, dword ptr [ecx + 10h]
        dec    eax
        cmp    eax, 9
        ja     SHORT loc_1_52A0
        db  0FFh, 024h, 085h, 0B8h, 052h, 000h, 000h
loc_1_5261:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   ecx
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_058C
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_52A0
loc_1_5274:
        push   edi
        push   ecx
        mov    eax, dword ptr [esi + 7dh]
        push   eax
        call   sub_1_05AA
        dec    dword ptr [esi + 5dh]
        jmp    SHORT loc_1_52A0
loc_1_5284:
        push   0
        push   14h
        push   ecx
        call   sub_1_063C
        mov    dword ptr [esi + 91h], 0
        jmp    SHORT loc_1_52A0
loc_1_529A:
        mov    eax, dword ptr [ecx + 4]
        dec    dword ptr [eax + 14h]
loc_1_52A0:
        cmp    dword ptr [esi + 89h], 0
        jne    SHORT loc_1_5235
loc_1_52A9:
        push   edi
        push   esi
        call   sub_1_52E0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h, 061h, 052h, 000h, 000h, 074h, 052h, 000h, 000h, 0A0h, 052h, 000h, 000h, 09Ah, 052h, 000h
        db  000h, 0A0h, 052h, 000h, 000h, 0A0h, 052h, 000h, 000h, 0A0h, 052h, 000h, 000h, 084h, 052h, 000h
        db  000h, 09Ah, 052h, 000h, 000h, 09Ah, 052h, 000h, 000h
sub_1_52E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0ceh]
        or     ecx, ecx
        je     SHORT loc_1_5314
        mov    dword ptr [esi + 0ceh], 0
        mov    edi, dword ptr [ecx + 28h]
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   ecx
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_057A
        dec    dword ptr [edi + 14h]
loc_1_5314:
        cmp    dword ptr [esi + 0e7h], 0
        je     SHORT loc_1_534F
        lea    ecx, [ecx]
loc_1_5320:
        mov    edx, dword ptr [esi + 0e7h]
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 0e7h], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_057A
        dec    dword ptr [edi + 14h]
        cmp    dword ptr [esi + 0e7h], 0
        jne    SHORT loc_1_5320
loc_1_534F:
        lea    edi, [esi + 179h]
        mov    dword ptr [ebp - 4], 80h
loc_1_535C:
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_1_538D
        mov    byte ptr [ecx + 30h], 0
        xor    eax, eax
        mov    al, byte ptr [ecx + 31h]
        mov    dword ptr [esi + eax*4 + 179h], 0
        mov    ebx, dword ptr [ecx + 28h]
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ecx + 2ch]
        push   eax
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_057A
        dec    dword ptr [ebx + 14h]
loc_1_538D:
        add    edi, 4
        dec    dword ptr [ebp - 4]
        jne    SHORT loc_1_535C
        mov    dword ptr [esi + 0efh], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_53B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 2ch], 0
        je     SHORT loc_1_5421
        cmp    dword ptr [esi + 0ceh], 0
        jne    SHORT loc_1_53D8
        cmp    dword ptr [esi + 0e7h], 0
        jne    SHORT loc_1_53D8
        cmp    dword ptr [esi + 89h], 0
        je     SHORT loc_1_5421
loc_1_53D8:
        mov    byte ptr [esi + 171h], 1
        mov    byte ptr [esi + 2ch], 0
        push   0
        push   esi
        call   sub_1_5220
        mov    al, byte ptr [esi + 2dh]
        cmp    al, 0ah
        jae    SHORT loc_1_541B
        inc    al
        mov    byte ptr [esi + 2dh], al
        push   0
        push   0aa000003h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 0ch
        push   0
        push   esi
        call   sub_1_1EF0
        push   esi
        call   sub_1_5080
loc_1_541B:
        mov    byte ptr [esi + 2ch], 0
        jmp    SHORT loc_1_5440
loc_1_5421:
        cmp    dword ptr [esi + 0ceh], 0
        jne    SHORT loc_1_543C
        cmp    dword ptr [esi + 0e7h], 0
        jne    SHORT loc_1_543C
        cmp    dword ptr [esi + 89h], 0
        je     SHORT loc_1_5440
loc_1_543C:
        mov    byte ptr [esi + 2ch], 1
loc_1_5440:
        cmp    byte ptr [esi + 177h], 0
        je     SHORT loc_1_5467
        cmp    byte ptr [esi + 176h], 0
        jne    SHORT loc_1_5467
        mov    byte ptr [esi + 176h], 1
        push   0
        push   esi
        call   sub_1_1EF0
        push   esi
        call   sub_1_5080
loc_1_5467:
        push   7530h
        add    esi, 20h
        push   esi
        call   sub_1_0612
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_5480:
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
        db  002h
loc_1_5504:
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
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
loc_1_5560:
        dec    edi
        inc    ebx
        dec    ecx
        push   eax
loc_1_5564:
        xor    byte ptr [ecx], dh
        xor    dword ptr [esi], esi
loc_1_5568:
        and    byte ptr [eax], dh
        cmp    dword ptr [ecx], edi
        push   ebx
        push   edx
        inc    ecx
        dec    ebp
        xor    al, 58h
        xor    esi, dword ptr [esi]
        and    byte ptr [eax], bh
        db  031h, 035h
loc_1_5578:
        dec    edi
        inc    ebx
        dec    ecx
        push   eax
loc_1_557C:
        xor    byte ptr [ecx], dh
        xor    dword ptr [esi], esi
loc_1_5580:
        and    byte ptr [eax], dh
        cmp    dword ptr [ecx], edi
loc_1_5584:
        push   ebx
        push   edx
        inc    ecx
        dec    ebp
loc_1_5588:
        xor    al, 58h
        xor    esi, dword ptr [esi]
loc_1_558C:
        and    byte ptr [eax], bh
        xor    dword ptr [546d6249h], esi
        outsd
        imul   eax, dword ptr [eax], 0
loc_1_5598:
        dec    ecx
        bound  ebp, qword ptr [ebp + 54h]
        outsd
        imul   eax, dword ptr [eax], 0
loc_1_55A0:
        dec    ecx
        bound  ebp, qword ptr [ebp + 20h]
        push   esp
        outsd
        imul   esp, dword ptr [ebp + 6eh], 20h
        push   edx
        imul   ebp, dword ptr [esi + 67h], 74654e20h
        ja     SHORT loc_1_5623
        db  072h, 06Bh
        and    byte ptr [ebx + 61h], al
        db  072h, 064h
        and    byte ptr [esi + 6fh], ah
        jb     SHORT loc_1_55E0
        push   eax
        inc    ebx
        and    byte ptr [ecx + 2fh], cl
        dec    edi
        and    byte ptr [edx + 75h], ah
        jae    SHORT loc_1_55F9
        add    byte ptr [ecx + 62h], cl
        insd
        and    byte ptr [edi + ebp*2 + 6bh], dl
        outsb
        and    byte ptr [edx + 69h], dl
        outsb
        and    byte ptr [bp + 65h], cl
        db  074h, 077h
        outsd
loc_1_55E0:
        jb     SHORT loc_1_564D
        and    byte ptr [ebx + 61h], al
        jb     SHORT loc_1_564B
        and    byte ptr [esi + 6fh], ah
        jb     SHORT loc_1_560C
        dec    ebp
        inc    ebx
        inc    ecx
        and    byte ptr [edx + 75h], ah
        jae    SHORT loc_1_5622
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ecx
loc_1_55F9:
        popad
        jo     SHORT loc_1_5671
        jb     NEAR PTR loc_1_5654
        jns    SHORT loc_1_5672
        add    byte ptr gs:[ecx + 64h], al
        popad
        jo     SHORT loc_1_567D
        jb     NEAR PTR loc_1_5660
loc_1_560C:
        jns    SHORT loc_1_567E
        add    byte ptr gs:[edx + 75h], al
        db  073h, 054h
        jns    SHORT loc_1_5686
        add    byte ptr gs:[edx + 75h], al
        jae    SHORT loc_1_5670
        db  079h, 070h
        add    byte ptr gs:[ecx + 4fh], cl
loc_1_5622:
        inc    edx
loc_1_5623:
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
loc_1_5628:
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        dec    esi
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
        dec    esi
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
        dec    ebp
        inc    ecx
        pop    eax
loc_1_564B:
        push   eax
        inc    ecx
loc_1_564D:
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        push   ebx
        dec    ecx
        pop    edx
loc_1_5654:
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebp + 41h], cl
        pop    eax
        push   eax
        inc    ecx
        inc    ebx
        dec    ebx
        inc    ebp
loc_1_5660:
        push   esp
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebp + 4dh], cl
        dec    ecx
        dec    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5670:
        dec    ebp
loc_1_5671:
        dec    ebp
loc_1_5672:
        dec    ecx
        dec    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   edx
        dec    ecx
        dec    esi
        inc    edi
        push   ebx
loc_1_567D:
        push   eax
loc_1_567E:
        inc    ebp
        inc    ebp
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [edx + 49h], dl
loc_1_5686:
        dec    esi
        inc    edi
        push   ebx
        push   eax
        inc    ebp
        inc    ebp
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [edx + 41h], dl
        dec    ebp
        add    byte ptr [edx + 41h], dl
        dec    ebp
        add    byte ptr [edx + 41h], dl
        dec    ebp
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [edx + 41h], dl
        dec    ebp
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56C5:
        add    byte ptr [eax], al
loc_1_56C7:
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
_LTEXT ends
        end
