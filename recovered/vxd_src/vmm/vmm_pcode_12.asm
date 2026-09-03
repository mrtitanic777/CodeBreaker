; vmm_pcode_12.asm -- VMM object 12 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

Hook_Device_Service:
        pushad
        movzx  ebx, ax
        shr    eax, 10h
        cmp    eax, 1
        jne    SHORT loc_12_001D
        cmp    ebx, 120h
        jae    SHORT loc_12_001D
        db  00Fh, 0A3h, 01Dh, 011h, 09Bh, 000h, 000h
        jb     SHORT loc_12_004A
loc_12_001D:
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        jecxz  loc_12_004C
        cmp    ebx, dword ptr [ecx + 34h]
        jae    SHORT loc_12_004C
        mov    edx, dword ptr [ecx + 30h]
        lea    edx, [edx + ebx*4]
        mov    edi, esi
        call   sub_12_1960
        pushfd
        cli
        xchg   dword ptr [edx], esi
        mov    dword ptr [esp + 8], esi
        jb     SHORT loc_12_0048
        cmp    dword ptr [edi], 0
        jne    SHORT loc_12_0048
        mov    dword ptr [edi], esi
loc_12_0048:
        popfd
        clc
loc_12_004A:
        popad
        ret
loc_12_004C:
        stc
        jmp    SHORT loc_12_004A
loc_12_004F:
        pushad
        movzx  ebx, ax
        shr    eax, 10h
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        jecxz  loc_12_0079
        cmp    ebx, dword ptr [ecx + 34h]
        jae    SHORT loc_12_0079
        mov    edx, dword ptr [ecx + 30h]
        lea    edi, [edx + ebx*4]
        call   sub_12_19B8
        jb     SHORT loc_12_0079
        or     esi, esi
        je     SHORT loc_12_0079
        xchg   dword ptr [edi], esi
        clc
loc_12_0077:
        popad
        ret
loc_12_0079:
        stc
        jmp    SHORT loc_12_0077
VMMAddImportModuleName:
        pushad
        cld
        lodsb
        movzx  ecx, al
        db  08Bh, 03Dh, 040h, 0C3h, 000h, 000h
loc_12_0088:
        call   sub_12_0B27
        je     SHORT loc_12_00FE
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_12_0088
        db  08Bh, 00Dh, 00Dh, 09Bh, 000h, 000h
        jecxz  loc_12_00C4
        db  08Bh, 01Dh, 009h, 09Bh, 000h, 000h
        dec    esi
        lodsb
loc_12_00A5:
        mov    edi, dword ptr [ebx]
        cmp    dword ptr [edi], -1
        jne    SHORT loc_12_00BF
        cmp    byte ptr [edi + 4], al
        jne    SHORT loc_12_00BF
        lea    edi, [edi + 5]
        push   ecx
        push   esi
        movzx  ecx, al
        repe cmpsb
        pop    esi
        pop    ecx
        je     SHORT loc_12_0118
loc_12_00BF:
        add    ebx, 4
        loop   loc_12_00A5
loc_12_00C4:
        movzx  ecx, al
        call   sub_12_0B41
loc_12_00CC:
        push   eax
        db  08Bh, 00Dh, 00Dh, 09Bh, 000h, 000h
        push   ecx
        inc    ecx
        shl    ecx, 2
        push   0
        push   ecx
        db  0FFh, 035h, 009h, 09Bh, 000h, 000h
        db  0CDh, 020h, 050h, 000h, 001h, 000h
        add    esp, 0ch
        db  0A3h, 009h, 09Bh, 000h, 000h
        pop    ecx
        pop    dword ptr [eax + ecx*4]
        mov    eax, ecx
        inc    eax
        db  0FFh, 005h, 00Dh, 09Bh, 000h, 000h
        jmp    SHORT loc_12_0124
loc_12_00FE:
        mov    eax, edi
        db  08Bh, 00Dh, 00Dh, 09Bh, 000h, 000h
        jecxz  loc_12_00CC
        db  08Bh, 03Dh, 009h, 09Bh, 000h, 000h
        cld
        repne scasd
        jne    SHORT loc_12_00CC
        mov    ebx, edi
        sub    ebx, 4
loc_12_0118:
        mov    eax, ebx
        db  02Bh, 005h, 009h, 09Bh, 000h, 000h
        shr    eax, 2
        inc    eax
loc_12_0124:
        mov    dword ptr [esp + 1ch], eax
        popad
        ret
        db  000h, 000h
sub_12_012C:
        push   eax
        push   ecx
        push   edi
        push   esi
        mov    eax, 4
        mov    ecx, 8
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_12_01B9
        db  003h, 03Dh, 084h, 000h, 000h, 000h
        mov    dword ptr [edi], esi
        mov    eax, 4
        mov    ecx, 4
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_12_01B9
        mov    dword ptr [edi + 4], esi
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_12_01B9
        push   3
        push   0
        push   1
        db  0CDh, 020h, 01Bh, 000h, 017h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_12_017E
        or     byte ptr [edi + 8], 8
loc_12_017E:
        db  0F6h, 005h, 0F9h, 0F8h, 000h, 000h, 001h
        jne    SHORT loc_12_019E
        push   1
        push   0
        push   1
        db  0CDh, 020h, 01Bh, 000h, 017h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_12_019E
        or     byte ptr [edi + 8], 2
loc_12_019E:
        push   4
        push   0
        push   1
        db  0CDh, 020h, 01Bh, 000h, 017h, 000h
        add    esp, 0ch
        and    byte ptr [ebx + 1dh], 0fdh
        or     eax, eax
        jne    SHORT loc_12_01B9
        or     byte ptr [ebx + 1dh], 2
loc_12_01B9:
        pop    esi
        pop    edi
        pop    ecx
        pop    eax
        ret
sub_12_01BE:
        db  0E8h, 086h, 00Dh, 0F4h, 0FFh
        db  003h, 03Dh, 084h, 000h, 000h, 000h
        push   3
        pop    esi
loc_12_01CC:
        xor    eax, eax
        xchg   dword ptr [edi + esi*4 + 14h], eax
        test   eax, eax
        je     SHORT loc_12_01E1
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_12_01E1:
        dec    esi
        jns    SHORT loc_12_01CC
        mov    esi, dword ptr [edi]
        test   esi, esi
        je     SHORT loc_12_0217
loc_12_01EA:
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        je     SHORT loc_12_020F
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [eax + 4]
        add    dword ptr [ebx + 34h], edx
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        push   0
        push   ecx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    SHORT loc_12_01EA
loc_12_020F:
        mov    dword ptr [edi], eax
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_12_0217:
        ret
sub_12_0218:
        pushad
        call   sub_12_01BE
loc_12_021E:
        mov    esi, dword ptr [edi + 4]
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        je     SHORT loc_12_023C
        mov    ecx, dword ptr [eax]
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    esi, ecx
        push   edi
        db  0E8h, 0B2h, 001h, 0F4h, 0FFh
        pop    edi
        jmp    SHORT loc_12_021E
loc_12_023C:
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        popad
        ret
loc_12_0244:
        cmp    al, 2
        db  00Fh, 087h, 0CCh, 0FDh, 0F3h, 0FFh
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        cmp    al, 1
        je     NEAR PTR loc_12_034F
        ja     NEAR PTR loc_12_03BC
        mov    byte ptr [ebp + 1dh], 48h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [ebp + 2ch], 1
        jne    NEAR PTR loc_12_04AA
        movzx  esi, word ptr [ebp + 1ch]
        movzx  edi, word ptr [ebp + 10h]
        shl    esi, 4
        shl    edi, 4
        test   edi, edi
        je     SHORT loc_12_028C
        dec    edi
loc_12_028C:
        push   0
        push   0
        push   0f2h
        push   edi
        push   esi
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        test   word ptr [ebx], 180h
        jne    SHORT loc_12_02AF
        mov    ecx, edi
        shr    ecx, 10h
        inc    ecx
        jmp    SHORT loc_12_02B4
loc_12_02AF:
        mov    ecx, 1
loc_12_02B4:
        push   ecx
        push   0
        push   ecx
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        pop    ecx
        test   eax, eax
        je     NEAR PTR loc_12_04AA
        mov    word ptr [ebp + 14h], ax
        push   edi
        push   esi
        push   ecx
        mov    ecx, eax
        db  0CDh, 020h, 088h, 000h, 001h, 000h
        db  003h, 03Dh, 084h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + 4]
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_12_04AA
        mov    dword ptr [eax], ecx
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        mov    eax, ecx
        pop    ecx
        pop    esi
        pop    edi
loc_12_02FE:
        dec    ecx
        je     NEAR PTR loc_12_049D
        add    eax, 8
        add    esi, 10000h
        push   ecx
        sub    edi, 10000h
        mov    ecx, edi
        cmp    ecx, 10000h
        jb     SHORT loc_12_0324
        mov    ecx, 0ffffh
loc_12_0324:
        push   eax
        push   0
        push   0
        push   0f2h
        push   ecx
        push   esi
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        mov    ecx, dword ptr [esp]
        push   0
        push   eax
        push   edx
        push   ebx
        push   ecx
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
        pop    eax
        pop    ecx
        jmp    SHORT loc_12_02FE
loc_12_034F:
        db  0CDh, 020h, 088h, 000h, 001h, 000h
        db  003h, 03Dh, 084h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + 4]
        movzx  ecx, word ptr [ebp + 14h]
        db  0E8h, 016h, 00Ch, 0F4h, 0FFh
        jb     NEAR PTR loc_12_04AA
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        push   0
        push   ecx
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        shr    eax, 4
        mov    word ptr [ebp + 38h], ax
        mov    cx, 4900h
        xchg   word ptr [ebp + 1ch], cx
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [ebp + 2ch], 1
        jne    NEAR PTR loc_12_04AA
        mov    word ptr [ebp + 1ch], cx
        movzx  esi, word ptr [ebp + 14h]
        db  0E8h, 034h, 000h, 0F4h, 0FFh
        jmp    NEAR PTR loc_12_049D
loc_12_03BC:
        mov    edi, dword ptr [ebp + 14h]
        push   0
        push   edi
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        mov    esi, eax
        shr    eax, 4
        mov    word ptr [ebp + 38h], ax
        test   word ptr [ebx], 180h
        jne    SHORT loc_12_0412
        lsl    ecx, edi
        shr    ecx, 10h
        mov    edx, dword ptr [ebp + 10h]
        shr    edx, 0ch
        push   ecx
        sub    ecx, edx
        pop    edx
        jb     NEAR PTR loc_12_04A8
        je     SHORT loc_12_0412
        lea    edx, [edi + edx*8]
loc_12_03F7:
        push   edx
        push   0
        push   edx
        push   ebx
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        je     NEAR PTR loc_12_04AA
        sub    edx, 8
        dec    ecx
        jne    SHORT loc_12_03F7
loc_12_0412:
        mov    cx, 4a00h
        xchg   word ptr [ebp + 1ch], cx
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [ebp + 2ch], 1
        jne    SHORT loc_12_04AA
        mov    eax, esi
        mov    edx, dword ptr [ebp + 10h]
        shl    edx, 4
        dec    edx
        push   edx
        push   0
        push   0
        push   0f2h
        push   edx
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        push   ebx
        push   edi
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
        pop    edx
        test   word ptr [ebx], 180h
        jne    SHORT loc_12_049D
        mov    eax, esi
        mov    esi, edx
        shr    esi, 10h
        je     SHORT loc_12_049D
        lea    ecx, [edi + esi*8]
        shl    esi, 10h
        lea    eax, [esi + eax]
        and    edx, 0ffffh
        push   ecx
        push   0
        push   0
        push   0f2h
        push   edx
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        pop    ecx
        push   0
        push   eax
        push   edx
        push   ebx
        push   ecx
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
loc_12_049D:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        db  0E9h, 075h, 0FBh, 0F3h, 0FFh
loc_12_04A8:
        jmp    SHORT loc_12_04AA
loc_12_04AA:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        db  0E9h, 063h, 0FBh, 0F3h, 0FFh
loc_12_04B5:
        test   al, al
        db  00Fh, 085h, 05Bh, 0FBh, 0F3h, 0FFh
        mov    ax, word ptr [ebp + 10h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 18h]
        mov    cx, word ptr [ebp + 4]
        shl    ecx, 10h
        mov    cx, word ptr [ebp]
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        db  00Fh, 084h, 02Fh, 0FBh, 0F3h, 0FFh
        mov    word ptr [ebp + 18h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 10h], ax
        db  0E9h, 024h, 0FBh, 0F3h, 0FFh
loc_12_04F9:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 1ch]
        test   eax, eax
        jne    SHORT loc_12_050F
        mov    word ptr [ebp + 1ch], 100h
        jmp    SHORT loc_12_051E
loc_12_050F:
        cmp    ax, 100h
        jne    SHORT loc_12_0523
        db  0E8h, 0B5h, 068h, 0F4h, 0FFh
        mov    word ptr [ebp + 1ch], ax
loc_12_051E:
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_12_0523:
        or     byte ptr [ebp + 2ch], 1
        ret
        db  0F6h, 003h, 020h, 074h, 00Eh, 066h, 081h, 07Dh, 010h, 0ADh, 00Bh, 075h, 006h, 066h, 0C7h, 045h
        db  01Ch, 000h, 000h, 0F8h, 0C3h
loc_12_053D:
        cmp    al, 0
        jne    SHORT loc_12_0565
        xchg   esi, eax
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        db  0BFh, 06Ah, 005h, 000h, 000h
        push   15h
        pop    ecx
        cld
        repe cmpsb
        jne    SHORT loc_12_0565
        push   20h
        push   ebx
        db  0CDh, 020h, 019h, 000h, 017h, 000h
        pop    ecx
        pop    ecx
loc_12_0565:
        db  0E9h, 0AEh, 0FAh, 0F3h, 0FFh
loc_12_056A:
        push   eax
        dec    eax
        inc    ecx
        push   edx
        dec    esp
        inc    ecx
        push   eax
        dec    esp
        dec    ecx
        dec    esi
        inc    ebp
        inc    ecx
        push   edx
        pop    edi
        push   edx
        inc    ebp
        dec    ebp
        inc    ecx
        push   eax
        add    al, ch
        push   es
        add    al, byte ptr [eax]
        add    bh, dh
        db  005h, 0A8h, 000h, 000h, 000h
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        jne    NEAR PTR loc_12_0740
        and    byte ptr [ebp + 2ch], 0feh
        mov    eax, dword ptr [ebp + 1ch]
        cmp    al, 3
        ja     NEAR PTR loc_12_0740
        je     NEAR PTR loc_12_0713
        cmp    al, 1
        ja     NEAR PTR loc_12_06E1
        je     NEAR PTR loc_12_0671
        xor    ecx, ecx
        mov    edx, 3
loc_12_05BE:
        db  085h, 015h, 0ACh, 000h, 000h, 000h
        je     SHORT loc_12_05D4
        shl    edx, 2
        inc    ecx
        cmp    ecx, 4
        jb     SHORT loc_12_05BE
        jmp    NEAR PTR loc_12_0740
loc_12_05D4:
        db  0BEh, 098h, 000h, 000h, 000h
        mov    eax, ecx
        shl    eax, 2
        add    esi, eax
        mov    ax, word ptr [ebp + 10h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 18h]
        mov    dword ptr [esi], eax
        db  009h, 015h, 0ACh, 000h, 000h, 000h
        mov    edx, 0fff0ffffh
        shl    cl, 2
        rol    edx, cl
        shr    cl, 2
        db  021h, 015h, 0ACh, 000h, 000h, 000h
        movzx  edx, word ptr [ebp + 14h]
        cmp    dh, 0
        jne    SHORT loc_12_0611
        mov    dl, 1
loc_12_0611:
        cmp    dl, 4
        ja     NEAR PTR loc_12_0740
        cmp    dl, 3
        je     NEAR PTR loc_12_0740
        dec    dl
        js     NEAR PTR loc_12_0740
        cmp    dh, 2
        ja     NEAR PTR loc_12_0740
        jne    SHORT loc_12_0638
        inc    dh
loc_12_0638:
        shl    dl, 2
        or     dl, dh
        xor    dh, dh
        shl    edx, 10h
        shl    cl, 2
        shl    edx, cl
        db  009h, 015h, 0ACh, 000h, 000h, 000h
        shr    cl, 2
        mov    edx, 0fffffffeh
        rol    edx, cl
        db  021h, 015h, 0A8h, 000h, 000h, 000h
        mov    word ptr [ebp + 10h], cx
        db  081h, 00Dh, 0ACh, 000h, 000h, 000h, 000h, 003h, 000h, 000h
        call   sub_12_0745
        ret
