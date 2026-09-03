; ircomm_lcode_01_01.asm -- IRCOMM object 1 segment _LTEXT  (part 1/2)
.386p
_LTEXT segment dword public use32 'LCODE'

public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 049h, 052h, 043h, 04Fh
        db  04Dh, 04Dh, 020h, 020h, 0FBh, 0FFh, 0FFh, 02Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00B8:
        add    byte ptr [eax + eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00EE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00F8:
        add    byte ptr [ecx], al
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    byte ptr [eax], al
loc_1_0100:
        add    byte ptr [ecx], al
        push   es
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    dword ptr [esi], eax
        add    dword ptr [ecx], eax
        add    dword ptr [edx + 53415203h], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_0118:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0138:
        sbb    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0154:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_0158:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_015C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0160:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0164:
        dec    ecx
        jb     SHORT loc_1_01AB
        inc    ecx
        cmp    cl, byte ptr [ecx + 72h]
        inc    ebx
        dec    edi
        dec    ebp
        dec    ebp
        add    byte ptr [eax + 61h], dl
        jb     SHORT loc_1_01D5
        insd
        je     NEAR PTR loc_1_01DD
        db  072h, 073h
        add    byte ptr [eax], al
loc_1_017C:
        dec    ecx
        db  072h, 04Ch
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ecx + 72h], cl
        inc    esp
        inc    ecx
        cmp    cl, byte ptr [ecx + 72h]
        dec    esp
        dec    ebp
        push   eax
        cmp    cl, byte ptr [ebx + esi*2 + 61h]
        jo     SHORT loc_1_01E7
        insb
        add    byte ptr [eax], al
loc_1_0198:
        dec    ecx
        jb     SHORT loc_1_01DF
        inc    ecx
        cmp    dl, byte ptr [ecx + ebp*2 + 6eh]
        db  079h, 054h
        push   eax
        cmp    cl, byte ptr [ebx + esi*2 + 61h]
        db  070h, 053h
        insb
loc_1_01AB:
        add    byte ptr [ecx + 72h], cl
        inc    esp
        inc    ecx
        cmp    cl, byte ptr [ecx + 72h]
        dec    esp
        dec    ebp
        push   eax
        cmp    cl, byte ptr [ebx + esi*2 + 61h]
        db  070h, 053h
        insb
        add    byte ptr [eax], al
loc_1_01C0:
        dec    ecx
        db  072h, 044h
        inc    ecx
        cmp    cl, byte ptr [ecx + 72h]
        dec    esp
        dec    ebp
        push   eax
        cmp    cl, byte ptr [ebx + edx*2 + 41h]
        push   eax
        push   ebx
        insb
        add    byte ptr [eax], al
loc_1_01D4:
        dec    ecx
loc_1_01D5:
        jb     SHORT loc_1_021A
        dec    edi
        dec    ebp
        dec    ebp
        add    byte ptr [eax], al
loc_1_01DC:
        dec    ecx
loc_1_01DD:
        db  072h, 044h
loc_1_01DF:
        inc    ecx
        cmp    cl, byte ptr [ecx + 72h]
        inc    ebx
        dec    edi
        dec    ebp
        dec    ebp
loc_1_01E7:
        add    byte ptr [ecx + 72h], cl
        inc    esp
        inc    ecx
        cmp    dl, byte ptr [ecx + ebp*2 + 6eh]
        jns    SHORT loc_1_0246
        push   eax
        cmp    cl, byte ptr [ebx + esi*2 + 61h]
        jo     SHORT loc_1_024C
        insb
        add    byte ptr [eax + 61h], dl
        jb     SHORT loc_1_0261
        insd
        je     NEAR PTR loc_1_0269
        jb     SHORT loc_1_0279
        add    byte ptr [eax], al
loc_1_0208:
        dec    esp
        push   eax
        push   esp
        db  000h
loc_1_020C:
        push   edx
        inc    ecx
        push   ebx
        db  000h
sub_1_0210:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_021A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0246:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_024C:
        pop    ss
        add    al, 80h
        add    eax, eax
        add    byte ptr [eax], ah
        xor    byte ptr [eax], al
        sbb    byte ptr [eax], al
        or     al, 0
        or     byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0261:
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0269:
        add    byte ptr [edx], al
        add    ah, cl
        int3
        int3
        int3
IRCOMM_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0279:
        cmp    eax, 1ch
        je     NEAR PTR loc_1_0284
        clc
        ret
loc_1_0284:
        push   esi
        push   edi
        push   ebx
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        call   sub_1_1B78
        pop    ebx
        pop    edi
        pop    esi
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh
sub_1_02A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_02B7
loc_1_02B4:
        dec    dword ptr [ebp - 4]
loc_1_02B7:
        cmp    dword ptr [ebp - 4], 0
        jbe    NEAR PTR loc_1_02D6
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp + 0ch]
        inc    dword ptr [ebp + 8]
        jmp    NEAR PTR loc_1_02B4
loc_1_02D6:
        mov    eax, dword ptr [ebp + 10h]
        jmp    NEAR PTR loc_1_02DE
loc_1_02DE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_02E3:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8
        push   eax
        call   sub_1_5F4C
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_0330
        call   sub_1_5F60
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8
        push   eax
        call   sub_1_5F4C
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_0330
        xor    eax, eax
        jmp    NEAR PTR loc_1_0346
loc_1_0330:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        jmp    NEAR PTR loc_1_0346
loc_1_0346:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_034B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_5F52
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_036E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_6134
        mov    dword ptr [ebp - 4], eax
loc_1_0385:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_03B6
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 110h], 2
        je     NEAR PTR loc_1_03B6
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_6124
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_0385
loc_1_03B6:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_03C5
        jmp    NEAR PTR loc_1_0427
loc_1_03C5:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_042C
        add    esp, 8
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_03F8
        push   800h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_16DA
        add    esp, 8
        jmp    NEAR PTR loc_1_0427
loc_1_03F8:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 418h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 110h], 3
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 1a5h], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
loc_1_0427:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_042C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1a3h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 121h]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_0529
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_0466
loc_1_0462:
        inc    word ptr [ebp - 4]
loc_1_0466:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_051A
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 303h]
        cmp    dword ptr [eax + ecx + 8], edx
        je     NEAR PTR loc_1_04AD
        jmp    NEAR PTR loc_1_0462
loc_1_04AD:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    dx, word ptr [edx + 1a3h]
        and    edx, 0ffffh
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*8]
        mov    ebx, dword ptr [ebp + 8]
        lea    edi, [edx + ebx + 1b7h]
        lea    esi, [eax + ecx + 4]
        mov    ecx, 0ah
        rep movsd
        movsb
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 1a3h]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1a3h]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_0515
        jmp    NEAR PTR loc_1_051A
loc_1_0515:
        jmp    NEAR PTR loc_1_0462
loc_1_051A:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1a3h]
        jmp    NEAR PTR loc_1_0650
loc_1_0529:
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_0538
loc_1_0534:
        inc    word ptr [ebp - 4]
loc_1_0538:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_05E6
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1a3h]
        and    eax, 0ffffh
        cmp    eax, 8
        jl     NEAR PTR loc_1_0571
        jmp    NEAR PTR loc_1_05E6
loc_1_0571:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + ecx + 6]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_05E1
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    dx, word ptr [edx + 1a3h]
        and    edx, 0ffffh
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*8]
        mov    ebx, dword ptr [ebp + 8]
        lea    edi, [edx + ebx + 1b7h]
        lea    esi, [eax + ecx + 4]
        mov    ecx, 0ah
        rep movsd
        movsb
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 1a3h]
loc_1_05E1:
        jmp    NEAR PTR loc_1_0534
loc_1_05E6:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 120h]
        and    eax, 0ffh
        test   al, 2
        je     NEAR PTR loc_1_0621
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0655
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_074C
        add    esp, 8
        jmp    NEAR PTR loc_1_0641
loc_1_0621:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_074C
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0655
        add    esp, 8
loc_1_0641:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1a3h]
        jmp    NEAR PTR loc_1_0650
loc_1_0650:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0655:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_066D
loc_1_066A:
        inc    dword ptr [ebp - 8]
loc_1_066D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jbe    NEAR PTR loc_1_0747
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1a3h]
        and    eax, 0ffffh
        cmp    eax, 8
        jl     NEAR PTR loc_1_069E
        jmp    NEAR PTR loc_1_0747
loc_1_069E:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + ecx + 6]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_06C3
        jmp    NEAR PTR loc_1_066A
loc_1_06C3:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        add    eax, dword ptr [ebp + 0ch]
        add    eax, 0ch
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 20h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0742
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   al, 8
        je     NEAR PTR loc_1_0742
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    dx, word ptr [edx + 1a3h]
        and    edx, 0ffffh
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*8]
        mov    ebx, dword ptr [ebp + 8]
        lea    edi, [edx + ebx + 1b7h]
        lea    esi, [eax + ecx + 4]
        mov    ecx, 0ah
        rep movsd
        movsb
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 1a3h]
loc_1_0742:
        jmp    NEAR PTR loc_1_066A
loc_1_0747:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_074C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_0764
loc_1_0761:
        inc    dword ptr [ebp - 8]
loc_1_0764:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jbe    NEAR PTR loc_1_0863
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1a3h]
        and    eax, 0ffffh
        cmp    eax, 8
        jl     NEAR PTR loc_1_0795
        jmp    NEAR PTR loc_1_0863
loc_1_0795:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + ecx + 6]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_07BA
        jmp    NEAR PTR loc_1_0761
loc_1_07BA:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        add    eax, dword ptr [ebp + 0ch]
        add    eax, 0ch
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 20h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_085E
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   al, 8
        jne    NEAR PTR loc_1_085E
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_085E
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        test   al, 4
        je     NEAR PTR loc_1_085E
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    dx, word ptr [edx + 1a3h]
        and    edx, 0ffffh
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*8]
        mov    ebx, dword ptr [ebp + 8]
        lea    edi, [edx + ebx + 1b7h]
        lea    esi, [eax + ecx + 4]
        mov    ecx, 0ah
        rep movsd
        movsb
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 1a3h]
loc_1_085E:
        jmp    NEAR PTR loc_1_0761
loc_1_0863:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0868:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_1_0871:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1a3h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 1a5h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jg     NEAR PTR loc_1_08A9
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1439
        add    esp, 4
        jmp    NEAR PTR loc_1_099E
loc_1_08A9:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 451h]
        and    eax, 9fh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 451h], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1a5h]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        add    eax, dword ptr [ebp + 8]
        add    eax, 1b7h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 348h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 354h], 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 34ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 350h], 0bb8h
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 055h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 361h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 35dh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 35bh], 45h
        db  0C7h, 005h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 00Bh
        db  0C7h, 005h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 005h, 000h, 000h, 000h, 00Ah
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 044h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 328h
        push   eax
        call   sub_1_5F3D
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_099E
        cmp    dword ptr [ebp - 4], 2
        je     NEAR PTR loc_1_099E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        jmp    NEAR PTR loc_1_0871
loc_1_099E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_09A3:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, 328h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 8], 0fh
        jne    NEAR PTR loc_1_09DE
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
        jmp    NEAR PTR loc_1_0EE1
