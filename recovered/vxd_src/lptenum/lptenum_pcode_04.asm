; lptenum_pcode_04.asm -- LPTENUM object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        push   esi
        cmp    dword ptr [esp + 8], 5
        mov    eax, 1
        jne    SHORT loc_4_003E
        mov    esi, dword ptr [esp + 10h]
        push   0
        push   esi
        call   sub_4_0CE4
        add    esp, 8
        mov    ecx, esi
        call   sub_4_0040
        push   0
        push   esi
        call   sub_4_0CEC
        add    esp, 8
        db  068h, 010h, 000h, 000h, 000h
        push   esi
        call   sub_4_019A
        add    esp, 8
        sub    eax, eax
loc_4_003E:
        pop    esi
        ret
sub_4_0040:
        sub    esp, 110h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, ecx
        push   2
        lea    eax, [esp + 18h]
        push   esi
        push   eax
        call   sub_4_0CCC
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_0065
        jmp    NEAR PTR loc_4_0126
loc_4_0065:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 10h], eax
        push   0
        push   0
        push   2
        lea    eax, [esp + 1ch]
        push   dword ptr [esp + 20h]
        push   eax
        call   sub_4_0CD4
        add    esp, 14h
        or     eax, eax
        jne    NEAR PTR loc_4_0126
        mov    eax, dword ptr [esp + 10h]
        push   0
        movzx  edi, word ptr [eax + 4]
        mov    dword ptr [esp + 1ch], 104h
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        push   1
        db  068h, 018h, 000h, 000h, 000h
        push   0
        push   esi
        call   sub_4_0CF4
        add    esp, 1ch
        or     eax, eax
        je     SHORT loc_4_00C0
        jmp    SHORT loc_4_0126
loc_4_00C0:
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_4_0C8C
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_4_00D5
        jmp    SHORT loc_4_0126
loc_4_00D5:
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_4_0C94
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_4_00EA
        jmp    SHORT loc_4_0126
loc_4_00EA:
        push   0
        push   0
        db  068h, 044h, 001h, 000h, 000h
        push   ebp
        push   1
        call   ebx
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_4_0101
        jmp    SHORT loc_4_0126
loc_4_0101:
        mov    edx, edi
        mov    ecx, esi
        call   sub_4_0530
        or     eax, eax
        mov    esi, 1
        jne    SHORT loc_4_0115
        sub    esi, esi
loc_4_0115:
        db  068h, 044h, 001h, 000h, 000h
        push   ebp
        push   3
        call   ebx
        add    esp, 0ch
        mov    eax, esi
        jmp    SHORT loc_4_0128
loc_4_0126:
        sub    eax, eax
loc_4_0128:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 110h
        ret
sub_4_0133:
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_4_0CC4
        add    esp, 0ch
        ret
loc_4_0144:
        sub    eax, eax
        ret
sub_4_0147:
        push   ebx
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
loc_4_0151:
        mov    cl, byte ptr [eax]
        cmp    cl, 61h
        jl     SHORT loc_4_0165
        cmp    cl, 7ah
        jg     SHORT loc_4_0165
        movsx  edx, cl
        sub    edx, 20h
        jmp    SHORT loc_4_0168
loc_4_0165:
        movsx  edx, cl
loc_4_0168:
        mov    bl, byte ptr [esi]
        cmp    bl, 61h
        jl     SHORT loc_4_017C
        cmp    bl, 7ah
        jg     SHORT loc_4_017C
        movsx  ecx, bl
        sub    ecx, 20h
        jmp    SHORT loc_4_017F
loc_4_017C:
        movsx  ecx, bl
loc_4_017F:
        cmp    edx, ecx
        jne    SHORT loc_4_018F
        or     bl, bl
        je     SHORT loc_4_018B
        inc    eax
        inc    esi
        jmp    SHORT loc_4_0151
loc_4_018B:
        sub    eax, eax
        jmp    SHORT loc_4_0197
loc_4_018F:
        movsx  eax, byte ptr [eax]
        movsx  ecx, byte ptr [esi]
        sub    eax, ecx
loc_4_0197:
        pop    esi
        pop    ebx
        ret
sub_4_019A:
        sub    esp, 0e8h
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 1ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   80000002h
        call   sub_4_0C6C
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_4_039A
        sub    esi, esi
        mov    edi, dword ptr [esp + 0fch]
        mov    ebx, dword ptr [esp + 100h]
