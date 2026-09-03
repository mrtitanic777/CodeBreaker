; mga_lcode_01.asm -- MGA object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

MGA_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_000F
        db  0FFh, 024h, 085h, 011h, 000h, 000h, 000h
loc_1_000F:
        clc
        ret
        db  000h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  07Dh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
MGA_SYSTEM_EXIT:
        push   edi
        push   esi
        push   eax
        push   ebx
        call   sub_1_06AB
        je     SHORT loc_1_0091
        db  0A1h, 078h, 00Bh, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_0097
loc_1_0091:
        pop    ebx
        pop    eax
        pop    esi
        pop    edi
        stc
        ret
loc_1_0097:
        mov    edi, eax
        pushad
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xor    eax, eax
        xor    edi, edi
        xor    ecx, ecx
        db  08Dh, 015h, 07Ch, 00Bh, 000h, 000h
        db  0BEh, 067h, 006h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        popad
        mov    byte ptr [edi + 1fdeh], 0ah
        mov    byte ptr [edi + 1fdfh], 80h
        db  00Fh, 0BAh, 025h, 074h, 00Bh, 000h, 000h, 017h
        jae    SHORT loc_1_00EB
        mov    dx, 46e8h
        xor    al, al
        out    dx, al
        db  00Fh, 0BAh, 025h, 074h, 00Bh, 000h, 000h, 008h
        jae    SHORT loc_1_00EB
        mov    dword ptr [edi + 2010h], 0ac000h
loc_1_00EB:
        mov    al, 67h
        mov    byte ptr [edi + 1fc2h], al
        db  0A1h, 074h, 00Bh, 000h, 000h
        and    eax, 0ffh
        cmp    eax, 9
        jne    SHORT loc_1_0158
        mov    byte ptr [edi + 3c00h], 2ch
        mov    byte ptr [edi + 3c28h], 0efh
        mov    byte ptr [edi + 3c00h], 2fh
        mov    byte ptr [edi + 3c28h], 0
        mov    byte ptr [edi + 3c00h], 2ch
        mov    byte ptr [edi + 3c28h], 0
        mov    byte ptr [edi + 3c00h], 2dh
        mov    byte ptr [edi + 3c28h], 0b0h
        mov    byte ptr [edi + 3c28h], 29h
        mov    byte ptr [edi + 3c28h], 0f2h
        mov    ah, 0ffh
loc_1_014A:
        dec    ah
        je     SHORT loc_1_0158
        mov    al, byte ptr [edi + 3c28h]
        and    al, 4
        je     SHORT loc_1_014A
loc_1_0158:
        mov    byte ptr [edi + 1fd4h], 11h
        mov    al, byte ptr [edi + 1fd5h]
        and    al, 7fh
        mov    byte ptr [edi + 1fd5h], al
        xor    al, al
        mov    byte ptr [edi + 1fd4h], 1
        mov    byte ptr [edi + 1fd5h], al
        mov    byte ptr [edi + 1fd4h], 0
        mov    byte ptr [edi + 1fd5h], al
        mov    byte ptr [edi + 1fd4h], 6
        mov    byte ptr [edi + 1fd5h], al
        mov    byte ptr [edi + 1fd4h], 11h
        mov    al, 80h
        or     byte ptr [edi + 1fd5h], al
        mov    al, 1
        or     byte ptr [edi + 1e52h], al
        xor    al, al
        mov    byte ptr [edi + 3e00h], al
        db  00Fh, 0BAh, 025h, 074h, 00Bh, 000h, 000h, 01Fh
        jae    SHORT loc_1_023E
        mov    al, byte ptr [edi + 3c80h]
        db  0A3h, 088h, 00Bh, 000h, 000h
        mov    al, 40h
        mov    byte ptr [edi + 3c80h], al
        mov    eax, 1
        mov    dword ptr [edi + 1e40h], eax
        mov    al, 0
        mov    byte ptr [edi + 3c84h], al
        mov    bl, byte ptr [edi + 3c88h]
        mov    bh, byte ptr [edi + 3c88h]
        mov    al, byte ptr [edi + 3c88h]
        mov    ah, byte ptr [edi + 3c88h]
        shl    eax, 10h
        mov    ax, bx
        or     eax, 81000000h
        mov    al, 0
        mov    byte ptr [edi + 3c84h], al
        mov    byte ptr [edi + 3c88h], al
        mov    byte ptr [edi + 3c88h], ah
        shr    eax, 10h
        mov    byte ptr [edi + 3c88h], al
        mov    byte ptr [edi + 3c88h], ah
        mov    al, 8
        mov    byte ptr [edi + 3c84h], al
        mov    eax, 0
        mov    dword ptr [edi + 1e40h], eax
