; azt16_lcode_01.asm -- AZT16 object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  083h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  085h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  084h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  086h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
loc_1_0040:
        sal    byte ptr [ebx], 42h
        mov    esp, cs
        mov    ah, 0ceh
        adc    dword ptr [edx + 53454494h], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0050:
        sal    dword ptr [ebx], 42h
        mov    esp, cs
        mov    ah, 0ceh
        adc    dword ptr [edx + 53454494h], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0060:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 05Fh, 004h, 004h, 000h, 000h, 000h, 061h, 07Ah, 074h, 031h
        db  036h, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00DC:
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], ah
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0115:
        add    byte ptr [eax], al
        add    byte ptr [esp + ecx], cl
        or     al, 0ch
        or     byte ptr [eax], cl
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax - 4], al
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h
loc_1_0136:
        add    dword ptr [edi], ecx
        add    byte ptr [esi], cl
        add    eax, dword ptr [edx]
        add    eax, 0d060407h
        or     dword ptr [ebx], ecx
        db  00Ch
loc_1_0144:
        mov    cl, 17h
        add    byte ptr [eax], al
        mov    ds, word ptr [eax + eax]
        add    byte ptr [eax + 22h], ah
        add    byte ptr [eax], al
        dec    eax
        sub    byte ptr [eax], al
        add    al, cl
        xor    al, 0
        add    byte ptr [edx], ch
        inc    esp
        add    byte ptr [eax], al
        sti
        dec    edi
        add    byte ptr [eax], al
        lahf
        pushad
        add    byte ptr [eax], al
        push   cs
        je     SHORT loc_1_0167
loc_1_0167:
        add    byte ptr [ecx], bl
        jg     SHORT loc_1_016B
loc_1_016B:
        add    byte ptr [ebp - 76h], ch
        add    byte ptr [eax], al
        neg    byte ptr [edi - 4c1e0000h]
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_017C:
        xchg   byte ptr [19dc0000h], dl
        add    byte ptr [eax], al
        inc    eax
        pop    ds
        add    byte ptr [eax], al
        and    byte ptr [2b110000h], 0
        add    byte ptr [eax - 2bffffc2h], al
        dec    ecx
        add    byte ptr [eax], al
        and    dl, byte ptr [esi]
        add    byte ptr [ebx + ebp*2], bl
        add    byte ptr [eax], al
        add    byte ptr [ebp], bh
        add    byte ptr [ebx], dh
        add    dword ptr [eax], 93a800h
        add    byte ptr [esp + ebp*4], al
        db  000h, 080h, 0BBh, 000h, 000h
azt16_Control:
        cmp    eax, 8
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_01BD:
        cmp    eax, 0bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_01C6:
        cmp    eax, 3
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_01CF:
        cmp    eax, 1bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_01D8:
        cmp    eax, 1ch
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_01E1:
        cmp    eax, 22h
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_01EA:
        clc
        ret
sub_1_01EC:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        push   esi
        pushfd
        mov    eax, dword ptr [ebp + 0ch]
        shl    eax, 3
        db  08Bh, 090h, 000h, 000h, 000h, 000h
        db  08Bh, 080h, 004h, 000h, 000h, 000h
        mov    dword ptr [ebp + 0ch], eax
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_0243
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_0243
loc_1_021F:
        mov    edi, dword ptr [eax]
        test   dword ptr [ebp + 0ch], 1
        jne    SHORT loc_1_0233
        cmp    dword ptr [edx + edi], ebx
        jne    SHORT loc_1_0239
loc_1_022F:
        popfd
        clc
        jmp    SHORT loc_1_0247
loc_1_0233:
        cmp    word ptr [edx + edi], bx
        je     SHORT loc_1_022F
loc_1_0239:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_0243
        jmp    SHORT loc_1_021F
loc_1_0243:
        popfd
        xor    edi, edi
        stc
