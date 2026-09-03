; vserver_lcode_01_01.asm -- VSERVER object 1 segment _LTEXT  (part 1/5)
.386p
_LTEXT segment dword public use32 'LCODE'

        db  0E8h, 017h, 078h, 000h, 000h, 0E9h, 04Ah, 001h, 000h, 000h
loc_1_000A:
        mov    eax, 10002h
        jmp    SHORT loc_1_0033
        db  02Eh, 08Bh, 0C0h
loc_1_0014:
        dec    byte ptr [ecx + 32h]
        jne    SHORT loc_1_002C
        cmp    dword ptr [ecx + 2ah], 0
        je     SHORT loc_1_002C
        push   eax
        lea    eax, [ecx + 2ah]
        call   sub_1_8BC0
        pop    eax
        mov    eax, eax
loc_1_002C:
        sub    ah, ah
        shl    eax, 10h
        mov    al, 1
loc_1_0033:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [esi + 30h]
        call   sub_1_0060
        mov    edx, dword ptr [esi + 20h]
        mov    dword ptr [edx + 5], eax
        mov    eax, dword ptr [esi + 28h]
        mov    byte ptr [eax], 0
        mov    word ptr [eax + 1], 0
        add    eax, 3
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
        db  08Bh, 0C0h
sub_1_0060:
        cmp    al, 1
        jne    SHORT loc_1_0088
        mov    ecx, eax
        shr    ecx, 10h
        cmp    ecx, 6ah
        jae    SHORT loc_1_0073
        cmp    ecx, 64h
        jae    SHORT loc_1_008E
loc_1_0073:
        cmp    ecx, 13h
        jb     SHORT loc_1_0088
        cmp    ecx, 1fh
        jbe    SHORT loc_1_0089
        cmp    byte ptr [edi + 3eh], 5
        jae    SHORT loc_1_0088
        cmp    ecx, 58h
        ja     SHORT loc_1_00A9
loc_1_0088:
        ret
loc_1_0089:
        mov    ax, 3803h
        ret
loc_1_008E:
        cmp    ecx, 65h
        je     SHORT loc_1_00A3
        cmp    ecx, 68h
        je     SHORT loc_1_00CA
        cmp    ecx, 67h
        jne    SHORT loc_1_00B8
        mov    eax, 1a3803h
        ret
loc_1_00A3:
        mov    eax, 1e3803h
        ret
loc_1_00A9:
        cmp    ecx, 6eh
        je     SHORT loc_1_00BE
        cmp    ecx, 70h
        je     SHORT loc_1_00C4
        cmp    ecx, 6ch
        je     SHORT loc_1_00CA
loc_1_00B8:
        mov    eax, 1f3803h
        ret
loc_1_00BE:
        mov    eax, 20001h
        ret
loc_1_00C4:
        mov    eax, 273803h
        ret
loc_1_00CA:
        mov    eax, 153803h
        ret
loc_1_00D0:
        xchg   ecx, eax
        dec    byte ptr [eax + 32h]
        jne    SHORT loc_1_00E4
        cmp    dword ptr [eax + 2ah], 0
        je     SHORT loc_1_00E4
        lea    eax, [eax + 2ah]
        call   sub_1_8BC0
loc_1_00E4:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    dword ptr [esi + 28h], ecx
        movzx  eax, byte ptr [ebx + 1]
        cmp    al, 0ffh
        je     SHORT loc_1_0134
        movzx  edi, word ptr [ebx + 3]
        mov    edx, dword ptr [esi + 20h]
        sub    ecx, edx
        mov    word ptr [ebx + 3], cx
        lea    ebx, [edx + edi]
        mov    dword ptr [esi + 24h], ebx
        mov    ecx, dword ptr [esi + 1ch]
        mov    byte ptr [ecx + 3ah], al
        mov    edi, dword ptr [esi + 30h]
        db  0FFh, 024h, 085h, 0E0h, 01Ch, 001h, 000h
loc_1_0118:
        jmp    NEAR PTR loc_1_82A0
        db  02Eh, 08Bh, 0C0h
loc_1_0120:
        dec    byte ptr [ecx + 32h]
        jne    SHORT loc_1_0134
        cmp    dword ptr [ecx + 2ah], 0
        je     SHORT loc_1_0134
        lea    eax, [ecx + 2ah]
        call   sub_1_8BC0
        nop
loc_1_0134:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    edx, dword ptr [esi + 20h]
        or     byte ptr [edx + 9], 80h
        mov    eax, dword ptr [esi + 28h]
        sub    eax, edx
        mov    ecx, dword ptr [esi + 1ch]
        mov    word ptr [ecx + 10h], ax
        call   sub_1_029C
        mov    eax, eax
loc_1_0154:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    dword ptr [esi + 30h], 0
        db  080h, 03Dh, 054h, 027h, 001h, 000h, 000h
        jne    SHORT loc_1_0118
        cli
        xor    ebx, ebx
loc_1_016D:
        db  087h, 01Dh, 018h, 01Ch, 001h, 000h
        test   ebx, ebx
        je     SHORT loc_1_016D
        db  08Bh, 00Dh, 014h, 01Ch, 001h, 000h
        test   ecx, ecx
        jne    SHORT loc_1_018C
        db  089h, 01Dh, 018h, 01Ch, 001h, 000h
        jmp    NEAR PTR loc_1_8780
loc_1_018C:
        db  0F0h, 0FEh, 00Dh, 0CBh, 01Ch, 001h, 000h
        mov    edx, dword ptr [ecx + 4ch]
        db  089h, 015h, 014h, 01Ch, 001h, 000h
        test   edx, edx
        jne    SHORT loc_1_01A5
        db  0BBh, 0C8h, 01Bh, 001h, 000h
loc_1_01A5:
        db  089h, 01Dh, 018h, 01Ch, 001h, 000h
        sti
        movzx  eax, byte ptr [ecx + 5]
        cmp    al, 3
        sbb    ebx, ebx
        and    eax, ebx
        db  0FFh, 024h, 085h, 038h, 01Ah, 001h, 000h
        db  02Eh, 08Bh, 0C0h
loc_1_01C0:
        mov    dword ptr [esi + 1ch], ecx
        mov    edi, dword ptr [ecx + 48h]
        cmp    byte ptr [ecx + 8], 91h
        je     SHORT loc_1_020C
        mov    al, byte ptr [ecx + 9]
        cmp    al, 5
        je     SHORT loc_1_01EC
        cmp    al, 0ah
        je     SHORT loc_1_01EC
        cmp    al, 18h
        je     SHORT loc_1_01EC
        cmp    al, 8
        je     SHORT loc_1_01EC
        cmp    al, 17h
        je     SHORT loc_1_01E7
        cmp    al, 0fh
        je     SHORT loc_1_01E7
loc_1_01E7:
        jmp    0
loc_1_01EC:
        cmp    byte ptr [edi + 1eh], 2
        jne    SHORT loc_1_0213
        movzx  esi, byte ptr [ecx + 7]
        db  08Bh, 034h, 0B5h, 0B0h, 01Bh, 001h, 000h
        movzx  eax, byte ptr [ecx + 0ah]
        mov    dword ptr [esi + eax*4 + 0f2h], 0
loc_1_020C:
        push   ecx
        call   sub_1_728F
        pop    ecx
loc_1_0213:
        jmp    0
loc_1_0218:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        cmp    byte ptr [ecx + 9], 0
        jne    SHORT loc_1_01C0
        mov    edx, dword ptr [ecx + 0ch]
        lea    ebx, [edx + 20h]
        mov    dword ptr [esi + 24h], ebx
        mov    dword ptr [esi + 28h], ebx
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 3ah], al
loc_1_0236:
        db  0A1h, 020h, 01Ch, 001h, 000h
        cmp    byte ptr [eax], 2
        je     NEAR PTR loc_1_0154
        mov    dword ptr [esi + 1ch], ecx
        mov    edi, dword ptr [ecx + 48h]
        cmp    byte ptr [edi + 1eh], 2
        jne    0
        mov    ax, word ptr [edi + 4ch]
        and    word ptr [edx + 0ah], ax
        mov    dword ptr [esi + 24h], ebx
        mov    dword ptr [esi + 20h], edx
        mov    dword ptr [esi + 30h], edi
        cmp    byte ptr [edx + 4], 2bh
        je     SHORT loc_1_0273
        db  0A1h, 028h, 028h, 001h, 000h
        mov    dword ptr [edi + 34h], eax
loc_1_0273:
        movzx  eax, byte ptr [ecx + 3ah]
        mov    dword ptr [esi + 2ch], 0
        db  0FFh, 024h, 085h, 0E0h, 01Ch, 001h, 000h
loc_1_0285:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    edx, dword ptr [ecx + 0ch]
        mov    ebx, dword ptr [ecx + 28h]
        mov    dword ptr [esi + 28h], ebx
        mov    ebx, dword ptr [ecx + 24h]
        jmp    SHORT loc_1_0236
        db  02Eh, 08Bh, 0C0h
sub_1_029C:
        db  08Bh, 015h, 01Ch, 01Ch, 001h, 000h
        movzx  eax, word ptr [ecx + 10h]
        add    dword ptr [edx], eax
        adc    dword ptr [edx + 4], 0
        db  0A1h, 0C4h, 031h, 001h, 000h
        sub    eax, dword ptr [ecx + 44h]
        add    dword ptr [edx + 10h], eax
        inc    dword ptr [edx + 14h]
        movzx  edx, byte ptr [ecx + 7]
        db  08Bh, 03Ch, 095h, 0B0h, 01Bh, 001h, 000h
        db  0F6h, 082h, 0B8h, 01Ch, 001h, 000h, 008h
        je     SHORT loc_1_030D
        mov    al, byte ptr [edi + 2]
        cmp    byte ptr [edi + 5], al
        ja     SHORT loc_1_030D
        mov    ax, word ptr [edi + 0ah]
        mov    word ptr [ecx + 12h], ax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [ecx + 14h], eax
        mov    byte ptr [ecx + 8], 0c8h
        mov    byte ptr [ecx + 5], 0
        inc    byte ptr [edi + 5]
        push   edi
        call   sub_1_759C
        pop    edi
        test   al, al
        jne    SHORT loc_1_02FB
        ret
loc_1_02FB:
        dec    byte ptr [edi + 5]
        cmp    al, 3
        jne    SHORT loc_1_032F
        movzx  edx, byte ptr [ecx + 7]
        db  080h, 0A2h, 0B8h, 01Ch, 001h, 000h, 0F7h
loc_1_030D:
        mov    byte ptr [ecx + 8], 94h
        mov    byte ptr [ecx + 5], 7eh
        push   edx
        push   edi
        call   sub_1_759C
        pop    edi
        pop    edx
        test   al, al
        jne    SHORT loc_1_032F
        mov    al, byte ptr [edi + 2]
        cmp    byte ptr [edi + 5], al
        jb     NEAR PTR sub_1_7388
        ret
loc_1_032F:
        call   sub_1_781C
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_0338:
        cli
        xor    ecx, ecx
loc_1_033B:
        db  087h, 00Dh, 018h, 01Ch, 001h, 000h
        jecxz  loc_1_033B
        db  08Bh, 015h, 014h, 01Ch, 001h, 000h
        mov    dword ptr [ebx + 4ch], edx
        db  089h, 01Dh, 014h, 01Ch, 001h, 000h
        test   edx, edx
        jne    SHORT loc_1_0358
        mov    ecx, ebx
loc_1_0358:
        db  089h, 00Dh, 018h, 01Ch, 001h, 000h
        sti
        ret
loc_1_0360:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [esi + 30h]
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 2
        mov    ecx, dword ptr [esi + 28h]
        add    ecx, 7
        jmp    NEAR PTR loc_1_00E4
        db  000h, 000h
sub_1_037C:
        and    ebp, 72ffffh
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    NEAR PTR loc_1_0410
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_0410
        mov    al, 3
        test   bp, 10h
        je     SHORT loc_1_03A0
        or     al, 4
loc_1_03A0:
        mov    ah, byte ptr [ebx + 10h]
        and    ah, al
        cmp    al, ah
        je     SHORT loc_1_0410
        test   ah, 4
        je     SHORT loc_1_03B3
        and    ebp, 0fffffff0h
        jmp    SHORT loc_1_0410
loc_1_03B3:
        test   ah, 1
        je     SHORT loc_1_03BD
        and    ebp, 0fh
        jne    SHORT loc_1_0410
loc_1_03BD:
        mov    eax, 40002h
        stc
        ret    4
loc_1_03C6:
        pop    ecx
        ret    4
loc_1_03CA:
        xchg   ebx, eax
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [esi + 30h]
        push   dword ptr [esp + 0ch]
        mov    eax, 10048h
        call   sub_1_19E4
        jb     SHORT loc_1_03F3
        xchg   ebx, eax
        or     byte ptr [ecx + 18h], 10h
        pop    ecx
        pop    edx
        mov    edx, dword ptr [esi + 24h]
        test   edx, edx
        ret    4
loc_1_03F3:
        add    esp, 8
        push   eax
        push   ecx
        push   edx
        mov    edx, ebx
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        stc
        ret    4
loc_1_0410:
        push   ecx
        movzx  esi, byte ptr [edx]
        lea    esi, [edx + esi*2 + 1]
        movzx  eax, word ptr [esi]
        mov    byte ptr [eax + esi + 1], 0
        movzx  eax, byte ptr [ebx + 0ah]
        add    ax, word ptr [esi]
        shr    ch, 1
        adc    esi, 2
        lea    edx, [edi + 50h]
        call   sub_1_1CA4
        jb     SHORT loc_1_03C6
        push   ecx
        mov    ecx, edi
        sub    ecx, eax
        mov    ebx, edx
        mov    eax, eax
loc_1_0440:
        mov    ebx, dword ptr [ebx]
        cmp    ebx, edx
        je     SHORT loc_1_03CA
        test   byte ptr [ebx + 18h], 10h
        je     SHORT loc_1_0440
        test   byte ptr [ebx + 18h], 2
        je     SHORT loc_1_0440
        mov    edi, dword ptr [esp + 0ch]
        cmp    dword ptr [ebx + 14h], edi
        jne    SHORT loc_1_0440
        mov    esi, eax
        mov    edi, dword ptr [ebx + 26h]
        push   ecx
        repe cmpsb
        pop    ecx
        jne    SHORT loc_1_0440
        add    esp, 4
        push   eax
        push   ebx
        xchg   ebp, eax
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    esi, dword ptr [esi + 40h]
        xor    edx, edx
        call   sub_1_9F3C
        pop    ecx
        pop    edx
        pushfd
        push   eax
        push   ecx
        push   edx
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        popfd
        pop    edx
        jb     SHORT loc_1_04D7
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    dword ptr [esi + 2ch], ecx
        mov    edi, dword ptr [esi + 28h]
        cmp    dl, 2dh
        je     SHORT loc_1_04E2
        mov    word ptr [edi + 3], bx
        mov    dword ptr [edi + 5], eax
        mov    eax, dword ptr [ecx + 22h]
        shr    eax, 10h
        mov    word ptr [edi + 0dh], ax
        mov    dword ptr [edi + 9], ebp
        mov    byte ptr [edi], 7
        mov    eax, dword ptr [ecx + 8]
        mov    word ptr [edi + 1], ax
        mov    word ptr [edi + 0fh], 0
        lea    eax, [edi + 11h]
        mov    dword ptr [esi + 28h], eax
        xor    eax, eax
        ret    4
loc_1_04D7:
        sub    ah, ah
        shl    eax, 10h
        mov    al, 1
        stc
        ret    4
loc_1_04E2:
        mov    edx, dword ptr [esi + 24h]
        mov    edx, dword ptr [edx + 1]
        mov    dword ptr [edi + 1], edx
        mov    word ptr [edi + 7], bx
        mov    dword ptr [edi + 9], eax
        mov    eax, dword ptr [ecx + 22h]
        shr    eax, 10h
        mov    word ptr [edi + 11h], ax
        mov    dword ptr [edi + 0dh], ebp
        mov    byte ptr [edi], 0fh
        mov    word ptr [edi + 17h], 2
        mov    eax, dword ptr [ecx + 8]
        mov    word ptr [edi + 5], ax
        xor    eax, eax
        mov    dword ptr [edi + 13h], eax
        mov    dword ptr [edi + 19h], eax
        mov    dword ptr [edi + 1dh], eax
        lea    ecx, [edi + 21h]
        mov    ebx, edi
        ret    4
        db  08Bh, 0C0h
loc_1_0524:
        mov    cl, al
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_0033
        mov    ebx, dword ptr [eax + 0ch]
        push   dword ptr [edx + 18h]
        mov    edx, dword ptr [esi + 24h]
        mov    ax, word ptr [edx + 1]
        test   ax, ax
        js     NEAR PTR loc_1_06BC
        shrd   ebp, eax, 10h
        mov    bp, 1
        cmp    ax, 0ffh
        jne    SHORT loc_1_0570
        mov    ch, 1
        call   sub_1_037C
        jb     NEAR PTR loc_1_0033
        jne    SHORT loc_1_05C8
        jmp    NEAR PTR loc_1_0134
        db  02Eh, 08Bh, 0C0h
loc_1_0570:
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_0595
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_0595
        and    eax, 3
        db  08Ah, 080h, 044h, 01Ah, 001h, 000h
        mov    ah, byte ptr [ebx + 10h]
        and    ah, al
        cmp    al, ah
        jne    NEAR PTR loc_1_06BC
loc_1_0595:
        mov    eax, 10048h
        call   sub_1_19E4
        jb     NEAR PTR loc_1_0033
        movzx  esi, byte ptr [edx]
        lea    esi, [edx + esi*2 + 4]
        movzx  eax, word ptr [esi - 3]
        mov    byte ptr [eax + esi - 2], 0
        movzx  eax, byte ptr [ebx + 0ah]
        add    ax, word ptr [esi - 3]
        call   sub_1_1CA4
        jb     NEAR PTR loc_1_06B0
loc_1_05C8:
        push   eax
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        lea    edi, [ecx + 22h]
        mov    ecx, dword ptr [ecx + 8]
        db  08Ah, 099h, 01Ch, 04Ah, 001h, 000h
        shl    ecx, 10h
        mov    cl, bl
        push   ecx
        mov    ecx, dword ptr [esi + 30h]
        mov    bx, word ptr [ecx + 1ch]
        shl    ebx, 10h
        mov    ecx, dword ptr [esi + 20h]
        mov    bx, word ptr [ecx + 1ah]
        mov    dword ptr [edi + 4], ebx
        xchg   ebp, eax
        movzx  ebx, word ptr [edx + 3]
        mov    bh, byte ptr [ecx + 9]
        shr    bh, 5
        db  022h, 03Dh, 0DDh, 01Ch, 001h, 000h
        sub    ecx, ecx
        mov    dword ptr [edi + 8], ecx
        sub    edx, edx
        mov    esi, dword ptr [esi + 40h]
        call   sub_1_94D4
        pop    edi
        pop    edi
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     SHORT loc_1_067A
        xchg   dword ptr [ecx + 22h], eax
        xchg   dword ptr [ecx + 26h], edi
        mov    ebx, dword ptr [esi + 1ch]
        mov    bl, byte ptr [ebx + 0ah]
        mov    byte ptr [ecx + 34h], bl
        mov    ebx, dword ptr [esi + 28h]
        mov    dword ptr [ebx + 3], eax
        mov    dword ptr [ebx + 7], edi
        sub    ebp, ebp
        xchg   dword ptr [ecx + 2ah], ebp
        mov    dword ptr [ebx + 0bh], ebp
        call   sub_1_1ADC
        jb     NEAR PTR loc_1_0033
        mov    edi, dword ptr [esi + 20h]
        shr    dh, 2
        xor    dh, byte ptr [edi + 9]
        and    dh, 20h
        xor    byte ptr [edi + 9], dh
        mov    byte ptr [ebx], 7
        mov    eax, dword ptr [ecx + 8]
        mov    word ptr [ebx + 1], ax
        mov    word ptr [ebx + 0fh], 0
        lea    eax, [ebx + 11h]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
loc_1_067A:
        push   eax
        push   ecx
        push   edx
        mov    edx, edi
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        sub    ah, ah
        shl    eax, 10h
        mov    al, 1
        mov    edi, eax
loc_1_0699:
        call   sub_1_1AB4
        mov    eax, edi
        cmp    eax, 0ee0001h
        jne    NEAR PTR loc_1_0033
        jmp    NEAR PTR loc_1_83C8
loc_1_06B0:
        xchg   edi, eax
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jmp    SHORT loc_1_0699
loc_1_06BC:
        pop    eax
        mov    eax, 40002h
        jmp    NEAR PTR loc_1_0033
        db  090h
loc_1_06C8:
        mov    cl, al
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_0033
        mov    ebx, dword ptr [eax + 0ch]
        push   dword ptr [edx + 18h]
        mov    edx, dword ptr [esi + 24h]
        cmp    byte ptr [ebx + 9], 1
        je     NEAR PTR loc_1_0859
        mov    ebp, 20010h
        cmp    cl, 0fh
        je     SHORT loc_1_06FA
        mov    bp, 12h
loc_1_06FA:
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_0720
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_0720
        mov    al, 7
        and    al, byte ptr [ebx + 10h]
        cmp    al, 7
        je     SHORT loc_1_0720
        test   al, 4
        je     NEAR PTR loc_1_084E
        mov    bp, 10h
        mov    eax, eax
loc_1_0720:
        mov    eax, 10048h
        call   sub_1_19E4
        jb     NEAR PTR loc_1_0033
        movzx  esi, byte ptr [edx]
        lea    esi, [edx + esi*2 + 4]
        movzx  eax, word ptr [esi - 3]
        mov    byte ptr [eax + esi - 2], 0
        movzx  eax, byte ptr [ebx + 0ah]
        add    ax, word ptr [esi - 3]
        call   sub_1_1CA4
        jb     NEAR PTR loc_1_0842
loc_1_0753:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [esi + 2ch]
        add    edi, 22h
        mov    dword ptr [edi + 8], ecx
        mov    ecx, dword ptr [esi + 30h]
        mov    bx, word ptr [ecx + 1ch]
        shl    ebx, 10h
        mov    ecx, dword ptr [esi + 20h]
        mov    bx, word ptr [ecx + 1ah]
        mov    dword ptr [edi + 4], ebx
        xchg   ebp, eax
        mov    ecx, dword ptr [edx + 3]
        test   ecx, ecx
        jne    SHORT loc_1_0784
        db  08Bh, 00Dh, 028h, 028h, 001h, 000h
loc_1_0784:
        mov    ebx, dword ptr [edx - 1]
        mov    bx, 6
        sub    edx, edx
        push   ebp
        mov    esi, dword ptr [esi + 40h]
        call   sub_1_94D4
        pop    edi
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    byte ptr [esi + 9], 0
        mov    ecx, dword ptr [esi + 2ch]
        jb     SHORT loc_1_07EC
        mov    dword ptr [ecx + 22h], eax
        mov    dword ptr [ecx + 26h], edi
        mov    dword ptr [ecx + 2ah], 0
        mov    ebx, dword ptr [esi + 1ch]
        mov    al, byte ptr [ebx + 0ah]
        mov    byte ptr [ecx + 34h], al
        call   sub_1_1ADC
        jb     NEAR PTR loc_1_0033
        mov    ebx, dword ptr [esi + 20h]
        and    byte ptr [ebx + 9], 0dfh
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 1
        mov    eax, dword ptr [ecx + 8]
        mov    word ptr [ebx + 1], ax
        mov    word ptr [ebx + 3], 0
        lea    eax, [ebx + 5]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
loc_1_07EC:
        push   eax
        push   ecx
        push   edx
        mov    edx, edi
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        sub    ah, ah
        shl    eax, 10h
        mov    al, 1
        mov    edi, eax
        cmp    dword ptr [ecx + 1ah], 0
        je     SHORT loc_1_0816
        call   sub_1_9449
loc_1_0816:
        call   sub_1_1AB4
        mov    eax, edi
        cmp    eax, 6e0001h
        jne    SHORT loc_1_0832
        mov    edx, dword ptr [esi + 30h]
        cmp    byte ptr [edx + 3eh], 5
        jae    SHORT loc_1_0832
        mov    eax, 500001h
loc_1_0832:
        cmp    eax, 0ee0001h
        jne    NEAR PTR loc_1_0033
        jmp    NEAR PTR loc_1_83C8
loc_1_0842:
        xchg   edi, eax
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jmp    SHORT loc_1_0816
loc_1_084E:
        pop    eax
        mov    eax, 40002h
        jmp    NEAR PTR loc_1_0033
loc_1_0859:
        mov    ebp, 10012h
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_0873
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_0873
        test   byte ptr [ebx + 10h], 4
        je     SHORT loc_1_084E
loc_1_0873:
        mov    eax, 10048h
        call   sub_1_19E4
        jb     NEAR PTR loc_1_0033
        and    ebp, 0ff8fffffh
        or     ebp, 200000h
        call   sub_1_931B
        jae    NEAR PTR loc_1_0753
        mov    edi, eax
        jmp    NEAR PTR loc_1_0816
        db  02Eh, 08Bh, 0C0h
loc_1_08A4:
        mov    cl, al
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_0033
        mov    ebx, dword ptr [eax + 0ch]
        push   dword ptr [edx + 18h]
        mov    edx, dword ptr [esi + 24h]
        cmp    byte ptr [ebx + 9], 1
        je     SHORT loc_1_08F7
        mov    ax, word ptr [edx + 7]
        test   ax, ax
        js     NEAR PTR loc_1_0ABE
        shrd   ebp, eax, 10h
        mov    bp, word ptr [edx + 11h]
        cmp    ax, 0ffh
        jne    SHORT loc_1_093E
        mov    ch, 0
        call   sub_1_037C
        jb     NEAR PTR loc_1_0033
        jne    NEAR PTR loc_1_09A7
        jmp    NEAR PTR loc_1_00E4
loc_1_08F7:
        mov    ebp, 10012h
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_0915
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_0915
        test   byte ptr [ebx + 10h], 4
        je     NEAR PTR loc_1_0ABE
loc_1_0915:
        mov    eax, 10048h
        call   sub_1_19E4
        jb     NEAR PTR loc_1_0033
        and    ebp, 0ff8fffffh
        or     ebp, 200000h
        call   sub_1_931B
        jae    SHORT loc_1_09A7
        push   eax
        jmp    NEAR PTR loc_1_0A9C
loc_1_093E:
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_0974
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_0974
        and    eax, 3
        db  08Ah, 080h, 044h, 01Ah, 001h, 000h
        test   bp, 10h
        je     SHORT loc_1_095F
        or     al, 4
loc_1_095F:
        mov    ah, byte ptr [ebx + 10h]
        and    ah, al
        cmp    al, ah
        je     SHORT loc_1_0974
        test   ah, 4
        je     NEAR PTR loc_1_0ABE
        and    ebp, 0fffffff0h
