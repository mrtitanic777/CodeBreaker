; v86mmgr_pcode_09.asm -- V86MMGR object 9 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_9_0000:
        test   byte ptr [ebx + 1], 1
        jne    SHORT loc_9_0034
        push   eax
        movzx  eax, byte ptr [edx]
        push   eax
        cmp    eax, 24h
        jb     SHORT loc_9_0013
        add    eax, 24h
loc_9_0013:
        push   dword ptr [eax + ebp]
        inc    edx
        db  0CDh, 020h, 006h, 000h, 006h, 000h
        pop    edx
        xchg   word ptr [eax + ebp], dx
        movzx  edx, dx
        shl    edx, 4
        db  0E8h, 041h, 000h, 0F7h, 0FFh
        pop    edx
        mov    word ptr [edx + ebp], ax
        pop    eax
        ret
loc_9_0034:
        ret
V86MMGR_Set_Xlat_Buff_State:
        push   edi
        db  08Bh, 03Dh, 0FCh, 011h, 000h, 000h
        add    edi, ebx
        mov    dword ptr [edi + 14h], ecx
        mov    dword ptr [edi + 18h], edx
        mov    word ptr [edi + 1eh], ax
        push   eax
        movzx  eax, ax
        shl    eax, 4
        add    eax, dword ptr [ebx + 4]
        mov    dword ptr [edi + 20h], eax
        pop    eax
        pop    edi
        ret
V86MMGR_Get_VM_Flat_Sel:
        db  0A1h, 0FCh, 011h, 000h, 000h
        movzx  eax, word ptr [eax + ebx + 24h]
        test   eax, eax
        je     SHORT loc_9_0067
        ret
loc_9_0067:
        push   ecx
        push   edx
        push   0
        push   0
        push   92h
        push   0fffffh
        push   dword ptr [ebx + 4]
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        mov    word ptr [edx + ebx + 24h], ax
        pop    edx
        pop    ecx
        ret
        db  000h, 000h, 000h
sub_9_00A4:
        enter  44h, 0
        push   esi
        push   ebx
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        db  0A3h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esi + ebx + 5ch]
        mov    edi, edx
        or     edx, edx
        jne    NEAR PTR loc_9_09A2
        db  083h, 03Dh, 008h, 000h, 000h, 000h, 000h
        stc
        je     NEAR PTR loc_9_09A2
        test   byte ptr [esi + ebx + 2ch], 2
        stc
        jne    NEAR PTR loc_9_09A2
        push   101h
        push   692h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        stc
        je     NEAR PTR loc_9_09A2
        mov    dword ptr [esi + ebx + 5ch], eax
        lea    edi, [eax + 240h]
        mov    edx, eax
        xor    eax, eax
        dec    eax
        mov    ecx, 10h
        cld
        rep stosd
        mov    edi, edx
        or     byte ptr [edi + 68eh], 4
        push   101h
        push   36h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_9_09DD
        mov    dword ptr [edi + 280h], eax
        mov    edx, dword ptr [esi + ebx]
        mov    dword ptr [eax], edx
        push   eax
        push   0
        push   edx
        db  0CDh, 020h, 058h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ecx + 28h], eax
        mov    byte ptr [ecx + 2ch], 40h
        mov    dword ptr [edi + 67ch], eax
        db  001h, 005h, 00Ch, 000h, 000h, 000h
        lea    esi, [ebp - 44h]
        push   0
        push   esi
        push   ebx
        db  0CDh, 020h, 074h, 000h, 001h, 000h
        add    esp, 0ch
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        mov    dword ptr [ebp - 1ch], eax
        mov    edx, eax
        add    eax, 3
        and    eax, 0fffffffch
        mov    ecx, eax
        sub    ecx, edx
        mov    dword ptr [ebp - 4], ecx
        or     ecx, ecx
        je     NEAR PTR loc_9_02D7
        push   eax
        push   ecx
        xor    eax, eax
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        test   byte ptr [edx + ebx + 2eh], 1
        je     SHORT loc_9_01A4
        or     eax, 80h
loc_9_01A4:
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_9_09DC
        push   edi
        push   ecx
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        mov    dword ptr [esi + ebx + 4], eax
        push   edx
        mov    edx, dword ptr [edi + 280h]
        pop    edi
        mov    esi, dword ptr [edx + 4]
        shl    ecx, 0ah
        cld
        rep movsd
        pop    ecx
        pop    edi
        call   sub_9_0EA5
        jb     NEAR PTR loc_9_09A6
        push   ecx
        push   0
        push   0
        push   ecx
        push   dword ptr [ebp - 1ch]
        push   ebx
        push   eax
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_9_09A6
        mov    edx, dword ptr [edi + 280h]
        sub    dword ptr [edx + 28h], ecx
        jae    SHORT loc_9_021C
        mov    dword ptr [edx + 28h], 0
loc_9_021C:
        sub    dword ptr [edi + 67ch], ecx
        jae    SHORT loc_9_022E
        mov    dword ptr [edi + 67ch], 0
loc_9_022E:
        db  029h, 00Dh, 00Ch, 000h, 000h, 000h
        jae    SHORT loc_9_0240
        db  0C7h, 005h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_9_0240:
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, ecx
        mov    dword ptr [ebp - 20h], eax
        push   edi
        mov    esi, dword ptr [edx + 4]
        mov    edi, esi
        shl    ecx, 0ch
        add    esi, ecx
        mov    ecx, dword ptr [edx + 28h]
        shl    ecx, 0ah
        cld
        rep movsd
        pop    edi
        cmp    dword ptr [edx + 28h], 0
        jne    SHORT loc_9_028D
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     SHORT loc_9_0276
        push   edx
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
loc_9_0276:
        xor    eax, eax
        db  08Bh, 00Dh, 0FCh, 011h, 000h, 000h
        mov    dword ptr [ecx + ebx], eax
        mov    dword ptr [edx], eax
        dec    eax
        mov    dword ptr [edx + 4], eax
        or     byte ptr [edx + 2ch], 4
        jmp    SHORT loc_9_02D6
loc_9_028D:
        push   edx
        push   0
        push   dword ptr [edx + 28h]
        push   dword ptr [edx]
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        mov    esi, edx
        pop    edx
        or     eax, eax
        je     NEAR PTR loc_9_09DC
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], esi
        call   sub_9_0EA5
        jb     NEAR PTR loc_9_09DC
        push   0
        push   0
        push   dword ptr [edx + 28h]
        push   dword ptr [ebp - 20h]
        push   ebx
        push   eax
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        je     NEAR PTR loc_9_09DC
loc_9_02D6:
        pop    eax
loc_9_02D7:
        mov    edx, dword ptr [edi + 280h]
        lea    edx, [edi + 4]
        mov    ecx, eax
        shr    ecx, 2
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        lea    esi, [esi + ebx + 64h]
loc_9_02EF:
        mov    byte ptr [edx + 2], 1
        add    edx, 8
        add    esi, 8
        loop   loc_9_02EF
        mov    ecx, dword ptr [edi + 280h]
        mov    ecx, dword ptr [ecx + 28h]
        xor    eax, eax
loc_9_0306:
        mov    word ptr [edx + 4], ax
        mov    byte ptr [edx + 2], 40h
        cmp    ecx, 4
        jb     SHORT loc_9_0348
        test   byte ptr [esi + 1], 0eh
        jne    SHORT loc_9_0342
        test   byte ptr [esi + 3], 0eh
        jne    SHORT loc_9_0342
        test   byte ptr [esi + 5], 0eh
        jne    SHORT loc_9_0342
        test   byte ptr [esi + 7], 0eh
        jne    SHORT loc_9_0342
        mov    byte ptr [edx + 1], 4
loc_9_032F:
        add    esi, 8
        add    edx, 8
        add    eax, 4
        sub    ecx, 4
        ja     SHORT loc_9_0306
        jmp    NEAR PTR loc_9_05A8
loc_9_0342:
        mov    byte ptr [edx + 2], 1
        jmp    SHORT loc_9_032F
loc_9_0348:
        mov    byte ptr [edx + 1], cl
        push   edx
        push   ecx
        lea    ecx, [edi + 4]
        sub    edx, ecx
        shr    edx, 1
        pop    eax
        add    edx, eax
        mov    dword ptr [ebp - 8], edx
        sub    eax, 4
        neg    eax
        mov    dword ptr [ebp - 0ch], eax
        mov    ecx, eax
        mov    eax, edx
        shr    eax, 5
        and    edx, 1fh
        lea    esi, [ebp - 44h]
loc_9_036F:
        bt     dword ptr [esi + eax*4], edx
        jb     SHORT loc_9_0389
loc_9_0375:
        inc    edx
        cmp    edx, 20h
        jb     SHORT loc_9_037E
        xor    edx, edx
        inc    eax
loc_9_037E:
        loop   loc_9_036F
        pop    edx
loc_9_0381:
        add    edx, 8
        jmp    NEAR PTR loc_9_05AF
loc_9_0389:
        push   edx
        push   eax
        shl    eax, 5
        add    eax, edx
        lea    eax, [ebx + eax*2 + 64h]
        db  003h, 005h, 0FCh, 011h, 000h, 000h
        test   byte ptr [eax + 1], 0eh
        je     SHORT loc_9_03A4
        xor    eax, eax
        jmp    SHORT loc_9_03A8
loc_9_03A4:
        test   byte ptr [eax + 1], 10h
loc_9_03A8:
        pop    eax
        pop    edx
        jne    SHORT loc_9_0375
        pop    edx
        or     byte ptr [edx + 2], 1
        mov    dword ptr [ebp - 0ch], 0
        push   edx
        movzx  ecx, byte ptr [edx + 1]
        xor    eax, eax
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        test   byte ptr [edx + ebx + 2eh], 1
        je     SHORT loc_9_03D1
        or     eax, 80h
loc_9_03D1:
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        or     eax, eax
        je     NEAR PTR loc_9_09DC
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        mov    dword ptr [esi + ebx + 8], eax
        mov    esi, edx
        pop    edx
        push   edx
        push   eax
        push   edi
        push   esi
        movzx  ecx, byte ptr [edx + 1]
        mov    esi, dword ptr [edi + 280h]
        sub    dword ptr [esi + 28h], ecx
        jae    SHORT loc_9_0416
        mov    dword ptr [esi + 28h], 0
loc_9_0416:
        sub    dword ptr [edi + 67ch], ecx
        jae    SHORT loc_9_0428
        mov    dword ptr [edi + 67ch], 0
loc_9_0428:
        db  029h, 00Dh, 00Ch, 000h, 000h, 000h
        jae    SHORT loc_9_043A
        db  0C7h, 005h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_9_043A:
        pop    edi
        mov    eax, dword ptr [esi + 28h]
        shl    eax, 0ch
        mov    esi, dword ptr [esi + 4]
        add    esi, eax
        shl    ecx, 0ah
        cld
        rep movsd
        pop    edi
        pop    eax
        pop    edx
        push   edx
        lea    ecx, [edi + 4]
        sub    edx, ecx
        shr    edx, 1
        mov    dword ptr [ebp - 8], edx
        pop    edx
        push   edx
        movzx  ecx, byte ptr [edx + 1]
        call   sub_9_0EA5
        jb     NEAR PTR loc_9_09C1
        push   0
        push   0
        push   ecx
        push   dword ptr [ebp - 8]
        push   ebx
        push   eax
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        je     NEAR PTR loc_9_09C1
        mov    esi, dword ptr [edi + 280h]
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_9_04BA
        mov    edx, dword ptr [esi]
        or     edx, edx
        je     SHORT loc_9_04A3
        push   0
        push   edx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_9_04A3:
        xor    eax, eax
        db  08Bh, 00Dh, 0FCh, 011h, 000h, 000h
        mov    dword ptr [ecx + ebx], eax
        mov    dword ptr [esi], eax
        dec    eax
        mov    dword ptr [esi + 4], eax
        or     byte ptr [esi + 2ch], 4
        jmp    SHORT loc_9_04D7
loc_9_04BA:
        push   0
        push   dword ptr [esi + 28h]
        push   dword ptr [esi]
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_9_09DC
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], edx
loc_9_04D7:
        pop    edx
        lea    esi, [ebp - 44h]
        jmp    NEAR PTR loc_9_0381
loc_9_04E0:
        push   edx
        push   eax
        shl    eax, 5
        add    eax, edx
        lea    eax, [ebx + eax*2 + 64h]
        db  003h, 005h, 0FCh, 011h, 000h, 000h
        test   byte ptr [eax + 1], 0eh
        je     SHORT loc_9_04F9
        jmp    SHORT loc_9_052C
loc_9_04F9:
        test   byte ptr [eax + 1], 10h
        jne    SHORT loc_9_052E
        or     byte ptr [eax + 1], 8
        pop    edx
        pop    ecx
        push   ecx
        push   edx
        inc    ecx
        add    eax, 2
        and    ecx, 3
        je     SHORT loc_9_052E
        sub    ecx, 4
        neg    ecx
        push   ebx
        mov    ebx, dword ptr [esp + 8]
        inc    ebx
loc_9_051B:
        test   byte ptr [eax + 1], 1eh
        jne    SHORT loc_9_0525
        or     byte ptr [eax + 1], 8
loc_9_0525:
        inc    ebx
        add    eax, 2
        loop   loc_9_051B
        pop    ebx
loc_9_052C:
        xor    eax, eax
loc_9_052E:
        pop    eax
        pop    edx
        jne    NEAR PTR loc_9_05D6
        pop    edx
        or     byte ptr [edx + 2], 1
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_9_062D
        push   edx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [ebp - 8]
        mov    edx, edi
        lea    edi, [ebx + edi*2 + 64h]
        db  003h, 03Dh, 0FCh, 011h, 000h, 000h
        xor    ecx, ecx
loc_9_055C:
        bts    word ptr [edi], 0ch
        jae    SHORT loc_9_0567
        jecxz  loc_9_0580
        jmp    SHORT loc_9_056E
loc_9_0567:
        inc    ecx
        add    edi, 2
        dec    esi
        jne    SHORT loc_9_055C
loc_9_056E:
        push   0
        push   ebx
        push   ecx
        push   edx
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        or     esi, esi
        je     SHORT loc_9_0599
loc_9_0580:
        add    edi, 2
        dec    esi
        je     SHORT loc_9_0599
        xor    ecx, ecx
        mov    edx, edi
        db  0A1h, 0FCh, 011h, 000h, 000h
        lea    eax, [eax + ebx + 64h]
        sub    edx, eax
        shr    edx, 1
        jmp    SHORT loc_9_055C
loc_9_0599:
        pop    edi
        pop    esi
        pop    edx
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_9_062D
loc_9_05A8:
        mov    dword ptr [ebp - 0ch], 0
loc_9_05AF:
        mov    eax, edx
        lea    ecx, [edi + 4]
        sub    eax, ecx
        shr    eax, 1
loc_9_05B8:
        mov    dword ptr [ebp - 10h], eax
        push   edx
        mov    edx, eax
        shr    eax, 5
        and    edx, 1fh
        mov    ecx, 4
        lea    esi, [ebp - 44h]
loc_9_05CC:
        bt     dword ptr [esi + eax*4], edx
        jb     NEAR PTR loc_9_04E0
loc_9_05D6:
        inc    edx
        cmp    edx, 20h
        jb     SHORT loc_9_05DF
        xor    edx, edx
        inc    eax
loc_9_05DF:
        loop   loc_9_05CC
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_9_05ED
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 8], eax
loc_9_05ED:
        add    dword ptr [ebp - 0ch], 4
        pop    edx
        mov    byte ptr [edx + 2], 20h
        mov    byte ptr [edx], 0ffh
        mov    byte ptr [edx + 1], 4
        mov    word ptr [edx + 4], 0
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 10h], eax
        jb     SHORT loc_9_062D
        push   edx
        call   sub_9_0EA5
        push   10h
        push   0
        push   4
        push   dword ptr [ebp - 10h]
        push   ebx
        db  0FFh, 035h, 000h, 012h, 000h, 000h
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        pop    edx
loc_9_062D:
        add    edx, 8
        mov    eax, edx
        lea    ecx, [edi + 4]
        sub    eax, ecx
        shr    eax, 1
        cmp    eax, 100h
        jb     NEAR PTR loc_9_05B8
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_9_06A6
        push   esi
        push   edi
        mov    esi, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [ebp - 8]
        mov    edx, edi
        lea    edi, [ebx + edi*2 + 64h]
        db  003h, 03Dh, 0FCh, 011h, 000h, 000h
        xor    ecx, ecx
loc_9_0660:
        bts    word ptr [edi], 0ch
        jae    SHORT loc_9_066B
        jecxz  loc_9_0684
        jmp    SHORT loc_9_0672
loc_9_066B:
        inc    ecx
        add    edi, 2
        dec    esi
        jne    SHORT loc_9_0660
loc_9_0672:
        push   0
        push   ebx
        push   ecx
        push   edx
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        or     esi, esi
        je     SHORT loc_9_069D
loc_9_0684:
        add    edi, 2
        dec    esi
        je     SHORT loc_9_069D
        xor    ecx, ecx
        mov    edx, edi
        db  0A1h, 0FCh, 011h, 000h, 000h
        lea    eax, [eax + ebx + 64h]
        sub    edx, eax
        shr    edx, 1
        jmp    SHORT loc_9_0660
loc_9_069D:
        pop    edi
        pop    esi
        mov    dword ptr [ebp - 0ch], 0
loc_9_06A6:
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 0DFh, 000h, 000h, 000h
        jae    SHORT loc_9_0717
        mov    edx, 0c0h
        db  03Bh, 015h, 000h, 000h, 000h, 000h
        jae    SHORT loc_9_06E1
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    edx, 3
        shr    edx, 2
        shl    edx, 2
        mov    ecx, 0e0h
        sub    ecx, edx
        jbe    SHORT loc_9_0717
        cmp    ecx, 10h
        jb     SHORT loc_9_0717
        shr    ecx, 2
        jmp    SHORT loc_9_06E6
loc_9_06E1:
        mov    ecx, 8
loc_9_06E6:
        lea    eax, [edi + 4]
        test   edx, 3
        stc
        jne    SHORT loc_9_06F6
        shl    edx, 1
        add    edx, eax
loc_9_06F6:
        xor    eax, eax
loc_9_06F8:
        test   byte ptr [edx + 2], 1
        jne    SHORT loc_9_0710
        or     eax, eax
        jne    SHORT loc_9_0704
        mov    esi, edx
loc_9_0704:
        inc    eax
        cmp    eax, 4
        jae    NEAR PTR loc_9_0871
        jmp    SHORT loc_9_0712
loc_9_0710:
        xor    eax, eax
loc_9_0712:
        add    edx, 8
        loop   loc_9_06F8
