; es1688_lcode_01.asm -- ES1688 object 1 segment _LTEXT
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
        db  000h, 000h, 000h, 000h, 000h, 004h, 0F6h, 033h, 004h, 000h, 000h, 000h, 041h, 055h, 044h, 044h
        db  052h, 056h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00DC:
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx + ebx], dl
        xor    dh, byte ptr [esi]
        cmp    byte ptr [esi], bh
        cmp    bh, byte ptr [eax + eax]
loc_1_0115:
        and    byte ptr [eax - 339f1f40h], 0cch
AUDDRV_Control:
        cmp    eax, 8
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0125:
        cmp    eax, 0bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_012E:
        cmp    eax, 3
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0137:
        cmp    eax, 1bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0140:
        cmp    eax, 1ch
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0149:
        cmp    eax, 22h
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0152:
        clc
        ret
sub_1_0154:
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
        je     SHORT loc_1_01AB
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_01AB
loc_1_0187:
        mov    edi, dword ptr [eax]
        test   dword ptr [ebp + 0ch], 1
        jne    SHORT loc_1_019B
        cmp    dword ptr [edx + edi], ebx
        jne    SHORT loc_1_01A1
loc_1_0197:
        popfd
        clc
        jmp    SHORT loc_1_01AF
loc_1_019B:
        cmp    word ptr [edx + edi], bx
        je     SHORT loc_1_0197
loc_1_01A1:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_01AB
        jmp    SHORT loc_1_0187
loc_1_01AB:
        popfd
        xor    edi, edi
        stc
loc_1_01AF:
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret
sub_1_01B5:
        push   eax
        push   ebx
        pushfd
        db  003h, 01Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     SHORT loc_1_01E2
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_01E2
loc_1_01CD:
        cmp    dword ptr [eax + 4], edi
        jne    SHORT loc_1_01D8
        mov    esi, eax
        popfd
        clc
        jmp    SHORT loc_1_01E6
loc_1_01D8:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_01E2
        jmp    SHORT loc_1_01CD
loc_1_01E2:
        popfd
        xor    esi, esi
        stc
loc_1_01E6:
        pop    ebx
        pop    eax
        ret
        db  02Eh, 08Bh, 0C0h, 0EBh, 006h, 0FFh, 025h, 000h, 000h, 000h, 000h
loc_1_01F4:
        pushad
        pushfd
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_024B
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_024B
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        popfd
        pushfd
        or     eax, eax
        je     SHORT loc_1_024B
loc_1_0215:
        xor    ecx, ecx
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 33h], ebx
        jne    SHORT loc_1_0221
        or     ecx, 1
loc_1_0221:
        cmp    dword ptr [edi + 3bh], ebx
        jne    SHORT loc_1_0229
        or     ecx, 2
loc_1_0229:
        cmp    dword ptr [edi + 43h], ebx
        jne    SHORT loc_1_0231
        or     ecx, 4
loc_1_0231:
        jecxz  loc_1_023C
        push   eax
        mov    eax, ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_023B:
        pop    eax
loc_1_023C:
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        popfd
        pushfd
        or     eax, eax
        je     SHORT loc_1_024B
        jmp    SHORT loc_1_0215
loc_1_024B:
        popfd
        popad
        db  0FFh, 025h, 000h, 000h, 000h, 000h
        db  090h
loc_1_0254:
        db  0CDh, 020h, 012h, 080h, 004h, 000h
        mov    eax, eax
loc_1_025C:
        cld
        push   edi
        push   edx
        push   ebx
        mov    edi, edx
        mov    ebx, dword ptr [edi + 33h]
        or     ebx, ebx
        je     SHORT loc_1_0273
        cmp    word ptr [edi + 84h], 0
        je     SHORT loc_1_0285
loc_1_0273:
        push   eax
        movzx  edx, word ptr [edi + 6]
        add    edx, 0eh
        in     al, dx
        pop    eax
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        jmp    SHORT loc_1_028B
loc_1_0285:
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_1_028B:
        pop    ebx
        pop    edx
        pop    edi
        clc
        ret
