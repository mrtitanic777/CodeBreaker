; drvspacx_pcode_07.asm -- DRVSPACX object 7 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_7_0000:
        dec    ebp
        push   ebx
        inc    esp
        inc    edx
        dec    esp
        db  036h, 02Eh, 030h
loc_7_0008:
        dec    ebp
        push   ebx
        inc    esp
        push   ebx
        push   eax
        xor    byte ptr cs:[ebx], dh
        db  0C0h, 0B9h, 044h, 00Eh, 000h, 000h, 08Bh
        or     ebx, esp
        pop    ds
        cmp    byte ptr [ecx + 8ah], bl
        db  075h, 0F4h
        movzx  edx, byte ptr [ecx + 88h]
        cmp    edx, ebx
        jne    SHORT loc_7_0035
        movzx  edx, byte ptr [ecx + 89h]
loc_7_0035:
        bts    eax, edx
        db  0EBh, 0DDh
        db  0C3h
loc_7_003B:
        mov    ebp, edx
        mov    dword ptr [ebp + 48h], 0ffffff44h
        mov    dword ptr [ebp + 23ch], esp
        lea    eax, [ebp + 0b6h]
        mov    dword ptr [ebp + 0e0h], eax
        mov    dword ptr [ebp + 0dch], 400h
        btr    dword ptr [ebp + 4], 0dh
        bts    dword ptr [ebp + 4], 0eh
        or     dword ptr [ebp + 58h], 0ffffffffh
        or     dword ptr [ebp + 64h], 0ffffffffh
        call   sub_7_106E
        call   sub_7_1CEA
        btr    dword ptr [ebp + 4], 0eh
        mov    ebx, 0ffffff44h
        call   sub_7_4344
        push   ebp
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
        ret
loc_7_0096:
        db  0E8h, 0F6h, 0FFh, 0F8h, 0FFh
        jb     SHORT loc_7_00F0
        btr    dword ptr [ebp + 4], 0dh
        db  0BEh, 03Bh, 000h, 000h, 000h
        mov    edx, ebp
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 408h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    ecx
        pop    ebx
        pop    eax
loc_7_00BE:
        cli
        bt     dword ptr [ebp + 4], 0dh
        jb     SHORT loc_7_00D9
        mov    eax, 5
        push   eax
        push   ebp
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        lea    esp, [esp + 8]
        jmp    SHORT loc_7_00BE
loc_7_00D9:
        db  0E8h, 057h, 000h, 0F9h, 0FFh
        xor    eax, eax
        xchg   word ptr [ebp + 0b6h], ax
        mov    edx, 0dh
        cmp    eax, 10h
        cmc
loc_7_00F0:
        ret
sub_7_00F1:
        push   edi
        push   esi
        lea    esi, [esi + 3]
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 8
        cld
        repe cmpsb
        pop    esi
        pop    edi
        jne    SHORT loc_7_0123
        movzx  ecx, word ptr [esi + 33h]
        cmp    ecx, 1
        je     SHORT loc_7_0122
        cmp    ecx, 2
        je     SHORT loc_7_0122
        cmp    ecx, 100h
        jne    SHORT loc_7_0122
        mov    ecx, 3
loc_7_0122:
        ret
loc_7_0123:
        push   edi
        push   esi
        lea    esi, [esi + 3]
        db  0BFh, 008h, 000h, 000h, 000h
        mov    ecx, 8
        cld
        repe cmpsb
        pop    esi
        pop    edi
        jne    SHORT loc_7_0122
        movzx  ecx, word ptr [esi + 33h]
        cmp    ecx, 1
        je     SHORT loc_7_0122
        cmp    ecx, 0
        jmp    SHORT loc_7_0122
sub_7_0147:
        sub    esp, 28h
        call   sub_7_00F1
        jne    NEAR PTR loc_7_02AD
        mov    eax, edi
        call   sub_7_02C9
        jb     NEAR PTR loc_7_02B4
        cmp    byte ptr [eax + 3ch], 9
        jne    NEAR PTR loc_7_02BB
        cmp    byte ptr [esi + 0dh], 10h
        je     SHORT loc_7_0188
        cmp    word ptr [esi + 33h], 100h
        jb     NEAR PTR loc_7_02C2
        cmp    byte ptr [esi + 0dh], 40h
        jne    NEAR PTR loc_7_02C2
loc_7_0188:
        cmp    word ptr [esi + 0eh], 0
        je     NEAR PTR loc_7_02AD
        cmp    byte ptr [esi + 10h], 1
        jne    NEAR PTR loc_7_02AD
        cmp    byte ptr [esi + 15h], 0f8h
        jne    NEAR PTR loc_7_02AD
        cmp    word ptr [esi + 16h], 0
        je     NEAR PTR loc_7_02AD
        cmp    dword ptr [esi + 1ch], 0
        jne    NEAR PTR loc_7_02AD
        cmp    dword ptr [esi + 20h], 1
        sbb    eax, eax
        cmp    word ptr [esi + 13h], 1
        adc    eax, 0
        je     NEAR PTR loc_7_02AD
        cmp    byte ptr [esi + 26h], 9
        jne    NEAR PTR loc_7_02BB
        movzx  eax, word ptr [esi + 0bh]
        bsf    edx, eax
        je     NEAR PTR loc_7_02BB
        btr    eax, edx
        or     eax, eax
        jne    NEAR PTR loc_7_02BB
        bts    eax, edx
        cmp    dl, 9
        jne    NEAR PTR loc_7_02BB
        movzx  edx, word ptr [esi + 11h]
        shl    edx, 5
        lea    edx, [eax + edx - 1]
        shr    edx, 9
        movzx  ecx, word ptr [esi + 16h]
        movzx  eax, word ptr [esi + 0eh]
        add    ecx, eax
        cmp    word ptr [esi + 29h], cx
        jne    NEAR PTR loc_7_02AD
        add    edx, ecx
        cmp    word ptr [esi + 2bh], dx
        jne    NEAR PTR loc_7_02AD
        movzx  edx, word ptr [esi + 27h]
        shl    edx, 9
        mov    eax, 0d602h
        mov    ecx, 24h
        mov    dword ptr [esp + 24h], esi
        lea    esi, [esp]
        cmp    ebx, -1
        jne    SHORT loc_7_0250
        call   sub_7_3692
        jmp    SHORT loc_7_0256
loc_7_0250:
        db  0CDh, 020h, 032h, 000h, 040h, 000h
loc_7_0256:
        mov    esi, dword ptr [esp + 24h]
        jb     SHORT loc_7_02AD
        lea    edx, [esp]
        cmp    byte ptr [edx], 0e9h
        je     SHORT loc_7_026F
        cmp    byte ptr [edx], 0ebh
        jne    SHORT loc_7_02AD
        cmp    byte ptr [edx + 2], 90h
        jne    SHORT loc_7_02AD
loc_7_026F:
        cmp    byte ptr [edx + 10h], 2
        jne    SHORT loc_7_02AD
        movzx  eax, word ptr [edx + 0bh]
        cmp    word ptr [esi + 0bh], ax
        jne    SHORT loc_7_02BB
        mov    al, byte ptr [edx + 0dh]
        cmp    byte ptr [esi + 0dh], al
        jne    SHORT loc_7_02C2
        movzx  eax, word ptr [edx + 0eh]
        cmp    word ptr [esi + 0eh], ax
        jne    SHORT loc_7_02AD
        movzx  eax, word ptr [edx + 11h]
        cmp    word ptr [esi + 11h], ax
        jne    SHORT loc_7_02AD
        movzx  eax, word ptr [edx + 16h]
        cmp    word ptr [esi + 16h], ax
        jne    SHORT loc_7_02AD
loc_7_02A5:
        lea    esp, [esp + 28h]
        ret
loc_7_02AA:
        stc
        jmp    SHORT loc_7_02A5
loc_7_02AD:
        mov    edx, 0dh
        jmp    SHORT loc_7_02AA
loc_7_02B4:
        mov    edx, 5
        jmp    SHORT loc_7_02AA
loc_7_02BB:
        mov    edx, 7
        jmp    SHORT loc_7_02AA
loc_7_02C2:
        mov    edx, 6
        jmp    SHORT loc_7_02AA
sub_7_02C9:
        sub    esp, 0ch
        mov    dword ptr [esp + 8], eax
        mov    eax, esp
        mov    word ptr [eax], 7
        push   eax
        db  0FFh, 015h, 058h, 009h, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        cmp    word ptr [esp + 6], 1
        cmc
        lea    esp, [esp + 10h]
        ret
sub_7_02EE:
        cmp    ebx, -1
        je     SHORT loc_7_0312
        mov    eax, 0d800h
        db  0CDh, 020h, 032h, 000h, 040h, 000h
        jb     NEAR PTR loc_7_04BF
        mov    edx, eax
        shr    edx, 9
        je     NEAR PTR loc_7_04C0
        mov    dword ptr [ebp + 50h], edx
loc_7_0312:
        movsx  eax, word ptr [esi + 33h]
        cmp    eax, 100h
        jne    SHORT loc_7_0322
        mov    eax, 3
loc_7_0322:
        mov    word ptr [ebp + 0b4h], ax
        db  080h, 03Dh, 05Eh, 00Eh, 000h, 000h, 000h
        je     SHORT loc_7_0344
        cmp    eax, -1
        je     SHORT loc_7_0344
        db  00Fh, 0B6h, 080h, 05Fh, 00Eh, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 042h, 000h
loc_7_0344:
        lea    edi, [ebp + 8eh]
        mov    ecx, 24h
        cld
        rep movsb
        sub    esi, 24h
        movsx  eax, word ptr [esi + 2dh]
        mov    dword ptr [ebp + 78h], eax
        movzx  edx, word ptr [esi + 2bh]
        mov    dword ptr [ebp + 6ch], edx
        movzx  eax, word ptr [esi + 27h]
        mov    dword ptr [ebp + 4ch], eax
        lea    eax, [eax + edx + 3]
        neg    eax
        add    eax, dword ptr [ebp + 50h]
        jae    NEAR PTR loc_7_04C0
        mov    dword ptr [ebp + 54h], eax
        mov    dword ptr [ebp + 0f8h], eax
        movzx  ecx, word ptr [ebp + 0a4h]
        add    edx, ecx
        mov    dword ptr [ebp + 74h], edx
        movzx  eax, word ptr [ebp + 0a1h]
        or     eax, eax
        jne    SHORT loc_7_039F
        mov    eax, dword ptr [ebp + 0aeh]
loc_7_039F:
        add    eax, ecx
        mov    dword ptr [ebp + 0aeh], eax
        mov    dword ptr [ebp + 0f4h], eax
        sub    eax, edx
        mov    cl, byte ptr [esi + 32h]
        mov    byte ptr [ebp + 8ch], cl
        shr    eax, cl
        cmp    eax, 0ff7h
        sbb    cl, cl
        and    cl, 2
        or     byte ptr [ebp + 4], cl
        add    eax, 2
        add    eax, dword ptr [ebp + 78h]
        mov    dword ptr [ebp + 7ch], eax
        movzx  eax, word ptr [esi + 24h]
        mov    dword ptr [ebp + 68h], eax
        mov    eax, dword ptr [ebp + 54h]
        dec    eax
        shr    eax, 0eh
        mov    dword ptr [ebp + 84h], eax
        cmp    eax, 100h
        jae    NEAR PTR loc_7_04C0
        cmp    word ptr [ebp + 0b4h], 3
        jae    SHORT loc_7_040E
        cmp    eax, 0ffh
        ja     NEAR PTR loc_7_04C0
        cmp    byte ptr [esi + 2fh], al
        ja     SHORT loc_7_044A
        jmp    NEAR PTR loc_7_04C0
loc_7_040E:
        cmp    word ptr [esi + 2fh], ax
        jbe    NEAR PTR loc_7_04C0
        mov    edx, dword ptr [esi + 44h]
        or     edx, edx
        je     NEAR PTR loc_7_04C0
        mov    eax, dword ptr [ebp + 54h]
        mov    cl, byte ptr [ebp + 8ch]
        shr    eax, cl
        mov    ecx, edx
        mul    dword ptr [esi + 40h]
        cmp    edx, ecx
        jae    NEAR PTR loc_7_04C0
        mov    eax, dword ptr [esi + 40h]
        mov    dword ptr [ebp + 0f8h], ecx
        mov    dword ptr [ebp + 0f4h], eax
loc_7_044A:
        movzx  eax, word ptr [esi + 29h]
        movzx  edx, word ptr [ebp + 0a4h]
        add    eax, edx
        mov    dword ptr [ebp + 70h], eax
        mov    al, byte ptr [esi + 15h]
        mov    byte ptr [ebp + 8dh], al
        or     dword ptr [ebp + 64h], 0ffffffffh
        or     dword ptr [ebp + 58h], 0ffffffffh
        mov    eax, dword ptr [ebp + 4ch]
        add    eax, dword ptr [ebp + 74h]
        movzx  edx, word ptr [ebp + 0a4h]
        sub    eax, edx
        mov    dword ptr [ebp + 60h], eax
        mov    eax, dword ptr [ebp + 54h]
        db  0F7h, 025h, 034h, 00Eh, 000h, 000h
        mov    ecx, 64h
        div    ecx
        mov    dword ptr [ebp + 5ch], eax
        cmp    ebx, -1
        je     SHORT loc_7_04BF
        push   esi
        sub    esp, 0ch
        mov    esi, esp
        mov    dword ptr [esi], 1
        mov    eax, dword ptr [ebp + 0bch]
        mov    dword ptr [esi + 8], eax
        mov    eax, 0df12h
        db  0CDh, 020h, 032h, 000h, 040h, 000h
        lea    esp, [esp + 0ch]
        pop    esi
        mov    edx, 8
loc_7_04BF:
        ret
loc_7_04C0:
        stc
        mov    edx, 0dh
        jmp    SHORT loc_7_04BF
sub_7_04C8:
        xor    eax, eax
sub_7_04CA:
        push   ebx
        sub    esp, 0ch
        mov    dword ptr [esp + 8], eax
        xor    edx, edx
        mov    dword ptr [esp + 4], edx
        mov    dword ptr [esp], edx
        dec    eax
        js     SHORT loc_7_0505
        sub    eax, dword ptr [ebp + 60h]
        jb     SHORT loc_7_0505
        add    eax, 0fh
        and    eax, 0fffffff0h
        cmp    dword ptr [ebp + 54h], eax
        jbe    NEAR PTR loc_7_05B0
        shld   edx, eax, 12h
        and    eax, 3fffh
        shr    eax, 3
        mov    dword ptr [esp + 4], edx
        mov    dword ptr [esp], eax
loc_7_0505:
        test   byte ptr [ebp + 5], 20h
        je     NEAR PTR loc_7_05B7
loc_7_050F:
        mov    ecx, dword ptr [ebp + 18h]
        cmp    dword ptr [ebp + 58h], -1
        je     SHORT loc_7_0521
        cmp    dword ptr [ebp + 58h], ecx
        jb     NEAR PTR loc_7_05B1
loc_7_0521:
        cmp    byte ptr [ebp + 0f2h], cl
        jbe    SHORT loc_7_059B
        mov    edx, dword ptr [ebp + 84h]
        inc    edx
        mov    eax, dword ptr [ebp + 80h]
loc_7_0536:
        cmp    dword ptr [esp + 4], eax
        ja     SHORT loc_7_0571
        push   ecx
        push   edx
        call   sub_7_101D
        pop    edx
        pop    ecx
        jb     SHORT loc_7_05B1
        mov    edi, dword ptr [ebp + 0d8h]
        mov    esi, dword ptr [ebp + 80h]
        cmp    dword ptr [esp + 4], esi
        mov    esi, dword ptr [esp]
        je     SHORT loc_7_055E
        xor    esi, esi
loc_7_055E:
        add    esi, edi
        add    edi, 800h
        push   ecx
        push   edx
        call   sub_7_079B
        pop    edx
        pop    ecx
        jae    SHORT loc_7_05B1
loc_7_0571:
        mov    eax, dword ptr [ebp + 80h]
        cmp    eax, dword ptr [ebp + 84h]
        cmc
        sbb    edi, edi
        or     eax, edi
        inc    eax
        dec    edx
        jne    SHORT loc_7_0536
        cmp    byte ptr [ebp + 0f2h], cl
        jb     SHORT loc_7_059B
        cmp    dword ptr [esp + 8], 0
        jne    SHORT loc_7_059B
        mov    byte ptr [ebp + 0f2h], cl
loc_7_059B:
        cmp    ecx, 1
        je     SHORT loc_7_05B0
        test   byte ptr [ebp + 6], 1
        jne    SHORT loc_7_05B0
        cmp    word ptr [ebp + 0b4h], 3
        jae    SHORT loc_7_05C6
loc_7_05B0:
        stc
loc_7_05B1:
        lea    esp, [esp + 0ch]
        pop    ebx
        ret
loc_7_05B7:
        push   ebx
        call   sub_7_106E
        pop    ebx
        jae    NEAR PTR loc_7_050F
        jmp    SHORT loc_7_05B0
loc_7_05C6:
        push   ecx
        mov    eax, dword ptr [ebp + 80h]
        call   sub_7_101D
        pop    ecx
        jb     SHORT loc_7_05B1
        test   byte ptr [ebp + 5], 1
        jne    SHORT loc_7_05E9
        lea    ecx, [ecx*4 + 207h]
        add    ecx, dword ptr [ebp + 1ch]
        shr    ecx, 9
        dec    ecx
loc_7_05E9:
        inc    ecx
        cmp    dword ptr [ebp + 58h], -1
        je     SHORT loc_7_05F5
        cmp    dword ptr [ebp + 58h], ecx
        jb     SHORT loc_7_05B1
loc_7_05F5:
        lea    ebx, [ebp + 0fch]
        and    dword ptr [ebx], 0
        mov    esi, dword ptr [ebp + 0d8h]
        call   sub_7_0A86
        or     ecx, ecx
        je     SHORT loc_7_0655
        mov    edx, dword ptr [ebp + 84h]
        or     edx, edx
        je     SHORT loc_7_05B0
loc_7_0617:
        mov    eax, dword ptr [ebp + 80h]
        or     eax, eax
        jne    SHORT loc_7_0628
        mov    eax, dword ptr [ebp + 84h]
        inc    eax
loc_7_0628:
        dec    eax
        push   ecx
        push   edx
        call   sub_7_101D
        pop    edx
        pop    ecx
        jb     NEAR PTR loc_7_05B1
        push   edx
        lea    ebx, [ebp + 0fch]
        mov    esi, dword ptr [ebp + 0d8h]
        call   sub_7_0A86
        pop    edx
        jecxz  loc_7_0655
        dec    edx
        jne    SHORT loc_7_0617
        jmp    NEAR PTR loc_7_05B0
loc_7_0655:
        mov    ecx, dword ptr [ebp + 0fch]
        dec    ecx
        mov    edx, 1
loc_7_0661:
        mov    eax, dword ptr [ebp + edx*4 + 100h]
        lea    esi, [edx - 1]
        rol    eax, 8
loc_7_066E:
        mov    ebx, dword ptr [ebp + esi*4 + 100h]
        rol    ebx, 8
        cmp    al, bl
        jbe    SHORT loc_7_0689
        ror    ebx, 8
        mov    dword ptr [ebp + esi*4 + 104h], ebx
        dec    esi
        jns    SHORT loc_7_066E
loc_7_0689:
        ror    eax, 8
        mov    dword ptr [ebp + esi*4 + 104h], eax
        inc    edx
        loop   loc_7_0661
        mov    ecx, dword ptr [ebp + 18h]
        test   byte ptr [ebp + 5], 1
        jne    SHORT loc_7_06AD
        lea    ecx, [ecx*4 + 207h]
        add    ecx, dword ptr [ebp + 1ch]
        shr    ecx, 9
        dec    ecx
loc_7_06AD:
        inc    ecx
        xor    edx, edx
loc_7_06B0:
        inc    edx
        sub    cl, byte ptr [ebp + edx*4 + 0ffh]
        ja     SHORT loc_7_06B0
        je     SHORT loc_7_06F8
        cmp    dword ptr [ebp + 0fch], edx
        jbe    SHORT loc_7_06F8
        add    cl, byte ptr [ebp + edx*4 + 0ffh]
        mov    eax, edx
loc_7_06CD:
        cmp    byte ptr [ebp + eax*4 + 103h], cl
        jb     SHORT loc_7_06F1
        je     SHORT loc_7_06E3
        inc    eax
        cmp    dword ptr [ebp + 0fch], eax
        ja     SHORT loc_7_06CD
        jmp    SHORT loc_7_06F1
loc_7_06E3:
        mov    eax, dword ptr [ebp + eax*4 + 100h]
        mov    dword ptr [ebp + edx*4 + 0fch], eax
loc_7_06F1:
        sub    cl, byte ptr [ebp + edx*4 + 0ffh]
loc_7_06F8:
        add    byte ptr [ebp + edx*4 + 0ffh], cl
        mov    dword ptr [ebp + 0fch], edx
        test   byte ptr [ebp + 5], 1
        jne    SHORT loc_7_0750
        lea    edx, [edx*4 + 4]
        mov    eax, dword ptr [ebp + 1ch]
        lea    edx, [eax + edx + 1ffh]
        mov    ecx, dword ptr [ebp + 18h]
        lea    eax, [eax + ecx*4 + 207h]
        shr    edx, 9
        shr    eax, 9
        sub    eax, edx
        jbe    SHORT loc_7_0750
        mov    ecx, dword ptr [ebp + 0fch]
        cmp    byte ptr [ebp + ecx*4 + 0ffh], 1
        jne    SHORT loc_7_0749
        dec    ecx
        mov    dword ptr [ebp + 0fch], ecx
        jmp    SHORT loc_7_0750
loc_7_0749:
        dec    byte ptr [ebp + ecx*4 + 0ffh]
loc_7_0750:
        mov    ecx, dword ptr [ebp + 0fch]
        dec    ecx
        mov    edx, 1
loc_7_075C:
        mov    eax, dword ptr [ebp + edx*4 + 100h]
        lea    esi, [edx - 1]
        rol    eax, 8
loc_7_0769:
        mov    ebx, dword ptr [ebp + esi*4 + 100h]
        rol    ebx, 8
        cmp    eax, ebx
        jae    SHORT loc_7_0784
        ror    ebx, 8
        mov    dword ptr [ebp + esi*4 + 104h], ebx
        dec    esi
        jns    SHORT loc_7_0769
loc_7_0784:
        ror    eax, 8
        mov    dword ptr [ebp + esi*4 + 104h], eax
        inc    edx
        loop   loc_7_075C
        call   sub_7_0EFA
        jmp    NEAR PTR loc_7_05B1
sub_7_079B:
        push   ebx
        call   sub_7_082F
        jb     NEAR PTR loc_7_082D
        mov    edx, esi
        sub    edx, dword ptr [ebp + 0d8h]
        shl    edx, 3
        add    edx, eax
        mov    ebx, dword ptr [ebp + 80h]
        cmp    dword ptr [ebp + 84h], ebx
        jne    SHORT loc_7_07D7
        mov    ebx, dword ptr [ebp + 54h]
        and    ebx, 3fffh
        je     SHORT loc_7_07D7
        dec    ebx
        sub    ebx, edx
        jb     SHORT loc_7_082D
        inc    ebx
        cmp    ebx, ecx
        jb     SHORT loc_7_082D
loc_7_07D7:
        push   ecx
        push   edx
        call   sub_7_0A19
        cmp    word ptr [ebp + 0b4h], 3
        jae    SHORT loc_7_07F7
        cmp    dword ptr [ebp + 64h], -1
        je     SHORT loc_7_07F7
        sub    dword ptr [ebp + 64h], edi
        jae    SHORT loc_7_07F7
        and    dword ptr [ebp + 64h], 0
loc_7_07F7:
        call   sub_7_0B80
        pop    edx
        pop    ecx
        cmp    dword ptr [ebp + 58h], -1
        je     SHORT loc_7_0807
        sub    dword ptr [ebp + 58h], ecx
loc_7_0807:
        mov    eax, dword ptr [ebp + 80h]
        shl    eax, 0eh
        add    eax, edx
        add    eax, dword ptr [ebp + 60h]
        inc    eax
        mov    dword ptr [ebp + 0fch], 1
        mov    dword ptr [ebp + 100h], eax
        mov    byte ptr [ebp + 103h], cl
        clc
loc_7_082D:
        pop    ebx
        ret
sub_7_082F:
        cmp    ecx, 1fh
        jb     SHORT sub_7_08A9
        sub    esp, 0ch
        mov    dword ptr [esp + 8], edi
        lea    ebx, [ecx - 0fh]
        shr    ebx, 4
        add    ebx, ebx
        mov    dword ptr [esp + 4], ebx
        add    ebx, esi
        lea    edi, [ebx + 4]
loc_7_084C:
        sub    ebx, 2
        cmp    word ptr [ebx], 0
        jne    SHORT loc_7_0863
        cmp    ebx, esi
        ja     SHORT loc_7_084C
        call   sub_7_08A9
        jae    SHORT loc_7_08A4
        lea    ebx, [edi - 4]
loc_7_0863:
        lea    esi, [ebx + 2]
        mov    ebx, dword ptr [esp + 4]
        add    ebx, esi
        cmp    dword ptr [esp + 8], ebx
        jbe    SHORT loc_7_08A3
        lea    edi, [ebx + 4]
loc_7_0875:
        sub    ebx, 2
        cmp    word ptr [ebx], 0
        jne    SHORT loc_7_0863
        cmp    ebx, esi
        ja     SHORT loc_7_0875
        sub    esi, 2
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esp], edi
        sub    edi, eax
        sbb    edx, edx
        and    edi, edx
        add    edi, eax
        call   sub_7_08A9
        jae    SHORT loc_7_08A4
        mov    ebx, dword ptr [esp]
        sub    ebx, 4
        jmp    SHORT loc_7_0863
loc_7_08A3:
        stc
loc_7_08A4:
        lea    esp, [esp + 0ch]
        ret
sub_7_08A9:
        cmp    ecx, 11h
        jb     SHORT loc_7_0912
        sub    esp, 8
        xor    eax, eax
        mov    ebx, eax
        sub    esi, 2
