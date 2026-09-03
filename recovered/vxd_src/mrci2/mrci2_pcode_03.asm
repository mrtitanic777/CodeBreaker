; mrci2_pcode_03.asm -- MRCI2 object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        cmp    word ptr [ebp + 1ch], 4a12h
        jne    NEAR PTR loc_3_009C
        mov    cx, word ptr [ebp + 18h]
        mov    dx, word ptr [ebp + 14h]
        cmp    cx, 4d52h
        jne    SHORT loc_3_0070
        cmp    dx, 4349h
        jne    SHORT loc_3_0070
        db  083h, 03Dh, 028h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_3_0143
        db  0A1h, 034h, 000h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_3_0143
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        bt     dword ptr [ebx], 5
        cmc
        sbb    ebx, ebx
        and    eax, ebx
        xchg   cl, ch
        xchg   dl, dh
        xchg   cx, dx
        mov    word ptr [ebp + 18h], cx
        mov    word ptr [ebp + 14h], dx
        mov    word ptr [ebp], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        db  0A1h, 028h, 000h, 000h, 000h
        mov    word ptr [ebp + 10h], ax
        clc
        ret
loc_3_0070:
        mov    edx, dword ptr [ebp + 14h]
        cmp    edx, 4d524349h
        jne    SHORT loc_3_009C
        db  083h, 03Dh, 0BAh, 000h, 000h, 000h, 000h
        je     SHORT loc_3_009C
        xchg   dl, dh
        rol    edx, 10h
        xchg   dl, dh
        mov    dword ptr [ebp + 14h], edx
        db  0C7h, 045h, 000h, 0A8h, 000h, 000h, 000h
        db  0E8h, 0D2h, 000h, 0FDh, 0FFh
        clc
        ret
loc_3_009C:
        cmp    word ptr [ebp + 1ch], 4a20h
        jne    NEAR PTR loc_3_0143
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        test   byte ptr [ebx], 20h
        db  08Bh, 015h, 09Ch, 000h, 000h, 000h
        mov    bx, 400h
        jne    SHORT loc_3_00C5
        db  08Bh, 015h, 008h, 000h, 000h, 000h
        xor    ebx, ebx
loc_3_00C5:
        dec    word ptr [ebp + 18h]
        jl     SHORT loc_3_0103
        xchg   edi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        add    dword ptr [ebp], 14h
        cld
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 9
        rep movsw
        mov    ax, 4d4ah
        stosw
        sub    edi, 14h
        mov    dword ptr [edi + 8], edx
        and    word ptr [edi + 0ch], 0fffch
        and    word ptr [edi + 0eh], 0fffch
        or     word ptr [edi + 0ch], bx
loc_3_0103:
        dec    word ptr [ebp + 18h]
        jl     SHORT loc_3_0143
        xchg   edi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        add    dword ptr [ebp], 14h
        cld
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 9
        rep movsw
        mov    ax, 5344h
        stosw
        sub    edi, 14h
        mov    dword ptr [edi + 8], edx
        and    word ptr [edi + 0ch], 0ff3fh
        and    word ptr [edi + 0eh], 0ff3fh
        or     word ptr [edi + 0ch], bx
loc_3_0143:
        stc
        ret
sub_3_0145:
        movzx  edx, ax
        shr    eax, 10h
        mov    ecx, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        xchg   ebx, ecx
        test   byte ptr [ecx], 20h
        jne    SHORT loc_3_0163
        shl    eax, 4
        add    eax, edx
loc_3_015F:
        add    eax, dword ptr [ecx + 4]
loc_3_0162:
        ret
loc_3_0163:
        push   ecx
        push   edx
        push   0
        push   eax
        push   ecx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        pop    ecx
        add    eax, 1
        jb     SHORT loc_3_0162
        lea    eax, [edx + eax - 1]
        cmp    eax, 110000h
        jae    SHORT loc_3_0162
        jmp    SHORT loc_3_015F
