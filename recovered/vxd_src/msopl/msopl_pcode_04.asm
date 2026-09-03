; msopl_pcode_04.asm -- MSOPL object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_0000:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_000E:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_4_001B
        cmp    dword ptr [edi + 10h], 0
        jne    SHORT loc_4_0022
loc_4_001B:
        mov    eax, 0
        jmp    SHORT loc_4_0027
loc_4_0022:
        mov    eax, 0ffffffffh
loc_4_0027:
        pop    edi
        pop    ebp
        ret
sub_4_002A:
        push   eax
        push   ecx
        push   esi
        push   edi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     SHORT loc_4_0065
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_4_0065
        push   eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 8
        shr    ecx, 2
        rep stosd
        pop    eax
        mov    edi, dword ptr [esp]
        mov    dword ptr [eax + 4], edi
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        clc
loc_4_0060:
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret
loc_4_0065:
        stc
        jmp    SHORT loc_4_0060
sub_4_0068:
        push   eax
        push   ecx
        push   esi
        push   edi
        pushfd
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esi + ebx]
        or     esi, esi
        je     SHORT loc_4_00A0
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_00A0
loc_4_0083:
        cmp    dword ptr [eax + 4], edi
        je     SHORT loc_4_0092
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_4_00A0
        jmp    SHORT loc_4_0083
loc_4_0092:
        push   eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_4_00A0:
        popfd
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret
sub_4_00A6:
        jmp    SHORT loc_4_00B2
loc_4_00A8:
        mov    esi, 0
        jmp    NEAR PTR loc_4_016B
loc_4_00B2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        pushad
        mov    dword ptr [ebp - 0ch], edx
        movzx  ecx, word ptr [edi]
        mov    eax, ecx
        shl    eax, 2
        add    eax, ecx
        shl    eax, 1
        mov    dword ptr [ebp - 4], eax
        push   101h
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_0165
        mov    dword ptr [ebp - 8], eax
        push   edi
        movzx  ecx, word ptr [edi]
        push   ecx
        mov    edi, dword ptr [ebp - 8]
loc_4_00ED:
        push   ecx
        mov    ecx, 0ah
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        cld
        rep movsb
        pop    ecx
        loop   loc_4_00ED
        pop    ecx
        mov    edi, dword ptr [esp]
        mov    esi, dword ptr [ebp - 8]
        add    edi, 2
loc_4_0109:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [edi + 2]
        sub    eax, esi
        sub    eax, 0ah
        mov    dword ptr [esi + 6], eax
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_4_013C
        add    edi, 6
        add    esi, 0ah
        loop   loc_4_0109
        pop    edi
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esp + 1ch], eax
        jmp    SHORT loc_4_016B
loc_4_013C:
        pop    edi
        movzx  eax, word ptr [edi]
        sub    ecx, eax
        neg    ecx
        jecxz  loc_4_0157
        add    edi, 2
loc_4_0149:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        add    edi, 6
        loop   loc_4_0149
loc_4_0157:
        push   0
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_0165:
        xor    eax, eax
        mov    dword ptr [esp + 1ch], eax
loc_4_016B:
        popad
        add    esp, 0ch
        pop    ebp
        ret
sub_4_0171:
        push   ecx
        push   edx
        push   edi
        push   eax
        movzx  ecx, word ptr [edi]
        add    edi, 2
loc_4_017B:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        add    edi, 6
        loop   loc_4_017B
        pop    eax
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    edi
        pop    edx
        pop    ecx
        ret