loc_7_08B8:
        add    esi, 2
        cmp    esi, edi
        jae    SHORT loc_7_090C
        mov    ax, word ptr [esi]
loc_7_08C2:
        mov    dword ptr [esp + 4], esi
        mov    bx, 10h
        cmp    eax, 0
        je     SHORT loc_7_08D2
        bsf    ebx, eax
loc_7_08D2:
        or     ebx, ebx
        je     SHORT loc_7_08B8
        mov    dword ptr [esp], ebx
loc_7_08D9:
        add    esi, 2
        cmp    esi, edi
        jae    SHORT loc_7_090C
        mov    ax, word ptr [esi]
        or     eax, eax
        jne    SHORT loc_7_08F0
        add    ebx, 10h
        cmp    ebx, ecx
        jb     SHORT loc_7_08D9
        jmp    SHORT loc_7_08FF
loc_7_08F0:
        bsr    dx, ax
        xor    dx, 0fh
        add    bx, dx
        cmp    ebx, ecx
        jb     SHORT loc_7_08C2
loc_7_08FF:
        mov    esi, dword ptr [esp + 4]
        mov    eax, 10h
        sub    eax, dword ptr [esp]
        stc
loc_7_090C:
        cmc
        lea    esp, [esp + 8]
        ret
loc_7_0912:
        sub    esp, 4
        mov    edx, 1
        ror    dx, cl
        mov    ebx, edi
        mov    dword ptr [esp], edx
        xchg   edi, ecx
        sub    ecx, esi
        cld
        mov    eax, 0ffffffffh
        shr    ecx, 2
        je     SHORT loc_7_093C
        xchg   esi, edi
        repe scasd
        xchg   esi, edi
        je     SHORT loc_7_093C
        sub    esi, 4
loc_7_093C:
        mov    ecx, edi
        inc    eax
loc_7_093F:
        cmp    ebx, esi
        jbe    SHORT loc_7_098C
        mov    ax, word ptr [esi]
        add    esi, 2
        cmp    ax, -1
        je     SHORT loc_7_093F
        mov    edx, dword ptr [esp]
loc_7_0952:
        mov    edi, edx
        test   edx, eax
        je     SHORT loc_7_0971
loc_7_0958:
        ror    dx, cl
        cmp    edx, edi
        jb     SHORT loc_7_0952
        cmp    ebx, esi
        jbe    SHORT loc_7_098C
        mov    ax, word ptr [esi]
        add    esi, 2
        cmp    ax, -1
        jne    SHORT loc_7_0952
        jmp    SHORT loc_7_093F
loc_7_0971:
        mov    edi, ecx
loc_7_0973:
        dec    edi
        je     SHORT loc_7_098F
        rol    dx, 1
        jb     SHORT loc_7_0983
loc_7_097B:
        test   edx, eax
        je     SHORT loc_7_0973
        mov    edi, edx
        jmp    SHORT loc_7_0958
loc_7_0983:
        sub    esi, 2
        mov    ax, word ptr [esi - 2]
        jmp    SHORT loc_7_097B
loc_7_098C:
        stc
        jmp    SHORT loc_7_0998
loc_7_098F:
        bsf    eax, edx
        sub    esi, 2
        xor    eax, 0fh
loc_7_0998:
        mov    edi, ebx
        lea    esp, [esp + 4]
        ret
sub_7_099F:
        xor    edi, edi
        mov    ebx, ecx
        sub    ecx, 10h
        sbb    edx, edx
        and    ecx, edx
        add    ecx, 10h
        mov    dx, 8000h
        dec    ecx
        sar    dx, cl
        mov    ecx, eax
        shr    dx, cl
        mov    ax, word ptr [esi]
        cmp    eax, 1
        sbb    edi, 0
        not    edx
        and    ax, dx
        cmp    eax, 1
        adc    edi, 0
        mov    word ptr [esi], ax
        neg    ecx
        add    ecx, 10h
        sub    ebx, ecx
        jbe    SHORT loc_7_0A18
        xor    eax, eax
loc_7_09DC:
        add    si, 2
        sub    ebx, 10h
        jb     SHORT loc_7_09F2
        cmp    word ptr [esi], 1
        sbb    edi, 0
        inc    edi
        mov    word ptr [esi], ax
        jmp    SHORT loc_7_09DC
loc_7_09F2:
        add    ebx, 10h
        je     SHORT loc_7_0A18
        mov    dx, 8000h
        lea    ecx, [ebx - 1]
        sar    dx, cl
        mov    ax, word ptr [esi]
        cmp    eax, 1
        sbb    edi, 0
        not    edx
        and    ax, dx
        cmp    eax, 1
        adc    edi, 0
        mov    word ptr [esi], ax
loc_7_0A18:
        ret
sub_7_0A19:
        xor    edi, edi
        mov    ebx, ecx
        sub    ecx, 10h
        sbb    edx, edx
        and    ecx, edx
        add    ecx, 10h
        mov    dx, 8000h
        dec    ecx
        sar    dx, cl
        mov    ecx, eax
        shr    dx, cl
        mov    ax, word ptr [esi]
        cmp    eax, 1
        sbb    edi, 0
        or     ax, dx
        mov    word ptr [esi], ax
        neg    ecx
        add    ecx, 10h
        sub    ebx, ecx
        jbe    SHORT loc_7_0A83
loc_7_0A4C:
        add    si, 2
        sub    ebx, 10h
        jb     SHORT loc_7_0A65
        mov    ax, word ptr [esi]
        cmp    eax, 1
        sbb    edi, 0
        mov    word ptr [esi], 0ffffh
        jmp    SHORT loc_7_0A4C
loc_7_0A65:
        add    ebx, 10h
        je     SHORT loc_7_0A83
        mov    dx, 8000h
        lea    ecx, [ebx - 1]
        sar    dx, cl
        mov    ax, word ptr [esi]
        cmp    eax, 1
        sbb    edi, 0
        or     ax, dx
        mov    word ptr [esi], ax
loc_7_0A83:
        neg    edi
        ret
sub_7_0A86:
        sub    esp, 10h
        mov    dword ptr [esp + 0ch], ebx
        lea    eax, [esi + 800h]
        mov    edx, dword ptr [ebp + 80h]
        cmp    dword ptr [ebp + 84h], edx
        jne    SHORT loc_7_0AB5
        mov    edx, dword ptr [ebp + 54h]
        and    edx, 3fffh
        je     SHORT loc_7_0AB5
        add    edx, 0fh
        shr    edx, 4
        lea    eax, [esi + edx*2]
loc_7_0AB5:
        mov    dword ptr [esp + 8], eax
        mov    edx, ecx
        cld
loc_7_0ABC:
        mov    ecx, dword ptr [esp + 8]
        sub    ecx, esi
        jbe    NEAR PTR loc_7_0B75
        shr    ecx, 1
        mov    ax, 0ffffh
        mov    edi, esi
        repe scasw
        mov    esi, edi
        je     NEAR PTR loc_7_0B75
        mov    ecx, 10h
loc_7_0AE0:
        lea    eax, [esi - 2]
        sub    eax, dword ptr [ebp + 0d8h]
        shl    eax, 3
        mov    edi, dword ptr [ebp + 80h]
        shl    edi, 0eh
        add    eax, edi
        mov    dword ptr [esp + 4], eax
        mov    ax, word ptr [esi - 2]
loc_7_0AFF:
        dec    ecx
        js     SHORT loc_7_0ABC
        bt     eax, ecx
        jb     SHORT loc_7_0AFF
        mov    dword ptr [esp], ecx
        xor    edi, edi
loc_7_0B0C:
        inc    edi
        dec    ecx
        js     SHORT loc_7_0B52
        bt     eax, ecx
        jae    SHORT loc_7_0B0C
loc_7_0B15:
        mov    ebx, dword ptr [esp]
        xor    ebx, 0fh
        add    ebx, dword ptr [esp + 4]
        lea    eax, [edi + ebx]
        sub    eax, dword ptr [ebp + 54h]
        jb     SHORT loc_7_0B2D
        xor    ecx, ecx
        sub    edi, eax
        jbe    SHORT loc_7_0ABC
loc_7_0B2D:
        sub    edx, edi
        inc    ebx
        add    ebx, dword ptr [ebp + 60h]
        shl    edi, 18h
        or     edi, ebx
        mov    eax, dword ptr [esp + 0ch]
        inc    dword ptr [eax]
        mov    ebx, dword ptr [eax]
        lea    ebx, [eax + ebx*4]
        mov    dword ptr [ebx], edi
        or     edx, edx
        jle    SHORT loc_7_0B75
        or     ecx, ecx
        jne    SHORT loc_7_0AE0
        jmp    NEAR PTR loc_7_0ABC
loc_7_0B52:
        cmp    dword ptr [esp + 8], esi
        jbe    SHORT loc_7_0B15
        mov    ax, word ptr [esi]
        add    esi, 2
        or     ax, ax
        jne    SHORT loc_7_0B6D
        add    edi, 10h
        cmp    edi, 41h
        jb     SHORT loc_7_0B52
        jmp    SHORT loc_7_0B15
loc_7_0B6D:
        dec    edi
        mov    ecx, 10h
        jmp    SHORT loc_7_0B0C
loc_7_0B75:
        cmp    edx, 42h
        sbb    ecx, ecx
        and    ecx, edx
        add    esp, 10h
        ret
sub_7_0B80:
        or     dword ptr [ebp + 4], 1000h
        mov    edi, dword ptr [ebp + 2ch]
        cmp    byte ptr [edi + 32h], 0ffh
        je     SHORT loc_7_0BA9
        mov    al, 0fh
        mov    ecx, dword ptr [ebp + 80h]
        and    ecx, 1
        shl    ecx, 2
        shl    al, cl
        cmp    byte ptr [edi + 32h], 0
        je     SHORT loc_7_0BAA
loc_7_0BA6:
        or     byte ptr [edi + 32h], al
loc_7_0BA9:
        ret
loc_7_0BAA:
        mov    ecx, eax
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [edi + 24h], eax
        db  0A1h, 090h, 00Dh, 000h, 000h
        mov    dword ptr [edi + 14h], eax
        db  0C7h, 047h, 018h, 07Ch, 00Dh, 000h, 000h
        mov    dword ptr [eax + 18h], edi
        db  089h, 03Dh, 090h, 00Dh, 000h, 000h
        mov    eax, ecx
        jmp    SHORT loc_7_0BA6
sub_7_0BD1:
        or     edx, 0ffffffffh
        jmp    SHORT loc_7_0BD8
sub_7_0BD6:
        xor    edx, edx
loc_7_0BD8:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    ecx, eax
        and    eax, 0ffffffh
        shr    ecx, 18h
        sub    eax, dword ptr [ebp + 60h]
        jbe    SHORT loc_7_0C30
        dec    eax
        js     SHORT loc_7_0C30
        lea    edx, [ecx + eax]
        cmp    dword ptr [ebp + 54h], edx
        jb     SHORT loc_7_0C30
        push   eax
        push   ecx
        shr    eax, 0eh
        call   sub_7_101D
        pop    ecx
        pop    eax
        jb     SHORT loc_7_0C2B
        mov    esi, eax
        and    esi, 3fffh
        mov    edx, esi
        shr    esi, 3
        and    esi, 0fffffffeh
        and    eax, 0fh
        add    esi, dword ptr [ebp + 0d8h]
        add    edx, ecx
        sub    edx, 4000h
        ja     SHORT loc_7_0C5C
        xor    edx, edx
loc_7_0C2B:
        lea    esp, [esp + 4]
        ret
loc_7_0C30:
        add    dword ptr [esp], 1
        jb     SHORT loc_7_0C2B
        mov    eax, dword ptr [ebp + 0e0h]
        mov    word ptr [eax], 14h
        bts    dword ptr [ebp + 4], 0fh
        jb     SHORT loc_7_0C2B
        call   sub_7_43D3
        jb     SHORT loc_7_0C2B
        mov    esi, 4
        call   sub_7_4B5E
        stc
        jmp    SHORT loc_7_0C2B
loc_7_0C5C:
        sub    ecx, edx
        mov    edi, dword ptr [ebp + 80h]
        inc    edi
        shl    edi, 0eh
        inc    edi
        add    edi, dword ptr [ebp + 60h]
        shl    edi, 8
        shld   edx, edi, 18h
        clc
        jmp    SHORT loc_7_0C2B
sub_7_0C76:
        test   byte ptr [ebp + 5], 80h
        jne    SHORT loc_7_0CAF
loc_7_0C7C:
        call   sub_7_0BD6
        jb     SHORT loc_7_0CAF
        push   edx
        cmp    dword ptr [ebp + 58h], -1
        je     SHORT loc_7_0C8D
        add    dword ptr [ebp + 58h], ecx
loc_7_0C8D:
        call   sub_7_099F
        cmp    word ptr [ebp + 0b4h], 3
        jae    SHORT loc_7_0CA5
        cmp    dword ptr [ebp + 64h], -1
        je     SHORT loc_7_0CA5
        add    dword ptr [ebp + 64h], edi
loc_7_0CA5:
        call   sub_7_0B80
        pop    eax
        or     eax, eax
        jne    SHORT loc_7_0C7C
loc_7_0CAF:
        ret
sub_7_0CB0:
        test   eax, 20000000h
        jne    SHORT loc_7_0CD1
        shl    eax, 8
        mov    al, ch
        ror    eax, 8
        mov    dword ptr [ebp + 100h], eax
        mov    dword ptr [ebp + 0fch], 1
        clc
loc_7_0CD0:
        ret
loc_7_0CD1:
        or     ebx, ebx
        je     NEAR PTR loc_7_0DBB
        test   eax, 40000000h
        je     SHORT loc_7_0D5D
        push   dword ptr [ebp + 0d0h]
        push   dword ptr [ebp + 0c8h]
        push   dword ptr [ebp + 0dch]
        lea    edx, [ebp + 224h]
        mov    dword ptr [ebp + 0d0h], edx
        and    dword ptr [ebp + 0c8h], 0
        or     dword ptr [ebp + 0dch], 2
        and    dword ptr [edx + 10h], 0
        mov    dword ptr [edx], 1
        movzx  esi, byte ptr [ebp + 9bh]
        dec    esi
        shl    esi, 9
        add    esi, ebx
        mov    dword ptr [edx + 4], esi
        movzx  ecx, ch
        dec    ecx
        mov    dword ptr [edx + 8], ecx
        mov    dword ptr [edx + 0ch], ebx
        push   eax
        push   ecx
        push   esi
        inc    ecx
        and    eax, 0ffffffh
        xor    edi, edi
        call   sub_7_2E50
        pop    esi
        pop    ecx
        pop    eax
        pop    dword ptr [ebp + 0dch]
        pop    dword ptr [ebp + 0c8h]
        pop    dword ptr [ebp + 0d0h]
        jb     NEAR PTR loc_7_0CD0
        jmp    SHORT loc_7_0D78
loc_7_0D5D:
        push   eax
        push   ecx
        movzx  ecx, ch
        and    eax, 0ffffffh
        mov    edi, ebx
        call   sub_7_2E50
        pop    ecx
        pop    eax
        jb     NEAR PTR loc_7_0CD0
        mov    esi, ebx
loc_7_0D78:
        push   eax
        and    eax, 0ffffffh
        call   sub_7_0DE5
        pop    eax
        jb     SHORT loc_7_0DB0
        bt     eax, 1eh
        sbb    ch, 0
        cmp    byte ptr [ebp + 9bh], ch
        jb     SHORT loc_7_0DB0
        or     ebx, ebx
        jne    NEAR PTR loc_7_0CD0
loc_7_0D9D:
        pushfd
        call   sub_7_489D
        db  083h, 025h, 02Ch, 048h, 000h, 000h, 000h
        popfd
        jmp    NEAR PTR loc_7_0CD0
loc_7_0DB0:
        cmp    ebx, 1
        jb     SHORT loc_7_0D9D
        stc
        jmp    NEAR PTR loc_7_0CD0
loc_7_0DBB:
        push   eax
        push   ebx
        push   ecx
        db  0E8h, 089h, 03Ah, 000h, 000h
        pop    ecx
        pop    ebx
        pop    eax
        jb     NEAR PTR loc_7_0CD0
        push   eax
        push   ecx
        mov    ecx, 1
        and    eax, 0ffffffh
        push   edi
        call   sub_7_2E50
        pop    esi
        pop    ecx
        pop    eax
        jb     SHORT loc_7_0D9D
        jmp    SHORT loc_7_0D78
sub_7_0DE5:
        lea    edi, [ebp + 0fch]
        mov    edx, dword ptr [esi]
        add    esi, 4
        cmp    edx, 41h
        ja     NEAR PTR loc_7_0E8D
        dec    edx
        jle    NEAR PTR loc_7_0E8D
        cmp    byte ptr [ebp + 9bh], dl
        jb     NEAR PTR loc_7_0E8C
        inc    edx
        mov    dword ptr [edi], edx
        add    edi, 4
        mov    edx, dword ptr [esi]
        add    esi, 4
        test   edx, 3800000h
        jne    SHORT loc_7_0E8D
        rol    edx, 8
        shr    dl, 2
        inc    dl
        or     ch, ch
        je     SHORT loc_7_0E2F
        cmp    dl, ch
        jne    SHORT loc_7_0E8D
loc_7_0E2F:
        mov    ch, dl
        shr    edx, 8
        cmp    edx, eax
        jne    SHORT loc_7_0E8D
        shl    edx, 8
        mov    dl, ch
        ror    edx, 8
        mov    dword ptr [edi], edx
        add    edi, 4
        mov    edx, dword ptr [ebp + 0fch]
        dec    edx
loc_7_0E4C:
        mov    eax, dword ptr [esi]
        add    esi, 4
        test   eax, 3800000h
        jne    SHORT loc_7_0E8D
        rol    eax, 8
        shr    al, 2
        inc    al
        ror    eax, 8
        mov    dword ptr [edi], eax
        add    edi, 4
        rol    eax, 8
        cmp    byte ptr [ebp + 9bh], al
        jb     SHORT loc_7_0E8C
        add    ch, al
        jb     SHORT loc_7_0E8C
        shr    eax, 8
        cmp    dword ptr [ebp + 60h], eax
        ja     SHORT loc_7_0E8D
        dec    edx
        jne    SHORT loc_7_0E4C
        dec    ch
        cmp    byte ptr [ebp + 9bh], ch
        inc    ch
loc_7_0E8C:
        ret
loc_7_0E8D:
        stc
        jmp    SHORT loc_7_0E8C
sub_7_0E90:
        test   byte ptr [ebp + 5], 80h
        jne    SHORT loc_7_0EBF
        mov    byte ptr [ebp + 0f2h], 0ffh
        xor    ebx, ebx
        call   sub_7_0CB0
        jb     SHORT loc_7_0EBF
        lea    ebx, [ebp + 0fch]
        mov    ecx, dword ptr [ebx]
loc_7_0EAE:
        mov    eax, dword ptr [ebx + ecx*4]
        push   ebx
        push   ecx
        call   sub_7_0C76
        pop    ecx
        pop    ebx
        jb     SHORT loc_7_0EBF
        loop   loc_7_0EAE
        clc
loc_7_0EBF:
        ret
sub_7_0EC0:
        call   sub_7_0BD6
        jb     SHORT loc_7_0EF9
        push   edx
        cmp    dword ptr [ebp + 58h], -1
        je     SHORT loc_7_0ED1
        sub    dword ptr [ebp + 58h], ecx
loc_7_0ED1:
        call   sub_7_0A19
        cmp    word ptr [ebp + 0b4h], 3
        jae    SHORT loc_7_0EEF
        cmp    dword ptr [ebp + 64h], -1
        je     SHORT loc_7_0EEF
        sub    dword ptr [ebp + 64h], edi
        jae    SHORT loc_7_0EEF
        and    dword ptr [ebp + 64h], 0
loc_7_0EEF:
        call   sub_7_0B80
        pop    eax
        or     eax, eax
        jne    SHORT sub_7_0EC0
loc_7_0EF9:
        ret
sub_7_0EFA:
        push   ebx
        lea    ebx, [ebp + 0fch]
        mov    ecx, dword ptr [ebx]
loc_7_0F03:
        mov    eax, dword ptr [ebx + ecx*4]
        push   ebx
        push   ecx
        call   sub_7_0EC0
        pop    ecx
        pop    ebx
        jb     SHORT loc_7_0F14
        loop   loc_7_0F03
        clc
loc_7_0F14:
        pop    ebx
        ret
sub_7_0F16:
        call   sub_7_0BD1
        jb     SHORT loc_7_0F2A
        push   edx
        call   sub_7_0F2B
        pop    eax
        jb     SHORT loc_7_0F2A
        or     eax, eax
        jne    SHORT sub_7_0F16
loc_7_0F2A:
        ret
sub_7_0F2B:
        mov    edi, ecx
        sub    ecx, 10h
        sbb    edx, edx
        and    ecx, edx
        add    ecx, 10h
        mov    dx, 8000h
        dec    ecx
        sar    dx, cl
        mov    ecx, eax
        shr    dx, cl
        test   word ptr [esi], dx
        jne    SHORT loc_7_0F79
        neg    ecx
        add    ecx, 10h
        sub    edi, ecx
        jbe    SHORT loc_7_0F77
loc_7_0F52:
        add    si, 2
        sub    edi, 10h
        jb     SHORT loc_7_0F63
        cmp    word ptr [esi], 0
        jne    SHORT loc_7_0F79
        jmp    SHORT loc_7_0F52
loc_7_0F63:
        add    edi, 10h
        je     SHORT loc_7_0F77
        mov    dx, 8000h
        lea    ecx, [edi - 1]
        sar    dx, cl
        test   word ptr [esi], dx
        jne    SHORT loc_7_0F79
loc_7_0F77:
        clc
loc_7_0F78:
        ret
loc_7_0F79:
        stc
        jmp    SHORT loc_7_0F78
sub_7_0F7C:
        push   eax
        push   ecx
        shl    eax, 8
        mov    al, ch
        ror    eax, 8
        mov    dword ptr [ebp + 100h], eax
        mov    dword ptr [ebp + 0fch], 1
        call   sub_7_0F16
        pop    ecx
        pop    eax
        jb     SHORT loc_7_0FA6
        test   eax, 20000000h
        jne    SHORT loc_7_0FA7
loc_7_0FA6:
        ret
loc_7_0FA7:
        call   sub_7_0CB0
        jb     SHORT loc_7_0FA6
        push   ebx
        call   sub_7_0FB7
        pop    ebx
        jmp    SHORT loc_7_0FA6
sub_7_0FB7:
        lea    edx, [ebp + 0fch]
        mov    ecx, dword ptr [edx]
loc_7_0FBF:
        mov    eax, dword ptr [edx + ecx*4]
        push   ecx
        push   edx
        call   sub_7_0F16
        pop    edx
        pop    ecx
        jb     SHORT loc_7_1007
        cmp    dword ptr [edx], ecx
        jbe    SHORT loc_7_1005
        push   ecx
        mov    eax, dword ptr [edx + ecx*4]
        xor    ebx, ebx
        shld   ebx, eax, 8
        and    eax, 0ffffffh
        lea    ebx, [ebx + eax - 1]
loc_7_0FE4:
        inc    ecx
        mov    edi, dword ptr [edx + ecx*4]
        xor    esi, esi
        shld   esi, edi, 8
        and    edi, 0ffffffh
        lea    esi, [esi + edi - 1]
        cmp    ebx, edi
        jb     SHORT loc_7_1000
        cmp    eax, esi
        jbe    SHORT loc_7_1008
loc_7_1000:
        cmp    dword ptr [edx], ecx
        ja     SHORT loc_7_0FE4
        pop    ecx
loc_7_1005:
        loop   loc_7_0FBF
loc_7_1007:
        ret
loc_7_1008:
        pop    ecx
        stc
        jmp    SHORT loc_7_1007
sub_7_100C:
        cmp    dword ptr [ebp + 58h], -1
        jne    SHORT loc_7_101B
        push   ebx
        call   sub_7_106E
        pop    ebx
        jb     SHORT loc_7_101C
loc_7_101B:
        clc
loc_7_101C:
        ret
sub_7_101D:
        mov    edx, eax
        and    al, 0feh
        mov    esi, dword ptr [ebp + 84h]
        inc    esi
        mov    ecx, eax
        add    ecx, 2
        sub    ecx, esi
        sbb    edi, edi
        and    ecx, edi
        add    ecx, esi
        sub    ecx, eax
        shl    ecx, 2
        shl    eax, 2
        push   ebx
        push   edx
        lea    ebx, [ebp + 2ch]
        call   sub_7_2C95
        pop    edx
        pop    ebx
        jb     SHORT loc_7_1061
        mov    dword ptr [ebp + 80h], edx
        and    edx, 1
        shl    edx, 0bh
        add    eax, edx
        mov    dword ptr [ebp + 0d8h], eax
        clc
loc_7_1060:
        ret
loc_7_1061:
        mov    esi, 2
        call   sub_7_4B5E
        stc
        jmp    SHORT loc_7_1060
sub_7_106E:
        mov    byte ptr [ebp + 0f2h], 0ffh
        mov    ecx, dword ptr [ebp + 4]
        xor    ecx, 2000h
        test   ecx, 0b000h
        setne  cl
        rol    ecx, 0ch
        and    ecx, 1000h
        and    dword ptr [ebp + 4], 0ffff7fffh
        mov    eax, dword ptr [ebp + 38h]
        test   byte ptr [eax + 20h], 2
        je     NEAR PTR loc_7_1254
        sub    esp, 28h
        mov    dword ptr [esp + 24h], ecx
        or     eax, 0ffffffffh
        mov    dword ptr [esp], eax
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 0ch], eax
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [esp + 18h], eax
        mov    dword ptr [esp + 1ch], eax
        mov    ecx, dword ptr [ebp + 54h]
        shr    ecx, 4
        mov    dword ptr [ebp + 64h], ecx
        mov    ecx, dword ptr [ebp + 54h]
        mov    dword ptr [ebp + 58h], ecx
        neg    ecx
        and    ecx, 0fh
        je     SHORT loc_7_1125
        mov    ebx, 1
        shl    ebx, cl
        dec    ebx
        mov    eax, dword ptr [ebp + 84h]
        btr    dword ptr [esp], eax
        call   sub_7_101D
        jb     NEAR PTR loc_7_126E
        mov    edi, eax
        mov    ecx, 200h
        xor    eax, eax
        cld
        rep stosd
        mov    edx, dword ptr [ebp + 54h]
        and    edx, 3fffh
        shr    edx, 4
        or     word ptr [edi + edx*2 - 800h], bx
        call   sub_7_0B80
