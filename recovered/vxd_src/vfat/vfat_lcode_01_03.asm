; vfat_lcode_01_03.asm -- VFAT object 1 segment _LTEXT  (part 3/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_9EB2:
        test   word ptr [ebp + 38h], 80h
        je     SHORT loc_1_9ED7
        mov    esi, dword ptr [ebp + 50h]
        mov    eax, dword ptr [ebp + 54h]
        push   40000000h
        push   eax
        push   esi
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        cld
        xor    word ptr [ebp + 38h], 80h
loc_1_9ED7:
        ret
sub_1_9ED8:
        mov    ax, 2020h
        mov    ecx, 2
        rep movsd
        mov    cl, 8
loc_1_9EE5:
        dec    edi
        cmp    byte ptr [edi], al
        loope  loc_1_9EE5
        inc    edi
        cmp    word ptr [esi], ax
        jne    SHORT loc_1_9EF5
        cmp    byte ptr [esi + 2], al
        je     SHORT loc_1_9F01
loc_1_9EF5:
        mov    al, 2eh
        stosb
        movsb
        movsb
        movsb
loc_1_9EFB:
        dec    edi
        cmp    byte ptr [edi], ah
        je     SHORT loc_1_9EFB
        inc    edi
loc_1_9F01:
        xor    eax, eax
        stosb
        ret
sub_1_9F05:
        call   sub_1_9DF0
        ror    eax, 10h
        mov    edx, esi
        mov    esi, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 28h]
        test   word ptr [ecx + 52h], 100h
        jne    SHORT loc_1_9F3D
        test   word ptr [ebp + 18h], 1
        jne    SHORT loc_1_9F3D
        test   dword ptr [edx + 0ch], 200004h
        jne    SHORT loc_1_9F3D
        cmp    ax, word ptr [esi + 3ch]
        je     SHORT loc_1_9F3D
        mov    word ptr [esi + 3ch], ax
        and    word ptr [edx + 0eh], 0fffbh
loc_1_9F3D:
        test   word ptr [edx + 0eh], 40h
        jne    SHORT loc_1_9F5B
        ror    eax, 10h
        cmp    eax, dword ptr [esi + 3eh]
        jne    SHORT loc_1_9F53
        cmp    byte ptr [esi + 42h], 0
        jne    SHORT loc_1_9F5B
loc_1_9F53:
        mov    dword ptr [esi + 38h], eax
        and    word ptr [edx + 0eh], 0fffbh
loc_1_9F5B:
        ret
sub_1_9F5C:
        cmp    dword ptr [edi + 2ch], 0
        jne    SHORT loc_1_9F84
        mov    eax, dword ptr [edi + 8]
        mov    ebx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ebx
        mov    dword ptr [ebx + 8], eax
        db  083h, 03Dh, 070h, 0BDh, 000h, 000h, 000h
        jne    SHORT loc_1_9F83
        call   sub_1_9F85
        xchg   edi, eax
        call   sub_1_A1F8
        xchg   edi, eax
loc_1_9F83:
        ret
loc_1_9F84:
        ret
sub_1_9F85:
        mov    eax, dword ptr [edi + 54h]
        or     eax, eax
        je     SHORT loc_1_9FA8
        test   byte ptr [edi + 24h], 1
        jne    SHORT loc_1_9F98
        call   sub_1_A1F8
        ret
loc_1_9F98:
        push   ecx
        push   edx
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        pop    edx
        pop    ecx
loc_1_9FA8:
        ret
sub_1_9FA9:
        cmp    dword ptr [edi + 2ch], 0
        jne    SHORT loc_1_9FD1
sub_1_9FAF:
        mov    eax, dword ptr [edi + 8]
        mov    ebx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ebx
        mov    dword ptr [ebx + 8], eax
        db  083h, 03Dh, 070h, 0BDh, 000h, 000h, 000h
        jne    SHORT loc_1_9FD0
        call   sub_1_9F85
        xchg   edi, eax
        call   sub_1_A1F8
        xchg   edi, eax
loc_1_9FD0:
        ret
loc_1_9FD1:
        ret
loc_1_9FD2:
        push   eax
        lea    eax, [edi + 60h]
        test   dword ptr [edi + 20h], 100ffffh
        jne    SHORT loc_1_9FE6
        push   eax
        call   sub_1_A258
        pop    eax
loc_1_9FE6:
        call   sub_1_A220
        pop    eax
sub_1_9FEC:
        test   byte ptr [edi + 23h], 81h
        jne    SHORT loc_1_9FD2
        cmp    word ptr [edi + 20h], 0
        jne    SHORT loc_1_9FFA
        ret
loc_1_9FFA:
        or     byte ptr [edi + 23h], 80h
        push   eax
        lea    eax, [edi + 60h]
        call   sub_1_A220
        pop    eax
        cmp    word ptr [edi + 20h], 0
        jne    SHORT loc_1_9FFA
        test   byte ptr [edi + 23h], 1
        jne    SHORT loc_1_9FFA
        and    byte ptr [edi + 23h], 7fh
        ret
sub_1_A01A:
        and    byte ptr [edi + 23h], 0feh
        cmp    dword ptr [edi + 60h], 0
        jne    SHORT loc_1_A025
        ret
loc_1_A025:
        lea    eax, [edi + 60h]
        call   sub_1_A258
        clc
        ret
sub_1_A02F:
        test   byte ptr [edi + 23h], 81h
        jne    SHORT loc_1_A036
        ret
loc_1_A036:
        lea    eax, [edi + 60h]
        call   sub_1_A220
        jmp    SHORT sub_1_A02F
sub_1_A040:
        dec    dword ptr [edi + 20h]
        js     SHORT sub_1_A046
        ret
sub_1_A046:
        test   byte ptr [edi + 23h], 80h
        je     SHORT loc_1_A05C
        cmp    word ptr [edi + 20h], 0
        jne    SHORT loc_1_A05B
        lea    eax, [edi + 60h]
        call   sub_1_A258
loc_1_A05B:
        clc
loc_1_A05C:
        ret
        db  000h, 000h, 000h
sub_1_A060:
        mov    esi, dword ptr [ebp + 20h]
        mov    esi, dword ptr [esi + 8]
        cmp    dword ptr [esi + 50h], 0
        je     NEAR PTR loc_1_A1A5
        mov    edi, dword ptr [esi]
        mov    eax, 8
loc_1_A077:
        cmp    edi, esi
        je     SHORT loc_1_A083
        and    ax, word ptr [edi + 0eh]
        mov    edi, dword ptr [edi]
        jmp    SHORT loc_1_A077
loc_1_A083:
        cmp    eax, 8
        je     NEAR PTR loc_1_A1A5
        sub    esp, 1ch
        mov    edi, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [esi + 50h]
        mov    dword ptr [esp + 4], eax
        mov    eax, dword ptr [esi + 54h]
        mov    dword ptr [esp + 0ch], eax
        sub    eax, eax
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [esp], eax
        mov    dword ptr [esp + 8], eax
        movzx  eax, byte ptr [ebp + 35h]
        sub    edx, edx
        bts    edx, eax
loc_1_A0B6:
        mov    eax, dword ptr [esp + 0ch]
        mov    ebx, dword ptr [eax + 4]
        add    ebx, dword ptr [esp + 10h]
        sub    ebx, 2
        mov    cl, byte ptr [edi + 25h]
        shl    ebx, cl
        add    ebx, dword ptr [edi + 28h]
        sub    ebx, dword ptr [edi + 0ch]
        movzx  eax, byte ptr [edi + 4eh]
        not    eax
        and    ebx, eax
        add    ebx, dword ptr [edi + 0ch]
loc_1_A0DA:
        db  08Ah, 025h, 0F8h, 0B6h, 000h, 000h
        sub    al, al
        db  0CDh, 020h, 004h, 000h, 08Bh, 004h
        jb     SHORT loc_1_A0FA
        sub    eax, eax
        cmp    al, byte ptr [esi + 32h]
        sbb    eax, eax
        or     dword ptr [esp], eax
        and    eax, edx
        or     word ptr [esi + 26h], ax
loc_1_A0FA:
        movzx  eax, byte ptr [edi + 4dh]
        add    ebx, eax
        add    dword ptr [esp + 8], eax
        movzx  eax, byte ptr [edi + 24h]
        cmp    dword ptr [esp + 8], eax
        jb     SHORT loc_1_A0DA
        and    dword ptr [esp + 8], 0
        mov    eax, dword ptr [esp + 0ch]
        inc    dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax]
        cmp    dword ptr [esp + 10h], ecx
        jb     SHORT loc_1_A12F
        add    eax, 8
        mov    dword ptr [esp + 0ch], eax
        and    dword ptr [esp + 10h], 0
loc_1_A12F:
        dec    dword ptr [esp + 4]
        jne    SHORT loc_1_A0B6
        movzx  ecx, byte ptr [edi + 4eh]
        not    ecx
        movzx  eax, word ptr [edi + 26h]
        sub    eax, dword ptr [edi + 0ch]
        and    eax, ecx
        add    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [edi + 18h]
        cmp    byte ptr [edi + 46h], 1
        je     SHORT loc_1_A157
        add    eax, dword ptr [edi + 30h]
loc_1_A157:
        sub    eax, dword ptr [edi + 0ch]
        and    eax, ecx
        add    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esp + 18h], eax
        db  0BEh, 0FCh, 0B6h, 000h, 000h
loc_1_A168:
        mov    esi, dword ptr [esi + 1ch]
        db  081h, 0FEh, 0FCh, 0B6h, 000h, 000h
        je     SHORT loc_1_A1A2
        cmp    dword ptr [esi + 0ch], edi
        jne    SHORT loc_1_A168
        mov    eax, dword ptr [esi + 8]
        cmp    dword ptr [esp + 14h], 0
        js     SHORT loc_1_A194
        cmp    eax, dword ptr [esp + 14h]
        jb     SHORT loc_1_A168
        cmp    eax, dword ptr [esp + 18h]
        jae    SHORT loc_1_A168
loc_1_A18E:
        or     word ptr [esi + 26h], dx
        jmp    SHORT loc_1_A168
loc_1_A194:
        cmp    eax, dword ptr [esp + 14h]
        je     SHORT loc_1_A18E
        cmp    eax, dword ptr [esp + 18h]
        jae    SHORT loc_1_A168
        jmp    SHORT loc_1_A18E
