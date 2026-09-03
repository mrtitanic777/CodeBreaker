; ppm_lcode_01.asm -- PPM object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 050h, 050h, 04Dh, 020h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  070h, 050h, 06Fh, 072h, 074h, 020h, 03Dh, 020h, 023h, 065h, 061h, 078h, 00Dh, 00Ah, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00F0:
        inc    eax
        add    byte ptr [edx], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 001h, 010h, 00Ch, 000h, 007h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 07Fh, 000h, 000h
loc_1_0134:
        nop
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    dword ptr [edx], eax
        add    eax, dword ptr [eax + 9080706h]
        or     cl, byte ptr [ebx]
        or     al, 0dh
        push   cs
        movups xmm2, xmmword ptr [ecx]
        adc    dl, byte ptr [ebx]
        adc    al, 15h
        push   ss
        pop    ss
        sbb    byte ptr [ecx], bl
        sbb    bl, byte ptr [ebx]
        sbb    al, 1dh
        push   ds
        pop    ds
        and    byte ptr [ecx], ah
        and    ah, byte ptr [ebx]
        and    al, 25h
        daa
        sub    byte ptr [ecx], ch
        sub    ch, byte ptr [ebx]
        sub    al, 2dh
        das
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
        cmp    byte ptr [ecx], bh
        cmp    bh, byte ptr [ebx]
        cmp    al, 3dh
        aas
        add    byte ptr [ecx], al
        add    al, byte ptr [ebx]
        add    al, 5
        push   es
        pop    es
        or     byte ptr [ecx], cl
        or     cl, byte ptr [ebx]
        or     al, 0dh
        push   cs
        movups xmm2, xmmword ptr [ecx]
        adc    dl, byte ptr [ebx]
        adc    al, 15h
        push   ss
        pop    ss
        sbb    byte ptr [ecx], bl
        sbb    bl, byte ptr [ebx]
        sbb    al, 1dh
        push   ds
        pop    ds
        and    byte ptr [ecx], ah
        and    ah, byte ptr [ebx]
        and    al, 25h
        daa
        sub    byte ptr [ecx], ch
        sub    ch, byte ptr [ebx]
        sub    al, 2dh
        das
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
        cmp    byte ptr [ecx], bh
        cmp    bh, byte ptr [ebx]
        cmp    al, 3dh
        aas
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
        adc    byte ptr [eax], dl
loc_1_0238:
        add    byte ptr [eax], 0
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        add    byte ptr [ecx + eax + 34000000h], cl
        add    byte ptr [eax], al
        add    byte ptr [eax + 2], al
        aas
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4], al
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        or     dword ptr [eax], 0
        add    byte ptr [eax], al
        aas
        add    byte ptr [eax], al
        add    byte ptr [eax + edx + 3f000000h], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 20h], al
        aas
        add    byte ptr [eax], al
        add    byte ptr [eax + 200h], cl
        retf   0
        db  000h, 08Eh, 000h, 004h, 000h, 000h, 03Fh, 000h, 000h, 000h, 093h, 000h, 008h, 000h, 000h, 03Fh
        db  000h, 000h, 000h, 094h, 000h, 010h, 000h, 000h, 03Fh, 000h, 000h, 000h, 095h, 000h, 020h, 000h
        db  000h, 041h, 000h, 000h, 000h, 097h, 000h, 040h, 000h, 000h, 03Fh, 000h, 000h, 000h, 096h, 000h
        db  001h, 000h, 000h, 03Fh, 000h, 000h, 000h, 08Dh, 000h, 000h
loc_1_02B8:
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        add    byte ptr [edx + 10000h], cl
        aas
        add    byte ptr [eax], al
        db  000h, 098h, 000h, 000h
PPM_Control:
        cmp    eax, 0bh
        jne    SHORT loc_1_02DB
        push   ebx
        call   sub_1_1B96
        cmp    eax, 1
        ret
loc_1_02DB:
        cmp    eax, 1bh
        jne    SHORT loc_1_02E9
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_02E5:
        cmp    eax, 1
        ret
loc_1_02E9:
        cmp    eax, 1ch
        jne    SHORT loc_1_02F7
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_02F3:
        cmp    eax, 1
        ret
loc_1_02F7:
        clc
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_02FC:
        push   edx
        call   sub_1_10E0
        add    esp, 4
        ret
        db  08Bh, 0C0h
loc_1_0308:
        push   edx
        call   sub_1_1042
        add    esp, 4
        ret
        db  08Bh, 0C0h
loc_1_0314:
        push   edx
        push   ebx
        push   eax
        call   sub_1_0FC8
        add    esp, 0ch
        cmp    eax, 1
        ret
        db  055h, 0A1h, 000h, 000h, 000h, 000h, 08Bh, 0ECh, 083h, 0ECh, 004h, 050h, 0E8h, 0D8h, 024h, 000h
        db  000h, 089h, 045h, 0FCh, 085h, 0C0h, 074h, 022h, 08Bh, 045h, 0FCh, 068h, 000h, 000h, 000h, 000h
        db  0E8h, 0BCh, 024h, 000h, 000h, 0FFh, 075h, 0FCh, 0FFh, 035h, 000h, 000h, 000h, 000h, 0E8h, 09Eh
        db  024h, 000h, 000h, 089h, 045h, 0FCh, 085h, 0C0h, 075h, 0DEh, 0B8h, 001h, 000h, 000h, 000h, 08Bh
        db  0E5h, 05Dh, 0C3h, 0C3h
sub_1_0367:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jae    SHORT loc_1_0378
        xor    eax, eax
loc_1_0378:
        pop    edi
        pop    ebp
        ret
sub_1_037B:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        test   edx, edx
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        je     SHORT loc_1_03D1
        mov    eax, dword ptr [ecx + 0f8h]
        mov    bl, byte ptr [ecx + 102h]
        shr    bl, 4
        mov    al, byte ptr [eax]
        movzx  esi, bl
        shr    al, 4
        xor    ebx, ebx
        movzx  eax, al
        not    eax
        and    eax, esi
        mov    bl, byte ptr [ecx + 0fch]
        and    ebx, 60h
        mov    esi, dword ptr [ecx + 4ch]
        or     eax, ebx
        mov    dword ptr [edx + 4], esi
        xor    ebx, ebx
        mov    bl, byte ptr [ecx + 100h]
        and    ebx, 18h
        or     eax, ebx
        mov    ebx, dword ptr [ecx + 58h]
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 8], ebx
loc_1_03D1:
        mov    eax, 1
        pop    esi
        mov    dword ptr [ecx + 0ch], 0
        pop    ebx
        pop    ebp
        ret
sub_1_03E1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 3bh], 1
        je     SHORT loc_1_040B
        push   esi
        call   sub_1_17B2
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_040B
        mov    dword ptr [esi + 0ch], 0fffffffbh
        xor    eax, eax
        jmp    NEAR PTR loc_1_0776
loc_1_040B:
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [esi + 177h], 0
        mov    dword ptr [esi + 94h], 30h
        test   cl, 1
        je     SHORT loc_1_0430
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [esi + 98h], edx
loc_1_0430:
        mov    ebx, ecx
        and    ebx, 2
        je     SHORT loc_1_0440
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [esi + 9ch], edx
loc_1_0440:
        test   cl, 4
        je     SHORT loc_1_044E
        mov    edx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 0a0h], edx
loc_1_044E:
        test   cl, 8
        je     SHORT loc_1_045C
        mov    edx, dword ptr [eax + 10h]
        mov    dword ptr [esi + 0a4h], edx
loc_1_045C:
        mov    edi, ecx
        and    edi, 70h
        je     SHORT loc_1_047E
        mov    dl, byte ptr [eax + 16h]
        mov    byte ptr [esi + 0aah], dl
        mov    dl, byte ptr [eax + 17h]
        mov    byte ptr [esi + 0abh], dl
        mov    dl, byte ptr [eax + 18h]
        mov    byte ptr [esi + 0ach], dl
loc_1_047E:
        test   cl, 80h
        je     SHORT loc_1_048C
        mov    dl, byte ptr [eax + 19h]
        mov    byte ptr [esi + 0adh], dl
loc_1_048C:
        test   ch, 1
        je     SHORT loc_1_049A
        mov    dl, byte ptr [eax + 1ah]
        mov    byte ptr [esi + 0aeh], dl
loc_1_049A:
        test   ch, 2
        je     SHORT loc_1_04A8
        mov    dl, byte ptr [eax + 1bh]
        mov    byte ptr [esi + 0afh], dl
loc_1_04A8:
        test   ch, 4
        je     SHORT loc_1_04B6
        mov    dl, byte ptr [eax + 1ch]
        mov    byte ptr [esi + 0b0h], dl
loc_1_04B6:
        test   ch, 8
        je     SHORT loc_1_04C4
        mov    dl, byte ptr [eax + 1dh]
        mov    byte ptr [esi + 0b1h], dl
loc_1_04C4:
        test   ch, 10h
        je     SHORT loc_1_04D2
        mov    dl, byte ptr [eax + 1eh]
        mov    byte ptr [esi + 0b2h], dl
loc_1_04D2:
        test   ch, 20h
        je     SHORT loc_1_04E0
        mov    edx, dword ptr [eax + 20h]
        mov    dword ptr [esi + 0b4h], edx
loc_1_04E0:
        test   ch, 80h
        je     SHORT loc_1_04EE
        mov    edx, dword ptr [eax + 28h]
        mov    dword ptr [esi + 0bch], edx
loc_1_04EE:
        test   ch, 40h
        je     SHORT loc_1_04FC
        mov    edx, dword ptr [eax + 24h]
        mov    dword ptr [esi + 0b8h], edx
loc_1_04FC:
        test   ecx, 10000h
        je     SHORT loc_1_050D
        mov    edx, dword ptr [eax + 2ch]
        mov    dword ptr [esi + 0c0h], edx
loc_1_050D:
        test   ch, 0e0h
        mov    ax, word ptr [eax + 14h]
        mov    word ptr [esi + 0a8h], ax
        je     SHORT loc_1_0587
        mov    edx, dword ptr [esi + 0bch]
        mov    al, 0
        mov    cl, 0
        cmp    edx, 1
        adc    al, 0ffh
        and    al, 20h
        cmp    dword ptr [esi + 0b8h], 1
        adc    cl, 0ffh
        and    cl, 10h
        or     al, cl
        mov    cl, 0
        cmp    dword ptr [esi + 0b4h], 1
        adc    cl, 0ffh
        and    cl, 80h
        or     al, cl
        not    edx
        mov    byte ptr [esi + 103h], al
        mov    eax, dword ptr [esi + 0b4h]
        not    eax
        cmp    eax, 1
        sbb    al, al
        and    al, 80h
        cmp    edx, 1
        sbb    cl, cl
        and    cl, 20h
        or     al, cl
        mov    ecx, dword ptr [esi + 0b8h]
        not    ecx
        cmp    ecx, 1
        sbb    cl, cl
        and    cl, 10h
        or     al, cl
        mov    byte ptr [esi + 104h], al
loc_1_0587:
        test   edi, edi
        je     NEAR PTR loc_1_0617
        movzx  eax, byte ptr [esi + 0aah]
        cmp    eax, 7
        je     SHORT loc_1_05A6
        mov    byte ptr [esi + 0feh], 0ffh
        mov    cl, 3
        jmp    SHORT loc_1_05AF
loc_1_05A6:
        mov    byte ptr [esi + 0feh], 7fh
        mov    cl, 2
loc_1_05AF:
        movzx  eax, byte ptr [esi + 0ach]
        cmp    eax, 2
        jne    SHORT loc_1_05BE
        or     cl, 4
loc_1_05BE:
        movzx  eax, byte ptr [esi + 0abh]
        cmp    eax, 1
        je     SHORT loc_1_05DB
        cmp    eax, 2
        je     SHORT loc_1_05E0
        cmp    eax, 3
        je     SHORT loc_1_05E5
        cmp    eax, 4
        je     SHORT loc_1_05EA
        jmp    SHORT loc_1_05ED
loc_1_05DB:
        or     cl, 8
        jmp    SHORT loc_1_05ED
