; nwredir_pcode_07_01.asm -- NWREDIR object 7 segment _PTEXT  (part 1/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_7_0000:
        inc    ecx
        add    byte ptr [eax], al
        db  000h, 064h, 000h, 000h
        db  000h, 070h, 000h
        add    byte ptr [eax], al
        jl     SHORT loc_7_000E
loc_7_000E:
        add    byte ptr [eax], al
        add    dword ptr [eax], 0
        add    dh, al
        db  005h, 096h, 003h, 000h, 000h
        db  001h, 080h, 03Dh, 095h, 003h, 000h
        add    byte ptr [eax], al
        jne    SHORT loc_7_0029
        db  0E8h, 006h, 000h, 0F9h, 0FFh
loc_7_0029:
        movzx  eax, byte ptr [ebp + 10h]
        cmp    eax, 0bh
        db  00Fh, 083h, 076h, 000h, 0F9h, 0FFh
        db  0FFh, 014h, 085h, 000h, 000h, 000h, 000h
        clc
        ret
loc_7_0041:
        mov    word ptr [ebp + 1ch], 0
        mov    word ptr [ebp + 10h], 1827h
        mov    word ptr [ebp + 18h], 3645h
        db  080h, 03Dh, 06Bh, 004h, 000h, 000h, 000h
        setne  al
        shl    al, 0
        or     byte ptr [ebp + 1ch], al
        ret
loc_7_0064:
        push   0
        db  0E8h, 045h, 0A6h, 0F9h, 0FFh
        add    esp, 4
        jmp    SHORT loc_7_0041
loc_7_0070:
        push   0
        db  0E8h, 069h, 0A6h, 0F9h, 0FFh
        add    esp, 4
        jmp    SHORT loc_7_0041
loc_7_007C:
        db  0E8h, 02Fh, 0A9h, 0F9h, 0FFh
        jmp    SHORT loc_7_0041
loc_7_0083:
        db  0E8h, 008h, 0A9h, 0F9h, 0FFh
        jmp    SHORT loc_7_0041
NWREDIR_W32_DEVICEIOCONTROL:
        db  0C6h, 005h, 096h, 003h, 000h, 000h, 001h
        db  080h, 03Dh, 095h, 003h, 000h, 000h, 000h
        jne    SHORT loc_7_009F
        db  0E8h, 090h, 0FFh, 0F8h, 0FFh
loc_7_009F:
        push   esi
        db  0E8h, 02Bh, 000h, 0F9h, 0FFh
        add    esp, 4
        ret
NWREDIR_SYS_VM_TERMINATE2:
        mov    eax, 2
        mov    ebx, 3
        db  0CDh, 020h, 006h, 000h, 087h, 004h
        db  0E9h, 0E2h, 003h, 0F9h, 0FFh
NWREDIR_DESTROY_VM:
        db  068h, 0CEh, 005h, 000h, 000h
        call   sub_7_01C2
        call   sub_7_01E8
        add    esp, 4
        ret
NWREDIR_SYS_VM_INIT:
        db  0C6h, 005h, 096h, 003h, 000h, 000h, 001h
        db  080h, 03Dh, 095h, 003h, 000h, 000h, 000h
        jne    SHORT loc_7_00E6
        db  0E8h, 049h, 0FFh, 0F8h, 0FFh
loc_7_00E6:
        push   ebx
        db  0E8h, 0E4h, 004h, 0F9h, 0FFh
        add    esp, 4
        ret
NWREDIR_VM_INIT:
        push   ebx
        db  0E8h, 05Ah, 005h, 0F9h, 0FFh
        add    esp, 4
        ret
NWREDIR_VM_TERMINATE:
        push   ebx
        db  0E8h, 080h, 006h, 0F9h, 0FFh
        add    esp, 4
        ret
NWREDIR_VM_SUSPEND:
        push   ebx
        db  0E8h, 026h, 007h, 0F9h, 0FFh
        add    esp, 4
        ret
NWREDIR_VM_RESUME:
        push   ebx
        db  0E8h, 05Ch, 007h, 0F9h, 0FFh
        add    esp, 4
        and    eax, eax
        ret
sub_7_011A:
        push   ebp
        xor    eax, eax
        push   eax
        mov    esi, esp
        lea    ebp, [esi + 2]
        db  0BFh, 0A0h, 004h, 000h, 000h
loc_7_0128:
        mov    al, byte ptr [esi]
        mov    byte ptr [edi], al
        and    al, al
        je     SHORT loc_7_0174
        cmp    al, 5fh
        je     SHORT loc_7_0174
        push   1
        push   1
        push   esi
        push   ebp
        db  0CDh, 020h, 042h, 000h, 040h, 000h
        add    esp, 10h
        and    eax, eax
        je     SHORT loc_7_0174
        xor    al, al
        test   edx, 2
        jne    SHORT loc_7_0172
        push   2
        push   ebp
        push   ebp
        db  0CDh, 020h, 043h, 000h, 040h, 000h
        add    esp, 0ch
        push   dword ptr [ebp]
        db  0CDh, 020h, 044h, 000h, 040h, 000h
        add    esp, 4
        test   ah, ah
        jne    SHORT loc_7_0174
        cmp    al, 5fh
        je     SHORT loc_7_0174
loc_7_0172:
        mov    byte ptr [edi], al
loc_7_0174:
        inc    edi
        inc    byte ptr [esi]
        jne    SHORT loc_7_0128
        pop    eax
        pop    ebp
        ret
sub_7_017C:
        pushad
        push   edx
        db  0CDh, 020h, 012h, 000h, 040h, 000h
        pop    edx
        popad
        ret
loc_7_0187:
        pushad
        push   edx
        db  0CDh, 020h, 013h, 000h, 040h, 000h
        pop    edx
        popad
        ret
sub_7_0192:
        mov    edx, dword ptr [esp + 4]
        add    dword ptr [edx], 1
loc_7_0199:
        test   dword ptr [edx], 80000000h
        jne    SHORT loc_7_01A2
        ret
loc_7_01A2:
        call   sub_7_017C
        jmp    SHORT loc_7_0199
sub_7_01A9:
        mov    edx, dword ptr [esp + 4]
        sub    dword ptr [edx], 1
        test   dword ptr [edx], 0ffffh
        je     SHORT loc_7_01B9
        ret
loc_7_01B9:
        test   dword ptr [edx], 7fff0000h
        jne    SHORT loc_7_0187
        ret
sub_7_01C2:
        mov    edx, dword ptr [esp + 4]
        add    dword ptr [edx], 10000h
loc_7_01CC:
        test   dword ptr [edx], 8000ffffh
        jne    SHORT loc_7_01E1
        sub    dword ptr [edx], 10000h
        add    dword ptr [edx], 80000000h
        ret
loc_7_01E1:
        call   sub_7_017C
        jmp    SHORT loc_7_01CC
sub_7_01E8:
        mov    edx, dword ptr [esp + 4]
        sub    dword ptr [edx], 80000000h
        test   dword ptr [edx], 7fffffffh
        jne    SHORT loc_7_0187
        ret
NWREDIR_PM_API:
        mov    dword ptr [ebp + 1ch], 0ffffffffh
        or     word ptr [ebp + 2ch], 1
        ret
NWREDIR_Service_00:
        mov    eax, 5
        clc
        ret
NWREDIR_Service_01:
        db  0C6h, 005h, 096h, 003h, 000h, 000h, 001h
        db  080h, 03Dh, 095h, 003h, 000h, 000h, 000h
        jne    SHORT loc_7_0224
        db  0E8h, 00Bh, 0FEh, 0F8h, 0FFh
loc_7_0224:
        mov    edx, dword ptr [esp + 4]
        sub    esp, 74h
        mov    dword ptr [esp + 8], 0
        mov    byte ptr [esp + 40h], 0
        mov    byte ptr [esp + 41h], 0ffh
        mov    word ptr [esp + 1ah], 0
        mov    eax, esp
        push   edi
        push   4
        db  0FFh, 035h, 0A0h, 005h, 000h, 000h
        push   edx
        push   eax
        call   sub_7_0656
        add    esp, 10h
        pop    edi
        mov    ax, word ptr [esp + 1ah]
        add    esp, 74h
        mov    edx, dword ptr [esp + 4]
        and    word ptr [edx + 2ch], 0fffeh
        and    ax, ax
        je     SHORT loc_7_027E
        or     word ptr [edx + 2ch], 1
        movzx  eax, ax
        mov    dword ptr [edx + 1ch], eax
loc_7_027E:
        ret
sub_7_027F:
        db  0C6h, 005h, 096h, 003h, 000h, 000h, 001h
        db  080h, 03Dh, 095h, 003h, 000h, 000h, 000h
        jne    SHORT loc_7_0294
        db  0E8h, 09Bh, 0FDh, 0F8h, 0FFh
loc_7_0294:
        mov    edx, dword ptr [esp + 8]
        pop    dword ptr [edx + 34h]
        call   sub_7_2340
        or     al, al
        jne    SHORT loc_7_02E7
        call   sub_7_0D50
        mov    eax, dword ptr [esp]
loc_7_02AC:
        call   sub_7_0E10
        movzx  eax, al
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [edx + 38h], eax
        cmp    al, 37h
        jne    SHORT loc_7_02D3
        db  0E8h, 09Ch, 020h, 0F9h, 0FFh
        and    al, al
        je     SHORT loc_7_02AC
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [edx + 38h], 37h
loc_7_02D3:
        call   sub_7_0D90
        db  0E8h, 023h, 008h, 0F9h, 0FFh
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [edx + 38h]
        jmp    dword ptr [edx + 34h]
loc_7_02E7:
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [edx + 38h], eax
        jmp    dword ptr [edx + 34h]
sub_7_02F1:
        db  083h, 03Dh, 0D0h, 00Dh, 000h, 000h, 000h
        jne    SHORT loc_7_0314
        push   esi
        call   sub_7_0370
        xor    edx, edx
        db  0BEh, 018h, 003h, 000h, 000h
        db  0CDh, 020h, 0FEh, 000h, 001h, 000h
        db  089h, 035h, 0D0h, 00Dh, 000h, 000h
        pop    esi
loc_7_0314:
        ret
        db  02Eh, 08Bh, 0C0h
loc_7_0318:
        db  003h, 00Dh, 0B4h, 003h, 000h, 000h
        push   ecx
        call   sub_7_23E0
        add    esp, 4
        xor    esi, esi
        and    al, al
        je     SHORT loc_7_0346
        db  0A1h, 0B4h, 003h, 000h, 000h
        xor    edx, edx
        db  0BEh, 018h, 003h, 000h, 000h
        db  0CDh, 020h, 0FEh, 000h, 001h, 000h
loc_7_033F:
        db  089h, 035h, 0D0h, 00Dh, 000h, 000h
        ret
loc_7_0346:
        call   sub_7_039A
        sub    esi, esi
        jmp    SHORT loc_7_033F
sub_7_034F:
        db  00Fh, 0BAh, 02Dh, 0B8h, 003h, 000h, 000h, 003h
        jb     SHORT loc_7_03A1
        db  00Fh, 0BAh, 02Dh, 0B8h, 003h, 000h, 000h, 001h
        jb     SHORT loc_7_03A1
        mov    eax, 2
        db  0CDh, 020h, 003h, 000h, 005h, 000h
        jmp    SHORT loc_7_03A1
sub_7_0370:
        db  00Fh, 0BAh, 02Dh, 0B8h, 003h, 000h, 000h, 002h
        jb     SHORT loc_7_03A1
        db  00Fh, 0BAh, 02Dh, 0B8h, 003h, 000h, 000h, 000h
        jb     SHORT loc_7_03A1
        mov    eax, 0ah
        db  0CDh, 020h, 003h, 000h, 005h, 000h
        jmp    SHORT loc_7_03A1
        db  080h, 025h, 0B8h, 003h, 000h, 000h, 0F7h, 0EBh, 007h
sub_7_039A:
        db  080h, 025h, 0B8h, 003h, 000h, 000h, 0FBh
loc_7_03A1:
        push   esi
        db  08Bh, 035h, 0D8h, 00Dh, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        call   sub_7_03ED
        db  08Bh, 00Dh, 0B8h, 003h, 000h, 000h
        test   cl, 2
        setne  ah
        test   cl, 8
        setne  al
        xor    ah, al
        jne    SHORT loc_7_03DB
        test   cl, 1
        setne  ah
        test   cl, 4
        setne  al
        xor    ah, al
        jne    SHORT loc_7_03DB
        pop    esi
        ret
loc_7_03DB:
        db  0BEh, 008h, 004h, 000h, 000h
        mov    eax, 3e8h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        pop    esi
        ret
sub_7_03ED:
        db  08Bh, 00Dh, 0B8h, 003h, 000h, 000h
        mov    eax, 2
        test   cl, 2
        jne    SHORT loc_7_0402
        mov    eax, 0ah
loc_7_0402:
        db  0A3h, 0B4h, 003h, 000h, 000h
        ret
loc_7_0408:
        db  08Bh, 00Dh, 0B8h, 003h, 000h, 000h
        test   cl, 2
        setne  ah
        test   cl, 8
        setne  al
        xor    ah, al
        je     SHORT loc_7_0430
        mov    eax, 2
        db  0CDh, 020h, 004h, 000h, 005h, 000h
        db  080h, 025h, 0B8h, 003h, 000h, 000h, 0FDh
loc_7_0430:
        test   cl, 1
        setne  ah
        test   cl, 4
        setne  al
        xor    ah, al
        je     SHORT loc_7_0452
        mov    eax, 0ah
        db  0CDh, 020h, 004h, 000h, 005h, 000h
        db  080h, 025h, 0B8h, 003h, 000h, 000h, 0FEh
loc_7_0452:
        call   sub_7_03ED
        db  0C7h, 005h, 0D8h, 00Dh, 000h, 000h, 000h, 000h, 000h, 000h
        ret
loc_7_0462:
        push   ecx
        push   edi
        push   edx
        call   sub_7_24B0
        add    esp, 0ch
        mov    esi, eax
        xor    ebx, ebx
        ret
loc_7_0472:
        mov    eax, dword ptr [edx + 24h]
        test   byte ptr [eax + 24h], 40h
        jne    SHORT loc_7_0485
        push   edx
        db  0E8h, 0FFh, 0B6h, 0F9h, 0FFh
        add    esp, 4
        ret
loc_7_0485:
        or     byte ptr [edx + 18h], 4
        mov    dword ptr [edx + 48h], 0
        ret
loc_7_0491:
        push   edx
        call   sub_7_19C0
        add    esp, 4
        ret
loc_7_049B:
        push   ecx
        push   edi
        push   edx
        call   sub_7_33B0
        add    esp, 0ch
        mov    esi, eax
        mov    ebx, 42h
        ret
loc_7_04AE:
        push   ecx
        push   edx
        call   sub_7_32E0
        add    esp, 8
        ret
loc_7_04B9:
        push   edx
        call   sub_7_2380
        add    esp, 4
        ret
loc_7_04C3:
        push   ecx
        push   edi
        push   edx
        db  0E8h, 015h, 0FCh, 0F8h, 0FFh
        add    esp, 0ch
        mov    esi, eax
        xor    ebx, ebx
        ret
loc_7_04D3:
        push   ecx
        push   edi
        push   esi
        cld
        mov    edx, esi
        lodsd
        cmp    ax, 5ch
        je     NEAR PTR loc_7_0633
        cmp    ax, 2fh
        je     NEAR PTR loc_7_0633
        rol    eax, 10h
        cmp    ax, 3ah
        rol    eax, 10h
        je     NEAR PTR loc_7_0633
        cmp    eax, 480041h
        jne    SHORT loc_7_0528
        lodsd
        cmp    eax, 20003ah
        jne    SHORT loc_7_0525
        mov    ax, 4
        stosw
        mov    ax, 3fh
        stosw
        mov    eax, 4
        mov    ebx, edx
        jmp    NEAR PTR loc_7_060E
loc_7_0525:
        mov    esi, edx
        lodsd
loc_7_0528:
        and    eax, 0ffdfffdfh
        cmp    eax, 45004eh
        jne    SHORT loc_7_0565
        lodsd
        and    eax, 0ffdfffdfh
        cmp    eax, 510054h
        jne    SHORT loc_7_0565
        lodsw
        cmp    ax, 3ah
        je     SHORT loc_7_054F
        cmp    ax, 2eh
        jne    SHORT loc_7_0551
loc_7_054F:
        lodsw
loc_7_0551:
        cmp    ax, 0
        je     NEAR PTR loc_7_061A
        cmp    ax, 20h
        je     NEAR PTR loc_7_061A
loc_7_0565:
        mov    edi, edx
        shr    ecx, 1
        je     NEAR PTR loc_7_0633
        mov    ax, 3ah
        repne scasw
        jne    NEAR PTR loc_7_0633
        mov    ebx, edi
        mov    ecx, ebx
        mov    edi, edx
        sub    ecx, edi
        shr    ecx, 1
        mov    ax, 2fh
        repne scasw
        je     SHORT loc_7_05B4
        mov    ecx, ebx
        mov    edi, edx
        sub    ecx, edi
        shr    ecx, 1
        mov    ax, 5ch
        repne scasw
        je     SHORT loc_7_05B4
        mov    esi, edx
        mov    edi, dword ptr [esp + 4]
        mov    ax, 4
        stosw
        mov    ax, 2ah
        stosw
        jmp    SHORT loc_7_05E3
loc_7_05B4:
        sub    edi, edx
        mov    ecx, edi
        mov    esi, edx
        mov    edi, dword ptr [esp + 4]
        add    cx, 2
        mov    ax, cx
        stosw
        mov    ax, 5ch
        stosw
        sub    ecx, 4
        push   ecx
        push   esi
        push   edi
        add    edi, ecx
        add    esi, ecx
        db  0CDh, 020h, 043h, 000h, 040h, 000h
        add    esp, 0ch
        add    esi, 2
loc_7_05E3:
        mov    ecx, ebx
        sub    ecx, esi
        mov    ax, cx
        stosw
        push   ecx
        push   esi
        sub    ecx, 2
        push   ecx
        push   esi
        push   edi
        add    esi, ecx
        add    edi, ecx
        db  0CDh, 020h, 043h, 000h, 040h, 000h
        add    esp, 0ch
        add    esi, 2
        pop    ebx
        pop    eax
        cmp    dword ptr [esi], 5ch
        jne    SHORT loc_7_060E
        add    esi, 2
loc_7_060E:
        add    esp, 0ch
        xor    edx, edx
        db  08Bh, 015h, 0A0h, 005h, 000h, 000h
        ret
loc_7_061A:
        mov    ax, 4
        stosw
        mov    ax, 2ah
        stosw
        mov    eax, 4
        mov    esi, edx
        add    esi, ecx
        mov    ebx, edx
        jmp    SHORT loc_7_060E
loc_7_0633:
        pop    esi
        pop    edi
        pop    ecx
        db  0FFh, 025h, 0E4h, 00Dh, 000h, 000h
loc_7_063C:
        mov    al, byte ptr [ebp + 1ch]
        cmp    al, 0c0h
        jb     SHORT loc_7_0650
        cmp    al, 0cfh
        ja     SHORT loc_7_0650
loc_7_0647:
        db  08Bh, 035h, 0A0h, 005h, 000h, 000h
        and    eax, eax
        ret
loc_7_0650:
        db  0FFh, 025h, 0E0h, 00Dh, 000h, 000h
sub_7_0656:
        db  080h, 03Dh, 0F4h, 00Dh, 000h, 000h, 000h
        je     SHORT loc_7_06DA
        mov    eax, dword ptr [esp + 0ch]
        db  03Bh, 005h, 0A0h, 005h, 000h, 000h
        je     SHORT loc_7_0670
        cmp    eax, -1
        jne    SHORT loc_7_06D4
loc_7_0670:
        mov    eax, dword ptr [esp + 8]
        mov    al, byte ptr [eax + 1dh]
        mov    edx, edi
        db  0BFh, 017h, 00Eh, 000h, 000h
        mov    ecx, 47h
        repne scasb
        xchg   edi, edx
        jne    SHORT loc_7_06BC
        db  081h, 0EAh, 017h, 00Eh, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 10h]
        mov    word ptr [eax + 42h], cx
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 44h], ecx
        db  0FFh, 014h, 095h, 05Ch, 00Eh, 000h, 000h
        cmp    eax, 2
        je     SHORT loc_7_06BB
        and    eax, eax
        jne    SHORT loc_7_06BC
        mov    eax, dword ptr [esp + 0ch]
        cmp    eax, -1
        je     SHORT loc_7_06D4
loc_7_06BB:
        ret
loc_7_06BC:
        mov    eax, dword ptr [esp + 0ch]
        db  03Bh, 005h, 0A0h, 005h, 000h, 000h
        jne    SHORT loc_7_06D4
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 1ch]
        and    eax, 0ffffh
loc_7_06D4:
        db  0FFh, 025h, 0DCh, 00Dh, 000h, 000h
loc_7_06DA:
        mov    eax, dword ptr [esp + 8]
        mov    dl, 80h
        cmp    word ptr [eax + 1ch], 1
        je     SHORT loc_7_06F0
        mov    dl, 0
        cmp    word ptr [eax + 1ch], 2
        jne    SHORT loc_7_06D4
loc_7_06F0:
        mov    eax, dword ptr [esp + 4]
        movzx  eax, byte ptr [eax + 4]
        dec    al
        cmp    al, 20h
        jae    SHORT loc_7_06D4
        db  088h, 090h, 0F6h, 00Dh, 000h, 000h
        jmp    SHORT loc_7_06D4
loc_7_0706:
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        and    eax, 0ffffh
        mov    eax, 1
        ret
        db  08Bh, 044h, 024h, 00Ch, 08Bh, 040h, 01Ch, 025h, 0FFh, 0FFh, 000h, 000h, 0B8h, 001h, 000h, 000h
        db  000h, 0C3h
loc_7_072A:
        cmp    word ptr [ebp + 1ch], 1684h
        jne    SHORT loc_7_074F
        cmp    word ptr [ebp + 10h], 1ah
        jne    SHORT loc_7_074F
        db  0A1h, 0F0h, 00Dh, 000h, 000h
        mov    word ptr [ebp], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        db  0CDh, 020h, 047h, 080h, 001h, 000h
loc_7_074F:
        db  066h, 08Bh, 00Dh, 0EAh, 00Dh, 000h, 000h
        db  08Bh, 015h, 0ECh, 00Dh, 000h, 000h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_7_0762:
        movzx  eax, word ptr [ebp + 1ch]
        mov    word ptr [ebp + 1ch], 0
        db  0CDh, 020h, 04Ah, 080h, 001h, 000h
        pushad
        pushfd
        push   0
        push   0
        push   0
        db  0CDh, 020h, 00Fh, 000h, 040h, 000h
        add    esp, 0ch
        and    eax, eax
        je     SHORT loc_7_0787
loc_7_0787:
        popfd
        popad
        ret
sub_7_078A:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        cld
        jcxz   loc_7_07A4
loc_7_079C:
        lodsb
        and    al, al
        je     SHORT loc_7_07A4
        stosb
        loop   loc_7_079C
loc_7_07A4:
        mov    byte ptr [edi], 0
        pop    edi
        pop    esi
        ret
sub_7_07AA:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
        cld
        xor    ecx, ecx
        mov    al, cl
        dec    ecx
        repne scasb
        dec    edi
        mov    ecx, dword ptr [esp + 14h]
        jcxz   loc_7_07A4
        jmp    SHORT loc_7_079C
sub_7_07C6:
        mov    edx, dword ptr [esp + 4]
        xor    eax, eax
loc_7_07CC:
        mov    al, byte ptr [edx]
        and    al, al
        je     SHORT loc_7_07EA
        db  08Ah, 080h, 0A0h, 004h, 000h, 000h
        and    al, al
        je     SHORT loc_7_07E1
        mov    byte ptr [edx], al
        inc    edx
        jmp    SHORT loc_7_07CC
loc_7_07E1:
        inc    edx
        test   byte ptr [edx], 0ffh
        je     SHORT loc_7_07EA
        inc    edx
        jmp    SHORT loc_7_07CC
loc_7_07EA:
        ret
sub_7_07EB:
        push   esi
        mov    esi, dword ptr [esp + 8]
loc_7_07F0:
        test   word ptr [esi], 0ffffh
        je     SHORT loc_7_0809
        push   2
        push   esi
        push   esi
        db  0CDh, 020h, 043h, 000h, 040h, 000h
        add    esp, 0ch
        add    esi, 2
        jmp    SHORT loc_7_07F0
loc_7_0809:
        pop    esi
        ret
sub_7_080B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
        cld
        xor    eax, eax
loc_7_0817:
        lodsb
        or     al, al
        je     SHORT loc_7_0823
        cmp    al, dl
        jne    SHORT loc_7_0817
        dec    esi
        mov    eax, esi
loc_7_0823:
        pop    esi
        ret
sub_7_0825:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        cld
        rep movsb
        pop    edi
        pop    esi
        ret
sub_7_0839:
        cld
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        jcxz   loc_7_086B
        mov    edx, ecx
        mov    edi, dword ptr [ebp + 8]
        mov    esi, edi
        xor    eax, eax
        repne scasb
        neg    ecx
        add    ecx, edx
        mov    edi, esi
        mov    esi, dword ptr [ebp + 0ch]
        repe cmpsb
        mov    al, byte ptr [esi - 1]
        xor    ecx, ecx
        cmp    al, byte ptr [edi - 1]
        ja     SHORT loc_7_0869
        je     SHORT loc_7_086B
        dec    ecx
        dec    ecx
loc_7_0869:
        not    ecx
loc_7_086B:
        mov    eax, ecx
        pop    esi
        pop    edi
        pop    ebp
        ret
sub_7_0871:
        push   edi
        db  0BFh, 0D8h, 005h, 000h, 000h
        mov    ecx, 0ffh
loc_7_087C:
        repne scasd
        jne    SHORT loc_7_0899
        mov    eax, edi
        db  02Dh, 0D8h, 005h, 000h, 000h
        db  08Bh, 090h, 0D0h, 009h, 000h, 000h
        cmp    dword ptr [edx + 8], 0
        jne    SHORT loc_7_087C
        shr    eax, 2
        test   ah, 80h
loc_7_0899:
        pop    edi
        ret
sub_7_089B:
        mov    eax, dword ptr [esp + 4]
        call   sub_7_0871
        jne    SHORT loc_7_08A7
        ret
loc_7_08A7:
        push   eax
        db  0E8h, 043h, 024h, 0F9h, 0FFh
        movzx  eax, al
        add    esp, 4
        ret
sub_7_08B4:
        mov    eax, dword ptr [esp + 4]
        call   sub_7_0871
        sete   al
        movzx  eax, al
        ret
sub_7_08C4:
        mov    eax, dword ptr [esp + 4]
        test   eax, 0ffff0000h
        je     SHORT loc_7_08D6
        call   sub_7_0871
        jne    SHORT loc_7_08F2
loc_7_08D6:
        db  0C7h, 004h, 085h, 0D4h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 004h, 085h, 0D0h, 009h, 000h, 000h
        push   eax
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
        add    esp, 4
