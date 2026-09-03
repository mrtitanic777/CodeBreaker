; bios_pcode_02.asm -- BIOS object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

BIOS_PM_API:
        push   ebp
        call   sub_2_26FC
        pop    ecx
        ret
        db  055h, 0A1h, 000h, 000h, 000h, 000h, 0C1h, 0E8h, 00Ch, 08Bh, 0ECh, 083h, 0ECh, 004h, 056h, 06Ah
        db  000h, 06Ah, 004h, 050h, 0E8h, 09Bh, 032h, 000h, 000h, 083h, 0C4h, 00Ch, 085h, 0C0h, 075h, 00Ah
        db  0BEh, 0FEh, 000h, 000h, 000h, 0E9h, 0B8h, 000h, 000h, 000h, 033h, 0C9h, 033h, 0D2h, 0CDh, 020h
        db  000h, 000h, 001h, 000h, 066h, 089h, 045h, 0FCh, 066h, 081h, 07Dh, 0FCh, 00Ah, 004h, 072h, 023h
        db  06Ah, 000h, 08Dh, 04Dh, 00Ch, 0FFh, 075h, 008h, 051h, 068h, 000h, 000h, 000h, 000h, 06Ah, 000h
        db  0E8h, 04Fh, 032h, 000h, 000h, 083h, 0C4h, 014h, 085h, 0C0h, 074h, 007h, 0BEh, 0FEh, 000h, 000h
        db  000h, 0EBh, 06Ah, 033h, 0C9h, 08Dh, 045h, 00Ch, 051h, 068h, 0FEh, 000h, 000h, 000h, 050h, 08Bh
        db  045h, 008h, 040h, 0D1h, 0E8h, 050h, 051h, 00Fh, 0B7h, 015h, 000h, 000h, 000h, 000h, 051h, 00Fh
        db  0B7h, 005h, 000h, 000h, 000h, 000h, 00Fh, 0B7h, 00Dh, 000h, 000h, 000h, 000h, 051h, 068h, 000h
        db  040h, 000h, 000h, 052h, 050h, 0E8h, 012h, 032h, 000h, 000h, 083h, 0C4h, 028h, 08Bh, 0F0h, 033h
        db  0C9h, 033h, 0D2h, 0CDh, 020h, 000h, 000h, 001h, 000h, 066h, 089h, 045h, 0FEh, 066h, 081h, 07Dh
        db  0FEh, 00Ah, 004h, 072h, 018h, 06Ah, 000h, 08Dh, 04Dh, 00Ch, 0FFh, 075h, 008h, 051h, 068h, 000h
        db  000h, 000h, 000h, 06Ah, 003h, 0E8h, 0DAh, 031h, 000h, 000h, 083h, 0C4h, 014h, 06Ah, 000h, 0A1h
        db  000h, 000h, 000h, 000h, 0C1h, 0E8h, 00Ch, 06Ah, 004h, 050h, 0E8h, 0BDh, 031h, 000h, 000h, 083h
        db  0C4h, 00Ch, 08Bh, 0C6h, 05Eh, 08Bh, 0E5h, 05Dh, 0C2h, 004h, 000h
sub_2_00F3:
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        shl    eax, 10h
        push   ebp
        mov    ebp, esp
        inc    eax
        sub    esp, 4
        mov    dword ptr [ebp - 4], eax
        db  066h, 0FFh, 035h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        push   3
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_0125
        db  0CDh, 020h, 002h, 000h, 029h, 000h
        jmp    SHORT loc_2_012F
loc_2_0125:
        push   8
        call   8
        add    esp, 8
loc_2_012F:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0133:
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        shl    eax, 10h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        lea    ecx, [eax + 2]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], ecx
        db  066h, 0FFh, 035h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_016D
        db  0CDh, 020h, 002h, 000h, 029h, 000h
        jmp    SHORT loc_2_0177
loc_2_016D:
        push   0ch
        call   8
        add    esp, 0ch
loc_2_0177:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_017B:
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        shl    eax, 10h
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    word ptr [ebp - 4], dx
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    byte ptr [edx], cl
        lea    ecx, [eax + 0dh]
        mov    dword ptr [ebp - 0ch], ecx
        mov    dword ptr [ebp - 8], eax
        push   0
        db  066h, 0FFh, 035h, 000h, 000h, 000h, 000h
        push   word ptr [ebp - 4]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        push   1
        xor    eax, eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_01CA
        db  0CDh, 020h, 002h, 000h, 029h, 000h
        jmp    SHORT loc_2_01DA
loc_2_01CA:
        push   10h
        call   8
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    esp, 10h
loc_2_01DA:
        mov    word ptr [ebp - 2], ax
        cmp    dword ptr [edx + 10h], 6081d041h
        jne    SHORT loc_2_01FF
        add    edx, 19h
        cmp    byte ptr [edx], 22h
        jne    SHORT loc_2_01FF
        cmp    byte ptr [edx + 3], 47h
        jne    SHORT loc_2_01FF
        cmp    byte ptr [edx + 0ah], 10h
        jne    SHORT loc_2_01FF
        mov    byte ptr [edx + 0ah], 20h
loc_2_01FF:
        mov    ax, word ptr [ebp - 2]
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0207:
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        shl    eax, 10h
        push   ebp
        mov    ebp, esp
        add    eax, 0dh
        sub    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    word ptr [ebp - 4], dx
        mov    word ptr [ebp - 2], cx
        db  066h, 0FFh, 035h, 000h, 000h, 000h, 000h
        push   word ptr [ebp - 4]
        push   dword ptr [ebp - 8]
        push   word ptr [ebp - 2]
        push   2
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_024B
        db  0CDh, 020h, 002h, 000h, 029h, 000h
        jmp    SHORT loc_2_0255
loc_2_024B:
        push   0ch
        call   8
        add    esp, 0ch
loc_2_0255:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0259:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    word ptr [ebp - 2], cx
        push   0
        db  066h, 0FFh, 035h, 000h, 000h, 000h, 000h
        push   word ptr [ebp - 2]
        push   4
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_0285
        db  0CDh, 020h, 002h, 000h, 029h, 000h
        jmp    SHORT loc_2_028F
loc_2_0285:
        push   8
        call   8
        add    esp, 8
loc_2_028F:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0293:
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        shl    eax, 10h
        push   ebp
        mov    ebp, esp
        add    eax, 3
        sub    esp, 4
        mov    dword ptr [ebp - 4], eax
        db  066h, 0FFh, 035h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        push   5
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_02C7
        db  0CDh, 020h, 002h, 000h, 029h, 000h
        jmp    SHORT loc_2_02D1
loc_2_02C7:
        push   8
        call   8
        add    esp, 8
loc_2_02D1:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_02D5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], ecx
        db  066h, 0FFh, 035h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   0bh
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_0302
        db  0CDh, 020h, 002h, 000h, 029h, 000h
        jmp    SHORT loc_2_030C
loc_2_0302:
        push   0ch
        call   8
        add    esp, 0ch
loc_2_030C:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0310:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], ecx
        db  066h, 0FFh, 035h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        push   9
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_0337
        db  0CDh, 020h, 002h, 000h, 029h, 000h
        jmp    SHORT loc_2_0341
loc_2_0337:
        push   8
        call   8
        add    esp, 8
loc_2_0341:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0345:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], ecx
        db  066h, 0FFh, 035h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        push   0ah
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_036C
        db  0CDh, 020h, 002h, 000h, 029h, 000h
        jmp    SHORT loc_2_0376
loc_2_036C:
        push   8
        call   8
        add    esp, 8
loc_2_0376:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_037A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], ecx
        db  066h, 0FFh, 035h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   41h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_03AA
        db  0CDh, 020h, 002h, 000h, 029h, 000h
        jmp    SHORT loc_2_03B4
loc_2_03AA:
        push   10h
        call   8
        add    esp, 10h
loc_2_03B4:
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_03BA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    word ptr [ebp - 2], dx
        mov    dword ptr [ebp - 8], ecx
        push   0
        db  066h, 0FFh, 035h, 000h, 000h, 000h, 000h
        push   word ptr [ebp - 2]
        push   dword ptr [ebp - 8]
        push   42h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_03EC
        db  0CDh, 020h, 002h, 000h, 029h, 000h
        jmp    SHORT loc_2_03F6
loc_2_03EC:
        push   0ch
        call   8
        add    esp, 0ch
loc_2_03F6:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_03FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    word ptr [ebp - 2], dx
        mov    dword ptr [ebp - 8], ecx
        push   0
        db  066h, 0FFh, 035h, 000h, 000h, 000h, 000h
        push   word ptr [ebp - 2]
        push   dword ptr [ebp - 8]
        push   43h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_042C
        db  0CDh, 020h, 002h, 000h, 029h, 000h
        jmp    SHORT loc_2_0436
loc_2_042C:
        push   0ch
        call   8
        add    esp, 0ch
loc_2_0436:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_043A:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    dl, byte ptr [esi]
        test   dl, dl
        jne    SHORT loc_2_044D
        xor    eax, eax
        jmp    NEAR PTR loc_2_04EA
loc_2_044D:
        mov    cl, byte ptr [esi + 1]
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        xor    ebx, ebx
        and    eax, 1fh
        mov    bl, cl
        and    ebx, 7
        shl    ebx, 5
        or     eax, ebx
        xor    ebx, ebx
        shl    eax, 6
        mov    bl, dl
        and    ebx, 1fh
        and    cl, 18h
        shr    cl, 3
        or     eax, ebx
        movsx  edx, cl
        shl    eax, 2
        mov    cl, byte ptr [esi + 3]
        or     eax, edx
        cmp    cl, 39h
        movsx  ecx, cl
        jle    SHORT loc_2_048E
        sub    ecx, 37h
        jmp    SHORT loc_2_0491
loc_2_048E:
        sub    ecx, 30h
loc_2_0491:
        movzx  ecx, cl
        shl    ecx, 14h
        or     eax, ecx
        mov    cl, byte ptr [esi + 4]
        cmp    cl, 39h
        movsx  ecx, cl
        jle    SHORT loc_2_04A9
        sub    ecx, 37h
        jmp    SHORT loc_2_04AC
loc_2_04A9:
        sub    ecx, 30h
loc_2_04AC:
        movzx  ecx, cl
        shl    ecx, 10h
        or     eax, ecx
        mov    cl, byte ptr [esi + 5]
        cmp    cl, 39h
        movsx  ecx, cl
        jle    SHORT loc_2_04C4
        sub    ecx, 37h
        jmp    SHORT loc_2_04C7
loc_2_04C4:
        sub    ecx, 30h
loc_2_04C7:
        movzx  ecx, cl
        shl    ecx, 1ch
        or     eax, ecx
        mov    cl, byte ptr [esi + 6]
        cmp    cl, 39h
        movsx  ecx, cl
        jle    SHORT loc_2_04DF
        sub    ecx, 37h
        jmp    SHORT loc_2_04E2
loc_2_04DF:
        sub    ecx, 30h
loc_2_04E2:
        movzx  ecx, cl
        shl    ecx, 18h
        or     eax, ecx
loc_2_04EA:
        pop    esi
        pop    ebx
        ret
sub_2_04ED:
        push   esi
        xor    eax, eax
        mov    edx, dword ptr [esp + 8]
        push   edi
        cmp    byte ptr [edx], al
        je     SHORT loc_2_0534
        mov    esi, dword ptr [esp + 10h]
        cmp    esi, 8
        jle    SHORT loc_2_0507
        mov    esi, 8
loc_2_0507:
        cmp    esi, 1
        jge    SHORT loc_2_0511
        mov    esi, 1
loc_2_0511:
        xor    edi, edi
        test   esi, esi
        jle    SHORT loc_2_0534
loc_2_0517:
        shl    eax, 4
        mov    cl, byte ptr [edx + edi]
        cmp    cl, 39h
        movsx  ecx, cl
        jle    SHORT loc_2_052A
        sub    ecx, 37h
        jmp    SHORT loc_2_052D
loc_2_052A:
        sub    ecx, 30h
loc_2_052D:
        add    eax, ecx
        inc    edi
        cmp    edi, esi
        jl     SHORT loc_2_0517
loc_2_0534:
        pop    edi
        pop    esi
        ret
sub_2_0537:
        mov    ax, word ptr [esp + 0ch]
        push   esi
        test   ax, ax
        push   edi
        je     SHORT loc_2_055C
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        movzx  eax, ax
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_2_055C:
        pop    edi
        pop    esi
        ret
sub_2_055F:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        xor    esi, esi
        cmp    eax, esi
        je     SHORT loc_2_0578
loc_2_056B:
        mov    bl, byte ptr [edx + esi]
        cmp    byte ptr [ecx + esi], bl
        jne    SHORT loc_2_0582
        inc    esi
        cmp    esi, eax
        jb     SHORT loc_2_056B
loc_2_0578:
        mov    eax, 1
loc_2_057D:
        pop    esi
        pop    ebx
        ret    4
loc_2_0582:
        xor    eax, eax
        jmp    SHORT loc_2_057D
sub_2_0586:
        test   ecx, ecx
        jne    SHORT loc_2_058E
        mov    eax, ecx
        jmp    SHORT loc_2_05AA
loc_2_058E:
        cmp    byte ptr [ecx], 0
        je     SHORT loc_2_059D
loc_2_0593:
        cmp    byte ptr [ecx], dl
        je     SHORT loc_2_059D
        inc    ecx
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_2_0593
loc_2_059D:
        mov    eax, 0
        cmp    byte ptr [ecx], 1
        adc    eax, -1
        and    eax, ecx
loc_2_05AA:
        ret
sub_2_05AB:
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        test   esi, esi
        je     SHORT loc_2_05E3
        cmp    dword ptr [esp + 0ch], 1
        jne    SHORT loc_2_05C7
        mov    dl, byte ptr [edi]
        mov    ecx, esi
        call   sub_2_0586
        jmp    SHORT loc_2_05E5
loc_2_05C7:
        cmp    byte ptr [esi], 0
        je     SHORT loc_2_05E3
loc_2_05CC:
        mov    edx, edi
        mov    ecx, esi
        push   dword ptr [esp + 0ch]
        call   sub_2_055F
        test   eax, eax
        jne    SHORT loc_2_05E3
        inc    esi
        cmp    byte ptr [esi], 0
        jne    SHORT loc_2_05CC
loc_2_05E3:
        mov    eax, esi
loc_2_05E5:
        pop    edi
        pop    esi
        ret    4
sub_2_05EA:
        push   ebp
        mov    eax, ecx
        mov    ebp, esp
        mov    ah, al
        sub    esp, 0ch
        mov    al, ch
        mov    word ptr [ebp - 2], ax
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 0ch], 1
        push   ebx
        push   esi
        db  0BEh, 006h, 000h, 000h, 000h
loc_2_060B:
        mov    bx, word ptr [ebp - 2]
        mov    al, 3
        sub    al, byte ptr [ebp - 0ch]
        mov    cl, 5
        imul   cl
        mov    cl, al
        inc    dword ptr [ebp - 0ch]
        shr    bx, cl
        and    bl, 1fh
        add    bl, 40h
        mov    byte ptr [esi], bl
        inc    esi
        cmp    dword ptr [ebp - 0ch], 3
        jbe    SHORT loc_2_060B
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        movzx  edx, dl
        shr    ecx, 18h
        push   edx
        shr    eax, 8
        and    ax, 0ff00h
        or     cx, ax
        movzx  eax, cx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_2_32C4
        add    esp, 10h
        db  0B8h, 000h, 000h, 000h, 000h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0663:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        mov    dword ptr [ebp - 0ch], ecx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, edx
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        push   ebx
        push   ecx
        lea    ecx, [ebp - 0ch]
        push   ecx
        call   sub_2_32CC
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_2_08DF
loc_2_068E:
        mov    edx, dword ptr [ebp - 0ch]
loc_2_0691:
        mov    edi, 1
        mov    al, byte ptr [esi]
        test   al, 80h
        je     SHORT loc_2_06AB
        mov    byte ptr [ebp - 1], al
        movzx  eax, word ptr [esi + 1]
        add    eax, 3
        mov    dword ptr [ebp - 10h], eax
        jmp    SHORT loc_2_06BE
