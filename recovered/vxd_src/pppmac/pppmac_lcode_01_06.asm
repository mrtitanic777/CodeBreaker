; pppmac_lcode_01_06.asm -- PPPMAC object 1 segment _LTEXT  (part 6/6)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_17EB0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    cl, byte ptr [esi + 11h]
        cmp    cl, 7
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 0feh]
        jne    SHORT loc_1_17EF0
        mov    byte ptr [esi + 10h], 6
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        mov    byte ptr [esi + 11h], 5
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_17EF0:
        cmp    cl, 6
        jne    SHORT loc_1_17F3A
        sub    edi, edi
        sub    cl, cl
        sub    dl, dl
        cmp    byte ptr [esi + 10h], 9
        jne    SHORT loc_1_17F0A
        mov    byte ptr [esi + 10h], 6
        mov    edi, 1
loc_1_17F0A:
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_17F11
        mov    cl, 1
loc_1_17F11:
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [eax + 10h], 0
        je     SHORT loc_1_17F1C
        mov    dl, 1
loc_1_17F1C:
        or     edi, edi
        jne    SHORT loc_1_17F24
        cmp    dl, cl
        je     SHORT loc_1_17F3A
loc_1_17F24:
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        mov    byte ptr [esi + 11h], 8
loc_1_17F3A:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_17F40:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    ebx, [esi + 4bh]
        mov    eax, dword ptr [esi + 4]
        mov    edi, dword ptr [eax + 0ch]
        mov    byte ptr [ebp - 1], 2
        mov    ecx, dword ptr [edi + 0feh]
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [edi + 0d8h]
        or     eax, eax
        mov    cl, byte ptr [eax + 14h]
        mov    byte ptr [ebp - 2], cl
        mov    dword ptr [edi + 0d0h], 801h
        mov    dword ptr [edi + 0d4h], 0
        mov    dword ptr [edi + 0dch], 0
        mov    dword ptr [edi + 0e0h], 0
        je     SHORT loc_1_17FB0
        push   0
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        push   eax
        call   sub_1_075C
        mov    dword ptr [edi + 0d8h], 0
loc_1_17FB0:
        mov    ecx, dword ptr [ebp + 0ch]
        push   5
        mov    dword ptr [ecx + 10h], 801h
        push   edi
        call   PPPMAC_Service_02
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_1809A
        mov    edx, dword ptr [eax + 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        add    edx, dword ptr [eax + 20h]
        cmp    dword ptr [ecx + 18h], 0
        je     SHORT loc_1_18012
        mov    ecx, dword ptr [ebp - 8]
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
        jmp    SHORT loc_1_1801C
loc_1_18012:
        lea    edi, [esi + 11h]
        mov    byte ptr [ebp - 1], 3
        mov    byte ptr [edi], 7
loc_1_1801C:
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [edx], cl
        mov    cl, byte ptr [ebp - 2]
        mov    byte ptr [edx + 1], cl
        mov    word ptr [edx + 2], 500h
        mov    byte ptr [edx + 4], 0
        mov    dword ptr [esi + 3bh], eax
        mov    dword ptr [esi + 35h], edx
        mov    word ptr [esi + 39h], 5
        push   esi
        call   sub_1_17EB0
        cmp    byte ptr [edi], 5
        je     SHORT loc_1_18063
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_18063:
        mov    eax, dword ptr [ebx]
        dec    eax
        mov    dword ptr [ebx], eax
        jne    SHORT loc_1_18085
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_18085:
        push   0ea60h
        lea    eax, [ebx + 4]
        push   eax
        call   sub_1_0732
        mov    dword ptr [ebx + 10h], 1
loc_1_1809A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_180B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 5bh], 0
        lea    eax, [esi + 4bh]
        je     SHORT loc_1_180E1
        mov    dword ptr [eax + 10h], 0
        mov    byte ptr [esi + 10h], 6
        mov    byte ptr [esi + 11h], 5
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
loc_1_180E1:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_180F0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0ch]
        lea    edx, [ecx + 4bh]
        mov    eax, dword ptr [ecx + 4]
        cmp    dword ptr [edx + 20h], 0
        mov    eax, dword ptr [eax + 0ch]
        je     SHORT loc_1_18150
        mov    dword ptr [edx + 20h], 0
        mov    dl, byte ptr [ecx + 10h]
        cmp    dl, 6
        jb     SHORT loc_1_18150
        cmp    dl, 8
        ja     SHORT loc_1_18150
        mov    dword ptr [eax + 0d0h], 800h
        mov    dword ptr [eax + 0d4h], 901h
        mov    dword ptr [eax + 0d8h], 0
        mov    dword ptr [eax + 0dch], ecx
        db  0C7h, 080h, 0E0h, 000h, 000h, 000h, 060h, 081h, 001h, 000h
        push   eax
        call   PPPMAC_Service_09
        add    esp, 4
loc_1_18150:
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_18160:
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
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_181C0:
        cmp    al, 41h
        insd
        imul   ebp, dword ptr [esi + 69h], 61727473h
        je     SHORT loc_1_1823C
        db  072h, 020h
        push   ebx
        db  070h, 065h
        arpl   word ptr [ecx + 66h], bp
        imul   esp, dword ptr [ebp + 64h], 3eh
loc_1_181DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_181E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1823C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_18248:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_1824C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_18254:
        adc    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
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
        add    byte ptr [10000101h], cl
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
        add    dword ptr [ecx], eax
        add    dword ptr [edx + eax], eax
        add    dword ptr [ecx], eax
        add    al, 3
        add    dword ptr [ecx], eax
        add    al, 5
        add    dword ptr [ecx], eax
        add    al, 4
        add    dword ptr [ecx], eax
        add    al, 6
        add    dword ptr [ecx], eax
        db  004h
loc_1_182E0:
        adc    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
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
        add    byte ptr [10000101h], cl
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
        add    dword ptr [ecx], eax
        add    dword ptr [edx + eax], eax
        add    dword ptr [ecx], eax
        add    al, 3
        add    dword ptr [ecx], eax
        add    al, 5
        add    dword ptr [ecx], eax
        add    al, 4
        add    dword ptr [ecx], eax
        add    al, 6
        add    dword ptr [ecx], eax
        add    al, 50h
        push   eax
        push   eax
        dec    ebp
        popad
        arpl   word ptr [eax], ax
        db  000h
loc_1_18374:
        push   eax
        push   eax
        push   eax
        dec    ebp
        popad
        arpl   word ptr [eax], ax
        add    byte ptr [ebp + 61h], cl
        db  078h, 046h
        jb     SHORT loc_1_183E3
        insd
        push   ebx
        imul   edi, dword ptr [edx + 65h], 0
loc_1_1838C:
        dec    ebp
        popad
        db  078h, 046h
        jb     SHORT loc_1_183F3
        insd
        push   ebx
        imul   edi, dword ptr [edx + 65h], 0
        inc    ebx
        outsd
        insd
        db  070h, 072h
        jae    NEAR PTR loc_1_18417
        push   ebx
        outsb
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
loc_1_183AC:
        inc    ebx
        outsd
        insd
        db  070h, 072h
        jae    NEAR PTR loc_1_18427
        push   ebx
        outsb
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        inc    ebx
        outsd
        insd
        db  070h, 072h
        jae    NEAR PTR loc_1_18437
        push   edx
        arpl   word ptr gs:[esi], si
        add    byte ptr [eax], al
        add    byte ptr [ebx + 6fh], al
        insd
        db  070h, 072h
        jae    NEAR PTR loc_1_18447
        push   edx
        arpl   word ptr gs:[esi], si
        add    byte ptr [eax], al
        add    byte ptr [esi + 72h], al
        popad
        insd
        db  065h, 073h, 050h
loc_1_183E3:
        jb     NEAR PTR loc_1_18436
        outsd
        jb     SHORT loc_1_1845D
        add    byte ptr [eax], al
        add    byte ptr [esi + 72h], al
        popad
        insd
        db  065h, 073h, 050h
loc_1_183F3:
        db  065h, 072h, 050h
        outsd
        db  072h, 074h
        add    byte ptr [eax], al
        add    byte ptr [eax + 6fh], bl
        outsb
        pop    eax
        outsd
        add    byte ptr [eax + 6fh], bl
        outsb
        pop    eax
        outsd
        add    byte ptr [ebx + 6fh], al
        insd
        db  070h, 072h
        jae    NEAR PTR loc_1_18487
        inc    edx
        inc    ebx
        popad
loc_1_18417:
        jae    SHORT loc_1_1848D
        add    byte ptr [eax], al
        add    byte ptr [ebx + 6fh], al
        insd
        db  070h, 072h
        jae    NEAR PTR loc_1_18497
        inc    edx
        inc    ebx
        popad
loc_1_18427:
        db  073h, 074h
        add    byte ptr [eax], al
        add    byte ptr [esp + ecx*2 + 6fh], dl
        db  067h, 069h, 06Eh, 000h, 000h
loc_1_18434:
        push   esp
        dec    esp
loc_1_18436:
        outsd
loc_1_18437:
        imul   ebp, dword ptr [bp], 676f4c00h
        imul   ebp, dword ptr [bp + 67h], 676f4c00h
