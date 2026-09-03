; vmouse_pcode_03.asm -- VMOUSE object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        test   edx, edx
        je     9
        cmp    edx, 0ch
        jne    SHORT loc_3_005E
        db  087h, 01Dh, 090h, 006h, 000h, 000h
        mov    edx, ebx
        db  003h, 015h, 08Ch, 006h, 000h, 000h
        je     SHORT loc_3_0060
        mov    eax, 10h
        xor    ecx, ecx
        db  0BEh, 067h, 000h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        xchg   dword ptr [edx + 8], esi
        db  0CDh, 020h, 015h, 000h, 001h, 000h
        call   sub_3_07F5
        db  08Bh, 01Dh, 090h, 006h, 000h, 000h
        mov    edx, ebx
        db  003h, 015h, 08Ch, 006h, 000h, 000h
        mov    eax, 10h
        xor    ecx, ecx
        db  0BEh, 067h, 000h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        call   sub_3_0DB2
loc_3_005E:
        clc
        ret
loc_3_0060:
        db  089h, 015h, 090h, 006h, 000h, 000h
        ret
loc_3_0067:
        and    dword ptr [edx + 8], 0
        test   byte ptr [edx], 2
        je     SHORT loc_3_00AC
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_3_00AC
        db  03Bh, 01Dh, 090h, 006h, 000h, 000h
        je     SHORT loc_3_0099
        test   byte ptr [edx + 7dh], 1
        je     SHORT loc_3_0091
        mov    eax, 0ffffffffh
        db  0CDh, 020h, 003h, 000h, 00Ah, 000h
loc_3_0091:
        mov    al, 3
        db  0CDh, 020h, 005h, 080h, 00Ch, 000h
loc_3_0099:
        xor    eax, eax
        db  0CDh, 020h, 003h, 000h, 00Ah, 000h
        mov    al, 2
        mov    ecx, dword ptr [edx + 18h]
        db  0CDh, 020h, 005h, 080h, 00Ch, 000h
loc_3_00AC:
        ret
loc_3_00AD:
        db  003h, 01Dh, 08Ch, 006h, 000h, 000h
        xor    esi, esi
        xchg   dword ptr [ebx + 4], esi
        db  0CDh, 020h, 015h, 000h, 001h, 000h
        xor    esi, esi
        xchg   dword ptr [ebx + 8], esi
        db  0CDh, 020h, 015h, 000h, 001h, 000h
        clc
        ret
VMD_Get_Version:
        mov    eax, 400h
        clc
        ret
VMD_Set_Mouse_Type:
        xor    eax, eax
        stc
        ret
VMD_Get_Mouse_Owner:
        db  08Bh, 01Dh, 090h, 006h, 000h, 000h
        clc
        ret
        db  000h, 000h
loc_3_00E0:
        db  0A1h, 04Ch, 007h, 000h, 000h
        mov    dword ptr [0cch], eax
        clc
        ret
loc_3_00EC:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 2ah
        ja     SHORT loc_3_0112
        mov    edx, ebx
        db  003h, 015h, 08Ch, 006h, 000h, 000h
        db  0FFh, 024h, 085h, 0A0h, 006h, 000h, 000h
loc_3_0112:
        ret
sub_3_0113:
        push   edx
        push   21h
        pop    edx
        xor    ecx, ecx
        db  0CDh, 020h, 030h, 001h, 001h, 000h
        db  08Ah, 00Dh, 095h, 006h, 000h, 000h
        cmp    cl, 7
        ja     SHORT loc_3_0144
        movzx  edx, al
        xor    eax, eax
        inc    eax
        shl    eax, cl
        not    eax
        and    eax, edx
        push   21h
        pop    edx
        mov    ecx, 4
        db  0CDh, 020h, 030h, 001h, 001h, 000h
loc_3_0144:
        pop    edx
        ret
loc_3_0146:
        mov    word ptr [ebp + 1ch], 0ffffh
        db  00Fh, 0B6h, 005h, 096h, 006h, 000h, 000h
        mov    word ptr [ebp + 10h], ax
        and    byte ptr [edx], 7fh
        call   sub_3_0113
        or     al, 1
        jmp    NEAR PTR loc_3_0711
        db  08Bh, 0C0h
loc_3_0168:
        xor    edi, edi
        mov    al, 9
        db  0CDh, 020h, 005h, 000h, 00Ch, 000h
        test   byte ptr [edx], 2
        jne    SHORT loc_3_0193
        inc    word ptr [edx + 76h]
        jne    SHORT loc_3_0193
        or     byte ptr [edx], 2
        db  03Bh, 01Dh, 090h, 006h, 000h, 000h
        jne    SHORT loc_3_0193
        mov    al, 2
        mov    ecx, dword ptr [edx + 18h]
        db  0CDh, 020h, 005h, 080h, 00Ch, 000h
loc_3_0193:
        ret
sub_3_0194:
        dec    word ptr [edx + 76h]
        test   byte ptr [edx], 2
        je     SHORT loc_3_01B0
        and    byte ptr [edx], 0fdh
        db  03Bh, 01Dh, 090h, 006h, 000h, 000h
        jne    SHORT loc_3_01B0
        mov    al, 3
        db  0CDh, 020h, 005h, 080h, 00Ch, 000h
loc_3_01B0:
        ret
loc_3_01B1:
        movzx  eax, byte ptr [edx + 55h]
        mov    word ptr [ebp + 10h], ax
        mov    eax, dword ptr [edx + 18h]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 18h], ax
        ret
loc_3_01C8:
        mov    eax, dword ptr [ebp + 18h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 14h]
        call   sub_3_0618
        mov    dword ptr [edx + 14h], eax
        call   sub_3_0653
        mov    dword ptr [edx + 18h], eax
        test   byte ptr [edx], 2
        je     SHORT loc_3_01F9
        db  03Bh, 01Dh, 090h, 006h, 000h, 000h
        jne    SHORT loc_3_01F9
        mov    ecx, eax
        mov    al, 2
        db  0CDh, 020h, 005h, 000h, 00Ch, 000h
loc_3_01F9:
        ret
loc_3_01FA:
        movzx  eax, word ptr [ebp + 10h]
        mov    esi, 30h
        test   eax, eax
        je     SHORT loc_3_0216
        mov    esi, 3ch
        dec    eax
        test   eax, eax
        je     SHORT loc_3_0216
        mov    esi, 48h
loc_3_0216:
        movzx  eax, byte ptr [edx + 55h]
        mov    word ptr [ebp + 1ch], ax
        add    esi, edx
        xor    eax, eax
        xchg   word ptr [esi + 8], ax
        mov    word ptr [ebp + 10h], ax
        mov    eax, dword ptr [esi]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 18h], ax
        ret
loc_3_0238:
        movzx  eax, word ptr [ebp + 10h]
        mov    esi, 30h
        test   eax, eax
        je     SHORT loc_3_0254
        mov    esi, 3ch
        dec    eax
        test   eax, eax
        je     SHORT loc_3_0254
        mov    esi, 48h
loc_3_0254:
        movzx  eax, byte ptr [edx + 55h]
        mov    word ptr [ebp + 1ch], ax
        add    esi, edx
        xor    eax, eax
        xchg   word ptr [esi + 0ah], ax
        mov    word ptr [ebp + 10h], ax
        mov    eax, dword ptr [esi + 4]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 18h], ax
        ret
loc_3_0277:
        movzx  esi, word ptr [ebp + 18h]
        movzx  edi, word ptr [ebp + 14h]
        test   byte ptr [ebp + 19h], 80h
        je     SHORT loc_3_0287
        xor    esi, esi
loc_3_0287:
        cmp    esi, edi
        jle    SHORT loc_3_028D
        xchg   edi, esi
loc_3_028D:
        mov    word ptr [edx + 0eh], si
        mov    word ptr [edx + 12h], di
        ret
loc_3_0296:
        movzx  esi, word ptr [ebp + 18h]
        movzx  edi, word ptr [ebp + 14h]
        test   byte ptr [ebp + 19h], 80h
        je     SHORT loc_3_02A6
        xor    esi, esi
loc_3_02A6:
        cmp    esi, edi
        jle    SHORT loc_3_02AC
        xchg   edi, esi
loc_3_02AC:
        mov    word ptr [edx + 0ch], si
        mov    word ptr [edx + 10h], di
        ret
loc_3_02B5:
        xor    eax, eax
        xchg   dword ptr [edx + 28h], eax
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 18h], ax
        ret
loc_3_02C6:
        and    byte ptr [edx], 0fbh
        test   byte ptr [ebx], 20h
        jne    SHORT loc_3_02E4
        movzx  esi, word ptr [ebp + 14h]
        mov    ax, word ptr [ebp + 38h]
loc_3_02D6:
        mov    dword ptr [edx + 59h], esi
        mov    word ptr [edx + 5dh], ax
        mov    al, byte ptr [ebp + 18h]
        mov    byte ptr [edx + 5fh], al
        ret
loc_3_02E4:
        or     byte ptr [edx], 4
        mov    ax, 3814h
        db  0CDh, 020h, 007h, 000h, 006h, 000h
        mov    ax, fs
        jmp    SHORT loc_3_02D6
loc_3_02F6:
        and    byte ptr [edx], 0f7h
        ret
loc_3_02FA:
        or     byte ptr [edx], 8
        ret
loc_3_02FE:
        movzx  eax, word ptr [ebp + 18h]
        or     eax, eax
        jne    SHORT loc_3_0307
        inc    eax
loc_3_0307:
        shl    eax, 10h
        mov    ax, word ptr [ebp + 14h]
        or     ax, ax
        jne    SHORT loc_3_0314
        inc    eax
loc_3_0314:
        mov    dword ptr [edx + 20h], eax
        ret
loc_3_0318:
        movzx  eax, word ptr [ebp + 18h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 14h]
        call   sub_3_0653
        mov    ecx, eax
        movzx  eax, word ptr [ebp + 4]
        shl    eax, 10h
        mov    ax, word ptr [ebp]
        call   sub_3_0653
        mov    esi, eax
        mov    al, 9
        mov    edi, eax
        db  0CDh, 020h, 005h, 080h, 00Ch, 000h
loc_3_0346:
        ret
loc_3_0347:
        mov    ax, word ptr [ebp + 38h]
        xchg   word ptr [edx + 5dh], ax
        mov    word ptr [ebp + 38h], ax
        movzx  eax, word ptr [ebp + 14h]
        xchg   dword ptr [edx + 59h], eax
        mov    word ptr [ebp + 14h], ax
        movzx  eax, byte ptr [ebp + 18h]
        and    al, 1fh
        xchg   byte ptr [edx + 5fh], al
        mov    word ptr [ebp + 18h], ax
        ret
loc_3_036C:
        mov    al, 6
        xor    ecx, ecx
        db  0CDh, 020h, 005h, 000h, 00Ch, 000h
        add    eax, 0a9h
        mov    word ptr [ebp + 10h], ax
        ret
loc_3_0380:
        xchg   edi, eax
        mov    ax, 3814h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        mov    ecx, 0a9h
        mov    esi, edx
        rep movsb
        mov    al, 6
        inc    ecx
        db  0CDh, 020h, 005h, 080h, 00Ch, 000h
loc_3_039E:
        xchg   esi, eax
        mov    ax, 3814h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        mov    ecx, 0a9h
        mov    edi, edx
        rep movsb
        mov    al, 6
        push   2
        pop    ecx
        db  0CDh, 020h, 005h, 080h, 00Ch, 000h
loc_3_03BE:
        mov    al, byte ptr [ebp + 18h]
        call   sub_3_0698
        jb     SHORT loc_3_03E5
        movzx  esi, word ptr [ebp + 14h]
        mov    cx, word ptr [ebp + 38h]
        or     cx, cx
        jne    SHORT loc_3_03DB
        or     esi, esi
        jne    SHORT loc_3_03DB
        xor    al, al
loc_3_03DB:
        mov    byte ptr [edi + 6], al
        mov    word ptr [edi + 4], cx
        mov    dword ptr [edi], esi
        ret
loc_3_03E5:
        mov    word ptr [ebp + 1ch], 0ffffh
        ret
loc_3_03EC:
        mov    al, byte ptr [ebp + 18h]
        call   sub_3_0698
        jne    SHORT loc_3_040D
        movzx  eax, byte ptr [edi + 6]
        mov    word ptr [ebp + 18h], ax
        mov    eax, dword ptr [edi]
        mov    word ptr [ebp + 14h], ax
        mov    ax, word ptr [edi + 4]
        mov    word ptr [ebp + 10h], ax
        ret
loc_3_040D:
        xor    eax, eax
        mov    word ptr [ebp + 10h], ax
        mov    word ptr [ebp + 18h], ax
        mov    word ptr [ebp + 14h], ax
        dec    eax
        mov    word ptr [ebp + 1ch], ax
        ret
loc_3_0421:
        mov    edi, 64h
        movzx  eax, word ptr [ebp + 14h]
        cmp    eax, edi
        jb     SHORT loc_3_0430
        mov    eax, edi
loc_3_0430:
        mov    byte ptr [edx + 58h], al
        movzx  eax, word ptr [ebp + 10h]
        cmp    eax, edi
        jb     SHORT loc_3_043D
        mov    eax, edi
loc_3_043D:
        mov    byte ptr [edx + 56h], al
        call   sub_3_06D1
        mov    esi, eax
        movzx  eax, word ptr [ebp + 18h]
        cmp    eax, edi
        jb     SHORT loc_3_0451
        mov    eax, edi
loc_3_0451:
        mov    byte ptr [edx + 57h], al
        call   sub_3_06D1
        shl    esi, 10h
        mov    si, ax
        mov    dword ptr [edx + 1ch], esi
        ret
loc_3_0463:
        movzx  eax, byte ptr [edx + 56h]
        mov    word ptr [ebp + 10h], ax
        mov    al, byte ptr [edx + 57h]
        mov    word ptr [ebp + 18h], ax
        mov    al, byte ptr [edx + 58h]
        mov    word ptr [ebp + 14h], ax
        ret
loc_3_047A:
        ret
loc_3_047B:
        movzx  ecx, word ptr [ebp + 10h]
        mov    byte ptr [edx + 7eh], cl
        mov    al, 5
        db  0CDh, 020h, 005h, 000h, 00Ch, 000h
        ret
loc_3_048B:
        movzx  ax, byte ptr [edx + 7eh]
        mov    word ptr [ebp + 10h], ax
        ret
loc_3_0495:
        mov    word ptr [ebp + 1ch], 0ffffh
        db  0A1h, 04Ch, 007h, 000h, 000h
        mov    word ptr [ebp + 10h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 18h], ax
        ret
loc_3_04AC:
        mov    word ptr [ebp + 1ch], 0ffffh
        db  00Fh, 0B6h, 005h, 096h, 006h, 000h, 000h
        mov    word ptr [ebp + 10h], ax
        ret
loc_3_04BE:
        mov    word ptr [ebp + 1ch], 0ffffh
        db  00Fh, 0B6h, 005h, 096h, 006h, 000h, 000h
        mov    word ptr [ebp + 10h], ax
        xor    al, al
        jmp    NEAR PTR loc_3_0711
loc_3_04D6:
        mov    eax, dword ptr [ebp + 10h]
        cmp    ax, 8
        ja     SHORT loc_3_04E2
        mov    byte ptr [edx + 78h], al
loc_3_04E2:
        ret
loc_3_04E3:
        movzx  eax, byte ptr [edx + 78h]
        mov    word ptr [ebp + 10h], ax
        ret
loc_3_04EC:
        mov    word ptr [ebp + 10h], 830h
        db  0A0h, 094h, 006h, 000h, 000h
        mov    byte ptr [ebp + 19h], al
        db  08Ah, 025h, 095h, 006h, 000h, 000h
        cmp    al, 4
        jne    SHORT loc_3_0506
        xor    ah, ah
loc_3_0506:
        mov    byte ptr [ebp + 18h], ah
        ret
loc_3_050A:
        mov    al, 8
        or     al, 80h
        test   byte ptr [edx + 7dh], 1
        je     SHORT loc_3_0516
        or     al, 10h
loc_3_0516:
        test   byte ptr [edx + 81h], 1
        je     SHORT loc_3_0521
        or     al, 20h
loc_3_0521:
        mov    byte ptr [ebp + 1dh], al
        xor    ah, ah
        mov    word ptr [ebp + 18h], ax
        mov    word ptr [ebp + 14h], ax
        dec    al
        mov    word ptr [ebp + 10h], ax
        ret
loc_3_0535:
        test   byte ptr [edx], 10h
        setne  byte ptr [ebp + 10h]
        mov    eax, dword ptr [edx + 83h]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 18h], ax
        ret
loc_3_054E:
        mov    eax, dword ptr [edx + 0a5h]
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 10h], ax
        xor    eax, eax
        xchg   dword ptr [edx + 24h], eax
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 18h], ax
        ret
loc_3_0570:
        movzx  ecx, word ptr [ebp + 18h]
        jecxz  loc_3_05AE
        mov    eax, ecx
        db  08Bh, 00Dh, 01Eh, 001h, 000h, 000h
        db  08Bh, 03Dh, 01Ah, 001h, 000h, 000h
        repne scasb
        jne    SHORT loc_3_05AD
        mov    cx, word ptr [ebp + 14h]
        mov    word ptr [edx + 95h], cx
        mov    byte ptr [ebp + 18h], 0
        or     byte ptr [edx + 1], 1
        cmp    al, 20h
        jne    SHORT loc_3_05A1
        xor    al, al
loc_3_05A1:
        mov    word ptr [edx + 9fh], ax
        jmp    NEAR PTR sub_3_12B2
loc_3_05AD:
        ret
loc_3_05AE:
        mov    word ptr [edx + 95h], cx
        mov    word ptr [edx + 9fh], 20h
        and    byte ptr [edx + 1], 0feh
        ret
loc_3_05C3:
        movzx  ecx, word ptr [ebp + 18h]
        or     ecx, ecx
        jne    SHORT loc_3_05D6
        db  0A1h, 01Ah, 001h, 000h, 000h
        mov    dword ptr [edx + 0a1h], eax
loc_3_05D6:
        mov    eax, dword ptr [edx + 0a1h]
        movzx  ecx, byte ptr [eax]
        jecxz  loc_3_05E7
        inc    dword ptr [edx + 0a1h]
loc_3_05E7:
        mov    word ptr [ebp + 18h], cx
        xor    eax, eax
        mov    word ptr [ebp + 38h], ax
        mov    word ptr [ebp + 14h], ax
        ret
loc_3_05F6:
        mov    ax, word ptr [edx + 76h]
        mov    word ptr [ebp + 1ch], ax
        mov    eax, dword ptr [edx + 2ch]
        mov    word ptr [ebp + 18h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 10h], ax
        db  00Fh, 0B6h, 005h, 094h, 006h, 000h, 000h
        mov    word ptr [ebp + 14h], ax
        ret
sub_3_0618:
        push   esi
        push   edi
        mov    esi, dword ptr [edx + 0ch]
        mov    edi, dword ptr [edx + 10h]
        cmp    ax, si
        jg     SHORT loc_3_062A
        mov    ax, si
        jmp    SHORT loc_3_0632
loc_3_062A:
        cmp    ax, di
        jl     SHORT loc_3_0632
        mov    ax, di
loc_3_0632:
        ror    eax, 10h
        shr    esi, 10h
        shr    edi, 10h
        cmp    ax, si
        jg     SHORT loc_3_0645
        mov    ax, si
        jmp    SHORT loc_3_064D
loc_3_0645:
        cmp    ax, di
        jl     SHORT loc_3_064D
        mov    ax, di
loc_3_064D:
        ror    eax, 10h
        pop    edi
        pop    esi
        ret
