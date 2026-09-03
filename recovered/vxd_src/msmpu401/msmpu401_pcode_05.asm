; msmpu401_pcode_05.asm -- MSMPU401 object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_0000:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0d8h
        test   eax, eax
        je     SHORT loc_5_002F
        cmp    eax, 1
        je     SHORT loc_5_0036
        cmp    eax, 2
        je     SHORT loc_5_008F
        cmp    eax, 3
        je     SHORT loc_5_009E
        cmp    eax, 4
        je     SHORT loc_5_008F
        mov    eax, 1
        jmp    NEAR PTR loc_5_00CA
loc_5_002F:
        xor    eax, eax
        jmp    NEAR PTR loc_5_00CA
loc_5_0036:
        push   0
        lea    ecx, [ebp - 0d8h]
        push   dword ptr [ebp + 10h]
        push   ecx
        call   sub_5_03F8
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_00CA
        cmp    word ptr [ebp - 7ch], 1
        je     SHORT loc_5_005C
        mov    eax, 13h
        jmp    SHORT loc_5_00CA
loc_5_005C:
        mov    ax, word ptr [ebp - 7ah]
        cmp    word ptr [ebp - 2ah], 0
        mov    cx, 0ffffh
        je     SHORT loc_5_0070
        movzx  cx, byte ptr [ebp - 28h]
loc_5_0070:
        movzx  edx, word ptr [ebp - 7ah]
        cmp    edx, -1
        jne    SHORT loc_5_0080
        mov    eax, 13h
        jmp    SHORT loc_5_00CA
loc_5_0080:
        push   ecx
        push   eax
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_008A:
        add    esp, 0ch
        jmp    SHORT loc_5_00CA
loc_5_008F:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0097:
        add    esp, 4
        xor    eax, eax
        jmp    SHORT loc_5_00CA
loc_5_009E:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_5_00B1
        cmp    eax, 1
        je     SHORT loc_5_00B1
        mov    eax, 1
        jmp    SHORT loc_5_00CA
loc_5_00B1:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_00B9:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
loc_5_00CA:
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh, 0CCh
loc_5_00D0:
        push   eax
        mov    eax, dword ptr [esi + 18h]
        cmp    eax, ebx
        je     SHORT loc_5_00EB
        or     eax, eax
        jne    SHORT loc_5_00FE
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        call   sub_5_0276
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_00FE
loc_5_00EB:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_00F7
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_00F7:
        je     SHORT loc_5_00FC
        in     al, dx
        jmp    SHORT loc_5_00FD
loc_5_00FC:
        out    dx, al
loc_5_00FD:
        ret
loc_5_00FE:
        mov    edi, esi
        call   sub_5_0152
        pop    eax
        xor    eax, eax
        dec    eax
        ret
sub_5_010A:
        push   esi
        push   eax
        push   ecx
        mov    si, 1
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0117:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0128
        inc    cx
        jmp    SHORT loc_5_0117
loc_5_0128:
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_012D:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 1
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_013B:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_014C
        inc    cx
        jmp    SHORT loc_5_013B
loc_5_014C:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_0152:
        test   word ptr [edi + 0ch], 1
        jne    SHORT loc_5_0188
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_015F:
        jb     SHORT loc_5_0188
        test   dword ptr [esi], 1
        jne    SHORT loc_5_0188
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        or     dword ptr [esi], 1
        mov    eax, 30h
        xor    esi, esi
        xor    edi, edi
        db  0CDh, 020h, 004h, 000h, 017h, 000h
loc_5_0188:
        ret
loc_5_0189:
        pushfd
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_01C1
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_01C1
loc_5_019D:
        popfd
        pushfd
        push   eax
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    dword ptr [edi + 18h], ebx
        jne    SHORT loc_5_01B5
        mov    dword ptr [edi + 1ch], 0ffffffffh
        call   sub_5_023A
loc_5_01B5:
        pop    eax
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_01C1
        jmp    SHORT loc_5_019D
loc_5_01C1:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     SHORT loc_5_01D6
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_5_01D6:
        popfd
        clc
        ret
