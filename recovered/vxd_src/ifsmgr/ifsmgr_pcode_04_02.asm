; ifsmgr_pcode_04_02.asm -- IFSMGR object 4 segment _PTEXT  (part 2/5)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_514C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    bl, byte ptr [ebp + 0ch]
        mov    byte ptr [esi + 89h], bl
        mov    word ptr [esi + 18h], 0
        db  068h, 026h, 034h, 000h, 000h
        push   esi
        call   sub_4_2547
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_517F
        movsx  eax, word ptr [esi + 1ah]
        jmp    NEAR PTR loc_4_5230
loc_4_517F:
        mov    al, 1
        mov    cl, bl
        shl    al, cl
        db  008h, 005h, 0D8h, 06Ch, 000h, 000h
        sub    edi, edi
        db  039h, 03Dh, 0D0h, 066h, 000h, 000h
        jle    SHORT loc_4_51D4
loc_4_5195:
        mov    eax, dword ptr [esi + 94h]
        push   1
        mov    eax, dword ptr [eax + 4ch]
        push   dword ptr [eax + edi*4]
        push   esi
        call   sub_4_5283
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_51CB
        mov    word ptr [esi + 6], di
        db  068h, 000h, 034h, 000h, 000h
        push   esi
        call   sub_4_2547
        add    esp, 8
        push   esi
        call   sub_4_5235
        add    esp, 4
loc_4_51CB:
        inc    edi
        db  039h, 03Dh, 0D0h, 066h, 000h, 000h
        jg     SHORT loc_4_5195
loc_4_51D4:
        db  068h, 017h, 033h, 000h, 000h
        push   esi
        call   sub_4_2547
        add    esp, 8
        mov    edi, 100h
        db  039h, 03Dh, 0D8h, 066h, 000h, 000h
        jle    SHORT loc_4_522E
loc_4_51EF:
        mov    eax, dword ptr [esi + 94h]
        push   1
        mov    eax, dword ptr [eax + 4ch]
        push   dword ptr [eax + edi*4]
        push   esi
        call   sub_4_5283
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_5225
        mov    word ptr [esi + 6], di
        db  068h, 000h, 034h, 000h, 000h
        push   esi
        call   sub_4_2547
        add    esp, 8
        push   esi
        call   sub_4_5235
        add    esp, 4
loc_4_5225:
        inc    edi
        db  03Bh, 03Dh, 0D8h, 066h, 000h, 000h
        jl     SHORT loc_4_51EF
loc_4_522E:
        sub    eax, eax
loc_4_5230:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_5235:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 78h]
        movzx  ecx, word ptr [eax + 6]
        cmp    ecx, 100h
        mov    dword ptr [edx + 15h], 0
        mov    word ptr [edx + 5], 88c0h
        mov    dword ptr [edx + 7], 10001h
        mov    bl, byte ptr [eax + 89h]
        mov    byte ptr [edx + 1fh], bl
        jge    SHORT loc_4_527C
        mov    eax, dword ptr [eax + 90h]
        mov    eax, dword ptr [eax + 4]
        db  003h, 005h, 044h, 059h, 000h, 000h
        or     byte ptr [ecx + eax + 2eh], 4
loc_4_527C:
        mov    word ptr [edx + 1bh], cx
        pop    ebx
        pop    ebp
        ret
sub_4_5283:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        mov    edx, dword ptr [ebp + 8]
        je     SHORT loc_4_52A5
        movzx  ecx, byte ptr [edx + 89h]
        db  083h, 03Ch, 08Dh, 0E4h, 06Fh, 000h, 000h, 000h
        jne    SHORT loc_4_52A5
        sub    eax, eax
loc_4_52A5:
        mov    esi, dword ptr [ebp + 0ch]
        or     esi, esi
        je     SHORT loc_4_5311
        cmp    word ptr [esi], 0
        je     SHORT loc_4_5311
        or     eax, eax
        je     SHORT loc_4_52E7
        test   byte ptr [esi + 5], 80h
        je     SHORT loc_4_5311
        mov    bl, byte ptr [edx + 89h]
        mov    eax, dword ptr [esi + 7]
        movzx  ecx, bl
sub_4_52C8:
        db  039h, 004h, 08Dh, 0E4h, 06Fh, 000h, 000h
        je     SHORT loc_4_52DD
        or     bl, bl
        jne    SHORT loc_4_5311
        db  039h, 005h, 008h, 070h, 000h, 000h
        jne    SHORT loc_4_5311
loc_4_52DD:
        mov    dword ptr [edx + 78h], esi
        mov    eax, 1
        jmp    SHORT loc_4_5313
loc_4_52E7:
        mov    ax, word ptr [esi + 5]
        and    ah, 88h
        cmp    ah, 88h
        jne    SHORT loc_4_5311
        cmp    dword ptr [esi + 7], 10001h
        jne    SHORT loc_4_5311
        mov    al, byte ptr [edx + 89h]
        cmp    byte ptr [esi + 1fh], al
        jne    SHORT loc_4_5311
        mov    dword ptr [edx + 78h], esi
        mov    eax, 1
        jmp    SHORT loc_4_5313
loc_4_5311:
        sub    eax, eax
loc_4_5313:
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_5317:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  08Bh, 03Dh, 0D0h, 066h, 000h, 000h
        mov    eax, dword ptr [esi + 90h]
        push   esi
        test   byte ptr [eax], 8
        je     SHORT loc_4_533B
        call   sub_4_505A
        add    esp, 4
        jmp    SHORT loc_4_5387
loc_4_533B:
        call   sub_4_538B
        add    esp, 4
        cmp    edi, 0ffh
        jge    SHORT loc_4_5385
loc_4_534B:
        mov    eax, dword ptr [esi + 94h]
        push   1
        mov    eax, dword ptr [eax + 4ch]
        push   dword ptr [eax + edi*4]
        push   esi
        call   sub_4_5283
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_537C
        mov    word ptr [esi + 6], di
        push   esi
        call   sub_4_5400
        add    esp, 4
        push   esi
        call   sub_4_5235
        add    esp, 4
loc_4_537C:
        inc    edi
        cmp    edi, 0ffh
        jl     SHORT loc_4_534B
loc_4_5385:
        sub    eax, eax
loc_4_5387:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_538B:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 90h]
        mov    ecx, dword ptr [edx + 94h]
        mov    edi, dword ptr [eax + 4]
        db  003h, 03Dh, 044h, 059h, 000h, 000h
        movzx  eax, byte ptr [edx + 89h]
        mov    esi, dword ptr [ecx + eax*4 + 24h]
        or     byte ptr [eax + edi + 5fh], 10h
        mov    al, 1
        mov    edi, dword ptr [edx + 94h]
        mov    cl, byte ptr [edx + 89h]
        shl    al, cl
        or     byte ptr [edi + 0bh], al
        mov    al, byte ptr [edx + 89h]
        cmp    al, 3
        jae    SHORT loc_4_53FC
        db  08Bh, 00Dh, 044h, 059h, 000h, 000h
        movzx  eax, al
        movzx  ecx, word ptr [ecx + eax*2 + 28h]
        mov    eax, dword ptr [edx + 90h]
        add    ecx, dword ptr [eax + 4]
        db  003h, 00Dh, 0ACh, 065h, 000h, 000h
        mov    dword ptr [esi + 54h], ecx
        mov    byte ptr [ecx + 1], 1
        mov    byte ptr [ecx], 0
loc_4_53FC:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_5400:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [eax + 94h]
        movzx  ecx, byte ptr [eax + 89h]
        mov    ecx, dword ptr [edx + ecx*4 + 24h]
        movzx  eax, word ptr [eax + 6]
        mov    dword ptr [edx + eax*4 + 0d4h], ecx
        sub    eax, eax
        ret
sub_4_5426:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  ebx, byte ptr [esi + 89h]
        mov    eax, dword ptr [esi + 94h]
        mov    edx, dword ptr [eax + ebx*4 + 24h]
        or     edx, edx
        jne    SHORT loc_4_546D
        push   1
        push   5ch
        db  0E8h, 0ABh, 031h, 001h, 000h
        add    esp, 8
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_4_5463
        mov    word ptr [esi + 1ah], 8
        mov    eax, 0ffffffffh
        jmp    SHORT loc_4_549A
loc_4_5463:
        mov    eax, dword ptr [esi + 94h]
        mov    dword ptr [eax + ebx*4 + 24h], edx
loc_4_546D:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 17h
        rep stosd
        mov    byte ptr [edx + 58h], bl
        mov    dword ptr [edx + 0ch], edx
        mov    eax, dword ptr [esi + 7ch]
        mov    dword ptr [edx + 10h], eax
        sub    eax, eax
        db  0C7h, 002h, 080h, 041h, 000h, 000h
        db  0C7h, 042h, 004h, 04Fh, 04Dh, 000h, 000h
        db  0C7h, 042h, 008h, 030h, 06Dh, 000h, 000h
loc_4_549A:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  0A1h, 0B0h, 055h, 000h, 000h
        lea    edi, [esi + 74h]
        cmp    dword ptr [esi + 90h], eax
        jne    SHORT loc_4_54CD
        mov    eax, dword ptr [edi]
        push   0
        push   dword ptr [eax + 14h]
        push   esi
        call   sub_4_5283
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_54E4
        jmp    SHORT loc_4_551F
loc_4_54CD:
        movzx  eax, byte ptr [esi + 89h]
        mov    edx, dword ptr [esi + 94h]
        mov    ecx, dword ptr [edi]
        cmp    dword ptr [edx + eax*4 + 24h], ecx
        je     SHORT loc_4_54E4
        jmp    SHORT loc_4_551F
loc_4_54E4:
        movzx  eax, byte ptr [esi + 89h]
        mov    ecx, dword ptr [edi]
        lea    edx, [eax*4]
        mov    eax, dword ptr [ecx + 10h]
        db  039h, 082h, 034h, 06Eh, 000h, 000h
        jne    SHORT loc_4_551F
        db  039h, 082h, 058h, 06Eh, 000h, 000h
        jne    SHORT loc_4_551F
        db  081h, 079h, 004h, 04Fh, 04Dh, 000h, 000h
        jne    SHORT loc_4_5512
        jmp    SHORT loc_4_551F
loc_4_5512:
        mov    word ptr [esi + 1ah], 961h
        mov    eax, 1
        jmp    SHORT loc_4_5521
loc_4_551F:
        sub    eax, eax
loc_4_5521:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_5525:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    cl, byte ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        mov    al, 1
        mov    byte ptr [esi + 89h], cl
        shl    al, cl
        not    al
        db  020h, 005h, 0D8h, 06Ch, 000h, 000h
        push   8
        db  068h, 015h, 036h, 000h, 000h
        push   esi
        call   sub_4_4F1A
        add    esp, 0ch
        db  068h, 0A5h, 036h, 000h, 000h
        push   esi
        call   sub_4_2547
        add    esp, 8
        sub    edi, edi
        db  039h, 03Dh, 0D0h, 066h, 000h, 000h
        jle    SHORT loc_4_55A9
loc_4_556A:
        mov    eax, dword ptr [esi + 94h]
        push   0
        mov    eax, dword ptr [eax + 4ch]
        push   dword ptr [eax + edi*4]
        push   esi
        call   sub_4_5283
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_55A0
        mov    word ptr [esi + 6], di
        db  068h, 0B2h, 038h, 000h, 000h
        push   esi
        call   sub_4_2547
        add    esp, 8
        push   esi
        call   sub_4_57EE
        add    esp, 4
loc_4_55A0:
        inc    edi
        db  039h, 03Dh, 0D0h, 066h, 000h, 000h
        jg     SHORT loc_4_556A
loc_4_55A9:
        db  068h, 03Ch, 038h, 000h, 000h
        push   esi
        call   sub_4_2547
        add    esp, 8
        mov    edi, 100h
        db  039h, 03Dh, 0D8h, 066h, 000h, 000h
        jle    SHORT loc_4_5603
loc_4_55C4:
        mov    eax, dword ptr [esi + 94h]
        push   0
        mov    eax, dword ptr [eax + 4ch]
        push   dword ptr [eax + edi*4]
        push   esi
        call   sub_4_5283
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_55FA
        mov    word ptr [esi + 6], di
        db  068h, 0B2h, 038h, 000h, 000h
        push   esi
        call   sub_4_2547
        add    esp, 8
        push   esi
        call   sub_4_57EE
        add    esp, 4
loc_4_55FA:
        inc    edi
        db  03Bh, 03Dh, 0D8h, 066h, 000h, 000h
        jl     SHORT loc_4_55C4
loc_4_5603:
        db  068h, 06Dh, 037h, 000h, 000h
        push   esi
        call   sub_4_2547
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 74h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 0B0h, 055h, 000h, 000h
        cmp    dword ptr [ecx + 90h], eax
        je     SHORT loc_4_5639
        jmp    SHORT loc_4_569C
loc_4_5639:
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 8]
        call   sub_4_5283
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_564F
        jmp    SHORT loc_4_569C
loc_4_564F:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 74h]
        db  081h, 078h, 010h, 0DCh, 06Dh, 000h, 000h
        jne    SHORT loc_4_5660
        jmp    SHORT loc_4_569C
loc_4_5660:
        mov    ecx, dword ptr [ebp + 8]
        movzx  edx, byte ptr [ecx + 89h]
        mov    ecx, dword ptr [ecx + 94h]
        cmp    dword ptr [ecx + edx*4 + 24h], eax
        jne    SHORT loc_4_5678
        jmp    SHORT loc_4_569C
loc_4_5678:
        int3
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 78h], eax
        mov    ax, word ptr [eax + 1bh]
        mov    word ptr [ecx + 6], ax
        push   ecx
        db  0E8h, 02Dh, 0E7h, 0FAh, 0FFh
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], 0
loc_4_569C:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  ecx, byte ptr [esi + 89h]
        mov    eax, dword ptr [esi + 94h]
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + ecx*4 + 24h]
        push   ecx
        mov    dword ptr [eax + 4ch], edx
        push   esi
        call   sub_4_6F90
        add    esp, 8
        mov    eax, dword ptr [esi + 74h]
        push   esi
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [esi + 90h]
        test   byte ptr [eax], 8
        je     SHORT loc_4_56ED
        call   sub_4_505A
        add    esp, 4
        jmp    SHORT loc_4_56F7
loc_4_56ED:
        call   sub_4_56FA
        add    esp, 4
        sub    eax, eax
loc_4_56F7:
        pop    esi
        pop    ebp
        ret
sub_4_56FA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    bl, 1
        movzx  ecx, byte ptr [edx + 89h]
        mov    eax, dword ptr [edx + 94h]
        shl    bl, cl
        not    bl
        and    byte ptr [eax + 0bh], bl
        cmp    ecx, 3
        jge    SHORT loc_4_573A
        db  0A1h, 044h, 059h, 000h, 000h
        movzx  esi, word ptr [eax + ecx*2 + 28h]
        mov    eax, dword ptr [edx + 90h]
        add    esi, dword ptr [eax + 4]
        db  0A1h, 0ACh, 065h, 000h, 000h
        mov    byte ptr [esi + eax + 1], 0
loc_4_573A:
        mov    eax, dword ptr [edx + 90h]
        movzx  esi, byte ptr [edx + 89h]
        mov    eax, dword ptr [eax + 4]
        db  003h, 005h, 044h, 059h, 000h, 000h
        and    byte ptr [esi + eax + 5fh], 0efh
        mov    eax, dword ptr [edx + 94h]
        pop    esi
        mov    eax, dword ptr [eax + ecx*4 + 24h]
loc_4_5760:
        mov    dword ptr [edx + 74h], eax
        mov    dword ptr [eax + 18h], 0
        pop    ebx
        pop    ebp
        ret
sub_4_576D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, byte ptr [esi + 89h]
        mov    ecx, dword ptr [esi + 94h]
        mov    edi, dword ptr [ecx + eax*4 + 24h]
        mov    eax, dword ptr [esi + 90h]
        test   byte ptr [eax], 8
        je     SHORT loc_4_5793
        jmp    SHORT loc_4_57E8
loc_4_5793:
        mov    eax, dword ptr [edi + 50h]
        or     eax, eax
        je     SHORT loc_4_57A5
        push   0
        push   eax
        db  0E8h, 05Eh, 02Eh, 001h, 000h
        add    esp, 8
loc_4_57A5:
        cmp    dword ptr [edi + 18h], 0
        je     SHORT loc_4_57C8
        db  0C7h, 047h, 010h, 0DCh, 06Dh, 000h, 000h
        db  0C7h, 007h, 06Dh, 041h, 000h, 000h
        db  0C7h, 047h, 004h, 06Dh, 041h, 000h, 000h
        db  0C7h, 047h, 008h, 078h, 06Dh, 000h, 000h
        jmp    SHORT loc_4_57E8
loc_4_57C8:
        mov    eax, dword ptr [esi + 94h]
        movzx  ecx, byte ptr [esi + 89h]
        mov    dword ptr [eax + ecx*4 + 24h], 0
        push   0
        push   edi
        db  0E8h, 01Bh, 02Eh, 001h, 000h
        add    esp, 8
loc_4_57E8:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_57EE:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 78h]
        mov    eax, dword ptr [ecx + 90h]
        mov    esi, dword ptr [eax + 4]
        db  003h, 035h, 044h, 059h, 000h, 000h
        movzx  eax, byte ptr [ecx + 89h]
        db  08Bh, 004h, 085h, 0E4h, 06Fh, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_5839
        mov    dword ptr [edx + 7], eax
        mov    ax, word ptr [edx + 5]
        and    ax, 77ffh
        mov    word ptr [edx + 5], ax
        or     ax, 80h
        mov    word ptr [edx + 5], ax
        movzx  eax, word ptr [ecx + 6]
        and    byte ptr [esi + eax + 2eh], 0fbh
loc_4_5839:
        pop    esi
        pop    ebp
        ret
sub_4_583C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  08Bh, 03Dh, 0D0h, 066h, 000h, 000h
        mov    eax, dword ptr [esi + 90h]
        test   byte ptr [eax], 8
        je     SHORT loc_4_5858
        jmp    SHORT loc_4_58AB
loc_4_5858:
        mov    ecx, dword ptr [esi + 94h]
        movzx  eax, byte ptr [esi + 89h]
        mov    ebx, dword ptr [ecx + eax*4 + 24h]
        cmp    edi, 0ffh
        mov    dword ptr [esi + 74h], ebx
        jge    SHORT loc_4_58AB
loc_4_5874:
        mov    eax, dword ptr [esi + 94h]
        cmp    dword ptr [eax + edi*4 + 0d4h], ebx
        jne    SHORT loc_4_58A2
        mov    word ptr [esi + 6], di
        mov    eax, dword ptr [eax + 4ch]
        push   esi
        mov    eax, dword ptr [eax + edi*4]
        mov    dword ptr [esi + 78h], eax
        call   sub_4_58B2
        add    esp, 4
        push   esi
        call   sub_4_57EE
        add    esp, 4
loc_4_58A2:
        inc    edi
        cmp    edi, 0ffh
        jl     SHORT loc_4_5874
loc_4_58AB:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_58B2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, word ptr [esi + 6]
        movzx  ecx, byte ptr [esi + 89h]
        lea    edi, [ecx*4]
        mov    ecx, dword ptr [esi + 94h]
        mov    edx, dword ptr [esi + 74h]
        lea    eax, [ecx + eax*4 + 0d4h]
        mov    ebx, dword ptr [eax]
        cmp    dword ptr [ecx + edi + 24h], ebx
        je     SHORT loc_4_58E7
        jmp    SHORT loc_4_5906
loc_4_58E7:
        db  083h, 0BFh, 0E4h, 06Fh, 000h, 000h, 000h
        jne    SHORT loc_4_5900
        mov    eax, dword ptr [esi + 90h]
        test   byte ptr [eax], 8
        jne    SHORT loc_4_5906
        inc    dword ptr [edx + 18h]
        jmp    SHORT loc_4_5906
loc_4_5900:
        mov    dword ptr [eax], 0
loc_4_5906:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_590D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, byte ptr [esi + 89h]
        lea    ecx, [eax*4]
        mov    eax, dword ptr [esi + 94h]
        mov    eax, dword ptr [ecx + eax + 24h]
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     NEAR PTR loc_4_59DA
        db  083h, 0B9h, 0E4h, 06Fh, 000h, 000h, 000h
        je     NEAR PTR loc_4_59DA
        push   esi
        call   sub_4_56FA
        add    esp, 4
        sub    edx, edx
        db  039h, 015h, 0D0h, 066h, 000h, 000h
        jle    SHORT loc_4_598A
        sub    edi, edi
loc_4_595E:
        movzx  ebx, byte ptr [esi + 89h]
        mov    eax, dword ptr [esi + 94h]
        mov    ecx, dword ptr [edi + eax + 0d4h]
        cmp    dword ptr [eax + ebx*4 + 24h], ecx
        jne    SHORT loc_4_597E
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 18h]
loc_4_597E:
        add    edi, 4
        inc    edx
        db  039h, 015h, 0D0h, 066h, 000h, 000h
        jg     SHORT loc_4_595E
loc_4_598A:
        push   esi
        call   sub_4_583C
        add    esp, 4
        mov    edi, 100h
        db  039h, 03Dh, 0D8h, 066h, 000h, 000h
        jle    SHORT loc_4_59D1
        mov    edx, 400h
loc_4_59A5:
        movzx  ebx, byte ptr [esi + 89h]
        mov    eax, dword ptr [esi + 94h]
        mov    ecx, dword ptr [edx + eax + 0d4h]
        cmp    dword ptr [eax + ebx*4 + 24h], ecx
        jne    SHORT loc_4_59C5
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 18h]
loc_4_59C5:
        add    edx, 4
        inc    edi
        db  03Bh, 03Dh, 0D8h, 066h, 000h, 000h
        jl     SHORT loc_4_59A5
loc_4_59D1:
        push   esi
        call   sub_4_576D
        add    esp, 4
loc_4_59DA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 8]
        movzx  ecx, word ptr [eax + 6]
        db  039h, 00Dh, 0D0h, 066h, 000h, 000h
        jle    SHORT loc_4_5A23
        sub    edx, edx
        mov    ebx, 0ffh
loc_4_59FC:
        db  038h, 09Ah, 0A4h, 083h, 000h, 000h
        je     SHORT loc_4_5A15
        db  00Fh, 0B6h, 0B2h, 098h, 083h, 000h, 000h
        cmp    esi, ecx
        je     SHORT loc_4_5A15
        inc    edx
        cmp    edx, 8
        jl     SHORT loc_4_59FC
loc_4_5A15:
        db  068h, 027h, 03Ah, 000h, 000h
        push   eax
        call   sub_4_2547
        add    esp, 8
loc_4_5A23:
        pop    esi
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        movzx  ecx, word ptr [eax + 6]
        mov    edx, dword ptr [eax + 94h]
        mov    eax, dword ptr [eax + 74h]
        lea    esi, [edx + ecx*4 + 0d4h]
        movsx  eax, byte ptr [eax + 58h]
        mov    ecx, dword ptr [esi]
        cmp    dword ptr [edx + eax*4 + 24h], ecx
        mov    eax, 0
        jne    SHORT loc_4_5A5B
        mov    dword ptr [esi], 0
        sub    eax, eax
loc_4_5A5B:
        pop    esi
        pop    ebp
        ret
sub_4_5A5E:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 94h]
        and    byte ptr [eax + 8], 0f7h
        mov    eax, dword ptr [ecx + 94h]
        mov    dl, byte ptr [eax + 8]
        add    eax, 8
        test   dl, 40h
        jne    SHORT loc_4_5A98
        or     dl, 40h
        mov    byte ptr [eax], dl
        push   dword ptr [ecx + 90h]
        db  0FFh, 035h, 0DCh, 06Ch, 000h, 000h
        call   sub_4_2461
        add    esp, 8
loc_4_5A98:
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 94h]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [esi + 94h]
        mov    cl, byte ptr [eax + 8]
        add    eax, 8
        test   cl, 8
        je     SHORT loc_4_5AD7
        and    cl, 0bfh
        mov    byte ptr [eax], cl
        push   esi
        call   sub_4_5A5E
        add    esp, 4
        jmp    NEAR PTR loc_4_5B70
loc_4_5AD7:
        mov    dword ptr [ebp - 4], 0
        sub    edi, edi
loc_4_5AE0:
        mov    eax, dword ptr [esi + 94h]
        mov    ebx, dword ptr [eax + edi*4 + 24h]
        or     ebx, ebx
        je     SHORT loc_4_5B44
        db  081h, 07Bh, 004h, 04Fh, 04Dh, 000h, 000h
        je     SHORT loc_4_5B44
        inc    dword ptr [ebp - 4]
        cmp    byte ptr [ebx + 5bh], 0
        je     SHORT loc_4_5B44
        mov    eax, dword ptr [ebx + 4ch]
        mov    dword ptr [esi + 8], eax
        db  0A0h, 0ECh, 06Ah, 000h, 000h
        mov    byte ptr [esi + 5], al
        push   ebx
        push   esi
        call   sub_4_11886
        add    esp, 8
        cmp    word ptr [esi + 1ah], 0
        jl     SHORT loc_4_5B63
        mov    al, byte ptr [ebx + 5bh]
        or     al, al
        je     SHORT loc_4_5B44
        cmp    dword ptr [ebx + 4ch], 1
        je     SHORT loc_4_5B44
        lea    ecx, [eax + 1]
        mov    byte ptr [ebx + 5bh], cl
        cmp    al, 1
        jle    SHORT loc_4_5B44
        push   edi
        push   esi
        call   sub_4_6F90
        add    esp, 8
        or     eax, eax
        jl     SHORT loc_4_5B63
loc_4_5B44:
        inc    edi
        cmp    edi, 9
        jl     SHORT loc_4_5AE0
        mov    eax, dword ptr [esi + 94h]
        and    byte ptr [eax + 8], 0bfh
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_5B63
        push   esi
        call   sub_4_5A5E
        add    esp, 4
loc_4_5B63:
        mov    word ptr [esi + 1ah], 0
        mov    dword ptr [esi + 7ch], 0