loc_1_18447:
        imul   ebp, dword ptr [bp + 67h], 58504900h
        dec    eax
        popad
        jb     NEAR PTR loc_1_18499
        outsd
        insd
        jo     SHORT loc_1_184CC
        db  065h, 073h, 073h
loc_1_1845D:
        imul   ebp, dword ptr [edi + 6eh], 0
loc_1_18464:
        dec    ecx
        push   eax
        pop    eax
        dec    eax
        popad
        jb     NEAR PTR loc_1_184B1
        outsd
        insd
        db  070h, 072h
        jae    NEAR PTR loc_1_184E8
        imul   ebp, dword ptr [edi + 6eh], 0
loc_1_1847C:
        inc    esp
        inc    ebp
        push   ebx
        push   esp
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
loc_1_18484:
        push   eax
        push   eax
        push   eax
loc_1_18487:
        and    byte ptr [ecx + 64h], al
        popad
        jo     SHORT loc_1_18501
loc_1_1848D:
        jb     NEAR PTR loc_1_184BE
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_18494:
        push   ebx
        db  079h, 073h
loc_1_18497:
        db  074h, 065h
loc_1_18499:
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        db  072h, 065h
        outsb
        je     SHORT loc_1_184E6
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        je     NEAR PTR loc_1_18509
        push   ebx
        jb     NEAR PTR loc_1_18527
loc_1_184B1:
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
        pop    esp
        push   eax
        push   eax
        push   eax
        pop    esp
loc_1_184BE:
        inc    ebx
        push   eax
        dec    esp
        imul   esi, dword ptr [ebx + 74h], 0
loc_1_184C8:
        db  070h, 070h
        jo     SHORT loc_1_18538
loc_1_184CC:
        outsd
        je     NEAR PTR loc_1_18549
        je     SHORT loc_1_184D3
loc_1_184D3:
        add    byte ptr [eax], ah
        push   ebx
        push   edx
        inc    ebx
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        and    byte ptr [ebp + eax*2 + 53h], al
        push   esp
        and    byte ptr [eax], al
        add    byte ptr [eax], ah
        push   ebx
loc_1_184E6:
        push   edx
        inc    ebx
loc_1_184E8:
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_1_184F4:
        add    byte ptr [eax], al
        and    dword ptr [eax], edx
        inc    edx
loc_1_184F9:
        and    byte ptr [ebx + 30h], ah
        test   byte ptr [eax - 5bh], al
        push   eax
        db  0C6h
loc_1_18501:
        pushad
loc_1_18502:
        out    70h, eax
        db  008h, 081h, 029h, 091h, 04Ah
loc_1_18509:
        mov    eax, dword ptr [0c18cb16bh]
        lodsd
        ror    esi, 1
        loope  loc_1_18502
        int1
        xor    dword ptr [edx], edx
        adc    byte ptr [edx], al
        jae    SHORT loc_1_1854C
        push   edx
        and    dh, byte ptr [ebp - 8bd6baeh]
        jb     SHORT loc_1_184F9
        bound  edi, qword ptr [ecx]
        xchg   ebx, eax
        db  018h
loc_1_18527:
        cmp    dword ptr [ebx - 4dh], 5ah
        mov    dword ptr [0c39cd3bdh], eax
        push   ebx
        fsubrp st(3)
        bound  esp, qword ptr [ebx + eax*2]
        db  034h
loc_1_18538:
        and    byte ptr [ecx + eax], al
        adc    al, 0e6h
        db  064h, 0C7h, 074h, 0A4h, 044h, 085h, 054h, 06Ah, 0A5h, 04Bh, 0B5h, 028h
loc_1_18549:
        test   dword ptr [ecx], ecx
        xchg   ebp, eax
loc_1_1854C:
        out    dx, al
        in     eax, 0cfh
        cmc
        lodsb
        lds    ecx, ptr [ebp + 723653d5h]
        adc    dword ptr es:[esi], edx
        xor    byte ptr [esi], al
        xlatb
        db  076h, 0F6h
        xchg   bp, ax
        push   esi
        mov    ah, 46h
        pop    ebx
        mov    bh, 7ah
        cmpsd
        sbb    dword ptr [edi - 82078c8h], edx
        db  0FEh, 0E7h, 09Dh, 0D7h, 0BCh, 0C7h, 0C4h, 048h, 0E5h, 058h, 086h, 068h, 0A7h, 078h, 040h, 008h
        db  061h, 018h, 002h, 028h, 023h, 038h, 0CCh, 0C9h, 0EDh, 0D9h, 08Eh, 0E9h, 0AFh, 0F9h, 048h, 089h
        db  069h, 099h, 00Ah, 0A9h, 02Bh, 0B9h, 0F5h, 05Ah, 0D4h, 04Ah, 0B7h, 07Ah, 096h, 06Ah, 071h, 01Ah
        db  050h, 00Ah, 033h, 03Ah, 012h, 02Ah, 0FDh, 0DBh, 0DCh, 0CBh, 0BFh, 0FBh, 09Eh, 0EBh, 079h, 09Bh
        db  058h, 08Bh, 03Bh, 0BBh, 01Ah, 0ABh, 0A6h, 06Ch, 087h, 07Ch, 0E4h, 04Ch, 0C5h, 05Ch, 022h, 02Ch
        db  003h, 03Ch, 060h, 00Ch, 041h, 01Ch, 0AEh, 0EDh, 08Fh, 0FDh, 0ECh, 0CDh, 0CDh, 0DDh, 02Ah, 0ADh
        db  00Bh, 0BDh, 068h, 08Dh, 049h, 09Dh, 097h, 07Eh, 0B6h, 06Eh, 0D5h, 05Eh, 0F4h, 04Eh, 013h, 03Eh
        db  032h, 02Eh, 051h, 01Eh, 070h, 00Eh, 09Fh, 0FFh, 0BEh, 0EFh, 0DDh, 0DFh, 0FCh, 0CFh, 01Bh, 0BFh
        db  03Ah, 0AFh, 059h, 09Fh, 078h, 08Fh, 088h, 091h, 0A9h, 081h, 0CAh, 0B1h, 0EBh, 0A1h, 00Ch, 0D1h
        db  02Dh, 0C1h, 04Eh, 0F1h, 06Fh, 0E1h, 080h, 010h, 0A1h, 000h, 0C2h, 030h, 0E3h, 020h, 004h, 050h
        db  025h, 040h, 046h, 070h, 067h, 060h, 0B9h, 083h, 098h, 093h, 0FBh, 0A3h, 0DAh, 0B3h, 03Dh, 0C3h
        db  01Ch, 0D3h, 07Fh, 0E3h, 05Eh, 0F3h, 0B1h, 002h, 090h, 012h, 0F3h, 022h, 0D2h, 032h, 035h, 042h
        db  014h, 052h, 077h, 062h, 056h, 072h, 0EAh, 0B5h, 0CBh, 0A5h, 0A8h, 095h, 089h, 085h, 06Eh, 0F5h
        db  04Fh, 0E5h, 02Ch, 0D5h, 00Dh, 0C5h, 0E2h, 034h, 0C3h, 024h, 0A0h, 014h, 081h, 004h, 066h, 074h
        db  047h, 064h, 024h, 054h, 005h, 044h, 0DBh, 0A7h, 0FAh, 0B7h, 099h, 087h, 0B8h, 097h, 05Fh, 0E7h
        db  07Eh, 0F7h, 01Dh, 0C7h, 03Ch, 0D7h, 0D3h, 026h, 0F2h, 036h, 091h, 006h, 0B0h, 016h, 057h, 066h
        db  076h, 076h, 015h, 046h, 034h, 056h, 04Ch, 0D9h, 06Dh, 0C9h, 00Eh, 0F9h, 02Fh, 0E9h, 0C8h, 099h
        db  0E9h, 089h, 08Ah, 0B9h, 0ABh, 0A9h, 044h, 058h, 065h, 048h, 006h, 078h, 027h, 068h, 0C0h, 018h
        db  0E1h, 008h, 082h, 038h, 0A3h, 028h, 07Dh, 0CBh, 05Ch, 0DBh, 03Fh, 0EBh, 01Eh, 0FBh, 0F9h, 08Bh
        db  0D8h, 09Bh, 0BBh, 0ABh, 09Ah, 0BBh, 075h, 04Ah, 054h, 05Ah, 037h, 06Ah, 016h, 07Ah, 0F1h, 00Ah
        db  0D0h, 01Ah, 0B3h, 02Ah, 092h, 03Ah, 02Eh, 0FDh, 00Fh, 0EDh, 06Ch, 0DDh, 04Dh, 0CDh, 0AAh, 0BDh
        db  08Bh, 0ADh, 0E8h, 09Dh, 0C9h, 08Dh, 026h, 07Ch, 007h, 06Ch, 064h, 05Ch, 045h, 04Ch, 0A2h, 03Ch
        db  083h, 02Ch, 0E0h, 01Ch, 0C1h, 00Ch, 01Fh, 0EFh, 03Eh, 0FFh, 05Dh, 0CFh, 07Ch, 0DFh, 09Bh, 0AFh
        db  0BAh, 0BFh, 0D9h, 08Fh, 0F8h, 09Fh, 017h, 06Eh, 036h, 07Eh, 055h, 04Eh, 074h, 05Eh, 093h, 02Eh
        db  0B2h, 03Eh, 0D1h, 00Eh, 0F0h, 01Eh