loc_1_0974:
        mov    eax, 10048h
        call   sub_1_19E4
        jb     NEAR PTR loc_1_0033
        movzx  esi, byte ptr [edx]
        lea    esi, [edx + esi*2 + 3]
        movzx  eax, word ptr [esi - 2]
        mov    byte ptr [eax + esi - 1], 0
        movzx  eax, byte ptr [ebx + 0ah]
        add    ax, word ptr [esi - 2]
        call   sub_1_1CA4
        jb     NEAR PTR loc_1_0AB2
loc_1_09A7:
        push   eax
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ebx, dword ptr [esi + 2ch]
        lea    edi, [ebx + 22h]
        mov    dword ptr [edi + 8], ecx
        mov    ecx, dword ptr [ebx + 8]
        db  08Ah, 099h, 01Ch, 04Ah, 001h, 000h
        shl    ecx, 10h
        mov    cl, bl
        push   ecx
        mov    ecx, dword ptr [esi + 30h]
        mov    bx, word ptr [ecx + 1ch]
        shl    ebx, 10h
        mov    ecx, dword ptr [esi + 20h]
        mov    bx, word ptr [ecx + 1ah]
        mov    dword ptr [edi + 4], ebx
        xchg   ebp, eax
        mov    ebx, dword ptr [edx + 9]
        mov    bh, byte ptr [edx + 5]
        shr    bh, 1
        db  022h, 03Dh, 0DDh, 01Ch, 001h, 000h
        mov    ecx, dword ptr [edx + 0dh]
        test   ecx, ecx
        jne    SHORT loc_1_09F6
        db  08Bh, 00Dh, 028h, 028h, 001h, 000h
loc_1_09F6:
        mov    edx, dword ptr [edx + 13h]
        mov    esi, dword ptr [esi + 40h]
        call   sub_1_94D4
        pop    esi
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    byte ptr [esi + 9], 0
        mov    ecx, dword ptr [esi + 2ch]
        jb     SHORT loc_1_0A71
        xchg   dword ptr [ecx + 22h], eax
        pop    ebp
        xchg   dword ptr [ecx + 26h], ebp
        mov    ebx, dword ptr [esi + 1ch]
        mov    bl, byte ptr [ebx + 0ah]
        mov    byte ptr [ecx + 34h], bl
        mov    ebx, dword ptr [esi + 28h]
        mov    edi, dword ptr [esi + 24h]
        mov    edi, dword ptr [edi + 1]
        mov    dword ptr [ebx + 1], edi
        mov    dword ptr [ebx + 7], eax
        mov    dword ptr [ebx + 0bh], ebp
        sub    ebp, ebp
        xchg   dword ptr [ecx + 2ah], ebp
        mov    dword ptr [ebx + 0fh], ebp
        call   sub_1_1ADC
        jb     NEAR PTR loc_1_0033
        mov    byte ptr [ebx], 0fh
        mov    eax, dword ptr [ecx + 8]
        mov    word ptr [ebx + 5], ax
        mov    word ptr [ebx + 17h], dx
        sub    eax, eax
        mov    word ptr [ebx + 13h], ax
        mov    word ptr [ebx + 15h], ax
        mov    dword ptr [ebx + 19h], eax
        mov    word ptr [ebx + 1dh], ax
        mov    word ptr [ebx + 1fh], ax
        lea    ecx, [ebx + 21h]
        jmp    NEAR PTR loc_1_00E4
loc_1_0A71:
        sub    ah, ah
        shl    eax, 10h
        mov    al, 1
        xchg   dword ptr [esp], eax
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        cmp    dword ptr [ecx + 1ah], 0
        je     SHORT loc_1_0A9C
        call   sub_1_9449
loc_1_0A9C:
        call   sub_1_1AB4
        pop    eax
        cmp    eax, 0ee0001h
        jne    NEAR PTR loc_1_0033
        jmp    NEAR PTR loc_1_83C8
loc_1_0AB2:
        push   eax
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jmp    SHORT loc_1_0A9C
loc_1_0ABE:
        pop    eax
        mov    eax, 40002h
        jmp    NEAR PTR loc_1_0033
        db  02Eh, 08Bh, 0C0h
loc_1_0ACC:
        mov    ebx, dword ptr [esi + 24h]
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 1
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        and    byte ptr [ecx + 18h], 0fdh
        call   sub_1_1B8C
loc_1_0AE9:
        cmp    byte ptr [ecx + 32h], 0
        jne    SHORT loc_1_0B2F
        mov    eax, dword ptr [ecx + 22h]
        mov    ecx, dword ptr [ebx + 3]
        sub    ebp, ebp
        call   sub_1_96A4
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     SHORT loc_1_0B42
        cmp    dword ptr [ecx + 1ah], 0
        jne    SHORT loc_1_0B39
loc_1_0B0D:
        cmp    byte ptr [ecx + 33h], 0
        jne    SHORT loc_1_0B62
        call   sub_1_1AB4
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0
        mov    word ptr [ebx + 1], 0
        lea    eax, [ebx + 3]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
loc_1_0B2F:
        lea    eax, [ecx + 2ah]
        call   sub_1_875C
        jmp    SHORT loc_1_0AE9
loc_1_0B39:
        call   sub_1_948C
        jae    SHORT loc_1_0B0D
        jmp    SHORT loc_1_0B53
loc_1_0B42:
        cmp    dword ptr [ecx + 1ah], 0
        je     SHORT loc_1_0B5C
        push   eax
        call   sub_1_948C
        jae    SHORT loc_1_0B5B
        add    esp, 4
loc_1_0B53:
        cmp    byte ptr [ecx + 33h], 0
        jne    SHORT loc_1_0B62
        jmp    SHORT loc_1_0B6C
loc_1_0B5B:
        pop    eax
loc_1_0B5C:
        cmp    byte ptr [ecx + 33h], 0
        je     SHORT loc_1_0B65
loc_1_0B62:
        mov    al, byte ptr [ecx + 33h]
loc_1_0B65:
        sub    ah, ah
        shl    eax, 10h
        mov    al, 1
loc_1_0B6C:
        push   eax
        call   sub_1_1AB4
        pop    eax
        jmp    NEAR PTR loc_1_0033
loc_1_0B78:
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 0
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        inc    byte ptr [ecx + 32h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    di, word ptr [edx + 1ah]
loc_1_0B97:
        mov    eax, dword ptr [ecx + 22h]
        mov    ebp, dword ptr [esi + 28h]
        add    ebp, 10h
        movzx  ecx, word ptr [ebx + 3]
        mov    edx, dword ptr [esi + 1ch]
        movzx  edx, byte ptr [edx + 7]
        db  08Bh, 014h, 095h, 0B0h, 01Bh, 001h, 000h
        movzx  edx, word ptr [edx + 0ah]
        add    edx, dword ptr [esi + 20h]
        sub    edx, ebp
        cmp    ecx, edx
        ja     SHORT loc_1_0C07
        mov    edx, dword ptr [ebx + 5]
        mov    ebx, ecx
        mov    esi, ebp
        call   sub_1_9796
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     NEAR PTR loc_1_0014
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 5
        mov    word ptr [ebx + 1], dx
        mov    byte ptr [ebx + 0dh], 1
        mov    word ptr [ebx + 0eh], dx
        add    edx, 3
        mov    word ptr [ebx + 0bh], dx
        lea    eax, [edx + ebx + 0dh]
        mov    dword ptr [esi + 28h], eax
        sub    eax, eax
        mov    dword ptr [ebx + 3], eax
        mov    dword ptr [ebx + 7], eax
        jmp    NEAR PTR loc_1_0120
loc_1_0C07:
        mov    ecx, dword ptr [esi + 2ch]
        mov    al, 57h
        jmp    NEAR PTR loc_1_0014
        db  02Eh, 08Bh, 0C0h
loc_1_0C14:
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 0
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        inc    byte ptr [ecx + 32h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    di, word ptr [edx + 1ah]
        mov    dword ptr [esi + 34h], edi
        mov    eax, dword ptr [ecx + 22h]
        mov    ecx, dword ptr [esi + 1ch]
        cmp    byte ptr [ecx + 5], 2
        je     SHORT loc_1_0CA2
        mov    esi, ecx
        movzx  ecx, word ptr [ebx + 3]
        mov    ebx, dword ptr [ebx + 5]
        mov    dl, 0
        xor    esi, esi
        call   sub_1_9984
        jb     SHORT loc_1_0C81
loc_1_0C56:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        mov    edi, dword ptr [esi + 34h]
        mov    ebx, dword ptr [esi + 24h]
        jb     SHORT loc_1_0C72
        mov    word ptr [ecx + 46h], 0
        jmp    NEAR PTR loc_1_0B97
loc_1_0C72:
        cmp    al, 21h
        je     SHORT loc_1_0C7C
        mov    word ptr [ecx + 46h], 0
loc_1_0C7C:
        jmp    NEAR PTR loc_1_0014
loc_1_0C81:
        cmp    al, 0aah
        stc
        jne    SHORT loc_1_0C56
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    eax, dword ptr [esi + 2ch]
        mov    ebx, dword ptr [esi + 24h]
        mov    edi, dword ptr [ebx + 5]
        xor    ebp, ebp
        call   sub_1_24CC
        jae    NEAR PTR loc_1_0154
loc_1_0CA2:
        mov    al, 21h
        cmp    byte ptr [ecx + 6], 1
        cmc
        jmp    SHORT loc_1_0C56
        db  090h
loc_1_0CAC:
        movzx  ecx, word ptr [ebx + 5]
        mov    al, 0
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        inc    byte ptr [ecx + 32h]
        movzx  eax, byte ptr [edi + 3fh]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    di, word ptr [edx + 1ah]
        mov    esi, dword ptr [esi + 28h]
        mov    dword ptr [esi + 1], 0ffh
        add    esi, 1eh
        and    esi, 0fffffffch
        db  08Bh, 004h, 085h, 0B0h, 01Bh, 001h, 000h
        movzx  eax, word ptr [eax + 0ah]
        lea    ebp, [edx + eax]
        sub    ebp, esi
        mov    edx, dword ptr [ebx + 7]
        movzx  ebx, word ptr [ebx + 0bh]
        mov    eax, dword ptr [ecx + 22h]
        cmp    ebx, ebp
        ja     SHORT loc_1_0D52
        mov    ecx, ebx
        call   sub_1_9796
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     NEAR PTR loc_1_0014
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0ch
        mov    word ptr [ebx + 0bh], dx
        lea    eax, [ebx + 1eh]
        mov    ebp, eax
        and    ebp, 0fffffffch
        sub    ebp, dword ptr [esi + 20h]
        mov    word ptr [ebx + 0dh], bp
        not    eax
        and    eax, 3
        add    edx, eax
        mov    word ptr [ebx + 19h], dx
        lea    eax, [edx + ebx + 1bh]
        mov    dword ptr [esi + 28h], eax
        sub    eax, eax
        mov    dword ptr [ebx + 7], eax
        mov    word ptr [ebx + 0fh], ax
        mov    dword ptr [ebx + 11h], eax
        mov    dword ptr [ebx + 15h], eax
        jmp    NEAR PTR loc_1_0120
loc_1_0D52:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        mov    al, 57h
        jmp    NEAR PTR loc_1_0014
loc_1_0D62:
        jmp    NEAR PTR loc_1_0033
        db  090h
loc_1_0D68:
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 0
        call   sub_1_18B8
        jb     SHORT loc_1_0D62
        inc    byte ptr [ecx + 32h]
        or     byte ptr [edx + 9], 80h
        push   dword ptr [ecx + 22h]
        push   dword ptr [ebx + 3]
        movzx  eax, word ptr [ebx + 7]
        push   eax
        mov    ax, word ptr [edi + 1ch]
        shl    eax, 10h
        mov    ax, word ptr [edx + 1ah]
        push   eax
        movzx  eax, byte ptr [edi + 3fh]
        db  08Bh, 004h, 085h, 0B0h, 01Bh, 001h, 000h
        movzx  eax, word ptr [eax + 0ah]
        movzx  ebx, word ptr [edi + 48h]
        sub    eax, ebx
        sbb    ebp, ebp
        and    eax, ebp
        add    eax, ebx
        and    eax, 0fffffffch
        mov    ebx, dword ptr [esi + 1ch]
        mov    byte ptr [ebx + 8], 94h
        mov    byte ptr [ebx + 5], 7fh
        mov    word ptr [ebx + 10h], ax
        mov    ebx, dword ptr [esi + 28h]
        lea    edi, [ebx + 16h]
        and    edi, 0fffffffch
        push   edi
        sub    edi, edx
        mov    ecx, eax
        sub    eax, edi
        push   eax
        mov    ebp, esp
        mov    byte ptr [ebx], 8
        mov    word ptr [ebx + 0fh], di
        mov    word ptr [ebx + 0dh], ax
        sub    edx, ebx
        add    ecx, edx
        sub    ecx, 13h
        mov    word ptr [ebx + 11h], cx
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [ebx + 5], ax
        mov    word ptr [ebx + 7], 0ffffh
loc_1_0DF6:
        push   ebp
        mov    ebx, dword ptr [ebp]
        mov    ecx, dword ptr [ebp + 0ch]
        sub    ebx, ecx
        sbb    eax, eax
        and    ebx, eax
        add    ebx, ecx
        mov    edx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 4]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 14h]
        call   sub_1_9796
        pop    ebp
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        jb     SHORT loc_1_0E71
        mov    ebx, dword ptr [esi + 28h]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebx + 1], eax
        sub    dword ptr [ebp + 0ch], edx
        je     SHORT loc_1_0E7C
        cmp    edx, dword ptr [ebp]
        jb     SHORT loc_1_0E7C
        add    eax, edx
        mov    dword ptr [ebp + 10h], eax
        mov    ecx, dword ptr [esi + 1ch]
        db  08Bh, 01Dh, 01Ch, 01Ch, 001h, 000h
        movzx  eax, word ptr [ecx + 10h]
        add    dword ptr [ebx], eax
        adc    dword ptr [ebx + 4], 0
        mov    dword ptr [ecx + 48h], 0
        or     byte ptr [ecx + 0bh], 80h
        push   ebp
        call   sub_1_759C
        call   sub_1_766C
        pop    ebp
        and    byte ptr [ecx + 0bh], 7fh
        test   al, al
        je     SHORT loc_1_0DF6
        mov    al, 1
        db  08Bh, 035h, 014h, 028h, 001h, 000h
loc_1_0E71:
        add    esp, 18h
        mov    ecx, dword ptr [esi + 2ch]
        jmp    NEAR PTR loc_1_0014
loc_1_0E7C:
        mov    word ptr [ebx + 0dh], dx
        mov    eax, dword ptr [ebp + 4]
        add    eax, edx
        mov    dword ptr [esi + 28h], eax
        sub    edx, dword ptr [ebp]
        add    word ptr [ebx + 11h], dx
        mov    ecx, dword ptr [ebp + 0ch]
        sub    word ptr [ebx + 5], cx
        mov    ecx, dword ptr [esi + 2ch]
        add    esp, 18h
        jmp    NEAR PTR loc_1_0120
loc_1_0EA1:
        mov    ecx, dword ptr [esi + 1ch]
loc_1_0EA4:
        push   edi
        call   sub_1_029C
        pop    edi
        dec    byte ptr [edi + 41h]
        jne    SHORT loc_1_0EC0
        cmp    word ptr [edi + 46h], 0
        je     NEAR PTR loc_1_0154
        call   sub_1_851D
loc_1_0EC0:
        jmp    NEAR PTR loc_1_0154
        db  02Eh, 08Bh, 0C0h
loc_1_0EC8:
        mov    word ptr [ecx + 10h], 0
        inc    byte ptr [edi + 41h]
        cmp    word ptr [edi + 46h], 0
        jne    SHORT loc_1_0EA4
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 2
        call   sub_1_18B8
        jb     SHORT loc_1_0EA1
        inc    byte ptr [ecx + 32h]
        movzx  eax, byte ptr [edi + 3fh]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    di, word ptr [edx + 1ah]
        mov    edx, ebx
        db  0F6h, 080h, 0B8h, 01Ch, 001h, 000h, 001h
        je     SHORT loc_1_0F76
        db  08Bh, 004h, 085h, 0B0h, 01Bh, 001h, 000h
        movzx  ebx, word ptr [edx + 7]
        cmp    bx, word ptr [eax + 0ah]
        jae    SHORT loc_1_0F7A
        mov    esi, dword ptr [esi + 20h]
        mov    eax, dword ptr [ecx + 22h]
        mov    edx, dword ptr [edx + 3]
        mov    ecx, ebx
        call   sub_1_9796
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        jb     SHORT loc_1_0F70
loc_1_0F2B:
        mov    ecx, dword ptr [esi + 1ch]
        mov    word ptr [ecx + 10h], dx
        call   sub_1_029C
loc_1_0F37:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        dec    byte ptr [ecx + 32h]
        jne    SHORT loc_1_0F53
        cmp    dword ptr [ecx + 2ah], 0
        je     SHORT loc_1_0F53
        lea    eax, [ecx + 2ah]
        call   sub_1_8BC0
loc_1_0F53:
        mov    edi, dword ptr [esi + 30h]
        dec    byte ptr [edi + 41h]
        jne    SHORT loc_1_0F6B
        cmp    word ptr [edi + 46h], 0
        je     NEAR PTR loc_1_0154
        call   sub_1_851D
loc_1_0F6B:
        jmp    NEAR PTR loc_1_0154
loc_1_0F70:
        mov    ecx, dword ptr [esi + 2ch]
        mov    byte ptr [ecx + 33h], al
loc_1_0F76:
        sub    edx, edx
        jmp    SHORT loc_1_0F2B
loc_1_0F7A:
        mov    al, byte ptr [eax]
        mov    ecx, 1
        mov    dword ptr [edx + 18h], ecx
        call   sub_1_78C0
        jb     SHORT loc_1_0F76
        mov    dword ptr [edx + 14h], ebx
        mov    dword ptr [edx + 1ch], 0
        movzx  eax, word ptr [edx + 7]
        call   sub_1_7879
        jb     NEAR PTR loc_1_1026
        mov    dword ptr [edx + 20h], eax
        mov    dword ptr [edx + 1ch], ecx
        mov    ecx, dword ptr [esi + 2ch]
        mov    esi, eax
        mov    eax, dword ptr [ecx + 22h]
        movzx  ebx, word ptr [edx + 7]
        mov    ecx, ebx
        mov    edx, dword ptr [edx + 3]
        call   sub_1_9796
        jb     SHORT loc_1_1028
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [esi + 1ch]
        db  0A1h, 01Ch, 01Ch, 001h, 000h
        add    dword ptr [eax], edx
        adc    dword ptr [eax + 4], 0
        db  08Bh, 02Dh, 0C4h, 031h, 001h, 000h
        sub    ebp, dword ptr [edi + 44h]
        add    dword ptr [eax + 10h], ebp
        inc    dword ptr [eax + 14h]
        mov    ebx, dword ptr [esi + 24h]
        mov    ecx, dword ptr [ebx + 14h]
        mov    word ptr [ecx + 10h], dx
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [ecx + 0ch], eax
        mov    byte ptr [ecx + 8], 94h
        mov    al, byte ptr [edi + 0ah]
        mov    byte ptr [ecx + 0ah], al
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [ecx + 58h], eax
        mov    byte ptr [ecx + 6], 2
        db  0C7h, 041h, 04Ch, 074h, 010h, 000h, 000h
        call   sub_1_75C0
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        test   al, al
        je     SHORT loc_1_1059
        call   sub_1_1074
        jmp    SHORT loc_1_1059
loc_1_1026:
        sub    al, al
loc_1_1028:
        cmp    al, 8
        je     SHORT loc_1_1026
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        mov    byte ptr [ecx + 33h], al
        mov    edi, dword ptr [esi + 24h]
        mov    ecx, dword ptr [edi + 18h]
        mov    ebx, dword ptr [edi + 14h]
        call   sub_1_78E4
        mov    eax, dword ptr [edi + 1ch]
        test   eax, eax
        je     SHORT loc_1_1052
        call   sub_1_78A8
loc_1_1052:
        sub    edx, edx
        jmp    NEAR PTR loc_1_0F2B
loc_1_1059:
        mov    ecx, dword ptr [esi + 1ch]
        movzx  edx, byte ptr [ecx + 7]
        db  08Bh, 03Ch, 095h, 0B0h, 01Bh, 001h, 000h
        call   sub_1_7391
        jmp    NEAR PTR loc_1_0F37
        db  02Eh, 08Bh, 0C0h
sub_1_1074:
        mov    eax, dword ptr [ecx + 58h]
        call   sub_1_78A8
        mov    ebx, ecx
        mov    ecx, 1
        call   sub_1_78E4
        ret
        db  000h, 000h, 000h
loc_1_108C:
        call   sub_1_96E4
        mov    ebx, 0
        jmp    SHORT loc_1_10D3