loc_7_1125:
        mov    ebx, dword ptr [ebp + 78h]
        add    ebx, 2
loc_7_112B:
        cmp    dword ptr [ebp + 7ch], ebx
        jbe    NEAR PTR loc_7_11FC
        mov    eax, ebx
        inc    ebx
        call   sub_7_2933
        jb     NEAR PTR loc_7_126E
        test   eax, 80000000h
        je     SHORT loc_7_112B
        push   ebx
        xor    ebx, ebx
        call   sub_7_0CB0
        pop    ebx
        jb     NEAR PTR loc_7_125E
        mov    edx, dword ptr [ebp + 0fch]
        mov    dword ptr [esp + 20h], edx
loc_7_1162:
        lea    edi, [ebp + 0fch]
        mov    edx, dword ptr [esp + 20h]
        mov    eax, dword ptr [edi + edx*4]
loc_7_116F:
        call   sub_7_0BD6
        jb     SHORT loc_7_11D3
        sub    dword ptr [ebp + 58h], ecx
        cmc
        sbb    edi, edi
        and    dword ptr [ebp + 58h], edi
        mov    edi, dword ptr [ebp + 80h]
        btr    dword ptr [esp], edi
        jae    SHORT loc_7_119F
        mov    edi, dword ptr [ebp + 0d8h]
        push   eax
        push   ecx
        mov    ecx, 200h
        xor    eax, eax
        cld
        rep stosd
        pop    ecx
        pop    eax
loc_7_119F:
        push   eax
        push   ecx
        push   edx
        push   esi
        call   sub_7_0F2B
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        jb     SHORT loc_7_11DE
loc_7_11AE:
        push   edx
        push   ebx
        call   sub_7_0A19
        pop    ebx
        cmp    word ptr [ebp + 0b4h], 3
        jae    SHORT loc_7_11C9
        sub    dword ptr [ebp + 64h], edi
        jae    SHORT loc_7_11C9
        and    dword ptr [ebp + 64h], 0
loc_7_11C9:
        call   sub_7_0B80
        pop    eax
        or     eax, eax
        jne    SHORT loc_7_116F
loc_7_11D3:
        dec    dword ptr [esp + 20h]
        jne    SHORT loc_7_1162
        jmp    NEAR PTR loc_7_112B
loc_7_11DE:
        bts    dword ptr [ebp + 4], 0fh
        jb     SHORT loc_7_11AE
        call   sub_7_43D3
        jb     SHORT loc_7_11AE
        push   esi
        push   edx
        mov    esi, 4
        call   sub_7_4B5E
        pop    edx
        pop    esi
        jmp    SHORT loc_7_11AE
loc_7_11FC:
        mov    edx, dword ptr [ebp + 84h]
        inc    edx
loc_7_1203:
        dec    edx
        js     SHORT loc_7_122A
        btr    dword ptr [esp], edx
        jae    SHORT loc_7_1203
        mov    eax, edx
        push   edx
        call   sub_7_101D
        pop    edx
        jb     SHORT loc_7_126E
        mov    edi, eax
        mov    ecx, 200h
        xor    eax, eax
        cld
        rep stosd
        call   sub_7_0B80
        jmp    SHORT loc_7_1203
loc_7_122A:
        mov    ecx, dword ptr [esp + 24h]
        btr    dword ptr [ebp + 4], 0ch
        or     dword ptr [ebp + 4], ecx
        inc    edx
        mov    dword ptr [ebp + 80h], edx
        test   byte ptr [ebp + 4], 4
        je     SHORT loc_7_1248
        call   sub_7_1CEA
loc_7_1248:
        lea    esp, [esp + 28h]
loc_7_124C:
        pushfd
        bts    dword ptr [ebp + 4], 0dh
        popfd
        ret
loc_7_1254:
        xor    eax, eax
        mov    dword ptr [ebp + 58h], eax
        mov    dword ptr [ebp + 64h], eax
        jmp    SHORT loc_7_124C
loc_7_125E:
        mov    eax, dword ptr [ebp + 0e0h]
        cmp    word ptr [eax], 10h
        jb     NEAR PTR loc_7_112B
loc_7_126E:
        bts    dword ptr [ebp + 4], 0fh
        xor    eax, eax
        stc
        mov    dword ptr [ebp + 58h], eax
        mov    dword ptr [ebp + 64h], eax
        jmp    SHORT loc_7_1248
        db  000h, 000h
sub_7_1280:
        cmp    eax, 0c0000000h
        cmc
        jb     SHORT loc_7_129C
        cmp    eax, 400000h
        jae    SHORT loc_7_129C
        mov    ecx, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        xchg   ebx, ecx
        add    eax, dword ptr [ecx + 4]
loc_7_129C:
        ret
sub_7_129D:
        sub    esp, 4
        mov    dword ptr [esp], eax
        or     edx, 40000000h
        lea    ecx, [ecx + eax - 1]
        shr    eax, 0ch
        shr    ecx, 0ch
        sub    ecx, eax
        jb     SHORT loc_7_12D4
        inc    ecx
        push   edx
        push   ecx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, 1
        jb     SHORT loc_7_12D4
        mov    edx, dword ptr [esp]
        and    edx, 0fffh
        or     eax, edx
loc_7_12D4:
        lea    esp, [esp + 4]
        ret
sub_7_12D9:
        lea    ecx, [ecx + eax - 1]
        shr    eax, 0ch
        shr    ecx, 0ch
        sub    ecx, eax
        jb     SHORT loc_7_12FB
        inc    ecx
        push   40000000h
        push   ecx
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, 1
loc_7_12FB:
        ret
sub_7_12FC:
        sub    esp, 10h
        mov    dword ptr [esp], ecx
        mov    dword ptr [esp + 0ch], edx
        xor    edx, edx
        mov    dword ptr [esp + 8], edx
        mov    word ptr [esi + 3], 0dh
        mov    ecx, dword ptr [esp + 0ch]
        cmp    ecx, 10h
        jb     SHORT loc_7_1395
        mov    word ptr [esi + 3], 2
        mov    eax, esi
        call   sub_7_129D
        jb     SHORT loc_7_1395
        mov    esi, eax
        movzx  ecx, word ptr [esi + 6]
        or     ecx, ecx
        je     SHORT loc_7_138A
        mov    dword ptr [esp + 4], ecx
        mov    word ptr [esi + 3], 0dh
        shl    ecx, 4
        add    ecx, 10h
        cmp    dword ptr [esp + 0ch], ecx
        jb     SHORT loc_7_139A
        mov    word ptr [esi + 3], 2
        lea    edi, [esi + 10h]
loc_7_1353:
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     SHORT loc_7_1373
        call   sub_7_1280
        jb     SHORT loc_7_139A
        movzx  ecx, byte ptr [edi + 0ah]
        shl    ecx, 9
        je     SHORT loc_7_13AA
        mov    edx, dword ptr [esp]
        call   sub_7_129D
        jb     SHORT loc_7_139A
loc_7_1373:
        mov    dword ptr [edi + 0ch], eax
        inc    dword ptr [esp + 8]
        add    edi, 10h
        dec    dword ptr [esp + 4]
        jne    SHORT loc_7_1353
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 0ch], eax
loc_7_138A:
        mov    word ptr [esi + 3], 4f4bh
        or     byte ptr [esi + 5], 1
        clc
loc_7_1395:
        lea    esp, [esp + 10h]
        ret
loc_7_139A:
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
        call   sub_7_13B5
        stc
        jmp    SHORT loc_7_1395
loc_7_13AA:
        mov    word ptr [esi + 3], 2
        jmp    SHORT loc_7_139A
sub_7_13B2:
        mov    ecx, dword ptr [esi + 0ch]
sub_7_13B5:
        sub    esp, 8
        mov    dword ptr [esp + 4], edx
        jecxz  loc_7_13DF
        mov    dword ptr [esp], ecx
        lea    edi, [esi + 10h]
loc_7_13C4:
        mov    eax, dword ptr [edi + 0ch]
        or     eax, eax
        je     SHORT loc_7_13D7
        movzx  ecx, byte ptr [edi + 0ah]
        shl    ecx, 9
        call   sub_7_12D9
loc_7_13D7:
        add    edi, 10h
        dec    dword ptr [esp]
        jne    SHORT loc_7_13C4
loc_7_13DF:
        and    byte ptr [esi + 5], 0feh
        mov    eax, esi
        mov    ecx, dword ptr [esp + 4]
        call   sub_7_12D9
        add    esp, 8
        ret
sub_7_13F2:
        sub    esp, 0ch
        mov    dword ptr [esp + 4], esi
        mov    dword ptr [ebp + 8], 0
        and    dword ptr [ebp + 4], 0ffffffbfh
        movzx  ecx, word ptr [esi + 6]
        or     ecx, ecx
        je     NEAR PTR loc_7_14BC
        mov    dword ptr [esp + 8], ecx
        lea    edi, [esi + 10h]
        mov    dword ptr [esp], edi
loc_7_141A:
        mov    edi, dword ptr [esp]
        movzx  eax, word ptr [edi + 8]
        or     eax, eax
        je     NEAR PTR loc_7_1516
        cmp    eax, 2
        jb     NEAR PTR loc_7_1537
        add    eax, dword ptr [ebp + 78h]
        cmp    dword ptr [ebp + 7ch], eax
        jbe    NEAR PTR loc_7_1537
        test   byte ptr [ebp + 5], 18h
        jne    NEAR PTR loc_7_1558
        call   sub_7_2933
        jb     NEAR PTR loc_7_153B
        test   eax, 80000000h
        je     NEAR PTR loc_7_153F
        mov    dword ptr [ebp + 0ch], eax
        mov    word ptr [ebp + 10h], cx
        test   eax, 20000000h
        jne    SHORT loc_7_14C1
        mov    edi, dword ptr [esp]
        mov    eax, edi
        mov    edi, dword ptr [edi + 0ch]
        or     edi, edi
        je     SHORT loc_7_1490
        cmp    byte ptr [eax + 0ah], ch
        jne    NEAR PTR loc_7_1554
        call   sub_7_2AA1
        jb     NEAR PTR loc_7_153B
        and    dword ptr [ebp + 4], 0ffffffbfh
loc_7_1490:
        mov    edi, dword ptr [esp]
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [edi + 0ah], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, eax
        shr    eax, 1ch
        mov    byte ptr [edi + 0bh], al
        and    edx, 0ffffffh
        mov    dword ptr [edi + 4], edx
loc_7_14AD:
        add    dword ptr [esp], 10h
        dec    dword ptr [esp + 8]
        jne    NEAR PTR loc_7_141A
        clc
loc_7_14BC:
        lea    esp, [esp + 0ch]
        ret
loc_7_14C1:
        db  0E8h, 086h, 033h, 000h, 000h
        jb     SHORT loc_7_153B
        push   edi
        call   sub_7_2AA1
        pop    esi
        jb     SHORT loc_7_1500
        and    dword ptr [ebp + 4], 0ffffffbfh
        add    esi, dword ptr [ebp + 20h]
        mov    edi, dword ptr [esp]
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_7_14F5
        mov    ecx, dword ptr [ebp + 18h]
        cmp    byte ptr [edi + 0ah], cl
        jne    SHORT loc_7_150B
        mov    edi, dword ptr [edi + 0ch]
        shl    ecx, 9
        sub    ecx, dword ptr [ebp + 20h]
        cld
        rep movsb
loc_7_14F5:
        call   sub_7_489D
        and    dword ptr [ebp + 20h], 0
        jmp    SHORT loc_7_1490
loc_7_1500:
        call   sub_7_489D
        and    dword ptr [ebp + 20h], 0
        jmp    SHORT loc_7_153B
loc_7_150B:
        call   sub_7_489D
        and    dword ptr [ebp + 20h], 0
        jmp    SHORT loc_7_1554
loc_7_1516:
        mov    eax, dword ptr [edi + 4]
        movzx  ecx, byte ptr [edi + 0ah]
        mov    edi, dword ptr [edi + 0ch]
        or     edi, edi
        je     SHORT loc_7_14AD
        cmp    eax, dword ptr [ebp + 60h]
        sbb    eax, 0
        call   sub_7_2E50
        jae    NEAR PTR loc_7_14AD
        jmp    SHORT loc_7_153B
loc_7_1537:
        mov    al, 4
        jmp    SHORT loc_7_1562
loc_7_153B:
        mov    al, 3
        jmp    SHORT loc_7_1562
loc_7_153F:
        mov    edi, dword ptr [esp]
        and    dword ptr [edi + 4], 0
        cmp    dword ptr [edi + 0ch], 0
        je     NEAR PTR loc_7_14AD
        mov    al, 5
        jmp    SHORT loc_7_1562
loc_7_1554:
        mov    al, 0ch
        jmp    SHORT loc_7_1562
loc_7_1558:
        mov    al, 1
        test   byte ptr [ebp + 5], 10h
        jne    SHORT loc_7_1562
        mov    al, 0eh
loc_7_1562:
        movzx  eax, al
        mov    esi, dword ptr [esp + 4]
        mov    word ptr [esi + 3], ax
        and    dword ptr [ebp + 4], 0ffffe7ffh
        stc
        jmp    NEAR PTR loc_7_14BC
sub_7_157A:
        sub    esp, 0ch
        test   byte ptr [ebp + 5], 18h
        jne    NEAR PTR loc_7_17B1
        and    dword ptr [ebp + 8], 0
        and    dword ptr [ebp + 4], 0ffffffbfh
        test   byte ptr [ebp + 5], 20h
        je     NEAR PTR loc_7_1756
        movzx  ecx, word ptr [esi + 6]
        or     ecx, ecx
        je     NEAR PTR loc_7_1744
        mov    dword ptr [esp + 8], ecx
        mov    dword ptr [esp + 4], esi
        lea    edi, [esi + 10h]
        mov    dword ptr [esp], edi
loc_7_15B3:
        mov    edi, dword ptr [esp]
        movzx  eax, word ptr [edi + 8]
        cmp    eax, 2
        jb     NEAR PTR loc_7_17A5
        add    eax, dword ptr [ebp + 78h]
        cmp    dword ptr [ebp + 7ch], eax
        jbe    NEAR PTR loc_7_17A5
        movzx  ecx, byte ptr [edi + 0ah]
        mov    eax, dword ptr [edi + 4]
        or     ecx, ecx
        je     NEAR PTR loc_7_17AD
        test   byte ptr [edi + 0bh], 2
        jne    NEAR PTR loc_7_180F
        cmp    byte ptr [ebp + 9bh], cl
        jb     NEAR PTR loc_7_17AD
        mov    edx, eax
        or     edx, edx
        js     NEAR PTR loc_7_1763
        rol    eax, 8
        or     al, al
        jne    NEAR PTR loc_7_17A9
        mov    al, cl
        ror    eax, 8
        mov    dword ptr [ebp + 100h], eax
        mov    dword ptr [ebp + 0fch], 1
        sub    edx, dword ptr [ebp + 60h]
        jbe    NEAR PTR loc_7_17A9
        dec    edx
        and    edx, 3fffh
        add    edx, ecx
        cmp    edx, 4000h
        ja     NEAR PTR loc_7_17BD
        call   sub_7_0F16
        jb     NEAR PTR loc_7_1793
loc_7_1647:
        call   sub_7_0EFA
        jb     NEAR PTR loc_7_17A1
loc_7_1652:
        add    dword ptr [esp], 10h
        dec    dword ptr [esp + 8]
        jne    NEAR PTR loc_7_15B3
        mov    ecx, dword ptr [esp + 4]
        mov    edi, ecx
        movzx  ecx, word ptr [ecx + 6]
        mov    dword ptr [esp + 8], ecx
        add    edi, 10h
        mov    dword ptr [esp], edi
loc_7_1674:
        mov    edi, dword ptr [esp]
        test   byte ptr [edi + 0bh], 2
        jne    NEAR PTR loc_7_1885
        mov    eax, dword ptr [edi + 4]
        rol    eax, 8
        mov    al, byte ptr [edi + 0ah]
        ror    eax, 8
        mov    edi, dword ptr [edi + 0ch]
        or     edi, edi
        je     NEAR PTR loc_7_17D6
        mov    dword ptr [ebp + 100h], eax
        mov    dword ptr [ebp + 0fch], 1
        call   sub_7_259A
        jb     NEAR PTR loc_7_17DA
loc_7_16B3:
        mov    edi, dword ptr [esp]
        movzx  eax, word ptr [edi + 8]
        add    eax, dword ptr [ebp + 78h]
        call   sub_7_2933
        jb     NEAR PTR loc_7_17DA
        mov    dword ptr [ebp + 0ch], eax
        mov    word ptr [ebp + 10h], cx
        test   eax, 0ffffffh
        je     NEAR PTR loc_7_17DE
        mov    edi, dword ptr [esp]
        mov    al, byte ptr [edi + 0bh]
        shl    eax, 1ch
        or     eax, dword ptr [edi + 4]
        test   eax, 20000000h
        jne    SHORT loc_7_1749
        mov    ch, byte ptr [edi + 0ah]
        test   eax, 40000000h
        je     SHORT loc_7_16F9
        mov    cl, ch
loc_7_16F9:
        movzx  edx, word ptr [edi + 8]
        add    edx, dword ptr [ebp + 78h]
        call   sub_7_29FE
        jb     NEAR PTR loc_7_17DA
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, 80000000h
        je     SHORT loc_7_1724
        mov    cx, word ptr [ebp + 10h]
        call   sub_7_0E90
        jb     NEAR PTR loc_7_17DA
loc_7_1724:
        add    dword ptr [esp], 10h
        dec    dword ptr [esp + 8]
        jne    NEAR PTR loc_7_1674
        call   sub_7_1CEA
        jb     NEAR PTR loc_7_17DA
        and    dword ptr [ebp + 4], 0ffffefffh
loc_7_1744:
        lea    esp, [esp + 0ch]
        ret
loc_7_1749:
        mov    esi, dword ptr [edi + 0ch]
        mov    ch, byte ptr [esi + 7]
        shr    ch, 2
        inc    ch
        jmp    SHORT loc_7_16F9
loc_7_1756:
        push   esi
        call   sub_7_106E
        pop    esi
        jae    SHORT loc_7_17B1
        mov    al, 3
        jmp    SHORT loc_7_17CE
loc_7_1763:
        btr    eax, 1fh
        mov    dword ptr [ebp + 18h], ecx
        bts    dword ptr [ebp + 4], 10h
        call   sub_7_04CA
        mov    edi, dword ptr [esp]
        pushfd
        btr    dword ptr [ebp + 4], 10h
        popfd
        jb     SHORT loc_7_1793
        mov    eax, dword ptr [ebp + 100h]
        and    eax, 0ffffffh
        mov    dword ptr [edi + 4], eax
        jmp    NEAR PTR loc_7_1652
loc_7_1793:
        mov    eax, dword ptr [ebp + 0e0h]
        cmp    word ptr [eax], 10h
        mov    al, 9
        jb     SHORT loc_7_17BF
loc_7_17A1:
        mov    al, 3
        jmp    SHORT loc_7_17BF
loc_7_17A5:
        mov    al, 4
        jmp    SHORT loc_7_17BF
loc_7_17A9:
        mov    al, 6
        jmp    SHORT loc_7_17BF
loc_7_17AD:
        mov    al, 7
        jmp    SHORT loc_7_17BF
loc_7_17B1:
        mov    al, 0eh
        test   byte ptr [ebp + 5], 10h
        je     SHORT loc_7_17CE
        mov    al, 1
        jmp    SHORT loc_7_17CE
loc_7_17BD:
        mov    al, 0ah
loc_7_17BF:
        mov    esi, dword ptr [esp + 4]
        and    dword ptr [ebp + 4], 0ffffdfffh
        or     dword ptr [ebp + 58h], 0ffffffffh
loc_7_17CE:
        mov    word ptr [esi + 6], 0
        jmp    SHORT loc_7_17F7
loc_7_17D6:
        mov    al, 0bh
        jmp    SHORT loc_7_17E0
loc_7_17DA:
        mov    al, 3
        jmp    SHORT loc_7_17E0
loc_7_17DE:
        mov    al, 8
loc_7_17E0:
        mov    esi, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        sub    word ptr [esi + 6], cx
        and    dword ptr [ebp + 4], 0ffffdfffh
        or     dword ptr [ebp + 58h], 0ffffffffh
loc_7_17F7:
        movzx  eax, al
        mov    word ptr [esi + 3], ax
        and    dword ptr [ebp + 4], 0ffffe7ffh
        stc
        jmp    NEAR PTR loc_7_1744
loc_7_180B:
        mov    al, 0fh
        jmp    SHORT loc_7_17BF
loc_7_180F:
        cmp    word ptr [ebp + 0b4h], 3
        jb     SHORT loc_7_180B
        xor    ecx, ecx
        push   edi
        mov    esi, dword ptr [edi + 0ch]
        call   sub_7_0DE5
        pop    edi
        jb     SHORT loc_7_180B
        cmp    byte ptr [edi + 0ah], ch
        jne    SHORT loc_7_180B
        bt     dword ptr [edi + 0bh], 2
        sbb    ch, 0
        cmp    byte ptr [ebp + 9bh], ch
        jb     SHORT loc_7_180B
        mov    ecx, dword ptr [ebp + 0fch]
        lea    esi, [ebp + 100h]
loc_7_1848:
        mov    edx, dword ptr [esi]
        add    esi, 4
        xor    eax, eax
        shld   eax, edx, 8
        and    edx, 0ffffffh
        sub    edx, dword ptr [ebp + 60h]
        dec    edx
        and    edx, 3fffh
        add    edx, eax
        cmp    edx, 4000h
        ja     NEAR PTR loc_7_17BD
        loop   loc_7_1848
        push   edi
        call   sub_7_0FB7
        pop    edi
        jae    NEAR PTR loc_7_1647
        jmp    NEAR PTR loc_7_1793
loc_7_1885:
        mov    esi, dword ptr [edi + 0ch]
        mov    edi, esi
        mov    ebx, dword ptr [esi]
        add    esi, 4
loc_7_188F:
        mov    eax, dword ptr [esi]
        add    esi, 4
        xor    ecx, ecx
        shld   ecx, eax, 6
        inc    ecx
        and    eax, 7fffffh
        push   esi
        call   sub_7_2E49
        pop    esi
        jb     NEAR PTR loc_7_17DA
        dec    ebx
        jne    SHORT loc_7_188F
        jmp    NEAR PTR loc_7_16B3
        db  000h, 000h, 000h
sub_7_18B8:
        sub    esp, 8
        cmp    dword ptr [ebp + 8], edx
        jne    SHORT loc_7_18CC
        call   sub_7_19E9
        cmp    edx, edx
        jmp    NEAR PTR loc_7_19E4
loc_7_18CC:
        test   byte ptr [ebp + 4], 80h
        je     SHORT loc_7_18E5
        mov    dword ptr [esp + 4], edx
        call   sub_7_1B8C
        mov    edx, dword ptr [esp + 4]
        jb     NEAR PTR loc_7_19E4
loc_7_18E5:
        movzx  ecx, byte ptr [ebp + 9bh]
        shr    ecx, 3
        mov    edi, edx
        shl    edi, 8
        lea    edi, [ecx + edi + 10000000h]
        mov    ebx, ebp
loc_7_18FD:
        db  08Ah, 025h, 068h, 00Eh, 000h, 000h
        xor    al, al
        dec    edi
        db  0CDh, 020h, 004h, 000h, 08Bh, 004h
        jb     SHORT loc_7_192F
        mov    dword ptr [ebp + ecx*4 + 200h], esi
        loop   loc_7_18FD
        mov    dword ptr [ebp + 8], edx
        or     dword ptr [ebp + 4], 40h
        call   sub_7_19E9
        call   sub_7_1A80
        cmp    eax, eax
        jmp    NEAR PTR loc_7_19E4
loc_7_192F:
        and    dword ptr [ebp + 4], 0ffffffbfh
        mov    dword ptr [esp + 4], edx
        db  0B8h, 0F0h, 00Ch, 000h, 000h
        call   sub_7_490C
        jb     NEAR PTR loc_7_19E4
        movzx  ecx, byte ptr [ebp + 9bh]
        shr    ecx, 3
        mov    dword ptr [esp], ecx
loc_7_1954:
        mov    edi, dword ptr [esp + 4]
        shl    edi, 8
        add    edi, dword ptr [esp]
        or     edi, 10000000h
        dec    edi
        mov    ebx, ebp
loc_7_1967:
        cmp    dword ptr [esp], 0
        je     SHORT loc_7_19B6
        db  08Ah, 025h, 068h, 00Eh, 000h, 000h
        mov    al, 17h
        db  0CDh, 020h, 004h, 000h, 08Bh, 004h
        or     esi, esi
        je     SHORT loc_7_198F
        dec    dword ptr [esp]
        dec    edi
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ebp + ecx*4 + 204h], esi
        jmp    SHORT loc_7_1967
loc_7_198F:
        call   sub_7_1B06
        call   sub_7_1CEA
        jb     SHORT loc_7_19A9
        jne    SHORT loc_7_1954
        db  0B8h, 00Ch, 00Dh, 000h, 000h
        call   sub_7_4974
        jmp    SHORT loc_7_1954
loc_7_19A9:
        db  0B8h, 0F0h, 00Ch, 000h, 000h
        call   sub_7_493D
        stc
        jmp    SHORT loc_7_19E4
loc_7_19B6:
        and    dword ptr [ebp + 4], 0fffff87fh
        or     dword ptr [ebp + 4], 20h
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [ebp + 8], eax
        xor    eax, eax
        mov    dword ptr [ebp + 20h], eax
        mov    dword ptr [ebp + 0ch], eax
        mov    dword ptr [ebp + 10h], eax
        call   sub_7_1A80
        db  0B8h, 0F0h, 00Ch, 000h, 000h
        call   sub_7_493D
        or     esp, esp