loc_4_5B70:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 108h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        db  003h, 00Dh, 0B4h, 055h, 000h, 000h
        mov    dword ptr [ebp - 4], ecx
        db  08Bh, 035h, 0B4h, 055h, 000h, 000h
        db  003h, 035h, 0B0h, 055h, 000h, 000h
        db  0A1h, 0D4h, 066h, 000h, 000h
        lea    eax, [ecx + eax*4]
        add    eax, 4d4h
        mov    dword ptr [ecx + 4ch], eax
        mov    ecx, dword ptr [ebp + 8]
        db  039h, 00Dh, 0B0h, 055h, 000h, 000h
        je     SHORT loc_4_5BE0
        mov    edi, dword ptr [ebp - 4]
        add    edi, 50h
        lea    edx, [esi + 50h]
        mov    ecx, 20h
loc_4_5BC4:
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     SHORT loc_4_5BCE
        inc    byte ptr [eax]
        mov    dword ptr [edi], eax
loc_4_5BCE:
        add    edi, 4
        add    edx, 4
        dec    ecx
        jne    SHORT loc_4_5BC4
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [esi + 0ah]
        mov    byte ptr [ecx + 0ah], al
loc_4_5BE0:
        mov    ecx, dword ptr [ebp - 4]
        db  0A1h, 08Ch, 065h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
loc_4_5BEB:
        db  089h, 00Dh, 08Ch, 065h, 000h, 000h
        mov    eax, dword ptr [ecx + 4]
        db  0C7h, 001h, 088h, 065h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A1h, 094h, 065h, 000h, 000h
        mov    dword ptr [ecx + 0ch], eax
        or     byte ptr [ecx + 8], 1
        push   dword ptr [ebp + 8]
        call   sub_4_22AA
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 78h], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 78h]
        mov    dword ptr [ebp - 74h], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, 1
        db  003h, 03Dh, 044h, 059h, 000h, 000h
        mov    byte ptr [edi + 12eh], 0
        db  039h, 005h, 044h, 066h, 000h, 000h
        jb     SHORT loc_4_5C68
        sub    edx, edx
        mov    ecx, eax
loc_4_5C43:
        db  039h, 014h, 085h, 0BCh, 065h, 000h, 000h
        jne    SHORT loc_4_5C5B
        db  084h, 088h, 08Bh, 000h, 000h, 000h
        jne    SHORT loc_4_5C5B
        and    byte ptr [edi + eax + 2eh], 0feh
        jmp    SHORT loc_4_5C5F
loc_4_5C5B:
        or     byte ptr [edi + eax + 2eh], cl
loc_4_5C5F:
        inc    eax
        db  039h, 005h, 044h, 066h, 000h, 000h
loc_4_5C66:
        jae    SHORT loc_4_5C43
loc_4_5C68:
        mov    byte ptr [ebp - 7fh], 0
loc_4_5C6C:
        movzx  edx, byte ptr [ebp - 7fh]
        lea    ecx, [edx*4]
        db  08Bh, 081h, 034h, 06Eh, 000h, 000h
        or     eax, eax
        jne    SHORT loc_4_5CA0
        db  083h, 0B9h, 058h, 06Eh, 000h, 000h, 000h
        jne    SHORT loc_4_5C94
        and    byte ptr [edx + edi + 5fh], 0efh
        jmp    NEAR PTR loc_4_5D2E
loc_4_5C94:
        or     eax, eax
        je     SHORT loc_4_5C9A
        jmp    SHORT loc_4_5CA0
loc_4_5C9A:
        db  08Bh, 081h, 058h, 06Eh, 000h, 000h
loc_4_5CA0:
        mov    dword ptr [ebp - 8ch], eax
        movzx  eax, byte ptr [ebp - 7fh]
        or     byte ptr [edi + eax + 5fh], 10h
        lea    eax, [ebp - 108h]
        push   eax
        call   sub_4_5426
loc_4_5CBB:
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_5D9A
        lea    eax, [ebp - 108h]
        push   eax
        call   sub_4_538B
        add    esp, 4
        sub    ebx, ebx
loc_4_5CD7:
        db  08Ah, 083h, 0A4h, 083h, 000h, 000h
        cmp    al, 0ffh
        je     SHORT loc_4_5D2E
        cmp    byte ptr [ebp - 7fh], al
        jne    SHORT loc_4_5D28
        db  066h, 00Fh, 0B6h, 083h, 098h, 083h, 000h, 000h
        mov    word ptr [ebp - 102h], ax
        movzx  ecx, ax
        mov    eax, dword ptr [ebp - 74h]
        mov    eax, dword ptr [eax + 4ch]
        mov    eax, dword ptr [eax + ecx*4]
        mov    dword ptr [ebp - 90h], eax
        cmp    word ptr [eax], 0
        jbe    SHORT loc_4_5D28
        mov    ax, word ptr [eax + 5]
        and    ah, 88h
        cmp    ah, 88h
        jne    SHORT loc_4_5D28
        lea    eax, [ebp - 108h]
loc_4_5D1F:
        push   eax
        call   sub_4_5400
        add    esp, 4
loc_4_5D28:
        inc    ebx
        cmp    ebx, 8
        jl     SHORT loc_4_5CD7
loc_4_5D2E:
        inc    byte ptr [ebp - 7fh]
        cmp    byte ptr [ebp - 7fh], 9
        jb     NEAR PTR loc_4_5C6C
        mov    ecx, dword ptr [ebp + 8]
        db  039h, 00Dh, 0B0h, 055h, 000h, 000h
        je     SHORT loc_4_5D81
        sub    edi, edi
        db  039h, 03Dh, 0D8h, 066h, 000h, 000h
        jle    SHORT loc_4_5D81
        sub    ecx, ecx
        add    esi, 0d4h
loc_4_5D58:
        mov    edx, 10h
loc_4_5D5D:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_4_5D72
        test   byte ptr [eax + 1fh], dl
loc_4_5D66:
        je     SHORT loc_4_5D72
        mov    ebx, dword ptr [ebp - 74h]
        mov    dword ptr [ecx + ebx + 0d4h], eax
loc_4_5D72:
        add    ecx, 4
        add    esi, 4
        inc    edi
        db  03Bh, 03Dh, 0D8h, 066h, 000h, 000h
        jl     SHORT loc_4_5D5D
loc_4_5D81:
        db  080h, 03Dh, 042h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_5D91
        mov    ecx, dword ptr [ebp - 4]
        or     byte ptr [ecx + 8], 20h
loc_4_5D91:
        sub    eax, eax
loc_4_5D93:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_5D9A:
        push   0
        push   dword ptr [ebp + 8]
        call   sub_4_5F1F
        add    esp, 8
        mov    eax, 8
        jmp    SHORT loc_4_5D93
sub_4_5DAE:
        push   ebp
loc_4_5DAF:
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, 100h
        db  039h, 03Dh, 0D8h, 066h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        jle    SHORT loc_4_5E3C
        sub    ebx, ebx
loc_4_5DC6:
        mov    eax, dword ptr [esi + 94h]
        cmp    dword ptr [eax + edi*4 + 0d4h], ebx
        je     SHORT loc_4_5E33
        mov    eax, dword ptr [eax + 4ch]
        mov    ecx, dword ptr [eax + edi*4]
        db  0A1h, 0A4h, 065h, 000h, 000h
        mov    ax, word ptr [eax]
        cmp    word ptr [ecx + 31h], ax
        jne    SHORT loc_4_5E33
        db  0A1h, 0A4h, 065h, 000h, 000h
        mov    dx, word ptr [ecx + 2fh]
        cmp    word ptr [eax + 2], dx
        jne    SHORT loc_4_5E33
        mov    word ptr [ecx], 1
        mov    eax, dword ptr [esi + 94h]
        push   esi
        mov    eax, dword ptr [eax + edi*4 + 0d4h]
        mov    dword ptr [esi + 74h], eax
        mov    word ptr [esi + 6], di
        mov    dword ptr [esi + 78h], ecx
        db  0E8h, 011h, 0E6h, 0FAh, 0FFh
        add    esp, 4
        cmp    eax, ebx
        jne    SHORT loc_4_5E33
        push   esi
        call   sub_4_7071
        add    esp, 4
        push   esi
        db  0E8h, 0CEh, 0EAh, 0FAh, 0FFh
        add    esp, 4
loc_4_5E33:
        inc    edi
        db  03Bh, 03Dh, 0D8h, 066h, 000h, 000h
        jl     SHORT loc_4_5DC6
loc_4_5E3C:
        mov    dword ptr [esi + 7ch], 0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_5E48:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 90h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], eax
loc_4_5E6A:
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 01Ch, 085h, 0E0h, 066h, 000h, 000h
        or     ebx, ebx
        je     NEAR PTR loc_4_5F11
        mov    dword ptr [ebp - 4], 0
        add    ebx, 4
loc_4_5E86:
        mov    ecx, dword ptr [ebx - 4]
        or     ecx, ecx
        je     SHORT loc_4_5EF2
        or     edi, edi
        je     SHORT loc_4_5E9F
        mov    eax, dword ptr [ebx]
        shr    eax, 10h
        cmp    eax, dword ptr [ebp - 0ch]
        je     SHORT loc_4_5EA6
        or     edi, edi
        jne    SHORT loc_4_5EF2
loc_4_5E9F:
        mov    eax, dword ptr [ebx]
        cmp    dword ptr [esi + 8], eax
        jne    SHORT loc_4_5EF2
loc_4_5EA6:
        or     edi, edi
        je     SHORT loc_4_5EAF
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 8], eax
loc_4_5EAF:
        mov    dword ptr [esi + 74h], ecx
        mov    eax, dword ptr [ebp - 8]
        add    ax, 2
        shl    ax, 8
        add    ax, word ptr [ebp - 4]
        mov    word ptr [esi + 6], ax
        push   esi
        db  0E8h, 060h, 0E5h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_5EF2
        movzx  eax, word ptr [esi + 6]
        push   eax
        push   esi
        db  0E8h, 011h, 0E5h, 0FAh, 0FFh
        add    esp, 8
        push   esi
        call   sub_4_7071
        add    esp, 4
        push   esi
        db  0E8h, 00Fh, 0EAh, 0FAh, 0FFh
        add    esp, 4
loc_4_5EF2:
        add    ebx, 8
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0ffh
        jle    SHORT loc_4_5E86
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], 0ffh
        jle    NEAR PTR loc_4_5E6A
loc_4_5F11:
        mov    dword ptr [esi + 7ch], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_5F1F:
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 0B4h, 055h, 000h, 000h
        add    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 8], 1
        je     NEAR PTR loc_4_6165
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 0ch], eax
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    al, byte ptr [esi + 8]
        and    al, 0feh
        mov    byte ptr [esi + 8], al
        test   al, 10h
        je     SHORT loc_4_5F7C
        and    al, 0efh
        mov    byte ptr [esi + 8], al
        db  0FEh, 00Dh, 00Dh, 06Bh, 000h, 000h
        jne    SHORT loc_4_5F7C
        db  0A1h, 044h, 059h, 000h, 000h
        db  080h, 025h, 00Ch, 06Bh, 000h, 000h, 0FDh
        and    byte ptr [eax + 11h], 0f7h
loc_4_5F7C:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 74h], ecx
        mov    dword ptr [ebp - 70h], esi
        lea    ebx, [esi + 50h]
        mov    edi, 20h
loc_4_5F8D:
        mov    eax, dword ptr [ebx]
        or     eax, eax
        je     SHORT loc_4_5FA2
        push   eax
        call   sub_4_61C3
        add    esp, 4
        mov    dword ptr [ebx], 0
loc_4_5FA2:
        add    ebx, 4
        dec    edi
        jne    SHORT loc_4_5F8D
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_4_5FCC
        push   eax
        db  0E8h, 0AAh, 0D0h, 0FAh, 0FFh
        add    esp, 4
        push   0
        push   dword ptr [esi + 14h]
        db  0E8h, 03Eh, 026h, 001h, 000h
        add    esp, 8
        mov    dword ptr [esi + 14h], 0
loc_4_5FCC:
        mov    eax, dword ptr [esi + 48h]
        or     eax, eax
        je     SHORT loc_4_5FF0
        push   eax
        db  0E8h, 086h, 0D0h, 0FAh, 0FFh
        add    esp, 4
        push   0
        push   dword ptr [esi + 48h]
        db  0E8h, 01Ah, 026h, 001h, 000h
        add    esp, 8
        mov    dword ptr [esi + 48h], 0
loc_4_5FF0:
        push   dword ptr [ebp + 8]
        call   sub_4_248A
        add    esp, 4
        mov    byte ptr [ebp - 7bh], 0
loc_4_5FFF:
        movzx  eax, byte ptr [ebp - 7bh]
loc_4_6003:
        lea    ecx, [eax*4]
        db  08Bh, 081h, 034h, 06Eh, 000h, 000h
        or     eax, eax
        jne    SHORT loc_4_601D
        db  083h, 0B9h, 058h, 06Eh, 000h, 000h, 000h
        je     SHORT loc_4_609A
loc_4_601D:
        mov    edi, dword ptr [ecx + esi + 24h]
        or     edi, edi
        je     SHORT loc_4_609A
        or     eax, eax
        je     SHORT loc_4_602B
        jmp    SHORT loc_4_6031
loc_4_602B:
        db  08Bh, 081h, 058h, 06Eh, 000h, 000h
loc_4_6031:
        mov    dword ptr [ebp - 88h], eax
        sub    ebx, ebx
loc_4_6039:
        db  08Ah, 083h, 0A4h, 083h, 000h, 000h
        cmp    al, 0ffh
        je     SHORT loc_4_607D
        cmp    al, byte ptr [ebp - 7bh]
        jne    SHORT loc_4_6077
        db  066h, 00Fh, 0B6h, 083h, 098h, 083h, 000h, 000h
        mov    word ptr [ebp - 0feh], ax
        mov    ecx, dword ptr [ebp - 70h]
        movzx  eax, byte ptr [ebp - 7bh]
        mov    eax, dword ptr [ecx + eax*4 + 24h]
        mov    dword ptr [ebp - 90h], eax
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_58B2
        add    esp, 4
loc_4_6077:
        inc    ebx
        cmp    ebx, 8
        jl     SHORT loc_4_6039
loc_4_607D:
        mov    eax, dword ptr [edi + 50h]
        or     eax, eax
        je     SHORT loc_4_608F
        push   0
        push   eax
        db  0E8h, 074h, 025h, 001h, 000h
        add    esp, 8
loc_4_608F:
        push   0
        push   edi
        db  0E8h, 069h, 025h, 001h, 000h
        add    esp, 8
loc_4_609A:
        inc    byte ptr [ebp - 7bh]
        cmp    byte ptr [ebp - 7bh], 9
        jb     NEAR PTR loc_4_5FFF
        sub    esi, esi
        db  039h, 035h, 0D8h, 066h, 000h, 000h
        jle    NEAR PTR loc_4_6154
        sub    edi, edi
loc_4_60B7:
        mov    eax, dword ptr [ebp - 70h]
        mov    ecx, dword ptr [edi + eax + 0d4h]
        or     ecx, ecx
        je     SHORT loc_4_6144
        mov    eax, dword ptr [eax + 4ch]
        mov    eax, dword ptr [edi + eax]
        db  08Bh, 01Dh, 088h, 065h, 000h, 000h
        sub    edx, edx
        db  081h, 0FBh, 088h, 065h, 000h, 000h
        je     SHORT loc_4_60F0
loc_4_60DB:
        cmp    dword ptr [edi + ebx + 0d4h], 0
        je     SHORT loc_4_60E6
        inc    edx
loc_4_60E6:
        mov    ebx, dword ptr [ebx]
        db  081h, 0FBh, 088h, 065h, 000h, 000h
        jne    SHORT loc_4_60DB
loc_4_60F0:
        mov    dword ptr [ebp - 90h], ecx
        test   byte ptr [ecx + 1fh], 10h
        jne    SHORT loc_4_6144
        cmp    edx, 1
        jne    SHORT loc_4_6106
        mov    word ptr [eax], 1
loc_4_6106:
        mov    word ptr [ebp - 0feh], si
        mov    dword ptr [ebp - 8ch], eax
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 00Ch, 0E3h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_6144
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_7071
        add    esp, 4
        lea    eax, [ebp - 104h]
        push   eax
loc_4_613C:
        db  0E8h, 0BDh, 0E7h, 0FAh, 0FFh
        add    esp, 4
loc_4_6144:
        add    edi, 4
        inc    esi
        db  03Bh, 035h, 0D8h, 066h, 000h, 000h
        jl     NEAR PTR loc_4_60B7
loc_4_6154:
        push   1
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_5E48
        add    esp, 8
loc_4_6165:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  0C3h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    word ptr [eax + 1ah], 5
        mov    eax, 5
        ret
loc_4_6180:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], 0
        sub    eax, eax
        ret
sub_4_6190:
        push   ebp
        mov    ebp, esp
        push   esi
        db  083h, 03Dh, 074h, 059h, 000h, 000h, 000h
        je     SHORT loc_4_61A1
        sub    eax, eax
        jmp    SHORT loc_4_61C0
loc_4_61A1:
        mov    esi, dword ptr [ebp + 8]
        push   201h
        lea    eax, [esi + 8]
        push   eax
        db  0E8h, 046h, 024h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_61C0
        mov    byte ptr [eax], 1
        mov    word ptr [eax + 2], si
loc_4_61C0:
        pop    esi
        pop    ebp
        ret
sub_4_61C3:
        push   ebp
        mov    ebp, esp
        db  083h, 03Dh, 074h, 059h, 000h, 000h, 000h
        jne    SHORT loc_4_61E9
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     SHORT loc_4_61E9
        mov    cl, byte ptr [eax]
        dec    cl
        mov    byte ptr [eax], cl
        jne    SHORT loc_4_61E9
        push   0
        push   eax
        db  0E8h, 01Ah, 024h, 001h, 000h
        add    esp, 8
loc_4_61E9:
        pop    ebp
        ret
sub_4_61EB:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 10h]
        cmp    ecx, -1
        jne    SHORT loc_4_6206
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
loc_4_6206:
        mov    eax, ecx
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_6220:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 10h]
        cmp    ecx, -1
        jne    SHORT loc_4_623B
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
loc_4_623B:
        mov    eax, ecx
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_4_6260:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
loc_4_6268:
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        jne    SHORT loc_4_6280
        mov    word ptr [ebp - 2], 0
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_4_6375
loc_4_6280:
        db  0E8h, 0E6h, 021h, 001h, 000h
        imul   eax, eax, -3ch
        add    esi, eax
        sub    esi, 12cea600h
        shr    esi, 1
        mov    ecx, 0a8c0h
        mov    eax, esi
        sub    edx, edx
        div    ecx
        mov    edi, edx
        mov    eax, esi
        sub    edx, edx
        div    ecx
        mov    esi, eax
        mov    ecx, 1eh
        mov    eax, edi
        sub    edx, edx
        div    ecx
        mov    word ptr [ebp - 4], dx
        mov    eax, edi
        sub    edx, edx
        div    ecx
        mov    edi, eax
        mov    ecx, 3ch
        sub    edx, edx
        div    ecx
        shl    dx, 5
        or     word ptr [ebp - 4], dx
        mov    eax, edi
        sub    edx, edx
        div    ecx
        shl    ax, 0bh
        or     word ptr [ebp - 4], ax
        mov    ecx, 5b5h
        mov    eax, esi
        sub    edx, edx
        div    ecx
        mov    word ptr [ebp - 2], ax
        mov    eax, esi
        sub    edx, edx
        div    ecx
        mov    ecx, edx
        shl    word ptr [ebp - 2], 2
        cmp    ecx, 3bh
        jne    SHORT loc_4_6310
        mov    ax, word ptr [ebp - 2]
        shl    ax, 9
        add    ax, 5dh
        mov    word ptr [ebp - 2], ax
        jmp    SHORT loc_4_6375
loc_4_6310:
        jbe    SHORT loc_4_6313
        dec    ecx
loc_4_6313:
        cmp    ecx, 16dh
        jb     SHORT loc_4_6337
        mov    esi, 16dh
        mov    eax, ecx
        sub    edx, edx
        div    esi
        mov    edx, eax
        imul   edx, edx, 0fffffe93h
        add    ecx, edx
loc_4_6330:
        inc    word ptr [ebp - 2]
        dec    eax
        jne    SHORT loc_4_6330
loc_4_6337:
        mov    esi, 1
        db  00Fh, 0B7h, 005h, 000h, 06Dh, 000h, 000h
        cmp    eax, ecx
        ja     SHORT loc_4_6357
        db  0B8h, 000h, 06Dh, 000h, 000h
loc_4_634C:
        add    eax, 2
        inc    esi
        movzx  edx, word ptr [eax]
        cmp    edx, ecx
        jbe    SHORT loc_4_634C
loc_4_6357:
        db  00Fh, 0B7h, 004h, 075h, 0FCh, 06Ch, 000h, 000h
        sub    ecx, eax
        shl    word ptr [ebp - 2], 4
        add    word ptr [ebp - 2], si
        inc    cx
        shl    word ptr [ebp - 2], 5
        add    word ptr [ebp - 2], cx
loc_4_6375:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_4_637E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cmp    word ptr [ebp + 0ah], 0
        jne    SHORT loc_4_6398
        cmp    word ptr [ebp + 8], 0
        jne    SHORT loc_4_6398
        sub    eax, eax
        jmp    NEAR PTR loc_4_6429
loc_4_6398:
        mov    ax, word ptr [ebp + 0ah]
        shr    ax, 9
        mov    dx, word ptr [ebp + 0ah]
        movzx  eax, ax
        mov    ecx, eax
        imul   ecx, ecx, 16dh
        shr    eax, 2
        lea    esi, [ecx + eax]
        and    dx, 1e0h
        mov    ax, dx
        shr    ax, 4
        mov    cx, word ptr [ebp + 0ah]
        movzx  eax, ax
        and    ecx, 1fh
        db  00Fh, 0B7h, 080h, 0FCh, 06Ch, 000h, 000h
        add    eax, ecx
        add    esi, eax
        test   byte ptr [ebp + 0bh], 6
        jne    SHORT loc_4_63E3
        cmp    dx, 40h
        ja     SHORT loc_4_63E3
        dec    esi
loc_4_63E3:
        mov    ax, word ptr [ebp + 8]
        shr    ax, 0bh
        movzx  ecx, ax
        mov    ax, word ptr [ebp + 8]
        imul   ecx, ecx, 3ch
        and    ax, 7e0h
        shr    ax, 5
        movzx  eax, ax
        add    ecx, eax
        imul   ecx, ecx, 1eh
        mov    ax, word ptr [ebp + 8]
        and    eax, 1fh
        lea    edi, [ecx + eax]
        db  0E8h, 057h, 020h, 001h, 000h
        add    esi, 0e44h
        imul   esi, esi, 5a0h
        add    eax, esi
        imul   eax, eax, 1eh
        add    eax, edi
        add    eax, eax
loc_4_6429:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_642D:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_4_637E
        add    esp, 4
        push   eax
        call   sub_4_11F5D
        mov    esp, ebp
        pop    ebp
        ret
sub_4_6445:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_11F7F
        add    esp, 8
        push   eax
        call   sub_4_6260
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 24h], 0
        movzx  eax, word ptr [esi + 0d0h]
        shl    eax, 4
        movzx  ecx, word ptr [esi + 98h]
        lea    edi, [eax + ecx]
        or     edi, edi
        je     NEAR PTR loc_4_6759
        mov    dword ptr [esi + 78h], edi
        movzx  eax, byte ptr [esi + 88h]
        cmp    eax, 8bh
        jl     NEAR PTR loc_4_653F
        cmp    eax, 8ch
        jle    SHORT loc_4_64C6
        cmp    eax, 8dh
        jl     NEAR PTR loc_4_653F
        cmp    eax, 8eh
        jle    SHORT loc_4_64D7
        cmp    eax, 0a4h
        je     SHORT loc_4_64FB
        jmp    SHORT loc_4_653F
loc_4_64C6:
        mov    word ptr [esi + 18h], 1
        mov    al, byte ptr [esi + 0b4h]
        mov    byte ptr [esi + 4], al
        jmp    SHORT loc_4_6539
loc_4_64D7:
        cmp    byte ptr [esi + 0b5h], 0
        mov    word ptr [esi + 18h], 10h
        jne    SHORT loc_4_64EC
        mov    word ptr [esi + 18h], 12h
loc_4_64EC:
        movzx  eax, byte ptr [esi + 0b4h]
        mov    dword ptr [esi], eax
        mov    byte ptr [esi + 4], 2
        jmp    SHORT loc_4_653F
loc_4_64FB:
        db  0A1h, 0A0h, 065h, 000h, 000h
        mov    ax, word ptr [eax + 56h]
        mov    word ptr [esi + 18h], ax
        db  0A1h, 0A0h, 065h, 000h, 000h
        test   byte ptr [eax + 58h], 2
        je     SHORT loc_4_651A
        or     byte ptr [esi + 8ah], 40h
loc_4_651A:
        db  0A1h, 0A0h, 065h, 000h, 000h
        test   byte ptr [eax + 56h], 1
        jne    SHORT loc_4_6539
        mov    eax, dword ptr [esi + 78h]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [esi + 4], al
        movzx  eax, byte ptr [esi + 0b4h]
        mov    dword ptr [esi], eax
        jmp    SHORT loc_4_653F
loc_4_6539:
        mov    dword ptr [esi], 0
loc_4_653F:
        mov    eax, dword ptr [esi + 78h]
        test   byte ptr [eax + 3], 80h
        je     SHORT loc_4_654C
        mov    byte ptr [esi + 4], 0ffh
loc_4_654C:
        db  0A1h, 0A0h, 065h, 000h, 000h
        movzx  ecx, word ptr [eax + 14h]
        db  003h, 00Dh, 0B0h, 065h, 000h, 000h
        cmp    word ptr [ecx], 504ch
        jne    SHORT loc_4_65C1
        cmp    byte ptr [ecx + 2], 54h
        jne    SHORT loc_4_65C1
        cmp    byte ptr [ecx + 4], 0
        jne    SHORT loc_4_65C1
        movsx  eax, byte ptr [ecx + 3]
        db  08Bh, 015h, 044h, 059h, 000h, 000h
        test   byte ptr [eax + edx + 2eh], 10h
        je     SHORT loc_4_65C1
        db  0A1h, 0A0h, 065h, 000h, 000h
        mov    ax, word ptr [eax + 0ch]
        mov    word ptr [esi + 6], ax
        movsx  eax, byte ptr [ecx + 3]
        sub    eax, 31h
        push   eax
        push   esi
        call   sub_4_6EAD
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_6759
        cmp    byte ptr [esi + 88h], 0a4h
        jne    NEAR PTR loc_4_6759
        push   1
        call   sub_4_21FE
        add    esp, 4
        jmp    NEAR PTR loc_4_6759