loc_4_01D3:
        mov    dword ptr [esp + 10h], 65h
        mov    eax, esi
        lea    ecx, [esp + 90h]
        push   65h
        push   ecx
        push   eax
        inc    esi
        push   dword ptr [esp + 28h]
        call   sub_4_0C7C
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_038E
        push   0
        lea    eax, [esp + 2ch]
        push   64h
        push   eax
        push   edi
        call   sub_4_0CB4
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_038E
        push   64h
        lea    ecx, [esp + 2ch]
        push   26h
        mov    edx, 5ch
        call   sub_4_0505
        db  0BAh, 024h, 000h, 000h, 000h
        lea    ecx, [esp + 90h]
        call   sub_4_04A5
        lea    edx, [esp + 28h]
        lea    ecx, [esp + 90h]
        call   sub_4_04A5
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 94h]
        push   eax
        push   dword ptr [esp + 24h]
        call   sub_4_0C6C
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_4_01D3
        mov    dword ptr [esp + 10h], 65h
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        push   0
        push   0
        db  068h, 028h, 000h, 000h, 000h
        push   dword ptr [esp + 28h]
        call   sub_4_0C84
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_4_037D
        lea    eax, [esp + 28h]
        push   ebx
        push   eax
        call   sub_4_0147
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_037D
        mov    ebp, 1
        mov    byte ptr [esp + 28h], 0
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [esp + 28h]
        call   sub_4_04A5
        db  0BAh, 024h, 000h, 000h, 000h
        lea    ecx, [esp + 28h]
        call   sub_4_04A5
        lea    edx, [esp + 90h]
        lea    ecx, [esp + 28h]
        call   sub_4_04A5
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        push   80000005h
        call   sub_4_0C6C
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_0341
        mov    dword ptr [esp + 10h], 4
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        push   0
        push   0
        db  068h, 030h, 000h, 000h, 000h
        push   dword ptr [esp + 2ch]
        call   sub_4_0C84
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_4_0335
        test   byte ptr [esp + 20h], 2
        je     SHORT loc_4_0335
        sub    ebp, ebp
loc_4_0335:
        push   dword ptr [esp + 18h]
        call   sub_4_0C74
        add    esp, 4
loc_4_0341:
        or     ebp, ebp
        je     SHORT loc_4_037D
        mov    byte ptr [esp + 28h], 0
        db  0BAh, 040h, 000h, 000h, 000h
        lea    ecx, [esp + 28h]
        call   sub_4_04A5
        lea    edx, [esp + 90h]
        lea    ecx, [esp + 28h]
        call   sub_4_04A5
        push   0
        lea    eax, [esp + 2ch]
        push   edi
        push   eax
        lea    eax, [esp + 30h]
        push   eax
        call   sub_4_0CBC
        add    esp, 10h
loc_4_037D:
        push   dword ptr [esp + 14h]
        call   sub_4_0C74
        add    esp, 4
        jmp    NEAR PTR loc_4_01D3
loc_4_038E:
        push   dword ptr [esp + 1ch]
        call   sub_4_0C74
        add    esp, 4
loc_4_039A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0e8h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 066h, 08Bh, 0C1h, 066h, 0C1h
        db  0E8h, 008h, 066h, 0C1h, 0E1h, 008h, 066h, 00Bh, 0C8h, 066h, 08Bh, 0C1h, 0C3h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_4_03D0:
        mov    eax, 1
        ret
sub_4_03D6:
        mov    eax, 1
        ret
sub_4_03DC:
        cmp    dword ptr [esp + 8], 0
        mov    eax, 1
        jne    SHORT loc_4_03F1
        mov    ecx, dword ptr [esp + 4]
        call   sub_4_0133
loc_4_03F1:
        ret    8
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_4_0400:
        push   edi
        or     ecx, ecx
        mov    eax, 0
        je     SHORT loc_4_041A
        mov    edi, ecx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
loc_4_041A:
        pop    edi
        ret
        db  02Bh, 0C0h, 03Bh, 0C8h, 075h, 002h, 0EBh, 010h, 080h, 039h, 000h, 074h, 00Bh, 038h, 011h, 074h
        db  00Ch, 041h, 040h, 080h, 039h, 000h, 075h, 0F5h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 0C3h
sub_4_043A:
        mov    eax, ecx
        or     eax, eax
        jne    SHORT loc_4_0442
        jmp    SHORT loc_4_0451