loc_1_186F4:
        inc    esp
        dec    ecx
        inc    ecx
        dec    esp
        dec    ecx
        dec    esi
        pop    edi
        inc    edi
        inc    ecx
        push   esp
        inc    ebp
        push   edi
        inc    ecx
        pop    ecx
        and    byte ptr [eax], ah
        inc    esp
        dec    ecx
        inc    ecx
        dec    esp
        dec    ecx
        dec    esi
        pop    edi
        inc    ebx
        dec    esp
        dec    ecx
        inc    ebp
        dec    esi
        push   esp
        add    byte ptr [eax], al
        db  000h, 0F4h
        xchg   byte ptr [ecx], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [ecx - 64dcedefh], cl
loc_1_18727:
        xor    ah, byte ptr [esi + eax*2]
        lodsd
        push   edi
        mov    edi, 0c18c4874h
        popfd
        pop    edx
        scasd
        sar    dword ptr [esi - 241a3594h], cl
        jle    SHORT loc_1_18727
        idiv   eax
        adc    dword ptr [eax], 33930108h
        sbb    ah, byte ptr [edx]
        movsd
        push   esi
        sub    al, 47h
        mov    bh, 75h
        leave
        pushfd
        inc    eax
        db  08Dh, 0DBh, 0BFh, 052h, 0AEh, 0EDh, 0DAh, 064h, 0CBh, 0FFh, 0F9h, 076h, 0E8h, 002h, 021h, 08Bh
        db  030h, 010h, 002h, 099h, 013h, 026h, 067h, 0AFh, 076h, 034h, 044h, 0BDh, 055h, 04Ah, 0ADh, 0C3h
        db  0BCh, 058h, 08Eh, 0D1h, 09Fh, 06Eh, 0EBh, 0E7h, 0FAh, 07Ch, 0C8h, 0F5h, 0D9h, 083h, 031h, 00Ah
        db  020h, 091h, 012h, 018h, 003h, 0A7h, 077h, 02Eh, 066h, 0B5h, 054h, 03Ch, 045h, 0CBh, 0BDh, 042h
        db  0ACh, 0D9h, 09Eh, 050h, 08Fh, 0EFh, 0FBh, 066h, 0EAh, 0FDh, 0D8h, 074h, 0C9h, 004h, 042h, 08Dh
        db  053h, 016h, 061h, 09Fh, 070h, 020h, 004h, 0A9h, 015h, 032h, 027h, 0BBh, 036h, 04Ch, 0CEh, 0C5h
        db  0DFh, 05Eh, 0EDh, 0D7h, 0FCh, 068h, 088h, 0E1h, 099h, 07Ah, 0ABh, 0F3h, 0BAh, 085h, 052h, 00Ch
        db  043h, 097h, 071h, 01Eh, 060h, 0A1h, 014h, 028h, 005h, 0B3h, 037h, 03Ah, 026h, 0CDh, 0DEh, 044h
        db  0CFh, 0DFh, 0FDh, 056h, 0ECh, 0E9h, 098h, 060h, 089h, 0FBh, 0BBh, 072h, 0AAh, 006h, 063h, 08Fh
        db  072h, 014h, 040h, 09Dh, 051h, 022h, 025h, 0ABh, 034h, 030h, 006h, 0B9h, 017h, 04Eh, 0EFh, 0C7h
        db  0FEh, 05Ch, 0CCh, 0D5h, 0DDh, 06Ah, 0A9h, 0E3h, 0B8h, 078h, 08Ah, 0F1h, 09Bh, 087h, 073h, 00Eh
        db  062h, 095h, 050h, 01Ch, 041h, 0A3h, 035h, 02Ah, 024h, 0B1h, 016h, 038h, 007h, 0CFh, 0FFh, 046h
        db  0EEh, 0DDh, 0DCh, 054h, 0CDh, 0EBh, 0B9h, 062h, 0A8h, 0F9h, 09Ah, 070h, 08Bh, 008h, 084h, 081h
        db  095h, 01Ah, 0A7h, 093h, 0B6h, 02Ch, 0C2h, 0A5h, 0D3h, 03Eh, 0E1h, 0B7h, 0F0h, 040h, 008h, 0C9h
        db  019h, 052h, 02Bh, 0DBh, 03Ah, 064h, 04Eh, 0EDh, 05Fh, 076h, 06Dh, 0FFh, 07Ch, 089h, 094h, 000h
        db  085h, 09Bh, 0B7h, 012h, 0A6h, 0ADh, 0D2h, 024h, 0C3h, 0BFh, 0F1h, 036h, 0E0h, 0C1h, 018h, 048h
        db  009h, 0D3h, 03Bh, 05Ah, 02Ah, 0E5h, 05Eh, 06Ch, 04Fh, 0F7h, 07Dh, 07Eh, 06Ch, 00Ah, 0A5h, 083h
        db  0B4h, 018h, 086h, 091h, 097h, 02Eh, 0E3h, 0A7h, 0F2h, 03Ch, 0C0h, 0B5h, 0D1h, 042h, 029h, 0CBh
        db  038h, 050h, 00Ah, 0D9h, 01Bh, 066h, 06Fh, 0EFh, 07Eh, 074h, 04Ch, 0FDh, 05Dh, 08Bh, 0B5h, 002h
        db  0A4h, 099h, 096h, 010h, 087h, 0AFh, 0F3h, 026h, 0E2h, 0BDh, 0D0h, 034h, 0C1h, 0C3h, 039h, 04Ah
        db  028h, 0D1h, 01Ah, 058h, 00Bh, 0E7h, 07Fh, 06Eh, 06Eh, 0F5h, 05Ch, 07Ch, 04Dh, 00Ch, 0C6h, 085h
        db  0D7h, 01Eh, 0E5h, 097h, 0F4h, 028h, 080h, 0A1h, 091h, 03Ah, 0A3h, 0B3h, 0B2h, 044h, 04Ah, 0CDh
        db  05Bh, 056h, 069h, 0DFh, 078h, 060h, 00Ch, 0E9h, 01Dh, 072h, 02Fh, 0FBh, 03Eh, 08Dh, 0D6h, 004h
        db  0C7h, 09Fh, 0F5h, 016h, 0E4h, 0A9h, 090h, 020h, 081h, 0BBh, 0B3h, 032h, 0A2h, 0C5h, 05Ah, 04Ch
        db  04Bh, 0D7h, 079h, 05Eh, 068h, 0E1h, 01Ch, 068h, 00Dh, 0F3h, 03Fh, 07Ah, 02Eh, 00Eh, 0E7h, 087h
        db  0F6h, 01Ch, 0C4h, 095h, 0D5h, 02Ah, 0A1h, 0A3h, 0B0h, 038h, 082h, 0B1h, 093h, 046h, 06Bh, 0CFh
        db  07Ah, 054h, 048h, 0DDh, 059h, 062h, 02Dh, 0EBh, 03Ch, 070h, 00Eh, 0F9h, 01Fh, 08Fh, 0F7h, 006h
        db  0E6h, 09Dh, 0D4h, 014h, 0C5h, 0ABh, 0B1h, 022h, 0A0h, 0B9h, 092h, 030h, 083h, 0C7h, 07Bh, 04Eh
        db  06Ah, 0D5h, 058h, 05Ch, 049h, 0E3h, 03Dh, 06Ah, 02Ch, 0F1h, 01Eh, 078h, 00Fh, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1892C:
        shr    byte ptr [eax], 1
        db  000h, 0E0h
        db  068h, 000h, 000h, 0F0h, 06Ch
        add    byte ptr [eax], al
        nop
        outsb
        add    byte ptr [eax], al
        add    byte ptr [edi], ch
        db  000h, 0A0h, 071h, 000h, 000h, 0D0h
        jno    SHORT loc_1_18947
loc_1_18947:
        db  000h, 000h
        jb     SHORT loc_1_1894B
loc_1_1894B:
        db  000h, 0D0h
        jb     SHORT loc_1_1894F
loc_1_1894F:
        db  000h, 0D0h
        jb     SHORT loc_1_18953
loc_1_18953:
        db  000h, 020h
        jae    SHORT loc_1_18957
loc_1_18957:
        db  000h, 0B0h, 073h, 000h, 000h, 000h
        je     SHORT loc_1_1895F