loc_1_1098:
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 0
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        inc    byte ptr [ecx + 32h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    di, word ptr [edx + 1ah]
        movzx  esi, byte ptr [ebx]
        lea    esi, [ebx + esi*2 + 6]
        mov    eax, dword ptr [ecx + 22h]
        mov    edx, dword ptr [ebx + 5]
        movzx  ecx, word ptr [ebx + 3]
        jecxz  loc_1_108C
        mov    ebx, ecx
        sub    ebp, ebp
        call   sub_1_97AB
loc_1_10D3:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     NEAR PTR loc_1_0014
        mov    eax, dword ptr [esi + 28h]
        mov    byte ptr [eax], 1
        mov    word ptr [eax + 1], bx
        mov    word ptr [eax + 3], 0
        add    eax, 5
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0120
        db  02Eh, 08Bh, 0C0h
loc_1_1100:
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 0
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        inc    byte ptr [ecx + 32h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    di, word ptr [edx + 1ah]
        mov    dword ptr [esi + 34h], edi
        movzx  esi, byte ptr [ebx]
        lea    esi, [ebx + esi*2 + 6]
        mov    eax, dword ptr [ecx + 22h]
        mov    edx, dword ptr [ebx + 5]
        movzx  ebx, word ptr [ebx + 3]
        mov    ecx, ebx
        sub    ebp, ebp
        push   ebx
        push   edx
        call   sub_1_97AB
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     SHORT loc_1_1183
        mov    eax, dword ptr [esi + 28h]
        mov    byte ptr [eax], 1
        mov    word ptr [eax + 1], bx
        mov    word ptr [eax + 3], 0
        mov    edi, dword ptr [esi + 34h]
        mov    eax, dword ptr [ecx + 22h]
        pop    ebx
        pop    ecx
        call   sub_1_997C
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     NEAR PTR loc_1_0014
        mov    eax, dword ptr [esi + 28h]
        add    eax, 5
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0120
loc_1_1183:
        add    esp, 8
        jmp    NEAR PTR loc_1_0014
        db  090h
loc_1_118C:
        movzx  ecx, word ptr [ebx + 5]
        mov    al, 0
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        inc    byte ptr [ecx + 32h]
        mov    esi, dword ptr [esi + 28h]
        mov    eax, dword ptr [ebx + 1]
        mov    dword ptr [esi + 1], eax
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    di, word ptr [edx + 1ah]
        movzx  esi, word ptr [ebx + 17h]
        add    esi, edx
        mov    eax, dword ptr [ecx + 22h]
        mov    edx, dword ptr [ebx + 7]
        movzx  ebp, word ptr [ebx + 0fh]
        and    ebp, 1
        shl    ebp, 0eh
        movzx  ebx, word ptr [ebx + 15h]
        mov    ecx, ebx
        call   sub_1_97AB
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     NEAR PTR loc_1_0014
        mov    eax, dword ptr [esi + 28h]
        mov    byte ptr [eax], 6
        mov    word ptr [eax + 5], bx
        xor    ebx, ebx
        mov    word ptr [eax + 7], bx
        mov    dword ptr [eax + 0bh], ebx
        mov    word ptr [eax + 9], bx
        mov    ebx, eax
        add    eax, 0fh
        jmp    NEAR PTR loc_1_00D0
        db  02Eh, 08Bh, 0C0h
loc_1_1208:
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 1
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        and    byte ptr [ecx + 18h], 0fdh
        call   sub_1_1B8C
        cmp    byte ptr [ecx + 33h], 0
        jne    SHORT loc_1_125D
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    di, word ptr [edx + 1ah]
        movzx  esi, byte ptr [ebx]
        lea    esi, [ebx + esi*2 + 4]
        mov    eax, dword ptr [ecx + 22h]
        mov    edx, dword ptr [ebx + 5]
        movzx  ebx, word ptr [ebx + 3]
        mov    ecx, ebx
        sub    ebp, ebp
        call   sub_1_97AB
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        sbb    ah, ah
        and    al, ah
        mov    byte ptr [ecx + 33h], al
loc_1_125D:
        mov    eax, dword ptr [esi + 28h]
        mov    byte ptr [eax], 1
        mov    word ptr [eax + 1], bx
        mov    word ptr [eax + 3], 0
        mov    ebx, dword ptr [esi + 24h]
loc_1_1270:
        cmp    byte ptr [ecx + 32h], 0
        jne    SHORT loc_1_12AD
        mov    eax, dword ptr [ecx + 22h]
        mov    ecx, dword ptr [ebx + 9]
        sub    ebp, ebp
        call   sub_1_96A4
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     SHORT loc_1_12B7
        cmp    dword ptr [ecx + 1ah], 0
        jne    SHORT loc_1_12EB
loc_1_1294:
        cmp    byte ptr [ecx + 33h], 0
        jne    SHORT loc_1_12D7
        call   sub_1_1AB4
        mov    eax, dword ptr [esi + 28h]
        add    eax, 5
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
loc_1_12AD:
        lea    eax, [ecx + 2ah]
        call   sub_1_875C
        jmp    SHORT loc_1_1270
loc_1_12B7:
        cmp    dword ptr [ecx + 1ah], 0
        je     SHORT loc_1_12D1
        push   eax
        call   sub_1_948C
        jae    SHORT loc_1_12D0
        add    esp, 4
loc_1_12C8:
        cmp    byte ptr [ecx + 33h], 0
        jne    SHORT loc_1_12D7
        jmp    SHORT loc_1_12E1
loc_1_12D0:
        pop    eax
loc_1_12D1:
        cmp    byte ptr [ecx + 33h], 0
        je     SHORT loc_1_12DA
loc_1_12D7:
        mov    al, byte ptr [ecx + 33h]
loc_1_12DA:
        sub    ah, ah
        shl    eax, 10h
        mov    al, 1
loc_1_12E1:
        call   sub_1_1AB4
        jmp    NEAR PTR loc_1_0033
loc_1_12EB:
        call   sub_1_948C
        jae    SHORT loc_1_1294
        jmp    SHORT loc_1_12C8
loc_1_12F4:
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 0
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        inc    byte ptr [ecx + 32h]
        mov    ecx, dword ptr [esi + 1ch]
        movzx  eax, byte ptr [ecx + 7]
        db  0F6h, 080h, 0B8h, 01Ch, 001h, 000h, 001h
        je     NEAR PTR loc_1_14ED
        mov    ecx, 1
        call   sub_1_78C0
        jb     NEAR PTR loc_1_14ED
        mov    dword ptr [esi + 34h], ebx
        mov    ecx, dword ptr [esi + 1ch]
        mov    al, byte ptr [ecx + 0ah]
        mov    byte ptr [ebx + 0ah], al
        mov    ecx, dword ptr [esi + 24h]
        movzx  eax, word ptr [ecx + 3]
        mov    dword ptr [ecx + 0bh], eax
        sub    ax, word ptr [ecx + 15h]
        jbe    NEAR PTR loc_1_14DD
        mov    word ptr [ebx + 10h], ax
        call   sub_1_7879
        jb     NEAR PTR loc_1_14DD
        push   ecx
        mov    ecx, dword ptr [esi + 34h]
        mov    dword ptr [ecx + 0ch], eax
        pop    dword ptr [ecx + 58h]
        mov    byte ptr [ecx + 8], 95h
        call   sub_1_155C
        mov    ebx, dword ptr [esi + 24h]
        mov    ecx, dword ptr [esi + 2ch]
        mov    edx, dword ptr [esi + 20h]
        mov    edi, dword ptr [esi + 30h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    di, word ptr [edx + 1ah]
        movzx  esi, word ptr [ebx + 17h]
        add    esi, edx
        mov    eax, dword ptr [ecx + 22h]
        mov    edx, dword ptr [ebx + 7]
        movzx  ebp, word ptr [ebx + 0fh]
        and    ebp, 1
        shl    ebp, 0eh
        mov    ecx, dword ptr [ebx + 0bh]
        movzx  ebx, word ptr [ebx + 15h]
        test   ebx, ebx
        je     SHORT loc_1_13AB
        call   sub_1_97AB
loc_1_13AB:
        sbb    ah, ah
        and    al, ah
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        mov    byte ptr [ecx + 33h], al
        mov    ecx, dword ptr [esi + 34h]
loc_1_13BE:
        cli
        test   byte ptr [ecx + 5], 80h
        je     SHORT loc_1_13CF
        lea    eax, [ecx + 4ch]
        call   sub_1_875C
        jmp    SHORT loc_1_13BE
loc_1_13CF:
        mov    al, byte ptr [ecx + 9]
        sti
        test   al, al
        jne    SHORT loc_1_1430
        mov    edx, dword ptr [esi + 2ch]
        mov    al, byte ptr [edx + 33h]
        test   al, al
        jne    SHORT loc_1_1421
        mov    eax, dword ptr [edx + 22h]
        mov    edx, dword ptr [esi + 20h]
        mov    edi, dword ptr [esi + 30h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    di, word ptr [edx + 1ah]
        mov    ebx, dword ptr [esi + 24h]
        movzx  edx, word ptr [ebx + 15h]
        add    edx, dword ptr [ebx + 7]
        movzx  ebp, word ptr [ebx + 0fh]
        and    ebp, 1
        shl    ebp, 0eh
        movzx  ebx, word ptr [ecx + 10h]
        mov    esi, dword ptr [ecx + 0ch]
        mov    ecx, ebx
        call   sub_1_97AB
        sbb    ah, ah
        and    al, ah
        db  08Bh, 035h, 014h, 028h, 001h, 000h
loc_1_1421:
        push   ebx
        push   eax
        mov    edx, dword ptr [esi + 34h]
        mov    eax, dword ptr [edx + 58h]
        call   sub_1_78A8
        jmp    SHORT loc_1_1449
loc_1_1430:
        mov    eax, dword ptr [ecx + 58h]
        call   sub_1_78A8
        mov    ebx, ecx
        mov    ecx, 1
        call   sub_1_78E4
        mov    ecx, dword ptr [esi + 2ch]
        jmp    SHORT loc_1_146A
loc_1_1449:
        mov    ecx, 1
        mov    ebx, edx
        call   sub_1_78E4
        mov    ecx, dword ptr [esi + 2ch]
        pop    eax
        mov    byte ptr [ecx + 33h], al
        and    byte ptr [ecx + 18h], 7fh
        pop    ebx
        mov    eax, dword ptr [esi + 24h]
        test   byte ptr [eax + 0fh], 1
        jne    SHORT loc_1_149A
loc_1_146A:
        dec    byte ptr [ecx + 32h]
        jne    SHORT loc_1_147D
        cmp    dword ptr [ecx + 2ah], 0
        je     SHORT loc_1_147D
        lea    eax, [ecx + 2ah]
        call   sub_1_8BC0
loc_1_147D:
        mov    ecx, dword ptr [esi + 1ch]
        call   sub_1_766C
        movzx  edx, byte ptr [ecx + 7]
        db  08Bh, 03Ch, 095h, 0B0h, 01Bh, 001h, 000h
        call   sub_1_7391
        jmp    NEAR PTR loc_1_0154
loc_1_149A:
        movzx  eax, word ptr [eax + 15h]
        lea    edx, [ebx + eax]
        push   ecx
        mov    ecx, dword ptr [esi + 1ch]
        call   sub_1_766C
        pop    ecx
        cmp    byte ptr [ecx + 33h], 0
        jne    SHORT loc_1_14D3
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 1
        mov    word ptr [ebx + 1], dx
        mov    word ptr [ebx + 3], 0
        add    ebx, 5
        mov    dword ptr [esi + 28h], ebx
        mov    ebx, dword ptr [esi + 20h]
        mov    byte ptr [ebx + 4], 20h
        jmp    NEAR PTR loc_1_0120
loc_1_14D3:
        sub    al, al
        xchg   byte ptr [ecx + 33h], al
        jmp    NEAR PTR loc_1_0014
loc_1_14DD:
        mov    ecx, 1
        mov    ebx, dword ptr [esi + 34h]
        call   sub_1_78E4
        mov    edx, dword ptr [esi + 20h]
loc_1_14ED:
        mov    ebx, dword ptr [esi + 24h]
        mov    ecx, dword ptr [esi + 2ch]
        mov    dword ptr [edx + 5], 590002h
        mov    byte ptr [edx + 4], 20h
        mov    edi, dword ptr [esi + 30h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    di, word ptr [edx + 1ah]
        movzx  esi, word ptr [ebx + 17h]
        add    esi, edx
        mov    eax, dword ptr [ecx + 22h]
        mov    edx, dword ptr [ebx + 7]
        movzx  ebp, word ptr [ebx + 0fh]
        and    ebp, 1
        shl    ebp, 0eh
        movzx  ebx, word ptr [ebx + 15h]
        mov    ecx, ebx
        jecxz  loc_1_152F
        call   sub_1_97AB
loc_1_152F:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     NEAR PTR loc_1_0014
        mov    eax, dword ptr [esi + 28h]
        mov    byte ptr [eax], 1
        mov    word ptr [eax + 1], bx
        mov    word ptr [eax + 3], 0
        add    eax, 5
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0120
        db  02Eh, 08Bh, 0C0h
sub_1_155C:
        mov    dword ptr [ecx + 4ch], 0
        movzx  eax, word ptr [ecx + 10h]
        push   eax
        call   sub_1_75C0
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        pop    ebp
        test   al, al
        jne    SHORT loc_1_15DF
        mov    ecx, dword ptr [esi + 2ch]
        or     byte ptr [ecx + 18h], 80h
        mov    ebx, dword ptr [esi + 24h]
        mov    byte ptr [ebx], 1
        mov    word ptr [ebx + 1], 0
        mov    word ptr [ebx + 3], 0
        add    ebx, 5
        mov    ecx, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esi + 20h]
        or     byte ptr [edx + 9], 80h
        sub    ebx, edx
        mov    word ptr [ecx + 10h], bx
        db  08Bh, 015h, 01Ch, 01Ch, 001h, 000h
        add    dword ptr [edx + 8], ebp
        adc    dword ptr [edx + 0ch], 0
        add    dword ptr [edx], ebx
        adc    dword ptr [edx + 4], 0
        db  0A1h, 0C4h, 031h, 001h, 000h
        sub    eax, dword ptr [ecx + 44h]
        add    dword ptr [edx + 10h], eax
        inc    dword ptr [edx + 14h]
        mov    byte ptr [ecx + 8], 94h
        mov    byte ptr [ecx + 5], 7fh
        mov    dword ptr [ecx + 48h], 0
        call   sub_1_759C
        db  08Bh, 035h, 014h, 028h, 001h, 000h
loc_1_15DF:
        ret
loc_1_15E0:
        jmp    NEAR PTR loc_1_0033
        db  02Eh, 08Bh, 0C0h
loc_1_15E8:
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 2
        call   sub_1_18B8
        jb     SHORT loc_1_15E0
        bts    dword ptr [ecx + 18h], 7
        jb     SHORT loc_1_161E
        mov    ax, word ptr [edx + 1eh]
        mov    word ptr [ecx + 3ah], ax
        xor    eax, eax
        mov    dword ptr [ecx + 36h], eax
        mov    dword ptr [ecx + 3eh], eax
        mov    word ptr [ecx + 3ch], ax
        mov    al, byte ptr [ebx + 0fh]
        mov    byte ptr [ecx + 35h], al
        test   al, 80h
        mov    ax, word ptr [ebx + 3]
        je     SHORT loc_1_1630
loc_1_161E:
        mov    ax, word ptr [edx + 14h]
        or     word ptr [ecx + 3ch], ax
        mov    eax, dword ptr [ebx + 11h]
        or     dword ptr [ecx + 36h], eax
        mov    ax, word ptr [ebx + 15h]
loc_1_1630:
        add    word ptr [ecx + 3eh], ax
        inc    byte ptr [ecx + 32h]
        cmp    byte ptr [ecx + 33h], 0
        jne    NEAR PTR loc_1_17B1
        movzx  eax, word ptr [ebx + 17h]
        lea    esi, [eax + edx]
        mov    eax, dword ptr [ecx + 22h]
        movzx  ecx, word ptr [ebx + 3]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    di, word ptr [edx + 1ah]
        movzx  ebp, word ptr [ebx + 0fh]
        and    ebp, 1
        shl    ebp, 0eh
        mov    edx, dword ptr [ebx + 7]
        movzx  ebx, word ptr [ebx + 15h]
        call   sub_1_97AB
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        mov    edx, dword ptr [esi + 20h]
        jb     NEAR PTR loc_1_17A8
loc_1_1682:
        add    bx, word ptr [ecx + 40h]
        mov    word ptr [ecx + 40h], bx
        cmp    bx, word ptr [ecx + 3eh]
        jae    SHORT loc_1_16B3
        test   byte ptr [ecx + 35h], 80h
        jne    SHORT loc_1_16C0
loc_1_1696:
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 1
        mov    word ptr [ebx + 1], 0ffffh
        mov    word ptr [ebx + 3], 0
        lea    eax, [ebx + 5]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0120
loc_1_16B3:
        test   byte ptr [ecx + 35h], 80h
        je     SHORT loc_1_1724
        cmp    word ptr [ecx + 3ch], 0
        jne    SHORT loc_1_16EB
loc_1_16C0:
        dec    byte ptr [ecx + 32h]
        jne    SHORT loc_1_16D3
        cmp    dword ptr [ecx + 2ah], 0
        je     SHORT loc_1_16D3
        lea    eax, [ecx + 2ah]
        call   sub_1_8BC0
loc_1_16D3:
        mov    ecx, dword ptr [esi + 1ch]
        movzx  edx, byte ptr [ecx + 7]
        db  08Bh, 03Ch, 095h, 0B0h, 01Bh, 001h, 000h
        call   sub_1_7391
        jmp    NEAR PTR loc_1_0154
loc_1_16EB:
        mov    ax, word ptr [ecx + 3ch]
        mov    word ptr [edx + 14h], ax
        cmp    byte ptr [ecx + 33h], 0
        jne    NEAR PTR loc_1_17C3
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 2
        mov    eax, dword ptr [ecx + 36h]
        mov    dword ptr [ebx + 1], eax
        mov    word ptr [ebx + 5], 0
        lea    eax, [ebx + 7]
        mov    dword ptr [esi + 28h], eax
        mov    word ptr [ecx + 3ah], 0
        and    byte ptr [ecx + 18h], 7fh
        jmp    NEAR PTR loc_1_0120
loc_1_1724:
        test   byte ptr [ecx + 35h], 1
        jne    SHORT loc_1_1739
        mov    word ptr [ecx + 3ah], 0
        and    byte ptr [ecx + 18h], 7fh
        jmp    NEAR PTR loc_1_1696
loc_1_1739:
        push   ebx
        mov    ebx, dword ptr [esi + 24h]
        mov    byte ptr [ebx], 1
        mov    word ptr [ebx + 1], 0ffffh
        mov    word ptr [ebx + 3], 0
        or     byte ptr [edx + 9], 80h
        mov    ecx, dword ptr [esi + 1ch]
        mov    byte ptr [ecx + 8], 94h
        mov    byte ptr [ecx + 5], 7fh
        mov    word ptr [ecx + 10h], 25h
        mov    dword ptr [ecx + 48h], 0
        call   sub_1_759C
        call   sub_1_766C
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        pop    eax
        mov    word ptr [ecx + 3ah], 0
        and    byte ptr [ecx + 18h], 7fh
        mov    edx, dword ptr [esi + 20h]
        mov    byte ptr [edx + 4], 20h
        mov    ebx, dword ptr [esi + 24h]
        mov    byte ptr [ebx], 1
        mov    word ptr [ebx + 1], ax
        mov    word ptr [ebx + 3], 0
        lea    eax, [ebx + 5]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0120
loc_1_17A8:
        cmp    byte ptr [ecx + 33h], 0
        jne    SHORT loc_1_17B1
        mov    byte ptr [ecx + 33h], al
loc_1_17B1:
        test   byte ptr [ecx + 35h], 80h
        je     SHORT loc_1_17C3
        mov    ebx, dword ptr [esi + 24h]
        mov    bx, word ptr [ebx + 15h]
        jmp    NEAR PTR loc_1_1682
loc_1_17C3:
        mov    word ptr [ecx + 3ah], 0
        and    byte ptr [ecx + 18h], 7fh
        mov    byte ptr [edx + 4], 20h
        mov    al, 0
        xchg   byte ptr [ecx + 33h], al
        jmp    NEAR PTR loc_1_0014
        db  090h
loc_1_17DC:
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 1
        call   sub_1_18B8
        jb     NEAR PTR loc_1_16D3
        mov    ax, word ptr [edx + 1eh]
        cmp    ax, word ptr [ecx + 3ah]
        jne    NEAR PTR loc_1_16D3
        test   byte ptr [ecx + 18h], 80h
        je     NEAR PTR loc_1_16D3
        inc    byte ptr [ecx + 32h]
        mov    ax, word ptr [ebx + 3]
        cmp    word ptr [ecx + 3eh], ax
        jb     SHORT loc_1_1816
        mov    word ptr [ecx + 3eh], ax
loc_1_1816:
        movzx  eax, word ptr [ebx + 0fh]
        lea    esi, [eax + edx]
        mov    eax, dword ptr [ecx + 22h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    di, word ptr [edx + 1ah]
        movzx  ebp, byte ptr [ecx + 35h]
        and    ebp, 1
        shl    ebp, 0eh
        mov    edx, dword ptr [ebx + 5]
        movzx  ebx, word ptr [ebx + 0dh]
        push   ebx
        mov    ecx, ebx
        call   sub_1_97AB
        pop    ebx
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     SHORT loc_1_1877
loc_1_1850:
        add    bx, word ptr [ecx + 40h]
        mov    word ptr [ecx + 40h], bx
        cmp    bx, word ptr [ecx + 3eh]
        jb     NEAR PTR loc_1_16C0
        mov    word ptr [ecx + 3ah], 0
        and    byte ptr [ecx + 18h], 7fh
        test   byte ptr [ecx + 35h], 1
        jne    SHORT loc_1_1882
        jmp    NEAR PTR loc_1_16C0
loc_1_1877:
        cmp    byte ptr [ecx + 33h], 0
        jne    SHORT loc_1_1850
        mov    byte ptr [ecx + 33h], al
        jmp    SHORT loc_1_1850
loc_1_1882:
        mov    edx, dword ptr [esi + 20h]
        mov    byte ptr [edx + 4], 20h
        sub    al, al
        xchg   byte ptr [ecx + 33h], al
        test   al, al
        jne    SHORT loc_1_18B1
        mov    ax, word ptr [ecx + 3eh]
        mov    ebx, dword ptr [esi + 24h]
        mov    byte ptr [ebx], 1
        mov    word ptr [ebx + 1], ax
        mov    word ptr [ebx + 3], 0
        lea    eax, [ebx + 5]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0120
loc_1_18B1:
        jmp    NEAR PTR loc_1_0014
        db  000h, 000h
sub_1_18B8:
        shl    eax, 10h
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_18C6
        mov    ecx, dword ptr [esi + 2ch]
        jmp    SHORT loc_1_18DA
loc_1_18C6:
        cmp    ecx, 400h
        jae    SHORT loc_1_1925
        db  08Bh, 00Ch, 08Dh, 01Ch, 03Ah, 001h, 000h
        mov    dword ptr [esi + 2ch], ecx
        jecxz  loc_1_1925
loc_1_18DA:
        cmp    byte ptr [ecx + 19h], 1
        jne    SHORT loc_1_1925
        mov    ax, word ptr [edx + 18h]
        cmp    word ptr [ecx + 14h], ax
        jne    SHORT loc_1_1917
        cmp    dword ptr [ecx + 0ch], edi
        jne    SHORT loc_1_1917
        mov    al, byte ptr [ecx + 18h]
        test   al, 2
        je     SHORT loc_1_192C
        test   al, 80h
        jne    SHORT loc_1_1940
        cmp    byte ptr [ecx + 33h], 0
        jne    SHORT loc_1_1901
        ret
loc_1_1901:
        shl    eax, 0fh
        js     SHORT loc_1_1916
        movzx  eax, byte ptr [ecx + 33h]
        jb     SHORT loc_1_1910
        mov    byte ptr [ecx + 33h], 0
loc_1_1910:
        shl    eax, 10h
        mov    al, 1
        stc
loc_1_1916:
        ret
loc_1_1917:
        test   eax, 20000h
        jne    SHORT loc_1_1925
        call   sub_1_196C
        jb     SHORT loc_1_192B
loc_1_1925:
        mov    eax, 60001h
        stc
loc_1_192B:
        ret
loc_1_192C:
        test   eax, 20000h
        jne    SHORT loc_1_1925
        and    byte ptr [ecx + 18h], 0fbh
        jne    SHORT loc_1_1925
        call   sub_1_1AB4
        jmp    SHORT loc_1_1925
loc_1_1940:
        mov    ax, word ptr [edx + 1eh]
        test   ax, ax
        je     SHORT loc_1_1956
        cmp    word ptr [ecx + 3ah], ax
        jne    SHORT loc_1_1956
        cmp    byte ptr [ecx + 33h], 0
        jne    SHORT loc_1_1901
        ret
loc_1_1956:
        push   eax
        lea    eax, [ecx + 2ah]
        mov    ecx, dword ptr [ecx + 8]
        call   sub_1_875C
        pop    eax
        jmp    NEAR PTR loc_1_18C6
sub_1_1968:
        jmp    SHORT loc_1_19A9
        db  08Bh, 0C0h
sub_1_196C:
        mov    ebp, eax
        and    eax, 7ffh
        shl    eax, 5
        movzx  ebx, ah
        db  08Bh, 01Ch, 09Dh, 098h, 01Ah, 001h, 000h
        test   ebx, ebx
        je     SHORT loc_1_19BB
        shr    al, 2
        movzx  eax, al
        shr    eax, 3
        imul   eax, eax, 41ch
        add    eax, ebx
        cmp    word ptr [eax + 16h], bp
        jne    SHORT loc_1_19BB
        cmp    dword ptr [eax + 8], edi
        jne    SHORT loc_1_19BB
        test   byte ptr [eax + 14h], 2
        je     SHORT loc_1_19C2
        mov    dword ptr [esi + 34h], eax
loc_1_19A9:
        mov    ebx, dword ptr [edi + 38h]
        mov    dword ptr [esi + 3ch], ebx
        xor    ebp, ebp
        mov    dword ptr [esi + 40h], ebp
        ret
        db  033h, 0EDh, 089h, 06Eh, 040h, 0C3h
loc_1_19BB:
        mov    eax, 50002h
        stc
        ret
loc_1_19C2:
        cmp    byte ptr [eax + 14h], 0
        je     SHORT loc_1_19BB
        and    byte ptr [eax + 14h], 0fbh
        jne    SHORT loc_1_19BB
        db  08Bh, 01Dh, 090h, 01Ah, 001h, 000h
        mov    dword ptr [eax], ebx
        db  0A3h, 090h, 01Ah, 001h, 000h
        jmp    SHORT loc_1_19BB
        db  0B8h, 002h, 000h, 05Bh, 000h, 0F9h, 0C3h
sub_1_19E4:
        push   ebx
        push   edi
        mov    ebx, eax
        movzx  eax, ax
        push   ecx
        push   edx
        push   eax
        call   sub_1_8CB6
        pop    edx
        pop    edx
        pop    ecx
        test   eax, eax
        je     SHORT loc_1_1A59
        mov    dword ptr [esi + 2ch], eax
        mov    edi, eax
        movzx  ecx, bx
        shr    ecx, 2
        sub    eax, eax
        rep stosd
loc_1_1A09:
        db  08Bh, 00Dh, 098h, 01Bh, 001h, 000h
        jecxz  loc_1_1A29
        db  08Bh, 03Dh, 0F8h, 01Bh, 001h, 000h
        sub    eax, eax
        repne scasd
        je     SHORT loc_1_1A64
        db  081h, 03Dh, 098h, 01Bh, 001h, 000h, 000h, 004h, 000h, 000h
        je     SHORT loc_1_1A3F
loc_1_1A29:
        db  0C7h, 005h, 098h, 01Bh, 001h, 000h, 000h, 004h, 000h, 000h
        db  0C7h, 005h, 0F8h, 01Bh, 001h, 000h, 01Ch, 03Ah, 001h, 000h
        jmp    SHORT loc_1_1A09
loc_1_1A3F:
        mov    eax, dword ptr [esi + 2ch]
        push   ecx
        push   edx
        push   eax
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        push   edx
        db  0B8h, 078h, 01Ch, 001h, 000h
        call   sub_1_9239
        pop    edx
loc_1_1A59:
        pop    edi
        pop    ebx
        mov    eax, 590002h
        stc
        ret    4
loc_1_1A64:
        db  089h, 00Dh, 098h, 01Bh, 001h, 000h
        db  089h, 03Dh, 0F8h, 01Bh, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        mov    dword ptr [edi - 4], ecx
        shr    ebx, 10h
        mov    byte ptr [ecx + 19h], bl
        mov    byte ptr [ecx + 18h], 1
        db  081h, 0EFh, 020h, 03Ah, 001h, 000h
        shr    edi, 2
        mov    dword ptr [ecx + 8], edi
loc_1_1A8C:
        db  0FEh, 087h, 01Ch, 04Ah, 001h, 000h
        db  0F6h, 087h, 01Ch, 04Ah, 001h, 000h, 007h
        je     SHORT loc_1_1A8C
        pop    edi
        mov    dword ptr [ecx + 0ch], edi
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [ecx + 10h], eax
        clc
        pop    ebx
        ret    4
        db  02Eh, 08Bh, 0C0h
sub_1_1AB4:
        mov    eax, dword ptr [ecx + 8]
        db  0C7h, 004h, 085h, 01Ch, 03Ah, 001h, 000h, 000h, 000h, 000h, 000h
        test   byte ptr [ecx + 18h], 8
        je     SHORT loc_1_1ACD
        call   sub_1_8550
loc_1_1ACD:
        push   ecx
        push   edx
        push   ecx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_1ADC:
        push   ebx
        mov    ebx, dword ptr [ecx + 0ch]
        cmp    byte ptr [ebx + 1eh], 2
        jne    SHORT loc_1_1B36
        lea    eax, [ebx + 50h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ecx + 10h]
        test   byte ptr [eax + 14h], 2
        je     SHORT loc_1_1B2C
        or     byte ptr [ecx + 18h], 2
        cmp    byte ptr [ecx + 19h], 1
        jne    SHORT loc_1_1B1B
        inc    word ptr [eax + 4]
        inc    word ptr [ebx + 44h]
        test   byte ptr [ecx + 18h], 8
        jne    SHORT loc_1_1B22
        pop    ebx
        ret
loc_1_1B1B:
        inc    word ptr [eax + 6]
        clc
        pop    ebx
        ret
loc_1_1B22:
        call   sub_1_8540
        and    dh, 7fh
        pop    ebx
        ret
loc_1_1B2C:
        mov    eax, dword ptr [ecx]
        mov    ebx, dword ptr [ecx + 4]
        mov    dword ptr [ebx], eax
        mov    dword ptr [eax + 4], ebx
loc_1_1B36:
        pop    ebx
        mov    al, byte ptr [ecx + 19h]
        cmp    al, 1
        jne    SHORT loc_1_1B79
        mov    eax, dword ptr [ecx + 26h]
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        mov    eax, dword ptr [ecx + 22h]
        sub    ebp, ebp
        lea    ecx, [ebp - 1]
        call   sub_1_96A4
loc_1_1B64:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        call   sub_1_1AB4
        mov    eax, 50002h
        stc
        ret
loc_1_1B79:
        mov    eax, dword ptr [ecx + 1ch]
        test   eax, eax
        je     SHORT loc_1_1B64
        sub    edi, edi
        call   sub_1_CB7A
        jmp    SHORT loc_1_1B64
        db  02Eh, 08Bh, 0C0h
sub_1_1B8C:
        mov    eax, dword ptr [ecx]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        xchg   dword ptr [eax + 4], ecx
        mov    eax, dword ptr [ecx + 10h]
        cmp    byte ptr [ecx + 19h], 1
        jne    SHORT loc_1_1BC4
        dec    word ptr [eax + 4]
        mov    eax, dword ptr [ecx + 0ch]
        dec    word ptr [eax + 44h]
        mov    eax, dword ptr [ecx + 26h]
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        ret
loc_1_1BC4:
        dec    word ptr [eax + 6]
        ret
sub_1_1BC9:
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        enter  0, 0
        mov    ebx, edx
        mov    esi, edx
        mov    edi, eax
        movzx  eax, byte ptr [edi]
        add    eax, edi
        movzx  ecx, byte ptr [eax]
        sub    eax, edx
        sub    byte ptr [edi], al
        add    eax, ecx
        push   0
        lea    ecx, [esp - 4]
loc_1_1BED:
        push   esi
        movzx  edx, byte ptr [esi]
        add    esi, edx
        sub    al, dl
        ja     SHORT loc_1_1BED
        mov    eax, esp
        push   1
        push   edi
        push   ecx
loc_1_1BFD:
        mov    ecx, dword ptr [eax]
        lea    edi, [eax + 4]
        jecxz  loc_1_1C5B
        movzx  edx, byte ptr [ecx]
        test   edx, edx
        je     SHORT loc_1_1C56
        cmp    word ptr [ecx + 1], 2e2eh
        jne    SHORT loc_1_1C56
        inc    ecx
        sub    edx, 2
        push   edx
loc_1_1C18:
        cmp    byte ptr [edx + ecx], 2eh
        jne    SHORT loc_1_1C24
        dec    edx
        jne    SHORT loc_1_1C18
        pop    edx
        jmp    SHORT loc_1_1C27
loc_1_1C24:
        pop    edx
        jmp    SHORT loc_1_1C56
loc_1_1C27:
        mov    byte ptr [ecx - 1], 0
loc_1_1C2B:
        mov    esi, dword ptr [edi]
        or     esi, esi
        je     SHORT loc_1_1C56
        lea    edi, [edi + 4]
        cmp    byte ptr [esi], 0
        je     SHORT loc_1_1C2B
        cmp    word ptr [esi + 1], 2e2eh
        jne    SHORT loc_1_1C50
        movzx  ecx, byte ptr [esi]
        inc    esi
        dec    ecx
loc_1_1C46:
        dec    ecx
        je     SHORT loc_1_1C2B
        cmp    byte ptr [ecx + esi], 2eh
        je     SHORT loc_1_1C46
        dec    esi
loc_1_1C50:
        mov    byte ptr [esi], 0
        dec    edx
        jne    SHORT loc_1_1C2B
loc_1_1C56:
        add    eax, 4
        jmp    SHORT loc_1_1BFD
loc_1_1C5B:
        pop    edx
        pop    eax
        cld
        xor    ecx, ecx
loc_1_1C60:
        mov    esi, dword ptr [edx]
        cmp    esi, 1
        je     SHORT loc_1_1C7B
        movzx  ecx, byte ptr [eax]
        lea    edi, [ecx + eax]
        movzx  ecx, byte ptr [esi]
        sub    edx, 4
        add    byte ptr [eax], cl
        push   ecx
        rep movsb
        pop    ecx
        jmp    SHORT loc_1_1C60
loc_1_1C7B:
        sub    byte ptr [eax], cl
        cmp    byte ptr [ebx], 0
        jne    SHORT loc_1_1C87
        mov    word ptr [ebx], 1
loc_1_1C87:
        clc
        leave
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret
loc_1_1C90:
        mov    eax, 80001h
        ret
loc_1_1C96:
        mov    eax, 20001h
        stc
        ret
loc_1_1C9D:
        mov    eax, 30001h
        stc
        ret
sub_1_1CA4:
        cmp    byte ptr [ebx + 9], 3
        je     SHORT loc_1_1C9D
        cmp    byte ptr [ebx + 9], 1
        je     SHORT loc_1_1C96
        push   edx
        db  08Bh, 015h, 014h, 028h, 001h, 000h
        test   byte ptr [edx + 9], 2
        jne    NEAR PTR loc_1_1E3A
loc_1_1CC1:
        pop    edx
        add    eax, 2
        push   ecx
        push   edx
        push   eax
        call   sub_1_8CB6
        pop    edx
        pop    edx
        pop    ecx
        test   eax, eax
        je     SHORT loc_1_1C90
sub_1_1CD4:
        enter  0ch, 0
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 0ch], 0
        push   edx
        push   eax
        push   0
        push   esi
        db  08Bh, 015h, 014h, 028h, 001h, 000h
        test   byte ptr [edx + 9], 2
        jne    NEAR PTR loc_1_1E4F
loc_1_1CFB:
        mov    esi, dword ptr [ebx + 2ch]
loc_1_1CFE:
        movzx  ecx, byte ptr [ebx + 0bh]
        mov    word ptr [eax], cx
        add    esi, 3
        lea    edi, [eax + 5]
        lea    ecx, [eax + 4]
        mov    edx, ecx
        db  08Bh, 01Dh, 054h, 037h, 001h, 000h
        mov    eax, eax
loc_1_1D18:
        lodsb
        cmp    al, 3ch
        je     SHORT loc_1_1D3E
        xlatb
        cmp    al, 5ch
        je     SHORT loc_1_1D18
        cmp    al, 2
        jbe    NEAR PTR loc_1_1DB3
        push   eax
        lea    eax, [esi - 1]
        mov    dword ptr [ebp - 4], eax
        pop    eax
        mov    eax, eax
loc_1_1D34:
        stosb
loc_1_1D35:
        lodsb
        cmp    al, 3eh
        je     SHORT loc_1_1D54
        cmp    al, 3ch
        jne    SHORT loc_1_1D60
loc_1_1D3E:
        push   eax
        lodsb
        test   al, al
        jne    SHORT loc_1_1D50
        pop    eax
        mov    al, 2ah
        stosb
        mov    al, 2eh
        stosb
        mov    al, 0
        stosb
        jmp    SHORT loc_1_1DC4
loc_1_1D50:
        pop    eax
        dec    esi
        jmp    SHORT loc_1_1D60
loc_1_1D54:
        push   edx
        db  08Bh, 015h, 014h, 028h, 001h, 000h
        or     byte ptr [edx + 9], 4
        pop    edx
loc_1_1D60:
        xlatb
        cmp    al, 5ch
        je     SHORT loc_1_1D74
        cmp    al, 2
        ja     SHORT loc_1_1D34
        jne    SHORT loc_1_1DC4
        mov    ax, word ptr [esi - 1]
loc_1_1D6F:
        stosw
        inc    esi
        jmp    SHORT loc_1_1D35
loc_1_1D74:
        xchg   edi, ecx
        sub    ecx, edi
        test   ch, ch
        jne    SHORT loc_1_1DA4
        mov    byte ptr [edi], cl
        cmp    word ptr [edi], 2e02h
        je     SHORT loc_1_1D9C
        cmp    word ptr [edi + 1], 2e2eh
        jne    SHORT loc_1_1D90
        inc    dword ptr [ebp - 8]
loc_1_1D90:
        mov    edx, edi
        add    ecx, edi
        lea    edi, [ecx + 1]
        jmp    NEAR PTR loc_1_1D18
loc_1_1D9C:
        mov    ecx, edi
        inc    edi
        jmp    NEAR PTR loc_1_1D18
loc_1_1DA4:
        pop    esi
        test   esi, esi
        jne    SHORT loc_1_1DA4
loc_1_1DA9:
        pop    eax
        mov    eax, 0ce0001h
        stc
        pop    edx
        leave
        ret
loc_1_1DB3:
        jne    SHORT loc_1_1DC4
        push   eax
        lea    eax, [esi - 1]
        mov    dword ptr [ebp - 4], eax
        pop    eax
        mov    ax, word ptr [esi - 1]
        jmp    SHORT loc_1_1D6F
        db  090h
loc_1_1DC4:
        mov    eax, esi
        pop    esi
        test   esi, esi
        je     SHORT loc_1_1DD3
        mov    dword ptr [ebp - 0ch], ecx
        jmp    NEAR PTR loc_1_1D18
loc_1_1DD3:
        mov    esi, eax
        mov    eax, edi
        sub    eax, ecx
        test   ah, ah
        jne    SHORT loc_1_1DA9
        cmp    al, 1
        je     SHORT loc_1_1E24
        mov    byte ptr [ecx], al
        cmp    word ptr [ecx], 2e02h
        je     SHORT loc_1_1E24
        cmp    word ptr [ecx + 1], 2e2eh
        jne    SHORT loc_1_1DF5
        inc    dword ptr [ebp - 8]
loc_1_1DF5:
        pop    eax
        add    eax, 2
        sub    ecx, eax
        mov    word ptr [eax], cx
        mov    ecx, edi
        sub    ecx, eax
        cmp    ecx, 104h
        ja     SHORT loc_1_1E5F
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], 0
        clc
        je     SHORT loc_1_1E21
        push   ecx
        mov    edx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        call   sub_1_1BC9
        pop    ecx
loc_1_1E21:
        pop    edx
        leave
        ret
loc_1_1E24:
        cmp    ecx, edx
        mov    ecx, edx
        jne    SHORT loc_1_1DF5
        mov    word ptr [ecx], 1
        jmp    SHORT loc_1_1DF5
        db  080h, 079h, 002h, 02Eh, 075h, 0BEh, 058h, 0EBh, 031h
loc_1_1E3A:
        movzx  edx, byte ptr [ebx + 44h]
        test   edx, edx
        je     SHORT loc_1_1E74
loc_1_1E42:
        add    eax, edx
        movzx  edx, byte ptr [ebx + 0ah]
        sub    eax, edx
        jmp    NEAR PTR loc_1_1CC1
loc_1_1E4F:
        mov    esi, dword ptr [ebx + 30h]
        or     esi, esi
        jne    NEAR PTR loc_1_1CFE
        jmp    NEAR PTR loc_1_1CFB
loc_1_1E5F:
        cmp    word ptr [eax], 104h
        mov    eax, 20001h
        jb     SHORT loc_1_1E70
        mov    eax, 30001h
loc_1_1E70:
        stc
        pop    edx
        leave
        ret
loc_1_1E74:
        pushad
        push   ebx
        call   sub_1_A164
        pop    ebx
        popad
        movzx  edx, byte ptr [ebx + 44h]
        test   edx, edx
        je     NEAR PTR loc_1_1CC1
        jmp    SHORT loc_1_1E42
        db  090h
sub_1_1E8C:
        push   esi
        cmp    byte ptr [ecx + 19h], 1
        jne    SHORT loc_1_1EFD
        push   eax
        btr    dword ptr [ecx + 18h], 1
        jae    SHORT loc_1_1EE8
        or     byte ptr [ecx + 18h], 4
        call   sub_1_1B8C
loc_1_1EA4:
        cmp    byte ptr [ecx + 32h], 0
        je     SHORT loc_1_1EBC
        lea    eax, [ecx + 2ah]
        call   sub_1_875C
        jmp    SHORT loc_1_1EA4
loc_1_1EB4:
        lea    eax, [ecx + 2ah]
        call   sub_1_8BC0
loc_1_1EBC:
        btr    dword ptr [ecx + 18h], 7
        jb     SHORT loc_1_1EB4
        mov    eax, dword ptr [ecx + 22h]
        push   ecx
        sub    ebp, ebp
        lea    ecx, [ebp - 1]
        call   sub_1_96A4
        pop    ecx
        test   byte ptr [ecx + 18h], 8
        je     SHORT loc_1_1EDD
        call   sub_1_8550
loc_1_1EDD:
        cmp    dword ptr [ecx + 1ah], 0
        je     SHORT loc_1_1EE8
        call   sub_1_948C
loc_1_1EE8:
        pop    eax
        mov    ah, 4
        cmp    al, 2
        je     SHORT loc_1_1EF1
        mov    ah, 0
loc_1_1EF1:
        and    byte ptr [ecx + 18h], ah
        jne    SHORT loc_1_1EFB
        call   sub_1_1AB4
loc_1_1EFB:
        pop    esi
        ret
loc_1_1EFD:
        and    byte ptr [ecx + 18h], 80h
        call   sub_1_1B8C
        cmp    byte ptr [ecx + 18h], 0
        jne    SHORT loc_1_1F3A
        cmp    byte ptr [ecx + 19h], 2
        jne    SHORT loc_1_1F24
        mov    eax, dword ptr [ecx + 48h]
        test   eax, eax
        je     SHORT loc_1_1F24
        push   ecx
        push   edx
        push   eax
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
loc_1_1F24:
        push   dword ptr [ecx + 1ch]
        push   dword ptr [ecx + 24h]
        call   sub_1_1AB4
        pop    edi
        pop    eax
        test   eax, eax
        je     SHORT loc_1_1F3A
        call   sub_1_CB7A
loc_1_1F3A:
        pop    esi
        ret
sub_1_1F3C:
        push   ebp
        sub    esp, 14h
        mov    ebp, esp
        mov    dword ptr [ebp], eax
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ebx, dword ptr [esi + 20h]
        mov    ebx, dword ptr [ebx + 18h]
        mov    dword ptr [ebp + 8], ebx
        mov    edi, dword ptr [esi + 30h]
        lea    edx, [edi + 50h]
        mov    dword ptr [ebp + 0ch], edx
        movzx  ecx, word ptr [eax]
        mov    edi, eax
        add    edi, ecx
        xor    ebx, ebx
        mov    bl, byte ptr [edi]
        add    ecx, ebx
        mov    dword ptr [ebp + 4], ecx
        mov    ebx, edx
        nop
loc_1_1F70:
        mov    ebx, dword ptr [ebx]
        cmp    ebx, dword ptr [ebp + 0ch]
        je     SHORT loc_1_1FAB
        cmp    byte ptr [ebx + 19h], 1
        jne    SHORT loc_1_1F70
        test   byte ptr [ebx + 18h], 2
        je     SHORT loc_1_1F70
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 14h], edi
        jne    SHORT loc_1_1F70
        mov    esi, dword ptr [ebp]
        mov    edi, dword ptr [ebx + 26h]
        mov    ecx, dword ptr [ebp + 4]
        repe cmpsb
        jne    SHORT loc_1_1F70
        mov    dword ptr [ebp + 10h], ebx
        mov    ecx, ebx
        mov    al, 6
        push   ebp
        call   sub_1_1E8C
        pop    ebp
        mov    ebx, dword ptr [ebp + 10h]
        jmp    SHORT loc_1_1F70
loc_1_1FAB:
        add    esp, 14h
        pop    ebp
        clc
        ret
sub_1_1FB1:
        db  0CDh, 020h, 005h, 080h, 093h, 004h
        db  0CDh, 020h, 000h, 000h, 087h, 004h
        jb     SHORT loc_1_1FC3
        xor    eax, eax
        inc    eax
        ret
loc_1_1FC3:
        xor    eax, eax
        ret
        db  000h, 000h
loc_1_1FC8:
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_0033
        mov    ebx, dword ptr [eax + 0ch]
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_1FEF
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_1FEF
        test   byte ptr [ebx + 10h], 4
        je     SHORT loc_1_2053
loc_1_1FEF:
        mov    eax, dword ptr [esi + 24h]
        movzx  esi, byte ptr [eax]
        lea    esi, [eax + esi*2 + 1]
        movzx  eax, byte ptr [ebx + 0ah]
        add    ax, word ptr [esi]
        add    esi, 3
        call   sub_1_1CA4
        jb     SHORT loc_1_2058
        push   eax
        mov    ebp, eax
        sub    edx, edx
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    esi, dword ptr [esi + 40h]
        call   sub_1_9E9C
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        jb     SHORT loc_1_205D
        pop    eax
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0
        mov    word ptr [ebx + 1], 0
        lea    eax, [ebx + 3]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
loc_1_2053:
        mov    eax, 40002h
loc_1_2058:
        jmp    NEAR PTR loc_1_0033
loc_1_205D:
        xchg   dword ptr [esp], eax
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        pop    eax
        sub    ah, ah
        shl    eax, 10h
        mov    al, 1
        jmp    NEAR PTR loc_1_0033
        db  090h
loc_1_2084:
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_0033
        mov    ebx, dword ptr [eax + 0ch]
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_20AB
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_20AB
        test   byte ptr [ebx + 10h], 10h
        je     SHORT loc_1_210D
loc_1_20AB:
        mov    eax, dword ptr [esi + 24h]
        movzx  esi, byte ptr [eax]
        lea    esi, [eax + esi*2 + 1]
        movzx  eax, byte ptr [ebx + 0ah]
        add    ax, word ptr [esi]
        add    esi, 3
        call   sub_1_1CA4
        jb     SHORT loc_1_2112
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    esi, dword ptr [esi + 40h]
        mov    ebp, eax
        push   ebp
        call   sub_1_9EA4
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        jb     SHORT loc_1_2117
        pop    eax
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0
        mov    word ptr [ebx + 1], 0
        lea    eax, [ebx + 3]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
loc_1_210D:
        mov    eax, 40002h
loc_1_2112:
        jmp    NEAR PTR loc_1_0033
loc_1_2117:
        xchg   dword ptr [esp], eax
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        pop    eax
        sub    ah, ah
        shl    eax, 10h
        mov    al, 1
        jmp    NEAR PTR loc_1_0033
        db  02Eh, 08Bh, 0C0h
loc_1_2140:
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_0033
        mov    ebx, dword ptr [eax + 0ch]
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_2167
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_2167
        test   byte ptr [ebx + 10h], 80h
        je     SHORT loc_1_21C9
loc_1_2167:
        mov    eax, dword ptr [esi + 24h]
        movzx  esi, byte ptr [eax]
        lea    esi, [eax + esi*2 + 1]
        movzx  eax, byte ptr [ebx + 0ah]
        add    ax, word ptr [esi]
        add    esi, 3
        call   sub_1_1CA4
        jb     SHORT loc_1_21CE
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    esi, dword ptr [esi + 40h]
        mov    ebp, eax
        push   ebp
        call   sub_1_9DEC
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        jb     SHORT loc_1_21D3
        pop    eax
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0
        mov    word ptr [ebx + 1], 0
        lea    eax, [ebx + 3]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
loc_1_21C9:
        mov    eax, 40002h
loc_1_21CE:
        jmp    NEAR PTR loc_1_0033
loc_1_21D3:
        xchg   dword ptr [esp], eax
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        pop    eax
        sub    ah, ah
        shl    eax, 10h
        mov    al, 1
        jmp    NEAR PTR loc_1_0033
        db  000h, 000h, 000h
loc_1_21FC:
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 0
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        inc    byte ptr [ecx + 32h]
        mov    eax, dword ptr [ecx + 22h]
        mov    ecx, dword ptr [esi + 1ch]
        cmp    byte ptr [ecx + 5], 2
        je     SHORT loc_1_2290
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    di, word ptr [edx + 1ah]
        mov    esi, ecx
        mov    ecx, dword ptr [ebx + 3]
        mov    ebx, dword ptr [ebx + 7]
        mov    dl, 0
        call   sub_1_9984
        jb     SHORT loc_1_226F
loc_1_2238:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     SHORT loc_1_2260
        mov    word ptr [ecx + 46h], 0
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0
        mov    word ptr [ebx + 1], 0
        lea    eax, [ebx + 3]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0120
loc_1_2260:
        cmp    al, 21h
        je     SHORT loc_1_226A
        mov    word ptr [ecx + 46h], 0
loc_1_226A:
        jmp    NEAR PTR loc_1_0014
loc_1_226F:
        cmp    al, 0aah
        stc
        jne    SHORT loc_1_2238
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    eax, dword ptr [esi + 2ch]
        mov    ebx, dword ptr [esi + 24h]
        mov    edi, dword ptr [ebx + 7]
        xor    ebp, ebp
        call   sub_1_24CC
        jae    NEAR PTR loc_1_0154
loc_1_2290:
        mov    al, 21h
        cmp    byte ptr [ecx + 6], 1
        cmc
        jmp    SHORT loc_1_2238
        db  02Eh, 08Bh, 0C0h
loc_1_229C:
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 0
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        inc    byte ptr [ecx + 32h]
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        mov    di, word ptr [edx + 1ah]
        mov    eax, dword ptr [ecx + 22h]
        mov    ecx, dword ptr [ebx + 3]
        mov    ebx, dword ptr [ebx + 7]
        call   sub_1_997C
        jmp    NEAR PTR loc_1_2238
        db  08Bh, 0C0h
loc_1_22D0:
        movzx  ecx, word ptr [ebx + 5]
        mov    al, 0
        call   sub_1_18B8
        jb     SHORT loc_1_22DF
        jmp    SHORT loc_1_2305
loc_1_22DF:
        cmp    dword ptr [ebx + 0dh], 0
        jne    SHORT loc_1_22EB
        test   byte ptr [ebx + 7], 2
        jne    SHORT loc_1_22F0
loc_1_22EB:
        jmp    NEAR PTR loc_1_0033
loc_1_22F0:
        mov    ecx, dword ptr [esi + 1ch]
        jmp    0
loc_1_22F8:
        call   sub_1_8540
        mov    ecx, dword ptr [esi + 1ch]
        jmp    0
loc_1_2305:
        mov    di, word ptr [edi + 1ch]
        shl    edi, 10h
        cmp    dword ptr [ebx + 0dh], 0
        jne    SHORT loc_1_2318
        test   byte ptr [ebx + 7], 2
        jne    SHORT loc_1_22F8
loc_1_2318:
        inc    byte ptr [ecx + 32h]
        mov    eax, dword ptr [ecx + 22h]
        mov    ecx, dword ptr [esi + 1ch]
        cmp    byte ptr [ecx + 5], 2
        je     NEAR PTR loc_1_245D
        movzx  esi, byte ptr [ebx]
        lea    esi, [ebx + esi*2 + 3]
        movzx  ecx, word ptr [ebx + 0dh]
        jecxz  loc_1_2362
loc_1_2338:
        push   eax
        push   ecx
        push   esi
        push   edi
        mov    di, word ptr [esi]
        mov    ecx, dword ptr [esi + 6]
        mov    ebx, dword ptr [esi + 2]
        call   sub_1_997C
        pop    edi
        pop    esi
        pop    ecx
        jb     NEAR PTR loc_1_23F2
        pop    eax
        add    esi, 0ah
        loop   loc_1_2338
        db  08Bh, 01Dh, 014h, 028h, 001h, 000h
        mov    ebx, dword ptr [ebx + 24h]
loc_1_2362:
        movzx  ecx, word ptr [ebx + 0fh]
        jecxz  loc_1_23A3
loc_1_2368:
        mov    dl, byte ptr [ebx + 7]
        and    edx, 1
        cmp    dword ptr [ebx + 9], 1
        cmc
        sbb    ebp, ebp
        db  08Bh, 01Dh, 014h, 028h, 001h, 000h
        and    ebp, dword ptr [ebx + 1ch]
        mov    eax, eax
loc_1_2380:
        push   eax
        push   ecx
        push   edx
        push   esi
        push   edi
        push   ebp
        mov    di, word ptr [esi]
        mov    ecx, dword ptr [esi + 6]
        mov    ebx, dword ptr [esi + 2]
        mov    esi, ebp
        call   sub_1_9984
        pop    ebp
        pop    edi
        pop    esi
        pop    edx
        jb     SHORT loc_1_23F8
        pop    ecx
        pop    eax
        add    esi, 0ah
        loop   loc_1_2380
loc_1_23A3:
        xor    al, al
loc_1_23A5:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ebx, dword ptr [esi + 24h]
        mov    ecx, dword ptr [esi + 2ch]
        test   byte ptr [ebx + 7], 2
        je     SHORT loc_1_23BC
        call   sub_1_8540
loc_1_23BC:
        test   al, al
        jne    SHORT loc_1_23E3
        mov    word ptr [ecx + 46h], 0
        mov    ebx, dword ptr [esi + 28h]
        mov    eax, dword ptr [esi + 24h]
        mov    eax, dword ptr [eax + 1]
        mov    dword ptr [ebx + 1], eax
        mov    byte ptr [ebx], 2
        mov    word ptr [ebx + 5], 0
        lea    eax, [ebx + 7]
        jmp    NEAR PTR loc_1_00D0
loc_1_23E3:
        cmp    al, 21h
        je     SHORT loc_1_23ED
        mov    word ptr [ecx + 46h], 0
loc_1_23ED:
        jmp    NEAR PTR loc_1_0014
loc_1_23F2:
        pop    ecx
        jmp    SHORT loc_1_23A5
loc_1_23F5:
        pop    eax
        jmp    SHORT loc_1_23A5
loc_1_23F8:
        cmp    al, 0aah
        je     SHORT loc_1_2433
        pop    ecx
        xchg   dword ptr [esp], eax
loc_1_2400:
        db  08Bh, 01Dh, 014h, 028h, 001h, 000h
        mov    ebx, dword ptr [ebx + 24h]
        movzx  ecx, byte ptr [ebx]
        lea    ecx, [ebx + ecx*2 + 3]
        movzx  edx, word ptr [ebx + 0dh]
        lea    edx, [edx + edx*4]
        lea    edx, [ecx + edx*2]
loc_1_241A:
        cmp    edx, esi
        je     SHORT loc_1_23F5
        pushad
        mov    di, word ptr [edx]
        mov    ecx, dword ptr [edx + 6]
        mov    ebx, dword ptr [edx + 2]
        call   sub_1_997C
        popad
        add    edx, 0ah
        jmp    SHORT loc_1_241A
loc_1_2433:
        db  08Bh, 01Dh, 014h, 028h, 001h, 000h
        mov    eax, dword ptr [ebx + 2ch]
        mov    ebx, dword ptr [ebx + 24h]
        mov    ebp, dword ptr [ebx + 9]
        mov    edi, dword ptr [esi + 2]
        pop    edx
        call   sub_1_24CC
        pop    eax
        jb     SHORT loc_1_2453
        jmp    NEAR PTR loc_1_0154
loc_1_2453:
        mov    ebx, dword ptr [esi + 24h]
        mov    dword ptr [ebx + 9], 0
loc_1_245D:
        movzx  esi, word ptr [ecx + 42h]
        add    esi, ebx
        cmp    byte ptr [ecx + 6], 0
        jne    SHORT loc_1_247C
        movzx  ecx, word ptr [ecx + 5ah]
        dec    ecx
        je     NEAR PTR loc_1_23A3
        add    esi, 0ah
        jmp    NEAR PTR loc_1_2368
loc_1_247C:
        push   21h
        jmp    SHORT loc_1_2400
        db  083h, 0F8h, 0FFh, 074h, 02Ch, 083h, 0C0h, 012h, 083h, 078h, 008h, 000h, 074h, 007h, 0E8h, 0C5h
        db  06Ch, 000h, 000h, 072h, 01Bh
loc_1_2495:
        lea    ebx, [eax - 12h]
        call   sub_1_0338
        db  0B8h, 054h, 001h, 000h, 000h
        call   sub_1_89B4
        jb     SHORT loc_1_24B0
        db  0F0h, 0FEh, 005h, 0CBh, 01Ch, 001h, 000h
loc_1_24B0:
        ret
        db  08Bh, 0DEh, 00Fh, 0A4h, 0D8h, 010h, 0CCh, 0C3h, 02Eh, 08Bh, 0C0h
loc_1_24BC:
        push   ecx
        mov    eax, dword ptr [ecx + 3eh]
        call   sub_1_99B6
        pop    ecx
        sbb    al, al
        mov    byte ptr [ecx + 6], al
        ret
sub_1_24CC:
        push   edi
        db  08Bh, 03Dh, 014h, 028h, 001h, 000h
        mov    ebx, dword ptr [edi + 1ch]
        mov    dword ptr [ebx + 3eh], ecx
        mov    word ptr [ebx + 5ah], dx
        mov    edx, dword ptr [edi + 28h]
        mov    dword ptr [ebx + 28h], edx
        mov    edx, dword ptr [edi + 24h]
        mov    dword ptr [ebx + 24h], edx
        sub    esi, edx
        mov    word ptr [ebx + 42h], si
        mov    byte ptr [ebx + 5], 2
        mov    dword ptr [ebx + 1ah], 0
        pop    edi
        mov    edx, dword ptr [eax + 42h]
        mov    dword ptr [eax + 42h], edi
        cmp    ebp, -1
        je     SHORT loc_1_2582
        test   ebp, ebp
        jne    SHORT loc_1_2554
        db  08Bh, 02Dh, 0E0h, 020h, 001h, 000h
        cmp    word ptr [eax + 46h], 0
        je     SHORT loc_1_2554
        cmp    edx, edi
        jne    NEAR PTR loc_1_25A0
        movzx  edx, word ptr [eax + 46h]
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    esi, dword ptr [esi + 30h]
        cmp    byte ptr [esi + 3eh], 2
        ja     SHORT loc_1_253B
        db  066h, 02Bh, 015h, 0ECh, 020h, 001h, 000h
        jbe    SHORT loc_1_25A0
loc_1_253B:
        db  00Fh, 0AFh, 015h, 0E8h, 020h, 001h, 000h
        add    ebp, edx
        db  02Bh, 02Dh, 0E4h, 020h, 001h, 000h
        sbb    edx, edx
        and    ebp, edx
        db  003h, 02Dh, 0E4h, 020h, 001h, 000h
loc_1_2554:
        add    ebp, dword ptr [ebx + 44h]
        db  02Bh, 02Dh, 0C4h, 031h, 001h, 000h
        jle    SHORT loc_1_25A0
        inc    word ptr [eax + 46h]
        lea    eax, [ebx + 12h]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], ebp
        db  0C7h, 040h, 008h, 095h, 024h, 000h, 000h
        call   sub_1_911C
loc_1_2582:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        dec    byte ptr [ecx + 32h]
        jne    SHORT loc_1_259E
        cmp    dword ptr [ecx + 2ah], 0
        je     SHORT loc_1_259E
        lea    eax, [ecx + 2ah]
        call   sub_1_8BC0
loc_1_259E:
        clc
        ret
loc_1_25A0:
        inc    word ptr [eax + 46h]
        xchg   ecx, eax
        call   sub_1_99B6
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 1ch]
        sbb    al, al
        mov    byte ptr [ecx + 6], al
        stc
        ret
        db  000h, 000h
