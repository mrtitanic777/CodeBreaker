; dosmgr_pcode_19.asm -- DOSMGR object 19 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

DOSMGR_V86_API:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 6
        jae    SHORT loc_19_0012
        db  0FFh, 024h, 085h, 098h, 000h, 000h, 000h
loc_19_0012:
        or     word ptr [ebp + 1ch], 0ffffh
        or     byte ptr [ebp + 2ch], 1
        ret
loc_19_001C:
        db  0CDh, 020h, 000h, 000h, 015h, 000h
        mov    word ptr [ebp + 1ch], ax
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_19_002B:
        db  0E8h, 0C8h, 002h, 0EDh, 0FFh
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_19_0035:
        mov    eax, 10007h
        db  08Dh, 015h, 0B0h, 001h, 000h, 000h
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        db  0CDh, 020h, 02Fh, 080h, 001h, 000h
loc_19_004F:
        db  08Bh, 015h, 0F8h, 001h, 000h, 000h
        push   5
        pop    ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        mov    byte ptr [edx + 1], 0ffh
        inc    byte ptr [edx]
        ret
loc_19_0065:
        or     byte ptr [ebp + 2ch], 1
        ret
loc_19_006A:
        db  080h, 03Dh, 01Ch, 000h, 000h, 000h, 000h
        je     SHORT loc_19_0082
        or     byte ptr [ebp + 2ch], 1
        movzx  eax, byte ptr [ebp + 10h]
        call   sub_19_0087
        jb     SHORT loc_19_0086
loc_19_0082:
        and    byte ptr [ebp + 2ch], 0feh
loc_19_0086:
        ret
sub_19_0087:
        pushad
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        add    edi, ebx
        xor    esi, esi
        xchg   dword ptr [edi + 28h], esi
        db  0CDh, 020h, 013h, 000h, 001h, 000h
        xor    ecx, ecx
        db  039h, 00Dh, 028h, 000h, 000h, 000h
        je     NEAR PTR loc_19_012E
        db  039h, 00Dh, 020h, 000h, 000h, 000h
        je     SHORT loc_19_012E
        db  039h, 00Dh, 024h, 000h, 000h, 000h
        je     SHORT loc_19_012E
        cmp    eax, 1ah
        jae    SHORT loc_19_00C2
        btr    dword ptr [edi + 24h], eax
loc_19_00C2:
        call   sub_19_014A
        db  03Bh, 005h, 028h, 000h, 000h, 000h
        jae    SHORT loc_19_0147
        db  08Bh, 035h, 020h, 000h, 000h, 000h
        push   eax
        db  0F7h, 025h, 024h, 000h, 000h, 000h
        lea    esi, [eax + esi]
        pop    eax
        test   byte ptr [esi + 44h], 40h
        je     SHORT loc_19_0147
        test   byte ptr [esi + 44h], 0a0h
        jne    SHORT loc_19_0147
        mov    ecx, ebx
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, ecx
        je     SHORT loc_19_012E
loc_19_00F8:
        test   dword ptr [ebx], 10h
        jne    SHORT loc_19_0124
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        add    edi, ebx
        bts    dword ptr [edi + 24h], eax
        test   dword ptr [ebx], 8
        jne    SHORT loc_19_0131
        xor    esi, esi
        xchg   dword ptr [edi + 28h], esi
        db  0CDh, 020h, 013h, 000h, 001h, 000h
        call   sub_19_014A
loc_19_0124:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, ecx
        jne    SHORT loc_19_00F8
loc_19_012E:
        clc
loc_19_012F:
        popad
        ret
loc_19_0131:
        cmp    dword ptr [edi + 28h], 0
        jne    SHORT loc_19_0124
        db  0BEh, 04Ah, 001h, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
        mov    dword ptr [edi + 28h], esi
        jmp    SHORT loc_19_0124
loc_19_0147:
        stc
        jmp    SHORT loc_19_012F
sub_19_014A:
        pushad
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        add    edi, ebx
        mov    dword ptr [edi + 28h], 0
        db  08Bh, 00Dh, 028h, 000h, 000h, 000h
        xor    eax, eax
        db  08Bh, 035h, 020h, 000h, 000h, 000h
        add    esi, dword ptr [ebx + 4]
loc_19_016B:
        btr    dword ptr [edi + 24h], eax
        jae    SHORT loc_19_0183
        test   byte ptr [esi + 44h], 40h
        je     SHORT loc_19_0183
        test   byte ptr [esi + 44h], 0a0h
        jne    SHORT loc_19_0183
        mov    word ptr [esi + 49h], 0ffffh
loc_19_0183:
        db  003h, 035h, 024h, 000h, 000h, 000h
        inc    eax
        loop   loc_19_016B
        popad
        ret
        db  000h, 000h
loc_19_0190:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        or     byte ptr [ebp + 2ch], 1
        ret
