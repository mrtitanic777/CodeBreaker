; vdd_icode_04.asm -- VDD object 4 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_4_0000:
        db  089h, 01Dh, 0F0h, 00Eh, 000h, 000h
        db  0C7h, 005h, 0D2h, 00Eh, 000h, 000h, 024h, 000h, 000h, 000h
        db  0C7h, 005h, 0C8h, 006h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0C4h, 006h, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        push   63ch
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_4_003E
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
loc_4_003E:
        db  0A3h, 0CCh, 006h, 000h, 000h
        mov    edi, ebx
        add    edi, eax
        db  0A1h, 0C8h, 006h, 000h, 000h
        mov    dword ptr [edi + 4], eax
        call   sub_4_012C
        call   sub_4_054C
        call   sub_4_0C20
        jb     NEAR PTR loc_4_0112
        db  0C7h, 005h, 0D2h, 00Eh, 000h, 000h, 010h, 000h, 000h, 000h
        ret
sub_4_006F:
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_4_0079
        ret
loc_4_0079:
        pushad
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        mov    ch, 0
        mov    edx, 0ah
        db  0E8h, 026h, 000h, 0FCh, 0FFh
        call   sub_4_03CC
        jb     SHORT loc_4_0112
        call   sub_4_0628
        jb     SHORT loc_4_0112
        call   sub_4_0C22
        jb     SHORT loc_4_0112
        call   sub_4_0150
        mov    eax, 17001ah
        db  0BEh, 0B5h, 008h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        jb     SHORT loc_4_0112
        call   sub_4_0BC4
        push   0
        db  068h, 0D0h, 00Ch, 000h, 000h
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_0112
        push   0
        db  068h, 0E4h, 00Ch, 000h, 000h
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_0112
        push   0
        db  068h, 0F8h, 00Ch, 000h, 000h
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_0112
        push   0
        db  068h, 00Ch, 00Dh, 000h, 000h
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_0112
        clc
        popad
        ret
loc_4_0112:
        db  00Fh, 0B7h, 00Dh, 00Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        pushad
        mov    esi, ecx
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
        add    byte ptr [eax], al
sub_4_012C:
        db  089h, 01Dh, 0A4h, 007h, 000h, 000h
        db  089h, 01Dh, 0A8h, 007h, 000h, 000h
        db  089h, 01Dh, 0ACh, 007h, 000h, 000h
        db  0B8h, 0BCh, 007h, 000h, 000h
        db  02Bh, 005h, 0CCh, 006h, 000h, 000h
        db  0A3h, 0B8h, 007h, 000h, 000h
        clc
        ret
sub_4_0150:
        db  0E8h, 01Bh, 0FFh, 0FBh, 0FFh
        jb     SHORT loc_4_0175
        db  0E8h, 050h, 01Dh, 0FCh, 0FFh
        call   sub_4_0176
        call   sub_4_01B6
        jb     SHORT loc_4_0175
        call   sub_4_0211
        jb     SHORT loc_4_0175
        db  0E8h, 048h, 04Ah, 0FCh, 0FFh
        clc
loc_4_0175:
        ret
sub_4_0176:
        push   edi
        db  0BFh, 08Ch, 00Dh, 000h, 000h
        db  0BEh, 098h, 00Dh, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        jb     SHORT loc_4_018B
        jne    SHORT loc_4_0193
loc_4_018B:
        movzx  eax, byte ptr [484h]
        inc    eax
loc_4_0193:
        cmp    al, 32h
        jae    SHORT loc_4_01A4
        cmp    al, 2bh
        jb     SHORT loc_4_01AD
        db  0C6h, 005h, 098h, 00Eh, 000h, 000h, 02Bh
        jmp    SHORT loc_4_01B4
loc_4_01A4:
        db  0C6h, 005h, 098h, 00Eh, 000h, 000h, 032h
        jmp    SHORT loc_4_01B4
loc_4_01AD:
        db  0C6h, 005h, 098h, 00Eh, 000h, 000h, 019h
loc_4_01B4:
        pop    edi
        ret
sub_4_01B6:
        mov    dh, 0a0h
        mov    dl, 10h
loc_4_01BA:
        movzx  eax, dh
        db  0E8h, 056h, 03Ch, 0FCh, 0FFh
        inc    dh
        dec    dl
        jne    SHORT loc_4_01BA
        mov    eax, 10h
        call   sub_4_031A
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0b8h], 0
        je     SHORT loc_4_01E7
        call   dword ptr [edi + 0b8h]