loc_7_19E4:
        lea    esp, [esp + 8]
        ret
sub_7_19E9:
        lea    edx, [ebp + 204h]
        movzx  ecx, byte ptr [ebp + 9bh]
        shr    ecx, 3
loc_7_19F9:
        mov    esi, dword ptr [edx + ecx*4 - 4]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        loop   loc_7_19F9
        mov    ecx, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esi + 20h]
        mov    dword ptr [ebp + 0ch], ecx
        mov    dword ptr [ebp + 10h], edx
        test   byte ptr [ebp + 4], 80h
        jne    SHORT loc_7_1A1A
        mov    byte ptr [ebp + 14h], dl
loc_7_1A1A:
        mov    ecx, dword ptr [esi + 18h]
        and    ecx, 780h
        mov    edx, dword ptr [ebp + 4]
        and    edx, 0fffff87fh
        lea    edx, [ecx + edx + 20h]
        mov    dword ptr [ebp + 4], edx
        movzx  ecx, byte ptr [esi + 24h]
        mov    dword ptr [ebp + 20h], ecx
        ret
sub_7_1A3B:
        lea    edx, [ebp + 204h]
        movzx  ecx, byte ptr [ebp + 9bh]
        shr    ecx, 3
        bt     dword ptr [ebp + 4], 7
        sbb    eax, eax
loc_7_1A52:
        mov    esi, dword ptr [edx + ecx*4 - 4]
        mov    byte ptr [esi + 32h], al
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        loop   loc_7_1A52
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 1ch], ecx
        mov    dword ptr [esi + 20h], edx
        xor    ecx, ecx
        xchg   dword ptr [ebp + 20h], ecx
        mov    byte ptr [esi + 24h], cl
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [esi + 18h], eax
        and    dword ptr [ebp + 4], 0ffffffdfh
        ret
sub_7_1A80:
        lea    ebx, [ebp + 204h]
        movzx  ecx, byte ptr [ebp + 9bh]
        shr    ecx, 3
        mov    edi, dword ptr [ebp + 24h]
        call   sub_7_1A99
        ret
sub_7_1A99:
        sub    esp, 8
        cmp    ecx, 1
        jb     SHORT loc_7_1B01
        mov    dword ptr [esp + 4], ecx
loc_7_1AA5:
        mov    eax, dword ptr [ebx + ecx*4 - 4]
        mov    edx, dword ptr [eax + 10h]
        shr    edx, 0ch
        lea    eax, [eax + 14h]
        mov    dword ptr [esp], ecx
        push   0
        push   eax
        push   1
        push   edx
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        cmp    eax, 1
        jb     SHORT loc_7_1B01
        mov    ecx, dword ptr [esp]
        dec    ecx
        jne    SHORT loc_7_1AA5
        mov    ecx, dword ptr [esp + 4]
        shr    edi, 0ch
        add    edi, ecx
loc_7_1AD8:
        dec    edi
        mov    eax, dword ptr [ebx + ecx*4 - 4]
        mov    eax, dword ptr [eax + 14h]
        shr    eax, 0ch
        mov    dword ptr [esp], ecx
        push   20080h
        push   eax
        push   1
        push   edi
        db  0CDh, 020h, 028h, 001h, 001h, 000h
        add    esp, 10h
        mov    ecx, dword ptr [esp]
        loop   loc_7_1AD8
        shl    edi, 0ch
        clc
loc_7_1B01:
        lea    esp, [esp + 8]
        ret
sub_7_1B06:
        push   edx
        xor    ecx, ecx
        mov    edx, ecx
        db  0B8h, 044h, 00Eh, 000h, 000h
loc_7_1B10:
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_7_1B4D
loc_7_1B16:
        test   byte ptr [eax + 4], 80h
        je     SHORT loc_7_1B10
        mov    dword ptr [eax + 28h], edx
        or     ecx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_7_1B16
        jecxz  loc_7_1B4D
        db  00Fh, 0BAh, 02Dh, 0FCh, 00Ch, 000h, 000h, 000h
        jb     SHORT loc_7_1B16
        db  0BEh, 03Ch, 003h, 000h, 000h
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 408h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    ecx
        pop    ebx
        pop    eax
loc_7_1B4D:
        pop    edx
        ret
sub_7_1B4F:
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_7_1B85
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [ebp + 28h], eax
        mov    eax, dword ptr [ebp + 0e0h]
        cmp    word ptr [eax], 10h
        jae    SHORT loc_7_1B79
        db  083h, 03Dh, 00Ch, 00Dh, 000h, 000h, 000h
        jne    SHORT loc_7_1B85
        call   sub_7_1A3B
loc_7_1B78:
        ret
loc_7_1B79:
        xor    eax, eax
        mov    dword ptr [ebp + 8], eax
        and    dword ptr [ebp + 4], 0ffffff7fh
loc_7_1B85:
        call   sub_7_1B8C
        jmp    SHORT loc_7_1B78
sub_7_1B8C:
        test   byte ptr [ebp + 4], 0a0h
        je     SHORT loc_7_1BCA
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_7_1BD6
        test   byte ptr [ebp + 4], 80h
        je     SHORT loc_7_1BC5
        test   byte ptr [ebp + 4], 20h
        jne    SHORT loc_7_1BA9
        call   sub_7_19E9
loc_7_1BA9:
        mov    eax, dword ptr [ebp + 0e0h]
        push   dword ptr [eax]
        mov    esi, dword ptr [ebp + 24h]
        call   sub_7_238A
        pop    edx
        jb     SHORT loc_7_1BD6
        mov    eax, dword ptr [ebp + 0e0h]
        mov    word ptr [eax], dx
loc_7_1BC5:
        call   sub_7_1A3B
loc_7_1BCA:
        db  0B8h, 00Ch, 00Dh, 000h, 000h
        call   sub_7_4996
        clc
loc_7_1BD5:
        ret
loc_7_1BD6:
        pushfd
        call   sub_7_1A3B
        movzx  ecx, byte ptr [ebp + 9bh]
        shr    ecx, 3
loc_7_1BE6:
        mov    esi, 0ffc00000h
        xchg   dword ptr [ebp + ecx*4 + 200h], esi
        push   ecx
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        pop    ecx
        loop   loc_7_1BE6
        db  0B8h, 00Ch, 00Dh, 000h, 000h
        call   sub_7_4996
        popfd
        jmp    SHORT loc_7_1BD5
sub_7_1C09:
        mov    edx, dword ptr [esi + 0ch]
        and    edx, 0fffff00h
        shr    edx, 8
        mov    ecx, dword ptr [esi + 8]
        cmp    dword ptr [ecx + 8], edx
        je     SHORT loc_7_1C1E
loc_7_1C1D:
        ret
loc_7_1C1E:
        xor    dword ptr [ecx + 8], edx
        and    dword ptr [ecx + 4], 0ffffffbfh
        jmp    SHORT loc_7_1C1D
sub_7_1C27:
        db  08Bh, 035h, 094h, 00Dh, 000h, 000h
loc_7_1C2D:
        db  081h, 0FEh, 07Ch, 00Dh, 000h, 000h
        je     SHORT loc_7_1C5D
        mov    eax, dword ptr [esi + 14h]
        mov    edx, dword ptr [esi + 18h]
        mov    dword ptr [eax + 18h], edx
        mov    dword ptr [edx + 14h], eax
        mov    edx, dword ptr [esi + 8]
        mov    eax, esi
        xchg   dword ptr [edx + 0e4h], eax
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [esi + 18h], 0ffffffcbh
        mov    esi, eax
        jmp    SHORT loc_7_1C2D
loc_7_1C5D:
        db  00Fh, 0BAh, 02Dh, 0FCh, 00Ch, 000h, 000h, 001h
        jb     SHORT loc_7_1C83
        xor    edx, edx
        db  0BEh, 0EDh, 003h, 000h, 000h
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 408h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    ecx
        pop    ebx
        pop    eax
loc_7_1C83:
        ret
sub_7_1C84:
        cmp    dword ptr [ebp + 0e4h], 0
        jne    SHORT loc_7_1C8E
loc_7_1C8D:
        ret
loc_7_1C8E:
        sub    esp, 4
        mov    dword ptr [esp], 0
loc_7_1C98:
        mov    esi, dword ptr [ebp + 0e4h]
        or     esi, esi
        je     SHORT loc_7_1CC6
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ebp + 0e4h], eax
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        call   sub_7_2DC6
        jae    SHORT loc_7_1C98
        mov    eax, dword ptr [ebp + 0e0h]
        movzx  eax, word ptr [eax]
        mov    dword ptr [esp], eax
        jmp    SHORT loc_7_1C98
loc_7_1CC6:
        db  0B8h, 000h, 00Dh, 000h, 000h
        call   sub_7_4A17
        mov    eax, dword ptr [esp]
        cmp    eax, 10h
        cmc
        jb     SHORT loc_7_1CDF
loc_7_1CD9:
        lea    esp, [esp + 4]
        jmp    SHORT loc_7_1C8D
loc_7_1CDF:
        mov    edx, dword ptr [ebp + 0e0h]
        mov    word ptr [edx], ax
        jmp    SHORT loc_7_1CD9
sub_7_1CEA:
        db  0A1h, 094h, 00Dh, 000h, 000h
loc_7_1CEF:
        mov    esi, eax
        db  081h, 0FEh, 07Ch, 00Dh, 000h, 000h
        je     SHORT loc_7_1D21
        mov    eax, dword ptr [esi + 18h]
        cmp    dword ptr [esi + 8], ebp
        jne    SHORT loc_7_1CEF
        mov    ebx, dword ptr [esi + 14h]
        mov    ecx, dword ptr [esi + 18h]
        mov    dword ptr [ebx + 18h], ecx
        mov    dword ptr [ecx + 14h], ebx
        mov    ebx, esi
        xchg   dword ptr [ebp + 0e4h], ebx
        mov    dword ptr [esi + 14h], ebx
        mov    dword ptr [esi + 18h], 0ffffffcah
        jmp    SHORT loc_7_1CEF
loc_7_1D21:
        cmp    dword ptr [ebp + 0e4h], 0
        je     SHORT loc_7_1D33
        call   sub_7_1C84
        jb     SHORT loc_7_1D33
        test   esp, esp
loc_7_1D33:
        ret
loc_7_1D34:
        mov    eax, dword ptr [ebp + 0d4h]
        movzx  ecx, word ptr [ebp + 0a4h]
        sub    eax, ecx
        mov    dword ptr [ebp + 0d4h], eax
loc_7_1D49:
        cmp    dword ptr [ebp + 0cch], 0
        je     NEAR PTR loc_7_20A0
        mov    edx, dword ptr [ebp + 0d4h]
        mov    cl, byte ptr [ebp + 8ch]
        shr    edx, cl
        push   edx
        call   sub_7_18B8
        pop    ebx
        jb     NEAR PTR loc_7_20A0
        test   byte ptr [ebp + 4], 40h
        jne    NEAR PTR loc_7_1F9D
        mov    eax, ebx
        call   sub_7_2933
        jb     NEAR PTR loc_7_2092
        mov    dword ptr [ebp + 0ch], eax
        mov    word ptr [ebp + 10h], cx
        test   eax, 80000000h
        jne    SHORT loc_7_1DC2
        test   eax, 0ffffffh
        je     SHORT loc_7_1DB2
        xor    ecx, ecx
        mov    dword ptr [ebp + 8], ecx
        call   sub_7_2BAD
        mov    dword ptr [ebp + 8], ebx
        jne    SHORT loc_7_1DC2
        jb     NEAR PTR loc_7_2092
loc_7_1DB2:
        or     dword ptr [ebp + 4], 140h
        and    dword ptr [ebp + 14h], 0
        and    word ptr [ebp + 10h], 0
loc_7_1DC2:
        test   byte ptr [ebp + 4], 40h
        jne    NEAR PTR loc_7_1F9D
        mov    edi, dword ptr [ebp + 24h]
        test   byte ptr [ebp + 0fh], 40h
        je     NEAR PTR loc_7_1F40
        test   byte ptr [ebp + 0dch], 2
        jne    NEAR PTR loc_7_1EB9
        movzx  ecx, byte ptr [ebp + 9bh]
        cmp    dword ptr [ebp + 0cch], ecx
        jb     NEAR PTR loc_7_1EB9
        dec    ecx
        test   dword ptr [ebp + 0d4h], ecx
        jne    NEAR PTR loc_7_1EB9
        xor    edx, edx
        xchg   dword ptr [ebp + 8], edx
        cmp    byte ptr [ebp + 10h], cl
        jbe    NEAR PTR loc_7_1F3A
        mov    edi, dword ptr [ebp + 0ch]
        and    edi, 0ffffffh
        inc    ecx
        mov    ebx, dword ptr [ebp + 0cch]
loc_7_1E24:
        add    edi, ecx
        sub    ebx, ecx
        cmp    ebx, ecx
        jb     SHORT loc_7_1E87
        inc    edx
        mov    eax, edx
        push   ecx
        push   edx
        push   edi
        call   sub_7_2933
        mov    esi, eax
        pop    edi
        mov    eax, ecx
        pop    edx
        pop    ecx
        jb     NEAR PTR loc_7_2092
        push   esi
        and    esi, 0e0000000h
        cmp    esi, 0c0000000h
        pop    esi
        jne    SHORT loc_7_1E87
        cmp    al, cl
        jne    SHORT loc_7_1E87
        and    esi, 0ffffffh
        cmp    esi, edi
        jne    SHORT loc_7_1E87
        push   ebx
        push   ecx
        push   edx
        push   edi
        mov    edi, edx
        shl    edi, 8
        add    edi, 10000000h
        mov    ebx, ebp
        db  08Ah, 025h, 068h, 00Eh, 000h, 000h
        xor    al, al
        db  0CDh, 020h, 004h, 000h, 08Bh, 004h
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_7_1E24
loc_7_1E87:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffffh
        sub    edi, eax
        cmp    edi, ecx
        jbe    NEAR PTR loc_7_1F3A
        mov    ecx, edi
        sub    dword ptr [ebp + 0cch], edi
        add    dword ptr [ebp + 0d4h], edi
        xor    edi, edi
        call   sub_7_2E50
        jae    NEAR PTR loc_7_1F8E
        jmp    NEAR PTR loc_7_2092
loc_7_1EB9:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, 20000000h
        jne    SHORT loc_7_1F40
        movzx  ecx, byte ptr [ebp + 9bh]
        lea    ebx, [ecx - 1]
        and    ebx, dword ptr [ebp + 0d4h]
        jne    SHORT loc_7_1EDE
        test   byte ptr [ebp + 0dch], 20h
        je     SHORT loc_7_1F40
loc_7_1EDE:
        movzx  edx, byte ptr [ebp + 11h]
        cmp    ecx, edx
        je     SHORT loc_7_1EF0
        add    ebx, dword ptr [ebp + 0cch]
        cmp    ebx, edx
        ja     SHORT loc_7_1F40
loc_7_1EF0:
        mov    edx, dword ptr [ebp + 0d4h]
        dec    ecx
        and    eax, 0ffffffh
        and    edx, ecx
        add    eax, edx
        inc    ecx
        sub    ecx, edx
        cmp    dword ptr [ebp + 0cch], ecx
        ja     SHORT loc_7_1F11
        mov    ecx, dword ptr [ebp + 0cch]
loc_7_1F11:
        xor    edi, edi
        push   ecx
        call   sub_7_2E50
        pop    ecx
        jb     NEAR PTR loc_7_2092
        sub    dword ptr [ebp + 0cch], ecx
        add    dword ptr [ebp + 0d4h], ecx
        and    dword ptr [ebp + 8], 0
        call   sub_7_1B4F
        jmp    NEAR PTR loc_7_1D49
loc_7_1F3A:
        mov    edi, dword ptr [ebp + 0d0h]
loc_7_1F40:
        push   edi
        call   sub_7_2AA1
        pop    edi
        jb     NEAR PTR loc_7_2092
        test   byte ptr [ebp + 0fh], 40h
        je     SHORT loc_7_1F9D
        mov    edx, dword ptr [ebp + 14h]
        shl    edx, 9
        add    edi, edx
        cmp    dword ptr [ebp + 8], 0
        jne    SHORT loc_7_1F9D
loc_7_1F61:
        movzx  ecx, byte ptr [ebp + 9bh]
        mov    edx, dword ptr [ebp + 14h]
        sub    ecx, edx
        xor    eax, eax
        shl    ecx, 7
        cld
        rep stosd
        mov    dword ptr [ebp + 0d0h], edi
        movzx  ecx, byte ptr [ebp + 9bh]
        add    dword ptr [ebp + 0d4h], ecx
        sub    dword ptr [ebp + 0cch], ecx
loc_7_1F8E:
        call   sub_7_1B4F
        call   sub_7_20A1
        jmp    NEAR PTR loc_7_1D49
loc_7_1F9D:
        test   byte ptr [ebp + 5], 1
        jne    SHORT loc_7_2012
        mov    edi, dword ptr [ebp + 0d0h]
        test   byte ptr [ebp + 0dch], 2
        jne    SHORT loc_7_1FCA
        movzx  ecx, byte ptr [ebp + 9bh]
        cmp    dword ptr [ebp + 0cch], ecx
        jb     SHORT loc_7_1FCA
        dec    ecx
        test   dword ptr [ebp + 0d4h], ecx
        je     SHORT loc_7_1FD5
loc_7_1FCA:
        db  0E8h, 07Dh, 028h, 000h, 000h
        jb     NEAR PTR loc_7_2092
loc_7_1FD5:
        mov    ecx, dword ptr [ebp + 14h]
        push   edi
        call   sub_7_281E
        pop    edi
        jb     NEAR PTR loc_7_2087
        cmp    dword ptr [ebp + 0d0h], edi
        je     SHORT loc_7_2000
        call   sub_7_48CC
        and    dword ptr [ebp + 20h], 0
        mov    edi, dword ptr [ebp + 24h]
        or     dword ptr [ebp + 4], 100h
loc_7_2000:
        mov    edx, dword ptr [ebp + 14h]
        shl    edx, 9
        add    edi, edx
        test   byte ptr [ebp + 5], 1
        je     NEAR PTR loc_7_1F61
loc_7_2012:
        movzx  ecx, byte ptr [ebp + 9bh]
        mov    esi, dword ptr [ebp + 0d4h]
        dec    ecx
        and    esi, ecx
        inc    ecx
        sub    ecx, esi
        cmp    dword ptr [ebp + 0cch], ecx
        ja     SHORT loc_7_2033
        mov    ecx, dword ptr [ebp + 0cch]
loc_7_2033:
        mov    edx, ecx
        add    ecx, esi
        mov    edi, dword ptr [ebp + 14h]
        sub    ecx, edi
        jbe    SHORT loc_7_204C
        shl    edi, 9
        shl    ecx, 7
        add    edi, dword ptr [ebp + 24h]
        xor    eax, eax
        cld
        rep stosd
loc_7_204C:
        mov    ecx, edx
        mov    edi, dword ptr [ebp + 24h]
        push   ecx
        call   sub_7_2859
        call   sub_7_1B4F
        pop    ecx
        add    dword ptr [ebp + 0d4h], ecx
        sub    dword ptr [ebp + 0cch], ecx
        movzx  ecx, byte ptr [ebp + 9bh]
        dec    ecx
        test   dword ptr [ebp + 0d4h], ecx
        jne    NEAR PTR loc_7_1D49
        call   sub_7_20A1
        jmp    NEAR PTR loc_7_1D49
loc_7_2087:
        mov    eax, dword ptr [ebp + 0e0h]
        mov    word ptr [eax], 14h
loc_7_2092:
        and    dword ptr [ebp + 8], 0
        call   sub_7_1B4F
        call   sub_7_489D
loc_7_20A0:
        ret
sub_7_20A1:
        test   byte ptr [ebp + 4], 80h
        jne    SHORT loc_7_20D4
        xor    edx, edx
        xchg   dword ptr [ebp + 8], edx
        or     edx, edx
        je     SHORT loc_7_20D4
        and    dword ptr [ebp + 4], 0ffffffbfh
        movzx  ecx, byte ptr [ebp + 9bh]
        shr    ecx, 3
loc_7_20BE:
        mov    esi, dword ptr [ebp + ecx*4 + 200h]
        push   ecx
        call   sub_7_1C09
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        pop    ecx
        loop   loc_7_20BE
loc_7_20D4:
        ret
loc_7_20D5:
        mov    eax, dword ptr [ebp + 0d4h]
        movzx  ecx, word ptr [ebp + 0a4h]
        sub    eax, ecx
        mov    dword ptr [ebp + 0d4h], eax
        db  083h, 03Dh, 02Ch, 00Eh, 000h, 000h, 000h
        jne    SHORT loc_7_20FB
        bts    dword ptr [ebp + 0dch], 18h
loc_7_20FB:
        cmp    dword ptr [ebp + 0cch], 0
        je     NEAR PTR loc_7_2389
        mov    edx, dword ptr [ebp + 0d4h]
        mov    cl, byte ptr [ebp + 8ch]
        shr    edx, cl
        push   edx
        call   sub_7_18B8
        pop    ebx
        jb     NEAR PTR loc_7_2389
        btr    dword ptr [ebp + 4], 0ah
        bt     dword ptr [ebp + 0dch], 18h
        sbb    eax, eax
        and    eax, 400h
        or     dword ptr [ebp + 4], eax
        test   byte ptr [ebp + 4], 40h
        jne    NEAR PTR loc_7_21E5
        mov    eax, ebx
        call   sub_7_2933
        jb     NEAR PTR loc_7_237B
        mov    dword ptr [ebp + 0ch], eax
        mov    word ptr [ebp + 10h], cx
        test   byte ptr [ebp + 0dfh], 1
        je     NEAR PTR loc_7_21E5
        mov    edx, eax
        and    edx, 0e0000000h
        cmp    edx, 0c0000000h
        jne    SHORT loc_7_21E5
        movzx  edx, byte ptr [ebp + 9bh]
        cmp    dl, ch
        je     SHORT loc_7_2194
        dec    edx
        and    edx, dword ptr [ebp + 0d4h]
        add    edx, dword ptr [ebp + 0cch]
        movzx  edi, ch
        cmp    edx, edi
        ja     SHORT loc_7_21E5
loc_7_2194:
        movzx  ecx, byte ptr [ebp + 9bh]
        mov    edx, dword ptr [ebp + 0d4h]
        dec    ecx
        and    eax, 0ffffffh
        and    edx, ecx
        add    eax, edx
        inc    ecx
        sub    ecx, edx
        cmp    dword ptr [ebp + 0cch], ecx
        ja     SHORT loc_7_21BC
        mov    ecx, dword ptr [ebp + 0cch]
loc_7_21BC:
        xor    edi, edi
        push   ecx
        call   sub_7_2E49
        pop    ecx
        jb     NEAR PTR loc_7_237B
        sub    dword ptr [ebp + 0cch], ecx
        add    dword ptr [ebp + 0d4h], ecx
        and    dword ptr [ebp + 8], 0
        call   sub_7_1B4F
        jmp    NEAR PTR loc_7_20FB
loc_7_21E5:
        test   byte ptr [ebp + 0dch], 2
        jne    SHORT loc_7_2252
        movzx  ecx, byte ptr [ebp + 9bh]
        cmp    dword ptr [ebp + 0cch], ecx
        jb     SHORT loc_7_2252
        dec    ecx
        test   dword ptr [ebp + 0d4h], ecx
        jne    SHORT loc_7_2252
        inc    ecx
        mov    dword ptr [ebp + 14h], ecx
        mov    esi, dword ptr [ebp + 0d0h]
        or     dword ptr [ebp + 4], 180h
        call   sub_7_238A
        jb     NEAR PTR loc_7_237B
        test   byte ptr [ebp + 5], 1
        je     SHORT loc_7_222C
        and    dword ptr [ebp + 8], 0
loc_7_222C:
        movzx  ecx, byte ptr [ebp + 9bh]
        add    dword ptr [ebp + 0d4h], ecx
        sub    dword ptr [ebp + 0cch], ecx
        shl    ecx, 9
        add    dword ptr [ebp + 0d0h], ecx
        call   sub_7_1B4F
        jmp    NEAR PTR loc_7_20FB
loc_7_2252:
        test   byte ptr [ebp + 4], 40h
        jne    SHORT loc_7_2288
        movzx  ecx, byte ptr [ebp + 9bh]
        cmp    dword ptr [ebp + 0cch], ecx
        jb     SHORT loc_7_2270
        dec    ecx
        test   dword ptr [ebp + 0d4h], ecx
        je     SHORT loc_7_2288
loc_7_2270:
        and    dword ptr [ebp + 14h], 0
        test   byte ptr [ebp + 0fh], 80h
        je     SHORT loc_7_2288
        mov    edi, dword ptr [ebp + 24h]
        call   sub_7_2AA1
        jb     NEAR PTR loc_7_237B
loc_7_2288:
        movzx  ecx, byte ptr [ebp + 9bh]
        mov    esi, dword ptr [ebp + 0d4h]
        dec    ecx
        and    esi, ecx
        inc    ecx
        sub    ecx, esi
        cmp    dword ptr [ebp + 0cch], ecx
        ja     SHORT loc_7_22A9
        mov    ecx, dword ptr [ebp + 0cch]
loc_7_22A9:
        mov    edx, dword ptr [ebp + 14h]
        cmp    edx, esi
        jb     SHORT loc_7_22BD
        lea    eax, [esi + ecx]
        cmp    byte ptr [ebp + 9bh], al
        ja     SHORT loc_7_22BD
        mov    edx, esi
loc_7_22BD:
        or     edx, edx
        je     SHORT loc_7_22F0
        test   byte ptr [ebp + 5], 1
        jne    SHORT loc_7_22F0
        push   esi
        push   ecx
        push   edx
        push   edx
        db  0E8h, 07Ch, 025h, 000h, 000h
        pop    ecx
        jb     SHORT loc_7_22D8
        call   sub_7_281E