loc_4_65C1:
        push   0
        push   esi
        call   sub_4_912E
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_6759
        test   byte ptr [esi + 18h], 10h
        je     SHORT loc_4_65FC
        mov    ecx, dword ptr [esi + 7ch]
        mov    eax, 0d0h
        test   byte ptr [ecx + 1bh], 8
        je     SHORT loc_4_65ED
        mov    eax, 58h
loc_4_65ED:
        test   dword ptr [esi], eax
        je     SHORT loc_4_65FC
        mov    word ptr [esi + 1ah], 5
        jmp    NEAR PTR loc_4_6759
loc_4_65FC:
        db  0A1h, 0A0h, 065h, 000h, 000h
        mov    ax, word ptr [eax + 0ch]
        or     byte ptr [esi + 8ah], 20h
        mov    word ptr [esi + 6], ax
        db  0A1h, 0B8h, 055h, 000h, 000h
        cmp    dword ptr [esi + 80h], eax
        jne    SHORT loc_4_6623
        or     word ptr [esi + 18h], 80h
loc_4_6623:
        push   0
        push   esi
        db  0E8h, 025h, 0D9h, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_6759
        and    word ptr [esi + 18h], 0ff7fh
        sub    ebx, ebx
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [esi + 14h], ebx
        test   byte ptr [esi + 18h], 10h
        je     SHORT loc_4_66AE
        push   1
        push   -24h
        push   -1
        push   esi
        db  0E8h, 00Eh, 005h, 0FBh, 0FFh
IFSMgr_Win32MapExtendedHandleToSFT:
        add    esp, 10h
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_4_66AE
        mov    ax, word ptr [esi + 18h]
        test   al, 3
        je     SHORT loc_4_66A0
        movzx  ecx, ax
        mov    dword ptr [ebp - 8], ecx
        and    ax, 0ffefh
        mov    word ptr [esi + 18h], ax
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   24h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 18h]
        db  0E8h, 005h, 0D5h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_66D3
        mov    cx, word ptr [ebp - 8]
        mov    word ptr [esi + 18h], cx
        mov    word ptr [esi + 1ah], 0
loc_4_66A0:
        push   4
        push   -24h
        push   ebx
        push   esi
        db  0E8h, 0BCh, 004h, 0FBh, 0FFh
        add    esp, 10h
loc_4_66AE:
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   24h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 18h]
        db  0E8h, 0CFh, 0D4h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_66D3
        push   esi
        db  0E8h, 0C9h, 0DBh, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_4_6741
loc_4_66D3:
        mov    eax, dword ptr [esi + 74h]
        mov    word ptr [eax + 20h], 1
        mov    ecx, dword ptr [esi + 74h]
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ecx + 0ch], eax
        movzx  cx, byte ptr [esi + 4]
        mov    ax, word ptr [edi + 2]
        xor    cx, ax
        and    cx, 3
        xor    cx, ax
        mov    word ptr [edi + 2], cx
        mov    al, byte ptr [esi]
        mov    byte ptr [edi + 4], al
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [edi + 11h], eax
        mov    ax, word ptr [esi + 28h]
        mov    word ptr [edi + 0dh], ax
        cmp    byte ptr [esi + 88h], 0a4h
        jne    SHORT loc_4_672A
        mov    ax, word ptr [esi + 18h]
        and    eax, 0ffh
        push   eax
        call   sub_4_21FE
        add    esp, 4
loc_4_672A:
        mov    ax, word ptr [esi + 2ah]
        mov    word ptr [edi + 0fh], ax
        push   1
        push   24h
        push   -1
        push   esi
        db  0E8h, 029h, 004h, 0FBh, 0FFh
        add    esp, 10h
loc_4_6741:
        or     ebx, ebx
        je     SHORT loc_4_6753
        push   5
        push   24h
        push   ebx
        push   esi
        db  0E8h, 017h, 004h, 0FBh, 0FFh
        add    esp, 10h
loc_4_6753:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 14h], eax
loc_4_6759:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_4_6A71
        add    esp, 4
        cmp    eax, ebx
        jne    NEAR PTR loc_4_6802
        test   byte ptr [edi + 18h], 10h
        je     SHORT loc_4_67B9
        test   byte ptr [edi], 8
        je     SHORT loc_4_67B9
        push   edi
        db  0E8h, 042h, 0D0h, 0FAh, 0FFh
        add    esp, 4
        cmp    eax, ebx
        jne    SHORT loc_4_6802
        mov    esi, dword ptr [edi + 14h]
        push   edi
        call   sub_4_2EFB
        add    esp, 4
        cmp    eax, ebx
        jne    SHORT loc_4_6802
        mov    eax, dword ptr [edi + 14h]
        cmp    esi, eax
        je     SHORT loc_4_67B0
        mov    ebx, eax
loc_4_67B0:
        and    byte ptr [edi + 8bh], 0bfh
        jmp    SHORT loc_4_67BC
loc_4_67B9:
        mov    esi, dword ptr [ebp - 4]
loc_4_67BC:
        push   0
        push   edi
        call   sub_4_91A0
        add    esp, 8
        or     ebx, ebx
        je     SHORT loc_4_67D9
        push   0
        push   ebx
        db  0E8h, 02Dh, 01Eh, 001h, 000h
        add    esp, 8
        mov    dword ptr [edi + 14h], esi
loc_4_67D9:
        mov    ax, word ptr [edi + 1ah]
        or     ax, ax
        je     SHORT loc_4_67F9
        cmp    ax, 0ffc0h
        jne    SHORT loc_4_6802
        test   byte ptr [edi + 2], 1
        je     SHORT loc_4_6802
IFSMgr_Win32MapSFTToExtendedHandle:
        and    word ptr [edi + 0a8h], 0efffh
        jmp    SHORT loc_4_6802
loc_4_67F9:
        push   edi
        call   sub_4_6809
        add    esp, 4
loc_4_6802:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_6809:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 18h]
        test   dl, 10h
        je     SHORT loc_4_6840
        mov    ecx, dword ptr [esi + 7ch]
        mov    eax, 0d0h
        test   byte ptr [ecx + 1bh], 8
        je     SHORT loc_4_6831
        mov    eax, 58h
loc_4_6831:
        test   dword ptr [esi], eax
        je     SHORT loc_4_6840
        mov    word ptr [esi + 1ah], 5
        jmp    NEAR PTR loc_4_6A6A
loc_4_6840:
        db  0A1h, 0B8h, 055h, 000h, 000h
        cmp    dword ptr [esi + 80h], eax
        jne    SHORT loc_4_6856
        or     dx, 80h
        mov    word ptr [esi + 18h], dx
loc_4_6856:
        mov    al, byte ptr [esi + 8bh]
        test   al, 80h
        je     SHORT loc_4_687D
        test   al, 4
        jne    SHORT loc_4_687D
        cmp    word ptr [esi + 0b4h], 71a9h
        jne    SHORT loc_4_6876
        mov    eax, 18h
        jmp    SHORT loc_4_6882
loc_4_6876:
        mov    eax, 20h
        jmp    SHORT loc_4_6882
loc_4_687D:
        mov    eax, 8
loc_4_6882:
        push   eax
        push   esi
        db  0E8h, 0C7h, 0D6h, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_6A6A
        push   esi
        db  0E8h, 0F8h, 0DAh, 0FAh, 0FFh
        add    esp, 4
        mov    edi, eax
        cmp    edi, -1
        jne    SHORT loc_4_68B8
        mov    word ptr [esi + 1ah], 4
        push   esi
        db  0E8h, 0E7h, 0D9h, 0FAh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_4_6A6A
loc_4_68B8:
        mov    eax, dword ptr [esi + 74h]
        mov    cx, word ptr [esi + 18h]
loc_4_68BF:
        and    cx, 0fb00h
        or     word ptr [eax + 22h], cx
        and    word ptr [esi + 18h], 0ff7fh
        sub    ebx, ebx
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [esi + 14h], ebx
        test   byte ptr [esi + 18h], 10h
        je     SHORT loc_4_6940
        push   1
        push   -24h
        push   -1
        push   esi
        db  0E8h, 07Ch, 002h, 0FBh, 0FFh
        add    esp, 10h
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_4_6940
        mov    ax, word ptr [esi + 18h]
        test   al, 3
        je     SHORT loc_4_6932
        movzx  ecx, ax
        mov    dword ptr [ebp - 8], ecx
        and    ax, 0ffefh
        mov    word ptr [esi + 18h], ax
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   24h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 18h]
        db  0E8h, 073h, 0D2h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_699B
        mov    cx, word ptr [ebp - 8]
        mov    word ptr [esi + 18h], cx
        mov    word ptr [esi + 1ah], 0
loc_4_6932:
        push   4
        push   -24h
        push   ebx
        push   esi
        db  0E8h, 02Ah, 002h, 0FBh, 0FFh
        add    esp, 10h
loc_4_6940:
        lea    eax, [esi + 7ch]
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   24h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 18h]
        db  0E8h, 03Ah, 0D2h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_699B
        push   edi
        push   esi
        db  0E8h, 08Ah, 0DAh, 0FAh, 0FFh
        add    esp, 8
        push   esi
        db  0E8h, 02Ah, 0D9h, 0FAh, 0FFh
        add    esp, 4
        cmp    word ptr [esi + 1ah], 70bh
        jne    NEAR PTR loc_4_6A52
        push   esi
        db  0E8h, 07Ch, 0DFh, 0FAh, 0FFh
        add    esp, 4
        push   3
        push   esi
        call   sub_4_F4CE
        add    esp, 8
        mov    word ptr [esi + 1ah], 0ffc0h
        jmp    NEAR PTR loc_4_6A52
loc_4_699B:
        mov    eax, dword ptr [esi + 74h]
        mov    word ptr [eax + 20h], 1
        mov    ecx, dword ptr [esi + 74h]
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ecx + 0ch], eax
        cmp    byte ptr [esi + 0b5h], 6ch
        je     SHORT loc_4_69C1
        cmp    word ptr [esi + 0b4h], 71a9h
        jne    SHORT loc_4_69D0
loc_4_69C1:
        mov    ax, word ptr [esi + 18h]
        and    ax, 0ffh
        mov    word ptr [esi + 0b0h], ax
loc_4_69D0:
        mov    ecx, dword ptr [esi + 78h]
        or     ecx, ecx
        je     SHORT loc_4_69F2
        mov    al, byte ptr [esi]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ecx + 11h], eax
        mov    ax, word ptr [esi + 28h]
        mov    word ptr [ecx + 0dh], ax
        mov    ax, word ptr [esi + 2ah]
        mov    word ptr [ecx + 0fh], ax
loc_4_69F2:
        push   1
        push   24h
        push   -1
        push   esi
        db  0E8h, 069h, 001h, 0FBh, 0FFh
        add    esp, 10h
        mov    ecx, dword ptr [esi + 7ch]
        cmp    byte ptr [ecx + 30h], 1
        jbe    SHORT loc_4_6A20
        mov    eax, dword ptr [ecx + 28h]
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_4_6A20
        mov    eax, dword ptr [esi + 74h]
        test   byte ptr [eax + 1fh], 40h
        je     SHORT loc_4_6A20
        or     word ptr [ecx + 32h], 4
loc_4_6A20:
        mov    al, byte ptr [esi + 8bh]
        test   al, 80h
        je     SHORT loc_4_6A4B
        test   al, 4
        jne    SHORT loc_4_6A4B
        mov    ecx, dword ptr [esi + 74h]
        mov    al, byte ptr [ecx + 1fh]
        add    ecx, 1fh
        test   al, 1
        je     SHORT loc_4_6A4B
        or     al, 10h
        mov    byte ptr [ecx], al
        push   dword ptr [esi + 74h]
        push   esi
        call   sub_4_46B0
        add    esp, 8
loc_4_6A4B:
        mov    word ptr [esi + 0b4h], di
loc_4_6A52:
        or     ebx, ebx
        je     SHORT loc_4_6A64
        push   5
        push   24h
        push   ebx
        push   esi
        db  0E8h, 006h, 001h, 0FBh, 0FFh
        add    esp, 10h
loc_4_6A64:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 14h], eax
loc_4_6A6A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_6A71:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 24h], 0
        and    dword ptr [esi], 0ff00ffffh
        movzx  eax, byte ptr [esi + 0b5h]
        cmp    eax, 3ch
        je     SHORT loc_4_6AB3
        cmp    eax, 3dh
        je     SHORT loc_4_6AC2
        cmp    eax, 5bh
        je     SHORT loc_4_6ADD
        cmp    eax, 6ch
        je     SHORT loc_4_6B0F
        cmp    eax, 71h
        je     SHORT loc_4_6AEC
loc_4_6AA5:
        mov    word ptr [esi + 1ah], 1
        mov    eax, 1
loc_4_6AB0:
        pop    esi
        pop    ebp
        ret
loc_4_6AB3:
        mov    word ptr [esi + 18h], 12h
        mov    byte ptr [esi + 4], 2
        jmp    NEAR PTR loc_4_6BB4
loc_4_6AC2:
        mov    al, byte ptr [esi + 0b4h]
        and    ax, 80h
        inc    ax
        mov    word ptr [esi + 18h], ax
        mov    dword ptr [esi], 0
        jmp    NEAR PTR loc_4_6BB4
loc_4_6ADD:
        mov    word ptr [esi + 18h], 10h
        mov    byte ptr [esi + 4], 2
        jmp    NEAR PTR loc_4_6BB4
loc_4_6AEC:
        cmp    byte ptr [esi + 0b4h], 0a9h
        jne    SHORT loc_4_6AA5
        mov    al, byte ptr [esi + 8bh]
        test   al, 80h
        je     SHORT loc_4_6AA5
        and    al, 0bfh
        mov    byte ptr [esi + 8bh], al
        or     al, 20h
        mov    byte ptr [esi + 8bh], al
loc_4_6B0F:
        test   byte ptr [esi + 0a9h], 20h
        je     SHORT loc_4_6B1F
        or     byte ptr [esi + 8ah], 40h
loc_4_6B1F:
        mov    al, byte ptr [esi + 0a8h]
        mov    byte ptr [esi + 4], al
        movzx  cx, byte ptr [esi + 0ach]
        mov    word ptr [esi + 18h], cx
        test   cl, 0ech
        jne    NEAR PTR loc_4_6AA5
        mov    ax, cx
        and    al, 3
        cmp    al, 2
        ja     NEAR PTR loc_4_6AA5
        or     cx, cx
        je     NEAR PTR loc_4_6AA5
        test   byte ptr [esi + 8bh], 40h
        mov    ax, word ptr [esi + 0a8h]
        je     SHORT loc_4_6B69
        and    ax, 4780h
        jmp    SHORT loc_4_6B6D
loc_4_6B69:
        and    ax, 4080h
loc_4_6B6D:
        or     ax, cx
IFSMgr_ChangeDir:
        test   byte ptr [esi + 0a9h], 10h
        mov    word ptr [esi + 18h], ax
        je     SHORT loc_4_6B83
        or     dword ptr [esi], 10000h
loc_4_6B83:
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_6B94
        mov    eax, dword ptr [esi + 9ch]
        jmp    SHORT loc_4_6BA1
loc_4_6B94:
        push   3c04h
        call   sub_4_2154
        add    esp, 4
loc_4_6BA1:
        mov    dword ptr [esi + 14h], eax
        test   byte ptr [esi + 19h], 4
        je     SHORT loc_4_6BB4
        movzx  eax, word ptr [esi + 98h]
        mov    dword ptr [esi + 24h], eax
loc_4_6BB4:
        test   byte ptr [esi + 8bh], 80h
        mov    al, byte ptr [esi + 4]
        je     SHORT loc_4_6BDA
        and    al, 70h
        cmp    al, 50h
        je     SHORT loc_4_6BCA
        cmp    al, 60h
        jne    SHORT loc_4_6BE8
loc_4_6BCA:
        mov    word ptr [esi + 1ah], 0ch
        mov    eax, 0ch
        jmp    NEAR PTR loc_4_6AB0
loc_4_6BDA:
        mov    cl, al
        and    cl, 70h
        cmp    cl, 40h
        ja     SHORT loc_4_6BEF
        test   al, 8
        jne    SHORT loc_4_6BEF
loc_4_6BE8:
        sub    eax, eax
        jmp    NEAR PTR loc_4_6AB0
loc_4_6BEF:
        mov    word ptr [esi + 1ah], 0ch
        mov    eax, 0ch
        jmp    NEAR PTR loc_4_6AB0
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_6A71
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_6D4A
        movzx  edi, byte ptr [esi + 88h]
        mov    eax, dword ptr [esi + 94h]
        sub    edi, 0beh
        mov    eax, dword ptr [eax + edi*4 + 24h]
        cmp    dword ptr [eax + 4ch], 0ffffh
        jne    SHORT loc_4_6C44
        mov    word ptr [esi + 1ah], 15h
        jmp    NEAR PTR loc_4_6D4A
loc_4_6C44:
        db  08Bh, 004h, 0BDh, 034h, 06Eh, 000h, 000h
        mov    dword ptr [esi + 7ch], eax
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 1ch], eax
        push   esi
        db  0E8h, 021h, 0E1h, 0FAh, 0FFh
        add    esp, 4
        mov    byte ptr [esi + 89h], 80h
        db  0A1h, 0B0h, 055h, 000h, 000h
        cmp    dword ptr [esi + 90h], eax
        jne    SHORT loc_4_6C9D
        mov    word ptr [esi + 18h], 12h
        db  0C7h, 046h, 00Ch, 018h, 06Dh, 000h, 000h
        mov    byte ptr [esi + 4], 21h
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 24h], 0
        push   esi
        call   sub_4_6809
        add    esp, 4
        jmp    NEAR PTR loc_4_6D4A
loc_4_6C9D:
        push   esi
        db  0E8h, 0EDh, 0C9h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_6CAF
        jmp    NEAR PTR loc_4_6D44
loc_4_6CAF:
        mov    ebx, dword ptr [esi + 78h]
        push   2
        mov    word ptr [ebx], 1
        movzx  ax, byte ptr [esi + 4]
        mov    word ptr [ebx + 2], ax
        mov    eax, dword ptr [esi + 94h]
        push   esi
        mov    eax, dword ptr [eax + edi*4 + 24h]
        mov    dword ptr [esi + 74h], eax
        db  0E8h, 07Bh, 0D2h, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_6D39
        push   esi
        db  0E8h, 0B0h, 0D6h, 0FAh, 0FFh
        add    esp, 4
        cmp    eax, -1
        jne    SHORT loc_4_6CFB
        mov    word ptr [esi + 1ah], 4
        push   esi
        db  0E8h, 0A1h, 0D5h, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_4_6D4A
loc_4_6CFB:
        mov    cx, word ptr [ebx + 5]
        or     cx, 880h
        mov    word ptr [ebx + 5], cx
        test   byte ptr [esi + 18h], 80h
        je     SHORT loc_4_6D17
        or     cx, 1000h
        mov    word ptr [ebx + 5], cx
loc_4_6D17:
        cmp    byte ptr [esi + 0b5h], 6ch
        jne    SHORT loc_4_6D30
        mov    cx, word ptr [esi + 18h]
        and    cx, 7fffh
        mov    word ptr [esi + 0b0h], cx
loc_4_6D30:
        mov    word ptr [esi + 0b4h], ax
        jmp    SHORT loc_4_6D4A
loc_4_6D39:
        and    word ptr [ebx + 5], 77ffh
        mov    word ptr [ebx], 0
loc_4_6D44:
        mov    word ptr [esi + 1ah], 4
loc_4_6D4A:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    edi, dword ptr [esi]
        mov    bx, word ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ebp - 4], eax
        call   sub_4_6D9F
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_6D7A
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_4_6D98
loc_4_6D7A:
        mov    word ptr [esi + 18h], bx
        mov    dword ptr [esi], edi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 14h], eax
        push   0
        push   esi
        push   1
        db  068h, 0C0h, 0F6h, 000h, 000h
        db  0E8h, 0FBh, 0CDh, 0FAh, 0FFh
        add    esp, 10h
loc_4_6D98:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_6D9F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 20h]
        cmp    dword ptr [edi + 4ch], 0ffffh
        jne    SHORT loc_4_6DCC
        test   byte ptr [esi + 18h], 80h
        jne    SHORT loc_4_6DCC
        mov    word ptr [esi + 1ah], 15h
        mov    eax, 15h
        jmp    NEAR PTR loc_4_6EA7
loc_4_6DCC:
        lea    eax, [ebp - 0ch]
        mov    dword ptr [esi + 10h], eax
        mov    word ptr [esi + 18h], 12h
        db  0C7h, 046h, 00Ch, 018h, 06Dh, 000h, 000h
        mov    byte ptr [esi + 4], 21h
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [edi + 18h], 0
        movzx  ax, byte ptr [edi + 59h]
        mov    word ptr [esi + 28h], ax
        movzx  ax, byte ptr [edi + 5ah]
        mov    word ptr [esi + 2ah], ax
        push   0
        push   esi
        mov    eax, dword ptr [edi + 10h]
        push   24h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 18h]
        db  0E8h, 074h, 0CDh, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_6E25
        jmp    SHORT loc_4_6EA3
loc_4_6E25:
        mov    eax, dword ptr [esi + 7ch]
        push   esi
        inc    word ptr [eax + 16h]
        db  0E8h, 049h, 0DFh, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [edi + 48h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [edi + 40h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [edi + 44h], eax
        db  0C7h, 047h, 004h, 0C0h, 0F6h, 000h, 000h
        db  0C7h, 047h, 008h, 054h, 06Dh, 000h, 000h
        mov    dword ptr [esi + 24h], 0
        cmp    byte ptr [edi + 59h], 0
        jbe    SHORT loc_4_6E94
        mov    eax, dword ptr [edi + 50h]
        mov    dword ptr [esi + 14h], eax
        movzx  eax, byte ptr [edi + 59h]
        mov    dword ptr [esi], eax
        push   0
        push   esi
        push   1
        push   dword ptr [edi + 40h]
        db  0E8h, 012h, 0CDh, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jge    SHORT loc_4_6E8E
        push   esi
        db  0E8h, 0C5h, 0DFh, 0FAh, 0FFh
        add    esp, 4
loc_4_6E8E:
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [edi + 18h], eax
loc_4_6E94:
        mov    dword ptr [esi + 20h], edi
        cmp    dword ptr [edi + 4ch], 0
        jne    SHORT loc_4_6EA3
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [edi + 4ch], eax
loc_4_6EA3:
        movsx  eax, word ptr [esi + 1ah]
loc_4_6EA7:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_4_6EAD:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        lea    edx, [eax*4]
        mov    edi, dword ptr [esi + 78h]
        mov    eax, dword ptr [esi + 94h]
        mov    ecx, dword ptr [edx + eax + 24h]
        mov    dword ptr [ebp - 8], edx
        cmp    dword ptr [ecx + 4ch], 0ffffh
        jne    SHORT loc_4_6EEC
        mov    word ptr [esi + 1ah], 15h
        mov    eax, 15h
        jmp    NEAR PTR loc_4_6F89
loc_4_6EEC:
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 74h], ecx
        db  08Bh, 082h, 034h, 06Eh, 000h, 000h
        or     eax, eax
        mov    dword ptr [esi + 7ch], eax
        jne    SHORT loc_4_6F08
        db  08Bh, 082h, 058h, 06Eh, 000h, 000h
        mov    dword ptr [esi + 7ch], eax
loc_4_6F08:
        db  081h, 079h, 004h, 04Fh, 04Dh, 000h, 000h
        je     SHORT loc_4_6F4D
        mov    ebx, dword ptr [esi + 8]
        mov    al, byte ptr [esi + 5]
        mov    byte ptr [ebp - 1], al
        mov    eax, dword ptr [ecx + 4ch]
        mov    dword ptr [esi + 8], eax
        db  0A0h, 0ECh, 06Ah, 000h, 000h
        mov    byte ptr [esi + 5], al
        push   ecx
        push   esi
        call   sub_4_11886
        add    esp, 8
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_4_6F90
        add    esp, 8
        mov    word ptr [esi + 1ah], 0
        mov    dword ptr [esi + 8], ebx
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 5], al
loc_4_6F4D:
        mov    byte ptr [esi + 89h], 80h
        push   2
        push   esi
        db  0E8h, 0F4h, 0CFh, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_6F7E
        mov    edx, dword ptr [ebp - 8]
        db  08Bh, 082h, 0E4h, 06Fh, 000h, 000h
        mov    dword ptr [edi + 7], eax
        or     word ptr [edi + 5], 880h
        mov    eax, dword ptr [esi + 74h]
        mov    word ptr [eax + 3ch], 0
loc_4_6F7E:
        mov    dword ptr [esi + 7ch], 0
        movsx  eax, word ptr [esi + 1ah]
loc_4_6F89:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_6F90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    ecx, dword ptr [esi + 94h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + eax*4 + 24h]
        mov    dword ptr [esi + 74h], eax
        db  0E8h, 07Dh, 0D4h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_6FBB
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_4_6FE8
loc_4_6FBB:
        mov    byte ptr [esi + 4], 2
        mov    word ptr [esi + 18h], 0
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   0bh
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 8]
        db  0E8h, 0B8h, 0CBh, 0FAh, 0FFh
        add    esp, 10h
        mov    edi, eax
        push   esi
        db  0E8h, 01Bh, 0D9h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, edi