loc_4_01E7:
        pop    edi
        mov    esi, dword ptr [edi + 4]
        db  089h, 035h, 0FCh, 00Dh, 000h, 000h
        push   edi
        lea    esi, [edi + 0ch]
        db  0BFh, 004h, 00Eh, 000h, 000h
        mov    ecx, 25h
        cld
        rep movsd
        pop    edi
        db  080h, 00Dh, 0C5h, 006h, 000h, 000h, 001h
        db  0E8h, 035h, 03Ch, 0FCh, 0FFh
        clc
        ret
sub_4_0211:
        call   sub_4_08C4
        jb     NEAR PTR loc_4_0319
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 160eh
        mov    byte ptr [ebp + 10h], 2
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 1202h
        mov    byte ptr [ebp + 10h], 30h
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 3
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 160eh
        mov    byte ptr [ebp + 10h], 1
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0E8h, 028h, 014h, 0FCh, 0FFh
        jb     SHORT loc_4_0319
        imul   eax, ecx
        shl    eax, 1
        add    eax, 0fffh
        shr    eax, 0ch
        cmp    eax, 2
        jae    SHORT loc_4_02B1
        mov    al, 2
loc_4_02B1:
        db  0A2h, 099h, 00Eh, 000h, 000h
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0b4h], 0
        je     SHORT loc_4_02CB
        call   dword ptr [edi + 0b4h]
loc_4_02CB:
        pop    edi
        mov    esi, dword ptr [edi + 4]
        db  089h, 035h, 0C0h, 007h, 000h, 000h
        push   edi
        lea    esi, [edi + 0ch]
        db  0BFh, 0C8h, 007h, 000h, 000h
        mov    ecx, 25h
        cld
        rep movsd
        mov    edi, dword ptr [esp]
        push   ebx
        lea    esi, [edi + 0a0h]
        lea    edx, [edi + 50h]
        db  0BFh, 05Ch, 008h, 000h, 000h
        mov    ecx, 10h
        xor    bl, bl
loc_4_02FF:
        movzx  ebx, byte ptr [edx]
        mov    eax, dword ptr [esi + ebx*4]
        stosd
        inc    edx
        loop   loc_4_02FF
        pop    ebx
        pop    edi
        call   sub_4_0935
        jb     SHORT loc_4_0319
        db  080h, 00Dh, 0C4h, 006h, 000h, 000h, 001h
loc_4_0319:
        ret
sub_4_031A:
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        push   eax
        mov    word ptr [ebp + 1ch], 160eh
        mov    byte ptr [ebp + 10h], 2
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        pop    eax
        mov    word ptr [ebp + 1ch], ax
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 160eh
        mov    byte ptr [ebp + 10h], 1
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
VDD_Get_Special_VM_IDs:
        db  0BEh, 0F8h, 00Dh, 000h, 000h
        db  08Bh, 03Dh, 0B8h, 007h, 000h, 000h
        ret
        db  000h
loc_4_0390:
        cmp    eax, 30740e07h
        cmp    ah, 4fh
        je     SHORT loc_4_03B9
        or     ah, ah
        je     SHORT loc_4_03B0
        cmp    ah, 5
        je     SHORT loc_4_03B0
        cmp    ah, 11h
        db  075h, 01Fh
        cmp    al, 3
        db  074h, 01Bh
        cmp    al, 30h
        db  074h, 017h
loc_4_03B0:
        nop
        pushfd
        lcall  0cf90h, 0
loc_4_03B9:
        nop
        push   eax
        push   ebx
        pushfd
        lcall  9090h, 0
        iretd
        db  090h, 0CFh, 0EAh, 000h, 000h, 000h, 000h