loc_12_0671:
        movzx  ecx, word ptr [ebp + 10h]
        cmp    ecx, 4
        jae    NEAR PTR loc_12_0740
        mov    edx, 3
        shl    edx, cl
        shl    edx, cl
        db  085h, 015h, 0ACh, 000h, 000h, 000h
        je     NEAR PTR loc_12_0740
        not    edx
        db  021h, 015h, 0ACh, 000h, 000h, 000h
        mov    edx, 0fffffffeh
        rol    edx, cl
        db  021h, 015h, 0A8h, 000h, 000h, 000h
        db  0BEh, 098h, 000h, 000h, 000h
        shl    ecx, 2
        add    esi, ecx
        mov    dword ptr [esi], 0
        mov    edx, 0fff0ffffh
        rol    edx, cl
        db  021h, 015h, 0ACh, 000h, 000h, 000h
        db  0F7h, 005h, 0ACh, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h
        jne    SHORT loc_12_06DB
        db  081h, 025h, 0ACh, 000h, 000h, 000h, 0FFh, 0FCh, 0FFh, 0FFh
loc_12_06DB:
        call   sub_12_0745
        ret
loc_12_06E1:
        movzx  ecx, word ptr [ebp + 10h]
        cmp    ecx, 4
        jae    SHORT loc_12_0740
        mov    edx, 3
        shl    edx, cl
        shl    edx, cl
        db  085h, 015h, 0ACh, 000h, 000h, 000h
        je     SHORT loc_12_0740
        mov    edx, 1
        shl    edx, cl
        xor    eax, eax
        db  085h, 015h, 0A8h, 000h, 000h, 000h
        je     SHORT loc_12_070E
        inc    al
loc_12_070E:
        mov    word ptr [ebp + 1ch], ax
        ret
loc_12_0713:
        movzx  ecx, word ptr [ebp + 10h]
        cmp    ecx, 4
        jae    SHORT loc_12_0740
        mov    edx, 3
        shl    edx, cl
        shl    edx, cl
        db  085h, 015h, 0ACh, 000h, 000h, 000h
        je     SHORT loc_12_0740
        mov    edx, 0fffffffeh
        rol    edx, cl
        db  021h, 015h, 0A8h, 000h, 000h, 000h
        call   sub_12_0745
        ret
loc_12_0740:
        or     byte ptr [ebp + 2ch], 1
        ret
sub_12_0745:
        push   esi
        push   edx
        push   eax
        cld
        db  0BEh, 098h, 000h, 000h, 000h
        lodsd
        mov    dr0, eax
        lodsd
        mov    dr1, eax
        lodsd
        mov    dr2, eax
        lodsd
        mov    dr3, eax
        lodsd
        and    eax, 0e00fh
        mov    edx, dr6
        and    edx, 0ffff1ff0h
        or     eax, edx
        mov    dr6, eax
        lodsd
        and    eax, 0ffff03ffh
        mov    edx, dr7
        and    edx, 0fc00h
        or     eax, edx
        mov    dr7, eax
        pop    eax
        pop    edx
        pop    esi
        ret
        db  050h, 057h, 0FCh, 0BFh, 098h, 000h, 000h, 000h, 00Fh, 021h, 0C0h, 0ABh, 00Fh, 021h, 0C8h, 0ABh
        db  00Fh, 021h, 0D0h, 0ABh, 00Fh, 021h, 0D8h, 0ABh, 00Fh, 021h, 0F0h, 0ABh, 00Fh, 021h, 0F8h, 0ABh
        db  05Fh, 058h, 0C3h, 000h, 000h, 000h
loc_12_07B0:
        or     eax, eax
        je     SHORT loc_12_07BC
        db  0C6h, 005h, 028h, 001h, 000h, 000h, 000h
        ret
loc_12_07BC:
        db  0C6h, 005h, 028h, 001h, 000h, 000h, 001h
        ret
loc_12_07C4:
        push   ebx
        xor    eax, eax
        dec    eax
        db  0F6h, 005h, 028h, 001h, 000h, 000h, 0FFh
        je     SHORT loc_12_07DD
        db  081h, 03Dh, 0DCh, 0C5h, 000h, 000h, 000h, 000h, 000h, 020h
        jb     SHORT loc_12_081C
loc_12_07DD:
        db  0C6h, 005h, 028h, 001h, 000h, 000h, 000h
        xor    eax, eax
        db  066h, 0F7h, 005h, 026h, 001h, 000h, 000h, 0FFh, 0FFh
        jne    SHORT loc_12_0812
        db  066h, 0C7h, 005h, 024h, 001h, 000h, 000h, 000h, 000h
        mov    ax, 4a16h
        push   2fh
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        or     ax, ax
        jne    SHORT loc_12_0819
        db  066h, 089h, 01Dh, 024h, 001h, 000h, 000h
loc_12_0812:
        db  066h, 0FFh, 005h, 026h, 001h, 000h, 000h
loc_12_0819:
        movsx  eax, ax
loc_12_081C:
        pop    ebx
        ret
loc_12_081E:
        push   ebx
        push   eax
        db  066h, 08Bh, 01Dh, 024h, 001h, 000h, 000h
        or     bx, bx
        je     SHORT loc_12_084A
        db  066h, 0FFh, 00Dh, 026h, 001h, 000h, 000h
        jne    SHORT loc_12_084A
        mov    ax, 4a18h
        push   2fh
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 024h, 001h, 000h, 000h, 000h, 000h
loc_12_084A:
        pop    eax
        pop    ebx
        ret
loc_12_084D:
        pushad
        db  066h, 08Bh, 01Dh, 024h, 001h, 000h, 000h
        or     bx, bx
        je     SHORT loc_12_08B2
        push   eax
        push   ecx
        push   edx
        mov    eax, dword ptr [46ch]
        push   eax
        db  068h, 035h, 001h, 000h, 000h
        db  068h, 029h, 001h, 000h, 000h
        db  0CDh, 020h, 074h, 001h, 001h, 000h
        add    esp, 0ch
        mov    ecx, eax
        db  0BAh, 029h, 001h, 000h, 000h
        mov    ah, 40h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edx
        pop    ecx
        pop    eax
        or     ecx, ecx
        jne    SHORT loc_12_089C
        push   edx
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        pop    ecx
        mov    edx, dword ptr [esp + 14h]
        mov    ecx, eax
loc_12_089C:
        mov    ah, 40h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    ax, 4a21h
        push   2fh
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_12_08B2:
        popad
        ret
Install_V86_Break_Point:
        pushad
        mov    ebx, eax
        db  0A1h, 07Ch, 09Bh, 000h, 000h
        db  08Bh, 00Dh, 074h, 09Bh, 000h, 000h
        sub    eax, ecx
        jbe    SHORT loc_12_0939
        lea    edi, [ecx*8]
        db  003h, 03Dh, 06Ch, 09Bh, 000h, 000h
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi], esi
        mov    esi, ebx
        shr    esi, 10h
        shl    esi, 4
        movzx  ebx, bx
        add    esi, ebx
        mov    bl, 63h
        xchg   byte ptr [esi], bl
        db  08Bh, 00Dh, 07Ch, 09Bh, 000h, 000h
        lea    edi, [ecx*8]
        db  003h, 03Dh, 06Ch, 09Bh, 000h, 000h
        mov    dword ptr [edi], esi
        mov    byte ptr [edi + 7], 0ffh
        mov    byte ptr [edi + 6], bl
        db  0A1h, 074h, 09Bh, 000h, 000h
        mov    word ptr [edi + 4], ax
        db  0FFh, 005h, 074h, 09Bh, 000h, 000h
        db  0FFh, 005h, 078h, 09Bh, 000h, 000h
        db  0FFh, 00Dh, 07Ch, 09Bh, 000h, 000h
        db  083h, 005h, 064h, 09Bh, 000h, 000h, 002h
        db  066h, 0FFh, 005h, 062h, 09Bh, 000h, 000h
        db  066h, 083h, 02Dh, 060h, 09Bh, 000h, 000h, 010h
        popad
        clc
        ret
loc_12_0939:
        popad
        stc
        ret
Remove_V86_Break_Point:
        pushad
        mov    ebx, eax
        shr    eax, 10h
        shl    eax, 4
        movzx  ebx, bx
        add    eax, ebx
        db  08Bh, 03Dh, 070h, 09Bh, 000h, 000h
        db  08Bh, 00Dh, 078h, 09Bh, 000h, 000h
        add    ecx, ecx
        std
        repne scasd
        cld
        jne    SHORT loc_12_096D
        mov    cl, byte ptr [edi + 0ah]
        mov    byte ptr [eax], cl
        mov    dword ptr [edi + 4], 0ffffffffh
        popad
        clc
        ret
loc_12_096D:
        popad
        stc
        ret
loc_12_0970:
        cmp    ch, 20h
        jb     SHORT loc_12_097A
        cmp    ch, 23h
        jbe    SHORT loc_12_09A8
loc_12_097A:
        cmp    ch, 6
        je     SHORT loc_12_09D1
        cmp    ch, 1
        db  00Fh, 085h, 073h, 007h, 0F4h, 0FFh
        test   byte ptr [ebx], 20h
        db  00Fh, 085h, 06Ah, 007h, 0F4h, 0FFh
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        db  00Fh, 084h, 05Eh, 007h, 0F4h, 0FFh
        push   5
        push   ebx
        db  0CDh, 020h, 019h, 000h, 017h, 000h
        pop    ecx
        pop    ecx
loc_12_09A8:
        movzx  esi, byte ptr [esi + 2]
        mov    edi, esi
        and    edi, 38h
        shr    edi, 3
        cmp    edi, 3
        ja     SHORT loc_12_0A1B
        and    esi, 7
        xor    esi, 7
        cmp    ch, 21h
        jne    SHORT loc_12_09D8
        mov    eax, dword ptr [ebx + edi*4 + 1ach]
        mov    dword ptr [ebp + esi*4], eax
        jmp    SHORT loc_12_0A1B
loc_12_09D1:
        clts
        add    dword ptr [ebp + 24h], 2
        ret
loc_12_09D8:
        cmp    ch, 23h
        jne    SHORT loc_12_09EA
        mov    eax, dword ptr [ebp + esi*4]
        mov    dword ptr [ebx + edi*4 + 1ach], eax
        jmp    SHORT loc_12_0A1B
loc_12_09EA:
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        je     SHORT loc_12_0A1B
        test   edi, edi
        jne    SHORT loc_12_0A1B
        mov    dl, ch
        xor    dl, 20h
        test   dl, 0fdh
        jne    SHORT loc_12_0A1B
        shr    dl, 1
        mov    eax, dword ptr [ebp + esi*4]
        shr    eax, 1
        db  0E8h, 01Bh, 008h, 0F4h, 0FFh
        cmp    dl, 0
        jne    SHORT loc_12_0A1B
        and    eax, 3
        shl    eax, 1
        mov    dword ptr [ebp + esi*4], eax
loc_12_0A1B:
        add    dword ptr [ebp + 24h], 3
        ret
loc_12_0A20:
        test   byte ptr [ebp + 2dh], 2
        je     SHORT loc_12_0A3F
        inc    dword ptr [ebp + 24h]
        mov    eax, ebx
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        jecxz  loc_12_0A39
        cmp    eax, ebx
        jne    SHORT loc_12_0A39
        hlt
        ret
loc_12_0A39:
        db  0CDh, 020h, 038h, 080h, 001h, 000h
loc_12_0A3F:
        db  0E9h, 0B7h, 006h, 0F4h, 0FFh
loc_12_0A44:
        db  0C7h, 005h, 0DCh, 0C5h, 000h, 000h, 000h, 000h, 000h, 050h
        db  0F7h, 005h, 054h, 00Ah, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_12_0A5F
        db  0E8h, 0F0h, 041h, 0F4h, 0FFh
loc_12_0A5F:
        clc
        ret
loc_12_0A61:
        db  0C7h, 005h, 0DCh, 0C5h, 000h, 000h, 000h, 000h, 000h, 0A0h
        db  0F7h, 005h, 071h, 00Ah, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_12_0A7C
        db  0E8h, 0F4h, 041h, 0F4h, 0FFh
loc_12_0A7C:
        clc
        ret
loc_12_0A7E:
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        mov    word ptr [ebp + 1ch], 1609h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
        clc
        ret
loc_12_0AC6:
        mov    eax, 3
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        db  08Bh, 03Dh, 09Ch, 0F6h, 000h, 000h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        db  0E9h, 01Eh, 0F8h, 0F3h, 0FFh
loc_12_0AE2:
        db  0A1h, 000h, 0C7h, 000h, 000h
        or     eax, eax
        je     SHORT loc_12_0AF7
        db  08Bh, 015h, 004h, 0C7h, 000h, 000h
        db  08Bh, 00Dh, 008h, 0C7h, 000h, 000h
loc_12_0AF7:
        ret
loc_12_0AF8:
        pushad
        db  08Bh, 00Dh, 054h, 001h, 000h, 000h
        jecxz  loc_12_0B1F
        db  080h, 03Dh, 0A4h, 001h, 000h, 000h, 000h
        je     SHORT loc_12_0B1F
        sub    ecx, 10h
        push   0
        push   ecx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        db  0C6h, 005h, 0A4h, 001h, 000h, 000h, 000h
loc_12_0B1F:
        db  089h, 035h, 054h, 001h, 000h, 000h
        popad
        ret
sub_12_0B27:
        push   edi
        lea    edi, [edi + 0ch]
        push   ecx
        push   esi
        cld
        repe cmpsb
        pop    esi
        jne    SHORT loc_12_0B3E
        pop    ecx
        push   ecx
        sub    ecx, 8
        neg    ecx
        mov    al, 20h
        repe scasb
loc_12_0B3E:
        pop    ecx
        pop    edi
        ret
sub_12_0B41:
        push   ecx
        add    ecx, 5
        push   0
        push   ecx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        mov    edi, eax
        pop    ecx
        mov    dword ptr [edi], 0ffffffffh
        mov    byte ptr [edi + 4], cl
        lea    edi, [edi + 5]
        rep movsb
        ret
loc_12_0B62:
        db  0E8h, 0B1h, 05Ah, 0F4h, 0FFh
        call   sub_12_18D1
        mov    eax, 4
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        db  0C7h, 005h, 0DCh, 0C5h, 000h, 000h, 000h, 000h, 000h, 0E0h
loc_12_0B87:
        db  080h, 03Dh, 069h, 0F6h, 000h, 000h, 004h
        db  00Fh, 082h, 0B0h, 028h, 0F4h, 0FFh
        mov    al, 0ffh
        db  086h, 005h, 0DBh, 0C5h, 000h, 000h
        test   al, al
        jne    SHORT loc_12_0BA5
        call   sub_12_2950
loc_12_0BA5:
        db  0E8h, 0B6h, 0F4h, 0F3h, 0FFh
        xor    eax, eax
        mov    al, 2
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        jecxz  loc_12_0BBA
        or     byte ptr [ecx + 0ah], 3
loc_12_0BBA:
        db  08Bh, 035h, 0DCh, 0C5h, 000h, 000h
        xor    eax, eax
        mov    al, 5
        xor    edx, edx
        inc    edx
        call   sub_12_0C43
        cmp    esi, 0e0000000h
        jb     SHORT loc_12_0BDE
        db  0C7h, 005h, 0DCh, 0C5h, 000h, 000h, 000h, 000h, 000h, 0E4h
loc_12_0BDE:
        xor    eax, eax
        mov    al, 25h
        call   sub_12_0C6D
        cmp    esi, 0e0000000h
        jb     SHORT loc_12_0BF9
        db  0C7h, 005h, 0DCh, 0C5h, 000h, 000h, 000h, 000h, 000h, 0F0h
loc_12_0BF9:
        xor    ecx, ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        cli
        xor    eax, eax
        mov    al, 6
        dec    edx
        call   sub_12_0C43
        cmp    esi, 0e0000000h
        jb     SHORT loc_12_0C1E
        db  0C7h, 005h, 0DCh, 0C5h, 000h, 000h, 000h, 000h, 000h, 0F4h
loc_12_0C1E:
        xor    eax, eax
        mov    al, 26h
        call   sub_12_0C6D
        cmp    esi, 0e0000000h
        jb     SHORT loc_12_0C39
        db  0C7h, 005h, 0DCh, 0C5h, 000h, 000h, 000h, 000h, 0F0h, 0FFh