loc_1_1895F:
        db  000h, 090h, 074h, 000h, 000h, 003h
        inc    dword ptr [eax]
        add    al, byte ptr [6ff07h]
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    eax, 6000000h
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    edi, edi
        add    byte ptr [ecx], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [ecx], al
        inc    dword ptr [eax]
        add    byte ptr [ecx], al
        inc    dword ptr [eax]
        add    byte ptr [ecx], al
        inc    dword ptr [eax]
        add    byte ptr [ecx], al
        add    bh, bh
        add    byte ptr [ecx], al
        add    edi, edi
        add    byte ptr [ecx], al
        inc    dword ptr [eax]
        add    byte ptr [ecx], al
        add    eax, 0ff06ff07h
        add    byte ptr [eax], al
        add    edi, edi
        add    byte ptr [eax], al
        add    eax, 50000ffh
        inc    dword ptr [eax]
        add    byte ptr [esi], al
        inc    dword ptr [eax]
        add    byte ptr [edi], al
        inc    dword ptr [eax]
        add    byte ptr [eax], cl
        inc    dword ptr [eax]
        add    byte ptr [ecx], cl
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [edx], al
        inc    dword ptr [eax]
        add    byte ptr [edx], al
        inc    dword ptr [eax]
        add    byte ptr [edi + edi*8], al
        add    byte ptr [eax], al
        add    eax, 4ff0a05h
        add    eax, 504ff0ah
        or     bh, bh
        add    al, 2
        add    eax, 40ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        or     bh, bh
        add    byte ptr [edx + ecx], al
        inc    dword ptr [eax]
        add    eax, 600ff07h
        pop    es
        inc    dword ptr [eax]
        push   es
        pop    es
        inc    dword ptr [eax]
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    eax, dword ptr [edi + edi*8]
        add    byte ptr [edx], al
        add    al, 0ffh
        add    byte ptr [ebx], al
        add    al, 0ffh
        add    byte ptr [ebx], al
        add    al, 0ffh
        add    byte ptr [ebx], al
        add    al, 0ffh
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        or     edi, edi
        add    byte ptr [edx], al
        add    eax, 0ff080807h
        add    byte ptr [eax], al
        add    al, 0ffh
        add    byte ptr [eax], al
        add    eax, 800ff08h
        or     byte ptr [ecx], al
        dec    dword ptr [ecx]
        or     bh, bh
        add    byte ptr [eax], cl
        add    al, byte ptr [edi]
        or     byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        or     edi, edi
        add    byte ptr [edx], al
        add    eax, 0ff060807h
        add    byte ptr [eax], al
        add    al, 0ffh
        add    byte ptr [eax], al
        add    eax, 600ff09h
        or     edi, edi
        add    byte ptr [edi], al
        or     edi, edi
        add    byte ptr [esi], al
        add    al, byte ptr [edi]
        or     dword ptr [esi], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        or     edi, edi
        add    byte ptr [edx], al
        or     edi, edi
        add    byte ptr [ebx], al
        inc    dword ptr [eax]
        add    byte ptr [edi + edi*8], al
        add    byte ptr [eax], al
        add    eax, 700ff05h
        pop    es
        inc    dword ptr [eax]
        push   es
        add    eax, 209ff01h
        pop    es
        inc    dword ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        or     edi, edi
        add    byte ptr [edx], al
        or     edi, edi
        add    byte ptr [ebx], al
        inc    dword ptr [eax]
        add    byte ptr [edi + edi*8], al
        add    byte ptr [eax], al
        add    eax, 6ff0705h
        pop    es
        inc    dword ptr [eax]
        push   es
        add    eax, 208ff07h
        pop    es
        inc    dword ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        or     edi, edi
        add    byte ptr [edx], al
        or     edi, edi
        add    byte ptr [ebx], al
        or     edi, edi
        add    byte ptr [ebx + ecx], al
        inc    dword ptr [eax]
        add    eax, 600ff0bh
        or     edi, edi
        add    byte ptr [esi], al
        or     edi, edi
        add    byte ptr [esi], al
        add    al, byte ptr [esi]
        or     eax, dword ptr [0]
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        inc    dword ptr [eax]
        add    byte ptr [edx], al
        inc    dword ptr [eax]
        add    byte ptr [ebx], al
        add    al, 0ffh
        add    byte ptr [edx], al
        add    al, 0ffh
        add    byte ptr [ebx], al
        inc    dword ptr [eax]
        add    byte ptr [esi], al
        inc    dword ptr [eax]
        add    byte ptr [esi], al
        inc    dword ptr [eax]
        add    byte ptr [eax], cl
        add    al, byte ptr [edi]
        inc    dword ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        or     al, 0ffh
        add    byte ptr [edx], al
        or     al, 0ffh
        add    byte ptr [ebx], al
        or     al, 0ffh
        add    byte ptr [esp + ecx], al
        inc    dword ptr [eax]
        add    eax, 600ff0ch
        or     al, 0ffh
        add    byte ptr [edi], al
        or     al, 0ffh
        add    byte ptr [eax], cl
        or     al, 0ffh
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        inc    dword ptr [eax]
        add    byte ptr [edx], al
        inc    dword ptr [eax]
        add    byte ptr [ebx], al
        inc    dword ptr [eax]
        add    byte ptr [edi + edi*8], al
        add    byte ptr [eax], al
        add    eax, 60000ffh
        inc    dword ptr [eax]
        add    byte ptr [esi], al
        inc    dword ptr [eax]
        add    byte ptr [eax], cl
        inc    dword ptr [eax]
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    al, 0ffh
        add    byte ptr [edx], al
        add    al, 0ffh
        add    byte ptr [ebx], al
        add    al, 0ffh
        add    byte ptr [edx], al
        add    al, 0ffh
        add    byte ptr [ebx], al
        add    al, 0ffh
        add    byte ptr [ebx], al
        add    al, 0ffh
        add    byte ptr [ebx], al
        add    al, 0ffh
        add    byte ptr [ebx], al
        add    al, byte ptr [50ah]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        inc    dword ptr [eax]
        add    byte ptr [edx], al
        inc    dword ptr [eax]
        add    byte ptr [ebx], al
        inc    dword ptr [eax]
        add    byte ptr [edi + edi*8], al
        add    byte ptr [eax], al
        add    eax, 60000ffh
        inc    dword ptr [eax]
        add    byte ptr [edi], al
        inc    dword ptr [eax]
        add    byte ptr [eax], cl
        or     eax, 10900ffh
        add    byte ptr [eax], al
        add    byte ptr [esi + 57h], cl
        dec    esp
        dec    ecx
        dec    esi
        dec    ebx
        add    byte ptr [eax], al
loc_1_18BF0:
        dec    esi
        push   edi
        dec    esp
        dec    ecx
        dec    esi
        dec    ebx
        add    byte ptr [eax], al
loc_1_18BF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_18C00:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_18C04:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_18C08:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ebp + 53h], cl
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 45h], cl
        push   esp
        inc    edx
        inc    ebp
        push   ebp
        dec    ecx
        add    byte ptr [esi + 45h], cl
        push   esp
        inc    edx
        inc    ebp
        push   ebp
        dec    ecx
loc_1_18C33:
        add    byte ptr [ebp + 3dh], al
        add    byte ptr [eax], al
loc_1_18C38:
        and    byte ptr [edx + 3dh], dl
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        push   edx
        cmp    eax, 3d432031h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 26009ccfh], dl