loc_1_0247:
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret
        db  0B8h, 000h, 004h, 000h, 000h, 0C3h
sub_1_0253:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_01EC
        add    esp, 8
        xor    eax, eax
        or     edi, edi
        je     SHORT loc_1_0270
        mov    eax, dword ptr [edi + 0b8h]
loc_1_0270:
        pop    edi
        pop    ebp
        ret
sub_1_0273:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_01EC
        add    esp, 8
        or     edi, edi
        je     SHORT loc_1_0291
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 0b8h], eax
loc_1_0291:
        pop    edi
        pop    ebp
        ret
sub_1_0294:
        push   eax
        push   ebx
        pushfd
        db  003h, 01Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     SHORT loc_1_02C1
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_02C1
loc_1_02AC:
        cmp    dword ptr [eax + 4], edi
        jne    SHORT loc_1_02B7
        mov    esi, eax
        popfd
        clc
        jmp    SHORT loc_1_02C5
loc_1_02B7:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_02C1
        jmp    SHORT loc_1_02AC
loc_1_02C1:
        popfd
        xor    esi, esi
        stc
loc_1_02C5:
        pop    ebx
        pop    eax
        ret
        db  0EBh, 006h, 0FFh, 025h, 000h, 000h, 000h, 000h
loc_1_02D0:
        pushad
        pushfd
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_031F
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_031F
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        popfd
        pushfd
        or     eax, eax
        je     SHORT loc_1_031F
loc_1_02F1:
        xor    ecx, ecx
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 24h], ebx
        jne    SHORT loc_1_02FD
        or     ecx, 1
loc_1_02FD:
        cmp    dword ptr [edi + 30h], ebx
        jne    SHORT loc_1_0305
        or     ecx, 2
loc_1_0305:
        jecxz  loc_1_0310
        push   eax
        mov    eax, ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_030F:
        pop    eax
loc_1_0310:
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        popfd
        pushfd
        or     eax, eax
        je     SHORT loc_1_031F
        jmp    SHORT loc_1_02F1
loc_1_031F:
        popfd
        popad
        db  0FFh, 025h, 000h, 000h, 000h, 000h
        db  090h
loc_1_0328:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_036C
        db  0CDh, 020h, 004h, 000h, 004h, 000h
        and    dl, 3fh
        push   edi
        push   2
        push   eax
        call   sub_1_01EC
        add    esp, 8
        or     edi, edi
        jne    SHORT loc_1_0354
        push   3
        push   eax
        call   sub_1_01EC
        add    esp, 8
loc_1_0354:
        or     edi, edi
        je     SHORT loc_1_0363
        test   word ptr [edi + 4], 4
        je     SHORT loc_1_0363
        or     dl, 40h
loc_1_0363:
        pop    edi
        xor    dh, dh
        db  0CDh, 020h, 005h, 000h, 004h, 000h
loc_1_036C:
        db  0CDh, 020h, 012h, 000h, 004h, 000h
        ret
        db  090h
loc_1_0374:
        push   eax
        push   edi
        push   edx
        mov    edi, edx
        mov    eax, dword ptr [edi + 24h]
        or     eax, eax
        je     SHORT loc_1_03EC
        cmp    word ptr [edi + 0bch], 0
        jne    SHORT loc_1_03EC
        mov    ebx, eax
        test   word ptr [edi + 52h], 8
        je     SHORT loc_1_03E1
        movzx  edx, word ptr [edi + 22h]
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_03E1
        push   ebx
        mov    ebx, dword ptr [edi + 30h]
        or     ebx, ebx
        je     SHORT loc_1_03CA
        test   word ptr [edi + 4], 2000h
        jne    SHORT loc_1_03CA
        mov    byte ptr [edi + 0ach], al
        or     word ptr [edi + 4], 2000h
        push   edx
        push   esi
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edx, edi
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    esi
        pop    edx