loc_7_22D8:
        pop    edx
        pop    ecx
        pop    esi
        jb     NEAR PTR loc_7_236A
        push   ecx
        push   edx
        push   esi
        call   sub_7_48CC
        pop    esi
        pop    edx
        pop    ecx
        and    dword ptr [ebp + 20h], 0
loc_7_22F0:
        cmp    edx, esi
        jae    SHORT loc_7_230C
        mov    edi, edx
        shl    edi, 9
        add    edi, dword ptr [ebp + 24h]
        xchg   ecx, edx
        neg    ecx
        add    ecx, esi
        shl    ecx, 7
        xor    eax, eax
        cld
        rep stosd
        mov    ecx, edx
loc_7_230C:
        push   ecx
        push   esi
        mov    eax, esi
        call   sub_7_28C2
        pop    esi
        pop    ecx
        or     dword ptr [ebp + 4], 1c0h
        add    dword ptr [ebp + 0d4h], ecx
        sub    dword ptr [ebp + 0cch], ecx
        add    ecx, esi
        cmp    dword ptr [ebp + 14h], ecx
        ja     SHORT loc_7_2334
        mov    dword ptr [ebp + 14h], ecx
loc_7_2334:
        cmp    dword ptr [ebp + 58h], -1
        je     SHORT loc_7_2356
        movzx  eax, byte ptr [ebp + 9bh]
        cmp    dword ptr [ebp + 58h], eax
        jbe    SHORT loc_7_2356
        cmp    word ptr [ebp + 0b4h], 3
        jae    SHORT loc_7_2360
        cmp    dword ptr [ebp + 64h], 0
        ja     SHORT loc_7_2360
loc_7_2356:
        mov    esi, dword ptr [ebp + 24h]
        call   sub_7_238A
        jb     SHORT loc_7_237B
loc_7_2360:
        call   sub_7_1B4F
        jmp    NEAR PTR loc_7_20FB
loc_7_236A:
        mov    eax, dword ptr [ebp + 0e0h]
        cmp    word ptr [eax], 10h
        jae    SHORT loc_7_237B
        mov    word ptr [eax], 14h
loc_7_237B:
        and    dword ptr [ebp + 8], 0
        call   sub_7_1B4F
        call   sub_7_489D
loc_7_2389:
        ret
sub_7_238A:
        mov    ebx, dword ptr [ebp + 24h]
        xor    ebx, esi
        or     dword ptr [ebp + 4], 200h
        and    dword ptr [ebp + 4], 0ffffff7fh
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebp + 18h], eax
        mov    edi, esi
        test   byte ptr [ebp + 5], 4
        jne    SHORT loc_7_23DC
        push   ebx
        db  0E8h, 09Bh, 024h, 000h, 000h
        pop    ebx
        jb     NEAR PTR loc_7_2559
        push   esi
        call   sub_7_2722
        pop    edi
        jb     SHORT loc_7_23D7
        call   sub_7_48CC
        mov    edi, dword ptr [ebp + 24h]
        and    dword ptr [ebp + 4], 0fffffcffh
        bts    dword ptr [ebp + 4], 6
        jmp    SHORT loc_7_23DC
loc_7_23D7:
        call   sub_7_489D
loc_7_23DC:
        test   byte ptr [ebp + 0dfh], 2
        je     SHORT loc_7_242F
        test   byte ptr [ebp + 0fh], 80h
        jne    SHORT loc_7_242F
        test   byte ptr [ebp + 0fh], 20h
        jne    SHORT loc_7_242F
        test   dword ptr [ebp + 0ch], 0ffffffh
        je     SHORT loc_7_242F
        mov    eax, dword ptr [ebp + 18h]
        cmp    byte ptr [ebp + 11h], al
        jne    SHORT loc_7_242F
        mov    eax, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ebp + 10h]
        push   ebx
        push   edi
        xor    ebx, ebx
        call   sub_7_0F7C
        pop    edi
        pop    ebx
        jb     SHORT loc_7_242F
        push   edi
        call   sub_7_0EFA
        pop    edi
        jb     NEAR PTR loc_7_2559
        or     dword ptr [ebp + 0ch], 80000000h
        jmp    NEAR PTR loc_7_24BF
loc_7_242F:
        mov    eax, dword ptr [ebp + 18h]
        xor    ecx, ecx
        test   byte ptr [ebp + 0fh], 80h
        je     SHORT loc_7_2449
        test   byte ptr [ebp + 0fh], 20h
        jne    SHORT loc_7_245A
        cmp    byte ptr [ebp + 11h], al
        je     SHORT loc_7_24BF
        movzx  ecx, byte ptr [ebp + 11h]
loc_7_2449:
        cmp    dword ptr [ebp + 58h], -1
        je     SHORT loc_7_245A
        add    ecx, dword ptr [ebp + 58h]
        cmp    ecx, eax
        jbe    NEAR PTR loc_7_254E
loc_7_245A:
        test   byte ptr [ebp + 0fh], 80h
        je     SHORT loc_7_2476
        mov    eax, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ebp + 10h]
        push   ebx
        push   edi
        call   sub_7_0E90
        pop    edi
        pop    ebx
        jb     NEAR PTR loc_7_2559
loc_7_2476:
        push   edi
        call   sub_7_04C8
        pop    edi
        jae    SHORT loc_7_24D7
        test   byte ptr [ebp + 0fh], 80h
        je     SHORT loc_7_24AA
        mov    eax, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ebp + 10h]
        push   ebx
        push   edi
        xor    ebx, ebx
        call   sub_7_0CB0
        pop    edi
        pop    ebx
        jb     NEAR PTR loc_7_2559
        push   edi
        call   sub_7_0EFA
        pop    edi
        jb     NEAR PTR loc_7_2559
loc_7_24AA:
        mov    eax, dword ptr [ebp + 0e0h]
        cmp    word ptr [eax], 10h
        jae    NEAR PTR loc_7_2559
        jmp    NEAR PTR loc_7_2554
loc_7_24BF:
        mov    eax, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ebp + 10h]
        push   ebx
        push   edi
        xor    ebx, ebx
        call   sub_7_0CB0
        pop    edi
        pop    ebx
        jb     NEAR PTR loc_7_2559
loc_7_24D7:
        push   ebx
        push   edi
        call   sub_7_259A
        pop    edi
        pop    ebx
        jb     NEAR PTR loc_7_2567
        mov    eax, dword ptr [ebp + 100h]
        shld   ecx, eax, 10h
        mov    cl, byte ptr [ebp + 14h]
        mov    word ptr [ebp + 10h], cx
        and    eax, 0ffffffh
        bt     dword ptr [ebp + 4], 9
        sbb    edx, edx
        and    edx, 40000000h
        or     eax, edx
        cmp    dword ptr [ebp + 0fch], 2
        cmc
        sbb    edx, edx
        and    edx, 20000000h
        or     eax, edx
        or     eax, 80000000h
        mov    dword ptr [ebp + 0ch], eax
        mov    edx, dword ptr [ebp + 8]
        call   sub_7_29FE
        jb     SHORT loc_7_2559
        or     ebx, ebx
        jne    SHORT loc_7_254C
        test   byte ptr [ebp + 5], 2
        jne    SHORT loc_7_254C
        call   sub_7_48BD
        jb     SHORT loc_7_254C
        call   sub_7_48CC
        or     dword ptr [ebp + 4], 100h
loc_7_254C:
        clc
loc_7_254D:
        ret
loc_7_254E:
        mov    eax, dword ptr [ebp + 0e0h]
loc_7_2554:
        mov    word ptr [eax], 2bh
loc_7_2559:
        and    dword ptr [ebp + 8], 0
        and    dword ptr [ebp + 4], 0fffff87fh
        stc
        jmp    SHORT loc_7_254D
loc_7_2567:
        and    dword ptr [ebp + 0ch], 0
        mov    eax, dword ptr [ebp + 0e0h]
        movzx  eax, word ptr [eax]
        cmp    eax, 0ffh
        ja     SHORT loc_7_2559
        push   edi
        db  0BFh, 091h, 025h, 000h, 000h
        mov    ecx, 9
        cld
        repne scasb
        pop    edi
        jne    SHORT loc_7_2559
        jmp    NEAR PTR loc_7_23DC
loc_7_2591:
        adc    byte ptr [edx + 1e1d1b1ah], dl
        and    byte ptr [ecx], ah
sub_7_259A:
        cmp    dword ptr [ebp + 0fch], 1
        ja     SHORT loc_7_25BA
        mov    eax, dword ptr [ebp + 100h]
        xor    ecx, ecx
        shld   ecx, eax, 8
        and    eax, 0ffffffh
        call   sub_7_2E49
loc_7_25B9:
        ret
loc_7_25BA:
        test   byte ptr [ebp + 4], 20h
        je     SHORT loc_7_25D5
        movzx  esi, byte ptr [ebp + 9bh]
        cmp    dword ptr [ebp + 18h], esi
        jae    SHORT loc_7_25D5
        dec    esi
        shl    esi, 9
        add    esi, dword ptr [ebp + 24h]
        jmp    SHORT loc_7_25E0
loc_7_25D5:
        push   edi
        db  0E8h, 071h, 022h, 000h, 000h
        mov    esi, edi
        pop    edi
        jb     SHORT loc_7_25B9
loc_7_25E0:
        call   sub_7_26EB
        test   byte ptr [ebp + 5], 1
        je     SHORT loc_7_25F9
        mov    edx, 45544550h
loc_7_25F0:
        mov    dword ptr [eax], edx
        add    eax, 4
        loop   loc_7_25F0
        jmp    SHORT loc_7_2605
loc_7_25F9:
        mov    edx, dword ptr [edi]
        add    edi, 4
        mov    dword ptr [eax], edx
        add    eax, 4
        loop   loc_7_25F9
loc_7_2605:
        movzx  ecx, byte ptr [ebp + 103h]
        dec    ecx
        je     SHORT loc_7_2684
        push   dword ptr [ebp + 0d0h]
        push   dword ptr [ebp + 0c8h]
        push   dword ptr [ebp + 0dch]
        lea    edx, [ebp + 224h]
        mov    dword ptr [ebp + 0d0h], edx
        xor    eax, eax
        mov    dword ptr [ebp + 0c8h], eax
        or     dword ptr [ebp + 0dch], 2
        mov    dword ptr [edx + 10h], eax
        inc    eax
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], esi
        mov    dword ptr [edx + 8], ecx
        mov    dword ptr [edx + 0ch], edi
        shl    ecx, 9
        add    edi, ecx
        mov    eax, dword ptr [ebp + 100h]
        rol    eax, 8
        movzx  ecx, al
        shr    eax, 8
        push   edi
        push   esi
        xor    edi, edi
        call   sub_7_2E49
        pop    esi
        pop    edi
        pop    dword ptr [ebp + 0dch]
        pop    dword ptr [ebp + 0c8h]
        pop    dword ptr [ebp + 0d0h]
        jb     NEAR PTR loc_7_25B9
        jmp    SHORT loc_7_26A5
loc_7_2684:
        mov    eax, dword ptr [ebp + 100h]
        and    eax, 0ffffffh
        mov    ecx, 1
        push   edi
        mov    edi, esi
        push   esi
        call   sub_7_2E49
        pop    esi
        pop    edi
        jb     NEAR PTR loc_7_25B9
loc_7_26A5:
        movzx  ecx, byte ptr [ebp + 9bh]
        dec    ecx
        shl    ecx, 9
        add    ecx, dword ptr [ebp + 24h]
        cmp    ecx, esi
        je     SHORT loc_7_26BC
        call   sub_7_489D
loc_7_26BC:
        mov    ecx, dword ptr [ebp + 0fch]
        dec    ecx
        lea    ebx, [ebp + 104h]
loc_7_26C9:
        push   ecx
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        rol    eax, 8
        movzx  ecx, al
        shr    eax, 8
        call   sub_7_2E49
        pop    ecx
        jb     NEAR PTR loc_7_25B9
        loop   loc_7_26C9
        jmp    NEAR PTR loc_7_25B9
sub_7_26EB:
        lea    ebx, [ebp + 0fch]
        mov    ecx, dword ptr [ebx]
        push   esi
        add    ebx, 4
        mov    dword ptr [esi], ecx
        add    esi, 4
loc_7_26FC:
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        rol    eax, 8
        dec    al
        shl    al, 2
        ror    eax, 8
        mov    dword ptr [esi], eax
        add    esi, 4
        loop   loc_7_26FC
        mov    eax, esi
        pop    esi
        lea    ecx, [esi + 200h]
        sub    ecx, eax
        shr    ecx, 2
        ret
sub_7_2722:
        push   ebx
        sub    esp, 14h
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ebp + 18h], ecx
        cmp    ecx, 2
        jb     NEAR PTR loc_7_27B6
        mov    dword ptr [esp], esi
        mov    dword ptr [esp + 8], edi
        movzx  eax, byte ptr [ebp + 9bh]
        shl    eax, 9
        mov    word ptr [esp + 0ch], ax
        shl    ecx, 9
        mov    word ptr [esp + 4], cx
        movzx  edx, word ptr [ebp + 0b4h]
        db  0BEh, 008h, 00Dh, 000h, 000h
loc_7_275F:
        add    esi, 10h
        cmp    dword ptr [esi], edx
        jne    SHORT loc_7_275F
        db  08Bh, 015h, 02Ch, 00Eh, 000h, 000h
        dec    edx
        je     SHORT loc_7_27BC
loc_7_276F:
        db  0B8h, 058h, 00Dh, 000h, 000h
        call   sub_7_490C
        jb     SHORT loc_7_27B6
        mov    eax, dword ptr [esi + edx*4]
loc_7_277E:
        lea    esi, [esp]
        push   ebp
        db  08Bh, 01Dh, 03Ch, 00Eh, 000h, 000h
        call   dword ptr [ebx + 8]
        pop    ebp
        cmp    eax, 1
        cmc
        jb     SHORT loc_7_27CC
loc_7_2792:
        movzx  edx, word ptr [esp + 0ch]
        mov    dword ptr [ebp + 1ch], edx
        add    edx, 1ffh
        shr    edx, 9
        mov    dword ptr [ebp + 18h], edx
        inc    edx
        cmp    dword ptr [ebp + 14h], edx
loc_7_27AA:
        pushfd
        db  0B8h, 058h, 00Dh, 000h, 000h
        call   sub_7_493D
        popfd
loc_7_27B6:
        lea    esp, [esp + 14h]
        pop    ebx
        ret
loc_7_27BC:
        mov    ecx, dword ptr [ebp + 58h]
        cmp    ecx, -1
        je     SHORT loc_7_27C9
        cmp    dword ptr [ebp + 5ch], ecx
        jb     SHORT loc_7_27B6
loc_7_27C9:
        inc    edx
        jmp    SHORT loc_7_276F
loc_7_27CC:
        cmp    eax, 9
        je     SHORT loc_7_27DE
        cmp    eax, 8
        mov    eax, 40h
        je     SHORT loc_7_277E
        stc
        jmp    SHORT loc_7_27AA
loc_7_27DE:
        push   edx
        mov    eax, 10h
        mov    edx, ebp
        db  0BEh, 003h, 028h, 000h, 000h
        cli
        db  0CDh, 020h, 0FEh, 000h, 001h, 000h
        pop    edx
        call   sub_7_4A6C
        lea    esi, [esp]
        call   edx
        or     eax, eax
        je     SHORT loc_7_2792
        jmp    SHORT loc_7_27CC
loc_7_2803:
        db  0BEh, 0EDh, 049h, 000h, 000h
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 408h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    ecx
        pop    ebx
        pop    eax
        ret
sub_7_281E:
        push   ebx
        sub    esp, 14h
        mov    esi, dword ptr [ebp + 24h]
        add    esi, dword ptr [ebp + 20h]
        shl    ecx, 9
        mov    word ptr [esp + 0ch], cx
        mov    dword ptr [esp + 8], edi
        mov    dword ptr [esp], esi
        and    dword ptr [esp + 10h], 0
        mov    eax, 20h
        lea    esi, [esp]
        push   ebp
        db  08Bh, 01Dh, 03Ch, 00Eh, 000h, 000h
        call   dword ptr [ebx + 8]
        pop    ebp
        cmp    eax, 1
        cmc
        lea    esp, [esp + 14h]
        pop    ebx
        ret
sub_7_2859:
        cld
        shl    esi, 9
        add    esi, edi
        test   byte ptr [ebp + 0dch], 2
        jne    SHORT loc_7_287A
        mov    edi, dword ptr [ebp + 0d0h]
        shl    ecx, 7
        rep movsd
        mov    dword ptr [ebp + 0d0h], edi
        ret
loc_7_287A:
        mov    eax, dword ptr [ebp + 0d0h]
        mov    edi, dword ptr [ebp + 0c8h]
        mov    edx, dword ptr [eax]
        sub    edx, edi
        push   ecx
        cmp    edx, ecx
        ja     SHORT loc_7_2891
        mov    ecx, edx
loc_7_2891:
        add    dword ptr [ebp + 0c8h], ecx
        shl    edi, 9
        add    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [ebp + 0c8h], eax
        jb     SHORT loc_7_28B5
        add    dword ptr [ebp + 0d0h], 8
        and    dword ptr [ebp + 0c8h], 0
loc_7_28B5:
        mov    eax, ecx
        shl    ecx, 7
        rep movsd
        pop    ecx
        sub    ecx, eax
        ja     SHORT loc_7_287A
        ret
sub_7_28C2:
        mov    esi, dword ptr [ebp + 24h]
sub_7_28C5:
        cld
        mov    edi, eax
        shl    edi, 9
        add    edi, esi
        test   dword ptr [ebp + 0dch], 2
        jne    SHORT loc_7_28EB
        mov    esi, dword ptr [ebp + 0d0h]
        shl    ecx, 7
        rep movsd
        mov    dword ptr [ebp + 0d0h], esi
        ret
loc_7_28EB:
        mov    eax, dword ptr [ebp + 0d0h]
        mov    esi, dword ptr [ebp + 0c8h]
        mov    edx, dword ptr [eax]
        sub    edx, esi
        push   ecx
        cmp    edx, ecx
        ja     SHORT loc_7_2902
        mov    ecx, edx
loc_7_2902:
        add    dword ptr [ebp + 0c8h], ecx
        shl    esi, 9
        add    esi, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [ebp + 0c8h], eax
        jb     SHORT loc_7_2926
        add    dword ptr [ebp + 0d0h], 8
        and    dword ptr [ebp + 0c8h], 0
loc_7_2926:
        mov    eax, ecx
        shl    ecx, 7
        rep movsd
        pop    ecx
        sub    ecx, eax
        ja     SHORT loc_7_28EB
        ret
sub_7_2933:
        cmp    word ptr [ebp + 0b4h], 3
        jae    SHORT loc_7_2996
        push   eax
        push   ebx
        shr    eax, 0ah
        shl    eax, 3
        add    eax, dword ptr [ebp + 68h]
        lea    ebx, [ebp + 30h]
        mov    ecx, 8
        call   sub_7_2C95
        pop    ebx
        pop    edx
        jb     SHORT loc_7_2982
        and    edx, 3ffh
        shl    edx, 2
        add    edx, eax
        mov    eax, dword ptr [edx]
        btr    eax, 15h
        shld   ecx, eax, 0ah
        mov    ch, cl
        shr    cl, 4
        and    ch, 0fh
        add    cx, 101h
        and    eax, 0c01fffffh
loc_7_2980:
        ret
loc_7_2981:
        pop    ebx
loc_7_2982:
        call   sub_7_43D3
        jb     SHORT loc_7_2980
        mov    esi, 3
        call   sub_7_4B5E
        stc
        jmp    SHORT loc_7_2980
loc_7_2996:
        push   ebx
        xor    edx, edx
        mov    ecx, 330h
        div    ecx
        shl    eax, 3
        add    eax, dword ptr [ebp + 68h]
        lea    ebx, [ebp + 30h]
        mov    ecx, 8
        push   edx
        call   sub_7_2C95
        mov    ebx, eax
        pop    eax
        jb     SHORT loc_7_2981
        xor    edx, edx
        mov    ecx, 66h
        div    ecx
        shl    eax, 9
        add    ebx, eax
        lea    edx, [edx + edx*4]
        add    edx, ebx
        mov    eax, dword ptr [edx]
        mov    cx, word ptr [edx + 3]
        xchg   ch, cl
        and    eax, 0ff7fffffh
        shl    eax, 8
        mov    al, cl
        and    cl, 3fh
        ror    eax, 8
        and    eax, 0c07fffffh
        shr    ch, 2
        sbb    ebx, ebx
        and    ebx, 20000000h
        add    cx, 101h
        or     eax, ebx
        pop    ebx
        jmp    SHORT loc_7_2980
sub_7_29FE:
        sub    cx, 101h
        cmp    word ptr [ebp + 0b4h], 3
        jae    SHORT loc_7_2A68
        shl    cl, 4
        or     cl, ch
        rol    eax, 0ah
        mov    al, cl
        ror    eax, 0ah
        push   eax
        mov    eax, edx
        call   sub_7_2933
        pop    eax
        jb     SHORT loc_7_2A43
        cmp    dword ptr [edx], eax
        je     SHORT loc_7_2A43
loc_7_2A29:
        or     dword ptr [ebp + 4], 1000h
        mov    dword ptr [edx], eax
        sub    edx, dword ptr [esi + 10h]
        shr    edx, 9
        cmp    byte ptr [esi + 32h], 0
        je     SHORT loc_7_2A44
loc_7_2A3E:
        bts    dword ptr [esi + 32h], edx
        clc
loc_7_2A43:
        ret
loc_7_2A44:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [esi + 24h], eax
        db  08Bh, 00Dh, 090h, 00Dh, 000h, 000h
        mov    dword ptr [esi + 14h], ecx
        db  0C7h, 046h, 018h, 07Ch, 00Dh, 000h, 000h
        mov    dword ptr [ecx + 18h], esi
        db  089h, 035h, 090h, 00Dh, 000h, 000h
        jmp    SHORT loc_7_2A3E
loc_7_2A68:
        xor    esi, esi
        btr    eax, 1dh
        rcl    esi, 1
        btr    eax, 1ch
        rcl    esi, 19h
        rol    eax, 8
        or     cl, al
        shl    ch, 2
        mov    al, ch
        ror    eax, 8
        or     eax, esi
        push   eax
        push   ecx
        mov    eax, edx
        call   sub_7_2933
        pop    ecx
        pop    eax
        jb     SHORT loc_7_2A3E
        cmp    dword ptr [edx], eax
        jne    SHORT loc_7_2A9C
        cmp    byte ptr [edx + 4], cl
        je     SHORT loc_7_2A43
loc_7_2A9C:
        mov    byte ptr [edx + 4], cl
        jmp    SHORT loc_7_2A29
sub_7_2AA1:
        xor    edx, edx
        mov    dword ptr [ebp + 20h], edx
        mov    cx, word ptr [ebp + 10h]
        mov    dl, cl
        mov    dword ptr [ebp + 14h], edx
        mov    eax, dword ptr [ebp + 0ch]
        bt     eax, 1eh
        sbb    edx, edx
        and    dword ptr [ebp + 4], 0fffffeffh
        and    edx, 100h
        or     dword ptr [ebp + 4], edx
        push   ebx
        push   edi
        mov    ebx, edi
        call   sub_7_0CB0
        pop    edi
        pop    ebx
        jb     SHORT loc_7_2B2F
        mov    eax, dword ptr [ebp + 0fch]
        dec    eax
        jne    SHORT loc_7_2B4B
        mov    edx, dword ptr [ebp + 100h]
        shld   eax, edx, 8
        mov    dword ptr [ebp + 18h], eax
        mov    ecx, eax
        mov    eax, edx
        and    eax, 0ffffffh
        push   edi
        call   sub_7_2E50
        pop    edi
        jb     SHORT loc_7_2B2E
        test   byte ptr [ebp + 5], 1
        jne    SHORT loc_7_2B18
        cmp    word ptr [edi], 5153h
        je     SHORT loc_7_2B18
        cmp    word ptr [edi], 4d4ah
        je     SHORT loc_7_2B18
        cmp    word ptr [edi], 5344h
        jne    SHORT loc_7_2B2F
loc_7_2B18:
        or     dword ptr [ebp + 4], 40h
        test   byte ptr [ebp + 5], 1
        je     SHORT loc_7_2B2E
        mov    eax, dword ptr [ebp + 18h]
        cmp    eax, dword ptr [ebp + 14h]
        ja     SHORT loc_7_2B2E
        mov    dword ptr [ebp + 14h], eax
        clc
loc_7_2B2E:
        ret
loc_7_2B2F:
        test   byte ptr [ebp + 0fh], 80h
        stc
        je     SHORT loc_7_2B2E
        mov    edx, dword ptr [ebp + 0e0h]
        cmp    word ptr [edx], 10h
        cmc
        jb     SHORT loc_7_2B2E
        mov    word ptr [edx], 14h
        stc
        jmp    SHORT loc_7_2B2E
loc_7_2B4B:
        mov    ecx, eax
        movzx  edx, byte ptr [ebp + 103h]
        dec    edx
        test   byte ptr [ebp + 0fh], 40h
        jne    SHORT loc_7_2B7D
        lea    eax, [eax*4 + 8]
        mov    dword ptr [ebp + 20h], eax
        inc    edx
        add    eax, edi
        cmp    word ptr [eax], 4d4ah
        je     SHORT loc_7_2B7D
        cmp    word ptr [eax], 5344h
        je     SHORT loc_7_2B7D
        cmp    word ptr [eax], 5153h
        jne    SHORT loc_7_2B2F
loc_7_2B7D:
        mov    dword ptr [ebp + 18h], edx
        shl    edx, 9
        add    edi, edx
        lea    esi, [ebp + 104h]
loc_7_2B8B:
        mov    eax, dword ptr [esi]
        add    esi, 4
        rol    eax, 8
        push   ecx
        push   esi
        mov    cl, al
        add    dword ptr [ebp + 18h], ecx
        shr    eax, 8
        call   sub_7_2E50
        pop    esi
        pop    ecx
        jb     SHORT loc_7_2B2E
        loop   loc_7_2B8B
        jmp    NEAR PTR loc_7_2B18