loc_9_0717:
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h
        jae    SHORT loc_9_0788
        mov    edx, 0e0h
        db  03Bh, 015h, 000h, 000h, 000h, 000h
        jae    SHORT loc_9_0752
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    edx, 3
        shr    edx, 2
        shl    edx, 2
        mov    ecx, 100h
        sub    ecx, edx
        jbe    SHORT loc_9_0788
        cmp    ecx, 10h
        jb     SHORT loc_9_0788
        shr    ecx, 2
        jmp    SHORT loc_9_0757
loc_9_0752:
        mov    ecx, 8
loc_9_0757:
        lea    eax, [edi + 4]
        test   edx, 3
        stc
        jne    SHORT loc_9_0767
        shl    edx, 1
        add    edx, eax
loc_9_0767:
        xor    eax, eax
loc_9_0769:
        test   byte ptr [edx + 2], 1
        jne    SHORT loc_9_0781
        or     eax, eax
        jne    SHORT loc_9_0775
        mov    esi, edx
loc_9_0775:
        inc    eax
        cmp    eax, 4
        jae    NEAR PTR loc_9_0871
        jmp    SHORT loc_9_0783
loc_9_0781:
        xor    eax, eax
loc_9_0783:
        add    edx, 8
        loop   loc_9_0769
loc_9_0788:
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 0BFh, 000h, 000h, 000h
        jae    SHORT loc_9_07F9
        mov    edx, 0a0h
        db  03Bh, 015h, 000h, 000h, 000h, 000h
        jae    SHORT loc_9_07C3
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    edx, 3
        shr    edx, 2
        shl    edx, 2
        mov    ecx, 0c0h
        sub    ecx, edx
        jbe    SHORT loc_9_07F9
        cmp    ecx, 10h
        jb     SHORT loc_9_07F9
        shr    ecx, 2
        jmp    SHORT loc_9_07C8
loc_9_07C3:
        mov    ecx, 8
loc_9_07C8:
        lea    eax, [edi + 4]
        test   edx, 3
        stc
        jne    SHORT loc_9_07D8
        shl    edx, 1
        add    edx, eax
loc_9_07D8:
        xor    eax, eax
loc_9_07DA:
        test   byte ptr [edx + 2], 1
        jne    SHORT loc_9_07F2
        or     eax, eax
        jne    SHORT loc_9_07E6
        mov    esi, edx
loc_9_07E6:
        inc    eax
        cmp    eax, 4
        jae    NEAR PTR loc_9_0871
        jmp    SHORT loc_9_07F4
loc_9_07F2:
        xor    eax, eax
loc_9_07F4:
        add    edx, 8
        loop   loc_9_07DA
loc_9_07F9:
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h
        jae    SHORT loc_9_0866
        mov    edx, 0a0h
        db  03Bh, 015h, 000h, 000h, 000h, 000h
        jae    SHORT loc_9_0834
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    edx, 3
        shr    edx, 2
        shl    edx, 2
        mov    ecx, 100h
        sub    ecx, edx
        jbe    SHORT loc_9_0866
        cmp    ecx, 10h
        jb     SHORT loc_9_0866
        shr    ecx, 2
        jmp    SHORT loc_9_0839
loc_9_0834:
        mov    ecx, 18h
loc_9_0839:
        lea    eax, [edi + 4]
        test   edx, 3
        stc
        jne    SHORT loc_9_0849
        shl    edx, 1
        add    edx, eax
loc_9_0849:
        xor    eax, eax
loc_9_084B:
        test   byte ptr [edx + 2], 1
        jne    SHORT loc_9_085F
        or     eax, eax
        jne    SHORT loc_9_0857
        mov    esi, edx
loc_9_0857:
        inc    eax
        cmp    eax, 4
        jae    SHORT loc_9_0871
        jmp    SHORT loc_9_0861
loc_9_085F:
        xor    eax, eax
loc_9_0861:
        add    edx, 8
        loop   loc_9_084B
loc_9_0866:
        mov    word ptr [edi + 68ah], 0
        jmp    SHORT loc_9_0892
loc_9_0871:
        mov    edx, esi
        lea    ecx, [edi + 4]
        sub    edx, ecx
        shl    edx, 7
        mov    word ptr [edi + 68ah], dx
        mov    edx, esi
        mov    ecx, 4
loc_9_0889:
        or     byte ptr [edx + 2], 10h
        add    edx, 8
        loop   loc_9_0889
loc_9_0892:
        movzx  edx, word ptr [edi + 68ah]
        or     edx, edx
        je     SHORT loc_9_08B3
        lea    ecx, [edi + 4]
        test   edx, 3ffh
        stc
        jne    SHORT loc_9_08AE
        shr    edx, 7
        add    edx, ecx
loc_9_08AE:
        jmp    NEAR PTR loc_9_094A
loc_9_08B3:
        db  083h, 03Dh, 010h, 000h, 000h, 000h, 000h
        je     SHORT loc_9_090F
        db  08Bh, 015h, 010h, 000h, 000h, 000h
        shr    edx, 8
        lea    edx, [ebx + edx*2 + 64h]
        db  003h, 015h, 0FCh, 011h, 000h, 000h
        mov    ecx, 10h
loc_9_08D4:
        test   byte ptr [edx + 1], 0eh
        jne    SHORT loc_9_090F
        add    edx, 2
        loop   loc_9_08D4
        lea    eax, [edi + 4]
        db  08Bh, 015h, 010h, 000h, 000h, 000h
        mov    word ptr [edi + 68ah], dx
        test   edx, 3ffh
        stc
        jne    SHORT loc_9_08FD
        shr    edx, 7
        add    edx, eax
loc_9_08FD:
        push   edx
        mov    ecx, 4
loc_9_0903:
        mov    byte ptr [edx + 2], 10h
        add    edx, 8
        loop   loc_9_0903
        pop    edx
        jmp    SHORT loc_9_094A
loc_9_090F:
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        mov    edx, eax
        inc    edx
        add    edx, 3
        and    edx, 0fffffffch
        mov    ecx, 100h
        sub    ecx, edx
        jbe    SHORT loc_9_0947
        shr    ecx, 2
        jecxz  loc_9_0947
        lea    eax, [edi + 4]
        test   edx, 3
        stc
        jne    SHORT loc_9_093C
        shl    edx, 1
        add    edx, eax
loc_9_093C:
        test   byte ptr [edx + 2], 1
        je     SHORT loc_9_094A
        add    edx, 8
        loop   loc_9_093C
loc_9_0947:
        lea    edx, [edi + 4]
loc_9_094A:
        mov    dword ptr [ebp - 14h], edx
        lea    eax, [edi + 204h]
        mov    dword ptr [ebp - 18h], eax
        lea    esi, [edi + 240h]
        lea    ecx, [edi + 4]
loc_9_095F:
        test   byte ptr [edx + 2], 1
        jne    SHORT loc_9_096F
        mov    eax, edx
        sub    eax, ecx
        shr    eax, 3
        mov    byte ptr [esi], al
        inc    esi
loc_9_096F:
        add    edx, 8
        cmp    edx, dword ptr [ebp - 18h]
        jb     SHORT loc_9_097A
        lea    edx, [edi + 4]
loc_9_097A:
        cmp    edx, dword ptr [ebp - 14h]
        jne    SHORT loc_9_095F
        lea    ecx, [edi + 240h]
        sub    esi, ecx
        mov    ecx, esi
        mov    byte ptr [edi + 68fh], cl
        mov    edx, ecx
        shl    ecx, 1
        add    ecx, edx
        add    ecx, 4
        mov    word ptr [edi + 688h], cx
        clc
        mov    edx, edi
loc_9_09A2:
        pop    ebx
        pop    esi
        leave
        ret
loc_9_09A6:
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        xor    eax, eax
        xchg   dword ptr [esi + ebx + 4], eax
        or     eax, eax
        je     SHORT loc_9_09C1
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_9_09C1:
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        xor    eax, eax
        xchg   dword ptr [esi + ebx + 8], eax
        or     eax, eax
        je     SHORT loc_9_09DC
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_9_09DC:
        pop    eax
loc_9_09DD:
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        xor    edi, edi
        xchg   dword ptr [esi + ebx + 5ch], edi
        or     edi, edi
        stc
        je     SHORT loc_9_09A2
        xor    eax, eax
        xchg   dword ptr [edi + 280h], eax
        or     eax, eax
        je     SHORT loc_9_0A05
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_9_0A05:
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        stc
        jmp    SHORT loc_9_09A2
sub_9_0A13:
        pushad
        bts    word ptr [edi + 68eh], 5
        jb     NEAR PTR loc_9_0AC6
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        db  0A3h, 000h, 000h, 000h, 000h
        lea    esi, [edi + 4]
        mov    ecx, 40h
loc_9_0A37:
        test   byte ptr [esi + 2], 3
        jne    SHORT loc_9_0AB3
        mov    al, byte ptr [edx + 2dh]
        cmp    al, byte ptr [esi]
        jne    SHORT loc_9_0AB3
        push   ecx
        push   edx
        push   esi
        test   byte ptr [edx + 2ch], 4
        jne    SHORT loc_9_0A9B
        movzx  eax, word ptr [esi + 4]
        cmp    eax, dword ptr [edx + 28h]
        jae    SHORT loc_9_0A9B
        mov    ecx, dword ptr [edx]
        test   byte ptr [edx + 2ch], 80h
        jne    SHORT loc_9_0AD4
        movzx  edx, byte ptr [esi + 1]
        push   ecx
        lea    ecx, [edi + 4]
        sub    esi, ecx
        shr    esi, 1
        pop    ecx
        test   byte ptr [edi + 68eh], 4
        jne    SHORT loc_9_0A7D
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        cmc
        jae    SHORT loc_9_0AAE
loc_9_0A7D:
        call   sub_9_0EA5
        jb     SHORT loc_9_0A98
        push   0
        push   eax
        push   edx
        push   esi
        push   ebx
        push   ecx
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
loc_9_0A94:
        or     eax, eax
        jne    SHORT loc_9_0AAE
loc_9_0A98:
        stc
        jmp    SHORT loc_9_0AAE
loc_9_0A9B:
        mov    eax, esi
        xor    esi, esi
        mov    ecx, 1
        db  0BAh, 0F2h, 00Eh, 000h, 000h
        call   sub_9_0CA2
loc_9_0AAE:
        pop    esi
        pop    edx
        pop    ecx
        jb     SHORT loc_9_0AC9
loc_9_0AB3:
        add    esi, 8
        dec    ecx
        jne    NEAR PTR loc_9_0A37
        btr    word ptr [edi + 68eh], 5
loc_9_0AC6:
        clc
loc_9_0AC7:
        popad
        ret
loc_9_0AC9:
        btr    word ptr [edi + 68eh], 5
        jmp    SHORT loc_9_0AC7
loc_9_0AD4:
        movzx  edx, byte ptr [esi + 1]
        push   ecx
        lea    ecx, [edi + 4]
        sub    esi, ecx
        shr    esi, 1
        pop    ecx
        test   byte ptr [edi + 68eh], 4
        jne    SHORT loc_9_0AF3
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        cmc
        jae    SHORT loc_9_0AAE
loc_9_0AF3:
        push   0
        push   eax
        push   edx
        push   esi
        push   ebx
        push   ecx
        call   sub_9_10D3
        add    esp, 18h
        jmp    SHORT loc_9_0A94
sub_9_0B04:
        pushad
        mov    eax, 57333800h
        cld
        stosd
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        push   eax
        push   0
        db  003h, 01Dh, 0FCh, 011h, 000h, 000h
        mov    ebx, dword ptr [ebx + 5ch]
        or     ebx, ebx
        je     SHORT loc_9_0B2D
        movzx  ebx, byte ptr [ebx + 68eh]
        mov    dword ptr [esp], ebx
loc_9_0B2D:
        mov    ecx, 40h
        xor    ebx, ebx
loc_9_0B34:
        test   byte ptr [esi + 2], 1
        jne    SHORT loc_9_0B80
        mov    al, byte ptr [esi]
        mov    byte ptr [edi], al
        test   byte ptr [esp], 4
        jne    SHORT loc_9_0B4A
        cmp    ebx, dword ptr [esp + 4]
        jb     SHORT loc_9_0B75
loc_9_0B4A:
        cmp    byte ptr [esi + 1], 4
        je     SHORT loc_9_0B75
        test   byte ptr [esi + 2], 2
        jne    SHORT loc_9_0B75
        mov    eax, esi
        mov    edx, dword ptr [esp + 0ch]
        sub    eax, edx
        shr    eax, 3
        and    eax, 3fh
        mov    ah, byte ptr [esi + 1]
        shl    ah, 6
        and    ah, 0c0h
        or     al, ah
        mov    edx, dword ptr [esp + 8]
        mov    byte ptr [edx], al
loc_9_0B75:
        movzx  eax, word ptr [esi + 4]
        mov    word ptr [edi + 1], ax
        add    edi, 3
loc_9_0B80:
        add    esi, 8
        lea    ebx, [ebx + 4]
        dec    ecx
        jne    SHORT loc_9_0B34
        add    esp, 8
        popad
        ret
sub_9_0B8E:
        enter  8, 0
        pushad
        xor    eax, eax
        mov    dword ptr [ebp - 4], eax
        dec    eax
        mov    dword ptr [ebp - 8], eax
        cld
        lodsd
        movzx  ecx, al
        jecxz  loc_9_0BD0
        push   eax
        mov    eax, ecx
        and    al, 3fh
        and    cl, 0c0h
        shr    cl, 6
        mov    dword ptr [ebp - 4], ecx
        or     ecx, ecx
        je     NEAR PTR loc_9_0C9E
        lea    eax, [edx + eax*8]
        mov    dword ptr [ebp - 8], eax
        sub    eax, edx
        lea    edx, [edi + eax + 4]
        test   byte ptr [edx + 2], 1
        jne    NEAR PTR loc_9_0C9E
        pop    eax
loc_9_0BD0:
        and    eax, 0ffffff00h
        cmp    eax, 57333800h
        jne    NEAR PTR loc_9_0C9F
        push   esi
        push   edx
        lea    esi, [edi + 4]
        mov    ebx, esi
        mov    ecx, 40h
loc_9_0BEC:
        mov    al, byte ptr [esi + 2]
        and    al, 13h
        mov    byte ptr [edx + 2], al
        add    edx, 8
        add    esi, 8
        loop   loc_9_0BEC
        pop    edx
        pop    esi
        movzx  eax, byte ptr [edi + 68fh]
        or     eax, eax
        je     NEAR PTR loc_9_0C9F
        mov    ecx, 40h
        mov    edi, edx
        add    edi, 200h
loc_9_0C1A:
        test   byte ptr [edx + 2], 1
        je     SHORT loc_9_0C2A
        add    edx, 8
        add    ebx, 8
        loop   loc_9_0C1A
        jmp    SHORT loc_9_0C9F
loc_9_0C2A:
        mov    ecx, eax
loc_9_0C2C:
        cmp    byte ptr [esi], 0ffh
        je     SHORT loc_9_0C36
        cmp    byte ptr [esi], 0ffh
        jae    SHORT loc_9_0C9F
loc_9_0C36:
        mov    al, byte ptr [esi]
        test   byte ptr [edx + 2], 2
        je     SHORT loc_9_0C42
        cmp    byte ptr [ebx], al
        jne    SHORT loc_9_0C9F
loc_9_0C42:
        mov    byte ptr [edx], al
        mov    al, 4
        cmp    edx, dword ptr [ebp - 8]
        jne    SHORT loc_9_0C4E
        mov    eax, dword ptr [ebp - 4]
loc_9_0C4E:
        test   byte ptr [edx + 2], 2
        je     SHORT loc_9_0C57
        mov    al, byte ptr [ebx + 1]
loc_9_0C57:
        mov    byte ptr [edx + 1], al
        movzx  eax, word ptr [esi + 1]
        test   byte ptr [edx + 2], 2
        je     SHORT loc_9_0C6A
        cmp    word ptr [ebx + 4], ax
        jne    SHORT loc_9_0C9F
loc_9_0C6A:
        mov    word ptr [edx + 4], ax
        cmp    byte ptr [edx], 0
        jne    SHORT loc_9_0C77
        or     byte ptr [edx + 2], 40h
loc_9_0C77:
        cmp    byte ptr [edx], 0ffh
        jne    SHORT loc_9_0C80
        or     byte ptr [edx + 2], 20h
loc_9_0C80:
        add    esi, 3
loc_9_0C83:
        add    edx, 8
        add    ebx, 8
        cmp    ecx, 1
        je     SHORT loc_9_0C98
        cmp    edx, edi
        jae    SHORT loc_9_0C9F
        test   byte ptr [edx + 2], 1
        jne    SHORT loc_9_0C83
loc_9_0C98:
        loop   loc_9_0C2C
        clc
loc_9_0C9B:
        popad
        leave
        ret
loc_9_0C9E:
        pop    eax
loc_9_0C9F:
        stc
        jmp    SHORT loc_9_0C9B
sub_9_0CA2:
        push   2
        jmp    SHORT loc_9_0CAC
sub_9_0CA6:
        push   1
        jmp    SHORT loc_9_0CAC
sub_9_0CAA:
        push   0
loc_9_0CAC:
        enter  0ch, 0
        pushad
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        db  0A3h, 000h, 000h, 000h, 000h
        shl    ecx, 2
        mov    dword ptr [ebp - 4], ecx
        shl    esi, 2
        mov    dword ptr [ebp - 8], esi
        test   byte ptr [edx + 2ch], 20h
        jne    SHORT loc_9_0CF8
        cmp    esi, dword ptr [edx + 28h]
        jae    NEAR PTR loc_9_0E9F
        add    esi, ecx
        cmp    esi, dword ptr [edx + 28h]
        jbe    SHORT loc_9_0CF8
        test   byte ptr [edx + 2ch], 40h
        je     NEAR PTR loc_9_0E9F
        sub    esi, dword ptr [edx + 28h]
        cmp    esi, 4
        jae    NEAR PTR loc_9_0E9F
        sub    dword ptr [ebp - 4], esi
loc_9_0CF8:
        cmp    dword ptr [ebp + 4], 0
        je     SHORT loc_9_0D44
        cmp    dword ptr [ebp + 4], 1
        je     SHORT loc_9_0D09
        mov    eax, dword ptr [ebp - 10h]
        jmp    SHORT loc_9_0D22
loc_9_0D09:
        mov    eax, dword ptr [ebp - 10h]
        lea    esi, [edi + 4]
        test   eax, 3ffh
        stc
        jne    SHORT loc_9_0D1C
        shr    eax, 7
        add    eax, esi
loc_9_0D1C:
        jb     NEAR PTR loc_9_0E99
loc_9_0D22:
        lea    esi, [edi + 204h]
        mov    ecx, dword ptr [ebp - 14h]
loc_9_0D2B:
        cmp    eax, esi
        jae    NEAR PTR loc_9_0E99
        test   byte ptr [eax + 2], 1
        jne    NEAR PTR loc_9_0E99
        add    eax, 8
        loop   loc_9_0D2B
        jmp    SHORT loc_9_0D66
