; nwsp_pcode_03.asm -- NWSP object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

SECPROV_Service_01:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [esp + 24h], 0
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 28h], eax
        sub    eax, 4
        mov    dword ptr [esp + 18h], 0
        mov    ecx, dword ptr [esp + 3ch]
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [ecx], 0
        mov    dword ptr [eax], 4
        mov    esi, dword ptr [esp + 38h]
        push   esi
        call   sub_3_0690
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_3_006B
        call   sub_3_0C05
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        je     SHORT loc_3_0067
        push   esi
        call   sub_3_04A0
        add    esp, 4
        mov    edi, eax
loc_3_0067:
        or     edi, edi
        je     SHORT loc_3_0090
loc_3_006B:
        lea    ebx, [edi + 40h]
        mov    eax, dword ptr [edi + 3ch]
        mov    dword ptr [esp + 1ch], eax
        shl    eax, 2
        cmp    dword ptr [esp + 20h], eax
        jae    SHORT loc_3_0086
        mov    dword ptr [esp + 24h], 2
loc_3_0086:
        sub    dword ptr [esp + 20h], eax
        jmp    SHORT loc_3_009C
        db  08Dh, 064h, 024h, 000h
loc_3_0090:
        mov    dword ptr [esp + 24h], 1
        mov    ebx, dword ptr [esp + 2ch]
loc_3_009C:
        cmp    dword ptr [esp + 24h], 0
        jne    NEAR PTR loc_3_0198
        mov    ebp, dword ptr [esp + 3ch]
        add    ebp, 4
        mov    ecx, dword ptr [esp + 3ch]
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], eax
        lea    eax, [ebp + eax*4]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 1ch]
        dec    dword ptr [esp + 1ch]
        or     eax, eax
        je     NEAR PTR loc_3_0198
loc_3_00D0:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_3_07A0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_0150
        mov    edx, eax
loc_3_00E1:
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [esp + 14h], ecx
        mov    eax, ecx
        cmp    dword ptr [esp + 20h], eax
        jb     NEAR PTR loc_3_0190
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [esp + 10h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    ebx, 4
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebp], eax
        add    ebp, 4
        add    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 14h]
        sub    dword ptr [esp + 20h], eax
loc_3_013E:
        mov    eax, dword ptr [esp + 1ch]
        dec    dword ptr [esp + 1ch]
        or     eax, eax
        jne    SHORT loc_3_00D0
        jmp    SHORT loc_3_0198
        db  08Dh, 064h, 024h, 000h
loc_3_0150:
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_3_0164
        call   sub_3_0C05
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        je     SHORT loc_3_0184
loc_3_0164:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_3_0410
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    NEAR PTR loc_3_00E1
        mov    ecx, dword ptr [esp + 3ch]
        add    ebx, 4
        dec    dword ptr [ecx]
        jmp    SHORT loc_3_013E
loc_3_0184:
        mov    dword ptr [esp + 24h], 4
        jmp    SHORT loc_3_0198
        db  08Bh, 0FFh
loc_3_0190:
        mov    dword ptr [esp + 24h], 2