loc_1_03CA:
        pop    ebx
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_0418
        push   ebx
        mov    ebx, 4
        mov    al, 60h
        call   sub_1_07FB
        pop    ebx
loc_1_03E1:
        pop    edx
        pop    edi
        pop    eax
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        clc
        ret
loc_1_03EC:
        test   word ptr [edi + 52h], 8
        je     SHORT loc_1_040B
        movzx  edx, word ptr [edi + 22h]
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_040B
        push   ebx
        mov    ebx, 4
        mov    al, 60h
        call   sub_1_07FB
        pop    ebx
loc_1_040B:
        movzx  edx, word ptr [edi + 42h]
        xor    eax, eax
        add    edx, 2
        out    dx, al
        sub    edx, 2
loc_1_0418:
        pop    edx
        pop    edi
        pop    eax
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        clc
        ret
        db  090h
loc_1_0424:
        push   ecx
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 8
        je     SHORT loc_1_043F
        db  0CDh, 020h, 004h, 000h, 003h, 000h
loc_1_043F:
        pop    ecx
        clc
        ret
        db  08Bh, 0C0h
loc_1_0444:
        push   edi
        push   1
        push   eax
        call   sub_1_01EC
        add    esp, 8
        or     edi, edi
        je     SHORT loc_1_0473
        test   word ptr [edi + 4], 8
        jne    SHORT loc_1_047D
        cmp    dword ptr [edi + 24h], ebx
        jne    SHORT loc_1_0473
        jecxz  loc_1_046B
loc_1_0463:
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_1_047D
loc_1_046B:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        jmp    SHORT loc_1_047D
loc_1_0473:
        test   word ptr [edi + 4], 8000h
        jne    SHORT loc_1_046B
        je     SHORT loc_1_0463
loc_1_047D:
        pop    edi
        clc
        ret
sub_1_0480:
        push   eax
        push   ecx
        push   edx
        add    dx, 0
        mov    ecx, 20000h
        in     al, dx
loc_1_048D:
        in     al, dx
        or     al, al
        jns    SHORT loc_1_0495
        loop   loc_1_048D
        stc
loc_1_0495:
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_1_0499:
        call   sub_1_0480
        jb     SHORT loc_1_04B6
        pushfd
        cli
        push   edx
        add    dx, 0
        mov    al, ah
        db  00Ah, 005h, 000h, 000h, 000h, 000h
        out    dx, al
        inc    dx
        in     al, dx
        pop    edx
        popfd
        clc
loc_1_04B6:
        ret
sub_1_04B7:
        call   sub_1_0480
        jb     SHORT loc_1_04D8
        pushfd
        cli
        push   eax
        push   edx
        add    dx, 0
        xchg   al, ah
        db  00Ah, 005h, 000h, 000h, 000h, 000h
        out    dx, al
        inc    dx
        mov    al, ah
        out    dx, al
        pop    edx
        pop    eax
        popfd
        clc
loc_1_04D8:
        ret
sub_1_04D9:
        push   edx
        push   ecx
        push   eax
        pushfd
        cli
        cmp    word ptr [edi + 4ch], 0
        jne    SHORT loc_1_0530
        mov    ecx, 6
        push   esi
        xor    esi, esi
loc_1_04ED:
        mov    eax, esi
        mov    ah, al
        call   sub_1_0499
        db  088h, 086h, 000h, 000h, 000h, 000h
        or     al, 80h
        call   sub_1_04B7
        inc    esi
        loop   loc_1_04ED
        mov    ecx, 2
loc_1_050B:
        mov    eax, esi
        mov    ah, al
        call   sub_1_0499
        db  088h, 086h, 000h, 000h, 000h, 000h
        push   eax
        and    al, 0fh
        cmp    al, 0dh
        pop    eax
        jbe    SHORT loc_1_0526
        mov    al, 0dh
        jmp    SHORT loc_1_0528
loc_1_0526:
        and    al, 0f0h