loc_4_6FE8:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_6FEC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_C339
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_704E
        cmp    word ptr [esi + 6], 200h
        jae    SHORT loc_4_7034
        mov    eax, dword ptr [esi + 74h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [esi + 78h], eax
        or     eax, eax
        jne    SHORT loc_4_7034
        mov    eax, dword ptr [esi + 94h]
        mov    eax, dword ptr [eax + 4ch]
        movzx  ecx, word ptr [esi + 6]
        mov    eax, dword ptr [eax + ecx*4]
        or     eax, eax
        mov    dword ptr [esi + 78h], eax
        jne    SHORT loc_4_7034
        mov    word ptr [esi + 1ah], 6
        jmp    SHORT loc_4_704E
loc_4_7034:
        movzx  eax, word ptr [esi + 0a8h]
        push   eax
        push   esi
        db  0E8h, 0ACh, 0D3h, 0FAh, 0FFh
        add    esp, 8
        push   esi
        call   sub_4_7071
        add    esp, 4
loc_4_704E:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_C3DC
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_706E
        push   esi
        call   sub_4_7071
        add    esp, 4
loc_4_706E:
        pop    esi
        pop    ebp
        ret
sub_4_7071:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, 2
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], 0
        mov    esi, dword ptr [edi + 74h]
        mov    eax, dword ptr [edi + 78h]
        mov    dword ptr [ebp - 8], eax
        cmp    word ptr [edi + 6], 200h
        jb     SHORT loc_4_70BC
        mov    ax, word ptr [esi + 20h]
        cmp    ax, 1
        je     SHORT loc_4_70FD
        dec    ax
        mov    word ptr [esi + 20h], ax
        push   edi
        call   sub_4_C53D
        add    esp, 4
        cmp    eax, 1
        sbb    ebx, ebx
        neg    ebx
        jmp    SHORT loc_4_70FD
loc_4_70BC:
        mov    eax, dword ptr [ebp - 8]
        mov    cx, word ptr [eax]
        cmp    cx, 1
        je     SHORT loc_4_70DF
        dec    cx
        mov    word ptr [eax], cx
        push   edi
        call   sub_4_C4F0
        add    esp, 4
        cmp    eax, 1
        sbb    ebx, ebx
        neg    ebx
        jmp    SHORT loc_4_70FD
loc_4_70DF:
        test   byte ptr [esi + 1fh], 2
        je     SHORT loc_4_70FD
        mov    ax, word ptr [esi + 20h]
        cmp    ax, 1
        je     SHORT loc_4_70FD
        dec    ax
        mov    word ptr [esi + 20h], ax
        mov    ebx, 1
        mov    dword ptr [ebp - 0ch], ebx
loc_4_70FD:
        mov    byte ptr [edi + 4], bl
        mov    word ptr [edi + 18h], 0
        mov    ax, word ptr [edi + 6]
        mov    word ptr [ebp - 2], ax
        mov    ax, word ptr [esi + 30h]
        mov    word ptr [edi + 6], ax
        push   ebx
        push   edi
        db  0E8h, 0DCh, 0CBh, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_713A
        push   0
        push   edi
        mov    eax, dword ptr [edi + 74h]
        push   0bh
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 8]
        db  0E8h, 059h, 0CAh, 0FAh, 0FFh
        add    esp, 10h
loc_4_713A:
        push   edi
        db  0E8h, 011h, 0CCh, 0FAh, 0FFh
        add    esp, 4
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [edi + 6], ax
        or     ebx, ebx
        je     SHORT loc_4_715E
        push   1
        push   0bh
        push   -1
        push   edi
        db  0E8h, 00Ch, 0FAh, 0FAh, 0FFh
        add    esp, 10h
loc_4_715E:
        cmp    ebx, 2
        jne    SHORT loc_4_716E
        push   edi
        db  0E8h, 02Eh, 0D1h, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_4_71BE
loc_4_716E:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_4_71BE
        mov    eax, dword ptr [ebp - 8]
        movzx  ecx, word ptr [eax + 1bh]
        db  0A1h, 044h, 059h, 000h, 000h
        and    byte ptr [ecx + eax + 2eh], 0fbh
        mov    eax, dword ptr [edi + 94h]
        mov    dword ptr [eax + ecx*4 + 0d4h], 0
        mov    eax, dword ptr [ebp - 8]
        and    word ptr [eax + 5], 77ffh
        test   byte ptr [edi + 8ah], 20h
        jne    SHORT loc_4_71AD
        mov    word ptr [eax], 0
loc_4_71AD:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 7], 0
        mov    dword ptr [esi + 14h], 0
loc_4_71BE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_71C5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 74h]
        cmp    dword ptr [edi + 0ch], edi
        je     SHORT loc_4_720A
        mov    eax, dword ptr [esi + 7ch]
        lea    edx, [edi + 2ch]
        dec    word ptr [eax + 16h]
        mov    eax, dword ptr [edi + 28h]
        or     eax, eax
        je     SHORT loc_4_71ED
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax + 2ch], ecx
        jmp    SHORT loc_4_71F5
loc_4_71ED:
        mov    ecx, dword ptr [edi + 10h]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx + 24h], eax
loc_4_71F5:
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     SHORT loc_4_7201
        mov    ecx, dword ptr [edi + 28h]
        mov    dword ptr [eax + 28h], ecx
loc_4_7201:
        push   esi
        db  0E8h, 0F7h, 0D6h, 0FAh, 0FFh
        add    esp, 4
loc_4_720A:
        db  0C7h, 046h, 07Ch, 0DCh, 06Dh, 000h, 000h
        push   6
        push   esi
        db  0E8h, 037h, 0CDh, 0FAh, 0FFh
        add    esp, 8
        db  0C7h, 007h, 06Dh, 041h, 000h, 000h
        db  0C7h, 047h, 004h, 06Dh, 041h, 000h, 000h
        db  0C7h, 047h, 008h, 078h, 06Dh, 000h, 000h
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_7234:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        db  0A1h, 09Ch, 065h, 000h, 000h
        cmp    word ptr [ecx + 1ah], 0eah
        jne    SHORT loc_4_7270
        mov    word ptr [eax + 4], 0eah
        db  0A1h, 09Ch, 065h, 000h, 000h
        mov    byte ptr [eax + 3], 3
        db  0A1h, 09Ch, 065h, 000h, 000h
        mov    byte ptr [eax + 7], 2
        db  0A1h, 09Ch, 065h, 000h, 000h
        mov    byte ptr [eax + 6], 1
        mov    word ptr [ecx + 1ah], 0
        jmp    SHORT loc_4_7291
loc_4_7270:
        mov    word ptr [eax + 4], 0
        db  0A1h, 09Ch, 065h, 000h, 000h
        mov    byte ptr [eax + 3], 0
        db  0A1h, 09Ch, 065h, 000h, 000h
        mov    byte ptr [eax + 7], 0
        db  0A1h, 09Ch, 065h, 000h, 000h
        mov    byte ptr [eax + 6], 0
loc_4_7291:
        movsx  eax, word ptr [ecx + 1ah]
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 1]
        push   eax
        push   47h
        push   219h
        push   0
        push   0
        db  0E8h, 03Eh, 013h, 001h, 000h
        add    esp, 14h
        push   esi
        push   7530h
        db  068h, 0ECh, 001h, 001h, 000h
        db  0E8h, 04Fh, 013h, 001h, 000h
        pop    esi
        pop    ebp
        ret
sub_4_72C8:
        push   ebp
        mov    ebp, esp
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 8]
        shl    edx, cl
        db  0A1h, 024h, 06Eh, 000h, 000h
        test   eax, edx
        jne    SHORT loc_4_72F5
        or     eax, edx
        db  0A3h, 024h, 06Eh, 000h, 000h
        push   0
        push   ecx
        db  068h, 097h, 052h, 000h, 000h
        db  0E8h, 0F6h, 012h, 001h, 000h
        add    esp, 0ch
loc_4_72F5:
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 90h]
        test   byte ptr [eax], 20h
        jne    SHORT loc_4_7328
        mov    cx, word ptr [esi + 0ach]
        and    ecx, 0fh
        mov    eax, dword ptr [esi]
        add    eax, ecx
        cmp    eax, 0ffffh
        jbe    SHORT loc_4_7328
        mov    eax, 10000h
        sub    eax, ecx
        mov    dword ptr [esi], eax
loc_4_7328:
        push   esi
        call   sub_4_C339
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_744F
        mov    edi, dword ptr [esi + 74h]
        mov    word ptr [esi + 18h], 0
        test   byte ptr [edi + 23h], 1
        je     SHORT loc_4_734E
        mov    word ptr [esi + 18h], 100h
loc_4_734E:
        push   esi
        db  0E8h, 032h, 0E3h, 0FAh, 0FFh
        add    esp, 4
        cmp    byte ptr [esi + 88h], 3fh
        jne    SHORT loc_4_737A
        cmp    dword ptr [esi], 0
        jbe    SHORT loc_4_7376
        push   0
        push   esi
        push   0
        push   dword ptr [edi]
        db  0E8h, 01Fh, 0C8h, 0FAh, 0FFh
        add    esp, 10h
        jmp    SHORT loc_4_7397
loc_4_7376:
        sub    eax, eax
        jmp    SHORT loc_4_7397
loc_4_737A:
        push   0
        push   esi
        push   1
        push   dword ptr [edi + 4]
        db  0E8h, 009h, 0C8h, 0FAh, 0FFh
        add    esp, 10h
        or     dword ptr [edi + 38h], 80000000h
        and    word ptr [edi + 1ch], 0ffbfh
loc_4_7397:
        or     eax, eax
        jge    SHORT loc_4_73A4
        push   esi
        db  0E8h, 0AFh, 0DAh, 0FAh, 0FFh
        add    esp, 4
loc_4_73A4:
        push   esi
        call   sub_4_7234
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_73EF
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_4_7410
        mov    ecx, dword ptr [esi + 24h]
        mov    dword ptr [eax + 15h], ecx
        cmp    byte ptr [esi + 88h], 40h
        jne    SHORT loc_4_7410
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_73DD
        mov    eax, dword ptr [edi + 14h]
        mov    ecx, dword ptr [esi + 24h]
        cmp    dword ptr [eax + 11h], ecx
loc_4_73DB:
        jae    SHORT loc_4_7410
loc_4_73DD:
        or     dword ptr [edi + 38h], 40000000h
        mov    eax, dword ptr [edi + 14h]
        mov    ecx, dword ptr [esi + 24h]
        mov    dword ptr [eax + 11h], ecx
        jmp    SHORT loc_4_7410
loc_4_73EF:
        cmp    eax, 27h
        jne    SHORT loc_4_7410
        mov    eax, dword ptr [esi + 7ch]
        test   byte ptr [eax + 1bh], 10h
        je     SHORT loc_4_740A
        movzx  eax, word ptr [eax + 14h]
        push   eax
        call   sub_4_72C8
        add    esp, 4
loc_4_740A:
        mov    word ptr [esi + 1ah], 0
loc_4_7410:
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_4_744F
        mov    eax, dword ptr [esi + 90h]
        test   dword ptr [eax], 180h
        mov    eax, dword ptr [esi]
        je     SHORT loc_4_7448
        mov    dword ptr [esi + 0b4h], eax
        db  08Bh, 00Dh, 09Ch, 065h, 000h, 000h
        cmp    word ptr [ecx + 4], 0eah
        jne    SHORT loc_4_744F
        or     eax, 80000000h
        mov    dword ptr [esi + 0b4h], eax
        jmp    SHORT loc_4_744F
loc_4_7448:
        mov    word ptr [esi + 0b4h], ax
loc_4_744F:
        pop    edi
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_C3DC
        add    esp, 4
loc_4_7464:
        or     eax, eax
        jne    NEAR PTR loc_4_756B
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [esi + 14h], eax
        mov    edi, dword ptr [esi + 74h]
        movzx  eax, word ptr [esi + 0ach]
        shl    eax, 10h
        movzx  ecx, word ptr [esi + 0b4h]
        add    eax, ecx
        mov    dword ptr [esi + 24h], eax
        mov    word ptr [esi + 18h], 0
        push   esi
loc_4_7492:
        db  0E8h, 0EFh, 0E1h, 0FAh, 0FFh
        add    esp, 4
        cmp    byte ptr [esi + 88h], 7eh
        push   0
        push   esi
        jne    SHORT loc_4_74B4
        push   0
        push   dword ptr [edi]
        db  0E8h, 0E1h, 0C6h, 0FAh, 0FFh
        add    esp, 10h
        jmp    SHORT loc_4_74CE
loc_4_74B4:
        push   1
        push   dword ptr [edi + 4]
        db  0E8h, 0D2h, 0C6h, 0FAh, 0FFh
        add    esp, 10h
        or     dword ptr [edi + 38h], 80000000h
        and    word ptr [edi + 1ch], 0ffbfh
loc_4_74CE:
        or     eax, eax
        jge    SHORT loc_4_74DB
        push   esi
        db  0E8h, 078h, 0D9h, 0FAh, 0FFh
        add    esp, 4
loc_4_74DB:
        push   esi
        call   sub_4_7234
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_7526
loc_4_74E8:
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_4_7547
        mov    ecx, dword ptr [esi + 24h]
        mov    dword ptr [eax + 15h], ecx
        cmp    byte ptr [esi + 88h], 7fh
        jne    SHORT loc_4_7547
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_7514
        mov    eax, dword ptr [edi + 14h]
        mov    ecx, dword ptr [esi + 24h]
        cmp    dword ptr [eax + 11h], ecx
        jae    SHORT loc_4_7547
loc_4_7514:
        or     dword ptr [edi + 38h], 40000000h
        mov    eax, dword ptr [edi + 14h]
        mov    ecx, dword ptr [esi + 24h]
        mov    dword ptr [eax + 11h], ecx
        jmp    SHORT loc_4_7547
loc_4_7526:
        cmp    eax, 27h
        jne    SHORT loc_4_7547
        mov    eax, dword ptr [esi + 7ch]
        test   byte ptr [eax + 1bh], 10h
        je     SHORT loc_4_7541
        movzx  eax, word ptr [eax + 14h]
        push   eax
        call   sub_4_72C8
        add    esp, 4
loc_4_7541:
        mov    word ptr [esi + 1ah], 0
loc_4_7547:
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_4_756B
        mov    eax, dword ptr [esi + 24h]
        mov    word ptr [esi + 0b4h], ax
        shr    eax, 10h
        mov    word ptr [esi + 0ach], ax
        mov    eax, dword ptr [esi]
        mov    word ptr [esi + 0b0h], ax
loc_4_756B:
        pop    edi
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
loc_4_7572:
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, word ptr [esi + 0b0h]
        shl    eax, 10h
        movzx  ecx, word ptr [esi + 0ach]
        lea    edi, [eax + ecx]
        cmp    byte ptr [esi + 88h], 97h
        jne    SHORT loc_4_75B2
        mov    ax, word ptr [esi + 0a8h]
        mov    word ptr [esi + 6], ax
        mov    byte ptr [esi + 4], 2
        push   esi
        call   sub_4_C3DC
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_75E0
        jmp    SHORT loc_4_762D
loc_4_75B2:
        push   esi
        call   sub_4_C339
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_762D
        movzx  eax, byte ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_4_75E0
        cmp    eax, 1
        je     SHORT loc_4_75D9
        cmp    eax, 2
        je     SHORT loc_4_75E0
        mov    word ptr [esi + 1ah], 1
        jmp    SHORT loc_4_762D
loc_4_75D9:
        add    edi, dword ptr [esi + 24h]
        mov    byte ptr [esi + 4], 0
loc_4_75E0:
        mov    dword ptr [esi + 24h], edi
        mov    edi, dword ptr [esi + 74h]
        push   esi
        db  0E8h, 09Ah, 0E0h, 0FAh, 0FFh
        add    esp, 4
        push   0
        push   esi
        mov    eax, dword ptr [edi + 8]
        push   0ah
        push   dword ptr [eax + 4]
        db  0E8h, 091h, 0C5h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_762D
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_4_7619
        mov    ecx, dword ptr [esi + 24h]
        mov    dword ptr [eax + 15h], ecx
loc_4_7619:
        mov    eax, dword ptr [esi + 24h]
        mov    word ptr [esi + 0b4h], ax
        shr    eax, 10h
        mov    word ptr [esi + 0ach], ax
loc_4_762D:
        pop    edi
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 8bh], 10h
        je     SHORT loc_4_7688
        lea    edi, [esi + 74h]
        mov    eax, dword ptr [esi + 0a8h]
        mov    dword ptr [edi], eax
        push   esi
        db  0E8h, 0D8h, 0CDh, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_775C
        mov    eax, dword ptr [edi]
        mov    ax, word ptr [eax + 30h]
        mov    word ptr [esi + 6], ax
        mov    eax, dword ptr [esi + 0ach]
        mov    dword ptr [esi + 24h], eax
        mov    eax, dword ptr [esi + 9ch]
        mov    dword ptr [esi + 28h], eax
        mov    eax, dword ptr [esi + 0b0h]
        mov    dword ptr [esi + 8], eax
        jmp    NEAR PTR loc_4_772A
loc_4_7688:
        push   esi
        call   sub_4_C339
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_775C
        mov    eax, dword ptr [esi + 74h]
        lea    edi, [esi + 74h]
        movzx  ecx, word ptr [esi + 0ach]
        mov    ax, word ptr [eax + 30h]
        mov    word ptr [esi + 6], ax
        movzx  eax, word ptr [esi + 0b0h]
        shl    eax, 10h
        add    eax, ecx
        mov    dword ptr [esi + 24h], eax
        movzx  eax, word ptr [esi + 9ch]
        shl    eax, 10h
        movzx  ecx, word ptr [esi + 98h]
        add    eax, ecx
        mov    dword ptr [esi + 28h], eax
        test   byte ptr [esi + 8bh], 80h
        mov    word ptr [esi + 18h], 0
        je     SHORT loc_4_7730
        mov    al, byte ptr [esi + 0b5h]
        cmp    al, 0bch
        jne    SHORT loc_4_7716
        mov    word ptr [esi + 18h], 2
        mov    byte ptr [esi + 4], 0
        movzx  eax, byte ptr [esi + 0b4h]
        cmp    eax, 1
        je     SHORT loc_4_7730
        cmp    eax, 3
        je     SHORT loc_4_770F
        mov    word ptr [esi + 1ah], 1
        jmp    SHORT loc_4_775C
loc_4_770F:
        or     word ptr [esi + 18h], 1
        jmp    SHORT loc_4_7730
loc_4_7716:
        cmp    al, 0bdh
        jne    SHORT loc_4_7726
        mov    word ptr [esi + 18h], 2
        mov    byte ptr [esi + 4], 1
        jmp    SHORT loc_4_7730
loc_4_7726:
        cmp    al, 5ch
        jne    SHORT loc_4_7730
loc_4_772A:
        mov    word ptr [esi + 18h], 0
loc_4_7730:
        cmp    byte ptr [esi + 4], 1
        jbe    SHORT loc_4_773E
        mov    word ptr [esi + 1ah], 1
        jmp    SHORT loc_4_775C
loc_4_773E:
        push   esi
        db  0E8h, 042h, 0DFh, 0FAh, 0FFh
        add    esp, 4
        push   0
        push   esi
        mov    eax, dword ptr [edi]
        push   0dh
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 10h]
        db  0E8h, 037h, 0C4h, 0FAh, 0FFh
        add    esp, 10h
loc_4_775C:
        pop    edi
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_C339
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_77BD
        mov    word ptr [esi + 18h], 0
        db  080h, 03Dh, 043h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_7792
        mov    eax, dword ptr [esi + 7ch]
        cmp    byte ptr [eax + 30h], 0
        jne    SHORT loc_4_7792
        mov    word ptr [esi + 18h], 4
loc_4_7792:
        push   esi
        db  0E8h, 0EEh, 0DEh, 0FAh, 0FFh
        add    esp, 4
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   0ch
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 0ch]
        db  0E8h, 0E2h, 0C3h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_77BD
        mov    eax, dword ptr [esi + 74h]
        or     word ptr [eax + 1ch], 40h
loc_4_77BD:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_C339
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_77F7
        mov    byte ptr [esi + 4], 0
        push   esi
        db  0E8h, 0A8h, 0DEh, 0FAh, 0FFh
        add    esp, 4
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   11h
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 20h]
        db  0E8h, 09Ch, 0C3h, 0FAh, 0FFh
        add    esp, 10h
loc_4_77F7:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    si, 0ffffh
        cmp    byte ptr [edi + 0b4h], 1
        ja     NEAR PTR loc_4_7918
        mov    bx, word ptr [edi + 0b4h]
        mov    ax, word ptr [edi + 0b0h]
        mov    word ptr [ebp - 2], ax
        mov    word ptr [edi + 0b0h], 4866h
        jne    SHORT loc_4_783D
        mov    word ptr [edi + 0b0h], 4846h
loc_4_783D:
        mov    word ptr [edi + 0b4h], 440dh
        mov    byte ptr [edi + 4], 0dh
        mov    al, byte ptr [edi + 8bh]
        test   al, 80h
        je     SHORT loc_4_78BF
        test   al, 4
        je     SHORT loc_4_78BF
        mov    ecx, dword ptr [edi + 90h]
        push   dword ptr [edi + 90h]
        mov    ecx, dword ptr [ecx + 8]
        push   dword ptr [edi + 14h]
        mov    si, word ptr [ecx + 3ch]
        mov    ecx, dword ptr [edi + 90h]
        mov    ecx, dword ptr [ecx + 8]
        mov    cx, word ptr [ecx + 14h]
        mov    word ptr [ebp - 4], cx
        call   sub_4_2DAD
        add    esp, 8
        mov    ecx, dword ptr [edi + 90h]
        mov    ecx, dword ptr [ecx + 8]
        mov    word ptr [ecx + 3ch], ax
        mov    eax, dword ptr [edi + 90h]
        mov    eax, dword ptr [eax + 8]
        cmp    word ptr [eax + 3ch], 0
        jne    SHORT loc_4_78B9
        mov    eax, dword ptr [edi + 90h]
        mov    word ptr [edi + 1ah], 8
        mov    eax, dword ptr [eax + 8]
        mov    word ptr [eax + 3ch], si
        jmp    SHORT loc_4_791E
loc_4_78B9:
        mov    word ptr [eax + 14h], 0
loc_4_78BF:
        push   edi
        call   sub_4_7925
        add    esp, 4
        cmp    si, 0ffffh
        je     SHORT loc_4_7904
        mov    eax, dword ptr [edi + 90h]
        push   eax
        mov    eax, dword ptr [eax + 8]
        movzx  eax, word ptr [eax + 3ch]
        push   eax
        call   sub_4_2DD8
        add    esp, 8
        mov    eax, dword ptr [edi + 90h]
        mov    eax, dword ptr [eax + 8]
        mov    word ptr [eax + 3ch], si
        mov    eax, dword ptr [edi + 90h]
        mov    eax, dword ptr [eax + 8]
        mov    cx, word ptr [ebp - 4]
        mov    word ptr [eax + 14h], cx
loc_4_7904:
        mov    word ptr [edi + 0b4h], bx
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [edi + 0b0h], ax
        jmp    SHORT loc_4_791E
loc_4_7918:
        mov    word ptr [edi + 1ah], 1
loc_4_791E:
        pop    edi
        pop    esi
        pop    ebx
loc_4_7921:
        mov    esp, ebp
        pop    ebp
        ret