sub_3_0653:
        push   edx
        push   esi
        push   edi
        push   ebx
        mov    esi, eax
        mov    edi, edx
        movsx  ebx, word ptr [edi + 89h]
        cmp    ebx, 1
        jbe    SHORT loc_3_0673
        movsx  eax, si
        cdq
        idiv   ebx
        imul   eax, ebx
        mov    si, ax
loc_3_0673:
        ror    esi, 10h
        movsx  ebx, word ptr [edi + 8bh]
        cmp    ebx, 1
        jbe    SHORT loc_3_068E
        movsx  eax, si
        cdq
        idiv   ebx
        imul   eax, ebx
        mov    si, ax
loc_3_068E:
        ror    esi, 10h
        mov    eax, esi
        pop    ebx
        pop    edi
        pop    esi
        pop    edx
        ret
sub_3_0698:
        push   eax
        push   ebx
        push   ecx
        push   esi
        mov    bl, 0e0h
        mov    bh, bl
        and    bh, al
        je     SHORT loc_3_06CA
        mov    ecx, 3
        lea    edi, [edx + 59h]
        xor    esi, esi
loc_3_06AE:
        add    edi, 7
        mov    al, byte ptr [edi + 6]
        and    al, bl
        jne    SHORT loc_3_06BA
        mov    esi, edi
loc_3_06BA:
        cmp    al, bh
        loopne loc_3_06AE
        jne    SHORT loc_3_06C2
        jmp    SHORT loc_3_06CC
loc_3_06C2:
        or     esi, esi
        je     SHORT loc_3_06CA
        mov    edi, esi
        jmp    SHORT loc_3_06CC
loc_3_06CA:
        shl    bl, 1
loc_3_06CC:
        pop    esi
        pop    ecx
        pop    ebx
        pop    eax
        ret
sub_3_06D1:
        push   edx
        mov    cl, 5
        mov    bl, 2
        idiv   cl
        cmp    al, 0ah
        jae    SHORT loc_3_06E4
        cmp    al, bl
        ja     SHORT loc_3_06EA
        sub    cl, bl
        jmp    SHORT loc_3_06EC
loc_3_06E4:
        inc    cl
        sub    al, cl
        jmp    SHORT loc_3_06EC
loc_3_06EA:
        sub    al, bl
loc_3_06EC:
        movzx  eax, al
        shl    eax, cl
        pop    edx
        ret
sub_3_06F3:
        mov    edx, ebx
        db  003h, 015h, 08Ch, 006h, 000h, 000h
        push   ebx
        call   sub_3_0F98
        pop    ebx
        db  0A1h, 01Ah, 001h, 000h, 000h
        mov    dword ptr [edx + 0a1h], eax
        or     al, 1
        jmp    SHORT loc_3_0711
loc_3_0711:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     NEAR PTR loc_3_07B7
        test   byte ptr [edx], 20h
        jne    NEAR PTR loc_3_07B7
        test   byte ptr [edx + 1], 1
        je     SHORT loc_3_072E
        xor    al, al
loc_3_072E:
        push   eax
        mov    al, 3
        db  0CDh, 020h, 005h, 000h, 00Ch, 000h
        pop    eax
        or     al, al
        movzx  eax, word ptr [edx + 9fh]
        je     SHORT loc_3_0750
        mov    eax, 0f00h
        call   sub_3_14B1
        and    eax, 7fh
loc_3_0750:
        call   sub_3_12B2
        mov    al, 0ch
        db  0CDh, 020h, 005h, 000h, 00Ch, 000h
        mov    byte ptr [edx + 7eh], 0
        mov    dword ptr [edx + 0ch], 0
        mov    eax, dword ptr [edx + 83h]
        mov    dword ptr [edx + 10h], eax
        add    eax, 10001h
        shr    ax, 1
        ror    eax, 10h
        shr    ax, 1
        ror    eax, 10h
        mov    dword ptr [edx + 14h], eax
        cmp    dword ptr [edx + 89h], 0
        je     SHORT loc_3_0793
        call   sub_3_0653
loc_3_0793:
        mov    dword ptr [edx + 18h], eax
        mov    dword ptr [edx + 20h], 80010h
        mov    byte ptr [edx + 5fh], 0
        mov    word ptr [edx + 76h], 0ffffh
        call   sub_3_0DB2
        call   sub_3_07F5
        and    dword ptr [edx], 0c1h
loc_3_07B7:
        clc
        ret
        db  0EBh, 006h, 0FFh, 025h, 09Ch, 006h, 000h, 000h
loc_3_07C1:
        db  080h, 03Dh, 095h, 006h, 000h, 000h, 007h
        jbe    SHORT loc_3_07CF
        call   sub_3_07D5
loc_3_07CF:
        db  0FFh, 025h, 09Ch, 006h, 000h, 000h
sub_3_07D5:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_3_07F4
        db  08Bh, 015h, 08Ch, 006h, 000h, 000h
        add    edx, ebx
        and    byte ptr [edx], 7fh
        test   ecx, ecx
        je     SHORT loc_3_07F4
        or     byte ptr [edx], 80h
        call   sub_3_07F5
loc_3_07F4:
        ret
sub_3_07F5:
        xor    esi, esi
        xchg   dword ptr [edx + 4], esi
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        and    byte ptr [edx], 0feh
        ret
VMD_Set_Mouse_Data:
        db  0A2h, 095h, 006h, 000h, 000h
        db  088h, 025h, 094h, 006h, 000h, 000h
        cmp    ah, 4
        jne    SHORT loc_3_081B
        db  0C6h, 005h, 08Bh, 007h, 000h, 000h, 001h
loc_3_081B:
        test   bl, 1
        je     SHORT loc_3_0827
        db  066h, 089h, 00Dh, 004h, 000h, 000h, 000h
loc_3_0827:
        db  088h, 015h, 097h, 006h, 000h, 000h
        db  088h, 035h, 096h, 006h, 000h, 000h
        ret
VMOUSE_PM_API:
        movzx  eax, word ptr [ebp + 1ch]
        test   eax, eax
        jne    SHORT loc_3_0847
        mov    word ptr [ebp + 1ch], 400h
        jmp    NEAR PTR loc_3_08F5
loc_3_0847:
        db  08Bh, 015h, 08Ch, 006h, 000h, 000h
        add    edx, ebx
        test   ah, ah
        jne    NEAR PTR loc_3_08FA
        cmp    al, 2
        jb     SHORT loc_3_08A5
        je     SHORT loc_3_089B
        cmp    al, 4
        jb     NEAR PTR loc_3_0914
        je     SHORT loc_3_08AF
        cmp    al, 5
        ja     NEAR PTR loc_3_090F
        call   sub_3_3378
        db  0A0h, 094h, 006h, 000h, 000h
        db  08Ah, 025h, 095h, 006h, 000h, 000h
        mov    word ptr [ebp + 1ch], ax
        db  066h, 0A1h, 07Dh, 001h, 000h, 000h
        mov    word ptr [ebp + 18h], ax
        db  066h, 00Fh, 0B6h, 005h, 097h, 006h, 000h, 000h
        mov    word ptr [ebp + 14h], ax
        jmp    SHORT loc_3_08F5
loc_3_089B:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_3_090F
        jmp    SHORT loc_3_08F5
loc_3_08A5:
        test   byte ptr [edx], 20h
        je     SHORT loc_3_090F
        jmp    NEAR PTR loc_3_0940
loc_3_08AF:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_3_090F
        mov    ebx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_3_090F
        db  087h, 01Dh, 090h, 006h, 000h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_3_08D6
        db  08Bh, 01Dh, 090h, 006h, 000h, 000h
loc_3_08D6:
        mov    edx, ebx
        db  003h, 015h, 08Ch, 006h, 000h, 000h
        mov    eax, 10h
        xor    ecx, ecx
        db  0BEh, 067h, 000h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        call   sub_3_0DB2
loc_3_08F5:
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_3_08FA:
        cmp    eax, 100h
        je     SHORT loc_3_08F5
        cmp    eax, 101h
        jne    SHORT loc_3_0927
        call   sub_3_3378
        jmp    SHORT loc_3_08F5
loc_3_090F:
        or     byte ptr [ebp + 2ch], 1
        ret