loc_1_0528:
        call   sub_1_04B7
        loop   loc_1_050B
        pop    esi
loc_1_0530:
        db  0A0h, 000h, 000h, 000h, 000h
        or     al, 40h
        db  0A2h, 000h, 000h, 000h, 000h
        call   sub_1_0480
        jb     SHORT loc_1_054C
        add    dx, 0
        out    dx, al
        popfd
        clc
        jmp    SHORT loc_1_054E
loc_1_054C:
        popfd
        stc
loc_1_054E:
        pop    eax
        pop    ecx
        pop    edx
        ret
sub_1_0552:
        push   edx
        push   ecx
        push   eax
        pushfd
        cli
        db  0A0h, 000h, 000h, 000h, 000h
        and    al, 0bfh
        db  0A2h, 000h, 000h, 000h, 000h
        call   sub_1_0480
        jb     SHORT loc_1_05BA
        or     al, 9
        add    dx, 0
        out    dx, al
        inc    dx
        in     al, dx
        test   al, 8
        je     SHORT loc_1_0590
        mov    ecx, 1388h
loc_1_057D:
        in     al, dx
        test   al, 20h
        jne    SHORT loc_1_0584
        loop   loc_1_057D
loc_1_0584:
        mov    ecx, 1388h
loc_1_0589:
        in     al, dx
        test   al, 20h
        je     SHORT loc_1_0590
        loop   loc_1_0589
loc_1_0590:
        sub    dx, 1
        cmp    word ptr [edi + 4ch], 0
        jne    SHORT loc_1_05B6
        mov    ecx, 8
        push   esi
        xor    esi, esi
loc_1_05A3:
        mov    eax, esi
        mov    ah, al
        db  08Ah, 086h, 000h, 000h, 000h, 000h
        call   sub_1_04B7
        inc    esi
        loop   loc_1_05A3
        pop    esi
loc_1_05B6:
        popfd
        clc
        jmp    SHORT loc_1_05BC
loc_1_05BA:
        popfd
        stc
loc_1_05BC:
        pop    eax
        pop    ecx
        pop    edx
        ret
sub_1_05C0:
        push   edx
        push   ecx
        push   eax
        mov    cx, ax
        mov    ah, 0ah
        call   sub_1_0499
        and    al, 3fh
        db  08Ah, 025h, 000h, 000h, 000h, 000h
        or     cx, cx
        je     SHORT loc_1_05DF
        or     ah, 40h
        jmp    SHORT loc_1_05F2
loc_1_05DF:
        and    ah, 0bfh
        push   eax
        mov    ecx, 5000h
        mov    ah, 0ah
loc_1_05EA:
        call   sub_1_0499
        loop   loc_1_05EA
        pop    eax
loc_1_05F2:
        db  088h, 025h, 000h, 000h, 000h, 000h
        or     al, ah
        mov    ah, 0ah
        call   sub_1_04B7
        pop    eax
        pop    ecx
        pop    edx
        clc
        ret
        db  052h, 051h, 050h, 09Ch, 0FAh, 0A0h, 000h, 000h, 000h, 000h, 00Ch, 020h, 0A2h, 000h, 000h, 000h
        db  000h, 0E8h, 064h, 0FEh, 0FFh, 0FFh, 072h, 009h, 066h, 083h, 0C2h, 000h, 0EEh, 09Dh, 0F8h, 0EBh
        db  002h, 09Dh, 0F9h, 058h, 059h, 05Ah, 0C3h, 052h, 051h, 050h, 09Ch, 0FAh, 0A0h, 000h, 000h, 000h
        db  000h, 024h, 0DFh, 0A2h, 000h, 000h, 000h, 000h, 0E8h, 03Dh, 0FEh, 0FFh, 0FFh, 072h, 0E2h, 066h
        db  083h, 0C2h, 000h, 0EEh, 09Dh, 0F8h, 0EBh, 002h, 09Dh, 0F9h, 058h, 059h, 05Ah, 0C3h