sub_4_7925:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], 0
        movzx  eax, byte ptr [ebx + 4]
        cmp    eax, 10h
        ja     NEAR PTR loc_4_7DAF
        db  0FFh, 024h, 085h, 03Ch, 061h, 000h, 000h
        db  053h, 0E8h, 0E0h, 049h, 000h, 000h, 083h, 0C4h, 004h, 00Bh, 0C0h, 00Fh, 085h, 0D1h, 007h, 000h
        db  000h, 08Bh, 04Bh, 07Ch, 0F6h, 041h, 01Bh, 080h, 074h, 023h, 08Dh, 083h, 098h, 000h, 000h, 000h
        db  089h, 043h, 028h, 06Ah, 000h, 053h, 08Bh, 0C1h, 06Ah, 02Ah, 08Bh, 040h, 00Ch, 0FFh, 070h, 030h
        db  0E8h, 008h, 0C2h, 0FAh, 0FFh, 083h, 0C4h, 010h, 0E9h, 0A5h, 007h, 000h, 000h, 08Bh, 043h, 074h
        db  080h, 079h, 01Ah, 002h, 066h, 08Bh, 040h, 01Ch, 075h, 004h, 066h, 0B8h, 0C0h, 0A0h, 066h, 089h
        db  083h, 0ACh, 000h, 000h, 000h, 0E9h, 06Ah, 007h, 000h, 000h, 053h, 0E8h, 086h, 049h, 000h, 000h
        db  083h, 0C4h, 004h, 00Bh, 0C0h, 00Fh, 085h, 077h, 007h, 000h, 000h, 08Bh, 043h, 07Ch, 080h, 078h
        db  01Ah, 002h, 00Fh, 084h, 04Ch, 007h, 000h, 000h, 0F6h, 040h, 01Bh, 080h, 074h, 024h, 08Dh, 083h
        db  098h, 000h, 000h, 000h, 089h, 043h, 028h, 06Ah, 000h, 053h, 08Bh, 043h, 07Ch, 06Ah, 02Ah, 08Bh
        db  040h, 00Ch, 0FFh, 070h, 030h, 0E8h, 0A3h, 0C1h, 0FAh, 0FFh, 083h, 0C4h, 010h, 0E9h, 040h, 007h
        db  000h, 000h, 066h, 0C7h, 043h, 01Ah, 001h, 000h, 0E9h, 017h, 007h, 000h, 000h, 053h, 0E8h, 033h
        db  049h, 000h, 000h, 083h, 0C4h, 004h, 00Bh, 0C0h, 00Fh, 085h, 024h, 007h, 000h, 000h, 08Bh, 043h
        db  07Ch, 0F6h, 040h, 01Bh, 080h, 075h, 00Bh, 066h, 0C7h, 043h, 01Ah, 001h, 000h, 0E9h, 010h, 007h
        db  000h, 000h, 08Dh, 083h, 098h, 000h, 000h, 000h, 089h, 043h, 028h, 06Ah, 000h, 053h, 08Bh, 043h
        db  07Ch, 06Ah, 02Ah, 08Bh, 040h, 00Ch, 0FFh, 070h, 030h, 0E8h, 04Fh, 0C1h, 0FAh, 0FFh, 083h, 0C4h
        db  010h, 0E9h, 0CEh, 006h, 000h, 000h, 053h, 0E8h, 0EAh, 048h, 000h, 000h, 083h, 0C4h, 004h, 00Bh
        db  0C0h, 00Fh, 085h, 0DBh, 006h, 000h, 000h, 08Bh, 043h, 07Ch, 080h, 078h, 01Ah, 002h, 075h, 01Eh
        db  080h, 07Bh, 004h, 006h, 075h, 00Ch, 0C6h, 083h, 0B4h, 000h, 000h, 000h, 000h, 0E9h, 0A2h, 006h
        db  000h, 000h, 0C6h, 083h, 0B4h, 000h, 000h, 000h, 0FFh, 0E9h, 096h, 006h, 000h, 000h, 0F6h, 040h
        db  01Bh, 080h, 074h, 024h, 08Dh, 083h, 098h, 000h, 000h, 000h, 089h, 043h, 028h, 06Ah, 000h, 053h
        db  08Bh, 043h, 07Ch, 06Ah, 02Ah, 08Bh, 040h, 00Ch, 0FFh, 070h, 030h, 0E8h, 0EDh, 0C0h, 0FAh, 0FFh
        db  083h, 0C4h, 010h, 0E9h, 06Ch, 006h, 000h, 000h, 08Bh, 043h, 074h, 0F6h, 040h, 01Fh, 003h, 074h
        db  05Bh, 080h, 07Bh, 004h, 006h, 075h, 049h, 0C7h, 043h, 024h, 000h, 000h, 000h, 000h, 0C6h, 043h
        db  004h, 002h, 06Ah, 000h, 053h, 08Bh, 040h, 008h, 06Ah, 00Ah, 0FFh, 070h, 004h, 0E8h, 0BBh, 0C0h
        db  0FAh, 0FFh, 083h, 0C4h, 010h, 00Bh, 0C0h, 00Fh, 085h, 037h, 006h, 000h, 000h, 08Bh, 043h, 074h
        db  08Bh, 04Bh, 024h, 039h, 048h, 018h, 072h, 00Ch, 0C6h, 083h, 0B4h, 000h, 000h, 000h, 000h, 0E9h
        db  020h, 006h, 000h, 000h, 0C6h, 083h, 0B4h, 000h, 000h, 000h, 0FFh, 0E9h, 014h, 006h, 000h, 000h
        db  0C6h, 083h, 0B4h, 000h, 000h, 000h, 0FFh, 0E9h, 008h, 006h, 000h, 000h, 066h, 0C7h, 043h, 01Ah
        db  001h, 000h, 0E9h, 0FDh, 005h, 000h, 000h, 066h, 00Fh, 0B6h, 08Bh, 0A8h, 000h, 000h, 000h, 066h
        db  00Bh, 0C9h, 075h, 00Dh, 08Bh, 083h, 094h, 000h, 000h, 000h, 066h, 00Fh, 0B6h, 048h, 00Ah, 066h
        db  041h, 066h, 083h, 0F9h, 002h, 075h, 044h, 0F6h, 005h, 08Dh, 000h, 000h, 000h, 008h, 074h, 03Bh
        db  0F6h, 005h, 08Dh, 000h, 000h, 000h, 080h, 075h, 032h, 0A1h, 0B0h, 055h, 000h, 000h, 039h, 083h
        db  090h, 000h, 000h, 000h, 075h, 025h, 00Fh, 0B7h, 0C1h, 08Bh, 004h, 085h, 0BCh, 065h, 000h, 000h
        db  00Bh, 0C0h, 074h, 00Ch, 08Bh, 000h, 00Bh, 0C0h, 074h, 006h, 0F6h, 040h, 01Bh, 010h, 074h, 00Bh
        db  066h, 0C7h, 043h, 01Ah, 00Fh, 000h, 0E9h, 099h, 005h, 000h, 000h, 00Fh, 0B7h, 0F9h, 066h, 0BEh
        db  000h, 010h, 08Dh, 047h, 0FFh, 050h, 0E8h, 03Ch, 0A5h, 000h, 000h, 083h, 0C4h, 004h, 00Bh, 0C0h
        db  075h, 041h, 08Bh, 00Ch, 0BDh, 0BCh, 065h, 000h, 000h, 00Bh, 0C9h, 074h, 01Bh, 08Bh, 001h, 00Bh
        db  0C0h, 074h, 015h, 0F6h, 040h, 01Bh, 008h, 075h, 01Ch, 08Bh, 041h, 014h, 00Bh, 0C0h, 074h, 011h
        db  0F6h, 040h, 044h, 080h, 075h, 00Fh, 0EBh, 009h, 0F6h, 087h, 08Bh, 000h, 000h, 000h, 001h, 074h
        db  004h, 066h, 0BEh, 040h, 008h, 0F6h, 087h, 08Bh, 000h, 000h, 000h, 080h, 074h, 005h, 066h, 081h
        db  0CEh, 000h, 080h, 066h, 0C7h, 083h, 0B4h, 000h, 000h, 000h, 000h, 000h, 066h, 089h, 0B3h, 0ACh
        db  000h, 000h, 000h, 0E9h, 02Ch, 005h, 000h, 000h, 053h, 0E8h, 048h, 047h, 000h, 000h, 083h, 0C4h
        db  004h, 00Bh, 0C0h, 00Fh, 085h, 039h, 005h, 000h, 000h, 08Bh, 053h, 07Ch, 08Ah, 04Ah, 01Bh, 0F6h
        db  0C1h, 080h, 074h, 023h, 08Dh, 083h, 098h, 000h, 000h, 000h, 089h, 043h, 028h, 06Ah, 000h, 053h
        db  08Bh, 0C2h, 06Ah, 02Ah, 08Bh, 040h, 00Ch, 0FFh, 070h, 030h, 0E8h, 06Eh, 0BFh, 0FAh, 0FFh, 083h
        db  0C4h, 010h, 0E9h, 00Bh, 005h, 000h, 000h, 08Bh, 043h, 074h, 080h, 07Ah, 01Ah, 002h, 066h, 08Bh
        db  040h, 01Ch, 075h, 006h, 066h, 0B8h, 0C0h, 088h, 0EBh, 009h, 0F6h, 0C1h, 008h, 074h, 004h, 066h
        db  00Dh, 000h, 080h, 066h, 0C7h, 083h, 0B4h, 000h, 000h, 000h, 000h, 000h, 066h, 089h, 083h, 0ACh
        db  000h, 000h, 000h, 0E9h, 0BCh, 004h, 000h, 000h, 066h, 0C7h, 043h, 01Ah, 001h, 000h, 0E9h, 0B1h
        db  004h, 000h, 000h, 00Fh, 0B6h, 083h, 0B0h, 000h, 000h, 000h, 083h
loc_4_7C6E:
        db  0E8h, 04Ah, 083h, 0F8h, 027h
        ja     NEAR PTR loc_4_7DAF
        db  0FFh, 024h, 085h, 080h, 061h, 000h, 000h
        db  08Ah, 083h, 0A8h, 000h, 000h, 000h, 00Ah, 0C0h, 075h, 00Bh, 08Bh, 083h, 094h, 000h, 000h, 000h
        db  08Ah, 040h, 00Ah, 0EBh, 002h, 0FEh, 0C8h, 088h, 083h, 089h, 000h, 000h, 000h, 06Ah, 001h, 053h
        db  0E8h, 003h, 0B9h, 000h, 000h, 083h, 0C4h, 008h, 0E9h, 088h, 004h, 000h, 000h, 053h, 0E8h, 077h
        db  0BCh, 000h, 000h, 083h, 0C4h, 004h, 0E9h, 07Ah, 004h, 000h, 000h, 053h, 0E8h, 0E7h, 0EDh, 0FAh
        db  0FFh, 083h, 0C4h, 004h, 0E9h, 06Ch, 004h, 000h, 000h, 08Ah, 083h, 0A8h, 000h, 000h, 000h, 00Ah
        db  0C0h, 075h, 00Bh, 08Bh, 083h, 094h, 000h, 000h, 000h, 08Ah, 040h, 00Ah, 0EBh, 002h, 0FEh, 0C8h
        db  088h, 083h, 089h, 000h, 000h, 000h, 053h, 0E8h, 0ACh, 0C5h, 000h, 000h, 083h, 0C4h, 004h, 0E9h
        db  041h, 004h, 000h, 000h, 08Ah, 083h, 0A8h, 000h, 000h, 000h, 00Ah, 0C0h, 075h, 00Bh, 08Bh, 083h
        db  094h, 000h, 000h, 000h, 08Ah, 040h, 00Ah, 0EBh, 002h, 0FEh, 0C8h, 088h, 083h, 089h, 000h, 000h
        db  000h, 053h, 0E8h, 00Fh, 0C6h, 000h, 000h, 083h, 0C4h, 004h, 0E9h, 016h, 004h, 000h, 000h, 053h
        db  0E8h, 0B5h, 0AFh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h, 008h, 004h, 000h, 000h, 08Ah, 083h, 0A8h
        db  000h, 000h, 000h, 00Ah, 0C0h, 075h, 00Bh, 08Bh, 083h, 094h, 000h, 000h, 000h, 08Ah, 040h, 00Ah
        db  0EBh, 002h, 0FEh, 0C8h, 088h, 083h, 089h, 000h, 000h, 000h, 053h, 0E8h, 0B1h
loc_4_7D4D:
        mov    dword ptr [eax], 4c48300h
        jmp    NEAR PTR loc_4_8135
        db  053h, 0E8h, 0E6h, 03Bh, 000h, 000h, 083h, 0C4h, 004h, 0E9h, 0CFh, 003h, 000h, 000h, 053h, 0E8h
        db  0CDh, 045h, 000h, 000h, 083h, 0C4h, 004h, 00Bh, 0C0h, 00Fh, 085h, 0BEh, 003h, 000h, 000h, 08Bh
        db  043h, 07Ch, 0F6h, 040h, 01Bh, 080h, 074h, 024h, 08Dh, 083h, 098h, 000h, 000h, 000h, 089h, 043h
        db  028h, 06Ah, 000h, 053h, 08Bh, 043h, 07Ch, 06Ah, 02Ah, 08Bh, 040h, 00Ch, 0FFh, 070h, 030h, 0E8h
        db  0F4h, 0BDh, 0FAh, 0FFh, 083h, 0C4h, 010h, 0E9h, 073h, 003h, 000h, 000h, 066h, 0C7h, 043h, 01Ah
        db  001h, 000h, 0E9h, 068h, 003h, 000h, 000h
loc_4_7DAF:
        movzx  ax, byte ptr [ebx + 0a8h]
        mov    word ptr [ebp - 2], ax
        or     ax, ax
        jne    SHORT loc_4_7DD1
        mov    eax, dword ptr [ebx + 94h]
        movzx  ax, byte ptr [eax + 0ah]
        inc    ax
        mov    word ptr [ebp - 2], ax
loc_4_7DD1:
        movzx  eax, word ptr [ebp - 2]
        mov    dword ptr [ebp - 0ch], eax
        db  0F6h, 080h, 08Bh, 000h, 000h, 000h, 001h
        jne    SHORT loc_4_7DEC
        mov    word ptr [ebx + 1ah], 1
        jmp    NEAR PTR loc_4_8117
loc_4_7DEC:
        mov    eax, dword ptr [ebp - 0ch]
        dec    eax
        push   eax
        call   sub_4_120CA
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_7E03
        cmp    byte ptr [ebx + 4], 8
        je     SHORT loc_4_7E22
loc_4_7E03:
        mov    eax, dword ptr [ebp - 0ch]
        db  08Dh, 004h, 085h, 0BCh, 065h, 000h, 000h
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_4_7E2D
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_4_7E2D
        test   byte ptr [eax + 1bh], 8
        je     SHORT loc_4_7E2D
loc_4_7E22:
        mov    word ptr [ebx + 1ah], 1
        jmp    NEAR PTR loc_4_8117
loc_4_7E2D:
        mov    ax, word ptr [ebp - 2]
        dec    al
        mov    byte ptr [ebx + 89h], al
        movzx  eax, byte ptr [ebx + 4]
        cmp    eax, 11h
        ja     NEAR PTR loc_4_8003
        db  0FFh, 024h, 085h, 020h, 062h, 000h, 000h
        db  0BEh, 001h, 000h, 000h, 000h, 0E9h, 0AEh, 001h, 000h, 000h, 00Fh, 0B6h, 083h, 0B0h, 000h, 000h
        db  000h, 083h, 0F8h, 060h, 07Fh, 01Eh, 00Fh, 084h, 065h, 001h, 000h, 000h, 083h, 0F8h, 041h, 074h
        db  039h, 083h, 0F8h, 042h, 00Fh, 084h, 0FDh, 000h, 000h, 000h, 083h, 0F8h, 046h, 07Ch, 021h, 083h
        db  0F8h, 047h, 0EBh, 016h, 083h, 0F8h, 061h, 074h, 021h, 083h, 0F8h, 062h, 00Fh, 084h, 01Dh, 001h
        db  000h, 000h, 083h, 0F8h, 066h, 07Ch, 009h, 083h, 0F8h, 068h, 00Fh, 08Eh, 066h, 001h, 000h, 000h
        db  0BEh, 001h, 000h, 000h, 000h, 0E9h, 05Eh, 001h, 000h, 000h, 08Bh, 083h, 090h, 000h, 000h, 000h
loc_4_7EAD:
        test   byte ptr [eax], 20h
        je     NEAR PTR loc_4_7F63
        mov    eax, dword ptr [ebp - 0ch]
        db  0F6h, 080h, 08Bh, 000h, 000h, 000h, 004h
        je     SHORT loc_4_7ECD
        mov    word ptr [ebx + 1ah], 0ffc0h
        jmp    NEAR PTR loc_4_8135
loc_4_7ECD:
        push   201h
        push   0dh
        db  0E8h, 01Fh, 007h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    SHORT loc_4_7EEE
        mov    word ptr [ebx + 1ah], 8
        jmp    NEAR PTR loc_4_8135
loc_4_7EEE:
        mov    edi, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebx + 14h]
        movsd
        movsd
        movsd
        movsb
        mov    eax, dword ptr [ebx + 90h]
        test   dword ptr [eax], 180h
        je     SHORT loc_4_7F18
        mov    eax, dword ptr [ebx + 14h]
        mov    esi, dword ptr [ebp - 8]
        mov    cx, word ptr [eax + 0dh]
        mov    eax, dword ptr [esi + 9]
        add    esi, 9
        jmp    SHORT loc_4_7F2B
loc_4_7F18:
        mov    esi, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + 9]
        add    esi, 9
        mov    ecx, eax
        shr    ecx, 10h
        and    eax, 0ffffh
loc_4_7F2B:
        mov    word ptr [ebx + 18h], 1
        push   eax
        movzx  eax, cx
        push   eax
        call   sub_4_21DA
        add    esp, 8
        mov    dword ptr [esi], eax
        cmp    eax, -1
        jne    SHORT loc_4_7F5D
        mov    word ptr [ebx + 1ah], 0dh
        push   0
        push   dword ptr [ebp - 8]
        db  0E8h, 0ABh, 006h, 001h, 000h
        add    esp, 8
        jmp    NEAR PTR loc_4_8135
loc_4_7F5D:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 14h], eax
loc_4_7F63:
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 0BBh, 0BEh, 0FAh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_4_8003
        db  08Bh, 083h, 090h, 000h, 000h, 000h, 0F6h, 000h, 020h, 074h, 02Dh, 08Bh, 045h, 0F4h, 0F6h, 080h
        db  08Bh, 000h, 000h, 000h, 004h, 074h, 00Bh, 066h, 0C7h, 043h, 01Ah, 0C0h, 0FFh, 0E9h, 09Fh, 001h
        db  000h, 000h, 08Dh, 07Dh, 0E4h, 08Bh, 073h, 014h, 0A5h, 066h, 0A5h, 0A4h, 08Dh, 045h, 0E4h, 066h
        db  0C7h, 043h, 018h, 001h, 000h, 089h, 043h, 014h, 08Bh, 083h, 090h, 000h, 000h, 000h, 0F6h, 000h
        db  020h, 074h, 028h, 08Bh, 045h, 0F4h, 0F6h, 080h, 08Bh, 000h, 000h, 000h, 004h, 074h
loc_4_7FC2:
        sbb    al, 66h
        mov    dword ptr [ebx + 1ah], 67e9ffc0h
        add    dword ptr [eax], eax
        add    byte ptr [ebx + 0f61443h], cl
        add    dword ptr [edx + eax - 15h], esi
        sub    edi, dword ptr [esi + 1]
        jmp    SHORT loc_4_8005
        db  08Dh, 045h, 0F0h, 050h, 0E8h, 03Fh, 0BEh, 0FAh, 0FFh, 083h, 0C4h, 004h, 0EBh, 016h, 00Fh, 0B6h
        db  083h, 0B0h, 000h, 000h, 000h, 083h, 0F8h, 046h, 074h, 00Ah, 083h, 0F8h, 066h, 0BEh, 001h, 000h
        db  000h, 000h, 075h, 002h
loc_4_8003:
        sub    esi, esi
loc_4_8005:
        mov    edi, dword ptr [ebp - 0ch]
        dec    edi
        cmp    esi, 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   edi
        push   ebx
        call   sub_4_4336
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_802D
        push   esi
        push   edi
        call   sub_4_4114
        add    esp, 8
        mov    word ptr [ebx + 1ah], ax
loc_4_802D:
        cmp    word ptr [ebx + 1ah], 0
        jne    NEAR PTR loc_4_8117
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx + 7ch], eax
        or     eax, eax
        jne    SHORT loc_4_8051
        mov    word ptr [ebx + 1ah], 0fh
        jmp    NEAR PTR loc_4_8117
loc_4_8051:
        push   ebx
        db  0E8h, 024h, 0CDh, 0FAh, 0FFh
        add    esp, 4
        mov    cl, byte ptr [ebx + 4]
        cmp    cl, 0dh
        jne    SHORT loc_4_808D
        mov    al, byte ptr [ebx + 0b0h]
        cmp    al, 41h
        je     SHORT loc_4_8070
        cmp    al, 42h
        jne    SHORT loc_4_808D
loc_4_8070:
        mov    eax, dword ptr [ebx + 7ch]
        cmp    byte ptr [eax + 30h], 0
        jne    SHORT loc_4_808D
        mov    eax, dword ptr [eax + 20h]
        test   byte ptr [eax + 2], 20h
        jne    SHORT loc_4_808D
        mov    word ptr [ebx + 1ah], 5
        jmp    NEAR PTR loc_4_8117
loc_4_808D:
        cmp    cl, 11h
        jne    SHORT loc_4_80CD
        mov    al, byte ptr [ebx + 0b0h]
        cmp    al, 4ah
        je     SHORT loc_4_80B4
        cmp    al, 4bh
        je     SHORT loc_4_80B4
        cmp    al, 6ah
        je     SHORT loc_4_80B4
        cmp    al, 6bh
        je     SHORT loc_4_80B4
        cmp    al, 6ch
        je     SHORT loc_4_80B4
        cmp    al, 6fh
        je     SHORT loc_4_80B4
        cmp    al, 70h
        jne    SHORT loc_4_80CD
loc_4_80B4:
        mov    eax, dword ptr [ebx + 7ch]
        test   byte ptr [eax + 1bh], 10h
        je     SHORT loc_4_80C5
        mov    word ptr [ebx + 1ah], 0
        jmp    SHORT loc_4_8135
loc_4_80C5:
        mov    word ptr [ebx + 1ah], 1
        jmp    SHORT loc_4_8135
loc_4_80CD:
        mov    eax, dword ptr [ebx + 7ch]
        push   0
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebx + 1ch], eax
        lea    eax, [ebx + 98h]
        mov    dword ptr [ebx + 28h], eax
        push   ebx
        mov    eax, dword ptr [ebx + 7ch]
        push   2ah
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 30h]
        db  0E8h, 09Eh, 0BAh, 0FAh, 0FFh
        add    esp, 10h
        cmp    byte ptr [ebx + 4], 0dh
        jne    SHORT loc_4_8117
        cmp    byte ptr [ebx + 0b0h], 6fh
        jne    SHORT loc_4_8117
        mov    eax, dword ptr [ebp - 0ch]
        db  0F6h, 080h, 08Bh, 000h, 000h, 000h, 010h
        je     SHORT loc_4_8117
        mov    eax, dword ptr [ebx + 14h]
        or     byte ptr [eax + 2], 4
loc_4_8117:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_812A
        push   0
        push   dword ptr [ebp - 8]
        db  0E8h, 0D9h, 004h, 001h, 000h
        add    esp, 8
loc_4_812A:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_4_8135
        db  0E8h, 03Ch, 0BDh, 0FAh, 0FFh
loc_4_8135:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  053h, 059h, 000h, 000h, 0ADh, 059h, 000h, 000h, 000h, 05Ah, 000h, 000h, 000h, 05Ah, 000h, 000h
        db  0AFh, 05Dh, 000h, 000h, 0AFh, 05Dh, 000h, 000h, 049h, 05Ah, 000h, 000h, 049h, 05Ah, 000h, 000h
        db  0AFh, 05Dh, 000h, 000h, 01Ah, 05Bh, 000h, 000h, 0EBh, 05Bh, 000h, 000h, 05Bh, 05Ch, 000h, 000h
        db  05Bh, 05Ch, 000h, 000h, 066h, 05Ch, 000h, 000h, 0AFh, 05Dh, 000h, 000h, 0AFh, 05Dh, 000h, 000h
        db  066h, 05Dh, 000h, 000h, 080h, 05Ch, 000h, 000h, 0ADh, 05Ch, 000h, 000h, 0BBh, 05Ch, 000h, 000h
        db  0AFh, 05Dh, 000h, 000h, 0AFh, 05Dh, 000h, 000h, 0AFh, 05Dh, 000h, 000h, 0AFh, 05Dh, 000h, 000h
        db  0AFh, 05Dh, 000h, 000h, 0AFh, 05Dh, 000h, 000h, 0AFh, 05Dh, 000h, 000h, 0AFh, 05Dh, 000h, 000h
        db  0AFh, 05Dh, 000h, 000h, 0AFh, 05Dh, 000h, 000h, 0AFh, 05Dh, 000h, 000h, 0AFh, 05Dh, 000h, 000h
        db  0AFh, 05Dh, 000h, 000h, 0AFh, 05Dh, 000h, 000h, 0AFh, 05Dh, 000h, 000h, 0AFh, 05Dh, 000h, 000h
        db  0AFh
loc_4_81CD:
        pop    ebp
        add    byte ptr [eax], al
        scasd
        pop    ebp
        add    byte ptr [eax], al
        scasd
        pop    ebp
loc_4_81D6:
        add    byte ptr [eax], al
        scasd
        pop    ebp
        add    byte ptr [eax], al
        scasd
        pop    ebp
        add    byte ptr [eax], al
        scasd
        pop    ebp
        add    byte ptr [eax], al
        scasd
        pop    ebp
        add    byte ptr [eax], al
        scasd
        pop    ebp
        add    byte ptr [eax], al
        scasd
        pop    ebp
        add    byte ptr [eax], al
loc_4_81F0:
        scasd
        pop    ebp
        add    byte ptr [eax], al
        scasd
        pop    ebp
        add    byte ptr [eax], al
        scasd
        pop    ebp
        add    byte ptr [eax], al
        scasd
loc_4_81FD:
        pop    ebp
        add    byte ptr [eax], al
        db  080h, 05Ch, 000h, 000h, 0ADh
        pop    esp
        add    byte ptr [eax], al
        leave
        pop    esp
        add    byte ptr [eax], al
        hlt
        pop    esp
        add    byte ptr [eax], al
        pop    ds
        pop    ebp
        add    byte ptr [eax], al
        scasd
        pop    ebp
        add    byte ptr [eax], al
        db  02Dh, 05Dh, 000h, 000h, 058h
        pop    ebp
        add    byte ptr [eax], al
        dec    ebp
loc_4_8221:
        pop    esi
        add    byte ptr [eax], al
        dec    ebp
        pop    esi
        add    byte ptr [eax], al
        add    esp, dword ptr [eax]
        db  000h, 003h
        pushad
        add    byte ptr [eax], al
        dec    ebp
        pop    esi
        add    byte ptr [eax], al
        dec    ebp
        pop    esi
        add    byte ptr [eax], al
        add    esp, dword ptr [eax]
        db  000h, 003h
        pushad
        add    byte ptr [eax], al
        dec    ebp
        pop    esi
        add    byte ptr [eax], al
        dec    ebp
        pop    esi
        add    byte ptr [eax], al
        add    esp, dword ptr [eax]
        db  000h, 003h
        pushad
        add    byte ptr [eax], al
        add    esp, dword ptr [eax]
        db  000h, 057h, 05Eh
        add    byte ptr [eax], al
        dec    ebp
        pop    esi
        add    byte ptr [eax], al
        dec    ebp
        pop    esi
        add    byte ptr [eax], al
        add    esp, dword ptr [eax]
        db  000h, 0EDh
        pop    edi
        add    byte ptr [eax], al
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   dword ptr [esi + 8]
        mov    ax, word ptr [esi + 0a8h]
        mov    word ptr [esi + 6], ax
        push   esi
        call   sub_4_829A
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_8297
        mov    ax, word ptr [esi + 6]
        mov    word ptr [esi + 0b4h], ax
loc_4_8297:
        pop    esi
        pop    ebp
        ret
sub_4_829A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_27B2
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_839A
        mov    edi, dword ptr [esi + 74h]
        test   byte ptr [edi + 1fh], 2
        jne    SHORT loc_4_82D0
        mov    word ptr [esi + 1ah], 6
        mov    eax, 6
        jmp    NEAR PTR loc_4_839E
loc_4_82D0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 7ch], eax
        push   esi
        db  0E8h, 09Fh, 0CAh, 0FAh, 0FFh
        add    esp, 4
        mov    ebx, dword ptr [ebp + 10h]
        and    ebx, 1
        je     SHORT loc_4_830F
        mov    eax, dword ptr [esi + 7ch]
        cmp    byte ptr [eax + 30h], 1
        jbe    SHORT loc_4_830F
        test   byte ptr [eax + 32h], 8
        je     SHORT loc_4_830F
        push   esi
        db  0E8h, 002h, 0C6h, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [esi + 1ah], 5
        mov    eax, 5
        jmp    NEAR PTR loc_4_839E