loc_1_05E0:
        or     cl, 18h
        jmp    SHORT loc_1_05ED
loc_1_05E5:
        or     cl, 28h
        jmp    SHORT loc_1_05ED
loc_1_05EA:
        or     cl, 38h
loc_1_05ED:
        test   cl, 8
        je     SHORT loc_1_05FB
        or     byte ptr [esi + 9ch], 4
        jmp    SHORT loc_1_0602
loc_1_05FB:
        and    byte ptr [esi + 9ch], 0fbh
loc_1_0602:
        cmp    byte ptr [esi + 179h], cl
        je     SHORT loc_1_0617
        or     byte ptr [esi + 181h], 4
        mov    byte ptr [esi + 179h], cl
loc_1_0617:
        test   ebx, ebx
        je     SHORT loc_1_068F
        mov    ebx, dword ptr [esi + 9ch]
        mov    eax, ebx
        mov    ecx, ebx
        and    eax, 2
        and    ecx, 80h
        shr    ecx, 7
        or     al, cl
        mov    cl, al
        xor    cl, 3
        cmp    byte ptr [esi + 17ah], cl
        je     SHORT loc_1_064D
        or     byte ptr [esi + 181h], 1
        mov    byte ptr [esi + 17ah], cl
loc_1_064D:
        mov    ecx, ebx
        mov    edx, ebx
        and    ecx, 2000h
        cmp    ecx, 1
        sbb    cl, cl
        and    edx, 4000h
        not    al
        inc    cl
        cmp    edx, 1
        mov    dl, 0
        adc    dl, 0ffh
        and    dl, 2
        or     cl, dl
        and    cl, al
        mov    eax, ebx
        and    eax, 8
        and    ebx, 10h
        add    eax, eax
        add    ebx, ebx
        or     al, bl
        mov    byte ptr [esi + 101h], cl
        mov    byte ptr [esi + 102h], al
loc_1_068F:
        mov    eax, dword ptr [esi + 40h]
        sub    eax, dword ptr [esi + 0a4h]
        mov    dword ptr [esi + 0ech], eax
        mov    al, byte ptr [esi + 0fch]
        test   al, 2
        jne    SHORT loc_1_06B0
        or     al, 4
        mov    byte ptr [esi + 0fch], al
loc_1_06B0:
        or     byte ptr [esi + 181h], 10h
        call   sub_1_2824
        mov    edi, eax
loc_1_06BE:
        cli
        push   esi
        call   sub_1_1089
        add    esp, 4
        sti
        call   sub_1_2824
        sub    eax, edi
        cmp    eax, 0c8h
        jb     SHORT loc_1_06BE
        lea    edi, [esi + 146h]
        push   edi
        call   sub_1_1D06
        push   edi
        call   sub_1_1D06
        mov    byte ptr [esi + 100h], 0
        mov    ecx, dword ptr [esi + 0f8h]
        test   byte ptr [esi + 9dh], 80h
        mov    byte ptr [ecx], al
        jne    SHORT loc_1_0739
        mov    cl, byte ptr [esi + 102h]
        mov    eax, dword ptr [esi + 0f8h]
        mov    al, byte ptr [eax]
        and    al, cl
        cmp    al, cl
        je     SHORT loc_1_0739
        cmp    cl, 30h
        jne    SHORT loc_1_0732
        test   byte ptr [esi + 0fch], 1
        jne    SHORT loc_1_0732
        cmp    al, 10h
        jne    SHORT loc_1_0732
        and    cl, 0dfh
        mov    byte ptr [esi + 102h], cl
        jmp    SHORT loc_1_0739
loc_1_0732:
        mov    byte ptr [esi + 100h], 0a0h
loc_1_0739:
        cli
        mov    byte ptr [esi + 177h], 0dh
        push   edi
        call   sub_1_1D1C
        sti
        test   byte ptr [esi + 11bh], 2
        jne    SHORT loc_1_076A
        push   esi
        call   sub_1_0FB2
        add    esp, 4
        or     byte ptr [esi + 11bh], 2
        push   esi
        call   sub_1_18FA
        add    esp, 4
loc_1_076A:
        mov    dword ptr [esi + 0ch], 0
        mov    eax, 1
loc_1_0776:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_077B:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 6
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 4ch]
        rep stosd
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        jne    SHORT loc_1_07DC
        mov    bl, byte ptr [esi + 11bh]
        test   bl, 8
        je     SHORT loc_1_07BF
        mov    ebx, dword ptr [esi + 3ch]
        push   0
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        push   ebx
        call   sub_1_2834
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_07DF
        mov    eax, ebx
        mov    edi, dword ptr [esi + 40h]
        jmp    SHORT loc_1_07DF
loc_1_07BF:
        push   0
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        call   sub_1_282C
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_07DF
        or     bl, 8
        mov    byte ptr [esi + 11bh], bl
        jmp    SHORT loc_1_07DF
loc_1_07DC:
        mov    edi, dword ptr [ebp + 10h]
loc_1_07DF:
        mov    dword ptr [esi + 3ch], eax
        mov    dword ptr [esi + 40h], edi
        mov    eax, dword ptr [ebp + 14h]
        test   eax, eax
        jne    SHORT loc_1_0831
        mov    bl, byte ptr [esi + 11bh]
        test   bl, 4
        je     SHORT loc_1_0814
        mov    ebx, dword ptr [esi + 44h]
        push   0
        mov    edi, dword ptr [ebp + 18h]
        push   edi
        push   ebx
        call   sub_1_2834
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_0834
        mov    eax, ebx
        mov    edi, dword ptr [esi + 48h]
        jmp    SHORT loc_1_0834
loc_1_0814:
        push   0
        mov    edi, dword ptr [ebp + 18h]
        push   edi
        call   sub_1_282C
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_0834
        or     bl, 4
        mov    byte ptr [esi + 11bh], bl
        jmp    SHORT loc_1_0834
loc_1_0831:
        mov    edi, dword ptr [ebp + 18h]
loc_1_0834:
        mov    dword ptr [esi + 44h], eax
        mov    dword ptr [esi + 48h], edi
        test   eax, eax
        je     SHORT loc_1_0845
        or     byte ptr [esi + 11bh], 10h
loc_1_0845:
        mov    eax, 1
        pop    edi
        mov    dword ptr [esi + 0ch], 0
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_0856:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 0fch]
        test   al, 40h
        je     SHORT loc_1_0872
        mov    dword ptr [esi + 0ch], 4000h
        xor    eax, eax
        jmp    SHORT loc_1_089A
loc_1_0872:
        cli
        or     al, 40h
        mov    cl, byte ptr [ebp + 0ch]
        push   esi
        mov    byte ptr [esi + 0fch], al
        mov    byte ptr [esi + 0ffh], cl
        call   sub_1_191E
        add    esp, 4
        sti
        mov    dword ptr [esi + 0ch], 0
        mov    eax, 1
loc_1_089A:
        pop    esi
        pop    ebp
        ret
loc_1_089D:
        push   ebp
        mov    ebp, esp
        push   esi
        cli
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_1089
        add    esp, 4
        sti
        mov    ecx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   esi
        mov    dword ptr [eax], ecx
        call   sub_1_037B
        add    esp, 8
        pop    esi
        pop    ebp
        ret
loc_1_08C6:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        cmp    edi, 0c8h
        ja     SHORT loc_1_08E8
        je     NEAR PTR loc_1_0A71
        cmp    edi, 15h
        ja     SHORT loc_1_08E8
        db  0FFh, 024h, 0BDh, 000h, 000h, 000h, 000h
loc_1_08E8:
        mov    esi, dword ptr [ebp + 8]
loc_1_08EB:
        mov    dword ptr [esi + 0ch], 0ffffffech
        xor    eax, eax
        jmp    NEAR PTR loc_1_0ACE
loc_1_08F9:
        mov    esi, dword ptr [ebp + 8]
        or     byte ptr [esi + 100h], 8
        jmp    NEAR PTR loc_1_0A9C
loc_1_0908:
        mov    esi, dword ptr [ebp + 8]
        and    byte ptr [esi + 100h], 0f7h
        jmp    NEAR PTR loc_1_0A9C
loc_1_0917:
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [esi + 17ah]
        mov    al, cl
        or     al, 2
        cmp    al, cl
        je     NEAR PTR loc_1_0A9C
        or     byte ptr [esi + 181h], 1
        mov    byte ptr [esi + 17ah], al
        jmp    NEAR PTR loc_1_0A9C
loc_1_093E:
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [esi + 17ah]
        movzx  eax, cl
        mov    edx, eax
        and    dl, 0fdh
        cmp    edx, eax
        je     NEAR PTR loc_1_0A9C
        or     byte ptr [esi + 181h], 1
        and    cl, 0fdh
        mov    byte ptr [esi + 17ah], cl
        jmp    NEAR PTR loc_1_0A9C
loc_1_096C:
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [esi + 17ah]
        mov    al, cl
        or     al, 1
        cmp    al, cl
        je     NEAR PTR loc_1_0A9C
        or     byte ptr [esi + 181h], 1
        mov    byte ptr [esi + 17ah], al
        jmp    NEAR PTR loc_1_0A9C
loc_1_0993:
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [esi + 17ah]
        movzx  eax, cl
        mov    edx, eax
        and    dl, 0feh
        cmp    edx, eax
        je     NEAR PTR loc_1_0A9C
        or     byte ptr [esi + 181h], 1
        and    cl, 0feh
        mov    byte ptr [esi + 17ah], cl
        jmp    NEAR PTR loc_1_0A9C
loc_1_09C1:
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [esi + 113h]
        shl    eax, 10h
        or     eax, dword ptr [esi + 0c4h]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_0A9C
loc_1_09DD:
        mov    esi, dword ptr [ebp + 8]
        or     byte ptr [esi + 100h], 40h
        mov    al, byte ptr [esi + 17ah]
        or     al, 40h
        cmp    byte ptr [esi + 179h], al
        je     NEAR PTR loc_1_0A9C
        or     byte ptr [esi + 181h], 4
        mov    byte ptr [esi + 179h], al
        jmp    NEAR PTR loc_1_0A9C
loc_1_0A0D:
        mov    esi, dword ptr [ebp + 8]
        movzx  edx, byte ptr [esi + 179h]
        mov    cl, byte ptr [esi + 17ah]
        and    byte ptr [esi + 100h], 0bfh
        movzx  eax, cl
        and    al, 0bfh
        cmp    eax, edx
        je     SHORT loc_1_0A9C
        or     byte ptr [esi + 181h], 4
        and    cl, 0bfh
        mov    byte ptr [esi + 179h], cl
        jmp    SHORT loc_1_0A9C
loc_1_0A3F:
        mov    esi, dword ptr [ebp + 8]
        or     byte ptr [esi + 11bh], 20h
        jmp    SHORT loc_1_0A9C
loc_1_0A4B:
        mov    eax, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 12eh], eax
        jmp    SHORT loc_1_0A9C
loc_1_0A59:
        mov    esi, dword ptr [ebp + 8]
        or     byte ptr [esi + 11bh], 40h
        jmp    SHORT loc_1_0A9C
loc_1_0A65:
        mov    esi, dword ptr [ebp + 8]
        and    byte ptr [esi + 11bh], 0dfh
        jmp    SHORT loc_1_0A9C
loc_1_0A71:
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 4ch], 0
        jbe    SHORT loc_1_0A8E
        mov    eax, dword ptr [esi + 3ch]
        mov    ecx, dword ptr [esi + 50h]
        movzx  edx, byte ptr [eax + ecx]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], edx
        jmp    NEAR PTR loc_1_08EB
loc_1_0A8E:
        mov    dword ptr [esi + 0ch], 0
        xor    eax, eax
        jmp    SHORT loc_1_0ACE
loc_1_0A99:
        mov    esi, dword ptr [ebp + 8]
