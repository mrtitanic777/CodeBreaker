; vserver_lcode_01_03.asm -- VSERVER object 1 segment _LTEXT  (part 3/5)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_9239:
        ret
        db  0C3h, 000h
sub_1_923C:
        db  0CDh, 020h, 042h, 080h, 040h, 000h
        mov    eax, eax
sub_1_9244:
        db  0CDh, 020h, 043h, 080h, 040h, 000h
        mov    eax, eax
sub_1_924C:
        db  0CDh, 020h, 040h, 080h, 040h, 000h
sub_1_9252:
        db  0CDh, 020h, 010h, 080h, 02Ch, 000h
sub_1_9258:
        db  0CDh, 020h, 00Ah, 080h, 02Ch, 000h
sub_1_925E:
        db  0CDh, 020h, 00Ch, 080h, 02Ch, 000h
sub_1_9264:
        db  0CDh, 020h, 008h, 080h, 02Ch, 000h
sub_1_926A:
        db  0CDh, 020h, 004h, 080h, 02Ch, 000h
sub_1_9270:
        db  0CDh, 020h, 007h, 080h, 02Ch, 000h
sub_1_9276:
        db  0CDh, 020h, 001h, 080h, 02Ch, 000h
sub_1_927C:
        db  0CDh, 020h, 009h, 080h, 02Ch, 000h
sub_1_9282:
        db  0CDh, 020h, 00Dh, 080h, 02Ch, 000h
sub_1_9288:
        db  0CDh, 020h, 070h, 080h, 040h, 000h
sub_1_928E:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    ebx, ecx
        mov    eax, 400h
        mov    ecx, 4
loc_1_929F:
        mov    edx, eax
        push   ebx
        push   ecx
        push   edx
        push   ecx
        push   edx
        push   eax
        call   sub_1_8CB6
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    ebx
        test   eax, eax
        je     SHORT loc_1_9304
        mov    edi, eax
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        mov    esi, esp
        push   esi
        push   edx
        push   edi
        push   2
        push   dword ptr [ebx + 1ah]
        push   dword ptr [ebx + 1eh]
        db  0CDh, 020h, 008h, 000h, 02Ch, 000h
        add    esp, 18h
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        cmp    eax, 7ah
        jne    SHORT loc_1_92F7
        push   ebx
        push   ecx
        push   esi
        push   ecx
        push   edx
        push   edi
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    esi
        pop    ecx
        pop    ebx
        mov    eax, esi
        loop   loc_1_929F
        mov    eax, 8
loc_1_92F7:
        test   eax, eax
        jne    SHORT loc_1_930C
        mov    eax, edi
        clc
loc_1_92FE:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
loc_1_9304:
        mov    eax, 8
loc_1_9309:
        stc
        jmp    SHORT loc_1_92FE
loc_1_930C:
        push   eax
        push   ecx
        push   edx
        push   edi
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    eax
        jmp    SHORT loc_1_9309
sub_1_931B:
        push   edx
        push   edi
        lea    eax, [ecx + 1eh]
        mov    edi, dword ptr [ebx + 2ch]
        push   ecx
        push   0
        push   eax
        push   edi
        call   sub_1_4C14
        add    esp, 0ch
        pop    ecx
        test   eax, eax
        jne    NEAR PTR loc_1_93EE
        mov    edi, 89h
        push   ecx
        push   edi
        push   ecx
        push   edx
        push   edi
        call   sub_1_8CB6
        pop    edx
        pop    edx
        pop    ecx
        pop    edi
        pop    ecx
        test   eax, eax
        je     NEAR PTR loc_1_93F7
        xchg   edi, eax
        push   edx
        mov    edx, esp
        push   ecx
        push   edx
        push   eax
        push   edi
        push   1
        push   dword ptr [ecx + 1eh]
        db  0CDh, 020h, 002h, 000h, 02Ch, 000h
        add    esp, 14h
        pop    ecx
        pop    edx
        test   eax, eax
        jne    NEAR PTR loc_1_940E
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 1ah], eax
        call   sub_1_928E
        jb     NEAR PTR loc_1_9420
        mov    ebx, dword ptr [esp]
        lea    esi, [ebx + 4]
        mov    dword ptr [eax + 8], esi
        mov    esi, dword ptr [ebx + 38h]
        lea    esi, [esi + 8]
        mov    dword ptr [eax + 0ch], esi
        push   ecx
        push   edi
        push   eax
        push   0
        push   eax
        push   2
        push   dword ptr [ecx + 1ah]
        push   dword ptr [ecx + 1eh]
        db  0CDh, 020h, 009h, 000h, 02Ch, 000h
        add    esp, 14h
        pop    edi
        push   eax
        push   ecx
        push   edx
        push   edi
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    eax
        pop    edi
        pop    ecx
        test   eax, eax
        jne    SHORT loc_1_9420
        mov    esi, dword ptr [edi]
        push   ecx
        push   edi
        call   sub_1_86C0
        pop    edi
        pop    ecx
        jb     SHORT loc_1_9420
        add    eax, 2
        push   eax
        push   ecx
        push   edx
        push   edi
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    eax
        db  08Bh, 015h, 014h, 028h, 001h, 000h
        or     byte ptr [edx + 9], 1
        pop    edi
        pop    edx
        clc
        ret
loc_1_93EE:
        mov    eax, 10002h
loc_1_93F3:
        pop    edi
        pop    edx
        stc
        ret
loc_1_93F7:
        mov    eax, 80001h
loc_1_93FC:
        push   eax
        push   ecx
        push   dword ptr [ecx + 1eh]
        db  0CDh, 020h, 004h, 000h, 02Ch, 000h
        add    esp, 4
        pop    ecx
        pop    eax
        jmp    SHORT loc_1_93F3
loc_1_940E:
        push   ecx
        push   edx
        push   edi
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        mov    eax, 310002h
        jmp    SHORT loc_1_93FC
loc_1_9420:
        push   ecx
        push   3
        push   0
        push   0
        push   dword ptr [ecx + 1ah]
        push   dword ptr [ecx + 1eh]
        db  0CDh, 020h, 009h, 000h, 02Ch, 000h
        add    esp, 14h
        pop    ecx
        push   ecx
        push   edx
        push   edi
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        mov    eax, 10002h
        jmp    SHORT loc_1_93FC
sub_1_9449:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        push   ecx
        push   3
        push   0
        push   0
        push   dword ptr [ecx + 1ah]
        push   dword ptr [ecx + 1eh]
        db  0CDh, 020h, 009h, 000h, 02Ch, 000h
        add    esp, 14h
        pop    ecx
        push   eax
        push   dword ptr [ecx + 1eh]
        db  0CDh, 020h, 004h, 000h, 02Ch, 000h
        add    esp, 4
        pop    ebx
        xchg   ebx, eax
        test   eax, eax
        jne    SHORT loc_1_9484
        xchg   ebx, eax
        test   eax, eax
        jne    SHORT loc_1_9484
        clc
loc_1_947E:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
loc_1_9484:
        mov    eax, 340002h
        stc
        jmp    SHORT loc_1_947E
sub_1_948C:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        push   ecx
        push   dword ptr [ecx + 1ah]
        push   dword ptr [ecx + 1eh]
        db  0CDh, 020h, 003h, 000h, 02Ch, 000h
        add    esp, 8
        pop    ecx
        push   eax
        push   dword ptr [ecx + 1eh]
        db  0CDh, 020h, 004h, 000h, 02Ch, 000h
        add    esp, 4
        pop    ebx
        xchg   ebx, eax
        test   eax, eax
        jne    SHORT loc_1_94C1
        xchg   ebx, eax
        test   eax, eax
        jne    SHORT loc_1_94C1
        clc
loc_1_94BB:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
loc_1_94C1:
        mov    eax, 340002h
        stc
        jmp    SHORT loc_1_94BB
loc_1_94C9:
        sub    eax, eax
        ret
loc_1_94CC:
        ret
        db  0C3h, 02Bh, 0C0h, 0C3h, 000h, 000h, 000h
sub_1_94D4:
        push   edx
        push   ebx
        push   eax
        push   edi
        mov    edi, dword ptr [edi + 4]
        call   sub_1_9AE8
        jb     NEAR PTR loc_1_9626
        push   ecx
        call   sub_1_9B0C
        jb     NEAR PTR loc_1_9625
        mov    dword ptr [eax + 8], ebx
        lea    edi, [eax + 2ah]
        mov    dword ptr [eax + 0ch], edi
        call   sub_1_9BB8
        pop    ecx
        jb     NEAR PTR loc_1_9626
        mov    dword ptr [eax], 714eh
        mov    dword ptr [eax + 10h], 0
        call   sub_1_9B2C
        jb     NEAR PTR loc_1_9626
        call   sub_1_9B3C
        jb     NEAR PTR loc_1_962B
        mov    ebx, dword ptr [eax]
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax], 71a1h
        call   sub_1_9B3C
        pop    edi
        mov    esi, dword ptr [eax + 20h]
        xchg   ebp, eax
        mov    eax, dword ptr [esi]
        mov    ebx, eax
        stosw
        push   dword ptr [esi + 18h]
        push   dword ptr [esi + 14h]
        call   sub_1_9D3F
        add    esp, 8
        stosd
        mov    eax, dword ptr [esi + 20h]
        stosd
        pop    eax
        mov    edx, eax
        and    edx, 0fh
        je     NEAR PTR loc_1_960D
        ror    eax, 10h
        mov    cl, 7
        and    cl, al
        cmp    cl, 3
        je     SHORT loc_1_957D
        cmp    al, 72h
        jne    SHORT loc_1_957F
        test   bl, 1
        je     SHORT loc_1_957F
        mov    al, 70h
        jmp    SHORT loc_1_957F
loc_1_957D:
        xor    al, cl
loc_1_957F:
        mov    word ptr [ebp + 28h], 0
loc_1_9585:
        mov    word ptr [edi], ax
        or     ah, 20h
        xchg   ebp, eax
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        je     SHORT loc_1_95B1
        test   byte ptr [eax + 28h], 4
        jne    SHORT loc_1_95AA
        mov    ecx, ebp
        and    ecx, 3
        db  08Ah, 089h, 044h, 01Ah, 001h, 000h
        or     word ptr [eax + 28h], cx
loc_1_95AA:
        call   sub_1_9BFF
        jb     SHORT loc_1_9612
loc_1_95B1:
        mov    dword ptr [eax + 4], ebp
        shr    ebp, 10h
        mov    dword ptr [eax + 0ch], ebp
        pop    ebx
        shl    bx, 8
        shr    ebx, 8
        xchg   bh, bl
        mov    dword ptr [eax + 8], ebx
        lea    ebx, [eax + 2ah]
        mov    dword ptr [eax + 10h], ebx
        mov    dword ptr [eax], 716ch
        call   sub_1_9B3C
        pop    ecx
        jb     SHORT loc_1_9616
        movzx  edx, byte ptr [ebp + 18h]
        cmp    dl, 3
        ja     SHORT loc_1_95E8
        test   dl, dl
        jne    SHORT loc_1_95EA
loc_1_95E8:
        mov    dl, 1
loc_1_95EA:
        mov    bx, word ptr [edi]
        shl    ebx, 10h
        mov    bx, word ptr [eax]
        push   ebx
        cmp    dl, 1
        je     SHORT loc_1_9605
        mov    dword ptr [edi - 4], 0
        cmp    dword ptr [edi - 4], ecx
        jne    SHORT loc_1_9659
loc_1_9605:
        call   sub_1_9B74
        pop    eax
        clc
        ret
loc_1_960D:
        xchg   ebp, eax
        mov    cx, 50h
loc_1_9612:
        mov    dword ptr [eax], ecx
        pop    ecx
        pop    ecx
loc_1_9616:
        call   sub_1_9B74
        stc
        ret
loc_1_961D:
        mov    dword ptr [eax], 2
        jmp    SHORT loc_1_9626
loc_1_9625:
        pop    ecx
loc_1_9626:
        add    esp, 10h
        jmp    SHORT loc_1_9616
loc_1_962B:
        cmp    dword ptr [eax], 2
        je     SHORT loc_1_9635
        cmp    dword ptr [eax], 12h
        jne    SHORT loc_1_9626
loc_1_9635:
        test   byte ptr [esp + 4], 10h
        je     SHORT loc_1_961D
        pop    edi
        xchg   ebp, eax
        mov    ax, word ptr [esp + 6]
        stosw
        xchg   ecx, eax
        stosd
        xor    eax, eax
        stosd
        pop    eax
        rol    eax, 10h
        mov    word ptr [ebp + 28h], 4
        jmp    NEAR PTR loc_1_9585
loc_1_9659:
        mov    dword ptr [edi - 4], ecx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 0ch], ecx
        shr    ecx, 10h
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax], 4200h
        call   sub_1_9B3C
        jb     SHORT loc_1_968D
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax], 4000h
        call   sub_1_9B3C
        jae    NEAR PTR loc_1_9605
loc_1_968D:
        pop    edx
        mov    edx, dword ptr [eax]
        mov    dword ptr [eax], 3e00h
        call   sub_1_9B3C
        mov    dword ptr [eax], edx
        jmp    NEAR PTR loc_1_9616
        db  08Bh, 0C0h
sub_1_96A4:
        xchg   ebx, eax
        call   sub_1_9AE8
        jb     SHORT loc_1_96DE
        mov    dword ptr [eax + 4], ebx
        cmp    ecx, -1
        je     SHORT loc_1_96D3
        push   eax
        push   ecx
        call   sub_1_9D5E
        pop    ecx
        mov    ecx, eax
        pop    eax
        mov    dword ptr [eax + 8], ecx
        shr    ecx, 10h
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [eax], 5701h
        call   sub_1_9B3C
loc_1_96D3:
        mov    dword ptr [eax], 3e00h
        call   sub_1_9B3C
loc_1_96DE:
        call   sub_1_9B74
        ret
sub_1_96E4:
        xor    ebx, ebx
        xor    ecx, ecx
        xor    esi, esi
        xor    ebp, ebp
sub_1_96EC:
        push   4000h
        jmp    SHORT loc_1_9709
        db  058h, 0B0h, 001h, 0F9h, 0C3h
loc_1_96F8:
        lea    eax, [eax + 2ah]
        call   sub_1_875C
        jmp    SHORT loc_1_970A
        db  08Bh, 0C0h
sub_1_9704:
        push   3f00h
loc_1_9709:
        push   eax
loc_1_970A:
        db  0A1h, 014h, 028h, 001h, 000h
        mov    eax, dword ptr [eax + 2ch]
        bts    dword ptr [eax + 18h], 6
        jb     SHORT loc_1_96F8
        pop    eax
        lea    ebp, [ebx + esi]
        push   ebx
        push   esi
        xchg   ebx, eax
        call   sub_1_9AE8
        jb     SHORT loc_1_978F
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 0ch], edx
        shr    edx, 10h
        mov    dword ptr [eax + 8], edx
        mov    dword ptr [eax], 4200h
        call   sub_1_9B3C
        jb     SHORT loc_1_978F
        pop    ebx
        mov    dword ptr [eax + 0ch], ebx
        pop    esi
        mov    dword ptr [eax + 8], esi
        pop    ecx
        mov    dword ptr [eax], ecx
        call   sub_1_9B3C
        jb     SHORT loc_1_975B
        mov    edx, dword ptr [eax]
        mov    ebx, edx
        cmp    ebx, esi
        jne    SHORT loc_1_9781
loc_1_975A:
        clc
loc_1_975B:
        call   sub_1_9B74
        pushfd
        push   edx
        push   eax
        db  0A1h, 014h, 028h, 001h, 000h
        mov    eax, dword ptr [eax + 2ch]
        and    byte ptr [eax + 18h], 0bfh
        cmp    dword ptr [eax + 2ah], 0
        je     SHORT loc_1_977D
        lea    eax, [eax + 2ah]
        call   sub_1_8BC0
loc_1_977D:
        pop    eax
        pop    edx
        popfd
        ret
loc_1_9781:
        cmp    ch, 40h
        jne    SHORT loc_1_975A
        mov    dword ptr [eax], 70h
loc_1_978C:
        stc
        jmp    SHORT loc_1_975B
loc_1_978F:
        add    esp, 0ch
        xor    ebx, ebx
        jmp    SHORT loc_1_978C
sub_1_9796:
        call   sub_1_9704
        jb     SHORT loc_1_97AA
        db  001h, 015h, 0E0h, 031h, 001h, 000h
        db  001h, 015h, 0E8h, 031h, 001h, 000h
        clc
loc_1_97AA:
        ret
sub_1_97AB:
        call   sub_1_96EC
        jb     SHORT loc_1_97BF
        db  001h, 01Dh, 0E4h, 031h, 001h, 000h
        db  001h, 01Dh, 0E8h, 031h, 001h, 000h
        clc
loc_1_97BF:
        ret
sub_1_97C0:
        xchg   ebx, eax
        call   sub_1_9AE8
        jb     SHORT loc_1_97D6
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax], 6800h
        call   sub_1_9B3C
loc_1_97D6:
        call   sub_1_9B74
        ret
sub_1_97DC:
        push   ebp
        db  08Bh, 02Dh, 018h, 028h, 001h, 000h
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 5d01h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 0d00h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        cld
        clc
        pop    ebp
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_9818:
        xchg   edx, eax
        call   sub_1_9AE8
        jb     SHORT loc_1_984A
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 0ch], ebx
        shr    ebx, 10h
        mov    dword ptr [eax + 8], ebx
        mov    ch, 42h
        mov    dword ptr [eax], ecx
        call   sub_1_9B3C
        jb     SHORT loc_1_984A
        mov    dx, word ptr [ebp + 14h]
        shl    edx, 10h
        mov    dx, word ptr [ebp + 1ch]
        call   sub_1_9B74
        xchg   edx, eax
        clc
        ret
loc_1_984A:
        call   sub_1_9B74
        ret
sub_1_9850:
        xchg   edx, eax
        call   sub_1_9AE8
        jb     NEAR PTR loc_1_98F7
        push   edi
        lea    edi, [eax + 2ah]
        mov    dword ptr [eax + 0ch], edi
        call   sub_1_9BB8
        pop    edi
        jb     NEAR PTR loc_1_98F7
        mov    dword ptr [eax], 4300h
        db  08Bh, 00Dh, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [ecx + 30h]
        cmp    byte ptr [ecx + 3eh], 8
        jne    SHORT loc_1_988D
        mov    word ptr [eax], 7143h
        mov    byte ptr [eax + 4], 0
loc_1_988D:
        call   sub_1_9B3C
        jb     SHORT loc_1_98F7
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [edi + 14h], cx
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax], 5700h
        call   sub_1_9B3C
        jb     SHORT loc_1_98F7
        mov    ebx, dword ptr [ebp + 14h]
        mov    word ptr [edi + 8], bx
        mov    ebx, dword ptr [ebp + 18h]
        mov    word ptr [edi + 0ah], bx
        xor    ebx, ebx
        mov    dword ptr [edi], ebx
        mov    dword ptr [edi + 4], ebx
        mov    dword ptr [eax], 4202h
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        call   sub_1_9B3C
        jb     SHORT loc_1_98F7
        mov    ebx, dword ptr [ebp + 14h]
        shl    ebx, 10h
        mov    bx, word ptr [ebp + 1ch]
        mov    dword ptr [edi + 0ch], ebx
        add    ebx, 7ffh
        and    ebx, 0fffff800h
        mov    dword ptr [edi + 10h], ebx
loc_1_98F7:
        call   sub_1_9B74
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_9900:
        xchg   edx, eax
        call   sub_1_9AE8
        jb     SHORT loc_1_9924
        mov    dword ptr [eax + 4], edx
        mov    bx, word ptr [esi + 8]
        mov    dword ptr [eax + 0ch], ebx
        mov    bx, word ptr [esi + 0ah]
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax], 5701h
        call   sub_1_9B3C
loc_1_9924:
        call   sub_1_9B74
        ret
        db  08Bh, 0C0h
sub_1_992C:
        push   ebp
        db  08Bh, 02Dh, 018h, 028h, 001h, 000h
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        lea    edx, [eax + 1]
        mov    word ptr [ebp + 14h], dx
        mov    word ptr [ebp + 1ch], 3600h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        cld
        cmp    word ptr [ebp + 1ch], -1
        je     SHORT loc_1_9972
        movzx  ecx, word ptr [ebp + 14h]
        movzx  edx, word ptr [ebp + 10h]
        movzx  esi, word ptr [ebp + 1ch]
        movzx  edi, word ptr [ebp + 18h]
        clc
        jmp    SHORT loc_1_9975
loc_1_9972:
        mov    al, 0fh
        stc
loc_1_9975:
        pop    ebp
        ret
        db  090h
sub_1_9978:
        mov    al, 32h
        stc
        ret
sub_1_997C:
        push   5c01h
        jmp    SHORT loc_1_9989
        db  090h
sub_1_9984:
        push   5c00h
loc_1_9989:
        xchg   edx, eax
        call   sub_1_9AE8
        jb     SHORT loc_1_99AE
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 0ch], ebx
        shr    ebx, 10h
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 14h], ecx
        shr    ecx, 10h
        mov    dword ptr [eax + 10h], ecx
        pop    ebx
        mov    dword ptr [eax], ebx
        call   sub_1_9B3C
loc_1_99AE:
        call   sub_1_9B74
        ret
sub_1_99B4:
        clc
        ret
sub_1_99B6:
        stc
        ret
        db  056h, 0E8h, 02Ah, 001h, 000h, 000h, 072h, 034h, 08Dh, 078h, 02Ah, 089h, 078h, 00Ch, 0E8h, 0EDh
        db  001h, 000h, 000h, 072h, 027h, 057h, 0C7h, 040h, 008h, 000h, 000h, 000h, 000h, 0C7h, 000h, 000h
        db  05Ah, 000h, 000h, 0E8h, 05Ch, 001h, 000h, 000h, 05Eh, 05Fh, 072h, 00Ah, 080h, 03Eh, 05Ch, 075h
        db  001h, 046h, 0A5h, 0A5h, 0A5h, 0A4h, 0E8h, 081h, 001h, 000h, 000h, 0C3h, 05Eh, 0EBh, 0F7h
