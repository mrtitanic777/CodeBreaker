; vfbackup_pcode_04.asm -- VFBACKUP object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

VFBACKUP_Lock_NEC:
        pushad
        cli
        db  08Bh, 015h, 000h, 004h, 000h, 000h
        mov    dword ptr [edx], ecx
        db  08Bh, 015h, 0F4h, 003h, 000h, 000h
        mov    ebp, dword ptr [edx]
        xor    al, 1
        shl    eax, 8
        db  0F6h, 005h, 0F0h, 003h, 000h, 000h, 010h
        je     NEAR PTR loc_4_01E3
        cmp    ebp, ebx
        je     NEAR PTR loc_4_012B
        or     ebp, ebp
        jne    NEAR PTR loc_4_01EA
        mov    dword ptr [edx], ebx
        sti
        push   eax
        xor    ecx, ecx
        sub    esp, 0ch
        mov    word ptr [esp], 7
        mov    dword ptr [esp + 8], 40000000h
        push   esp
        db  0FFh, 015h, 068h, 000h, 000h, 000h
        mov    ebx, dword ptr [esp + 8]
        movzx  eax, word ptr [esp + 6]
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_007E
        test   byte ptr [ebx + 23h], 2
        je     SHORT loc_4_006E
        test   byte ptr [ebx + 24h], 8
        jne    SHORT loc_4_007E
loc_4_006E:
        push   ecx
        push   1
        push   0
        db  0CDh, 020h, 04Ch, 000h, 040h, 000h
        add    esp, 8
        pop    ecx
        inc    ecx
loc_4_007E:
        bt     ecx, 0
        rcl    ecx, 1
        sub    esp, 0ch
        mov    word ptr [esp], 7
        mov    dword ptr [esp + 8], 40000000h
        push   esp
        db  0FFh, 015h, 068h, 000h, 000h, 000h
        mov    ebx, dword ptr [esp + 8]
        movzx  eax, word ptr [esp + 6]
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_00BE
        or     ecx, 1
        test   byte ptr [ebx + 23h], 2
        je     SHORT loc_4_00BE
        test   byte ptr [ebx + 24h], 8
        je     SHORT loc_4_00BE
        and    ecx, 2
loc_4_00BE:
        test   ecx, 1
        je     SHORT loc_4_00D5
        push   ecx
        push   1
        push   1
        db  0CDh, 020h, 04Ch, 000h, 040h, 000h
        add    esp, 8
        pop    ecx
loc_4_00D5:
        pop    eax
        db  08Bh, 015h, 0F4h, 003h, 000h, 000h
        mov    ebx, dword ptr [edx]
        push   ecx
        mov    cl, 1
        db  0CDh, 020h, 009h, 000h, 010h, 000h
        pop    ecx
        jb     NEAR PTR loc_4_01EA
        or     eax, 40000000h
        push   ecx
        mov    cl, 1
        db  0CDh, 020h, 009h, 000h, 010h, 000h
        pop    ecx
        and    eax, 0bfffffffh
        db  083h, 03Dh, 018h, 004h, 000h, 000h, 001h
        je     SHORT loc_4_012B
        inc    eax
        push   ecx
        mov    cl, 1
        db  0CDh, 020h, 009h, 000h, 010h, 000h
        pop    ecx
        jb     NEAR PTR loc_4_01EA
        or     eax, 40000000h
        push   ecx
        mov    cl, 1
        db  0CDh, 020h, 009h, 000h, 010h, 000h
        pop    ecx
loc_4_012B:
        mov    ebp, eax
        db  0A1h, 0F8h, 003h, 000h, 000h
        cmp    dword ptr [eax], 0
        sti
        jne    SHORT loc_4_0179
        xor    ebx, ebx
loc_4_013A:
        xor    ebx, 1
        bt     ecx, ebx
        pushfd
        xor    ebx, 1
        popfd
        jae    SHORT loc_4_0173
        push   ecx
        call   sub_4_04E4
        pop    ecx
        or     esi, esi
        je     NEAR PTR loc_4_01EA
        push   ecx
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        pop    ecx
        cmp    word ptr [esi + 6], 10h
        jb     SHORT loc_4_016C
        cmp    word ptr [esi + 6], 16h
        jne    SHORT loc_4_0179
loc_4_016C:
        push   ecx
        call   sub_4_0568
        pop    ecx