loc_3_0914:
        or     byte ptr [edx], 20h
        mov    ax, word ptr [ebp + 14h]
        mov    dword ptr [edx + 59h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [edx + 5dh], ax
        jmp    SHORT loc_3_08F5
loc_3_0927:
        cmp    eax, 103h
        jne    SHORT loc_3_090F
        xor    eax, eax
        test   byte ptr [edx], 20h
        je     SHORT loc_3_0937
        inc    al
loc_3_0937:
        mov    word ptr [ebp + 1ch], ax
        jmp    SHORT loc_3_08F5
        db  02Eh, 08Bh, 0C0h
loc_3_0940:
        mov    ebx, dword ptr [ebp + 10h]
        mov    edx, ebx
        db  003h, 015h, 08Ch, 006h, 000h, 000h
        test   byte ptr [edx], 0c1h
        jne    SHORT loc_3_09CA
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [edx + 55h], al
        shr    eax, 10h
        mov    byte ptr [edx + 75h], al
        mov    esi, dword ptr [edx + 18h]
        test   al, 4
        je     SHORT loc_3_096C
        inc    word ptr [edx + 3ah]
        mov    dword ptr [edx + 34h], esi
        jmp    SHORT loc_3_0977
loc_3_096C:
        test   al, 2
        je     SHORT loc_3_0977
        inc    word ptr [edx + 38h]
        mov    dword ptr [edx + 30h], esi
loc_3_0977:
        test   al, 10h
        je     SHORT loc_3_0984
        inc    word ptr [edx + 46h]
        mov    dword ptr [edx + 40h], esi
        jmp    SHORT loc_3_098F
loc_3_0984:
        test   al, 8
        je     SHORT loc_3_098F
        inc    word ptr [edx + 44h]
        mov    dword ptr [edx + 3ch], esi
loc_3_098F:
        mov    eax, dword ptr [ebp]
        mov    ecx, eax
        mov    dword ptr [edx + 18h], eax
        xchg   dword ptr [edx + 14h], ecx
        call   sub_3_0CB4
        add    word ptr [edx + 26h], si
        add    word ptr [edx + 24h], di
        add    word ptr [edx + 2ah], si
        add    word ptr [edx + 28h], di
        db  0BEh, 0CFh, 009h, 000h, 000h
        mov    eax, 400000h
        mov    ecx, 9
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        mov    dword ptr [edx + 4], esi
        or     byte ptr [edx], 1
loc_3_09CA:
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_3_09CF:
        mov    dword ptr [edx + 4], 0
        movzx  eax, byte ptr [edx + 5fh]
        and    al, byte ptr [edx + 75h]
        je     SHORT loc_3_09E7
        lea    edi, [edx + 59h]
        call   sub_3_0CE4
loc_3_09E7:
        and    byte ptr [edx], 0feh
        ret
VMD_Manipulate_Pointer_Message:
        ret
loc_3_09EC:
        mov    dword ptr [edx + 4], 0
        push   edx
        db  0A0h, 067h, 007h, 000h, 000h
        db  08Bh, 035h, 058h, 007h, 000h, 000h
        db  08Bh, 03Dh, 05Ch, 007h, 000h, 000h
        db  08Bh, 015h, 060h, 007h, 000h, 000h
        db  0CDh, 020h, 008h, 000h, 00Ch, 000h
        pop    edx
        db  0A2h, 066h, 007h, 000h, 000h
        db  089h, 035h, 050h, 007h, 000h, 000h
        db  089h, 03Dh, 054h, 007h, 000h, 000h
        db  08Ah, 01Dh, 066h, 007h, 000h, 000h
        mov    byte ptr [edx + 54h], bl
        test   bl, 80h
        jne    SHORT loc_3_0A3D
        and    bl, 7fh
        db  08Ah, 03Dh, 067h, 007h, 000h, 000h
        mov    byte ptr [edx + 54h], bh
loc_3_0A3D:
        call   sub_3_0B8C
        call   sub_3_0BB0
        mov    byte ptr [edx + 75h], al
        db  08Bh, 035h, 050h, 007h, 000h, 000h
        db  08Bh, 03Dh, 054h, 007h, 000h, 000h
        db  08Bh, 01Dh, 090h, 006h, 000h, 000h
        db  0F6h, 005h, 066h, 007h, 000h, 000h, 080h
        je     SHORT loc_3_0A79
        db  0A1h, 058h, 007h, 000h, 000h
        add    word ptr [edx + 26h], ax
        db  0A1h, 05Ch, 007h, 000h, 000h
        add    word ptr [edx + 24h], ax
        jmp    SHORT loc_3_0A81
loc_3_0A79:
        add    word ptr [edx + 26h], si
        add    word ptr [edx + 24h], di
loc_3_0A81:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_3_0A8E
        test   byte ptr [edx], 20h
        je     SHORT loc_3_0AA6
loc_3_0A8E:
        mov    eax, esi
        or     eax, edi
        je     SHORT loc_3_0A9D
        or     byte ptr [edx + 75h], 1
        call   sub_3_0C1C
loc_3_0A9D:
        call   sub_3_0D54
loc_3_0AA2:
        and    byte ptr [edx], 0feh
        ret
loc_3_0AA6:
        mov    eax, esi
        or     eax, edi
        je     SHORT loc_3_0ADD
        or     byte ptr [edx + 75h], 1
        call   sub_3_0C1C
        add    word ptr [edx + 2ah], si
        add    word ptr [edx + 28h], di
        call   sub_3_0C70
        add    word ptr [edx + 16h], si
        add    word ptr [edx + 14h], di
        mov    eax, dword ptr [edx + 14h]
        call   sub_3_0618
        mov    dword ptr [edx + 14h], eax
        call   sub_3_0653
        mov    dword ptr [edx + 18h], eax
loc_3_0ADD:
        db  08Bh, 01Dh, 090h, 006h, 000h, 000h
        movzx  eax, byte ptr [edx + 5fh]
        and    al, byte ptr [edx + 75h]
        je     SHORT loc_3_0AF4
        lea    edi, [edx + 59h]
        call   sub_3_0CE4
loc_3_0AF4:
        pushad
        db  00Fh, 0B7h, 005h, 064h, 007h, 000h, 000h
        mov    esi, eax
        shr    esi, 7
        shl    esi, 6
        and    al, 1
        shl    al, 5
        or     eax, esi
        call   sub_3_0698
        jne    SHORT loc_3_0B28
        mov    ah, byte ptr [edx + 75h]
        and    ah, 1fh
        and    ah, byte ptr [edi + 6]
        je     SHORT loc_3_0B28
        and    al, 0e0h
        or     al, ah
        xor    ah, ah
        call   sub_3_0CE4
loc_3_0B28:
        popad
        test   byte ptr [edx], 2
        je     NEAR PTR loc_3_0AA2
        db  03Bh, 01Dh, 090h, 006h, 000h, 000h
        jne    NEAR PTR loc_3_0AA2
        mov    al, 2
        mov    ecx, dword ptr [edx + 18h]
        db  0CDh, 020h, 005h, 000h, 00Ch, 000h
        jmp    NEAR PTR loc_3_0AA2
loc_3_0B4E:
        db  08Bh, 035h, 068h, 007h, 000h, 000h
        db  08Bh, 03Dh, 06Ch, 007h, 000h, 000h
        db  0A0h, 070h, 007h, 000h, 000h
        mov    byte ptr [edx + 75h], al
        call   sub_3_0D54
        jmp    NEAR PTR loc_3_0AA2
VMD_Set_Middle_Button:
        ret
VMD_Enable_Disable_Mouse_Events:
        push   edx
        push   esi
        db  08Bh, 015h, 08Ch, 006h, 000h, 000h
        add    edx, ebx
        and    byte ptr [edx], 0bfh
        or     eax, eax
        jne    SHORT loc_3_0B86
        or     byte ptr [edx], 40h
        call   sub_3_07F5
loc_3_0B86:
        pop    esi
        pop    edx
        ret
        db  02Eh, 08Bh, 0C0h
sub_3_0B8C:
        xor    bh, bh
        db  0CDh, 020h, 009h, 000h, 00Ch, 000h
        db  084h, 01Dh, 0A6h, 000h, 000h, 000h
        je     SHORT loc_3_0B9E
        inc    bh
loc_3_0B9E:
        db  084h, 01Dh, 0A7h, 000h, 000h, 000h
        je     SHORT loc_3_0BA9
        or     bh, 2
loc_3_0BA9:
        mov    bl, bh
        xchg   byte ptr [edx + 55h], bl
        ret
        db  090h
sub_3_0BB0:
        mov    esi, dword ptr [edx + 18h]
        xor    eax, eax
        mov    ecx, ebx
        inc    ah
        and    bl, ah
        and    bh, ah
        cmp    bl, bh
        je     SHORT loc_3_0BD7
        jb     SHORT loc_3_0BCE
        inc    word ptr [edx + 3ah]
        mov    dword ptr [edx + 34h], esi
        or     al, 4
        jmp    SHORT loc_3_0BD7
loc_3_0BCE:
        inc    word ptr [edx + 38h]
        mov    dword ptr [edx + 30h], esi
        or     al, 2
loc_3_0BD7:
        inc    ah
        mov    ebx, ecx
        and    bl, ah
        and    bh, ah
        cmp    bl, bh
        je     SHORT loc_3_0BF9
        jb     SHORT loc_3_0BF0
        inc    word ptr [edx + 46h]
        mov    dword ptr [edx + 40h], esi
        or     al, 10h
        jmp    SHORT loc_3_0BF9
loc_3_0BF0:
        inc    word ptr [edx + 44h]
        mov    dword ptr [edx + 3ch], esi
        or     al, 8
loc_3_0BF9:
        add    ah, ah
        and    cl, ah
        and    ch, ah
        cmp    cl, ch
        je     SHORT loc_3_0C19
        jb     SHORT loc_3_0C10
        inc    word ptr [edx + 52h]
        mov    dword ptr [edx + 4ch], esi
        or     al, 40h
        jmp    SHORT loc_3_0C19
loc_3_0C10:
        inc    word ptr [edx + 50h]
        mov    dword ptr [edx + 48h], esi
        or     al, 20h
loc_3_0C19:
        ret
        db  08Bh, 0C0h
sub_3_0C1C:
        push   ebp
        push   edx
        movzx  eax, word ptr [edx + 1eh]
        push   eax
        movzx  eax, word ptr [edx + 1ch]
        push   eax
        db  08Bh, 02Dh, 010h, 000h, 000h, 000h
        mov    eax, edi
        pop    ebx
        imul   eax, ebx
        movsx  ebx, bp
        add    eax, ebx
        mov    ebx, 100h
        cdq
        idiv   ebx
        mov    bp, dx
        mov    edi, eax
        ror    ebp, 10h
        mov    eax, esi
        pop    ebx
        imul   eax, ebx
        movsx  ebx, bp
        add    eax, ebx
        mov    ebx, 100h
        cdq
        idiv   ebx
        mov    bp, dx
        mov    esi, eax
        ror    ebp, 10h
        db  089h, 02Dh, 010h, 000h, 000h, 000h
        pop    edx
        pop    ebp
        ret
        db  02Eh, 08Bh, 0C0h
sub_3_0C70:
        push   ebp
        push   edx
        mov    ecx, dword ptr [edx + 20h]
        db  08Bh, 02Dh, 014h, 000h, 000h, 000h
        or     edi, edi
        je     SHORT loc_3_0C90
        movsx  edx, bp
        lea    eax, [edx + edi*8]
        movzx  ebx, cx
        cdq
        idiv   ebx
        mov    bp, dx
        mov    edi, eax
loc_3_0C90:
        shr    ecx, 10h
        ror    ebp, 10h
        or     esi, esi
        je     SHORT loc_3_0CA8
        movsx  edx, bp
        lea    eax, [edx + esi*8]
        cdq
        idiv   ecx
        mov    bp, dx
        mov    esi, eax
loc_3_0CA8:
        ror    ebp, 10h
        db  089h, 02Dh, 014h, 000h, 000h, 000h
        pop    edx
        pop    ebp
        ret
sub_3_0CB4:
        push   ebx
        push   edx
        mov    ebx, dword ptr [edx + 20h]
        movzx  edx, bx
        mov    edi, eax
        sub    di, cx
        movsx  edi, di
        imul   edi, edx
        shr    edi, 3
        shr    ebx, 10h
        shr    eax, 10h
        shr    ecx, 10h
        mov    esi, eax
        sub    si, cx
        movsx  esi, si
        imul   esi, ebx
        shr    esi, 3
        pop    edx
        pop    ebx
        ret
sub_3_0CE4:
        push   edx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        test   byte ptr [edx], 4
        jne    SHORT loc_3_0D01
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        jmp    SHORT loc_3_0D07
loc_3_0D01:
        db  0CDh, 020h, 083h, 000h, 001h, 000h
loc_3_0D07:
        mov    dword ptr [ebp + 1ch], eax
        mov    al, byte ptr [edx + 55h]
        mov    dword ptr [ebp + 10h], eax
        mov    eax, dword ptr [edx + 18h]
        mov    dword ptr [ebp + 14h], eax
        shr    eax, 10h
        mov    dword ptr [ebp + 18h], eax
        mov    eax, dword ptr [edx + 28h]
        mov    dword ptr [ebp], eax
        shr    eax, 10h
        mov    dword ptr [ebp + 4], eax
        mov    cx, word ptr [edi + 4]
        mov    edx, dword ptr [edi]
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    edx
        ret
        db  02Eh, 08Bh, 0C0h
sub_3_0D54:
        movzx  ecx, word ptr [edx + 5dh]
        jecxz  loc_3_0DB1
        push   edx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    dword ptr [ebp + 10h], esi
        mov    dword ptr [ebp + 18h], edi
        xor    eax, eax
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [ebp], eax
        mov    al, byte ptr [edx + 75h]
        mov    dword ptr [ebp + 1ch], eax
        db  0A0h, 096h, 006h, 000h, 000h
        mov    dword ptr [ebp + 14h], eax
        mov    edx, dword ptr [edx + 59h]
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    edx
loc_3_0DB1:
        ret
sub_3_0DB2:
        xor    eax, eax
        db  0A3h, 010h, 000h, 000h, 000h
        db  0A3h, 014h, 000h, 000h, 000h
        clc
        ret
VMD_Get_Mouse_Data:
        db  08Ah, 025h, 094h, 006h, 000h, 000h
        db  0A0h, 095h, 006h, 000h, 000h
        db  066h, 08Bh, 00Dh, 004h, 000h, 000h, 000h
        db  08Ah, 015h, 097h, 006h, 000h, 000h
        db  08Ah, 035h, 096h, 006h, 000h, 000h
        ret
        db  000h
sub_3_0DE0:
        db  080h, 03Dh, 08Bh, 007h, 000h, 000h, 000h
        je     SHORT loc_3_0DFB
        push   ebx
        db  003h, 01Dh, 090h, 007h, 000h, 000h
        mov    dword ptr [ebx], 0
        mov    byte ptr [ebx + 5], 0
        pop    ebx
loc_3_0DFB:
        clc
        ret
sub_3_0DFD:
        db  080h, 03Dh, 08Bh, 007h, 000h, 000h, 000h
        je     NEAR PTR loc_3_0EC5
        call   sub_3_32F7
        db  0C6h, 005h, 097h, 007h, 000h, 000h, 0FFh
        db  0C6h, 005h, 096h, 007h, 000h, 000h, 0FFh
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 0c201h
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        db  080h, 03Dh, 098h, 007h, 000h, 000h, 0FFh
        jne    SHORT loc_3_0EA4
        db  0A1h, 090h, 007h, 000h, 000h
        add    eax, ebx
        cmp    byte ptr [eax + 4], 0ffh
        je     SHORT loc_3_0EA4
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_3_0EA4
        mov    word ptr [ebp + 10h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        mov    word ptr [ebp + 1ch], 0c207h
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        mov    word ptr [ebp + 1ch], 0c200h
        mov    word ptr [ebp + 10h], 100h
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
loc_3_0EA4:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0C6h, 005h, 096h, 007h, 000h, 000h, 000h
        call   sub_3_32B4
loc_3_0EC5:
        ret
loc_3_0EC6:
        db  0A1h, 080h, 007h, 000h, 000h
        db  0BEh, 01Ch, 000h, 000h, 000h
        mov    dword ptr [esi + 10h], eax
        call   sub_3_3217
        mov    ecx, edx
        call   sub_3_2E22
        ret
loc_3_0EE0:
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        mov    edi, esi
        add    di, 0ah
        mov    al, byte ptr [edi]
        db  0A2h, 094h, 007h, 000h, 000h
        db  03Bh, 01Dh, 090h, 006h, 000h, 000h
        jne    SHORT loc_3_0F7D
        db  003h, 01Dh, 090h, 007h, 000h, 000h
        cmp    byte ptr [ebx + 5], 0ffh
        jne    SHORT loc_3_0F84
        cmp    byte ptr [ebx + 4], 0ffh
        je     SHORT loc_3_0F21
        mov    ecx, dword ptr [ebx]
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_3_0F21:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        add    si, 0ah
        mov    ecx, 4
        push   dword ptr [ebp + 30h]
loc_3_0F42:
        mov    ax, word ptr [esi]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        sub    si, 2
        loop   loc_3_0F42
        mov    ecx, dword ptr [ebx]
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        pop    dword ptr [ebp + 30h]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
loc_3_0F7D:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        ret
loc_3_0F84:
        movzx  eax, word ptr [esi + 0ah]
        shl    eax, 10h
        mov    ah, byte ptr [esi + 8]
        mov    al, byte ptr [esi + 6]
        db  0E8h, 056h, 0F4h, 0FCh, 0FFh
        jmp    SHORT loc_3_0F7D
sub_3_0F98:
        db  00Fh, 0B6h, 005h, 0A4h, 000h, 000h, 000h
        mov    byte ptr [edx + 56h], al
        call   sub_3_06D1
        mov    esi, eax
        shl    esi, 10h
        db  00Fh, 0B6h, 005h, 0A5h, 000h, 000h, 000h
        mov    byte ptr [edx + 57h], al
        call   sub_3_06D1
        mov    si, ax
        mov    dword ptr [edx + 1ch], esi
        clc
        ret
sub_3_0FC3:
        db  0BEh, 0A9h, 000h, 000h, 000h
        call   sub_3_100E
        jb     SHORT loc_3_0FE2
        cmp    dl, 1
        ja     SHORT loc_3_0FF0
        je     SHORT loc_3_0FE3
        db  0BAh, 01Bh, 02Dh, 000h, 000h
        db  0C6h, 005h, 0A8h, 000h, 000h, 000h, 000h
loc_3_0FE2:
        ret
loc_3_0FE3:
        db  0C6h, 005h, 0A8h, 000h, 000h, 000h, 003h
        db  0BAh, 00Ch, 027h, 000h, 000h
        ret
loc_3_0FF0:
        db  0C6h, 005h, 0A8h, 000h, 000h, 000h, 009h
        cmp    dl, 2
        db  0BAh, 038h, 02Bh, 000h, 000h
        je     SHORT loc_3_0FE2
        db  0C6h, 005h, 0A8h, 000h, 000h, 000h, 00Fh
        db  0BAh, 022h, 02Eh, 000h, 000h
        ret
sub_3_100E:
        mov    ebx, edi
        xor    edx, edx
        xor    ecx, ecx
loc_3_1014:
        movzx  ecx, byte ptr [esi]
        inc    esi
        jecxz  loc_3_1047
loc_3_101A:
        mov    al, byte ptr [edi]
        cmp    al, 5ah
        ja     SHORT loc_3_1026
        cmp    al, 41h
        jb     SHORT loc_3_1026
        or     al, 20h
loc_3_1026:
        mov    ah, byte ptr [esi]
        cmp    ah, 5ah
        ja     SHORT loc_3_1035
        cmp    ah, 41h
        jb     SHORT loc_3_1035
        or     ah, 20h
loc_3_1035:
        cmp    ah, al
        jne    SHORT loc_3_103F
        inc    esi
        inc    edi
        loop   loc_3_101A
        clc
        ret
loc_3_103F:
        mov    edi, ebx
        add    esi, ecx
        inc    dl
        jmp    SHORT loc_3_1014
loc_3_1047:
        stc
        ret
        db  000h, 000h, 000h
loc_3_104C:
        db  08Bh, 00Dh, 0CCh, 007h, 000h, 000h
        movzx  edx, cx
        shr    ecx, 10h
        mov    eax, 10h
        db  0CDh, 020h, 043h, 000h, 001h, 000h
        call   sub_3_0DFD
        ret
loc_3_1069:
        test   word ptr [edx + 81h], 1
        jne    SHORT loc_3_10BE
        db  0BFh, 0CAh, 000h, 000h, 000h
        xor    eax, eax
        mov    word ptr [edi], 12h
        db  0C7h, 047h, 002h, 0D6h, 000h, 000h, 000h
        mov    byte ptr [edi + 6], al
        cmp    word ptr [ebp + 10h], 0
        je     SHORT loc_3_1099
        or     byte ptr [edx + 7dh], 1
        or     byte ptr [edi + 6], 1
loc_3_1099:
        mov    byte ptr [edi + 7], 1
        mov    dword ptr [edi + 0ch], eax
        mov    ax, word ptr [ebp + 14h]
        mov    word ptr [edi + 8], ax
        shl    eax, 10h
        mov    ax, word ptr [ebp + 18h]
        mov    word ptr [edi + 0ah], ax
        mov    dword ptr [edx + 0a5h], eax
        jmp    NEAR PTR sub_3_13D8
loc_3_10BE:
        ret
loc_3_10BF:
        test   byte ptr [edx + 81h], 1
        je     SHORT loc_3_1128
        db  0BFh, 0CAh, 000h, 000h, 000h
        mov    eax, 10h
        mov    word ptr [edi], 50h
        mov    dword ptr [edi + 2], 10h
        mov    word ptr [edi + 6], ax
        mov    word ptr [edi + 8], ax
        mov    cx, word ptr [ebp + 10h]
        mov    word ptr [edi + 0ah], cx
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 18h]
        mov    word ptr [edi + 0ch], cx
        mov    dword ptr [edx + 2ch], ecx
        mov    word ptr [edi + 0eh], 1
        db  0BFh, 0DAh, 000h, 000h, 000h
        movzx  esi, word ptr [ebp + 38h]
        movzx  ecx, word ptr [ebp + 14h]
        shl    esi, 4
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        mov    ecx, 10h
        rep movsd
        db  0BFh, 0CAh, 000h, 000h, 000h
        jmp    NEAR PTR sub_3_140A
loc_3_1128:
        ret
VMD_Set_Cursor_Proc:
        pushad
        db  08Bh, 015h, 0C8h, 007h, 000h, 000h
        mov    dword ptr [eax + 4], edx
        db  0A3h, 0C8h, 007h, 000h, 000h
        jecxz  loc_3_1183
        push   ecx
        db  003h, 00Dh, 01Eh, 001h, 000h, 000h
        db  083h, 03Dh, 01Ah, 001h, 000h, 000h, 000h
        jne    SHORT loc_3_1157
        push   0
        push   ecx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        jmp    SHORT loc_3_1169
loc_3_1157:
        push   0
        push   ecx
        db  0FFh, 035h, 01Ah, 001h, 000h, 000h
        db  0CDh, 020h, 050h, 000h, 001h, 000h
        add    esp, 0ch
loc_3_1169:
        pop    ecx
        or     eax, eax
        je     SHORT loc_3_1183
        db  0A3h, 01Ah, 001h, 000h, 000h
        mov    edi, eax
        db  003h, 03Dh, 01Eh, 001h, 000h, 000h
        db  001h, 00Dh, 01Eh, 001h, 000h, 000h
        rep movsb
loc_3_1183:
        popad
        ret
VMD_Call_Cursor_Proc:
        movzx  eax, al
        push   edx
        db  08Bh, 015h, 08Ch, 006h, 000h, 000h
        add    edx, ebx
        cmp    dword ptr [edx + 97h], 0
        je     SHORT loc_3_11A0
        call   dword ptr [edx + 97h]
loc_3_11A0:
        pop    edx
        ret
loc_3_11A2:
        mov    edx, ebx
        db  003h, 015h, 08Ch, 006h, 000h, 000h
        movzx  eax, word ptr [ebp + 1ch]
        cmp    ah, 4
        je     SHORT loc_3_11C8
        cmp    ah, 11h
        jne    SHORT loc_3_11BC
        cmp    al, 30h
        jne    SHORT loc_3_11F0
loc_3_11BC:
        cmp    ah, 1
        jb     SHORT loc_3_11D6
        cmp    ah, 1ch
        ja     SHORT loc_3_11CA
loc_3_11C6:
        stc
        ret
loc_3_11C8:
        jmp    SHORT loc_3_11C6
loc_3_11CA:
        cmp    ax, 4f02h
        jne    SHORT loc_3_11D6
        or     dword ptr [edx + 79h], 4
        jmp    SHORT loc_3_11F0
loc_3_11D6:
        test   dword ptr [edx + 79h], 8
        je     SHORT loc_3_11EC
        call   sub_3_12B0
        jae    SHORT loc_3_11EC
        or     dword ptr [edx + 79h], 10h
        jmp    SHORT loc_3_11F0
loc_3_11EC:
        or     ah, ah
        jne    SHORT loc_3_11C6
loc_3_11F0:
        test   byte ptr [edx + 79h], 1
        jne    SHORT loc_3_11C6
        call   sub_3_1219
        mov    eax, 1388h
        db  0BEh, 044h, 012h, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
        stc
        ret
loc_3_120D:
        inc    word ptr [ebp + 24h]
        db  08Bh, 015h, 08Ch, 006h, 000h, 000h
        add    edx, ebx
sub_3_1219:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_3_1232
        bts    dword ptr [edx + 79h], 0
        jb     SHORT loc_3_1232
        test   byte ptr [edx], 2
        je     SHORT loc_3_1232
        call   sub_3_0194
loc_3_1232:
        ret
loc_3_1233:
        inc    word ptr [ebp + 24h]
        db  08Bh, 015h, 08Ch, 006h, 000h, 000h
        test   byte ptr [edx + ebx + 79h], 4
        jne    SHORT loc_3_12AF
loc_3_1244:
        db  08Bh, 015h, 08Ch, 006h, 000h, 000h
        add    edx, ebx
        test   byte ptr [edx + 79h], 1
        je     SHORT loc_3_12AF
        test   byte ptr [edx + 79h], 4
        jne    SHORT loc_3_126E
        test   byte ptr [edx + 79h], 10h
        jne    SHORT loc_3_1285
        mov    eax, 0f00h
        call   sub_3_14B1
        and    al, 7fh
        cbw
        jmp    SHORT loc_3_128C
loc_3_126E:
        mov    eax, 4f03h
        call   sub_3_14B1
        cmp    ax, 4fh
        jne    SHORT loc_3_12AB
        ror    eax, 10h
        and    al, 7fh
        jmp    SHORT loc_3_128C
loc_3_1285:
        stc
        db  0FFh, 015h, 0C0h, 000h, 000h, 000h
loc_3_128C:
        call   sub_3_12B2
        jae    SHORT loc_3_12AB
        test   byte ptr [edx + 79h], 4
        jne    SHORT loc_3_12AB
        xor    eax, eax
        mov    al, 3
        cmp    word ptr [edx + 9fh], ax
        je     SHORT loc_3_12AB
        call   sub_3_12B2
loc_3_12AB:
        and    byte ptr [edx + 79h], 0fah
loc_3_12AF:
        ret
sub_3_12B0:
        clc
        ret
sub_3_12B2:
        mov    ecx, eax
        db  08Bh, 03Dh, 0C8h, 007h, 000h, 000h
        db  0C7h, 005h, 0C4h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0A3h, 0C8h, 000h, 000h, 000h
loc_3_12CA:
        test   byte ptr [edi + 0dh], 1
        je     SHORT loc_3_12DE
        xchg   ecx, eax
        mov    eax, 8
        call   dword ptr [edi]
        xchg   ecx, eax
        cmp    ax, cx
        jne    SHORT loc_3_12ED
loc_3_12DE:
        mov    edi, dword ptr [edi + 4]
        or     edi, edi
        jne    SHORT loc_3_12CA
        db  08Bh, 03Dh, 0C8h, 007h, 000h, 000h
        jmp    SHORT loc_3_1314
loc_3_12ED:
        db  083h, 03Dh, 0C4h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_1304
        push   eax
        db  00Fh, 0B6h, 005h, 0D0h, 000h, 000h, 000h
        cmp    al, byte ptr [edi + 0ch]
        pop    eax
        jae    SHORT loc_3_12DE
loc_3_1304:
        db  066h, 0A3h, 0C8h, 000h, 000h, 000h
        mov    eax, ecx
        db  089h, 03Dh, 0C4h, 000h, 000h, 000h
        jmp    SHORT loc_3_12DE
loc_3_1314:
        db  083h, 03Dh, 0C4h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_1323
        db  08Bh, 03Dh, 0C4h, 000h, 000h, 000h
loc_3_1323:
        db  00Fh, 0B7h, 00Dh, 0C8h, 000h, 000h, 000h
loc_3_132A:
        mov    eax, 4
        call   dword ptr [edi]
        or     ax, ax
        jne    SHORT loc_3_133F
        mov    edi, dword ptr [edi + 4]
        or     edi, edi
        jne    SHORT loc_3_132A
        stc
        ret
loc_3_133F:
        mov    dword ptr [edx + 9bh], edi
        mov    edi, dword ptr [edi]
        mov    dword ptr [edx + 97h], edi
        call   sub_3_1378
        call   sub_3_13A7
        call   sub_3_14FA
        mov    byte ptr [edx + 7eh], 0
        db  066h, 0A1h, 0C8h, 000h, 000h, 000h
        mov    word ptr [edx + 9fh], ax
        mov    eax, dword ptr [edx + 83h]
        mov    dword ptr [edx + 10h], eax
        clc
        ret
sub_3_1378:
        lea    edi, [edx + 7fh]
        mov    word ptr [edi], 16h
        mov    eax, 7
        call   dword ptr [edx + 97h]
        mov    ax, word ptr [edx + 95h]
        or     al, al
        je     SHORT loc_3_139C
        mov    byte ptr [edx + 87h], al
loc_3_139C:
        or     ah, ah
        je     SHORT loc_3_13A6
        mov    byte ptr [edx + 88h], ah
loc_3_13A6:
        ret
sub_3_13A7:
        push   edx
        test   word ptr [edx + 81h], 1
        jne    SHORT loc_3_13C3
        db  0BFh, 044h, 000h, 000h, 000h
        and    byte ptr [edx + 7dh], 0feh
        call   sub_3_13D8
        jmp    SHORT loc_3_13CD
loc_3_13C3:
        db  0BFh, 054h, 000h, 000h, 000h
        call   sub_3_140A
loc_3_13CD:
        pop    edx
        and    byte ptr [edx], 0fdh
        mov    word ptr [edx + 76h], 0ffffh
        ret
sub_3_13D8:
        test   byte ptr [edx + 81h], 1
        stc
        jne    SHORT loc_3_1409
        call   sub_3_14F5
        jb     SHORT loc_3_1409
        and    byte ptr [edx + 7dh], 0e5h
        cmp    byte ptr [edi + 7], 1
        jne    SHORT loc_3_13F8
        call   sub_3_14F7
loc_3_13F8:
        call   sub_3_14F9
        mov    eax, 0
        call   dword ptr [edx + 97h]
        clc
loc_3_1409:
        ret
sub_3_140A:
        test   byte ptr [edx + 81h], 1
        stc
        je     SHORT loc_3_144D
        call   sub_3_144E
        jb     SHORT loc_3_144D
        and    byte ptr [edx + 7dh], 0e4h
        mov    ax, 10h
        cmp    cx, ax
        jne    SHORT loc_3_143C
        ror    ecx, 10h
        cmp    cx, ax
        jne    SHORT loc_3_143C
        cmp    word ptr [edi + 0eh], 1
        jne    SHORT loc_3_143C
        call   sub_3_14F8
loc_3_143C:
        call   sub_3_14F9
        mov    eax, 1
        call   dword ptr [edx + 97h]
        clc
loc_3_144D:
        ret
sub_3_144E:
        cmp    dword ptr [edi + 2], 10h
        jne    SHORT loc_3_14AF
        mov    ax, word ptr [edi + 0eh]
        or     ah, ah
        jne    SHORT loc_3_14AF
        cmp    al, 1
        je     SHORT loc_3_1471
        test   byte ptr [edx + 81h], 2
        je     SHORT loc_3_14AF
        cmp    al, byte ptr [edx + 8dh]
        jne    SHORT loc_3_14AF
loc_3_1471:
        mov    cx, word ptr [edi + 6]
        test   cx, 0ff07h
        jne    SHORT loc_3_14AF
        cmp    cl, byte ptr [edx + 91h]
        ja     SHORT loc_3_14AF
        shl    ecx, 10h
        mov    cx, word ptr [edi + 8]
        or     ch, ch
        jne    SHORT loc_3_14AF
        cmp    cl, byte ptr [edx + 92h]
        ja     SHORT loc_3_14AF
        cbw
        inc    ax
        mul    cl
        shr    ax, 3
        rol    ecx, 10h
        mul    cl
        add    ax, 10h
        cmp    word ptr [edi], ax
        je     SHORT loc_3_14B0
loc_3_14AF:
        stc
loc_3_14B0:
        ret
sub_3_14B1:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], ax
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 10h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 1ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
sub_3_14F5:
        clc
        ret