loc_1_A1A2:
        add    esp, 1ch
loc_1_A1A5:
        ret
        db  000h, 000h, 051h, 0C1h, 0E9h, 002h, 033h, 0DBh, 0E3h, 007h, 0ADh, 0D1h, 0C3h, 033h, 0D8h, 0E2h
        db  0F9h, 059h, 0F6h, 0C1h, 002h, 074h, 008h, 066h, 0ADh, 066h, 003h, 0D8h, 0C1h, 0C3h, 003h, 0D0h
        db  0E9h, 073h, 003h, 0ACh, 002h, 0D8h, 093h, 0C3h
sub_1_A1CE:
        push   edx
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        pop    edx
        cmp    eax, 1
        ret
sub_1_A1E0:
        push   edx
        push   101h
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        pop    edx
        cmp    eax, 1
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_A1F8:
        push   ecx
        push   edx
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    edx
        pop    ecx
        ret
        db  000h, 000h, 000h
sub_1_A20C:
        cli
        test   dword ptr [eax], ecx
        jne    SHORT loc_1_A213
        sti
        ret
loc_1_A213:
        add    eax, 4
        call   sub_1_A220
        sub    eax, 4
        jmp    SHORT sub_1_A20C
sub_1_A220:
        cli
        inc    word ptr [eax]
        push   edx
        push   ecx
        push   eax
        test   byte ptr [ebp + 37h], 90h
        je     SHORT loc_1_A24C
        mov    ecx, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ecx + 14h]
        test   dword ptr [ecx + 4], 80h
        jne    SHORT loc_1_A24C
        test   dword ptr [ecx], 400000h
        jne    SHORT loc_1_A24C
        db  0CDh, 020h, 036h, 000h, 040h, 000h
        jmp    SHORT loc_1_A252
loc_1_A24C:
        db  0CDh, 020h, 012h, 000h, 040h, 000h
loc_1_A252:
        pop    eax
        pop    ecx
        pop    edx
        ret
        db  08Bh, 0C0h
sub_1_A258:
        push   ecx
        push   edx
        mov    dword ptr [eax], 0
        push   eax
        db  0CDh, 020h, 013h, 000h, 040h, 000h
        pop    eax
        pop    edx
        pop    ecx
        ret
sub_1_A26B:
        push   ebx
loc_1_A26C:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    ebx, edi
        pop    edi
        cld
        db  083h, 03Dh, 0ACh, 0B8h, 000h, 000h, 000h
        jne    SHORT loc_1_A2A9
        db  080h, 00Dh, 0AFh, 0B8h, 000h, 000h, 080h
        movzx  ebx, word ptr [ebx + 18h]
        db  089h, 01Dh, 0B4h, 0B8h, 000h, 000h
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  003h, 03Dh, 08Ch, 0BDh, 000h, 000h
        inc    dword ptr [edi]
        pop    edi
        pop    ebx
        db  0FFh, 005h, 0B0h, 0B8h, 000h, 000h
        ret
loc_1_A2A9:
        db  080h, 00Dh, 0AFh, 0B8h, 000h, 000h, 080h
        push   eax
        db  0B8h, 0B8h, 0B8h, 000h, 000h
        call   sub_1_A220
        pop    eax
        db  083h, 03Dh, 0B0h, 0B8h, 000h, 000h, 000h
        jne    SHORT loc_1_A2A9
        db  0C6h, 005h, 0AFh, 0B8h, 000h, 000h, 000h
        jmp    SHORT loc_1_A26C
sub_1_A2CE:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  003h, 03Dh, 08Ch, 0BDh, 000h, 000h
        dec    dword ptr [edi]
        pop    edi
        db  0FFh, 00Dh, 0B0h, 0B8h, 000h, 000h
        db  083h, 03Dh, 0B8h, 0B8h, 000h, 000h, 000h
        je     SHORT loc_1_A2FA
        push   eax
        db  0B8h, 0B8h, 0B8h, 000h, 000h
        call   sub_1_A258
        pop    eax
        ret
loc_1_A2FA:
        db  0C6h, 005h, 0AFh, 0B8h, 000h, 000h, 000h
        db  083h, 03Dh, 0BCh, 0B8h, 000h, 000h, 000h
        je     SHORT loc_1_A316
        push   eax
        db  0B8h, 0BCh, 0B8h, 000h, 000h
        call   sub_1_A258
        pop    eax
loc_1_A316:
        ret
        db  090h
sub_1_A318:
        db  0FFh, 005h, 0ACh, 0B8h, 000h, 000h
        js     SHORT loc_1_A334
loc_1_A320:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  003h, 03Dh, 08Ch, 0BDh, 000h, 000h
loc_1_A32D:
        inc    dword ptr [edi]
        pop    edi
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_A334:
        test   byte ptr [ebp + 37h], 90h
        jne    SHORT loc_1_A320
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  003h, 03Dh, 08Ch, 0BDh, 000h, 000h
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_A32D
        pop    edi
        db  0FFh, 00Dh, 0ACh, 0B8h, 000h, 000h
        push   eax
        db  0B8h, 0BCh, 0B8h, 000h, 000h
        call   sub_1_A220
        pop    eax
        jmp    SHORT sub_1_A318
sub_1_A361:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  003h, 03Dh, 08Ch, 0BDh, 000h, 000h
        dec    dword ptr [edi]
        pop    edi
        db  0FFh, 00Dh, 0ACh, 0B8h, 000h, 000h
        jns    SHORT loc_1_A38F
        db  066h, 083h, 03Dh, 0ACh, 0B8h, 000h, 000h, 000h
        jne    SHORT loc_1_A38F
        push   eax
        db  0B8h, 0B8h, 0B8h, 000h, 000h
        call   sub_1_A258
        pop    eax
loc_1_A38F:
        ret
loc_1_A390:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        ret
        db  000h, 052h, 025h, 000h, 0FFh, 000h, 000h, 08Bh, 077h, 014h, 00Fh, 0B7h, 056h, 00Ah, 003h, 0DAh
        db  02Bh, 0D2h, 0EBh, 02Fh
sub_1_A3AF:
        push   edx
        sub    edx, edx
        and    eax, 0ff00h
        jmp    SHORT loc_1_A3D4
sub_1_A3B9:
        push   edx
        sub    eax, eax
        mov    esi, dword ptr [edi + 14h]
        bt     dword ptr [ebp + 38h], 0ch
        sbb    edx, edx
        and    edx, 2000000h
        test   byte ptr [ebp + 39h], 5
        jne    SHORT loc_1_A41B
        mov    ah, 2
loc_1_A3D4:
        mov    esi, dword ptr [edi + 14h]
loc_1_A3D7:
        movzx  ebx, word ptr [esi + 0ah]
        add    ebx, 40h
        test   byte ptr [esi + 1], 8
        je     SHORT loc_1_A3ED
        push   eax
        movzx  eax, word ptr [esi + 8]
        lea    ebx, [ebx + eax*8]
        pop    eax
loc_1_A3ED:
        mov    esi, dword ptr [esi + 0ch]
        call   sub_1_A4E1
        jb     SHORT loc_1_A419
        lea    eax, [ebx + 94h]
        mov    dword ptr [ebx + 48h], eax
        sub    eax, eax
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx + 14h], eax
        mov    dword ptr [ebx + 18h], eax
        mov    byte ptr [ebx + 4ch], al
        or     dx, 401h
        mov    dword ptr [ebx + 8], edx
        clc
loc_1_A419:
        pop    edx
        ret
loc_1_A41B:
        or     edx, 80808h
        test   byte ptr [ebp + 39h], 1
        jne    SHORT loc_1_A42D
        and    edx, 0fffff7ffh
loc_1_A42D:
        test   byte ptr [esi + 1], 40h
        jne    SHORT loc_1_A3D7
        and    edx, 0fff7ffffh
        jmp    SHORT loc_1_A3D7
sub_1_A43B:
        push   ecx
        push   edx
        push   esi
        push   edi
        push   0
        test   byte ptr [ebp + 37h], 90h
        jne    SHORT loc_1_A45E
        call   sub_1_A3B9
        bts    dword ptr [esp], 0
loc_1_A451:
        sub    eax, eax
loc_1_A453:
        bt     dword ptr [esp], 0
        pop    ecx
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        ret
loc_1_A45E:
        mov    ah, 1
        call   sub_1_A3AF
        jae    SHORT loc_1_A451
        db  00Fh, 0BAh, 02Dh, 0F4h, 0B8h, 000h, 000h, 000h
        jb     SHORT loc_1_A495
        db  08Bh, 01Dh, 0F8h, 0B8h, 000h, 000h
        db  0A1h, 0FCh, 0B8h, 000h, 000h
        mov    dword ptr [ebx], eax
        add    eax, 14h
        mov    dword ptr [ebx + 14h], eax
        mov    ah, 4
        lea    ecx, [ebx + 14h]
        call   sub_1_A3AF
        mov    eax, 1
        jmp    SHORT loc_1_A453
loc_1_A495:
        bts    dword ptr [esp], 0
        db  00Fh, 0BAh, 02Dh, 0F4h, 0B8h, 000h, 000h, 01Fh
        db  0B8h, 000h, 0B9h, 000h, 000h
        call   sub_1_A220
        jmp    SHORT loc_1_A45E
sub_1_A4AE:
        pushfd
        sub    ebx, dword ptr [ebx + 20h]
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], ebx
        mov    word ptr [esi], 4
        push   esi
        db  0FFh, 015h, 0C0h, 0B8h, 000h, 000h
        add    esp, 0ch
        db  00Fh, 0BAh, 035h, 0F4h, 0B8h, 000h, 000h, 01Fh
        jae    SHORT loc_1_A4DF
        push   eax
        db  0B8h, 000h, 0B9h, 000h, 000h
        call   sub_1_A258
        pop    eax
loc_1_A4DF:
        popfd
        ret