loc_2_06AB:
        mov    cl, al
        and    cl, 0f8h
        mov    byte ptr [ebp - 1], cl
        xor    ecx, ecx
        mov    cl, al
        and    ecx, 7
        inc    ecx
        mov    dword ptr [ebp - 10h], ecx
loc_2_06BE:
        mov    ecx, dword ptr [ebp - 8]
        and    byte ptr [ebp - 7], 7fh
        and    ecx, 8000h
        shr    ecx, 0fh
        test   ecx, ecx
        je     SHORT loc_2_06DE
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_08F1
loc_2_06DE:
        movzx  eax, byte ptr [ebp - 1]
        cmp    eax, 28h
        jg     SHORT loc_2_06F0
        je     SHORT loc_2_075D
        cmp    eax, 20h
        je     SHORT loc_2_0733
        jmp    SHORT loc_2_072C
loc_2_06F0:
        cmp    eax, 40h
        je     NEAR PTR loc_2_0782
        cmp    eax, 48h
        je     NEAR PTR loc_2_07CD
        cmp    eax, 78h
        je     NEAR PTR loc_2_0905
        cmp    eax, 81h
        je     NEAR PTR loc_2_0806
        cmp    eax, 85h
        je     NEAR PTR loc_2_0852
        cmp    eax, 86h
        je     NEAR PTR loc_2_088F
loc_2_072C:
        xor    edi, edi
        jmp    NEAR PTR loc_2_08B7
loc_2_0733:
        cmp    dword ptr [ebp - 8], 4
        jne    NEAR PTR loc_2_08F5
        test   ecx, ecx
        je     SHORT loc_2_074A
        mov    word ptr [esi + 1], bx
        jmp    NEAR PTR loc_2_08B7
loc_2_074A:
        mov    ax, 1
        mov    cl, byte ptr [edx + 2]
        shl    ax, cl
        mov    word ptr [esi + 1], ax
        jmp    NEAR PTR loc_2_08B7
loc_2_075D:
        cmp    dword ptr [ebp - 8], 3
        jne    NEAR PTR loc_2_08F9
        test   ecx, ecx
        je     SHORT loc_2_0773
        mov    byte ptr [esi + 1], bl
        jmp    NEAR PTR loc_2_08B7
loc_2_0773:
        mov    al, 1
        mov    cl, byte ptr [edx + 1]
        shl    al, cl
        mov    byte ptr [esi + 1], al
        jmp    NEAR PTR loc_2_08B7
loc_2_0782:
        cmp    dword ptr [ebp - 8], 2
        jne    NEAR PTR loc_2_08FD
        test   ecx, ecx
        je     SHORT loc_2_0796
        mov    word ptr [esi + 2], bx
        jmp    SHORT loc_2_079E
loc_2_0796:
        mov    ax, word ptr [edx + 4]
        mov    word ptr [esi + 2], ax
loc_2_079E:
        test   ecx, ecx
        je     SHORT loc_2_07A8
        mov    word ptr [esi + 4], bx
        jmp    SHORT loc_2_07B0
loc_2_07A8:
        mov    ax, word ptr [edx + 4]
        mov    word ptr [esi + 4], ax
loc_2_07B0:
        test   ecx, ecx
        je     SHORT loc_2_07BC
        mov    byte ptr [esi + 7], bl
        jmp    NEAR PTR loc_2_08B7
loc_2_07BC:
        mov    ax, word ptr [edx + 6]
        sub    al, byte ptr [edx + 4]
        inc    al
        mov    byte ptr [esi + 7], al
        jmp    NEAR PTR loc_2_08B7
loc_2_07CD:
        cmp    dword ptr [ebp - 8], 2
        jne    NEAR PTR loc_2_0901
        test   ecx, ecx
        je     SHORT loc_2_07E1
        mov    word ptr [esi + 1], bx
        jmp    SHORT loc_2_07E9
loc_2_07E1:
        mov    ax, word ptr [edx + 4]
        mov    word ptr [esi + 1], ax
loc_2_07E9:
        test   ecx, ecx
        je     SHORT loc_2_07F5
        mov    byte ptr [esi + 3], bl
        jmp    NEAR PTR loc_2_08B7
loc_2_07F5:
        mov    ax, word ptr [edx + 6]
        sub    al, byte ptr [edx + 4]
        inc    al
        mov    byte ptr [esi + 3], al
        jmp    NEAR PTR loc_2_08B7
loc_2_0806:
        cmp    dword ptr [ebp - 8], 1
        jne    NEAR PTR loc_2_0909
        test   ecx, ecx
        je     SHORT loc_2_081A
        mov    word ptr [esi + 4], bx
        jmp    SHORT loc_2_0824
loc_2_081A:
        mov    eax, dword ptr [edx + 4]
        shr    eax, 8
        mov    word ptr [esi + 4], ax
loc_2_0824:
        test   ecx, ecx
        je     SHORT loc_2_082E
        mov    word ptr [esi + 6], bx
        jmp    SHORT loc_2_0838
loc_2_082E:
        mov    eax, dword ptr [edx + 4]
        shr    eax, 8
        mov    word ptr [esi + 6], ax
loc_2_0838:
        test   ecx, ecx
        je     SHORT loc_2_0842
        mov    word ptr [esi + 0ah], bx
        jmp    SHORT loc_2_08B7
loc_2_0842:
        mov    eax, dword ptr [edx + 8]
        sub    eax, dword ptr [edx + 4]
        inc    eax
        shr    eax, 8
        mov    word ptr [esi + 0ah], ax
        jmp    SHORT loc_2_08B7
loc_2_0852:
        cmp    dword ptr [ebp - 8], 1
        jne    NEAR PTR loc_2_090D
        test   ecx, ecx
        je     SHORT loc_2_0865
        mov    dword ptr [esi + 4], ebx
        jmp    SHORT loc_2_086B
loc_2_0865:
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], eax
loc_2_086B:
        test   ecx, ecx
        je     SHORT loc_2_0874
        mov    dword ptr [esi + 8], ebx
        jmp    SHORT loc_2_087A
loc_2_0874:
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 8], eax
loc_2_087A:
        test   ecx, ecx
        je     SHORT loc_2_0883
        mov    dword ptr [esi + 10h], ebx
        jmp    SHORT loc_2_08B7
loc_2_0883:
        mov    eax, dword ptr [edx + 8]
        sub    eax, dword ptr [edx + 4]
        inc    eax
        mov    dword ptr [esi + 10h], eax
        jmp    SHORT loc_2_08B7
loc_2_088F:
        cmp    dword ptr [ebp - 8], 1
        jne    SHORT loc_2_0911
        test   ecx, ecx
        je     SHORT loc_2_089E
        mov    dword ptr [esi + 4], ebx
        jmp    SHORT loc_2_08A4
loc_2_089E:
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], eax
loc_2_08A4:
        test   ecx, ecx
        je     SHORT loc_2_08AD
        mov    dword ptr [esi + 8], ebx
        jmp    SHORT loc_2_08B7
loc_2_08AD:
        mov    eax, dword ptr [edx + 8]
        sub    eax, dword ptr [edx + 4]
        inc    eax
        mov    dword ptr [esi + 8], eax
loc_2_08B7:
        add    esi, dword ptr [ebp - 10h]
        test   edi, edi
        je     NEAR PTR loc_2_0691
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 0ch]
        push   ebx
        push   dword ptr [ebp - 0ch]
        push   edx
        call   sub_2_32CC
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_2_068E
loc_2_08DF:
        mov    dl, byte ptr [esi]
        test   dl, 80h
        mov    al, dl
        je     SHORT loc_2_0915
        movzx  ecx, word ptr [esi + 1]
        add    ecx, 3
        jmp    SHORT loc_2_091F
loc_2_08F1:
        xor    eax, eax
        jmp    SHORT loc_2_0961
loc_2_08F5:
        xor    eax, eax
        jmp    SHORT loc_2_0961
loc_2_08F9:
        xor    eax, eax
        jmp    SHORT loc_2_0961
loc_2_08FD:
        xor    eax, eax
        jmp    SHORT loc_2_0961
loc_2_0901:
        xor    eax, eax
        jmp    SHORT loc_2_0961
loc_2_0905:
        xor    eax, eax
        jmp    SHORT loc_2_0961
loc_2_0909:
        xor    eax, eax
        jmp    SHORT loc_2_0961
loc_2_090D:
        xor    eax, eax
        jmp    SHORT loc_2_0961
loc_2_0911:
        xor    eax, eax
        jmp    SHORT loc_2_0961
loc_2_0915:
        and    al, 0f8h
        xor    ecx, ecx
        mov    cl, dl
        and    ecx, 7
        inc    ecx
loc_2_091F:
        movzx  eax, al
        cmp    eax, 28h
        jg     SHORT loc_2_0930
        je     SHORT loc_2_0958
        cmp    eax, 20h
        je     SHORT loc_2_0958
        jmp    SHORT loc_2_0954
loc_2_0930:
        cmp    eax, 40h
        je     SHORT loc_2_0958
        cmp    eax, 48h
        je     SHORT loc_2_0958
        cmp    eax, 78h
        je     SHORT loc_2_095C
        cmp    eax, 81h
        je     SHORT loc_2_0958
        cmp    eax, 85h
        jl     SHORT loc_2_0954
        cmp    eax, 86h
        jle    SHORT loc_2_0958
loc_2_0954:
        add    esi, ecx
        jmp    SHORT loc_2_08DF
loc_2_0958:
        xor    eax, eax
        jmp    SHORT loc_2_0961
loc_2_095C:
        mov    eax, 1
loc_2_0961:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0968:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    dword ptr [ebp - 8], ecx
        push   esi
        xor    ebx, ebx
        mov    word ptr [edx], bx
        push   edi
        mov    word ptr [edx + 5ch], bx
        lea    edi, [edx + 0aeh]
        mov    word ptr [edx + 0beh], bx
        lea    eax, [edx + 2]
        mov    word ptr [edi], bx
        mov    esi, edx
        mov    dword ptr [ebp - 4], 9
        lea    edx, [esi + 4ah]
loc_2_099C:
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 24h], ebx
        add    eax, 4
        mov    word ptr [edx], bx
        add    edx, 2
        dec    dword ptr [ebp - 4]
        jne    SHORT loc_2_099C
        lea    eax, [esi + 5eh]
        mov    edx, 14h
        xor    ebx, ebx
loc_2_09B9:
        mov    word ptr [eax], bx
        add    eax, 2
        mov    word ptr [eax + 26h], bx
        dec    edx
        jne    SHORT loc_2_09B9
        xor    edx, edx
loc_2_09C8:
        mov    byte ptr [esi + edx + 0b0h], 0
        mov    byte ptr [esi + edx + 0b7h], 0
        inc    edx
        cmp    edx, 7
        jl     SHORT loc_2_09C8
        xor    edx, edx
        xor    eax, eax
loc_2_09E2:
        mov    byte ptr [esi + edx + 0c0h], al
        inc    edx
        mov    word ptr [esi + edx*2 + 0c5h], ax
        cmp    edx, 7
        jl     SHORT loc_2_09E2
        xor    ebx, ebx
        lea    eax, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   eax
        push   ebx
        push   ecx
        push   edx
        call   sub_2_32CC
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_2_0B05
loc_2_0A14:
        test   byte ptr [ebp - 3], 80h
        mov    ecx, dword ptr [ebp - 8]
        jne    NEAR PTR loc_2_0AE8
        cmp    dword ptr [ebp - 4], 1
        jne    SHORT loc_2_0A52
        movzx  edx, word ptr [esi]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + edx*4 + 2], eax
        mov    edx, dword ptr [ecx + 8]
        sub    edx, eax
        movzx  eax, word ptr [esi]
        inc    edx
        mov    dword ptr [esi + eax*4 + 26h], edx
        mov    dx, word ptr [ecx + 0ch]
        movzx  ecx, word ptr [esi]
        mov    word ptr [esi + ecx*2 + 4ah], dx
        inc    word ptr [esi]
        jmp    NEAR PTR loc_2_0AE8
loc_2_0A52:
        cmp    dword ptr [ebp - 4], 2
        jne    SHORT loc_2_0A80
        mov    ax, word ptr [ecx + 4]
        movzx  edx, word ptr [esi + 5ch]
        mov    cx, word ptr [ecx + 6]
        sub    cx, ax
        mov    word ptr [esi + edx*2 + 5eh], ax
        movzx  eax, word ptr [esi + 5ch]
        inc    cx
        mov    word ptr [esi + eax*2 + 86h], cx
        inc    word ptr [esi + 5ch]
        jmp    SHORT loc_2_0AE8
loc_2_0A80:
        cmp    dword ptr [ebp - 4], 3
        jne    SHORT loc_2_0AAF
        mov    al, byte ptr [ecx + 1]
        movzx  ecx, word ptr [esi + 0beh]
        mov    byte ptr [ecx + esi + 0c0h], al
        movzx  edx, word ptr [esi + 0beh]
        mov    word ptr [esi + edx*2 + 0c7h], bx
        inc    word ptr [esi + 0beh]
        jmp    SHORT loc_2_0AE8
loc_2_0AAF:
        cmp    dword ptr [ebp - 4], 4
        jne    SHORT loc_2_0AE8
        movzx  edx, word ptr [edi]
        mov    al, byte ptr [ecx + 2]
        mov    byte ptr [edx + esi + 0b0h], al
        mov    al, byte ptr [ecx]
        movzx  ecx, word ptr [edi]
        and    al, 1
        mov    byte ptr [ecx + esi + 0b7h], al
        mov    al, byte ptr [ebp + 8]
        movzx  edx, word ptr [edi]
        sub    al, 4
        cmp    al, 1
        sbb    al, al
        and    al, 2
        or     byte ptr [edx + esi + 0b7h], al
        inc    word ptr [edi]
loc_2_0AE8:
        push   ebx
        lea    eax, [ebp - 4]
        push   eax
        lea    edx, [ebp - 8]
        push   ebx
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_2_32CC
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_2_0A14
loc_2_0B05:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_0B0E:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, 100h
        mov    ebx, 80h
loc_2_0B1D:
        mov    al, byte ptr [ecx]
        mov    dl, al
        and    dl, bl
        mov    byte ptr [esp + 8], dl
        cmp    dl, bl
        jne    SHORT loc_2_0B34
        movzx  edx, word ptr [ecx + 1]
        add    edx, 3
        jmp    SHORT loc_2_0B3C
loc_2_0B34:
        xor    edx, edx
        mov    dl, al
        and    edx, 7
        inc    edx
loc_2_0B3C:
        add    ecx, edx
        cmp    byte ptr [esp + 8], bl
        je     SHORT loc_2_0B4A
        and    al, 0f8h
        cmp    al, 78h
        je     SHORT loc_2_0B4D
loc_2_0B4A:
        dec    esi
        jne    SHORT loc_2_0B1D
loc_2_0B4D:
        mov    eax, 0
        cmp    esi, 1
        adc    eax, -1
        pop    esi
        and    eax, ecx
        pop    ebx
        add    esp, 4
        ret
sub_2_0B60:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        mov    esi, ecx
        lea    edx, [ebp - 4]
        mov    dword ptr [ebp - 8], 4
        xor    ecx, ecx
        push   ecx
        mov    dword ptr [ebp - 4], ecx
        push   eax
        push   edx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   esi
        call   sub_2_32D4
        add    esp, 1ch
        mov    eax, 1
        test   byte ptr [ebp - 4], 1
        jne    SHORT loc_2_0BCA
        xor    ecx, ecx
        push   200h
        lea    eax, [ebp - 8]
        lea    edx, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [ebp - 8], 4
        push   edx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   esi
        call   sub_2_32D4
        add    esp, 1ch
        mov    eax, dword ptr [ebp - 4]
        and    eax, 1
loc_2_0BCA:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0BCF:
        push   esi
        mov    esi, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 19h
        call   sub_2_0B0E
        test   eax, eax
        je     SHORT loc_2_0BFD
        mov    ecx, eax
        call   sub_2_0B0E
        test   eax, eax
        je     SHORT loc_2_0BFD
        push   1
        push   0
        push   esi
        push   eax
        call   sub_2_32DC
        add    esp, 10h
loc_2_0BFD:
        pop    esi
        ret
