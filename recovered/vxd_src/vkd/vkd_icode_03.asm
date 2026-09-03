; vkd_icode_03.asm -- VKD object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  0B9h, 000h, 000h, 001h, 000h, 0E4h, 064h, 0A8h, 002h, 0E0h, 0FAh, 0C3h
sub_3_000C:
        mov    ecx, 10000h
loc_3_0011:
        in     al, 64h
        test   al, 1
        loope  loc_3_0011
        ret
VKD_SYS_CRITICAL_INIT:
        db  089h, 01Dh, 029h, 012h, 000h, 000h
        push   0
        push   0fah
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    SHORT loc_3_0037
loc_3_0031:
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_3_0037:
        db  0A3h, 010h, 012h, 000h, 000h
        xor    eax, eax
        inc    eax
        push   3
        pop    ecx
        xor    esi, esi
        db  0BFh, 0F8h, 007h, 000h, 000h
        db  0CDh, 020h, 0ADh, 000h, 001h, 000h
        db  0A3h, 024h, 012h, 000h, 000h
        mov    eax, 1f4h
        push   3
        pop    ecx
        db  0BFh, 0A1h, 008h, 000h, 000h
        db  0CDh, 020h, 0ADh, 000h, 001h, 000h
        db  0A3h, 055h, 012h, 000h, 000h
        or     eax, 0ffffffffh
        db  0BFh, 096h, 008h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        db  0A2h, 05Ah, 012h, 000h, 000h
        db  08Bh, 035h, 010h, 012h, 000h, 000h
        add    esi, ebx
        db  0E8h, 074h, 0FFh, 0FCh, 0FFh
        db  083h, 03Dh, 031h, 012h, 000h, 000h, 000h
        jne    SHORT loc_3_00AB
        mov    eax, 10038h
        db  0BEh, 035h, 008h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 031h, 012h, 000h, 000h
loc_3_00AB:
        xor    eax, eax
        push   18h
        pop    ecx
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     NEAR PTR loc_3_0031
        db  089h, 035h, 008h, 013h, 000h, 000h
        xor    eax, eax
        push   0ch
        pop    ecx
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     NEAR PTR loc_3_0031
        db  089h, 035h, 00Ch, 013h, 000h, 000h
        db  0BFh, 074h, 007h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     NEAR PTR loc_3_01B6
        db  0A3h, 00Ch, 012h, 000h, 000h
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   cl, 8
        je     SHORT loc_3_010C
        db  0CDh, 020h, 004h, 000h, 003h, 000h
loc_3_010C:
        call   0
        jne    SHORT loc_3_0140
        mov    al, 0adh
        out    64h, al
        jmp    SHORT loc_3_0119
loc_3_0119:
        in     al, 64h
        test   al, 1
        je     SHORT loc_3_0125
        in     al, 60h
        jmp    SHORT loc_3_0123
loc_3_0123:
        jmp    SHORT loc_3_0119
loc_3_0125:
        call   0
        jne    SHORT loc_3_0140
        mov    al, 20h
        out    64h, al
        jmp    SHORT loc_3_0132
loc_3_0132:
        call   0
        jne    SHORT loc_3_0140
        call   sub_3_000C
        jne    SHORT loc_3_0144
loc_3_0140:
        mov    al, 45h
        jmp    SHORT loc_3_015E
loc_3_0144:
        in     al, 60h
        jmp    SHORT loc_3_0148
loc_3_0148:
        jmp    SHORT loc_3_014A
loc_3_014A:
        mov    ah, al
        in     al, 64h
        jmp    SHORT loc_3_0150
loc_3_0150:
        jmp    SHORT loc_3_0152
loc_3_0152:
        in     al, 60h
        jmp    SHORT loc_3_0156
loc_3_0156:
        jmp    SHORT loc_3_0158
loc_3_0158:
        mov    al, ah
        or     al, 1
        and    al, 0efh