loc_4_0442:
        cmp    byte ptr [eax], 0
        je     SHORT loc_4_0451
loc_4_0447:
        cmp    byte ptr [eax], dl
        je     SHORT loc_4_0453
        inc    eax
        cmp    byte ptr [eax], 0
        jne    SHORT loc_4_0447
loc_4_0451:
        sub    eax, eax
loc_4_0453:
        ret
sub_4_0454:
        push   esi
        push   edi
        mov    esi, ecx
        or     esi, esi
        je     SHORT loc_4_0480
        or     edx, edx
        je     SHORT loc_4_0480
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, eax
        shr    ecx, 2
        mov    edi, edx
        repe cmpsd
        jne    SHORT loc_4_0476
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
loc_4_0476:
        mov    eax, 0
        sete   al
        jmp    SHORT loc_4_0482
loc_4_0480:
        sub    eax, eax
loc_4_0482:
        pop    edi
        pop    esi
        ret    4
        db  00Bh, 0C9h, 074h, 019h, 080h, 039h, 000h, 074h, 014h, 08Ah, 001h, 03Ch, 061h, 07Ch, 008h, 03Ch
        db  07Ah, 07Fh, 004h, 02Ch, 020h, 088h, 001h, 041h, 080h, 039h, 000h, 075h, 0ECh, 0C3h
sub_4_04A5:
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_4_04DB:
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_4_0505:
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        cmp    ecx, esi
        je     SHORT loc_4_0528
        mov    bl, byte ptr [esp + 10h]
        mov    eax, dword ptr [esp + 14h]
loc_4_0516:
        mov    edi, esi
        inc    esi
        cmp    edi, eax
        jge    SHORT loc_4_0528
        cmp    byte ptr [ecx], dl
        jne    SHORT loc_4_0523
        mov    byte ptr [ecx], bl
loc_4_0523:
        inc    ecx
        or     ecx, ecx
        jne    SHORT loc_4_0516
loc_4_0528:
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  000h, 000h
sub_4_0530:
        sub    esp, 158h
        push   esi
        push   edi
        mov    dword ptr [esp + 14h], 0
        mov    dword ptr [esp + 0ch], 0
        mov    dword ptr [esp + 8], 0
        mov    dword ptr [esp + 20h], 0
        mov    esi, ecx
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 24h], 0
        lea    eax, [esp + 14h]
        mov    ecx, edx
        mov    edx, eax
        call   sub_4_08E2
        or     eax, eax
        jne    SHORT loc_4_058A
        sub    eax, eax
        jmp    NEAR PTR loc_4_08D9
loc_4_058A:
        lea    eax, [esp + 1ch]
        push   dword ptr [esp + 14h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 30h]
        push   eax
        lea    eax, [esp + 30h]
        lea    edx, [esp + 18h]
        lea    ecx, [esp + 1ch]
        push   eax
        call   sub_4_09E8
        cmp    dword ptr [esp + 0ch], 0
        je     NEAR PTR loc_4_08C0
        cmp    dword ptr [esp + 8], 0
        je     NEAR PTR loc_4_08C0
        push   0
        lea    eax, [esp + 100h]
        push   64h
        push   eax
        push   esi
        call   sub_4_0CB4
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_05E3
        jmp    NEAR PTR loc_4_08C0
loc_4_05E3:
        push   64h
        lea    ecx, [esp + 100h]
        push   26h
        mov    edx, 5ch
        call   sub_4_0505
        lea    ecx, [esp + 34h]
        mov    edx, dword ptr [esp + 0ch]
        call   sub_4_04DB
        lea    ecx, [esp + 34h]
        mov    edx, dword ptr [esp + 8]
        call   sub_4_04A5
        mov    dword ptr [esp + 28h], 0
        lea    eax, [esp + 28h]
        push   0
        lea    ecx, [esp + 38h]
        push   eax
        call   sub_4_0400
        push   eax
        lea    eax, [esp + 40h]
        push   eax
        call   sub_4_0CDC
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_0644
        mov    dword ptr [esp + 28h], 0deadh
loc_4_0644:
        push   dword ptr [esp + 28h]
        lea    eax, [esp + 30h]
        db  068h, 04Ch, 000h, 000h, 000h
        push   eax
        call   sub_4_0CAC
        mov    ecx, dword ptr [esp + 14h]
        add    esp, 0ch
        call   sub_4_0400
        mov    edi, eax
        mov    ecx, dword ptr [esp + 0ch]
        call   sub_4_0400
        add    eax, edi
        cmp    eax, 14h
        jbe    SHORT loc_4_067A
        mov    eax, 14h