loc_3_0198:
        mov    ecx, dword ptr [esp + 28h]
        mov    eax, dword ptr [esp + 40h]
        sub    ecx, dword ptr [esp + 20h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 24h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_01C0:
        db  083h, 03Dh, 004h, 005h, 000h, 000h, 000h
        je     SHORT loc_3_01DE
        db  0C7h, 005h, 004h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 010h, 006h, 000h, 000h
        push   eax
        call   sub_3_0F60
loc_3_01DE:
        ret
        db  090h
SECPROV_Service_02:
        push   esi
        push   1
        db  0A1h, 010h, 006h, 000h, 000h
        push   eax
        call   sub_3_0F6C
        call   sub_3_0C05
        db  0A1h, 01Ch, 001h, 000h, 000h
        push   0ch
        mov    word ptr [eax], 100h
        mov    byte ptr [eax + 2], 17h
        db  0A1h, 020h, 001h, 000h, 000h
        push   eax
        push   3
        db  0A1h, 01Ch, 001h, 000h, 000h
        push   eax
        push   17h
        call   sub_3_0E1F
        add    esp, 14h
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_3_0262
        db  0A1h, 020h, 001h, 000h, 000h
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [esp + 0ch]
        push   0
        mov    dword ptr [eax], 8
        push   1388h
        db  068h, 0C0h, 001h, 000h, 000h
        call   sub_3_0F78
        db  0A3h, 008h, 005h, 000h, 000h
        db  0C7h, 005h, 004h, 005h, 000h, 000h, 001h, 000h, 000h, 000h
        mov    eax, esi
        pop    esi
        ret
loc_3_0262:
        db  0A1h, 010h, 006h, 000h, 000h
        push   eax
        call   sub_3_0F60
        mov    eax, esi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
SECPROV_Service_03:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 01Dh, 01Ch, 001h, 000h, 000h
        cmp    dword ptr [esp + 24h], 8
        je     SHORT loc_3_02A0
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_02A0:
        db  080h, 03Dh, 025h, 001h, 000h, 000h, 000h
        jne    SHORT loc_3_02B3
        mov    eax, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_02B3:
        mov    byte ptr [ebx + 2], 4ah
        mov    eax, dword ptr [esp + 20h]
        lea    edx, [ebx + 3]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], eax
        mov    ecx, 0ffffffffh
        mov    word ptr [ebx + 0bh], 100h
        mov    ebp, dword ptr [esp + 18h]
        mov    edi, ebp
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [ebx + 0dh], cl
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 0eh]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    al, byte ptr [ebx + 0dh]
        and    eax, 0ffff00ffh
        add    ax, 0ch
        push   eax
        call   sub_3_0B09
        add    esp, 4
        mov    word ptr [ebx], ax
        push   0
        db  0A1h, 020h, 001h, 000h, 000h
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebx + 0dh]
        add    eax, 0eh
        push   eax
        db  0A1h, 01Ch, 001h, 000h, 000h
        push   eax
        push   17h
        call   sub_3_0E1F
        add    esp, 14h
        or     eax, eax
        mov    eax, dword ptr [esp + 30h]
        je     SHORT loc_3_0350
        mov    word ptr [eax], 5
        jmp    SHORT loc_3_035E
        db  08Dh, 049h, 000h
loc_3_0350:
        mov    word ptr [eax], 0
        push   ebp
        call   sub_3_04A0
        add    esp, 4
loc_3_035E:
        db  083h, 03Dh, 004h, 005h, 000h, 000h, 000h
        je     SHORT loc_3_0387
        db  0C7h, 005h, 004h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 008h, 005h, 000h, 000h
        push   eax
        call   sub_3_0F94
        db  0A1h, 010h, 006h, 000h, 000h
        push   eax
        call   sub_3_0F60
loc_3_0387:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
SECPROV_Service_06:
        mov    eax, 1
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
SECPROV_Service_04:
        mov    eax, dword ptr [esp + 8]
        push   eax
        call   sub_3_0730
        add    esp, 4
        sub    eax, eax
        ret