sub_4_03CC:
        mov    eax, 10h
        db  0CDh, 020h, 042h, 000h, 001h, 000h
        shl    ecx, 10h
        mov    cx, dx
        db  089h, 00Dh, 0ACh, 00Eh, 000h, 000h
        push   ecx
        push   0
        push   3ch
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        pop    edx
        or     eax, eax
        jne    SHORT loc_4_03FF
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_4_03FF:
        push   edi
        mov    edi, eax
        db  0BEh, 090h, 003h, 000h, 000h
        mov    ecx, 3ch
        cld
        rep movsb
        mov    edi, eax
        mov    dword ptr [edi + 23h], edx
        mov    dword ptr [edi + 38h], edx
        mov    dword ptr [edi + 2eh], edx
        shr    eax, 4
        movzx  ecx, ax
        mov    edx, edi
        and    edx, 0fh
        mov    eax, 10h
        db  0CDh, 020h, 043h, 000h, 001h, 000h
        mov    eax, ecx
        shl    eax, 10h
        add    eax, edx
        db  0A3h, 0B0h, 00Eh, 000h, 000h
        add    eax, 20h
        xor    edx, edx
        db  0BEh, 053h, 027h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        add    eax, 7
        db  0BEh, 041h, 028h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        db  0A1h, 0B0h, 00Eh, 000h, 000h
        add    eax, 29h
        xor    edx, edx
        db  0BEh, 0F8h, 002h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        add    eax, 9
        db  0BEh, 05Ch, 003h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        db  0A1h, 0B0h, 00Eh, 000h, 000h
        add    eax, 35h
        xor    edx, edx
        db  0BEh, 055h, 005h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        pop    edi
        db  0BEh, 074h, 021h, 000h, 000h
        mov    eax, 10h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        db  0BEh, 0F2h, 020h, 000h, 000h
        mov    eax, 16h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        db  0BEh, 0D9h, 020h, 000h, 000h
        mov    eax, 9
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        mov    eax, 10h
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        db  089h, 00Dh, 0B4h, 00Eh, 000h, 000h
        db  089h, 015h, 0B8h, 00Eh, 000h, 000h
        db  0BEh, 08Fh, 028h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 10h
        mov    eax, 10h
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        db  0BEh, 0ABh, 003h, 000h, 000h
        mov    eax, 2fh
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        mov    eax, 2fh
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        db  089h, 00Dh, 0BCh, 00Eh, 000h, 000h
        db  089h, 015h, 0C0h, 00Eh, 000h, 000h
        db  0BEh, 08Ch, 003h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 10h
        mov    eax, 2fh
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        clc
        ret
        db  000h, 000h, 000h
sub_4_054C:
        push   0
        push   20000h
        push   0a0000h
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        inc    eax
        jne    SHORT loc_4_0566
        jmp    SHORT loc_4_059F
loc_4_0566:
        dec    eax
        db  0A3h, 0C8h, 00Eh, 000h, 000h
        db  0A1h, 0D2h, 00Eh, 000h, 000h
        shl    eax, 3
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_05B7
        db  0A3h, 0D6h, 00Eh, 000h, 000h
        xor    eax, eax
        mov    ecx, 10h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_4_05B7
        db  089h, 035h, 0DAh, 00Eh, 000h, 000h
        ret
loc_4_059F:
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        pushad
        mov    esi, ecx
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_4_05B7:
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
        add    byte ptr [eax], al
        db  000h
VDD_Get_Mini_Dispatch_Table:
        mov    ecx, 31h
        db  0BFh, 0D0h, 006h, 000h, 000h
        ret
VDD_Register_Virtual_Port:
        cmp    edx, 3ceh
        jne    SHORT loc_4_05DC
        db  083h, 00Dh, 09Ch, 010h, 000h, 000h, 001h
        jmp    SHORT loc_4_0608
loc_4_05DC:
        db  0BEh, 0C2h, 006h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_4_0608
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        cmp    cl, 2
        jne    SHORT loc_4_0608
        inc    edx
        db  0BEh, 0C2h, 006h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_4_0608
        db  0CDh, 020h, 099h, 000h, 001h, 000h
loc_4_0608:
        ret
VDD_Takeover_VGA_Port:
        cmp    edx, 3dfh
        ja     SHORT loc_4_0623
        sub    edx, 3c0h
        js     SHORT loc_4_0623
        db  087h, 00Ch, 095h, 018h, 010h, 000h, 000h
        clc
        jmp    SHORT loc_4_0624
loc_4_0623:
        stc
loc_4_0624:
        ret
        db  000h, 000h, 000h
sub_4_0628:
        db  0A1h, 0D2h, 00Eh, 000h, 000h
        db  0E8h, 04Ah, 02Eh, 0FCh, 0FFh
        or     byte ptr [edi + 4], 1
        xor    eax, eax
        xor    esi, esi
        push   edi
        db  0BFh, 078h, 00Dh, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        pop    edi
        or     eax, eax
        je     SHORT loc_4_064F
        or     byte ptr [edi + 4], 80h
loc_4_064F:
        or     eax, 0ffffffffh
        xor    esi, esi
        push   edi
        db  0BFh, 065h, 00Dh, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        pop    edi
        or     eax, eax
        jne    SHORT loc_4_0669
        or     byte ptr [edi + 4], 40h