loc_1_0A9C:
        cli
        push   esi
        call   sub_1_1089
        add    esp, 4
        sti
        cmp    edi, 0ah
        je     SHORT loc_1_0AC2
        cmp    edi, 0c8h
        je     SHORT loc_1_0AC2
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_1_0AC2
        mov    eax, dword ptr [esi + 34h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
loc_1_0AC2:
        mov    dword ptr [esi + 0ch], 0
        mov    eax, 1
loc_1_0ACE:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0B2A:
        push   ebp
        mov    ebp, esp
        push   esi
        cli
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ecx + 0f4h]
        mov    edx, dword ptr [esi]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [ebp + 0ch]
        not    eax
        and    eax, edx
        mov    dword ptr [esi], eax
        sti
        mov    eax, 1
        pop    esi
        mov    dword ptr [ecx + 0ch], 0
        pop    ebp
        ret
loc_1_0B58:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 11bh], 10h
        jne    SHORT loc_1_0B9D
        mov    eax, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [esi + 44h], eax
        xor    eax, eax
        mov    dword ptr [esi + 48h], edi
        mov    dword ptr [esi + 58h], edi
        mov    dword ptr [esi + 60h], eax
        mov    dword ptr [esi + 5ch], eax
        mov    dword ptr [ecx], edi
        test   edi, edi
        je     SHORT loc_1_0BEC
        cmp    dword ptr [esi + 0e0h], edi
        ja     SHORT loc_1_0BEC
        and    byte ptr [esi + 0c9h], 0fdh
        jmp    SHORT loc_1_0BEC
loc_1_0B9D:
        push   esi
        call   sub_1_1969
        add    esp, 4
        test   al, al
        je     SHORT loc_1_0BB5
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], 0
        jmp    SHORT loc_1_0BEC
loc_1_0BB5:
        lea    eax, [ebp - 4]
        mov    edi, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_0D05
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 14h]
        test   eax, eax
        mov    dword ptr [ecx], eax
        je     SHORT loc_1_0BE4
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0e0h], ecx
        ja     SHORT loc_1_0BE4
        and    byte ptr [esi + 0c9h], 0fdh
loc_1_0BE4:
        cmp    edi, eax
        je     SHORT loc_1_0BEC
        or     byte ptr [esi + 35h], 1
loc_1_0BEC:
        cli
        push   esi
        call   sub_1_191E
        add    esp, 4
        sti
        mov    eax, 1
        pop    edi
        mov    dword ptr [esi + 0ch], 0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0C09:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 11bh], 40h
        jne    SHORT loc_1_0C34
        mov    eax, dword ptr [esi + 34h]
        test   eax, eax
        je     SHORT loc_1_0C34
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [esi + 0ch], eax
        xor    eax, eax
        mov    dword ptr [ecx], 0
        jmp    SHORT loc_1_0C8E
loc_1_0C34:
        lea    eax, [ebp - 4]
        mov    edi, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_0EE2
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 14h]
        test   eax, eax
        mov    dword ptr [ecx], eax
        je     SHORT loc_1_0C6E
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0d4h], eax
        jbe    SHORT loc_1_0C63
        and    byte ptr [esi + 0c9h], 0feh
loc_1_0C63:
        push   esi
        call   sub_1_1B18
        add    esp, 4
        jmp    SHORT loc_1_0C82
loc_1_0C6E:
        test   byte ptr [esi + 9ch], 1
        jne    SHORT loc_1_0C82
        test   byte ptr [esi + 0fch], 20h
        je     SHORT loc_1_0C82
        mov    dword ptr [ecx], edi
loc_1_0C82:
        mov    dword ptr [esi + 0ch], 0
        mov    eax, 1
loc_1_0C8E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_0C94:
        push   ebx
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        push   eax
        call   sub_1_280C
        test   eax, eax
        je     SHORT loc_1_0CCF
        mov    ebx, 20h
        mov    esi, 0ffffffffh
loc_1_0CAF:
        test   byte ptr [eax + 11bh], bl
        jne    SHORT loc_1_0CBF
        cmp    dword ptr [eax + 0d4h], esi
        jne    SHORT loc_1_0CE7
loc_1_0CBF:
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_1_27F4
        test   eax, eax
        jne    SHORT loc_1_0CAF
loc_1_0CCF:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_2858
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0CE4:
        pop    esi
        pop    ebx
        ret
loc_1_0CE7:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0CE4
        push   0
        push   64h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_283C
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0CE4
sub_1_0D05:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0ch
        mov    eax, dword ptr [ecx + 58h]
        mov    edx, dword ptr [ecx + 48h]
        push   ebx
        mov    dword ptr [ebp - 8], eax
        push   esi
        mov    dword ptr [ebp - 4], edx
        push   edi
        cmp    eax, edx
        jae    NEAR PTR loc_1_0DA7
        mov    eax, edx
        sub    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp + 10h]
        jae    SHORT loc_1_0D33
        mov    dword ptr [ebp + 10h], eax
loc_1_0D33:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ecx + 60h]
        add    eax, edi
        cmp    eax, dword ptr [ebp - 4]
        jbe    SHORT loc_1_0D4F
        mov    edx, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp + 10h]
        sub    edx, edi
        sub    ebx, edx
        jmp    SHORT loc_1_0D54
loc_1_0D4F:
        mov    edx, dword ptr [ebp + 10h]
        xor    ebx, ebx
loc_1_0D54:
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 44h]
        add    edi, ecx
        mov    dword ptr [ebp - 0ch], ecx
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        test   ebx, ebx
        je     SHORT loc_1_0D8A
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp - 0ch]
        add    esi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
loc_1_0D8A:
        sub    edx, edx
        mov    ecx, dword ptr [ebp + 8]
        div    dword ptr [ebp - 4]
        mov    dword ptr [ecx + 60h], edx
        cli
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 14h]
        add    edx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 58h], edx
        mov    dword ptr [eax], edx
        sti
        jmp    SHORT loc_1_0DAE
loc_1_0DA7:
        mov    dword ptr [ebp + 10h], 0
loc_1_0DAE:
        mov    eax, dword ptr [ebp + 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_0DB8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 58h]
        test   eax, eax
        je     SHORT loc_1_0E3D
        cmp    dword ptr [ebp + 10h], eax
        jbe    SHORT loc_1_0DD0
        mov    dword ptr [ebp + 10h], eax
loc_1_0DD0:
        mov    ecx, dword ptr [ebx + 5ch]
        mov    edx, dword ptr [ebx + 48h]
        mov    eax, dword ptr [ebp + 10h]
        add    eax, ecx
        cmp    eax, edx
        jbe    SHORT loc_1_0DE8
        sub    edx, ecx
        mov    eax, dword ptr [ebp + 10h]
        sub    eax, edx
        jmp    SHORT loc_1_0DED
loc_1_0DE8:
        mov    edx, dword ptr [ebp + 10h]
        xor    eax, eax
loc_1_0DED:
        mov    esi, dword ptr [ebx + 44h]
        mov    edi, dword ptr [ebp + 0ch]
        add    esi, ecx
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        test   eax, eax
        je     SHORT loc_1_0E1D
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebx + 44h]
        add    edi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_0E1D:
        mov    eax, dword ptr [ebx + 5ch]
        sub    edx, edx
        add    eax, dword ptr [ebp + 10h]
        div    dword ptr [ebx + 48h]
        mov    dword ptr [ebx + 5ch], edx
        cli
        mov    ecx, dword ptr [ebx + 58h]
        mov    eax, dword ptr [ebp + 14h]
        sub    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx + 58h], ecx
        mov    dword ptr [eax], ecx
        sti
        jmp    SHORT loc_1_0E44
loc_1_0E3D:
        mov    dword ptr [ebp + 10h], 0
loc_1_0E44:
        mov    eax, dword ptr [ebp + 10h]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 4ch]
        mov    edx, dword ptr [ebx + 40h]
        cmp    eax, edx
        jae    SHORT loc_1_0ED3
        mov    ecx, edx
        sub    ecx, eax
        cmp    ecx, dword ptr [ebp + 10h]
        jae    SHORT loc_1_0E6B
        mov    dword ptr [ebp + 10h], ecx
loc_1_0E6B:
        mov    edi, dword ptr [ebx + 54h]
        mov    eax, dword ptr [ebp + 10h]
        add    eax, edi
        cmp    eax, edx
        jbe    SHORT loc_1_0E80
        sub    edx, edi
        mov    eax, dword ptr [ebp + 10h]
        sub    eax, edx
        jmp    SHORT loc_1_0E85
loc_1_0E80:
        mov    edx, dword ptr [ebp + 10h]
        xor    eax, eax
loc_1_0E85:
        add    edi, dword ptr [ebx + 3ch]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        test   eax, eax
        je     SHORT loc_1_0EB3
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebx + 3ch]
        add    esi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_0EB3:
        mov    eax, dword ptr [ebx + 54h]
        sub    edx, edx
        add    eax, dword ptr [ebp + 10h]
        div    dword ptr [ebx + 40h]
        mov    dword ptr [ebx + 54h], edx
        cli
        mov    ecx, dword ptr [ebx + 4ch]
        mov    edx, dword ptr [ebp + 14h]
        add    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx + 4ch], ecx
        mov    dword ptr [edx], ecx
        sti
        jmp    SHORT loc_1_0EDA
loc_1_0ED3:
        mov    dword ptr [ebp + 10h], 0
loc_1_0EDA:
        mov    eax, dword ptr [ebp + 10h]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_0EE2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 4ch]
        test   eax, eax
        je     SHORT loc_1_0F67
        cmp    eax, dword ptr [ebp + 10h]
        jae    SHORT loc_1_0EFA
        mov    dword ptr [ebp + 10h], eax
loc_1_0EFA:
        mov    ecx, dword ptr [ebx + 50h]
        mov    edx, dword ptr [ebx + 40h]
        mov    eax, dword ptr [ebp + 10h]
        add    eax, ecx
        cmp    eax, edx
        jbe    SHORT loc_1_0F12
        sub    edx, ecx
        mov    eax, dword ptr [ebp + 10h]
        sub    eax, edx
        jmp    SHORT loc_1_0F17
loc_1_0F12:
        mov    edx, dword ptr [ebp + 10h]
        xor    eax, eax
loc_1_0F17:
        mov    esi, dword ptr [ebx + 3ch]
        mov    edi, dword ptr [ebp + 0ch]
        add    esi, ecx
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        test   eax, eax
        je     SHORT loc_1_0F47
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebx + 3ch]
        add    edi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_0F47:
        mov    eax, dword ptr [ebx + 50h]
        sub    edx, edx
        add    eax, dword ptr [ebp + 10h]
        div    dword ptr [ebx + 40h]
        mov    dword ptr [ebx + 50h], edx
        cli
        mov    ecx, dword ptr [ebx + 4ch]
        mov    eax, dword ptr [ebp + 14h]
        sub    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx + 4ch], ecx
        mov    dword ptr [eax], ecx
        sti
        jmp    SHORT loc_1_0F6E
loc_1_0F67:
        mov    dword ptr [ebp + 10h], 0
loc_1_0F6E:
        mov    eax, dword ptr [ebp + 10h]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_0F76:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0c4h]
        lea    edx, [ecx + 2]
        in     al, dx
        test   al, 10h
        je     SHORT loc_1_0F98
        lea    edx, [ecx + 1]
        in     al, dx
        test   al, 40h
        mov    eax, 1
        jne    SHORT loc_1_0F9A
loc_1_0F98:
        xor    eax, eax
loc_1_0F9A:
        pop    ebp
        ret
sub_1_0F9C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, dword ptr [eax + 0c4h]
        add    edx, 2
        pop    ebp
        in     al, dx
        and    al, 0efh
        out    dx, al
        ret
sub_1_0FB2:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, dword ptr [eax + 0c4h]
        add    edx, 2
        pop    ebp
        in     al, dx
        or     al, 10h
        out    dx, al
        ret
sub_1_0FC8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 10h]
        test   byte ptr [esi + 3bh], 1
        jne    SHORT loc_1_103B
        push   esi
        call   sub_1_0F76
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_1022
        push   esi
        call   sub_1_0F9C
        add    esp, 4
        mov    eax, dword ptr [esi + 13eh]
        test   eax, eax
        je     SHORT loc_1_1008
        push   eax
        call   sub_1_2858
        mov    dword ptr [esi + 13eh], 0
