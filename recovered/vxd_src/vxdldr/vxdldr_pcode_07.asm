; vxdldr_pcode_07.asm -- VXDLDR object 7 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_7_0000:
        db  0FEh, 00Dh, 004h, 000h, 000h, 000h
        clc
        ret
VXDLDR_LoadDevice:
        pushad
        push   edx
        call   sub_7_0383
        jb     NEAR PTR loc_7_0095
        db  080h, 03Dh, 0E4h, 000h, 000h, 000h, 000h
        jne    SHORT loc_7_0028
        xor    esi, esi
        db  0E8h, 028h, 000h, 0F9h, 0FFh
        jb     SHORT loc_7_0028
        inc    esi
loc_7_0028:
        mov    edx, dword ptr [esp + 18h]
        call   sub_7_0FE0
        call   sub_7_033D
        pushfd
        db  080h, 03Dh, 0E4h, 000h, 000h, 000h, 000h
        jne    SHORT loc_7_0049
        or     esi, esi
        je     SHORT loc_7_0049
        db  0E8h, 055h, 000h, 0F9h, 0FFh
loc_7_0049:
        popfd
        jb     SHORT loc_7_0088
        or     byte ptr [eax + 0bh], 80h
        mov    dword ptr [esp + 18h], edx
        xchg   dword ptr [esp + 20h], eax
        cmp    byte ptr [edx + 4], 0
        je     SHORT loc_7_0069
        inc    byte ptr [edx + 4]
        jne    SHORT loc_7_0067
        mov    byte ptr [edx + 4], 0ffh
loc_7_0067:
        jmp    SHORT loc_7_0088
loc_7_0069:
        call   sub_7_0C5B
        jb     SHORT loc_7_00A1
        call   sub_7_0E9B
        mov    edi, dword ptr [esp + 20h]
        db  0CDh, 020h, 0F7h, 000h, 001h, 000h
        test   al, 1
        je     SHORT loc_7_0088
        call   sub_7_0C1B
loc_7_0088:
        pushfd
        movzx  eax, byte ptr [ebp + 0eh]
        call   sub_7_0AAA
        popfd
        jae    SHORT loc_7_0099
loc_7_0095:
        mov    dword ptr [esp + 20h], eax
loc_7_0099:
        pop    edx
        call   sub_7_1014
        popad
        ret
loc_7_00A1:
        mov    dword ptr [esp + 20h], ecx
        xor    edi, edi
        xchg   dword ptr [esp + 18h], edi
        mov    esi, 2
        call   sub_7_0D4C
        call   sub_7_0DB8
        clc
        jmp    SHORT loc_7_0088
VXDLDR_UnloadDevice:
        jmp    NEAR PTR loc_7_0C75
sub_7_00C2:
        push   0
        push   edx
        call   sub_7_019D
        je     SHORT loc_7_00D1
        mov    edi, dword ptr [edx + 5]
        test   edi, edi
loc_7_00D1:
        ret
VXDLDR_DevInitSucceeded:
        xor    eax, eax
        pushad
        call   sub_7_00C2
        jns    SHORT loc_7_0141
        or     byte ptr [edi + 0ah], 2
        inc    byte ptr [edx + 4]
        jne    SHORT loc_7_00E9
        mov    byte ptr [edx + 4], 0ffh
loc_7_00E9:
        cmp    dword ptr [edi + 1ch], 0
        je     SHORT loc_7_0108
        cmp    dword ptr [edi + 24h], 0
        jne    SHORT loc_7_0108
        db  0BEh, 071h, 00Fh, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        jb     SHORT loc_7_0108
        mov    dword ptr [edi + 24h], eax
        mov    dword ptr [edx + 1bh], eax
loc_7_0108:
        cmp    dword ptr [edi + 20h], 0
        je     SHORT loc_7_0127
        cmp    dword ptr [edi + 28h], 0
        jne    SHORT loc_7_0127
        db  0BEh, 08Eh, 00Fh, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jb     SHORT loc_7_0127
        mov    dword ptr [edi + 28h], eax
        mov    dword ptr [edx + 1fh], eax
loc_7_0127:
        db  0CDh, 020h, 011h, 001h, 001h, 000h
        cmp    eax, 40000000h
        jb     SHORT loc_7_013E
        mov    edi, edx
        xor    esi, esi
        inc    esi
        call   sub_7_0D55
loc_7_013E:
        clc
        popad
        ret
loc_7_0141:
        popad
        mov    al, 8
        stc
        ret
VXDLDR_DevInitFailed:
        xor    eax, eax
        pushad
        call   sub_7_00C2
        jns    SHORT loc_7_0141
        or     byte ptr [edi + 0ah], 2
        db  0CDh, 020h, 0F8h, 000h, 001h, 000h
        mov    edi, edx
        push   2
        pop    esi
        call   sub_7_0D4C
        call   sub_7_0C08
        call   sub_7_0DB8
        popad
        clc
        ret
        db  061h, 0B0h, 008h, 0C3h
sub_7_0175:
        pushad
        push   5
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        pop    ecx
        pop    ecx
        popad
        ret
sub_7_0188:
        pushad
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        pop    ecx
        popad
        ret
VXDLDR_UnloadMe:
        jmp    NEAR PTR loc_7_0F18
sub_7_019D:
        mov    edx, dword ptr [esp + 4]
        call   sub_7_0175
        db  0B8h, 0E0h, 000h, 000h, 000h
loc_7_01AB:
        mov    ecx, eax
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     SHORT loc_7_01C3
        cmp    eax, edx
        jne    SHORT loc_7_01AB
        test   byte ptr [esp + 8], 1
        je     SHORT loc_7_01C3
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        inc    eax
loc_7_01C3:
        call   sub_7_0188
        test   eax, eax
        ret    8
sub_7_01CD:
        mov    edx, dword ptr [esp + 4]
        call   sub_7_0175
        db  08Bh, 00Dh, 0E0h, 000h, 000h, 000h
        mov    dword ptr [edx], ecx
        db  089h, 015h, 0E0h, 000h, 000h, 000h
        call   sub_7_0188
        ret    4
        db  0E8h, 084h, 0FFh, 0FFh, 0FFh, 051h, 0B9h, 0E0h, 000h, 000h, 000h, 08Bh, 009h, 0E3h, 004h, 03Bh
        db  0D1h, 075h, 0F8h, 0E8h, 084h, 0FFh, 0FFh, 0FFh, 083h, 0F9h, 001h, 059h, 0F5h, 0C3h