loc_5_01D9:
        pushad
        pushfd
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    eax, 5
        mov    ecx, 8
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_5_0232
        mov    dword ptr [edi + ebx], esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_0236
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_0236
loc_5_020A:
        mov    edi, dword ptr [eax]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0211:
        jb     SHORT loc_5_021F
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_0236
        jmp    SHORT loc_5_020A
loc_5_021F:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     SHORT loc_5_0232
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_5_0232:
        popfd
        stc
        jmp    SHORT loc_5_0238
loc_5_0236:
        popfd
        clc
loc_5_0238:
        popad
        ret
sub_5_023A:
        push   eax
        push   edx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0242:
        jb     SHORT loc_5_0271
        cmp    dword ptr [edi + 18h], ebx
        jne    SHORT loc_5_0271
        call   sub_5_010A
        mov    dword ptr [edi + 18h], 0
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_5_026E
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
loc_5_026E:
        and    dword ptr [esi], 0fffffffeh
loc_5_0271:
        pop    esi
        pop    edx
        pop    eax
        clc
        ret
sub_5_0276:
        push   eax
        push   ecx
        push   edx
        cmp    dword ptr [edi + 18h], 0
        je     SHORT loc_5_0287
        cmp    dword ptr [edi + 18h], ebx
        je     SHORT loc_5_02BE
        stc
        jb     SHORT loc_5_02BF
loc_5_0287:
        mov    dword ptr [edi + 18h], ebx
        cmp    dword ptr [edi + 1ch], ebx
        je     SHORT loc_5_0293
        movzx  edx, word ptr [edi + 6]
loc_5_0293:
        mov    dword ptr [edi + 1ch], ebx
        call   sub_5_012D
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_5_02BE
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 10h
        je     SHORT loc_5_02B8
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_5_02BE
loc_5_02B8:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_5_02BE:
        clc
loc_5_02BF:
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_5_02C3:
        mov    eax, 400h
        clc
        ret
        db  0CCh, 0CCh
msmpu401_PM_API:
        movzx  eax, word ptr [ebp + 14h]
        cmp    ah, 4
        jae    SHORT loc_5_02F6
        movzx  ecx, ah
        shl    ecx, 3
        db  08Dh, 089h, 000h, 000h, 000h, 000h
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    SHORT loc_5_02F6
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     SHORT loc_5_02F6
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_5_02F6:
        or     word ptr [ebp + 2ch], 1
        ret
loc_5_02FC:
        call   sub_5_02C3
        mov    word ptr [ebp + 1ch], ax
        clc
        ret
loc_5_0307:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_0337
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0317:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_0337
        mov    ecx, dword ptr [edi + 1ch]
        mov    word ptr [ebp + 1ch], 0
        call   sub_5_0276
        jb     SHORT loc_5_032F
        ret
loc_5_032F:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_033D
loc_5_0337:
        mov    word ptr [ebp + 1ch], 1
loc_5_033D:
        stc
        ret
loc_5_033F:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_0372
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_034F:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_0372
        mov    ecx, dword ptr [edi + 18h]
        cmp    ecx, ebx
        je     SHORT loc_5_0365
        mov    word ptr [ebp + 1ch], 3
        jmp    SHORT loc_5_0378
loc_5_0365:
        call   sub_5_023A
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_0372:
        mov    word ptr [ebp + 1ch], 1
loc_5_0378:
        stc
        ret
loc_5_037A:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        cmp    ax, 1
        jne    SHORT loc_5_03EF
        mov    ecx, dword ptr [ebp + 18h]
        cli
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_03EF
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_03EF
loc_5_039B:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 10h], ecx
        je     SHORT loc_5_03AC
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_03EF
        jmp    SHORT loc_5_039B
loc_5_03AC:
        popfd
        pushfd
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_03EF
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_03EF
        cmp    ecx, 24h
        ja     SHORT loc_5_03EF
        mov    word ptr [edi + 4], 0
        mov    ax, word ptr [esi + 6]
        mov    word ptr [edi + 6], ax
        mov    al, byte ptr [esi + 8]
        mov    byte ptr [edi + 8], al
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [edi + 10h], eax
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_5_03EF:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
sub_5_03F8:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
_PTEXT ends
        end