loc_1_0290:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        clc
        ret
        db  08Bh, 0C0h
loc_1_02A0:
        push   edi
        push   1
        push   eax
        call   sub_1_0154
        add    esp, 8
        or     edi, edi
        je     SHORT loc_1_02D1
        cmp    dword ptr [edi + 33h], ebx
        jne    SHORT loc_1_02C7
        jecxz  loc_1_02BF
loc_1_02B7:
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_1_02D1
loc_1_02BF:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        jmp    SHORT loc_1_02D1
loc_1_02C7:
        test   word ptr [edi + 4], 8000h
        jne    SHORT loc_1_02BF
        je     SHORT loc_1_02B7
loc_1_02D1:
        pop    edi
        clc
        ret
loc_1_02D4:
        cld
        push   edi
        push   edx
        push   ebx
        mov    edi, edx
        movsx  edx, word ptr [edi + 0ah]
        or     edx, edx
        js     SHORT loc_1_02FC
        mov    ebx, dword ptr [edi + 43h]
        or     ebx, ebx
        jne    SHORT loc_1_0304
        push   eax
        push   ecx
        mov    ecx, 32h
loc_1_02F0:
        inc    edx
        in     al, dx
        or     al, al
        js     SHORT loc_1_02FA
        dec    edx
        in     al, dx
        loop   loc_1_02F0
loc_1_02FA:
        pop    ecx
        pop    eax
loc_1_02FC:
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        jmp    SHORT loc_1_030A
loc_1_0304:
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_1_030A:
        pop    ebx
        pop    edx
        pop    edi
        clc
        ret
        db  090h
loc_1_0310:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        clc
        ret
        db  08Bh, 0C0h
loc_1_0320:
        push   edi
        push   5
        push   eax
        call   sub_1_0154
        add    esp, 8
        or     edi, edi
        je     SHORT loc_1_0351
        cmp    dword ptr [edi + 43h], ebx
        jne    SHORT loc_1_0347
        jecxz  loc_1_033F
loc_1_0337:
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_1_0351
loc_1_033F:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        jmp    SHORT loc_1_0351
loc_1_0347:
        test   word ptr [edi + 4], 4000h
        jne    SHORT loc_1_033F
        je     SHORT loc_1_0337
loc_1_0351:
        pop    edi
        clc
        ret
sub_1_0354:
        push   ecx
        push   edx
        add    edx, 0eh
        mov    ecx, 5dch
loc_1_035E:
        in     al, dx
        or     al, al
        js     SHORT loc_1_0368
        loop   loc_1_035E
        stc
        jmp    SHORT loc_1_036D
loc_1_0368:
        sub    edx, 4
        in     al, dx
        clc
loc_1_036D:
        pop    edx
        pop    ecx
        ret
sub_1_0370:
        push   eax
        push   ecx
        push   edx
        add    edx, 0ch
        push   eax
        mov    ecx, 0a0000h
loc_1_037C:
        in     al, dx
        or     al, al
        jns    SHORT loc_1_0387
        loop   loc_1_037C
        pop    eax
        stc
        jmp    SHORT loc_1_038A
loc_1_0387:
        pop    eax
        out    dx, al
        clc
loc_1_038A:
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_1_038E:
        push   eax
        push   edx
        add    edx, 6
        mov    al, 1
        out    dx, al
        in     al, dx
        in     al, dx
        in     al, dx
        sub    edx, 6
        jmp    SHORT loc_1_039E
loc_1_039E:
        jmp    SHORT loc_1_03A0
loc_1_03A0:
        add    edx, 6
        xor    al, al
        out    dx, al
        sub    edx, 6
        call   sub_1_0354
        pop    edx
        pop    eax
        ret
sub_1_03B1:
        push   eax
        call   sub_1_038E
        jb     SHORT loc_1_03C9
        mov    al, 10h
        call   sub_1_0370
        jb     SHORT loc_1_03C9
        mov    al, 80h
        call   sub_1_0370
loc_1_03C9:
        pop    eax
        ret