VXDLDR_PM_API:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 2
        ja     SHORT loc_7_0232
        db  0FFh, 014h, 085h, 0D4h, 000h, 000h, 000h
        jb     SHORT loc_7_0229
        mov    word ptr [ebp + 1ch], 0
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_7_0229:
        mov    word ptr [ebp + 1ch], ax
        or     byte ptr [ebp + 2ch], 1
        ret
loc_7_0232:
        mov    ax, 0bh
        jmp    SHORT loc_7_0229
loc_7_0238:
        mov    word ptr [ebp + 14h], 100h
        clc
        ret
loc_7_0240:
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edx, eax
        mov    eax, 1
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        ret
loc_7_0257:
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edx, eax
        mov    bx, word ptr [ebp + 10h]
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        ret
loc_7_026D:
        jecxz  loc_7_02B6
        push   ebx
        push   ecx
        push   edx
        push   esi
        dec    ecx
        cmp    ecx, 1
        ja     SHORT loc_7_02A2
        db  0FFh, 014h, 08Dh, 0E0h, 000h, 000h, 000h
        mov    dword ptr [esi + 20h], ecx
        jb     SHORT loc_7_028A
        xor    eax, eax
loc_7_028A:
        pushfd
        cmp    eax, 0bh
        jbe    SHORT loc_7_0295
        mov    eax, 0bh
loc_7_0295:
        db  00Fh, 0B6h, 080h, 0C6h, 000h, 000h, 000h
        popfd
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
loc_7_02A2:
        mov    edx, dword ptr [esi + 18h]
        mov    eax, 0bh
        mov    word ptr [edx], ax
        mov    dword ptr [esi + 20h], 2
        jmp    SHORT loc_7_028A
loc_7_02B6:
        xor    eax, eax
        mov    ecx, 100h
        ret
loc_7_02BE:
        push   esi
        push   edi
        mov    edx, dword ptr [esi + 10h]
        mov    eax, 1
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        mov    edi, dword ptr [esi + 18h]
        mov    word ptr [edi], ax
        mov    ecx, 2
        jb     SHORT loc_7_0308
        mov    word ptr [edi], 0
        or     edx, edx
        je     SHORT loc_7_030B
        mov    ax, word ptr [edx + 9]
        mov    word ptr [edi + 2], ax
        mov    eax, dword ptr [esi + 1ch]
        mov    esi, dword ptr [edx + 0bh]
        mov    ecx, 4
        lea    edi, [edi + 4]
loc_7_02FA:
        cmp    eax, ecx
        jbe    SHORT loc_7_0306
        movsb
        inc    ecx
        cmp    byte ptr [esi - 1], 0
        jne    SHORT loc_7_02FA
loc_7_0306:
        xor    eax, eax
loc_7_0308:
        pop    edi
        pop    esi
        ret
loc_7_030B:
        xor    eax, eax
        mov    ecx, 5
        mov    word ptr [edi + 2], ax
        mov    byte ptr [edi + 4], al
        jmp    SHORT loc_7_0308
loc_7_031B:
        push   edi
        mov    edx, dword ptr [esi + 10h]
        mov    bx, word ptr [edx + 2]
        lea    edx, [edx + 4]
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        mov    edi, dword ptr [esi + 18h]
        jb     SHORT loc_7_0333
        xor    eax, eax
loc_7_0333:
        mov    word ptr [edi], ax
        mov    ecx, 2
        pop    edi
        ret
sub_7_033D:
        push   esi
        call   sub_7_03A0
        jb     SHORT loc_7_037A
        call   sub_7_03CD
        jb     SHORT loc_7_037A
        call   sub_7_0599
        jb     SHORT loc_7_037A
        call   sub_7_064C
        jb     SHORT loc_7_0376
        call   sub_7_065F
        jb     SHORT loc_7_037A
        call   sub_7_0730
        jb     SHORT loc_7_037A
        call   sub_7_075A
        jb     SHORT loc_7_037A
        call   sub_7_0ACA
        jb     SHORT loc_7_037A
loc_7_0376:
        clc
        mov    eax, dword ptr [edx + 5]
loc_7_037A:
        pushfd
        call   sub_7_03B7
        popfd
        pop    esi
        ret
sub_7_0383:
        push   1
        push   417h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_7_039D
        mov    ebp, eax
        dec    word ptr [ebp]
        ret
loc_7_039D:
        inc    eax
        stc
        ret
sub_7_03A0:
        lea    edi, [ebp + 313h]
        db  0CDh, 020h, 0B8h, 000h, 001h, 000h
        mov    byte ptr [ebp + 0eh], 3
        jb     SHORT loc_7_03B6
        mov    word ptr [ebp], ax
loc_7_03B6:
        ret
sub_7_03B7:
        push   eax
        mov    bx, word ptr [ebp]
        cmp    bx, -1
        je     SHORT loc_7_03CB
        mov    ax, 3e00h
        call   sub_7_0FD3
loc_7_03CB:
        pop    eax
        ret
sub_7_03CD:
        mov    byte ptr [ebp + 0eh], 4
        mov    ax, 3f00h
        mov    bx, word ptr [ebp]
        mov    ecx, 40h
        lea    edx, [ebp + 0d3h]
        call   sub_7_0FAF
        jb     NEAR PTR loc_7_04B6
        cmp    ax, 40h
        jne    NEAR PTR loc_7_04B6
        mov    byte ptr [ebp + 0eh], 6
        cmp    word ptr [ebp + 0d3h], 5a4dh
        jne    NEAR PTR loc_7_04B6
        mov    edx, dword ptr [ebp + 10fh]
loc_7_0412:
        mov    byte ptr [ebp + 0eh], 4
        mov    ecx, edx
        shr    ecx, 10h
        and    edx, 0ffffh
        mov    ax, 4200h
        call   sub_7_0FBE
        jb     NEAR PTR loc_7_04B6
        mov    word ptr [ebp + 0ah], ax
        mov    word ptr [ebp + 0ch], dx
        mov    ax, 3f00h
        mov    ecx, 0c4h
        lea    edx, [ebp + 0fh]
        call   sub_7_0FAF
        jb     SHORT loc_7_04B6
        cmp    ax, 0c4h
        jne    SHORT loc_7_04B6
        mov    byte ptr [ebp + 0eh], 6
        cmp    word ptr [ebp + 0fh], 454ch
        jne    SHORT loc_7_04B8
        cmp    word ptr [ebp + 19h], 4
        jne    SHORT loc_7_04B6
        cmp    word ptr [ebp + 17h], 2
        jb     SHORT loc_7_04B6
        mov    eax, dword ptr [ebp + 1fh]
        test   eax, 8000h
        je     SHORT loc_7_04B6
        and    eax, 38000h
        cmp    eax, 38000h
        jne    SHORT loc_7_04B6
        cmp    word ptr [ebp + 0d1h], 400h
        ja     SHORT loc_7_04B6
        cmp    word ptr [ebp + 0d1h], 300h
        jb     SHORT loc_7_04B6
        mov    eax, dword ptr [ebp + 307h]
        add    dword ptr [ebp + 8fh], eax
        clc
        ret