sub_1_A4E1:
        sub    esp, 10h
        mov    word ptr [esp + 4], bx
        mov    dword ptr [esp + 6], esi
        mov    byte ptr [esp + 0eh], ah
        mov    word ptr [esp], 2
        mov    dword ptr [esp + 0ah], ecx
        mov    ebx, esp
        push   ebx
        db  0FFh, 015h, 0C0h, 0B8h, 000h, 000h
        mov    ebx, dword ptr [ebx + 0ah]
        add    esp, 14h
        cmp    ebx, 1
        jb     SHORT loc_1_A515
        lea    ebx, [esi + ebx]
        mov    dword ptr [ebx + 20h], esi
loc_1_A515:
        ret
sub_1_A516:
        push   edx
        sub    esp, 10h
        mov    edx, esp
        mov    word ptr [edx + 4], ax
        mov    word ptr [edx + 0ah], 9
        mov    word ptr [edx], 3
        mov    byte ptr [edx + 0ch], 8
        push   edx
        db  0FFh, 015h, 0C0h, 0B8h, 000h, 000h
        mov    eax, dword ptr [edx + 6]
        add    esp, 14h
        pop    edx
        ret
sub_1_A53E:
        push   edx
        sub    esp, 8
        mov    edx, esp
        mov    dword ptr [edx + 4], eax
        mov    word ptr [edx], 4
        push   edx
        db  0FFh, 015h, 0C0h, 0B8h, 000h, 000h
        add    esp, 0ch
        pop    edx
        ret
sub_1_A558:
        push   edi
        push   ebx
        mov    ebx, dword ptr [edi + 14h]
        mov    dword ptr [eax + 44h], ebx
        push   eax
        db  0FFh, 015h, 0D0h, 0B8h, 000h, 000h
        or     eax, eax
        pop    eax
        pop    ebx
        pop    edi
        jne    SHORT loc_1_A56F
        ret
loc_1_A56F:
        or     dword ptr [eax + 8], 40h
        ret
sub_1_A574:
        push   ebx
        push   ecx
        push   edx
        push   esi
        xor    eax, eax
        mov    ebx, 5eh
        mov    ecx, 5
        add    esi, 3
loc_1_A587:
        mul    ebx
        movzx  edx, byte ptr [esi]
        inc    esi
        sub    dl, 21h
        add    eax, edx
        loop   loc_1_A587
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
sub_1_A599:
        push   ebx
        push   ecx
        push   edx
        mov    ebx, 5eh
        mov    ecx, 5
loc_1_A5A6:
        xor    edx, edx
        div    ebx
        add    dl, 21h
        mov    byte ptr [ecx + esi + 2], dl
        loop   loc_1_A5A6
        pop    edx
        pop    ecx
        pop    ebx
        ret
sub_1_A5B7:
        push   0
        push   0
        push   0
        or     byte ptr [esp], 2
        mov    ebx, dword ptr [ebp + 1ch]
        test   byte ptr [ebx + 53h], 10h
        jne    SHORT loc_1_A629
        mov    eax, dword ptr [esi + 8]
        and    eax, 0ffffffh
        cmp    eax, 434849h
        je     SHORT loc_1_A629
        call   sub_1_A7BE
        je     SHORT loc_1_A629
        and    byte ptr [esp], 0fdh
        mov    eax, dword ptr [ebx + 14h]
        test   byte ptr [eax + 4], 40h
        jne    SHORT loc_1_A632
        call   sub_1_9DF0
        mov    edx, eax
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        xor    eax, edx
        call   sub_1_A599
        and    dword ptr [esi + 8], 0ff000000h
        or     dword ptr [esi + 8], 434849h
        push   esi
        mov    edi, ebx
        xor    ebx, ebx
        mov    ecx, 1
        call   sub_1_1574
        pop    esi
        jb     NEAR PTR loc_1_A707
        or     byte ptr [esp], 4
loc_1_A629:
        call   sub_1_A574
        mov    dword ptr [esp + 8], eax
loc_1_A632:
        mov    ebx, dword ptr [ebp + 1ch]
        movzx  ecx, word ptr [ebx + 44h]
        mov    edi, dword ptr [ebx + 34h]
        test   byte ptr [ebx + 52h], 3
        jne    SHORT loc_1_A65E
        mov    edi, dword ptr [ebx + 3ch]
        sub    edi, 2
        jb     NEAR PTR loc_1_A707
        mov    cl, byte ptr [ebx + 25h]
        shl    edi, cl
        add    edi, dword ptr [ebx + 28h]
        movzx  ecx, word ptr [ebx + 4ah]
        inc    ecx
        shr    ecx, 5
loc_1_A65E:
        mov    ebx, dword ptr [ebp + 1ch]
        push   ecx
        push   edi
        push   esi
        mov    ecx, 1
        call   sub_1_1458
        pop    esi
        pop    edi
        pop    ecx
        jb     NEAR PTR loc_1_A707
        inc    edi
        mov    edx, dword ptr [esp + 4]
        mov    ebx, dword ptr [ebp + 1ch]
        push   ecx
        push   esi
        movzx  ecx, word ptr [ebx + 20h]
        shr    ecx, 2
loc_1_A688:
        ror    edx, 5
        lodsd
        xor    edx, eax
        loop   loc_1_A688
        pop    esi
        pop    ecx
        mov    dword ptr [esp + 4], edx
        movzx  edx, word ptr [ebx + 20h]
        mov    eax, esi
loc_1_A69C:
        mov    bl, byte ptr [eax]
        cmp    bl, 0e5h
        je     SHORT loc_1_A6F7
        or     bl, bl
        je     SHORT loc_1_A707
        mov    bh, byte ptr [eax + 0bh]
        test   bh, 8
        je     SHORT loc_1_A6F7
        and    bh, 7
        cmp    bh, 7
        je     SHORT loc_1_A6F7
        bts    dword ptr [esp], 0
        jb     SHORT loc_1_A6F7
        push   ecx
        push   edi
        push   eax
        mov    ecx, dword ptr [ebp + 1ch]
        lea    edi, [ecx + 0aeh]
        xchg   esi, eax
        movsd
        movsd
        movsw
        movsb
        push   eax
        lea    edi, [edi - 0bh]
        call   sub_1_6EC3
        mov    byte ptr [ecx + 0b9h], al
        pop    eax
        cmp    bl, 5
        jne    SHORT loc_1_A6EC
        mov    byte ptr [ecx + 0aeh], 0e5h
loc_1_A6EC:
        mov    esi, eax
        pop    eax
        pop    edi
        pop    ecx
        test   byte ptr [esp], 2
        jne    SHORT loc_1_A707
loc_1_A6F7:
        add    eax, 20h
        sub    edx, 20h
        loopne loc_1_A69C
        or     ecx, ecx
        jne    NEAR PTR loc_1_A65E
loc_1_A707:
        mov    ebx, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [ebx + 58h], eax
        test   byte ptr [esp], 6
        je     SHORT loc_1_A724
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ebx + 5ch], eax
        or     word ptr [ebx + 52h], 200h
loc_1_A724:
        bt     dword ptr [esp], 0
        cmc
        lea    esp, [esp + 0ch]
        ret
sub_1_A72F:
        pushad
        mov    ecx, dword ptr [ebx + 14h]
loc_1_A733:
        db  0BBh, 048h, 0B7h, 000h, 000h
loc_1_A738:
        sti
        mov    ebx, dword ptr [ebx]
        db  081h, 0FBh, 048h, 0B7h, 000h, 000h
        je     SHORT loc_1_A77B
        cmp    dword ptr [ebx + 14h], ecx
        jne    SHORT loc_1_A738
        db  083h, 03Dh, 070h, 0BDh, 000h, 000h, 000h
        jne    SHORT loc_1_A767
        push   ebx
        push   ecx
        xor    eax, eax
        call   sub_1_0979
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        call   sub_1_0AF3
        pop    ecx
        pop    ebx
loc_1_A767:
        cli
        cmp    dword ptr [ebx + 64h], 0
        je     SHORT loc_1_A738
        lea    eax, [ebx + 8ch]
        call   sub_1_A220
        jmp    SHORT loc_1_A733
loc_1_A77B:
        db  0BAh, 08Eh, 0A7h, 000h, 000h
        db  08Ah, 025h, 0F8h, 0B6h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 08Bh, 004h
        popad
        ret
loc_1_A78E:
        mov    edx, dword ptr [esi + 0ch]
        cmp    dword ptr [edx + 14h], ecx
        je     SHORT loc_1_A797
loc_1_A796:
        ret
loc_1_A797:
        cmp    byte ptr [esi + 32h], 0
        jne    SHORT loc_1_A796
        test   byte ptr [esi + 2bh], 2
        jne    SHORT loc_1_A796
        db  0CDh, 020h, 00Dh, 000h, 08Bh, 004h
        or     eax, eax
        jne    SHORT loc_1_A796
        call   0
        push   ebx
        push   ecx
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        pop    ecx
        pop    ebx
        jmp    SHORT loc_1_A796
sub_1_A7BE:
        cld
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        db  08Bh, 01Dh, 094h, 0B8h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_A7EF
loc_1_A7CE:
        mov    ecx, dword ptr [ebx]
        jecxz  loc_1_A7EF
        lea    edx, [ebx + 4]
loc_1_A7D5:
        push   ecx
        push   esi
        movzx  eax, word ptr [edx + 2]
        add    esi, eax
        lea    edi, [edx + 4]
        movzx  ecx, word ptr [edx]
        lea    edx, [ecx + edx + 4]
        repe cmpsb
        pop    esi
        pop    ecx
        je     SHORT loc_1_A7FF
        loop   loc_1_A7D5
loc_1_A7EF:
        db  081h, 0FBh, 004h, 0B9h, 000h, 000h
        db  0BBh, 004h, 0B9h, 000h, 000h
        jne    SHORT loc_1_A7CE
        or     eax, 1
loc_1_A7FF:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
        db  000h, 000h, 000h