sub_1_99F7:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   ebp
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        sub    edx, edx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 4409h
        mov    byte ptr [ebp + 10h], al
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 2ch]
        test   eax, 1
        jne    NEAR PTR loc_1_9AC2
        mov    ax, word ptr [ebp + 14h]
        test   eax, 1000h
        jne    SHORT loc_1_9A59
        test   eax, 8000h
        je     SHORT loc_1_9A96
        mov    edx, 10h
        jmp    SHORT loc_1_9AC2
loc_1_9A59:
        mov    edx, 4
        mov    word ptr [ebp + 1ch], 150bh
        movzx  ax, byte ptr [ebp + 10h]
        dec    ax
        mov    word ptr [ebp + 10h], 0
        mov    word ptr [ebp + 18h], ax
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    word ptr [ebp + 10h], 0adadh
        jne    SHORT loc_1_9AC2
        cmp    word ptr [ebp + 1ch], 0
        je     SHORT loc_1_9AC2
        mov    edx, 8
        jmp    SHORT loc_1_9AC2
loc_1_9A96:
        or     edx, 2
        mov    word ptr [ebp + 1ch], 4408h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ax, word ptr [ebp + 2ch]
        test   eax, 1
        jne    SHORT loc_1_9AC2
        mov    ax, word ptr [ebp + 1ch]
        or     al, al
        je     SHORT loc_1_9AC2
        mov    edx, 1
loc_1_9AC2:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        mov    eax, edx
        pop    ebp
        pop    ebx
        ret
sub_1_9ADC:
        db  0CDh, 020h, 0B7h, 000h, 001h, 000h
        mov    eax, edx
        ret
        db  000h, 000h, 000h
sub_1_9AE8:
        push   ebx
        db  08Bh, 01Dh, 014h, 028h, 001h, 000h
        lea    eax, [ebx + 44h]
        mov    word ptr [eax + 1eh], di
        shld   ebx, edi, 10h
        mov    word ptr [eax + 1ch], bx
        mov    word ptr [eax + 28h], 0ffffh
        mov    byte ptr [eax + 24h], 1
        clc
        pop    ebx
        ret
        db  090h
sub_1_9B0C:
        push   ebx
        lea    ebx, [eax + 234h]
        mov    dword ptr [eax + 20h], ebx
        clc
        pop    ebx
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_9B1C:
        push   ebx
        call   sub_1_9B0C
        mov    ebx, dword ptr [eax + 20h]
        mov    dword ptr [eax + 0ch], ebx
        pop    ebx
        ret
        db  08Bh, 0C0h
sub_1_9B2C:
        push   ebx
        call   sub_1_9B0C
        mov    ebx, dword ptr [eax + 20h]
        mov    dword ptr [eax + 14h], ebx
        pop    ebx
        ret
        db  08Bh, 0C0h
sub_1_9B3C:
        push   eax
        db  08Bh, 02Dh, 018h, 028h, 001h, 000h
        db  08Bh, 01Dh, 014h, 031h, 001h, 000h
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    esi, eax
        movzx  ecx, byte ptr [esi + 1]
        db  0CDh, 020h, 00Ah, 000h, 040h, 000h
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        jb     SHORT loc_1_9B6B
        mov    ah, byte ptr [ebp + 2ch]
        sahf
        mov    ebx, dword ptr [ebp + 1ch]
loc_1_9B66:
        pop    eax
        mov    dword ptr [eax], ebx
        cld
        ret
loc_1_9B6B:
        mov    ebx, 1
        jmp    SHORT loc_1_9B66
        db  08Bh, 0C0h
sub_1_9B74:
        pushfd
        mov    eax, dword ptr [eax]
        popfd
        ret
sub_1_9B79:
        push   ebx
        push   ebp
        push   esi
        push   edi
        mov    ebp, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 18h]
        sub    esp, 234h
        mov    eax, esp
        mov    word ptr [eax + 28h], 0ffffh
        call   sub_1_9BB8
        jb     SHORT loc_1_9BAA
        add    esp, 234h
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        pop    ebx
        ret
loc_1_9BAA:
        add    esp, 234h
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        pop    ebx
        ret
        db  090h
sub_1_9BB8:
        push   eax
        mov    esi, ebp
        mov    ax, word ptr [esi - 2]
        add    ax, 3a41h
        stosw
        xor    eax, eax
        lodsw
        sub    eax, 2
        movzx  ecx, byte ptr [eax + esi]
        add    ecx, eax
        cmp    ecx, 207h
        jae    SHORT loc_1_9BF6
        mov    ebx, edi
        rep movsb
        xor    eax, eax
loc_1_9BE0:
        mov    al, 5ch
        xchg   byte ptr [ebx], al
        add    ebx, eax
        cmp    ebx, edi
        jb     SHORT loc_1_9BE0
        mov    byte ptr [edi], 0
        pop    eax
        call   sub_1_9BFF
        jb     SHORT loc_1_9BFB
        ret
loc_1_9BF6:
        pop    eax
        mov    cx, 3
loc_1_9BFB:
        mov    dword ptr [eax], ecx
        stc
        ret
sub_1_9BFF:
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        je     SHORT loc_1_9C3D
        cmp    word ptr [eax + 28h], -1
        je     SHORT loc_1_9C3D
        db  08Bh, 00Dh, 014h, 028h, 001h, 000h
        test   byte ptr [ecx + 9], 1
        jne    SHORT loc_1_9C3D
        push   esi
        lea    esi, [eax + 2ah]
        call   sub_1_9C43
        movzx  esi, word ptr [eax + 28h]
        and    ecx, esi
        test   si, 2000h
        jne    SHORT loc_1_9C3E
        xor    ecx, esi
        sub    ecx, 1
        cmc
loc_1_9C37:
        mov    ecx, 5
        pop    esi
loc_1_9C3D:
        ret
loc_1_9C3E:
        sub    ecx, 1
        jmp    SHORT loc_1_9C37
sub_1_9C43:
        push   edx
        mov    edx, dword ptr [ecx + 30h]
        test   byte ptr [edx + 1fh], 1
        mov    edx, ecx
        mov    ecx, 0ffh
        jne    SHORT loc_1_9CBD
        pushad
        mov    eax, dword ptr [edx + 34h]
        push   eax
        mov    esi, edx
        mov    edi, dword ptr [edx + 30h]
        mov    edx, dword ptr [edx + 20h]
        mov    ax, word ptr [edx + 18h]
        mov    dword ptr [esi + 38h], 0
        call   sub_1_196C
        jb     SHORT loc_1_9C7C
        mov    eax, dword ptr [esi + 34h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 38h], eax
loc_1_9C7C:
        pop    eax
        mov    dword ptr [edx + 34h], eax
        popad
        push   eax
        mov    ecx, 81h
        mov    eax, dword ptr [edx + 38h]
        test   eax, eax
        je     SHORT loc_1_9C99
        test   word ptr [eax + 42h], 1
        je     SHORT loc_1_9C99
        pop    eax
        jmp    SHORT loc_1_9CBD
loc_1_9C99:
        pop    eax
        push   eax
        push   ebx
        push   eax
        push   esp
        push   0
        push   0
        push   esi
        mov    eax, dword ptr [edx + 3ch]
        lea    ebx, [eax + 8]
        push   ebx
        lea    ebx, [eax + 24h]
        push   ebx
        db  0CDh, 020h, 004h, 000h, 091h, 004h
        add    esp, 18h
        pop    ecx
        movzx  ecx, cx
        pop    ebx
        pop    eax
loc_1_9CBD:
        pop    edx
        ret
        db  000h
sub_1_9CC0:
        sub    eax, eax
        db  08Bh, 01Dh, 050h, 037h, 001h, 000h
        lodsb
        mov    al, byte ptr [ebx + eax]
        cmp    al, 2
        je     SHORT loc_1_9CD3
        test   eax, eax
        ret
loc_1_9CD3:
        mov    ax, word ptr [esi - 1]
        inc    esi
        ret
        db  02Eh, 08Bh, 0C0h, 08Bh, 01Dh, 050h, 037h, 001h, 000h, 0ACh, 08Ah, 0C8h, 0FEh, 0C0h, 0AAh, 084h
        db  0C9h, 075h, 009h, 0C3h, 02Eh, 08Bh, 0C0h, 0AAh, 0FEh, 0C9h, 074h, 012h, 0ACh, 0D7h, 03Ch, 002h
        db  077h, 0F5h, 066h, 08Bh, 046h, 0FFh, 066h, 0ABh, 046h, 080h, 0E9h, 002h, 077h, 0EEh, 0C3h
sub_1_9D08:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0fh], 0
loc_1_9D15:
        call   sub_1_9CC0
        je     SHORT loc_1_9D2A
        or     ah, ah
        jne    SHORT loc_1_9D15
        cmp    al, 20h
        je     SHORT loc_1_9D26
        jmp    SHORT loc_1_9D15
loc_1_9D26:
        mov    byte ptr [esi - 1], 0
loc_1_9D2A:
        pop    esi
        pop    ecx
        pop    ebx
        leave
        ret
sub_1_9D2F:
        db  0CDh, 020h, 060h, 000h, 040h, 000h
        lea    eax, [eax + eax*2]
        shl    eax, 2
        lea    eax, [eax + eax*4]
        ret
sub_1_9D3F:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 03Ch, 000h, 040h, 000h
        add    esp, 8
        push   eax
        call   sub_1_9D2F
        pop    edx
        sub    eax, edx
        neg    eax
        leave
        ret
sub_1_9D5E:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_1_9D2F
        add    dword ptr [esp], eax
        db  0CDh, 020h, 037h, 000h, 040h, 000h
        add    esp, 4
        leave
        ret
        db  000h
sub_1_9D78:
        and    dx, 0ffefh
        call   sub_1_9AE8
        jb     SHORT loc_1_9DB5
        cmp    byte ptr [ebx + 3eh], 8
        je     SHORT loc_1_9D8D
        or     byte ptr [eax + 24h], 4
loc_1_9D8D:
        mov    dword ptr [eax + 8], edx
        lea    edi, [eax + 2ah]
        mov    dword ptr [eax + 0ch], edi
        mov    word ptr [eax + 28h], 10h
        call   sub_1_9BB8
        jb     SHORT loc_1_9DB5
        mov    dword ptr [eax + 10h], 1
        mov    dword ptr [eax], 7141h
        call   sub_1_9B3C
loc_1_9DB5:
        call   sub_1_9B74
        ret
sub_1_9DBB:
        push   ecx
        push   ebx
        push   ebp
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 8001h
        mov    eax, 7160h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_1_9DE3
        mov    eax, 1
        jmp    SHORT loc_1_9DE5
loc_1_9DE3:
        xor    eax, eax
loc_1_9DE5:
        pop    edi
        pop    esi
        pop    ebp
        pop    ebx
        pop    ecx
        ret
        db  090h
sub_1_9DEC:
        db  0A1h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [eax + 30h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    eax, dword ptr [eax + 20h]
        mov    di, word ptr [eax + 1ah]
        call   sub_1_9AE8
        jb     NEAR PTR loc_1_9E95
        lea    edi, [eax + 2ah]
        mov    dword ptr [eax + 0ch], edi
        mov    word ptr [eax + 28h], 0ffffh
        call   sub_1_9BB8
        jb     SHORT loc_1_9E95
        push   eax
        push   edi
        lea    edi, [eax + 2ah]
        xor    eax, eax
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        dec    ecx
        pop    edi
        pop    eax
        cmp    ecx, 3
        jne    SHORT loc_1_9E3B
        clc
        jmp    SHORT loc_1_9E95
loc_1_9E3B:
        mov    word ptr [eax], 7143h
        mov    byte ptr [eax + 4], 0
        call   sub_1_9B3C
        jb     SHORT loc_1_9E56
        mov    ecx, dword ptr [ebp + 18h]
        test   ecx, 10h
        jne    SHORT loc_1_9E95
loc_1_9E56:
        mov    word ptr [eax], 3
        stc
        jmp    SHORT loc_1_9E95
loc_1_9E5E:
        db  0A1h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [eax + 30h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    eax, dword ptr [eax + 20h]
        mov    di, word ptr [eax + 1ah]
        call   sub_1_9AE8
        jb     SHORT loc_1_9E95
        lea    edi, [eax + 2ah]
        mov    dword ptr [eax + 0ch], edi
        mov    word ptr [eax + 28h], cx
        call   sub_1_9BB8
        jb     SHORT loc_1_9E95
        mov    dh, 71h
        mov    dword ptr [eax], edx
        call   sub_1_9B3C
loc_1_9E95:
        call   sub_1_9B74
        ret
        db  090h
sub_1_9E9C:
        mov    dl, 39h
        mov    cx, 4
        jmp    SHORT loc_1_9E5E
sub_1_9EA4:
        mov    dl, 3ah
        mov    cx, 10h
        jmp    SHORT loc_1_9E5E
sub_1_9EAC:
        push   eax
        call   sub_1_9AE8
        jb     NEAR PTR loc_1_9F39
        mov    dword ptr [eax + 8], edx
        lea    edi, [eax + 2ah]
        mov    dword ptr [eax + 0ch], edi
        mov    word ptr [eax + 28h], 14h
        call   sub_1_9BB8
        pop    ebp
        jb     SHORT loc_1_9F33
        call   sub_1_9B0C
        jb     SHORT loc_1_9F33
        mov    edi, dword ptr [eax + 20h]
        mov    dword ptr [eax + 14h], edi
        call   sub_1_9BB8
        jb     SHORT loc_1_9F33
        push   ecx
        push   eax
        push   edx
        push   edi
        push   esi
        push   ebx
        mov    edx, dword ptr [eax + 0ch]
        mov    edi, dword ptr [eax + 14h]
        mov    esi, edi
        cld
loc_1_9EF2:
        mov    ecx, 0ffffffffh
        mov    edi, esi
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    ebx, ecx
        mov    al, 2ah
        mov    edi, esi
        repne scasb
        je     SHORT loc_1_9F21
        mov    ecx, ebx
        mov    al, 3fh
        mov    edi, esi
        repne scasb
        je     SHORT loc_1_9F21
        mov    ecx, 7156h
        cmp    esi, edx
        je     SHORT loc_1_9F26
        mov    esi, edx
        jmp    SHORT loc_1_9EF2
loc_1_9F21:
        mov    ecx, 5600h
loc_1_9F26:
        pop    ebx
        pop    esi
        pop    edi
        pop    edx
        pop    eax
        mov    dword ptr [eax], ecx
        pop    ecx
        call   sub_1_9B3C
loc_1_9F33:
        call   sub_1_9B74
        ret
loc_1_9F39:
        pop    ebp
        jmp    SHORT loc_1_9F33
sub_1_9F3C:
        cmp    dword ptr [ebp], 10002h
        je     NEAR PTR loc_1_A037
        db  0A1h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [eax + 30h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    eax, dword ptr [eax + 20h]
        mov    di, word ptr [eax + 1ah]
        call   sub_1_9AE8
        jb     NEAR PTR loc_1_A046
        or     edx, edx
        jne    SHORT loc_1_9F74
        mov    word ptr [eax + 28h], 2021h
loc_1_9F74:
        call   sub_1_9B0C
        jb     NEAR PTR loc_1_A046
        mov    dword ptr [eax + 8], 0feh
        lea    edi, [eax + 2ah]
        mov    dword ptr [eax + 0ch], edi
        call   sub_1_9BB8
        jb     NEAR PTR loc_1_A046
        mov    dword ptr [eax], 714eh
        mov    dword ptr [eax + 10h], 0
        call   sub_1_9B2C
        jb     NEAR PTR loc_1_A046
        call   sub_1_9B3C
        jb     NEAR PTR loc_1_A046
        mov    ebx, dword ptr [eax]
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax], 71a1h
        call   sub_1_9B3C
        test   edx, edx
        jne    SHORT loc_1_A00B
        mov    esi, eax
        mov    edi, dword ptr [eax + 20h]
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        call   sub_1_9D3F
        add    esp, 8
        push   eax
        push   dword ptr [edi + 10h]
        push   dword ptr [edi + 0ch]
        call   sub_1_9D3F
        add    esp, 8
        push   eax
        push   dword ptr [edi + 18h]
        push   dword ptr [edi + 14h]
        call   sub_1_9D3F
        add    esp, 8
        pop    ecx
        pop    edx
        mov    ebx, dword ptr [edi]
        mov    ebp, dword ptr [edi + 20h]
        xchg   esi, eax
        mov    edi, ebp
        jmp    SHORT loc_1_A02E
loc_1_A00B:
        push   eax
        mov    esi, dword ptr [eax + 20h]
        lea    esi, [esi + 12bh]
        mov    edi, edx
        mov    al, cl
        mov    ecx, 0ffffffffh
        mov    edx, 1ch
        sub    edx, 4
        call   sub_1_CBB1
        mov    esi, eax
        pop    eax
loc_1_A02E:
        call   sub_1_9B74
        mov    eax, esi
loc_1_A035:
        clc
        ret
loc_1_A037:
        xor    eax, eax
        xor    ebp, ebp
        xor    ecx, ecx
        xor    edx, edx
        xor    edi, edi
        lea    ebx, [eax + 10h]
        jmp    SHORT loc_1_A035
loc_1_A046:
        cmp    dword ptr [eax], 12h
        jne    SHORT loc_1_A051
        mov    dword ptr [eax], 2
loc_1_A051:
        call   sub_1_9B74
        stc
        ret
sub_1_A058:
        test   eax, eax
        jne    SHORT loc_1_A091
        call   sub_1_9AE8
        jb     SHORT loc_1_A08B
        mov    word ptr [eax + 28h], 2
        mov    dword ptr [eax + 8], ebx
        lea    edi, [eax + 2ah]
        mov    dword ptr [eax + 0ch], edi
        call   sub_1_9BB8
        jb     SHORT loc_1_A08B
        mov    dword ptr [eax], 7143h
        mov    dword ptr [eax + 4], 1
        call   sub_1_9B3C
loc_1_A08B:
        call   sub_1_9B74
        ret
loc_1_A091:
        mov    al, 32h
        stc
        ret
        db  000h, 000h, 000h
sub_1_A098:
        jecxz  loc_1_A0EB
        imul   ecx, ecx, 2bh
        sub    edx, ecx
        sbb    eax, eax
        and    edx, eax
        add    edx, ecx
        call   sub_1_9AE8
        jb     NEAR PTR loc_1_A156
        call   sub_1_9B0C
        jb     NEAR PTR loc_1_A156
        and    bl, 0feh
        cmp    bx, 1016h
        jne    SHORT loc_1_A0C9
        mov    bx, 0d7h
loc_1_A0C9:
        mov    dword ptr [eax + 8], ebx
        push   0
        push   edi
        lea    edi, [eax + 2ah]
        mov    dword ptr [eax + 0ch], edi
        mov    word ptr [eax + 28h], 80h
        call   sub_1_9BB8
        pop    edi
        jb     SHORT loc_1_A14C
        mov    dword ptr [eax], 4e00h
        jmp    SHORT loc_1_A11D
loc_1_A0EB:
        ret
sub_1_A0EC:
        jecxz  loc_1_A0EB
        imul   ecx, ecx, 2bh
        sub    edx, ecx
        sbb    eax, eax
        and    edx, eax
        add    edx, ecx
        call   sub_1_9AE8
        jb     SHORT loc_1_A156
        call   sub_1_9B0C
        jb     SHORT loc_1_A156
        push   edi
        mov    esi, ebx
        mov    edi, dword ptr [eax + 20h]
        mov    ecx, 15h
        rep movsb
        pop    edi
        push   0
loc_1_A117:
        mov    dword ptr [eax], 4f00h
loc_1_A11D:
        sub    edx, 2bh
        jb     SHORT loc_1_A14C
        call   sub_1_9B3C
        jb     SHORT loc_1_A14C
        mov    esi, dword ptr [eax + 20h]
        test   byte ptr [esi + 15h], 10h
        jne    SHORT loc_1_A138
        cmp    byte ptr [esi + 0ch], 0d7h
        je     SHORT loc_1_A147
loc_1_A138:
        mov    ecx, 0ah
        rep movsd
        movsw
        movsb
        inc    dword ptr [esp]
        jmp    SHORT loc_1_A117
loc_1_A147:
        add    edx, 2bh
        jmp    SHORT loc_1_A117
loc_1_A14C:
        pop    ecx
        jecxz  loc_1_A156
        mov    dword ptr [eax], 0
        clc
loc_1_A156:
        call   sub_1_9B74
        ret
loc_1_A15C:
        mov    eax, 32h
        ret
        db  000h, 000h
sub_1_A164:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   100h
        call   sub_1_8CB6
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_A184
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_A184:
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ecx + 2ch]
        call   sub_1_9DBB
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_A230
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    ebx, [ecx - 1]
        movzx  ecx, bx
        lea    eax, [ecx + 2]
        push   eax
        call   sub_1_8CB6
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 30h], eax
        mov    edx, eax
        or     edx, edx
        je     SHORT loc_1_A230
        mov    edi, esi
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
        cmp    bx, 3
        jbe    SHORT loc_1_A21D
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 30h]
        movzx  ecx, bx
        add    eax, ecx
        cmp    byte ptr [eax - 1], 5ch
        jne    SHORT loc_1_A20B
        movzx  ecx, byte ptr [eax - 2]
        db  080h, 0B9h, 058h, 037h, 001h, 000h, 002h
        jne    SHORT loc_1_A21D
loc_1_A20B:
        mov    byte ptr [eax], 5ch
        inc    bx
        movzx  eax, bx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 30h]
        mov    byte ptr [eax + ecx], 0