loc_9_0D44:
        lea    esi, [edi + 240h]
        mov    ecx, dword ptr [ebp - 14h]
loc_9_0D4D:
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, 40h
        jae    NEAR PTR loc_9_0E99
        cmp    byte ptr [eax + esi], 0ffh
        je     NEAR PTR loc_9_0E99
        inc    eax
        loop   loc_9_0D4D
loc_9_0D66:
        mov    esi, dword ptr [edx]
        lea    edi, [edi + 4]
loc_9_0D6B:
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 2ch]
        cmp    dword ptr [ebp + 4], 0
        je     SHORT loc_9_0D94
        cmp    dword ptr [ebp + 4], 1
        je     SHORT loc_9_0D82
        mov    eax, dword ptr [ebp - 10h]
        jmp    SHORT loc_9_0DA2
loc_9_0D82:
        mov    eax, dword ptr [ebp - 10h]
        test   eax, 3ffh
        stc
        jne    SHORT loc_9_0D92
        shr    eax, 7
        add    eax, edi
loc_9_0D92:
        jmp    SHORT loc_9_0DA2
loc_9_0D94:
        mov    ecx, dword ptr [ebp - 10h]
        movzx  eax, byte ptr [ecx + eax + 240h]
        lea    eax, [edi + eax*8]
loc_9_0DA2:
        mov    cl, byte ptr [edx + 2dh]
        mov    dword ptr [ebp - 0ch], eax
        test   byte ptr [eax + 2], 2
        je     SHORT loc_9_0DB6
        cmp    byte ptr [eax], cl
        jne    NEAR PTR loc_9_0E99
loc_9_0DB6:
        mov    byte ptr [eax], cl
        mov    cl, byte ptr [edx + 2ch]
        and    cl, 0e0h
        and    byte ptr [eax + 2], 13h
        or     byte ptr [eax + 2], cl
        mov    ecx, dword ptr [ebp - 8]
        test   byte ptr [eax + 2], 2
        je     SHORT loc_9_0DD8
        cmp    word ptr [eax + 4], cx
        jne    NEAR PTR loc_9_0E99
loc_9_0DD8:
        mov    word ptr [eax + 4], cx
        mov    ecx, dword ptr [ebp - 4]
        cmp    ecx, 4
        jbe    SHORT loc_9_0DE9
        mov    ecx, 4
loc_9_0DE9:
        mov    byte ptr [eax + 1], cl
        sub    eax, edi
        shr    eax, 1
        push   edi
        mov    edi, dword ptr [ebp - 2ch]
        test   byte ptr [edi + 68eh], 4
        pop    edi
        jne    SHORT loc_9_0E0A
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jb     NEAR PTR loc_9_0E99
loc_9_0E0A:
        test   byte ptr [edx + 2ch], 80h
        jne    SHORT loc_9_0E77
        mov    edx, dword ptr [ebp - 0ch]
        test   byte ptr [edx + 2], 2
        jne    SHORT loc_9_0E46
        push   edx
        mov    edx, dword ptr [ebp - 18h]
        test   byte ptr [edx + 2ch], 20h
        jne    SHORT loc_9_0E2F
        call   sub_9_0EA5
        jae    SHORT loc_9_0E2F
        pop    edx
        xor    eax, eax
        jmp    SHORT loc_9_0E42
loc_9_0E2F:
        pop    edx
        push   10h
        push   dword ptr [ebp - 8]
        push   ecx
        push   eax
        push   ebx
        push   esi
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
loc_9_0E42:
        or     eax, eax
        je     SHORT loc_9_0E93
loc_9_0E46:
        cmp    dword ptr [ebp + 4], 0
        je     SHORT loc_9_0E61
        cmp    dword ptr [ebp + 4], 1
        je     SHORT loc_9_0E58
        add    dword ptr [ebp - 10h], 8
        jmp    SHORT loc_9_0E64
loc_9_0E58:
        add    dword ptr [ebp - 10h], 400h
        jmp    SHORT loc_9_0E64
loc_9_0E61:
        inc    dword ptr [ebp - 10h]
loc_9_0E64:
        add    dword ptr [ebp - 8], 4
        sub    dword ptr [ebp - 4], 4
        ja     NEAR PTR loc_9_0D6B
        clc
        popad
loc_9_0E74:
        leave
        pop    ecx
        ret
loc_9_0E77:
        mov    edx, dword ptr [ebp - 0ch]
        test   byte ptr [edx + 2], 2
        jne    SHORT loc_9_0E46
        push   0
        push   dword ptr [ebp - 8]
        push   ecx
        push   eax
        push   ebx
        push   esi
        call   sub_9_10D3
        add    esp, 18h
        jmp    SHORT loc_9_0E42
loc_9_0E93:
        popad
        mov    ah, 80h
        stc
        jmp    SHORT loc_9_0E74
loc_9_0E99:
        popad
        mov    ah, 8bh
        stc
        jmp    SHORT loc_9_0E74
loc_9_0E9F:
        popad
        mov    ah, 8ah
        stc
        jmp    SHORT loc_9_0E74
sub_9_0EA5:
        push   edx
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        test   byte ptr [edx + ebx + 2ch], 20h
        jne    SHORT loc_9_0EBA
        call   sub_9_13F2
        jae    SHORT loc_9_0EBA
loc_9_0EBA:
        pop    edx
        ret
sub_9_0EBC:
        push   edi
        push   esi
        push   edx
        push   ecx
        push   ebx
        or     ecx, ecx
        je     NEAR PTR loc_9_0F5F
        bts    word ptr [edi + 68eh], 5
        jb     NEAR PTR loc_9_0F65
        test   ecx, 0ffffff00h
        jne    NEAR PTR loc_9_0F69
        cmp    byte ptr [edi + 68fh], 0
        je     SHORT loc_9_0F69
        cmp    cl, byte ptr [edi + 68fh]
        ja     SHORT loc_9_0F69
        or     eax, eax
        je     SHORT loc_9_0F28
loc_9_0EF9:
        push   ecx
        push   esi
        push   edx
        movzx  eax, word ptr [esi + 2]
        movzx  esi, word ptr [esi]
        cmp    esi, 0ffffh
        jne    SHORT loc_9_0F12
        xor    esi, esi
        db  0BAh, 0F2h, 00Eh, 000h, 000h
loc_9_0F12:
        mov    ecx, 1
        call   sub_9_0CA6
        pop    edx
        pop    esi
        pop    ecx
        jb     SHORT loc_9_0F6B
        add    esi, 4
        loop   loc_9_0EF9
        jmp    SHORT loc_9_0F55
loc_9_0F28:
        push   ecx
        push   esi
        push   edx
        movzx  eax, word ptr [esi + 2]
        movzx  esi, word ptr [esi]
        cmp    esi, 0ffffh
        jne    SHORT loc_9_0F41
        xor    esi, esi
        db  0BAh, 0F2h, 00Eh, 000h, 000h
loc_9_0F41:
        mov    ecx, 1
        call   sub_9_0CAA
        pop    edx
        pop    esi
        pop    ecx
        jb     SHORT loc_9_0F6B
        add    esi, 4
        loop   loc_9_0F28
loc_9_0F55:
        btr    word ptr [edi + 68eh], 5
        clc
loc_9_0F5F:
        pop    ebx
        pop    ecx
        pop    edx
        pop    esi
        pop    edi
        ret
loc_9_0F65:
        mov    ah, 80h
        jmp    SHORT loc_9_0F74
loc_9_0F69:
        mov    ah, 8bh
loc_9_0F6B:
        btr    word ptr [edi + 68eh], 5
loc_9_0F74:
        stc
        jmp    SHORT loc_9_0F5F
sub_9_0F77:
        bt     dword ptr [esi + ebx + 2ch], 3
        jae    SHORT loc_9_0FE1
        bts    word ptr [edi + 68eh], 0
        cmc
        jae    SHORT loc_9_0FE1
        cmp    word ptr [edi + 68ah], 0
        je     SHORT loc_9_0FE1
        pushad
        movzx  edx, word ptr [edi + 68ah]
        lea    ecx, [edi + 4]
        test   edx, 3ffh
        stc
        jne    SHORT loc_9_0FAE
        shr    edx, 7
        add    edx, ecx
loc_9_0FAE:
        sub    edx, ecx
        shr    edx, 1
        lea    edx, [ebx + edx*2 + 64h]
        db  003h, 015h, 0FCh, 011h, 000h, 000h
        mov    esi, edx
        mov    ecx, 10h
loc_9_0FC3:
        test   byte ptr [edx + 1], 0eh
        stc
        jne    SHORT loc_9_0FE0
        add    edx, 2
        loop   loc_9_0FC3
        mov    edx, esi
        mov    ecx, 10h
loc_9_0FD6:
        or     byte ptr [edx + 1], 1
        add    edx, 2
        loop   loc_9_0FD6
        clc
loc_9_0FE0:
        popad
loc_9_0FE1:
        ret
sub_9_0FE2:
        bt     dword ptr [esi + ebx + 2ch], 3
        jae    NEAR PTR loc_9_10CE
        bts    word ptr [edi + 68eh], 1
        cmc
        jae    NEAR PTR loc_9_10CE
        pushad
        lea    esi, [edi + 240h]
        movzx  ecx, byte ptr [edi + 68fh]
        or     ecx, ecx
        je     NEAR PTR loc_9_10CC
loc_9_1014:
        cld
        lodsb
        movzx  eax, al
        shl    eax, 2
        push   ecx
        push   eax
        lea    edx, [edi + 4]
        test   eax, 3
        stc
        jne    SHORT loc_9_102D
        shl    eax, 1
        add    eax, edx
loc_9_102D:
        jb     NEAR PTR loc_9_10CF
        test   byte ptr [eax + 2], 2
        jne    SHORT loc_9_1078
        pop    eax
        push   eax
        lea    eax, [ebx + eax*2 + 64h]
        db  003h, 005h, 0FCh, 011h, 000h, 000h
        mov    ecx, 4
loc_9_104A:
        test   byte ptr [eax + 1], 0eh
        je     SHORT loc_9_1073
        db  083h, 03Dh, 098h, 000h, 000h, 000h, 000h
        stc
        je     SHORT loc_9_10CF
        pop    eax
        push   eax
        lea    edx, [edi + 4]
        test   eax, 3
        stc
        jne    SHORT loc_9_106B
        shl    eax, 1
        add    eax, edx
loc_9_106B:
        jb     SHORT loc_9_10CF
        or     byte ptr [eax + 2], 2
        jmp    SHORT loc_9_1078
loc_9_1073:
        add    eax, 2
        loop   loc_9_104A
loc_9_1078:
        pop    eax
        pop    ecx
        dec    ecx
        jne    SHORT loc_9_1014
        lea    esi, [edi + 240h]
        movzx  ecx, byte ptr [edi + 68fh]
        or     ecx, ecx
        je     SHORT loc_9_10CC
loc_9_108E:
        cld
        lodsb
        movzx  eax, al
        shl    eax, 2
        push   eax
        lea    edx, [edi + 4]
        test   eax, 3
        stc
        jne    SHORT loc_9_10A6
        shl    eax, 1
        add    eax, edx
loc_9_10A6:
        jb     SHORT loc_9_10D0
        test   byte ptr [eax + 2], 2
        pop    eax
        jne    SHORT loc_9_10C9
        lea    eax, [ebx + eax*2 + 64h]
        db  003h, 005h, 0FCh, 011h, 000h, 000h
        push   ecx
        mov    ecx, 4
loc_9_10BF:
        or     byte ptr [eax + 1], 1
        add    eax, 2
        loop   loc_9_10BF
        pop    ecx
loc_9_10C9:
        dec    ecx
        jne    SHORT loc_9_108E
loc_9_10CC:
        clc
loc_9_10CD:
        popad
loc_9_10CE:
        ret
loc_9_10CF:
        pop    eax
loc_9_10D0:
        pop    eax
        jmp    SHORT loc_9_10CD
sub_9_10D3:
        enter  14h, 0
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        movzx  ecx, word ptr [esi]
        lea    esi, [esi + 2]
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 14h], esi
        mov    dword ptr [ebp - 8], ecx
        xor    eax, eax
loc_9_10EE:
        movzx  edx, word ptr [esi]
        add    eax, edx
        add    esi, 4
        loop   loc_9_10EE
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 10h], 0
        mov    ecx, dword ptr [ebp + 18h]
        jecxz  loc_9_112E
        cmp    ecx, dword ptr [ebp - 0ch]
        jae    SHORT loc_9_1185
        mov    edi, dword ptr [ebp - 4]
loc_9_110F:
        movzx  edx, word ptr [edi]
        xor    eax, eax
        sub    ecx, edx
        je     SHORT loc_9_1125
        jb     SHORT loc_9_111F
        add    edi, 4
        jmp    SHORT loc_9_110F
loc_9_111F:
        add    ecx, edx
        mov    eax, ecx
        jmp    SHORT loc_9_1128
loc_9_1125:
        add    edi, 4
loc_9_1128:
        mov    dword ptr [ebp - 14h], edi
        mov    dword ptr [ebp - 10h], eax
loc_9_112E:
        mov    ecx, dword ptr [ebp + 14h]
        add    ecx, dword ptr [ebp + 18h]
        cmp    ecx, dword ptr [ebp - 0ch]
        ja     SHORT loc_9_1185
        mov    ecx, dword ptr [ebp + 14h]
        or     ecx, ecx
        je     SHORT loc_9_117D
        mov    esi, dword ptr [ebp - 14h]
loc_9_1143:
        xor    eax, eax
        xchg   dword ptr [ebp - 10h], eax
        push   ecx
        movzx  edi, word ptr [esi]
        sub    edi, eax
        cmp    edi, ecx
        jbe    SHORT loc_9_1154
        mov    edi, ecx
loc_9_1154:
        movzx  ecx, word ptr [esi + 2]
        add    ecx, eax
        push   dword ptr [ebp + 1ch]
        push   edi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   ecx
        db  0CDh, 020h, 05Eh, 000h, 001h, 000h
        add    esp, 14h
        pop    ecx
        or     eax, eax
        je     SHORT loc_9_1180
        add    esi, 4
        add    dword ptr [ebp + 10h], edi
        sub    ecx, edi
        ja     SHORT loc_9_1143
loc_9_117D:
        xor    eax, eax
        dec    eax
loc_9_1180:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_9_1185:
        xor    eax, eax
        jmp    SHORT loc_9_1180
        db  000h, 000h, 000h
sub_9_118C:
        db  00Fh, 0BAh, 02Dh, 004h, 012h, 000h, 000h, 000h
        jb     NEAR PTR loc_9_12B1
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        db  0A3h, 000h, 000h, 000h, 000h
        db  0A1h, 004h, 000h, 000h, 000h
        inc    eax
        je     SHORT loc_9_11B4
        dec    eax
        mov    dword ptr [19ch], eax
loc_9_11B4:
        db  083h, 03Dh, 098h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_9_12A2
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 004h
        jne    NEAR PTR loc_9_12A2
        db  08Bh, 03Dh, 098h, 000h, 000h, 000h
        mov    esi, edi
        lea    edi, [edi + 204h]
        mov    ecx, 0fh
        xor    eax, eax
        cld
        repe scasd
        jne    NEAR PTR loc_9_12B3
        mov    edi, esi
        mov    eax, dword ptr [edi]
        sub    esi, esi
        db  087h, 035h, 0A0h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_9_1202
        db  089h, 035h, 09Ch, 000h, 000h, 000h
loc_9_1202:
        db  08Bh, 035h, 09Ch, 000h, 000h, 000h
        cmp    eax, dword ptr [esi + 6]
        jne    NEAR PTR loc_9_12B3
        lea    esi, [edi + 4]
        mov    ebx, esi
        mov    ecx, 40h
loc_9_121B:
        test   byte ptr [esi + 2], 3
        jne    SHORT loc_9_1298
        mov    eax, esi
        sub    eax, ebx
        shr    eax, 1
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jae    SHORT loc_9_123B
        cmp    byte ptr [esi], 0
        je     SHORT loc_9_1298
        cmp    byte ptr [esi], 0ffh
        jne    SHORT loc_9_12B3
        jmp    SHORT loc_9_1298
loc_9_123B:
        movzx  edx, byte ptr [esi]
        or     edx, edx
        je     SHORT loc_9_1298
        cmp    dl, 0ffh
        je     SHORT loc_9_1298
        cmp    edx, 0ffh
        jae    SHORT loc_9_1298
        mov    edx, dword ptr [edi + edx*4 + 280h]
        or     edx, edx
        je     SHORT loc_9_1298
        test   byte ptr [edx + 2ch], 80h
        je     SHORT loc_9_12B3
        push   edi
        db  08Bh, 03Dh, 09Ch, 000h, 000h, 000h
        lea    edi, [edi + 0ah]
        push   ecx
        mov    ecx, eax
        test   ecx, 3
        stc
        jne    SHORT loc_9_1284
        shr    ecx, 2
        mov    eax, ecx
        shl    eax, 1
        shl    ecx, 2
        add    ecx, eax
        add    ecx, edi
loc_9_1284:
        jb     SHORT loc_9_12B3
        mov    al, byte ptr [esi]
        mov    byte ptr [ecx + 1], al
        movzx  eax, word ptr [esi + 4]
        shr    eax, 2
        mov    word ptr [ecx + 2], ax
        pop    ecx
        pop    edi
loc_9_1298:
        add    esi, 8
        dec    ecx
        jne    NEAR PTR loc_9_121B
loc_9_12A2:
        db  0A1h, 094h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_9_12B1
        db  0CDh, 020h, 00Ah, 000h, 001h, 000h
loc_9_12B1:
        clc
        ret
loc_9_12B3:
        db  00Fh, 0B7h, 035h, 00Ch, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h
        mov    eax, 1
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_9_12CB:
        inc    word ptr [ebp + 24h]
        movzx  eax, word ptr [ebp + 38h]
        shl    eax, 4
        movzx  ecx, word ptr [ebp + 10h]
        add    eax, ecx
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        mov    dword ptr [esi + ebx + 58h], eax
        ret
loc_9_12E7:
        inc    word ptr [ebp + 24h]
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        mov    eax, dword ptr [esi + ebx + 58h]
        mov    word ptr [eax + 3], 100h
        ret
loc_9_12FC:
        inc    word ptr [ebp + 24h]
        and    byte ptr [ebp + 2ch], 0feh
        mov    ax, word ptr [ebp + 1ch]
        cmp    ah, 0
        je     SHORT loc_9_1320
        cmp    ah, 1
        jne    SHORT loc_9_131A
        cmp    al, 2
        ja     SHORT loc_9_131A
        cmp    al, 1
        jne    SHORT loc_9_1320
loc_9_131A:
        mov    ah, 1
        or     byte ptr [ebp + 2ch], 1
loc_9_1320:
        ret