loc_19_019B:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        test   byte ptr [edi + ebx], 20h
        je     SHORT loc_19_01C4
        db  0A1h, 00Ch, 000h, 000h, 000h
        mov    edi, dword ptr [ebx + 4]
        cmp    dword ptr [edi + 8ch], eax
        jne    SHORT loc_19_01C6
        mov    eax, dword ptr [ebp + 2ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        or     byte ptr [ebp + 2ch], 1
loc_19_01C4:
        clc
        ret
loc_19_01C6:
        stc
        ret
loc_19_01C8:
        db  03Bh, 01Dh, 050h, 000h, 000h, 000h
        je     SHORT loc_19_0214
        db  083h, 03Dh, 050h, 000h, 000h, 000h, 000h
        jne    SHORT loc_19_0225
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        db  089h, 01Dh, 050h, 000h, 000h, 000h
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        db  0BEh, 02Bh, 002h, 000h, 000h
        mov    eax, 3e8h
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
        db  066h, 08Bh, 00Dh, 066h, 000h, 000h, 000h
        db  066h, 08Bh, 015h, 064h, 000h, 000h, 000h
        db  0CDh, 020h, 04Ch, 080h, 001h, 000h
loc_19_0214:
        db  0A1h, 064h, 000h, 000h, 000h
        mov    word ptr [ebp + 24h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 28h], ax
        ret
loc_19_0225:
        db  0CDh, 020h, 047h, 080h, 001h, 000h
loc_19_022B:
        db  0C7h, 005h, 050h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 020h, 080h, 001h, 000h
loc_19_023B:
        db  003h, 01Dh, 000h, 000h, 000h, 000h
        test   byte ptr [ebx], 20h
        je     SHORT loc_19_0248
        stc
        ret
loc_19_0248:
        clc
        ret
loc_19_024A:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        db  08Bh, 015h, 08Ch, 001h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        add    edx, dword ptr [ebx + 4]
        or     byte ptr [eax + ebx], 30h
        movzx  edx, word ptr [edx + 330h]
        cmp    word ptr [eax + ebx + 2], 0
        jne    SHORT loc_19_0276
        mov    word ptr [eax + ebx + 2], dx
loc_19_0276:
        mov    esi, edx
        shl    esi, 4
        add    esi, dword ptr [ebx + 4]
        db  0CDh, 020h, 00Dh, 080h, 015h, 000h
DOSMGR_Begin_V86_App:
        ret
        db  000h, 000h, 000h
loc_19_0288:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        and    byte ptr [edx + ebx + 1], 0f7h
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        sub    eax, eax
        call   sub_19_02A1
        ret
sub_19_02A1:
        push   ecx
        push   edx
        push   esi
        push   edi
        sub    esi, esi
        db  08Bh, 00Dh, 004h, 001h, 000h, 000h
        jecxz  loc_19_030C
        or     eax, eax
        mov    edx, 0fffffffdh
        je     SHORT loc_19_02BE
        or     eax, 0ffffffffh
        xchg   edx, eax
        not    eax
loc_19_02BE:
        db  08Bh, 03Dh, 008h, 001h, 000h, 000h
        db  081h, 03Ch, 0B7h, 044h, 003h, 000h, 000h
        je     SHORT loc_19_0303
        cmp    esi, 0a0h
        jb     SHORT loc_19_02E0
        mov    edi, dword ptr [edi + esi*4]
        db  03Bh, 03Dh, 010h, 001h, 000h, 000h
        jne    SHORT loc_19_0303
loc_19_02E0:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [edi + ebx + 54h]
        test   eax, eax
        jne    SHORT loc_19_02F5
        bts    dword ptr [edi], esi
        jae    SHORT loc_19_02FA
        jmp    SHORT loc_19_0303
loc_19_02F5:
        btr    dword ptr [edi], esi
        jae    SHORT loc_19_0303
loc_19_02FA:
        mov    edi, dword ptr [ebx - 10h]
        and    dword ptr [edi + esi*4], edx
        or     dword ptr [edi + esi*4], eax
loc_19_0303:
        inc    esi
        loop   loc_19_02BE
        mov    eax, cr3
        mov    cr3, eax
loc_19_030C:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        ret
loc_19_0311:
        mov    eax, dword ptr [edi + 4]
        mov    esi, dword ptr [edi]
        db  03Bh, 005h, 004h, 001h, 000h, 000h
        cmc
        jb     SHORT loc_19_033A
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [edi + ebx + 54h]
        cmp    ecx, 1
        jb     SHORT loc_19_033A
        bt     dword ptr [ecx], eax
        cmc
        jb     SHORT loc_19_033A
        call   sub_19_0357
        clc
loc_19_033A:
        ret
loc_19_033B:
        db  08Bh, 03Dh, 008h, 001h, 000h, 000h
        jmp    dword ptr [edi + eax*4]
loc_19_0344:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [edi + ebx + 54h]
        test   ecx, ecx
        je     SHORT loc_19_033B
        bt     dword ptr [ecx], eax
        jae    SHORT loc_19_033B
sub_19_0357:
        test   byte ptr [edi + ebx + 1], 8
        jne    NEAR PTR loc_19_04C4
        mov    ecx, dword ptr [ebx - 10h]
        bts    dword ptr [ecx + eax*4], 0
        jae    NEAR PTR loc_19_04D2
        test   dword ptr [ebp + 2ch], 20000h
        je     NEAR PTR loc_19_04C4
        movzx  ecx, word ptr [ebp + 28h]
        shl    ecx, 4
        add    ecx, dword ptr [ebp + 24h]
        cmp    word ptr [ecx], 4730h
        jne    SHORT loc_19_0398
        cmp    byte ptr [ebp + 1ch], 0
        je     NEAR PTR loc_19_04C4
loc_19_0398:
        shr    ecx, 0ch
        db  08Bh, 015h, 008h, 001h, 000h, 000h
        db  081h, 03Ch, 08Ah, 044h, 003h, 000h, 000h
        jne    NEAR PTR loc_19_04C4
        movzx  ecx, word ptr [ebp + 34h]
        shl    ecx, 4
        add    ecx, dword ptr [ebp + 30h]
        shr    ecx, 0ch
        db  081h, 03Ch, 08Ah, 044h, 003h, 000h, 000h
        jne    NEAR PTR loc_19_04C4
        db  08Bh, 015h, 00Ch, 001h, 000h, 000h
        or     edx, edx
        je     SHORT loc_19_03E1
        mov    ecx, dword ptr [edx]
loc_19_03D4:
        add    edx, 4
        cmp    esi, dword ptr [edx]
        je     NEAR PTR loc_19_04C4
        loop   loc_19_03D4
loc_19_03E1:
        push   eax
        test   byte ptr [edi + ebx + 1], 20h
        jne    SHORT loc_19_03FD
        push   1
        push   esi
        db  0CDh, 020h, 039h, 001h, 001h, 000h
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_19_04C3
loc_19_03FD:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     NEAR PTR loc_19_04C3
        mov    eax, ebx
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        xchg   ebx, eax
        jecxz  loc_19_0418
        cmp    eax, ebx
        je     SHORT loc_19_0491
loc_19_0418:
        mov    eax, dword ptr [edi + ebx + 58h]
        or     eax, eax
        jne    SHORT loc_19_0432
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        or     eax, eax
        je     NEAR PTR loc_19_04C3
        mov    dword ptr [edi + ebx + 58h], eax
loc_19_0432:
        mov    edx, dword ptr [edi + ebx + 5ch]
        or     edx, edx
        jne    SHORT loc_19_0454
        push   100h
        push   10h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    edx, eax
        or     eax, eax
        je     SHORT loc_19_04C3
        mov    dword ptr [edi + ebx + 5ch], eax
loc_19_0454:
        mov    dword ptr [edx], ebx
        movzx  eax, word ptr [ebp + 28h]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ebp + 24h]
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx + 0ch], esi
        sub    eax, eax
        mov    ecx, 427h
        db  0BEh, 0D9h, 004h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        jb     SHORT loc_19_04C3
        mov    eax, dword ptr [edi + ebx + 58h]
        mov    ecx, 5
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        cmp    dword ptr [edx], 0
        je     SHORT loc_19_04C3
        mov    esi, dword ptr [edx + 0ch]