loc_1_A21D:
        pop    edi
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 44h], bl
        mov    eax, 1
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_A230:
        push   esi
        call   sub_1_8CF5
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_A240:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    ebx, [ecx - 1]
        movzx  eax, bx
        add    eax, 47h
        push   eax
        call   sub_1_8CB6
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_A274
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_A274:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 11h
        rep stosd
        stosb
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [edx + 11h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    edi, [edx + 45h]
        mov    dword ptr [edx + 2ch], edi
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [edx + 45h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    byte ptr [ebp + 0ch], 0
        jne    SHORT loc_1_A300
        cmp    bx, 3
        jbe    SHORT loc_1_A300
        movzx  eax, bx
        add    eax, dword ptr [edx + 2ch]
        cmp    byte ptr [eax - 1], 5ch
        jne    SHORT loc_1_A2F1
        movzx  ecx, byte ptr [eax - 2]
        db  080h, 0B9h, 058h, 037h, 001h, 000h, 002h
        jne    SHORT loc_1_A300
loc_1_A2F1:
        mov    byte ptr [eax], 5ch
        inc    bx
        movzx  eax, bx
        mov    ecx, dword ptr [edx + 2ch]
        mov    byte ptr [eax + ecx], 0
loc_1_A300:
        mov    byte ptr [edx + 0ah], bl
        mov    al, byte ptr [ebp + 18h]
        mov    byte ptr [edx + 10h], al
        mov    edi, dword ptr [ebp + 14h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [edx + 1eh]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ax, word ptr [ebp + 1ch]
        mov    word ptr [edx + 40h], ax
        mov    word ptr [edx + 42h], 0
        mov    al, byte ptr [ebp + 0ch]
        mov    byte ptr [edx + 9], al
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [edx + 34h], eax
        mov    byte ptr [edx + 8], 2
        mov    word ptr [edx + 0eh], 0
        mov    dword ptr [edx + 38h], 0
        mov    dword ptr [edx + 3ch], 0
        mov    word ptr [edx + 0ch], 0ffffh
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax]
        or     al, 20h
        sub    al, 61h
        mov    byte ptr [edx + 0bh], al
        mov    eax, edx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
sub_1_A378:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, 1
        mov    edx, dword ptr [ebp + 8]
        mov    eax, edx
        mov    esi, dword ptr [ebp + 0ch]
        cmp    esi, edx
        je     SHORT loc_1_A395
loc_1_A38E:
        mov    eax, dword ptr [eax]
        inc    edi
        cmp    eax, esi
        jne    SHORT loc_1_A38E
loc_1_A395:
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
        db  029h, 03Dh, 0F1h, 024h, 001h, 000h
        db  0A1h, 008h, 01Ch, 001h, 000h
        mov    dword ptr [esi], eax
        db  0C7h, 042h, 004h, 008h, 01Ch, 001h, 000h
        mov    dword ptr [eax + 4], esi
        db  089h, 015h, 008h, 01Ch, 001h, 000h
        db  068h, 030h, 01Ch, 001h, 000h
        call   sub_1_8BBC
        add    esp, 4
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_A3D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 108h
        db  068h, 0D0h, 033h, 001h, 000h
        call   sub_1_7E77
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_A45F
        lea    eax, [ebp - 4]
        push   eax
        push   101h
        lea    eax, [ebp - 108h]
        push   eax
        push   1
        push   0
        push   0
        call   sub_1_9282
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_1_A45F
        db  068h, 0DCh, 033h, 001h, 000h
        push   4
        push   89h
        db  068h, 0E0h, 033h, 001h, 000h
        lea    eax, [ebp - 108h]
        push   eax
        push   0
        db  068h, 0E4h, 033h, 001h, 000h
        call   sub_1_A240
        add    esp, 1ch
        or     eax, eax
        je     SHORT loc_1_A45F
        db  0FFh, 005h, 0F1h, 024h, 001h, 000h
        or     word ptr [eax + 42h], 1
        db  08Bh, 00Dh, 000h, 01Ch, 001h, 000h
        mov    dword ptr [eax], ecx
        db  08Bh, 00Dh, 000h, 01Ch, 001h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A3h, 000h, 01Ch, 001h, 000h
        db  0C7h, 040h, 004h, 000h, 01Ch, 001h, 000h
loc_1_A45F:
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_A464:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [edi + 2]
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 11h], 1
        jne    SHORT loc_1_A484
        mov    eax, 41h
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_A484:
        push   1
        push   esi
        call   sub_1_6674
        add    esp, 8
        cmp    word ptr [edi + 6], 32h
        je     SHORT loc_1_A4A0
        mov    eax, 7ch
        pop    edi
        pop    esi
        leave
        ret
        db  090h
loc_1_A4A0:
        push   esi
        call   sub_1_A4E0
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_A4DA
        mov    ax, word ptr [esi + 0eh]
        test   ah, 1
        je     SHORT loc_1_A4DA
        and    eax, 0ffffh
        push   eax
        movzx  eax, byte ptr [esi + 0dh]
        push   eax
        lea    eax, [esi + 18h]
        push   eax
        lea    eax, [esi + 21h]
        push   eax
        push   dword ptr [esi + 10h]
        push   dword ptr [esi + 14h]
        push   esi
        call   sub_1_DB5C
        add    esp, 1ch
loc_1_A4DA:
        mov    eax, edi
        pop    edi
        pop    esi
        leave
        ret
sub_1_A4E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 14h]
        mov    dword ptr [ebp - 0ch], eax
        call   sub_1_E1B4
        movzx  eax, byte ptr [ebx + 0dh]
        or     eax, eax
        je     SHORT loc_1_A528
        cmp    eax, 1
        je     NEAR PTR loc_1_A5A0
        cmp    eax, 2
        je     NEAR PTR loc_1_A5EF
        cmp    eax, 3
        je     NEAR PTR loc_1_A600
        mov    esi, 57h
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_A528:
        db  083h, 03Dh, 098h, 034h, 001h, 000h, 000h
        jne    SHORT loc_1_A540
        mov    esi, 842h
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_A540:
        sub    eax, eax
        mov    edi, dword ptr [ebp - 0ch]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 3
        jb     SHORT loc_1_A594
        mov    eax, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [eax]
        cmp    cl, 41h
        jl     SHORT loc_1_A594
        cmp    cl, 5ah
        jg     SHORT loc_1_A594
        cmp    byte ptr [eax + 1], 3ah
        jne    SHORT loc_1_A594
        cmp    byte ptr [eax + 2], 5ch
        jne    SHORT loc_1_A594
        movsx  eax, cl
        sub    eax, 40h
        push   eax
        call   sub_1_99F7
        add    esp, 4
        test   al, 14h
        jne    SHORT loc_1_A586
        or     eax, eax
        jne    SHORT loc_1_A5EF
loc_1_A586:
        pop    edi
        mov    esi, 844h
        mov    eax, esi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_A594:
        pop    edi
        mov    esi, 844h
        mov    eax, esi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_A5A0:
        db  083h, 03Dh, 094h, 034h, 001h, 000h, 000h
        jne    SHORT loc_1_A5B8
        mov    esi, 842h
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_A5B8:
        push   0
        lea    eax, [ebp - 18h]
        push   eax
        push   dword ptr [ebp - 0ch]
        call   sub_1_4C14
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_A5E4
        cmp    eax, 871h
        je     SHORT loc_1_A5E0
        mov    esi, 844h
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_A5E0:
        or     eax, eax
        jne    SHORT loc_1_A5EF
loc_1_A5E4:
        push   dword ptr [ebp - 18h]
        call   sub_1_926A
        add    esp, 4
loc_1_A5EF:
        cmp    byte ptr [ebx], 0
        jne    SHORT loc_1_A614
        mov    esi, 57h
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_A600:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_A5EF
        mov    esi, 57h
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_A614:
        push   ebx
        call   sub_1_7E77
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_A630
        mov    esi, 846h
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_A630:
        mov    edi, dword ptr [ebx + 10h]
        or     edi, edi
        mov    ecx, 1
        je     SHORT loc_1_A647
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
loc_1_A647:
        push   ecx
        call   sub_1_8CB6
        add    esp, 4
        mov    dword ptr [ebp - 14h], eax
        or     eax, eax
        jne    SHORT loc_1_A664
        mov    esi, 8
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_A664:
        mov    edi, dword ptr [ebx + 10h]
        or     edi, edi
        je     SHORT loc_1_A690
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [ebp - 14h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_1_A696
        db  08Dh, 049h, 000h
loc_1_A690:
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [eax], 0
loc_1_A696:
        mov    cx, word ptr [ebx + 0eh]
        mov    ax, cx
        and    ax, 200h
        cmp    ax, 1
        cmc
        sbb    edi, edi
        and    edi, 4
        mov    ax, cx
        and    ax, 100h
        or     ax, ax
        mov    ax, 0
        setne  al
        or     di, ax
        sub    esi, esi
        mov    dword ptr [ebp - 4], esi
        test   cl, 2
        je     SHORT loc_1_A6E9
        push   dword ptr [ebp - 14h]
        push   edi
        push   0bfh
        lea    eax, [ebx + 18h]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    al, byte ptr [ebx + 0dh]
        push   eax
        push   ebx
        call   sub_1_A240
        add    esp, 1ch
        mov    dword ptr [ebp - 4], eax
loc_1_A6E9:
        test   byte ptr [ebx + 0eh], 1
        je     SHORT loc_1_A70E
        push   dword ptr [ebp - 14h]
        push   edi
        push   89h
        lea    eax, [ebx + 21h]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    al, byte ptr [ebx + 0dh]
        push   eax
        push   ebx
        call   sub_1_A240
        add    esp, 1ch
        mov    esi, eax
loc_1_A70E:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_A738
        or     esi, esi
        je     SHORT loc_1_A72C
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], esi
        mov    dword ptr [esi + 4], eax
        db  083h, 005h, 0F1h, 024h, 001h, 000h, 002h
        jmp    SHORT loc_1_A749
        db  08Dh, 049h, 000h
loc_1_A72C:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_A738
        mov    esi, dword ptr [ebp - 4]
        jmp    SHORT loc_1_A743
        db  090h
loc_1_A738:
        or     esi, esi
        je     NEAR PTR loc_1_A8D4
        mov    dword ptr [ebp - 4], esi
loc_1_A743:
        db  0FFh, 005h, 0F1h, 024h, 001h, 000h
loc_1_A749:
        db  0A1h, 000h, 01Ch, 001h, 000h
        mov    dword ptr [esi], eax
        db  0A1h, 000h, 01Ch, 001h, 000h
        mov    dword ptr [eax + 4], esi
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 000h, 01Ch, 001h, 000h
        db  0C7h, 040h, 004h, 000h, 01Ch, 001h, 000h
        cmp    byte ptr [ebx + 0dh], 0
        jne    NEAR PTR loc_1_A8B1
        push   1fh
        call   sub_1_8CB6
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        or     eax, eax
        je     NEAR PTR loc_1_A8B1
        db  0BEh, 0F0h, 033h, 001h, 000h
        mov    edi, eax
        movsw
        movsb
        db  0BFh, 092h, 024h, 001h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, dword ptr [ebp - 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        db  0BEh, 0F4h, 033h, 001h, 000h
        mov    edi, dword ptr [ebp - 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        movsw
        mov    edi, dword ptr [ebp - 4]
        add    edi, 11h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, dword ptr [ebp - 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [eax + 2ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        call   sub_1_8CB6
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_1_A8A6
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [eax + 2ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [ebp - 8]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [ecx + eax - 1]
        push   eax
        push   dword ptr [ebp - 8]
        call   sub_1_E2C8
        add    esp, 8
        mov    edx, eax
        cmp    byte ptr [edx], 5ch
        jne    SHORT loc_1_A88D
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 3
        jbe    SHORT loc_1_A88D
        mov    byte ptr [edx], 0
loc_1_A88D:
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 10h]
        call   sub_1_8C74
        add    esp, 8
        push   dword ptr [ebp - 8]
        call   sub_1_8CF5
        add    esp, 4
loc_1_A8A6:
        push   dword ptr [ebp - 10h]
        call   sub_1_8CF5
        add    esp, 4
loc_1_A8B1:
        cmp    byte ptr [ebx + 0dh], 1
        jne    SHORT loc_1_A8CB
        call   sub_1_A3D0
        push   1
        call   sub_1_DA7C
        add    esp, 4
        call   sub_1_9252
loc_1_A8CB:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_A8D4:
        mov    esi, 8
        push   dword ptr [ebp - 14h]
        call   sub_1_8CF5
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_A8EC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 11h], 1
        jne    SHORT loc_1_A910
        mov    eax, 41h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_A910:
        mov    eax, dword ptr [ebp + 0ch]
        push   dword ptr [eax + 2]
        call   sub_1_7E77
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_A934
        mov    dword ptr [ebp - 8], 958h
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_A934:
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [eax + 34h]
        cmp    dword ptr [ebx + 34h], ecx
        jne    SHORT loc_1_A94C
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [eax + 34h], 0
        jmp    SHORT loc_1_A94F
        db  08Bh, 0FFh
loc_1_A94C:
        mov    dword ptr [ebp - 0ch], ebx
loc_1_A94F:
        cmp    byte ptr [ebx + 9], 0
        jne    NEAR PTR loc_1_AA02
        push   1fh
        call   sub_1_8CB6
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     NEAR PTR loc_1_AA02
        db  0BEh, 0F8h, 033h, 001h, 000h
        mov    edi, eax
        movsw
        movsb
        db  0BFh, 092h, 024h, 001h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        db  0BEh, 0FCh, 033h, 001h, 000h
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        movsw
        lea    edi, [ebx + 11h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        push   dword ptr [ebp - 4]
        call   sub_1_8C7A
        add    esp, 4
        push   dword ptr [ebp - 4]
        call   sub_1_8CF5
        add    esp, 4
loc_1_AA02:
        push   dword ptr [ebp - 0ch]
        push   ebx
        call   sub_1_A378
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   dword ptr [eax + 2]
        call   sub_1_DEF8
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_AA24:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [esi + 6]
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 11h], 1
        jne    SHORT loc_1_AA48
        mov    eax, 41h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_AA48:
        cmp    word ptr [esi + 0ah], 32h
        je     SHORT loc_1_AA5C
        mov    eax, 7ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_AA5C:
        push   dword ptr [esi + 0ch]
        call   sub_1_7E77
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_AA78
        mov    eax, 958h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_AA78:
        sub    eax, eax
        mov    edi, dword ptr [edx + 2ch]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        lea    edi, [ecx - 1]
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [edx + 34h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, dword ptr [ebp - 4]
        lea    ecx, [ecx + edi - 1]
        add    cx, 2ch
        mov    eax, dword ptr [esi]
        mov    word ptr [eax], cx
        mov    eax, dword ptr [esi]
        mov    ax, word ptr [eax]
        cmp    word ptr [esi + 4], ax
        jae    SHORT loc_1_AAC0
        mov    eax, 84bh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_AAC0:
        sub    eax, eax
        lea    edi, [edx + 11h]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    al, byte ptr [edx + 9]
        mov    byte ptr [ebx + 0dh], al
        cmp    byte ptr [edx + 10h], 0bfh
        jne    SHORT loc_1_AB0C
        mov    eax, dword ptr [edx]
        mov    ecx, dword ptr [edx + 34h]
        cmp    dword ptr [eax + 34h], ecx
        jne    SHORT loc_1_AB00
        mov    word ptr [ebx + 0eh], 3
        jmp    SHORT loc_1_AB15
loc_1_AB00:
        lea    eax, [ebx + 0eh]
        mov    word ptr [ebx + 0eh], 2
        jmp    SHORT loc_1_AB15
        db  090h
loc_1_AB0C:
        lea    eax, [ebx + 0eh]
        mov    word ptr [ebx + 0eh], 1
loc_1_AB15:
        mov    ax, word ptr [edx + 40h]
        test   al, 1
        je     SHORT loc_1_AB23
        or     word ptr [ebx + 0eh], 100h
loc_1_AB23:
        test   al, 4
        je     SHORT loc_1_AB2D
        or     word ptr [ebx + 0eh], 200h
loc_1_AB2D:
        sub    eax, eax
        lea    edi, [ebx + 2ah]
        mov    dword ptr [ebx + 14h], edi
        mov    edi, dword ptr [edx + 2ch]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 2ah]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    byte ptr [edx + 9], 0
        jne    SHORT loc_1_AB6D
        cmp    dword ptr [ebp - 4], 3
        jbe    SHORT loc_1_AB6D
        mov    eax, dword ptr [ebx + 14h]
        mov    edi, dword ptr [ebp - 4]
        mov    byte ptr [eax + edi - 1], 0
loc_1_AB6D:
        mov    esi, dword ptr [edx + 34h]
        or     esi, esi
        je     SHORT loc_1_ABB0
        mov    edi, dword ptr [ebx + 14h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx + ebx - 1]
        add    eax, 2bh
        mov    dword ptr [ebx + 10h], eax
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [ebx + 10h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_1_ABB7
loc_1_ABB0:
        mov    dword ptr [ebx + 10h], 0
loc_1_ABB7:
        lea    edi, [edx + 1eh]
        test   byte ptr [ebx + 0eh], 2
        mov    ecx, 0ffffffffh
        je     SHORT loc_1_AC20
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 18h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        test   byte ptr [ebx + 0eh], 1
        je     SHORT loc_1_AC14
        mov    edi, dword ptr [edx]
        add    edi, 1eh
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 21h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_AC14:
        sub    eax, eax
        mov    byte ptr [ebx + 21h], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_AC20:
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 21h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    byte ptr [ebx + 18h], 0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_AC48:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 11h], 1
        jne    SHORT loc_1_AC74
        mov    eax, 41h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_AC74:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    word ptr [eax + 8], 32h
        je     SHORT loc_1_AC88
        mov    eax, 7ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_AC88:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    word ptr [eax], 0
        je     SHORT loc_1_AC9C
        mov    eax, 57h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_AC9C:
        mov    eax, dword ptr [ebp + 0ch]
        push   dword ptr [eax + 0ah]
        call   sub_1_7E77
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    SHORT loc_1_ACC0
        mov    dword ptr [ebp - 18h], 958h
        mov    eax, dword ptr [ebp - 18h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_ACC0:
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ecx]
        mov    edi, ecx
        add    edi, 34h
        mov    dword ptr [ebp - 14h], edi
        mov    edi, dword ptr [edi]
        cmp    dword ptr [ebx + 34h], edi
        jne    SHORT loc_1_ACD8
        mov    al, 1
        jmp    SHORT loc_1_ACDD
loc_1_ACD8:
        sub    al, al
        mov    ebx, dword ptr [ebp - 4]
loc_1_ACDD:
        or     al, al
        je     SHORT loc_1_AD20
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0eh
        mov    dword ptr [ebp - 10h], eax
        mov    ax, word ptr [eax]
        mov    cx, ax
        and    cl, 3
        cmp    cl, 3
        jne    SHORT loc_1_AD04
        mov    dword ptr [ebp - 0ch], 4
        jmp    NEAR PTR loc_1_AD9A
loc_1_AD04:
        test   al, 2
        je     SHORT loc_1_AD14
        mov    dword ptr [ebp - 0ch], 8
        jmp    NEAR PTR loc_1_AD9A
loc_1_AD14:
        mov    dword ptr [ebp - 0ch], 7
        jmp    NEAR PTR loc_1_AD9A
loc_1_AD20:
        mov    ecx, dword ptr [ebp - 4]
        cmp    byte ptr [ecx + 10h], 0bfh
        mov    eax, dword ptr [ebp - 8]
        jne    SHORT loc_1_AD68
        add    eax, 0eh
        mov    dword ptr [ebp - 10h], eax
        mov    ax, word ptr [eax]
        mov    cx, ax
        and    cl, 3
        cmp    cl, 3
        jne    SHORT loc_1_AD4C
        mov    dword ptr [ebp - 0ch], 6
        jmp    SHORT loc_1_AD9A
        db  08Dh, 049h, 000h
loc_1_AD4C:
        test   al, 2
        je     SHORT loc_1_AD5C
        mov    dword ptr [ebp - 0ch], 1
        jmp    SHORT loc_1_AD9A
        db  08Dh, 049h, 000h
loc_1_AD5C:
        mov    dword ptr [ebp - 0ch], 3
        jmp    SHORT loc_1_AD9A
        db  08Dh, 049h, 000h
loc_1_AD68:
        add    eax, 0eh
        mov    dword ptr [ebp - 10h], eax
        mov    ax, word ptr [eax]
        mov    cx, ax
        and    cl, 3
        cmp    cl, 3
        jne    SHORT loc_1_AD88
        mov    dword ptr [ebp - 0ch], 5
        jmp    SHORT loc_1_AD9A
        db  08Dh, 049h, 000h
loc_1_AD88:
        test   al, 2
        mov    dword ptr [ebp - 0ch], 2
        jne    SHORT loc_1_AD9A
        mov    dword ptr [ebp - 0ch], 0
loc_1_AD9A:
        or     edi, edi
        je     SHORT loc_1_ADB0
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edx, [ecx - 1]
        jmp    SHORT loc_1_ADB2
        db  08Bh, 0FFh
loc_1_ADB0:
        sub    edx, edx
loc_1_ADB2:
        mov    eax, dword ptr [ebp - 8]
        mov    edi, dword ptr [eax + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, edx
        jbe    SHORT loc_1_AE30
        push   ecx
        call   sub_1_8CB6
        add    esp, 4
        mov    dword ptr [ebp - 1ch], eax
        or     eax, eax
        je     SHORT loc_1_AE20
        mov    eax, dword ptr [ebp - 8]
        mov    edi, dword ptr [eax + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [ebp - 1ch]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebp - 14h]
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     SHORT loc_1_AE10
        push   eax
        call   sub_1_8CF5
        add    esp, 4
loc_1_AE10:
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebx + 34h], eax
        mov    edi, dword ptr [ebp - 14h]
        mov    dword ptr [edi], eax
        jmp    SHORT loc_1_AE58
        db  08Dh, 049h, 000h
loc_1_AE20:
        pop    edi
        mov    dword ptr [ebp - 18h], 8
        mov    eax, dword ptr [ebp - 18h]
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_AE30:
        mov    eax, dword ptr [ebp - 8]
        mov    edi, dword ptr [eax + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [ebp - 14h]
        mov    edi, dword ptr [edi]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_AE58:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        mov    cx, ax
        and    cx, 200h
        cmp    cx, 1
        cmc
        sbb    edx, edx
        and    edx, 4
        and    ax, 100h
        or     ax, ax
        mov    ax, 0
        setne  al
        or     dx, ax
        mov    word ptr [ebx + 40h], dx
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 40h], dx
        mov    eax, dword ptr [ebp - 0ch]
        cmp    eax, 5
        je     SHORT loc_1_AEB0
        cmp    eax, 6
        je     SHORT loc_1_AF08
        cmp    eax, 7
        je     NEAR PTR loc_1_AF60
        cmp    eax, 8
        je     NEAR PTR loc_1_AF7C
        jmp    NEAR PTR loc_1_AF90
loc_1_AEB0:
        push   dword ptr [ebx + 34h]
        push   edx
        push   0bfh
        mov    eax, dword ptr [ebp - 8]
        add    eax, 18h
        push   eax
        push   dword ptr [ebx + 2ch]
        mov    al, byte ptr [ebx + 9]
        push   eax
        lea    eax, [ebx + 11h]
        push   eax
        call   sub_1_A240
        add    esp, 1ch
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    SHORT loc_1_AEEC
        mov    dword ptr [ebp - 18h], 8
        mov    eax, dword ptr [ebp - 18h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_AEEC:
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], ebx
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ebx + 4], ecx
        db  0FFh, 005h, 0F1h, 024h, 001h, 000h
        jmp    NEAR PTR loc_1_AF90
        db  090h
loc_1_AF08:
        mov    edi, dword ptr [ebp - 14h]
        push   dword ptr [edi]
        push   edx
        push   89h
        mov    eax, dword ptr [ebp - 8]
        add    eax, 21h
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 2ch]
        mov    al, byte ptr [ecx + 9]
        push   eax
        mov    eax, ecx
        add    eax, 11h
        push   eax
        call   sub_1_A240
        add    esp, 1ch
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_AF48
        mov    dword ptr [ebp - 18h], 8
        mov    eax, dword ptr [ebp - 18h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_AF48:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [ecx], ebx
        db  0FFh, 005h, 0F1h, 024h, 001h, 000h
        jmp    SHORT loc_1_AF90
        db  090h
loc_1_AF60:
        mov    edi, dword ptr [ebp - 14h]
        mov    dword ptr [edi], 0
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 4]
        call   sub_1_A378
        add    esp, 8
        mov    dword ptr [ebp - 4], ebx
        jmp    SHORT loc_1_AF90
loc_1_AF7C:
        push   ebx
        mov    dword ptr [ebx + 34h], 0
        push   ebx
        call   sub_1_A378
        add    esp, 8
        mov    ebx, dword ptr [ebp - 4]
loc_1_AF90:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    eax, 8
        ja     SHORT loc_1_AFDB
        db  0FFh, 024h, 085h, 0C4h, 0B0h, 000h, 000h
        db  090h
loc_1_AFA0:
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 10h], 0bfh
loc_1_AFA7:
        sub    eax, eax
        mov    edx, dword ptr [ebp - 4]
        add    edx, 1eh
        mov    edi, edx
        stosd
        stosd
        stosd
        stosw
        mov    edi, dword ptr [ebp - 8]
        add    edi, 18h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