loc_4_830F:
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 8], eax
        push   esi
        db  0E8h, 0B8h, 0ADh, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_8384
        inc    word ptr [edi + 20h]
        or     ebx, ebx
        je     SHORT loc_4_8384
        cmp    dword ptr [edi + 24h], 0
        je     SHORT loc_4_8351
        push   edi
        db  0E8h, 03Ch, 0BBh, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [esi + 1ah], ax
        or     ax, ax
        je     SHORT loc_4_8351
        push   esi
        db  0E8h, 053h, 0AEh, 0FAh, 0FFh
        add    esp, 4
loc_4_8351:
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_4_8384
        mov    ax, word ptr [esi + 6]
        or     byte ptr [edi + 1fh], 20h
        mov    word ptr [edi + 32h], ax
        test   byte ptr [ebp + 10h], 2
        je     SHORT loc_4_836E
        or     byte ptr [edi + 1eh], 2
loc_4_836E:
        mov    eax, dword ptr [esi + 7ch]
        cmp    byte ptr [eax + 30h], 1
        jbe    SHORT loc_4_8384
        mov    ecx, dword ptr [esi + 8]
        cmp    dword ptr [eax + 28h], ecx
        je     SHORT loc_4_8384
        or     word ptr [eax + 32h], 4
loc_4_8384:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 8], eax
        push   esi
        db  0E8h, 06Eh, 0C5h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 7ch], 0
loc_4_839A:
        movsx  eax, word ptr [esi + 1ah]
loc_4_839E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    word ptr [eax + 1ah], 6
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 10ch
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 10h]
        mov    ax, word ptr [ecx]
        mov    word ptr [ebp - 2], ax
        lea    eax, [ebp - 10ch]
        push   eax
        call   sub_4_2751
        add    esp, 4
        mov    eax, dword ptr [ebp - 7ch]
        mov    ecx, dword ptr [eax + 0ch]
        imul   ecx, ecx, 10001h
        mov    edx, dword ptr [ebp + 8]
        lea    eax, [ecx + edx]
        mov    dword ptr [ebp - 104h], eax
        cmp    word ptr [ebp - 2], 200h
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 106h], ax
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], 0
        jb     NEAR PTR loc_4_8551
        mov    bl, byte ptr [ebp + 14h]
        add    ecx, dword ptr [ebp + 0ch]
        movzx  eax, bl
        push   eax
        push   ecx
        lea    eax, [ebp - 10ch]
        push   eax
        call   sub_4_829A
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_4_8628
        test   bl, 1
        je     NEAR PTR loc_4_8503
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 98h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 1ch]
        or     dword ptr [eax], 1
        mov    eax, dword ptr [ebp - 98h]
        mov    eax, dword ptr [eax + 10h]
        test   byte ptr [eax + 1bh], 10h
        je     NEAR PTR loc_4_850F
        mov    dword ptr [ebp - 90h], eax
        lea    eax, [ebp - 10ch]
        push   eax
        db  0E8h, 00Ah, 0C9h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp - 10ch], 0
        mov    eax, dword ptr [ebp - 98h]
        mov    word ptr [ebp - 0f4h], 8000h
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0f0h], eax
        mov    eax, dword ptr [ebp - 98h]
        push   0
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ech], eax
        mov    eax, dword ptr [ebp - 98h]
        and    byte ptr [eax + 1fh], 0dfh
        lea    eax, [ebp - 10ch]
        push   eax
        mov    eax, dword ptr [ebp - 98h]
        push   0
        push   dword ptr [eax]
        db  0E8h, 0C6h, 0B6h, 0FAh, 0FFh
        add    esp, 10h
        mov    eax, dword ptr [ebp - 98h]
        or     byte ptr [eax + 1fh], 20h
        mov    eax, dword ptr [ebp - 90h]
        mov    eax, dword ptr [eax + 20h]
        test   byte ptr [eax + 4], 80h
        jne    SHORT loc_4_84F2
        test   byte ptr [eax + 2], 40h
        jne    SHORT loc_4_84F2
        mov    eax, dword ptr [ebp + 1ch]
        and    dword ptr [eax], 0fffffffeh
loc_4_84F2:
        lea    eax, [ebp - 10ch]
        push   eax
        db  0E8h, 000h, 0C4h, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_4_850F
loc_4_8503:
        mov    ecx, dword ptr [ebp + 10h]
        movzx  eax, word ptr [ebp - 106h]
        mov    dword ptr [ecx], eax
loc_4_850F:
        mov    eax, dword ptr [ebp - 98h]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 1ch]
        cmp    eax, 0ah
        jl     SHORT loc_4_854A
        cmp    eax, 12h
        jge    SHORT loc_4_854A
        push   eax
        push   1
        call   sub_4_2047
        add    esp, 8
        cmp    eax, -1
        je     SHORT loc_4_854A
        imul   eax, eax, 14h
        db  081h, 0B8h, 0A4h, 056h, 000h, 000h, 000h, 000h, 003h, 000h
        jne    SHORT loc_4_854A
        mov    eax, dword ptr [ebp + 1ch]
        or     dword ptr [eax], 2
loc_4_854A:
        sub    eax, eax
        jmp    NEAR PTR loc_4_862F
loc_4_8551:
        mov    eax, dword ptr [ebp - 7ch]
        shl    edx, 4
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [edx + ecx + 34h]
        mov    edx, eax
        and    edx, 0ffff0fffh
        shr    edx, 0ch
        and    eax, 0ffffh
        add    edx, eax
        lea    eax, [ecx + edx]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        shl    eax, 4
        lea    edi, [ecx + eax]
        mov    eax, dword ptr [edi + 34h]
        mov    edx, eax
        and    edx, 0ffff0fffh
        shr    edx, 0ch
        and    eax, 0ffffh
        add    edx, eax
        lea    esi, [ecx + edx]
        mov    bl, byte ptr [ebp + 14h]
        and    bl, 1
        je     SHORT loc_4_85C1
        movzx  ecx, word ptr [ebp - 2]
        mov    eax, dword ptr [ebp - 8]
        movzx  eax, byte ptr [ecx + eax]
        db  039h, 005h, 0D0h, 066h, 000h, 000h
        ja     SHORT loc_4_85C1
        db  039h, 005h, 0DCh, 066h, 000h, 000h
        jbe    SHORT loc_4_85C1
        mov    eax, 6
        jmp    SHORT loc_4_862F
loc_4_85C1:
        db  080h, 03Dh, 0CDh, 061h, 000h, 000h, 001h
        jne    SHORT loc_4_85F2
        mov    eax, dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 10h]
        mov    dword ptr [eax], 2
        push   dword ptr [ebp + 18h]
        push   esi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 8]
        movzx  eax, word ptr [ebp - 2]
        push   eax
        push   dword ptr [ebp - 7ch]
        call   sub_4_28D1
        add    esp, 1ch
        jmp    SHORT loc_4_862F
loc_4_85F2:
        push   dword ptr [ebp - 2]
        lea    eax, [ebp - 10ch]
        push   eax
        call   sub_4_2864
        add    esp, 8
        movzx  ecx, ax
        cmp    ecx, -1
        je     SHORT loc_4_8628
        sub    dx, dx
        mov    ax, word ptr [edi + 32h]
        cmp    ax, dx
        je     SHORT loc_4_8628
loc_4_8618:
        movzx  edi, dx
        cmp    byte ptr [esi + edi], 0ffh
        je     SHORT loc_4_8636
        inc    dx
        cmp    dx, ax
        jb     SHORT loc_4_8618
loc_4_8628:
        movsx  eax, word ptr [ebp - 0f2h]
loc_4_862F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_8636:
        mov    eax, dword ptr [ebp - 8]
        add    ecx, eax
        movzx  edx, dx
        mov    al, byte ptr [ecx]
        mov    byte ptr [edx + esi], al
        mov    byte ptr [ecx], 0ffh
        mov    ecx, dword ptr [ebp + 10h]
        or     bl, bl
        mov    dword ptr [ecx], edx
        je     SHORT loc_4_8655
        mov    eax, dword ptr [ebp + 1ch]
        or     dword ptr [eax], 1
loc_4_8655:
        sub    eax, eax
        jmp    SHORT loc_4_862F
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_2751
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 200h
        jb     NEAR PTR loc_4_87E2
        mov    word ptr [ebp - 0feh], ax
        db  0A1h, 0B0h, 055h, 000h, 000h
        mov    edi, dword ptr [ebp - 0fch]
        mov    ecx, dword ptr [eax + 0ch]
        imul   ecx, ecx, 10001h
        add    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0fch], ecx
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_27B2
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_87D9
        mov    ebx, dword ptr [ebp - 90h]
        mov    esi, ebx
        mov    eax, dword ptr [esi + 14h]
        add    esi, 14h
        or     eax, eax
        je     SHORT loc_4_86E5
        inc    word ptr [eax]
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 1bh]
        mov    word ptr [ecx], ax
        sub    eax, eax
        jmp    NEAR PTR loc_4_87E7
loc_4_86E5:
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 09Fh, 0AFh, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_8702
        mov    eax, 4
        jmp    NEAR PTR loc_4_87E7
loc_4_8702:
        mov    eax, dword ptr [ebx + 10h]
        mov    dword ptr [ebp - 88h], eax
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 064h, 0C6h, 0FAh, 0FFh
        add    esp, 4
        mov    ecx, dword ptr [ebp - 8ch]
        mov    dword ptr [ebp - 0fch], edi
        mov    eax, edi
        shr    eax, 10h
        sub    di, ax
        mov    word ptr [ecx + 31h], di
        mov    word ptr [ecx + 2fh], ax
        mov    word ptr [ecx + 33h], 0
        mov    word ptr [ecx], 1
        lea    edi, [ecx + 20h]
        mov    ax, word ptr [ebx + 22h]
        mov    word ptr [ecx + 2], ax
        mov    eax, 20202020h
        stosd
        stosd
        stosw
        stosb
        mov    eax, dword ptr [ebx + 18h]
        mov    dword ptr [ecx + 15h], eax
        mov    dword ptr [ecx + 7], 10001h
        mov    ax, word ptr [ebx + 1ch]
        add    ax, 8000h
        mov    word ptr [ecx + 5], ax
        db  0A1h, 044h, 059h, 000h, 000h
        mov    eax, dword ptr [eax + 1c8h]
        or     eax, eax
        je     SHORT loc_4_8782
        mov    word ptr [ecx + 39h], ax
loc_4_8782:
        movzx  edx, word ptr [ebp - 0feh]
        db  0A1h, 044h, 059h, 000h, 000h
        or     byte ptr [edx + eax + 2eh], 4
        mov    ax, word ptr [ebp - 0feh]
        mov    word ptr [ecx + 1bh], ax
        mov    edx, dword ptr [ebp - 70h]
        movzx  eax, word ptr [ebp - 0feh]
        mov    dword ptr [edx + eax*4 + 0d4h], ebx
        lea    eax, [ebp - 104h]
        mov    dword ptr [esi], ecx
        or     byte ptr [ebx + 1fh], 1
        inc    word ptr [ebx + 20h]
        push   eax
        db  0E8h, 039h, 0C1h, 0FAh, 0FFh
        add    esp, 4
        mov    ecx, dword ptr [ebp + 10h]
        mov    ax, word ptr [ebp - 0feh]
        mov    word ptr [ecx], ax
        sub    eax, eax
        jmp    SHORT loc_4_87E7
loc_4_87D9:
        movsx  eax, word ptr [ebp - 0eah]
        jmp    SHORT loc_4_87E7
loc_4_87E2:
        mov    eax, 6
loc_4_87E7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   esi
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_2751
        mov    si, word ptr [ebp + 8]
        add    esp, 4
        movzx  eax, si
        push   eax
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_C36E
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_8827
        jmp    NEAR PTR loc_4_88B3
loc_4_8827:
        db  0F6h, 005h, 00Ch, 06Bh, 000h, 000h, 002h
        je     SHORT loc_4_8843
        cmp    si, 1
        je     SHORT loc_4_883C
        cmp    si, 2
        jne    SHORT loc_4_8843
loc_4_883C:
        mov    eax, 5
        jmp    SHORT loc_4_88BA
loc_4_8843:
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 0DCh, 0BBh, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_8858
        jmp    SHORT loc_4_88B3
loc_4_8858:
        db  0A1h, 0B0h, 055h, 000h, 000h
        mov    esi, dword ptr [ebp - 90h]
        mov    eax, dword ptr [eax + 0ch]
        imul   eax, eax, 10001h
        db  08Bh, 00Dh, 0B8h, 055h, 000h, 000h
        shr    ecx, 4
        add    eax, ecx
        mov    dword ptr [ebp - 0fch], eax
        db  08Dh, 085h, 0FCh, 0FEh, 0FFh
sub_4_8882:
        call   dword ptr [eax - 18h]
        push   eax
        test   al, 0fah
        inc    dword ptr [ebx - 3ff4fb3ch]
        jne    SHORT loc_4_88A4
        inc    word ptr [esi + 20h]
        or     byte ptr [esi + 1fh], 2
        mov    eax, dword ptr [ebp + 0ch]
        movzx  ecx, word ptr [ebp - 0feh]
        mov    dword ptr [eax], ecx
loc_4_88A4:
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 04Eh, 0C0h, 0FAh, 0FFh
        add    esp, 4
loc_4_88B3:
        movsx  eax, word ptr [ebp - 0eah]
loc_4_88BA:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    word ptr [eax + 1ah], 101h
        ret
        db  000h, 000h, 000h
sub_4_88D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0ch]
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [esi + 8bh]
        and    al, 40h
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0ffffff4ch
        add    eax, 0f7h
        mov    dword ptr [ebp - 10h], eax
        mov    ax, word ptr [edi + 2]
        mov    word ptr [ebp - 14h], ax
        cmp    ax, 4
        jne    SHORT loc_4_8938
        mov    ax, word ptr [edi]
        shr    ax, 1
        movzx  eax, ax
        cmp    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp + 0ch]
        jl     SHORT loc_4_8925
        mov    dword ptr [eax], 0
        jmp    SHORT loc_4_892B
loc_4_8925:
        mov    dword ptr [eax], 1
loc_4_892B:
        mov    word ptr [esi + 1ah], 0
        sub    eax, eax
        jmp    NEAR PTR loc_4_8A84
loc_4_8938:
        lea    ebx, [edi + 4]
        mov    ecx, 4
        cmp    word ptr [ebx], 0
        je     SHORT loc_4_8962
        movzx  eax, word ptr [ebp - 14h]
        mov    dword ptr [ebp - 0ch], eax
loc_4_894D:
        movzx  edx, word ptr [ebx]
        lea    eax, [ecx + edx]
        cmp    eax, dword ptr [ebp - 0ch]
        je     SHORT loc_4_8962
loc_4_8958:
        add    ebx, edx
        add    ecx, edx
        cmp    word ptr [ebx], 0
        jne    SHORT loc_4_894D
loc_4_8962:
        movzx  eax, word ptr [ebx]
        add    ebx, eax
        movzx  eax, word ptr [ebx]
        mov    dword ptr [ebp - 8], eax
        movzx  eax, word ptr [ebp - 14h]
        mov    dword ptr [ebp - 1ch], eax
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [edi + 2], cx
        sub    word ptr [edi], ax
        mov    word ptr [ebx], 0
        mov    byte ptr [esi + 4], 4
        mov    eax, dword ptr [esi + 8ch]
        add    eax, 208h
        mov    dword ptr [esi + 10h], eax
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   20h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 8]
        db  0E8h, 0E7h, 0B1h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_8A22
        mov    eax, dword ptr [esi + 10h]
        movzx  eax, word ptr [eax]
        add    eax, dword ptr [ebp - 8]
        sar    eax, 1
        mov    dword ptr [ebp - 18h], eax
        test   byte ptr [esi + 8bh], 40h
        je     SHORT loc_4_89DE
        cmp    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 0ch]
        jg     NEAR PTR loc_4_8A5E
        mov    dword ptr [eax], 0
        jmp    NEAR PTR loc_4_8A64
loc_4_89DE:
        mov    byte ptr [esi + 4], 3
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   20h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 8]
        db  0E8h, 09Bh, 0B1h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_8A64
        mov    eax, dword ptr [esi + 10h]
        movzx  eax, word ptr [eax]
        add    eax, dword ptr [ebp - 8]
        sar    eax, 1
        cmp    dword ptr [ebp - 10h], eax
        jle    SHORT loc_4_8A17
        cmp    dword ptr [ebp - 18h], 0f7h
        jge    SHORT loc_4_8A17
        jmp    SHORT loc_4_8A5B
loc_4_8A17:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 0
        jmp    SHORT loc_4_8A64
loc_4_8A22:
        mov    byte ptr [esi + 4], 3
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   20h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 8]
        db  0E8h, 057h, 0B1h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_8A64
        mov    eax, dword ptr [esi + 10h]
        movzx  eax, word ptr [eax]
        add    eax, dword ptr [ebp - 8]
        sar    eax, 1
        cmp    dword ptr [ebp - 10h], eax
        jg     SHORT loc_4_8A5B
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 0
        jmp    SHORT loc_4_8A64
loc_4_8A5B:
        mov    eax, dword ptr [ebp + 0ch]
loc_4_8A5E:
        mov    dword ptr [eax], 1
loc_4_8A64:
        mov    ax, word ptr [ebp - 8]
        add    word ptr [edi], ax
        mov    ax, word ptr [ebp - 1ch]
        mov    word ptr [edi + 2], ax
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ebx], ax
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 4], al
        movsx  eax, word ptr [esi + 1ah]
loc_4_8A84:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   0
        mov    al, byte ptr [esi + 0b5h]
        sub    al, 39h
        mov    byte ptr [esi + 4], al
        push   esi
        call   sub_4_91A0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_8B0F
        cmp    byte ptr [esi + 4], 0
        jne    SHORT loc_4_8AE4
        mov    eax, dword ptr [esi + 0ch]
        cmp    word ptr [eax], 4
        jne    SHORT loc_4_8AC0
        jmp    SHORT loc_4_8AD7
loc_4_8AC0:
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_4_88D0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_8ADD
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_8ADD
loc_4_8AD7:
        mov    word ptr [esi + 1ah], 5
loc_4_8ADD:
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_4_8B1C
loc_4_8AE4:
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   20h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 8]
        db  0E8h, 099h, 0B0h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_8B1C
        push   1
        push   20h
        push   -1
        push   esi
        db  0E8h, 05Dh, 0E0h, 0FAh, 0FFh
        add    esp, 10h
        jmp    SHORT loc_4_8B1C
loc_4_8B0F:
        cmp    word ptr [esi + 1ah], 2
        jne    SHORT loc_4_8B1C
        mov    word ptr [esi + 1ah], 3
loc_4_8B1C:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   0
        cmp    byte ptr [esi + 88h], 79h
        setb   al
        mov    byte ptr [esi + 4], al
        push   esi
        call   sub_4_912E
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_8B6D
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   20h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 8]
        db  0E8h, 039h, 0B0h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_8B6D
        push   1
        push   20h
        push   -1
        push   esi
        db  0E8h, 0FDh, 0DFh, 0FAh, 0FFh
        add    esp, 10h
loc_4_8B6D:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        or     edi, edi
        mov    al, byte ptr [esi + 88h]
        mov    byte ptr [ebp - 1], al
        je     SHORT loc_4_8BA8
        mov    eax, dword ptr [esi + 90h]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [esi + 90h], edi
        db  0A1h, 0B4h, 055h, 000h, 000h
        add    eax, edi
        mov    dword ptr [esi + 94h], eax
loc_4_8BA8:
        mov    byte ptr [esi + 88h], 3bh
        mov    bl, byte ptr [esi + 8bh]
        mov    al, bl
        or     al, 40h
        mov    byte ptr [esi + 8bh], al
        push   esi
        call   sub_4_8C0F
        add    esp, 4
        cmp    dword ptr [esi + 7ch], 0
        je     SHORT loc_4_8BDE
        push   esi
        db  0E8h, 02Ah, 0BDh, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 7ch], 0
loc_4_8BDE:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 88h], al
        mov    byte ptr [esi + 8bh], bl
        or     edi, edi
        je     SHORT loc_4_8C08
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 90h], eax
        db  0A1h, 0B4h, 055h, 000h, 000h
        add    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 94h], eax
loc_4_8C08:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_8C0F:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], 0
        cmp    byte ptr [ebx + 88h], 7bh
        push   0
        push   ebx
        je     SHORT loc_4_8C38
        call   sub_4_91A0
        add    esp, 8
        jmp    SHORT loc_4_8C53
loc_4_8C38:
        call   sub_4_912E
        db  08Bh, 00Dh, 0A0h, 065h, 000h, 000h
        add    esp, 8
        movzx  ecx, word ptr [ecx + 14h]
        db  003h, 00Dh, 0B0h, 065h, 000h, 000h
        mov    dword ptr [ebp - 0ch], ecx
loc_4_8C53:
        mov    byte ptr [ebx + 4], 2
        or     eax, eax
        jne    NEAR PTR loc_4_8E9D
        mov    al, byte ptr [ebx + 89h]
        test   al, 80h
        jne    NEAR PTR loc_4_8E9D
        movzx  eax, al
        db  08Bh, 00Ch, 085h, 0BCh, 065h, 000h, 000h
        mov    dword ptr [ebx + 10h], 0fffffbbbh
        push   0
        push   ebx
        mov    eax, dword ptr [ebx + 7ch]
        push   20h
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 4], ecx
        push   dword ptr [eax + 8]
        db  0E8h, 0FCh, 0AEh, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_8EAA
        mov    eax, dword ptr [ebx + 94h]
        movzx  ecx, byte ptr [ebx + 89h]
        mov    eax, dword ptr [eax + ecx*4 + 4ch]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebx + 0ch]
        cmp    word ptr [eax], 4
        jne    SHORT loc_4_8D01
        push   dword ptr [ebp - 10h]
        call   sub_4_61C3
        add    esp, 4
        mov    ecx, dword ptr [ebx + 94h]
        movzx  eax, byte ptr [ebx + 89h]
        mov    dword ptr [ecx + eax*4 + 4ch], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 4]
        mov    byte ptr [eax], 0
        cmp    byte ptr [ebx + 88h], 7bh
        jne    NEAR PTR loc_4_8E92
        mov    esi, dword ptr [ecx + 4]
        mov    edi, dword ptr [ebp - 0ch]
        sub    esi, 3
        movsd
        jmp    NEAR PTR loc_4_8E92
loc_4_8D01:
        push   dword ptr [ebx + 2ch]
        push   dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebx + 8ch]
        add    eax, 208h
        push   eax
        call   sub_4_15ECC
        add    esp, 0ch
        mov    dword ptr [ebp - 14h], eax
        push   eax
        call   sub_4_6190
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     NEAR PTR loc_4_8E8C
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebx + 8ch]
        mov    edi, dword ptr [ebp - 8]
        add    edi, 4
        add    esi, 20ah
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   dword ptr [ebp - 10h]
        movzx  eax, byte ptr [ebx + 89h]
        mov    ecx, dword ptr [ebx + 94h]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + eax*4 + 4ch], edx
        call   sub_4_61C3
        add    esp, 4
        mov    esi, dword ptr [ebx + 8ch]
        mov    edx, dword ptr [ebp - 8]
        add    esi, 20ah
        movzx  eax, word ptr [edx + 2]
        push   eax
        push   esi
        push   esi
        call   sub_4_14A8A
        add    esp, 0ch
        push   1
        mov    edx, dword ptr [ebp - 8]
        push   3fh
        movzx  eax, word ptr [edx + 2]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   esi
        push   dword ptr [ecx + 4]
        call   sub_4_14A00
        add    esp, 14h
        mov    dword ptr [ebp - 1ch], eax
        test   dl, 1
        je     SHORT loc_4_8E29
        cmp    eax, 3fh
        jae    SHORT loc_4_8E29
        mov    byte ptr [ebx + 4], 3
        mov    eax, dword ptr [ebx + 8ch]
        add    eax, 208h
        mov    dword ptr [ebx + 10h], eax
        push   0
        push   ebx
        mov    eax, dword ptr [ebx + 7ch]
        push   20h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 8]
        db  0E8h, 0B1h, 0ADh, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_8E04
        push   1
        push   40h
        mov    eax, dword ptr [ebx + 10h]
        add    eax, 4
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ecx + 4]
        dec    eax
        push   eax
        call   sub_4_14A26
        add    esp, 10h
        dec    eax
        jmp    SHORT loc_4_8E35
loc_4_8E04:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 4]
        or     byte ptr [eax + 1], 2
        mov    eax, dword ptr [ecx + 4]
        mov    byte ptr [eax], 0
        cmp    byte ptr [ebx + 88h], 7bh
        jne    SHORT loc_4_8E92
        mov    esi, dword ptr [ecx + 4]
        mov    edi, dword ptr [ebp - 0ch]
        sub    esi, 3
        movsd
        jmp    SHORT loc_4_8E92
loc_4_8E29:
        test   byte ptr [ebx + 3], 20h
        jne    SHORT loc_4_8E6E
        cmp    dword ptr [ebp - 1ch], 3fh
        jae    SHORT loc_4_8E6E
loc_4_8E35:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    byte ptr [eax + ecx], 0
        or     byte ptr [edx + 1], 1
        cmp    byte ptr [ebx + 88h], 7bh
        jne    SHORT loc_4_8E92
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 4
        mov    esi, dword ptr [ecx + 4]
        mov    edi, dword ptr [ebp - 0ch]
        sub    esi, 3
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_4_8E92
loc_4_8E6E:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 4]
        mov    byte ptr [eax], 0
        cmp    byte ptr [ebx + 88h], 7bh
        jne    SHORT loc_4_8E92
        mov    esi, dword ptr [ecx + 4]
        mov    edi, dword ptr [ebp - 0ch]
        sub    esi, 3
        movsd
        jmp    SHORT loc_4_8E92
loc_4_8E8C:
        mov    word ptr [ebx + 1ah], 3
loc_4_8E92:
        mov    word ptr [ebx + 0b4h], 0
        jmp    SHORT loc_4_8EAA
loc_4_8E9D:
        cmp    word ptr [ebx + 1ah], -40h
        je     SHORT loc_4_8EAA
        mov    word ptr [ebx + 1ah], 3
