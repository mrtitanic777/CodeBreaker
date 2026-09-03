; qic117_lcode_01_01.asm -- QIC117 object 1 segment _LTEXT  (part 1/3)
.386p
_LTEXT segment dword public use32 'LCODE'

VQIC117_Control:
        cmp    eax, 1bh
        je     NEAR PTR loc_1_0290
        cmp    eax, 1ch
        je     NEAR PTR loc_1_02BB
        cmp    eax, 23h
        jne    SHORT loc_1_0024
        push   esi
loc_1_0018:
        push   edx
        push   ecx
        push   ebx
        call   sub_1_0026
loc_1_0020:
        cmp    eax, 1
        ret
loc_1_0024:
        clc
        ret
sub_1_0026:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 4
loc_1_0030:
        cmp    eax, 100h
        push   esi
        push   edi
        ja     SHORT loc_1_0045
        je     NEAR PTR loc_1_00CB
        test   eax, eax
        je     SHORT loc_1_00BF
        jmp    SHORT loc_1_00B5
loc_1_0045:
        cmp    eax, 20dh
        ja     SHORT loc_1_008B
        je     NEAR PTR loc_1_01C1
        cmp    eax, 101h
        je     NEAR PTR loc_1_00F5
        cmp    eax, 102h
        je     NEAR PTR loc_1_0114
        cmp    eax, 103h
        je     NEAR PTR loc_1_0155
        cmp    eax, 104h
        je     NEAR PTR loc_1_0187
        cmp    eax, 106h
        je     NEAR PTR loc_1_01A4
        jmp    SHORT loc_1_00B5
loc_1_008B:
        cmp    eax, 20eh
        je     NEAR PTR loc_1_0206
        cmp    eax, 20fh
        je     NEAR PTR loc_1_0254
        cmp    eax, 306h
        je     NEAR PTR loc_1_026D
        cmp    eax, -1
        je     NEAR PTR loc_1_0280
loc_1_00B5:
        mov    eax, 75h
        jmp    NEAR PTR loc_1_0287
loc_1_00BF:
        mov    ecx, 400h
        xor    eax, eax
        jmp    NEAR PTR loc_1_0287
loc_1_00CB:
        mov    edi, dword ptr [ebp + 14h]
        cmp    dword ptr [edi + 1ch], 4
        jb     SHORT loc_1_00EB
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [eax], 10302h
        mov    ecx, dword ptr [edi + 20h]
        mov    dword ptr [ecx], 4
        jmp    NEAR PTR loc_1_0285
loc_1_00EB:
        mov    eax, 1156h
        jmp    NEAR PTR loc_1_0287
loc_1_00F5:
        mov    edi, dword ptr [ebp + 14h]
        push   dword ptr [edi + 4]
        push   0
        db  068h, 078h, 0CEh, 040h, 000h
        db  068h, 080h, 0CFh, 040h, 000h
        call   sub_1_1C52
        add    esp, 10h
        jmp    NEAR PTR loc_1_0287
loc_1_0114:
        db  083h, 03Dh, 060h, 0D0h, 040h, 000h, 000h
        je     SHORT loc_1_012A
        db  068h, 048h, 0CDh, 040h, 000h
        call   sub_1_110B
        add    esp, 4
loc_1_012A:
        mov    edi, dword ptr [ebp + 14h]
        db  0C7h, 005h, 060h, 0D0h, 040h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [edi + 10h]
        mov    ax, word ptr [eax]
        push   eax
        db  068h, 078h, 0CEh, 040h, 000h
        db  068h, 080h, 0CFh, 040h, 000h
        call   sub_1_1BF3
        add    esp, 0ch
        jmp    NEAR PTR loc_1_0287
loc_1_0155:
        mov    edi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [edi + 10h]
        push   dword ptr [edi + 24h]
        push   dword ptr [ecx]
        push   dword ptr [edi + 18h]
        db  068h, 078h, 0CEh, 040h, 000h
        db  068h, 080h, 0CFh, 040h, 000h
        call   sub_1_19B5
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_1_0287
        mov    eax, 3e5h
        jmp    NEAR PTR loc_1_0287
loc_1_0187:
        mov    edi, dword ptr [ebp + 14h]
        push   dword ptr [edi + 10h]
        db  068h, 078h, 0CEh, 040h, 000h
        db  068h, 080h, 0CFh, 040h, 000h
        call   sub_1_1957
        add    esp, 0ch
        jmp    NEAR PTR loc_1_0287
loc_1_01A4:
        mov    edi, dword ptr [ebp + 14h]
        push   dword ptr [edi + 18h]
        db  068h, 078h, 0CEh, 040h, 000h
        db  068h, 080h, 0CFh, 040h, 000h
        call   sub_1_181E
        add    esp, 0ch
        jmp    NEAR PTR loc_1_0287
loc_1_01C1:
        lea    eax, [ebp - 4]
        mov    edi, dword ptr [ebp + 14h]
        push   eax
        mov    ecx, dword ptr [edi + 10h]
        mov    ax, word ptr [ecx]
        push   eax
        db  068h, 078h, 0CEh, 040h, 000h
        call   sub_1_17D6
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_01F2
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        add    eax, 548h
        db  0A3h, 014h, 0D0h, 040h, 000h
        jmp    SHORT loc_1_01FC
loc_1_01F2:
        db  0C7h, 005h, 014h, 0D0h, 040h, 000h, 0E0h, 0CFh, 040h, 000h
loc_1_01FC:
        db  0A1h, 014h, 0D0h, 040h, 000h
        jmp    NEAR PTR loc_1_0287
loc_1_0206:
        mov    edi, dword ptr [ebp + 14h]
        mov    eax, dword ptr [edi + 18h]
        mov    byte ptr [eax + 22h], 22h
        push   dword ptr [edi + 18h]
        push   0
        mov    dword ptr [ebp - 4], 0
        call   sub_1_1179
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_0287
        db  068h, 048h, 0CDh, 040h, 000h
        call   sub_1_1644
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_0287
        db  0BEh, 060h, 0CDh, 040h, 000h
        mov    edi, dword ptr [edi + 18h]
        mov    ecx, 45h
        rep movsd
        db  0C7h, 005h, 060h, 0D0h, 040h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_1_0287
loc_1_0254:
        db  0C7h, 005h, 060h, 0D0h, 040h, 000h, 000h, 000h, 000h, 000h
        db  068h, 048h, 0CDh, 040h, 000h
        call   sub_1_110B
        add    esp, 4
        jmp    SHORT loc_1_0287
loc_1_026D:
        mov    edi, dword ptr [ebp + 14h]
        push   dword ptr [edi + 4]
        call   sub_1_02C4
        movzx  eax, ax
        add    esp, 4
        jmp    SHORT loc_1_0287
loc_1_0280:
        call   sub_1_1090
loc_1_0285:
        xor    eax, eax
loc_1_0287:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  0CCh
loc_1_0290:
        db  0C6h, 005h, 0FFh, 01Fh, 040h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 036h, 000h
        jb     SHORT loc_1_02A6
        db  0C6h, 005h, 0FFh, 01Fh, 040h, 000h, 001h
loc_1_02A6:
        db  0CDh, 020h, 030h, 000h, 001h, 000h
        push   0
        push   0
        push   ebx
        call   sub_1_16F4
        add    esp, 0ch
        clc
        ret
loc_1_02BB:
        call   sub_1_1090
        clc
        ret
        db  0CCh, 0CCh
sub_1_02C4:
        enter  0, 0
        in     al, 0a1h
        mov    ah, al
        in     al, 21h
        bts    ax, 6
        mov    bx, ax
        mov    ecx, 0fh
        mov    edx, 1
loc_1_02E0:
        bt     bx, dx
        jae    SHORT loc_1_02F9
        mov    eax, edx
        pushad
        mov    ebx, dword ptr [ebp + 8]
        db  0CDh, 020h, 00Bh, 000h, 003h, 000h
        popad
        jae    SHORT loc_1_02F9
        btr    bx, dx
loc_1_02F9:
        inc    dx
        loop   loc_1_02E0
        mov    ax, bx
        leave
        ret
        db  0CCh, 0CCh
sub_1_0304:
        enter  0, 0
        push   0
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_031E
        xor    eax, eax
        jmp    SHORT loc_1_0321
loc_1_031E:
        mov    eax, dword ptr [ebp + 8]
loc_1_0321:
        leave
        ret
        db  0CCh
sub_1_0324:
        enter  0, 0
        push   ebx
        push   ecx
        mov    ebx, 406h
        movzx  ecx, word ptr [ebp + 8]
        xor    eax, eax
        cmp    ax, cx
        je     SHORT loc_1_0342
loc_1_033A:
        add    ebx, 2
        loop   loc_1_033A
        movzx  eax, word ptr [ebx]
loc_1_0342:
        pop    ecx
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh
sub_1_0348:
        enter  0, 0
        push   edx
        mov    edx, dword ptr [ebp + 8]
        or     dx, dx
        je     SHORT loc_1_037E
        add    dx, 402h
        in     al, dx
        and    al, 1fh
        out    dx, al
        or     al, 40h
        out    dx, al
        in     al, dx
        mov    ah, al
        and    al, 0c0h
        cmp    al, 40h
        jne    SHORT loc_1_037E
        mov    al, ah
        bt     ax, 0
        jae    SHORT loc_1_037E
        mov    al, 14h
        out    dx, al
        mov    eax, 1
        pop    edx
        leave
        ret
loc_1_037E:
        mov    eax, 0
        pop    edx
        leave
        ret
        db  0CCh, 0CCh
sub_1_0388:
        pushfd
        push   ebx
        push   ecx
        push   edx
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h
        bt     ebx, 13h
        jb     SHORT loc_1_03A5
        bt     ebx, 12h
        jb     SHORT loc_1_03AC
        mov    eax, 2
        jmp    SHORT loc_1_03B1
loc_1_03A5:
        mov    eax, 3
        jmp    SHORT loc_1_03B1
loc_1_03AC:
        mov    eax, 1
loc_1_03B1:
        bt     ebx, 10h
        jae    SHORT loc_1_03BA
        or     eax, 10h
loc_1_03BA:
        pop    edx
        pop    ecx
        pop    ebx
        popfd
        ret
        db  0CCh
sub_1_03C0:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        push   ecx
        push   edx
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 14h]
        push   esi
        push   0fffh
        push   0
        push   dword ptr [ebp + 10h]
        push   ebx
        push   0
        push   dword ptr [ebp + 0ch]
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        test   eax, eax
        je     SHORT loc_1_0412
        mov    ebx, dword ptr [ebp + 18h]
        mov    dword ptr [ebx], edx
        push   eax
        shr    edx, 0ch
        push   0
        push   esi
        push   1
        push   edx
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        mov    eax, dword ptr [esi]
        and    eax, 0fffff000h
        mov    dword ptr [esi], eax
        pop    eax
loc_1_0412:
        pop    edx
        pop    ecx
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
        db  0CCh, 0CCh, 0CCh
sub_1_041C:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        push   ecx
        push   edx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, eax
        shr    eax, 0ch
        shr    ecx, 0ch
        sub    ecx, eax
        inc    ecx
        push   40000000h
        push   ecx
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        pop    ecx
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
        db  0CCh, 0CCh, 0CCh
sub_1_0450:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        push   ecx
        push   edx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, eax
        shr    eax, 0ch
        shr    ecx, 0ch
        sub    ecx, eax
        inc    ecx
        push   40000000h
        push   ecx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_0489
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0fffh
        add    eax, ecx
loc_1_0489:
        pop    edx
        pop    ecx
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
sub_1_0490:
        enter  0, 0
        pushad
        db  08Bh, 03Dh, 014h, 0D0h, 040h, 000h
        mov    ah, byte ptr [edi]
        mov    bl, byte ptr [edi + 2]
        mov    bh, byte ptr [edi + 7]
        xor    edx, edx
        mov    dx, word ptr [edi + 18h]
        mov    ecx, edx
        add    edx, 2
        ror    ecx, 10h
        or     edx, ecx
        xor    ecx, ecx
        test   bl, 8
        jne    SHORT loc_1_04C1
        inc    ecx
        or     bl, 8
        mov    al, bl
        out    dx, al
loc_1_04C1:
        mov    al, byte ptr [ebp + 8]
        and    al, 7fh
        ror    edx, 10h
        xor    bh, al
        test   byte ptr [edi + 3], 10h
        je     SHORT loc_1_04D6
        cmp    ecx, 0
        jne    SHORT loc_1_04FA
loc_1_04D6:
        cmp    al, ah
        je     SHORT loc_1_04DF
        out    dx, al
        mov    ah, al
        jmp    SHORT loc_1_04EB
loc_1_04DF:
        xor    bl, 2
        mov    al, bl
        ror    edx, 10h
        out    dx, al
        ror    edx, 10h
loc_1_04EB:
        movzx  ecx, byte ptr [edi + 0fh]
        cmp    ecx, 0
        je     SHORT loc_1_04F8
loc_1_04F4:
        in     al, 21h
        loop   loc_1_04F4
loc_1_04F8:
        jmp    SHORT loc_1_0522
loc_1_04FA:
        out    dx, al
        mov    ah, al
        ror    edx, 10h
        test   byte ptr [edi + 3], 3
        je     SHORT loc_1_050C
        and    bl, 0dfh
        mov    al, bl
        out    dx, al
loc_1_050C:
        xor    bl, 2
        mov    al, bl
        out    dx, al
        ror    edx, 10h
        movzx  ecx, byte ptr [edi + 11h]
        cmp    ecx, 0
        je     SHORT loc_1_0522
loc_1_051E:
        in     al, 21h
        loop   loc_1_051E
loc_1_0522:
        mov    al, byte ptr [ebp + 0ch]
        xor    bh, al
        cmp    al, ah
        je     SHORT loc_1_0530
        out    dx, al
        mov    ah, al
        jmp    SHORT loc_1_0539
loc_1_0530:
        xor    bl, 2
        mov    al, bl
        ror    edx, 10h
        out    dx, al
loc_1_0539:
        mov    byte ptr [edi], ah
        mov    byte ptr [edi + 2], bl
        mov    byte ptr [edi + 7], bh
        popad
        leave
        ret
sub_1_0544:
        enter  0, 0
        pushad
        db  08Bh, 03Dh, 014h, 0D0h, 040h, 000h
        mov    dx, word ptr [edi + 18h]
        mov    ecx, edx
        add    edx, 2
        mov    bl, byte ptr [edi + 2]
        mov    bh, byte ptr [edi + 7]
        test   bl, 8
        jne    SHORT loc_1_0569
        or     bl, 8
        mov    al, bl
        out    dx, al
loc_1_0569:
        mov    al, byte ptr [ebp + 8]
        and    al, 7fh
        xchg   ecx, edx
        out    dx, al
        xor    bh, al
        xor    bl, 2
        mov    al, bl
        xchg   ecx, edx
        out    dx, al
        push   ecx
        movzx  ecx, byte ptr [edi + 10h]
        cmp    ecx, 0
        je     SHORT loc_1_0589
loc_1_0585:
        in     al, 21h
        loop   loc_1_0585
loc_1_0589:
        pop    ecx
        mov    al, byte ptr [ebp + 0ch]
        xchg   ecx, edx
        out    dx, al
        xor    bh, al
        mov    byte ptr [edi], al
        xor    bl, 2
        mov    al, bl
        xchg   ecx, edx
        out    dx, al
        mov    byte ptr [edi + 7], bh
        mov    byte ptr [edi + 2], bl
        popad
        leave
        ret
        db  0CCh, 0CCh, 0CCh
sub_1_05A8:
        push   ecx
        push   ebx
        db  08Bh, 01Dh, 0F1h, 01Fh, 040h, 000h
        cmp    ebx, 0
        je     SHORT loc_1_05D1
        db  0A1h, 0EDh, 01Fh, 040h, 000h
        cmp    eax, 0
        je     SHORT loc_1_05D1
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        xor    eax, eax
        and    ecx, 4
        je     SHORT loc_1_05D1
        mov    eax, 1
loc_1_05D1:
        pop    ebx
        pop    ecx
        ret
sub_1_05D4:
        enter  0, 0
        mov    eax, dword ptr [ebp + 0ch]
        leave
        ret
        db  0CCh, 0CCh, 0CCh
sub_1_05E0:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     SHORT loc_1_05F7
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_1_05F7:
        leave
        ret
        db  0CCh, 0CCh, 0CCh
sub_1_05FC:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     SHORT loc_1_0613
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
loc_1_0613:
        leave
        ret
        db  0CCh, 0CCh, 0CCh
sub_1_0618:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebp + 0ch], 6
        je     SHORT loc_1_066D
        db  0BFh, 070h, 020h, 040h, 000h
        db  083h, 03Dh, 0F1h, 01Fh, 040h, 000h, 000h
        jne    SHORT loc_1_068E
        db  083h, 03Dh, 0EDh, 01Fh, 040h, 000h, 000h
        jne    SHORT loc_1_0662
        mov    eax, dword ptr [ebp + 14h]
        db  0A3h, 0E8h, 01Fh, 040h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [edi], ax
        mov    eax, dword ptr [ebp + 10h]
        db  0A3h, 0E4h, 01Fh, 040h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     SHORT loc_1_068E
        db  0A3h, 0EDh, 01Fh, 040h, 000h
loc_1_0662:
        db  089h, 01Dh, 0F1h, 01Fh, 040h, 000h
        mov    ax, word ptr [edi]
        jmp    SHORT loc_1_0692
loc_1_066D:
        db  080h, 03Dh, 0FFh, 01Fh, 040h, 000h, 000h
        je     SHORT loc_1_0692
        mov    eax, dword ptr [ebp + 14h]
        db  0A3h, 0E8h, 01Fh, 040h, 000h
        mov    eax, dword ptr [ebp + 10h]
        db  0A3h, 0E4h, 01Fh, 040h, 000h
        db  089h, 01Dh, 0F1h, 01Fh, 040h, 000h
        jmp    SHORT loc_1_0692
loc_1_068E:
        mov    ax, 0
loc_1_0692:
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
        db  0CCh
sub_1_0698:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        db  080h, 03Dh, 0ECh, 01Fh, 040h, 000h, 000h
        je     SHORT loc_1_06AF
        mov    eax, 1
        jmp    SHORT loc_1_0701
loc_1_06AF:
        mov    ebx, dword ptr [ebp + 8]
        db  080h, 03Dh, 0FFh, 01Fh, 040h, 000h, 000h
        je     SHORT loc_1_06EF
        xor    eax, eax
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    ebx, edi
        db  0B9h, 030h, 017h, 040h, 000h
        db  0CDh, 020h, 001h, 000h, 036h, 000h
        jae    SHORT loc_1_06D9
        mov    eax, 2
        jmp    SHORT loc_1_0701
loc_1_06D9:
        db  0A3h, 0EDh, 01Fh, 040h, 000h
        push   ebx
        db  0CDh, 020h, 005h, 000h, 036h, 000h
        pop    ebx
        jae    SHORT loc_1_06EF
        mov    eax, 4
        jmp    SHORT loc_1_0701
loc_1_06EF:
        db  089h, 01Dh, 0E0h, 01Fh, 040h, 000h
        mov    dx, 3f7h
        in     al, dx
        db  0A2h, 0F6h, 01Fh, 040h, 000h
        xor    eax, eax
loc_1_0701:
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
        db  0CCh, 0CCh
sub_1_0708:
        pushad
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        popad
        ret
        db  0CCh, 0CCh, 0CCh
sub_1_0720:
        mov    ecx, dword ptr [esp + 4]
        db  0CDh, 020h, 00Ah, 000h, 005h, 000h
        mov    eax, ecx
        ret
        db  0CCh, 0CCh, 0CCh
loc_1_0730:
        db  083h, 03Dh, 0F1h, 01Fh, 040h, 000h, 000h
        je     SHORT loc_1_0749
        push   edx
        db  08Bh, 015h, 0E8h, 01Fh, 040h, 000h
        db  0FFh, 015h, 0E4h, 01Fh, 040h, 000h
        pop    edx
        clc
        ret
loc_1_0749:
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        clc
        ret
        db  0CCh, 0CCh, 0CCh
loc_1_0754:
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 003h, 080h, 003h, 000h
loc_1_0760:
        db  083h, 03Dh, 0F1h, 01Fh, 040h, 000h, 000h
        je     SHORT loc_1_078E
        db  03Bh, 01Dh, 0F1h, 01Fh, 040h, 000h
        je     SHORT loc_1_0773
        jmp    SHORT loc_1_078E
loc_1_0773:
        db  03Bh, 005h, 0EDh, 01Fh, 040h, 000h
        je     SHORT loc_1_077B
loc_1_077B:
        cmp    ecx, 0
        je     SHORT loc_1_0788
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_1_078E
loc_1_0788:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_1_078E:
        ret
        db  0CCh
sub_1_0790:
        db  0A1h, 0EDh, 01Fh, 040h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        ret
sub_1_079C:
        enter  0, 0
        pushfd
        push   edi
        push   esi
        push   ebx
        push   ecx
        push   edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, 8
        mov    edx, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebp + 10h]
        mov    edi, 0
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        mov    eax, esi
        pop    edx
        pop    ecx
        pop    ebx
        pop    esi
        pop    edi
        popfd
        leave
        ret
loc_1_07CC:
        xor    esi, esi
        db  083h, 03Dh, 0E0h, 01Fh, 040h, 000h, 000h
        je     SHORT loc_1_07F4
        db  0A1h, 0F7h, 01Fh, 040h, 000h
        mov    byte ptr [eax], 0ffh
        mov    eax, 2710h
        mov    edx, 0
        db  0BEh, 0CCh, 017h, 040h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
loc_1_07F4:
        db  089h, 035h, 0FBh, 01Fh, 040h, 000h
        ret
        db  0CCh
sub_1_07FC:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        db  0A1h, 0EDh, 01Fh, 040h, 000h
        cmp    eax, 0
        je     SHORT loc_1_0823
        cmp    byte ptr [ebp + 8], 6
        je     SHORT loc_1_0823
        db  0CDh, 020h, 011h, 000h, 003h, 000h
        db  0C7h, 005h, 0EDh, 01Fh, 040h, 000h, 000h, 000h, 000h, 000h
loc_1_0823:
        db  0C7h, 005h, 0F1h, 01Fh, 040h, 000h, 000h, 000h, 000h, 000h
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
        db  0CCh, 0CCh
