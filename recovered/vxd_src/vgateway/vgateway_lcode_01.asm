; vgateway_lcode_01.asm -- VGATEWAY object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0000:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0CDh, 020h, 00Dh, 000h, 040h, 000h
        pop    ecx
        test   eax, eax
        je     SHORT loc_1_0015
loc_1_0012:
        cld
        leave
        ret
loc_1_0015:
        db  08Bh, 00Dh, 0F4h, 006h, 000h, 000h
        cmp    word ptr [ecx], 0
        jne    SHORT loc_1_0012
        db  0CDh, 020h, 011h, 000h, 040h, 000h
        push   dword ptr [ebp + 0ch]
        db  0CDh, 020h, 00Dh, 000h, 040h, 000h
        pop    ecx
        cld
        leave
        ret
sub_1_0034:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
        pop    eax
        cld
        xor    eax, eax
        leave
        ret
sub_1_0047:
        push   esi
        mov    eax, 1
        cmp    byte ptr [ebx + 4], 0
        jne    SHORT loc_1_0069
        xor    esi, esi
        xchg   dword ptr [ebx + 2d0h], esi
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        push   ebx
        call   sub_1_0034
        pop    edx
        dec    eax
loc_1_0069:
        pop    esi
        ret
loc_1_006B:
        db  0CDh, 020h, 000h, 000h, 014h, 000h
        jb     SHORT loc_1_007B
        cmp    eax, 30ah
        jb     SHORT loc_1_007B
        clc
loc_1_007B:
        ret
loc_1_007C:
        xor    esi, esi
        db  087h, 035h, 0ECh, 006h, 000h, 000h
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        db  083h, 03Dh, 0E8h, 006h, 000h, 000h, 000h
        je     SHORT loc_1_009B
        sub    eax, eax
        db  0CDh, 020h, 003h, 000h, 098h, 004h
loc_1_009B:
        clc
        ret
VGATEWAY_Control:
        cmp    eax, 1
        je     SHORT loc_1_006B
        cmp    eax, 1bh
        je     SHORT loc_1_006B
        cmp    eax, 1ch
        je     SHORT loc_1_007C
        cmp    eax, 23h
        je     SHORT loc_1_00B3
        clc
        ret
loc_1_00B3:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, 0
        jne    SHORT loc_1_00C2
        xor    eax, eax
        jmp    SHORT loc_1_00E8
loc_1_00C2:
        cmp    ecx, -1
        jne    SHORT loc_1_00CB
        xor    eax, eax
        jmp    SHORT loc_1_00E8
loc_1_00CB:
        sub    ecx, 1
        cmp    ecx, 5
        jae    SHORT loc_1_00ED
        mov    edi, dword ptr [esi + 10h]
        db  0FFh, 014h, 08Dh, 010h, 007h, 000h, 000h
        cmp    ax, 1
        jne    SHORT loc_1_00ED
        xor    eax, eax
loc_1_00E8:
        clc
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_00ED:
        mov    eax, 1
        jmp    SHORT loc_1_00E8
VGATEWAY_PM_API:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 6
        jae    SHORT loc_1_0127
        mov    word ptr [ebp + 1ch], 1
        mov    cx, word ptr [ebp + 38h]
        or     cx, word ptr [ebp + 38h]
        je     SHORT loc_1_011B
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
loc_1_011B:
        db  0FFh, 014h, 085h, 0F8h, 006h, 000h, 000h
        mov    word ptr [ebp + 1ch], ax
        ret
loc_1_0127:
        mov    word ptr [ebp + 1ch], 0
        ret
loc_1_012E:
        mov    eax, 8287h
        ret
loc_1_0134:
        db  0CDh, 020h, 003h, 000h, 014h, 000h
        or     edx, eax
        xor    ecx, ecx
        xor    eax, eax
        db  0A3h, 028h, 00Ah, 000h, 000h
        db  0BFh, 02Ch, 00Ah, 000h, 000h
loc_1_014A:
        bt     edx, ecx
        jae    SHORT loc_1_0169
        pushad
        push   ecx
        db  0E8h, 095h, 086h, 0FFh, 0FFh
        pop    ecx
        or     eax, eax
        popad
        je     SHORT loc_1_0165
        db  00Fh, 0ABh, 00Dh, 028h, 00Ah, 000h, 000h
        jmp    SHORT loc_1_0169