loc_1_25BC:
        movzx  ecx, word ptr [ebx + 1]
        cmp    cx, -1
        je     SHORT loc_1_2609
        mov    al, 0
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        inc    byte ptr [ecx + 32h]
        mov    eax, dword ptr [ecx + 22h]
        mov    ebp, 4000h
        call   sub_1_97C0
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     NEAR PTR loc_1_0014
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0
        mov    word ptr [ebx + 1], 0
        lea    eax, [ebx + 3]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0120
loc_1_2609:
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_0033
        mov    ebx, dword ptr [eax + 0ch]
        movzx  ebx, byte ptr [ebx + 0bh]
        sub    ax, ax
        call   sub_1_97DC
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0
        mov    word ptr [ebx + 1], 0
        lea    eax, [ebx + 3]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
loc_1_2644:
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 0
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        inc    byte ptr [ecx + 32h]
loc_1_2658:
        mov    eax, dword ptr [ecx + 22h]
        movzx  ecx, word ptr [ebx + 3]
        mov    ebx, dword ptr [ebx + 5]
        call   sub_1_9818
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     SHORT loc_1_268C
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 2
        mov    dword ptr [ebx + 1], eax
        mov    word ptr [ebx + 5], 0
        lea    eax, [ebx + 7]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0120
loc_1_268C:
        cmp    al, 83h
        jne    NEAR PTR loc_1_0014
        mov    edi, dword ptr [esi + 30h]
        cmp    byte ptr [edi + 3eh], 5
        jae    NEAR PTR loc_1_0014
        mov    ebx, dword ptr [esi + 24h]
        mov    dword ptr [ebx + 5], 0
        jmp    SHORT loc_1_2658
        db  02Eh, 08Bh, 0C0h