sub_3_14F7:
        ret
sub_3_14F8:
        ret
sub_3_14F9:
        ret
sub_3_14FA:
        ret
        db  000h
loc_3_14FC:
        cmp    eax, 0dh
        jae    SHORT loc_3_1520
        pushad
        db  089h, 01Dh, 0DCh, 007h, 000h, 000h
        db  003h, 01Dh, 0D8h, 007h, 000h, 000h
        shl    eax, 2
        db  005h, 00Ch, 008h, 000h, 000h
        call   dword ptr [eax]
        mov    dword ptr [esp + 1ch], eax
        popad
        stc
loc_3_1520:
        cmc
        ret
loc_3_1522:
        ret
loc_3_1523:
        and    byte ptr [ebx + 9fh], 0fdh
        or     edi, edi
        je     SHORT loc_3_1551
        or     byte ptr [ebx + 9fh], 2
        mov    dword ptr [ebx + 0ah], ecx
        mov    dword ptr [ebx + 0eh], esi
        test   byte ptr [ebx + 9dh], 2
        je     SHORT loc_3_1551
        call   sub_3_1A3A
        mov    ecx, dword ptr [ebx + 2]
        call   sub_3_2327
loc_3_1551:
        ret
loc_3_1552:
        xor    eax, eax
        test   ch, 0ffh
        jne    SHORT loc_3_157B
        mov    byte ptr [ebx], cl
        mov    eax, ecx
        db  0BFh, 045h, 008h, 000h, 000h
        mov    ecx, 0fh
        repne scasb
        jne    SHORT loc_3_157C
        call   sub_3_15A6
        mov    eax, dword ptr [ebx + 1ah]
        mov    dword ptr [ebx + 16h], eax
        mov    eax, 1
loc_3_157B:
        ret
loc_3_157C:
        xor    eax, eax
        mov    word ptr [ebx + 9dh], ax
        mov    dword ptr [ebx + 0d0h], eax
        mov    dword ptr [ebx + 0cch], eax
        db  0BEh, 024h, 001h, 000h, 000h
        lea    edi, [ebx + 87h]
        cld
        push   16h
        pop    ecx
        rep movsb
        xor    eax, eax
        inc    eax
        ret
sub_3_15A6:
        xor    eax, eax
        mov    word ptr [ebx + 87h], 16h
        mov    word ptr [ebx + 89h], ax
        mov    word ptr [ebx + 9dh], ax
        mov    byte ptr [ebx + 9bh], al
        mov    byte ptr [ebx + 9ch], 1
        mov    word ptr [ebx + 91h], 1
        mov    word ptr [ebx + 93h], 1
        mov    dword ptr [ebx + 0b4h], 0ffffffffh
        mov    dword ptr [ebx + 0d0h], eax
        mov    dword ptr [ebx + 7fh], eax
        mov    byte ptr [ebx + 1], al
        mov    word ptr [ebx + 96h], 1000h
        mov    edx, ebx
        db  02Bh, 015h, 0D8h, 007h, 000h, 000h
        mov    edx, dword ptr [edx + 4]
        mov    dword ptr [ebx + 1ah], edx
        add    dword ptr [ebx + 1ah], 0b8000h
        mov    al, byte ptr [ebx]
        cmp    al, 7
        ja     NEAR PTR loc_3_16A9
        je     SHORT loc_3_164A
        cmp    al, 3
        ja     NEAR PTR loc_3_16A9
        mov    cl, al
        cmp    cl, 2
        jae    SHORT loc_3_1651
        mov    word ptr [ebx + 93h], 10h
        mov    word ptr [ebx + 8dh], 28h
        mov    word ptr [ebx + 96h], 800h
        jmp    SHORT loc_3_1663
loc_3_164A:
        add    dword ptr [ebx + 1ah], 0ffff8000h
loc_3_1651:
        mov    word ptr [ebx + 93h], 8
        mov    word ptr [ebx + 8dh], 50h
loc_3_1663:
        mov    byte ptr [ebx + 98h], 1
        movzx  eax, byte ptr [484h]
        inc    ax
        mov    word ptr [ebx + 8bh], ax
        mov    word ptr [ebx + 91h], 8
        mov    byte ptr [ebx + 95h], 0
        mov    word ptr [ebx + 8fh], 101h
        xor    ecx, ecx
        xchg   dword ptr [ebx + 0e0h], ecx
        jecxz  loc_3_16A8
        push   0
        push   ecx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_3_16A8:
        ret
loc_3_16A9:
        db  0C7h, 083h, 0CCh, 000h, 000h, 000h, 004h, 01Fh, 000h, 000h
        db  0C7h, 083h, 0D0h, 000h, 000h, 000h, 01Ch, 01Ah, 000h, 000h
        mov    dword ptr [ebx + 7fh], 2000h
        cmp    byte ptr [ebx], 6
        jbe    SHORT loc_3_16D7
        mov    dword ptr [ebx + 7fh], 0
        add    dword ptr [ebx + 1ah], 0fffe8000h
loc_3_16D7:
        mov    eax, 9
        sub    al, cl
        db  08Ah, 088h, 054h, 008h, 000h, 000h
        mov    byte ptr [ebx + 95h], cl
        db  08Ah, 0A8h, 090h, 008h, 000h, 000h
        xor    cl, cl
        mov    word ptr [ebx + 96h], cx
        db  066h, 00Fh, 0B6h, 088h, 05Eh, 008h, 000h, 000h
        mov    word ptr [ebx + 93h], cx
        cmp    eax, 3
        jb     SHORT loc_3_171B
        db  00Fh, 0B7h, 00Ch, 045h, 094h, 008h, 000h, 000h
        mov    dword ptr [ebx + 0dch], ecx
loc_3_171B:
        db  08Bh, 00Ch, 085h, 068h, 008h, 000h, 000h
        mov    word ptr [ebx + 8dh], cx
        shr    ecx, 10h
        mov    word ptr [ebx + 8bh], cx
        or     byte ptr [ebx + 89h], 1
        cmp    dword ptr [ebx + 7fh], 0
        je     SHORT loc_3_1745
        jmp    NEAR PTR loc_3_17DC
loc_3_1745:
        mov    dword ptr [ebx + 0b4h], 0
        db  0C7h, 083h, 0CCh, 000h, 000h, 000h, 01Ch, 01Fh, 000h, 000h
        db  0C7h, 083h, 0D0h, 000h, 000h, 000h, 02Ch, 01Ah, 000h, 000h
        cmp    byte ptr [ebx + 95h], 8
        jb     SHORT loc_3_1789
        or     byte ptr [ebx + 9dh], 40h
        mov    dword ptr [ebx + 0b4h], 0ffffffffh
        db  0C7h, 083h, 0CCh, 000h, 000h, 000h, 0D8h, 01Fh, 000h, 000h
        jmp    SHORT loc_3_17DC
loc_3_1789:
        cmp    byte ptr [ebx], 10h
        jne    SHORT loc_3_179E
        db  080h, 03Dh, 040h, 008h, 000h, 000h, 010h
        ja     SHORT loc_3_179E
        mov    byte ptr [ebx + 95h], 2
loc_3_179E:
        mov    al, byte ptr [ebx]
        cmp    al, 0fh
        jb     SHORT loc_3_17CE
        cmp    al, 10h
        ja     SHORT loc_3_17CE
        push   eax
        mov    edx, 3c4h
        mov    ah, 4
        call   sub_3_25A4
        test   ah, 2
        pop    eax
        jne    SHORT loc_3_17CE
        or     byte ptr [ebx + 9dh], 20h
        mov    dword ptr [ebx + 0b4h], 2
        jmp    SHORT loc_3_17DC
loc_3_17CE:
        cmp    al, 11h
        jne    SHORT loc_3_17DC
        mov    dword ptr [ebx + 0b4h], 1
loc_3_17DC:
        mov    al, 10h
        mov    byte ptr [ebx + 99h], al
        mov    byte ptr [ebx + 9ah], al
        movzx  eax, word ptr [ebx + 8dh]
        cmp    byte ptr [ebx + 95h], 8
        je     SHORT loc_3_17FD
        shr    eax, 3
loc_3_17FD:
        mov    dword ptr [ebx + 0d8h], eax
        cmp    byte ptr [ebx], 5
        ja     SHORT loc_3_180E
        add    dword ptr [ebx + 0d8h], eax
loc_3_180E:
        movzx  ecx, word ptr [ebx + 8bh]
        imul   ecx
        cmp    byte ptr [ebx], 6
        jne    SHORT loc_3_181E
        shr    eax, 1
loc_3_181E:
        cmp    byte ptr [ebx], 6
        ja     SHORT loc_3_1828
        add    eax, 2000h
loc_3_1828:
        mov    dword ptr [ebx + 0d4h], eax
        cmp    dword ptr [ebx + 0e0h], 0
        jne    SHORT loc_3_1883
        push   0
        push   188h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    SHORT loc_3_1850
        db  0CDh, 020h, 02Fh, 080h, 001h, 000h
loc_3_1850:
        mov    dword ptr [ebx + 0e0h], eax
        lea    eax, [eax + 30h]
        mov    dword ptr [ebx + 0e4h], eax
        lea    eax, [eax + 30h]
        mov    dword ptr [ebx + 0e8h], eax
        lea    eax, [eax + 64h]
        mov    dword ptr [ebx + 0ech], eax
        lea    eax, [eax + 64h]
        mov    dword ptr [ebx + 0f0h], eax
        lea    eax, [eax + 30h]
        mov    dword ptr [ebx + 0f4h], eax
loc_3_1883:
        mov    eax, dword ptr [ebx + 0e0h]
        mov    dword ptr [ebx + 0b8h], eax
        mov    eax, dword ptr [ebx + 0e8h]
        mov    dword ptr [ebx + 0bch], eax
        db  081h, 0BBh, 0CCh, 000h, 000h, 000h, 004h, 01Fh, 000h, 000h
        je     SHORT loc_3_1902
        mov    eax, dword ptr [ebx + 0d4h]
        add    eax, dword ptr [ebx + 1ah]
        add    eax, 16h
        mov    dword ptr [ebx + 0b8h], eax
        add    eax, 30h
        mov    dword ptr [ebx + 0bch], eax
        cmp    byte ptr [ebx + 95h], 8
        jb     SHORT loc_3_18D5
        add    dword ptr [ebx + 0bch], 150h
loc_3_18D5:
        mov    dword ptr [ebx + 0abh], 0
        mov    byte ptr [ebx + 0afh], 0
        mov    byte ptr [ebx + 0b0h], 1
        mov    byte ptr [ebx + 0b1h], 5
        mov    byte ptr [ebx + 0b2h], 0fh
        mov    byte ptr [ebx + 0b3h], 0ffh
loc_3_1902:
        ret
loc_3_1903:
        push   edi
        lea    esi, [ebx + 87h]
        mov    ecx, 16h
        mov    eax, ecx
        rep movsb
        pop    edi
        movzx  eax, word ptr [ebx + 8dh]
        imul   word ptr [ebx + 93h]
        mov    ecx, eax
        movzx  eax, word ptr [ebx + 8bh]
        imul   word ptr [ebx + 91h]
        dec    ecx
        dec    eax
        mov    word ptr [edi + 6], cx
        mov    word ptr [edi + 4], ax
        ret
loc_3_193D:
        xor    eax, eax
        test   byte ptr [ebx + 9dh], 2
        je     SHORT loc_3_1951
        push   edi
        call   sub_3_1A3A
        pop    edi
        mov    eax, ebx
loc_3_1951:
        push   eax
        lea    esi, [ebx + 1eh]
        movzx  ecx, word ptr [edi]
        xchg   edi, esi
        rep movsb
        call   sub_3_24D2
        pop    eax
        or     eax, eax
        je     SHORT loc_3_1989
        movzx  eax, word ptr [ebx + 4]
        mul    word ptr [ebx + 93h]
        mov    ecx, eax
        shl    ecx, 10h
        movzx  eax, word ptr [ebx + 2]
        mul    word ptr [ebx + 91h]
        mov    cx, ax
        call   sub_3_2327
loc_3_1989:
        ret
loc_3_198A:
        lea    esi, [ebx + 1eh]
        xchg   edi, esi
        push   esi
        movzx  ecx, word ptr [esi]
        rep movsb
        pop    esi
        lea    esi, [esi + 10h]
        call   sub_3_19C4
        mov    edx, 0ffffffffh
        mov    edi, dword ptr [ebx + 0f0h]
        call   sub_3_19FC
        xor    edx, edx
        mov    byte ptr [ebx + 6eh], dl
        lea    esi, [ebx + 4eh]
        mov    edi, dword ptr [ebx + 0f4h]
        call   sub_3_19FC
        ret
        db  08Bh, 0C0h
sub_3_19C4:
        and    byte ptr [ebx + 9dh], 0fbh
        test   byte ptr [ebx + 9dh], 40h
        jne    SHORT loc_3_19FB
        push   esi
        push   edi
        mov    eax, 20h
        mov    ecx, eax
        mov    edi, eax
        or     byte ptr [ebx + 9dh], 4
        mov    eax, eax
loc_3_19E8:
        mov    al, byte ptr [esi]
        and    al, byte ptr [edi + esi]
        jne    SHORT loc_3_19F9
        inc    esi
        loop   loc_3_19E8
        and    byte ptr [ebx + 9dh], 0fbh
loc_3_19F9:
        pop    edi
        pop    esi
loc_3_19FB:
        ret
sub_3_19FC:
        cmp    dword ptr [ebx + 0d0h], 0
        je     SHORT loc_3_1A39
        mov    ecx, 10h
        test   byte ptr [ebx + 9dh], 4
        jne    SHORT loc_3_1A1C
        jmp    dword ptr [ebx + 0d0h]
        db  02Eh, 08Bh, 0C0h
loc_3_1A1C:
        lodsw
        xchg   ah, al
        stosw
        mov    al, dl
        stosb
        loop   loc_3_1A1C
        jmp    SHORT loc_3_1A39
        db  02Eh, 08Bh, 0C0h
loc_3_1A2C:
        lodsw
        xchg   ah, al
        xor    eax, edx
        stosw
        xor    al, al
        stosb
        loop   loc_3_1A2C
loc_3_1A39:
        ret
sub_3_1A3A:
        btr    word ptr [ebx + 9dh], 1
        jae    SHORT loc_3_1A78
        test   byte ptr [ebx + 9dh], 8
        jne    SHORT loc_3_1A78
        test   byte ptr [ebx + 89h], 1
        je     SHORT loc_3_1A5D
        call   sub_3_1AEF
        ret
loc_3_1A5D:
        mov    esi, dword ptr [ebx + 16h]
        call   sub_3_26F8
        add    esi, dword ptr [ebx + 6]
        mov    ax, word ptr [ebx + 14h]
        cmp    ax, word ptr [esi]
        jne    SHORT loc_3_1A78
        mov    ax, word ptr [ebx + 12h]
        mov    word ptr [esi], ax
loc_3_1A78:
        ret
sub_3_1A79:
        mov    edi, esi
        db  00Fh, 0B7h, 005h, 043h, 008h, 000h, 000h
        movsx  ecx, word ptr [ebx + 28h]
        db  081h, 0BBh, 0CCh, 000h, 000h, 000h, 004h, 01Fh, 000h, 000h
        je     SHORT loc_3_1A9F
        cmp    word ptr [ebx + 8dh], 140h
        jne    SHORT loc_3_1A9F
        shr    eax, 1
loc_3_1A9F:
        db  081h, 0BBh, 0CCh, 000h, 000h, 000h, 004h, 01Fh, 000h, 000h
        jne    SHORT loc_3_1AAD
        shr    ecx, 1
loc_3_1AAD:
        sub    eax, ecx
        mov    dword ptr [ebx + 6fh], eax
        db  00Fh, 0B7h, 005h, 041h, 008h, 000h, 000h
        movsx  ecx, word ptr [ebx + 2ah]
        sub    eax, ecx
        mov    dword ptr [ebx + 73h], eax
        call   sub_3_218D
        call   sub_3_1B12
        call   sub_3_1DC8
        test   byte ptr [ebx + 9dh], 40h
        jne    SHORT loc_3_1ADF
        call   sub_3_2299
loc_3_1ADF:
        call   sub_3_2003
        call   sub_3_1D50
        call   sub_3_2218
        ret
sub_3_1AEF:
        test   byte ptr [ebx + 9dh], 10h
        je     SHORT loc_3_1B11
        mov    edi, dword ptr [ebx + 16h]
        call   sub_3_218D
        call   sub_3_1C5C
        call   sub_3_2218
        and    byte ptr [ebx + 9dh], 0efh
loc_3_1B11:
        ret
sub_3_1B12:
        call   sub_3_1E50
        test   byte ptr [ebx + 9dh], 10h
        je     SHORT loc_3_1B30
        and    byte ptr [ebx + 9dh], 0efh
        or     edx, edx
        je     SHORT loc_3_1B36
        call   sub_3_1C5C
loc_3_1B30:
        call   sub_3_1B44
        ret
loc_3_1B36:
        call   sub_3_1B44
        call   sub_3_1CD0
        ret
        db  02Eh, 08Bh, 0C0h