loc_4_0173:
        inc    ebx
        cmp    bl, 1
        jbe    SHORT loc_4_013A
loc_4_0179:
        db  08Bh, 01Dh, 0F4h, 003h, 000h, 000h
        mov    ebx, dword ptr [ebx]
        push   ebx
        test   ebp, 100h
        je     SHORT loc_4_018D
        mov    ebx, dword ptr [ebx + 14h]
loc_4_018D:
        cli
        mov    ecx, 6
        db  0BFh, 0DCh, 003h, 000h, 000h
loc_4_0198:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        add    edi, 2
        loop   loc_4_0198
        pop    ebx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        jecxz  loc_4_01B1
        call   ecx
loc_4_01B1:
        db  0A1h, 0F8h, 003h, 000h, 000h
        mov    dword ptr [eax], ebx
        and    ebp, 100h
        db  0A1h, 0FCh, 003h, 000h, 000h
        and    dword ptr [eax], 0fffffefeh
        or     dword ptr [eax], ebp
        db  08Bh, 00Dh, 000h, 004h, 000h, 000h
        cmp    dword ptr [ecx], 0
        je     SHORT loc_4_01E0
        db  0A1h, 014h, 004h, 000h, 000h
        mov    dword ptr [esp + 1ch], eax
        clc
loc_4_01E0:
        sti
loc_4_01E1:
        popad
        ret
loc_4_01E3:
        sti
        mov    ebp, eax
        mov    dword ptr [edx], ebx
        jmp    SHORT loc_4_0179
loc_4_01EA:
        mov    dword ptr [edx], ebp
        xor    eax, eax
        xor    ecx, ecx
        db  0CDh, 020h, 009h, 000h, 010h, 000h
        inc    eax
        xor    ecx, ecx
        db  0CDh, 020h, 009h, 000h, 010h, 000h
        stc
        jmp    SHORT loc_4_01E1
VFBACKUP_Lock_All_Ports:
        db  08Bh, 015h, 0F4h, 003h, 000h, 000h
        cmp    dword ptr [edx], ebx
        stc
        jne    SHORT loc_4_0224
        db  08Bh, 015h, 0FCh, 003h, 000h, 000h
        or     byte ptr [edx], 8
        test   byte ptr [edx + 1], 1
        je     SHORT loc_4_021F
        mov    ebx, dword ptr [ebx + 14h]
loc_4_021F:
        db  0E8h, 061h, 0FFh, 0FBh, 0FFh
loc_4_0224:
        ret
VFBACKUP_UnLock_NEC:
        pushad
        db  08Bh, 015h, 0F4h, 003h, 000h, 000h
        db  0F6h, 005h, 0F0h, 003h, 000h, 000h, 010h
        je     SHORT loc_4_0270
        cmp    dword ptr [edx], ebx
        stc
        jne    NEAR PTR loc_4_02FF
        xor    eax, eax
        xor    ecx, ecx
        db  0CDh, 020h, 009h, 000h, 010h, 000h
        or     eax, 40000000h
        db  0CDh, 020h, 009h, 000h, 010h, 000h
        db  083h, 03Dh, 018h, 004h, 000h, 000h, 001h
        je     SHORT loc_4_0270
        inc    eax
        xor    ecx, ecx
        db  0CDh, 020h, 009h, 000h, 010h, 000h
        and    eax, 0bfffffffh
        db  0CDh, 020h, 009h, 000h, 010h, 000h
loc_4_0270:
        db  0A1h, 0FCh, 003h, 000h, 000h
        and    byte ptr [eax], 0f7h
        cli
        mov    ecx, 6
        db  0BFh, 0DCh, 003h, 000h, 000h
        test   dword ptr [eax], 100h
        je     SHORT loc_4_028E
        mov    ebx, dword ptr [ebx + 14h]
loc_4_028E:
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_4_02A4
loc_4_0296:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        add    edi, 2
        loop   loc_4_0296
loc_4_02A4:
        db  08Bh, 015h, 0F4h, 003h, 000h, 000h
        mov    dword ptr [edx], 0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        jecxz  loc_4_02BA
        call   ecx