sub_4_019A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [ebp - 4], 0
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     NEAR PTR loc_4_0273
        movzx  edx, word ptr [ebp + 0ch]
        push   1
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_01C2:
        add    esp, 8
        or     edi, edi
        jne    NEAR PTR loc_4_0273
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_4_0273
        mov    dword ptr [ebp - 4], eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    dword ptr [eax], 0
        push   1
        push   1ch
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_0273
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0ch], eax
        mov    ax, word ptr [ebp + 0ch]
        mov    word ptr [edi + 6], ax
        mov    ax, word ptr [ebp + 10h]
        mov    word ptr [edi + 4], ax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_021B:
        mov    word ptr [edi + 8], ax
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [ebp + 0ch]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0231:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0231
        mov    edx, dword ptr [esp]
        call   sub_4_00A6
        pop    edi
        mov    dword ptr [edi + 18h], eax
        or     eax, eax
        je     SHORT loc_4_0273
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebx
loc_4_0251:
        call   sub_4_002A
        jb     SHORT loc_4_0272
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_4_0251
        pop    ebx
        mov    dword ptr [esp + 1ch], 0ffffffffh
loc_4_026C:
        popad
        add    esp, 4
        pop    ebp
        ret
loc_4_0272:
        pop    ebx
loc_4_0273:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     SHORT loc_4_027F
        call   sub_4_0289
loc_4_027F:
        mov    dword ptr [esp + 1ch], 0
        jmp    SHORT loc_4_026C
sub_4_0289:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     SHORT loc_4_02D3
        mov    eax, dword ptr [edi + 18h]
        or     eax, eax
        je     SHORT loc_4_02C7
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 6]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_02B2:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_02B2
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 18h]
        call   sub_4_0171
        pop    edi
loc_4_02C7:
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_02D3:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        popad
        add    esp, 4
        pop    ebp
        ret
sub_4_02F0:
        push   ebp
        mov    ebp, esp
        pushad
        mov    dword ptr [esp + 1ch], 13h
        movsx  edx, word ptr [ebp + 0ch]
        call   sub_4_03E8
        jb     SHORT loc_4_0325
        push   eax
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_019A
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_0322
        mov    dword ptr [esp + 1ch], 0
loc_4_0322:
        popad
        pop    ebp
        ret
loc_4_0325:
        mov    dword ptr [esp + 1ch], 24h
        jmp    SHORT loc_4_0322
sub_4_032F:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        pushfd
        push   dword ptr [ebp + 8]
        call   0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_0362
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_034F:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_4_0362
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_4_0362
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0362:
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_4_03AF
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_03AF
loc_4_0378:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0ch], ebx
        je     SHORT loc_4_0389
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_4_03AF
        jmp    SHORT loc_4_0378
loc_4_0389:
        popfd
        pushfd
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebx
loc_4_0392:
        call   sub_4_0068
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_4_0392
        pop    ebx
        mov    edi, eax
        call   sub_4_0289
        mov    eax, 0ffffffffh
loc_4_03AF:
        popfd
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_03B5:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03CC:
        add    esp, 14h
        leave
        clc
        ret
loc_4_03D2:
        mov    eax, ebx
        db  0BBh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 04Ah, 004h
        mov    eax, 0
        stc
        ret
        db  0CCh, 0CCh
sub_4_03E8:
        push   ebx
        push   ecx
        pushfd
        cli
        mov    ebx, 4
        mov    al, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03F8:
        mov    ebx, 4
        mov    al, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0404:
        mov    ebx, 2
        mov    al, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0410:
        mov    ebx, 4
        mov    al, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_041C:
        mov    cx, 4000h
loc_4_0420:
        in     al, dx
        test   al, 40h
        jne    SHORT loc_4_0427
        loop   loc_4_0420
loc_4_0427:
        push   eax
        mov    ebx, 4
        mov    al, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0434:
        mov    ebx, 4
        mov    al, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0440:
        pop    eax
        popfd
        test   al, 40h
        je     SHORT loc_4_0456
        test   al, 6
        jne    SHORT loc_4_0451
        mov    eax, 1
        jmp    SHORT loc_4_0453
loc_4_0451:
        xor    eax, eax
loc_4_0453:
        clc
        jmp    SHORT loc_4_0457
loc_4_0456:
        stc
loc_4_0457:
        pop    ecx
        pop    ebx
        ret
_PTEXT ends
        end