loc_12_0C39:
        db  0E8h, 00Ah, 0F6h, 0F3h, 0FFh
        db  0E9h, 001h, 028h, 0F4h, 0FFh
sub_12_0C43:
        pushad
        db  0B9h, 040h, 0C3h, 000h, 000h
loc_12_0C49:
        mov    ecx, dword ptr [ecx]
        jecxz  loc_12_0C6B
        bt     dword ptr [ecx + 0ah], edx
        jae    SHORT loc_12_0C49
        push   ecx
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        call   dword ptr [ecx + 18h]
        pop    ecx
        mov    eax, dword ptr [esp + 1ch]
        mov    edx, dword ptr [esp + 14h]
        jmp    SHORT loc_12_0C49
loc_12_0C6B:
        popad
        ret
sub_12_0C6D:
        pushad
        db  0B9h, 080h, 0D9h, 000h, 000h
loc_12_0C73:
        mov    ecx, dword ptr [ecx + 3ch]
        jecxz  loc_12_0C96
        bt     dword ptr [ecx + 0ah], edx
        jae    SHORT loc_12_0C73
        push   ecx
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        call   dword ptr [ecx + 18h]
        pop    ecx
        mov    eax, dword ptr [esp + 1ch]
        mov    edx, dword ptr [esp + 14h]
        jmp    SHORT loc_12_0C73
loc_12_0C96:
        popad
        ret
loc_12_0C98:
        pushad
        or     ecx, ecx
        db  00Fh, 08Fh, 08Bh, 0F4h, 0F3h, 0FFh
        mov    edx, 4
        mov    eax, 0bh
        db  0E8h, 0BBh, 007h, 0F4h, 0FFh
        mov    eax, 28h
        db  0E8h, 001h, 008h, 0F4h, 0FFh
        mov    eax, 0ch
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        db  0E8h, 0A1h, 007h, 0F4h, 0FFh
        mov    eax, 29h
        db  0E8h, 0E7h, 007h, 0F4h, 0FFh
        popad
        db  0E9h, 055h, 0F4h, 0F3h, 0FFh
loc_12_0CDA:
        stc
        ret
loc_12_0CDC:
        push   eax
        push   ecx
        or     ecx, ecx
        jg     SHORT loc_12_0CF6
        mov    eax, 16h
        db  0E8h, 07Fh, 007h, 0F4h, 0FFh
        mov    eax, 2ch
        db  0E8h, 0C5h, 007h, 0F4h, 0FFh
loc_12_0CF6:
        pop    ecx
        pop    eax
        stc
        ret
loc_12_0CFA:
        push   eax
        or     ecx, ecx
        jge    SHORT loc_12_0D13
        mov    eax, 0dh
        db  0E8h, 062h, 007h, 0F4h, 0FFh
        mov    eax, 2ah
        db  0E8h, 0A8h, 007h, 0F4h, 0FFh
loc_12_0D13:
        pop    eax
        stc
        ret
loc_12_0D16:
        push   10h
        jmp    SHORT loc_12_0D1C
loc_12_0D1A:
        push   8
loc_12_0D1C:
        push   ebx
        mov    edx, esp
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xor    eax, eax
        mov    ecx, 0ah
        db  0BEh, 05Bh, 00Dh, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        xor    ecx, ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        dec    ecx
        db  0CDh, 020h, 023h, 000h, 001h, 000h
        db  0CDh, 020h, 021h, 000h, 001h, 000h
        pushad
        xor    esi, esi
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_12_0D5B:
        mov    ebx, dword ptr [edx]
        mov    edx, dword ptr [edx + 4]
        mov    eax, 0bh
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        mov    eax, 0ch
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        ret
loc_12_0D7D:
        pushfd
        pushad
        or     ecx, ecx
        db  00Fh, 08Fh, 0C2h, 0F2h, 0F3h, 0FFh
        mov    eax, 20h
        db  0E8h, 02Ah, 007h, 0F4h, 0FFh
        mov    eax, 21h
        db  0E8h, 020h, 007h, 0F4h, 0FFh
        popad
        popfd
        db  0E9h, 05Eh, 0F2h, 0F3h, 0FFh
loc_12_0DA2:
        db  0A1h, 040h, 0C3h, 000h, 000h
        ret
loc_12_0DA8:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edx
        mov    eax, dword ptr [ebp + 8]
        cmp    word ptr [eax + 4], 400h
        jb     SHORT loc_12_0DD4
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 38h], edx
        or     word ptr [eax + 0ah], 4000h
        movzx  eax, word ptr [eax + 6]
        cmp    eax, 40h
        jae    SHORT loc_12_0DD4
        db  089h, 014h, 085h, 0E0h, 0C5h, 000h, 000h
loc_12_0DD4:
        pop    edx
        pop    eax
        leave
        ret
sub_12_0DD8:
        pushad
        push   1
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   1
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        test   eax, eax
        je     SHORT loc_12_0E4A
        mov    dword ptr [ebx - 38h], eax
        mov    dword ptr [ebx - 30h], edx
        mov    edi, edx
        push   0
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 056h, 000h, 001h, 000h
        add    esp, 10h
        mov    edx, edi
        db  0BEh, 09Ch, 09Bh, 000h, 000h
        mov    ecx, 0c0h
        cld
        rep movsd
        db  0BEh, 00Ch, 0C7h, 000h, 000h
        xchg   edx, edi
        mov    eax, 10h
        mov    ecx, 2fh
loc_12_0E2D:
        db  0E8h, 0E6h, 004h, 0F4h, 0FFh
        inc    eax
        loop   loc_12_0E2D
        mov    edi, edx
        mov    dword ptr [ebx - 8], edi
        db  0BEh, 00Ch, 0C7h, 000h, 000h
        mov    ecx, 200h
        cld
        rep movsd
        popad
        clc
        ret
loc_12_0E4A:
        popad
        stc
        ret
sub_12_0E4D:
        pushad
        db  0C7h, 043h, 0D0h, 09Ch, 09Bh, 000h, 000h
        xor    ecx, ecx
        xchg   dword ptr [ebx - 38h], ecx
        jecxz  loc_12_0E67
        push   0
        push   ecx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_12_0E67:
        popad
        clc
        ret
loc_12_0E6A:
        test   byte ptr [ebp + 2dh], 2
        jne    SHORT loc_12_0E87
        cli
        cmp    dword ptr [ebx - 4], -1
        jne    SHORT loc_12_0E82
        db  0BEh, 088h, 00Eh, 000h, 000h
        db  0CDh, 020h, 019h, 000h, 001h, 000h
loc_12_0E82:
        sti
        mov    byte ptr [ebx - 67h], 0ffh
loc_12_0E87:
        stc
loc_12_0E88:
        ret
Hook_V86_Int_Chain:
        cmp    eax, 0ffh
        ja     NEAR PTR loc_12_0F1D
        pushad
        db  08Bh, 03Dh, 00Ch, 0D3h, 000h, 000h
        or     edi, edi
        je     SHORT loc_12_0EAA
        mov    edx, dword ptr [edi + 1]
        db  089h, 015h, 00Ch, 0D3h, 000h, 000h
        jmp    SHORT loc_12_0EBC
loc_12_0EAA:
        mov    edi, eax
        push   0
        push   17h
        db  0E8h, 077h, 057h, 0F4h, 0FFh
        pop    ecx
        pop    ecx
        test   eax, eax
        je     SHORT loc_12_0F1F
        xchg   edi, eax
loc_12_0EBC:
        mov    edx, esi
        db  0BEh, 010h, 0D3h, 000h, 000h
        mov    ecx, 17h
        cld
        rep movsb
        sub    edi, 17h
        sub    edx, edi
        sub    edx, 5
        mov    dword ptr [edi + 1], edx
        mov    dword ptr [edi + 6], eax
        mov    edx, edi
        db  087h, 014h, 085h, 00Ch, 0CFh, 000h, 000h
        sub    edx, edi
        sub    edx, 16h
        mov    dword ptr [edi + 12h], edx
        cmp    eax, 60h
        jae    SHORT loc_12_0F1A
        cmp    eax, 10h
        jb     SHORT loc_12_0F1A
        sub    eax, 10h
        db  08Dh, 034h, 0C5h, 099h, 030h, 000h, 000h
        mov    ecx, esi
        neg    ecx
        db  081h, 0C1h, 050h, 000h, 000h, 000h
        cmp    ecx, dword ptr [esi - 4]
        jne    SHORT loc_12_0F1A
        mov    ecx, esi
        neg    ecx
        db  081h, 0C1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi - 4], ecx
loc_12_0F1A:
        popad
        clc
        ret
loc_12_0F1D:
        stc
        ret
loc_12_0F1F:
        popad
        stc
        ret
loc_12_0F22:
        cmp    eax, 0ffh
        ja     SHORT loc_12_0F54
        pushad
        xor    ecx, ecx
        db  08Bh, 03Ch, 085h, 00Ch, 0CFh, 000h, 000h
loc_12_0F33:
        db  081h, 0FFh, 03Ch, 015h, 000h, 000h
        je     SHORT loc_12_0F53
        mov    edx, dword ptr [edi + 1]
        add    edx, edi
        add    edx, 5
        cmp    edx, esi
        je     SHORT loc_12_0F56
        mov    ecx, edi
        mov    edx, dword ptr [edi + 12h]
        add    edi, edx
        add    edi, 16h
        jmp    SHORT loc_12_0F33
loc_12_0F53:
        popad
loc_12_0F54:
        stc
        ret
loc_12_0F56:
        mov    edx, dword ptr [edi + 12h]
        add    edx, edi
        add    edx, 16h
        jecxz  loc_12_0F76
        sub    edx, ecx
        sub    edx, 16h
        mov    dword ptr [ecx + 12h], edx
loc_12_0F68:
        mov    eax, edi
        db  087h, 005h, 00Ch, 0D3h, 000h, 000h
        mov    dword ptr [edi + 1], eax
        popad
        clc
        ret
loc_12_0F76:
        db  089h, 014h, 085h, 00Ch, 0CFh, 000h, 000h
        db  081h, 0FAh, 03Ch, 015h, 000h, 000h
        jne    SHORT loc_12_0F68
        cmp    eax, 60h
        jae    SHORT loc_12_0F68
        cmp    eax, 10h
        jb     SHORT loc_12_0F68
        sub    eax, 10h
        db  08Dh, 034h, 0C5h, 099h, 030h, 000h, 000h
        mov    ecx, esi
        neg    ecx
        db  081h, 0C1h, 000h, 000h, 000h, 000h
        cmp    ecx, dword ptr [esi - 4]
        jne    SHORT loc_12_0F68
        mov    ecx, esi
        neg    ecx
        db  081h, 0C1h, 050h, 000h, 000h, 000h
        mov    dword ptr [esi - 4], ecx
        jmp    SHORT loc_12_0F68
Get_PM_Int_Type:
        push   eax
        push   ebx
        shl    eax, 3
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        test   byte ptr [ebx], 40h
        je     SHORT loc_12_0FD2
        cmp    dword ptr [ebx - 38h], 0
        je     SHORT loc_12_0FD2
        add    eax, dword ptr [ebx - 8]
        jmp    SHORT loc_12_0FD8
loc_12_0FD2:
        db  08Dh, 080h, 00Ch, 0C7h, 000h, 000h
loc_12_0FD8:
        movsx  edx, word ptr [eax + 6]
        cmp    dx, 0fcffh
        je     SHORT loc_12_0FE5
        xor    edx, edx
loc_12_0FE5:
        pop    ebx
        pop    eax
        ret
Set_PM_Int_Type:
        pushad
        or     edx, edx
        mov    dx, 0feffh
        je     SHORT loc_12_0FF5
        mov    dx, 0fcffh
loc_12_0FF5:
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        test   byte ptr [ebx], 40h
        je     SHORT loc_12_100E
        cmp    dword ptr [ebx - 38h], 0
        je     SHORT loc_12_100E
        mov    esi, dword ptr [ebx - 8]
        mov    edi, dword ptr [ebx - 30h]
        jmp    SHORT loc_12_1013
loc_12_100E:
        db  0BEh, 00Ch, 0C7h, 000h, 000h
loc_12_1013:
        mov    word ptr [esi + eax*8 + 6], dx
        je     SHORT loc_12_101F
        db  0E8h, 0F9h, 002h, 0F4h, 0FFh
loc_12_101F:
        popad
        ret
        db  000h, 000h, 000h
loc_12_1024:
        mov    dword ptr [ebp + 20h], esi
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        mov    edx, dword ptr [edi + 44h]
        mov    eax, dword ptr [edx + 4]
        mov    eax, dword ptr [eax]
        db  03Dh, 0CEh, 02Ah, 000h, 000h
        je     SHORT loc_12_1043
        db  03Dh, 0CEh, 02Ah, 000h, 000h
        jne    SHORT loc_12_1056
loc_12_1043:
        mov    eax, dword ptr [edx - 0ch]
        db  03Dh, 002h, 02Bh, 000h, 000h
        jne    SHORT loc_12_1056
        mov    eax, dword ptr [edx - 8]
        mov    eax, dword ptr [eax + 0ch]
        sub    edx, 8
loc_12_1056:
        sub    edx, 8
        push   ebp
        push   eax
        clc
        db  0CDh, 020h, 090h, 001h, 001h, 000h
        pop    esi
        pop    ebp
        jb     SHORT loc_12_10D8
        sub    esp, 50h
        mov    edi, 0bh
        mov    eax, dword ptr [ebp + 28h]
        mov    ecx, dword ptr [ebp + 24h]
        call   sub_12_10E7
        inc    edx
        jne    SHORT loc_12_107D
        inc    edi
loc_12_107D:
        sub    esp, 50h
        mov    eax, cs
        mov    ecx, esi
        call   sub_12_10E7
        mov    ecx, esp
        lea    edx, [ecx + 50h]
        db  00Fh, 0B7h, 005h, 01Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        add    edi, 240000h
        push   eax
        push   ecx
        push   esi
        push   edx
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 20h]
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        mov    ecx, eax
        mov    eax, 40001000h
        xor    edi, edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        add    esp, dword ptr [esp]
        add    esp, 0a0h
loc_12_10D8:
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        mov    ebp, dword ptr [edi + 44h]
        sub    ebp, 4
        mov    esp, ebp
        ret
sub_12_10E7:
        mov    edx, cs
        cmp    ax, dx
        jne    SHORT loc_12_1100
        lea    edx, [esp + 4]
        push   edx
        push   ecx
        db  0CDh, 020h, 07Eh, 001h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    SHORT loc_12_1108
loc_12_1100:
        mov    dword ptr [esp + 4], 2d2d2dh
loc_12_1108:
        ret
loc_12_1109:
        clc
        ret
Hook_NMI_Event:
        pushad
        push   esi
        db  08Bh, 035h, 0E8h, 0D9h, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        pop    dword ptr [eax]
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        popad
        ret
loc_12_1123:
        pushad
        db  08Bh, 03Dh, 0E8h, 0D9h, 000h, 000h
        db  0E8h, 0EBh, 075h, 0F4h, 0FFh
        popad
        ret
        db  000h, 000h, 000h
Get_Machine_Info:
        db  00Fh, 0B7h, 005h, 060h, 001h, 000h, 000h
        db  08Bh, 01Dh, 044h, 001h, 000h, 000h
        db  08Bh, 015h, 04Ch, 001h, 000h, 000h
        db  08Bh, 00Dh, 048h, 001h, 000h, 000h
        jecxz  loc_12_1154
        db  0B9h, 062h, 001h, 000h, 000h
loc_12_1154:
        ret
Set_System_Exit_Code:
        db  08Bh, 015h, 098h, 09Bh, 000h, 000h
        mov    byte ptr [edx + 1ch], al
        ret
GetSet_HMA_Info:
        db  0A1h, 098h, 09Bh, 000h, 000h
        jecxz  loc_12_116E
        mov    word ptr [eax + 18h], dx
        xor    eax, eax
        jmp    SHORT loc_12_1178
loc_12_116E:
        movzx  edx, word ptr [eax + 18h]
        mov    ecx, dword ptr [eax]
        movzx  eax, word ptr [eax + 16h]
loc_12_1178:
        ret
Set_Delete_On_Exit_File:
        push   ebx
        db  08Bh, 01Dh, 098h, 09Bh, 000h, 000h
        lea    edi, [ebx + 11dh]
        mov    edx, edi
        mov    ecx, 183h
loc_12_118D:
        cmp    word ptr [edi], 0
        je     SHORT loc_12_1198
        inc    edi
        loop   loc_12_118D
        jmp    SHORT loc_12_11B8