SECPROV_Service_05:
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], 3
        db  0A1h, 044h, 001h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 040h, 001h, 000h, 000h
        mov    dword ptr [ecx + 8], eax
        db  0A1h, 044h, 001h, 000h, 000h
        mov    dword ptr [ecx + 0ch], eax
        db  0A1h, 040h, 001h, 000h, 000h
        mov    dword ptr [ecx + 10h], eax
        db  0A1h, 048h, 001h, 000h, 000h
        mov    dword ptr [ecx + 14h], eax
        sub    eax, eax
        db  0C7h, 041h, 018h, 00Ch, 005h, 000h, 000h
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_03F0:
        db  0C7h, 005h, 074h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0C0h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0410:
        sub    esp, 4
        push   esi
        db  0A1h, 01Ch, 001h, 000h, 000h
        mov    dword ptr [esp + 4], eax
        mov    word ptr [eax], 500h
        add    dword ptr [esp + 4], 2
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [eax], 36h
        inc    dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 4]
        mov    esi, dword ptr [esp + 0ch]
        mov    dword ptr [eax], esi
        db  0A1h, 018h, 001h, 000h, 000h
        push   eax
        db  0A1h, 020h, 001h, 000h, 000h
        push   eax
        push   7
        db  0A1h, 01Ch, 001h, 000h, 000h
        push   eax
        push   17h
        call   sub_3_0E1F
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_3_0465
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret
loc_3_0465:
        db  0A1h, 020h, 001h, 000h, 000h
        add    eax, 6
        push   eax
        push   eax
        call   sub_3_0AB7
        db  0A1h, 020h, 001h, 000h, 000h
        add    eax, 6
        push   eax
        push   esi
        call   sub_3_07D0
        add    esp, 8
        db  0A1h, 020h, 001h, 000h, 000h
        add    eax, 6
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_04A0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   200h
        push   1d0h
        call   sub_3_0F48
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_3_04D0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_04D0:
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 4]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    dword ptr [ebx + 34h], 0
        call   sub_3_0FA4
        mov    dword ptr [ebx + 38h], eax
        mov    dword ptr [ebx + 3ch], 0
        db  0A1h, 01Ch, 001h, 000h, 000h
        mov    dword ptr [esp + 10h], eax
        add    dword ptr [esp + 10h], 2
        mov    eax, dword ptr [esp + 10h]
        lea    ebp, [ebx + 40h]
        mov    byte ptr [eax], 3dh
        inc    dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, 0ffffffffh
        mov    word ptr [eax], 100h
        add    dword ptr [esp + 10h], 2
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [esp + 10h]
        inc    edi
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [esp + 10h]
        dec    ecx
        mov    byte ptr [eax], cl
        mov    eax, dword ptr [esp + 10h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        inc    eax
        add    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 10h]
        inc    dword ptr [esp + 10h]
        mov    byte ptr [eax], 1
        db  0BAh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax], 0dh
        inc    dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    ax, word ptr [edx + 0ch]
        mov    word ptr [ecx + 0ch], ax
        add    dword ptr [esp + 10h], 0dh
        mov    eax, dword ptr [esp + 10h]
        db  02Bh, 005h, 01Ch, 001h, 000h, 000h
        sub    ax, 2
        push   eax
        call   sub_3_0B09
        db  08Bh, 00Dh, 01Ch, 001h, 000h, 000h
        add    esp, 4
        mov    word ptr [ecx], ax
        db  08Bh, 035h, 020h, 001h, 000h, 000h
loc_3_05E2:
        db  0A1h, 018h, 001h, 000h, 000h
        push   eax
        db  0A1h, 020h, 001h, 000h, 000h
        push   eax
        mov    eax, dword ptr [esp + 18h]
        db  02Bh, 005h, 01Ch, 001h, 000h, 000h
        push   eax
        db  0A1h, 01Ch, 001h, 000h, 000h
        push   eax
        push   17h
        call   sub_3_0E1F
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_3_0638
        cmp    dword ptr [esi], 0
        je     SHORT loc_3_062D
        mov    ecx, esi
loc_3_0614:
        cmp    dword ptr [ebx + 3ch], 64h
        jae    SHORT loc_3_062D
        mov    edx, dword ptr [ecx]
        mov    dword ptr [ebp], edx
        add    ebp, 4
        add    ecx, 4
        inc    dword ptr [ebx + 3ch]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_3_0614
loc_3_062D:
        cmp    byte ptr [esi + 80h], 1
        jne    SHORT loc_3_0641
        jmp    SHORT loc_3_05E2
loc_3_0638:
        cmp    eax, 0ech
        jne    SHORT loc_3_0641
        sub    eax, eax
loc_3_0641:
        or     eax, eax
        jne    SHORT loc_3_0677
        push   200h
        mov    eax, dword ptr [ebx + 3ch]
        lea    eax, [eax*4 + 40h]
        push   eax
        push   ebx
        call   sub_3_0F58
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_0664
        mov    ebx, eax
loc_3_0664:
        push   ebx
        call   sub_3_0700
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_3_0677:
        push   0
        push   ebx
        call   sub_3_0F50
        add    esp, 8
        sub    ebx, ebx
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_3_0690:
        push   ebx
        push   esi
        push   edi
        db  0BAh, 074h, 001h, 000h, 000h
        db  0A1h, 074h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_06F9
        mov    esi, dword ptr [esp + 10h]
loc_3_06A5:
        lea    ecx, [eax + 4]
        mov    edi, esi
loc_3_06AA:
        mov    bl, byte ptr [ecx]
        cmp    bl, byte ptr [edi]
        jne    SHORT loc_3_06CA
        or     bl, bl
        je     SHORT loc_3_06C6
        mov    bl, byte ptr [ecx + 1]
        cmp    bl, byte ptr [edi + 1]
        jne    SHORT loc_3_06CA
        add    ecx, 2
        add    edi, 2
        or     bl, bl
        jne    SHORT loc_3_06AA