loc_4_0669:
        or     eax, 0ffffffffh
        xor    esi, esi
        push   edi
        db  0BFh, 0CEh, 00Dh, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        pop    edi
        or     eax, eax
        jne    SHORT loc_4_0683
        or     byte ptr [edi + 4], 2
loc_4_0683:
        db  0CDh, 020h, 000h, 000h, 01Ch, 000h
        jae    NEAR PTR loc_4_0739
        xor    esi, esi
        push   edi
        db  0BFh, 0A6h, 00Dh, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        pop    edi
        jb     SHORT loc_4_06B0
        je     SHORT loc_4_06B0
        or     eax, eax
        je     SHORT loc_4_06FF
loc_4_06A6:
        and    byte ptr [edi + 4], 0fbh
        or     byte ptr [edi + 4], 20h
        jmp    SHORT loc_4_0721
loc_4_06B0:
        mov    dx, 3ceh
        in     al, dx
        mov    ah, al
        mov    al, 6
        out    dx, al
        inc    edx
        in     al, dx
        push   eax
        or     al, 0ch
        out    dx, al
        db  08Bh, 035h, 0C8h, 00Eh, 000h, 000h
        add    esi, 10000h
        mov    ax, word ptr [esi]
        mov    ecx, eax
        xor    cx, 5555h
        mov    word ptr [esi], cx
        xchg   word ptr [esi], ax
        cmp    eax, ecx
        jne    SHORT loc_4_06F5
        add    esi, 2
        mov    ax, word ptr [esi]
        mov    ecx, eax
        xor    cx, 0aaaah
        mov    word ptr [esi], cx
        xchg   word ptr [esi], ax
        cmp    eax, ecx
loc_4_06F5:
        pop    eax
        pushfd
        out    dx, al
        dec    edx
        mov    al, ah
        out    dx, al
        popfd
        je     SHORT loc_4_06A6
loc_4_06FF:
        xor    esi, esi
        push   edi
        or     eax, 0ffffffffh
        db  0BFh, 0B2h, 00Dh, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        pop    edi
        or     byte ptr [edi + 4], 4
        or     eax, eax
        jne    SHORT loc_4_0721
        jmp    SHORT loc_4_0739
        db  0F6h, 047h, 004h, 004h, 074h, 01Ch
loc_4_0721:
        xor    eax, eax
        push   eax
        push   eax
        push   8
        push   0b0h
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_073D
loc_4_0739:
        and    byte ptr [edi + 4], 0dbh
loc_4_073D:
        xor    eax, eax
        push   eax
        db  068h, 020h, 00Dh, 000h, 000h
        push   eax
        db  0CDh, 020h, 074h, 000h, 001h, 000h
        add    esp, 0ch
        db  0A1h, 034h, 00Dh, 000h, 000h
        test   eax, 0ff00ffffh
        je     SHORT loc_4_075D
        jmp    SHORT loc_4_078F
loc_4_075D:
        xor    eax, eax
        push   eax
        push   eax
        push   10h
        push   0a0h
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_0777
        jmp    SHORT loc_4_078F
loc_4_0777:
        xor    eax, eax
        push   eax
        push   eax
        push   8
        push   0b8h
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_07A9
loc_4_078F:
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        pushad
        mov    esi, ecx
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
        stc
        ret
loc_4_07A9:
        push   edi
        xor    esi, esi
        db  0BFh, 0BEh, 00Dh, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        pop    edi
        jb     SHORT loc_4_07C8
        je     SHORT loc_4_07C8
        and    byte ptr [edi + 7], 7fh
        or     eax, eax
        je     SHORT loc_4_07C8
        or     byte ptr [edi + 7], 80h
loc_4_07C8:
        test   byte ptr [edi + 7], 80h
        je     NEAR PTR loc_4_0857
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 1130h
        mov    byte ptr [ebp + 11h], 2
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  ecx, word ptr [ebp + 38h]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        cmp    ch, 0c0h
        jb     SHORT loc_4_0857
        test   cx, 0fffh
        jne    SHORT loc_4_0857
        shr    ecx, 8
        mov    eax, ecx
        shr    eax, 5
        shl    eax, 2
        db  08Bh, 080h, 020h, 00Dh, 000h, 000h
        test   al, 0c0h
        jne    SHORT loc_4_0853
        xor    eax, eax
        add    ecx, 6
        db  089h, 00Dh, 0B4h, 00Fh, 000h, 000h
        push   eax
        push   eax
        push   2
        push   ecx
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_0857
loc_4_0853:
        and    byte ptr [edi + 7], 7fh