loc_1_AFCF:
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_AFDB:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        test   ah, 1
        je     NEAR PTR loc_1_B0AC
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        movzx  eax, byte ptr [eax + 0dh]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 18h
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 21h
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   dword ptr [eax + 10h]
        push   0
        push   eax
        call   sub_1_DB5C
        add    esp, 1ch
        mov    eax, dword ptr [ebp - 18h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_B020:
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 10h], 89h
loc_1_B027:
        sub    eax, eax
        add    ebx, 1eh
        mov    edi, ebx
        stosd
        stosd
        stosd
        stosw
        mov    edi, dword ptr [ebp - 8]
        add    edi, 21h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        jmp    SHORT loc_1_AFCF
        db  08Bh, 0FFh
loc_1_B050:
        sub    eax, eax
        mov    edx, dword ptr [ebp - 4]
        add    edx, 1eh
        mov    edi, edx
        stosd
        stosd
        stosd
        stosw
        add    ebx, 1eh
        mov    edi, ebx
        stosd
        stosd
        stosd
        stosw
        mov    edi, dword ptr [ebp - 8]
        add    edi, 18h
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
        mov    edi, dword ptr [ebp - 8]
        add    edi, 21h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        jmp    NEAR PTR loc_1_AFCF
loc_1_B0AC:
        mov    eax, dword ptr [ebp + 0ch]
        push   dword ptr [eax + 0ah]
        call   sub_1_DEF8
        add    esp, 4
        mov    eax, dword ptr [ebp - 18h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh, 027h, 0B0h, 000h, 000h, 0A7h, 0AFh, 000h, 000h, 0A0h, 0AFh, 000h, 000h, 020h, 0B0h
        db  000h, 000h, 050h, 0B0h, 000h, 000h, 027h, 0B0h, 000h, 000h, 0A7h, 0AFh, 000h, 000h, 027h, 0B0h
        db  000h, 000h, 0A7h, 0AFh, 000h, 000h
sub_1_B0E8:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        sub    eax, eax
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    ecx, eax
        je     SHORT loc_1_B103
        nop
loc_1_B0F8:
        cmp    byte ptr [edx], 0
        je     SHORT loc_1_B103
        inc    eax
        inc    edx
        cmp    eax, ecx
        jb     SHORT loc_1_B0F8
loc_1_B103:
        dec    edx
        cmp    byte ptr [edx], 20h
        jne    SHORT loc_1_B114
loc_1_B109:
        or     eax, eax
        je     SHORT loc_1_B114
        dec    eax
        dec    edx
        cmp    byte ptr [edx], 20h
        je     SHORT loc_1_B109
loc_1_B114:
        leave
        ret
        db  08Bh, 0FFh
loc_1_B118:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 24h], 0
        mov    dword ptr [ebp - 30h], 0
        mov    dword ptr [ebp - 2ch], 0
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 11h], 1
        jne    SHORT loc_1_B14C
        mov    eax, 41h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_B14C:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    word ptr [eax + 0eh], 32h
        je     SHORT loc_1_B160
        mov    eax, 7ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_B160:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ah]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 0ch]
        movzx  ecx, word ptr [eax + 8]
        mov    eax, dword ptr [ebp - 10h]
        add    eax, ecx
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_B185
        mov    dword ptr [ebp - 24h], 0eah
loc_1_B185:
        db  0A1h, 0A6h, 024h, 001h, 000h
        mov    dword ptr [ebp - 20h], eax
        or     eax, eax
        je     NEAR PTR loc_1_B2D0
loc_1_B195:
        mov    dword ptr [ebp - 0ch], 0
loc_1_B19C:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 20h]
        mov    ebx, dword ptr [ecx + eax*4 + 0f2h]
        or     ebx, ebx
        je     NEAR PTR loc_1_B2AF
        lea    eax, [ebx + 42h]
        mov    dword ptr [ebp - 1ch], eax
        cmp    word ptr [eax], 0
        jbe    NEAR PTR loc_1_B2AF
        inc    dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 4], ecx
        add    dword ptr [ebp - 10h], 1ah
        lea    esi, [ebx + 4]
        push   0fh
        push   esi
        call   sub_1_B0E8
        add    esp, 8
        mov    edx, eax
        lea    eax, [edx + 1]
        sub    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 18h], eax
        mov    edi, dword ptr [ebx + 38h]
        or     edi, edi
        je     SHORT loc_1_B20F
        add    edi, 8
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        mov    dword ptr [ebp - 14h], eax
        inc    eax
        sub    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 28h], eax
loc_1_B20F:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 10h]
        jae    SHORT loc_1_B21E
        mov    dword ptr [ebp - 24h], 0eah
loc_1_B21E:
        cmp    dword ptr [ebp - 24h], 0
        jne    NEAR PTR loc_1_B2AF
        mov    ecx, dword ptr [ebp - 20h]
        inc    dword ptr [ebp - 30h]
        movsx  eax, byte ptr [ecx]
        shl    eax, 8
        or     eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ax, word ptr [eax]
        mov    word ptr [ecx + 0ch], ax
        mov    ax, word ptr [ebx + 44h]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 028h, 028h, 001h, 000h
        sub    eax, dword ptr [ebx + 30h]
        mov    dword ptr [ecx + 10h], eax
        db  0A1h, 028h, 028h, 001h, 000h
        sub    eax, dword ptr [ebx + 34h]
        mov    dword ptr [ecx + 14h], eax
        mov    al, byte ptr [ebx + 3eh]
        mov    byte ptr [ecx + 18h], al
        mov    edi, dword ptr [ebp - 18h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx]
        mov    byte ptr [eax + edx], 0
        mov    esi, dword ptr [ebx + 38h]
        or     esi, esi
        je     SHORT loc_1_B2AF
        mov    eax, dword ptr [ebp - 14h]
        inc    eax
        add    esi, 8
        mov    edi, dword ptr [ecx + 4]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_B2AF:
        inc    dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 0ch], 100h
        jb     NEAR PTR loc_1_B19C
        mov    ecx, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ecx + 0ch]
        mov    dword ptr [ebp - 20h], ecx
        or     ecx, ecx
        jne    NEAR PTR loc_1_B195
loc_1_B2D0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 4]
        mov    ax, word ptr [ebp - 30h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        mov    ax, word ptr [ebp - 2ch]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 24h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_B2F4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    bx, word ptr [eax]
        mov    cl, bh
        mov    edx, dword ptr [ebp + 8]
        test   byte ptr [edx + 11h], 1
        jne    SHORT loc_1_B318
        mov    eax, 41h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_B318:
        db  08Bh, 035h, 0A6h, 024h, 001h, 000h
        or     esi, esi
        je     NEAR PTR loc_1_B3CC
        and    ecx, 0ffh
loc_1_B32C:
        movsx  edi, byte ptr [esi]
        cmp    edi, ecx
        je     SHORT loc_1_B33A
        mov    esi, dword ptr [esi + 0ch]
        or     esi, esi
        jne    SHORT loc_1_B32C
loc_1_B33A:
        or     esi, esi
        je     NEAR PTR loc_1_B3CC
        movzx  ecx, bl
        lea    esi, [esi + ecx*4 + 0f2h]
        mov    edi, dword ptr [esi]
        or     edi, edi
        je     SHORT loc_1_B3CC
        mov    eax, dword ptr [eax + 2]
        lea    ebx, [edi + 4]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_B36E
loc_1_B35D:
        mov    cl, byte ptr [ebx]
        or     cl, cl
        je     SHORT loc_1_B36E
        cmp    byte ptr [eax], cl
        jne    SHORT loc_1_B388
        inc    eax
        inc    ebx
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_B35D
loc_1_B36E:
        cmp    byte ptr [ebx], 20h
        je     SHORT loc_1_B394
        sub    ebx, edi
        sub    ebx, 4
        cmp    ebx, 0fh
        jge    SHORT loc_1_B394
        mov    eax, 908h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_B388:
        pop    edi
        mov    eax, 908h
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_B394:
        cli
        cmp    byte ptr [edi + 1eh], 2
        jne    SHORT loc_1_B3C2
        cmp    dword ptr [edx + 0ch], edi
        je     SHORT loc_1_B3A4
        mov    byte ptr [edi + 1eh], 5
loc_1_B3A4:
        mov    ecx, dword ptr [esi]
        db  0A1h, 05Ch, 027h, 001h, 000h
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [esi]
        db  0A3h, 05Ch, 027h, 001h, 000h
        db  068h, 030h, 01Ch, 001h, 000h
        call   sub_1_8BBC
        add    esp, 4
loc_1_B3C2:
        sti
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_B3CC:
        pop    edi
        mov    eax, 99dh
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_B3D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 18h], 0
        sub    ecx, ecx
        mov    dword ptr [ebp - 24h], ecx
        mov    dword ptr [ebp - 2ch], ecx
        mov    dword ptr [ebp - 1ch], ecx
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 11h], 1
        jne    SHORT loc_1_B408
        mov    eax, 41h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_B408:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    word ptr [eax + 0eh], 32h
        je     SHORT loc_1_B41C
        mov    eax, 7ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_B41C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ah]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 0ch]
        movzx  edx, word ptr [eax + 8]
        add    edx, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 0ch], edx
        cmp    dword ptr [ebp - 14h], 0
        jne    SHORT loc_1_B43F
        mov    dword ptr [ebp - 18h], 0eah
loc_1_B43F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 30h], edx
        cmp    byte ptr [edx], 5ch
        jne    NEAR PTR loc_1_B63C
        mov    byte ptr [ebp - 1], 0
        db  0A1h, 0A6h, 024h, 001h, 000h
        mov    dword ptr [ebp - 34h], eax
        or     eax, eax
        je     SHORT loc_1_B4C1
loc_1_B461:
        sub    esi, esi
        inc    eax
        mov    eax, dword ptr [ebp - 34h]
        add    eax, 0f2h
loc_1_B46C:
        mov    edi, dword ptr [eax]
        or     edi, edi
        je     SHORT loc_1_B4A8
        cmp    word ptr [edi + 42h], 0
        jbe    SHORT loc_1_B4A8
        mov    ecx, dword ptr [ebp - 30h]
        add    ecx, 2
        lea    ebx, [edi + 4]
        cmp    byte ptr [ecx], 0
        je     SHORT loc_1_B499
loc_1_B488:
        mov    dl, byte ptr [ebx]
        or     dl, dl
        je     SHORT loc_1_B499
        cmp    byte ptr [ecx], dl
        jne    SHORT loc_1_B4A8
        inc    ecx
        inc    ebx
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_1_B488
loc_1_B499:
        cmp    byte ptr [ebx], 20h
        je     SHORT loc_1_B4CC
        sub    ebx, edi
        sub    ebx, 4
        cmp    ebx, 10h
        jae    SHORT loc_1_B4CC
loc_1_B4A8:
        inc    esi
        add    eax, 4
        cmp    esi, 100h
        jb     SHORT loc_1_B46C
        mov    eax, dword ptr [ebp - 34h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 34h], eax
        or     eax, eax
        jne    SHORT loc_1_B461
loc_1_B4C1:
        pop    edi
        mov    eax, 908h
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_B4CC:
        mov    eax, dword ptr [ebp - 34h]
        mov    eax, dword ptr [eax + esi*4 + 0f2h]
        mov    dword ptr [ebp - 2ch], eax
loc_1_B4D9:
        mov    dword ptr [ebp - 28h], 0
        db  0C7h, 045h, 0C8h, 098h, 01Ah, 001h, 000h
        nop
loc_1_B4E8:
        mov    eax, dword ptr [ebp - 38h]
        mov    ebx, dword ptr [eax]
        or     ebx, ebx
        je     NEAR PTR loc_1_B60A
        add    ebx, 0ch
        mov    dword ptr [ebp - 20h], 8
        nop
loc_1_B500:
        test   byte ptr [ebx + 8], 2
        je     NEAR PTR loc_1_B5FB
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_B522
        mov    eax, dword ptr [ebp - 2ch]
        cmp    dword ptr [ebx - 4], eax
        je     SHORT loc_1_B53C
        cmp    byte ptr [ebp - 1], 0
        je     NEAR PTR loc_1_B5FB
loc_1_B522:
        mov    eax, dword ptr [ebp - 1ch]
        cmp    dword ptr [ebx], eax
        je     SHORT loc_1_B53C
        cmp    byte ptr [ebp - 1], 0
        je     NEAR PTR loc_1_B5FB
        cmp    dword ptr [ebx], 0
        jne    NEAR PTR loc_1_B5FB
loc_1_B53C:
        inc    dword ptr [ebp - 24h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 8], ecx
        add    dword ptr [ebp - 14h], 10h
        push   0ch
        mov    eax, dword ptr [ebx]
        add    eax, 11h
        push   eax
        call   sub_1_B0E8
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        inc    eax
        sub    dword ptr [ebp - 0ch], eax
        mov    edi, dword ptr [ebp - 0ch]
        push   14h
        mov    eax, dword ptr [ebx - 4]
        add    eax, 4
        push   eax
        call   sub_1_B0E8
        add    esp, 8
        lea    ecx, [eax + 1]
        sub    dword ptr [ebp - 0ch], ecx
        mov    edx, dword ptr [ebp - 0ch]
        cmp    edx, dword ptr [ebp - 14h]
        jae    SHORT loc_1_B589
        mov    dword ptr [ebp - 18h], 0eah
loc_1_B589:
        cmp    dword ptr [ebp - 18h], 0
        jne    SHORT loc_1_B5FB
        mov    dx, word ptr [ebx - 8]
        mov    ecx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 28h]
        mov    word ptr [ecx + 2], dx
        mov    esi, dword ptr [ebx]
        movsx  dx, byte ptr [esi + 9]
        mov    word ptr [ecx], dx
        mov    dword ptr [ecx + 8], edi
        mov    edx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 0ch], edx
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [ebp - 10h]
        add    esi, 11h
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [edx + ecx], 0
        mov    esi, dword ptr [ebx - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ecx + 0ch]
        add    esi, 4
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0ch]
        mov    byte ptr [ecx + eax], 0
loc_1_B5FB:
        add    ebx, 41ch
        dec    dword ptr [ebp - 20h]
        jne    NEAR PTR loc_1_B500
loc_1_B60A:
        add    dword ptr [ebp - 38h], 4
        db  081h, 07Dh, 0C8h, 098h, 01Bh, 001h, 000h
        jb     NEAR PTR loc_1_B4E8
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 4]
        mov    ax, word ptr [ebp - 28h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        mov    ax, word ptr [ebp - 24h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 18h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_B63C:
        db  0A1h, 000h, 01Ch, 001h, 000h
        mov    byte ptr [ebp - 1], 1
        mov    dword ptr [ebp - 1ch], eax
        db  08Bh, 015h, 0F1h, 024h, 001h, 000h
        or     edx, edx
        je     SHORT loc_1_B69A
loc_1_B652:
        mov    eax, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [eax + 10h]
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 11h
loc_1_B65E:
        mov    bl, byte ptr [esi]
        cmp    bl, byte ptr [eax]
        jne    SHORT loc_1_B680
        or     bl, bl
        je     SHORT loc_1_B67A
        mov    bl, byte ptr [esi + 1]
        cmp    bl, byte ptr [eax + 1]
        jne    SHORT loc_1_B680
        add    esi, 2
        add    eax, 2
        or     bl, bl
        jne    SHORT loc_1_B65E
loc_1_B67A:
        sub    eax, eax
        jmp    SHORT loc_1_B685
        db  08Bh, 0FFh
loc_1_B680:
        sbb    eax, eax
        sbb    eax, -1
loc_1_B685:
        or     eax, eax
        je     NEAR PTR loc_1_B4D9
        inc    ecx
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 1ch], eax
        cmp    ecx, edx
        jb     SHORT loc_1_B652
loc_1_B69A:
        pop    edi
        mov    eax, 906h
        pop    esi
        pop    ebx
        leave
        ret
loc_1_B6A4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0a8h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 20h], 0
        mov    dword ptr [ebp - 28h], 0
        mov    dword ptr [ebp - 24h], 0
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 11h], 1
        jne    SHORT loc_1_B6D8
        mov    eax, 41h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_B6D8:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    word ptr [eax + 0eh], 32h
        je     SHORT loc_1_B6EC
        mov    eax, 7ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_B6EC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ah]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 0ch]
        movzx  ecx, word ptr [eax + 8]
        mov    eax, dword ptr [ebp - 10h]
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_B711
        mov    dword ptr [ebp - 20h], 0eah
loc_1_B711:
        mov    dword ptr [ebp - 14h], 0
        db  0C7h, 045h, 0F8h, 01Ch, 03Ah, 001h, 000h
        nop
loc_1_B720:
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     NEAR PTR loc_1_B88F
        cmp    byte ptr [esi + 19h], 1
        jne    NEAR PTR loc_1_B88F
        test   byte ptr [esi + 18h], 2
        je     NEAR PTR loc_1_B88F
        lea    eax, [ebp - 0a8h]
        push   eax
        push   dword ptr [esi + 26h]
        call   sub_1_9B79
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 0
        jne    NEAR PTR loc_1_B88F
        inc    dword ptr [ebp - 24h]
        mov    ebx, dword ptr [ebp - 10h]
        add    dword ptr [ebp - 10h], 14h
        lea    edi, [ebp - 0a8h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edi, [ecx - 1]
        push   14h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 0ch]
        add    eax, 4
        push   eax
        call   sub_1_B0E8
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        push   0ch
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 0ch]
        add    eax, 11h
        push   eax
        call   sub_1_B0E8
        add    esp, 8
        mov    edx, eax
        inc    edi
        sub    dword ptr [ebp - 4], edi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        inc    eax
        sub    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [edx + 1]
        sub    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 10h]
        jae    SHORT loc_1_B7DB
        mov    dword ptr [ebp - 20h], 0eah
loc_1_B7DB:
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_1_B88F
        mov    eax, dword ptr [ebp - 14h]
        inc    dword ptr [ebp - 28h]
        db  00Fh, 0B6h, 080h, 01Ch, 04Ah, 001h, 000h
        shl    eax, 8
        add    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 22h]
        shr    eax, 10h
        mov    word ptr [ebx + 4], ax
        mov    word ptr [ebx + 6], 0
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebx + 0ch], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 10h], eax
        lea    edi, [ebp - 0a8h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [ebp - 1ch]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 18h], ecx
        mov    eax, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ecx + 0ch]
        mov    edi, dword ptr [ebx + 0ch]
        add    esi, 4
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebx + 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + eax], 0
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ecx + 10h]
        mov    esi, dword ptr [eax + 0ch]
        mov    edi, dword ptr [ebx + 10h]
        add    esi, 11h
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebx + 10h]
        mov    byte ptr [eax + edx], 0