loc_3_06C6:
        sub    ecx, ecx
        jmp    SHORT loc_3_06CF
loc_3_06CA:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_3_06CF:
        or     ecx, ecx
        je     SHORT loc_3_06E0
        mov    edx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_3_06A5
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_3_06E0:
        db  03Bh, 005h, 074h, 001h, 000h, 000h
        je     SHORT loc_3_06F9
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        db  08Bh, 00Dh, 074h, 001h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 074h, 001h, 000h, 000h
loc_3_06F9:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_3_0700:
        push   esi
        mov    esi, dword ptr [esp + 8]
        lea    eax, [esi + 4]
        push   eax
        call   sub_3_0730
        add    esp, 4
        db  0A1h, 074h, 001h, 000h, 000h
        mov    dword ptr [esi], eax
        db  089h, 035h, 074h, 001h, 000h, 000h
        mov    eax, esi
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0730:
        push   ebx
        push   esi
        push   edi
        db  0B9h, 074h, 001h, 000h, 000h
        db  08Bh, 015h, 074h, 001h, 000h, 000h
        or     edx, edx
        je     SHORT loc_3_078F
        mov    esi, dword ptr [esp + 10h]
loc_3_0746:
        lea    eax, [edx + 4]
        mov    edi, esi
loc_3_074B:
        mov    bl, byte ptr [eax]
        cmp    bl, byte ptr [edi]
        jne    SHORT loc_3_076B
        or     bl, bl
        je     SHORT loc_3_0767
        mov    bl, byte ptr [eax + 1]
        cmp    bl, byte ptr [edi + 1]
        jne    SHORT loc_3_076B
        add    eax, 2
        add    edi, 2
        or     bl, bl
        jne    SHORT loc_3_074B
loc_3_0767:
        sub    eax, eax
        jmp    SHORT loc_3_0770
loc_3_076B:
        sbb    eax, eax
        sbb    eax, -1
loc_3_0770:
        or     eax, eax
        je     SHORT loc_3_0780
        mov    ecx, edx
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    SHORT loc_3_0746
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_0780:
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        push   0
        push   edx
        call   sub_3_0F50
        add    esp, 8
loc_3_078F:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_07A0:
        db  0B8h, 0C4h, 001h, 000h, 000h
        sub    edx, edx
        db  03Bh, 015h, 0C0h, 001h, 000h, 000h
        jae    SHORT loc_3_07CA
        mov    ecx, dword ptr [esp + 4]
loc_3_07B3:
        cmp    dword ptr [eax + 30h], ecx
        je     SHORT loc_3_07C4
        inc    edx
        add    eax, 34h
        db  03Bh, 015h, 0C0h, 001h, 000h, 000h
        jb     SHORT loc_3_07B3
loc_3_07C4:
        db  03Bh, 015h, 0C0h, 001h, 000h, 000h
loc_3_07CA:
        jne    SHORT loc_3_07CE
        sub    eax, eax
loc_3_07CE:
        ret
        db  090h
sub_3_07D0:
        push   esi
        push   edi
        db  083h, 03Dh, 0C0h, 001h, 000h, 000h, 010h
        jb     SHORT loc_3_07E0
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_3_07E0:
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        call   sub_3_07A0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_0833
        db  0A1h, 0C0h, 001h, 000h, 000h
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        lea    eax, [ecx + eax*4]
        db  08Dh, 014h, 085h, 0C4h, 001h, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        mov    dword ptr [edx + 30h], esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0FFh, 005h, 0C0h, 001h, 000h, 000h
        mov    eax, edx
loc_3_0833:
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0840:
        sub    esp, 70h
        push   esi
        push   edi
        call   sub_3_0C05
        mov    byte ptr [esp + 0ah], 35h
        mov    word ptr [esp + 0bh], 100h
        mov    edx, dword ptr [esp + 7ch]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [esp + 0dh], cl
        cmp    cl, 30h
        jbe    SHORT loc_3_0879
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 70h
        ret
loc_3_0879:
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 0eh]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  esi, byte ptr [esp + 0dh]
        lea    eax, [esi + 4]
        add    esi, 6
        push   eax
        call   sub_3_0B09
        mov    word ptr [esp + 0ch], ax
        add    esp, 4
        lea    eax, [esp + 40h]
        push   36h
        push   eax
        lea    eax, [esp + 10h]
        push   esi
        push   eax
        push   17h
        call   sub_3_0E1F
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_3_08D8
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 70h
        ret
