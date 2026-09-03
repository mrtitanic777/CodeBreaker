; ati_icode_02.asm -- ATI object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        cld
        db  089h, 01Dh, 080h, 001h, 000h, 000h
        db  066h, 0C7h, 005h, 0A4h, 001h, 000h, 000h, 000h, 000h
        mov    ax, 0a012h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        cmp    ah, 0
        jne    NEAR PTR loc_2_0179
        cmp    dx, 2ech
        je     SHORT loc_2_003C
        cmp    dx, 1c8h
        je     SHORT loc_2_003C
        or     cx, cx
        je     NEAR PTR loc_2_0179
loc_2_003C:
        sub    dx, 2ech
        db  066h, 089h, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 089h, 00Dh, 0A6h, 001h, 000h, 000h
        xor    edx, edx
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_006A
        add    dx, 42ech
        jmp    SHORT loc_2_006F
loc_2_006A:
        add    dx, 36ch
loc_2_006F:
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        mov    ecx, eax
        sub    dx, 3
        mov    eax, 0a55aa55ah
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        sub    dx, 3
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        sub    dx, 3
        xchg   ecx, eax
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        cmp    ecx, 0a55aa55ah
        jne    NEAR PTR loc_2_0179
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_00FA
        add    dx, 46ech
        jmp    SHORT loc_2_00FF
loc_2_00FA:
        add    dx, 370h
loc_2_00FF:
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        mov    ecx, eax
        sub    dx, 3
        mov    eax, 12345678h
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        sub    dx, 3
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        inc    dl
        in     al, dx
        ror    eax, 8
        sub    dx, 3
        xchg   ecx, eax
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        ror    eax, 8
        inc    dl
        out    dx, al
        cmp    ecx, 12345678h
        jne    SHORT loc_2_0179
        db  0C6h, 005h, 0A2h, 001h, 000h, 000h, 064h
        jmp    NEAR PTR loc_2_026F
loc_2_0179:
        mov    dx, 42e8h
        mov    ax, 900fh
        out    dx, ax
        db  0E8h, 043h, 00Eh, 0FEh, 0FFh
        mov    ax, 400fh
        out    dx, ax
        mov    dx, 92e8h
        mov    ax, 5555h
        out    dx, ax
        db  0E8h, 02Eh, 00Eh, 0FEh, 0FFh
        in     ax, dx
        cmp    ax, 5555h
        jne    SHORT loc_2_0202
        mov    ax, 0aaaah
        out    dx, ax
        db  0E8h, 01Bh, 00Eh, 0FEh, 0FFh
        in     ax, dx
        cmp    ax, 0aaaah
        jne    SHORT loc_2_0202
        mov    dx, 52eeh
        in     ax, dx
        ror    eax, 10h
        mov    ax, 5555h
        out    dx, ax
        db  0E8h, 0FFh, 00Dh, 0FEh, 0FFh
        in     ax, dx
        ror    eax, 10h
        out    dx, ax
        ror    eax, 10h
        cmp    ax, 5555h
        jne    SHORT loc_2_0202
        db  0C6h, 005h, 0A2h, 001h, 000h, 000h, 008h
        mov    dx, 8ee8h
        mov    ax, 0aaaah
        out    dx, ax
        mov    dx, 0daeeh
        in     ax, dx
        cmp    ax, 2aah
        jne    SHORT loc_2_026F
        db  0C6h, 005h, 0A2h, 001h, 000h, 000h, 032h
        jmp    SHORT loc_2_026F
loc_2_0202:
        push   0
        push   100h
        push   0c0000h
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_2_0A71
        cld
        mov    edi, eax
        add    edi, 30h
        db  0BEh, 074h, 001h, 000h, 000h
        mov    ecx, 0ah
        repe cmpsb
        jne    NEAR PTR loc_2_0A71
        cmp    byte ptr [eax + 40h], 33h
        jne    NEAR PTR loc_2_0A71
        push   ebp
        push   ebx
        mov    eax, 1203h
        mov    ebx, 5506h
        mov    ebp, 0ffffh
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        cmp    bp, -1
        pop    ebx
        pop    ebp
        je     NEAR PTR loc_2_0A71
        db  0C6h, 005h, 0A2h, 001h, 000h, 000h, 000h