loc_1_B88F:
        inc    dword ptr [ebp - 14h]
        add    dword ptr [ebp - 8], 4
        db  081h, 07Dh, 0F8h, 01Ch, 04Ah, 001h, 000h
        jb     NEAR PTR loc_1_B720
        mov    cx, word ptr [ebp - 28h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [eax], cx
        mov    edx, dword ptr [ebp - 24h]
        sub    dx, cx
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax], dx
        mov    eax, dword ptr [ebp - 20h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_B8C8:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 11h], 1
        jne    SHORT loc_1_B8DC
        mov    eax, 41h
        leave
        ret
        db  090h
loc_1_B8DC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    dl, ah
        and    eax, 0ffh
        db  08Bh, 00Ch, 085h, 01Ch, 03Ah, 001h, 000h
        or     ecx, ecx
        je     SHORT loc_1_B928
        cmp    byte ptr [ecx + 19h], 1
        jne    SHORT loc_1_B928
        test   byte ptr [ecx + 18h], 2
        je     SHORT loc_1_B928
        db  038h, 090h, 01Ch, 04Ah, 001h, 000h
        jne    SHORT loc_1_B928
        db  0A1h, 058h, 027h, 001h, 000h
        mov    dword ptr [ecx + 2eh], eax
        db  089h, 00Dh, 058h, 027h, 001h, 000h
        db  068h, 030h, 01Ch, 001h, 000h
        call   sub_1_8BBC
        add    esp, 4
        sub    eax, eax
        leave
        ret
        db  08Bh, 0FFh
loc_1_B928:
        mov    eax, 90ah
        leave
        ret
        db  090h
loc_1_B930:
        mov    eax, 32h
        ret
        db  08Bh, 0FFh
sub_1_B938:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        push   160h
        call   sub_1_8CB6
        add    esp, 4
        mov    ebx, eax
        cmp    ebx, esi
        jne    SHORT loc_1_B960
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_B960:
        lea    eax, [ebx + 2ah]
        mov    dword ptr [ebp - 0ch], eax
        add    eax, 105h
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebx + 10h], eax
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        je     SHORT loc_1_B9D8
        db  0BEh, 000h, 034h, 001h, 000h
        mov    edi, ebx
        movsd
        movsw
        movsb
        call   sub_1_9ADC
        mov    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [ebp - 0ch]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [eax], 0
        mov    byte ptr [ebx + 0dh], 0
        mov    word ptr [ebx + 0eh], 202h
        mov    byte ptr [ebx + 18h], 0
        mov    byte ptr [ebx + 21h], 0
        push   ebx
        call   sub_1_A4E0
        add    esp, 4
        mov    esi, eax
loc_1_B9D8:
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 043h, 052h, 0FDh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_B9FC
        push   ebx
        call   sub_1_8CF5
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_B9FC:
        sub    edi, edi
        push   ebx
        push   edi
        push   dword ptr [ebp - 10h]
        db  0E8h, 050h, 052h, 0FDh, 0FFh
        add    esp, 0ch
        cmp    eax, edi
        jne    SHORT loc_1_BA5D
        nop
loc_1_BA10:
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebx + 18h]
        push   eax
        lea    eax, [ebx + 21h]
        push   eax
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 0ch]
        push   ebx
        db  0E8h, 004h, 04Ch, 0FDh, 0FFh
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_1_BA4B
        mov    al, byte ptr [ebp - 8]
        mov    byte ptr [ebx + 0dh], al
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [ebx + 0eh], ax
        push   ebx
        call   sub_1_A4E0
        add    esp, 4
        add    esi, eax
loc_1_BA4B:
        inc    edi
        push   ebx
        push   edi
        push   dword ptr [ebp - 10h]
        db  0E8h, 002h, 052h, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_BA10
loc_1_BA5D:
        push   dword ptr [ebp - 10h]
        db  0E8h, 02Fh, 052h, 0FDh, 0FFh
        add    esp, 4
        push   ebx
        call   sub_1_8CF5
        add    esp, 4
        cmp    esi, 1
        sbb    eax, eax
        neg    eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_BA80:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   esi
        push   edi
        db  0BEh, 008h, 034h, 001h, 000h
        lea    edi, [ebp - 4]
        movsd
        db  0BEh, 010h, 034h, 001h, 000h
        lea    edi, [ebp - 30h]
        movsw
        movsb
        lea    edi, [ebp - 2dh]
        sub    eax, eax
        stosd
        stosd
        stosw
        mov    byte ptr [ebp - 23h], al
        mov    word ptr [ebp - 22h], 202h
        mov    dword ptr [ebp - 20h], eax
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 1ch], eax
        db  0BEh, 018h, 034h, 001h, 000h
        lea    edi, [ebp - 18h]
        movsb
        lea    edi, [ebp - 17h]
        sub    eax, eax
        stosd
        stosd
        db  0BEh, 020h, 034h, 001h, 000h
        lea    edi, [ebp - 0fh]
        movsb
        lea    edi, [ebp - 0eh]
        stosd
        stosd
        mov    edi, dword ptr [ebp + 8]
        cmp    edi, eax
        je     SHORT loc_1_BAFC
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp - 18h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_BAFC:
        mov    esi, 1
loc_1_BB01:
        push   esi
        call   sub_1_99F7
        add    esp, 4
        cmp    eax, 1
        jne    SHORT loc_1_BB1B
        lea    eax, [ebp - 30h]
        push   eax
        call   sub_1_A4E0
        add    esp, 4
loc_1_BB1B:
        inc    esi
        inc    byte ptr [ebp - 4]
        inc    byte ptr [ebp - 30h]
        cmp    esi, 1ah
        jle    SHORT loc_1_BB01
        mov    eax, 1
        pop    edi
        pop    esi
        leave
        ret
sub_1_BB30:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 40h]
        mov    dword ptr [ecx + 8], 0
        cmp    byte ptr [eax + 18h], 0
        jne    SHORT loc_1_BB52
        add    eax, 10h
        push   eax
        call   sub_1_BD24
        add    esp, 4
loc_1_BB52:
        leave
        ret
sub_1_BB54:
        push   esi
        push   edi
        db  0BFh, 028h, 034h, 001h, 000h
        db  08Bh, 035h, 028h, 034h, 001h, 000h
        or     esi, esi
        je     SHORT loc_1_BB99
loc_1_BB65:
        cmp    word ptr [esi + 2ah], 0
        jne    SHORT loc_1_BB90
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_1_BB80
        push   eax
        call   sub_1_727F
        add    esp, 4
loc_1_BB80:
        db  0A1h, 024h, 034h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        db  089h, 035h, 024h, 034h, 001h, 000h
        jmp    SHORT loc_1_BB93
loc_1_BB90:
        lea    edi, [esi + 4]
loc_1_BB93:
        mov    esi, dword ptr [edi]
        or     esi, esi
        jne    SHORT loc_1_BB65
loc_1_BB99:
        pop    edi
        pop    esi
        ret
sub_1_BB9C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        call   sub_1_BB54
        db  08Bh, 035h, 024h, 034h, 001h, 000h
        or     esi, esi
        jne    SHORT loc_1_BBF0
        db  081h, 03Dh, 030h, 034h, 001h, 000h, 0FFh, 000h, 000h, 000h
        jl     SHORT loc_1_BBC4
        sub    eax, eax
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
loc_1_BBC4:
        push   5ch
        call   sub_1_8CB6
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_BBDC
        sub    eax, eax
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
loc_1_BBDC:
        db  0FFh, 005h, 030h, 034h, 001h, 000h
        db  066h, 0A1h, 030h, 034h, 001h, 000h
        mov    word ptr [esi + 10h], ax
        jmp    SHORT loc_1_BBF8
        db  08Bh, 0FFh
loc_1_BBF0:
        mov    eax, dword ptr [esi + 4]
        db  0A3h, 024h, 034h, 001h, 000h
loc_1_BBF8:
        call   sub_1_73C7
        mov    ecx, eax
        or     ecx, ecx
        jne    SHORT loc_1_BC18
        db  0A1h, 024h, 034h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        db  089h, 035h, 024h, 034h, 001h, 000h
        sub    eax, eax
        pop    edi
        pop    esi
        leave
        ret
        db  090h
loc_1_BC18:
        mov    ax, word ptr [esi + 10h]
        add    ax, 100h
        mov    word ptr [esi + 10h], ax
        je     SHORT loc_1_BC18
        lea    edi, [esi + 14h]
        sub    eax, eax
        stosd
        mov    dword ptr [esi], ecx
        mov    dword ptr [ecx + 14h], esi
        cli
        db  0A1h, 02Ch, 034h, 001h, 000h
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dl, byte ptr [eax]
        mov    byte ptr [ecx + 3fh], dl
        db  089h, 00Dh, 02Ch, 034h, 001h, 000h
        inc    byte ptr [eax + 1]
        inc    byte ptr [eax + 7]
        sti
        mov    eax, 1
        pop    edi
        pop    esi
        leave
        ret
sub_1_BC58:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        lea    ebx, [edx + 3eh]
        movzx  eax, byte ptr [ebx]
        add    eax, eax
        add    eax, 3
        add    ebx, eax
        movzx  eax, word ptr [ebx - 2]
        add    ebx, eax
        lea    esi, [ebx + 10h]
        db  0BFh, 048h, 023h, 001h, 000h
        mov    ecx, 4
        repe cmpsd
        je     SHORT loc_1_BC94
        mov    word ptr [edx + 25h], 12h
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_BC94:
        cli
        db  08Bh, 00Dh, 02Ch, 034h, 001h, 000h
        or     ecx, ecx
        jne    SHORT loc_1_BCB0
        sti
        mov    word ptr [edx + 25h], 59h
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_BCB0:
        mov    eax, dword ptr [ecx + 18h]
        db  0A3h, 02Ch, 034h, 001h, 000h
        mov    eax, dword ptr [ecx + 14h]
        mov    si, word ptr [eax + 10h]
        mov    word ptr [edx + 30h], si
        mov    dword ptr [eax + 8], 0
        mov    word ptr [eax + 12h], 0
        mov    word ptr [eax + 18h], 0ffffh
        db  08Bh, 035h, 0C4h, 031h, 001h, 000h
        mov    dword ptr [eax + 0ch], esi
        lea    esi, [edx + 12h]
        lea    edi, [eax + 20h]
        movsd
        movsd
        movsd
        mov    dl, byte ptr [eax + 10h]
        mov    byte ptr [ecx + 40h], dl
        lea    edi, [ecx + 4]
        mov    esi, ebx
        movsd
        movsd
        movsd
        movsw
        movsb
        mov    byte ptr [ecx + 13h], 0
        db  08Bh, 015h, 028h, 028h, 001h, 000h
        mov    dword ptr [ecx + 30h], edx
        mov    dword ptr [ecx + 34h], edx
        mov    byte ptr [ecx + 1eh], 2
        db  08Bh, 00Dh, 028h, 034h, 001h, 000h
        mov    dword ptr [eax + 4], ecx
        db  0A3h, 028h, 034h, 001h, 000h
        sti
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_BD24:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 30h]
        or     eax, eax
        je     SHORT loc_1_BD3B
        mov    eax, dword ptr [eax + 8]
        sub    eax, ecx
        cmp    eax, -10h
        je     SHORT loc_1_BD5A
loc_1_BD3B:
        mov    ax, word ptr [ecx + 28h]
        sub    ax, 1eh
        mov    word ptr [ecx], ax
        mov    byte ptr [ecx - 7], 0
        mov    byte ptr [ecx + 29h], 0
        sub    ecx, 10h
        push   ecx
        call   sub_1_7A48
        add    esp, 4
loc_1_BD5A:
        leave
        ret
loc_1_BD5C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 040h, 004h, 020h, 0C1h, 000h, 000h
        mov    cx, word ptr [eax + 2eh]
        mov    word ptr [eax + 28h], cx
        push   eax
        push   4
        call   sub_1_78F0
        leave
        ret
        db  090h
sub_1_BD7C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 9]
        or     al, al
        je     SHORT loc_1_BDA8
        cmp    al, 0fdh
        jne    NEAR PTR loc_1_C11B
        push   ecx
        push   4
        call   sub_1_78F0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_BDA8:
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 24h]
        mov    dword ptr [ebp - 4], ecx
        mov    ecx, dword ptr [ebp + 8]
        movsx  eax, byte ptr [ecx - 9]
        db  08Bh, 034h, 085h, 0B0h, 01Bh, 001h, 000h
        mov    dword ptr [ebp - 8], esi
        mov    ecx, dword ptr [ebp - 4]
        movzx  edx, word ptr [ecx + 30h]
        or     edx, edx
        jne    NEAR PTR loc_1_BEB0
        cmp    byte ptr [ecx + 22h], 72h
        jne    NEAR PTR loc_1_BEB0
        db  08Bh, 01Dh, 028h, 034h, 001h, 000h
        or     ebx, ebx
        je     NEAR PTR loc_1_BE7C
        mov    eax, ecx
        lea    eax, [eax + 12h]
loc_1_BDEE:
        mov    edi, eax
        lea    esi, [ebx + 20h]
        mov    ecx, 3
        repe cmpsd
        je     SHORT loc_1_BE08
        mov    ebx, dword ptr [ebx + 4]
        or     ebx, ebx
        jne    SHORT loc_1_BDEE
        jmp    SHORT loc_1_BE7C
        db  08Dh, 049h, 000h
loc_1_BE08:
        mov    edi, dword ptr [ebp - 4]
        add    edi, 3eh
        movsx  eax, byte ptr [edi]
        add    eax, eax
        add    eax, 3
        add    edi, eax
        movzx  eax, word ptr [edi - 2]
        add    edi, eax
        cmp    word ptr [ebx + 12h], 0
        jne    SHORT loc_1_BE70
        mov    esi, dword ptr [ebx]
        mov    ecx, 4
        lea    esi, [esi + 4]
        repe cmpsd
        jne    SHORT loc_1_BE70
        mov    ax, word ptr [ebx + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 30h], ax
loc_1_BE3E:
        mov    si, word ptr [ebx + 18h]
        cmp    si, -1
        je     NEAR PTR loc_1_C0B0
        or     si, si
        jne    NEAR PTR loc_1_C058
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        jne    NEAR PTR loc_1_C030
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 25h], 1
        jmp    NEAR PTR loc_1_C0C5
        db  08Bh, 0FFh
loc_1_BE70:
        mov    eax, dword ptr [ebx]
        mov    byte ptr [eax + 1eh], 5
        mov    word ptr [ebx + 2ah], 0
loc_1_BE7C:
        push   dword ptr [ebp - 4]
        call   sub_1_BC58
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_C0C5
        mov    esi, dword ptr [ebp - 8]
        dec    byte ptr [esi + 1]
        mov    eax, dword ptr [ebx]
        mov    cx, word ptr [ebx + 10h]
        and    ecx, 0ffh
        mov    dword ptr [esi + ecx*4 + 0f2h], eax
        jmp    NEAR PTR loc_1_BFDF
        db  090h
loc_1_BEB0:
        mov    eax, edx
        and    eax, 0ffh
        mov    esi, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + eax*4 + 0f2h]
        or     eax, eax
        je     NEAR PTR loc_1_C0BC
        mov    ebx, dword ptr [eax + 14h]
        movzx  eax, word ptr [ebx + 10h]
        cmp    eax, edx
        jne    NEAR PTR loc_1_C0BC
        cmp    word ptr [ebx + 2ah], 0
        je     NEAR PTR loc_1_C0BC
        db  0A1h, 0C4h, 031h, 001h, 000h
        mov    dword ptr [ebx + 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    si, word ptr [ecx + 32h]
        mov    ax, word ptr [ebx + 12h]
        cmp    si, ax
        je     NEAR PTR loc_1_BE3E
        cli
        or     si, si
        je     NEAR PTR loc_1_BF90
        or     ax, ax
        je     SHORT loc_1_BF2C
loc_1_BF0E:
        inc    ax
        je     SHORT loc_1_BF0E
        cmp    ax, si
        je     SHORT loc_1_BF2C
        push   dword ptr [ebp + 8]
        push   4
        call   sub_1_78F0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_BF2C:
        mov    eax, dword ptr [ebx + 8]
        mov    word ptr [ebx + 12h], si
        mov    word ptr [ebx + 18h], 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_BFDF
        mov    edx, dword ptr [eax + 0ch]
        mov    di, word ptr [edx + 1eh]
        mov    ecx, dword ptr [ebp - 4]
        cmp    word ptr [ecx + 3ch], di
        jne    SHORT loc_1_BF5C
        mov    word ptr [edx + 14h], si
        mov    dword ptr [ebp - 4], 0
loc_1_BF5C:
        mov    dword ptr [ebx + 8], 0
        cmp    byte ptr [eax + 18h], 0
        jne    SHORT loc_1_BF75
        add    eax, 10h
        push   eax
        call   sub_1_BD24
        add    esp, 4
loc_1_BF75:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_BFDF
        push   dword ptr [ebp + 8]
        push   4
        call   sub_1_78F0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_BF90:
        mov    ecx, dword ptr [ebp - 4]
        cmp    byte ptr [ecx + 22h], 1eh
        je     SHORT loc_1_BFDF
        mov    edx, 0ffffffffh
        sub    esi, esi
        mov    cx, word ptr [ecx + 3ch]
loc_1_BFA4:
        mov    ax, word ptr [ebx + esi*2 + 14h]
        cmp    ax, cx
        je     NEAR PTR loc_1_C0B0
        or     ax, ax
        jne    SHORT loc_1_BFB9
        mov    edx, esi
loc_1_BFB9:
        inc    esi
        cmp    esi, 2
        jl     SHORT loc_1_BFA4
        cmp    edx, -1
        jne    SHORT loc_1_BFD4
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 25h], 11h
        jmp    NEAR PTR loc_1_C0C5
        db  08Bh, 0FFh
loc_1_BFD4:
        mov    word ptr [ebx + edx*2 + 14h], cx
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx - 5], dl
loc_1_BFDF:
        mov    esi, dword ptr [ebp - 4]
        add    esi, 12h
        mov    edi, dword ptr [ebp - 4]
        add    edi, 6
        movsd
        movsd
        movsd
        mov    ecx, dword ptr [ebp - 4]
        movzx  ax, byte ptr [ecx + 3]
        movzx  dx, byte ptr [ecx + 2]
        shl    dx, 8
        add    dx, ax
        sub    dx, 1eh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx], dx
        mov    al, byte ptr [ebx + 10h]
        mov    byte ptr [ecx - 6], al
        mov    byte ptr [ecx - 7], 0
        mov    byte ptr [ecx + 29h], 0
        mov    eax, ecx
        sub    eax, 10h
        push   eax
        call   sub_1_7A48
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_C030:
        cmp    byte ptr [eax + 18h], 0
        jne    SHORT loc_1_C044
        add    eax, 10h
        push   eax
        push   3
        call   sub_1_78F0
        add    esp, 8
loc_1_C044:
        push   dword ptr [ebp + 8]
        push   4
        call   sub_1_78F0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_C058:
        mov    al, byte ptr [ebx + 1ah]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 23h], al
        mov    al, byte ptr [ebx + 1bh]
        mov    byte ptr [ecx + 24h], al
        mov    ax, word ptr [ebx + 1ch]
        mov    word ptr [ecx + 25h], ax
        mov    al, byte ptr [ecx + 22h]
        cmp    al, 70h
        je     SHORT loc_1_C07E
        cmp    al, 75h
        je     SHORT loc_1_C07E
        cmp    al, 73h
        jne    SHORT loc_1_C089
loc_1_C07E:
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ebx + 1eh]
        mov    word ptr [ecx + 36h], ax
loc_1_C089:
        movsx  eax, si
        mov    edx, dword ptr [ebp - 4]
        add    edx, 3eh
        lea    esi, [ebx + 2ch]
        mov    edi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movsx  eax, word ptr [ebx + 18h]
        add    edx, eax
        jmp    SHORT loc_1_C0E1
        db  08Dh, 049h, 000h
loc_1_C0B0:
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 25h], 11h
        jmp    SHORT loc_1_C0C5
        db  090h
loc_1_C0BC:
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 25h], 10h
loc_1_C0C5:
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 23h], 2
        mov    byte ptr [ecx + 24h], 0
        mov    edx, ecx
        add    edx, 3eh
        mov    byte ptr [edx], 0
        inc    edx
        mov    word ptr [edx], 0
        add    edx, 2
loc_1_C0E1:
        mov    esi, dword ptr [ebp - 4]
        add    esi, 12h
        mov    edi, dword ptr [ebp - 4]
        add    edi, 6
        movsd
        movsd
        movsd
        mov    ecx, dword ptr [ebp - 4]
        or     byte ptr [ecx + 27h], 80h
        mov    ecx, dword ptr [ebp + 8]
        db  0C7h, 041h, 004h, 05Ch, 0BDh, 000h, 000h
        sub    edx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + 28h]
        mov    word ptr [ecx + 2eh], ax
        mov    word ptr [ecx + 28h], dx
        push   ecx
        push   3
        call   sub_1_78F0
        add    esp, 8
loc_1_C11B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_C120:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_1_BD7C
        add    esp, 4
        sti
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_C134:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 1ah], 5005h
        mov    word ptr [edx + 32h], 1
        mov    ax, word ptr [edx + 10h]
        add    ax, 1eh
        mov    word ptr [edx + 38h], ax
        movzx  eax, byte ptr [edx + 8]
        cmp    eax, 94h
        je     SHORT loc_1_C1B0
        cmp    eax, 96h
        je     SHORT loc_1_C180
        cmp    eax, 97h
        je     SHORT loc_1_C19C
        cmp    eax, 0a0h
        je     SHORT loc_1_C1D4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_C180:
        lea    eax, [edx + 10h]
        db  0C7h, 042h, 014h, 020h, 0C1h, 000h, 000h
        push   eax
        push   4
        call   sub_1_78F0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_C19C:
        mov    eax, dword ptr [edx + 14h]
        mov    dword ptr [edx + 3ah], eax
        mov    ax, word ptr [edx + 12h]
        mov    word ptr [edx + 3eh], ax
        mov    word ptr [edx + 32h], 2