sub_7_2BAD:
        push   ebx
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 24h]
        mov    cx, word ptr [ebp + 10h]
        mov    word ptr [ebp + 0f0h], 0
        call   sub_7_0F7C
        pop    ecx
        pop    ebx
        jb     SHORT loc_7_2C09
        jecxz  loc_7_2BD4
        or     dword ptr [ebp + 0ch], 80000000h
loc_7_2BD4:
        xor    eax, eax
        xchg   dword ptr [ebp + 8], eax
        test   byte ptr [ebp + 0fh], 40h
        jne    SHORT loc_7_2BEB
        mov    dword ptr [ebp + 8], ebx
        push   ecx
        call   sub_7_2C3B
        pop    ecx
        jb     SHORT loc_7_2C09
loc_7_2BEB:
        or     esp, esp
        jecxz  loc_7_2C08
        call   sub_7_0EFA
        jb     SHORT loc_7_2C08
        mov    eax, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ebp + 10h]
        mov    edx, ebx
        call   sub_7_29FE
        jb     SHORT loc_7_2C08
        or     esp, esp
loc_7_2C08:
        ret
loc_7_2C09:
        and    dword ptr [ebp + 8], 0
        cmp    word ptr [ebp + 0f0h], 10h
        cmc
        jb     SHORT loc_7_2C08
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 0e0h]
        mov    word ptr [ecx], ax
        mov    dword ptr [ebp + 0ch], eax
        mov    dword ptr [ebp + 10h], eax
        mov    ecx, 101h
        mov    edx, ebx
        call   sub_7_29FE
        jb     SHORT loc_7_2C08
        cmp    eax, eax
        jmp    SHORT loc_7_2C08
sub_7_2C3B:
        mov    edi, dword ptr [ebp + 24h]
        call   sub_7_2AA1
        jb     SHORT loc_7_2C8C
        test   byte ptr [ebp + 5], 1
        jne    SHORT loc_7_2C8C
        mov    esi, dword ptr [ebp + 24h]
        mov    edi, dword ptr [ebp + 18h]
        movzx  ecx, byte ptr [ebp + 9bh]
        sub    ecx, edi
        je     SHORT loc_7_2C69
        shl    edi, 9
        add    edi, esi
        shl    ecx, 7
        xor    eax, eax
        cld
        rep stosd
loc_7_2C69:
        push   ebx
        db  0E8h, 0DDh, 01Bh, 000h, 000h
        pop    ebx
        jb     SHORT loc_7_2C8C
        mov    ecx, dword ptr [ebp + 14h]
        call   sub_7_281E
        jb     SHORT loc_7_2C8D
        call   sub_7_48CC
        and    dword ptr [ebp + 20h], 0
        or     dword ptr [ebp + 4], 140h
loc_7_2C8C:
        ret
loc_7_2C8D:
        call   sub_7_489D
        stc
        jmp    SHORT loc_7_2C8C
sub_7_2C95:
        sub    esp, 0ch
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     SHORT loc_7_2CFF
        cmp    dword ptr [esi + 8], ebp
        jne    SHORT loc_7_2CF8
        cmp    dword ptr [esi + 0ch], eax
        jne    SHORT loc_7_2CF8
loc_7_2CA8:
        mov    eax, dword ptr [esi + 10h]
        cmp    byte ptr [esi + 32h], 0
        jne    SHORT loc_7_2CB6
loc_7_2CB1:
        lea    esp, [esp + 0ch]
        ret
loc_7_2CB6:
        cmp    dword ptr [esi + 18h], -36h
        jae    SHORT loc_7_2CB1
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [esi + 24h], eax
        mov    eax, dword ptr [esi + 10h]
        db  039h, 035h, 090h, 00Dh, 000h, 000h
        je     SHORT loc_7_2CB1
        mov    ecx, dword ptr [esi + 14h]
        mov    edx, dword ptr [esi + 18h]
        mov    dword ptr [ecx + 18h], edx
        mov    dword ptr [edx + 14h], ecx
        db  08Bh, 00Dh, 090h, 00Dh, 000h, 000h
        mov    dword ptr [esi + 14h], ecx
        db  0C7h, 046h, 018h, 07Ch, 00Dh, 000h, 000h
        mov    dword ptr [ecx + 18h], esi
        db  089h, 035h, 090h, 00Dh, 000h, 000h
        clc
        jmp    SHORT loc_7_2CB1
loc_7_2CF8:
        and    dword ptr [esi + 1ch], 0
        and    dword ptr [ebx], 0
loc_7_2CFF:
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp], ebx
        mov    dword ptr [esp + 4], ecx
loc_7_2D0A:
        db  083h, 03Dh, 000h, 00Dh, 000h, 000h, 000h
        jne    NEAR PTR loc_7_2D99
loc_7_2D17:
        mov    ebx, ebp
        mov    edi, dword ptr [esp + 8]
        db  08Ah, 025h, 067h, 00Eh, 000h, 000h
        mov    al, 15h
        db  0CDh, 020h, 004h, 000h, 08Bh, 004h
        jb     SHORT loc_7_2D3A
loc_7_2D2D:
        mov    ebx, dword ptr [esp]
        mov    dword ptr [esi + 1ch], ebx
        mov    dword ptr [ebx], esi
        jmp    NEAR PTR loc_7_2CA8
loc_7_2D3A:
        or     esi, esi
        je     SHORT loc_7_2D99
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [esi + 20h], ecx
        mov    edi, dword ptr [esi + 10h]
        mov    eax, dword ptr [esp + 8]
        push   esi
        call   sub_7_2E50
        pop    esi
        pushfd
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        cmp    byte ptr [esi + 32h], 0
        jne    SHORT loc_7_2D7B
        db  083h, 03Dh, 000h, 00Dh, 000h, 000h, 000h
        je     SHORT loc_7_2D7B
        push   eax
        db  0B8h, 000h, 00Dh, 000h, 000h
        call   sub_7_4A17
        pop    eax
loc_7_2D7B:
        popfd
        jae    SHORT loc_7_2D2D
        call   sub_7_2E3C
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        db  0B8h, 000h, 00Dh, 000h, 000h
        call   sub_7_4A17
        stc
        jmp    NEAR PTR loc_7_2CB1
loc_7_2D99:
        call   sub_7_1C27
        cmp    dword ptr [ebp + 0e4h], 0
        je     SHORT loc_7_2DB7
        call   sub_7_1C84
        jae    NEAR PTR loc_7_2D0A
        jmp    NEAR PTR loc_7_2CB1
loc_7_2DB7:
        db  0B8h, 000h, 00Dh, 000h, 000h
        call   sub_7_4974
        jmp    NEAR PTR loc_7_2D17
sub_7_2DC6:
        xor    edx, edx
        xchg   byte ptr [esi + 32h], dl
        bsf    eax, edx
        bsr    ecx, edx
        sub    ecx, eax
        inc    ecx
        mov    edi, eax
        shl    edi, 9
        add    eax, dword ptr [esi + 0ch]
        add    edi, dword ptr [esi + 10h]
        push   esi
        call   sub_7_2E49
        pop    esi
        pushfd
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        cmp    byte ptr [esi + 32h], 0
        jne    SHORT loc_7_2E08
        db  083h, 03Dh, 000h, 00Dh, 000h, 000h, 000h
        je     SHORT loc_7_2E08
        push   eax
        db  0B8h, 000h, 00Dh, 000h, 000h
        call   sub_7_4A17
        pop    eax
loc_7_2E08:
        popfd
        jb     SHORT loc_7_2E0C
loc_7_2E0B:
        ret
loc_7_2E0C:
        call   sub_7_2E3C
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        bts    dword ptr [ebp + 4], 0fh
        jb     SHORT loc_7_2E2F
        call   sub_7_43D3
        jb     SHORT loc_7_2E2F
        mov    esi, 5
        call   sub_7_4B5E
loc_7_2E2F:
        db  0B8h, 000h, 00Dh, 000h, 000h
        call   sub_7_4A17
        stc
        jmp    SHORT loc_7_2E0B
sub_7_2E3C:
        mov    ecx, dword ptr [esi + 1ch]
        jecxz  loc_7_2E48
        cmp    dword ptr [ecx], esi
        jne    SHORT loc_7_2E48
        and    dword ptr [ecx], 0
loc_7_2E48:
        ret
sub_7_2E49:
        mov    edx, 1
        jmp    SHORT sub_7_2E52
sub_7_2E50:
        xor    edx, edx
sub_7_2E52:
        push   edx
        inc    eax
        mov    edx, ecx
        mov    cl, 9
        sub    cl, byte ptr [ebp + 8bh]
        shl    edx, cl
        shl    eax, cl
        mov    ecx, edx
        call   sub_7_2EB1
        jb     SHORT loc_7_2EA0
loc_7_2E6B:
        mov    eax, dword ptr [esi]
        sub    eax, edx
        cmp    ecx, eax
        ja     SHORT loc_7_2E7B
        add    edx, dword ptr [esi + 4]
        pop    eax
        call   dword ptr [ebp + 34h]
        ret
loc_7_2E7B:
        push   eax
        push   ecx
        mov    ecx, eax
        add    edx, dword ptr [esi + 4]
        mov    eax, dword ptr [esp + 8]
        call   dword ptr [ebp + 34h]
        pop    edx
        pop    eax
        jb     SHORT loc_7_2EAE
        sub    edx, eax
        add    esi, 8
        cmp    dword ptr [ebp + 0c0h], esi
        jbe    SHORT loc_7_2EA0
        mov    ecx, edx
        xor    edx, edx
        jmp    SHORT loc_7_2E6B
loc_7_2EA0:
        mov    eax, 10h
        mov    edx, dword ptr [ebp + 0e0h]
        mov    word ptr [edx], ax
loc_7_2EAE:
        stc
        pop    edx
        ret
sub_7_2EB1:
        mov    esi, dword ptr [ebp + 0bch]
        mov    edx, eax
loc_7_2EB9:
        sub    edx, dword ptr [esi]
        jb     SHORT loc_7_2ECA
        add    esi, 8
        cmp    dword ptr [ebp + 0c0h], esi
        ja     SHORT loc_7_2EB9
        stc
        ret
loc_7_2ECA:
        add    edx, dword ptr [esi]
        clc
        ret
loc_7_2ECE:
        pushad
        mov    esi, dword ptr [ebp + 240h]
        mov    word ptr [esi + 68h], ax
        mov    eax, dword ptr [ebp + 38h]
        mov    dword ptr [esi + 8], eax
        add    edx, dword ptr [eax + 28h]
        mov    eax, dword ptr [eax]
        movzx  eax, byte ptr [eax + 77h]
        inc    eax
        shl    eax, 3
        lea    eax, [eax + esi + 0bch]
        mov    dword ptr [esi + 0ach], eax
        xor    eax, eax
        mov    byte ptr [esi + 0b0h], al
        mov    dword ptr [esi + 64h], eax
        mov    word ptr [esi + 6ah], ax
        mov    dword ptr [esi + 78h], eax
        mov    eax, dword ptr [ebp + 0dch]
        and    eax, 829h
        or     eax, 400h
        mov    dword ptr [esi + 6ch], eax
        mov    dword ptr [esi + 74h], edx
        mov    dword ptr [esi + 7ch], ecx
        cmp    word ptr [esi + 68h], 0dh
        je     SHORT loc_7_2F82
        mov    eax, ecx
        mov    cl, byte ptr [ebp + 8bh]
        shl    eax, cl
        mov    ecx, dword ptr [esi + 7ch]
        or     edi, edi
        jne    SHORT loc_7_2F69
        test   dword ptr [ebp + 0dch], 2
        jne    SHORT loc_7_2F58
        mov    edi, dword ptr [ebp + 0d0h]
        add    dword ptr [ebp + 0d0h], eax
        xor    eax, eax
        jmp    SHORT loc_7_2F69
loc_7_2F58:
        call   sub_7_3017
        or     dword ptr [esi + 6ch], 2
        lea    edi, [esi + 0bch]
        xor    eax, eax
loc_7_2F69:
        mov    dword ptr [esi + 80h], edi
        add    dword ptr [esp], eax
        push   esi
        db  0FFh, 015h, 06Ch, 009h, 000h, 000h
        pop    esi
        or     eax, eax
        je     SHORT loc_7_2F82
        or     dword ptr [esi + 6ch], 40h
loc_7_2F82:
        mov    al, byte ptr [ebp + 89h]
        mov    byte ptr [esi + 0b1h], al
        db  0C7h, 046h, 070h, 0E7h, 02Fh, 000h, 000h
        xor    edx, edx
        test   byte ptr [esi + 6ch], 8
        jne    SHORT loc_7_2FA0
        mov    edx, dword ptr [ebp + 3ch]
loc_7_2FA0:
        mov    dword ptr [esi + 0a8h], edx
        mov    dword ptr [esi + 84h], ebp
        lea    esi, [esi + 64h]
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        call   sub_7_4A6C
        movzx  edx, word ptr [esi + 6]
        mov    dword ptr [esp + 1ch], edx
        mov    word ptr [ebp + 0f0h], dx
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 0e0h]
        mov    ebx, dword ptr [ebp + 240h]
        mov    word ptr [eax], dx
        mov    word ptr [ebx + 0ch], cx
        mov    dword ptr [ebx + 4], ecx
        cmp    edx, 10h
        cmc
        popad
        ret
loc_7_2FE7:
        lea    edx, [esi - 64h]
        db  0BEh, 005h, 030h, 000h, 000h
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 408h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    ecx
        pop    ebx
        pop    eax
        ret
loc_7_3005:
        mov    edx, dword ptr [edx + 84h]
        and    dword ptr [edx + 4], 0fffdffffh
        jmp    NEAR PTR sub_7_49ED
sub_7_3017:
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 240h]
        lea    edx, [ebx + 0bch]
        mov    byte ptr [ebx + 0b0h], 0
        mov    esi, dword ptr [ebp + 0d0h]
loc_7_3036:
        or     ecx, ecx
        je     SHORT loc_7_308B
        mov    eax, dword ptr [esi]
        mov    edi, dword ptr [ebp + 0c8h]
        sub    eax, edi
        cmp    ecx, eax
        ja     SHORT loc_7_304A
        mov    eax, ecx
loc_7_304A:
        sub    ecx, eax
        mov    dword ptr [edx], eax
        add    dword ptr [ebp + 0c8h], eax
        mov    eax, ecx
        mov    cl, byte ptr [ebp + 8bh]
        shl    edi, cl
        mov    ecx, eax
        add    edi, dword ptr [esi + 4]
        mov    dword ptr [edx + 4], edi
        inc    byte ptr [ebx + 0b0h]
        add    edx, 8
        mov    eax, dword ptr [ebp + 0c8h]
        cmp    dword ptr [esi], eax
        ja     SHORT loc_7_3036
        add    esi, 8
        mov    dword ptr [ebp + 0d0h], esi
        and    dword ptr [ebp + 0c8h], 0
        jmp    SHORT loc_7_3036
loc_7_308B:
        and    dword ptr [edx], 0
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret
loc_7_3095:
        pushad
        push   ecx
        push   edx
        mov    edx, 2
        call   sub_7_4AD7
        pop    edx
        pop    ecx
        mov    word ptr [esi + 68h], ax
        mov    eax, dword ptr [ebp + 38h]
        mov    dword ptr [esi + 8], eax
        add    edx, dword ptr [eax + 28h]
        mov    eax, dword ptr [eax]
        movzx  eax, byte ptr [eax + 77h]
        inc    eax
        shl    eax, 3
        lea    eax, [eax + esi + 0bch]
        mov    dword ptr [esi + 0ach], eax
        xor    eax, eax
        mov    byte ptr [esi + 0b0h], al
        mov    dword ptr [esi + 64h], eax
        mov    word ptr [esi + 6ah], ax
        mov    dword ptr [esi + 78h], eax
        mov    dword ptr [esi + 6ch], 500h
        mov    dword ptr [esi + 74h], edx
        mov    dword ptr [esi + 7ch], ecx
        mov    eax, ecx
        mov    cl, byte ptr [ebp + 8bh]
        shl    eax, cl
        mov    ecx, dword ptr [esi + 7ch]
        cmp    word ptr [esi + 68h], 0dh
        je     SHORT loc_7_3114
        mov    dword ptr [esi + 80h], edi
        add    dword ptr [esp], eax
        push   esi
        db  0FFh, 015h, 06Ch, 009h, 000h, 000h
        pop    esi
        or     eax, eax
        je     SHORT loc_7_3114
        or     dword ptr [esi + 6ch], 40h
loc_7_3114:
        mov    al, byte ptr [ebp + 89h]
        mov    byte ptr [esi + 0b1h], al
        xor    eax, eax
        test   byte ptr [esi + 6ch], 8
        jne    SHORT loc_7_312B
        mov    eax, dword ptr [ebp + 3ch]
loc_7_312B:
        mov    dword ptr [esi + 0a8h], eax
        lea    esi, [esi + 64h]
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        and    dword ptr [ebp + 4], 0fffdffffh
        movzx  edx, word ptr [esi + 6]
        mov    word ptr [ebp + 0f0h], dx
        lea    esi, [esi - 64h]
        call   sub_7_4B35
        cmp    edx, 10h
        cmc
        mov    dword ptr [esp + 1ch], edx
        popad
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_7_316A:
        push   edx
        inc    ebp
        inc    ecx
        inc    esp
        push   esp
        dec    eax
        dec    ecx
        push   ebx
        push   esp
        pop    eax
        push   esp
loc_7_3175:
        inc    esp
        push   edx
        push   esi
        push   ebx
        push   eax
        inc    ecx
        inc    ebx
        inc    ebp
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_318D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_3191:
        add    byte ptr [eax], al
loc_7_3193:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
loc_7_31C0:
        cmp    dword ptr [ebx + 8ch], -1
        jne    SHORT loc_7_31F0
        test   byte ptr [ebp + 4], 80h
        jne    SHORT loc_7_31F0
        and    dword ptr [ebp + 8], 0
        and    byte ptr [ebp + 4], 0bfh
        push   ebx
        call   sub_7_31F8
        pop    ebx
        sbb    edx, edx
        and    eax, edx
        mov    dword ptr [ebx + 0a0h], eax
        and    edx, 1ah
        mov    word ptr [ebx + 6ah], dx
loc_7_31EF:
        ret
loc_7_31F0:
        mov    word ptr [ebx + 6ah], 21h
        jmp    SHORT loc_7_31EF
sub_7_31F8:
        and    dword ptr [ebp + 4], 0fff95ffdh
        or     dword ptr [ebp + 4], 1010h
        db  0B8h, 0D8h, 036h, 000h, 000h
        call   sub_7_490C
        jb     NEAR PTR loc_7_33FF
        db  0BEh, 0E4h, 036h, 000h, 000h
        xor    edx, edx
        mov    ecx, 1
        mov    edi, esi
        call   sub_7_3415
        jb     NEAR PTR loc_7_3406
        cmp    byte ptr [esi], 0e9h
        je     SHORT loc_7_3247
        cmp    byte ptr [esi], 0ebh
        jne    NEAR PTR loc_7_3406
        cmp    byte ptr [esi + 2], 90h
        jne    NEAR PTR loc_7_3406
loc_7_3247:
        cmp    word ptr [esi + 0bh], 200h
        jne    NEAR PTR loc_7_3406
        cmp    byte ptr [esi + 0dh], 0
        je     NEAR PTR loc_7_3406
        db  0BFh, 080h, 031h, 000h, 000h
        mov    ecx, 24h
        cmp    word ptr [esi + 16h], 0
        jne    SHORT loc_7_3271
        add    ecx, 1ch
loc_7_3271:
        cld
        rep movsb
        db  00Fh, 0B6h, 00Dh, 08Dh, 031h, 000h, 000h
        bsf    ecx, ecx
        db  088h, 00Dh, 069h, 031h, 000h, 000h
        db  00Fh, 0B7h, 005h, 096h, 031h, 000h, 000h
        or     eax, eax
        je     SHORT loc_7_32FA
        db  00Fh, 0B6h, 00Dh, 090h, 031h, 000h, 000h
        mul    ecx
        db  00Fh, 0B7h, 015h, 08Eh, 031h, 000h, 000h
        add    eax, edx
        db  00Fh, 0B7h, 015h, 091h, 031h, 000h, 000h
        shr    edx, 4
        add    eax, edx
        db  0A3h, 060h, 031h, 000h, 000h
        db  0C6h, 005h, 068h, 031h, 000h, 000h, 002h
        db  066h, 083h, 03Dh, 096h, 031h, 000h, 000h, 00Ch
        jb     SHORT loc_7_331D
        db  00Fh, 0B7h, 005h, 093h, 031h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_7_32D3
        db  0A1h, 0A0h, 031h, 000h, 000h
loc_7_32D3:
        db  02Bh, 005h, 060h, 031h, 000h, 000h
        jb     NEAR PTR loc_7_3406
        xor    edx, edx
        db  00Fh, 0B6h, 00Dh, 08Dh, 031h, 000h, 000h
        div    ecx
        cmp    eax, 0ff6h
        jb     SHORT loc_7_331D
        db  0C6h, 005h, 068h, 031h, 000h, 000h, 000h
        jmp    SHORT loc_7_331D
loc_7_32FA:
        db  0C6h, 005h, 068h, 031h, 000h, 000h, 001h
        db  00Fh, 0B6h, 00Dh, 090h, 031h, 000h, 000h
        db  0A1h, 0A4h, 031h, 000h, 000h
        mul    ecx
        db  00Fh, 0B7h, 015h, 08Eh, 031h, 000h, 000h
        add    eax, edx
        db  0A3h, 060h, 031h, 000h, 000h
loc_7_331D:
        db  0BEh, 075h, 031h, 000h, 000h
        movzx  eax, byte ptr [ebp + 0b2h]
        mov    cl, 0ah
        div    cl
        add    ah, 30h
        mov    byte ptr [esi + 0ah], ah
        xor    ah, ah
        div    cl
        add    ah, 30h
        mov    byte ptr [esi + 9], ah
        add    al, 30h
        mov    byte ptr [esi + 8], al
        mov    word ptr [esi + 1], 5652h
        db  0BFh, 0E4h, 036h, 000h, 000h
        call   sub_7_341F
        jae    SHORT loc_7_336A
        mov    word ptr [esi + 1], 4c42h
        db  0BFh, 0E4h, 036h, 000h, 000h
        call   sub_7_341F
        jb     NEAR PTR loc_7_3406
loc_7_336A:
        test   byte ptr [edi + 0bh], 18h
        jne    NEAR PTR loc_7_3406
        mov    eax, dword ptr [edi + 1ch]
        shr    eax, 9
        je     NEAR PTR loc_7_3406
        mov    dword ptr [ebp + 50h], eax
        xor    ebx, ebx
        db  066h, 083h, 03Dh, 096h, 031h, 000h, 000h, 000h
        jne    SHORT loc_7_3392
        mov    ebx, dword ptr [edi + 12h]
loc_7_3392:
        mov    bx, word ptr [edi + 1ah]
        or     ebx, ebx
        je     SHORT loc_7_3406
        call   sub_7_34F6
        jb     SHORT loc_7_3406
        or     eax, 0ffffffffh
        lea    ecx, [eax + 2]
        db  0BFh, 0E4h, 036h, 000h, 000h
        call   sub_7_2E50
        jb     SHORT loc_7_33F0
        movzx  edi, byte ptr [ebp + 89h]
        db  0BEh, 0E4h, 036h, 000h, 000h
        or     ebx, 0ffffffffh
        call   sub_7_0147
        jb     SHORT loc_7_340E
        call   sub_7_02EE
        jb     SHORT loc_7_340E
        db  0BFh, 0E4h, 036h, 000h, 000h
        db  0BEh, 06Ah, 031h, 000h, 000h
        call   sub_7_341F
        cmc
        sbb    eax, eax
        and    eax, 40000h
        or     dword ptr [ebp + 4], eax
        and    dword ptr [ebp + 4], 0ffffffefh
        xor    eax, eax
loc_7_33F0:
        pushfd
        push   eax
        db  0B8h, 0D8h, 036h, 000h, 000h
        call   sub_7_493D
        pop    eax
        popfd
loc_7_33FE:
        ret
loc_7_33FF:
        mov    eax, 8
        jmp    SHORT loc_7_33FE
loc_7_3406:
        stc
        mov    eax, 2
        jmp    SHORT loc_7_33F0
loc_7_340E:
        mov    eax, 0bh
        jmp    SHORT loc_7_33F0
sub_7_3415:
        mov    eax, 0
        jmp    NEAR PTR loc_7_2ECE
sub_7_341F:
        db  080h, 03Dh, 068h, 031h, 000h, 000h, 001h
        je     SHORT loc_7_347D
        db  00Fh, 0B7h, 01Dh, 091h, 031h, 000h, 000h
        add    ebx, 0fh
        shr    ebx, 4
        db  08Bh, 015h, 060h, 031h, 000h, 000h
        sub    edx, ebx
loc_7_343D:
        dec    ebx
        js     SHORT loc_7_347B
        push   edx
        push   edi
        mov    ecx, 1
        call   sub_7_3415
        pop    edi
        pop    edx
        jb     SHORT loc_7_347B
        inc    edx
        mov    ecx, 10h
        push   edi
        cld
loc_7_3458:
        cmp    byte ptr [edi], 0
        je     SHORT loc_7_347A
        push   ecx
        push   edi
        push   esi
        mov    ecx, 0bh
        repe cmpsb
        pop    esi
        pop    edi
        pop    ecx
        je     SHORT loc_7_3474
        add    edi, 20h
        loop   loc_7_3458
        pop    edi
        jmp    SHORT loc_7_343D
loc_7_3474:
        lea    esp, [esp + 4]
        jmp    SHORT loc_7_347C
loc_7_347A:
        pop    edi
loc_7_347B:
        stc
loc_7_347C:
        ret
loc_7_347D:
        db  08Bh, 01Dh, 0ACh, 031h, 000h, 000h
loc_7_3483:
        mov    edx, ebx
        sub    edx, 2
        db  08Ah, 00Dh, 069h, 031h, 000h, 000h
        shl    edx, cl
        db  003h, 015h, 060h, 031h, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 08Dh, 031h, 000h, 000h