sub_3_1B44:
        test   byte ptr [ebx + 9dh], 20h
        je     SHORT sub_3_1B80
        mov    al, 0
        call   sub_3_25C3
        mov    eax, dword ptr [ebx + 0e8h]
        mov    dword ptr [ebx + 0bch], eax
        call   sub_3_1B80
        mov    al, 2
        call   sub_3_25C3
        mov    eax, dword ptr [ebx + 0ech]
        mov    dword ptr [ebx + 0bch], eax
        call   sub_3_1B80
        ret
        db  08Bh, 0C0h
sub_3_1B80:
        push   edi
        push   ebp
        mov    esi, edi
        add    esi, dword ptr [ebx + 6]
        mov    edx, edi
        add    edx, dword ptr [ebx + 0d4h]
        mov    edi, dword ptr [ebx + 0bch]
        mov    ecx, dword ptr [ebx + 0c8h]
        mov    eax, 5
        test   byte ptr [ebx + 9dh], 40h
        je     SHORT loc_3_1BAE
        mov    eax, 28h
loc_3_1BAE:
        neg    eax
        add    eax, dword ptr [ebx + 0d8h]
        mov    ebp, dword ptr [ebx + 7fh]
        test   byte ptr [ebx + 9dh], 40h
        je     SHORT loc_3_1BFD
        mov    eax, eax
loc_3_1BC4:
        cmp    esi, edx
        jae    NEAR PTR loc_3_1C56
        push   ecx
        sub    ecx, ecx
        cmp    esi, dword ptr [ebx + 16h]
        jae    SHORT loc_3_1BDF
        mov    ecx, dword ptr [ebx + 16h]
        sub    ecx, esi
        add    esi, ecx
        add    edi, ecx
        neg    ecx
loc_3_1BDF:
        add    ecx, 28h
        jg     SHORT loc_3_1BEC
        add    esi, ecx
        add    edi, ecx
        xor    ecx, ecx
        mov    eax, eax
loc_3_1BEC:
        jecxz  loc_3_1BF6
        push   eax
        nop
loc_3_1BF0:
        lodsb
        stosb
        dec    ecx
        jne    SHORT loc_3_1BF0
        pop    eax
loc_3_1BF6:
        pop    ecx
        add    esi, eax
        loop   loc_3_1BC4
        jmp    SHORT loc_3_1C56
loc_3_1BFD:
        cmp    esi, dword ptr [ebx + 16h]
        jae    SHORT loc_3_1C2C
        push   ecx
        mov    ecx, dword ptr [ebx + 16h]
        sub    ecx, esi
        add    esi, ecx
        add    edi, ecx
        neg    ecx
        add    ecx, 5
        jg     SHORT loc_3_1C1C
        add    esi, ecx
        add    edi, ecx
        sub    ecx, ecx
        mov    eax, eax
loc_3_1C1C:
        jecxz  loc_3_1C26
        push   eax
        nop
loc_3_1C20:
        lodsb
        stosb
        dec    ecx
        jne    SHORT loc_3_1C20
        pop    eax
loc_3_1C26:
        pop    ecx
        jmp    SHORT loc_3_1C38
        db  02Eh, 08Bh, 0C0h
loc_3_1C2C:
        push   eax
        lodsb
        stosb
        lodsb
        stosb
        lodsb
        stosb
        lodsb
        stosb
        lodsb
        stosb
        pop    eax
loc_3_1C38:
        add    esi, eax
        or     ebp, ebp
        je     SHORT loc_3_1C54
        sub    esi, dword ptr [ebx + 0d8h]
        add    esi, ebp
        cmp    esi, edx
        jb     SHORT loc_3_1C54
        sub    esi, ebp
        sub    esi, ebp
        add    esi, dword ptr [ebx + 0d8h]
loc_3_1C54:
        loop   loc_3_1BFD
loc_3_1C56:
        pop    ebp
        pop    edi
        ret
        db  02Eh, 08Bh, 0C0h
sub_3_1C5C:
        test   byte ptr [ebx + 9dh], 20h
        je     SHORT sub_3_1CA0
        mov    al, 3
        call   sub_3_25CE
        mov    eax, dword ptr [ebx + 0e0h]
        mov    dword ptr [ebx + 0b8h], eax
        call   sub_3_1CA0
        mov    al, 3
        call   sub_3_25CE
        mov    eax, dword ptr [ebx + 0e4h]
        mov    dword ptr [ebx + 0b8h], eax
        call   sub_3_1CA0
        mov    al, 0fh
        call   sub_3_25CE
        ret
        db  02Eh, 08Bh, 0C0h
sub_3_1CA0:
        push   edi
        mov    eax, dword ptr [ebx + 77h]
        mov    esi, dword ptr [ebx + 7bh]
        push   eax
        push   esi
        call   sub_3_1ED8
        add    esi, edi
        mov    edx, edi
        add    edx, dword ptr [ebx + 0d4h]
        mov    edi, dword ptr [ebx + 0b8h]
        xchg   edi, esi
        sub    ecx, ecx
        push   10h
        call   sub_3_20C0
        add    esp, 0ch
        pop    edi
        ret
        db  08Bh, 0C0h
sub_3_1CD0:
        test   byte ptr [ebx + 9dh], 20h
        je     SHORT sub_3_1D14
        mov    eax, dword ptr [ebx + 0e0h]
        mov    dword ptr [ebx + 0b8h], eax
        mov    eax, dword ptr [ebx + 0e8h]
        mov    dword ptr [ebx + 0bch], eax
        call   sub_3_1D14
        mov    eax, dword ptr [ebx + 0e4h]
        mov    dword ptr [ebx + 0b8h], eax
        mov    eax, dword ptr [ebx + 0ech]
        mov    dword ptr [ebx + 0bch], eax
        call   sub_3_1D14
        ret
sub_3_1D14:
        mov    eax, dword ptr [ebx + 77h]
        mov    esi, dword ptr [ebx + 7bh]
        call   sub_3_207C
        push   edi
        mov    edi, dword ptr [ebx + 0b8h]
        xchg   edi, esi
        test   byte ptr [ebx + 9dh], 40h
        je     SHORT loc_3_1D44
        mov    eax, eax
loc_3_1D34:
        push   ecx
        mov    ecx, 18h
        rep movsb
        pop    ecx
        add    edi, eax
        loop   loc_3_1D34
        jmp    SHORT loc_3_1D4B
        db  090h
loc_3_1D44:
        movsb
        movsb
        movsb
        add    edi, eax
        loop   loc_3_1D44
loc_3_1D4B:
        pop    edi
        ret
        db  02Eh, 08Bh, 0C0h
sub_3_1D50:
        test   byte ptr [ebx + 9dh], 20h
        je     SHORT sub_3_1D94
        mov    al, 3
        call   sub_3_25CE
        mov    eax, dword ptr [ebx + 0e8h]
        mov    dword ptr [ebx + 0bch], eax
        call   sub_3_1D94
        mov    al, 0ch
        call   sub_3_25CE
        mov    eax, dword ptr [ebx + 0ech]
        mov    dword ptr [ebx + 0bch], eax
        call   sub_3_1D94
        mov    al, 0fh
        call   sub_3_25CE
        ret
        db  02Eh, 08Bh, 0C0h
sub_3_1D94:
        push   edi
        mov    esi, dword ptr [ebx + 0bch]
        mov    edx, edi
        add    edx, dword ptr [ebx + 0d4h]
        add    edi, dword ptr [ebx + 6]
        mov    ecx, 2
        push   dword ptr [ebx + 0c0h]
        push   dword ptr [ebx + 0c4h]
        push   dword ptr [ebx + 0c8h]
        call   sub_3_20C0
        add    esp, 0ch
        pop    edi
        ret
        db  090h
sub_3_1DC8:
        test   byte ptr [ebx + 9dh], 20h
        je     SHORT sub_3_1E0C
        mov    eax, dword ptr [ebx + 0e0h]
        mov    dword ptr [ebx + 0b8h], eax
        mov    eax, dword ptr [ebx + 0e8h]
        mov    dword ptr [ebx + 0bch], eax
        call   sub_3_1E0C
        mov    eax, dword ptr [ebx + 0e4h]
        mov    dword ptr [ebx + 0b8h], eax
        mov    eax, dword ptr [ebx + 0ech]
        mov    dword ptr [ebx + 0bch], eax
        call   sub_3_1E0C
        ret
sub_3_1E0C:
        mov    eax, dword ptr [ebx + 6fh]
        mov    esi, dword ptr [ebx + 73h]
        mov    dword ptr [ebx + 77h], eax
        mov    dword ptr [ebx + 7bh], esi
        call   sub_3_207C
        push   edi
        mov    edi, dword ptr [ebx + 0b8h]
        test   byte ptr [ebx + 9dh], 40h
        je     SHORT loc_3_1E40
        mov    eax, eax
loc_3_1E30:
        push   ecx
        mov    ecx, 18h
        rep movsb
        pop    ecx
        add    esi, eax
        loop   loc_3_1E30
        jmp    SHORT loc_3_1E47
        db  090h
loc_3_1E40:
        movsb
        movsb
        movsb
        add    esi, eax
        loop   loc_3_1E40
loc_3_1E47:
        pop    edi
        or     byte ptr [ebx + 9dh], 10h
        ret
sub_3_1E50:
        sub    edx, edx
        test   byte ptr [ebx + 9dh], 10h
        je     SHORT loc_3_1E9A
        mov    eax, dword ptr [ebx + 6fh]
        mov    ecx, dword ptr [ebx + 77h]
        cmp    eax, ecx
        jge    SHORT loc_3_1E66
        xchg   ecx, eax
loc_3_1E66:
        test   byte ptr [ebx + 9dh], 40h
        je     SHORT loc_3_1E76
        sub    eax, ecx
        cmp    eax, 10h
        jmp    SHORT loc_3_1E7E
loc_3_1E76:
        and    ecx, 0fffffff8h
        sub    eax, ecx
        cmp    eax, 18h
loc_3_1E7E:
        jge    SHORT loc_3_1E9A
        mov    dword ptr [ebx + 0c0h], ecx
        mov    eax, dword ptr [ebx + 73h]
        mov    ecx, dword ptr [ebx + 7bh]
        sub    eax, ecx
        jge    SHORT loc_3_1E95
        neg    eax
        mov    ecx, dword ptr [ebx + 73h]
loc_3_1E95:
        cmp    eax, 4
        jle    SHORT loc_3_1EB4
loc_3_1E9A:
        mov    eax, dword ptr [ebx + 6fh]
        and    eax, 0fffffff8h
        mov    dword ptr [ebx + 0c0h], eax
        mov    eax, dword ptr [ebx + 73h]
        mov    dword ptr [ebx + 0c4h], eax
        sub    eax, eax
        inc    edx
        jmp    SHORT loc_3_1EBA
loc_3_1EB4:
        mov    dword ptr [ebx + 0c4h], ecx
loc_3_1EBA:
        add    eax, 10h
        mov    dword ptr [ebx + 0c8h], eax
        mov    eax, dword ptr [ebx + 0c0h]
        mov    esi, dword ptr [ebx + 0c4h]
        call   sub_3_1ED8
        mov    dword ptr [ebx + 6], esi
        ret
sub_3_1ED8:
        push   edx
        push   edi
        xchg   esi, eax
        mov    edi, dword ptr [ebx + 7fh]
        or     edi, edi
        je     SHORT loc_3_1EE8
        sar    eax, 1
        jb     SHORT loc_3_1EE8
        sub    edi, edi
loc_3_1EE8:
        imul   dword ptr [ebx + 0d8h]
        xchg   esi, eax
        test   byte ptr [ebx + 9dh], 40h
        jne    SHORT loc_3_1EFB
        sar    eax, 3
loc_3_1EFB:
        add    esi, eax
        add    esi, edi
        pop    edi
        pop    edx
        ret
        db  08Bh, 0C0h
loc_3_1F04:
        lodsw
        and    eax, dword ptr [edi]
        xor    eax, dword ptr [ebx]
        stosw
        add    ebx, 2
        lodsb
        and    al, byte ptr [edi]
        xor    al, byte ptr [ebx]
        stosb
        inc    ebx
        add    edi, 2
        loop   loc_3_1F04
        ret
loc_3_1F1C:
        mov    ah, 1
        push   eax
        and    ah, 0feh
        mov    al, 5
        test   dl, 4
        mov    edx, 3ceh
        pushfd
        call   sub_3_2669
        popfd
        jne    SHORT loc_3_1F72
        mov    al, 8
        call   sub_3_267D
        inc    edx
        mov    eax, eax
loc_3_1F40:
        lodsb
        call   sub_3_2685
        mov    ah, byte ptr [edi]
        mov    al, byte ptr [ebx]
        stosb
        inc    ebx
        lodsb
        call   sub_3_2685
        mov    ah, byte ptr [edi]
        mov    al, byte ptr [ebx]
        stosb
        inc    ebx
        lodsb
        call   sub_3_2685
        mov    ah, byte ptr [edi]
        mov    al, byte ptr [ebx]
        stosb
        inc    ebx
        add    edi, 2
        loop   loc_3_1F40
        mov    al, 0ffh
        call   sub_3_2685
        jmp    SHORT loc_3_1FCC
loc_3_1F72:
        mov    al, 3
        call   sub_3_267D
        inc    edx
        mov    eax, eax
loc_3_1F7C:
        mov    al, 8
        call   sub_3_2685
        mov    al, byte ptr [edi]
        lodsb
        mov    byte ptr [edi], al
        mov    al, 18h
        call   sub_3_2685
        mov    al, byte ptr [edi]
        mov    al, byte ptr [ebx]
        stosb
        inc    ebx
        mov    al, 8
        call   sub_3_2685
        mov    al, byte ptr [edi]
        lodsb
        mov    byte ptr [edi], al
        mov    al, 18h
        call   sub_3_2685
        mov    al, byte ptr [edi]
        mov    al, byte ptr [ebx]
        stosb
        inc    ebx
        mov    al, 8
        call   sub_3_2685
        mov    al, byte ptr [edi]
        lodsb
        mov    byte ptr [edi], al
        mov    al, 18h
        call   sub_3_2685
        mov    al, byte ptr [edi]
        mov    al, byte ptr [ebx]
        stosb
        inc    ebx
        add    edi, 2
        loop   loc_3_1F7C
loc_3_1FCC:
        dec    edx
        pop    eax
        mov    al, 5
        call   sub_3_2669
        ret
        db  08Bh, 0C0h
loc_3_1FD8:
        mov    edx, 3
loc_3_1FDD:
        push   ebx
        mov    bh, byte ptr [ebx]
        mov    bl, byte ptr [esi]
        mov    ah, 8
loc_3_1FE4:
        mov    al, byte ptr [edi]
        shl    bl, 1
        jae    SHORT loc_3_1FEC
        xor    al, al
loc_3_1FEC:
        shl    bh, 1
        jae    SHORT loc_3_1FF2
        xor    al, 0fh
loc_3_1FF2:
        stosb
        dec    ah
        jne    SHORT loc_3_1FE4
        pop    ebx
        inc    ebx
        inc    esi
        dec    edx
        jne    SHORT loc_3_1FDD
        add    edi, 10h
        loop   loc_3_1FD8
        ret
sub_3_2003:
        test   byte ptr [ebx + 9dh], 20h
        je     SHORT sub_3_2048
        mov    eax, dword ptr [ebx + 0e0h]
        mov    dword ptr [ebx + 0b8h], eax
        mov    eax, dword ptr [ebx + 0e8h]
        mov    dword ptr [ebx + 0bch], eax
        call   sub_3_2048
        mov    eax, dword ptr [ebx + 0e4h]
        mov    dword ptr [ebx + 0b8h], eax
        mov    eax, dword ptr [ebx + 0ech]
        mov    dword ptr [ebx + 0bch], eax
        call   sub_3_2048
        ret
        db  090h
sub_3_2048:
        mov    eax, dword ptr [ebx + 6fh]
        mov    esi, dword ptr [ebx + 73h]
        call   sub_3_207C
        push   ebx
        push   edi
        mov    edi, esi
        db  068h, 076h, 020h, 000h, 000h
        push   dword ptr [ebx + 0cch]
        movzx  edx, word ptr [ebx + 9dh]
        mov    esi, dword ptr [ebx + 0f0h]
        mov    ebx, dword ptr [ebx + 0f4h]
        ret
loc_3_2076:
        pop    edi
        pop    ebx
        ret
        db  02Eh, 08Bh, 0C0h
sub_3_207C:
        sub    eax, dword ptr [ebx + 0c0h]
        mov    ecx, 28h
        mov    edx, 10h
        test   byte ptr [ebx + 9dh], 40h
        jne    SHORT loc_3_20A2
        sar    eax, 3
        mov    ecx, 5
        mov    edx, 2
loc_3_20A2:
        xchg   esi, eax
        sub    eax, dword ptr [ebx + 0c4h]
        push   edx
        imul   ecx
        pop    edx
        add    esi, eax
        add    esi, dword ptr [ebx + 0bch]
        mov    ecx, 10h
        mov    eax, edx
        ret
        db  02Eh, 08Bh, 0C0h
sub_3_20C0:
        enter  0ch, 0
        mov    dword ptr [ebp - 4], edx
        add    ecx, 3
        test   byte ptr [ebx + 9dh], 40h
        je     SHORT loc_3_20D6
        shl    ecx, 3
loc_3_20D6:
        mov    eax, dword ptr [ebx + 0d8h]
        sub    eax, ecx
        mov    edx, ecx
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 0ch], 0
        mov    ecx, dword ptr [ebp + 10h]
        test   byte ptr [ebx + 9dh], 40h
        jne    SHORT loc_3_20F9
        sar    ecx, 3
loc_3_20F9:
        or     ecx, ecx
        jge    SHORT loc_3_2105
        neg    ecx
        add    edi, ecx
        add    esi, ecx
        jmp    SHORT loc_3_2110
loc_3_2105:
        add    ecx, dword ptr [ebp - 8]
        sub    ecx, dword ptr [ebx + 0d8h]
        jle    SHORT loc_3_2119
loc_3_2110:
        mov    dword ptr [ebp - 0ch], ecx
        add    eax, ecx
        sub    edx, ecx
        jle    SHORT loc_3_218B
loc_3_2119:
        mov    ecx, dword ptr [ebp + 0ch]
        or     ecx, ecx
        jge    SHORT loc_3_2144
        neg    ecx
        push   ecx
        nop
loc_3_2124:
        add    esi, dword ptr [ebp - 8]
        cmp    dword ptr [ebx + 7fh], 0
        je     SHORT loc_3_213B
        add    edi, dword ptr [ebx + 7fh]
        cmp    edi, dword ptr [ebp - 4]
        jb     SHORT loc_3_213F
        sub    edi, dword ptr [ebx + 7fh]
        sub    edi, dword ptr [ebx + 7fh]
loc_3_213B:
        add    edi, eax
        add    edi, edx
loc_3_213F:
        loop   loc_3_2124
        pop    ecx
        jmp    SHORT loc_3_2150
loc_3_2144:
        add    ecx, dword ptr [ebp + 8]
        sub    cx, word ptr [ebx + 8bh]
        jle    SHORT loc_3_2155
loc_3_2150:
        sub    dword ptr [ebp + 8], ecx
        jle    SHORT loc_3_218B
loc_3_2155:
        mov    dh, byte ptr [ebp + 8]
loc_3_2158:
        sub    ecx, ecx
        mov    cl, dl
        jecxz  loc_3_2166
        push   eax
        nop
loc_3_2160:
        lodsb
        stosb
        dec    ecx
        jne    SHORT loc_3_2160
        pop    eax