sub_1_0834:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        db  08Bh, 01Dh, 0E0h, 01Fh, 040h, 000h
        db  0C7h, 005h, 0E0h, 01Fh, 040h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 0F5h, 01Fh, 040h, 000h, 001h
        db  0A0h, 0F6h, 01Fh, 040h, 000h
        mov    dx, 3f7h
        out    dx, al
        db  080h, 03Dh, 0FFh, 01Fh, 040h, 000h, 000h
        je     SHORT loc_1_086B
        db  0CDh, 020h, 002h, 000h, 036h, 000h
loc_1_086B:
        db  08Bh, 035h, 0FBh, 01Fh, 040h, 000h
        cmp    esi, 0
        je     SHORT loc_1_0886
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        db  0C7h, 005h, 0FBh, 01Fh, 040h, 000h, 000h, 000h, 000h, 000h
loc_1_0886:
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
        db  0CCh
sub_1_088C:
        enter  0, 0
        pushad
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        popad
        leave
        ret
        db  0CCh, 0CCh, 0CCh
sub_1_08A0:
        enter  0, 0
        pushfd
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        pop    ecx
        popfd
        leave
        ret
sub_1_08B8:
        enter  0, 0
        pushfd
        push   ecx
        mov    ecx, dword ptr [ebp + 8]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_1_08CB
        xor    eax, eax
loc_1_08CB:
        pop    ecx
        popfd
        leave
        ret
        db  0CCh
sub_1_08D0:
        enter  0, 0
        pushfd
        pushad
        cld
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        mov    bh, byte ptr [esi + 6]
        mov    bl, byte ptr [esi]
        mov    al, byte ptr [esi + 2]
        xor    edx, edx
        mov    dx, word ptr [esi + 18h]
        mov    ecx, edx
        add    ecx, 1
        ror    ecx, 10h
        or     edx, ecx
        test   al, 8
        je     SHORT loc_1_0916
        and    al, 0f7h
        add    edx, 2
        out    dx, al
        sub    edx, 2
        mov    byte ptr [esi + 2], al
        movzx  ecx, byte ptr [esi + 0bh]
        cmp    ecx, 0
        je     SHORT loc_1_0912
loc_1_090E:
        in     al, 21h
        loop   loc_1_090E
loc_1_0912:
        and    byte ptr [esi + 3], 0f7h
loc_1_0916:
        movzx  ecx, word ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        movzx  esi, byte ptr [esi + 0bh]
loc_1_0921:
        xor    bl, 80h
        mov    al, bl
        out    dx, al
        cmp    esi, 0
        je     SHORT loc_1_0933
        push   esi
loc_1_092D:
        in     al, 21h
        dec    esi
        jne    SHORT loc_1_092D
        pop    esi
loc_1_0933:
        ror    edx, 10h
        in     al, dx
        mov    ah, al
        and    ah, 0f0h
        ror    edx, 10h
        xor    bl, 80h
        mov    al, bl
        out    dx, al
        cmp    esi, 0
        je     SHORT loc_1_0951
        push   esi
loc_1_094B:
        in     al, 21h
        dec    esi
        jne    SHORT loc_1_094B
        pop    esi
loc_1_0951:
        ror    edx, 10h
        in     al, dx
        shr    al, 1
        shr    al, 1
        shr    al, 1
        shr    al, 1
        or     al, ah
        xor    al, 88h
        stosb
        xor    bh, al
        ror    edx, 10h
        loop   loc_1_0921
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        mov    byte ptr [esi], bl
        mov    byte ptr [esi + 6], bh
        popad
        popfd
        leave
        ret
sub_1_0978:
        enter  0, 0
        pushfd
        pushad
        cld
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        mov    bh, byte ptr [esi + 6]
        mov    bl, byte ptr [esi + 2]
        xor    edx, edx
        mov    dx, word ptr [esi + 18h]
        mov    ecx, edx
        add    ecx, 2
        ror    edx, 10h
        or     edx, ecx
        mov    ch, byte ptr [esi + 21h]
        cmp    ch, 1
        jne    SHORT loc_1_09B8
        and    byte ptr [esi + 22h], 7fh
        mov    al, byte ptr [esi + 22h]
        out    65h, al
        and    bl, 0f7h
        mov    al, bl
        out    dx, al
        and    byte ptr [esi + 3], 0f7h
        jmp    SHORT loc_1_09CF
loc_1_09B8:
        cmp    ch, 4
        jne    SHORT loc_1_09C1
        mov    cl, 80h
        jmp    SHORT loc_1_09C3
loc_1_09C1:
        mov    cl, 20h
loc_1_09C3:
        or     bl, cl
        and    bl, 0f7h
        mov    al, bl
        out    dx, al
        and    byte ptr [esi + 3], 0f7h
loc_1_09CF:
        push   ecx
        movzx  ecx, word ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        movzx  esi, byte ptr [esi + 0bh]
loc_1_09DB:
        xor    bl, 2
        mov    al, bl
        out    dx, al
        cmp    esi, 0
        je     SHORT loc_1_09ED
        push   esi
loc_1_09E7:
        in     al, 21h
        dec    esi
        jne    SHORT loc_1_09E7
        pop    esi
loc_1_09ED:
        ror    edx, 10h
        in     al, dx
        xor    bh, al
        stosb
        ror    edx, 10h
        loop   loc_1_09DB
        pop    ecx
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        cmp    ch, 1
        jne    SHORT loc_1_0A11
        or     cl, 80h
        mov    al, cl
        out    65h, al
        mov    byte ptr [esi + 22h], al
        jmp    SHORT loc_1_0A18
loc_1_0A11:
        not    cl
        and    bl, cl
        mov    al, bl
        out    dx, al
loc_1_0A18:
        mov    byte ptr [esi + 2], bl
        mov    byte ptr [esi + 6], bh
        popad
        popfd
        leave
        ret
        db  0CCh, 0CCh
sub_1_0A24:
        enter  0, 0
        pushfd
        pushad
        cld
        db  08Bh, 03Dh, 014h, 0D0h, 040h, 000h
        mov    bl, byte ptr [edi + 2]
        mov    bh, byte ptr [edi + 6]
        xor    edx, edx
        mov    dx, word ptr [edi + 18h]
        mov    ecx, edx
        add    ecx, 2
        ror    edx, 10h
        or     edx, ecx
        movzx  ecx, word ptr [ebp + 0ch]
        test   bl, 28h
        je     SHORT loc_1_0A5A
        and    bl, 0d7h
        mov    al, bl
        out    dx, al
        and    byte ptr [edi + 3], 0f7h
loc_1_0A5A:
        ror    edx, 10h
        movzx  edi, byte ptr [edi + 0ah]
        mov    esi, dword ptr [ebp + 8]
loc_1_0A64:
        cmp    edi, 0
        je     SHORT loc_1_0A70
        push   edi
loc_1_0A6A:
        in     al, 21h
        dec    edi
        jne    SHORT loc_1_0A6A
        pop    edi
loc_1_0A70:
        lodsb
        xor    bh, al
        out    dx, al
        ror    edx, 10h
        xor    bl, 2
        mov    al, bl
        out    dx, al
        ror    edx, 10h
        loop   loc_1_0A64
        db  08Bh, 03Dh, 014h, 0D0h, 040h, 000h
        dec    esi
        mov    al, byte ptr [esi]
        mov    byte ptr [edi], al
        mov    byte ptr [edi + 2], bl
        mov    byte ptr [edi + 6], bh
        popad
        popfd
        leave
        ret
        db  0CCh
sub_1_0A98:
        enter  0, 0
        pushfd
        pushad
        cld
        db  08Bh, 03Dh, 014h, 0D0h, 040h, 000h
        mov    ah, byte ptr [edi]
        mov    bl, byte ptr [edi + 2]
        mov    bh, byte ptr [edi + 6]
        xor    edx, edx
        mov    dx, word ptr [edi + 18h]
        mov    ecx, edx
        add    ecx, 2
        ror    edx, 10h
        or     edx, ecx
        movzx  ecx, word ptr [ebp + 0ch]
        test   bl, 28h
        je     SHORT loc_1_0AD0
        and    bl, 0d7h
        mov    al, bl
        out    dx, al
        and    byte ptr [edi + 3], 0f7h
loc_1_0AD0:
        ror    edx, 10h
        movzx  edi, byte ptr [edi + 9]
        mov    esi, dword ptr [ebp + 8]
loc_1_0ADA:
        cmp    edi, 0
        je     SHORT loc_1_0AE6
        push   edi
loc_1_0AE0:
        in     al, 21h
        dec    edi
        jne    SHORT loc_1_0AE0
        pop    edi
loc_1_0AE6:
        lodsb
        xor    bh, al
        cmp    al, ah
        je     SHORT loc_1_0AF4
        mov    ah, al
        out    dx, al
        loop   loc_1_0ADA
        jmp    SHORT loc_1_0B02
loc_1_0AF4:
        ror    edx, 10h
        xor    bl, 2
        mov    al, bl
        out    dx, al
        ror    edx, 10h
        loop   loc_1_0ADA
loc_1_0B02:
        db  08Bh, 03Dh, 014h, 0D0h, 040h, 000h
        mov    byte ptr [edi], ah
        mov    byte ptr [edi + 2], bl
        mov    byte ptr [edi + 6], bh
        popad
        popfd
        leave
        ret
sub_1_0B14:
        enter  0, 0
        pushfd
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        popfd
        leave
        ret
        db  0CCh
sub_1_0B28:
        enter  0, 0
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        db  08Bh, 03Dh, 014h, 0D0h, 040h, 000h
        mov    ah, byte ptr [edi]
        mov    bl, byte ptr [edi + 2]
        mov    bh, byte ptr [edi + 7]
        xor    edx, edx
        mov    dx, word ptr [edi + 18h]
        mov    ecx, edx
        add    edx, 2
        ror    ecx, 10h
        or     edx, ecx
        xor    ecx, ecx
        test   bl, 8
        jne    SHORT loc_1_0B5D
        inc    ecx
        or     bl, 8
        mov    al, bl
        out    dx, al
loc_1_0B5D:
        mov    al, byte ptr [ebp + 8]
        or     al, 80h
        ror    edx, 10h
        xor    bh, al
        test   byte ptr [edi + 3], 10h
        je     SHORT loc_1_0B72
        cmp    ecx, 0
        jne    SHORT loc_1_0BA1
loc_1_0B72:
        cmp    al, ah
        je     SHORT loc_1_0B7B
        out    dx, al
        mov    ah, al
        jmp    SHORT loc_1_0B87
loc_1_0B7B:
        xor    bl, 2
        mov    al, bl
        ror    edx, 10h
        out    dx, al
        ror    edx, 10h
loc_1_0B87:
        test   ah, 70h
        je     SHORT loc_1_0B92
        movzx  ecx, byte ptr [edi + 0ch]
        jmp    SHORT loc_1_0B96
loc_1_0B92:
        movzx  ecx, byte ptr [edi + 0dh]
loc_1_0B96:
        cmp    ecx, 0
        je     SHORT loc_1_0B9F
loc_1_0B9B:
        in     al, 21h
        loop   loc_1_0B9B
loc_1_0B9F:
        jmp    SHORT loc_1_0BC9
loc_1_0BA1:
        out    dx, al
        mov    ah, al
        ror    edx, 10h
        test   byte ptr [edi + 3], 3
        je     SHORT loc_1_0BB3
        and    bl, 0dfh
        mov    al, bl
        out    dx, al
loc_1_0BB3:
        xor    bl, 2
        mov    al, bl
        out    dx, al
        ror    edx, 10h
        movzx  ecx, byte ptr [edi + 0eh]
        cmp    ecx, 0
        je     SHORT loc_1_0BC9
loc_1_0BC5:
        in     al, 21h
        loop   loc_1_0BC5
loc_1_0BC9:
        mov    byte ptr [edi + 2], bl
        inc    edx
        in     al, dx
        and    al, 0f0h
        xchg   al, ah
        and    al, 7fh
        dec    dx
        out    dx, al
        mov    byte ptr [edi], al
        inc    edx
        in     al, dx
        shr    al, 1
        shr    al, 1
        shr    al, 1
        shr    al, 1
        or     al, ah
        xor    al, 88h
        xor    bh, al
        mov    byte ptr [edi + 7], bh
        xor    ah, ah
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh
sub_1_0BF8:
        enter  0, 0
        push   ebx
        push   ecx
        push   edx
        push   edi
        db  08Bh, 03Dh, 014h, 0D0h, 040h, 000h
        mov    bl, byte ptr [edi + 2]
        mov    bh, byte ptr [edi + 7]
        mov    dx, word ptr [edi + 18h]
        mov    ecx, edx
        add    edx, 2
        test   bl, 8
        jne    SHORT loc_1_0C20
        or     bl, 8
        mov    al, bl
        out    dx, al
loc_1_0C20:
        mov    al, byte ptr [ebp + 8]
        or     al, 80h
        xchg   ecx, edx
        out    dx, al
        xor    bh, al
        xor    bl, 2
        xchg   bl, al
        xchg   ecx, edx
        out    dx, al
        mov    byte ptr [edi + 2], al
        push   ecx
        test   bl, 70h
        je     SHORT loc_1_0C41
        movzx  ecx, byte ptr [edi + 0ch]
        jmp    SHORT loc_1_0C45
loc_1_0C41:
        movzx  ecx, byte ptr [edi + 0dh]
loc_1_0C45:
        cmp    ecx, 0
        je     SHORT loc_1_0C4E
loc_1_0C4A:
        in     al, 21h
        loop   loc_1_0C4A
loc_1_0C4E:
        pop    ecx
        add    edx, -1
        in     al, dx
        and    al, 0f0h
        mov    ah, al
        mov    al, bl
        and    al, 7fh
        xchg   ecx, edx
        out    dx, al
        mov    byte ptr [edi], al
        push   ecx
        movzx  ecx, byte ptr [edi + 0ch]
        cmp    ecx, 0
        je     SHORT loc_1_0C6E
loc_1_0C6A:
        in     al, 21h
        loop   loc_1_0C6A
loc_1_0C6E:
        pop    ecx
        xchg   ecx, edx
        in     al, dx
        shr    al, 1
        shr    al, 1
        shr    al, 1
        shr    al, 1
        or     al, ah
        xor    al, 88h
        xor    bh, al
        mov    byte ptr [edi + 7], bh
        xor    ah, ah
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        leave
        ret
        db  0CCh
sub_1_0C8C:
        enter  0, 0
        push   ecx
        push   eax
        db  0A1h, 0EDh, 01Fh, 040h, 000h
        cmp    eax, 0
        je     SHORT loc_1_0CB2
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, 0
        jne    SHORT loc_1_0CAC
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_1_0CB2
loc_1_0CAC:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_1_0CB2:
        pop    eax
        pop    ecx
        leave
        ret
        db  0CCh, 0CCh
sub_1_0CB8:
        enter  0, 0
        pushfd
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        popfd
        leave
        ret
        db  0CCh, 0CCh, 0CCh
sub_1_0CCC:
        enter  0, 0
        pushfd
        push   esi
        push   edx
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 0
        je     SHORT loc_1_0CE9
        mov    edx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
loc_1_0CE9:
        pop    edx
        pop    esi
        popfd
        leave
        ret
        db  0CCh, 0CCh
sub_1_0CF0:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 004h, 000h, 005h, 000h
        leave
        ret
        db  0CCh
sub_1_0D00:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 005h, 000h
        leave
        ret
        db  0CCh
sub_1_0D10:
        push   edi
        push   esi
        push   ebx
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 14h]
        pushfd
        push   edx
        db  080h, 03Dh, 081h, 0CFh, 040h, 000h, 001h
        jne    SHORT loc_1_0D3E
        mov    al, 90h
        or     al, cl
        mov    dx, 18h
        out    dx, al
        mov    al, 50h
        or     al, cl
        out    dx, al
        mov    dx, 1ah
        in     al, dx
        mov    ah, al
        in     al, dx
        jmp    SHORT loc_1_0D6E
loc_1_0D3E:
        movzx  eax, cx
        mov    edi, 0eh
        mul    edi
        db  005h, 000h, 020h, 040h, 000h
        mov    edi, eax
        mov    al, byte ptr [edi + 0ch]
        or     al, 4
        mov    dx, word ptr [edi]
        out    dx, al
        call   sub_1_3405
        mov    dx, word ptr [edi + 6]
        in     al, dx
        mov    ah, al
        mov    dx, word ptr [edi + 6]
        in     al, dx
        call   sub_1_33EF
loc_1_0D6E:
        xchg   al, ah
        inc    ax
        movzx  eax, ax
        pop    edx
        cmp    dx, 0
        je     SHORT loc_1_0D7E
        shl    eax, 1
loc_1_0D7E:
        popfd
        pop    ebx
        pop    esi
        pop    edi
        ret
        db  0CCh
sub_1_0D84:
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [esp + 10h]
        mov    esi, edi
        pushfd
        db  083h, 03Dh, 014h, 0D0h, 040h, 000h, 000h
        je     NEAR PTR loc_1_0E58
        db  08Bh, 03Dh, 014h, 0D0h, 040h, 000h
        movzx  eax, byte ptr [edi + 3]
        and    eax, 0ffffffbfh
        push   eax
        push   18h
        call   sub_1_1D05
        add    esp, 8
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_1_0DCF
        movzx  eax, byte ptr [edi + 3]
        and    eax, 0ffffff7fh
        push   eax
        push   18h
        call   sub_1_1D05
        add    esp, 8
        jmp    SHORT loc_1_0DE3
loc_1_0DCF:
        movzx  eax, byte ptr [edi + 3]
        or     eax, 80h
        push   eax
        push   18h
        call   sub_1_1D05
        add    esp, 8
loc_1_0DE3:
        test   byte ptr [edi + 8], 2
        je     SHORT loc_1_0DEB
        shl    dword ptr [esi], 1
loc_1_0DEB:
        mov    eax, dword ptr [esi]
        push   eax
        push   15h
        call   sub_1_1D05
        add    esp, 8
        mov    eax, dword ptr [esi]
        shr    eax, 8
        push   eax
        push   16h
        call   sub_1_1D05
        add    esp, 8
        mov    eax, dword ptr [esi]
        shr    eax, 10h
        push   eax
        push   17h
        call   sub_1_1D05
        add    esp, 8
        mov    eax, dword ptr [esi + 4]
        push   eax
        push   13h
        call   sub_1_1D05
        add    esp, 8
        mov    eax, dword ptr [esi + 4]
        shr    eax, 8
        push   eax
        push   14h
        call   sub_1_1D05
        add    esp, 8
        push   2
        call   sub_1_3B66
        add    esp, 4
        movzx  eax, byte ptr [edi + 3]
        or     eax, 40h
        push   eax
        push   18h
        call   sub_1_1D05
        add    esp, 8
        jmp    NEAR PTR loc_1_0F66
loc_1_0E58:
        db  080h, 03Dh, 081h, 0CFh, 040h, 000h, 001h
        jne    NEAR PTR loc_1_0EEC
        mov    al, 90h
        or     al, byte ptr [esi + 0ch]
        mov    dx, 18h
        out    dx, al
        mov    al, 4
        cmp    dword ptr [esi + 8], 0
        jne    SHORT loc_1_0E79
        or     al, 8
loc_1_0E79:
        mov    ebx, dword ptr [esi + 4]
        cmp    dword ptr [esi + 14h], 0
        je     SHORT loc_1_0E86
        shr    ebx, 1
        or     al, 40h
loc_1_0E86:
        cmp    dword ptr [esi + 10h], 0
        je     SHORT loc_1_0EA5
        or     al, 1
        push   eax
        mov    eax, dword ptr [esi + 10h]
        mov    ecx, 2
        mov    edx, 0
        or     edx, dword ptr [esi + 0ch]
        call   sub_1_0F6C
        pop    eax
loc_1_0EA5:
        mov    edx, 70h
        or     edx, dword ptr [esi + 0ch]
        mov    ecx, 1
        call   sub_1_0F6C
        mov    eax, ebx
        dec    eax
        mov    ecx, 2
        mov    edx, 40h
        or     edx, dword ptr [esi + 0ch]
        call   sub_1_0F6C
        mov    eax, dword ptr [esi]
        mov    ecx, 3
        mov    edx, 20h
        or     edx, dword ptr [esi + 0ch]
        call   sub_1_0F6C
        mov    al, 0a0h
        or     al, byte ptr [esi + 0ch]
        mov    dx, 18h
        out    dx, al
        jmp    SHORT loc_1_0F66
loc_1_0EEC:
        mov    eax, dword ptr [esi + 0ch]
        mov    edi, 0eh
        mul    edi
        db  005h, 000h, 020h, 040h, 000h
        mov    edi, eax
        mov    al, byte ptr [edi + 0ch]
        or     al, 4
        mov    dx, word ptr [edi]
        out    dx, al
        mov    eax, dword ptr [esi]
        shr    eax, 10h
        mov    dx, word ptr [edi + 8]
        out    dx, al
        mov    ebx, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        cmp    byte ptr [edi + 0dh], 0
        je     SHORT loc_1_0F1F
        shr    ebx, 1
        shr    ecx, 1
loc_1_0F1F:
        call   sub_1_3405
        mov    dx, word ptr [edi + 2]
        out    dx, al
        mov    al, bl
        mov    dx, word ptr [edi + 4]
        out    dx, al
        mov    al, bh
        mov    dx, word ptr [edi + 4]
        out    dx, al
        dec    ecx
        mov    al, cl
        mov    dx, word ptr [edi + 6]
        out    dx, al
        mov    al, ch
        mov    dx, word ptr [edi + 6]
        out    dx, al
        call   sub_1_33EF
        mov    al, 4
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_1_0F55
        mov    al, 8
loc_1_0F55:
        or     al, 40h
        or     al, byte ptr [edi + 0ch]
        mov    dx, word ptr [edi + 0ah]
        out    dx, al
        mov    al, byte ptr [edi + 0ch]
        mov    dx, word ptr [edi]
        out    dx, al
loc_1_0F66:
        popfd
        pop    ebx
        pop    esi
        pop    edi
        ret
        db  0CCh
sub_1_0F6C:
        push   eax
        mov    al, dl
        mov    dx, 18h
        out    dx, al
        pop    eax
        mov    dx, 1ah
loc_1_0F79:
        out    dx, al
        shr    eax, 8
        loop   loc_1_0F79
        ret