loc_3_015E:
        db  0A2h, 059h, 012h, 000h, 000h
        call   0
        mov    al, 0aeh
        out    64h, al
        db  089h, 01Dh, 014h, 012h, 000h, 000h
        db  08Bh, 035h, 010h, 012h, 000h, 000h
        add    esi, ebx
        db  0E8h, 0A3h, 007h, 0FDh, 0FFh
        db  0A3h, 02Dh, 012h, 000h, 000h
        mov    word ptr [esi + 36h], ax
        or     dword ptr [esi], 100000h
        and    ax, 70h
        mov    byte ptr [esi + 41h], al
        db  0A0h, 059h, 012h, 000h, 000h
        mov    byte ptr [esi + 3ch], al
        db  0A1h, 00Ch, 012h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        call   sub_3_0470
        db  080h, 00Dh, 051h, 012h, 000h, 000h, 010h
        clc
        ret
loc_3_01B6:
        db  00Fh, 0B7h, 035h, 00Ch, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
VKD_DEVICE_INIT:
        call   sub_3_051C
        jb     NEAR PTR loc_3_0031
        push   0
        db  068h, 094h, 007h, 000h, 000h
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_3_0031
        push   0
        db  068h, 0A8h, 007h, 000h, 000h
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_3_0031
        push   0
        db  068h, 0BCh, 007h, 000h, 000h
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_3_0031
        push   0
        db  068h, 0D0h, 007h, 000h, 000h
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_3_0031
        db  0E8h, 06Bh, 004h, 0FDh, 0FFh
        db  0E8h, 067h, 00Dh, 0FDh, 0FFh
        db  0E8h, 057h, 004h, 0FDh, 0FFh
        sub    esp, 6ch
        mov    edi, esp
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        db  0CDh, 020h, 083h, 000h, 001h, 000h
loc_3_0252:
        mov    byte ptr [ebp + 1dh], 1
        xor    eax, eax
        mov    al, 16h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 6
        jb     SHORT loc_3_0278
        mov    byte ptr [ebp + 1dh], 0
        xor    eax, eax
        mov    al, 16h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        jmp    SHORT loc_3_0252
loc_3_0278:
        mov    dword ptr [ebp + 1ch], 1607h
        mov    dword ptr [ebp + 10h], 0dh
        xor    eax, eax
        mov    dword ptr [ebp + 18h], eax
        mov    dword ptr [ebp + 14h], eax
        mov    dword ptr [ebp], eax
        mov    dword ptr [ebp + 4], eax
        mov    word ptr [ebp + 38h], ax
        xor    eax, eax
        mov    al, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 38h]
        shl    eax, 4
        je     SHORT loc_3_02F6
        movzx  ecx, word ptr [ebp]
        cmp    dword ptr [ecx + eax], 0eff2e9ch
        jne    SHORT loc_3_02F6
        cmp    dword ptr [ecx + eax + 6], 9d900175h
        jne    SHORT loc_3_02F6
        movzx  edx, word ptr [ecx + eax + 4]
        add    edx, eax
        db  089h, 015h, 01Ch, 012h, 000h, 000h
        db  089h, 015h, 0ECh, 007h, 000h, 000h
        shl    eax, 0ch
        add    ecx, 8
        mov    ax, cx
        db  0A3h, 018h, 012h, 000h, 000h
        db  0BEh, 0D4h, 003h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        jae    SHORT loc_3_030B
loc_3_02F0:
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_3_02F6:
        db  00Fh, 0B7h, 035h, 00Eh, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_3_030B:
        push   0
        db  068h, 0E4h, 007h, 000h, 000h
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        je     SHORT loc_3_02F0
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    esi, esp
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        add    esp, 6ch
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  08Bh, 035h, 010h, 012h, 000h, 000h
        add    esi, ebx
        mov    edx, esi
        push   ebx
        db  08Bh, 01Dh, 02Dh, 012h, 000h, 000h
        and    ebx, 70h
        db  089h, 01Dh, 02Dh, 012h, 000h, 000h
        mov    ax, word ptr [esi + 36h]
        db  0E8h, 0FDh, 002h, 0FDh, 0FFh
        pop    ebx
        je     SHORT loc_3_0360
        db  0E8h, 050h, 0FEh, 0FCh, 0FFh