loc_1_26B0:
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 0
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        inc    byte ptr [ecx + 32h]
        mov    eax, dword ptr [ecx + 22h]
        mov    edi, dword ptr [esi + 28h]
        add    edi, 1
        mov    ebp, dword ptr [ecx + 26h]
        call   sub_1_9850
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     NEAR PTR loc_1_0014
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0bh
        mov    word ptr [ebx + 17h], 0
        lea    eax, [ebx + 19h]
        mov    dword ptr [esi + 28h], eax
        mov    edi, dword ptr [esi + 30h]
        cmp    byte ptr [edi + 3eh], 6
        jae    NEAR PTR loc_1_0120
        xor    ebp, ebp
        mov    dword ptr [ebx + 1], ebp
        mov    dword ptr [ebx + 5], ebp
        jmp    NEAR PTR loc_1_0120
loc_1_2710:
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 0
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        inc    byte ptr [ecx + 32h]
        mov    eax, dword ptr [ecx + 22h]
        lea    esi, [ebx + 3]
        call   sub_1_9900
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     NEAR PTR loc_1_0014
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0
        mov    word ptr [ebx + 1], 0
        lea    eax, [ebx + 3]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0120
        db  02Eh, 08Bh, 0C0h
loc_1_2758:
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_0033
        push   0
        push   0
        push   dword ptr [esi + 40h]
        sub    esp, 1ch
        mov    ebp, esp
        mov    ebx, dword ptr [eax + 0ch]
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_278F
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_278F
        test   byte ptr [ebx + 10h], 10h
        je     NEAR PTR loc_1_2829
loc_1_278F:
        mov    cx, word ptr [edi + 1ch]
        shl    ecx, 10h
        mov    edx, dword ptr [esi + 20h]
        mov    cx, word ptr [edx + 1ah]
        mov    dword ptr [ebp], ecx
        mov    dword ptr [ebp + 1ch], edi
        mov    eax, dword ptr [esi + 24h]
        movzx  edx, word ptr [eax + 1]
        mov    dword ptr [ebp + 10h], edx
        movzx  esi, byte ptr [eax]
        lea    esi, [eax + esi*2 + 1]
        movzx  eax, byte ptr [ebx + 0ah]
        add    ax, word ptr [esi]
        add    esi, 3
        call   sub_1_1CA4
        jb     SHORT loc_1_282E
        mov    dword ptr [ebp + 4], eax
loc_1_27C8:
        push   ebp
        mov    edx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp]
        mov    ebx, dword ptr [ebp + 1ch]
        mov    ebp, dword ptr [ebp + 4]
        call   sub_1_9D78
        pop    ebp
        jb     SHORT loc_1_2819
        mov    eax, dword ptr [ebp + 4]
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0
        mov    word ptr [ebx + 1], 0
        lea    eax, [ebx + 3]
        mov    dword ptr [esi + 28h], eax
        add    esp, 28h
        jmp    NEAR PTR loc_1_0134
loc_1_2819:
        cmp    al, 0aah
        jne    SHORT loc_1_2848
        mov    eax, dword ptr [ebp + 4]
        call   sub_1_1F3C
        jae    SHORT loc_1_27C8
        jmp    SHORT loc_1_2848
loc_1_2829:
        mov    eax, 40002h
loc_1_282E:
        add    esp, 28h
        jmp    NEAR PTR loc_1_0033
        db  0B0h, 008h, 0EBh, 00Eh, 08Bh, 055h, 00Ch, 051h, 052h, 052h, 0E8h, 0B0h, 064h, 000h, 000h, 05Ah
        db  05Ah, 059h
loc_1_2848:
        mov    edx, dword ptr [ebp + 4]
        push   eax
        push   ecx
        push   edx
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        add    esp, 28h
        cmp    al, 0eeh
        jne    NEAR PTR loc_1_002C
        jmp    NEAR PTR loc_1_83C8
loc_1_286F:
        mov    eax, 40002h
        add    esp, 28h
loc_1_2877:
        jmp    NEAR PTR loc_1_0033
loc_1_287C:
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     SHORT loc_1_2877
        push   dword ptr [esi + 40h]
        sub    esp, 24h
        mov    ebp, esp
        mov    ebx, dword ptr [eax + 0ch]
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_28AA
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_28AA
        mov    al, 14h
        and    al, byte ptr [ebx + 10h]
        cmp    al, 14h
        jne    SHORT loc_1_286F
loc_1_28AA:
        mov    cx, word ptr [edi + 1ch]
        shl    ecx, 10h
        mov    edx, dword ptr [esi + 20h]
        mov    cx, word ptr [edx + 1ah]
        mov    dword ptr [ebp], ecx
        mov    eax, dword ptr [esi + 24h]
        movzx  edx, word ptr [eax + 1]
        mov    dword ptr [ebp + 18h], edx
        movzx  esi, byte ptr [eax]
        lea    esi, [eax + esi*2 + 1]
        movzx  eax, byte ptr [ebx + 0ah]
        add    eax, eax
        add    ax, word ptr [esi]
        add    esi, 3
        push   ebx
        call   sub_1_1CA4
        pop    ebx
        jb     NEAR PTR loc_1_29A8
        mov    dword ptr [ebp + 4], eax
        mov    eax, edi
        inc    esi
        call   sub_1_1CD4
        jb     NEAR PTR loc_1_298F
        mov    dword ptr [ebp + 8], eax
        mov    dword ptr [ebp + 20h], ecx
loc_1_28FC:
        push   ebp
        mov    edx, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 24h]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 20h]
        mov    edi, dword ptr [ebp]
        mov    ebp, dword ptr [ebp + 4]
        call   sub_1_9EAC
        pop    ebp
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        jb     SHORT loc_1_2950
        mov    eax, dword ptr [ebp + 4]
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0
        mov    word ptr [ebx + 1], 0
        lea    eax, [ebx + 3]
        mov    dword ptr [esi + 28h], eax
        add    esp, 28h
        jmp    NEAR PTR loc_1_0134
loc_1_2950:
        cmp    al, 0aah
        jne    SHORT loc_1_2960
        mov    eax, dword ptr [ebp + 4]
        call   sub_1_1F3C
        jae    SHORT loc_1_28FC
        jmp    SHORT loc_1_2960
loc_1_2960:
        mov    edx, dword ptr [ebp + 4]
        push   eax
        push   ecx
        push   edx
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        add    esp, 28h
        cmp    al, 0eeh
        je     NEAR PTR loc_1_83C8
        jmp    NEAR PTR loc_1_002C
        db  083h, 0C4h, 004h, 0B8h, 001h, 000h, 008h, 000h
loc_1_298F:
        mov    ebx, dword ptr [ebp + 4]
        push   eax
        push   ecx
        push   edx
        mov    edx, ebx
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
loc_1_29A8:
        add    esp, 28h
        jmp    NEAR PTR loc_1_0033
loc_1_29B0:
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_0033
        mov    ebx, dword ptr [eax + 0ch]
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_29D7
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_29D7
        test   byte ptr [ebx + 10h], 80h
        je     SHORT loc_1_2A4D
loc_1_29D7:
        mov    edx, dword ptr [esi + 28h]
        mov    eax, dword ptr [esi + 24h]
        movzx  esi, byte ptr [eax]
        lea    esi, [eax + esi*2 + 1]
        movzx  eax, byte ptr [ebx + 0ah]
        add    ax, word ptr [esi]
        add    esi, 3
        call   sub_1_1CA4
        jb     SHORT loc_1_2A52
        mov    ebp, eax
        push   ebp
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    esi, dword ptr [esi + 40h]
        xor    edx, edx
        call   sub_1_9F3C
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        jb     SHORT loc_1_2A57
        mov    edx, dword ptr [esi + 28h]
        mov    byte ptr [edx], 0ah
        mov    word ptr [edx + 1], bx
        mov    dword ptr [edx + 3], eax
        mov    dword ptr [edx + 7], ebp
        sub    eax, eax
        mov    dword ptr [edx + 0bh], eax
        mov    dword ptr [edx + 0fh], eax
        mov    dword ptr [edx + 13h], eax
        lea    eax, [edx + 17h]
        mov    dword ptr [esi + 28h], eax
        pop    eax
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        jmp    NEAR PTR loc_1_0134
loc_1_2A4D:
        mov    eax, 40002h
loc_1_2A52:
        jmp    NEAR PTR loc_1_0033
loc_1_2A57:
        pop    edx
        push   eax
        push   ecx
        push   edx
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        cmp    al, 0eeh
        jne    NEAR PTR loc_1_002C
        jmp    NEAR PTR loc_1_83C8
        db  02Eh, 08Bh, 0C0h
loc_1_2A7C:
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_0033
        mov    ebx, dword ptr [eax + 0ch]
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_2AA3
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_2AA3
        test   byte ptr [ebx + 10h], 20h
        je     SHORT loc_1_2B21
loc_1_2AA3:
        mov    edx, dword ptr [esi + 24h]
        movzx  esi, byte ptr [edx]
        lea    esi, [edx + esi*2 + 1]
        movzx  eax, byte ptr [ebx + 0ah]
        add    ax, word ptr [esi]
        add    esi, 3
        call   sub_1_1CA4
        jb     SHORT loc_1_2B26
        mov    ebp, eax
        movzx  ebx, word ptr [edx + 1]
        mov    eax, dword ptr [edx + 3]
        push   ebp
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 30h]
        mov    di, word ptr [ecx + 1ch]
        shl    edi, 10h
        mov    ecx, dword ptr [esi + 20h]
        mov    di, word ptr [ecx + 1ah]
        mov    esi, dword ptr [esi + 40h]
        sub    ecx, ecx
        sub    edx, edx
        call   sub_1_A058
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        jb     SHORT loc_1_2B2B
        pop    eax
        push   eax
        push   ecx
        push   edx
        mov    edx, eax
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0
        mov    word ptr [ebx + 1], 0
        lea    eax, [ebx + 3]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
loc_1_2B21:
        mov    eax, 40002h
loc_1_2B26:
        jmp    NEAR PTR loc_1_0033
loc_1_2B2B:
        pop    edx
        push   eax
        push   ecx
        push   edx
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        cmp    al, 0eeh
        jne    NEAR PTR loc_1_002C
        jmp    NEAR PTR loc_1_83C8
loc_1_2B4D:
        lea    ecx, [edi + 50h]
        movzx  edi, word ptr [edi + 1ch]
        mov    dword ptr [esi + 34h], ecx
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [esi + 34h], ecx
        je     SHORT loc_1_2BAA
        shl    edi, 10h
        mov    di, word ptr [edx + 1ah]
        mov    dword ptr [esi + 24h], edi
loc_1_2B68:
        cmp    byte ptr [ecx + 19h], 1
        jne    SHORT loc_1_2B86
        mov    eax, dword ptr [ecx + 22h]
        mov    ebx, 0ffffffffh
        push   ecx
        call   sub_1_99B4
        pop    ecx
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [esi + 24h]
loc_1_2B86:
        cmp    word ptr [ecx + 16h], di
        je     SHORT loc_1_2B95
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [esi + 34h], ecx
        jne    SHORT loc_1_2B68
        jmp    SHORT loc_1_2BAA
loc_1_2B95:
        mov    dword ptr [esi + 2ch], ecx
        push   dword ptr [ecx]
        mov    al, 1
        call   sub_1_1E8C
        mov    edi, dword ptr [esi + 24h]
        pop    ecx
        cmp    dword ptr [esi + 34h], ecx
        jne    SHORT loc_1_2B68
loc_1_2BAA:
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0
        mov    word ptr [ebx + 1], 0
        lea    eax, [ebx + 3]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
        db  02Eh, 08Bh, 0C0h
loc_1_2BC4:
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_0033
        mov    ebx, dword ptr [eax + 0ch]
        movzx  eax, byte ptr [ebx + 0bh]
        call   sub_1_992C
        jb     NEAR PTR loc_1_002C
loc_1_2BE5:
        test   ecx, 0ffff0000h
        je     SHORT loc_1_2BF5
        add    esi, esi
        shr    ecx, 1
        shr    edx, 1
        jmp    SHORT loc_1_2BE5
loc_1_2BF5:
        mov    ebp, esi
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 5
        mov    word ptr [ebx + 1], cx
        mov    word ptr [ebx + 3], bp
        mov    word ptr [ebx + 5], di
        mov    word ptr [ebx + 7], dx
        mov    word ptr [ebx + 9], 0
        mov    word ptr [ebx + 0bh], 0
        lea    eax, [ebx + 0dh]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
        db  08Bh, 0C0h
loc_1_2C2C:
        cmp    word ptr [ebx + 1], 0
        je     0
        mov    ax, word ptr [edx + 18h]
        cmp    ax, -1
        je     SHORT loc_1_2C4C
        call   sub_1_196C
        jb     NEAR PTR loc_1_0033
loc_1_2C4C:
        mov    ebx, dword ptr [esi + 28h]
        mov    word ptr [ebx + 1], 1
        movzx  eax, word ptr [ebx + 3]
        lea    eax, [ebx + eax + 5]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
        db  02Eh, 08Bh, 0C0h
loc_1_2C68:
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 0
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        inc    byte ptr [ecx + 32h]
        cmp    word ptr [ebx + 3], 53h
        jne    SHORT loc_1_2C8C
        cmp    word ptr [ebx + 5], 60h
        jne    SHORT loc_1_2C8C
        jmp    SHORT loc_1_2C94
loc_1_2C8C:
        mov    al, 32h
        jmp    NEAR PTR loc_1_0014
        db  090h
loc_1_2C94:
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 8
        lea    eax, [ebx + 16h]
        mov    edx, eax
        and    eax, 0fffffffch
        mov    edi, eax
        sub    eax, dword ptr [esi + 20h]
        mov    word ptr [ebx + 0dh], ax
        not    edx
        and    edx, 3
        add    edx, 20h
        mov    word ptr [ebx + 11h], dx
        mov    word ptr [ebx + 3], 20h
        mov    word ptr [ebx + 0bh], 20h
        lea    eax, [edi + 20h]
        mov    dword ptr [esi + 28h], eax
        mov    eax, dword ptr [ecx + 1ah]
        stosw
        sub    eax, eax
        mov    word ptr [ebx + 1], ax
        mov    word ptr [ebx + 5], ax
        mov    word ptr [ebx + 7], ax
        mov    word ptr [ebx + 9], ax
        mov    word ptr [ebx + 0fh], ax
        db  0BEh, 092h, 024h, 001h, 000h
        movsd
        movsd
        movsd
        movsd
        mov    esi, dword ptr [ecx + 10h]
        mov    esi, dword ptr [esi + 0ch]
        lea    esi, [esi + 11h]
        push   ecx
        mov    ecx, 0eh
        call   sub_1_7E13
        pop    ecx
        jmp    NEAR PTR loc_1_0120
        db  000h
loc_1_2D08:
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_0033
        mov    ebx, dword ptr [eax + 0ch]
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_2D33
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_2D33
        test   byte ptr [ebx + 10h], 80h
        je     NEAR PTR loc_1_2DF1
loc_1_2D33:
        mov    edx, dword ptr [esi + 24h]
        movzx  eax, byte ptr [edx]
        lea    eax, [edx + eax*2 + 4]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_2D57
        cmp    word ptr [eax + 2], 0
        jne    NEAR PTR loc_1_2E0E
        mov    eax, 120001h
        jmp    NEAR PTR loc_1_0033
loc_1_2D57:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        or     byte ptr [esi + 9], 2
        mov    esi, eax
        movzx  eax, byte ptr [ebx + 0ah]
        add    ax, word ptr [esi - 3]
        call   sub_1_1CA4
        jb     SHORT loc_1_2DE2
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        and    byte ptr [esi + 9], 0fdh
        mov    edi, dword ptr [esi + 28h]
        add    edi, 8
        mov    bx, word ptr [edx + 3]
        movzx  ecx, word ptr [edx + 1]
        mov    ebp, eax
        mov    edx, dword ptr [esi + 1ch]
        movzx  edx, byte ptr [edx + 7]
        db  08Bh, 014h, 095h, 0B0h, 01Bh, 001h, 000h
        movzx  edx, word ptr [edx + 0ah]
        add    edx, dword ptr [esi + 20h]
        sub    edx, edi
        mov    esi, dword ptr [esi + 40h]
        push   ebp
        sub    eax, eax
        call   sub_1_A098
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        pop    ebx
        pushfd
        push   eax
        push   ecx
        push   edx
        mov    edx, ebx
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        popfd
        jb     SHORT loc_1_2DF8
        mov    edx, dword ptr [esi + 28h]
        mov    eax, dword ptr [edx + 19h]
        mov    ebx, dword ptr [esi + 34h]
        mov    dword ptr [ebx], eax
        mov    al, byte ptr [edx + 8]
        mov    byte ptr [ebx + 6], al
        jmp    SHORT loc_1_2E57
loc_1_2DE2:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        and    byte ptr [esi + 9], 0fdh
loc_1_2DEC:
        jmp    NEAR PTR loc_1_0033
loc_1_2DF1:
        mov    eax, 40002h
        jmp    SHORT loc_1_2DEC
loc_1_2DF8:
        cmp    al, 5
        jne    SHORT loc_1_2DFE
        mov    al, 12h
loc_1_2DFE:
        jmp    NEAR PTR loc_1_002C
loc_1_2E03:
        popad
        mov    eax, 10002h
        jmp    NEAR PTR loc_1_0033
loc_1_2E0E:
        lea    ebx, [eax + 4]
        mov    edi, dword ptr [esi + 34h]
        pushad
        push   ebx
        push   edi
        call   sub_1_14E7C
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_2E03
        popad
        mov    al, byte ptr [edi + 6]
        mov    byte ptr [ebx], al
        movzx  ecx, word ptr [edx + 1]
        mov    edi, dword ptr [esi + 28h]
        add    edi, 8
        mov    edx, dword ptr [esi + 1ch]
        movzx  edx, byte ptr [edx + 7]
        db  08Bh, 014h, 095h, 0B0h, 01Bh, 001h, 000h
        movzx  edx, word ptr [edx + 0ah]
        add    edx, dword ptr [esi + 20h]
        sub    edx, edi
        call   sub_1_A0EC
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        jb     SHORT loc_1_2DFE
loc_1_2E57:
        mov    edx, dword ptr [esi + 28h]
        mov    byte ptr [edx], 1
        mov    word ptr [edx + 1], cx
        mov    byte ptr [edx + 5], 5
        imul   eax, ecx, 2bh
        mov    word ptr [edx + 6], ax
        add    eax, 3
        mov    word ptr [edx + 3], ax
        lea    eax, [eax + edx + 5]
        mov    dword ptr [esi + 28h], eax
        push   ecx
        add    edx, 8
        push   edx
        mov    edi, dword ptr [esi + 34h]
        push   edi
        call   sub_1_14E1C
        add    esp, 0ch
        jmp    NEAR PTR loc_1_0134
loc_1_2E90:
        mov    edx, dword ptr [esi + 28h]
        mov    byte ptr [edx], 1
        mov    word ptr [edx + 1], 0
        mov    byte ptr [edx + 5], 5
        mov    word ptr [edx + 6], 0
        mov    word ptr [edx + 3], 3
        lea    eax, [edx + 8]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
        db  090h, 0F9h, 0C3h, 08Bh, 0C0h
sub_1_2EBC:
        push   ebx
        push   ecx
        push   edx
        push   edi
        mov    dword ptr [edx], 2a5c3a40h
        add    byte ptr [edx], bl
        mov    dword ptr [edx + 4], 2a2eh
        call   sub_1_9AE8
        jb     SHORT loc_1_2F22
        call   sub_1_9B0C
        jb     SHORT loc_1_2F22
        mov    dword ptr [eax], 4e00h
        mov    dword ptr [eax + 8], 8
        mov    dword ptr [eax + 0ch], edx
        mov    word ptr [eax + 28h], 80h
        call   sub_1_9B3C
        jb     SHORT loc_1_2F22
        mov    esi, dword ptr [eax + 20h]
        test   byte ptr [esi + 15h], 8
        je     SHORT loc_1_2F22
        mov    ecx, 0bh
        mov    edi, edx
        lea    esi, [esi + 1eh]
loc_1_2F0D:
        lodsb
        cmp    al, 2eh
        je     SHORT loc_1_2F0D
        or     al, al
        stosb
        je     SHORT loc_1_2F19
        loop   loc_1_2F0D
loc_1_2F19:
        sub    ecx, 0bh
        neg    ecx
        mov    eax, ecx
        jmp    SHORT loc_1_2F29
loc_1_2F22:
        xor    eax, eax
        mov    edi, edx
        stosd
        stosd
        stosd
loc_1_2F29:
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        ret
        db  000h, 000h
sub_1_2F30:
        test   esp, 0ffffffffh
        ret
        db  090h
sub_1_2F38:
        mov    dword ptr [edi + 38h], 0
        push   eax
        push   edi
        push   ecx
        push   ecx
        push   edx
        push   34h
        call   sub_1_8CB6
        pop    edx
        pop    edx
        pop    ecx
        test   eax, eax
        je     SHORT loc_1_2FCF
        pop    ecx
        push   edx
        push   esi
        push   eax
        lea    edi, [eax + 8]
        mov    edx, 14h
        jecxz  loc_1_2F6D
loc_1_2F60:
        lodsb
        test   al, al
        je     SHORT loc_1_2F76
        test   edx, edx
        je     SHORT loc_1_2F6B
        stosb
        dec    edx
loc_1_2F6B:
        loop   loc_1_2F60
loc_1_2F6D:
        pop    eax
        mov    byte ptr [edi], 0
        lea    edi, [eax + 24h]
        jmp    SHORT loc_1_2F8C
loc_1_2F76:
        pop    eax
        mov    byte ptr [edi], 0
        lea    edi, [eax + 24h]
        dec    ecx
        je     SHORT loc_1_2F8C
        cmp    ecx, 0fh
        jbe    SHORT loc_1_2F8A
        mov    ecx, 0fh
loc_1_2F8A:
        rep movsb
loc_1_2F8C:
        mov    byte ptr [edi], 0
        pop    esi
        pop    edx
        pop    edi
        mov    dword ptr [edi + 38h], eax
        sub    ecx, ecx
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], ecx
        mov    byte ptr [eax + 1dh], cl
        mov    word ptr [eax + 22h], cx
        mov    dword ptr [eax + 1eh], ecx
        push   eax
        db  0A1h, 014h, 028h, 001h, 000h
        pop    dword ptr [eax + 3ch]
        mov    eax, ecx
        call   sub_1_2F30
        sete   al
        or     byte ptr [edi + 1fh], al
        pop    ebx
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_2FCC
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_2FD9
loc_1_2FCC:
        sub    ecx, ecx
        ret
loc_1_2FCF:
        mov    eax, 590002h
        pop    ecx
        pop    edi
        pop    eax
        stc
        ret
loc_1_2FD9:
        cmp    byte ptr [esi], 0
        je     SHORT loc_1_2FCC
        push   edx
        push   ebp
        push   esp
        cmp    ebp, 18h
        je     SHORT loc_1_302B
        cmp    word ptr [ebx], 20h
        je     SHORT loc_1_302B
        pop    eax
        sub    esp, 0fch
        push   eax
        lea    eax, [esp + 1ch]
        pushad
        push   eax
        lea    esi, [eax - 18h]
        push   esi
        push   dword ptr [edi + 24h]
        mov    edi, esi
        mov    ecx, 6
        sub    eax, eax
        rep stosd
        mov    edi, esi
        mov    esi, ebx