loc_1_C1B0:
        mov    ebx, dword ptr [edx + 34h]
        movsx  eax, byte ptr [edx + 7]
        db  08Bh, 004h, 085h, 0B0h, 01Bh, 001h, 000h
        movzx  ecx, byte ptr [edx + 0ah]
        mov    eax, dword ptr [eax + ecx*4 + 0f2h]
        or     eax, eax
        jne    SHORT loc_1_C200
        sub    eax, eax
        jmp    SHORT loc_1_C203
        db  08Dh, 049h, 000h
loc_1_C1D4:
        mov    eax, dword ptr [edx + 34h]
        db  0C7h, 042h, 014h, 024h, 0BDh, 000h, 000h
        mov    ax, word ptr [eax + 10h]
        mov    word ptr [edx + 1ah], ax
        mov    dword ptr [edx + 40h], 0
        lea    eax, [edx + 10h]
        push   eax
        push   3
        call   sub_1_78F0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_C200:
        mov    eax, dword ptr [eax + 14h]
loc_1_C203:
        or     eax, eax
        mov    dword ptr [edx + 40h], eax
        je     NEAR PTR loc_1_C308
        mov    cx, word ptr [eax + 10h]
        cmp    word ptr [ebx + 30h], cx
        jne    NEAR PTR loc_1_C308
        cmp    word ptr [eax + 2ah], 0
        je     NEAR PTR loc_1_C308
        cmp    word ptr [ebx + 32h], 0
        je     NEAR PTR loc_1_C2D8
        movzx  ecx, word ptr [edx + 10h]
        sub    ecx, 20h
        mov    dword ptr [ebp - 4], ecx
        cmp    ecx, 30h
        ja     SHORT loc_1_C248
        cmp    word ptr [edx + 32h], 1
        je     SHORT loc_1_C287
loc_1_C248:
        mov    cl, byte ptr [ebx + 22h]
        cmp    cl, 25h
        je     SHORT loc_1_C2C8
        cmp    cl, 26h
        je     SHORT loc_1_C2C8
        cmp    cl, 32h
        je     SHORT loc_1_C2C8
        cmp    cl, 33h
        je     SHORT loc_1_C2C8
        mov    word ptr [ebx + 25h], 1
        mov    byte ptr [ebx + 23h], 2
        mov    byte ptr [ebx + 24h], 0
        mov    dword ptr [ebx + 3eh], 0
        mov    dword ptr [ebp - 4], 3
        mov    word ptr [edx + 32h], 1
        mov    word ptr [edx + 38h], 41h
loc_1_C287:
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], ecx
        lea    esi, [ebx + 3eh]
        lea    edi, [eax + 2ch]
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 3
        rep movsb
        mov    cl, byte ptr [ebx + 23h]
        mov    byte ptr [eax + 1ah], cl
        mov    cl, byte ptr [ebx + 24h]
        mov    byte ptr [eax + 1bh], cl
        mov    cx, word ptr [ebx + 25h]
        mov    word ptr [eax + 1ch], cx
        mov    cx, word ptr [ebx + 36h]
        mov    word ptr [eax + 1eh], cx
        mov    cx, word ptr [ebp - 4]
        mov    word ptr [eax + 18h], cx
        jmp    SHORT loc_1_C2ED
        db  08Bh, 0FFh
loc_1_C2C8:
        mov    byte ptr [edx + 18h], 0ffh
        mov    dword ptr [eax + 8], edx
        mov    word ptr [eax + 18h], 0
        jmp    SHORT loc_1_C2ED
        db  090h
loc_1_C2D8:
        mov    cl, byte ptr [edx + 0bh]
        test   cl, 80h
        jne    SHORT loc_1_C2ED
        and    ecx, 0ffh
        mov    word ptr [eax + ecx*2 + 14h], 0
loc_1_C2ED:
        lea    eax, [edx + 10h]
        db  0C7h, 042h, 014h, 024h, 0BDh, 000h, 000h
        push   eax
        push   3
        call   sub_1_78F0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_C308:
        push   edx
        mov    byte ptr [edx + 39h], 0ah
        mov    byte ptr [edx + 9], 0ah
        call   sub_1_7A48
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_C320:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 040h, 004h, 038h, 0C3h, 000h, 000h
        push   eax
        push   4
        call   sub_1_78F0
        leave
        ret
        db  090h
loc_1_C338:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 9], 0
        je     SHORT loc_1_C358
        push   eax
        push   4
        call   sub_1_78F0
        add    esp, 8
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_C358:
        mov    edx, dword ptr [eax + 24h]
        lea    esi, [edx + 42h]
        db  0BFh, 048h, 023h, 001h, 000h
        mov    ecx, 4
        repe cmpsd
        je     SHORT loc_1_C37C
        push   eax
        push   4
        call   sub_1_78F0
        add    esp, 8
        pop    edi
        pop    esi
        leave
        ret
        db  090h
loc_1_C37C:
        lea    esi, [edx + 12h]
        mov    byte ptr [edx + 5], 4
        mov    byte ptr [edx + 3eh], 0f4h
        lea    edi, [edx + 6]
        movsd
        movsd
        movsd
        db  0C7h, 040h, 004h, 020h, 0C3h, 000h, 000h
        push   eax
        push   3
        call   sub_1_78F0
        add    esp, 8
        pop    edi
        pop    esi
        leave
        ret
        db  090h
sub_1_C3A4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, 1
        push   5005h
        push   0ffh
        call   sub_1_7937
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_C3CC
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_C3CC:
        push   5105h
        push   0ffh
        call   sub_1_7937
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_C443
        push   1c2h
        call   sub_1_8CB6
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_C3FC
        mov    esi, 2
        jmp    SHORT loc_1_C42D
loc_1_C3FC:
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 14h], edi
        lea    ebx, [edi + 126h]
        mov    esi, ebx
        push   ebx
        call   sub_1_C49C
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_C458
        cmp    eax, 0deadbeefh
        jne    SHORT loc_1_C428
        mov    esi, 2
        jmp    SHORT loc_1_C436
        db  08Bh, 0FFh
loc_1_C428:
        mov    esi, 3
loc_1_C42D:
        push   edi
        call   sub_1_8CF5
        add    esp, 4
loc_1_C436:
        push   5105h
        call   sub_1_794F
        add    esp, 4
loc_1_C443:
        push   5005h
        call   sub_1_794F
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_C458:
        db  0C7h, 046h, 004h, 038h, 0C3h, 000h, 000h
        mov    word ptr [esi + 0ah], 5105h
        mov    word ptr [esi + 22h], 1
        mov    dword ptr [esi + 24h], edi
        mov    word ptr [esi + 28h], 62h
        push   esi
        push   4
        call   sub_1_78F0
        add    esp, 8
        add    esi, 34h
        add    edi, 62h
        cmp    ebx, edi
        mov    eax, 0
        seta   al
        or     eax, eax
        jne    SHORT loc_1_C458
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
sub_1_C49C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 34h
        mov    word ptr [ebx + 0ah], 5005h
        mov    word ptr [ebx + 22h], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + 24h], eax
        mov    word ptr [ebx + 28h], 62h
        push   ebx
        push   4
        call   sub_1_78F0
        add    esp, 8
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 62h
        mov    byte ptr [ecx + 4], 0
        mov    byte ptr [ecx + 5], 14h
        mov    dword ptr [ecx + 6], 0
        lea    edi, [ecx + 0ah]
        mov    eax, 0ffffffffh
        stosd
        stosw
        mov    word ptr [ecx + 10h], 5105h
        mov    word ptr [ecx + 40h], 0
        mov    byte ptr [ecx + 3eh], 0f1h
        mov    byte ptr [ecx + 3fh], 1
        db  0BEh, 048h, 023h, 001h, 000h
        lea    edi, [ecx + 42h]
        movsd
        movsd
        movsd
        movsd
        db  0BEh, 048h, 023h, 001h, 000h
        lea    edi, [ecx + 52h]
        movsd
        movsd
        movsd
        movsd
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0ah], 5005h
        mov    edi, eax
        add    edi, 1ch
        mov    eax, 0ffffffffh
        stosd
        stosw
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 22h], 1
        mov    dword ptr [eax + 24h], ecx
        mov    word ptr [eax + 28h], 62h
        sub    esi, esi
loc_1_C541:
        push   dword ptr [ebp + 8]
        push   3
        call   sub_1_78F0
        add    esp, 8
loc_1_C54E:
        call   sub_1_791E
        mov    edi, eax
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 8], 0
        jne    SHORT loc_1_C54E
loc_1_C55E:
        cmp    byte ptr [ebx + 8], 0
        je     SHORT loc_1_C594
        call   sub_1_791E
        sub    eax, edi
        cmp    eax, 1f4h
        jb     SHORT loc_1_C55E
        inc    esi
        cmp    esi, 5
        jl     SHORT loc_1_C541
        push   ebx
        push   6
        call   sub_1_78F0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_C5A8
        mov    eax, 0deadbeefh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_C594:
        push   34h
        call   sub_1_7910
        add    esp, 4
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_C5A8:
        cmp    byte ptr [ebx + 8], 0
        je     SHORT loc_1_C5B9
loc_1_C5AE:
        call   sub_1_791E
        cmp    byte ptr [ebx + 8], 0
        jne    SHORT loc_1_C5AE
loc_1_C5B9:
        sub    eax, eax
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 1ah
        rep stosd
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_C5CC:
        mov    eax, 400002h
        stc
        ret
        db  090h
loc_1_C5D4:
        mov    eax, 40002h
        mov    eax, eax
loc_1_C5DC:
        mov    ecx, eax
        mov    edx, dword ptr [esp]
        mov    edi, dword ptr [edx + 30h]
        sub    eax, eax
        stosw
        test   word ptr [esp + 10h], 1
        je     SHORT loc_1_C5F5
        stosd
        stosd
        stosd
        stosd
loc_1_C5F5:
        stosw
        stosd
        mov    eax, dword ptr [esp + 18h]
        stosw
        test   word ptr [esp + 10h], 8
        je     SHORT loc_1_C608
        stosd
loc_1_C608:
        sub    edi, dword ptr [edx + 30h]
        mov    word ptr [edx + 2ch], di
        add    esp, 1ch
        mov    eax, ecx
        stc
        ret
loc_1_C616:
        mov    edx, eax
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        call   sub_1_1AB4
        mov    eax, edx
        jmp    SHORT loc_1_C5DC
        db  08Bh, 0C0h
loc_1_C62C:
        cmp    word ptr [eax + 14h], 1dh
        jb     SHORT loc_1_C5CC
        sub    esp, 1ch
        mov    dword ptr [esp], eax
        mov    dword ptr [esp + 18h], 0
        mov    word ptr [eax + 2eh], 0
        mov    eax, dword ptr [edx + 18h]
        mov    dword ptr [esp + 4], eax
        call   sub_1_196C
        jb     SHORT loc_1_C5DC
        mov    ebx, dword ptr [eax + 0ch]
        mov    edx, dword ptr [esp]
        mov    edx, dword ptr [edx + 1ch]
        mov    cx, word ptr [edx]
        mov    word ptr [esp + 10h], cx
        mov    ebp, dword ptr [edx]
        mov    bp, word ptr [edx + 0ch]
        test   ebp, ebp
        js     NEAR PTR loc_1_C5D4
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_C6F0
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_C6F0
        shld   eax, ebp, 10h
        cmp    ax, 0ffh
        je     SHORT loc_1_C6B7
        and    eax, 3
        db  08Ah, 080h, 044h, 01Ah, 001h, 000h
        test   ebp, 10h
        je     SHORT loc_1_C6A0
        or     al, 4
loc_1_C6A0:
        mov    ah, byte ptr [ebx + 10h]
        and    ah, al
        cmp    al, ah
        je     SHORT loc_1_C6F0
        test   al, 4
        je     NEAR PTR loc_1_C5D4
        and    bp, 0fff0h
        jmp    SHORT loc_1_C6F0
loc_1_C6B7:
        mov    al, 3
        test   ebp, 10h
        je     SHORT loc_1_C6C3
        or     al, 4
loc_1_C6C3:
        mov    ah, byte ptr [ebx + 10h]
        and    al, ah
        cmp    al, ah
        je     SHORT loc_1_C6F0
        test   al, 4
        je     SHORT loc_1_C6D6
        and    bp, 0fff0h
        jmp    SHORT loc_1_C6F0
loc_1_C6D6:
        test   al, 1
        je     NEAR PTR loc_1_C5D4
        and    bp, 0f0h
        je     NEAR PTR loc_1_C5D4
        and    ebp, 0ffffh
        nop
loc_1_C6F0:
        mov    eax, 10048h
        mov    edx, dword ptr [esp + 4]
        push   edx
        call   sub_1_19E4
        jb     NEAR PTR loc_1_C5DC
        mov    edx, dword ptr [esp]
        mov    edx, dword ptr [edx + 1ch]
        mov    ax, word ptr [edi + 1ch]
        shl    eax, 10h
        mov    ax, word ptr [esp + 6]
        mov    dword ptr [esp + 4], eax
        mov    edi, dword ptr [esp]
        movzx  eax, byte ptr [ebx + 0ah]
        add    ax, word ptr [edi + 14h]
        sub    ax, 1ch
        lea    esi, [edx + 1ch]
        call   sub_1_1CA4
        jb     NEAR PTR loc_1_C616
        xchg   ebp, eax
        mov    dword ptr [esp + 8], ebp
        mov    ebx, dword ptr [esp]
        mov    edi, dword ptr [ebx + 30h]
        add    edi, 2
        mov    dword ptr [edi + 8], ecx
        mov    ebx, dword ptr [ebx + 20h]
        mov    dword ptr [edi], ebx
        mov    ebx, dword ptr [esp + 4]
        mov    dword ptr [edi + 4], ebx
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ebx, dword ptr [esi + 2ch]
        mov    ecx, dword ptr [ebx + 8]
        db  08Ah, 099h, 01Ch, 04Ah, 001h, 000h
        shl    ecx, 10h
        mov    cl, bl
        push   ecx
        mov    ebx, dword ptr [edx + 4]
        mov    bh, byte ptr [edx]
        shr    bh, 1
        db  022h, 03Dh, 0DDh, 01Ch, 001h, 000h
        mov    ecx, dword ptr [esi + 20h]
        bt     word ptr [ecx + 0ah], 1
        jae    SHORT loc_1_C785
loc_1_C785:
        mov    ecx, dword ptr [edx + 8]
        test   ecx, ecx
        jne    SHORT loc_1_C792
        db  08Bh, 00Dh, 028h, 028h, 001h, 000h
loc_1_C792:
        mov    edx, dword ptr [edx + 0eh]
        mov    esi, dword ptr [esi + 40h]
        call   sub_1_94D4
        pop    esi
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    byte ptr [esi + 9], 0
        mov    ecx, dword ptr [esi + 2ch]
        jb     NEAR PTR loc_1_C85F
        mov    dword ptr [ecx + 22h], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 26h], eax
        mov    ebx, dword ptr [esi + 1ch]
        mov    al, byte ptr [ebx + 0ah]
        mov    byte ptr [ecx + 34h], al
        call   sub_1_1ADC
        jb     NEAR PTR loc_1_C5DC
        mov    edi, dword ptr [esp]
        mov    esi, dword ptr [edi + 1ch]
        mov    edi, dword ptr [edi + 30h]
        mov    ax, word ptr [edi + 2]
        mov    word ptr [esp + 12h], ax
        mov    bx, word ptr [esp + 10h]
        mov    eax, dword ptr [ecx + 8]
        stosw
        test   bl, 1
        je     SHORT loc_1_C7FA
        lea    edi, [edi + 10h]
        mov    dword ptr [edi - 4], 0
loc_1_C7FA:
        mov    eax, edx
        stosw
        sub    eax, eax
        stosd
        stosw
        test   word ptr [esp + 10h], 8
        je     SHORT loc_1_C84F
        push   edi
        push   ecx
        push   edx
        push   1ah
        call   sub_1_8CB6
        pop    edx
        pop    edx
        pop    ecx
        test   eax, eax
        je     SHORT loc_1_C848
        mov    edi, eax
        mov    edx, 2
        db  0A1h, 014h, 028h, 001h, 000h
        mov    eax, dword ptr [eax + 2ch]
        mov    eax, dword ptr [eax + 22h]
        push   edi
        call   sub_1_9850
        pop    edi
        pushfd
        mov    eax, dword ptr [edi + 16h]
        push   ecx
        push   edx
        push   edi
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        popfd
        jae    SHORT loc_1_C84D
loc_1_C848:
        mov    eax, 0ffffffffh
loc_1_C84D:
        pop    edi
        stosd
loc_1_C84F:
        mov    eax, dword ptr [esp]
        sub    edi, dword ptr [eax + 30h]
        mov    word ptr [eax + 2ch], di
        add    esp, 1ch
        sub    eax, eax
        ret
loc_1_C85F:
        mov    dword ptr [esp + 18h], edx
        sub    ah, ah
        shl    eax, 10h
        mov    al, 1
        xchg   dword ptr [esp + 8], eax
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        cmp    dword ptr [ecx + 1ah], 0
        je     SHORT loc_1_C88F
        call   sub_1_9449
loc_1_C88F:
        mov    eax, dword ptr [esp + 8]
        jmp    NEAR PTR loc_1_C616
        db  0BDh, 012h, 000h, 001h, 000h, 0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h, 00Fh, 085h, 046h, 0FEh
        db  0FFh, 0FFh, 0F6h, 047h, 01Fh, 001h, 00Fh, 085h, 03Ch, 0FEh, 0FFh, 0FFh, 0F6h, 043h, 010h, 004h
        db  00Fh, 084h, 016h, 0FDh, 0FFh, 0FFh, 0B8h, 048h, 000h, 001h, 000h, 08Bh, 054h, 024h, 004h, 052h
        db  0E8h, 017h, 051h, 0FFh, 0FFh, 00Fh, 082h, 009h, 0FDh, 0FFh, 0FFh, 081h, 0E5h, 0FFh, 0FFh, 08Fh
        db  0FFh, 081h, 0CDh, 000h, 000h, 020h, 000h, 0E8h, 037h, 0CAh, 0FFh, 0FFh, 08Bh, 014h, 024h, 08Bh
        db  052h, 01Ch, 00Fh, 083h, 048h, 0FEh, 0FFh, 0FFh, 089h, 044h, 024h, 008h, 0EBh, 099h, 08Bh, 0C0h
sub_1_C8F8:
        mov    ecx, ebx
        mov    edx, dword ptr [ecx + 1ch]
        call   sub_1_1B8C
        mov    eax, dword ptr [ecx + 48h]
        test   eax, eax
        je     SHORT loc_1_C914
        push   ecx
        push   edx
        push   eax
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
loc_1_C914:
        call   sub_1_1AB4
        mov    eax, edx
        sub    edi, edi
        call   sub_1_CB7A
        sub    eax, eax
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_C928:
        cmp    word ptr [eax + 14h], 0dh
        jb     NEAR PTR loc_1_C5CC
        mov    word ptr [eax + 2ch], 0ah
        push   eax
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_CB44
        mov    ebx, dword ptr [eax + 0ch]
        push   dword ptr [edx + 18h]
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_C968
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_C968
        test   byte ptr [ebx + 10h], 80h
        je     NEAR PTR loc_1_CB3A
loc_1_C968:
        mov    eax, 2004ch
        call   sub_1_19E4
        jb     NEAR PTR loc_1_CB44
        mov    word ptr [ecx + 1ah], 0
        mov    byte ptr [ecx + 2ch], 0
        mov    edx, dword ptr [esp]
        mov    esi, dword ptr [edx + 1ch]
        movzx  eax, word ptr [esi + 6]
        cmp    eax, 1
        je     SHORT loc_1_C9AC
        cmp    eax, 2
        je     SHORT loc_1_C9AC
        cmp    eax, 101h
        jb     NEAR PTR loc_1_CAE2
        cmp    eax, 104h
        ja     NEAR PTR loc_1_CAE2
loc_1_C9AC:
        mov    word ptr [ecx + 28h], ax
        mov    ax, word ptr [esi + 4]
        mov    word ptr [ecx + 2ah], ax
        db  0A1h, 014h, 028h, 001h, 000h
        mov    eax, dword ptr [eax + 20h]
        mov    ax, word ptr [eax + 0ah]
        and    ah, 80h
        mov    byte ptr [ecx + 2ch], ah
        and    eax, 1
        shl    eax, 6
        or     ax, word ptr [esi]
        mov    dword ptr [ecx + 20h], eax
        lea    esi, [esi + 0ch]
        cmp    byte ptr [esi], 0
        je     NEAR PTR loc_1_CADB
        movzx  eax, byte ptr [ebx + 0ah]
        add    ax, word ptr [edx + 14h]
        sub    eax, 0ch
        call   sub_1_1CA4
        jb     NEAR PTR loc_1_CB08
        push   eax
        movzx  eax, word ptr [edx + 2eh]
        call   sub_1_7879
        jb     NEAR PTR loc_1_CAE9
        mov    dword ptr [edx + 34h], eax
        mov    dword ptr [edx + 3ch], ecx
        mov    edi, eax
        pop    ebp
        mov    ecx, dword ptr [edx + 1ch]
        movzx  ecx, word ptr [ecx + 2]
        movzx  edx, word ptr [edx + 2eh]
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ebx, dword ptr [esi + 30h]
        mov    bx, word ptr [ebx + 1ch]
        shl    ebx, 10h
        mov    eax, dword ptr [esi + 20h]
        mov    bx, word ptr [eax + 1ah]
        mov    eax, dword ptr [esi + 2ch]
        mov    esi, dword ptr [esi + 40h]
        push   ebp
        call   sub_1_CC48
        mov    edx, dword ptr [esp + 4]
        pushfd
        sub    esi, dword ptr [edx + 34h]
        mov    word ptr [edx + 2eh], si
        sub    ebp, dword ptr [edx + 34h]
        mov    edx, dword ptr [edx + 30h]
        mov    word ptr [edx + 8], bp
        popfd
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    edx, eax
        pop    eax
        pushfd
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        popfd
        jb     NEAR PTR loc_1_CB11
        mov    ebx, dword ptr [esi + 2ch]
        push   ecx
        mov    ecx, ebx
        call   sub_1_1ADC
        pop    ecx
        jb     NEAR PTR loc_1_CB44
        mov    edi, dword ptr [esp]
        mov    eax, dword ptr [edi + 1ch]
        test   word ptr [eax + 4], 1
        jne    NEAR PTR loc_1_CB53
        test   word ptr [eax + 4], 2
        jne    NEAR PTR loc_1_CB49