loc_3_0360:
        mov    al, 1
        db  0CDh, 020h, 009h, 000h, 00Dh, 000h
        mov    eax, 15h
        db  0BEh, 06Ah, 000h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        clc
        ret
VKD_INIT_COMPLETE:
        db  0CDh, 020h, 000h, 000h, 017h, 000h
        jb     SHORT loc_3_03B3
        cmp    eax, 314h
        jb     SHORT loc_3_03B3
        db  0FEh, 005h, 020h, 012h, 000h, 000h
        mov    eax, 5
        sub    edx, edx
        db  0BEh, 09Bh, 000h, 000h, 000h
        db  0CDh, 020h, 00Ah, 000h, 017h, 000h
        mov    eax, 2
        xor    edx, edx
        db  0BEh, 00Ah, 004h, 000h, 000h
        db  0CDh, 020h, 00Ah, 000h, 017h, 000h
loc_3_03B3:
        db  0CDh, 020h, 000h, 000h, 018h, 000h
        jb     SHORT loc_3_03D5
        cmp    eax, 400h
        jb     SHORT loc_3_03D5
        db  066h, 0C7h, 005h, 0F7h, 003h, 000h, 000h, 0CDh, 020h
        db  0C7h, 005h, 0F9h, 003h, 000h, 000h, 003h, 000h, 018h, 000h
loc_3_03D5:
        clc
        ret
        db  000h
sub_3_03D8:
        cmp    al, 61h
        jb     SHORT loc_3_03E2
        cmp    al, 7ah
        ja     SHORT loc_3_03E2
        sub    al, 20h
loc_3_03E2:
        ret
sub_3_03E3:
        cld
loc_3_03E4:
        lodsb
        or     al, al
        je     SHORT loc_3_03EF
        cmp    al, 20h
        jbe    SHORT loc_3_03E4
        or     al, al
loc_3_03EF:
        ret
sub_3_03F0:
        pushad
        or     cl, 0c0h
        cmp    byte ptr [edx], 0
        je     SHORT loc_3_046E
        db  0CDh, 020h, 0B0h, 000h, 001h, 000h
        cmp    eax, 100h
        jae    SHORT loc_3_046E
        mov    ebx, eax
        mov    esi, edx
        call   sub_3_03E3
        je     SHORT loc_3_0468
        cmp    al, 2ch
        jne    SHORT loc_3_046E
        call   sub_3_03E3
        je     SHORT loc_3_0468
        cmp    al, 2dh
        jne    SHORT loc_3_0425
        or     cl, 1fh
        jmp    SHORT loc_3_043B
loc_3_0425:
        cmp    al, 2ch
        je     SHORT loc_3_0446
        mov    edx, esi
        dec    edx
        db  0CDh, 020h, 0ACh, 000h, 001h, 000h
        cmp    eax, 1fh
        jae    SHORT loc_3_046E
        or     cl, al
        mov    esi, edx
loc_3_043B:
        call   sub_3_03E3
        je     SHORT loc_3_0468
        cmp    al, 2ch
        jne    SHORT loc_3_046E
loc_3_0446:
        call   sub_3_03E3
        je     SHORT loc_3_0468
        call   sub_3_03D8
        cmp    al, 46h
        je     SHORT loc_3_0462
        cmp    al, 4eh
        jne    SHORT loc_3_0468
        and    cl, 3fh
        or     cl, 40h
        jmp    SHORT loc_3_0468
loc_3_0462:
        and    cl, 3fh
        or     cl, 80h
loc_3_0468:
        db  088h, 08Bh, 01Ch, 013h, 000h, 000h
loc_3_046E:
        popad
        ret
sub_3_0470:
        mov    edx, 64h
        db  0BEh, 0C4h, 00Ah, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 60h
        db  0BEh, 0D8h, 00Bh, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h
        xor    eax, eax
        test   ebx, 40000h
        je     SHORT loc_3_04A1
        dec    eax