loc_1_18C51:
        lahf
        cmp    dword ptr [ecx], eax
        mov    ecx, 4c01d66eh
        db  03Eh, 073h, 002h
        fisttp word ptr [0dd720310h]
        lodsb
        add    eax, dword ptr [980449adh]
        jl     SHORT loc_1_18C51
        add    al, 2bh
        dec    esp
        add    dword ptr [6201bbeh], 51h
        jmp    SHORT loc_1_18C33
        db  006h, 0E4h, 0BAh, 059h, 007h, 077h, 08Ah, 0F6h, 007h, 00Ah, 05Ah, 093h, 008h, 09Dh, 029h, 030h
        db  009h, 030h, 0F9h, 0CCh, 009h, 0C3h, 0C8h, 069h, 00Ah, 056h, 098h, 006h, 00Bh, 0E9h, 067h, 0A3h
        db  00Bh, 07Ch, 037h, 040h, 00Ch, 00Fh, 007h, 0DDh, 00Ch, 0A2h, 0D6h, 079h, 00Dh, 035h, 0A6h, 016h
        db  00Eh, 0C8h, 075h, 0B3h, 00Eh, 05Bh, 045h, 050h, 00Fh, 0EEh, 014h, 0EDh, 00Fh, 081h, 0E4h, 089h
        db  010h, 014h, 0B4h, 026h, 011h, 0A7h, 083h, 0C3h, 011h, 03Ah, 053h, 060h, 012h, 0CDh, 022h, 0FDh
        db  012h, 060h, 0F2h, 099h, 013h, 0F3h, 0C1h, 036h, 014h, 086h, 091h, 0D3h, 014h, 019h, 061h, 070h
        db  015h, 0ACh, 030h, 00Dh, 016h, 03Fh, 000h, 0AAh, 016h, 0D2h, 0CFh, 046h, 017h, 065h, 09Fh, 0E3h
        db  017h, 0F8h, 06Eh, 080h, 018h, 08Bh, 03Eh, 01Dh, 019h, 01Eh, 00Eh, 0BAh, 019h, 0B1h, 0DDh, 056h
        db  01Ah, 044h, 0ADh, 0F3h, 01Ah, 0D7h, 07Ch, 090h, 01Bh, 06Ah, 04Ch, 02Dh, 01Ch, 0FDh, 01Bh, 0CAh
        db  01Ch, 090h, 0EBh, 066h, 01Dh, 023h, 0BBh, 003h, 01Eh, 0B6h, 08Ah, 0A0h, 01Eh, 049h, 05Ah, 03Dh
        db  01Fh, 0DCh, 029h, 0DAh, 01Fh, 06Fh, 0F9h, 076h, 020h, 002h, 0C9h, 013h, 021h, 095h, 098h, 0B0h
        db  021h, 028h, 068h, 04Dh, 022h, 0BBh, 037h, 0EAh, 022h, 04Eh, 007h, 087h, 023h, 0E1h, 0D6h, 023h
        db  024h, 074h, 0A6h, 0C0h, 024h, 007h, 076h, 05Dh, 025h, 09Ah, 045h, 0FAh, 025h, 02Dh, 015h, 097h
        db  026h, 0C0h, 0E4h, 033h, 027h, 053h, 0B4h, 0D0h, 027h, 0E6h, 083h, 06Dh, 028h, 079h, 053h, 00Ah
        db  029h, 00Ch, 023h, 0A7h, 029h, 09Fh, 0F2h, 043h, 02Ah, 032h, 0C2h, 0E0h, 02Ah, 0C5h, 091h, 07Dh
        db  02Bh, 058h, 061h, 01Ah, 02Ch, 0EBh, 030h, 0B7h, 02Ch, 07Eh, 000h, 054h, 02Dh, 011h, 0D0h, 0F0h
        db  02Dh, 0A4h, 09Fh, 08Dh, 02Eh, 037h, 06Fh, 02Ah, 02Fh, 0CAh, 03Eh, 0C7h, 02Fh, 05Dh, 00Eh, 064h
        db  030h, 0F0h, 0DDh, 000h, 031h, 083h, 0ADh, 09Dh, 031h, 016h, 07Dh, 03Ah, 032h, 0A9h, 04Ch, 0D7h
        db  032h, 03Ch, 01Ch, 074h, 033h, 0CFh, 0EBh, 010h, 034h, 062h, 0BBh, 0ADh, 034h, 0F5h, 08Ah, 04Ah
        db  035h, 088h, 05Ah, 0E7h, 035h, 01Bh, 02Ah, 084h, 036h, 0AEh, 0F9h, 020h, 037h, 041h, 0C9h, 0BDh
        db  037h, 0D4h, 098h, 05Ah, 038h, 067h, 068h, 0F7h, 038h, 0FAh, 037h, 094h, 039h, 08Dh, 007h, 031h
        db  03Ah, 020h, 0D7h, 0CDh, 03Ah, 0B3h, 0A6h, 06Ah, 03Bh, 046h, 076h, 007h, 03Ch, 0D9h, 045h, 0A4h
        db  03Ch, 06Ch, 015h, 041h, 03Dh, 0FFh, 0E4h, 0DDh, 03Dh, 092h, 0B4h, 07Ah, 03Eh, 025h, 084h, 017h
        db  03Fh, 0B8h, 053h, 0B4h, 03Fh, 04Bh, 023h, 051h, 040h, 0DEh, 0F2h, 0EDh, 040h, 071h, 0C2h, 08Ah
        db  041h, 004h, 092h, 027h, 042h, 097h, 061h, 0C4h, 042h, 02Ah, 031h, 061h, 043h, 0BDh, 000h, 0FEh
        db  043h, 050h, 0D0h, 09Ah, 044h, 0E3h, 09Fh, 037h, 045h, 076h, 06Fh, 0D4h, 045h, 009h, 03Fh, 071h
        db  046h, 09Ch, 00Eh, 00Eh, 047h, 02Fh, 0DEh, 0AAh, 047h, 0C2h, 0ADh, 047h, 048h, 055h, 07Dh, 0E4h
        db  048h, 0E8h, 04Ch, 081h, 049h, 07Bh, 01Ch, 01Eh, 04Ah, 00Eh, 0ECh, 0BAh, 04Ah, 0A1h, 0BBh, 057h
        db  04Bh, 034h, 08Bh, 0F4h, 04Bh, 0C7h, 05Ah, 091h, 04Ch, 05Ah, 02Ah, 02Eh, 04Dh, 0EDh, 0F9h, 0CAh
        db  04Dh, 080h, 0C9h, 067h, 04Eh, 013h, 099h, 004h, 04Fh, 0A6h, 068h, 0A1h, 04Fh, 039h, 038h, 03Eh
        db  050h, 0CCh, 007h, 0DBh, 050h, 05Fh, 0D7h, 077h, 051h, 0F2h, 0A6h, 014h, 052h, 085h, 076h, 0B1h
        db  052h, 018h, 046h, 04Eh, 053h, 0ABh, 015h, 0EBh, 053h, 03Eh, 0E5h, 087h, 054h, 0D1h, 0B4h, 024h
        db  055h, 064h, 084h, 0C1h, 055h, 0F7h, 053h, 05Eh, 056h, 08Ah, 023h, 0FBh, 056h, 01Dh, 0F3h, 097h
        db  057h, 0B0h, 0C2h, 034h, 058h, 043h, 092h, 0D1h, 058h, 0D6h, 061h, 06Eh, 059h, 069h, 031h, 00Bh
        db  05Ah, 0FCh, 000h, 0A8h, 05Ah, 08Fh, 0D0h, 044h, 05Bh, 022h, 0A0h, 0E1h, 05Bh, 0B5h, 06Fh, 07Eh
        db  05Ch, 048h, 03Fh, 01Bh, 05Dh, 0DBh, 00Eh, 0B8h, 05Dh, 06Eh, 0DEh, 054h, 05Eh, 001h, 0AEh, 0F1h
        db  05Eh, 094h, 07Dh, 08Eh, 05Fh, 027h, 04Dh, 02Bh, 060h, 0BAh, 01Ch, 0C8h, 060h, 04Dh, 0ECh, 064h
        db  061h, 0E0h, 0BBh, 001h, 062h, 073h, 08Bh, 09Eh, 062h, 006h, 05Bh, 03Bh, 063h, 099h, 02Ah, 0D8h
        db  063h, 02Ch, 0FAh, 074h, 064h, 0BFh, 0C9h, 011h, 065h, 052h, 099h, 0AEh, 065h, 0E5h, 068h, 04Bh
        db  066h, 078h, 038h, 0E8h, 066h, 00Bh, 008h, 085h, 067h, 09Eh, 0D7h, 021h, 068h, 031h, 0A7h, 0BEh
        db  068h, 0C4h, 076h, 05Bh, 069h, 057h, 046h, 0F8h, 069h, 0EAh, 015h, 095h, 06Ah, 07Dh, 0E5h, 031h
        db  06Bh, 010h, 0B5h, 0CEh, 06Bh, 0A3h, 084h, 06Bh, 06Ch, 036h, 054h, 008h, 06Dh, 0C9h, 023h, 0A5h
        db  06Dh, 05Ch, 0F3h, 041h, 06Eh, 0EFh, 0C2h, 0DEh, 06Eh, 082h, 092h, 07Bh, 06Fh, 015h, 062h, 018h
        db  070h, 0A8h, 031h, 0B5h, 070h, 03Bh, 001h, 052h, 071h, 0CEh, 0D0h, 0EEh, 071h, 061h, 0A0h, 08Bh
        db  072h, 0F4h, 06Fh, 028h, 073h, 087h, 03Fh, 0C5h, 073h, 01Ah, 00Fh, 062h, 074h, 0ADh, 0DEh, 0FEh
        db  074h, 040h, 0AEh, 09Bh, 075h, 0D3h, 07Dh, 038h, 076h, 066h, 04Dh, 0D5h, 076h, 0F9h, 01Ch, 072h
        db  077h, 08Ch, 0ECh, 00Eh, 078h, 01Fh, 0BCh, 0ABh, 078h, 0B2h, 08Bh, 048h, 079h, 045h, 05Bh, 0E5h
        db  079h, 0D8h, 02Ah, 082h, 07Ah, 06Bh, 0FAh, 01Eh, 07Bh, 0FEh, 0C9h, 0BBh, 07Bh, 091h, 099h, 058h
        db  07Ch, 024h, 069h, 0F5h, 07Ch, 0B7h, 038h, 092h, 07Dh, 04Ah, 008h, 02Fh, 07Eh, 0DDh, 0D7h, 0CBh
        db  07Eh, 070h, 0A7h, 068h, 07Fh, 003h, 077h, 005h, 080h, 096h, 046h, 0A2h, 080h, 029h, 016h, 03Fh
        db  081h, 0BCh, 0E5h, 0DBh, 081h, 04Fh, 0B5h, 078h, 082h, 0E2h, 084h, 015h, 083h, 075h, 054h, 0B2h
        db  083h, 008h, 024h, 04Fh, 084h, 09Bh, 0F3h, 0EBh, 084h, 02Eh, 0C3h, 088h, 085h, 0C1h, 092h, 025h
        db  086h, 054h, 062h, 0C2h, 086h, 0E7h, 031h, 05Fh, 087h, 07Ah, 001h, 0FCh, 087h, 00Dh, 0D1h, 098h
        db  088h, 0A0h, 0A0h, 035h, 089h, 033h, 070h, 0D2h, 089h, 0C6h, 03Fh, 06Fh, 08Ah, 059h, 00Fh, 00Ch
        db  08Bh, 0ECh, 0DEh, 0A8h, 08Bh, 07Fh, 0AEh, 045h, 08Ch, 012h, 07Eh, 0E2h, 08Ch, 0A5h, 04Dh, 07Fh
        db  08Dh, 038h, 01Dh, 01Ch, 08Eh, 0CBh, 0ECh, 0B8h, 08Eh, 05Eh, 0BCh, 055h, 08Fh, 0F1h, 08Bh, 0F2h
        db  08Fh, 084h, 05Bh, 08Fh, 090h, 017h, 02Bh, 02Ch, 091h, 0AAh, 0FAh, 0C8h, 091h, 03Dh, 0CAh, 065h
        db  092h, 0D0h, 099h, 002h, 093h, 063h, 069h, 09Fh, 093h, 0F6h, 038h, 03Ch, 094h, 089h, 008h, 0D9h
        db  094h, 01Ch, 0D8h, 075h, 095h, 0AFh, 0A7h, 012h, 096h, 042h, 077h, 0AFh, 096h, 0D5h, 046h, 04Ch
        db  097h, 068h, 016h, 0E9h, 097h, 0FBh, 0E5h, 085h, 098h, 08Eh, 0B5h, 022h, 099h, 021h, 085h, 0BFh
        db  099h, 0B4h, 054h, 05Ch, 09Ah, 047h, 024h, 0F9h, 09Ah, 0DAh, 0F3h, 095h, 09Bh, 06Dh, 0C3h, 032h
        db  09Ch, 000h, 000h, 000h, 000h, 000h, 093h, 0CFh, 09Ch, 000h, 026h, 09Fh, 039h, 000h, 0B9h, 06Eh
        db  0D6h, 000h, 04Ch, 03Eh, 073h, 000h, 0DFh, 00Dh, 010h, 000h, 072h, 0DDh, 0ACh, 000h, 005h, 0ADh
        db  049h, 000h, 098h, 07Ch, 0E6h, 000h, 02Bh, 04Ch, 083h, 000h, 0BEh, 01Bh, 020h, 000h, 051h, 0EBh
        db  0BCh, 000h, 0E4h, 0BAh, 059h, 000h, 077h, 08Ah, 0F6h, 000h, 00Ah, 05Ah, 093h, 000h, 09Dh, 029h
        db  030h, 000h, 030h, 0F9h, 0CCh, 000h, 0C3h, 0C8h, 069h, 000h, 056h, 098h, 006h, 000h, 0E9h, 067h
        db  0A3h, 000h, 07Ch, 037h, 040h, 000h, 00Fh, 007h, 0DDh, 000h, 0A2h, 0D6h, 079h, 000h, 035h, 0A6h
        db  016h, 000h, 0C8h, 075h, 0B3h, 000h, 05Bh, 045h, 050h, 000h, 0EEh, 014h, 0EDh, 000h, 081h, 0E4h
        db  089h, 000h, 014h, 0B4h, 026h, 000h, 0A7h, 083h, 0C3h, 000h, 03Ah, 053h, 060h, 000h, 0CDh, 022h
        db  0FDh, 000h, 060h, 0F2h, 099h, 000h, 0F3h, 0C1h, 036h, 000h, 086h, 091h, 0D3h, 000h, 019h, 061h
        db  070h, 000h, 0ACh, 030h, 00Dh, 000h, 03Fh, 000h, 0AAh, 000h, 0D2h, 0CFh, 046h, 000h, 065h, 09Fh
        db  0E3h, 000h, 0F8h, 06Eh, 080h, 000h, 08Bh, 03Eh, 01Dh, 000h, 01Eh, 00Eh, 0BAh, 000h, 0B1h, 0DDh
        db  056h, 000h, 044h, 0ADh, 0F3h, 000h, 0D7h, 07Ch, 090h, 000h, 06Ah, 04Ch, 02Dh, 000h, 0FDh, 01Bh
        db  0CAh, 000h, 090h, 0EBh, 066h, 000h, 023h, 0BBh, 003h, 000h, 0B6h, 08Ah, 0A0h, 000h, 049h, 05Ah
        db  03Dh, 000h, 0DCh, 029h, 0DAh, 000h, 06Fh, 0F9h, 076h, 000h, 002h, 0C9h, 013h, 000h, 095h, 098h
        db  0B0h, 000h, 028h, 068h, 04Dh, 000h, 0BBh, 037h, 0EAh, 000h, 04Eh, 007h, 087h, 000h, 0E1h, 0D6h
        db  023h, 000h, 074h, 0A6h, 0C0h, 000h, 007h, 076h, 05Dh, 000h, 09Ah, 045h, 0FAh, 000h, 02Dh, 015h
        db  097h, 000h, 0C0h, 0E4h, 033h, 000h, 053h, 0B4h, 0D0h, 000h, 0E6h, 083h, 06Dh, 000h, 079h, 053h
        db  00Ah, 000h, 00Ch, 023h, 0A7h, 000h, 09Fh, 0F2h, 043h, 000h, 032h, 0C2h, 0E0h, 000h, 0C5h, 091h
        db  07Dh, 000h, 058h, 061h, 01Ah, 000h, 0EBh, 030h, 0B7h, 000h, 07Eh, 000h, 054h, 000h, 011h, 0D0h
        db  0F0h, 000h, 0A4h, 09Fh, 08Dh, 000h, 037h, 06Fh, 02Ah, 000h, 0CAh, 03Eh, 0C7h, 000h, 05Dh, 00Eh
        db  064h, 000h, 0F0h, 0DDh, 000h, 000h, 083h, 0ADh, 09Dh, 000h, 016h, 07Dh, 03Ah, 000h, 0A9h, 04Ch
        db  0D7h, 000h, 03Ch, 01Ch, 074h, 000h, 0CFh, 0EBh, 010h, 000h, 062h, 0BBh, 0ADh, 000h, 0F5h, 08Ah
        db  04Ah, 000h, 088h, 05Ah, 0E7h, 000h, 01Bh, 02Ah, 084h, 000h, 0AEh, 0F9h, 020h, 000h, 041h, 0C9h
        db  0BDh, 000h, 0D4h, 098h, 05Ah, 000h, 067h, 068h, 0F7h, 000h, 0FAh, 037h, 094h, 000h, 08Dh, 007h
        db  031h, 000h, 020h, 0D7h, 0CDh, 000h, 0B3h, 0A6h, 06Ah, 000h, 046h, 076h, 007h, 000h, 0D9h, 045h
        db  0A4h, 000h, 06Ch, 015h, 041h, 000h, 0FFh, 0E4h, 0DDh, 000h, 092h, 0B4h, 07Ah, 000h, 025h, 084h
        db  017h, 000h, 0B8h, 053h, 0B4h, 000h, 04Bh, 023h, 051h, 000h, 0DEh, 0F2h, 0EDh, 000h, 071h, 0C2h
        db  08Ah, 000h, 004h, 092h, 027h, 000h, 097h, 061h, 0C4h, 000h, 02Ah, 031h, 061h, 000h, 0BDh, 000h
        db  0FEh, 000h, 050h, 0D0h, 09Ah, 000h, 0E3h, 09Fh, 037h, 000h, 076h, 06Fh, 0D4h, 000h, 009h, 03Fh
        db  071h, 000h, 09Ch, 00Eh, 00Eh, 000h, 02Fh, 0DEh, 0AAh, 000h, 0C2h, 0ADh, 047h, 000h, 055h, 07Dh
        db  0E4h, 000h, 0E8h, 04Ch, 081h, 000h, 07Bh, 01Ch, 01Eh, 000h, 00Eh, 0ECh, 0BAh, 000h, 0A1h, 0BBh
        db  057h, 000h, 034h, 08Bh, 0F4h, 000h, 0C7h, 05Ah, 091h, 000h, 05Ah, 02Ah, 02Eh, 000h, 0EDh, 0F9h
        db  0CAh, 000h, 080h, 0C9h, 067h, 000h, 013h, 099h, 004h, 000h, 0A6h, 068h, 0A1h, 000h, 039h, 038h
        db  03Eh, 000h, 0CCh, 007h, 0DBh, 000h, 05Fh, 0D7h, 077h, 000h, 0F2h, 0A6h, 014h, 000h, 085h, 076h
        db  0B1h, 000h, 018h, 046h, 04Eh, 000h, 0ABh, 015h, 0EBh, 000h, 03Eh, 0E5h, 087h, 000h, 0D1h, 0B4h
        db  024h, 000h, 064h, 084h, 0C1h, 000h, 0F7h, 053h, 05Eh, 000h, 08Ah, 023h, 0FBh, 000h, 01Dh, 0F3h
        db  097h, 000h, 0B0h, 0C2h, 034h, 000h, 043h, 092h, 0D1h, 000h, 0D6h, 061h, 06Eh, 000h, 069h, 031h
        db  00Bh, 000h, 0FCh, 000h, 0A8h, 000h, 08Fh, 0D0h, 044h, 000h, 022h, 0A0h, 0E1h, 000h, 0B5h, 06Fh
        db  07Eh, 000h, 048h, 03Fh, 01Bh, 000h, 0DBh, 00Eh, 0B8h, 000h, 06Eh, 0DEh, 054h, 000h, 001h, 0AEh
        db  0F1h, 000h, 094h, 07Dh, 08Eh, 000h, 027h, 04Dh, 02Bh, 000h, 0BAh, 01Ch, 0C8h, 000h, 04Dh, 0ECh
        db  064h, 000h, 0E0h, 0BBh, 001h, 000h, 073h, 08Bh, 09Eh, 000h, 006h, 05Bh, 03Bh, 000h, 099h, 02Ah
        db  0D8h, 000h, 02Ch, 0FAh, 074h, 000h, 0BFh, 0C9h, 011h, 000h, 052h, 099h, 0AEh, 000h, 0E5h, 068h
        db  04Bh, 000h, 078h, 038h, 0E8h, 000h, 00Bh, 008h, 085h, 000h, 09Eh, 0D7h, 021h, 000h, 031h, 0A7h
        db  0BEh, 000h, 0C4h, 076h, 05Bh, 000h, 057h, 046h, 0F8h, 000h, 0EAh, 015h, 095h, 000h, 07Dh, 0E5h
        db  031h, 000h, 010h, 0B5h, 0CEh, 000h, 0A3h, 084h, 06Bh, 000h, 036h, 054h, 008h, 000h, 0C9h, 023h
        db  0A5h, 000h, 05Ch, 0F3h, 041h, 000h, 0EFh, 0C2h, 0DEh, 000h, 082h, 092h, 07Bh, 000h, 015h, 062h
        db  018h, 000h, 0A8h, 031h, 0B5h, 000h, 03Bh, 001h, 052h, 000h, 0CEh, 0D0h, 0EEh, 000h, 061h, 0A0h
        db  08Bh, 000h, 0F4h, 06Fh, 028h, 000h, 087h, 03Fh, 0C5h, 000h, 01Ah, 00Fh, 062h, 000h, 0ADh, 0DEh
        db  0FEh, 000h, 040h, 0AEh, 09Bh, 000h, 0D3h, 07Dh, 038h, 000h, 066h, 04Dh, 0D5h, 000h, 0F9h, 01Ch
        db  072h, 000h, 08Ch, 0ECh, 00Eh, 000h, 01Fh, 0BCh, 0ABh, 000h, 0B2h, 08Bh, 048h, 000h, 045h, 05Bh
        db  0E5h, 000h, 0D8h, 02Ah, 082h, 000h, 06Bh, 0FAh, 01Eh, 000h, 0FEh, 0C9h, 0BBh, 000h, 091h, 099h
        db  058h, 000h, 024h, 069h, 0F5h, 000h, 0B7h, 038h, 092h, 000h, 04Ah, 008h, 02Fh, 000h, 0DDh, 0D7h
        db  0CBh, 000h, 070h, 0A7h, 068h, 000h, 003h, 077h, 005h, 000h, 096h, 046h, 0A2h, 000h, 029h, 016h
        db  03Fh, 000h, 0BCh, 0E5h, 0DBh, 000h, 04Fh, 0B5h, 078h, 000h, 0E2h, 084h, 015h, 000h, 075h, 054h
        db  0B2h, 000h, 008h, 024h, 04Fh, 000h, 09Bh, 0F3h, 0EBh, 000h, 02Eh, 0C3h, 088h, 000h, 0C1h, 092h
        db  025h, 000h, 054h, 062h, 0C2h, 000h, 0E7h, 031h, 05Fh, 000h, 07Ah, 001h, 0FCh, 000h, 00Dh, 0D1h
        db  098h, 000h, 0A0h, 0A0h, 035h, 000h, 033h, 070h, 0D2h, 000h, 0C6h, 03Fh, 06Fh, 000h, 059h, 00Fh
        db  00Ch, 000h, 0ECh, 0DEh, 0A8h, 000h, 07Fh, 0AEh, 045h, 000h, 012h, 07Eh, 0E2h, 000h, 0A5h, 04Dh
        db  07Fh, 000h, 038h, 01Dh, 01Ch, 000h, 0CBh, 0ECh, 0B8h, 000h, 05Eh, 0BCh, 055h, 000h, 0F1h, 08Bh
        db  0F2h, 000h, 084h, 05Bh, 08Fh, 000h, 017h, 02Bh, 02Ch, 000h, 0AAh, 0FAh, 0C8h, 000h, 03Dh, 0CAh
        db  065h, 000h, 0D0h, 099h, 002h, 000h, 063h, 069h, 09Fh, 000h, 0F6h, 038h, 03Ch, 000h, 089h, 008h
        db  0D9h, 000h, 01Ch, 0D8h, 075h, 000h, 0AFh, 0A7h, 012h, 000h, 042h, 077h, 0AFh, 000h, 0D5h, 046h
        db  04Ch, 000h, 068h, 016h, 0E9h, 000h, 0FBh, 0E5h, 085h, 000h, 08Eh, 0B5h, 022h, 000h, 021h, 085h
        db  0BFh, 000h, 0B4h, 054h, 05Ch, 000h, 047h, 024h, 0F9h, 000h, 0DAh, 0F3h, 095h, 000h, 06Dh, 0C3h
        db  032h, 000h, 000h, 000h, 000h, 000h, 000h, 093h, 0CFh, 000h, 000h, 026h, 09Fh, 000h, 000h, 0B9h
        db  06Eh, 000h, 000h, 04Ch, 03Eh, 000h, 000h, 0DFh, 00Dh, 000h, 000h, 072h, 0DDh, 000h, 000h, 005h
        db  0ADh, 000h, 000h, 098h, 07Ch, 000h, 000h, 02Bh, 04Ch, 000h, 000h, 0BEh, 01Bh, 000h, 000h, 051h
        db  0EBh, 000h, 000h, 0E4h, 0BAh, 000h, 000h, 077h, 08Ah, 000h, 000h, 00Ah, 05Ah, 000h, 000h, 09Dh
        db  029h, 000h, 000h, 030h, 0F9h, 000h, 000h, 0C3h, 0C8h, 000h, 000h, 056h, 098h, 000h, 000h, 0E9h
        db  067h, 000h, 000h, 07Ch, 037h, 000h, 000h, 00Fh, 007h, 000h, 000h, 0A2h, 0D6h, 000h, 000h, 035h
        db  0A6h, 000h, 000h, 0C8h, 075h, 000h, 000h, 05Bh, 045h, 000h, 000h, 0EEh, 014h, 000h, 000h, 081h
        db  0E4h, 000h, 000h, 014h, 0B4h, 000h, 000h, 0A7h, 083h, 000h, 000h, 03Ah, 053h, 000h, 000h, 0CDh
        db  022h, 000h, 000h, 060h, 0F2h, 000h, 000h, 0F3h, 0C1h, 000h, 000h, 086h, 091h, 000h, 000h, 019h
        db  061h, 000h, 000h, 0ACh, 030h, 000h, 000h, 03Fh, 000h, 000h, 000h, 0D2h, 0CFh, 000h, 000h, 065h
        db  09Fh, 000h, 000h, 0F8h, 06Eh, 000h, 000h, 08Bh, 03Eh, 000h, 000h, 01Eh, 00Eh, 000h, 000h, 0B1h
        db  0DDh, 000h, 000h, 044h, 0ADh, 000h, 000h, 0D7h, 07Ch, 000h, 000h, 06Ah, 04Ch, 000h, 000h, 0FDh
        db  01Bh, 000h, 000h, 090h, 0EBh, 000h, 000h, 023h, 0BBh, 000h, 000h, 0B6h, 08Ah, 000h, 000h, 049h
        db  05Ah, 000h, 000h, 0DCh, 029h, 000h, 000h, 06Fh, 0F9h, 000h, 000h, 002h, 0C9h, 000h, 000h, 095h
        db  098h, 000h, 000h, 028h, 068h, 000h, 000h, 0BBh, 037h, 000h, 000h, 04Eh, 007h, 000h, 000h, 0E1h
        db  0D6h, 000h, 000h, 074h, 0A6h, 000h, 000h, 007h, 076h, 000h, 000h, 09Ah, 045h, 000h, 000h, 02Dh
        db  015h, 000h, 000h, 0C0h, 0E4h, 000h, 000h, 053h, 0B4h, 000h, 000h, 0E6h, 083h, 000h, 000h, 079h
        db  053h, 000h, 000h, 00Ch, 023h, 000h, 000h, 09Fh, 0F2h, 000h, 000h, 032h, 0C2h, 000h, 000h, 0C5h
        db  091h, 000h, 000h, 058h, 061h, 000h, 000h, 0EBh, 030h, 000h, 000h, 07Eh, 000h, 000h, 000h, 011h
        db  0D0h, 000h, 000h, 0A4h, 09Fh, 000h, 000h, 037h, 06Fh, 000h, 000h, 0CAh, 03Eh, 000h, 000h, 05Dh
        db  00Eh, 000h, 000h, 0F0h, 0DDh, 000h, 000h, 083h, 0ADh, 000h, 000h, 016h, 07Dh, 000h, 000h, 0A9h
        db  04Ch, 000h, 000h, 03Ch, 01Ch, 000h, 000h, 0CFh, 0EBh, 000h, 000h, 062h, 0BBh, 000h, 000h, 0F5h
        db  08Ah, 000h, 000h, 088h, 05Ah, 000h, 000h, 01Bh, 02Ah, 000h, 000h, 0AEh, 0F9h, 000h, 000h, 041h
        db  0C9h, 000h, 000h, 0D4h, 098h, 000h, 000h, 067h, 068h, 000h, 000h, 0FAh, 037h, 000h, 000h, 08Dh
        db  007h, 000h, 000h, 020h, 0D7h, 000h, 000h, 0B3h, 0A6h, 000h, 000h, 046h, 076h, 000h, 000h, 0D9h
        db  045h, 000h, 000h, 06Ch, 015h, 000h, 000h, 0FFh, 0E4h, 000h, 000h, 092h, 0B4h, 000h, 000h, 025h
        db  084h, 000h, 000h, 0B8h, 053h, 000h, 000h, 04Bh, 023h, 000h, 000h, 0DEh, 0F2h, 000h, 000h, 071h
        db  0C2h, 000h, 000h, 004h, 092h, 000h, 000h, 097h, 061h, 000h, 000h, 02Ah, 031h, 000h, 000h, 0BDh
        db  000h, 000h, 000h, 050h, 0D0h, 000h, 000h, 0E3h, 09Fh, 000h, 000h, 076h, 06Fh, 000h, 000h, 009h
        db  03Fh, 000h, 000h, 09Ch, 00Eh, 000h, 000h, 02Fh, 0DEh, 000h, 000h, 0C2h, 0ADh, 000h, 000h, 055h
        db  07Dh, 000h, 000h, 0E8h, 04Ch, 000h, 000h, 07Bh, 01Ch, 000h, 000h, 00Eh, 0ECh, 000h, 000h, 0A1h
        db  0BBh, 000h, 000h, 034h, 08Bh, 000h, 000h, 0C7h, 05Ah, 000h, 000h, 05Ah, 02Ah, 000h, 000h, 0EDh
        db  0F9h, 000h, 000h, 080h, 0C9h, 000h, 000h, 013h, 099h, 000h, 000h, 0A6h, 068h, 000h, 000h, 039h
        db  038h, 000h, 000h, 0CCh, 007h, 000h, 000h, 05Fh, 0D7h, 000h, 000h, 0F2h, 0A6h, 000h, 000h, 085h
        db  076h, 000h, 000h, 018h, 046h, 000h, 000h, 0ABh, 015h, 000h, 000h, 03Eh, 0E5h, 000h, 000h, 0D1h
        db  0B4h, 000h, 000h, 064h, 084h, 000h, 000h, 0F7h, 053h, 000h, 000h, 08Ah, 023h, 000h, 000h, 01Dh
        db  0F3h, 000h, 000h, 0B0h, 0C2h, 000h, 000h, 043h, 092h, 000h, 000h, 0D6h, 061h, 000h, 000h, 069h
        db  031h, 000h, 000h, 0FCh, 000h, 000h, 000h, 08Fh, 0D0h, 000h, 000h, 022h, 0A0h, 000h, 000h, 0B5h
        db  06Fh, 000h, 000h, 048h, 03Fh, 000h, 000h, 0DBh, 00Eh, 000h, 000h, 06Eh, 0DEh, 000h, 000h, 001h
        db  0AEh, 000h, 000h, 094h, 07Dh, 000h, 000h, 027h, 04Dh, 000h, 000h, 0BAh, 01Ch, 000h, 000h, 04Dh
        db  0ECh, 000h, 000h, 0E0h, 0BBh, 000h, 000h, 073h, 08Bh, 000h, 000h, 006h, 05Bh, 000h, 000h, 099h
        db  02Ah, 000h, 000h, 02Ch, 0FAh, 000h, 000h, 0BFh, 0C9h, 000h, 000h, 052h, 099h, 000h, 000h, 0E5h
        db  068h, 000h, 000h, 078h, 038h, 000h, 000h, 00Bh, 008h, 000h, 000h, 09Eh, 0D7h, 000h, 000h, 031h
        db  0A7h, 000h, 000h, 0C4h, 076h, 000h, 000h, 057h, 046h, 000h, 000h, 0EAh, 015h, 000h, 000h, 07Dh
        db  0E5h, 000h, 000h, 010h, 0B5h, 000h, 000h, 0A3h, 084h, 000h, 000h, 036h, 054h, 000h, 000h, 0C9h
        db  023h, 000h, 000h, 05Ch, 0F3h, 000h, 000h, 0EFh, 0C2h, 000h, 000h, 082h, 092h, 000h, 000h, 015h
        db  062h, 000h, 000h, 0A8h, 031h, 000h, 000h, 03Bh, 001h, 000h, 000h, 0CEh, 0D0h, 000h, 000h, 061h
        db  0A0h, 000h, 000h, 0F4h, 06Fh, 000h, 000h, 087h, 03Fh, 000h, 000h, 01Ah, 00Fh, 000h, 000h, 0ADh
        db  0DEh, 000h, 000h, 040h, 0AEh, 000h, 000h, 0D3h, 07Dh, 000h, 000h, 066h, 04Dh, 000h, 000h, 0F9h
        db  01Ch, 000h, 000h, 08Ch, 0ECh, 000h, 000h, 01Fh, 0BCh, 000h, 000h, 0B2h, 08Bh, 000h, 000h, 045h
        db  05Bh, 000h, 000h, 0D8h, 02Ah, 000h, 000h, 06Bh, 0FAh, 000h, 000h, 0FEh, 0C9h, 000h, 000h, 091h
        db  099h, 000h, 000h, 024h, 069h, 000h, 000h, 0B7h, 038h, 000h, 000h, 04Ah, 008h, 000h, 000h, 0DDh
        db  0D7h, 000h, 000h, 070h, 0A7h, 000h, 000h, 003h, 077h, 000h, 000h, 096h, 046h, 000h, 000h, 029h
        db  016h, 000h, 000h, 0BCh, 0E5h, 000h, 000h, 04Fh, 0B5h, 000h, 000h, 0E2h, 084h, 000h, 000h, 075h
        db  054h, 000h, 000h, 008h, 024h, 000h, 000h, 09Bh, 0F3h, 000h, 000h, 02Eh, 0C3h, 000h, 000h, 0C1h
        db  092h, 000h, 000h, 054h, 062h, 000h, 000h, 0E7h, 031h, 000h, 000h, 07Ah, 001h, 000h, 000h, 00Dh
        db  0D1h, 000h, 000h, 0A0h, 0A0h, 000h, 000h, 033h, 070h, 000h, 000h, 0C6h, 03Fh, 000h, 000h, 059h
        db  00Fh, 000h, 000h, 0ECh, 0DEh, 000h, 000h, 07Fh, 0AEh, 000h, 000h, 012h, 07Eh, 000h, 000h, 0A5h
        db  04Dh, 000h, 000h, 038h, 01Dh, 000h, 000h, 0CBh, 0ECh, 000h, 000h, 05Eh, 0BCh, 000h, 000h, 0F1h
        db  08Bh, 000h, 000h, 084h, 05Bh, 000h, 000h, 017h, 02Bh, 000h, 000h, 0AAh, 0FAh, 000h, 000h, 03Dh
        db  0CAh, 000h, 000h, 0D0h, 099h, 000h, 000h, 063h, 069h, 000h, 000h, 0F6h, 038h, 000h, 000h, 089h
        db  008h, 000h, 000h, 01Ch, 0D8h, 000h, 000h, 0AFh, 0A7h, 000h, 000h, 042h, 077h, 000h, 000h, 0D5h
        db  046h, 000h, 000h, 068h, 016h, 000h, 000h, 0FBh, 0E5h, 000h, 000h, 08Eh, 0B5h, 000h, 000h, 021h
        db  085h, 000h, 000h, 0B4h, 054h, 000h, 000h, 047h, 024h, 000h, 000h, 0DAh, 0F3h, 000h, 000h, 06Dh
        db  0C3h