loc_12_1198:
        cmp    edi, edx
        je     SHORT loc_12_11A0
        inc    edi
        dec    ecx
        je     SHORT loc_12_11B8
loc_12_11A0:
        mov    edx, edi
loc_12_11A2:
        cld
        lodsb
        stosb
        or     al, al
        je     SHORT loc_12_11AD
        loop   loc_12_11A2
        jmp    SHORT loc_12_11B5
loc_12_11AD:
        stosb
        or     byte ptr [ebx + 1ah], 2
        clc
loc_12_11B3:
        pop    ebx
        ret
loc_12_11B5:
        mov    byte ptr [edx], 0
loc_12_11B8:
        stc
        jmp    SHORT loc_12_11B3
Get_Debug_Options:
        push   edi
        push   ecx
        push   eax
        db  0BFh, 028h, 002h, 000h, 000h
        mov    ecx, 7dh
        cmp    al, 41h
        jb     SHORT loc_12_11D2
        cmp    al, 5ah
        ja     SHORT loc_12_11D2
        or     al, 20h
loc_12_11D2:
        repne scasb
        pop    eax
        pop    ecx
        pop    edi
        ret
loc_12_11D8:
        xor    eax, eax
        stc
        pushfd
        pushad
        sti
        sub    esp, 50h
        mov    esi, esp
        mov    eax, dword ptr [esp + 74h]
        push   esi
        push   eax
        db  0CDh, 020h, 07Eh, 001h, 001h, 000h
        pop    ecx
        pop    ecx
        db  00Fh, 0B7h, 005h, 01Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   esi
        push   100001h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        xchg   ecx, eax
        xor    edi, edi
        mov    eax, 40001000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        add    esp, dword ptr [esp]
        add    esp, 50h
        popad
        popfd
        ret
        db  000h, 000h, 000h
loc_12_1230:
        xor    eax, eax
        xor    edx, edx
        db  08Bh, 00Dh, 098h, 0F6h, 000h, 000h
        mov    ecx, dword ptr [ecx + 48h]
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        je     SHORT loc_12_1250
        movzx  edx, word ptr [ecx + 2]
        mov    eax, dword ptr [ecx + 4]
        mov    esi, dword ptr [ecx + 0ch]
        mov    ecx, dword ptr [ecx + 8]
loc_12_1250:
        ret
loc_12_1251:
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        push   eax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        and    eax, 1
        inc    word ptr [ebx - 66h]
        cmp    word ptr [ebx - 66h], 1
        ja     SHORT loc_12_12DE
        test   al, 1
        je     SHORT loc_12_1279
        or     byte ptr [ebx], 80h
        jmp    SHORT loc_12_12B7
loc_12_1279:
        push   eax
        push   esi
        db  08Bh, 035h, 098h, 0F6h, 000h, 000h
        movzx  eax, word ptr [esi - 2eh]
        push   0
        push   0
        push   eax
        db  0CDh, 020h, 07Bh, 000h, 001h, 000h
        add    esp, 0ch
        and    edx, 0ffbfffffh
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        mov    word ptr [esi - 30h], ax
        test   eax, eax
        pop    esi
        pop    eax
        je     NEAR PTR loc_12_1403
loc_12_12B7:
        call   sub_12_0DD8
        jb     NEAR PTR loc_12_1459
        or     byte ptr [ebx], 40h
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        db  03Bh, 03Dh, 09Ch, 0F6h, 000h, 000h
        jne    SHORT loc_12_12F1
        movzx  eax, word ptr [ebp + 38h]
        db  0A3h, 0FCh, 0D9h, 000h, 000h
        jmp    SHORT loc_12_12F1
loc_12_12DE:
        mov    ecx, dword ptr [ebx]
        and    ecx, 80h
        shr    ecx, 7
        xor    ecx, eax
        jne    NEAR PTR loc_12_1403
loc_12_12F1:
        movzx  edi, word ptr [ebp + 38h]
        shl    edi, 4
        add    edi, dword ptr [ebx + 4]
        xor    eax, eax
        db  08Bh, 00Dh, 0F8h, 0D9h, 000h, 000h
        shr    ecx, 2
        push   edi
        cld
        rep stosd
        pop    edi
        xor    ecx, ecx
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        mov    word ptr [edi + 8], ax
        mov    word ptr [edi + 0ah], 0cbb0h
        mov    ax, word ptr [ebp + 34h]
        mov    word ptr [edi + 240h], ax
        mov    ax, word ptr [ebp + 30h]
        mov    word ptr [edi + 242h], ax
        push   edi
        add    edi, 0ch
        lea    esi, [ebp + 48h]
        mov    ecx, 9
        cld
        rep movsd
        pop    edi
        mov    ecx, edi
        xchg   dword ptr [ebx - 3ch], ecx
        mov    dword ptr [edi + 4], ecx
        db  0CDh, 020h, 08Ah, 000h, 001h, 000h
        xor    edi, edi
        test   byte ptr [ebx], 80h
        je     SHORT loc_12_135B
        or     edi, 40h
loc_12_135B:
        movzx  eax, word ptr [ebp + 5ch]
        xchg   word ptr [ebp + 58h], ax
        mov    ecx, 240h
        xchg   dword ptr [ebp + 54h], ecx
        mov    dword ptr [ebp + 30h], ecx
        mov    esi, eax
        call   sub_12_147A
        je     NEAR PTR loc_12_1452
        mov    word ptr [ebp + 34h], ax
        movzx  edx, word ptr [ebp + 60h]
        cmp    esi, edx
        je     SHORT loc_12_1394
        mov    eax, edx
        call   sub_12_147A
        je     NEAR PTR loc_12_1441
loc_12_1394:
        mov    word ptr [ebp + 3ch], ax
        mov    eax, dword ptr [esp]
        shl    eax, 4
        push   0
        push   0
        push   0fah
        push   0ffffh
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_12_142A
        mov    word ptr [ebp + 28h], ax
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [ebp + 24h], eax
        xor    eax, eax
        mov    word ptr [ebp + 40h], ax
        mov    word ptr [ebp + 44h], ax
        and    word ptr [ebp + 2ch], 0fffeh
        mov    eax, 15h
        mov    edi, dword ptr [ebx - 3ch]
        call   sub_12_012C
        mov    edx, dword ptr [ebp + 1ch]
        and    edx, 1
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        jb     SHORT loc_12_1419
        add    esp, 8
        ret
loc_12_1403:
        dec    word ptr [ebx - 66h]
        jne    SHORT loc_12_140C
        and    byte ptr [ebx], 3fh
loc_12_140C:
        or     word ptr [ebp + 2ch], 1
        pop    ecx
        pop    edx
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_12_1419:
        movzx  eax, word ptr [ebp + 28h]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        add    esp, 0ch
loc_12_142A:
        movzx  eax, word ptr [ebp + 3ch]
        cmp    ax, word ptr [ebp + 34h]
        je     SHORT loc_12_1441
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        add    esp, 0ch
loc_12_1441:
        movzx  eax, word ptr [ebp + 34h]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        add    esp, 0ch
loc_12_1452:
        call   sub_12_14A7
        jmp    SHORT loc_12_140C
loc_12_1459:
        xor    eax, eax
        push   ebx
        db  08Bh, 01Dh, 098h, 0F6h, 000h, 000h
        xchg   word ptr [ebx - 30h], ax
        pop    ebx
        test   eax, eax
        je     SHORT loc_12_1403
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        add    esp, 0ch
        jmp    SHORT loc_12_1403
sub_12_147A:
        shl    eax, 4
        push   0
        push   edi
        push   0f2h
        push   0ffffh
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        test   eax, eax
        ret
sub_12_14A7:
        pushad
        mov    edi, dword ptr [ebx - 3ch]
        call   sub_12_0218
        mov    dword ptr [edi + 8], edi
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ebx - 3ch], ecx
        db  0CDh, 020h, 089h, 000h, 001h, 000h
        push   edi
        lea    esi, [edi + 0ch]
        lea    edi, [ebp + 48h]
        mov    ecx, 9
        cld
        rep movsd
        pop    edi
        mov    ax, word ptr [edi + 240h]
        mov    word ptr [ebp + 34h], ax
        movzx  eax, word ptr [edi + 242h]
        mov    dword ptr [ebp + 30h], eax
        dec    word ptr [ebx - 66h]
        jl     SHORT loc_12_1510
        jne    SHORT loc_12_150D
        call   sub_12_0E4D
        db  0E8h, 082h, 0ECh, 0F3h, 0FFh
        db  08Bh, 035h, 098h, 0F6h, 000h, 000h
        xor    eax, eax
        mov    word ptr [esi - 30h], ax
        mov    dword ptr [esi + 24h], eax
        db  0E8h, 06Eh, 0EBh, 0F3h, 0FFh
        and    byte ptr [ebx], 3fh
loc_12_150D:
        clc
        popad
        ret
loc_12_1510:
        inc    word ptr [ebx - 66h]
        jmp    SHORT loc_12_150D
loc_12_1516:
        pushad
        cmp    ch, 3
        je     SHORT loc_12_154D
        db  0CDh, 020h, 088h, 000h, 001h, 000h
        cmp    word ptr [edi + 8], dx
        jne    SHORT loc_12_154D
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        mov    eax, 16h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        mov    esi, esp
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        add    esp, 6ch
loc_12_154D:
        popad
        db  0FFh, 025h, 021h, 000h, 000h, 000h
Install_Mult_IO_Handlers:
        pushad
        movzx  ecx, word ptr [edi]
        jecxz  loc_12_1570
        add    edi, 2
loc_12_155D:
        movzx  edx, word ptr [edi]
        mov    esi, dword ptr [edi + 2]
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_12_1573
        add    edi, 6
        loop   loc_12_155D
loc_12_1570:
        popad
        clc
        ret
loc_12_1573:
        mov    dword ptr [esp + 14h], edx
        mov    edi, dword ptr [esp]
        mov    word ptr [edi], 0
        add    edi, 2
loc_12_1582:
        movzx  edx, word ptr [edi]
        cmp    edx, dword ptr [esp + 14h]
        je     SHORT loc_12_1596
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        add    edi, 6
        jmp    SHORT loc_12_1582
loc_12_1596:
        popad
        stc
        ret
loc_12_1599:
        pushad
        movzx  ecx, word ptr [edi]
        jecxz  loc_12_15B0
        add    edi, 2
loc_12_15A2:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        add    edi, 6
        loop   loc_12_15A2
loc_12_15B0:
        popad
        ret
Install_IO_Handler:
        pushad
        db  081h, 03Dh, 028h, 0DAh, 000h, 000h, 07Fh, 001h, 000h, 000h
        je     SHORT loc_12_163A
        db  0E8h, 07Ch, 017h, 0F4h, 0FFh
        je     SHORT loc_12_163A
        mov    edi, edx
        rol    dl, 1
        xor    dl, dh
        movzx  edx, dl
        lea    edx, [edx + edx*2]
        shr    edx, 1
        xchg   edi, edx
loc_12_15D6:
        db  0E8h, 04Bh, 017h, 0F4h, 0FFh
        je     SHORT loc_12_15E7
        dec    edi
        jns    SHORT loc_12_15D6
        mov    edi, 17fh
        jmp    SHORT loc_12_15D6
loc_12_15E7:
        db  066h, 089h, 014h, 07Dh, 030h, 0DAh, 000h, 000h
        db  089h, 034h, 0BDh, 060h, 0DDh, 000h, 000h
        db  0FFh, 005h, 02Ch, 0DAh, 000h, 000h
        push   edx
        db  0E8h, 0F0h, 0F6h, 0F3h, 0FFh
        mov    edx, dword ptr [esp]
        cmp    edx, 0e0h
        jb     SHORT loc_12_161D
        cmp    edx, 2e0h
        jb     SHORT loc_12_162A
        cmp    edx, 400h
        jae    SHORT loc_12_162A
loc_12_161D:
        db  00Fh, 0ABh, 03Dh, 030h, 0DDh, 000h, 000h
        db  0FFh, 00Dh, 02Ch, 0DAh, 000h, 000h
loc_12_162A:
        pop    edx
        db  0FFh, 005h, 028h, 0DAh, 000h, 000h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        clc
loc_12_1638:
        popad
        ret
loc_12_163A:
        stc
        jmp    SHORT loc_12_1638
loc_12_163D:
        pushad
        db  0E8h, 0FDh, 016h, 0F4h, 0FFh
        stc
        jne    SHORT loc_12_167C
        db  0E8h, 0FAh, 0F6h, 0F3h, 0FFh
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        db  066h, 0C7h, 004h, 07Dh, 030h, 0DAh, 000h, 000h, 000h, 000h
        db  0C7h, 004h, 0BDh, 060h, 0DDh, 000h, 000h, 028h, 02Fh, 000h, 000h
        db  00Fh, 0B3h, 03Dh, 030h, 0DDh, 000h, 000h
        jb     SHORT loc_12_1675
        db  0FFh, 00Dh, 02Ch, 0DAh, 000h, 000h
loc_12_1675:
        db  0FFh, 00Dh, 028h, 0DAh, 000h, 000h
        clc
loc_12_167C:
        popad
        ret
        db  08Bh, 0C0h
loc_12_1680:
        mov    ecx, 20h
        jmp    SHORT loc_12_16A8
        db  090h
loc_12_1688:
        db  0E8h, 06Ch, 016h, 0F4h, 0FFh
        or     ecx, 20h
        jmp    SHORT loc_12_16A8
        db  08Bh, 0C0h
loc_12_1694:
        mov    ecx, 24h
        jmp    SHORT loc_12_16A8
        db  090h
loc_12_169C:
        db  0E8h, 058h, 016h, 0F4h, 0FFh
        or     ecx, 24h
        jmp    SHORT loc_12_16A8
        db  08Bh, 0C0h
loc_12_16A8:
        inc    dword ptr [ebp + 24h]
        movzx  edx, dx
        xor    esi, esi
        bt     edi, 12h
        rcl    esi, 1
        test   byte ptr [ebx], 20h
        je     SHORT loc_12_16CB
        push   eax
        lar    eax, word ptr [ebp + 28h]
        test   eax, 400000h
        pop    eax
        je     SHORT loc_12_16CB
        xor    esi, 1
loc_12_16CB:
        bt     edi, 0eh
        jb     SHORT loc_12_16D5
        bt     edi, 0dh
loc_12_16D5:
        rcl    esi, 1
        shl    esi, 6
        test   byte ptr [ebp + 2dh], 4
        je     SHORT loc_12_16E6
        add    esi, 100h
loc_12_16E6:
        ror    esi, 10h
        test   cl, 4
        jne    SHORT loc_12_16F4
        mov    si, word ptr [ebp + 38h]
        jmp    SHORT loc_12_1712
loc_12_16F4:
        mov    si, word ptr [ebp + 3ch]
        and    edi, 9f00h
        shr    edi, 8
        je     SHORT loc_12_1712
        bsf    edi, edi
        shl    edi, 2
        neg    edi
        add    edi, 44h
        mov    si, word ptr [edi + ebp]
loc_12_1712:
        ror    esi, 10h
        add    ecx, esi
        mov    edi, edx
        rol    dl, 1
        xor    dl, dh
        movzx  edx, dl
        lea    edx, [edx + edx*2]
        shr    edx, 1
        xchg   edi, edx
        db  066h, 039h, 014h, 07Dh, 030h, 0DAh, 000h, 000h
        jne    SHORT loc_12_1738
loc_12_1731:
        db  0FFh, 024h, 0BDh, 060h, 0DDh, 000h, 000h
loc_12_1738:
        db  068h, 031h, 017h, 000h, 000h
        db  0E9h, 02Ah, 016h, 0F4h, 0FFh
loc_12_1742:
        db  0E8h, 0F9h, 015h, 0F4h, 0FFh
        push   edi
        push   ecx
        shr    ecx, 3
        and    ecx, 3
        mov    ebx, 1
        shl    ebx, cl
        mov    ecx, dword ptr [esp]
        test   ch, 1
        je     SHORT loc_12_1760
        neg    ebx
loc_12_1760:
        push   ebx
        mov    edi, dword ptr [ebp + 4]
        test   cl, 4
        jne    SHORT loc_12_176C
        mov    edi, dword ptr [ebp]
loc_12_176C:
        mov    eax, ecx
        shr    eax, 10h
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        test   byte ptr [ebx], 20h
        jne    SHORT loc_12_1781
        shl    eax, 4
        jmp    SHORT loc_12_1792