loc_1_1008:
        cmp    dword ptr [esi + 142h], 0
        jne    SHORT loc_1_1022
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2868
        mov    dword ptr [esi + 142h], eax
loc_1_1022:
        mov    eax, dword ptr [esi + 117h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        mov    eax, 1
        jmp    SHORT loc_1_103D
loc_1_103B:
        xor    eax, eax
loc_1_103D:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_1042:
        push   ebp
        mov    ebp, esp
        push   esi
        cli
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 13eh], 0
        cmp    dword ptr [esi + 142h], 0
        jne    SHORT loc_1_1085
        cmp    dword ptr [esi + 117h], 0
        je     SHORT loc_1_106F
        push   esi
        call   sub_1_0F9C
        add    esp, 4
loc_1_106F:
        mov    dword ptr [esi + 142h], 0ffffffffh
        sti
        push   esi
        call   sub_1_10E0
        add    esp, 4
        jmp    SHORT loc_1_1086
loc_1_1085:
        sti
loc_1_1086:
        pop    esi
        pop    ebp
        ret
sub_1_1089:
        push   ebp
        mov    ebp, esp
        push   esi
        cli
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 142h], 0
        jne    SHORT loc_1_10DC
        cmp    dword ptr [esi + 117h], 0
        je     SHORT loc_1_10AC
        push   esi
        call   sub_1_0F9C
        add    esp, 4
loc_1_10AC:
        mov    eax, dword ptr [esi + 13eh]
        test   eax, eax
        je     SHORT loc_1_10C6
        push   eax
        call   sub_1_2858
        mov    dword ptr [esi + 13eh], 0
loc_1_10C6:
        mov    dword ptr [esi + 142h], 0ffffffffh
        sti
        push   esi
        call   sub_1_10E0
        add    esp, 4
        jmp    SHORT loc_1_10DD
loc_1_10DC:
        sti
loc_1_10DD:
        pop    esi
        pop    ebp
        ret
sub_1_10E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0f4h]
        lea    ecx, [esi + 146h]
        push   ecx
        mov    edi, dword ptr [eax]
        call   sub_1_1D32
        mov    ebx, eax
        mov    ecx, dword ptr [esi + 0f0h]
        mov    eax, dword ptr [esi + 0f4h]
        and    ecx, dword ptr [eax]
        test   byte ptr [esi + 0c9h], 40h
        mov    dword ptr [eax], ecx
        je     SHORT loc_1_1129
        not    edi
        test   edi, ecx
        je     SHORT loc_1_1129
        push   4
        push   esi
        call   sub_1_1A00
        add    esp, 8
loc_1_1129:
        cli
        test   ebx, ebx
        je     SHORT loc_1_113C
        lea    eax, [esi + 13ah]
        mov    dword ptr [eax], 3e8h
        jmp    SHORT loc_1_1168
loc_1_113C:
        cmp    dword ptr [esi + 117h], 0
        je     SHORT loc_1_115C
        push   esi
        call   sub_1_0FB2
        add    esp, 4
        lea    eax, [esi + 13ah]
        mov    dword ptr [eax], 64h
        jmp    SHORT loc_1_1168
loc_1_115C:
        lea    eax, [esi + 13ah]
        mov    dword ptr [eax], 0ah
loc_1_1168:
        push   esi
        push   dword ptr [eax]
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_283C
        mov    dword ptr [esi + 142h], 0
        mov    dword ptr [esi + 13eh], eax
        sti
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_118B:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ecx, dword ptr [eax + 0f8h]
        push   edi
        mov    dl, byte ptr [ebp + 0ch]
        mov    esi, dword ptr [eax + 0f4h]
        mov    byte ptr [ecx], dl
        mov    cl, dl
        mov    bl, dl
        and    cl, 80h
        mov    edi, 0
        cmp    cl, 1
        mov    ecx, 0
        adc    ecx, -1
        and    bl, 40h
        and    ecx, 1000h
        cmp    bl, 1
        adc    edi, -1
        mov    bl, dl
        and    edi, 100h
        and    bl, 2
        or     ecx, edi
        mov    edi, 0
        cmp    bl, 1
        mov    bl, dl
        adc    edi, -1
        and    bl, 4
        and    edi, 10h
        or     ecx, edi
        mov    edi, 0
        cmp    bl, 1
        mov    bl, dl
        adc    edi, -1
        and    bl, 8
        and    edi, 2000h
        or     ecx, edi
        mov    edi, 0
        cmp    bl, 1
        mov    bl, dl
        adc    edi, -1
        and    bl, 1
        and    edi, 20h
        or     ecx, edi
        mov    edi, 0
        cmp    bl, 1
        mov    bl, dl
        adc    edi, -1
        and    bl, 20h
        and    edi, 8
        or     ecx, edi
        mov    edi, 0
        cmp    bl, 1
        mov    bl, dl
        adc    edi, -1
        and    bl, 10h
        and    edi, 800h
        or     ecx, edi
        mov    edi, 0
        cmp    bl, 1
        adc    edi, -1
        and    edi, 400h
        or     ecx, edi
        or     dword ptr [esi], ecx
        mov    cl, byte ptr [eax + 102h]
        test   cl, cl
        je     SHORT loc_1_127F
        and    dl, cl
        cmp    dl, cl
        jne    SHORT loc_1_1276
        and    byte ptr [eax + 100h], 5fh
        jmp    SHORT loc_1_127F
loc_1_1276:
        or     byte ptr [eax + 100h], 20h
        jmp    SHORT loc_1_128C
loc_1_127F:
        cmp    dword ptr [eax + 58h], 0
        jbe    SHORT loc_1_128C
        or     byte ptr [eax + 177h], 2
loc_1_128C:
        push   eax
        call   sub_1_27EC
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_129C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [eax + 0f4h]
        mov    bl, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [edx]
        or     cl, 80h
        test   bl, 0eh
        mov    dword ptr [edx], ecx
        je     SHORT loc_1_12BF
        mov    byte ptr [eax + 106h], bl
loc_1_12BF:
        test   bl, 10h
        je     SHORT loc_1_12C9
        or     cl, 40h
        mov    dword ptr [edx], ecx
loc_1_12C9:
        cmp    dword ptr [eax + 58h], 0
        jbe    SHORT loc_1_12D6
        or     byte ptr [eax + 177h], 2
loc_1_12D6:
        push   eax
        call   sub_1_27EC
        add    esp, 4
        pop    ebx
        pop    ebp
        ret    8
loc_1_12E4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        call   sub_1_2824
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 12eh]
        and    byte ptr [esi + 0c9h], 7fh
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [esi + 106h]
        mov    byte ptr [ebp - 1], al
        test   al, al
        je     SHORT loc_1_1327
        mov    byte ptr [esi + 106h], 0
        mov    al, byte ptr [esi + 0fdh]
        and    al, byte ptr [ebp - 1]
        movzx  ecx, al
        or     dword ptr [esi + 34h], ecx
loc_1_1327:
        mov    eax, dword ptr [esi + 0f4h]
        mov    dword ptr [ebp - 8], 0
        or     byte ptr [eax], 1
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_1550
loc_1_1341:
        mov    eax, dword ptr [esi + 4ch]
        cmp    dword ptr [esi + 40h], eax
        jbe    NEAR PTR loc_1_1550
        mov    dl, byte ptr [esi + 0fch]
        test   dl, 20h
        jne    NEAR PTR loc_1_1550
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [ebp - 8]
        inc    dword ptr [ebp + 0ch]
        dec    dword ptr [ebp + 10h]
        test   byte ptr [ebp - 1], 4
        mov    cl, byte ptr [eax]
        je     NEAR PTR loc_1_145B
        test   byte ptr [esi + 9dh], 4
        je     NEAR PTR loc_1_145B
        mov    cl, byte ptr [esi + 0afh]
loc_1_1387:
        movsx  eax, byte ptr [esi + 0b1h]
        movzx  edx, cl
        cmp    eax, edx
        jne    SHORT loc_1_139E
        mov    eax, dword ptr [esi + 0f4h]
        or     byte ptr [eax], 2
loc_1_139E:
        test   byte ptr [esi + 11bh], 1
        jne    NEAR PTR loc_1_1546
        mov    edx, dword ptr [esi + 3ch]
        mov    eax, dword ptr [esi + 54h]
        mov    byte ptr [edx + eax], cl
        mov    eax, dword ptr [esi + 54h]
        inc    eax
        mov    dword ptr [esi + 54h], eax
        cmp    dword ptr [esi + 40h], eax
        ja     SHORT loc_1_13C7
        mov    dword ptr [esi + 54h], 0
loc_1_13C7:
        mov    edi, dword ptr [esi + 4ch]
        inc    edi
        mov    dword ptr [esi + 4ch], edi
        cmp    dword ptr [esi + 0ech], edi
        ja     SHORT loc_1_1432
        mov    dl, byte ptr [esi + 100h]
        test   dl, 12h
        jne    SHORT loc_1_1432
        mov    cl, byte ptr [esi + 101h]
        test   cl, cl
        je     SHORT loc_1_141B
        movzx  eax, byte ptr [esi + 17ah]
        movzx  ebx, cl
        not    ebx
        and    ebx, eax
        cmp    ebx, eax
        je     SHORT loc_1_1412
        not    cl
        or     byte ptr [esi + 181h], 1
        and    cl, byte ptr [esi + 17ah]
        mov    byte ptr [esi + 17ah], cl
loc_1_1412:
        or     dl, 2
        mov    byte ptr [esi + 100h], dl
loc_1_141B:
        test   byte ptr [esi + 9dh], 2
        je     SHORT loc_1_1432
        or     byte ptr [esi + 100h], 1
        or     byte ptr [esi + 177h], 2
loc_1_1432:
        cmp    dword ptr [esi + 0d4h], edi
        ja     NEAR PTR loc_1_153F
        test   byte ptr [esi + 0c9h], 1
        jne    NEAR PTR loc_1_1546
        push   1
        push   esi
        call   sub_1_1A00
        add    esp, 8
        jmp    NEAR PTR loc_1_1546
loc_1_145B:
        and    cl, byte ptr [esi + 0feh]
        mov    edi, dword ptr [esi + 9ch]
        test   edi, 800h
        je     SHORT loc_1_1477
        test   cl, cl
        je     NEAR PTR loc_1_1546
loc_1_1477:
        test   edi, 1
        jne    SHORT loc_1_149B
        movsx  eax, byte ptr [esi + 0b0h]
        movzx  ebx, cl
        cmp    eax, ebx
        jne    SHORT loc_1_149B
        or     dl, 20h
        mov    byte ptr [esi + 0fch], dl
        jmp    NEAR PTR loc_1_1546
loc_1_149B:
        mov    eax, edi
        and    eax, 100h
        je     SHORT loc_1_14F8
        movsx  edx, byte ptr [esi + 0aeh]
        movzx  ebx, cl
        cmp    edx, ebx
        jne    SHORT loc_1_14F8
        mov    cl, byte ptr [esi + 100h]
        or     cl, 8
        test   edi, 60h
        mov    byte ptr [esi + 100h], cl
        je     SHORT loc_1_1546
        mov    eax, dword ptr [esi + 4ch]
        cmp    dword ptr [esi + 0a0h], eax
        jb     SHORT loc_1_1546
        or     byte ptr [esi + 177h], 2
        and    cl, 0f7h
        mov    byte ptr [esi + 100h], cl
        and    cl, 0efh
        mov    byte ptr [esi + 100h], cl
        or     cl, 4
        mov    byte ptr [esi + 100h], cl
        jmp    SHORT loc_1_1546
loc_1_14F8:
        test   eax, eax
        je     NEAR PTR loc_1_1387
        movsx  eax, byte ptr [esi + 0adh]
        movzx  edx, cl
        cmp    eax, edx
        jne    NEAR PTR loc_1_1387
        mov    al, byte ptr [esi + 100h]
        and    al, 0f7h
        test   edi, 60h
        mov    byte ptr [esi + 100h], al
        je     SHORT loc_1_1530
        and    al, 0f7h
        mov    byte ptr [esi + 100h], al