loc_7_04A5:
        mov    eax, dword ptr [ebp + 30bh]
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_7_04B6:
        stc
        ret
loc_7_04B8:
        cmp    dword ptr [ebp + 307h], 0
        jne    SHORT loc_7_04B6
        cmp    word ptr [ebp + 0fh], 454eh
        jne    SHORT loc_7_04B6
        mov    ax, word ptr [ebp + 33h]
        mov    dx, ax
        sub    dx, word ptr [ebp + 35h]
        je     SHORT loc_7_04B6
        neg    dx
        push   edx
        mov    dx, 0ff3ch
        mov    cx, 0ffffh
        add    dx, ax
        adc    cx, 0
        mov    ax, 4201h
        call   sub_7_0FBE
        pop    ecx
        jb     SHORT loc_7_04B6
        movzx  ecx, cx
        push   ecx
        push   0
        push   ecx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_7_04B6
        mov    dword ptr [ebp + 30bh], eax
        mov    edx, eax
        push   edx
        mov    ax, 3f00h
        call   sub_7_0FAF
        pop    ebx
        jb     SHORT loc_7_04A5
        cmp    ax, cx
        jne    SHORT loc_7_04A5
        add    ebx, 2
loc_7_0526:
        movzx  eax, word ptr [ebx]
        or     eax, eax
        je     NEAR PTR loc_7_04A5
        movzx  ecx, word ptr [ebx + 2]
        add    ebx, 8
        test   ah, 80h
        je     SHORT loc_7_0545
        and    ah, 7fh
        cmp    eax, 14h
        je     SHORT loc_7_054C
loc_7_0545:
        imul   ecx, ecx, 0ch
        add    ebx, ecx
        jmp    SHORT loc_7_0526
loc_7_054C:
        or     ecx, ecx
        je     NEAR PTR loc_7_04A5
loc_7_0554:
        mov    ax, word ptr [ebx + 6]
        test   ah, 80h
        je     SHORT loc_7_0565
        and    ah, 7fh
        cmp    eax, 1
        je     SHORT loc_7_056F
loc_7_0565:
        add    ebx, 0ch
        loop   loc_7_0554
        jmp    NEAR PTR loc_7_04A5
loc_7_056F:
        movzx  edx, word ptr [ebx]
        mov    ebx, dword ptr [ebp + 30bh]
        mov    cx, word ptr [ebx]
        shl    edx, cl
        mov    dword ptr [ebp + 307h], edx
        push   edx
        push   0
        push   ebx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
        mov    bx, word ptr [ebp]
        jmp    NEAR PTR loc_7_0412
sub_7_0599:
        mov    dword ptr [ebp + 2], 0
        mov    eax, dword ptr [ebp + 8fh]
        sub    eax, dword ptr [ebp + 0ah]
        mov    dword ptr [ebp + 6], eax
        mov    byte ptr [ebp + 0eh], 1
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_7_0636
        mov    dword ptr [ebp + 2], eax
        mov    byte ptr [ebp + 0eh], 4
        xor    edx, edx
        mov    ecx, edx
        mov    dx, word ptr [ebp + 0ah]
        mov    cx, word ptr [ebp + 0ch]
        mov    bx, word ptr [ebp]
        mov    ax, 4200h
        call   sub_7_0FBE
        jb     SHORT loc_7_0636
        mov    edx, dword ptr [ebp + 2]
        mov    ecx, dword ptr [ebp + 6]
        mov    ax, 3f00h
        call   sub_7_0FAF
        cmp    eax, dword ptr [ebp + 6]
        jne    SHORT loc_7_0636
        mov    esi, dword ptr [ebp + 67h]
        add    esi, dword ptr [ebp + 2]
        cmp    byte ptr [esi], 0
        je     SHORT loc_7_0636
        mov    byte ptr [ebp + 0eh], 6
        mov    esi, dword ptr [ebp + 6bh]
        add    esi, dword ptr [ebp + 2]
        cmp    byte ptr [esi], 0
        je     SHORT loc_7_0636
        mov    al, byte ptr [esi + 1]
        and    al, 7fh
        cmp    al, 3
        jne    SHORT loc_7_0636
        movzx  eax, word ptr [esi + 2]
        cmp    eax, dword ptr [ebp + 53h]
        ja     SHORT loc_7_0636
        mov    word ptr [ebp + 301h], ax
        add    esi, 4
        mov    eax, dword ptr [esi + 1]
        mov    dword ptr [ebp + 303h], eax
        clc
        ret
loc_7_0636:
        xor    eax, eax
        xchg   dword ptr [ebp + 2], eax
        or     eax, eax
        je     SHORT loc_7_064A
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_7_064A:
        stc
        ret
sub_7_064C:
        call   sub_7_0E64
        jb     SHORT loc_7_065D
        cmp    byte ptr [edi + 4], 0
        je     SHORT loc_7_065D
        mov    edx, edi
        stc
        ret
loc_7_065D:
        clc
        ret
sub_7_065F:
        lea    edi, [ebp + 113h]
        mov    ecx, dword ptr [ebp + 53h]
        mov    esi, dword ptr [ebp + 4fh]
        add    esi, dword ptr [ebp + 2]
        xor    edx, edx
loc_7_0670:
        inc    edx
        mov    eax, dword ptr [esi + 8]
        push   ecx
        push   esi
        call   sub_7_0711
        pop    esi
        pop    ecx
        mov    byte ptr [ebp + 0eh], 6
        jb     SHORT loc_7_06E1
        cmp    word ptr [ebp + 301h], dx
        jne    SHORT loc_7_0696
        cmp    eax, 3
        jb     SHORT loc_7_0696
        cmp    eax, 6
        jbe    SHORT loc_7_06E1