loc_1_09DE:
        cmp    dword ptr [ebp + 8], 0
        jne    NEAR PTR loc_1_09FB
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 31h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0B20
loc_1_09FB:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 120h]
        and    eax, 3
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 20h]
        mov    al, byte ptr [eax + 28h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0A7D
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 20h]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        test   al, 8
        je     NEAR PTR loc_1_0A7D
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 123h]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_0A7D
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 80h
        jne    NEAR PTR loc_1_0A7D
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 120h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        cmp    eax, ecx
        je     NEAR PTR loc_1_0A9A
loc_1_0A7D:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
        jmp    NEAR PTR loc_1_0EE1
loc_1_0A9A:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 1a5h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [eax + ecx + 1afh], 1
        db  0C7h, 005h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 005h
        db  0C7h, 005h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 005h, 000h, 000h, 000h, 012h
        mov    eax, dword ptr [ebp + 0ch]
        db  0C7h, 040h, 01Ch, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_5F3D
        add    esp, 4
        mov    dword ptr [ebp + 8], eax
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_0B1B
        cmp    dword ptr [ebp + 8], 2
        je     NEAR PTR loc_1_0B1B
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
loc_1_0B1B:
        jmp    NEAR PTR loc_1_0EE1
loc_1_0B20:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 20h
        jne    NEAR PTR loc_1_0DEE
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 32h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_0DEE
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 367h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_0DEE
        mov    eax, dword ptr [ebp - 4]
        add    eax, 36ah
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 28h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 368h]
        mov    word ptr [ebp - 14h], ax
        mov    word ptr [ebp - 24h], 0
        mov    byte ptr [ebp - 20h], 3
        mov    byte ptr [ebp - 10h], 0
        mov    byte ptr [ebp - 1ch], 0
        mov    byte ptr [ebp - 18h], 0
loc_1_0B96:
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_0D00
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        mov    dword ptr [ebp - 2ch], eax
        jmp    NEAR PTR loc_1_0CD5
loc_1_0BCA:
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        add    eax, 2
        mov    word ptr [ebp - 24h], ax
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 20h], al
        inc    word ptr [ebp - 24h]
        jmp    NEAR PTR loc_1_0CFB
loc_1_0BF3:
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        add    eax, 2
        mov    word ptr [ebp - 24h], ax
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 10h], al
        inc    word ptr [ebp - 24h]
        jmp    NEAR PTR loc_1_0CFB
loc_1_0C1C:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 80h
        jne    NEAR PTR loc_1_0C4B
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + ecx]
        and    eax, 80h
        or     al, byte ptr [ebp - 18h]
        mov    byte ptr [ebp - 18h], al
loc_1_0C4B:
        inc    word ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 1ch], al
        inc    word ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        add    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 28h], eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp - 24h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 24h], ax
        jmp    NEAR PTR loc_1_0CFB
loc_1_0C8B:
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + ecx]
        and    eax, 80h
        or     al, byte ptr [ebp - 18h]
        mov    byte ptr [ebp - 18h], al
        inc    word ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 24h]
        and    ecx, 0ffffh
        add    eax, ecx
        inc    eax
        mov    word ptr [ebp - 24h], ax
        jmp    NEAR PTR loc_1_0CFB
        db  0E9h, 026h, 000h, 000h, 000h
loc_1_0CD5:
        cmp    dword ptr [ebp - 2ch], 0
        je     NEAR PTR loc_1_0BF3
        cmp    dword ptr [ebp - 2ch], 1
        je     NEAR PTR loc_1_0BCA
        cmp    dword ptr [ebp - 2ch], 82h
        je     NEAR PTR loc_1_0C1C
        jmp    NEAR PTR loc_1_0C8B
loc_1_0CFB:
        jmp    NEAR PTR loc_1_0B96
loc_1_0D00:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 123h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 10h]
        and    eax, ecx
        mov    byte ptr [ebp - 10h], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 120h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 20h]
        and    eax, ecx
        mov    byte ptr [ebp - 20h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        test   eax, eax
        jne    NEAR PTR loc_1_0DEE
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        test   eax, eax
        je     NEAR PTR loc_1_0DEE
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 120h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 20h]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_0DEE
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_0D9F
        xor    eax, eax
        mov    al, byte ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f4h]
        push   eax
        call   sub_1_4AC0
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_0DEE
loc_1_0D9F:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        or     al, 20h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 451h], al
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 365h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 11ch], ax
        mov    al, byte ptr [ebp - 10h]
        and    eax, 0feh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 1a5h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx + 1afh], al
loc_1_0DEE:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 361h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 363h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_0EE1
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 20h
        je     NEAR PTR loc_1_0EC4
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 1a5h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx + 1afh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0E67
        db  0C7h, 005h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 005h, 000h, 000h, 000h, 013h
        jmp    NEAR PTR loc_1_0E92
loc_1_0E67:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 1a5h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [eax + ecx + 1afh], 1
        db  0C7h, 005h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 005h, 000h, 000h, 000h, 012h
loc_1_0E92:
        mov    eax, dword ptr [ebp + 0ch]
        db  0C7h, 040h, 01Ch, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_5F3D
        add    esp, 4
        mov    dword ptr [ebp + 8], eax
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_0EBF
        cmp    dword ptr [ebp + 8], 2
        jne    NEAR PTR loc_1_0EC4
loc_1_0EBF:
        jmp    NEAR PTR loc_1_0EE1
loc_1_0EC4:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
        jmp    NEAR PTR loc_1_0EE1
loc_1_0EE1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0EE6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, 328h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 8], 0
        jne    NEAR PTR loc_1_0F17
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 31h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0F34
loc_1_0F17:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
        jmp    NEAR PTR loc_1_1047
loc_1_0F34:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 40h
        jne    NEAR PTR loc_1_0FD7
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 32h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_0FD7
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 367h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_0FD7
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 11ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 365h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_0FD7
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        or     al, 40h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 451h], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 368h]
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 1a5h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx + 1a7h], al
loc_1_0FD7:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 361h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 363h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_1047
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 40h
        je     NEAR PTR loc_1_102F
        mov    eax, dword ptr [ebp - 4]
        inc    word ptr [eax + 1a5h]
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 1a3h], 1
        jmp    NEAR PTR loc_1_103B
loc_1_102F:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
loc_1_103B:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
loc_1_1047:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_104C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, 328h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 8], 0fh
        jne    NEAR PTR loc_1_1087
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
        jmp    NEAR PTR loc_1_1221
loc_1_1087:
        cmp    dword ptr [ebp + 8], 0
        jne    NEAR PTR loc_1_10A4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 31h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_10F8
loc_1_10A4:
        db  0C7h, 005h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        db  0C7h, 040h, 01Ch, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_5F3D
        add    esp, 4
        mov    dword ptr [ebp + 8], eax
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_10F3
        cmp    dword ptr [ebp + 8], 2
        je     NEAR PTR loc_1_10F3
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
loc_1_10F3:
        jmp    NEAR PTR loc_1_1221
loc_1_10F8:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 40h
        jne    NEAR PTR loc_1_11B1
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 32h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_11B1
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 367h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_11B1
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 20h
        je     NEAR PTR loc_1_1175
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 11ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 365h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_11B1
loc_1_1175:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        or     al, 40h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 451h], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 368h]
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 1a5h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx + 1a7h], al
loc_1_11B1:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 361h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 363h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_1221
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 40h
        je     NEAR PTR loc_1_1209
        mov    eax, dword ptr [ebp - 4]
        inc    word ptr [eax + 1a5h]
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 1a3h], 1
        jmp    NEAR PTR loc_1_1215
loc_1_1209:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
loc_1_1215:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
loc_1_1221:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_1226:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, 328h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 8], 0
        jne    NEAR PTR loc_1_1257
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 31h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_1274
loc_1_1257:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
        jmp    NEAR PTR loc_1_139D
loc_1_1274:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 40h
        jne    NEAR PTR loc_1_132D
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 32h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_132D
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 367h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_132D
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 20h
        je     NEAR PTR loc_1_12F1
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 11ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 365h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_132D
loc_1_12F1:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        or     al, 40h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 451h], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 368h]
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 1a5h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx + 1a7h], al
loc_1_132D:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 361h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 363h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_139D
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 40h
        je     NEAR PTR loc_1_1385
        mov    eax, dword ptr [ebp - 4]
        inc    word ptr [eax + 1a5h]
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 1a3h], 1
        jmp    NEAR PTR loc_1_1391
loc_1_1385:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13A2
        add    esp, 4
loc_1_1391:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
loc_1_139D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_13A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 1a3h]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1a5h]
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_13CC
loc_1_13C8:
        inc    word ptr [ebp - 4]
loc_1_13CC:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1a3h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_142F
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        inc    eax
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 4]
        and    edx, 0ffffh
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*8]
        mov    ebx, dword ptr [ebp + 8]
        lea    edi, [edx + ebx + 1b7h]
        lea    esi, [eax + ecx + 1b7h]
        mov    ecx, 0ah
        rep movsd
        movsb
        jmp    NEAR PTR loc_1_13C8
loc_1_142F:
        jmp    NEAR PTR loc_1_1434
loc_1_1434:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1439:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1a3h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_146F
        push   800h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_16DA
        add    esp, 8
        jmp    NEAR PTR loc_1_1568
loc_1_146F:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1a3h]
        and    eax, 0ffffh
        cmp    eax, 1
        jle    NEAR PTR loc_1_1498
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_156D
        add    esp, 4
        jmp    NEAR PTR loc_1_14A4
loc_1_1498:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1a5h], 0
loc_1_14A4:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1a5h]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        add    eax, dword ptr [ebp + 8]
        add    eax, 1b7h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 410h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 121h]
        and    eax, 80h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 121h], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1a5h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 1a7h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 419h], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1a5h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 1afh]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 450h], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 110h], 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1584
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        je     NEAR PTR loc_1_1568
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1568
        push   400h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_16DA
        add    esp, 8
loc_1_1568:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_156D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1a5h], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1584:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        shr    eax, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 116h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 124h], 0ffffh
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 126h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 128h], 0ffffh
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 12ah], 0ffffh
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 450h]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_1661
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 116h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 152h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 152h]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 156h], 2
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 116h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 11ah], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 156h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 14fh
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_42D9
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 156h], ax
        jmp    NEAR PTR loc_1_1691
loc_1_1661:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 156h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        shr    eax, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 116h], ax
        db  0A1h, 000h, 000h, 000h, 000h
        shr    eax, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 11ah], ax
loc_1_1691:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 414h], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 14fh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 124h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 410h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3dfh
        push   eax
        call   sub_1_5F0D
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_16D5
loc_1_16D5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_16DA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 110h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 410h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 418h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1a3h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1a5h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        or     dword ptr [ecx + 34h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4C3C
        add    esp, 4
        jmp    NEAR PTR loc_1_1736
loc_1_1736:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_173B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_1762
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_1767
loc_1_1762:
        jmp    NEAR PTR loc_1_1817
loc_1_1767:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_6134
        mov    dword ptr [ebp - 4], eax
loc_1_1775:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_17BC
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 110h], 0
        jne    NEAR PTR loc_1_17A5
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_17BC
loc_1_17A5:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_6124
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1775
loc_1_17BC:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_17CB
        jmp    NEAR PTR loc_1_1817
loc_1_17CB:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_181C
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_17E8
        jmp    NEAR PTR loc_1_1817
loc_1_17E8:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 418h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 110h], 3
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 1a5h], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
loc_1_1817:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_181C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1a3h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 121h]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_1871
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 303h]
        cmp    dword ptr [eax + 4], ecx
        je     NEAR PTR loc_1_186C
        xor    eax, eax
        jmp    NEAR PTR loc_1_18F3