loc_4_02BA:
        db  08Bh, 00Dh, 000h, 004h, 000h, 000h
        mov    dword ptr [ecx], 0
        db  0F6h, 005h, 0ACh, 004h, 000h, 000h, 008h
        jne    SHORT loc_4_02FD
        db  0F6h, 005h, 0ACh, 004h, 000h, 000h, 004h
        je     SHORT loc_4_02FD
        xor    edx, edx
        db  087h, 015h, 0A8h, 004h, 000h, 000h
        sub    edx, 2000h
        push   0
        push   edx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_02FD
        db  080h, 025h, 0ACh, 004h, 000h, 000h, 0FBh
loc_4_02FD:
        clc
        sti
loc_4_02FF:
        popad
        ret
        db  000h, 000h, 000h, 000h
loc_4_0305:
        ret
loc_4_0306:
        movzx  eax, byte ptr [ebp + 1dh]
        cmp    al, 4
        db  077h, 036h
        db  08Bh, 00Dh, 040h, 003h, 000h, 000h
        jmp    dword ptr cs:[ecx + eax*4]
loc_4_0318:
        push   esi
        add    eax, dword ptr [eax]
        db  000h, 05Dh, 003h
        add    byte ptr [eax], al
        jecxz  loc_4_0325
        add    byte ptr [eax], al
        pop    es
loc_4_0325:
        add    al, 0
        db  000h, 073h, 004h
        add    byte ptr [eax], al
loc_4_032C:
        push   esi
        add    eax, dword ptr [eax]
        db  000h, 05Dh, 003h
        add    byte ptr [eax], al
        jecxz  loc_4_0339
        add    byte ptr [eax], al
        pop    es
loc_4_0339:
        add    al, 0
        db  000h, 073h, 004h
        add    byte ptr [eax], al
loc_4_0340:
        faddp  st(0)
        lodsd
        mov    edx, 840f103ch
        dec    edx
        add    dword ptr [eax], eax
        add    byte ptr [ecx + edx], bh
        je     NEAR PTR loc_4_04C4
        jmp    SHORT sub_4_0356
sub_4_0356:
        db  0FFh, 015h, 03Ch, 004h, 000h, 000h
        ret
loc_4_035D:
        movzx  eax, byte ptr [ebp + 1ch]
        and    al, 0fh
        or     eax, eax
        jne    SHORT loc_4_0369
        mov    al, 6
loc_4_0369:
        cmp    al, 6
        jne    SHORT loc_4_039F
        push   ebx
        push   edi
        db  0A1h, 0F4h, 003h, 000h, 000h
        cmp    dword ptr [eax], ebx
        jne    SHORT loc_4_037D
        call   VFBACKUP_UnLock_NEC
loc_4_037D:
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    ebx, edi
        xor    ecx, ecx
        mov    eax, ecx
        call   0
        pop    edi
        pop    ebx
        mov    word ptr [ebp + 1ch], 1
        jb     SHORT loc_4_039E
        mov    word ptr [ebp + 1ch], 0
loc_4_039E:
        ret
loc_4_039F:
        cmp    al, 0ah
        jb     SHORT sub_4_0356
        cmp    al, 0ch
        ja     SHORT sub_4_0356
        mov    eax, 30001h
        db  0BEh, 0D9h, 003h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        call   sub_4_0356
        mov    eax, 30001h
        db  0BEh, 0D9h, 003h, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
        ret
loc_4_03CD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jmp    SHORT loc_4_03D9
        db  0FFh, 025h, 0CDh, 003h, 000h, 000h
loc_4_03D9:
        and    byte ptr [edi + 2], 0fdh
        db  0FFh, 025h, 0CDh, 003h, 000h, 000h
loc_4_03E3:
        push   edi
        push   ebx
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    ebx, edi
        call   VFBACKUP_UnLock_NEC
        pop    ebx
        pop    edi
        xor    eax, eax
        db  087h, 005h, 038h, 004h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_0406
        db  0CDh, 020h, 011h, 000h, 003h, 000h
loc_4_0406:
        ret
loc_4_0407:
        db  083h, 03Dh, 030h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_046C
        push   0
        push   0
        push   92h
        push   8000h
        db  0FFh, 035h, 034h, 004h, 000h, 000h
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_4_046C
        db  0A3h, 001h, 003h, 000h, 000h
        xchg   edi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        db  0A1h, 001h, 003h, 000h, 000h
        shl    eax, 10h
        mov    dword ptr [edi], eax
        db  0A1h, 030h, 004h, 000h, 000h
        mov    dword ptr [edi + 4], eax
        mov    word ptr [ebp + 1ch], 0
        jmp    SHORT loc_4_0472