loc_4_0857:
        db  0BEh, 0BBh, 03Ah, 000h, 000h
        mov    eax, 0a0h
        mov    ecx, 10h
loc_4_0866:
        db  0CDh, 020h, 071h, 000h, 001h, 000h
        inc    eax
        loop   loc_4_0866
        mov    eax, 0b8h
        mov    ecx, 8
loc_4_0879:
        db  0CDh, 020h, 071h, 000h, 001h, 000h
        inc    eax
        loop   loc_4_0879
        test   byte ptr [edi + 4], 4
        je     SHORT loc_4_089B
        mov    eax, 0b0h
        mov    ecx, 8
loc_4_0892:
        db  0CDh, 020h, 071h, 000h, 001h, 000h
        inc    eax
        loop   loc_4_0892
loc_4_089B:
        mov    eax, dword ptr [edi + 4]
        db  0A3h, 0C8h, 006h, 000h, 000h
        db  0E8h, 090h, 0F9h, 0FBh, 0FFh
        db  0E8h, 05Ch, 02Ch, 0FCh, 0FFh
        ret
loc_4_08AE:
        db  0C7h, 005h, 0B8h, 00Fh, 000h, 000h, 001h, 001h, 000h, 000h
        call   sub_4_0CC4
        jmp    NEAR PTR loc_4_0B83
        db  000h, 000h
sub_4_08C4:
        mov    dx, 0a010h
loc_4_08C8:
        movzx  eax, dh
        db  0E8h, 048h, 035h, 0FCh, 0FFh
        inc    dh
        dec    dl
        jne    SHORT loc_4_08C8
        xor    eax, eax
        push   9
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   1
        push   2
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_4_0911
        db  0A3h, 044h, 00Dh, 000h, 000h
        db  089h, 015h, 048h, 00Dh, 000h, 000h
        push   0
        push   0
        push   2
        push   0a0h
        push   ebx
        push   eax
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
loc_4_0911:
        mov    dh, 0b8h
        mov    dl, 8
        test   byte ptr [edi + 4], 4
        je     SHORT loc_4_091F
        mov    dh, 0b0h
        mov    dl, 10h
loc_4_091F:
        movzx  eax, dh
        db  0E8h, 0F1h, 034h, 0FCh, 0FFh
        inc    dh
        dec    dl
        jne    SHORT loc_4_091F
        clc
        ret
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
sub_4_0935:
        movzx  eax, byte ptr [484h]
        inc    eax
        db  0A3h, 0E4h, 00Eh, 000h, 000h
        movzx  eax, byte ptr [44ah]
        db  0A3h, 0E8h, 00Eh, 000h, 000h
        movzx  ecx, byte ptr [edi + 15h]
        and    cl, 1fh
        inc    ecx
        db  088h, 00Dh, 0D0h, 00Fh, 000h, 000h
        shl    ecx, 8
        db  089h, 00Dh, 04Ch, 00Dh, 000h, 000h
        db  083h, 03Dh, 044h, 00Dh, 000h, 000h, 000h
        je     SHORT loc_4_0995
        push   edi
        xor    esi, esi
        db  0BFh, 0F0h, 00Dh, 000h, 000h
        or     eax, 0ffffffffh
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        pop    edi
        jbe    SHORT loc_4_0988
        or     al, al
        je     SHORT loc_4_0988
        mov    al, 1
loc_4_0988:
        db  0A2h, 050h, 00Dh, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_4_0AE6
loc_4_0995:
        push   edi
        db  00Fh, 0B6h, 00Dh, 0D0h, 00Fh, 000h, 000h
        push   ecx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    al, 6
        mov    cl, 10h
        db  038h, 00Dh, 0D0h, 00Fh, 000h, 000h
        jae    SHORT loc_4_09D0
        mov    cl, 8
        mov    al, 3
        db  080h, 03Dh, 0D0h, 00Fh, 000h, 000h, 00Eh
        jb     SHORT loc_4_09D0
        mov    al, 2
        mov    cl, 0eh
loc_4_09D0:
        db  086h, 00Dh, 0D0h, 00Fh, 000h, 000h
        db  038h, 00Dh, 0D0h, 00Fh, 000h, 000h
        jbe    SHORT loc_4_09EB
        db  083h, 03Dh, 044h, 00Dh, 000h, 000h, 000h
        jne    NEAR PTR loc_4_0AC9
