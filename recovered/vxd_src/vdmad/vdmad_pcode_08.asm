; vdmad_pcode_08.asm -- VDMAD object 8 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_8_0000:
        db  066h, 00Fh, 0BAh, 02Dh, 00Dh, 000h, 000h, 000h, 001h
        jae    SHORT loc_8_0011
        mov    eax, 13h
        ret
loc_8_0011:
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        push   0
        push   0
        push   ebx
        db  0CDh, 020h, 00Fh, 000h, 033h, 000h
        add    esp, 0ch
        push   0
        push   0
        push   0
        push   ebx
        db  0CDh, 020h, 00Eh, 000h, 033h, 000h
        add    esp, 10h
        mov    eax, 0
        ret
loc_8_003B:
        push   ebx
        push   esi
        push   edi
        mov    eax, 1
        mov    esi, dword ptr [esp + 10h]
        cmp    esi, 7
        ja     SHORT loc_8_005A
        mov    ecx, dword ptr [esp + 1ch]
        db  0FFh, 014h, 0B5h, 010h, 000h, 000h, 000h
loc_8_005A:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_8_005E:
        mov    ebx, dword ptr [ecx]
        call   sub_8_02F4
        db  0A0h, 008h, 000h, 000h, 000h
        db  0A2h, 009h, 000h, 000h, 000h
        db  0A0h, 00Bh, 000h, 000h, 000h
        db  0A2h, 00Ch, 000h, 000h, 000h
        call   sub_8_02F5
        mov    edx, dword ptr [ebx + 0ch]
        call   sub_8_0144
        clc
        je     SHORT loc_8_00A1
        push   -1
        call   sub_8_00C0
        lea    esp, [esp + 4]
        jae    SHORT loc_8_00A1
        push   0
        call   sub_8_00C0
        lea    esp, [esp + 4]
loc_8_00A1:
        db  0C7h, 005h, 02Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jae    SHORT loc_8_00B7
        db  0C7h, 005h, 02Ch, 000h, 000h, 000h, 013h, 000h, 000h, 000h
loc_8_00B7:
        db  0A1h, 02Ch, 000h, 000h, 000h
        ret
        db  02Eh, 08Bh, 0C0h
sub_8_00C0:
        enter  8, 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, 0ffffffffh
        mov    dword ptr [ebp - 8], eax
loc_8_00D2:
        mov    eax, 80h
loc_8_00D7:
        test   byte ptr [edi + 2], al
        je     SHORT loc_8_00EB
        call   sub_8_01A0
        jb     SHORT loc_8_00EB
        db  084h, 005h, 009h, 000h, 000h, 000h
        je     SHORT loc_8_0109
loc_8_00EB:
        shr    al, 1
        jne    SHORT loc_8_00D7
        test   dword ptr [ebp + 8], 0ffffffffh
        jne    SHORT loc_8_0104
        xor    dword ptr [ebp - 4], 0ffffffffh
        je     SHORT loc_8_00D2
        xor    dword ptr [ebp - 8], 0ffffffffh
        je     SHORT loc_8_00D2
loc_8_0104:
        leave
        stc
        ret
        db  0EBh, 0E2h
loc_8_0109:
        db  08Ah, 025h, 009h, 000h, 000h, 000h
        db  008h, 005h, 009h, 000h, 000h, 000h
        pushad
        call   sub_8_0144
        je     SHORT loc_8_0134
        push   dword ptr [ebp - 8]
        call   sub_8_00C0
        lea    esp, [esp + 4]
        jae    SHORT loc_8_0134
        popad
        db  088h, 025h, 009h, 000h, 000h, 000h
        jmp    SHORT loc_8_00EB
loc_8_0134:
        popad
        add    esp, 4
        bsf    eax, eax
        mov    byte ptr [edi + 1], al
        leave
        clc
        ret
        db  02Eh, 08Bh, 0C0h
sub_8_0144:
        push   0
        push   0
        push   3
        push   edx
        db  068h, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        cmp    eax, 0
        jne    SHORT loc_8_0171
        db  08Bh, 03Dh, 004h, 000h, 000h, 000h
        mov    edx, edi
        mov    al, byte ptr [edi + 2]
        call   sub_8_017C
        or     edi, edi
loc_8_0170:
        ret
loc_8_0171:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     SHORT loc_8_0170
        mov    edx, dword ptr [ebx + 0ch]
        jmp    SHORT sub_8_0144
sub_8_017C:
        push   eax
        mov    ah, 0aah
        and    ah, al
        xor    al, ah
        add    ah, ah
        adc    al, ah
        mov    ah, 0cch
        and    ah, al
        xor    al, ah
        shr    ah, 2
        add    al, ah
        mov    ah, al
        shr    ah, 4
        and    al, 0fh
        add    al, ah
        movzx  ecx, al
        pop    eax
        ret
sub_8_01A0:
        cmp    ecx, 1
        jbe    SHORT loc_8_01BD
        db  084h, 005h, 01Bh, 021h, 000h, 000h
        je     SHORT loc_8_01B5
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_8_01BB
loc_8_01B3:
        stc
        ret