loc_3_0186:
        call   0
        jb     SHORT loc_3_0194
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        ret
loc_3_0194:
        db  08Bh, 00Dh, 0A4h, 000h, 000h, 000h
        db  08Bh, 015h, 0A0h, 000h, 000h, 000h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_3_01A6:
        movzx  eax, word ptr [ebp + 1ch]
        sub    esp, 14h
        push   eax
        push   eax
        inc    eax
        je     NEAR PTR loc_3_02BC
        dec    eax
        db  085h, 005h, 028h, 000h, 000h, 000h
        je     NEAR PTR loc_3_029D
        xchg   esi, eax
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        lea    edi, [esp + 8]
        mov    ecx, 14h
        cld
        rep movsb
        lea    edi, [esi - 14h]
        lea    esi, [esp + 8]
        movzx  edx, word ptr [esi + 0ch]
        mov    dword ptr [esp + 4], edx
        push   eax
        mov    eax, dword ptr [esi]
        call   sub_3_0145
        mov    edx, eax
        pop    eax
        jb     NEAR PTR loc_3_02B5
        mov    dword ptr [esi], edx
        push   eax
        mov    eax, dword ptr [esi + 8]
        call   sub_3_0145
        mov    edx, eax
        pop    eax
        jb     NEAR PTR loc_3_02B5
        mov    ecx, edx
        mov    dword ptr [esi + 8], edx
        sub    ecx, dword ptr [ebx + 4]
        sbb    edx, edx
        or     ecx, edx
        cmp    ecx, 110000h
        cmc
        sbb    edx, edx
        or     ecx, edx
        test   eax, 200h
        je     SHORT loc_3_0248
        mov    dx, word ptr [edi + 6]
        mov    word ptr [esi + 6], dx
        push   eax
        push   ecx
        mov    eax, dword ptr [esi + 10h]
        call   sub_3_0145
        mov    edx, eax
        pop    ecx
        pop    eax
        jb     SHORT loc_3_02B5
        mov    dword ptr [esi + 10h], edx
loc_3_0248:
        push   eax
        push   ecx
        db  0E8h, 0B1h, 0FDh, 0FCh, 0FFh
        pop    ecx
        pop    eax
        jb     SHORT loc_3_028D
        push   ebx
        push   edx
        push   edi
        db  0E8h, 0B6h, 0FEh, 0FCh, 0FFh
        mov    edx, edi
        pop    edi
        pop    esi
        pop    ebx
        or     eax, eax
        jne    SHORT loc_3_0276
        mov    word ptr [edi + 0ch], dx
        add    word ptr [edi], cx
        add    word ptr [edi + 8], dx
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [edi + 10h], ecx
loc_3_0276:
        mov    word ptr [ebp + 1ch], ax
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
loc_3_0280:
        pop    eax
        pop    edx
        mov    esi, esp
        db  0E8h, 00Ah, 0FEh, 0FCh, 0FFh
        add    esp, 14h
        ret
loc_3_028D:
        mov    word ptr [ebp + 1ch], 4
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        add    esp, 1ch
        ret
loc_3_029D:
        db  0A1h, 02Ch, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_02B5
        rol    eax, 10h
        mov    word ptr [ebp + 28h], ax
        shr    eax, 10h
        mov    dword ptr [ebp + 24h], eax
        jmp    SHORT loc_3_0280
loc_3_02B5:
        mov    eax, 1
        jmp    SHORT loc_3_0276
loc_3_02BC:
        movzx  eax, word ptr [ebp + 10h]
        not    eax
        db  021h, 005h, 028h, 000h, 000h, 000h
        sub    eax, eax
        jmp    SHORT loc_3_0276
loc_3_02CC:
        db  08Bh, 03Dh, 024h, 000h, 000h, 000h
        or     edi, edi
        je     SHORT loc_3_02E3
        db  0BEh, 012h, 000h, 000h, 000h
        mov    ecx, 12h
        cld
        rep movsb
loc_3_02E3:
        ret
_PTEXT ends
        end