loc_3_2166:
        add    edi, eax
        add    esi, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebx + 7fh]
        jecxz  loc_3_2187
        sub    edi, dword ptr [ebx + 0d8h]
        add    edi, ecx
        cmp    edi, dword ptr [ebp - 4]
        jb     SHORT loc_3_2187
        sub    edi, ecx
        sub    edi, ecx
        add    edi, dword ptr [ebx + 0d8h]
loc_3_2187:
        dec    dh
        jne    SHORT loc_3_2158
loc_3_218B:
        leave
        ret
sub_3_218D:
        push   edi
        db  081h, 0BBh, 0CCh, 000h, 000h, 000h, 01Ch, 01Fh, 000h, 000h
        jne    SHORT loc_3_2216
        test   byte ptr [ebx + 9fh], 1
        jne    SHORT loc_3_21E8
        or     byte ptr [ebx + 9fh], 1
        lea    edi, [ebx + 0a1h]
        mov    edx, 3c4h
        mov    ah, 2
        call   sub_3_25A4
        mov    byte ptr [edi + 9], ah
        mov    eax, 0f02h
        call   sub_3_2655
        mov    edx, 3ceh
        mov    ecx, 8
        call   sub_3_2563
        mov    ecx, 708h
        lea    edi, [ebx + 0abh]
        call   sub_3_258E
loc_3_21E8:
        test   byte ptr [ebx + 89h], 1
        je     SHORT loc_3_2206
        mov    eax, 105h
        call   sub_3_2669
        pop    edi
        push   edi
        mov    ecx, dword ptr [ebx + 0dch]
        mov    byte ptr [ecx + edi], al
loc_3_2206:
        mov    ecx, 5
        lea    edi, [ebx + 0abh]
        call   sub_3_258E
loc_3_2216:
        pop    edi
        ret
sub_3_2218:
        push   edi
        db  081h, 0BBh, 0CCh, 000h, 000h, 000h, 01Ch, 01Fh, 000h, 000h
        jne    SHORT loc_3_2297
        test   byte ptr [ebx + 89h], 1
        je     SHORT loc_3_2263
        and    byte ptr [ebx + 9fh], 0feh
        mov    edx, 3ceh
        mov    eax, 3
        call   sub_3_2669
        mov    eax, 0ff08h
        call   sub_3_2669
        mov    eax, 0ff02h
        mov    dl, 0c4h
        call   sub_3_2669
        mov    edx, dword ptr [ebx + 0dch]
        mov    al, byte ptr [edx + edi]
loc_3_2263:
        lea    edi, [ebx + 0a1h]
        mov    edx, 3c4h
        mov    ah, byte ptr [edi + 9]
        mov    al, 2
        call   sub_3_2655
        mov    edx, 3ceh
        mov    ecx, 5
        call   sub_3_258E
        lea    edi, [ebx + 0a1h]
        mov    ecx, 708h
        call   sub_3_258E
loc_3_2297:
        pop    edi
        ret
sub_3_2299:
        mov    dl, 7
        mov    al, byte ptr [ebx + 6fh]
        and    al, dl
        and    dl, byte ptr [ebx + 6eh]
        mov    byte ptr [ebx + 6eh], al
        sub    al, dl
        je     SHORT loc_3_22E2
        push   edi
        jl     SHORT loc_3_22C7
        push   eax
        mov    esi, dword ptr [ebx + 0f0h]
        call   sub_3_2308
        pop    eax
        mov    esi, dword ptr [ebx + 0f4h]
        call   sub_3_2308
        jmp    SHORT loc_3_22E1
loc_3_22C7:
        neg    al
        push   eax
        mov    esi, dword ptr [ebx + 0f0h]
        call   sub_3_22E3
        pop    eax
        mov    esi, dword ptr [ebx + 0f4h]
        call   sub_3_22E3
loc_3_22E1:
        pop    edi
loc_3_22E2:
        ret
sub_3_22E3:
        mov    dl, al
        add    esi, 30h
        dec    esi
        std
        push   esi
        mov    edi, esi
        mov    cl, al
        mov    dh, 30h
        xor    dl, dl
        nop
loc_3_22F4:
        xor    ah, ah
        lodsb
        shl    ax, cl
        or     al, dl
        mov    dl, ah
        stosb
        dec    dh
        jne    SHORT loc_3_22F4
        pop    esi
        cld
        or     byte ptr [esi], dl
        ret
sub_3_2308:
        push   esi
        mov    edi, esi
        mov    cl, al
        xor    dl, dl
        mov    dh, 30h
        mov    eax, eax
loc_3_2314:
        xor    ah, ah
        lodsb
        ror    ax, cl
        or     al, dl
        mov    dl, ah
        stosb
        dec    dh
        jne    SHORT loc_3_2314
        pop    esi
        or     byte ptr [esi], dl
        ret
sub_3_2327:
        db  089h, 00Dh, 041h, 008h, 000h, 000h
        test   byte ptr [ebx + 89h], 4
        jne    NEAR PTR loc_3_2495
        test   byte ptr [ebx + 89h], 1
        jne    SHORT loc_3_236D
        db  00Fh, 0B7h, 005h, 043h, 008h, 000h, 000h
        cdq
        div    word ptr [ebx + 93h]
        db  066h, 0A3h, 043h, 008h, 000h, 000h
        db  00Fh, 0B7h, 005h, 041h, 008h, 000h, 000h
        cdq
        div    word ptr [ebx + 91h]
        db  066h, 0A3h, 041h, 008h, 000h, 000h
loc_3_236D:
        test   byte ptr [ebx + 9dh], 2
        je     SHORT loc_3_23A0
        db  066h, 0A1h, 043h, 008h, 000h, 000h
        cmp    ax, word ptr [ebx + 4]
        jne    SHORT loc_3_2392
        db  066h, 0A1h, 041h, 008h, 000h, 000h
        cmp    ax, word ptr [ebx + 2]
        je     NEAR PTR loc_3_2495
loc_3_2392:
        test   byte ptr [ebx + 89h], 1
        jne    SHORT loc_3_23A0
        call   sub_3_1A3A
loc_3_23A0:
        db  0A1h, 041h, 008h, 000h, 000h
        call   sub_3_2496
        jb     NEAR PTR loc_3_2495
        mov    esi, dword ptr [ebx + 16h]
        test   byte ptr [ebx + 89h], 1
        je     SHORT loc_3_23C6
        call   sub_3_1A79
        jmp    NEAR PTR loc_3_2486
loc_3_23C6:
        mov    ax, word ptr [ebx + 8dh]
        mul    word ptr [ebx + 93h]
        db  066h, 039h, 005h, 043h, 008h, 000h, 000h
        ja     NEAR PTR loc_3_2495
        mov    ax, word ptr [ebx + 8bh]
        mul    word ptr [ebx + 91h]
        db  066h, 08Bh, 00Dh, 041h, 008h, 000h, 000h
        cmp    cx, ax
        ja     NEAR PTR loc_3_2495
        movzx  eax, cx
        movzx  ecx, word ptr [ebx + 8dh]
        test   byte ptr [ebx + 9dh], 8
        je     SHORT loc_3_2457
        mul    ecx
        db  00Fh, 0B7h, 00Dh, 043h, 008h, 000h, 000h
        add    eax, ecx
        pushad
        push   eax
        mov    al, 0eh
        mov    ecx, 0ch
        db  08Bh, 015h, 0F8h, 007h, 000h, 000h
        db  08Bh, 01Dh, 0DCh, 007h, 000h, 000h
        db  0FFh, 015h, 0F4h, 007h, 000h, 000h
        pop    eax
        mov    ah, al
        mov    al, 0fh
        mov    ecx, 0ch
        db  08Bh, 015h, 0F8h, 007h, 000h, 000h
        db  08Bh, 01Dh, 0DCh, 007h, 000h, 000h
        db  0FFh, 015h, 0F4h, 007h, 000h, 000h
        popad
        jmp    SHORT loc_3_2486
loc_3_2457:
        call   sub_3_26F8
        shl    ecx, 1
        mul    ecx
        db  00Fh, 0B7h, 00Dh, 043h, 008h, 000h, 000h
        add    eax, ecx
        add    eax, ecx
        mov    dword ptr [ebx + 6], eax
        add    esi, eax
        mov    ax, word ptr [esi]
        mov    word ptr [ebx + 12h], ax
        and    ax, word ptr [ebx + 28h]
        xor    ax, word ptr [ebx + 26h]
        mov    word ptr [ebx + 14h], ax
        mov    word ptr [esi], ax
loc_3_2486:
        db  0A1h, 041h, 008h, 000h, 000h
        mov    dword ptr [ebx + 2], eax
        or     byte ptr [ebx + 9dh], 2
loc_3_2495:
        ret
sub_3_2496:
        test   byte ptr [ebx + 9fh], 2
        je     SHORT loc_3_24D0
        mov    ecx, eax
        test   byte ptr [ebx + 89h], 1
        je     SHORT loc_3_24B0
        add    ecx, 27001fh
loc_3_24B0:
        cmp    ax, word ptr [ebx + 0eh]
        ja     SHORT loc_3_24D0
        cmp    cx, word ptr [ebx + 0ah]
        jl     SHORT loc_3_24D0
        shr    ecx, 10h
        shr    eax, 10h
        cmp    ax, word ptr [ebx + 10h]
        ja     SHORT loc_3_24D0
        cmp    cx, word ptr [ebx + 0ch]
        jl     SHORT loc_3_24D0
        stc
        ret
loc_3_24D0:
        clc
        ret
sub_3_24D2:
        test   byte ptr [ebx + 89h], 1
        je     SHORT loc_3_24DC
        ret
loc_3_24DC:
        and    byte ptr [ebx + 9dh], 0f7h
        test   byte ptr [ebx + 24h], 1
        je     SHORT loc_3_253D
        or     byte ptr [ebx + 9dh], 8
        movzx  ecx, word ptr [ebx + 26h]
        movzx  eax, word ptr [ebx + 28h]
        mov    ch, al
        and    ecx, 0f0fh
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 1
        mov    word ptr [ebp + 18h], cx
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
loc_3_253D:
        ret
loc_3_253E:
        xor    eax, eax
        db  02Bh, 01Dh, 0D8h, 007h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 00Ah, 000h
        db  003h, 01Dh, 0D8h, 007h, 000h, 000h
        call   sub_3_1A3A
        mov    byte ptr [ebx + 1], 0
        and    byte ptr [ebx + 9fh], 0fdh
        ret
sub_3_2563:
        movzx  eax, ch
        add    edi, eax
        call   sub_3_2619
        push   ax
loc_3_256F:
        mov    al, ch
        call   sub_3_267D
        inc    dl
        call   sub_3_2637
        stosb
        inc    ch
        dec    dl
        cmp    ch, cl
        jbe    SHORT loc_3_256F
        pop    ax
        call   sub_3_267D
        ret
sub_3_258E:
        movzx  eax, ch
        add    edi, eax
loc_3_2593:
        mov    al, ch
        mov    ah, byte ptr [edi]
        inc    edi
        call   sub_3_2669
        inc    ch
        cmp    ch, cl
        jbe    SHORT loc_3_2593
        ret
sub_3_25A4:
        push   ecx
        call   sub_3_25DD
        xchg   al, ah
        call   sub_3_2699
        inc    dl
        call   sub_3_25FB
        xchg   al, ah
        dec    dl
        call   sub_3_2699
        pop    ecx
        ret
sub_3_25C3:
        mov    ah, al
        mov    al, 4
        mov    edx, 3ceh
        jmp    SHORT loc_3_25D7
sub_3_25CE:
        mov    ah, al
        mov    al, 2
        mov    edx, 3c4h
loc_3_25D7:
        call   sub_3_2655
        ret
sub_3_25DD:
        pushad
        mov    ecx, 0
        db  08Bh, 01Dh, 0DCh, 007h, 000h, 000h
        db  0FFh, 015h, 0E4h, 007h, 000h, 000h
        mov    ebx, dword ptr [esp + 1ch]
        mov    bl, al
        mov    dword ptr [esp + 1ch], ebx
        popad
        ret
sub_3_25FB:
        pushad
        mov    ecx, 0
        db  08Bh, 01Dh, 0DCh, 007h, 000h, 000h
        db  0FFh, 015h, 0E8h, 007h, 000h, 000h
        mov    ebx, dword ptr [esp + 1ch]
        mov    bl, al
        mov    dword ptr [esp + 1ch], ebx
        popad
        ret
sub_3_2619:
        pushad
        mov    ecx, 0
        db  08Bh, 01Dh, 0DCh, 007h, 000h, 000h
        db  0FFh, 015h, 0ECh, 007h, 000h, 000h
        mov    ebx, dword ptr [esp + 1ch]
        mov    bl, al
        mov    dword ptr [esp + 1ch], ebx
        popad
        ret
sub_3_2637:
        pushad
        mov    ecx, 0
        db  08Bh, 01Dh, 0DCh, 007h, 000h, 000h
        db  0FFh, 015h, 0F0h, 007h, 000h, 000h
        mov    ebx, dword ptr [esp + 1ch]
        mov    bl, al
        mov    dword ptr [esp + 1ch], ebx
        popad
        ret
sub_3_2655:
        pushad
        mov    ecx, 0ch
loc_3_265B:
        db  08Bh, 01Dh, 0DCh, 007h, 000h, 000h
        db  0FFh, 015h, 0E4h, 007h, 000h, 000h
        popad
        ret
sub_3_2669:
        pushad
        mov    ecx, 0ch
loc_3_266F:
        db  08Bh, 01Dh, 0DCh, 007h, 000h, 000h
        db  0FFh, 015h, 0ECh, 007h, 000h, 000h
        popad
        ret
sub_3_267D:
        pushad
        mov    ecx, 4
        jmp    SHORT loc_3_266F
sub_3_2685:
        pushad
        mov    ecx, 4
        db  08Bh, 01Dh, 0DCh, 007h, 000h, 000h
        db  0FFh, 015h, 0F0h, 007h, 000h, 000h
        popad
        ret
sub_3_2699:
        pushad
        mov    ecx, 4
        jmp    SHORT loc_3_265B
        db  060h, 0B9h, 004h, 000h, 000h, 000h, 08Bh, 01Dh, 0DCh, 007h, 000h, 000h, 0FFh, 015h, 0E8h, 007h
        db  000h, 000h, 061h, 0C3h
loc_3_26B5:
        cmp    ecx, 4
        ja     SHORT loc_3_26C0
        jb     SHORT loc_3_26BE
        out    dx, al
        ret
loc_3_26BE:
        in     al, dx
        ret
loc_3_26C0:
        cmp    ecx, 0ch
        jne    SHORT loc_3_26C8
        out    dx, ax
        ret
loc_3_26C8:
        ret
loc_3_26C9:
        cmp    cx, 1
        mov    ecx, 0f8h
        jb     SHORT loc_3_26DE
        je     SHORT loc_3_26DA
        mov    edi, ebx
        jmp    SHORT loc_3_26DC
loc_3_26DA:
        mov    esi, ebx
loc_3_26DC:
        rep movsb
loc_3_26DE:
        mov    eax, ecx
        ret
loc_3_26E1:
        mov    byte ptr [ebx + 1], cl
        movzx  eax, word ptr [ebx + 96h]
        movzx  ecx, cl
        imul   eax, ecx
        add    eax, dword ptr [ebx + 1ah]
        mov    dword ptr [ebx + 16h], eax
        ret
sub_3_26F8:
        cmp    byte ptr [ebx + 1], 0
        jne    SHORT loc_3_2709
        push   eax
        movzx  eax, word ptr [44eh]
        add    esi, eax
        pop    eax
loc_3_2709:
        ret
loc_3_270A:
        ret
loc_3_270B:
        ret
sub_3_270C:
        db  089h, 00Dh, 05Ch, 001h, 000h, 000h
        or     ecx, ecx
        je     NEAR PTR loc_3_2821
        or     word ptr [ecx], 8
        mov    edi, ecx
        db  0BBh, 0ADh, 001h, 000h, 000h
        mov    dword ptr [ebx], 4
        lea    eax, [edi + 2]
        push   0
        push   ebx
        push   eax
        push   3
        db  068h, 0B5h, 001h, 000h, 000h
        push   0
        push   dword ptr [edi + 0ch]
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        test   eax, eax
        je     SHORT loc_3_27C4
        push   0
        push   dword ptr [edi + 0ch]
        push   ebx
        db  0CDh, 020h, 003h, 000h, 033h, 000h
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_284B
        db  0BEh, 0B1h, 001h, 000h, 000h
        push   2
        push   dword ptr [ebx]
        push   esi
        db  0CDh, 020h, 01Ah, 000h, 033h, 000h
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_284B
        push   0
        push   0
        push   2
        push   dword ptr [esi]
        push   ebx
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_3_284B
        mov    eax, dword ptr [ebx]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edi + 2], ax
        push   0
        push   0
        push   4
        push   dword ptr [esi]
        push   ebx
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_3_284B
        mov    eax, dword ptr [ebx]
        movzx  eax, word ptr [eax + 2]
        mov    byte ptr [edi + 4], al
loc_3_27C4:
        mov    ecx, edi
        db  0BEh, 074h, 001h, 000h, 000h
        movzx  ax, byte ptr [ecx + 4]
        mov    word ptr [esi + 2], ax
        mov    ax, word ptr [ecx + 2]
        mov    word ptr [esi], ax
        db  08Dh, 03Dh, 064h, 001h, 000h, 000h
loc_3_27E1:
        cmp    ax, word ptr [edi]
        je     SHORT loc_3_27EB
        add    edi, 4
        jmp    SHORT loc_3_27E1
loc_3_27EB:
        mov    esi, edi
        call   sub_3_2977
        jb     SHORT loc_3_284C
        call   sub_3_28CB
        jb     SHORT loc_3_284C
        db  08Bh, 035h, 05Ch, 001h, 000h, 000h
        mov    word ptr [esi + 6], 2
        and    word ptr [esi], 0fff7h
        or     word ptr [esi], 4
        db  0A0h, 097h, 006h, 000h, 000h
        mov    byte ptr [esi + 5], al
        db  0C7h, 046h, 018h, 0D5h, 02Ah, 000h, 000h
        clc
        jmp    SHORT loc_3_284C
loc_3_2821:
        db  0BEh, 064h, 001h, 000h, 000h
        mov    cl, 0
loc_3_2828:
        push   ecx
        call   sub_3_2977
        pop    ecx
        jae    SHORT loc_3_283D
        add    esi, 4
        inc    cl
        cmp    cl, 4
        jb     SHORT loc_3_2828
        jmp    SHORT loc_3_284B
loc_3_283D:
        call   sub_3_28CB
        db  080h, 03Dh, 094h, 006h, 000h, 000h, 002h
        je     SHORT loc_3_284C
loc_3_284B:
        stc
loc_3_284C:
        ret
sub_3_284D:
        pushad
        db  0CDh, 020h, 000h, 000h, 00Eh, 000h
        cmp    eax, 400h
        jb     SHORT loc_3_28C8
        movzx  eax, word ptr [esi]
        db  0CDh, 020h, 009h, 000h, 00Eh, 000h
        jae    SHORT loc_3_2870
        cmp    word ptr [esi], 2e8h
        jne    SHORT loc_3_28C8
        stc
        jmp    SHORT loc_3_28C9
loc_3_2870:
        test   edx, edx
        je     SHORT loc_3_2896
        push   eax
        db  08Bh, 00Dh, 05Ch, 001h, 000h, 000h
        jecxz  loc_3_2882
        mov    eax, dword ptr [ecx + 8]
        jmp    SHORT loc_3_2887
loc_3_2882:
        db  0A1h, 079h, 001h, 000h, 000h
loc_3_2887:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 007h, 000h, 00Eh, 000h
        pop    eax
        jmp    SHORT loc_3_28C2