loc_7_0696:
        cmp    eax, 0bh
        je     SHORT loc_7_06C5
        cmp    eax, 0ch
        je     SHORT loc_7_06C5
        cmp    eax, -1
        mov    dword ptr [edi + 8], eax
        je     SHORT loc_7_06BA
loc_7_06A8:
        mov    byte ptr [ebp + 0eh], 1
        call   sub_7_0DE7
        jb     SHORT loc_7_06E1
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 4], eax
loc_7_06BA:
        add    edi, 10h
        add    esi, 18h
        dec    ecx
        jne    SHORT loc_7_0670
        clc
        ret
loc_7_06C5:
        mov    dword ptr [edi + 8], eax
        cmp    dword ptr [esi], 0
        je     SHORT loc_7_06D9
        db  0F7h, 005h, 0D3h, 006h, 000h, 000h, 000h, 000h, 040h, 000h
        jne    SHORT loc_7_06A8
loc_7_06D9:
        mov    dword ptr [edi], 0ffc00000h
        jmp    SHORT loc_7_06BA
loc_7_06E1:
        sub    ecx, dword ptr [ebp + 53h]
        neg    ecx
        jecxz  loc_7_070F
        lea    esi, [ebp + 113h]
loc_7_06EE:
        cmp    dword ptr [esi + 8], -1
        je     SHORT loc_7_070A
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_7_070A
        push   ecx
        push   0
        push   dword ptr [esi]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
        or     eax, eax
loc_7_070A:
        add    esi, 10h
        loop   loc_7_06EE
loc_7_070F:
        stc
        ret
sub_7_0711:
        db  0BEh, 005h, 000h, 000h, 000h
        mov    ecx, 10h
loc_7_071B:
        push   eax
        and    eax, dword ptr [esi]
        cmp    eax, dword ptr [esi + 4]
        pop    eax
        je     SHORT loc_7_072B
        add    esi, 0ch
        loop   loc_7_071B
        stc
        ret
loc_7_072B:
        mov    eax, dword ptr [esi + 8]
        clc
        ret
sub_7_0730:
        mov    ecx, dword ptr [ebp + 83h]
        jecxz  loc_7_0758
        mov    esi, dword ptr [ebp + 7fh]
        add    esi, dword ptr [ebp + 2]
        cld
        xor    ebx, ebx
loc_7_0741:
        db  0CDh, 020h, 0F6h, 000h, 001h, 000h
        mov    dword ptr [ebx + ebp + 2a3h], eax
        xor    eax, eax
        lodsb
        add    esi, eax
        add    ebx, 4
        loop   loc_7_0741
loc_7_0758:
        clc
        ret
sub_7_075A:
        mov    esi, dword ptr [ebp + 4fh]
        add    esi, dword ptr [ebp + 2]
        mov    ecx, dword ptr [ebp + 53h]
        lea    edi, [ebp + 113h]
        mov    byte ptr [ebp + 0eh], 6
        xor    edx, edx
loc_7_076F:
        inc    edx
        cmp    dword ptr [edi + 8], -1
        je     SHORT loc_7_079D
        cmp    dword ptr [edi + 8], 0bh
        je     SHORT loc_7_07D3
        cmp    dword ptr [edi + 8], 0ch
        je     SHORT loc_7_07D3
loc_7_0782:
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebp + 2e3h], eax
        push   edi
        push   edx
        call   sub_7_080A
        pop    edx
        pop    edi
        jb     SHORT loc_7_07A7
        call   sub_7_07E1
        jb     SHORT loc_7_07A7
loc_7_079D:
        add    esi, 18h
        add    edi, 10h
        loop   loc_7_076F
        clc
        ret
loc_7_07A7:
        lea    esi, [ebp + 113h]
        mov    ecx, dword ptr [ebp + 53h]
loc_7_07B0:
        cmp    dword ptr [esi + 8], -1
        je     SHORT loc_7_07CA
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_7_07CA
        push   ecx
        push   0
        push   dword ptr [esi]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
loc_7_07CA:
        add    esi, 10h
        or     eax, eax
        loop   loc_7_07B0
        stc
        ret
loc_7_07D3:
        db  0F7h, 005h, 0D9h, 007h, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_7_079D
        jmp    SHORT loc_7_0782
sub_7_07E1:
        cmp    word ptr [ebp + 0d1h], 400h
        jne    SHORT loc_7_0808
        cmp    word ptr [ebp + 301h], dx
        jne    SHORT loc_7_0808
        mov    eax, dword ptr [ebp + 303h]
        add    eax, dword ptr [edi]
        cmp    dword ptr [eax + 3ch], 50726576h
        je     SHORT loc_7_0808
        stc
        ret
loc_7_0808:
        clc
        ret
sub_7_080A:
        push   ecx
        mov    ebx, dword ptr [ebp + 57h]
        add    ebx, dword ptr [ebp + 2]
        mov    ecx, dword ptr [esi + 10h]
        mov    eax, dword ptr [esi + 0ch]
        dec    eax
        lea    ebx, [ebx + eax*4]
        mov    edx, dword ptr [edi + 4]
        mov    dword ptr [ebp + 30fh], edx
        mov    edi, dword ptr [edi]
        cmp    dword ptr [ebp + 2e3h], 0
        jne    SHORT loc_7_0836
        call   sub_7_083D
        jb     SHORT loc_7_083B
loc_7_0836:
        call   sub_7_089A
loc_7_083B:
        pop    ecx
        ret
sub_7_083D:
        push   ebx
        push   ecx
        push   esi
        push   edi
loc_7_0841:
        jecxz  loc_7_087D
        mov    al, byte ptr [ebx + 3]
        and    al, 3
        cmp    al, 0
        je     SHORT loc_7_0890
        cmp    al, 3
        jne    SHORT loc_7_0897
        push   ecx
        call   sub_7_094F
        mov    ecx, dword ptr [ebp + 37h]
        jne    SHORT loc_7_085E
        mov    ecx, dword ptr [ebp + 3bh]
loc_7_085E:
        cmp    ecx, dword ptr [ebp + 30fh]
        jbe    SHORT loc_7_086C
        mov    ecx, dword ptr [ebp + 30fh]
loc_7_086C:
        sub    dword ptr [ebp + 30fh], ecx
        cld
        xor    al, al
        rep stosb
        pop    ecx
        add    ebx, 4
        loop   loc_7_0841