loc_1_023E:
        db  0A1h, 074h, 00Bh, 000h, 000h
        and    eax, 0ffh
        cmp    eax, 0
        je     NEAR PTR loc_1_0360
        cmp    eax, 2
        je     NEAR PTR loc_1_0334
        cmp    eax, 9
        je     SHORT loc_1_02AD
        mov    byte ptr [edi + 3c18h], 0ffh
        mov    byte ptr [edi + 3c1ch], 0
        mov    byte ptr [edi + 3c18h], 1dh
        mov    byte ptr [edi + 3c1ch], 0
        db  00Fh, 0BAh, 025h, 074h, 00Bh, 000h, 000h, 01Fh
        jae    SHORT loc_1_02A8
        mov    byte ptr [edi + 3c84h], 0ah
        mov    al, byte ptr [edi + 3c88h]
        and    al, 0bh
        mov    byte ptr [edi + 3c84h], 0ah
        mov    byte ptr [edi + 3c88h], al
        mov    byte ptr [edi + 3c84h], 8
loc_1_02A8:
        jmp    NEAR PTR loc_1_0390
loc_1_02AD:
        mov    byte ptr [edi + 3c00h], 2ah
        mov    byte ptr [edi + 3c28h], 1
        mov    byte ptr [edi + 3c00h], 2bh
        mov    byte ptr [edi + 3c28h], 1
        mov    byte ptr [edi + 3c00h], 1eh
        mov    byte ptr [edi + 3c28h], 4
        mov    byte ptr [edi + 3c00h], 1dh
        mov    byte ptr [edi + 3c28h], 0
        mov    byte ptr [edi + 3c00h], 1ah
        mov    byte ptr [edi + 3c28h], 0
        mov    byte ptr [edi + 3c00h], 18h
        mov    byte ptr [edi + 3c28h], 80h
        mov    byte ptr [edi + 3c00h], 19h
        mov    byte ptr [edi + 3c28h], 98h
        mov    byte ptr [edi + 3c00h], 39h
        mov    byte ptr [edi + 3c28h], 18h
        mov    byte ptr [edi + 3c00h], 6
        mov    al, byte ptr [edi + 3c28h]
        and    al, 0fch
        mov    byte ptr [edi + 3c28h], al
        jmp    SHORT loc_1_0390
loc_1_0334:
        mov    al, 80h
        mov    byte ptr [edi + 3c18h], al
        mov    al, 1
        mov    byte ptr [edi + 3c00h], al
        xor    al, al
        mov    byte ptr [edi + 3c20h], al
        mov    byte ptr [edi + 3c24h], al
        mov    byte ptr [edi + 3c28h], al
        mov    byte ptr [edi + 3c18h], al
        jmp    SHORT loc_1_0390
loc_1_0360:
        mov    al, 1
        mov    byte ptr [edi + 3c18h], al
        mov    al, 2
        mov    byte ptr [edi + 3c00h], al
        mov    al, 0
        mov    byte ptr [edi + 3c08h], al
        mov    al, 3
        mov    byte ptr [edi + 3c00h], al
        mov    al, 10h
        mov    byte ptr [edi + 3c08h], al
        mov    al, 0
        mov    byte ptr [edi + 3c18h], al
loc_1_0390:
        push   eax
        mov    eax, 10000h
loc_1_0396:
        dec    eax
        jne    SHORT loc_1_0396
        pop    eax
        mov    al, 0
        mov    byte ptr [edi + 1fc4h], al
        mov    al, 3
        mov    byte ptr [edi + 1fc5h], al
        mov    dword ptr [edi + 1e40h], 1
        push   ecx
        mov    ecx, 32h