loc_1_A808:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], 2000h
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        mov    dword ptr [ebp + 6ch], eax
        mov    dword ptr [ebp + 64h], eax
        call   sub_1_A318
        mov    eax, 0c000h
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        mov    ax, word ptr [ebp]
        and    al, 1eh
        mov    word ptr [ebp + 45h], ax
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_1_91B8
        jb     NEAR PTR loc_1_A936
        cmp    ebx, -1
        je     NEAR PTR loc_1_A93C
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        mov    esi, dword ptr [ebp + 10h]
        db  0C7h, 006h, 043h, 0A9h, 000h, 000h
        db  0C7h, 046h, 004h, 068h, 046h, 000h, 000h
        db  0C7h, 046h, 008h, 020h, 0B9h, 000h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        movzx  ecx, word ptr [esi]
        sub    cx, word ptr [esi + 2]
        lea    eax, [ecx + 1bh]
        push   ecx
        push   esi
        call   sub_1_A1CE
        pop    esi
        pop    ecx
        mov    edi, eax
        mov    al, 8
        or     edi, edi
        je     NEAR PTR loc_1_A936
        movzx  eax, word ptr [esi + 2]
        lea    esi, [eax + esi]
        mov    edx, edi
        lea    edi, [edi + 17h]
        rep movsb
        sub    eax, eax
        stosw
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ebp + 50h]
        mov    dword ptr [edx + 0ch], eax
        mov    eax, dword ptr [ebp + 4ch]
        mov    dword ptr [edx + 10h], eax
        mov    ax, word ptr [ebp + 45h]
        mov    word ptr [edx + 14h], ax
        sub    eax, eax
        test   word ptr [ebp + 42h], 8
        setne  al
        shl    al, 3
        or     ah, al
        test   word ptr [ebp + 42h], 20h
        setne  al
        shl    al, 2
        or     ah, al
        test   word ptr [ebp + 42h], 1
        setne  al
        shl    al, 0
        or     ah, al
        mov    byte ptr [edx + 16h], ah
        mov    dword ptr [ebp + 20h], edx
        mov    eax, dword ptr [ebp + 50h]
        mov    dword ptr [edx + 0ch], eax
        call   sub_1_AA38
        mov    edx, dword ptr [ebp + 20h]
        jb     SHORT loc_1_A921
        db  0B9h, 018h, 0B9h, 000h, 000h
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [ecx + 4], edx
        xor    eax, eax
        jmp    SHORT loc_1_A928
loc_1_A921:
        xchg   edx, eax
        call   sub_1_A1F8
        xchg   edx, eax
loc_1_A928:
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        jmp    NEAR PTR loc_1_454A
loc_1_A936:
        jne    NEAR PTR loc_1_454A
loc_1_A93C:
        mov    al, 2
        jmp    NEAR PTR loc_1_454A
loc_1_A943:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], 2000h
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        mov    dword ptr [ebp + 6ch], eax
        mov    dword ptr [ebp + 64h], eax
        call   sub_1_A318
        mov    ebx, dword ptr [ebp + 20h]
        cmp    dword ptr [ebx + 8], 0
        je     NEAR PTR loc_1_A9FB
        test   word ptr [ebp + 18h], 40h
        jne    SHORT loc_1_A983
        test   byte ptr [ebx + 16h], 1
        je     SHORT loc_1_A9FB
loc_1_A983:
        mov    eax, 10h
        test   byte ptr [ebx + 16h], 1
        je     SHORT loc_1_A991
        or     eax, 1
loc_1_A991:
        test   byte ptr [ebx + 16h], 4
        je     SHORT loc_1_A99A
        or     eax, 20h
loc_1_A99A:
        test   byte ptr [ebx + 16h], 8
        je     SHORT loc_1_A9A3
        or     eax, 8
loc_1_A9A3:
        mov    word ptr [ebp + 42h], ax
        mov    ax, word ptr [ebx + 14h]
        mov    word ptr [ebp + 45h], ax
        mov    edx, dword ptr [ebx + 10h]
        inc    edx
        test   word ptr [ebp + 18h], 40h
        je     SHORT loc_1_A9C3
        mov    edx, dword ptr [ebp + 24h]
        cmp    edx, dword ptr [ebx + 10h]
        ja     SHORT loc_1_A9FB
loc_1_A9C3:
        lea    eax, [ebx + 17h]
        mov    ebx, dword ptr [ebx + 0ch]
        mov    ecx, 2
        call   sub_1_9366
        jb     SHORT loc_1_A9F3
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        call   sub_1_AA38
        push   eax
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    eax
        jmp    NEAR PTR loc_1_454A
loc_1_A9F3:
        or     eax, eax
        jne    NEAR PTR loc_1_454A
loc_1_A9FB:
        mov    al, 12h
        jmp    NEAR PTR loc_1_454A
loc_1_AA02:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        call   sub_1_A318
        mov    ebx, dword ptr [ebp + 20h]
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [edx], eax
        xchg   ebx, eax
        call   sub_1_A1F8
        xchg   ebx, eax
        jmp    NEAR PTR loc_1_4574
sub_1_AA38:
        mov    edx, dword ptr [ebp + 20h]
        mov    eax, dword ptr [ebp + 50h]
        cmp    dword ptr [edx + 0ch], eax
        jne    NEAR PTR loc_1_AB56
        mov    eax, dword ptr [ebp + 4ch]
        cmp    eax, dword ptr [edx + 10h]
        jb     NEAR PTR loc_1_AB56
        mov    dword ptr [edx + 10h], eax
        mov    edx, dword ptr [ebp + 6ch]
        test   byte ptr [edx + 7], 2
        je     SHORT loc_1_AA62
        mov    eax, dword ptr [edx + 0ch]
loc_1_AA62:
        mov    dword ptr [ebp + 24h], eax
        mov    edx, dword ptr [ebp + 14h]
        movzx  eax, byte ptr [ebx + 0bh]
        and    al, 3fh
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 1ch], 0
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [edx + 20h], eax
        mov    eax, dword ptr [ebx + 0eh]
        push   edx
        push   eax
        db  0CDh, 020h, 039h, 000h, 040h, 000h
        add    esp, 4
        mov    ecx, edx
        pop    edx
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edx + 8], ecx
        push   edx
        movzx  eax, byte ptr [ebx + 0dh]
        mov    edx, 186a0h
        mul    edx
        pop    edx
        add    dword ptr [edx + 4], eax
        adc    dword ptr [edx + 8], 0
        mov    ax, word ptr [ebx + 12h]
        shl    eax, 10h
        push   edx
        push   eax
        db  0CDh, 020h, 039h, 000h, 040h, 000h
        add    esp, 4
        mov    ecx, edx
        pop    edx
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [edx + 10h], ecx
        mov    eax, dword ptr [ebx + 16h]
        push   edx
        push   eax
        db  0CDh, 020h, 039h, 000h, 040h, 000h
        add    esp, 4
        mov    ecx, edx
        pop    edx
        mov    dword ptr [edx + 14h], eax
        mov    dword ptr [edx + 18h], ecx
        sub    eax, eax
        mov    dword ptr [edx + 28h], eax
        mov    dword ptr [edx + 24h], eax
        mov    esi, dword ptr [ebp + 6ch]
        mov    word ptr [edx + 2ch], 0
        mov    word ptr [edx + 234h], 0
        lea    edi, [edx + 2ch]
        test   byte ptr [esi + 7], 2
        je     SHORT loc_1_AB24
        push   esi
        lea    esi, [esi + 0b2h]
        mov    ecx, 103h
        sub    eax, eax
loc_1_AB0B:
        movsw
        cmp    word ptr [edi - 2], ax
        loopne loc_1_AB0B
        pop    esi
        je     SHORT loc_1_AB18
        stosw
loc_1_AB18:
        lea    edi, [edx + 234h]
        test   byte ptr [esi + 7], 1
        je     SHORT loc_1_AB53
loc_1_AB24:
        movzx  ecx, word ptr [esi + 94h]
        sub    ecx, 2
        jbe    SHORT loc_1_AB4F
        cmp    ecx, 18h
        ja     SHORT loc_1_AB4F
        lea    eax, [esi + 96h]
        mov    esi, eax
        rep movsb
        cmp    word ptr [edi - 2], 2eh
        jne    SHORT loc_1_AB4F
        cmp    word ptr [eax], 2eh
        je     SHORT loc_1_AB4F
        sub    edi, 2
loc_1_AB4F:
        sub    eax, eax
        stosw
loc_1_AB53:
        xor    eax, eax
        ret
loc_1_AB56:
        mov    edx, dword ptr [ebp + 20h]
        mov    al, 12h
        stc
        ret
sub_1_AB5D:
        db  0BAh, 018h, 0B9h, 000h, 000h
loc_1_AB62:
        mov    edx, dword ptr [edx]
        db  081h, 0FAh, 018h, 0B9h, 000h, 000h
        je     SHORT loc_1_AB84
        cmp    dword ptr [edx + 8], ebx
        jne    SHORT loc_1_AB62
        cmp    eax, -1
        je     SHORT loc_1_AB7B
        cmp    dword ptr [edx + 0ch], eax
        jne    SHORT loc_1_AB62
loc_1_AB7B:
        mov    dword ptr [edx + 8], 0
        jmp    SHORT loc_1_AB62
loc_1_AB84:
        ret
loc_1_AB85:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        call   sub_1_A318
        cmp    word ptr [ebp + 18h], 0
        je     NEAR PTR loc_1_4574
        mov    al, 1
        cmp    word ptr [ebp + 18h], 2
        jne    NEAR PTR loc_1_4576
        mov    edi, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [ebp + 14h]
        test   byte ptr [edi + 52h], 3
        je     SHORT loc_1_ABD4
        cmp    dword ptr [ebp], 4
        jb     SHORT loc_1_ABE6
        mov    dword ptr [eax], 544146h
        jmp    SHORT loc_1_ABE6
loc_1_ABD4:
        cmp    dword ptr [ebp], 6
        jb     SHORT loc_1_ABE6
        mov    dword ptr [eax], 33544146h
        mov    word ptr [eax + 4], 32h
loc_1_ABE6:
        mov    dword ptr [ebp], 10400ffh
        mov    dx, 4006h
        test   word ptr [edi + 52h], 10h
        je     SHORT loc_1_ABFE
        or     dx, 8000h
loc_1_ABFE:
        mov    word ptr [ebp + 18h], dx
        movzx  eax, word ptr [edi + 20h]
        mov    dword ptr [ebp + 24h], eax
        jmp    NEAR PTR loc_1_4574
        db  08Bh, 0C0h, 061h, 0ACh, 000h, 000h, 0F7h, 0ACh, 000h, 000h, 022h, 0ADh, 000h, 000h, 05Bh, 0ADh
        db  000h, 000h, 0E0h, 0ADh, 000h, 000h