loc_1_186C:
        jmp    NEAR PTR loc_1_18C9
loc_1_1871:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 20h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_188B
        xor    eax, eax
        jmp    NEAR PTR loc_1_18F3
loc_1_188B:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   al, 8
        jne    NEAR PTR loc_1_18C9
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_18C2
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        test   al, 4
        jne    NEAR PTR loc_1_18C9
loc_1_18C2:
        xor    eax, eax
        jmp    NEAR PTR loc_1_18F3
loc_1_18C9:
        mov    edi, dword ptr [ebp + 8]
        add    edi, 1b7h
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, 0ah
        rep movsd
        movsb
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1a3h], 1
        mov    eax, 1
        jmp    NEAR PTR loc_1_18F3
loc_1_18F3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_18F8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_1A9C
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_6134
        mov    dword ptr [ebp - 4], eax
loc_1_1919:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1A97
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 410h], 0
        je     NEAR PTR loc_1_1A80
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 410h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        cmp    dword ptr [eax + 4], ecx
        jne    NEAR PTR loc_1_1A80
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1A25
loc_1_1956:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 121h]
        and    eax, 0ffh
        or     al, 1
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 121h], al
        jmp    NEAR PTR loc_1_1A80
loc_1_1974:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 121h]
        and    eax, 0ffh
        or     al, 2
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 121h], al
        jmp    NEAR PTR loc_1_1A80
loc_1_1992:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 121h]
        and    eax, 0ffh
        or     al, 4
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 121h], al
        jmp    NEAR PTR loc_1_1A80
loc_1_19B0:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 121h]
        and    eax, 0feh
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 121h], al
        jmp    NEAR PTR loc_1_1A80
loc_1_19CC:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 121h]
        and    eax, 0f9h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 121h], al
        jmp    NEAR PTR loc_1_1A80
loc_1_19E8:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 121h]
        and    eax, 0f8h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 121h], al
        jmp    NEAR PTR loc_1_1A80
loc_1_1A04:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 121h]
        and    eax, 0f8h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 121h], al
loc_1_1A1B:
        jmp    NEAR PTR loc_1_1A80
        db  0E9h, 05Bh, 000h, 000h, 000h
loc_1_1A25:
        sub    dword ptr [ebp - 8], 19h
        cmp    dword ptr [ebp - 8], 17h
        ja     NEAR PTR loc_1_1A80
        mov    eax, dword ptr [ebp - 8]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 001h, 002h, 007h, 003h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 004h, 005h, 006h
loc_1_1A80:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_6124
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1919
loc_1_1A97:
        jmp    NEAR PTR loc_1_1B05
loc_1_1A9C:
        cmp    dword ptr [ebp + 0ch], 18h
        je     NEAR PTR loc_1_1AAB
        jmp    NEAR PTR loc_1_1B05
loc_1_1AAB:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_6134
        mov    dword ptr [ebp - 4], eax
loc_1_1AB9:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1AEA
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 110h], 1
        je     NEAR PTR loc_1_1AEA
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_6124
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1AB9
loc_1_1AEA:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_1AF9
        jmp    NEAR PTR loc_1_1B05
loc_1_1AF9:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1B0A
        add    esp, 4
loc_1_1B05:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1B0A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 110h], 2
        push   6
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3dfh
        push   eax
        call   sub_1_5F07
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 17h
        jne    NEAR PTR loc_1_1B52
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 110h], 1
        jmp    NEAR PTR loc_1_1B73
loc_1_1B52:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1B73
        cmp    dword ptr [ebp - 4], 2
        je     NEAR PTR loc_1_1B73
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 110h], 0
loc_1_1B73:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1B78:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_1B81:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1BAE
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_034B
        add    esp, 4
        jmp    NEAR PTR loc_1_1B81
loc_1_1BAE:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_1BDB
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_034B
        add    esp, 4
        jmp    NEAR PTR loc_1_1BAE
loc_1_1BDB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1BE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1BF9:
        add    esp, 4
        cmp    dword ptr [ebp - 4], 2
        jne    NEAR PTR loc_1_1C9C
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_1C29
loc_1_1C26:
        inc    dword ptr [ebp - 8]
loc_1_1C29:
        mov    eax, dword ptr [ebp - 8]
        db  039h, 005h, 000h, 000h, 000h, 000h
        jbe    NEAR PTR loc_1_1C9C
        push   5ch
        call   sub_1_02E3
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_1C54
        jmp    NEAR PTR loc_1_1CA6
loc_1_1C54:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 10h]
        db  0A3h, 000h, 000h, 000h, 000h
        push   800h
        call   sub_1_02E3
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_1C85
        jmp    NEAR PTR loc_1_1CA6
loc_1_1C85:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1C26
loc_1_1C9C:
        mov    eax, 1
        jmp    NEAR PTR loc_1_1CB2
loc_1_1CA6:
        call   sub_1_1B78
        xor    eax, eax
        jmp    NEAR PTR loc_1_1CB2
loc_1_1CB2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1CB7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1CE1
        push   5ch
        call   sub_1_02E3
        add    esp, 4
        jmp    NEAR PTR loc_1_1D05
        db  0E9h, 024h, 000h, 000h, 000h
loc_1_1CE1:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1CFD
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_1CFD:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1D05
loc_1_1D05:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1D0A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1D2E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_034B
        add    esp, 4
        jmp    NEAR PTR loc_1_1D4A
loc_1_1D2E:
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_1_1D4A
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_1D4A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1D4F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1D8D
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        cmp    eax, ecx
        ja     NEAR PTR loc_1_1D7A
        mov    eax, ecx
loc_1_1D7A:
        push   eax
        call   sub_1_02E3
        add    esp, 4
        jmp    NEAR PTR loc_1_1DB1
        db  0E9h, 024h, 000h, 000h, 000h
loc_1_1D8D:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1DA9
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_1DA9:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1DB1
loc_1_1DB1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1DB6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_1DDA
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_034B
        add    esp, 4
        jmp    NEAR PTR loc_1_1DEC
loc_1_1DDA:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_1DEC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1DF1:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_1E09
loc_1_1E06:
        inc    dword ptr [ebp - 8]
loc_1_1E09:
        mov    eax, dword ptr [ebp - 8]
        db  039h, 005h, 000h, 000h, 000h, 000h
        jbe    NEAR PTR loc_1_1EA5
        call   sub_1_1CB7
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1E9B
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1D4F
        add    esp, 4
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0bh], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0bh], 0
        je     NEAR PTR loc_1_1E85
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 8], 50h
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0ah], 0
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 58h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0fh], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 12ch
        push   eax
        call   sub_1_5F70
        add    esp, 8
        jmp    NEAR PTR loc_1_1E96
loc_1_1E85:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D0A
        add    esp, 4
        jmp    NEAR PTR loc_1_1EBE
loc_1_1E96:
        jmp    NEAR PTR loc_1_1EA0
loc_1_1E9B:
        jmp    NEAR PTR loc_1_1EBE
loc_1_1EA0:
        jmp    NEAR PTR loc_1_1E06
loc_1_1EA5:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 114h], ax
        mov    eax, 1
        jmp    NEAR PTR loc_1_1F03
loc_1_1EBE:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 12ch
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1EFC
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0bh]
        push   eax
        call   sub_1_1DB6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D0A
        add    esp, 4
        jmp    NEAR PTR loc_1_1EBE
loc_1_1EFC:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1F03
loc_1_1F03:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_1F08:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 410h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1DF1
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_1F4B
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2026
        add    esp, 0ch
        jmp    NEAR PTR loc_1_2021
loc_1_1F4B:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 11eh]
        and    eax, 0ffffh
        or     eax, 13eh
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 11eh], ax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0f7h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 451h], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 121h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 110h], 5
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_4F72
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 450h]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_2013
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_41AF
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_1FEE
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2026
        add    esp, 0ch
        jmp    NEAR PTR loc_1_2021
loc_1_1FEE:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax], ecx
        jbe    NEAR PTR loc_1_2013
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_46E9
        add    esp, 0ch
        mov    dword ptr [ebp - 8], eax
loc_1_2013:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_35B5
        add    esp, 8
loc_1_2021:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2026:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_371B
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    al, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0bh]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0fh], 1
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_5F19
        add    esp, 8
        cmp    eax, 2
        je     NEAR PTR loc_1_2082
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_38FA
        add    esp, 0ch
loc_1_2082:
        push   400h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_16DA
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4FDF
        add    esp, 4
        jmp    NEAR PTR loc_1_20A4
loc_1_20A4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_20A9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 410h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_20EA
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2270
        add    esp, 0ch
        jmp    NEAR PTR loc_1_226B
loc_1_20EA:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1DF1
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_2119
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2270
        add    esp, 0ch
        jmp    NEAR PTR loc_1_226B
loc_1_2119:
        db  0A1h, 000h, 000h, 000h, 000h
        shr    eax, 1
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 116h], ax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_41AF
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_215F
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2270
        add    esp, 0ch
        jmp    NEAR PTR loc_1_226B
loc_1_215F:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 11eh]
        and    eax, 0ffffh
        or     eax, 13eh
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 11eh], ax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 123h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 450h], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0f7h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 451h], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 121h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 110h], 5
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_4F72
        add    esp, 4
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax], ecx
        jbe    NEAR PTR loc_1_21EE
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_46E9
        add    esp, 0ch
        mov    dword ptr [ebp - 8], eax
loc_1_21EE:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 116h]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 152h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 116h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 156h], 1
        mov    eax, dword ptr [ebp - 4]
        add    eax, 14fh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5F13
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 2
        je     NEAR PTR loc_1_225D
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_225D
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2270
        add    esp, 0ch
        jmp    NEAR PTR loc_1_226B
loc_1_225D:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_35B5
        add    esp, 8
loc_1_226B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2270:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 152h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 156h], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 14fh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_5F19
        add    esp, 8
        cmp    eax, 2
        je     NEAR PTR loc_1_22B8
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 418h], 0
loc_1_22B8:
        jmp    NEAR PTR loc_1_22BD
loc_1_22BD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_22C2:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 410h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 110h], 2
        je     NEAR PTR loc_1_2306
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 110h], 3
        je     NEAR PTR loc_1_2306
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 110h], 4
        jne    NEAR PTR loc_1_231C
loc_1_2306:
        push   800h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_16DA
        add    esp, 8
        jmp    NEAR PTR loc_1_2589
loc_1_231C:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 110h], 7
        jne    NEAR PTR loc_1_239E
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 110h], 9
        mov    eax, dword ptr [ebp - 8]
        add    eax, 410h
        push   eax
        call   sub_1_5F43
        add    esp, 4
loc_1_234A:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 134h
        cmp    dword ptr [eax + 134h], ecx
        je     NEAR PTR loc_1_238D
        mov    eax, dword ptr [ebp - 8]
        add    eax, 134h
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_38FA
        add    esp, 0ch
        jmp    NEAR PTR loc_1_234A
loc_1_238D:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_6144
        add    esp, 4
        jmp    NEAR PTR loc_1_2589