sub_1_0F80:
        pushfd
        push   ebx
        push   ecx
        push   edx
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        pop    edx
        pop    ecx
        pop    ebx
        popfd
        ret
        db  0CCh
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 002h, 002h, 000h, 000h, 056h, 051h, 049h, 043h
        db  031h, 031h, 037h, 020h, 000h, 000h, 000h, 080h, 000h, 010h, 040h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h
sub_1_0FE4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0FE8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0FEC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0FF6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1000:
        or     al, byte ptr [eax]
        or     al, 0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        xchg   dword ptr [eax], eax
        or     eax, dword ptr [eax]
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        or     al, 0
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    dword ptr [eax], 0bh
        add    byte ptr [ecx], al
        add    byte ptr [edx], cl
        add    byte ptr [eax + eax], cl
        add    al, 0
        add    eax, 0b008100h
        add    byte ptr [edx], al
        add    byte ptr [edx], cl
        add    byte ptr [eax + eax], cl
        push   es
        add    byte ptr [edi], al
        add    byte ptr [edx + 3000b00h], al
        add    ah, dl
        add    al, bl
        add    al, al
        add    dl, al
        add    byte ptr [edi + 0d600h], cl
        add    esp, edx
        add    al, bl
        add    ah, al
        add    dh, al
        add    byte ptr [ebx + 100d600h], cl
        add    esp, edx
        add    al, bl
        add    al, cl
        add    dl, cl
        add    byte ptr [ecx + 200d600h], cl
        add    esp, edx
        add    al, bl
        add    ah, cl
        add    dh, cl
        add    byte ptr [edx + 300d600h], cl
        add    dword ptr [eax], eax
        add    byte ptr [ebx], al
        db  000h, 030h
        pop    ss
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 054h, 017h, 040h
        db  000h, 060h, 017h
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, dh
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
sub_1_1090:
        db  080h, 03Dh, 082h, 0CDh, 040h, 000h, 022h
        push   ebx
        push   esi
        jne    SHORT loc_1_10A8
        db  068h, 048h, 0CDh, 040h, 000h
        call   sub_1_110B
        add    esp, 4
loc_1_10A8:
        db  066h, 083h, 03Dh, 09Ch, 0CFh, 040h, 000h, 000h
        je     SHORT loc_1_10F4
        mov    bl, 0
        jbe    SHORT loc_1_10EB
loc_1_10B6:
        movzx  eax, bl
        shl    eax, 4
        db  08Dh, 0B0h, 0ACh, 0CFh, 040h, 000h
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_1_10DB
        push   eax
        call   sub_1_0304
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_10DB
        mov    dword ptr [esi], 0
loc_1_10DB:
        inc    bl
        db  00Fh, 0B7h, 00Dh, 09Ch, 0CFh, 040h, 000h
        movzx  eax, bl
        cmp    eax, ecx
        jb     SHORT loc_1_10B6
loc_1_10EB:
        db  066h, 0C7h, 005h, 09Ch, 0CFh, 040h, 000h, 000h, 000h
loc_1_10F4:
        push   1
        db  068h, 078h, 0CEh, 040h, 000h
        db  068h, 080h, 0CFh, 040h, 000h
        call   sub_1_1BF3
        add    esp, 0ch
        pop    esi
        pop    ebx
        ret
sub_1_110B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 3ah], 22h
        jne    SHORT loc_1_1163
        cmp    dword ptr [esi + 10h], 0
        je     SHORT loc_1_1155
        db  0A1h, 014h, 0D0h, 040h, 000h
        mov    al, byte ptr [eax + 8]
        or     al, 4
        push   eax
        push   1eh
        call   sub_1_1D05
        add    esp, 8
        call   sub_1_1CD7
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     SHORT loc_1_114C
        db  068h, 0D4h, 02Ch, 040h, 000h
        push   dword ptr [esi + 10h]
        push   3
        call   eax
        add    esp, 0ch
loc_1_114C:
        mov    dword ptr [esi + 10h], 0
        jmp    SHORT loc_1_1177
loc_1_1155:
        push   2
        push   1587ah
        push   1513h
        jmp    SHORT loc_1_116F
loc_1_1163:
        push   1
        push   1587ah
        push   1514h
loc_1_116F:
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_1177:
        pop    esi
        ret
sub_1_1179:
        push   ebp
        mov    eax, 0b4h
        mov    ebp, esp
        sub    esp, 0d8h
        mov    ecx, 76h
        push   ebx
        push   esi
        mov    byte ptr [ebp - 0d8h], al
        push   edi
        mov    edx, 0f6h
        mov    ebx, 0b6h
        mov    byte ptr [ebp - 0d5h], cl
        mov    byte ptr [ebp - 0d4h], dl
        mov    byte ptr [ebp - 0d3h], bl
        mov    byte ptr [ebp - 0d7h], 34h
        mov    byte ptr [ebp - 0d6h], 74h
        mov    byte ptr [ebp - 0d1h], 0f4h
        mov    byte ptr [ebp - 0d0h], 0fch
        mov    byte ptr [ebp - 0d2h], dl
        mov    byte ptr [ebp - 0cch], dl
        mov    byte ptr [ebp - 0cfh], 7ch
        mov    byte ptr [ebp - 0ceh], 7eh
        mov    byte ptr [ebp - 0cbh], cl
        mov    byte ptr [ebp - 0c8h], al
        mov    byte ptr [ebp - 0c7h], bl
        mov    byte ptr [ebp - 0c6h], al
        mov    byte ptr [ebp - 0cdh], 0feh
        mov    byte ptr [ebp - 0cah], 74h
        mov    byte ptr [ebp - 0c9h], 34h
        mov    byte ptr [ebp - 0c2h], dl
        mov    byte ptr [ebp - 0c1h], bl
        mov    byte ptr [ebp - 0c0h], al
        mov    byte ptr [ebp - 0bfh], al
        mov    byte ptr [ebp - 0c5h], 0bch
        mov    byte ptr [ebp - 0c4h], 0fch
        mov    byte ptr [ebp - 0bch], cl
        mov    byte ptr [ebp - 0bbh], dl
        mov    byte ptr [ebp - 0bah], bl
        mov    byte ptr [ebp - 0b9h], dl
        mov    byte ptr [ebp - 0c3h], 0f4h
        mov    byte ptr [ebp - 0beh], 34h
        mov    byte ptr [ebp - 0bdh], 74h
        mov    byte ptr [ebp - 0b8h], 0f4h
        mov    byte ptr [ebp - 0b7h], 0fch
        mov    byte ptr [ebp - 0b3h], dl
        mov    byte ptr [ebp - 0b2h], cl
        mov    byte ptr [ebp - 0b6h], 7ch
        mov    byte ptr [ebp - 0b5h], 7eh
        mov    byte ptr [ebp - 0afh], al
        mov    byte ptr [ebp - 0aeh], bl
        mov    byte ptr [ebp - 0b4h], 0feh
        mov    byte ptr [ebp - 0b1h], 74h
        mov    byte ptr [ebp - 0b0h], 34h
        mov    byte ptr [ebp - 0adh], dl
        mov    byte ptr [ebp - 0a9h], dl
        mov    byte ptr [ebp - 0a8h], bl
        mov    byte ptr [ebp - 0a7h], al
        mov    byte ptr [ebp - 0ach], 0f4h
        mov    byte ptr [ebp - 0abh], 0fch
        mov    byte ptr [ebp - 0aah], 0f4h
        mov    byte ptr [ebp - 0a6h], al
        mov    byte ptr [ebp - 0a3h], cl
        mov    byte ptr [ebp - 0a2h], dl
        mov    byte ptr [ebp - 0a1h], bl
        mov    byte ptr [ebp - 0a0h], dl
        mov    byte ptr [ebp - 0a5h], 34h
        mov    byte ptr [ebp - 0a4h], 74h
        mov    byte ptr [ebp - 9fh], 0f4h
        mov    byte ptr [ebp - 9eh], 0fch
        mov    byte ptr [ebp - 9bh], cl
        mov    byte ptr [ebp - 99h], cl
        mov    byte ptr [ebp - 9dh], 7ch
        mov    byte ptr [ebp - 9ch], 7eh
        mov    byte ptr [ebp - 96h], al
        mov    byte ptr [ebp - 95h], bl
        mov    byte ptr [ebp - 9ah], 36h
        mov    byte ptr [ebp - 98h], 74h
        mov    byte ptr [ebp - 97h], 34h
        mov    byte ptr [ebp - 94h], al
        mov    byte ptr [ebp - 90h], dl
        mov    byte ptr [ebp - 8fh], bl
        mov    byte ptr [ebp - 8eh], al
        mov    byte ptr [ebp - 93h], 0bch
        mov    byte ptr [ebp - 92h], 0fch
        mov    byte ptr [ebp - 8dh], al
        mov    byte ptr [ebp - 8ah], cl
        mov    byte ptr [ebp - 89h], dl
        mov    byte ptr [ebp - 88h], bl
        mov    byte ptr [ebp - 91h], 0f4h
        mov    byte ptr [ebp - 8ch], 34h
        mov    byte ptr [ebp - 8bh], 74h
        mov    byte ptr [ebp - 86h], 0f4h
        mov    byte ptr [ebp - 87h], dl
        mov    byte ptr [ebp - 82h], cl
        mov    byte ptr [ebp - 85h], 0fch
        mov    byte ptr [ebp - 84h], 7ch
        mov    byte ptr [ebp - 83h], 7eh
        mov    byte ptr [ebp - 80h], cl
        mov    byte ptr [ebp - 7dh], al
        mov    byte ptr [ebp - 7ch], bl
        mov    byte ptr [ebp - 7bh], dl
        mov    byte ptr [ebp - 81h], 36h
        mov    byte ptr [ebp - 7fh], 74h
        mov    byte ptr [ebp - 7eh], 34h
        mov    byte ptr [ebp - 77h], dl
        mov    byte ptr [ebp - 76h], bl
        mov    byte ptr [ebp - 75h], al
        mov    byte ptr [ebp - 74h], al
        mov    byte ptr [ebp - 7ah], 0f4h
        mov    byte ptr [ebp - 79h], 0fch
        mov    byte ptr [ebp - 78h], 0f4h
        mov    byte ptr [ebp - 71h], cl
        mov    byte ptr [ebp - 6fh], cl
        mov    byte ptr [ebp - 73h], 34h
        mov    byte ptr [ebp - 72h], 74h
        mov    byte ptr [ebp - 70h], 36h
        mov    byte ptr [ebp - 6dh], 0f4h
        mov    byte ptr [ebp - 6ch], 0fch
        mov    byte ptr [ebp - 6bh], 7ch
        mov    byte ptr [ebp - 6ah], 7eh
        mov    byte ptr [ebp - 6eh], dl
        mov    byte ptr [ebp - 69h], 0feh
        mov    byte ptr [ebp - 68h], dl
        mov    byte ptr [ebp - 67h], cl
        mov    byte ptr [ebp - 64h], al
        mov    byte ptr [ebp - 63h], bl
        mov    byte ptr [ebp - 66h], 74h
        mov    byte ptr [ebp - 65h], 34h
        mov    byte ptr [ebp - 62h], al
        mov    byte ptr [ebp - 5eh], dl
        mov    byte ptr [ebp - 5dh], bl
        mov    byte ptr [ebp - 5ch], al
        mov    byte ptr [ebp - 61h], 0bch
        mov    byte ptr [ebp - 60h], 0fch
        mov    byte ptr [ebp - 5bh], al
        mov    byte ptr [ebp - 58h], cl
        mov    byte ptr [ebp - 5fh], 0f4h
        mov    byte ptr [ebp - 56h], cl
        mov    byte ptr [ebp - 55h], dl
        mov    byte ptr [ebp - 5ah], 34h
        mov    byte ptr [ebp - 59h], 74h
        mov    byte ptr [ebp - 57h], 36h
        mov    byte ptr [ebp - 54h], 0f4h
        mov    byte ptr [ebp - 53h], 0fch
        mov    byte ptr [ebp - 4fh], dl
        mov    byte ptr [ebp - 4eh], cl
        mov    byte ptr [ebp - 52h], 7ch
        mov    byte ptr [ebp - 51h], 7eh
        mov    byte ptr [ebp - 4bh], al
        mov    byte ptr [ebp - 4ah], bl
        mov    byte ptr [ebp - 50h], 0feh
        mov    byte ptr [ebp - 4dh], 74h
        mov    byte ptr [ebp - 4ch], 34h
        mov    byte ptr [ebp - 49h], dl
        mov    byte ptr [ebp - 45h], dl
        mov    byte ptr [ebp - 44h], bl
        mov    byte ptr [ebp - 43h], al
        mov    byte ptr [ebp - 48h], 0f4h
        mov    byte ptr [ebp - 47h], 0fch
        mov    byte ptr [ebp - 42h], al
        mov    byte ptr [ebp - 3fh], cl
        mov    byte ptr [ebp - 46h], 0f4h
        mov    byte ptr [ebp - 3dh], cl
        mov    byte ptr [ebp - 3ch], dl
        mov    byte ptr [ebp - 41h], 34h
        mov    byte ptr [ebp - 40h], 74h
        mov    byte ptr [ebp - 3eh], 36h
        mov    byte ptr [ebp - 3bh], 0f4h
        mov    byte ptr [ebp - 3ah], 0fch
        mov    byte ptr [ebp - 37h], cl
        mov    byte ptr [ebp - 35h], cl
        mov    byte ptr [ebp - 39h], 7ch
        mov    byte ptr [ebp - 38h], 7eh
        mov    byte ptr [ebp - 32h], al
        mov    byte ptr [ebp - 31h], bl
        mov    byte ptr [ebp - 36h], 36h
        mov    byte ptr [ebp - 34h], 74h
        mov    byte ptr [ebp - 33h], 34h
        mov    byte ptr [ebp - 2fh], 0bch
        mov    byte ptr [ebp - 2eh], 0fch
        mov    byte ptr [ebp - 2dh], 0f4h
        mov    byte ptr [ebp - 30h], al
        mov    byte ptr [ebp - 2ch], dl
        mov    byte ptr [ebp - 2bh], bl
        mov    byte ptr [ebp - 2ah], al
        mov    byte ptr [ebp - 29h], al
        mov    byte ptr [ebp - 26h], cl
        mov    byte ptr [ebp - 24h], cl
        mov    byte ptr [ebp - 23h], dl
        mov    byte ptr [ebp - 28h], 34h
        mov    byte ptr [ebp - 27h], 74h
        mov    byte ptr [ebp - 25h], 36h
        mov    byte ptr [ebp - 22h], 0f4h
        mov    byte ptr [ebp - 21h], 0fch
        mov    byte ptr [ebp - 1eh], cl
        mov    byte ptr [ebp - 1ch], cl
        mov    byte ptr [ebp - 20h], 7ch
        mov    byte ptr [ebp - 1fh], 7eh
        mov    byte ptr [ebp - 19h], al
        mov    byte ptr [ebp - 18h], bl
        mov    byte ptr [ebp - 1dh], 36h
        mov    byte ptr [ebp - 1bh], 74h
        mov    byte ptr [ebp - 1ah], 34h
        mov    byte ptr [ebp - 17h], dl
        mov    byte ptr [ebp - 13h], dl
        mov    byte ptr [ebp - 12h], bl
        mov    byte ptr [ebp - 11h], al
        mov    byte ptr [ebp - 16h], 0f4h
        mov    byte ptr [ebp - 15h], 0fch
        mov    byte ptr [ebp - 14h], 0f4h
        mov    byte ptr [ebp - 10h], 38h
        mov    byte ptr [ebp - 0fh], 28h
        mov    byte ptr [ebp - 0eh], 8
        mov    byte ptr [ebp - 0dh], 0
        mov    byte ptr [ebp - 0ch], 2ah
        mov    byte ptr [ebp - 0bh], 0ah
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [ebp - 0ah], 2
        cmp    byte ptr [esi + 22h], 22h
        jne    NEAR PTR loc_1_1622
        db  0C7h, 045h, 0F8h, 048h, 0CDh, 040h, 000h
        mov    al, byte ptr [esi + 0fh]
        add    al, 30h
        db  0A2h, 04Bh, 0CDh, 040h, 000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0CDh, 020h, 021h, 000h, 02Bh, 000h
        add    esp, 4
        db  0C7h, 045h, 0FCh, 048h, 0CDh, 040h, 000h
        db  0A3h, 050h, 0CDh, 040h, 000h
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 054h, 0CDh, 040h, 000h
        push   eax
        db  0A3h, 054h, 0CDh, 040h, 000h
        db  0CDh, 020h, 022h, 000h, 02Bh, 000h
        db  0A3h, 054h, 0CDh, 040h, 000h
        add    esp, 4
        db  0BFh, 060h, 0CDh, 040h, 000h
        mov    ecx, 45h
        push   0c8h
        db  0A3h, 054h, 0CDh, 040h, 000h
        rep movsd
        db  068h, 09Dh, 0CDh, 040h, 000h
        lea    eax, [ebp - 0d8h]
        push   eax
        call   sub_1_1D93
        add    esp, 0ch
        lea    eax, [ebp - 10h]
        push   7
        db  068h, 065h, 0CEh, 040h, 000h
        push   eax
        call   sub_1_1D93
        add    esp, 0ch
        db  066h, 00Fh, 0B6h, 005h, 06Fh, 0CDh, 040h, 000h
        push   eax
        call   sub_1_0324
        db  066h, 0A3h, 076h, 0CDh, 040h, 000h
        add    esp, 4
        jmp    SHORT loc_1_1633
loc_1_1622:
        xor    eax, eax
        db  0A3h, 050h, 0CDh, 040h, 000h
        db  0A3h, 054h, 0CDh, 040h, 000h
        db  0A3h, 058h, 0CDh, 040h, 000h
loc_1_1633:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        db  0A3h, 05Ch, 0CDh, 040h, 000h
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_1644:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        cmp    byte ptr [esi + 3ah], 22h
        jne    NEAR PTR loc_1_16D9
        mov    eax, dword ptr [esi + 8]
        lea    edi, [esi + 10h]
        test   eax, eax
        je     SHORT loc_1_1678
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_1697
        push   1
        db  068h, 0D4h, 02Ch, 040h, 000h
        push   dword ptr [esi + 0ch]
        push   1
        call   eax
        add    esp, 10h
        mov    dword ptr [edi], eax
        jmp    SHORT loc_1_1692
loc_1_1678:
        push   2
        mov    dword ptr [edi], 0
        push   15878h
        push   1599h
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_1692:
        cmp    dword ptr [edi], 0
        je     SHORT loc_1_16CB
loc_1_1697:
        movzx  eax, word ptr [esi + 2eh]
        push   eax
        add    esi, 18h
        call   sub_1_0348
        add    esp, 4
        db  0A1h, 090h, 0CFh, 040h, 000h
        push   eax
        push   esi
        call   sub_1_1DDF
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_1_16EF
        db  068h, 048h, 0CDh, 040h, 000h
        call   sub_1_110B
        add    esp, 4
        jmp    SHORT loc_1_16EF
loc_1_16CB:
        push   3
        push   15878h
        push   15a1h
        jmp    SHORT loc_1_16E5
loc_1_16D9:
        push   1
        push   15878h
        push   1514h
loc_1_16E5:
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, eax
loc_1_16EF:
        mov    eax, esi
        pop    edi
        pop    esi
        ret
sub_1_16F4:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        db  089h, 01Dh, 00Ch, 0D0h, 040h, 000h
        db  089h, 01Dh, 010h, 0D0h, 040h, 000h
        cmp    dword ptr [esp + 14h], ebx
        jne    NEAR PTR loc_1_17A6
        db  066h, 089h, 01Dh, 09Ch, 0CFh, 040h, 000h
        db  038h, 01Dh, 0FFh, 01Fh, 040h, 000h
        je     SHORT loc_1_1784
        mov    esi, dword ptr [esp + 10h]
loc_1_1722:
        movzx  edi, bl
        shl    edi, 4
        db  08Dh, 087h, 0A8h, 0CFh, 040h, 000h
        db  08Dh, 08Fh, 0A4h, 0CFh, 040h, 000h
        push   eax
        push   ecx
        push   0eh
        push   7
        push   8
        push   esi
        call   sub_1_03C0
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_1_1756
        db  066h, 0FFh, 005h, 09Ch, 0CFh, 040h, 000h
        db  089h, 087h, 0ACh, 0CFh, 040h, 000h
loc_1_1756:
        inc    bl
        cmp    bl, 4
        jae    SHORT loc_1_1761
        test   eax, eax
        jne    SHORT loc_1_1722
loc_1_1761:
        db  066h, 083h, 03Dh, 09Ch, 0CFh, 040h, 000h, 002h
        jae    SHORT loc_1_1784
        push   1
        push   15810h
        push   1503h
        call   sub_1_1CB6
        add    esp, 0ch
        db  0A3h, 00Ch, 0D0h, 040h, 000h
loc_1_1784:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_17BF
        push   1
        push   15810h
        push   1583h
        call   sub_1_1CB6
        add    esp, 0ch
        db  0A3h, 010h, 0D0h, 040h, 000h
        jmp    SHORT loc_1_17BF
loc_1_17A6:
        push   1
        push   15810h
        push   1584h
        call   sub_1_1CB6
        add    esp, 0ch
        db  0A3h, 00Ch, 0D0h, 040h, 000h
loc_1_17BF:
        call   sub_1_0388
        and    al, 0fh
        pop    edi
        pop    esi
        db  0A2h, 081h, 0CFh, 040h, 000h
        db  0C6h, 005h, 080h, 0CFh, 040h, 000h, 000h
        pop    ebx
        ret
sub_1_17D6:
        push   esi
        xor    eax, eax
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [esi], eax
        mov    edx, dword ptr [ecx]
        test   edx, edx
        je     SHORT loc_1_1803
        mov    cx, word ptr [esp + 0ch]
loc_1_17EE:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_181C
        cmp    word ptr [edx], cx
        jne    SHORT loc_1_17FC
        mov    dword ptr [esi], edx
        jmp    SHORT loc_1_17FF
loc_1_17FC:
        mov    edx, dword ptr [edx + 0ch]
loc_1_17FF:
        test   edx, edx
        jne    SHORT loc_1_17EE
loc_1_1803:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_181C
        push   1
        push   15827h
        push   1513h
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_181C:
        pop    esi
        ret