loc_1_3010:
        lodsb
        stosb
        or     al, al
        jne    SHORT loc_1_3010
        db  0CDh, 020h, 005h, 000h, 080h, 004h
        add    esp, 0ch
        popad
        mov    ebx, eax
        mov    byte ptr [ebx + 19h], 0
        mov    ebp, 18h
loc_1_302B:
        sub    eax, eax
        push   eax
        push   eax
        mov    eax, esp
        push   eax
        add    eax, 4
        push   eax
        movzx  eax, byte ptr [edi + 4eh]
        push   eax
        push   dword ptr [edi + 24h]
        push   0
        push   0
        push   ebp
        push   ebx
        lea    eax, [edi + 4]
        push   eax
        push   esi
        push   esi
        db  0A1h, 014h, 028h, 001h, 000h
        mov    eax, dword ptr [eax + 3ch]
        lea    eax, [eax + 24h]
        push   eax
        push   eax
        db  0CDh, 020h, 006h, 000h, 093h, 004h
        add    esp, 34h
        pop    ecx
        pop    edx
        pop    esp
        pop    ebp
        pop    edx
        or     eax, eax
        jne    SHORT loc_1_30C2
        or     ax, cx
        jne    SHORT loc_1_30C2
        db  08Bh, 00Dh, 014h, 028h, 001h, 000h
        mov    eax, dword ptr [ecx + 3ch]
        or     byte ptr [eax + 1eh], 2
        push   edx
        lea    ecx, [eax + 8]
        push   ecx
        lea    ecx, [eax + 24h]
        push   ecx
        db  0CDh, 020h, 005h, 000h, 091h, 004h
        add    esp, 8
        mov    ecx, eax
        xor    eax, eax
        pop    edx
        or     ecx, ecx
        je     NEAR PTR loc_1_2FCC
        or     byte ptr [edi + 1fh], 1
        db  083h, 03Dh, 03Ch, 034h, 001h, 000h, 000h
        jne    NEAR PTR loc_1_2FCC
        push   eax
        push   edx
        push   0
        call   sub_1_BA80
        pop    edx
        pop    edx
        pop    eax
        db  0FFh, 005h, 03Ch, 034h, 001h, 000h
        jmp    NEAR PTR loc_1_2FCC
loc_1_30C2:
        cmp    eax, 51fh
        jne    SHORT loc_1_30E4
        cmp    byte ptr [edi + 3eh], 4
        jbe    SHORT loc_1_30E4
        cmp    byte ptr [edi + 3eh], 7
        jbe    SHORT loc_1_30DD
        shl    eax, 10h
        or     eax, 1
        jmp    SHORT loc_1_30E9
loc_1_30DD:
        mov    eax, 8a70001h
        jmp    SHORT loc_1_30E9
loc_1_30E4:
        mov    eax, 50001h
loc_1_30E9:
        stc
        ret
        db  090h
loc_1_30EC:
        cmp    word ptr [edi + 48h], 0
        jne    SHORT loc_1_30FB
        mov    ax, word ptr [ebx + 5]
        mov    word ptr [edi + 48h], ax
loc_1_30FB:
        movzx  eax, byte ptr [ebx]
        lea    eax, [ebx + eax*2 + 3]
        movzx  esi, word ptr [ebx + 0fh]
        mov    ebp, esi
        movzx  ecx, word ptr [eax - 2]
        cmp    byte ptr [edi + 3eh], 8
        jne    SHORT loc_1_3116
        add    si, word ptr [ebx + 11h]
loc_1_3116:
        sub    ecx, esi
        add    esi, eax
        mov    bx, word ptr [ebx + 9]
        call   sub_1_2F38
        jb     NEAR PTR loc_1_0033
        mov    word ptr [edx + 1ch], cx
        cmp    byte ptr [edi + 3eh], 8
        jne    SHORT loc_1_3139
        mov    ecx, dword ptr [edx + 17h]
        mov    dword ptr [edi + 20h], ecx
loc_1_3139:
        test   byte ptr [edx + 9], 10h
        je     SHORT loc_1_3143
        or     byte ptr [edi + 1fh], 10h
loc_1_3143:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ebx, dword ptr [esi + 24h]
        mov    ecx, dword ptr [ebx + 1]
        mov    ebx, dword ptr [esi + 28h]
        mov    dword ptr [ebx + 1], ecx
        mov    byte ptr [ebx], 3
        mov    word ptr [ebx + 5], ax
        sub    ecx, ecx
        mov    word ptr [ebx + 7], cx
        cmp    byte ptr [edi + 3eh], 7
        jb     SHORT loc_1_3181
        lea    edi, [ebx + 9]
        db  08Bh, 035h, 0A8h, 01Bh, 001h, 000h
        db  08Bh, 00Dh, 0A4h, 01Bh, 001h, 000h
        mov    word ptr [ebx + 7], cx
        rep movsb
        movzx  ecx, word ptr [ebx + 7]
loc_1_3181:
        lea    ecx, [ebx + ecx + 9]
        jmp    NEAR PTR loc_1_00E4
loc_1_318A:
        mov    eax, 10002h
loc_1_318F:
        jmp    NEAR PTR loc_1_0033
loc_1_3194:
        movzx  eax, byte ptr [ebx]
        lea    ebx, [ebx + eax*2 + 4]
        movzx  ecx, word ptr [ebx - 3]
        dec    ecx
        js     SHORT loc_1_318A
        mov    edi, ebx
        sub    al, al
        repne scasb
        jne    SHORT loc_1_318A
        inc    edi
        dec    ecx
        js     SHORT loc_1_318A
        mov    edx, edi
        repne scasb
        jne    SHORT loc_1_318A
        mov    ebp, edi
        sub    ebp, edx
        inc    edi
        dec    ecx
        mov    esi, dword ptr [esi + 30h]
        test   byte ptr [esi + 1fh], 8
        jne    SHORT loc_1_31E8
        push   ebx
        push   edi
        push   esi
        lea    edi, [esi + 4]
        push   edi
        mov    cl, 10h
        repne scasb
        sub    edi, dword ptr [esp]
        mov    ecx, edi
        mov    edi, esi
        pop    esi
        mov    eax, edx
        xor    ebx, ebx
        call   sub_1_2F38
        pop    esi
        pop    edi
        pop    ebx
        jb     NEAR PTR loc_1_0033
loc_1_31E8:
        push   edi
        call   sub_1_33E0
        pop    edx
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [esi + 30h]
        jb     SHORT loc_1_318F
        je     SHORT loc_1_3259
        call   sub_1_3368
        jb     SHORT loc_1_318F
        mov    dword ptr [ecx + 8], edi
        inc    word ptr [edi + 42h]
        or     byte ptr [edi + 1fh], 8
        mov    eax, dword ptr [edi + 34h]
        mov    dword ptr [ecx + 10h], eax
        mov    word ptr [ecx + 18h], 0
        mov    word ptr [ecx + 1ah], 0cccch
        mov    dword ptr [ecx + 1ch], 0
        mov    eax, dword ptr [esi + 28h]
        mov    byte ptr [eax], 2
        movzx  edx, byte ptr [edi + 3fh]
        db  08Bh, 014h, 095h, 0B0h, 01Bh, 001h, 000h
        mov    dx, word ptr [edx + 0ah]
        mov    word ptr [eax + 1], dx
        mov    dx, word ptr [ecx + 16h]
        mov    word ptr [eax + 3], dx
        mov    word ptr [eax + 5], 0
        add    eax, 7
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
loc_1_3259:
        mov    eax, 60002h
        jmp    NEAR PTR loc_1_0033
loc_1_3263:
        pop    edi
        mov    eax, 10002h
loc_1_3269:
        jmp    NEAR PTR loc_1_0033
loc_1_326E:
        mov    eax, 60002h
        jmp    NEAR PTR loc_1_0033
loc_1_3278:
        test   word ptr [ebx + 5], 1
        je     SHORT loc_1_3294
        pushad
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     SHORT loc_1_3293
        mov    bl, 5
        call   sub_1_3730
loc_1_3293:
        popad
loc_1_3294:
        push   edi
        movzx  ebp, word ptr [ebx + 7]
        movzx  eax, byte ptr [ebx]
        lea    edx, [ebx + eax*2 + 3]
        movzx  ecx, word ptr [edx - 2]
        sub    ecx, ebp
        js     SHORT loc_1_3263
        lea    ebx, [ebp + edx]
        mov    edi, ebx
        sub    al, al
        repne scasb
        jne    SHORT loc_1_3263
        mov    esi, dword ptr [esp]
        push   edi
        call   sub_1_33E0
        pop    edx
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        pop    edi
        jb     SHORT loc_1_3269
        je     SHORT loc_1_326E
        call   sub_1_3368
        jb     SHORT loc_1_3269
        mov    dword ptr [ecx + 8], edi
        inc    word ptr [edi + 42h]
        or     byte ptr [edi + 1fh], 8
        mov    eax, dword ptr [edi + 34h]
        mov    dword ptr [ecx + 10h], eax
        mov    word ptr [ecx + 18h], 0
        mov    word ptr [ecx + 1ah], 0cccch
        mov    dword ptr [ecx + 1ch], 0
        mov    edx, dword ptr [esi + 20h]
        mov    ax, word ptr [ecx + 16h]
        mov    word ptr [edx + 18h], ax
        movzx  eax, byte ptr [ebx + 9]
        mov    ebx, dword ptr [esi + 28h]
        mov    edx, dword ptr [esi + 24h]
        mov    edx, dword ptr [edx + 1]
        mov    dword ptr [ebx + 1], edx
        mov    byte ptr [ebx], 2
        db  08Bh, 034h, 085h, 018h, 021h, 001h, 000h
        mov    ecx, dword ptr [esi - 4]
        mov    word ptr [ebx + 5], cx
        lea    edi, [ebx + 7]
        rep movsb
        mov    ecx, edi
        jmp    NEAR PTR loc_1_00E4
loc_1_332C:
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_0033
        mov    bl, 5
        call   sub_1_3730
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        jb     NEAR PTR loc_1_0033
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0
        mov    word ptr [ebx + 1], 0
        lea    eax, [ebx + 3]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
        db  02Eh, 08Bh, 0C0h
sub_1_3368:
        cmp    byte ptr [ebx + 9], 1
        jne    SHORT loc_1_33AA
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        je     SHORT loc_1_33AA
        push   eax
        push   ecx
        push   esi
        push   edi
        mov    esi, dword ptr [ebx + 2ch]
        db  0BFh, 037h, 021h, 001h, 000h
        mov    ecx, 8
        rep movsd
        db  08Bh, 00Dh, 014h, 028h, 001h, 000h
        db  0BEh, 030h, 021h, 001h, 000h
        call   sub_1_9C43
        test   cl, 4
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        jne    SHORT loc_1_33AA
        mov    eax, 40002h
        stc
        ret
loc_1_33AA:
        cmp    dword ptr [edx], 3f3f3f3fh
        je     SHORT loc_1_33DC
        cmp    byte ptr [ebx + 9], 0
        je     SHORT loc_1_33D5
        pushad
        mov    edi, edx
        movzx  eax, byte ptr [ebx + 9]
        db  08Bh, 034h, 085h, 018h, 021h, 001h, 000h
        mov    ecx, dword ptr [esi - 4]
        repe cmpsb
        popad
        je     SHORT loc_1_33DC
loc_1_33CE:
        mov    eax, 70002h
        stc
        ret
loc_1_33D5:
        cmp    word ptr [edx + 1], 3ah
        jne    SHORT loc_1_33CE
loc_1_33DC:
        clc
        ret
        db  08Bh, 0C0h
sub_1_33E0:
        push   esi
        cmp    word ptr [ebx], 5c5ch
        je     SHORT loc_1_33EF
        cmp    word ptr [ebx], 2f2fh
        jne    SHORT loc_1_3401
loc_1_33EF:
        lea    esi, [ebx + 2]
loc_1_33F2:
        call   sub_1_9CC0
        je     SHORT loc_1_33FE
        cmp    al, 5ch
        jne    SHORT loc_1_33F2
        inc    esi
loc_1_33FE:
        lea    ebx, [esi - 1]
loc_1_3401:
        sub    al, al
        mov    ecx, 0ffffh
        mov    edi, ebx
        repne scasb
        mov    eax, edi
        sub    eax, ebx
        cmp    eax, 0dh
        ja     SHORT loc_1_343B
        push   ebp
        db  08Bh, 02Dh, 000h, 01Ch, 001h, 000h
loc_1_341C:
        db  081h, 0FDh, 000h, 01Ch, 001h, 000h
        je     SHORT loc_1_343A
        cmp    byte ptr [ebp + 8], 2
        jne    SHORT loc_1_3435
        lea    esi, [ebp + 11h]
        mov    edi, ebx
        mov    ecx, eax
        repe cmpsb
        je     SHORT loc_1_346F
loc_1_3435:
        mov    ebp, dword ptr [ebp]
        jmp    SHORT loc_1_341C
loc_1_343A:
        pop    ebp
loc_1_343B:
        mov    eax, 60002h
        pop    esi
        stc
        ret
loc_1_3443:
        mov    eax, dword ptr [ebx + 34h]
        mov    ebx, dword ptr [ebx]
        db  081h, 0FBh, 000h, 01Ch, 001h, 000h
        je     SHORT loc_1_345A
        cmp    eax, dword ptr [ebx + 34h]
        je     SHORT loc_1_3484
        mov    eax, 20002h
loc_1_345A:
        pop    esi
        stc
        ret
        db  033h, 0DBh
loc_1_345F:
        pop    esi
        ret
loc_1_3461:
        mov    edx, ebx
        mov    ebx, 0ah
        mov    eax, 590002h
        jmp    SHORT loc_1_345A
loc_1_346F:
        mov    ebx, ebp
        pop    ecx
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_348D
        mov    esi, dword ptr [esp]
        test   byte ptr [esi + 1fh], 1
        jne    SHORT loc_1_348D
loc_1_3484:
        call   sub_1_360B
        jb     SHORT loc_1_345F
        jne    SHORT loc_1_3443
loc_1_348D:
        test   byte ptr [ebx + 10h], 40h
        jne    NEAR PTR loc_1_3520
        db  0A1h, 020h, 01Ch, 001h, 000h
        cmp    byte ptr [eax], 1
        jae    NEAR PTR loc_1_35D9
loc_1_34A5:
        mov    dx, word ptr [ebx + 0eh]
        cmp    word ptr [ebx + 0ch], dx
        jbe    SHORT loc_1_3461
loc_1_34AF:
        db  08Bh, 00Dh, 090h, 01Ah, 001h, 000h
        test   ecx, ecx
        je     SHORT loc_1_350F
        mov    eax, dword ptr [ecx]
        db  0A3h, 090h, 01Ah, 001h, 000h
        inc    word ptr [ebx + 0eh]
        mov    dword ptr [ecx + 0ch], ebx
        mov    byte ptr [ecx + 14h], 3
        mov    al, byte ptr [ecx + 17h]
        add    al, 8
        or     al, 0c0h
        mov    byte ptr [ecx + 17h], al
        mov    byte ptr [ecx + 15h], 0
        mov    esi, dword ptr [esp]
        test   byte ptr [esi + 1fh], 8
        jne    SHORT loc_1_34EA
        db  0A1h, 024h, 01Ch, 001h, 000h
        inc    dword ptr [eax + 10h]
loc_1_34EA:
        pop    esi
        cmp    dword ptr [ebx + 11h], 494d4441h
        jne    SHORT loc_1_3502
        cmp    word ptr [ebx + 15h], 244eh
        jne    SHORT loc_1_3502
        cmp    byte ptr [ebx + 17h], 0
        je     SHORT loc_1_3563
loc_1_3502:
        db  0B8h, 030h, 01Ch, 001h, 000h
        call   sub_1_8BC0
        test   ebx, ebx
        ret
loc_1_350F:
        call   sub_1_36A4
        jae    SHORT loc_1_34AF
        jmp    SHORT loc_1_3518
loc_1_3518:
        pop    edi
        mov    eax, 590002h
        stc
        ret
loc_1_3520:
        mov    esi, dword ptr [esp]
        test   byte ptr [esi + 1fh], 1
        jne    NEAR PTR loc_1_34A5
loc_1_352D:
        mov    edx, ebx
        mov    ebx, 0bh
        mov    eax, 50001h
        jmp    NEAR PTR loc_1_345A
        db  08Bh, 03Ch, 024h, 053h, 08Bh, 035h, 014h, 028h, 001h, 000h, 0E8h, 01Bh, 0E4h, 0FFh, 0FFh, 05Bh
        db  00Bh, 0EDh, 00Fh, 084h, 04Fh, 0FFh, 0FFh, 0FFh, 03Eh, 083h, 07Dh, 010h, 000h, 00Fh, 085h, 044h
        db  0FFh, 0FFh, 0FFh, 0EBh, 0CAh
loc_1_3563:
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        je     SHORT loc_1_357D
        test   byte ptr [esi + 1fh], 1
        jne    SHORT loc_1_3502
        dec    word ptr [ebx + 0eh]
        mov    byte ptr [ecx + 14h], 0
        push   esi
        jmp    SHORT loc_1_352D
loc_1_357D:
        or     byte ptr [esi + 1fh], 1
        mov    byte ptr [ecx + 15h], 1
        mov    edi, esi
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        db  083h, 03Dh, 03Ch, 034h, 001h, 000h, 000h
        jne    SHORT loc_1_35AA
        push   edx
        push   ecx
        lea    ecx, [ebx + 1eh]
        push   ecx
        call   sub_1_BA80
        pop    ecx
        pop    ecx
        pop    edx
        db  0FFh, 005h, 03Ch, 034h, 001h, 000h
loc_1_35AA:
        push   ebx
        call   sub_1_1968
        jb     SHORT loc_1_35C5
        db  08Bh, 03Dh, 028h, 01Ch, 001h, 000h
        mov    edi, dword ptr [edi + 0e2h]
        mov    dword ptr [ebx + 4], edi
        pop    ebx
        test   ebx, ebx
        ret
loc_1_35C5:
        pop    edx
        mov    byte ptr [ecx + 14h], 0
        mov    ebx, 0ch
        mov    eax, 10002h
        jmp    NEAR PTR loc_1_345A
loc_1_35D9:
        jne    SHORT loc_1_3603
        mov    esi, dword ptr [esp]
        test   byte ptr [esi + 1fh], 1
        jne    NEAR PTR loc_1_34A5
        cmp    dword ptr [ebx + 11h], 494d4441h
        jne    SHORT loc_1_3603
        cmp    word ptr [ebx + 15h], 244eh
        jne    SHORT loc_1_3603
        cmp    byte ptr [ebx + 17h], 0
        je     NEAR PTR loc_1_34A5
loc_1_3603:
        mov    eax, 510002h
        pop    edi
        stc
        ret
sub_1_360B:
        push   ebp
        mov    ebp, esp
        lea    edi, [ebx + 1eh]
        mov    eax, dword ptr [ebx + 34h]
        mov    esi, dword ptr [ebx]
        db  081h, 0FEh, 000h, 01Ch, 001h, 000h
        je     SHORT loc_1_3623
        cmp    eax, dword ptr [esi + 34h]
        je     SHORT loc_1_3633
loc_1_3623:
        mov    esi, dword ptr [ebx + 4]
        db  081h, 0FEh, 000h, 01Ch, 001h, 000h
        je     SHORT loc_1_3641
        cmp    eax, dword ptr [esi + 34h]
        jne    SHORT loc_1_3641
loc_1_3633:
        cmp    word ptr [edx], 20h
        jne    SHORT loc_1_3646
        mov    eax, 8460002h
        stc
        leave
        ret
loc_1_3641:
        cmp    byte ptr [edi], 0
        je     SHORT loc_1_366C
loc_1_3646:
        cmp    ecx, 18h
        je     SHORT loc_1_366F
        cmp    ecx, 9
        ja     SHORT loc_1_366C
        cmc
        sbb    ecx, 0
        push   ecx
        push   ebx
        db  08Bh, 01Dh, 050h, 037h, 001h, 000h
        mov    esi, edx
        sub    eax, eax
        dec    edi
loc_1_3661:
        inc    edi
        lodsb
        mov    al, byte ptr [eax + ebx]
        cmp    al, byte ptr [edi]
        loope  loc_1_3661
        pop    ebx
        pop    ecx
loc_1_366C:
        clc
        leave
        ret
loc_1_366F:
        sub    esp, 0e4h
        mov    eax, esp
        pushad
        push   edx
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        push   eax
        push   edi
        call   sub_1_6D28
        add    esp, 10h
        test   eax, eax
        popad
        jmp    SHORT loc_1_366C
loc_1_368F:
        db  0B8h, 057h, 021h, 001h, 000h
        call   sub_1_875C
        db  0F6h, 005h, 05Bh, 021h, 001h, 000h, 001h
        jne    SHORT loc_1_368F
        ret
        db  090h
sub_1_36A4:
        db  00Fh, 0BAh, 02Dh, 05Bh, 021h, 001h, 000h, 000h
        jb     SHORT loc_1_368F
        db  08Bh, 015h, 0FCh, 01Bh, 001h, 000h
        cmp    edx, 200h
        jae    SHORT loc_1_371D
        shr    edx, 3
        push   ecx
        push   edx
        push   20e0h
        call   sub_1_8CB6
        pop    edx
        pop    edx
        pop    ecx
        test   eax, eax
        je     SHORT loc_1_371D
        db  089h, 004h, 095h, 098h, 01Ah, 001h, 000h
        shl    edx, 3
        mov    ecx, 8
        db  0BFh, 090h, 01Ah, 001h, 000h
        sub    ebp, ebp
loc_1_36E8:
        mov    word ptr [eax + 16h], dx
        mov    dword ptr [eax + 4], ebp
        mov    byte ptr [eax + 14h], 0
        mov    dword ptr [eax + 8], ebp
        mov    dword ptr [edi], eax
        mov    edi, eax
        add    eax, 41ch
        inc    edx
        loop   loc_1_36E8
        mov    dword ptr [edi], ebp
        db  089h, 015h, 0FCh, 01Bh, 001h, 000h
        db  0C6h, 005h, 05Bh, 021h, 001h, 000h, 000h
        db  0B8h, 057h, 021h, 001h, 000h
        call   sub_1_8BC0
        clc
        ret
loc_1_371D:
        db  0C6h, 005h, 05Bh, 021h, 001h, 000h, 000h
        db  0B8h, 057h, 021h, 001h, 000h
        call   sub_1_8BC0
        stc
        ret
sub_1_3730:
        xchg   ebx, eax
        btr    dword ptr [ebx + 14h], 1
        jae    SHORT loc_1_3771
        or     byte ptr [ebx + 14h], 4
        mov    ecx, dword ptr [ebx + 0ch]
        mov    edx, dword ptr [ebx + 8]
        dec    word ptr [ecx + 0eh]
        dec    word ptr [edx + 42h]
        push   eax
        mov    edx, dword ptr [ebx + 8]
        add    edx, 50h
        nop
loc_1_3752:
        mov    ecx, edx
loc_1_3754:
        mov    ecx, dword ptr [ecx]
        cmp    ecx, edx
        je     SHORT loc_1_3770
        cmp    dword ptr [ecx + 10h], ebx
        jne    SHORT loc_1_3754
        push   ebx
        push   edx
        mov    eax, dword ptr [esp + 8]
        call   sub_1_1E8C
        pop    edx
        pop    ebx
        jmp    SHORT loc_1_3752
        db  08Bh, 0C0h
loc_1_3770:
        pop    eax
loc_1_3771:
        test   byte ptr [ebx + 15h], 1
        je     SHORT loc_1_379A
        mov    byte ptr [ebx + 15h], 0
        mov    edx, dword ptr [ebx + 8]
        and    byte ptr [edx + 1fh], 0feh
        db  08Bh, 03Dh, 028h, 01Ch, 001h, 000h
        mov    edi, dword ptr [edi + 0deh]
        mov    ecx, dword ptr [edx + 38h]
loc_1_3791:
        mov    dword ptr [ecx + 4], edi
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_1_3791
loc_1_379A:
        mov    dword ptr [ebx + 0ch], 0
        mov    ah, 4
        cmp    al, 3
        je     SHORT loc_1_37A9
        mov    ah, 0
loc_1_37A9:
        and    byte ptr [ebx + 14h], ah
        jne    SHORT loc_1_37BB
        db  0A1h, 090h, 01Ah, 001h, 000h
        mov    dword ptr [ebx], eax
        db  089h, 01Dh, 090h, 01Ah, 001h, 000h
loc_1_37BB:
        ret
sub_1_37BC:
        mov    esi, 8
        cmp    bl, 4
        je     SHORT loc_1_37CB
        mov    esi, 0ch
loc_1_37CB:
        db  0BAh, 094h, 01Ah, 001h, 000h
loc_1_37D0:
        add    edx, 4
        db  081h, 0FAh, 098h, 01Bh, 001h, 000h
        jae    SHORT loc_1_3810
        mov    eax, dword ptr [edx]
        test   eax, eax
        je     SHORT loc_1_37D0
        mov    ecx, 8
        sub    eax, 41ch
loc_1_37EB:
        add    eax, 41ch
        test   byte ptr [eax + 14h], 2
        loope  loc_1_37EB
        je     SHORT loc_1_37D0
        cmp    dword ptr [esi + eax], edi
        je     SHORT loc_1_3803
        test   ecx, ecx
        jne    SHORT loc_1_37EB
        jmp    SHORT loc_1_37D0
loc_1_3803:
        pushad
        call   sub_1_3730
        popad
        test   ecx, ecx
        jne    SHORT loc_1_37EB
        jmp    SHORT loc_1_37D0
loc_1_3810:
        ret
        db  000h, 000h, 000h
loc_1_3814:
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     NEAR PTR loc_1_0033
        mov    ebx, dword ptr [eax + 0ch]
        cmp    byte ptr [ebx + 9], 1
        mov    eax, 70002h
        jne    NEAR PTR loc_1_0033
        push   dword ptr [edx + 18h]
        mov    edx, dword ptr [esi + 24h]
        mov    ebp, 10012h
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_385C
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_385C
        test   byte ptr [ebx + 10h], 4
        je     NEAR PTR loc_1_394A
        mov    eax, eax
loc_1_385C:
        mov    eax, 10048h
        or     eax, 10000h
        call   sub_1_19E4
        jb     NEAR PTR loc_1_0033
        and    ebp, 0ff8fffffh
        or     ebp, 200000h
        call   sub_1_931B
        jb     NEAR PTR loc_1_393E
        push   eax
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        lea    edi, [ecx + 22h]
        mov    ecx, dword ptr [esi + 30h]
        mov    bx, word ptr [ecx + 1ch]
        shl    ebx, 10h
        mov    ecx, dword ptr [esi + 20h]
        mov    bx, word ptr [ecx + 1ah]
        mov    dword ptr [edi + 4], ebx
        xchg   ebp, eax
        db  08Bh, 00Dh, 028h, 028h, 001h, 000h
        mov    ebx, 6
        sub    edx, edx
        mov    esi, dword ptr [esi + 40h]
        call   sub_1_94D4
        pop    edi
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    byte ptr [esi + 9], 0
        mov    ecx, dword ptr [esi + 2ch]
        jb     SHORT loc_1_390E
        mov    dword ptr [ecx + 22h], eax
        mov    dword ptr [ecx + 26h], edi
        mov    dword ptr [ecx + 2ah], 0
        mov    ebx, dword ptr [esi + 1ch]
        mov    al, byte ptr [ebx + 0ah]
        mov    byte ptr [ecx + 34h], al
        call   sub_1_1ADC
        jb     NEAR PTR loc_1_0033
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 1
        mov    eax, dword ptr [ecx + 8]
        mov    word ptr [ebx + 1], ax
        mov    word ptr [ebx + 3], 0
        lea    eax, [ebx + 5]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
