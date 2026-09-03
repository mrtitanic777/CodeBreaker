; msopl_pcode_05.asm -- MSOPL object 5 segment _PTEXT
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
        je     SHORT loc_5_007D
        cmp    eax, 3
        je     SHORT loc_5_008C
        cmp    eax, 4
        je     SHORT loc_5_007D
        mov    eax, 1
        jmp    NEAR PTR loc_5_00B8
loc_5_002F:
        xor    eax, eax
        jmp    NEAR PTR loc_5_00B8
loc_5_0036:
        push   0
        lea    ecx, [ebp - 0d8h]
        push   dword ptr [ebp + 10h]
        push   ecx
        call   sub_5_0490
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_00B8
        cmp    word ptr [ebp - 7ch], 1
        je     SHORT loc_5_005C
        mov    eax, 13h
        jmp    SHORT loc_5_00B8
loc_5_005C:
        mov    ax, word ptr [ebp - 7ah]
        movzx  ecx, ax
        cmp    ecx, -1
        jne    SHORT loc_5_006F
        mov    eax, 13h
        jmp    SHORT loc_5_00B8
loc_5_006F:
        push   eax
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0078:
        add    esp, 8
        jmp    SHORT loc_5_00B8
loc_5_007D:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0085:
        add    esp, 4
        xor    eax, eax
        jmp    SHORT loc_5_00B8
loc_5_008C:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_5_009F
        cmp    eax, 1
        je     SHORT loc_5_009F
        mov    eax, 1
        jmp    SHORT loc_5_00B8
loc_5_009F:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_00A7:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
loc_5_00B8:
        mov    esp, ebp
        pop    ebp
        ret
sub_5_00BC:
        push   eax
        in     al, dx
        jmp    SHORT loc_5_00C0
loc_5_00C0:
        in     al, dx
        jmp    SHORT loc_5_00C3
loc_5_00C3:
        pop    eax
        ret
sub_5_00C5:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_5_00BC
        pop    eax
        inc    dl
        out    dx, al
        call   sub_5_00BC
        pop    edx
        pop    ebx
        ret
sub_5_00E2:
        push   eax
        push   edx
        push   ebx
        push   ecx
        movzx  edx, word ptr [edi + 6]
        mov    ebx, 105h
        mov    al, 1
        call   sub_5_00C5
        mov    ebx, 4
        mov    al, 60h
        call   sub_5_00C5
        mov    ebx, 104h
        mov    al, 3fh
        call   sub_5_00C5
        mov    ebx, 8
        xor    al, al
        call   sub_5_00C5
        mov    ebx, 0bdh
        mov    al, 0c0h
        call   sub_5_00C5
        mov    ecx, 15h
        mov    ebx, 40h
        mov    al, 3fh
loc_5_0132:
        xor    bh, bh
        call   sub_5_00C5
        mov    bh, 1
        call   sub_5_00C5
        inc    bl
        loop   loc_5_0132
        mov    ecx, 8
        mov    ebx, 0b0h
        xor    eax, eax
loc_5_0150:
        xor    bh, bh
        call   sub_5_00C5
        mov    bh, 1
        call   sub_5_00C5
        inc    bl
        loop   loc_5_0150
        mov    ebx, 104h
        xor    eax, eax
        call   sub_5_00C5
        mov    ebx, 105h
        call   sub_5_00C5
        call   sub_5_00BC
        call   sub_5_00BC
        call   sub_5_00BC
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        ret
loc_5_018C:
        push   eax
        mov    eax, dword ptr [esi + 10h]
        cmp    eax, ebx
        je     SHORT loc_5_01A7
        or     eax, eax
        jne    SHORT loc_5_01BA
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        call   sub_5_032E
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_01BA
loc_5_01A7:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_01B3
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_01B3:
        je     SHORT loc_5_01B8
        in     al, dx
        jmp    SHORT loc_5_01B9
loc_5_01B8:
        out    dx, al
loc_5_01B9:
        ret
loc_5_01BA:
        mov    edi, esi
        call   sub_5_0226
        pop    eax
        xor    eax, eax
        dec    eax
        ret
sub_5_01C6:
        push   esi
        push   eax
        push   ecx
        mov    si, 1
        test   word ptr [edi + 4], 1
        je     SHORT loc_5_01D9
        mov    si, 3
loc_5_01D9:
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_01DF:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_01F0
        inc    cx
        jmp    SHORT loc_5_01DF
loc_5_01F0:
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_01F5:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 1
        test   word ptr [edi + 4], 1
        je     SHORT loc_5_0209
        mov    si, 3
loc_5_0209:
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_020F:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0220
        inc    cx
        jmp    SHORT loc_5_020F