sub_1_181E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi], 0
        je     NEAR PTR loc_1_1938
        lea    eax, [ebp - 4]
        mov    ebx, dword ptr [ebp + 10h]
        mov    cx, word ptr [ebx + 1ah]
        push   eax
        push   ecx
        push   dword ptr [ebp + 0ch]
        call   sub_1_17D6
        add    esp, 0ch
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_194E
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [ecx + 378h]
        push   dword ptr [edx]
        push   ecx
        push   dword ptr [esi + 10h]
        call   sub_1_21E0
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        mov    eax, dword ptr [edx + 378h]
        push   dword ptr [eax]
        push   ebx
        call   sub_1_219F
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 8]
        add    eax, 378h
        push   eax
        call   sub_1_1F57
        add    esp, 4
        cmp    word ptr [ebx + 18h], 1112h
        jne    SHORT loc_1_18AE
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        mov    byte ptr [ecx + 4ach], 0ffh
loc_1_18AE:
        test   edi, edi
        jne    SHORT loc_1_190E
loc_1_18B2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        mov    eax, dword ptr [ecx + 250h]
        test   eax, eax
        je     SHORT loc_1_190E
        push   dword ptr [eax]
        push   ecx
        push   dword ptr [esi + 10h]
        call   sub_1_2001
        add    esp, 0ch
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_190E
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 8]
        mov    eax, dword ptr [edx + 250h]
        add    edx, 128h
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        push   edx
        call   sub_1_1F88
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        add    eax, 250h
        push   eax
        call   sub_1_1F57
        add    esp, 4
        test   edi, edi
        je     SHORT loc_1_18B2
loc_1_190E:
        xor    edi, edi
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        cmp    dword ptr [eax + 4a8h], edi
        jne    SHORT loc_1_194E
        cmp    dword ptr [eax + 128h], edi
        je     SHORT loc_1_194E
        push   1
        push   dword ptr [eax + 4a4h]
        call   sub_1_1F1A
        add    esp, 8
        jmp    SHORT loc_1_194E
loc_1_1938:
        push   1
        push   15816h
        push   1581h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    edi, eax
loc_1_194E:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_1957:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 4
        push   esi
        cmp    byte ptr [eax], 0
        push   edi
        je     SHORT loc_1_1997
        lea    eax, [esp + 8]
        mov    edi, dword ptr [esp + 18h]
        mov    cx, word ptr [edi + 1ah]
        push   eax
        push   ecx
        push   dword ptr [esp + 1ch]
        call   sub_1_17D6
        add    esp, 0ch
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_19AD
        mov    eax, dword ptr [esp + 8]
        push   edi
        push   dword ptr [eax + 4]
        call   sub_1_2310
        add    esp, 8
        jmp    SHORT loc_1_19AD
loc_1_1997:
        push   1
        push   15815h
        push   1581h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, eax
loc_1_19AD:
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 4
        ret
sub_1_19B5:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 24h]
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        cmp    byte ptr [esi], bl
        je     NEAR PTR loc_1_1BD3
        mov    eax, dword ptr [esp + 30h]
        cmp    eax, ebx
        je     SHORT loc_1_19FF
        push   14h
        push   eax
        call   sub_1_0450
        add    esp, 8
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_1_1A19
        push   2
        push   15814h
        push   1585h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
        jmp    SHORT loc_1_1A19
loc_1_19FF:
        push   1
        push   15814h
        push   1585h
        call   sub_1_1CB6
        mov    ebp, dword ptr [esp + 24h]
        add    esp, 0ch
        mov    ebx, eax
loc_1_1A19:
        test   ebx, ebx
        jne    SHORT loc_1_1A39
        mov    cx, word ptr [edi + 1ah]
        lea    eax, [esp + 10h]
        push   eax
        push   ecx
        push   dword ptr [esp + 2ch]
        call   sub_1_17D6
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_1_1A4D
loc_1_1A39:
        mov    ax, word ptr [edi + 18h]
        cmp    ax, 1101h
        je     SHORT loc_1_1A4D
        cmp    ax, 1100h
        jne    NEAR PTR loc_1_1BBE
loc_1_1A4D:
        movzx  eax, word ptr [edi + 18h]
        cmp    eax, 1100h
        jl     NEAR PTR loc_1_1B3E
        cmp    eax, 1101h
        jle    SHORT loc_1_1A7E
        cmp    eax, 1102h
        je     NEAR PTR loc_1_1B09
        cmp    eax, 1112h
        je     NEAR PTR loc_1_1B28
        jmp    NEAR PTR loc_1_1B3E
loc_1_1A7E:
        mov    cx, word ptr [edi + 1ah]
        lea    eax, [esp + 10h]
        push   eax
        push   ecx
        push   dword ptr [esp + 2ch]
        call   sub_1_2706
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_1AE7
        lea    eax, [edi + 2ch]
        mov    ecx, dword ptr [esp + 10h]
        push   eax
        push   dword ptr [ecx + 8]
        push   esi
        call   sub_1_2601
        mov    ecx, dword ptr [esp + 1ch]
        add    esp, 0ch
        push   dword ptr [ecx + 8]
        push   dword ptr [ecx + 4]
        call   sub_1_257D
        mov    edx, dword ptr [esp + 18h]
        add    esp, 8
        mov    ecx, dword ptr [edx + 8]
        movzx  edx, word ptr [ecx + 4dah]
        mov    al, byte ptr [ecx + 4e4h]
        push   eax
        push   edx
        mov    edx, dword ptr [esp + 18h]
        push   dword ptr [edx + 4]
        call   sub_1_2482
        add    esp, 0ch
        jmp    SHORT loc_1_1AF8
loc_1_1AE7:
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1580h
        jne    SHORT loc_1_1AF8
        xor    ebx, ebx
loc_1_1AF8:
        push   dword ptr [esp + 10h]
        push   dword ptr [esi + 10h]
        call   sub_1_23E0
        add    esp, 8
        jmp    SHORT loc_1_1B3E
loc_1_1B09:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax + 8]
        mov    byte ptr [ecx + 4b1h], 0ffh
        push   dword ptr [esp + 10h]
        push   dword ptr [esi + 10h]
        call   sub_1_23E0
        add    esp, 8
        jmp    SHORT loc_1_1B3E
loc_1_1B28:
        mov    al, byte ptr [edi + 2ch]
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, dword ptr [ecx + 8]
        mov    byte ptr [edx + 4ach], al
        mov    dword ptr [edi], 0
loc_1_1B3E:
        test   ebx, ebx
        jne    SHORT loc_1_1BBE
        lea    eax, [esp + 14h]
        push   eax
        push   ebp
        push   edi
        call   sub_1_2364
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_1BC2
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 8]
        lea    ecx, [eax + 250h]
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_1B74
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 18h]
        push   ecx
        jmp    SHORT loc_1_1BA4
loc_1_1B74:
        push   dword ptr [esp + 14h]
        push   eax
        push   dword ptr [esi + 10h]
        call   sub_1_2001
        mov    edx, dword ptr [esp + 1ch]
        add    esp, 0ch
        test   eax, eax
        mov    eax, dword ptr [edx + 8]
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 18h]
        jne    SHORT loc_1_1B9E
        add    eax, 128h
        jmp    SHORT loc_1_1BA3
loc_1_1B9E:
        add    eax, 250h
loc_1_1BA3:
        push   eax
loc_1_1BA4:
        call   sub_1_1F88
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_1BC2
        push   dword ptr [esp + 10h]
        call   sub_1_232C
        add    esp, 4
loc_1_1BBE:
        test   ebx, ebx
        je     SHORT loc_1_1BE9
loc_1_1BC2:
        test   ebp, ebp
        je     SHORT loc_1_1BE9
        push   14h
        push   ebp
        call   sub_1_041C
        add    esp, 8
        jmp    SHORT loc_1_1BE9
loc_1_1BD3:
        push   1
        push   15814h
        push   1581h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
loc_1_1BE9:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_1BF3:
        push   ebx
        xor    eax, eax
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        push   ebp
        cmp    byte ptr [esi], al
        je     SHORT loc_1_1C39
        mov    edi, dword ptr [esp + 18h]
        mov    ebp, dword ptr [edi]
        test   ebp, ebp
        je     SHORT loc_1_1C30
        mov    bx, word ptr [esp + 1ch]
loc_1_1C10:
        push   ebx
        push   ebp
        push   esi
        call   sub_1_28ED
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_1C29
        push   ebp
        push   edi
        call   sub_1_283E
        add    esp, 8
loc_1_1C29:
        mov    ebp, dword ptr [ebp + 0ch]
        test   ebp, ebp
        jne    SHORT loc_1_1C10
loc_1_1C30:
        test   eax, eax
        jne    SHORT loc_1_1C4D
        mov    byte ptr [esi], 0
        jmp    SHORT loc_1_1C4D
loc_1_1C39:
        push   1
        push   15813h
        push   1581h
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_1C4D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1C52:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 8ch]
        test   eax, eax
        jne    SHORT loc_1_1CAE
        cmp    byte ptr [esi], bl
        je     SHORT loc_1_1C78
        push   1
        push   dword ptr [esp + 18h]
        push   esi
        call   sub_1_1BF3
        add    esp, 0ch
loc_1_1C78:
        mov    eax, dword ptr [esp + 1ch]
        lea    edi, [esi + 15h]
        mov    dword ptr [esi + 10h], eax
        xor    eax, eax
        mov    word ptr [esi + 2], ax
        mov    dword ptr [esi + 0ch], eax
        stosd
        stosw
        stosb
        lea    eax, [esi + 1ch]
        push   eax
        push   1
        call   sub_1_29FE
        add    esp, 8
        push   dword ptr [esp + 14h]
        call   sub_1_29CB
        add    esp, 4
        mov    byte ptr [esi], 1
        jmp    SHORT loc_1_1CB0
loc_1_1CAE:
        mov    ebx, eax
loc_1_1CB0:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1CB6:
        movzx  ecx, byte ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 8]
        and    eax, 0fffh
        shl    eax, 4
        or     eax, ecx
        movzx  ecx, word ptr [esp + 4]
        shl    ecx, 10h
        or     eax, ecx
        ret
loc_1_1CD4:
        xor    eax, eax
        ret
sub_1_1CD7:
        db  0A1h, 014h, 0D0h, 040h, 000h
        movzx  eax, byte ptr [eax + 22h]
        cmp    eax, 1
        je     SHORT loc_1_1CF1
        cmp    eax, 2
        je     SHORT loc_1_1CF8
        cmp    eax, 3
        je     SHORT loc_1_1CFF
        jmp    SHORT loc_1_1D04
loc_1_1CF1:
        call   sub_1_2B30
        jmp    SHORT loc_1_1D04
loc_1_1CF8:
        call   sub_1_2ADC
        jmp    SHORT loc_1_1D04
loc_1_1CFF:
        jmp    NEAR PTR sub_1_2A36
loc_1_1D04:
        ret
sub_1_1D05:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        call   sub_1_05A8
        mov    byte ptr [ebp - 1], al
        test   al, al
        jne    SHORT loc_1_1D24
        call   sub_1_2BE8
loc_1_1D24:
        movzx  eax, byte ptr [esi + 13h]
        test   eax, eax
        je     SHORT loc_1_1D4E
        cmp    eax, 1
        je     SHORT loc_1_1D5F
        mov    bl, byte ptr [ebp + 0ch]
loc_1_1D34:
        movzx  eax, word ptr [ebp + 8]
        cmp    eax, 18h
        je     SHORT loc_1_1D70
        cmp    eax, 19h
        je     SHORT loc_1_1D75
        cmp    eax, 1ah
        je     SHORT loc_1_1D7A
        cmp    eax, 1eh
        je     SHORT loc_1_1D7F
        jmp    SHORT loc_1_1D82
loc_1_1D4E:
        mov    bl, byte ptr [ebp + 0ch]
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_0544
        add    esp, 8
        jmp    SHORT loc_1_1D34
loc_1_1D5F:
        mov    bl, byte ptr [ebp + 0ch]
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_0490
        add    esp, 8
        jmp    SHORT loc_1_1D34
loc_1_1D70:
        mov    byte ptr [esi + 3], bl
        jmp    SHORT loc_1_1D82
loc_1_1D75:
        mov    byte ptr [esi + 4], bl
        jmp    SHORT loc_1_1D82
loc_1_1D7A:
        mov    byte ptr [esi + 5], bl
        jmp    SHORT loc_1_1D82
loc_1_1D7F:
        mov    byte ptr [esi + 8], bl
loc_1_1D82:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_1D8D
        call   sub_1_2C1D
loc_1_1D8D:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_1D93:
        mov    edx, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    edi, dword ptr [esp + 14h]
        mov    ecx, edi
        shr    edi, 2
        and    cx, 3
        mov    eax, edi
        dec    edi
        test   eax, eax
        je     SHORT loc_1_1DC2
loc_1_1DB1:
        mov    eax, dword ptr [edx]
        add    edx, 4
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, edi
        dec    edi
        test   eax, eax
        jne    SHORT loc_1_1DB1
loc_1_1DC2:
        mov    ax, cx
        dec    cx
        test   ax, ax
        je     SHORT loc_1_1DDC
loc_1_1DCC:
        mov    al, byte ptr [edx]
        inc    edx
        mov    byte ptr [esi], al
        inc    esi
        mov    ax, cx
        dec    cx
        test   ax, ax
        jne    SHORT loc_1_1DCC
loc_1_1DDC:
        pop    edi
        pop    esi
        ret
sub_1_1DDF:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 014h, 0D0h, 040h, 000h
        push   2ch
        push   0
        push   ebx
        lea    edi, [ebx + 9]
        call   sub_1_33CF
        mov    esi, dword ptr [esp + 1ch]
        add    esp, 0ch
        mov    eax, 5050505h
        mov    dword ptr [esi], ebx
        stosd
        stosd
        stosb
        mov    byte ptr [ebx + 12h], 0
        mov    byte ptr [ebx + 13h], 0
        mov    eax, 2
        mov    byte ptr [ebx + 15h], al
        mov    byte ptr [ebx + 14h], al
        mov    cx, word ptr [esi + 16h]
        mov    word ptr [ebx + 18h], cx
        cmp    byte ptr [esi + 3ch], 1
        jne    SHORT loc_1_1E35
        push   esi
        call   sub_1_32DA
        add    esp, 4
        call   sub_1_3288
loc_1_1E35:
        cmp    byte ptr [esi + 1fh], 1
        jne    SHORT loc_1_1E5E
        cmp    byte ptr [esi + 3ch], 1
        jne    SHORT loc_1_1E5E
        mov    di, word ptr [esi + 16h]
        add    di, 3
        push   edi
        call   sub_1_3281
        add    esp, 4
        and    al, 0c2h
        push   eax
        push   edi
        call   sub_1_3276
        add    esp, 8
loc_1_1E5E:
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_1_2FD2
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_1F14
        cmp    byte ptr [esi + 3ch], 0
        jne    SHORT loc_1_1E9B
        push   1
        push   esi
        call   sub_1_2F18
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_1F14
        push   esi
        call   sub_1_2CE7
        add    esp, 4
        mov    edi, eax
loc_1_1E9B:
        test   edi, edi
        jne    SHORT loc_1_1F14
        cmp    byte ptr [esi + 3ch], 1
        jne    SHORT loc_1_1F14
        push   1
        push   esi
        call   sub_1_2F18
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_1F14
        push   esi
        call   sub_1_2F02
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_1F14
        push   esi
        call   sub_1_2CE7
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_1F14
        push   1ch
        call   sub_1_2C87
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_1EF0
        push   3
        push   15910h
        push   1597h
        jmp    SHORT loc_1_1F0A
loc_1_1EF0:
        push   1bh
        call   sub_1_2C87
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_1F14
        push   4
        push   15910h
        push   1596h
loc_1_1F0A:
        call   sub_1_1CB6
        add    esp, 0ch
        mov    edi, eax
loc_1_1F14:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1F1A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        push   ebp
        push   ds
        push   es
        push   fs
        push   gs
        push   ss
        call   sub_1_3405
        mov    eax, dword ptr [ebp + 0ch]
        db  089h, 025h, 08Ch, 0CFh, 040h, 000h
        mov    esp, dword ptr [ebp + 8]
        push   eax
        call   sub_1_33EF
        pop    eax
        pop    ss
        pop    gs
        pop    fs
        pop    es
        pop    ds
        pop    ebp
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_1F57:
        push   esi
        call   sub_1_3405
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
        test   ecx, ecx
        je     SHORT loc_1_1F81
        lea    edx, [ecx + 8]
        mov    esi, dword ptr [edx]
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [edx], 0
loc_1_1F81:
        call   sub_1_33EF
        pop    esi
        ret
sub_1_1F88:
        push   ebx
        push   esi
        xor    ebx, ebx
        call   sub_1_3405
        xor    esi, esi
        xor    cl, cl
        mov    edx, dword ptr [esp + 0ch]
loc_1_1F99:
        test   esi, esi
        jne    SHORT loc_1_1FB9
        movzx  eax, cl
        imul   eax, eax, 0ch
        lea    eax, [eax + edx + 8]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_1FAE
        mov    esi, eax
loc_1_1FAE:
        inc    cl
        cmp    cl, 18h
        jb     SHORT loc_1_1F99
        test   esi, esi
        je     SHORT loc_1_1FE1
loc_1_1FB9:
        cmp    dword ptr [edx], 0
        jne    SHORT loc_1_1FC2
        mov    dword ptr [edx], esi
        jmp    SHORT loc_1_1FC8
loc_1_1FC2:
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax + 8], esi
loc_1_1FC8:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [edx + 4], esi
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi + 8], 0
        jmp    SHORT loc_1_1FF7
loc_1_1FE1:
        push   1
        push   1582ah
        push   1501h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
loc_1_1FF7:
        call   sub_1_33EF
        mov    eax, ebx
        pop    esi
        pop    ebx
        ret
sub_1_2001:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 24h]
        push   ebp
        mov    cx, word ptr [edi + 18h]
        mov    esi, dword ptr [esp + 24h]
        cmp    cx, 110fh
        mov    dword ptr [esp + 14h], ebx
        jne    SHORT loc_1_206C
        cmp    byte ptr [esi + 4e6h], 22h
        jne    SHORT loc_1_206C
        lea    eax, [edi + 10h]
        lea    ecx, [edi + 14h]
        push   eax
        push   ecx
        push   8
        push   ebx
        push   1
        push   dword ptr [esp + 34h]
        call   sub_1_03C0
        add    esp, 18h
        mov    dword ptr [esi + 4b4h], eax
        cmp    eax, ebx
        jne    NEAR PTR loc_1_2193
        push   1
        push   15821h
        push   1503h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    dword ptr [edi], eax
        jmp    NEAR PTR loc_1_2193
loc_1_206C:
        lea    eax, [esp + 12h]
        lea    edx, [esp + 18h]
        push   eax
        push   edx
        push   ecx
        call   sub_1_3532
        mov    bp, ax
        add    esp, 0ch
        cmp    byte ptr [esi + 4ach], 0ffh
        jne    NEAR PTR loc_1_2193
        test   byte ptr [esp + 18h], 1
        je     NEAR PTR loc_1_2193
        call   sub_1_3405
        xor    al, al
        mov    ecx, dword ptr [esi + 500h]
        cmp    word ptr [ecx], 0
        jbe    SHORT loc_1_20F6
loc_1_20AD:
        test   bl, bl
        jne    SHORT loc_1_20F6
        movzx  edx, al
        shl    edx, 4
        mov    ecx, dword ptr [esi + 500h]
        lea    ecx, [ecx + edx + 4]
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_1_20E5
        mov    byte ptr [ecx], 1
        mov    ecx, dword ptr [esi + 500h]
        mov    ebx, dword ptr [ecx + edx + 8]
        mov    dword ptr [edi + 14h], ebx
        mov    bl, 1
        mov    ecx, dword ptr [esi + 500h]
        mov    edx, dword ptr [ecx + edx + 0ch]
        mov    dword ptr [edi + 10h], edx
loc_1_20E5:
        inc    al
        mov    edx, dword ptr [esi + 500h]
        movzx  cx, al
        cmp    cx, word ptr [edx]
        jb     SHORT loc_1_20AD
loc_1_20F6:
        call   sub_1_33EF
        test   bl, bl
        je     SHORT loc_1_217B
        test   byte ptr [esp + 18h], 2
        je     NEAR PTR loc_1_2193
        push   dword ptr [edi + 0ch]
        push   dword ptr [esp + 24h]
        call   sub_1_05D4
        add    esp, 8
        cmp    eax, -1
        je     SHORT loc_1_214C
        cmp    byte ptr [esi + 4e6h], 22h
        jne    SHORT loc_1_213A
        push   0
        push   2
        push   ebp
        push   dword ptr [edi + 14h]
        push   eax
        call   sub_1_3434
        add    esp, 14h
        jmp    SHORT loc_1_2160
loc_1_213A:
        movzx  ecx, bp
        push   ecx
        push   dword ptr [edi + 14h]
        push   eax
        call   sub_1_1D93
        add    esp, 0ch
        jmp    SHORT loc_1_2164
loc_1_214C:
        push   1
        push   15821h
        push   1585h
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_2160:
        mov    dword ptr [esp + 14h], eax
loc_1_2164:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_2193
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 14h], 0
        mov    dword ptr [edi], eax
        jmp    SHORT loc_1_2193
loc_1_217B:
        push   1
        push   15821h
        push   1582h
        call   sub_1_1CB6
        mov    dword ptr [esp + 20h], eax
        add    esp, 0ch
loc_1_2193:
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_219F:
        sub    esp, 8
        lea    eax, [esp + 2]
        push   esi
        lea    ecx, [esp + 8]
        push   eax
        mov    esi, dword ptr [esp + 14h]
        push   ecx
        mov    ax, word ptr [esi + 18h]
        push   eax
        call   sub_1_3532
        movzx  eax, word ptr [esp + 12h]
        add    esp, 0ch
        push   eax
        push   esi
        mov    esi, dword ptr [esp + 1ch]
        push   esi
        call   sub_1_1D93
        add    esp, 0ch
        push   esi
        call   sub_1_05E0
        add    esp, 4
        pop    esi
        add    esp, 8
        ret
sub_1_21E0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 20h]
        push   ebp
        mov    cx, word ptr [edi + 18h]
        mov    esi, dword ptr [esp + 20h]
        cmp    cx, 110fh
        mov    byte ptr [esp + 11h], bl
        jne    SHORT loc_1_2222
        cmp    byte ptr [esi + 4e6h], 22h
        jne    SHORT loc_1_2222
        push   dword ptr [esi + 4b4h]
        call   sub_1_0304
        add    esp, 4
        mov    dword ptr [esi + 4b4h], ebx
        jmp    NEAR PTR loc_1_2306