loc_1_239E:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 110h], 5
        jne    NEAR PTR loc_1_2589
        mov    eax, dword ptr [ebp - 8]
        add    eax, 410h
        push   eax
        call   sub_1_5F43
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 120h]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_241A
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 121h]
        and    eax, 0ffh
        or     al, 80h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 121h], al
        mov    eax, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 8]
        add    edi, 2ffh
        mov    esi, dword ptr [eax + 410h]
        mov    ecx, 0ah
        rep movsd
        movsb
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 110h], 0
        jmp    NEAR PTR loc_1_2431
loc_1_241A:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 121h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 110h], 6
loc_1_2431:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_4FDF
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 104h]
        and    eax, 0ffh
        test   al, 40h
        jne    NEAR PTR loc_1_2460
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 58h], 0
        je     NEAR PTR loc_1_2476
loc_1_2460:
        mov    eax, dword ptr [ebp - 8]
        or     dword ptr [eax + 34h], 400h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_4C3C
        add    esp, 4
loc_1_2476:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 410h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 418h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 1a3h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 1a5h], 0
loc_1_24A5:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 13ch
        cmp    dword ptr [eax + 13ch], ecx
        je     NEAR PTR loc_1_24F1
        mov    eax, dword ptr [ebp - 8]
        add    eax, 13ch
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0bh]
        push   eax
        call   sub_1_1DB6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D0A
        add    esp, 4
        jmp    NEAR PTR loc_1_24A5
loc_1_24F1:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 134h
        cmp    dword ptr [eax + 134h], ecx
        je     NEAR PTR loc_1_253D
        mov    eax, dword ptr [ebp - 8]
        add    eax, 134h
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0bh]
        push   eax
        call   sub_1_1DB6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D0A
        add    esp, 4
        jmp    NEAR PTR loc_1_24F1
loc_1_253D:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 12ch
        cmp    dword ptr [eax + 12ch], ecx
        je     NEAR PTR loc_1_2589
        mov    eax, dword ptr [ebp - 8]
        add    eax, 12ch
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0bh]
        push   eax
        call   sub_1_1DB6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D0A
        add    esp, 4
        jmp    NEAR PTR loc_1_253D
loc_1_2589:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_258E:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 410h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 450h]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_278D
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 8]
        mov    cx, word ptr [ecx + 11ah]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 11ah], ax
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], 1
        jbe    NEAR PTR loc_1_2719
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_46E9
        add    esp, 0ch
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax], ecx
        jbe    NEAR PTR loc_1_270F
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 120h]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_264F
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        add    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_5B35
        add    esp, 10h
        jmp    NEAR PTR loc_1_2672
loc_1_264F:
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        add    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_5A92
        add    esp, 10h
loc_1_2672:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 0ch]
        cmp    eax, dword ptr [ebp - 10h]
        jbe    NEAR PTR loc_1_2700
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_371B
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_26FB
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 0ch]
        sub    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        add    eax, dword ptr [ebp - 10h]
        add    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0bh]
        push   eax
        call   sub_1_02A0
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 0ch]
        sub    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0fh], ax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 58h], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 13ch
        push   eax
        call   sub_1_5F70
        add    esp, 8
        jmp    NEAR PTR loc_1_26FB
loc_1_26FB:
        jmp    NEAR PTR loc_1_270A
loc_1_2700:
        mov    eax, dword ptr [ebp - 8]
        inc    word ptr [eax + 116h]
loc_1_270A:
        jmp    NEAR PTR loc_1_2719
loc_1_270F:
        mov    eax, dword ptr [ebp - 8]
        inc    word ptr [eax + 116h]
loc_1_2719:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 110h], 5
        je     NEAR PTR loc_1_2739
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 110h], 7
        jne    NEAR PTR loc_1_2788
loc_1_2739:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 11ah]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_2763
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_35B5
        add    esp, 8
        jmp    NEAR PTR loc_1_2788
loc_1_2763:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 116h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_2788
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_35B5
        add    esp, 8
loc_1_2788:
        jmp    NEAR PTR loc_1_288C
loc_1_278D:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 120h]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_27C5
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_5B35
        add    esp, 10h
        jmp    NEAR PTR loc_1_27E2
loc_1_27C5:
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_5A92
        add    esp, 10h
loc_1_27E2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [eax], ecx
        jbe    NEAR PTR loc_1_288C
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 12ch
        cmp    dword ptr [eax + 12ch], ecx
        je     NEAR PTR loc_1_2877
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_371B
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_2872
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        add    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0bh]
        push   eax
        call   sub_1_02A0
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0fh], ax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 58h], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 13ch
        push   eax
        call   sub_1_5F70
        add    esp, 8
loc_1_2872:
        jmp    NEAR PTR loc_1_288C
loc_1_2877:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5F2B
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        or     dword ptr [eax + 34h], 1
loc_1_288C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_2891:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, 410h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 14fh
        cmp    eax, dword ptr [ebp + 10h]
        je     NEAR PTR loc_1_28EA
        mov    eax, dword ptr [ebp - 8]
        dec    byte ptr [eax + 14eh]
        mov    eax, dword ptr [ebp - 8]
        add    eax, 144h
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_38FA
        add    esp, 0ch
        jmp    NEAR PTR loc_1_2906
loc_1_28EA:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 110h], 8
        jne    NEAR PTR loc_1_2906
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_6144
        add    esp, 4
loc_1_2906:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 110h], 5
        je     NEAR PTR loc_1_2926
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 110h], 7
        jne    NEAR PTR loc_1_293E
loc_1_2926:
        cmp    dword ptr [ebp + 8], 0
        jne    NEAR PTR loc_1_293E
        push   2
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_35B5
        add    esp, 8
loc_1_293E:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 110h], 7
        jne    NEAR PTR loc_1_29AE
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 144h
        cmp    dword ptr [eax + 144h], ecx
        jne    NEAR PTR loc_1_29AE
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 134h
        cmp    dword ptr [eax + 134h], ecx
        jne    NEAR PTR loc_1_29AE
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 110h], 8
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 410h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 121h], 0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_6144
        add    esp, 4
loc_1_29AE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_6134
        mov    dword ptr [ebp - 4], eax
loc_1_29CA:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_2A11
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 110h], 0
        jne    NEAR PTR loc_1_29FA
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 8
        jne    NEAR PTR loc_1_2A11
loc_1_29FA:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_6124
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_29CA
loc_1_2A11:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_2A20
        jmp    NEAR PTR loc_1_2A71
loc_1_2A20:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_042C
        add    esp, 8
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_2A42
        jmp    NEAR PTR loc_1_2A71
loc_1_2A42:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 418h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 110h], 3
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 1a5h], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0868
        add    esp, 4
loc_1_2A71:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_2A76:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 2dh
        je     NEAR PTR loc_1_2A8E
        jmp    NEAR PTR loc_1_2B11
loc_1_2A8E:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_6134
        mov    dword ptr [ebp - 4], eax
loc_1_2A9C:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_2B11
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 110h], 9
        je     NEAR PTR loc_1_2AFA
        mov    eax, dword ptr [ebp - 4]
        or     dword ptr [eax + 34h], 400h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_4C3C
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 410h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 121h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 110h], 9
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 418h], 0
loc_1_2AFA:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_6124
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_2A9C
loc_1_2B11:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_2B16:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 3a6h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0efh
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 451h], al
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_6144
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2B52:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_5ED8
        test   eax, eax
        jne    NEAR PTR loc_1_2B72
        mov    eax, 1
        jmp    NEAR PTR loc_1_2EED
loc_1_2B72:
        call   sub_1_5EE3
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_2B98
        cmp    dword ptr [ebp - 4], 4
        je     NEAR PTR loc_1_2B98
        mov    eax, 1
        jmp    NEAR PTR loc_1_2EED
loc_1_2B98:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 451h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 410h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1a3h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1a5h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 121h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 11eh], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_49BF
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 114h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 14dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 14ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 14eh], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3dfh
        push   eax
        call   sub_1_5EE9
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_2C36
        mov    eax, 1
        jmp    NEAR PTR loc_1_2EED
loc_1_2C36:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 418h], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 410h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3dfh
        push   eax
        call   sub_1_5EEF
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_2C6C
        mov    eax, 1
        jmp    NEAR PTR loc_1_2EED
loc_1_2C6C:
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3dfh
        push   eax
        push   0ffffh
        call   sub_1_5EFB
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_1_2CA3
        mov    eax, 1
        jmp    NEAR PTR loc_1_2EED
loc_1_2CA3:
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 0BDh, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 3bch], 0bh
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 0C1h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 3c5h], 2
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 0C8h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 3c7h], 13h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 3cch], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 3dfh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 3cdh], eax
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 0D4h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 3d3h], 0ah
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 3d8h], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_2D60
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 3d9h], 0bh
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 0DBh, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_2DAD
loc_1_2D60:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 120h]
        and    eax, 0ffh
        test   al, 2
        je     NEAR PTR loc_1_2D94
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 3d9h], 6
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 0DBh, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_2DAD
loc_1_2D94:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 3d9h], 6
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 0DBh, 003h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2DAD:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 13ch
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 140h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 140h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 13ch], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 134h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 138h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 138h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 134h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 144h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 148h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 148h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 144h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 12ch
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 130h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 130h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 12ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 14fh], 50h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 151h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 156h], 40h
        push   40h
        call   sub_1_02E3
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 152h], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 152h], 0
        jne    NEAR PTR loc_1_2E88
        mov    eax, 1
        jmp    NEAR PTR loc_1_2EED
loc_1_2E88:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3a6h
        push   eax
        call   sub_1_5F31
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_2EBF
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        or     al, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 451h], al
        jmp    NEAR PTR loc_1_2EDA
loc_1_2EBF:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3dfh
        push   eax
        call   sub_1_5F01
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_2EED
loc_1_2EDA:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1B0A
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_2EED
loc_1_2EED:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2EF2:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        or     al, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 451h], al
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 17h
        jne    NEAR PTR loc_1_2F38
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 110h], 0
loc_1_2F38:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 2
        je     NEAR PTR loc_1_2F68
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 3
        je     NEAR PTR loc_1_2F68
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 4
        jne    NEAR PTR loc_1_2F72
loc_1_2F68:
        call   sub_1_4C75
        jmp    NEAR PTR loc_1_2F38
loc_1_2F72:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 121h]
        and    eax, 0ffh
        test   al, 7
        je     NEAR PTR loc_1_2FBF
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 14fh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 410h
        push   eax
        call   sub_1_5F19
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        add    eax, 410h
        push   eax
        call   sub_1_5F43
        add    esp, 4
        jmp    NEAR PTR loc_1_30B7
loc_1_2FBF:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 5
        jne    NEAR PTR loc_1_30B7
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 110h], 7
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 144h
        cmp    dword ptr [eax + 144h], ecx
        jne    NEAR PTR loc_1_300C
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 134h
        cmp    dword ptr [eax + 134h], ecx
        je     NEAR PTR loc_1_301A
loc_1_300C:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_614C
        add    esp, 8
loc_1_301A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4FDF
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 152h]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 156h], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 14fh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 410h
        push   eax
        call   sub_1_5F19
        add    esp, 8
        cmp    eax, 2
        jne    NEAR PTR loc_1_30B7
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 110h], 8
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_614C
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 110h], 9
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 121h]
        and    eax, 0ffh
        test   al, 7
        je     NEAR PTR loc_1_30B7
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 410h
        push   eax
        call   sub_1_5F43
        add    esp, 4