loc_4_8EAA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  ebx, byte ptr [esi + 0ach]
        or     ebx, ebx
        jne    SHORT loc_4_8ED3
        mov    eax, dword ptr [esi + 94h]
        movzx  ebx, byte ptr [eax + 0ah]
        inc    ebx
loc_4_8ED3:
        mov    byte ptr [esi + 89h], bl
        db  0F6h, 083h, 08Bh, 000h, 000h, 000h, 001h
        je     SHORT loc_4_8F0F
        lea    edi, [ebx - 1]
        push   1
        push   edi
        push   esi
        call   sub_4_4336
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_8F04
        push   0
        push   edi
        call   sub_4_4114
        add    esp, 8
        mov    word ptr [esi + 1ah], ax
loc_4_8F04:
        cmp    word ptr [esi + 1ah], 0
        jne    NEAR PTR loc_4_9113
loc_4_8F0F:
        db  08Bh, 004h, 09Dh, 0BCh, 065h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [eax], 0
        jne    SHORT loc_4_8F29
        mov    word ptr [esi + 1ah], 0fh
        jmp    NEAR PTR loc_4_9113
loc_4_8F29:
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_8F3D
        mov    ecx, dword ptr [esi + 9ch]
        mov    dword ptr [ebp - 4], ecx
loc_4_8F3B:
        jmp    SHORT loc_4_8F4D
loc_4_8F3D:
        push   3c04h
        call   sub_4_2154
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
loc_4_8F4D:
        mov    eax, dword ptr [esi + 94h]
        mov    edi, dword ptr [eax + ebx*4 + 4ch]
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax + 1ch], 0
        jne    NEAR PTR loc_4_9021
        mov    dword ptr [esi + 7ch], eax
        push   esi
        db  0E8h, 00Ch, 0BEh, 0FAh, 0FFh
        add    esp, 4
        dec    ebx
        push   ebx
        call   sub_4_120CA
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_8FFD
        or     edi, edi
        je     SHORT loc_4_8FF2
        mov    al, byte ptr [esi + 8bh]
        test   al, 40h
        je     SHORT loc_4_8FC9
        and    eax, 3
        push   eax
        push   103h
        movzx  eax, word ptr [edi + 2]
        push   eax
        add    edi, 4
        push   edi
        push   dword ptr [ebp - 4]
        call   sub_4_14A00
        add    esp, 14h
        cmp    eax, 103h
        jb     SHORT loc_4_8FBD
        mov    word ptr [esi + 1ah], 0fh
        jmp    NEAR PTR loc_4_9113
loc_4_8FBD:
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [eax + ecx], 0
        jmp    NEAR PTR loc_4_9113
loc_4_8FC9:
        test   byte ptr [edi + 1], 1
        je     SHORT loc_4_8FE7
        mov    eax, dword ptr [ebp - 8]
        push   -1
        push   dword ptr [eax + 4]
        push   dword ptr [ebp - 4]
        call   sub_4_61EB
        add    esp, 0ch
        jmp    NEAR PTR loc_4_9113
loc_4_8FE7:
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], 0
        jmp    NEAR PTR loc_4_9113
loc_4_8FF2:
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], 0
        jmp    NEAR PTR loc_4_9113
loc_4_8FFD:
        mov    eax, dword ptr [esi + 7ch]
        push   0
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 1ch], eax
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   20h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 8]
        db  0E8h, 077h, 0ABh, 0FAh, 0FFh
        add    esp, 10h
        jmp    NEAR PTR loc_4_9113
loc_4_9021:
        or     edi, edi
        jne    SHORT loc_4_9030
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], 0
        jmp    NEAR PTR loc_4_910A
loc_4_9030:
        mov    al, byte ptr [esi + 8bh]
        test   al, 40h
        je     SHORT loc_4_9075
        and    eax, 3
        push   eax
        push   103h
        movzx  eax, word ptr [edi + 2]
        push   eax
        add    edi, 4
        push   edi
        push   dword ptr [ebp - 4]
        call   sub_4_14A00
        add    esp, 14h
IFSMgr_CDROM_Attach:
        cmp    eax, 103h
        jb     SHORT loc_4_9069
        mov    word ptr [esi + 1ah], 0eah
        jmp    NEAR PTR loc_4_910A
loc_4_9069:
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [eax + ecx], 0
        jmp    NEAR PTR loc_4_910A
loc_4_9075:
        mov    al, byte ptr [edi + 1]
        test   al, 1
        jne    SHORT loc_4_90F7
        test   al, 2
        jne    NEAR PTR loc_4_9126
        lea    eax, [ebp - 0ch]
        mov    dword ptr [esi + 14h], eax
        add    bl, 40h
        mov    byte ptr [ebp - 0ch], bl
        mov    byte ptr [ebp - 0bh], 3ah
        mov    byte ptr [ebp - 0ah], 0
        push   0
        push   esi
        call   sub_4_91A0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_9113
        mov    byte ptr [esi + 4], 3
        mov    eax, dword ptr [esi + 8ch]
        add    eax, 208h
        mov    dword ptr [esi + 10h], eax
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   20h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 8]
        db  0E8h, 0C4h, 0AAh, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_911A
        push   1
        push   40h
        mov    eax, dword ptr [esi + 10h]
        add    eax, 4
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        dec    eax
        push   eax
        call   sub_4_14A26
        add    esp, 10h
        cmp    eax, 40h
        jae    SHORT loc_4_911A
        or     byte ptr [edi + 1], 1
loc_4_90F7:
        mov    eax, dword ptr [ebp - 8]
        push   -1
        db  0FFh
IFSMgr_CDROM_Detach:
        db  070h, 004h
        push   dword ptr [ebp - 4]
        call   sub_4_61EB
        add    esp, 0ch
loc_4_910A:
        mov    word ptr [esi + 0b4h], 0
loc_4_9113:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_911A:
        or     byte ptr [edi + 1], 2
        mov    word ptr [esi + 1ah], 0fh
        jmp    SHORT loc_4_9113
loc_4_9126:
        mov    word ptr [esi + 1ah], 0fh
        jmp    SHORT loc_4_9113
sub_4_912E:
        push   ebp
        mov    ebp, esp
        push   esi
        db  0A1h, 0A0h, 065h, 000h, 000h
        movzx  eax, word ptr [eax + 14h]
        db  003h, 005h, 0B0h, 065h, 000h, 000h
        cmp    word ptr [eax], 504ch
        jne    SHORT loc_4_9175
        cmp    byte ptr [eax + 2], 54h
        jne    SHORT loc_4_9175
        cmp    byte ptr [eax + 4], 0
        jne    SHORT loc_4_9175
        movsx  ecx, byte ptr [eax + 3]
        db  08Bh, 015h, 044h, 059h, 000h, 000h
        test   byte ptr [ecx + edx + 2eh], 10h
        je     SHORT loc_4_9175
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 2
        mov    word ptr [esi + 1ah], 2
        jmp    SHORT loc_4_919D
loc_4_9175:
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 14h], eax
        push   esi
        call   sub_4_91A0
        add    esp, 8
        cmp    eax, -40h
        jne    SHORT loc_4_9199
        mov    word ptr [esi + 1ah], 3
        mov    eax, 3
        jmp    SHORT loc_4_919D
loc_4_9199:
        movsx  eax, word ptr [esi + 1ah]
loc_4_919D:
        pop    esi
        pop    ebp
        ret
sub_4_91A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        sub    esi, esi
        mov    ax, word ptr [ebx + 18h]
        mov    word ptr [ebp - 4], ax
        push   ebx
        call   sub_4_15558
        add    esp, 4
        mov    edi, eax
        mov    eax, dword ptr [ebx]
        xor    eax, edi
        and    eax, 0ffffffh
        xor    eax, edi
        mov    dword ptr [ebx], eax
        db  0F6h, 005h, 00Ch, 06Bh, 000h, 000h, 001h
        je     SHORT loc_4_9215
        test   byte ptr [ebx + 8bh], 40h
        je     SHORT loc_4_9215
        test   edi, 20000000h
        jne    SHORT loc_4_91F0
        test   edi, 4000000h
        je     SHORT loc_4_9215
loc_4_91F0:
        mov    dword ptr [ebx + 7ch], 0
        and    edi, 4000000h
        cmp    edi, 1
        sbb    eax, eax
        and    eax, 1
        add    ax, 2
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        jmp    NEAR PTR loc_4_956B
loc_4_9215:
        test   edi, 80000000h
        je     SHORT loc_4_9249
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 1
        je     SHORT loc_4_9249
        cmp    eax, 2
        jne    SHORT loc_4_9232
        test   edi, 44000000h
        je     SHORT loc_4_9249
loc_4_9232:
        mov    dword ptr [ebx + 7ch], 0
        mov    word ptr [ebx + 1ah], 2
        mov    eax, 2
        jmp    NEAR PTR loc_4_956B
loc_4_9249:
        mov    eax, edi
        and    al, 0fh
        cmp    al, 8
        jne    NEAR PTR loc_4_9364
        mov    eax, dword ptr [ebx + 14h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 30h], 201h
        mov    dword ptr [ebp - 2ch], 208h
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 2ch]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    dword ptr [ebx + 14h], eax
        or     eax, eax
        je     SHORT loc_4_92BD
        push   ebx
        call   sub_4_2A43
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_92DA
        mov    dword ptr [ebp - 24h], 0
        mov    eax, dword ptr [ebx + 14h]
        mov    dword ptr [ebp - 20h], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 34h], eax
loc_4_92BD:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + 14h], eax
        mov    dword ptr [ebx + 7ch], 0
        mov    word ptr [ebx + 1ah], 0ffc0h
        mov    eax, 0ffffffc0h
        jmp    NEAR PTR loc_4_956B
loc_4_92DA:
        add    eax, dword ptr [ebx + 2ch]
        mov    dword ptr [ebp - 8], eax
        push   eax
        call   sub_4_14D65
        add    esp, 4
        lea    eax, [eax*2 + 2]
        mov    dword ptr [ebp - 10h], eax
        lea    edi, [ebp - 4ch]
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    esi, 1
        mov    al, byte ptr [ebx + 8bh]
        mov    byte ptr [ebp - 1], al
        and    al, 0fch
        mov    byte ptr [ebx + 8bh], al
        or     al, 2
        mov    byte ptr [ebx + 8bh], al
        push   ebx
        call   sub_4_15558
        add    esp, 4
        mov    edi, eax
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 8bh], al
        mov    dword ptr [ebp - 1ch], 0
        mov    eax, dword ptr [ebx + 14h]
        mov    dword ptr [ebp - 18h], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + 14h], eax
loc_4_9364:
        mov    eax, edi
        and    eax, 0fh
        cmp    eax, 7
        ja     NEAR PTR loc_4_94E8
        db  0FFh, 024h, 085h, 072h, 075h, 000h, 000h
        db  00Fh, 0B6h, 0BBh, 089h, 000h, 000h, 000h, 04Fh, 0F6h, 087h, 08Ch, 000h, 000h, 000h, 001h, 074h
        db  03Ch, 06Ah, 001h, 057h, 053h, 0E8h, 0A3h, 0AFh, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 074h
        db  020h, 06Ah, 000h, 057h, 0E8h, 072h, 0ADh, 0FFh, 0FFh, 083h, 0C4h, 008h, 066h, 089h, 043h, 01Ah
        db  066h, 00Bh, 0C0h, 075h, 00Ch, 08Bh, 004h, 0BDh, 0C0h, 065h, 000h, 000h, 08Bh, 000h, 089h, 043h
        db  07Ch, 066h, 083h, 07Bh, 01Ah, 000h, 074h, 005h, 0E9h, 09Ah, 001h, 000h, 000h, 083h, 07Bh, 07Ch
        db  000h, 00Fh, 085h, 006h, 001h, 000h, 000h, 066h, 0C7h, 043h, 01Ah, 0C0h, 0FFh, 0E9h, 00Dh, 001h
        db  000h, 000h, 08Bh, 043h, 028h, 089h, 045h, 0ECh, 080h, 08Bh, 089h, 000h, 000h, 000h, 080h, 0C7h
        db  043h, 028h, 000h, 000h, 000h, 000h, 06Ah, 000h, 06Ah, 001h, 053h, 0E8h, 027h, 044h, 000h, 000h
        db  083h, 0C4h, 00Ch, 08Bh, 045h, 0ECh, 089h, 043h, 028h, 080h, 03Dh, 0E1h, 055h, 000h, 000h, 000h
        db  00Fh, 085h, 0D9h, 000h, 000h, 000h, 066h, 083h, 07Bh
loc_4_9412:
        sbb    dh, byte ptr [0ce850fh]
        add    byte ptr [eax], al
        mov    word ptr [ebx + 1ah], 0ffc0h
        mov    eax, 0ffffffc0h
        jmp    NEAR PTR loc_4_956B
        db  066h, 083h, 07Bh, 01Ah, 002h, 075h, 018h, 080h, 0BBh, 088h, 000h, 000h, 000h, 04Eh, 075h, 00Fh
        db  0F6h, 083h, 08Bh, 000h, 000h, 000h, 040h, 075h, 006h, 066h, 0C7h, 043h, 01Ah, 003h, 000h, 080h
        db  03Dh, 0E1h, 055h, 000h, 000h, 000h, 075h, 006h, 066h, 0C7h, 043h, 01Ah, 0C0h, 0FFh, 0C7h, 043h
        db  07Ch, 000h, 000h, 000h, 000h, 0E9h, 084h, 000h, 000h, 000h, 0F7h, 0C7h, 000h, 000h, 000h, 024h
        db  074h, 017h, 081h, 0E7h, 000h, 000h, 000h, 004h, 083h, 0FFh, 001h, 01Bh, 0C0h, 083h, 0E0h, 001h
        db  083h, 0C0h, 002h, 066h, 089h, 043h, 01Ah, 0EBh, 006h, 066h, 0C7h, 043h, 01Ah, 0C0h, 0FFh, 0C7h
        db  043h, 07Ch, 000h, 000h, 000h, 000h, 0EBh, 056h, 083h, 07Bh, 07Ch, 000h, 075h, 03Eh, 053h, 0E8h
        db  08Dh, 05Dh, 000h, 000h, 083h, 0C4h, 004h, 00Bh, 0C0h, 074h, 031h, 0F7h, 0C7h, 000h, 000h, 000h
        db  024h, 074h, 019h, 083h, 0C3h, 01Ah, 081h, 0E7h, 000h, 000h, 000h, 004h, 083h, 0FFh, 001h, 01Bh
        db  0C0h, 083h, 0E0h, 001h, 083h, 0C0h, 002h, 066h, 089h, 003h, 0EBh, 008h, 083h, 0C3h, 01Ah, 066h
        db  0C7h, 003h, 0C0h, 0FFh, 00Fh, 0BFh, 003h, 0E9h, 095h, 000h, 000h, 000h, 053h, 0E8h, 09Fh, 0B8h
        db  0FAh, 0FFh, 083h, 0C4h, 004h, 08Bh, 043h, 07Ch, 08Bh, 040h, 008h, 089h, 043h, 01Ch
loc_4_94E8:
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ebx + 18h], ax
        or     esi, esi
        je     SHORT loc_4_9567
        cmp    word ptr [ebx + 1ah], 0
        jne    SHORT loc_4_9567
        mov    byte ptr [ebx + 4], 2
        push   0
        push   ebx
        mov    eax, dword ptr [ebx + 7ch]
        push   20h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 8]
        db  0E8h, 07Eh, 0A6h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_9528
        push   dword ptr [ebx + 2ch]
        call   sub_4_2B8F
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_9557
loc_4_9528:
        push   dword ptr [ebp - 0ch]
        push   ebx
        call   sub_4_2AC9
        add    esp, 8
        mov    eax, dword ptr [ebp - 10h]
        lea    esi, [ebp - 4ch]
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 2ch], eax
        mov    word ptr [ebx + 1ah], 0ffc0h
loc_4_9557:
        push   ebx
        db  0E8h, 0A1h, 0B3h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [ebx + 7ch], 0
loc_4_9567:
        movsx  eax, word ptr [ebx + 1ah]
loc_4_956B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  079h, 073h, 000h, 000h, 0E8h, 074h, 000h, 000h, 0DBh, 073h, 000h, 000h, 02Ah, 074h, 000h, 000h
        db  0DBh, 073h, 000h, 000h, 064h, 074h, 000h, 000h, 064h, 074h, 000h, 000h, 092h, 074h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_91A0
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_96F8
        mov    eax, dword ptr [esi + 2ch]
        cmp    word ptr [eax + 2], 3ah
        jne    SHORT loc_4_95D6
        cmp    word ptr [eax + 4], 0
        jne    SHORT loc_4_95D6
        mov    word ptr [esi + 1ah], 3
        jmp    NEAR PTR loc_4_9734
loc_4_95D6:
        mov    dl, byte ptr [esi + 4]
        cmp    dl, 1
        jne    SHORT loc_4_9600
        mov    ecx, dword ptr [esi + 7ch]
        mov    eax, 0d0h
        test   byte ptr [ecx + 1bh], 8
        je     SHORT loc_4_95F1
        mov    eax, 58h
loc_4_95F1:
        test   dword ptr [esi], eax
        je     SHORT loc_4_961B
        mov    word ptr [esi + 1ah], 5
        jmp    NEAR PTR loc_4_9734
loc_4_9600:
        cmp    dl, 8
        ja     NEAR PTR loc_4_96F0
        cmp    dl, 2
        jb     SHORT loc_4_961B
        test   byte ptr [esi + 8bh], 40h
        je     NEAR PTR loc_4_96F0
loc_4_961B:
        cmp    dl, 3
        jb     SHORT loc_4_9646
        test   dl, 1
        je     SHORT loc_4_9646
        mov    ax, word ptr [esi + 98h]
        mov    word ptr [esi + 2ah], ax
        mov    ax, word ptr [esi + 0b0h]
        mov    word ptr [esi + 28h], ax
        mov    ax, word ptr [esi + 9ch]
        mov    word ptr [esi + 18h], ax
loc_4_9646:
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   21h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 0ch]
        db  0E8h, 037h, 0A5h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_9734
        mov    al, byte ptr [esi + 4]
        or     al, al
        jne    SHORT loc_4_967E
        mov    ax, word ptr [esi]
        mov    word ptr [esi + 0b0h], ax
        mov    word ptr [esi + 0b4h], ax
        jmp    SHORT loc_4_96DF
loc_4_967E:
        cmp    al, 2
        jne    SHORT loc_4_9698
        mov    eax, dword ptr [esi + 24h]
        mov    word ptr [esi + 0b4h], ax
        shr    eax, 10h
        mov    word ptr [esi + 0ach], ax
        jmp    SHORT loc_4_96DF
loc_4_9698:
        cmp    al, 4
        jb     SHORT loc_4_96DF
        test   al, 1
        jne    SHORT loc_4_96DF
        mov    ax, word ptr [esi + 2ah]
        cmp    byte ptr [esi + 4], 6
        mov    word ptr [esi + 98h], ax
        mov    word ptr [esi + 0b0h], 0
        mov    word ptr [esi + 9ch], 0
        je     SHORT loc_4_96DF
        mov    ax, word ptr [esi + 28h]
        cmp    byte ptr [esi + 4], 8
        mov    word ptr [esi + 0b0h], ax
        jne    SHORT loc_4_96DF
        mov    ax, word ptr [esi + 18h]
        mov    word ptr [esi + 9ch], ax
loc_4_96DF:
        push   1
        push   21h
        push   -1
        push   esi
        db  0E8h, 07Ch, 0D4h, 0FAh, 0FFh
        add    esp, 10h
        jmp    SHORT loc_4_9734
loc_4_96F0:
        mov    word ptr [esi + 1ah], 1
        jmp    SHORT loc_4_9734
loc_4_96F8:
        cmp    eax, -40h
        jne    SHORT loc_4_9734
        cmp    byte ptr [esi + 0b4h], 0
        jne    SHORT loc_4_9734
        mov    eax, dword ptr [esi + 0ch]
        cmp    word ptr [eax], 4
        jne    SHORT loc_4_9734
        movzx  eax, byte ptr [esi + 89h]
        dec    eax
        push   eax
        call   sub_4_120CA
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_9734
        push   0
        db  068h, 0DAh, 00Eh, 000h, 000h
        push   esi
        call   sub_4_2E4A
        add    esp, 0ch
loc_4_9734:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_91A0
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_9823
        cmp    byte ptr [esi + 89h], 2
        jne    SHORT loc_4_977E
        db  0F6h, 005h, 08Dh, 000h, 000h, 000h, 008h
        je     SHORT loc_4_977E
        db  0F6h, 005h, 08Dh, 000h, 000h, 000h, 080h
        jne    SHORT loc_4_977E
        db  0A1h, 0B0h, 055h, 000h, 000h
        cmp    dword ptr [esi + 90h], eax
        jne    SHORT loc_4_977E
        jmp    NEAR PTR loc_4_985F
loc_4_977E:
        mov    word ptr [esi + 18h], 2
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_9795
        mov    eax, dword ptr [esi + 98h]
        jmp    SHORT loc_4_97A2
loc_4_9795:
        push   3800h
        call   sub_4_2154
        add    esp, 4
loc_4_97A2:
        mov    dword ptr [esi + 14h], eax
        movzx  eax, word ptr [esi + 0b0h]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 24h], 0
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   27h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 24h]
        db  0E8h, 0C8h, 0A3h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_9865
        mov    ax, word ptr [esi + 18h]
        mov    word ptr [esi + 0a8h], ax
        mov    eax, dword ptr [esi + 24h]
        mov    word ptr [esi + 0b4h], ax
        db  0F6h, 005h, 00Ch, 06Bh, 000h, 000h, 001h
        je     SHORT loc_4_980E
        mov    word ptr [esi + 0b0h], 0ch
        and    word ptr [esi + 0a8h], 8000h
        mov    word ptr [esi + 0ach], 50h
        jmp    SHORT loc_4_9865
loc_4_980E:
        mov    eax, dword ptr [esi]
        mov    word ptr [esi + 0b0h], ax
        shr    eax, 10h
        mov    word ptr [esi + 0ach], ax
        jmp    SHORT loc_4_9865
loc_4_9823:
        cmp    eax, -40h
        jne    SHORT loc_4_9865
        movzx  eax, byte ptr [esi + 89h]
        push   eax
        call   sub_4_2A1F
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_985F
        mov    word ptr [esi + 0b0h], 0ch
        mov    word ptr [esi + 0ach], 43h
        mov    word ptr [esi + 0a8h], 0
        mov    word ptr [esi + 1ah], 0
        jmp    SHORT loc_4_9865
loc_4_985F:
        mov    word ptr [esi + 1ah], 0fh
loc_4_9865:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_912E
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_98CF
        mov    byte ptr [esi + 4], 0
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   21h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 0ch]
        db  0E8h, 0FBh, 0A2h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_98CF
        mov    eax, dword ptr [esi]
        mov    word ptr [esi + 0b4h], ax
        mov    ax, word ptr [esi + 28h]
        mov    word ptr [esi + 0b0h], ax
        mov    ax, word ptr [esi + 2ah]
        mov    word ptr [esi + 0ach], ax
        mov    eax, dword ptr [esi + 24h]
        mov    word ptr [esi + 98h], ax
        shr    eax, 10h
        mov    word ptr [esi + 0a8h], ax
loc_4_98CF:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_912E
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_991E
        mov    byte ptr [esi + 4], 1
        movzx  eax, word ptr [esi + 0b4h]
        mov    dword ptr [esi], eax
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   21h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 0ch]
        db  0E8h, 088h, 0A2h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_991E
        push   1
        push   21h
        push   -1
        push   esi
        db  0E8h, 04Ch, 0D2h, 0FAh, 0FFh
        add    esp, 10h
loc_4_991E:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        mov    al, byte ptr [esi + 0b5h]
        cmp    al, 73h
        jne    NEAR PTR loc_4_99F3
        push   edi
        push   esi
        call   sub_4_91A0
        add    esp, 8
        cmp    eax, edi
        jne    NEAR PTR loc_4_9C67
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_9960
        mov    edi, dword ptr [esi + 98h]
        jmp    SHORT loc_4_996F
loc_4_9960:
        push   3800h
        call   sub_4_2154
        add    esp, 4
        mov    edi, eax
loc_4_996F:
        mov    dword ptr [esi + 14h], edi
        movzx  ebx, byte ptr [esi + 89h]
        mov    word ptr [esi + 18h], 1
loc_4_997F:
        test   byte ptr [esi + 8bh], 10h
        je     NEAR PTR loc_4_9A9A
        test   byte ptr [esi + 4], 80h
        je     SHORT loc_4_99A5
        mov    edi, dword ptr [esi + 98h]
        mov    dword ptr [esi + 14h], edi
        and    byte ptr [esi + 4], 7fh
        mov    word ptr [esi + 18h], 1
loc_4_99A5:
        mov    al, byte ptr [esi + 4]
        or     al, al
        jne    NEAR PTR loc_4_9A7C
        mov    eax, dword ptr [esi + 7ch]
        test   byte ptr [eax + 32h], 0c0h
        jne    SHORT loc_4_99C3
        cmp    byte ptr [eax + 30h], 4
        jne    NEAR PTR loc_4_9AA1
loc_4_99C3:
        mov    word ptr [esi + 0ach], 0
        mov    word ptr [esi + 0b0h], 0
        mov    word ptr [esi + 0a8h], 0
        mov    word ptr [esi + 0b4h], 0
        mov    dword ptr [esi + 7ch], 0
        jmp    NEAR PTR loc_4_9C67
loc_4_99F3:
        cmp    al, 1bh
        je     SHORT loc_4_9A02
        movzx  ebx, byte ptr [esi + 0ach]
        or     ebx, ebx
loc_4_9A00:
        jne    SHORT loc_4_9A0D
loc_4_9A02:
        mov    eax, dword ptr [esi + 94h]
        movzx  ebx, byte ptr [eax + 0ah]
        inc    ebx
loc_4_9A0D:
        mov    byte ptr [esi + 89h], bl
        lea    eax, [ebx - 1]
        mov    dword ptr [ebp - 4], eax
        db  0F6h, 080h, 08Ch, 000h, 000h, 000h, 001h
        je     SHORT loc_4_9A5D
        push   1
        push   eax
        push   esi
        call   sub_4_4336
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_9A43
        push   0
        push   dword ptr [ebp - 4]
        call   sub_4_4114
        add    esp, 8
        mov    word ptr [esi + 1ah], ax