loc_9_1321:
        inc    word ptr [ebp + 24h]
        jmp    SHORT loc_9_134A
loc_9_1327:
        stc
        ret
loc_9_1329:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        jmp    SHORT loc_9_134A
loc_9_1331:
        mov    eax, 67h
        db  0CDh, 020h, 042h, 000h, 001h, 000h
        shl    ecx, 10h
        mov    cx, dx
        db  03Bh, 00Dh, 018h, 000h, 000h, 000h
        jne    SHORT loc_9_1327
loc_9_134A:
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        movzx  eax, byte ptr [ebp + 1dh]
        sub    eax, 40h
        jb     SHORT loc_9_13B6
        cmp    eax, 1eh
        jae    SHORT loc_9_13B6
        db  083h, 03Dh, 098h, 000h, 000h, 000h, 000h
        jne    SHORT loc_9_1370
        test   byte ptr [esi + ebx + 2fh], 80h
        je     SHORT loc_9_13CD
loc_9_1370:
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        db  0FEh, 005h, 0EAh, 002h, 000h, 000h
        inc    byte ptr [esi + ebx + 27h]
        push   ebx
        db  0FFh, 014h, 085h, 01Ch, 000h, 000h, 000h
        pop    ebx
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        cmp    byte ptr [esi + ebx + 27h], 0
        je     SHORT loc_9_139F
        dec    byte ptr [esi + ebx + 27h]
loc_9_139F:
        db  080h, 03Dh, 0EAh, 002h, 000h, 000h, 000h
        je     SHORT loc_9_13AE
        db  0FEh, 00Dh, 0EAh, 002h, 000h, 000h
loc_9_13AE:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_9_13B4:
        clc
        ret
loc_9_13B6:
        cmp    byte ptr [ebp + 1dh], 0deh
        jne    SHORT loc_9_13C7
        push   20h
        push   ebx
        db  0CDh, 020h, 019h, 000h, 017h, 000h
        pop    ecx
        pop    ecx
loc_9_13C7:
        mov    byte ptr [ebp + 1dh], 84h
        jmp    SHORT loc_9_13B4
loc_9_13CD:
        or     byte ptr [esi + ebx + 2fh], 80h
        mov    dword ptr [esi + ebx + 50h], 0
        mov    dword ptr [esi + ebx + 54h], 200h
        jmp    SHORT loc_9_1370
loc_9_13E4:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        jmp    SHORT loc_9_13B6
sub_9_13F2:
        db  080h, 03Dh, 015h, 012h, 000h, 000h, 002h
        clc
        jne    SHORT loc_9_1404
        pushad
        db  0CDh, 020h, 006h, 000h, 007h, 000h
        popad
loc_9_1404:
        ret
V86MMGR_LocalGlobalReg:
        pushad
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    edx, edi
        ja     NEAR PTR loc_9_14CE
        cmp    edx, 110h
        jae    NEAR PTR loc_9_14CE
        db  03Bh, 015h, 000h, 000h, 000h, 000h
        jb     NEAR PTR loc_9_14CE
        cmp    edx, 100h
        jae    SHORT loc_9_14A7
        db  08Bh, 035h, 09Ch, 000h, 000h, 000h
        or     esi, esi
        je     NEAR PTR loc_9_14CE
        cmp    byte ptr [esi + 18bh], 0
        je     SHORT loc_9_14CE
        mov    ecx, edi
        sub    ecx, edx
        inc    ecx
        mov    eax, edx
        db  08Dh, 004h, 045h, 028h, 00Fh, 000h, 000h
        xor    esi, esi
loc_9_1461:
        test   word ptr [eax], 2000h
        je     SHORT loc_9_1474
        test   word ptr [eax], 4000h
        jne    SHORT loc_9_1479
        or     esi, 1
        jmp    SHORT loc_9_147C
loc_9_1474:
        or     esi, 4
        jmp    SHORT loc_9_147C
loc_9_1479:
        or     esi, 2
loc_9_147C:
        add    eax, 2
        loop   loc_9_1461
        or     esi, esi
        je     SHORT loc_9_14CE
        cmp    esi, 4
        je     SHORT loc_9_14CE
        cmp    esi, 3
        je     SHORT loc_9_14C4
        test   si, 4
        jne    SHORT loc_9_14C4
        test   si, 1
        jne    SHORT loc_9_14BA
loc_9_149D:
        mov    dword ptr [esp + 1ch], 0
loc_9_14A5:
        popad
        ret
loc_9_14A7:
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 020h
        je     SHORT loc_9_14B2
        jmp    SHORT loc_9_149D
loc_9_14B2:
        cmp    edi, 110h
        jae    SHORT loc_9_14C4
loc_9_14BA:
        mov    dword ptr [esp + 1ch], 1
        jmp    SHORT loc_9_14A5
loc_9_14C4:
        mov    dword ptr [esp + 1ch], 2
        jmp    SHORT loc_9_14A5
loc_9_14CE:
        mov    dword ptr [esp + 1ch], 3
        jmp    SHORT loc_9_14A5
loc_9_14D8:
        mov    byte ptr [ebp + 1ch], 40h
loc_9_14DC:
        mov    byte ptr [ebp + 1dh], 0
        ret
loc_9_14E1:
        db  083h, 03Dh, 008h, 000h, 000h, 000h, 000h
        je     SHORT loc_9_1556
        test   byte ptr [esi + ebx + 2ch], 2
        jne    SHORT loc_9_1556
loc_9_14F1:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_2BF4
        call   sub_9_0F77
        jb     NEAR PTR loc_9_2BF4
        movzx  eax, word ptr [edi + 68ah]
        or     eax, eax
        jne    SHORT loc_9_156C
        mov    word ptr [ebp + 10h], 0b000h
        lea    edx, [edi + 4]
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        add    eax, 3
        and    eax, 0fffffffch
        mov    ecx, 100h
        sub    ecx, eax
        jbe    SHORT loc_9_1551
        shr    ecx, 2
        jecxz  loc_9_1551
        test   eax, 3
        stc
        jne    SHORT loc_9_1542
        shl    eax, 1
        add    eax, edx
loc_9_1542:
        test   byte ptr [eax + 2], 1
        jne    SHORT loc_9_1565
        sub    eax, edx
        shl    eax, 7
        mov    word ptr [ebp + 10h], ax
loc_9_1551:
        mov    byte ptr [ebp + 1dh], 81h
        ret
loc_9_1556:
        cmp    dword ptr [esi + ebx + 5ch], 0
        jne    SHORT loc_9_14F1
        mov    word ptr [ebp + 10h], 0b000h
        jmp    SHORT loc_9_1551
loc_9_1565:
        add    eax, 8
        loop   loc_9_1542
        jmp    SHORT loc_9_1551
loc_9_156C:
        mov    word ptr [ebp + 10h], ax
        jmp    NEAR PTR loc_9_14DC
loc_9_1575:
        xor    ecx, ecx
        xor    edx, edx
        db  083h, 03Dh, 008h, 000h, 000h, 000h, 000h
        je     SHORT loc_9_15A2
        test   byte ptr [esi + ebx + 2ch], 2
        jne    SHORT loc_9_15A2
        call   sub_9_13F2
        jae    SHORT loc_9_1592
        jmp    SHORT loc_9_15A2
loc_9_1592:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_2BF4
        call   sub_9_15AF
loc_9_15A2:
        mov    word ptr [ebp + 14h], cx
        mov    word ptr [ebp + 10h], dx
        jmp    NEAR PTR loc_9_14DC
sub_9_15AF:
        push   0
        db  0CDh, 020h, 059h, 000h, 001h, 000h
        pop    ecx
        xchg   ecx, eax
        mov    eax, 80h
        test   byte ptr [esi + ebx + 2ch], 20h
        je     SHORT loc_9_15CF
        xor    eax, eax
        cmp    ecx, edx
        jbe    SHORT loc_9_15CF
        mov    al, 10h
        mov    ecx, edx
loc_9_15CF:
        sub    ecx, eax
        jae    SHORT loc_9_15D5
        xor    ecx, ecx
loc_9_15D5:
        mov    eax, dword ptr [edi + 67ch]
        mov    edx, dword ptr [edi + 280h]
        or     edx, edx
        je     SHORT loc_9_15EE
        mov    edx, dword ptr [edx + 28h]
        sub    eax, edx
        jae    SHORT loc_9_15EE
        xor    eax, eax
loc_9_15EE:
        pushad
        sub    esp, 40h
        mov    edi, esp
        push   0
        push   edi
        db  0CDh, 020h, 06Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        mov    edx, dword ptr [edi]
        cmp    dword ptr [esi + ebx + 54h], -1
        jne    SHORT loc_9_1611
        mov    eax, dword ptr [edi + 4]
        cmp    edx, eax
        jb     SHORT loc_9_1611
        mov    edx, eax
loc_9_1611:
        add    esp, 40h
        cmp    edx, dword ptr [esi + ebx + 54h]
        jbe    SHORT loc_9_161E
        mov    edx, dword ptr [esi + ebx + 54h]
loc_9_161E:
        mov    dword ptr [esp + 14h], edx
        popad
        db  03Bh, 015h, 008h, 000h, 000h, 000h
        jbe    SHORT loc_9_1631
        db  08Bh, 015h, 008h, 000h, 000h, 000h
loc_9_1631:
        cmp    dword ptr [esi + ebx + 48h], 0
        je     SHORT loc_9_164C
        mov    ecx, edx
        mov    edx, dword ptr [esi + ebx + 48h]
        shr    edx, 2
        shr    ecx, 2
        cmp    edx, ecx
        jbe    SHORT loc_9_1663
        mov    ecx, edx
        jmp    SHORT loc_9_1663
loc_9_164C:
        shr    eax, 2
        shr    edx, 2
        shr    ecx, 2
        push   edx
        sub    edx, eax
        jae    SHORT loc_9_165C
        xor    edx, edx
loc_9_165C:
        cmp    edx, ecx
        jbe    SHORT loc_9_1662
        mov    edx, ecx
loc_9_1662:
        pop    ecx
loc_9_1663:
        cmp    edx, 400h
        jbe    SHORT loc_9_1670
        mov    edx, 400h
loc_9_1670:
        test   ecx, 0ffff0000h
        je     SHORT loc_9_167D
        mov    ecx, 0ffffh
loc_9_167D:
        ret
loc_9_167E:
        db  083h, 03Dh, 008h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_9_187F
        test   byte ptr [esi + ebx + 2ch], 2
        jne    NEAR PTR loc_9_187F
        call   sub_9_00A4
        jb     NEAR PTR loc_9_187F
        mov    esi, edi
        bts    word ptr [esi + 68eh], 3
        jb     NEAR PTR loc_9_1871
        lea    edi, [edi + 280h]
        mov    ecx, 0ffh
        xor    eax, eax
        cld
        repne scasd
        jne    NEAR PTR loc_9_1868
        sub    edi, 4
        mov    eax, edi
        lea    ecx, [esi + 280h]
        sub    eax, ecx
        shr    eax, 2
        mov    word ptr [ebp + 14h], ax
        cmp    byte ptr [ebp + 1dh], 43h
        jne    SHORT loc_9_16EF
        cmp    word ptr [ebp + 10h], 0
        je     NEAR PTR loc_9_185A
        jmp    SHORT loc_9_1705
loc_9_16EF:
        cmp    byte ptr [ebp + 1ch], 1
        ja     SHORT loc_9_16F7
        jmp    SHORT loc_9_1705
loc_9_16F7:
        btr    word ptr [esi + 68eh], 3
        jmp    NEAR PTR loc_9_2EE9
loc_9_1705:
        push   edi
        mov    edi, esi
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        call   sub_9_15AF
        mov    esi, edi
        pop    edi
        movzx  edx, word ptr [ebp + 10h]
        cmp    edx, ecx
        ja     NEAR PTR loc_9_1876
        or     edx, edx
        je     SHORT loc_9_1777
        cmp    edx, 4000h
        ja     NEAR PTR loc_9_1876
        shl    edx, 2
        mov    ecx, edx
        db  003h, 00Dh, 00Ch, 000h, 000h, 000h
        add    edx, dword ptr [esi + 67ch]
        mov    eax, dword ptr [esi + 280h]
        or     eax, eax
        je     SHORT loc_9_1750
        mov    eax, dword ptr [eax + 28h]
loc_9_1750:
        sub    ecx, eax
        jae    SHORT loc_9_1756
        xor    ecx, ecx
loc_9_1756:
        sub    edx, eax
        jae    SHORT loc_9_175C
        xor    edx, edx
loc_9_175C:
        db  03Bh, 00Dh, 008h, 000h, 000h, 000h
        ja     NEAR PTR loc_9_1939
        db  0A1h, 0FCh, 011h, 000h, 000h
        cmp    edx, dword ptr [eax + ebx + 54h]
        ja     NEAR PTR loc_9_1939
loc_9_1777:
        push   101h
        push   36h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_9_1939
        mov    dword ptr [edi], eax
        btr    word ptr [esi + 68eh], 3
        movzx  edx, word ptr [ebp + 10h]
        or     edx, edx
        je     NEAR PTR loc_9_1848
        shl    edx, 2
        push   edx
        xor    eax, eax
        db  08Bh, 00Dh, 0FCh, 011h, 000h, 000h
        test   byte ptr [ecx + ebx + 2ch], 20h
        je     SHORT loc_9_17BD
        or     eax, 80h
loc_9_17BD:
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   edx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_9_1884
        mov    edi, dword ptr [edi]
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi + 28h], ecx
        add    dword ptr [esi + 67ch], ecx
        db  001h, 00Dh, 00Ch, 000h, 000h, 000h
        movzx  eax, word ptr [ebp + 14h]
        mov    byte ptr [edi + 2dh], al
loc_9_17F9:
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        mov    eax, dword ptr [edx + ebx + 48h]
        or     eax, eax
        je     NEAR PTR loc_9_14DC
        sub    eax, ecx
        jbe    SHORT loc_9_182E
        mov    dword ptr [edx + ebx + 48h], eax
        push   edx
        push   0
        push   eax
        push   dword ptr [edx + ebx + 44h]
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        mov    dword ptr [edx + ebx + 44h], eax
        jmp    NEAR PTR loc_9_14DC
loc_9_182E:
        xor    eax, eax
        mov    dword ptr [edx + ebx + 48h], eax
        xchg   dword ptr [edx + ebx + 44h], eax
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        jmp    NEAR PTR loc_9_14DC
loc_9_1848:
        mov    byte ptr [eax + 2ch], 4
        mov    edi, eax
        movzx  eax, word ptr [ebp + 14h]
        mov    byte ptr [edi + 2dh], al
        jmp    NEAR PTR loc_9_14DC
loc_9_185A:
        btr    word ptr [esi + 68eh], 3
        mov    byte ptr [ebp + 1dh], 89h
        ret
loc_9_1868:
        btr    word ptr [esi + 68eh], 3
loc_9_1871:
        mov    byte ptr [ebp + 1dh], 85h
        ret
loc_9_1876:
        btr    word ptr [esi + 68eh], 3
loc_9_187F:
        mov    byte ptr [ebp + 1dh], 87h
        ret
loc_9_1884:
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        mov    eax, dword ptr [edx + ebx + 48h]
        or     eax, eax
        je     NEAR PTR loc_9_1925
        sub    eax, ecx
        jb     NEAR PTR loc_9_1925
        je     SHORT loc_9_18BE
        mov    dword ptr [edx + ebx + 48h], eax
        push   ecx
        push   edx
        push   0
        push   eax
        push   dword ptr [edx + ebx + 44h]
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        pop    ecx
        mov    dword ptr [edx + ebx + 44h], eax
        jmp    SHORT loc_9_18D3
loc_9_18BE:
        mov    dword ptr [edx + ebx + 48h], eax
        xchg   dword ptr [edx + ebx + 44h], eax
        push   ecx
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
loc_9_18D3:
        xor    eax, eax
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        test   byte ptr [edx + ebx + 2ch], 20h
        je     SHORT loc_9_18E7
        or     eax, 80h
loc_9_18E7:
        push   ecx
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        pop    ecx
        or     eax, eax
        je     SHORT loc_9_1925
        mov    edi, dword ptr [edi]
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi + 28h], ecx
        add    dword ptr [esi + 67ch], ecx
        db  001h, 00Dh, 00Ch, 000h, 000h, 000h
        movzx  eax, word ptr [ebp + 14h]
        mov    byte ptr [edi + 2dh], al
        jmp    NEAR PTR loc_9_14DC
loc_9_1925:
        xor    eax, eax
        xchg   dword ptr [edi], eax
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_9_1934:
        mov    byte ptr [ebp + 1dh], 88h
        ret
loc_9_1939:
        btr    word ptr [esi + 68eh], 3
        jmp    SHORT loc_9_1934
loc_9_1944:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_1BB8
        cmp    byte ptr [ebp + 1ch], 4
        jb     SHORT loc_9_1962
        call   sub_9_0FE2
        jb     NEAR PTR loc_9_2BF4
        jmp    SHORT loc_9_196D
loc_9_1962:
        call   sub_9_0F77
        jb     NEAR PTR loc_9_2BF4
loc_9_196D:
        movzx  edx, word ptr [ebp + 14h]
        cmp    edx, 0ffh
        jae    NEAR PTR loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 280h]
        or     edx, edx
        je     NEAR PTR loc_9_1BB8
        movzx  esi, word ptr [ebp + 10h]
        cmp    esi, 0ffffh
        jne    SHORT loc_9_199D
        db  0BAh, 0F2h, 00Eh, 000h, 000h
loc_9_199D:
        mov    ecx, 1
        movzx  eax, byte ptr [ebp + 1ch]
        bts    word ptr [edi + 68eh], 5
        jb     NEAR PTR loc_9_2BF4
        call   sub_9_0CAA
        pushfd
        and    byte ptr [edi + 68eh], 0dfh
        popfd
        jae    NEAR PTR loc_9_14DC
loc_9_19C9:
        mov    byte ptr [ebp + 1dh], ah
        ret
loc_9_19CD:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_1BB8
        movzx  edx, word ptr [ebp + 14h]
        or     edx, edx
        je     NEAR PTR loc_9_1BB8
        cmp    edx, 0ffh
        jae    NEAR PTR loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 280h]
        or     edx, edx
        je     NEAR PTR loc_9_1BB8
        test   byte ptr [edx + 2ch], 80h
        jne    NEAR PTR loc_9_1BB8
        test   byte ptr [edx + 2ch], 2
        jne    NEAR PTR loc_9_1B0E
        xor    eax, eax
        movzx  edx, word ptr [ebp + 14h]
        bts    word ptr [edi + 68eh], 5
        jb     SHORT loc_9_1A5D
        lea    esi, [edi + 4]
        mov    ecx, 40h