sub_2_0BFF:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        call   sub_2_0133
        test   ax, ax
        jne    NEAR PTR loc_2_0EBC
        db  0A1h, 000h, 000h, 000h, 000h
        movzx  esi, word ptr [eax + 2]
        db  039h, 035h, 000h, 000h, 000h, 000h
        jae    SHORT loc_2_0C5F
        lea    edi, [esi*2 + 19h]
        push   202h
        push   edi
        push   eax
        call   sub_2_332C
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_2_0EBC
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   edi
        mov    edx, eax
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_2_2697
loc_2_0C5F:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        xor    bl, bl
        xor    esi, esi
        call   sub_2_3324
        add    esp, 8
        mov    edi, 1
loc_2_0C78:
        mov    edx, edi
        mov    ecx, ebx
        call   sub_2_017B
        test   ax, ax
        db  0A1h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_0EAC
        mov    edx, ebx
        mov    ecx, dword ptr [eax + 10h]
        call   sub_2_05EA
        push   esi
        lea    edx, [ebp - 8]
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   eax
        push   edx
        call   sub_2_331C
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_0E9C
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        movzx  eax, word ptr [eax + 17h]
        movzx  ecx, bl
        shl    eax, 10h
        add    eax, ecx
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_2_3314
        add    esp, 10h
        mov    byte ptr [ebp - 1], bl
        lea    ebx, [ebp - 1]
        push   esi
        push   ebx
        push   edi
        push   40h
        push   dword ptr [ebp - 8]
        call   sub_2_330C
        add    esp, 14h
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 19h
        push   2
        push   esi
        push   dword ptr [ebp - 8]
        push   eax
        call   sub_2_32DC
        add    esp, 10h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 19h
        call   sub_2_0B0E
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_0D31
        mov    cl, byte ptr [eax]
        test   cl, 80h
        jne    SHORT loc_2_0D2D
        and    cl, 0f8h
        cmp    cl, 78h
        je     SHORT loc_2_0D31
loc_2_0D2D:
        mov    ebx, edi
        jmp    SHORT loc_2_0D33
loc_2_0D31:
        xor    ebx, ebx
loc_2_0D33:
        test   ebx, ebx
        je     SHORT loc_2_0D42
        test   byte ptr [edx + 17h], 2
        mov    ecx, 0
        je     SHORT loc_2_0D47
loc_2_0D42:
        mov    ecx, 3
loc_2_0D47:
        test   ebx, ebx
        jne    SHORT loc_2_0D4E
        lea    eax, [edx + 19h]
loc_2_0D4E:
        push   ecx
        push   esi
        push   dword ptr [ebp - 8]
        push   eax
        call   sub_2_32DC
        add    esp, 10h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + 10h]
        cmp    eax, 30ed041h
        jne    SHORT loc_2_0D7D
        push   esi
        push   dword ptr [ebp - 8]
        call   sub_2_3304
        add    esp, 8
        jmp    NEAR PTR loc_2_0E9C
loc_2_0D7D:
        cmp    eax, 6d041h
        jne    SHORT loc_2_0DFF
        push   3
        lea    ecx, [ebp - 1ch]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_32FC
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0DFF
        push   esi
        lea    eax, [ebp - 18h]
        push   eax
        lea    edx, [ebp - 10h]
        push   esi
        push   dword ptr [ebp - 1ch]
        push   edx
        call   sub_2_32CC
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_0DFF
        cmp    dword ptr [ebp - 18h], 4
        jne    SHORT loc_2_0DFF
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 2]
        cmp    ax, 0eh
        je     SHORT loc_2_0DCB
        cmp    ax, 0fh
        jne    SHORT loc_2_0DFF
loc_2_0DCB:
        push   esi
        lea    ecx, [ebp - 10h]
        push   esi
        push   esi
        push   dword ptr [ebp - 10h]
        push   ecx
        call   sub_2_32CC
        add    esp, 14h
        cmp    eax, 0fh
        jne    SHORT loc_2_0DFF
        push   esi
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_2_3304
        add    esp, 8
        db  089h, 035h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_0E9C
loc_2_0DFF:
        push   3
        push   dword ptr [ebp - 8]
        push   esi
        call   sub_2_32FC
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_2_0E9C
        push   esi
        push   dword ptr [ebp - 8]
        push   esi
        call   sub_2_32FC
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_0E9C
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 17h]
        test   al, 2
        je     SHORT loc_2_0E42
        push   2
        push   dword ptr [ebp - 8]
        call   sub_2_32F4
        add    esp, 8
        jmp    SHORT loc_2_0E9C
loc_2_0E42:
        test   ax, 180h
        jne    SHORT loc_2_0E9C
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0B60
        test   eax, eax
        jne    SHORT loc_2_0E9C
        mov    ebx, 200h
        lea    eax, [ebp - 14h]
        push   ebx
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    dword ptr [ebp - 0ch], esi
        mov    dword ptr [ebp - 14h], 4
        push   ecx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        push   dword ptr [ebp - 8]
        call   sub_2_32D4
        add    esp, 1ch
        lea    ecx, [ebp - 0ch]
        or     dword ptr [ebp - 0ch], edi
        push   ebx
        push   4
        push   ecx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        push   dword ptr [ebp - 8]
        call   sub_2_32EC
        add    esp, 1ch
loc_2_0E9C:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    bl, byte ptr [eax]
        cmp    bl, 0ffh
        jne    NEAR PTR loc_2_0C78
loc_2_0EAC:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_32E4
        add    esp, 8
loc_2_0EBC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0EC3:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        lea    eax, [ebp - 8]
        xor    esi, esi
        push   eax
        push   1
        mov    dword ptr [ebp - 4], esi
        db  068h, 000h, 000h, 000h, 000h
        push   2
        call   sub_2_3344
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_2_0F17
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 4]
        push   eax
        lea    edx, [ebp - 10h]
        mov    dword ptr [ebp - 0ch], 4
        push   ecx
        push   edx
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_2_333C
        add    esp, 18h
        push   dword ptr [ebp - 8]
        call   sub_2_3334
        add    esp, 4
loc_2_0F17:
        test   byte ptr [ebp - 4], 1
        jne    SHORT loc_2_0F22
        call   sub_2_3178
loc_2_0F22:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0F27:
        mov    ecx, 43h
        jmp    NEAR PTR sub_2_0259
sub_2_0F31:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        lea    eax, [ebp - 8]
        push   edi
        mov    dword ptr [ebp - 0ch], ecx
        push   2
        push   ecx
        push   eax
        call   sub_2_32FC
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_0F5A
        xor    eax, eax
        jmp    NEAR PTR loc_2_107E
loc_2_0F5A:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        add    edi, 19h
        mov    ecx, edi
        call   sub_2_0B0E
        test   eax, eax
        je     NEAR PTR loc_2_1079
        sub    eax, edi
        lea    ebx, [eax - 2]
        test   ebx, ebx
        je     NEAR PTR loc_2_1079
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    ebx, eax
        jae    NEAR PTR loc_2_1079
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ebx
        push   edi
        lea    esi, [ecx + eax + 19h]
        push   esi
        call   sub_2_334C
        add    esp, 0ch
        mov    edx, edi
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0663
        test   eax, eax
        je     NEAR PTR loc_2_1079
        mov    ecx, ebx
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_2_0FC3
        mov    ecx, ebx
        and    ecx, 3
        repe cmpsb
loc_2_0FC3:
        je     SHORT loc_2_0FFC
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 17h]
        test   al, 2
        jne    NEAR PTR loc_2_1079
        xor    ecx, ecx
        mov    cx, ax
        mov    eax, 180h
        and    ecx, eax
        je     SHORT loc_2_1000
        cmp    ecx, 80h
        je     SHORT loc_2_101D
        cmp    ecx, 100h
        je     NEAR PTR loc_2_1079
        cmp    ecx, eax
        je     SHORT loc_2_1065
loc_2_0FFC:
        xor    eax, eax
        jmp    SHORT loc_2_107E
loc_2_1000:
        movzx  cx, byte ptr [ebp - 1]
        mov    edx, 2
        call   sub_2_0207
        cmp    ax, 1
        sbb    eax, eax
        and    eax, 0fh
        add    eax, 13h
        jmp    SHORT loc_2_107E
loc_2_101D:
        push   4
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 0ch]
        push   ecx
        call   sub_2_32FC
        movzx  cx, byte ptr [ebp - 1]
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_104E
        mov    edx, 3
        call   sub_2_0207
        test   ax, ax
        je     SHORT loc_2_0FFC
        cmp    ax, 7fh
        je     SHORT loc_2_0FFC
        jmp    SHORT loc_2_1079
loc_2_104E:
        mov    edx, 1
        call   sub_2_0207
        test   ax, ax
        je     SHORT loc_2_0FFC
        cmp    ax, 7fh
        je     SHORT loc_2_0FFC
        jmp    SHORT loc_2_1079
loc_2_1065:
        movzx  cx, byte ptr [ebp - 1]
        mov    edx, 1
        call   sub_2_0207
        test   ax, ax
        je     SHORT loc_2_0FFC
loc_2_1079:
        mov    eax, 13h
loc_2_107E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_1085:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        mov    byte ptr [ebp - 2], dl
        push   esi
        mov    dword ptr [ebp - 0ch], ecx
        xor    si, si
        push   edi
        push   0
        push   100000h
        push   ecx
        call   sub_2_3354
        add    esp, 0ch
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ebx + 17h]
        test   al, 1
        jne    NEAR PTR loc_2_118E
        xor    ecx, ecx
        mov    cx, ax
        mov    eax, 180h
        and    ecx, eax
        je     SHORT loc_2_10DD
        cmp    ecx, 80h
        je     SHORT loc_2_10ED
        cmp    ecx, 100h
        je     SHORT loc_2_10ED
        cmp    ecx, eax
        je     SHORT loc_2_10FC
        jmp    SHORT loc_2_1100
loc_2_10DD:
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_2_118E
        mov    si, 2
        jmp    SHORT loc_2_1100
loc_2_10ED:
        cmp    dword ptr [ebp + 8], 1
        sbb    esi, esi
        and    esi, 0fffffffeh
        add    si, 3
        jmp    SHORT loc_2_1100
loc_2_10FC:
        mov    si, 1
loc_2_1100:
        add    ebx, 19h
loc_2_1103:
        mov    al, byte ptr [ebx]
        test   al, 80h
        je     SHORT loc_2_111A
        movzx  edx, word ptr [ebx + 1]
        add    edx, 3
        mov    byte ptr [ebp - 1], al
        mov    eax, 3
        jmp    SHORT loc_2_1136
loc_2_111A:
        mov    cl, al
        xor    edx, edx
        and    cl, 0f8h
        mov    dl, al
        and    edx, 7
        mov    eax, 1
        inc    edx
        mov    byte ptr [ebp - 1], cl
        cmp    cl, 8
        jne    SHORT loc_2_1136
        mov    eax, edx
loc_2_1136:
        lea    edi, [eax + ebx]
        mov    dword ptr [ebp - 8], edx
        add    ebx, edx
        sub    dword ptr [ebp - 8], eax
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        shr    ecx, 2
        rep stosd
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 3
        rep stosb
        cmp    byte ptr [ebp - 1], 78h
        jne    SHORT loc_2_1103
        movzx  cx, byte ptr [ebp - 2]
        mov    edx, esi
        call   sub_2_0207
        cmp    si, 3
        je     SHORT loc_2_1171
        cmp    si, 1
        jne    SHORT loc_2_118E
loc_2_1171:
        test   ax, ax
        je     SHORT loc_2_117C
        cmp    ax, 7fh
        je     SHORT loc_2_118E
loc_2_117C:
        push   1
        push   100000h
        push   dword ptr [ebp - 0ch]
        call   sub_2_3354
        add    esp, 0ch
loc_2_118E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1197:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 14h]
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_11E3
        cmp    dword ptr [ebp + 8], 5
        jne    SHORT loc_2_11B9
        call   sub_2_0BFF
        jmp    NEAR PTR loc_2_12DB
loc_2_11B9:
        cmp    dword ptr [ebp + 8], 12h
        jne    SHORT loc_2_11C9
        call   sub_2_0EC3
        jmp    NEAR PTR loc_2_12DB
loc_2_11C9:
        cmp    dword ptr [ebp + 8], 0dh
        jne    SHORT loc_2_11D9
        call   sub_2_0F27
        jmp    NEAR PTR loc_2_12DB
loc_2_11D9:
        mov    eax, 1
        jmp    NEAR PTR loc_2_12DD
loc_2_11E3:
        push   0
        lea    eax, [ebp - 4]
        push   esi
        push   eax
        call   sub_2_336C
        add    esp, 0ch
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 4], eax
        je     SHORT loc_2_1206
        mov    eax, 1
        jmp    NEAR PTR loc_2_12DD
loc_2_1206:
        push   0
        lea    ecx, [ebp - 4]
        push   eax
        push   esi
        push   ecx
        call   sub_2_3364
        add    esp, 10h
        mov    edx, 1
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_017B
        test   ax, ax
        je     SHORT loc_2_1232
        mov    eax, 1
        jmp    NEAR PTR loc_2_12DD
loc_2_1232:
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        je     SHORT loc_2_125C
        cmp    eax, 2
        je     SHORT loc_2_1268
        cmp    eax, 3
        je     SHORT loc_2_1285
        cmp    eax, 4
        je     SHORT loc_2_1268
        cmp    eax, 6
        je     NEAR PTR loc_2_12D4
        mov    eax, 1
        jmp    NEAR PTR loc_2_12DD
loc_2_125C:
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, esi
        call   sub_2_0F31
        jmp    SHORT loc_2_12DD
loc_2_1268:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_12DB
        mov    ecx, esi
        call   sub_2_0B60
        push   eax
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, esi
        call   sub_2_1085
        jmp    SHORT loc_2_12DB
loc_2_1285:
        push   0
        lea    eax, [ebp - 8]
        push   esi
        push   0
        push   eax
        call   sub_2_335C
        add    esp, 10h
        test   byte ptr [ebp - 6], 40h
        je     SHORT loc_2_12A3
        mov    eax, 13h
        jmp    SHORT loc_2_12DD
loc_2_12A3:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_2_12B6
        cmp    eax, 1
        je     SHORT loc_2_12C3
loc_2_12AF:
        mov    eax, 1
        jmp    SHORT loc_2_12DD
loc_2_12B6:
        test   byte ptr [ebp - 2], 1
        je     SHORT loc_2_12AF
        mov    eax, 13h
        jmp    SHORT loc_2_12DD
loc_2_12C3:
        mov    ax, word ptr [ebp - 2]
        and    al, 0e2h
        cmp    al, 2
        jne    SHORT loc_2_12AF
        mov    eax, 13h
        jmp    SHORT loc_2_12DD
loc_2_12D4:
        mov    ecx, esi
        call   sub_2_0BCF
loc_2_12DB:
        xor    eax, eax
loc_2_12DD:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_12E2:
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_3374
        add    esp, 0ch
        ret
sub_2_12F3:
        test   edx, edx
        jle    SHORT loc_2_12FD
loc_2_12F7:
        shr    cx, 1
        dec    edx
        jne    SHORT loc_2_12F7
loc_2_12FD:
        xor    eax, eax
        mov    ax, cx
        and    eax, 1
        ret
sub_2_1306:
        xor    ax, ax
        push   esi
        push   edi
        xor    di, di
        test   dx, dx
        je     SHORT loc_2_1325
loc_2_1313:
        movzx  esi, di
        inc    di
        movzx  si, byte ptr [esi + ecx]
        add    ax, si
        cmp    di, dx
        jb     SHORT loc_2_1313
loc_2_1325:
        neg    ax
        pop    edi
        pop    esi
        ret
sub_2_132B:
        test   dx, dx
        jne    SHORT loc_2_1336
        mov    dx, 0ch
        jmp    SHORT loc_2_133D
loc_2_1336:
        movzx  eax, dx
        add    dx, word ptr [eax + ecx]
loc_2_133D:
        mov    ax, dx
        ret
sub_2_1341:
        mov    ax, word ptr [ecx]
        add    ax, 2
        ret
sub_2_1349:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        lea    edi, [edx + 0ch]
        mov    bl, byte ptr [esp + 10h]
        test   bl, bl
        je     SHORT loc_2_136B