loc_1_30B7:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 410h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 121h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 110h], 9
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 10h
        je     NEAR PTR loc_1_3127
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3a6h
        push   eax
        call   sub_1_5F37
        add    esp, 8
        cmp    eax, 2
        je     NEAR PTR loc_1_3127
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0efh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 451h], al
loc_1_3127:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 10h
        je     NEAR PTR loc_1_314B
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_614C
        add    esp, 8
loc_1_314B:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3dfh
        push   eax
        call   sub_1_5F01
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 152h]
        push   eax
        call   sub_1_034B
        add    esp, 4
loc_1_316E:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 13ch
        cmp    dword ptr [eax + 13ch], ecx
        je     NEAR PTR loc_1_31BA
        mov    eax, dword ptr [ebp + 8]
        add    eax, 13ch
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0bh]
        push   eax
        call   sub_1_1DB6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D0A
        add    esp, 4
        jmp    NEAR PTR loc_1_316E
loc_1_31BA:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 134h
        cmp    dword ptr [eax + 134h], ecx
        je     NEAR PTR loc_1_3206
        mov    eax, dword ptr [ebp + 8]
        add    eax, 134h
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0bh]
        push   eax
        call   sub_1_1DB6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D0A
        add    esp, 4
        jmp    NEAR PTR loc_1_31BA
loc_1_3206:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 12ch
        cmp    dword ptr [eax + 12ch], ecx
        je     NEAR PTR loc_1_3252
        mov    eax, dword ptr [ebp + 8]
        add    eax, 12ch
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0bh]
        push   eax
        call   sub_1_1DB6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D0A
        add    esp, 4
        jmp    NEAR PTR loc_1_3206
loc_1_3252:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_325A
loc_1_325A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_325F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 6e0h
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        or     al, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 451h], al
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 17h
        jne    NEAR PTR loc_1_32A5
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 110h], 0
loc_1_32A5:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 2
        je     NEAR PTR loc_1_32D5
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 3
        je     NEAR PTR loc_1_32D5
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 4
        jne    NEAR PTR loc_1_32E1
loc_1_32D5:
        mov    dword ptr [ebp - 4], 6e1h
        jmp    NEAR PTR loc_1_335F
loc_1_32E1:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 5
        jne    NEAR PTR loc_1_335F
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 104h]
        and    eax, 0ffh
        test   al, 40h
        jne    NEAR PTR loc_1_3314
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 58h], 0
        je     NEAR PTR loc_1_335F
loc_1_3314:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 121h]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_3336
        mov    dword ptr [ebp - 4], 6e2h
        jmp    NEAR PTR loc_1_335F
loc_1_3336:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 121h]
        and    eax, 0ffh
        test   al, 6
        je     NEAR PTR loc_1_3358
        mov    dword ptr [ebp - 4], 6e3h
        jmp    NEAR PTR loc_1_335F
loc_1_3358:
        mov    dword ptr [ebp - 4], 6e1h
loc_1_335F:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_3367
loc_1_3367:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_336C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_1_3375:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 14dh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_33C0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 13ch
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_38FA
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 116h]
        jmp    NEAR PTR loc_1_3375
loc_1_33C0:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 116h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_33E3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_33E8
        add    esp, 4
loc_1_33E3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_33E8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 450h]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_3418
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_35B5
        add    esp, 8
        jmp    NEAR PTR loc_1_342B
loc_1_3418:
        push   0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 410h
        push   eax
        call   sub_1_5F2B
        add    esp, 8
loc_1_342B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_3430:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_343B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_3444:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 13ch
        cmp    dword ptr [eax + 13ch], ecx
        jne    NEAR PTR loc_1_3461
        jmp    NEAR PTR loc_1_358D
loc_1_3461:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 13ch
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 120h]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_34CF
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0fh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 58h]
        and    ecx, 0ffffh
        sub    eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 58h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        add    eax, dword ptr [ecx + 0bh]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5B35
        add    esp, 10h
        jmp    NEAR PTR loc_1_350E
loc_1_34CF:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0fh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 58h]
        and    ecx, 0ffffh
        sub    eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 58h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        add    eax, dword ptr [ecx + 0bh]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5A92
        add    esp, 10h
loc_1_350E:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 58h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 58h], ax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 58h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0fh]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_356E
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_38FA
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 116h]
        jmp    NEAR PTR loc_1_3583
loc_1_356E:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 13ch
        push   eax
        call   sub_1_4B5D
        add    esp, 8
loc_1_3583:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_3444
loc_1_358D:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 116h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_35B0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_33E8
        add    esp, 4
loc_1_35B0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_35B5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 0
        jne    NEAR PTR loc_1_35ED
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        test   al, 8
        je     NEAR PTR loc_1_35ED
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1B0A
        add    esp, 4
loc_1_35ED:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 6
        je     NEAR PTR loc_1_360D
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 9
        jne    NEAR PTR loc_1_3628
loc_1_360D:
        mov    eax, dword ptr [ebp + 8]
        or     dword ptr [eax + 34h], 400h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4C3C
        add    esp, 4
        jmp    NEAR PTR loc_1_3716
loc_1_3628:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 5
        je     NEAR PTR loc_1_364D
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 110h], 7
        je     NEAR PTR loc_1_364D
        jmp    NEAR PTR loc_1_3716
loc_1_364D:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 108h]
        and    eax, 0ffh
        test   al, 40h
        je     NEAR PTR loc_1_36AB
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 120h]
        and    eax, 0ffh
        test   al, 1
        jne    NEAR PTR loc_1_368F
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 450h]
        and    eax, 0ffh
        test   al, 4
        je     NEAR PTR loc_1_36AB
loc_1_368F:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 11eh]
        and    eax, 0ffffh
        or     eax, 40h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 11eh], ax
loc_1_36AB:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 120h]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_3701
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 450h]
        and    eax, 0ffh
        test   al, 4
        je     NEAR PTR loc_1_36EC
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_39EE
        add    esp, 8
        jmp    NEAR PTR loc_1_36FC
loc_1_36EC:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_394F
        add    esp, 8
loc_1_36FC:
        jmp    NEAR PTR loc_1_3711
loc_1_3701:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_394F
        add    esp, 8
loc_1_3711:
        jmp    NEAR PTR loc_1_3716
loc_1_3716:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_371B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_373E
        mov    eax, dword ptr [ebp + 8]
        add    eax, 14ch
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_3753
loc_1_373E:
        cmp    dword ptr [ebp + 0ch], 1
        jne    NEAR PTR loc_1_3753
        mov    eax, dword ptr [ebp + 8]
        add    eax, 14dh
        mov    dword ptr [ebp - 4], eax
loc_1_3753:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        shr    ecx, 1
        cmp    eax, ecx
        jae    NEAR PTR loc_1_37E6
        mov    eax, dword ptr [ebp - 4]
        inc    byte ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        add    eax, 12ch
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 5ah], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 58h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0bh]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0bh]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 450h]
        and    eax, 0ffh
        test   al, 4
        je     NEAR PTR loc_1_37D0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0fh], 2
        jmp    NEAR PTR loc_1_37D9
loc_1_37D0:
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0fh], 0
loc_1_37D9:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_37ED
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_37E6:
        xor    eax, eax
        jmp    NEAR PTR loc_1_37ED
loc_1_37ED:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_37F2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_3815
        mov    eax, dword ptr [ebp + 8]
        add    eax, 134h
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_383A
loc_1_3815:
        cmp    dword ptr [ebp + 0ch], 1
        jne    NEAR PTR loc_1_382F
        mov    eax, dword ptr [ebp + 8]
        add    eax, 13ch
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_383A
loc_1_382F:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 144h
        mov    dword ptr [ebp - 4], eax
loc_1_383A:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax], ecx
        je     NEAR PTR loc_1_3873
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3886
loc_1_3873:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_371B
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
loc_1_3886:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_388E
loc_1_388E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_3893:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_1_38B6
        mov    eax, dword ptr [ebp + 8]
        add    eax, 134h
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_38DB
loc_1_38B6:
        cmp    dword ptr [ebp + 10h], 1
        jne    NEAR PTR loc_1_38D0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 13ch
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_38DB
loc_1_38D0:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 144h
        mov    dword ptr [ebp - 4], eax
loc_1_38DB:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_38F5
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_5F70
        add    esp, 8
loc_1_38F5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_38FA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_1_3918
        mov    eax, dword ptr [ebp + 8]
        dec    byte ptr [eax + 14ch]
        jmp    NEAR PTR loc_1_392B
loc_1_3918:
        cmp    dword ptr [ebp + 10h], 1
        jne    NEAR PTR loc_1_392B
        mov    eax, dword ptr [ebp + 8]
        dec    byte ptr [eax + 14dh]
loc_1_392B:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_394A
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 12ch
        push   eax
        call   sub_1_5F70
        add    esp, 8
loc_1_394A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_394F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 104h]
        and    eax, 0ffh
        test   al, 40h
        jne    NEAR PTR loc_1_3998
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 58h], 0
        jne    NEAR PTR loc_1_3998
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 134h
        cmp    dword ptr [eax + 134h], ecx
        jne    NEAR PTR loc_1_3998
        jmp    NEAR PTR loc_1_39E9
loc_1_3998:
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_37F2
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_39B8
        jmp    NEAR PTR loc_1_39E9
loc_1_39B8:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E00
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3893
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4050
        add    esp, 4
loc_1_39E9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_39EE:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 11eh]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_3A65
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 104h]
        and    eax, 0ffh
        test   al, 40h
        jne    NEAR PTR loc_1_3A65
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 58h], 0
        jne    NEAR PTR loc_1_3A65
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 116h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_3A65
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 134h
        cmp    dword ptr [eax + 134h], ecx
        jne    NEAR PTR loc_1_3A65
        jmp    NEAR PTR loc_1_3BE0
loc_1_3A65:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3BB8
loc_1_3A70:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3BE5
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4050
        add    esp, 4
        jmp    NEAR PTR loc_1_3BE0
loc_1_3A8D:
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_37F2
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_3AAD
        jmp    NEAR PTR loc_1_3BE0
loc_1_3AAD:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E00
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3893
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4050
        add    esp, 4
        jmp    NEAR PTR loc_1_3BE0
loc_1_3AE3:
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_37F2
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_3B03
        jmp    NEAR PTR loc_1_3BE0
loc_1_3B03:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3CBE
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3893
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4050
        add    esp, 4
        jmp    NEAR PTR loc_1_3BE0
loc_1_3B39:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3BE5
        add    esp, 4
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_37F2
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_3B65
        jmp    NEAR PTR loc_1_3BE0
loc_1_3B65:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3CBE
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E00
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3893
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4050
        add    esp, 4
        jmp    NEAR PTR loc_1_3BE0
        db  0E9h, 02Dh, 000h, 000h, 000h, 0E9h, 028h, 000h, 000h, 000h
loc_1_3BB8:
        cmp    dword ptr [ebp - 8], 4
        ja     NEAR PTR loc_1_3BE0
        mov    eax, dword ptr [ebp - 8]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_3BE0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_3BE5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 134h
        cmp    dword ptr [eax + 134h], ecx
        je     NEAR PTR loc_1_3C4F
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 134h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0bh]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 116h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0bh]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 116h], 0
        jmp    NEAR PTR loc_1_3CB9
        db  0E9h, 06Ah, 000h, 000h, 000h
loc_1_3C4F:
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_371B
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_3C6F
        jmp    NEAR PTR loc_1_3CB9