sub_1_0654:
        pushad
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 42h]
        push   edi
        push   esi
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        call   sub_1_0294
        jb     SHORT loc_1_0674
        mov    edi, dword ptr [esp]
        mov    eax, dword ptr [esi + 0ah]
        mov    dword ptr [edi + 0ah], eax
loc_1_0674:
        pop    esi
        pop    edi
        pushfd
        cli
        cmp    word ptr [edi + 4ch], 0
        jne    SHORT loc_1_0689
        mov    eax, 1
        call   sub_1_05C0
loc_1_0689:
        call   sub_1_04D9
        mov    ecx, 8
        mov    ah, 0fh
loc_1_0695:
        mov    al, byte ptr [ecx + esi + 0fh]
        cmp    ah, 0ah
        jne    SHORT loc_1_06A6
        db  00Ah, 005h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_06B6
loc_1_06A6:
        cmp    ah, 9
        jne    SHORT loc_1_06AF
        and    al, 0ch
        jmp    SHORT loc_1_06B6
loc_1_06AF:
        cmp    ah, 0ah
        jne    SHORT loc_1_06B6
        and    al, 0fdh
loc_1_06B6:
        call   sub_1_04B7
        dec    ah
        loop   loc_1_0695
        mov    ah, 1ch
        mov    al, byte ptr [esi + 24h]
        call   sub_1_04B7
        call   sub_1_0552
        mov    ecx, 7
        mov    ah, 7
loc_1_06D5:
        mov    al, byte ptr [ecx + esi + 8]
        call   sub_1_04B7
        dec    ah
        loop   loc_1_06D5
        cmp    word ptr [edi + 4ch], 0
        jne    SHORT loc_1_06F0
        xor    eax, eax
        call   sub_1_05C0
loc_1_06F0:
        test   word ptr [edi + 4], 200h
        je     SHORT loc_1_072A
        add    edx, 0ah
        push   edx
        mov    eax, 6
        out    dx, al
        movzx  edx, word ptr [edi + 0aeh]
        mov    ecx, 4000h
        mov    eax, 80h
loc_1_0713:
        out    dx, al
        loop   loc_1_0713
        xchg   dword ptr [esp], edx
        mov    al, 7
        out    dx, al
        pop    edx
        mov    ecx, 4000h
        mov    eax, 80h
loc_1_0727:
        out    dx, al
        loop   loc_1_0727
loc_1_072A:
        popfd
        clc
        popad
        ret
sub_1_072E:
        pushad
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 42h]
        pushfd
        cli
        mov    ecx, 0fh
        mov    ah, 0fh
loc_1_073F:
        call   sub_1_0499
        mov    byte ptr [ecx + esi + 8], al
        dec    ah
        loop   loc_1_073F
        mov    ah, 1ch
        call   sub_1_0499
        mov    byte ptr [esi + 24h], al
        popfd
        clc
        popad
        ret
        db  050h, 051h, 052h, 056h, 033h, 0C9h, 00Fh, 0B7h, 0C0h, 03Bh, 081h, 000h, 000h, 000h, 000h, 076h
        db  00Fh, 083h, 0C1h, 004h, 083h, 0B9h, 000h, 000h, 000h, 000h, 0FFh, 075h, 0ECh, 083h, 0E9h, 004h
        db  08Bh, 081h, 000h, 000h, 000h, 000h, 089h, 086h, 090h, 000h, 000h, 000h, 0C1h, 0E9h, 002h, 08Ah
        db  081h, 000h, 000h, 000h, 000h, 03Ah, 086h, 08Ch, 000h, 000h, 000h, 074h, 056h, 088h, 086h, 08Ch
        db  000h, 000h, 000h, 00Fh, 0B7h, 056h, 042h, 066h, 083h, 07Eh, 04Ch, 000h, 075h, 00Ah, 0B8h, 001h
        db  000h, 000h, 000h, 0E8h, 00Eh, 0FEh, 0FFh, 0FFh, 0E8h, 022h, 0FDh, 0FFh, 0FFh, 072h, 034h, 0B4h
        db  008h, 08Ah, 086h, 08Ch, 000h, 000h, 000h, 0E8h, 0F1h, 0FCh, 0FFh, 0FFh, 0E8h, 087h, 0FDh, 0FFh
        db  0FFh, 080h, 07Eh, 00Bh, 08Ah, 074h, 006h, 080h, 07Eh, 00Bh, 081h, 075h, 008h, 0B9h, 0C8h, 000h
        db  000h, 000h, 0ECh, 0E2h, 0FDh, 066h, 083h, 07Eh, 04Ch, 000h, 075h, 007h, 033h, 0C0h, 0E8h, 0D3h
        db  0FDh, 0FFh, 0FFh, 05Eh, 05Ah, 059h, 058h, 0C3h