loc_9_1A2C:
        test   byte ptr [esi + 2], 3
        jne    SHORT loc_9_1A4F
        cmp    dl, byte ptr [esi]
        jne    SHORT loc_9_1A4F
        push   esi
        push   ecx
        push   edx
        mov    eax, esi
        xor    esi, esi
        mov    ecx, 1
        db  0BAh, 0F2h, 00Eh, 000h, 000h
        call   sub_9_0CA2
        pop    edx
        pop    ecx
        pop    esi
loc_9_1A4F:
        add    esi, 8
        loop   loc_9_1A2C
        btr    word ptr [edi + 68eh], 5
loc_9_1A5D:
        xor    eax, eax
        movzx  edx, word ptr [ebp + 14h]
        xchg   dword ptr [edi + edx*4 + 280h], eax
        test   byte ptr [eax + 2ch], 4
        jne    NEAR PTR loc_9_1AFE
        push   eax
        mov    ecx, dword ptr [eax + 28h]
        sub    dword ptr [edi + 67ch], ecx
        db  029h, 00Dh, 00Ch, 000h, 000h, 000h
        mov    eax, dword ptr [eax]
        push   ecx
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        mov    eax, dword ptr [edx + ebx + 4ch]
        sub    eax, dword ptr [edx + ebx + 48h]
        jbe    SHORT loc_9_1AFD
        cmp    eax, ecx
        jae    SHORT loc_9_1AA9
        mov    ecx, eax
loc_9_1AA9:
        mov    eax, dword ptr [edx + ebx + 44h]
        add    ecx, dword ptr [edx + ebx + 48h]
        push   ecx
        test   byte ptr [edx + ebx + 2ch], 20h
        je     SHORT loc_9_1AC0
        mov    edx, 80h
        jmp    SHORT loc_9_1AC2
loc_9_1AC0:
        xor    edx, edx
loc_9_1AC2:
        or     eax, eax
        je     SHORT loc_9_1AD4
        push   edx
        push   ecx
        push   eax
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        jmp    SHORT loc_9_1AEA
loc_9_1AD4:
        push   edx
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
loc_9_1AEA:
        pop    ecx
        or     eax, eax
        je     SHORT loc_9_1AFD
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        mov    dword ptr [edx + ebx + 44h], eax
        mov    dword ptr [edx + ebx + 48h], ecx
loc_9_1AFD:
        pop    eax
loc_9_1AFE:
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        jmp    NEAR PTR loc_9_14DC
loc_9_1B0E:
        mov    byte ptr [ebp + 1dh], 86h
        ret
loc_9_1B13:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_1BBD
        call   sub_9_0F77
        jb     NEAR PTR loc_9_2BF4
        cmp    word ptr [edi + 68ah], 0
        je     NEAR PTR loc_9_1551
        movzx  edx, word ptr [ebp + 14h]
        or     edx, edx
        je     SHORT loc_9_1BB8
        cmp    edx, 0ffh
        jae    SHORT loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 280h]
        or     edx, edx
        je     SHORT loc_9_1BB8
        bts    word ptr [edx + 2ch], 3
        jb     NEAR PTR loc_9_2BF4
        bts    word ptr [edx + 2ch], 1
        jb     SHORT loc_9_1BA2
        bts    word ptr [edi + 68eh], 5
        jb     SHORT loc_9_1BAD
        lea    eax, [edx + 8]
        lea    esi, [edi + 4]
        push   edi
        mov    edi, eax
loc_9_1B7A:
        test   byte ptr [esi + 2], 10h
        jne    SHORT loc_9_1B85
        add    esi, 8
        jmp    SHORT loc_9_1B7A
loc_9_1B85:
        mov    ecx, 8
        cld
        rep movsd
        pop    edi
        btr    word ptr [edi + 68eh], 5
        btr    word ptr [edx + 2ch], 3
        jmp    NEAR PTR loc_9_14DC
loc_9_1BA2:
        btr    word ptr [edx + 2ch], 3
        mov    byte ptr [ebp + 1dh], 8dh
        ret
loc_9_1BAD:
        btr    word ptr [edx + 2ch], 3
        jmp    NEAR PTR loc_9_2BF4
loc_9_1BB8:
        mov    byte ptr [ebp + 1dh], 83h
        ret
loc_9_1BBD:
        mov    byte ptr [ebp + 1dh], 8ch
        ret
loc_9_1BC2:
        call   sub_9_00A4
        jb     SHORT loc_9_1BB8
        cmp    word ptr [edi + 68ah], 0
        je     NEAR PTR loc_9_1551
        movzx  edx, word ptr [ebp + 14h]
        cmp    edx, 0ffh
        jae    SHORT loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 280h]
        or     edx, edx
        je     SHORT loc_9_1BB8
        bts    word ptr [edx + 2ch], 3
        jb     NEAR PTR loc_9_2BF4
        btr    word ptr [edx + 2ch], 1
        jae    SHORT loc_9_1C4C
        sub    esp, 200h
        lea    eax, [edi + 4]
        mov    esi, esp
loc_9_1C0D:
        test   byte ptr [eax + 2], 10h
        jne    SHORT loc_9_1C1B
        add    esi, 8
        add    eax, 8
        jmp    SHORT loc_9_1C0D
loc_9_1C1B:
        push   edi
        mov    edi, esi
        lea    esi, [edx + 8]
        mov    ecx, 8
        cld
        rep movsd
        btr    word ptr [edx + 2ch], 3
        pop    edi
        mov    eax, esp
        xor    ecx, ecx
        db  0E8h, 03Bh, 0E4h, 0F6h, 0FFh
        lahf
        add    esp, 200h
        sahf
        jb     NEAR PTR loc_9_2BF4
        jmp    NEAR PTR loc_9_14DC
loc_9_1C4C:
        btr    word ptr [edx + 2ch], 3
        mov    byte ptr [ebp + 1dh], 8eh
        ret
loc_9_1C57:
        mov    byte ptr [ebp + 1dh], 84h
        ret
loc_9_1C5C:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_2BF4
        lea    esi, [edi + 280h]
        xor    edx, edx
        mov    ecx, 0ffh
loc_9_1C74:
        cld
        lodsd
        or     eax, eax
        je     SHORT loc_9_1C7B
        inc    edx
loc_9_1C7B:
        loop   loc_9_1C74
        mov    word ptr [ebp + 10h], dx
        jmp    NEAR PTR loc_9_14DC
loc_9_1C86:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_1BB8
        movzx  edx, word ptr [ebp + 14h]
        cmp    edx, 0ffh
        jae    NEAR PTR loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 280h]
        or     edx, edx
        je     NEAR PTR loc_9_1BB8
        mov    eax, dword ptr [edx + 28h]
        test   byte ptr [edx + 2ch], 40h
        je     SHORT loc_9_1CBF
        db  003h, 005h, 0A4h, 000h, 000h, 000h
loc_9_1CBF:
        add    eax, 3
        shr    eax, 2
        mov    word ptr [ebp + 10h], ax
        jmp    NEAR PTR loc_9_14DC
loc_9_1CCE:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_2BF4
        lea    esi, [edi + 280h]
        movzx  edi, word ptr [ebp + 38h]
        shl    edi, 4
        movzx  ecx, word ptr [ebp]
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        xor    edx, edx
        mov    ecx, 0ffh
loc_9_1CF6:
        cld
        lodsd
        or     eax, eax
        je     SHORT loc_9_1D22
        push   ebx
        movzx  ebx, byte ptr [eax + 2dh]
        mov    word ptr [edi], bx
        mov    ebx, dword ptr [eax + 28h]
        test   byte ptr [eax + 2ch], 40h
        je     SHORT loc_9_1D13
        db  003h, 01Dh, 0A4h, 000h, 000h, 000h
loc_9_1D13:
        add    ebx, 3
        shr    ebx, 2
        mov    word ptr [edi + 2], bx
        add    edi, 4
        inc    edx
        pop    ebx
loc_9_1D22:
        loop   loc_9_1CF6
        mov    word ptr [ebp + 10h], dx
        jmp    NEAR PTR loc_9_14DC
loc_9_1D2D:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_2BF4
        call   sub_9_0FE2
        jb     NEAR PTR loc_9_2BF4
        movzx  eax, word ptr [edx + 688h]
        test   eax, 0ffffff00h
        jne    NEAR PTR loc_9_2BF4
        movzx  eax, byte ptr [ebp + 1ch]
        or     al, al
        jne    SHORT loc_9_1D94
sub_9_1D5D:
        bts    word ptr [edi + 68eh], 5
        jb     NEAR PTR loc_9_2BF4
        lea    esi, [edi + 4]
        push   edi
        movzx  edi, word ptr [ebp + 38h]
        shl    edi, 4
        movzx  ecx, word ptr [ebp]
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        call   sub_9_0B04
        pop    edi
        btr    word ptr [edi + 68eh], 5
        jmp    NEAR PTR loc_9_14DC
loc_9_1D94:
        cmp    al, 1
        jne    SHORT loc_9_1DD6
loc_9_1D98:
        movzx  esi, word ptr [ebp + 3ch]
        shl    esi, 4
        movzx  ecx, word ptr [ebp + 4]
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        sub    esp, 200h
        mov    edx, esp
        call   sub_9_0B8E
        jb     SHORT loc_9_1E06
        mov    eax, esp
        mov    ecx, 1
        db  0E8h, 0B1h, 0E2h, 0F6h, 0FFh
        lahf
        add    esp, 200h
        sahf
        jb     NEAR PTR loc_9_2BF4
        jmp    NEAR PTR loc_9_14DC
loc_9_1DD6:
        cmp    al, 2
        jne    SHORT loc_9_1DEA
        push   edi
        push   edx
        call   sub_9_1D5D
        pop    edx
        pop    edi
        cmp    byte ptr [ebp + 1dh], 0
        je     SHORT loc_9_1D98
        ret
loc_9_1DEA:
        cmp    al, 3
        jne    NEAR PTR loc_9_2EE9
        mov    al, byte ptr [edx + 688h]
        mov    byte ptr [ebp + 1ch], al
        jmp    NEAR PTR loc_9_14DC
loc_9_1E00:
        mov    byte ptr [ebp + 1dh], 0a3h
        ret
loc_9_1E05:
        pop    eax
loc_9_1E06:
        add    esp, 200h
        jmp    SHORT loc_9_1E00
loc_9_1E0E:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_2BF4
        call   sub_9_0FE2
        jb     NEAR PTR loc_9_2BF4
        movzx  eax, byte ptr [ebp + 1ch]
        cmp    al, 2
        ja     NEAR PTR loc_9_2EE9
        je     NEAR PTR loc_9_1F7F
        or     al, al
        jne    NEAR PTR loc_9_1EE5
        mov    edx, edi
        movzx  edi, word ptr [ebp + 38h]
        shl    edi, 4
        movzx  ecx, word ptr [ebp]
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        movzx  esi, word ptr [ebp + 3ch]
        shl    esi, 4
        movzx  ecx, word ptr [ebp + 4]
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        movzx  ecx, word ptr [esi]
        lea    esi, [esi + 2]
        test   ch, 0ffh
        jne    SHORT loc_9_1E00
        cmp    byte ptr [edx + 68fh], 0
        je     SHORT loc_9_1E00
        cmp    cl, byte ptr [edx + 68fh]
        ja     SHORT loc_9_1E00
        bts    word ptr [edx + 68eh], 5
        jb     NEAR PTR loc_9_2BF4
        mov    eax, 57333800h
        cld
        stosd
        mov    al, cl
        stosb
loc_9_1E95:
        push   edx
        push   ecx
        cld
        lodsw
        push   esi
        movzx  eax, ax
        lea    ecx, [edx + 4]
        test   eax, 3ffh
        stc
        jne    SHORT loc_9_1EAE
        shr    eax, 7
        add    eax, ecx
loc_9_1EAE:
        jb     NEAR PTR loc_9_1FB6
        test   byte ptr [eax + 2], 1
        jne    NEAR PTR loc_9_1FB6
        mov    edx, eax
        sub    edx, ecx
        shr    edx, 3
        mov    byte ptr [eax + 3], dl
        mov    esi, eax
        mov    ecx, 6
        cld
        rep movsb
        pop    esi
        pop    ecx
        pop    edx
        loop   loc_9_1E95
        btr    word ptr [edx + 68eh], 5
        jmp    NEAR PTR loc_9_14DC
loc_9_1EE5:
        sub    esp, 200h
        mov    edx, edi
        lea    esi, [edi + 4]
        mov    edi, esp
        mov    ecx, 80h
        cld
        rep movsd
        movzx  esi, word ptr [ebp + 3ch]
        shl    esi, 4
        movzx  ecx, word ptr [ebp + 4]
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        cld
        lodsd
        cmp    eax, 57333800h
        jne    NEAR PTR loc_9_1E06
        lodsb
        movzx  ecx, al
        cmp    byte ptr [edx + 68fh], 0
        je     NEAR PTR loc_9_1E06
        cmp    cl, byte ptr [edx + 68fh]
        ja     NEAR PTR loc_9_1E06
        mov    eax, esp
loc_9_1F36:
        push   ecx
        movzx  ecx, byte ptr [esi + 3]
        cmp    cl, 40h
        jae    NEAR PTR loc_9_1E05
        mov    edi, ecx
        lea    edi, [eax + edi*8]
        test   byte ptr [edi + 2], 1
        jne    NEAR PTR loc_9_1E05
        mov    ecx, 6
        cld
        rep movsb
        pop    ecx
        loop   loc_9_1F36
        mov    edi, edx
        mov    eax, esp
        mov    ecx, 1
        db  0E8h, 008h, 0E1h, 0F6h, 0FFh
        lahf
        add    esp, 200h
        sahf
        jb     NEAR PTR loc_9_2BF4
        jmp    NEAR PTR loc_9_14DC
loc_9_1F7F:
        movzx  eax, word ptr [ebp + 10h]
        test   ah, 0ffh
        jne    SHORT loc_9_1FC2
        cmp    byte ptr [edi + 68fh], 0
        je     SHORT loc_9_1FC2
        cmp    al, byte ptr [edi + 68fh]
        ja     SHORT loc_9_1FC2
        mov    cl, 6
        mul    cl
        movzx  eax, ax
        add    eax, 5
        cmp    eax, 0ffh
        ja     NEAR PTR loc_9_2BF4
        mov    byte ptr [ebp + 1ch], al
        jmp    NEAR PTR loc_9_14DC
loc_9_1FB6:
        pop    esi
        pop    ecx
        pop    edx
        btr    word ptr [edx + 68eh], 5
loc_9_1FC2:
        mov    byte ptr [ebp + 1dh], 8bh
        ret
loc_9_1FC7:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_1BB8
        call   sub_9_0FE2
        jb     NEAR PTR loc_9_2BF4
        cmp    byte ptr [ebp + 1ch], 1
        ja     NEAR PTR loc_9_2EE9
        movzx  edx, word ptr [ebp + 14h]
        cmp    edx, 0ffh
        jae    NEAR PTR loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 280h]
        or     edx, edx
        je     NEAR PTR loc_9_1BB8
        movzx  esi, word ptr [ebp + 3ch]
        shl    esi, 4
        movzx  ecx, word ptr [ebp + 4]
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        movzx  ecx, word ptr [ebp + 18h]
        movzx  eax, byte ptr [ebp + 1ch]
        call   sub_9_0EBC
        jb     NEAR PTR loc_9_19C9
        jmp    NEAR PTR loc_9_14DC
loc_9_202E:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_1BB8
        movzx  edx, word ptr [ebp + 14h]
        cmp    edx, 0ffh
        jae    NEAR PTR loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 280h]
        or     edx, edx
        je     NEAR PTR loc_9_1BB8
        test   byte ptr [edx + 2ch], 80h
        jne    NEAR PTR loc_9_1BB8
        test   byte ptr [edx + 2ch], 20h
        jne    NEAR PTR loc_9_1BB8
        db  083h, 03Dh, 008h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_9_187F
        test   byte ptr [esi + ebx + 2ch], 2
        jne    NEAR PTR loc_9_187F
        movzx  ecx, word ptr [ebp + 10h]
        shl    ecx, 2
        test   byte ptr [edx + 2ch], 40h
        je     SHORT loc_9_2097
        db  02Bh, 00Dh, 0A4h, 000h, 000h, 000h
loc_9_2097:
        cmp    ecx, 10000h
        ja     NEAR PTR loc_9_23DD
        cmp    ecx, dword ptr [edx + 28h]
        je     NEAR PTR loc_9_14DC
        jb     NEAR PTR loc_9_230C
        test   byte ptr [edx + 2ch], 4
        jne    NEAR PTR loc_9_21C5
        test   byte ptr [edx + 2ch], 40h
        je     SHORT loc_9_20D1
        mov    eax, dword ptr [edx + 28h]
        and    eax, 3
        je     SHORT loc_9_20D1
        sub    eax, 4
        neg    eax
        add    ecx, eax
loc_9_20D1:
        push   ecx
        push   edx
        call   sub_9_15AF
        mov    eax, ecx
        shl    eax, 2
        pop    edx
        pop    ecx
        cmp    ecx, eax
        ja     NEAR PTR loc_9_23DD
        mov    eax, ecx
        sub    eax, dword ptr [edx + 28h]
        push   ecx
        push   eax
        db  003h, 005h, 00Ch, 000h, 000h, 000h
        mov    ecx, dword ptr [edi + 280h]
        jecxz  loc_9_20FF
        mov    ecx, dword ptr [ecx + 28h]
loc_9_20FF:
        sub    eax, ecx
        jae    SHORT loc_9_2105
        xor    eax, eax
loc_9_2105:
        db  03Bh, 005h, 008h, 000h, 000h, 000h
        ja     NEAR PTR loc_9_247F
        pop    eax
        push   eax
        add    eax, dword ptr [edi + 67ch]
        sub    eax, ecx
        jae    SHORT loc_9_211F
        xor    eax, eax
loc_9_211F:
        cmp    eax, dword ptr [esi + ebx + 54h]
        ja     NEAR PTR loc_9_247F
        mov    ecx, dword ptr [esp + 4]
        push   edx
        xor    eax, eax
        test   byte ptr [esi + ebx + 2ch], 20h
        je     SHORT loc_9_213C
        or     eax, 80h
loc_9_213C:
        push   eax
        push   ecx
        push   dword ptr [edx]
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        mov    esi, edx
        pop    edx
        or     eax, eax
        je     NEAR PTR loc_9_23FB
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], esi
        and    byte ptr [edx + 2ch], 0fbh
        pop    ecx
        pop    eax
        mov    dword ptr [edx + 28h], eax
        push   edx
        push   ecx
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        mov    eax, dword ptr [edx + ebx + 48h]
        or     eax, eax
        je     SHORT loc_9_21A7
        sub    eax, ecx
        jbe    SHORT loc_9_2192
        mov    dword ptr [edx + ebx + 48h], eax
        push   edx
        push   0
        push   eax
        push   dword ptr [edx + ebx + 44h]
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        mov    dword ptr [edx + ebx + 44h], eax
        jmp    SHORT loc_9_21A7