loc_7_08F2:
        ret
sub_7_08F3:
        push   ebx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        mov    byte ptr [ebx + 1dh], 51h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    bx, word ptr [ebx + 10h]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
loc_7_093A:
        movzx  eax, bx
        shl    eax, 4
        mov    cx, word ptr [eax + 16h]
        db  066h, 03Bh, 00Dh, 0E8h, 00Dh, 000h, 000h
        je     SHORT loc_7_0955
        cmp    cx, bx
        mov    bx, cx
        jne    SHORT loc_7_093A
loc_7_0955:
        mov    ax, word ptr [eax + 2ch]
        pop    ebx
        ret
        db  08Bh, 044h, 024h, 004h, 0CDh, 020h, 003h, 000h, 005h, 000h, 0C3h, 08Bh, 044h, 024h, 004h, 0CDh
        db  020h, 004h, 000h, 005h, 000h, 0C3h, 08Bh, 0C7h, 0CDh, 020h, 008h, 001h, 001h, 000h, 097h, 0C3h
sub_7_097B:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
sub_7_0986:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
sub_7_0991:
        push   ebx
        mov    ebx, dword ptr [esp + 8]
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
sub_7_09A0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        db  0CDh, 020h, 01Dh, 000h, 001h, 000h
        jae    SHORT loc_7_09B0
loc_7_09B0:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 0ch]
        mov    word ptr [eax], cx
        ret
sub_7_09BE:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
loc_7_09C4:
        push   0
        push   edx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        ret
sub_7_09D1:
        mov    edx, dword ptr [esp + 4]
        push   esi
        db  0BEh, 0C4h, 009h, 000h, 000h
        db  0CDh, 020h, 024h, 000h, 001h, 000h
        pop    esi
        ret
sub_7_09E3:
        mov    eax, dword ptr [esp + 4]
        db  0CDh, 020h, 01Ch, 080h, 001h, 000h
sub_7_09ED:
        db  0CDh, 020h, 0CFh, 080h, 001h, 000h
sub_7_09F3:
        db  0CDh, 020h, 03Fh, 080h, 001h, 000h
        db  0CDh, 020h, 038h, 080h, 001h, 000h
sub_7_09FF:
        mov    eax, dword ptr [esp + 4]
        db  0CDh, 020h, 061h, 081h, 001h, 000h
sub_7_0A09:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 10h]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret
        db  09Ch, 060h, 08Bh, 074h, 024h, 028h, 0CDh, 020h, 0C2h, 000h, 001h, 000h, 061h, 09Dh, 0C3h, 0CDh
        db  020h, 0C4h, 000h, 001h, 000h, 075h, 002h, 0B0h, 000h, 0C3h
sub_7_0A3A:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        mov    eax, 0
        jb     SHORT loc_7_0A4E
        mov    eax, edx
loc_7_0A4E:
        pop    esi
        ret
sub_7_0A50:
        mov    eax, dword ptr [esp + 4]
        mov    edx, eax
        and    edx, 0fffh
        add    edx, dword ptr [esp + 8]
        add    edx, 0fffh
        shr    edx, 0ch
        shr    eax, 0ch
        push   0c0000000h
        push   edx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        and    eax, eax
        je     SHORT loc_7_0A8C
        mov    edx, dword ptr [esp + 4]
        and    edx, 0fffh
        or     eax, edx
loc_7_0A8C:
        ret
sub_7_0A8D:
        mov    eax, dword ptr [esp + 4]
        mov    edx, eax
        and    edx, 0fffh
        add    edx, dword ptr [esp + 8]
        add    edx, 0fffh
        shr    edx, 0ch
        shr    eax, 0ch
        push   40000000h
        push   edx
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        ret
sub_7_0ABA:
        db  0CDh, 020h, 040h, 080h, 040h, 000h
sub_7_0AC0:
        db  0CDh, 020h, 042h, 080h, 040h, 000h
sub_7_0AC6:
        db  0CDh, 020h, 044h, 080h, 040h, 000h
sub_7_0ACC:
        db  0CDh, 020h, 039h, 080h, 040h, 000h
sub_7_0AD2:
        db  0CDh, 020h, 060h, 080h, 040h, 000h
sub_7_0AD8:
        db  0CDh, 020h, 00Dh, 080h, 040h, 000h
sub_7_0ADE:
        db  0CDh, 020h, 00Eh, 080h, 040h, 000h
sub_7_0AE4:
        db  0CDh, 020h, 012h, 080h, 040h, 000h
sub_7_0AEA:
        db  0CDh, 020h, 013h, 080h, 040h, 000h
sub_7_0AF0:
        db  0CDh, 020h, 01Bh, 080h, 040h, 000h
sub_7_0AF6:
        push   ebx
        mov    eax, dword ptr [esp + 8]
        push   eax
        xor    eax, eax
        xor    ebx, ebx
        db  0CDh, 020h, 073h, 000h, 040h, 000h
        add    esp, 4
        pop    ebx
        ret
sub_7_0B0B:
        db  0CDh, 020h, 02Ah, 080h, 040h, 000h
sub_7_0B11:
        db  0CDh, 020h, 02Ch, 080h, 040h, 000h
sub_7_0B17:
        db  0CDh, 020h, 02Bh, 080h, 040h, 000h
sub_7_0B1D:
        db  0CDh, 020h, 02Dh, 080h, 040h, 000h
sub_7_0B23:
        db  0CDh, 020h, 05Eh, 080h, 040h, 000h
sub_7_0B29:
        db  0CDh, 020h, 05Dh, 080h, 040h, 000h
sub_7_0B2F:
        db  0CDh, 020h, 066h, 080h, 040h, 000h
sub_7_0B35:
        db  0CDh, 020h, 06Bh, 080h, 040h, 000h
        db  0CDh, 020h, 061h, 080h, 040h, 000h
sub_7_0B41:
        db  0CDh, 020h, 06Ah, 080h, 040h, 000h
sub_7_0B47:
        db  0CDh, 020h, 000h, 000h, 094h, 004h
        mov    eax, 0ffffffffh
        adc    eax, 0
        ret
sub_7_0B56:
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 18h]
        db  0CDh, 020h, 001h, 000h, 094h, 004h
        pop    esi
        pop    edi
        ret
loc_7_0B71:
        db  0CDh, 020h, 002h, 080h, 052h, 004h
sub_7_0B77:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 14h]
        mov    ebx, 2
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        mov    edi, dword ptr [esp + 18h]
        mov    word ptr [edi], cx
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_7_0B98:
        push   esi
        push   ebx
        mov    esi, dword ptr [esp + 0ch]
        mov    ebx, 9
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    ebx
        pop    esi
        ret
sub_7_0BAC:
        push   edi
        push   esi
        push   ebx
        mov    ebx, 1ah
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    ebx
        pop    esi
        pop    edi
        ret
sub_7_0BBE:
        push   edi
        push   esi
        push   ebx
        mov    ebx, 0dh
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        mov    eax, ecx
        pop    ebx
        pop    esi
        pop    edi
        ret
sub_7_0BD2:
        push   ebx
        mov    eax, 0ffh
        mov    edx, dword ptr [esp + 8]
        movzx  edx, word ptr [edx]
        mov    ebx, 0
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        mov    ebx, dword ptr [esp + 8]
        mov    word ptr [ebx], dx
        pop    ebx
        ret
sub_7_0BF3:
        push   ebx
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        movzx  edx, word ptr [edx]
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 14h]
        mov    ebx, 0eh
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        mov    ebx, dword ptr [esp + 0ch]
        mov    word ptr [ebx], dx
        pop    esi
        pop    ebx
        ret
sub_7_0C19:
        push   ebx
        movzx  edx, word ptr [esp + 8]
        mov    ebx, 1
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    ebx
        ret
sub_7_0C2C:
        db  080h, 03Dh, 097h, 003h, 000h, 000h, 000h
        jne    SHORT loc_7_0C49
loc_7_0C35:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    ebx, 3
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    esi
        pop    ebx
        ret
loc_7_0C49:
        mov    edx, dword ptr [esp + 4]
        cmp    dword ptr [edx + 4], 0
        je     SHORT loc_7_0C35
        mov    ecx, esi
        db  0BEh, 063h, 00Ch, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    esi, ecx
        ret
loc_7_0C63:
        mov    esi, edx
        mov    ebx, 3
        db  0CDh, 020h, 001h, 080h, 087h, 004h
sub_7_0C70:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        xor    ecx, ecx
        mov    ebx, 4
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    esi
        pop    ebx
        ret
loc_7_0C86:
        ret
sub_7_0C87:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    ebx, 6
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    esi
        pop    ebx
        and    al, al
        jne    SHORT loc_7_0C86
        ret
sub_7_0C9F:
        push   ebx
        mov    ebx, 0ah
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        pop    ebx
        ret
sub_7_0CAD:
        push   ebx
        mov    ebx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 004h, 000h, 087h, 004h
        cmc
        sbb    edx, edx
        and    eax, edx
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_7_0CD0:
        push   ebx
        db  0C6h, 005h, 096h, 003h, 000h, 000h, 001h
        db  080h, 03Dh, 095h, 003h, 000h, 000h, 000h
        jne    SHORT loc_7_0CE6
        db  0E8h, 049h, 0F3h, 0F8h, 0FFh
loc_7_0CE6:
        db  080h, 03Dh, 012h, 004h, 000h, 000h, 000h
        je     SHORT loc_7_0D1B
        db  0FEh, 005h, 013h, 004h, 000h, 000h
        db  080h, 03Dh, 012h, 004h, 000h, 000h, 000h
        je     SHORT loc_7_0D15
        sub    ebx, ebx
loc_7_0D00:
        db  068h, 012h, 004h, 000h, 000h
        call   sub_7_0AE4
        add    esp, 4
        db  038h, 01Dh, 012h, 004h, 000h, 000h
        jne    SHORT loc_7_0D00
loc_7_0D15:
        db  0FEh, 00Dh, 013h, 004h, 000h, 000h
loc_7_0D1B:
        db  0C6h, 005h, 012h, 004h, 000h, 000h, 001h
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_7_0D30:
        db  0C6h, 005h, 012h, 004h, 000h, 000h, 000h
        db  080h, 03Dh, 013h, 004h, 000h, 000h, 000h
        je     SHORT loc_7_0D4D
        db  068h, 012h, 004h, 000h, 000h
        call   sub_7_0AEA
        add    esp, 4
loc_7_0D4D:
        ret
        db  08Bh, 0FFh
sub_7_0D50:
        push   esi
        db  0C6h, 005h, 096h, 003h, 000h, 000h, 001h
        db  080h, 03Dh, 095h, 003h, 000h, 000h, 000h
        jne    SHORT loc_7_0D66
        db  0E8h, 0C9h, 0F2h, 0F8h, 0FFh
loc_7_0D66:
        mov    esi, dword ptr [esp + 8]
        mov    al, byte ptr [esi + 35h]
        or     al, al
        je     SHORT loc_7_0D8A
        inc    byte ptr [esi + 36h]
        or     al, al
        je     SHORT loc_7_0D87
loc_7_0D78:
        push   esi
        call   sub_7_0AE4
        add    esp, 4
        cmp    byte ptr [esi + 35h], 0
        jne    SHORT loc_7_0D78
loc_7_0D87:
        dec    byte ptr [esi + 36h]
loc_7_0D8A:
        mov    byte ptr [esi + 35h], 1
        pop    esi
        ret
sub_7_0D90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 35h], 0
        cmp    byte ptr [eax + 36h], 0
        jbe    SHORT loc_7_0DB2
        push   eax
        call   sub_7_0AEA
        add    esp, 4
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_7_0DB2:
        mov    cl, byte ptr [eax + 58h]
        test   cl, 4
        je     SHORT loc_7_0DD9
        and    cl, 0fbh
        mov    byte ptr [eax + 58h], cl
        mov    dword ptr [ebp - 8], eax
        db  0C7h, 045h, 0FCh, 0AFh, 048h, 000h, 000h
        mov    edx, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp - 4]
        db  0CDh, 020h, 010h, 000h, 001h, 000h
        mov    dword ptr [ebp - 0ch], esi
loc_7_0DD9:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_7_0DE0:
        movsx  eax, cl
        cmp    eax, 5ch
        je     SHORT loc_7_0E00
        cmp    eax, 0aah
        je     SHORT loc_7_0E03
        cmp    eax, 0aeh
        je     SHORT loc_7_0E06
        cmp    eax, 0bfh
        je     SHORT loc_7_0E09
        mov    al, cl
        ret
loc_7_0E00:
        mov    al, 13h
        ret
loc_7_0E03:
        mov    al, 12h
        ret
loc_7_0E06:
        mov    al, 11h
        ret
loc_7_0E09:
        mov    al, 10h
        ret
        db  08Dh, 064h, 024h, 000h
sub_7_0E10:
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 8]
        push   eax
        call   sub_7_0E30
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_7_0E30:
        push   ebp
        mov    ebp, esp
        sub    esp, 44h
        push   ebx
        push   esi
        push   edi
        mov    edi, edx
        mov    edx, ecx
        mov    dword ptr [ebp - 28h], ecx
        mov    ecx, dword ptr [edx + 40h]
        mov    edx, dword ptr [ebp - 28h]
        mov    dword ptr [ebp - 18h], ecx
        mov    ecx, dword ptr [edx + 44h]
        mov    dword ptr [ebp - 14h], ecx
        mov    esi, dword ptr [edx + 48h]
        db  0C6h, 005h, 096h, 003h, 000h, 000h, 001h
        db  080h, 03Dh, 095h, 003h, 000h, 000h, 000h
        jne    SHORT loc_7_0E67
        db  0E8h, 0C8h, 0F1h, 0F8h, 0FFh
loc_7_0E67:
        mov    edx, dword ptr [ebp - 28h]
        cmp    byte ptr [edx + 34h], 0ffh
        je     SHORT loc_7_0E80
        mov    al, 33h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_7_0E80:
        mov    ecx, dword ptr [ebp - 18h]
        cmp    byte ptr [ecx + 8], 0
        je     SHORT loc_7_0E94
        mov    al, 39h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_7_0E94:
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 0c6h
        mov    dword ptr [ebp - 10h], eax
        add    eax, 262h
        mov    dword ptr [ebp - 38h], eax
        mov    word ptr [ebp - 2], 0
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 95h], 3
        movsx  eax, byte ptr [eax]
        sub    eax, 41h
        cmp    eax, 17h
        ja     SHORT loc_7_0ED2
        db  0FFh, 024h, 085h, 0ACh, 018h, 000h, 000h
loc_7_0ED2:
        mov    al, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_7_0EE0:
        mov    ecx, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 95h], 0
        db  0B9h, 0BCh, 013h, 000h, 000h
        add    edx, 0aeh
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        db  0B8h, 0CCh, 013h, 000h, 000h
        mov    edx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx + 1ch], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 20h], ax
        add    edx, 1ch
        mov    ecx, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 28h]
        mov    byte ptr [ecx + 0adh], 11h
        mov    dword ptr [ecx + 90h], edx
        mov    ecx, dword ptr [ebp - 14h]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 90h], edx
        mov    word ptr [edx + 6ch], 302h
        jmp    NEAR PTR loc_7_11D8
loc_7_0F4A:
        mov    ecx, dword ptr [ebp - 14h]
        or     esi, esi
        mov    byte ptr [ecx + 95h], 4
        jne    SHORT loc_7_0F63
        mov    al, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_7_0F63:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        mov    dword ptr [esi + 3ch], eax
        or     eax, eax
        je     SHORT loc_7_0F7A
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        mov    dword ptr [esi + 40h], eax
loc_7_0F7A:
        mov    ecx, dword ptr [ebp - 18h]
        mov    word ptr [ebp - 2], 24h
        mov    dword ptr [ecx + 98h], 0
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 0c0h], 0
        add    dword ptr [ebp - 10h], 24h
        jmp    NEAR PTR loc_7_11D8
loc_7_0FA3:
        mov    ecx, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 28h]
        mov    byte ptr [ecx + 95h], 1
        cmp    byte ptr [edx + 5ch], 0
        jne    SHORT loc_7_1032
        mov    ecx, edx
        add    ecx, 78h
        mov    edx, dword ptr [edx + 40h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx + 0aeh], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 0b2h], eax
        add    edx, 0aeh
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        lea    eax, [ebp - 0ah]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 1ch
        push   eax
        push   ecx
        call   sub_7_0B77
        add    esp, 0ch
        or     al, al
        je     SHORT loc_7_1005
        db  066h, 0FFh, 005h, 0C0h, 020h, 000h, 000h
        mov    al, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_7_1005:
        mov    ax, word ptr [ebp - 0ah]
        imul   ax, ax, 0a5h
        mov    edx, dword ptr [ebp - 28h]
        add    ax, 113h
        mov    word ptr [edx + 6ch], ax
        mov    ax, word ptr [ebp - 0ah]
        imul   ax, ax, 226h
        add    ax, 113h
        mov    word ptr [edx + 6eh], ax
        mov    ax, word ptr [ebp - 0ah]
        mov    word ptr [edx + 52h], ax
loc_7_1032:
        mov    ecx, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 28h]
        mov    byte ptr [ecx + 0adh], 11h
        mov    dword ptr [ecx + 90h], edx
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ebp - 2], 8
        mov    dword ptr [ecx + 90h], edx
        mov    eax, dword ptr [ebp - 10h]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 4], 0
        add    dword ptr [ebp - 10h], 6
        jmp    NEAR PTR loc_7_11D8
loc_7_1067:
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 95h], 2
loc_7_1071:
        cmp    byte ptr [edi + 41h], 0
        jne    NEAR PTR loc_7_1183
        mov    eax, dword ptr [edi + 8]
        test   eax, 0ffff0000h
        jne    NEAR PTR loc_7_1173
        jmp    NEAR PTR loc_7_117C
        db  08Bh, 0FFh
loc_7_1090:
        mov    ecx, dword ptr [ebp - 14h]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 95h], 5
        mov    ecx, dword ptr [ebp - 18h]
        add    ebx, 4
        mov    ax, word ptr [ecx + 0b8h]
        mov    word ptr [ecx + 96h], ax
        mov    ax, word ptr [ebx - 4]
        mov    word ptr [ecx + 0b8h], ax
        jmp    NEAR PTR loc_7_11D8
loc_7_10C1:
        mov    edx, dword ptr [ebp - 28h]
        mov    dx, word ptr [edx + 9ch]
        or     dx, dx
        jne    SHORT loc_7_10E0
        mov    al, 32h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_7_10E0:
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 95h], 6
        mov    ecx, dword ptr [ebp - 18h]
        mov    ax, word ptr [ecx + 0b8h]
        mov    word ptr [ecx + 96h], ax
        mov    word ptr [ecx + 0b8h], dx
        cmp    byte ptr [edi + 41h], 0
        jne    SHORT loc_7_1124
        mov    eax, dword ptr [edi + 8]
        test   eax, 0ffff0000h
        jne    SHORT loc_7_1114
        jmp    SHORT loc_7_111D
loc_7_1114:
        push   eax
        call   sub_7_089B
        add    esp, 4
loc_7_111D:
        mov    byte ptr [edi + 40h], al
        mov    byte ptr [edi + 41h], 0ffh
loc_7_1124:
        mov    eax, dword ptr [ebp - 10h]
        mov    cl, byte ptr [edi + 40h]
        mov    byte ptr [eax + 4], cl
        add    dword ptr [ebp - 10h], 6
        mov    edx, dword ptr [ebp - 28h]
        mov    word ptr [ebp - 2], 8
        mov    eax, dword ptr [ebp - 10h]
        mov    cl, byte ptr [edx + 9eh]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 10h]
        add    dword ptr [ebp - 10h], 2
        mov    eax, dword ptr [ebp - 10h]
        mov    cl, byte ptr [edx + 9fh]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 10h]
        add    dword ptr [ebp - 10h], 4
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ebx]
        add    ebx, 4
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 10h], 2
        jmp    SHORT loc_7_11D8
loc_7_1173:
        push   eax
        call   sub_7_089B
        add    esp, 4
loc_7_117C:
        mov    byte ptr [edi + 40h], al
        mov    byte ptr [edi + 41h], 0ffh
loc_7_1183:
        mov    eax, dword ptr [ebp - 10h]
        mov    cl, byte ptr [edi + 40h]
        mov    byte ptr [eax + 4], cl
        add    dword ptr [ebp - 10h], 6
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 2], 8
        cmp    byte ptr [eax], 4ch
        jne    SHORT loc_7_11A7
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 57h
        inc    dword ptr [ebp - 10h]
loc_7_11A7:
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    byte ptr [eax], 44h
        je     SHORT loc_7_11BF
        mov    cl, byte ptr [ebx]
        add    ebx, 4
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 10h]
loc_7_11BF:
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax], 53h
        jne    SHORT loc_7_11D8
        add    dword ptr [ebp - 10h], 2
        mov    cl, byte ptr [ebx]
        add    ebx, 4
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 10h]
loc_7_11D8:
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, ecx
        mov    word ptr [ecx + 22h], 1
        add    eax, 0a8h
        mov    edx, dword ptr [ebp - 28h]
        mov    dword ptr [ecx + 24h], eax
        mov    ecx, dword ptr [ebp - 14h]
        mov    al, byte ptr [edx + 54h]
        mov    byte ptr [ecx + 0b7h], al
        xor    ecx, ecx
        mov    cx, word ptr [edx + 6eh]
        and    eax, 0ffh
        imul   eax, ecx
        cmp    eax, 1d4c0h
        jle    SHORT loc_7_122B
        mov    eax, 1d4c0h
        cdq
        idiv   ecx
        mov    ecx, dword ptr [ebp - 14h]
        cmp    al, 5
        mov    byte ptr [ecx + 0b7h], al
        jae    SHORT loc_7_122B
        mov    byte ptr [ecx + 0b7h], 5
loc_7_122B:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 24h], eax
loc_7_1231:
        inc    dword ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 24h]
        movsx  eax, byte ptr [eax]
        cmp    eax, 62h
        jg     SHORT loc_7_1298
        je     NEAR PTR loc_7_13D1
        cmp    eax, 3dh
        jg     SHORT loc_7_1272
        je     NEAR PTR loc_7_12D6
        or     eax, eax
        je     NEAR PTR loc_7_15A1
        cmp    eax, 21h
        je     SHORT loc_7_12A7
        cmp    eax, 2bh
        je     SHORT loc_7_12B7
        cmp    eax, 2ch
        je     NEAR PTR loc_7_15A1
        cmp    eax, 2dh
        je     SHORT loc_7_12DF
        jmp    SHORT loc_7_12E8
loc_7_1272:
        cmp    eax, 44h
        je     NEAR PTR loc_7_1300
        cmp    eax, 45h
        je     NEAR PTR loc_7_1310
        cmp    eax, 57h
        je     NEAR PTR loc_7_1390
        cmp    eax, 5fh
        je     NEAR PTR loc_7_13A5
        jmp    SHORT loc_7_12E8
loc_7_1298:
        sub    eax, 63h
        cmp    eax, 14h
        ja     SHORT loc_7_12E8
        db  0FFh, 024h, 085h, 00Ch, 019h, 000h, 000h
loc_7_12A7:
        mov    ecx, dword ptr [ebp - 14h]
        db  0A0h, 05Fh, 004h, 000h, 000h
        mov    byte ptr [ecx + 0b7h], al
        jmp    SHORT loc_7_12E8
loc_7_12B7:
        add    ebx, 4
        mov    eax, ebx
        add    ebx, 4
        mov    al, byte ptr [eax - 4]
        and    eax, 0ffh
        mov    dl, byte ptr [ebx - 4]
        mov    ecx, dword ptr [ebp - 18h]
        add    byte ptr [eax + ecx + 0cch], dl
        jmp    SHORT loc_7_12E8
loc_7_12D6:
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 0
        inc    dword ptr [ebp - 10h]
loc_7_12DF:
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 0
loc_7_12E5:
        inc    dword ptr [ebp - 10h]
loc_7_12E8:
        mov    eax, dword ptr [ebp - 38h]
        cmp    dword ptr [ebp - 10h], eax
        jbe    NEAR PTR loc_7_1231
        mov    al, 0ffh
loc_7_12F6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_7_1300:
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx], eax
        add    dword ptr [ebp - 10h], 4
        jmp    SHORT loc_7_12E8
loc_7_1310:
        mov    esi, dword ptr [ebx]
        add    ebx, 4
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        mov    dword ptr [ebp - 20h], eax
        mov    al, byte ptr [ebx]
        add    ebx, 4
        or     al, al
        jne    SHORT loc_7_1334
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 0
        mov    edi, dword ptr [ebp - 10h]
        inc    dword ptr [ebp - 10h]
        jmp    SHORT loc_7_1343
loc_7_1334:
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 18h]
        lea    edi, [eax + ecx + 0ceh]
loc_7_1343:
        cmp    dword ptr [ebp - 20h], esi
        jbe    SHORT loc_7_12E8
loc_7_1348:
        mov    ax, word ptr [esi]
        or     ax, ax
        je     SHORT loc_7_12E8
        inc    byte ptr [edi]
        sub    ax, 2
        and    eax, 0ffffh
        push   1
        push   eax
        push   eax
        lea    eax, [esi + 2]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        inc    eax
        push   eax
        call   sub_7_0ABA
        add    esp, 14h
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx], al
        and    eax, 0ffffh
        inc    eax
        add    dword ptr [ebp - 10h], eax
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    esi, eax
        cmp    dword ptr [ebp - 20h], esi
        ja     SHORT loc_7_1348
        jmp    NEAR PTR loc_7_12E8
        db  090h
loc_7_1390:
        mov    ax, word ptr [ebx]
        add    ebx, 4
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 10h], 2
        jmp    NEAR PTR loc_7_12E8
loc_7_13A5:
        mov    cx, word ptr [ebx]
        add    ebx, 4
        mov    ax, cx
        dec    cx
        or     ax, ax
        je     NEAR PTR loc_7_12E8
loc_7_13B9:
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 0
        mov    ax, cx
        inc    dword ptr [ebp - 10h]
        dec    cx
        or     ax, ax
        jne    SHORT loc_7_13B9
        jmp    NEAR PTR loc_7_12E8
loc_7_13D1:
        mov    cl, byte ptr [ebx]
        add    ebx, 4
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], cl
        jmp    NEAR PTR loc_7_12E5
loc_7_13E0:
        mov    esi, dword ptr [ebx]
        add    ebx, 4
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    eax, ecx
        and    eax, 0ffffh
        mov    edi, dword ptr [ebp - 10h]
        lea    edx, [eax + 1]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        inc    eax
        add    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_7_12E8
loc_7_1417:
        add    ebx, 4
        mov    eax, dword ptr [ebx - 4]
        mov    dword ptr [ebp - 34h], eax
        mov    eax, dword ptr [ebp - 34h]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx], eax
        add    dword ptr [ebp - 10h], 4
        jmp    NEAR PTR loc_7_12E8
        db  08Bh, 0FFh