loc_1_AC24:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        mov    ax, word ptr [ebp + 18h]
        and    ah, 91h
        mov    byte ptr [ebp + 37h], ah
        sub    eax, eax
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        call   sub_1_A318
        movzx  eax, byte ptr [ebp + 4]
        cmp    eax, 5
        jae    SHORT loc_1_AC5A
        db  0FFh, 024h, 085h, 010h, 0ACh, 000h, 000h
loc_1_AC5A:
        mov    al, 1
        jmp    NEAR PTR loc_1_4576
loc_1_AC61:
        mov    esi, dword ptr [ebp + 20h]
        test   byte ptr [esi + 0eh], 10h
        jne    NEAR PTR loc_1_456D
        mov    esi, dword ptr [esi + 8]
        mov    edi, dword ptr [ebp + 14h]
        movzx  eax, byte ptr [esi + 43h]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 38h]
        push   eax
        db  0CDh, 020h, 039h, 000h, 040h, 000h
        add    esp, 4
        mov    dword ptr [edi + 14h], eax
        mov    dword ptr [edi + 18h], edx
        mov    eax, dword ptr [esi + 3eh]
        push   eax
        db  0CDh, 020h, 039h, 000h, 040h, 000h
        add    esp, 4
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [edi + 8], edx
        movzx  eax, byte ptr [esi + 42h]
        mov    edx, 186a0h
        mul    edx
        add    dword ptr [edi + 4], eax
        adc    dword ptr [edi + 8], edx
        movzx  eax, word ptr [esi + 3ch]
        shl    eax, 10h
        push   eax
        db  0CDh, 020h, 039h, 000h, 040h, 000h
        add    esp, 4
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [edi + 10h], edx
        mov    edx, dword ptr [esi + 28h]
        mov    eax, dword ptr [edx + 54h]
        mov    dword ptr [edi + 1ch], eax
        mov    eax, dword ptr [esi + 30h]
        mov    dword ptr [edi + 24h], eax
        mov    dword ptr [edi + 20h], 0
        mov    dword ptr [edi + 28h], 1
        movzx  eax, byte ptr [esi + 25h]
        mov    dword ptr [edi + 30h], eax
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [edi + 2ch], eax
        jmp    NEAR PTR loc_1_4574
loc_1_ACF7:
        mov    esi, dword ptr [ebp + 20h]
        test   byte ptr [esi + 0eh], 10h
        jne    NEAR PTR loc_1_456D
        mov    esi, dword ptr [esi + 8]
        mov    edx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        mov    edi, dword ptr [ebp + 0ch]
        call   sub_1_1A6C
        mov    al, 8
        jb     NEAR PTR loc_1_454A
        jmp    NEAR PTR loc_1_4548
loc_1_AD22:
        mov    esi, dword ptr [ebp + 0ch]
        mov    word ptr [esi], 0
        mov    esi, dword ptr [ebp + 20h]
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     SHORT loc_1_AD54
        cmp    dword ptr [ebp + 1ch], eax
        jne    SHORT loc_1_AD54
        mov    edx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [esi + 10h]
        mov    edi, dword ptr [ebp + 0ch]
        call   sub_1_1A6C
        mov    al, 8
        jb     NEAR PTR loc_1_454A
        jmp    NEAR PTR loc_1_4548
loc_1_AD54:
        mov    al, 12h
        jmp    NEAR PTR loc_1_454A
loc_1_AD5B:
        or     word ptr [ebp + 38h], 2000h
        mov    dword ptr [ebp + 6ch], 0
        mov    dword ptr [ebp + 64h], 0
        mov    esi, dword ptr [ebp + 0ch]
        cmp    word ptr [esi], 0
        je     NEAR PTR loc_1_4548
        mov    esi, dword ptr [ebp + 20h]
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     SHORT loc_1_ADD9
        cmp    dword ptr [ebp + 1ch], eax
        jne    SHORT loc_1_ADD9
        mov    ax, word ptr [esi + 14h]
        mov    word ptr [ebp + 45h], ax
        mov    word ptr [ebp + 42h], 8
        mov    eax, dword ptr [ebp + 0ch]
        movzx  edx, word ptr [eax + 2]
        lea    edx, [edx + eax + 2]
        cmp    word ptr [edx], 2eh
        jne    SHORT loc_1_ADB4
        mov    word ptr [edx], 3fh
        cmp    word ptr [edx + 2], 2eh
loc_1_ADB4:
        sete   dl
        movzx  edx, dl
        push   edx
        call   sub_1_91B8
        pop    edx
        jb     SHORT loc_1_ADD9
        mov    esi, dword ptr [ebp + 20h]
        mov    eax, dword ptr [ebp + 50h]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [ebp + 4ch]
        add    eax, edx
        mov    dword ptr [esi + 10h], eax
        jmp    NEAR PTR loc_1_4548
loc_1_ADD9:
        mov    al, 12h
        jmp    NEAR PTR loc_1_454A
loc_1_ADE0:
        or     word ptr [ebp + 38h], 2000h
        mov    dword ptr [ebp + 6ch], 0
        mov    dword ptr [ebp + 64h], 0
        call   sub_1_AF87
        mov    eax, 2
        xor    edx, edx
        call   sub_1_43C9
        mov    byte ptr [ebp + 45h], 16h
        mov    byte ptr [ebp + 46h], 0
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_1_91B8
        jb     NEAR PTR loc_1_AF3D
        je     NEAR PTR loc_1_AF44
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        mov    eax, dword ptr [ebp + 20h]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 1ch]
        cmp    ecx, dword ptr [eax + 28h]
        jne    NEAR PTR loc_1_AF4B
        mov    cl, byte ptr [ebx + 0bh]
        test   cl, 18h
        jne    NEAR PTR loc_1_AF4B
        cmp    cl, byte ptr [eax + 43h]
        jne    NEAR PTR loc_1_AF4B
        lea    esi, [ebx]
        lea    edi, [eax + 45h]
        mov    ecx, 0bh
        repe cmpsb
        jne    NEAR PTR loc_1_AF4B
        test   dword ptr [ebp], 1
        je     SHORT loc_1_AE77
loc_1_AE6C:
        bts    dword ptr [eax + 24h], 4
        jb     NEAR PTR loc_1_AF70
loc_1_AE77:
        cli
        mov    dl, byte ptr [ebx + 0dh]
        mov    byte ptr [eax + 42h], dl
        mov    dx, word ptr [ebx + 0eh]
        mov    word ptr [eax + 3eh], dx
        mov    dx, word ptr [ebx + 10h]
        mov    word ptr [eax + 40h], dx
        mov    dx, word ptr [ebx + 12h]
        mov    word ptr [eax + 3ch], dx
        mov    dx, word ptr [ebx + 16h]
        mov    word ptr [eax + 38h], dx
        mov    dx, word ptr [ebx + 18h]
        mov    word ptr [eax + 3ah], dx
        mov    edx, dword ptr [ebx + 1ch]
        mov    dword ptr [eax + 30h], edx
        mov    esi, dword ptr [ebp + 64h]
        mov    edx, ebx
        sub    edx, dword ptr [esi + 10h]
        shr    edx, 5
        mov    byte ptr [eax + 25h], dl
        mov    edi, dword ptr [ebp + 1ch]
        mov    cl, byte ptr [edi + 4ch]
        sub    cl, 5
        shr    edx, cl
        add    edx, dword ptr [esi + 8]
        mov    dword ptr [eax + 1ch], edx
        mov    edx, dword ptr [ebp + 50h]
        mov    dword ptr [eax + 18h], edx
        mov    edx, dword ptr [ebp + 4ch]
        mov    dword ptr [eax + 14h], edx
        xor    edx, edx
        test   byte ptr [eax + 24h], 8
        je     SHORT loc_1_AEE2
        mov    edx, dword ptr [ebx + 12h]
loc_1_AEE2:
        mov    dx, word ptr [ebx + 1ah]
        mov    dword ptr [eax + 10h], edx
        test   dword ptr [ebp], 1
        je     SHORT loc_1_AEFF
        and    dword ptr [eax + 50h], 0
        mov    esi, dword ptr [eax + 54h]
        sub    esi, 8
        mov    dword ptr [eax + 5ch], esi
loc_1_AEFF:
        and    byte ptr [eax + 44h], 0feh
        sti
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        sub    eax, eax
loc_1_AF0F:
        push   eax
        call   sub_1_AFC5
        test   dword ptr [ebp], 1
        je     SHORT loc_1_AF37
        mov    eax, dword ptr [ebp + 20h]
        mov    eax, dword ptr [eax + 8]
        and    byte ptr [eax + 24h], 0efh
        db  0B8h, 0B0h, 0B6h, 000h, 000h
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_AF37
        call   sub_1_A258
loc_1_AF37:
        pop    eax
loc_1_AF38:
        jmp    NEAR PTR loc_1_454A
loc_1_AF3D:
        mov    eax, 3
        jmp    SHORT loc_1_AF0F
loc_1_AF44:
        mov    eax, 2
        jmp    SHORT loc_1_AF0F
loc_1_AF4B:
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        mov    eax, 2
        jmp    SHORT loc_1_AF0F
loc_1_AF5B:
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        call   sub_1_AFC5
        mov    eax, 5
        jmp    SHORT loc_1_AF38
loc_1_AF70:
        test   byte ptr [eax + 24h], 80h
        jne    SHORT loc_1_AF5B
        push   eax
        db  0B8h, 0B0h, 0B6h, 000h, 000h
        call   sub_1_A220
        pop    eax
        jmp    NEAR PTR loc_1_AE6C
sub_1_AF87:
        test   byte ptr [ebp + 37h], 90h
        je     SHORT loc_1_AFC4
loc_1_AF8D:
        db  00Fh, 0BAh, 02Dh, 044h, 0B9h, 000h, 000h, 000h
        jae    SHORT loc_1_AFAB
        db  00Fh, 0BAh, 02Dh, 044h, 0B9h, 000h, 000h, 01Fh
        db  0B8h, 048h, 0B9h, 000h, 000h
        call   sub_1_A220
        jmp    SHORT loc_1_AF8D