loc_3_08D8:
        mov    eax, dword ptr [esp + 40h]
        pop    edi
        pop    esi
        add    esp, 70h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_08F0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 4]
        push   ecx
        mov    eax, dword ptr [eax]
        push   eax
        call   SECPROV_Service_02
        add    esp, 8
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0910:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 1ch], 4
        je     SHORT loc_3_0928
        mov    eax, 32h
        pop    esi
        add    esp, 4
        ret
loc_3_0928:
        lea    eax, [esp + 6]
        push   eax
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    eax, dword ptr [esi + 10h]
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   SECPROV_Service_03
        add    esp, 20h
        xor    edx, edx
        mov    ecx, dword ptr [esi + 20h]
        mov    dx, word ptr [esp + 6]
        pop    esi
        mov    dword ptr [ecx], edx
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0970:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 8]
        push   eax
        mov    eax, dword ptr [ecx + 4]
        push   eax
        mov    eax, dword ptr [ecx]
        push   eax
        call   SECPROV_Service_04
        add    esp, 0ch
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0990:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx], 4
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], 3
        sub    eax, eax
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_09B0:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edx + 4]
        db  08Bh, 00Dh, 044h, 001h, 000h, 000h
        cmp    dword ptr [eax], ecx
        jae    SHORT loc_3_09D0
        mov    eax, 6fh
        pop    edi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_09D0:
        db  0A1h, 044h, 001h, 000h, 000h
        mov    edi, dword ptr [edx]
        db  08Bh, 035h, 040h, 001h, 000h, 000h
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [edx + 4]
        db  0A1h, 044h, 001h, 000h, 000h
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_0A00:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_3_0840
        add    esp, 4
        mov    dword ptr [esi + 4], eax
        or     eax, eax
        jne    SHORT loc_3_0A20
        mov    eax, 1
        pop    esi
        ret
        db  08Bh, 0FFh
loc_3_0A20:
        sub    eax, eax
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0A30:
        mov    eax, 1
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0A40:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 24h]
        sub    eax, eax
        mov    ebx, dword ptr [edi]
        mov    dword ptr [edi], eax
        mov    edx, dword ptr [esp + 18h]
        cmp    dword ptr [esp + 1ch], eax
        je     SHORT loc_3_0AA8
loc_3_0A58:
        mov    esi, dword ptr [edx + 4]
        mov    edi, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esi + 4]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edx], ecx
        add    dword ptr [edi], ecx
        mov    ebp, dword ptr [edx]
        cmp    ebx, ebp
        jb     SHORT loc_3_0A93
        mov    edi, dword ptr [esp + 20h]
        mov    esi, dword ptr [esi]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [edx]
        add    dword ptr [esp + 20h], ecx
        sub    ebx, dword ptr [edx]
        jmp    SHORT loc_3_0A9F
loc_3_0A93:
        mov    dword ptr [edx + 8], 0
        mov    eax, 0eah
loc_3_0A9F:
        add    edx, 0ch
        dec    dword ptr [esp + 1ch]
        jne    SHORT loc_3_0A58
loc_3_0AA8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
SECPROV_Service_00:
        mov    eax, 400h
        clc
        ret
sub_3_0AB7:
        db  0CDh, 020h, 00Fh, 080h, 091h, 004h
        push   esi
        push   edi
        cld
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
loc_3_0AC8:
        lodsb
        stosb
        or     al, al
        jne    SHORT loc_3_0AC8
        pop    edi
        pop    esi
        ret
sub_3_0AD1:
        push   esi
        push   edi
        cld
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
loc_3_0ADC:
        mov    al, byte ptr [edi]
        cmp    al, byte ptr [esi]
        jne    SHORT loc_3_0AED
        inc    edi
        inc    esi
        or     al, al
        jne    SHORT loc_3_0ADC
        sub    eax, eax
loc_3_0AEA:
        pop    edi
        pop    esi
        ret
loc_3_0AED:
        mov    eax, 0ffffffffh
        jmp    SHORT loc_3_0AEA
        db  057h, 08Bh, 07Ch, 024h, 008h, 02Bh, 0C0h, 08Bh, 0C8h, 049h, 0F2h, 0AEh, 0F7h, 0D9h, 083h, 0E9h
        db  002h, 08Bh, 0C1h, 05Fh, 0C3h
