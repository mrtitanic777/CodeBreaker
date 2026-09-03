; dosmgr_pcode_10.asm -- DOSMGR object 10 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_10_0000:
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        call   sub_10_0174
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_10_006A
        cmp    byte ptr [ebp + 1dh], 50h
        je     SHORT loc_10_0074
        db  00Fh, 0B7h, 00Dh, 0D8h, 004h, 000h, 000h
        jecxz  loc_10_006A
        db  08Bh, 035h, 0D0h, 004h, 000h, 000h
        mov    si, word ptr [esi]
        cmp    cx, si
        je     SHORT loc_10_006A
        mov    edx, 5000h
        xchg   dword ptr [ebp + 1ch], edx
        xchg   dword ptr [ebp + 10h], esi
        mov    eax, 21h
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        xchg   dword ptr [ebp + 10h], esi
        db  066h, 089h, 035h, 0D8h, 004h, 000h, 000h
        mov    dword ptr [ebp + 1ch], edx
loc_10_006A:
        cmp    word ptr [ebp + 1ch], 5803h
        je     SHORT loc_10_0080
loc_10_0072:
        stc
        ret
loc_10_0074:
        mov    ax, word ptr [ebp + 10h]
        db  066h, 0A3h, 0D8h, 004h, 000h, 000h
        jmp    SHORT loc_10_0072
loc_10_0080:
        db  083h, 03Dh, 014h, 003h, 000h, 000h, 000h
        je     SHORT loc_10_0072
        db  080h, 03Dh, 020h, 003h, 000h, 000h, 000h
        je     SHORT loc_10_0072
        db  083h, 03Dh, 018h, 003h, 000h, 000h, 000h
        je     SHORT loc_10_0072
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        cmp    dword ptr [edx + ebx + 20h], 0
        je     SHORT loc_10_0072
        cmp    word ptr [ebp + 10h], 0
        je     SHORT loc_10_00D9
        bts    dword ptr [edx + ebx], 0ah
        jb     SHORT loc_10_0072
        test   byte ptr [edx + ebx + 1], 1
        jne    SHORT loc_10_0072
        push   100h
        push   0
        db  0FFh, 035h, 018h, 003h, 000h, 000h
        push   dword ptr [edx + ebx + 20h]
        db  0CDh, 020h, 056h, 000h, 001h, 000h
        add    esp, 10h
        jmp    SHORT loc_10_0072
loc_10_00D9:
        btr    dword ptr [edx + ebx], 0ah
        jae    SHORT loc_10_0072
        test   byte ptr [edx + ebx + 1], 1
        jne    SHORT loc_10_0072
        db  0BEh, 014h, 002h, 000h, 000h
        db  08Bh, 00Dh, 014h, 003h, 000h, 000h
        xor    edi, edi
loc_10_00F4:
        test   word ptr [esi + 4], 100h
        jne    SHORT loc_10_014B
        movzx  eax, word ptr [esi]
        dec    eax
        shl    eax, 4
        add    eax, dword ptr [ebx + 4]
        push   ecx
        movzx  ecx, word ptr [eax + 3]
        cmp    cx, word ptr [esi + 2]
        pop    ecx
        jne    SHORT loc_10_0119
        cmp    word ptr [eax + 1], 0
        je     SHORT loc_10_014B
loc_10_0119:
        bts    word ptr [edx + ebx + 60h], di
        jb     SHORT loc_10_014B
        push   ecx
        push   edx
        movzx  ecx, word ptr [esi + 2]
        inc    ecx
        test   byte ptr [esi + 4], 2
        je     SHORT loc_10_012F
        inc    ecx
loc_10_012F:
        shr    ecx, 8
        sub    eax, dword ptr [ebx + 4]
        shr    eax, 0ch
        push   400h
        push   ecx
        push   eax
        push   ebx
        db  0CDh, 020h, 065h, 000h, 001h, 000h
        add    esp, 10h
        pop    edx
        pop    ecx
loc_10_014B:
        add    esi, 10h
        inc    edi
        dec    ecx
        jne    SHORT loc_10_00F4
        push   100h
        push   0
        db  0FFh, 035h, 018h, 003h, 000h, 000h
        push   dword ptr [edx + ebx + 20h]
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
        jmp    NEAR PTR loc_10_0072
        db  000h, 000h, 000h
sub_10_0174:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        cmp    dword ptr [edx + ebx + 54h], 0
        je     SHORT loc_10_01AB
        or     byte ptr [edx + ebx + 1], 8
        mov    al, byte ptr [ebp + 1dh]
        cmp    al, 70h
        ja     SHORT loc_10_01AB
        cmp    al, 0
        je     SHORT loc_10_01AB
        cmp    al, 4bh
        je     SHORT loc_10_01AB
        cmp    al, 26h
        je     SHORT loc_10_01AB
        db  08Bh, 00Dh, 0FCh, 000h, 000h, 000h
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
loc_10_01AB:
        ret
loc_10_01AC:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
sub_10_01B2:
        movzx  eax, byte ptr [ebp + 1dh]
        cmp    eax, 71h
        ja     NEAR PTR loc_10_025C
        db  00Fh, 0B6h, 090h, 0A6h, 001h, 000h, 000h
        db  08Bh, 014h, 095h, 018h, 002h, 000h, 000h
loc_10_01CD:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        db  00Fh, 085h, 0D3h, 002h, 0F6h, 0FFh
        cmp    byte ptr [ebp + 1dh], 4ch
        db  00Fh, 084h, 0C9h, 002h, 0F6h, 0FFh
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  08Bh, 035h, 0F4h, 001h, 000h, 000h
        test   esi, esi
        db  00Fh, 084h, 0BBh, 002h, 0F6h, 0FFh
        push   edx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
loc_10_0207:
        sub    esp, 8
        push   0
        mov    eax, dword ptr [esi + edi]
        mov    dword ptr [esi + edi], esp
        push   eax
        push   esi
        db  0E8h, 099h, 002h, 0F6h, 0FFh
        pop    esi
        pop    dword ptr [esi + edi]
        test   byte ptr [ebp + 2ch], 1
        jne    SHORT loc_10_0227
loc_10_0223:
        add    esp, 7ch
        ret
loc_10_0227:
        cmp    byte ptr [esp], 0
        je     SHORT loc_10_0223
        mov    edx, esp
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0E8h, 05Ah, 009h, 0F6h, 0FFh
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        cmp    al, 1
        jne    SHORT loc_10_0223
        add    esp, 0ch
        lea    esi, [esp]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        db  08Bh, 035h, 0F4h, 001h, 000h, 000h
        mov    edx, dword ptr [esp + 6ch]
        jmp    SHORT loc_10_0207
loc_10_025C:
        db  0BAh, 010h, 003h, 000h, 000h
        jmp    NEAR PTR loc_10_01CD
loc_10_0266:
        mov    al, byte ptr [ebp + 1ch]
        push   ecx
        push   edi
        mov    ecx, 10h
        db  0BFh, 007h, 005h, 000h, 000h
        repne scasb
        movzx  edx, byte ptr [edi + 0fh]
        pop    edi
        pop    ecx
        db  08Bh, 014h, 095h, 018h, 002h, 000h, 000h
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_10_028A:
        db  0BAh, 061h, 005h, 000h, 000h
        cmp    word ptr [ebp + 10h], 1
        ja     SHORT loc_10_02A1
        je     SHORT loc_10_029B
        sub    edx, 7
loc_10_029B:
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_10_02A1:
        db  0BAh, 010h, 003h, 000h, 000h
        jmp    SHORT loc_10_029B
_PTEXT ends
        end