loc_1_2222:
        lea    eax, [esp + 12h]
        lea    edx, [esp + 14h]
        push   eax
        push   edx
        push   ecx
        call   sub_1_3532
        mov    bp, ax
        add    esp, 0ch
        test   byte ptr [esp + 14h], 1
        je     NEAR PTR loc_1_2306
        cmp    byte ptr [esi + 4ach], 0ffh
        jne    SHORT loc_1_22B8
        test   byte ptr [esp + 14h], 4
        je     SHORT loc_1_22B8
        push   dword ptr [edi + 0ch]
        push   dword ptr [esp + 20h]
        call   sub_1_05D4
        add    esp, 8
        cmp    eax, -1
        je     SHORT loc_1_2295
        cmp    byte ptr [esi + 4e6h], 22h
        jne    SHORT loc_1_2283
        push   0
        push   3
        push   ebp
        push   dword ptr [edi + 14h]
        push   eax
        call   sub_1_3434
        add    esp, 14h
        jmp    SHORT loc_1_22A9
loc_1_2283:
        movzx  ecx, bp
        push   ecx
        push   eax
        push   dword ptr [edi + 14h]
        call   sub_1_1D93
        add    esp, 0ch
        jmp    SHORT loc_1_22AB
loc_1_2295:
        push   1
        push   15822h
        push   1585h
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_22A9:
        mov    ebx, eax
loc_1_22AB:
        test   ebx, ebx
        je     SHORT loc_1_22B8
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_22B8
        mov    dword ptr [edi], ebx
        xor    ebx, ebx
loc_1_22B8:
        call   sub_1_3405
        xor    al, al
        mov    edx, dword ptr [esi + 500h]
        cmp    word ptr [edx], 0
        jbe    SHORT loc_1_2301
loc_1_22CB:
        cmp    byte ptr [esp + 11h], 0
        jne    SHORT loc_1_2301
        movzx  edx, al
        shl    edx, 4
        mov    ebp, dword ptr [edi + 10h]
        add    edx, dword ptr [esi + 500h]
        cmp    dword ptr [edx + 0ch], ebp
        jne    SHORT loc_1_22EF
        mov    byte ptr [edx + 4], 0
        mov    byte ptr [esp + 11h], 1
loc_1_22EF:
        inc    al
        mov    ebp, dword ptr [esi + 500h]
        movzx  dx, al
        cmp    dx, word ptr [ebp]
        jb     SHORT loc_1_22CB
loc_1_2301:
        call   sub_1_33EF
loc_1_2306:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_2310:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    ecx, dword ptr [esp + 0ch]
        push   edi
        lea    esi, [eax + 34h]
        lea    edi, [ecx + 20h]
        movsd
        movsd
        movsd
        mov    edx, dword ptr [eax + 7ch]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 2ch], edx
        ret
sub_1_232C:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    byte ptr [eax + 4a0h], 0
        jne    SHORT loc_1_234A
        push   dword ptr [eax + 4a4h]
        push   ecx
        call   sub_1_362D
        jmp    SHORT loc_1_2360
loc_1_234A:
        cmp    dword ptr [eax + 4a8h], 0
        jne    SHORT loc_1_2363
        push   1
        push   dword ptr [eax + 4a4h]
        call   sub_1_1F1A
loc_1_2360:
        add    esp, 8
loc_1_2363:
        ret
sub_1_2364:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        xor    esi, esi
        push   eax
        mov    ebx, dword ptr [ebp + 8]
        mov    ax, word ptr [ebx + 18h]
        push   ecx
        push   eax
        call   sub_1_3532
        movzx  eax, word ptr [ebp - 2]
        add    esp, 0ch
        add    eax, 8
        push   eax
        call   sub_1_05FC
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_23C1
        movzx  eax, word ptr [ebp - 2]
        push   eax
        push   edi
        push   ebx
        call   sub_1_1D93
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 10h]
        movzx  ecx, word ptr [ebp - 2]
        mov    dword ptr [eax], edi
        add    edi, ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], eax
        jmp    SHORT loc_1_23D7
loc_1_23C1:
        push   1
        push   1582dh
        push   1503h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, eax
loc_1_23D7:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_23E0:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 8]
        cmp    byte ptr [eax + 4a1h], 0
        jne    NEAR PTR loc_1_2478
        mov    cl, byte ptr [eax + 4e8h]
        cmp    cl, 6
        je     SHORT loc_1_2445
        cmp    byte ptr [eax + 4e9h], 2
        je     SHORT loc_1_2445
        cmp    byte ptr [eax + 4eah], 2
        je     SHORT loc_1_2445
        movzx  eax, cl
        push   esi
        db  068h, 080h, 046h, 040h, 000h
        push   eax
        push   dword ptr [esp + 14h]
        call   sub_1_0618
        add    esp, 10h
        call   sub_1_2BE8
        mov    eax, dword ptr [esi + 8]
        add    eax, 4c4h
        push   eax
        call   sub_1_3758
        add    esp, 4
        call   sub_1_2C1D
        jmp    SHORT loc_1_246E
loc_1_2445:
        push   dword ptr [esp + 8]
        call   sub_1_0698
        add    esp, 4
        mov    ecx, dword ptr [esi + 8]
        movzx  eax, byte ptr [ecx + 4e8h]
        push   esi
        db  068h, 080h, 046h, 040h, 000h
        push   eax
        push   dword ptr [esp + 14h]
        call   sub_1_0618
        add    esp, 10h
loc_1_246E:
        mov    eax, dword ptr [esi + 8]
        mov    byte ptr [eax + 4a1h], 1
loc_1_2478:
        pop    esi
        ret
sub_1_247A:
        mov    eax, dword ptr [esp + 4]
        shr    eax, 10h
        ret
sub_1_2482:
        sub    esp, 4
        push   ebx
        push   esi
        mov    bl, byte ptr [esp + 18h]
        push   edi
        mov    esi, dword ptr [esp + 14h]
        push   ebp
        push   dword ptr [esi + 110h]
        mov    byte ptr [esi + 0f4h], bl
        call   sub_1_384C
        add    esp, 4
        test   al, al
        je     SHORT loc_1_24FA
        lea    edi, [esi + 0e4h]
        lea    ebp, [esi + 0ech]
        mov    dword ptr [esi + 0d8h], 7
        lea    edx, [esi + 0f0h]
        mov    dword ptr [edi], 4
        lea    eax, [esi + 0e8h]
        mov    dword ptr [ebp], 2
        mov    dword ptr [esi + 0dch], 5
        mov    dword ptr [edx], 2
        mov    dword ptr [eax], 3
        mov    dword ptr [esi + 0e0h], 4
        jmp    SHORT loc_1_2540
loc_1_24FA:
        mov    ecx, dword ptr [esp + 1ch]
        lea    edi, [esi + 0e4h]
        lea    edx, [ecx + 7]
        lea    ebp, [ecx + 5]
        lea    eax, [ecx + 4]
        mov    dword ptr [esi + 0d8h], edx
        lea    edx, [esi + 0f0h]
        mov    dword ptr [esi + 0dch], ebp
        add    ecx, 3
        mov    dword ptr [edi], eax
        lea    ebp, [esi + 0ech]
        mov    dword ptr [esi + 0e0h], eax
        lea    eax, [ecx - 1]
        mov    dword ptr [ebp], eax
        mov    dword ptr [edx], eax
        lea    eax, [esi + 0e8h]
        mov    dword ptr [eax], ecx
loc_1_2540:
        test   bl, bl
        je     SHORT loc_1_2562
        xor    byte ptr [edi], 80h
        xor    byte ptr [edx], 80h
        xor    byte ptr [esi + 0d8h], 80h
        xor    byte ptr [esi + 0dch], 80h
        xor    byte ptr [esi + 0e0h], 80h
        xor    byte ptr [eax], 80h
loc_1_2562:
        push   dword ptr [ebp]
        push   dword ptr [edx]
        push   dword ptr [esi + 110h]
        call   sub_1_380E
        add    esp, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_257D:
        push   ebx
        push   esi
        push   124h
        xor    ebx, ebx
        mov    esi, dword ptr [esp + 10h]
        push   ebx
        push   esi
        call   sub_1_33CF
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        mov    dword ptr [esi + 110h], eax
        mov    byte ptr [esi + 11bh], bl
        mov    eax, 1
        mov    byte ptr [esi + 11fh], bl
        mov    byte ptr [esi + 121h], al
        mov    byte ptr [esi + 11ch], bl
        mov    byte ptr [esi + 11dh], bl
        mov    byte ptr [esi + 3bh], al
        mov    word ptr [esi + 106h], bx
        mov    byte ptr [esi], al
        mov    byte ptr [esi + 41h], bl
        mov    byte ptr [esi + 1ch], al
        mov    eax, 2
        mov    byte ptr [esi + 10eh], bl
        mov    byte ptr [esi + 10fh], bl
        mov    byte ptr [esi + 10dh], bl
        mov    byte ptr [esi + 3eh], al
        mov    byte ptr [esi + 3dh], bl
        mov    byte ptr [esi + 44h], al
        mov    byte ptr [esi + 45h], bl
        mov    byte ptr [esi + 19h], 0ah
        mov    byte ptr [esi + 46h], 0efh
        pop    esi
        pop    ebx
        ret
sub_1_2601:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 10h]
        push   edi
        push   ebx
        call   sub_1_385F
        add    esp, 4
        lea    eax, [ebx + 128h]
        push   eax
        call   sub_1_385F
        add    esp, 4
        lea    eax, [ebx + 250h]
        push   eax
        call   sub_1_385F
        add    esp, 4
        xor    ecx, ecx
        lea    eax, [ebx + 0d74h]
        mov    byte ptr [ebx + 4a0h], cl
        mov    byte ptr [ebx + 4a1h], cl
        mov    dword ptr [ebx + 4a4h], eax
        mov    dword ptr [ebx + 4a8h], ecx
        mov    byte ptr [ebx + 4adh], cl
        mov    byte ptr [ebx + 4aeh], cl
        mov    byte ptr [ebx + 4afh], cl
        mov    byte ptr [ebx + 4b0h], cl
        mov    dword ptr [ebx + 4b4h], ecx
        lea    edi, [ebx + 4c4h]
        mov    dword ptr [ebx + 4b8h], ecx
        mov    esi, dword ptr [esp + 18h]
        mov    dword ptr [ebx + 4bch], ecx
        mov    byte ptr [ebx + 4ach], 0ffh
        mov    dword ptr [ebx + 4c0h], ecx
        mov    ecx, 0fh
        rep movsd
        cmp    byte ptr [ebx + 4e6h], 22h
        jne    SHORT loc_1_26D9
        lea    eax, [ebx + 504h]
        mov    esi, dword ptr [esp + 10h]
        add    esi, 60h
        mov    ecx, 0bh
        mov    dword ptr [ebx + 500h], eax
        lea    eax, [ebx + 548h]
        mov    edi, eax
        rep movsd
        db  0A3h, 014h, 0D0h, 040h, 000h
        push   dword ptr [ebx + 500h]
        push   2
        call   sub_1_29FE
        add    esp, 8
        jmp    SHORT loc_1_26F0
loc_1_26D9:
        mov    esi, dword ptr [esp + 10h]
        add    esi, 1ch
        mov    dword ptr [ebx + 500h], esi
        db  0C7h, 005h, 014h, 0D0h, 040h, 000h, 000h, 000h, 000h, 000h
loc_1_26F0:
        lea    edi, [ebx + 574h]
        mov    eax, 0cccccccch
        mov    ecx, 200h
        rep stosd
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_2706:
        sub    esp, 4
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 18h]
        push   edi
        mov    edi, dword ptr [esp + 14h]
        push   ebp
        push   ebx
        push   dword ptr [esp + 20h]
        push   edi
        call   sub_1_17D6
        add    esp, 0ch
        push   eax
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1513h
        jne    NEAR PTR loc_1_281A
        mov    dword ptr [esp + 10h], 0
        push   0d74h
        call   sub_1_05FC
        add    esp, 4
        mov    ebp, eax
        call   sub_1_3887
        movzx  eax, ax
        push   eax
        call   sub_1_05FC
        add    esp, 4
        mov    esi, eax
        call   sub_1_3405
        xor    al, al
        xor    edx, edx
        mov    dword ptr [ebx], 0
loc_1_276F:
        cmp    dword ptr [ebx], edx
        jne    SHORT loc_1_278B
        movzx  ecx, al
        shl    ecx, 4
        add    ecx, edi
        cmp    dword ptr [ecx + 0ch], edx
        jne    SHORT loc_1_2785
        add    ecx, 8
        mov    dword ptr [ebx], ecx
loc_1_2785:
        inc    al
        cmp    al, 10h
        jb     SHORT loc_1_276F
loc_1_278B:
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     SHORT loc_1_27DA
        test   ebp, ebp
        je     SHORT loc_1_27CC
        test   esi, esi
        je     SHORT loc_1_27CC
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_27A2
        mov    dword ptr [edi], eax
        jmp    SHORT loc_1_27A8
loc_1_27A2:
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ecx + 0ch], eax
loc_1_27A8:
        mov    dx, word ptr [esp + 1ch]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [edi + 4], eax
        mov    ecx, dword ptr [ebx]
        mov    word ptr [ecx], dx
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 4], esi
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 8], ebp
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 0ch], 0
        jmp    SHORT loc_1_27F2
loc_1_27CC:
        push   1
        push   15825h
        push   1503h
        jmp    SHORT loc_1_27E6
loc_1_27DA:
        push   1
        push   15825h
        push   1505h
loc_1_27E6:
        call   sub_1_1CB6
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
loc_1_27F2:
        call   sub_1_33EF
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_2832
        test   ebp, ebp
        je     SHORT loc_1_280B
        push   ebp
        call   sub_1_05E0
        add    esp, 4
loc_1_280B:
        test   esi, esi
        je     SHORT loc_1_2832
        push   esi
        call   sub_1_05E0
        add    esp, 4
        jmp    SHORT loc_1_2832
loc_1_281A:
        push   1
        push   15825h
        push   1580h
        call   sub_1_1CB6
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
loc_1_2832:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_283E:
        sub    esp, 4
        xor    eax, eax
        mov    dword ptr [esp], eax
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [esp + 10h]
        push   edi
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esi]
        test   ecx, ecx
        je     SHORT loc_1_286F
loc_1_285A:
        test   al, al
        jne    SHORT loc_1_2873
        cmp    ecx, edi
        jne    SHORT loc_1_2866
        mov    al, 1
        jmp    SHORT loc_1_286B
loc_1_2866:
        mov    ebx, ecx
        mov    ecx, dword ptr [ecx + 0ch]
loc_1_286B:
        test   ecx, ecx
        jne    SHORT loc_1_285A
loc_1_286F:
        test   al, al
        je     SHORT loc_1_28CA
loc_1_2873:
        push   dword ptr [edi + 4]
        call   sub_1_05E0
        add    esp, 4
        push   dword ptr [edi + 8]
        call   sub_1_05E0
        add    esp, 4
        call   sub_1_3405
        mov    eax, dword ptr [esi]
        cmp    edi, eax
        jne    SHORT loc_1_289B
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi], eax
        jmp    SHORT loc_1_28A9
loc_1_289B:
        cmp    dword ptr [esi + 4], edi
        jne    SHORT loc_1_28A3
        mov    dword ptr [esi + 4], ebx
loc_1_28A3:
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 0ch], eax
loc_1_28A9:
        mov    dword ptr [edi + 4], 0
        mov    dword ptr [edi + 8], 0
        mov    dword ptr [edi + 0ch], 0
        mov    word ptr [edi], 0ffffh
        call   sub_1_33EF
        jmp    SHORT loc_1_28E2
loc_1_28CA:
        push   1
        push   15826h
        push   1513h
        call   sub_1_1CB6
        mov    dword ptr [esp + 18h], eax
        add    esp, 0ch
loc_1_28E2:
        mov    eax, dword ptr [esp + 0ch]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_28ED:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [esp + 14h]
        mov    edx, dword ptr [esi + 8]
        lea    ecx, [edx + 4a0h]
        cmp    byte ptr [ecx], bl
        je     NEAR PTR loc_1_29C5
        cmp    word ptr [esp + 18h], 1
        jne    SHORT loc_1_2971
        call   sub_1_3405
        mov    eax, dword ptr [esi + 8]
        cmp    dword ptr [eax + 128h], ebx
        je     SHORT loc_1_296A
        mov    edi, dword ptr [esp + 10h]
        mov    dword ptr [edi + 4], ebx
        mov    eax, dword ptr [esi + 8]
        mov    byte ptr [eax + 4ach], bl
        mov    eax, dword ptr [esi + 8]
        mov    byte ptr [eax + 4a0h], bl
        call   sub_1_33EF
        mov    eax, dword ptr [esi + 8]
        cmp    dword ptr [eax + 4a8h], ebx
        je     SHORT loc_1_2958
        add    eax, 4b8h
        push   eax
        call   sub_1_3900
        add    esp, 4
        jmp    SHORT loc_1_29B5
loc_1_2958:
        push   1
        push   dword ptr [eax + 4a4h]
        call   sub_1_1F1A
        add    esp, 8
        jmp    SHORT loc_1_29B5
loc_1_296A:
        call   sub_1_33EF
        jmp    SHORT loc_1_29B1
loc_1_2971:
        cmp    dword ptr [edx + 250h], 0
        jne    SHORT loc_1_299B
        cmp    dword ptr [edx + 128h], 0
        jne    SHORT loc_1_299B
        push   1
        mov    byte ptr [ecx], 0
        mov    eax, dword ptr [esi + 8]
        push   dword ptr [eax + 4a4h]
        call   sub_1_1F1A
        add    esp, 8
        jmp    SHORT loc_1_29B1
loc_1_299B:
        push   1
        push   15834h
        push   1502h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
loc_1_29B1:
        mov    edi, dword ptr [esp + 10h]
loc_1_29B5:
        test   ebx, ebx
        jne    SHORT loc_1_29C5
        push   esi
        push   dword ptr [edi + 10h]
        call   sub_1_388C
        add    esp, 8
loc_1_29C5:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_29CB:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        push   edi
        xor    edx, edx
        mov    esi, 0ffffh
        xor    edi, edi
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], edx
loc_1_29DF:
        movzx  eax, dl
        shl    eax, 4
        inc    dl
        add    eax, ecx
        cmp    dl, 10h
        mov    word ptr [eax + 8], si
        mov    dword ptr [eax + 0ch], edi
        mov    dword ptr [eax + 10h], edi
        mov    dword ptr [eax + 14h], edi
        jb     SHORT loc_1_29DF
        pop    edi
        pop    esi
        ret
sub_1_29FE:
        push   ebx
        xor    ecx, ecx
        mov    ebx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        xor    edx, edx
        mov    edi, dword ptr [esp + 10h]
loc_1_2A0D:
        movzx  esi, cl
        mov    eax, esi
        shl    eax, 4
        add    eax, ebx
        cmp    edi, 2
        mov    byte ptr [eax + 4], dl
        jne    SHORT loc_1_2A2B
        shl    esi, 0fh
        inc    word ptr [ebx]
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 0ch], esi
loc_1_2A2B:
        inc    cl
        cmp    cl, 4
        jb     SHORT loc_1_2A0D
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_2A36:
        push   esi
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        mov    al, byte ptr [esi + 29h]
        push   eax
        push   102h
        call   sub_1_3276
        add    esp, 8
        call   sub_1_3405
        push   0fc23h
        call   sub_1_3281
        add    esp, 4
        push   0f023h
        call   sub_1_3281
        add    esp, 4
        push   0c023h
        call   sub_1_3281
        add    esp, 4
        push   23h
        call   sub_1_3281
        add    esp, 4
        push   22h
        call   sub_1_392C
        add    esp, 4
        test   al, 1
        jne    SHORT loc_1_2AD5
        push   23h
        call   sub_1_3281
        add    esp, 4
        test   al, 80h
        jne    SHORT loc_1_2AD5
        push   61h
        push   24h
        call   sub_1_3276
        add    esp, 8
        mov    al, byte ptr [esi + 2ah]
        push   eax
        push   25h
        call   sub_1_3276
        add    esp, 8
        push   0fah
        push   24h
        call   sub_1_3276
        add    esp, 8
        push   1
        push   25h
        call   sub_1_3276
        add    esp, 8
loc_1_2AD5:
        call   sub_1_33EF
        pop    esi
        ret
sub_1_2ADC:
        push   esi
        push   94h
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        call   sub_1_3281
        add    esp, 4
        and    al, 7fh
        push   eax
        push   94h
        call   sub_1_3276
        add    esp, 8
        mov    cl, byte ptr [esi + 24h]
        push   ecx
        push   102h
        call   sub_1_3276
        add    esp, 8
        push   94h
        call   sub_1_3281
        add    esp, 4
        or     al, 80h
        push   eax
        push   94h
        call   sub_1_3276
        add    esp, 8
        pop    esi
        ret
sub_1_2B30:
        db  0A1h, 014h, 0D0h, 040h, 000h
        mov    al, byte ptr [eax + 23h]
        push   eax
        push   65h
        call   sub_1_3276
        add    esp, 8
        ret
sub_1_2B44:
        sub    esp, 4
        mov    word ptr [esp + 2], 0
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_05A8
        mov    byte ptr [esp + 11h], al
        test   al, al
        jne    SHORT loc_1_2B64
        call   sub_1_2BE8
loc_1_2B64:
        mov    esi, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 1ch]
loc_1_2B6C:
        xor    bp, bp
        push   20h
        push   dword ptr [esp + 24h]
        push   esi
        cmp    byte ptr [esp + 30h], 2
        jne    SHORT loc_1_2B84
        call   sub_1_39F7
        jmp    SHORT loc_1_2B89
loc_1_2B84:
        call   sub_1_398D
loc_1_2B89:
        add    esp, 0ch
        mov    ebx, eax
        call   sub_1_2C1D
        call   sub_1_2BE8
        cmp    byte ptr [esp + 28h], 0
        jne    SHORT loc_1_2BA4
        call   sub_1_0708
loc_1_2BA4:
        test   ebx, ebx
        je     SHORT loc_1_2BAC
        mov    bp, 1