loc_4_09EB:
        push   eax
        mov    word ptr [ebp + 1ch], 160eh
        mov    byte ptr [ebp + 10h], 2
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        pop    eax
        mov    byte ptr [ebp + 11h], al
        mov    word ptr [ebp + 1ch], 1130h
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  esi, word ptr [ebp + 38h]
        movzx  ecx, word ptr [ebp + 8]
        shl    esi, 4
        add    esi, ecx
        mov    word ptr [ebp + 1ch], 160eh
        mov    byte ptr [ebp + 10h], 1
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  080h, 03Dh, 050h, 00Dh, 000h, 000h, 001h
        je     SHORT loc_4_0A79
        xor    eax, eax
        push   eax
        db  068h, 020h, 00Dh, 000h, 000h
        push   eax
        db  0CDh, 020h, 074h, 000h, 001h, 000h
        add    esp, 0ch
        mov    ecx, esi
        shr    ecx, 0ch
        db  00Fh, 0A3h, 00Dh, 020h, 00Dh, 000h, 000h
        jae    SHORT loc_4_0A79
        pop    ecx
        jmp    SHORT loc_4_0A90
loc_4_0A79:
        db  08Bh, 00Dh, 04Ch, 00Dh, 000h, 000h
        push   0
        push   ecx
        push   esi
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        inc    eax
        jne    SHORT loc_4_0AA5
loc_4_0A90:
        pop    edi
        db  088h, 00Dh, 0D0h, 00Fh, 000h, 000h
        db  083h, 03Dh, 044h, 00Dh, 000h, 000h, 000h
        jne    SHORT loc_4_0AE6
        jmp    NEAR PTR loc_4_0B7D
loc_4_0AA5:
        dec    eax
        db  0A3h, 0CCh, 00Fh, 000h, 000h
        db  0A1h, 044h, 00Dh, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_4_0B73
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        jmp    NEAR PTR loc_4_0B73
loc_4_0AC9:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ecx
        db  088h, 00Dh, 0D0h, 00Fh, 000h, 000h
        pop    edi
loc_4_0AE6:
        db  08Bh, 00Dh, 04Ch, 00Dh, 000h, 000h
        add    ecx, 0fffh
        shr    ecx, 0ch
        cmp    ecx, 2
        jb     SHORT loc_4_0B01
        db  0C6h, 005h, 0D0h, 00Fh, 000h, 000h, 020h
loc_4_0B01:
        xor    eax, eax
        push   80h
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   1
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_4_0B7D
        db  0A3h, 0C8h, 00Fh, 000h, 000h
        db  089h, 015h, 0CCh, 00Fh, 000h, 000h
        push   edi
        db  00Fh, 0B6h, 005h, 0D0h, 00Fh, 000h, 000h
        mov    ecx, 100h
        db  08Bh, 035h, 048h, 00Dh, 000h, 000h
        mov    edi, edx
        cld
loc_4_0B3E:
        push   ecx
        push   esi
        mov    ecx, eax
        rep movsb
        pop    esi
        add    esi, 20h
        pop    ecx
        loop   loc_4_0B3E
        mov    edi, dword ptr [esp]
        mov    eax, 0a0h
        db  0E8h, 0C0h, 032h, 0FCh, 0FFh
        mov    eax, 0a1h
        db  0E8h, 0B6h, 032h, 0FCh, 0FFh
        push   0
        db  0FFh, 035h, 044h, 00Dh, 000h, 000h
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_4_0B73:
        clc
        pop    edi
        pushfd
        db  0E8h, 0C9h, 032h, 0FCh, 0FFh
        popfd
        ret
loc_4_0B7D:
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
loc_4_0B83:
        pushad
        db  0A1h, 0C8h, 00Fh, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_0BAE
        db  08Bh, 00Dh, 04Ch, 00Dh, 000h, 000h
        add    ecx, 0fffh
        shr    ecx, 0ch
        push   100h
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_4_0BAE:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        mov    ecx, 4
        db  0E8h, 059h, 0F4h, 0FBh, 0FFh
        popad
        clc
        ret
        db  000h