loc_7_349D:
        push   ecx
        push   edi
        mov    ecx, 1
        call   sub_7_3415
        pop    edi
        pop    ecx
        jb     SHORT loc_7_347C
        inc    edx
        push   ecx
        push   edi
        mov    ecx, 10h
        cld
loc_7_34B6:
        cmp    byte ptr [edi], 0
        je     SHORT loc_7_34F1
        push   ecx
        push   edi
        push   esi
        mov    ecx, 0bh
        repe cmpsb
        pop    esi
        pop    edi
        pop    ecx
        je     SHORT loc_7_34EB
        add    edi, 20h
        loop   loc_7_34B6
        pop    edi
        pop    ecx
        loop   loc_7_349D
        push   edi
        call   sub_7_3564
        pop    edi
        jb     SHORT loc_7_347C
        cmp    ebx, 0ffffff8h
        jae    SHORT loc_7_347B
        cmp    ebx, 1
        jb     SHORT loc_7_347C
        jmp    SHORT loc_7_3483
loc_7_34EB:
        lea    esp, [esp + 8]
        jmp    SHORT loc_7_347C
loc_7_34F1:
        add    esp, 8
        jmp    SHORT loc_7_347B
sub_7_34F6:
        db  083h, 00Dh, 064h, 031h, 000h, 000h, 0FFh
        mov    esi, dword ptr [ebp + 0bch]
        or     ebx, ebx
        je     SHORT loc_7_3559
loc_7_3507:
        cmp    dword ptr [ebp + 0c4h], esi
        jbe    SHORT loc_7_3561
        cmp    ebx, 2
        jb     SHORT loc_7_3561
        mov    eax, ebx
        sub    eax, 2
        db  08Ah, 00Dh, 069h, 031h, 000h, 000h
        shl    eax, cl
        db  003h, 005h, 060h, 031h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi], 0
loc_7_3530:
        db  00Fh, 0B6h, 005h, 08Dh, 031h, 000h, 000h
        add    dword ptr [esi], eax
        push   ebx
        push   esi
        call   sub_7_3564
        pop    esi
        pop    eax
        jb     SHORT loc_7_3561
        cmp    ebx, 0ffffff8h
        jae    SHORT loc_7_3556
        inc    eax
        cmp    eax, ebx
        je     SHORT loc_7_3530
        add    esi, 8
        jmp    SHORT loc_7_3507
loc_7_3556:
        add    esi, 8
loc_7_3559:
        mov    dword ptr [ebp + 0c0h], esi
        clc
loc_7_3560:
        ret
loc_7_3561:
        stc
        jmp    SHORT loc_7_3560
sub_7_3564:
        db  080h, 03Dh, 068h, 031h, 000h, 000h, 001h
        je     SHORT loc_7_35C1
        ja     NEAR PTR loc_7_361D
        lea    edx, [ebx + ebx]
        shr    edx, 9
        db  00Fh, 0B7h, 005h, 08Eh, 031h, 000h, 000h
        add    edx, eax
        db  039h, 015h, 064h, 031h, 000h, 000h
        je     SHORT loc_7_35A1
        db  089h, 015h, 064h, 031h, 000h, 000h
        db  0BFh, 0E4h, 036h, 000h, 000h
        mov    ecx, 1
        call   sub_7_3415
        jb     SHORT loc_7_35C0
loc_7_35A1:
        lea    edx, [ebx + ebx]
        and    edx, 1ffh
        db  00Fh, 0B7h, 09Ah, 0E4h, 036h, 000h, 000h
        cmp    ebx, 0fff8h
        jb     SHORT loc_7_35BF
        or     ebx, 0fff0000h
loc_7_35BF:
        clc
loc_7_35C0:
        ret
loc_7_35C1:
        lea    edx, [ebx*4]
        shr    edx, 9
        db  00Fh, 0B7h, 005h, 08Eh, 031h, 000h, 000h
        db  0F6h, 005h, 0A8h, 031h, 000h, 000h, 00Fh
        je     SHORT loc_7_35E1
        db  003h, 005h, 0A4h, 031h, 000h, 000h
loc_7_35E1:
        add    edx, eax
        db  039h, 015h, 064h, 031h, 000h, 000h
        je     SHORT loc_7_3602
        db  089h, 015h, 064h, 031h, 000h, 000h
        db  0BFh, 0E4h, 036h, 000h, 000h
        mov    ecx, 1
        call   sub_7_3415
        jb     SHORT loc_7_35C0
loc_7_3602:
        lea    edx, [ebx*4]
        and    edx, 1ffh
        db  08Bh, 09Ah, 0E4h, 036h, 000h, 000h
        and    ebx, 0fffffffh
        jmp    SHORT loc_7_35C0
loc_7_361D:
        mov    eax, ebx
        xor    edx, edx
        mov    ecx, 400h
        div    ecx
        push   edx
        lea    edx, [eax + eax*2]
        db  00Fh, 0B7h, 005h, 08Eh, 031h, 000h, 000h
        add    edx, eax
        db  039h, 015h, 064h, 031h, 000h, 000h
        je     SHORT loc_7_3652
        db  089h, 015h, 064h, 031h, 000h, 000h
        db  0BFh, 0E4h, 036h, 000h, 000h
        mov    ecx, 3
        call   sub_7_3415
loc_7_3652:
        pop    edx
        jb     NEAR PTR loc_7_35C0
        lea    eax, [edx + edx*2]
        shr    eax, 1
        db  00Fh, 0B7h, 098h, 0E4h, 036h, 000h, 000h
        test   dl, 1
        je     SHORT loc_7_366D
        shr    ebx, 4
loc_7_366D:
        and    ebx, 0fffh
        cmp    ebx, 0ff0h
        je     SHORT loc_7_3687
        cmp    ebx, 0ff8h
        jb     NEAR PTR loc_7_35BF
loc_7_3687:
        or     ebx, 0fffffffh
        jmp    NEAR PTR loc_7_35C0
sub_7_3692:
        push   edi
        sub    esi, esp
        push   esi
        push   ecx
        push   edx
        mov    eax, edx
        shr    eax, 9
        dec    eax
        mov    ecx, 1
        db  0BFh, 0E4h, 038h, 000h, 000h
        call   sub_7_2E50
        jb     SHORT loc_7_36D3
        pop    edx
        pop    ecx
        pop    edi
        add    edi, esp
        push   edi
        push   ecx
        push   edx
        and    edx, 1ffh
        lea    eax, [edx + ecx]
        cmp    eax, 201h
        cmc
        jb     SHORT loc_7_36D3
        mov    eax, ecx
        db  08Dh, 0B2h, 0E4h, 038h, 000h, 000h
        cld
        rep movsb
loc_7_36D3:
        pop    edx
        pop    ecx
        pop    esi
        pop    edi
        ret
        db  000h, 000h, 000h, 000h, 0D8h, 036h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_7_3CE4:
        mov    eax, dword ptr [ebp + 38h]
        or     dword ptr [ebp + 4], 800h
        test   byte ptr [ebp + 5], 20h
        jne    SHORT loc_7_3CFF
        call   sub_7_100C
        jb     NEAR PTR loc_7_3E85
loc_7_3CFF:
        cmp    dword ptr [ebx + 78h], 0
        jne    NEAR PTR loc_7_3E7F
        mov    eax, dword ptr [ebx + 7ch]
        mov    dword ptr [ebp + 0cch], eax
        mov    edx, dword ptr [ebx + 80h]
        test   byte ptr [ebx + 6ch], 2
        jne    SHORT loc_7_3D72
loc_7_3D1E:
        mov    dword ptr [ebp + 0d0h], edx
        mov    dword ptr [ebp + 0c8h], 0
        mov    edx, dword ptr [ebx + 74h]
        test   byte ptr [ebx + 6eh], 8
        jne    SHORT loc_7_3D43
        mov    ecx, dword ptr [ebp + 38h]
        sub    edx, dword ptr [ecx + 28h]
        jb     NEAR PTR loc_7_3E7F
loc_7_3D43:
        mov    dword ptr [ebp + 0d4h], edx
        add    eax, edx
        jb     NEAR PTR loc_7_3E7F
        cmp    dword ptr [ebp + 0aeh], eax
        jb     NEAR PTR loc_7_3E7F
        cmp    dword ptr [ebp + 74h], edx
        ja     SHORT loc_7_3D82
loc_7_3D62:
        cmp    word ptr [ebx + 68h], 0
        je     NEAR PTR loc_7_1D34
        jmp    NEAR PTR loc_7_20D5
loc_7_3D72:
        cmp    dword ptr [edx], eax
        jne    SHORT loc_7_3D1E
        mov    edx, dword ptr [edx + 4]
        and    dword ptr [ebp + 0dch], 0fffffffdh
        jmp    SHORT loc_7_3D1E
loc_7_3D82:
        xor    ecx, ecx
        sub    eax, dword ptr [ebp + 74h]
        jbe    SHORT loc_7_3D91
        mov    ecx, eax
        sub    dword ptr [ebp + 0cch], eax
loc_7_3D91:
        push   ecx
        movzx  eax, word ptr [ebp + 9ch]
        sub    edx, eax
        jae    SHORT loc_7_3DB4
        neg    edx
        cmp    dword ptr [ebx + 7ch], edx
        ja     SHORT loc_7_3DA7
        mov    edx, dword ptr [ebx + 7ch]
loc_7_3DA7:
        mov    ecx, edx
        call   sub_7_47D7
        jbe    NEAR PTR loc_7_3E66
loc_7_3DB4:
        mov    eax, dword ptr [ebp + 0d4h]
        movzx  ecx, word ptr [ebp + 9ch]
        sub    eax, ecx
        movzx  ecx, word ptr [ebp + 0a4h]
        sub    eax, ecx
        jae    SHORT loc_7_3DF5
        neg    eax
        mov    ecx, dword ptr [ebp + 0cch]
        cmp    ecx, eax
        jb     SHORT loc_7_3DDC
        mov    ecx, eax
loc_7_3DDC:
        cmp    word ptr [ebx + 68h], 0
        je     SHORT loc_7_3DEE
        push   ebx
        push   ecx
        call   sub_7_4407
        pop    ecx
        pop    ebx
        jb     SHORT loc_7_3E66
loc_7_3DEE:
        call   sub_7_47D7
        jbe    SHORT loc_7_3E66
loc_7_3DF5:
        mov    eax, dword ptr [ebp + 0d4h]
        movzx  ecx, word ptr [ebp + 9ch]
        sub    eax, ecx
        movzx  ecx, word ptr [ebp + 0a4h]
        sub    eax, ecx
        sub    dword ptr [ebp + 0d4h], ecx
        sub    eax, ecx
        jae    SHORT loc_7_3E5B
        neg    eax
        mov    ecx, dword ptr [ebp + 0cch]
        cmp    ecx, eax
        jb     SHORT loc_7_3E25
        mov    ecx, eax
loc_7_3E25:
        cmp    word ptr [ebx + 68h], 0
        je     SHORT loc_7_3E54
        test   dword ptr [ebp + 0dch], 2
        je     SHORT loc_7_3E3F
        call   sub_7_3017
        jmp    SHORT loc_7_3E4A
loc_7_3E3F:
        mov    edx, ecx
        shl    edx, 9
        add    dword ptr [ebp + 0d0h], edx
loc_7_3E4A:
        sub    dword ptr [ebp + 0cch], ecx
        je     SHORT loc_7_3E66
        jmp    SHORT loc_7_3E5B
loc_7_3E54:
        call   sub_7_47D7
        jbe    SHORT loc_7_3E66
loc_7_3E5B:
        mov    ecx, dword ptr [ebp + 0cch]
        call   sub_7_47D7
loc_7_3E66:
        pop    ecx
        jb     SHORT loc_7_3E85
        jecxz  loc_7_3E85
        mov    dword ptr [ebp + 0cch], ecx
        mov    eax, dword ptr [ebp + 74h]
        mov    dword ptr [ebp + 0d4h], eax
        jmp    NEAR PTR loc_7_3D62
loc_7_3E7F:
        mov    word ptr [ebx + 6ah], 10h
loc_7_3E85:
        ret
loc_7_3E86:
        movzx  eax, word ptr [ebx + 92h]
        cmp    ax, 4404h
        db  00Fh, 085h, 035h, 0CAh, 0F8h, 0FFh
        cmp    dword ptr [ebx + 94h], 5
        db  00Fh, 082h, 028h, 0CAh, 0F8h, 0FFh
        mov    edi, dword ptr [ebx + 98h]
        cmp    word ptr [edi], 444dh
        db  00Fh, 085h, 017h, 0CAh, 0F8h, 0FFh
        push   edi
        mov    al, byte ptr [edi + 2]
        cmp    al, 46h
        je     SHORT loc_7_3ED3
        cmp    al, 49h
        je     SHORT loc_7_3EDD
        cmp    al, 72h
        je     SHORT loc_7_3EEC
        cmp    al, 73h
        je     SHORT loc_7_3EFE
        cmp    al, 77h
        je     SHORT loc_7_3F46
        stc
        jmp    NEAR PTR loc_7_3F5F
loc_7_3ED3:
        db  0E8h, 066h, 0CAh, 0F8h, 0FFh
        jmp    NEAR PTR loc_7_3F5F
loc_7_3EDD:
        db  0E8h, 05Ch, 0CAh, 0F8h, 0FFh
        jb     SHORT loc_7_3F5F
        call   sub_7_4687
        clc
        jmp    SHORT loc_7_3F5F
loc_7_3EEC:
        push   edi
        push   ebx
        db  0E8h, 04Bh, 0CAh, 0F8h, 0FFh
        pop    ebx
        pop    esi
        jb     SHORT loc_7_3F5F
        call   sub_7_13F2
        jmp    SHORT loc_7_3F5F
loc_7_3EFE:
        cmp    dword ptr [ebx + 94h], 11h
        jb     SHORT loc_7_3F5F
        db  0E8h, 032h, 0CAh, 0F8h, 0FFh
        jb     SHORT loc_7_3F5F
        call   sub_7_100C
        jb     SHORT loc_7_3F5F
        mov    edi, dword ptr [esp]
        mov    eax, dword ptr [ebp + 54h]
        mov    dword ptr [edi + 5], eax
        mov    eax, dword ptr [ebp + 58h]
        mov    dword ptr [edi + 9], eax
        mov    eax, dword ptr [ebp + 64h]
        cmp    word ptr [ebp + 0b4h], 3
        jb     SHORT loc_7_3F40
        movzx  ecx, byte ptr [ebp + 9bh]
        inc    ecx
        mov    eax, dword ptr [ebp + 58h]
        xor    edx, edx
        div    ecx
loc_7_3F40:
        mov    dword ptr [edi + 0dh], eax
        clc
        jmp    SHORT loc_7_3F5F
loc_7_3F46:
        push   edi
        push   ebx
        db  0E8h, 0F1h, 0C9h, 0F8h, 0FFh
        pop    ebx
        pop    esi
        jb     SHORT loc_7_3F5F
        push   esi
        xor    edx, edx
        call   sub_7_470E
        pop    esi
        call   sub_7_157A
loc_7_3F5F:
        pop    edi
        jb     SHORT loc_7_3F68
        mov    word ptr [edi + 3], 4f4bh
loc_7_3F68:
        ret
loc_7_3F69:
        movzx  eax, word ptr [ebx + 92h]
        cmp    ah, 0dbh
        jne    SHORT loc_7_3F83
        cmp    al, 0ch
        jae    SHORT loc_7_3F83
        movzx  eax, al
        db  0FFh, 024h, 085h, 08Ch, 03Fh, 000h, 000h
loc_7_3F83:
        mov    word ptr [ebx + 6ah], 16h
        ret
        db  08Bh, 0C0h, 0BCh, 03Fh, 000h, 000h, 0C0h, 031h, 000h, 000h, 0C4h, 03Fh, 000h, 000h, 00Eh, 040h
        db  000h, 000h, 083h, 03Fh, 000h, 000h, 083h, 03Fh, 000h, 000h, 083h, 03Fh, 000h, 000h, 083h, 03Fh
        db  000h, 000h, 046h, 040h, 000h, 000h, 0CBh, 040h, 000h, 000h, 05Ah, 041h, 000h, 000h, 099h, 042h
        db  000h, 000h
loc_7_3FBC:
        and    dword ptr [ebx + 0a0h], 0
        ret
loc_7_3FC4:
        call   sub_7_100C
        push   ebx
        call   sub_7_1B8C
        pop    ebx
        mov    eax, dword ptr [ebp + 58h]
        cmp    eax, 80h
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        mov    cl, byte ptr [ebp + 8ch]
        shr    eax, cl
        db  083h, 03Dh, 02Ch, 00Eh, 000h, 000h, 001h
        jbe    SHORT loc_7_3FFA
        mul    dword ptr [ebp + 0f4h]
        div    dword ptr [ebp + 0f8h]
loc_7_3FFA:
        cmp    eax, 10000h
        cmc
        sbb    ecx, ecx
        or     eax, ecx
        movzx  eax, ax
        mov    dword ptr [ebx + 0a0h], eax
        ret
loc_7_400E:
        xor    eax, eax
        test   byte ptr [ebp + 5], 80h
        jne    SHORT loc_7_403F
        cmp    dword ptr [ebp + 58h], -1
        je     SHORT loc_7_403F
        push   ebx
        call   sub_7_1B8C
        pop    ebx
        mov    eax, dword ptr [ebp + 64h]
        cmp    word ptr [ebp + 0b4h], 3
        jb     SHORT loc_7_403F
        movzx  ecx, byte ptr [ebp + 9bh]
        inc    ecx
        mov    eax, dword ptr [ebp + 58h]
        xor    edx, edx
        div    ecx
loc_7_403F:
        mov    dword ptr [ebx + 0a0h], eax
        ret
loc_7_4046:
        and    dword ptr [ebx + 0a0h], 0
        cmp    word ptr [ebp + 0b4h], 3
        jb     SHORT loc_7_40AC
        mov    eax, dword ptr [ebx + 94h]
        cmp    eax, 2
        jb     SHORT loc_7_40B2
        add    eax, dword ptr [ebp + 78h]
        cmp    dword ptr [ebp + 7ch], eax
        jbe    SHORT loc_7_40B2
        cmp    dword ptr [ebp + 8], eax
        je     SHORT loc_7_40B8
        mov    edi, eax
        shl    edi, 8
        add    edi, 10000000h
        push   eax
        mov    ebx, ebp
        db  08Ah, 025h, 068h, 00Eh, 000h, 000h
        xor    al, al
        db  0CDh, 020h, 004h, 000h, 08Bh, 004h
        pop    eax
        mov    ebx, dword ptr [ebp + 48h]
        jae    SHORT loc_7_40B8
        push   eax
        call   sub_7_2933
        pop    edx
        jb     SHORT loc_7_40AB
        test   eax, 80000000h
        jne    SHORT loc_7_40B8
        and    eax, 0ff000000h
        call   sub_7_29FE
loc_7_40AB:
        ret
loc_7_40AC:
        inc    dword ptr [ebx + 0a0h]
loc_7_40B2:
        inc    dword ptr [ebx + 0a0h]
loc_7_40B8:
        inc    dword ptr [ebx + 0a0h]
        mov    eax, dword ptr [ebp + 0e0h]
        mov    word ptr [eax], 16h
        jmp    SHORT loc_7_40AB
loc_7_40CB:
        and    dword ptr [ebx + 0a0h], 0
        mov    ecx, dword ptr [ebx + 98h]
        or     ecx, ecx
        je     SHORT loc_7_4159
        mov    eax, dword ptr [ebp + 0e0h]
        mov    word ptr [eax], 10h
        mov    edx, dword ptr [ebx + 94h]
        add    edx, dword ptr [ebp + 78h]
        lea    esi, [ecx + edx]
        cmp    dword ptr [ebp + 7ch], esi
        jb     SHORT loc_7_4159
        mov    word ptr [eax], 0
        mov    ebx, ecx
        test   byte ptr [ebp + 4], 80h
        je     SHORT loc_7_411A
        cmp    dword ptr [ebp + 8], edx
        jb     SHORT loc_7_411A
        cmp    dword ptr [ebp + 8], esi
        jae    SHORT loc_7_411A
        push   ebx
        push   edx
        call   sub_7_1B8C
        pop    edx
        pop    ebx
        jb     SHORT loc_7_4159
loc_7_411A:
        push   edx
        mov    eax, edx
        call   sub_7_2933
        pop    edx
        jb     SHORT loc_7_4159
        test   eax, 80000000h
        je     SHORT loc_7_4155
        push   ebx
        push   edx
        xor    ebx, ebx
        call   sub_7_0CB0
        pop    edx
        pop    ebx
        jb     SHORT loc_7_4159
        mov    ecx, dword ptr [ebp + 0fch]
        mov    esi, dword ptr [ebp + 48h]
loc_7_4142:
        movzx  eax, byte ptr [ebp + ecx*4 + 0ffh]
        shl    eax, 9
        add    dword ptr [esi + 0a0h], eax
        loop   loc_7_4142
loc_7_4155:
        inc    edx
        dec    ebx
        jne    SHORT loc_7_411A
loc_7_4159:
        ret
loc_7_415A:
        sub    esp, 10h
        and    dword ptr [ebx + 0a0h], 0
        mov    dword ptr [esp + 0ch], ebx
        call   sub_7_1B8C
        mov    ebx, dword ptr [esp + 0ch]
        jb     NEAR PTR loc_7_4216
        and    dword ptr [ebp + 8], 0
        and    byte ptr [ebp + 4], 0bfh
        mov    eax, dword ptr [ebx + 94h]
        cmp    eax, 2
        jb     NEAR PTR loc_7_423C
        add    eax, dword ptr [ebp + 78h]
        cmp    dword ptr [ebp + 7ch], eax
        jbe    NEAR PTR loc_7_423C
        mov    edx, eax
        call   sub_7_470E
        push   eax
        call   sub_7_2933
        pop    edx
        jb     SHORT loc_7_4216
        or     dword ptr [ebp + 4], 100h
        movzx  esi, byte ptr [ebp + 9bh]
        mov    dword ptr [ebp + 18h], esi
        test   eax, 80000000h
        je     SHORT loc_7_41E3
        cmp    byte ptr [ebp + 9bh], ch
        ja     SHORT loc_7_41D8
        test   eax, 20000000h
        jne    SHORT loc_7_41D8
        test   eax, 40000000h
        jne    SHORT loc_7_4216
loc_7_41D8:
        mov    eax, edx
        xor    ecx, ecx
        call   sub_7_45E9
        jb     SHORT loc_7_4216
loc_7_41E3:
        push   edx
        call   sub_7_04C8
        pop    edx
        jb     SHORT loc_7_4232
        mov    eax, dword ptr [ebp + 100h]
        shld   ecx, eax, 10h
        mov    cl, byte ptr [ebp + 9bh]
        and    eax, 0ffffffh
        cmp    dword ptr [ebp + 0fch], 2
        jae    SHORT loc_7_4248
loc_7_420A:
        or     eax, 0c0000000h
        call   sub_7_29FE
        jb     SHORT loc_7_421B
loc_7_4216:
        lea    esp, [esp + 10h]
        ret
loc_7_421B:
        or     dword ptr [ebp + 58h], 0ffffffffh
        or     dword ptr [ebp + 64h], 0ffffffffh
        btr    dword ptr [ebp + 4], 0dh
        mov    ebx, dword ptr [esp + 0ch]
        inc    dword ptr [ebx + 0a0h]
loc_7_4232:
        mov    ebx, dword ptr [esp + 0ch]
        inc    dword ptr [ebx + 0a0h]
loc_7_423C:
        mov    ebx, dword ptr [esp + 0ch]
        inc    dword ptr [ebx + 0a0h]
        jmp    SHORT loc_7_4216
loc_7_4248:
        mov    dword ptr [esp + 8], edx
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp], ecx
        db  0E8h, 0F4h, 005h, 000h, 000h
        jb     SHORT loc_7_421B
        mov    esi, edi
        call   sub_7_26EB
        mov    edx, 45544550h
loc_7_4266:
        mov    dword ptr [eax], edx
        add    eax, 4
        loop   loc_7_4266
        mov    eax, dword ptr [esp + 4]
        mov    ecx, 1
        mov    edi, esi
        call   sub_7_2E49
        jb     SHORT loc_7_421B
        call   sub_7_489D
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp]
        mov    edx, dword ptr [esp + 8]
        or     eax, 20000000h
        jmp    NEAR PTR loc_7_420A
loc_7_4299:
        call   sub_7_100C
        push   ebx
        call   sub_7_1B8C
        pop    ebx
        mov    eax, dword ptr [ebp + 58h]
        mov    dword ptr [ebx + 94h], eax
        mov    eax, dword ptr [ebp + 54h]
        mov    dword ptr [ebx + 0a0h], eax
        ret
loc_7_42B8:
        sub    esp, 8
        mov    edx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edx + 14h]
        mov    eax, dword ptr [eax + 4]
        db  0B9h, 044h, 00Eh, 000h, 000h
loc_7_42CA:
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        je     SHORT loc_7_4331
        cmp    ecx, eax
        jne    SHORT loc_7_42CA
        mov    cl, byte ptr [eax + 89h]
        cmp    byte ptr [edx + 0b1h], cl
        je     SHORT loc_7_42F9
        mov    ecx, dword ptr [edx + 8]
        test   byte ptr [ecx + 1dh], 20h
        je     SHORT loc_7_4331
        mov    cl, byte ptr [eax + 8ah]
        cmp    byte ptr [edx + 0b1h], cl
        jne    SHORT loc_7_4331
loc_7_42F9:
        mov    cl, byte ptr [eax + 88h]
        mov    byte ptr [edx + 0b1h], cl
        mov    dword ptr [esp + 4], edi
        mov    dword ptr [esp], esi
        mov    edi, dword ptr [edx + 98h]
        lea    edi, [edi + 7]
        lea    esi, [eax + 99h]
        mov    ecx, 19h
        cld
        rep movsb
        sub    edi, 19h
        mov    byte ptr [edi + 5], 2
        mov    edi, dword ptr [esp + 4]
        mov    esi, dword ptr [esp]