loc_7_1440:
        mov    esi, dword ptr [ebx]
        add    ebx, 4
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 2ch], eax
        mov    byte ptr [eax], 0
        inc    dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 1ch], esi
        jbe    SHORT loc_7_14B5
        mov    edi, edi
loc_7_1460:
        mov    ax, word ptr [esi]
        sub    ax, 2
        and    eax, 0ffffh
        push   1
        push   eax
        push   eax
        lea    eax, [esi + 2]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_7_0ABA
        add    esp, 14h
        mov    di, ax
        or     di, di
        je     SHORT loc_7_149E
loc_7_1488:
        mov    eax, dword ptr [ebp - 10h]
        mov    cl, byte ptr [eax]
        call   sub_7_0DE0
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 10h]
        dec    di
        jne    SHORT loc_7_1488
loc_7_149E:
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    esi, eax
        cmp    dword ptr [ebp - 1ch], esi
        jbe    SHORT loc_7_14B5
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 5ch
        inc    dword ptr [ebp - 10h]
        jmp    SHORT loc_7_1460
loc_7_14B5:
        mov    ecx, dword ptr [ebp - 10h]
        sub    ecx, dword ptr [ebp - 2ch]
        mov    eax, dword ptr [ebp - 2ch]
        dec    cl
        mov    byte ptr [eax], cl
        jmp    NEAR PTR loc_7_12E8
loc_7_14C7:
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        mov    cl, byte ptr [eax + 1eh]
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], cl
        jmp    NEAR PTR loc_7_12E5
loc_7_14D9:
        mov    esi, dword ptr [ebx]
        add    ebx, 4
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [ebp - 10h]
        dec    ecx
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 10h]
        xor    eax, eax
        mov    al, cl
        mov    edi, dword ptr [ebp - 10h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_7_12E8
loc_7_1511:
        add    ebx, 4
        mov    eax, ebx
        add    ebx, 4
        xor    edx, edx
        mov    dx, word ptr [ebx - 4]
        mov    edi, dword ptr [ebp - 10h]
        mov    esi, dword ptr [eax - 4]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        add    dword ptr [ebp - 10h], edx
        jmp    NEAR PTR loc_7_12E8
        db  005h, 000h, 000h, 000h, 000h
loc_7_1540:
        add    ebx, 4
        mov    ax, word ptr [ebx - 4]
        mov    word ptr [ebp - 6], ax
        mov    ax, word ptr [ebp - 6]
        xchg   al, ah
        mov    word ptr [ebp - 4], ax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 10h], 2
        jmp    NEAR PTR loc_7_12E8
loc_7_1568:
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        mov    dx, word ptr [ebx]
        add    ebx, 4
        or     dx, dx
        je     SHORT loc_7_159E
        mov    ecx, dword ptr [ebp - 18h]
        mov    word ptr [ebp - 8], dx
        mov    dword ptr [ecx + 2ah], eax
        mov    word ptr [ecx + 2eh], dx
        mov    word ptr [ecx + 22h], 2
        jmp    SHORT loc_7_159E
        db  08Bh, 0FFh
loc_7_1590:
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 98h], eax
loc_7_159E:
        inc    dword ptr [ebp - 24h]
loc_7_15A1:
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 18h]
        sub    ax, 0a8h
        mov    word ptr [ecx + 28h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        cmp    al, 53h
        jne    SHORT loc_7_15DC
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, ecx
        sub    ax, 0cfh
        add    word ptr [ebp - 8], ax
        mov    ah, byte ptr [ebp - 7]
        mov    byte ptr [ecx + 0cdh], ah
        mov    al, byte ptr [ebp - 8]
        mov    byte ptr [ecx + 0ceh], al
        jmp    SHORT loc_7_1612
loc_7_15DC:
        cmp    al, 58h
        jne    SHORT loc_7_1612
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, dword ptr [ebp - 18h]
        sub    ax, 0cfh
        add    word ptr [ebp - 8], ax
        mov    ecx, dword ptr [ebp - 18h]
        mov    ah, byte ptr [ebp - 7]
        mov    byte ptr [ecx + 0cdh], ah
        mov    al, byte ptr [ebp - 8]
        mov    byte ptr [ecx + 0ceh], al
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        dec    eax
        mov    dword ptr [ecx + 0d0h], eax
loc_7_1612:
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, ecx
        mov    word ptr [ecx + 22h], 1
        add    eax, 0d0h
        mov    dword ptr [ecx + 24h], eax
        mov    esi, ebx
        mov    edx, dword ptr [ebp - 24h]
        mov    dword ptr [ecx + 9ch], 0
        mov    eax, edx
        mov    dword ptr [ecx + 0a4h], 0
        cmp    byte ptr [eax], 2ch
        jne    NEAR PTR loc_7_1790
        inc    edx
        cmp    word ptr [ebp - 2], 163h
        jae    NEAR PTR loc_7_1796
loc_7_1656:
        inc    dword ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 24h]
        movsx  eax, byte ptr [eax]
        cmp    eax, 63h
        jg     SHORT loc_7_16B2
        je     NEAR PTR loc_7_1730
        cmp    eax, 40h
        jg     SHORT loc_7_1695
        je     SHORT loc_7_16E1
        or     eax, eax
        je     NEAR PTR loc_7_1790
        cmp    eax, 2dh
        je     NEAR PTR loc_7_1723
        cmp    eax, 3dh
        je     NEAR PTR loc_7_1713
        cmp    eax, 3fh
        je     SHORT loc_7_16C5
        jmp    NEAR PTR loc_7_1752
loc_7_1695:
        cmp    eax, 44h
        je     SHORT loc_7_1700
        cmp    eax, 57h
        je     SHORT loc_7_1710
        cmp    eax, 5fh
        je     NEAR PTR loc_7_1743
        cmp    eax, 62h
        je     SHORT loc_7_1720
        jmp    NEAR PTR loc_7_1752
loc_7_16B2:
        sub    eax, 64h
        cmp    eax, 16h
        ja     NEAR PTR loc_7_1752
        db  0FFh, 024h, 085h, 060h, 019h, 000h, 000h
loc_7_16C5:
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        mov    ecx, dword ptr [ebp - 14h]
        add    ebx, 4
        mov    dword ptr [ecx + 0a4h], eax
        mov    eax, dword ptr [ebx - 4]
        mov    dword ptr [ecx + 0a8h], eax
        jmp    SHORT loc_7_1752
loc_7_16E1:
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        mov    ecx, dword ptr [ebp - 14h]
        add    ebx, 4
        mov    dword ptr [ecx + 9ch], eax
        mov    eax, dword ptr [ebx - 4]
        mov    dword ptr [ecx + 0a0h], eax
        jmp    SHORT loc_7_1752
        db  08Dh, 049h, 000h
loc_7_1700:
        add    ebx, 4
        add    word ptr [ebp - 2], 4
        jmp    SHORT loc_7_1752
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_7_1710:
        add    ebx, 4
loc_7_1713:
        add    word ptr [ebp - 2], 2
        jmp    SHORT loc_7_1752
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_7_1720:
        add    ebx, 4
loc_7_1723:
        inc    word ptr [ebp - 2]
        jmp    SHORT loc_7_1752
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_7_1730:
        add    ebx, 4
        mov    word ptr [ebp - 2], 163h
        jmp    SHORT loc_7_1752
        db  005h, 000h, 000h, 000h, 000h
loc_7_1740:
        add    ebx, 4
loc_7_1743:
        add    ebx, 4
        mov    ax, word ptr [ebp - 2]
        add    ax, word ptr [ebx - 4]
        mov    word ptr [ebp - 2], ax
loc_7_1752:
        cmp    word ptr [ebp - 2], 163h
        jb     NEAR PTR loc_7_1656
        jmp    SHORT loc_7_1790
loc_7_1760:
        mov    edi, dword ptr [ebx]
        add    ebx, 4
        mov    ax, word ptr [ebx]
        add    ebx, 4
        or     ax, ax
        je     SHORT loc_7_1790
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 2ah], edi
        mov    word ptr [ecx + 2eh], ax
        mov    word ptr [ecx + 22h], 2
        jmp    SHORT loc_7_1790
loc_7_1782:
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 0c0h], eax
loc_7_1790:
        cmp    word ptr [ebp - 2], 163h
loc_7_1796:
        jbe    SHORT loc_7_179E
        mov    word ptr [ebp - 2], 163h
loc_7_179E:
        mov    ax, word ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 14h]
        add    ax, 1eh
        cmp    dword ptr [ecx + 9ch], 0
        mov    word ptr [ecx + 28h], ax
        mov    dword ptr [ecx + 0bch], edx
        jne    SHORT loc_7_17C4
        mov    dword ptr [ecx + 0b8h], esi
        jmp    SHORT loc_7_1809
loc_7_17C4:
        mov    eax, dword ptr [ebp - 24h]
        sub    ebx, esi
        cmp    byte ptr [eax], 66h
        jne    SHORT loc_7_17D2
        sub    bx, 8
loc_7_17D2:
        and    ebx, 0ffffh
        push   ebx
        call   sub_7_0AD8
        add    esp, 4
        mov    ecx, dword ptr [ebp - 14h]
        mov    edi, eax
        mov    dword ptr [ecx + 0b8h], eax
        or     edi, edi
        jne    SHORT loc_7_17FB
        mov    al, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_7_17FB:
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
loc_7_1809:
        mov    ecx, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 28h]
        db  0C7h, 081h, 098h, 000h, 000h, 000h, 0E0h, 019h, 000h, 000h
        test   byte ptr [edx + 58h], 40h
        je     SHORT loc_7_1853
        mov    byte ptr [ecx + 96h], 3fh
        cmp    dword ptr [ecx + 9ch], 0
        je     SHORT loc_7_1899
        mov    dword ptr [ebp - 40h], edx
        db  0C7h, 045h, 0C4h, 091h, 004h, 000h, 000h
        mov    edx, dword ptr [ebp - 40h]
        mov    esi, dword ptr [ebp - 3ch]
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    dword ptr [ebp - 44h], esi
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_7_1853:
        mov    edx, dword ptr [ebp - 28h]
        mov    al, byte ptr [edx + 5ch]
        or     al, al
        je     SHORT loc_7_1865
        cmp    al, 1
        jne    NEAR PTR loc_7_12F6
loc_7_1865:
        mov    ecx, dword ptr [ebp - 28h]
        call   sub_7_1E80
        mov    ecx, dword ptr [ebp - 14h]
        cmp    dword ptr [ecx + 9ch], 0
        je     SHORT loc_7_1884
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_7_1884:
        mov    ecx, dword ptr [ebp - 14h]
        cmp    byte ptr [ecx + 94h], 0
        je     SHORT loc_7_1899
        push   ecx
        call   sub_7_0AE4
        add    esp, 4
loc_7_1899:
        mov    ecx, dword ptr [ebp - 14h]
        pop    edi
        mov    al, byte ptr [ecx + 96h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h, 0E0h, 00Eh, 000h, 000h, 04Ah, 00Fh, 000h, 000h, 0A3h, 00Fh, 000h, 000h, 067h, 010h, 000h
        db  000h, 090h, 010h, 000h, 000h, 071h, 010h, 000h, 000h, 0D2h, 00Eh, 000h, 000h, 0D2h, 00Eh, 000h
        db  000h, 0D2h, 00Eh, 000h, 000h, 0D2h, 00Eh, 000h, 000h, 0D2h, 00Eh, 000h, 000h, 071h, 010h, 000h
        db  000h, 0D2h, 00Eh, 000h, 000h, 0D2h, 00Eh, 000h, 000h, 0D2h, 00Eh, 000h, 000h, 0D2h, 00Eh, 000h
        db  000h, 0D2h, 00Eh, 000h, 000h, 0D2h, 00Eh, 000h, 000h, 071h, 010h, 000h, 000h, 0D2h, 00Eh, 000h
        db  000h, 0D2h, 00Eh, 000h, 000h, 0D2h, 00Eh, 000h, 000h, 0D2h, 00Eh, 000h, 000h, 0C1h, 010h, 000h
        db  000h, 0E0h, 013h, 000h, 000h, 017h, 014h, 000h, 000h, 040h, 014h, 000h, 000h, 068h, 015h, 000h
        db  000h, 090h, 015h, 000h, 000h, 0C7h, 014h, 000h, 000h, 040h, 014h, 000h, 000h, 0E8h, 012h, 000h
        db  000h, 0E8h, 012h, 000h, 000h, 0E8h, 012h, 000h, 000h, 0E8h, 012h, 000h, 000h, 0E8h, 012h, 000h
        db  000h, 0E8h, 012h, 000h, 000h, 0D9h, 014h, 000h, 000h, 0E8h, 012h, 000h, 000h, 011h, 015h, 000h
        db  000h, 0E8h, 012h, 000h, 000h, 0E8h, 012h, 000h, 000h, 0E8h, 012h, 000h, 000h, 0E8h, 012h, 000h
        db  000h, 040h, 015h, 000h, 000h, 000h, 017h, 000h, 000h, 052h, 017h, 000h, 000h, 060h, 017h, 000h
        db  000h, 082h, 017h, 000h, 000h, 020h, 017h, 000h, 000h, 052h, 017h, 000h, 000h, 052h, 017h, 000h
        db  000h, 052h, 017h, 000h, 000h, 052h, 017h, 000h, 000h, 052h, 017h, 000h, 000h, 052h, 017h, 000h
        db  000h, 052h, 017h, 000h, 000h, 030h, 017h, 000h, 000h, 030h, 017h, 000h, 000h, 040h, 017h, 000h
        db  000h, 052h, 017h, 000h, 000h, 052h, 017h, 000h, 000h, 030h, 017h, 000h, 000h, 052h, 017h, 000h
        db  000h, 010h, 017h, 000h, 000h, 052h, 017h, 000h, 000h, 052h, 017h, 000h, 000h, 030h, 017h, 000h
        db  000h, 08Dh, 064h, 024h, 000h
sub_7_19C0:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 44h]
        mov    ecx, dword ptr [eax + 0a0h]
        jmp    dword ptr [eax + 9ch]
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_7_19E0:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 34h]
        mov    edx, dword ptr [ebx + 90h]
        mov    esi, dword ptr [edx + 40h]
        mov    cl, byte ptr [ebx + 96h]
        lea    eax, [ebx + 0eeh]
        mov    dword ptr [esp + 1ch], eax
        mov    ax, word ptr [ebx + 0d2h]
        mov    word ptr [esp + 1ah], ax
        mov    ax, word ptr [esp + 1ah]
        xchg   al, ah
        mov    word ptr [esp + 18h], ax
        mov    edi, dword ptr [esp + 18h]
        and    edi, 0ffffh
        mov    eax, dword ptr [esp + 1ch]
        lea    edi, [edi + eax - 1eh]
        mov    dword ptr [esp + 2ch], edi
        mov    eax, dword ptr [ebx + 0bch]
        mov    dword ptr [esp + 20h], eax
        mov    ebp, dword ptr [ebx + 0b8h]
        mov    byte ptr [esp + 13h], 0
        xor    eax, eax
        mov    al, byte ptr [ebx + 95h]
        dec    eax
        cmp    eax, 5
        ja     SHORT loc_7_1ACE
        db  0FFh, 024h, 085h, 004h, 01Eh, 000h, 000h
        db  08Dh, 064h, 024h, 000h
loc_7_1A60:
        or     cl, cl
        jne    NEAR PTR loc_7_1DC0
        mov    eax, dword ptr [esp + 1ch]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [edx + 72h], al
        mov    eax, dword ptr [esp + 1ch]
        mov    al, byte ptr [eax + 3]
        mov    byte ptr [edx + 73h], al
        mov    eax, dword ptr [esp + 1ch]
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [edx + 74h], al
        jmp    NEAR PTR loc_7_1DC0
        db  005h, 000h, 000h, 000h, 000h
loc_7_1A90:
        or     cl, cl
        jne    NEAR PTR loc_7_1DC0
        add    dword ptr [esp + 1ch], 24h
        jmp    SHORT loc_7_1ACE
        db  090h
loc_7_1AA0:
        mov    ax, word ptr [esi + 96h]
        mov    word ptr [esi + 0b8h], ax
        jmp    NEAR PTR loc_7_1DC0
loc_7_1AB3:
        mov    ax, word ptr [esi + 96h]
        mov    word ptr [esi + 0b8h], ax
loc_7_1AC1:
        or     cl, cl
        jne    NEAR PTR loc_7_1DC0
        add    dword ptr [esp + 1ch], 8
loc_7_1ACE:
        mov    eax, dword ptr [esp + 20h]
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_7_1DC0
        add    eax, 0
loc_7_1AE0:
        mov    eax, dword ptr [esp + 20h]
        mov    al, byte ptr [eax]
        cmp    al, 66h
        je     NEAR PTR loc_7_1DC0
        cmp    al, 67h
        je     NEAR PTR loc_7_1DC0
        mov    edi, dword ptr [esp + 2ch]
        cmp    edi, dword ptr [esp + 1ch]
        ja     SHORT loc_7_1B0B
        cmp    byte ptr [esp + 13h], 1
        je     NEAR PTR loc_7_1DC0
loc_7_1B0B:
        movsx  eax, al
        cmp    eax, 63h
        jg     SHORT loc_7_1B66
        je     NEAR PTR loc_7_1C17
        cmp    eax, 57h
        jg     SHORT loc_7_1B46
        je     NEAR PTR loc_7_1BC0
        cmp    eax, 2dh
        je     SHORT loc_7_1B80
        cmp    eax, 3dh
        je     SHORT loc_7_1B90
        cmp    eax, 3fh
        jl     NEAR PTR loc_7_1D9C
        cmp    eax, 40h
        jle    SHORT loc_7_1BA0
        cmp    eax, 44h
        je     SHORT loc_7_1BA8
        jmp    NEAR PTR loc_7_1D9C
loc_7_1B46:
        cmp    eax, 5eh
        je     NEAR PTR loc_7_1BE0
        cmp    eax, 5fh
        je     NEAR PTR loc_7_1BEA
        cmp    eax, 62h
        je     NEAR PTR loc_7_1C00
        jmp    NEAR PTR loc_7_1D9C
loc_7_1B66:
        sub    eax, 64h
        cmp    eax, 16h
        ja     NEAR PTR loc_7_1D9C
        db  0FFh, 024h, 085h, 01Ch, 01Eh, 000h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_7_1B80:
        inc    dword ptr [esp + 1ch]
        jmp    NEAR PTR loc_7_1D9C
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_7_1B90:
        add    dword ptr [esp + 1ch], 2
        jmp    NEAR PTR loc_7_1D9C
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_7_1BA0:
        add    ebp, 8
        jmp    NEAR PTR loc_7_1D9C
loc_7_1BA8:
        mov    eax, dword ptr [esp + 1ch]
        add    ebp, 4
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        add    dword ptr [esp + 1ch], 4
        jmp    NEAR PTR loc_7_1D9C
loc_7_1BC0:
        mov    eax, dword ptr [esp + 1ch]
        add    ebp, 4
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], ax
        add    dword ptr [esp + 1ch], 2
        jmp    NEAR PTR loc_7_1D9C
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_7_1BE0:
        mov    byte ptr [esp + 13h], 1
        jmp    NEAR PTR loc_7_1D9C
loc_7_1BEA:
        xor    eax, eax
        mov    ax, word ptr [ebp]
        add    ebp, 4
        add    dword ptr [esp + 1ch], eax
        jmp    NEAR PTR loc_7_1D9C
        db  08Dh, 064h, 024h, 000h
loc_7_1C00:
        mov    eax, dword ptr [esp + 1ch]
        add    ebp, 4
        mov    cl, byte ptr [eax]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], cl
        inc    dword ptr [esp + 1ch]
        jmp    NEAR PTR loc_7_1D9C
loc_7_1C17:
        add    ebp, 4
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    eax, ecx
        and    eax, 0ffffh
        mov    edi, dword ptr [ebp - 4]
        lea    edx, [eax + 1]
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        inc    eax
        add    dword ptr [esp + 1ch], eax
        jmp    NEAR PTR loc_7_1D9C
loc_7_1C53:
        add    ebp, 4
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esp + 1ch]
        add    dword ptr [esp + 1ch], 4
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 28h], eax
        mov    eax, dword ptr [esp + 28h]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [esp + 24h], eax
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_7_1D9C
loc_7_1C82:
        mov    eax, dword ptr [esp + 1ch]
        add    ebp, 4
        mov    cl, byte ptr [eax]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1eh], cl
        inc    dword ptr [esp + 1ch]
        jmp    NEAR PTR loc_7_1D9C
loc_7_1C9A:
        mov    edx, dword ptr [ebp]
        add    ebp, 4
        mov    eax, dword ptr [esp + 1ch]
        mov    edi, edx
        mov    al, byte ptr [eax]
        inc    dword ptr [esp + 1ch]
        and    eax, 0ffh
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    byte ptr [eax + edx], 0
        add    dword ptr [esp + 1ch], eax
        jmp    NEAR PTR loc_7_1D9C
loc_7_1CD0:
        add    ebp, 4
        mov    eax, ebp
        add    ebp, 4
        xor    edx, edx
        mov    dx, word ptr [ebp - 4]
        mov    edi, dword ptr [eax - 4]
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        add    dword ptr [esp + 1ch], edx
        jmp    NEAR PTR loc_7_1D9C
        db  08Dh, 064h, 024h, 000h
loc_7_1D00:
        mov    esi, dword ptr [ebp]
        add    ebp, 4
        mov    eax, dword ptr [esp + 1ch]
        push   1
        mov    al, byte ptr [eax]
        inc    dword ptr [esp + 20h]
        mov    edi, eax
        and    edi, 0ffh
        mov    eax, dword ptr [esp + 20h]
        push   edi
        push   eax
        push   esi
        call   sub_7_0AC0
        add    esp, 10h
        and    eax, 0fffffffeh
        mov    word ptr [esi + eax], 0
        add    dword ptr [esp + 1ch], edi
        jmp    SHORT loc_7_1D9C
loc_7_1D38:
        add    ebp, 4
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esp + 1ch]
        add    dword ptr [esp + 1ch], 2
        mov    ax, word ptr [eax]
        mov    word ptr [esp + 16h], ax
        mov    ax, word ptr [esp + 16h]
        xchg   al, ah
        mov    word ptr [esp + 14h], ax
        mov    ax, word ptr [esp + 14h]
        mov    word ptr [ecx], ax
        jmp    SHORT loc_7_1D9C
loc_7_1D65:
        add    ebp, 4
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    esi, ecx
        and    esi, 0ffffh
        push   1
        lea    eax, [esi + 1]
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_7_0AC0
        add    esp, 10h
        inc    esi
        add    dword ptr [esp + 1ch], esi
loc_7_1D9C:
        lea    eax, [ebx + 251h]
        cmp    eax, dword ptr [esp + 1ch]
        jae    SHORT loc_7_1DAF
        mov    byte ptr [ebx + 96h], 0ffh
loc_7_1DAF:
        inc    dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 20h]
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_7_1AE0
loc_7_1DC0:
        cmp    dword ptr [ebx + 9ch], 0
        je     SHORT loc_7_1DF0
        mov    eax, dword ptr [ebx + 0b8h]
        push   eax
        call   sub_7_0ADE
        add    esp, 4
        mov    ecx, dword ptr [ebx + 0a0h]
        call   dword ptr [ebx + 9ch]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 064h, 024h, 000h
loc_7_1DF0:
        push   ebx
        call   sub_7_0AEA
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h, 060h, 01Ah, 000h, 000h, 0C0h, 01Dh, 000h, 000h, 0C1h, 01Ah, 000h, 000h, 090h
        db  01Ah, 000h, 000h, 0A0h, 01Ah, 000h, 000h, 0B3h, 01Ah, 000h, 000h, 053h, 01Ch, 000h, 000h, 09Ch
        db  01Dh, 000h, 000h, 09Ch, 01Dh, 000h, 000h, 09Ch, 01Dh, 000h, 000h, 082h, 01Ch, 000h, 000h, 09Ch
        db  01Dh, 000h, 000h, 09Ch, 01Dh, 000h, 000h, 09Ch, 01Dh, 000h, 000h, 09Ch, 01Dh, 000h, 000h, 09Ch
        db  01Dh, 000h, 000h, 09Ch, 01Dh, 000h, 000h, 09Ch, 01Dh, 000h, 000h, 09Ah, 01Ch, 000h, 000h, 09Ch
        db  01Dh, 000h, 000h, 0D0h, 01Ch, 000h, 000h, 09Ch, 01Dh, 000h, 000h, 09Ch, 01Dh, 000h, 000h, 000h
        db  01Dh, 000h, 000h, 09Ch, 01Dh, 000h, 000h, 038h, 01Dh, 000h, 000h, 09Ch, 01Dh, 000h, 000h, 09Ch
        db  01Dh, 000h, 000h, 065h, 01Dh, 000h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_7_1E80:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, ecx
        mov    dword ptr [esp + 1ch], ecx
        mov    edx, dword ptr [edx + 40h]
        mov    dword ptr [esp + 18h], edx
        mov    edx, ecx
        mov    esi, dword ptr [esp + 18h]
        mov    ebx, dword ptr [edx + 44h]
        mov    edx, ecx
        add    esi, 0c6h
        mov    ebp, dword ptr [edx + 48h]
        mov    edx, ecx
        mov    cl, 22h
        mov    di, word ptr [edx + 60h]
        xor    eax, eax
        mov    al, byte ptr [ebx + 95h]
        cmp    eax, 1
        je     SHORT loc_7_1ED7
        cmp    eax, 2
        je     SHORT loc_7_1EF0
        cmp    eax, 3
        je     SHORT loc_7_1EF2
        cmp    eax, 4
        je     NEAR PTR loc_7_20F0
        cmp    eax, 6
        je     SHORT loc_7_1EF2
        jmp    SHORT loc_7_1F1A
loc_7_1ED7:
        mov    byte ptr [esi], 11h
        inc    esi
        mov    byte ptr [esi], 11h
        inc    esi
        mov    byte ptr [esi], 0
        inc    esi
        mov    byte ptr [esi], 0ffh
        inc    esi
        mov    byte ptr [esi], 0
        mov    byte ptr [esi + 1], 0ffh
        jmp    SHORT loc_7_1F1A
loc_7_1EF0:
        mov    cl, 55h
loc_7_1EF2:
        mov    edx, dword ptr [esp + 1ch]
        db  0FFh, 005h, 0A0h, 020h, 000h, 000h
        mov    al, byte ptr [edx + 72h]
        inc    al
        mov    byte ptr [edx + 72h], al
        mov    byte ptr [esi], cl
        mov    byte ptr [esi + 1], cl
        inc    esi
        mov    byte ptr [esi + 1], al
        inc    esi
        inc    esi
        mov    al, byte ptr [edx + 73h]
        mov    byte ptr [esi], al
        mov    al, byte ptr [edx + 74h]
        mov    byte ptr [esi + 2], al