sub_4_0BC4:
        push   edi
        mov    eax, 32h
        db  0BFh, 054h, 00Dh, 000h, 000h
        xor    esi, esi
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        db  0A3h, 0D4h, 00Fh, 000h, 000h
        add    eax, eax
        db  0A3h, 0D8h, 00Fh, 000h, 000h
        pop    edi
        push   edi
        mov    eax, 2
        db  0BFh, 0E0h, 00Dh, 000h, 000h
        xor    esi, esi
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        or     eax, eax
        jne    SHORT loc_4_0C00
        mov    eax, 2
loc_4_0C00:
        db  0A3h, 0DCh, 00Fh, 000h, 000h
        pop    edi
        db  0BEh, 070h, 046h, 000h, 000h
        mov    eax, 10038h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 0E0h, 00Fh, 000h, 000h
        ret
        db  000h, 000h, 000h
sub_4_0C20:
        clc
        ret
sub_4_0C22:
        pushad
        push   edi
        or     byte ptr [edi + 5e2h], 0eh
        bts    dword ptr [edi], 10h
        mov    dword ptr [edi + 4a7h], 0ffffffffh
        db  0BFh, 010h, 00Eh, 000h, 000h
        db  0CDh, 020h, 095h, 000h, 001h, 000h
        jb     SHORT loc_4_0CC1
        mov    edi, dword ptr [esp]
        test   byte ptr [edi + 4], 20h
        jne    SHORT loc_4_0C80
        db  0CDh, 020h, 000h, 000h, 01Ch, 000h
        jae    SHORT loc_4_0C80
        db  0BFh, 0FCh, 00Dh, 000h, 000h
        db  0CDh, 020h, 095h, 000h, 001h, 000h
        mov    edi, dword ptr [esp]
        jae    SHORT loc_4_0C8D
        btr    dword ptr [edi + 4], 2
        jae    SHORT loc_4_0C80
        db  080h, 025h, 0C8h, 006h, 000h, 000h, 0FBh
        or     byte ptr [edi + 4], 20h
        db  080h, 00Dh, 0C8h, 006h, 000h, 000h, 020h
loc_4_0C80:
        and    byte ptr [edi + 4], 0f7h
        db  080h, 025h, 0C8h, 006h, 000h, 000h, 0F7h
        jmp    SHORT loc_4_0CC0
loc_4_0C8D:
        db  080h, 00Dh, 0C8h, 006h, 000h, 000h, 008h
        or     byte ptr [edi + 4], 8
        bts    dword ptr [edi], 17h
        test   byte ptr [edi + 4], 4
        jne    SHORT loc_4_0CC0
        mov    edx, 3b4h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        inc    edx
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    dl, 0bah
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        btr    dword ptr [edi], 17h
loc_4_0CC0:
        clc
loc_4_0CC1:
        pop    edi
        popad
        ret
sub_4_0CC4:
        db  066h, 083h, 00Dh, 01Bh, 00Bh, 000h, 000h, 040h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 049h, 004h, 000h, 000h, 01Eh
        db  000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 084h
        db  004h, 000h, 000h, 007h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 0A8h, 004h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 004h, 000h, 000h, 002h, 000h, 000h, 000h, 000h
        db  002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_0D54:
        push   edi
        imul   ebp, dword ptr [esi + 64h], 7055776fh
        popad
        db  074h, 065h
        push   esp
        db  069h, 06Dh, 065h, 000h
loc_4_0D65:
        push   esi
        imul   esp, dword ptr [ebp + 6fh], 6b636142h
        db  067h, 072h, 06Fh
        db  075h, 06Eh
        dec    ebp
        db  073h, 067h
        db  000h
loc_4_0D78:
        push   esi
        imul   esp, dword ptr [ebp + 6fh], 70737553h
        outsb
        inc    esp
        imul   esi, dword ptr [ebx + 61h], 656c62h
loc_4_0D8C:
        push   ebx
        arpl   word ptr [edx + 65h], si
        outsb
        dec    esp
        imul   ebp, dword ptr [esi + 65h], 6f4e0073h
        outsb
        push   edi
        imul   ebp, dword ptr [esi + 64h], 4173776fh
        jo     SHORT loc_4_0E15
        add    byte ptr [ebp + esi*2 + 61h], al
        insb
        inc    esp
        imul   esi, dword ptr [ebx + 70h], 79616ch
loc_4_0DB2:
        push   esi
        inc    edi
        inc    ecx
        dec    ebp
        outsd
        outsb
        outsd
        push   esp
        db  065h, 078h, 074h
        db  000h
loc_4_0DBE:
        push   edx
        jae    NEAR PTR loc_4_0E27
        jb     SHORT loc_4_0E3A
        push   esi
        imul   esp, dword ptr [ebp + 6fh], 4d4f52h