loc_1_1530:
        cmp    dword ptr [esi + 58h], 0
        jbe    SHORT loc_1_1546
        or     byte ptr [esi + 177h], 2
        jmp    SHORT loc_1_1546
loc_1_153F:
        and    byte ptr [esi + 0c9h], 0feh
loc_1_1546:
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_1_1341
loc_1_1550:
        cmp    dword ptr [esi + 58h], 0
        jbe    SHORT loc_1_155D
        or     byte ptr [esi + 177h], 2
loc_1_155D:
        push   esi
        call   sub_1_27ED
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1572:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    al, byte ptr [esi + 100h]
        test   al, 60h
        jne    NEAR PTR loc_1_16FF
        mov    ecx, dword ptr [esi + 9ch]
        mov    dword ptr [ebp - 0ch], ecx
        and    ecx, 60h
        mov    dword ptr [ebp - 10h], ecx
        cmp    ecx, edi
        jne    SHORT loc_1_15F9
        mov    ecx, dword ptr [ebp + 10h]
        cmp    ecx, edi
        je     SHORT loc_1_15C8
        test   al, 4
        je     SHORT loc_1_15C8
        and    al, 0ebh
        mov    dl, byte ptr [esi + 0adh]
        mov    byte ptr [esi + 100h], al
        mov    eax, dword ptr [ebp + 0ch]
        dec    ecx
        mov    edi, 1
        mov    byte ptr [eax], dl
        inc    eax
        jmp    SHORT loc_1_15CB
loc_1_15C8:
        mov    eax, dword ptr [ebp + 0ch]
loc_1_15CB:
        test   ecx, ecx
        je     SHORT loc_1_15FF
        mov    dl, byte ptr [esi + 100h]
        test   dl, 1
        je     SHORT loc_1_15FF
        and    dl, 0feh
        mov    bl, byte ptr [esi + 0aeh]
        dec    ecx
        inc    edi
        mov    byte ptr [esi + 100h], dl
        mov    byte ptr [eax], bl
        or     dl, 10h
        inc    eax
        mov    byte ptr [esi + 100h], dl
        jmp    SHORT loc_1_15FF
loc_1_15F9:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
loc_1_15FF:
        mov    dl, byte ptr [esi + 100h]
        test   dl, 6dh
        mov    byte ptr [ebp - 8], dl
        jne    NEAR PTR loc_1_16FF
        test   ecx, ecx
        je     SHORT loc_1_1634
        mov    dl, byte ptr [esi + 0fch]
        test   dl, 40h
        je     SHORT loc_1_1634
        and    dl, 0bfh
        mov    bl, byte ptr [esi + 0ffh]
        dec    ecx
        inc    edi
        mov    byte ptr [esi + 0fch], dl
        mov    byte ptr [eax], bl
        inc    eax
loc_1_1634:
        mov    edx, dword ptr [esi + 58h]
        test   edx, edx
        jne    SHORT loc_1_1644
        mov    ebx, dword ptr [esi + 0f4h]
        or     byte ptr [ebx], 4
loc_1_1644:
        test   ecx, ecx
        je     NEAR PTR loc_1_16FF
        test   edx, edx
        je     NEAR PTR loc_1_16FF
        test   byte ptr [esi + 0fch], 12h
        jne    SHORT loc_1_1676
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_1676
        lea    edx, [ebp - 4]
        push   edx
        push   ecx
        push   eax
        push   esi
        call   sub_1_0DB8
        add    esp, 10h
        add    edi, eax
        jmp    SHORT loc_1_16D6
loc_1_1676:
        test   byte ptr [ebp - 0ch], 40h
        je     SHORT loc_1_16AB
        mov    ecx, dword ptr [esi + 107h]
        cmp    dword ptr [esi + 0a0h], ecx
        jne    SHORT loc_1_16AB
        mov    cl, byte ptr [ebp - 8]
        mov    dl, byte ptr [esi + 0aeh]
        or     cl, 8
        inc    edi
        mov    byte ptr [esi + 100h], cl
        mov    byte ptr [eax], dl
        mov    dword ptr [esi + 107h], 0
        jmp    SHORT loc_1_16D0
loc_1_16AB:
        mov    ebx, dword ptr [esi + 5ch]
        mov    ecx, dword ptr [esi + 44h]
        inc    dword ptr [esi + 107h]
        mov    cl, byte ptr [ecx + ebx]
        inc    ebx
        mov    byte ptr [eax], cl
        mov    dword ptr [esi + 5ch], ebx
        cmp    dword ptr [esi + 48h], ebx
        ja     SHORT loc_1_16CC
        mov    dword ptr [esi + 5ch], 0
loc_1_16CC:
        dec    edx
        mov    dword ptr [esi + 58h], edx
loc_1_16D0:
        mov    eax, dword ptr [esi + 58h]
        mov    dword ptr [ebp - 4], eax
loc_1_16D6:
        mov    eax, dword ptr [esi + 0f4h]
        or     byte ptr [eax + 1], 40h
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0e0h], ecx
        jbe    SHORT loc_1_16FF
        test   byte ptr [esi + 0c9h], 2
        jne    SHORT loc_1_16FF
        push   2
        push   esi
        call   sub_1_1A00
        add    esp, 8
loc_1_16FF:
        test   edi, edi
        jne    SHORT loc_1_170A
        and    byte ptr [esi + 177h], 0fdh
loc_1_170A:
        push   esi
        call   sub_1_27EE
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_171E:
        push   ebp
        mov    cl, byte ptr [esp + 0ch]
        movzx  eax, cl
        mov    ebp, esp
        and    al, 0bfh
        cmp    eax, 0afh
        je     SHORT loc_1_1738
        cmp    eax, 0bfh
        jne    SHORT loc_1_1744
loc_1_1738:
        push   dword ptr [ebp + 10h]
        push   ecx
        push   dword ptr [ebp + 8]
        call   sub_1_27EF
loc_1_1744:
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_1_174A:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 126h]
        test   ecx, ecx
        je     SHORT loc_1_1783
        mov    eax, dword ptr [esi + 12ah]
        test   eax, eax
        je     SHORT loc_1_1783
        push   1
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        push   1
        call   ecx
        add    esp, 14h
        mov    dword ptr [esi + 10bh], eax
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        jmp    SHORT loc_1_1788
loc_1_1783:
        mov    eax, 1
loc_1_1788:
        pop    esi
        pop    ebp
        ret
sub_1_178B:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 10bh], 0
        je     SHORT loc_1_17AE
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [eax + 12ah]
        push   3
        call   dword ptr [eax + 126h]
loc_1_17AE:
        mov    esp, ebp
        pop    ebp
        ret
sub_1_17B2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    bl, byte ptr [esi + 3bh]
        test   bl, 1
        je     SHORT loc_1_17E8
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [esi + 12ah]
        push   2
        call   dword ptr [esi + 126h]
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_17E4
        and    bl, 0feh
        mov    byte ptr [esi + 3bh], bl
        jmp    SHORT loc_1_17E8
loc_1_17E4:
        xor    eax, eax
        jmp    SHORT loc_1_17ED
loc_1_17E8:
        mov    eax, 1
loc_1_17ED:
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_17F1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        cli
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 142h]
        test   eax, eax
        je     SHORT loc_1_1816
        push   eax
        call   sub_1_2880
        mov    dword ptr [esi + 142h], 0
loc_1_1816:
        mov    eax, dword ptr [esi + 13eh]
        test   eax, eax
        je     SHORT loc_1_1830
        push   eax
        call   sub_1_2858
        mov    dword ptr [esi + 13eh], 0
loc_1_1830:
        test   byte ptr [esi + 3bh], 1
        je     SHORT loc_1_1843
        push   esi
        call   sub_1_17B2
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_1855
loc_1_1843:
        push   esi
        call   sub_1_18AF
        add    esp, 4
        push   esi
        call   sub_1_0F9C
        add    esp, 4
loc_1_1855:
        mov    eax, dword ptr [esi + 117h]
        test   eax, eax
        je     SHORT loc_1_1875
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 011h, 000h, 003h, 000h
        mov    dword ptr [esi + 117h], 0
loc_1_1875:
        sti
        lea    eax, [esi + 146h]
        push   eax
        call   sub_1_1CAD
        push   esi
        call   sub_1_178B
        add    esp, 4
        mov    eax, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1895:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_1_18A9
        or     byte ptr [eax + 3bh], 1
        jmp    SHORT loc_1_18AD
loc_1_18A9:
        and    byte ptr [eax + 3bh], 0feh
loc_1_18AD:
        pop    ebp
        ret
sub_1_18AF:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0ch
        mov    ecx, dword ptr [eax + 117h]
        push   ebx
        test   ecx, ecx
        je     SHORT loc_1_18F3
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], ecx
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ebx, dword ptr [ebp - 8]
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        xchg   ecx, eax
        test   al, 4
        jne    SHORT loc_1_18F3
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        mov    eax, 1
        jmp    SHORT loc_1_18F5
loc_1_18F3:
        xor    eax, eax
loc_1_18F5:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_18FA:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [eax + 117h]
        test   eax, eax
        je     SHORT loc_1_191A
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_1_191A:
        mov    esp, ebp
        pop    ebp
        ret
sub_1_191E:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 3bh], 1
        je     SHORT loc_1_194B
        push   esi
        call   sub_1_17B2
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_194B
        xor    eax, eax
        mov    ecx, dword ptr [esi + 5ch]
        mov    dword ptr [esi + 58h], eax
        mov    dword ptr [esi + 107h], eax
        mov    dword ptr [esi + 60h], ecx
        jmp    SHORT loc_1_1966
loc_1_194B:
        mov    al, byte ptr [esi + 177h]
        test   al, 2
        jne    SHORT loc_1_1966
        or     al, 2
        push   esi
        mov    byte ptr [esi + 177h], al
        call   sub_1_1089
        add    esp, 4
loc_1_1966:
        pop    esi
        pop    ebp
        ret
sub_1_1969:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        mov    byte ptr [ebp - 1], al
        push   esi
        mov    dword ptr [ebp - 8], eax
        mov    esi, dword ptr [ebp + 8]
loc_1_197C:
        mov    dword ptr [ebp - 0ch], 0bh
loc_1_1983:
        mov    eax, dword ptr [esi + 0f8h]
        mov    bl, byte ptr [eax]
        not    bl
        and    bl, byte ptr [esi + 103h]
        je     SHORT loc_1_19EC
        cli
        push   esi
        call   sub_1_1089
        add    esp, 4
        sti
        test   bl, 80h
        je     SHORT loc_1_19B4
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 0b4h], eax
        ja     SHORT loc_1_19B4
        or     byte ptr [ebp - 1], 80h
loc_1_19B4:
        test   bl, 20h
        je     SHORT loc_1_19C8
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 0bch], eax
        ja     SHORT loc_1_19C8
        or     byte ptr [ebp - 1], 40h
loc_1_19C8:
        test   bl, 10h
        je     SHORT loc_1_19DC
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 0b8h], eax
        ja     SHORT loc_1_19DC
        or     byte ptr [ebp - 1], 20h
loc_1_19DC:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_19F0
        dec    dword ptr [ebp - 0ch]
        jne    SHORT loc_1_1983
        inc    dword ptr [ebp - 8]
        jmp    SHORT loc_1_197C
loc_1_19EC:
        xor    al, al
        jmp    SHORT loc_1_19FA
loc_1_19F0:
        movzx  eax, byte ptr [ebp - 1]
        or     dword ptr [esi + 34h], eax
        mov    al, byte ptr [ebp - 1]
loc_1_19FA:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_1A00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pushad
        movzx  edx, byte ptr [ebp + 0ch]
        cmp    edx, 1
        je     SHORT loc_1_1A1C
        cmp    edx, 2
        je     SHORT loc_1_1A3E
        cmp    edx, 4
        je     SHORT loc_1_1A60
        jmp    SHORT loc_1_1A83
loc_1_1A1C:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0f4h]
        or     byte ptr [ecx + 0c9h], 1
        push   dword ptr [eax]
        push   edx
        push   dword ptr [ecx + 0dch]
        push   ecx
        call   dword ptr [ecx + 0d8h]
        jmp    SHORT loc_1_1A80