loc_7_1F1A:
        mov    edx, dword ptr [esp + 18h]
        mov    word ptr [ebx + 0ah], di
        mov    word ptr [edx + 0ah], di
        mov    byte ptr [ebx + 96h], 0
        mov    byte ptr [ebx + 94h], 0ffh
        mov    byte ptr [ebx + 0b6h], 0
        cmp    byte ptr [ebx + 95h], 4
        mov    word ptr [ebx + 0b4h], 0
        mov    dword ptr [ebx + 0c8h], 0
        mov    byte ptr [ebx + 97h], 0
        jne    NEAR PTR loc_7_21E0
        db  0C7h, 042h, 004h, 0E0h, 031h, 000h, 000h
        mov    cx, word ptr [edx + 22h]
        mov    word ptr [ebp + 58h], cx
        mov    edx, ecx
        and    edx, 0ffffh
        lea    eax, [ebp + 0beh]
        lea    edx, [edx + edx*2]
        add    edx, edx
        mov    esi, dword ptr [esp + 18h]
        add    esi, 24h
        mov    edi, eax
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [esp + 18h]
        lea    edi, [ebp + 0c2h]
        mov    word ptr [edx + 28h], 42h
        add    dword ptr [eax], 42h
        sub    word ptr [edi], 42h
        lea    ecx, [ebx + 28h]
        add    dword ptr [ebx + 24h], 42h
        sub    esi, esi
        sub    word ptr [ecx], 42h
        mov    ax, word ptr [edx + 22h]
        cmp    ax, si
        je     SHORT loc_7_1FDD
        and    eax, 0ffffh
        lea    ecx, [ecx]
loc_7_1FD0:
        xor    edx, edx
        mov    dx, word ptr [edi]
        add    edi, 6
        add    esi, edx
        dec    eax
        jne    SHORT loc_7_1FD0
loc_7_1FDD:
        mov    edx, dword ptr [esp + 18h]
        mov    eax, dword ptr [edx + 98h]
        or     eax, eax
        je     SHORT loc_7_1FF4
loc_7_1FEB:
        add    esi, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_7_1FEB
loc_7_1FF4:
        xor    eax, eax
        mov    ax, word ptr [ebp + 46h]
        cmp    eax, esi
        jae    SHORT loc_7_2035
        mov    edx, dword ptr [esp + 1ch]
        xor    edi, edi
        mov    di, word ptr [edx + 6ch]
        mov    dword ptr [esp + 2ch], 3e8h
        mov    eax, dword ptr [ebp + 0ch]
        sub    edx, edx
        div    dword ptr [esp + 2ch]
        add    edi, eax
        cmp    edi, 1f4h
        ja     SHORT loc_7_2028
        mov    edi, 1f4h
loc_7_2028:
        mov    dword ptr [ebx + 0c8h], edi
        mov    byte ptr [ebx + 97h], 1
loc_7_2035:
        mov    word ptr [ebp + 5ah], 1
        mov    dword ptr [ebp + 5eh], 0
        mov    dword ptr [ebp + 62h], esi
        mov    dword ptr [esp + 28h], esi
        mov    eax, dword ptr [esp + 28h]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [esp + 24h], eax
        mov    edx, dword ptr [esp + 24h]
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax + 18h], edx
        sub    edx, edx
        mov    ax, word ptr [ebx + 22h]
        cmp    ax, dx
        je     SHORT loc_7_2080
        and    eax, 0ffffh
loc_7_2073:
        xor    esi, esi
        mov    si, word ptr [ecx]
        add    ecx, 6
        add    edx, esi
        dec    eax
        jne    SHORT loc_7_2073
loc_7_2080:
        mov    eax, dword ptr [ebx + 0c0h]
        or     eax, eax
        je     SHORT loc_7_2093
loc_7_208A:
        add    edx, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_7_208A
loc_7_2093:
        xor    eax, eax
        mov    ax, word ptr [ebp + 46h]
        cmp    eax, edx
        jae    SHORT loc_7_20A4
        or     byte ptr [ebx + 97h], 2
loc_7_20A4:
        mov    word ptr [ebp + 5ch], 1
        mov    dword ptr [ebp + 8eh], 0
        mov    dword ptr [ebp + 92h], edx
        mov    dword ptr [ebp + 1ch], 0
        mov    ecx, dword ptr [esp + 1ch]
        call   sub_7_2D80
        call   sub_7_09F3
        mov    edx, dword ptr [esp + 18h]
        xor    ecx, ecx
        mov    dword ptr [edx + 0a0h], eax
        mov    cx, word ptr [ebp + 46h]
        add    ecx, 84h
        jmp    NEAR PTR loc_7_221E
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_7_20F0:
        mov    ax, word ptr [ebp + 48h]
        mov    word ptr [ebp + 4eh], ax
        mov    ax, word ptr [ebp + 4ah]
        mov    word ptr [ebp + 50h], ax
        mov    ax, word ptr [ebp + 4ch]
        mov    word ptr [ebp + 52h], ax
        mov    dword ptr [esp + 20h], esi
        mov    dword ptr [ebp + 38h], esi
        mov    word ptr [esi], 7777h
        mov    byte ptr [esi + 3], 2
        mov    eax, dword ptr [ebp]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [esi + 8], eax
        mov    ecx, 64h
        mov    eax, dword ptr [ebp + 10h]
        sub    edx, edx
        div    ecx
        mov    dword ptr [esp + 34h], eax
        mov    eax, dword ptr [esp + 34h]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [esp + 30h], eax
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [esi + 10h], eax
        mov    ax, word ptr [ebp + 48h]
        mov    word ptr [esp + 16h], ax
        mov    ax, word ptr [esp + 16h]
        xchg   al, ah
        mov    word ptr [esp + 14h], ax
        mov    ax, word ptr [esp + 14h]
        mov    word ptr [esi + 14h], ax
        mov    ax, word ptr [ebp + 4ah]
        mov    word ptr [esp + 12h], ax
        mov    ax, word ptr [esp + 12h]
        xchg   al, ah
        mov    word ptr [esp + 10h], ax
        mov    ax, word ptr [esp + 10h]
        mov    word ptr [esi + 16h], ax
        mov    ax, word ptr [ebp + 4ah]
        mov    word ptr [ebp + 4ch], ax
        mov    word ptr [ebp + 56h], 0
        mov    byte ptr [ebp + 44h], 0
        mov    byte ptr [ebp + 45h], 0
        mov    edx, dword ptr [esp + 1ch]
        cmp    dword ptr [edx + 24h], 0ee6bh
        jb     SHORT loc_7_21B8
        call   sub_7_09F3
        mov    dword ptr [ebp + 20h], eax
        mov    dword ptr [ebp + 34h], 0
loc_7_21B8:
        mov    di, word ptr [ebp + 54h]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 3e8h
        jbe    NEAR PTR loc_7_1F1A
        cmp    eax, 0c350h
        jae    NEAR PTR loc_7_1F1A
        call   sub_7_034F
        jmp    NEAR PTR loc_7_1F1A
        db  090h
loc_7_21E0:
        mov    edx, dword ptr [esp + 18h]
        xor    ecx, ecx
        db  0C7h, 042h, 004h, 0D0h, 022h, 000h, 000h
        db  0C7h, 043h, 004h, 0E0h, 026h, 000h, 000h
        xor    eax, eax
        mov    ax, word ptr [edx + 28h]
        mov    cx, word ptr [ebx + 28h]
        add    ecx, eax
        cmp    word ptr [edx + 22h], 1
        jbe    SHORT loc_7_220F
        xor    eax, eax
        mov    ax, word ptr [edx + 2eh]
        add    ecx, eax
loc_7_220F:
        cmp    word ptr [ebx + 22h], 1
        jbe    SHORT loc_7_221E
        xor    eax, eax
        mov    ax, word ptr [ebx + 2eh]
        add    ecx, eax
loc_7_221E:
        mov    edx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [edx + 24h]
        imul   eax, ecx
        mov    ecx, 0f4240h
        sub    edx, edx
        div    ecx
        mov    edx, dword ptr [esp + 1ch]
        mov    word ptr [edx + 70h], ax
        or     byte ptr [ebx + 97h], 4
        cli
        lea    eax, [ebx + 0ach]
        db  0C7h, 000h, 0C4h, 005h, 000h, 000h
        db  08Bh, 00Dh, 0C8h, 005h, 000h, 000h
        mov    dword ptr [ebx + 0b0h], ecx
        mov    dword ptr [ecx], eax
        db  0A3h, 0C8h, 005h, 000h, 000h
        sti
        call   sub_7_02F1
        mov    edx, dword ptr [esp + 18h]
        push   edx
        call   sub_7_0C2C
        add    esp, 4
        mov    edx, dword ptr [esp + 18h]
        cmp    byte ptr [edx + 8], 0
        jne    SHORT loc_7_22C7
        mov    al, byte ptr [edx + 9]
        or     al, al
        je     SHORT loc_7_22C7
        cmp    al, 0ffh
        je     SHORT loc_7_22C7
        cmp    byte ptr [ebx + 94h], 0
        je     SHORT loc_7_22C7
        mov    al, byte ptr [ebx + 0b7h]
        mov    byte ptr [ebx + 0b6h], al
        cmp    byte ptr [ebx + 95h], 4
        jne    SHORT loc_7_22B2
        mov    word ptr [ebp + 5ah], 0
        mov    word ptr [ebp + 5ch], 0
loc_7_22B2:
        mov    edx, dword ptr [esp + 1ch]
        mov    word ptr [edx + 70h], 0
        mov    ax, word ptr [edx + 6ch]
        mov    word ptr [ebx + 0b4h], ax
loc_7_22C7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  090h
loc_7_22D0:
        mov    ecx, dword ptr [esp + 4]
        cmp    dword ptr [ecx + 0a4h], 444e6573h
        jne    SHORT loc_7_22F6
        mov    dword ptr [ecx + 0a4h], 646e6573h
        push   0
        push   ecx
        call   sub_7_09D1
        add    esp, 8
        ret
loc_7_22F6:
        mov    eax, dword ptr [ecx + 90h]
        mov    eax, dword ptr [eax + 44h]
        and    byte ptr [eax + 97h], 0fbh
        cmp    byte ptr [ecx + 94h], 0
        je     SHORT loc_7_2320
        mov    byte ptr [ecx + 94h], 0
        push   ecx
        call   sub_7_0C2C
        add    esp, 4
        ret
loc_7_2320:
        cmp    dword ptr [ecx + 9ch], 0
        je     SHORT loc_7_2332
        push   ecx
        call   sub_7_0AEA
        add    esp, 4
loc_7_2332:
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_7_2340:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 40h]
        cmp    byte ptr [eax + 34h], 0
        jne    SHORT loc_7_2352
        sub    al, al
        pop    esi
        ret
loc_7_2352:
        cmp    byte ptr [esi + 8], 0
        je     SHORT loc_7_2373
loc_7_2358:
        inc    dword ptr [esi + 9ch]
        push   esi
        call   sub_7_0AE4
        add    esp, 4
        dec    dword ptr [esi + 9ch]
        cmp    byte ptr [esi + 8], 0
        jne    SHORT loc_7_2358
loc_7_2373:
        sub    al, al
        pop    esi
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_7_2380:
        push   esi
        mov    esi, dword ptr [esp + 8]
        or     byte ptr [esi + 97h], 8
        mov    dword ptr [esi + 0c4h], 0
        cmp    byte ptr [esi + 95h], 4
        jne    SHORT loc_7_23B3
        mov    ecx, dword ptr [esi + 90h]
        call   sub_7_3310
        and    byte ptr [esi + 97h], 0f7h
        pop    esi
        ret
loc_7_23B3:
        cmp    byte ptr [esi + 8], 0
        jne    SHORT loc_7_23CC
        db  0FFh, 005h, 058h, 010h, 000h, 000h
        mov    byte ptr [esi + 9], 0fch
        push   esi
        call   sub_7_26E0
        add    esp, 4
loc_7_23CC:
        and    byte ptr [esi + 97h], 0f7h
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_7_23E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  08Bh, 03Dh, 0C8h, 005h, 000h, 000h
        db  081h, 0FFh, 0C4h, 005h, 000h, 000h
        je     NEAR PTR loc_7_248E
        mov    cx, word ptr [ebp + 8]
        nop
loc_7_2400:
        lea    ebx, [edi - 0ach]
        mov    edi, dword ptr [edi + 4]
        test   byte ptr [ebx + 97h], 4
        jne    SHORT loc_7_2482
        mov    dx, word ptr [ebx + 0b4h]
        add    dx, cx
        mov    word ptr [ebx + 0b4h], dx
        mov    eax, dword ptr [ebx + 0c8h]
        or     eax, eax
        jne    SHORT loc_7_2444
        mov    eax, dword ptr [ebx + 90h]
        xor    esi, esi
        mov    si, word ptr [eax + 6ch]
        mov    ax, word ptr [eax + 70h]
        and    eax, 0ffffh
        add    eax, esi
loc_7_2444:
        and    edx, 0ffffh
        cmp    edx, eax
        jbe    SHORT loc_7_2482
        cmp    dword ptr [ebx + 0c4h], 0
        jne    SHORT loc_7_2482
        test   byte ptr [ebx + 97h], 8
        jne    SHORT loc_7_2482
        mov    dword ptr [ebp - 0ch], ebx
        db  0C7h, 045h, 0F8h, 0B9h, 004h, 000h, 000h
        mov    edx, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ebp - 8]
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    dword ptr [ebp - 4], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0c4h], eax
loc_7_2482:
        db  081h, 0FFh, 0C4h, 005h, 000h, 000h
        jne    NEAR PTR loc_7_2400
loc_7_248E:
        db  0A1h, 0C8h, 005h, 000h, 000h
        db  02Dh, 0C4h, 005h, 000h, 000h
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_7_24B0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        db  08Bh, 03Dh, 0C8h, 005h, 000h, 000h
        lea    ebp, [ebx + 12h]
        mov    ax, word ptr [ebx + 10h]
        mov    word ptr [esp + 12h], ax
loc_7_24D0:
        db  081h, 0FFh, 0C4h, 005h, 000h, 000h
        je     SHORT loc_7_2552
        mov    eax, dword ptr [edi - 1ch]
        lea    esi, [edi - 0ach]
        mov    eax, dword ptr [eax + 40h]
        mov    edi, dword ptr [edi + 4]
        mov    dx, word ptr [eax + 0c4h]
        add    eax, 0aeh
        cmp    byte ptr [esi + 95h], 4
        je     SHORT loc_7_24D0
        cmp    byte ptr [esi + 8], 0
        jne    SHORT loc_7_24D0
        mov    ecx, dword ptr [eax]
        cmp    dword ptr [ebp], ecx
        jne    SHORT loc_7_2520
        mov    ecx, dword ptr [ebp + 4]
        cmp    dword ptr [eax + 4], ecx
        jne    SHORT loc_7_2520
        mov    ecx, dword ptr [eax + 8]
        cmp    dword ptr [ebp + 8], ecx
        jne    SHORT loc_7_2520
        cmp    dx, word ptr [esp + 12h]
        je     SHORT loc_7_252B
loc_7_2520:
        cmp    dword ptr [eax], 0
        jne    SHORT loc_7_24D0
        cmp    dword ptr [eax + 4], -1
        jne    SHORT loc_7_24D0
loc_7_252B:
        mov    eax, dword ptr [esi + 0a4h]
        or     eax, eax
        je     SHORT loc_7_2544
        lea    edx, [ebx + 1eh]
        mov    ecx, dword ptr [esi + 0a8h]
        call   eax
        or     al, al
        je     SHORT loc_7_24D0
loc_7_2544:
        mov    byte ptr [esi + 8], 0ffh
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_7_2552:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h
sub_7_2560:
        push   esi
        push   edi
        mov    esi, dword ptr [ecx]
        mov    edi, edx
        mov    eax, esi
        cdq
        idiv   edi
        add    eax, esi
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_7_2580:
        push   ebx
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        cmp    dword ptr [edi + 24h], 0ee6bh
        jb     SHORT loc_7_25E2
        mov    ebx, dword ptr [edi + 48h]
        call   sub_7_09F3
        sub    eax, dword ptr [ebx + 20h]
        mov    ecx, eax
        shl    ecx, 6
        add    ecx, eax
        sub    edx, edx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [eax + ecx*4]
        lea    ecx, [eax + ecx*4]
        lea    ecx, [ecx + ecx*4]
        shl    ecx, 6
        mov    eax, ecx
        mov    ecx, dword ptr [edi + 24h]
        div    dword ptr [ebx + 34h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, eax
        shr    ecx, 2
        mov    dword ptr [edi + 24h], ecx
        xor    eax, eax
        mov    ax, word ptr [edi + 4ch]
        imul   eax, ecx
        mov    ecx, 5dch
        sub    edx, edx
        div    ecx
        mov    ecx, dword ptr [esi]
        cmp    eax, ecx
        ja     SHORT loc_7_25E0
        mov    eax, ecx
loc_7_25E0:
        mov    dword ptr [esi], eax
loc_7_25E2:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_7_25F0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], edx
        or     ecx, ecx
        mov    esi, dword ptr [esp + 18h]
        je     SHORT loc_7_2610
        lea    ecx, [esi + 10h]
        lea    edi, [esi + 28h]
        jmp    SHORT loc_7_2616
        db  005h, 000h, 000h, 000h, 000h
loc_7_2610:
        lea    ecx, [esi + 0ch]
        lea    edi, [esi + 24h]
loc_7_2616:
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_7_2634
        xor    edx, edx
        mov    dx, word ptr [esi + 2ch]
        neg    edx
        call   sub_7_2560
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
loc_7_2634:
        mov    bx, word ptr [esi + 2eh]
        mov    eax, dword ptr [edi]
        mov    ebp, ebx
        and    ebp, 0ffffh
        cdq
        idiv   ebp
        lea    ebp, [eax + 12ch]
        mov    eax, dword ptr [ecx]
        sub    eax, dword ptr [edi]
        cdq
        xor    eax, edx
        sub    eax, edx
        cmp    ebp, eax
        mov    ax, word ptr [esi + 32h]
        jle    SHORT loc_7_2680
        test   al, 1
        jne    SHORT loc_7_26A5
        or     ax, 1
        mov    word ptr [esi + 32h], ax
        lea    eax, [ebx*2]
        mov    word ptr [esi + 2eh], ax
        mov    ax, word ptr [esi + 2ch]
        imul   ax, ax, 6
        jmp    SHORT loc_7_26A1
        db  08Dh, 049h, 000h
loc_7_2680:
        test   al, 1
        je     SHORT loc_7_26A5
        and    ax, 0fffeh
        mov    word ptr [esi + 32h], ax
        shr    bx, 1
        mov    word ptr [esi + 2eh], bx
        mov    bx, 6
        mov    ax, word ptr [esi + 2ch]
        sub    dx, dx
        div    bx
loc_7_26A1:
        mov    word ptr [esi + 2ch], ax
loc_7_26A5:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edi], eax
        cmp    dword ptr [esp + 10h], 1
        jne    SHORT loc_7_26C5
        xor    edx, edx
        mov    dx, word ptr [esi + 2eh]
        call   sub_7_2560
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
loc_7_26C5:
        xor    edx, edx
        mov    dx, word ptr [esi + 30h]
        call   sub_7_2560
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_7_26E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0eeh
        mov    ebx, dword ptr [edx + 90h]
        mov    edx, dword ptr [ebx + 40h]
        mov    dword ptr [ebp - 8], edx
        mov    edx, dword ptr [ebp + 8]
        mov    al, byte ptr [edx + 9]
        or     al, al
        je     SHORT loc_7_2770
        cmp    al, 0fdh
        je     SHORT loc_7_2770
        db  066h, 0FFh, 005h, 0A8h, 020h, 000h, 000h
        db  0A0h, 05Eh, 004h, 000h, 000h
        cmp    byte ptr [ebx + 54h], al
        jne    SHORT loc_7_275D
        cmp    byte ptr [edx + 95h], 5
        je     SHORT loc_7_275D
        cmp    byte ptr [edx + 0b6h], 1
        lea    edx, [ebx + 6ch]
        jne    SHORT loc_7_2740
        add    word ptr [edx], 6eh
        jmp    SHORT loc_7_2751
        db  005h, 000h, 000h, 000h, 000h
loc_7_2740:
        mov    ax, word ptr [edx]
        mov    cx, ax
        shr    cx, 1
        add    cx, ax
        inc    cx
        mov    word ptr [edx], cx
loc_7_2751:
        mov    ax, word ptr [ebx + 6eh]
        cmp    word ptr [edx], ax
        jbe    SHORT loc_7_275D
        mov    word ptr [edx], ax
loc_7_275D:
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [edx + 94h], 1
        jmp    NEAR PTR loc_7_2AD2
        db  08Dh, 064h, 024h, 000h
loc_7_2770:
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    cl, byte ptr [edx + 95h]
        mov    al, cl
        cmp    eax, 6
        ja     NEAR PTR loc_7_2A39
        db  0FFh, 024h, 085h, 058h, 02Dh, 000h, 000h
        db  08Dh, 049h, 000h
loc_7_2790:
        mov    edx, dword ptr [ebp - 8]
        mov    al, byte ptr [esi]
        cmp    byte ptr [edx + 0c6h], al
        jne    NEAR PTR loc_7_2AD2
        mov    al, byte ptr [edx + 0c8h]
        cmp    byte ptr [esi + 2], al
        jne    NEAR PTR loc_7_2AD2
        mov    al, byte ptr [edx + 0c9h]
        cmp    byte ptr [esi + 3], al
        je     NEAR PTR loc_7_2A39
        jmp    NEAR PTR loc_7_2AD2
loc_7_27C4:
        mov    al, byte ptr [esi]
        mov    byte ptr [ebp - 0ch], al
        cmp    al, 99h
        jne    SHORT loc_7_2820
        cmp    byte ptr [esi + 1], 99h
        jne    SHORT loc_7_2820
        mov    edx, dword ptr [ebp - 8]
        mov    al, byte ptr [edx + 0c8h]
        cmp    byte ptr [esi + 2], al
        jne    SHORT loc_7_2820
        mov    al, byte ptr [edx + 0c9h]
        cmp    byte ptr [esi + 3], al
        jne    SHORT loc_7_2820
        mov    edx, dword ptr [ebp + 8]
        db  066h, 0FFh, 005h, 0B6h, 020h, 000h, 000h
        mov    byte ptr [edx + 0b6h], 0
        mov    ax, word ptr [ebx + 6ch]
        add    ax, 6eh
        mov    word ptr [ebx + 6ch], ax
        mov    cx, word ptr [ebx + 6eh]
        cmp    ax, cx
        jbe    NEAR PTR loc_7_2AD2
        mov    word ptr [ebx + 6ch], cx
        jmp    NEAR PTR loc_7_2AD2
        db  090h
loc_7_2820:
        cmp    byte ptr [ebp - 0ch], 33h
        jne    NEAR PTR loc_7_2903
        cmp    byte ptr [esi + 1], 33h
        jne    NEAR PTR loc_7_2903
        cmp    cl, 1
        je     NEAR PTR loc_7_28E1
        mov    edx, dword ptr [ebp - 8]
        mov    al, byte ptr [edx + 0c9h]
        cmp    byte ptr [esi + 3], al
        je     SHORT loc_7_2857
        db  066h, 0FFh, 005h, 0AEh, 020h, 000h, 000h
        jmp    NEAR PTR loc_7_2AD2
loc_7_2857:
        mov    edx, dword ptr [ebp - 8]
        mov    al, byte ptr [edx + 0c8h]
        cmp    byte ptr [esi + 2], al
        je     SHORT loc_7_2890
        db  066h, 0FFh, 005h, 0B0h, 020h, 000h, 000h
        mov    cx, word ptr [ebx + 6ch]
        add    cx, 6eh
        mov    word ptr [ebx + 6ch], cx
        mov    ax, word ptr [ebx + 6eh]
        cmp    cx, ax
        jbe    NEAR PTR loc_7_2AD2
        mov    word ptr [ebx + 6ch], ax
        jmp    NEAR PTR loc_7_2AD2
        db  08Bh, 0FFh
loc_7_2890:
        db  0A0h, 05Eh, 004h, 000h, 000h
        mov    byte ptr [ebx + 54h], al
        cmp    cl, 2
        je     NEAR PTR loc_7_2A39
        mov    al, byte ptr [esi + 7]
        test   al, 1
        je     SHORT loc_7_28C0
        mov    edx, dword ptr [ebp + 8]
        db  066h, 0FFh, 005h, 0BAh, 020h, 000h, 000h
        mov    byte ptr [edx + 96h], 37h
        jmp    NEAR PTR loc_7_2CDB
        db  08Bh, 0FFh
loc_7_28C0:
        test   al, 40h
        je     SHORT loc_7_28C8
        mov    byte ptr [ebx + 55h], 1
loc_7_28C8:
        mov    al, byte ptr [esi + 6]
        or     al, al
        je     NEAR PTR loc_7_2A39
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [edx + 96h], al
        jmp    NEAR PTR loc_7_2CDB
loc_7_28E1:
        cmp    byte ptr [esi + 6], 0
        je     NEAR PTR loc_7_2A39
        db  066h, 0FFh, 005h, 0C2h, 020h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 6]
        mov    byte ptr [edx + 96h], al
        jmp    NEAR PTR loc_7_2CDB
loc_7_2903:
        db  066h, 0FFh, 005h, 0ACh, 020h, 000h, 000h
        jmp    NEAR PTR loc_7_2AD2
        db  090h
loc_7_2910:
        xor    eax, eax
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 6ch]
        mov    ax, word ptr [ebx + 70h]
        add    eax, ecx
        shl    eax, 2
        mov    esi, dword ptr [ebx + 48h]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    edi, [eax + eax*8]
        mov    al, byte ptr [esi + 44h]
        or     al, al
        je     SHORT loc_7_294A
        push   esi
        xor    edx, edx
        mov    dl, al
        sub    ecx, ecx
        call   sub_7_25F0
        cmp    dword ptr [esi + 0ch], edi
        jbe    SHORT loc_7_299A
        mov    dword ptr [esi + 0ch], edi
        jmp    SHORT loc_7_299A
loc_7_294A:
        mov    eax, dword ptr [esi + 38h]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebx + 24h], 0ee6bh
        mov    eax, 0
        setb   al
        inc    eax
        xor    ecx, ecx
        mov    cx, word ptr [esi + 46h]
        imul   eax, ecx
        cmp    eax, dword ptr [ebp - 18h]
        jae    SHORT loc_7_299A
        push   esi
        sub    edx, edx
        sub    ecx, ecx
        call   sub_7_25F0
        mov    edx, ebx
        lea    ecx, [esi + 0ch]
        call   sub_7_2580
        mov    eax, dword ptr [esi + 0ch]
        db  0A3h, 064h, 010h, 000h, 000h