loc_1_AFAB:
        sub    eax, eax
        db  08Dh, 03Dh, 04Ch, 0B9h, 000h, 000h
        mov    dword ptr [ebp + 6ch], edi
        mov    ecx, 0f5h
        rep stosd
        mov    cl, 0
        call   sub_1_8D94
loc_1_AFC4:
        ret
sub_1_AFC5:
        test   byte ptr [ebp + 37h], 90h
        je     SHORT loc_1_AFEE
        db  00Fh, 0BAh, 035h, 044h, 0B9h, 000h, 000h, 000h
        mov    dword ptr [ebp + 6ch], 0
        db  00Fh, 0BAh, 035h, 044h, 0B9h, 000h, 000h, 01Fh
        jae    SHORT loc_1_AFEE
        db  0B8h, 048h, 0B9h, 000h, 000h
        call   sub_1_A258
loc_1_AFEE:
        ret
sub_1_AFEF:
        mov    ebx, dword ptr [esi]
        or     ebx, ebx
        jne    SHORT loc_1_B002
        mov    eax, dword ptr [ebp + 1ch]
        movzx  eax, word ptr [eax + 44h]
        cmp    word ptr [esi + 4], ax
        jae    SHORT loc_1_B02C
loc_1_B002:
        call   sub_1_1201
        jb     SHORT loc_1_B02C
        movzx  eax, word ptr [esi + 4]
        push   esi
        call   sub_1_0BD7
        pop    esi
        jb     SHORT loc_1_B02C
        test   byte ptr [ebx + 0bh], 0d8h
        jne    SHORT loc_1_B02C
        lea    esi, [esi + 6]
        lea    edi, [ebx]
        mov    ecx, 0bh
        repe cmpsb
        jne    SHORT loc_1_B02C
        clc
        ret
loc_1_B02C:
        stc
        ret
sub_1_B02E:
        call   sub_1_A26B
        sub    esi, esi
        mov    edi, dword ptr [ebp + 1ch]
        cli
        mov    eax, dword ptr [edi + 64h]
        or     eax, dword ptr [edi + 68h]
        or     eax, dword ptr [edi + 6ch]
        or     eax, dword ptr [edi + 80h]
        or     eax, dword ptr [edi + 88h]
        or     eax, dword ptr [edi + 8ch]
        jne    NEAR PTR loc_1_B15D
        mov    word ptr [edi + 1ch], ax
        mov    dword ptr [edi + 78h], eax
        mov    byte ptr [edi + 0bbh], al
        mov    dword ptr [edi + 84h], eax
        mov    dword ptr [edi + 0bch], eax
        mov    dword ptr [edi + 0c0h], eax
        mov    dword ptr [edi + 0c4h], eax
        mov    dword ptr [edi + 0c8h], eax
        sti
        test   byte ptr [edi + 52h], 10h
        je     SHORT loc_1_B0A1
        mov    cx, 0db01h
        mov    ebx, edi
        push   esi
        push   edi
        call   sub_1_449E
        pop    edi
        pop    esi
        jb     NEAR PTR loc_1_B15D
loc_1_B0A1:
        push   edi
        push   800h
        db  0CDh, 020h, 00Dh, 000h, 040h, 000h
        add    esp, 4
        pop    edi
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_B15D
        bt     word ptr [edi + 52h], 4
        cmc
        sbb    ebx, ebx
        and    ebx, 100h
        or     word ptr [ebp + 38h], bx
        push   esi
        push   edi
        mov    ebx, edi
        mov    ecx, 1
        sub    edi, edi
        call   sub_1_1458
        pop    edi
        pop    esi
        jb     SHORT loc_1_B15D
        push   esi
        push   edi
        call   sub_1_68DF
        pop    edi
        pop    esi
        jb     SHORT loc_1_B15D
        xor    ecx, ecx
        xchg   dword ptr [edi + 9ch], ecx
        jecxz  loc_1_B103
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
        call   sub_1_6556
loc_1_B103:
        test   byte ptr [edi + 52h], 10h
        jne    SHORT loc_1_B126
        push   esi
        push   edi
        mov    ebx, edi
        mov    ecx, 1
        sub    edi, edi
        call   sub_1_1458
        pop    edi
        pop    esi
        jb     SHORT loc_1_B15D
        push   esi
        push   edi
        call   sub_1_A5B7
        pop    edi
        pop    esi
loc_1_B126:
        push   esi
        push   edi
        sub    eax, eax
        mov    ebx, edi
        call   sub_1_0979
        pop    edi
        pop    esi
        push   esi
        push   edi
        sub    eax, eax
        mov    ebx, edi
        push   eax
        push   eax
        push   eax
        push   eax
        call   sub_1_0AF3
        pop    edi
        pop    esi
        sub    eax, eax
loc_1_B146:
        push   eax
        or     esi, esi
        je     SHORT loc_1_B155
        push   esi
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
        add    esp, 4
loc_1_B155:
        pop    eax
        or     eax, eax
        je     SHORT loc_1_B15B
        stc
loc_1_B15B:
        sti
        ret
loc_1_B15D:
        mov    eax, 5
        jmp    SHORT loc_1_B146
VFAT_Control:
        cmp    eax, 0
        db  00Fh, 084h, 057h, 051h, 0FEh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 050h, 051h, 0FEh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 015h, 053h, 0FEh, 0FFh
        cmp    eax, 4
        je     SHORT loc_1_B195
        cmp    eax, 14h
        je     SHORT loc_1_B19D
        cmp    eax, 6
        je     SHORT loc_1_B195
        cmp    eax, 5
        je     SHORT loc_1_B195
        clc
        ret
loc_1_B195:
        db  0FFh, 005h, 070h, 0BDh, 000h, 000h
        clc
        ret
loc_1_B19D:
        db  0F7h, 005h, 0A3h, 0B1h, 000h, 000h, 000h, 000h, 040h, 000h
        db  00Fh, 085h, 0AAh, 050h, 0FEh, 0FFh
        ret
        db  0F8h, 0C3h, 0F8h, 0C3h, 000h, 000h, 000h, 000h, 000h, 0F0h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 001h, 001h, 002h, 003h, 004h, 005h, 000h, 000h, 000h
loc_1_B1EC:
        pop    esp
        outsb
        cmp    eax, 4f203d3dh
        inc    esi
        push   esp
        and    byte ptr [ebp + edx*2 + 4dh], al
        push   eax
        and    byte ptr [6e5c3d3dh], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B204:
        dec    edi
        inc    esi
        push   esp
        and    byte ptr [eax], ch
        and    eax, 2029786ch
        and    eax, 2e253a63h
        xor    dword ptr [ecx], esi
        db  073h, 05Ch
        outsb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B21C:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   2f746e63h
        insb
        popad
        cmp    eax, 20786c25h
        and    byte ptr [eax], al
loc_1_B230:
        and    byte ptr [eax], ah
        push   edx
        das
        push   edi
        db  074h, 020h
        cmp    eax, 786c2520h
        pop    esp
        outsb
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 46h], dl
        push   esp
        and    byte ptr [ebx + 68h], al
        popad
        imul   ebp, dword ptr [esi + 3ah], 6e5ch
        add    byte ptr [eax], al
loc_1_B254:
        or     dword ptr [ebp + 6dh], eax
        db  070h, 074h
        db  079h, 02Eh
        pop    esp
        outsb
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        push   ebp
        dec    ecx
        inc    esp
        cmp    eax, 20786c25h
        push   eax
        dec    ecx
        inc    esp
        cmp    eax, 5c786c25h
        outsb
        add    byte ptr [eax], al
loc_1_B274:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B278:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B27C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B280:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B284:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B288:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B28C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B290:
        xchg   esi, eax
        xor    byte ptr [edi], al
        ja     SHORT loc_1_B2C1
        popad
        push   cs
        out    dx, al
        mov    edx, 19990951h
        les    ebp, ptr [ebp + 7]
        db  08Fh, 0F4h, 06Ah, 070h, 035h, 0A5h, 063h, 0E9h, 0A3h, 095h, 064h, 09Eh, 032h, 088h, 0DBh, 00Eh
        db  0A4h, 0B8h, 0DCh, 079h, 01Eh, 0E9h, 0D5h, 0E0h, 088h, 0D9h, 0D2h, 097h, 02Bh, 04Ch, 0B6h, 009h
        db  0BDh