loc_3_2896:
        push   eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   4
        pop    ecx
        xor    esi, esi
        xor    edi, edi
        db  0CDh, 020h, 006h, 000h, 00Eh, 000h
        mov    ebx, eax
        pop    eax
        jb     SHORT loc_3_28C9
        db  08Bh, 00Dh, 05Ch, 001h, 000h, 000h
        jecxz  loc_3_28BC
        mov    dword ptr [ecx + 8], ebx
        jmp    SHORT loc_3_28C2
loc_3_28BC:
        db  089h, 01Dh, 079h, 001h, 000h, 000h
loc_3_28C2:
        db  0CDh, 020h, 00Ah, 000h, 00Eh, 000h
loc_3_28C8:
        clc
loc_3_28C9:
        popad
        ret
sub_3_28CB:
        push   1
        push   0ch
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        jne    SHORT loc_3_28E1
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_3_28E1:
        mov    edx, eax
        mov    ax, word ptr [esi]
        mov    word ptr [edx], ax
        db  0A0h, 078h, 001h, 000h, 000h
        mov    byte ptr [edx + 2], al
        db  0BFh, 03Ch, 001h, 000h, 000h
        movzx  eax, word ptr [esi + 2]
        mov    word ptr [edi], ax
        db  0C7h, 047h, 004h, 004h, 005h, 000h, 000h
        mov    word ptr [edi + 2], 6
        mov    dword ptr [edi + 1ch], edx
        db  08Bh, 00Dh, 05Ch, 001h, 000h, 000h
        mov    dword ptr [edx + 8], ecx
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jae    SHORT loc_3_2928
        mov    edx, esp
        call   sub_3_284D
        stc
        jmp    SHORT loc_3_2976
loc_3_2928:
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        db  08Bh, 00Dh, 05Ch, 001h, 000h, 000h
        jecxz  loc_3_2946
        mov    dword ptr [ecx + 10h], eax
        db  0C7h, 041h, 01Ch, 0FFh, 02Ah, 000h, 000h
loc_3_2946:
        mov    eax, esi
        db  02Dh, 064h, 001h, 000h, 000h
        shr    al, 2
        inc    al
        db  0A2h, 097h, 006h, 000h, 000h
        mov    ax, word ptr [esi]
        db  066h, 0A3h, 07Dh, 001h, 000h, 000h
        mov    ax, word ptr [esi + 2]
        db  0A2h, 095h, 006h, 000h, 000h
        db  0C6h, 005h, 094h, 006h, 000h, 000h, 002h
        call   sub_3_2AD5
        clc
loc_3_2976:
        ret
sub_3_2977:
        xor    edx, edx
        call   sub_3_284D
        jb     SHORT loc_3_2994
        push   esi
        movzx  esi, word ptr [esi]
        call   sub_3_2995
        pop    esi
        jae    SHORT loc_3_2994
        mov    edx, esp
        call   sub_3_284D
        stc
loc_3_2994:
        ret
sub_3_2995:
        lea    edx, [esi + 3]
        in     al, dx
        push   eax
        mov    al, 80h
        jmp    SHORT loc_3_299E
loc_3_299E:
        out    dx, al
        lea    edx, [esi + 1]
        jmp    SHORT loc_3_29A4
loc_3_29A4:
        in     al, dx
        push   eax
        mov    al, 0
        jmp    SHORT loc_3_29AA
loc_3_29AA:
        out    dx, al
        lea    edx, [esi]
        jmp    SHORT loc_3_29AF
loc_3_29AF:
        in     al, dx
        push   eax
        mov    al, 60h
        jmp    SHORT loc_3_29B5
loc_3_29B5:
        out    dx, al
        lea    edx, [esi + 3]
        mov    al, 2
        out    dx, al
        lea    edx, [esi + 1]
        jmp    SHORT loc_3_29C1
loc_3_29C1:
        in     al, dx
        push   eax
        xor    al, al
        jmp    SHORT loc_3_29C7
loc_3_29C7:
        out    dx, al
        lea    edx, [esi + 3]
        jmp    SHORT loc_3_29CD
loc_3_29CD:
        in     al, dx
        lea    edx, [esi + 4]
        jmp    SHORT loc_3_29D3
loc_3_29D3:
        in     al, dx
        push   eax
        db  0C7h, 005h, 0BDh, 001h, 000h, 000h, 064h, 000h, 000h, 000h
        db  0C7h, 005h, 0C1h, 001h, 000h, 000h, 032h, 000h, 000h, 000h
        call   sub_3_2A43
        jae    SHORT loc_3_2A3A
        lea    edx, [esi + 4]
        db  0C7h, 005h, 0BDh, 001h, 000h, 000h, 0F4h, 001h, 000h, 000h
        db  0C7h, 005h, 0C1h, 001h, 000h, 000h, 090h, 001h, 000h, 000h
        call   sub_3_2A43
        jae    SHORT loc_3_2A3A
        lea    edx, [esi + 4]
        pop    eax
        jmp    SHORT loc_3_2A14
loc_3_2A14:
        out    dx, al
        lea    edx, [esi + 1]
        pop    eax
        jmp    SHORT loc_3_2A1B
loc_3_2A1B:
        out    dx, al
        lea    edx, [esi + 3]
        mov    al, 80h
        jmp    SHORT loc_3_2A23
loc_3_2A23:
        out    dx, al
        lea    edx, [esi]
        pop    eax
        jmp    SHORT loc_3_2A29
loc_3_2A29:
        out    dx, al
        lea    edx, [esi + 1]
        pop    eax
        jmp    SHORT loc_3_2A30
loc_3_2A30:
        out    dx, al
        lea    edx, [esi + 3]
        pop    eax
        jmp    SHORT loc_3_2A37
loc_3_2A37:
        out    dx, al
        stc
        ret
loc_3_2A3A:
        inc    edx
        mov    al, 1
        out    dx, al
        add    esp, 14h
        clc
        ret
sub_3_2A43:
        mov    al, 1
        out    dx, al
        sub    dl, 4
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    ecx, eax
        in     al, dx
loc_3_2A52:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        cmp    eax, ecx
        je     SHORT loc_3_2A52
        in     al, dx
        db  08Bh, 03Dh, 0BDh, 001h, 000h, 000h
loc_3_2A63:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, ecx
        cmp    edi, eax
        jae    SHORT loc_3_2A63
        add    dl, 5
        in     al, dx
        dec    dx
        mov    al, 0bh
        out    dx, al
        mov    ebx, 4d0000h
        mov    bx, 4203h
loc_3_2A81:
        lea    edx, [esi + 5]
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    ecx, eax
        db  08Bh, 03Dh, 0C1h, 001h, 000h, 000h
loc_3_2A92:
        in     al, dx
        test   al, 1
        jne    SHORT loc_3_2AA5
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, ecx
        cmp    edi, eax
        jae    SHORT loc_3_2A92
        stc
        ret
loc_3_2AA5:
        mov    edx, esi
        in     al, dx
        cmp    al, bh
        jne    SHORT loc_3_2ABC
        db  0C6h, 005h, 078h, 001h, 000h, 000h, 004h
        db  066h, 0FFh, 005h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_3_2AD3
loc_3_2ABC:
        ror    ebx, 8
        cmp    al, bh
        je     SHORT loc_3_2ACC
        rol    ebx, 8
        dec    bl
        jne    SHORT loc_3_2A81
        stc
        ret
loc_3_2ACC:
        db  0C6h, 005h, 078h, 001h, 000h, 000h, 003h
loc_3_2AD3:
        clc
        ret
sub_3_2AD5:
        movzx  edx, word ptr [esi]
        add    dl, 4
        mov    al, 3
        out    dx, al
        jmp    SHORT loc_3_2AE0
loc_3_2AE0:
        jmp    SHORT loc_3_2AE2
loc_3_2AE2:
        sub    dl, 3
        mov    al, 1
        out    dx, al
        jmp    SHORT loc_3_2AEA
loc_3_2AEA:
        inc    dl
        xor    al, al
        out    dx, al
        add    dl, 2
        jmp    SHORT loc_3_2AF4
loc_3_2AF4:
        jmp    SHORT loc_3_2AF6
loc_3_2AF6:
        in     al, dx
        or     al, 8
        jmp    SHORT loc_3_2AFB
loc_3_2AFB:
        jmp    SHORT loc_3_2AFD
loc_3_2AFD:
        out    dx, al
        ret
loc_3_2AFF:
        movzx  edx, word ptr [esi + 2]
        or     edx, edx
        je     SHORT loc_3_2B0E
        add    dl, 4
        in     al, dx
        and    al, 0f7h
        out    dx, al
loc_3_2B0E:
        call   sub_3_2D0B
        xor    eax, eax
        xchg   dword ptr [esi + 8], eax
        cmp    eax, 0ffffh
        ja     SHORT loc_3_2B2B
        xor    eax, eax
        db  087h, 005h, 079h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_2B37
loc_3_2B2B:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 007h, 000h, 00Eh, 000h
loc_3_2B37:
        ret
sub_3_2B38:
        db  089h, 00Dh, 05Ch, 001h, 000h, 000h
        jecxz  loc_3_2BBB
        db  080h, 03Dh, 0C8h, 001h, 000h, 000h, 000h
        je     SHORT loc_3_2B6B
        movzx  edx, word ptr [ecx + 2]
        cmp    edx, 23ch
        db  0BEh, 09Ch, 004h, 000h, 000h
        je     SHORT loc_3_2B79
        cmp    edx, 238h
        db  0BEh, 094h, 004h, 000h, 000h
        je     SHORT loc_3_2B79
loc_3_2B67:
        or     word ptr [ecx], 20h
loc_3_2B6B:
        stc
        ret
loc_3_2B6D:
        db  08Bh, 00Dh, 05Ch, 001h, 000h, 000h
        or     word ptr [ecx], 40h
        stc
        ret
loc_3_2B79:
        call   sub_3_2C5A
        jb     SHORT loc_3_2B67
        movzx  ecx, byte ptr [ecx + 4]
        cmp    cl, 9
        jne    SHORT loc_3_2B8B
        mov    cl, 2
loc_3_2B8B:
        mov    al, 1
        shl    eax, cl
        db  0A2h, 0C9h, 001h, 000h, 000h
        call   sub_3_2C86
        jb     SHORT loc_3_2B6D
        call   sub_3_2C05
        jb     SHORT loc_3_2BBA
        db  08Bh, 00Dh, 05Ch, 001h, 000h, 000h
        or     word ptr [ecx], 4
        db  0C7h, 041h, 018h, 0EEh, 02Ch, 000h, 000h
        mov    word ptr [ecx + 6], 3
        clc
loc_3_2BBA:
        ret
loc_3_2BBB:
        mov    edx, 23ch
        db  0BEh, 09Ch, 004h, 000h, 000h
        call   sub_3_2BE1
        mov    edx, 238h
        db  0BEh, 094h, 004h, 000h, 000h
        call   sub_3_2BE1
        db  0A0h, 0C8h, 001h, 000h, 000h
        ror    al, 1
        ret
sub_3_2BE1:
        call   sub_3_2C5A
        jb     SHORT loc_3_2C59
        db  0C6h, 005h, 0C9h, 001h, 000h, 000h, 0BCh
        call   sub_3_2C86
        jb     SHORT loc_3_2C59
        db  081h, 0FEh, 09Ch, 004h, 000h, 000h
        je     SHORT sub_3_2C05
        db  0C6h, 005h, 097h, 006h, 000h, 000h, 004h
sub_3_2C05:
        db  0C6h, 005h, 094h, 006h, 000h, 000h, 003h
        db  088h, 01Dh, 095h, 006h, 000h, 000h
        cmp    bl, 2
        jne    SHORT loc_3_2C19
        mov    bl, 9
loc_3_2C19:
        db  0BFh, 03Ch, 001h, 000h, 000h
        mov    byte ptr [edi], bl
        mov    dword ptr [edi + 4], esi
        push   edx
        db  08Bh, 015h, 05Ch, 001h, 000h, 000h
        mov    ecx, edx
        mov    word ptr [edi + 2], 4
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        pop    edx
        jb     SHORT loc_3_2C59
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        jecxz  loc_3_2C4D
        mov    dword ptr [ecx + 10h], eax
        db  0C7h, 041h, 01Ch, 002h, 02Dh, 000h, 000h
loc_3_2C4D:
        call   sub_3_2CEE
        db  0C6h, 005h, 0C8h, 001h, 000h, 000h, 000h
loc_3_2C59:
        ret
sub_3_2C5A:
        push   edx
        add    edx, 2
        mov    ah, 0deh
        in     al, dx
        cmp    al, ah
        je     SHORT loc_3_2C6D
        in     al, dx
        cmp    al, ah
        je     SHORT loc_3_2C6D
loc_3_2C6A:
        pop    edx
        stc
        ret
loc_3_2C6D:
        in     al, dx
        mov    bl, al
        in     al, dx
        cmp    al, ah
        jne    SHORT loc_3_2C6A
        in     al, dx
        cmp    al, bl
        jne    SHORT loc_3_2C6A
        cmp    al, 11h
        ja     SHORT loc_3_2C83
        db  0A2h, 0D4h, 007h, 000h, 000h
loc_3_2C83:
        pop    edx
        clc
        ret
sub_3_2C86:
        cli
        in     al, 21h
        push   eax
        mov    al, 80h
        out    dx, al
        jmp    SHORT loc_3_2C8F
loc_3_2C8F:
        jmp    SHORT loc_3_2C91
loc_3_2C91:
        jmp    SHORT loc_3_2C93
loc_3_2C93:
        mov    al, 7
        out    dx, al
        db  08Ah, 025h, 0C9h, 001h, 000h, 000h
        mov    al, ah
        out    21h, al
        jmp    SHORT loc_3_2CA2
loc_3_2CA2:
        mov    al, 0ah
        out    20h, al
        jmp    SHORT loc_3_2CA8
loc_3_2CA8:
        inc    edx
        movzx  ebx, ah
        mov    ecx, 0ah
loc_3_2CB1:
        mov    al, 10h
        out    dx, al
        jmp    SHORT loc_3_2CB6
loc_3_2CB6:
        in     al, 20h
        and    al, ah
        xor    al, ah
        and    bl, al
        je     SHORT loc_3_2CEB
        mov    al, 16h
        out    dx, al
        jmp    SHORT loc_3_2CC5
loc_3_2CC5:
        in     al, 20h
        and    al, ah
        and    bl, al
        je     SHORT loc_3_2CEB
        loop   loc_3_2CB1
        bsf    eax, ebx
        je     SHORT loc_3_2CEB
        btr    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_3_2CEB
        mov    ebx, eax
        clc
loc_3_2CDE:
        mov    al, 0
        out    dx, al
        dec    edx
        mov    al, 0bh
        out    20h, al
        pop    eax
        out    21h, al
        sti
        ret
loc_3_2CEB:
        stc
        jmp    SHORT loc_3_2CDE
sub_3_2CEE:
        mov    al, 80h
        out    dx, al
        jmp    SHORT loc_3_2CF3
loc_3_2CF3:
        jmp    SHORT loc_3_2CF5
loc_3_2CF5:
        jmp    SHORT loc_3_2CF7
loc_3_2CF7:
        mov    al, 7
        out    dx, al
        jmp    SHORT loc_3_2CFC
loc_3_2CFC:
        inc    edx
        mov    al, 9
        out    dx, al
        clc
        ret
loc_3_2D02:
        db  0C6h, 005h, 0C8h, 001h, 000h, 000h, 0FFh
        jmp    SHORT sub_3_2D0B
sub_3_2D0B:
        xor    eax, eax
        xchg   dword ptr [esi + 10h], eax
        or     eax, eax
        je     SHORT loc_3_2D1A
        db  0CDh, 020h, 011h, 000h, 003h, 000h
loc_3_2D1A:
        ret
sub_3_2D1B:
        db  089h, 00Dh, 05Ch, 001h, 000h, 000h
        jecxz  sub_3_2D4F
        movzx  edx, word ptr [ecx + 2]
        cmp    edx, 23ch
        jne    SHORT loc_3_2D49
        push   ecx
        call   sub_3_2D4F
        pop    ecx
        jb     SHORT loc_3_2D43
        mov    word ptr [ecx + 6], 1
        or     word ptr [ecx], 4
        ret
loc_3_2D43:
        or     word ptr [ecx], 8
        stc
        ret
loc_3_2D49:
        or     word ptr [ecx], 20h
        stc
        ret
sub_3_2D4F:
        mov    edx, 23ch
        call   sub_3_2C5A
        cmc
        jb     SHORT loc_3_2DAC
        call   sub_3_2DAD
        jb     SHORT loc_3_2DAC
        db  0C6h, 005h, 094h, 006h, 000h, 000h, 001h
        db  0A2h, 095h, 006h, 000h, 000h
        cmp    al, 2
        jne    SHORT loc_3_2D75
        mov    al, 9
loc_3_2D75:
        db  0BFh, 03Ch, 001h, 000h, 000h
        mov    byte ptr [edi], al
        db  0C7h, 047h, 004h, 014h, 001h, 000h, 000h
        mov    word ptr [edi + 2], 4
        db  08Bh, 015h, 05Ch, 001h, 000h, 000h
        mov    ecx, edx
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     SHORT loc_3_2DAC
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        jecxz  loc_3_2DAB
        mov    dword ptr [ecx + 10h], eax
        db  0C7h, 041h, 01Ch, 014h, 02Eh, 000h, 000h
loc_3_2DAB:
        clc
loc_3_2DAC:
        ret
sub_3_2DAD:
        mov    edx, 23fh
        mov    al, 91h
        out    dx, al
        jmp    SHORT loc_3_2DB7
loc_3_2DB7:
        mov    al, 0a5h
        mov    ah, al
        dec    edx
        dec    edx
        out    dx, al
        jmp    SHORT loc_3_2DC0
loc_3_2DC0:
        mov    al, 11h
        inc    edx
        inc    edx
        out    dx, al
        jmp    SHORT loc_3_2DC7
loc_3_2DC7:
        dec    edx
        dec    edx
        in     al, dx
        cmp    al, ah
        jne    SHORT loc_3_2E12
        inc    edx
        push   3
        pop    edi
loc_3_2DD2:
        inc    edx
        mov    al, 91h
        out    dx, al
        jmp    SHORT loc_3_2DD8
loc_3_2DD8:
        jmp    SHORT loc_3_2DDA
loc_3_2DDA:
        dec    edx
        xor    al, al
        out    dx, al
        jmp    SHORT loc_3_2DE0
loc_3_2DE0:
        jmp    SHORT loc_3_2DE2
loc_3_2DE2:
        in     al, dx
        jmp    SHORT loc_3_2DE5
loc_3_2DE5:
        mov    ah, al
        xor    ebx, ebx
        mov    ecx, 2710h
loc_3_2DEE:
        in     al, dx
        jmp    SHORT loc_3_2DF1
loc_3_2DF1:
        xor    ah, al
        or     bl, ah
        mov    ah, al
        loop   loc_3_2DEE
        mov    al, 10h
        out    dx, al
        bsf    ecx, ebx
        jne    SHORT loc_3_2E06
        dec    edi
        jne    SHORT loc_3_2DD2
        jmp    SHORT loc_3_2E12
loc_3_2E06:
        xor    al, al
        out    dx, al
        mov    eax, 5
        sub    eax, ecx
        clc
        ret
loc_3_2E12:
        stc
        ret
loc_3_2E14:
        mov    edx, 23fh
        mov    al, 10h
        out    dx, al
        call   sub_3_2D0B
        ret