loc_1_2BAC:
        test   bp, bp
        je     SHORT loc_1_2BD2
        inc    word ptr [esp + 12h]
        cmp    word ptr [esp + 12h], 3
        jae    SHORT loc_1_2BC9
        push   edi
        call   sub_1_3934
        add    esp, 4
        mov    ebx, eax
loc_1_2BC9:
        test   bp, bp
        je     SHORT loc_1_2BD2
        test   ebx, ebx
        je     SHORT loc_1_2B6C
loc_1_2BD2:
        cmp    byte ptr [esp + 11h], 0
        jne    SHORT loc_1_2BDE
        call   sub_1_2C1D
loc_1_2BDE:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_2BE8:
        push   ebp
        db  0A1h, 0EDh, 01Fh, 040h, 000h
        mov    ebp, esp
        sub    esp, 4
        test   eax, eax
        je     SHORT loc_1_2C03
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 008h, 000h, 003h, 000h
loc_1_2C03:
        xor    eax, eax
        mov    esp, ebp
        db  0A3h, 024h, 0D0h, 040h, 000h
        db  0A3h, 028h, 0D0h, 040h, 000h
        db  0C7h, 005h, 020h, 0D0h, 040h, 000h, 001h, 000h, 000h, 000h
        pop    ebp
        ret
sub_1_2C1D:
        push   ebp
        db  0A1h, 0EDh, 01Fh, 040h, 000h
        db  0C7h, 005h, 020h, 0D0h, 040h, 000h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        test   eax, eax
        je     SHORT loc_1_2C42
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_1_2C42:
        xor    ecx, ecx
        db  039h, 00Dh, 028h, 0D0h, 040h, 000h
        je     SHORT loc_1_2C65
        db  089h, 00Dh, 028h, 0D0h, 040h, 000h
        db  089h, 00Dh, 024h, 0D0h, 040h, 000h
        db  08Bh, 015h, 02Ch, 0D0h, 040h, 000h
        call   sub_1_3AC2
        jmp    SHORT loc_1_2C83
loc_1_2C65:
        db  083h, 03Dh, 024h, 0D0h, 040h, 000h, 000h
        je     SHORT loc_1_2C83
        db  0C7h, 005h, 024h, 0D0h, 040h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 02Ch, 0D0h, 040h, 000h
        call   sub_1_3A57
loc_1_2C83:
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2C87:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        push   ebp
        call   sub_1_3405
        mov    di, word ptr [esp + 14h]
        xor    ebp, ebp
        cmp    di, 1bh
        jne    SHORT loc_1_2CA8
        mov    bl, byte ptr [esi + 6]
        jmp    SHORT loc_1_2CAB
loc_1_2CA8:
        mov    bl, byte ptr [esi + 7]
loc_1_2CAB:
        push   edi
        call   sub_1_3B66
        add    esp, 4
        cmp    al, bl
        je     SHORT loc_1_2CDB
        cmp    di, 1bh
        push   1
        push   158f0h
        jne    SHORT loc_1_2CCC
        push   1596h
        jmp    SHORT loc_1_2CD1
loc_1_2CCC:
        push   1597h
loc_1_2CD1:
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebp, eax
loc_1_2CDB:
        call   sub_1_33EF
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_2CE7:
        sub    esp, 0ch
        db  0A1h, 014h, 0D0h, 040h, 000h
        mov    dword ptr [esp + 8], eax
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    byte ptr [esp + 0eh], bl
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        push   ebp
        push   ebx
        push   ebx
        push   8
        push   ebx
        push   esi
        call   sub_1_4433
        mov    byte ptr [esp + 2bh], al
        add    esp, 14h
        mov    al, byte ptr [esi + 0dh]
        mov    byte ptr [esp + 10h], al
        cmp    al, 0ffh
        jne    SHORT loc_1_2D33
        push   ebx
        mov    al, byte ptr [esi + 0eh]
        push   ebx
        push   8
        push   eax
        push   esi
        call   sub_1_4433
        mov    byte ptr [esp + 24h], al
        add    esp, 14h
loc_1_2D33:
        push   8000h
        call   sub_1_05FC
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_2D61
        push   1
        push   15914h
        push   159dh
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
        jmp    NEAR PTR loc_1_2EF8
loc_1_2D61:
        mov    byte ptr [esp + 14h], 0
        mov    ebp, edi
loc_1_2D68:
        push   dword ptr [esp + 14h]
        push   2000h
        push   ebp
        push   0
        add    ebp, 2000h
        call   sub_1_423F
        inc    byte ptr [esp + 24h]
        add    esp, 10h
        cmp    byte ptr [esp + 14h], 4
        jb     SHORT loc_1_2D68
loc_1_2D8D:
        lea    eax, [esp + 15h]
        lea    ecx, [esp + 13h]
        lea    edx, [esp + 11h]
        push   eax
        lea    eax, [esp + 16h]
        push   ecx
        push   edx
        push   eax
        push   dword ptr [esp + 20h]
        push   esi
        call   sub_1_3BE7
        add    esp, 18h
        test   ebx, ebx
        jne    NEAR PTR loc_1_2E6E
        mov    eax, dword ptr [esp + 18h]
        mov    al, byte ptr [eax + 3]
        mov    cl, al
        and    cl, 3
        cmp    cl, byte ptr [esp + 12h]
        jne    SHORT loc_1_2DD3
        and    al, 4
        shr    al, 2
        cmp    al, byte ptr [esp + 11h]
        je     SHORT loc_1_2DF6
loc_1_2DD3:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 1bh]
        push   esi
        call   sub_1_40A6
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_2E6E
        mov    al, byte ptr [esp + 10h]
        mov    byte ptr [esp + 17h], al
loc_1_2DF6:
        test   ebx, ebx
        jne    SHORT loc_1_2E6E
        push   edi
        push   dword ptr [esp + 17h]
        push   esi
        call   sub_1_3C35
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_2E18
        mov    byte ptr [esp + 16h], 1
        jmp    NEAR PTR loc_1_2ED2
loc_1_2E18:
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1595h
        jne    NEAR PTR loc_1_2ED2
        xor    ebx, ebx
        cmp    byte ptr [esp + 10h], bl
        je     SHORT loc_1_2E6A
        lea    eax, [esp + 15h]
        lea    ecx, [esp + 13h]
        lea    edx, [esp + 11h]
        push   eax
        lea    eax, [esp + 16h]
        push   ecx
        dec    byte ptr [esp + 18h]
        push   edx
        push   eax
        push   dword ptr [esp + 20h]
        push   esi
        call   sub_1_3BE7
        add    esp, 18h
        cmp    byte ptr [esi + 0dh], 0ffh
        je     SHORT loc_1_2ED2
        cmp    byte ptr [esp + 11h], bl
        jne    SHORT loc_1_2ED2
        dec    byte ptr [esp + 10h]
        jmp    SHORT loc_1_2ED2
loc_1_2E6A:
        push   1
        jmp    SHORT loc_1_2EBE
loc_1_2E6E:
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1592h
        jne    SHORT loc_1_2ED2
        xor    ebx, ebx
        cmp    byte ptr [esp + 10h], bl
        je     SHORT loc_1_2EBC
        lea    eax, [esp + 15h]
        lea    ecx, [esp + 13h]
        lea    edx, [esp + 11h]
        push   eax
        lea    eax, [esp + 16h]
        push   ecx
        dec    byte ptr [esp + 18h]
        push   edx
        push   eax
        push   dword ptr [esp + 20h]
        push   esi
        call   sub_1_3BE7
        add    esp, 18h
        cmp    byte ptr [esi + 0dh], 0ffh
        je     SHORT loc_1_2ED2
        cmp    byte ptr [esp + 11h], bl
        jne    SHORT loc_1_2ED2
        dec    byte ptr [esp + 10h]
        jmp    SHORT loc_1_2ED2
loc_1_2EBC:
        push   2
loc_1_2EBE:
        push   15914h
        push   1593h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
loc_1_2ED2:
        test   ebx, ebx
        jne    SHORT loc_1_2EE1
        cmp    byte ptr [esp + 16h], 0
        je     NEAR PTR loc_1_2D8D
loc_1_2EE1:
        cmp    byte ptr [esp + 16h], 0
        je     SHORT loc_1_2EEF
        mov    al, byte ptr [esp + 10h]
        mov    byte ptr [esi + 0dh], al
loc_1_2EEF:
        push   edi
        call   sub_1_05E0
        add    esp, 4
loc_1_2EF8:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_2F02:
        call   sub_1_44A7
        test   eax, eax
        jne    SHORT loc_1_2F17
        push   dword ptr [esp + 4]
        call   sub_1_4485
        add    esp, 4
loc_1_2F17:
        ret
sub_1_2F18:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        db  08Bh, 03Dh, 014h, 0D0h, 040h, 000h
        xor    ebp, ebp
        mov    dword ptr [esp + 10h], ebp
loc_1_2F2F:
        mov    byte ptr [edi + 12h], 0
        mov    byte ptr [edi + 13h], 0
        mov    bl, byte ptr [edi + 8]
        mov    al, byte ptr [edi + 2]
        and    bl, 1
        and    al, 0fbh
        push   eax
        mov    byte ptr [edi + 2], al
        movzx  eax, word ptr [esi + 16h]
        add    eax, 2
        push   eax
        call   sub_1_3276
        add    esp, 8
        push   0ah
        call   sub_1_0720
        add    esp, 4
        mov    al, byte ptr [edi + 2]
        or     al, 4
        push   eax
        mov    byte ptr [edi + 2], al
        movzx  eax, word ptr [esi + 16h]
        add    eax, 2
        push   eax
        call   sub_1_3276
        add    esp, 8
        push   1388h
        call   sub_1_0720
        add    esp, 4
        cmp    byte ptr [esp + 1ch], 1
        jne    SHORT loc_1_2FB5
        push   esi
        call   sub_1_4679
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_1_2FB9
        test   bl, bl
        je     SHORT loc_1_2FB5
        call   sub_1_4656
        mov    al, byte ptr [edi + 8]
        or     al, 2
        push   eax
        push   1eh
        call   sub_1_1D05
        add    esp, 8
loc_1_2FB5:
        test   ebp, ebp
        je     SHORT loc_1_2FC8
loc_1_2FB9:
        inc    dword ptr [esp + 10h]
        cmp    dword ptr [esp + 10h], 2
        jb     NEAR PTR loc_1_2F2F
loc_1_2FC8:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_2FD2:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_4A71
        add    esp, 4
        push   esi
        call   sub_1_49C4
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_3053
        cmp    byte ptr [esi + 3ch], 1
        jne    SHORT loc_1_3053
        push   esi
        call   sub_1_48FA
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_3053
        push   dword ptr [esp + 0ch]
        call   sub_1_02C4
        add    esp, 4
        mov    dl, byte ptr [esi + 24h]
        mov    cl, dl
        shr    ax, cl
        test   al, 1
        je     SHORT loc_1_3023
        push   esi
        call   sub_1_47BA
        add    esp, 4
        jmp    SHORT loc_1_3053
loc_1_3023:
        movzx  ax, dl
        push   eax
        call   sub_1_3055
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_303F
        push   esi
        call   sub_1_47BA
        add    esp, 4
        jmp    SHORT loc_1_3053
loc_1_303F:
        push   1
        push   15911h
        push   159bh
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_3053:
        pop    esi
        ret
sub_1_3055:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        lea    eax, [ebp - 0ch]
        xor    esi, esi
        push   esi
        push   esi
        push   eax
        db  0E8h, 097h, 0CFh, 0FFh, 0FFh
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_309D
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], esi
        push   ecx
        mov    dword ptr [ebp - 8], esi
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 0ch]
        call   sub_1_3168
        add    esp, 10h
        cmp    dword ptr [ebp - 8], esi
        je     SHORT loc_1_309D
        cmp    dword ptr [ebp - 4], esi
        jne    SHORT loc_1_309D
        mov    eax, 1
        jmp    SHORT loc_1_309F
loc_1_309D:
        xor    eax, eax
loc_1_309F:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_30A4:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        mov    ecx, dword ptr [esp + 0ch]
loc_1_30AD:
        mov    bl, byte ptr [eax]
        test   bl, bl
        je     SHORT loc_1_30DD
        mov    dl, byte ptr [ecx]
        test   dl, dl
        je     SHORT loc_1_30DD
        inc    eax
        inc    ecx
        cmp    bl, 61h
        jl     SHORT loc_1_30C8
        cmp    bl, 7ah
        jg     SHORT loc_1_30C8
        sub    bl, 20h
loc_1_30C8:
        cmp    dl, 61h
        jl     SHORT loc_1_30D5
        cmp    dl, 7ah
        jg     SHORT loc_1_30D5
        sub    dl, 20h
loc_1_30D5:
        cmp    dl, bl
        je     SHORT loc_1_30AD
        xor    eax, eax
        jmp    SHORT loc_1_30E7
loc_1_30DD:
        mov    cl, byte ptr [ecx]
        sub    cl, byte ptr [eax]
        cmp    cl, 1
        sbb    eax, eax
        inc    eax
loc_1_30E7:
        pop    ebx
        ret
sub_1_30E9:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   esi
        lea    eax, [ebp - 4]
        push   0
        lea    ecx, [ebp - 2ch]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 20h
        push   ecx
        push   1
        db  068h, 030h, 0D0h, 040h, 000h
        push   0
        push   esi
        db  0E8h, 0F5h, 0CEh, 0FFh, 0FFh
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_1_3161
        db  068h, 038h, 0D0h, 040h, 000h
        lea    eax, [ebp - 2ch]
        push   eax
        call   sub_1_30A4
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_3161
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 0ch]
        mov    dword ptr [ebp - 4], 1
        push   ecx
        push   3
        db  068h, 040h, 0D0h, 040h, 000h
        push   0
        push   esi
        db  0E8h, 0B9h, 0CEh, 0FFh, 0FFh
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_1_3161
        cmp    byte ptr [ebp - 0ch], 0
        mov    eax, 1
        je     SHORT loc_1_3163
loc_1_3161:
        xor    eax, eax
loc_1_3163:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3168:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 2ch]
        test   ebp, ebp
        je     NEAR PTR loc_1_326E
        mov    si, word ptr [esp + 30h]
        mov    edi, dword ptr [esp + 34h]
        mov    ebx, dword ptr [esp + 38h]
loc_1_3188:
        cmp    dword ptr [edi], 0
        jne    NEAR PTR loc_1_326E
        lea    eax, [esp + 18h]
        push   0
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 1ch], 0
        push   ebp
        push   eax
        db  0E8h, 082h, 0CEh, 0FFh, 0FFh
        add    esp, 0ch
        push   ebx
        push   edi
        push   esi
        push   dword ptr [esp + 24h]
        call   sub_1_3168
        add    esp, 10h
        cmp    dword ptr [edi], 0
        jne    NEAR PTR loc_1_3252
        lea    eax, [esp + 1ch]
        push   2
        push   ebp
        push   eax
        db  0E8h, 052h, 0CEh, 0FFh, 0FFh
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_3252
        mov    eax, dword ptr [esp + 1ch]
        push   0
        lea    ecx, [esp + 14h]
        push   0
        mov    dword ptr [esp + 18h], eax
        push   4
        push   dword ptr [esp + 1ch]
        push   ecx
        db  0E8h, 01Fh, 0CEh, 0FFh, 0FFh
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_1_3252
loc_1_3200:
        lea    eax, [esp + 20h]
        push   0
        push   8
        push   eax
        push   dword ptr [esp + 1ch]
        db  0E8h, 00Eh, 0CEh, 0FFh, 0FFh
        add    esp, 10h
        cmp    word ptr [esp + 22h], si
        jne    SHORT loc_1_3237
        push   ebp
        call   sub_1_30E9
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_3231
        mov    dword ptr [ebx], 1
        jmp    SHORT loc_1_3237
loc_1_3231:
        mov    dword ptr [edi], 1
loc_1_3237:
        lea    ecx, [esp + 10h]
        push   0
        push   0
        push   4
        push   dword ptr [esp + 1ch]
        push   ecx
        db  0E8h, 0CDh, 0CDh, 0FFh, 0FFh
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_1_3200
loc_1_3252:
        lea    eax, [esp + 14h]
        push   0
        push   ebp
        push   eax
        db  0E8h, 0B1h, 0CDh, 0FFh, 0FFh
        mov    ebp, dword ptr [esp + 20h]
        add    esp, 0ch
        test   ebp, ebp
        jne    NEAR PTR loc_1_3188
loc_1_326E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
sub_1_3276:
        mov    dx, word ptr [esp + 4]
        mov    al, byte ptr [esp + 8]
        out    dx, al
        ret
sub_1_3281:
        mov    dx, word ptr [esp + 4]
        in     al, dx
        ret
sub_1_3288:
        db  08Bh, 00Dh, 014h, 0D0h, 040h, 000h
        movzx  eax, byte ptr [ecx + 22h]
        test   eax, eax
        je     SHORT loc_1_32AC
        cmp    eax, 1
        je     SHORT loc_1_32B3
        cmp    eax, 2
        jl     SHORT loc_1_32D9
        cmp    eax, 3
        jle    SHORT loc_1_32AC
        cmp    eax, 4
        je     SHORT loc_1_32C0
        jmp    SHORT loc_1_32D9
loc_1_32AC:
        mov    al, byte ptr [ecx + 2]
        and    al, 0dfh
        jmp    SHORT loc_1_32C5
loc_1_32B3:
        mov    al, byte ptr [ecx + 23h]
        or     al, 80h
        push   eax
        mov    byte ptr [ecx + 23h], al
        push   65h
        jmp    SHORT loc_1_32D1
loc_1_32C0:
        mov    al, byte ptr [ecx + 2]
        and    al, 7fh
loc_1_32C5:
        push   eax
        mov    byte ptr [ecx + 2], al
        movzx  eax, word ptr [ecx + 18h]
        add    eax, 2
        push   eax
loc_1_32D1:
        call   sub_1_3276
        add    esp, 8
loc_1_32D9:
        ret
sub_1_32DA:
        sub    esp, 4
        mov    byte ptr [esp + 3], 0
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        db  08Bh, 03Dh, 014h, 0D0h, 040h, 000h
        mov    byte ptr [esi + 0eh], 0ffh
        mov    byte ptr [edi + 22h], 0
        push   esi
        call   sub_1_4B6B
        add    esp, 4
        test   al, al
        jne    SHORT loc_1_3317
        push   esi
        call   sub_1_4B68
        add    esp, 4
        test   al, al
        jne    SHORT loc_1_3317
        call   sub_1_4B4E
loc_1_3317:
        movzx  eax, word ptr [esi + 16h]
        add    eax, 2
        xor    ebx, ebx
        xor    bp, bp
        push   eax
        call   sub_1_3281
        add    esp, 4
        mov    byte ptr [edi + 2], al
        call   sub_1_3288
loc_1_3334:
        mov    ax, word ptr [esi + 16h]
        push   ebx
        push   eax
        call   sub_1_3276
        mov    ax, word ptr [esi + 16h]
        add    esp, 8
        push   eax
        call   sub_1_3281
        add    esp, 4
        cmp    al, bl
        jne    SHORT loc_1_3355
        inc    bp
loc_1_3355:
        inc    bx
        cmp    bx, 0ffh
        jbe    SHORT loc_1_3334
        cmp    bp, 100h
        jne    SHORT loc_1_33C2
        mov    byte ptr [esi + 0eh], 0
loc_1_3369:
        xor    bp, bp
        call   sub_1_4AFC
        xor    ebx, ebx
loc_1_3373:
        mov    ax, word ptr [esi + 16h]
        push   ebx
        push   eax
        call   sub_1_3276
        mov    ax, word ptr [esi + 16h]
        add    esp, 8
        push   eax
        call   sub_1_3281
        add    esp, 4
        cmp    al, bl
        jne    SHORT loc_1_3394
        inc    bp
loc_1_3394:
        inc    bx
        cmp    bx, 0ffh
        jbe    SHORT loc_1_3373
        cmp    bp, 100h
        jae    SHORT loc_1_33AA
        mov    byte ptr [esi + 0eh], 2
        jmp    SHORT loc_1_33B6
loc_1_33AA:
        cmp    byte ptr [edi + 22h], 0
        jne    SHORT loc_1_33B6
        mov    byte ptr [edi + 22h], 4
        jmp    SHORT loc_1_33BB
loc_1_33B6:
        mov    byte ptr [esp + 13h], 1
loc_1_33BB:
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_1_3369
loc_1_33C2:
        call   sub_1_1CD7
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_33CF:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        je     SHORT loc_1_33ED
        mov    bl, byte ptr [esp + 0ch]
loc_1_33E3:
        mov    eax, ecx
        mov    byte ptr [edx], bl
        inc    edx
        dec    ecx
        test   eax, eax
        jne    SHORT loc_1_33E3
loc_1_33ED:
        pop    ebx
        ret
sub_1_33EF:
        push   eax
        db  080h, 03Dh, 094h, 0CFh, 040h, 000h, 000h
        jne    SHORT loc_1_3403
        db  066h, 0FFh, 00Dh, 082h, 0CFh, 040h, 000h
        jne    SHORT loc_1_3403
        sti
loc_1_3403:
        pop    eax
        ret
sub_1_3405:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   eax
        pushfd
        pop    eax
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        and    al, 1
        db  0A2h, 094h, 0CFh, 040h, 000h
        jne    SHORT loc_1_342F
        db  066h, 083h, 03Dh, 082h, 0CFh, 040h, 000h, 000h
        jne    SHORT loc_1_3428
        cli
loc_1_3428:
        db  066h, 0FFh, 005h, 082h, 0CFh, 040h, 000h
loc_1_342F:
        pop    eax
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3434:
        sub    esp, 8
        db  0A1h, 014h, 0D0h, 040h, 000h
        mov    dword ptr [esp + 4], eax
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        push   ebp
        push   esi
        call   sub_1_3934
        mov    bx, word ptr [esp + 28h]
        add    esp, 4
        mov    edi, eax
        mov    ebp, dword ptr [esp + 1ch]
        movzx  eax, bx
        add    eax, esi
        cmp    bx, 80h
        mov    dword ptr [esp + 10h], eax
        jb     SHORT loc_1_34A5
loc_1_346C:
        test   edi, edi
        jne    SHORT loc_1_34A5
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   80h
        push   esi
        push   ebp
        call   sub_1_2B44
        add    esp, 14h
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_349E
        sub    bx, 80h
        add    ebp, 80h
        add    esi, 80h
loc_1_349E:
        cmp    bx, 80h
        jae    SHORT loc_1_346C