loc_4_9A43:
        cmp    word ptr [esi + 1ah], 0
        je     SHORT loc_4_9A5D
        mov    al, byte ptr [esi + 0b5h]
        cmp    al, 1ch
        je     NEAR PTR loc_4_9C5A
        jmp    NEAR PTR loc_4_9C56
loc_4_9A5D:
        db  08Bh, 004h, 09Dh, 0BCh, 065h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 7ch], eax
        or     eax, eax
        jne    NEAR PTR loc_4_997F
        mov    word ptr [esi + 1ah], 0fh
        jmp    NEAR PTR loc_4_9C67
loc_4_9A7C:
        cmp    al, 1
        jne    SHORT loc_4_9A88
        or     word ptr [esi + 18h], 1000h
        jmp    SHORT loc_4_9AA1
loc_4_9A88:
        mov    word ptr [esi + 1ah], 1
        mov    dword ptr [esi + 7ch], 0
        jmp    NEAR PTR loc_4_9C67
loc_4_9A9A:
        lea    eax, [esi + 4]
        mov    byte ptr [esi + 4], 0
loc_4_9AA1:
        cmp    byte ptr [esi + 0b5h], 73h
        je     SHORT loc_4_9AB3
        push   esi
        db  0E8h, 0CBh, 0B2h, 0FAh, 0FFh
        add    esp, 4
loc_4_9AB3:
        mov    eax, dword ptr [esi + 7ch]
        test   bl, 80h
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 1ch], eax
        jne    SHORT loc_4_9AE1
        db  0F6h, 083h, 08Bh, 000h, 000h, 000h, 020h
        je     SHORT loc_4_9AE1
        push   0
        dec    ebx
        push   ebx
        call   sub_4_14881
        add    esp, 8
        mov    word ptr [esi + 1ah], 0ffc0h
        jmp    NEAR PTR loc_4_9C67
loc_4_9AE1:
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   23h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 14h]
        db  0E8h, 09Ch, 0A0h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_9C4C
        mov    al, byte ptr [esi + 8bh]
        and    al, 10h
        jne    SHORT loc_4_9B12
        cmp    byte ptr [esi + 0b5h], 73h
        jne    SHORT loc_4_9B27
loc_4_9B12:
        or     al, al
        je     NEAR PTR loc_4_9BC1
        test   byte ptr [esi + 0b4h], 80h
        jne    NEAR PTR loc_4_9BC1
loc_4_9B27:
        mov    eax, dword ptr [esi]
        mov    word ptr [esi + 0b0h], ax
        mov    eax, dword ptr [esi + 24h]
        mov    word ptr [esi + 0ach], ax
        mov    al, byte ptr [esi + 0b5h]
        cmp    al, 1ch
        je     SHORT loc_4_9B62
        cmp    al, 1bh
        je     SHORT loc_4_9B62
        mov    eax, dword ptr [esi + 28h]
        mov    word ptr [esi + 0a8h], ax
        mov    ax, word ptr [esi + 18h]
        mov    word ptr [esi + 0b4h], ax
        jmp    NEAR PTR loc_4_9C67
loc_4_9B62:
        mov    al, byte ptr [esi + 18h]
        mov    byte ptr [esi + 0b4h], al
        db  08Bh, 00Dh, 0C4h, 061h, 000h, 000h
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [ecx + 12fh], al
        mov    eax, dword ptr [esi + 90h]
        add    ecx, 12fh
        test   byte ptr [eax], 20h
        je     SHORT loc_4_9BA5
        db  0A1h, 0C8h, 061h, 000h, 000h
        mov    word ptr [esi + 0d4h], ax
        mov    word ptr [esi + 0a8h], 12fh
        jmp    NEAR PTR loc_4_9C67
loc_4_9BA5:
        mov    eax, ecx
        shr    eax, 4
        mov    word ptr [esi + 0d4h], ax
        and    cx, 0fh
        mov    word ptr [esi + 0a8h], cx
        jmp    NEAR PTR loc_4_9C67
loc_4_9BC1:
        mov    eax, dword ptr [esi + 7ch]
        sub    ecx, ecx
        test   byte ptr [eax + 1bh], 8
        je     SHORT loc_4_9BD1
        mov    ecx, 1
loc_4_9BD1:
        push   dword ptr [eax + 1ch]
        push   ecx
        call   sub_4_2047
        add    esp, 8
        mov    ecx, dword ptr [esi + 7ch]
        test   byte ptr [ecx + 1bh], 8
        je     SHORT loc_4_9BF1
        imul   eax, eax, 14h
        db  08Bh, 080h, 0A0h, 056h, 000h, 000h
        jmp    SHORT loc_4_9BFA
loc_4_9BF1:
        shl    eax, 4
        db  08Bh, 080h, 000h, 056h, 000h, 000h
loc_4_9BFA:
        test   al, 1
        jne    SHORT loc_4_9C67
        mov    word ptr [edi], 2ch
        mov    word ptr [edi + 2], 0
        movzx  eax, word ptr [esi + 18h]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [edi + 0ch], eax
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [edi + 10h], eax
        movzx  eax, word ptr [esi + 18h]
        imul   eax, dword ptr [esi + 28h]
        mov    dword ptr [edi + 14h], eax
        movzx  eax, word ptr [esi + 18h]
        add    edi, 24h
        imul   eax, dword ptr [esi + 24h]
        mov    dword ptr [edi - 0ch], eax
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [edi - 8], eax
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [edi - 4], eax
        sub    eax, eax
        stosd
        stosd
        jmp    SHORT loc_4_9C67
loc_4_9C4C:
        mov    al, byte ptr [esi + 0b5h]
        cmp    al, 1ch
        je     SHORT loc_4_9C5A
loc_4_9C56:
        cmp    al, 1bh
        jne    SHORT loc_4_9C67
loc_4_9C5A:
        mov    byte ptr [esi + 0b4h], 0ffh
        mov    word ptr [esi + 1ah], 0
loc_4_9C67:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        movzx  eax, word ptr [esi + 0d0h]
        shl    eax, 4
        movzx  ecx, word ptr [esi + 98h]
        add    eax, ecx
        db  0B9h, 048h, 066h, 000h, 000h
loc_4_9C90:
        cmp    dword ptr [ecx], eax
        je     SHORT loc_4_9CA0
        add    ecx, 4
        inc    edi
        db  081h, 0F9h, 0C8h, 066h, 000h, 000h
        jb     SHORT loc_4_9C90
loc_4_9CA0:
        cmp    edi, 20h
        jne    SHORT loc_4_9CB0
        mov    word ptr [esi + 1ah], 0fh
        jmp    NEAR PTR loc_4_9D49
loc_4_9CB0:
        db  0F6h, 087h, 08Ch, 000h, 000h, 000h, 001h
        je     SHORT loc_4_9CDF
        push   1
        push   edi
        push   esi
        call   sub_4_4336
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_9CD8
        push   0
        push   edi
        call   sub_4_4114
        add    esp, 8
        mov    word ptr [esi + 1ah], ax
loc_4_9CD8:
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_4_9D49
loc_4_9CDF:
        db  08Bh, 004h, 0BDh, 0C0h, 065h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 7ch], eax
        or     eax, eax
        jne    SHORT loc_4_9CF7
        mov    word ptr [esi + 1ah], 0fh
        jmp    SHORT loc_4_9D49
loc_4_9CF7:
        push   esi
        db  0E8h, 07Eh, 0B0h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [esi + 7ch]
        push   0
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 1ch], eax
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   23h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 14h]
        db  0E8h, 074h, 09Eh, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_9D49
        mov    al, byte ptr [esi + 18h]
        mov    byte ptr [esi + 0b4h], al
        mov    eax, dword ptr [esi + 24h]
        mov    word ptr [esi + 0a8h], ax
        mov    eax, dword ptr [esi]
        mov    word ptr [esi + 0b0h], ax
        mov    eax, dword ptr [esi + 28h]
        mov    word ptr [esi + 0ach], ax
loc_4_9D49:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_9D4D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  ebx, byte ptr [esi + 0ach]
        or     ebx, ebx
        jne    SHORT loc_4_9D6F
        mov    eax, dword ptr [esi + 94h]
        movzx  ebx, byte ptr [eax + 0ah]
        inc    ebx
loc_4_9D6F:
        mov    byte ptr [esi + 89h], bl
        lea    edi, [ebx - 1]
        cmp    byte ptr [esi + 0b5h], 73h
        jne    SHORT loc_4_9D88
        mov    byte ptr [esi + 88h], 73h
loc_4_9D88:
        db  0F6h, 087h, 08Ch, 000h, 000h, 000h, 001h
        je     SHORT loc_4_9DD6
        push   1
        push   edi
        push   esi
        call   sub_4_4336
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_9DB0
        push   0
        push   edi
        call   sub_4_4114
        add    esp, 8
        mov    word ptr [esi + 1ah], ax
loc_4_9DB0:
        cmp    word ptr [esi + 1ah], 0
        je     SHORT loc_4_9DD6
        cmp    byte ptr [esi + 0b5h], 73h
        je     NEAR PTR loc_4_9EA6
        mov    byte ptr [esi + 0b4h], 0ffh
        mov    word ptr [esi + 1ah], 0
        jmp    NEAR PTR loc_4_9EA6
loc_4_9DD6:
        db  08Bh, 004h, 09Dh, 0BCh, 065h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        cmp    ebx, 3
        jge    SHORT loc_4_9DF7
        call   sub_4_358B
        or     eax, eax
        je     SHORT loc_4_9DF7
        push   edi
        call   sub_4_35A7
        add    esp, 4
loc_4_9DF7:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 7ch], eax
        or     eax, eax
        jne    SHORT loc_4_9E0E
        mov    word ptr [esi + 1ah], 0fh
        jmp    NEAR PTR loc_4_9EA6
loc_4_9E0E:
        push   esi
        db  0E8h, 067h, 0AFh, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [esi + 7ch]
        cmp    byte ptr [esi + 0b5h], 73h
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 1ch], eax
        mov    byte ptr [esi + 89h], bl
        jne    SHORT loc_4_9E56
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_9E40
        mov    eax, dword ptr [esi + 98h]
        jmp    SHORT loc_4_9E4D
loc_4_9E40:
        push   3800h
        call   sub_4_2154
        add    esp, 4
loc_4_9E4D:
        mov    dword ptr [esi + 14h], eax
        mov    word ptr [esi + 18h], 1
loc_4_9E56:
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   2bh
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 34h]
        db  0E8h, 027h, 09Dh, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_9E96
        cmp    byte ptr [esi + 0b5h], 73h
        je     SHORT loc_4_9EA6
        mov    eax, dword ptr [esi + 10h]
        mov    word ptr [esi + 0a8h], ax
        shr    eax, 10h
        mov    word ptr [esi + 0d4h], ax
        mov    byte ptr [esi + 0b4h], 0
        jmp    SHORT loc_4_9EA6
loc_4_9E96:
        cmp    byte ptr [esi + 0b5h], 73h
        je     SHORT loc_4_9EA6
        mov    byte ptr [esi + 0b4h], 0ffh
loc_4_9EA6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    bl, byte ptr [esi + 0ach]
        mov    byte ptr [esi + 0ach], 0
        call   sub_4_9D4D
        add    esp, 4
        mov    byte ptr [esi + 0ach], bl
        pop    esi
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  ebx, byte ptr [esi + 0ach]
        or     ebx, ebx
        jne    SHORT loc_4_9EF4
        mov    eax, dword ptr [esi + 94h]
        movzx  ebx, byte ptr [eax + 0ah]
        inc    ebx
loc_4_9EF4:
        db  0F6h, 083h, 08Bh, 000h, 000h, 000h, 001h
        je     SHORT loc_4_9F2A
        lea    edi, [ebx - 1]
        push   1
        push   edi
        push   esi
        call   sub_4_4336
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_9F1F
        push   0
        push   edi
        call   sub_4_4114
        add    esp, 8
        mov    word ptr [esi + 1ah], ax
loc_4_9F1F:
        cmp    word ptr [esi + 1ah], 0
        jne    NEAR PTR loc_4_9FBD
loc_4_9F2A:
        db  08Bh, 004h, 09Dh, 0BCh, 065h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 7ch], eax
        or     eax, eax
        jne    SHORT loc_4_9F42
        mov    word ptr [esi + 1ah], 0fh
        jmp    SHORT loc_4_9FBD
loc_4_9F42:
        push   dword ptr [eax + 1ch]
        push   0
        call   sub_4_2047
        shl    eax, 4
        add    esp, 8
        db  0F6h, 080h, 000h, 056h, 000h, 000h, 001h
        jne    SHORT loc_4_9F6A
        mov    word ptr [esi + 1ah], 1
        mov    dword ptr [esi + 7ch], 0
        jmp    SHORT loc_4_9FBD
loc_4_9F6A:
        push   esi
        db  0E8h, 00Bh, 0AEh, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [esi + 7ch]
        test   byte ptr [esi + 8bh], 4
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 1ch], eax
        mov    byte ptr [esi + 89h], bl
        je     SHORT loc_4_9F93
        mov    eax, dword ptr [esi + 98h]
        jmp    SHORT loc_4_9FA0
loc_4_9F93:
        push   3800h
        call   sub_4_2154
        add    esp, 4
loc_4_9FA0:
        mov    dword ptr [esi + 14h], eax
        mov    byte ptr [esi + 4], 3
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   2dh
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 3ch]
        db  0E8h, 0D6h, 09Bh, 0FAh, 0FFh
        add    esp, 10h
loc_4_9FBD:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 0b4h]
        cmp    al, 7
        ja     NEAR PTR loc_4_A096
        cmp    al, 2
        je     NEAR PTR loc_4_A096
        cmp    al, 3
        je     NEAR PTR loc_4_A096
        db  0F6h, 005h, 00Ch, 06Bh, 000h, 000h, 001h
        je     SHORT loc_4_9FF9
        cmp    al, 4
        jb     SHORT loc_4_9FF9
        jmp    NEAR PTR loc_4_A096
loc_4_9FF9:
        push   esi
        call   sub_4_C339
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_A09C
        mov    ax, word ptr [esi + 0b0h]
        mov    word ptr [esi + 28h], ax
        mov    ax, word ptr [esi + 0ach]
        mov    word ptr [esi + 2ah], ax
        mov    ax, word ptr [esi + 9ch]
        mov    word ptr [esi + 18h], ax
        push   esi
        db  0E8h, 055h, 0B6h, 0FAh, 0FFh
        add    esp, 4
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   0eh
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 14h]
        db  0E8h, 049h, 09Bh, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_A09C
        mov    al, byte ptr [esi + 4]
        or     al, al
        je     SHORT loc_4_A06D
        cmp    al, 4
        je     SHORT loc_4_A06D
        cmp    al, 6
        je     SHORT loc_4_A06D
        cmp    al, 1
        jne    SHORT loc_4_A09C
        mov    eax, dword ptr [esi + 74h]
        or     dword ptr [eax + 38h], 80000000h
        jmp    SHORT loc_4_A09C
loc_4_A06D:
        mov    ax, word ptr [esi + 28h]
        cmp    byte ptr [esi + 4], 6
        mov    word ptr [esi + 0b0h], ax
        mov    ax, word ptr [esi + 2ah]
        mov    word ptr [esi + 0ach], ax
        jne    SHORT loc_4_A09C
        mov    ax, word ptr [esi + 18h]
        mov    word ptr [esi + 9ch], ax
        jmp    SHORT loc_4_A09C
loc_4_A096:
        mov    word ptr [esi + 1ah], 1
loc_4_A09C:
        pop    esi
        pop    ebp
        ret
sub_4_A09F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebx + 0ch]
        movzx  ecx, word ptr [eax + 2]
        lea    edx, [eax + ecx]
        mov    eax, dword ptr [ebp + 10h]
        lea    edi, [edx + 2]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebx + 0ch]
        mov    si, ax
        sub    si, word ptr [edx]
        add    word ptr [ecx], si
        mov    word ptr [edx], ax
        mov    al, byte ptr [ebx + 8bh]
        and    eax, 3
        push   eax
        push   104h
        mov    eax, dword ptr [ebx + 0ch]
        add    eax, 4
        push   eax
        push   dword ptr [ebp + 0ch]
        call   sub_4_14A26
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + ecx], 0
        mov    dword ptr [ebx + 14h], ecx
        mov    esi, dword ptr [ebx + 7ch]
        mov    cl, byte ptr [ebx + 89h]
        mov    eax, dword ptr [ebx + 94h]
        mov    byte ptr [ebp - 1], cl
        mov    al, byte ptr [eax + 0ah]
        inc    al
        mov    byte ptr [ebx + 89h], al
        push   ebx
        call   sub_4_15558
        add    esp, 4
        mov    ecx, dword ptr [ebx]
        xor    ecx, eax
        and    ecx, 0ffffffh
loc_4_A136:
        xor    ecx, eax
        mov    dword ptr [ebx], ecx
        mov    dword ptr [ebx + 7ch], esi
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 89h], cl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_A14D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [edi], 1
        je     SHORT loc_4_A16F
        test   byte ptr [esi], 1
        jne    SHORT loc_4_A16F
        mov    word ptr [esi + 1ah], 5
        mov    eax, 5
        jmp    SHORT loc_4_A1EC
loc_4_A16F:
        add    edi, 2ch
        push   edi
        call   sub_4_1201E
        add    esp, 4
        mov    ecx, dword ptr [esi + 0ch]
        add    eax, 2
        movzx  ecx, word ptr [ecx + 2]
        add    ecx, eax
        add    ecx, 2
        cmp    ecx, 208h
        jle    SHORT loc_4_A19F
        mov    word ptr [esi + 1ah], 2
        mov    eax, 2
        jmp    SHORT loc_4_A1EC
loc_4_A19F:
        push   eax
        push   edi
        push   esi
        call   sub_4_A09F
        add    esp, 0ch
        test   eax, 80000000h
        je     SHORT loc_4_A1BE
        mov    word ptr [esi + 1ah], 2
        mov    eax, 2
        jmp    SHORT loc_4_A1EC
loc_4_A1BE:
        and    eax, 0fh
        cmp    eax, 8
        ja     SHORT loc_4_A1D6
        db  0FFh, 024h, 085h, 0FDh, 081h, 000h, 000h
        db  08Bh, 046h, 07Ch, 08Bh, 040h, 008h, 089h, 046h, 01Ch
loc_4_A1D6:
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   1fh
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 4]
        db  0E8h, 0A7h, 099h, 0FAh, 0FFh
        add    esp, 10h
loc_4_A1EC:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  066h, 0C7h, 046h, 01Ah, 002h, 000h, 0B8h, 002h, 000h, 000h, 000h, 0EBh, 0EFh, 0CDh, 081h, 000h
        db  000h, 0D6h, 081h, 000h, 000h, 0CDh, 081h, 000h, 000h, 0F0h, 081h, 000h, 000h, 0CDh, 081h, 000h
        db  000h, 0F0h, 081h, 000h, 000h, 0CDh
loc_4_A216:
        db  081h, 000h, 000h, 0F0h, 081h, 000h
        db  000h, 0F0h
        db  081h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    si, word ptr [edi + 9ch]
        mov    al, byte ptr [edi + 8bh]
        test   al, 80h
        je     SHORT loc_4_A24C
        mov    eax, 2
        jmp    SHORT loc_4_A288
loc_4_A24C:
        test   al, 40h
        je     SHORT loc_4_A280
        test   al, 10h
        je     SHORT loc_4_A258
        mov    si, 1
loc_4_A258:
        or     si, si
        jne    SHORT loc_4_A25F
        jmp    SHORT loc_4_A280
loc_4_A25F:
        cmp    si, 1
        jne    SHORT loc_4_A275
        movzx  eax, word ptr [edi + 0b0h]
        mov    dword ptr [edi], eax
        mov    eax, 1
        jmp    SHORT loc_4_A288
loc_4_A275:
        mov    word ptr [edi + 1ah], 1
        jmp    NEAR PTR loc_4_A479
loc_4_A280:
        mov    dword ptr [edi], 6
        sub    eax, eax
loc_4_A288:
        push   eax
        push   edi
        call   sub_4_91A0
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_A479
        push   0
        push   1
        push   -1
        push   edi
        db  0E8h, 0C1h, 0C8h, 0FAh, 0FFh
        add    esp, 10h
        mov    ebx, eax
        mov    eax, dword ptr [edi]
        test   eax, 80000000h
        db  074h
IFSMgr_FSDGetCurrentDrive:
        pop    esi
        mov    ecx, dword ptr [edi + 7ch]
        test   byte ptr [ecx + 1bh], 8
        je     SHORT loc_4_A312
        mov    word ptr [edi + 1ah], 1
loc_4_A2C3:
        test   byte ptr [edi + 8bh], 40h
        je     NEAR PTR loc_4_A467
        cmp    si, 1
        jne    NEAR PTR loc_4_A467
        cmp    word ptr [edi + 1ah], 1
        jne    NEAR PTR loc_4_A467
        test   byte ptr [edi + 3], 80h
        je     NEAR PTR loc_4_A467
        push   201h
        push   250h
        db  0E8h, 0FAh, 0E2h, 000h, 000h
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_4_A372
        mov    word ptr [edi + 1ah], 8
        jmp    NEAR PTR loc_4_A479
loc_4_A312:
        test   byte ptr [edi + 8bh], 8
        je     SHORT loc_4_A334
        test   eax, 4000000h
        je     SHORT loc_4_A32D
        mov    word ptr [edi + 1ah], 2
        jmp    NEAR PTR loc_4_A467
loc_4_A32D:
        and    eax, 0efffffffh
        mov    dword ptr [edi], eax
loc_4_A334:
        lea    eax, [edi + 7ch]
        push   0
        push   edi
        mov    eax, dword ptr [edi + 7ch]
        push   1fh
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 4]
        db  0E8h, 046h, 098h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_A2C3
        or     ebx, ebx
        je     SHORT loc_4_A367
        push   3
        push   -1
        push   ebx
        push   edi
        db  0E8h, 003h, 0C8h, 0FAh, 0FFh
        add    esp, 10h
loc_4_A367:
        push   1
        push   1fh
        push   -1
        jmp    NEAR PTR loc_4_A470
loc_4_A372:
        push   201h
        push   40h
        db  0E8h, 07Ah, 0E2h, 000h, 000h
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_4_A39D
        push   0
        push   esi
        db  0E8h, 071h, 0E2h, 000h, 000h
        add    esp, 8
        mov    word ptr [edi + 1ah], 8
        jmp    NEAR PTR loc_4_A479
loc_4_A39D:
        mov    dword ptr [edi + 14h], esi
        mov    dword ptr [edi + 10h], ebx
        mov    word ptr [edi + 1ah], 0
        push   0
        push   edi
        mov    eax, dword ptr [edi + 7ch]
        push   2ch
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 38h]
        db  0E8h, 0D4h, 097h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_A45A
        mov    eax, dword ptr [edi + 20h]
        mov    dword ptr [ebx + 0ch], eax
        mov    byte ptr [ebx + 1fh], 4
loc_4_A3D1:
        push   esi
        push   edi
        call   sub_4_A14D
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_A3E4
        inc    dword ptr [ebp - 8]
        jmp    SHORT loc_4_A3EC
loc_4_A3E4:
        mov    ax, word ptr [edi + 1ah]
        mov    word ptr [ebp - 2], ax
loc_4_A3EC:
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [edi + 20h], eax
        mov    dword ptr [edi + 14h], esi
        mov    word ptr [edi + 1ah], 0
        push   0
        push   edi
        push   2
        push   dword ptr [ebx]
        db  0E8h, 089h, 097h, 0FAh, 0FFh
        add    esp, 10h
        cmp    word ptr [edi + 1ah], 12h
        jne    SHORT loc_4_A3D1
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [edi + 20h], eax
        push   0
        push   edi
        mov    eax, dword ptr [ebx + 8]
        push   0bh
        push   dword ptr [eax + 8]
        db  0E8h, 069h, 097h, 0FAh, 0FFh
        add    esp, 10h
        push   0
        push   ebx
        db  0E8h, 0CEh, 0E1h, 000h, 000h
        add    esp, 8
        cmp    dword ptr [ebp - 8], 0
        jle    SHORT loc_4_A452
        push   1
        push   1fh
        push   -1
        push   edi
        db  0E8h, 020h, 0C7h, 0FAh, 0FFh
        add    esp, 10h
        mov    word ptr [edi + 1ah], 0
        jmp    SHORT loc_4_A45A
loc_4_A452:
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [edi + 1ah], ax
loc_4_A45A:
        push   0
        push   esi
        db  0E8h, 09Eh, 0E1h, 000h, 000h
        add    esp, 8
        jmp    SHORT loc_4_A479
loc_4_A467:
        or     ebx, ebx
        je     SHORT loc_4_A479
        push   2
        push   -1
        push   ebx
loc_4_A470:
        push   edi
        db  0E8h, 0F1h, 0C6h, 0FAh, 0FFh
        add    esp, 10h
loc_4_A479:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        db  0A1h, 0A8h, 065h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 2]
        mov    dword ptr [esi], eax
        push   2
        push   esi
        call   sub_4_912E
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_A4F9
        push   0
        push   1
        push   -1
        push   esi
        db  0E8h, 0B9h, 0C6h, 0FAh, 0FFh
        add    esp, 10h
        mov    edi, eax
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   1fh
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 4]
        db  0E8h, 0CAh, 096h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_A4E7
        or     edi, edi
        je     SHORT loc_4_A4DF
        push   3
        push   -1
        push   edi
        push   esi
        db  0E8h, 08Bh, 0C6h, 0FAh, 0FFh
        add    esp, 10h
loc_4_A4DF:
        push   1
        push   1fh
        push   -1
        jmp    SHORT loc_4_A4F0
loc_4_A4E7:
        or     edi, edi
        je     SHORT loc_4_A4F9
        push   2
        push   -1
        push   edi
loc_4_A4F0:
        push   esi
        db  0E8h, 071h, 0C6h, 0FAh, 0FFh
        add    esp, 10h
loc_4_A4F9:
        pop    edi
        pop    esi
        pop    ebp
        ret
_PTEXT ends
        end