loc_2_1359:
        movzx  eax, di
        lea    ecx, [eax + esi]
        call   sub_2_1341
        add    di, ax
        dec    bl
        jne    SHORT loc_2_1359
loc_2_136B:
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_2_1374:
        push   ebx
        push   esi
        mov    bl, 1
        lea    esi, [ecx + 0ch]
        mov    ecx, esi
        call   sub_2_1341
        movzx  eax, ax
        cmp    word ptr [eax + esi], 0
        je     SHORT loc_2_13AB
loc_2_138C:
        mov    ecx, esi
        inc    bl
        call   sub_2_1341
        movzx  eax, ax
        add    esi, eax
        mov    ecx, esi
        call   sub_2_1341
        movzx  eax, ax
        cmp    word ptr [eax + esi], 0
        jne    SHORT loc_2_138C
loc_2_13AB:
        cmp    dword ptr [esi + 6], 47464341h
        jne    SHORT loc_2_13B6
        dec    bl
loc_2_13B6:
        mov    al, bl
        pop    esi
        pop    ebx
        ret
sub_2_13BB:
        push   ebp
        mov    al, byte ptr [ecx + 8]
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        mov    dword ptr [ebp - 8], ecx
        push   esi
        mov    byte ptr [ebp - 1], al
        push   edi
        lea    esi, [ecx + 0ch]
        mov    di, 0ch
        test   al, al
        je     SHORT loc_2_1430
loc_2_13D8:
        mov    bx, 0ch
        mov    dword ptr [ebp - 0ch], esi
        mov    dword ptr [ebp - 10h], esi
        add    esi, 0ch
        cmp    word ptr [esi], 0
        je     SHORT loc_2_1407
loc_2_13EB:
        mov    ecx, esi
        call   sub_2_1341
        add    bx, ax
        mov    ecx, esi
        call   sub_2_1341
        movzx  eax, ax
        add    esi, eax
        cmp    word ptr [esi], 0
        jne    SHORT loc_2_13EB
loc_2_1407:
        add    bx, 2
        add    esi, 2
        mov    eax, dword ptr [ebp - 10h]
        mov    edx, ebx
        lea    ecx, [ebx + 2]
        lea    edi, [ebx + edi + 2]
        mov    word ptr [eax], cx
        mov    ecx, dword ptr [ebp - 0ch]
        call   sub_2_1306
        mov    word ptr [esi], ax
        add    esi, 2
        dec    byte ptr [ebp - 1]
        jne    SHORT loc_2_13D8
loc_2_1430:
        lea    ecx, [edi + 2]
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], cx
        mov    edx, edi
        mov    ecx, eax
        call   sub_2_1306
        mov    word ptr [esi], ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_144C:
        push   esi
        lea    esi, [ecx + 0ch]
        cmp    word ptr [esi], 0
        jne    SHORT loc_2_145A
        xor    eax, eax
        jmp    SHORT loc_2_1497
loc_2_145A:
        mov    ecx, esi
        call   sub_2_1341
        movzx  eax, ax
        cmp    word ptr [eax + esi], 0
        je     SHORT loc_2_1488
loc_2_146B:
        mov    ecx, esi
        call   sub_2_1341
        movzx  eax, ax
        add    esi, eax
        mov    ecx, esi
        call   sub_2_1341
        movzx  ecx, ax
        cmp    word ptr [ecx + esi], 0
        jne    SHORT loc_2_146B
loc_2_1488:
        mov    eax, dword ptr [esi + 6]
        sub    eax, 47464341h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, esi
loc_2_1497:
        pop    esi
        ret
sub_2_1499:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    dword ptr [ebp - 8], edx
        push   esi
        xor    edx, edx
        push   edi
        mov    edi, ecx
        call   sub_2_132B
        mov    bx, ax
        mov    al, byte ptr [edi + 8]
        mov    byte ptr [ebp - 2], al
        test   al, al
        je     SHORT loc_2_152F
loc_2_14BB:
        movzx  eax, bx
        lea    ecx, [eax + edi]
        call   sub_2_144C
        test   eax, eax
        je     SHORT loc_2_151E
        movzx  ecx, byte ptr [eax + 0ch]
        cmp    ecx, 4
        je     SHORT loc_2_14DA
        cmp    ecx, 10h
        je     SHORT loc_2_150C
        jmp    SHORT loc_2_151E
loc_2_14DA:
        lea    esi, [eax + 16h]
        movzx  eax, bx
        mov    byte ptr [ebp - 1], 0
        lea    ecx, [eax + edi]
        call   sub_2_1374
        test   al, al
        je     SHORT loc_2_151E
        mov    dl, byte ptr [ebp - 8]
loc_2_14F3:
        cmp    byte ptr [esi], dl
        jne    SHORT loc_2_14FF
        mov    cl, byte ptr [ebp + 8]
        cmp    byte ptr [esi + 1], cl
        je     SHORT loc_2_153B
loc_2_14FF:
        add    esi, 8
        inc    byte ptr [ebp - 1]
        cmp    al, byte ptr [ebp - 1]
        ja     SHORT loc_2_14F3
        jmp    SHORT loc_2_151E
loc_2_150C:
        add    eax, 16h
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax], ecx
        jne    SHORT loc_2_151E
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], ecx
        je     SHORT loc_2_1549
loc_2_151E:
        mov    edx, ebx
        mov    ecx, edi
        call   sub_2_132B
        mov    bx, ax
        dec    byte ptr [ebp - 2]
        jne    SHORT loc_2_14BB
loc_2_152F:
        xor    ax, ax
loc_2_1532:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_153B:
        mov    edx, ebx
        mov    ecx, edi
        push   dword ptr [ebp - 1]
        call   sub_2_1349
        jmp    SHORT loc_2_1532
loc_2_1549:
        mov    edx, ebx
        mov    ecx, edi
        push   dword ptr [ebp + 0ch]
        call   sub_2_1349
        jmp    SHORT loc_2_1532
sub_2_1557:
        movzx  eax, word ptr [ecx + 0ch]
        push   esi
        add    eax, 0ch
        mov    si, 0ch
        push   edi
        movzx  edi, dx
        cmp    eax, edi
        jge    SHORT loc_2_157F
loc_2_156B:
        movzx  eax, si
        add    si, word ptr [eax + ecx]
        movzx  eax, si
        movzx  edx, word ptr [eax + ecx]
        add    edx, eax
        cmp    edx, edi
        jl     SHORT loc_2_156B
loc_2_157F:
        movzx  eax, si
        lea    eax, [eax + ecx + 9]
        mov    cl, byte ptr [eax]
        or     cl, 4
        mov    byte ptr [eax], cl
        and    cl, 0c7h
        pop    edi
        mov    byte ptr [eax], cl
        or     cl, 40h
        pop    esi
        mov    byte ptr [eax], cl
        and    cl, 7fh
        mov    byte ptr [eax], cl
        ret
sub_2_159F:
        push   ebx
        push   esi
        mov    si, word ptr [ecx]
        push   edi
        mov    di, dx
        mov    ebx, ecx
        sub    si, 2
        xor    edx, edx
        call   sub_2_132B
        cmp    si, ax
        jbe    SHORT loc_2_15D7
        movzx  edi, di
loc_2_15BD:
        movzx  edx, ax
        movzx  ecx, byte ptr [edx + ebx + 2]
        cmp    ecx, edi
        je     SHORT loc_2_15DA
        mov    edx, eax
        mov    ecx, ebx
        call   sub_2_132B
        cmp    si, ax
        ja     SHORT loc_2_15BD
loc_2_15D7:
        xor    ax, ax
loc_2_15DA:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_15DE:
        push   ebx
        push   esi
        mov    bx, word ptr [ecx]
        push   edi
        sub    bx, 2
        mov    edi, edx
        mov    esi, ecx
        xor    edx, edx
        call   sub_2_132B
        cmp    ax, bx
        jae    SHORT loc_2_160F
loc_2_15F8:
        movzx  ecx, ax
        cmp    dword ptr [ecx + esi + 4], edi
        je     SHORT loc_2_1612
        mov    edx, eax
        mov    ecx, esi
        call   sub_2_132B
        cmp    ax, bx
        jb     SHORT loc_2_15F8
loc_2_160F:
        xor    ax, ax
loc_2_1612:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_1616:
        sub    esp, 10h
        mov    edx, 0ffffd041h
        push   ebx
        push   esi
        mov    bx, word ptr [ecx]
        push   edi
        sub    bx, 2
        mov    esi, ecx
        call   sub_2_15DE
        test   ax, ax
        je     SHORT loc_2_1643
        cmp    bx, ax
        jbe    SHORT loc_2_1643
        movzx  ecx, ax
        movzx  eax, byte ptr [ecx + esi + 2]
        jmp    SHORT loc_2_1693
loc_2_1643:
        lea    edi, [esp + 0ch]
        xor    eax, eax
        xor    edx, edx
        mov    ecx, esi
        stosd
        stosd
        stosd
        stosd
        call   sub_2_132B
        test   ax, ax
        je     SHORT loc_2_1682
loc_2_165B:
        cmp    bx, ax
        jbe    SHORT loc_2_1682
        movzx  ecx, ax
        mov    cl, byte ptr [ecx + esi + 2]
        cmp    cl, 10h
        jae    SHORT loc_2_1674
        movzx  ecx, cl
        mov    byte ptr [esp + ecx + 0ch], 1
loc_2_1674:
        mov    edx, eax
        mov    ecx, esi
        call   sub_2_132B
        test   ax, ax
        jne    SHORT loc_2_165B
loc_2_1682:
        mov    eax, 0fh
loc_2_1687:
        cmp    byte ptr [esp + eax + 0ch], 0
        je     SHORT loc_2_1693
        dec    eax
        test   eax, eax
        jg     SHORT loc_2_1687
loc_2_1693:
        test   eax, eax
        jg     SHORT loc_2_1699
        mov    al, 0fh
loc_2_1699:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_2_16A0:
        push   esi
        mov    esi, ecx
        call   sub_2_15DE
        test   ax, ax
        jne    SHORT loc_2_16B3
        mov    ax, 1
        jmp    SHORT loc_2_16E3
loc_2_16B3:
        movzx  edx, ax
        movzx  eax, word ptr [esi]
        movzx  ecx, word ptr [edx + esi]
        sub    eax, ecx
        cmp    eax, edx
        jle    SHORT loc_2_16D6
loc_2_16C3:
        lea    eax, [esi + edx]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [esi + edx], al
        inc    edx
        movzx  eax, word ptr [esi]
        sub    eax, ecx
        cmp    eax, edx
        jg     SHORT loc_2_16C3
loc_2_16D6:
        mov    ecx, esi
        dec    byte ptr [esi + 8]
        call   sub_2_13BB
        xor    ax, ax
loc_2_16E3:
        pop    esi
        ret
sub_2_16E5:
        xor    eax, eax
        cmp    word ptr [ecx], ax
        jne    SHORT loc_2_1704
        cmp    word ptr [ecx + 5ch], ax
        jne    SHORT loc_2_1704
        cmp    word ptr [ecx + 0aeh], ax
        jne    SHORT loc_2_1704
        cmp    word ptr [ecx + 0beh], ax
        je     SHORT loc_2_1709
loc_2_1704:
        mov    eax, 1
loc_2_1709:
        ret
sub_2_170A:
        push   esi
        push   edi
        mov    si, word ptr [ecx]
        movzx  eax, si
        cmp    eax, edx
        jle    SHORT loc_2_174C
        dec    si
        movzx  edi, si
        cmp    edx, edi
        je     SHORT loc_2_1739
        mov    eax, dword ptr [ecx + edi*4 + 2]
        mov    dword ptr [ecx + edx*4 + 2], eax
        mov    eax, dword ptr [ecx + edi*4 + 26h]
        mov    dword ptr [ecx + edx*4 + 26h], eax
        mov    ax, word ptr [ecx + edi*2 + 4ah]
        mov    word ptr [ecx + edx*2 + 4ah], ax
loc_2_1739:
        lea    eax, [ecx + edi*4]
        xor    edx, edx
        mov    dword ptr [eax + 2], edx
        mov    dword ptr [eax + 26h], edx
        mov    word ptr [ecx + edi*2 + 4ah], dx
        mov    word ptr [ecx], si
loc_2_174C:
        pop    edi
        pop    esi
        ret
sub_2_174F:
        push   ebx
        lea    eax, [ecx + 0aeh]
        push   esi
        push   edi
        mov    di, word ptr [eax]
        movzx  esi, di
        cmp    esi, edx
        jle    SHORT loc_2_179A
        dec    di
        movzx  esi, di
        cmp    edx, esi
        je     SHORT loc_2_1787
        mov    bl, byte ptr [esi + ecx + 0b0h]
        mov    byte ptr [ecx + edx + 0b0h], bl
        mov    bl, byte ptr [esi + ecx + 0b7h]
        mov    byte ptr [ecx + edx + 0b7h], bl
loc_2_1787:
        mov    byte ptr [esi + ecx + 0b0h], 0
        mov    byte ptr [esi + ecx + 0b7h], 0
        mov    word ptr [eax], di
loc_2_179A:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_179E:
        push   ebx
        push   esi
        push   edi
        lea    edi, [ecx + 0beh]
        mov    ax, word ptr [edi]
        movzx  esi, ax
        cmp    esi, edx
        jle    SHORT loc_2_17EC
        dec    ax
        movzx  esi, ax
        cmp    esi, edx
        je     SHORT loc_2_17D8
        mov    bl, byte ptr [ecx + esi + 0c0h]
        mov    byte ptr [ecx + edx + 0c0h], bl
        mov    bx, word ptr [ecx + esi*2 + 0c7h]
        mov    word ptr [ecx + edx*2 + 0c7h], bx
loc_2_17D8:
        xor    edx, edx
        mov    byte ptr [ecx + esi + 0c0h], dl
        mov    word ptr [ecx + esi*2 + 0c7h], dx
        mov    word ptr [edi], ax
loc_2_17EC:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_17F0:
        push   esi
        push   edi
        mov    si, word ptr [ecx + 5ch]
        movzx  eax, si
        cmp    eax, edx
        jle    SHORT loc_2_1834
        dec    si
        movzx  edi, si
        cmp    edx, edi
        je     SHORT loc_2_1820
        mov    ax, word ptr [ecx + edi*2 + 5eh]
        mov    word ptr [ecx + edx*2 + 5eh], ax
        mov    ax, word ptr [ecx + edi*2 + 86h]
        mov    word ptr [ecx + edx*2 + 86h], ax
loc_2_1820:
        lea    eax, [ecx + edi*2]
        xor    edx, edx
        mov    word ptr [eax + 5eh], dx
        mov    word ptr [eax + 86h], dx
        mov    word ptr [ecx + 5ch], si
loc_2_1834:
        pop    edi
        pop    esi
        ret
sub_2_1837:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        mov    dword ptr [ebp - 0ch], ecx
        push   esi
        push   edi
        mov    edi, edx
loc_2_1845:
        mov    eax, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [eax]
        movzx  ecx, byte ptr [esi + 4]
        movzx  eax, byte ptr [esi + 3]
        shl    ecx, 8
        add    ecx, eax
        movzx  eax, byte ptr [esi + 2]
        shl    ecx, 8
        add    ecx, eax
        shl    ecx, 8
        mov    dword ptr [ebp - 4], ecx
        movzx  eax, byte ptr [esi + 5]
        movzx  ecx, byte ptr [esi + 6]
        shl    ecx, 8
        xor    ebx, ebx
        add    ecx, eax
        mov    eax, dword ptr [ebp - 4]
        shl    ecx, 0ah
        cmp    word ptr [edi], bx
        lea    edx, [ecx + eax - 1]
        mov    dword ptr [ebp - 8], edx
        jbe    SHORT loc_2_18CD
loc_2_1887:
        mov    ecx, dword ptr [edi + ebx*4 + 2]
        mov    eax, dword ptr [edi + ebx*4 + 26h]
        cmp    ecx, dword ptr [ebp - 4]
        lea    eax, [eax + ecx - 1]
        jb     SHORT loc_2_189D
        cmp    ecx, dword ptr [ebp - 8]
        jbe    SHORT loc_2_18BB