sub_1_03CB:
        push   eax
        mov    al, 0c6h
        call   sub_1_0370
        jb     SHORT loc_1_03F9
        mov    al, 0c3h
        call   sub_1_0370
        jb     SHORT loc_1_03F9
        call   sub_1_0354
        jb     SHORT loc_1_03F9
        test   al, 1
        jne    SHORT loc_1_03F9
        mov    al, 0c2h
        call   sub_1_0370
        jb     SHORT loc_1_03F9
        mov    al, 1
        call   sub_1_0370
loc_1_03F9:
        pop    eax
        ret
sub_1_03FB:
        push   eax
        push   edx
        push   edi
        push   esi
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_0434
        cld
        movzx  edx, word ptr [edi + 6]
        call   sub_1_03CB
        jb     SHORT loc_1_0418
        call   sub_1_04F1
loc_1_0418:
        add    dx, 4
        add    edi, 63h
        db  0BEh, 000h, 000h, 000h, 000h
loc_1_0424:
        lodsb
        or     al, al
        je     SHORT loc_1_0434
        out    dx, al
        inc    dx
        mov    al, byte ptr [edi]
        inc    edi
        out    dx, al
        dec    dx
        jmp    SHORT loc_1_0424
loc_1_0434:
        pop    esi
        pop    edi
        pop    edx
        pop    eax
        clc
        ret
sub_1_043A:
        push   eax
        push   edx
        push   edi
        push   esi
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_0465
        cld
        movzx  edx, word ptr [edi + 6]
        add    dx, 4
        add    edi, 63h
        db  0BEh, 000h, 000h, 000h, 000h
loc_1_0457:
        lodsb
        or     al, al
        je     SHORT loc_1_0465
        out    dx, al
        inc    dx
        in     al, dx
        stosb
        dec    dx
        jmp    SHORT loc_1_0457
loc_1_0465:
        pop    esi
        pop    edi
        pop    edx
        pop    eax
        clc
        ret
sub_1_046B:
        push   eax
        push   ecx
        push   esi
        cld
        xor    cl, cl
loc_1_0471:
        lodsw
        cmp    ax, bx
        je     SHORT loc_1_0481
        inc    cl
        cmp    ax, -1
        jne    SHORT loc_1_0471
        stc
loc_1_0481:
        mov    bl, cl
        pop    esi
        pop    ecx
        pop    eax
        ret
sub_1_0487:
        push   eax
        push   ebx
        call   sub_1_046B
        jb     SHORT loc_1_04B4
        mov    al, bl
        shl    al, 2
        or     al, 10h
        push   eax
        mov    al, 0c6h
        call   sub_1_0370
        pop    eax
        jb     SHORT loc_1_04B4
        push   eax
        mov    al, 0b1h
        call   sub_1_0370
        pop    eax
        jb     SHORT loc_1_04B4
        call   sub_1_0370
        jae    SHORT loc_1_04B7
loc_1_04B4:
        stc
        jmp    SHORT loc_1_04B8
loc_1_04B7:
        clc
loc_1_04B8:
        pop    ebx
        pop    eax
        ret
sub_1_04BB:
        push   eax
        push   ebx
        call   sub_1_046B
        jb     SHORT loc_1_04EA
        mov    al, bl
        inc    al
        shl    al, 2
        or     al, 10h
        push   eax
        mov    al, 0c6h
        call   sub_1_0370
        pop    eax
        jb     SHORT loc_1_04EA
        push   eax
        mov    al, 0b2h
        call   sub_1_0370
        pop    eax
        jb     SHORT loc_1_04EA
        call   sub_1_0370
        jae    SHORT loc_1_04ED
loc_1_04EA:
        stc
        jmp    SHORT loc_1_04EE
loc_1_04ED:
        clc
loc_1_04EE:
        pop    ebx
        pop    eax
        ret