loc_1_34A5:
        test   bx, bx
        je     SHORT loc_1_34C3
        test   edi, edi
        jne    SHORT loc_1_34C3
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   ebx
        push   esi
        push   ebp
        call   sub_1_2B44
        add    esp, 14h
        mov    edi, eax
loc_1_34C3:
        mov    eax, dword ptr [esp + 14h]
        test   byte ptr [eax + 3], 10h
        je     SHORT loc_1_34D2
        add    dword ptr [esp + 10h], 4
loc_1_34D2:
        mov    eax, dword ptr [esp + 14h]
        test   byte ptr [eax + 8], 2
        je     SHORT loc_1_34E6
        mov    eax, dword ptr [esp + 10h]
        add    eax, eax
        mov    dword ptr [esp + 10h], eax
loc_1_34E6:
        push   10h
        call   sub_1_3B66
        movzx  eax, al
        add    esp, 4
        test   edi, edi
        jne    SHORT loc_1_3513
        cmp    byte ptr [esp + 10h], al
        je     SHORT loc_1_3513
        push   1
        push   158f9h
        push   1596h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    edi, eax
loc_1_3513:
        mov    eax, dword ptr [esp + 14h]
        test   byte ptr [eax + 3], 10h
        je     SHORT loc_1_3528
        call   sub_1_4CBF
        test   edi, edi
        jne    SHORT loc_1_3528
        mov    edi, eax
loc_1_3528:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_3532:
        movzx  eax, word ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        sub    eax, 1100h
        cmp    eax, 14h
        mov    dword ptr [ecx], 0
        ja     SHORT loc_1_3559
        db  00Fh, 0B6h, 090h, 018h, 046h, 040h, 000h
        db  0FFh, 024h, 095h, 0E4h, 045h, 040h, 000h
loc_1_3559:
        xor    ax, ax
        jmp    NEAR PTR loc_1_35E3
loc_1_3561:
        xor    ax, ax
        mov    ecx, dword ptr [esp + 0ch]
        mov    word ptr [ecx], 9ch
        jmp    SHORT loc_1_35E3
loc_1_356F:
        xor    ax, ax
        mov    ecx, dword ptr [esp + 0ch]
        mov    word ptr [ecx], 4ch
        jmp    SHORT loc_1_35E3
loc_1_357D:
        xor    ax, ax
        mov    ecx, dword ptr [esp + 0ch]
        mov    word ptr [ecx], 44h
        jmp    SHORT loc_1_35E3
loc_1_358B:
        mov    ax, 8000h
        mov    dword ptr [ecx], 5
        mov    ecx, dword ptr [esp + 0ch]
        mov    word ptr [ecx], 48h
        jmp    SHORT loc_1_35E3
loc_1_35A0:
        mov    ax, 8000h
        mov    dword ptr [ecx], 3
        mov    ecx, dword ptr [esp + 0ch]
        mov    word ptr [ecx], 48h
        jmp    SHORT loc_1_35E3
loc_1_35B5:
        xor    ax, ax
        mov    dword ptr [ecx], 1
        mov    ecx, dword ptr [esp + 0ch]
        mov    word ptr [ecx], 44h
        jmp    SHORT loc_1_35E3
loc_1_35C9:
        xor    ax, ax
        mov    ecx, dword ptr [esp + 0ch]
        mov    word ptr [ecx], 40h
        jmp    SHORT loc_1_35E3
loc_1_35D7:
        xor    ax, ax
        mov    ecx, dword ptr [esp + 0ch]
        mov    word ptr [ecx], 30h
loc_1_35E3:
        ret
        db  061h, 045h, 040h, 000h, 0D7h, 045h, 040h, 000h, 06Fh, 045h, 040h, 000h, 0D7h, 045h, 040h, 000h
        db  07Dh, 045h, 040h, 000h, 08Bh, 045h, 040h, 000h, 0A0h, 045h, 040h, 000h, 0B5h, 045h, 040h, 000h
        db  0D7h, 045h, 040h, 000h, 0C9h, 045h, 040h, 000h, 0D7h, 045h, 040h, 000h, 061h, 045h, 040h, 000h
        db  059h, 045h, 040h, 000h, 000h, 000h, 001h, 001h, 002h, 003h, 003h, 003h, 004h, 005h, 005h, 005h
        db  005h, 006h, 006h, 007h, 008h, 009h, 00Ah, 00Ch, 00Bh
sub_1_362D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        push   ebp
        push   ds
        push   es
        push   fs
        push   gs
        push   ss
        call   sub_1_3405
        db  089h, 025h, 08Ch, 0CFh, 040h, 000h
        mov    esp, dword ptr [ebp + 0ch]
        call   sub_1_33EF
        push   dword ptr [ebp + 8]
        call   sub_1_4D4A
        add    esp, 4
        call   sub_1_3405
        db  08Bh, 025h, 08Ch, 0CFh, 040h, 000h
        call   sub_1_33EF
        pop    ss
        pop    gs
        pop    fs
        pop    es
        pop    ds
        pop    ebp
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_3680:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        cld
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 8]
        call   sub_1_5368
        add    esp, 4
        test   al, al
        je     NEAR PTR loc_1_374D
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [eax + 8]
        cmp    dword ptr [esi + 4a8h], 0
        je     NEAR PTR loc_1_374D
        mov    al, byte ptr [esi + 4b0h]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   dword ptr [ecx + 4]
        call   sub_1_50E0
        add    esp, 8
        mov    dword ptr [esi + 4c0h], eax
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 8]
        call   sub_1_505D
        add    esp, 4
        call   sub_1_0790
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    byte ptr [eax + 4b0h], 0
        je     SHORT loc_1_3752
        mov    ebx, 1
        mov    byte ptr [eax + 4afh], bl
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        mov    byte ptr [ecx + 4b0h], 0
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 4]
        call   sub_1_5052
        add    esp, 4
        test   al, al
        je     SHORT loc_1_372C
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 4]
        call   sub_1_4DA6
        add    esp, 4
        mov    bl, al
loc_1_372C:
        test   bl, bl
        je     SHORT loc_1_3752
        push   dword ptr [ebp - 4]
        db  068h, 0C2h, 04Ah, 040h, 000h
        push   400000h
        db  0FFh, 035h, 090h, 0CFh, 040h, 000h
        call   sub_1_079C
        add    esp, 10h
        jmp    SHORT loc_1_3752
loc_1_374D:
        call   sub_1_0790
loc_1_3752:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3758:
        push   esi
        push   edi
        db  08Bh, 03Dh, 014h, 0D0h, 040h, 000h
        mov    esi, dword ptr [esp + 0ch]
        cmp    byte ptr [esi + 1dh], 0
        je     SHORT loc_1_378D
        cmp    byte ptr [esi + 22h], 22h
        je     SHORT loc_1_3793
        mov    ax, word ptr [esi]
        push   8
        push   eax
        call   sub_1_3276
        mov    ax, word ptr [esi + 2]
        add    esp, 8
        push   8
        push   eax
        call   sub_1_3276
        add    esp, 8
loc_1_378D:
        cmp    byte ptr [esi + 22h], 22h
        jne    SHORT loc_1_37D6
loc_1_3793:
        mov    ax, word ptr [esi + 16h]
        add    ax, 2
        push   eax
        call   sub_1_3281
        add    esp, 4
        and    al, 10h
        shr    al, 4
        mov    byte ptr [edi + 20h], al
        mov    al, byte ptr [edi + 4]
        or     al, 0fh
        push   eax
        push   19h
        call   sub_1_1D05
        add    esp, 8
        mov    al, byte ptr [edi + 2]
        or     al, 10h
        push   eax
        mov    byte ptr [edi + 2], al
        mov    ax, word ptr [esi + 16h]
        add    ax, 2
        push   eax
        call   sub_1_3276
        add    esp, 8
loc_1_37D6:
        cmp    byte ptr [esi + 1dh], 0
        je     SHORT loc_1_380B
        cmp    byte ptr [esi + 1fh], 0
        je     SHORT loc_1_37F7
        mov    ax, word ptr [esi + 16h]
        push   4
        add    ax, 12h
        push   eax
        call   sub_1_54A6
        add    esp, 8
        jmp    SHORT loc_1_380B
loc_1_37F7:
        mov    ax, word ptr [esi + 16h]
        push   0ffh
        push   1
        push   eax
        call   sub_1_53D1
        add    esp, 0ch
loc_1_380B:
        pop    edi
        pop    esi
        ret
sub_1_380E:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_384C
        add    esp, 4
        test   al, al
        mov    eax, dword ptr [esp + 0ch]
        je     SHORT loc_1_3838
        mov    word ptr [esi + 4cch], ax
        mov    ecx, dword ptr [esp + 10h]
        mov    word ptr [esi + 4ceh], cx
        jmp    SHORT loc_1_384A
loc_1_3838:
        mov    word ptr [esi + 4c4h], ax
        mov    ecx, dword ptr [esp + 10h]
        mov    word ptr [esi + 4c6h], cx
loc_1_384A:
        pop    esi
        ret
sub_1_384C:
        mov    eax, dword ptr [esp + 4]
        mov    al, byte ptr [eax + 4e6h]
        sub    al, 22h
        cmp    al, 1
        sbb    al, al
        neg    al
        ret
sub_1_385F:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        xor    edx, edx
        xor    esi, esi
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], edx
loc_1_386D:
        movzx  eax, dl
        imul   eax, eax, 0ch
        add    eax, ecx
        inc    dl
        cmp    dl, 18h
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 0ch], esi
        mov    dword ptr [eax + 10h], esi
        jb     SHORT loc_1_386D
        pop    esi
        ret
sub_1_3887:
        mov    ax, 124h
        ret
sub_1_388C:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 8]
        cmp    byte ptr [eax + 4a1h], 0
        je     SHORT loc_1_38FE
        cmp    byte ptr [eax + 4e8h], 6
        je     SHORT loc_1_38BF
        cmp    byte ptr [eax + 4e9h], 2
        je     SHORT loc_1_38BF
        cmp    byte ptr [eax + 4eah], 2
        je     SHORT loc_1_38BF
        call   sub_1_2BE8
        jmp    SHORT loc_1_38CB
loc_1_38BF:
        push   dword ptr [esp + 8]
        call   sub_1_0834
        add    esp, 4
loc_1_38CB:
        mov    eax, dword ptr [esi + 8]
        add    eax, 4c4h
        push   eax
        call   sub_1_54C5
        add    esp, 4
        mov    ecx, dword ptr [esi + 8]
        movzx  edx, byte ptr [ecx + 4e8h]
        db  068h, 0C2h, 04Ah, 040h, 000h
        push   edx
        call   sub_1_07FC
        add    esp, 8
        mov    ecx, dword ptr [esi + 8]
        mov    byte ptr [ecx + 4a1h], 0
loc_1_38FE:
        pop    esi
        ret
sub_1_3900:
        push   esi
        push   0
        call   sub_1_08B8
        mov    esi, dword ptr [esp + 0ch]
        add    esp, 4
        push   eax
        mov    dword ptr [esi], eax
        call   sub_1_08A0
        add    esp, 4
        push   dword ptr [esi]
        call   sub_1_088C
        add    esp, 4
        mov    dword ptr [esi], 0
        pop    esi
        ret
sub_1_392C:
        mov    dx, word ptr [esp + 4]
        in     ax, dx
        ret
sub_1_3934:
        push   esi
        xor    eax, eax
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        test   byte ptr [esi + 3], 10h
        je     SHORT loc_1_3948
        call   sub_1_4CBF
loc_1_3948:
        test   byte ptr [esi + 8], 2
        mov    esi, dword ptr [esp + 8]
        je     SHORT loc_1_3954
        add    esi, esi
loc_1_3954:
        test   eax, eax
        jne    SHORT loc_1_398B
        push   esi
        push   10h
        call   sub_1_1D05
        add    esp, 8
        mov    eax, esi
        shr    eax, 8
        shr    esi, 10h
        push   eax
        push   11h
        call   sub_1_1D05
        add    esp, 8
        push   esi
        push   12h
        call   sub_1_1D05
        add    esp, 8
        push   1ch
        call   sub_1_2C87
        add    esp, 4
loc_1_398B:
        pop    esi
        ret
sub_1_398D:
        push   esi
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        call   sub_1_55CB
        test   byte ptr [esi + 2], 8
        jne    SHORT loc_1_39A5
        test   byte ptr [esi + 3], 10h
        jne    SHORT loc_1_39B3
loc_1_39A5:
        push   dword ptr [esp + 10h]
        push   3
        call   sub_1_558B
        add    esp, 8
loc_1_39B3:
        mov    ax, word ptr [esp + 0ch]
        test   ax, ax
        je     SHORT loc_1_39E6
        movzx  ecx, byte ptr [esi + 15h]
        cmp    ecx, 1
        je     SHORT loc_1_39CD
        cmp    ecx, 2
        je     SHORT loc_1_39D9
        jmp    SHORT loc_1_39E6
loc_1_39CD:
        push   eax
        push   dword ptr [esp + 0ch]
        call   sub_1_0978
        jmp    SHORT loc_1_39E3
loc_1_39D9:
        push   eax
        push   dword ptr [esp + 0ch]
        call   sub_1_08D0
loc_1_39E3:
        add    esp, 8
loc_1_39E6:
        call   sub_1_553D
        push   1bh
        call   sub_1_2C87
        add    esp, 4
        pop    esi
        ret
sub_1_39F7:
        push   esi
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        test   byte ptr [esi + 2], 8
        jne    SHORT loc_1_3A0A
        test   byte ptr [esi + 3], 10h
        je     SHORT loc_1_3A18
loc_1_3A0A:
        push   dword ptr [esp + 10h]
        push   2
        call   sub_1_558B
        add    esp, 8
loc_1_3A18:
        mov    ax, word ptr [esp + 0ch]
        test   ax, ax
        je     SHORT loc_1_3A4B
        movzx  ecx, byte ptr [esi + 14h]
        cmp    ecx, 1
        je     SHORT loc_1_3A32
        cmp    ecx, 2
        je     SHORT loc_1_3A3E
        jmp    SHORT loc_1_3A4B
loc_1_3A32:
        push   eax
        push   dword ptr [esp + 0ch]
        call   sub_1_0A98
        jmp    SHORT loc_1_3A48
loc_1_3A3E:
        push   eax
        push   dword ptr [esp + 0ch]
        call   sub_1_0A24
loc_1_3A48:
        add    esp, 8
loc_1_3A4B:
        push   1bh
        call   sub_1_2C87
        add    esp, 4
        pop    esi
        ret
sub_1_3A57:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cld
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4a8h
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_3ABE
        db  083h, 03Dh, 020h, 0D0h, 040h, 000h, 000h
        je     SHORT loc_1_3A8B
        db  0C7h, 005h, 024h, 0D0h, 040h, 000h, 001h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 02Ch, 0D0h, 040h, 000h
        jmp    SHORT loc_1_3ABE
loc_1_3A8B:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4aeh
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_3ABE
        push   1
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 4a4h]
        call   sub_1_1F1A
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 4aeh], 0
loc_1_3ABE:
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3AC2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cld
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 4a8h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        mov    eax, dword ptr [ecx + 4bch]
        test   eax, eax
        je     SHORT loc_1_3B0F
        push   eax
        call   sub_1_0B14
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 4bch], 0
        db  0C7h, 005h, 024h, 0D0h, 040h, 000h, 000h, 000h, 000h, 000h
loc_1_3B0F:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        add    eax, 4aeh
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_3B62
        db  083h, 03Dh, 020h, 0D0h, 040h, 000h, 000h
        je     SHORT loc_1_3B3C
        db  0C7h, 005h, 028h, 0D0h, 040h, 000h, 001h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 02Ch, 0D0h, 040h, 000h
        jmp    SHORT loc_1_3B62
loc_1_3B3C:
        push   0
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        push   dword ptr [ecx + 4a4h]
        call   sub_1_1F1A
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        mov    byte ptr [edx + 4aeh], 0
loc_1_3B62:
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3B66:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        xor    bl, bl
        call   sub_1_55CB
        mov    di, word ptr [esp + 14h]
        mov    byte ptr [esp + 0fh], al
        cmp    di, 19h
        jne    SHORT loc_1_3B8D
        mov    bl, byte ptr [esi + 4]
        jmp    SHORT loc_1_3BD9
loc_1_3B8D:
        movzx  eax, byte ptr [esi + 12h]
        test   eax, eax
        je     SHORT loc_1_3B9C
        cmp    eax, 1
        je     SHORT loc_1_3BA4
        jmp    SHORT loc_1_3BAF
loc_1_3B9C:
        push   edi
        call   sub_1_0BF8
        jmp    SHORT loc_1_3BAA
loc_1_3BA4:
        push   edi
        call   sub_1_0B28
loc_1_3BAA:
        add    esp, 4
        mov    bl, al
loc_1_3BAF:
        movzx  eax, di
        cmp    eax, 1bh
        je     SHORT loc_1_3BBE
        cmp    eax, 1ch
        je     SHORT loc_1_3BC4
        jmp    SHORT loc_1_3BD9
loc_1_3BBE:
        mov    byte ptr [esi + 6], 0
        jmp    SHORT loc_1_3BD9
loc_1_3BC4:
        mov    byte ptr [esi + 7], 0
        cmp    byte ptr [esp + 0fh], 1
        mov    al, 0
        adc    al, 0ffh
        and    al, 19h
        xor    al, bl
        xor    al, 9ch
        mov    bl, al
loc_1_3BD9:
        call   sub_1_553D
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_3BE7:
        mov    al, byte ptr [esp + 8]
        cmp    al, 6
        jbe    SHORT loc_1_3BF1
        mov    al, 6
loc_1_3BF1:
        movzx  ecx, al
        mov    eax, dword ptr [esp + 4]
        lea    edx, [ecx + eax + 105h]
        mov    eax, dword ptr [esp + 0ch]
        mov    cl, byte ptr [edx]
        and    cl, 3
        mov    byte ptr [eax], cl
        mov    cl, byte ptr [edx]
        and    cl, 8
        mov    eax, dword ptr [esp + 10h]
        shr    cl, 3
        mov    byte ptr [eax], cl
        mov    cl, byte ptr [edx]
        and    cl, 10h
        mov    eax, dword ptr [esp + 14h]
        shr    cl, 4
        mov    byte ptr [eax], cl
        mov    cl, byte ptr [edx]
        and    cl, 20h
        mov    eax, dword ptr [esp + 18h]
        shr    cl, 5
        mov    byte ptr [eax], cl
        ret
sub_1_3C35:
        push   ebp
        db  08Bh, 00Dh, 014h, 0D0h, 040h, 000h
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        mov    dword ptr [ebp - 18h], ecx
        push   edi
        xor    esi, esi
        cmp    byte ptr [eax + 3ch], 0
        jne    SHORT loc_1_3CAE
        mov    al, byte ptr [eax + 10ch]
        mov    edx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [edx + 0dh], al
        mov    al, byte ptr [ecx + 10ch]
        mov    byte ptr [edx + 0ch], al
        mov    al, byte ptr [ecx + 10ch]
        mov    byte ptr [edx + 0fh], al
        mov    al, byte ptr [ecx + 10ch]
        mov    byte ptr [edx + 10h], al
        mov    al, byte ptr [ecx + 10ch]
        mov    byte ptr [edx + 0eh], al
        mov    al, byte ptr [ecx + 10ch]
        mov    byte ptr [edx + 11h], al
        mov    al, byte ptr [ecx + 10eh]
        mov    byte ptr [edx + 0ah], al
        mov    al, byte ptr [ecx + 10eh]
        mov    byte ptr [edx + 9], al
        mov    al, byte ptr [ecx + 10dh]
        mov    byte ptr [edx + 0bh], al
        jmp    NEAR PTR loc_1_409D
loc_1_3CAE:
        cmp    byte ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_3E42
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], 0
        mov    al, byte ptr [ecx + 10fh]
        mov    byte ptr [ebp - 8], al
        test   al, al
        jne    SHORT loc_1_3CCF
        inc    byte ptr [ebp - 8]
loc_1_3CCF:
        push   19h
        call   sub_1_3B66
        add    esp, 4
        mov    byte ptr [ebp - 9], al
        and    al, 0f7h
        push   eax
        push   19h
        call   sub_1_1D05
        add    esp, 8
        call   sub_1_55CB
loc_1_3CEE:
        xor    bx, bx
loc_1_3CF1:
        test   esi, esi
        jne    NEAR PTR loc_1_3DC4
        mov    al, bl
        or     al, 0f0h
        mov    byte ptr [ebp - 14h], al
        not    al
        push   eax
        push   2
        call   sub_1_1D05
        add    esp, 8
        push   dword ptr [ebp - 14h]
        push   2
        call   sub_1_1D05
        add    esp, 8
        push   2
        call   sub_1_3B66
        add    esp, 4
        mov    byte ptr [ebp - 1], al
        mov    al, bl
        and    al, 0fh
        mov    byte ptr [ebp - 10h], al
        not    al
        push   eax
        push   2
        call   sub_1_1D05
        add    esp, 8
        push   dword ptr [ebp - 10h]
        push   2
        call   sub_1_1D05
        add    esp, 8
        push   2
        call   sub_1_3B66
        add    esp, 4
        mov    cl, byte ptr [ebp - 14h]
        cmp    byte ptr [ebp - 1], cl
        jne    SHORT loc_1_3D7D
        cmp    al, byte ptr [ebp - 10h]
        jne    SHORT loc_1_3D7D
        push   1ch
        call   sub_1_2C87
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_1_3D93
        push   2
        push   15934h
        push   1597h
        jmp    SHORT loc_1_3D89
loc_1_3D7D:
        push   1
        push   15934h
        push   1595h
loc_1_3D89:
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, eax
loc_1_3D93:
        inc    bx
        cmp    bx, 10h
        jb     NEAR PTR loc_1_3CF1
        test   esi, esi
        jne    SHORT loc_1_3DC4
        mov    ecx, dword ptr [ebp - 18h]
        mov    al, byte ptr [ecx + 0dh]
        cmp    byte ptr [ebp - 8], al
        jae    SHORT loc_1_3DBE
        dec    al
        mov    byte ptr [ecx + 0dh], al
        mov    byte ptr [ecx + 0ch], al
        mov    al, byte ptr [ecx + 0dh]
        mov    byte ptr [ecx + 10h], al
        jmp    SHORT loc_1_3DF4