loc_5_0220:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_0226:
        test   word ptr [edi + 0ah], 1
        jne    SHORT loc_5_025C
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0233:
        jb     SHORT loc_5_025C
        test   dword ptr [esi], 1
        jne    SHORT loc_5_025C
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        or     dword ptr [esi], 1
        mov    eax, 30h
        xor    esi, esi
        xor    edi, edi
        db  0CDh, 020h, 004h, 000h, 017h, 000h
loc_5_025C:
        ret
loc_5_025D:
        pushfd
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_0295
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_0295
loc_5_0271:
        popfd
        pushfd
        push   eax
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    dword ptr [edi + 10h], ebx
        jne    SHORT loc_5_0289
        mov    dword ptr [edi + 14h], 0ffffffffh
        call   sub_5_030B
loc_5_0289:
        pop    eax
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_0295
        jmp    SHORT loc_5_0271
loc_5_0295:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     SHORT loc_5_02AA
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_5_02AA:
        popfd
        clc
        ret
loc_5_02AD:
        pushad
        pushfd
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    eax, 5
        mov    ecx, 8
        cli
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_5_0303
        mov    dword ptr [edi + ebx], esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_0307
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_0307
loc_5_02DD:
        mov    edi, dword ptr [eax]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_02E4:
        jb     SHORT loc_5_02F0
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_0307
        jmp    SHORT loc_5_02DD
loc_5_02F0:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     SHORT loc_5_0303
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_5_0303:
        popfd
        stc
        jmp    SHORT loc_5_0309
loc_5_0307:
        popfd
        clc
loc_5_0309:
        popad
        ret
sub_5_030B:
        push   eax
        push   edx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0313:
        jb     SHORT loc_5_0329
        cmp    dword ptr [edi + 10h], ebx
        jne    SHORT loc_5_0329
        call   sub_5_01C6
        mov    dword ptr [edi + 10h], 0
        and    dword ptr [esi], 0fffffffeh
loc_5_0329:
        pop    esi
        pop    edx
        pop    eax
        clc
        ret
sub_5_032E:
        push   eax
        push   ecx
        push   edx
        cmp    dword ptr [edi + 10h], 0
        je     SHORT loc_5_033F
        cmp    dword ptr [edi + 10h], ebx
        je     SHORT loc_5_035C
        stc
        jb     SHORT loc_5_035D
loc_5_033F:
        mov    dword ptr [edi + 10h], ebx
        cmp    dword ptr [edi + 14h], ebx
        je     SHORT loc_5_0354
        test   word ptr [edi + 4], 1
        je     SHORT loc_5_0354
        call   sub_5_00E2
loc_5_0354:
        mov    dword ptr [edi + 14h], ebx
        call   sub_5_01F5
loc_5_035C:
        clc
loc_5_035D:
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_5_0361:
        mov    eax, 400h
        clc
        ret
msopl_PM_API:
        movzx  eax, word ptr [ebp + 14h]
        cmp    ah, 2
        jae    SHORT loc_5_0392
        movzx  ecx, ah
        shl    ecx, 3
        db  08Dh, 089h, 000h, 000h, 000h, 000h
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    SHORT loc_5_0392
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     SHORT loc_5_0392
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_5_0392:
        or     word ptr [ebp + 2ch], 1
        ret
loc_5_0398:
        call   sub_5_0361
        mov    word ptr [ebp + 1ch], ax
        clc
        ret
loc_5_03A3:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_03D3
        push   1
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03B3:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_03D3
        mov    ecx, dword ptr [edi + 14h]
        mov    word ptr [ebp + 1ch], 0
        call   sub_5_032E
        jb     SHORT loc_5_03CB
        ret
loc_5_03CB:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_03D9
loc_5_03D3:
        mov    word ptr [ebp + 1ch], 1
loc_5_03D9:
        stc
        ret
loc_5_03DB:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_040E
        push   1
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03EB:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_040E
        mov    ecx, dword ptr [edi + 10h]
        cmp    ecx, ebx
        je     SHORT loc_5_0401
        mov    word ptr [ebp + 1ch], 3
        jmp    SHORT loc_5_0414
loc_5_0401:
        call   sub_5_030B
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_040E:
        mov    word ptr [ebp + 1ch], 1
loc_5_0414:
        stc
        ret
loc_5_0416:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        cmp    ax, 1
        jne    SHORT loc_5_0487
        mov    ecx, dword ptr [ebp + 18h]
        cli
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_0487
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_0487
loc_5_0437:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0ch], ecx
        je     SHORT loc_5_0448
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_0487
        jmp    SHORT loc_5_0437
loc_5_0448:
        popfd
        pushfd
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_0487
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_0487
        cmp    ecx, 1ch
        ja     SHORT loc_5_0487
        mov    ax, word ptr [esi + 4]
        mov    word ptr [edi + 4], ax
        mov    ax, word ptr [esi + 6]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [edi + 0ch], eax
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_5_0487:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
sub_5_0490:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
_PTEXT ends
        end