loc_4_046C:
        mov    word ptr [ebp + 1ch], 1
loc_4_0472:
        ret
loc_4_0473:
        mov    word ptr [ebp + 1ch], 0
        push   0
        db  0FFh, 035h, 001h, 003h, 000h, 000h
        push   ebx
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_0495
        mov    word ptr [ebp + 1ch], 1
loc_4_0495:
        ret
        db  066h, 0C7h, 045h, 01Ch, 001h, 000h, 0A1h, 0F4h, 003h, 000h, 000h, 039h, 018h, 075h, 005h, 0E8h
        db  07Bh, 0FDh, 0FFh, 0FFh, 053h, 057h, 0CDh, 020h, 008h, 001h, 001h, 000h, 08Bh, 0DFh, 033h, 0C9h
        db  08Bh, 0C1h, 0E8h, 043h, 0FBh, 0FFh, 0FFh, 05Fh, 05Bh, 066h, 089h, 04Dh, 01Ch, 0C3h
loc_4_04C4:
        mov    word ptr [ebp + 1ch], 1
        xor    ecx, ecx
        push   edi
        push   ebx
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    ebx, edi
        call   VFBACKUP_UnLock_NEC
        pop    ebx
        pop    edi
        jb     SHORT loc_4_04E3
        mov    word ptr [ebp + 1ch], cx
loc_4_04E3:
        ret
sub_4_04E4:
        push   ebx
        sub    esp, 0ch
        mov    word ptr [esp], 7
        mov    dword ptr [esp + 8], ebx
        push   esp
        db  0FFh, 015h, 068h, 000h, 000h, 000h
        mov    ebx, dword ptr [esp + 8]
        movzx  eax, word ptr [esp + 6]
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_0564
        sub    esp, 10h
        mov    esi, esp
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, ecx
        add    ecx, 0bch
        add    eax, 64h
        mov    dword ptr [esi + 6], eax
        mov    word ptr [esi + 4], cx
        mov    byte ptr [esi + 0eh], 0
        mov    word ptr [esi], 2
        push   esi
        db  0FFh, 015h, 068h, 000h, 000h, 000h
        mov    esi, dword ptr [esi + 0ah]
        add    esi, eax
        mov    dword ptr [esi + 20h], eax
        add    esp, 14h
        or     esi, esi
        je     SHORT loc_4_0563
        xor    eax, eax
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 44h], eax
        mov    dword ptr [esi + 18h], eax
        mov    dword ptr [esi + 8], 14508h
        mov    dword ptr [esi], 0
        mov    word ptr [esi + 4], 10h
        pop    ebx
        mov    byte ptr [esi + 4dh], bl
loc_4_0563:
        ret
loc_4_0564:
        pop    ebx
        xor    esi, esi
        ret
sub_4_0568:
        sub    esp, 8
        mov    edi, esp
        sub    esi, dword ptr [esi + 20h]
        mov    dword ptr [edi + 4], esi
        mov    word ptr [edi], 4
        push   edi
        db  0FFh, 015h, 068h, 000h, 000h, 000h
        add    esp, 0ch
        ret
        db  000h
loc_4_0584:
        push   ecx
        db  08Bh, 00Dh, 0F4h, 003h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        jecxz  loc_4_05A1
        push   eax
        db  0A1h, 0FCh, 003h, 000h, 000h
        test   dword ptr [eax], 100h
        pop    eax
        je     SHORT loc_4_05A1
        mov    ecx, dword ptr [ecx + 14h]
loc_4_05A1:
        cmp    ebx, ecx
        jne    SHORT loc_4_05C0
        xor    edi, edi
loc_4_05A7:
        db  08Bh, 00Dh, 0FCh, 003h, 000h, 000h
        or     dword ptr [ecx], edi
        db  0C7h, 005h, 09Ch, 004h, 000h, 000h, 01Eh, 000h, 000h, 000h
        pop    ecx
        db  0CDh, 020h, 012h, 080h, 004h, 000h
loc_4_05C0:
        push   eax
        xor    ecx, ecx
        xor    eax, eax
        inc    eax
        call   0
        mov    edi, 1
        pop    eax
        jae    SHORT loc_4_05A7
        db  0CDh, 020h, 02Fh, 000h, 001h, 000h