sub_1_07F2:
        push   eax
        in     al, dx
        jmp    SHORT loc_1_07F6
loc_1_07F6:
        in     al, dx
        jmp    SHORT loc_1_07F9
loc_1_07F9:
        pop    eax
        ret
sub_1_07FB:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_1_07F2
        pop    eax
        inc    dl
        out    dx, al
        call   sub_1_07F2
        pop    edx
        pop    ebx
        ret
sub_1_0818:
        pushad
        mov    ecx, 8000h
        mov    dx, 388h
loc_1_0822:
        in     al, dx
        loop   loc_1_0822
        popad
        ret
sub_1_0827:
        push   ecx
        push   edx
        add    edx, 0eh
        mov    ecx, 5dch
loc_1_0831:
        in     al, dx
        or     al, al
        js     SHORT loc_1_083B
        loop   loc_1_0831
        stc
        jmp    SHORT loc_1_0840
loc_1_083B:
        sub    edx, 4
        in     al, dx
        clc
loc_1_0840:
        pop    edx
        pop    ecx
        ret
sub_1_0843:
        push   eax
        push   ecx
        push   edx
        add    edx, 0ch
        push   eax
        xor    ecx, ecx
        mov    ah, 0ah
loc_1_084E:
        in     al, dx
        or     al, al
        jns    SHORT loc_1_085D
        loop   loc_1_084E
        dec    ah
        jne    SHORT loc_1_084E
        pop    eax
        stc
        jmp    SHORT loc_1_0860
loc_1_085D:
        pop    eax
        out    dx, al
        clc
loc_1_0860:
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_1_0864:
        push   eax
        push   edx
        push   ecx
        add    edx, 6
        mov    al, 1
        out    dx, al
        sub    edx, 6
        jmp    SHORT loc_1_0872
loc_1_0872:
        jmp    SHORT loc_1_0874
loc_1_0874:
        add    edx, 6
        xor    al, al
        out    dx, al
        sub    edx, 6
        call   sub_1_0818
        mov    ecx, 600h
loc_1_0887:
        call   sub_1_0827
        jb     SHORT loc_1_0892
        cmp    al, 0aah
        je     SHORT loc_1_089C
loc_1_0892:
        call   sub_1_0818
        loop   loc_1_0887
        stc
        jmp    SHORT loc_1_089D
loc_1_089C:
        clc
loc_1_089D:
        pop    ecx
        pop    edx
        pop    eax
        ret
sub_1_08A1:
        cmp    byte ptr [edi + 0b4h], 11h
        jne    SHORT loc_1_0908
        cmp    byte ptr [edi + 0b5h], 0
        je     SHORT loc_1_0908
        pushad
        movzx  edx, word ptr [edi + 40h]
        mov    al, 9
        call   sub_1_0843
        jb     SHORT loc_1_090A
        mov    al, 1
        call   sub_1_0843
        jb     SHORT loc_1_090A
        call   sub_1_0818
        movzx  edx, word ptr [edi + 40h]
        add    dx, 404h
        mov    ecx, 10h