loc_2_189D:
        cmp    eax, dword ptr [ebp - 4]
        jb     SHORT loc_2_18A7
        cmp    eax, dword ptr [ebp - 8]
        jbe    SHORT loc_2_18BB
loc_2_18A7:
        cmp    ecx, dword ptr [ebp - 4]
        ja     SHORT loc_2_18B1
        cmp    eax, dword ptr [ebp - 4]
        jae    SHORT loc_2_18BB
loc_2_18B1:
        cmp    ecx, dword ptr [ebp - 8]
        ja     SHORT loc_2_18C5
        cmp    eax, dword ptr [ebp - 8]
        jb     SHORT loc_2_18C5
loc_2_18BB:
        mov    edx, ebx
        mov    ecx, edi
        dec    ebx
        call   sub_2_170A
loc_2_18C5:
        movzx  eax, word ptr [edi]
        inc    ebx
        cmp    eax, ebx
        jg     SHORT loc_2_1887
loc_2_18CD:
        lea    ecx, [esi + 7]
        mov    eax, dword ptr [ebp - 0ch]
        test   byte ptr [esi], 80h
        mov    dword ptr [eax], ecx
        jne    NEAR PTR loc_2_1845
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_18E5:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   ebp
        mov    edi, edx
        lea    ebp, [edx + 0aeh]
loc_2_18F3:
        xor    ebx, ebx
        cmp    word ptr [ebp], bx
        jbe    SHORT loc_2_191D
loc_2_18FB:
        mov    eax, dword ptr [esi]
        mov    al, byte ptr [eax]
        and    al, 0fh
        cmp    byte ptr [edi + ebx + 0b0h], al
        jne    SHORT loc_2_1914
        mov    edx, ebx
        mov    ecx, edi
        dec    ebx
        call   sub_2_174F
loc_2_1914:
        movzx  eax, word ptr [ebp]
        inc    ebx
        cmp    eax, ebx
        jg     SHORT loc_2_18FB
loc_2_191D:
        mov    eax, dword ptr [esi]
        test   byte ptr [eax], 80h
        lea    ecx, [eax + 2]
        mov    dword ptr [esi], ecx
        jne    SHORT loc_2_18F3
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_192E:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   ebp
        mov    edi, edx
        lea    ebp, [edx + 0beh]
loc_2_193C:
        xor    ebx, ebx
        cmp    word ptr [ebp], bx
        jbe    SHORT loc_2_1966
loc_2_1944:
        mov    eax, dword ptr [esi]
        mov    al, byte ptr [eax]
        and    al, 7
        cmp    byte ptr [edi + ebx + 0c0h], al
        jne    SHORT loc_2_195D
        mov    edx, ebx
        mov    ecx, edi
        dec    ebx
        call   sub_2_179E
loc_2_195D:
        movzx  eax, word ptr [ebp]
        inc    ebx
        cmp    eax, ebx
        jg     SHORT loc_2_1944
loc_2_1966:
        mov    eax, dword ptr [esi]
        test   byte ptr [eax], 80h
        lea    ecx, [eax + 2]
        mov    dword ptr [esi], ecx
        jne    SHORT loc_2_193C
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_1977:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        mov    dword ptr [ebp - 8], ecx
        push   esi
        push   edi
        mov    esi, edx
loc_2_1985:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ecx]
        mov    di, word ptr [eax + 1]
        mov    al, byte ptr [eax]
        xor    ebx, ebx
        mov    byte ptr [ebp - 0ch], al
        xor    ax, ax
        mov    al, byte ptr [ebp - 0ch]
        and    ax, 1fh
        add    ax, di
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [esi + 5ch], bx
        jbe    SHORT loc_2_19FD
loc_2_19AC:
        mov    ax, word ptr [esi + ebx*2 + 5eh]
        mov    cx, word ptr [esi + ebx*2 + 86h]
        add    cx, ax
        dec    cx
        cmp    ax, di
        jb     SHORT loc_2_19C9
        cmp    ax, word ptr [ebp - 2]
        jbe    SHORT loc_2_19EA
loc_2_19C9:
        cmp    cx, di
        jb     SHORT loc_2_19D4
        cmp    cx, word ptr [ebp - 2]
        jbe    SHORT loc_2_19EA
loc_2_19D4:
        cmp    ax, di
        ja     SHORT loc_2_19DE
        cmp    cx, di
        jae    SHORT loc_2_19EA
loc_2_19DE:
        cmp    ax, word ptr [ebp - 2]
        ja     SHORT loc_2_19F4
        cmp    cx, word ptr [ebp - 2]
        jb     SHORT loc_2_19F4
loc_2_19EA:
        mov    edx, ebx
        mov    ecx, esi
        dec    ebx
        call   sub_2_17F0
loc_2_19F4:
        movzx  eax, word ptr [esi + 5ch]
        inc    ebx
        cmp    eax, ebx
        jg     SHORT loc_2_19AC
loc_2_19FD:
        mov    ecx, dword ptr [ebp - 8]
        add    dword ptr [ecx], 3
        test   byte ptr [ebp - 0ch], 80h
        jne    NEAR PTR loc_2_1985
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_1A14:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    dword ptr [ebp - 4], ecx
        cmp    word ptr [ecx], 0
        push   edi
        mov    esi, edx
        je     NEAR PTR loc_2_1AB2
        add    dword ptr [ebp - 4], 2
        mov    ecx, dword ptr [ebp - 4]
        movzx  eax, byte ptr [ecx]
        inc    eax
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp - 4]
        inc    dword ptr [ebp - 4]
        test   eax, eax
        je     SHORT loc_2_1A5D
        test   byte ptr [edi], 80h
        je     SHORT loc_2_1A62
        mov    cx, word ptr [eax + 8]
        mov    edx, dword ptr [ebp + 0ch]
        call   sub_2_12F3
        test   eax, eax
        je     SHORT loc_2_1A62
        jmp    SHORT loc_2_1AB2
loc_2_1A5D:
        test   byte ptr [edi], 80h
        jne    SHORT loc_2_1AB2
loc_2_1A62:
        mov    al, byte ptr [edi]
        test   al, 40h
        jne    SHORT loc_2_1AB2
        test   al, 1
        je     SHORT loc_2_1A76
        mov    ecx, dword ptr [ebp - 4]
        movzx  eax, byte ptr [ecx]
        inc    eax
        add    dword ptr [ebp - 4], eax
loc_2_1A76:
        test   byte ptr [edi], 2
        je     SHORT loc_2_1A85
        lea    ecx, [ebp - 4]
        mov    edx, esi
        call   sub_2_1837
loc_2_1A85:
        test   byte ptr [edi], 4
        je     SHORT loc_2_1A94
        lea    ecx, [ebp - 4]
        mov    edx, esi
        call   sub_2_18E5
loc_2_1A94:
        test   byte ptr [edi], 8
        je     SHORT loc_2_1AA3
        lea    ecx, [ebp - 4]
        mov    edx, esi
        call   sub_2_192E
loc_2_1AA3:
        test   byte ptr [edi], 10h
        je     SHORT loc_2_1AB2
        lea    ecx, [ebp - 4]
        mov    edx, esi
        call   sub_2_1977
loc_2_1AB2:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_1ABA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi]
        sub    ax, 2
        mov    di, 0ch
        mov    word ptr [ebp - 2], ax
        cmp    ax, di
        jbe    SHORT loc_2_1B49
loc_2_1ADA:
        movzx  eax, di
        lea    ebx, [edi + 0ch]
        lea    ecx, [eax + esi]
        call   sub_2_144C
        mov    dword ptr [ebp - 0ch], eax
        movzx  eax, bx
        mov    dword ptr [ebp - 8], 0
        cmp    word ptr [eax + esi], 0
        je     SHORT loc_2_1B37
loc_2_1AFC:
        push   dword ptr [ebp - 8]
        movzx  eax, bx
        push   dword ptr [ebp - 0ch]
        lea    ecx, [eax + esi]
        mov    edx, dword ptr [ebp + 0ch]
        call   sub_2_1A14
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_2_16E5
        test   eax, eax
        je     SHORT loc_2_1B49
        movzx  eax, bx
        lea    ecx, [eax + esi]
        call   sub_2_1341
        add    bx, ax
        inc    dword ptr [ebp - 8]
        movzx  ecx, bx
        cmp    word ptr [ecx + esi], 0
        jne    SHORT loc_2_1AFC
loc_2_1B37:
        mov    edx, edi
        mov    ecx, esi
        call   sub_2_132B
        cmp    word ptr [ebp - 2], ax
        mov    di, ax
        ja     SHORT loc_2_1ADA
loc_2_1B49:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        mov    bx, dx
        push   edi
        movzx  eax, bx
        mov    esi, ecx
        lea    ecx, [eax + ecx]
        call   sub_2_1341
        mov    di, word ptr [esi]
        push   1
        sub    di, bx
        sub    di, ax
        mov    word ptr [ebp - 2], ax
        movzx  eax, di
        add    eax, 2
        push   eax
        call   sub_2_3384
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_2_1BEF
        movzx  ecx, word ptr [ebp - 2]
        push   edi
        add    ecx, esi
        push   eax
        movzx  eax, bx
        add    ecx, eax
        push   ecx
        call   sub_2_0537
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_1341
        push   eax
        movzx  eax, bx
        add    eax, esi
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_0537
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_1341
        add    bx, ax
        push   edi
        movzx  eax, bx
        add    eax, esi
        push   eax
        push   dword ptr [ebp - 8]
        call   sub_2_0537
        add    esp, 0ch
        mov    ecx, esi
        call   sub_2_13BB
        push   0
        push   dword ptr [ebp - 8]
        call   sub_2_337C
        add    esp, 8
loc_2_1BEF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_1BF8:
        xor    ax, ax
        sub    esp, 8
        push   ebx
        push   esi
        mov    bx, word ptr [edx]
        push   edi
        push   ebp
        xor    esi, esi
        test   bx, bx
        je     NEAR PTR loc_2_1CA1
        lea    edi, [edx + 4ah]
        add    edx, 2
        movzx  ebp, bx
loc_2_1C19:
        mov    bx, word ptr [edi]
        mov    esi, dword ptr [edx]
        shr    esi, 8
        add    edi, 2
        add    edx, 4
        mov    dword ptr [esp + 10h], esi
        mov    esi, dword ptr [edx + 20h]
        shr    esi, 0ah
        mov    dword ptr [esp + 14h], esi
        movzx  esi, ax
        add    ax, 7
        add    esi, ecx
        xor    bl, byte ptr [esi]
        and    bl, 1
        xor    bl, byte ptr [esi]
        mov    byte ptr [esi], bl
        and    bl, 0fdh
        mov    byte ptr [esi], bl
        and    bl, 0fbh
        mov    byte ptr [esi], bl
        and    bl, 0e7h
        mov    byte ptr [esi], bl
        and    bl, 0dfh
        mov    byte ptr [esi], bl
        and    bl, 0bfh
        mov    byte ptr [esi], bl
        or     bl, 80h
        mov    byte ptr [esi], bl
        mov    bl, byte ptr [esi + 1]
        and    bl, 0fch
        mov    byte ptr [esi + 1], bl
        and    bl, 0f3h
        mov    byte ptr [esi + 1], bl
        and    bl, 0fh
        mov    byte ptr [esi + 1], bl
        mov    bx, word ptr [esp + 10h]
        mov    byte ptr [esi + 2], bl
        mov    byte ptr [esi + 3], bh
        mov    ebx, dword ptr [esp + 10h]
        shr    ebx, 10h
        dec    ebp
        mov    byte ptr [esi + 4], bl
        mov    bx, word ptr [esp + 14h]
        mov    byte ptr [esi + 5], bl
        mov    byte ptr [esi + 6], bh
        jne    NEAR PTR loc_2_1C19
loc_2_1CA1:
        pop    ebp
        and    byte ptr [esi], 7fh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    8
sub_2_1CAE:
        xor    ax, ax
        sub    esp, 4
        push   ebx
        push   esi
        mov    bx, word ptr [edx + 0aeh]
        push   edi
        push   ebp
        xor    esi, esi
        xor    edi, edi
        cmp    bx, ax
        je     SHORT loc_2_1D46
        movzx  ebp, bx