loc_1_03BA:
        dec    ecx
        jne    SHORT loc_1_03BA
        pop    ecx
        db  00Fh, 0BAh, 025h, 074h, 00Bh, 000h, 000h, 010h
        jb     SHORT loc_1_0438
        db  00Fh, 0BAh, 025h, 074h, 00Bh, 000h, 000h, 011h
        jae    SHORT loc_1_0438
        db  00Fh, 0BAh, 025h, 074h, 00Bh, 000h, 000h, 017h
        jb     SHORT loc_1_03E6
        mov    dword ptr [edi + 1e40h], 0
loc_1_03E6:
        mov    al, 8dh
        mov    byte ptr [edi + 1e47h], al
        mov    dword ptr [edi + 1e40h], 0
        db  00Fh, 0BAh, 025h, 074h, 00Bh, 000h, 000h, 008h
        jb     SHORT loc_1_0424
        mov    eax, dword ptr [edi + 1e50h]
        or     eax, 10000000h
        or     eax, 400h
        or     eax, 10000000h
        or     eax, 200h
        mov    dword ptr [edi + 1e50h], eax
        jmp    SHORT loc_1_042E
loc_1_0424:
        mov    dword ptr [edi + 1e50h], 11000700h
loc_1_042E:
        xor    al, al
        mov    byte ptr [edi + 1e47h], al
        jmp    SHORT loc_1_0459
loc_1_0438:
        mov    al, 8dh
        mov    byte ptr [edi + 1e47h], al
        mov    eax, dword ptr [edi + 1e50h]
        or     eax, 400h
        mov    dword ptr [edi + 1e50h], eax
        xor    al, al
        mov    byte ptr [edi + 1e47h], al
loc_1_0459:
        db  00Fh, 0BAh, 025h, 074h, 00Bh, 000h, 000h, 01Fh
        jae    SHORT loc_1_04C1
        mov    al, 0
        mov    byte ptr [edi + 3c84h], al
        mov    al, byte ptr [edi + 3c88h]
        mov    ah, byte ptr [edi + 3c88h]
        shl    eax, 10h
        mov    al, byte ptr [edi + 3c88h]
        mov    ah, byte ptr [edi + 3c88h]
        or     ah, 1
        rol    eax, 10h
        mov    byte ptr [edi + 3c84h], 0
        mov    byte ptr [edi + 3c88h], al
        mov    byte ptr [edi + 3c88h], ah
        shr    eax, 10h
        mov    byte ptr [edi + 3c88h], al
        mov    byte ptr [edi + 3c88h], ah
        mov    al, 8
        mov    byte ptr [edi + 3c84h], al
        db  0A1h, 088h, 00Bh, 000h, 000h
        mov    byte ptr [edi + 3c80h], al
loc_1_04C1:
        db  00Fh, 0BAh, 025h, 074h, 00Bh, 000h, 000h, 010h
        jb     SHORT loc_1_0507
        db  00Fh, 0BAh, 025h, 074h, 00Bh, 000h, 000h, 011h
        jae    SHORT loc_1_0507
        db  00Fh, 0BAh, 025h, 074h, 00Bh, 000h, 000h, 008h
        jae    SHORT loc_1_0507
        mov    al, 8dh
        mov    byte ptr [edi + 1e47h], al
        mov    byte ptr [edi + 1e50h], 0
        mov    eax, dword ptr [edi + 1e50h]
        or     eax, 200h
        mov    dword ptr [edi + 1e50h], eax
        xor    al, al
        mov    byte ptr [edi + 1e47h], al
loc_1_0507:
        mov    dword ptr [edi + 1e40h], 0
        db  00Fh, 0BAh, 025h, 074h, 00Bh, 000h, 000h, 017h
        jae    SHORT loc_1_0536
        db  00Fh, 0BAh, 025h, 074h, 00Bh, 000h, 000h, 008h
        jae    SHORT loc_1_052F
        mov    dword ptr [edi + 2010h], 800ac000h
loc_1_052F:
        mov    dx, 46e8h
        mov    al, 8
        out    dx, al
loc_1_0536:
        mov    dx, 3ceh
        mov    al, 6
        out    dx, al
        inc    dx
        mov    al, 5
        out    dx, al
        pop    ebx
        pop    eax
        pop    esi
        pop    edi
        clc
        ret