loc_2_026F:
        push   0
        push   47h
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 088h, 001h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_0A71
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     NEAR PTR loc_2_0A71
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 000h
        je     NEAR PTR loc_2_035E
        db  0C7h, 047h, 004h, 0BEh, 000h, 000h, 000h
        db  0C7h, 087h, 0C0h, 000h, 000h, 000h, 0D7h, 00Fh, 000h, 000h
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 032h
        jb     NEAR PTR loc_2_035E
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 087h, 094h, 000h, 000h, 000h, 019h, 010h, 000h, 000h
        db  0C7h, 087h, 090h, 000h, 000h, 000h, 011h, 010h, 000h, 000h
        ja     SHORT loc_2_034A
        db  0C7h, 047h, 008h, 007h, 000h, 000h, 000h
        db  0C7h, 047h, 00Ch, 06Ah, 000h, 000h, 000h
        db  0C7h, 047h, 058h, 0C8h, 000h, 000h, 000h
        db  0C7h, 047h, 05Ch, 00Dh, 001h, 000h, 000h
        db  0C7h, 047h, 03Ch, 086h, 00Fh, 000h, 000h
        db  0C7h, 087h, 088h, 000h, 000h, 000h, 071h, 010h, 000h, 000h
        db  0C7h, 087h, 080h, 000h, 000h, 000h, 0B7h, 010h, 000h, 000h
        db  0C7h, 087h, 084h, 000h, 000h, 000h, 0E6h, 010h, 000h, 000h
        db  0C7h, 087h, 09Ch, 000h, 000h, 000h, 022h, 010h, 000h, 000h
        db  0C7h, 087h, 0A0h, 000h, 000h, 000h, 04Dh, 010h, 000h, 000h
        db  0C7h, 087h, 0ACh, 000h, 000h, 000h, 0F5h, 000h, 000h, 000h
        db  0C7h, 087h, 08Ch, 000h, 000h, 000h, 00Ah, 001h, 000h, 000h
        jmp    SHORT loc_2_035E
loc_2_034A:
        db  0C7h, 087h, 080h, 000h, 000h, 000h, 0B7h, 010h, 000h, 000h
        db  0C7h, 087h, 084h, 000h, 000h, 000h, 0E6h, 010h, 000h, 000h
loc_2_035E:
        db  0C7h, 047h, 010h, 03Ch, 001h, 000h, 000h
        db  0C7h, 047h, 014h, 06Dh, 001h, 000h, 000h
        db  0C7h, 047h, 018h, 099h, 001h, 000h, 000h
        db  0C7h, 047h, 01Ch, 0BDh, 001h, 000h, 000h
        db  0C7h, 047h, 034h, 044h, 002h, 000h, 000h
        db  0C7h, 047h, 038h, 017h, 005h, 000h, 000h
        db  0C7h, 087h, 0A8h, 000h, 000h, 000h, 0B0h, 00Ah, 000h, 000h
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 032h
        jne    NEAR PTR loc_2_059F
        mov    edx, 76eeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 82e8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 82eeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 86e8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 8ae8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 8ee8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 8eeeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 92e8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 9aeeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0a2e8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0a2eeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0a6e8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0a6eeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0aaeeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0aeeeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0b2eeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0baeeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0bee8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0beeeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0c2eeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0caeeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0ceeeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0d2eeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0d6eeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0daeeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0deeeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0e2e8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0e2eeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0e6eeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0feeeh
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        db  0BEh, 07Dh, 000h, 000h, 000h
        mov    edx, 4aeeh
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     NEAR PTR loc_2_0A71
        mov    edx, 4aeeh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 4aefh
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     NEAR PTR loc_2_0A71
        mov    edx, 4aefh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        db  0BEh, 0B9h, 000h, 000h, 000h
        mov    edx, 4ae8h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     NEAR PTR loc_2_0A71
        mov    edx, 4ae8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 4ae9h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     NEAR PTR loc_2_0A71
        mov    edx, 4ae9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
loc_2_059F:
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        mov    edx, 2e8h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        mov    edx, 1ceh
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     NEAR PTR loc_2_0A71
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        mov    edx, 1cfh
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     NEAR PTR loc_2_0A71
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        db  0C7h, 005h, 08Ch, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 064h
        jne    NEAR PTR loc_2_0A4A
        xor    edx, edx
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_0624
        add    dx, 4eech
        jmp    SHORT loc_2_0629
loc_2_0624:
        add    dx, 38ch
loc_2_0629:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_0652
        add    dx, 52ech
        jmp    SHORT loc_2_0657
loc_2_0652:
        add    dx, 39ch
loc_2_0657:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_0680
        add    dx, 56ech
        jmp    SHORT loc_2_0685
loc_2_0680:
        add    dx, 3a0h
loc_2_0685:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_06AE
        add    dx, 5aech
        jmp    SHORT loc_2_06B3
loc_2_06AE:
        add    dx, 3a4h