loc_7_087D:
        mov    ecx, dword ptr [ebp + 30fh]
        jecxz  loc_7_088A
        xor    al, al
        cld
        rep stosb
loc_7_088A:
        clc
loc_7_088B:
        pop    edi
        pop    esi
        pop    ecx
        pop    ebx
        ret
loc_7_0890:
        call   sub_7_08D0
        jae    SHORT loc_7_0841
loc_7_0897:
        stc
        jmp    SHORT loc_7_088B
sub_7_089A:
        push   ebx
        push   esi
        jecxz  loc_7_08C9
loc_7_089E:
        call   sub_7_094F
        dec    eax
        js     SHORT loc_7_08C7
        mov    esi, dword ptr [ebp + 2]
        add    esi, dword ptr [ebp + 77h]
        lea    esi, [esi + eax*4]
        mov    edx, dword ptr [esi + 4]
        mov    esi, dword ptr [esi]
        add    edx, dword ptr [ebp + 7bh]
        add    esi, dword ptr [ebp + 7bh]
        call   sub_7_095F
        jb     SHORT loc_7_08CD
        add    edi, dword ptr [ebp + 37h]
        add    ebx, 4
loc_7_08C7:
        loop   loc_7_089E
loc_7_08C9:
        clc
loc_7_08CA:
        pop    esi
        pop    ebx
        ret
loc_7_08CD:
        stc
        jmp    SHORT loc_7_08CA
sub_7_08D0:
        push   ebx
        call   sub_7_094F
        mov    esi, eax
        xor    edx, edx
loc_7_08DA:
        call   sub_7_094F
        cmp    esi, eax
        jne    SHORT loc_7_08FF
        mov    al, byte ptr [ebx + 3]
        and    al, 3
        cmp    al, 0
        jne    SHORT loc_7_08FF
        cmp    dword ptr [ebp + 23h], esi
        mov    eax, dword ptr [ebp + 37h]
        jne    SHORT loc_7_08F7
        mov    eax, dword ptr [ebp + 3bh]
loc_7_08F7:
        add    edx, eax
        inc    esi
        add    ebx, 4
        loop   loc_7_08DA
loc_7_08FF:
        pop    esi
        push   ebx
        push   ecx
        push   edx
        mov    ebx, esi
        call   sub_7_094F
        dec    eax
        mul    dword ptr [ebp + 37h]
        add    eax, dword ptr [ebp + 8fh]
        xchg   edx, eax
        shld   ecx, edx, 10h
        mov    ax, 4200h
        mov    bx, word ptr [ebp]
        call   sub_7_0FBE
        pop    edx
        mov    ecx, edx
        cmp    ecx, dword ptr [ebp + 30fh]
        jbe    SHORT loc_7_0937
        mov    ecx, dword ptr [ebp + 30fh]
loc_7_0937:
        sub    dword ptr [ebp + 30fh], ecx
        mov    edx, edi
        add    edi, ecx
        mov    ah, 3fh
        call   sub_7_0FAF
        jb     SHORT loc_7_094C
        cmp    eax, ecx
loc_7_094C:
        pop    ecx
        pop    ebx
        ret
sub_7_094F:
        movzx  eax, byte ptr [ebx]
        shl    eax, 10h
        mov    ax, word ptr [ebx + 1]
        xchg   al, ah
        cmp    dword ptr [ebp + 23h], eax
        ret
sub_7_095F:
        push   ebx
        push   ecx
        add    esi, dword ptr [ebp + 2]
        add    edx, dword ptr [ebp + 2]
        cld
loc_7_0968:
        push   edx
        cmp    esi, edx
        jae    SHORT loc_7_09D9
        lodsw
        mov    dx, ax
        shl    edx, 10h
        xor    dx, dx
        lodsb
        mov    dl, al
        test   edx, 200000h
        jne    SHORT loc_7_0986
        lodsb
        mov    dh, al
loc_7_0986:
        xor    ebx, ebx
        lodsb
        mov    bl, al
        test   edx, 40000000h
        je     SHORT loc_7_0996
        lodsb
        mov    bh, al
loc_7_0996:
        mov    eax, edx
        and    eax, 3000000h
        cmp    eax, 0
        jne    SHORT loc_7_09AB
        call   sub_7_09E3
        jmp    SHORT loc_7_09B7
loc_7_09A9:
        jmp    SHORT loc_7_09DE
loc_7_09AB:
        cmp    eax, 1000000h
        jne    SHORT loc_7_09A9
        call   sub_7_09F8
loc_7_09B7:
        mov    ecx, 1
        test   edx, 200000h
        je     SHORT loc_7_09CD
        movzx  ecx, dx
loc_7_09C7:
        mov    dx, word ptr [esi]
        add    esi, 2
loc_7_09CD:
        call   sub_7_0A41
        jb     SHORT loc_7_09DE
        loop   loc_7_09C7
        pop    edx
        jmp    SHORT loc_7_0968
loc_7_09D9:
        pop    edx
        pop    ecx
        pop    ebx
        clc
        ret
loc_7_09DE:
        pop    edx
        pop    ecx
        pop    ebx
        stc
        ret
sub_7_09E3:
        xor    eax, eax
        lodsw
        test   edx, 10000000h
        je     SHORT loc_7_09F7
        ror    eax, 10h
        lodsw
        ror    eax, 10h
loc_7_09F7:
        ret
sub_7_09F8:
        xor    eax, eax
        lodsb
        test   edx, 80000000h
        jne    SHORT loc_7_0A1C
        ror    ax, 8
        lodsb
        ror    ax, 8
        test   edx, 10000000h
        je     SHORT loc_7_09F7
        ror    eax, 10h
        lodsw
        ror    eax, 10h
loc_7_0A1C:
        cmp    eax, 2
        xor    eax, eax
        test   edx, 4000000h
        je     SHORT loc_7_0A3B
        lodsw
        test   edx, 20000000h
        je     SHORT loc_7_0A3B
        ror    eax, 10h
        lodsw
        ror    eax, 10h
loc_7_0A3B:
        test   eax, 0ffff0000h
        ret
sub_7_0A41:
        push   eax
        push   ebx
        push   edx
        push   esi
        cmp    ebx, dword ptr [ebp + 53h]
        ja     SHORT loc_7_0A70
        movsx  esi, dx
        shr    edx, 10h
        and    dh, 3
        cmp    dh, 0
        jne    SHORT loc_7_0A8B
        dec    ebx
        shl    ebx, 4
        add    eax, dword ptr [ebx + ebp + 113h]
        and    dl, 0fh
        cmp    dl, 7
        je     SHORT loc_7_0A76
        cmp    dl, 8
        je     SHORT loc_7_0A7B