loc_1_0548:
        pushad
        mov    eax, dword ptr [ebp + 18h]
        cmp    eax, 0ac53962bh
        jne    NEAR PTR loc_1_0641
        db  0A1h, 078h, 00Bh, 000h, 000h
        or     eax, eax
        jne    NEAR PTR loc_1_063F
        mov    ax, word ptr [ebp + 3ch]
        shl    eax, 10h
        mov    ax, word ptr [ebp]
        db  0A3h, 07Ch, 00Bh, 000h, 000h
        movzx  eax, word ptr [ebp + 40h]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        inc    eax
        je     NEAR PTR loc_1_0635
        dec    eax
        movzx  esi, word ptr [ebp + 4]
        add    esi, eax
        mov    al, byte ptr [esi + 1ch]
        and    eax, 0ffh
        db  0A3h, 074h, 00Bh, 000h, 000h
        mov    eax, dword ptr [esi + 4]
        and    eax, 0eh
        cmp    al, 0ah
        jne    SHORT loc_1_05E4
        db  00Fh, 0BAh, 02Dh, 074h, 00Bh, 000h, 000h, 011h
        push   0
        push   1000h
        push   0c0000h
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        inc    eax
        je     SHORT loc_1_05E4
        dec    eax
        add    eax, 78h
        mov    eax, dword ptr [eax]
        and    eax, 0ffffffh
        cmp    eax, 42565fh
        jne    SHORT loc_1_05E4
        db  00Fh, 0BAh, 02Dh, 074h, 00Bh, 000h, 000h, 017h
loc_1_05E4:
        mov    eax, dword ptr [esi]
        cmp    eax, 0ac000h
        jne    SHORT loc_1_05F5
        db  00Fh, 0BAh, 02Dh, 074h, 00Bh, 000h, 000h, 008h
loc_1_05F5:
        push   0
        push   4000h
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        inc    eax
        je     SHORT loc_1_0635
        dec    eax
        db  0A3h, 078h, 00Bh, 000h, 000h
        mov    esi, eax
        mov    eax, dword ptr [esi + 1e50h]
        bt     eax, 4
        jb     SHORT loc_1_0625
        db  00Fh, 0BAh, 02Dh, 074h, 00Bh, 000h, 000h, 01Fh
loc_1_0625:
        bt     eax, 1bh
        jae    SHORT loc_1_0633
        db  00Fh, 0BAh, 02Dh, 074h, 00Bh, 000h, 000h, 010h
loc_1_0633:
        jmp    SHORT loc_1_063F
loc_1_0635:
        db  0C7h, 005h, 078h, 00Bh, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_063F:
        jmp    SHORT loc_1_0659
loc_1_0641:
        push   0
        push   4000h
        push   0ac000h
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        mov    dword ptr [ebp + 14h], eax
loc_1_0659:
        popad
        ret
loc_1_065B:
        call   MGA_SYSTEM_EXIT
        ret
loc_1_0661:
        call   MGA_SYSTEM_EXIT
        ret
loc_1_0667:
        pushad
        mov    eax, dword ptr [edx]
        mov    esi, dword ptr [edx + 8]
        and    esi, 0ffffh
        mov    edi, dword ptr [edx + 4]
        and    edi, 0ffffh
        mov    edx, eax
        shr    eax, 10h
        push   esi
        push   edi
        push   edx
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        pop    edi
        pop    esi
        inc    eax
        je     SHORT loc_1_06A9
        dec    eax
        and    edx, 0ffffh
        add    edx, eax
        mov    byte ptr [edx], 0
        add    esi, eax
        add    edi, eax
        xor    dl, dl
loc_1_06A9:
        popad
        ret
sub_1_06AB:
        push   edx
        push   eax
        push   ebx
        mov    dx, 3c4h
        in     al, dx
        jmp    SHORT loc_1_06B5
loc_1_06B5:
        mov    ah, al
        xor    al, 7
        out    dx, al
        jmp    SHORT loc_1_06BC
loc_1_06BC:
        mov    bl, al
        in     al, dx
        jmp    SHORT loc_1_06C1
loc_1_06C1:
        cmp    al, bl
        pushfd
        mov    al, ah
        popfd
        out    dx, al
        pop    ebx
        pop    eax
        pop    edx
        ret