loc_12_1781:
        push   ecx
        push   edx
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        pop    ecx
loc_12_1792:
        mov    esi, eax
        push   esi
        test   cl, 80h
        jne    SHORT loc_12_179D
        movzx  edi, di
loc_12_179D:
        test   cl, 40h
        jne    SHORT loc_12_17A9
        mov    ecx, 1
        jmp    SHORT loc_12_17BD
loc_12_17A9:
        test   cl, 80h
        je     SHORT loc_12_17B5
        xor    ecx, ecx
        xchg   dword ptr [ebp + 18h], ecx
        jmp    SHORT loc_12_17BB
loc_12_17B5:
        xor    ecx, ecx
        xchg   word ptr [ebp + 18h], cx
loc_12_17BB:
        jecxz  loc_12_182E
loc_12_17BD:
        mov    ebx, dword ptr [esp + 8]
loc_12_17C1:
        test   bl, 4
        je     SHORT loc_12_17DE
        test   bl, 10h
        jne    SHORT loc_12_17D5
        test   bl, 8
        jne    SHORT loc_12_17DA
        mov    al, byte ptr [edi + esi]
        jmp    SHORT loc_12_17DE
loc_12_17D5:
        mov    eax, dword ptr [edi + esi]
        jmp    SHORT loc_12_17DE
loc_12_17DA:
        mov    ax, word ptr [edi + esi]
loc_12_17DE:
        push   ecx
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        and    ecx, 1fh
        movzx  edx, word ptr [ebp + 14h]
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        mov    edi, dword ptr [esp + 14h]
        db  0E8h, 01Ah, 017h, 0F4h, 0FFh
        pop    edi
        pop    ecx
        mov    esi, dword ptr [esp]
        mov    ebx, dword ptr [esp + 8]
        test   bl, 4
        jne    SHORT loc_12_1820
        test   bl, 8
        jne    SHORT loc_12_181C
        test   bl, 10h
        jne    SHORT loc_12_1817
        mov    byte ptr [edi + esi], al
        jmp    SHORT loc_12_1820
loc_12_1817:
        mov    dword ptr [edi + esi], eax
        jmp    SHORT loc_12_1820
loc_12_181C:
        mov    word ptr [edi + esi], ax
loc_12_1820:
        add    edi, dword ptr [esp + 4]
        test   bl, 80h
        jne    SHORT loc_12_182C
        movzx  edi, di
loc_12_182C:
        loop   loc_12_17C1
loc_12_182E:
        add    esp, 8
        pop    ecx
        mov    eax, 4
        test   cl, 4
        jne    SHORT loc_12_1841
        mov    eax, 0
loc_12_1841:
        test   cl, 80h
        jne    SHORT loc_12_184C
        mov    word ptr [eax + ebp], di
        pop    edi
        ret
loc_12_184C:
        mov    dword ptr [eax + ebp], edi
        pop    edi
        ret
loc_12_1851:
        cmp    ecx, 4
        jbe    SHORT loc_12_185C
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_12_185C:
        mov    al, 0ffh
        ret
sub_12_185F:
        mov    eax, 0c02h
        cmp    word ptr [ebp + 18h], ax
        jb     SHORT loc_12_18B6
        xchg   edi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        push   edi
        xor    edx, edx
        db  0E8h, 0C2h, 014h, 0F4h, 0FFh
        mov    eax, 0
        jne    SHORT loc_12_1886
        dec    eax
loc_12_1886:
        pop    edi
        cld
        stosw
        mov    ecx, 300h
        db  0BEh, 030h, 0DAh, 000h, 000h
        rep movsb
        mov    ecx, 900h
        db  08Bh, 035h, 0D8h, 0E3h, 000h, 000h
        xor    eax, eax
        or     esi, esi
        je     SHORT loc_12_18AA
        rep movsb
loc_12_18A9:
        ret
loc_12_18AA:
        rep stosb
        push   -2
        pop    eax
        jmp    SHORT loc_12_18A9
loc_12_18B1:
        or     eax, 0ffffffffh
        jmp    SHORT loc_12_18A9
loc_12_18B6:
        movzx  ecx, word ptr [ebp + 18h]
        cmp    ecx, 1
        ja     SHORT loc_12_18B1
        xor    eax, eax
        btr    word ptr [ebx + 1ch], 7
        adc    eax, eax
        jecxz  loc_12_18A9
        or     byte ptr [ebx + 1ch], 80h
        jmp    SHORT loc_12_18A9
sub_12_18D1:
        or     byte ptr [ebx + 1dh], 1
        ret
        db  000h, 000h
Install_Exception_Handler:
        push   eax
        mov    eax, esi
        db  087h, 005h, 0F4h, 0E3h, 000h, 000h
        mov    dword ptr [esi], eax
        pop    eax
        clc
        ret
Remove_Exception_Handler:
        push   eax
        db  0A1h, 0F4h, 0E3h, 000h, 000h
        cmp    eax, esi
        je     SHORT loc_12_1902
loc_12_18F0:
        test   eax, eax
        je     SHORT loc_12_190C
        cmp    dword ptr [eax], esi
        je     SHORT loc_12_18FC
        mov    eax, dword ptr [eax]
        jmp    SHORT loc_12_18F0
loc_12_18FC:
        push   dword ptr [esi]
        pop    dword ptr [eax]
        jmp    SHORT loc_12_1909
loc_12_1902:
        mov    eax, dword ptr [eax]
        db  0A3h, 0F4h, 0E3h, 000h, 000h
loc_12_1909:
        pop    eax
        clc
        ret
loc_12_190C:
        pop    eax
        stc
        ret
Get_Fault_Hook_Addrs:
        cmp    eax, 60h
        jae    SHORT loc_12_195E
        cmp    eax, 2
        je     SHORT loc_12_195E
        db  08Bh, 014h, 085h, 01Ch, 0E4h, 000h, 000h
        db  08Bh, 034h, 085h, 0A4h, 0E5h, 000h, 000h
        db  08Bh, 03Ch, 085h, 02Ch, 0E7h, 000h, 000h
        db  081h, 0FAh, 0E5h, 032h, 000h, 000h
        je     SHORT loc_12_193E
        db  081h, 0FAh, 0CBh, 032h, 000h, 000h
        jne    SHORT loc_12_1940
loc_12_193E:
        xor    edx, edx
loc_12_1940:
        db  081h, 0FEh, 0F4h, 032h, 000h, 000h
        jne    SHORT loc_12_194A
        xor    esi, esi
loc_12_194A:
        db  081h, 0FFh, 003h, 033h, 000h, 000h
        je     SHORT loc_12_195A
        db  081h, 0FFh, 003h, 034h, 000h, 000h
        jne    SHORT loc_12_195C
loc_12_195A:
        xor    edi, edi
loc_12_195C:
        clc
        ret
loc_12_195E:
        stc
        ret
sub_12_1960:
        test   edi, edi
        je     SHORT loc_12_19AF
        push   eax
        push   ecx
        push   edx
        lea    eax, [edi - 0ch]
        shr    eax, 0ch
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 067h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        lea    eax, [edi - 0ch]
        je     SHORT loc_12_198A
        cmp    dword ptr [eax], 25ff0aebh
        je     SHORT loc_12_19B1
loc_12_198A:
        add    eax, 4
        push   eax
        shr    eax, 0ch
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 067h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        pop    eax
        je     SHORT loc_12_19AC
        cmp    dword ptr [eax], 25ff06ebh
        je     SHORT loc_12_19B1
loc_12_19AC:
        pop    edx
        pop    ecx
        pop    eax
loc_12_19AF:
        stc
        ret
loc_12_19B1:
        mov    edi, dword ptr [eax + 4]
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_12_19B8:
        push   ecx
        push   edi
        mov    ecx, edi
        mov    edi, dword ptr [edi]
loc_12_19BE:
        cmp    edi, esi
        je     SHORT loc_12_19CF
        call   sub_12_1960
        mov    ecx, edi
        mov    edi, dword ptr [edi]
        jae    SHORT loc_12_19BE
        jmp    SHORT loc_12_19DE
loc_12_19CF:
        call   sub_12_1960
        jb     SHORT loc_12_19DE
        xor    esi, esi
        xchg   dword ptr [edi], esi
        mov    dword ptr [esp], ecx
        clc
loc_12_19DE:
        pop    edi
        pop    ecx
        ret
Hook_V86_Fault:
        db  068h, 01Ch, 0E4h, 000h, 000h
        jmp    SHORT loc_12_19F4
Hook_PM_Fault:
        db  068h, 0A4h, 0E5h, 000h, 000h
        jmp    SHORT loc_12_19F4
Hook_VMM_Fault:
        db  068h, 02Ch, 0E7h, 000h, 000h
loc_12_19F4:
        pushad
        mov    edx, dword ptr [esp + 20h]
        cmp    eax, 60h
        jae    SHORT loc_12_1A2E
        cmp    eax, 2
        je     SHORT loc_12_1A2E
        mov    edi, esi
        xchg   dword ptr [edx + eax*4], esi
        mov    ecx, esi
        cmp    esi, dword ptr [edx - 4]
        je     SHORT loc_12_1A14
        cmp    esi, dword ptr [edx - 8]
        jne    SHORT loc_12_1A16
loc_12_1A14:
        xor    ecx, ecx
loc_12_1A16:
        mov    dword ptr [esp + 4], ecx
        call   sub_12_1960
        jb     SHORT loc_12_1A28
        cmp    dword ptr [edi], 0
        jne    SHORT loc_12_1A28
        mov    dword ptr [edi], esi
loc_12_1A28:
        popad
        add    esp, 4
        clc
        ret
loc_12_1A2E:
        popad
        add    esp, 4
        stc
        ret
loc_12_1A34:
        db  068h, 01Ch, 0E4h, 000h, 000h
        jmp    SHORT loc_12_1A47
loc_12_1A3B:
        db  068h, 0A4h, 0E5h, 000h, 000h
        jmp    SHORT loc_12_1A47
loc_12_1A42:
        db  068h, 02Ch, 0E7h, 000h, 000h
loc_12_1A47:
        pushad
        mov    edx, dword ptr [esp + 20h]
        cmp    eax, 60h
        jae    SHORT loc_12_1A2E
        cmp    eax, 2
        je     SHORT loc_12_1A2E
        lea    edi, [edx + eax*4]
        call   sub_12_19B8
        jb     SHORT loc_12_1A2E
        test   esi, esi
        je     SHORT loc_12_1A2E
        mov    dword ptr [edi], esi
        popad
        add    esp, 4
        clc
        ret
Get_NMI_Handler_Addr:
        db  08Bh, 035h, 0ACh, 0ECh, 000h, 000h
        ret
Set_NMI_Handler_Addr:
        db  089h, 035h, 0ACh, 0ECh, 000h, 000h
loc_12_1A79:
        ret
        db  000h, 000h
loc_12_1A7C:
        pushad
        xor    ecx, ecx
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        movzx  eax, ax
        shl    eax, 4
        mov    eax, dword ptr [eax - 8]
        xor    eax, 4e4957h
        and    eax, 0ffdfdfdfh
        je     SHORT loc_12_1A9B
        stc
loc_12_1A9B:
        popad
        ret
loc_12_1A9D:
        test   byte ptr [ebx], 20h
        je     SHORT loc_12_1AE3
        db  0A1h, 0C4h, 0ECh, 000h, 000h
        or     eax, eax
        jne    SHORT loc_12_1AD1
        db  0BEh, 0E0h, 0C6h, 000h, 000h
        db  0CDh, 020h, 0EFh, 000h, 001h, 000h
        db  0BEh, 0F0h, 0C6h, 000h, 000h
        db  0CDh, 020h, 0EFh, 000h, 001h, 000h
        db  0BEh, 07Ch, 012h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        db  0A3h, 0C4h, 0ECh, 000h, 000h
loc_12_1AD1:
        mov    word ptr [ebp], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        and    word ptr [ebp + 2ch], 0fffeh
        clc
        ret
loc_12_1AE3:
        or     word ptr [ebp + 2ch], 1
        clc
        ret
loc_12_1AEA:
        test   byte ptr [ebx], 20h
        jne    SHORT loc_12_1B43
        db  0A1h, 0D0h, 0ECh, 000h, 000h
        mov    word ptr [ebp], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        db  0A1h, 0F8h, 0D9h, 000h, 000h
        add    eax, 0fh
        shr    eax, 4
        mov    word ptr [ebp + 4], ax
        mov    word ptr [ebp + 10h], 1
        mov    word ptr [ebp + 14h], 5ah
        mov    word ptr [ebp + 1ch], 0
        mov    word ptr [ebp + 18h], 3
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h
        test   ebx, 100000h
        je     SHORT loc_12_1B38
        or     byte ptr [ebp + 19h], 1
loc_12_1B38:
        test   ebx, 10000h
        je     SHORT loc_12_1B43
        inc    byte ptr [ebp + 18h]
loc_12_1B43:
        clc
        ret
Hook_Device_V86_API:
        pushad
        mov    edx, 1ch
        jmp    SHORT loc_12_1B53
Hook_Device_PM_API:
        pushad
        mov    edx, 20h
loc_12_1B53:
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        jecxz  loc_12_1B65
        xchg   dword ptr [edx + ecx], esi
        mov    dword ptr [esp + 4], esi
        popad
        clc
        ret
loc_12_1B65:
        popad
        stc
        ret
loc_12_1B68:
        cmp    word ptr [ebp + 1ch], 0
        jne    SHORT loc_12_1B76
        mov    eax, dword ptr [ebx + 0ch]
        mov    word ptr [ebp + 10h], ax
loc_12_1B76:
        mov    cx, word ptr [ebp + 38h]
        movzx  edx, word ptr [ebp]
        db  0CDh, 020h, 049h, 080h, 001h, 000h
sub_12_1B84:
        db  08Bh, 035h, 0C0h, 0ECh, 000h, 000h
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        ret
        db  000h, 000h, 000h
loc_12_1B94:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 020h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    0ch
loc_12_1BA9:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 021h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_12_1BBE:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 032h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    0ch
loc_12_1BD3:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 050h, 000h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    14h
loc_12_1BE8:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 057h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    8
loc_12_1BFD:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_12_1C12:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    0ch
loc_12_1C27:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 06Fh, 000h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_12_1C3C:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 075h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    1ch
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_12_1C60:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        sub    esi, 4
        mov    edi, dword ptr [esp + 0ch]
        push   esi
        push   edi
        db  0E8h, 055h, 0FDh, 0F3h, 0FFh
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_12_1C8E
        mov    esi, dword ptr [esi]
        and    esi, 0ffffffch
        push   edi
        db  0E8h, 0A7h, 0FDh, 0F3h, 0FFh
        lea    eax, [esi - 4]
loc_12_1C8E:
        pop    edi
        pop    esi
        ret    8
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_12_1CA0:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 1ch], 0ffffffffh
        mov    esi, dword ptr [esp + 30h]
        db  08Bh, 00Dh, 01Ch, 0EFh, 000h, 000h
        add    ecx, esi
        db  089h, 00Dh, 01Ch, 0EFh, 000h, 000h
        db  03Bh, 00Dh, 024h, 0EFh, 000h, 000h
        jle    SHORT loc_12_1CD7
        push   esi
        db  0E8h, 0F7h, 037h, 0F4h, 0FFh
        or     eax, eax
        je     NEAR PTR loc_12_1E6D
loc_12_1CD7:
        push   4
        push   esi
        db  0E8h, 085h, 001h, 0F4h, 0FFh
        or     eax, eax
        je     NEAR PTR loc_12_1E6D
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 44h]
        push   dword ptr [esp + 44h]
        push   esi
        db  0E8h, 084h, 034h, 0F4h, 0FFh
        mov    dword ptr [esp + 18h], eax
        cmp    eax, -1
        je     NEAR PTR loc_12_1E62
        mov    ecx, eax
        imul   ecx, ecx, 0dh
        db  08Bh, 01Dh, 0E4h, 0F7h, 000h, 000h
        add    ebx, ecx
        test   byte ptr [esp + 37h], 10h
        mov    edi, dword ptr [esp + 2ch]
        jne    NEAR PTR loc_12_1E7F
        push   edi
        db  0E8h, 088h, 01Bh, 0F4h, 0FFh
        mov    dword ptr [esp + 20h], eax
        lea    eax, [esp + 10h]
        push   eax
        push   esi
        push   edi
        db  0E8h, 0B8h, 01Bh, 0F4h, 0FFh
        mov    dword ptr [esp + 24h], eax
        or     eax, eax
        je     NEAR PTR loc_12_1E21
        mov    eax, dword ptr [esp + 34h]
        and    eax, 60000h
        or     eax, 2010000h
        shr    eax, 10h
        mov    ecx, dword ptr [esp + 18h]
        shl    ecx, 0ch
        or     eax, ecx
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esp + 10h]
        lea    ebp, [eax + esi*4]
        cmp    ebp, eax
        jbe    SHORT loc_12_1DBD