loc_1_B2C1:
        jl     SHORT loc_1_B274
        db  07Eh, 007h
        sub    eax, 1d91e7b8h
        mov    edi, 0b7106490h
        sbb    eax, 6ab020f2h
        dec    eax
        jno    SHORT loc_1_B290
        fiadd  word ptr [ecx - 42h]
        test   byte ptr [ebp - 2ch], bh
        ficomp dword ptr [edx]
        db  0EBh, 0E4h
        db  0DDh, 06Dh, 051h, 0B5h, 0D4h, 0F4h, 0C7h, 085h, 0D3h, 083h, 056h, 098h, 06Ch, 013h, 0C0h, 0A8h
        db  06Bh, 064h, 07Ah, 0F9h, 062h, 0FDh, 0ECh, 0C9h, 065h, 08Ah, 04Fh, 05Ch, 001h, 014h, 0D9h, 06Ch
        db  006h, 063h, 063h, 03Dh, 00Fh, 0FAh, 0F5h, 00Dh, 008h, 08Dh, 0C8h, 020h, 06Eh, 03Bh, 05Eh, 010h
        db  069h, 04Ch, 0E4h, 041h, 060h, 0D5h, 072h, 071h, 067h, 0A2h, 0D1h, 0E4h, 003h, 03Ch, 047h, 0D4h
        db  004h, 04Bh, 0FDh, 085h, 00Dh, 0D2h, 06Bh, 0B5h, 00Ah, 0A5h, 0FAh, 0A8h, 0B5h, 035h, 06Ch, 098h
        db  0B2h, 042h, 0D6h, 0C9h, 0BBh, 0DBh, 040h, 0F9h, 0BCh, 0ACh, 0E3h, 06Ch, 0D8h, 032h, 075h, 05Ch
        db  0DFh, 045h, 0CFh, 00Dh, 0D6h, 0DCh, 059h, 03Dh, 0D1h, 0ABh, 0ACh, 030h, 0D9h, 026h, 03Ah, 000h
        db  0DEh, 051h, 080h, 051h, 0D7h, 0C8h, 016h, 061h, 0D0h, 0BFh, 0B5h, 0F4h, 0B4h, 021h, 023h, 0C4h
        db  0B3h, 056h, 099h, 095h, 0BAh, 0CFh, 00Fh, 0A5h, 0BDh, 0B8h, 09Eh, 0B8h, 002h, 028h, 008h, 088h
        db  005h, 05Fh, 0B2h, 0D9h, 00Ch, 0C6h, 024h, 0E9h, 00Bh, 0B1h, 087h, 07Ch, 06Fh, 02Fh, 011h, 04Ch
        db  068h, 058h, 0ABh, 01Dh, 061h, 0C1h, 03Dh, 02Dh, 066h, 0B6h, 090h, 041h, 0DCh, 076h, 006h, 071h
        db  0DBh, 001h, 0BCh, 020h, 0D2h, 098h, 02Ah, 010h, 0D5h, 0EFh, 089h, 085h, 0B1h, 071h, 01Fh, 0B5h
        db  0B6h, 006h, 0A5h, 0E4h, 0BFh, 09Fh, 033h, 0D4h, 0B8h, 0E8h, 0A2h, 0C9h, 007h, 078h, 034h, 0F9h
        db  000h, 00Fh, 08Eh, 0A8h, 009h, 096h, 018h, 098h, 00Eh, 0E1h, 0BBh, 00Dh, 06Ah, 07Fh, 02Dh, 03Dh
        db  06Dh, 008h, 097h, 06Ch, 064h, 091h, 001h, 05Ch, 063h, 0E6h, 0F4h, 051h, 06Bh, 06Bh, 062h, 061h
        db  06Ch, 01Ch, 0D8h, 030h, 065h, 085h, 04Eh, 000h, 062h, 0F2h, 0EDh, 095h, 006h, 06Ch, 07Bh, 0A5h
        db  001h, 01Bh, 0C1h, 0F4h, 008h, 082h, 057h, 0C4h, 00Fh, 0F5h, 0C6h, 0D9h, 0B0h, 065h, 050h, 0E9h
        db  0B7h, 012h, 0EAh, 0B8h, 0BEh, 08Bh, 07Ch, 088h, 0B9h, 0FCh, 0DFh, 01Dh, 0DDh, 062h, 049h, 02Dh
        db  0DAh, 015h, 0F3h, 07Ch, 0D3h, 08Ch, 065h, 04Ch, 0D4h, 0FBh, 058h, 061h, 0B2h, 04Dh, 0CEh, 051h
        db  0B5h, 03Ah, 074h, 000h, 0BCh, 0A3h, 0E2h, 030h, 0BBh, 0D4h, 041h, 0A5h, 0DFh, 04Ah, 0D7h, 095h
        db  0D8h, 03Dh, 06Dh, 0C4h, 0D1h, 0A4h, 0FBh, 0F4h, 0D6h, 0D3h, 06Ah, 0E9h, 069h, 043h, 0FCh, 0D9h
        db  06Eh, 034h, 046h, 088h, 067h, 0ADh, 0D0h, 0B8h, 060h, 0DAh, 073h, 02Dh, 004h, 044h, 0E5h, 01Dh
        db  003h, 033h, 05Fh, 04Ch, 00Ah, 0AAh, 0C9h, 07Ch, 00Dh, 0DDh, 03Ch, 071h, 005h, 050h, 0AAh, 041h
        db  002h, 027h, 010h, 010h, 00Bh, 0BEh, 086h, 020h, 00Ch, 0C9h, 025h, 0B5h, 068h, 057h, 0B3h, 085h
        db  06Fh, 020h, 009h, 0D4h, 066h, 0B9h, 09Fh, 0E4h, 061h, 0CEh, 00Eh, 0F9h, 0DEh, 05Eh, 098h, 0C9h
        db  0D9h, 029h, 022h, 098h, 0D0h, 0B0h, 0B4h, 0A8h, 0D7h, 0C7h, 017h, 03Dh, 0B3h, 059h, 081h, 00Dh
        db  0B4h, 02Eh, 03Bh, 05Ch, 0BDh, 0B7h, 0ADh, 06Ch, 0BAh, 0C0h, 020h, 083h, 0B8h, 0EDh, 0B6h, 0B3h
        db  0BFh, 09Ah, 00Ch, 0E2h, 0B6h, 003h, 09Ah, 0D2h, 0B1h, 074h, 039h, 047h, 0D5h, 0EAh, 0AFh, 077h
        db  0D2h, 09Dh, 015h, 026h, 0DBh, 004h, 083h, 016h, 0DCh, 073h, 012h, 00Bh, 063h, 0E3h, 084h, 03Bh
        db  064h, 094h, 03Eh, 06Ah, 06Dh, 00Dh, 0A8h, 05Ah, 06Ah, 07Ah, 00Bh, 0CFh, 00Eh, 0E4h, 09Dh, 0FFh
        db  009h, 093h, 027h, 0AEh, 000h, 00Ah, 0B1h, 09Eh, 007h, 07Dh, 044h, 093h, 00Fh, 0F0h, 0D2h, 0A3h
        db  008h, 087h, 068h, 0F2h, 001h, 01Eh, 0FEh, 0C2h, 006h, 069h, 05Dh, 057h, 062h, 0F7h, 0CBh, 067h
        db  065h, 080h, 071h, 036h, 06Ch, 019h, 0E7h, 006h, 06Bh, 06Eh, 076h, 01Bh, 0D4h, 0FEh, 0E0h, 02Bh
        db  0D3h, 089h, 05Ah, 07Ah, 0DAh, 010h, 0CCh, 04Ah, 0DDh, 067h, 06Fh, 0DFh, 0B9h, 0F9h, 0F9h, 0EFh
        db  0BEh, 08Eh, 043h, 0BEh, 0B7h, 017h, 0D5h, 08Eh, 0B0h, 060h, 0E8h, 0A3h, 0D6h, 0D6h, 07Eh, 093h
        db  0D1h, 0A1h, 0C4h, 0C2h, 0D8h, 038h, 052h, 0F2h, 0DFh, 04Fh, 0F1h, 067h, 0BBh, 0D1h, 067h, 057h
        db  0BCh, 0A6h, 0DDh, 006h, 0B5h, 03Fh, 04Bh, 036h, 0B2h, 048h, 0DAh, 02Bh, 00Dh, 0D8h, 04Ch, 01Bh
        db  00Ah, 0AFh, 0F6h, 04Ah, 003h, 036h, 060h, 07Ah, 004h, 041h, 0C3h, 0EFh, 060h, 0DFh, 055h, 0DFh
        db  067h, 0A8h, 0EFh, 08Eh, 06Eh, 031h, 079h, 0BEh, 069h, 046h, 08Ch, 0B3h, 061h, 0CBh, 01Ah, 083h
        db  066h, 0BCh, 0A0h, 0D2h, 06Fh, 025h, 036h, 0E2h, 068h, 052h, 095h, 077h, 00Ch, 0CCh, 003h, 047h
        db  00Bh, 0BBh, 0B9h, 016h, 002h, 022h, 02Fh, 026h, 005h, 055h, 0BEh, 03Bh, 0BAh, 0C5h, 028h, 00Bh
        db  0BDh, 0B2h, 092h, 05Ah, 0B4h, 02Bh, 004h, 06Ah, 0B3h, 05Ch, 0A7h, 0FFh, 0D7h, 0C2h, 031h, 0CFh
        db  0D0h, 0B5h, 08Bh, 09Eh, 0D9h, 02Ch, 01Dh, 0AEh, 0DEh, 05Bh, 0B0h, 0C2h, 064h, 09Bh, 026h, 0F2h
        db  063h, 0ECh, 09Ch, 0A3h, 06Ah, 075h, 00Ah, 093h, 06Dh, 002h, 0A9h, 006h, 009h, 09Ch, 03Fh, 036h
        db  00Eh, 0EBh, 085h, 067h, 007h, 072h, 013h, 057h, 000h, 005h, 082h, 04Ah, 0BFh, 095h, 014h, 07Ah
        db  0B8h, 0E2h, 0AEh, 02Bh, 0B1h, 07Bh, 038h, 01Bh, 0B6h, 00Ch, 09Bh, 08Eh, 0D2h, 092h, 00Dh, 0BEh
        db  0D5h, 0E5h, 0B7h, 0EFh, 0DCh, 07Ch, 021h, 0DFh, 0DBh, 00Bh, 0D4h, 0D2h, 0D3h, 086h, 042h, 0E2h
        db  0D4h, 0F1h, 0F8h, 0B3h, 0DDh, 068h, 06Eh, 083h, 0DAh, 01Fh, 0CDh, 016h, 0BEh, 081h, 05Bh, 026h
        db  0B9h, 0F6h, 0E1h, 077h, 0B0h, 06Fh, 077h, 047h, 0B7h, 018h, 0E6h, 05Ah, 008h, 088h, 070h, 06Ah
        db  00Fh, 0FFh, 0CAh, 03Bh, 006h, 066h, 05Ch, 00Bh, 001h, 011h, 0FFh, 09Eh, 065h, 08Fh, 069h, 0AEh
        db  062h, 0F8h, 0D3h, 0FFh, 06Bh, 061h, 045h, 0CFh, 06Ch, 016h, 078h, 0E2h, 00Ah, 0A0h, 0EEh, 0D2h
        db  00Dh, 0D7h, 054h, 083h, 004h, 04Eh, 0C2h, 0B3h, 003h, 039h, 061h, 026h, 067h, 0A7h, 0F7h, 016h
        db  060h, 0D0h, 04Dh, 047h, 069h, 049h, 0DBh, 077h, 06Eh, 03Eh, 04Ah, 06Ah, 0D1h, 0AEh, 0DCh, 05Ah
        db  0D6h, 0D9h, 066h, 00Bh, 0DFh, 040h, 0F0h, 03Bh, 0D8h, 037h, 053h, 0AEh, 0BCh, 0A9h, 0C5h, 09Eh
        db  0BBh, 0DEh, 07Fh, 0CFh, 0B2h, 047h, 0E9h, 0FFh, 0B5h, 030h, 01Ch, 0F2h, 0BDh, 0BDh, 08Ah, 0C2h
        db  0BAh, 0CAh, 030h, 093h, 0B3h, 053h, 0A6h, 0A3h, 0B4h, 024h, 005h, 036h, 0D0h, 0BAh, 093h, 006h
        db  0D7h, 0CDh, 029h, 057h, 0DEh, 054h, 0BFh, 067h, 0D9h, 023h, 02Eh, 07Ah, 066h, 0B3h, 0B8h, 04Ah
        db  061h, 0C4h, 002h, 01Bh, 068h, 05Dh, 094h, 02Bh, 06Fh, 02Ah, 037h, 0BEh, 00Bh, 0B4h, 0A1h, 08Eh
        db  00Ch, 0C3h, 01Bh, 0DFh, 005h, 05Ah, 08Dh, 0EFh, 002h, 02Dh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_B6F0:
        mov    esp, 0b6h
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
        add    byte ptr [edi], cl
        push   cs
        or     al, 8
        push   ecx
        test   dword ptr [ecx], 0f0703h
        add    al, byte ptr [esi]
        push   cs
        add    byte ptr [eax], al
        add    al, 0ch
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
loc_1_B748:
        dec    eax
        mov    bh, 0
        db  000h, 048h, 0B7h
        add    byte ptr [eax], al