loc_1_06CC:
        pushad
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ax, 4f10h
        jne    SHORT loc_1_06EE
        mov    ax, word ptr [ebp + 1ch]
        mov    bx, word ptr [ebp + 10h]
        call   sub_1_06F4
        mov    word ptr [ebp + 1ch], ax
        mov    word ptr [ebp + 10h], bx
        popad
        stc
        ret
loc_1_06EE:
        popad
        clc
        ret
        db  000h, 000h, 000h
sub_1_06F4:
        cmp    bl, 0
        jne    SHORT loc_1_0700
        call   sub_1_071D
        jmp    SHORT loc_1_071C
loc_1_0700:
        cmp    bl, 1
        jne    SHORT loc_1_070C
        call   sub_1_07AD
        jmp    SHORT loc_1_071C
loc_1_070C:
        cmp    bl, 2
        jne    SHORT loc_1_0718
        call   sub_1_0726
        jmp    SHORT loc_1_071C
loc_1_0718:
        mov    ax, 0
loc_1_071C:
        ret
sub_1_071D:
        mov    ax, 4fh
        mov    bh, 6
        mov    bl, 10h
        ret
sub_1_0726:
        push   edx
        call   sub_1_0732
        pop    edx
        mov    ax, 4fh
        ret
sub_1_0732:
        push   edi
        call   sub_1_06AB
        je     SHORT loc_1_0785
        db  08Bh, 03Dh, 078h, 00Bh, 000h, 000h
        and    edi, 0fffffh
        cmp    edi, 0ac000h
        jne    SHORT loc_1_074E
loc_1_074E:
        db  08Bh, 03Dh, 078h, 00Bh, 000h, 000h
        mov    eax, dword ptr [edi + 1e50h]
        and    eax, 400h
        jne    SHORT loc_1_0785
        mov    byte ptr [edi + 1fd4h], 0
        mov    al, byte ptr [edi + 1fd5h]
        or     al, al
        je     SHORT loc_1_07A9
        mov    byte ptr [edi + 1fd4h], 6
        mov    al, byte ptr [edi + 1fd5h]
        or     al, al
        je     SHORT loc_1_07A5
        jmp    SHORT loc_1_07A1
loc_1_0785:
        mov    dx, 3d4h
        mov    al, 0
        out    dx, al
        inc    dx
        in     al, dx
        or     al, al
        je     SHORT loc_1_07A9
        dec    dx
        mov    al, 6
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
        or     al, al
        je     SHORT loc_1_07A5
loc_1_07A1:
        mov    bh, 0
        pop    edi
        ret
loc_1_07A5:
        mov    bh, 2
        pop    edi
        ret
loc_1_07A9:
        mov    bh, 4
        pop    edi
        ret
sub_1_07AD:
        push   edi
        db  0A0h, 094h, 00Bh, 000h, 000h
        or     al, al
        jne    SHORT loc_1_07BC
        call   sub_1_0812
loc_1_07BC:
        mov    al, bh
        or     al, al
        je     SHORT loc_1_07C6
        and    al, 6
        je     SHORT loc_1_080C
loc_1_07C6:
        db  088h, 03Dh, 094h, 00Bh, 000h, 000h
        cmp    bh, 0
        jne    SHORT loc_1_07DC
        call   sub_1_0899
        mov    ax, 4fh
        jmp    SHORT loc_1_0810
loc_1_07DC:
        cmp    bh, 1
        jne    SHORT loc_1_07EC
        call   sub_1_0B21
        mov    ax, 4fh
        jmp    SHORT loc_1_0810
loc_1_07EC:
        cmp    bh, 2
        jne    SHORT loc_1_07FC
        call   sub_1_097D
        mov    ax, 4fh
        jmp    SHORT loc_1_0810
loc_1_07FC:
        cmp    bh, 4
        jne    SHORT loc_1_080C
        call   sub_1_0A5B
        mov    ax, 4fh
        jmp    SHORT loc_1_0810
loc_1_080C:
        mov    ax, 0
loc_1_0810:
        pop    edi
        ret