loc_9_2192:
        xor    eax, eax
        mov    dword ptr [edx + ebx + 48h], eax
        xchg   dword ptr [edx + ebx + 44h], eax
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_9_21A7:
        pop    ecx
        pop    edx
loc_9_21A9:
        call   sub_9_0A13
        jb     NEAR PTR loc_9_2BF4
        add    dword ptr [edi + 67ch], ecx
        db  001h, 00Dh, 00Ch, 000h, 000h, 000h
        jmp    NEAR PTR loc_9_14DC
loc_9_21C5:
        push   ecx
        push   edx
        call   sub_9_15AF
        mov    eax, ecx
        shl    eax, 2
        pop    edx
        pop    ecx
        cmp    ecx, eax
        ja     NEAR PTR loc_9_23DD
        mov    eax, ecx
        push   ecx
        db  003h, 005h, 00Ch, 000h, 000h, 000h
        mov    ecx, dword ptr [edi + 280h]
        jecxz  loc_9_21EF
        mov    ecx, dword ptr [ecx + 28h]
loc_9_21EF:
        sub    eax, ecx
        jae    SHORT loc_9_21F5
        xor    eax, eax
loc_9_21F5:
        pop    ecx
        db  03Bh, 005h, 008h, 000h, 000h, 000h
        ja     NEAR PTR loc_9_2482
        mov    eax, ecx
        push   ecx
        add    eax, dword ptr [edi + 67ch]
        mov    ecx, dword ptr [edi + 280h]
        jecxz  loc_9_2216
        mov    ecx, dword ptr [ecx + 28h]
loc_9_2216:
        sub    eax, ecx
        jae    SHORT loc_9_221C
        xor    eax, eax
loc_9_221C:
        pop    ecx
        cmp    eax, dword ptr [esi + ebx + 54h]
        ja     NEAR PTR loc_9_2482
        push   ecx
        push   edx
        xor    eax, eax
        test   byte ptr [esi + ebx + 2ch], 20h
        je     SHORT loc_9_2237
        or     eax, 80h
loc_9_2237:
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        mov    esi, edx
        pop    edx
        pop    ecx
        or     eax, eax
        je     SHORT loc_9_2272
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], esi
        and    byte ptr [edx + 2ch], 0fbh
        mov    dword ptr [edx + 28h], ecx
        add    dword ptr [edi + 67ch], ecx
        db  001h, 00Dh, 00Ch, 000h, 000h, 000h
        jmp    NEAR PTR loc_9_17F9
loc_9_2272:
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        mov    eax, dword ptr [esi + ebx + 48h]
        or     eax, eax
        je     NEAR PTR loc_9_2482
        sub    eax, ecx
        jb     NEAR PTR loc_9_2482
        push   edx
        push   ecx
        je     SHORT loc_9_22AA
        mov    dword ptr [esi + ebx + 48h], eax
        push   0
        push   eax
        push   dword ptr [esi + ebx + 44h]
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        mov    dword ptr [esi + ebx + 44h], eax
        jmp    SHORT loc_9_22BD
loc_9_22AA:
        mov    dword ptr [esi + ebx + 48h], eax
        xchg   dword ptr [esi + ebx + 44h], eax
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_9_22BD:
        pop    ecx
        push   ecx
        xor    eax, eax
        test   byte ptr [esi + ebx + 2ch], 20h
        je     SHORT loc_9_22CD
        or     eax, 80h
loc_9_22CD:
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        mov    esi, edx
        pop    ecx
        pop    edx
        or     eax, eax
        je     NEAR PTR loc_9_2482
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], esi
        and    byte ptr [edx + 2ch], 0fbh
        mov    dword ptr [edx + 28h], ecx
        add    dword ptr [edi + 67ch], ecx
        db  001h, 00Dh, 00Ch, 000h, 000h, 000h
        jmp    NEAR PTR loc_9_14DC
loc_9_230C:
        jecxz  loc_9_233B
        mov    eax, dword ptr [edx + 28h]
        sub    eax, ecx
        push   eax
        push   ecx
        push   edx
        push   0
        push   ecx
        push   dword ptr [edx]
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        mov    esi, edx
        pop    edx
        or     eax, eax
        je     NEAR PTR loc_9_23DA
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], esi
        pop    eax
        pop    ecx
        mov    dword ptr [edx + 28h], eax
        jmp    SHORT loc_9_2352
loc_9_233B:
        push   edx
        push   0
        push   dword ptr [edx]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
        xor    ecx, ecx
        xchg   dword ptr [edx + 28h], ecx
        or     byte ptr [edx + 2ch], 4
loc_9_2352:
        sub    dword ptr [edi + 67ch], ecx
        db  029h, 00Dh, 00Ch, 000h, 000h, 000h
        push   edx
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        mov    eax, dword ptr [edx + ebx + 4ch]
        sub    eax, dword ptr [edx + ebx + 48h]
        jbe    SHORT loc_9_23C9
        cmp    eax, ecx
        jae    SHORT loc_9_2375
        mov    ecx, eax
loc_9_2375:
        mov    eax, dword ptr [edx + ebx + 44h]
        add    ecx, dword ptr [edx + ebx + 48h]
        push   ecx
        test   byte ptr [edx + ebx + 2ch], 20h
        je     SHORT loc_9_238C
        mov    edx, 80h
        jmp    SHORT loc_9_238E
loc_9_238C:
        xor    edx, edx
loc_9_238E:
        or     eax, eax
        je     SHORT loc_9_23A0
        push   edx
        push   ecx
        push   eax
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        jmp    SHORT loc_9_23B6
loc_9_23A0:
        push   edx
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
loc_9_23B6:
        pop    ecx
        or     eax, eax
        je     SHORT loc_9_23C9
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        mov    dword ptr [edx + ebx + 44h], eax
        mov    dword ptr [edx + ebx + 48h], ecx
loc_9_23C9:
        pop    edx
        call   sub_9_0A13
        jb     NEAR PTR loc_9_2BF4
        jmp    NEAR PTR loc_9_14DC
loc_9_23DA:
        add    esp, 8
loc_9_23DD:
        mov    eax, dword ptr [edx + 28h]
        test   byte ptr [edx + 2ch], 40h
        je     SHORT loc_9_23EC
        db  003h, 005h, 0A4h, 000h, 000h, 000h
loc_9_23EC:
        add    eax, 3
        shr    eax, 2
        mov    word ptr [ebp + 10h], ax
        jmp    NEAR PTR loc_9_187F
loc_9_23FB:
        pop    eax
        pop    ecx
        push   edx
        push   ecx
        push   eax
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        mov    ecx, dword ptr [esi + ebx + 48h]
        or     ecx, ecx
        je     SHORT loc_9_247C
        sub    ecx, eax
        jb     SHORT loc_9_247C
        je     SHORT loc_9_242E
        mov    dword ptr [esi + ebx + 48h], ecx
        push   0
        push   ecx
        push   dword ptr [esi + ebx + 44h]
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        mov    dword ptr [esi + ebx + 44h], eax
        jmp    SHORT loc_9_2441
loc_9_242E:
        mov    dword ptr [esi + ebx + 48h], ecx
        xchg   dword ptr [esi + ebx + 44h], ecx
        push   0
        push   ecx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_9_2441:
        pop    eax
        pop    ecx
        pop    edx
        push   ecx
        push   eax
        push   edx
        xor    eax, eax
        test   byte ptr [esi + ebx + 2ch], 20h
        je     SHORT loc_9_2455
        or     eax, 80h
loc_9_2455:
        push   eax
        push   ecx
        push   dword ptr [edx]
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_9_247C
        mov    esi, edx
        pop    edx
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], esi
        and    byte ptr [edx + 2ch], 0fbh
        pop    ecx
        pop    eax
        mov    dword ptr [edx + 28h], eax
        jmp    NEAR PTR loc_9_21A9
loc_9_247C:
        add    esp, 4
loc_9_247F:
        add    esp, 8
loc_9_2482:
        mov    eax, dword ptr [edx + 28h]
        test   byte ptr [edx + 2ch], 40h
        je     SHORT loc_9_2491
        db  003h, 005h, 0A4h, 000h, 000h, 000h
loc_9_2491:
        add    eax, 3
        shr    eax, 2
        mov    word ptr [ebp + 10h], ax
        jmp    NEAR PTR loc_9_1934
loc_9_24A0:
        movzx  eax, byte ptr [ebp + 1ch]
        cmp    al, 1
        jbe    SHORT loc_9_24B9
        cmp    al, 2
        jne    NEAR PTR loc_9_2EE9
        mov    byte ptr [ebp + 1ch], 0
        jmp    NEAR PTR loc_9_14DC
loc_9_24B9:
        mov    byte ptr [ebp + 1dh], 91h
        ret
loc_9_24BE:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_1BB8
        movzx  edx, word ptr [ebp + 14h]
        cmp    edx, 0ffh
        jae    NEAR PTR loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 280h]
        or     edx, edx
        je     NEAR PTR loc_9_1BB8
        movzx  eax, byte ptr [ebp + 1ch]
        cmp    al, 1
        ja     NEAR PTR loc_9_2EE9
        or     al, al
        jne    SHORT loc_9_2518
        lea    esi, [edx + 2eh]
        movzx  edi, word ptr [ebp + 38h]
        shl    edi, 4
        movzx  ecx, word ptr [ebp]
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        mov    ecx, 8
        cld
        rep movsb
        jmp    NEAR PTR loc_9_14DC
loc_9_2518:
        test   byte ptr [edx + 2ch], 80h
        jne    NEAR PTR loc_9_1BB8
        movzx  esi, word ptr [ebp + 3ch]
        shl    esi, 4
        movzx  ecx, word ptr [ebp + 4]
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        mov    ecx, 8
        push   edi
        mov    edi, esi
        xor    al, al
        cld
        repe scasb
        pop    edi
        jne    SHORT loc_9_2548
        and    byte ptr [edx + 2ch], 0feh
        jmp    SHORT loc_9_2580
loc_9_2548:
        mov    ecx, 0ffh
        xor    eax, eax
loc_9_254F:
        push   eax
        mov    eax, dword ptr [edi + eax*4 + 280h]
        or     eax, eax
        je     SHORT loc_9_2578
        test   byte ptr [eax + 2ch], 1
        je     SHORT loc_9_2578
        cmp    eax, edx
        je     SHORT loc_9_2578
        push   esi
        push   edi
        push   ecx
        lea    edi, [eax + 2eh]
        mov    ecx, 8
        cld
        repe cmpsb
        pop    ecx
        pop    edi
        pop    esi
        je     SHORT loc_9_2590
loc_9_2578:
        pop    eax
        inc    eax
        loop   loc_9_254F
        or     byte ptr [edx + 2ch], 1
loc_9_2580:
        lea    edi, [edx + 2eh]
        mov    ecx, 8
        cld
        rep movsb
        jmp    NEAR PTR loc_9_14DC
loc_9_2590:
        pop    eax
        mov    byte ptr [ebp + 1dh], 0a1h
        ret
loc_9_2596:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_2BF4
        movzx  eax, byte ptr [ebp + 1ch]
        cmp    al, 2
        ja     NEAR PTR loc_9_2EE9
        je     NEAR PTR loc_9_2650
        or     al, al
        je     SHORT loc_9_2602
        lea    esi, [edi + 280h]
        movzx  edi, word ptr [ebp + 3ch]
        shl    edi, 4
        movzx  ecx, word ptr [ebp + 4]
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        mov    ecx, 0ffh
loc_9_25D2:
        cld
        lodsd
        or     eax, eax
        je     SHORT loc_9_25FB
        push   esi
        push   edi
        push   ecx
        lea    esi, [eax + 2eh]
        mov    ecx, 8
        cld
        repe cmpsb
        jne    SHORT loc_9_25F8
        movzx  edx, byte ptr [eax + 2dh]
        mov    word ptr [ebp + 14h], dx
        add    esp, 0ch
        jmp    NEAR PTR loc_9_14DC
loc_9_25F8:
        pop    ecx
        pop    edi
        pop    esi
loc_9_25FB:
        loop   loc_9_25D2
        mov    byte ptr [ebp + 1dh], 0a0h
        ret
loc_9_2602:
        mov    edx, edi
        movzx  edi, word ptr [ebp + 38h]
        shl    edi, 4
        movzx  ecx, word ptr [ebp]
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        lea    esi, [edx + 280h]
        xor    edx, edx
        mov    ecx, 0ffh
loc_9_2621:
        cld
        lodsd
        or     eax, eax
        je     SHORT loc_9_2646
        push   esi
        push   edi
        push   ecx
        movzx  ecx, byte ptr [eax + 2dh]
        mov    word ptr [edi], cx
        lea    esi, [eax + 2eh]
        lea    edi, [edi + 2]
        mov    ecx, 8
        cld
        rep movsb
        pop    ecx
        pop    edi
        pop    esi
        add    edi, 0ah
        inc    edx
loc_9_2646:
        loop   loc_9_2621
        mov    byte ptr [ebp + 1ch], dl
        jmp    NEAR PTR loc_9_14DC
loc_9_2650:
        mov    word ptr [ebp + 10h], 0ffh
        jmp    NEAR PTR loc_9_14DC
loc_9_265B:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_1BB8
        call   sub_9_0FE2
        jb     NEAR PTR loc_9_2BF4
        cmp    byte ptr [ebp + 1ch], 1
        ja     NEAR PTR loc_9_2EE9
        movzx  edx, word ptr [ebp + 14h]
        cmp    edx, 0ffh
        jae    NEAR PTR loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 280h]
        or     edx, edx
        je     NEAR PTR loc_9_1BB8
        movzx  esi, word ptr [ebp + 3ch]
        shl    esi, 4
        movzx  ecx, word ptr [ebp + 4]
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        push   esi
        movzx  eax, word ptr [esi + 7]
        shl    eax, 4
        movzx  ecx, word ptr [esi + 5]
        add    eax, ecx
        movzx  ecx, byte ptr [esi + 4]
        mov    esi, eax
        movzx  eax, byte ptr [ebp + 1ch]
        call   sub_9_0EBC
        pop    esi
        jb     NEAR PTR loc_9_19C9
        mov    ecx, dword ptr [esi]
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 049h, 000h, 001h, 000h
        jmp    NEAR PTR loc_9_14DC
loc_9_26E1:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_1BB8
        call   sub_9_0FE2
        jb     NEAR PTR loc_9_2BF4
        cmp    byte ptr [ebp + 1ch], 2
        ja     NEAR PTR loc_9_2EE9
        je     NEAR PTR loc_9_2788
        movzx  edx, word ptr [ebp + 14h]
        cmp    edx, 0ffh
        jae    NEAR PTR loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 280h]
        or     edx, edx
        je     NEAR PTR loc_9_1BB8
        movzx  esi, word ptr [ebp + 3ch]
        shl    esi, 4
        movzx  ecx, word ptr [ebp + 4]
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        push   esi
        movzx  eax, word ptr [esi + 7]
        shl    eax, 4
        movzx  ecx, word ptr [esi + 5]
        add    eax, ecx
        add    eax, dword ptr [ebx + 4]
        movzx  ecx, byte ptr [esi + 4]
        mov    esi, eax
        movzx  eax, byte ptr [ebp + 1ch]
        call   sub_9_0EBC
        pop    esi
        jb     NEAR PTR loc_9_19C9
        mov    eax, esi
        db  0E8h, 095h, 0DEh, 0F6h, 0FFh
        db  0A1h, 014h, 000h, 000h, 000h
        db  0E8h, 08Bh, 0DEh, 0F6h, 0FFh
        mov    eax, dword ptr [esi]
        db  0E8h, 084h, 0DEh, 0F6h, 0FFh
        mov    al, byte ptr [ebp + 1ch]
        mov    byte ptr [esi + 10h], al
        movzx  eax, word ptr [ebp + 14h]
        mov    word ptr [esi + 0eh], ax
        jmp    NEAR PTR loc_9_14DC
loc_9_2788:
        mov    word ptr [ebp + 10h], 0ch
        jmp    NEAR PTR loc_9_14DC
loc_9_2793:
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        db  08Bh, 00Dh, 0FCh, 011h, 000h, 000h
        db  0FEh, 005h, 0EAh, 002h, 000h, 000h
        inc    byte ptr [ecx + ebx + 27h]
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    edx, eax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        shl    eax, 10h
        mov    ax, dx
        mov    esi, eax
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        db  08Bh, 00Dh, 0FCh, 011h, 000h, 000h
        mov    edi, dword ptr [ecx + ebx + 5ch]
        or     edi, edi
        je     NEAR PTR loc_9_2BF4
        movzx  edx, word ptr [esi + 0eh]
        cmp    edx, 0ffh
        jae    NEAR PTR loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 280h]
        or     edx, edx
        je     NEAR PTR loc_9_1BB8
        cmp    byte ptr [esi + 10h], 1
        ja     NEAR PTR loc_9_2EE9
        movzx  eax, word ptr [esi + 0ch]
        shl    eax, 4
        movzx  ecx, word ptr [esi + 0ah]
        add    eax, ecx
        add    eax, dword ptr [ebx + 4]
        push   eax
        movzx  ecx, byte ptr [esi + 9]
        movzx  eax, byte ptr [esi + 10h]
        pop    esi
        call   sub_9_0EBC
        pushfd
        db  08Bh, 00Dh, 0FCh, 011h, 000h, 000h
        cmp    byte ptr [ecx + ebx + 27h], 0
        je     SHORT loc_9_2836
        dec    byte ptr [ecx + ebx + 27h]
loc_9_2836:
        db  080h, 03Dh, 0EAh, 002h, 000h, 000h, 000h
        je     SHORT loc_9_2845
        db  0FEh, 00Dh, 0EAh, 002h, 000h, 000h
loc_9_2845:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        popfd
        jb     NEAR PTR loc_9_19C9
        jmp    NEAR PTR loc_9_14DC
loc_9_2857:
        cmp    byte ptr [ebp + 1ch], 1
        ja     NEAR PTR loc_9_2EE9
        movzx  edx, word ptr [ebp + 3ch]
        shl    edx, 4
        movzx  ecx, word ptr [ebp + 4]
        add    edx, ecx
        add    edx, dword ptr [ebx + 4]
        mov    ecx, dword ptr [edx]
        or     ecx, ecx
        je     NEAR PTR loc_9_14DC
        cmp    ecx, 100000h
        ja     NEAR PTR loc_9_2A2B
        xor    ecx, ecx
        cmp    byte ptr [edx + 4], 1
        ja     NEAR PTR loc_9_2A26
        jne    SHORT loc_9_28D3
        or     cl, 20h
        push   ecx
        push   edx
        call   sub_9_00A4
        pop    edx
        pop    ecx
        jb     NEAR PTR loc_9_1BB8
        push   edx
        movzx  edx, word ptr [edx + 5]
        or     edx, edx
        je     SHORT loc_9_28CB
        cmp    edx, 0ffh
        jae    SHORT loc_9_28CB
        mov    edx, dword ptr [edi + edx*4 + 280h]
        or     edx, edx
        je     SHORT loc_9_28CB
        bt     word ptr [edx + 2ch], 7
        jmp    SHORT loc_9_28CC