loc_7_0A70:
        stc
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        ret
loc_7_0A76:
        mov    dword ptr [esi + edi], eax
        jmp    SHORT loc_7_0A85
loc_7_0A7B:
        sub    eax, edi
        sub    eax, esi
        sub    eax, 4
        mov    dword ptr [esi + edi], eax
loc_7_0A85:
        clc
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        ret
loc_7_0A8B:
        cmp    ebx, dword ptr [ebp + 83h]
        ja     SHORT loc_7_0A70
        mov    ebx, dword ptr [ebp + ebx*4 + 29fh]
        or     bx, 0f000h
        mov    word ptr [esi + edi], ax
        mov    word ptr [esi + edi + 2], bx
        jmp    SHORT loc_7_0A85
sub_7_0AAA:
        push   eax
        mov    eax, dword ptr [ebp + 2]
        or     eax, eax
        je     SHORT loc_7_0ABD
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_7_0ABD:
        push   0
        push   ebp
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    eax
        ret
sub_7_0ACA:
        call   sub_7_0E64
        jb     SHORT loc_7_0AF9
        cmp    dword ptr [edi + 17h], 0
        je     SHORT loc_7_0AE9
        mov    byte ptr [ebp + 0eh], 6
        mov    eax, dword ptr [ebp + 53h]
        cmp    dword ptr [edi + 13h], eax
        jne    NEAR PTR loc_7_0BD1
        jmp    SHORT loc_7_0B5D
loc_7_0AE9:
        call   sub_7_0BD3
        je     NEAR PTR loc_7_0BD1
        mov    dword ptr [edi + 17h], eax
        jmp    SHORT loc_7_0B5D
loc_7_0AF9:
        mov    byte ptr [ebp + 0eh], 1
        push   1
        push   23h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        mov    edi, eax
        je     NEAR PTR loc_7_0BAC
        mov    dword ptr [edi + 0fh], 444c5658h
        mov    eax, dword ptr [ebp + 53h]
        mov    dword ptr [edi + 13h], eax
        call   sub_7_0BD3
        je     NEAR PTR loc_7_0BAC
        mov    dword ptr [edi + 17h], eax
        mov    esi, dword ptr [ebp + 67h]
        add    esi, dword ptr [ebp + 2]
        movzx  ecx, byte ptr [esi]
        push   ecx
        inc    ecx
        push   1
        push   ecx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
        mov    dword ptr [edi + 0bh], eax
        or     eax, eax
        je     SHORT loc_7_0BAC
        push   edi
        inc    esi
        mov    edi, eax
        cld
        rep movsb
        xor    al, al
        stosb
        pop    edi
        push   edi
        call   sub_7_01CD
loc_7_0B5D:
        push   edi
        mov    ax, word ptr [ebp + 0cfh]
        mov    word ptr [edi + 9], ax
        mov    esi, dword ptr [ebp + 6bh]
        add    esi, dword ptr [ebp + 2]
        movzx  eax, word ptr [esi + 2]
        add    esi, 4
        mov    esi, dword ptr [esi + 1]
        dec    eax
        imul   eax, eax, 10h
        add    eax, ebp
        add    eax, 113h
        add    esi, dword ptr [eax]
        mov    dword ptr [edi + 5], esi
        mov    eax, dword ptr [edi + 1bh]
        mov    dword ptr [esi + 24h], eax
        mov    eax, dword ptr [edi + 1fh]
        mov    dword ptr [esi + 28h], eax
        mov    eax, dword ptr [ebp + 53h]
        imul   eax, eax, 10h
        mov    ecx, eax
        lea    esi, [ebp + 113h]
        mov    edi, dword ptr [edi + 17h]
        cld
        rep movsb
        pop    edx
        clc
        ret
loc_7_0BAC:
        or     edi, edi
        je     SHORT loc_7_0BD1
        cmp    dword ptr [edi + 17h], 0
        je     SHORT loc_7_0BC3
        push   0
        push   dword ptr [edi + 17h]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_7_0BC3:
        inc    dword ptr [edi + 0fh]
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_7_0BD1:
        stc
        ret
sub_7_0BD3:
        mov    eax, dword ptr [ebp + 53h]
        mov    ebx, 10h
        mul    ebx
        or     edx, edx
        jne    SHORT loc_7_0BEF
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        ret
loc_7_0BEF:
        xor    eax, eax
        ret
sub_7_0BF2:
        mov    ecx, dword ptr [edi + 13h]
        mov    ebx, dword ptr [edi + 17h]
loc_7_0BF8:
        mov    eax, dword ptr [ebx + 8]
        cmp    al, 5
        je     SHORT loc_7_0C1A
        cmp    al, 6
        je     SHORT loc_7_0C1A
        add    ebx, 10h
        loop   loc_7_0BF8
sub_7_0C08:
        xor    eax, eax
        xchg   dword ptr [edi + 17h], eax
        push   edx
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
loc_7_0C1A:
        ret
sub_7_0C1B:
        call   sub_7_107D
        mov    eax, 1bh
        pushad
        mov    ecx, edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  0CDh, 020h, 047h, 001h, 001h, 000h
        popad
        jb     SHORT loc_7_0C4A
        call   sub_7_10A5
        db  0CDh, 020h, 003h, 000h, 027h, 000h
        clc
        ret
loc_7_0C4A:
        call   sub_7_10A5
        db  0CDh, 020h, 004h, 000h, 027h, 000h
        mov    byte ptr [ebp + 0eh], 7
        stc
        ret
sub_7_0C5B:
        push   edi
        push   eax
        mov    edi, dword ptr [edx + 5]
        movzx  eax, word ptr [edi + 6]
        lea    edi, [edi + 0ch]
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        or     ecx, ecx
        je     SHORT loc_7_0C72
        stc
loc_7_0C72:
        pop    eax
        pop    edi
        ret
loc_7_0C75:
        pushad
        call   sub_7_0CFA
        mov    eax, 8
        jb     SHORT loc_7_0CF3
        cmp    byte ptr [edi + 4], 0
        je     SHORT loc_7_0CF3
        mov    edx, edi
        mov    edi, dword ptr [edx + 5]
        mov    eax, 9
        cmp    dword ptr [edi + 30h], 0
        je     SHORT loc_7_0CA0
        test   word ptr [edi + 0ah], 2000h
        jne    SHORT loc_7_0CF3