loc_1_B750:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B754:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B758:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_B75C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B760:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B764:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B768:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B76C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B770:
        dec    esi
        dec    edi
        and    byte ptr [esi + 41h], cl
        dec    ebp
        inc    ebp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_B77B:
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
        add    byte ptr [edx], cl
        add    edx, dword ptr [eax]
        db  00Fh, 028h, 068h, 000h
        db  000h, 028h
        db  068h, 000h, 000h, 028h, 068h
        add    byte ptr [eax], al
        sub    byte ptr [eax], ch
        db  000h, 028h
        db  068h, 000h, 000h, 028h, 068h
        add    byte ptr [eax], al
        sub    byte ptr [eax], ch
        db  000h, 028h
        db  068h, 000h, 000h, 028h, 068h
        add    byte ptr [eax], al
        db  036h, 068h, 000h, 000h, 028h, 068h
        add    byte ptr [eax], al
        sub    byte ptr [eax], ch
        db  000h, 028h
        db  068h, 000h, 000h, 028h, 068h
        add    byte ptr [eax], al
        sub    byte ptr [eax], ch
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    eax, dword ptr [esi + eax]
        pop    ds
loc_1_B81D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B835:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], cl
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_B86F:
        cmp    edi, -1
        db  0FFh, 0FFh
loc_1_B874:
        hlt
        add    dword ptr [eax], eax
        add    al, ch
        add    eax, dword ptr [eax]
        db  000h
loc_1_B87C:
        rol    byte ptr [edi], 1
        add    byte ptr [eax], al
loc_1_B880:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B884:
        std
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B897:
        add    byte ptr [eax], al
        add    bl, bh
        jg     SHORT loc_1_B89D
loc_1_B89D:
        add    bl, bh
        jg     SHORT loc_1_B897
        inc    dword ptr [eax]
        add    dh, dh
        inc    dword ptr [eax]
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_B8C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_B8D0:
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
loc_1_B8F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B8F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B8FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B900:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B904:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        db  0F0h, 001h, 0CDh, 013h, 0CDh, 014h, 0CDh, 001h, 0CDh, 003h
loc_1_B914:
        inc    ecx
        cmp    al, byte ptr [eax]
        db  000h
loc_1_B918:
        db  018h, 0B9h, 000h, 000h, 018h, 0B9h
        add    byte ptr [eax], al
loc_1_B920:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
        db  068h, 046h, 000h, 000h, 002h
        stosb
        add    byte ptr [eax], al
        db  068h, 046h, 000h, 000h, 068h
        inc    esi
        add    byte ptr [eax], al
        db  068h, 046h, 000h, 000h, 068h
        inc    esi
        add    byte ptr [eax], al
        db  068h, 046h, 000h, 000h, 024h
        lodsb
        add    byte ptr [eax], al
loc_1_B944:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B948:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B94C:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 086h, 004h, 003h, 000h, 000h, 000h, 056h, 046h, 041h, 054h
        db  020h, 020h, 020h, 020h, 000h, 001h, 001h, 0A0h, 064h, 0B1h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 038h, 082h
        db  000h, 000h, 053h, 082h, 000h, 000h, 055h, 082h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_BD90:
        pop    ebx
        and    eax, 2d205d63h
        and    byte ptr [6e5c73h], ah
loc_1_BD9C:
        and    eax, 3f6e5c73h
        and    byte ptr [eax], al
        db  000h
loc_1_BDA4:
        and    eax, 6e5c63h
        add    byte ptr [eax], al
        db  000h
loc_1_BDAC:
        pop    edx
        db  065h, 072h, 06Fh
        and    byte ptr [esi + 69h], ah
        insb
        db  064h, 073h, 020h
        db  077h, 069h
        insb
        insb
        and    byte ptr [20656273h], ah
        imul   esi, dword ptr fs:[ebx + 70h], 6579616ch
        pop    esp
        outsb
        add    byte ptr [eax], al
loc_1_BDD0:
        push   ebx
        outsd
        db  072h, 072h
        db  079h, 02Ch
        and    byte ptr [ecx + 6fh], bh
        db  075h, 020h
        outsb
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        and    byte ptr [ebp + 62h], ah
        db  075h, 067h
        and    byte ptr [esi + 65h], dh
        db  072h, 073h
        db  069h, 06Fh, 06Eh, 02Eh, 000h, 000h
loc_1_BDF4:
        and    eax, 42435463h
        cmp    eax, 6c383025h
        db  078h, 020h
        and    byte ptr [eax + ecx*2 + 49h], dl
        inc    esp
        cmp    eax, 6c343025h
        js     SHORT loc_1_BE2C
        and    byte ptr [ebx + 6fh], al
        db  075h, 06Eh
        db  074h, 03Dh
        and    eax, 786c3830h
        pop    esp
        outsb
        add    byte ptr [eax], al
loc_1_BE1C:
        and    eax, 42435463h
        cmp    eax, 6c383025h
        js     SHORT loc_1_BE48
        and    byte ptr [eax + ecx*2 + 49h], dl
loc_1_BE2C:
        inc    esp
        cmp    eax, 6c343025h
        db  078h, 020h
        and    byte ptr [ebx + 6fh], al
        db  075h, 06Eh
        je     SHORT loc_1_BE78
        and    eax, 786c3830h
        pop    esp
        outsb
        add    byte ptr [eax], al
loc_1_BE44:
        inc    ebp
        js     SHORT loc_1_BEAA
        insb
loc_1_BE48:
        and    byte ptr [eax + ebp*2 + 72h], dh
        popad
        jae    NEAR PTR loc_1_BE71
        cmp    eax, 38302520h
        insb
        js     SHORT loc_1_BE79
        and    byte ptr [eax], ah
        dec    esi
        outsd
        outsb
        sub    eax, 6c637865h
        and    byte ptr [eax + ebp*2 + 72h], dh
        popad
        db  064h, 073h, 020h
        cmp    eax, 38302520h
loc_1_BE71:
        insb
        db  078h, 05Ch
        outsb
        add    byte ptr [eax], al
        db  000h
loc_1_BE78:
        dec    esi
loc_1_BE79:
        outsd
        and    byte ptr [eax + ebp*2 + 72h], dh
        popad
        jae    NEAR PTR loc_1_BEA3
        arpl   word ptr [ebp + 72h], si
        jb     SHORT loc_1_BEED
        outsb
        db  074h, 06Ch
        db  079h, 020h
        imul   ebp, dword ptr [esi + 20h], 54414656h
        pop    esp
        outsb
        add    byte ptr [eax], al
loc_1_BE98:
        push   esp
        dec    ecx
        inc    esp
        cmp    eax, 78343025h
        and    byte ptr [ebx + 42h], al
loc_1_BEA3:
        dec    esp
        dec    ebx
        cmp    eax, 78343025h
loc_1_BEAA:
        and    byte ptr [6e5c73h], ah
loc_1_BEB0:
        pop    esp
        outsb
        and    eax, 64412073h
        jb     NEAR PTR loc_1_BF1F
        db  073h, 073h
        and    byte ptr [38302520h], bh
        insb
        db  078h, 05Ch
        outsb
        add    byte ptr [eax], al
loc_1_BEC8:
        and    eax, 203d2073h
        and    eax, 786c3430h
        add    byte ptr [eax], al
loc_1_BED4:
        and    eax, 203d2073h
        and    eax, 786c3830h
        add    byte ptr [eax], al
loc_1_BEE0:
        and    eax, 203d2073h
        add    byte ptr [eax], al
        add    byte ptr [786c3230h], ah
loc_1_BEED:
        add    byte ptr [eax], al
        db  000h
loc_1_BEF0:
        and    eax, 203d2073h
        daa
        add    byte ptr [eax], al
        db  025h, 063h, 000h, 000h
loc_1_BEFC:
        daa
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [esi + ebp*2], bl
        add    byte ptr [esi + ebp*2], bl
        add    byte ptr [eax + eax], bl
        add    byte ptr [edx], ch
        add    byte ptr [eax], al
        add    byte ptr [73312eh], ah
        add    byte ptr [eax], al
        add    byte ptr [esi + ebp*2], bl
loc_1_BF1F:
        add    byte ptr [ecx + 72h], ch
        pop    edi
        db  070h, 070h
        popad
        db  074h, 068h
        db  020h, 03Dh, 020h, 000h
loc_1_BF2C:
        pop    esp
        outsb
        add    byte ptr [eax], al
loc_1_BF30:
        dec    edi
        db  066h, 074h, 05Fh
        dec    esi
        popad
        insd
        and    byte ptr gs:[73252720h], bh
        daa
        pop    esp
        outsb
        add    byte ptr [eax], al
        add    byte ptr [edi + 66h], cl
        db  074h, 05Fh
        inc    esp
        db  072h, 069h
        db  076h, 065h
        and    byte ptr [3a632520h], bh
        pop    esp
        outsb
        add    byte ptr [eax], al
        add    byte ptr [edi + 66h], cl
        db  074h, 05Fh
        dec    esi
        popad
        insd
        and    byte ptr gs:[73252720h], bh
        daa
        pop    esp
        outsb
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