loc_3_04A1:
        xor    esi, esi
        db  0BFh, 072h, 008h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        or     eax, eax
        je     SHORT loc_3_04DC
        db  0C6h, 005h, 0C0h, 013h, 000h, 000h, 001h
        db  080h, 00Dh, 0C0h, 013h, 000h, 000h, 0C0h
        db  0C6h, 005h, 0C1h, 013h, 000h, 000h, 01Fh
        db  080h, 00Dh, 0C1h, 013h, 000h, 000h, 0A0h
        db  0C6h, 005h, 0C2h, 013h, 000h, 000h, 000h
        db  080h, 00Dh, 0C2h, 013h, 000h, 000h, 0A0h
loc_3_04DC:
        db  0BFh, 07Dh, 008h, 000h, 000h
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
loc_3_04E7:
        jb     SHORT loc_3_04FB
        mov    ecx, 0
        call   sub_3_03F0
        db  0CDh, 020h, 0B4h, 000h, 001h, 000h
        jmp    SHORT loc_3_04E7
loc_3_04FB:
        db  0BFh, 089h, 008h, 000h, 000h
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
loc_3_0506:
        jb     SHORT loc_3_051A
        mov    ecx, 20h
        call   sub_3_03F0
        db  0CDh, 020h, 0B4h, 000h, 001h, 000h
        jmp    SHORT loc_3_0506
loc_3_051A:
        ret
        db  000h
sub_3_051C:
        pushad
        db  0CDh, 020h, 000h, 000h, 00Ch, 000h
        cmp    eax, 400h
        jae    NEAR PTR loc_3_0767
        db  089h, 01Dh, 0B4h, 014h, 000h, 000h
        db  0B8h, 0F4h, 000h, 000h, 000h
        db  0B9h, 04Bh, 004h, 000h, 000h
        shr    eax, 0ch
        shr    ecx, 0ch
        sub    ecx, eax
        inc    ecx
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        db  0E8h, 0A7h, 0FAh, 0FCh, 0FFh
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    eax, 11h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        test   byte ptr [ebp + 1ch], 4
        je     NEAR PTR loc_3_06BD
        mov    word ptr [ebp + 1ch], 0c205h
        mov    byte ptr [ebp + 11h], 3
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     NEAR PTR loc_3_06BD
        mov    word ptr [ebp + 1ch], 0c201h
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     NEAR PTR loc_3_06BD
        mov    word ptr [ebp + 1ch], 0c202h
        mov    byte ptr [ebp + 11h], 0
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     NEAR PTR loc_3_06BD
        mov    word ptr [ebp + 1ch], 0c204h
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     NEAR PTR loc_3_06BD
        mov    ax, word ptr [ebp + 10h]
        db  066h, 0A3h, 008h, 015h, 000h, 000h
        mov    word ptr [ebp + 1ch], 0c203h
        mov    byte ptr [ebp + 11h], 3
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     NEAR PTR loc_3_06BD
        db  0BEh, 0A8h, 003h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 0c207h
        mov    word ptr [ebp + 10h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     SHORT loc_3_06BD
        mov    word ptr [ebp + 1ch], 0c206h
        mov    byte ptr [ebp + 11h], 1
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     SHORT loc_3_06BD
        mov    word ptr [ebp + 1ch], 0c202h
        mov    byte ptr [ebp + 11h], 2
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     SHORT loc_3_06BD
        mov    word ptr [ebp + 1ch], 0c200h
        mov    byte ptr [ebp + 11h], 1
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     SHORT loc_3_06BD
        db  0C6h, 005h, 011h, 015h, 000h, 000h, 0FFh
loc_3_06BD:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0E8h, 065h, 0F9h, 0FCh, 0FFh
        mov    eax, 15h
        db  0BEh, 058h, 001h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        db  080h, 03Dh, 011h, 015h, 000h, 000h, 0FFh
        jne    SHORT loc_3_0767
        db  0BFh, 0B0h, 008h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     SHORT loc_3_0771
        db  0A3h, 0FEh, 014h, 000h, 000h
        mov    eax, 15h
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        db  089h, 00Dh, 0ACh, 014h, 000h, 000h
        db  089h, 015h, 0B0h, 014h, 000h, 000h
        db  0BEh, 035h, 001h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 10h
        mov    eax, 15h
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        push   0
        push   7
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        db  0A3h, 00Ch, 015h, 000h, 000h
        db  0E8h, 0A3h, 0F9h, 0FCh, 0FFh
        db  0CDh, 020h, 000h, 000h, 00Ch, 000h
        test   eax, eax
        je     SHORT loc_3_0767
        mov    ecx, 4
        mov    eax, 0ch
        db  0CDh, 020h, 001h, 000h, 00Ch, 000h
loc_3_0767:
        db  0C6h, 005h, 012h, 015h, 000h, 000h, 0FFh
        popad
        clc
        ret
loc_3_0771:
        popad
        stc
        ret
loc_3_0774:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    al, 0eh
        add    byte ptr [eax], al
        mov    al, 7
        add    byte ptr [eax], al
        add    byte ptr [edi], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0CCh
        pop    es
        add    byte ptr [eax], al
        hlt
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [29000004h], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4], dh
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
loc_3_07BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], 0
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
loc_3_07D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xchg   esi, eax
        add    al, 0
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        db  000h
loc_3_07F8:
        dec    ebx
        jns    NEAR PTR loc_3_083E
        outsd
        outsd
        jae    SHORT loc_3_0874
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 746c4100h
        dec    ebx
        jns    NEAR PTR loc_3_0850
        insb
        popad
        jns    SHORT loc_3_0811