loc_1_390E:
        push   eax
        push   ecx
        push   edx
        mov    edx, edi
        sub    edx, 2
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        mov    edi, eax
        call   sub_1_9449
loc_1_392B:
        call   sub_1_1AB4
        mov    eax, edi
        sub    ah, ah
        shl    eax, 10h
        mov    al, 1
        jmp    NEAR PTR loc_1_0033
loc_1_393E:
        xchg   edi, eax
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jmp    SHORT loc_1_392B
loc_1_394A:
        pop    eax
        mov    eax, 40002h
        jmp    NEAR PTR loc_1_0033
        db  02Eh, 08Bh, 0C0h
loc_1_3958:
        mov    ebx, dword ptr [esi + 24h]
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 0
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        inc    byte ptr [ecx + 32h]
        mov    eax, dword ptr [ecx + 22h]
        push   edi
        mov    ebx, 0
        mov    ecx, 1
        call   sub_1_9818
        pop    edi
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     NEAR PTR loc_1_0014
        mov    ebx, dword ptr [esi + 24h]
        mov    edx, dword ptr [esi + 20h]
        mov    di, word ptr [edx + 1ah]
        movzx  edx, byte ptr [ebx]
        lea    edx, [ebx + edx*2 + 4]
        movzx  ebx, word ptr [edx]
        test   ebx, ebx
        je     SHORT loc_1_39CA
        lea    esi, [edx + 2]
        mov    edx, eax
        mov    eax, dword ptr [ecx + 22h]
        mov    ecx, ebx
        sub    ebp, ebp
        call   sub_1_97AB
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        jb     NEAR PTR loc_1_0014
loc_1_39CA:
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0
        mov    word ptr [ebx + 1], 0
        lea    eax, [ebx + 3]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0120
        db  02Eh, 08Bh, 0C0h
loc_1_39E4:
        mov    ebx, dword ptr [esi + 24h]
        movzx  ecx, word ptr [ebx + 1]
        mov    al, 1
        call   sub_1_18B8
        jb     NEAR PTR loc_1_0033
        and    byte ptr [ecx + 18h], 0fdh
        call   sub_1_1B8C
loc_1_3A01:
        cmp    byte ptr [ecx + 32h], 0
        jne    SHORT loc_1_3A48
        mov    eax, dword ptr [ecx + 22h]
        mov    ecx, 0ffffffffh
        sub    ebp, ebp
        call   sub_1_96A4
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 2ch]
        call   sub_1_948C
        jb     SHORT loc_1_3A52
        cmp    byte ptr [ecx + 33h], 0
        jne    SHORT loc_1_3A58
        call   sub_1_1AB4
        mov    ebx, dword ptr [esi + 28h]
        mov    byte ptr [ebx], 0
        mov    word ptr [ebx + 1], 0
        lea    eax, [ebx + 3]
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
loc_1_3A48:
        lea    eax, [ecx + 2ah]
        call   sub_1_875C
        jmp    SHORT loc_1_3A01
loc_1_3A52:
        cmp    byte ptr [ecx + 33h], 0
        je     SHORT loc_1_3A62
loc_1_3A58:
        mov    al, byte ptr [ecx + 33h]
        sub    ah, ah
        shl    eax, 10h
        mov    al, 1
loc_1_3A62:
        push   eax
        call   sub_1_1AB4
        pop    eax
        jmp    NEAR PTR loc_1_0033
        db  000h, 000h
loc_1_3A70:
        cmp    byte ptr [ebx + 1bh], 1
        jne    NEAR PTR loc_1_3C1E
        cmp    word ptr [ebx + 1dh], 0dh
        jbe    SHORT loc_1_3A92
        jmp    NEAR PTR loc_1_3C1E
        db  08Bh, 0C0h
loc_1_3A88:
        cmp    byte ptr [ebx + 1bh], 0
        jne    NEAR PTR loc_1_3C1E
loc_1_3A92:
        mov    ecx, dword ptr [edx + 18h]
        mov    ax, word ptr [edx + 1eh]
        db  08Bh, 035h, 038h, 01Ch, 001h, 000h
loc_1_3A9F:
        and    esi, esi
        je     SHORT loc_1_3ACD
        cmp    dword ptr [esi + 0ch], ecx
        jne    SHORT loc_1_3AAE
        cmp    word ptr [esi + 10h], ax
        je     SHORT loc_1_3AB2
loc_1_3AAE:
        mov    esi, dword ptr [esi]
        jmp    SHORT loc_1_3A9F
loc_1_3AB2:
        cmp    dword ptr [esi + 4], 0ee0001h
        jne    NEAR PTR loc_1_3C1E
        mov    eax, esi
        mov    dword ptr [esi + 4], 0
        jmp    NEAR PTR loc_1_3D21
loc_1_3ACD:
        mov    esi, edi
        push   ecx
        push   edx
        push   44h
        call   sub_1_8CB6
        pop    edx
        pop    edx
        pop    ecx
        test   eax, eax
        je     NEAR PTR loc_1_3C14
        mov    edi, eax
        mov    ecx, 11h
        sub    eax, eax
        rep stosd
        sub    edi, 44h
        cmp    al, byte ptr [ebx + 1bh]
        sbb    eax, eax
        mov    word ptr [edi + 12h], ax
        mov    ax, word ptr [ebx + 1dh]
        inc    ax
        and    word ptr [edi + 12h], ax
        mov    eax, dword ptr [edx + 18h]
        mov    dword ptr [edi + 0ch], eax
        mov    ax, word ptr [edx + 1eh]
        mov    word ptr [edi + 10h], ax
        mov    ax, word ptr [ebx + 13h]
        mov    word ptr [edi + 18h], ax
        mov    ax, word ptr [ebx + 17h]
        mov    word ptr [edi + 1ah], ax
        mov    eax, dword ptr [ebx + 5]
        mov    dword ptr [edi + 2ch], eax
        mov    eax, dword ptr [ebx + 1]
        mov    dword ptr [edi + 14h], eax
        mov    dword ptr [edi + 8], esi
        push   ebx
        add    ax, word ptr [edi + 16h]
        jb     SHORT loc_1_3B57
        movzx  eax, ax
        call   sub_1_7879
        jb     NEAR PTR loc_1_3C08
        mov    dword ptr [edi + 1ch], eax
        mov    dword ptr [edi + 24h], ecx
        movzx  ecx, word ptr [edi + 14h]
        add    ecx, eax
        mov    dword ptr [edi + 20h], ecx
        jmp    SHORT loc_1_3B84
loc_1_3B57:
        movzx  eax, word ptr [edi + 14h]
        call   sub_1_7879
        jb     NEAR PTR loc_1_3C08
        mov    dword ptr [edi + 1ch], eax
        mov    dword ptr [edi + 24h], ecx
        movzx  eax, word ptr [edi + 16h]
        call   sub_1_7879
        jb     NEAR PTR loc_1_3BFD
        mov    dword ptr [edi + 20h], eax
        mov    dword ptr [edi + 28h], ecx
        mov    eax, eax
loc_1_3B84:
        pop    ebx
        db  0A1h, 038h, 01Ch, 001h, 000h
        mov    dword ptr [edi], eax
        db  089h, 03Dh, 038h, 01Ch, 001h, 000h
        mov    eax, edi
        movzx  esi, word ptr [ebx + 15h]
        add    esi, edx
        mov    edi, dword ptr [eax + 1ch]
        movzx  ecx, word ptr [eax + 18h]
        mov    ebp, ecx
        shr    ecx, 2
        rep movsd
        mov    cl, 3
        and    ecx, ebp
        rep movsb
        movzx  esi, word ptr [ebx + 19h]
        add    esi, edx
        mov    edi, dword ptr [eax + 20h]
        movzx  ecx, word ptr [eax + 1ah]
        mov    ebp, ecx
        shr    ecx, 2
        rep movsd
        mov    cl, 3
        and    ecx, ebp
        rep movsb
        mov    edi, eax
        sub    eax, eax
        mov    ax, word ptr [edi + 16h]
        cmp    ax, word ptr [edi + 1ah]
        ja     SHORT loc_1_3BE4
        mov    ax, word ptr [edi + 14h]
        cmp    ax, word ptr [edi + 18h]
        jbe    NEAR PTR loc_1_3D1F
loc_1_3BE4:
        sub    eax, eax
        mov    byte ptr [ebx], al
        mov    word ptr [ebx + 1], ax
        lea    eax, [ebx + 3]
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    dword ptr [esi + 28h], eax
        jmp    NEAR PTR loc_1_0134
loc_1_3BFD:
        mov    eax, dword ptr [edi + 24h]
        call   sub_1_78A8
        mov    eax, eax
loc_1_3C08:
        pop    ebx
        push   ecx
        push   edx
        push   edi
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
loc_1_3C14:
        mov    eax, 80001h
        jmp    NEAR PTR loc_1_0033
loc_1_3C1E:
        mov    eax, 10002h
        jmp    NEAR PTR loc_1_0033
loc_1_3C28:
        mov    ecx, dword ptr [edx + 18h]
        mov    ax, word ptr [edx + 1eh]
        db  08Bh, 035h, 038h, 01Ch, 001h, 000h
loc_1_3C35:
        and    esi, esi
        je     SHORT loc_1_3C48
        cmp    dword ptr [esi + 0ch], ecx
        jne    SHORT loc_1_3C44
        cmp    word ptr [esi + 10h], ax
        je     SHORT loc_1_3C56
loc_1_3C44:
        mov    esi, dword ptr [esi]
        jmp    SHORT loc_1_3C35
loc_1_3C48:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 1ch]
        jmp    0
loc_1_3C56:
        mov    eax, esi
        cmp    dword ptr [eax + 4], 0ee0001h
        mov    dword ptr [eax + 4], 0
        je     NEAR PTR loc_1_3D21
        mov    ax, word ptr [ebx + 1]
        sub    ax, word ptr [esi + 14h]
        sbb    cx, cx
        and    ax, cx
        add    ax, word ptr [esi + 14h]
        mov    word ptr [ebx + 1], ax
        mov    ax, word ptr [ebx + 3]
        sub    ax, word ptr [esi + 16h]
        sbb    cx, cx
        and    ax, cx
        add    ax, word ptr [esi + 16h]
        mov    word ptr [ebx + 3], ax
        mov    eax, esi
        movzx  esi, word ptr [ebx + 9]
        mov    edi, dword ptr [eax + 1ch]
        add    edi, esi
        movzx  esi, word ptr [ebx + 7]
        add    esi, edx
        movzx  ecx, word ptr [ebx + 5]
        mov    ebp, ecx
        shr    ecx, 2
        rep movsd
        mov    cl, 3
        and    ecx, ebp
        rep movsb
        movzx  esi, word ptr [ebx + 0fh]
        mov    edi, dword ptr [eax + 20h]
        add    edi, esi
        movzx  esi, word ptr [ebx + 0dh]
        add    esi, edx
        movzx  ecx, word ptr [ebx + 0bh]
        mov    ebp, ecx
        shr    ecx, 2
        rep movsd
        mov    cl, 3
        and    ecx, ebp
        rep movsb
        mov    cx, word ptr [ebx + 0bh]
        add    word ptr [eax + 1ah], cx
        mov    cx, word ptr [eax + 1ah]
        cmp    cx, word ptr [eax + 16h]
        jb     SHORT loc_1_3D00
        mov    cx, word ptr [ebx + 5]
        add    word ptr [eax + 18h], cx
        mov    cx, word ptr [eax + 18h]
        cmp    cx, word ptr [eax + 14h]
        jae    SHORT loc_1_3D21
        mov    eax, eax
loc_1_3D00:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 1ch]
        movzx  eax, byte ptr [ecx + 7]
        db  0F6h, 080h, 0B8h, 01Ch, 001h, 000h, 002h
        jne    NEAR PTR loc_1_3BE4
        jmp    0
loc_1_3D1F:
        mov    eax, edi
loc_1_3D21:
        mov    byte ptr [ebx + 13h], 0
        lea    ecx, [ebx + 17h]
        mov    dword ptr [eax + 30h], ecx
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [eax + 8]
        movzx  ecx, word ptr [eax + 12h]
        push   eax
        push   edx
        db  0FFh, 014h, 08Dh, 03Ch, 01Ch, 001h, 000h
        pop    edx
        pop    ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edx + 5], eax
        mov    eax, ecx
        jb     NEAR PTR loc_1_3EE3
        cmp    dword ptr [eax + 4], 0ee0001h
        je     NEAR PTR loc_1_3EE3
        db  08Bh, 01Dh, 014h, 028h, 001h, 000h
        mov    ebx, dword ptr [ebx + 28h]
        mov    ecx, dword ptr [eax + 2ch]
        mov    dword ptr [ebx + 1], ecx
        sub    ecx, ecx
        mov    word ptr [ebx + 5], cx
        mov    word ptr [ebx + 0bh], cx
        mov    word ptr [ebx + 11h], cx
        movzx  ecx, byte ptr [ebx + 13h]
        add    ecx, 0ah
        mov    byte ptr [ebx], cl
        lea    esi, [ebx + ecx*2 + 3]
        mov    ecx, esi
        sub    ecx, edx
        mov    word ptr [ebx + 9], cx
        mov    ecx, dword ptr [eax + 8]
        movzx  ecx, word ptr [ecx + 48h]
        add    ecx, edx
        sub    ecx, esi
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    esi, dword ptr [esi + 1ch]
        db  08Bh, 02Dh, 0C4h, 031h, 001h, 000h
        sub    ebp, dword ptr [esi + 44h]
        db  08Bh, 035h, 01Ch, 01Ch, 001h, 000h
        add    dword ptr [esi + 10h], ebp
        inc    dword ptr [esi + 14h]
        sub    esp, 0ch
        mov    ebp, esp
        mov    dword ptr [ebp + 8], ecx
        mov    dword ptr [ebp + 4], edx
        mov    dword ptr [ebp], eax
        cmp    word ptr [eax + 2ch], cx
        ja     NEAR PTR loc_1_3F16
        movzx  esi, word ptr [ebx + 9]
        add    esi, edx
        cmp    esi, dword ptr [eax + 30h]
        jne    NEAR PTR loc_1_3F16
        mov    dx, word ptr [eax + 2ch]
        mov    word ptr [ebx + 7], dx
        add    dx, word ptr [ebx + 9]
        mov    di, dx
        and    edi, 1
        add    dx, di
        mov    word ptr [ebx + 0fh], dx
        mov    ecx, dword ptr [ebp + 8]
        sub    cx, word ptr [ebx + 7]
        sub    cx, di
        sub    cx, word ptr [eax + 2eh]
        sbb    dx, dx
        and    cx, dx
        add    cx, word ptr [eax + 2eh]
        cmp    cx, word ptr [eax + 2eh]
        sbb    edx, edx
        dec    edx
        and    ecx, edx
        mov    word ptr [ebx + 0dh], cx
        lea    edx, [edi + ecx]
        add    dx, word ptr [ebx + 7]
        mov    word ptr [ebx + 15h], dx
        mov    esi, dword ptr [eax + 34h]
        pushad
        mov    ax, word ptr [ebx + 9]
        add    ax, word ptr [ebx + 7]
        add    ax, di
        db  08Bh, 03Dh, 01Ch, 01Ch, 001h, 000h
        movzx  edx, dx
        add    edx, 23h
        add    dword ptr [edi], edx
        adc    dword ptr [edi + 4], 0
        call   sub_1_7568
        call   sub_1_8C44
        call   sub_1_766C
        call   sub_1_8C2C
        and    al, al
        popad
        jne    SHORT loc_1_3EE0
        movzx  ecx, word ptr [ebx + 0dh]
        add    esi, ecx
        add    word ptr [ebx + 11h], cx
        movzx  edx, word ptr [eax + 2eh]
        sub    edx, ecx
loc_1_3E73:
        and    edx, edx
        je     SHORT loc_1_3EE0
        mov    cx, word ptr [ebx + 9]
        mov    di, cx
        and    edi, 1
        add    cx, di
        mov    word ptr [ebx + 0fh], cx
        sub    ecx, ecx
        mov    dword ptr [ebx + 7], ecx
        mov    word ptr [ebx + 0bh], cx
loc_1_3E91:
        mov    ecx, dword ptr [ebp + 8]
        sub    ecx, edi
        sub    ecx, edx
        sbb    eax, eax
        and    ecx, eax
        add    ecx, edx
        cmp    ecx, edx
        sbb    eax, eax
        dec    eax
        and    ecx, eax
        mov    word ptr [ebx + 0dh], cx
        lea    eax, [edi + ecx]
        mov    word ptr [ebx + 15h], ax
        mov    ax, word ptr [ebx + 0fh]
        pushad
        db  08Bh, 03Dh, 01Ch, 01Ch, 001h, 000h
        mov    edx, eax
        add    edx, 23h
        add    dword ptr [edi], edx
        adc    dword ptr [edi + 4], 0
        call   sub_1_7568
        call   sub_1_766C
        and    al, al
        popad
        jne    SHORT loc_1_3EE0
        add    esi, ecx
        add    word ptr [ebx + 11h], cx
        sub    edx, ecx
        ja     SHORT loc_1_3E91
        clc
loc_1_3EE0:
        pop    eax
        pop    edx
        pop    ecx
loc_1_3EE3:
        pushfd
        cmp    dword ptr [eax + 4], 0ee0001h
        je     SHORT loc_1_3F10
        push   dword ptr [eax + 4]
        call   sub_1_3F88
        pop    eax
        popfd
        jb     SHORT loc_1_3F07
loc_1_3EF9:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    ecx, dword ptr [esi + 1ch]
        jmp    0
loc_1_3F07:
        and    eax, eax
        je     SHORT loc_1_3EF9
        jmp    NEAR PTR loc_1_0033
loc_1_3F10:
        popfd
        jmp    NEAR PTR loc_1_83C8
loc_1_3F16:
        sub    ecx, ecx
        mov    word ptr [ebx + 0fh], cx
        mov    word ptr [ebx + 0dh], cx
        movzx  edx, word ptr [eax + 2ch]
        mov    esi, dword ptr [eax + 30h]
loc_1_3F27:
        mov    ecx, edx
        sub    ecx, dword ptr [ebp + 8]
        sbb    eax, eax
        and    ecx, eax
        add    ecx, dword ptr [ebp + 8]
        mov    word ptr [ebx + 7], cx
        mov    word ptr [ebx + 15h], cx
        mov    ax, word ptr [ebx + 9]
        pushad
        db  08Bh, 03Dh, 01Ch, 01Ch, 001h, 000h
        movzx  edx, cx
        add    edx, 23h
        add    dword ptr [edi], edx
        adc    dword ptr [edi + 4], 0
        call   sub_1_7568
        call   sub_1_766C
        and    al, al
        popad
        jne    NEAR PTR loc_1_3EE0
        mov    edi, dword ptr [ebp + 4]
        mov    byte ptr [edi + 4], 33h
        add    esi, ecx
        add    word ptr [ebx + 0bh], cx
        sub    edx, ecx
        ja     SHORT loc_1_3F27
        mov    eax, dword ptr [ebp]
        mov    esi, dword ptr [eax + 34h]
        movzx  edx, word ptr [eax + 2eh]
        jmp    NEAR PTR loc_1_3E73
        db  02Eh, 08Bh, 0C0h
sub_1_3F88:
        mov    esi, eax
        mov    eax, dword ptr [esi + 24h]
        and    eax, eax
        je     SHORT loc_1_3F96
        call   sub_1_78A8
loc_1_3F96:
        mov    eax, dword ptr [esi + 28h]
        and    eax, eax
        je     SHORT loc_1_3FA2
        call   sub_1_78A8
loc_1_3FA2:
        mov    eax, dword ptr [esi + 38h]
        and    eax, eax
        je     SHORT loc_1_3FAE
        call   sub_1_78A8
loc_1_3FAE:
        mov    eax, dword ptr [esi + 3ch]
        and    eax, eax
        je     SHORT loc_1_3FBA
        call   sub_1_78A8
loc_1_3FBA:
        db  0A1h, 038h, 01Ch, 001h, 000h
        cmp    eax, esi
        jne    SHORT loc_1_3FCC
        mov    eax, dword ptr [esi]
        db  0A3h, 038h, 01Ch, 001h, 000h
        jmp    SHORT loc_1_3FE0
loc_1_3FCC:
        and    eax, eax
        je     SHORT loc_1_3FD8
        cmp    esi, dword ptr [eax]
        je     SHORT loc_1_3FDA
        mov    eax, dword ptr [eax]
        jmp    SHORT loc_1_3FCC
loc_1_3FD8:
        jmp    SHORT loc_1_3FE0
loc_1_3FDA:
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    eax, eax
loc_1_3FE0:
        push   dword ptr [esi]
        push   ecx
        push   edx
        push   esi
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    eax
        ret
        db  08Bh, 035h, 014h, 028h, 001h, 000h, 08Bh, 056h, 020h, 08Bh, 04Ah, 018h, 066h, 08Bh, 042h, 01Eh
        db  08Bh, 03Dh, 038h, 01Ch, 001h, 000h, 023h, 0C0h, 074h, 063h, 039h, 04Fh, 00Ch, 075h, 006h, 066h
        db  039h, 047h, 010h, 074h, 004h, 08Bh, 03Fh, 0EBh, 0EDh, 08Dh, 042h, 037h, 089h, 047h, 030h, 08Bh
        db  0DFh, 00Fh, 0B7h, 04Fh, 02Ch, 083h, 0C1h, 017h, 08Dh, 07Ah, 020h, 02Bh, 0C0h, 0F3h, 0AAh, 0B8h
        db  001h, 000h, 020h, 000h, 089h, 042h, 005h, 0C6h, 042h, 004h, 032h, 08Dh, 042h, 020h, 0C6h, 000h
        db  00Ah, 00Fh, 0B7h, 04Bh, 02Ch, 066h, 089h, 048h, 001h, 066h, 089h, 048h, 007h, 066h, 089h, 048h
        db  015h, 08Dh, 04Ch, 001h, 017h, 089h, 04Eh, 028h, 08Bh, 04Bh, 030h, 02Bh, 0CAh, 066h, 089h, 048h
        db  009h, 08Bh, 0C3h, 0E8h, 021h, 0FFh, 0FFh, 0FFh, 0E9h, 0C8h, 0C0h, 0FFh, 0FFh, 08Bh, 04Eh, 01Ch
        db  0E9h, 08Ch, 0BFh, 0FFh, 0FFh
loc_1_4074:
        mov    eax, 320001h
        stc
        ret
        db  000h
loc_1_407C:
        mov    ecx, eax
        mov    ax, word ptr [edx + 18h]
        call   sub_1_196C
        jb     SHORT loc_1_40DF
        push   ebx
        push   edi
        push   ecx
        movzx  eax, word ptr [ecx + 2eh]
        mov    edx, ecx
        test   eax, eax
        je     SHORT loc_1_40AD
loc_1_4096:
        call   sub_1_7879
        jae    SHORT loc_1_40A7
        test   eax, eax
        je     SHORT loc_1_40E0
        mov    word ptr [edx + 2eh], ax
        jmp    SHORT loc_1_4096
loc_1_40A7:
        mov    dword ptr [edx + 34h], eax
        mov    dword ptr [edx + 3ch], ecx
loc_1_40AD:
        xor    ecx, ecx
        mov    eax, dword ptr [edx + 1ch]
        mov    ax, word ptr [eax]
        db  08Bh, 00Dh, 0E8h, 025h, 001h, 000h
        db  0BFh, 0F5h, 024h, 001h, 000h
        test   edi, edi
        repne scasw
        jne    SHORT loc_1_40D4
        db  081h, 0EFh, 0F7h, 024h, 001h, 000h
        db  08Bh, 00Ch, 07Dh, 048h, 025h, 001h, 000h
loc_1_40D4:
        push   ecx
        call   sub_1_40EC
        add    esp, 10h
        xor    eax, eax
loc_1_40DF:
        ret
loc_1_40E0:
        add    esp, 0ch
        mov    eax, 80001h
        stc
        ret
        db  000h, 000h
sub_1_40EC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0a8h
        push   ebx
        push   esi
        push   edi
        lea    edx, [ebp - 7ch]
        mov    dword ptr [ebp - 14h], edx
        mov    byte ptr [ebp - 1], 0
        cmp    dword ptr [ebp + 8], 0
        mov    word ptr [ebp - 6], 0
        mov    ecx, dword ptr [ebp + 0ch]
        jne    SHORT loc_1_4134
        mov    eax, dword ptr [ecx + 1ch]
        cmp    word ptr [eax], 3fh
        mov    edx, 41h
        je     SHORT loc_1_4124
        mov    edx, 32h
loc_1_4124:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 30h]
        mov    word ptr [eax], dx
        jmp    NEAR PTR loc_1_4847
        db  08Bh, 0FFh
loc_1_4134:
        cmp    word ptr [ecx + 14h], 168h
        jbe    SHORT loc_1_414C
        mov    eax, dword ptr [ecx + 30h]
        mov    word ptr [eax], 57h
        jmp    NEAR PTR loc_1_4847
        db  08Dh, 049h, 000h
loc_1_414C:
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 10h], eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 1ch]
        add    ecx, 2
        mov    dword ptr [ebp - 18h], ecx
        mov    edi, ecx
loc_1_4166:
        mov    eax, dword ptr [ebp - 18h]
        inc    dword ptr [ebp - 18h]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_4166
        inc    ecx
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 58h], ecx
loc_1_4178:
        mov    eax, dword ptr [ebp - 18h]
        inc    dword ptr [ebp - 18h]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_4178
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 30h]
        add    eax, 4
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 4]
        push   edi
        call   sub_1_4938
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_41B0
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 30h]
        mov    word ptr [ecx], ax
        jmp    NEAR PTR loc_1_4847
loc_1_41B0:
        mov    dword ptr [ebp - 20h], edi
        cmp    byte ptr [edi], 0
        mov    ebx, dword ptr [ebp - 5ch]
        je     NEAR PTR loc_1_4484
        nop
loc_1_41C0:
        mov    eax, dword ptr [ebp - 20h]
        movsx  eax, byte ptr [eax]
        sub    eax, 44h
        cmp    eax, 36h
        ja     NEAR PTR loc_1_483C
        db  0FFh, 024h, 085h, 05Ch, 048h, 000h, 000h
        db  08Dh, 049h, 000h