loc_1_3C6F:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0bh]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 116h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0bh]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 116h], 0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3893
        add    esp, 0ch
        jmp    NEAR PTR loc_1_3CB9
loc_1_3CB9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_3CBE:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 11eh]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_3DF3
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 5ah]
        and    eax, 0ffffh
        test   al, 2
        jne    NEAR PTR loc_1_3D6A
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 114h]
        and    eax, 0ffffh
        sub    eax, 40h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 0fh]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_3D6A
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0fh]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 8
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 11eh]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_42D9
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 0fh], ax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 11eh], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 5ah]
        and    eax, 1
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 5ah], ax
        jmp    NEAR PTR loc_1_3DF3
loc_1_3D6A:
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3893
        add    esp, 0ch
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_371B
        add    esp, 8
        mov    dword ptr [ebp + 0ch], eax
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_3DA4
        mov    eax, dword ptr [ebp + 0ch]
        jmp    NEAR PTR loc_1_3DFB
        db  0E9h, 04Fh, 000h, 000h, 000h
loc_1_3DA4:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0fh]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 8
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 11eh]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_42D9
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 0fh], ax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 11eh], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 5ah]
        and    eax, 1
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 5ah], ax
loc_1_3DF3:
        mov    eax, dword ptr [ebp + 0ch]
        jmp    NEAR PTR loc_1_3DFB
loc_1_3DFB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_3E00:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_3E1B
        mov    eax, dword ptr [ebp + 0ch]
        jmp    NEAR PTR loc_1_404B
loc_1_3E1B:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 104h]
        and    eax, 0ffh
        test   al, 40h
        je     NEAR PTR loc_1_3EEB
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 114h]
        and    eax, 0ffffh
        dec    eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 0fh]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jg     NEAR PTR loc_1_3E8B
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3893
        add    esp, 0ch
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_371B
        add    esp, 8
        mov    dword ptr [ebp + 0ch], eax
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_3E8B
        mov    eax, dword ptr [ebp + 0ch]
        jmp    NEAR PTR loc_1_404B
loc_1_3E8B:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 106h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 0fh]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [edx + 0bh]
        mov    byte ptr [ecx + edx], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0fh]
        and    eax, 0ffffh
        inc    eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 0fh], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 5ah]
        and    eax, 0ffffh
        or     eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 5ah], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 104h]
        and    eax, 0bfh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 104h], al
loc_1_3EEB:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 58h], 0
        je     NEAR PTR loc_1_4043
        mov    dword ptr [ebp - 4], 0
loc_1_3EFF:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 5ah]
        and    eax, 0ffffh
        or     eax, 2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 5ah], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 120h]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_3F82
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 114h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 0fh]
        and    ecx, 0ffffh
        sub    eax, ecx
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0fh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        add    eax, dword ptr [ecx + 0bh]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5D7A
        add    esp, 10h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5E54
        add    esp, 8
        jmp    NEAR PTR loc_1_3FD4
loc_1_3F82:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 114h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 0fh]
        and    ecx, 0ffffh
        sub    eax, ecx
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0fh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        add    eax, dword ptr [ecx + 0bh]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5D20
        add    esp, 10h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5E0B
        add    esp, 8
loc_1_3FD4:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0fh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 0fh], ax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 58h], 0
        je     NEAR PTR loc_1_4039
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3893
        add    esp, 0ch
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_371B
        add    esp, 8
        mov    dword ptr [ebp + 0ch], eax
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_4034
        mov    eax, dword ptr [ebp + 0ch]
        jmp    NEAR PTR loc_1_404B
loc_1_4034:
        jmp    NEAR PTR loc_1_403E
loc_1_4039:
        jmp    NEAR PTR loc_1_4043
loc_1_403E:
        jmp    NEAR PTR loc_1_3EFF
loc_1_4043:
        mov    eax, dword ptr [ebp + 0ch]
        jmp    NEAR PTR loc_1_404B
loc_1_404B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4050:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_4059:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 14eh]
        and    eax, 0ffh
        cmp    eax, 2
        jge    NEAR PTR loc_1_41AA
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 134h
        cmp    dword ptr [eax + 134h], ecx
        je     NEAR PTR loc_1_41AA
        mov    eax, dword ptr [ebp + 8]
        add    eax, 134h
        push   eax
        call   sub_1_4B2A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0fh]
        and    eax, 0ffffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_40BD
        mov    eax, dword ptr [ebp - 4]
        dec    word ptr [eax + 0fh]
        jmp    NEAR PTR loc_1_40EE
loc_1_40BD:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 11ah]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_40EE
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 134h
        push   eax
        call   sub_1_4B5D
        add    esp, 8
        jmp    NEAR PTR loc_1_41AA
loc_1_40EE:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 410h
        push   eax
        call   sub_1_5F1F
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 2
        je     NEAR PTR loc_1_4152
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_4152
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 134h
        push   eax
        call   sub_1_4B5D
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        or     dword ptr [eax + 34h], 400h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4C3C
        add    esp, 4
        jmp    NEAR PTR loc_1_41AA
        db  0E9h, 053h, 000h, 000h, 000h
loc_1_4152:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0fh]
        and    eax, 0ffffh
        cmp    eax, 1
        jle    NEAR PTR loc_1_4187
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 450h]
        and    eax, 0ffh
        test   al, 4
        je     NEAR PTR loc_1_4187
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 11ah]
loc_1_4187:
        mov    eax, dword ptr [ebp + 8]
        inc    byte ptr [eax + 14eh]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 144h
        push   eax
        call   sub_1_5F70
        add    esp, 8
        jmp    NEAR PTR loc_1_4059
loc_1_41AA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_41AF:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 7fh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 11ah], ax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        test   al, 80h
        jne    NEAR PTR loc_1_41FE
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_42D4
loc_1_41FE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        inc    eax
        add    dword ptr [ebp - 0ch], eax
        inc    dword ptr [ebp - 8]
loc_1_4216:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 8], eax
        jae    NEAR PTR loc_1_42CC
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_1_42B8
loc_1_4244:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 4], al
        inc    dword ptr [ebp - 8]
loc_1_4256:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_1_4294
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 18h], eax
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_428C
        xor    eax, eax
        jmp    NEAR PTR loc_1_42D4
loc_1_428C:
        dec    byte ptr [ebp - 4]
        jmp    NEAR PTR loc_1_4256
loc_1_4294:
        jmp    NEAR PTR loc_1_42C7
loc_1_4299:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        inc    eax
        add    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_42C7
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_1_42B8:
        cmp    dword ptr [ebp - 1ch], 1
        je     NEAR PTR loc_1_4244
        jmp    NEAR PTR loc_1_4299
loc_1_42C7:
        jmp    NEAR PTR loc_1_4216
loc_1_42CC:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_42D4
loc_1_42D4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_42D9:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 3]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 450h]
        and    eax, 0ffh
        test   al, 4
        jne    NEAR PTR loc_1_4336
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 120h]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_1_432C
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 3fh
        mov    word ptr [ebp + 0ch], ax
        jmp    NEAR PTR loc_1_4336
loc_1_432C:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 1
        mov    word ptr [ebp + 0ch], ax
loc_1_4336:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        test   al, 1
        je     NEAR PTR loc_1_4375
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 0
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 1
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 450h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        inc    dword ptr [ebp + 14h]
loc_1_4375:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        test   al, 2
        je     NEAR PTR loc_1_43EB
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4B90
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 10h
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 4
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp - 0ch]
        shr    eax, 18h
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        mov    eax, dword ptr [ebp - 0ch]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx + 1], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx + 2], ah
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx + 3], al
        add    dword ptr [ebp + 14h], 4
loc_1_43EB:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        test   al, 4
        je     NEAR PTR loc_1_442D
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 11h
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 1
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 10fh]
        and    eax, 3fh
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        inc    dword ptr [ebp + 14h]
loc_1_442D:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        test   al, 8
        je     NEAR PTR loc_1_4520
        mov    byte ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 9dh], 2
        je     NEAR PTR loc_1_445B
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        or     al, 1
        mov    byte ptr [ebp - 10h], al
loc_1_445B:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 9dh], 1
        je     NEAR PTR loc_1_4475
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        or     al, 2
        mov    byte ptr [ebp - 10h], al
loc_1_4475:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 9dh], 40h
        je     NEAR PTR loc_1_448F
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        or     al, 4
        mov    byte ptr [ebp - 10h], al
loc_1_448F:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 9ch], 8
        je     NEAR PTR loc_1_44A9
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        or     al, 8
        mov    byte ptr [ebp - 10h], al
loc_1_44A9:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 9dh], 20h
        je     NEAR PTR loc_1_44C3
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        or     al, 10h
        mov    byte ptr [ebp - 10h], al
loc_1_44C3:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 9ch], 10h
        je     NEAR PTR loc_1_44DD
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        or     al, 20h
        mov    byte ptr [ebp - 10h], al
loc_1_44DD:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 9ch], 60h
        je     NEAR PTR loc_1_44F7
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        or     al, 40h
        mov    byte ptr [ebp - 10h], al
loc_1_44F7:
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 12h
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 1
        inc    dword ptr [ebp + 14h]
        mov    al, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        inc    dword ptr [ebp + 14h]
loc_1_4520:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        test   al, 10h
        je     NEAR PTR loc_1_4574
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 13h
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 2
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0adh]
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0aeh]
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        inc    dword ptr [ebp + 14h]
loc_1_4574:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        test   al, 20h
        je     NEAR PTR loc_1_45C8
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 14h
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 2
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0aeh]
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0adh]
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        inc    dword ptr [ebp + 14h]
loc_1_45C8:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        test   al, 40h
        je     NEAR PTR loc_1_463B
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 16h
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 1
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 108h]
        and    eax, 0ffh
        test   al, 40h
        je     NEAR PTR loc_1_4617
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 1
        jmp    NEAR PTR loc_1_4621
loc_1_4617:
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 0
loc_1_4621:
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 108h]
        and    eax, 0bfh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 108h], al
loc_1_463B:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        test   ah, 1
        je     NEAR PTR loc_1_4692
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 20h
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 1
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 452h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], al
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 452h]
        and    eax, 0fch
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 452h], al
loc_1_4692:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        test   ah, 4
        je     NEAR PTR loc_1_46BD
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 22h
        inc    dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 0
        inc    dword ptr [ebp + 14h]
loc_1_46BD:
        mov    eax, dword ptr [ebp + 14h]
        sub    eax, dword ptr [ebp - 4]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 8]
        mov    cl, byte ptr [ecx + 1]
        and    ecx, 0ffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 14h]
        jmp    NEAR PTR loc_1_46E4
loc_1_46E4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_46E9:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        inc    eax
        add    dword ptr [ebp + 10h], eax
        inc    dword ptr [ebp - 8]
loc_1_4710:
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp - 8], eax
        jae    NEAR PTR loc_1_49B2
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_494F
loc_1_473E:
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 4], al
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 123h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        and    eax, ecx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_1_4784
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 450h], al
loc_1_4784:
        jmp    NEAR PTR loc_1_49AD
loc_1_4789:
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 4], al
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 120h]
        and    eax, 0ffh
        test   al, 1
        jne    NEAR PTR loc_1_47CA
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 450h]
        and    eax, 0ffh
        test   al, 4
        je     NEAR PTR loc_1_47DA
loc_1_47CA:
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_51DA
        add    esp, 8
loc_1_47DA:
        jmp    NEAR PTR loc_1_49AD