loc_1_1A3E:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0f4h]
        or     byte ptr [ecx + 0c9h], 2
        push   dword ptr [eax]
        push   edx
        push   dword ptr [ecx + 0e8h]
        push   ecx
        call   dword ptr [ecx + 0e4h]
        jmp    SHORT loc_1_1A80
loc_1_1A60:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0f4h]
        or     byte ptr [ecx + 0c9h], 4
        push   dword ptr [eax]
        push   edx
        push   dword ptr [ecx + 0d0h]
        push   ecx
        call   dword ptr [ecx + 0cch]
loc_1_1A80:
        add    esp, 10h
loc_1_1A83:
        popad
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_1A89:
        push   ebx
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        xor    esi, esi
        push   eax
        db  089h, 035h, 000h, 000h, 000h, 000h
        call   sub_1_280C
        cmp    eax, esi
        mov    ebx, eax
        je     SHORT loc_1_1B01
        mov    edi, 0ffffffffh
loc_1_1AAA:
        test   byte ptr [ebx + 11bh], 20h
        jne    SHORT loc_1_1AEF
        cmp    dword ptr [ebx + 0d4h], edi
        je     SHORT loc_1_1AEF
        cmp    dword ptr [ebx + 4ch], esi
        je     SHORT loc_1_1AEF
        mov    eax, dword ptr [ebx + 0c8h]
        test   ah, 1
        jne    SHORT loc_1_1AEF
        test   ah, 80h
        je     SHORT loc_1_1AE6
        and    ah, 7fh
        push   1
        push   ebx
        mov    dword ptr [ebx + 0c8h], eax
        call   sub_1_1A00
        add    esp, 8
        jmp    SHORT loc_1_1AEF
loc_1_1AE6:
        or     ah, 80h
        mov    dword ptr [ebx + 0c8h], eax
loc_1_1AEF:
        push   ebx
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_27F4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_1AAA
loc_1_1B01:
        push   0
        push   64h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_283C
        pop    edi
        db  0A3h, 000h, 000h, 000h, 000h
        pop    esi
        pop    ebx
        ret
sub_1_1B18:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [edx + 9ch]
        and    esi, 60h
        je     SHORT loc_1_1B37
        lea    ecx, [edx + 100h]
        test   byte ptr [ecx], 12h
        jne    SHORT loc_1_1B46
loc_1_1B37:
        test   esi, esi
        jne    SHORT loc_1_1B92
        lea    ecx, [edx + 100h]
        test   byte ptr [ecx], 12h
        je     SHORT loc_1_1B92
loc_1_1B46:
        mov    eax, dword ptr [edx + 4ch]
        cmp    dword ptr [edx + 0a0h], eax
        jb     SHORT loc_1_1B92
        cli
        mov    al, byte ptr [edx + 101h]
        test   al, al
        je     SHORT loc_1_1B78
        mov    bl, byte ptr [edx + 17ah]
        or     al, bl
        cmp    al, bl
        je     SHORT loc_1_1B75
        or     byte ptr [edx + 181h], 1
        mov    byte ptr [edx + 17ah], al
loc_1_1B75:
        and    byte ptr [ecx], 0fdh
loc_1_1B78:
        mov    al, byte ptr [ecx]
        test   esi, esi
        test   al, 10h
        je     SHORT loc_1_1B88
        and    al, 0efh
        mov    byte ptr [ecx], al
        or     al, 4
        mov    byte ptr [ecx], al
loc_1_1B88:
        push   edx
        call   sub_1_191E
        add    esp, 4
        sti
loc_1_1B92:
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_1B96:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_280C
        test   eax, eax
        je     SHORT loc_1_1BB5
loc_1_1BA5:
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_1_27F4
        test   eax, eax
        jne    SHORT loc_1_1BA5
loc_1_1BB5:
        mov    eax, 1
        ret    4
sub_1_1BBD:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   68ch
        call   sub_1_23B6
        push   29h
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_1_23B6
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, 6bh
        mov    edi, esi
        db  0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        rep stosd
        stosw
        lea    eax, [esi + 0d0h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp + 18h]
        mov    dword ptr [esi + 0cbh], eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [esi], ecx
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [esi + 4], edx
        mov    edx, dword ptr [ebp + 24h]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [ebp + 28h]
        mov    dword ptr [esi + 0ch], ecx
        mov    dword ptr [esi + 10h], edx
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    dword ptr [esi + 18h], eax
        call   sub_1_2419
        mov    dword ptr [esi + 1ch], eax
        test   eax, eax
        jne    SHORT loc_1_1C36
        mov    eax, 1005h
        jmp    SHORT loc_1_1CA7
loc_1_1C36:
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 8
        jne    SHORT loc_1_1C47
        mov    dword ptr [esi + 1ch], 8
        jmp    SHORT loc_1_1C53
loc_1_1C47:
        cmp    eax, 4
        jne    SHORT loc_1_1C53
        mov    dword ptr [esi + 1ch], 4
loc_1_1C53:
        cmp    dword ptr [esi + 1ch], 8
        jne    SHORT loc_1_1C65
        db  0C7h, 086h, 0C2h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_1C6F
loc_1_1C65:
        db  0C7h, 086h, 0C2h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1C6F:
        db  0C7h, 046h, 037h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 24h], 0ffffffffh
        mov    byte ptr [esi + 16eh], 40h
        mov    dword ptr [esi + 0c6h], 0
        mov    dword ptr [esi + 3bh], 5c3ah
        push   esi
        call   sub_1_232D
        push   esi
        call   sub_1_2306
        push   esi
        call   sub_1_1D32
loc_1_1CA7:
        pop    edi
        pop    esi
        pop    ebp
        ret    24h
sub_1_1CAD:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
loc_1_1CB4:
        or     byte ptr [esi + 3dh], 1
        push   esi
        call   sub_1_1D32
        test   eax, eax
        je     SHORT loc_1_1CB4
        pop    esi
        pop    ebp
        ret    4
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cli
        mov    edx, dword ptr [ebp + 8]
        test   byte ptr [edx + 3dh], 2
        je     SHORT loc_1_1CDD
        mov    eax, 1007h
        jmp    SHORT loc_1_1CFF
loc_1_1CDD:
        mov    al, byte ptr [ebp + 0ch]
        lea    edi, [edx + 83h]
        mov    esi, dword ptr [ebp + 10h]
        mov    ecx, 0fh
        mov    byte ptr [edx + 82h], al
        rep movsd
        movsw
        movsb
        or     byte ptr [edx + 3dh], 2
        xor    eax, eax
loc_1_1CFF:
        sti
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
sub_1_1D06:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    al, byte ptr [ecx + 36h]
        mov    dl, al
        pop    ebp
        and    dl, 0f0h
        mov    byte ptr [ecx + 36h], dl
        ret    4
sub_1_1D1C:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    al, byte ptr [ecx + 35h]
        mov    dl, al
        pop    ebp
        and    dl, 0e1h
        mov    byte ptr [ecx + 35h], dl
        ret    4
sub_1_1D32:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 24h]
        inc    eax
        mov    dword ptr [edi + 24h], eax
        jne    NEAR PTR loc_1_1E7B
        push   edi
        call   sub_1_1E92
loc_1_1D52:
        test   dword ptr [edi + 3bh], 7fbfh
        je     SHORT loc_1_1D6C
        cli
        mov    edx, dword ptr [edi + 18h]
        add    edx, 2
        in     al, dx
        and    al, 0f7h
        out    dx, al
        sti
        mov    ebx, 1
loc_1_1D6C:
        mov    edx, dword ptr [edi + 18h]
        inc    edx
        in     al, dx
        test   al, 40h
        je     SHORT loc_1_1DA8
        push   edi
        call   sub_1_1F17
        mov    eax, dword ptr [edi + 20h]
        test   eax, eax
        jne    SHORT loc_1_1D8D
        mov    dword ptr [edi + 2ch], 0
        xor    esi, esi
        jmp    SHORT loc_1_1DA8
loc_1_1D8D:
        cmp    eax, 1001h
        jne    NEAR PTR loc_1_1E7B
        mov    eax, dword ptr [edi + 2ch]
        inc    eax
        mov    dword ptr [edi + 2ch], eax
        cmp    eax, 10h
        jae    NEAR PTR loc_1_1E74
loc_1_1DA8:
        test   byte ptr [edi + 31h], 2
        je     SHORT loc_1_1DD6
        test   byte ptr [edi + 3ch], 80h
        jne    SHORT loc_1_1DD6
        test   byte ptr [edi + 41h], 2
        jne    SHORT loc_1_1DD6
        movzx  eax, byte ptr [edi + 0cah]
        push   eax
        lea    ecx, [edi + 43h]
        push   ecx
        push   dword ptr [edi]
        call   dword ptr [edi + 8]
        test   eax, eax
        je     SHORT loc_1_1DD6
        or     byte ptr [edi + 3ch], 80h
        mov    byte ptr [edi + 42h], al
loc_1_1DD6:
        cmp    dword ptr [edi + 3bh], 0
        je     SHORT loc_1_1DF7
        push   edi
        call   sub_1_21CA
        mov    eax, dword ptr [edi + 20h]
        test   eax, eax
        jne    SHORT loc_1_1E43
        mov    dword ptr [edi + 2ch], 0
        mov    dword ptr [edi + 28h], 0
loc_1_1DF7:
        mov    edx, dword ptr [edi + 18h]
        inc    edx
        in     al, dx
        test   al, 40h
        jne    NEAR PTR loc_1_1D52
        mov    eax, dword ptr [edi + 3bh]
        test   eax, 0ffff7fffh
        jne    NEAR PTR loc_1_1D52
        test   ah, 80h
        je     SHORT loc_1_1E21
        test   byte ptr [edi + 41h], 2
        je     NEAR PTR loc_1_1D52
loc_1_1E21:
        test   byte ptr [edi + 41h], 1
        jne    SHORT loc_1_1E7B
        test   byte ptr [edi + 16eh], 40h
        jne    SHORT loc_1_1E7B
        test   ebx, ebx
        jne    SHORT loc_1_1E7B
        inc    esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        jae    NEAR PTR loc_1_1D52
        jmp    SHORT loc_1_1E7B
loc_1_1E43:
        cmp    eax, 1
        je     SHORT loc_1_1E88
        cmp    eax, 1001h
        jne    SHORT loc_1_1E5D
        mov    eax, dword ptr [edi + 2ch]
        inc    eax
        mov    dword ptr [edi + 2ch], eax
        cmp    eax, 10h
        jb     SHORT loc_1_1DF7
        jmp    SHORT loc_1_1E74
loc_1_1E5D:
        cmp    eax, 1003h
        jne    SHORT loc_1_1E7B
        mov    eax, dword ptr [edi + 28h]
        inc    eax
        mov    dword ptr [edi + 28h], eax
        cmp    eax, 20h
        jb     NEAR PTR loc_1_1D52
loc_1_1E74:
        mov    dword ptr [edi + 20h], 1004h
loc_1_1E7B:
        mov    eax, dword ptr [edi + 20h]
        dec    dword ptr [edi + 24h]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1E88:
        xor    eax, eax
        mov    dword ptr [edi + 2ch], eax
        mov    dword ptr [edi + 28h], eax
        jmp    SHORT loc_1_1E7B
sub_1_1E92:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 31h], 8
        je     SHORT loc_1_1EB1
        test   byte ptr [esi + 36h], 0fh
        je     SHORT loc_1_1EB1
        push   esi
        call   sub_1_1D06
        push   eax
        push   dword ptr [esi]
        call   dword ptr [esi + 10h]
loc_1_1EB1:
        test   byte ptr [esi + 31h], 4
        je     SHORT loc_1_1EC9
        test   byte ptr [esi + 35h], 1eh
        je     SHORT loc_1_1EC9
        push   esi
        call   sub_1_1D1C
        push   eax
        push   dword ptr [esi]
        call   dword ptr [esi + 0ch]