loc_4_067A:
        mov    byte ptr [esp + eax + 34h], 0
        lea    ecx, [esp + 34h]
        call   sub_4_0400
        push   eax
        lea    ecx, [esp + 38h]
        push   5fh
        mov    edx, 20h
        call   sub_4_0505
        lea    edx, [esp + 2ch]
        lea    ecx, [esp + 34h]
        call   sub_4_04A5
        db  0BAh, 050h, 000h, 000h, 000h
        lea    ecx, [esp + 98h]
        call   sub_4_04DB
        db  0BAh, 024h, 000h, 000h, 000h
        lea    ecx, [esp + 98h]
        call   sub_4_04A5
        lea    edx, [esp + 34h]
        lea    ecx, [esp + 98h]
        call   sub_4_04A5
        db  0BAh, 024h, 000h, 000h, 000h
        lea    ecx, [esp + 98h]
        call   sub_4_04A5
        lea    edx, [esp + 0fch]
        lea    ecx, [esp + 98h]
        call   sub_4_04A5
        push   0
        lea    eax, [esp + 9ch]
        push   esi
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_4_0CBC
        add    esp, 10h
        cmp    eax, 2
        jne    SHORT loc_4_071E
        jmp    NEAR PTR loc_4_08C0
loc_4_071E:
        cmp    eax, 10h
        jne    SHORT loc_4_072D
        mov    esi, 1
        jmp    NEAR PTR loc_4_08C2
loc_4_072D:
        or     eax, eax
        je     SHORT loc_4_0736
        jmp    NEAR PTR loc_4_08C0
loc_4_0736:
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_4_0762
        mov    ecx, dword ptr [esp + 8]
        push   0
        call   sub_4_0400
        push   eax
        push   dword ptr [esp + 10h]
        push   1
        db  068h, 058h, 000h, 000h, 000h
        push   0
        push   dword ptr [esp + 28h]
        call   sub_4_0CFC
        add    esp, 1ch
loc_4_0762:
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_4_078E
        mov    ecx, dword ptr [esp + 0ch]
        push   0
        call   sub_4_0400
        push   eax
        push   dword ptr [esp + 14h]
        push   1
        db  068h, 060h, 000h, 000h, 000h
        push   0
        push   dword ptr [esp + 28h]
        call   sub_4_0CFC
        add    esp, 1ch
loc_4_078E:
        cmp    dword ptr [esp + 24h], 0
        jne    SHORT loc_4_0810
        mov    ecx, dword ptr [esp + 8]
        call   sub_4_0400
        mov    esi, eax
        mov    ecx, dword ptr [esp + 0ch]
        call   sub_4_0400
        lea    eax, [esi + eax + 2]
        push   201h
        push   eax
        call   sub_4_0C9C
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_4_0835
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, esi
        call   sub_4_04DB
        db  0BAh, 070h, 000h, 000h, 000h
        mov    ecx, esi
        call   sub_4_04A5
        mov    edx, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_4_04A5
        push   0
        mov    ecx, esi
        call   sub_4_0400
        push   eax
        push   esi
        push   1
        db  068h, 074h, 000h, 000h, 000h
        push   0
        push   dword ptr [esp + 28h]
        call   sub_4_0CFC
        add    esp, 1ch
        push   0
        push   esi
        call   sub_4_0CA4
        add    esp, 8
        jmp    SHORT loc_4_0835
loc_4_0810:
        mov    ecx, dword ptr [esp + 24h]
        push   0
        call   sub_4_0400
        push   eax
        push   dword ptr [esp + 2ch]
        push   1
        db  068h, 074h, 000h, 000h, 000h
        push   0
        push   dword ptr [esp + 28h]
        call   sub_4_0CFC
        add    esp, 1ch
loc_4_0835:
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_4_0861
        mov    ecx, dword ptr [esp + 20h]
        push   0
        call   sub_4_0400
        push   eax
        push   dword ptr [esp + 28h]
        push   1
        db  068h, 028h, 000h, 000h, 000h
        push   0
        push   dword ptr [esp + 28h]
        call   sub_4_0CFC
        add    esp, 1ch