sub_4_05D9:
        push   edi
        sub    esp, 20h
        mov    edi, esp
        mov    word ptr [edi], ax
        mov    word ptr [edi + 2], 2
        db  0C7h, 047h, 004h, 029h, 001h, 000h, 000h
        mov    dword ptr [edi + 8], 0
        db  0C7h, 047h, 00Ch, 077h, 001h, 000h, 000h
        mov    dword ptr [edi + 10h], 0
        mov    dword ptr [edi + 14h], 0
        mov    dword ptr [edi + 18h], 1f4h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        mov    edi, eax
        lahf
        add    esp, 20h
        rcr    ah, 1
        mov    eax, edi
        pop    edi
        ret
loc_4_0624:
        clc
        ret
loc_4_0626:
        db  00Fh, 0BAh, 02Dh, 0ACh, 004h, 000h, 000h, 002h
        jb     SHORT loc_4_0659
        push   8
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        push   2
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        or     edx, edx
        je     SHORT sub_4_067E
        add    edx, 2000h
        db  089h, 015h, 0A8h, 004h, 000h, 000h
loc_4_0659:
        db  00Fh, 0BAh, 02Dh, 0ACh, 004h, 000h, 000h, 003h
        jb     SHORT sub_4_067E
        db  087h, 025h, 0A8h, 004h, 000h, 000h
        call   sub_4_067E
        db  087h, 025h, 0A8h, 004h, 000h, 000h
        db  00Fh, 0BAh, 035h, 0ACh, 004h, 000h, 000h, 003h
        clc
        ret
sub_4_067E:
        db  0FFh, 025h, 0A0h, 004h, 000h, 000h
loc_4_0684:
        clc
        ret
loc_4_0686:
        sbb    eax, dword ptr [edi]
        add    byte ptr [eax], al
        and    al, byte ptr [edi]
        add    byte ptr [eax], al
        pop    esi
        pop    es
        add    byte ptr [eax], al
        xchg   byte ptr [edi], al
        add    byte ptr [eax], al
        mov    byte ptr [esi], 0
        db  000h, 0C6h
        push   es
        add    byte ptr [eax], al
        mov    byte ptr [esi], 0
        db  000h, 0C6h
        push   es
        add    byte ptr [eax], al
        mov    byte ptr [esi], 0
        add    byte ptr [esi - 7dh], ah
        sub    al, 0feh
        movzx  edx, byte ptr [ebp + 1dh]
        cmp    dl, 8
        ja     SHORT loc_4_06BF
        db  0FFh, 024h, 095h, 086h, 006h, 000h, 000h
loc_4_06BF:
        or     word ptr [ebp + 2ch], 1
        clc
        ret
loc_4_06C6:
        db  0FFh, 025h, 0C4h, 004h, 000h, 000h
loc_4_06CC:
        db  0C7h, 005h, 0C0h, 004h, 000h, 000h, 000h, 000h, 000h, 000h
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  00Fh, 0B7h, 00Dh, 0BEh, 004h, 000h, 000h
        db  00Fh, 0B7h, 015h, 0BCh, 004h, 000h, 000h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
loc_4_071B:
        mov    word ptr [ebp + 1ch], 200h
        ret
loc_4_0722:
        xor    ecx, ecx
        mov    eax, ecx
        push   ebx
        push   edi
        db  0A1h, 0F4h, 003h, 000h, 000h
        cmp    dword ptr [eax], ebx
        jne    SHORT loc_4_0736
        call   VFBACKUP_UnLock_NEC
loc_4_0736:
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    ebx, edi
        call   0
        pop    edi
        pop    ebx
        jb     SHORT loc_4_0758
        movzx  eax, word ptr [ebp + 18h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 14h]
        db  0A3h, 0BCh, 004h, 000h, 000h
        ret
loc_4_0758:
        or     word ptr [ebp + 2ch], 1
        ret
loc_4_075E:
        push   edi
        push   ebx
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    ebx, edi
        call   VFBACKUP_UnLock_NEC
        pop    ebx
        pop    edi
        jb     SHORT loc_4_0758
        db  066h, 08Bh, 015h, 0E0h, 003h, 000h, 000h
        mov    al, 0ch
        out    dx, al
        db  0C7h, 005h, 0BCh, 004h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
loc_4_0786:
        ret
_PTEXT ends
        end