sub_3_0B09:
        mov    eax, dword ptr [esp + 4]
        xchg   al, ah
        ret
sub_3_0B10:
        enter  34h, 0
        lea    ebx, [ebp - 34h]
        db  0C7h, 003h, 018h, 006h, 000h, 000h
        mov    dword ptr [ebx + 1ch], 5f03h
        mov    dword ptr [ebx + 10h], 0
        mov    word ptr [ebx + 14h], 0
        push   ebx
        db  0CDh, 020h, 001h, 000h, 08Eh, 004h
        pop    eax
        mov    ax, word ptr [ebx + 2ch]
        test   ax, 1
        jne    SHORT loc_3_0B6F
        mov    eax, dword ptr [ebx + 1ch]
        shr    eax, 10h
        db  0A2h, 025h, 001h, 000h, 000h
        db  066h, 0C7h, 005h, 026h, 001h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 1ch], 5f03h
        mov    word ptr [ebx + 14h], 203h
        push   ebx
        db  0CDh, 020h, 001h, 000h, 08Eh, 004h
        pop    eax
        clc
loc_3_0B6D:
        leave
        ret
loc_3_0B6F:
        db  0C6h, 005h, 025h, 001h, 000h, 000h, 000h
        stc
        jmp    SHORT loc_3_0B6D
sub_3_0B79:
        enter  34h, 0
        lea    ebx, [ebp - 34h]
        db  0C7h, 003h, 018h, 006h, 000h, 000h
        mov    dword ptr [ebx + 1ch], 5f03h
        mov    dword ptr [ebx + 10h], 0
        mov    word ptr [ebx + 14h], 201h
        push   ebx
        db  0CDh, 020h, 001h, 000h, 08Eh, 004h
        pop    eax
        mov    ax, word ptr [ebx + 2ch]
        test   ax, 1
        jne    SHORT loc_3_0BC3
        mov    eax, dword ptr [ebx + 1ch]
        shr    eax, 10h
        db  0A2h, 025h, 001h, 000h, 000h
        db  066h, 0C7h, 005h, 026h, 001h, 000h, 000h, 000h, 002h
        clc
loc_3_0BC1:
        leave
        ret
loc_3_0BC3:
        db  0C6h, 005h, 025h, 001h, 000h, 000h, 000h
        stc
        jmp    SHORT loc_3_0BC1
sub_3_0BCD:
        enter  34h, 0
        push   ebx
        db  0C6h, 005h, 025h, 001h, 000h, 000h, 000h
        lea    ebx, [ebp - 34h]
        db  0C7h, 003h, 018h, 006h, 000h, 000h
        mov    dword ptr [ebx + 1ch], 5f03h
        mov    dword ptr [ebx + 10h], 0
        mov    word ptr [ebx + 14h], 203h
        push   ebx
        db  0CDh, 020h, 001h, 000h, 08Eh, 004h
        pop    ebx
        mov    ax, word ptr [ebx + 2ch]
        pop    ebx
        leave
        ret
sub_3_0C05:
        enter  34h, 0
        push   esi
        push   edi
        push   ebx
        db  0A1h, 008h, 001h, 000h, 000h
        mov    ecx, 5
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        db  00Fh, 0B6h, 005h, 025h, 001h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0C76
        lea    ebx, [ebp - 34h]
        shl    eax, 10h
        mov    ax, 0e9feh
        mov    dword ptr [ebx + 1ch], eax
        db  0A1h, 01Ch, 001h, 000h, 000h
        mov    dword ptr [ebx], eax
        push   ebx
        db  0CDh, 020h, 001h, 000h, 08Eh, 004h
        add    esp, 4
        lea    ebx, [ebp - 34h]
        mov    ax, word ptr [ebx + 2ch]
        test   ax, 1
        jne    SHORT loc_3_0C76
        db  0FFh, 035h, 01Ch, 001h, 000h, 000h
        db  0FFh, 035h, 040h, 001h, 000h, 000h
        call   sub_3_0AD1
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_0C76
        db  066h, 083h, 03Dh, 026h, 001h, 000h, 000h, 000h
        je     SHORT loc_3_0C84
        jmp    SHORT loc_3_0CA5