loc_7_4331:
        mov    eax, dword ptr [edx + 14h]
        sub    eax, 8
        mov    dword ptr [edx + 14h], eax
        push   edx
        call   dword ptr [eax]
        add    esp, 4
        add    esp, 8
        ret
sub_7_4344:
        db  083h, 03Dh, 00Ch, 00Dh, 000h, 000h, 000h
        jne    SHORT loc_7_43A3
loc_7_434D:
        cmp    dword ptr [ebp + 0e4h], 0
        jne    SHORT loc_7_43AC
loc_7_4356:
        pop    edx
        lea    eax, [ebp + 40h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [eax + 8], ecx
        or     ecx, ecx
        jne    SHORT loc_7_4365
        jmp    edx
loc_7_4365:
        xor    esi, esi
        xchg   dword ptr [ecx + 8ch], esi
        mov    dword ptr [eax], esi
        or     esi, esi
        jne    SHORT loc_7_4376
        mov    dword ptr [eax + 4], eax
loc_7_4376:
        cmp    ebx, 0ffffff44h
        je     SHORT loc_7_4385
        db  068h, 0CBh, 043h, 000h, 000h
        jmp    edx
loc_7_4385:
        push   edx
        mov    edx, ebp
        db  0BEh, 0C9h, 043h, 000h, 000h
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 408h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    ecx
        pop    ebx
        pop    eax
        ret
loc_7_43A3:
        push   ebx
        call   sub_7_1B8C
        pop    ebx
        jmp    SHORT loc_7_434D
loc_7_43AC:
        push   ebx
        mov    eax, dword ptr [ebp + 0e0h]
        push   dword ptr [eax]
        call   sub_7_1C84
        pop    edx
        pop    ebx
        jb     SHORT loc_7_4356
        mov    eax, dword ptr [ebp + 0e0h]
        mov    word ptr [eax], dx
        jmp    SHORT loc_7_4356
loc_7_43C9:
        mov    ebp, edx
loc_7_43CB:
        mov    edx, dword ptr [ebp + 48h]
        db  0E9h, 061h, 0C1h, 0F8h, 0FFh
sub_7_43D3:
        push   eax
        push   ecx
        push   edx
        sub    esp, 4
        xor    eax, eax
        lea    ecx, [esp]
        movzx  edx, byte ptr [ebp + 88h]
        cmp    dl, 0ffh
        je     SHORT loc_7_43FF
        push   eax
        push   eax
        push   ecx
        push   edx
        db  0CDh, 020h, 06Fh, 000h, 040h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_7_43FF
        cmp    dword ptr [esp], -1
loc_7_43FF:
        lea    esp, [esp + 4]
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_7_4407:
        mov    eax, dword ptr [ebp + 0d4h]
        mov    esi, dword ptr [ebp + 0d0h]
        test   dword ptr [ebp + 0dch], 2
        je     NEAR PTR sub_7_4500
        push   esi
        push   dword ptr [ebp + 0c8h]
        test   byte ptr [ebp + 4], 2
        jne    SHORT loc_7_4488
        mov    edi, esi
loc_7_4432:
        or     ecx, ecx
        je     NEAR PTR loc_7_44F3
        mov    edx, dword ptr [edi]
        mov    esi, dword ptr [ebp + 0c8h]
        sub    edx, esi
        shl    esi, 9
        add    esi, dword ptr [edi + 4]
        cmp    edx, ecx
        jb     SHORT loc_7_4450
        mov    edx, ecx
loc_7_4450:
        push   eax
        push   ecx
        push   edx
        push   edi
        mov    ecx, edx
        call   sub_7_4500
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        jb     NEAR PTR loc_7_44F3
        add    eax, edx
        add    dword ptr [ebp + 0c8h], edx
        sub    ecx, edx
        mov    edx, dword ptr [ebp + 0c8h]
        cmp    dword ptr [edi], edx
        ja     SHORT loc_7_4432
        add    edi, 8
        mov    dword ptr [ebp + 0c8h], 0
        jmp    SHORT loc_7_4432
loc_7_4488:
        push   eax
        push   ecx
        db  0B8h, 0D8h, 036h, 000h, 000h
        call   sub_7_490C
        pop    ecx
        pop    eax
        jb     SHORT loc_7_44F3
        push   eax
        movzx  edx, word ptr [ebp + 9ch]
        sub    eax, edx
        xor    edx, edx
        mov    edi, 3
        div    edi
        pop    eax
        sub    edi, edx
        or     ecx, ecx
        je     SHORT loc_7_44E7
loc_7_44B2:
        cmp    edi, ecx
        jb     SHORT loc_7_44B8
        mov    edi, ecx
loc_7_44B8:
        push   ecx
        mov    ecx, edi
        push   eax
        push   ecx
        xor    eax, eax
        db  0BEh, 0E4h, 036h, 000h, 000h
        call   sub_7_28C5
        pop    ecx
        pop    eax
        db  0BEh, 0E4h, 036h, 000h, 000h
        push   eax
        push   ecx
        call   sub_7_4500
        pop    edx
        pop    eax
        pop    ecx
        jb     SHORT loc_7_44E7
        add    eax, edx
        mov    edi, 3
        sub    ecx, edx
        ja     SHORT loc_7_44B2
loc_7_44E7:
        pushfd
        db  0B8h, 0D8h, 036h, 000h, 000h
        call   sub_7_493D
        popfd
loc_7_44F3:
        pop    dword ptr [ebp + 0c8h]
        pop    dword ptr [ebp + 0d0h]
        ret
sub_7_4500:
        mov    edx, ecx
        test   byte ptr [ebp + 4], 2
        jne    SHORT loc_7_4556
        movzx  ecx, word ptr [ebp + 9ch]
        sub    eax, ecx
        shl    eax, 9
        push   edx
        shl    edx, 9
        pop    ecx
        add    edx, eax
        shr    edx, 1
        add    edx, dword ptr [ebp + 78h]
        cmp    dword ptr [ebp + 7ch], edx
        ja     SHORT loc_7_4528
        mov    edx, dword ptr [ebp + 7ch]
loc_7_4528:
        shr    eax, 1
        cmp    eax, 1
        sbb    ecx, ecx
        add    ecx, ecx
        sub    eax, ecx
        add    ecx, ecx
        sub    esi, ecx
        add    eax, dword ptr [ebp + 78h]
loc_7_453A:
        cmp    eax, edx
        jae    NEAR PTR loc_7_45E8
        movzx  ecx, word ptr [esi]
        call   sub_7_45E9
        jb     NEAR PTR loc_7_45E8
        add    esi, 2
        inc    eax
        jmp    SHORT loc_7_453A
loc_7_4556:
        movzx  ecx, word ptr [ebp + 9ch]
        sub    eax, ecx
        shl    eax, 9
        push   edx
        shl    edx, 9
        pop    ecx
        push   eax
        add    eax, edx
        add    eax, eax
        xor    edx, edx
        mov    edi, 3
        div    edi
        cmp    edx, 1
        cmc
        sbb    eax, 0
        add    eax, dword ptr [ebp + 78h]
        cmp    dword ptr [ebp + 7ch], eax
        ja     SHORT loc_7_4587
        mov    eax, dword ptr [ebp + 7ch]
loc_7_4587:
        xchg   dword ptr [esp], eax
        add    eax, eax
        xor    edx, edx
        mov    edi, 3
        div    edi
        cmp    edx, 1
        cmc
        adc    eax, 0
        cmp    eax, 1
        sbb    ecx, ecx
        sub    eax, ecx
        sub    eax, ecx
        lea    ecx, [ecx + ecx*2]
        sub    esi, ecx
        add    eax, dword ptr [ebp + 78h]
        bt     edx, 0
        adc    esi, 0
        bt     edx, 1
        pop    edx
        jb     SHORT loc_7_45D1
loc_7_45BB:
        cmp    eax, edx
        jae    SHORT loc_7_45E8
        movzx  ecx, word ptr [esi]
        and    ecx, 0fffh
        call   sub_7_45E9
        jb     SHORT loc_7_45E8
        inc    esi
        inc    eax
loc_7_45D1:
        cmp    eax, edx
        jae    SHORT loc_7_45E8
        movzx  ecx, word ptr [esi]
        shr    ecx, 4
        call   sub_7_45E9
        jb     SHORT loc_7_45E8
        add    esi, 2
        inc    eax
        jmp    SHORT loc_7_45BB
loc_7_45E8:
        ret
sub_7_45E9:
        push   eax
        push   ecx
        push   edx
        push   esi
        jecxz  loc_7_464C
        test   dword ptr [ebp + 0dch], 2000000h
        je     NEAR PTR loc_7_4682
        push   eax
        call   sub_7_2933
        pop    edx
        jb     SHORT loc_7_4682
        test   eax, 80000000h
        jne    SHORT loc_7_4682
        test   eax, 0ffffffh
        je     SHORT loc_7_4682
        cmp    dword ptr [ebp + 8], edx
        jne    SHORT loc_7_4621
        test   byte ptr [ebp + 4], 80h
        jne    SHORT loc_7_4682
loc_7_4621:
        push   eax
        push   ecx
        push   edx
        call   sub_7_18B8
        pop    ebx
        pop    ecx
        pop    eax
        jb     SHORT loc_7_4682
        and    dword ptr [ebp + 4], 0ffffffbfh
        mov    dword ptr [ebp + 0ch], eax
        mov    word ptr [ebp + 10h], cx
        mov    ecx, 1
        call   sub_7_2BAD
        pushfd
        call   sub_7_1B4F
        popfd
        jmp    SHORT loc_7_4682
loc_7_464C:
        push   eax
        call   sub_7_2933
        pop    edx
        jb     SHORT loc_7_4682
        test   eax, 80000000h
        je     SHORT loc_7_4682
        cmp    dword ptr [ebp + 8], edx
        jne    SHORT loc_7_4666
        btr    dword ptr [ebp + 4], 7
loc_7_4666:
        push   ecx
        call   sub_7_470E
        pop    ecx
        push   eax
        push   ecx
        push   edx
        call   sub_7_0E90
        pop    edx
        pop    ecx
        pop    eax
        and    eax, 7fffffffh
        call   sub_7_29FE
loc_7_4682:
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_7_4687:
        call   sub_7_46B8
        xor    edx, edx
        call   sub_7_470E
        or     dword ptr [ebp + 64h], 0ffffffffh
        or     dword ptr [ebp + 58h], 0ffffffffh
        mov    byte ptr [ebp + 0f2h], 0ffh
        or     dword ptr [ebp + 4], 20000h
        db  080h, 03Dh, 065h, 00Eh, 000h, 000h, 000h
        je     SHORT loc_7_46B7
        btr    dword ptr [ebp + 4], 0dh
loc_7_46B7:
        ret
sub_7_46B8:
        db  08Ah, 025h, 067h, 00Eh, 000h, 000h
        db  0BAh, 0D4h, 046h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 08Bh, 004h
        db  0B8h, 000h, 00Dh, 000h, 000h
        call   sub_7_4A17
        ret
loc_7_46D4:
        jne    SHORT loc_7_470D
        cmp    dword ptr [esi + 8], ebp
        jne    SHORT loc_7_470D
        test   dword ptr [esi + 0ch], 0f0000000h
        jne    SHORT loc_7_470D
        cmp    byte ptr [esi + 32h], 0
        je     SHORT loc_7_4702
        mov    eax, dword ptr [esi + 0ch]
        cmp    dword ptr [ebp + 68h], eax
        jbe    SHORT loc_7_470D
        mov    eax, dword ptr [esi + 14h]
        mov    edx, dword ptr [esi + 18h]
        mov    dword ptr [eax + 18h], edx
        mov    dword ptr [edx + 14h], eax
        mov    byte ptr [esi + 32h], 0
loc_7_4702:
        call   sub_7_2E3C
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
loc_7_470D:
        ret
sub_7_470E:
        test   byte ptr [ebp + 4], 20h
        jne    NEAR PTR loc_7_479F
        test   edx, edx
        je     SHORT loc_7_4721
        cmp    dword ptr [ebp + 8], edx
        jne    SHORT loc_7_4729
loc_7_4721:
        and    dword ptr [ebp + 8], 0
        and    byte ptr [ebp + 4], 0bfh
loc_7_4729:
        xor    ebx, ebx
        test   edx, edx
        jne    SHORT loc_7_4748
        mov    ecx, edx
        push   eax
        push   edx
        db  08Ah, 025h, 068h, 00Eh, 000h, 000h
        db  0BAh, 0A0h, 047h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 08Bh, 004h
        pop    edx
        pop    eax
        jmp    SHORT loc_7_478F
loc_7_4748:
        movzx  ecx, byte ptr [ebp + 9bh]
        shr    ecx, 3
        push   eax
        mov    edi, edx
        shl    edi, 8
        lea    edi, [ecx + edi + 10000000h]
loc_7_475F:
        push   ebx
        mov    ebx, ebp
        db  08Ah, 025h, 068h, 00Eh, 000h, 000h
        xor    al, al
        dec    edi
        db  0CDh, 020h, 004h, 000h, 08Bh, 004h
        pop    ebx
        jb     SHORT loc_7_478C
        db  0CDh, 020h, 00Dh, 000h, 08Bh, 004h
        jne    SHORT loc_7_478C
        push   ecx
        push   edx
        call   sub_7_1C09
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        pop    edx
        pop    ecx
        inc    ebx
loc_7_478C:
        loop   loc_7_475F
        pop    eax
loc_7_478F:
        test   ebx, ebx
        je     SHORT loc_7_479F
        push   eax
        db  0B8h, 00Ch, 00Dh, 000h, 000h
        call   sub_7_4996
        pop    eax
loc_7_479F:
        ret
loc_7_47A0:
        jne    SHORT loc_7_47D6
        mov    edx, dword ptr [esi + 0ch]
        mov    eax, edx
        and    edx, 0f0000000h
        cmp    edx, 10000000h
        jne    SHORT loc_7_47D6
        cmp    dword ptr [esi + 8], ebp
        jne    SHORT loc_7_47D6
        jecxz  loc_7_47C8
        and    eax, 0fffff00h
        shr    eax, 8
        cmp    eax, ecx
        jne    SHORT loc_7_47D6
loc_7_47C8:
        push   ecx
        call   sub_7_1C09
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        pop    ecx
        inc    ebx
loc_7_47D6:
        ret
sub_7_47D7:
        mov    eax, dword ptr [ebp + 0d4h]
        add    eax, dword ptr [ebp + 4ch]
        dec    eax
        movzx  edx, word ptr [ebx + 68h]
        xor    edi, edi
        push   ecx
        call   sub_7_2E52
        pop    ecx
        jb     SHORT loc_7_47FC
        add    dword ptr [ebp + 0d4h], ecx
        sub    dword ptr [ebp + 0cch], ecx
loc_7_47FC:
        ret
        db  000h, 000h, 000h, 0B8h, 0FFh, 0FFh, 0FFh
loc_7_4804:
        inc    ebx
        add    byte ptr [eax], al
loc_7_4808:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_4828:
        add    al, dh
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 030h, 048h, 000h, 000h, 000h, 000h
        db  000h, 000h, 0FFh
loc_7_483D:
        inc    edx
        dec    esp
        add    byte ptr [edx + 56h], dl
        add    byte ptr [ecx], al
loc_7_4845:
        inc    ecx
        add    byte ptr [eax], dh
        xor    byte ptr [eax], dh
        db  000h, 090h, 0B8h, 030h, 048h, 000h
        add    al, ch
        mov    dh, 0
        add    byte ptr [eax], al
        jb     SHORT loc_7_4885
        db  089h, 02Dh, 02Ch, 048h, 000h, 000h
        db  08Bh, 03Dh, 028h, 048h, 000h, 000h
        db  080h, 03Dh, 03Ch, 048h, 000h, 000h, 000h
        je     SHORT loc_7_4885
        db  0BBh, 008h, 048h, 000h, 000h
        mov    ecx, 8
        call   sub_7_1A99
        jb     SHORT loc_7_4886
        db  0C6h, 005h, 03Ch, 048h, 000h, 000h, 000h
loc_7_4885:
        ret
loc_7_4886:
        db  0B8h, 030h, 048h, 000h, 000h
        call   sub_7_493D
        db  0C7h, 005h, 02Ch, 048h, 000h, 000h, 000h, 000h, 000h, 000h
        stc
        jmp    SHORT loc_7_4885
sub_7_489D:
        db  039h, 02Dh, 038h, 048h, 000h, 000h
        jne    SHORT loc_7_48BC
        db  0B8h, 030h, 048h, 000h, 000h
        call   sub_7_493D
        cmp    dword ptr [eax + 8], 0
        je     SHORT loc_7_48BC
        db  083h, 025h, 02Ch, 048h, 000h, 000h, 000h
loc_7_48BC:
        ret
sub_7_48BD:
        db  039h, 02Dh, 02Ch, 048h, 000h, 000h
        stc
        jne    SHORT loc_7_48CB
        db  0E8h, 081h, 0FFh, 0FFh, 0FFh
loc_7_48CB:
        ret
sub_7_48CC:
        mov    eax, dword ptr [ebp + 24h]
        db  087h, 005h, 028h, 048h, 000h, 000h
        mov    dword ptr [ebp + 24h], eax
        movzx  ecx, byte ptr [ebp + 9bh]
        shr    ecx, 3
loc_7_48E2:
        mov    esi, dword ptr [ebp + ecx*4 + 200h]
        db  08Bh, 03Ch, 08Dh, 004h, 048h, 000h, 000h
        db  0CDh, 020h, 011h, 000h, 08Bh, 004h
        loop   loc_7_48E2
        cmp    byte ptr [ebp + 9bh], 40h
        db  00Fh, 095h, 005h, 03Ch, 048h, 000h, 000h
        call   sub_7_489D
        ret
sub_7_490C:
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_7_4916
        mov    dword ptr [eax + 8], ebp
loc_7_4915:
        ret
loc_7_4916:
        pushad
        call   sub_7_1CEA
        popad
        jb     SHORT loc_7_4915
        jne    SHORT sub_7_490C
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], ebp
        lea    ecx, [ebp + 244h]
        mov    dword ptr [ecx], 0
        mov    dword ptr [eax + 4], ecx
        call   sub_7_4A6C
        clc
        jmp    SHORT loc_7_4915
sub_7_493D:
        push   edx
        push   esi
        mov    edx, dword ptr [eax]
        mov    dword ptr [eax + 8], edx
        or     edx, edx
        je     SHORT loc_7_4971
        mov    esi, dword ptr [edx + 244h]
        mov    dword ptr [eax], esi
        or     esi, esi
        jne    SHORT loc_7_4957
        mov    dword ptr [eax + 4], eax
loc_7_4957:
        db  0BEh, 0EDh, 049h, 000h, 000h
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 408h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    ecx
        pop    ebx
        pop    eax
loc_7_4971:
        pop    esi
        pop    edx
        ret
sub_7_4974:
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], ebp
        lea    ecx, [ebp + 244h]
        mov    dword ptr [ecx], 0
        mov    dword ptr [eax + 4], ecx
        cmp    dword ptr [eax], ebp
        sete   cl
        or     byte ptr [eax + 8], cl
        call   sub_7_4A6C
        ret
sub_7_4996:
        btr    dword ptr [eax + 8], 0
        jb     SHORT loc_7_499E
        ret
loc_7_499E:
        bts    dword ptr [eax + 8], 1
        jb     SHORT loc_7_49C5
        push   edx
        push   esi
        mov    edx, eax
        db  0BEh, 0C6h, 049h, 000h, 000h
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 408h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    ecx
        pop    ebx
        pop    eax
        pop    esi
        pop    edx
loc_7_49C5:
        ret
loc_7_49C6:
        btr    dword ptr [edx + 8], 1
        bt     dword ptr [edx + 8], 2
        jb     SHORT loc_7_4A4C
        mov    eax, edx
        mov    edx, dword ptr [edx]
        xor    ecx, ecx
        xchg   dword ptr [edx + 244h], ecx
        mov    dword ptr [eax], ecx
        or     ecx, ecx
        setne  cl
        jne    SHORT loc_7_49EA
        mov    dword ptr [eax + 4], eax
loc_7_49EA:
        or     byte ptr [eax + 8], cl
sub_7_49ED:
        mov    dword ptr [edx + 23ch], esp
        mov    ecx, dword ptr [edx + 238h]
        jecxz  loc_7_4A16
        sub    esp, ecx
        lea    esi, [edx + 24ch]
        mov    edi, esp
        cld
        shrd   eax, ecx, 2
        shr    ecx, 2
        rep movsd
        shld   ecx, eax, 2
        rep movsb
        popad
loc_7_4A16:
        ret
sub_7_4A17:
        btr    dword ptr [eax + 8], 0
        jb     SHORT loc_7_4A1F
        ret
loc_7_4A1F:
        bts    dword ptr [eax + 8], 2
        bts    dword ptr [eax + 8], 1
        jb     SHORT loc_7_4A4B
        push   edx
        push   esi
        mov    edx, eax
        db  0BEh, 04Ch, 04Ah, 000h, 000h
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 408h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    ecx
        pop    ebx
        pop    eax
        pop    esi
        pop    edx
loc_7_4A4B:
        ret
loc_7_4A4C:
        mov    eax, edx
        xor    edx, edx
        mov    dword ptr [eax + 8], edx
        xchg   dword ptr [eax], edx
        mov    dword ptr [eax + 4], eax
loc_7_4A58:
        xor    eax, eax
        xchg   dword ptr [edx + 244h], eax
        push   eax
        call   sub_7_49ED
        pop    edx
        or     edx, edx
        jne    SHORT loc_7_4A58
        ret
sub_7_4A6C:
        pushad
        mov    ecx, dword ptr [ebp + 23ch]
        sub    ecx, esp
        cmp    ecx, 100h
        ja     SHORT loc_7_4AA3
        mov    dword ptr [ebp + 238h], ecx
        mov    esi, esp
        lea    edi, [ebp + 24ch]
        cld
        shrd   eax, ecx, 2
        shr    ecx, 2
        rep movsd
        shld   ecx, eax, 2
        rep movsb
        add    esp, dword ptr [ebp + 238h]
        sti
        ret
loc_7_4AA3:
        and    dword ptr [ebp + 238h], 0
        db  00Fh, 0B7h, 03Dh, 016h, 000h, 000h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 018h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, 0e0000000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
loc_7_4AD5:
        jmp    SHORT loc_7_4AD5
sub_7_4AD7:
        mov    ecx, dword ptr [ebp + 38h]
        cmp    ecx, 1
        jb     SHORT loc_7_4B10
loc_7_4ADF:
        sub    esp, 10h
        mov    word ptr [esp], 2
        mov    byte ptr [esp + 0eh], dl
        call   sub_7_4B11
        mov    word ptr [esp + 4], si
        mov    dword ptr [esp + 6], edx
        mov    esi, esp
        push   esi
        db  0FFh, 015h, 058h, 009h, 000h, 000h
        mov    esi, dword ptr [esi + 0ah]
        add    esp, 14h
        cmp    esi, 1
        lea    esi, [edx + esi - 64h]
loc_7_4B10:
        ret
sub_7_4B11:
        mov    esi, dword ptr [ecx]
        movzx  esi, byte ptr [esi + 77h]
        test   dword ptr [ecx + 1ch], 800h
        je     SHORT loc_7_4B22
        add    esi, esi
loc_7_4B22:
        inc    esi
        shl    esi, 3
        mov    edx, dword ptr [ecx + 4]
        add    esi, edx
        add    esi, 0bch
        add    edx, 64h
        ret
sub_7_4B35:
        mov    ecx, dword ptr [ebp + 38h]
loc_7_4B38:
        mov    ecx, dword ptr [ecx + 4]
        add    ecx, 64h
        neg    ecx
        lea    esi, [ecx + esi + 64h]
        sub    esp, 8
        mov    dword ptr [esp + 4], esi
        mov    word ptr [esp], 4
        mov    esi, esp
        push   esi
        db  0FFh, 015h, 058h, 009h, 000h, 000h
        add    esp, 0ch
        ret
sub_7_4B5E:
        bt     dword ptr [ebp + 4], 0eh
        jb     SHORT loc_7_4B8B
        cmp    dword ptr [ebp + 0e8h], 1
        cmc
        jb     SHORT loc_7_4B8B
        mov    dword ptr [ebp + 0ech], esi
        mov    edx, ebp
        db  0BEh, 08Ch, 04Bh, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    dword ptr [ebp + 0e8h], esi
        cmp    esi, 1
loc_7_4B8B:
        ret
loc_7_4B8C:
        cld
        movzx  eax, byte ptr [edx + 88h]
        add    al, 41h
        db  0A2h, 043h, 048h, 000h, 000h
        db  0B8h, 043h, 048h, 000h, 000h
        movzx  ebx, byte ptr [edx + 89h]
        add    bl, 41h
        db  088h, 01Dh, 045h, 048h, 000h, 000h
        db  0BBh, 045h, 048h, 000h, 000h
        db  0B9h, 03Dh, 048h, 000h, 000h
        cmp    word ptr [edx + 0b4h], 0
        je     SHORT loc_7_4BC9
        db  0B9h, 040h, 048h, 000h, 000h
loc_7_4BC9:
        movzx  edi, byte ptr [edx + 0b2h]
        push   eax
        push   edx
        mov    eax, edi
        db  0BFh, 047h, 048h, 000h, 000h
        mov    dl, 0ah
        div    dl
        add    ah, 30h
        mov    byte ptr [edi + 2], ah
        xor    ah, ah
        div    dl
        add    ax, 3030h
        mov    word ptr [edi], ax
        pop    edx
        pop    eax
        push   edi
        push   ecx
        push   ebx
        push   eax
        push   180001h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        mov    edi, eax
        mov    eax, dword ptr [edx + 0ech]
        add    eax, 100000h
        db  068h, 045h, 048h, 000h, 000h
        db  068h, 043h, 048h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        mov    ecx, eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, 80001000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        add    esp, dword ptr [esp]
        add    esp, dword ptr [esp]
        xor    eax, eax
        mov    dword ptr [edx + 0e8h], eax
        ret
        db  026h, 044h, 052h
_PTEXT ends
        end