loc_3_0811:
        dec    ebx
        db  065h, 079h, 050h
        popad
        db  073h, 074h
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 74756fh
        dec    ebx
        jns    NEAR PTR loc_3_0875
        popad
        jae    SHORT loc_3_089C
        push   ebx
        imul   ebp, dword ptr [ecx + 70h], 43h
        outsd
        jne    SHORT loc_3_089F
        je     SHORT loc_3_0833
loc_3_0833:
        dec    ebx
        jns    NEAR PTR loc_3_0887
        popad
        db  073h, 074h
        inc    ebx
        push   edx
        push   ebx
loc_3_083E:
        imul   ebp, dword ptr [ecx + 70h], 43h
        outsd
        db  075h, 06Eh
        je     SHORT loc_3_0847
loc_3_0847:
        dec    ebx
        jns    NEAR PTR loc_3_089B
        popad
        jae    SHORT loc_3_08C2
        inc    esp
loc_3_0850:
        insb
        popad
        jns    SHORT loc_3_0855
loc_3_0855:
        inc    ecx
        insb
        je     SHORT loc_3_08A9
        popad
        db  073h, 074h
        inc    esp
        insb
        popad
        jns    SHORT loc_3_0863
loc_3_0863:
        dec    ebx
        jns    NEAR PTR loc_3_08A9
        db  075h, 066h
        db  066h, 065h, 072h, 044h
        insb
        popad
        jns    SHORT loc_3_0872
loc_3_0872:
        dec    ebx
        pop    ecx
loc_3_0874:
        inc    edx
loc_3_0875:
        inc    esp
        push   eax
        inc    ecx
        push   ebx
        push   ebx
        push   edi
        inc    esp
        db  000h
loc_3_087D:
        cmp    byte ptr [eax], dh
        xor    al, 32h
        push   edx
        inc    ebp
        inc    ecx
        inc    esp
        inc    ebx
        dec    ebp
loc_3_0887:
        inc    esp
        add    byte ptr [eax], bh
        xor    byte ptr [edx + esi], dh
        push   edi
        push   edx
        dec    ecx
        push   esp
        inc    ebp
        inc    ebx
        dec    ebp
        inc    esp
        add    byte ptr [ebx + 59h], cl
        inc    edx
        inc    esp
        push   edx
loc_3_089B:
        inc    ebp
loc_3_089C:
        inc    edx
        dec    edi
        dec    edi
loc_3_089F:
        push   esp
        add    byte ptr [ebx + 45h], cl
        pop    ecx
        dec    ecx
        inc    esp
        dec    esp
        inc    ebp
        inc    esp
loc_3_08A9:
        inc    ebp
        dec    esp
        inc    ecx
        pop    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], cl
        add    al, byte ptr [eax]
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_08C2:
        add    byte ptr [eax], al
        pushfd
        add    eax, dword ptr [eax]
        add    byte ptr [eax + eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_ITEXT ends
        end