loc_1_0165:
        mov    dword ptr [eax + edi], ecx
        inc    eax
loc_1_0169:
        inc    ecx
        cmp    ecx, 10h
        jne    SHORT loc_1_014A
        db  0A3h, 054h, 00Ah, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_017E
        mov    eax, 0
        ret
loc_1_017E:
        db  083h, 03Dh, 0E8h, 006h, 000h, 000h, 001h
        je     NEAR PTR loc_1_021B
        db  0CDh, 020h, 000h, 000h, 098h, 004h
        jb     SHORT loc_1_019F
        db  0C7h, 005h, 0E8h, 006h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_1_021B
loc_1_019F:
        db  0CDh, 020h, 0B6h, 000h, 001h, 000h
        cld
        mov    esi, edx
        db  0BFh, 03Eh, 007h, 000h, 000h
        rep movsb
        mov    byte ptr [edi], 0
        db  068h, 024h, 007h, 000h, 000h
        db  068h, 03Eh, 007h, 000h, 000h
        call   sub_1_0578
        add    esp, 8
        mov    edx, eax
        mov    eax, 1
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jae    SHORT loc_1_0207
        db  0CDh, 020h, 0B6h, 000h, 001h, 000h
        cld
        mov    esi, edx
        db  0BFh, 03Eh, 007h, 000h, 000h
        rep movsb
        mov    byte ptr [edi], 0
        db  068h, 031h, 007h, 000h, 000h
        db  068h, 03Eh, 007h, 000h, 000h
        call   sub_1_0578
        add    esp, 8
        mov    edx, eax
        mov    eax, 1
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_1_021B
loc_1_0207:
        db  0CDh, 020h, 000h, 000h, 098h, 004h
        jb     SHORT loc_1_021B
        db  0C7h, 005h, 0E8h, 006h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_1_021B
loc_1_021B:
        db  0CDh, 020h, 004h, 000h, 015h, 000h
        db  0A3h, 0F4h, 006h, 000h, 000h
        db  0E8h, 031h, 07Eh, 0FFh, 0FFh
        db  083h, 03Dh, 0E8h, 006h, 000h, 000h, 000h
        je     SHORT loc_1_023F
        db  0B8h, 058h, 004h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 098h, 004h
loc_1_023F:
        db  0E8h, 06Ch, 05Fh, 0FFh, 0FFh
        mov    eax, 1
        ret
loc_1_024A:
        push   304h
        push   0
        call   0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_02C0
        mov    dword ptr [edi], eax
        mov    ebx, dword ptr [edi + 4]
        test   ebx, 1
        je     SHORT loc_1_027D
        db  0BBh, 0EDh, 005h, 000h, 000h
        mov    dword ptr [eax + 8], ebx
        mov    ebx, dword ptr [edi + 8]
        mov    dword ptr [eax + 2c4h], ebx
        jmp    SHORT loc_1_028E
loc_1_027D:
        db  0BBh, 0B0h, 005h, 000h, 000h
        mov    dword ptr [eax + 8], ebx
        mov    ebx, dword ptr [edi + 8]
        mov    dword ptr [eax + 2c4h], ebx
loc_1_028E:
        mov    ebx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ebx
        mov    byte ptr [eax + 4], 0
        sub    ebx, ebx
        mov    dword ptr [eax + 2d0h], ebx
        mov    dword ptr [eax + 2c8h], ebx
        mov    dword ptr [eax + 2cch], ebx
        push   edi
        push   dword ptr [edi + 10h]
        push   eax
        db  0E8h, 012h, 07Ch, 0FFh, 0FFh
        add    esp, 8
        pop    edi
        mov    eax, 1
        ret
loc_1_02C0:
        mov    eax, 0
        ret