loc_12_1D6D:
        push   dword ptr [esp + 10h]
        db  0E8h, 051h, 028h, 0F4h, 0FFh
        or     eax, eax
        je     SHORT loc_12_1DD2
        db  0FFh, 035h, 0ACh, 0F8h, 000h, 000h
        db  0E8h, 09Bh, 01Ch, 0F4h, 0FFh
        mov    dword ptr [eax], 10040001h
        mov    ecx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [ecx]
        shr    ecx, 0ch
        mov    word ptr [eax + 4], cx
        mov    dword ptr [eax + 6], ebx
        mov    dword ptr [ebx], eax
        add    ebx, 0dh
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ecx], eax
        add    dword ptr [esp + 10h], 4
        add    dword ptr [esp + 14h], 1000h
        cmp    ebp, dword ptr [esp + 10h]
        ja     SHORT loc_12_1D6D
loc_12_1DBD:
        mov    eax, dword ptr [esp + 20h]
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        jmp    NEAR PTR loc_12_1E7F
loc_12_1DD2:
        mov    eax, dword ptr [esp + 24h]
        db  02Bh, 005h, 0ECh, 0FAh, 000h, 000h
        mov    ecx, 0ch
        sub    edx, edx
        div    ecx
        shl    eax, cl
        mov    dword ptr [esp + 14h], eax
        sub    dword ptr [esp + 10h], 4
        lea    eax, [esi*4]
        sub    ebp, eax
        cmp    ebp, dword ptr [esp + 10h]
        ja     SHORT loc_12_1E1C
        mov    edi, 0fffffffch
loc_12_1E04:
        push   dword ptr [esp + 14h]
        dec    esi
        push   dword ptr [esp + 14h]
        db  0E8h, 01Eh, 01Dh, 0F4h, 0FFh
        add    dword ptr [esp + 10h], edi
        cmp    ebp, dword ptr [esp + 10h]
        jbe    SHORT loc_12_1E04
loc_12_1E1C:
        db  0E8h, 0EDh, 018h, 0F4h, 0FFh
loc_12_1E21:
        mov    eax, esi
        imul   eax, eax, 0dh
        lea    edi, [ebx + eax]
        cmp    edi, ebx
        jbe    SHORT loc_12_1E52
loc_12_1E2D:
        mov    dword ptr [ebx], 0
        db  0FFh, 005h, 040h, 0F9h, 000h, 000h
        db  0FFh, 035h, 0A0h, 0FDh, 000h, 000h
        db  0FFh, 035h, 060h, 0FAh, 000h, 000h
        push   ebx
        db  0E8h, 045h, 02Fh, 0F4h, 0FFh
        add    ebx, 0dh
        cmp    edi, ebx
        ja     SHORT loc_12_1E2D
loc_12_1E52:
        mov    eax, dword ptr [esp + 20h]
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
loc_12_1E62:
        db  029h, 035h, 088h, 0EEh, 000h, 000h
        db  0E8h, 065h, 001h, 0F4h, 0FFh
loc_12_1E6D:
        db  029h, 035h, 01Ch, 0EFh, 000h, 000h
loc_12_1E73:
        mov    eax, dword ptr [esp + 1ch]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_12_1E7F:
        mov    eax, dword ptr [esp + 18h]
        shl    eax, 0ch
        mov    dword ptr [esp + 1ch], eax
        test   byte ptr [esp + 34h], 1
        je     SHORT loc_12_1E73
        mov    ecx, esi
        shl    ecx, 0ch
        shl    edi, 0ch
        sub    eax, eax
        shr    ecx, 2
        rep stosd
        jmp    SHORT loc_12_1E73
sub_12_1EA2:
        sub    esp, 4
        push   esi
        push   edi
        db  08Bh, 035h, 0A8h, 0F8h, 000h, 000h
        mov    dword ptr [esp + 8], esi
        and    esi, 0fffff3ffh
        shr    esi, 0ah
        lea    edi, [esi - 7ff000h]
        sub    esi, 800000h
        push   5
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0FFh, 015h, 0F0h, 0F8h, 000h, 000h
        add    esp, 8
        cmp    edi, esi
        jbe    SHORT loc_12_1F18
        mov    eax, 1000h
loc_12_1EE0:
        test   byte ptr [esi + 1], 2
        je     SHORT loc_12_1EF3
        add    esi, 4
        add    dword ptr [esp + 8], eax
        cmp    edi, esi
        ja     SHORT loc_12_1EE0
        jmp    SHORT loc_12_1F18
loc_12_1EF3:
        mov    eax, dword ptr [esp + 10h]
        or     eax, 60000h
        push   eax
        push   0
        push   1
        mov    eax, dword ptr [esp + 14h]
        push   1
        shr    eax, 0ch
        push   eax
        db  0FFh, 015h, 0A0h, 0D7h, 000h, 000h
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_12_1F20
loc_12_1F18:
        mov    dword ptr [esp + 8], 0
loc_12_1F20:
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 8]
        pop    edi
        pop    esi
        add    esp, 4
        ret    4
        db  000h
loc_12_1F40:
        push   ebx
        push   esi
        push   edi
        sub    eax, eax
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        lea    ebx, [edi + ecx - 1]
        sub    ecx, ecx
        db  08Bh, 015h, 06Ch, 0F8h, 000h, 000h
        cmp    edx, eax
        je     SHORT loc_12_1F92
        db  0BEh, 06Ch, 0FAh, 000h, 000h
loc_12_1F62:
        cmp    dword ptr [esi], ebx
        ja     SHORT loc_12_1F92
        cmp    dword ptr [esi + 4], edi
        jae    SHORT loc_12_1F75
        add    esi, 8
        inc    ecx
        cmp    ecx, edx
        jb     SHORT loc_12_1F62
        jmp    SHORT loc_12_1F92
loc_12_1F75:
        shl    ecx, 3
        mov    eax, 1
        db  039h, 0B9h, 06Ch, 0FAh, 000h, 000h
        ja     SHORT loc_12_1F92
        db  039h, 099h, 070h, 0FAh, 000h, 000h
        jb     SHORT loc_12_1F92
        mov    eax, 2
loc_12_1F92:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_12_1FA0:
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        push   5
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0FFh, 015h, 0F0h, 0F8h, 000h, 000h
        mov    esi, dword ptr [esp + 18h]
        add    esp, 8
        test   dword ptr [esi + 1ch], 0fffffffch
        jne    SHORT loc_12_2007
        sub    ebx, ebx
        db  039h, 01Dh, 008h, 0FCh, 000h, 000h
        je     SHORT loc_12_1FE2
        db  0B8h, 008h, 0FCh, 000h, 000h
loc_12_1FD2:
        db  03Dh, 084h, 0FCh, 000h, 000h
        je     SHORT loc_12_2007
        add    eax, 4
        inc    ebx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_12_1FD2
loc_12_1FE2:
        push   0
        push   20h
        db  0FFh, 035h, 0E8h, 0F7h, 000h, 000h
        db  0E8h, 0EFh, 0F4h, 0F3h, 0FFh
        or     eax, eax
        je     SHORT loc_12_2007
        mov    edi, eax
        mov    ecx, 8
        rep movsd
        db  089h, 004h, 09Dh, 008h, 0FCh, 000h, 000h
        mov    edi, ebx
loc_12_2007:
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_12_201C:
        push   esi
        push   edi
        push   5
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0FFh, 015h, 0F0h, 0F8h, 000h, 000h
        mov    eax, dword ptr [esp + 14h]
        add    esp, 8
        or     eax, eax
        je     SHORT loc_12_2059
        cmp    eax, 20h
        jae    SHORT loc_12_2059
        db  08Bh, 034h, 085h, 008h, 0FCh, 000h, 000h
        or     esi, esi
        je     SHORT loc_12_2059
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, 8
        rep movsd
        mov    esi, 1
        jmp    SHORT loc_12_205B
loc_12_2059:
        sub    esi, esi
loc_12_205B:
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        ret
loc_12_206F:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   5
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0FFh, 015h, 0F0h, 0F8h, 000h, 000h
        mov    esi, dword ptr [esp + 20h]
        add    esp, 8
        or     esi, esi
        je     SHORT loc_12_20A6
        cmp    esi, 20h
        jae    SHORT loc_12_20A6
        db  08Bh, 03Ch, 0B5h, 008h, 0FCh, 000h, 000h
        or     edi, edi
        je     SHORT loc_12_20AA
        mov    eax, 1
        jmp    SHORT loc_12_20AC
loc_12_20A6:
        mov    edi, dword ptr [esp + 10h]
loc_12_20AA:
        sub    eax, eax
loc_12_20AC:
        or     eax, eax
        je     SHORT loc_12_210C
        db  08Bh, 01Dh, 0ACh, 0F8h, 000h, 000h
        mov    ecx, ebx
        mov    eax, dword ptr [ebx - 18h]
        mov    ebp, 0ah
        sub    eax, 1ch
        sub    edx, edx
        div    ebp
        imul   eax, ebp
        lea    edx, [ebx + eax]
        cmp    edx, ebx
        jbe    SHORT loc_12_20EE
loc_12_20D1:
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     SHORT loc_12_20E7
        cmp    eax, ebx
        jb     SHORT loc_12_20DF
        cmp    eax, edx
        jbe    SHORT loc_12_20E7
loc_12_20DF:
        movzx  eax, byte ptr [ecx + 2]
        cmp    eax, esi
        je     SHORT loc_12_210C
loc_12_20E7:
        add    ecx, 0ah
        cmp    edx, ecx
        ja     SHORT loc_12_20D1
loc_12_20EE:
        db  0C7h, 004h, 0B5h, 008h, 0FCh, 000h, 000h, 000h, 000h, 000h, 000h
        push   edi
        db  0FFh, 035h, 0E8h, 0F7h, 000h, 000h
        db  0E8h, 04Bh, 0F3h, 0F3h, 0FFh
        mov    esi, 1
        jmp    SHORT loc_12_210E
loc_12_210C:
        sub    esi, esi
loc_12_210E:
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_12_2127:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 2ch], 0
        mov    dword ptr [esp + 28h], 0
        mov    esi, dword ptr [esp + 34h]
        push   esi
        db  0E8h, 068h, 017h, 0F4h, 0FFh
        mov    dword ptr [esp + 20h], eax
        mov    ebx, dword ptr [esp + 44h]
        and    ebx, 1
        je     SHORT loc_12_2189
        cmp    dword ptr [esp + 3ch], 0
        je     SHORT loc_12_2179
        cmp    dword ptr [esp + 3ch], 20h
        jae    SHORT loc_12_2179
        mov    ecx, dword ptr [esp + 3ch]
        db  08Bh, 02Ch, 08Dh, 008h, 0FCh, 000h, 000h
        or     ebp, ebp
        je     SHORT loc_12_217D
        mov    eax, 1
        jmp    SHORT loc_12_217F
loc_12_2179:
        mov    ebp, dword ptr [esp + 1ch]
loc_12_217D:
        sub    eax, eax
loc_12_217F:
        or     eax, eax
        je     NEAR PTR loc_12_2318
        jmp    SHORT loc_12_218D
loc_12_2189:
        mov    ebp, dword ptr [esp + 1ch]
loc_12_218D:
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 3ch]
        push   esi
        db  0E8h, 054h, 017h, 0F4h, 0FFh
        or     eax, eax
        je     NEAR PTR loc_12_2318
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 38h]
        lea    edi, [eax + ecx*4]
        cmp    edi, eax
        jbe    SHORT loc_12_2211
        mov    esi, dword ptr [esp + 24h]
loc_12_21B7:
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax]
        test   ah, 2
        je     NEAR PTR loc_12_2318
        test   ah, 8
        jne    NEAR PTR loc_12_2318
        or     ebx, ebx
        je     SHORT loc_12_2206
        test   byte ptr [esp + 44h], 4
        je     SHORT loc_12_21E0
        test   byte ptr [esi + 3], 80h
        jne    SHORT loc_12_2206
loc_12_21E0:
        push   dword ptr [esp + 10h]
        db  0E8h, 0DAh, 01Ah, 0F4h, 0FFh
        mov    esi, eax
        push   esi
        movzx  eax, byte ptr [esi + 2]
        db  08Bh, 004h, 085h, 008h, 0FCh, 000h, 000h
        mov    eax, dword ptr [eax + 1ch]
        and    eax, 1
        inc    dword ptr [esp + eax*4 + 2ch]
        db  0E8h, 0B7h, 014h, 0F4h, 0FFh
loc_12_2206:
        add    dword ptr [esp + 10h], 4
        cmp    edi, dword ptr [esp + 10h]
        ja     SHORT loc_12_21B7
loc_12_2211:
        or     ebx, ebx
        je     SHORT loc_12_2251
        test   byte ptr [ebp + 1ch], 1
        jne    SHORT loc_12_223C
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 30h]
        db  0E8h, 03Ch, 0FCh, 0F3h, 0FFh
        or     eax, eax
        je     NEAR PTR loc_12_2318
        mov    eax, dword ptr [esp + 2ch]
        db  029h, 005h, 088h, 0EEh, 000h, 000h
        jmp    SHORT loc_12_224C
loc_12_223C:
        mov    eax, dword ptr [esp + 28h]
        db  029h, 005h, 084h, 0EEh, 000h, 000h
        db  001h, 005h, 088h, 0EEh, 000h, 000h
loc_12_224C:
        db  0E8h, 081h, 0FDh, 0F3h, 0FFh
loc_12_2251:
        mov    edx, edi
        mov    ecx, dword ptr [esp + 38h]
        lea    eax, [ecx*4]
        sub    edx, eax
        mov    dword ptr [esp + 10h], edx
        cmp    edi, edx
        jbe    NEAR PTR loc_12_2311
        mov    eax, dword ptr [esp + 44h]
        and    eax, 4
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esp + 44h]
        and    eax, 40000000h
        mov    dword ptr [esp + 14h], eax
        mov    esi, dword ptr [esp + 40h]
loc_12_2288:
        push   dword ptr [esp + 10h]
        db  0E8h, 032h, 01Ah, 0F4h, 0FFh
        cmp    dword ptr [esp + 18h], 0
        db  08Bh, 00Dh, 0F4h, 0F8h, 000h, 000h
        je     SHORT loc_12_22A4
        test   byte ptr [eax + 3], 80h
        jne    SHORT loc_12_22F4
loc_12_22A4:
        test   byte ptr [esp + 44h], 2
        je     SHORT loc_12_22BC
        test   byte ptr [eax + 3], 10h
        je     SHORT loc_12_22B9
        mov    edx, dword ptr [eax + 6]
        mov    dword ptr [edx + 4], esi
        jmp    SHORT loc_12_22BC
loc_12_22B9:
        mov    dword ptr [eax + 6], esi
loc_12_22BC:
        or     ebx, ebx
        je     SHORT loc_12_22F4
        test   byte ptr [eax + 3], 10h
        je     SHORT loc_12_22ED
        movzx  edx, byte ptr [eax + 2]
        db  08Bh, 014h, 095h, 008h, 0FCh, 000h, 000h
        test   byte ptr [edx + 1ch], 2
        je     SHORT loc_12_22E0
        test   byte ptr [ebp + 1ch], 2
        jne    SHORT loc_12_22ED
        dec    ecx
        jmp    SHORT loc_12_22E7
loc_12_22E0:
        test   byte ptr [ebp + 1ch], 2
        je     SHORT loc_12_22ED
        inc    ecx
loc_12_22E7:
        db  089h, 00Dh, 0F4h, 0F8h, 000h, 000h
loc_12_22ED:
        mov    cl, byte ptr [esp + 3ch]
        mov    byte ptr [eax + 2], cl
loc_12_22F4:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_12_22FC
        inc    esi
loc_12_22FC:
        push   eax
        db  0E8h, 0BBh, 013h, 0F4h, 0FFh
        add    dword ptr [esp + 10h], 4
        cmp    edi, dword ptr [esp + 10h]
        ja     NEAR PTR loc_12_2288
loc_12_2311:
        mov    esi, 1
        jmp    SHORT loc_12_231A
loc_12_2318:
        sub    esi, esi