loc_7_299A:
        mov    al, byte ptr [esi + 45h]
        or     al, al
        je     SHORT loc_7_29C0
        push   esi
        xor    edx, edx
        mov    dl, al
        mov    ecx, 1
        call   sub_7_25F0
        cmp    dword ptr [esi + 10h], edi
        jbe    SHORT loc_7_29FE
        mov    dword ptr [esi + 10h], edi
        jmp    SHORT loc_7_29FE
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_7_29C0:
        mov    ecx, 0
        cmp    dword ptr [ebx + 24h], 0ee6bh
        setb   cl
        inc    ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 46h]
        imul   ecx, eax
        cmp    ecx, dword ptr [esi + 1ch]
        jae    SHORT loc_7_29FE
        push   esi
        sub    edx, edx
        mov    ecx, 1
        call   sub_7_25F0
        lea    edi, [esi + 10h]
        mov    edx, ebx
        mov    ecx, edi
        call   sub_7_2580
        mov    eax, dword ptr [edi]
        db  0A3h, 068h, 010h, 000h, 000h
loc_7_29FE:
        mov    ecx, 3e8h
        mov    eax, dword ptr [esi + 10h]
        sub    edx, edx
        div    ecx
        mov    ecx, eax
        and    ecx, 0ffffh
        xor    edx, edx
        mov    dx, word ptr [ebx + 6ch]
        shl    ecx, 2
        cmp    ecx, edx
        jle    SHORT loc_7_2A31
        shl    ax, 2
        cmp    word ptr [ebx + 6eh], ax
        mov    word ptr [ebx + 6ch], ax
        jae    SHORT loc_7_2A31
        mov    byte ptr [ebx + 59h], 1
loc_7_2A31:
        db  0A0h, 05Eh, 004h, 000h, 000h
        mov    byte ptr [ebx + 54h], al
loc_7_2A39:
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [edx + 96h], 0
        mov    al, byte ptr [edx + 0b6h]
        or     al, al
        jne    NEAR PTR loc_7_2CAC
        mov    ax, word ptr [edx + 0b4h]
        mov    cx, word ptr [ebx + 70h]
        cmp    ax, cx
        jbe    NEAR PTR loc_7_2C80
        sub    ax, cx
        mov    word ptr [ebp - 2], ax
        jmp    NEAR PTR loc_7_2C86
loc_7_2A71:
        mov    edx, dword ptr [ebp - 8]
        mov    ax, word ptr [edx + 0aah]
        mov    edx, dword ptr [ebp + 8]
        cmp    word ptr [edx + 0d2h], ax
        jne    SHORT loc_7_2AD2
        mov    edx, dword ptr [ebp - 8]
        cmp    word ptr [edx + 22h], 1
        jbe    SHORT loc_7_2A39
        mov    edx, dword ptr [ebp + 8]
        cmp    word ptr [edx + 22h], 1
        jbe    SHORT loc_7_2A39
        mov    edx, dword ptr [ebp - 8]
        mov    ax, word ptr [edx + 2eh]
        mov    edx, dword ptr [ebp + 8]
        cmp    word ptr [edx + 2eh], ax
        jb     SHORT loc_7_2A39
        and    eax, 0ffffh
        mov    ecx, eax
        mov    edx, dword ptr [ebp - 8]
        shr    ecx, 2
        mov    esi, dword ptr [edx + 2ah]
        mov    edx, dword ptr [ebp + 8]
        mov    edi, dword ptr [edx + 2ah]
        repe cmpsd
        jne    SHORT loc_7_2ACC
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
loc_7_2ACC:
        je     NEAR PTR loc_7_2A39
loc_7_2AD2:
        mov    edx, dword ptr [ebp + 8]
        mov    al, byte ptr [edx + 0b6h]
        inc    al
        mov    byte ptr [edx + 0b6h], al
        mov    cl, byte ptr [edx + 0b7h]
        cmp    al, cl
        jbe    NEAR PTR loc_7_2B9E
        mov    al, byte ptr [edx + 95h]
        cmp    al, 3
        je     SHORT loc_7_2AFF
        cmp    al, 6
        jne    SHORT loc_7_2B34
loc_7_2AFF:
        db  0A0h, 05Fh, 004h, 000h, 000h
        db  03Ah, 005h, 05Eh, 004h, 000h, 000h
        je     SHORT loc_7_2B34
        cmp    al, cl
        je     SHORT loc_7_2B34
        mov    edx, dword ptr [ebp + 8]
        lea    eax, [ebp - 2]
        mov    byte ptr [edx + 8], 48h
        push   eax
        lea    eax, [ebp - 2ch]
        push   eax
        lea    eax, [ebx + 78h]
        push   eax
        call   sub_7_0B77
        mov    edx, dword ptr [ebp + 8]
        add    esp, 0ch
        mov    byte ptr [edx + 8], 0
        jmp    SHORT loc_7_2B36
loc_7_2B34:
        mov    al, 0ffh
loc_7_2B36:
        or     al, al
        jne    NEAR PTR loc_7_2BFF
        mov    edx, dword ptr [ebp - 8]
        lea    ecx, [ebp - 2ch]
        add    edx, 1ch
        mov    esi, dword ptr [ecx]
        cmp    esi, dword ptr [edx]
        jne    SHORT loc_7_2B55
        mov    cx, word ptr [ecx + 4]
        cmp    cx, word ptr [edx + 4]
loc_7_2B55:
        je     NEAR PTR loc_7_2BFB
        lea    eax, [ebp - 2ch]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        mov    ax, word ptr [ebp - 2]
        imul   ax, ax, 0a5h
        add    ax, 113h
        mov    word ptr [ebx + 6ch], ax
        mov    ax, word ptr [ebp - 2]
        imul   ax, ax, 226h
        add    ax, 113h
        mov    word ptr [ebx + 6eh], ax
        mov    ax, word ptr [ebp - 2]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ebx + 52h], ax
        mov    byte ptr [edx + 0b6h], 1
loc_7_2B9E:
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 0b4h], 0
        mov    eax, dword ptr [edx + 0c4h]
        or     eax, eax
        je     SHORT loc_7_2BCA
        mov    dword ptr [ebp - 14h], eax
        mov    esi, dword ptr [ebp - 14h]
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        mov    dword ptr [edx + 0c4h], 0
loc_7_2BCA:
        mov    edx, dword ptr [ebp - 8]
        cmp    byte ptr [edx + 94h], 0
        je     NEAR PTR loc_7_2D51
        cmp    byte ptr [edx + 8], 0
        jne    NEAR PTR loc_7_2D51
        mov    byte ptr [edx + 94h], 0
        push   edx
        call   sub_7_0C2C
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_7_2BFB:
        or     al, al
        je     SHORT loc_7_2C06
loc_7_2BFF:
        db  066h, 0FFh, 005h, 0B4h, 020h, 000h, 000h
loc_7_2C06:
        mov    edx, dword ptr [ebp + 8]
        cmp    byte ptr [edx + 95h], 1
        jne    SHORT loc_7_2C19
        db  066h, 0FFh, 005h, 0C4h, 020h, 000h, 000h
loc_7_2C19:
        mov    edx, dword ptr [ebp + 8]
        db  066h, 0FFh, 005h, 0AAh, 020h, 000h, 000h
        mov    dl, byte ptr [edx + 95h]
        cmp    dl, 5
        je     SHORT loc_7_2C5E
        xor    eax, eax
        xor    ecx, ecx
        db  08Ah, 00Dh, 0ECh, 003h, 000h, 000h
        mov    al, byte ptr [ebx + 56h]
        sub    eax, ecx
        cmp    eax, -1
        jne    SHORT loc_7_2C49
        db  0C6h, 005h, 0ECh, 003h, 000h, 000h, 000h
loc_7_2C49:
        db  0A0h, 05Fh, 004h, 000h, 000h
        db  03Ah, 005h, 05Eh, 004h, 000h, 000h
        mov    byte ptr [ebx + 5ch], 37h
        jb     SHORT loc_7_2C5E
        mov    byte ptr [ebx + 5ch], 33h
loc_7_2C5E:
        cmp    dl, 3
        je     SHORT loc_7_2C68
        cmp    dl, 6
        jne    SHORT loc_7_2C6B
loc_7_2C68:
        dec    byte ptr [ebx + 72h]
loc_7_2C6B:
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [edx + 96h], 33h
        db  0A0h, 05Fh, 004h, 000h, 000h
        mov    byte ptr [ebx + 54h], al
        jmp    SHORT loc_7_2CDB
        db  090h
loc_7_2C80:
        mov    word ptr [ebp - 2], 0
loc_7_2C86:
        mov    ax, word ptr [ebp - 2]
        cmp    word ptr [ebx + 6eh], ax
        jbe    SHORT loc_7_2CB4
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 6ch]
        and    eax, 0ffffh
        mov    esi, 5
        lea    eax, [eax + ecx*4]
        cdq
        idiv   esi
        mov    word ptr [ebx + 6ch], ax
        jmp    SHORT loc_7_2CB4
loc_7_2CAC:
        cmp    al, 4
        jbe    SHORT loc_7_2CB4
        shr    word ptr [ebx + 6ch], 1
loc_7_2CB4:
        db  0A1h, 0B4h, 003h, 000h, 000h
        add    eax, eax
        cmp    eax, 6eh
        ja     SHORT loc_7_2CC5
        mov    eax, 6eh
loc_7_2CC5:
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 6ch]
        cmp    ecx, eax
        jae    SHORT loc_7_2CD3
        mov    word ptr [ebx + 6ch], ax
loc_7_2CD3:
        mov    byte ptr [ebx + 57h], 1
        mov    byte ptr [ebx + 77h], 0
loc_7_2CDB:
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [edx + 94h], 0
        cli
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 0b4h], 0
        mov    eax, dword ptr [edx + 0c4h]
        or     eax, eax
        je     SHORT loc_7_2D12
        mov    dword ptr [ebp - 10h], eax
        mov    esi, dword ptr [ebp - 10h]
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        mov    dword ptr [edx + 0c4h], 0
loc_7_2D12:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0b0h]
        mov    eax, dword ptr [edx + 0ach]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        sti
        mov    byte ptr [edx + 94h], 0
        mov    eax, dword ptr [edx + 98h]
        or     eax, eax
        je     SHORT loc_7_2D45
        push   edx
        call   eax
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_7_2D45:
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_7_0AEA
        add    esp, 4
loc_7_2D51:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h, 027h, 000h, 000h, 0C4h, 027h, 000h, 000h, 0C4h, 027h, 000h, 000h, 0C4h, 027h, 000h, 000h
        db  010h, 029h, 000h, 000h, 071h, 02Ah, 000h, 000h, 0C4h, 027h, 000h, 000h, 005h, 000h, 000h, 000h
        db  000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_7_2D80:
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [ecx + 40h]
        mov    ebx, dword ptr [ecx + 48h]
        mov    dword ptr [esp + 30h], edi
        mov    esi, dword ptr [ebx + 38h]
        db  0C6h, 005h, 096h, 003h, 000h, 000h, 001h
        db  080h, 03Dh, 095h, 003h, 000h, 000h, 000h
        jne    SHORT loc_7_2DA9
        db  0E8h, 086h, 0D2h, 0F8h, 0FFh
loc_7_2DA9:
        mov    eax, dword ptr [ebx + 5eh]
        lea    edi, [ebx + 5eh]
        mov    dword ptr [esp + 14h], eax
        xor    eax, eax
        mov    ax, word ptr [ebx + 46h]
        mov    edx, dword ptr [ebx + 62h]
        cmp    edx, eax
        jbe    SHORT loc_7_2DC2
        mov    edx, eax
loc_7_2DC2:
        mov    eax, dword ptr [ebx + 8]
        inc    eax
        mov    dword ptr [ebx + 8], eax
        mov    byte ptr [esi + 2], 0
        mov    dword ptr [esp + 40h], eax
        mov    eax, dword ptr [esp + 40h]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [esp + 3ch], eax
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 38h], eax
        mov    eax, dword ptr [esp + 38h]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [esp + 34h], eax
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [esi + 1ch], eax
        mov    word ptr [esp + 12h], dx
        mov    ax, word ptr [esp + 12h]
        xchg   al, ah
        mov    word ptr [esp + 10h], ax
        mov    ax, word ptr [esp + 10h]
        mov    word ptr [esi + 20h], ax
        mov    word ptr [esi + 22h], 0
        mov    eax, dword ptr [ebx + 62h]
        cmp    edx, eax
        jae    SHORT loc_7_2E35
        add    dword ptr [edi], edx
        sub    eax, edx
        mov    dword ptr [ebx + 62h], eax
        jmp    SHORT loc_7_2E5B
loc_7_2E35:
        mov    ax, word ptr [ebx + 5ah]
        dec    ax
        mov    word ptr [ebx + 5ah], ax
        je     SHORT loc_7_2E57
        and    eax, 0ffffh
        lea    ecx, [eax*8]
        lea    esi, [ebx + 66h]
        shr    ecx, 2
        rep movsd
        jmp    SHORT loc_7_2E5B
loc_7_2E57:
        or     byte ptr [esi + 2], 10h
loc_7_2E5B:
        xor    eax, eax
        mov    ax, word ptr [ebx + 58h]
        sub    ecx, ecx
        mov    edi, dword ptr [esp + 30h]
        mov    dword ptr [esp + 18h], eax
        mov    esi, dword ptr [edi + 98h]
        lea    edi, [ebx + 0c2h]
loc_7_2E77:
        cmp    dword ptr [esp + 18h], ecx
        jbe    SHORT loc_7_2E84
        xor    eax, eax
        mov    ax, word ptr [edi]
        jmp    SHORT loc_7_2E87
loc_7_2E84:
        mov    eax, dword ptr [esi + 10h]
loc_7_2E87:
        cmp    dword ptr [esp + 14h], eax
        jb     SHORT loc_7_2EA4
        sub    dword ptr [esp + 14h], eax
        cmp    dword ptr [esp + 18h], ecx
        jbe    SHORT loc_7_2EA0
        add    edi, 6
        inc    ecx
        jmp    SHORT loc_7_2E77
        db  08Dh, 049h, 000h
loc_7_2EA0:
        mov    esi, dword ptr [esi]
        jmp    SHORT loc_7_2E77
loc_7_2EA4:
        mov    dword ptr [esp + 2ch], 1
        or     edx, edx
        je     NEAR PTR loc_7_2F40
        mov    eax, dword ptr [esp + 30h]
        add    eax, 2ah
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [ecx + ecx*2]
        lea    eax, [ebx + eax*2 + 0c2h]
        mov    dword ptr [esp + 20h], eax
loc_7_2ED0:
        cmp    dword ptr [esp + 18h], ecx
        jbe    SHORT loc_7_2EE4
        mov    eax, dword ptr [esp + 20h]
        xor    edi, edi
        mov    di, word ptr [eax]
        mov    eax, dword ptr [eax - 4]
        jmp    SHORT loc_7_2EEA
loc_7_2EE4:
        mov    edi, dword ptr [esi + 10h]
        mov    eax, dword ptr [esi + 14h]
loc_7_2EEA:
        mov    dword ptr [esp + 28h], eax
        sub    edi, dword ptr [esp + 14h]
        cmp    edi, edx
        jbe    SHORT loc_7_2EF8
        mov    edi, edx
loc_7_2EF8:
        mov    ebp, dword ptr [esp + 28h]
        mov    dword ptr [esp + 24h], ebp
        mov    eax, dword ptr [esp + 14h]
        add    dword ptr [esp + 24h], eax
        sub    edx, edi
        mov    eax, dword ptr [esp + 1ch]
        mov    ebp, dword ptr [esp + 24h]
        mov    dword ptr [eax], ebp
        mov    word ptr [eax + 4], di
        add    dword ptr [ebx + 34h], edi
        mov    dword ptr [esp + 14h], 0
        cmp    dword ptr [esp + 18h], ecx
        jbe    SHORT loc_7_2F31
        add    dword ptr [esp + 20h], 6
        inc    ecx
        jmp    SHORT loc_7_2F33
loc_7_2F31:
        mov    esi, dword ptr [esi]
loc_7_2F33:
        add    dword ptr [esp + 1ch], 6
        inc    dword ptr [esp + 2ch]
        or     edx, edx
        jne    SHORT loc_7_2ED0
loc_7_2F40:
        mov    edi, dword ptr [esp + 30h]
        add    dword ptr [ebx + 34h], 14h
        mov    ax, word ptr [esp + 2ch]
        mov    word ptr [edi + 22h], ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_7_2F60:
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, ecx
        mov    dword ptr [esp + 30h], ecx
        mov    ecx, dword ptr [edx + 40h]
        mov    edx, dword ptr [esp + 30h]
        db  0A1h, 05Ch, 010h, 000h, 000h
        inc    eax
        mov    edx, dword ptr [edx + 44h]
        mov    dword ptr [esp + 2ch], edx
        mov    edx, dword ptr [esp + 30h]
        mov    esi, dword ptr [edx + 48h]
        mov    edx, dword ptr [esi + 38h]
        mov    dword ptr [esp + 24h], edx
        db  0A3h, 05Ch, 010h, 000h, 000h
        cmp    dword ptr [esi + 1ch], 0
        jne    NEAR PTR loc_7_3064
        mov    eax, dword ptr [edx + 18h]
        mov    dword ptr [esp + 4ch], eax
        mov    eax, dword ptr [esp + 4ch]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [esp + 48h], eax
        mov    eax, dword ptr [esp + 48h]
        mov    edx, dword ptr [esp + 2ch]
        test   byte ptr [edx + 97h], 1
        je     SHORT loc_7_2FE0
        mov    edx, dword ptr [esp + 30h]
        cmp    dword ptr [edx + 24h], 0ee6bh
        jb     SHORT loc_7_2FE0
        mov    dl, byte ptr [esi + 44h]
        cmp    dl, 2
        jae    SHORT loc_7_2FE0
        add    dl, 2
        mov    byte ptr [esi + 44h], dl
loc_7_2FE0:
        mov    edx, dword ptr [esp + 2ch]
        test   byte ptr [edx + 0b6h], 1
        jne    SHORT loc_7_3040
        xor    edx, edx
        mov    dx, word ptr [esi + 46h]
        cmp    edx, eax
        jae    SHORT loc_7_3040
        mov    eax, dword ptr [esi + 8]
        inc    eax
        mov    dword ptr [esi + 8], eax
        mov    edx, dword ptr [esp + 24h]
        mov    byte ptr [edx + 2], 80h
        mov    dword ptr [esp + 44h], eax
        mov    eax, dword ptr [esp + 44h]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [esp + 40h], eax
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [edx + 1ch], 0
        mov    word ptr [edx + 20h], 0
        mov    word ptr [edx + 22h], 0
        mov    word ptr [ecx + 22h], 1
        jmp    NEAR PTR loc_7_317C
        db  08Bh, 0FFh
loc_7_3040:
        mov    word ptr [esi + 5ah], 1
        mov    dword ptr [esi + 5eh], 0
        mov    dword ptr [esi + 62h], eax
        mov    ecx, dword ptr [esp + 30h]
        call   sub_7_2D80
        mov    word ptr [esi + 5ah], 0
        jmp    NEAR PTR loc_7_317C
loc_7_3064:
        inc    byte ptr [esi + 45h]
        mov    eax, dword ptr [esi + 8]
        inc    eax
        mov    dword ptr [esi + 8], eax
        mov    edx, dword ptr [esp + 24h]
        mov    byte ptr [edx + 2], 80h
        mov    dword ptr [esp + 3ch], eax
        mov    eax, dword ptr [esp + 3ch]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [esp + 38h], eax
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [edx + 1ch], 0
        mov    word ptr [edx + 20h], 0
        lea    ebx, [edx + 24h]
        mov    dword ptr [esp + 34h], ebx
        mov    dword ptr [ecx + 2ah], ebx
        mov    dx, word ptr [esi + 5ch]
        mov    ax, dx
        imul   ax, ax, 6
        mov    word ptr [ecx + 2eh], ax
        mov    word ptr [ecx + 22h], 2
        sub    ecx, ecx
        mov    dword ptr [esp + 28h], ecx
        cmp    dx, cx
        je     NEAR PTR loc_7_315E
        lea    eax, [esi + 8eh]
        mov    dword ptr [esp + 20h], eax
loc_7_30D5:
        mov    eax, dword ptr [esp + 20h]
        mov    edx, dword ptr [eax]
        mov    edi, dword ptr [eax + 4]
        or     edi, edi
        je     SHORT loc_7_3145
        mov    ebx, dword ptr [esp + 34h]
        lea    eax, [ecx + ecx*2]
        lea    ebx, [ebx + eax*2]
loc_7_30F0:
        mov    bp, 0fffch
        cmp    edi, 0fffch
        jae    SHORT loc_7_30FF
        mov    bp, di
loc_7_30FF:
        mov    dword ptr [esp + 1ch], edx
        mov    eax, dword ptr [esp + 1ch]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ebx], eax
        mov    word ptr [esp + 12h], bp
        mov    ax, word ptr [esp + 12h]
        xchg   al, ah
        mov    word ptr [esp + 10h], ax
        mov    ax, word ptr [esp + 10h]
        mov    word ptr [ebx + 4], ax
        mov    eax, ebp
        and    eax, 0ffffh
        add    edx, eax
        sub    edi, eax
        add    ebx, 6
        inc    ecx
        or     edi, edi
        jne    SHORT loc_7_30F0
loc_7_3145:
        add    dword ptr [esp + 20h], 8
        xor    eax, eax
        inc    dword ptr [esp + 28h]
        mov    ax, word ptr [esi + 5ch]
        cmp    eax, dword ptr [esp + 28h]
        ja     NEAR PTR loc_7_30D5
loc_7_315E:
        mov    word ptr [esp + 16h], cx
        mov    ax, word ptr [esp + 16h]
        xchg   al, ah
        mov    word ptr [esp + 14h], ax
        mov    ax, word ptr [esp + 14h]
        mov    edx, dword ptr [esp + 24h]
        mov    word ptr [edx + 22h], ax
loc_7_317C:
        mov    edx, dword ptr [esp + 2ch]
        test   byte ptr [edx + 97h], 1
        jne    SHORT loc_7_31D6
        cmp    dword ptr [edx + 0c8h], 0
        jne    SHORT loc_7_31A6
        mov    edx, dword ptr [esp + 30h]
        xor    eax, eax
        mov    ax, word ptr [edx + 6ch]
        mov    edx, dword ptr [esp + 2ch]
        mov    dword ptr [edx + 0c8h], eax
loc_7_31A6:
        mov    edx, dword ptr [esp + 2ch]
        mov    eax, dword ptr [edx + 0c8h]
        mov    ecx, eax
        shr    ecx, 1
        lea    eax, [eax + ecx + 1]
        mov    dword ptr [edx + 0c8h], eax
        mov    edx, dword ptr [esp + 30h]
        xor    ecx, ecx
        mov    cx, word ptr [edx + 6eh]
        cmp    ecx, eax
        jae    SHORT loc_7_31D6
        mov    edx, dword ptr [esp + 2ch]
        mov    dword ptr [edx + 0c8h], ecx
loc_7_31D6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 40h
        ret
        db  08Bh, 0FFh
sub_7_31E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 90h]
        mov    eax, dword ptr [edi + 44h]
        mov    dword ptr [ebp - 4], eax
        mov    ebx, dword ptr [edi + 48h]
        and    byte ptr [eax + 97h], 0fbh
        cmp    word ptr [ebx + 5ah], 0
        jbe    SHORT loc_7_3260
        call   sub_7_09F3
        mov    dword ptr [ebp - 8], eax
        mov    ecx, 3e8h
        mov    eax, dword ptr [ebx + 0ch]
        sub    edx, edx
        div    ecx
        add    eax, dword ptr [esi + 0a0h]
        cmp    eax, dword ptr [ebp - 8]
        jbe    SHORT loc_7_3243
        cmp    dword ptr [edi + 30h], 0
        jne    SHORT loc_7_328D
        push   edi
        db  068h, 0AEh, 004h, 000h, 000h
        sub    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_7_0A09
        add    esp, 0ch
        mov    dword ptr [edi + 30h], eax
loc_7_3243:
        cmp    dword ptr [edi + 30h], 0
        jne    SHORT loc_7_328D
        mov    ecx, edi
        call   sub_7_2D80
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0a0h], eax
        jmp    SHORT loc_7_3284
        db  005h, 000h, 000h, 000h, 000h
loc_7_3260:
        cmp    byte ptr [esi + 94h], 0
        je     SHORT loc_7_328D
        cmp    dword ptr [ebx + 1ch], 0
        je     SHORT loc_7_3276
        cmp    word ptr [ebx + 5ch], 0
        jbe    SHORT loc_7_328D
loc_7_3276:
        mov    ecx, edi
        call   sub_7_2F60
        mov    byte ptr [esi + 94h], 0
loc_7_3284:
        push   esi
        call   sub_7_0C2C
        add    esp, 4
loc_7_328D:
        cmp    dword ptr [esi + 9ch], 0
        je     SHORT loc_7_329F
        push   esi
        call   sub_7_0AEA
        add    esp, 4
loc_7_329F:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [esi + 94h], 0
        mov    word ptr [eax + 0b4h], 0
        mov    eax, dword ptr [eax + 0c4h]
        or     eax, eax
        je     SHORT loc_7_32D5
        mov    dword ptr [ebp - 0ch], eax
        mov    esi, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0c4h], 0
loc_7_32D5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_7_32E0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 40h]
        mov    dword ptr [eax + 30h], 0
        cmp    byte ptr [ecx + 8], 0
        jne    SHORT loc_7_3301
        mov    byte ptr [ecx + 9], 0
        push   ecx
        call   sub_7_31E0
        add    esp, 4
loc_7_3301:
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_7_3310:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [ecx + 44h]
        mov    edx, dword ptr [ecx + 48h]
        mov    esi, dword ptr [ecx + 40h]
        cmp    word ptr [edx + 5ah], 0
        ja     SHORT loc_7_33A1
        cmp    word ptr [edx + 5ch], 0
        je     SHORT loc_7_33A1
        mov    bl, byte ptr [eax + 0b6h]
        inc    bl
        mov    byte ptr [eax + 0b6h], bl
        movzx  ebp, byte ptr [eax + 0b7h]
        shl    ebp, 2
        mov    edi, ebx
        and    edi, 0ffh
        cmp    ebp, edi
        jge    SHORT loc_7_3370
        mov    word ptr [edx + 5ah], 0
        mov    word ptr [edx + 5ch], 0
        mov    byte ptr [eax + 9], 0fch
        push   eax
        call   sub_7_26E0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_7_3370:
        cmp    byte ptr [esi + 8], 0
        je     SHORT loc_7_3393
        mov    word ptr [eax + 0b4h], 0
        dec    bl
        mov    byte ptr [eax + 0b6h], bl
        mov    byte ptr [esi + 94h], 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_7_3393:
        call   sub_7_2F60
        push   esi
        call   sub_7_0C2C
        add    esp, 4
loc_7_33A1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_7_33B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 7ch
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edi + 44h]
        mov    dword ptr [ebp - 3ch], ecx
        lea    ecx, [esi + 1eh]
        mov    eax, dword ptr [edi + 40h]
        mov    dword ptr [ebp - 54h], eax
        mov    eax, dword ptr [edi + 48h]
        mov    dword ptr [ebp - 20h], eax
        mov    dword ptr [ebp - 34h], ecx
        db  0C6h, 005h, 096h, 003h, 000h, 000h, 001h
        db  080h, 03Dh, 095h, 003h, 000h, 000h, 000h
        jne    SHORT loc_7_33EC
        db  0E8h, 043h, 0CCh, 0F8h, 0FFh