loc_2_1CCB:
        movzx  esi, ax
        add    esi, ecx
        mov    bl, byte ptr [edi + edx + 0b0h]
        xor    bl, byte ptr [esi]
        and    bl, 0fh
        xor    bl, byte ptr [esi]
        mov    byte ptr [esi], bl
        and    bl, 0efh
        mov    byte ptr [esp + 10h], bl
        mov    byte ptr [esi], bl
        mov    bl, byte ptr [edi + edx + 0b7h]
        and    bl, 2
        shr    bl, 1
        shl    bl, 5
        xor    bl, byte ptr [esp + 10h]
        and    bl, 20h
        xor    bl, byte ptr [esp + 10h]
        mov    byte ptr [esp + 10h], bl
        mov    byte ptr [esi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_1D1C
        test   bl, 20h
        jne    SHORT loc_2_1D1C
        and    bl, 0bfh
        jmp    SHORT loc_2_1D34
loc_2_1D1C:
        mov    bl, byte ptr [edi + edx + 0b7h]
        and    bl, 1
        shl    bl, 6
        xor    bl, byte ptr [esp + 10h]
        and    bl, 40h
        xor    bl, byte ptr [esp + 10h]
loc_2_1D34:
        add    ax, 2
        inc    edi
        mov    byte ptr [esi + 1], 0
        mov    byte ptr [esi], bl
        or     byte ptr [esi], 80h
        cmp    ebp, edi
        jg     SHORT loc_2_1CCB
loc_2_1D46:
        pop    ebp
        and    byte ptr [esi], 7fh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
sub_2_1D53:
        xor    ax, ax
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        xor    edi, edi
        cmp    word ptr [edx + 0beh], ax
        je     SHORT loc_2_1DCF
loc_2_1D6C:
        movzx  edi, ax
        add    ax, 2
        add    edi, ecx
        mov    bl, byte ptr [edx + esi + 0c0h]
        inc    esi
        xor    bl, byte ptr [edi]
        and    bl, 7
        xor    bl, byte ptr [edi]
        mov    byte ptr [edi], bl
        and    bl, 0c7h
        mov    byte ptr [edi], bl
        and    bl, 0bfh
        mov    byte ptr [edi], bl
        or     bl, 80h
        mov    byte ptr [edi], bl
        mov    bl, byte ptr [edi + 1]
        and    bl, 0fch
        mov    byte ptr [ebp - 4], bl
        mov    byte ptr [edi + 1], bl
        mov    bx, word ptr [edx + esi*2 + 0c5h]
        shl    bl, 2
        xor    bl, byte ptr [ebp - 4]
        and    bl, 0ch
        xor    bl, byte ptr [ebp - 4]
        mov    byte ptr [edi + 1], bl
        and    bl, 0cfh
        mov    byte ptr [edi + 1], bl
        and    bl, 3fh
        mov    byte ptr [edi + 1], bl
        movzx  ebx, word ptr [edx + 0beh]
        cmp    esi, ebx
        jl     SHORT loc_2_1D6C
loc_2_1DCF:
        and    byte ptr [edi], 7fh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_1DDB:
        xor    ax, ax
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    edi, edi
        cmp    word ptr [edx + 5ch], ax
        mov    dword ptr [esp + 14h], edi
        jbe    NEAR PTR loc_2_1E82
        lea    esi, [edx + 86h]
loc_2_1DFB:
        cmp    word ptr [esi], 140h
        jbe    SHORT loc_2_1E07
        mov    word ptr [esi], 140h
loc_2_1E07:
        cmp    word ptr [esi], 0
        jbe    SHORT loc_2_1E6D
loc_2_1E0D:
        movzx  edi, ax
        add    edi, ecx
        cmp    word ptr [esi], 20h
        jbe    SHORT loc_2_1E1D
        or     byte ptr [edi], 1fh
        jmp    SHORT loc_2_1E2B
loc_2_1E1D:
        mov    bx, word ptr [esi]
        dec    bl
        xor    bl, byte ptr [edi]
        and    bl, 1fh
        xor    bl, byte ptr [edi]
        mov    byte ptr [edi], bl
loc_2_1E2B:
        mov    bp, word ptr [esi - 28h]
        mov    bl, byte ptr [edi]
        add    ax, 3
        and    bl, 0dfh
        mov    word ptr [edi + 1], bp
        mov    byte ptr [edi], bl
        and    bl, 0bfh
        mov    byte ptr [edi], bl
        or     bl, 80h
        mov    byte ptr [esp + 10h], bl
        mov    byte ptr [edi], bl
        xor    bx, bx
        mov    bl, byte ptr [esp + 10h]
        and    bx, 1fh
        add    bp, bx
        inc    bp
        mov    word ptr [esi - 28h], bp
        mov    bp, word ptr [esi]
        sub    bp, bx
        dec    bp
        mov    word ptr [esi], bp
        jne    SHORT loc_2_1E0D
loc_2_1E6D:
        add    esi, 2
        inc    dword ptr [esp + 14h]
        movzx  ebx, word ptr [edx + 5ch]
        cmp    ebx, dword ptr [esp + 14h]
        jg     NEAR PTR loc_2_1DFB
loc_2_1E82:
        pop    ebp
        and    byte ptr [edi], 7fh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    8
sub_2_1E8F:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    esi, ecx
        xor    edi, edi
        mov    word ptr [ebp - 0ah], di
        mov    word ptr [ebp - 8], di
        mov    word ptr [ebp - 6], di
        call   sub_2_1341
        mov    word ptr [ebp - 2], ax
        mov    ecx, esi
        call   sub_2_1341
        mov    word ptr [ebp - 4], ax
        push   1
        movzx  eax, word ptr [ebp - 2]
        push   eax
        call   sub_2_3384
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        cmp    eax, edi
        jne    SHORT loc_2_1EDD
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_2_20B2
loc_2_1EDD:
        push   dword ptr [ebp - 2]
        push   dword ptr [ebp - 10h]
        push   esi
        call   sub_2_0537
        mov    byte ptr [esi + 2], 1
        mov    byte ptr [esi + 3], 0
        add    esp, 0ch
        cmp    word ptr [ebp - 4], 5
        jae    SHORT loc_2_1EFF
        mov    byte ptr [esi + 4], 0
loc_2_1EFF:
        cmp    word ptr [ebx], 1
        mov    al, byte ptr [esi + 4]
        sbb    cl, cl
        mov    byte ptr [ebp - 1], al
        lea    ecx, [ecx*2 + 2]
        xor    cl, al
        and    cl, 2
        xor    cl, al
        cmp    word ptr [ebx + 0aeh], 1
        sbb    al, al
        mov    byte ptr [esi + 4], cl
        lea    eax, [eax*4 + 4]
        xor    al, cl
        and    al, 4
        xor    al, cl
        cmp    word ptr [ebx + 0beh], 1
        sbb    cl, cl
        mov    byte ptr [esi + 4], al
        lea    ecx, [ecx*8 + 8]
        xor    cl, al
        and    cl, 8
        xor    cl, al
        cmp    word ptr [ebx + 5ch], 1
        sbb    al, al
        mov    byte ptr [esi + 4], cl
        mov    di, 5
        inc    al
        shl    al, 4
        xor    al, cl
        and    al, 10h
        xor    al, cl
        mov    byte ptr [esi + 4], al
        test   al, 1
        je     SHORT loc_2_1F77
        movzx  di, byte ptr [esi + 5]
        add    di, 6
loc_2_1F77:
        test   byte ptr [ebp - 1], 2
        mov    ax, di
        je     SHORT loc_2_1F90
loc_2_1F80:
        movzx  edx, ax
        add    ax, 7
        mov    ecx, dword ptr [ebp - 10h]
        test   byte ptr [edx + ecx], 80h
        jne    SHORT loc_2_1F80
loc_2_1F90:
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_2_1FAA
        mov    word ptr [ebp - 0ah], ax
loc_2_1F9A:
        movzx  edx, ax
        add    ax, 2
        mov    ecx, dword ptr [ebp - 10h]
        test   byte ptr [edx + ecx], 80h
        jne    SHORT loc_2_1F9A
loc_2_1FAA:
        test   byte ptr [ebp - 1], 8
        je     SHORT loc_2_1FC4
        mov    word ptr [ebp - 8], ax
loc_2_1FB4:
        movzx  edx, ax
        add    ax, 2
        mov    ecx, dword ptr [ebp - 10h]
        test   byte ptr [edx + ecx], 80h
        jne    SHORT loc_2_1FB4
loc_2_1FC4:
        test   byte ptr [ebp - 1], 10h
        je     SHORT loc_2_1FCE
        mov    word ptr [ebp - 6], ax
loc_2_1FCE:
        mov    al, byte ptr [esi + 4]
        mov    edx, ebx
        push   dword ptr [ebp - 1]
        push   eax
        movzx  eax, di
        lea    ecx, [eax + esi]
        call   sub_2_1BF8
        add    di, ax
        cmp    word ptr [ebp - 0ah], 0
        je     SHORT loc_2_200B
        mov    ax, word ptr [ebp - 4]
        sub    ax, word ptr [ebp - 0ah]
        push   eax
        movzx  eax, di
        add    eax, esi
        push   eax
        movzx  eax, word ptr [ebp - 0ah]
        add    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_2_0537
        add    esp, 0ch
loc_2_200B:
        mov    al, byte ptr [esi + 4]
        mov    edx, ebx
        push   dword ptr [ebp - 1]
        push   eax
        movzx  eax, di
        lea    ecx, [eax + esi]
        call   sub_2_1CAE
        add    di, ax
        cmp    word ptr [ebp - 8], 0
        je     SHORT loc_2_2048
        mov    ax, word ptr [ebp - 4]
        sub    ax, word ptr [ebp - 8]
        push   eax
        movzx  eax, di
        add    eax, esi
        push   eax
        movzx  eax, word ptr [ebp - 8]
        add    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_2_0537
        add    esp, 0ch
loc_2_2048:
        mov    al, byte ptr [esi + 4]
        mov    edx, ebx
        push   dword ptr [ebp - 1]
        push   eax
        movzx  eax, di
        lea    ecx, [eax + esi]
        call   sub_2_1D53
        add    di, ax
        cmp    word ptr [ebp - 6], 0
        je     SHORT loc_2_2085
        mov    ax, word ptr [ebp - 4]
        sub    ax, word ptr [ebp - 6]
        push   eax
        movzx  eax, di
        add    eax, esi
        push   eax
        movzx  eax, word ptr [ebp - 6]
        add    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_2_0537
        add    esp, 0ch
loc_2_2085:
        mov    al, byte ptr [esi + 4]
        mov    edx, ebx
        push   dword ptr [ebp - 1]
        push   eax
        movzx  eax, di
        lea    ecx, [eax + esi]
        call   sub_2_1DDB
        add    di, ax
        push   0
        lea    ecx, [edi - 2]
        push   dword ptr [ebp - 10h]
        mov    word ptr [esi], cx
        call   sub_2_337C
        mov    ax, di
        add    esp, 8
loc_2_20B2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_20B9:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        mov    bx, dx
        push   edi
        mov    edi, ecx
        movzx  ecx, bx
        add    ecx, edi
        call   sub_2_1341
        mov    si, ax
        push   1
        mov    ax, word ptr [edi]
        sub    ax, bx
        sub    ax, si
        mov    word ptr [ebp - 2], ax
        movzx  eax, ax
        add    eax, 2
        push   eax
        call   sub_2_3384
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        mov    ax, si
        je     SHORT loc_2_2156
        push   dword ptr [ebp - 2]
        movzx  eax, si
        movzx  ecx, bx
        push   dword ptr [ebp - 8]
        add    eax, ecx
        add    eax, edi
        push   eax
        call   sub_2_0537
        movzx  ecx, bx
        add    esp, 0ch
        mov    edx, dword ptr [ebp + 8]
        lea    esi, [edi + ecx]
        mov    ecx, esi
        call   sub_2_1E8F
        add    bx, ax
        push   dword ptr [ebp - 2]
        mov    si, ax
        movzx  eax, bx
        add    eax, edi
        push   eax
        push   dword ptr [ebp - 8]
        call   sub_2_0537
        add    esp, 0ch
        mov    ecx, edi
        call   sub_2_13BB
        push   0
        push   dword ptr [ebp - 8]
        call   sub_2_337C
        mov    ax, si
        add    esp, 8
loc_2_2156:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_215F:
        sub    esp, 0e0h
        push   ebx
        push   esi
        mov    bx, dx
        push   edi
        push   ebp
        mov    esi, ecx
        lea    ecx, [esp + 14h]
        push   2
        push   dword ptr [esp + 0fch]
        push   ecx
        call   sub_2_32FC
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_2191
        mov    ax, 1
        jmp    NEAR PTR loc_2_22C6
loc_2_2191:
        lea    edx, [esp + 18h]
        push   1
        mov    ecx, dword ptr [esp + 18h]
        call   sub_2_0968
        lea    eax, [esp + 18h]
        push   eax
        push   esi
        call   sub_2_1ABA
        lea    ecx, [esp + 20h]
        add    esp, 8
        call   sub_2_16E5
        test   eax, eax
        jne    SHORT loc_2_21C4
        mov    ax, 1
        jmp    NEAR PTR loc_2_22C6
loc_2_21C4:
        mov    edx, ebx
        mov    ecx, esi
        call   sub_2_159F
        mov    di, ax
        xor    edx, edx
        test   di, di
        jne    NEAR PTR loc_2_2281
        mov    di, word ptr [esi]
        inc    byte ptr [esi + 8]
        sub    di, 2
        movzx  eax, di
        mov    byte ptr [eax + esi + 2], bl
        mov    byte ptr [eax + esi + 3], dl
        lea    ecx, [eax + esi]
        lea    ebx, [edi + 4]
        mov    word ptr [ecx], dx
        mov    eax, dword ptr [esp + 0f4h]
        movzx  ebp, bx
        add    bx, 4
        mov    dword ptr [ebp + esi], eax
        movzx  eax, bx
        add    eax, esi
        mov    dword ptr [esp + 10h], eax
        mov    al, byte ptr [eax]
        and    al, 0f0h
        mov    ebp, dword ptr [esp + 10h]
        mov    byte ptr [ebp], al
        and    al, 0cfh
        mov    byte ptr [ebp], al
        or     al, 40h
        mov    byte ptr [ebp], al
        and    al, 7fh
        mov    byte ptr [ebp], al
        inc    ebp
        mov    al, byte ptr [ebp]
        and    al, 0feh
        mov    byte ptr [ebp], al
        and    al, 0fdh
        mov    byte ptr [ebp], al
        or     al, 4
        mov    byte ptr [ebp], al
        and    al, 0c7h
        add    bx, 2
        mov    byte ptr [ebp], al
        and    al, 0bfh
        mov    byte ptr [ebp], al
        and    al, 7fh
        mov    byte ptr [ebp], al
        movzx  ebp, bx
        add    bx, 2
        movzx  eax, bx
        mov    word ptr [ebp + esi], dx
        add    eax, esi
        mov    word ptr [eax], dx
        mov    word ptr [eax + 2], dx
        mov    ax, bx
        sub    ax, di
        add    ax, 6
        mov    word ptr [ecx], ax
        lea    ecx, [ebx + 8]
        mov    word ptr [esi], cx
        jmp    SHORT loc_2_229D
loc_2_2281:
        movzx  eax, di
        mov    bx, word ptr [eax + esi]
        xor    ecx, ecx
        add    bx, di
        sub    bx, 4
        movzx  eax, bx
        add    eax, esi
        mov    word ptr [eax], cx
        mov    word ptr [eax + 2], cx
loc_2_229D:
        lea    eax, [esp + 18h]
        mov    edx, ebx
        push   eax
        mov    ecx, esi
        call   sub_2_20B9
        add    bx, ax
        mov    ecx, esi
        add    bx, 2
        movzx  eax, di
        sub    bx, di
        mov    word ptr [eax + esi], bx
        call   sub_2_13BB
        xor    ax, ax
loc_2_22C6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0e0h
        ret    8
sub_2_22D3:
        mov    ax, 0ch
        push   ebx
        push   esi
        mov    ebx, ecx
        xor    si, si
        push   edi
        cmp    word ptr [ebx], dx
        ja     SHORT loc_2_2361
        cmp    dword ptr [ebx + 2], 47464341h
        jne    SHORT loc_2_2361
        cmp    byte ptr [ebx + 7], 2
        jb     SHORT loc_2_2361
        xor    edi, edi
        cmp    byte ptr [ebx + 8], 0
        jbe    SHORT loc_2_234D
loc_2_22FB:
        mov    edx, esi
        mov    ecx, ebx
        call   sub_2_132B
        mov    si, ax
        movzx  ecx, word ptr [ebx]
        movzx  eax, si
        add    eax, 0ch
        cmp    eax, ecx
        jg     SHORT loc_2_2361
        mov    edx, esi
        mov    ecx, ebx
        call   sub_2_132B
        lea    edx, [esi + 0ch]
loc_2_2320:
        movzx  ecx, dx
        mov    cx, word ptr [ecx + ebx]
        test   cx, cx
        je     SHORT loc_2_2337
        lea    edx, [ecx + edx + 2]
        cmp    dx, ax
        jb     SHORT loc_2_2320
        jmp    SHORT loc_2_2361
loc_2_2337:
        movzx  ecx, dx
        movzx  edx, ax
        sub    ecx, edx
        cmp    ecx, -4
        jne    SHORT loc_2_2361
        movzx  ecx, byte ptr [ebx + 8]
        inc    edi
        cmp    ecx, edi
        ja     SHORT loc_2_22FB
loc_2_234D:
        movzx  ecx, word ptr [ebx]
        movzx  eax, ax
        sub    ecx, eax
        sub    ecx, 2
        cmp    ecx, 1
        sbb    eax, eax
        neg    eax
        jmp    SHORT loc_2_2363
loc_2_2361:
        xor    eax, eax
loc_2_2363:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_2367:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        push   eax
        push   eax
        push   eax
        call   sub_2_33B4
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_2384
        xor    eax, eax
        jmp    SHORT loc_2_23F7
loc_2_2384:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_2_33AC
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_23F2
        xor    esi, esi
loc_2_239E:
        push   esi
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_33A4
        add    esp, 0ch
        mov    edi, eax
        lea    edx, [ebp - 0ch]
        push   esi
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_2_3364
        add    esp, 10h
        test   byte ptr [ebp - 0ah], 20h
        je     SHORT loc_2_23E8
        push   esi
        push   dword ptr [ebp - 4]
        call   sub_2_339C
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_2_23FD
        push   esi
        push   dword ptr [ebp - 4]
        call   sub_2_3304
        add    esp, 8
loc_2_23E8:
        mov    eax, dword ptr [ebp - 8]
        test   edi, edi
        mov    dword ptr [ebp - 4], eax
        je     SHORT loc_2_239E
loc_2_23F2:
        mov    eax, 1
loc_2_23F7:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_2_23FD:
        push   0
        push   dword ptr [ebp - 4]
        call   sub_2_3394
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        push   0
        push   eax
        call   sub_2_338C
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_2_23F7
sub_2_241E:
        push   esi
        push   edi
        call   sub_2_0293
        sub    ax, 87h
        cmp    ax, 1
        mov    eax, dword ptr [esp + 0ch]
        sbb    esi, esi
        inc    esi
        cmp    eax, 1
        je     SHORT loc_2_2460
        cmp    eax, 2
        je     SHORT loc_2_24B7
        cmp    eax, 3
        je     NEAR PTR loc_2_2535
        cmp    eax, 4
        je     NEAR PTR loc_2_2500
        cmp    eax, 0ffffh
        je     NEAR PTR loc_2_2535
        jmp    NEAR PTR loc_2_2545
loc_2_2460:
        test   esi, esi
        jne    SHORT loc_2_247F
        call   sub_2_2578
        push   1
        push   0
        push   0
        call   sub_2_33B4
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_249E
        xor    ecx, ecx
        jmp    SHORT loc_2_24A3
loc_2_247F:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_248C
        xor    ecx, ecx
        jmp    SHORT loc_2_24A3
loc_2_248C:
        call   sub_2_2367
        test   eax, eax
        je     SHORT loc_2_249E
        call   sub_2_2578
        xor    ecx, ecx
        jmp    SHORT loc_2_24A3
loc_2_249E:
        mov    ecx, 1
loc_2_24A3:
        call   sub_2_0259
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_2545
loc_2_24B7:
        xor    ecx, ecx
        test   esi, esi
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_24CE
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 3], ecx
        mov    dword ptr [eax + 7], ecx