loc_1_08DD:
        in     al, dx
        test   al, 8
        jne    SHORT loc_1_08E9
        call   sub_1_0818
        loop   loc_1_08DD
loc_1_08E9:
        mov    byte ptr [edi + 0b5h], 0
        movzx  edx, word ptr [edi + 40h]
        call   sub_1_0864
        mov    al, 40h
        call   sub_1_0843
        mov    al, 1
        call   sub_1_0843
        popad
loc_1_0908:
        clc
        ret
loc_1_090A:
        popad
        stc
        ret
sub_1_090D:
        cmp    byte ptr [edi + 0b4h], 11h
        jne    SHORT loc_1_0966
        cmp    byte ptr [edi + 0b5h], 1
        je     SHORT loc_1_0966
        pushad
        movzx  edx, word ptr [edi + 40h]
        call   sub_1_0864
        jb     SHORT loc_1_0968
        mov    al, 9
        call   sub_1_0843
        jb     SHORT loc_1_0968
        mov    al, 0
        call   sub_1_0843
        jb     SHORT loc_1_0968
        call   sub_1_0818
        movzx  edx, word ptr [edi + 40h]
        add    dx, 404h
        mov    ecx, 10h
loc_1_0950:
        in     al, dx
        test   al, 8
        je     SHORT loc_1_095E
        call   sub_1_0818
        loop   loc_1_0950
        jmp    SHORT loc_1_0968
loc_1_095E:
        mov    byte ptr [edi + 0b5h], 1
        popad
loc_1_0966:
        clc
        ret
loc_1_0968:
        popad
        stc
        ret
sub_1_096B:
        pushad
        movzx  edx, word ptr [edi + 40h]
        mov    ecx, 5
        mov    bx, ax
loc_1_0978:
        mov    al, 8
        call   sub_1_0843
        jb     SHORT loc_1_09D3
        mov    al, 8
        call   sub_1_0843
        jb     SHORT loc_1_09D3
        mov    al, bh
        call   sub_1_0843
        jb     SHORT loc_1_09D3
        mov    al, bl
        call   sub_1_0843
        jb     SHORT loc_1_09D3
        call   sub_1_0818
        call   sub_1_0818
        mov    al, 8
        call   sub_1_0843
        jb     SHORT loc_1_09D3
        mov    al, 7
        call   sub_1_0843
        jb     SHORT loc_1_09D3
        mov    al, bh
        call   sub_1_0843
        jb     SHORT loc_1_09D3
        call   sub_1_0818
        call   sub_1_0827
        jb     SHORT loc_1_09D3
        cmp    al, bl
        je     SHORT loc_1_09D6
        loop   loc_1_0978
loc_1_09D3:
        stc
        popad
        ret
loc_1_09D6:
        clc
        popad
        ret
sub_1_09D9:
        pushad
        cmp    byte ptr [edi + 0b4h], 11h
        je     SHORT loc_1_09EE
        cmp    byte ptr [edi + 0b4h], 0ch
        je     SHORT loc_1_09F7
        jmp    SHORT loc_1_0A16
loc_1_09EE:
        mov    ah, 0bh
        mov    ecx, 4
        jmp    SHORT loc_1_09FE
loc_1_09F7:
        mov    ah, 0ch
        mov    ecx, 3
loc_1_09FE:
        movzx  edx, word ptr [edi + 40h]
        add    dx, 400h
loc_1_0A07:
        in     al, dx
        call   sub_1_096B
        inc    ah
        inc    dx
        loop   loc_1_0A07
        clc
        popad
        ret
loc_1_0A16:
        stc
        popad
        ret
        db  0CCh, 0CCh, 0CCh
_LTEXT ends
        end