sub_1_04F1:
        push   eax
        push   edx
        movzx  edx, word ptr [edi + 6]
        mov    al, 0c6h
        call   sub_1_0370
        jb     SHORT loc_1_0545
        mov    al, 0b1h
        call   sub_1_0370
        jb     SHORT loc_1_0545
        mov    al, byte ptr [edi + 6dh]
        call   sub_1_0370
        jb     SHORT loc_1_0545
        mov    al, 0b2h
        call   sub_1_0370
        jb     SHORT loc_1_0545
        mov    al, byte ptr [edi + 6eh]
        call   sub_1_0370
        jb     SHORT loc_1_0545
        mov    al, 0e1h
        call   sub_1_0370
        call   sub_1_0354
        call   sub_1_0354
        add    dx, 4
        mov    al, 40h
        out    dx, al
        inc    dx
        mov    al, byte ptr [edi + 6fh]
        out    dx, al
        clc
loc_1_0545:
        pop    edx
        pop    eax
        ret
sub_1_0548:
        push   eax
        push   edx
        movzx  edx, word ptr [edi + 6]
        call   sub_1_038E
        jb     SHORT loc_1_05A4
        mov    al, 0c6h
        call   sub_1_0370
        jb     SHORT loc_1_05A4
        mov    al, 0c0h
        call   sub_1_0370
        jb     SHORT loc_1_05A4
        mov    al, 0b1h
        call   sub_1_0370
        jb     SHORT loc_1_05A4
        call   sub_1_0354
        jb     SHORT loc_1_05A4
        mov    byte ptr [edi + 6dh], al
        mov    al, 0c0h
        call   sub_1_0370
        jb     SHORT loc_1_05A4
        mov    al, 0b2h
        call   sub_1_0370
        jb     SHORT loc_1_05A4
        call   sub_1_0354
        jb     SHORT loc_1_05A4
        mov    byte ptr [edi + 6eh], al
        add    dx, 4
        mov    al, 40h
        out    dx, al
        inc    dx
        in     al, dx
        mov    byte ptr [edi + 6fh], al
        clc
loc_1_05A4:
        pop    edx
        pop    eax
        ret
sub_1_05A7:
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        cmp    ecx, -1
        mov    ecx, eax
        mov    ah, 2
        je     SHORT loc_1_05B8
        or     ah, 1
loc_1_05B8:
        cmp    ecx, -1
        je     SHORT loc_1_05DE
        call   sub_1_046B
        jb     SHORT loc_1_05F0
        movzx  ebx, bl
        db  00Ah, 0A3h, 000h, 000h, 000h, 000h
        mov    bx, cx
        mov    esi, edi
        call   sub_1_046B
        jb     SHORT loc_1_05F0
        shl    bl, 3
        or     ah, bl
loc_1_05DE:
        add    dx, 4
        mov    al, 40h
        out    dx, al
        inc    dx
        mov    al, ah
        and    al, 1fh
        out    dx, al
        mov    al, ah
        out    dx, al
        clc
loc_1_05F0:
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret
sub_1_05F6:
        push   eax
        push   edx
        add    edx, 4
        in     al, dx
        mov    ah, al
        mov    al, 40h
        out    dx, al
        inc    edx
        in     al, dx
        push   eax
        and    al, 1fh
        out    dx, al
        pop    eax
        out    dx, al
        dec    edx
        mov    al, ah
        out    dx, al
        clc
        pop    edx
        pop    eax
        ret
sub_1_0611:
        pushad
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 6]
        pushfd
        cli
        popfd
        clc
        popad
        ret
sub_1_061F:
        pushad
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 6]
        pushfd
        cli
        popfd
        clc
        popad
        ret
sub_1_062D:
        push   eax
        push   ecx
        mov    ecx, 6
loc_1_0634:
        in     al, dx
        jmp    SHORT loc_1_0637
loc_1_0637:
        loop   loc_1_0634
        pop    ecx
        pop    eax
        ret
sub_1_063C:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_1_062D
        pop    eax
        inc    dl
        out    dx, al
        call   sub_1_062D
        pop    edx
        pop    ebx
        ret
        db  0CCh, 0CCh, 0CCh
_LTEXT ends
        end