loc_1_41DC:
        lea    eax, [ebp - 0a8h]
        sub    dword ptr [ebp - 14h], 4
        cmp    dword ptr [ebp - 14h], eax
        jb     SHORT loc_1_41F5
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [ebp - 14h]
        mov    dword ptr [edx], eax
loc_1_41F5:
        add    dword ptr [ebp - 18h], 4
        jmp    NEAR PTR loc_1_4442
        db  08Bh, 0FFh
loc_1_4200:
        lea    eax, [ebp - 20h]
        push   eax
        push   dword ptr [ebp - 20h]
        call   sub_1_4A1C
        add    esp, 8
        cmp    eax, -2
        je     NEAR PTR loc_1_483C
        add    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_4442
loc_1_4220:
        mov    ecx, dword ptr [ebp - 18h]
        mov    ax, word ptr [ecx]
        mov    word ptr [ebp - 6], ax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx + 2eh]
        cmp    word ptr [ebp - 6], ax
        jbe    SHORT loc_1_423F
        mov    word ptr [ebp - 6], ax
        or     byte ptr [ebp - 1], 10h
loc_1_423F:
        lea    eax, [ebp - 0a8h]
        sub    dword ptr [ebp - 14h], 2
        cmp    dword ptr [ebp - 14h], eax
        jb     SHORT loc_1_4258
        mov    ax, word ptr [ebp - 6]
        mov    edx, dword ptr [ebp - 14h]
        mov    word ptr [edx], ax
loc_1_4258:
        add    dword ptr [ebp - 18h], 2
        jmp    NEAR PTR loc_1_4442
        db  08Dh, 049h, 000h
loc_1_4264:
        test   esi, 80000000h
        je     NEAR PTR loc_1_4454
        add    esi, esi
        sub    dword ptr [ebp - 14h], 4
        lea    eax, [ebp - 0a8h]
        cmp    dword ptr [ebp - 14h], eax
        jb     NEAR PTR loc_1_4442
        mov    edx, dword ptr [ebp - 14h]
        mov    dword ptr [edx], 0
        jmp    NEAR PTR loc_1_4442
        db  090h
loc_1_4294:
        mov    ecx, dword ptr [ebp - 18h]
        mov    cx, word ptr [ecx]
        or     cx, cx
        je     SHORT loc_1_42A3
        or     byte ptr [ebp - 1], 8
loc_1_42A3:
        lea    eax, [ebp - 0a8h]
        sub    dword ptr [ebp - 14h], 2
        cmp    dword ptr [ebp - 14h], eax
        jb     SHORT loc_1_42B8
        mov    edx, dword ptr [ebp - 14h]
        mov    word ptr [edx], cx
loc_1_42B8:
        add    dword ptr [ebp - 18h], 2
        jmp    NEAR PTR loc_1_4442
        db  08Dh, 049h, 000h
loc_1_42C4:
        lea    eax, [ebp - 0a8h]
        sub    dword ptr [ebp - 14h], 2
        cmp    dword ptr [ebp - 14h], eax
        jb     NEAR PTR loc_1_4442
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx + 16h]
        mov    edx, dword ptr [ebp - 14h]
        mov    word ptr [edx], ax
        jmp    NEAR PTR loc_1_4442
        db  08Dh, 049h, 000h
loc_1_42EC:
        lea    eax, [ebp - 0a8h]
        sub    dword ptr [ebp - 14h], 2
        cmp    dword ptr [ebp - 14h], eax
        jb     SHORT loc_1_4307
        mov    ecx, dword ptr [ebp - 18h]
        mov    ax, word ptr [ecx]
        mov    edx, dword ptr [ebp - 14h]
        mov    word ptr [edx], ax
loc_1_4307:
        add    dword ptr [ebp - 18h], 2
        jmp    NEAR PTR loc_1_4442
loc_1_4310:
        add    esi, esi
        sub    dword ptr [ebp - 14h], 4
        lea    eax, [ebp - 0a8h]
        cmp    dword ptr [ebp - 14h], eax
        jb     SHORT loc_1_4329
        mov    ecx, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 14h]
        mov    dword ptr [edx], ecx
loc_1_4329:
        lea    eax, [ebp - 20h]
        push   eax
        push   dword ptr [ebp - 20h]
        call   sub_1_4A1C
        add    esp, 8
        cmp    eax, -2
        je     NEAR PTR loc_1_483C
        add    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_4442
        db  08Dh, 049h, 000h
loc_1_434C:
        add    esi, esi
        or     byte ptr [ebp - 1], 1
        mov    eax, dword ptr [ebp - 28h]
        mov    dword ptr [ebp - 44h], eax
        sub    dword ptr [ebp - 14h], 4
        lea    eax, [ebp - 0a8h]
        cmp    dword ptr [ebp - 14h], eax
        jb     SHORT loc_1_436F
        mov    eax, dword ptr [ebp - 28h]
        mov    edx, dword ptr [ebp - 14h]
        mov    dword ptr [edx], eax
loc_1_436F:
        add    dword ptr [ebp - 28h], 2
        jmp    NEAR PTR loc_1_4442
loc_1_4378:
        add    esi, esi
        sub    dword ptr [ebp - 14h], 4
        lea    eax, [ebp - 0a8h]
        cmp    dword ptr [ebp - 14h], eax
        jb     SHORT loc_1_4391
        mov    eax, dword ptr [ebp - 28h]
        mov    edx, dword ptr [ebp - 14h]
        mov    dword ptr [edx], eax
loc_1_4391:
        lea    eax, [ebp - 20h]
        push   eax
        push   dword ptr [ebp - 20h]
        call   sub_1_4A1C
        add    esp, 8
        cmp    eax, -2
        je     NEAR PTR loc_1_483C
        add    dword ptr [ebp - 28h], eax
        jmp    NEAR PTR loc_1_4442
        db  08Dh, 049h, 000h
loc_1_43B4:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 34h]
        or     eax, eax
        jne    SHORT loc_1_43CB
        mov    ecx, dword ptr [ebp - 18h]
        cmp    word ptr [ecx], 0
        jne    NEAR PTR loc_1_4464
loc_1_43CB:
        mov    dword ptr [ebp - 2ch], eax
        or     byte ptr [ebp - 1], 2
        sub    dword ptr [ebp - 14h], 4
        lea    eax, [ebp - 0a8h]
        cmp    dword ptr [ebp - 14h], eax
        jb     SHORT loc_1_4442
        mov    ecx, dword ptr [ebp - 2ch]
        mov    edx, dword ptr [ebp - 14h]
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_1_4442
        db  090h
loc_1_43EC:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ecx + 20h]
        or     ebx, ebx
        jne    SHORT loc_1_43FD
        cmp    word ptr [ecx + 16h], 0
        jne    SHORT loc_1_4474
loc_1_43FD:
        lea    eax, [ebp - 0a8h]
        or     byte ptr [ebp - 1], 4
        sub    dword ptr [ebp - 14h], 4
        cmp    dword ptr [ebp - 14h], eax
        jb     SHORT loc_1_4442
        mov    edx, dword ptr [ebp - 14h]
        mov    dword ptr [edx], ebx
        jmp    SHORT loc_1_4442
        db  090h
loc_1_4418:
        add    esi, esi
        sub    dword ptr [ebp - 14h], 4
        lea    eax, [ebp - 0a8h]
        cmp    dword ptr [ebp - 14h], eax
        jb     SHORT loc_1_4431
        mov    ecx, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 14h]
        mov    dword ptr [edx], ecx
loc_1_4431:
        sub    eax, eax
        mov    edi, dword ptr [ebp - 18h]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        add    dword ptr [ebp - 18h], ecx
loc_1_4442:
        inc    dword ptr [ebp - 20h]
        mov    eax, dword ptr [ebp - 20h]
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_1_41C0
        jmp    SHORT loc_1_4484
        db  090h
loc_1_4454:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 30h]
        mov    word ptr [eax], 57h
        jmp    NEAR PTR loc_1_4847
loc_1_4464:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 30h]
        mov    word ptr [eax], 57h
        jmp    NEAR PTR loc_1_4847
loc_1_4474:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 30h]
        mov    word ptr [eax], 57h
        jmp    NEAR PTR loc_1_4847
loc_1_4484:
        lea    eax, [ebp - 0a8h]
        cmp    dword ptr [ebp - 14h], eax
        jb     NEAR PTR loc_1_483C
        mov    edx, dword ptr [ebp + 10h]
        lea    eax, [edx + 4]
        mov    dword ptr [ebp - 7ch], eax
        mov    ecx, dword ptr [ebp + 14h]
        lea    eax, [ecx + 8]
        mov    dword ptr [ebp - 78h], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebp - 74h], eax
        mov    al, byte ptr [edx + 3eh]
        mov    byte ptr [ebp - 6ch], al
        mov    al, byte ptr [edx + 1fh]
        mov    byte ptr [ebp - 6bh], al
        mov    dword ptr [ebp - 70h], edx
        movsx  eax, byte ptr [edx + 3fh]
        db  08Bh, 004h, 085h, 0B0h, 01Bh, 001h, 000h
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 6ah], al
        add    ecx, 24h
        mov    dword ptr [ebp - 68h], ecx
        lea    eax, [ebp - 50h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        push   dword ptr [ebp - 58h]
        call   sub_1_49A4
        add    esp, 0ch
        mov    dword ptr [ebp - 30h], eax
        cmp    eax, -2
        je     NEAR PTR loc_1_483C
        cmp    dword ptr [ebp - 34h], -1
        je     SHORT loc_1_451A
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 48h], eax
        lea    eax, [ebp - 54h]
        push   eax
        lea    eax, [ebp - 60h]
        push   eax
        push   dword ptr [ebp - 18h]
        call   sub_1_49A4
        add    esp, 0ch
        mov    dword ptr [ebp - 3ch], eax
        cmp    eax, -2
        je     NEAR PTR loc_1_483C
loc_1_451A:
        test   byte ptr [ebp - 1], 4
        je     NEAR PTR loc_1_4681
        test   byte ptr [ebp - 1], 8
        jne    NEAR PTR loc_1_4681
        cmp    dword ptr [ebp - 34h], -1
        jne    SHORT loc_1_4544
        mov    word ptr [ebp - 8], 0
        mov    dword ptr [ebp - 40h], 1
        jmp    SHORT loc_1_4556
        db  090h
loc_1_4544:
        mov    eax, dword ptr [ebp - 34h]
        mov    ax, word ptr [eax + ebx]
        mov    word ptr [ebp - 8], ax
        mov    dword ptr [ebp - 40h], 2
loc_1_4556:
        movzx  eax, word ptr [ebp - 8]
        imul   eax, dword ptr [ebp - 3ch]
        add    eax, dword ptr [ebp - 30h]
        lea    esi, [eax + ebx]
        mov    eax, dword ptr [ebp - 58h]
        mov    dword ptr [ebp - 24h], eax
        cmp    dword ptr [ebp - 40h], 0
        mov    word ptr [ebp - 4], 1
        mov    dword ptr [ebp - 38h], 0
        jle    NEAR PTR loc_1_4681
loc_1_4581:
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 1ch], 0
        mov    dword ptr [ebp - 0ch], eax
        cmp    word ptr [ebp - 4], 0
        je     NEAR PTR loc_1_4664
loc_1_4599:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        or     al, al
        je     NEAR PTR loc_1_464E
loc_1_45A6:
        cmp    al, 5ah
        jle    SHORT loc_1_4624
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_45D0
        test   byte ptr [ebp - 0dh], 80h
        je     NEAR PTR loc_1_46EC
        add    ebx, 4
        lea    eax, [ebp - 0ch]
        push   eax
        push   dword ptr [ebp - 0ch]
        call   sub_1_4A1C
        add    esp, 8
        jmp    SHORT loc_1_4617
        db  08Dh, 049h, 000h
loc_1_45D0:
        mov    dword ptr [ebx], esi
        add    ebx, 4
        movsx  eax, al
        cmp    eax, 6ch
        je     SHORT loc_1_4600
        cmp    eax, 7ah
        je     SHORT loc_1_4608
        lea    eax, [ebp - 0ch]
        push   eax
        push   dword ptr [ebp - 0ch]
        call   sub_1_4A1C
        add    esp, 8
        cmp    eax, -2
        je     NEAR PTR loc_1_483C
        add    esi, eax
        jmp    SHORT loc_1_4617
        db  08Bh, 0FFh
loc_1_4600:
        movzx  eax, word ptr [esi]
        add    esi, eax
        jmp    SHORT loc_1_4617
        db  090h
loc_1_4608:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    esi, ecx
loc_1_4617:
        mov    eax, dword ptr [ebp - 10h]
        add    eax, eax
        mov    dword ptr [ebp - 10h], eax
        jmp    SHORT loc_1_463E
        db  08Dh, 049h, 000h
loc_1_4624:
        lea    eax, [ebp - 0ch]
        push   eax
        push   dword ptr [ebp - 0ch]
        call   sub_1_4ADC
        add    esp, 8
        cmp    eax, -2
        je     NEAR PTR loc_1_483C
        add    ebx, eax
loc_1_463E:
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        or     al, al
        jne    NEAR PTR loc_1_45A6
loc_1_464E:
        inc    dword ptr [ebp - 1ch]
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 0ch], eax
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 1ch]
        jg     NEAR PTR loc_1_4599
loc_1_4664:
        mov    eax, dword ptr [ebp - 48h]
        inc    dword ptr [ebp - 38h]
        mov    dword ptr [ebp - 24h], eax
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 38h]
        cmp    eax, dword ptr [ebp - 40h]
        jl     NEAR PTR loc_1_4581
loc_1_4681:
        push   dword ptr [ebp - 14h]
        lea    eax, [ebp - 7ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax]
        add    esp, 8
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 30h]
        mov    word ptr [ecx], ax
        or     eax, eax
        je     SHORT loc_1_46B0
        cmp    eax, 0eah
        jne    SHORT loc_1_46FC
        test   byte ptr [ebp - 1], 10h
        je     SHORT loc_1_46B0
loc_1_46AA:
        mov    word ptr [ebp - 6], 0
loc_1_46B0:
        mov    ecx, dword ptr [ebp + 0ch]
        test   byte ptr [ebp - 1], 2
        mov    eax, dword ptr [ecx + 30h]
        mov    word ptr [eax + 2], 0
        je     NEAR PTR loc_1_482A
        cmp    word ptr [ebp - 6], 0
        je     NEAR PTR loc_1_482A
        cmp    dword ptr [ebp - 30h], 0
        je     NEAR PTR loc_1_482A
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_1_4720
        mov    eax, dword ptr [ebp - 44h]
        mov    ax, word ptr [eax]
        jmp    SHORT loc_1_4724
        db  08Dh, 049h, 000h
loc_1_46EC:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 30h]
        mov    word ptr [eax], 57h
        jmp    NEAR PTR loc_1_4847
loc_1_46FC:
        cmp    eax, 84bh
        jne    SHORT loc_1_4708
        jmp    SHORT loc_1_46AA
        db  08Dh, 049h, 000h
loc_1_4708:
        cmp    eax, 90dh
        je     SHORT loc_1_46B0
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 2eh], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_4720:
        mov    ax, 1
loc_1_4724:
        or     ax, ax
        jne    SHORT loc_1_4734
        mov    word ptr [ebp - 6], 0
        jmp    NEAR PTR loc_1_482A
loc_1_4734:
        mov    edx, dword ptr [ebp - 2ch]
        cmp    dword ptr [ebp - 34h], -1
        je     SHORT loc_1_4768
        or     ax, ax
        je     SHORT loc_1_4780
        movzx  esi, ax
loc_1_4745:
        mov    ecx, edx
        mov    eax, dword ptr [ebp - 34h]
        mov    ax, word ptr [eax + edx]
        mov    word ptr [ebp - 8], ax
        and    eax, 0ffffh
        imul   eax, dword ptr [ebp - 3ch]
        add    eax, dword ptr [ebp - 30h]
        add    edx, eax
        dec    esi
        jne    SHORT loc_1_4745
        jmp    SHORT loc_1_4783
        db  08Dh, 049h, 000h
loc_1_4768:
        and    eax, 0ffffh
        imul   eax, dword ptr [ebp - 30h]
        mov    edx, dword ptr [ebp - 2ch]
        add    edx, eax
        mov    ecx, edx
        sub    ecx, dword ptr [ebp - 30h]
        jmp    SHORT loc_1_4783
        db  08Dh, 049h, 000h
loc_1_4780:
        mov    ecx, dword ptr [ebp - 64h]
loc_1_4783:
        cmp    dword ptr [ebp - 34h], -1
        je     SHORT loc_1_47B0
        cmp    dword ptr [ebp - 54h], -1
        je     SHORT loc_1_47B0
        mov    eax, dword ptr [ebp - 34h]
        cmp    word ptr [eax + ecx], 0
        je     SHORT loc_1_47B0
        movzx  esi, word ptr [ebp - 8]
        dec    esi
        imul   esi, dword ptr [ebp - 3ch]
        add    esi, ecx
        add    esi, dword ptr [ebp - 30h]
        mov    eax, dword ptr [ebp - 54h]
        mov    ebx, dword ptr [esi + eax]
        jmp    SHORT loc_1_47C9
        db  090h
loc_1_47B0:
        cmp    dword ptr [ebp - 50h], -1
        je     SHORT loc_1_47C0
        mov    eax, dword ptr [ebp - 50h]
        mov    ebx, dword ptr [eax + ecx]
        jmp    SHORT loc_1_47C9
        db  08Bh, 0FFh
loc_1_47C0:
        movzx  eax, word ptr [ebp - 6]
        mov    ebx, dword ptr [ebp - 2ch]
        add    ebx, eax
loc_1_47C9:
        or     ebx, ebx
        je     SHORT loc_1_481C
        cmp    ebx, edx
        je     SHORT loc_1_481C
        jb     SHORT loc_1_483C
        movzx  ecx, word ptr [ebp - 6]
        mov    eax, dword ptr [ebp - 2ch]
        add    eax, ecx
        cmp    eax, ebx
        jb     SHORT loc_1_483C
        mov    eax, dword ptr [ebp - 2ch]
        sub    eax, ebx
        add    ax, cx
        movzx  ecx, ax
        mov    dword ptr [ebp - 4ch], ecx
        mov    edi, edx
        mov    esi, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [ebp - 4ch]
        and    ecx, 3
        rep movsb
        mov    ecx, edx
        sub    ecx, dword ptr [ebp - 2ch]
        add    ax, cx
        mov    word ptr [ebp - 6], ax
        sub    ebx, edx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 30h]
        add    ebx, dword ptr [ebp - 2ch]
        mov    word ptr [eax + 2], bx
        jmp    SHORT loc_1_482A
loc_1_481C:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 30h]
        mov    cx, word ptr [ebp - 2ch]
        mov    word ptr [eax + 2], cx
loc_1_482A:
        pop    edi
        mov    ax, word ptr [ebp - 6]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 2eh], ax
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_483C:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 30h]
        mov    word ptr [eax], 85ch
loc_1_4847:
        pop    edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 2ch], 2
        mov    word ptr [ecx + 2eh], 0
        pop    esi
        pop    ebx
        leave
        ret
        db  090h, 0DCh, 041h, 000h, 000h, 03Ch, 048h, 000h, 000h, 000h, 042h, 000h, 000h, 03Ch, 048h, 000h
        db  000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h
        db  000h, 020h, 042h, 000h, 000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h, 000h, 064h, 042h, 000h
        db  000h, 094h, 042h, 000h, 000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h
        db  000h, 0C4h, 042h, 000h, 000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h, 000h, 0ECh, 042h, 000h
        db  000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h
        db  000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h
        db  000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h, 000h, 010h, 043h, 000h, 000h, 03Ch, 048h, 000h
        db  000h, 010h, 043h, 000h, 000h, 04Ch, 043h, 000h, 000h, 03Ch, 048h, 000h, 000h, 078h, 043h, 000h
        db  000h, 078h, 043h, 000h, 000h, 078h, 043h, 000h, 000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h
        db  000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h
        db  000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h, 000h, 0B4h, 043h, 000h, 000h, 0ECh, 043h, 000h
        db  000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h, 000h, 010h, 043h, 000h
        db  000h, 03Ch, 048h, 000h, 000h, 03Ch, 048h, 000h, 000h, 018h, 044h, 000h, 000h
sub_1_4938:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ecx, dword ptr [ebp + 0ch]
        or     ecx, ecx
        jne    SHORT loc_1_494C
        mov    eax, 85eh
        pop    ebx
        leave
        ret
        db  090h
loc_1_494C:
        inc    eax
        mov    eax, dword ptr [ebp + 8]
loc_1_4950:
        mov    dl, byte ptr [eax]
        inc    eax
        mov    bl, byte ptr [ecx]
        inc    ecx
        cmp    dl, 4fh
        jne    SHORT loc_1_4988
        cmp    bl, 64h
        je     SHORT loc_1_498C
        cmp    bl, 77h
        je     SHORT loc_1_498C
        cmp    bl, 62h
        je     SHORT loc_1_498C
        cmp    bl, 68h
        je     SHORT loc_1_498C
        cmp    bl, 67h
        je     SHORT loc_1_498C
        cmp    bl, 69h
        je     SHORT loc_1_498C
        cmp    bl, 7ah
        je     SHORT loc_1_498C
        mov    eax, 85ch
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_4988:
        cmp    dl, bl
        jne    SHORT loc_1_499C
loc_1_498C:
        or     dl, dl
        je     SHORT loc_1_4994
        or     bl, bl
        jne    SHORT loc_1_4950
loc_1_4994:
        sub    eax, eax
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_499C:
        pop    ebx
        mov    eax, 85ch
        leave
        ret
sub_1_49A4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [edi], 0ffffffffh
        mov    ebx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx], 0ffffffffh
        sub    si, si
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        or     al, al
        je     SHORT loc_1_4A07
loc_1_49C8:
        cmp    al, 4eh
        jne    SHORT loc_1_49D8
        movzx  eax, si
        mov    dword ptr [edi], eax
        add    si, 2
        jmp    SHORT loc_1_49FB
        db  090h
loc_1_49D8:
        cmp    al, 5ah
        jle    SHORT loc_1_49E1
        movzx  eax, si
        mov    dword ptr [ebx], eax
loc_1_49E1:
        lea    eax, [ebp + 8]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_4ADC
        add    esp, 8
        movzx  ecx, ax
        cmp    ecx, -2
        je     SHORT loc_1_4A10
        add    si, ax
loc_1_49FB:
        inc    dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        or     al, al
        jne    SHORT loc_1_49C8
loc_1_4A07:
        movzx  eax, si
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_4A10:
        pop    edi
        and    eax, 0ffffh
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
sub_1_4A1C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        movsx  eax, byte ptr [edx]
        cmp    eax, 67h
        jg     SHORT loc_1_4A68
        je     SHORT loc_1_4A8C
        cmp    eax, 57h
        jg     SHORT loc_1_4A50
        je     SHORT loc_1_4A9C
        cmp    eax, 42h
        je     SHORT loc_1_4A8C
        cmp    eax, 44h
        je     SHORT loc_1_4A94
        cmp    eax, 46h
        je     SHORT loc_1_4A8C
        int3
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_4A50:
        cmp    eax, 62h
        je     SHORT loc_1_4A8C
        cmp    eax, 64h
        je     SHORT loc_1_4A94
        int3
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_4A68:
        cmp    eax, 68h
        je     SHORT loc_1_4A9C
        cmp    eax, 69h
        je     SHORT loc_1_4A94
        cmp    eax, 6ch
        je     SHORT loc_1_4AD4
        cmp    eax, 77h
        je     SHORT loc_1_4A9C
        cmp    eax, 7ah
        je     SHORT loc_1_4AD4
        int3
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_4A8C:
        mov    eax, 1
        jmp    SHORT loc_1_4AA1
        db  090h
loc_1_4A94:
        mov    eax, 4
        jmp    SHORT loc_1_4AA1
        db  090h
loc_1_4A9C:
        mov    eax, 2
loc_1_4AA1:
        sub    ecx, ecx
        inc    edx
        cmp    byte ptr [edx], 39h
        jg     SHORT loc_1_4AC5
        mov    esi, dword ptr [ebp + 0ch]
loc_1_4AAC:
        mov    bl, byte ptr [edx]
        cmp    bl, 30h
        jl     SHORT loc_1_4AC5
        movsx  edi, bl
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [edi + ecx*2 - 30h]
        inc    edx
        inc    dword ptr [esi]
        cmp    byte ptr [edx], 39h
        jle    SHORT loc_1_4AAC
loc_1_4AC5:
        or     ecx, ecx
        je     SHORT loc_1_4AD6
        imul   eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_4AD4:
        sub    eax, eax
loc_1_4AD6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
sub_1_4ADC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx]
        cmp    al, 5ah
        jg     SHORT loc_1_4B44
        cmp    al, 4fh
        je     SHORT loc_1_4B44
        movsx  eax, al
        sub    eax, 42h
        cmp    eax, 15h
        ja     SHORT loc_1_4B04
        db  0FFh, 024h, 085h, 064h, 04Bh, 000h, 000h
        db  08Dh, 049h, 000h
loc_1_4B04:
        int3
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_4B10:
        push   dword ptr [ebp + 0ch]
        push   ecx
        call   sub_1_4A1C
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_4B24:
        pop    edi
        mov    eax, 4
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_4B30:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_4B38:
        pop    edi
        mov    eax, 2
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_4B44:
        inc    ecx
        cmp    byte ptr [ecx], 39h
        jg     SHORT loc_1_4B5A
        mov    eax, dword ptr [ebp + 0ch]
loc_1_4B4D:
        cmp    byte ptr [ecx], 30h
        jl     SHORT loc_1_4B5A
        inc    dword ptr [eax]
        inc    ecx
        cmp    byte ptr [ecx], 39h
        jle    SHORT loc_1_4B4D
loc_1_4B5A:
        pop    edi
        mov    eax, 4
        pop    esi
        pop    ebx
        leave
        ret
        db  010h, 04Bh, 000h, 000h, 024h, 04Bh, 000h, 000h, 010h, 04Bh, 000h, 000h, 004h, 04Bh, 000h, 000h
        db  004h, 04Bh, 000h, 000h, 004h, 04Bh, 000h, 000h, 004h, 04Bh, 000h, 000h, 004h, 04Bh, 000h, 000h
        db  004h, 04Bh, 000h, 000h, 030h, 04Bh, 000h, 000h, 038h, 04Bh, 000h, 000h, 004h, 04Bh, 000h, 000h
        db  038h, 04Bh, 000h, 000h, 004h, 04Bh, 000h, 000h, 038h, 04Bh, 000h, 000h, 004h, 04Bh, 000h, 000h
        db  004h, 04Bh, 000h, 000h, 004h, 04Bh, 000h, 000h, 038h, 04Bh, 000h, 000h, 004h, 04Bh, 000h, 000h
        db  004h, 04Bh, 000h, 000h, 010h, 04Bh, 000h, 000h
_LTEXT ends
        end