loc_8_01B5:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_8_01B3
loc_8_01BB:
        clc
        ret
loc_8_01BD:
        db  084h, 005h, 01Bh, 021h, 000h, 000h
        je     SHORT loc_8_01C7
        clc
        ret
loc_8_01C7:
        db  008h, 005h, 01Bh, 021h, 000h, 000h
        db  080h, 00Dh, 00Dh, 000h, 000h, 000h, 001h
        clc
        ret
sub_8_01D6:
        cmp    eax, 8
        jae    SHORT loc_8_01EB
        db  00Fh, 0ABh, 005h, 008h, 000h, 000h, 000h
        jb     SHORT loc_8_01EB
        db  00Fh, 0ABh, 005h, 00Bh, 000h, 000h, 000h
loc_8_01EB:
        ret
sub_8_01EC:
        cmp    eax, 8
        jae    SHORT loc_8_0201
        db  00Fh, 0B3h, 005h, 00Bh, 000h, 000h, 000h
        jae    SHORT loc_8_0201
        db  00Fh, 0B3h, 005h, 008h, 000h, 000h, 000h
loc_8_0201:
        ret
loc_8_0202:
        db  066h, 00Fh, 0BAh, 035h, 00Dh, 000h, 000h, 000h, 002h
loc_8_020B:
        mov    ebx, dword ptr [ecx]
        db  0A0h, 008h, 000h, 000h, 000h
        db  0A2h, 009h, 000h, 000h, 000h
        db  0A0h, 00Bh, 000h, 000h, 000h
        db  0A2h, 00Ch, 000h, 000h, 000h
        call   sub_8_02F5
        cmp    eax, 0
        jne    SHORT loc_8_0263
        mov    edx, dword ptr [ebx + 0ch]
loc_8_022E:
        call   sub_8_0144
        je     SHORT loc_8_025E
        movzx  eax, byte ptr [edi + 1]
        cmp    eax, 7
        ja     SHORT loc_8_0269
        db  00Fh, 0ABh, 005h, 01Bh, 021h, 000h, 000h
        db  00Fh, 0A3h, 005h, 009h, 000h, 000h, 000h
        jb     SHORT loc_8_0269
loc_8_024E:
        db  00Fh, 0ABh, 005h, 009h, 000h, 000h, 000h
        db  00Fh, 0B3h, 005h, 00Ch, 000h, 000h, 000h
        jmp    SHORT loc_8_022E
loc_8_025E:
        mov    eax, 0
loc_8_0263:
        db  0A3h, 02Ch, 000h, 000h, 000h
        ret
loc_8_0269:
        db  00Fh, 0A3h, 005h, 00Ch, 000h, 000h, 000h
        jb     SHORT loc_8_024E
        db  066h, 00Fh, 0BAh, 025h, 00Dh, 000h, 000h, 000h, 002h
        jb     SHORT loc_8_024E
        mov    eax, 13h
        jmp    SHORT loc_8_0263
loc_8_0284:
        db  066h, 00Fh, 0BAh, 02Dh, 00Dh, 000h, 000h, 000h, 002h
        jmp    NEAR PTR loc_8_020B
loc_8_0292:
        db  083h, 03Dh, 02Ch, 000h, 000h, 000h, 000h
        mov    eax, 13h
        db  0A3h, 02Ch, 000h, 000h, 000h
        jne    SHORT loc_8_02BE
        db  0A0h, 009h, 000h, 000h, 000h
        db  0A2h, 008h, 000h, 000h, 000h
        db  0A0h, 00Ch, 000h, 000h, 000h
        db  0A2h, 00Bh, 000h, 000h, 000h
        mov    eax, 0
loc_8_02BE:
        ret
loc_8_02BF:
        db  0C7h, 005h, 02Ch, 000h, 000h, 000h, 013h, 000h, 000h, 000h
        mov    eax, 0
        ret
loc_8_02CF:
        db  0A0h, 008h, 000h, 000h, 000h
        not    al
        db  0A2h, 00Ah, 000h, 000h, 000h
        db  0C7h, 001h, 00Ah, 000h, 000h, 000h
        mov    dword ptr [ecx + 4], 1
        mov    eax, 0
        ret
loc_8_02EE:
        mov    eax, 0
        ret
sub_8_02F4:
        ret
sub_8_02F5:
        pushad
loc_8_02F6:
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        je     SHORT loc_8_0345
        push   2
        push   eax
        db  068h, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 01Ah, 000h, 033h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    SHORT loc_8_0345
loc_8_0313:
        db  0A1h, 004h, 000h, 000h, 000h
        push   0
        push   0
        push   3
        push   eax
        db  068h, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        cmp    eax, 0
        jne    SHORT loc_8_0345
        db  08Bh, 03Dh, 004h, 000h, 000h, 000h
        movzx  eax, byte ptr [edi + 1]
        db  00Fh, 0B3h, 005h, 009h, 000h, 000h, 000h
        jmp    SHORT loc_8_0313
loc_8_0345:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_8_02F6
        popad
        mov    eax, 0
        ret
_PTEXT ends
        end