loc_2_24CE:
        xor    edi, edi
        call   sub_2_2578
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_24EA
        push   eax
        call   sub_2_33BC
        db  089h, 03Dh, 000h, 000h, 000h, 000h
loc_2_24EA:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   dword ptr [eax + 7]
        push   dword ptr [eax + 3]
        call   sub_2_33CC
        add    esp, 0ch
        jmp    SHORT loc_2_252E
loc_2_2500:
        xor    esi, esi
        db  089h, 035h, 000h, 000h, 000h, 000h
        call   sub_2_2578
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, esi
        je     SHORT loc_2_2522
        push   eax
        call   sub_2_33BC
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_2_2522:
        push   0
        push   0
        call   sub_2_3394
        add    esp, 8
loc_2_252E:
        call   sub_2_25AC
        jmp    SHORT loc_2_2545
loc_2_2535:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_338C
        add    esp, 8
loc_2_2545:
        pop    edi
        pop    esi
        ret
loc_2_2548:
        call   sub_2_00F3
        db  0A1h, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [eax + 1]
        push   ecx
        call   sub_2_241E
        add    esp, 4
        ret
loc_2_2560:
        xor    eax, eax
        push   eax
        db  0A3h, 000h, 000h, 000h, 000h
        push   4
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_2574:
        add    esp, 0ch
        ret
sub_2_2578:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_25AB
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        push   0
        call   sub_2_33F0
        add    esp, 4
        push   0
        push   0ea60h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_33D4
        db  0A3h, 000h, 000h, 000h, 000h
loc_2_25AB:
        ret
sub_2_25AC:
        push   esi
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_25DD
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, esi
        je     SHORT loc_2_25CC
        push   eax
        call   sub_2_33BC
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_2_25CC:
        xor    esi, esi
        push   esi
        call   sub_2_33F8
        add    esp, 4
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_2_25DD:
        pop    esi
        ret
loc_2_25DF:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        test   byte ptr [eax], 1
        je     SHORT loc_2_2629
        xor    esi, esi
        call   sub_2_00F3
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 1]
        cmp    ax, 1
        jne    SHORT loc_2_2619
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_260F
        mov    esi, 1
        jmp    SHORT loc_2_2619
loc_2_260F:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_2_2619:
        test   esi, esi
        jne    SHORT loc_2_2629
        movzx  eax, ax
        push   eax
        call   sub_2_241E
        add    esp, 4
loc_2_2629:
        push   0
        push   0fah
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_33D4
        pop    esi
        ret
sub_2_263C:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 0ch], ecx
        cmp    dword ptr [ebp + 8], 1
        sbb    eax, eax
        dec    dword ptr [ebp - 8]
        and    al, 0f8h
        add    eax, 9bh
        mov    dword ptr [ebp - 10h], eax
        push   1
        push   0
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        mov    word ptr [ebp - 2], 0
        cmp    edx, 0
        je     SHORT loc_2_268D
        mov    word ptr [ebp - 2], ax
loc_2_268D:
        mov    ax, word ptr [ebp - 2]
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_2697:
        push   ebp
        dec    dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        movzx  eax, cx
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], eax
        push   1
        push   0
        push   93h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        push   0
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_26DA:
        movzx  eax, cx
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        test   eax, eax
        mov    dword ptr [ebp - 4], eax
        je     SHORT loc_2_26F8
        push   0
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
loc_2_26F8:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_26FC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        jne    SHORT loc_2_271F
        or     byte ptr [esi + 2ch], 1
        mov    word ptr [esi + 1ch], 82h
        jmp    NEAR PTR loc_2_27A0
loc_2_271F:
        lea    edi, [esi + 1ch]
        movzx  eax, word ptr [edi]
        test   eax, eax
        je     SHORT loc_2_2741
        cmp    eax, 100h
        je     SHORT loc_2_2748
        cmp    eax, 200h
        je     SHORT loc_2_274F
        cmp    eax, 300h
        je     SHORT loc_2_2756
        stc
        jmp    SHORT loc_2_277C
loc_2_2741:
        call   sub_2_27B1
        jmp    SHORT loc_2_2779
loc_2_2748:
        call   sub_2_2815
        jmp    SHORT loc_2_2779
loc_2_274F:
        call   sub_2_27D3
        jmp    SHORT loc_2_2779
loc_2_2756:
        movzx  ecx, word ptr [esi + 38h]
        movzx  eax, word ptr [esi]
        shl    ecx, 10h
        add    ecx, eax
        movzx  eax, word ptr [esi + 14h]
        shl    eax, 10h
        push   ecx
        movzx  ecx, word ptr [esi + 10h]
        add    eax, ecx
        push   eax
        call   sub_2_288E
        add    esp, 8
loc_2_2779:
        mov    word ptr [edi], ax
loc_2_277C:
        jb     SHORT loc_2_2787
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_2_278E
loc_2_2787:
        mov    dword ptr [ebp - 4], 1
loc_2_278E:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_279A
        or     byte ptr [esi + 2ch], 1
        jmp    SHORT loc_2_27A0
loc_2_279A:
        and    word ptr [esi + 2ch], 0fffeh
loc_2_27A0:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_2_27A6:
        clc
        mov    ax, 400h
        ret
BIOS_Service_00:
        jmp    NEAR PTR loc_2_27A6
sub_2_27B1:
        jmp    NEAR PTR loc_2_27A6
loc_2_27B6:
        call   sub_2_0293
        test   ax, ax
        jne    SHORT loc_2_27CC
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [eax + 0bh]
        clc
        jmp    SHORT loc_2_27CD
loc_2_27CC:
        stc
loc_2_27CD:
        ret
BIOS_Service_02:
        jmp    NEAR PTR loc_2_27B6
sub_2_27D3:
        jmp    NEAR PTR loc_2_27B6
loc_2_27D8:
        call   sub_2_2367
        test   eax, eax
        je     SHORT loc_2_27FA
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        call   sub_2_2578
        mov    ecx, 40h
        jmp    NEAR PTR sub_2_0259
loc_2_27FA:
        ret
loc_2_27FB:
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_3400
        xor    ax, ax
        add    esp, 0ch
        ret
BIOS_Service_01:
        jmp    NEAR PTR loc_2_27FB
sub_2_2815:
        jmp    NEAR PTR loc_2_27FB
sub_2_281A:
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        call   sub_2_02D5
        test   ax, ax
        jne    SHORT loc_2_282F
        clc
        jmp    SHORT loc_2_2830
loc_2_282F:
        stc
loc_2_2830:
        ret
BIOS_Service_03:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        movzx  edx, word ptr [eax]
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        xor    esi, esi
        push   esi
        call   sub_2_263C
        mov    word ptr [ebp + 0eh], ax
        push   esi
        mov    word ptr [ebp + 0ch], si
        mov    edx, 2
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_263C
        mov    word ptr [ebp + 0ah], ax
        push   dword ptr [ebp + 0ch]
        mov    word ptr [ebp + 8], si
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_281A
        mov    si, ax
        add    esp, 8
        mov    ecx, dword ptr [ebp + 0ah]
        call   sub_2_26DA
        mov    ecx, dword ptr [ebp + 0eh]
        call   sub_2_26DA
        mov    ax, si
        pop    esi
        pop    ebp
        ret
sub_2_288E:
        push   ebp
        mov    ebp, esp
        push   ebp
        db  08Bh, 02Dh, 000h, 000h, 000h, 000h
        mov    ax, 1410h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        pop    ebp
        mov    dword ptr [ebp + 8], eax
        push   ebp
        db  08Bh, 02Dh, 000h, 000h, 000h, 000h
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        pop    ebp
        mov    dword ptr [ebp + 0ch], eax
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   BIOS_Service_03
        mov    esp, ebp
        pop    ebp
        ret
sub_2_28CA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        db  0CDh, 020h, 000h, 000h, 033h, 000h
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 400h
        jae    SHORT loc_2_28EA
        mov    eax, 13h
        jmp    SHORT loc_2_295E
loc_2_28EA:
        cmp    dword ptr [ebp + 0ch], 1
        je     SHORT loc_2_28F7
        mov    eax, 13h
        jmp    SHORT loc_2_295E
loc_2_28F7:
        call   sub_2_344C
        cmp    eax, 20000000h
        jb     SHORT loc_2_290A
        mov    eax, 22h
        jmp    SHORT loc_2_295E
loc_2_290A:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_291A
        mov    eax, 21h
        jmp    SHORT loc_2_295E
loc_2_291A:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_2935
        push   0
        push   eax
        call   sub_2_338C
        add    esp, 8
        mov    eax, 13h
        jmp    SHORT loc_2_295E
loc_2_2935:
        mov    esi, dword ptr [ebp + 8]
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_2943:
        test   eax, eax
        jne    SHORT loc_2_295E
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   esi
        call   sub_2_3444
        add    esp, 10h
        mov    ecx, esi
        call   sub_2_12E2
        xor    eax, eax
loc_2_295E:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_2965:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   esi
        je     SHORT loc_2_29C8
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0ch]
        test   eax, eax
        je     SHORT loc_2_2992
        cmp    eax, 100h
        je     SHORT loc_2_2995
        cmp    eax, 200h
        je     SHORT loc_2_299C
        cmp    eax, 300h
        je     SHORT loc_2_29B2
        clc
        jmp    SHORT loc_2_29A9
loc_2_2992:
        clc
        jmp    SHORT loc_2_29A9
loc_2_2995:
        call   BIOS_Service_01
        jmp    SHORT loc_2_29A1
loc_2_299C:
        call   BIOS_Service_02
loc_2_29A1:
        mov    ecx, dword ptr [esi + 10h]
        movzx  eax, ax
        mov    dword ptr [ecx], eax
loc_2_29A9:
        mov    eax, 0
        adc    eax, eax
        jmp    SHORT loc_2_29CB
loc_2_29B2:
        mov    esi, dword ptr [esi + 10h]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        call   BIOS_Service_03
        movzx  ecx, ax
        add    esp, 8
        mov    dword ptr [esi], ecx
loc_2_29C8:
        clc
        xor    eax, eax
loc_2_29CB:
        pop    esi
        ret    4
sub_2_29CF:
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    esi, ecx
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        mov    ecx, dword ptr [esi]
        call   sub_2_05AB
        cmp    byte ptr [eax], 0
        mov    edx, eax
        mov    dword ptr [esi], eax
        mov    eax, 0
        je     SHORT loc_2_2A12
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx + edx - 1]
        mov    dword ptr [esi], eax
        mov    eax, 1
loc_2_2A12:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_2A16:
        sub    esp, 4
        db  0BAh, 000h, 000h, 000h, 000h
        mov    dword ptr [esp], ecx
        lea    ecx, [esp]
        call   sub_2_29CF
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_2_2A42
        push   2
        push   dword ptr [esp + 4]
        call   sub_2_04ED
        add    esp, 8
loc_2_2A42:
        add    esp, 4
        ret
sub_2_2A46:
        push   ebp
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    dword ptr [ebp - 4], ecx
        lea    ecx, [ebp - 4]
        call   sub_2_29CF
        test   eax, eax
        jne    SHORT loc_2_2A65
        xor    eax, eax
        jmp    SHORT loc_2_2AAF
loc_2_2A65:
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        call   sub_2_29CF
        test   eax, eax
        jne    SHORT loc_2_2A7A
        xor    eax, eax
        jmp    SHORT loc_2_2AAF
loc_2_2A7A:
        push   2
        push   dword ptr [ebp - 4]
        call   sub_2_04ED
        add    esp, 8
        mov    esi, eax
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        call   sub_2_29CF
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_2_2AAF
        push   2
        push   dword ptr [ebp - 4]
        call   sub_2_04ED
        add    esp, 8
        lea    eax, [eax + esi*8]
loc_2_2AAF:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  083h, 0ECh, 004h, 0BAh, 000h, 000h, 000h, 000h, 089h, 04Ch, 024h, 000h, 08Dh, 04Ch, 024h, 000h
        db  0E8h, 006h, 0FFh, 0FFh, 0FFh, 085h, 0C0h, 0B8h, 000h, 000h, 000h, 000h, 074h, 00Ch, 0FFh, 074h
        db  024h, 000h, 0E8h, 05Fh, 0D9h, 0FFh, 0FFh, 083h, 0C4h, 004h, 083h, 0C4h, 004h, 0C3h
sub_2_2AE2:
        sub    esp, 4
        db  0BAh, 000h, 000h, 000h, 000h
        mov    dword ptr [esp], ecx
        lea    ecx, [esp]
        call   sub_2_29CF
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_2_2B0C
        push   dword ptr [esp]
        call   sub_2_043A
        add    esp, 4
loc_2_2B0C:
        add    esp, 4
        ret
sub_2_2B10:
        push   ebp
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], ecx
        lea    ecx, [ebp - 4]
        call   sub_2_29CF
        test   eax, eax
        jne    SHORT loc_2_2B2E
        xor    eax, eax
        jmp    SHORT loc_2_2B4E
loc_2_2B2E:
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        call   sub_2_29CF
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_2_2B4E
        push   8
        push   dword ptr [ebp - 4]
        call   sub_2_04ED
loc_2_2B4E:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_2B52:
        sub    esp, 4
        db  0BAh, 000h, 000h, 000h, 000h
        mov    dword ptr [esp], ecx
        lea    ecx, [esp]
        call   sub_2_29CF
        test   eax, eax
        mov    al, 0
        je     SHORT loc_2_2B7B
        push   4
        push   dword ptr [esp + 4]
        call   sub_2_04ED
        add    esp, 8
loc_2_2B7B:
        add    esp, 4
        ret
loc_2_2B7F:
        push   ebp
        mov    ebp, esp
        sub    esp, 1e8h
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        push   4
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   eax
        call   sub_2_32FC
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_2CC6
        xor    ebx, ebx
        lea    eax, [ebp - 10h]
        push   ebx
        lea    ecx, [ebp - 0ch]
        push   esi
        push   eax
        push   ecx
        call   sub_2_335C
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_2CC6
        test   byte ptr [ebp - 0bh], 10h
        jne    NEAR PTR loc_2_2CC6
        push   ebx
        lea    eax, [ebp - 8]
        push   esi
        push   eax
        call   sub_2_345C
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_2CC6
        cmp    dword ptr [ebp - 8], 0ffh
        jae    NEAR PTR loc_2_2CC6
        push   ebx
        lea    eax, [ebp - 110h]
        push   100h
        push   eax
        push   esi
        call   sub_2_3454
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_2CC6
        push   3
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 110h]
        call   sub_2_055F
        test   eax, eax
        je     SHORT loc_2_2C42
        lea    ecx, [ebp - 110h]
        mov    bl, 4
        call   sub_2_2A16
        mov    esi, eax
        lea    ecx, [ebp - 110h]
        call   sub_2_2A46
        mov    edi, eax
        xor    al, al
        jmp    SHORT loc_2_2C7F
loc_2_2C42:
        push   6
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 110h]
        call   sub_2_055F
        test   eax, eax
        je     SHORT loc_2_2CC6
        lea    ecx, [ebp - 110h]
        call   sub_2_2AE2
        mov    esi, eax
        lea    ecx, [ebp - 110h]
        mov    bl, 10h
        call   sub_2_2B10
        mov    edi, eax
        lea    ecx, [ebp - 110h]
        call   sub_2_2B52
loc_2_2C7F:
        push   eax
        mov    edx, esi
        push   edi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_1499
        mov    si, ax
        test   si, si
        je     SHORT loc_2_2CC6
        push   ebx
        lea    edx, [ebp - 1e8h]
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_0968
        lea    eax, [ebp - 1e8h]
        mov    edx, esi
        push   eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_20B9
        mov    edx, esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_1557
loc_2_2CC6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_2CCD:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        mov    esi, edx
        mov    ebx, ecx