loc_1_02C6:
        xor    eax, eax
        mov    ebx, dword ptr [edi]
        push   word ptr [ebp + 10h]
        push   word ptr [ebp + 38h]
        mov    eax, dword ptr [edi + 4]
        mov    word ptr [ebp + 10h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        push   edi
        push   ebx
        db  0E8h, 08Dh, 086h, 0FFh, 0FFh
        pop    ebx
        pop    edi
        call   sub_1_0047
        pop    word ptr [ebp + 38h]
        pop    word ptr [ebp + 10h]
        ret
loc_1_0303:
        push   edi
        mov    eax, dword ptr [edi + 4]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 070h, 086h, 0FFh, 0FFh
        add    esp, 8
        pop    edi
        mov    ebx, dword ptr [edi]
        call   sub_1_0047
        ret
loc_1_031C:
        xor    eax, eax
        mov    ebx, dword ptr [edi]
        mov    ebx, dword ptr [edi + 4]
        cmp    ebx, 10h
        jb     SHORT loc_1_0352
        xor    ecx, ecx
        db  08Bh, 00Dh, 028h, 00Ah, 000h, 000h
loc_1_0330:
        bsf    ebx, ecx
        jne    SHORT loc_1_0339
        sub    eax, eax
        jmp    SHORT loc_1_0384
loc_1_0339:
        pushad
        push   ebx
        db  0E8h, 06Bh, 085h, 0FFh, 0FFh
        pop    ebx
        or     eax, eax
        popad
        jne    SHORT loc_1_034B
        btr    ecx, ebx
        jmp    SHORT loc_1_0330
loc_1_034B:
        db  00Fh, 0B3h, 01Dh, 028h, 00Ah, 000h, 000h
loc_1_0352:
        push   ebx
        push   dword ptr [edi]
        db  0E8h, 015h, 087h, 0FFh, 0FFh
        add    esp, 8
        mov    ebx, dword ptr [edi]
        call   sub_1_0047
        or     eax, eax
        je     SHORT loc_1_0384
        mov    eax, 3e8h
        db  0BEh, 09Ch, 003h, 000h, 000h
        mov    edx, dword ptr [edi]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [edx + 2c8h], esi
        mov    ax, 1
loc_1_0384:
        ret
loc_1_0385:
        xor    eax, eax
        mov    ebx, dword ptr [edi]
        push   ebx
        push   ebx
        db  0E8h, 010h, 087h, 0FFh, 0FFh
        add    esp, 4
        pop    ebx
        push   eax
        call   sub_1_0047
        pop    eax
        ret
loc_1_039C:
        mov    edi, edx
        sub    esi, esi
        mov    dword ptr [edx + 2c8h], esi
        cmp    byte ptr [edi + 4], 0
        je     SHORT loc_1_03D6
        mov    esi, dword ptr [edi + 2cch]
        or     esi, esi
        jne    SHORT loc_1_03D6
        push   edx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0BEh, 0EDh, 003h, 000h, 000h
        mov    ecx, 4bh
        sub    eax, eax
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    edx
        mov    dword ptr [edx + 2cch], esi
loc_1_03D6:
        mov    eax, 3e8h
        db  0BEh, 09Ch, 003h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [edx + 2c8h], esi
        ret
loc_1_03ED:
        push   edx
        xor    esi, esi
        mov    dword ptr [edx + 2cch], esi
        db  0E8h, 04Ch, 0FDh, 0FEh, 0FFh
        pop    edx
        cmp    byte ptr [edx + 4], 0
        jne    SHORT loc_1_0418
        xor    esi, esi
        xchg   dword ptr [edx + 2d0h], esi
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        push   edx
        call   sub_1_0034
        pop    edx
        ret
loc_1_0418:
        ret
sub_1_0419:
        pushad
        lea    ecx, [ebx - 34h]
        mov    eax, dword ptr [ecx + 24h]
        mov    edi, dword ptr [ecx + 28h]
        bts    dword ptr [edi + 2d4h], eax
        mov    esi, dword ptr [edi + 2d0h]
        or     esi, esi
        jne    SHORT loc_1_0456
        mov    edx, edi
        push   edi
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0BEh, 090h, 004h, 000h, 000h
        mov    ecx, 4bh
        sub    eax, eax
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    edi
        mov    dword ptr [edi + 2d0h], esi
loc_1_0456:
        popad
        ret
loc_1_0458:
        pushad
        lea    ecx, [ebx - 1ch]
        mov    eax, dword ptr [ecx + 18h]
        db  00Fh, 0ABh, 005h, 0F0h, 006h, 000h, 000h
        db  08Bh, 035h, 0ECh, 006h, 000h, 000h
        or     esi, esi
        jne    SHORT loc_1_048E
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0BEh, 0D8h, 004h, 000h, 000h
        mov    ecx, 4bh
        sub    eax, eax
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        db  089h, 035h, 0ECh, 006h, 000h, 000h
loc_1_048E:
        popad
        ret
loc_1_0490:
        pushad
        cld
loc_1_0492:
        pushfd
        cli
        sub    ebx, ebx
        xchg   dword ptr [edx + 2d4h], ebx
        or     ebx, ebx
        jne    SHORT loc_1_04AA
        mov    dword ptr [edx + 2d0h], ebx
        popfd
loc_1_04A7:
        cld
        popad
        ret
loc_1_04AA:
        popfd
        mov    ecx, 9
loc_1_04B0:
        dec    ecx
        btr    ebx, ecx
        jae    SHORT loc_1_04D3
        cld
        pushad
        push   ebx
        push   ecx
        push   edx
        db  0E8h, 040h, 0FBh, 0FEh, 0FFh
        pop    edx
        pop    ecx
        pop    ebx
        popad
        cmp    byte ptr [edx + 4], 0
        jne    SHORT loc_1_04D3
        push   edx
        call   sub_1_0034
        pop    edx
        jmp    SHORT loc_1_04A7
loc_1_04D3:
        inc    ecx
        loop   loc_1_04B0
        jmp    SHORT loc_1_0492
loc_1_04D8:
        pushad
        cld
        pushfd
        cli
        db  08Bh, 01Dh, 0F0h, 006h, 000h, 000h
        xor    esi, esi
        db  089h, 035h, 0F0h, 006h, 000h, 000h
        db  089h, 035h, 0ECh, 006h, 000h, 000h
        popfd
        btr    ebx, 9
        jae    SHORT loc_1_0502
        pushad
        cld
        push   0
        db  0E8h, 040h, 06Ah, 0FFh, 0FFh
        pop    eax
        popad
loc_1_0502:
        popad
        cld
        ret
sub_1_0505:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 2c8h]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        xor    esi, esi
        xchg   dword ptr [edi + 2cch], esi
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        pop    edi
        pop    esi
        leave
        ret