loc_12_231A:
        mov    eax, dword ptr [esp + 20h]
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_12_2340:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    esi, dword ptr [esp + 18h]
        mov    ebp, esi
        shr    ebp, 0ch
        push   ebp
        db  0E8h, 058h, 015h, 0F4h, 0FFh
        mov    edi, eax
        lea    eax, [esp + 10h]
        push   eax
        push   1
        push   ebp
        db  0E8h, 089h, 015h, 0F4h, 0FFh
        cmp    eax, ebx
        je     SHORT loc_12_237A
        mov    ecx, dword ptr [eax + 4]
        and    ecx, 0fffff000h
        cmp    ecx, esi
        jne    SHORT loc_12_237A
        mov    ebx, dword ptr [eax]
loc_12_237A:
        push   dword ptr [edi + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_12_2390:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 1ch], 0
        lea    eax, [esp + 10h]
        push   eax
        mov    esi, dword ptr [esp + 38h]
        push   dword ptr [esp + 40h]
        push   esi
        db  0E8h, 03Eh, 015h, 0F4h, 0FFh
        mov    edi, eax
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 40h]
        db  0E8h, 02Ah, 015h, 0F4h, 0FFh
        mov    ebx, eax
        lea    eax, [esp + 2ch]
        push   eax
        push   dword ptr [esp + 40h]
        push   esi
        db  0E8h, 017h, 0FEh, 0F3h, 0FFh
        mov    dword ptr [esp + 28h], eax
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 40h]
        db  0E8h, 001h, 0FEh, 0F3h, 0FFh
        mov    dword ptr [esp + 20h], eax
        db  08Bh, 02Dh, 0ECh, 0FAh, 000h, 000h
        sub    ebx, ebp
        mov    ecx, 0ch
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        mov    esi, eax
        sub    edi, ebp
        mov    eax, edi
        sub    edx, edx
        div    ecx
        mov    edi, eax
        shl    eax, cl
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 3ch]
        lea    ebx, [eax + ecx*4]
        cmp    ebx, eax
        jbe    SHORT loc_12_2497
loc_12_2425:
        push   dword ptr [esp + 10h]
        db  0E8h, 095h, 018h, 0F4h, 0FFh
        mov    ebp, eax
        mov    eax, dword ptr [esp + 10h]
        test   byte ptr [eax], 1
        je     SHORT loc_12_2460
        mov    eax, dword ptr [ebp + 6]
        cmp    word ptr [eax + 8], 0
        je     SHORT loc_12_2460
        push   dword ptr [esp + 14h]
        db  0E8h, 07Bh, 021h, 0F4h, 0FFh
        or     eax, eax
        je     SHORT loc_12_24CC
        cmp    word ptr [ebp], 1
        jne    SHORT loc_12_2460
        push   dword ptr [esp + 10h]
        db  0E8h, 0CBh, 021h, 0F4h, 0FFh
loc_12_2460:
        cmp    word ptr [ebp + 4], 0
        je     SHORT loc_12_246B
        mov    word ptr [ebp + 4], si
loc_12_246B:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax], ecx
        push   ebp
        db  0E8h, 036h, 012h, 0F4h, 0FFh
        add    dword ptr [esp + 10h], 4
        add    dword ptr [esp + 14h], 4
        cmp    ebx, dword ptr [esp + 10h]
        ja     SHORT loc_12_2425
loc_12_2497:
        mov    dword ptr [esp + 1ch], 1
loc_12_249F:
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        db  0E8h, 0B0h, 0FDh, 0F3h, 0FFh
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        db  0E8h, 0A3h, 0FDh, 0F3h, 0FFh
        db  0E8h, 050h, 012h, 0F4h, 0FFh
        mov    eax, dword ptr [esp + 1ch]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret    0ch
loc_12_24CC:
        push   ebp
        db  0E8h, 0EBh, 011h, 0F4h, 0FFh
        mov    ecx, dword ptr [esp + 3ch]
        lea    eax, [ecx*4]
        sub    ebx, eax
        sub    dword ptr [esp + 10h], 4
        sub    dword ptr [esp + 14h], 4
        shl    esi, 0ch
        mov    dword ptr [esp + 18h], esi
        cmp    ebx, dword ptr [esp + 10h]
        ja     SHORT loc_12_255D
        sub    ebp, ebp
loc_12_24F8:
        push   dword ptr [esp + 14h]
        db  0E8h, 0C2h, 017h, 0F4h, 0FFh
        mov    esi, eax
        mov    eax, dword ptr [esp + 14h]
        test   byte ptr [eax], 1
        je     SHORT loc_12_2527
        mov    eax, dword ptr [esi + 6]
        cmp    word ptr [eax + 8], bp
        je     SHORT loc_12_2527
        push   dword ptr [esp + 10h]
        db  0E8h, 0A9h, 020h, 0F4h, 0FFh
        push   dword ptr [esp + 14h]
        db  0E8h, 004h, 021h, 0F4h, 0FFh
loc_12_2527:
        cmp    word ptr [esi + 4], bp
        je     SHORT loc_12_2531
        mov    word ptr [esi + 4], di
loc_12_2531:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx], eax
        push   esi
        db  0E8h, 070h, 011h, 0F4h, 0FFh
        sub    dword ptr [esp + 10h], 4
        sub    dword ptr [esp + 14h], 4
        cmp    ebx, dword ptr [esp + 10h]
        jbe    SHORT loc_12_24F8
loc_12_255D:
        db  0E8h, 0ACh, 011h, 0F4h, 0FFh
        jmp    NEAR PTR loc_12_249F
_PageResetHandlePAddr:
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [esp + 20h], 0
        jne    NEAR PTR loc_12_261F
        mov    esi, dword ptr [esp + 18h]
        cmp    esi, 1
        jne    NEAR PTR loc_12_261F
        mov    edx, dword ptr [esp + 10h]
        test   edx, 0fffh
        jne    NEAR PTR loc_12_261F
        mov    edi, dword ptr [esp + 1ch]
        cmp    edi, 9fh
        mov    ebx, dword ptr [esp + 14h]
        jne    SHORT loc_12_25AA
        cmp    ebx, 9fh
        jbe    SHORT loc_12_25BA
loc_12_25AA:
        cmp    edi, 9eh
        jne    SHORT loc_12_261F
        cmp    ebx, 9eh
        ja     SHORT loc_12_261F
loc_12_25BA:
        db  08Bh, 00Dh, 00Ch, 0F7h, 000h, 000h
        db  039h, 00Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_12_261F
        shr    edx, 0ch
        add    ebx, edx
        push   0
        push   esi
        push   ebx
        db  0FFh, 015h, 0A4h, 0D7h, 000h, 000h
        add    esp, 0ch
        lea    eax, [ebx*4 - 800000h]
        push   0
        push   1
        shr    eax, 0ch
        push   eax
        db  0FFh, 015h, 0B4h, 0D4h, 000h, 000h
        add    esp, 0ch
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        or     eax, eax
        jne    SHORT loc_12_2621
        push   61001h
        push   edi
        push   esi
        push   ebx
        db  0E8h, 00Ah, 039h, 0F4h, 0FFh
        add    esp, 10h
        push   61001h
        push   edi
        push   esi
        push   edi
        db  0E8h, 0FAh, 038h, 0F4h, 0FFh
        add    esp, 10h
        jmp    SHORT loc_12_2621
loc_12_261F:
        sub    eax, eax
loc_12_2621:
        pop    edi
        pop    esi
        pop    ebx
        ret
_MapPhysToLinear:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   5
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0FFh, 015h, 0F0h, 0F8h, 000h, 000h
        mov    esi, dword ptr [esp + 28h]
        add    esp, 8
        or     esi, esi
        jne    SHORT loc_12_2650
        db  08Bh, 03Dh, 04Ch, 0F9h, 000h, 000h
        jmp    NEAR PTR loc_12_2751
loc_12_2650:
        mov    edi, dword ptr [esp + 1ch]
        lea    eax, [esi + edi - 1]
        db  0BAh, 0E0h, 0FCh, 000h, 000h
        mov    ecx, edx
        or     ecx, ecx
        je     SHORT loc_12_267B
loc_12_2663:
        cmp    dword ptr [edx], edi
        ja     SHORT loc_12_2670
        cmp    dword ptr [edx + 4], eax
        jae    NEAR PTR loc_12_2736
loc_12_2670:
        mov    dword ptr [esp + 14h], edx
        mov    edx, dword ptr [edx + 0ch]
        or     edx, edx
        jne    SHORT loc_12_2663
loc_12_267B:
        shr    esi, 0ch
        add    esi, 40h
        push   0
        push   esi
        push   80080000h
        db  0FFh, 015h, 09Ch, 0D7h, 000h, 000h
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_12_274C
        mov    ebp, eax
        shr    ebp, 0ch
        push   0
        mov    eax, ebp
        and    eax, 3ffh
        add    eax, esi
        add    eax, 3ffh
        shr    eax, 0ah
        push   eax
        lea    eax, [ebp*4]
        sub    eax, 800000h
        shr    eax, 0ch
        push   eax
        db  0FFh, 015h, 0B4h, 0D4h, 000h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_12_273D
        mov    ebx, edi
        shr    ebx, 0ch
        sub    ebx, 20h
        jns    SHORT loc_12_26E1
        sub    ebx, ebx
loc_12_26E1:
        push   61001h
        push   ebx
        push   esi
        push   ebp
        db  0E8h, 026h, 038h, 0F4h, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_12_273D
        mov    ebp, ebx
        shl    ebp, 0ch
        sub    edi, ebp
        add    edi, dword ptr [esp + 10h]
        push   0
        push   10h
        db  0FFh, 035h, 0E8h, 0F7h, 000h, 000h
        db  0E8h, 0D1h, 0EDh, 0F3h, 0FFh
        or     eax, eax
        je     SHORT loc_12_2751
        mov    dword ptr [eax], ebp
        lea    ecx, [esi + ebx]
        shl    ecx, 0ch
        dec    ecx
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [eax + 0ch], 0
        mov    dword ptr [ecx + 0ch], eax
        jmp    SHORT loc_12_2751
loc_12_2736:
        sub    edi, dword ptr [edx]
        add    edi, dword ptr [edx + 8]
        jmp    SHORT loc_12_2751
loc_12_273D:
        push   0
        push   dword ptr [esp + 14h]
        db  0FFh, 015h, 07Ch, 0D4h, 000h, 000h
        add    esp, 8
loc_12_274C:
        mov    edi, 0ffffffffh
loc_12_2751:
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_12_276A:
        push   esi
        push   edi
        push   5
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0BEh, 0E0h, 0FCh, 000h, 000h
        db  0FFh, 015h, 0F0h, 0F8h, 000h, 000h
        add    esp, 8
        mov    eax, esi
        or     eax, eax
        je     SHORT loc_12_27AD
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 10h]
loc_12_2790:
        mov    eax, dword ptr [esi + 8]
        cmp    eax, ecx
        ja     SHORT loc_12_27A6
        mov    edi, dword ptr [esi + 4]
        sub    edi, dword ptr [esi]
        add    edi, eax
        lea    eax, [edx + ecx]
        dec    eax
        cmp    edi, eax
        jae    SHORT loc_12_27AD
loc_12_27A6:
        mov    esi, dword ptr [esi + 0ch]
        or     esi, esi
        jne    SHORT loc_12_2790
loc_12_27AD:
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        ret    8
        db  000h
_GetSetPageOutCount:
        db  08Bh, 00Dh, 0D4h, 0FCh, 000h, 000h
        test   byte ptr [esp + 8], 1
        jne    SHORT loc_12_27E6
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 1
        jne    SHORT loc_12_27E6
        test   byte ptr [ecx + 1], 2
        jne    SHORT loc_12_27E6
        mov    dword ptr [ecx], 0fffff200h
loc_12_27E6:
        mov    eax, 8
        test   byte ptr [ecx + 1], 2
        je     SHORT loc_12_27F3
        mov    al, 1
loc_12_27F3:
        ret
_PageOutDirtyPages:
        xor    eax, eax
        ret
        db  000h
_HeapGetSize:
        push   dword ptr [esp + 4]
        db  0E8h, 0FFh, 0EBh, 0F3h, 0FFh
        or     eax, eax
        je     SHORT loc_12_280F
        push   dword ptr [esp + 4]
        push   eax
        call   sub_12_1C60
loc_12_280F:
        ret
_GetNulPageHandle:
        lea    eax, [0ffffffffh]
        ret
_PageGetSizeAddr:
        xor    eax, eax
        cmp    dword ptr [esp + 4], -1
        jne    SHORT loc_12_2829
        inc    eax
        db  08Bh, 015h, 04Ch, 0F9h, 000h, 000h
        jmp    SHORT loc_12_283C
loc_12_2829:
        push   dword ptr [esp + 4]
        call   sub_12_2340
        pop    ecx
        mov    edx, dword ptr [esp + 4]
        test   eax, eax
        jne    SHORT loc_12_283C
        cdq
loc_12_283C:
        ret
_GetFreePageCount:
        db  08Bh, 015h, 024h, 0EFh, 000h, 000h
        db  02Bh, 015h, 01Ch, 0EFh, 000h, 000h
        db  0A1h, 040h, 0F9h, 000h, 000h
        ret
_GetSysPageCount:
        db  08Bh, 00Dh, 084h, 0EEh, 000h, 000h
        db  0A1h, 018h, 0EFh, 000h, 000h
        db  02Bh, 005h, 088h, 0EEh, 000h, 000h
        jae    SHORT loc_12_2864
        xor    eax, eax
loc_12_2864:
        db  023h, 005h, 064h, 0EFh, 000h, 000h
        sub    ecx, eax
        db  08Bh, 015h, 028h, 0EFh, 000h, 000h
        sub    edx, ecx
        mov    eax, 100h
        ret
_GetVMPgCount:
        mov    edx, 40h
        mov    eax, 100h
        db  0B9h, 008h, 0F9h, 000h, 000h
        ret
        db  000h, 000h
_DeAssign_Device_V86_Pages:
        push   edi
        push   esi
        push   ebx
        test   byte ptr [esp + 1ch], 1
        jne    SHORT loc_12_28A3
        db  080h, 03Dh, 068h, 0F6h, 000h, 000h, 000h
        je     NEAR PTR loc_12_294C
loc_12_28A3:
        mov    ebx, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        db  0E8h, 01Bh, 0DEh, 0F3h, 0FFh
        jb     NEAR PTR loc_12_294C
        je     SHORT loc_12_28D2
        push   2
        db  0E8h, 061h, 0DCh, 0F3h, 0FFh
        jb     NEAR PTR loc_12_294C
        push   8
        db  0E8h, 054h, 0DCh, 0F3h, 0FFh
        jmp    SHORT loc_12_2946
loc_12_28D2:
        push   2
        db  0E8h, 04Bh, 0DCh, 0F3h, 0FFh
        jb     SHORT loc_12_294C
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
loc_12_28E1:
        push   ebx
        lea    ebx, [ebx + 178h]
        push   2
        db  0E8h, 035h, 0DCh, 0F3h, 0FFh
        pop    ebx
        jb     SHORT loc_12_294C
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_12_28E1
        db  0BBh, 0C0h, 0F0h, 000h, 000h
        push   8
        db  0E8h, 018h, 0DCh, 0F3h, 0FFh
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
loc_12_2912:
        push   ebx
        lea    ebx, [ebx + 178h]
        push   8
        db  0E8h, 004h, 0DCh, 0F3h, 0FFh
        pop    ebx
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_12_2912
        test   byte ptr [esp + 1ch], 1
        je     SHORT loc_12_2946
        mov    eax, dword ptr [esp + 10h]
loc_12_293A:
        and    byte ptr [esi], 0efh
        inc    esi
        db  0E8h, 06Dh, 028h, 0F4h, 0FFh
        inc    eax
        loop   loc_12_293A
loc_12_2946:
        mov    al, 1
loc_12_2948:
        pop    ebx
        pop    esi
        pop    edi
        ret
loc_12_294C:
        xor    eax, eax
        jmp    SHORT loc_12_2948
sub_12_2950:
        pushad
        db  083h, 03Dh, 05Ch, 0F6h, 000h, 000h, 000h
        je     SHORT loc_12_2996
        db  08Bh, 015h, 0E4h, 0F0h, 000h, 000h
        db  00Fh, 0B6h, 01Dh, 0F8h, 0F1h, 000h, 000h
        test   ebx, ebx
        je     SHORT loc_12_2996