loc_19_0491:
        db  0A1h, 00Ch, 001h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_19_04B6
        push   101h
        push   84h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_19_04C3
        db  0A3h, 00Ch, 001h, 000h, 000h
loc_19_04B6:
        cmp    dword ptr [eax], 20h
        jae    SHORT loc_19_04C3
        mov    ecx, dword ptr [eax]
        mov    dword ptr [eax + ecx*4 + 4], esi
        inc    dword ptr [eax]
loc_19_04C3:
        pop    eax
loc_19_04C4:
        mov    ecx, dword ptr [ebx - 10h]
        or     byte ptr [ecx + eax*4], 2
        mov    ecx, dword ptr [edi + ebx + 54h]
        btr    dword ptr [ecx], eax
loc_19_04D2:
        mov    ecx, cr3
        mov    cr3, ecx
        ret
loc_19_04D9:
        mov    ebx, dword ptr [edx]
        jb     SHORT loc_19_0506
        cmp    dword ptr [ebp + 1ch], 7
        jne    SHORT loc_19_0506
        mov    dword ptr [edx], 0
        or     cl, 1
        mov    eax, 10006h
        sub    edx, edx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        sub    ecx, ecx
        mov    eax, 1388h
        db  0CDh, 020h, 0ECh, 000h, 001h, 000h
loc_19_0506:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [edi + ebx + 58h]
        db  0CDh, 020h, 028h, 080h, 001h, 000h
        add    byte ptr [eax], al
loc_19_0518:
        mov    eax, 23h
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        je     SHORT loc_19_0547
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    si, word ptr [ebp + 30h]
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    si, word ptr [ebp + 30h]
        je     SHORT loc_19_0541
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
loc_19_0541:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
loc_19_0547:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        ret
        db  024h, 044h
_PTEXT ends
        end