loc_7_0CA0:
        cmp    byte ptr [edx + 4], 1
        jne    SHORT loc_7_0CED
        mov    eax, 1ch
        pushad
        mov    ecx, edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  0CDh, 020h, 047h, 001h, 001h, 000h
        popad
        mov    eax, 7
        jb     SHORT loc_7_0CF3
        db  0CDh, 020h, 0F8h, 000h, 001h, 000h
        mov    eax, 8
        jb     SHORT loc_7_0CF3
        xor    esi, esi
        mov    edi, edx
        dec    byte ptr [edx + 4]
        call   sub_7_0D4C
        call   sub_7_0BF2
        call   sub_7_0DB8
        jmp    SHORT loc_7_0CF0
loc_7_0CED:
        dec    byte ptr [edx + 4]
loc_7_0CF0:
        popad
        clc
        ret
loc_7_0CF3:
        mov    dword ptr [esp + 1ch], eax
        popad
        stc
        ret
sub_7_0CFA:
        call   sub_7_0175
        db  0BEh, 03Bh, 00Dh, 000h, 000h
        or     bx, bx
        je     SHORT loc_7_0D0F
        cmp    bx, -1
        jne    SHORT loc_7_0D21
loc_7_0D0F:
        db  0BEh, 040h, 00Dh, 000h, 000h
        mov    edi, edx
        cld
        xor    ecx, ecx
        dec    ecx
        xor    al, al
        repne scasb
        neg    ecx
        dec    ecx
loc_7_0D21:
        db  0B8h, 0E0h, 000h, 000h, 000h
loc_7_0D26:
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_7_0D32
        call   esi
        jne    SHORT loc_7_0D26
        mov    edi, eax
loc_7_0D32:
        call   sub_7_0188
        cmp    eax, 1
        ret
loc_7_0D3B:
        cmp    word ptr [eax + 9], bx
        ret
loc_7_0D40:
        push   esi
        push   ecx
        mov    edi, edx
        mov    esi, dword ptr [eax + 0bh]
        repe cmpsb
        pop    ecx
        pop    esi
        ret
sub_7_0D4C:
        xor    eax, eax
        xchg   dword ptr [edi + 5], eax
        and    dword ptr [eax + 0ch], 0
sub_7_0D55:
        pushad
        mov    ecx, dword ptr [edi + 13h]
        mov    ebx, dword ptr [edi + 17h]
        xor    edx, edx
loc_7_0D5E:
        inc    edx
        cmp    dword ptr [ebx + 8], -1
        je     SHORT loc_7_0DB1
        xor    eax, eax
        cmp    dword ptr [ebx + 8], 11h
        jae    SHORT loc_7_0D6E
        inc    eax
loc_7_0D6E:
        xor    eax, esi
        je     SHORT loc_7_0DB1
        cmp    dword ptr [ebx + 8], 5
        je     SHORT loc_7_0DB1
        cmp    dword ptr [ebx + 8], 6
        je     SHORT loc_7_0DB1
        xor    eax, eax
        db  0CDh, 020h, 00Fh, 000h, 027h, 000h
        db  080h, 03Dh, 0C5h, 000h, 000h, 000h, 000h
        je     SHORT loc_7_0DA1
        pushad
        mov    bx, dx
        mov    edi, dword ptr [edi + 0bh]
        mov    dx, ds
        mov    eax, 152h
        int    41h
        popad
loc_7_0DA1:
        push   ecx
        push   edx
        push   0
        push   dword ptr [ebx]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
        pop    ecx
loc_7_0DB1:
        add    ebx, 10h
        loop   loc_7_0D5E
        popad
        ret
sub_7_0DB8:
        mov    eax, dword ptr [edi + 1bh]
        or     eax, dword ptr [edi + 1fh]
        or     eax, dword ptr [edi + 17h]
        jne    SHORT loc_7_0DE6
        push   1
        push   edi
        call   sub_7_019D
        inc    dword ptr [edi + 0fh]
        push   0
        push   dword ptr [edi + 0bh]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_7_0DE6:
        ret
sub_7_0DE7:
        push   ecx
        push   edx
        mov    eax, dword ptr [edi + 8]
        cmp    al, 5
        je     SHORT loc_7_0E2E
        cmp    al, 6
        je     SHORT loc_7_0E2E
loc_7_0DF4:
        xor    ecx, ecx
        cmp    al, 4
        je     SHORT loc_7_0E03
        cmp    al, 3
        jne    SHORT loc_7_0E06
        mov    ecx, 800h
loc_7_0E03:
        or     ch, 1
loc_7_0E06:
        cmp    al, 11h
        jb     SHORT loc_7_0E1C
        push   ecx
        db  0CDh, 020h, 011h, 001h, 001h, 000h
        pop    ecx
        cmp    eax, 40000000h
        ja     SHORT loc_7_0E1C
        or     ch, 4
loc_7_0E1C:
        push   ecx
        push   dword ptr [esi]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_7_0E3E
loc_7_0E2B:
        pop    edx
        pop    ecx
        ret
loc_7_0E2E:
        call   sub_7_0E41
        jb     SHORT loc_7_0DF4
        mov    dword ptr [edi + 0ch], 1
        jmp    SHORT loc_7_0E2B
loc_7_0E3E:
        stc
        jmp    SHORT loc_7_0E2B
sub_7_0E41:
        push   edi
        call   sub_7_0E64
        jb     SHORT loc_7_0E62
        push   esi
        mov    esi, dword ptr [edi + 17h]
        mov    ecx, dword ptr [edi + 13h]
loc_7_0E50:
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_7_0E5E
        add    esi, 10h
        loop   loc_7_0E50
        pop    esi
        stc
        jmp    SHORT loc_7_0E62
loc_7_0E5E:
        mov    eax, dword ptr [esi]
        pop    esi
        clc
loc_7_0E62:
        pop    edi
        ret
sub_7_0E64:
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 67h]
        add    esi, dword ptr [ebp + 2]
        call   sub_7_0175
        db  0BBh, 0E0h, 000h, 000h, 000h
loc_7_0E76:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     SHORT loc_7_0E90
        push   esi
        movzx  ecx, byte ptr [esi]
        inc    esi
        mov    edi, dword ptr [ebx + 0bh]
        cld
        repe cmpsb
        pop    esi
        jne    SHORT loc_7_0E76
        cmp    byte ptr [edi], cl
        jne    SHORT loc_7_0E76
        mov    edi, ebx