loc_9_28CB:
        stc
loc_9_28CC:
        pop    edx
        jb     NEAR PTR loc_9_1BB8
loc_9_28D3:
        cmp    byte ptr [edx + 0bh], 1
        ja     NEAR PTR loc_9_2A26
        jne    SHORT loc_9_291D
        or     cl, 10h
        push   edx
        push   ecx
        call   sub_9_00A4
        pop    ecx
        pop    edx
        jb     NEAR PTR loc_9_1BB8
        push   edx
        movzx  edx, word ptr [edx + 0ch]
        or     edx, edx
        je     SHORT loc_9_2915
        cmp    edx, 0ffh
        jae    SHORT loc_9_2915
        mov    edx, dword ptr [edi + edx*4 + 280h]
        or     edx, edx
        je     SHORT loc_9_2915
        bt     word ptr [edx + 2ch], 7
        jmp    SHORT loc_9_2916
loc_9_2915:
        stc
loc_9_2916:
        pop    edx
        jb     NEAR PTR loc_9_1BB8
loc_9_291D:
        test   cl, 30h
        je     NEAR PTR loc_9_2A26
        mov    esi, edx
        mov    al, byte ptr [esi + 4]
        and    al, byte ptr [esi + 0bh]
        je     SHORT loc_9_2976
        movzx  eax, word ptr [esi + 5]
        cmp    ax, word ptr [esi + 0ch]
        jne    SHORT loc_9_2976
        movzx  eax, word ptr [esi + 9]
        movzx  edx, word ptr [esi + 7]
        shl    eax, 0eh
        add    eax, edx
        push   eax
        movzx  edx, word ptr [esi + 10h]
        movzx  eax, word ptr [esi + 0eh]
        shl    edx, 0eh
        add    edx, eax
        pop    eax
        sub    edx, eax
        je     NEAR PTR loc_9_14DC
        jg     SHORT loc_9_2965
        or     cl, 80h
        neg    edx
loc_9_2965:
        cmp    edx, dword ptr [esi]
        jae    SHORT loc_9_2976
        cmp    byte ptr [ebp + 1ch], 1
        je     NEAR PTR loc_9_2A21
        or     cl, 40h
loc_9_2976:
        push   ecx
        mov    ecx, dword ptr [esi]
        lea    eax, [esi + 4]
        call   sub_9_2A30
        or     ah, ah
        jne    NEAR PTR loc_9_2A1B
        push   edx
        lea    eax, [esi + 0bh]
        call   sub_9_2A30
        pop    esi
        or     ah, ah
        jne    NEAR PTR loc_9_2A1B
        mov    edi, edx
        cmp    byte ptr [ebp + 1ch], 1
        je     SHORT loc_9_29EA
        test   byte ptr [esp], 40h
        je     SHORT loc_9_29D5
        mov    byte ptr [ebp + 1dh], 92h
        test   byte ptr [esp], 80h
        jne    SHORT loc_9_29D9
        add    esi, ecx
        sub    esi, 4
        add    edi, ecx
        sub    edi, 4
        std
        mov    eax, ecx
        shr    ecx, 2
        rep movsd
        add    esi, 3
        add    edi, 3
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cld
        jmp    SHORT loc_9_29E8
loc_9_29D5:
        mov    byte ptr [ebp + 1dh], 0
loc_9_29D9:
        cld
        mov    eax, ecx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_9_29E8:
        pop    eax
        ret
loc_9_29EA:
        push   ecx
        shr    ecx, 2
        jecxz  loc_9_2A03
        mov    edx, 4
loc_9_29F5:
        mov    eax, dword ptr [esi]
        mov    ebx, dword ptr [edi]
        mov    dword ptr [edi], eax
        mov    dword ptr [esi], ebx
        add    esi, edx
        add    edi, edx
        loop   loc_9_29F5
loc_9_2A03:
        pop    ecx
        and    ecx, 3
        jecxz  loc_9_2A15
loc_9_2A09:
        mov    al, byte ptr [esi]
        mov    bl, byte ptr [edi]
        mov    byte ptr [edi], al
        mov    byte ptr [esi], bl
        inc    esi
        inc    edi
        loop   loc_9_2A09
loc_9_2A15:
        pop    eax
        jmp    NEAR PTR loc_9_14DC
loc_9_2A1B:
        pop    ecx
        jmp    NEAR PTR loc_9_19C9
loc_9_2A21:
        mov    byte ptr [ebp + 1dh], 97h
        ret
loc_9_2A26:
        mov    byte ptr [ebp + 1dh], 98h
        ret
loc_9_2A2B:
        mov    byte ptr [ebp + 1dh], 96h
        ret
sub_9_2A30:
        push   ecx
        mov    dl, byte ptr [eax]
        or     dl, dl
        je     SHORT loc_9_2A9A
        movzx  edx, word ptr [eax + 1]
        mov    edx, dword ptr [edi + edx*4 + 280h]
        cmp    word ptr [eax + 3], 4000h
        jae    NEAR PTR loc_9_2AE0
        test   byte ptr [edx + 2ch], 4
        jne    NEAR PTR loc_9_2AE4
        mov    ecx, dword ptr [edx + 28h]
        shr    ecx, 2
        cmp    ecx, 0ffffh
        ja     SHORT loc_9_2A6C
        cmp    cx, word ptr [eax + 5]
        jbe    SHORT loc_9_2AE4
loc_9_2A6C:
        push   eax
        movzx  eax, word ptr [eax + 5]
        sub    ecx, eax
        pop    eax
        shl    ecx, 0eh
        push   eax
        movzx  eax, word ptr [eax + 3]
        sub    ecx, eax
        pop    eax
        cmp    ecx, dword ptr [esp]
        jb     SHORT loc_9_2AE4
        movzx  ecx, word ptr [eax + 5]
        shl    ecx, 0eh
        push   eax
        movzx  eax, word ptr [eax + 3]
        add    ecx, eax
        pop    eax
        add    ecx, dword ptr [edx + 4]
        mov    edx, ecx
        jmp    SHORT loc_9_2ADC
loc_9_2A9A:
        movzx  edx, word ptr [eax + 5]
        shl    edx, 4
        movzx  ecx, word ptr [eax + 3]
        add    edx, ecx
        mov    ecx, edx
        add    ecx, dword ptr [esp]
        cmp    ecx, 100000h
        jae    SHORT loc_9_2AEC
        cmp    word ptr [edi + 68ah], 0
        je     SHORT loc_9_2AD9
        movzx  eax, word ptr [edi + 68ah]
        shl    eax, 4
        cmp    ecx, eax
        jbe    SHORT loc_9_2AD9
        cmp    edx, eax
        jbe    SHORT loc_9_2AE8
        add    eax, 10000h
        cmp    edx, eax
        jb     SHORT loc_9_2AE8
loc_9_2AD9:
        add    edx, dword ptr [ebx + 4]
loc_9_2ADC:
        pop    ecx
        mov    ah, 0
        ret
loc_9_2AE0:
        pop    ecx
        mov    ah, 95h
        ret
loc_9_2AE4:
        pop    ecx
        mov    ah, 93h
        ret
loc_9_2AE8:
        pop    ecx
        mov    ah, 94h
        ret
loc_9_2AEC:
        pop    ecx
        mov    ah, 0a2h
        ret
loc_9_2AF0:
        call   sub_9_00A4
        mov    ecx, 0
        jb     NEAR PTR loc_9_2B93
        call   sub_9_0FE2
        jb     NEAR PTR loc_9_2BF4
        mov    al, byte ptr [ebp + 1ch]
        cmp    al, 1
        ja     NEAR PTR loc_9_2EE9
        or     al, al
        jne    SHORT loc_9_2B7D
        movzx  edi, word ptr [ebp + 38h]
        shl    edi, 4
        movzx  ecx, word ptr [ebp]
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        push   edi
        lea    eax, [edx + 240h]
        lea    edx, [edx + 4]
        mov    esi, edx
        mov    ecx, 40h
loc_9_2B3B:
        test   byte ptr [edx + 2], 1
        jne    SHORT loc_9_2B6E
        push   ecx
        push   edi
        push   eax
        mov    eax, edx
        sub    eax, esi
        shl    eax, 7
        mov    word ptr [edi], ax
        mov    eax, edx
        sub    eax, esi
        shr    eax, 3
        pop    edi
        push   edi
        mov    ecx, 40h
        cld
        repne scasb
        dec    edi
        mov    ecx, edi
        pop    eax
        sub    ecx, eax
        pop    edi
        mov    word ptr [edi + 2], cx
        pop    ecx
        add    edi, 4
loc_9_2B6E:
        add    edx, 8
        loop   loc_9_2B3B
        pop    edx
        mov    eax, edi
        sub    eax, edx
        shr    eax, 2
        jmp    SHORT loc_9_2B93
loc_9_2B7D:
        lea    edi, [edi + 4]
        mov    ecx, 40h
        xor    eax, eax
loc_9_2B87:
        test   byte ptr [edi + 2], 1
        jne    SHORT loc_9_2B8E
        inc    eax
loc_9_2B8E:
        add    edi, 8
        loop   loc_9_2B87
loc_9_2B93:
        mov    word ptr [ebp + 18h], ax
        jmp    NEAR PTR loc_9_14DC
loc_9_2B9C:
        call   sub_9_00A4
        jb     SHORT loc_9_2BF4
        movzx  edi, word ptr [ebp + 38h]
        shl    edi, 4
        movzx  ecx, word ptr [ebp]
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        cmp    byte ptr [edx + 691h], 2
        jae    SHORT loc_9_2BEF
        mov    al, byte ptr [ebp + 1ch]
        or     al, al
        jne    SHORT loc_9_2BE2
        db  0BEh, 0E8h, 00Eh, 000h, 000h
        mov    ecx, 0ah
        push   edi
        cld
        rep movsb
        pop    edi
        movzx  eax, word ptr [edx + 688h]
        mov    word ptr [edi + 4], ax
        jmp    NEAR PTR loc_9_14DC
loc_9_2BE2:
        cmp    al, 1
        je     NEAR PTR loc_9_1575
        jmp    NEAR PTR loc_9_2EE9
loc_9_2BEF:
        mov    byte ptr [ebp + 1dh], 0a4h
        ret
loc_9_2BF4:
        mov    byte ptr [ebp + 1dh], 80h
        ret
loc_9_2BF9:
        call   sub_9_00A4
        jb     SHORT loc_9_2BEF
        call   sub_9_0FE2
        jb     SHORT loc_9_2BF4
        cmp    byte ptr [edi + 691h], 2
        jae    SHORT loc_9_2BEF
        movzx  eax, byte ptr [ebp + 1ch]
        cmp    al, 8
        ja     NEAR PTR loc_9_2EE9
        db  0FFh, 024h, 085h, 0C4h, 00Eh, 000h, 000h
loc_9_2C23:
        mov    byte ptr [ebp + 1dh], 9eh
        ret
loc_9_2C28:
        mov    al, byte ptr [edi + 690h]
        mov    byte ptr [ebp + 10h], al
        or     al, al
        jne    NEAR PTR loc_9_14DC
        mov    eax, dword ptr [edi + 684h]
        mov    word ptr [ebp], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        mov    eax, dword ptr [edi + 680h]
        or     eax, eax
        je     NEAR PTR loc_9_14DC
        bts    word ptr [edi + 68eh], 5
        jb     SHORT loc_9_2BF4
        lea    esi, [edi + 4]
        push   edi
        mov    edi, eax
        call   sub_9_0B04
        pop    edi
        btr    word ptr [edi + 68eh], 5
        jmp    NEAR PTR loc_9_14DC
loc_9_2C7D:
        movzx  eax, byte ptr [ebp + 10h]
        cmp    al, 10h
        jae    NEAR PTR loc_9_2E5F
        or     al, al
        je     SHORT loc_9_2CAA
        mov    edx, eax
        dec    edx
        cmp    dword ptr [edi + edx*4 + 204h], 0
        je     NEAR PTR loc_9_2E64
        cmp    al, byte ptr [edi + 690h]
        je     NEAR PTR loc_9_14DC
loc_9_2CAA:
        push   eax
        push   edi
        lea    esi, [edi + 4]
        cmp    byte ptr [edi + 690h], 0
        je     SHORT loc_9_2CE9
        movzx  edx, byte ptr [edi + 690h]
        dec    edx
        bts    word ptr [edi + 68eh], 5
        jb     NEAR PTR loc_9_2D57
        mov    edi, dword ptr [edi + edx*4 + 204h]
        mov    ecx, 80h
        cld
        rep movsd
        pop    edi
        btr    word ptr [edi + 68eh], 5
        push   edi
loc_9_2CE9:
        pop    edi
        pop    eax
        mov    byte ptr [edi + 690h], al
        or     al, al
        jne    SHORT loc_9_2D5F
        movzx  edx, word ptr [ebp + 38h]
        mov    word ptr [edi + 686h], dx
        shl    edx, 4
        movzx  ecx, word ptr [ebp]
        mov    word ptr [edi + 684h], cx
        add    edx, ecx
        je     SHORT loc_9_2D15
        add    edx, dword ptr [ebx + 4]
loc_9_2D15:
        mov    dword ptr [edi + 680h], edx
        or     edx, edx
        je     NEAR PTR loc_9_14DC
        mov    esi, edx
        sub    esp, 200h
        mov    edx, esp
        call   sub_9_0B8E
        jb     NEAR PTR loc_9_1E06
        mov    eax, esp
        mov    ecx, 1
        db  0E8h, 030h, 0D3h, 0F6h, 0FFh
        lahf
        add    esp, 200h
        sahf
        jb     NEAR PTR loc_9_2BF4
        jmp    NEAR PTR loc_9_14DC
loc_9_2D57:
        add    esp, 8
        jmp    NEAR PTR loc_9_2BF4
loc_9_2D5F:
        movzx  eax, byte ptr [edi + 690h]
        dec    eax
        mov    eax, dword ptr [edi + eax*4 + 204h]
        or     eax, eax
        je     NEAR PTR loc_9_2BF4
        mov    ecx, 1
        db  0E8h, 0F4h, 0D2h, 0F6h, 0FFh
        jb     NEAR PTR loc_9_2BF4
        jmp    NEAR PTR loc_9_14DC
loc_9_2D8B:
        movzx  eax, word ptr [edi + 688h]
        mov    word ptr [ebp + 14h], ax
        jmp    NEAR PTR loc_9_14DC
loc_9_2D9B:
        xor    eax, eax
        mov    ecx, 0fh
        bts    word ptr [edi + 68eh], 4
        jb     SHORT loc_9_2DC3
loc_9_2DAD:
        cmp    dword ptr [edi + eax*4 + 204h], 0
        je     SHORT loc_9_2DD3
        inc    eax
        loop   loc_9_2DAD
loc_9_2DBA:
        btr    word ptr [edi + 68eh], 4
loc_9_2DC3:
        mov    byte ptr [ebp + 1dh], 9bh
        ret
loc_9_2DC8:
        btr    word ptr [edi + 68eh], 5
        jmp    SHORT loc_9_2DBA
loc_9_2DD3:
        bts    word ptr [edi + 68eh], 5
        jb     SHORT loc_9_2DBA
        push   eax
        push   101h
        push   200h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_9_2DC8
        mov    dword ptr [edi + ecx*4 + 204h], eax
        btr    word ptr [edi + 68eh], 4
        lea    esi, [edi + 4]
        push   edi
        mov    edi, eax
        push   ecx
        mov    ecx, 80h
        cld
        rep movsd
        pop    eax
        pop    edi
        btr    word ptr [edi + 68eh], 5
        inc    eax
        mov    byte ptr [ebp + 10h], al
        jmp    NEAR PTR loc_9_14DC
loc_9_2E29:
        movzx  eax, byte ptr [ebp + 10h]
        or     al, al
        je     NEAR PTR loc_9_14DC
        cmp    al, byte ptr [edi + 690h]
        je     SHORT loc_9_2E64
        cmp    al, 10h
        jae    SHORT loc_9_2E5F
        dec    eax
        xor    edx, edx
        xchg   dword ptr [edi + eax*4 + 204h], edx
        or     edx, edx
        je     SHORT loc_9_2E64
        push   0
        push   edx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        jmp    NEAR PTR loc_9_14DC
loc_9_2E5F:
        mov    byte ptr [ebp + 1dh], 9ah
        ret
loc_9_2E64:
        mov    byte ptr [ebp + 1dh], 9dh
        ret
loc_9_2E69:
        jmp    NEAR PTR loc_9_14DC
loc_9_2E6E:
        call   sub_9_00A4
        jb     NEAR PTR loc_9_2BF4
        mov    al, byte ptr [ebp + 1ch]
        cmp    al, 2
        ja     SHORT loc_9_2EE9
        cmp    dword ptr [edx], 0
        jne    SHORT loc_9_2EA1
        cmp    al, 2
        je     NEAR PTR loc_9_2BEF
        mov    eax, 44554d42h
        mov    dword ptr [edx], eax
        mov    word ptr [ebp + 10h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 18h], ax
        jmp    SHORT loc_9_2EB4
loc_9_2EA1:
        movzx  eax, word ptr [ebp + 18h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 10h]
        cmp    eax, dword ptr [edx]
        jne    NEAR PTR loc_9_2BEF
loc_9_2EB4:
        mov    al, byte ptr [ebp + 1ch]
        or     al, al
        jne    SHORT loc_9_2EC7
        mov    byte ptr [edx + 691h], 1
        jmp    NEAR PTR loc_9_14DC
loc_9_2EC7:
        cmp    al, 1
        jne    SHORT loc_9_2ED7
        mov    byte ptr [edx + 691h], 2
        jmp    NEAR PTR loc_9_14DC
loc_9_2ED7:
        mov    byte ptr [edx + 691h], 0
        mov    dword ptr [edx], 0
        jmp    NEAR PTR loc_9_14DC
loc_9_2EE9:
        mov    byte ptr [ebp + 1dh], 8fh
        ret
        db  000h, 000h
V86MMGR_SYS_VM_INIT:
        db  08Bh, 03Dh, 0FCh, 011h, 000h, 000h
        bts    dword ptr [edi + ebx + 2ch], 3
        jb     SHORT loc_9_2F14
        lea    edi, [edi + ebx + 64h]
        db  0BEh, 028h, 00Fh, 000h, 000h
        mov    ecx, 88h
        cld
        rep movsd
        db  0E9h, 0B8h, 0D4h, 0F6h, 0FFh
loc_9_2F14:
        clc
        ret