loc_1_3DBE:
        mov    byte ptr [ebp - 2], 1
        jmp    SHORT loc_1_3DF4
loc_1_3DC4:
        mov    ecx, dword ptr [ebp - 18h]
        mov    al, byte ptr [ecx + 0dh]
        cmp    al, 5
        jne    SHORT loc_1_3DE6
        push   3
        push   15934h
        push   1595h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, eax
        jmp    SHORT loc_1_3DF4
loc_1_3DE6:
        inc    al
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ebp - 2], 1
        xor    esi, esi
        mov    byte ptr [ecx + 0dh], al
loc_1_3DF4:
        test   esi, esi
        jne    SHORT loc_1_3E02
        cmp    byte ptr [ebp - 2], 0
        je     NEAR PTR loc_1_3CEE
loc_1_3E02:
        push   0
        push   2
        call   sub_1_1D05
        add    esp, 8
        push   0ah
        call   sub_1_0720
        add    esp, 4
        push   4
        push   2
        call   sub_1_1D05
        add    esp, 8
        push   1
        push   1ah
        call   sub_1_1D05
        add    esp, 8
        push   dword ptr [ebp - 9]
        push   19h
        call   sub_1_1D05
        add    esp, 8
        call   sub_1_553D
loc_1_3E42:
        test   esi, esi
        jne    NEAR PTR loc_1_409D
        cmp    byte ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_403A
        mov    byte ptr [ebp - 2], 0
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ebp - 1], 5
        test   byte ptr [ecx + 3], 4
        je     SHORT loc_1_3E6A
        add    ecx, 0ah
        jmp    SHORT loc_1_3E70
loc_1_3E6A:
        mov    ecx, dword ptr [ebp - 18h]
        add    ecx, 9
loc_1_3E70:
        mov    dword ptr [ebp - 8], ecx
loc_1_3E73:
        mov    eax, dword ptr [ebp - 8]
        mov    dl, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 10h]
        xor    bl, bl
        mov    byte ptr [eax], dl
loc_1_3E80:
        test   esi, esi
        jne    NEAR PTR loc_1_3F77
        push   0
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 112h]
        push   2
        dec    ax
        push   eax
        push   0
        push   edi
        call   sub_1_3434
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_1_3EC8
        push   4
        push   15934h
        push   eax
        call   sub_1_247A
        add    esp, 4
        push   eax
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, eax
        jmp    NEAR PTR loc_1_3F49
loc_1_3EC8:
        push   0
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 112h]
        push   3
        dec    ax
        push   eax
        push   0
        push   edi
        call   sub_1_3434
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_1_3F05
        push   5
        push   15934h
        push   eax
        call   sub_1_247A
        add    esp, 4
        push   eax
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, eax
        jmp    SHORT loc_1_3F49
loc_1_3F05:
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 112h]
        dec    ax
        push   eax
        push   edi
        push   1
        call   sub_1_423F
        add    esp, 10h
        cmp    al, 1
        jne    SHORT loc_1_3F49
        push   6
        push   15934h
        push   1596h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, eax
        push   ebx
        push   2000h
        push   edi
        push   0
        call   sub_1_423F
        add    esp, 10h
loc_1_3F49:
        add    edi, 2000h
        inc    bl
        cmp    bl, 4
        jb     NEAR PTR loc_1_3E80
        test   esi, esi
        jne    SHORT loc_1_3F77
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [ebp - 1]
        cmp    byte ptr [ecx + 110h], dl
        jae    SHORT loc_1_3F71
        dec    byte ptr [ebp - 1]
        jmp    SHORT loc_1_3FC4
loc_1_3F71:
        mov    byte ptr [ebp - 2], 1
        jmp    SHORT loc_1_3FC4
loc_1_3F77:
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1596h
        je     SHORT loc_1_3F95
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1597h
        jne    SHORT loc_1_3FC4
loc_1_3F95:
        cmp    byte ptr [ebp - 1], 5
        jae    SHORT loc_1_3FAE
        mov    eax, dword ptr [ebp - 8]
        xor    esi, esi
        mov    byte ptr [ebp - 2], 1
        inc    byte ptr [ebp - 1]
        mov    dl, byte ptr [ebp - 1]
        mov    byte ptr [eax], dl
        jmp    SHORT loc_1_3FC4
loc_1_3FAE:
        push   8
        push   15934h
        push   1595h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, eax
loc_1_3FC4:
        test   esi, esi
        jne    SHORT loc_1_3FF7
        cmp    byte ptr [ebp - 2], 0
        je     NEAR PTR loc_1_3E73
        test   esi, esi
        jne    SHORT loc_1_4005
        mov    eax, dword ptr [ebp - 18h]
        sub    eax, dword ptr [ebp - 8]
        cmp    eax, -9
        je     SHORT loc_1_3FE6
        cmp    eax, -0ah
        jne    SHORT loc_1_3FEA
loc_1_3FE6:
        mov    byte ptr [ebp - 2], 0
loc_1_3FEA:
        mov    byte ptr [ebp - 1], 5
        mov    ecx, dword ptr [ebp - 18h]
        add    ecx, 0bh
        mov    dword ptr [ebp - 8], ecx
loc_1_3FF7:
        test   esi, esi
        jne    SHORT loc_1_403A
        cmp    byte ptr [ebp - 2], 0
        je     NEAR PTR loc_1_3E73
loc_1_4005:
        test   esi, esi
        jne    NEAR PTR loc_1_409D
        mov    ecx, dword ptr [ebp - 18h]
        test   byte ptr [ecx + 3], 10h
        je     SHORT loc_1_403A
        call   sub_1_4CBF
        test   eax, eax
        je     SHORT loc_1_403A
        push   9
        push   15934h
        push   eax
        call   sub_1_247A
        add    esp, 4
        push   eax
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, eax
loc_1_403A:
        test   esi, esi
        jne    SHORT loc_1_409D
        mov    ecx, dword ptr [ebp - 18h]
        mov    al, byte ptr [ecx + 0dh]
        mov    byte ptr [ecx + 0ch], al
        mov    al, byte ptr [ecx + 0dh]
        mov    byte ptr [ecx + 0fh], al
        mov    al, byte ptr [ecx + 0dh]
        mov    byte ptr [ecx + 10h], al
        mov    al, byte ptr [ecx + 0dh]
        mov    byte ptr [ecx + 0eh], al
        mov    al, byte ptr [ecx + 0dh]
        mov    byte ptr [ecx + 11h], al
        test   byte ptr [ecx + 3], 4
        je     SHORT loc_1_406F
        lea    eax, [ecx + 9]
        mov    cl, byte ptr [ecx + 0ah]
        mov    byte ptr [eax], cl
        jmp    SHORT loc_1_407D
loc_1_406F:
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 18h]
        add    eax, 9
        mov    dl, byte ptr [eax]
        mov    byte ptr [ecx + 0ah], dl
loc_1_407D:
        mov    ecx, dword ptr [ebp - 18h]
        mov    ebx, dword ptr [ebp + 8]
        mov    dl, byte ptr [ecx + 0ch]
        mov    byte ptr [ebx + 10ch], dl
        mov    dl, byte ptr [ecx + 0bh]
        mov    byte ptr [ebx + 10dh], dl
        mov    al, byte ptr [eax]
        mov    byte ptr [ebx + 10eh], al
loc_1_409D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_40A6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    eax, 2
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    byte ptr [ebp - 5], bl
        db  08Bh, 03Dh, 014h, 0D0h, 040h, 000h
        mov    cl, byte ptr [ebp + 10h]
        mov    byte ptr [ebp - 8], al
        mov    byte ptr [ebp - 7], al
        mov    dl, byte ptr [edi + 13h]
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 4], bl
        mov    byte ptr [ebp - 1], cl
        mov    byte ptr [ebp - 6], dl
loc_1_40D7:
        lea    eax, [ebp - 9]
        lea    ecx, [ebp - 0ah]
        push   eax
        lea    edx, [ebp - 3]
        push   ecx
        lea    eax, [ebp - 2]
        push   edx
        push   eax
        push   dword ptr [ebp - 1]
        push   esi
        call   sub_1_3BE7
        add    esp, 18h
        mov    al, byte ptr [edi + 3]
        cmp    byte ptr [ebp - 3], 0
        jne    SHORT loc_1_4116
        and    al, 0f8h
        or     al, byte ptr [ebp - 2]
        push   eax
        push   18h
        call   sub_1_1D05
        mov    byte ptr [edi + 12h], 1
        mov    byte ptr [edi + 13h], 1
        add    esp, 8
        jmp    SHORT loc_1_4133
loc_1_4116:
        and    al, 0fch
        mov    cl, byte ptr [ebp - 2]
        add    cl, 4
        or     al, cl
        push   eax
        push   18h
        call   sub_1_1D05
        mov    byte ptr [edi + 12h], 0
        mov    byte ptr [edi + 13h], 0
        add    esp, 8
loc_1_4133:
        push   18h
        call   sub_1_3B66
        add    esp, 4
        cmp    byte ptr [edi + 3], al
        je     NEAR PTR loc_1_41D0
        movzx  eax, byte ptr [ebp - 5]
        test   eax, eax
        je     SHORT loc_1_4170
        cmp    eax, 1
        je     SHORT loc_1_4197
        push   1
        cmp    eax, 2
        je     SHORT loc_1_41A5
        push   15933h
        push   1593h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
        jmp    SHORT loc_1_41CB
loc_1_4170:
        push   1
        push   esi
        call   sub_1_2F18
        add    esp, 8
        mov    ebx, eax
        call   sub_1_4656
        mov    al, byte ptr [edi + 8]
        or     al, 2
        push   eax
        push   1eh
        call   sub_1_1D05
        add    esp, 8
        mov    al, byte ptr [ebp + 10h]
        jmp    SHORT loc_1_41C8
loc_1_4197:
        mov    al, byte ptr [ebp - 6]
        mov    cl, byte ptr [ebp + 0ch]
        mov    byte ptr [edi + 13h], al
        mov    byte ptr [ebp - 1], cl
        jmp    SHORT loc_1_41CB
loc_1_41A5:
        push   esi
        call   sub_1_2F18
        add    esp, 8
        mov    ebx, eax
        call   sub_1_4656
        mov    al, byte ptr [edi + 8]
        or     al, 2
        push   eax
        push   1eh
        call   sub_1_1D05
        add    esp, 8
        mov    al, byte ptr [ebp + 0ch]
loc_1_41C8:
        mov    byte ptr [ebp - 1], al
loc_1_41CB:
        inc    byte ptr [ebp - 5]
        jmp    SHORT loc_1_41F4
loc_1_41D0:
        mov    al, byte ptr [ebp - 1]
        cmp    al, byte ptr [ebp + 0ch]
        jne    SHORT loc_1_41F0
        push   1
        push   15933h
        push   1592h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
        jmp    SHORT loc_1_41F4
loc_1_41F0:
        mov    byte ptr [ebp - 4], 1
loc_1_41F4:
        test   ebx, ebx
        jne    SHORT loc_1_4236
        cmp    byte ptr [ebp - 4], 0
        je     NEAR PTR loc_1_40D7
        test   ebx, ebx
        jne    SHORT loc_1_4236
        movzx  eax, byte ptr [ebp - 2]
        test   eax, eax
        je     SHORT loc_1_4215
        cmp    eax, 2
        je     SHORT loc_1_421B
        jmp    SHORT loc_1_422A
loc_1_4215:
        mov    byte ptr [ebp - 7], 2
        jmp    SHORT loc_1_421F
loc_1_421B:
        mov    byte ptr [ebp - 7], 1
loc_1_421F:
        cmp    byte ptr [ebp - 3], 1
        sbb    al, al
        add    al, 2
        mov    byte ptr [ebp - 8], al
loc_1_422A:
        mov    al, byte ptr [ebp - 8]
        mov    cl, byte ptr [ebp - 7]
        mov    byte ptr [edi + 14h], al
        mov    byte ptr [edi + 15h], cl
loc_1_4236:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_423F:
        push   ebp
        xor    al, al
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    edx, dword ptr [ebp + 0ch]
        movzx  ecx, byte ptr [ebp + 14h]
        cmp    byte ptr [ebp + 8], al
        jne    NEAR PTR loc_1_430C
        test   ecx, ecx
        je     SHORT loc_1_4271
        cmp    ecx, 1
        je     SHORT loc_1_4291
        cmp    ecx, 2
        je     SHORT loc_1_42B1
        cmp    ecx, 3
        je     SHORT loc_1_42E8
        jmp    NEAR PTR loc_1_442E
loc_1_4271:
        movzx  ecx, word ptr [ebp + 10h]
        add    ecx, edx
        cmp    ecx, edx
        jbe    NEAR PTR loc_1_442E
loc_1_427F:
        mov    byte ptr [edx], bl
        inc    bl
        inc    edx
        mov    byte ptr [edx], 0
        inc    edx
        cmp    ecx, edx
        ja     SHORT loc_1_427F
        jmp    NEAR PTR loc_1_442E
loc_1_4291:
        movzx  ecx, word ptr [ebp + 10h]
        add    ecx, edx
        cmp    ecx, edx
        jbe    NEAR PTR loc_1_442E
loc_1_429F:
        mov    byte ptr [edx], bl
        inc    bl
        inc    edx
        mov    byte ptr [edx], 0ffh
        inc    edx
        cmp    ecx, edx
        ja     SHORT loc_1_429F
        jmp    NEAR PTR loc_1_442E
loc_1_42B1:
        movzx  ecx, word ptr [ebp + 10h]
        add    ecx, edx
        cmp    ecx, edx
        jbe    NEAR PTR loc_1_442E
loc_1_42BF:
        mov    byte ptr [edx], 0f0h
        inc    edx
        mov    byte ptr [edx], 0fh
        inc    edx
        mov    byte ptr [edx], 0f0h
        inc    edx
        mov    byte ptr [edx], 0fh
        inc    edx
        mov    byte ptr [edx], 55h
        inc    edx
        mov    byte ptr [edx], 0aah
        inc    edx
        mov    byte ptr [edx], 5ah
        inc    edx
        mov    byte ptr [edx], 0a5h
        inc    edx
        cmp    ecx, edx
        ja     SHORT loc_1_42BF
        jmp    NEAR PTR loc_1_442E
loc_1_42E8:
        movzx  ecx, word ptr [ebp + 10h]
        lea    esi, [ecx + edx]
        cmp    esi, edx
        jbe    NEAR PTR loc_1_442E
loc_1_42F7:
        mov    cl, bl
        mov    byte ptr [edx], bl
        inc    edx
        inc    bl
        not    cl
        mov    byte ptr [edx], cl
        inc    edx
        cmp    esi, edx
        ja     SHORT loc_1_42F7
        jmp    NEAR PTR loc_1_442E
loc_1_430C:
        test   ecx, ecx
        je     SHORT loc_1_4328
        cmp    ecx, 1
        je     SHORT loc_1_435F
        cmp    ecx, 2
        je     SHORT loc_1_4396
        cmp    ecx, 3
        je     NEAR PTR loc_1_4403
        jmp    NEAR PTR loc_1_442E
loc_1_4328:
        movzx  ecx, word ptr [ebp + 10h]
        lea    esi, [ecx + edx]
        cmp    esi, edx
        jbe    NEAR PTR loc_1_442E
loc_1_4337:
        test   al, al
        jne    NEAR PTR loc_1_442E
        mov    edi, edx
        mov    cl, bl
        inc    edx
        inc    bl
        cmp    byte ptr [edi], cl
        je     SHORT loc_1_434C
        mov    al, 1
loc_1_434C:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0
        je     SHORT loc_1_4356
        mov    al, 1
loc_1_4356:
        cmp    esi, edx
        ja     SHORT loc_1_4337
        jmp    NEAR PTR loc_1_442E
loc_1_435F:
        movzx  ecx, word ptr [ebp + 10h]
        lea    esi, [ecx + edx]
        cmp    esi, edx
        jbe    NEAR PTR loc_1_442E
loc_1_436E:
        test   al, al
        jne    NEAR PTR loc_1_442E
        mov    edi, edx
        mov    cl, bl
        inc    edx
        inc    bl
        cmp    byte ptr [edi], cl
        je     SHORT loc_1_4383
        mov    al, 1
loc_1_4383:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0ffh
        je     SHORT loc_1_438D
        mov    al, 1
loc_1_438D:
        cmp    esi, edx
        ja     SHORT loc_1_436E
        jmp    NEAR PTR loc_1_442E
loc_1_4396:
        movzx  ecx, word ptr [ebp + 10h]
        lea    esi, [ecx + edx]
        cmp    esi, edx
        jbe    NEAR PTR loc_1_442E
loc_1_43A5:
        test   al, al
        jne    NEAR PTR loc_1_442E
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0f0h
        je     SHORT loc_1_43B7
        mov    al, 1
loc_1_43B7:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0fh
        je     SHORT loc_1_43C1
        mov    al, 1
loc_1_43C1:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0f0h
        je     SHORT loc_1_43CB
        mov    al, 1
loc_1_43CB:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0fh
        je     SHORT loc_1_43D5
        mov    al, 1
loc_1_43D5:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 55h
        je     SHORT loc_1_43DF
        mov    al, 1
loc_1_43DF:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0aah
        je     SHORT loc_1_43E9
        mov    al, 1
loc_1_43E9:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 5ah
        je     SHORT loc_1_43F3
        mov    al, 1
loc_1_43F3:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0a5h
        je     SHORT loc_1_43FD
        mov    al, 1
loc_1_43FD:
        cmp    esi, edx
        ja     SHORT loc_1_43A5
        jmp    SHORT loc_1_442E
loc_1_4403:
        movzx  ecx, word ptr [ebp + 10h]
        lea    esi, [ecx + edx]
        cmp    esi, edx
        jbe    SHORT loc_1_442E
loc_1_440E:
        test   al, al
        jne    SHORT loc_1_442E
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], bl
        je     SHORT loc_1_441B
        mov    al, 1
loc_1_441B:
        mov    edi, edx
        mov    cl, bl
        inc    edx
        inc    bl
        not    cl
        cmp    cl, byte ptr [edi]
        je     SHORT loc_1_442A
        mov    al, 1
loc_1_442A:
        cmp    esi, edx
        ja     SHORT loc_1_440E
loc_1_442E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_4433:
        mov    cl, byte ptr [esp + 0ch]
        sub    esp, 4
        test   cl, cl
        push   ebx
        push   esi
        je     SHORT loc_1_4442
        mov    cl, 8
loc_1_4442:
        mov    al, byte ptr [esp + 1ch]
        test   al, al
        je     SHORT loc_1_444C
        mov    al, 10h
loc_1_444C:
        mov    bl, byte ptr [esp + 20h]
        test   bl, bl
        je     SHORT loc_1_4456
        mov    bl, 20h
loc_1_4456:
        xor    dl, dl
        add    bl, al
        add    bl, cl
        mov    al, byte ptr [esp + 14h]
        add    bl, al
        mov    ecx, dword ptr [esp + 10h]
        mov    al, byte ptr [esp + 0bh]
loc_1_446A:
        movzx  esi, dl
        cmp    byte ptr [esi + ecx + 105h], bl
        jne    SHORT loc_1_4478
        mov    al, dl
loc_1_4478:
        inc    dl
        cmp    dl, 6
        jbe    SHORT loc_1_446A
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_4485:
        push   esi
        call   sub_1_4656
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_5768
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_44A5
        push   esi
        call   sub_1_564A
        add    esp, 4
loc_1_44A5:
        pop    esi
        ret
sub_1_44A7:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        xor    cl, cl
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
loc_1_44BA:
        movzx  eax, cl
        mov    byte ptr [ebp + eax - 8], cl
        inc    cl
        cmp    cl, 5
        jb     SHORT loc_1_44BA
        mov    al, byte ptr [esi + 4]
        or     al, 0ch
        push   eax
        push   19h
        call   sub_1_1D05
        add    esp, 8
        lea    ecx, [ebp - 8]
        push   0
        push   5
        push   ecx
        call   sub_1_39F7
        add    esp, 0ch
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_462B
        movzx  eax, word ptr [esi + 18h]
        inc    eax
        push   eax
        call   sub_1_3281
        add    esp, 4
        test   al, 40h
        jne    SHORT loc_1_451F
        push   1
        push   15940h
        push   1594h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
        jmp    NEAR PTR loc_1_4627
loc_1_451F:
        movzx  eax, word ptr [esi + 18h]
        inc    eax
        push   eax
        call   sub_1_3281
        add    esp, 4
        test   al, 8
        jne    SHORT loc_1_454C
        push   2
        push   15940h
        push   1594h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
        jmp    NEAR PTR loc_1_4627
loc_1_454C:
        push   0
        lea    eax, [ebp - 8]
        push   5
        push   eax
        call   sub_1_398D
        add    esp, 0ch
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_462B
        push   4
        push   1ah
        call   sub_1_1D05
        add    esp, 8
        push   1ah
        call   sub_1_3B66
        add    esp, 4
        test   al, 4
        je     SHORT loc_1_459B
        push   3
        push   15940h
        push   1594h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
        jmp    NEAR PTR loc_1_4627
loc_1_459B:
        push   0
        lea    eax, [ebp - 8]
        push   5
        push   eax
        call   sub_1_398D
        add    esp, 0ch
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_462B
        movzx  eax, word ptr [esi + 18h]
        inc    eax
        push   eax
        call   sub_1_3281
        add    esp, 4
        test   al, 40h
        jne    SHORT loc_1_45DB
        push   4
        push   15940h
        push   1594h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
        jmp    SHORT loc_1_4627
loc_1_45DB:
        movzx  eax, word ptr [esi + 18h]
        inc    eax
        push   eax
        call   sub_1_3281
        add    esp, 4
        test   al, 8
        jne    SHORT loc_1_4605
        push   5
        push   15940h
        push   1594h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
        jmp    SHORT loc_1_4627
loc_1_4605:
        push   0
        lea    eax, [ebp - 8]
        push   5
        push   eax
        call   sub_1_39F7
        add    esp, 0ch
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_462B
        push   4
        push   1ah
        call   sub_1_1D05
        add    esp, 8
loc_1_4627:
        test   edi, edi
        je     SHORT loc_1_463D
loc_1_462B:
        push   1
        push   15940h
        push   edi
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
loc_1_463D:
        mov    al, byte ptr [esi + 4]
        and    al, 0f3h
        push   eax
        push   19h
        call   sub_1_1D05
        add    esp, 8
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
_LTEXT ends
        end