loc_1_CAAD:
        mov    eax, dword ptr [edi + 30h]
        mov    edx, dword ptr [ebx + 8]
        mov    word ptr [eax], dx
        db  08Ah, 092h, 01Ch, 04Ah, 001h, 000h
        shl    dx, 0dh
        or     word ptr [eax], dx
        mov    word ptr [eax + 2], cx
        mov    word ptr [eax + 4], 0
        mov    word ptr [eax + 6], 0
        mov    edi, dword ptr [edi + 34h]
        jmp    NEAR PTR loc_1_D0A8
loc_1_CADB:
        mov    eax, 70002h
        jmp    SHORT loc_1_CB08
loc_1_CAE2:
        mov    eax, 10002h
        jmp    SHORT loc_1_CB08
loc_1_CAE9:
        pop    eax
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        mov    eax, 80001h
        mov    eax, eax
loc_1_CB08:
        xchg   edx, eax
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        jmp    SHORT loc_1_CB2F
loc_1_CB11:
        cmp    dl, 2
        je     SHORT loc_1_CB1B
        cmp    dl, 12h
        jne    SHORT loc_1_CB28
loc_1_CB1B:
        mov    dl, 2
        mov    ecx, dword ptr [esi + 30h]
        cmp    byte ptr [ecx + 3eh], 8
        jae    SHORT loc_1_CB28
        mov    dl, 12h
loc_1_CB28:
        sub    dh, dh
        shl    edx, 10h
        mov    dl, 1
loc_1_CB2F:
        mov    ecx, dword ptr [esi + 2ch]
        call   sub_1_1AB4
        xchg   edx, eax
        jmp    SHORT loc_1_CB44
loc_1_CB3A:
        add    esp, 4
        mov    eax, 40002h
        mov    eax, eax
loc_1_CB44:
        add    esp, 4
        stc
        ret
loc_1_CB49:
        test   byte ptr [ebx + 2ch], 2
        je     NEAR PTR loc_1_CAAD
loc_1_CB53:
        pushad
        call   sub_1_C8F8
        popad
        mov    eax, dword ptr [edi + 30h]
        mov    word ptr [eax], 0
        mov    word ptr [eax + 2], cx
        mov    word ptr [eax + 4], 1
        mov    word ptr [eax + 6], 0
        mov    edi, dword ptr [edi + 34h]
        jmp    NEAR PTR loc_1_D0A8
sub_1_CB7A:
        mov    ecx, eax
        test   edi, edi
        jne    SHORT loc_1_CB96
        db  0A1h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [eax + 30h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    eax, dword ptr [eax + 20h]
        mov    di, word ptr [eax + 1ah]
loc_1_CB96:
        call   sub_1_9AE8
        jb     SHORT loc_1_CBAB
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax], 71a1h
        call   sub_1_9B3C
loc_1_CBAB:
        call   sub_1_9B74
        ret
sub_1_CBB1:
        cmp    ecx, -1
        jne    SHORT loc_1_CBC3
        push   edi
        push   eax
        mov    edi, esi
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        pop    eax
        pop    edi
loc_1_CBC3:
        jecxz  loc_1_CC01
        test   al, 80h
        je     SHORT loc_1_CBEC
        lea    eax, [ecx + ecx]
        cmp    edx, eax
        jb     SHORT loc_1_CC04
        push   edx
        push   1
        push   ecx
        push   esi
        push   edi
        db  0CDh, 020h, 042h, 000h, 040h, 000h
        add    esp, 10h
        pop    edx
        dec    edx
        cmp    edx, eax
        jbe    SHORT loc_1_CBEB
        mov    word ptr [eax + edi], 0
loc_1_CBEB:
        ret
loc_1_CBEC:
        cmp    edx, ecx
        jb     SHORT loc_1_CC04
        mov    edx, ecx
        mov    eax, ecx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        ret
loc_1_CC01:
        sub    eax, eax
        ret
loc_1_CC04:
        mov    eax, 7ah
        ret
sub_1_CC0A:
        cmp    ecx, -1
        jne    SHORT loc_1_CC20
        push   edi
        push   eax
        mov    edi, esi
        sub    eax, eax
        repne scasw
        not    ecx
        lea    ecx, [ecx + ecx - 2]
        pop    eax
        pop    edi
loc_1_CC20:
        jecxz  loc_1_CC01
        test   eax, 80h
        jne    SHORT loc_1_CBEC
        cmp    edx, ecx
        jb     SHORT loc_1_CC04
        push   edx
        push   1
        push   edx
        push   ecx
        push   esi
        push   edi
        db  0CDh, 020h, 040h, 000h, 040h, 000h
        add    esp, 14h
        pop    edx
        cmp    edx, eax
        jbe    SHORT loc_1_CC46
        mov    byte ptr [eax + edi], 0
loc_1_CC46:
        ret
        db  090h
sub_1_CC48:
        push   ebp
        push   ecx
        push   ecx
        movzx  ecx, byte ptr [edx + 2ch]
        shr    cl, 7
        not    ecx
        and    edx, ecx
        push   edx
        push   edi
        mov    esi, eax
        db  0A1h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [eax + 30h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    eax, dword ptr [eax + 20h]
        mov    di, word ptr [eax + 1ah]
        mov    dword ptr [esi + 24h], edi
        call   sub_1_9AE8
        jb     NEAR PTR loc_1_CDA0
        call   sub_1_9B0C
        jb     NEAR PTR loc_1_CDA0
        db  08Bh, 015h, 014h, 028h, 001h, 000h
        test   byte ptr [edx + 9], 4
        je     SHORT loc_1_CC9D
        and    byte ptr [edx + 9], 0fbh
        or     byte ptr [eax + 24h], 4
loc_1_CC9D:
        test   byte ptr [esi + 2ch], 1
        jne    SHORT loc_1_CD22
        and    byte ptr [esi + 2ch], 0fbh
        mov    dword ptr [eax + 8], 0feh
        lea    edi, [eax + 2ah]
        mov    dword ptr [eax + 0ch], edi
        mov    word ptr [eax + 28h], 80h
        push   esi
        call   sub_1_9BB8
        pop    esi
        jb     NEAR PTR loc_1_CDA0
        mov    dword ptr [eax], 714eh
        mov    dword ptr [eax + 10h], 0
        test   byte ptr [eax + 24h], 4
        je     SHORT loc_1_CD07
        push   eax
        push   esi
        push   edi
        lea    edi, [eax + 2ah]
        push   edi
        xor    eax, eax
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        dec    ecx
        mov    esi, edi
        dec    esi
        pop    edi
        mov    al, 5ch
        call   sub_1_E338
        jecxz  loc_1_CCFD
        inc    esi
        mov    edi, esi
loc_1_CCFD:
        mov    esi, edi
        call   sub_1_E4F4
        pop    edi
        pop    esi
        pop    eax
loc_1_CD07:
        call   sub_1_9B2C
        call   sub_1_9B3C
        jb     NEAR PTR loc_1_CDF7
        or     byte ptr [esi + 2ch], 1
        mov    ebx, dword ptr [eax]
        mov    dword ptr [esi + 1ch], ebx
        jmp    SHORT loc_1_CD56
loc_1_CD22:
        mov    dword ptr [eax], 714fh
        mov    dword ptr [eax + 10h], 0
        mov    ecx, dword ptr [esi + 1ch]
        mov    dword ptr [eax + 4], ecx
        call   sub_1_9B2C
        test   byte ptr [esi + 2ch], 4
        je     SHORT loc_1_CD4B
        and    byte ptr [esi + 2ch], 0fbh
        mov    ecx, dword ptr [esi + 48h]
        test   ecx, ecx
        jne    SHORT loc_1_CD59
loc_1_CD4B:
        call   sub_1_9B3C
        jb     NEAR PTR loc_1_CDF7
loc_1_CD56:
        mov    ecx, dword ptr [eax + 20h]
loc_1_CD59:
        mov    bl, byte ptr [esi + 20h]
        not    bl
        and    bl, byte ptr [ecx]
        and    bl, 1eh
        jne    SHORT loc_1_CD22
        mov    edx, esi
        mov    ebx, eax
        mov    ebp, dword ptr [ebp + 18h]
        mov    esi, ecx
        lea    edi, [esi + 2ch]
        mov    ecx, 104h
        sub    eax, eax
        repne scasw
        sub    ecx, 104h
        not    ecx
        add    ecx, ecx
        cmp    word ptr [edx + 28h], 2
        ja     SHORT loc_1_CD93
        call   sub_1_CE2C
        jmp    SHORT loc_1_CD98
loc_1_CD93:
        call   sub_1_CEF4
loc_1_CD98:
        jb     SHORT loc_1_CDB8
        dec    dword ptr [esp + 8]
        jne    SHORT loc_1_CD22
loc_1_CDA0:
        pop    esi
        pop    edx
        pop    edx
        pop    ecx
        pop    ebp
        cmp    ecx, edx
        stc
        je     SHORT loc_1_CDB2
        mov    dword ptr [eax], 0
        sub    ecx, edx
loc_1_CDB2:
        call   sub_1_9B74
        ret
loc_1_CDB8:
        mov    eax, dword ptr [edx + 48h]
        test   eax, eax
        jne    SHORT loc_1_CDDF
        push   ecx
        push   edx
        push   250h
        call   sub_1_8CB6
        pop    edx
        pop    edx
        pop    ecx
        mov    dword ptr [edx + 48h], eax
        test   eax, eax
        jne    SHORT loc_1_CDDF
        mov    eax, ebx
        mov    dword ptr [eax], 8
        jmp    SHORT loc_1_CDA0
loc_1_CDDF:
        mov    edi, eax
        mov    ecx, 94h
        cld
        rep movsd
        or     byte ptr [edx + 2ch], 4
        mov    eax, ebx
        mov    dword ptr [eax], 6fh
        jmp    SHORT loc_1_CDA0
loc_1_CDF7:
        cmp    dword ptr [eax], 2
        je     SHORT loc_1_CE01
        cmp    dword ptr [eax], 12h
        jne    SHORT loc_1_CDA0
loc_1_CE01:
        or     byte ptr [esi + 2ch], 2
        mov    ecx, dword ptr [esi + 48h]
        jecxz  loc_1_CDA0
        and    byte ptr [esi + 2ch], 0fbh
        push   eax
        push   ecx
        push   edx
        push   ecx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        xor    eax, eax
        mov    dword ptr [esi + 48h], 0
        pop    eax
        jmp    NEAR PTR loc_1_CDA0
        db  02Eh, 08Bh, 0C0h
sub_1_CE2C:
        movzx  eax, word ptr [edx + 2ah]
        and    eax, 4
        add    eax, 17h
        movzx  edi, byte ptr [edx + 2ch]
        shr    edi, 7
        add    eax, edi
        cmp    word ptr [edx + 28h], 1
        je     SHORT loc_1_CE49
        add    eax, 4
loc_1_CE49:
        inc    ecx
        inc    ecx
        add    eax, ecx
        sub    dword ptr [esp + 8], eax
        jb     NEAR PTR loc_1_CEEB
        mov    edi, dword ptr [esp + 4]
        add    dword ptr [esp + 4], eax
        test   word ptr [edx + 2ah], 4
        je     SHORT loc_1_CE6A
        mov    eax, ebp
        stosd
loc_1_CE6A:
        lodsd
        push   eax
        push   ecx
        push   edx
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        db  0CDh, 020h, 03Ah, 000h, 040h, 000h
        add    esp, 8
        ror    eax, 10h
        stosd
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        db  0CDh, 020h, 03Ah, 000h, 040h, 000h
        add    esp, 8
        ror    eax, 10h
        stosd
        push   dword ptr [esi + 14h]
        push   dword ptr [esi + 10h]
        db  0CDh, 020h, 03Ah, 000h, 040h, 000h
        add    esp, 8
        ror    eax, 10h
        stosd
        add    esi, 18h
        pop    edx
        pop    ecx
        lodsd
        lodsd
        stosd
        stosd
        pop    eax
        stosw
        cmp    word ptr [edx + 28h], 1
        je     SHORT loc_1_CEBC
        sub    eax, eax
        stosd
loc_1_CEBC:
        push   edi
        inc    edi
        lodsd
        lodsd
        mov    dword ptr [esp + 18h], edi
        movzx  eax, byte ptr [edx + 2ch]
        shr    eax, 7
        add    edi, eax
        mov    al, byte ptr [edx + 2ch]
        push   ecx
        push   edx
        mov    edx, ecx
        call   sub_1_CC0A
        pop    esi
        pop    edx
        pop    ecx
        jb     SHORT loc_1_CEEB
        sub    edx, eax
        dec    edx
        sub    dword ptr [esp + 4], edx
        add    dword ptr [esp + 8], edx
        mov    byte ptr [ecx], al
loc_1_CEEB:
        mov    eax, ebx
loc_1_CEED:
        ret
        db  040h, 044h, 00Ch, 05Eh, 08Bh, 0C0h
sub_1_CEF4:
        movzx  eax, word ptr [edx + 28h]
        and    eax, 7
        db  00Fh, 0B6h, 080h, 0EDh, 0CEh, 000h, 000h
        add    eax, ecx
        sub    dword ptr [esp + 8], eax
        jb     NEAR PTR loc_1_CFC4
        mov    edi, dword ptr [esp + 4]
        add    dword ptr [esp + 4], eax
        push   edi
        mov    eax, 0
        stosd
        mov    eax, ebp
        stosd
        mov    ebp, edi
        stosd
        cmp    word ptr [edx + 28h], 103h
        je     SHORT loc_1_CF91
        sub    edi, 4
        push   esi
        lea    esi, [esi + 4]
        movsd
        movsd
        movsd
        movsd
        movsd
        movsd
        sub    esi, 8
        movsd
        movsd
        pop    esi
        push   edx
        mov    eax, dword ptr [esi + 20h]
        mov    edx, dword ptr [esi + 1ch]
        stosd
        xchg   edx, eax
        stosd
        xchg   edx, eax
        stosd
        xchg   edx, eax
        stosd
        pop    edx
        mov    eax, dword ptr [esi]
        stosd
        mov    ebp, edi
        stosd
        cmp    word ptr [edx + 28h], 101h
        je     SHORT loc_1_CF91
        sub    eax, eax
        stosd
        cmp    word ptr [edx + 28h], 102h
        je     SHORT loc_1_CF91
        push   ecx
        push   esi
        push   edx
        push   edi
        stosw
        lea    esi, [esi + 234h]
        mov    al, byte ptr [edx + 2ch]
        mov    edx, 18h
        mov    ecx, 0ffffffffh
        call   sub_1_CC0A
        jae    SHORT loc_1_CF87
        int3
loc_1_CF87:
        pop    edi
        mov    word ptr [edi], ax
        add    edi, 1ah
        pop    edx
        pop    esi
        pop    ecx
loc_1_CF91:
        mov    dword ptr [esp + 18h], edi
        lea    esi, [esi + 2ch]
        mov    al, byte ptr [edx + 2ch]
        push   ecx
        push   edx
        mov    edx, ecx
        call   sub_1_CC0A
        pop    esi
        pop    edx
        pop    ecx
        jb     SHORT loc_1_CFC4
        mov    dword ptr [ebp], eax
        sub    edx, eax
        and    dl, 0feh
        sub    dword ptr [esp + 4], edx
        add    dword ptr [esp + 8], edx
        add    eax, dword ptr [esp + 14h]
        inc    eax
        and    al, 0feh
        sub    eax, ecx
        mov    dword ptr [ecx], eax
loc_1_CFC4:
        mov    eax, ebx
        ret
loc_1_CFC7:
        mov    eax, 60001h
        stc
        ret
        db  0B8h, 001h, 000h, 005h, 000h, 0F9h, 0C3h
loc_1_CFD5:
        add    esp, 8
        pop    ebx
        mov    eax, 590001h
        stc
        ret
loc_1_CFE0:
        mov    word ptr [eax + 2ch], 8
        cmp    word ptr [eax + 14h], 0ch
        jb     NEAR PTR loc_1_C5CC
        mov    ecx, dword ptr [eax + 1ch]
        movzx  ebx, word ptr [ecx]
        and    bh, 1fh
        mov    bp, word ptr [edx + 18h]
        db  08Ah, 093h, 01Ch, 04Ah, 001h, 000h
        db  08Bh, 01Ch, 09Dh, 01Ch, 03Ah, 001h, 000h
        test   ebx, ebx
        je     SHORT loc_1_CFC7
        cmp    dword ptr [ebx + 0ch], edi
        jne    SHORT loc_1_CFC7
        mov    dh, byte ptr [ecx + 1]
        shr    dh, 5
        and    dl, 7
        cmp    dh, dl
        jne    SHORT loc_1_CFC7
        cmp    word ptr [ebx + 14h], bp
        jne    SHORT loc_1_CFC7
        cmp    byte ptr [ebx + 19h], 2
        jne    SHORT loc_1_CFC7
        mov    dword ptr [esi + 2ch], ebx
        push   ebx
        push   ecx
        push   eax
        movzx  eax, word ptr [eax + 2eh]
        call   sub_1_7879
        jb     SHORT loc_1_CFD5
        mov    ebx, eax
        pop    eax
        mov    dword ptr [eax + 34h], ebx
        mov    dword ptr [eax + 3ch], ecx
        pop    ecx
        pop    ebx
        push   eax
        movzx  edx, word ptr [ecx + 2]
        lea    ebp, [ecx + 6]
        mov    ecx, edx
        movzx  edx, word ptr [eax + 2eh]
        mov    edi, dword ptr [eax + 34h]
        mov    eax, ebx
        push   edi
        call   sub_1_CC48
        pop    edi
        mov    edx, ebp
        mov    ebp, esi
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        jb     SHORT loc_1_D0D5
        mov    ebx, dword ptr [esi + 2ch]
        mov    eax, dword ptr [esp]
        mov    esi, ebp
        sub    esi, edi
loc_1_D079:
        mov    word ptr [eax + 2eh], si
        mov    esi, dword ptr [eax + 1ch]
        test   word ptr [esi + 0ah], 1
        jne    SHORT loc_1_D0B4
        test   word ptr [esi + 0ah], 2
        jne    SHORT loc_1_D0AE
loc_1_D090:
        mov    eax, dword ptr [eax + 30h]
        mov    word ptr [eax], cx
        mov    word ptr [eax + 2], 0
        mov    word ptr [eax + 4], 0
        sub    edx, edi
        mov    word ptr [eax + 6], dx
loc_1_D0A8:
        pop    eax
        mov    eax, dword ptr [eax + 4]
        clc
        ret
loc_1_D0AE:
        test   byte ptr [ebx + 2ch], 2
        je     SHORT loc_1_D090
loc_1_D0B4:
        pushad
        call   sub_1_C8F8
        popad
        mov    eax, dword ptr [eax + 30h]
        mov    word ptr [eax], cx
        mov    word ptr [eax + 2], 1
        mov    word ptr [eax + 4], 0
        mov    edi, dword ptr [esp]
        mov    edi, dword ptr [edi + 34h]
        jmp    SHORT loc_1_D0A8
loc_1_D0D5:
        cmp    al, 12h
        jne    SHORT loc_1_D0E7
        mov    ebx, dword ptr [esi + 2ch]
        mov    eax, dword ptr [esp]
        sub    esi, esi
        sub    ecx, ecx
        mov    edx, edi
        jmp    SHORT loc_1_D079
loc_1_D0E7:
        push   ebx
        mov    ebx, dword ptr [esi + 2ch]
        pop    ebx
        sub    ah, ah
        shl    eax, 10h
        mov    al, 1
        add    esp, 4
        stc
        ret
loc_1_D0F8:
        mov    ecx, dword ptr [esi + 24h]
        movzx  ebx, word ptr [ecx + 1]
        and    bh, 1fh
        mov    bp, word ptr [edx + 18h]
        db  08Ah, 093h, 01Ch, 04Ah, 001h, 000h
        db  08Bh, 01Ch, 09Dh, 01Ch, 03Ah, 001h, 000h
        test   ebx, ebx
        je     SHORT loc_1_D18E
        cmp    dword ptr [ebx + 0ch], edi
        jne    SHORT loc_1_D18E
        mov    dh, byte ptr [ecx + 2]
        shr    dh, 5
        and    dl, 7
        cmp    dh, dl
        jne    SHORT loc_1_D18E
        cmp    word ptr [ebx + 14h], bp
        jne    SHORT loc_1_D18E
        cmp    byte ptr [ebx + 19h], 2
        jne    SHORT loc_1_D18E
        mov    ecx, ebx
        mov    edx, dword ptr [ecx + 1ch]
        and    byte ptr [ecx + 18h], 80h
        call   sub_1_1B8C
        cmp    byte ptr [ecx + 18h], 0
        jne    SHORT loc_1_D177
        mov    eax, dword ptr [ecx + 48h]
        test   eax, eax
        je     SHORT loc_1_D15B
        push   ecx
        push   edx
        push   eax
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
loc_1_D15B:
        call   sub_1_1AB4
        mov    eax, edx
        push   edi
        sub    edi, edi
        call   sub_1_CB7A
        pop    edi
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        jb     NEAR PTR loc_1_002C
loc_1_D177:
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0
        mov    word ptr [ebx + 1], 0
        lea    eax, [ebx + 3]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
loc_1_D18E:
        mov    eax, 60001h
        jmp    NEAR PTR loc_1_0033
loc_1_D198:
        cmp    word ptr [eax + 14h], 7
        jb     NEAR PTR loc_1_C5CC
        mov    word ptr [eax + 2ch], 2
        mov    ecx, dword ptr [eax + 30h]
        mov    word ptr [ecx], 0
        push   eax
        mov    eax, dword ptr [eax + 1ch]
        push   dword ptr [eax]
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        pop    ecx
        jb     NEAR PTR loc_1_D2EC
        mov    ebx, dword ptr [eax + 0ch]
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_D1FA
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_D1FA
        sub    cx, 3
        jb     SHORT loc_1_D1F0
        cmp    cx, 2
        jae    SHORT loc_1_D1FA
        test   byte ptr [ebx + 10h], 1
        jne    SHORT loc_1_D1FA
        jmp    NEAR PTR loc_1_D2FD
loc_1_D1F0:
        test   byte ptr [ebx + 10h], 80h
        je     NEAR PTR loc_1_D2FD
loc_1_D1FA:
        mov    edx, dword ptr [esp]
        mov    esi, dword ptr [edx + 1ch]
        lea    esi, [esi + 6]
        movzx  eax, byte ptr [ebx + 0ah]
        add    ax, word ptr [edx + 14h]
        sub    eax, 6
        push   ebx
        call   sub_1_1CA4
        pop    ebx
        jb     NEAR PTR loc_1_D2EC
        mov    ebp, eax
        mov    ecx, dword ptr [edx + 1ch]
        movzx  ecx, word ptr [ecx]
        dec    ecx
        jne    SHORT loc_1_D22E
        mov    word ptr [edx + 2eh], 16h
        jmp    SHORT loc_1_D23C
loc_1_D22E:
        dec    ecx
        jne    NEAR PTR loc_1_D33B
        mov    word ptr [edx + 2eh], 1ah
        nop
loc_1_D23C:
        push   ebp
        movzx  eax, byte ptr [ebx + 0bh]
        call   sub_1_992C
        jb     NEAR PTR loc_1_D314
        mov    eax, esi
        mul    edi
        push   eax
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    esi, dword ptr [esi + 40h]
        xor    edx, edx
        call   sub_1_9F3C
        jb     NEAR PTR loc_1_D313
        mov    esi, dword ptr [esp + 8]
        mov    esi, dword ptr [esi + 30h]
        add    esi, 2
        mov    dword ptr [esi + 0ch], ebp
        mov    word ptr [esi + 14h], bx
        mov    dword ptr [esi + 16h], 0
        push   ecx
        push   edx
        push   eax
        call   sub_1_9D5E
        pop    ecx
        mov    dword ptr [esi + 8], eax
        call   sub_1_9D5E
        pop    ecx
        mov    dword ptr [esi], eax
        call   sub_1_9D5E
        pop    ecx
        mov    dword ptr [esi + 4], eax
        pop    ecx
        mov    eax, dword ptr [esi + 0ch]
        sub    edx, edx
        dec    ecx
        add    eax, ecx
        adc    edx, edx
        push   eax
        inc    ecx
        div    ecx
        pop    eax
        sub    eax, edx
        mov    dword ptr [esi + 10h], eax
        pop    eax
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        pop    eax
        mov    dword ptr [eax + 34h], esi
        sub    eax, eax
        clc
        ret
loc_1_D2D0:
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        mov    eax, 10001h
        nop
loc_1_D2EC:
        pop    ebx
        mov    word ptr [ebx + 2eh], 0
        mov    ebx, dword ptr [ebx + 30h]
        mov    word ptr [ebx], 0
        clc
        ret
loc_1_D2FD:
        mov    eax, 50001h
        pop    ebx
        mov    word ptr [ebx + 2eh], 0
        mov    ebx, dword ptr [ebx + 30h]
        mov    word ptr [ebx], 0
        clc
        ret
loc_1_D313:
        pop    ecx
loc_1_D314:
        sub    ah, ah
        pop    ecx
        push   eax
        push   ecx
        push   edx
        mov    edx, ecx
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        pop    ecx
        mov    word ptr [ecx + 2eh], 0
        shl    eax, 10h
        mov    al, 1
        clc
        ret
loc_1_D33B:
        dec    ecx
        je     SHORT loc_1_D344
        dec    ecx
        jne    SHORT loc_1_D364
        mov    eax, eax
loc_1_D344:
        mov    ecx, dword ptr [edx + 30h]
        add    ecx, 2
        mov    dword ptr [edx + 34h], ecx
        mov    word ptr [ecx], 0
        mov    word ptr [edx + 2eh], 2
        jmp    SHORT loc_1_D374
        db  083h, 0C4h, 00Ch, 0B8h, 008h, 000h, 001h, 000h, 0F9h, 0C3h
loc_1_D364:
        dec    ecx
        je     NEAR PTR loc_1_D2D0
        dec    ecx
        jne    SHORT loc_1_D390
        mov    word ptr [edx + 2eh], 0
loc_1_D374:
        mov    eax, ebp
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        pop    edx
        sub    eax, eax
        ret
loc_1_D390:
        sub    ecx, 102h
        jne    NEAR PTR loc_1_D2D0
        push   ebp
        mov    edx, dword ptr [edx + 30h]
        add    edx, 6
        mov    al, 80h
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    esi, dword ptr [esi + 40h]
        call   sub_1_9F3C
        jb     NEAR PTR loc_1_D314
        pop    edx
        push   eax
        push   ecx
        push   edx
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        pop    edx
        mov    edi, dword ptr [edx + 30h]
        add    edi, 2
        mov    dword ptr [edx + 34h], edi
        mov    dword ptr [edi], eax
        add    eax, 4
        mov    word ptr [edx + 2eh], ax
        sub    eax, eax
        ret
loc_1_D3E4:
        mov    ecx, dword ptr [eax + 1ch]
        mov    dword ptr [eax + 34h], ecx
        cmp    word ptr [ecx], 1
        jne    SHORT loc_1_D44C
        push   eax
        push   ecx
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_D6A1
        mov    eax, dword ptr [eax + 0ch]
        movzx  eax, byte ptr [eax + 0bh]
        call   sub_1_992C
        jb     SHORT loc_1_D435
        pop    ebx
        pop    eax
        cmp    word ptr [eax + 2eh], 12h
        jb     SHORT loc_1_D445
        mov    dword ptr [ebx + 4], esi
        mov    dword ptr [ebx + 8], ecx
        mov    dword ptr [ebx + 0ch], edx
        mov    word ptr [ebx + 10h], di
        mov    word ptr [eax + 2eh], 12h
        mov    word ptr [eax + 2ch], 0
        sub    eax, eax
        clc
        ret
loc_1_D435:
        pop    ebx
        pop    ebx
loc_1_D437:
        sub    ah, ah
        shl    eax, 10h
        mov    ax, 1
        jmp    NEAR PTR loc_1_D6AC
loc_1_D445:
        mov    eax, 0ea0001h
        stc
        ret
loc_1_D44C:
        cmp    word ptr [ecx], 2
        jne    NEAR PTR loc_1_D4F4
        cmp    word ptr [eax + 2eh], 11h
        jb     SHORT loc_1_D445
        push   eax
        push   ecx
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_D6A1
        mov    eax, dword ptr [eax + 0ch]
        movzx  ebx, byte ptr [eax + 0bh]
        inc    ebx
        db  0A1h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [eax + 30h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    eax, dword ptr [eax + 20h]
        mov    di, word ptr [eax + 1ah]
        mov    edx, ecx
        lea    edx, [edx + 5]
        call   sub_1_2EBC
        mov    byte ptr [edx - 1], al
        call   sub_1_9AE8
        jb     SHORT loc_1_D4DA
        call   sub_1_9B1C
        jb     SHORT loc_1_D4DA
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax], 6900h
        call   sub_1_9B3C
        jb     SHORT loc_1_D4CB
        mov    edi, ecx
        mov    ecx, eax
        mov    esi, dword ptr [eax + 20h]
        lea    esi, [esi + 2]
        movsd
        mov    eax, ecx
        mov    ebx, 11h
        jmp    SHORT loc_1_D4DA
loc_1_D4CB:
        clc
        mov    edi, ecx
        mov    ecx, eax
        xor    eax, eax
        stosd
        mov    eax, ecx
        mov    ebx, 11h
loc_1_D4DA:
        call   sub_1_9B74
        pop    ecx
        pop    edx
        jb     NEAR PTR loc_1_D437
        mov    word ptr [edx + 2eh], bx
        mov    word ptr [edx + 2ch], 0
        sub    eax, eax
        ret
loc_1_D4F4:
        cmp    word ptr [ecx], 102h
        jne    NEAR PTR loc_1_D5D3
        cmp    word ptr [eax + 2eh], 2ah
        jb     NEAR PTR loc_1_D445
        push   eax
        push   ecx
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_D6A1
        mov    eax, dword ptr [eax + 0ch]
        movzx  ebx, byte ptr [eax + 0bh]
        inc    ebx
        db  0A1h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [eax + 30h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    eax, dword ptr [eax + 20h]
        mov    di, word ptr [eax + 1ah]
        mov    edx, ecx
        lea    edx, [edx + 12h]
        call   sub_1_2EBC
        mov    dword ptr [edx - 6], eax
        call   sub_1_9AE8
        jb     SHORT loc_1_D4DA
        push   eax
        push   edx
        push   ecx
        push   esi
        push   edi
        mov    edx, ecx
        mov    ecx, dword ptr [edx + 0ch]
        lea    esi, [edx + 12h]
        lea    edi, [eax + 2ah]
        rep movsd
        mov    ecx, dword ptr [edx + 0ch]
        lea    edi, [edx + 12h]
        lea    esi, [eax + 2ah]
        push   edx
        push   1
        push   ecx
        push   esi
        push   edi
        db  0CDh, 020h, 042h, 000h, 040h, 000h
        add    esp, 10h
        pop    edx
        mov    dword ptr [edx + 0ch], eax
        pop    edi
        pop    esi
        pop    ecx
        pop    edx
        pop    eax
        call   sub_1_9B1C
        jb     NEAR PTR loc_1_D4DA
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax], 6900h
        call   sub_1_9B3C
        mov    edi, ecx
        mov    ecx, eax
        mov    esi, dword ptr [eax + 20h]
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 4], 0
        mov    byte ptr [edi + 10h], 0
        jb     SHORT loc_1_D5C3
        lea    esi, [esi + 2]
        movsd
        mov    eax, ecx
        mov    ebx, 2ah
        jmp    NEAR PTR loc_1_D4DA