loc_1_1EC9:
        test   byte ptr [esi + 31h], 1
        je     SHORT loc_1_1F12
        mov    al, byte ptr [esi + 0cfh]
        test   al, al
        je     SHORT loc_1_1F12
        movzx  eax, al
        push   eax
        push   dword ptr [esi + 0cbh]
        push   dword ptr [esi]
        call   dword ptr [esi + 4]
        mov    cl, byte ptr [esi + 0cfh]
        add    dword ptr [esi + 0cbh], eax
        sub    cl, al
        mov    byte ptr [esi + 0cfh], cl
        test   cl, cl
        jne    SHORT loc_1_1F12
        lea    eax, [esi + 0d0h]
        push   esi
        mov    dword ptr [esi + 0cbh], eax
        call   sub_1_232D
loc_1_1F12:
        pop    esi
        pop    ebp
        ret    4
sub_1_1F17:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        lea    esi, [ebx + 16fh]
        lea    edi, [ebx + 16eh]
        push   esi
        push   edi
        push   dword ptr [ebx + 18h]
        call   dword ptr [ebx + 0c2h]
        mov    dword ptr [ebx + 20h], eax
        test   eax, eax
        jne    NEAR PTR loc_1_2154
        mov    cl, byte ptr [edi]
        test   cl, 80h
        jne    SHORT loc_1_1FCA
        movzx  eax, cl
        test   byte ptr [ebx + 31h], 1
        db  00Fh, 0B6h, 0B8h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_1F6E
        cmp    byte ptr [ebx + 0cfh], 0
        jne    SHORT loc_1_1F6E
        push   edi
        push   esi
        push   dword ptr [ebx]
        call   dword ptr [ebx + 4]
        jmp    SHORT loc_1_1F70
loc_1_1F6E:
        xor    eax, eax
loc_1_1F70:
        sub    edi, eax
        mov    dword ptr [ebp - 4], edi
        je     NEAR PTR loc_1_2154
        mov    edi, dword ptr [ebx + 0cbh]
        mov    ecx, ebx
        movzx  esi, byte ptr [ebx + 0cfh]
        sub    ecx, edi
        sub    ecx, esi
        add    ecx, 14eh
        cmp    ecx, dword ptr [ebp - 4]
        jae    SHORT loc_1_1F9C
        mov    dword ptr [ebp - 4], ecx
loc_1_1F9C:
        mov    edx, dword ptr [ebp - 4]
        add    edi, esi
        lea    esi, [ebx + eax + 16fh]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        push   ebx
        and    ecx, 3
        rep movsb
        mov    dl, byte ptr [ebp - 4]
        add    byte ptr [ebx + 0cfh], dl
        call   sub_1_234B
        jmp    NEAR PTR loc_1_2154
loc_1_1FCA:
        movzx  edx, cl
        mov    eax, edx
        and    al, 0bfh
        sub    eax, 81h
        cmp    eax, 30h
        ja     SHORT loc_1_1FE9
        db  00Fh, 0B6h, 080h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_1FE9:
        mov    edx, dword ptr [ebx + 14h]
        test   edx, edx
        je     NEAR PTR loc_1_2154
        push   esi
        push   ecx
        push   dword ptr [ebx]
        call   edx
        jmp    NEAR PTR loc_1_2154
loc_1_1FFF:
        or     byte ptr [ebx + 3bh], 1
        jmp    NEAR PTR loc_1_2154
loc_1_2008:
        test   byte ptr [ebx + 3bh], 1
        jne    SHORT loc_1_2013
        mov    al, byte ptr [esi]
        mov    byte ptr [ebx + 34h], al
loc_1_2013:
        or     byte ptr [ebx + 0c6h], 1
        jmp    NEAR PTR loc_1_2154
loc_1_201F:
        or     byte ptr [ebx + 3bh], 4
        jmp    NEAR PTR loc_1_2154
loc_1_2028:
        test   byte ptr [ebx + 3bh], 4
        jne    SHORT loc_1_2033
        mov    al, byte ptr [esi]
        mov    byte ptr [ebx + 33h], al
loc_1_2033:
        or     byte ptr [ebx + 0c6h], 2
        jmp    NEAR PTR loc_1_2154
loc_1_203F:
        mov    cl, byte ptr [ebx + 36h]
        mov    al, byte ptr [esi]
        and    cl, 0fh
        mov    dl, al
        and    dl, 0fbh
        and    al, 44h
        or     dl, cl
        mov    byte ptr [ebx + 36h], cl
        or     byte ptr [ebx + 0c6h], 4
        mov    byte ptr [ebx + 36h], dl
        cmp    al, 4
        jne    SHORT loc_1_2067
        or     dl, 4
        mov    byte ptr [ebx + 36h], dl
loc_1_2067:
        test   byte ptr [ebx + 31h], 8
        je     NEAR PTR loc_1_2154
        test   byte ptr [ebx + 36h], 0fh
        je     NEAR PTR loc_1_2154
        push   ebx
        call   sub_1_1D06
        push   eax
        push   dword ptr [ebx]
        call   dword ptr [ebx + 10h]
        jmp    NEAR PTR loc_1_2154
loc_1_208C:
        mov    cl, byte ptr [ebx + 35h]
        mov    al, byte ptr [esi]
        or     byte ptr [ebx + 0c6h], 8
        and    cl, 1eh
        or     al, cl
        mov    byte ptr [ebx + 35h], cl
        test   byte ptr [ebx + 31h], 4
        mov    byte ptr [ebx + 35h], al
        je     NEAR PTR loc_1_2154
        test   al, 1eh
        je     NEAR PTR loc_1_2154
        push   ebx
        call   sub_1_1D1C
        push   eax
        push   dword ptr [ebx]
        call   dword ptr [ebx + 0ch]
        jmp    NEAR PTR loc_1_2154
loc_1_20C6:
        or     byte ptr [ebx + 0c6h], 10h
        jmp    NEAR PTR loc_1_2154
loc_1_20D2:
        push   ebx
        call   sub_1_2306
        jmp    SHORT loc_1_2154
loc_1_20DA:
        push   ebx
        call   sub_1_22DF
        jmp    SHORT loc_1_2154
loc_1_20E2:
        test   byte ptr [ebx + 3ch], 2
        jne    SHORT loc_1_20F0
        mov    al, byte ptr [esi]
        mov    byte ptr [ebx + 0cah], al
loc_1_20F0:
        or     byte ptr [ebx + 0c6h], 80h
        jmp    SHORT loc_1_2154
loc_1_20F9:
        or     byte ptr [ebx + 3ch], 20h
        jmp    SHORT loc_1_2154
loc_1_20FF:
        push   ebx
        call   sub_1_2368
        or     byte ptr [ebx + 0c7h], 4
        jmp    SHORT loc_1_2154
loc_1_210E:
        db  00Fh, 0B6h, 082h, 000h, 000h, 000h, 000h
        lea    edi, [ebx + 14eh]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        or     byte ptr [ebx + 0c7h], 1
        jmp    SHORT loc_1_2154
loc_1_2132:
        db  00Fh, 0B6h, 082h, 000h, 000h, 000h, 000h
        lea    edi, [ebx + 15eh]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        or     byte ptr [ebx + 0c7h], 2
loc_1_2154:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 00Eh, 002h
        db  003h, 00Eh, 004h, 00Eh, 005h, 00Eh, 006h, 007h, 008h, 009h, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh
        db  00Eh, 00Ah, 00Bh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh
        db  00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Ch, 00Dh
sub_1_21CA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 3bh]
        test   eax, 7fbfh
        je     SHORT loc_1_221D
        db  0BFh, 000h, 000h, 000h, 000h
loc_1_21E2:
        test   dword ptr [edi], eax
        jne    SHORT loc_1_21F6
        add    edi, 9
        db  081h, 0FFh, 07Eh, 000h, 000h, 000h
        jb     SHORT loc_1_21E2
        jmp    NEAR PTR loc_1_22D8
loc_1_21F6:
        mov    eax, dword ptr [edi + 4]
        mov    cl, byte ptr [edi + 8]
        add    eax, esi
        push   eax
        push   ecx
        push   dword ptr [esi + 18h]
        call   dword ptr [esi + 37h]
        mov    dword ptr [esi + 20h], eax
        test   eax, eax
        jne    NEAR PTR loc_1_22D8
        mov    eax, dword ptr [edi]
        not    eax
        and    dword ptr [esi + 3bh], eax
        jmp    NEAR PTR loc_1_22D8
loc_1_221D:
        test   ah, 80h
        je     SHORT loc_1_2269
        test   byte ptr [esi + 41h], 2
        jne    SHORT loc_1_2269
        lea    edi, [esi + 43h]
        mov    al, byte ptr [esi + 42h]
        push   edi
        push   eax
        push   dword ptr [esi + 18h]
        call   dword ptr [esi + 37h]
        mov    dword ptr [esi + 20h], eax
        test   eax, eax
        jne    NEAR PTR loc_1_22D8
        test   byte ptr [esi + 31h], 2
        mov    eax, 0
        je     SHORT loc_1_225A
        movzx  eax, byte ptr [esi + 0cah]
        push   eax
        push   edi
        push   dword ptr [esi]
        call   dword ptr [esi + 8]
loc_1_225A:
        test   eax, eax
        je     SHORT loc_1_2263
        mov    byte ptr [esi + 42h], al
        jmp    SHORT loc_1_22D8
loc_1_2263:
        and    byte ptr [esi + 3ch], 7fh
        jmp    SHORT loc_1_22D8
loc_1_2269:
        test   eax, 20000h
        je     SHORT loc_1_2291
        lea    eax, [esi + 83h]
        mov    cl, byte ptr [esi + 82h]
        push   eax
        push   ecx
        push   dword ptr [esi + 18h]
        call   dword ptr [esi + 37h]
        mov    dword ptr [esi + 20h], eax
        test   eax, eax
        jne    SHORT loc_1_22D8
        and    byte ptr [esi + 3dh], 0fdh
        jmp    SHORT loc_1_22D8
loc_1_2291:
        test   eax, 10040h
        je     SHORT loc_1_22D8
        db  0BFh, 000h, 000h, 000h, 000h
loc_1_229D:
        test   dword ptr [edi], eax
        jne    SHORT loc_1_22AE
        add    edi, 9
        db  081h, 0FFh, 012h, 000h, 000h, 000h
        jb     SHORT loc_1_229D
        jmp    SHORT loc_1_22D8
loc_1_22AE:
        mov    bl, byte ptr [edi + 8]
        mov    eax, dword ptr [edi + 4]
        add    eax, esi
        push   eax
        push   ebx
        push   dword ptr [esi + 18h]
        call   dword ptr [esi + 37h]
        mov    dword ptr [esi + 20h], eax
        test   eax, eax
        jne    SHORT loc_1_22D8
        mov    eax, dword ptr [edi]
        not    eax
        and    dword ptr [esi + 3bh], eax
        cmp    bl, 98h
        jne    SHORT loc_1_22D8
        mov    dword ptr [esi + 20h], 1
loc_1_22D8:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_1_22DF:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0c6h]
        or     cl, 40h
        pop    ebp
        and    byte ptr [eax + 41h], 0fdh
        mov    dword ptr [eax + 0c6h], ecx
        and    cl, 0dfh
        mov    dword ptr [eax + 0c6h], ecx
        ret    4
sub_1_2306:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0c6h]
        or     cl, 20h
        pop    ebp
        or     byte ptr [eax + 41h], 2
        mov    dword ptr [eax + 0c6h], ecx
        and    cl, 0bfh
        mov    dword ptr [eax + 0c6h], ecx
        ret    4
sub_1_232D:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 3bh]
        or     ch, 1
        pop    ebp
        and    byte ptr [eax + 41h], 0feh
        mov    dword ptr [eax + 3bh], ecx
        and    cl, 7fh
        mov    dword ptr [eax + 3bh], ecx
        ret    4
sub_1_234B:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 3bh]
        or     al, 80h
        pop    ebp
        or     byte ptr [ecx + 41h], 1
        mov    dword ptr [ecx + 3bh], eax
        and    ah, 0feh
        mov    dword ptr [ecx + 3bh], eax
        ret    4