loc_2_06B3:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_06DC
        add    dx, 66ech
        jmp    SHORT loc_2_06E1
loc_2_06DC:
        add    dx, 3bch
loc_2_06E1:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_070A
        add    dx, 6aech
        jmp    SHORT loc_2_070F
loc_2_070A:
        add    dx, 3c8h
loc_2_070F:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_0738
        add    dx, 6ech
        jmp    SHORT loc_2_073D
loc_2_0738:
        add    dx, 2f0h
loc_2_073D:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_0766
        add    dx, 0aech
        jmp    SHORT loc_2_076B
loc_2_0766:
        add    dx, 2f4h
loc_2_076B:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_0794
        add    dx, 0eech
        jmp    SHORT loc_2_0799
loc_2_0794:
        add    dx, 2f8h
loc_2_0799:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_07C2
        add    dx, 12ech
        jmp    SHORT loc_2_07C7
loc_2_07C2:
        add    dx, 2fch
loc_2_07C7:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_07F0
        add    dx, 16ech
        jmp    SHORT loc_2_07F5
loc_2_07F0:
        add    dx, 300h
loc_2_07F5:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_081E
        add    dx, 1aech
        jmp    SHORT loc_2_0823
loc_2_081E:
        add    dx, 304h
loc_2_0823:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_084C
        add    dx, 1eech
        jmp    SHORT loc_2_0851
loc_2_084C:
        add    dx, 308h
loc_2_0851:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_087A
        add    dx, 22ech
        jmp    SHORT loc_2_087F
loc_2_087A:
        add    dx, 32ch
loc_2_087F:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_08A8
        add    dx, 26ech
        jmp    SHORT loc_2_08AD
loc_2_08A8:
        add    dx, 330h
loc_2_08AD:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_08D6
        add    dx, 2aech
        jmp    SHORT loc_2_08DB
loc_2_08D6:
        add    dx, 334h
loc_2_08DB:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_0904
        add    dx, 2eech
        jmp    SHORT loc_2_0909
loc_2_0904:
        add    dx, 34ch
loc_2_0909:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_0932
        add    dx, 32ech
        jmp    SHORT loc_2_0937
loc_2_0932:
        add    dx, 350h
loc_2_0937:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_0960
        add    dx, 36ech
        jmp    SHORT loc_2_0965
loc_2_0960:
        add    dx, 354h
loc_2_0965:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_098E
        add    dx, 3aech
        jmp    SHORT loc_2_0993
loc_2_098E:
        add    dx, 358h
loc_2_0993:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_09BC
        add    dx, 3eech
        jmp    SHORT loc_2_09C1
loc_2_09BC:
        add    dx, 35ch
loc_2_09C1:
        call   sub_2_0A73
        jb     NEAR PTR loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_09EA
        add    dx, 4aech
        jmp    SHORT loc_2_09EF
loc_2_09EA:
        add    dx, 37ch
loc_2_09EF:
        call   sub_2_0A73
        jb     SHORT loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_0A14
        add    dx, 5eech
        jmp    SHORT loc_2_0A19
loc_2_0A14:
        add    dx, 3ach
loc_2_0A19:
        call   sub_2_0A73
        jb     SHORT loc_2_0A72
        db  0BEh, 0ABh, 00Eh, 000h, 000h
        db  066h, 08Bh, 015h, 0A4h, 001h, 000h, 000h
        db  066h, 0F7h, 005h, 0A6h, 001h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_0A3E
        add    dx, 62ech
        jmp    SHORT loc_2_0A43
loc_2_0A3E:
        add    dx, 3b0h
loc_2_0A43:
        call   sub_2_0A73
        jb     SHORT loc_2_0A72
loc_2_0A4A:
        db  080h, 03Dh, 0A2h, 001h, 000h, 000h, 032h
        jne    SHORT loc_2_0A6E
        mov    edx, 52eeh
        in     al, dx
        and    eax, 7fh
        mov    edx, 80h
        mul    edx
        add    eax, 0c000h
        db  066h, 0A3h, 098h, 001h, 000h, 000h
loc_2_0A6E:
        clc
        jmp    SHORT loc_2_0A72
loc_2_0A71:
        stc
loc_2_0A72:
        ret
sub_2_0A73:
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_0AAF
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        inc    edx
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_0AAF
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        inc    edx
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_0AAF
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        inc    edx
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_0AAF
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        clc
loc_2_0AAF:
        ret
loc_2_0AB0:
        db  00Fh, 0B6h, 005h, 0A2h, 001h, 000h, 000h
        ret
_ITEXT ends
        end