loc_4_0861:
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_4_088D
        mov    ecx, dword ptr [esp + 1ch]
        push   0
        call   sub_4_0400
        push   eax
        push   dword ptr [esp + 24h]
        push   1
        db  068h, 080h, 000h, 000h, 000h
        push   0
        push   dword ptr [esp + 28h]
        call   sub_4_0CFC
        add    esp, 1ch
loc_4_088D:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_4_08B9
        mov    ecx, dword ptr [esp + 18h]
        push   0
        call   sub_4_0400
        push   eax
        push   dword ptr [esp + 20h]
        push   1
        db  068h, 090h, 000h, 000h, 000h
        push   0
        push   dword ptr [esp + 28h]
        call   sub_4_0CFC
        add    esp, 1ch
loc_4_08B9:
        mov    esi, 1
        jmp    SHORT loc_4_08C2
loc_4_08C0:
        sub    esi, esi
loc_4_08C2:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_4_08D7
        push   0
        push   dword ptr [esp + 18h]
        call   sub_4_0CA4
        add    esp, 8
loc_4_08D7:
        mov    eax, esi
loc_4_08D9:
        pop    edi
        pop    esi
        add    esp, 158h
        ret
sub_4_08E2:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        mov    dword ptr [esp + 0ch], 0
        push   4
        push   esi
        db  0E8h, 000h, 0F8h, 0FBh, 0FFh
        add    esp, 8
        cmp    eax, 10000h
        je     SHORT loc_4_090B
        jmp    NEAR PTR loc_4_09DF
loc_4_090B:
        lea    ebx, [esi + 1]
        push   ebx
        db  0E8h, 0BCh, 0FAh, 0FBh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_093A
        lea    edx, [esi + 2]
        mov    al, 0e6h
        out    dx, al
        out    dx, al
        push   23h
        push   0
        push   8
        push   ebx
        db  0E8h, 0BAh, 0FAh, 0FBh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_093A
        jmp    NEAR PTR loc_4_09D4
loc_4_093A:
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 10h]
        push   2
        push   eax
        push   esi
        db  0E8h, 02Ah, 0F9h, 0FBh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_0955
        jmp    SHORT loc_4_09D4
loc_4_0955:
        mov    eax, dword ptr [esp + 0ch]
        xchg   ah, al
        mov    dword ptr [esp + 0ch], eax
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_4_09D4
        cmp    dword ptr [esp + 10h], 2
        jne    SHORT loc_4_09D4
        mov    eax, dword ptr [esp + 0ch]
        push   201h
        inc    eax
        push   eax
        call   sub_4_0C9C
        add    esp, 8
        mov    dword ptr [edi], eax
        mov    ecx, eax
        or     ecx, ecx
        jne    SHORT loc_4_098A
        jmp    SHORT loc_4_09D4
loc_4_098A:
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 10h]
        sub    eax, 2
        push   eax
        push   ecx
        push   esi
        db  0E8h, 0D8h, 0F8h, 0FBh, 0FFh
        add    esp, 10h
        or     eax, eax
        push   0
        push   esi
        jne    SHORT loc_4_09BB
        db  0E8h, 083h, 0F6h, 0FBh, 0FFh
        add    esp, 8
        push   0
        push   dword ptr [edi]
        call   sub_4_0CA4
        jmp    SHORT loc_4_09DC
loc_4_09BB:
        db  0E8h, 070h, 0F6h, 0FBh, 0FFh
        add    esp, 8
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [esp + 0ch]
        mov    byte ptr [eax + ecx], 0
        mov    eax, 1
        jmp    SHORT loc_4_09E1
loc_4_09D4:
        push   0
        push   esi
        db  0E8h, 054h, 0F6h, 0FBh, 0FFh
loc_4_09DC:
        add    esp, 8
loc_4_09DF:
        sub    eax, eax
loc_4_09E1:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_4_09E8:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], edx
        mov    esi, ecx
        mov    ebx, dword ptr [esp + 28h]
        or     ebx, ebx
        je     NEAR PTR loc_4_0C60
loc_4_0A01:
        cmp    byte ptr [ebx], 20h
        jne    SHORT loc_4_0A0C
loc_4_0A06:
        inc    ebx
        cmp    byte ptr [ebx], 20h
        je     SHORT loc_4_0A06