loc_3_0C76:
        call   sub_3_0B10
        jae    SHORT loc_3_0C84
        call   sub_3_0B79
        jb     SHORT loc_3_0CBC
loc_3_0C84:
        call   sub_3_0D40
        mov    edx, dword ptr [eax]
        or     edx, edx
        jne    SHORT loc_3_0CA5
        db  066h, 083h, 03Dh, 026h, 001h, 000h, 000h, 000h
        jne    SHORT loc_3_0CA0
        call   sub_3_0B79
        jb     SHORT loc_3_0CBC
loc_3_0CA0:
        call   sub_3_0DDC
loc_3_0CA5:
        db  0A1h, 008h, 001h, 000h, 000h
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        db  00Fh, 0B6h, 005h, 025h, 001h, 000h, 000h
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_3_0CBC:
        jmp    SHORT loc_3_0CA5
loc_3_0CBE:
        ret
        db  057h, 056h, 08Bh, 03Dh, 01Ch, 001h, 000h, 000h, 0C6h, 047h, 002h, 03Dh, 083h, 0C7h, 003h, 066h
        db  0C7h, 007h, 000h, 004h, 08Bh, 00Dh, 044h, 001h, 000h, 000h, 049h, 088h, 04Fh, 002h, 083h, 0C7h
        db  003h, 08Bh, 035h, 040h, 001h, 000h, 000h, 0F3h, 0A4h, 0C6h, 007h, 001h, 0B9h, 00Bh, 000h, 000h
        db  000h, 047h, 088h, 00Fh, 047h, 0BEh, 030h, 001h, 000h, 000h, 0F3h, 0A4h, 08Bh, 035h, 01Ch, 001h
        db  000h, 000h, 02Bh, 0FEh, 066h, 08Bh, 0C7h, 066h, 083h, 0E8h, 002h, 086h, 0E0h, 066h, 089h, 006h
        db  0FFh, 035h, 018h, 001h, 000h, 000h, 0FFh, 035h, 020h, 001h, 000h, 000h, 057h, 0FFh, 035h, 01Ch
        db  001h, 000h, 000h, 0B8h, 017h, 000h, 000h, 000h, 050h, 0E8h, 0F2h, 000h, 000h, 000h, 083h, 0C4h
        db  014h, 00Bh, 0C0h, 075h, 008h, 0A1h, 020h, 001h, 000h, 000h, 05Eh, 05Fh, 0C3h, 02Bh, 0C0h, 0EBh
        db  0F9h
sub_3_0D40:
        enter  34h, 0
        lea    edi, [ebp - 34h]
        db  00Fh, 0B6h, 005h, 025h, 001h, 000h, 000h
        shl    eax, 10h
        mov    ax, 0e9ffh
        mov    dword ptr [edi + 1ch], eax
        db  0A1h, 020h, 001h, 000h, 000h
        mov    dword ptr [edi], eax
        push   edi
        db  0CDh, 020h, 001h, 000h, 08Eh, 004h
        pop    eax
        mov    ax, word ptr [edi + 2ch]
        and    ax, 1
        jne    SHORT loc_3_0DB5
        db  08Bh, 03Dh, 020h, 001h, 000h, 000h
        db  0A1h, 01Ch, 001h, 000h, 000h
        cmp    byte ptr [edi + 1ch], 0
        jne    SHORT loc_3_0DC4
        mov    cl, byte ptr [edi + 1bh]
        mov    byte ptr [eax + 2], 16h
        mov    word ptr [eax], 200h
        mov    byte ptr [eax + 3], cl
        mov    edx, 4
loc_3_0D96:
        db  0FFh, 035h, 018h, 001h, 000h, 000h
        db  0FFh, 035h, 020h, 001h, 000h, 000h
        push   edx
        db  0FFh, 035h, 01Ch, 001h, 000h, 000h
        push   17h
        call   sub_3_0E1F
        add    esp, 14h
        or     eax, eax
loc_3_0DB5:
        db  0A1h, 020h, 001h, 000h, 000h
        je     SHORT loc_3_0DC2
        mov    dword ptr [eax], 0
loc_3_0DC2:
        leave
        ret
loc_3_0DC4:
        mov    byte ptr [eax + 2], 1ch
        mov    word ptr [eax], 500h
        mov    cx, word ptr [edi + 1bh]
        mov    word ptr [eax + 3], cx
        mov    edx, 7
        jmp    SHORT loc_3_0D96