V86MMGR_SYS_CRITICAL_EXIT:
        call   sub_9_118C
        db  0E8h, 081h, 0D2h, 0F6h, 0FFh
        clc
        ret
V86MMGR_Get_EMS_XMS_Limits:
        push   ebx
        db  003h, 01Dh, 0FCh, 011h, 000h, 000h
        mov    edx, dword ptr [ebx + 54h]
        mov    eax, dword ptr [ebx + 50h]
        mov    edi, dword ptr [ebx + 40h]
        mov    esi, dword ptr [ebx + 3ch]
        shl    edx, 2
        shl    eax, 2
        shl    edi, 2
        shl    esi, 2
        test   byte ptr [ebx + 2fh], 40h
        jne    SHORT loc_9_2F4A
        or     edi, 0ffffffffh
loc_9_2F4A:
        test   byte ptr [ebx + 2fh], 80h
        jne    SHORT loc_9_2F53
        or     edx, 0ffffffffh
loc_9_2F53:
        xor    ecx, ecx
        test   byte ptr [ebx + 2ch], 4
        jne    SHORT loc_9_2F5C
        inc    ecx
loc_9_2F5C:
        pop    ebx
        ret
V86MMGR_Get_EMS_XMS_Avail:
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        xor    ecx, ecx
        xor    edx, edx
        db  083h, 03Dh, 008h, 000h, 000h, 000h, 000h
        je     SHORT loc_9_2F8E
        test   byte ptr [esi + ebx + 2ch], 2
        jne    SHORT loc_9_2F8E
        mov    edx, dword ptr [esi + ebx + 54h]
        shr    edx, 2
        mov    ecx, edx
        mov    edi, dword ptr [esi + ebx + 5ch]
        or     edi, edi
        je     SHORT loc_9_2F8E
        call   sub_9_15AF
loc_9_2F8E:
        xchg   edx, ecx
        push   ecx
        push   edx
        mov    eax, dword ptr [esi + ebx + 40h]
        shl    eax, 2
        mov    edi, eax
        cmp    dword ptr [esi + ebx + 60h], 0
        je     SHORT loc_9_2FCD
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 14h]
        push   esi
        push   ebx
        db  0E8h, 0B2h, 0D5h, 0F6h, 0FFh
        movzx  eax, word ptr [ebp + 1ch]
        pop    ebx
        pop    esi
        mov    edi, dword ptr [esi + ebx + 40h]
        shl    edi, 2
        pop    dword ptr [ebp + 14h]
        pop    dword ptr [ebp + 10h]
        pop    dword ptr [ebp + 1ch]
        pop    ebp
loc_9_2FCD:
        pop    edx
        pop    ecx
        xor    esi, esi
        dec    esi
        ret
V86MMGR_GetPgStatus:
        push   edx
        push   ecx
        xor    eax, eax
        db  038h, 005h, 014h, 012h, 000h, 000h
        je     SHORT loc_9_3015
        cmp    ecx, 110h
        jae    SHORT loc_9_3015
        or     ebx, ebx
        je     SHORT loc_9_3004
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        test   byte ptr [edx + ebx + 2ch], 8
        je     SHORT loc_9_3015
        lea    ecx, [ebx + ecx*2 + 64h]
        db  003h, 00Dh, 0FCh, 011h, 000h, 000h
        jmp    SHORT loc_9_300B
loc_9_3004:
        db  08Dh, 00Ch, 04Dh, 028h, 00Fh, 000h, 000h
loc_9_300B:
        mov    ax, word ptr [ecx]
        and    eax, 2706h
        shr    eax, 1
loc_9_3015:
        pop    ecx
        pop    edx
        ret
sub_9_3018:
        pushad
        pushfd
        xor    al, al
        db  086h, 005h, 027h, 013h, 000h, 000h
        or     al, al
        je     SHORT loc_9_3031
        db  0A1h, 098h, 032h, 000h, 000h
        db  0CDh, 020h, 00Ah, 000h, 001h, 000h
loc_9_3031:
        db  0E8h, 06Bh, 0D1h, 0F6h, 0FFh
        popfd
        popad
        ret
        db  000h, 000h, 000h, 0E9h, 0E4h, 0D2h, 0F6h, 0FFh, 0A1h, 02Ch, 013h, 000h, 000h, 0B9h, 005h, 000h
        db  000h, 000h, 0CDh, 020h, 027h, 000h, 001h, 000h, 0E8h, 041h, 0D0h, 0F6h, 0FFh, 0A1h, 02Ch, 013h
        db  000h, 000h, 0CDh, 020h, 028h, 000h, 001h, 000h, 0E9h, 0B3h, 0D2h, 0F6h, 0FFh
loc_9_3066:
        push   2
        push   ebx
        db  0CDh, 020h, 07Dh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_9_3099
        mov    eax, dword ptr [edi]
        test   byte ptr [edi + 14h], 2
        je     SHORT loc_9_3081
        or     byte ptr [edi + 14h], 4
loc_9_3081:
        push   0
        push   0
        push   10h
        push   100h
        push   ebx
        push   eax
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        or     eax, eax
loc_9_3099:
        ret
        db  0B0h, 093h, 0E9h, 0AFh, 0CFh, 0F6h, 0FFh, 0F6h, 044h, 01Eh, 02Ch, 004h, 0B0h, 090h, 00Fh, 085h
        db  0A2h, 0CFh, 0F6h, 0FFh, 083h, 07Ch, 01Eh, 060h, 000h, 074h, 0E5h, 08Bh, 07Ch, 01Eh, 060h, 066h
        db  00Fh, 0BAh, 077h, 014h, 001h, 073h, 0D9h, 0A1h, 000h, 012h, 000h, 000h, 087h, 007h, 06Ah, 000h
        db  050h, 0CDh, 020h, 055h, 000h, 001h, 000h, 059h, 059h, 066h, 00Fh, 0BAh, 077h, 014h, 002h, 06Ah
        db  008h, 053h, 0CDh, 020h, 07Dh, 000h, 001h, 000h, 059h, 059h, 00Bh, 0C0h, 00Fh, 084h, 02Dh, 0D2h
        db  0F6h, 0FFh, 06Ah, 000h, 06Ah, 000h, 06Ah, 010h, 068h, 000h, 001h, 000h, 000h, 053h, 0FFh, 035h
        db  000h, 012h, 000h, 000h, 0CDh, 020h, 05Dh, 000h, 001h, 000h, 083h, 0C4h, 018h, 0E9h, 00Dh, 0D2h
        db  0F6h, 0FFh, 00Fh, 0B6h, 00Dh, 020h, 013h, 000h, 000h, 00Bh, 0C9h, 00Fh, 084h, 094h, 000h, 000h
        db  000h, 08Bh, 07Ch, 01Eh, 060h, 0E8h, 0DFh, 001h, 000h, 000h, 00Fh, 082h, 085h, 000h, 000h, 000h
        db  033h, 0C0h, 00Fh, 0BFh, 055h, 014h, 00Bh, 0D2h, 079h, 005h, 0BAh, 0FFh, 07Fh, 000h, 000h, 083h
        db  0FAh, 010h, 077h, 002h, 0B2h, 010h, 08Bh, 036h, 00Fh, 0BFh, 04Eh, 006h, 03Bh, 0CAh, 07Dh, 008h
        db  03Bh, 0C8h, 07Ch, 0F2h, 08Bh, 0C1h, 0EBh, 0EEh, 066h, 083h, 07Eh, 004h, 0FFh, 074h, 052h, 02Bh
        db  0CAh, 083h, 0F9h, 010h, 072h, 02Fh, 051h, 052h, 068h, 001h, 001h, 000h, 000h, 06Ah, 008h, 0CDh
        db  020h, 04Fh, 000h, 001h, 000h, 059h, 059h, 05Ah, 059h, 00Bh, 0C0h, 074h, 018h, 066h, 089h, 048h
        db  006h, 066h, 089h, 056h, 006h, 08Bh, 00Eh, 089h, 008h, 089h, 006h, 00Fh, 0B7h, 04Eh, 004h, 003h
        db  0CAh, 066h, 089h, 048h, 004h, 00Fh, 0B7h, 046h, 004h, 066h, 089h, 045h, 010h, 00Fh, 0B7h, 046h
        db  006h, 066h, 089h, 045h, 014h, 080h, 04Eh, 007h, 080h, 066h, 0C7h, 045h, 01Ch, 001h, 000h, 0F8h
        db  0C3h, 00Bh, 0C0h, 075h, 007h, 0B0h, 0B1h, 0E9h, 09Ah, 0CEh, 0F6h, 0FFh, 066h, 089h, 045h, 014h
        db  0B0h, 0B0h, 0E9h, 08Fh, 0CEh, 0F6h, 0FFh, 00Fh, 0B7h, 055h, 014h, 0E8h, 0C9h, 002h, 000h, 000h
        db  075h, 045h, 066h, 00Fh, 0BAh, 076h, 006h, 00Fh, 073h, 03Dh, 066h, 08Bh, 041h, 004h, 066h, 003h
        db  041h, 006h, 066h, 039h, 046h, 004h, 075h, 019h, 066h, 08Bh, 046h, 006h, 066h, 001h, 041h, 006h
        db  08Bh, 006h, 089h, 001h, 087h, 0F1h, 06Ah, 000h, 051h, 0CDh, 020h, 051h, 000h, 001h, 000h, 059h
        db  059h, 08Bh, 0CEh, 08Bh, 031h, 066h, 083h, 07Eh, 004h, 0FFh, 074h, 006h, 0F6h, 046h, 007h, 080h
        db  074h, 0C8h, 0E9h, 008h, 0D1h, 0F6h, 0FFh, 0B0h, 0B2h, 0E9h, 038h, 0CEh, 0F6h, 0FFh, 00Fh, 0B7h
        db  055h, 014h, 0E8h, 072h, 002h, 000h, 000h, 075h, 0EEh, 0F6h, 046h, 007h, 080h, 074h, 0E8h, 00Fh
        db  0BFh, 055h, 010h, 00Bh, 0D2h, 079h, 005h, 0BAh, 0FFh, 07Fh, 000h, 000h, 083h, 0FAh, 010h, 077h
        db  002h, 0B2h, 010h, 08Bh, 00Eh, 080h, 0C6h, 080h, 066h, 02Bh, 056h, 006h, 074h, 051h, 072h, 054h
        db  08Bh, 0C2h, 0F7h, 0D8h, 050h, 066h, 08Bh, 046h, 004h, 066h, 003h, 046h, 006h, 080h, 0ECh, 080h
        db  066h, 039h, 041h, 004h, 058h, 00Fh, 085h, 08Ch, 000h, 000h, 000h, 00Fh, 0BFh, 041h, 006h, 02Bh
        db  0C2h, 00Fh, 08Ch, 080h, 000h, 000h, 000h, 083h, 0F8h, 010h, 072h, 00Eh, 066h, 029h, 051h, 006h
        db  066h, 001h, 051h, 004h, 066h, 001h, 056h, 006h, 0EBh, 015h, 003h, 0C2h, 066h, 001h, 046h, 006h
        db  08Bh, 001h, 089h, 006h, 06Ah, 000h, 051h, 0CDh, 020h, 051h, 000h, 001h, 000h, 059h, 059h, 0E9h
        db  07Bh, 0D0h, 0F6h, 0FFh, 0F6h, 041h, 007h, 080h, 075h, 011h, 066h, 08Bh, 046h, 004h, 066h, 003h
        db  046h, 006h, 080h, 0ECh, 080h, 066h, 039h, 041h, 004h, 074h, 0C1h, 066h, 083h, 0FAh, 0F0h, 077h
        db  0DEh, 052h, 068h, 001h, 001h, 000h, 000h, 06Ah, 008h, 0CDh, 020h, 04Fh, 000h, 001h, 000h, 059h
        db  059h, 05Ah, 00Bh, 0C0h, 074h, 0C9h, 0F7h, 0DAh, 066h, 029h, 056h, 006h, 066h, 089h, 050h, 006h
        db  00Fh, 0B7h, 04Eh, 004h, 066h, 003h, 04Eh, 006h, 080h, 0EDh, 080h, 066h, 089h, 048h, 004h, 08Bh
        db  00Eh, 089h, 008h, 089h, 006h, 0EBh, 0A8h, 003h, 0C2h, 07Dh, 002h, 033h, 0C0h, 066h, 003h, 046h
        db  006h, 080h, 0ECh, 080h, 0E9h, 0B3h, 0FEh, 0FFh, 0FFh, 08Dh, 077h, 00Ch, 083h, 07Fh, 018h, 000h
        db  074h, 001h, 0C3h, 0C8h, 00Ch, 000h, 000h, 0CDh, 020h, 0D5h, 000h, 001h, 000h, 040h, 08Bh, 0F0h
        db  0C1h, 0E6h, 008h, 08Dh, 044h, 043h, 064h, 003h, 005h, 0FCh, 011h, 000h, 000h, 033h, 0D2h, 089h
        db  055h, 0FCh, 089h, 055h, 0F4h, 08Dh, 057h, 00Ch, 033h, 0C9h, 0F6h, 040h, 001h, 040h, 074h, 066h
        db  0F6h, 040h, 001h, 005h, 075h, 060h, 0F6h, 040h, 001h, 010h, 075h, 01Ah, 050h, 052h, 051h, 08Bh
        db  0C6h, 0C1h, 0E8h, 008h, 06Ah, 000h, 053h, 06Ah, 001h, 050h, 0CDh, 020h, 072h, 000h, 001h, 000h
        db  083h, 0C4h, 010h, 059h, 05Ah, 058h, 080h, 048h, 001h, 012h, 0FFh, 045h, 0FCh, 03Bh, 0CEh, 075h
        db  008h, 066h, 0FFh, 042h, 007h, 0FEh, 0C5h, 0EBh, 02Dh, 050h, 052h, 068h, 001h, 001h, 000h, 000h
        db  06Ah, 008h, 0CDh, 020h, 04Fh, 000h, 001h, 000h, 059h, 059h, 05Ah, 089h, 002h, 08Bh, 0D0h, 058h
        db  00Bh, 0D2h, 00Fh, 084h, 0C6h, 000h, 000h, 000h, 066h, 089h, 072h, 004h, 066h, 0C7h, 042h, 006h
        db  000h, 001h, 08Bh, 0CEh, 0FEh, 0C5h, 083h, 0C0h, 002h, 066h, 081h, 0C6h, 000h, 001h, 073h, 08Ah
        db  08Dh, 04Fh, 00Ch, 089h, 00Ah, 083h, 07Dh, 0FCh, 000h, 00Fh, 084h, 09Fh, 000h, 000h, 000h, 068h
        db  080h, 000h, 000h, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 053h, 06Ah, 000h, 0FFh
        db  075h, 0FCh, 0CDh, 020h, 053h, 000h, 001h, 000h, 083h, 0C4h, 020h, 089h, 055h, 0F8h, 00Bh, 0C0h
        db  074h, 07Ch, 089h, 047h, 018h, 08Bh, 077h, 00Ch, 056h, 057h, 00Fh, 0B7h, 046h, 004h, 00Fh, 0B7h
        db  04Eh, 006h, 0C1h, 0E0h, 004h, 050h, 003h, 043h, 004h, 08Bh, 0F0h, 08Bh, 07Dh, 0F4h, 0C1h, 0E7h
        db  00Ch, 003h, 07Dh, 0F8h, 0C1h, 0E1h, 002h, 0FCh, 0F3h, 0A5h, 058h, 05Fh, 05Eh, 0C1h, 0E8h, 00Ch
        db  06Ah, 000h, 0FFh, 075h, 0F4h, 06Ah, 001h, 050h, 053h, 0FFh, 077h, 018h, 0CDh, 020h, 05Dh, 000h
        db  001h, 000h, 083h, 0C4h, 018h, 00Fh, 0B7h, 046h, 006h, 0C1h, 0E8h, 008h, 001h, 045h, 0F4h, 08Bh
        db  036h, 066h, 083h, 07Eh, 004h, 0FFh, 075h, 0B0h, 08Bh, 035h, 0FCh, 011h, 000h, 000h, 0F6h, 044h
        db  01Eh, 02Eh, 001h, 075h, 013h, 06Ah, 000h, 06Ah, 000h, 0FFh, 075h, 0FCh, 0FFh, 077h, 018h, 0CDh
        db  020h, 057h, 000h, 001h, 000h, 083h, 0C4h, 010h, 08Dh, 077h, 00Ch, 0F8h, 0C9h, 0C3h, 033h, 0F6h
        db  087h, 077h, 018h, 00Bh, 0F6h, 074h, 00Bh, 06Ah, 000h, 056h, 0CDh, 020h, 055h, 000h, 001h, 000h
        db  059h, 059h, 08Bh, 077h, 00Ch, 00Bh, 0F6h, 074h, 017h, 066h, 083h, 07Eh, 004h, 0FFh, 074h, 010h
        db  0FFh, 036h, 06Ah, 000h, 056h, 0CDh, 020h, 051h, 000h, 001h, 000h, 059h, 059h, 05Eh, 0EBh, 0E5h
        db  08Dh, 077h, 00Ch, 089h, 077h, 00Ch, 0F9h, 0C9h, 0C3h, 08Bh, 07Ch, 01Eh, 060h, 00Bh, 0FFh, 074h
        db  014h, 08Dh, 077h, 00Ch, 066h, 083h, 0FAh, 0FFh, 074h, 00Bh, 08Bh, 0CEh, 08Bh, 036h, 066h, 039h
        db  056h, 004h, 072h, 0F6h, 0C3h, 083h, 0C9h, 001h, 0C3h, 000h
V86MMGR_PM_API:
        or     byte ptr [ebp + 2ch], 1
        movzx  eax, word ptr [ebp + 1ch]
        sub    eax, 1
        jb     SHORT loc_9_34FA
        ja     SHORT loc_9_3504
        db  0B9h, 004h, 012h, 000h, 000h
        db  039h, 005h, 09Ch, 000h, 000h, 000h
        je     SHORT loc_9_34D2
        or     al, 2
loc_9_34D2:
        test   byte ptr [ecx + 1], 9
        je     SHORT loc_9_34D9
        inc    eax
loc_9_34D9:
        test   byte ptr [ecx + 1], 4
        je     SHORT loc_9_34E1
        or     al, 4
loc_9_34E1:
        db  066h, 083h, 03Dh, 0E8h, 002h, 000h, 000h, 000h
        je     SHORT loc_9_34ED
        or     al, 8
loc_9_34ED:
        test   byte ptr [ecx + 1], 8
        je     SHORT loc_9_34F5
        or     al, 10h
loc_9_34F5:
        mov    dword ptr [ebp + 1ch], eax
        jmp    SHORT loc_9_3500
loc_9_34FA:
        mov    word ptr [ebp + 1ch], 400h
loc_9_3500:
        and    byte ptr [ebp + 2ch], 0feh
loc_9_3504:
        ret
_PTEXT ends
        end