sub_1_052B:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        repe cmpsb
        movzx  eax, byte ptr [esi - 1]
        movzx  edx, byte ptr [edi - 1]
        sub    eax, edx
        pop    edi
        pop    esi
        leave
        ret
sub_1_054A:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        rep movsb
        mov    eax, edi
        pop    edi
        pop    esi
        leave
        ret
sub_1_0561:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        rep stosb
        mov    eax, edi
        pop    edi
        pop    esi
        leave
        ret
sub_1_0578:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    esi, edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        mov    edi, esi
        mov    esi, dword ptr [ebp + 0ch]
        not    ecx
        shr    ecx, 1
        rep movsw
        adc    ecx, ecx
        rep movsb
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        leave
        ret
loc_1_05B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 2c4h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        mov    ebx, dword ptr [ebp + 0ch]
        and    ebx, 0ffffh
        push   0
        push   0
        push   edx
        push   ecx
        push   ebx
        push   eax
        db  0CDh, 020h, 006h, 000h, 017h, 000h
        add    esp, 18h
        cld
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_05ED:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edx + 0ch]
        push   eax
        push   edx
        mov    eax, dword ptr [edx + 2c4h]
        call   eax
        add    esp, 10h
        cld
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0614:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        db  08Bh, 00Dh, 0F4h, 006h, 000h, 000h
        cmp    word ptr [ecx], 0
        jne    SHORT loc_1_062C
        db  0CDh, 020h, 011h, 000h, 040h, 000h