loc_2_2CD9:
        push   0
        lea    eax, [ebp - 8]
        push   ebx
        push   eax
        call   sub_2_33AC
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_2CF6
        mov    edx, esi
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_2CCD
loc_2_2CF6:
        push   ebx
        call   esi
        add    esp, 4
        lea    eax, [ebp - 4]
        push   0
        push   ebx
        push   eax
        call   sub_2_33A4
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_2D14
        mov    ebx, dword ptr [ebp - 4]
        jmp    SHORT loc_2_2CD9
loc_2_2D14:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_2D1A:
        sub    esp, 4
        xor    eax, eax
        mov    dword ptr [esp], eax
        push   eax
        push   eax
        lea    eax, [esp + 8]
        push   eax
        call   sub_2_3424
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_2D44
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [esp]
        call   sub_2_2CCD
loc_2_2D44:
        add    esp, 4
        ret
loc_2_2D48:
        push   ebp
        mov    ebp, esp
        sub    esp, 108h
        push   esi
        lea    eax, [ebp - 8]
        push   2
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   eax
        call   sub_2_32FC
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_2DCB
        push   0
        lea    eax, [ebp - 4]
        push   esi
        push   eax
        call   sub_2_345C
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_2DCB
        cmp    dword ptr [ebp - 4], 0ffh
        jae    SHORT loc_2_2DCB
        push   0
        lea    eax, [ebp - 108h]
        push   100h
        push   eax
        push   esi
        call   sub_2_3454
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_2_2DCB
        cmp    byte ptr [ebp - 108h], 0
        je     SHORT loc_2_2DCB
        push   esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  066h, 00Fh, 0B6h, 015h, 000h, 000h, 000h, 000h
        push   0ffffd041h
        call   sub_2_215F
        cmp    ax, 1
        sbb    eax, eax
        neg    eax
        jmp    SHORT loc_2_2DCD
loc_2_2DCB:
        xor    eax, eax
loc_2_2DCD:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_2DD2:
        sub    esp, 8
        db  0B9h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        mov    esi, 2
        push   ebp
        mov    edx, esi
        push   0
        call   sub_2_263C
        mov    di, ax
        push   0
        lea    ecx, [esp + 16h]
        mov    edx, esi
        call   sub_2_263C
        mov    si, ax
        push   0
        lea    ecx, [esp + 18h]
        mov    edx, 4
        call   sub_2_263C
        movzx  edx, si
        mov    bx, ax
        movzx  ecx, di
        movzx  eax, bx
        shl    eax, 10h
        shl    edx, 10h
        push   eax
        shl    ecx, 10h
        call   sub_2_037A
        mov    bp, ax
        mov    ecx, edi
        call   sub_2_26DA
        mov    ecx, esi
        call   sub_2_26DA
        mov    ecx, ebx
        call   sub_2_26DA
        test   bp, bp
        je     SHORT loc_2_2E50
        mov    ax, 1
        jmp    NEAR PTR loc_2_2FB4
loc_2_2E50:
        xor    esi, esi
        cmp    dword ptr [esp + 14h], esi
        je     SHORT loc_2_2E7D
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        push   esi
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_2_3408
        add    esp, 0ch
        mov    edx, 10000h
        mov    ecx, eax
        push   esi
        call   sub_2_263C
        mov    si, ax
loc_2_2E7D:
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        push   1
        push   eax
        call   sub_2_3384
        add    esp, 8
        db  0A3h, 000h, 000h, 000h, 000h
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        push   1
        push   eax
        call   sub_2_3384
        add    esp, 8
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 000h, 000h, 000h, 000h
        push   0
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_2_263C
        mov    bx, ax
        mov    edx, esi
        movzx  ebp, bx
        shl    ebp, 10h
        mov    ecx, ebp
        call   sub_2_03BA
        test   ax, ax
        jne    NEAR PTR loc_2_2F7F
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_22D3
        test   eax, eax
        je     NEAR PTR loc_2_2F7F
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ecx]
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_0537
        add    esp, 0ch
        mov    edx, 0ffffd041h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_16A0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_1616
        db  0B9h, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        call   sub_2_3204
        mov    di, ax
        call   sub_2_2D1A
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_13BB
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ecx]
        cmp    word ptr [esp + 12h], ax
        jb     SHORT loc_2_2F83
        sub    ax, 2
        movzx  edx, ax
        push   edx
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        call   sub_2_055F
        test   eax, eax
        jne    SHORT loc_2_2F83
        mov    edx, esi
        mov    ecx, ebp
        call   sub_2_03FA
        test   ax, ax
        je     SHORT loc_2_2F83
loc_2_2F7F:
        mov    di, 1
loc_2_2F83:
        mov    ecx, esi
        call   sub_2_26DA
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_337C
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        push   0
        push   eax
        call   sub_2_337C
        add    esp, 8
        mov    ecx, ebx
        call   sub_2_26DA
        mov    ax, di
loc_2_2FB4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_2_2FBC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0dch
        push   ebx
        lea    ecx, [ebp - 0dch]
        push   esi
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        push   ecx
        call   sub_2_3464
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_2FE7
        xor    eax, eax
        jmp    NEAR PTR loc_2_30F2
loc_2_2FE7:
        xor    eax, eax
        cmp    word ptr [ebp - 80h], ax
        jbe    SHORT loc_2_3022
        lea    edi, [ebp - 7eh]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  08Bh, 035h, 000h, 000h, 000h, 000h
loc_2_2FFE:
        lea    ebx, [edx + esi]
        inc    eax
        add    esi, 4
        mov    byte ptr [ebx], 4bh
        mov    cx, word ptr [edi]
        add    edi, 2
        mov    word ptr [ebx + 1], cx
        mov    cl, byte ptr [edi + 26h]
        mov    byte ptr [ebx + 3], cl
        movzx  ecx, word ptr [ebp - 80h]
        cmp    ecx, eax
        jg     SHORT loc_2_2FFE
        jmp    SHORT loc_2_302E
loc_2_3022:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  08Bh, 035h, 000h, 000h, 000h, 000h
loc_2_302E:
        mov    dword ptr [ebp - 4], 0
        cmp    word ptr [ebp - 0dch], 0
        jbe    SHORT loc_2_308F
        lea    edi, [ebp - 0dah]
        lea    eax, [ebp - 92h]
loc_2_304B:
        lea    ebx, [edx + esi]
        inc    dword ptr [ebp - 4]
        mov    word ptr [ebx + 1], 9
        add    esi, 0ch
        mov    byte ptr [ebx], 81h
        mov    cx, word ptr [eax]
        add    eax, 2
        and    cl, 1
        mov    byte ptr [ebx + 3], cl
        mov    ecx, dword ptr [edi]
        shr    ecx, 8
        add    edi, 4
        mov    word ptr [ebx + 4], cx
        mov    word ptr [ebx + 6], cx
        mov    ecx, dword ptr [edi + 20h]
        shr    ecx, 8
        mov    word ptr [ebx + 0ah], cx
        movzx  ecx, word ptr [ebp - 0dch]
        cmp    ecx, dword ptr [ebp - 4]
        jg     SHORT loc_2_304B
loc_2_308F:
        xor    edi, edi
        cmp    word ptr [ebp - 2eh], di
        jbe    SHORT loc_2_30B8
loc_2_3097:
        mov    bx, 1
        lea    eax, [edx + esi]
        add    esi, 3
        inc    edi
        mov    byte ptr [eax], 22h
        mov    cl, byte ptr [ebp + edi - 2dh]
        shl    bx, cl
        mov    word ptr [eax + 1], bx
        movzx  eax, word ptr [ebp - 2eh]
        cmp    eax, edi
        jg     SHORT loc_2_3097
loc_2_30B8:
        xor    edi, edi
        db  089h, 035h, 000h, 000h, 000h, 000h
        cmp    word ptr [ebp - 1eh], di
        jbe    SHORT loc_2_30ED
loc_2_30C6:
        lea    ebx, [edx + esi]
        mov    al, 1
        add    esi, 3
        inc    edi
        mov    byte ptr [ebx], 2ah
        db  089h, 035h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [ebp + edi - 1dh]
        shl    al, cl
        mov    byte ptr [ebx + 2], 0
        mov    byte ptr [ebx + 1], al
        movzx  eax, word ptr [ebp - 1eh]
        cmp    eax, edi
        jg     SHORT loc_2_30C6
loc_2_30ED:
        mov    eax, 1
loc_2_30F2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_30F9:
        push   esi
        push   1
        mov    esi, 800h
        push   esi
        call   sub_2_3384
        add    esp, 8
        mov    edx, esi
        mov    ecx, eax
        db  0A3h, 000h, 000h, 000h, 000h
        push   0
        call   sub_2_263C
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  0B9h, 000h, 000h, 000h, 000h
        call   sub_2_3204
        test   ax, ax
        jne    SHORT loc_2_3159
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [eax + ecx], 79h
        mov    byte ptr [eax + ecx + 1], 0
        db  083h, 005h, 000h, 000h, 000h, 000h, 002h
        db  00Fh, 0B7h, 00Dh, 000h, 000h, 000h, 000h
        shl    ecx, 10h
        call   sub_2_0310
loc_2_3159:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        call   sub_2_26DA
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_337C
        add    esp, 8
        pop    esi
        ret
sub_2_3178:
        push   esi
        mov    eax, 800h
        xor    esi, esi
        push   1
        push   eax
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_2_3384
        add    esp, 8
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    eax, esi
        je     SHORT loc_2_3202
        push   esi
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        call   sub_2_263C
        movzx  ecx, ax
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        shl    ecx, 10h
        call   sub_2_0345
        movzx  eax, ax
        test   eax, eax
        je     SHORT loc_2_31DA
        cmp    eax, 8dh
        je     SHORT loc_2_31E1
        jmp    SHORT loc_2_31E6
loc_2_31DA:
        call   sub_2_30F9
        jmp    SHORT loc_2_31E6
loc_2_31E1:
        call   sub_2_2DD2
loc_2_31E6:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_337C
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    esp, 8
        call   sub_2_26DA
loc_2_3202:
        pop    esi
        ret
sub_2_3204:
        push   ebp
        mov    ebp, esp
        sub    esp, 0e4h
        push   esi
        push   edi
        mov    esi, ecx
        xor    edi, edi
        push   edi
        lea    eax, [ebp - 4]
        push   edi
        mov    dword ptr [ebp - 4], edi
        push   eax
        call   sub_2_3424
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_3298
        push   edi
        lea    ecx, [ebp - 4]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_33AC
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_3298
loc_2_323C:
        push   edi
        lea    ecx, [ebp - 0e4h]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_3464
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_3279
        push   edi
        lea    ecx, [ebp - 0ch]
        push   dword ptr [ebp - 4]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        call   sub_2_335C
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_2_3279
        test   byte ptr [ebp - 7], 10h
        jne    SHORT loc_2_3279
        push   dword ptr [ebp - 4]
        call   esi
        add    esp, 4
loc_2_3279:
        push   edi
        lea    ecx, [ebp - 4]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_33A4
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_323C
        sub    eax, 0dh
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        jmp    SHORT loc_2_329C
loc_2_3298:
        mov    ax, 1
loc_2_329C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh, 0CCh, 0CDh, 020h, 064h, 080h, 001h, 000h, 0CCh, 0CCh, 0CDh, 020h, 096h, 081h, 001h, 000h
        db  0CCh, 0CCh, 0CDh, 020h, 093h, 081h, 001h, 000h, 0CCh, 0CCh, 0CDh, 020h, 063h, 080h, 001h, 000h
        db  0CCh, 0CCh
sub_2_32C4:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        int3
        int3
sub_2_32CC:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        int3
        int3
sub_2_32D4:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        int3
        int3
sub_2_32DC:
        db  0CDh, 020h, 038h, 080h, 033h, 000h
        int3
        int3
sub_2_32E4:
        db  0CDh, 020h, 037h, 080h, 033h, 000h
        int3
        int3
sub_2_32EC:
        db  0CDh, 020h, 03Fh, 080h, 033h, 000h
        int3
        int3
sub_2_32F4:
        db  0CDh, 020h, 040h, 080h, 033h, 000h
        int3
        int3
sub_2_32FC:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        int3
        int3
sub_2_3304:
        db  0CDh, 020h, 00Dh, 080h, 033h, 000h
        int3
        int3
sub_2_330C:
        db  0CDh, 020h, 043h, 080h, 033h, 000h
        int3
        int3
sub_2_3314:
        db  0CDh, 020h, 00Ah, 080h, 033h, 000h
        int3
        int3
sub_2_331C:
        db  0CDh, 020h, 00Bh, 080h, 033h, 000h
        int3
        int3
sub_2_3324:
        db  0CDh, 020h, 035h, 080h, 033h, 000h
        int3
        int3
sub_2_332C:
        db  0CDh, 020h, 050h, 080h, 001h, 000h
        int3
        int3
sub_2_3334:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        int3
        int3
sub_2_333C:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        int3
        int3
sub_2_3344:
        db  0CDh, 020h, 06Fh, 081h, 001h, 000h
        int3
        int3
sub_2_334C:
        db  0CDh, 020h, 07Dh, 081h, 001h, 000h
        int3
        int3
sub_2_3354:
        db  0CDh, 020h, 05Bh, 080h, 033h, 000h
        int3
        int3
sub_2_335C:
        db  0CDh, 020h, 036h, 080h, 033h, 000h
        int3
        int3
sub_2_3364:
        db  0CDh, 020h, 009h, 080h, 033h, 000h
        int3
        int3
sub_2_336C:
        db  0CDh, 020h, 003h, 080h, 033h, 000h
        int3
        int3
sub_2_3374:
        db  0CDh, 020h, 00Fh, 080h, 033h, 000h
        int3
        int3
sub_2_337C:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_2_3384:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_2_338C:
        db  0CDh, 020h, 033h, 080h, 033h, 000h
        int3
        int3
sub_2_3394:
        db  0CDh, 020h, 04Fh, 080h, 033h, 000h
        int3
        int3
sub_2_339C:
        db  0CDh, 020h, 00Ch, 080h, 033h, 000h
        int3
        int3
sub_2_33A4:
        db  0CDh, 020h, 005h, 080h, 033h, 000h
        int3
        int3
sub_2_33AC:
        db  0CDh, 020h, 004h, 080h, 033h, 000h
        int3
        int3
sub_2_33B4:
        db  0CDh, 020h, 047h, 080h, 033h, 000h
        int3
        int3
sub_2_33BC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
        db  0CCh
sub_2_33CC:
        db  0CDh, 020h, 046h, 080h, 033h, 000h
        int3
        int3
sub_2_33D4:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  0CCh, 0CCh, 0CCh
sub_2_33F0:
        db  0CDh, 020h, 016h, 080h, 033h, 000h
        int3
        int3
sub_2_33F8:
        db  0CDh, 020h, 017h, 080h, 033h, 000h
        int3
        int3
sub_2_3400:
        db  0CDh, 020h, 04Eh, 080h, 033h, 000h
        int3
        int3
sub_2_3408:
        db  0CDh, 020h, 06Ch, 080h, 001h, 000h
        int3
        int3
sub_2_3410:
        push   edi
        mov    edi, dword ptr [esp + 8]
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        cmc
        sbb    ecx, ecx
        pop    edi
        and    eax, ecx
        ret    4
sub_2_3424:
        db  0CDh, 020h, 002h, 080h, 033h, 000h
        int3
        int3
sub_2_342C:
        db  0CDh, 020h, 061h, 080h, 001h, 000h
        int3
        int3
sub_2_3434:
        db  0CDh, 020h, 053h, 080h, 001h, 000h
        int3
        int3
sub_2_343C:
        db  0CDh, 020h, 032h, 080h, 033h, 000h
        int3
        int3
sub_2_3444:
        db  0CDh, 020h, 00Eh, 080h, 033h, 000h
        int3
        int3
sub_2_344C:
        db  0CDh, 020h, 011h, 081h, 001h, 000h
        int3
        int3
sub_2_3454:
        db  0CDh, 020h, 007h, 080h, 033h, 000h
        int3
        int3
sub_2_345C:
        db  0CDh, 020h, 006h, 080h, 033h, 000h
        int3
        int3
sub_2_3464:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
_PTEXT ends
        end