loc_1_47DF:
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 4], al
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 120h]
        and    eax, 0ffh
        test   al, 1
        jne    NEAR PTR loc_1_4820
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 450h]
        and    eax, 0ffh
        test   al, 4
        je     NEAR PTR loc_1_4830
loc_1_4820:
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5186
        add    esp, 8
loc_1_4830:
        jmp    NEAR PTR loc_1_49AD
loc_1_4835:
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 4], al
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 450h]
        and    eax, 0ffh
        test   al, 4
        je     NEAR PTR loc_1_4896
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4A4A
        add    esp, 8
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_501C
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 11eh]
        and    eax, 0fbffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 11eh], ax
loc_1_4896:
        jmp    NEAR PTR loc_1_49AD
loc_1_489B:
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp - 4], al
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 450h]
        and    eax, 0ffh
        test   al, 4
        je     NEAR PTR loc_1_48EF
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_501C
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 11eh]
        and    eax, 0fbffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 11eh], ax
loc_1_48EF:
        jmp    NEAR PTR loc_1_49AD
loc_1_48F4:
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 450h]
        and    eax, 0ffh
        test   al, 4
        je     NEAR PTR loc_1_492B
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 11eh]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 11eh], ax
loc_1_492B:
        jmp    NEAR PTR loc_1_49AD
loc_1_4930:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        inc    eax
        add    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_49AD
        db  0E9h, 05Eh, 000h, 000h, 000h
loc_1_494F:
        cmp    dword ptr [ebp - 10h], 22h
        ja     NEAR PTR loc_1_4930
        mov    eax, dword ptr [ebp - 10h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 006h, 006h, 006h
        db  006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h
        db  006h, 001h, 002h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 003h, 004h, 005h
loc_1_49AD:
        jmp    NEAR PTR loc_1_4710
loc_1_49B2:
        mov    eax, dword ptr [ebp + 10h]
        jmp    NEAR PTR loc_1_49BA
loc_1_49BA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_49BF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   3
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f4h]
        push   eax
        call   sub_1_4AC0
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_49FF
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 120h], 2
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 123h], 3
        jmp    NEAR PTR loc_1_4A45
loc_1_49FF:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 120h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 123h], 7
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4A21:
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_4A45
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 451h]
        and    eax, 0ffh
        or     al, 80h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 451h], al
loc_1_4A45:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4A4A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 0
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        test   al, 8
        je     NEAR PTR loc_1_4A6E
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 10h
        mov    byte ptr [ebp - 4], al
loc_1_4A6E:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        test   al, 2
        je     NEAR PTR loc_1_4A85
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 1
        mov    byte ptr [ebp - 4], al
loc_1_4A85:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        test   al, 4
        je     NEAR PTR loc_1_4A9C
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 0a0h
        mov    byte ptr [ebp - 4], al
loc_1_4A9C:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        test   al, 1
        je     NEAR PTR loc_1_4AB3
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 0ah
        mov    byte ptr [ebp - 4], al
loc_1_4AB3:
        mov    al, byte ptr [ebp - 4]
        jmp    NEAR PTR loc_1_4ABB
loc_1_4ABB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4AC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_4AC9:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_4B0A
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        inc    dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 8]
        mov    cl, byte ptr [ecx]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_4B0A
        dec    dword ptr [ebp + 10h]
        jmp    NEAR PTR loc_1_4AC9
loc_1_4B0A:
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_1_4B1E
        mov    eax, 1
        jmp    NEAR PTR loc_1_4B20
loc_1_4B1E:
        xor    eax, eax
loc_1_4B20:
        jmp    NEAR PTR loc_1_4B25
loc_1_4B25:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4B2A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_4B58
loc_1_4B58:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4B5D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh
sub_1_4B90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 98h]
        cmp    eax, 1c200h
        je     SHORT loc_1_4BC6
        cmp    eax, 0ff10h
        jb     SHORT loc_1_4BC6
        mov    ebx, eax
        sub    ebx, 0ff10h
        shl    ebx, 1
        db  00Fh, 0B7h, 08Bh, 000h, 000h, 000h, 000h
        xor    edx, edx
        mov    eax, 1c200h
        div    ecx
loc_1_4BC6:
        pop    ebx
        pop    esi
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        pop    esi
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        pop    esi
        leave
        ret
sub_1_4BED:
        pushad
        or     byte ptr [esi + 0cdh], al
        mov    ebx, dword ptr [esi + 0f0h]
        push   dword ptr [ebx]
        push   eax
        cmp    eax, 4
        jne    SHORT loc_1_4C11
        push   dword ptr [esi + 0c8h]
        push   esi
        call   dword ptr [esi + 0c4h]
        jmp    SHORT loc_1_4C37
loc_1_4C11:
        cmp    eax, 1
        jne    SHORT loc_1_4C25
        push   dword ptr [esi + 0d4h]
        push   esi
        call   dword ptr [esi + 0d0h]
        jmp    SHORT loc_1_4C37
loc_1_4C25:
        cmp    eax, 2
        jne    SHORT loc_1_4C37
        push   dword ptr [esi + 0dch]
        push   esi
        call   dword ptr [esi + 0d8h]
loc_1_4C37:
        add    esp, 10h
        popad
        ret
sub_1_4C3C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 0f0h]
        bts    dword ptr [ebx], 9
        jb     SHORT loc_1_4C70
        test   byte ptr [esi + 0cdh], 40h
        je     SHORT loc_1_4C70
        test   dword ptr [esi + 0ech], 200h
        je     SHORT loc_1_4C70
        mov    eax, 4
        call   sub_1_4BED
loc_1_4C70:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_1_4C75:
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
        ret
sub_1_4CA8:
        cld
        and    dword ptr [esi + 34h], 0fffffeffh
        mov    eax, dword ptr [esi + 48h]
        sub    eax, dword ptr [esi + 58h]
        jle    SHORT loc_1_4D1D
        cmp    ecx, eax
        jbe    SHORT loc_1_4CC5
        mov    ecx, eax
        or     dword ptr [esi + 34h], 100h
loc_1_4CC5:
        push   ecx
        mov    eax, dword ptr [esi + 60h]
        xor    edx, edx
        add    eax, ecx
        sub    eax, dword ptr [esi + 48h]
        jbe    SHORT loc_1_4CD6
        mov    edx, eax
        sub    ecx, edx
loc_1_4CD6:
        push   esi
        mov    eax, dword ptr [esi + 44h]
        mov    esi, dword ptr [esi + 60h]
        add    esi, eax
        xchg   edi, esi
        cld
        rep movsb
        mov    ecx, edx
        jecxz  loc_1_4CEC
        mov    edi, eax
        rep movsb
loc_1_4CEC:
        pop    esi
        sub    edi, eax
        cmp    edi, dword ptr [esi + 48h]
        jb     SHORT loc_1_4CF6
        xor    edi, edi
loc_1_4CF6:
        mov    dword ptr [esi + 60h], edi
        pop    eax
        add    dword ptr [esi + 58h], eax
        mov    ebx, dword ptr [esi + 0e4h]
        cmp    dword ptr [esi + 58h], ebx
        jb     SHORT loc_1_4D0F
        and    byte ptr [esi + 0cdh], 0fdh
loc_1_4D0F:
        push   eax
        push   3
        push   esi
        call   sub_1_35B5
        add    esp, 8
        pop    eax
        ret
loc_1_4D1D:
        or     eax, eax
        je     SHORT loc_1_4D2A
        or     dword ptr [esi + 34h], 100h
        xor    eax, eax
loc_1_4D2A:
        jmp    SHORT loc_1_4D0F
sub_1_4D2C:
        or     ebx, ebx
        je     SHORT loc_1_4D77
        xor    eax, eax
        test   byte ptr [esi + 120h], 2
        jne    SHORT loc_1_4D69
        mov    al, byte ptr [esi + 10dh]
        and    al, byte ptr [esi + 10ah]
        xor    al, byte ptr [esi + 10ah]
        shr    al, 4
        mov    ah, byte ptr [esi + 108h]
        and    ah, 18h
        or     al, ah
        mov    ah, byte ptr [esi + 104h]
        and    ah, 60h
        or     al, ah
        movzx  eax, al
loc_1_4D69:
        mov    dword ptr [ebx], eax
        mov    ecx, dword ptr [esi + 4ch]
        mov    dword ptr [ebx + 4], ecx
        mov    ecx, dword ptr [esi + 58h]
        mov    dword ptr [ebx + 8], ecx
loc_1_4D77:
        ret
sub_1_4D78:
        test   byte ptr [esi + 108h], 12h
        je     SHORT loc_1_4DC7
        cmp    ecx, dword ptr [esi + 0a0h]
        ja     SHORT loc_1_4DC7
        test   byte ptr [esi + 108h], 2
        je     SHORT loc_1_4DB0
        mov    ah, byte ptr [esi + 109h]
        or     ah, ah
        je     SHORT loc_1_4DB0
        mov    al, byte ptr [esi + 10eh]
        or     al, ah
        and    byte ptr [esi + 108h], 0fdh
        call   sub_1_5036
loc_1_4DB0:
        test   byte ptr [esi + 108h], 10h
        je     SHORT loc_1_4DC7
        and    byte ptr [esi + 108h], 0efh
        or     byte ptr [esi + 108h], 4
loc_1_4DC7:
        ret
sub_1_4DC8:
        mov    dword ptr [esi + 94h], 30h
        test   cl, 1
        je     SHORT loc_1_4DE8
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 98h], eax
        or     word ptr [esi + 11eh], 2
loc_1_4DE8:
        test   cl, 70h
        je     SHORT loc_1_4E10
        mov    al, byte ptr [ebx + 16h]
        mov    byte ptr [esi + 0aah], al
        mov    al, byte ptr [ebx + 17h]
        mov    byte ptr [esi + 0abh], al
        mov    al, byte ptr [ebx + 18h]
        mov    byte ptr [esi + 0ach], al
        or     word ptr [esi + 11eh], 4
loc_1_4E10:
        test   cl, 80h
        je     SHORT loc_1_4E26
        mov    al, byte ptr [ebx + 19h]
        mov    byte ptr [esi + 0adh], al
        or     word ptr [esi + 11eh], 30h
loc_1_4E26:
        test   ch, 1
        je     SHORT loc_1_4E3C
        mov    al, byte ptr [ebx + 1ah]
        mov    byte ptr [esi + 0aeh], al
        or     word ptr [esi + 11eh], 30h
loc_1_4E3C:
        test   cl, 2
        je     SHORT loc_1_4E5C
        mov    eax, dword ptr [ebx + 8]
        xchg   dword ptr [esi + 9ch], eax
        xor    eax, dword ptr [ebx + 8]
        test   eax, 6378h
        je     SHORT loc_1_4E5C
        or     word ptr [esi + 11eh], 8