loc_7_33EC:
        mov    eax, dword ptr [ebp - 20h]
        cmp    word ptr [eax + 5ch], 0
        jne    NEAR PTR loc_7_34B0
        mov    ecx, dword ptr [ebp - 34h]
        cmp    word ptr [ecx + 22h], 0
        jbe    NEAR PTR loc_7_34A1
        db  08Bh, 01Dh, 0B4h, 005h, 000h, 000h
        cmp    byte ptr [ebx + 8], 0
        jne    NEAR PTR loc_7_34A1
        lea    edi, [ebx + 2ah]
        mov    ecx, 7
        rep movsd
        movsw
        lea    edx, [ebx + 48h]
        mov    edi, edx
        mov    esi, dword ptr [ebp - 34h]
        mov    ecx, 9
        rep movsd
        mov    byte ptr [edx + 2], 84h
        mov    word ptr [edx + 22h], 0
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx + 8]
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [edx + 8], eax
        mov    ax, word ptr [edx + 14h]
        mov    cx, word ptr [edx + 16h]
        mov    word ptr [edx + 14h], cx
        mov    word ptr [edx + 16h], ax
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [eax + 8]
        inc    ecx
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [ebp - 74h], ecx
        mov    eax, dword ptr [ebp - 74h]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [ebp - 70h], eax
        mov    eax, dword ptr [ebp - 70h]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [edx + 18h], 0
        mov    dword ptr [edx + 10h], 0
        mov    word ptr [ebx + 22h], 0
        db  0C7h, 043h, 004h, 0F0h, 03Ah, 000h, 000h
        mov    byte ptr [ebx + 8], 0ffh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_7_34A1:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_7_34B0:
        cmp    word ptr [ebp + 10h], 42h
        jae    SHORT loc_7_34C0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_7_34C0:
        mov    ecx, dword ptr [ebp - 34h]
        cmp    word ptr [ecx], 7777h
        je     SHORT loc_7_34D3
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_7_34D3:
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 34h]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [ecx + 4], eax
        je     SHORT loc_7_34EA
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_7_34EA:
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 34h]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [ecx + 8], eax
        je     SHORT loc_7_3500
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_7_3500:
        mov    ecx, dword ptr [ebp - 34h]
        mov    ax, word ptr [ecx + 14h]
        mov    word ptr [ebp - 18h], ax
        mov    ax, word ptr [ebp - 18h]
        xchg   al, ah
        mov    word ptr [ebp - 16h], ax
        mov    cx, word ptr [ebp - 16h]
        mov    eax, dword ptr [ebp - 20h]
        cmp    word ptr [eax + 4ch], cx
        je     SHORT loc_7_3530
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_7_3530:
        mov    ecx, dword ptr [ebp - 34h]
        mov    ax, word ptr [ecx + 16h]
        mov    word ptr [ebp - 14h], ax
        mov    ax, word ptr [ebp - 14h]
        xchg   al, ah
        mov    word ptr [ebp - 12h], ax
        mov    cx, word ptr [ebp - 12h]
        mov    eax, dword ptr [ebp - 20h]
        mov    word ptr [eax + 48h], cx
        mov    ecx, dword ptr [ebp - 34h]
        mov    ax, word ptr [ecx + 14h]
        mov    word ptr [ebp - 10h], ax
        mov    ax, word ptr [ebp - 10h]
        xchg   al, ah
        mov    word ptr [ebp - 0eh], ax
        mov    cx, word ptr [ebp - 0eh]
        inc    cx
        mov    eax, dword ptr [ebp - 20h]
        mov    word ptr [eax + 4ah], cx
        mov    ecx, dword ptr [ebp - 3ch]
        mov    eax, dword ptr [ecx + 0c4h]
        or     eax, eax
        je     SHORT loc_7_3595
        mov    dword ptr [ebp - 6ch], eax
        mov    esi, dword ptr [ebp - 6ch]
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        mov    dword ptr [ecx + 0c4h], 0
loc_7_3595:
        mov    ecx, dword ptr [ebp - 3ch]
        sub    eax, eax
        mov    word ptr [ecx + 0b4h], 0
        mov    byte ptr [ecx + 0b6h], 0
        db  08Bh, 00Dh, 0B0h, 003h, 000h, 000h
        sub    edx, edx
loc_7_35B2:
        mov    dword ptr [ebp - 2ch], ecx
        cmp    byte ptr [ecx + 8], dl
        je     SHORT loc_7_35C6
        add    ecx, 9ah
        inc    eax
        cmp    eax, 8
        jb     SHORT loc_7_35B2
loc_7_35C6:
        cmp    eax, 8
        jne    SHORT loc_7_35E0
        db  0A1h, 05Ch, 010h, 000h, 000h
        inc    eax
        db  0A3h, 05Ch, 010h, 000h, 000h
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
loc_7_35E0:
        mov    esi, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 34h]
        mov    dword ptr [esi + 90h], edi
        mov    word ptr [esi + 98h], 0
        mov    cx, word ptr [ecx + 22h]
        or     cx, cx
        je     NEAR PTR loc_7_36D6
        db  0A1h, 05Ch, 010h, 000h, 000h
        inc    eax
        db  0A3h, 05Ch, 010h, 000h, 000h
        mov    eax, dword ptr [ebp - 20h]
        inc    byte ptr [eax + 44h]
        mov    word ptr [ebp - 0ch], cx
        mov    ax, word ptr [ebp - 0ch]
        xchg   al, ah
        mov    word ptr [ebp - 0ah], ax
        mov    cx, word ptr [ebp - 0ah]
        mov    eax, dword ptr [ebp - 20h]
        mov    word ptr [eax + 5ah], cx
        cmp    cx, 6
        jbe    SHORT loc_7_3638
        mov    word ptr [eax + 5ah], 6
loc_7_3638:
        mov    ecx, dword ptr [ebp - 34h]
        add    ecx, 24h
        mov    eax, dword ptr [ebp - 20h]
        sub    esi, esi
        cmp    word ptr [eax + 5ah], si
        jbe    SHORT loc_7_3699
        mov    edx, eax
        add    edx, 5eh
        mov    edi, edi
loc_7_3650:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 48h], eax
        mov    eax, dword ptr [ebp - 48h]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [ebp - 44h], eax
        mov    eax, dword ptr [ebp - 44h]
        mov    dword ptr [edx], eax
        mov    ax, word ptr [ecx + 4]
        mov    word ptr [ebp - 4], ax
        mov    ax, word ptr [ebp - 4]
        xchg   al, ah
        mov    word ptr [ebp - 2], ax
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        mov    dword ptr [edx + 4], eax
        add    edx, 8
        inc    esi
        add    ecx, 6
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 5ah]
        and    eax, 0ffffh
        cmp    eax, esi
        ja     SHORT loc_7_3650
loc_7_3699:
        mov    eax, dword ptr [ebp - 54h]
        cmp    byte ptr [eax + 8], 0
        jne    SHORT loc_7_36CD
        mov    esi, dword ptr [ebp - 2ch]
        mov    byte ptr [eax + 94h], 2
        mov    word ptr [esi + 22h], 0
        mov    eax, dword ptr [ebp - 20h]
        db  0C7h, 046h, 004h, 010h, 03Ah, 000h, 000h
        inc    word ptr [eax + 56h]
        mov    eax, esi
        mov    byte ptr [esi + 8], 0ffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_7_36CD:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_7_36D6:
        mov    ecx, dword ptr [ebp - 34h]
        test   byte ptr [ecx + 2], 80h
        je     SHORT loc_7_36E8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_7_36E8:
        mov    eax, dword ptr [ebp - 20h]
        cmp    dword ptr [eax + 1ch], 0
        jne    SHORT loc_7_3739
        mov    ecx, dword ptr [ebp - 34h]
        mov    eax, dword ptr [ecx + 18h]
        mov    dword ptr [ebp - 68h], eax
        mov    eax, dword ptr [ebp - 68h]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [ebp - 64h], eax
        mov    ecx, dword ptr [ebp - 64h]
        mov    eax, dword ptr [ebp - 20h]
        mov    dword ptr [eax + 1ch], ecx
        cmp    dword ptr [eax + 92h], ecx
        jbe    SHORT loc_7_371E
        mov    dword ptr [eax + 92h], ecx
loc_7_371E:
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, 3e8h
        mov    eax, dword ptr [eax + 10h]
        shl    eax, 2
        sub    edx, edx
        div    ecx
        mov    ecx, dword ptr [ebp - 3ch]
        mov    dword ptr [ecx + 0c8h], eax
loc_7_3739:
        mov    ecx, dword ptr [ebp - 3ch]
        test   byte ptr [ecx + 97h], 1
        jne    SHORT loc_7_374F
        mov    dword ptr [ecx + 0c8h], 0
loc_7_374F:
        mov    ecx, dword ptr [ebp - 34h]
        mov    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 60h], eax
        mov    eax, dword ptr [ebp - 60h]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [ebp - 5ch], eax
        mov    ebx, dword ptr [ebp - 5ch]
        mov    dword ptr [ebp - 1ch], ebx
        mov    ax, word ptr [ecx + 20h]
        mov    word ptr [ebp - 8], ax
        mov    ax, word ptr [ebp - 8]
        xchg   al, ah
        mov    word ptr [ebp - 6], ax
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        mov    dword ptr [ebp - 24h], eax
        mov    dword ptr [ebp - 30h], 0
        mov    eax, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + 5ch]
        mov    word ptr [ebp - 40h], ax
        or     ax, ax
        je     SHORT loc_7_37D0
        mov    esi, dword ptr [ebp - 20h]
        add    esi, 8eh
loc_7_37A6:
        mov    ecx, dword ptr [esi]
        mov    edx, dword ptr [esi + 4]
        cmp    ecx, dword ptr [ebp - 1ch]
        ja     SHORT loc_7_37B8
        lea    eax, [ecx + edx]
        cmp    eax, dword ptr [ebp - 1ch]
        ja     SHORT loc_7_37D6
loc_7_37B8:
        add    esi, 8
        inc    dword ptr [ebp - 30h]
        mov    eax, dword ptr [ebp - 40h]
        and    eax, 0ffffh
        cmp    dword ptr [ebp - 30h], eax
        jb     SHORT loc_7_37A6
        jmp    SHORT loc_7_37D6
        db  08Dh, 049h, 000h
loc_7_37D0:
        mov    ecx, dword ptr [ebp - 78h]
        mov    edx, dword ptr [ebp - 7ch]
loc_7_37D6:
        mov    edi, dword ptr [ebp - 40h]
        and    edi, 0ffffh
        cmp    dword ptr [ebp - 30h], edi
        jne    SHORT loc_7_37F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_7_37F0:
        mov    esi, dword ptr [ebp - 24h]
        add    esi, dword ptr [ebp - 1ch]
        lea    eax, [ecx + edx]
        cmp    esi, eax
        jbe    SHORT loc_7_3807
        mov    eax, edx
        sub    eax, dword ptr [ebp - 1ch]
        add    eax, ecx
        mov    dword ptr [ebp - 24h], eax
loc_7_3807:
        mov    esi, dword ptr [ebp - 2ch]
        mov    ebx, dword ptr [ebp - 1ch]
        mov    dword ptr [esi + 94h], ebx
        mov    ax, word ptr [ebp - 24h]
        cmp    ecx, ebx
        mov    word ptr [esi + 98h], ax
        jne    SHORT loc_7_3880
        mov    eax, dword ptr [ebp - 20h]
        mov    esi, dword ptr [ebp - 30h]
        lea    edi, [eax + esi*8]
        mov    eax, dword ptr [ebp - 24h]
        cmp    dword ptr [edi + 92h], eax
        jbe    SHORT loc_7_3847
        add    dword ptr [edi + 8eh], eax
        sub    dword ptr [edi + 92h], eax
        jmp    NEAR PTR loc_7_390A
loc_7_3847:
        mov    cx, word ptr [ebp - 40h]
        mov    eax, dword ptr [ebp - 20h]
        dec    cx
        mov    word ptr [eax + 5ch], cx
        mov    eax, ecx
        and    eax, 0ffffh
        sub    eax, dword ptr [ebp - 30h]
        lea    ecx, [eax*8]
        lea    esi, [edi + 96h]
        add    edi, 8eh
        shr    ecx, 2
        rep movsd
        jmp    NEAR PTR loc_7_390A
        db  005h, 000h, 000h, 000h, 000h
loc_7_3880:
        mov    esi, dword ptr [ebp - 24h]
        add    esi, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 58h], esi
        lea    eax, [ecx + edx]
        cmp    eax, esi
        jbe    SHORT loc_7_38F8
        cmp    word ptr [ebp - 40h], 6
        jne    SHORT loc_7_38A0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_7_38A0:
        mov    eax, dword ptr [ebp - 30h]
        inc    eax
        cmp    eax, edi
        jae    SHORT loc_7_38D7
        mov    eax, dword ptr [ebp - 20h]
        lea    esi, [eax + edi*8 + 8eh]
        mov    eax, edi
        sub    eax, dword ptr [ebp - 30h]
        dec    eax
        mov    dword ptr [ebp - 50h], eax
        sub    edi, eax
loc_7_38C0:
        mov    ebx, dword ptr [esi - 8]
        lea    eax, [esi - 8]
        sub    esi, 8
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 8], ebx
        mov    dword ptr [esi + 0ch], eax
        dec    dword ptr [ebp - 50h]
        jne    SHORT loc_7_38C0
loc_7_38D7:
        mov    eax, dword ptr [ebp - 20h]
        sub    edx, dword ptr [ebp - 24h]
        inc    word ptr [eax + 5ch]
        lea    eax, [eax + edi*8]
        mov    esi, dword ptr [ebp - 58h]
        mov    dword ptr [eax + 8eh], esi
        sub    edx, dword ptr [ebp - 1ch]
        add    edx, ecx
        mov    dword ptr [eax + 92h], edx
loc_7_38F8:
        mov    edx, dword ptr [ebp - 1ch]
        sub    edx, ecx
        mov    eax, dword ptr [ebp - 20h]
        mov    esi, dword ptr [ebp - 30h]
        mov    dword ptr [eax + esi*8 + 92h], edx
loc_7_390A:
        mov    ecx, dword ptr [ebp - 34h]
        test   byte ptr [ecx + 2], 10h
        je     SHORT loc_7_3927
        mov    eax, dword ptr [ebp - 20h]
        cmp    word ptr [eax + 5ch], 0
        jbe    SHORT loc_7_3927
        mov    eax, dword ptr [ebp - 54h]
        mov    byte ptr [eax + 94h], 1
loc_7_3927:
        mov    ecx, dword ptr [ebp - 3ch]
        xor    eax, eax
        mov    ax, word ptr [ecx + 22h]
        sub    edx, edx
        mov    dword ptr [ebp - 28h], eax
        mov    edi, dword ptr [ecx + 0c0h]
        mov    eax, ecx
        add    eax, 28h
loc_7_3940:
        cmp    dword ptr [ebp - 28h], edx
        jbe    SHORT loc_7_3950
        xor    ecx, ecx
        mov    cx, word ptr [eax]
        jmp    SHORT loc_7_3953
        db  08Dh, 064h, 024h, 000h
loc_7_3950:
        mov    ecx, dword ptr [edi + 10h]
loc_7_3953:
        cmp    ecx, dword ptr [ebp - 1ch]
        ja     SHORT loc_7_3970
        sub    dword ptr [ebp - 1ch], ecx
        cmp    dword ptr [ebp - 28h], edx
        jbe    SHORT loc_7_3966
        add    eax, 6
        inc    edx
        jmp    SHORT loc_7_3940
loc_7_3966:
        mov    edi, dword ptr [edi]
        jmp    SHORT loc_7_3940
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_7_3970:
        mov    dword ptr [ebp - 4ch], 0
        cmp    dword ptr [ebp - 24h], 0
        je     SHORT loc_7_39E3
        mov    esi, dword ptr [ebp - 2ch]
        add    esi, 24h
        mov    ecx, dword ptr [ebp - 3ch]
        lea    eax, [edx + edx*2]
        lea    ecx, [ecx + eax*2 + 28h]
loc_7_3990:
        cmp    dword ptr [ebp - 28h], edx
        jbe    SHORT loc_7_39A0
        xor    eax, eax
        mov    ax, word ptr [ecx]
        mov    ebx, dword ptr [ecx - 4]
        jmp    SHORT loc_7_39A6
        db  090h
loc_7_39A0:
        mov    eax, dword ptr [edi + 10h]
        mov    ebx, dword ptr [edi + 14h]
loc_7_39A6:
        mov    dword ptr [ebp - 38h], ebx
        sub    eax, dword ptr [ebp - 1ch]
        cmp    eax, dword ptr [ebp - 24h]
        jbe    SHORT loc_7_39B4
        mov    eax, dword ptr [ebp - 24h]
loc_7_39B4:
        mov    ebx, dword ptr [ebp - 1ch]
        add    ebx, dword ptr [ebp - 38h]
        mov    dword ptr [esi], ebx
        sub    dword ptr [ebp - 24h], eax
        mov    word ptr [esi + 4], ax
        mov    dword ptr [ebp - 1ch], 0
        cmp    dword ptr [ebp - 28h], edx
        jbe    SHORT loc_7_39D5
        add    ecx, 6
        inc    edx
        jmp    SHORT loc_7_39D7
loc_7_39D5:
        mov    edi, dword ptr [edi]
loc_7_39D7:
        add    esi, 6
        inc    dword ptr [ebp - 4ch]
        cmp    dword ptr [ebp - 24h], 0
        jne    SHORT loc_7_3990
loc_7_39E3:
        mov    esi, dword ptr [ebp - 2ch]
        mov    ax, word ptr [ebp - 4ch]
        mov    word ptr [esi + 22h], ax
        mov    eax, dword ptr [ebp - 20h]
        db  0C7h, 046h, 004h, 010h, 03Ah, 000h, 000h
        inc    word ptr [eax + 56h]
        mov    eax, esi
        mov    byte ptr [esi + 8], 0ffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_7_3A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 90h]
        mov    dword ptr [ebp - 4], eax
        mov    edi, dword ptr [eax + 44h]
        mov    ebx, dword ptr [eax + 48h]
        mov    esi, dword ptr [eax + 40h]
        dec    word ptr [ebx + 56h]
        mov    ax, word ptr [ecx + 98h]
        mov    edx, eax
        and    edx, 0ffffh
        add    edx, 14h
        add    dword ptr [ebx + 34h], edx
        cmp    dword ptr [ebx + 3ch], 0
        je     SHORT loc_7_3A5F
        or     ax, ax
        je     SHORT loc_7_3A5F
        push   eax
        mov    edx, dword ptr [ecx + 94h]
        mov    ecx, dword ptr [ebx + 40h]
        call   dword ptr [ebx + 3ch]
loc_7_3A5F:
        cmp    word ptr [ebx + 5ch], 0
        jne    SHORT loc_7_3A80
        cmp    word ptr [ebx + 56h], 0
        jne    SHORT loc_7_3AB5
        mov    word ptr [ebx + 5ah], 0
        mov    byte ptr [edi + 9], 0
        push   edi
        call   sub_7_26E0
        jmp    SHORT loc_7_3AB2
        db  090h
loc_7_3A80:
        mov    al, byte ptr [esi + 94h]
        or     al, al
        je     SHORT loc_7_3AB5
        cmp    byte ptr [esi + 8], 0
        jne    SHORT loc_7_3AB5
        cmp    al, 2
        mov    ecx, dword ptr [ebp - 4]
        jne    SHORT loc_7_3AA0
        call   sub_7_2D80
        jmp    SHORT loc_7_3AA5
        db  08Bh, 0FFh
loc_7_3AA0:
        call   sub_7_2F60
loc_7_3AA5:
        mov    byte ptr [esi + 94h], 0
        push   esi
        call   sub_7_0C2C
loc_7_3AB2:
        add    esp, 4
loc_7_3AB5:
        mov    eax, dword ptr [edi + 0c4h]
        or     eax, eax
        je     SHORT loc_7_3AD5
        mov    dword ptr [ebp - 8], eax
        mov    esi, dword ptr [ebp - 8]
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        mov    dword ptr [edi + 0c4h], 0
loc_7_3AD5:
        mov    word ptr [edi + 0b4h], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_7_3AF0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    word ptr [esi + 22h], 1
        lea    eax, [esi + 2ah]
        mov    dword ptr [esi + 24h], eax
        mov    word ptr [esi + 28h], 42h
        db  0C7h, 046h, 004h, 040h, 03Bh, 000h, 000h
        mov    dx, word ptr [esi + 3ah]
        lea    edi, [esi + 30h]
        mov    eax, dword ptr [esi + 3ch]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 40h]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi + 44h]
        lea    ecx, [esi + 3ch]
        mov    dword ptr [edi + 8], eax
        mov    word ptr [esi + 0ah], dx
        push   esi
        call   sub_7_0C2C
        add    esp, 4
        pop    edi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_7_3B40:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_7_3B50:
        or     cl, cl
        jne    SHORT loc_7_3B58
        sub    ax, ax
        ret
loc_7_3B58:
        cmp    cl, 0ffh
        jne    SHORT loc_7_3B62
        mov    ax, 2
        ret
loc_7_3B62:
        cmp    cl, 80h
        jne    SHORT loc_7_3B70
        mov    ax, 20h
        ret
        db  08Dh, 064h, 024h, 000h
loc_7_3B70:
        cmp    cl, 85h
        jne    SHORT loc_7_3B7A
        mov    ax, 50h
        ret
loc_7_3B7A:
        cmp    cl, 92h
        jne    SHORT loc_7_3B84
        mov    ax, 50h
        ret
loc_7_3B84:
        cmp    cl, 98h
        jne    SHORT loc_7_3B90
        mov    ax, 3
        ret
        db  08Bh, 0FFh
loc_7_3B90:
        cmp    cl, 9ch
        jne    SHORT loc_7_3B9A
        mov    ax, 3
        ret
loc_7_3B9A:
        cmp    cl, 0a5h
        jne    SHORT loc_7_3BA4
        mov    ax, 2
        ret
loc_7_3BA4:
        cmp    cl, 0fdh
        jne    SHORT loc_7_3BB0
        mov    ax, 21h
        ret
        db  08Bh, 0FFh
loc_7_3BB0:
        cmp    cl, 33h
        jne    SHORT loc_7_3BBA
        mov    ax, 33h
        ret
loc_7_3BBA:
        cmp    cl, 37h
        jne    SHORT loc_7_3BC4
        mov    ax, 37h
        ret
loc_7_3BC4:
        mov    ax, 5
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_7_3BD0:
        mov    eax, dword ptr [ecx + 1ch]
        or     edx, edx
        mov    al, byte ptr [eax + 21h]
        je     SHORT loc_7_3BE5
        cmp    al, 4
        je     SHORT loc_7_3BE1
        mov    al, 80h
        ret
loc_7_3BE1:
        or     edx, edx
        jne    SHORT loc_7_3BEB
loc_7_3BE5:
        cmp    al, 4
        jne    SHORT loc_7_3BEB
        mov    al, 0ffh
loc_7_3BEB:
        ret
        db  08Dh, 064h, 024h, 000h
sub_7_3BF0:
        mov    edx, dword ptr [ecx]
        and    edx, 24000000h
        jmp    NEAR PTR sub_7_3BD0
        db  08Dh, 049h, 000h
sub_7_3C00:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, edx
        mov    esi, ecx
        mov    bp, 6
        mov    ebx, dword ptr [esp + 14h]
loc_7_3C10:
        mov    eax, dword ptr [esp + 18h]
        push   eax
        push   ebx
        mov    eax, ebp
        and    eax, 0ffffh
        push   eax
        push   esi
        push   -1
        push   40h
        db  068h, 018h, 015h, 000h, 000h
        push   edi
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_7_027F
        add    esp, 24h
        test   bp, 10h
        jne    SHORT loc_7_3C44
        or     bp, 10h
        or     al, al
        jne    SHORT loc_7_3C10
loc_7_3C44:
        cmp    al, 1
        pop    ebp
        sbb    eax, eax
        neg    eax
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_7_3C60:
        sub    esp, 54h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 12h], 0
        mov    dword ptr [esp + 28h], 0
        mov    byte ptr [esp + 16h], 0
        mov    byte ptr [esp + 13h], 0ffh
        mov    byte ptr [esp + 17h], 0
        mov    ebx, dword ptr [esp + 68h]
        mov    byte ptr [esp + 15h], 0
        mov    esi, dword ptr [ebx + 0ch]
        mov    dl, byte ptr [ebx + 4]
        cmp    dl, 1
        je     SHORT loc_7_3CB3
        or     dl, dl
        je     SHORT loc_7_3CB3
        cmp    dl, 2
        je     SHORT loc_7_3CB3
        mov    word ptr [ebx + 1ah], 35h
        mov    eax, 35h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
loc_7_3CB3:
        test   byte ptr [ebx + 18h], 8
        je     SHORT loc_7_3CD5
        db  080h, 03Dh, 0C6h, 003h, 000h, 000h, 000h
        jne    SHORT loc_7_3CD5
        mov    word ptr [ebx + 1ah], 35h
        mov    eax, 35h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
loc_7_3CD5:
        mov    di, word ptr [esi + 4]
        add    esi, 4
        mov    eax, edi
        and    eax, 0ffffh
        lea    ecx, [esi + eax]
        mov    dword ptr [esp + 24h], ecx
        db  080h, 03Dh, 094h, 003h, 000h, 000h, 000h
        je     NEAR PTR loc_7_3E54
        mov    cx, word ptr [esi + 2]
        cmp    cx, 2ah
        jne    NEAR PTR loc_7_3DA0
        cmp    word ptr [esi + 4], 0
        jne    NEAR PTR loc_7_3DA0
        cmp    dl, 2
        je     SHORT loc_7_3D1D
        or     dl, dl
        jne    NEAR PTR loc_7_3DA0
loc_7_3D1D:
        db  08Bh, 00Dh, 0B8h, 014h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_7_3D3A
        mov    eax, dword ptr [ebx + 8]
loc_7_3D2A:
        cmp    dword ptr [ecx + 40h], eax
        je     SHORT loc_7_3D36
        mov    ecx, dword ptr [ecx + 4]
        or     ecx, ecx
        jne    SHORT loc_7_3D2A
loc_7_3D36:
        or     ecx, ecx
        jne    SHORT loc_7_3D50
loc_7_3D3A:
        mov    word ptr [ebx + 1ah], 43h
        mov    eax, 43h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
        db  08Dh, 049h, 000h
loc_7_3D50:
        cmp    dword ptr [ecx], 534552h
        jne    SHORT loc_7_3D60
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [ebx + 20h], eax
        jmp    SHORT loc_7_3D6C