loc_12_296B:
        cmp    dword ptr [edx + 10h], 0
        je     SHORT loc_12_2990
        mov    edi, dword ptr [edx + 14h]
        shl    edi, 0ch
        cmp    dword ptr [edx + 14h], 100h
        jb     SHORT loc_12_2982
        mov    edi, dword ptr [edx]
loc_12_2982:
        mov    esi, dword ptr [edx + 4]
        db  003h, 035h, 05Ch, 0F6h, 000h, 000h
        xor    ecx, ecx
        call   dword ptr [edx + 0ch]
loc_12_2990:
        add    edx, 18h
        dec    ebx
        jne    SHORT loc_12_296B
loc_12_2996:
        popad
        ret
_TestGlobalV86Mem:
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        add    esi, dword ptr [esp + 14h]
        dec    esi
        mov    edx, eax
        shr    edx, 0ch
        and    eax, 0fffh
        mov    edi, esi
        shr    edi, 0ch
        and    esi, 0fffh
        cmp    edx, 110h
        jae    SHORT loc_12_2A03
        cmp    edi, 110h
        jae    SHORT loc_12_2A03
        db  08Bh, 00Dh, 070h, 0F6h, 000h, 000h
        inc    ecx
        cmp    edx, ecx
        jae    SHORT loc_12_2A15
        cmp    edi, ecx
        ja     SHORT loc_12_2A15
        db  03Bh, 015h, 06Ch, 0F6h, 000h, 000h
        jae    SHORT loc_12_2A03
        db  03Bh, 03Dh, 06Ch, 0F6h, 000h, 000h
        jae    SHORT loc_12_2A07
loc_12_29EA:
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        db  0CDh, 020h, 039h, 001h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    SHORT loc_12_2A0E
        inc    eax
loc_12_29FF:
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_12_2A03:
        xor    eax, eax
        jmp    SHORT loc_12_29FF
loc_12_2A07:
        mov    eax, 2
        jmp    SHORT loc_12_29FF
loc_12_2A0E:
        mov    eax, 3
        jmp    SHORT loc_12_29FF
loc_12_2A15:
        pushad
        db  0CDh, 020h, 000h, 000h, 006h, 000h
        cmp    eax, 30ah
        popad
        mov    al, 3
        jb     SHORT loc_12_2A2C
        db  0CDh, 020h, 00Fh, 000h, 006h, 000h
loc_12_2A2C:
        push   eax
        pushad
        db  0CDh, 020h, 000h, 000h, 015h, 000h
        cmp    eax, 30ah
        popad
        mov    al, 3
        jb     SHORT loc_12_2A48
        db  0CDh, 020h, 00Bh, 000h, 015h, 000h
        mov    byte ptr [esp + 1], al
loc_12_2A48:
        pop    eax
        shl    ah, 4
        or     al, ah
        cmp    al, 30h
        jb     SHORT loc_12_2A5C
        je     SHORT loc_12_2A70
        cmp    al, 32h
        je     SHORT loc_12_2A5A
loc_12_2A58:
        jmp    SHORT loc_12_29EA
loc_12_2A5A:
        jmp    SHORT loc_12_2A07
loc_12_2A5C:
        cmp    al, 20h
        jae    SHORT loc_12_2A5A
        cmp    al, 10h
        jb     SHORT loc_12_2A6C
        je     SHORT loc_12_2A70
        cmp    al, 12h
        je     SHORT loc_12_2A5A
        jmp    SHORT loc_12_2A58
loc_12_2A6C:
        cmp    al, 2
        je     SHORT loc_12_2A5A
loc_12_2A70:
        jmp    SHORT loc_12_2A03
        db  000h, 000h
_MapFreePhysReg:
        xor    eax, eax
        ret
        db  090h
_UnmapFreePhysReg:
        xor    eax, eax
        ret
        db  000h
sub_12_2A7C:
        push   ebx
        push   esi
        push   edi
        db  0E8h, 00Dh, 021h, 0F4h, 0FFh
        db  0E8h, 0ADh, 027h, 0F4h, 0FFh
        db  0E8h, 013h, 025h, 0F4h, 0FFh
        db  0E8h, 055h, 01Bh, 0F4h, 0FFh
        db  0E8h, 030h, 0E9h, 0F3h, 0FFh
        db  0FEh, 005h, 068h, 0F6h, 000h, 000h
        xor    eax, eax
        db  087h, 005h, 0F8h, 083h, 000h, 000h
        or     eax, eax
        je     SHORT loc_12_2AB5
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_12_2AB5:
        call   sub_12_2AC4
        db  0FEh, 005h, 068h, 0F6h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_12_2AC4:
        push   20000000h
        db  0FFh, 035h, 01Ch, 0EBh, 000h, 000h
        db  0FFh, 035h, 018h, 0EBh, 000h, 000h
        db  0CDh, 020h, 01Fh, 001h, 001h, 000h
        add    esp, 0ch
        mov    eax, 0ffff1111h
        db  087h, 005h, 0F8h, 0F7h, 000h, 000h
        push   10h
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        ret
_SetResetV86Pageable:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        push   6
        pop    ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 1e00h
        je     NEAR PTR loc_12_2CAB
        cmp    eax, 800h
        je     NEAR PTR loc_12_2C6D
        cmp    eax, 1000h
        je     NEAR PTR loc_12_2C6D
        cmp    eax, 200h
        je     SHORT loc_12_2B3D
        cmp    eax, 400h
        jne    NEAR PTR loc_12_2CAB
loc_12_2B3D:
        test   byte ptr [ebx + 1], 10h
        jne    SHORT loc_12_2B4D
        test   byte ptr [ebp + 15h], 2
        je     NEAR PTR loc_12_2CAB
loc_12_2B4D:
        mov    ecx, dword ptr [ebp + 10h]
        or     ecx, ecx
        je     NEAR PTR loc_12_2C4F
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, edi
        db  03Bh, 03Dh, 06Ch, 0F6h, 000h, 000h
        jb     NEAR PTR loc_12_2CAB
        cmp    edi, 100h
        jae    NEAR PTR loc_12_2CAB
        mov    ecx, dword ptr [ebp + 10h]
        add    esi, ecx
        cmp    esi, 100h
        ja     NEAR PTR loc_12_2CAB
        mov    edx, edi
        mov    eax, dword ptr [ebx + 4]
        shr    eax, 0ch
        add    edx, eax
        mov    esi, edi
        and    esi, 1fh
        shr    edi, 5
        test   byte ptr [ebp + 15h], 2
        jne    SHORT loc_12_2C08
loc_12_2B9E:
        btr    dword ptr [ebx + edi*4 - 60h], esi
        jae    SHORT loc_12_2BD5
        test   byte ptr [ebx], 10h
        jne    SHORT loc_12_2BD5
        test   byte ptr [ebx + 24h], 4
        je     SHORT loc_12_2BD5
        test   byte ptr [ebx + 1], 20h
        jne    SHORT loc_12_2BBF
        db  0F6h, 005h, 0F9h, 0F8h, 000h, 000h, 001h
        je     SHORT loc_12_2BD5
loc_12_2BBF:
        push   ecx
        push   edx
        push   0
        push   1
        push   edx
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        pop    ecx
        or     eax, eax
        je     SHORT loc_12_2BE3
loc_12_2BD5:
        inc    edx
        inc    esi
        cmp    esi, 20h
        jb     SHORT loc_12_2BDF
        xor    esi, esi
        inc    edi
loc_12_2BDF:
        loop   loc_12_2B9E
        jmp    SHORT loc_12_2C4F
loc_12_2BE3:
        bts    dword ptr [ebx + edi*4 - 60h], esi
        sub    ecx, dword ptr [ebp + 10h]
        je     NEAR PTR loc_12_2CAB
        neg    ecx
        mov    edi, dword ptr [ebp + 0ch]
        mov    edx, edi
        mov    eax, dword ptr [ebx + 4]
        shr    eax, 0ch
        add    edx, eax
        mov    esi, edi
        and    esi, 1fh
        shr    edi, 5
loc_12_2C08:
        bts    dword ptr [ebx + edi*4 - 60h], esi
        jb     SHORT loc_12_2C42
        test   byte ptr [ebx], 10h
        jne    SHORT loc_12_2C42
        test   byte ptr [ebx + 24h], 4
        je     SHORT loc_12_2C42
        test   byte ptr [ebx + 1], 20h
        jne    SHORT loc_12_2C29
        db  0F6h, 005h, 0F9h, 0F8h, 000h, 000h, 001h
        je     SHORT loc_12_2C42
loc_12_2C29:
        push   ecx
        push   edx
        push   8000000h
        push   1
        push   edx
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        pop    ecx
        or     eax, eax
        je     SHORT loc_12_2CAB
loc_12_2C42:
        inc    edx
        inc    esi
        cmp    esi, 20h
        jb     SHORT loc_12_2C4C
        xor    esi, esi
        inc    edi
loc_12_2C4C:
        dec    ecx
        jne    SHORT loc_12_2C08
loc_12_2C4F:
        xor    esi, esi
        dec    esi
loc_12_2C52:
        lea    edi, [ebx - 60h]
        mov    ecx, 8
        xor    eax, eax
        cld
        repe scasd
        je     SHORT loc_12_2C67
        or     byte ptr [ebx + 1], 10h
        jmp    SHORT loc_12_2C9A
loc_12_2C67:
        and    byte ptr [ebx + 1], 0efh
        jmp    SHORT loc_12_2C9A
loc_12_2C6D:
        test   byte ptr [ebp + 15h], 8
        je     SHORT loc_12_2C84
        bts    dword ptr [ebx], 0dh
        jb     SHORT loc_12_2C97
        call   sub_12_2CAF
        or     eax, eax
        je     SHORT loc_12_2CA7
        jmp    SHORT loc_12_2C97
loc_12_2C84:
        bt     dword ptr [ebx], 0dh
        jae    SHORT loc_12_2C97
        call   sub_12_2CAF
        or     eax, eax
        je     SHORT loc_12_2CAB
        btr    dword ptr [ebx], 0dh
loc_12_2C97:
        xor    esi, esi
        dec    esi
loc_12_2C9A:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        mov    eax, esi
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_12_2CA7:
        btr    dword ptr [ebx], 0dh
loc_12_2CAB:
        xor    esi, esi
        jmp    SHORT loc_12_2C52
sub_12_2CAF:
        sub    esp, 4
        mov    dword ptr [esp], 1
        db  0F6h, 005h, 0F9h, 0F8h, 000h, 000h, 001h
        jne    NEAR PTR loc_12_2D63
        test   byte ptr [ebx], 10h
        jne    NEAR PTR loc_12_2D63
        test   byte ptr [ebx + 24h], 4
        je     NEAR PTR loc_12_2D63
        mov    edi, 110h
        bt     dword ptr [ebx + 24h], 0
        jb     SHORT loc_12_2CEA
        mov    edi, 100h
loc_12_2CEA:
        mov    esi, dword ptr [ebx + 4]
        shr    esi, 0ch
loc_12_2CF0:
        mov    eax, esi
        and    eax, 1fh
        mov    ecx, esi
        and    ecx, 1ffh
        shr    ecx, 5
        bt     dword ptr [ebx + ecx*4 - 60h], eax
        jb     SHORT loc_12_2D5F
        test   byte ptr [esp + 15h], 8
        je     SHORT loc_12_2D4D
        push   0
        push   1
        push   esi
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        jmp    SHORT loc_12_2D5B
loc_12_2D1E:
        mov    dword ptr [esp], 0
        test   byte ptr [esp + 15h], 8
        jne    SHORT loc_12_2D2F
        stc
        jmp    SHORT loc_12_2D63
loc_12_2D2F:
        mov    ecx, edi
        mov    edi, 110h
        bt     dword ptr [ebx + 24h], 0
        jb     SHORT loc_12_2D42
        mov    edi, 100h
loc_12_2D42:
        sub    edi, ecx
        je     SHORT loc_12_2D63
        and    byte ptr [esp + 15h], 0f7h
        jmp    SHORT loc_12_2CEA
loc_12_2D4D:
        push   0
        push   1
        push   esi
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
loc_12_2D5B:
        or     eax, eax
        je     SHORT loc_12_2D1E
loc_12_2D5F:
        inc    esi
        dec    edi
        jne    SHORT loc_12_2CF0
loc_12_2D63:
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h
_AllocateThreadDataSlot:
        db  0A1h, 0A8h, 0F6h, 000h, 000h
        or     eax, eax
        je     SHORT loc_12_2D90
        db  0FFh, 035h, 084h, 0F8h, 000h, 000h
        push   eax
        db  0E8h, 01Ah, 00Ah, 0F4h, 0FFh
        or     eax, eax
        lea    eax, [eax*4 + 78h]
        jne    SHORT loc_12_2D92
loc_12_2D90:
        sub    eax, eax
loc_12_2D92:
        ret
loc_12_2D93:
        mov    ecx, dword ptr [esp + 4]
        sub    ecx, 7ch
        shr    ecx, 2
        db  03Bh, 00Dh, 084h, 0F8h, 000h, 000h
        jae    SHORT loc_12_2DB6
        db  0A1h, 0A8h, 0F6h, 000h, 000h
        or     eax, eax
        je     SHORT loc_12_2DB6
        inc    ecx
        push   ecx
        push   eax
        db  0E8h, 035h, 00Ah, 0F4h, 0FFh
loc_12_2DB6:
        ret
        db  000h
loc_12_2DB8:
        mov    edi, edx
        db  0CDh, 020h, 00Bh, 001h, 001h, 000h
        je     SHORT loc_12_2E08
        push   edi
        db  0FFh, 035h, 030h, 0F7h, 000h, 000h
        push   0c0008h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        xchg   ecx, eax
        mov    eax, 0c0001030h
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        xor    edi, edi
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        add    esp, dword ptr [esp]
        pop    edi
        db  0CDh, 020h, 00Eh, 001h, 001h, 000h
        je     SHORT loc_12_2DFF
        db  0CDh, 020h, 007h, 081h, 001h, 000h
loc_12_2DFF:
        mov    ebx, dword ptr [edi + 14h]
        db  0CDh, 020h, 02Eh, 080h, 001h, 000h
loc_12_2E08:
        db  0FFh, 035h, 030h, 0F7h, 000h, 000h
        push   0c0009h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        xchg   ecx, eax
        mov    eax, 0c0001030h
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        xor    edi, edi
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        add    esp, dword ptr [esp]
        ret
loc_12_2E36:
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 0ah
        db  0BEh, 082h, 02Eh, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        db  083h, 03Dh, 034h, 0F7h, 000h, 000h, 000h
        jl     SHORT loc_12_2E54
        ret
loc_12_2E54:
        db  0FFh, 035h, 030h, 0F7h, 000h, 000h
        push   0c0007h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        xchg   ecx, eax
        mov    eax, 0c0001030h
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        xor    edi, edi
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        add    esp, dword ptr [esp]
        ret
loc_12_2E82:
        db  08Bh, 035h, 03Ch, 0F7h, 000h, 000h
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        jne    SHORT loc_12_2EB4
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_12_2F0C
        push   eax
        push   10h
        push   1
        push   80080000h
        db  0CDh, 020h, 01Dh, 001h, 001h, 000h
        add    esp, 0ch
        pop    edx
        test   eax, eax
        je     SHORT loc_12_2F15
        xchg   edx, eax
        mov    dword ptr [eax + 4], edx
loc_12_2EB4:
        push   eax
        mov    eax, dword ptr [eax + 4]
        shr    eax, 0ch
        push   eax
        push   20060080h
        push   0
        push   1
        push   1
        push   eax
        db  0CDh, 020h, 01Eh, 001h, 001h, 000h
        add    esp, 14h
        pop    ecx
        pop    edx
        test   eax, eax
        je     SHORT loc_12_2F20
        push   edx
        lea    edx, [edx]
        push   0
        push   edx
        push   1
        push   ecx
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        pop    eax
        shr    dword ptr [eax], 0ch
        db  08Ah, 00Dh, 00Ch, 02Fh, 000h, 000h
        db  08Bh, 035h, 038h, 0F7h, 000h, 000h
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        db  0FFh, 005h, 034h, 0F7h, 000h, 000h
        db  0FFh, 005h, 030h, 0F7h, 000h, 000h
        clc
loc_12_2F0C:
        push   0
        db  08Fh, 005h, 040h, 0F7h, 000h, 000h
        ret
loc_12_2F15:
        mov    eax, edx
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        stc
        jmp    SHORT loc_12_2F0C
loc_12_2F20:
        mov    eax, edx
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        stc
        jmp    SHORT loc_12_2F0C
        db  000h
_PTEXT ends
        end