loc_7_0E90:
        call   sub_7_0188
        cmp    ebx, 1
        pop    ebx
        pop    esi
        ret
sub_7_0E9B:
        pushad
        mov    edi, edx
        mov    ecx, dword ptr [edx + 13h]
        mov    ebx, dword ptr [edx + 17h]
        lea    ebp, [ebp + 2e7h]
        xor    edx, edx
loc_7_0EAC:
        inc    edx
        cmp    dword ptr [ebx + 8], -1
        je     SHORT loc_7_0F11
        db  080h, 03Dh, 0C5h, 000h, 000h, 000h, 000h
        je     SHORT loc_7_0F08
        mov    word ptr [ebp], dx
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebp + 4], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [edi + 0bh]
        mov    dword ptr [ebp + 0ch], eax
        mov    word ptr [ebp + 10h], ds
        mov    dword ptr [ebp + 12h], eax
        mov    word ptr [ebp + 16h], ds
        push   ebx
        push   edx
        xor    esi, esi
        test   byte ptr [ebx + 8], 1
        mov    ebx, 300028h
        jne    SHORT loc_7_0EEF
        inc    esi
        ror    ebx, 10h
loc_7_0EEF:
        mov    word ptr [ebp + 2], bx
        shr    ebx, 10h
        mov    word ptr [ebp + 18h], bx
        mov    dx, ds
        mov    ebx, ebp
        mov    eax, 150h
        int    41h
        pop    edx
        pop    ebx
loc_7_0F08:
        xor    eax, eax
        inc    eax
        db  0CDh, 020h, 00Fh, 000h, 027h, 000h
loc_7_0F11:
        add    ebx, 10h
        loop   loc_7_0EAC
        popad
        ret
loc_7_0F18:
        pushad
        call   sub_7_0CFA
        mov    eax, 8
        jb     NEAR PTR loc_7_0CF3
        cmp    byte ptr [edi + 4], 0
        je     SHORT loc_7_0F60
        mov    edx, edi
        mov    edi, dword ptr [edx + 5]
        mov    eax, 9
        cmp    dword ptr [edi + 30h], 0
        jne    SHORT loc_7_0F60
        db  0CDh, 020h, 0F8h, 000h, 001h, 000h
        jb     SHORT loc_7_0F60
        dec    byte ptr [edx + 4]
        jne    SHORT loc_7_0F5D
        db  0BEh, 067h, 00Fh, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
loc_7_0F5D:
        popad
        clc
        ret
loc_7_0F60:
        mov    dword ptr [esp + 1ch], eax
        popad
        stc
        ret
loc_7_0F67:
        mov    edi, edx
        xor    esi, esi
        call   sub_7_0D4C
        ret
loc_7_0F71:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        cmp    byte ptr [edx + 4], 0
        je     SHORT loc_7_0F83
        mov    edx, dword ptr [edx + 5]
        jmp    dword ptr [edx + 1ch]
loc_7_0F83:
        mov    word ptr [ebp + 1ch], 0ffffh
        or     dword ptr [ebp + 2ch], 1
        ret
loc_7_0F8E:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        cmp    byte ptr [edx + 4], 0
        je     SHORT loc_7_0F83
        mov    edx, dword ptr [edx + 5]
        jmp    dword ptr [edx + 20h]
loc_7_0FA0:
        pushad
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    dword ptr [esp + 1ch], eax
        popad
        ret
sub_7_0FAF:
        pushad
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    dword ptr [esp + 1ch], eax
        popad
        ret
sub_7_0FBE:
        pushad
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    word ptr [esp + 1ch], ax
        mov    word ptr [esp + 14h], dx
        popad
        ret
sub_7_0FD3:
        pushad
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        popad
        ret
VXDLDR_Notify:
        ret
        db  000h
sub_7_0FE0:
        db  0F6h, 005h, 004h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_7_1013
        push   edx
        push   edx
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        pop    ecx
        pop    edx
        cmp    eax, 104h
        je     SHORT loc_7_1013
        push   edx
        push   0c000ch
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        call   sub_7_10DC
        add    esp, dword ptr [esp]
loc_7_1013:
        ret
sub_7_1014:
        pushfd
        push   eax
        push   ecx
        mov    ecx, eax
        mov    eax, 0dh
        jb     SHORT loc_7_1027
        xor    ecx, ecx
        mov    eax, 0eh
loc_7_1027:
        db  0F6h, 005h, 004h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_7_1079
        push   eax
        push   ecx
        push   edx
        push   edx
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        pop    ecx
        pop    edx
        pop    ecx
        cmp    eax, 104h
        pop    eax
        je     SHORT loc_7_1079
        add    eax, 0c0000h
        push   edx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        call   sub_7_10DC
        add    esp, dword ptr [esp]
        jecxz  loc_7_1079
        cmp    ecx, 0bh
        ja     SHORT loc_7_1079
        db  00Fh, 0B7h, 004h, 04Dh, 00Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        call   sub_7_10DC
loc_7_1079:
        pop    ecx
        pop    eax
        popfd
        ret
sub_7_107D:
        db  0F6h, 005h, 004h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_7_10A4
        push   edi
        mov    edi, dword ptr [edx + 0bh]
        push   edi
        push   0c000fh
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        call   sub_7_10DC
        add    esp, dword ptr [esp]
        pop    edi
loc_7_10A4:
        ret
sub_7_10A5:
        push   edi
        push   eax
        mov    eax, 10h
        jb     SHORT loc_7_10B3
        mov    eax, 11h
loc_7_10B3:
        db  0F6h, 005h, 004h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_7_10D9
        mov    edi, dword ptr [edx + 0bh]
        add    eax, 0c0000h
        push   edi
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        call   sub_7_10DC
        add    esp, dword ptr [esp]
loc_7_10D9:
        pop    eax
        pop    edi
        ret
sub_7_10DC:
        pushad
        db  0F6h, 005h, 004h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_7_1102
        db  0CDh, 020h, 088h, 001h, 001h, 000h
        or     eax, eax
        jne    SHORT loc_7_1102
        mov    edx, dword ptr [esp + 1ch]
        xor    ecx, ecx
        db  0CDh, 020h, 089h, 001h, 001h, 000h
        db  0CDh, 020h, 08Ah, 001h, 001h, 000h
loc_7_1102:
        popad
        ret
_PTEXT ends
        end