loc_7_3D60:
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx], 534552h
loc_7_3D6C:
        mov    byte ptr [ebx + 4], 2
        mov    byte ptr [ecx + 1ch], 2
        mov    byte ptr [ecx + 1dh], 0
        mov    dword ptr [ebx + 1ch], ecx
        sub    eax, eax
        mov    dword ptr [ebx], 1
        db  0C7h, 043h, 010h, 078h, 014h, 000h, 000h
        mov    word ptr [ebx + 6], 0
        mov    word ptr [ebx + 1ah], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
        db  08Bh, 0FFh
loc_7_3DA0:
        cmp    cx, 3fh
        jne    NEAR PTR loc_7_3E54
        cmp    word ptr [esi + 4], 12h
        jne    NEAR PTR loc_7_3E54
        mov    byte ptr [esp + 15h], 1
        sub    edx, edx
        lea    esp, [esp]
loc_7_3DC0:
        mov    ecx, dword ptr [esp + 24h]
        mov    ax, word ptr [ecx + edx*2 + 2]
        cmp    ax, 41h
        jb     SHORT loc_7_3DE0
        and    eax, 0ffffh
        sub    eax, 37h
        jmp    SHORT loc_7_3DE8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_7_3DE0:
        and    eax, 0ffffh
        sub    eax, 30h
loc_7_3DE8:
        mov    cl, 7
        sub    cl, dl
        shl    cl, 2
        shl    eax, cl
        add    dword ptr [esp + 28h], eax
        inc    edx
        cmp    edx, 8
        jb     SHORT loc_7_3DC0
        mov    eax, dword ptr [esp + 28h]
        mov    al, byte ptr [eax]
        cmp    al, 81h
        jb     SHORT loc_7_3E20
        xor    ecx, ecx
        mov    cl, al
        lea    ecx, [ecx + ecx*4]
        shl    ecx, 5
        db  0A1h, 0A0h, 003h, 000h, 000h
        lea    ecx, [ecx + eax - 50a0h]
        jmp    SHORT loc_7_3E2D
        db  08Dh, 049h, 000h
loc_7_3E20:
        xor    ecx, ecx
        mov    cl, al
        db  0A1h, 0ACh, 003h, 000h, 000h
        mov    ecx, dword ptr [eax + ecx*4 - 4]
loc_7_3E2D:
        mov    dword ptr [esp + 1ch], ecx
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [esp + 20h], eax
        mov    ecx, eax
        xor    eax, eax
        mov    ax, word ptr [ecx]
        mov    ecx, dword ptr [esp + 24h]
        add    ecx, eax
        mov    dword ptr [esp + 18h], ecx
        mov    byte ptr [esp + 14h], 1
        jmp    NEAR PTR loc_7_3F64
loc_7_3E54:
        mov    cx, word ptr [esi + 2]
        cmp    cx, 2ah
        jne    SHORT loc_7_3E70
        mov    ecx, ebx
        db  0E8h, 0DBh, 0E1h, 0F8h, 0FFh
        mov    dword ptr [esp + 1ch], eax
        jmp    NEAR PTR loc_7_3F2B
        db  08Bh, 0FFh
loc_7_3E70:
        cmp    cx, 5ch
        jne    NEAR PTR loc_7_4448
        cmp    di, 4
        jbe    NEAR PTR loc_7_4448
        mov    ecx, dword ptr [esp + 24h]
        cmp    word ptr [ecx], 2
        jbe    NEAR PTR loc_7_4448
        push   1
        push   2fh
        sub    eax, 4
        push   eax
        add    esi, 4
        lea    eax, [esp + 40h]
        push   esi
        push   eax
        call   sub_7_0ABA
        add    esp, 14h
        mov    byte ptr [esp + eax + 34h], 0
        sub    eax, eax
        db  066h, 08Bh, 02Dh, 014h, 004h, 000h, 000h
        cmp    bp, ax
        je     SHORT loc_7_3F17
        db  08Bh, 00Dh, 0A0h, 003h, 000h, 000h
loc_7_3EC4:
        mov    dword ptr [esp + 1ch], ecx
        cmp    byte ptr [ecx + 34h], 0
        je     SHORT loc_7_3F04
        cmp    byte ptr [ecx + 5bh], 0
        jne    SHORT loc_7_3F04
        lea    esi, [esp + 34h]
        mov    edi, dword ptr [ecx + 3ch]
loc_7_3EDB:
        mov    dl, byte ptr [esi]
        cmp    dl, byte ptr [edi]
        jne    SHORT loc_7_3EFB
        or     dl, dl
        je     SHORT loc_7_3EF7
        mov    dl, byte ptr [esi + 1]
        cmp    dl, byte ptr [edi + 1]
        jne    SHORT loc_7_3EFB
        add    esi, 2
        add    edi, 2
        or     dl, dl
        jne    SHORT loc_7_3EDB
loc_7_3EF7:
        sub    edx, edx
        jmp    SHORT loc_7_3F00
loc_7_3EFB:
        sbb    edx, edx
        sbb    edx, -1
loc_7_3F00:
        or     edx, edx
        je     SHORT loc_7_3F17
loc_7_3F04:
        add    ecx, 0a0h
        inc    eax
        mov    edx, ebp
        and    edx, 0ffffh
        cmp    edx, eax
        ja     SHORT loc_7_3EC4
loc_7_3F17:
        mov    ecx, ebp
        and    ecx, 0ffffh
        cmp    ecx, eax
        jne    SHORT loc_7_3F2B
        mov    dword ptr [esp + 1ch], 0
loc_7_3F2B:
        cmp    dword ptr [esp + 1ch], 0
        jne    SHORT loc_7_3F45
        mov    word ptr [ebx + 1ah], 35h
        mov    eax, 35h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
loc_7_3F45:
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [esp + 20h], eax
        mov    ecx, eax
        xor    eax, eax
        mov    ax, word ptr [ecx]
        mov    ecx, dword ptr [esp + 24h]
        add    ecx, eax
        mov    dword ptr [esp + 18h], ecx
        test   byte ptr [ebx + 18h], 15h
        je     SHORT loc_7_3FAB
loc_7_3F64:
        db  08Bh, 015h, 0A8h, 005h, 000h, 000h
        or     edx, edx
        je     SHORT loc_7_3FAB
        mov    edi, edi
loc_7_3F70:
        mov    ebp, dword ptr [edx + 0ch]
        cmp    byte ptr [edx + 1fh], 0ffh
        jne    SHORT loc_7_3FA4
        mov    ecx, dword ptr [esp + 1ch]
        cmp    dword ptr [edx + 10h], ecx
        jne    SHORT loc_7_3FA4
        lea    esi, [ebp + 4]
        xor    eax, eax
        mov    ax, word ptr [ebp]
        sub    eax, 4
        mov    ecx, eax
        shr    ecx, 2
        mov    edi, dword ptr [esp + 24h]
        repe cmpsd
        jne    SHORT loc_7_3FA2
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
loc_7_3FA2:
        je     SHORT loc_7_3FD0
loc_7_3FA4:
        mov    edx, dword ptr [edx + 4]
        or     edx, edx
        jne    SHORT loc_7_3F70
loc_7_3FAB:
        cmp    byte ptr [esp + 15h], 0
        jne    NEAR PTR loc_7_424E
        test   byte ptr [ebx + 18h], 10h
        je     SHORT loc_7_4024
        mov    word ptr [ebx + 1ah], 43h
        mov    eax, 43h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
        db  090h
loc_7_3FD0:
        add    ebp, 4
        mov    dword ptr [ebx], 1
        cmp    word ptr [ebp], 0
        je     SHORT loc_7_3FF1
loc_7_3FE0:
        xor    eax, eax
        inc    dword ptr [ebx]
        mov    ax, word ptr [ebp]
        add    ebp, eax
        cmp    word ptr [ebp], 0
        jne    SHORT loc_7_3FE0
loc_7_3FF1:
        cmp    byte ptr [esp + 15h], 0
        je     SHORT loc_7_3FFF
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [edx + 28h], eax
loc_7_3FFF:
        mov    al, byte ptr [edx + 1ch]
        mov    byte ptr [ebx + 4], al
        mov    dword ptr [ebx + 1ch], edx
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ebx + 20h], eax
        sub    eax, eax
        mov    word ptr [ebx + 6], 0
        mov    word ptr [ebx + 1ah], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
loc_7_4024:
        mov    al, byte ptr [ebx + 4]
        cmp    al, 1
        je     SHORT loc_7_4033
        or     al, al
        jne    NEAR PTR loc_7_4227
loc_7_4033:
        lea    eax, [esp + 16h]
        mov    ecx, dword ptr [esp + 18h]
        push   eax
        push   ecx
        mov    ecx, dword ptr [esp + 2ch]
        push   ecx
        push   5
        push   16h
        db  068h, 020h, 015h, 000h, 000h
        mov    ecx, dword ptr [esp + 34h]
        push   ebx
        push   ecx
        call   sub_7_027F
        add    esp, 20h
        or     al, al
        je     SHORT loc_7_4080
        cmp    byte ptr [ebx + 4], 0
        je     NEAR PTR loc_7_43C8
        mov    word ptr [ebx + 1ah], 43h
        mov    eax, 43h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_7_4080:
        mov    esi, dword ptr [esp + 18h]
        test   byte ptr [ebx + 18h], 2
        je     SHORT loc_7_40AD
        mov    ecx, esi
        cmp    word ptr [ecx], 0
        je     SHORT loc_7_40AD
loc_7_4092:
        mov    ecx, dword ptr [esp + 18h]
        xor    eax, eax
        mov    ax, word ptr [ecx]
        mov    dword ptr [esp + 24h], ecx
        add    dword ptr [esp + 18h], eax
        mov    ecx, dword ptr [esp + 18h]
        cmp    word ptr [ecx], 0
        jne    SHORT loc_7_4092
loc_7_40AD:
        db  0A0h, 064h, 004h, 000h, 000h
        cmp    al, 1
        ja     SHORT loc_7_40F0
        or     al, al
        je     SHORT loc_7_40E2
        mov    ecx, dword ptr [esp + 1ch]
        xor    eax, eax
        mov    al, byte ptr [ecx + 75h]
        lea    eax, [eax*4 + 8]
        mov    cl, byte ptr [ecx + 76h]
        lea    eax, [eax + eax*4]
        and    ecx, 0ffh
        lea    eax, [eax + eax*4]
        add    eax, ecx
        cmp    eax, 138h
        jge    SHORT loc_7_40F0
loc_7_40E2:
        mov    al, 0ffh
        jmp    NEAR PTR loc_7_4172
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_7_40F0:
        mov    byte ptr [esp + 13h], 4
        lea    eax, [esp + 12h]
        push   eax
        mov    ecx, dword ptr [esp + 1ch]
        push   0
        mov    eax, dword ptr [esp + 28h]
        push   ecx
        push   eax
        push   0ffh
        push   0
        push   0
        xor    eax, eax
        mov    al, byte ptr [esp + 32h]
        push   eax
        push   4
        push   0ch
        db  068h, 028h, 015h, 000h, 000h
        mov    ecx, dword ptr [esp + 48h]
        push   ebx
        push   ecx
        call   sub_7_027F
        add    esp, 34h
        or     al, al
        je     NEAR PTR loc_7_4222
        mov    byte ptr [esp + 13h], 0
        lea    eax, [esp + 12h]
        push   eax
        mov    ecx, dword ptr [esp + 1ch]
        push   0
        mov    eax, dword ptr [esp + 28h]
        push   ecx
        push   eax
        push   0ffh
        push   0
        push   0
        xor    eax, eax
        mov    al, byte ptr [esp + 32h]
        push   eax
        push   0
        push   0ch
        db  068h, 034h, 015h, 000h, 000h
        mov    ecx, dword ptr [esp + 48h]
        push   ebx
        push   ecx
        call   sub_7_027F
        add    esp, 34h
loc_7_4172:
        or     al, al
        je     NEAR PTR loc_7_4222
        mov    byte ptr [esp + 13h], 0ffh
        lea    eax, [esp + 12h]
        push   eax
        push   1
        push   6
        push   3ah
        mov    eax, dword ptr [esp + 30h]
        push   esi
        push   eax
        push   0
        push   0
        push   12h
        push   16h
        db  068h, 040h, 015h, 000h, 000h
        mov    ecx, dword ptr [esp + 48h]
        push   ebx
        push   ecx
        call   sub_7_027F
        add    esp, 34h
        or     al, al
        je     SHORT loc_7_41C2
        mov    word ptr [ebx + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
loc_7_41C2:
        cmp    dword ptr [esp + 18h], esi
        je     SHORT loc_7_4222
        xor    eax, eax
        mov    al, byte ptr [esp + 12h]
        mov    ecx, dword ptr [esp + 18h]
        push   ecx
        push   esi
        push   eax
        push   eax
        push   0
        push   16h
        db  068h, 04Ch, 015h, 000h, 000h
        mov    ecx, dword ptr [esp + 38h]
        push   ebx
        push   ecx
        call   sub_7_027F
        add    esp, 24h
        or     al, al
        je     SHORT loc_7_4222
        xor    eax, eax
        mov    al, byte ptr [esp + 12h]
        push   eax
        push   14h
        push   16h
        db  068h, 054h, 015h, 000h, 000h
        mov    ecx, dword ptr [esp + 2ch]
        push   ebx
        push   ecx
        call   sub_7_027F
        add    esp, 18h
        mov    word ptr [ebx + 1ah], 43h
        mov    eax, 43h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
loc_7_4222:
        mov    byte ptr [esp + 14h], 1
loc_7_4227:
        cmp    byte ptr [ebx + 4], 2
        jne    SHORT loc_7_424E
        mov    eax, dword ptr [esp + 20h]
        cmp    word ptr [eax + 2], 2ah
        jne    NEAR PTR loc_7_43C8
        mov    byte ptr [esp + 17h], 0ffh
        mov    dword ptr [esp + 28h], 0
loc_7_4249:
        mov    byte ptr [esp + 14h], 2
loc_7_424E:
        mov    eax, dword ptr [esp + 18h]
        sub    eax, dword ptr [esp + 20h]
        add    eax, 4
        mov    dword ptr [esp + 30h], eax
        push   1
        push   34h
        call   sub_7_09BE
        add    esp, 8
        mov    ebp, eax
        mov    eax, dword ptr [esp + 30h]
        push   1
        add    eax, 2
        push   eax
        call   sub_7_09BE
        mov    dword ptr [esp + 34h], eax
        add    esp, 8
        or     ebp, ebp
        je     NEAR PTR loc_7_441F
        or     eax, eax
        je     NEAR PTR loc_7_4410
        lea    eax, [ebp + 14h]
        mov    dword ptr [ebp + 18h], eax
        mov    dword ptr [ebp + 14h], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    dx, word ptr [esp + 30h]
        mov    ecx, dword ptr [esp + 24h]
        mov    word ptr [eax], dx
        sub    dx, word ptr [ecx]
        mov    word ptr [eax + 2], dx
        mov    eax, dword ptr [esp + 30h]
        sub    eax, 4
        mov    edi, dword ptr [esp + 2ch]
        add    edi, 4
        mov    esi, dword ptr [esp + 20h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0A1h, 0A8h, 005h, 000h, 000h
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [ebp], 534552h
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ebp + 10h], ecx
        mov    al, byte ptr [esp + 12h]
        mov    byte ptr [ebp + 1eh], al
        mov    byte ptr [ebp + 1fh], 0ffh
        mov    al, byte ptr [esp + 16h]
        mov    byte ptr [ebp + 20h], al
        mov    al, byte ptr [esp + 13h]
        mov    byte ptr [ebp + 21h], al
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [ebp + 28h], eax
        mov    al, byte ptr [esp + 14h]
        mov    byte ptr [ebp + 1ch], al
        mov    byte ptr [ebp + 1dh], 0
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [ebp + 0ch], eax
        mov    al, byte ptr [esp + 17h]
        mov    byte ptr [ebp + 22h], al
        cmp    byte ptr [esp + 15h], 0
        mov    word ptr [ebp + 24h], 0
        je     SHORT loc_7_4339
        mov    word ptr [ebp + 24h], 4
loc_7_4339:
        mov    word ptr [ebp + 26h], 0
        db  089h, 02Dh, 0A8h, 005h, 000h, 000h
        mov    dword ptr [ebx], 1
        mov    ecx, dword ptr [esp + 18h]
        cmp    ecx, dword ptr [esp + 20h]
        jbe    SHORT loc_7_4371
loc_7_4355:
        mov    eax, dword ptr [esp + 20h]
        inc    dword ptr [ebx]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        add    dword ptr [esp + 20h], eax
        mov    ecx, dword ptr [esp + 18h]
        cmp    ecx, dword ptr [esp + 20h]
        ja     SHORT loc_7_4355
loc_7_4371:
        mov    al, byte ptr [esp + 14h]
        mov    byte ptr [ebx + 4], al
        mov    dword ptr [ebx + 1ch], ebp
        db  0B8h, 0D4h, 013h, 000h, 000h
        cmp    byte ptr [esp + 14h], 1
        je     SHORT loc_7_438C
        db  0B8h, 014h, 014h, 000h, 000h
loc_7_438C:
        mov    dword ptr [ebx + 10h], eax
        mov    word ptr [ebx + 6], 0
        db  080h, 03Dh, 068h, 004h, 000h, 000h, 000h
        je     SHORT loc_7_43B8
        cmp    byte ptr [esp + 13h], 4
        jne    SHORT loc_7_43B8
        mov    ecx, dword ptr [esp + 1ch]
        cmp    dword ptr [ecx + 98h], 0
        jne    SHORT loc_7_43B8
        mov    word ptr [ebx + 6], 1
loc_7_43B8:
        mov    word ptr [ebx + 1ah], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
loc_7_43C8:
        lea    eax, [esp + 28h]
        mov    ecx, dword ptr [esp + 18h]
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   ecx
        push   eax
        push   3
        push   -1
        push   37h
        push   17h
        db  068h, 058h, 015h, 000h, 000h
        mov    ecx, dword ptr [esp + 3ch]
        push   ebx
        push   ecx
        call   sub_7_027F
        add    esp, 28h
        or     al, al
        je     NEAR PTR loc_7_4249
        mov    word ptr [ebx + 1ah], 43h
        mov    eax, 43h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
        db  08Dh, 049h, 000h
loc_7_4410:
        or     ebp, ebp
        je     SHORT loc_7_441F
        push   0
        push   ebp
        call   sub_7_09D1
        add    esp, 8
loc_7_441F:
        cmp    dword ptr [esp + 2ch], 0
        je     SHORT loc_7_4435
        mov    eax, dword ptr [esp + 2ch]
        push   0
        push   eax
        call   sub_7_09D1
        add    esp, 8
loc_7_4435:
        mov    word ptr [ebx + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
loc_7_4448:
        mov    word ptr [ebx + 1ah], 35h
        mov    eax, 35h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_7_4460:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        mov    ebp, dword ptr [ebx + 1ch]
        mov    ecx, dword ptr [ebx + 0ch]
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [esp + 14h], ecx
        cmp    word ptr [ebx + 18h], 2
        jne    SHORT loc_7_44D0
        mov    eax, dword ptr [ebx + 14h]
        or     eax, eax
        je     SHORT loc_7_449C
        mov    ecx, dword ptr [ebx]
        or     ecx, ecx
        je     SHORT loc_7_449C
        dec    ecx
        push   ecx
        db  068h, 060h, 015h, 000h, 000h
        push   eax
        call   sub_7_078A
        add    esp, 0ch
loc_7_449C:
        cmp    byte ptr [ebp + 21h], 4
        je     SHORT loc_7_44B0
        mov    word ptr [ebx + 18h], 0
        mov    dword ptr [ebx], 40000ch
        jmp    SHORT loc_7_44BC
loc_7_44B0:
        mov    word ptr [ebx + 18h], 4002h
        mov    dword ptr [ebx], 0ff00f0h
loc_7_44BC:
        mov    word ptr [ebx + 1ah], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h
loc_7_44D0:
        cmp    dword ptr [esp + 14h], 0
        je     NEAR PTR loc_7_4566
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esp + 14h]
        add    ecx, 4
        mov    dword ptr [esp + 10h], ecx
        mov    edi, dword ptr [edx + 3ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   1
        mov    eax, ecx
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [edx + 3ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        add    eax, 4
        push   eax
        call   sub_7_0AC0
        add    ax, 4
        mov    ecx, dword ptr [esp + 20h]
        add    esp, 10h
        mov    word ptr [ecx], ax
        mov    dx, word ptr [esi]
        mov    ecx, dword ptr [esp + 14h]
        lea    edi, [eax + edx]
        mov    word ptr [ecx], di
        mov    di, word ptr [esi + 2]
        add    di, ax
        mov    word ptr [ecx + 2], di
        mov    ecx, dword ptr [esp + 10h]
        and    eax, 0ffffh
        mov    word ptr [ecx + 2], 5ch
        and    edx, 0ffffh
        sub    edx, 2
        add    esi, 4
        mov    edi, ecx
        add    edi, eax
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_7_4566:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 20h], eax
        mov    al, byte ptr [ebp + 1dh]
        and    eax, 0ffff00ffh
        mov    word ptr [ebx + 18h], ax
        mov    al, byte ptr [ebp + 1ch]
        mov    byte ptr [ebx + 4], al
        mov    word ptr [ebx + 1ah], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_7_4590:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    esi, dword ptr [eax + 1ch]
        db  0B8h, 0A8h, 005h, 000h, 000h
        mov    edi, dword ptr [esi + 10h]
        db  03Bh, 035h, 0A8h, 005h, 000h, 000h
        je     SHORT loc_7_45B4
loc_7_45AB:
        mov    eax, dword ptr [eax]
        add    eax, 4
        cmp    dword ptr [eax], esi
        jne    SHORT loc_7_45AB
loc_7_45B4:
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        cmp    byte ptr [esi + 1ch], 1
        jne    NEAR PTR loc_7_4640
        inc    word ptr [esi + 26h]
        mov    ebp, dword ptr [esi + 14h]
        lea    ebx, [esi + 14h]
        cmp    ebp, ebx
        je     SHORT loc_7_45E3
loc_7_45D1:
        or     dword ptr [ebp + 54h], 2
        mov    ecx, ebp
        db  0E8h, 0F4h, 062h, 0F9h, 0FFh
        mov    ebp, dword ptr [ebp]
        cmp    ebp, ebx
        jne    SHORT loc_7_45D1
loc_7_45E3:
        mov    ax, word ptr [esi + 26h]
        dec    ax
        mov    word ptr [esi + 26h], ax
        jne    SHORT loc_7_45FE
        test   byte ptr [esi + 24h], 1
        je     SHORT loc_7_45FE
        push   ebx
        call   sub_7_5598
        add    esp, 4
loc_7_45FE:
        test   byte ptr [edi + 58h], 10h
        jne    SHORT loc_7_465F
        db  0E8h, 0F7h, 068h, 0F9h, 0FFh
        mov    ebx, eax
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [ebx + 14h], 8
        mov    dword ptr [ebx + 48h], esi
        mov    dword ptr [ebx + 18h], edi
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebx + 8], eax
        inc    word ptr [edi + 92h]
        push   edi
        call   sub_7_4670
        add    esp, 4
        mov    edx, 1
        mov    ecx, ebx
        db  0E8h, 0B5h, 076h, 0F9h, 0FFh
        jmp    SHORT loc_7_465F
        db  08Dh, 049h, 000h
loc_7_4640:
        mov    dword ptr [esi], 736572h
        push   0
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        call   sub_7_09D1
        add    esp, 8
        push   0
        push   esi
        call   sub_7_09D1
        add    esp, 8
loc_7_465F:
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        mov    word ptr [eax + 1ah], 0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_7_4670:
        mov    edx, dword ptr [esp + 4]
        inc    dword ptr [edx + 18h]
        cmp    dword ptr [edx + 1ch], 0
        jne    SHORT loc_7_467E
loc_7_467D:
        ret
loc_7_467E:
        push   esi
        sub    esi, esi
        xchg   dword ptr [edx + 1ch], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        db  0BEh, 0AFh, 048h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    esi
        jmp    SHORT loc_7_467D
sub_7_4698:
        mov    eax, dword ptr [esp + 4]
        dec    dword ptr [eax + 18h]
        ret
sub_7_46A0:
        test   al, 0f8h
        jne    SHORT loc_7_46E2
        db  08Bh, 035h, 03Ch, 01Ah, 000h, 000h
        or     esi, esi
        je     SHORT loc_7_46E2
        cmp    dword ptr [esi + 8], ebx
        je     SHORT loc_7_46C5
        mov    esi, dword ptr [esi + 1ch]
        cmp    dword ptr [esi + 8], ebx
        jne    SHORT loc_7_46E2
        cmp    edi, esi
        je     SHORT loc_7_46E2
        db  089h, 035h, 03Ch, 01Ah, 000h, 000h
loc_7_46C5:
        cmp    dword ptr [esi + 0ch], edi
        jne    SHORT loc_7_46E2
        test   al, 4
        je     SHORT loc_7_46D6
        push   eax
        db  0CDh, 020h, 006h, 000h, 08Bh, 004h
        pop    eax
loc_7_46D6:
        test   al, 2
        mov    eax, dword ptr [esi + 10h]
        jne    NEAR PTR sub_7_556A
        ret
loc_7_46E2:
        db  0CDh, 020h, 069h, 000h, 040h, 000h
        db  0CDh, 020h, 004h, 000h, 08Bh, 004h
        or     esi, esi
        je     SHORT loc_7_46FE
        db  089h, 035h, 03Ch, 01Ah, 000h, 000h
        cmp    dword ptr [esi + 1ch], 0
        je     SHORT loc_7_46FF
loc_7_46FE:
        ret
loc_7_46FF:
        mov    edx, dword ptr [edi + 20h]
        cmp    edx, edi
        je     SHORT loc_7_4715
        cmp    ebx, dword ptr [edx + 8]
        mov    edx, edi
        ja     SHORT loc_7_4715
loc_7_470D:
        mov    edx, dword ptr [edx + 1ch]
        cmp    ebx, dword ptr [edx + 8]
        ja     SHORT loc_7_470D
loc_7_4715:
        mov    ecx, dword ptr [edx + 20h]
        mov    dword ptr [ecx + 1ch], esi
        mov    dword ptr [esi + 1ch], edx
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [edx + 20h], esi
        mov    dword ptr [esi + 28h], esi
        mov    dword ptr [esi + 2ch], esi
        ret
sub_7_472B:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        mov    eax, 0fffh
        mov    ecx, dword ptr [ebp + 24h]
        mov    edx, dword ptr [ebp]
        and    eax, ecx
        add    eax, edx
        cmp    eax, 1000h
        ja     SHORT loc_7_47C1
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 20h]
        mov    ebx, 0fffff000h
        mov    edi, dword ptr [edi + 18h]
        and    ebx, ecx
        add    eax, ebx
        cmp    eax, dword ptr [edi + 5ch]
        ja     SHORT loc_7_47BE
        db  08Ah, 025h, 016h, 00Eh, 000h, 000h
        mov    al, 6
        call   sub_7_46A0
        or     esi, esi
        mov    eax, 1
        je     SHORT loc_7_47BE
        cmp    byte ptr [esi + 14h], 0ffh
        jne    SHORT loc_7_47B9
        mov    ecx, dword ptr [ebp + 24h]
        mov    edx, dword ptr [ebp]
        mov    eax, ecx
        add    eax, edx
        mov    dword ptr [ebp + 24h], eax
        sub    ecx, ebx
        mov    ebx, esi
        mov    esi, dword ptr [esi + 10h]
        mov    edi, dword ptr [ebp + 14h]
        add    esi, ecx
        mov    ecx, edx
        cmp    ecx, 10h
        jbe    SHORT loc_7_47B3
        mov    ecx, 4
        sub    ecx, esi
        and    ecx, 3
        sub    edx, ecx
        rep movsb
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
loc_7_47B3:
        rep movsb
        sub    eax, eax
        mov    esi, ebx