sub_1_0812:
        push   edi
        call   sub_1_06AB
        je     SHORT loc_1_086C
        db  08Bh, 03Dh, 078h, 00Bh, 000h, 000h
        and    edi, 0fffffh
        cmp    edi, 0ac000h
        jne    SHORT loc_1_082E
loc_1_082E:
        db  08Bh, 03Dh, 078h, 00Bh, 000h, 000h
        mov    byte ptr [edi + 1fd4h], 0
        mov    al, byte ptr [edi + 1fd5h]
        db  0A2h, 095h, 00Bh, 000h, 000h
        mov    byte ptr [edi + 1fd4h], 1
        mov    al, byte ptr [edi + 1fd5h]
        db  0A2h, 096h, 00Bh, 000h, 000h
        mov    byte ptr [edi + 1fd4h], 6
        mov    al, byte ptr [edi + 1fd5h]
        db  0A2h, 097h, 00Bh, 000h, 000h
        pop    edi
        ret
loc_1_086C:
        mov    dx, 3d4h
        mov    al, 0
        out    dx, al
        inc    dx
        in     al, dx
        db  0A2h, 095h, 00Bh, 000h, 000h
        dec    dx
        mov    al, 1
        out    dx, al
        inc    dx
        in     al, dx
        db  0A2h, 096h, 00Bh, 000h, 000h
        dec    dx
        mov    al, 6
        out    dx, al
        inc    dx
        in     al, dx
        db  0A2h, 097h, 00Bh, 000h, 000h
        dec    dx
        pop    edi
        ret
sub_1_0899:
        push   edi
        call   sub_1_06AB
        je     NEAR PTR loc_1_0929
        db  08Bh, 03Dh, 078h, 00Bh, 000h, 000h
        and    edi, 0fffffh
        cmp    edi, 0ac000h
        jne    SHORT loc_1_08B9
loc_1_08B9:
        db  08Bh, 03Dh, 078h, 00Bh, 000h, 000h
        mov    byte ptr [edi + 1fd4h], 11h
        mov    al, byte ptr [edi + 1fd5h]
        and    al, 7fh
        mov    byte ptr [edi + 1fd5h], al
        mov    byte ptr [edi + 1fd4h], 0
        db  0A0h, 095h, 00Bh, 000h, 000h
        mov    byte ptr [edi + 1fd5h], al
        mov    byte ptr [edi + 1fd4h], 1
        db  0A0h, 096h, 00Bh, 000h, 000h
        mov    byte ptr [edi + 1fd5h], al
        mov    byte ptr [edi + 1fd4h], 6
        db  0A0h, 097h, 00Bh, 000h, 000h
        mov    byte ptr [edi + 1fd5h], al
        mov    byte ptr [edi + 1fd4h], 11h
        mov    al, 80h
        or     byte ptr [edi + 1fd5h], al
        mov    byte ptr [edi + 1fc4h], 1
        and    byte ptr [edi + 1fc5h], 0dfh
        jmp    SHORT loc_1_097B
loc_1_0929:
        mov    dx, 3d4h
        mov    al, 11h
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 7fh
        out    dx, al
        mov    dx, 3d4h
        mov    al, 0
        out    dx, al
        inc    dx
        db  0A0h, 095h, 00Bh, 000h, 000h
        out    dx, al
        dec    dx
        mov    al, 1
        out    dx, al
        inc    dx
        db  0A0h, 096h, 00Bh, 000h, 000h
        out    dx, al
        dec    dx
        mov    al, 6
        out    dx, al
        inc    dx
        db  0A0h, 097h, 00Bh, 000h, 000h
        out    dx, al
        dec    dx
        mov    dx, 3d4h
        mov    al, 11h
        out    dx, al
        inc    dx
        in     al, dx
        or     al, 80h
        out    dx, al
        mov    dx, 3c4h
        mov    al, 1
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0dfh
        out    dx, al
loc_1_097B:
        pop    edi
        ret
sub_1_097D:
        push   edi
        call   sub_1_06AB
        je     NEAR PTR loc_1_0A0A
        db  08Bh, 03Dh, 078h, 00Bh, 000h, 000h
        and    edi, 0fffffh
        cmp    edi, 0ac000h
        jne    SHORT loc_1_099D