loc_4_0A0C:
        mov    edx, 3ah
        mov    ecx, ebx
        call   sub_4_043A
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_4_0C60
        inc    edi
        mov    ecx, edi
        sub    ecx, ebx
        movsx  eax, byte ptr [ebx]
        cmp    eax, 41h
        je     SHORT loc_4_0A65
        cmp    eax, 43h
        je     NEAR PTR loc_4_0AC6
        cmp    eax, 44h
        je     NEAR PTR loc_4_0B6A
        cmp    eax, 4dh
        je     NEAR PTR loc_4_0BCB
        mov    edx, 3bh
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_4_0C60
        jmp    NEAR PTR loc_4_0C54
loc_4_0A65:
        movzx  ebp, cx
        push   ebp
        db  0BAh, 004h, 001h, 000h, 000h
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    SHORT loc_4_0AA5
        push   ebp
        db  0BAh, 014h, 001h, 000h, 000h
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    SHORT loc_4_0AA5
        mov    edx, 3bh
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_4_0C60
        jmp    NEAR PTR loc_4_0C54
loc_4_0AA5:
        mov    eax, dword ptr [esp + 20h]
        mov    edx, 3bh
        mov    dword ptr [eax], edi
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_4_0C60
        jmp    NEAR PTR loc_4_0C54
loc_4_0AC6:
        movzx  ebp, cx
        push   ebp
        db  0BAh, 0C4h, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    SHORT loc_4_0B49
        push   ebp
        db  0BAh, 0CCh, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    SHORT loc_4_0B49
        push   ebp
        db  0BAh, 0D4h, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    SHORT loc_4_0B28
        push   ebp
        db  0BAh, 0E4h, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    SHORT loc_4_0B28
        mov    edx, 3bh
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_4_0C60
        jmp    NEAR PTR loc_4_0C54
loc_4_0B28:
        mov    eax, dword ptr [esp + 24h]
        mov    edx, 3bh
        mov    dword ptr [eax], edi
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_4_0C60
        jmp    NEAR PTR loc_4_0C54
loc_4_0B49:
        mov    eax, dword ptr [esp + 18h]
        mov    edx, 3bh
        mov    dword ptr [eax], edi
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_4_0C60
        jmp    NEAR PTR loc_4_0C54
loc_4_0B6A:
        movzx  ebp, cx
        push   ebp
        db  0BAh, 0ECh, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    SHORT loc_4_0BAA
        push   ebp
        db  0BAh, 0FCh, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    SHORT loc_4_0BAA
        mov    edx, 3bh
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_4_0C60
        jmp    NEAR PTR loc_4_0C54
loc_4_0BAA:
        mov    eax, dword ptr [esp + 1ch]
        mov    edx, 3bh
        mov    dword ptr [eax], edi
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_4_0C60
        jmp    NEAR PTR loc_4_0C54
loc_4_0BCB:
        movzx  ebp, cx
        push   ebp
        db  0BAh, 09Ch, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    SHORT loc_4_0C40
        push   ebp
        db  0BAh, 0ACh, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    SHORT loc_4_0C40
        push   ebp
        db  0BAh, 0B4h, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    SHORT loc_4_0C26
        push   ebp
        db  0BAh, 0BCh, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    SHORT loc_4_0C26
        mov    edx, 3bh
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_4_0C60
        jmp    SHORT loc_4_0C54
loc_4_0C26:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, 3bh
        mov    dword ptr [eax], edi
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_4_0C60
        jmp    SHORT loc_4_0C54
loc_4_0C40:
        mov    dword ptr [esi], edi
        mov    edx, 3bh
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_4_0C60
loc_4_0C54:
        mov    byte ptr [ebx], 0
        inc    ebx
        or     ebx, ebx
        jne    NEAR PTR loc_4_0A01
loc_4_0C60:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    14h
        db  000h, 000h
sub_4_0C6C:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_4_0C74:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_4_0C7C:
        db  0CDh, 020h, 04Ch, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_4_0C84:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_4_0C8C:
        db  0CDh, 020h, 021h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_4_0C94:
        db  0CDh, 020h, 022h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_4_0C9C:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_4_0CA4:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_4_0CAC:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_4_0CB4:
        db  0CDh, 020h, 007h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_0CBC:
        db  0CDh, 020h, 00Bh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_0CC4:
        db  0CDh, 020h, 00Fh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_0CCC:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_0CD4:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_0CDC:
        db  0CDh, 020h, 031h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_0CE4:
        db  0CDh, 020h, 035h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_0CEC:
        db  0CDh, 020h, 037h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_0CF4:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_0CFC:
        db  0CDh, 020h, 03Fh, 080h, 033h, 025h
_PTEXT ends
        end