sub_3_2E22:
        db  089h, 00Dh, 05Ch, 001h, 000h, 000h
        jecxz  loc_3_2E36
        cmp    byte ptr [ecx + 4], 0ch
        je     SHORT loc_3_2E36
        or     word ptr [ecx], 40h
        stc
        ret
loc_3_2E36:
        pushad
        xor    ebx, ebx
        db  038h, 01Dh, 095h, 007h, 000h, 000h
        jne    NEAR PTR loc_3_319B
        db  088h, 01Dh, 08Bh, 007h, 000h, 000h
        db  088h, 01Dh, 099h, 007h, 000h, 000h
        db  066h, 089h, 01Dh, 084h, 007h, 000h, 000h
        db  0C6h, 005h, 096h, 007h, 000h, 000h, 0FFh
        jecxz  loc_3_2E8F
        mov    edx, 0fe010h
        mov    ebx, dword ptr [edx]
        mov    edx, dword ptr [edx + 4]
        cmp    ebx, 48534f54h
        jne    SHORT loc_3_2E8F
        and    edx, 0ffffffh
        cmp    edx, 414249h
        jne    SHORT loc_3_2E8F
        db  0C6h, 005h, 08Ch, 007h, 000h, 000h, 001h
        db  0C6h, 005h, 08Bh, 007h, 000h, 000h, 002h
loc_3_2E8F:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        call   sub_3_32F7
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        push   ebx
        mov    word ptr [ebp + 1ch], 0f963h
        mov    word ptr [ebp + 10h], 4954h
        mov    word ptr [ebp + 14h], 0
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     SHORT loc_3_2F33
        mov    word ptr [ebp + 38h], 0
        mov    word ptr [ebp + 1ch], 0f95fh
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     SHORT loc_3_2F1E
        xor    ebx, ebx
        xor    eax, eax
        mov    bx, word ptr [ebp + 38h]
        shl    ebx, 4
        mov    ax, word ptr [ebp + 10h]
        add    ebx, eax
        cmp    ebx, 4954h
        je     SHORT loc_3_2F33
        cmp    word ptr [ebx], 0ah
        jl     SHORT loc_3_2F33
        test   word ptr [ebx + 14h], 40h
        je     SHORT loc_3_2F33
loc_3_2F1E:
        db  0C6h, 005h, 08Ah, 007h, 000h, 000h, 001h
        db  0C6h, 005h, 08Bh, 007h, 000h, 000h, 002h
        db  0C6h, 005h, 08Ch, 007h, 000h, 000h, 001h
loc_3_2F33:
        pop    ebx
        db  080h, 03Dh, 08Bh, 007h, 000h, 000h, 000h
        jne    SHORT loc_3_2F64
        db  0FEh, 005h, 0C5h, 001h, 000h, 000h
        mov    eax, 11h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        db  0FEh, 00Dh, 0C5h, 001h, 000h, 000h
        test   byte ptr [ebp + 1ch], 4
        je     NEAR PTR loc_3_30B2
loc_3_2F64:
        mov    word ptr [ebp + 1ch], 0c205h
        mov    byte ptr [ebp + 11h], 3
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     NEAR PTR loc_3_30B2
        mov    word ptr [ebp + 1ch], 0c201h
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     NEAR PTR loc_3_30B2
        mov    word ptr [ebp + 1ch], 0c202h
        mov    byte ptr [ebp + 11h], 0
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     NEAR PTR loc_3_30B2
        mov    word ptr [ebp + 1ch], 0c204h
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     NEAR PTR loc_3_30B2
        movzx  ax, byte ptr [ebp + 11h]
        db  066h, 0A3h, 084h, 007h, 000h, 000h
        or     ax, ax
        je     SHORT loc_3_2FF6
        db  066h, 0A3h, 004h, 000h, 000h, 000h
loc_3_2FF6:
        mov    word ptr [ebp + 1ch], 0c203h
        mov    byte ptr [ebp + 11h], 3
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     NEAR PTR loc_3_30B2
        db  0A1h, 0CEh, 001h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_3_3030
        db  0BEh, 0E0h, 00Eh, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        db  0A3h, 0CEh, 001h, 000h, 000h
loc_3_3030:
        mov    word ptr [ebp + 1ch], 0c207h
        mov    word ptr [ebp + 10h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     SHORT loc_3_30B2
        mov    word ptr [ebp + 1ch], 0c206h
        mov    byte ptr [ebp + 11h], 1
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     SHORT loc_3_30B2
        mov    word ptr [ebp + 1ch], 0c202h
        mov    byte ptr [ebp + 11h], 2
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     SHORT loc_3_30B2
        mov    word ptr [ebp + 1ch], 0c200h
        mov    byte ptr [ebp + 11h], 1
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jb     SHORT loc_3_30B2
        db  0C6h, 005h, 08Bh, 007h, 000h, 000h, 001h
loc_3_30B2:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        call   sub_3_32B4
        xor    eax, eax
        db  0A2h, 096h, 007h, 000h, 000h
        inc    al
        db  086h, 005h, 0CDh, 001h, 000h, 000h
        or     al, al
        jne    SHORT loc_3_30F8
        mov    al, 15h
        db  0BEh, 0CCh, 001h, 000h, 000h
        db  0FEh, 005h, 0C5h, 001h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        db  0FEh, 00Dh, 0C5h, 001h, 000h, 000h
loc_3_30F8:
        db  038h, 025h, 08Bh, 007h, 000h, 000h
        je     NEAR PTR loc_3_31C6
        db  0BFh, 03Ch, 001h, 000h, 000h
        mov    word ptr [edi], 0ch
        db  0C7h, 047h, 004h, 000h, 003h, 000h, 000h
        mov    word ptr [edi + 2], 6
        db  08Bh, 015h, 05Ch, 001h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     NEAR PTR loc_3_31D3
        db  0A3h, 080h, 007h, 000h, 000h
        db  080h, 03Dh, 0CCh, 001h, 000h, 000h, 000h
        jne    SHORT loc_3_3181
        mov    eax, 15h
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        db  089h, 00Dh, 078h, 007h, 000h, 000h
        db  089h, 015h, 07Ch, 007h, 000h, 000h
        db  0BEh, 0A8h, 001h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 10h
        mov    eax, 15h
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        push   0
        push   7
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        db  0A3h, 090h, 007h, 000h, 000h
loc_3_3181:
        db  080h, 00Dh, 0CCh, 001h, 000h, 000h, 001h
        call   sub_3_0DE0
        db  0C6h, 005h, 094h, 006h, 000h, 000h, 004h
        db  0C6h, 005h, 095h, 006h, 000h, 000h, 00Ch
loc_3_319B:
        db  08Bh, 00Dh, 05Ch, 001h, 000h, 000h
        jecxz  loc_3_31BC
        or     word ptr [ecx], 4
        mov    word ptr [ecx + 6], 4
        db  0A1h, 080h, 007h, 000h, 000h
        mov    dword ptr [ecx + 10h], eax
        db  0C7h, 041h, 01Ch, 0DFh, 031h, 000h, 000h
loc_3_31BC:
        db  0C6h, 005h, 095h, 007h, 000h, 000h, 0FFh
        popad
        clc
        ret
loc_3_31C6:
        mov    ecx, dword ptr [esp + 18h]
        jecxz  loc_3_31D0
        or     word ptr [ecx], 8
loc_3_31D0:
        popad
        stc
        ret
loc_3_31D3:
        mov    ecx, dword ptr [esp + 18h]
        jecxz  loc_3_31D0
        or     word ptr [ecx], 40h
        jmp    SHORT loc_3_31D0
loc_3_31DF:
        push   esi
        xor    eax, eax
        mov    al, 15h
        db  0BEh, 0CCh, 001h, 000h, 000h
        db  0CDh, 020h, 018h, 001h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        call   sub_3_0DFD
        pop    ebp
        pop    esi
        call   sub_3_2D0B
        xor    eax, eax
        db  0A2h, 097h, 007h, 000h, 000h
        db  0A2h, 095h, 007h, 000h, 000h
        db  0A2h, 0CDh, 001h, 000h, 000h
        ret
sub_3_3217:
        pushad
        call   sub_3_2D0B
        call   sub_3_32F7
        db  0C6h, 005h, 096h, 007h, 000h, 000h, 0FFh
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        push   0ah
        pop    ecx
loc_3_324A:
        mov    word ptr [ebp + 1ch], 0c200h
        mov    byte ptr [ebp + 11h], 0
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jae    SHORT loc_3_3269
        loop   loc_3_324A
loc_3_3269:
        mov    cl, 0ah
loc_3_326B:
        mov    word ptr [ebp + 1ch], 0c200h
        mov    byte ptr [ebp + 11h], 1
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [ebp + 2ch], 0
        jae    SHORT loc_3_328A
        loop   loc_3_326B
loc_3_328A:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        call   sub_3_32B4
        db  0C6h, 005h, 096h, 007h, 000h, 000h, 000h
        db  0C6h, 005h, 095h, 007h, 000h, 000h, 000h
        popad
        ret
sub_3_32B4:
        mov    edx, 61h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_32CE
        db  0CDh, 020h, 00Fh, 080h, 00Dh, 000h
loc_3_32CE:
        mov    edx, 60h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 64h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        mov    al, 0aeh
        call   sub_3_3348
        ret
sub_3_32F7:
        mov    edx, 61h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_3311
        db  0CDh, 020h, 010h, 080h, 00Dh, 000h
loc_3_3311:
        push   ebx
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        mov    edx, 60h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 64h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    al, 0adh
        call   sub_3_3348
        pop    ebx
        ret
sub_3_3348:
        push   eax
        push   ecx
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_3359
        db  0CDh, 020h, 008h, 000h, 003h, 000h
loc_3_3359:
        mov    ecx, 10000h
loc_3_335E:
        in     al, 64h
        test   al, 2
        loopne loc_3_335E
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_3373
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_3_3373:
        pop    ecx
        pop    eax
        out    64h, al
        ret
sub_3_3378:
        db  0A0h, 094h, 006h, 000h, 000h
        test   al, al
        je     SHORT loc_3_338E
        cmp    al, 4
        jne    SHORT loc_3_33DF
        db  080h, 03Dh, 08Bh, 007h, 000h, 000h, 001h
        je     SHORT loc_3_33DF
loc_3_338E:
        db  08Bh, 035h, 060h, 001h, 000h, 000h
        test   esi, esi
        je     SHORT loc_3_33BE
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_3_33BE
loc_3_33A0:
        mov    edx, dword ptr [eax + 20h]
        test   edx, edx
        je     SHORT loc_3_33B6
        push   eax
        push   ebx
        push   esi
        xor    ecx, ecx
        mov    eax, 1bh
        call   edx
        pop    esi
        pop    ebx
        pop    eax
loc_3_33B6:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_3_33A0
loc_3_33BE:
        db  080h, 03Dh, 094h, 006h, 000h, 000h, 000h
        jne    SHORT loc_3_33DF
        db  0A0h, 0A8h, 000h, 000h, 000h
        cmp    al, 0ffh
        je     SHORT loc_3_3403
        cmp    al, 3
        jb     SHORT loc_3_33FA
        ja     SHORT loc_3_33E0
        xor    ecx, ecx
        call   sub_3_270C
        jb     SHORT loc_3_3403
loc_3_33DF:
        ret
loc_3_33E0:
        cmp    al, 9
        jne    SHORT loc_3_33EF
        xor    ecx, ecx
        call   sub_3_2B38
        jae    SHORT loc_3_33DF
        jb     SHORT loc_3_3403
loc_3_33EF:
        xor    ecx, ecx
        call   sub_3_2E22
        jae    SHORT loc_3_33DF
        jb     SHORT loc_3_3403
loc_3_33FA:
        xor    ecx, ecx
        call   sub_3_2D1B
        jae    SHORT loc_3_33DF
loc_3_3403:
        xor    ecx, ecx
        call   sub_3_2B38
        jae    SHORT loc_3_33DF
        xor    ecx, ecx
        call   sub_3_2D1B
        jae    SHORT loc_3_33DF
        xor    ecx, ecx
        call   sub_3_2E22
        jae    SHORT loc_3_33DF
        xor    ecx, ecx
        call   sub_3_270C
        jmp    SHORT loc_3_33DF
        db  033h, 0C9h, 0E8h, 00Ah, 0F7h, 0FFh, 0FFh, 033h, 0C9h, 0E8h, 0E6h, 0F8h, 0FFh, 0FFh, 033h, 0C9h
        db  0E8h, 0E6h, 0F9h, 0FFh, 0FFh, 033h, 0C9h, 0E8h, 0C9h, 0F2h, 0FFh, 0FFh, 0C3h
loc_3_3444:
        mov    eax, 21h
        db  0F6h, 005h, 006h, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_3_353C
        cmp    edx, 1
        jne    NEAR PTR loc_3_353C
        call   sub_3_3545
        jae    SHORT loc_3_3470
        mov    eax, 2
        jmp    NEAR PTR loc_3_353C
loc_3_3470:
        mov    edi, eax
        mov    dword ptr [edi + 0ch], ebx
        db  0B8h, 07Fh, 001h, 000h, 000h
        mov    ecx, 1
        mov    edx, ebx
        call   sub_3_3585
        jne    SHORT loc_3_34F4
        push   edi
        push   ebx
        push   8
        pop    ecx
        cld
loc_3_348E:
        lodsb
        or     al, al
        je     SHORT loc_3_3499
        cmp    al, 2eh
        je     SHORT loc_3_3499
        loop   loc_3_348E
loc_3_3499:
        jecxz  loc_3_34A2
        mov    edi, esi
        dec    edi
        mov    al, 20h
        rep stosb
loc_3_34A2:
        db  0BFh, 095h, 001h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        pop    ebx
        pop    edi
        jecxz  loc_3_34BB
        mov    eax, dword ptr [ecx + 18h]
        mov    dword ptr [edi + 20h], eax
        jmp    SHORT loc_3_3528
loc_3_34BB:
        mov    esi, edi
        db  0BFh, 095h, 001h, 000h, 000h
        mov    al, 20h
        mov    ecx, 8
        repne scasb
        jne    SHORT loc_3_34CE
        dec    edi
loc_3_34CE:
        mov    dword ptr [edi], 6478762eh
        mov    byte ptr [edi + 4], 0
        db  0BAh, 095h, 001h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        mov    edi, esi
        jb     SHORT loc_3_3528
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [edi + 20h], eax
        mov    dword ptr [edi + 24h], edx
        jmp    SHORT loc_3_3528
loc_3_34F4:
        db  0B8h, 08Bh, 001h, 000h, 000h
        mov    ecx, 1
        call   sub_3_3585
        je     SHORT loc_3_3516
        db  0B8h, 08Bh, 001h, 000h, 000h
        mov    ecx, 0
        call   sub_3_3585
        jne    SHORT loc_3_353E
loc_3_3516:
        push   edi
        push   ebx
        mov    edi, esi
        call   sub_3_0FC3
        pop    ebx
        pop    edi
        or     edx, edx
        je     SHORT loc_3_353E
        mov    dword ptr [edi + 20h], edx
loc_3_3528:
        push   3
        push   edi
        db  068h, 0AFh, 035h, 000h, 000h
        push   ebx
        db  0CDh, 020h, 00Eh, 000h, 033h, 000h
        add    esp, 10h
        xor    eax, eax
loc_3_353C:
        stc
        ret
loc_3_353E:
        mov    eax, 13h
        stc
        ret
sub_3_3545:
        db  08Bh, 035h, 060h, 001h, 000h, 000h
        or     esi, esi
        jne    SHORT loc_3_3567
        mov    eax, 4
        mov    ecx, 28h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_3_3584
        db  089h, 035h, 060h, 001h, 000h, 000h
loc_3_3567:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_3_3584
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        push   eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 0ah
        cld
        rep stosd
        pop    eax
        clc
loc_3_3584:
        ret
sub_3_3585:
        push   edx
        db  0C7h, 005h, 0ADh, 001h, 000h, 000h, 031h, 000h, 000h, 000h
        db  0BEh, 095h, 001h, 000h, 000h
        push   ecx
        db  068h, 0ADh, 001h, 000h, 000h
        push   esi
        push   1
        push   eax
        push   0
        push   edx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        pop    edx
        test   eax, eax
        ret
loc_3_35AF:
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [esp + 10h]
        cmp    eax, 1
        jne    NEAR PTR loc_3_365E
        mov    edi, dword ptr [esp + 1ch]
        db  0BEh, 0B1h, 001h, 000h, 000h
        push   2
        push   dword ptr [edi + 0ch]
        push   esi
        db  0CDh, 020h, 01Ah, 000h, 033h, 000h
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_362A
        db  0B8h, 0ADh, 001h, 000h, 000h
        push   0
        push   0
        push   2
        push   dword ptr [esi]
        push   eax
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_3_3603
        db  0A1h, 0ADh, 001h, 000h, 000h
        movzx  eax, word ptr [eax + 4]
        mov    word ptr [edi + 2], ax
loc_3_3603:
        db  0B8h, 0ADh, 001h, 000h, 000h
        push   0
        push   0
        push   4
        push   dword ptr [esi]
        push   eax
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_3_362A
        db  0A1h, 0ADh, 001h, 000h, 000h
        movzx  eax, word ptr [eax + 2]
        mov    byte ptr [edi + 4], al
loc_3_362A:
        xor    eax, eax
        cmp    dword ptr [edi + 20h], 0
        je     SHORT loc_3_368A
        push   edi
        mov    ecx, edi
        mov    eax, 1bh
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0FEh, 005h, 0C5h, 001h, 000h, 000h
        call   dword ptr [edi + 20h]
        db  0FEh, 00Dh, 0C5h, 001h, 000h, 000h
        pop    edi
        test   byte ptr [edi], 4
        jne    NEAR PTR loc_3_3702
        jmp    NEAR PTR loc_3_36FB
loc_3_365E:
        cmp    eax, 2
        je     NEAR PTR loc_3_3702
        cmp    eax, 4
        jne    SHORT loc_3_368A
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_3_3683
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esi + 1ch]
        jecxz  loc_3_3683
        push   esi
        call   ecx
        pop    esi
        and    byte ptr [esi], 0fbh
loc_3_3683:
        mov    eax, 0
        jmp    SHORT loc_3_3704
loc_3_368A:
        cmp    eax, 3
        je     SHORT loc_3_3702
        cmp    eax, 8
        jne    SHORT loc_3_369B
        cmp    dword ptr [esp + 14h], 7
        je     SHORT loc_3_36A5
loc_3_369B:
        cmp    eax, 0bh
        mov    eax, 1
        jne    SHORT loc_3_3704
loc_3_36A5:
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esi + 1ch]
        jecxz  loc_3_36B2
        push   esi
        call   ecx
        pop    esi
loc_3_36B2:
        and    byte ptr [esi], 0fbh
        cmp    dword ptr [esi + 20h], 0
        je     SHORT loc_3_36FB
        mov    ecx, esi
        mov    eax, 1bh
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   esi
        db  0FEh, 005h, 0C5h, 001h, 000h, 000h
        call   dword ptr [esi + 20h]
        db  0FEh, 00Dh, 0C5h, 001h, 000h, 000h
        pop    esi
        test   word ptr [esi], 4
        jne    SHORT loc_3_3702
        db  0C6h, 005h, 094h, 006h, 000h, 000h, 000h
        db  0C6h, 005h, 0A8h, 000h, 000h, 000h, 0FFh
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        call   sub_3_3378
        jmp    SHORT loc_3_3702
loc_3_36FB:
        mov    eax, 24h
        jmp    SHORT loc_3_3704
loc_3_3702:
        xor    eax, eax
loc_3_3704:
        pop    ebx
        pop    edi
        pop    esi
        ret
_PTEXT ends
        end