loc_1_19848:
        dec    esp
        jp     SHORT loc_1_198BE
        xor    dh, byte ptr [edx]
        xor    dword ptr [65762043h], ebp
        jb     SHORT loc_1_198C8
        imul   ebp, dword ptr [edi + 6eh], 302e3420h
        inc    esi
        or     al, byte ptr [ebx + 6fh]
        db  070h, 079h
        jb     SHORT loc_1_198CD
        push   39312074h
        cmp    byte ptr [eax], bh
        sub    eax, 53203539h
        je     SHORT loc_1_198D4
        arpl   word ptr [eax], sp
        inc    ebp
        insb
        arpl   word ptr gs:[edx + esi*2 + 6fh], si
        outsb
        imul   esp, dword ptr [ebx + 73h], 6143202ch
        db  072h, 06Ch
        db  073h, 062h
        popad
        sub    al, 20h
        inc    ebx
        popad
        insb
        imul   esp, dword ptr [esi + 6fh], 61696e72h
        or     al, byte ptr cs:[ecx + 6ch]
        insb
        and    byte ptr [edx + 69h], dh
        push   72207374h
        db  065h, 073h, 065h
        db  072h, 076h
        add    byte ptr cs:[eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_198BE:
        add    byte ptr [eax], al
loc_1_198C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_198C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_198C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_198CD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_198D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_198D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