sub_1_2368:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 41h]
        mov    cl, byte ptr [esi + 16fh]
        xor    cl, al
        test   cl, 1
        je     SHORT loc_1_2390
        push   esi
        test   al, 1
        je     SHORT loc_1_238B
        call   sub_1_234B
        jmp    SHORT loc_1_2390
loc_1_238B:
        call   sub_1_232D
loc_1_2390:
        mov    al, byte ptr [esi + 16fh]
        mov    cl, byte ptr [esi + 41h]
        xor    cl, al
        test   cl, 2
        je     SHORT loc_1_23B1
        push   esi
        test   al, 2
        je     SHORT loc_1_23AC
        call   sub_1_2306
        jmp    SHORT loc_1_23B1
loc_1_23AC:
        call   sub_1_22DF
loc_1_23B1:
        pop    esi
        pop    ebp
        ret    4
sub_1_23B6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        cli
        mov    dx, 61h
        in     al, dx
        mov    bl, al
        and    al, 0feh
        out    dx, al
        mov    al, 0b0h
        out    43h, al
        mov    eax, dword ptr [ebp + 8]
        out    42h, al
        mov    al, ah
        out    42h, al
        mov    ecx, 7fffh
        in     al, dx
        or     al, 1
        out    dx, al
loc_1_23DE:
        in     al, dx
        test   al, 20h
        loope  loc_1_23DE
        mov    al, bl
        out    dx, al
        mov    eax, 7fffh
        sub    eax, ecx
        mov    dword ptr [ebp - 4], eax
        sti
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_23FB:
        push   ebp
        mov    cl, 0ffh
        mov    ebp, esp
        push   ebx
        xor    bl, bl
        mov    edx, dword ptr [ebp + 8]
loc_1_2406:
        mov    al, cl
        out    dx, al
        in     al, dx
        xor    al, cl
        or     bl, al
        dec    cl
        jne    SHORT loc_1_2406
        mov    al, bl
        pop    ebx
        pop    ebp
        ret    4
sub_1_2419:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        lea    esi, [edi + 2]
        mov    edx, esi
        in     al, dx
        mov    bl, al
        lea    edx, [edi + 3]
        in     al, dx
        and    al, 0feh
        push   edi
        out    dx, al
        mov    edx, esi
        in     al, dx
        and    al, 0dfh
        out    dx, al
        call   sub_1_23FB
        test   al, al
        je     SHORT loc_1_2445
        xor    ecx, ecx
        jmp    SHORT loc_1_2463
loc_1_2445:
        mov    edx, esi
        push   edi
        in     al, dx
        or     al, 20h
        out    dx, al
        call   sub_1_23FB
        sub    al, 0ffh
        mov    edx, esi
        cmp    al, 1
        in     al, dx
        sbb    ecx, ecx
        and    al, 0dfh
        out    dx, al
        and    ecx, 4
        add    ecx, 4
loc_1_2463:
        mov    edx, esi
        mov    al, bl
        out    dx, al
        mov    eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2471:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cld
        cli
        mov    bx, ds
        mov    es, bx
        xor    ebx, ebx
        mov    edx, dword ptr [ebp + 8]
        mov    bl, byte ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 10h]
        db  08Ah, 0BBh, 000h, 000h, 000h, 000h
        or     bh, bh
        mov    edi, 1002h
        je     NEAR PTR loc_1_255C
        add    dx, 2
        in     al, dx
        and    al, 10h
        mov    ah, al
        and    ah, 0f7h
        mov    edi, 1003h
        add    dx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_24BB:
        in     al, dx
        test   al, 80h
        loopne loc_1_24BB
        je     NEAR PTR loc_1_255C
        test   al, 40h
        jne    NEAR PTR loc_1_255C
        sub    dx, 1
        add    edx, 2
        mov    al, ah
        out    dx, al
        add    edx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_24E1:
        in     al, dx
        test   al, 80h
        loopne loc_1_24E1
        je     SHORT loc_1_255C
        test   al, 40h
        jne    SHORT loc_1_255C
        mov    edi, 1001h
        add    edx, -1
        mov    al, bl
        out    dx, al
        add    edx, 2
        mov    al, ah
        or     al, 1
        out    dx, al
        and    al, 0feh
        out    dx, al
        add    edx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_250B:
        in     al, dx
        test   al, 80h
        loopne loc_1_250B
        jne    SHORT loc_1_255C
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_2518:
        in     al, dx
        test   al, 80h
        loope  loc_1_2518
        je     SHORT loc_1_255C
loc_1_251F:
        add    edx, -1
        outsb
        add    edx, 2
        mov    al, ah
        or     al, 1
        out    dx, al
        add    edx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_2534:
        in     al, dx
        test   al, 80h
        loopne loc_1_2534
        jne    SHORT loc_1_255C
        add    edx, 1
        mov    al, ah
        and    al, 0feh
        out    dx, al
        add    edx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_254C:
        in     al, dx
        test   al, 80h
        loope  loc_1_254C
        je     SHORT loc_1_255C
        dec    bh
        jne    SHORT loc_1_251F
        mov    edi, 0
loc_1_255C:
        mov    edx, dword ptr [ebp + 8]
        add    dx, 2
        mov    al, ah
        or     al, 8
        out    dx, al
        sti
        mov    dword ptr [ebp - 4], edi
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_2578:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cld
        cli
        mov    bx, ds
        mov    es, bx
        xor    ebx, ebx
        mov    edx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        add    dx, 2
        in     al, dx
        mov    bh, al
        and    bh, 8
        and    al, 10h
        mov    ah, al
        or     ah, 8
        mov    esi, 1001h
        mov    al, ah
        or     al, 1
        out    dx, al
        and    al, 0feh
        out    dx, al
        add    edx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_25B8:
        in     al, dx
        test   al, 80h
        loope  loc_1_25B8
        je     NEAR PTR loc_1_26D7
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_25C9:
        in     al, dx
        test   al, 80h
        loopne loc_1_25C9
        jne    NEAR PTR loc_1_26D7
        in     al, dx
        shl    al, 1
        and    al, 0f0h
        mov    bl, al
        add    edx, 1
        mov    al, ah
        or     al, 1
        out    dx, al
        add    edx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_25EC:
        in     al, dx
        test   al, 80h
        loope  loc_1_25EC
        je     NEAR PTR loc_1_26D7
        add    edx, 1
        mov    al, ah
        and    al, 0feh
        out    dx, al
        add    edx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_2608:
        in     al, dx
        test   al, 80h
        loopne loc_1_2608
        jne    NEAR PTR loc_1_26D7
        in     al, dx
        shr    al, 3
        and    al, 0fh
        or     bl, al
        add    edx, 1
        mov    al, ah
        or     al, 1
        out    dx, al
        add    edx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_262C:
        in     al, dx
        test   al, 80h
        loope  loc_1_262C
        je     NEAR PTR loc_1_26D7
        add    edx, 1
        mov    al, ah
        and    al, 0feh
        out    dx, al
        mov    byte ptr [edi], bl
        and    ah, 0f7h
        or     ah, bh
        xor    bh, bh
        db  08Ah, 0BBh, 000h, 000h, 000h, 000h
        or     bh, bh
        jne    SHORT loc_1_2659
        mov    esi, 1002h
        jmp    SHORT loc_1_26D7
loc_1_2659:
        mov    edi, dword ptr [ebp + 10h]
loc_1_265C:
        add    edx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_2665:
        in     al, dx
        test   al, 80h
        loopne loc_1_2665
        jne    SHORT loc_1_26D7
        in     al, dx
        shl    al, 1
        and    al, 0f0h
        mov    bl, al
        add    edx, 1
        mov    al, ah
        or     al, 1
        out    dx, al
        add    edx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_2684:
        in     al, dx
        test   al, 80h
        loope  loc_1_2684
        je     SHORT loc_1_26D7
        add    edx, 1
        mov    al, ah
        and    al, 0feh
        out    dx, al
        add    edx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_269C:
        in     al, dx
        test   al, 80h
        loopne loc_1_269C
        jne    SHORT loc_1_26D7
        in     al, dx
        shr    al, 3
        and    al, 0fh
        or     bl, al
        add    edx, 1
        mov    al, ah
        or     al, 1
        out    dx, al
        add    edx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_26BC:
        in     al, dx
        test   al, 80h
        loope  loc_1_26BC
        je     SHORT loc_1_26D7
        add    edx, 1
        mov    al, ah
        and    al, 0feh
        out    dx, al
        mov    al, bl
        stosb
        dec    bh
        jne    SHORT loc_1_265C
        mov    esi, 0
loc_1_26D7:
        mov    edx, dword ptr [ebp + 8]
        add    dx, 2
        mov    al, ah
        out    dx, al
        sti
        mov    dword ptr [ebp - 4], esi
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_26F1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cld
        cli
        mov    bx, ds
        mov    es, bx
        xor    ebx, ebx
        mov    edx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        add    dx, 2
        in     al, dx
        mov    bh, al
        and    bh, 8
        and    al, 10h
        mov    ah, al
        or     ah, 2ch
        mov    esi, 1001h
        mov    al, ah
        or     al, 1
        out    dx, al
        and    al, 0feh
        out    dx, al
        add    edx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_2731:
        in     al, dx
        test   al, 80h
        loope  loc_1_2731
        je     NEAR PTR loc_1_27CD
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_2742:
        in     al, dx
        test   al, 80h
        loopne loc_1_2742
        jne    NEAR PTR loc_1_27CD
        add    edx, -1
        in     al, dx
        mov    bl, al
        add    edx, 2
        mov    al, ah
        or     al, 1
        out    dx, al
        add    edx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_2764:
        in     al, dx
        test   al, 80h
        loope  loc_1_2764
        je     SHORT loc_1_27CD
        add    edx, 1
        mov    al, ah
        and    al, 0feh
        out    dx, al
        mov    byte ptr [edi], bl
        and    ah, 0f7h
        or     ah, bh
        xor    bh, bh
        db  08Ah, 0BBh, 000h, 000h, 000h, 000h
        or     bh, bh
        jne    SHORT loc_1_278D
        mov    esi, 1002h
        jmp    SHORT loc_1_27CD
loc_1_278D:
        mov    edi, dword ptr [ebp + 10h]
loc_1_2790:
        add    edx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_2799:
        in     al, dx
        test   al, 80h
        loopne loc_1_2799
        jne    SHORT loc_1_27CD
        add    edx, -1
        insb
        add    edx, 2
        mov    al, ah
        or     al, 1
        out    dx, al
        add    edx, -1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_1_27B5:
        in     al, dx
        test   al, 80h
        loope  loc_1_27B5
        je     SHORT loc_1_27CD
        add    edx, 1
        mov    al, ah
        and    al, 0feh
        out    dx, al
        dec    bh
        jne    SHORT loc_1_2790
        mov    esi, 0
loc_1_27CD:
        mov    edx, dword ptr [ebp + 8]
        add    dx, 2
        mov    al, ah
        out    dx, al
        sti
        mov    dword ptr [ebp - 4], esi
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_1_27E7:
        ret
sub_1_27E8:
        ret
sub_1_27E9:
        ret
sub_1_27EA:
        ret
sub_1_27EB:
        ret
sub_1_27EC:
        ret
sub_1_27ED:
        ret
sub_1_27EE:
        ret
sub_1_27EF:
        ret
        db  0C3h
sub_1_27F1:
        ret
        db  0CCh, 0CCh
sub_1_27F4:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CDh, 020h, 0F3h, 080h, 001h, 000h, 0CCh, 0CCh
sub_1_280C:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_1_281C:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_1_2824:
        db  0CDh, 020h, 0CFh, 080h, 001h, 000h
        int3
        int3
sub_1_282C:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_1_2834:
        db  0CDh, 020h, 050h, 080h, 001h, 000h
        int3
        int3
sub_1_283C:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  0CCh, 0CCh, 0CCh
sub_1_2858:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
        db  0CCh
sub_1_2868:
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    8
        db  0CCh, 0CCh, 0CCh
sub_1_2880:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        pop    esi
        ret    4
        db  0CCh
_LTEXT ends
        end