loc_1_4E5C:
        push   esi
        lea    esi, [esi + 94h]
        test   cl, 4
        je     SHORT loc_1_4E6E
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 0ch], eax
loc_1_4E6E:
        test   cl, 8
        je     SHORT loc_1_4E79
        mov    eax, dword ptr [ebx + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_4E79:
        test   ch, 2
        je     SHORT loc_1_4E84
        mov    al, byte ptr [ebx + 1bh]
        mov    byte ptr [esi + 1bh], al
loc_1_4E84:
        test   ch, 4
        je     SHORT loc_1_4E8F
        mov    al, byte ptr [ebx + 1ch]
        mov    byte ptr [esi + 1ch], al
loc_1_4E8F:
        test   ch, 8
        je     SHORT loc_1_4E9A
        mov    al, byte ptr [ebx + 1dh]
        mov    byte ptr [esi + 1dh], al
loc_1_4E9A:
        test   ch, 10h
        je     SHORT loc_1_4EA5
        mov    al, byte ptr [ebx + 1eh]
        mov    byte ptr [esi + 1eh], al
loc_1_4EA5:
        test   ch, 20h
        je     SHORT loc_1_4EB0
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [esi + 20h], eax
loc_1_4EB0:
        test   ch, 80h
        je     SHORT loc_1_4EBB
        mov    eax, dword ptr [ebx + 28h]
        mov    dword ptr [esi + 28h], eax
loc_1_4EBB:
        test   ch, 40h
        je     SHORT loc_1_4EC6
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [esi + 24h], eax
loc_1_4EC6:
        test   ecx, 10000h
        je     SHORT loc_1_4ED4
        mov    eax, dword ptr [ebx + 2ch]
        mov    dword ptr [esi + 2ch], eax
loc_1_4ED4:
        mov    ax, word ptr [ebx + 14h]
        mov    word ptr [esi + 14h], ax
        pop    esi
        ret
sub_1_4EDE:
        push   edx
        mov    ecx, dword ptr [ebx + 4]
        xor    eax, eax
        cmp    ecx, 1c200h
        je     SHORT loc_1_4EFB
        cmp    ecx, 0ff10h
        jae    SHORT loc_1_4F02
        cmp    ecx, 2
        jae    SHORT loc_1_4EFB
loc_1_4EF9:
        mov    ecx, eax
loc_1_4EFB:
        mov    eax, 0fffffff4h
        pop    edx
        ret
loc_1_4F02:
        cmp    ecx, 0ff1fh
        ja     SHORT loc_1_4EF9
        push   ebx
        mov    ebx, ecx
        sub    ebx, 0ff10h
        shl    ebx, 1
        db  00Fh, 0B7h, 083h, 000h, 000h, 000h, 000h
        pop    ebx
        jmp    SHORT loc_1_4EF9
sub_1_4F1F:
        movzx  eax, word ptr [ebx + 16h]
        cmp    ah, 4
        ja     SHORT loc_1_4F6B
        mov    edi, 0ff1eh
        or     ah, ah
        jne    SHORT loc_1_4F37
        xor    edi, 404h
loc_1_4F37:
        cmp    al, 8
        ja     SHORT loc_1_4F64
        sub    al, 5
        jb     SHORT loc_1_4F64
        add    ah, ah
        je     SHORT loc_1_4F45
        dec    ah
loc_1_4F45:
        shl    ah, 3
        or     al, ah
        or     al, 4
        mov    ah, byte ptr [ebx + 18h]
        or     ah, ah
        js     SHORT loc_1_4F6B
        je     SHORT loc_1_4F60
        sub    ah, 2
        je     SHORT loc_1_4F62
        jns    SHORT loc_1_4F6B
        test   al, 3
        jne    SHORT loc_1_4F6B
loc_1_4F60:
        and    al, 0fbh
loc_1_4F62:
        clc
        ret
loc_1_4F64:
        mov    eax, 0fffffff5h
        stc
        ret
loc_1_4F6B:
        mov    eax, 0fffffffbh
        stc
        ret
sub_1_4F72:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 120h], 1
        je     SHORT loc_1_4FDB
        and    byte ptr [esi + 108h], 0f7h
        test   byte ptr [esi + 450h], 4
        je     SHORT loc_1_4FBA
        mov    ah, 3
        mov    al, byte ptr [esi + 10eh]
        test   al, 2
        je     SHORT loc_1_4FA2
        or     ah, 8
loc_1_4FA2:
        test   al, 1
        je     SHORT loc_1_4FA9
        or     ah, 4
loc_1_4FA9:
        mov    byte ptr [esi + 452h], ah
        or     word ptr [esi + 11eh], 100h
        jmp    SHORT loc_1_4FDB
loc_1_4FBA:
        xor    ah, ah
        mov    al, byte ptr [esi + 10eh]
        test   al, 2
        je     SHORT loc_1_4FC9
        or     ah, 11h
loc_1_4FC9:
        test   al, 1
        je     SHORT loc_1_4FD0
        or     ah, 0aah
loc_1_4FD0:
        mov    byte ptr [esi + 10dh], ah
        call   sub_1_50F0
loc_1_4FDB:
        pop    ebx
        pop    esi
        leave
        ret
sub_1_4FDF:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 120h], 1
        je     SHORT loc_1_5018
        or     byte ptr [esi + 108h], 8
        xor    ah, ah
        mov    al, byte ptr [esi + 10eh]
        test   al, 2
        je     SHORT loc_1_5006
        or     ah, 1
loc_1_5006:
        test   al, 1
        je     SHORT loc_1_500D
        or     ah, 0ah
loc_1_500D:
        mov    byte ptr [esi + 10dh], ah
        call   sub_1_50F0
loc_1_5018:
        pop    ebx
        pop    esi
        leave
        ret
sub_1_501C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0ch]
        mov    byte ptr [esi + 10dh], al
        call   sub_1_50F0
        pop    ebx
        pop    esi
        leave
        ret
sub_1_5036:
        mov    ah, al
        xor    ah, byte ptr [esi + 10eh]
        and    ah, 3
        mov    byte ptr [esi + 10eh], al
        and    al, 3
        shl    al, 2
        or     al, ah
        mov    ah, byte ptr [esi + 109h]
        or     ah, ah
        je     SHORT loc_1_5076
        push   eax
        mov    al, byte ptr [esi + 10eh]
        and    al, ah
        cmp    al, ah
        je     SHORT loc_1_506E
        or     byte ptr [esi + 108h], 2
        jmp    SHORT loc_1_5075
loc_1_506E:
        and    byte ptr [esi + 108h], 0fdh
loc_1_5075:
        pop    eax
loc_1_5076:
        xor    ah, ah
        cmp    dword ptr [esi + 110h], 5
        je     SHORT loc_1_50A4
        push   eax
        call   sub_1_50E0
        pop    eax
        and    al, 5
        cmp    al, 5
        jne    SHORT loc_1_50A2
        cmp    dword ptr [esi + 110h], 0
        jne    SHORT loc_1_50A2
        pushad
        push   esi
        call   sub_1_1B0A
        add    esp, 4
        popad
loc_1_50A2:
        jmp    SHORT loc_1_50EF
loc_1_50A4:
        test   byte ptr [esi + 450h], 4
        je     SHORT loc_1_50BE
        mov    byte ptr [esi + 452h], al
        or     word ptr [esi + 11eh], 100h
        jmp    SHORT loc_1_50EF
loc_1_50BE:
        test   al, 8
        je     SHORT loc_1_50C5
        or     ah, 10h
loc_1_50C5:
        test   al, 2
        je     SHORT loc_1_50CC
        or     ah, 1
loc_1_50CC:
        test   al, 4
        je     SHORT loc_1_50D6
        or     ah, 20h
        or     ah, 80h
loc_1_50D6:
        test   al, 1
        je     SHORT sub_1_50E0
        or     ah, 2
        or     ah, 8
sub_1_50E0:
        push   ebx
        push   ecx
        mov    byte ptr [esi + 10dh], ah
        call   sub_1_50F0
        pop    ecx
        pop    ebx
loc_1_50EF:
        ret
sub_1_50F0:
        mov    al, byte ptr [esi + 10dh]
        mov    ebx, dword ptr [esi + 0fch]
        mov    byte ptr [ebx], al
        mov    ebx, dword ptr [esi + 0f0h]
        mov    ecx, dword ptr [ebx]
        push   ecx
        mov    ch, al
        movzx  eax, al
        mov    ah, al
        shr    eax, 2
        shr    ah, 1
        shr    eax, 3
        and    eax, 138h
        or     dword ptr [ebx], eax
        mov    ah, ch
        shr    ah, 2
        and    eax, 0c00h
        or     dword ptr [ebx], eax
        mov    ah, ch
        shr    ah, 3
        and    eax, 1000h
        or     dword ptr [ebx], eax
        mov    ah, ch
        shl    ah, 3
        and    eax, 2000h
        or     dword ptr [ebx], eax
        mov    al, byte ptr [esi + 10ah]
        or     al, al
        je     SHORT loc_1_5161
        and    ch, al
        cmp    ch, al
        je     SHORT loc_1_515A
        or     byte ptr [esi + 108h], 20h
        jmp    SHORT loc_1_5161
loc_1_515A:
        and    byte ptr [esi + 108h], 5fh
loc_1_5161:
        mov    eax, dword ptr [esi + 0ech]
        and    eax, dword ptr [ebx]
        mov    dword ptr [ebx], eax
        pop    ebx
        test   byte ptr [esi + 0cdh], 40h
        je     SHORT loc_1_5185
        not    ebx
        and    eax, ebx
        je     SHORT loc_1_5185
        mov    eax, 4
        call   sub_1_4BED
loc_1_5185:
        ret
sub_1_5186:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0ch]
        and    ah, 1
        je     SHORT loc_1_51D6
        mov    ah, 10h
        and    ah, byte ptr [esi + 107h]
        je     SHORT loc_1_51D6
        or     byte ptr [esi + 34h], ah
        mov    ebx, dword ptr [esi + 0f0h]
        mov    ecx, dword ptr [ebx]
        push   ecx
        or     byte ptr [ebx], 80h
        or     byte ptr [ebx], 40h
        mov    eax, dword ptr [esi + 0ech]
        and    eax, dword ptr [ebx]
        mov    dword ptr [ebx], eax
        pop    ebx
        test   byte ptr [esi + 0cdh], 40h
        je     SHORT loc_1_51D6
        not    ebx
        and    eax, ebx
        je     SHORT loc_1_51D6
        mov    eax, 4
        call   sub_1_4BED
loc_1_51D6:
        pop    ebx
        pop    esi
        leave
        ret
sub_1_51DA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0ch]
        and    ah, 0eh
        and    ah, byte ptr [esi + 107h]
        je     SHORT loc_1_5223
        or     byte ptr [esi + 34h], ah
        mov    ebx, dword ptr [esi + 0f0h]
        mov    ecx, dword ptr [ebx]
        push   ecx
        or     byte ptr [ebx], 80h
        mov    eax, dword ptr [esi + 0ech]
        and    eax, dword ptr [ebx]
        mov    dword ptr [ebx], eax
        pop    ebx
        test   byte ptr [esi + 0cdh], 40h
        je     SHORT loc_1_5223
        not    ebx
        and    eax, ebx
        je     SHORT loc_1_5223
        mov    eax, 4
        call   sub_1_4BED
loc_1_5223:
        pop    ebx
        pop    esi
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        test   byte ptr [esi + 108h], 60h
        jne    SHORT loc_1_525B
        test   byte ptr [esi + 9ch], 60h
        jne    SHORT loc_1_5252
        test   byte ptr [esi + 108h], 4
        je     SHORT loc_1_5252
        and    byte ptr [esi + 108h], 0ebh
loc_1_5252:
        test   byte ptr [esi + 108h], 8
        je     SHORT loc_1_525C
loc_1_525B:
        inc    eax
loc_1_525C:
        pop    esi
        leave
        ret
_LTEXT ends
        end