loc_7_47B9:
        call   sub_7_5570
loc_7_47BE:
        pop    edi
        pop    esi
        pop    ebx
loc_7_47C1:
        pop    ebp
        ret
loc_7_47C3:
        pushad
        mov    esi, ecx
loc_7_47C6:
        db  0CDh, 020h, 00Dh, 000h, 08Bh, 004h
        jne    SHORT loc_7_47DD
        db  0E8h, 0CDh, 0B9h, 0F8h, 0FFh
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        popad
        sub    eax, eax
        ret
loc_7_47DD:
        call   sub_7_556A
        inc    byte ptr [esi + 18h]
        push   esi
        db  0CDh, 020h, 012h, 000h, 040h, 000h
        add    esp, 4
        dec    byte ptr [esi + 18h]
        db  068h, 0C6h, 047h, 000h, 000h
        jmp    NEAR PTR sub_7_5570
sub_7_47FC:
        push   ecx
        mov    ecx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_7_480F
        push   dword ptr [eax]
        db  0FFh, 005h, 054h, 01Ah, 000h, 000h
        pop    dword ptr [ecx]
loc_7_480F:
        pop    ecx
        ret
sub_7_4811:
        db  08Dh, 005h, 070h, 01Ah, 000h, 000h
        call   sub_7_47FC
        je     SHORT loc_7_481F
        ret
loc_7_481F:
        db  08Dh, 005h, 060h, 01Ah, 000h, 000h
        db  0FFh, 00Dh, 050h, 01Ah, 000h, 000h
        call   sub_7_47FC
        ret
sub_7_4831:
        db  0CDh, 020h, 091h, 001h, 001h, 000h
        or     eax, eax
        jne    SHORT sub_7_4811
        call   sub_7_486B
loc_7_4840:
        db  08Dh, 005h, 070h, 01Ah, 000h, 000h
        call   sub_7_47FC
        je     SHORT loc_7_484E
        ret
loc_7_484E:
        push   ecx
        push   edx
        db  068h, 064h, 01Ah, 000h, 000h
        db  0E8h, 0DAh, 0B9h, 0F8h, 0FFh
        db  0E8h, 001h, 067h, 0F9h, 0FFh
        db  0E8h, 0EBh, 0B9h, 0F8h, 0FFh
        add    esp, 4
        pop    edx
        pop    ecx
        jmp    SHORT loc_7_4840
sub_7_486B:
        db  083h, 03Dh, 050h, 01Ah, 000h, 000h, 028h
        jb     SHORT loc_7_4875
        ret
loc_7_4875:
        db  068h, 06Bh, 048h, 000h, 000h
        db  068h, 0F6h, 001h, 000h, 000h
        jmp    SHORT loc_7_4840
sub_7_4881:
        mov    eax, dword ptr [esp + 4]
loc_7_4885:
        cmp    word ptr [eax], 0
        jne    SHORT loc_7_488C
        ret
loc_7_488C:
        push   eax
        db  0CDh, 020h, 012h, 000h, 040h, 000h
        pop    eax
        jmp    SHORT loc_7_4885
sub_7_4896:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax], 0
        jne    SHORT loc_7_48A1
        ret
loc_7_48A1:
        mov    word ptr [eax], 0
        push   eax
        db  0CDh, 020h, 013h, 000h, 040h, 000h
        pop    eax
        ret
loc_7_48AF:
        mov    dword ptr [edx + 1ch], 0
        push   edx
        db  0E8h, 004h, 07Bh, 0F9h, 0FFh
        add    esp, 4
        ret
loc_7_48C0:
        push   edx
        db  0E8h, 04Ah, 078h, 0F9h, 0FFh
        add    esp, 4
        ret
        db  052h, 0E8h, 0C6h, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0C3h
sub_7_48D4:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax], 1
        ret
sub_7_48DE:
        bts    dword ptr [esi + 15h], 0
        jb     SHORT loc_7_48E6
        ret
loc_7_48E6:
        inc    byte ptr [esi + 18h]
        push   eax
        push   ecx
        push   edx
        push   esi
        db  0CDh, 020h, 012h, 000h, 040h, 000h
        pop    esi
        dec    byte ptr [esi + 18h]
        pop    edx
        pop    ecx
        pop    eax
        jmp    SHORT sub_7_48DE
loc_7_48FC:
        and    byte ptr [esi + 15h], 0feh
        cmp    byte ptr [esi + 18h], 0
        jne    SHORT loc_7_4907
        ret
loc_7_4907:
        push   eax
        push   ecx
        push   edx
        push   esi
        db  0CDh, 020h, 013h, 000h, 040h, 000h
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        ret
loc_7_4916:
        db  0A0h, 016h, 00Eh, 000h, 000h
        pushf
        cli
        cmp    byte ptr [esi + 33h], al
        jne    SHORT loc_7_4959
        cmp    dword ptr [esi + 28h], 0
        je     SHORT loc_7_4959
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        popf
        mov    edx, esi
        sub    eax, eax
        mov    al, byte ptr [esi + 14h]
loc_7_4938:
        mov    edx, dword ptr [edx + 28h]
        cmp    edx, esi
        je     SHORT loc_7_4946
        add    ecx, dword ptr [edx + 10h]
        jmp    SHORT loc_7_4938
loc_7_4944:
        je     SHORT loc_7_4952
loc_7_4946:
        shr    al, 1
        jae    SHORT loc_7_4944
        add    ecx, 200h
        jmp    SHORT loc_7_4946
loc_7_4952:
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        ret
loc_7_4959:
        popf
        jmp    SHORT loc_7_4952
loc_7_495D:
        db  0A0h, 016h, 00Eh, 000h, 000h
        pushf
        cli
        cmp    byte ptr [esi + 33h], al
        jne    SHORT loc_7_4993
        cmp    dword ptr [esi + 28h], 0
        je     SHORT loc_7_4993
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        popf
        mov    edx, esi
loc_7_497A:
        mov    edx, dword ptr [edx + 28h]
        cmp    edx, esi
        je     SHORT loc_7_498C
        test   byte ptr [edx + 24h], 40h
        jne    SHORT loc_7_497A
        add    ecx, dword ptr [edx + 10h]
        jmp    SHORT loc_7_497A
loc_7_498C:
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
loc_7_4992:
        ret
loc_7_4993:
        popf
        jmp    SHORT loc_7_4992
loc_7_4997:
        db  0BAh, 016h, 049h, 000h, 000h
        jmp    SHORT loc_7_49A3
loc_7_499E:
        db  0BAh, 05Dh, 049h, 000h, 000h
loc_7_49A3:
        db  08Ah, 025h, 016h, 00Eh, 000h, 000h
        sub    ecx, ecx
        db  0CDh, 020h, 009h, 000h, 08Bh, 004h
        mov    eax, ecx
        ret
sub_7_49B4:
        push   ebp
        push   esi
        push   edi
        push   ebx
        call   sub_7_4811
        mov    edi, eax
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, 16h
        rep movsd
        mov    ebp, eax
        mov    dword ptr [ebp + 4ch], 0
        and    byte ptr [ebp + 24h], 0dfh
        mov    dword ptr [ebp + 0ch], ebp
        mov    dword ptr [ebp + 10h], ebp
        call   sub_7_4811
        mov    edi, eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 0ch], edi
        mov    dword ptr [edi + 0ch], ebp
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [ebp + 10h], edi
        mov    dword ptr [edi + 24h], ebp
        sub    eax, eax
        mov    dword ptr [edi + 18h], eax
        mov    byte ptr [edi + 2dh], al
        mov    dword ptr [edi + 1ch], eax
        mov    dword ptr [edi], edi
        mov    dword ptr [edi + 4], edi
        mov    eax, dword ptr [ebp + 48h]
        mov    dword ptr [edi + 14h], eax
        mov    ebx, dword ptr [esp + 14h]
        mov    ebx, dword ptr [ebx + 0ch]
loc_7_4A13:
        mov    esi, dword ptr [ebx]
        cmp    esi, ebx
        je     NEAR PTR loc_7_4AD2
        mov    eax, dword ptr [esi + 10h]
        cmp    eax, dword ptr [esp + 1ch]
        jbe    SHORT loc_7_4A99
        call   sub_7_4811
        mov    edx, eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx], edi
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edx + 20h], edi
        inc    byte ptr [edi + 2dh]
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [edx + 1ch], eax
        mov    ecx, dword ptr [eax + 2ch]
        mov    dword ptr [ecx + 28h], edx
        mov    dword ptr [edx + 28h], eax
        mov    dword ptr [edx + 2ch], ecx
        mov    dword ptr [eax + 2ch], edx
        inc    word ptr [eax + 19h]
        mov    byte ptr [eax + 32h], 1
        xchg   esi, eax
        call   sub_7_556A
        mov    esi, eax
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [edx + 0ch], eax
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [edx + 14h], eax
        mov    dword ptr [edx + 18h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edx + 10h], eax
        mov    dword ptr [edx + 24h], 14h
        add    dword ptr [esi + 14h], eax
        add    dword ptr [esi + 18h], eax
        add    dword ptr [esi + 0ch], eax
        sub    dword ptr [esi + 10h], eax
        add    dword ptr [ebx + 14h], eax
        sub    dword ptr [ebx + 1ch], eax
        add    dword ptr [edi + 1ch], eax
        jmp    SHORT loc_7_4AD2
loc_7_4A99:
        sub    dword ptr [ebx + 1ch], eax
        add    dword ptr [ebx + 14h], eax
        add    dword ptr [edi + 1ch], eax
        mov    ecx, dword ptr [esi]
        mov    edx, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        dec    byte ptr [ebx + 2dh]
        mov    edx, dword ptr [edi + 4]
        mov    dword ptr [edx], esi
        mov    dword ptr [esi], edi
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [edi + 4], esi
        inc    byte ptr [edi + 2dh]
        mov    dword ptr [esi + 20h], edi
        sub    dword ptr [esp + 1ch], eax
        je     SHORT loc_7_4AD2
        dec    dword ptr [esp + 18h]
        jne    NEAR PTR loc_7_4A13
loc_7_4AD2:
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esi + 0ch]
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [esi + 48h], eax
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [esi + 4ch], eax
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [ebp + 4ch], eax
        mov    eax, ebp
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_7_4AF5:
        enter  0ch, 0
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 2ch]
        mov    eax, dword ptr [ebx + 4ch]
        mov    ebx, dword ptr [ebx + 48h]
        add    eax, ebx
        jb     NEAR PTR loc_7_4ECD
        and    ebx, 0fffff000h
        mov    dword ptr [ebp - 4], eax
        lea    eax, [esp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0ch], 0
loc_7_4B27:
        db  08Ah, 025h, 016h, 00Eh, 000h, 000h
        mov    al, 7
        call   sub_7_46A0
        or     esi, esi
        je     NEAR PTR loc_7_4EC8
        push   esi
        call   sub_7_48DE
        add    ebx, 1000h
        cmp    byte ptr [esi + 14h], 0ffh
        adc    dword ptr [ebp - 0ch], 0
        cmp    ebx, dword ptr [ebp - 4]
        jb     SHORT loc_7_4B27
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_7_4ED7
        mov    ecx, dword ptr [ebp + 8]
        push   ebp
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0fffffe00h
        push   eax
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 8]
        mov    ebx, dword ptr [ecx + 48h]
        mov    ebp, dword ptr [ebp + 8]
        mov    ecx, 0fffh
        and    ecx, ebx
        mov    eax, dword ptr [edx]
        shr    ecx, 9
        mov    ch, 1
        shl    ch, cl
        mov    cl, byte ptr [eax + 14h]
        db  068h, 0C3h, 04Bh, 000h, 000h
        jmp    NEAR PTR sub_7_4D3C
loc_7_4B98:
        test   byte ptr [edi + 18h], 2
        je     SHORT loc_7_4BC3
        test   byte ptr [edi + 18h], 48h
        jne    SHORT loc_7_4BC3
        or     byte ptr [edi + 18h], 1
        or     byte ptr [ebp + 24h], 2
        and    byte ptr [ebp + 24h], 0feh
        mov    esi, ebp
loc_7_4BB2:
        mov    esi, dword ptr [esi + 0ch]
        cmp    esi, ebp
        je     SHORT loc_7_4BC3
        test   byte ptr [esi + 18h], 1
        jne    SHORT loc_7_4BB2
        or     byte ptr [ebp + 24h], 1
loc_7_4BC3:
        call   sub_7_4831
        je     NEAR PTR loc_7_4EB2
        mov    edi, eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [edi + 0ch], ebp
        mov    dword ptr [eax + 0ch], edi
        mov    dword ptr [ebp + 10h], edi
        sub    eax, eax
        mov    dword ptr [edi + 18h], eax
        mov    byte ptr [edi + 2dh], al
        mov    dword ptr [edi + 14h], ebx
        mov    dword ptr [edi + 20h], ebx
        mov    dword ptr [edi + 1ch], eax
        mov    dword ptr [edi + 28h], eax
        mov    dword ptr [edi], edi
        mov    dword ptr [edi + 4], edi
        mov    dword ptr [edi + 24h], ebp
        test   cl, ch
        setne  al
        or     byte ptr [edi + 18h], al
        inc    al
        or     byte ptr [ebp + 24h], al
loc_7_4C08:
        test   byte ptr [edi + 18h], 1
        jne    SHORT loc_7_4C24
        mov    al, byte ptr [esp + 4]
        cmp    al, byte ptr [edi + 2dh]
        je     SHORT loc_7_4B98
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [edi + 1ch], eax
        je     NEAR PTR loc_7_4B98
loc_7_4C24:
        call   sub_7_4831
        je     NEAR PTR loc_7_4EB2
        mov    esi, eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi], edi
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 4], esi
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi + 1ch], eax
        xchg   esi, eax
        call   sub_7_556A
        xchg   esi, eax
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esi + 14h], eax
        mov    eax, 0fffh
        and    eax, ebx
        add    eax, dword ptr [esi + 14h]
        mov    dword ptr [esi + 14h], eax
        mov    dword ptr [esi + 18h], eax
        mov    dword ptr [esi + 0ch], ebx
        inc    byte ptr [edi + 2dh]
        sub    eax, eax
        mov    dword ptr [esi + 20h], edi
        mov    dword ptr [esi + 10h], eax
        mov    dword ptr [esi + 24h], eax
        mov    dword ptr [esi + 28h], esi
        mov    dword ptr [esi + 2ch], esi
        nop
loc_7_4C78:
        test   byte ptr [edi + 18h], 1
        mov    eax, 200h
        jne    SHORT loc_7_4CD6
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [edi + 1ch], eax
        je     NEAR PTR loc_7_4B98
        mov    eax, dword ptr [edx]
        cmp    dword ptr [eax + 28h], eax
        je     SHORT loc_7_4CC8
        or     byte ptr [edi + 18h], 2
        push   ecx
        push   edi
        push   edx
        mov    edx, dword ptr [edx]
        push   edx
        sub    edi, edi
loc_7_4CA3:
        mov    edx, dword ptr [edx + 28h]
        cmp    edx, dword ptr [esp]
        je     NEAR PTR loc_7_4D6E
        mov    eax, dword ptr [edx + 0ch]
        mov    ecx, dword ptr [edx + 10h]
        add    ecx, eax
        cmp    ecx, ebx
        jbe    SHORT loc_7_4CA3
        or     edi, edi
        je     SHORT loc_7_4CC4
        cmp    eax, dword ptr [edi + 0ch]
        jae    SHORT loc_7_4CA3
loc_7_4CC4:
        mov    edi, edx
        jmp    SHORT loc_7_4CA3
loc_7_4CC8:
        mov    eax, 200h
        or     byte ptr [edi + 18h], 40h
        jmp    NEAR PTR loc_7_4DBD
loc_7_4CD6:
        test   ebx, 0fffh
        jne    NEAR PTR loc_7_4DD0
        cmp    cl, 0ffh
        jne    NEAR PTR loc_7_4DD0
        push   ebp
        mov    ebp, dword ptr [edx]
        mov    eax, dword ptr [esp + 4]
        cmp    byte ptr [ebp + 16h], 0
        jne    SHORT loc_7_4D31
        sub    eax, ebx
        cmp    eax, 1000h
        jb     SHORT loc_7_4D31
        mov    eax, dword ptr [esp + 0ch]
        sub    eax, dword ptr [edi + 1ch]
        cmp    eax, 1000h
        jb     SHORT loc_7_4D31
        mov    eax, 1000h
        mov    byte ptr [ebp + 16h], 0ffh
        add    ebx, eax
        mov    dword ptr [esi + 10h], eax
        add    dword ptr [edi + 1ch], eax
        sub    ch, ch
        pop    ebp
        cmp    ebx, dword ptr [esp]
        je     NEAR PTR loc_7_4E8A
        jmp    NEAR PTR loc_7_4E2E
loc_7_4D31:
        mov    eax, 200h
        pop    ebp
        jmp    NEAR PTR loc_7_4DD0
sub_7_4D3C:
        test   ch, cl
        jne    SHORT loc_7_4D6D
        test   byte ptr [eax + 16h], ch
        je     SHORT loc_7_4D6A
        cmp    dword ptr [ebp + 50h], ebx
        jb     NEAR PTR loc_7_4E5C
        or     byte ptr [eax + 17h], ch
        db  0CDh, 020h, 069h, 000h, 040h, 000h
        jae    SHORT loc_7_4D65
        push   ecx
        push   edx
        push   eax
        db  0CDh, 020h, 012h, 000h, 040h, 000h
        pop    eax
        pop    edx
        pop    ecx
loc_7_4D65:
        mov    cl, byte ptr [eax + 14h]
        jmp    SHORT sub_7_4D3C
loc_7_4D6A:
        or     byte ptr [eax + 16h], ch
loc_7_4D6D:
        ret
loc_7_4D6E:
        mov    eax, 200h
        or     edi, edi
        je     SHORT loc_7_4D98
        cmp    ebx, dword ptr [edi + 0ch]
        jae    SHORT loc_7_4D83
        mov    eax, dword ptr [edi + 0ch]
        sub    eax, ebx
        jmp    SHORT loc_7_4D98
loc_7_4D83:
        db  08Bh, 015h, 068h, 01Ah, 000h, 000h
        or     byte ptr [esi + 24h], 2
        mov    dword ptr [esi + 14h], edx
        mov    eax, dword ptr [edi + 0ch]
        sub    eax, ebx
        add    eax, dword ptr [edi + 10h]
loc_7_4D98:
        add    esp, 4
        mov    edx, ebx
        or     edx, 1ffh
        inc    edx
        sub    edx, ebx
        cmp    eax, edx
        jbe    SHORT loc_7_4DAC
        mov    eax, edx
loc_7_4DAC:
        pop    edx
        pop    edi
        bt     dword ptr [esi + 24h], 1
        cmc
        sbb    ecx, ecx
        and    ecx, 8
        or     dword ptr [edi + 18h], ecx
        pop    ecx
loc_7_4DBD:
        add    eax, dword ptr [edi + 1ch]
        cmp    eax, dword ptr [esp + 8]
        jbe    SHORT loc_7_4DCA
        mov    eax, dword ptr [esp + 8]
loc_7_4DCA:
        sub    eax, dword ptr [edi + 1ch]
        mov    eax, eax
loc_7_4DD0:
        add    eax, ebx
        sub    eax, dword ptr [esp]
        jb     SHORT loc_7_4DD9
        sub    eax, eax
loc_7_4DD9:
        add    eax, dword ptr [esp]
        sub    eax, ebx
        add    dword ptr [esi + 10h], eax
        add    dword ptr [edi + 1ch], eax
        add    ebx, eax
        cmp    ebx, dword ptr [esp]
        je     NEAR PTR loc_7_4E8A
        test   bx, 1ffh
        jne    NEAR PTR loc_7_4C08
        add    ch, ch
        je     SHORT loc_7_4E2E
        mov    eax, dword ptr [edx]
        call   sub_7_4D3C
        db  068h, 078h, 04Ch, 000h, 000h
        test   byte ptr [edi + 18h], 2
        je     SHORT loc_7_4E17
        db  0C7h, 004h, 024h, 008h, 04Ch, 000h, 000h
loc_7_4E17:
        test   ch, cl
        setne  ah
        test   byte ptr [edi + 18h], 1
        setne  al
        xor    al, ah
        pop    eax
        jne    NEAR PTR loc_7_4B98
        jmp    eax
loc_7_4E2E:
        mov    eax, esi
        mov    esi, dword ptr [esi + 1ch]
        db  068h, 044h, 04Eh, 000h, 000h
        db  068h, 070h, 055h, 000h, 000h
        jmp    NEAR PTR loc_7_48FC
        db  08Bh, 0C0h
loc_7_4E44:
        mov    esi, eax
        sub    edx, 4
        mov    eax, dword ptr [edx]
        inc    ch
        mov    cl, byte ptr [eax + 14h]
        call   sub_7_4D3C
        db  068h, 008h, 04Ch, 000h, 000h
        jmp    SHORT loc_7_4E17
loc_7_4E5C:
        add    esp, 10h
        mov    esi, ebp
        sub    eax, eax
loc_7_4E63:
        mov    esi, dword ptr [esi + 0ch]
        add    eax, dword ptr [esi + 1ch]
        cmp    esi, ebp
        jne    SHORT loc_7_4E63
        sub    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp + 4ch], eax
        pop    ebp
loc_7_4E74:
        pop    esi
        db  068h, 084h, 04Eh, 000h, 000h
        db  068h, 070h, 055h, 000h, 000h
        jmp    NEAR PTR loc_7_48FC
loc_7_4E84:
        cmp    esp, edx
        jbe    SHORT loc_7_4E74
        jmp    SHORT loc_7_4EA0
loc_7_4E8A:
        mov    esi, dword ptr [esi + 1ch]
        db  068h, 09Ch, 04Eh, 000h, 000h
        db  068h, 070h, 055h, 000h, 000h
        jmp    NEAR PTR loc_7_48FC
loc_7_4E9C:
        add    esp, 0ch
        pop    ebp
loc_7_4EA0:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        mov    esp, eax
loc_7_4EA8:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  083h, 0C4h, 004h
loc_7_4EB2:
        add    esp, 0ch
        pop    ebp
loc_7_4EB6:
        pop    esi
        db  068h, 0C8h, 04Eh, 000h, 000h
        db  068h, 070h, 055h, 000h, 000h
        jmp    NEAR PTR loc_7_48FC
        db  08Bh, 0C0h
loc_7_4EC8:
        cmp    esp, dword ptr [ebp - 8]
        jbe    SHORT loc_7_4EB6
loc_7_4ECD:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_7_4ED7:
        mov    ebx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebx + 28h]
        mov    edi, dword ptr [ebx + 14h]
        mov    eax, dword ptr [ebx]
        add    eax, dword ptr [ebx + 24h]
        mov    ecx, dword ptr [ebx + 20h]
        mov    ecx, dword ptr [ecx + 18h]
        neg    eax
        add    eax, dword ptr [ecx + 5ch]
        js     SHORT loc_7_4EF4
        sub    eax, eax
loc_7_4EF4:
        add    eax, dword ptr [ebx]
        add    edi, eax
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebx], eax
        add    eax, dword ptr [ebx + 24h]
loc_7_4F00:
        pop    esi
        db  068h, 010h, 04Fh, 000h, 000h
        db  068h, 070h, 055h, 000h, 000h
        jmp    NEAR PTR loc_7_48FC
loc_7_4F10:
        cmp    dword ptr [esi + 8], eax
        jae    SHORT loc_7_4F00
        mov    edx, dword ptr [ebx + 24h]
        mov    dword ptr [ebx + 24h], eax
        mov    ebx, edx
        sub    edx, dword ptr [esi + 8]
        ja     SHORT loc_7_4F24
        sub    edx, edx
loc_7_4F24:
        dec    eax
        and    eax, 0fffh
        inc    eax
        mov    esi, dword ptr [esi + 10h]
        sub    eax, edx
        add    esi, edx
        sub    edi, eax
        sub    dword ptr [ebp - 4], eax
        push   edi
        mov    ecx, eax
        cmp    ecx, 10h
        jbe    SHORT loc_7_4F59
        mov    ecx, 4
        sub    ecx, esi
        and    ecx, 3
        sub    eax, ecx
        rep movsb
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
loc_7_4F59:
        rep movsb
        pop    edi
loc_7_4F5C:
        sub    dword ptr [ebp - 4], 1000h
        jb     SHORT loc_7_4F90
        pop    esi
        db  068h, 078h, 04Fh, 000h, 000h
        db  068h, 070h, 055h, 000h, 000h
        jmp    NEAR PTR loc_7_48FC
        db  02Eh, 08Bh, 0C0h
loc_7_4F78:
        sub    edi, 1000h
        mov    esi, dword ptr [esi + 10h]
        mov    ecx, 400h
        rep movsd
        sub    edi, 1000h
        jmp    SHORT loc_7_4F5C
loc_7_4F90:
        add    dword ptr [ebp - 4], 1000h
        je     NEAR PTR loc_7_4EA8
        pop    esi
        mov    eax, 0fffh
        db  068h, 0B4h, 04Fh, 000h, 000h
        db  068h, 070h, 055h, 000h, 000h
        jmp    NEAR PTR loc_7_48FC
        db  08Bh, 0C0h
loc_7_4FB4:
        and    eax, ebx
        mov    esi, dword ptr [esi + 10h]
        add    esi, eax
        mov    edx, dword ptr [ebp - 4]
        sub    edi, edx
        mov    ecx, edx
        cmp    ecx, 10h
        jbe    SHORT loc_7_4FE1
        mov    ecx, 4
        sub    ecx, esi
        and    ecx, 3
        sub    edx, ecx
        rep movsb
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
loc_7_4FE1:
        rep movsb
        jmp    NEAR PTR loc_7_4EA8
loc_7_4FE8:
        inc    byte ptr [esi + 18h]
        push   ecx
        push   edx
        push   esi
        db  0CDh, 020h, 012h, 000h, 040h, 000h
        pop    esi
        dec    byte ptr [esi + 18h]
        pop    edx
        pop    ecx
        jmp    SHORT loc_7_503D
_PTEXT ends
        end