loc_1_062C:
        mov    ebx, dword ptr [ebp + 8]
        cld
        db  0CDh, 020h, 002h, 000h, 014h, 000h
        cld
        and    eax, 0ffh
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_1_0641:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 0E8h, 006h, 000h, 000h, 000h
        je     SHORT loc_1_0665
        cld
        mov    ebx, dword ptr [ebp + 8]
        db  0CDh, 020h, 002h, 000h, 098h, 004h
        cld
        and    eax, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0665:
        push   dword ptr [ebp + 8]
        call   sub_1_0614
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0672:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        db  083h, 03Dh, 0E8h, 006h, 000h, 000h, 000h
        je     SHORT loc_1_068C
        cld
        mov    ebx, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 098h, 004h
        cld
loc_1_068C:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
        db  000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 056h, 047h, 041h, 054h
        db  045h, 057h, 041h, 059h, 000h, 000h, 000h, 080h, 09Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0F4h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
sub_1_06F8:
        add    dword ptr cs:[eax], eax
        db  000h, 034h, 001h
        add    byte ptr [eax], al
        dec    edx
        add    al, byte ptr [eax]
        db  000h, 01Ch, 003h
        add    byte ptr [eax], al
        test   dword ptr [ebx], eax
        add    byte ptr [eax], al
        mov    byte ptr [edx], 0
        db  000h, 034h, 001h
        add    byte ptr [eax], al
        dec    edx
        add    al, byte ptr [eax]
        db  000h, 01Ch, 003h
        add    byte ptr [eax], al
        test   dword ptr [ebx], eax
        add    byte ptr [eax], al
        add    eax, dword ptr [ebx]
        add    byte ptr [eax], al
loc_1_0724:
        db  073h, 070h
        insb
        imul   esi, dword ptr [esp + esi*2 + 65h], 78762e72h
        add    byte ptr fs:[ebx + 70h], dh
        insb
        imul   esi, dword ptr [esp + esi*2 + 65h], 38332e72h
        add    byte ptr ss:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0793:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp - 75h], dl
        in     al, dx
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_07F3:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0810:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h
loc_1_0840:
        inc    esp
        dec    ecx
        inc    ecx
        dec    esp
        dec    ecx
        dec    esi
        pop    edi
        inc    edi
        inc    ecx
        push   esp
        inc    ebp
        push   edi
        inc    ecx
        pop    ecx
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0854:
        dec    esi
        inc    ebx
        inc    edx
        push   ebx
        push   esp
        inc    ecx
        push   edx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0860:
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_0864:
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_0868:
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_086C:
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_0870:
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_0874:
        dec    esi
        inc    ebx
        inc    edx
        push   ebx
        push   esp
        inc    ecx
        push   edx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0880:
        dec    esi
        inc    ebx
        inc    edx
        push   ebx
        push   esp
        inc    ecx
        push   edx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_088C:
        dec    esi
        inc    ebx
        inc    edx
        push   ebx
        push   esp
        inc    ecx
        push   edx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0898:
        dec    esi
        inc    ebx
        inc    edx
        push   ebx
        push   esp
        inc    ecx
        push   edx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_08A4:
        dec    esi
        inc    ebx
        inc    edx
        push   ebx
        push   esp
        inc    ecx
        push   edx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_08B0:
        push   ebx
        jns    SHORT loc_1_0926
        db  074h, 065h
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        jb     SHORT loc_1_0921
        outsb
        je     SHORT loc_1_0902
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        push   ebx
        jb     NEAR PTR loc_1_0943
        imul   esp, dword ptr [ebx + 65h], 65525c73h
        insd
        outsd
        je     SHORT loc_1_093D
        inc    ecx
        arpl   word ptr [ebx + 65h], sp
        jae    SHORT loc_1_0951
        pop    esp
        push   eax
        popad
        db  072h, 061h
        insd
        je     NEAR PTR loc_1_094C
        db  072h, 073h
        pop    esp
        dec    esi
        db  065h, 074h, 062h
        imul   ebp, dword ptr [edi + 73h], 65746147h
        db  077h, 061h
        jns    SHORT loc_1_08F9
loc_1_08F9:
        add    byte ptr [eax], al
        db  000h
loc_1_08FC:
        dec    ebp
        popad
        js     SHORT loc_1_094E
        popad
        insd
loc_1_0902:
        jae    NEAR PTR loc_1_0905