loc_1_099D:
        db  08Bh, 03Dh, 078h, 00Bh, 000h, 000h
        mov    byte ptr [edi + 1fc4h], 1
        or     byte ptr [edi + 1fc5h], 20h
        mov    byte ptr [edi + 1fd4h], 11h
        mov    al, byte ptr [edi + 1fd5h]
        and    al, 7fh
        mov    byte ptr [edi + 1fd5h], al
        mov    byte ptr [edi + 1fd4h], 0
        db  0A0h, 095h, 00Bh, 000h, 000h
        mov    byte ptr [edi + 1fd5h], al
        mov    byte ptr [edi + 1fd4h], 1
        db  0A0h, 096h, 00Bh, 000h, 000h
        mov    byte ptr [edi + 1fd5h], al
        mov    byte ptr [edi + 1fd4h], 6
        xor    al, al
        mov    byte ptr [edi + 1fd5h], al
        mov    byte ptr [edi + 1fd4h], 11h
        mov    al, 80h
        or     byte ptr [edi + 1fd5h], al
        jmp    SHORT loc_1_0A59
loc_1_0A0A:
        mov    dx, 3c4h
        mov    al, 1
        out    dx, al
        inc    dx
        in     al, dx
        or     al, 20h
        out    dx, al
        mov    dx, 3d4h
        mov    al, 11h
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 7fh
        out    dx, al
        mov    dx, 3d4h
        mov    al, 0
        out    dx, al
        inc    dx
        db  0A0h, 095h, 00Bh, 000h, 000h
        out    dx, al
        dec    dx
        mov    al, 1
        out    dx, al
        inc    dx
        db  0A0h, 096h, 00Bh, 000h, 000h
        out    dx, al
        dec    dx
        mov    al, 6
        out    dx, al
        inc    dx
        mov    al, 0
        out    dx, al
        dec    dx
        mov    dx, 3d4h
        mov    al, 11h
        out    dx, al
        inc    dx
        in     al, dx
        or     al, 80h
        out    dx, al
loc_1_0A59:
        pop    edi
        ret
sub_1_0A5B:
        push   edi
        call   sub_1_06AB
        je     SHORT loc_1_0ADA
        db  08Bh, 03Dh, 078h, 00Bh, 000h, 000h
        and    edi, 0fffffh
        cmp    edi, 0ac000h
        jne    SHORT loc_1_0A77
loc_1_0A77:
        db  08Bh, 03Dh, 078h, 00Bh, 000h, 000h
        mov    byte ptr [edi + 1fc4h], 1
        or     byte ptr [edi + 1fc5h], 20h
        mov    byte ptr [edi + 1fd4h], 11h
        mov    al, byte ptr [edi + 1fd5h]
        and    al, 7fh
        mov    byte ptr [edi + 1fd5h], al
        xor    al, al
        mov    byte ptr [edi + 1fd4h], 1
        mov    byte ptr [edi + 1fd5h], al
        mov    byte ptr [edi + 1fd4h], 0
        mov    byte ptr [edi + 1fd5h], al
        mov    byte ptr [edi + 1fd4h], 6
        mov    byte ptr [edi + 1fd5h], al
        mov    byte ptr [edi + 1fd4h], 11h
        mov    al, 80h
        or     byte ptr [edi + 1fd5h], al
        jmp    SHORT loc_1_0B1F
loc_1_0ADA:
        mov    dx, 3c4h
        mov    al, 1
        out    dx, al
        inc    dx
        in     al, dx
        or     al, 20h
        out    dx, al
        mov    dx, 3d4h
        mov    al, 11h
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 7fh
        out    dx, al
        mov    dx, 3d4h
        mov    al, 1
        out    dx, al
        inc    dx
        xor    al, al
        out    dx, al
        dec    dx
        out    dx, al
        inc    dx
        out    dx, al
        dec    dx
        mov    al, 6
        out    dx, al
        inc    dx
        xor    al, al
        out    dx, al
        dec    dx
        mov    dx, 3d4h
        mov    al, 11h
        out    dx, al
        inc    dx
        in     al, dx
        or     al, 80h
        out    dx, al
loc_1_0B1F:
        pop    edi
        ret
sub_1_0B21:
        ret
        db  000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 001h, 000h, 000h, 04Dh, 047h, 041h, 020h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 028h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
_LTEXT ends
        end