sub_3_0DDC:
        enter  34h, 0
        push   ebx
        db  0B8h, 078h, 001h, 000h, 000h
        lea    edx, [ebp - 34h]
        mov    dword ptr [edx], eax
        db  0B8h, 03Ch, 001h, 000h, 000h
        mov    dword ptr [edx + 4], eax
        mov    ax, 5f03h
        mov    dword ptr [edx + 1ch], eax
        mov    byte ptr [edx + 10h], 0
        mov    word ptr [edx + 18h], 1
        mov    word ptr [edx + 14h], 201h
        push   edx
        db  0CDh, 020h, 001h, 000h, 08Eh, 004h
        pop    edx
        movzx  eax, word ptr [edx + 2ch]
        and    al, 1
        or     al, al
        jne    SHORT loc_3_0E1C
loc_3_0E1C:
        pop    ebx
        leave
        ret
sub_3_0E1F:
        enter  34h, 0
        push   ebx
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  0A3h, 028h, 001h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        lea    ebx, [ebp - 34h]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [ebx + 18h], ax
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [ebx + 14h], ax
        db  00Fh, 0B6h, 005h, 025h, 001h, 000h, 000h
        shl    eax, 10h
        mov    ah, 0f2h
        mov    al, byte ptr [ebp + 8]
        mov    dword ptr [ebx + 1ch], eax
        push   ebx
        db  0CDh, 020h, 001h, 000h, 08Eh, 004h
        pop    eax
        db  0A1h, 02Ch, 001h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_3_0E97
        db  066h, 0A1h, 026h, 001h, 000h, 000h
        cmp    ax, 0
        je     SHORT loc_3_0E97
        mov    eax, 0ea60h
        mov    edx, 0
        push   esi
        db  0BEh, 09Eh, 00Eh, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 02Ch, 001h, 000h, 000h
        pop    esi
loc_3_0E97:
        movzx  eax, byte ptr [ebx + 1ch]
        pop    ebx
        leave
        ret
loc_3_0E9E:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  02Bh, 005h, 028h, 001h, 000h, 000h
        cmp    eax, 7530h
        ja     SHORT loc_3_0ECF
        mov    eax, 0ea60h
        mov    edx, 0
        push   esi
        db  0BEh, 09Eh, 00Eh, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 02Ch, 001h, 000h, 000h
        pop    esi
        ret
loc_3_0ECF:
        db  0C7h, 005h, 02Ch, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0
        push   eax
        push   eax
        db  068h, 0EFh, 00Eh, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        ret
loc_3_0EEF:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  02Bh, 005h, 028h, 001h, 000h, 000h
        cmp    eax, 7530h
        jbe    SHORT loc_3_0F07
        call   sub_3_0BCD
loc_3_0F07:
        ret
SECPROV_W32_DEVICEIOCONTROL:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, 0
        jne    SHORT loc_3_0F17
        mov    ecx, 400h
        jmp    SHORT loc_3_0F1C
loc_3_0F17:
        cmp    ecx, -1
        jne    SHORT loc_3_0F20
loc_3_0F1C:
        xor    eax, eax
        clc
        ret
loc_3_0F20:
        cmp    ecx, 64h
        jl     SHORT loc_3_0F3F
        cmp    ecx, 6ch
        jge    SHORT loc_3_0F3F
        sub    ecx, 64h
        push   dword ptr [esi + 14h]
        push   dword ptr [esi + 10h]
        db  0FFh, 014h, 08Dh, 010h, 000h, 000h, 000h
        add    esp, 8
        clc
        ret
loc_3_0F3F:
        mov    eax, 1
        clc
        ret
        db  000h, 000h
sub_3_0F48:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_3_0F50:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_3_0F58:
        db  0CDh, 020h, 050h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_3_0F60:
        pop    edx
        pop    eax
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        jmp    edx
        db  000h, 000h
sub_3_0F6C:
        pop    edx
        pop    eax
        pop    ecx
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        jmp    edx
        db  000h
sub_3_0F78:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  000h, 000h, 000h
sub_3_0F94:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
        db  000h
sub_3_0FA4:
        db  0CDh, 020h, 0CFh, 080h, 001h, 000h
_PTEXT ends
        end