loc_4_0DCE:
        inc    ecx
        db  075h, 074h
        outsd
        push   edx
        jae    NEAR PTR loc_4_0E4A
        outsd
        jb     SHORT loc_4_0E3E
        push   ebx
        arpl   word ptr [edx + 65h], si
        outsb
        add    byte ptr [ebx + 63h], dl
        jb     SHORT loc_4_0E53
        insb
        insb
        inc    esi
        jb     SHORT loc_4_0E4E
        db  071h, 075h
        outsb
        arpl   word ptr [ecx], di
loc_4_0DF0:
        push   ebp
        jae    SHORT loc_4_0E58
        push   edx
        dec    edi
        dec    ebp
        outsw
        outsb
        je     SHORT loc_4_0DFB
loc_4_0DFB:
        add    byte ptr [ebx], al
        db  000h, 0BAh, 003h, 0B4h, 04Ch, 000h
        db  000h, 0B4h, 003h, 0B4h, 04Ch, 000h, 000h
        mov    ch, 3
        mov    ah, 4ch
        add    byte ptr [eax], al
loc_4_0E10:
        and    byte ptr [eax], al
        db  0C0h, 003h, 0B4h
loc_4_0E15:
        dec    esp
        add    byte ptr [eax], al
        db  0C1h, 003h, 0B4h
        dec    esp
        add    byte ptr [eax], al
        db  0C2h, 003h, 0B4h
        db  04Ch, 000h, 000h, 0C3h, 003h, 0B4h
loc_4_0E27:
        dec    esp
        add    byte ptr [eax], al
        les    eax, ptr [ebx]
        mov    ah, 4ch
        add    byte ptr [eax], al
        lds    eax, ptr [ebx]
        mov    ah, 4ch
        add    byte ptr [eax], al
        db  0C6h, 003h, 0B4h
        dec    esp
loc_4_0E3A:
        add    byte ptr [eax], al
        db  0C7h, 003h
loc_4_0E3E:
        mov    ah, 4ch
        add    byte ptr [eax], al
        db  0C8h, 003h, 0B4h, 04Ch
        add    byte ptr [eax], al
        leave
        db  003h
loc_4_0E4A:
        mov    ah, 4ch
        add    byte ptr [eax], al
loc_4_0E4E:
        db  0CAh, 003h, 0B4h
        db  04Ch, 000h
loc_4_0E53:
        add    bl, cl
        db  003h, 0B4h, 04Ch
loc_4_0E58:
        add    byte ptr [eax], al
        int3
        db  003h, 0B4h, 04Ch, 000h, 000h, 0CDh, 003h
        mov    ah, 4ch
        add    byte ptr [eax], al
        into
        db  003h, 0B4h, 04Ch, 000h, 000h, 0CFh, 003h
        mov    ah, 4ch
        add    byte ptr [eax], al
        rol    byte ptr [ebx], 1
        mov    ah, 4ch
        add    byte ptr [eax], al
        rol    dword ptr [ebx], 1
        mov    ah, 4ch
        add    byte ptr [eax], al
        rol    byte ptr [ebx], cl
        mov    ah, 4ch
        add    byte ptr [eax], al
        rol    dword ptr [ebx], cl
        mov    ah, 4ch
        add    byte ptr [eax], al
        aam    3
        mov    ah, 4ch
        add    byte ptr [eax], al
        aad    3
        mov    ah, 4ch
        add    byte ptr [eax], al
        salc
        db  003h, 0B4h, 04Ch, 000h, 000h, 0D7h, 003h
        mov    ah, 4ch
        add    byte ptr [eax], al
        fadd   dword ptr [ebx]
        mov    ah, 4ch
        add    byte ptr [eax], al
        fld    dword ptr [ebx]
        mov    ah, 4ch
        add    byte ptr [eax], al
        fiadd  dword ptr [ebx]
        mov    ah, 4ch
        add    byte ptr [eax], al
        fild   dword ptr [ebx]
        mov    ah, 4ch
        add    byte ptr [eax], al
        fadd   qword ptr [ebx]
        mov    ah, 4ch
        add    byte ptr [eax], al
        fld    qword ptr [ebx]
        mov    ah, 4ch
        add    byte ptr [eax], al
        fiadd  word ptr [ebx]
        mov    ah, 4ch
        add    byte ptr [eax], al
        fild   word ptr [ebx]
        mov    ah, 4ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_ITEXT ends
        end