loc_1_D5C3:
        clc
        xor    eax, eax
        stosd
        mov    eax, ecx
        mov    ebx, 2ah
        jmp    NEAR PTR loc_1_D4DA
loc_1_D5D3:
        cmp    word ptr [ecx], 105h
        jne    NEAR PTR loc_1_D6A6
        cmp    word ptr [eax + 2eh], 8dh
        jb     NEAR PTR loc_1_D445
        push   eax
        push   ecx
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_D6A1
        mov    eax, dword ptr [eax + 0ch]
        mov    esi, dword ptr [eax + 2ch]
        push   ecx
        movzx  ecx, byte ptr [eax + 0ah]
        push   eax
        db  0A1h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [eax + 30h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    eax, dword ptr [eax + 20h]
        mov    di, word ptr [eax + 1ah]
        pop    eax
        call   sub_1_9AE8
        jb     NEAR PTR loc_1_D4DA
        call   sub_1_9B1C
        jb     NEAR PTR loc_1_D4DA
        call   sub_1_9B2C
        jb     NEAR PTR loc_1_D4DA
        dec    ecx
        mov    edi, dword ptr [eax + 20h]
        js     SHORT loc_1_D647
        rep movsb
loc_1_D647:
        mov    byte ptr [edi], 0
        pop    ecx
        mov    dword ptr [eax + 8], 80h
        mov    dword ptr [eax], 71a0h
        call   sub_1_9B3C
        jb     NEAR PTR loc_1_D4DA
        mov    edi, ecx
        mov    ebx, eax
        movzx  eax, word ptr [ebp + 10h]
        stosd
        movzx  eax, word ptr [ebp + 18h]
        stosd
        push   edi
        stosd
        mov    esi, dword ptr [ebx + 20h]
        mov    ecx, 0ffffffffh
        mov    al, 80h
        mov    edx, 80h
        call   sub_1_CBB1
        pop    edi
        jb     SHORT loc_1_D694
        stosd
        xchg   ebx, eax
        add    ebx, 0ch
        jmp    NEAR PTR loc_1_D4DA
loc_1_D694:
        mov    eax, ebx
        mov    dword ptr [eax], 6fh
        jmp    NEAR PTR loc_1_D4DA
loc_1_D6A1:
        add    esp, 8
        jmp    SHORT loc_1_D6AC
loc_1_D6A6:
        mov    eax, 400002h
        nop
loc_1_D6AC:
        stc
        ret
        db  08Bh, 0C0h
loc_1_D6B0:
        sub    esp, 18h
        push   esi
        mov    esi, dword ptr [eax + 1ch]
        movzx  ecx, word ptr [esi + 2]
        pop    esi
        mov    dword ptr [esp + 14h], eax
        cmp    ecx, 107h
        jne    SHORT loc_1_D6D6
        cmp    word ptr [eax + 2eh], 14ah
        jb     NEAR PTR loc_1_D8BC
        jmp    SHORT loc_1_D6ED
loc_1_D6D6:
        cmp    ecx, 101h
        jne    NEAR PTR loc_1_D8C6
        cmp    word ptr [eax + 2eh], 28h
        jb     NEAR PTR loc_1_D8BC
loc_1_D6ED:
        mov    word ptr [eax + 2ch], 2
        mov    ebx, dword ptr [eax + 1ch]
        mov    al, 0
        movzx  ecx, word ptr [ebx]
        call   sub_1_18B8
        jb     NEAR PTR loc_1_D8CC
        mov    ebp, dword ptr [ecx + 26h]
        mov    edx, dword ptr [ecx + 22h]
        mov    dword ptr [esp + 4], edx
        xor    edx, edx
        call   sub_1_9F3C
        mov    dword ptr [esp], ebp
        mov    ah, 0
        jb     NEAR PTR loc_1_D8B0
        call   sub_1_9AE8
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax], 5706h
        call   sub_1_9B3C
        jb     NEAR PTR loc_1_D8E3
        mov    ecx, dword ptr [ebp + 14h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 18h]
        push   eax
        push   ecx
        db  0CDh, 020h, 039h, 000h, 040h, 000h
        add    esp, 4
        xchg   ecx, eax
        pop    eax
        mov    edi, dword ptr [eax + 20h]
        mov    dword ptr [edi + 8], edx
        mov    dword ptr [edi + 4], ecx
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax], 5700h
        call   sub_1_9B3C
        jb     NEAR PTR loc_1_D8E3
        mov    ecx, dword ptr [ebp + 14h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 18h]
        push   eax
        push   ecx
        db  0CDh, 020h, 039h, 000h, 040h, 000h
        add    esp, 4
        xchg   ecx, eax
        pop    eax
        mov    edi, dword ptr [eax + 20h]
        mov    dword ptr [edi + 18h], edx
        mov    dword ptr [edi + 14h], ecx
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax], 5704h
        call   sub_1_9B3C
        jb     NEAR PTR loc_1_D8E3
        mov    ecx, dword ptr [ebp + 14h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 18h]
        push   eax
        push   ecx
        db  0CDh, 020h, 039h, 000h, 040h, 000h
        add    esp, 4
        xchg   ecx, eax
        pop    eax
        mov    edi, dword ptr [eax + 20h]
        mov    dword ptr [edi + 10h], edx
        mov    dword ptr [edi + 0ch], ecx
        mov    edx, dword ptr [esp + 14h]
        mov    esi, dword ptr [edx + 1ch]
        movzx  ecx, word ptr [esi + 2]
        mov    word ptr [esp + 8], cx
        cmp    ecx, 101h
        je     SHORT loc_1_D7F3
        cmp    ecx, 107h
        jne    NEAR PTR loc_1_D8C6
loc_1_D7F3:
        mov    esi, dword ptr [edx + 30h]
        mov    word ptr [esi], 0
        add    esi, 2
        mov    dword ptr [edx + 34h], esi
        mov    word ptr [edx + 2eh], 28h
        mov    dword ptr [esi + 20h], ebx
        mov    edi, dword ptr [eax + 20h]
        mov    edx, dword ptr [edi + 8]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [esi], ecx
        mov    edx, dword ptr [edi + 10h]
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 0ch], edx
        mov    dword ptr [esi + 8], ecx
        mov    edx, dword ptr [edi + 18h]
        mov    ecx, dword ptr [edi + 14h]
        mov    dword ptr [esi + 14h], edx
        mov    dword ptr [esi + 10h], ecx
        mov    dword ptr [esi + 1ch], edx
        mov    dword ptr [esi + 18h], ecx
        cmp    word ptr [esp + 8], 107h
        jne    SHORT loc_1_D8A3
        lea    edi, [edi + 2ch]
        mov    dword ptr [esp + 10h], edi
        mov    ecx, dword ptr [esp]
        mov    dword ptr [esi + 28h], ecx
        mov    dword ptr [esi + 30h], ecx
        xor    ecx, ecx
        mov    dword ptr [esi + 2ch], ecx
        mov    dword ptr [esi + 34h], ecx
        mov    dword ptr [esi + 3ch], ecx
        mov    byte ptr [esi + 3ch], cl
        test   ebx, 10h
        jne    SHORT loc_1_D8AA
        mov    byte ptr [esi + 3dh], cl
loc_1_D868:
        mov    dword ptr [esi + 40h], ecx
        inc    ecx
        mov    dword ptr [esi + 38h], ecx
        lea    esi, [esi + 44h]
        mov    dword ptr [esp + 0ch], esi
        lea    edi, [esi + 4]
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, 0ffffffffh
        mov    edx, 0ffh
        xor    eax, eax
        call   sub_1_CC0A
        jb     SHORT loc_1_D8B0
        mov    edi, dword ptr [esp + 0ch]
        mov    ecx, eax
        add    ecx, 48h
        mov    dword ptr [edi], eax
        mov    edx, dword ptr [esp + 14h]
        mov    word ptr [edx + 2eh], cx
loc_1_D8A3:
        sub    eax, eax
        clc
        add    esp, 18h
        ret
loc_1_D8AA:
        mov    byte ptr [esi + 3dh], 1
        jmp    SHORT loc_1_D868
loc_1_D8B0:
        shl    eax, 10h
        mov    ax, 1
        clc
        add    esp, 18h
        ret
loc_1_D8BC:
        mov    eax, 0ea0001h
        stc
        add    esp, 18h
        ret
loc_1_D8C6:
        mov    eax, 10001h
        nop
loc_1_D8CC:
        mov    ebx, dword ptr [esp + 14h]
        mov    word ptr [ebx + 2eh], 0
        mov    ecx, dword ptr [ebx + 30h]
        mov    word ptr [ecx], 0
        clc
        add    esp, 18h
        ret
loc_1_D8E3:
        call   sub_1_9B74
        jb     SHORT loc_1_D8B0
        mov    eax, eax
loc_1_D8EC:
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [eax + 1ch]
        movzx  ecx, word ptr [esi + 2]
        pop    esi
        mov    dword ptr [esp + 4], eax
        cmp    word ptr [eax + 14h], 6
        jb     NEAR PTR loc_1_C5CC
        mov    word ptr [eax + 2ch], 2
        mov    word ptr [eax + 2eh], 0
        mov    ebx, dword ptr [eax + 1ch]
        movzx  ecx, word ptr [ebx]
        mov    al, 0
        call   sub_1_18B8
        jb     NEAR PTR loc_1_DA18
        mov    eax, dword ptr [ecx + 22h]
        movzx  ecx, word ptr [ebx + 2]
        cmp    ecx, 101h
        jne    NEAR PTR loc_1_DA11
        mov    ebx, dword ptr [esp + 4]
        cmp    word ptr [ebx + 16h], 28h
        jne    NEAR PTR loc_1_C5CC
        mov    esi, dword ptr [ebx + 20h]
        xchg   edx, eax
        call   sub_1_9AE8
        jb     NEAR PTR loc_1_D9F7
        push   eax
        push   edx
        push   dword ptr [esi + 14h]
        push   dword ptr [esi + 10h]
        call   sub_1_9D3F
        add    esp, 8
        push   eax
        call   sub_1_9D5E
        pop    ecx
        mov    ecx, eax
        pop    edx
        pop    eax
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 8], ecx
        shr    ecx, 10h
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [eax], 5701h
        call   sub_1_9B3C
        jb     SHORT loc_1_D9F7
        push   eax
        push   edx
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   sub_1_9D3F
        add    esp, 8
        push   eax
        call   sub_1_9D5E
        pop    ecx
        mov    ecx, eax
        pop    edx
        pop    eax
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 8], ecx
        shr    ecx, 10h
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [eax + 10h], 0
        mov    dword ptr [eax], 5707h
        call   sub_1_9B3C
        jb     SHORT loc_1_D9F7
        push   eax
        push   edx
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        call   sub_1_9D3F
        add    esp, 8
        push   eax
        call   sub_1_9D5E
        pop    ecx
        mov    ecx, eax
        pop    edx
        pop    eax
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 8], ecx
        shr    ecx, 10h
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [eax], 5705h
        call   sub_1_9B3C
loc_1_D9F7:
        call   sub_1_9B74
        jb     SHORT loc_1_DA05
        sub    eax, eax
        clc
        add    esp, 8
        ret
loc_1_DA05:
        shl    eax, 10h
        mov    ax, 1
        clc
        add    esp, 8
        ret
loc_1_DA11:
        mov    eax, 10001h
        mov    eax, eax
loc_1_DA18:
        mov    ebx, dword ptr [esp + 4]
        mov    ebx, dword ptr [ebx + 30h]
        mov    word ptr [ebx], 0
        clc
        add    esp, 8
        ret
        db  000h, 000h, 000h
_LTEXT ends
        end