loc_1_0905:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 61h], cl
        db  078h, 053h
        jae    NEAR PTR loc_1_0982
        imul   ebp, dword ptr [edi + 6eh], 69530073h
        db  07Ah, 057h
        outsd
        db  072h, 06Bh
        bound  esi, qword ptr [ebp + 66h]
        add    byte ptr [eax], al
loc_1_0920:
        dec    ebp
loc_1_0921:
        popad
        db  078h, 044h
        jns    SHORT loc_1_0994
loc_1_0926:
        dec    ebp
        insd
        add    byte ptr [eax], al
        add    byte ptr [edx + 65h], dl
        insd
        outsd
        je     SHORT loc_1_0997
        dec    esp
        imul   esi, dword ptr [ebx + 74h], 6e65h
        add    byte ptr [eax], al
loc_1_093C:
        inc    ebp
loc_1_093D:
        outsb
        popad
        bound  ebp, qword ptr [ebp + 42h]
loc_1_0943:
        db  072h, 06Fh
        popad
        arpl   word ptr fs:[ecx + 73h], sp
        je     SHORT loc_1_094C
loc_1_094C:
        dec    esi
        popad
loc_1_094E:
        insd
        push   ebp
loc_1_0951:
        db  070h, 064h
        popad
        db  074h, 065h
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 69440000h
        jae    SHORT loc_1_09C1
        bound  ebp, qword ptr [ebp + 4dh]
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_09AF
        db  077h, 064h
        push   edi
        push   65536e65h
        db  073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 66617254h
        imul   sp, word ptr [ebx], 0
loc_1_0980:
        dec    ebp
        popad
loc_1_0982:
        js     SHORT loc_1_09C6
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_09CD
        inc    edx
        db  075h, 066h
        jb     NEAR PTR loc_1_09F6
        add    byte ptr fs:[eax], al
loc_1_0994:
        dec    esi
        jne    SHORT loc_1_0A04
loc_1_0997:
        push   edx
        arpl   word ptr gs:[esi + 51h], si
        db  075h, 065h
        db  072h, 079h
        dec    ecx
        outsb
        imul   esp, dword ptr fs:[ebx + 61h], 6e6f6974h
        jae    SHORT loc_1_09AC
loc_1_09AC:
        inc    ecx
        db  075h, 074h
loc_1_09AF:
        outsd
        imul   esi, dword ptr fs:[ebx + 63h], 656e6e6fh
        arpl   word ptr [eax + eax], si
loc_1_09BC:
        dec    ebp
        db  075h, 06Ch
        je     SHORT loc_1_0A2A
loc_1_09C1:
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_0A0C
loc_1_09C6:
        outsd
        jb     SHORT loc_1_0A40
        popad
        jb     SHORT loc_1_0A30
        push   edx
loc_1_09CD:
        popad
        db  074h, 065h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_09D4:
        dec    ebp
        popad
        js     SHORT loc_1_0A1C
        inc    edx
        jne    SHORT loc_1_0A42
        db  066h, 065h, 072h, 065h
        push   eax
        jb     NEAR PTR loc_1_0A2C
        jb     SHORT loc_1_0A56
        db  075h, 070h
        dec    esi
        popad
        insd
        add    byte ptr gs:[eax], al
        add    byte ptr [edx + 63h], dl
        jbe    SHORT loc_1_0A38
        push   ebx
loc_1_09F6:
        jne    SHORT loc_1_0A5A
        insd
        imul   esi, dword ptr [esp + esi*2 + 65h], 72655064h
        inc    edi
        db  072h, 06Fh
loc_1_0A04:
        db  075h, 070h
        dec    esi
        popad
        insd
        add    byte ptr gs:[eax], al
loc_1_0A0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A10:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A1C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A20:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A24:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A28:
        add    byte ptr [eax], al
loc_1_0A2A:
        add    byte ptr [eax], al
loc_1_0A2C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A30:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A38:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A40:
        add    byte ptr [eax], al
loc_1_0A42:
        add    byte ptr [eax], al
loc_1_0A44:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A50:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A54:
        add    byte ptr [eax], al
loc_1_0A56:
        add    byte ptr [eax], al
loc_1_0A58:
        add    byte ptr [eax], al
loc_1_0A5A:
        add    byte ptr [eax], al
_LTEXT ends
        end
